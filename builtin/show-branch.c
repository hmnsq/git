#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "hash.h"
#অন্তর্ভাব "hex.h"
#অন্তর্ভাব "pretty.h"
#অন্তর্ভাব "refs.h"
#অন্তর্ভাব "color.h"
#অন্তর্ভাব "strvec.h"
#অন্তর্ভাব "object-name.h"
#অন্তর্ভাব "parse-options.h"

#অন্তর্ভাব "dir.h"
#অন্তর্ভাব "commit-slab.h"
#অন্তর্ভাব "date.h"
#অন্তর্ভাব "wildmatch.h"
#অন্তর্ভাব "prio-queue.h"

স্থির ধ্রুবক অক্ষর*ধ্রুবক show_branch_usage[] = {
    N_("git show-branch [-a | --all] [-r | --remotes] [--topo-order | --date-order]\n"
       "                [--current] [--color[=<when>] | --no-color] [--sparse]\n"
       "                [--more=<n> | --list | --independent | --merge-base]\n"
       "                [--no-name | --sha1-name] [--topics]\n"
       "                [(<rev> | <glob>)...]"),
    N_("git show-branch (-g | --reflog)[=<n>[,<base>]] [--list] [<ref>]"),
    ফাঁকা
};

স্থির গণনা git_colorbool showbranch_use_color = GIT_COLOR_UNKNOWN;

স্থির কাঠামো strvec default_args = STRVEC_INIT;

/*
 * TODO: convert this use of commit->object.flags to commit-slab
 * instead to store a pointer to ref name directly. Then use the same
 * UNINTERESTING definition from revision.h here.
 */
#define UNINTERESTING	01

#define REV_SHIFT	 2
#define MAX_REVS	(FLAG_BITS - REV_SHIFT) /* should not exceed bits_per_int - REV_SHIFT */

#define DEFAULT_REFLOG	4

স্থির ধ্রুবক অক্ষর *get_color_code(পূর্ণসংখ্যা idx)
{
	যদি (want_color(showbranch_use_color))
		ফেরত column_colors_ansi[idx % column_colors_ansi_max];
	ফেরত "";
}

স্থির ধ্রুবক অক্ষর *get_color_reset_code(শূন্য)
{
	যদি (want_color(showbranch_use_color))
		ফেরত GIT_COLOR_RESET;
	ফেরত "";
}

স্থির কাঠামো commit *interesting(কাঠামো prio_queue *queue)
{
	কাঠামো commit *commit;
	prio_queue_for_each(queue, commit) {
		যদি (!(commit->object.flags & UNINTERESTING))
			ফেরত commit;
	}
	ফেরত ফাঁকা;
}

কাঠামো commit_name {
	ধ্রুবক অক্ষর *শীর্ষ_name; /* which head's ancestor? */
	পূর্ণসংখ্যা generation; /* how many parents away from head_name */
};

define_commit_slab(commit_name_slab, কাঠামো commit_name *);
স্থির কাঠামো commit_name_slab name_slab;

স্থির কাঠামো commit_name *commit_to_name(কাঠামো commit *commit)
{
	ফেরত *commit_name_slab_at(&name_slab, commit);
}


/* Name the commit as nth generation ancestor of head_name;
 * we count only the first-parent relationship for naming purposes.
 */
স্থির শূন্য name_commit(কাঠামো commit *commit, ধ্রুবক অক্ষর *শীর্ষ_name, পূর্ণসংখ্যা nth)
{
	কাঠামো commit_name *name;

	name = *commit_name_slab_at(&name_slab, commit);
	যদি (!name) {
		name = xmalloc(আকার(*name));
		*commit_name_slab_at(&name_slab, commit) = name;
	}
	name->শীর্ষ_name = শীর্ষ_name;
	name->generation = nth;
}

/* Parent is the first parent of the commit.  We may name it
 * as (n+1)th generation ancestor of the same head_name as
 * commit is nth generation ancestor of, if that generation
 * number is better than the name it already has.
 */
স্থির শূন্য name_parent(কাঠামো commit *commit, কাঠামো commit *parent)
{
	কাঠামো commit_name *commit_name = commit_to_name(commit);
	কাঠামো commit_name *parent_name = commit_to_name(parent);
	যদি (!commit_name)
		ফেরত;
	যদি (!parent_name ||
	    commit_name->generation + 1 < parent_name->generation)
		name_commit(parent, commit_name->শীর্ষ_name,
			    commit_name->generation + 1);
}

স্থির পূর্ণসংখ্যা name_first_parent_chain(কাঠামো commit *c)
{
	পূর্ণসংখ্যা i = 0;
	যখন (c) {
		কাঠামো commit *p;
		যদি (!commit_to_name(c))
			বিরতি;
		যদি (!c->parents)
			বিরতি;
		p = c->parents->item;
		যদি (!commit_to_name(p)) {
			name_parent(c, p);
			i++;
		}
		অন্যথা
			বিরতি;
		c = p;
	}
	ফেরত i;
}

স্থির শূন্য name_commits(কাঠামো commit_list *list,
			 কাঠামো commit **rev,
			 অক্ষর **ref_name,
			 পূর্ণসংখ্যা অঙ্ক_rev)
{
	কাঠামো commit_list *cl;
	কাঠামো commit *c;
	পূর্ণসংখ্যা i;

	/* First give names to the given heads */
	জন্য (cl = list; cl; cl = cl->পরবর্তী) {
		c = cl->item;
		যদি (commit_to_name(c))
			চালু;
		জন্য (i = 0; i < অঙ্ক_rev; i++) {
			যদি (rev[i] == c) {
				name_commit(c, ref_name[i], 0);
				বিরতি;
			}
		}
	}

	/* Then commits on the first parent ancestry chain */
	করো {
		i = 0;
		জন্য (cl = list; cl; cl = cl->পরবর্তী) {
			i += name_first_parent_chain(cl->item);
		}
	} যখন (i);

	/* Finally, any unnamed commits */
	করো {
		i = 0;
		জন্য (cl = list; cl; cl = cl->পরবর্তী) {
			কাঠামো commit_list *parents;
			কাঠামো commit_name *n;
			পূর্ণসংখ্যা nth;
			c = cl->item;
			যদি (!commit_to_name(c))
				চালু;
			n = commit_to_name(c);
			parents = c->parents;
			nth = 0;
			যখন (parents) {
				কাঠামো commit *p = parents->item;
				কাঠামো strbuf newname = STRBUF_INIT;
				parents = parents->পরবর্তী;
				nth++;
				যদি (commit_to_name(p))
					চালু;
				নির্বাচন (n->generation) {
				ক্ষেত্র 0:
					strbuf_addstr(&newname, n->শীর্ষ_name);
					বিরতি;
				ক্ষেত্র 1:
					strbuf_addf(&newname, "%s^", n->শীর্ষ_name);
					বিরতি;
				পূর্বনির্ধারিত:
					strbuf_addf(&newname, "%s~%d",
						    n->শীর্ষ_name, n->generation);
					বিরতি;
				}
				যদি (nth == 1)
					strbuf_addch(&newname, '^');
				অন্যথা
					strbuf_addf(&newname, "^%d", nth);
				name_commit(p, strbuf_detach(&newname, ফাঁকা), 0);
				i++;
				name_first_parent_chain(p);
			}
		}
	} যখন (i);
}

স্থির পূর্ণসংখ্যা mark_seen(কাঠামো commit *commit, কাঠামো commit_list **seen_p)
{
	যদি (!commit->object.flags) {
		commit_list_ঢোকাও(commit, seen_p);
		ফেরত 1;
	}
	ফেরত 0;
}

স্থির শূন্য join_revs(কাঠামো prio_queue *queue,
		      কাঠামো commit_list **seen_p,
		      পূর্ণসংখ্যা অঙ্ক_rev, পূর্ণসংখ্যা extra)
{
	পূর্ণসংখ্যা all_mask = ((1u << (REV_SHIFT + অঙ্ক_rev)) - 1);
	পূর্ণসংখ্যা all_revs = all_mask & ~((1u << REV_SHIFT) - 1);
	কাঠামো commit *commit;

	যখন ((commit = prio_queue_peek(queue))) {
		কাঠামো commit_list *parents;
		পূর্ণসংখ্যা still_interesting = !!interesting(queue);
		পূর্ণসংখ্যা flags = commit->object.flags & all_mask;

		যদি (!still_interesting && extra <= 0)
			বিরতি;

		prio_queue_get(queue);

		mark_seen(commit, seen_p);
		যদি ((flags & all_revs) == all_revs)
			flags |= UNINTERESTING;
		parents = commit->parents;

		যখন (parents) {
			কাঠামো commit *p = parents->item;
			পূর্ণসংখ্যা this_flag = p->object.flags;
			parents = parents->পরবর্তী;
			যদি ((this_flag & flags) == flags)
				চালু;
			repo_parse_commit(the_repository, p);
			যদি (mark_seen(p, seen_p) && !still_interesting)
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
	জন্য (;;) {
		পূর্ণসংখ্যা changed = 0;
		কাঠামো commit_list *s;
		জন্য (s = *seen_p; s; s = s->পরবর্তী) {
			কাঠামো commit *c = s->item;
			কাঠামো commit_list *parents;

			যদি (((c->object.flags & all_revs) != all_revs) &&
			    !(c->object.flags & UNINTERESTING))
				চালু;

			/* The current commit is either a merge base or
			 * already uninteresting one.  Mark its parents
			 * as uninteresting commits _only_ if they are
			 * already parsed.  No reason to find new ones
			 * here.
			 */
			parents = c->parents;
			যখন (parents) {
				কাঠামো commit *p = parents->item;
				parents = parents->পরবর্তী;
				যদি (!(p->object.flags & UNINTERESTING)) {
					p->object.flags |= UNINTERESTING;
					changed = 1;
				}
			}
		}
		যদি (!changed)
			বিরতি;
	}
}

স্থির শূন্য show_one_commit(কাঠামো commit *commit, পূর্ণসংখ্যা no_name)
{
	কাঠামো strbuf pretty = STRBUF_INIT;
	ধ্রুবক অক্ষর *pretty_str = "(unavailable)";
	কাঠামো commit_name *name = commit_to_name(commit);

	যদি (commit->object.parsed) {
		pp_commit_easy(CMIT_FMT_ONELINE, commit, &pretty);
		pretty_str = pretty.buf;
	}
	skip_prefix(pretty_str, "[PATCH] ", &pretty_str);

	যদি (!no_name) {
		যদি (name && name->শীর্ষ_name) {
			printf("[%s", name->শীর্ষ_name);
			যদি (name->generation) {
				যদি (name->generation == 1)
					printf("^");
				অন্যথা
					printf("~%d", name->generation);
			}
			printf("] ");
		}
		অন্যথা
			printf("[%s] ",
			       repo_find_unique_abbrev(the_repository, &commit->object.oid,
						       DEFAULT_ABBREV));
	}
	puts(pretty_str);
	strbuf_release(&pretty);
}

স্থির অক্ষর *ref_name[MAX_REVS + 1];
স্থির পূর্ণসংখ্যা ref_name_cnt;

স্থির ধ্রুবক অক্ষর *find_digit_prefix(ধ্রুবক অক্ষর *s, পূর্ণসংখ্যা *v)
{
	ধ্রুবক অক্ষর *p;
	পূর্ণসংখ্যা ver;
	অক্ষর ch;

	জন্য (p = s, ver = 0;
	     '0' <= (ch = *p) && ch <= '9';
	     p++)
		ver = ver * 10 + ch - '0';
	*v = ver;
	ফেরত p;
}


স্থির পূর্ণসংখ্যা version_cmp(ধ্রুবক অক্ষর *a, ধ্রুবক অক্ষর *b)
{
	যখন (1) {
		পূর্ণসংখ্যা va, vb;

		a = find_digit_prefix(a, &va);
		b = find_digit_prefix(b, &vb);
		যদি (va != vb)
			ফেরত va - vb;

		যখন (1) {
			পূর্ণসংখ্যা ca = *a;
			পূর্ণসংখ্যা cb = *b;
			যদি ('0' <= ca && ca <= '9')
				ca = 0;
			যদি ('0' <= cb && cb <= '9')
				cb = 0;
			যদি (ca != cb)
				ফেরত ca - cb;
			যদি (!ca)
				বিরতি;
			a++;
			b++;
		}
		যদি (!*a && !*b)
			ফেরত 0;
	}
}

স্থির পূর্ণসংখ্যা compare_ref_name(ধ্রুবক শূন্য *a_, ধ্রুবক শূন্য *b_)
{
	ধ্রুবক অক্ষর * ধ্রুবক*a = a_, * ধ্রুবক*b = b_;
	ফেরত version_cmp(*a, *b);
}

স্থির শূন্য sort_ref_range(পূর্ণসংখ্যা bottom, পূর্ণসংখ্যা শীর্ষ_2)
{
	QSORT(ref_name + bottom, শীর্ষ_2 - bottom, compare_ref_name);
}

স্থির পূর্ণসংখ্যা append_ref(ধ্রুবক অক্ষর *refname, ধ্রুবক কাঠামো object_id *oid,
		      পূর্ণসংখ্যা allow_dups)
{
	কাঠামো commit *commit = lookup_commit_reference_gently(the_repository,
							       oid, 1);
	পূর্ণসংখ্যা i;

	যদি (!commit)
		ফেরত 0;

	যদি (!allow_dups) {
		/* Avoid adding the same thing twice */
		জন্য (i = 0; i < ref_name_cnt; i++)
			যদি (!strcmp(refname, ref_name[i]))
				ফেরত 0;
	}
	যদি (MAX_REVS <= ref_name_cnt) {
		warning(Q_("ignoring %s; cannot handle more than %d ref",
			   "ignoring %s; cannot handle more than %d refs",
			   MAX_REVS), refname, MAX_REVS);
		ফেরত 0;
	}
	ref_name[ref_name_cnt++] = xstrdup(refname);
	ref_name[ref_name_cnt] = ফাঁকা;
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা append_শীর্ষ_ref(ধ্রুবক কাঠামো reference *ref, শূন্য *cb_তথ্য UNUSED)
{
	কাঠামো object_id tmp;
	পূর্ণসংখ্যা ofs = 11;
	যদি (!starts_with(ref->name, "refs/heads/"))
		ফেরত 0;
	/* If both heads/foo and tags/foo exists, get_sha1 would
	 * get confused.
	 */
	যদি (repo_get_oid(the_repository, ref->name + ofs, &tmp) || !oideq(&tmp, ref->oid))
		ofs = 5;
	ফেরত append_ref(ref->name + ofs, ref->oid, 0);
}

স্থির পূর্ণসংখ্যা append_remote_ref(ধ্রুবক কাঠামো reference *ref, শূন্য *cb_তথ্য UNUSED)
{
	কাঠামো object_id tmp;
	পূর্ণসংখ্যা ofs = 13;
	যদি (!starts_with(ref->name, "refs/remotes/"))
		ফেরত 0;
	/* If both heads/foo and tags/foo exists, get_sha1 would
	 * get confused.
	 */
	যদি (repo_get_oid(the_repository, ref->name + ofs, &tmp) || !oideq(&tmp, ref->oid))
		ofs = 5;
	ফেরত append_ref(ref->name + ofs, ref->oid, 0);
}

স্থির পূর্ণসংখ্যা append_tag_ref(ধ্রুবক অক্ষর *refname, ধ্রুবক কাঠামো object_id *oid,
			  পূর্ণসংখ্যা flag UNUSED, শূন্য *cb_তথ্য UNUSED)
{
	যদি (!starts_with(refname, "refs/tags/"))
		ফেরত 0;
	ফেরত append_ref(refname + 5, oid, 0);
}

স্থির ধ্রুবক অক্ষর *match_ref_pattern = ফাঁকা;
স্থির পূর্ণসংখ্যা match_ref_slash = 0;

স্থির পূর্ণসংখ্যা append_matching_ref(ধ্রুবক কাঠামো reference *ref, শূন্য *cb_তথ্য)
{
	/* we want to allow pattern hold/<asterisk> to show all
	 * branches under refs/heads/hold/, and v0.99.9? to show
	 * refs/tags/v0.99.9a and friends.
	 */
	ধ্রুবক অক্ষর *লেজ;
	পূর্ণসংখ্যা slash = গুনতি_slashes(ref->name);
	জন্য (লেজ = ref->name; *লেজ && match_ref_slash < slash; )
		যদি (*লেজ++ == '/')
			slash--;
	যদি (!*লেজ)
		ফেরত 0;
	যদি (wildmatch(match_ref_pattern, লেজ, 0))
		ফেরত 0;
	যদি (starts_with(ref->name, "refs/heads/"))
		ফেরত append_শীর্ষ_ref(ref, cb_তথ্য);
	যদি (starts_with(ref->name, "refs/tags/"))
		ফেরত append_tag_ref(ref->name, ref->oid, ref->flags, cb_তথ্য);
	ফেরত append_ref(ref->name, ref->oid, 0);
}

স্থির শূন্য snarf_refs(পূর্ণসংখ্যা শীর্ষ, পূর্ণসংখ্যা remotes)
{
	যদি (শীর্ষ) {
		পূর্ণসংখ্যা orig_cnt = ref_name_cnt;

		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_শীর্ষ_ref, ফাঁকা);
		sort_ref_range(orig_cnt, ref_name_cnt);
	}
	যদি (remotes) {
		পূর্ণসংখ্যা orig_cnt = ref_name_cnt;

		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_remote_ref, ফাঁকা);
		sort_ref_range(orig_cnt, ref_name_cnt);
	}
}

স্থির পূর্ণসংখ্যা rev_is_শীর্ষ(ধ্রুবক অক্ষর *শীর্ষ, ধ্রুবক অক্ষর *name)
{
	যদি (!শীর্ষ)
		ফেরত 0;
	skip_prefix(শীর্ষ, "refs/heads/", &শীর্ষ);
	যদি (!skip_prefix(name, "refs/heads/", &name))
		skip_prefix(name, "heads/", &name);
	ফেরত !strcmp(শীর্ষ, name);
}

স্থির পূর্ণসংখ্যা show_merge_base(ধ্রুবক কাঠামো commit_list *seen, পূর্ণসংখ্যা অঙ্ক_rev)
{
	পূর্ণসংখ্যা all_mask = ((1u << (REV_SHIFT + অঙ্ক_rev)) - 1);
	পূর্ণসংখ্যা all_revs = all_mask & ~((1u << REV_SHIFT) - 1);
	পূর্ণসংখ্যা exit_status = 1;

	জন্য (ধ্রুবক কাঠামো commit_list *s = seen; s; s = s->পরবর্তী) {
		কাঠামো commit *commit = s->item;
		পূর্ণসংখ্যা flags = commit->object.flags & all_mask;
		যদি (!(flags & UNINTERESTING) &&
		    ((flags & all_revs) == all_revs)) {
			puts(oid_to_hex(&commit->object.oid));
			exit_status = 0;
			commit->object.flags |= UNINTERESTING;
		}
	}
	ফেরত exit_status;
}

স্থির পূর্ণসংখ্যা show_independent(কাঠামো commit **rev,
			    পূর্ণসংখ্যা অঙ্ক_rev,
			    unsigned পূর্ণসংখ্যা *rev_mask)
{
	পূর্ণসংখ্যা i;

	জন্য (i = 0; i < অঙ্ক_rev; i++) {
		কাঠামো commit *commit = rev[i];
		unsigned পূর্ণসংখ্যা flag = rev_mask[i];

		যদি (commit->object.flags == flag)
			puts(oid_to_hex(&commit->object.oid));
		commit->object.flags |= UNINTERESTING;
	}
	ফেরত 0;
}

স্থির শূন্য append_one_rev(ধ্রুবক অক্ষর *av)
{
	কাঠামো object_id revkey;
	যদি (!repo_get_oid(the_repository, av, &revkey)) {
		append_ref(av, &revkey, 0);
		ফেরত;
	}
	যদি (strpbrk(av, "*?[")) {
		/* glob style match */
		পূর্ণসংখ্যা saved_matches = ref_name_cnt;

		match_ref_pattern = av;
		match_ref_slash = গুনতি_slashes(av);
		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_matching_ref, ফাঁকা);
		যদি (saved_matches == ref_name_cnt &&
		    ref_name_cnt < MAX_REVS)
			error(_("no matching refs with %s"), av);
		sort_ref_range(saved_matches, ref_name_cnt);
		ফেরত;
	}
	die("bad sha1 reference %s", av);
}

স্থির পূর্ণসংখ্যা git_show_branch_config(ধ্রুবক অক্ষর *var, ধ্রুবক অক্ষর *মান,
				  ধ্রুবক কাঠামো config_context *ctx, শূন্য *cb)
{
	যদি (!strcmp(var, "showbranch.default")) {
		যদি (!মান)
			ফেরত config_error_nonbool(var);
		/*
		 * default_arg is now passed to parse_options(), so we need to
		 * mimic the real argv a bit better.
		 */
		যদি (!default_args.nr)
			strvec_ঠেলো(&default_args, "show-branch");
		strvec_ঠেলো(&default_args, মান);
		ফেরত 0;
	}

	যদি (!strcmp(var, "color.showbranch")) {
		showbranch_use_color = git_config_colorbool(var, মান);
		ফেরত 0;
	}

	যদি (git_color_config(var, মান, cb) < 0)
		ফেরত -1;

	ফেরত git_default_config(var, মান, ctx, cb);
}

স্থির পূর্ণসংখ্যা omit_in_dense(কাঠামো commit *commit, কাঠামো commit **rev, পূর্ণসংখ্যা n)
{
	/* If the commit is tip of the named branches, do not
	 * omit it.
	 * Otherwise, if it is a merge that is reachable from only one
	 * tip, it is not that interesting.
	 */
	পূর্ণসংখ্যা i, flag, গুনতি;
	জন্য (i = 0; i < n; i++)
		যদি (rev[i] == commit)
			ফেরত 0;
	flag = commit->object.flags;
	জন্য (i = গুনতি = 0; i < n; i++) {
		যদি (flag & (1u << (i + REV_SHIFT)))
			গুনতি++;
	}
	যদি (গুনতি == 1)
		ফেরত 1;
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা reflog = 0;

স্থির পূর্ণসংখ্যা parse_reflog_param(ধ্রুবক কাঠামো option *opt, ধ্রুবক অক্ষর *arg,
			      পূর্ণসংখ্যা unset)
{
	অক্ষর *ep;
	ধ্রুবক অক্ষর **base = (ধ্রুবক অক্ষর **)opt->মান;
	BUG_ON_OPT_NEG(unset);
	যদি (!arg)
		arg = "";
	reflog = strtoul(arg, &ep, 10);
	যদি (*ep == ',')
		*base = ep + 1;
	অন্যথা যদি (*ep)
		ফেরত error("unrecognized reflog param '%s'", arg);
	অন্যথা
		*base = ফাঁকা;
	যদি (reflog <= 0)
		reflog = DEFAULT_REFLOG;
	ফেরত 0;
}

পূর্ণসংখ্যা cmd_show_branch(পূর্ণসংখ্যা ac,
		ধ্রুবক অক্ষর **av,
		ধ্রুবক অক্ষর *prefix,
		কাঠামো repository *repo UNUSED)
{
	কাঠামো commit *rev[MAX_REVS], *commit;
	অক্ষর *reflog_বার্তা[MAX_REVS] = {0};
	কাঠামো commit_list *seen = ফাঁকা;
	কাঠামো prio_queue queue = { compare_commits_by_commit_date };
	unsigned পূর্ণসংখ্যা rev_mask[MAX_REVS];
	পূর্ণসংখ্যা অঙ্ক_rev, i, extra = 0;
	পূর্ণসংখ্যা all_heads = 0, all_remotes = 0;
	পূর্ণসংখ্যা all_mask, all_revs;
	গণনা rev_sort_order sort_order = REV_SORT_IN_GRAPH_ORDER;
	অক্ষর *শীর্ষ;
	কাঠামো object_id শীর্ষ_oid;
	পূর্ণসংখ্যা merge_base = 0;
	পূর্ণসংখ্যা independent = 0;
	পূর্ণসংখ্যা no_name = 0;
	পূর্ণসংখ্যা sha1_name = 0;
	পূর্ণসংখ্যা shown_merge_point = 0;
	পূর্ণসংখ্যা with_current_branch = 0;
	পূর্ণসংখ্যা শীর্ষ_at = -1;
	পূর্ণসংখ্যা topics = 0;
	পূর্ণসংখ্যা sparse = 0;
	ধ্রুবক অক্ষর *reflog_base = ফাঁকা;
	কাঠামো option builtin_show_branch_options[] = {
		OPT_BOOL('a', "all", &all_heads,
			 N_("show remote-tracking and local branches")),
		OPT_BOOL('r', "remotes", &all_remotes,
			 N_("show remote-tracking branches")),
		OPT__COLOR(&showbranch_use_color,
			    N_("color '*!+-' corresponding to the branch")),
		{
			.type = OPTION_INTEGER,
			.long_name = "more",
			.মান = &extra,
			.precision = আকার(extra),
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
	ধ্রুবক অক্ষর **args_copy = ফাঁকা;
	পূর্ণসংখ্যা ret;

	init_commit_name_slab(&name_slab);

	repo_config(the_repository, git_show_branch_config, ফাঁকা);

	/* If nothing is specified, try the default first */
	যদি (ac == 1 && default_args.nr) {
		DUP_ARRAY(args_copy, default_args.v, default_args.nr);
		ac = default_args.nr;
		av = args_copy;
	}

	ac = parse_options(ac, av, prefix, builtin_show_branch_options,
			   show_branch_usage, PARSE_OPT_STOP_AT_NON_OPTION);
	যদি (all_heads)
		all_remotes = 1;

	যদি (extra || reflog) {
		/* "listing" mode is incompatible with
		 * independent nor merge-base modes.
		 */
		যদি (independent || merge_base)
			usage_with_options(show_branch_usage,
					   builtin_show_branch_options);
		যদি (reflog && ((0 < extra) || all_heads || all_remotes))
			/*
			 * Asking for --more in reflog mode does not
			 * make sense.  --list is Ok.
			 *
			 * Also --all and --remotes do not make sense either.
			 */
			die(_("options '%s' and '%s' cannot be used together"), "--reflog",
				"--all/--remotes/--independent/--merge-base");
	}

	যদি (with_current_branch && reflog)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--reflog", "--current");

	/* If nothing is specified, show all branches by default */
	যদি (ac <= topics && all_heads + all_remotes == 0)
		all_heads = 1;

	যদি (reflog) {
		কাঠামো object_id oid;
		অক্ষর *ref;
		পূর্ণসংখ্যা base = 0;
		unsigned পূর্ণসংখ্যা flags = 0;

		যদি (ac == 0) {
			স্থির ধ্রুবক অক্ষর *fake_av[2];

			fake_av[0] = refs_resolve_refdup(get_main_ref_store(the_repository),
							 "HEAD",
							 RESOLVE_REF_READING,
							 &oid,
							 ফাঁকা);
			fake_av[1] = ফাঁকা;
			av = fake_av;
			ac = 1;
			যদি (!*av)
				die(_("no branches given, and HEAD is not valid"));
		}
		যদি (ac != 1)
			die(_("--reflog option needs one branch name"));

		যদি (MAX_REVS < reflog)
			die(Q_("only %d entry can be shown at one time.",
			       "only %d entries can be shown at one time.",
			       MAX_REVS), MAX_REVS);
		যদি (!repo_dwim_ref(the_repository, *av, strlen(*av), &oid,
				   &ref, 0))
			die(_("no such ref %s"), *av);

		/* Has the base been specified? */
		যদি (reflog_base) {
			অক্ষর *ep;
			base = strtoul(reflog_base, &ep, 10);
			যদি (*ep) {
				/* Ah, that is a date spec... */
				timestamp_t at;
				at = approxidate(reflog_base);
				read_ref_at(get_main_ref_store(the_repository),
					    ref, flags, at, -1, &oid, ফাঁকা,
					    ফাঁকা, ফাঁকা, &base);
			}
		}

		জন্য (i = 0; i < reflog; i++) {
			অক্ষর *logmsg = ফাঁকা;
			অক্ষর *nth_desc;
			ধ্রুবক অক্ষর *বার্তা;
			অক্ষর *end;
			timestamp_t timestamp;
			পূর্ণসংখ্যা tz;

			যদি (read_ref_at(get_main_ref_store(the_repository),
					ref, flags, 0, base + i, &oid, &logmsg,
					&timestamp, &tz, ফাঁকা)) {
				মুক্ত(logmsg);
				reflog = i;
				বিরতি;
			}

			end = strchr(logmsg, '\n');
			যদি (end)
				*end = '\0';

			বার্তা = (*logmsg == '\0') ? "(none)" : logmsg;
			reflog_বার্তা[i] = xstrfmt("(%s) %s",
						show_date(timestamp, tz,
							  DATE_MODE(RELATIVE)),
						বার্তা);
			মুক্ত(logmsg);

			nth_desc = xstrfmt("%s@{%d}", *av, base+i);
			append_ref(nth_desc, &oid, 1);
			মুক্ত(nth_desc);
		}
		মুক্ত(ref);
	}
	অন্যথা {
		যখন (0 < ac) {
			append_one_rev(*av);
			ac--; av++;
		}
		যদি (all_heads + all_remotes)
			snarf_refs(all_heads, all_remotes);
	}

	শীর্ষ = refs_resolve_refdup(get_main_ref_store(the_repository), "HEAD",
				   RESOLVE_REF_READING,
				   &শীর্ষ_oid, ফাঁকা);

	যদি (with_current_branch && শীর্ষ) {
		পূর্ণসংখ্যা has_শীর্ষ = 0;
		জন্য (i = 0; !has_শীর্ষ && i < ref_name_cnt; i++) {
			/* We are only interested in adding the branch
			 * HEAD points at.
			 */
			যদি (rev_is_শীর্ষ(শীর্ষ, ref_name[i]))
				has_শীর্ষ++;
		}
		যদি (!has_শীর্ষ) {
			ধ্রুবক অক্ষর *name = শীর্ষ;
			skip_prefix(name, "refs/heads/", &name);
			append_one_rev(name);
		}
	}

	যদি (!ref_name_cnt) {
		fprintf(stderr, "No revs to be shown.\n");
		ret = 0;
		যাও out;
	}

	জন্য (অঙ্ক_rev = 0; ref_name[অঙ্ক_rev]; অঙ্ক_rev++) {
		কাঠামো object_id revkey;
		unsigned পূর্ণসংখ্যা flag = 1u << (অঙ্ক_rev + REV_SHIFT);

		যদি (MAX_REVS <= অঙ্ক_rev)
			die(Q_("cannot handle more than %d rev.",
			       "cannot handle more than %d revs.",
			       MAX_REVS), MAX_REVS);
		যদি (repo_get_oid(the_repository, ref_name[অঙ্ক_rev], &revkey))
			die(_("'%s' is not a valid ref."), ref_name[অঙ্ক_rev]);
		commit = lookup_commit_reference(the_repository, &revkey);
		যদি (!commit)
			die(_("cannot find commit %s (%s)"),
			    ref_name[অঙ্ক_rev], oid_to_hex(&revkey));
		repo_parse_commit(the_repository, commit);
		mark_seen(commit, &seen);

		/* rev#0 uses bit REV_SHIFT, rev#1 uses bit REV_SHIFT+1,
		 * and so on.  REV_SHIFT bits from bit 0 are used for
		 * internal bookkeeping.
		 */
		commit->object.flags |= flag;
		যদি (commit->object.flags == flag)
			prio_queue_put(&queue, commit);
		rev[অঙ্ক_rev] = commit;
	}
	জন্য (i = 0; i < অঙ্ক_rev; i++)
		rev_mask[i] = rev[i]->object.flags;

	যদি (0 <= extra)
		join_revs(&queue, &seen, অঙ্ক_rev, extra);

	commit_list_sort_by_date(&seen);

	যদি (merge_base) {
		ret = show_merge_base(seen, অঙ্ক_rev);
		যাও out;
	}

	যদি (independent) {
		ret = show_independent(rev, অঙ্ক_rev, rev_mask);
		যাও out;
	}

	/* Show list; --more=-1 means list-only */
	যদি (1 < অঙ্ক_rev || extra < 0) {
		জন্য (i = 0; i < অঙ্ক_rev; i++) {
			পূর্ণসংখ্যা j;
			পূর্ণসংখ্যা is_শীর্ষ = rev_is_শীর্ষ(শীর্ষ, ref_name[i]) &&
				      oideq(&শীর্ষ_oid, &rev[i]->object.oid);
			যদি (extra < 0)
				printf("%c [%s] ",
				       is_শীর্ষ ? '*' : ' ', ref_name[i]);
			অন্যথা {
				জন্য (j = 0; j < i; j++)
					putchar(' ');
				printf("%s%c%s [%s] ",
				       get_color_code(i),
				       is_শীর্ষ ? '*' : '!',
				       get_color_reset_code(), ref_name[i]);
			}

			যদি (!reflog) {
				/* header lines never need name */
				show_one_commit(rev[i], 1);
			}
			অন্যথা
				puts(reflog_বার্তা[i]);

			যদি (is_শীর্ষ)
				শীর্ষ_at = i;
		}
		যদি (0 <= extra) {
			জন্য (i = 0; i < অঙ্ক_rev; i++)
				putchar('-');
			putchar('\n');
		}
	}
	যদি (extra < 0) {
		ret = 0;
		যাও out;
	}

	/* Sort topologically */
	sort_in_topological_order(&seen, sort_order);

	/* Give names to commits */
	যদি (!sha1_name && !no_name)
		name_commits(seen, rev, ref_name, অঙ্ক_rev);

	all_mask = ((1u << (REV_SHIFT + অঙ্ক_rev)) - 1);
	all_revs = all_mask & ~((1u << REV_SHIFT) - 1);

	জন্য (কাঠামো commit_list *l = seen; l; l = l->পরবর্তী) {
		কাঠামো commit *commit = l->item;
		পূর্ণসংখ্যা this_flag = commit->object.flags;
		পূর্ণসংখ্যা is_merge_point = ((this_flag & all_revs) == all_revs);

		shown_merge_point |= is_merge_point;

		যদি (1 < অঙ্ক_rev) {
			পূর্ণসংখ্যা is_merge = !!(commit->parents &&
					  commit->parents->পরবর্তী);
			যদি (topics &&
			    !is_merge_point &&
			    (this_flag & (1u << REV_SHIFT)))
				চালু;
			যদি (!sparse && is_merge &&
			    omit_in_dense(commit, rev, অঙ্ক_rev))
				চালু;
			জন্য (i = 0; i < অঙ্ক_rev; i++) {
				পূর্ণসংখ্যা mark;
				যদি (!(this_flag & (1u << (i + REV_SHIFT))))
					mark = ' ';
				অন্যথা যদি (is_merge)
					mark = '-';
				অন্যথা যদি (i == শীর্ষ_at)
					mark = '*';
				অন্যথা
					mark = '+';
				যদি (mark == ' ')
					putchar(mark);
				অন্যথা
					printf("%s%c%s",
					       get_color_code(i),
					       mark, get_color_reset_code());
			}
			putchar(' ');
		}
		show_one_commit(commit, no_name);

		যদি (shown_merge_point && --extra < 0)
			বিরতি;
	}

	ret = 0;

out:
	জন্য (আকার_t i = 0; i < ARRAY_আকার(reflog_বার্তা); i++)
		মুক্ত(reflog_বার্তা[i]);
	commit_list_free(seen);
	clear_prio_queue(&queue);
	মুক্ত(args_copy);
	মুক্ত(শীর্ষ);
	ফেরত ret;
}
