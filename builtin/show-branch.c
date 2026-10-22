#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE
#ವ್ಯಾಖ್ಯಾನಿಸು DISABLE_SIGN_COMPARE_WARNINGS

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

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*ಸ್ಥಿರಾಂಕ show_branch_usage[] = {
    N_("git show-branch [-a | --all] [-r | --remotes] [--topo-order | --date-order]\n"
       "                [--current] [--color[=<when>] | --no-color] [--sparse]\n"
       "                [--more=<n> | --list | --independent | --merge-base]\n"
       "                [--no-name | --sha1-name] [--topics]\n"
       "                [(<rev> | <glob>)...]"),
    N_("git show-branch (-g | --reflog)[=<n>[,<base>]] [--list] [<ref>]"),
    ಖಾಲಿ
};

ಸ್ಥಿರ ಎಣಿಕೆ git_colorbool showbranch_use_color = GIT_COLOR_UNKNOWN;

ಸ್ಥಿರ ರಚನೆ strvec default_ವಾದಗಳು = STRVEC_INIT;

/*
 * TODO: convert this use of commit->object.flags to commit-slab
 * instead to store a pointer to ref name directly. Then use the same
 * UNINTERESTING definition from revision.h here.
 */
#ವ್ಯಾಖ್ಯಾನಿಸು UNINTERESTING	01

#ವ್ಯಾಖ್ಯಾನಿಸು REV_ಸರಿಸು	 2
#ವ್ಯಾಖ್ಯಾನಿಸು MAX_REVS	(FLAG_ಬಿಟ್ಟುಗಳು - REV_ಸರಿಸು) /* should not exceed bits_per_int - REV_SHIFT */

#ವ್ಯಾಖ್ಯಾನಿಸು DEFAULT_REFLOG	4

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *get_color_ಸಂಕೇತ(ಪೂರ್ಣಾಂಕ idx)
{
	ಆದರೆ (want_color(showbranch_use_color))
		ಹಿಂತಿರುಗಿ ಸ್ತಂಭ_colors_ansi[idx % ಸ್ತಂಭ_colors_ansi_max];
	ಹಿಂತಿರುಗಿ "";
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *get_color_reset_ಸಂಕೇತ(ಶೂನ್ಯ)
{
	ಆದರೆ (want_color(showbranch_use_color))
		ಹಿಂತಿರುಗಿ GIT_COLOR_RESET;
	ಹಿಂತಿರುಗಿ "";
}

ಸ್ಥಿರ ರಚನೆ commit *interesting(ರಚನೆ prio_ಸರತಿ *ಸರತಿ)
{
	ರಚನೆ commit *commit;
	prio_ಸರತಿ_for_each(ಸರತಿ, commit) {
		ಆದರೆ (!(commit->object.flags & UNINTERESTING))
			ಹಿಂತಿರುಗಿ commit;
	}
	ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

ರಚನೆ commit_name {
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ತಲೆ_name; /* which head's ancestor? */
	ಪೂರ್ಣಾಂಕ generation; /* how many parents away from head_name */
};

define_commit_slab(commit_name_slab, ರಚನೆ commit_name *);
ಸ್ಥಿರ ರಚನೆ commit_name_slab name_slab;

ಸ್ಥಿರ ರಚನೆ commit_name *commit_ಗಮ್ಯ_name(ರಚನೆ commit *commit)
{
	ಹಿಂತಿರುಗಿ *commit_name_slab_at(&name_slab, commit);
}


/* Name the commit as nth generation ancestor of head_name;
 * we count only the first-parent relationship for naming purposes.
 */
ಸ್ಥಿರ ಶೂನ್ಯ name_commit(ರಚನೆ commit *commit, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ತಲೆ_name, ಪೂರ್ಣಾಂಕ nth)
{
	ರಚನೆ commit_name *name;

	name = *commit_name_slab_at(&name_slab, commit);
	ಆದರೆ (!name) {
		name = xmalloc(ಗಾತ್ರ(*name));
		*commit_name_slab_at(&name_slab, commit) = name;
	}
	name->ತಲೆ_name = ತಲೆ_name;
	name->generation = nth;
}

/* Parent is the first parent of the commit.  We may name it
 * as (n+1)th generation ancestor of the same head_name as
 * commit is nth generation ancestor of, if that generation
 * number is better than the name it already has.
 */
ಸ್ಥಿರ ಶೂನ್ಯ name_ಪೋಷಕ(ರಚನೆ commit *commit, ರಚನೆ commit *ಪೋಷಕ)
{
	ರಚನೆ commit_name *commit_name = commit_ಗಮ್ಯ_name(commit);
	ರಚನೆ commit_name *ಪೋಷಕ_name = commit_ಗಮ್ಯ_name(ಪೋಷಕ);
	ಆದರೆ (!commit_name)
		ಹಿಂತಿರುಗಿ;
	ಆದರೆ (!ಪೋಷಕ_name ||
	    commit_name->generation + 1 < ಪೋಷಕ_name->generation)
		name_commit(ಪೋಷಕ, commit_name->ತಲೆ_name,
			    commit_name->generation + 1);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ name_ಮೊದಲ_ಪೋಷಕ_chain(ರಚನೆ commit *c)
{
	ಪೂರ್ಣಾಂಕ i = 0;
	ಆಗ (c) {
		ರಚನೆ commit *p;
		ಆದರೆ (!commit_ಗಮ್ಯ_name(c))
			ನಿಲ್ಲಿಸು;
		ಆದರೆ (!c->parents)
			ನಿಲ್ಲಿಸು;
		p = c->parents->ಅಂಶ;
		ಆದರೆ (!commit_ಗಮ್ಯ_name(p)) {
			name_ಪೋಷಕ(c, p);
			i++;
		}
		ಇಲ್ಲದಿದ್ದರೆ
			ನಿಲ್ಲಿಸು;
		c = p;
	}
	ಹಿಂತಿರುಗಿ i;
}

ಸ್ಥಿರ ಶೂನ್ಯ name_commits(ರಚನೆ commit_list *list,
			 ರಚನೆ commit **rev,
			 ಅಕ್ಷರ **ref_name,
			 ಪೂರ್ಣಾಂಕ ಅಂಕೆ_rev)
{
	ರಚನೆ commit_list *cl;
	ರಚನೆ commit *c;
	ಪೂರ್ಣಾಂಕ i;

	/* First give names to the given heads */
	ಗಾಗಿ (cl = list; cl; cl = cl->ಮುಂದಿನ) {
		c = cl->ಅಂಶ;
		ಆದರೆ (commit_ಗಮ್ಯ_name(c))
			ಮುಂದುವರಿ;
		ಗಾಗಿ (i = 0; i < ಅಂಕೆ_rev; i++) {
			ಆದರೆ (rev[i] == c) {
				name_commit(c, ref_name[i], 0);
				ನಿಲ್ಲಿಸು;
			}
		}
	}

	/* Then commits on the first parent ancestry chain */
	ಮಾಡು {
		i = 0;
		ಗಾಗಿ (cl = list; cl; cl = cl->ಮುಂದಿನ) {
			i += name_ಮೊದಲ_ಪೋಷಕ_chain(cl->ಅಂಶ);
		}
	} ಆಗ (i);

	/* Finally, any unnamed commits */
	ಮಾಡು {
		i = 0;
		ಗಾಗಿ (cl = list; cl; cl = cl->ಮುಂದಿನ) {
			ರಚನೆ commit_list *parents;
			ರಚನೆ commit_name *n;
			ಪೂರ್ಣಾಂಕ nth;
			c = cl->ಅಂಶ;
			ಆದರೆ (!commit_ಗಮ್ಯ_name(c))
				ಮುಂದುವರಿ;
			n = commit_ಗಮ್ಯ_name(c);
			parents = c->parents;
			nth = 0;
			ಆಗ (parents) {
				ರಚನೆ commit *p = parents->ಅಂಶ;
				ರಚನೆ strbuf newname = STRBUF_INIT;
				parents = parents->ಮುಂದಿನ;
				nth++;
				ಆದರೆ (commit_ಗಮ್ಯ_name(p))
					ಮುಂದುವರಿ;
				ಆಯ್ಕೆ (n->generation) {
				ಸ್ಥಿತಿ 0:
					strbuf_addstr(&newname, n->ತಲೆ_name);
					ನಿಲ್ಲಿಸು;
				ಸ್ಥಿತಿ 1:
					strbuf_addf(&newname, "%s^", n->ತಲೆ_name);
					ನಿಲ್ಲಿಸು;
				ಪೂರ್ವನಿಯೋಜಿತ:
					strbuf_addf(&newname, "%s~%d",
						    n->ತಲೆ_name, n->generation);
					ನಿಲ್ಲಿಸು;
				}
				ಆದರೆ (nth == 1)
					strbuf_addch(&newname, '^');
				ಇಲ್ಲದಿದ್ದರೆ
					strbuf_addf(&newname, "^%d", nth);
				name_commit(p, strbuf_detach(&newname, ಖಾಲಿ), 0);
				i++;
				name_ಮೊದಲ_ಪೋಷಕ_chain(p);
			}
		}
	} ಆಗ (i);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ mark_ನೋಡಿದ(ರಚನೆ commit *commit, ರಚನೆ commit_list **ನೋಡಿದ_p)
{
	ಆದರೆ (!commit->object.flags) {
		commit_list_ಸೇರಿಸು(commit, ನೋಡಿದ_p);
		ಹಿಂತಿರುಗಿ 1;
	}
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ join_revs(ರಚನೆ prio_ಸರತಿ *ಸರತಿ,
		      ರಚನೆ commit_list **ನೋಡಿದ_p,
		      ಪೂರ್ಣಾಂಕ ಅಂಕೆ_rev, ಪೂರ್ಣಾಂಕ extra)
{
	ಪೂರ್ಣಾಂಕ all_ಮುಖವಾಡ = ((1u << (REV_ಸರಿಸು + ಅಂಕೆ_rev)) - 1);
	ಪೂರ್ಣಾಂಕ all_revs = all_ಮುಖವಾಡ & ~((1u << REV_ಸರಿಸು) - 1);
	ರಚನೆ commit *commit;

	ಆಗ ((commit = prio_ಸರತಿ_peek(ಸರತಿ))) {
		ರಚನೆ commit_list *parents;
		ಪೂರ್ಣಾಂಕ still_interesting = !!interesting(ಸರತಿ);
		ಪೂರ್ಣಾಂಕ flags = commit->object.flags & all_ಮುಖವಾಡ;

		ಆದರೆ (!still_interesting && extra <= 0)
			ನಿಲ್ಲಿಸು;

		prio_ಸರತಿ_get(ಸರತಿ);

		mark_ನೋಡಿದ(commit, ನೋಡಿದ_p);
		ಆದರೆ ((flags & all_revs) == all_revs)
			flags |= UNINTERESTING;
		parents = commit->parents;

		ಆಗ (parents) {
			ರಚನೆ commit *p = parents->ಅಂಶ;
			ಪೂರ್ಣಾಂಕ this_flag = p->object.flags;
			parents = parents->ಮುಂದಿನ;
			ಆದರೆ ((this_flag & flags) == flags)
				ಮುಂದುವರಿ;
			repo_parse_commit(the_repository, p);
			ಆದರೆ (mark_ನೋಡಿದ(p, ನೋಡಿದ_p) && !still_interesting)
				extra--;
			p->object.flags |= flags;
			prio_ಸರತಿ_put(ಸರತಿ, p);
		}
	}

	/*
	 * Postprocess to complete well-poisoning.
	 *
	 * At this point we have all the commits we have seen in
	 * seen_p list.  Mark anything that can be reached from
	 * uninteresting commits not interesting.
	 */
	ಗಾಗಿ (;;) {
		ಪೂರ್ಣಾಂಕ ಬದಲಾಯಿತು = 0;
		ರಚನೆ commit_list *s;
		ಗಾಗಿ (s = *ನೋಡಿದ_p; s; s = s->ಮುಂದಿನ) {
			ರಚನೆ commit *c = s->ಅಂಶ;
			ರಚನೆ commit_list *parents;

			ಆದರೆ (((c->object.flags & all_revs) != all_revs) &&
			    !(c->object.flags & UNINTERESTING))
				ಮುಂದುವರಿ;

			/* The current commit is either a merge base or
			 * already uninteresting one.  Mark its parents
			 * as uninteresting commits _only_ if they are
			 * already parsed.  No reason to find new ones
			 * here.
			 */
			parents = c->parents;
			ಆಗ (parents) {
				ರಚನೆ commit *p = parents->ಅಂಶ;
				parents = parents->ಮುಂದಿನ;
				ಆದರೆ (!(p->object.flags & UNINTERESTING)) {
					p->object.flags |= UNINTERESTING;
					ಬದಲಾಯಿತು = 1;
				}
			}
		}
		ಆದರೆ (!ಬದಲಾಯಿತು)
			ನಿಲ್ಲಿಸು;
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ show_one_commit(ರಚನೆ commit *commit, ಪೂರ್ಣಾಂಕ no_name)
{
	ರಚನೆ strbuf pretty = STRBUF_INIT;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *pretty_str = "(unavailable)";
	ರಚನೆ commit_name *name = commit_ಗಮ್ಯ_name(commit);

	ಆದರೆ (commit->object.parsed) {
		pp_commit_easy(CMIT_FMT_ONELINE, commit, &pretty);
		pretty_str = pretty.buf;
	}
	skip_ಪೂರ್ವಪ್ರತ್ಯಯ(pretty_str, "[PATCH] ", &pretty_str);

	ಆದರೆ (!no_name) {
		ಆದರೆ (name && name->ತಲೆ_name) {
			ಬರೆ("[%s", name->ತಲೆ_name);
			ಆದರೆ (name->generation) {
				ಆದರೆ (name->generation == 1)
					ಬರೆ("^");
				ಇಲ್ಲದಿದ್ದರೆ
					ಬರೆ("~%d", name->generation);
			}
			ಬರೆ("] ");
		}
		ಇಲ್ಲದಿದ್ದರೆ
			ಬರೆ("[%s] ",
			       repo_ಕಂಡುಹಿಡಿ_unique_abbrev(the_repository, &commit->object.oid,
						       DEFAULT_ABBREV));
	}
	puts(pretty_str);
	strbuf_release(&pretty);
}

ಸ್ಥಿರ ಅಕ್ಷರ *ref_name[MAX_REVS + 1];
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ref_name_cnt;

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕಂಡುಹಿಡಿ_digit_ಪೂರ್ವಪ್ರತ್ಯಯ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s, ಪೂರ್ಣಾಂಕ *v)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *p;
	ಪೂರ್ಣಾಂಕ ver;
	ಅಕ್ಷರ ch;

	ಗಾಗಿ (p = s, ver = 0;
	     '0' <= (ch = *p) && ch <= '9';
	     p++)
		ver = ver * 10 + ch - '0';
	*v = ver;
	ಹಿಂತಿರುಗಿ p;
}


ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಆವೃತ್ತಿ_cmp(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *a, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *b)
{
	ಆಗ (1) {
		ಪೂರ್ಣಾಂಕ va, vb;

		a = ಕಂಡುಹಿಡಿ_digit_ಪೂರ್ವಪ್ರತ್ಯಯ(a, &va);
		b = ಕಂಡುಹಿಡಿ_digit_ಪೂರ್ವಪ್ರತ್ಯಯ(b, &vb);
		ಆದರೆ (va != vb)
			ಹಿಂತಿರುಗಿ va - vb;

		ಆಗ (1) {
			ಪೂರ್ಣಾಂಕ ca = *a;
			ಪೂರ್ಣಾಂಕ cb = *b;
			ಆದರೆ ('0' <= ca && ca <= '9')
				ca = 0;
			ಆದರೆ ('0' <= cb && cb <= '9')
				cb = 0;
			ಆದರೆ (ca != cb)
				ಹಿಂತಿರುಗಿ ca - cb;
			ಆದರೆ (!ca)
				ನಿಲ್ಲಿಸು;
			a++;
			b++;
		}
		ಆದರೆ (!*a && !*b)
			ಹಿಂತಿರುಗಿ 0;
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ compare_ref_name(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *a_, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *b_)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ*a = a_, * ಸ್ಥಿರಾಂಕ*b = b_;
	ಹಿಂತಿರುಗಿ ಆವೃತ್ತಿ_cmp(*a, *b);
}

ಸ್ಥಿರ ಶೂನ್ಯ ವಿಂಗಡಿಸು_ref_range(ಪೂರ್ಣಾಂಕ bottom, ಪೂರ್ಣಾಂಕ ತುದಿ)
{
	QSORT(ref_name + bottom, ತುದಿ - bottom, compare_ref_name);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ append_ref(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *refname, ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid,
		      ಪೂರ್ಣಾಂಕ allow_dups)
{
	ರಚನೆ commit *commit = lookup_commit_reference_gently(the_repository,
							       oid, 1);
	ಪೂರ್ಣಾಂಕ i;

	ಆದರೆ (!commit)
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (!allow_dups) {
		/* Avoid adding the same thing twice */
		ಗಾಗಿ (i = 0; i < ref_name_cnt; i++)
			ಆದರೆ (!strcmp(refname, ref_name[i]))
				ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (MAX_REVS <= ref_name_cnt) {
		warning(Q_("ignoring %s; cannot handle more than %d ref",
			   "ignoring %s; cannot handle more than %d refs",
			   MAX_REVS), refname, MAX_REVS);
		ಹಿಂತಿರುಗಿ 0;
	}
	ref_name[ref_name_cnt++] = xstrdup(refname);
	ref_name[ref_name_cnt] = ಖಾಲಿ;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ append_ತಲೆ_ref(ಸ್ಥಿರಾಂಕ ರಚನೆ reference *ref, ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ ಬಳಸದ)
{
	ರಚನೆ object_id tmp;
	ಪೂರ್ಣಾಂಕ ofs = 11;
	ಆದರೆ (!starts_with(ref->name, "refs/heads/"))
		ಹಿಂತಿರುಗಿ 0;
	/* If both heads/foo and tags/foo exists, get_sha1 would
	 * get confused.
	 */
	ಆದರೆ (repo_get_oid(the_repository, ref->name + ofs, &tmp) || !oideq(&tmp, ref->oid))
		ofs = 5;
	ಹಿಂತಿರುಗಿ append_ref(ref->name + ofs, ref->oid, 0);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ append_remote_ref(ಸ್ಥಿರಾಂಕ ರಚನೆ reference *ref, ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ ಬಳಸದ)
{
	ರಚನೆ object_id tmp;
	ಪೂರ್ಣಾಂಕ ofs = 13;
	ಆದರೆ (!starts_with(ref->name, "refs/remotes/"))
		ಹಿಂತಿರುಗಿ 0;
	/* If both heads/foo and tags/foo exists, get_sha1 would
	 * get confused.
	 */
	ಆದರೆ (repo_get_oid(the_repository, ref->name + ofs, &tmp) || !oideq(&tmp, ref->oid))
		ofs = 5;
	ಹಿಂತಿರುಗಿ append_ref(ref->name + ofs, ref->oid, 0);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ append_ಟ್ಯಾಗ್_ref(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *refname, ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid,
			  ಪೂರ್ಣಾಂಕ flag ಬಳಸದ, ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ ಬಳಸದ)
{
	ಆದರೆ (!starts_with(refname, "refs/tags/"))
		ಹಿಂತಿರುಗಿ 0;
	ಹಿಂತಿರುಗಿ append_ref(refname + 5, oid, 0);
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಹೊಂದಿಕೆ_ref_ನಮೂನೆ = ಖಾಲಿ;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಹೊಂದಿಕೆ_ref_slash = 0;

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ append_matching_ref(ಸ್ಥಿರಾಂಕ ರಚನೆ reference *ref, ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ)
{
	/* we want to allow pattern hold/<asterisk> to show all
	 * branches under refs/heads/hold/, and v0.99.9? to show
	 * refs/tags/v0.99.9a and friends.
	 */
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಬಾಲ;
	ಪೂರ್ಣಾಂಕ slash = ಎಣಿಕೆ_slashes(ref->name);
	ಗಾಗಿ (ಬಾಲ = ref->name; *ಬಾಲ && ಹೊಂದಿಕೆ_ref_slash < slash; )
		ಆದರೆ (*ಬಾಲ++ == '/')
			slash--;
	ಆದರೆ (!*ಬಾಲ)
		ಹಿಂತಿರುಗಿ 0;
	ಆದರೆ (wildmatch(ಹೊಂದಿಕೆ_ref_ನಮೂನೆ, ಬಾಲ, 0))
		ಹಿಂತಿರುಗಿ 0;
	ಆದರೆ (starts_with(ref->name, "refs/heads/"))
		ಹಿಂತಿರುಗಿ append_ತಲೆ_ref(ref, cb_ದತ್ತಾಂಶ);
	ಆದರೆ (starts_with(ref->name, "refs/tags/"))
		ಹಿಂತಿರುಗಿ append_ಟ್ಯಾಗ್_ref(ref->name, ref->oid, ref->flags, cb_ದತ್ತಾಂಶ);
	ಹಿಂತಿರುಗಿ append_ref(ref->name, ref->oid, 0);
}

ಸ್ಥಿರ ಶೂನ್ಯ snarf_refs(ಪೂರ್ಣಾಂಕ ತಲೆ, ಪೂರ್ಣಾಂಕ remotes)
{
	ಆದರೆ (ತಲೆ) {
		ಪೂರ್ಣಾಂಕ orig_cnt = ref_name_cnt;

		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_ತಲೆ_ref, ಖಾಲಿ);
		ವಿಂಗಡಿಸು_ref_range(orig_cnt, ref_name_cnt);
	}
	ಆದರೆ (remotes) {
		ಪೂರ್ಣಾಂಕ orig_cnt = ref_name_cnt;

		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_remote_ref, ಖಾಲಿ);
		ವಿಂಗಡಿಸು_ref_range(orig_cnt, ref_name_cnt);
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ rev_is_ತಲೆ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ತಲೆ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name)
{
	ಆದರೆ (!ತಲೆ)
		ಹಿಂತಿರುಗಿ 0;
	skip_ಪೂರ್ವಪ್ರತ್ಯಯ(ತಲೆ, "refs/heads/", &ತಲೆ);
	ಆದರೆ (!skip_ಪೂರ್ವಪ್ರತ್ಯಯ(name, "refs/heads/", &name))
		skip_ಪೂರ್ವಪ್ರತ್ಯಯ(name, "heads/", &name);
	ಹಿಂತಿರುಗಿ !strcmp(ತಲೆ, name);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_ವಿಲೀನ_ತಳ(ಸ್ಥಿರಾಂಕ ರಚನೆ commit_list *ನೋಡಿದ, ಪೂರ್ಣಾಂಕ ಅಂಕೆ_rev)
{
	ಪೂರ್ಣಾಂಕ all_ಮುಖವಾಡ = ((1u << (REV_ಸರಿಸು + ಅಂಕೆ_rev)) - 1);
	ಪೂರ್ಣಾಂಕ all_revs = all_ಮುಖವಾಡ & ~((1u << REV_ಸರಿಸು) - 1);
	ಪೂರ್ಣಾಂಕ exit_ಸ್ಥಿತಿ = 1;

	ಗಾಗಿ (ಸ್ಥಿರಾಂಕ ರಚನೆ commit_list *s = ನೋಡಿದ; s; s = s->ಮುಂದಿನ) {
		ರಚನೆ commit *commit = s->ಅಂಶ;
		ಪೂರ್ಣಾಂಕ flags = commit->object.flags & all_ಮುಖವಾಡ;
		ಆದರೆ (!(flags & UNINTERESTING) &&
		    ((flags & all_revs) == all_revs)) {
			puts(oid_ಗಮ್ಯ_hex(&commit->object.oid));
			exit_ಸ್ಥಿತಿ = 0;
			commit->object.flags |= UNINTERESTING;
		}
	}
	ಹಿಂತಿರುಗಿ exit_ಸ್ಥಿತಿ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_independent(ರಚನೆ commit **rev,
			    ಪೂರ್ಣಾಂಕ ಅಂಕೆ_rev,
			    ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ *rev_ಮುಖವಾಡ)
{
	ಪೂರ್ಣಾಂಕ i;

	ಗಾಗಿ (i = 0; i < ಅಂಕೆ_rev; i++) {
		ರಚನೆ commit *commit = rev[i];
		ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flag = rev_ಮುಖವಾಡ[i];

		ಆದರೆ (commit->object.flags == flag)
			puts(oid_ಗಮ್ಯ_hex(&commit->object.oid));
		commit->object.flags |= UNINTERESTING;
	}
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ append_one_rev(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *av)
{
	ರಚನೆ object_id revkey;
	ಆದರೆ (!repo_get_oid(the_repository, av, &revkey)) {
		append_ref(av, &revkey, 0);
		ಹಿಂತಿರುಗಿ;
	}
	ಆದರೆ (strpbrk(av, "*?[")) {
		/* glob style match */
		ಪೂರ್ಣಾಂಕ saved_matches = ref_name_cnt;

		ಹೊಂದಿಕೆ_ref_ನಮೂನೆ = av;
		ಹೊಂದಿಕೆ_ref_slash = ಎಣಿಕೆ_slashes(av);
		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_matching_ref, ಖಾಲಿ);
		ಆದರೆ (saved_matches == ref_name_cnt &&
		    ref_name_cnt < MAX_REVS)
			error(_("no matching refs with %s"), av);
		ವಿಂಗಡಿಸು_ref_range(saved_matches, ref_name_cnt);
		ಹಿಂತಿರುಗಿ;
	}
	die("bad sha1 reference %s", av);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ git_show_branch_config(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *var, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ,
				  ಸ್ಥಿರಾಂಕ ರಚನೆ config_context *ctx, ಶೂನ್ಯ *cb)
{
	ಆದರೆ (!strcmp(var, "showbranch.default")) {
		ಆದರೆ (!ಮೌಲ್ಯ)
			ಹಿಂತಿರುಗಿ config_error_nonbool(var);
		/*
		 * default_arg is now passed to parse_options(), so we need to
		 * mimic the real argv a bit better.
		 */
		ಆದರೆ (!default_ವಾದಗಳು.nr)
			strvec_ತಳ್ಳು(&default_ವಾದಗಳು, "show-branch");
		strvec_ತಳ್ಳು(&default_ವಾದಗಳು, ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (!strcmp(var, "color.showbranch")) {
		showbranch_use_color = git_config_colorbool(var, ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (git_color_config(var, ಮೌಲ್ಯ, cb) < 0)
		ಹಿಂತಿರುಗಿ -1;

	ಹಿಂತಿರುಗಿ git_default_config(var, ಮೌಲ್ಯ, ctx, cb);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ omit_ಒಳ_dense(ರಚನೆ commit *commit, ರಚನೆ commit **rev, ಪೂರ್ಣಾಂಕ n)
{
	/* If the commit is tip of the named branches, do not
	 * omit it.
	 * Otherwise, if it is a merge that is reachable from only one
	 * tip, it is not that interesting.
	 */
	ಪೂರ್ಣಾಂಕ i, flag, ಎಣಿಕೆ_2;
	ಗಾಗಿ (i = 0; i < n; i++)
		ಆದರೆ (rev[i] == commit)
			ಹಿಂತಿರುಗಿ 0;
	flag = commit->object.flags;
	ಗಾಗಿ (i = ಎಣಿಕೆ_2 = 0; i < n; i++) {
		ಆದರೆ (flag & (1u << (i + REV_ಸರಿಸು)))
			ಎಣಿಕೆ_2++;
	}
	ಆದರೆ (ಎಣಿಕೆ_2 == 1)
		ಹಿಂತಿರುಗಿ 1;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ reflog = 0;

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ parse_reflog_param(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg,
			      ಪೂರ್ಣಾಂಕ unset)
{
	ಅಕ್ಷರ *ep;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **ತಳ = (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **)opt->ಮೌಲ್ಯ;
	BUG_ON_OPT_NEG(unset);
	ಆದರೆ (!arg)
		arg = "";
	reflog = strtoul(arg, &ep, 10);
	ಆದರೆ (*ep == ',')
		*ತಳ = ep + 1;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (*ep)
		ಹಿಂತಿರುಗಿ error("unrecognized reflog param '%s'", arg);
	ಇಲ್ಲದಿದ್ದರೆ
		*ತಳ = ಖಾಲಿ;
	ಆದರೆ (reflog <= 0)
		reflog = DEFAULT_REFLOG;
	ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ cmd_show_branch(ಪೂರ್ಣಾಂಕ ac,
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **av,
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ commit *rev[MAX_REVS], *commit;
	ಅಕ್ಷರ *reflog_ಸಂದೇಶ[MAX_REVS] = {0};
	ರಚನೆ commit_list *ನೋಡಿದ = ಖಾಲಿ;
	ರಚನೆ prio_ಸರತಿ ಸರತಿ = { compare_commits_by_commit_date };
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ rev_ಮುಖವಾಡ[MAX_REVS];
	ಪೂರ್ಣಾಂಕ ಅಂಕೆ_rev, i, extra = 0;
	ಪೂರ್ಣಾಂಕ all_heads = 0, all_remotes = 0;
	ಪೂರ್ಣಾಂಕ all_ಮುಖವಾಡ, all_revs;
	ಎಣಿಕೆ rev_ವಿಂಗಡಿಸು_ಕ್ರಮ ವಿಂಗಡಿಸು_ಕ್ರಮ = REV_ವಿಂಗಡಿಸು_ಒಳ_ನಕ್ಷೆ_ಕ್ರಮ;
	ಅಕ್ಷರ *ತಲೆ;
	ರಚನೆ object_id ತಲೆ_oid;
	ಪೂರ್ಣಾಂಕ ವಿಲೀನ_ತಳ = 0;
	ಪೂರ್ಣಾಂಕ independent = 0;
	ಪೂರ್ಣಾಂಕ no_name = 0;
	ಪೂರ್ಣಾಂಕ sha1_name = 0;
	ಪೂರ್ಣಾಂಕ shown_ವಿಲೀನ_ಬಿಂದು = 0;
	ಪೂರ್ಣಾಂಕ with_current_branch = 0;
	ಪೂರ್ಣಾಂಕ ತಲೆ_at = -1;
	ಪೂರ್ಣಾಂಕ topics = 0;
	ಪೂರ್ಣಾಂಕ sparse = 0;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *reflog_ತಳ = ಖಾಲಿ;
	ರಚನೆ option builtin_show_branch_options[] = {
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
			.precision = ಗಾತ್ರ(extra),
			.argh = N_("n"),
			.help = N_("show <n> more commits after the common ancestor"),
			.flags = PARSE_OPT_OPTARG,
			.defval = 1,
		},
		OPT_ಗಣ_INT(0, "list", &extra, N_("synonym to more=-1"), -1),
		OPT_BOOL(0, "no-name", &no_name, N_("suppress naming strings")),
		OPT_BOOL(0, "current", &with_current_branch,
			 N_("include the current branch")),
		OPT_BOOL(0, "sha1-name", &sha1_name,
			 N_("name commits with their object names")),
		OPT_BOOL(0, "merge-base", &ವಿಲೀನ_ತಳ,
			 N_("show possible merge bases")),
		OPT_BOOL(0, "independent", &independent,
			    N_("show refs unreachable from any other ref")),
		OPT_ಗಣ_INT_F(0, "topo-order", &ವಿಂಗಡಿಸು_ಕ್ರಮ,
			      N_("show commits in topological order"),
			      REV_ವಿಂಗಡಿಸು_ಒಳ_ನಕ್ಷೆ_ಕ್ರಮ, PARSE_OPT_NONEG),
		OPT_BOOL(0, "topics", &topics,
			 N_("show only commits not on the first branch")),
		OPT_ಗಣ_INT(0, "sparse", &sparse,
			    N_("show merges reachable from only one tip"), 1),
		OPT_ಗಣ_INT_F(0, "date-order", &ವಿಂಗಡಿಸು_ಕ್ರಮ,
			      N_("topologically sort, maintaining date order "
				 "where possible"),
			      REV_ವಿಂಗಡಿಸು_BY_COMMIT_DATE, PARSE_OPT_NONEG),
		OPT_CALLBACK_F('g', "reflog", &reflog_ತಳ, N_("<n>[,<base>]"),
			    N_("show <n> most recent ref-log entries starting at "
			       "base"),
			    PARSE_OPT_OPTARG | PARSE_OPT_NONEG,
			    parse_reflog_param),
		OPT_ಕೊನೆ()
	};
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **ವಾದಗಳು_copy = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ ret;

	init_commit_name_slab(&name_slab);

	repo_config(the_repository, git_show_branch_config, ಖಾಲಿ);

	/* If nothing is specified, try the default first */
	ಆದರೆ (ac == 1 && default_ವಾದಗಳು.nr) {
		DUP_ARRAY(ವಾದಗಳು_copy, default_ವಾದಗಳು.v, default_ವಾದಗಳು.nr);
		ac = default_ವಾದಗಳು.nr;
		av = ವಾದಗಳು_copy;
	}

	ac = parse_options(ac, av, ಪೂರ್ವಪ್ರತ್ಯಯ, builtin_show_branch_options,
			   show_branch_usage, PARSE_OPT_STOP_AT_NON_OPTION);
	ಆದರೆ (all_heads)
		all_remotes = 1;

	ಆದರೆ (extra || reflog) {
		/* "listing" mode is incompatible with
		 * independent nor merge-base modes.
		 */
		ಆದರೆ (independent || ವಿಲೀನ_ತಳ)
			usage_with_options(show_branch_usage,
					   builtin_show_branch_options);
		ಆದರೆ (reflog && ((0 < extra) || all_heads || all_remotes))
			/*
			 * Asking for --more in reflog mode does not
			 * make sense.  --list is Ok.
			 *
			 * Also --all and --remotes do not make sense either.
			 */
			die(_("options '%s' and '%s' cannot be used together"), "--reflog",
				"--all/--remotes/--independent/--merge-base");
	}

	ಆದರೆ (with_current_branch && reflog)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--reflog", "--current");

	/* If nothing is specified, show all branches by default */
	ಆದರೆ (ac <= topics && all_heads + all_remotes == 0)
		all_heads = 1;

	ಆದರೆ (reflog) {
		ರಚನೆ object_id oid;
		ಅಕ್ಷರ *ref;
		ಪೂರ್ಣಾಂಕ ತಳ = 0;
		ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags = 0;

		ಆದರೆ (ac == 0) {
			ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fake_av[2];

			fake_av[0] = refs_resolve_refdup(get_main_ref_store(the_repository),
							 "HEAD",
							 RESOLVE_REF_READING,
							 &oid,
							 ಖಾಲಿ);
			fake_av[1] = ಖಾಲಿ;
			av = fake_av;
			ac = 1;
			ಆದರೆ (!*av)
				die(_("no branches given, and HEAD is not valid"));
		}
		ಆದರೆ (ac != 1)
			die(_("--reflog option needs one branch name"));

		ಆದರೆ (MAX_REVS < reflog)
			die(Q_("only %d entry can be shown at one time.",
			       "only %d entries can be shown at one time.",
			       MAX_REVS), MAX_REVS);
		ಆದರೆ (!repo_dwim_ref(the_repository, *av, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(*av), &oid,
				   &ref, 0))
			die(_("no such ref %s"), *av);

		/* Has the base been specified? */
		ಆದರೆ (reflog_ತಳ) {
			ಅಕ್ಷರ *ep;
			ತಳ = strtoul(reflog_ತಳ, &ep, 10);
			ಆದರೆ (*ep) {
				/* Ah, that is a date spec... */
				timestamp_t at;
				at = approxidate(reflog_ತಳ);
				read_ref_at(get_main_ref_store(the_repository),
					    ref, flags, at, -1, &oid, ಖಾಲಿ,
					    ಖಾಲಿ, ಖಾಲಿ, &ತಳ);
			}
		}

		ಗಾಗಿ (i = 0; i < reflog; i++) {
			ಅಕ್ಷರ *logmsg = ಖಾಲಿ;
			ಅಕ್ಷರ *nth_desc;
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ;
			ಅಕ್ಷರ *ಕೊನೆ;
			timestamp_t timestamp;
			ಪೂರ್ಣಾಂಕ tz;

			ಆದರೆ (read_ref_at(get_main_ref_store(the_repository),
					ref, flags, 0, ತಳ + i, &oid, &logmsg,
					&timestamp, &tz, ಖಾಲಿ)) {
				ಬಿಡುಗಡೆ(logmsg);
				reflog = i;
				ನಿಲ್ಲಿಸು;
			}

			ಕೊನೆ = strchr(logmsg, '\n');
			ಆದರೆ (ಕೊನೆ)
				*ಕೊನೆ = '\0';

			ಸಂದೇಶ = (*logmsg == '\0') ? "(none)" : logmsg;
			reflog_ಸಂದೇಶ[i] = xstrfmt("(%s) %s",
						show_date(timestamp, tz,
							  DATE_ಬಹುಲಕ(RELATIVE)),
						ಸಂದೇಶ);
			ಬಿಡುಗಡೆ(logmsg);

			nth_desc = xstrfmt("%s@{%d}", *av, ತಳ+i);
			append_ref(nth_desc, &oid, 1);
			ಬಿಡುಗಡೆ(nth_desc);
		}
		ಬಿಡುಗಡೆ(ref);
	}
	ಇಲ್ಲದಿದ್ದರೆ {
		ಆಗ (0 < ac) {
			append_one_rev(*av);
			ac--; av++;
		}
		ಆದರೆ (all_heads + all_remotes)
			snarf_refs(all_heads, all_remotes);
	}

	ತಲೆ = refs_resolve_refdup(get_main_ref_store(the_repository), "HEAD",
				   RESOLVE_REF_READING,
				   &ತಲೆ_oid, ಖಾಲಿ);

	ಆದರೆ (with_current_branch && ತಲೆ) {
		ಪೂರ್ಣಾಂಕ has_ತಲೆ = 0;
		ಗಾಗಿ (i = 0; !has_ತಲೆ && i < ref_name_cnt; i++) {
			/* We are only interested in adding the branch
			 * HEAD points at.
			 */
			ಆದರೆ (rev_is_ತಲೆ(ತಲೆ, ref_name[i]))
				has_ತಲೆ++;
		}
		ಆದರೆ (!has_ತಲೆ) {
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name = ತಲೆ;
			skip_ಪೂರ್ವಪ್ರತ್ಯಯ(name, "refs/heads/", &name);
			append_one_rev(name);
		}
	}

	ಆದರೆ (!ref_name_cnt) {
		fprintf(stderr, "No revs to be shown.\n");
		ret = 0;
		ಹೋಗು ಹೊರಹೋಗು;
	}

	ಗಾಗಿ (ಅಂಕೆ_rev = 0; ref_name[ಅಂಕೆ_rev]; ಅಂಕೆ_rev++) {
		ರಚನೆ object_id revkey;
		ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flag = 1u << (ಅಂಕೆ_rev + REV_ಸರಿಸು);

		ಆದರೆ (MAX_REVS <= ಅಂಕೆ_rev)
			die(Q_("cannot handle more than %d rev.",
			       "cannot handle more than %d revs.",
			       MAX_REVS), MAX_REVS);
		ಆದರೆ (repo_get_oid(the_repository, ref_name[ಅಂಕೆ_rev], &revkey))
			die(_("'%s' is not a valid ref."), ref_name[ಅಂಕೆ_rev]);
		commit = lookup_commit_reference(the_repository, &revkey);
		ಆದರೆ (!commit)
			die(_("cannot find commit %s (%s)"),
			    ref_name[ಅಂಕೆ_rev], oid_ಗಮ್ಯ_hex(&revkey));
		repo_parse_commit(the_repository, commit);
		mark_ನೋಡಿದ(commit, &ನೋಡಿದ);

		/* rev#0 uses bit REV_SHIFT, rev#1 uses bit REV_SHIFT+1,
		 * and so on.  REV_SHIFT bits from bit 0 are used for
		 * internal bookkeeping.
		 */
		commit->object.flags |= flag;
		ಆದರೆ (commit->object.flags == flag)
			prio_ಸರತಿ_put(&ಸರತಿ, commit);
		rev[ಅಂಕೆ_rev] = commit;
	}
	ಗಾಗಿ (i = 0; i < ಅಂಕೆ_rev; i++)
		rev_ಮುಖವಾಡ[i] = rev[i]->object.flags;

	ಆದರೆ (0 <= extra)
		join_revs(&ಸರತಿ, &ನೋಡಿದ, ಅಂಕೆ_rev, extra);

	commit_list_ವಿಂಗಡಿಸು_by_date(&ನೋಡಿದ);

	ಆದರೆ (ವಿಲೀನ_ತಳ) {
		ret = show_ವಿಲೀನ_ತಳ(ನೋಡಿದ, ಅಂಕೆ_rev);
		ಹೋಗು ಹೊರಹೋಗು;
	}

	ಆದರೆ (independent) {
		ret = show_independent(rev, ಅಂಕೆ_rev, rev_ಮುಖವಾಡ);
		ಹೋಗು ಹೊರಹೋಗು;
	}

	/* Show list; --more=-1 means list-only */
	ಆದರೆ (1 < ಅಂಕೆ_rev || extra < 0) {
		ಗಾಗಿ (i = 0; i < ಅಂಕೆ_rev; i++) {
			ಪೂರ್ಣಾಂಕ j;
			ಪೂರ್ಣಾಂಕ is_ತಲೆ = rev_is_ತಲೆ(ತಲೆ, ref_name[i]) &&
				      oideq(&ತಲೆ_oid, &rev[i]->object.oid);
			ಆದರೆ (extra < 0)
				ಬರೆ("%c [%s] ",
				       is_ತಲೆ ? '*' : ' ', ref_name[i]);
			ಇಲ್ಲದಿದ್ದರೆ {
				ಗಾಗಿ (j = 0; j < i; j++)
					ಅಕ್ಷರ_ಬರೆ(' ');
				ಬರೆ("%s%c%s [%s] ",
				       get_color_ಸಂಕೇತ(i),
				       is_ತಲೆ ? '*' : '!',
				       get_color_reset_ಸಂಕೇತ(), ref_name[i]);
			}

			ಆದರೆ (!reflog) {
				/* header lines never need name */
				show_one_commit(rev[i], 1);
			}
			ಇಲ್ಲದಿದ್ದರೆ
				puts(reflog_ಸಂದೇಶ[i]);

			ಆದರೆ (is_ತಲೆ)
				ತಲೆ_at = i;
		}
		ಆದರೆ (0 <= extra) {
			ಗಾಗಿ (i = 0; i < ಅಂಕೆ_rev; i++)
				ಅಕ್ಷರ_ಬರೆ('-');
			ಅಕ್ಷರ_ಬರೆ('\n');
		}
	}
	ಆದರೆ (extra < 0) {
		ret = 0;
		ಹೋಗು ಹೊರಹೋಗು;
	}

	/* Sort topologically */
	ವಿಂಗಡಿಸು_ಒಳ_topological_ಕ್ರಮ(&ನೋಡಿದ, ವಿಂಗಡಿಸು_ಕ್ರಮ);

	/* Give names to commits */
	ಆದರೆ (!sha1_name && !no_name)
		name_commits(ನೋಡಿದ, rev, ref_name, ಅಂಕೆ_rev);

	all_ಮುಖವಾಡ = ((1u << (REV_ಸರಿಸು + ಅಂಕೆ_rev)) - 1);
	all_revs = all_ಮುಖವಾಡ & ~((1u << REV_ಸರಿಸು) - 1);

	ಗಾಗಿ (ರಚನೆ commit_list *l = ನೋಡಿದ; l; l = l->ಮುಂದಿನ) {
		ರಚನೆ commit *commit = l->ಅಂಶ;
		ಪೂರ್ಣಾಂಕ this_flag = commit->object.flags;
		ಪೂರ್ಣಾಂಕ is_ವಿಲೀನ_ಬಿಂದು = ((this_flag & all_revs) == all_revs);

		shown_ವಿಲೀನ_ಬಿಂದು |= is_ವಿಲೀನ_ಬಿಂದು;

		ಆದರೆ (1 < ಅಂಕೆ_rev) {
			ಪೂರ್ಣಾಂಕ is_ವಿಲೀನ = !!(commit->parents &&
					  commit->parents->ಮುಂದಿನ);
			ಆದರೆ (topics &&
			    !is_ವಿಲೀನ_ಬಿಂದು &&
			    (this_flag & (1u << REV_ಸರಿಸು)))
				ಮುಂದುವರಿ;
			ಆದರೆ (!sparse && is_ವಿಲೀನ &&
			    omit_ಒಳ_dense(commit, rev, ಅಂಕೆ_rev))
				ಮುಂದುವರಿ;
			ಗಾಗಿ (i = 0; i < ಅಂಕೆ_rev; i++) {
				ಪೂರ್ಣಾಂಕ mark;
				ಆದರೆ (!(this_flag & (1u << (i + REV_ಸರಿಸು))))
					mark = ' ';
				ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (is_ವಿಲೀನ)
					mark = '-';
				ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (i == ತಲೆ_at)
					mark = '*';
				ಇಲ್ಲದಿದ್ದರೆ
					mark = '+';
				ಆದರೆ (mark == ' ')
					ಅಕ್ಷರ_ಬರೆ(mark);
				ಇಲ್ಲದಿದ್ದರೆ
					ಬರೆ("%s%c%s",
					       get_color_ಸಂಕೇತ(i),
					       mark, get_color_reset_ಸಂಕೇತ());
			}
			ಅಕ್ಷರ_ಬರೆ(' ');
		}
		show_one_commit(commit, no_name);

		ಆದರೆ (shown_ವಿಲೀನ_ಬಿಂದು && --extra < 0)
			ನಿಲ್ಲಿಸು;
	}

	ret = 0;

ಹೊರಹೋಗು:
	ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < ARRAY_ಗಾತ್ರ(reflog_ಸಂದೇಶ); i++)
		ಬಿಡುಗಡೆ(reflog_ಸಂದೇಶ[i]);
	commit_list_free(ನೋಡಿದ);
	clear_prio_ಸರತಿ(&ಸರತಿ);
	ಬಿಡುಗಡೆ(ವಾದಗಳು_copy);
	ಬಿಡುಗಡೆ(ತಲೆ);
	ಹಿಂತಿರುಗಿ ret;
}
