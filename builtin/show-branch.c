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

static const char *get_color_code(số_nguyên idx)
{
	nếu (want_color(showbranch_use_color))
		trả_về column_colors_ansi[idx % column_colors_ansi_max];
	trả_về "";
}

static const char *get_color_reset_code(void)
{
	nếu (want_color(showbranch_use_color))
		trả_về GIT_COLOR_RESET;
	trả_về "";
}

static struct commit *interesting(struct prio_queue *queue)
{
	struct commit *commit;
	prio_queue_for_each(queue, commit) {
		nếu (!(commit->object.flags & UNINTERESTING))
			trả_về commit;
	}
	trả_về NULL;
}

struct commit_name {
	const char *head_name; /* which head's ancestor? */
	số_nguyên generation; /* how many parents away from head_name */
};

define_commit_slab(commit_name_slab, struct commit_name *);
static struct commit_name_slab name_slab;

static struct commit_name *commit_to_name(struct commit *commit)
{
	trả_về *commit_name_slab_at(&name_slab, commit);
}


/* Name the commit as nth generation ancestor of head_name;
 * we count only the first-parent relationship for naming purposes.
 */
static void name_commit(struct commit *commit, const char *head_name, số_nguyên nth)
{
	struct commit_name *name;

	name = *commit_name_slab_at(&name_slab, commit);
	nếu (!name) {
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
	nếu (!commit_name)
		trả_về;
	nếu (!parent_name ||
	    commit_name->generation + 1 < parent_name->generation)
		name_commit(parent, commit_name->head_name,
			    commit_name->generation + 1);
}

static số_nguyên name_first_parent_chain(struct commit *c)
{
	số_nguyên i = 0;
	trong_khi (c) {
		struct commit *p;
		nếu (!commit_to_name(c))
			break;
		nếu (!c->parents)
			break;
		p = c->parents->item;
		nếu (!commit_to_name(p)) {
			name_parent(c, p);
			i++;
		}
		không_thì
			break;
		c = p;
	}
	trả_về i;
}

static void name_commits(struct commit_list *list,
			 struct commit **rev,
			 char **ref_name,
			 số_nguyên num_rev)
{
	struct commit_list *cl;
	struct commit *c;
	số_nguyên i;

	/* First give names to the given heads */
	cho (cl = list; cl; cl = cl->next) {
		c = cl->item;
		nếu (commit_to_name(c))
			continue;
		cho (i = 0; i < num_rev; i++) {
			nếu (rev[i] == c) {
				name_commit(c, ref_name[i], 0);
				break;
			}
		}
	}

	/* Then commits on the first parent ancestry chain */
	do {
		i = 0;
		cho (cl = list; cl; cl = cl->next) {
			i += name_first_parent_chain(cl->item);
		}
	} trong_khi (i);

	/* Finally, any unnamed commits */
	do {
		i = 0;
		cho (cl = list; cl; cl = cl->next) {
			struct commit_list *parents;
			struct commit_name *n;
			số_nguyên nth;
			c = cl->item;
			nếu (!commit_to_name(c))
				continue;
			n = commit_to_name(c);
			parents = c->parents;
			nth = 0;
			trong_khi (parents) {
				struct commit *p = parents->item;
				struct strbuf newname = STRBUF_INIT;
				parents = parents->next;
				nth++;
				nếu (commit_to_name(p))
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
				nếu (nth == 1)
					strbuf_addch(&newname, '^');
				không_thì
					strbuf_addf(&newname, "^%d", nth);
				name_commit(p, strbuf_detach(&newname, NULL), 0);
				i++;
				name_first_parent_chain(p);
			}
		}
	} trong_khi (i);
}

static số_nguyên mark_seen(struct commit *commit, struct commit_list **seen_p)
{
	nếu (!commit->object.flags) {
		commit_list_insert(commit, seen_p);
		trả_về 1;
	}
	trả_về 0;
}

static void join_revs(struct prio_queue *queue,
		      struct commit_list **seen_p,
		      số_nguyên num_rev, số_nguyên extra)
{
	số_nguyên all_mask = ((1u << (REV_SHIFT + num_rev)) - 1);
	số_nguyên all_revs = all_mask & ~((1u << REV_SHIFT) - 1);
	struct commit *commit;

	trong_khi ((commit = prio_queue_peek(queue))) {
		struct commit_list *parents;
		số_nguyên still_interesting = !!interesting(queue);
		số_nguyên flags = commit->object.flags & all_mask;

		nếu (!still_interesting && extra <= 0)
			break;

		prio_queue_get(queue);

		mark_seen(commit, seen_p);
		nếu ((flags & all_revs) == all_revs)
			flags |= UNINTERESTING;
		parents = commit->parents;

		trong_khi (parents) {
			struct commit *p = parents->item;
			số_nguyên this_flag = p->object.flags;
			parents = parents->next;
			nếu ((this_flag & flags) == flags)
				continue;
			repo_parse_commit(the_repository, p);
			nếu (mark_seen(p, seen_p) && !still_interesting)
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
	cho (;;) {
		số_nguyên changed = 0;
		struct commit_list *s;
		cho (s = *seen_p; s; s = s->next) {
			struct commit *c = s->item;
			struct commit_list *parents;

			nếu (((c->object.flags & all_revs) != all_revs) &&
			    !(c->object.flags & UNINTERESTING))
				continue;

			/* The current commit is either a merge base or
			 * already uninteresting one.  Mark its parents
			 * as uninteresting commits _only_ if they are
			 * already parsed.  No reason to find new ones
			 * here.
			 */
			parents = c->parents;
			trong_khi (parents) {
				struct commit *p = parents->item;
				parents = parents->next;
				nếu (!(p->object.flags & UNINTERESTING)) {
					p->object.flags |= UNINTERESTING;
					changed = 1;
				}
			}
		}
		nếu (!changed)
			break;
	}
}

static void show_one_commit(struct commit *commit, số_nguyên no_name)
{
	struct strbuf pretty = STRBUF_INIT;
	const char *pretty_str = "(unavailable)";
	struct commit_name *name = commit_to_name(commit);

	nếu (commit->object.parsed) {
		pp_commit_easy(CMIT_FMT_ONELINE, commit, &pretty);
		pretty_str = pretty.buf;
	}
	skip_prefix(pretty_str, "[PATCH] ", &pretty_str);

	nếu (!no_name) {
		nếu (name && name->head_name) {
			printf("[%s", name->head_name);
			nếu (name->generation) {
				nếu (name->generation == 1)
					printf("^");
				không_thì
					printf("~%d", name->generation);
			}
			printf("] ");
		}
		không_thì
			printf("[%s] ",
			       repo_find_unique_abbrev(the_repository, &commit->object.oid,
						       DEFAULT_ABBREV));
	}
	puts(pretty_str);
	strbuf_release(&pretty);
}

static char *ref_name[MAX_REVS + 1];
static số_nguyên ref_name_cnt;

static const char *find_digit_prefix(const char *s, số_nguyên *v)
{
	const char *p;
	số_nguyên ver;
	char ch;

	cho (p = s, ver = 0;
	     '0' <= (ch = *p) && ch <= '9';
	     p++)
		ver = ver * 10 + ch - '0';
	*v = ver;
	trả_về p;
}


static số_nguyên version_cmp(const char *a, const char *b)
{
	trong_khi (1) {
		số_nguyên va, vb;

		a = find_digit_prefix(a, &va);
		b = find_digit_prefix(b, &vb);
		nếu (va != vb)
			trả_về va - vb;

		trong_khi (1) {
			số_nguyên ca = *a;
			số_nguyên cb = *b;
			nếu ('0' <= ca && ca <= '9')
				ca = 0;
			nếu ('0' <= cb && cb <= '9')
				cb = 0;
			nếu (ca != cb)
				trả_về ca - cb;
			nếu (!ca)
				break;
			a++;
			b++;
		}
		nếu (!*a && !*b)
			trả_về 0;
	}
}

static số_nguyên compare_ref_name(const void *a_, const void *b_)
{
	const char * const*a = a_, * const*b = b_;
	trả_về version_cmp(*a, *b);
}

static void sort_ref_range(số_nguyên bottom, số_nguyên top)
{
	QSORT(ref_name + bottom, top - bottom, compare_ref_name);
}

static số_nguyên append_ref(const char *refname, const struct object_id *oid,
		      số_nguyên allow_dups)
{
	struct commit *commit = lookup_commit_reference_gently(the_repository,
							       oid, 1);
	số_nguyên i;

	nếu (!commit)
		trả_về 0;

	nếu (!allow_dups) {
		/* Avoid adding the same thing twice */
		cho (i = 0; i < ref_name_cnt; i++)
			nếu (!strcmp(refname, ref_name[i]))
				trả_về 0;
	}
	nếu (MAX_REVS <= ref_name_cnt) {
		warning(Q_("ignoring %s; cannot handle more than %d ref",
			   "ignoring %s; cannot handle more than %d refs",
			   MAX_REVS), refname, MAX_REVS);
		trả_về 0;
	}
	ref_name[ref_name_cnt++] = xstrdup(refname);
	ref_name[ref_name_cnt] = NULL;
	trả_về 0;
}

static số_nguyên append_head_ref(const struct reference *ref, void *cb_data UNUSED)
{
	struct object_id tmp;
	số_nguyên ofs = 11;
	nếu (!starts_with(ref->name, "refs/heads/"))
		trả_về 0;
	/* If both heads/foo and tags/foo exists, get_sha1 would
	 * get confused.
	 */
	nếu (repo_get_oid(the_repository, ref->name + ofs, &tmp) || !oideq(&tmp, ref->oid))
		ofs = 5;
	trả_về append_ref(ref->name + ofs, ref->oid, 0);
}

static số_nguyên append_remote_ref(const struct reference *ref, void *cb_data UNUSED)
{
	struct object_id tmp;
	số_nguyên ofs = 13;
	nếu (!starts_with(ref->name, "refs/remotes/"))
		trả_về 0;
	/* If both heads/foo and tags/foo exists, get_sha1 would
	 * get confused.
	 */
	nếu (repo_get_oid(the_repository, ref->name + ofs, &tmp) || !oideq(&tmp, ref->oid))
		ofs = 5;
	trả_về append_ref(ref->name + ofs, ref->oid, 0);
}

static số_nguyên append_tag_ref(const char *refname, const struct object_id *oid,
			  số_nguyên flag UNUSED, void *cb_data UNUSED)
{
	nếu (!starts_with(refname, "refs/tags/"))
		trả_về 0;
	trả_về append_ref(refname + 5, oid, 0);
}

static const char *match_ref_pattern = NULL;
static số_nguyên match_ref_slash = 0;

static số_nguyên append_matching_ref(const struct reference *ref, void *cb_data)
{
	/* we want to allow pattern hold/<asterisk> to show all
	 * branches under refs/heads/hold/, and v0.99.9? to show
	 * refs/tags/v0.99.9a and friends.
	 */
	const char *tail;
	số_nguyên slash = count_slashes(ref->name);
	cho (tail = ref->name; *tail && match_ref_slash < slash; )
		nếu (*tail++ == '/')
			slash--;
	nếu (!*tail)
		trả_về 0;
	nếu (wildmatch(match_ref_pattern, tail, 0))
		trả_về 0;
	nếu (starts_with(ref->name, "refs/heads/"))
		trả_về append_head_ref(ref, cb_data);
	nếu (starts_with(ref->name, "refs/tags/"))
		trả_về append_tag_ref(ref->name, ref->oid, ref->flags, cb_data);
	trả_về append_ref(ref->name, ref->oid, 0);
}

static void snarf_refs(số_nguyên head, số_nguyên remotes)
{
	nếu (head) {
		số_nguyên orig_cnt = ref_name_cnt;

		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_head_ref, NULL);
		sort_ref_range(orig_cnt, ref_name_cnt);
	}
	nếu (remotes) {
		số_nguyên orig_cnt = ref_name_cnt;

		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_remote_ref, NULL);
		sort_ref_range(orig_cnt, ref_name_cnt);
	}
}

static số_nguyên rev_is_head(const char *head, const char *name)
{
	nếu (!head)
		trả_về 0;
	skip_prefix(head, "refs/heads/", &head);
	nếu (!skip_prefix(name, "refs/heads/", &name))
		skip_prefix(name, "heads/", &name);
	trả_về !strcmp(head, name);
}

static số_nguyên show_merge_base(const struct commit_list *seen, số_nguyên num_rev)
{
	số_nguyên all_mask = ((1u << (REV_SHIFT + num_rev)) - 1);
	số_nguyên all_revs = all_mask & ~((1u << REV_SHIFT) - 1);
	số_nguyên exit_status = 1;

	cho (const struct commit_list *s = seen; s; s = s->next) {
		struct commit *commit = s->item;
		số_nguyên flags = commit->object.flags & all_mask;
		nếu (!(flags & UNINTERESTING) &&
		    ((flags & all_revs) == all_revs)) {
			puts(oid_to_hex(&commit->object.oid));
			exit_status = 0;
			commit->object.flags |= UNINTERESTING;
		}
	}
	trả_về exit_status;
}

static số_nguyên show_independent(struct commit **rev,
			    số_nguyên num_rev,
			    unsigned số_nguyên *rev_mask)
{
	số_nguyên i;

	cho (i = 0; i < num_rev; i++) {
		struct commit *commit = rev[i];
		unsigned số_nguyên flag = rev_mask[i];

		nếu (commit->object.flags == flag)
			puts(oid_to_hex(&commit->object.oid));
		commit->object.flags |= UNINTERESTING;
	}
	trả_về 0;
}

static void append_one_rev(const char *av)
{
	struct object_id revkey;
	nếu (!repo_get_oid(the_repository, av, &revkey)) {
		append_ref(av, &revkey, 0);
		trả_về;
	}
	nếu (strpbrk(av, "*?[")) {
		/* glob style match */
		số_nguyên saved_matches = ref_name_cnt;

		match_ref_pattern = av;
		match_ref_slash = count_slashes(av);
		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_matching_ref, NULL);
		nếu (saved_matches == ref_name_cnt &&
		    ref_name_cnt < MAX_REVS)
			error(_("no matching refs with %s"), av);
		sort_ref_range(saved_matches, ref_name_cnt);
		trả_về;
	}
	die("bad sha1 reference %s", av);
}

static số_nguyên git_show_branch_config(const char *var, const char *value,
				  const struct config_context *ctx, void *cb)
{
	nếu (!strcmp(var, "showbranch.default")) {
		nếu (!value)
			trả_về config_error_nonbool(var);
		/*
		 * default_arg is now passed to parse_options(), so we need to
		 * mimic the real argv a bit better.
		 */
		nếu (!default_args.nr)
			strvec_push(&default_args, "show-branch");
		strvec_push(&default_args, value);
		trả_về 0;
	}

	nếu (!strcmp(var, "color.showbranch")) {
		showbranch_use_color = git_config_colorbool(var, value);
		trả_về 0;
	}

	nếu (git_color_config(var, value, cb) < 0)
		trả_về -1;

	trả_về git_default_config(var, value, ctx, cb);
}

static số_nguyên omit_in_dense(struct commit *commit, struct commit **rev, số_nguyên n)
{
	/* If the commit is tip of the named branches, do not
	 * omit it.
	 * Otherwise, if it is a merge that is reachable from only one
	 * tip, it is not that interesting.
	 */
	số_nguyên i, flag, count;
	cho (i = 0; i < n; i++)
		nếu (rev[i] == commit)
			trả_về 0;
	flag = commit->object.flags;
	cho (i = count = 0; i < n; i++) {
		nếu (flag & (1u << (i + REV_SHIFT)))
			count++;
	}
	nếu (count == 1)
		trả_về 1;
	trả_về 0;
}

static số_nguyên reflog = 0;

static số_nguyên parse_reflog_param(const struct option *opt, const char *arg,
			      số_nguyên unset)
{
	char *ep;
	const char **base = (const char **)opt->value;
	BUG_ON_OPT_NEG(unset);
	nếu (!arg)
		arg = "";
	reflog = strtoul(arg, &ep, 10);
	nếu (*ep == ',')
		*base = ep + 1;
	không_thì nếu (*ep)
		trả_về error("unrecognized reflog param '%s'", arg);
	không_thì
		*base = NULL;
	nếu (reflog <= 0)
		reflog = DEFAULT_REFLOG;
	trả_về 0;
}

số_nguyên cmd_show_branch(số_nguyên ac,
		const char **av,
		const char *prefix,
		struct repository *repo UNUSED)
{
	struct commit *rev[MAX_REVS], *commit;
	char *reflog_msg[MAX_REVS] = {0};
	struct commit_list *seen = NULL;
	struct prio_queue queue = { compare_commits_by_commit_date };
	unsigned số_nguyên rev_mask[MAX_REVS];
	số_nguyên num_rev, i, extra = 0;
	số_nguyên all_heads = 0, all_remotes = 0;
	số_nguyên all_mask, all_revs;
	enum rev_sort_order sort_order = REV_SORT_IN_GRAPH_ORDER;
	char *head;
	struct object_id head_oid;
	số_nguyên merge_base = 0;
	số_nguyên independent = 0;
	số_nguyên no_name = 0;
	số_nguyên sha1_name = 0;
	số_nguyên shown_merge_point = 0;
	số_nguyên with_current_branch = 0;
	số_nguyên head_at = -1;
	số_nguyên topics = 0;
	số_nguyên sparse = 0;
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
	số_nguyên ret;

	init_commit_name_slab(&name_slab);

	repo_config(the_repository, git_show_branch_config, NULL);

	/* If nothing is specified, try the default first */
	nếu (ac == 1 && default_args.nr) {
		DUP_ARRAY(args_copy, default_args.v, default_args.nr);
		ac = default_args.nr;
		av = args_copy;
	}

	ac = parse_options(ac, av, prefix, builtin_show_branch_options,
			   show_branch_usage, PARSE_OPT_STOP_AT_NON_OPTION);
	nếu (all_heads)
		all_remotes = 1;

	nếu (extra || reflog) {
		/* "listing" mode is incompatible with
		 * independent nor merge-base modes.
		 */
		nếu (independent || merge_base)
			usage_with_options(show_branch_usage,
					   builtin_show_branch_options);
		nếu (reflog && ((0 < extra) || all_heads || all_remotes))
			/*
			 * Asking for --more in reflog mode does not
			 * make sense.  --list is Ok.
			 *
			 * Also --all and --remotes do not make sense either.
			 */
			die(_("options '%s' and '%s' cannot be used together"), "--reflog",
				"--all/--remotes/--independent/--merge-base");
	}

	nếu (with_current_branch && reflog)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--reflog", "--current");

	/* If nothing is specified, show all branches by default */
	nếu (ac <= topics && all_heads + all_remotes == 0)
		all_heads = 1;

	nếu (reflog) {
		struct object_id oid;
		char *ref;
		số_nguyên base = 0;
		unsigned số_nguyên flags = 0;

		nếu (ac == 0) {
			static const char *fake_av[2];

			fake_av[0] = refs_resolve_refdup(get_main_ref_store(the_repository),
							 "HEAD",
							 RESOLVE_REF_READING,
							 &oid,
							 NULL);
			fake_av[1] = NULL;
			av = fake_av;
			ac = 1;
			nếu (!*av)
				die(_("no branches given, and HEAD is not valid"));
		}
		nếu (ac != 1)
			die(_("--reflog option needs one branch name"));

		nếu (MAX_REVS < reflog)
			die(Q_("only %d entry can be shown at one time.",
			       "only %d entries can be shown at one time.",
			       MAX_REVS), MAX_REVS);
		nếu (!repo_dwim_ref(the_repository, *av, strlen(*av), &oid,
				   &ref, 0))
			die(_("no such ref %s"), *av);

		/* Has the base been specified? */
		nếu (reflog_base) {
			char *ep;
			base = strtoul(reflog_base, &ep, 10);
			nếu (*ep) {
				/* Ah, that is a date spec... */
				timestamp_t at;
				at = approxidate(reflog_base);
				read_ref_at(get_main_ref_store(the_repository),
					    ref, flags, at, -1, &oid, NULL,
					    NULL, NULL, &base);
			}
		}

		cho (i = 0; i < reflog; i++) {
			char *logmsg = NULL;
			char *nth_desc;
			const char *msg;
			char *end;
			timestamp_t timestamp;
			số_nguyên tz;

			nếu (read_ref_at(get_main_ref_store(the_repository),
					ref, flags, 0, base + i, &oid, &logmsg,
					&timestamp, &tz, NULL)) {
				free(logmsg);
				reflog = i;
				break;
			}

			end = strchr(logmsg, '\n');
			nếu (end)
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
	không_thì {
		trong_khi (0 < ac) {
			append_one_rev(*av);
			ac--; av++;
		}
		nếu (all_heads + all_remotes)
			snarf_refs(all_heads, all_remotes);
	}

	head = refs_resolve_refdup(get_main_ref_store(the_repository), "HEAD",
				   RESOLVE_REF_READING,
				   &head_oid, NULL);

	nếu (with_current_branch && head) {
		số_nguyên has_head = 0;
		cho (i = 0; !has_head && i < ref_name_cnt; i++) {
			/* We are only interested in adding the branch
			 * HEAD points at.
			 */
			nếu (rev_is_head(head, ref_name[i]))
				has_head++;
		}
		nếu (!has_head) {
			const char *name = head;
			skip_prefix(name, "refs/heads/", &name);
			append_one_rev(name);
		}
	}

	nếu (!ref_name_cnt) {
		fprintf(stderr, "No revs to be shown.\n");
		ret = 0;
		goto out;
	}

	cho (num_rev = 0; ref_name[num_rev]; num_rev++) {
		struct object_id revkey;
		unsigned số_nguyên flag = 1u << (num_rev + REV_SHIFT);

		nếu (MAX_REVS <= num_rev)
			die(Q_("cannot handle more than %d rev.",
			       "cannot handle more than %d revs.",
			       MAX_REVS), MAX_REVS);
		nếu (repo_get_oid(the_repository, ref_name[num_rev], &revkey))
			die(_("'%s' is not a valid ref."), ref_name[num_rev]);
		commit = lookup_commit_reference(the_repository, &revkey);
		nếu (!commit)
			die(_("cannot find commit %s (%s)"),
			    ref_name[num_rev], oid_to_hex(&revkey));
		repo_parse_commit(the_repository, commit);
		mark_seen(commit, &seen);

		/* rev#0 uses bit REV_SHIFT, rev#1 uses bit REV_SHIFT+1,
		 * and so on.  REV_SHIFT bits from bit 0 are used for
		 * internal bookkeeping.
		 */
		commit->object.flags |= flag;
		nếu (commit->object.flags == flag)
			prio_queue_put(&queue, commit);
		rev[num_rev] = commit;
	}
	cho (i = 0; i < num_rev; i++)
		rev_mask[i] = rev[i]->object.flags;

	nếu (0 <= extra)
		join_revs(&queue, &seen, num_rev, extra);

	commit_list_sort_by_date(&seen);

	nếu (merge_base) {
		ret = show_merge_base(seen, num_rev);
		goto out;
	}

	nếu (independent) {
		ret = show_independent(rev, num_rev, rev_mask);
		goto out;
	}

	/* Show list; --more=-1 means list-only */
	nếu (1 < num_rev || extra < 0) {
		cho (i = 0; i < num_rev; i++) {
			số_nguyên j;
			số_nguyên is_head = rev_is_head(head, ref_name[i]) &&
				      oideq(&head_oid, &rev[i]->object.oid);
			nếu (extra < 0)
				printf("%c [%s] ",
				       is_head ? '*' : ' ', ref_name[i]);
			không_thì {
				cho (j = 0; j < i; j++)
					putchar(' ');
				printf("%s%c%s [%s] ",
				       get_color_code(i),
				       is_head ? '*' : '!',
				       get_color_reset_code(), ref_name[i]);
			}

			nếu (!reflog) {
				/* header lines never need name */
				show_one_commit(rev[i], 1);
			}
			không_thì
				puts(reflog_msg[i]);

			nếu (is_head)
				head_at = i;
		}
		nếu (0 <= extra) {
			cho (i = 0; i < num_rev; i++)
				putchar('-');
			putchar('\n');
		}
	}
	nếu (extra < 0) {
		ret = 0;
		goto out;
	}

	/* Sort topologically */
	sort_in_topological_order(&seen, sort_order);

	/* Give names to commits */
	nếu (!sha1_name && !no_name)
		name_commits(seen, rev, ref_name, num_rev);

	all_mask = ((1u << (REV_SHIFT + num_rev)) - 1);
	all_revs = all_mask & ~((1u << REV_SHIFT) - 1);

	cho (struct commit_list *l = seen; l; l = l->next) {
		struct commit *commit = l->item;
		số_nguyên this_flag = commit->object.flags;
		số_nguyên is_merge_point = ((this_flag & all_revs) == all_revs);

		shown_merge_point |= is_merge_point;

		nếu (1 < num_rev) {
			số_nguyên is_merge = !!(commit->parents &&
					  commit->parents->next);
			nếu (topics &&
			    !is_merge_point &&
			    (this_flag & (1u << REV_SHIFT)))
				continue;
			nếu (!sparse && is_merge &&
			    omit_in_dense(commit, rev, num_rev))
				continue;
			cho (i = 0; i < num_rev; i++) {
				số_nguyên mark;
				nếu (!(this_flag & (1u << (i + REV_SHIFT))))
					mark = ' ';
				không_thì nếu (is_merge)
					mark = '-';
				không_thì nếu (i == head_at)
					mark = '*';
				không_thì
					mark = '+';
				nếu (mark == ' ')
					putchar(mark);
				không_thì
					printf("%s%c%s",
					       get_color_code(i),
					       mark, get_color_reset_code());
			}
			putchar(' ');
		}
		show_one_commit(commit, no_name);

		nếu (shown_merge_point && --extra < 0)
			break;
	}

	ret = 0;

out:
	cho (size_t i = 0; i < ARRAY_SIZE(reflog_msg); i++)
		free(reflog_msg[i]);
	commit_list_free(seen);
	clear_prio_queue(&queue);
	free(args_copy);
	free(head);
	trả_về ret;
}
