#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "hash.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "pretty.h"
#ಅನ್ತರ್ಭಾವ "refs.h"
#ಅನ್ತರ್ಭಾವ "color.h"
#ಅನ್ತರ್ಭಾವ "strvec.h"
#ಅನ್ತರ್ಭಾವ "object-name.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"

#ಅನ್ತರ್ಭಾವ "dir.h"
#ಅನ್ತರ್ಭಾವ "commit-slab.h"
#ಅನ್ತರ್ಭಾವ "date.h"
#ಅನ್ತರ್ಭಾವ "wildmatch.h"
#ಅನ್ತರ್ಭಾವ "prio-queue.h"

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ*ಧ್ರುವ show_branch_usage[] = {
    N_("git show-branch [-a | --all] [-r | --remotes] [--topo-order | --date-order]\n"
       "                [--current] [--color[=<when>] | --no-color] [--sparse]\n"
       "                [--more=<n> | --list | --independent | --merge-base]\n"
       "                [--no-name | --sha1-name] [--topics]\n"
       "                [(<rev> | <glob>)...]"),
    N_("git show-branch (-g | --reflog)[=<n>[,<base>]] [--list] [<ref>]"),
    ರಿಕ್ತ
};

ಸ್ಥಿರ ಗಣನಾ git_colorbool showbranch_use_color = GIT_COLOR_UNKNOWN;

ಸ್ಥಿರ ಸಂರಚನಾ strvec default_args = STRVEC_INIT;

/*
 * TODO: convert this use of commit->object.flags to commit-slab
 * instead to store a pointer to ref name directly. Then use the same
 * UNINTERESTING definition from revision.h here.
 */
#define UNINTERESTING	01

#define REV_SHIFT	 2
#define MAX_REVS	(FLAG_BITS - REV_SHIFT) /* should not exceed bits_per_int - REV_SHIFT */

#define DEFAULT_REFLOG	4

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *get_color_code(ಪೂರ್ಣಾಙ್ಕ idx)
{
	ಯದಿ (want_color(showbranch_use_color))
		ಪ್ರತ್ಯಾಗಮ column_colors_ansi[idx % column_colors_ansi_max];
	ಪ್ರತ್ಯಾಗಮ "";
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *get_color_reset_code(ಶೂನ್ಯ)
{
	ಯದಿ (want_color(showbranch_use_color))
		ಪ್ರತ್ಯಾಗಮ GIT_COLOR_RESET;
	ಪ್ರತ್ಯಾಗಮ "";
}

ಸ್ಥಿರ ಸಂರಚನಾ commit *interesting(ಸಂರಚನಾ prio_queue *queue)
{
	ಸಂರಚನಾ commit *commit;
	prio_queue_for_each(queue, commit) {
		ಯದಿ (!(commit->object.flags & UNINTERESTING))
			ಪ್ರತ್ಯಾಗಮ commit;
	}
	ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

ಸಂರಚನಾ commit_name {
	ಧ್ರುವ ವರ್ಣ *head_name; /* which head's ancestor? */
	ಪೂರ್ಣಾಙ್ಕ generation; /* how many parents away from head_name */
};

define_commit_slab(commit_name_slab, ಸಂರಚನಾ commit_name *);
ಸ್ಥಿರ ಸಂರಚನಾ commit_name_slab name_slab;

ಸ್ಥಿರ ಸಂರಚನಾ commit_name *commit_to_name(ಸಂರಚನಾ commit *commit)
{
	ಪ್ರತ್ಯಾಗಮ *commit_name_slab_at(&name_slab, commit);
}


/* Name the commit as nth generation ancestor of head_name;
 * we count only the first-parent relationship for naming purposes.
 */
ಸ್ಥಿರ ಶೂನ್ಯ name_commit(ಸಂರಚನಾ commit *commit, ಧ್ರುವ ವರ್ಣ *head_name, ಪೂರ್ಣಾಙ್ಕ nth)
{
	ಸಂರಚನಾ commit_name *name;

	name = *commit_name_slab_at(&name_slab, commit);
	ಯದಿ (!name) {
		name = xmalloc(ಪರಿಮಾಣ(*name));
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
ಸ್ಥಿರ ಶೂನ್ಯ name_parent(ಸಂರಚನಾ commit *commit, ಸಂರಚನಾ commit *parent)
{
	ಸಂರಚನಾ commit_name *commit_name = commit_to_name(commit);
	ಸಂರಚನಾ commit_name *parent_name = commit_to_name(parent);
	ಯದಿ (!commit_name)
		ಪ್ರತ್ಯಾಗಮ;
	ಯದಿ (!parent_name ||
	    commit_name->generation + 1 < parent_name->generation)
		name_commit(parent, commit_name->head_name,
			    commit_name->generation + 1);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ name_first_parent_chain(ಸಂರಚನಾ commit *c)
{
	ಪೂರ್ಣಾಙ್ಕ i = 0;
	ಯಾವತ್ (c) {
		ಸಂರಚನಾ commit *p;
		ಯದಿ (!commit_to_name(c))
			ಭಙ್ಗ;
		ಯದಿ (!c->parents)
			ಭಙ್ಗ;
		p = c->parents->item;
		ಯದಿ (!commit_to_name(p)) {
			name_parent(c, p);
			i++;
		}
		ಅನ್ಯಥಾ
			ಭಙ್ಗ;
		c = p;
	}
	ಪ್ರತ್ಯಾಗಮ i;
}

ಸ್ಥಿರ ಶೂನ್ಯ name_commits(ಸಂರಚನಾ commit_list *list,
			 ಸಂರಚನಾ commit **rev,
			 ವರ್ಣ **ref_name,
			 ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_rev)
{
	ಸಂರಚನಾ commit_list *cl;
	ಸಂರಚನಾ commit *c;
	ಪೂರ್ಣಾಙ್ಕ i;

	/* First give names to the given heads */
	ಪ್ರತಿ (cl = list; cl; cl = cl->next) {
		c = cl->item;
		ಯದಿ (commit_to_name(c))
			ಅನುವರ್ತ;
		ಪ್ರತಿ (i = 0; i < ಅಂಕೆ_rev; i++) {
			ಯದಿ (rev[i] == c) {
				name_commit(c, ref_name[i], 0);
				ಭಙ್ಗ;
			}
		}
	}

	/* Then commits on the first parent ancestry chain */
	ಕುರು {
		i = 0;
		ಪ್ರತಿ (cl = list; cl; cl = cl->next) {
			i += name_first_parent_chain(cl->item);
		}
	} ಯಾವತ್ (i);

	/* Finally, any unnamed commits */
	ಕುರು {
		i = 0;
		ಪ್ರತಿ (cl = list; cl; cl = cl->next) {
			ಸಂರಚನಾ commit_list *parents;
			ಸಂರಚನಾ commit_name *n;
			ಪೂರ್ಣಾಙ್ಕ nth;
			c = cl->item;
			ಯದಿ (!commit_to_name(c))
				ಅನುವರ್ತ;
			n = commit_to_name(c);
			parents = c->parents;
			nth = 0;
			ಯಾವತ್ (parents) {
				ಸಂರಚನಾ commit *p = parents->item;
				ಸಂರಚನಾ strbuf newname = STRBUF_INIT;
				parents = parents->next;
				nth++;
				ಯದಿ (commit_to_name(p))
					ಅನುವರ್ತ;
				ವಿಕಲ್ಪ (n->generation) {
				ಪಕ್ಷ 0:
					strbuf_addstr(&newname, n->head_name);
					ಭಙ್ಗ;
				ಪಕ್ಷ 1:
					strbuf_addf(&newname, "%s^", n->head_name);
					ಭಙ್ಗ;
				ಸ್ವಭಾವ:
					strbuf_addf(&newname, "%s~%d",
						    n->head_name, n->generation);
					ಭಙ್ಗ;
				}
				ಯದಿ (nth == 1)
					strbuf_addch(&newname, '^');
				ಅನ್ಯಥಾ
					strbuf_addf(&newname, "^%d", nth);
				name_commit(p, strbuf_detach(&newname, ರಿಕ್ತ), 0);
				i++;
				name_first_parent_chain(p);
			}
		}
	} ಯಾವತ್ (i);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ mark_seen(ಸಂರಚನಾ commit *commit, ಸಂರಚನಾ commit_list **seen_p)
{
	ಯದಿ (!commit->object.flags) {
		commit_list_ಸೇರಿಸು(commit, seen_p);
		ಪ್ರತ್ಯಾಗಮ 1;
	}
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ join_revs(ಸಂರಚನಾ prio_queue *queue,
		      ಸಂರಚನಾ commit_list **seen_p,
		      ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_rev, ಪೂರ್ಣಾಙ್ಕ extra)
{
	ಪೂರ್ಣಾಙ್ಕ all_mask = ((1u << (REV_SHIFT + ಅಂಕೆ_rev)) - 1);
	ಪೂರ್ಣಾಙ್ಕ all_revs = all_mask & ~((1u << REV_SHIFT) - 1);
	ಸಂರಚನಾ commit *commit;

	ಯಾವತ್ ((commit = prio_queue_peek(queue))) {
		ಸಂರಚನಾ commit_list *parents;
		ಪೂರ್ಣಾಙ್ಕ still_interesting = !!interesting(queue);
		ಪೂರ್ಣಾಙ್ಕ flags = commit->object.flags & all_mask;

		ಯದಿ (!still_interesting && extra <= 0)
			ಭಙ್ಗ;

		prio_queue_get(queue);

		mark_seen(commit, seen_p);
		ಯದಿ ((flags & all_revs) == all_revs)
			flags |= UNINTERESTING;
		parents = commit->parents;

		ಯಾವತ್ (parents) {
			ಸಂರಚನಾ commit *p = parents->item;
			ಪೂರ್ಣಾಙ್ಕ this_flag = p->object.flags;
			parents = parents->next;
			ಯದಿ ((this_flag & flags) == flags)
				ಅನುವರ್ತ;
			repo_parse_commit(the_repository, p);
			ಯದಿ (mark_seen(p, seen_p) && !still_interesting)
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
	ಪ್ರತಿ (;;) {
		ಪೂರ್ಣಾಙ್ಕ changed = 0;
		ಸಂರಚನಾ commit_list *s;
		ಪ್ರತಿ (s = *seen_p; s; s = s->next) {
			ಸಂರಚನಾ commit *c = s->item;
			ಸಂರಚನಾ commit_list *parents;

			ಯದಿ (((c->object.flags & all_revs) != all_revs) &&
			    !(c->object.flags & UNINTERESTING))
				ಅನುವರ್ತ;

			/* The current commit is either a merge base or
			 * already uninteresting one.  Mark its parents
			 * as uninteresting commits _only_ if they are
			 * already parsed.  No reason to find new ones
			 * here.
			 */
			parents = c->parents;
			ಯಾವತ್ (parents) {
				ಸಂರಚನಾ commit *p = parents->item;
				parents = parents->next;
				ಯದಿ (!(p->object.flags & UNINTERESTING)) {
					p->object.flags |= UNINTERESTING;
					changed = 1;
				}
			}
		}
		ಯದಿ (!changed)
			ಭಙ್ಗ;
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ show_one_commit(ಸಂರಚನಾ commit *commit, ಪೂರ್ಣಾಙ್ಕ no_name)
{
	ಸಂರಚನಾ strbuf pretty = STRBUF_INIT;
	ಧ್ರುವ ವರ್ಣ *pretty_str = "(unavailable)";
	ಸಂರಚನಾ commit_name *name = commit_to_name(commit);

	ಯದಿ (commit->object.parsed) {
		pp_commit_easy(CMIT_FMT_ONELINE, commit, &pretty);
		pretty_str = pretty.buf;
	}
	skip_prefix(pretty_str, "[PATCH] ", &pretty_str);

	ಯದಿ (!no_name) {
		ಯದಿ (name && name->head_name) {
			printf("[%s", name->head_name);
			ಯದಿ (name->generation) {
				ಯದಿ (name->generation == 1)
					printf("^");
				ಅನ್ಯಥಾ
					printf("~%d", name->generation);
			}
			printf("] ");
		}
		ಅನ್ಯಥಾ
			printf("[%s] ",
			       repo_find_unique_abbrev(the_repository, &commit->object.oid,
						       DEFAULT_ABBREV));
	}
	puts(pretty_str);
	strbuf_release(&pretty);
}

ಸ್ಥಿರ ವರ್ಣ *ref_name[MAX_REVS + 1];
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ ref_name_cnt;

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *find_digit_prefix(ಧ್ರುವ ವರ್ಣ *s, ಪೂರ್ಣಾಙ್ಕ *v)
{
	ಧ್ರುವ ವರ್ಣ *p;
	ಪೂರ್ಣಾಙ್ಕ ver;
	ವರ್ಣ ch;

	ಪ್ರತಿ (p = s, ver = 0;
	     '0' <= (ch = *p) && ch <= '9';
	     p++)
		ver = ver * 10 + ch - '0';
	*v = ver;
	ಪ್ರತ್ಯಾಗಮ p;
}


ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ version_cmp(ಧ್ರುವ ವರ್ಣ *a, ಧ್ರುವ ವರ್ಣ *b)
{
	ಯಾವತ್ (1) {
		ಪೂರ್ಣಾಙ್ಕ va, vb;

		a = find_digit_prefix(a, &va);
		b = find_digit_prefix(b, &vb);
		ಯದಿ (va != vb)
			ಪ್ರತ್ಯಾಗಮ va - vb;

		ಯಾವತ್ (1) {
			ಪೂರ್ಣಾಙ್ಕ ca = *a;
			ಪೂರ್ಣಾಙ್ಕ cb = *b;
			ಯದಿ ('0' <= ca && ca <= '9')
				ca = 0;
			ಯದಿ ('0' <= cb && cb <= '9')
				cb = 0;
			ಯದಿ (ca != cb)
				ಪ್ರತ್ಯಾಗಮ ca - cb;
			ಯದಿ (!ca)
				ಭಙ್ಗ;
			a++;
			b++;
		}
		ಯದಿ (!*a && !*b)
			ಪ್ರತ್ಯಾಗಮ 0;
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ compare_ref_name(ಧ್ರುವ ಶೂನ್ಯ *a_, ಧ್ರುವ ಶೂನ್ಯ *b_)
{
	ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ*a = a_, * ಧ್ರುವ*b = b_;
	ಪ್ರತ್ಯಾಗಮ version_cmp(*a, *b);
}

ಸ್ಥಿರ ಶೂನ್ಯ sort_ref_range(ಪೂರ್ಣಾಙ್ಕ bottom, ಪೂರ್ಣಾಙ್ಕ ತುದಿ)
{
	QSORT(ref_name + bottom, ತುದಿ - bottom, compare_ref_name);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ append_ref(ಧ್ರುವ ವರ್ಣ *refname, ಧ್ರುವ ಸಂರಚನಾ object_id *oid,
		      ಪೂರ್ಣಾಙ್ಕ allow_dups)
{
	ಸಂರಚನಾ commit *commit = lookup_commit_reference_gently(the_repository,
							       oid, 1);
	ಪೂರ್ಣಾಙ್ಕ i;

	ಯದಿ (!commit)
		ಪ್ರತ್ಯಾಗಮ 0;

	ಯದಿ (!allow_dups) {
		/* Avoid adding the same thing twice */
		ಪ್ರತಿ (i = 0; i < ref_name_cnt; i++)
			ಯದಿ (!strcmp(refname, ref_name[i]))
				ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (MAX_REVS <= ref_name_cnt) {
		warning(Q_("ignoring %s; cannot handle more than %d ref",
			   "ignoring %s; cannot handle more than %d refs",
			   MAX_REVS), refname, MAX_REVS);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ref_name[ref_name_cnt++] = xstrdup(refname);
	ref_name[ref_name_cnt] = ರಿಕ್ತ;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ append_head_ref(ಧ್ರುವ ಸಂರಚನಾ reference *ref, ಶೂನ್ಯ *cb_data UNUSED)
{
	ಸಂರಚನಾ object_id tmp;
	ಪೂರ್ಣಾಙ್ಕ ofs = 11;
	ಯದಿ (!starts_with(ref->name, "refs/heads/"))
		ಪ್ರತ್ಯಾಗಮ 0;
	/* If both heads/foo and tags/foo exists, get_sha1 would
	 * get confused.
	 */
	ಯದಿ (repo_get_oid(the_repository, ref->name + ofs, &tmp) || !oideq(&tmp, ref->oid))
		ofs = 5;
	ಪ್ರತ್ಯಾಗಮ append_ref(ref->name + ofs, ref->oid, 0);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ append_remote_ref(ಧ್ರುವ ಸಂರಚನಾ reference *ref, ಶೂನ್ಯ *cb_data UNUSED)
{
	ಸಂರಚನಾ object_id tmp;
	ಪೂರ್ಣಾಙ್ಕ ofs = 13;
	ಯದಿ (!starts_with(ref->name, "refs/remotes/"))
		ಪ್ರತ್ಯಾಗಮ 0;
	/* If both heads/foo and tags/foo exists, get_sha1 would
	 * get confused.
	 */
	ಯದಿ (repo_get_oid(the_repository, ref->name + ofs, &tmp) || !oideq(&tmp, ref->oid))
		ofs = 5;
	ಪ್ರತ್ಯಾಗಮ append_ref(ref->name + ofs, ref->oid, 0);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ append_tag_ref(ಧ್ರುವ ವರ್ಣ *refname, ಧ್ರುವ ಸಂರಚನಾ object_id *oid,
			  ಪೂರ್ಣಾಙ್ಕ flag UNUSED, ಶೂನ್ಯ *cb_data UNUSED)
{
	ಯದಿ (!starts_with(refname, "refs/tags/"))
		ಪ್ರತ್ಯಾಗಮ 0;
	ಪ್ರತ್ಯಾಗಮ append_ref(refname + 5, oid, 0);
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *match_ref_pattern = ರಿಕ್ತ;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ match_ref_slash = 0;

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ append_matching_ref(ಧ್ರುವ ಸಂರಚನಾ reference *ref, ಶೂನ್ಯ *cb_data)
{
	/* we want to allow pattern hold/<asterisk> to show all
	 * branches under refs/heads/hold/, and v0.99.9? to show
	 * refs/tags/v0.99.9a and friends.
	 */
	ಧ್ರುವ ವರ್ಣ *tail;
	ಪೂರ್ಣಾಙ್ಕ slash = ಎಣಿಕೆ_slashes(ref->name);
	ಪ್ರತಿ (tail = ref->name; *tail && match_ref_slash < slash; )
		ಯದಿ (*tail++ == '/')
			slash--;
	ಯದಿ (!*tail)
		ಪ್ರತ್ಯಾಗಮ 0;
	ಯದಿ (wildmatch(match_ref_pattern, tail, 0))
		ಪ್ರತ್ಯಾಗಮ 0;
	ಯದಿ (starts_with(ref->name, "refs/heads/"))
		ಪ್ರತ್ಯಾಗಮ append_head_ref(ref, cb_data);
	ಯದಿ (starts_with(ref->name, "refs/tags/"))
		ಪ್ರತ್ಯಾಗಮ append_tag_ref(ref->name, ref->oid, ref->flags, cb_data);
	ಪ್ರತ್ಯಾಗಮ append_ref(ref->name, ref->oid, 0);
}

ಸ್ಥಿರ ಶೂನ್ಯ snarf_refs(ಪೂರ್ಣಾಙ್ಕ head, ಪೂರ್ಣಾಙ್ಕ remotes)
{
	ಯದಿ (head) {
		ಪೂರ್ಣಾಙ್ಕ orig_cnt = ref_name_cnt;

		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_head_ref, ರಿಕ್ತ);
		sort_ref_range(orig_cnt, ref_name_cnt);
	}
	ಯದಿ (remotes) {
		ಪೂರ್ಣಾಙ್ಕ orig_cnt = ref_name_cnt;

		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_remote_ref, ರಿಕ್ತ);
		sort_ref_range(orig_cnt, ref_name_cnt);
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ rev_is_head(ಧ್ರುವ ವರ್ಣ *head, ಧ್ರುವ ವರ್ಣ *name)
{
	ಯದಿ (!head)
		ಪ್ರತ್ಯಾಗಮ 0;
	skip_prefix(head, "refs/heads/", &head);
	ಯದಿ (!skip_prefix(name, "refs/heads/", &name))
		skip_prefix(name, "heads/", &name);
	ಪ್ರತ್ಯಾಗಮ !strcmp(head, name);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_merge_base(ಧ್ರುವ ಸಂರಚನಾ commit_list *seen, ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_rev)
{
	ಪೂರ್ಣಾಙ್ಕ all_mask = ((1u << (REV_SHIFT + ಅಂಕೆ_rev)) - 1);
	ಪೂರ್ಣಾಙ್ಕ all_revs = all_mask & ~((1u << REV_SHIFT) - 1);
	ಪೂರ್ಣಾಙ್ಕ exit_status = 1;

	ಪ್ರತಿ (ಧ್ರುವ ಸಂರಚನಾ commit_list *s = seen; s; s = s->next) {
		ಸಂರಚನಾ commit *commit = s->item;
		ಪೂರ್ಣಾಙ್ಕ flags = commit->object.flags & all_mask;
		ಯದಿ (!(flags & UNINTERESTING) &&
		    ((flags & all_revs) == all_revs)) {
			puts(oid_to_hex(&commit->object.oid));
			exit_status = 0;
			commit->object.flags |= UNINTERESTING;
		}
	}
	ಪ್ರತ್ಯಾಗಮ exit_status;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_independent(ಸಂರಚನಾ commit **rev,
			    ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_rev,
			    unsigned ಪೂರ್ಣಾಙ್ಕ *rev_mask)
{
	ಪೂರ್ಣಾಙ್ಕ i;

	ಪ್ರತಿ (i = 0; i < ಅಂಕೆ_rev; i++) {
		ಸಂರಚನಾ commit *commit = rev[i];
		unsigned ಪೂರ್ಣಾಙ್ಕ flag = rev_mask[i];

		ಯದಿ (commit->object.flags == flag)
			puts(oid_to_hex(&commit->object.oid));
		commit->object.flags |= UNINTERESTING;
	}
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ append_one_rev(ಧ್ರುವ ವರ್ಣ *av)
{
	ಸಂರಚನಾ object_id revkey;
	ಯದಿ (!repo_get_oid(the_repository, av, &revkey)) {
		append_ref(av, &revkey, 0);
		ಪ್ರತ್ಯಾಗಮ;
	}
	ಯದಿ (strpbrk(av, "*?[")) {
		/* glob style match */
		ಪೂರ್ಣಾಙ್ಕ saved_matches = ref_name_cnt;

		match_ref_pattern = av;
		match_ref_slash = ಎಣಿಕೆ_slashes(av);
		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_matching_ref, ರಿಕ್ತ);
		ಯದಿ (saved_matches == ref_name_cnt &&
		    ref_name_cnt < MAX_REVS)
			error(_("no matching refs with %s"), av);
		sort_ref_range(saved_matches, ref_name_cnt);
		ಪ್ರತ್ಯಾಗಮ;
	}
	die("bad sha1 reference %s", av);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ git_show_branch_config(ಧ್ರುವ ವರ್ಣ *var, ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ,
				  ಧ್ರುವ ಸಂರಚನಾ config_context *ctx, ಶೂನ್ಯ *cb)
{
	ಯದಿ (!strcmp(var, "showbranch.default")) {
		ಯದಿ (!ಮೌಲ್ಯ)
			ಪ್ರತ್ಯಾಗಮ config_error_nonbool(var);
		/*
		 * default_arg is now passed to parse_options(), so we need to
		 * mimic the real argv a bit better.
		 */
		ಯದಿ (!default_args.nr)
			strvec_ತಳ್ಳು(&default_args, "show-branch");
		strvec_ತಳ್ಳು(&default_args, ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (!strcmp(var, "color.showbranch")) {
		showbranch_use_color = git_config_colorbool(var, ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (git_color_config(var, ಮೌಲ್ಯ, cb) < 0)
		ಪ್ರತ್ಯಾಗಮ -1;

	ಪ್ರತ್ಯಾಗಮ git_default_config(var, ಮೌಲ್ಯ, ctx, cb);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ omit_in_dense(ಸಂರಚನಾ commit *commit, ಸಂರಚನಾ commit **rev, ಪೂರ್ಣಾಙ್ಕ n)
{
	/* If the commit is tip of the named branches, do not
	 * omit it.
	 * Otherwise, if it is a merge that is reachable from only one
	 * tip, it is not that interesting.
	 */
	ಪೂರ್ಣಾಙ್ಕ i, flag, ಎಣಿಕೆ;
	ಪ್ರತಿ (i = 0; i < n; i++)
		ಯದಿ (rev[i] == commit)
			ಪ್ರತ್ಯಾಗಮ 0;
	flag = commit->object.flags;
	ಪ್ರತಿ (i = ಎಣಿಕೆ = 0; i < n; i++) {
		ಯದಿ (flag & (1u << (i + REV_SHIFT)))
			ಎಣಿಕೆ++;
	}
	ಯದಿ (ಎಣಿಕೆ == 1)
		ಪ್ರತ್ಯಾಗಮ 1;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ reflog = 0;

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ parse_reflog_param(ಧ್ರುವ ಸಂರಚನಾ option *opt, ಧ್ರುವ ವರ್ಣ *arg,
			      ಪೂರ್ಣಾಙ್ಕ unset)
{
	ವರ್ಣ *ep;
	ಧ್ರುವ ವರ್ಣ **base = (ಧ್ರುವ ವರ್ಣ **)opt->ಮೌಲ್ಯ;
	BUG_ON_OPT_NEG(unset);
	ಯದಿ (!arg)
		arg = "";
	reflog = strtoul(arg, &ep, 10);
	ಯದಿ (*ep == ',')
		*base = ep + 1;
	ಅನ್ಯಥಾ ಯದಿ (*ep)
		ಪ್ರತ್ಯಾಗಮ error("unrecognized reflog param '%s'", arg);
	ಅನ್ಯಥಾ
		*base = ರಿಕ್ತ;
	ಯದಿ (reflog <= 0)
		reflog = DEFAULT_REFLOG;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ cmd_show_branch(ಪೂರ್ಣಾಙ್ಕ ac,
		ಧ್ರುವ ವರ್ಣ **av,
		ಧ್ರುವ ವರ್ಣ *prefix,
		ಸಂರಚನಾ repository *repo UNUSED)
{
	ಸಂರಚನಾ commit *rev[MAX_REVS], *commit;
	ವರ್ಣ *reflog_ಸಂದೇಶ[MAX_REVS] = {0};
	ಸಂರಚನಾ commit_list *seen = ರಿಕ್ತ;
	ಸಂರಚನಾ prio_queue queue = { compare_commits_by_commit_date };
	unsigned ಪೂರ್ಣಾಙ್ಕ rev_mask[MAX_REVS];
	ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_rev, i, extra = 0;
	ಪೂರ್ಣಾಙ್ಕ all_heads = 0, all_remotes = 0;
	ಪೂರ್ಣಾಙ್ಕ all_mask, all_revs;
	ಗಣನಾ rev_sort_order sort_order = REV_SORT_IN_GRAPH_ORDER;
	ವರ್ಣ *head;
	ಸಂರಚನಾ object_id head_oid;
	ಪೂರ್ಣಾಙ್ಕ merge_base = 0;
	ಪೂರ್ಣಾಙ್ಕ independent = 0;
	ಪೂರ್ಣಾಙ್ಕ no_name = 0;
	ಪೂರ್ಣಾಙ್ಕ sha1_name = 0;
	ಪೂರ್ಣಾಙ್ಕ shown_merge_point = 0;
	ಪೂರ್ಣಾಙ್ಕ with_current_branch = 0;
	ಪೂರ್ಣಾಙ್ಕ head_at = -1;
	ಪೂರ್ಣಾಙ್ಕ topics = 0;
	ಪೂರ್ಣಾಙ್ಕ sparse = 0;
	ಧ್ರುವ ವರ್ಣ *reflog_base = ರಿಕ್ತ;
	ಸಂರಚನಾ option builtin_show_branch_options[] = {
		OPT_BOOL('a', "all", &all_heads,
			 N_("show remote-tracking and local branches")),
		OPT_BOOL('r', "remotes", &all_remotes,
			 N_("show remote-tracking branches")),
		OPT__COLOR(&showbranch_use_color,
			    N_("color '*!+-' corresponding to the branch")),
		{
			.type = OPTION_INTEGER,
			.long_name = "more",
			.ಮೌಲ್ಯ = &extra,
			.precision = ಪರಿಮಾಣ(extra),
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
	ಧ್ರುವ ವರ್ಣ **args_copy = ರಿಕ್ತ;
	ಪೂರ್ಣಾಙ್ಕ ret;

	init_commit_name_slab(&name_slab);

	repo_config(the_repository, git_show_branch_config, ರಿಕ್ತ);

	/* If nothing is specified, try the default first */
	ಯದಿ (ac == 1 && default_args.nr) {
		DUP_ARRAY(args_copy, default_args.v, default_args.nr);
		ac = default_args.nr;
		av = args_copy;
	}

	ac = parse_options(ac, av, prefix, builtin_show_branch_options,
			   show_branch_usage, PARSE_OPT_STOP_AT_NON_OPTION);
	ಯದಿ (all_heads)
		all_remotes = 1;

	ಯದಿ (extra || reflog) {
		/* "listing" mode is incompatible with
		 * independent nor merge-base modes.
		 */
		ಯದಿ (independent || merge_base)
			usage_with_options(show_branch_usage,
					   builtin_show_branch_options);
		ಯದಿ (reflog && ((0 < extra) || all_heads || all_remotes))
			/*
			 * Asking for --more in reflog mode does not
			 * make sense.  --list is Ok.
			 *
			 * Also --all and --remotes do not make sense either.
			 */
			die(_("options '%s' and '%s' cannot be used together"), "--reflog",
				"--all/--remotes/--independent/--merge-base");
	}

	ಯದಿ (with_current_branch && reflog)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--reflog", "--current");

	/* If nothing is specified, show all branches by default */
	ಯದಿ (ac <= topics && all_heads + all_remotes == 0)
		all_heads = 1;

	ಯದಿ (reflog) {
		ಸಂರಚನಾ object_id oid;
		ವರ್ಣ *ref;
		ಪೂರ್ಣಾಙ್ಕ base = 0;
		unsigned ಪೂರ್ಣಾಙ್ಕ flags = 0;

		ಯದಿ (ac == 0) {
			ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *fake_av[2];

			fake_av[0] = refs_resolve_refdup(get_main_ref_store(the_repository),
							 "HEAD",
							 RESOLVE_REF_READING,
							 &oid,
							 ರಿಕ್ತ);
			fake_av[1] = ರಿಕ್ತ;
			av = fake_av;
			ac = 1;
			ಯದಿ (!*av)
				die(_("no branches given, and HEAD is not valid"));
		}
		ಯದಿ (ac != 1)
			die(_("--reflog option needs one branch name"));

		ಯದಿ (MAX_REVS < reflog)
			die(Q_("only %d entry can be shown at one time.",
			       "only %d entries can be shown at one time.",
			       MAX_REVS), MAX_REVS);
		ಯದಿ (!repo_dwim_ref(the_repository, *av, strlen(*av), &oid,
				   &ref, 0))
			die(_("no such ref %s"), *av);

		/* Has the base been specified? */
		ಯದಿ (reflog_base) {
			ವರ್ಣ *ep;
			base = strtoul(reflog_base, &ep, 10);
			ಯದಿ (*ep) {
				/* Ah, that is a date spec... */
				timestamp_t at;
				at = approxidate(reflog_base);
				read_ref_at(get_main_ref_store(the_repository),
					    ref, flags, at, -1, &oid, ರಿಕ್ತ,
					    ರಿಕ್ತ, ರಿಕ್ತ, &base);
			}
		}

		ಪ್ರತಿ (i = 0; i < reflog; i++) {
			ವರ್ಣ *logmsg = ರಿಕ್ತ;
			ವರ್ಣ *nth_desc;
			ಧ್ರುವ ವರ್ಣ *ಸಂದೇಶ;
			ವರ್ಣ *end;
			timestamp_t timestamp;
			ಪೂರ್ಣಾಙ್ಕ tz;

			ಯದಿ (read_ref_at(get_main_ref_store(the_repository),
					ref, flags, 0, base + i, &oid, &logmsg,
					&timestamp, &tz, ರಿಕ್ತ)) {
				ಮುಕ್ತ(logmsg);
				reflog = i;
				ಭಙ್ಗ;
			}

			end = strchr(logmsg, '\n');
			ಯದಿ (end)
				*end = '\0';

			ಸಂದೇಶ = (*logmsg == '\0') ? "(none)" : logmsg;
			reflog_ಸಂದೇಶ[i] = xstrfmt("(%s) %s",
						show_date(timestamp, tz,
							  DATE_MODE(RELATIVE)),
						ಸಂದೇಶ);
			ಮುಕ್ತ(logmsg);

			nth_desc = xstrfmt("%s@{%d}", *av, base+i);
			append_ref(nth_desc, &oid, 1);
			ಮುಕ್ತ(nth_desc);
		}
		ಮುಕ್ತ(ref);
	}
	ಅನ್ಯಥಾ {
		ಯಾವತ್ (0 < ac) {
			append_one_rev(*av);
			ac--; av++;
		}
		ಯದಿ (all_heads + all_remotes)
			snarf_refs(all_heads, all_remotes);
	}

	head = refs_resolve_refdup(get_main_ref_store(the_repository), "HEAD",
				   RESOLVE_REF_READING,
				   &head_oid, ರಿಕ್ತ);

	ಯದಿ (with_current_branch && head) {
		ಪೂರ್ಣಾಙ್ಕ has_head = 0;
		ಪ್ರತಿ (i = 0; !has_head && i < ref_name_cnt; i++) {
			/* We are only interested in adding the branch
			 * HEAD points at.
			 */
			ಯದಿ (rev_is_head(head, ref_name[i]))
				has_head++;
		}
		ಯದಿ (!has_head) {
			ಧ್ರುವ ವರ್ಣ *name = head;
			skip_prefix(name, "refs/heads/", &name);
			append_one_rev(name);
		}
	}

	ಯದಿ (!ref_name_cnt) {
		fprintf(stderr, "No revs to be shown.\n");
		ret = 0;
		ಗಚ್ಛ out;
	}

	ಪ್ರತಿ (ಅಂಕೆ_rev = 0; ref_name[ಅಂಕೆ_rev]; ಅಂಕೆ_rev++) {
		ಸಂರಚನಾ object_id revkey;
		unsigned ಪೂರ್ಣಾಙ್ಕ flag = 1u << (ಅಂಕೆ_rev + REV_SHIFT);

		ಯದಿ (MAX_REVS <= ಅಂಕೆ_rev)
			die(Q_("cannot handle more than %d rev.",
			       "cannot handle more than %d revs.",
			       MAX_REVS), MAX_REVS);
		ಯದಿ (repo_get_oid(the_repository, ref_name[ಅಂಕೆ_rev], &revkey))
			die(_("'%s' is not a valid ref."), ref_name[ಅಂಕೆ_rev]);
		commit = lookup_commit_reference(the_repository, &revkey);
		ಯದಿ (!commit)
			die(_("cannot find commit %s (%s)"),
			    ref_name[ಅಂಕೆ_rev], oid_to_hex(&revkey));
		repo_parse_commit(the_repository, commit);
		mark_seen(commit, &seen);

		/* rev#0 uses bit REV_SHIFT, rev#1 uses bit REV_SHIFT+1,
		 * and so on.  REV_SHIFT bits from bit 0 are used for
		 * internal bookkeeping.
		 */
		commit->object.flags |= flag;
		ಯದಿ (commit->object.flags == flag)
			prio_queue_put(&queue, commit);
		rev[ಅಂಕೆ_rev] = commit;
	}
	ಪ್ರತಿ (i = 0; i < ಅಂಕೆ_rev; i++)
		rev_mask[i] = rev[i]->object.flags;

	ಯದಿ (0 <= extra)
		join_revs(&queue, &seen, ಅಂಕೆ_rev, extra);

	commit_list_sort_by_date(&seen);

	ಯದಿ (merge_base) {
		ret = show_merge_base(seen, ಅಂಕೆ_rev);
		ಗಚ್ಛ out;
	}

	ಯದಿ (independent) {
		ret = show_independent(rev, ಅಂಕೆ_rev, rev_mask);
		ಗಚ್ಛ out;
	}

	/* Show list; --more=-1 means list-only */
	ಯದಿ (1 < ಅಂಕೆ_rev || extra < 0) {
		ಪ್ರತಿ (i = 0; i < ಅಂಕೆ_rev; i++) {
			ಪೂರ್ಣಾಙ್ಕ j;
			ಪೂರ್ಣಾಙ್ಕ is_head = rev_is_head(head, ref_name[i]) &&
				      oideq(&head_oid, &rev[i]->object.oid);
			ಯದಿ (extra < 0)
				printf("%c [%s] ",
				       is_head ? '*' : ' ', ref_name[i]);
			ಅನ್ಯಥಾ {
				ಪ್ರತಿ (j = 0; j < i; j++)
					putchar(' ');
				printf("%s%c%s [%s] ",
				       get_color_code(i),
				       is_head ? '*' : '!',
				       get_color_reset_code(), ref_name[i]);
			}

			ಯದಿ (!reflog) {
				/* header lines never need name */
				show_one_commit(rev[i], 1);
			}
			ಅನ್ಯಥಾ
				puts(reflog_ಸಂದೇಶ[i]);

			ಯದಿ (is_head)
				head_at = i;
		}
		ಯದಿ (0 <= extra) {
			ಪ್ರತಿ (i = 0; i < ಅಂಕೆ_rev; i++)
				putchar('-');
			putchar('\n');
		}
	}
	ಯದಿ (extra < 0) {
		ret = 0;
		ಗಚ್ಛ out;
	}

	/* Sort topologically */
	sort_in_topological_order(&seen, sort_order);

	/* Give names to commits */
	ಯದಿ (!sha1_name && !no_name)
		name_commits(seen, rev, ref_name, ಅಂಕೆ_rev);

	all_mask = ((1u << (REV_SHIFT + ಅಂಕೆ_rev)) - 1);
	all_revs = all_mask & ~((1u << REV_SHIFT) - 1);

	ಪ್ರತಿ (ಸಂರಚನಾ commit_list *l = seen; l; l = l->next) {
		ಸಂರಚನಾ commit *commit = l->item;
		ಪೂರ್ಣಾಙ್ಕ this_flag = commit->object.flags;
		ಪೂರ್ಣಾಙ್ಕ is_merge_point = ((this_flag & all_revs) == all_revs);

		shown_merge_point |= is_merge_point;

		ಯದಿ (1 < ಅಂಕೆ_rev) {
			ಪೂರ್ಣಾಙ್ಕ is_merge = !!(commit->parents &&
					  commit->parents->next);
			ಯದಿ (topics &&
			    !is_merge_point &&
			    (this_flag & (1u << REV_SHIFT)))
				ಅನುವರ್ತ;
			ಯದಿ (!sparse && is_merge &&
			    omit_in_dense(commit, rev, ಅಂಕೆ_rev))
				ಅನುವರ್ತ;
			ಪ್ರತಿ (i = 0; i < ಅಂಕೆ_rev; i++) {
				ಪೂರ್ಣಾಙ್ಕ mark;
				ಯದಿ (!(this_flag & (1u << (i + REV_SHIFT))))
					mark = ' ';
				ಅನ್ಯಥಾ ಯದಿ (is_merge)
					mark = '-';
				ಅನ್ಯಥಾ ಯದಿ (i == head_at)
					mark = '*';
				ಅನ್ಯಥಾ
					mark = '+';
				ಯದಿ (mark == ' ')
					putchar(mark);
				ಅನ್ಯಥಾ
					printf("%s%c%s",
					       get_color_code(i),
					       mark, get_color_reset_code());
			}
			putchar(' ');
		}
		show_one_commit(commit, no_name);

		ಯದಿ (shown_merge_point && --extra < 0)
			ಭಙ್ಗ;
	}

	ret = 0;

out:
	ಪ್ರತಿ (size_t i = 0; i < ARRAY_SIZE(reflog_ಸಂದೇಶ); i++)
		ಮುಕ್ತ(reflog_ಸಂದೇಶ[i]);
	commit_list_free(seen);
	clear_prio_queue(&queue);
	ಮುಕ್ತ(args_copy);
	ಮುಕ್ತ(head);
	ಪ್ರತ್ಯಾಗಮ ret;
}
