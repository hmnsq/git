#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#include "builtin.h"
#include "config.h"
#include "environment.h"
#include "gettext.h"
#include "hash.h"
#include "hex.h"
#include "pretty.h"
#include "refs.h"
#include "color.h"
#include "strvec.h"
#include "object-name.h"
#include "parse-options.h"

#include "dir.h"
#include "commit-slab.h"
#include "date.h"
#include "wildmatch.h"
#include "prio-queue.h"

static const char*const show_branch_usage[] = {
    N_("git show-branch [-a | --all] [-r | --remotes] [--topo-order | --date-order]\n"
       "                [--current] [--color[=<when>] | --no-color] [--sparse]\n"
       "                [--more=<n> | --list | --independent | --merge-base]\n"
       "                [--no-name | --sha1-name] [--topics]\n"
       "                [(<rev> | <glob>)...]"),
    N_("git show-branch (-g | --reflog)[=<n>[,<base>]] [--list] [<ref>]"),
    NULL
};

static enum git_colorbool showbranch_use_color = GIT_COLOR_UNKNOWN;

static struct strvec default_args = STRVEC_INIT;

/*
 * TODO: convert this use of commit->object.flags to commit-slab
 * instead to store a pointer to ref name directly. Then use the same
 * UNINTERESTING definition from revision.h here.
 */
#define UNINTERESTING	01

#define REV_SHIFT	 2
#define MAX_REVS	(FLAG_BITS - REV_SHIFT) /* should not exceed bits_per_int - REV_SHIFT */

#define DEFAULT_REFLOG	4

static const char *get_color_code(цяло idx)
{
	ако (want_color(showbranch_use_color))
		върни column_colors_ansi[idx % column_colors_ansi_max];
	върни "";
}

static const char *get_color_reset_code(void)
{
	ако (want_color(showbranch_use_color))
		върни GIT_COLOR_RESET;
	върни "";
}

static struct commit *interesting(struct prio_queue *queue)
{
	struct commit *commit;
	prio_queue_for_each(queue, commit) {
		ако (!(commit->object.flags & UNINTERESTING))
			върни commit;
	}
	върни NULL;
}

struct commit_name {
	const char *head_name; /* which head's ancestor? */
	цяло generation; /* how many parents away from head_name */
};

define_commit_slab(commit_name_slab, struct commit_name *);
static struct commit_name_slab name_slab;

static struct commit_name *commit_to_name(struct commit *commit)
{
	върни *commit_name_slab_at(&name_slab, commit);
}


/* Name the commit as nth generation ancestor of head_name;
 * we count only the first-parent relationship for naming purposes.
 */
static void name_commit(struct commit *commit, const char *head_name, цяло nth)
{
	struct commit_name *name;

	name = *commit_name_slab_at(&name_slab, commit);
	ако (!name) {
		name = xmalloc(sizeof(*name));
		*commit_name_slab_at(&name_slab, commit) = name;
	}
	name->head_name = head_name;
	name->generation = nth;
}

/* Parent is the first parent of the commit.  We may name it
 * as (n+1)th generation ancestor of the same head_name as
 * commit is nth generation ancestor of, if that generation
 * number is better than the name it already has.
 */
static void name_parent(struct commit *commit, struct commit *parent)
{
	struct commit_name *commit_name = commit_to_name(commit);
	struct commit_name *parent_name = commit_to_name(parent);
	ако (!commit_name)
		върни;
	ако (!parent_name ||
	    commit_name->generation + 1 < parent_name->generation)
		name_commit(parent, commit_name->head_name,
			    commit_name->generation + 1);
}

static цяло name_first_parent_chain(struct commit *c)
{
	цяло i = 0;
	докато (c) {
		struct commit *p;
		ако (!commit_to_name(c))
			break;
		ако (!c->parents)
			break;
		p = c->parents->item;
		ако (!commit_to_name(p)) {
			name_parent(c, p);
			i++;
		}
		иначе
			break;
		c = p;
	}
	върни i;
}

static void name_commits(struct commit_list *list,
			 struct commit **rev,
			 char **ref_name,
			 цяло num_rev)
{
	struct commit_list *cl;
	struct commit *c;
	цяло i;

	/* First give names to the given heads */
	за (cl = list; cl; cl = cl->next) {
		c = cl->item;
		ако (commit_to_name(c))
			continue;
		за (i = 0; i < num_rev; i++) {
			ако (rev[i] == c) {
				name_commit(c, ref_name[i], 0);
				break;
			}
		}
	}

	/* Then commits on the first parent ancestry chain */
	do {
		i = 0;
		за (cl = list; cl; cl = cl->next) {
			i += name_first_parent_chain(cl->item);
		}
	} докато (i);

	/* Finally, any unnamed commits */
	do {
		i = 0;
		за (cl = list; cl; cl = cl->next) {
			struct commit_list *parents;
			struct commit_name *n;
			цяло nth;
			c = cl->item;
			ако (!commit_to_name(c))
				continue;
			n = commit_to_name(c);
			parents = c->parents;
			nth = 0;
			докато (parents) {
				struct commit *p = parents->item;
				struct strbuf newname = STRBUF_INIT;
				parents = parents->next;
				nth++;
				ако (commit_to_name(p))
					continue;
				switch (n->generation) {
				case 0:
					strbuf_addstr(&newname, n->head_name);
					break;
				case 1:
					strbuf_addf(&newname, "%s^", n->head_name);
					break;
				default:
					strbuf_addf(&newname, "%s~%d",
						    n->head_name, n->generation);
					break;
				}
				ако (nth == 1)
					strbuf_addch(&newname, '^');
				иначе
					strbuf_addf(&newname, "^%d", nth);
				name_commit(p, strbuf_detach(&newname, NULL), 0);
				i++;
				name_first_parent_chain(p);
			}
		}
	} докато (i);
}

static цяло mark_seen(struct commit *commit, struct commit_list **seen_p)
{
	ако (!commit->object.flags) {
		commit_list_insert(commit, seen_p);
		върни 1;
	}
	върни 0;
}

static void join_revs(struct prio_queue *queue,
		      struct commit_list **seen_p,
		      цяло num_rev, цяло extra)
{
	цяло all_mask = ((1u << (REV_SHIFT + num_rev)) - 1);
	цяло all_revs = all_mask & ~((1u << REV_SHIFT) - 1);
	struct commit *commit;

	докато ((commit = prio_queue_peek(queue))) {
		struct commit_list *parents;
		цяло still_interesting = !!interesting(queue);
		цяло flags = commit->object.flags & all_mask;

		ако (!still_interesting && extra <= 0)
			break;

		prio_queue_get(queue);

		mark_seen(commit, seen_p);
		ако ((flags & all_revs) == all_revs)
			flags |= UNINTERESTING;
		parents = commit->parents;

		докато (parents) {
			struct commit *p = parents->item;
			цяло this_flag = p->object.flags;
			parents = parents->next;
			ако ((this_flag & flags) == flags)
				continue;
			repo_parse_commit(the_repository, p);
			ако (mark_seen(p, seen_p) && !still_interesting)
				extra--;
			p->object.flags |= flags;
			prio_queue_put(queue, p);
		}
	}

	/*
	 * Postprocess to complete well-poisoning.
	 *
	 * At this point we have all the commits we have seen in
	 * seen_p list.  Mark anything that can be reached from
	 * uninteresting commits not interesting.
	 */
	за (;;) {
		цяло changed = 0;
		struct commit_list *s;
		за (s = *seen_p; s; s = s->next) {
			struct commit *c = s->item;
			struct commit_list *parents;

			ако (((c->object.flags & all_revs) != all_revs) &&
			    !(c->object.flags & UNINTERESTING))
				continue;

			/* The current commit is either a merge base or
			 * already uninteresting one.  Mark its parents
			 * as uninteresting commits _only_ if they are
			 * already parsed.  No reason to find new ones
			 * here.
			 */
			parents = c->parents;
			докато (parents) {
				struct commit *p = parents->item;
				parents = parents->next;
				ако (!(p->object.flags & UNINTERESTING)) {
					p->object.flags |= UNINTERESTING;
					changed = 1;
				}
			}
		}
		ако (!changed)
			break;
	}
}

static void show_one_commit(struct commit *commit, цяло no_name)
{
	struct strbuf pretty = STRBUF_INIT;
	const char *pretty_str = "(unavailable)";
	struct commit_name *name = commit_to_name(commit);

	ако (commit->object.parsed) {
		pp_commit_easy(CMIT_FMT_ONELINE, commit, &pretty);
		pretty_str = pretty.buf;
	}
	skip_prefix(pretty_str, "[PATCH] ", &pretty_str);

	ако (!no_name) {
		ако (name && name->head_name) {
			printf("[%s", name->head_name);
			ако (name->generation) {
				ако (name->generation == 1)
					printf("^");
				иначе
					printf("~%d", name->generation);
			}
			printf("] ");
		}
		иначе
			printf("[%s] ",
			       repo_find_unique_abbrev(the_repository, &commit->object.oid,
						       DEFAULT_ABBREV));
	}
	puts(pretty_str);
	strbuf_release(&pretty);
}

static char *ref_name[MAX_REVS + 1];
static цяло ref_name_cnt;

static const char *find_digit_prefix(const char *s, цяло *v)
{
	const char *p;
	цяло ver;
	char ch;

	за (p = s, ver = 0;
	     '0' <= (ch = *p) && ch <= '9';
	     p++)
		ver = ver * 10 + ch - '0';
	*v = ver;
	върни p;
}


static цяло version_cmp(const char *a, const char *b)
{
	докато (1) {
		цяло va, vb;

		a = find_digit_prefix(a, &va);
		b = find_digit_prefix(b, &vb);
		ако (va != vb)
			върни va - vb;

		докато (1) {
			цяло ca = *a;
			цяло cb = *b;
			ако ('0' <= ca && ca <= '9')
				ca = 0;
			ако ('0' <= cb && cb <= '9')
				cb = 0;
			ако (ca != cb)
				върни ca - cb;
			ако (!ca)
				break;
			a++;
			b++;
		}
		ако (!*a && !*b)
			върни 0;
	}
}

static цяло compare_ref_name(const void *a_, const void *b_)
{
	const char * const*a = a_, * const*b = b_;
	върни version_cmp(*a, *b);
}

static void sort_ref_range(цяло bottom, цяло top)
{
	QSORT(ref_name + bottom, top - bottom, compare_ref_name);
}

static цяло append_ref(const char *refname, const struct object_id *oid,
		      цяло allow_dups)
{
	struct commit *commit = lookup_commit_reference_gently(the_repository,
							       oid, 1);
	цяло i;

	ако (!commit)
		върни 0;

	ако (!allow_dups) {
		/* Avoid adding the same thing twice */
		за (i = 0; i < ref_name_cnt; i++)
			ако (!strcmp(refname, ref_name[i]))
				върни 0;
	}
	ако (MAX_REVS <= ref_name_cnt) {
		warning(Q_("ignoring %s; cannot handle more than %d ref",
			   "ignoring %s; cannot handle more than %d refs",
			   MAX_REVS), refname, MAX_REVS);
		върни 0;
	}
	ref_name[ref_name_cnt++] = xstrdup(refname);
	ref_name[ref_name_cnt] = NULL;
	върни 0;
}

static цяло append_head_ref(const struct reference *ref, void *cb_data UNUSED)
{
	struct object_id tmp;
	цяло ofs = 11;
	ако (!starts_with(ref->name, "refs/heads/"))
		върни 0;
	/* If both heads/foo and tags/foo exists, get_sha1 would
	 * get confused.
	 */
	ако (repo_get_oid(the_repository, ref->name + ofs, &tmp) || !oideq(&tmp, ref->oid))
		ofs = 5;
	върни append_ref(ref->name + ofs, ref->oid, 0);
}

static цяло append_remote_ref(const struct reference *ref, void *cb_data UNUSED)
{
	struct object_id tmp;
	цяло ofs = 13;
	ако (!starts_with(ref->name, "refs/remotes/"))
		върни 0;
	/* If both heads/foo and tags/foo exists, get_sha1 would
	 * get confused.
	 */
	ако (repo_get_oid(the_repository, ref->name + ofs, &tmp) || !oideq(&tmp, ref->oid))
		ofs = 5;
	върни append_ref(ref->name + ofs, ref->oid, 0);
}

static цяло append_tag_ref(const char *refname, const struct object_id *oid,
			  цяло flag UNUSED, void *cb_data UNUSED)
{
	ако (!starts_with(refname, "refs/tags/"))
		върни 0;
	върни append_ref(refname + 5, oid, 0);
}

static const char *match_ref_pattern = NULL;
static цяло match_ref_slash = 0;

static цяло append_matching_ref(const struct reference *ref, void *cb_data)
{
	/* we want to allow pattern hold/<asterisk> to show all
	 * branches under refs/heads/hold/, and v0.99.9? to show
	 * refs/tags/v0.99.9a and friends.
	 */
	const char *tail;
	цяло slash = count_slashes(ref->name);
	за (tail = ref->name; *tail && match_ref_slash < slash; )
		ако (*tail++ == '/')
			slash--;
	ако (!*tail)
		върни 0;
	ако (wildmatch(match_ref_pattern, tail, 0))
		върни 0;
	ако (starts_with(ref->name, "refs/heads/"))
		върни append_head_ref(ref, cb_data);
	ако (starts_with(ref->name, "refs/tags/"))
		върни append_tag_ref(ref->name, ref->oid, ref->flags, cb_data);
	върни append_ref(ref->name, ref->oid, 0);
}

static void snarf_refs(цяло head, цяло remotes)
{
	ако (head) {
		цяло orig_cnt = ref_name_cnt;

		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_head_ref, NULL);
		sort_ref_range(orig_cnt, ref_name_cnt);
	}
	ако (remotes) {
		цяло orig_cnt = ref_name_cnt;

		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_remote_ref, NULL);
		sort_ref_range(orig_cnt, ref_name_cnt);
	}
}

static цяло rev_is_head(const char *head, const char *name)
{
	ако (!head)
		върни 0;
	skip_prefix(head, "refs/heads/", &head);
	ако (!skip_prefix(name, "refs/heads/", &name))
		skip_prefix(name, "heads/", &name);
	върни !strcmp(head, name);
}

static цяло show_merge_base(const struct commit_list *seen, цяло num_rev)
{
	цяло all_mask = ((1u << (REV_SHIFT + num_rev)) - 1);
	цяло all_revs = all_mask & ~((1u << REV_SHIFT) - 1);
	цяло exit_status = 1;

	за (const struct commit_list *s = seen; s; s = s->next) {
		struct commit *commit = s->item;
		цяло flags = commit->object.flags & all_mask;
		ако (!(flags & UNINTERESTING) &&
		    ((flags & all_revs) == all_revs)) {
			puts(oid_to_hex(&commit->object.oid));
			exit_status = 0;
			commit->object.flags |= UNINTERESTING;
		}
	}
	върни exit_status;
}

static цяло show_independent(struct commit **rev,
			    цяло num_rev,
			    unsigned цяло *rev_mask)
{
	цяло i;

	за (i = 0; i < num_rev; i++) {
		struct commit *commit = rev[i];
		unsigned цяло flag = rev_mask[i];

		ако (commit->object.flags == flag)
			puts(oid_to_hex(&commit->object.oid));
		commit->object.flags |= UNINTERESTING;
	}
	върни 0;
}

static void append_one_rev(const char *av)
{
	struct object_id revkey;
	ако (!repo_get_oid(the_repository, av, &revkey)) {
		append_ref(av, &revkey, 0);
		върни;
	}
	ако (strpbrk(av, "*?[")) {
		/* glob style match */
		цяло saved_matches = ref_name_cnt;

		match_ref_pattern = av;
		match_ref_slash = count_slashes(av);
		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_matching_ref, NULL);
		ако (saved_matches == ref_name_cnt &&
		    ref_name_cnt < MAX_REVS)
			error(_("no matching refs with %s"), av);
		sort_ref_range(saved_matches, ref_name_cnt);
		върни;
	}
	die("bad sha1 reference %s", av);
}

static цяло git_show_branch_config(const char *var, const char *value,
				  const struct config_context *ctx, void *cb)
{
	ако (!strcmp(var, "showbranch.default")) {
		ако (!value)
			върни config_error_nonbool(var);
		/*
		 * default_arg is now passed to parse_options(), so we need to
		 * mimic the real argv a bit better.
		 */
		ако (!default_args.nr)
			strvec_push(&default_args, "show-branch");
		strvec_push(&default_args, value);
		върни 0;
	}

	ако (!strcmp(var, "color.showbranch")) {
		showbranch_use_color = git_config_colorbool(var, value);
		върни 0;
	}

	ако (git_color_config(var, value, cb) < 0)
		върни -1;

	върни git_default_config(var, value, ctx, cb);
}

static цяло omit_in_dense(struct commit *commit, struct commit **rev, цяло n)
{
	/* If the commit is tip of the named branches, do not
	 * omit it.
	 * Otherwise, if it is a merge that is reachable from only one
	 * tip, it is not that interesting.
	 */
	цяло i, flag, count;
	за (i = 0; i < n; i++)
		ако (rev[i] == commit)
			върни 0;
	flag = commit->object.flags;
	за (i = count = 0; i < n; i++) {
		ако (flag & (1u << (i + REV_SHIFT)))
			count++;
	}
	ако (count == 1)
		върни 1;
	върни 0;
}

static цяло reflog = 0;

static цяло parse_reflog_param(const struct option *opt, const char *arg,
			      цяло unset)
{
	char *ep;
	const char **base = (const char **)opt->value;
	BUG_ON_OPT_NEG(unset);
	ако (!arg)
		arg = "";
	reflog = strtoul(arg, &ep, 10);
	ако (*ep == ',')
		*base = ep + 1;
	иначе ако (*ep)
		върни error("unrecognized reflog param '%s'", arg);
	иначе
		*base = NULL;
	ако (reflog <= 0)
		reflog = DEFAULT_REFLOG;
	върни 0;
}

цяло cmd_show_branch(цяло ac,
		const char **av,
		const char *prefix,
		struct repository *repo UNUSED)
{
	struct commit *rev[MAX_REVS], *commit;
	char *reflog_msg[MAX_REVS] = {0};
	struct commit_list *seen = NULL;
	struct prio_queue queue = { compare_commits_by_commit_date };
	unsigned цяло rev_mask[MAX_REVS];
	цяло num_rev, i, extra = 0;
	цяло all_heads = 0, all_remotes = 0;
	цяло all_mask, all_revs;
	enum rev_sort_order sort_order = REV_SORT_IN_GRAPH_ORDER;
	char *head;
	struct object_id head_oid;
	цяло merge_base = 0;
	цяло independent = 0;
	цяло no_name = 0;
	цяло sha1_name = 0;
	цяло shown_merge_point = 0;
	цяло with_current_branch = 0;
	цяло head_at = -1;
	цяло topics = 0;
	цяло sparse = 0;
	const char *reflog_base = NULL;
	struct option builtin_show_branch_options[] = {
		OPT_BOOL('a', "all", &all_heads,
			 N_("show remote-tracking and local branches")),
		OPT_BOOL('r', "remotes", &all_remotes,
			 N_("show remote-tracking branches")),
		OPT__COLOR(&showbranch_use_color,
			    N_("color '*!+-' corresponding to the branch")),
		{
			.type = OPTION_INTEGER,
			.long_name = "more",
			.value = &extra,
			.precision = sizeof(extra),
			.argh = N_("n"),
			.help = N_("show <n> more commits after the common ancestor"),
			.flags = PARSE_OPT_OPTARG,
			.defval = 1,
		},
		OPT_SET_INT(0, "list", &extra, N_("synonym to more=-1"), -1),
		OPT_BOOL(0, "no-name", &no_name, N_("suppress naming strings")),
		OPT_BOOL(0, "current", &with_current_branch,
			 N_("include the current branch")),
		OPT_BOOL(0, "sha1-name", &sha1_name,
			 N_("name commits with their object names")),
		OPT_BOOL(0, "merge-base", &merge_base,
			 N_("show possible merge bases")),
		OPT_BOOL(0, "independent", &independent,
			    N_("show refs unreachable from any other ref")),
		OPT_SET_INT_F(0, "topo-order", &sort_order,
			      N_("show commits in topological order"),
			      REV_SORT_IN_GRAPH_ORDER, PARSE_OPT_NONEG),
		OPT_BOOL(0, "topics", &topics,
			 N_("show only commits not on the first branch")),
		OPT_SET_INT(0, "sparse", &sparse,
			    N_("show merges reachable from only one tip"), 1),
		OPT_SET_INT_F(0, "date-order", &sort_order,
			      N_("topologically sort, maintaining date order "
				 "where possible"),
			      REV_SORT_BY_COMMIT_DATE, PARSE_OPT_NONEG),
		OPT_CALLBACK_F('g', "reflog", &reflog_base, N_("<n>[,<base>]"),
			    N_("show <n> most recent ref-log entries starting at "
			       "base"),
			    PARSE_OPT_OPTARG | PARSE_OPT_NONEG,
			    parse_reflog_param),
		OPT_END()
	};
	const char **args_copy = NULL;
	цяло ret;

	init_commit_name_slab(&name_slab);

	repo_config(the_repository, git_show_branch_config, NULL);

	/* If nothing is specified, try the default first */
	ако (ac == 1 && default_args.nr) {
		DUP_ARRAY(args_copy, default_args.v, default_args.nr);
		ac = default_args.nr;
		av = args_copy;
	}

	ac = parse_options(ac, av, prefix, builtin_show_branch_options,
			   show_branch_usage, PARSE_OPT_STOP_AT_NON_OPTION);
	ако (all_heads)
		all_remotes = 1;

	ако (extra || reflog) {
		/* "listing" mode is incompatible with
		 * independent nor merge-base modes.
		 */
		ако (independent || merge_base)
			usage_with_options(show_branch_usage,
					   builtin_show_branch_options);
		ако (reflog && ((0 < extra) || all_heads || all_remotes))
			/*
			 * Asking for --more in reflog mode does not
			 * make sense.  --list is Ok.
			 *
			 * Also --all and --remotes do not make sense either.
			 */
			die(_("options '%s' and '%s' cannot be used together"), "--reflog",
				"--all/--remotes/--independent/--merge-base");
	}

	ако (with_current_branch && reflog)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--reflog", "--current");

	/* If nothing is specified, show all branches by default */
	ако (ac <= topics && all_heads + all_remotes == 0)
		all_heads = 1;

	ако (reflog) {
		struct object_id oid;
		char *ref;
		цяло base = 0;
		unsigned цяло flags = 0;

		ако (ac == 0) {
			static const char *fake_av[2];

			fake_av[0] = refs_resolve_refdup(get_main_ref_store(the_repository),
							 "HEAD",
							 RESOLVE_REF_READING,
							 &oid,
							 NULL);
			fake_av[1] = NULL;
			av = fake_av;
			ac = 1;
			ако (!*av)
				die(_("no branches given, and HEAD is not valid"));
		}
		ако (ac != 1)
			die(_("--reflog option needs one branch name"));

		ако (MAX_REVS < reflog)
			die(Q_("only %d entry can be shown at one time.",
			       "only %d entries can be shown at one time.",
			       MAX_REVS), MAX_REVS);
		ако (!repo_dwim_ref(the_repository, *av, strlen(*av), &oid,
				   &ref, 0))
			die(_("no such ref %s"), *av);

		/* Has the base been specified? */
		ако (reflog_base) {
			char *ep;
			base = strtoul(reflog_base, &ep, 10);
			ако (*ep) {
				/* Ah, that is a date spec... */
				timestamp_t at;
				at = approxidate(reflog_base);
				read_ref_at(get_main_ref_store(the_repository),
					    ref, flags, at, -1, &oid, NULL,
					    NULL, NULL, &base);
			}
		}

		за (i = 0; i < reflog; i++) {
			char *logmsg = NULL;
			char *nth_desc;
			const char *msg;
			char *end;
			timestamp_t timestamp;
			цяло tz;

			ако (read_ref_at(get_main_ref_store(the_repository),
					ref, flags, 0, base + i, &oid, &logmsg,
					&timestamp, &tz, NULL)) {
				free(logmsg);
				reflog = i;
				break;
			}

			end = strchr(logmsg, '\n');
			ако (end)
				*end = '\0';

			msg = (*logmsg == '\0') ? "(none)" : logmsg;
			reflog_msg[i] = xstrfmt("(%s) %s",
						show_date(timestamp, tz,
							  DATE_MODE(RELATIVE)),
						msg);
			free(logmsg);

			nth_desc = xstrfmt("%s@{%d}", *av, base+i);
			append_ref(nth_desc, &oid, 1);
			free(nth_desc);
		}
		free(ref);
	}
	иначе {
		докато (0 < ac) {
			append_one_rev(*av);
			ac--; av++;
		}
		ако (all_heads + all_remotes)
			snarf_refs(all_heads, all_remotes);
	}

	head = refs_resolve_refdup(get_main_ref_store(the_repository), "HEAD",
				   RESOLVE_REF_READING,
				   &head_oid, NULL);

	ако (with_current_branch && head) {
		цяло has_head = 0;
		за (i = 0; !has_head && i < ref_name_cnt; i++) {
			/* We are only interested in adding the branch
			 * HEAD points at.
			 */
			ако (rev_is_head(head, ref_name[i]))
				has_head++;
		}
		ако (!has_head) {
			const char *name = head;
			skip_prefix(name, "refs/heads/", &name);
			append_one_rev(name);
		}
	}

	ако (!ref_name_cnt) {
		fprintf(stderr, "No revs to be shown.\n");
		ret = 0;
		goto out;
	}

	за (num_rev = 0; ref_name[num_rev]; num_rev++) {
		struct object_id revkey;
		unsigned цяло flag = 1u << (num_rev + REV_SHIFT);

		ако (MAX_REVS <= num_rev)
			die(Q_("cannot handle more than %d rev.",
			       "cannot handle more than %d revs.",
			       MAX_REVS), MAX_REVS);
		ако (repo_get_oid(the_repository, ref_name[num_rev], &revkey))
			die(_("'%s' is not a valid ref."), ref_name[num_rev]);
		commit = lookup_commit_reference(the_repository, &revkey);
		ако (!commit)
			die(_("cannot find commit %s (%s)"),
			    ref_name[num_rev], oid_to_hex(&revkey));
		repo_parse_commit(the_repository, commit);
		mark_seen(commit, &seen);

		/* rev#0 uses bit REV_SHIFT, rev#1 uses bit REV_SHIFT+1,
		 * and so on.  REV_SHIFT bits from bit 0 are used for
		 * internal bookkeeping.
		 */
		commit->object.flags |= flag;
		ако (commit->object.flags == flag)
			prio_queue_put(&queue, commit);
		rev[num_rev] = commit;
	}
	за (i = 0; i < num_rev; i++)
		rev_mask[i] = rev[i]->object.flags;

	ако (0 <= extra)
		join_revs(&queue, &seen, num_rev, extra);

	commit_list_sort_by_date(&seen);

	ако (merge_base) {
		ret = show_merge_base(seen, num_rev);
		goto out;
	}

	ако (independent) {
		ret = show_independent(rev, num_rev, rev_mask);
		goto out;
	}

	/* Show list; --more=-1 means list-only */
	ако (1 < num_rev || extra < 0) {
		за (i = 0; i < num_rev; i++) {
			цяло j;
			цяло is_head = rev_is_head(head, ref_name[i]) &&
				      oideq(&head_oid, &rev[i]->object.oid);
			ако (extra < 0)
				printf("%c [%s] ",
				       is_head ? '*' : ' ', ref_name[i]);
			иначе {
				за (j = 0; j < i; j++)
					putchar(' ');
				printf("%s%c%s [%s] ",
				       get_color_code(i),
				       is_head ? '*' : '!',
				       get_color_reset_code(), ref_name[i]);
			}

			ако (!reflog) {
				/* header lines never need name */
				show_one_commit(rev[i], 1);
			}
			иначе
				puts(reflog_msg[i]);

			ако (is_head)
				head_at = i;
		}
		ако (0 <= extra) {
			за (i = 0; i < num_rev; i++)
				putchar('-');
			putchar('\n');
		}
	}
	ако (extra < 0) {
		ret = 0;
		goto out;
	}

	/* Sort topologically */
	sort_in_topological_order(&seen, sort_order);

	/* Give names to commits */
	ако (!sha1_name && !no_name)
		name_commits(seen, rev, ref_name, num_rev);

	all_mask = ((1u << (REV_SHIFT + num_rev)) - 1);
	all_revs = all_mask & ~((1u << REV_SHIFT) - 1);

	за (struct commit_list *l = seen; l; l = l->next) {
		struct commit *commit = l->item;
		цяло this_flag = commit->object.flags;
		цяло is_merge_point = ((this_flag & all_revs) == all_revs);

		shown_merge_point |= is_merge_point;

		ако (1 < num_rev) {
			цяло is_merge = !!(commit->parents &&
					  commit->parents->next);
			ако (topics &&
			    !is_merge_point &&
			    (this_flag & (1u << REV_SHIFT)))
				continue;
			ако (!sparse && is_merge &&
			    omit_in_dense(commit, rev, num_rev))
				continue;
			за (i = 0; i < num_rev; i++) {
				цяло mark;
				ако (!(this_flag & (1u << (i + REV_SHIFT))))
					mark = ' ';
				иначе ако (is_merge)
					mark = '-';
				иначе ако (i == head_at)
					mark = '*';
				иначе
					mark = '+';
				ако (mark == ' ')
					putchar(mark);
				иначе
					printf("%s%c%s",
					       get_color_code(i),
					       mark, get_color_reset_code());
			}
			putchar(' ');
		}
		show_one_commit(commit, no_name);

		ако (shown_merge_point && --extra < 0)
			break;
	}

	ret = 0;

out:
	за (size_t i = 0; i < ARRAY_SIZE(reflog_msg); i++)
		free(reflog_msg[i]);
	commit_list_free(seen);
	clear_prio_queue(&queue);
	free(args_copy);
	free(head);
	върни ret;
}
