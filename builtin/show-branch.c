#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#شامل "builtin.h"
#شامل "config.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "hash.h"
#شامل "hex.h"
#شامل "pretty.h"
#شامل "refs.h"
#شامل "color.h"
#شامل "strvec.h"
#شامل "object-name.h"
#شامل "parse-options.h"

#شامل "dir.h"
#شامل "commit-slab.h"
#شامل "date.h"
#شامل "wildmatch.h"
#شامل "prio-queue.h"

ایستا ثابت نویسه*ثابت show_branch_usage[] = {
    N_("git show-branch [-a | --all] [-r | --remotes] [--topo-order | --date-order]\n"
       "                [--current] [--color[=<when>] | --no-color] [--sparse]\n"
       "                [--more=<n> | --list | --independent | --merge-base]\n"
       "                [--no-name | --sha1-name] [--topics]\n"
       "                [(<rev> | <glob>)...]"),
    N_("git show-branch (-g | --reflog)[=<n>[,<base>]] [--list] [<ref>]"),
    پوچ
};

ایستا شمارش git_colorbool showbranch_use_color = GIT_COLOR_UNKNOWN;

ایستا ساختار strvec default_args = STRVEC_INIT;

/*
 * TODO: convert this use of commit->object.flags to commit-slab
 * instead to store a pointer to ref name directly. Then use the same
 * UNINTERESTING definition from revision.h here.
 */
#define UNINTERESTING	01

#define REV_SHIFT	 2
#define MAX_REVS	(FLAG_BITS - REV_SHIFT) /* should not exceed bits_per_int - REV_SHIFT */

#define DEFAULT_REFLOG	4

ایستا ثابت نویسه *get_color_code(صحیح idx)
{
	اگر (want_color(showbranch_use_color))
		بازگشت column_colors_ansi[idx % column_colors_ansi_max];
	بازگشت "";
}

ایستا ثابت نویسه *get_color_reset_code(تهی)
{
	اگر (want_color(showbranch_use_color))
		بازگشت GIT_COLOR_RESET;
	بازگشت "";
}

ایستا ساختار commit *interesting(ساختار prio_queue *queue)
{
	ساختار commit *commit;
	prio_queue_for_each(queue, commit) {
		اگر (!(commit->object.flags & UNINTERESTING))
			بازگشت commit;
	}
	بازگشت پوچ;
}

ساختار commit_name {
	ثابت نویسه *head_name; /* which head's ancestor? */
	صحیح generation; /* how many parents away from head_name */
};

define_commit_slab(commit_name_slab, ساختار commit_name *);
ایستا ساختار commit_name_slab name_slab;

ایستا ساختار commit_name *commit_to_name(ساختار commit *commit)
{
	بازگشت *commit_name_slab_at(&name_slab, commit);
}


/* Name the commit as nth generation ancestor of head_name;
 * we count only the first-parent relationship for naming purposes.
 */
ایستا تهی name_commit(ساختار commit *commit, ثابت نویسه *head_name, صحیح nth)
{
	ساختار commit_name *name;

	name = *commit_name_slab_at(&name_slab, commit);
	اگر (!name) {
		name = xmalloc(اندازه(*name));
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
ایستا تهی name_parent(ساختار commit *commit, ساختار commit *parent)
{
	ساختار commit_name *commit_name = commit_to_name(commit);
	ساختار commit_name *parent_name = commit_to_name(parent);
	اگر (!commit_name)
		بازگشت;
	اگر (!parent_name ||
	    commit_name->generation + 1 < parent_name->generation)
		name_commit(parent, commit_name->head_name,
			    commit_name->generation + 1);
}

ایستا صحیح name_first_parent_chain(ساختار commit *c)
{
	صحیح i = 0;
	وقتی (c) {
		ساختار commit *p;
		اگر (!commit_to_name(c))
			بشکن;
		اگر (!c->parents)
			بشکن;
		p = c->parents->item;
		اگر (!commit_to_name(p)) {
			name_parent(c, p);
			i++;
		}
		وگرنه
			بشکن;
		c = p;
	}
	بازگشت i;
}

ایستا تهی name_commits(ساختار commit_list *list,
			 ساختار commit **rev,
			 نویسه **ref_name,
			 صحیح رقم_rev)
{
	ساختار commit_list *cl;
	ساختار commit *c;
	صحیح i;

	/* First give names to the given heads */
	برای (cl = list; cl; cl = cl->next) {
		c = cl->item;
		اگر (commit_to_name(c))
			ادامه;
		برای (i = 0; i < رقم_rev; i++) {
			اگر (rev[i] == c) {
				name_commit(c, ref_name[i], 0);
				بشکن;
			}
		}
	}

	/* Then commits on the first parent ancestry chain */
	انجامبده {
		i = 0;
		برای (cl = list; cl; cl = cl->next) {
			i += name_first_parent_chain(cl->item);
		}
	} وقتی (i);

	/* Finally, any unnamed commits */
	انجامبده {
		i = 0;
		برای (cl = list; cl; cl = cl->next) {
			ساختار commit_list *parents;
			ساختار commit_name *n;
			صحیح nth;
			c = cl->item;
			اگر (!commit_to_name(c))
				ادامه;
			n = commit_to_name(c);
			parents = c->parents;
			nth = 0;
			وقتی (parents) {
				ساختار commit *p = parents->item;
				ساختار strbuf newname = STRBUF_INIT;
				parents = parents->next;
				nth++;
				اگر (commit_to_name(p))
					ادامه;
				انتخاب (n->generation) {
				حالت 0:
					strbuf_addstr(&newname, n->head_name);
					بشکن;
				حالت 1:
					strbuf_addf(&newname, "%s^", n->head_name);
					بشکن;
				پیشفرض:
					strbuf_addf(&newname, "%s~%d",
						    n->head_name, n->generation);
					بشکن;
				}
				اگر (nth == 1)
					strbuf_addch(&newname, '^');
				وگرنه
					strbuf_addf(&newname, "^%d", nth);
				name_commit(p, strbuf_detach(&newname, پوچ), 0);
				i++;
				name_first_parent_chain(p);
			}
		}
	} وقتی (i);
}

ایستا صحیح mark_seen(ساختار commit *commit, ساختار commit_list **seen_p)
{
	اگر (!commit->object.flags) {
		commit_list_درج(commit, seen_p);
		بازگشت 1;
	}
	بازگشت 0;
}

ایستا تهی join_revs(ساختار prio_queue *queue,
		      ساختار commit_list **seen_p,
		      صحیح رقم_rev, صحیح extra)
{
	صحیح all_mask = ((1u << (REV_SHIFT + رقم_rev)) - 1);
	صحیح all_revs = all_mask & ~((1u << REV_SHIFT) - 1);
	ساختار commit *commit;

	وقتی ((commit = prio_queue_peek(queue))) {
		ساختار commit_list *parents;
		صحیح still_interesting = !!interesting(queue);
		صحیح flags = commit->object.flags & all_mask;

		اگر (!still_interesting && extra <= 0)
			بشکن;

		prio_queue_get(queue);

		mark_seen(commit, seen_p);
		اگر ((flags & all_revs) == all_revs)
			flags |= UNINTERESTING;
		parents = commit->parents;

		وقتی (parents) {
			ساختار commit *p = parents->item;
			صحیح this_flag = p->object.flags;
			parents = parents->next;
			اگر ((this_flag & flags) == flags)
				ادامه;
			repo_parse_commit(the_repository, p);
			اگر (mark_seen(p, seen_p) && !still_interesting)
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
	برای (;;) {
		صحیح changed = 0;
		ساختار commit_list *s;
		برای (s = *seen_p; s; s = s->next) {
			ساختار commit *c = s->item;
			ساختار commit_list *parents;

			اگر (((c->object.flags & all_revs) != all_revs) &&
			    !(c->object.flags & UNINTERESTING))
				ادامه;

			/* The current commit is either a merge base or
			 * already uninteresting one.  Mark its parents
			 * as uninteresting commits _only_ if they are
			 * already parsed.  No reason to find new ones
			 * here.
			 */
			parents = c->parents;
			وقتی (parents) {
				ساختار commit *p = parents->item;
				parents = parents->next;
				اگر (!(p->object.flags & UNINTERESTING)) {
					p->object.flags |= UNINTERESTING;
					changed = 1;
				}
			}
		}
		اگر (!changed)
			بشکن;
	}
}

ایستا تهی show_one_commit(ساختار commit *commit, صحیح no_name)
{
	ساختار strbuf pretty = STRBUF_INIT;
	ثابت نویسه *pretty_str = "(unavailable)";
	ساختار commit_name *name = commit_to_name(commit);

	اگر (commit->object.parsed) {
		pp_commit_easy(CMIT_FMT_ONELINE, commit, &pretty);
		pretty_str = pretty.buf;
	}
	skip_prefix(pretty_str, "[PATCH] ", &pretty_str);

	اگر (!no_name) {
		اگر (name && name->head_name) {
			printf("[%s", name->head_name);
			اگر (name->generation) {
				اگر (name->generation == 1)
					printf("^");
				وگرنه
					printf("~%d", name->generation);
			}
			printf("] ");
		}
		وگرنه
			printf("[%s] ",
			       repo_find_unique_abbrev(the_repository, &commit->object.oid,
						       DEFAULT_ABBREV));
	}
	puts(pretty_str);
	strbuf_release(&pretty);
}

ایستا نویسه *ref_name[MAX_REVS + 1];
ایستا صحیح ref_name_cnt;

ایستا ثابت نویسه *find_digit_prefix(ثابت نویسه *s, صحیح *v)
{
	ثابت نویسه *p;
	صحیح ver;
	نویسه ch;

	برای (p = s, ver = 0;
	     '0' <= (ch = *p) && ch <= '9';
	     p++)
		ver = ver * 10 + ch - '0';
	*v = ver;
	بازگشت p;
}


ایستا صحیح version_cmp(ثابت نویسه *a, ثابت نویسه *b)
{
	وقتی (1) {
		صحیح va, vb;

		a = find_digit_prefix(a, &va);
		b = find_digit_prefix(b, &vb);
		اگر (va != vb)
			بازگشت va - vb;

		وقتی (1) {
			صحیح ca = *a;
			صحیح cb = *b;
			اگر ('0' <= ca && ca <= '9')
				ca = 0;
			اگر ('0' <= cb && cb <= '9')
				cb = 0;
			اگر (ca != cb)
				بازگشت ca - cb;
			اگر (!ca)
				بشکن;
			a++;
			b++;
		}
		اگر (!*a && !*b)
			بازگشت 0;
	}
}

ایستا صحیح compare_ref_name(ثابت تهی *a_, ثابت تهی *b_)
{
	ثابت نویسه * ثابت*a = a_, * ثابت*b = b_;
	بازگشت version_cmp(*a, *b);
}

ایستا تهی sort_ref_range(صحیح bottom, صحیح بالاترین)
{
	QSORT(ref_name + bottom, بالاترین - bottom, compare_ref_name);
}

ایستا صحیح append_ref(ثابت نویسه *refname, ثابت ساختار object_id *oid,
		      صحیح allow_dups)
{
	ساختار commit *commit = lookup_commit_reference_gently(the_repository,
							       oid, 1);
	صحیح i;

	اگر (!commit)
		بازگشت 0;

	اگر (!allow_dups) {
		/* Avoid adding the same thing twice */
		برای (i = 0; i < ref_name_cnt; i++)
			اگر (!strcmp(refname, ref_name[i]))
				بازگشت 0;
	}
	اگر (MAX_REVS <= ref_name_cnt) {
		warning(Q_("ignoring %s; cannot handle more than %d ref",
			   "ignoring %s; cannot handle more than %d refs",
			   MAX_REVS), refname, MAX_REVS);
		بازگشت 0;
	}
	ref_name[ref_name_cnt++] = xstrdup(refname);
	ref_name[ref_name_cnt] = پوچ;
	بازگشت 0;
}

ایستا صحیح append_head_ref(ثابت ساختار reference *ref, تهی *cb_data UNUSED)
{
	ساختار object_id tmp;
	صحیح ofs = 11;
	اگر (!starts_with(ref->name, "refs/heads/"))
		بازگشت 0;
	/* If both heads/foo and tags/foo exists, get_sha1 would
	 * get confused.
	 */
	اگر (repo_get_oid(the_repository, ref->name + ofs, &tmp) || !oideq(&tmp, ref->oid))
		ofs = 5;
	بازگشت append_ref(ref->name + ofs, ref->oid, 0);
}

ایستا صحیح append_remote_ref(ثابت ساختار reference *ref, تهی *cb_data UNUSED)
{
	ساختار object_id tmp;
	صحیح ofs = 13;
	اگر (!starts_with(ref->name, "refs/remotes/"))
		بازگشت 0;
	/* If both heads/foo and tags/foo exists, get_sha1 would
	 * get confused.
	 */
	اگر (repo_get_oid(the_repository, ref->name + ofs, &tmp) || !oideq(&tmp, ref->oid))
		ofs = 5;
	بازگشت append_ref(ref->name + ofs, ref->oid, 0);
}

ایستا صحیح append_tag_ref(ثابت نویسه *refname, ثابت ساختار object_id *oid,
			  صحیح flag UNUSED, تهی *cb_data UNUSED)
{
	اگر (!starts_with(refname, "refs/tags/"))
		بازگشت 0;
	بازگشت append_ref(refname + 5, oid, 0);
}

ایستا ثابت نویسه *match_ref_pattern = پوچ;
ایستا صحیح match_ref_slash = 0;

ایستا صحیح append_matching_ref(ثابت ساختار reference *ref, تهی *cb_data)
{
	/* we want to allow pattern hold/<asterisk> to show all
	 * branches under refs/heads/hold/, and v0.99.9? to show
	 * refs/tags/v0.99.9a and friends.
	 */
	ثابت نویسه *tail;
	صحیح slash = شمار_slashes(ref->name);
	برای (tail = ref->name; *tail && match_ref_slash < slash; )
		اگر (*tail++ == '/')
			slash--;
	اگر (!*tail)
		بازگشت 0;
	اگر (wildmatch(match_ref_pattern, tail, 0))
		بازگشت 0;
	اگر (starts_with(ref->name, "refs/heads/"))
		بازگشت append_head_ref(ref, cb_data);
	اگر (starts_with(ref->name, "refs/tags/"))
		بازگشت append_tag_ref(ref->name, ref->oid, ref->flags, cb_data);
	بازگشت append_ref(ref->name, ref->oid, 0);
}

ایستا تهی snarf_refs(صحیح head, صحیح remotes)
{
	اگر (head) {
		صحیح orig_cnt = ref_name_cnt;

		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_head_ref, پوچ);
		sort_ref_range(orig_cnt, ref_name_cnt);
	}
	اگر (remotes) {
		صحیح orig_cnt = ref_name_cnt;

		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_remote_ref, پوچ);
		sort_ref_range(orig_cnt, ref_name_cnt);
	}
}

ایستا صحیح rev_is_head(ثابت نویسه *head, ثابت نویسه *name)
{
	اگر (!head)
		بازگشت 0;
	skip_prefix(head, "refs/heads/", &head);
	اگر (!skip_prefix(name, "refs/heads/", &name))
		skip_prefix(name, "heads/", &name);
	بازگشت !strcmp(head, name);
}

ایستا صحیح show_merge_base(ثابت ساختار commit_list *seen, صحیح رقم_rev)
{
	صحیح all_mask = ((1u << (REV_SHIFT + رقم_rev)) - 1);
	صحیح all_revs = all_mask & ~((1u << REV_SHIFT) - 1);
	صحیح exit_status = 1;

	برای (ثابت ساختار commit_list *s = seen; s; s = s->next) {
		ساختار commit *commit = s->item;
		صحیح flags = commit->object.flags & all_mask;
		اگر (!(flags & UNINTERESTING) &&
		    ((flags & all_revs) == all_revs)) {
			puts(oid_to_hex(&commit->object.oid));
			exit_status = 0;
			commit->object.flags |= UNINTERESTING;
		}
	}
	بازگشت exit_status;
}

ایستا صحیح show_independent(ساختار commit **rev,
			    صحیح رقم_rev,
			    unsigned صحیح *rev_mask)
{
	صحیح i;

	برای (i = 0; i < رقم_rev; i++) {
		ساختار commit *commit = rev[i];
		unsigned صحیح flag = rev_mask[i];

		اگر (commit->object.flags == flag)
			puts(oid_to_hex(&commit->object.oid));
		commit->object.flags |= UNINTERESTING;
	}
	بازگشت 0;
}

ایستا تهی append_one_rev(ثابت نویسه *av)
{
	ساختار object_id revkey;
	اگر (!repo_get_oid(the_repository, av, &revkey)) {
		append_ref(av, &revkey, 0);
		بازگشت;
	}
	اگر (strpbrk(av, "*?[")) {
		/* glob style match */
		صحیح saved_matches = ref_name_cnt;

		match_ref_pattern = av;
		match_ref_slash = شمار_slashes(av);
		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_matching_ref, پوچ);
		اگر (saved_matches == ref_name_cnt &&
		    ref_name_cnt < MAX_REVS)
			error(_("no matching refs with %s"), av);
		sort_ref_range(saved_matches, ref_name_cnt);
		بازگشت;
	}
	die("bad sha1 reference %s", av);
}

ایستا صحیح git_show_branch_config(ثابت نویسه *var, ثابت نویسه *مقدار,
				  ثابت ساختار config_context *ctx, تهی *cb)
{
	اگر (!strcmp(var, "showbranch.default")) {
		اگر (!مقدار)
			بازگشت config_error_nonbool(var);
		/*
		 * default_arg is now passed to parse_options(), so we need to
		 * mimic the real argv a bit better.
		 */
		اگر (!default_args.nr)
			strvec_افزودن(&default_args, "show-branch");
		strvec_افزودن(&default_args, مقدار);
		بازگشت 0;
	}

	اگر (!strcmp(var, "color.showbranch")) {
		showbranch_use_color = git_config_colorbool(var, مقدار);
		بازگشت 0;
	}

	اگر (git_color_config(var, مقدار, cb) < 0)
		بازگشت -1;

	بازگشت git_default_config(var, مقدار, ctx, cb);
}

ایستا صحیح omit_in_dense(ساختار commit *commit, ساختار commit **rev, صحیح n)
{
	/* If the commit is tip of the named branches, do not
	 * omit it.
	 * Otherwise, if it is a merge that is reachable from only one
	 * tip, it is not that interesting.
	 */
	صحیح i, flag, شمار;
	برای (i = 0; i < n; i++)
		اگر (rev[i] == commit)
			بازگشت 0;
	flag = commit->object.flags;
	برای (i = شمار = 0; i < n; i++) {
		اگر (flag & (1u << (i + REV_SHIFT)))
			شمار++;
	}
	اگر (شمار == 1)
		بازگشت 1;
	بازگشت 0;
}

ایستا صحیح reflog = 0;

ایستا صحیح parse_reflog_param(ثابت ساختار option *opt, ثابت نویسه *arg,
			      صحیح unset)
{
	نویسه *ep;
	ثابت نویسه **base = (ثابت نویسه **)opt->مقدار;
	BUG_ON_OPT_NEG(unset);
	اگر (!arg)
		arg = "";
	reflog = strtoul(arg, &ep, 10);
	اگر (*ep == ',')
		*base = ep + 1;
	وگرنه اگر (*ep)
		بازگشت error("unrecognized reflog param '%s'", arg);
	وگرنه
		*base = پوچ;
	اگر (reflog <= 0)
		reflog = DEFAULT_REFLOG;
	بازگشت 0;
}

صحیح cmd_show_branch(صحیح ac,
		ثابت نویسه **av,
		ثابت نویسه *prefix,
		ساختار repository *repo UNUSED)
{
	ساختار commit *rev[MAX_REVS], *commit;
	نویسه *reflog_پیام[MAX_REVS] = {0};
	ساختار commit_list *seen = پوچ;
	ساختار prio_queue queue = { compare_commits_by_commit_date };
	unsigned صحیح rev_mask[MAX_REVS];
	صحیح رقم_rev, i, extra = 0;
	صحیح all_heads = 0, all_remotes = 0;
	صحیح all_mask, all_revs;
	شمارش rev_sort_order sort_order = REV_SORT_IN_GRAPH_ORDER;
	نویسه *head;
	ساختار object_id head_oid;
	صحیح merge_base = 0;
	صحیح independent = 0;
	صحیح no_name = 0;
	صحیح sha1_name = 0;
	صحیح shown_merge_point = 0;
	صحیح with_current_branch = 0;
	صحیح head_at = -1;
	صحیح topics = 0;
	صحیح sparse = 0;
	ثابت نویسه *reflog_base = پوچ;
	ساختار option builtin_show_branch_options[] = {
		OPT_BOOL('a', "all", &all_heads,
			 N_("show remote-tracking and local branches")),
		OPT_BOOL('r', "remotes", &all_remotes,
			 N_("show remote-tracking branches")),
		OPT__COLOR(&showbranch_use_color,
			    N_("color '*!+-' corresponding to the branch")),
		{
			.type = OPTION_INTEGER,
			.long_name = "more",
			.مقدار = &extra,
			.precision = اندازه(extra),
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
	ثابت نویسه **args_copy = پوچ;
	صحیح ret;

	init_commit_name_slab(&name_slab);

	repo_config(the_repository, git_show_branch_config, پوچ);

	/* If nothing is specified, try the default first */
	اگر (ac == 1 && default_args.nr) {
		DUP_ARRAY(args_copy, default_args.v, default_args.nr);
		ac = default_args.nr;
		av = args_copy;
	}

	ac = parse_options(ac, av, prefix, builtin_show_branch_options,
			   show_branch_usage, PARSE_OPT_STOP_AT_NON_OPTION);
	اگر (all_heads)
		all_remotes = 1;

	اگر (extra || reflog) {
		/* "listing" mode is incompatible with
		 * independent nor merge-base modes.
		 */
		اگر (independent || merge_base)
			usage_with_options(show_branch_usage,
					   builtin_show_branch_options);
		اگر (reflog && ((0 < extra) || all_heads || all_remotes))
			/*
			 * Asking for --more in reflog mode does not
			 * make sense.  --list is Ok.
			 *
			 * Also --all and --remotes do not make sense either.
			 */
			die(_("options '%s' and '%s' cannot be used together"), "--reflog",
				"--all/--remotes/--independent/--merge-base");
	}

	اگر (with_current_branch && reflog)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--reflog", "--current");

	/* If nothing is specified, show all branches by default */
	اگر (ac <= topics && all_heads + all_remotes == 0)
		all_heads = 1;

	اگر (reflog) {
		ساختار object_id oid;
		نویسه *ref;
		صحیح base = 0;
		unsigned صحیح flags = 0;

		اگر (ac == 0) {
			ایستا ثابت نویسه *fake_av[2];

			fake_av[0] = refs_resolve_refdup(get_main_ref_store(the_repository),
							 "HEAD",
							 RESOLVE_REF_READING,
							 &oid,
							 پوچ);
			fake_av[1] = پوچ;
			av = fake_av;
			ac = 1;
			اگر (!*av)
				die(_("no branches given, and HEAD is not valid"));
		}
		اگر (ac != 1)
			die(_("--reflog option needs one branch name"));

		اگر (MAX_REVS < reflog)
			die(Q_("only %d entry can be shown at one time.",
			       "only %d entries can be shown at one time.",
			       MAX_REVS), MAX_REVS);
		اگر (!repo_dwim_ref(the_repository, *av, strlen(*av), &oid,
				   &ref, 0))
			die(_("no such ref %s"), *av);

		/* Has the base been specified? */
		اگر (reflog_base) {
			نویسه *ep;
			base = strtoul(reflog_base, &ep, 10);
			اگر (*ep) {
				/* Ah, that is a date spec... */
				timestamp_t at;
				at = approxidate(reflog_base);
				read_ref_at(get_main_ref_store(the_repository),
					    ref, flags, at, -1, &oid, پوچ,
					    پوچ, پوچ, &base);
			}
		}

		برای (i = 0; i < reflog; i++) {
			نویسه *logmsg = پوچ;
			نویسه *nth_desc;
			ثابت نویسه *پیام;
			نویسه *end;
			timestamp_t timestamp;
			صحیح tz;

			اگر (read_ref_at(get_main_ref_store(the_repository),
					ref, flags, 0, base + i, &oid, &logmsg,
					&timestamp, &tz, پوچ)) {
				آزاد(logmsg);
				reflog = i;
				بشکن;
			}

			end = strchr(logmsg, '\n');
			اگر (end)
				*end = '\0';

			پیام = (*logmsg == '\0') ? "(none)" : logmsg;
			reflog_پیام[i] = xstrfmt("(%s) %s",
						show_date(timestamp, tz,
							  DATE_MODE(RELATIVE)),
						پیام);
			آزاد(logmsg);

			nth_desc = xstrfmt("%s@{%d}", *av, base+i);
			append_ref(nth_desc, &oid, 1);
			آزاد(nth_desc);
		}
		آزاد(ref);
	}
	وگرنه {
		وقتی (0 < ac) {
			append_one_rev(*av);
			ac--; av++;
		}
		اگر (all_heads + all_remotes)
			snarf_refs(all_heads, all_remotes);
	}

	head = refs_resolve_refdup(get_main_ref_store(the_repository), "HEAD",
				   RESOLVE_REF_READING,
				   &head_oid, پوچ);

	اگر (with_current_branch && head) {
		صحیح has_head = 0;
		برای (i = 0; !has_head && i < ref_name_cnt; i++) {
			/* We are only interested in adding the branch
			 * HEAD points at.
			 */
			اگر (rev_is_head(head, ref_name[i]))
				has_head++;
		}
		اگر (!has_head) {
			ثابت نویسه *name = head;
			skip_prefix(name, "refs/heads/", &name);
			append_one_rev(name);
		}
	}

	اگر (!ref_name_cnt) {
		fprintf(stderr, "No revs to be shown.\n");
		ret = 0;
		برو out;
	}

	برای (رقم_rev = 0; ref_name[رقم_rev]; رقم_rev++) {
		ساختار object_id revkey;
		unsigned صحیح flag = 1u << (رقم_rev + REV_SHIFT);

		اگر (MAX_REVS <= رقم_rev)
			die(Q_("cannot handle more than %d rev.",
			       "cannot handle more than %d revs.",
			       MAX_REVS), MAX_REVS);
		اگر (repo_get_oid(the_repository, ref_name[رقم_rev], &revkey))
			die(_("'%s' is not a valid ref."), ref_name[رقم_rev]);
		commit = lookup_commit_reference(the_repository, &revkey);
		اگر (!commit)
			die(_("cannot find commit %s (%s)"),
			    ref_name[رقم_rev], oid_to_hex(&revkey));
		repo_parse_commit(the_repository, commit);
		mark_seen(commit, &seen);

		/* rev#0 uses bit REV_SHIFT, rev#1 uses bit REV_SHIFT+1,
		 * and so on.  REV_SHIFT bits from bit 0 are used for
		 * internal bookkeeping.
		 */
		commit->object.flags |= flag;
		اگر (commit->object.flags == flag)
			prio_queue_put(&queue, commit);
		rev[رقم_rev] = commit;
	}
	برای (i = 0; i < رقم_rev; i++)
		rev_mask[i] = rev[i]->object.flags;

	اگر (0 <= extra)
		join_revs(&queue, &seen, رقم_rev, extra);

	commit_list_sort_by_date(&seen);

	اگر (merge_base) {
		ret = show_merge_base(seen, رقم_rev);
		برو out;
	}

	اگر (independent) {
		ret = show_independent(rev, رقم_rev, rev_mask);
		برو out;
	}

	/* Show list; --more=-1 means list-only */
	اگر (1 < رقم_rev || extra < 0) {
		برای (i = 0; i < رقم_rev; i++) {
			صحیح j;
			صحیح is_head = rev_is_head(head, ref_name[i]) &&
				      oideq(&head_oid, &rev[i]->object.oid);
			اگر (extra < 0)
				printf("%c [%s] ",
				       is_head ? '*' : ' ', ref_name[i]);
			وگرنه {
				برای (j = 0; j < i; j++)
					putchar(' ');
				printf("%s%c%s [%s] ",
				       get_color_code(i),
				       is_head ? '*' : '!',
				       get_color_reset_code(), ref_name[i]);
			}

			اگر (!reflog) {
				/* header lines never need name */
				show_one_commit(rev[i], 1);
			}
			وگرنه
				puts(reflog_پیام[i]);

			اگر (is_head)
				head_at = i;
		}
		اگر (0 <= extra) {
			برای (i = 0; i < رقم_rev; i++)
				putchar('-');
			putchar('\n');
		}
	}
	اگر (extra < 0) {
		ret = 0;
		برو out;
	}

	/* Sort topologically */
	sort_in_topological_order(&seen, sort_order);

	/* Give names to commits */
	اگر (!sha1_name && !no_name)
		name_commits(seen, rev, ref_name, رقم_rev);

	all_mask = ((1u << (REV_SHIFT + رقم_rev)) - 1);
	all_revs = all_mask & ~((1u << REV_SHIFT) - 1);

	برای (ساختار commit_list *l = seen; l; l = l->next) {
		ساختار commit *commit = l->item;
		صحیح this_flag = commit->object.flags;
		صحیح is_merge_point = ((this_flag & all_revs) == all_revs);

		shown_merge_point |= is_merge_point;

		اگر (1 < رقم_rev) {
			صحیح is_merge = !!(commit->parents &&
					  commit->parents->next);
			اگر (topics &&
			    !is_merge_point &&
			    (this_flag & (1u << REV_SHIFT)))
				ادامه;
			اگر (!sparse && is_merge &&
			    omit_in_dense(commit, rev, رقم_rev))
				ادامه;
			برای (i = 0; i < رقم_rev; i++) {
				صحیح mark;
				اگر (!(this_flag & (1u << (i + REV_SHIFT))))
					mark = ' ';
				وگرنه اگر (is_merge)
					mark = '-';
				وگرنه اگر (i == head_at)
					mark = '*';
				وگرنه
					mark = '+';
				اگر (mark == ' ')
					putchar(mark);
				وگرنه
					printf("%s%c%s",
					       get_color_code(i),
					       mark, get_color_reset_code());
			}
			putchar(' ');
		}
		show_one_commit(commit, no_name);

		اگر (shown_merge_point && --extra < 0)
			بشکن;
	}

	ret = 0;

out:
	برای (size_t i = 0; i < ARRAY_SIZE(reflog_پیام); i++)
		آزاد(reflog_پیام[i]);
	commit_list_free(seen);
	clear_prio_queue(&queue);
	آزاد(args_copy);
	آزاد(head);
	بازگشت ret;
}
