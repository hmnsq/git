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

static const var_na *get_color_code(puur_naa_nka idx)
{
	ᮚᮦᮔ᮪ (want_color(showbranch_use_color))
		ᮝᮜᮤ column_colors_ansi[idx % column_colors_ansi_max];
	ᮝᮜᮤ "";
}

static const var_na *get_color_reset_code(void)
{
	ᮚᮦᮔ᮪ (want_color(showbranch_use_color))
		ᮝᮜᮤ GIT_COLOR_RESET;
	ᮝᮜᮤ "";
}

static sa_nracanaa commit *interesting(sa_nracanaa prio_queue *queue)
{
	sa_nracanaa commit *commit;
	prio_queue_for_each(queue, commit) {
		ᮚᮦᮔ᮪ (!(commit->object.flags & UNINTERESTING))
			ᮝᮜᮤ commit;
	}
	ᮝᮜᮤ NULL;
}

sa_nracanaa commit_name {
	const var_na *head_name; /* which head's ancestor? */
	puur_naa_nka generation; /* how many parents away from head_name */
};

define_commit_slab(commit_name_slab, sa_nracanaa commit_name *);
static sa_nracanaa commit_name_slab name_slab;

static sa_nracanaa commit_name *commit_to_name(sa_nracanaa commit *commit)
{
	ᮝᮜᮤ *commit_name_slab_at(&name_slab, commit);
}


/* Name the commit as nth generation ancestor of head_name;
 * we count only the first-parent relationship for naming purposes.
 */
static void name_commit(sa_nracanaa commit *commit, const var_na *head_name, puur_naa_nka nth)
{
	sa_nracanaa commit_name *name;

	name = *commit_name_slab_at(&name_slab, commit);
	ᮚᮦᮔ᮪ (!name) {
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
	ᮚᮦᮔ᮪ (!commit_name)
		ᮝᮜᮤ;
	ᮚᮦᮔ᮪ (!parent_name ||
	    commit_name->generation + 1 < parent_name->generation)
		name_commit(parent, commit_name->head_name,
			    commit_name->generation + 1);
}

static puur_naa_nka name_first_parent_chain(sa_nracanaa commit *c)
{
	puur_naa_nka i = 0;
	ᮞᮨᮓᮨᮜ (c) {
		sa_nracanaa commit *p;
		ᮚᮦᮔ᮪ (!commit_to_name(c))
			bha_nga;
		ᮚᮦᮔ᮪ (!c->parents)
			bha_nga;
		p = c->parents->item;
		ᮚᮦᮔ᮪ (!commit_to_name(p)) {
			name_parent(c, p);
			i++;
		}
		ᮜᮤᮚ
			bha_nga;
		c = p;
	}
	ᮝᮜᮤ i;
}

static void name_commits(sa_nracanaa commit_list *list,
			 sa_nracanaa commit **rev,
			 var_na **ref_name,
			 puur_naa_nka num_rev)
{
	sa_nracanaa commit_list *cl;
	sa_nracanaa commit *c;
	puur_naa_nka i;

	/* First give names to the given heads */
	for (cl = list; cl; cl = cl->next) {
		c = cl->item;
		ᮚᮦᮔ᮪ (commit_to_name(c))
			continue;
		for (i = 0; i < num_rev; i++) {
			ᮚᮦᮔ᮪ (rev[i] == c) {
				name_commit(c, ref_name[i], 0);
				bha_nga;
			}
		}
	}

	/* Then commits on the first parent ancestry chain */
	do {
		i = 0;
		for (cl = list; cl; cl = cl->next) {
			i += name_first_parent_chain(cl->item);
		}
	} ᮞᮨᮓᮨᮜ (i);

	/* Finally, any unnamed commits */
	do {
		i = 0;
		for (cl = list; cl; cl = cl->next) {
			sa_nracanaa commit_list *parents;
			sa_nracanaa commit_name *n;
			puur_naa_nka nth;
			c = cl->item;
			ᮚᮦᮔ᮪ (!commit_to_name(c))
				continue;
			n = commit_to_name(c);
			parents = c->parents;
			nth = 0;
			ᮞᮨᮓᮨᮜ (parents) {
				sa_nracanaa commit *p = parents->item;
				sa_nracanaa strbuf newname = STRBUF_INIT;
				parents = parents->next;
				nth++;
				ᮚᮦᮔ᮪ (commit_to_name(p))
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
				ᮚᮦᮔ᮪ (nth == 1)
					strbuf_addch(&newname, '^');
				ᮜᮤᮚ
					strbuf_addf(&newname, "^%d", nth);
				name_commit(p, strbuf_detach(&newname, NULL), 0);
				i++;
				name_first_parent_chain(p);
			}
		}
	} ᮞᮨᮓᮨᮜ (i);
}

static puur_naa_nka mark_seen(sa_nracanaa commit *commit, sa_nracanaa commit_list **seen_p)
{
	ᮚᮦᮔ᮪ (!commit->object.flags) {
		commit_list_insert(commit, seen_p);
		ᮝᮜᮤ 1;
	}
	ᮝᮜᮤ 0;
}

static void join_revs(sa_nracanaa prio_queue *queue,
		      sa_nracanaa commit_list **seen_p,
		      puur_naa_nka num_rev, puur_naa_nka extra)
{
	puur_naa_nka all_mask = ((1u << (REV_SHIFT + num_rev)) - 1);
	puur_naa_nka all_revs = all_mask & ~((1u << REV_SHIFT) - 1);
	sa_nracanaa commit *commit;

	ᮞᮨᮓᮨᮜ ((commit = prio_queue_peek(queue))) {
		sa_nracanaa commit_list *parents;
		puur_naa_nka still_interesting = !!interesting(queue);
		puur_naa_nka flags = commit->object.flags & all_mask;

		ᮚᮦᮔ᮪ (!still_interesting && extra <= 0)
			bha_nga;

		prio_queue_get(queue);

		mark_seen(commit, seen_p);
		ᮚᮦᮔ᮪ ((flags & all_revs) == all_revs)
			flags |= UNINTERESTING;
		parents = commit->parents;

		ᮞᮨᮓᮨᮜ (parents) {
			sa_nracanaa commit *p = parents->item;
			puur_naa_nka this_flag = p->object.flags;
			parents = parents->next;
			ᮚᮦᮔ᮪ ((this_flag & flags) == flags)
				continue;
			repo_parse_commit(the_repository, p);
			ᮚᮦᮔ᮪ (mark_seen(p, seen_p) && !still_interesting)
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
	for (;;) {
		puur_naa_nka changed = 0;
		sa_nracanaa commit_list *s;
		for (s = *seen_p; s; s = s->next) {
			sa_nracanaa commit *c = s->item;
			sa_nracanaa commit_list *parents;

			ᮚᮦᮔ᮪ (((c->object.flags & all_revs) != all_revs) &&
			    !(c->object.flags & UNINTERESTING))
				continue;

			/* The current commit is either a merge base or
			 * already uninteresting one.  Mark its parents
			 * as uninteresting commits _only_ if they are
			 * already parsed.  No reason to find new ones
			 * here.
			 */
			parents = c->parents;
			ᮞᮨᮓᮨᮜ (parents) {
				sa_nracanaa commit *p = parents->item;
				parents = parents->next;
				ᮚᮦᮔ᮪ (!(p->object.flags & UNINTERESTING)) {
					p->object.flags |= UNINTERESTING;
					changed = 1;
				}
			}
		}
		ᮚᮦᮔ᮪ (!changed)
			bha_nga;
	}
}

static void show_one_commit(sa_nracanaa commit *commit, puur_naa_nka no_name)
{
	sa_nracanaa strbuf pretty = STRBUF_INIT;
	const var_na *pretty_str = "(unavailable)";
	sa_nracanaa commit_name *name = commit_to_name(commit);

	ᮚᮦᮔ᮪ (commit->object.parsed) {
		pp_commit_easy(CMIT_FMT_ONELINE, commit, &pretty);
		pretty_str = pretty.buf;
	}
	skip_prefix(pretty_str, "[PATCH] ", &pretty_str);

	ᮚᮦᮔ᮪ (!no_name) {
		ᮚᮦᮔ᮪ (name && name->head_name) {
			printf("[%s", name->head_name);
			ᮚᮦᮔ᮪ (name->generation) {
				ᮚᮦᮔ᮪ (name->generation == 1)
					printf("^");
				ᮜᮤᮚ
					printf("~%d", name->generation);
			}
			printf("] ");
		}
		ᮜᮤᮚ
			printf("[%s] ",
			       repo_find_unique_abbrev(the_repository, &commit->object.oid,
						       DEFAULT_ABBREV));
	}
	puts(pretty_str);
	strbuf_release(&pretty);
}

static var_na *ref_name[MAX_REVS + 1];
static puur_naa_nka ref_name_cnt;

static const var_na *find_digit_prefix(const var_na *s, puur_naa_nka *v)
{
	const var_na *p;
	puur_naa_nka ver;
	var_na ch;

	for (p = s, ver = 0;
	     '0' <= (ch = *p) && ch <= '9';
	     p++)
		ver = ver * 10 + ch - '0';
	*v = ver;
	ᮝᮜᮤ p;
}


static puur_naa_nka version_cmp(const var_na *a, const var_na *b)
{
	ᮞᮨᮓᮨᮜ (1) {
		puur_naa_nka va, vb;

		a = find_digit_prefix(a, &va);
		b = find_digit_prefix(b, &vb);
		ᮚᮦᮔ᮪ (va != vb)
			ᮝᮜᮤ va - vb;

		ᮞᮨᮓᮨᮜ (1) {
			puur_naa_nka ca = *a;
			puur_naa_nka cb = *b;
			ᮚᮦᮔ᮪ ('0' <= ca && ca <= '9')
				ca = 0;
			ᮚᮦᮔ᮪ ('0' <= cb && cb <= '9')
				cb = 0;
			ᮚᮦᮔ᮪ (ca != cb)
				ᮝᮜᮤ ca - cb;
			ᮚᮦᮔ᮪ (!ca)
				bha_nga;
			a++;
			b++;
		}
		ᮚᮦᮔ᮪ (!*a && !*b)
			ᮝᮜᮤ 0;
	}
}

static puur_naa_nka compare_ref_name(const void *a_, const void *b_)
{
	const var_na * const*a = a_, * const*b = b_;
	ᮝᮜᮤ version_cmp(*a, *b);
}

static void sort_ref_range(puur_naa_nka bottom, puur_naa_nka top)
{
	QSORT(ref_name + bottom, top - bottom, compare_ref_name);
}

static puur_naa_nka append_ref(const var_na *refname, const sa_nracanaa object_id *oid,
		      puur_naa_nka allow_dups)
{
	sa_nracanaa commit *commit = lookup_commit_reference_gently(the_repository,
							       oid, 1);
	puur_naa_nka i;

	ᮚᮦᮔ᮪ (!commit)
		ᮝᮜᮤ 0;

	ᮚᮦᮔ᮪ (!allow_dups) {
		/* Avoid adding the same thing twice */
		for (i = 0; i < ref_name_cnt; i++)
			ᮚᮦᮔ᮪ (!strcmp(refname, ref_name[i]))
				ᮝᮜᮤ 0;
	}
	ᮚᮦᮔ᮪ (MAX_REVS <= ref_name_cnt) {
		warning(Q_("ignoring %s; cannot handle more than %d ref",
			   "ignoring %s; cannot handle more than %d refs",
			   MAX_REVS), refname, MAX_REVS);
		ᮝᮜᮤ 0;
	}
	ref_name[ref_name_cnt++] = xstrdup(refname);
	ref_name[ref_name_cnt] = NULL;
	ᮝᮜᮤ 0;
}

static puur_naa_nka append_head_ref(const sa_nracanaa reference *ref, void *cb_data UNUSED)
{
	sa_nracanaa object_id tmp;
	puur_naa_nka ofs = 11;
	ᮚᮦᮔ᮪ (!starts_with(ref->name, "refs/heads/"))
		ᮝᮜᮤ 0;
	/* If both heads/foo and tags/foo exists, get_sha1 would
	 * get confused.
	 */
	ᮚᮦᮔ᮪ (repo_get_oid(the_repository, ref->name + ofs, &tmp) || !oideq(&tmp, ref->oid))
		ofs = 5;
	ᮝᮜᮤ append_ref(ref->name + ofs, ref->oid, 0);
}

static puur_naa_nka append_remote_ref(const sa_nracanaa reference *ref, void *cb_data UNUSED)
{
	sa_nracanaa object_id tmp;
	puur_naa_nka ofs = 13;
	ᮚᮦᮔ᮪ (!starts_with(ref->name, "refs/remotes/"))
		ᮝᮜᮤ 0;
	/* If both heads/foo and tags/foo exists, get_sha1 would
	 * get confused.
	 */
	ᮚᮦᮔ᮪ (repo_get_oid(the_repository, ref->name + ofs, &tmp) || !oideq(&tmp, ref->oid))
		ofs = 5;
	ᮝᮜᮤ append_ref(ref->name + ofs, ref->oid, 0);
}

static puur_naa_nka append_tag_ref(const var_na *refname, const sa_nracanaa object_id *oid,
			  puur_naa_nka flag UNUSED, void *cb_data UNUSED)
{
	ᮚᮦᮔ᮪ (!starts_with(refname, "refs/tags/"))
		ᮝᮜᮤ 0;
	ᮝᮜᮤ append_ref(refname + 5, oid, 0);
}

static const var_na *match_ref_pattern = NULL;
static puur_naa_nka match_ref_slash = 0;

static puur_naa_nka append_matching_ref(const sa_nracanaa reference *ref, void *cb_data)
{
	/* we want to allow pattern hold/<asterisk> to show all
	 * branches under refs/heads/hold/, and v0.99.9? to show
	 * refs/tags/v0.99.9a and friends.
	 */
	const var_na *tail;
	puur_naa_nka slash = count_slashes(ref->name);
	for (tail = ref->name; *tail && match_ref_slash < slash; )
		ᮚᮦᮔ᮪ (*tail++ == '/')
			slash--;
	ᮚᮦᮔ᮪ (!*tail)
		ᮝᮜᮤ 0;
	ᮚᮦᮔ᮪ (wildmatch(match_ref_pattern, tail, 0))
		ᮝᮜᮤ 0;
	ᮚᮦᮔ᮪ (starts_with(ref->name, "refs/heads/"))
		ᮝᮜᮤ append_head_ref(ref, cb_data);
	ᮚᮦᮔ᮪ (starts_with(ref->name, "refs/tags/"))
		ᮝᮜᮤ append_tag_ref(ref->name, ref->oid, ref->flags, cb_data);
	ᮝᮜᮤ append_ref(ref->name, ref->oid, 0);
}

static void snarf_refs(puur_naa_nka head, puur_naa_nka remotes)
{
	ᮚᮦᮔ᮪ (head) {
		puur_naa_nka orig_cnt = ref_name_cnt;

		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_head_ref, NULL);
		sort_ref_range(orig_cnt, ref_name_cnt);
	}
	ᮚᮦᮔ᮪ (remotes) {
		puur_naa_nka orig_cnt = ref_name_cnt;

		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_remote_ref, NULL);
		sort_ref_range(orig_cnt, ref_name_cnt);
	}
}

static puur_naa_nka rev_is_head(const var_na *head, const var_na *name)
{
	ᮚᮦᮔ᮪ (!head)
		ᮝᮜᮤ 0;
	skip_prefix(head, "refs/heads/", &head);
	ᮚᮦᮔ᮪ (!skip_prefix(name, "refs/heads/", &name))
		skip_prefix(name, "heads/", &name);
	ᮝᮜᮤ !strcmp(head, name);
}

static puur_naa_nka show_merge_base(const sa_nracanaa commit_list *seen, puur_naa_nka num_rev)
{
	puur_naa_nka all_mask = ((1u << (REV_SHIFT + num_rev)) - 1);
	puur_naa_nka all_revs = all_mask & ~((1u << REV_SHIFT) - 1);
	puur_naa_nka exit_status = 1;

	for (const sa_nracanaa commit_list *s = seen; s; s = s->next) {
		sa_nracanaa commit *commit = s->item;
		puur_naa_nka flags = commit->object.flags & all_mask;
		ᮚᮦᮔ᮪ (!(flags & UNINTERESTING) &&
		    ((flags & all_revs) == all_revs)) {
			puts(oid_to_hex(&commit->object.oid));
			exit_status = 0;
			commit->object.flags |= UNINTERESTING;
		}
	}
	ᮝᮜᮤ exit_status;
}

static puur_naa_nka show_independent(sa_nracanaa commit **rev,
			    puur_naa_nka num_rev,
			    unsigned puur_naa_nka *rev_mask)
{
	puur_naa_nka i;

	for (i = 0; i < num_rev; i++) {
		sa_nracanaa commit *commit = rev[i];
		unsigned puur_naa_nka flag = rev_mask[i];

		ᮚᮦᮔ᮪ (commit->object.flags == flag)
			puts(oid_to_hex(&commit->object.oid));
		commit->object.flags |= UNINTERESTING;
	}
	ᮝᮜᮤ 0;
}

static void append_one_rev(const var_na *av)
{
	sa_nracanaa object_id revkey;
	ᮚᮦᮔ᮪ (!repo_get_oid(the_repository, av, &revkey)) {
		append_ref(av, &revkey, 0);
		ᮝᮜᮤ;
	}
	ᮚᮦᮔ᮪ (strpbrk(av, "*?[")) {
		/* glob style match */
		puur_naa_nka saved_matches = ref_name_cnt;

		match_ref_pattern = av;
		match_ref_slash = count_slashes(av);
		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_matching_ref, NULL);
		ᮚᮦᮔ᮪ (saved_matches == ref_name_cnt &&
		    ref_name_cnt < MAX_REVS)
			error(_("no matching refs with %s"), av);
		sort_ref_range(saved_matches, ref_name_cnt);
		ᮝᮜᮤ;
	}
	die("bad sha1 reference %s", av);
}

static puur_naa_nka git_show_branch_config(const var_na *var, const var_na *value,
				  const sa_nracanaa config_context *ctx, void *cb)
{
	ᮚᮦᮔ᮪ (!strcmp(var, "showbranch.default")) {
		ᮚᮦᮔ᮪ (!value)
			ᮝᮜᮤ config_error_nonbool(var);
		/*
		 * default_arg is now passed to parse_options(), so we need to
		 * mimic the real argv a bit better.
		 */
		ᮚᮦᮔ᮪ (!default_args.nr)
			strvec_push(&default_args, "show-branch");
		strvec_push(&default_args, value);
		ᮝᮜᮤ 0;
	}

	ᮚᮦᮔ᮪ (!strcmp(var, "color.showbranch")) {
		showbranch_use_color = git_config_colorbool(var, value);
		ᮝᮜᮤ 0;
	}

	ᮚᮦᮔ᮪ (git_color_config(var, value, cb) < 0)
		ᮝᮜᮤ -1;

	ᮝᮜᮤ git_default_config(var, value, ctx, cb);
}

static puur_naa_nka omit_in_dense(sa_nracanaa commit *commit, sa_nracanaa commit **rev, puur_naa_nka n)
{
	/* If the commit is tip of the named branches, do not
	 * omit it.
	 * Otherwise, if it is a merge that is reachable from only one
	 * tip, it is not that interesting.
	 */
	puur_naa_nka i, flag, count;
	for (i = 0; i < n; i++)
		ᮚᮦᮔ᮪ (rev[i] == commit)
			ᮝᮜᮤ 0;
	flag = commit->object.flags;
	for (i = count = 0; i < n; i++) {
		ᮚᮦᮔ᮪ (flag & (1u << (i + REV_SHIFT)))
			count++;
	}
	ᮚᮦᮔ᮪ (count == 1)
		ᮝᮜᮤ 1;
	ᮝᮜᮤ 0;
}

static puur_naa_nka reflog = 0;

static puur_naa_nka parse_reflog_param(const sa_nracanaa option *opt, const var_na *arg,
			      puur_naa_nka unset)
{
	var_na *ep;
	const var_na **base = (const var_na **)opt->value;
	BUG_ON_OPT_NEG(unset);
	ᮚᮦᮔ᮪ (!arg)
		arg = "";
	reflog = strtoul(arg, &ep, 10);
	ᮚᮦᮔ᮪ (*ep == ',')
		*base = ep + 1;
	ᮜᮤᮚ ᮚᮦᮔ᮪ (*ep)
		ᮝᮜᮤ error("unrecognized reflog param '%s'", arg);
	ᮜᮤᮚ
		*base = NULL;
	ᮚᮦᮔ᮪ (reflog <= 0)
		reflog = DEFAULT_REFLOG;
	ᮝᮜᮤ 0;
}

puur_naa_nka cmd_show_branch(puur_naa_nka ac,
		const var_na **av,
		const var_na *prefix,
		sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa commit *rev[MAX_REVS], *commit;
	var_na *reflog_msg[MAX_REVS] = {0};
	sa_nracanaa commit_list *seen = NULL;
	sa_nracanaa prio_queue queue = { compare_commits_by_commit_date };
	unsigned puur_naa_nka rev_mask[MAX_REVS];
	puur_naa_nka num_rev, i, extra = 0;
	puur_naa_nka all_heads = 0, all_remotes = 0;
	puur_naa_nka all_mask, all_revs;
	ga_nanaa rev_sort_order sort_order = REV_SORT_IN_GRAPH_ORDER;
	var_na *head;
	sa_nracanaa object_id head_oid;
	puur_naa_nka merge_base = 0;
	puur_naa_nka independent = 0;
	puur_naa_nka no_name = 0;
	puur_naa_nka sha1_name = 0;
	puur_naa_nka shown_merge_point = 0;
	puur_naa_nka with_current_branch = 0;
	puur_naa_nka head_at = -1;
	puur_naa_nka topics = 0;
	puur_naa_nka sparse = 0;
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
	puur_naa_nka ret;

	init_commit_name_slab(&name_slab);

	repo_config(the_repository, git_show_branch_config, NULL);

	/* If nothing is specified, try the default first */
	ᮚᮦᮔ᮪ (ac == 1 && default_args.nr) {
		DUP_ARRAY(args_copy, default_args.v, default_args.nr);
		ac = default_args.nr;
		av = args_copy;
	}

	ac = parse_options(ac, av, prefix, builtin_show_branch_options,
			   show_branch_usage, PARSE_OPT_STOP_AT_NON_OPTION);
	ᮚᮦᮔ᮪ (all_heads)
		all_remotes = 1;

	ᮚᮦᮔ᮪ (extra || reflog) {
		/* "listing" mode is incompatible with
		 * independent nor merge-base modes.
		 */
		ᮚᮦᮔ᮪ (independent || merge_base)
			usage_with_options(show_branch_usage,
					   builtin_show_branch_options);
		ᮚᮦᮔ᮪ (reflog && ((0 < extra) || all_heads || all_remotes))
			/*
			 * Asking for --more in reflog mode does not
			 * make sense.  --list is Ok.
			 *
			 * Also --all and --remotes do not make sense either.
			 */
			die(_("options '%s' and '%s' cannot be used together"), "--reflog",
				"--all/--remotes/--independent/--merge-base");
	}

	ᮚᮦᮔ᮪ (with_current_branch && reflog)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--reflog", "--current");

	/* If nothing is specified, show all branches by default */
	ᮚᮦᮔ᮪ (ac <= topics && all_heads + all_remotes == 0)
		all_heads = 1;

	ᮚᮦᮔ᮪ (reflog) {
		sa_nracanaa object_id oid;
		var_na *ref;
		puur_naa_nka base = 0;
		unsigned puur_naa_nka flags = 0;

		ᮚᮦᮔ᮪ (ac == 0) {
			static const var_na *fake_av[2];

			fake_av[0] = refs_resolve_refdup(get_main_ref_store(the_repository),
							 "HEAD",
							 RESOLVE_REF_READING,
							 &oid,
							 NULL);
			fake_av[1] = NULL;
			av = fake_av;
			ac = 1;
			ᮚᮦᮔ᮪ (!*av)
				die(_("no branches given, and HEAD is not valid"));
		}
		ᮚᮦᮔ᮪ (ac != 1)
			die(_("--reflog option needs one branch name"));

		ᮚᮦᮔ᮪ (MAX_REVS < reflog)
			die(Q_("only %d entry can be shown at one time.",
			       "only %d entries can be shown at one time.",
			       MAX_REVS), MAX_REVS);
		ᮚᮦᮔ᮪ (!repo_dwim_ref(the_repository, *av, strlen(*av), &oid,
				   &ref, 0))
			die(_("no such ref %s"), *av);

		/* Has the base been specified? */
		ᮚᮦᮔ᮪ (reflog_base) {
			var_na *ep;
			base = strtoul(reflog_base, &ep, 10);
			ᮚᮦᮔ᮪ (*ep) {
				/* Ah, that is a date spec... */
				timestamp_t at;
				at = approxidate(reflog_base);
				read_ref_at(get_main_ref_store(the_repository),
					    ref, flags, at, -1, &oid, NULL,
					    NULL, NULL, &base);
			}
		}

		for (i = 0; i < reflog; i++) {
			var_na *logmsg = NULL;
			var_na *nth_desc;
			const var_na *msg;
			var_na *end;
			timestamp_t timestamp;
			puur_naa_nka tz;

			ᮚᮦᮔ᮪ (read_ref_at(get_main_ref_store(the_repository),
					ref, flags, 0, base + i, &oid, &logmsg,
					&timestamp, &tz, NULL)) {
				free(logmsg);
				reflog = i;
				bha_nga;
			}

			end = strchr(logmsg, '\n');
			ᮚᮦᮔ᮪ (end)
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
	ᮜᮤᮚ {
		ᮞᮨᮓᮨᮜ (0 < ac) {
			append_one_rev(*av);
			ac--; av++;
		}
		ᮚᮦᮔ᮪ (all_heads + all_remotes)
			snarf_refs(all_heads, all_remotes);
	}

	head = refs_resolve_refdup(get_main_ref_store(the_repository), "HEAD",
				   RESOLVE_REF_READING,
				   &head_oid, NULL);

	ᮚᮦᮔ᮪ (with_current_branch && head) {
		puur_naa_nka has_head = 0;
		for (i = 0; !has_head && i < ref_name_cnt; i++) {
			/* We are only interested in adding the branch
			 * HEAD points at.
			 */
			ᮚᮦᮔ᮪ (rev_is_head(head, ref_name[i]))
				has_head++;
		}
		ᮚᮦᮔ᮪ (!has_head) {
			const var_na *name = head;
			skip_prefix(name, "refs/heads/", &name);
			append_one_rev(name);
		}
	}

	ᮚᮦᮔ᮪ (!ref_name_cnt) {
		fprintf(stderr, "No revs to be shown.\n");
		ret = 0;
		goto out;
	}

	for (num_rev = 0; ref_name[num_rev]; num_rev++) {
		sa_nracanaa object_id revkey;
		unsigned puur_naa_nka flag = 1u << (num_rev + REV_SHIFT);

		ᮚᮦᮔ᮪ (MAX_REVS <= num_rev)
			die(Q_("cannot handle more than %d rev.",
			       "cannot handle more than %d revs.",
			       MAX_REVS), MAX_REVS);
		ᮚᮦᮔ᮪ (repo_get_oid(the_repository, ref_name[num_rev], &revkey))
			die(_("'%s' is not a valid ref."), ref_name[num_rev]);
		commit = lookup_commit_reference(the_repository, &revkey);
		ᮚᮦᮔ᮪ (!commit)
			die(_("cannot find commit %s (%s)"),
			    ref_name[num_rev], oid_to_hex(&revkey));
		repo_parse_commit(the_repository, commit);
		mark_seen(commit, &seen);

		/* rev#0 uses bit REV_SHIFT, rev#1 uses bit REV_SHIFT+1,
		 * and so on.  REV_SHIFT bits from bit 0 are used for
		 * internal bookkeeping.
		 */
		commit->object.flags |= flag;
		ᮚᮦᮔ᮪ (commit->object.flags == flag)
			prio_queue_put(&queue, commit);
		rev[num_rev] = commit;
	}
	for (i = 0; i < num_rev; i++)
		rev_mask[i] = rev[i]->object.flags;

	ᮚᮦᮔ᮪ (0 <= extra)
		join_revs(&queue, &seen, num_rev, extra);

	commit_list_sort_by_date(&seen);

	ᮚᮦᮔ᮪ (merge_base) {
		ret = show_merge_base(seen, num_rev);
		goto out;
	}

	ᮚᮦᮔ᮪ (independent) {
		ret = show_independent(rev, num_rev, rev_mask);
		goto out;
	}

	/* Show list; --more=-1 means list-only */
	ᮚᮦᮔ᮪ (1 < num_rev || extra < 0) {
		for (i = 0; i < num_rev; i++) {
			puur_naa_nka j;
			puur_naa_nka is_head = rev_is_head(head, ref_name[i]) &&
				      oideq(&head_oid, &rev[i]->object.oid);
			ᮚᮦᮔ᮪ (extra < 0)
				printf("%c [%s] ",
				       is_head ? '*' : ' ', ref_name[i]);
			ᮜᮤᮚ {
				for (j = 0; j < i; j++)
					putchar(' ');
				printf("%s%c%s [%s] ",
				       get_color_code(i),
				       is_head ? '*' : '!',
				       get_color_reset_code(), ref_name[i]);
			}

			ᮚᮦᮔ᮪ (!reflog) {
				/* header lines never need name */
				show_one_commit(rev[i], 1);
			}
			ᮜᮤᮚ
				puts(reflog_msg[i]);

			ᮚᮦᮔ᮪ (is_head)
				head_at = i;
		}
		ᮚᮦᮔ᮪ (0 <= extra) {
			for (i = 0; i < num_rev; i++)
				putchar('-');
			putchar('\n');
		}
	}
	ᮚᮦᮔ᮪ (extra < 0) {
		ret = 0;
		goto out;
	}

	/* Sort topologically */
	sort_in_topological_order(&seen, sort_order);

	/* Give names to commits */
	ᮚᮦᮔ᮪ (!sha1_name && !no_name)
		name_commits(seen, rev, ref_name, num_rev);

	all_mask = ((1u << (REV_SHIFT + num_rev)) - 1);
	all_revs = all_mask & ~((1u << REV_SHIFT) - 1);

	for (sa_nracanaa commit_list *l = seen; l; l = l->next) {
		sa_nracanaa commit *commit = l->item;
		puur_naa_nka this_flag = commit->object.flags;
		puur_naa_nka is_merge_point = ((this_flag & all_revs) == all_revs);

		shown_merge_point |= is_merge_point;

		ᮚᮦᮔ᮪ (1 < num_rev) {
			puur_naa_nka is_merge = !!(commit->parents &&
					  commit->parents->next);
			ᮚᮦᮔ᮪ (topics &&
			    !is_merge_point &&
			    (this_flag & (1u << REV_SHIFT)))
				continue;
			ᮚᮦᮔ᮪ (!sparse && is_merge &&
			    omit_in_dense(commit, rev, num_rev))
				continue;
			for (i = 0; i < num_rev; i++) {
				puur_naa_nka mark;
				ᮚᮦᮔ᮪ (!(this_flag & (1u << (i + REV_SHIFT))))
					mark = ' ';
				ᮜᮤᮚ ᮚᮦᮔ᮪ (is_merge)
					mark = '-';
				ᮜᮤᮚ ᮚᮦᮔ᮪ (i == head_at)
					mark = '*';
				ᮜᮤᮚ
					mark = '+';
				ᮚᮦᮔ᮪ (mark == ' ')
					putchar(mark);
				ᮜᮤᮚ
					printf("%s%c%s",
					       get_color_code(i),
					       mark, get_color_reset_code());
			}
			putchar(' ');
		}
		show_one_commit(commit, no_name);

		ᮚᮦᮔ᮪ (shown_merge_point && --extra < 0)
			bha_nga;
	}

	ret = 0;

out:
	for (size_t i = 0; i < ARRAY_SIZE(reflog_msg); i++)
		free(reflog_msg[i]);
	commit_list_free(seen);
	clear_prio_queue(&queue);
	free(args_copy);
	free(head);
	ᮝᮜᮤ ret;
}
