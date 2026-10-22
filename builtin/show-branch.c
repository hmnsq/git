#وصف_ڪريو USE_THE_REPOSITORY_VARIABLE
#وصف_ڪريو DISABLE_SIGN_COMPARE_WARNINGS

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

ساكن بنية strvec default_دليلون = STRVEC_INIT;

/*
 * TODO: convert this use of commit->object.flags to commit-slab
 * instead to store a pointer to ref name directly. Then use the same
 * UNINTERESTING definition from revision.h here.
 */
#وصف_ڪريو UNINTERESTING	01

#وصف_ڪريو REV_سرڪايو	 2
#وصف_ڪريو MAX_REVS	(FLAG_بٽ - REV_سرڪايو) /* should not exceed bits_per_int - REV_SHIFT */

#وصف_ڪريو DEFAULT_REFLOG	4

ساكن ثابت حرف *get_color_ڪوڊ(صحيح idx)
{
	إذا (want_color(showbranch_use_color))
		أرجع ڪالم_colors_ansi[idx % ڪالم_colors_ansi_max];
	أرجع "";
}

ساكن ثابت حرف *get_color_reset_ڪوڊ(فراغ)
{
	إذا (want_color(showbranch_use_color))
		أرجع GIT_COLOR_RESET;
	أرجع "";
}

ساكن بنية commit *interesting(بنية prio_قطار *قطار)
{
	بنية commit *commit;
	prio_قطار_for_each(قطار, commit) {
		إذا (!(commit->object.flags & UNINTERESTING))
			أرجع commit;
	}
	أرجع عدم;
}

بنية commit_name {
	ثابت حرف *سر_name; /* which head's ancestor? */
	صحيح generation; /* how many parents away from head_name */
};

define_commit_slab(commit_name_slab, بنية commit_name *);
ساكن بنية commit_name_slab name_slab;

ساكن بنية commit_name *commit_منزل_name(بنية commit *commit)
{
	أرجع *commit_name_slab_at(&name_slab, commit);
}


/* Name the commit as nth generation ancestor of head_name;
 * we count only the first-parent relationship for naming purposes.
 */
ساكن فراغ name_commit(بنية commit *commit, ثابت حرف *سر_name, صحيح nth)
{
	بنية commit_name *name;

	name = *commit_name_slab_at(&name_slab, commit);
	إذا (!name) {
		name = xmalloc(حجم(*name));
		*commit_name_slab_at(&name_slab, commit) = name;
	}
	name->سر_name = سر_name;
	name->generation = nth;
}

/* Parent is the first parent of the commit.  We may name it
 * as (n+1)th generation ancestor of the same head_name as
 * commit is nth generation ancestor of, if that generation
 * number is better than the name it already has.
 */
ساكن فراغ name_والد(بنية commit *commit, بنية commit *والد)
{
	بنية commit_name *commit_name = commit_منزل_name(commit);
	بنية commit_name *والد_name = commit_منزل_name(والد);
	إذا (!commit_name)
		أرجع;
	إذا (!والد_name ||
	    commit_name->generation + 1 < والد_name->generation)
		name_commit(والد, commit_name->سر_name,
			    commit_name->generation + 1);
}

ساكن صحيح name_پهريون_والد_chain(بنية commit *c)
{
	صحيح i = 0;
	طالما (c) {
		بنية commit *p;
		إذا (!commit_منزل_name(c))
			اكسر;
		إذا (!c->parents)
			اكسر;
		p = c->parents->شي;
		إذا (!commit_منزل_name(p)) {
			name_والد(c, p);
			i++;
		}
		وإلا
			اكسر;
		c = p;
	}
	أرجع i;
}

ساكن فراغ name_commits(بنية commit_list *list,
			 بنية commit **rev,
			 حرف **ref_name,
			 صحيح انگ_rev)
{
	بنية commit_list *cl;
	بنية commit *c;
	صحيح i;

	/* First give names to the given heads */
	لكل (cl = list; cl; cl = cl->ايندڙ) {
		c = cl->شي;
		إذا (commit_منزل_name(c))
			استمر;
		لكل (i = 0; i < انگ_rev; i++) {
			إذا (rev[i] == c) {
				name_commit(c, ref_name[i], 0);
				اكسر;
			}
		}
	}

	/* Then commits on the first parent ancestry chain */
	افعل {
		i = 0;
		لكل (cl = list; cl; cl = cl->ايندڙ) {
			i += name_پهريون_والد_chain(cl->شي);
		}
	} طالما (i);

	/* Finally, any unnamed commits */
	افعل {
		i = 0;
		لكل (cl = list; cl; cl = cl->ايندڙ) {
			بنية commit_list *parents;
			بنية commit_name *n;
			صحيح nth;
			c = cl->شي;
			إذا (!commit_منزل_name(c))
				استمر;
			n = commit_منزل_name(c);
			parents = c->parents;
			nth = 0;
			طالما (parents) {
				بنية commit *p = parents->شي;
				بنية strbuf newname = STRBUF_INIT;
				parents = parents->ايندڙ;
				nth++;
				إذا (commit_منزل_name(p))
					استمر;
				بدل (n->generation) {
				حالة 0:
					strbuf_addstr(&newname, n->سر_name);
					اكسر;
				حالة 1:
					strbuf_addf(&newname, "%s^", n->سر_name);
					اكسر;
				افتراضي:
					strbuf_addf(&newname, "%s~%d",
						    n->سر_name, n->generation);
					اكسر;
				}
				إذا (nth == 1)
					strbuf_addch(&newname, '^');
				وإلا
					strbuf_addf(&newname, "^%d", nth);
				name_commit(p, strbuf_detach(&newname, عدم), 0);
				i++;
				name_پهريون_والد_chain(p);
			}
		}
	} طالما (i);
}

ساكن صحيح mark_ڏٺل(بنية commit *commit, بنية commit_list **ڏٺل_p)
{
	إذا (!commit->object.flags) {
		commit_list_وجهو(commit, ڏٺل_p);
		أرجع 1;
	}
	أرجع 0;
}

ساكن فراغ join_revs(بنية prio_قطار *قطار,
		      بنية commit_list **ڏٺل_p,
		      صحيح انگ_rev, صحيح extra)
{
	صحيح all_نقاب = ((1u << (REV_سرڪايو + انگ_rev)) - 1);
	صحيح all_revs = all_نقاب & ~((1u << REV_سرڪايو) - 1);
	بنية commit *commit;

	طالما ((commit = prio_قطار_peek(قطار))) {
		بنية commit_list *parents;
		صحيح still_interesting = !!interesting(قطار);
		صحيح flags = commit->object.flags & all_نقاب;

		إذا (!still_interesting && extra <= 0)
			اكسر;

		prio_قطار_get(قطار);

		mark_ڏٺل(commit, ڏٺل_p);
		إذا ((flags & all_revs) == all_revs)
			flags |= UNINTERESTING;
		parents = commit->parents;

		طالما (parents) {
			بنية commit *p = parents->شي;
			صحيح this_flag = p->object.flags;
			parents = parents->ايندڙ;
			إذا ((this_flag & flags) == flags)
				استمر;
			repo_parse_commit(the_repository, p);
			إذا (mark_ڏٺل(p, ڏٺل_p) && !still_interesting)
				extra--;
			p->object.flags |= flags;
			prio_قطار_put(قطار, p);
		}
	}

	/*
	 * Postprocess to complete well-poisoning.
	 *
	 * At this point we have all the commits we have seen in
	 * seen_p list.  Mark anything that can be reached from
	 * uninteresting commits not interesting.
	 */
	لكل (;;) {
		صحيح بدليو = 0;
		بنية commit_list *s;
		لكل (s = *ڏٺل_p; s; s = s->ايندڙ) {
			بنية commit *c = s->شي;
			بنية commit_list *parents;

			إذا (((c->object.flags & all_revs) != all_revs) &&
			    !(c->object.flags & UNINTERESTING))
				استمر;

			/* The current commit is either a merge base or
			 * already uninteresting one.  Mark its parents
			 * as uninteresting commits _only_ if they are
			 * already parsed.  No reason to find new ones
			 * here.
			 */
			parents = c->parents;
			طالما (parents) {
				بنية commit *p = parents->شي;
				parents = parents->ايندڙ;
				إذا (!(p->object.flags & UNINTERESTING)) {
					p->object.flags |= UNINTERESTING;
					بدليو = 1;
				}
			}
		}
		إذا (!بدليو)
			اكسر;
	}
}

ساكن فراغ show_one_commit(بنية commit *commit, صحيح no_name)
{
	بنية strbuf pretty = STRBUF_INIT;
	ثابت حرف *pretty_str = "(unavailable)";
	بنية commit_name *name = commit_منزل_name(commit);

	إذا (commit->object.parsed) {
		pp_commit_easy(CMIT_FMT_ONELINE, commit, &pretty);
		pretty_str = pretty.buf;
	}
	skip_اڳياڙي(pretty_str, "[PATCH] ", &pretty_str);

	إذا (!no_name) {
		إذا (name && name->سر_name) {
			لکو("[%s", name->سر_name);
			إذا (name->generation) {
				إذا (name->generation == 1)
					لکو("^");
				وإلا
					لکو("~%d", name->generation);
			}
			لکو("] ");
		}
		وإلا
			لکو("[%s] ",
			       repo_ڳوليو_unique_abbrev(the_repository, &commit->object.oid,
						       DEFAULT_ABBREV));
	}
	puts(pretty_str);
	strbuf_release(&pretty);
}

ساكن حرف *ref_name[MAX_REVS + 1];
ساكن صحيح ref_name_cnt;

ساكن ثابت حرف *ڳوليو_digit_اڳياڙي(ثابت حرف *s, صحيح *v)
{
	ثابت حرف *p;
	صحيح ver;
	حرف ch;

	لكل (p = s, ver = 0;
	     '0' <= (ch = *p) && ch <= '9';
	     p++)
		ver = ver * 10 + ch - '0';
	*v = ver;
	أرجع p;
}


ساكن صحيح نسخو_cmp(ثابت حرف *a, ثابت حرف *b)
{
	طالما (1) {
		صحيح va, vb;

		a = ڳوليو_digit_اڳياڙي(a, &va);
		b = ڳوليو_digit_اڳياڙي(b, &vb);
		إذا (va != vb)
			أرجع va - vb;

		طالما (1) {
			صحيح ca = *a;
			صحيح cb = *b;
			إذا ('0' <= ca && ca <= '9')
				ca = 0;
			إذا ('0' <= cb && cb <= '9')
				cb = 0;
			إذا (ca != cb)
				أرجع ca - cb;
			إذا (!ca)
				اكسر;
			a++;
			b++;
		}
		إذا (!*a && !*b)
			أرجع 0;
	}
}

ساكن صحيح compare_ref_name(ثابت فراغ *a_, ثابت فراغ *b_)
{
	ثابت حرف * ثابت*a = a_, * ثابت*b = b_;
	أرجع نسخو_cmp(*a, *b);
}

ساكن فراغ ترتيب_ڏيو_ref_range(صحيح bottom, صحيح چوٽي)
{
	QSORT(ref_name + bottom, چوٽي - bottom, compare_ref_name);
}

ساكن صحيح append_ref(ثابت حرف *refname, ثابت بنية object_id *oid,
		      صحيح allow_dups)
{
	بنية commit *commit = lookup_commit_reference_gently(the_repository,
							       oid, 1);
	صحيح i;

	إذا (!commit)
		أرجع 0;

	إذا (!allow_dups) {
		/* Avoid adding the same thing twice */
		لكل (i = 0; i < ref_name_cnt; i++)
			إذا (!strcmp(refname, ref_name[i]))
				أرجع 0;
	}
	إذا (MAX_REVS <= ref_name_cnt) {
		warning(Q_("ignoring %s; cannot handle more than %d ref",
			   "ignoring %s; cannot handle more than %d refs",
			   MAX_REVS), refname, MAX_REVS);
		أرجع 0;
	}
	ref_name[ref_name_cnt++] = xstrdup(refname);
	ref_name[ref_name_cnt] = عدم;
	أرجع 0;
}

ساكن صحيح append_سر_ref(ثابت بنية reference *ref, فراغ *cb_معلومات اڻ_استعمال_ٿيل)
{
	بنية object_id tmp;
	صحيح ofs = 11;
	إذا (!starts_with(ref->name, "refs/heads/"))
		أرجع 0;
	/* If both heads/foo and tags/foo exists, get_sha1 would
	 * get confused.
	 */
	إذا (repo_get_oid(the_repository, ref->name + ofs, &tmp) || !oideq(&tmp, ref->oid))
		ofs = 5;
	أرجع append_ref(ref->name + ofs, ref->oid, 0);
}

ساكن صحيح append_remote_ref(ثابت بنية reference *ref, فراغ *cb_معلومات اڻ_استعمال_ٿيل)
{
	بنية object_id tmp;
	صحيح ofs = 13;
	إذا (!starts_with(ref->name, "refs/remotes/"))
		أرجع 0;
	/* If both heads/foo and tags/foo exists, get_sha1 would
	 * get confused.
	 */
	إذا (repo_get_oid(the_repository, ref->name + ofs, &tmp) || !oideq(&tmp, ref->oid))
		ofs = 5;
	أرجع append_ref(ref->name + ofs, ref->oid, 0);
}

ساكن صحيح append_ٽيگ_ref(ثابت حرف *refname, ثابت بنية object_id *oid,
			  صحيح flag اڻ_استعمال_ٿيل, فراغ *cb_معلومات اڻ_استعمال_ٿيل)
{
	إذا (!starts_with(refname, "refs/tags/"))
		أرجع 0;
	أرجع append_ref(refname + 5, oid, 0);
}

ساكن ثابت حرف *ميلاپ_ref_نمونو = عدم;
ساكن صحيح ميلاپ_ref_slash = 0;

ساكن صحيح append_matching_ref(ثابت بنية reference *ref, فراغ *cb_معلومات)
{
	/* we want to allow pattern hold/<asterisk> to show all
	 * branches under refs/heads/hold/, and v0.99.9? to show
	 * refs/tags/v0.99.9a and friends.
	 */
	ثابت حرف *پڇ;
	صحيح slash = ڳڻپ_slashes(ref->name);
	لكل (پڇ = ref->name; *پڇ && ميلاپ_ref_slash < slash; )
		إذا (*پڇ++ == '/')
			slash--;
	إذا (!*پڇ)
		أرجع 0;
	إذا (wildmatch(ميلاپ_ref_نمونو, پڇ, 0))
		أرجع 0;
	إذا (starts_with(ref->name, "refs/heads/"))
		أرجع append_سر_ref(ref, cb_معلومات);
	إذا (starts_with(ref->name, "refs/tags/"))
		أرجع append_ٽيگ_ref(ref->name, ref->oid, ref->flags, cb_معلومات);
	أرجع append_ref(ref->name, ref->oid, 0);
}

ساكن فراغ snarf_refs(صحيح سر, صحيح remotes)
{
	إذا (سر) {
		صحيح orig_cnt = ref_name_cnt;

		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_سر_ref, عدم);
		ترتيب_ڏيو_ref_range(orig_cnt, ref_name_cnt);
	}
	إذا (remotes) {
		صحيح orig_cnt = ref_name_cnt;

		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_remote_ref, عدم);
		ترتيب_ڏيو_ref_range(orig_cnt, ref_name_cnt);
	}
}

ساكن صحيح rev_is_سر(ثابت حرف *سر, ثابت حرف *name)
{
	إذا (!سر)
		أرجع 0;
	skip_اڳياڙي(سر, "refs/heads/", &سر);
	إذا (!skip_اڳياڙي(name, "refs/heads/", &name))
		skip_اڳياڙي(name, "heads/", &name);
	أرجع !strcmp(سر, name);
}

ساكن صحيح show_ملايو_بنياد(ثابت بنية commit_list *ڏٺل, صحيح انگ_rev)
{
	صحيح all_نقاب = ((1u << (REV_سرڪايو + انگ_rev)) - 1);
	صحيح all_revs = all_نقاب & ~((1u << REV_سرڪايو) - 1);
	صحيح exit_حالت = 1;

	لكل (ثابت بنية commit_list *s = ڏٺل; s; s = s->ايندڙ) {
		بنية commit *commit = s->شي;
		صحيح flags = commit->object.flags & all_نقاب;
		إذا (!(flags & UNINTERESTING) &&
		    ((flags & all_revs) == all_revs)) {
			puts(oid_منزل_hex(&commit->object.oid));
			exit_حالت = 0;
			commit->object.flags |= UNINTERESTING;
		}
	}
	أرجع exit_حالت;
}

ساكن صحيح show_independent(بنية commit **rev,
			    صحيح انگ_rev,
			    بنانشان صحيح *rev_نقاب)
{
	صحيح i;

	لكل (i = 0; i < انگ_rev; i++) {
		بنية commit *commit = rev[i];
		بنانشان صحيح flag = rev_نقاب[i];

		إذا (commit->object.flags == flag)
			puts(oid_منزل_hex(&commit->object.oid));
		commit->object.flags |= UNINTERESTING;
	}
	أرجع 0;
}

ساكن فراغ append_one_rev(ثابت حرف *av)
{
	بنية object_id revkey;
	إذا (!repo_get_oid(the_repository, av, &revkey)) {
		append_ref(av, &revkey, 0);
		أرجع;
	}
	إذا (strpbrk(av, "*?[")) {
		/* glob style match */
		صحيح saved_matches = ref_name_cnt;

		ميلاپ_ref_نمونو = av;
		ميلاپ_ref_slash = ڳڻپ_slashes(av);
		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_matching_ref, عدم);
		إذا (saved_matches == ref_name_cnt &&
		    ref_name_cnt < MAX_REVS)
			error(_("no matching refs with %s"), av);
		ترتيب_ڏيو_ref_range(saved_matches, ref_name_cnt);
		أرجع;
	}
	die("bad sha1 reference %s", av);
}

ساكن صحيح git_show_branch_config(ثابت حرف *var, ثابت حرف *قدر,
				  ثابت بنية config_context *ctx, فراغ *cb)
{
	إذا (!strcmp(var, "showbranch.default")) {
		إذا (!قدر)
			أرجع config_error_nonbool(var);
		/*
		 * default_arg is now passed to parse_options(), so we need to
		 * mimic the real argv a bit better.
		 */
		إذا (!default_دليلون.nr)
			strvec_ڌڪو(&default_دليلون, "show-branch");
		strvec_ڌڪو(&default_دليلون, قدر);
		أرجع 0;
	}

	إذا (!strcmp(var, "color.showbranch")) {
		showbranch_use_color = git_config_colorbool(var, قدر);
		أرجع 0;
	}

	إذا (git_color_config(var, قدر, cb) < 0)
		أرجع -1;

	أرجع git_default_config(var, قدر, ctx, cb);
}

ساكن صحيح omit_اندريون_dense(بنية commit *commit, بنية commit **rev, صحيح n)
{
	/* If the commit is tip of the named branches, do not
	 * omit it.
	 * Otherwise, if it is a merge that is reachable from only one
	 * tip, it is not that interesting.
	 */
	صحيح i, flag, ڳڻپ;
	لكل (i = 0; i < n; i++)
		إذا (rev[i] == commit)
			أرجع 0;
	flag = commit->object.flags;
	لكل (i = ڳڻپ = 0; i < n; i++) {
		إذا (flag & (1u << (i + REV_سرڪايو)))
			ڳڻپ++;
	}
	إذا (ڳڻپ == 1)
		أرجع 1;
	أرجع 0;
}

ساكن صحيح reflog = 0;

ساكن صحيح parse_reflog_param(ثابت بنية option *opt, ثابت حرف *arg,
			      صحيح unset)
{
	حرف *ep;
	ثابت حرف **بنياد = (ثابت حرف **)opt->قدر;
	BUG_ON_OPT_NEG(unset);
	إذا (!arg)
		arg = "";
	reflog = strtoul(arg, &ep, 10);
	إذا (*ep == ',')
		*بنياد = ep + 1;
	وإلا إذا (*ep)
		أرجع error("unrecognized reflog param '%s'", arg);
	وإلا
		*بنياد = عدم;
	إذا (reflog <= 0)
		reflog = DEFAULT_REFLOG;
	أرجع 0;
}

صحيح cmd_show_branch(صحيح ac,
		ثابت حرف **av,
		ثابت حرف *اڳياڙي,
		بنية repository *repo اڻ_استعمال_ٿيل)
{
	بنية commit *rev[MAX_REVS], *commit;
	حرف *reflog_نياپو[MAX_REVS] = {0};
	بنية commit_list *ڏٺل = عدم;
	بنية prio_قطار قطار = { compare_commits_by_commit_date };
	بنانشان صحيح rev_نقاب[MAX_REVS];
	صحيح انگ_rev, i, extra = 0;
	صحيح all_heads = 0, all_remotes = 0;
	صحيح all_نقاب, all_revs;
	تعداد rev_ترتيب_ڏيو_ترتيب ترتيب_ڏيو_ترتيب = REV_ترتيب_ڏيو_اندريون_گراف_ترتيب;
	حرف *سر;
	بنية object_id سر_oid;
	صحيح ملايو_بنياد = 0;
	صحيح independent = 0;
	صحيح no_name = 0;
	صحيح sha1_name = 0;
	صحيح shown_ملايو_نقطو = 0;
	صحيح with_current_branch = 0;
	صحيح سر_at = -1;
	صحيح topics = 0;
	صحيح sparse = 0;
	ثابت حرف *reflog_بنياد = عدم;
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
			.قدر = &extra,
			.precision = حجم(extra),
			.argh = N_("n"),
			.help = N_("show <n> more commits after the common ancestor"),
			.flags = PARSE_OPT_OPTARG,
			.defval = 1,
		},
		OPT_سيٽ_INT(0, "list", &extra, N_("synonym to more=-1"), -1),
		OPT_BOOL(0, "no-name", &no_name, N_("suppress naming strings")),
		OPT_BOOL(0, "current", &with_current_branch,
			 N_("include the current branch")),
		OPT_BOOL(0, "sha1-name", &sha1_name,
			 N_("name commits with their object names")),
		OPT_BOOL(0, "merge-base", &ملايو_بنياد,
			 N_("show possible merge bases")),
		OPT_BOOL(0, "independent", &independent,
			    N_("show refs unreachable from any other ref")),
		OPT_سيٽ_INT_F(0, "topo-order", &ترتيب_ڏيو_ترتيب,
			      N_("show commits in topological order"),
			      REV_ترتيب_ڏيو_اندريون_گراف_ترتيب, PARSE_OPT_NONEG),
		OPT_BOOL(0, "topics", &topics,
			 N_("show only commits not on the first branch")),
		OPT_سيٽ_INT(0, "sparse", &sparse,
			    N_("show merges reachable from only one tip"), 1),
		OPT_سيٽ_INT_F(0, "date-order", &ترتيب_ڏيو_ترتيب,
			      N_("topologically sort, maintaining date order "
				 "where possible"),
			      REV_ترتيب_ڏيو_BY_COMMIT_DATE, PARSE_OPT_NONEG),
		OPT_CALLBACK_F('g', "reflog", &reflog_بنياد, N_("<n>[,<base>]"),
			    N_("show <n> most recent ref-log entries starting at "
			       "base"),
			    PARSE_OPT_OPTARG | PARSE_OPT_NONEG,
			    parse_reflog_param),
		OPT_پڇاڙي()
	};
	ثابت حرف **دليلون_copy = عدم;
	صحيح ret;

	init_commit_name_slab(&name_slab);

	repo_config(the_repository, git_show_branch_config, عدم);

	/* If nothing is specified, try the default first */
	إذا (ac == 1 && default_دليلون.nr) {
		DUP_ARRAY(دليلون_copy, default_دليلون.v, default_دليلون.nr);
		ac = default_دليلون.nr;
		av = دليلون_copy;
	}

	ac = parse_options(ac, av, اڳياڙي, builtin_show_branch_options,
			   show_branch_usage, PARSE_OPT_STOP_AT_NON_OPTION);
	إذا (all_heads)
		all_remotes = 1;

	إذا (extra || reflog) {
		/* "listing" mode is incompatible with
		 * independent nor merge-base modes.
		 */
		إذا (independent || ملايو_بنياد)
			usage_with_options(show_branch_usage,
					   builtin_show_branch_options);
		إذا (reflog && ((0 < extra) || all_heads || all_remotes))
			/*
			 * Asking for --more in reflog mode does not
			 * make sense.  --list is Ok.
			 *
			 * Also --all and --remotes do not make sense either.
			 */
			die(_("options '%s' and '%s' cannot be used together"), "--reflog",
				"--all/--remotes/--independent/--merge-base");
	}

	إذا (with_current_branch && reflog)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--reflog", "--current");

	/* If nothing is specified, show all branches by default */
	إذا (ac <= topics && all_heads + all_remotes == 0)
		all_heads = 1;

	إذا (reflog) {
		بنية object_id oid;
		حرف *ref;
		صحيح بنياد = 0;
		بنانشان صحيح flags = 0;

		إذا (ac == 0) {
			ساكن ثابت حرف *fake_av[2];

			fake_av[0] = refs_resolve_refdup(get_main_ref_store(the_repository),
							 "HEAD",
							 RESOLVE_REF_READING,
							 &oid,
							 عدم);
			fake_av[1] = عدم;
			av = fake_av;
			ac = 1;
			إذا (!*av)
				die(_("no branches given, and HEAD is not valid"));
		}
		إذا (ac != 1)
			die(_("--reflog option needs one branch name"));

		إذا (MAX_REVS < reflog)
			die(Q_("only %d entry can be shown at one time.",
			       "only %d entries can be shown at one time.",
			       MAX_REVS), MAX_REVS);
		إذا (!repo_dwim_ref(the_repository, *av, لڙي_ڊيگهه(*av), &oid,
				   &ref, 0))
			die(_("no such ref %s"), *av);

		/* Has the base been specified? */
		إذا (reflog_بنياد) {
			حرف *ep;
			بنياد = strtoul(reflog_بنياد, &ep, 10);
			إذا (*ep) {
				/* Ah, that is a date spec... */
				timestamp_t at;
				at = approxidate(reflog_بنياد);
				read_ref_at(get_main_ref_store(the_repository),
					    ref, flags, at, -1, &oid, عدم,
					    عدم, عدم, &بنياد);
			}
		}

		لكل (i = 0; i < reflog; i++) {
			حرف *logmsg = عدم;
			حرف *nth_desc;
			ثابت حرف *نياپو;
			حرف *پڇاڙي;
			timestamp_t timestamp;
			صحيح tz;

			إذا (read_ref_at(get_main_ref_store(the_repository),
					ref, flags, 0, بنياد + i, &oid, &logmsg,
					&timestamp, &tz, عدم)) {
				حرر(logmsg);
				reflog = i;
				اكسر;
			}

			پڇاڙي = strchr(logmsg, '\n');
			إذا (پڇاڙي)
				*پڇاڙي = '\0';

			نياپو = (*logmsg == '\0') ? "(none)" : logmsg;
			reflog_نياپو[i] = xstrfmt("(%s) %s",
						show_date(timestamp, tz,
							  DATE_گهڻائي(RELATIVE)),
						نياپو);
			حرر(logmsg);

			nth_desc = xstrfmt("%s@{%d}", *av, بنياد+i);
			append_ref(nth_desc, &oid, 1);
			حرر(nth_desc);
		}
		حرر(ref);
	}
	وإلا {
		طالما (0 < ac) {
			append_one_rev(*av);
			ac--; av++;
		}
		إذا (all_heads + all_remotes)
			snarf_refs(all_heads, all_remotes);
	}

	سر = refs_resolve_refdup(get_main_ref_store(the_repository), "HEAD",
				   RESOLVE_REF_READING,
				   &سر_oid, عدم);

	إذا (with_current_branch && سر) {
		صحيح has_سر = 0;
		لكل (i = 0; !has_سر && i < ref_name_cnt; i++) {
			/* We are only interested in adding the branch
			 * HEAD points at.
			 */
			إذا (rev_is_سر(سر, ref_name[i]))
				has_سر++;
		}
		إذا (!has_سر) {
			ثابت حرف *name = سر;
			skip_اڳياڙي(name, "refs/heads/", &name);
			append_one_rev(name);
		}
	}

	إذا (!ref_name_cnt) {
		fprintf(stderr, "No revs to be shown.\n");
		ret = 0;
		اذهب ٻاهريون;
	}

	لكل (انگ_rev = 0; ref_name[انگ_rev]; انگ_rev++) {
		بنية object_id revkey;
		بنانشان صحيح flag = 1u << (انگ_rev + REV_سرڪايو);

		إذا (MAX_REVS <= انگ_rev)
			die(Q_("cannot handle more than %d rev.",
			       "cannot handle more than %d revs.",
			       MAX_REVS), MAX_REVS);
		إذا (repo_get_oid(the_repository, ref_name[انگ_rev], &revkey))
			die(_("'%s' is not a valid ref."), ref_name[انگ_rev]);
		commit = lookup_commit_reference(the_repository, &revkey);
		إذا (!commit)
			die(_("cannot find commit %s (%s)"),
			    ref_name[انگ_rev], oid_منزل_hex(&revkey));
		repo_parse_commit(the_repository, commit);
		mark_ڏٺل(commit, &ڏٺل);

		/* rev#0 uses bit REV_SHIFT, rev#1 uses bit REV_SHIFT+1,
		 * and so on.  REV_SHIFT bits from bit 0 are used for
		 * internal bookkeeping.
		 */
		commit->object.flags |= flag;
		إذا (commit->object.flags == flag)
			prio_قطار_put(&قطار, commit);
		rev[انگ_rev] = commit;
	}
	لكل (i = 0; i < انگ_rev; i++)
		rev_نقاب[i] = rev[i]->object.flags;

	إذا (0 <= extra)
		join_revs(&قطار, &ڏٺل, انگ_rev, extra);

	commit_list_ترتيب_ڏيو_by_date(&ڏٺل);

	إذا (ملايو_بنياد) {
		ret = show_ملايو_بنياد(ڏٺل, انگ_rev);
		اذهب ٻاهريون;
	}

	إذا (independent) {
		ret = show_independent(rev, انگ_rev, rev_نقاب);
		اذهب ٻاهريون;
	}

	/* Show list; --more=-1 means list-only */
	إذا (1 < انگ_rev || extra < 0) {
		لكل (i = 0; i < انگ_rev; i++) {
			صحيح j;
			صحيح is_سر = rev_is_سر(سر, ref_name[i]) &&
				      oideq(&سر_oid, &rev[i]->object.oid);
			إذا (extra < 0)
				لکو("%c [%s] ",
				       is_سر ? '*' : ' ', ref_name[i]);
			وإلا {
				لكل (j = 0; j < i; j++)
					اکر_لکو(' ');
				لکو("%s%c%s [%s] ",
				       get_color_ڪوڊ(i),
				       is_سر ? '*' : '!',
				       get_color_reset_ڪوڊ(), ref_name[i]);
			}

			إذا (!reflog) {
				/* header lines never need name */
				show_one_commit(rev[i], 1);
			}
			وإلا
				puts(reflog_نياپو[i]);

			إذا (is_سر)
				سر_at = i;
		}
		إذا (0 <= extra) {
			لكل (i = 0; i < انگ_rev; i++)
				اکر_لکو('-');
			اکر_لکو('\n');
		}
	}
	إذا (extra < 0) {
		ret = 0;
		اذهب ٻاهريون;
	}

	/* Sort topologically */
	ترتيب_ڏيو_اندريون_topological_ترتيب(&ڏٺل, ترتيب_ڏيو_ترتيب);

	/* Give names to commits */
	إذا (!sha1_name && !no_name)
		name_commits(ڏٺل, rev, ref_name, انگ_rev);

	all_نقاب = ((1u << (REV_سرڪايو + انگ_rev)) - 1);
	all_revs = all_نقاب & ~((1u << REV_سرڪايو) - 1);

	لكل (بنية commit_list *l = ڏٺل; l; l = l->ايندڙ) {
		بنية commit *commit = l->شي;
		صحيح this_flag = commit->object.flags;
		صحيح is_ملايو_نقطو = ((this_flag & all_revs) == all_revs);

		shown_ملايو_نقطو |= is_ملايو_نقطو;

		إذا (1 < انگ_rev) {
			صحيح is_ملايو = !!(commit->parents &&
					  commit->parents->ايندڙ);
			إذا (topics &&
			    !is_ملايو_نقطو &&
			    (this_flag & (1u << REV_سرڪايو)))
				استمر;
			إذا (!sparse && is_ملايو &&
			    omit_اندريون_dense(commit, rev, انگ_rev))
				استمر;
			لكل (i = 0; i < انگ_rev; i++) {
				صحيح mark;
				إذا (!(this_flag & (1u << (i + REV_سرڪايو))))
					mark = ' ';
				وإلا إذا (is_ملايو)
					mark = '-';
				وإلا إذا (i == سر_at)
					mark = '*';
				وإلا
					mark = '+';
				إذا (mark == ' ')
					اکر_لکو(mark);
				وإلا
					لکو("%s%c%s",
					       get_color_ڪوڊ(i),
					       mark, get_color_reset_ڪوڊ());
			}
			اکر_لکو(' ');
		}
		show_one_commit(commit, no_name);

		إذا (shown_ملايو_نقطو && --extra < 0)
			اكسر;
	}

	ret = 0;

ٻاهريون:
	لكل (سائيز_t i = 0; i < ARRAY_سائيز(reflog_نياپو); i++)
		حرر(reflog_نياپو[i]);
	commit_list_free(ڏٺل);
	clear_prio_قطار(&قطار);
	حرر(دليلون_copy);
	حرر(سر);
	أرجع ret;
}
