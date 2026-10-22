#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "hash.h"
#ضمن "hex.h"
#ضمن "pretty.h"
#ضمن "refs.h"
#ضمن "color.h"
#ضمن "strvec.h"
#ضمن "object-name.h"
#ضمن "parse-options.h"

#ضمن "dir.h"
#ضمن "commit-slab.h"
#ضمن "date.h"
#ضمن "wildmatch.h"
#ضمن "prio-queue.h"

ساكن ثابت حرف*ثابت show_branch_usage[] = {
    N_("git show-branch [-a | --all] [-r | --remotes] [--topo-order | --date-order]\n"
       "                [--current] [--color[=<when>] | --no-color] [--sparse]\n"
       "                [--more=<n> | --list | --independent | --merge-base]\n"
       "                [--no-name | --sha1-name] [--topics]\n"
       "                [(<rev> | <glob>)...]"),
    N_("git show-branch (-g | --reflog)[=<n>[,<base>]] [--list] [<ref>]"),
    عدم
};

ساكن تعداد git_colorbool showbranch_use_color = GIT_COLOR_UNKNOWN;

ساكن بنية strvec default_args = STRVEC_INIT;

/*
 * TODO: convert this use of commit->object.flags to commit-slab
 * instead to store a pointer to ref name directly. Then use the same
 * UNINTERESTING definition from revision.h here.
 */
#define UNINTERESTING	01

#define REV_SHIFT	 2
#define MAX_REVS	(FLAG_BITS - REV_SHIFT) /* should not exceed bits_per_int - REV_SHIFT */

#define DEFAULT_REFLOG	4

ساكن ثابت حرف *get_color_code(صحيح idx)
{
	אם (want_color(showbranch_use_color))
		החזר column_colors_ansi[idx % column_colors_ansi_max];
	החזר "";
}

ساكن ثابت حرف *get_color_reset_code(فراغ)
{
	אם (want_color(showbranch_use_color))
		החזר GIT_COLOR_RESET;
	החזר "";
}

ساكن بنية commit *interesting(بنية prio_queue *queue)
{
	بنية commit *commit;
	prio_queue_for_each(queue, commit) {
		אם (!(commit->object.flags & UNINTERESTING))
			החזר commit;
	}
	החזר عدم;
}

بنية commit_name {
	ثابت حرف *head_name; /* which head's ancestor? */
	صحيح generation; /* how many parents away from head_name */
};

define_commit_slab(commit_name_slab, بنية commit_name *);
ساكن بنية commit_name_slab name_slab;

ساكن بنية commit_name *commit_to_name(بنية commit *commit)
{
	החזר *commit_name_slab_at(&name_slab, commit);
}


/* Name the commit as nth generation ancestor of head_name;
 * we count only the first-parent relationship for naming purposes.
 */
ساكن فراغ name_commit(بنية commit *commit, ثابت حرف *head_name, صحيح nth)
{
	بنية commit_name *name;

	name = *commit_name_slab_at(&name_slab, commit);
	אם (!name) {
		name = xmalloc(حجم(*name));
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
ساكن فراغ name_parent(بنية commit *commit, بنية commit *parent)
{
	بنية commit_name *commit_name = commit_to_name(commit);
	بنية commit_name *parent_name = commit_to_name(parent);
	אם (!commit_name)
		החזר;
	אם (!parent_name ||
	    commit_name->generation + 1 < parent_name->generation)
		name_commit(parent, commit_name->head_name,
			    commit_name->generation + 1);
}

ساكن صحيح name_first_parent_chain(بنية commit *c)
{
	صحيح i = 0;
	כלעוד (c) {
		بنية commit *p;
		אם (!commit_to_name(c))
			اكسر;
		אם (!c->parents)
			اكسر;
		p = c->parents->item;
		אם (!commit_to_name(p)) {
			name_parent(c, p);
			i++;
		}
		אחרת
			اكسر;
		c = p;
	}
	החזר i;
}

ساكن فراغ name_commits(بنية commit_list *list,
			 بنية commit **rev,
			 حرف **ref_name,
			 صحيح num_rev)
{
	بنية commit_list *cl;
	بنية commit *c;
	صحيح i;

	/* First give names to the given heads */
	עבור (cl = list; cl; cl = cl->next) {
		c = cl->item;
		אם (commit_to_name(c))
			استمر;
		עבור (i = 0; i < num_rev; i++) {
			אם (rev[i] == c) {
				name_commit(c, ref_name[i], 0);
				اكسر;
			}
		}
	}

	/* Then commits on the first parent ancestry chain */
	עשה {
		i = 0;
		עבור (cl = list; cl; cl = cl->next) {
			i += name_first_parent_chain(cl->item);
		}
	} כלעוד (i);

	/* Finally, any unnamed commits */
	עשה {
		i = 0;
		עבור (cl = list; cl; cl = cl->next) {
			بنية commit_list *parents;
			بنية commit_name *n;
			صحيح nth;
			c = cl->item;
			אם (!commit_to_name(c))
				استمر;
			n = commit_to_name(c);
			parents = c->parents;
			nth = 0;
			כלעוד (parents) {
				بنية commit *p = parents->item;
				بنية strbuf newname = STRBUF_INIT;
				parents = parents->next;
				nth++;
				אם (commit_to_name(p))
					استمر;
				بدل (n->generation) {
				حالة 0:
					strbuf_addstr(&newname, n->head_name);
					اكسر;
				حالة 1:
					strbuf_addf(&newname, "%s^", n->head_name);
					اكسر;
				افتراضي:
					strbuf_addf(&newname, "%s~%d",
						    n->head_name, n->generation);
					اكسر;
				}
				אם (nth == 1)
					strbuf_addch(&newname, '^');
				אחרת
					strbuf_addf(&newname, "^%d", nth);
				name_commit(p, strbuf_detach(&newname, عدم), 0);
				i++;
				name_first_parent_chain(p);
			}
		}
	} כלעוד (i);
}

ساكن صحيح mark_seen(بنية commit *commit, بنية commit_list **seen_p)
{
	אם (!commit->object.flags) {
		commit_list_insert(commit, seen_p);
		החזר 1;
	}
	החזר 0;
}

ساكن فراغ join_revs(بنية prio_queue *queue,
		      بنية commit_list **seen_p,
		      صحيح num_rev, صحيح extra)
{
	صحيح all_mask = ((1u << (REV_SHIFT + num_rev)) - 1);
	صحيح all_revs = all_mask & ~((1u << REV_SHIFT) - 1);
	بنية commit *commit;

	כלעוד ((commit = prio_queue_peek(queue))) {
		بنية commit_list *parents;
		صحيح still_interesting = !!interesting(queue);
		صحيح flags = commit->object.flags & all_mask;

		אם (!still_interesting && extra <= 0)
			اكسر;

		prio_queue_get(queue);

		mark_seen(commit, seen_p);
		אם ((flags & all_revs) == all_revs)
			flags |= UNINTERESTING;
		parents = commit->parents;

		כלעוד (parents) {
			بنية commit *p = parents->item;
			صحيح this_flag = p->object.flags;
			parents = parents->next;
			אם ((this_flag & flags) == flags)
				استمر;
			repo_parse_commit(the_repository, p);
			אם (mark_seen(p, seen_p) && !still_interesting)
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
	עבור (;;) {
		صحيح changed = 0;
		بنية commit_list *s;
		עבור (s = *seen_p; s; s = s->next) {
			بنية commit *c = s->item;
			بنية commit_list *parents;

			אם (((c->object.flags & all_revs) != all_revs) &&
			    !(c->object.flags & UNINTERESTING))
				استمر;

			/* The current commit is either a merge base or
			 * already uninteresting one.  Mark its parents
			 * as uninteresting commits _only_ if they are
			 * already parsed.  No reason to find new ones
			 * here.
			 */
			parents = c->parents;
			כלעוד (parents) {
				بنية commit *p = parents->item;
				parents = parents->next;
				אם (!(p->object.flags & UNINTERESTING)) {
					p->object.flags |= UNINTERESTING;
					changed = 1;
				}
			}
		}
		אם (!changed)
			اكسر;
	}
}

ساكن فراغ show_one_commit(بنية commit *commit, صحيح no_name)
{
	بنية strbuf pretty = STRBUF_INIT;
	ثابت حرف *pretty_str = "(unavailable)";
	بنية commit_name *name = commit_to_name(commit);

	אם (commit->object.parsed) {
		pp_commit_easy(CMIT_FMT_ONELINE, commit, &pretty);
		pretty_str = pretty.buf;
	}
	skip_prefix(pretty_str, "[PATCH] ", &pretty_str);

	אם (!no_name) {
		אם (name && name->head_name) {
			printf("[%s", name->head_name);
			אם (name->generation) {
				אם (name->generation == 1)
					printf("^");
				אחרת
					printf("~%d", name->generation);
			}
			printf("] ");
		}
		אחרת
			printf("[%s] ",
			       repo_find_unique_abbrev(the_repository, &commit->object.oid,
						       DEFAULT_ABBREV));
	}
	puts(pretty_str);
	strbuf_release(&pretty);
}

ساكن حرف *ref_name[MAX_REVS + 1];
ساكن صحيح ref_name_cnt;

ساكن ثابت حرف *find_digit_prefix(ثابت حرف *s, صحيح *v)
{
	ثابت حرف *p;
	صحيح ver;
	حرف ch;

	עבור (p = s, ver = 0;
	     '0' <= (ch = *p) && ch <= '9';
	     p++)
		ver = ver * 10 + ch - '0';
	*v = ver;
	החזר p;
}


ساكن صحيح version_cmp(ثابت حرف *a, ثابت حرف *b)
{
	כלעוד (1) {
		صحيح va, vb;

		a = find_digit_prefix(a, &va);
		b = find_digit_prefix(b, &vb);
		אם (va != vb)
			החזר va - vb;

		כלעוד (1) {
			صحيح ca = *a;
			صحيح cb = *b;
			אם ('0' <= ca && ca <= '9')
				ca = 0;
			אם ('0' <= cb && cb <= '9')
				cb = 0;
			אם (ca != cb)
				החזר ca - cb;
			אם (!ca)
				اكسر;
			a++;
			b++;
		}
		אם (!*a && !*b)
			החזר 0;
	}
}

ساكن صحيح compare_ref_name(ثابت فراغ *a_, ثابت فراغ *b_)
{
	ثابت حرف * ثابت*a = a_, * ثابت*b = b_;
	החזר version_cmp(*a, *b);
}

ساكن فراغ sort_ref_range(صحيح bottom, صحيح top)
{
	QSORT(ref_name + bottom, top - bottom, compare_ref_name);
}

ساكن صحيح append_ref(ثابت حرف *refname, ثابت بنية object_id *oid,
		      صحيح allow_dups)
{
	بنية commit *commit = lookup_commit_reference_gently(the_repository,
							       oid, 1);
	صحيح i;

	אם (!commit)
		החזר 0;

	אם (!allow_dups) {
		/* Avoid adding the same thing twice */
		עבור (i = 0; i < ref_name_cnt; i++)
			אם (!strcmp(refname, ref_name[i]))
				החזר 0;
	}
	אם (MAX_REVS <= ref_name_cnt) {
		warning(Q_("ignoring %s; cannot handle more than %d ref",
			   "ignoring %s; cannot handle more than %d refs",
			   MAX_REVS), refname, MAX_REVS);
		החזר 0;
	}
	ref_name[ref_name_cnt++] = xstrdup(refname);
	ref_name[ref_name_cnt] = عدم;
	החזר 0;
}

ساكن صحيح append_head_ref(ثابت بنية reference *ref, فراغ *cb_data UNUSED)
{
	بنية object_id tmp;
	صحيح ofs = 11;
	אם (!starts_with(ref->name, "refs/heads/"))
		החזר 0;
	/* If both heads/foo and tags/foo exists, get_sha1 would
	 * get confused.
	 */
	אם (repo_get_oid(the_repository, ref->name + ofs, &tmp) || !oideq(&tmp, ref->oid))
		ofs = 5;
	החזר append_ref(ref->name + ofs, ref->oid, 0);
}

ساكن صحيح append_remote_ref(ثابت بنية reference *ref, فراغ *cb_data UNUSED)
{
	بنية object_id tmp;
	صحيح ofs = 13;
	אם (!starts_with(ref->name, "refs/remotes/"))
		החזר 0;
	/* If both heads/foo and tags/foo exists, get_sha1 would
	 * get confused.
	 */
	אם (repo_get_oid(the_repository, ref->name + ofs, &tmp) || !oideq(&tmp, ref->oid))
		ofs = 5;
	החזר append_ref(ref->name + ofs, ref->oid, 0);
}

ساكن صحيح append_tag_ref(ثابت حرف *refname, ثابت بنية object_id *oid,
			  صحيح flag UNUSED, فراغ *cb_data UNUSED)
{
	אם (!starts_with(refname, "refs/tags/"))
		החזר 0;
	החזר append_ref(refname + 5, oid, 0);
}

ساكن ثابت حرف *match_ref_pattern = عدم;
ساكن صحيح match_ref_slash = 0;

ساكن صحيح append_matching_ref(ثابت بنية reference *ref, فراغ *cb_data)
{
	/* we want to allow pattern hold/<asterisk> to show all
	 * branches under refs/heads/hold/, and v0.99.9? to show
	 * refs/tags/v0.99.9a and friends.
	 */
	ثابت حرف *tail;
	صحيح slash = count_slashes(ref->name);
	עבור (tail = ref->name; *tail && match_ref_slash < slash; )
		אם (*tail++ == '/')
			slash--;
	אם (!*tail)
		החזר 0;
	אם (wildmatch(match_ref_pattern, tail, 0))
		החזר 0;
	אם (starts_with(ref->name, "refs/heads/"))
		החזר append_head_ref(ref, cb_data);
	אם (starts_with(ref->name, "refs/tags/"))
		החזר append_tag_ref(ref->name, ref->oid, ref->flags, cb_data);
	החזר append_ref(ref->name, ref->oid, 0);
}

ساكن فراغ snarf_refs(صحيح head, صحيح remotes)
{
	אם (head) {
		صحيح orig_cnt = ref_name_cnt;

		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_head_ref, عدم);
		sort_ref_range(orig_cnt, ref_name_cnt);
	}
	אם (remotes) {
		صحيح orig_cnt = ref_name_cnt;

		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_remote_ref, عدم);
		sort_ref_range(orig_cnt, ref_name_cnt);
	}
}

ساكن صحيح rev_is_head(ثابت حرف *head, ثابت حرف *name)
{
	אם (!head)
		החזר 0;
	skip_prefix(head, "refs/heads/", &head);
	אם (!skip_prefix(name, "refs/heads/", &name))
		skip_prefix(name, "heads/", &name);
	החזר !strcmp(head, name);
}

ساكن صحيح show_merge_base(ثابت بنية commit_list *seen, صحيح num_rev)
{
	صحيح all_mask = ((1u << (REV_SHIFT + num_rev)) - 1);
	صحيح all_revs = all_mask & ~((1u << REV_SHIFT) - 1);
	صحيح exit_status = 1;

	עבור (ثابت بنية commit_list *s = seen; s; s = s->next) {
		بنية commit *commit = s->item;
		صحيح flags = commit->object.flags & all_mask;
		אם (!(flags & UNINTERESTING) &&
		    ((flags & all_revs) == all_revs)) {
			puts(oid_to_hex(&commit->object.oid));
			exit_status = 0;
			commit->object.flags |= UNINTERESTING;
		}
	}
	החזר exit_status;
}

ساكن صحيح show_independent(بنية commit **rev,
			    صحيح num_rev,
			    unsigned صحيح *rev_mask)
{
	صحيح i;

	עבור (i = 0; i < num_rev; i++) {
		بنية commit *commit = rev[i];
		unsigned صحيح flag = rev_mask[i];

		אם (commit->object.flags == flag)
			puts(oid_to_hex(&commit->object.oid));
		commit->object.flags |= UNINTERESTING;
	}
	החזר 0;
}

ساكن فراغ append_one_rev(ثابت حرف *av)
{
	بنية object_id revkey;
	אם (!repo_get_oid(the_repository, av, &revkey)) {
		append_ref(av, &revkey, 0);
		החזר;
	}
	אם (strpbrk(av, "*?[")) {
		/* glob style match */
		صحيح saved_matches = ref_name_cnt;

		match_ref_pattern = av;
		match_ref_slash = count_slashes(av);
		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_matching_ref, عدم);
		אם (saved_matches == ref_name_cnt &&
		    ref_name_cnt < MAX_REVS)
			error(_("no matching refs with %s"), av);
		sort_ref_range(saved_matches, ref_name_cnt);
		החזר;
	}
	die("bad sha1 reference %s", av);
}

ساكن صحيح git_show_branch_config(ثابت حرف *var, ثابت حرف *value,
				  ثابت بنية config_context *ctx, فراغ *cb)
{
	אם (!strcmp(var, "showbranch.default")) {
		אם (!value)
			החזר config_error_nonbool(var);
		/*
		 * default_arg is now passed to parse_options(), so we need to
		 * mimic the real argv a bit better.
		 */
		אם (!default_args.nr)
			strvec_push(&default_args, "show-branch");
		strvec_push(&default_args, value);
		החזר 0;
	}

	אם (!strcmp(var, "color.showbranch")) {
		showbranch_use_color = git_config_colorbool(var, value);
		החזר 0;
	}

	אם (git_color_config(var, value, cb) < 0)
		החזר -1;

	החזר git_default_config(var, value, ctx, cb);
}

ساكن صحيح omit_in_dense(بنية commit *commit, بنية commit **rev, صحيح n)
{
	/* If the commit is tip of the named branches, do not
	 * omit it.
	 * Otherwise, if it is a merge that is reachable from only one
	 * tip, it is not that interesting.
	 */
	صحيح i, flag, count;
	עבור (i = 0; i < n; i++)
		אם (rev[i] == commit)
			החזר 0;
	flag = commit->object.flags;
	עבור (i = count = 0; i < n; i++) {
		אם (flag & (1u << (i + REV_SHIFT)))
			count++;
	}
	אם (count == 1)
		החזר 1;
	החזר 0;
}

ساكن صحيح reflog = 0;

ساكن صحيح parse_reflog_param(ثابت بنية option *opt, ثابت حرف *arg,
			      صحيح unset)
{
	حرف *ep;
	ثابت حرف **base = (ثابت حرف **)opt->value;
	BUG_ON_OPT_NEG(unset);
	אם (!arg)
		arg = "";
	reflog = strtoul(arg, &ep, 10);
	אם (*ep == ',')
		*base = ep + 1;
	אחרת אם (*ep)
		החזר error("unrecognized reflog param '%s'", arg);
	אחרת
		*base = عدم;
	אם (reflog <= 0)
		reflog = DEFAULT_REFLOG;
	החזר 0;
}

صحيح cmd_show_branch(صحيح ac,
		ثابت حرف **av,
		ثابت حرف *prefix,
		بنية repository *repo UNUSED)
{
	بنية commit *rev[MAX_REVS], *commit;
	حرف *reflog_msg[MAX_REVS] = {0};
	بنية commit_list *seen = عدم;
	بنية prio_queue queue = { compare_commits_by_commit_date };
	unsigned صحيح rev_mask[MAX_REVS];
	صحيح num_rev, i, extra = 0;
	صحيح all_heads = 0, all_remotes = 0;
	صحيح all_mask, all_revs;
	تعداد rev_sort_order sort_order = REV_SORT_IN_GRAPH_ORDER;
	حرف *head;
	بنية object_id head_oid;
	صحيح merge_base = 0;
	صحيح independent = 0;
	صحيح no_name = 0;
	صحيح sha1_name = 0;
	صحيح shown_merge_point = 0;
	صحيح with_current_branch = 0;
	صحيح head_at = -1;
	صحيح topics = 0;
	صحيح sparse = 0;
	ثابت حرف *reflog_base = عدم;
	بنية option builtin_show_branch_options[] = {
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
			.precision = حجم(extra),
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
	ثابت حرف **args_copy = عدم;
	صحيح ret;

	init_commit_name_slab(&name_slab);

	repo_config(the_repository, git_show_branch_config, عدم);

	/* If nothing is specified, try the default first */
	אם (ac == 1 && default_args.nr) {
		DUP_ARRAY(args_copy, default_args.v, default_args.nr);
		ac = default_args.nr;
		av = args_copy;
	}

	ac = parse_options(ac, av, prefix, builtin_show_branch_options,
			   show_branch_usage, PARSE_OPT_STOP_AT_NON_OPTION);
	אם (all_heads)
		all_remotes = 1;

	אם (extra || reflog) {
		/* "listing" mode is incompatible with
		 * independent nor merge-base modes.
		 */
		אם (independent || merge_base)
			usage_with_options(show_branch_usage,
					   builtin_show_branch_options);
		אם (reflog && ((0 < extra) || all_heads || all_remotes))
			/*
			 * Asking for --more in reflog mode does not
			 * make sense.  --list is Ok.
			 *
			 * Also --all and --remotes do not make sense either.
			 */
			die(_("options '%s' and '%s' cannot be used together"), "--reflog",
				"--all/--remotes/--independent/--merge-base");
	}

	אם (with_current_branch && reflog)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--reflog", "--current");

	/* If nothing is specified, show all branches by default */
	אם (ac <= topics && all_heads + all_remotes == 0)
		all_heads = 1;

	אם (reflog) {
		بنية object_id oid;
		حرف *ref;
		صحيح base = 0;
		unsigned صحيح flags = 0;

		אם (ac == 0) {
			ساكن ثابت حرف *fake_av[2];

			fake_av[0] = refs_resolve_refdup(get_main_ref_store(the_repository),
							 "HEAD",
							 RESOLVE_REF_READING,
							 &oid,
							 عدم);
			fake_av[1] = عدم;
			av = fake_av;
			ac = 1;
			אם (!*av)
				die(_("no branches given, and HEAD is not valid"));
		}
		אם (ac != 1)
			die(_("--reflog option needs one branch name"));

		אם (MAX_REVS < reflog)
			die(Q_("only %d entry can be shown at one time.",
			       "only %d entries can be shown at one time.",
			       MAX_REVS), MAX_REVS);
		אם (!repo_dwim_ref(the_repository, *av, strlen(*av), &oid,
				   &ref, 0))
			die(_("no such ref %s"), *av);

		/* Has the base been specified? */
		אם (reflog_base) {
			حرف *ep;
			base = strtoul(reflog_base, &ep, 10);
			אם (*ep) {
				/* Ah, that is a date spec... */
				timestamp_t at;
				at = approxidate(reflog_base);
				read_ref_at(get_main_ref_store(the_repository),
					    ref, flags, at, -1, &oid, عدم,
					    عدم, عدم, &base);
			}
		}

		עבור (i = 0; i < reflog; i++) {
			حرف *logmsg = عدم;
			حرف *nth_desc;
			ثابت حرف *msg;
			حرف *end;
			timestamp_t timestamp;
			صحيح tz;

			אם (read_ref_at(get_main_ref_store(the_repository),
					ref, flags, 0, base + i, &oid, &logmsg,
					&timestamp, &tz, عدم)) {
				حرر(logmsg);
				reflog = i;
				اكسر;
			}

			end = strchr(logmsg, '\n');
			אם (end)
				*end = '\0';

			msg = (*logmsg == '\0') ? "(none)" : logmsg;
			reflog_msg[i] = xstrfmt("(%s) %s",
						show_date(timestamp, tz,
							  DATE_MODE(RELATIVE)),
						msg);
			حرر(logmsg);

			nth_desc = xstrfmt("%s@{%d}", *av, base+i);
			append_ref(nth_desc, &oid, 1);
			حرر(nth_desc);
		}
		حرر(ref);
	}
	אחרת {
		כלעוד (0 < ac) {
			append_one_rev(*av);
			ac--; av++;
		}
		אם (all_heads + all_remotes)
			snarf_refs(all_heads, all_remotes);
	}

	head = refs_resolve_refdup(get_main_ref_store(the_repository), "HEAD",
				   RESOLVE_REF_READING,
				   &head_oid, عدم);

	אם (with_current_branch && head) {
		صحيح has_head = 0;
		עבור (i = 0; !has_head && i < ref_name_cnt; i++) {
			/* We are only interested in adding the branch
			 * HEAD points at.
			 */
			אם (rev_is_head(head, ref_name[i]))
				has_head++;
		}
		אם (!has_head) {
			ثابت حرف *name = head;
			skip_prefix(name, "refs/heads/", &name);
			append_one_rev(name);
		}
	}

	אם (!ref_name_cnt) {
		fprintf(stderr, "No revs to be shown.\n");
		ret = 0;
		اذهب out;
	}

	עבור (num_rev = 0; ref_name[num_rev]; num_rev++) {
		بنية object_id revkey;
		unsigned صحيح flag = 1u << (num_rev + REV_SHIFT);

		אם (MAX_REVS <= num_rev)
			die(Q_("cannot handle more than %d rev.",
			       "cannot handle more than %d revs.",
			       MAX_REVS), MAX_REVS);
		אם (repo_get_oid(the_repository, ref_name[num_rev], &revkey))
			die(_("'%s' is not a valid ref."), ref_name[num_rev]);
		commit = lookup_commit_reference(the_repository, &revkey);
		אם (!commit)
			die(_("cannot find commit %s (%s)"),
			    ref_name[num_rev], oid_to_hex(&revkey));
		repo_parse_commit(the_repository, commit);
		mark_seen(commit, &seen);

		/* rev#0 uses bit REV_SHIFT, rev#1 uses bit REV_SHIFT+1,
		 * and so on.  REV_SHIFT bits from bit 0 are used for
		 * internal bookkeeping.
		 */
		commit->object.flags |= flag;
		אם (commit->object.flags == flag)
			prio_queue_put(&queue, commit);
		rev[num_rev] = commit;
	}
	עבור (i = 0; i < num_rev; i++)
		rev_mask[i] = rev[i]->object.flags;

	אם (0 <= extra)
		join_revs(&queue, &seen, num_rev, extra);

	commit_list_sort_by_date(&seen);

	אם (merge_base) {
		ret = show_merge_base(seen, num_rev);
		اذهب out;
	}

	אם (independent) {
		ret = show_independent(rev, num_rev, rev_mask);
		اذهب out;
	}

	/* Show list; --more=-1 means list-only */
	אם (1 < num_rev || extra < 0) {
		עבור (i = 0; i < num_rev; i++) {
			صحيح j;
			صحيح is_head = rev_is_head(head, ref_name[i]) &&
				      oideq(&head_oid, &rev[i]->object.oid);
			אם (extra < 0)
				printf("%c [%s] ",
				       is_head ? '*' : ' ', ref_name[i]);
			אחרת {
				עבור (j = 0; j < i; j++)
					putchar(' ');
				printf("%s%c%s [%s] ",
				       get_color_code(i),
				       is_head ? '*' : '!',
				       get_color_reset_code(), ref_name[i]);
			}

			אם (!reflog) {
				/* header lines never need name */
				show_one_commit(rev[i], 1);
			}
			אחרת
				puts(reflog_msg[i]);

			אם (is_head)
				head_at = i;
		}
		אם (0 <= extra) {
			עבור (i = 0; i < num_rev; i++)
				putchar('-');
			putchar('\n');
		}
	}
	אם (extra < 0) {
		ret = 0;
		اذهب out;
	}

	/* Sort topologically */
	sort_in_topological_order(&seen, sort_order);

	/* Give names to commits */
	אם (!sha1_name && !no_name)
		name_commits(seen, rev, ref_name, num_rev);

	all_mask = ((1u << (REV_SHIFT + num_rev)) - 1);
	all_revs = all_mask & ~((1u << REV_SHIFT) - 1);

	עבור (بنية commit_list *l = seen; l; l = l->next) {
		بنية commit *commit = l->item;
		صحيح this_flag = commit->object.flags;
		صحيح is_merge_point = ((this_flag & all_revs) == all_revs);

		shown_merge_point |= is_merge_point;

		אם (1 < num_rev) {
			صحيح is_merge = !!(commit->parents &&
					  commit->parents->next);
			אם (topics &&
			    !is_merge_point &&
			    (this_flag & (1u << REV_SHIFT)))
				استمر;
			אם (!sparse && is_merge &&
			    omit_in_dense(commit, rev, num_rev))
				استمر;
			עבור (i = 0; i < num_rev; i++) {
				صحيح mark;
				אם (!(this_flag & (1u << (i + REV_SHIFT))))
					mark = ' ';
				אחרת אם (is_merge)
					mark = '-';
				אחרת אם (i == head_at)
					mark = '*';
				אחרת
					mark = '+';
				אם (mark == ' ')
					putchar(mark);
				אחרת
					printf("%s%c%s",
					       get_color_code(i),
					       mark, get_color_reset_code());
			}
			putchar(' ');
		}
		show_one_commit(commit, no_name);

		אם (shown_merge_point && --extra < 0)
			اكسر;
	}

	ret = 0;

out:
	עבור (size_t i = 0; i < ARRAY_SIZE(reflog_msg); i++)
		حرر(reflog_msg[i]);
	commit_list_free(seen);
	clear_prio_queue(&queue);
	حرر(args_copy);
	حرر(head);
	החזר ret;
}
