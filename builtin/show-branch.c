#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DISABLE_SIGN_COMPARE_WARNINGS

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hash.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hex.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "pretty.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "refs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "color.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "strvec.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-name.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "dir.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "commit-slab.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "date.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "wildmatch.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "prio-queue.h"

ਸਥਿਰ ਅਚਲ ਅੱਖਰ*ਅਚਲ show_branch_usage[] = {
    N_("git show-branch [-a | --all] [-r | --remotes] [--topo-order | --date-order]\n"
       "                [--current] [--color[=<when>] | --no-color] [--sparse]\n"
       "                [--more=<n> | --list | --independent | --merge-base]\n"
       "                [--no-name | --sha1-name] [--topics]\n"
       "                [(<rev> | <glob>)...]"),
    N_("git show-branch (-g | --reflog)[=<n>[,<base>]] [--list] [<ref>]"),
    ਨਿਰਾ
};

ਸਥਿਰ ਗਿਣਤੀ git_colorbool showbranch_use_color = GIT_COLOR_UNKNOWN;

ਸਥਿਰ ਬਣਤਰ strvec default_ਦਲੀਲਾਂ = STRVEC_INIT;

/*
 * TODO: convert this use of commit->object.flags to commit-slab
 * instead to store a pointer to ref name directly. Then use the same
 * UNINTERESTING definition from revision.h here.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ UNINTERESTING	01

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ REV_ਖਿਸਕਾਓ	 2
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MAX_REVS	(FLAG_ਬਿੱਟ - REV_ਖਿਸਕਾਓ) /* should not exceed bits_per_int - REV_SHIFT */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEFAULT_REFLOG	4

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *get_color_ਕੋਡ(ਪੂਰਨ idx)
{
	ਜੇ (want_color(showbranch_use_color))
		ਵਾਪਸ ਕਾਲਮ_colors_ansi[idx % ਕਾਲਮ_colors_ansi_max];
	ਵਾਪਸ "";
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *get_color_reset_ਕੋਡ(ਖਾਲੀ)
{
	ਜੇ (want_color(showbranch_use_color))
		ਵਾਪਸ GIT_COLOR_RESET;
	ਵਾਪਸ "";
}

ਸਥਿਰ ਬਣਤਰ commit *interesting(ਬਣਤਰ prio_ਕਤਾਰ *ਕਤਾਰ)
{
	ਬਣਤਰ commit *commit;
	prio_ਕਤਾਰ_for_each(ਕਤਾਰ, commit) {
		ਜੇ (!(commit->object.flags & UNINTERESTING))
			ਵਾਪਸ commit;
	}
	ਵਾਪਸ ਨਿਰਾ;
}

ਬਣਤਰ commit_name {
	ਅਚਲ ਅੱਖਰ *ਸਿਰ_name; /* which head's ancestor? */
	ਪੂਰਨ generation; /* how many parents away from head_name */
};

define_commit_slab(commit_name_slab, ਬਣਤਰ commit_name *);
ਸਥਿਰ ਬਣਤਰ commit_name_slab name_slab;

ਸਥਿਰ ਬਣਤਰ commit_name *commit_ਮੰਜ਼ਿਲ_name(ਬਣਤਰ commit *commit)
{
	ਵਾਪਸ *commit_name_slab_at(&name_slab, commit);
}


/* Name the commit as nth generation ancestor of head_name;
 * we count only the first-parent relationship for naming purposes.
 */
ਸਥਿਰ ਖਾਲੀ name_commit(ਬਣਤਰ commit *commit, ਅਚਲ ਅੱਖਰ *ਸਿਰ_name, ਪੂਰਨ nth)
{
	ਬਣਤਰ commit_name *name;

	name = *commit_name_slab_at(&name_slab, commit);
	ਜੇ (!name) {
		name = xmalloc(ਆਕਾਰ(*name));
		*commit_name_slab_at(&name_slab, commit) = name;
	}
	name->ਸਿਰ_name = ਸਿਰ_name;
	name->generation = nth;
}

/* Parent is the first parent of the commit.  We may name it
 * as (n+1)th generation ancestor of the same head_name as
 * commit is nth generation ancestor of, if that generation
 * number is better than the name it already has.
 */
ਸਥਿਰ ਖਾਲੀ name_ਮਾਪਾ(ਬਣਤਰ commit *commit, ਬਣਤਰ commit *ਮਾਪਾ)
{
	ਬਣਤਰ commit_name *commit_name = commit_ਮੰਜ਼ਿਲ_name(commit);
	ਬਣਤਰ commit_name *ਮਾਪਾ_name = commit_ਮੰਜ਼ਿਲ_name(ਮਾਪਾ);
	ਜੇ (!commit_name)
		ਵਾਪਸ;
	ਜੇ (!ਮਾਪਾ_name ||
	    commit_name->generation + 1 < ਮਾਪਾ_name->generation)
		name_commit(ਮਾਪਾ, commit_name->ਸਿਰ_name,
			    commit_name->generation + 1);
}

ਸਥਿਰ ਪੂਰਨ name_ਪਹਿਲਾ_ਮਾਪਾ_chain(ਬਣਤਰ commit *c)
{
	ਪੂਰਨ i = 0;
	ਜਦੋਂ (c) {
		ਬਣਤਰ commit *p;
		ਜੇ (!commit_ਮੰਜ਼ਿਲ_name(c))
			ਰੋਕੋ;
		ਜੇ (!c->parents)
			ਰੋਕੋ;
		p = c->parents->ਵਸਤੂ;
		ਜੇ (!commit_ਮੰਜ਼ਿਲ_name(p)) {
			name_ਮਾਪਾ(c, p);
			i++;
		}
		ਨਹੀਂਤਾਂ
			ਰੋਕੋ;
		c = p;
	}
	ਵਾਪਸ i;
}

ਸਥਿਰ ਖਾਲੀ name_commits(ਬਣਤਰ commit_list *list,
			 ਬਣਤਰ commit **rev,
			 ਅੱਖਰ **ref_name,
			 ਪੂਰਨ ਅੰਕ_rev)
{
	ਬਣਤਰ commit_list *cl;
	ਬਣਤਰ commit *c;
	ਪੂਰਨ i;

	/* First give names to the given heads */
	ਲਈ (cl = list; cl; cl = cl->ਅਗਲਾ) {
		c = cl->ਵਸਤੂ;
		ਜੇ (commit_ਮੰਜ਼ਿਲ_name(c))
			ਜਾਰੀ;
		ਲਈ (i = 0; i < ਅੰਕ_rev; i++) {
			ਜੇ (rev[i] == c) {
				name_commit(c, ref_name[i], 0);
				ਰੋਕੋ;
			}
		}
	}

	/* Then commits on the first parent ancestry chain */
	ਕਰੋ {
		i = 0;
		ਲਈ (cl = list; cl; cl = cl->ਅਗਲਾ) {
			i += name_ਪਹਿਲਾ_ਮਾਪਾ_chain(cl->ਵਸਤੂ);
		}
	} ਜਦੋਂ (i);

	/* Finally, any unnamed commits */
	ਕਰੋ {
		i = 0;
		ਲਈ (cl = list; cl; cl = cl->ਅਗਲਾ) {
			ਬਣਤਰ commit_list *parents;
			ਬਣਤਰ commit_name *n;
			ਪੂਰਨ nth;
			c = cl->ਵਸਤੂ;
			ਜੇ (!commit_ਮੰਜ਼ਿਲ_name(c))
				ਜਾਰੀ;
			n = commit_ਮੰਜ਼ਿਲ_name(c);
			parents = c->parents;
			nth = 0;
			ਜਦੋਂ (parents) {
				ਬਣਤਰ commit *p = parents->ਵਸਤੂ;
				ਬਣਤਰ strbuf newname = STRBUF_INIT;
				parents = parents->ਅਗਲਾ;
				nth++;
				ਜੇ (commit_ਮੰਜ਼ਿਲ_name(p))
					ਜਾਰੀ;
				ਚੋਣ (n->generation) {
				ਹਾਲਤ 0:
					strbuf_addstr(&newname, n->ਸਿਰ_name);
					ਰੋਕੋ;
				ਹਾਲਤ 1:
					strbuf_addf(&newname, "%s^", n->ਸਿਰ_name);
					ਰੋਕੋ;
				ਮੂਲ:
					strbuf_addf(&newname, "%s~%d",
						    n->ਸਿਰ_name, n->generation);
					ਰੋਕੋ;
				}
				ਜੇ (nth == 1)
					strbuf_addch(&newname, '^');
				ਨਹੀਂਤਾਂ
					strbuf_addf(&newname, "^%d", nth);
				name_commit(p, strbuf_detach(&newname, ਨਿਰਾ), 0);
				i++;
				name_ਪਹਿਲਾ_ਮਾਪਾ_chain(p);
			}
		}
	} ਜਦੋਂ (i);
}

ਸਥਿਰ ਪੂਰਨ mark_ਵੇਖਿਆ(ਬਣਤਰ commit *commit, ਬਣਤਰ commit_list **ਵੇਖਿਆ_p)
{
	ਜੇ (!commit->object.flags) {
		commit_list_ਪਾਓ(commit, ਵੇਖਿਆ_p);
		ਵਾਪਸ 1;
	}
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ join_revs(ਬਣਤਰ prio_ਕਤਾਰ *ਕਤਾਰ,
		      ਬਣਤਰ commit_list **ਵੇਖਿਆ_p,
		      ਪੂਰਨ ਅੰਕ_rev, ਪੂਰਨ extra)
{
	ਪੂਰਨ all_ਨਕਾਬ = ((1u << (REV_ਖਿਸਕਾਓ + ਅੰਕ_rev)) - 1);
	ਪੂਰਨ all_revs = all_ਨਕਾਬ & ~((1u << REV_ਖਿਸਕਾਓ) - 1);
	ਬਣਤਰ commit *commit;

	ਜਦੋਂ ((commit = prio_ਕਤਾਰ_peek(ਕਤਾਰ))) {
		ਬਣਤਰ commit_list *parents;
		ਪੂਰਨ still_interesting = !!interesting(ਕਤਾਰ);
		ਪੂਰਨ flags = commit->object.flags & all_ਨਕਾਬ;

		ਜੇ (!still_interesting && extra <= 0)
			ਰੋਕੋ;

		prio_ਕਤਾਰ_get(ਕਤਾਰ);

		mark_ਵੇਖਿਆ(commit, ਵੇਖਿਆ_p);
		ਜੇ ((flags & all_revs) == all_revs)
			flags |= UNINTERESTING;
		parents = commit->parents;

		ਜਦੋਂ (parents) {
			ਬਣਤਰ commit *p = parents->ਵਸਤੂ;
			ਪੂਰਨ this_flag = p->object.flags;
			parents = parents->ਅਗਲਾ;
			ਜੇ ((this_flag & flags) == flags)
				ਜਾਰੀ;
			repo_parse_commit(the_repository, p);
			ਜੇ (mark_ਵੇਖਿਆ(p, ਵੇਖਿਆ_p) && !still_interesting)
				extra--;
			p->object.flags |= flags;
			prio_ਕਤਾਰ_put(ਕਤਾਰ, p);
		}
	}

	/*
	 * Postprocess to complete well-poisoning.
	 *
	 * At this point we have all the commits we have seen in
	 * seen_p list.  Mark anything that can be reached from
	 * uninteresting commits not interesting.
	 */
	ਲਈ (;;) {
		ਪੂਰਨ ਬਦਲਿਆ = 0;
		ਬਣਤਰ commit_list *s;
		ਲਈ (s = *ਵੇਖਿਆ_p; s; s = s->ਅਗਲਾ) {
			ਬਣਤਰ commit *c = s->ਵਸਤੂ;
			ਬਣਤਰ commit_list *parents;

			ਜੇ (((c->object.flags & all_revs) != all_revs) &&
			    !(c->object.flags & UNINTERESTING))
				ਜਾਰੀ;

			/* The current commit is either a merge base or
			 * already uninteresting one.  Mark its parents
			 * as uninteresting commits _only_ if they are
			 * already parsed.  No reason to find new ones
			 * here.
			 */
			parents = c->parents;
			ਜਦੋਂ (parents) {
				ਬਣਤਰ commit *p = parents->ਵਸਤੂ;
				parents = parents->ਅਗਲਾ;
				ਜੇ (!(p->object.flags & UNINTERESTING)) {
					p->object.flags |= UNINTERESTING;
					ਬਦਲਿਆ = 1;
				}
			}
		}
		ਜੇ (!ਬਦਲਿਆ)
			ਰੋਕੋ;
	}
}

ਸਥਿਰ ਖਾਲੀ show_one_commit(ਬਣਤਰ commit *commit, ਪੂਰਨ no_name)
{
	ਬਣਤਰ strbuf pretty = STRBUF_INIT;
	ਅਚਲ ਅੱਖਰ *pretty_str = "(unavailable)";
	ਬਣਤਰ commit_name *name = commit_ਮੰਜ਼ਿਲ_name(commit);

	ਜੇ (commit->object.parsed) {
		pp_commit_easy(CMIT_FMT_ONELINE, commit, &pretty);
		pretty_str = pretty.buf;
	}
	skip_ਅਗੇਤਰ(pretty_str, "[PATCH] ", &pretty_str);

	ਜੇ (!no_name) {
		ਜੇ (name && name->ਸਿਰ_name) {
			ਲਿਖੋ("[%s", name->ਸਿਰ_name);
			ਜੇ (name->generation) {
				ਜੇ (name->generation == 1)
					ਲਿਖੋ("^");
				ਨਹੀਂਤਾਂ
					ਲਿਖੋ("~%d", name->generation);
			}
			ਲਿਖੋ("] ");
		}
		ਨਹੀਂਤਾਂ
			ਲਿਖੋ("[%s] ",
			       repo_ਲੱਭੋ_unique_abbrev(the_repository, &commit->object.oid,
						       DEFAULT_ABBREV));
	}
	puts(pretty_str);
	strbuf_release(&pretty);
}

ਸਥਿਰ ਅੱਖਰ *ref_name[MAX_REVS + 1];
ਸਥਿਰ ਪੂਰਨ ref_name_cnt;

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਲੱਭੋ_digit_ਅਗੇਤਰ(ਅਚਲ ਅੱਖਰ *s, ਪੂਰਨ *v)
{
	ਅਚਲ ਅੱਖਰ *p;
	ਪੂਰਨ ver;
	ਅੱਖਰ ch;

	ਲਈ (p = s, ver = 0;
	     '0' <= (ch = *p) && ch <= '9';
	     p++)
		ver = ver * 10 + ch - '0';
	*v = ver;
	ਵਾਪਸ p;
}


ਸਥਿਰ ਪੂਰਨ ਸੰਸਕਰਣ_cmp(ਅਚਲ ਅੱਖਰ *a, ਅਚਲ ਅੱਖਰ *b)
{
	ਜਦੋਂ (1) {
		ਪੂਰਨ va, vb;

		a = ਲੱਭੋ_digit_ਅਗੇਤਰ(a, &va);
		b = ਲੱਭੋ_digit_ਅਗੇਤਰ(b, &vb);
		ਜੇ (va != vb)
			ਵਾਪਸ va - vb;

		ਜਦੋਂ (1) {
			ਪੂਰਨ ca = *a;
			ਪੂਰਨ cb = *b;
			ਜੇ ('0' <= ca && ca <= '9')
				ca = 0;
			ਜੇ ('0' <= cb && cb <= '9')
				cb = 0;
			ਜੇ (ca != cb)
				ਵਾਪਸ ca - cb;
			ਜੇ (!ca)
				ਰੋਕੋ;
			a++;
			b++;
		}
		ਜੇ (!*a && !*b)
			ਵਾਪਸ 0;
	}
}

ਸਥਿਰ ਪੂਰਨ compare_ref_name(ਅਚਲ ਖਾਲੀ *a_, ਅਚਲ ਖਾਲੀ *b_)
{
	ਅਚਲ ਅੱਖਰ * ਅਚਲ*a = a_, * ਅਚਲ*b = b_;
	ਵਾਪਸ ਸੰਸਕਰਣ_cmp(*a, *b);
}

ਸਥਿਰ ਖਾਲੀ ਛਾਂਟੋ_ref_range(ਪੂਰਨ bottom, ਪੂਰਨ ਸਿਖਰ)
{
	QSORT(ref_name + bottom, ਸਿਖਰ - bottom, compare_ref_name);
}

ਸਥਿਰ ਪੂਰਨ append_ref(ਅਚਲ ਅੱਖਰ *refname, ਅਚਲ ਬਣਤਰ object_id *oid,
		      ਪੂਰਨ allow_dups)
{
	ਬਣਤਰ commit *commit = lookup_commit_reference_gently(the_repository,
							       oid, 1);
	ਪੂਰਨ i;

	ਜੇ (!commit)
		ਵਾਪਸ 0;

	ਜੇ (!allow_dups) {
		/* Avoid adding the same thing twice */
		ਲਈ (i = 0; i < ref_name_cnt; i++)
			ਜੇ (!strcmp(refname, ref_name[i]))
				ਵਾਪਸ 0;
	}
	ਜੇ (MAX_REVS <= ref_name_cnt) {
		warning(Q_("ignoring %s; cannot handle more than %d ref",
			   "ignoring %s; cannot handle more than %d refs",
			   MAX_REVS), refname, MAX_REVS);
		ਵਾਪਸ 0;
	}
	ref_name[ref_name_cnt++] = xstrdup(refname);
	ref_name[ref_name_cnt] = ਨਿਰਾ;
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ append_ਸਿਰ_ref(ਅਚਲ ਬਣਤਰ reference *ref, ਖਾਲੀ *cb_ਅੰਕੜੇ ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ object_id tmp;
	ਪੂਰਨ ofs = 11;
	ਜੇ (!starts_with(ref->name, "refs/heads/"))
		ਵਾਪਸ 0;
	/* If both heads/foo and tags/foo exists, get_sha1 would
	 * get confused.
	 */
	ਜੇ (repo_get_oid(the_repository, ref->name + ofs, &tmp) || !oideq(&tmp, ref->oid))
		ofs = 5;
	ਵਾਪਸ append_ref(ref->name + ofs, ref->oid, 0);
}

ਸਥਿਰ ਪੂਰਨ append_remote_ref(ਅਚਲ ਬਣਤਰ reference *ref, ਖਾਲੀ *cb_ਅੰਕੜੇ ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ object_id tmp;
	ਪੂਰਨ ofs = 13;
	ਜੇ (!starts_with(ref->name, "refs/remotes/"))
		ਵਾਪਸ 0;
	/* If both heads/foo and tags/foo exists, get_sha1 would
	 * get confused.
	 */
	ਜੇ (repo_get_oid(the_repository, ref->name + ofs, &tmp) || !oideq(&tmp, ref->oid))
		ofs = 5;
	ਵਾਪਸ append_ref(ref->name + ofs, ref->oid, 0);
}

ਸਥਿਰ ਪੂਰਨ append_ਟੈਗ_ref(ਅਚਲ ਅੱਖਰ *refname, ਅਚਲ ਬਣਤਰ object_id *oid,
			  ਪੂਰਨ flag ਅਣਵਰਤਿਆ, ਖਾਲੀ *cb_ਅੰਕੜੇ ਅਣਵਰਤਿਆ)
{
	ਜੇ (!starts_with(refname, "refs/tags/"))
		ਵਾਪਸ 0;
	ਵਾਪਸ append_ref(refname + 5, oid, 0);
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਮੇਲ_ref_ਨਮੂਨਾ = ਨਿਰਾ;
ਸਥਿਰ ਪੂਰਨ ਮੇਲ_ref_slash = 0;

ਸਥਿਰ ਪੂਰਨ append_matching_ref(ਅਚਲ ਬਣਤਰ reference *ref, ਖਾਲੀ *cb_ਅੰਕੜੇ)
{
	/* we want to allow pattern hold/<asterisk> to show all
	 * branches under refs/heads/hold/, and v0.99.9? to show
	 * refs/tags/v0.99.9a and friends.
	 */
	ਅਚਲ ਅੱਖਰ *ਪੂਛ;
	ਪੂਰਨ slash = ਗਿਣਤੀ_slashes(ref->name);
	ਲਈ (ਪੂਛ = ref->name; *ਪੂਛ && ਮੇਲ_ref_slash < slash; )
		ਜੇ (*ਪੂਛ++ == '/')
			slash--;
	ਜੇ (!*ਪੂਛ)
		ਵਾਪਸ 0;
	ਜੇ (wildmatch(ਮੇਲ_ref_ਨਮੂਨਾ, ਪੂਛ, 0))
		ਵਾਪਸ 0;
	ਜੇ (starts_with(ref->name, "refs/heads/"))
		ਵਾਪਸ append_ਸਿਰ_ref(ref, cb_ਅੰਕੜੇ);
	ਜੇ (starts_with(ref->name, "refs/tags/"))
		ਵਾਪਸ append_ਟੈਗ_ref(ref->name, ref->oid, ref->flags, cb_ਅੰਕੜੇ);
	ਵਾਪਸ append_ref(ref->name, ref->oid, 0);
}

ਸਥਿਰ ਖਾਲੀ snarf_refs(ਪੂਰਨ ਸਿਰ, ਪੂਰਨ remotes)
{
	ਜੇ (ਸਿਰ) {
		ਪੂਰਨ orig_cnt = ref_name_cnt;

		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_ਸਿਰ_ref, ਨਿਰਾ);
		ਛਾਂਟੋ_ref_range(orig_cnt, ref_name_cnt);
	}
	ਜੇ (remotes) {
		ਪੂਰਨ orig_cnt = ref_name_cnt;

		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_remote_ref, ਨਿਰਾ);
		ਛਾਂਟੋ_ref_range(orig_cnt, ref_name_cnt);
	}
}

ਸਥਿਰ ਪੂਰਨ rev_is_ਸਿਰ(ਅਚਲ ਅੱਖਰ *ਸਿਰ, ਅਚਲ ਅੱਖਰ *name)
{
	ਜੇ (!ਸਿਰ)
		ਵਾਪਸ 0;
	skip_ਅਗੇਤਰ(ਸਿਰ, "refs/heads/", &ਸਿਰ);
	ਜੇ (!skip_ਅਗੇਤਰ(name, "refs/heads/", &name))
		skip_ਅਗੇਤਰ(name, "heads/", &name);
	ਵਾਪਸ !strcmp(ਸਿਰ, name);
}

ਸਥਿਰ ਪੂਰਨ show_ਮਿਲਾਓ_ਅਧਾਰ(ਅਚਲ ਬਣਤਰ commit_list *ਵੇਖਿਆ, ਪੂਰਨ ਅੰਕ_rev)
{
	ਪੂਰਨ all_ਨਕਾਬ = ((1u << (REV_ਖਿਸਕਾਓ + ਅੰਕ_rev)) - 1);
	ਪੂਰਨ all_revs = all_ਨਕਾਬ & ~((1u << REV_ਖਿਸਕਾਓ) - 1);
	ਪੂਰਨ exit_ਸਥਿਤੀ = 1;

	ਲਈ (ਅਚਲ ਬਣਤਰ commit_list *s = ਵੇਖਿਆ; s; s = s->ਅਗਲਾ) {
		ਬਣਤਰ commit *commit = s->ਵਸਤੂ;
		ਪੂਰਨ flags = commit->object.flags & all_ਨਕਾਬ;
		ਜੇ (!(flags & UNINTERESTING) &&
		    ((flags & all_revs) == all_revs)) {
			puts(oid_ਮੰਜ਼ਿਲ_hex(&commit->object.oid));
			exit_ਸਥਿਤੀ = 0;
			commit->object.flags |= UNINTERESTING;
		}
	}
	ਵਾਪਸ exit_ਸਥਿਤੀ;
}

ਸਥਿਰ ਪੂਰਨ show_independent(ਬਣਤਰ commit **rev,
			    ਪੂਰਨ ਅੰਕ_rev,
			    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *rev_ਨਕਾਬ)
{
	ਪੂਰਨ i;

	ਲਈ (i = 0; i < ਅੰਕ_rev; i++) {
		ਬਣਤਰ commit *commit = rev[i];
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flag = rev_ਨਕਾਬ[i];

		ਜੇ (commit->object.flags == flag)
			puts(oid_ਮੰਜ਼ਿਲ_hex(&commit->object.oid));
		commit->object.flags |= UNINTERESTING;
	}
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ append_one_rev(ਅਚਲ ਅੱਖਰ *av)
{
	ਬਣਤਰ object_id revkey;
	ਜੇ (!repo_get_oid(the_repository, av, &revkey)) {
		append_ref(av, &revkey, 0);
		ਵਾਪਸ;
	}
	ਜੇ (strpbrk(av, "*?[")) {
		/* glob style match */
		ਪੂਰਨ saved_matches = ref_name_cnt;

		ਮੇਲ_ref_ਨਮੂਨਾ = av;
		ਮੇਲ_ref_slash = ਗਿਣਤੀ_slashes(av);
		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_matching_ref, ਨਿਰਾ);
		ਜੇ (saved_matches == ref_name_cnt &&
		    ref_name_cnt < MAX_REVS)
			error(_("no matching refs with %s"), av);
		ਛਾਂਟੋ_ref_range(saved_matches, ref_name_cnt);
		ਵਾਪਸ;
	}
	die("bad sha1 reference %s", av);
}

ਸਥਿਰ ਪੂਰਨ git_show_branch_config(ਅਚਲ ਅੱਖਰ *var, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ,
				  ਅਚਲ ਬਣਤਰ config_context *ctx, ਖਾਲੀ *cb)
{
	ਜੇ (!strcmp(var, "showbranch.default")) {
		ਜੇ (!ਮੁੱਲ)
			ਵਾਪਸ config_error_nonbool(var);
		/*
		 * default_arg is now passed to parse_options(), so we need to
		 * mimic the real argv a bit better.
		 */
		ਜੇ (!default_ਦਲੀਲਾਂ.nr)
			strvec_ਧੱਕੋ(&default_ਦਲੀਲਾਂ, "show-branch");
		strvec_ਧੱਕੋ(&default_ਦਲੀਲਾਂ, ਮੁੱਲ);
		ਵਾਪਸ 0;
	}

	ਜੇ (!strcmp(var, "color.showbranch")) {
		showbranch_use_color = git_config_colorbool(var, ਮੁੱਲ);
		ਵਾਪਸ 0;
	}

	ਜੇ (git_color_config(var, ਮੁੱਲ, cb) < 0)
		ਵਾਪਸ -1;

	ਵਾਪਸ git_default_config(var, ਮੁੱਲ, ctx, cb);
}

ਸਥਿਰ ਪੂਰਨ omit_ਵਿੱਚ_dense(ਬਣਤਰ commit *commit, ਬਣਤਰ commit **rev, ਪੂਰਨ n)
{
	/* If the commit is tip of the named branches, do not
	 * omit it.
	 * Otherwise, if it is a merge that is reachable from only one
	 * tip, it is not that interesting.
	 */
	ਪੂਰਨ i, flag, ਗਿਣਤੀ_2;
	ਲਈ (i = 0; i < n; i++)
		ਜੇ (rev[i] == commit)
			ਵਾਪਸ 0;
	flag = commit->object.flags;
	ਲਈ (i = ਗਿਣਤੀ_2 = 0; i < n; i++) {
		ਜੇ (flag & (1u << (i + REV_ਖਿਸਕਾਓ)))
			ਗਿਣਤੀ_2++;
	}
	ਜੇ (ਗਿਣਤੀ_2 == 1)
		ਵਾਪਸ 1;
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ reflog = 0;

ਸਥਿਰ ਪੂਰਨ parse_reflog_param(ਅਚਲ ਬਣਤਰ option *opt, ਅਚਲ ਅੱਖਰ *arg,
			      ਪੂਰਨ unset)
{
	ਅੱਖਰ *ep;
	ਅਚਲ ਅੱਖਰ **ਅਧਾਰ = (ਅਚਲ ਅੱਖਰ **)opt->ਮੁੱਲ;
	BUG_ON_OPT_NEG(unset);
	ਜੇ (!arg)
		arg = "";
	reflog = strtoul(arg, &ep, 10);
	ਜੇ (*ep == ',')
		*ਅਧਾਰ = ep + 1;
	ਨਹੀਂਤਾਂ ਜੇ (*ep)
		ਵਾਪਸ error("unrecognized reflog param '%s'", arg);
	ਨਹੀਂਤਾਂ
		*ਅਧਾਰ = ਨਿਰਾ;
	ਜੇ (reflog <= 0)
		reflog = DEFAULT_REFLOG;
	ਵਾਪਸ 0;
}

ਪੂਰਨ cmd_show_branch(ਪੂਰਨ ac,
		ਅਚਲ ਅੱਖਰ **av,
		ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ commit *rev[MAX_REVS], *commit;
	ਅੱਖਰ *reflog_ਸੁਨੇਹਾ[MAX_REVS] = {0};
	ਬਣਤਰ commit_list *ਵੇਖਿਆ = ਨਿਰਾ;
	ਬਣਤਰ prio_ਕਤਾਰ ਕਤਾਰ = { compare_commits_by_commit_date };
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ rev_ਨਕਾਬ[MAX_REVS];
	ਪੂਰਨ ਅੰਕ_rev, i, extra = 0;
	ਪੂਰਨ all_heads = 0, all_remotes = 0;
	ਪੂਰਨ all_ਨਕਾਬ, all_revs;
	ਗਿਣਤੀ rev_ਛਾਂਟੋ_ਕ੍ਰਮ ਛਾਂਟੋ_ਕ੍ਰਮ = REV_ਛਾਂਟੋ_ਵਿੱਚ_ਗ੍ਰਾਫ_ਕ੍ਰਮ;
	ਅੱਖਰ *ਸਿਰ;
	ਬਣਤਰ object_id ਸਿਰ_oid;
	ਪੂਰਨ ਮਿਲਾਓ_ਅਧਾਰ = 0;
	ਪੂਰਨ independent = 0;
	ਪੂਰਨ no_name = 0;
	ਪੂਰਨ sha1_name = 0;
	ਪੂਰਨ shown_ਮਿਲਾਓ_ਬਿੰਦੂ = 0;
	ਪੂਰਨ with_current_branch = 0;
	ਪੂਰਨ ਸਿਰ_at = -1;
	ਪੂਰਨ topics = 0;
	ਪੂਰਨ sparse = 0;
	ਅਚਲ ਅੱਖਰ *reflog_ਅਧਾਰ = ਨਿਰਾ;
	ਬਣਤਰ option builtin_show_branch_options[] = {
		OPT_BOOL('a', "all", &all_heads,
			 N_("show remote-tracking and local branches")),
		OPT_BOOL('r', "remotes", &all_remotes,
			 N_("show remote-tracking branches")),
		OPT__COLOR(&showbranch_use_color,
			    N_("color '*!+-' corresponding to the branch")),
		{
			.type = OPTION_INTEGER,
			.long_name = "more",
			.ਮੁੱਲ = &extra,
			.precision = ਆਕਾਰ(extra),
			.argh = N_("n"),
			.help = N_("show <n> more commits after the common ancestor"),
			.flags = PARSE_OPT_OPTARG,
			.defval = 1,
		},
		OPT_ਸੈੱਟ_INT(0, "list", &extra, N_("synonym to more=-1"), -1),
		OPT_BOOL(0, "no-name", &no_name, N_("suppress naming strings")),
		OPT_BOOL(0, "current", &with_current_branch,
			 N_("include the current branch")),
		OPT_BOOL(0, "sha1-name", &sha1_name,
			 N_("name commits with their object names")),
		OPT_BOOL(0, "merge-base", &ਮਿਲਾਓ_ਅਧਾਰ,
			 N_("show possible merge bases")),
		OPT_BOOL(0, "independent", &independent,
			    N_("show refs unreachable from any other ref")),
		OPT_ਸੈੱਟ_INT_F(0, "topo-order", &ਛਾਂਟੋ_ਕ੍ਰਮ,
			      N_("show commits in topological order"),
			      REV_ਛਾਂਟੋ_ਵਿੱਚ_ਗ੍ਰਾਫ_ਕ੍ਰਮ, PARSE_OPT_NONEG),
		OPT_BOOL(0, "topics", &topics,
			 N_("show only commits not on the first branch")),
		OPT_ਸੈੱਟ_INT(0, "sparse", &sparse,
			    N_("show merges reachable from only one tip"), 1),
		OPT_ਸੈੱਟ_INT_F(0, "date-order", &ਛਾਂਟੋ_ਕ੍ਰਮ,
			      N_("topologically sort, maintaining date order "
				 "where possible"),
			      REV_ਛਾਂਟੋ_BY_COMMIT_DATE, PARSE_OPT_NONEG),
		OPT_CALLBACK_F('g', "reflog", &reflog_ਅਧਾਰ, N_("<n>[,<base>]"),
			    N_("show <n> most recent ref-log entries starting at "
			       "base"),
			    PARSE_OPT_OPTARG | PARSE_OPT_NONEG,
			    parse_reflog_param),
		OPT_ਸਿਰਾ()
	};
	ਅਚਲ ਅੱਖਰ **ਦਲੀਲਾਂ_copy = ਨਿਰਾ;
	ਪੂਰਨ ret;

	init_commit_name_slab(&name_slab);

	repo_config(the_repository, git_show_branch_config, ਨਿਰਾ);

	/* If nothing is specified, try the default first */
	ਜੇ (ac == 1 && default_ਦਲੀਲਾਂ.nr) {
		DUP_ARRAY(ਦਲੀਲਾਂ_copy, default_ਦਲੀਲਾਂ.v, default_ਦਲੀਲਾਂ.nr);
		ac = default_ਦਲੀਲਾਂ.nr;
		av = ਦਲੀਲਾਂ_copy;
	}

	ac = parse_options(ac, av, ਅਗੇਤਰ, builtin_show_branch_options,
			   show_branch_usage, PARSE_OPT_STOP_AT_NON_OPTION);
	ਜੇ (all_heads)
		all_remotes = 1;

	ਜੇ (extra || reflog) {
		/* "listing" mode is incompatible with
		 * independent nor merge-base modes.
		 */
		ਜੇ (independent || ਮਿਲਾਓ_ਅਧਾਰ)
			usage_with_options(show_branch_usage,
					   builtin_show_branch_options);
		ਜੇ (reflog && ((0 < extra) || all_heads || all_remotes))
			/*
			 * Asking for --more in reflog mode does not
			 * make sense.  --list is Ok.
			 *
			 * Also --all and --remotes do not make sense either.
			 */
			die(_("options '%s' and '%s' cannot be used together"), "--reflog",
				"--all/--remotes/--independent/--merge-base");
	}

	ਜੇ (with_current_branch && reflog)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--reflog", "--current");

	/* If nothing is specified, show all branches by default */
	ਜੇ (ac <= topics && all_heads + all_remotes == 0)
		all_heads = 1;

	ਜੇ (reflog) {
		ਬਣਤਰ object_id oid;
		ਅੱਖਰ *ref;
		ਪੂਰਨ ਅਧਾਰ = 0;
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags = 0;

		ਜੇ (ac == 0) {
			ਸਥਿਰ ਅਚਲ ਅੱਖਰ *fake_av[2];

			fake_av[0] = refs_resolve_refdup(get_main_ref_store(the_repository),
							 "HEAD",
							 RESOLVE_REF_READING,
							 &oid,
							 ਨਿਰਾ);
			fake_av[1] = ਨਿਰਾ;
			av = fake_av;
			ac = 1;
			ਜੇ (!*av)
				die(_("no branches given, and HEAD is not valid"));
		}
		ਜੇ (ac != 1)
			die(_("--reflog option needs one branch name"));

		ਜੇ (MAX_REVS < reflog)
			die(Q_("only %d entry can be shown at one time.",
			       "only %d entries can be shown at one time.",
			       MAX_REVS), MAX_REVS);
		ਜੇ (!repo_dwim_ref(the_repository, *av, ਲੜੀ_ਲੰਬਾਈ(*av), &oid,
				   &ref, 0))
			die(_("no such ref %s"), *av);

		/* Has the base been specified? */
		ਜੇ (reflog_ਅਧਾਰ) {
			ਅੱਖਰ *ep;
			ਅਧਾਰ = strtoul(reflog_ਅਧਾਰ, &ep, 10);
			ਜੇ (*ep) {
				/* Ah, that is a date spec... */
				timestamp_t at;
				at = approxidate(reflog_ਅਧਾਰ);
				read_ref_at(get_main_ref_store(the_repository),
					    ref, flags, at, -1, &oid, ਨਿਰਾ,
					    ਨਿਰਾ, ਨਿਰਾ, &ਅਧਾਰ);
			}
		}

		ਲਈ (i = 0; i < reflog; i++) {
			ਅੱਖਰ *logmsg = ਨਿਰਾ;
			ਅੱਖਰ *nth_desc;
			ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ;
			ਅੱਖਰ *ਸਿਰਾ;
			timestamp_t timestamp;
			ਪੂਰਨ tz;

			ਜੇ (read_ref_at(get_main_ref_store(the_repository),
					ref, flags, 0, ਅਧਾਰ + i, &oid, &logmsg,
					&timestamp, &tz, ਨਿਰਾ)) {
				ਮੁਕਤ(logmsg);
				reflog = i;
				ਰੋਕੋ;
			}

			ਸਿਰਾ = strchr(logmsg, '\n');
			ਜੇ (ਸਿਰਾ)
				*ਸਿਰਾ = '\0';

			ਸੁਨੇਹਾ = (*logmsg == '\0') ? "(none)" : logmsg;
			reflog_ਸੁਨੇਹਾ[i] = xstrfmt("(%s) %s",
						show_date(timestamp, tz,
							  DATE_ਬਹੁਲਕ(RELATIVE)),
						ਸੁਨੇਹਾ);
			ਮੁਕਤ(logmsg);

			nth_desc = xstrfmt("%s@{%d}", *av, ਅਧਾਰ+i);
			append_ref(nth_desc, &oid, 1);
			ਮੁਕਤ(nth_desc);
		}
		ਮੁਕਤ(ref);
	}
	ਨਹੀਂਤਾਂ {
		ਜਦੋਂ (0 < ac) {
			append_one_rev(*av);
			ac--; av++;
		}
		ਜੇ (all_heads + all_remotes)
			snarf_refs(all_heads, all_remotes);
	}

	ਸਿਰ = refs_resolve_refdup(get_main_ref_store(the_repository), "HEAD",
				   RESOLVE_REF_READING,
				   &ਸਿਰ_oid, ਨਿਰਾ);

	ਜੇ (with_current_branch && ਸਿਰ) {
		ਪੂਰਨ has_ਸਿਰ = 0;
		ਲਈ (i = 0; !has_ਸਿਰ && i < ref_name_cnt; i++) {
			/* We are only interested in adding the branch
			 * HEAD points at.
			 */
			ਜੇ (rev_is_ਸਿਰ(ਸਿਰ, ref_name[i]))
				has_ਸਿਰ++;
		}
		ਜੇ (!has_ਸਿਰ) {
			ਅਚਲ ਅੱਖਰ *name = ਸਿਰ;
			skip_ਅਗੇਤਰ(name, "refs/heads/", &name);
			append_one_rev(name);
		}
	}

	ਜੇ (!ref_name_cnt) {
		fprintf(stderr, "No revs to be shown.\n");
		ret = 0;
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	ਲਈ (ਅੰਕ_rev = 0; ref_name[ਅੰਕ_rev]; ਅੰਕ_rev++) {
		ਬਣਤਰ object_id revkey;
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flag = 1u << (ਅੰਕ_rev + REV_ਖਿਸਕਾਓ);

		ਜੇ (MAX_REVS <= ਅੰਕ_rev)
			die(Q_("cannot handle more than %d rev.",
			       "cannot handle more than %d revs.",
			       MAX_REVS), MAX_REVS);
		ਜੇ (repo_get_oid(the_repository, ref_name[ਅੰਕ_rev], &revkey))
			die(_("'%s' is not a valid ref."), ref_name[ਅੰਕ_rev]);
		commit = lookup_commit_reference(the_repository, &revkey);
		ਜੇ (!commit)
			die(_("cannot find commit %s (%s)"),
			    ref_name[ਅੰਕ_rev], oid_ਮੰਜ਼ਿਲ_hex(&revkey));
		repo_parse_commit(the_repository, commit);
		mark_ਵੇਖਿਆ(commit, &ਵੇਖਿਆ);

		/* rev#0 uses bit REV_SHIFT, rev#1 uses bit REV_SHIFT+1,
		 * and so on.  REV_SHIFT bits from bit 0 are used for
		 * internal bookkeeping.
		 */
		commit->object.flags |= flag;
		ਜੇ (commit->object.flags == flag)
			prio_ਕਤਾਰ_put(&ਕਤਾਰ, commit);
		rev[ਅੰਕ_rev] = commit;
	}
	ਲਈ (i = 0; i < ਅੰਕ_rev; i++)
		rev_ਨਕਾਬ[i] = rev[i]->object.flags;

	ਜੇ (0 <= extra)
		join_revs(&ਕਤਾਰ, &ਵੇਖਿਆ, ਅੰਕ_rev, extra);

	commit_list_ਛਾਂਟੋ_by_date(&ਵੇਖਿਆ);

	ਜੇ (ਮਿਲਾਓ_ਅਧਾਰ) {
		ret = show_ਮਿਲਾਓ_ਅਧਾਰ(ਵੇਖਿਆ, ਅੰਕ_rev);
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	ਜੇ (independent) {
		ret = show_independent(rev, ਅੰਕ_rev, rev_ਨਕਾਬ);
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	/* Show list; --more=-1 means list-only */
	ਜੇ (1 < ਅੰਕ_rev || extra < 0) {
		ਲਈ (i = 0; i < ਅੰਕ_rev; i++) {
			ਪੂਰਨ j;
			ਪੂਰਨ is_ਸਿਰ = rev_is_ਸਿਰ(ਸਿਰ, ref_name[i]) &&
				      oideq(&ਸਿਰ_oid, &rev[i]->object.oid);
			ਜੇ (extra < 0)
				ਲਿਖੋ("%c [%s] ",
				       is_ਸਿਰ ? '*' : ' ', ref_name[i]);
			ਨਹੀਂਤਾਂ {
				ਲਈ (j = 0; j < i; j++)
					ਅੱਖਰ_ਲਿਖੋ(' ');
				ਲਿਖੋ("%s%c%s [%s] ",
				       get_color_ਕੋਡ(i),
				       is_ਸਿਰ ? '*' : '!',
				       get_color_reset_ਕੋਡ(), ref_name[i]);
			}

			ਜੇ (!reflog) {
				/* header lines never need name */
				show_one_commit(rev[i], 1);
			}
			ਨਹੀਂਤਾਂ
				puts(reflog_ਸੁਨੇਹਾ[i]);

			ਜੇ (is_ਸਿਰ)
				ਸਿਰ_at = i;
		}
		ਜੇ (0 <= extra) {
			ਲਈ (i = 0; i < ਅੰਕ_rev; i++)
				ਅੱਖਰ_ਲਿਖੋ('-');
			ਅੱਖਰ_ਲਿਖੋ('\n');
		}
	}
	ਜੇ (extra < 0) {
		ret = 0;
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	/* Sort topologically */
	ਛਾਂਟੋ_ਵਿੱਚ_topological_ਕ੍ਰਮ(&ਵੇਖਿਆ, ਛਾਂਟੋ_ਕ੍ਰਮ);

	/* Give names to commits */
	ਜੇ (!sha1_name && !no_name)
		name_commits(ਵੇਖਿਆ, rev, ref_name, ਅੰਕ_rev);

	all_ਨਕਾਬ = ((1u << (REV_ਖਿਸਕਾਓ + ਅੰਕ_rev)) - 1);
	all_revs = all_ਨਕਾਬ & ~((1u << REV_ਖਿਸਕਾਓ) - 1);

	ਲਈ (ਬਣਤਰ commit_list *l = ਵੇਖਿਆ; l; l = l->ਅਗਲਾ) {
		ਬਣਤਰ commit *commit = l->ਵਸਤੂ;
		ਪੂਰਨ this_flag = commit->object.flags;
		ਪੂਰਨ is_ਮਿਲਾਓ_ਬਿੰਦੂ = ((this_flag & all_revs) == all_revs);

		shown_ਮਿਲਾਓ_ਬਿੰਦੂ |= is_ਮਿਲਾਓ_ਬਿੰਦੂ;

		ਜੇ (1 < ਅੰਕ_rev) {
			ਪੂਰਨ is_ਮਿਲਾਓ = !!(commit->parents &&
					  commit->parents->ਅਗਲਾ);
			ਜੇ (topics &&
			    !is_ਮਿਲਾਓ_ਬਿੰਦੂ &&
			    (this_flag & (1u << REV_ਖਿਸਕਾਓ)))
				ਜਾਰੀ;
			ਜੇ (!sparse && is_ਮਿਲਾਓ &&
			    omit_ਵਿੱਚ_dense(commit, rev, ਅੰਕ_rev))
				ਜਾਰੀ;
			ਲਈ (i = 0; i < ਅੰਕ_rev; i++) {
				ਪੂਰਨ mark;
				ਜੇ (!(this_flag & (1u << (i + REV_ਖਿਸਕਾਓ))))
					mark = ' ';
				ਨਹੀਂਤਾਂ ਜੇ (is_ਮਿਲਾਓ)
					mark = '-';
				ਨਹੀਂਤਾਂ ਜੇ (i == ਸਿਰ_at)
					mark = '*';
				ਨਹੀਂਤਾਂ
					mark = '+';
				ਜੇ (mark == ' ')
					ਅੱਖਰ_ਲਿਖੋ(mark);
				ਨਹੀਂਤਾਂ
					ਲਿਖੋ("%s%c%s",
					       get_color_ਕੋਡ(i),
					       mark, get_color_reset_ਕੋਡ());
			}
			ਅੱਖਰ_ਲਿਖੋ(' ');
		}
		show_one_commit(commit, no_name);

		ਜੇ (shown_ਮਿਲਾਓ_ਬਿੰਦੂ && --extra < 0)
			ਰੋਕੋ;
	}

	ret = 0;

ਬਾਹਰਲਾ:
	ਲਈ (ਮਾਪ_t i = 0; i < ARRAY_ਮਾਪ(reflog_ਸੁਨੇਹਾ); i++)
		ਮੁਕਤ(reflog_ਸੁਨੇਹਾ[i]);
	commit_list_free(ਵੇਖਿਆ);
	clear_prio_ਕਤਾਰ(&ਕਤਾਰ);
	ਮੁਕਤ(ਦਲੀਲਾਂ_copy);
	ਮੁਕਤ(ਸਿਰ);
	ਵਾਪਸ ret;
}
