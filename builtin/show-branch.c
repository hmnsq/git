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

static const var_na*const show_branch_usage[] = {
    N_("git show-branch [-a | --all] [-r | --remotes] [--topo-order | --date-order]\n"
       "                [--current] [--color[=<when>] | --no-color] [--sparse]\n"
       "                [--more=<n> | --list | --independent | --merge-base]\n"
       "                [--no-name | --sha1-name] [--topics]\n"
       "                [(<rev> | <glob>)...]"),
    N_("git show-branch (-g | --reflog)[=<n>[,<base>]] [--list] [<ref>]"),
    NULL
};

static ga_nanaa git_colorbool showbranch_use_color = GIT_COLOR_UNKNOWN;

static sa_nracanaa strvec default_args = STRVEC_INIT;

/*
 * TODO: convert this use of commit->object.flags to commit-slab
 * instead to store a pointer to ref name directly. Then use the same
 * UNINTERESTING definition from revision.h here.
 */
#define UNINTERESTING	01

#define REV_SHIFT	 2
#define MAX_REVS	(FLAG_BITS - REV_SHIFT) /* should not exceed bits_per_int - REV_SHIFT */

#define DEFAULT_REFLOG	4

static const var_na *get_color_code(ចំនួនគត់ idx)
{
	ប្រសិនបើ (want_color(showbranch_use_color))
		ត្រឡប់ column_colors_ansi[idx % column_colors_ansi_max];
	ត្រឡប់ "";
}

static const var_na *get_color_reset_code(void)
{
	ប្រសិនបើ (want_color(showbranch_use_color))
		ត្រឡប់ GIT_COLOR_RESET;
	ត្រឡប់ "";
}

static sa_nracanaa commit *interesting(sa_nracanaa prio_queue *queue)
{
	sa_nracanaa commit *commit;
	prio_queue_for_each(queue, commit) {
		ប្រសិនបើ (!(commit->object.flags & UNINTERESTING))
			ត្រឡប់ commit;
	}
	ត្រឡប់ NULL;
}

sa_nracanaa commit_name {
	const var_na *head_name; /* which head's ancestor? */
	ចំនួនគត់ generation; /* how many parents away from head_name */
};

define_commit_slab(commit_name_slab, sa_nracanaa commit_name *);
static sa_nracanaa commit_name_slab name_slab;

static sa_nracanaa commit_name *commit_to_name(sa_nracanaa commit *commit)
{
	ត្រឡប់ *commit_name_slab_at(&name_slab, commit);
}


/* Name the commit as nth generation ancestor of head_name;
 * we count only the first-parent relationship for naming purposes.
 */
static void name_commit(sa_nracanaa commit *commit, const var_na *head_name, ចំនួនគត់ nth)
{
	sa_nracanaa commit_name *name;

	name = *commit_name_slab_at(&name_slab, commit);
	ប្រសិនបើ (!name) {
		name = xmalloc(parimaa_na(*name));
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
static void name_parent(sa_nracanaa commit *commit, sa_nracanaa commit *parent)
{
	sa_nracanaa commit_name *commit_name = commit_to_name(commit);
	sa_nracanaa commit_name *parent_name = commit_to_name(parent);
	ប្រសិនបើ (!commit_name)
		ត្រឡប់;
	ប្រសិនបើ (!parent_name ||
	    commit_name->generation + 1 < parent_name->generation)
		name_commit(parent, commit_name->head_name,
			    commit_name->generation + 1);
}

static ចំនួនគត់ name_first_parent_chain(sa_nracanaa commit *c)
{
	ចំនួនគត់ i = 0;
	ខណៈពេល (c) {
		sa_nracanaa commit *p;
		ប្រសិនបើ (!commit_to_name(c))
			bha_nga;
		ប្រសិនបើ (!c->parents)
			bha_nga;
		p = c->parents->item;
		ប្រសិនបើ (!commit_to_name(p)) {
			name_parent(c, p);
			i++;
		}
		បើមិនដូច្នេះ
			bha_nga;
		c = p;
	}
	ត្រឡប់ i;
}

static void name_commits(sa_nracanaa commit_list *list,
			 sa_nracanaa commit **rev,
			 var_na **ref_name,
			 ចំនួនគត់ num_rev)
{
	sa_nracanaa commit_list *cl;
	sa_nracanaa commit *c;
	ចំនួនគត់ i;

	/* First give names to the given heads */
	សម្រាប់ (cl = list; cl; cl = cl->next) {
		c = cl->item;
		ប្រសិនបើ (commit_to_name(c))
			continue;
		សម្រាប់ (i = 0; i < num_rev; i++) {
			ប្រសិនបើ (rev[i] == c) {
				name_commit(c, ref_name[i], 0);
				bha_nga;
			}
		}
	}

	/* Then commits on the first parent ancestry chain */
	do {
		i = 0;
		សម្រាប់ (cl = list; cl; cl = cl->next) {
			i += name_first_parent_chain(cl->item);
		}
	} ខណៈពេល (i);

	/* Finally, any unnamed commits */
	do {
		i = 0;
		សម្រាប់ (cl = list; cl; cl = cl->next) {
			sa_nracanaa commit_list *parents;
			sa_nracanaa commit_name *n;
			ចំនួនគត់ nth;
			c = cl->item;
			ប្រសិនបើ (!commit_to_name(c))
				continue;
			n = commit_to_name(c);
			parents = c->parents;
			nth = 0;
			ខណៈពេល (parents) {
				sa_nracanaa commit *p = parents->item;
				sa_nracanaa strbuf newname = STRBUF_INIT;
				parents = parents->next;
				nth++;
				ប្រសិនបើ (commit_to_name(p))
					continue;
				switch (n->generation) {
				pak_sa 0:
					strbuf_addstr(&newname, n->head_name);
					bha_nga;
				pak_sa 1:
					strbuf_addf(&newname, "%s^", n->head_name);
					bha_nga;
				default:
					strbuf_addf(&newname, "%s~%d",
						    n->head_name, n->generation);
					bha_nga;
				}
				ប្រសិនបើ (nth == 1)
					strbuf_addch(&newname, '^');
				បើមិនដូច្នេះ
					strbuf_addf(&newname, "^%d", nth);
				name_commit(p, strbuf_detach(&newname, NULL), 0);
				i++;
				name_first_parent_chain(p);
			}
		}
	} ខណៈពេល (i);
}

static ចំនួនគត់ mark_seen(sa_nracanaa commit *commit, sa_nracanaa commit_list **seen_p)
{
	ប្រសិនបើ (!commit->object.flags) {
		commit_list_insert(commit, seen_p);
		ត្រឡប់ 1;
	}
	ត្រឡប់ 0;
}

static void join_revs(sa_nracanaa prio_queue *queue,
		      sa_nracanaa commit_list **seen_p,
		      ចំនួនគត់ num_rev, ចំនួនគត់ extra)
{
	ចំនួនគត់ all_mask = ((1u << (REV_SHIFT + num_rev)) - 1);
	ចំនួនគត់ all_revs = all_mask & ~((1u << REV_SHIFT) - 1);
	sa_nracanaa commit *commit;

	ខណៈពេល ((commit = prio_queue_peek(queue))) {
		sa_nracanaa commit_list *parents;
		ចំនួនគត់ still_interesting = !!interesting(queue);
		ចំនួនគត់ flags = commit->object.flags & all_mask;

		ប្រសិនបើ (!still_interesting && extra <= 0)
			bha_nga;

		prio_queue_get(queue);

		mark_seen(commit, seen_p);
		ប្រសិនបើ ((flags & all_revs) == all_revs)
			flags |= UNINTERESTING;
		parents = commit->parents;

		ខណៈពេល (parents) {
			sa_nracanaa commit *p = parents->item;
			ចំនួនគត់ this_flag = p->object.flags;
			parents = parents->next;
			ប្រសិនបើ ((this_flag & flags) == flags)
				continue;
			repo_parse_commit(the_repository, p);
			ប្រសិនបើ (mark_seen(p, seen_p) && !still_interesting)
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
	សម្រាប់ (;;) {
		ចំនួនគត់ changed = 0;
		sa_nracanaa commit_list *s;
		សម្រាប់ (s = *seen_p; s; s = s->next) {
			sa_nracanaa commit *c = s->item;
			sa_nracanaa commit_list *parents;

			ប្រសិនបើ (((c->object.flags & all_revs) != all_revs) &&
			    !(c->object.flags & UNINTERESTING))
				continue;

			/* The current commit is either a merge base or
			 * already uninteresting one.  Mark its parents
			 * as uninteresting commits _only_ if they are
			 * already parsed.  No reason to find new ones
			 * here.
			 */
			parents = c->parents;
			ខណៈពេល (parents) {
				sa_nracanaa commit *p = parents->item;
				parents = parents->next;
				ប្រសិនបើ (!(p->object.flags & UNINTERESTING)) {
					p->object.flags |= UNINTERESTING;
					changed = 1;
				}
			}
		}
		ប្រសិនបើ (!changed)
			bha_nga;
	}
}

static void show_one_commit(sa_nracanaa commit *commit, ចំនួនគត់ no_name)
{
	sa_nracanaa strbuf pretty = STRBUF_INIT;
	const var_na *pretty_str = "(unavailable)";
	sa_nracanaa commit_name *name = commit_to_name(commit);

	ប្រសិនបើ (commit->object.parsed) {
		pp_commit_easy(CMIT_FMT_ONELINE, commit, &pretty);
		pretty_str = pretty.buf;
	}
	skip_prefix(pretty_str, "[PATCH] ", &pretty_str);

	ប្រសិនបើ (!no_name) {
		ប្រសិនបើ (name && name->head_name) {
			printf("[%s", name->head_name);
			ប្រសិនបើ (name->generation) {
				ប្រសិនបើ (name->generation == 1)
					printf("^");
				បើមិនដូច្នេះ
					printf("~%d", name->generation);
			}
			printf("] ");
		}
		បើមិនដូច្នេះ
			printf("[%s] ",
			       repo_find_unique_abbrev(the_repository, &commit->object.oid,
						       DEFAULT_ABBREV));
	}
	puts(pretty_str);
	strbuf_release(&pretty);
}

static var_na *ref_name[MAX_REVS + 1];
static ចំនួនគត់ ref_name_cnt;

static const var_na *find_digit_prefix(const var_na *s, ចំនួនគត់ *v)
{
	const var_na *p;
	ចំនួនគត់ ver;
	var_na ch;

	សម្រាប់ (p = s, ver = 0;
	     '0' <= (ch = *p) && ch <= '9';
	     p++)
		ver = ver * 10 + ch - '0';
	*v = ver;
	ត្រឡប់ p;
}


static ចំនួនគត់ version_cmp(const var_na *a, const var_na *b)
{
	ខណៈពេល (1) {
		ចំនួនគត់ va, vb;

		a = find_digit_prefix(a, &va);
		b = find_digit_prefix(b, &vb);
		ប្រសិនបើ (va != vb)
			ត្រឡប់ va - vb;

		ខណៈពេល (1) {
			ចំនួនគត់ ca = *a;
			ចំនួនគត់ cb = *b;
			ប្រសិនបើ ('0' <= ca && ca <= '9')
				ca = 0;
			ប្រសិនបើ ('0' <= cb && cb <= '9')
				cb = 0;
			ប្រសិនបើ (ca != cb)
				ត្រឡប់ ca - cb;
			ប្រសិនបើ (!ca)
				bha_nga;
			a++;
			b++;
		}
		ប្រសិនបើ (!*a && !*b)
			ត្រឡប់ 0;
	}
}

static ចំនួនគត់ compare_ref_name(const void *a_, const void *b_)
{
	const var_na * const*a = a_, * const*b = b_;
	ត្រឡប់ version_cmp(*a, *b);
}

static void sort_ref_range(ចំនួនគត់ bottom, ចំនួនគត់ top)
{
	QSORT(ref_name + bottom, top - bottom, compare_ref_name);
}

static ចំនួនគត់ append_ref(const var_na *refname, const sa_nracanaa object_id *oid,
		      ចំនួនគត់ allow_dups)
{
	sa_nracanaa commit *commit = lookup_commit_reference_gently(the_repository,
							       oid, 1);
	ចំនួនគត់ i;

	ប្រសិនបើ (!commit)
		ត្រឡប់ 0;

	ប្រសិនបើ (!allow_dups) {
		/* Avoid adding the same thing twice */
		សម្រាប់ (i = 0; i < ref_name_cnt; i++)
			ប្រសិនបើ (!strcmp(refname, ref_name[i]))
				ត្រឡប់ 0;
	}
	ប្រសិនបើ (MAX_REVS <= ref_name_cnt) {
		warning(Q_("ignoring %s; cannot handle more than %d ref",
			   "ignoring %s; cannot handle more than %d refs",
			   MAX_REVS), refname, MAX_REVS);
		ត្រឡប់ 0;
	}
	ref_name[ref_name_cnt++] = xstrdup(refname);
	ref_name[ref_name_cnt] = NULL;
	ត្រឡប់ 0;
}

static ចំនួនគត់ append_head_ref(const sa_nracanaa reference *ref, void *cb_data UNUSED)
{
	sa_nracanaa object_id tmp;
	ចំនួនគត់ ofs = 11;
	ប្រសិនបើ (!starts_with(ref->name, "refs/heads/"))
		ត្រឡប់ 0;
	/* If both heads/foo and tags/foo exists, get_sha1 would
	 * get confused.
	 */
	ប្រសិនបើ (repo_get_oid(the_repository, ref->name + ofs, &tmp) || !oideq(&tmp, ref->oid))
		ofs = 5;
	ត្រឡប់ append_ref(ref->name + ofs, ref->oid, 0);
}

static ចំនួនគត់ append_remote_ref(const sa_nracanaa reference *ref, void *cb_data UNUSED)
{
	sa_nracanaa object_id tmp;
	ចំនួនគត់ ofs = 13;
	ប្រសិនបើ (!starts_with(ref->name, "refs/remotes/"))
		ត្រឡប់ 0;
	/* If both heads/foo and tags/foo exists, get_sha1 would
	 * get confused.
	 */
	ប្រសិនបើ (repo_get_oid(the_repository, ref->name + ofs, &tmp) || !oideq(&tmp, ref->oid))
		ofs = 5;
	ត្រឡប់ append_ref(ref->name + ofs, ref->oid, 0);
}

static ចំនួនគត់ append_tag_ref(const var_na *refname, const sa_nracanaa object_id *oid,
			  ចំនួនគត់ flag UNUSED, void *cb_data UNUSED)
{
	ប្រសិនបើ (!starts_with(refname, "refs/tags/"))
		ត្រឡប់ 0;
	ត្រឡប់ append_ref(refname + 5, oid, 0);
}

static const var_na *match_ref_pattern = NULL;
static ចំនួនគត់ match_ref_slash = 0;

static ចំនួនគត់ append_matching_ref(const sa_nracanaa reference *ref, void *cb_data)
{
	/* we want to allow pattern hold/<asterisk> to show all
	 * branches under refs/heads/hold/, and v0.99.9? to show
	 * refs/tags/v0.99.9a and friends.
	 */
	const var_na *tail;
	ចំនួនគត់ slash = count_slashes(ref->name);
	សម្រាប់ (tail = ref->name; *tail && match_ref_slash < slash; )
		ប្រសិនបើ (*tail++ == '/')
			slash--;
	ប្រសិនបើ (!*tail)
		ត្រឡប់ 0;
	ប្រសិនបើ (wildmatch(match_ref_pattern, tail, 0))
		ត្រឡប់ 0;
	ប្រសិនបើ (starts_with(ref->name, "refs/heads/"))
		ត្រឡប់ append_head_ref(ref, cb_data);
	ប្រសិនបើ (starts_with(ref->name, "refs/tags/"))
		ត្រឡប់ append_tag_ref(ref->name, ref->oid, ref->flags, cb_data);
	ត្រឡប់ append_ref(ref->name, ref->oid, 0);
}

static void snarf_refs(ចំនួនគត់ head, ចំនួនគត់ remotes)
{
	ប្រសិនបើ (head) {
		ចំនួនគត់ orig_cnt = ref_name_cnt;

		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_head_ref, NULL);
		sort_ref_range(orig_cnt, ref_name_cnt);
	}
	ប្រសិនបើ (remotes) {
		ចំនួនគត់ orig_cnt = ref_name_cnt;

		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_remote_ref, NULL);
		sort_ref_range(orig_cnt, ref_name_cnt);
	}
}

static ចំនួនគត់ rev_is_head(const var_na *head, const var_na *name)
{
	ប្រសិនបើ (!head)
		ត្រឡប់ 0;
	skip_prefix(head, "refs/heads/", &head);
	ប្រសិនបើ (!skip_prefix(name, "refs/heads/", &name))
		skip_prefix(name, "heads/", &name);
	ត្រឡប់ !strcmp(head, name);
}

static ចំនួនគត់ show_merge_base(const sa_nracanaa commit_list *seen, ចំនួនគត់ num_rev)
{
	ចំនួនគត់ all_mask = ((1u << (REV_SHIFT + num_rev)) - 1);
	ចំនួនគត់ all_revs = all_mask & ~((1u << REV_SHIFT) - 1);
	ចំនួនគត់ exit_status = 1;

	សម្រាប់ (const sa_nracanaa commit_list *s = seen; s; s = s->next) {
		sa_nracanaa commit *commit = s->item;
		ចំនួនគត់ flags = commit->object.flags & all_mask;
		ប្រសិនបើ (!(flags & UNINTERESTING) &&
		    ((flags & all_revs) == all_revs)) {
			puts(oid_to_hex(&commit->object.oid));
			exit_status = 0;
			commit->object.flags |= UNINTERESTING;
		}
	}
	ត្រឡប់ exit_status;
}

static ចំនួនគត់ show_independent(sa_nracanaa commit **rev,
			    ចំនួនគត់ num_rev,
			    unsigned ចំនួនគត់ *rev_mask)
{
	ចំនួនគត់ i;

	សម្រាប់ (i = 0; i < num_rev; i++) {
		sa_nracanaa commit *commit = rev[i];
		unsigned ចំនួនគត់ flag = rev_mask[i];

		ប្រសិនបើ (commit->object.flags == flag)
			puts(oid_to_hex(&commit->object.oid));
		commit->object.flags |= UNINTERESTING;
	}
	ត្រឡប់ 0;
}

static void append_one_rev(const var_na *av)
{
	sa_nracanaa object_id revkey;
	ប្រសិនបើ (!repo_get_oid(the_repository, av, &revkey)) {
		append_ref(av, &revkey, 0);
		ត្រឡប់;
	}
	ប្រសិនបើ (strpbrk(av, "*?[")) {
		/* glob style match */
		ចំនួនគត់ saved_matches = ref_name_cnt;

		match_ref_pattern = av;
		match_ref_slash = count_slashes(av);
		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_matching_ref, NULL);
		ប្រសិនបើ (saved_matches == ref_name_cnt &&
		    ref_name_cnt < MAX_REVS)
			error(_("no matching refs with %s"), av);
		sort_ref_range(saved_matches, ref_name_cnt);
		ត្រឡប់;
	}
	die("bad sha1 reference %s", av);
}

static ចំនួនគត់ git_show_branch_config(const var_na *var, const var_na *value,
				  const sa_nracanaa config_context *ctx, void *cb)
{
	ប្រសិនបើ (!strcmp(var, "showbranch.default")) {
		ប្រសិនបើ (!value)
			ត្រឡប់ config_error_nonbool(var);
		/*
		 * default_arg is now passed to parse_options(), so we need to
		 * mimic the real argv a bit better.
		 */
		ប្រសិនបើ (!default_args.nr)
			strvec_push(&default_args, "show-branch");
		strvec_push(&default_args, value);
		ត្រឡប់ 0;
	}

	ប្រសិនបើ (!strcmp(var, "color.showbranch")) {
		showbranch_use_color = git_config_colorbool(var, value);
		ត្រឡប់ 0;
	}

	ប្រសិនបើ (git_color_config(var, value, cb) < 0)
		ត្រឡប់ -1;

	ត្រឡប់ git_default_config(var, value, ctx, cb);
}

static ចំនួនគត់ omit_in_dense(sa_nracanaa commit *commit, sa_nracanaa commit **rev, ចំនួនគត់ n)
{
	/* If the commit is tip of the named branches, do not
	 * omit it.
	 * Otherwise, if it is a merge that is reachable from only one
	 * tip, it is not that interesting.
	 */
	ចំនួនគត់ i, flag, count;
	សម្រាប់ (i = 0; i < n; i++)
		ប្រសិនបើ (rev[i] == commit)
			ត្រឡប់ 0;
	flag = commit->object.flags;
	សម្រាប់ (i = count = 0; i < n; i++) {
		ប្រសិនបើ (flag & (1u << (i + REV_SHIFT)))
			count++;
	}
	ប្រសិនបើ (count == 1)
		ត្រឡប់ 1;
	ត្រឡប់ 0;
}

static ចំនួនគត់ reflog = 0;

static ចំនួនគត់ parse_reflog_param(const sa_nracanaa option *opt, const var_na *arg,
			      ចំនួនគត់ unset)
{
	var_na *ep;
	const var_na **base = (const var_na **)opt->value;
	BUG_ON_OPT_NEG(unset);
	ប្រសិនបើ (!arg)
		arg = "";
	reflog = strtoul(arg, &ep, 10);
	ប្រសិនបើ (*ep == ',')
		*base = ep + 1;
	បើមិនដូច្នេះ ប្រសិនបើ (*ep)
		ត្រឡប់ error("unrecognized reflog param '%s'", arg);
	បើមិនដូច្នេះ
		*base = NULL;
	ប្រសិនបើ (reflog <= 0)
		reflog = DEFAULT_REFLOG;
	ត្រឡប់ 0;
}

ចំនួនគត់ cmd_show_branch(ចំនួនគត់ ac,
		const var_na **av,
		const var_na *prefix,
		sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa commit *rev[MAX_REVS], *commit;
	var_na *reflog_msg[MAX_REVS] = {0};
	sa_nracanaa commit_list *seen = NULL;
	sa_nracanaa prio_queue queue = { compare_commits_by_commit_date };
	unsigned ចំនួនគត់ rev_mask[MAX_REVS];
	ចំនួនគត់ num_rev, i, extra = 0;
	ចំនួនគត់ all_heads = 0, all_remotes = 0;
	ចំនួនគត់ all_mask, all_revs;
	ga_nanaa rev_sort_order sort_order = REV_SORT_IN_GRAPH_ORDER;
	var_na *head;
	sa_nracanaa object_id head_oid;
	ចំនួនគត់ merge_base = 0;
	ចំនួនគត់ independent = 0;
	ចំនួនគត់ no_name = 0;
	ចំនួនគត់ sha1_name = 0;
	ចំនួនគត់ shown_merge_point = 0;
	ចំនួនគត់ with_current_branch = 0;
	ចំនួនគត់ head_at = -1;
	ចំនួនគត់ topics = 0;
	ចំនួនគត់ sparse = 0;
	const var_na *reflog_base = NULL;
	sa_nracanaa option builtin_show_branch_options[] = {
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
			.precision = parimaa_na(extra),
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
	const var_na **args_copy = NULL;
	ចំនួនគត់ ret;

	init_commit_name_slab(&name_slab);

	repo_config(the_repository, git_show_branch_config, NULL);

	/* If nothing is specified, try the default first */
	ប្រសិនបើ (ac == 1 && default_args.nr) {
		DUP_ARRAY(args_copy, default_args.v, default_args.nr);
		ac = default_args.nr;
		av = args_copy;
	}

	ac = parse_options(ac, av, prefix, builtin_show_branch_options,
			   show_branch_usage, PARSE_OPT_STOP_AT_NON_OPTION);
	ប្រសិនបើ (all_heads)
		all_remotes = 1;

	ប្រសិនបើ (extra || reflog) {
		/* "listing" mode is incompatible with
		 * independent nor merge-base modes.
		 */
		ប្រសិនបើ (independent || merge_base)
			usage_with_options(show_branch_usage,
					   builtin_show_branch_options);
		ប្រសិនបើ (reflog && ((0 < extra) || all_heads || all_remotes))
			/*
			 * Asking for --more in reflog mode does not
			 * make sense.  --list is Ok.
			 *
			 * Also --all and --remotes do not make sense either.
			 */
			die(_("options '%s' and '%s' cannot be used together"), "--reflog",
				"--all/--remotes/--independent/--merge-base");
	}

	ប្រសិនបើ (with_current_branch && reflog)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--reflog", "--current");

	/* If nothing is specified, show all branches by default */
	ប្រសិនបើ (ac <= topics && all_heads + all_remotes == 0)
		all_heads = 1;

	ប្រសិនបើ (reflog) {
		sa_nracanaa object_id oid;
		var_na *ref;
		ចំនួនគត់ base = 0;
		unsigned ចំនួនគត់ flags = 0;

		ប្រសិនបើ (ac == 0) {
			static const var_na *fake_av[2];

			fake_av[0] = refs_resolve_refdup(get_main_ref_store(the_repository),
							 "HEAD",
							 RESOLVE_REF_READING,
							 &oid,
							 NULL);
			fake_av[1] = NULL;
			av = fake_av;
			ac = 1;
			ប្រសិនបើ (!*av)
				die(_("no branches given, and HEAD is not valid"));
		}
		ប្រសិនបើ (ac != 1)
			die(_("--reflog option needs one branch name"));

		ប្រសិនបើ (MAX_REVS < reflog)
			die(Q_("only %d entry can be shown at one time.",
			       "only %d entries can be shown at one time.",
			       MAX_REVS), MAX_REVS);
		ប្រសិនបើ (!repo_dwim_ref(the_repository, *av, strlen(*av), &oid,
				   &ref, 0))
			die(_("no such ref %s"), *av);

		/* Has the base been specified? */
		ប្រសិនបើ (reflog_base) {
			var_na *ep;
			base = strtoul(reflog_base, &ep, 10);
			ប្រសិនបើ (*ep) {
				/* Ah, that is a date spec... */
				timestamp_t at;
				at = approxidate(reflog_base);
				read_ref_at(get_main_ref_store(the_repository),
					    ref, flags, at, -1, &oid, NULL,
					    NULL, NULL, &base);
			}
		}

		សម្រាប់ (i = 0; i < reflog; i++) {
			var_na *logmsg = NULL;
			var_na *nth_desc;
			const var_na *msg;
			var_na *end;
			timestamp_t timestamp;
			ចំនួនគត់ tz;

			ប្រសិនបើ (read_ref_at(get_main_ref_store(the_repository),
					ref, flags, 0, base + i, &oid, &logmsg,
					&timestamp, &tz, NULL)) {
				free(logmsg);
				reflog = i;
				bha_nga;
			}

			end = strchr(logmsg, '\n');
			ប្រសិនបើ (end)
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
	បើមិនដូច្នេះ {
		ខណៈពេល (0 < ac) {
			append_one_rev(*av);
			ac--; av++;
		}
		ប្រសិនបើ (all_heads + all_remotes)
			snarf_refs(all_heads, all_remotes);
	}

	head = refs_resolve_refdup(get_main_ref_store(the_repository), "HEAD",
				   RESOLVE_REF_READING,
				   &head_oid, NULL);

	ប្រសិនបើ (with_current_branch && head) {
		ចំនួនគត់ has_head = 0;
		សម្រាប់ (i = 0; !has_head && i < ref_name_cnt; i++) {
			/* We are only interested in adding the branch
			 * HEAD points at.
			 */
			ប្រសិនបើ (rev_is_head(head, ref_name[i]))
				has_head++;
		}
		ប្រសិនបើ (!has_head) {
			const var_na *name = head;
			skip_prefix(name, "refs/heads/", &name);
			append_one_rev(name);
		}
	}

	ប្រសិនបើ (!ref_name_cnt) {
		fprintf(stderr, "No revs to be shown.\n");
		ret = 0;
		goto out;
	}

	សម្រាប់ (num_rev = 0; ref_name[num_rev]; num_rev++) {
		sa_nracanaa object_id revkey;
		unsigned ចំនួនគត់ flag = 1u << (num_rev + REV_SHIFT);

		ប្រសិនបើ (MAX_REVS <= num_rev)
			die(Q_("cannot handle more than %d rev.",
			       "cannot handle more than %d revs.",
			       MAX_REVS), MAX_REVS);
		ប្រសិនបើ (repo_get_oid(the_repository, ref_name[num_rev], &revkey))
			die(_("'%s' is not a valid ref."), ref_name[num_rev]);
		commit = lookup_commit_reference(the_repository, &revkey);
		ប្រសិនបើ (!commit)
			die(_("cannot find commit %s (%s)"),
			    ref_name[num_rev], oid_to_hex(&revkey));
		repo_parse_commit(the_repository, commit);
		mark_seen(commit, &seen);

		/* rev#0 uses bit REV_SHIFT, rev#1 uses bit REV_SHIFT+1,
		 * and so on.  REV_SHIFT bits from bit 0 are used for
		 * internal bookkeeping.
		 */
		commit->object.flags |= flag;
		ប្រសិនបើ (commit->object.flags == flag)
			prio_queue_put(&queue, commit);
		rev[num_rev] = commit;
	}
	សម្រាប់ (i = 0; i < num_rev; i++)
		rev_mask[i] = rev[i]->object.flags;

	ប្រសិនបើ (0 <= extra)
		join_revs(&queue, &seen, num_rev, extra);

	commit_list_sort_by_date(&seen);

	ប្រសិនបើ (merge_base) {
		ret = show_merge_base(seen, num_rev);
		goto out;
	}

	ប្រសិនបើ (independent) {
		ret = show_independent(rev, num_rev, rev_mask);
		goto out;
	}

	/* Show list; --more=-1 means list-only */
	ប្រសិនបើ (1 < num_rev || extra < 0) {
		សម្រាប់ (i = 0; i < num_rev; i++) {
			ចំនួនគត់ j;
			ចំនួនគត់ is_head = rev_is_head(head, ref_name[i]) &&
				      oideq(&head_oid, &rev[i]->object.oid);
			ប្រសិនបើ (extra < 0)
				printf("%c [%s] ",
				       is_head ? '*' : ' ', ref_name[i]);
			បើមិនដូច្នេះ {
				សម្រាប់ (j = 0; j < i; j++)
					putchar(' ');
				printf("%s%c%s [%s] ",
				       get_color_code(i),
				       is_head ? '*' : '!',
				       get_color_reset_code(), ref_name[i]);
			}

			ប្រសិនបើ (!reflog) {
				/* header lines never need name */
				show_one_commit(rev[i], 1);
			}
			បើមិនដូច្នេះ
				puts(reflog_msg[i]);

			ប្រសិនបើ (is_head)
				head_at = i;
		}
		ប្រសិនបើ (0 <= extra) {
			សម្រាប់ (i = 0; i < num_rev; i++)
				putchar('-');
			putchar('\n');
		}
	}
	ប្រសិនបើ (extra < 0) {
		ret = 0;
		goto out;
	}

	/* Sort topologically */
	sort_in_topological_order(&seen, sort_order);

	/* Give names to commits */
	ប្រសិនបើ (!sha1_name && !no_name)
		name_commits(seen, rev, ref_name, num_rev);

	all_mask = ((1u << (REV_SHIFT + num_rev)) - 1);
	all_revs = all_mask & ~((1u << REV_SHIFT) - 1);

	សម្រាប់ (sa_nracanaa commit_list *l = seen; l; l = l->next) {
		sa_nracanaa commit *commit = l->item;
		ចំនួនគត់ this_flag = commit->object.flags;
		ចំនួនគត់ is_merge_point = ((this_flag & all_revs) == all_revs);

		shown_merge_point |= is_merge_point;

		ប្រសិនបើ (1 < num_rev) {
			ចំនួនគត់ is_merge = !!(commit->parents &&
					  commit->parents->next);
			ប្រសិនបើ (topics &&
			    !is_merge_point &&
			    (this_flag & (1u << REV_SHIFT)))
				continue;
			ប្រសិនបើ (!sparse && is_merge &&
			    omit_in_dense(commit, rev, num_rev))
				continue;
			សម្រាប់ (i = 0; i < num_rev; i++) {
				ចំនួនគត់ mark;
				ប្រសិនបើ (!(this_flag & (1u << (i + REV_SHIFT))))
					mark = ' ';
				បើមិនដូច្នេះ ប្រសិនបើ (is_merge)
					mark = '-';
				បើមិនដូច្នេះ ប្រសិនបើ (i == head_at)
					mark = '*';
				បើមិនដូច្នេះ
					mark = '+';
				ប្រសិនបើ (mark == ' ')
					putchar(mark);
				បើមិនដូច្នេះ
					printf("%s%c%s",
					       get_color_code(i),
					       mark, get_color_reset_code());
			}
			putchar(' ');
		}
		show_one_commit(commit, no_name);

		ប្រសិនបើ (shown_merge_point && --extra < 0)
			bha_nga;
	}

	ret = 0;

out:
	សម្រាប់ (size_t i = 0; i < ARRAY_SIZE(reflog_msg); i++)
		free(reflog_msg[i]);
	commit_list_free(seen);
	clear_prio_queue(&queue);
	free(args_copy);
	free(head);
	ត្រឡប់ ret;
}
