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

static const char *get_color_code(整数 idx)
{
	如果 (want_color(showbranch_use_color))
		返回 column_colors_ansi[idx % column_colors_ansi_max];
	返回 "";
}

static const char *get_color_reset_code(void)
{
	如果 (want_color(showbranch_use_color))
		返回 GIT_COLOR_RESET;
	返回 "";
}

static struct commit *interesting(struct prio_queue *queue)
{
	struct commit *commit;
	prio_queue_for_each(queue, commit) {
		如果 (!(commit->object.flags & UNINTERESTING))
			返回 commit;
	}
	返回 NULL;
}

struct commit_name {
	const char *head_name; /* which head's ancestor? */
	整数 generation; /* how many parents away from head_name */
};

define_commit_slab(commit_name_slab, struct commit_name *);
static struct commit_name_slab name_slab;

static struct commit_name *commit_to_name(struct commit *commit)
{
	返回 *commit_name_slab_at(&name_slab, commit);
}


/* Name the commit as nth generation ancestor of head_name;
 * we count only the first-parent relationship for naming purposes.
 */
static void name_commit(struct commit *commit, const char *head_name, 整数 nth)
{
	struct commit_name *name;

	name = *commit_name_slab_at(&name_slab, commit);
	如果 (!name) {
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
	如果 (!commit_name)
		返回;
	如果 (!parent_name ||
	    commit_name->generation + 1 < parent_name->generation)
		name_commit(parent, commit_name->head_name,
			    commit_name->generation + 1);
}

static 整数 name_first_parent_chain(struct commit *c)
{
	整数 i = 0;
	当 (c) {
		struct commit *p;
		如果 (!commit_to_name(c))
			break;
		如果 (!c->parents)
			break;
		p = c->parents->item;
		如果 (!commit_to_name(p)) {
			name_parent(c, p);
			i++;
		}
		否则
			break;
		c = p;
	}
	返回 i;
}

static void name_commits(struct commit_list *list,
			 struct commit **rev,
			 char **ref_name,
			 整数 num_rev)
{
	struct commit_list *cl;
	struct commit *c;
	整数 i;

	/* First give names to the given heads */
	为 (cl = list; cl; cl = cl->next) {
		c = cl->item;
		如果 (commit_to_name(c))
			continue;
		为 (i = 0; i < num_rev; i++) {
			如果 (rev[i] == c) {
				name_commit(c, ref_name[i], 0);
				break;
			}
		}
	}

	/* Then commits on the first parent ancestry chain */
	do {
		i = 0;
		为 (cl = list; cl; cl = cl->next) {
			i += name_first_parent_chain(cl->item);
		}
	} 当 (i);

	/* Finally, any unnamed commits */
	do {
		i = 0;
		为 (cl = list; cl; cl = cl->next) {
			struct commit_list *parents;
			struct commit_name *n;
			整数 nth;
			c = cl->item;
			如果 (!commit_to_name(c))
				continue;
			n = commit_to_name(c);
			parents = c->parents;
			nth = 0;
			当 (parents) {
				struct commit *p = parents->item;
				struct strbuf newname = STRBUF_INIT;
				parents = parents->next;
				nth++;
				如果 (commit_to_name(p))
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
				如果 (nth == 1)
					strbuf_addch(&newname, '^');
				否则
					strbuf_addf(&newname, "^%d", nth);
				name_commit(p, strbuf_detach(&newname, NULL), 0);
				i++;
				name_first_parent_chain(p);
			}
		}
	} 当 (i);
}

static 整数 mark_seen(struct commit *commit, struct commit_list **seen_p)
{
	如果 (!commit->object.flags) {
		commit_list_insert(commit, seen_p);
		返回 1;
	}
	返回 0;
}

static void join_revs(struct prio_queue *queue,
		      struct commit_list **seen_p,
		      整数 num_rev, 整数 extra)
{
	整数 all_mask = ((1u << (REV_SHIFT + num_rev)) - 1);
	整数 all_revs = all_mask & ~((1u << REV_SHIFT) - 1);
	struct commit *commit;

	当 ((commit = prio_queue_peek(queue))) {
		struct commit_list *parents;
		整数 still_interesting = !!interesting(queue);
		整数 flags = commit->object.flags & all_mask;

		如果 (!still_interesting && extra <= 0)
			break;

		prio_queue_get(queue);

		mark_seen(commit, seen_p);
		如果 ((flags & all_revs) == all_revs)
			flags |= UNINTERESTING;
		parents = commit->parents;

		当 (parents) {
			struct commit *p = parents->item;
			整数 this_flag = p->object.flags;
			parents = parents->next;
			如果 ((this_flag & flags) == flags)
				continue;
			repo_parse_commit(the_repository, p);
			如果 (mark_seen(p, seen_p) && !still_interesting)
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
	为 (;;) {
		整数 changed = 0;
		struct commit_list *s;
		为 (s = *seen_p; s; s = s->next) {
			struct commit *c = s->item;
			struct commit_list *parents;

			如果 (((c->object.flags & all_revs) != all_revs) &&
			    !(c->object.flags & UNINTERESTING))
				continue;

			/* The current commit is either a merge base or
			 * already uninteresting one.  Mark its parents
			 * as uninteresting commits _only_ if they are
			 * already parsed.  No reason to find new ones
			 * here.
			 */
			parents = c->parents;
			当 (parents) {
				struct commit *p = parents->item;
				parents = parents->next;
				如果 (!(p->object.flags & UNINTERESTING)) {
					p->object.flags |= UNINTERESTING;
					changed = 1;
				}
			}
		}
		如果 (!changed)
			break;
	}
}

static void show_one_commit(struct commit *commit, 整数 no_name)
{
	struct strbuf pretty = STRBUF_INIT;
	const char *pretty_str = "(unavailable)";
	struct commit_name *name = commit_to_name(commit);

	如果 (commit->object.parsed) {
		pp_commit_easy(CMIT_FMT_ONELINE, commit, &pretty);
		pretty_str = pretty.buf;
	}
	skip_prefix(pretty_str, "[PATCH] ", &pretty_str);

	如果 (!no_name) {
		如果 (name && name->head_name) {
			printf("[%s", name->head_name);
			如果 (name->generation) {
				如果 (name->generation == 1)
					printf("^");
				否则
					printf("~%d", name->generation);
			}
			printf("] ");
		}
		否则
			printf("[%s] ",
			       repo_find_unique_abbrev(the_repository, &commit->object.oid,
						       DEFAULT_ABBREV));
	}
	puts(pretty_str);
	strbuf_release(&pretty);
}

static char *ref_name[MAX_REVS + 1];
static 整数 ref_name_cnt;

static const char *find_digit_prefix(const char *s, 整数 *v)
{
	const char *p;
	整数 ver;
	char ch;

	为 (p = s, ver = 0;
	     '0' <= (ch = *p) && ch <= '9';
	     p++)
		ver = ver * 10 + ch - '0';
	*v = ver;
	返回 p;
}


static 整数 version_cmp(const char *a, const char *b)
{
	当 (1) {
		整数 va, vb;

		a = find_digit_prefix(a, &va);
		b = find_digit_prefix(b, &vb);
		如果 (va != vb)
			返回 va - vb;

		当 (1) {
			整数 ca = *a;
			整数 cb = *b;
			如果 ('0' <= ca && ca <= '9')
				ca = 0;
			如果 ('0' <= cb && cb <= '9')
				cb = 0;
			如果 (ca != cb)
				返回 ca - cb;
			如果 (!ca)
				break;
			a++;
			b++;
		}
		如果 (!*a && !*b)
			返回 0;
	}
}

static 整数 compare_ref_name(const void *a_, const void *b_)
{
	const char * const*a = a_, * const*b = b_;
	返回 version_cmp(*a, *b);
}

static void sort_ref_range(整数 bottom, 整数 top)
{
	QSORT(ref_name + bottom, top - bottom, compare_ref_name);
}

static 整数 append_ref(const char *refname, const struct object_id *oid,
		      整数 allow_dups)
{
	struct commit *commit = lookup_commit_reference_gently(the_repository,
							       oid, 1);
	整数 i;

	如果 (!commit)
		返回 0;

	如果 (!allow_dups) {
		/* Avoid adding the same thing twice */
		为 (i = 0; i < ref_name_cnt; i++)
			如果 (!strcmp(refname, ref_name[i]))
				返回 0;
	}
	如果 (MAX_REVS <= ref_name_cnt) {
		warning(Q_("ignoring %s; cannot handle more than %d ref",
			   "ignoring %s; cannot handle more than %d refs",
			   MAX_REVS), refname, MAX_REVS);
		返回 0;
	}
	ref_name[ref_name_cnt++] = xstrdup(refname);
	ref_name[ref_name_cnt] = NULL;
	返回 0;
}

static 整数 append_head_ref(const struct reference *ref, void *cb_data UNUSED)
{
	struct object_id tmp;
	整数 ofs = 11;
	如果 (!starts_with(ref->name, "refs/heads/"))
		返回 0;
	/* If both heads/foo and tags/foo exists, get_sha1 would
	 * get confused.
	 */
	如果 (repo_get_oid(the_repository, ref->name + ofs, &tmp) || !oideq(&tmp, ref->oid))
		ofs = 5;
	返回 append_ref(ref->name + ofs, ref->oid, 0);
}

static 整数 append_remote_ref(const struct reference *ref, void *cb_data UNUSED)
{
	struct object_id tmp;
	整数 ofs = 13;
	如果 (!starts_with(ref->name, "refs/remotes/"))
		返回 0;
	/* If both heads/foo and tags/foo exists, get_sha1 would
	 * get confused.
	 */
	如果 (repo_get_oid(the_repository, ref->name + ofs, &tmp) || !oideq(&tmp, ref->oid))
		ofs = 5;
	返回 append_ref(ref->name + ofs, ref->oid, 0);
}

static 整数 append_tag_ref(const char *refname, const struct object_id *oid,
			  整数 flag UNUSED, void *cb_data UNUSED)
{
	如果 (!starts_with(refname, "refs/tags/"))
		返回 0;
	返回 append_ref(refname + 5, oid, 0);
}

static const char *match_ref_pattern = NULL;
static 整数 match_ref_slash = 0;

static 整数 append_matching_ref(const struct reference *ref, void *cb_data)
{
	/* we want to allow pattern hold/<asterisk> to show all
	 * branches under refs/heads/hold/, and v0.99.9? to show
	 * refs/tags/v0.99.9a and friends.
	 */
	const char *tail;
	整数 slash = count_slashes(ref->name);
	为 (tail = ref->name; *tail && match_ref_slash < slash; )
		如果 (*tail++ == '/')
			slash--;
	如果 (!*tail)
		返回 0;
	如果 (wildmatch(match_ref_pattern, tail, 0))
		返回 0;
	如果 (starts_with(ref->name, "refs/heads/"))
		返回 append_head_ref(ref, cb_data);
	如果 (starts_with(ref->name, "refs/tags/"))
		返回 append_tag_ref(ref->name, ref->oid, ref->flags, cb_data);
	返回 append_ref(ref->name, ref->oid, 0);
}

static void snarf_refs(整数 head, 整数 remotes)
{
	如果 (head) {
		整数 orig_cnt = ref_name_cnt;

		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_head_ref, NULL);
		sort_ref_range(orig_cnt, ref_name_cnt);
	}
	如果 (remotes) {
		整数 orig_cnt = ref_name_cnt;

		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_remote_ref, NULL);
		sort_ref_range(orig_cnt, ref_name_cnt);
	}
}

static 整数 rev_is_head(const char *head, const char *name)
{
	如果 (!head)
		返回 0;
	skip_prefix(head, "refs/heads/", &head);
	如果 (!skip_prefix(name, "refs/heads/", &name))
		skip_prefix(name, "heads/", &name);
	返回 !strcmp(head, name);
}

static 整数 show_merge_base(const struct commit_list *seen, 整数 num_rev)
{
	整数 all_mask = ((1u << (REV_SHIFT + num_rev)) - 1);
	整数 all_revs = all_mask & ~((1u << REV_SHIFT) - 1);
	整数 exit_status = 1;

	为 (const struct commit_list *s = seen; s; s = s->next) {
		struct commit *commit = s->item;
		整数 flags = commit->object.flags & all_mask;
		如果 (!(flags & UNINTERESTING) &&
		    ((flags & all_revs) == all_revs)) {
			puts(oid_to_hex(&commit->object.oid));
			exit_status = 0;
			commit->object.flags |= UNINTERESTING;
		}
	}
	返回 exit_status;
}

static 整数 show_independent(struct commit **rev,
			    整数 num_rev,
			    unsigned 整数 *rev_mask)
{
	整数 i;

	为 (i = 0; i < num_rev; i++) {
		struct commit *commit = rev[i];
		unsigned 整数 flag = rev_mask[i];

		如果 (commit->object.flags == flag)
			puts(oid_to_hex(&commit->object.oid));
		commit->object.flags |= UNINTERESTING;
	}
	返回 0;
}

static void append_one_rev(const char *av)
{
	struct object_id revkey;
	如果 (!repo_get_oid(the_repository, av, &revkey)) {
		append_ref(av, &revkey, 0);
		返回;
	}
	如果 (strpbrk(av, "*?[")) {
		/* glob style match */
		整数 saved_matches = ref_name_cnt;

		match_ref_pattern = av;
		match_ref_slash = count_slashes(av);
		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_matching_ref, NULL);
		如果 (saved_matches == ref_name_cnt &&
		    ref_name_cnt < MAX_REVS)
			error(_("no matching refs with %s"), av);
		sort_ref_range(saved_matches, ref_name_cnt);
		返回;
	}
	die("bad sha1 reference %s", av);
}

static 整数 git_show_branch_config(const char *var, const char *value,
				  const struct config_context *ctx, void *cb)
{
	如果 (!strcmp(var, "showbranch.default")) {
		如果 (!value)
			返回 config_error_nonbool(var);
		/*
		 * default_arg is now passed to parse_options(), so we need to
		 * mimic the real argv a bit better.
		 */
		如果 (!default_args.nr)
			strvec_push(&default_args, "show-branch");
		strvec_push(&default_args, value);
		返回 0;
	}

	如果 (!strcmp(var, "color.showbranch")) {
		showbranch_use_color = git_config_colorbool(var, value);
		返回 0;
	}

	如果 (git_color_config(var, value, cb) < 0)
		返回 -1;

	返回 git_default_config(var, value, ctx, cb);
}

static 整数 omit_in_dense(struct commit *commit, struct commit **rev, 整数 n)
{
	/* If the commit is tip of the named branches, do not
	 * omit it.
	 * Otherwise, if it is a merge that is reachable from only one
	 * tip, it is not that interesting.
	 */
	整数 i, flag, count;
	为 (i = 0; i < n; i++)
		如果 (rev[i] == commit)
			返回 0;
	flag = commit->object.flags;
	为 (i = count = 0; i < n; i++) {
		如果 (flag & (1u << (i + REV_SHIFT)))
			count++;
	}
	如果 (count == 1)
		返回 1;
	返回 0;
}

static 整数 reflog = 0;

static 整数 parse_reflog_param(const struct option *opt, const char *arg,
			      整数 unset)
{
	char *ep;
	const char **base = (const char **)opt->value;
	BUG_ON_OPT_NEG(unset);
	如果 (!arg)
		arg = "";
	reflog = strtoul(arg, &ep, 10);
	如果 (*ep == ',')
		*base = ep + 1;
	否则 如果 (*ep)
		返回 error("unrecognized reflog param '%s'", arg);
	否则
		*base = NULL;
	如果 (reflog <= 0)
		reflog = DEFAULT_REFLOG;
	返回 0;
}

整数 cmd_show_branch(整数 ac,
		const char **av,
		const char *prefix,
		struct repository *repo UNUSED)
{
	struct commit *rev[MAX_REVS], *commit;
	char *reflog_msg[MAX_REVS] = {0};
	struct commit_list *seen = NULL;
	struct prio_queue queue = { compare_commits_by_commit_date };
	unsigned 整数 rev_mask[MAX_REVS];
	整数 num_rev, i, extra = 0;
	整数 all_heads = 0, all_remotes = 0;
	整数 all_mask, all_revs;
	enum rev_sort_order sort_order = REV_SORT_IN_GRAPH_ORDER;
	char *head;
	struct object_id head_oid;
	整数 merge_base = 0;
	整数 independent = 0;
	整数 no_name = 0;
	整数 sha1_name = 0;
	整数 shown_merge_point = 0;
	整数 with_current_branch = 0;
	整数 head_at = -1;
	整数 topics = 0;
	整数 sparse = 0;
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
	整数 ret;

	init_commit_name_slab(&name_slab);

	repo_config(the_repository, git_show_branch_config, NULL);

	/* If nothing is specified, try the default first */
	如果 (ac == 1 && default_args.nr) {
		DUP_ARRAY(args_copy, default_args.v, default_args.nr);
		ac = default_args.nr;
		av = args_copy;
	}

	ac = parse_options(ac, av, prefix, builtin_show_branch_options,
			   show_branch_usage, PARSE_OPT_STOP_AT_NON_OPTION);
	如果 (all_heads)
		all_remotes = 1;

	如果 (extra || reflog) {
		/* "listing" mode is incompatible with
		 * independent nor merge-base modes.
		 */
		如果 (independent || merge_base)
			usage_with_options(show_branch_usage,
					   builtin_show_branch_options);
		如果 (reflog && ((0 < extra) || all_heads || all_remotes))
			/*
			 * Asking for --more in reflog mode does not
			 * make sense.  --list is Ok.
			 *
			 * Also --all and --remotes do not make sense either.
			 */
			die(_("options '%s' and '%s' cannot be used together"), "--reflog",
				"--all/--remotes/--independent/--merge-base");
	}

	如果 (with_current_branch && reflog)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--reflog", "--current");

	/* If nothing is specified, show all branches by default */
	如果 (ac <= topics && all_heads + all_remotes == 0)
		all_heads = 1;

	如果 (reflog) {
		struct object_id oid;
		char *ref;
		整数 base = 0;
		unsigned 整数 flags = 0;

		如果 (ac == 0) {
			static const char *fake_av[2];

			fake_av[0] = refs_resolve_refdup(get_main_ref_store(the_repository),
							 "HEAD",
							 RESOLVE_REF_READING,
							 &oid,
							 NULL);
			fake_av[1] = NULL;
			av = fake_av;
			ac = 1;
			如果 (!*av)
				die(_("no branches given, and HEAD is not valid"));
		}
		如果 (ac != 1)
			die(_("--reflog option needs one branch name"));

		如果 (MAX_REVS < reflog)
			die(Q_("only %d entry can be shown at one time.",
			       "only %d entries can be shown at one time.",
			       MAX_REVS), MAX_REVS);
		如果 (!repo_dwim_ref(the_repository, *av, strlen(*av), &oid,
				   &ref, 0))
			die(_("no such ref %s"), *av);

		/* Has the base been specified? */
		如果 (reflog_base) {
			char *ep;
			base = strtoul(reflog_base, &ep, 10);
			如果 (*ep) {
				/* Ah, that is a date spec... */
				timestamp_t at;
				at = approxidate(reflog_base);
				read_ref_at(get_main_ref_store(the_repository),
					    ref, flags, at, -1, &oid, NULL,
					    NULL, NULL, &base);
			}
		}

		为 (i = 0; i < reflog; i++) {
			char *logmsg = NULL;
			char *nth_desc;
			const char *msg;
			char *end;
			timestamp_t timestamp;
			整数 tz;

			如果 (read_ref_at(get_main_ref_store(the_repository),
					ref, flags, 0, base + i, &oid, &logmsg,
					&timestamp, &tz, NULL)) {
				free(logmsg);
				reflog = i;
				break;
			}

			end = strchr(logmsg, '\n');
			如果 (end)
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
	否则 {
		当 (0 < ac) {
			append_one_rev(*av);
			ac--; av++;
		}
		如果 (all_heads + all_remotes)
			snarf_refs(all_heads, all_remotes);
	}

	head = refs_resolve_refdup(get_main_ref_store(the_repository), "HEAD",
				   RESOLVE_REF_READING,
				   &head_oid, NULL);

	如果 (with_current_branch && head) {
		整数 has_head = 0;
		为 (i = 0; !has_head && i < ref_name_cnt; i++) {
			/* We are only interested in adding the branch
			 * HEAD points at.
			 */
			如果 (rev_is_head(head, ref_name[i]))
				has_head++;
		}
		如果 (!has_head) {
			const char *name = head;
			skip_prefix(name, "refs/heads/", &name);
			append_one_rev(name);
		}
	}

	如果 (!ref_name_cnt) {
		fprintf(stderr, "No revs to be shown.\n");
		ret = 0;
		goto out;
	}

	为 (num_rev = 0; ref_name[num_rev]; num_rev++) {
		struct object_id revkey;
		unsigned 整数 flag = 1u << (num_rev + REV_SHIFT);

		如果 (MAX_REVS <= num_rev)
			die(Q_("cannot handle more than %d rev.",
			       "cannot handle more than %d revs.",
			       MAX_REVS), MAX_REVS);
		如果 (repo_get_oid(the_repository, ref_name[num_rev], &revkey))
			die(_("'%s' is not a valid ref."), ref_name[num_rev]);
		commit = lookup_commit_reference(the_repository, &revkey);
		如果 (!commit)
			die(_("cannot find commit %s (%s)"),
			    ref_name[num_rev], oid_to_hex(&revkey));
		repo_parse_commit(the_repository, commit);
		mark_seen(commit, &seen);

		/* rev#0 uses bit REV_SHIFT, rev#1 uses bit REV_SHIFT+1,
		 * and so on.  REV_SHIFT bits from bit 0 are used for
		 * internal bookkeeping.
		 */
		commit->object.flags |= flag;
		如果 (commit->object.flags == flag)
			prio_queue_put(&queue, commit);
		rev[num_rev] = commit;
	}
	为 (i = 0; i < num_rev; i++)
		rev_mask[i] = rev[i]->object.flags;

	如果 (0 <= extra)
		join_revs(&queue, &seen, num_rev, extra);

	commit_list_sort_by_date(&seen);

	如果 (merge_base) {
		ret = show_merge_base(seen, num_rev);
		goto out;
	}

	如果 (independent) {
		ret = show_independent(rev, num_rev, rev_mask);
		goto out;
	}

	/* Show list; --more=-1 means list-only */
	如果 (1 < num_rev || extra < 0) {
		为 (i = 0; i < num_rev; i++) {
			整数 j;
			整数 is_head = rev_is_head(head, ref_name[i]) &&
				      oideq(&head_oid, &rev[i]->object.oid);
			如果 (extra < 0)
				printf("%c [%s] ",
				       is_head ? '*' : ' ', ref_name[i]);
			否则 {
				为 (j = 0; j < i; j++)
					putchar(' ');
				printf("%s%c%s [%s] ",
				       get_color_code(i),
				       is_head ? '*' : '!',
				       get_color_reset_code(), ref_name[i]);
			}

			如果 (!reflog) {
				/* header lines never need name */
				show_one_commit(rev[i], 1);
			}
			否则
				puts(reflog_msg[i]);

			如果 (is_head)
				head_at = i;
		}
		如果 (0 <= extra) {
			为 (i = 0; i < num_rev; i++)
				putchar('-');
			putchar('\n');
		}
	}
	如果 (extra < 0) {
		ret = 0;
		goto out;
	}

	/* Sort topologically */
	sort_in_topological_order(&seen, sort_order);

	/* Give names to commits */
	如果 (!sha1_name && !no_name)
		name_commits(seen, rev, ref_name, num_rev);

	all_mask = ((1u << (REV_SHIFT + num_rev)) - 1);
	all_revs = all_mask & ~((1u << REV_SHIFT) - 1);

	为 (struct commit_list *l = seen; l; l = l->next) {
		struct commit *commit = l->item;
		整数 this_flag = commit->object.flags;
		整数 is_merge_point = ((this_flag & all_revs) == all_revs);

		shown_merge_point |= is_merge_point;

		如果 (1 < num_rev) {
			整数 is_merge = !!(commit->parents &&
					  commit->parents->next);
			如果 (topics &&
			    !is_merge_point &&
			    (this_flag & (1u << REV_SHIFT)))
				continue;
			如果 (!sparse && is_merge &&
			    omit_in_dense(commit, rev, num_rev))
				continue;
			为 (i = 0; i < num_rev; i++) {
				整数 mark;
				如果 (!(this_flag & (1u << (i + REV_SHIFT))))
					mark = ' ';
				否则 如果 (is_merge)
					mark = '-';
				否则 如果 (i == head_at)
					mark = '*';
				否则
					mark = '+';
				如果 (mark == ' ')
					putchar(mark);
				否则
					printf("%s%c%s",
					       get_color_code(i),
					       mark, get_color_reset_code());
			}
			putchar(' ');
		}
		show_one_commit(commit, no_name);

		如果 (shown_merge_point && --extra < 0)
			break;
	}

	ret = 0;

out:
	为 (size_t i = 0; i < ARRAY_SIZE(reflog_msg); i++)
		free(reflog_msg[i]);
	commit_list_free(seen);
	clear_prio_queue(&queue);
	free(args_copy);
	free(head);
	返回 ret;
}
