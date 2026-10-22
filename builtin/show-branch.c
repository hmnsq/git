#تعریف_کړه USE_THE_REPOSITORY_VARIABLE
#تعریف_کړه DISABLE_SIGN_COMPARE_WARNINGS

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

ساكن بنية strvec default_دلیلونه = STRVEC_INIT;

/*
 * TODO: convert this use of commit->object.flags to commit-slab
 * instead to store a pointer to ref name directly. Then use the same
 * UNINTERESTING definition from revision.h here.
 */
#تعریف_کړه UNINTERESTING	01

#تعریف_کړه REV_ښويدنه	 2
#تعریف_کړه MAX_REVS	(FLAG_بټونه - REV_ښويدنه) /* should not exceed bits_per_int - REV_SHIFT */

#تعریف_کړه DEFAULT_REFLOG	4

ساكن ثابت حرف *get_color_کوډ(صحيح idx)
{
	إذا (want_color(showbranch_use_color))
		أرجع ستنه_colors_ansi[idx % ستنه_colors_ansi_max];
	أرجع "";
}

ساكن ثابت حرف *get_color_reset_کوډ(فراغ)
{
	إذا (want_color(showbranch_use_color))
		أرجع GIT_COLOR_RESET;
	أرجع "";
}

ساكن بنية commit *interesting(بنية prio_کتار *کتار)
{
	بنية commit *commit;
	prio_کتار_for_each(کتار, commit) {
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
ساكن فراغ name_پلار(بنية commit *commit, بنية commit *پلار)
{
	بنية commit_name *commit_name = commit_منزل_name(commit);
	بنية commit_name *پلار_name = commit_منزل_name(پلار);
	إذا (!commit_name)
		أرجع;
	إذا (!پلار_name ||
	    commit_name->generation + 1 < پلار_name->generation)
		name_commit(پلار, commit_name->سر_name,
			    commit_name->generation + 1);
}

ساكن صحيح name_لومړی_پلار_chain(بنية commit *c)
{
	صحيح i = 0;
	طالما (c) {
		بنية commit *p;
		إذا (!commit_منزل_name(c))
			اكسر;
		إذا (!c->parents)
			اكسر;
		p = c->parents->توکی;
		إذا (!commit_منزل_name(p)) {
			name_پلار(c, p);
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
			 صحيح عدد_rev)
{
	بنية commit_list *cl;
	بنية commit *c;
	صحيح i;

	/* First give names to the given heads */
	لكل (cl = list; cl; cl = cl->بل) {
		c = cl->توکی;
		إذا (commit_منزل_name(c))
			استمر;
		لكل (i = 0; i < عدد_rev; i++) {
			إذا (rev[i] == c) {
				name_commit(c, ref_name[i], 0);
				اكسر;
			}
		}
	}

	/* Then commits on the first parent ancestry chain */
	افعل {
		i = 0;
		لكل (cl = list; cl; cl = cl->بل) {
			i += name_لومړی_پلار_chain(cl->توکی);
		}
	} طالما (i);

	/* Finally, any unnamed commits */
	افعل {
		i = 0;
		لكل (cl = list; cl; cl = cl->بل) {
			بنية commit_list *parents;
			بنية commit_name *n;
			صحيح nth;
			c = cl->توکی;
			إذا (!commit_منزل_name(c))
				استمر;
			n = commit_منزل_name(c);
			parents = c->parents;
			nth = 0;
			طالما (parents) {
				بنية commit *p = parents->توکی;
				بنية strbuf newname = STRBUF_INIT;
				parents = parents->بل;
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
				name_لومړی_پلار_chain(p);
			}
		}
	} طالما (i);
}

ساكن صحيح mark_کتل_شوی(بنية commit *commit, بنية commit_list **کتل_شوی_p)
{
	إذا (!commit->object.flags) {
		commit_list_ورننباسه(commit, کتل_شوی_p);
		أرجع 1;
	}
	أرجع 0;
}

ساكن فراغ join_revs(بنية prio_کتار *کتار,
		      بنية commit_list **کتل_شوی_p,
		      صحيح عدد_rev, صحيح extra)
{
	صحيح all_ماسک = ((1u << (REV_ښويدنه + عدد_rev)) - 1);
	صحيح all_revs = all_ماسک & ~((1u << REV_ښويدنه) - 1);
	بنية commit *commit;

	طالما ((commit = prio_کتار_peek(کتار))) {
		بنية commit_list *parents;
		صحيح still_interesting = !!interesting(کتار);
		صحيح flags = commit->object.flags & all_ماسک;

		إذا (!still_interesting && extra <= 0)
			اكسر;

		prio_کتار_get(کتار);

		mark_کتل_شوی(commit, کتل_شوی_p);
		إذا ((flags & all_revs) == all_revs)
			flags |= UNINTERESTING;
		parents = commit->parents;

		طالما (parents) {
			بنية commit *p = parents->توکی;
			صحيح this_flag = p->object.flags;
			parents = parents->بل;
			إذا ((this_flag & flags) == flags)
				استمر;
			repo_parse_commit(the_repository, p);
			إذا (mark_کتل_شوی(p, کتل_شوی_p) && !still_interesting)
				extra--;
			p->object.flags |= flags;
			prio_کتار_put(کتار, p);
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
		صحيح بدل_شوی = 0;
		بنية commit_list *s;
		لكل (s = *کتل_شوی_p; s; s = s->بل) {
			بنية commit *c = s->توکی;
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
				بنية commit *p = parents->توکی;
				parents = parents->بل;
				إذا (!(p->object.flags & UNINTERESTING)) {
					p->object.flags |= UNINTERESTING;
					بدل_شوی = 1;
				}
			}
		}
		إذا (!بدل_شوی)
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
	skip_مختاړی(pretty_str, "[PATCH] ", &pretty_str);

	إذا (!no_name) {
		إذا (name && name->سر_name) {
			چاپ_کړه("[%s", name->سر_name);
			إذا (name->generation) {
				إذا (name->generation == 1)
					چاپ_کړه("^");
				وإلا
					چاپ_کړه("~%d", name->generation);
			}
			چاپ_کړه("] ");
		}
		وإلا
			چاپ_کړه("[%s] ",
			       repo_پیدا_کړه_unique_abbrev(the_repository, &commit->object.oid,
						       DEFAULT_ABBREV));
	}
	puts(pretty_str);
	strbuf_release(&pretty);
}

ساكن حرف *ref_name[MAX_REVS + 1];
ساكن صحيح ref_name_cnt;

ساكن ثابت حرف *پیدا_کړه_digit_مختاړی(ثابت حرف *s, صحيح *v)
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


ساكن صحيح نسخه_cmp(ثابت حرف *a, ثابت حرف *b)
{
	طالما (1) {
		صحيح va, vb;

		a = پیدا_کړه_digit_مختاړی(a, &va);
		b = پیدا_کړه_digit_مختاړی(b, &vb);
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
	أرجع نسخه_cmp(*a, *b);
}

ساكن فراغ ترتیبول_ref_range(صحيح bottom, صحيح سر_2)
{
	QSORT(ref_name + bottom, سر_2 - bottom, compare_ref_name);
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

ساكن صحيح append_سر_ref(ثابت بنية reference *ref, فراغ *cb_معلومات ناکارول_شوی)
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

ساكن صحيح append_remote_ref(ثابت بنية reference *ref, فراغ *cb_معلومات ناکارول_شوی)
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

ساكن صحيح append_نښان_ref(ثابت حرف *refname, ثابت بنية object_id *oid,
			  صحيح flag ناکارول_شوی, فراغ *cb_معلومات ناکارول_شوی)
{
	إذا (!starts_with(refname, "refs/tags/"))
		أرجع 0;
	أرجع append_ref(refname + 5, oid, 0);
}

ساكن ثابت حرف *سمون_ref_نمونه = عدم;
ساكن صحيح سمون_ref_slash = 0;

ساكن صحيح append_matching_ref(ثابت بنية reference *ref, فراغ *cb_معلومات)
{
	/* we want to allow pattern hold/<asterisk> to show all
	 * branches under refs/heads/hold/, and v0.99.9? to show
	 * refs/tags/v0.99.9a and friends.
	 */
	ثابت حرف *لکۍ;
	صحيح slash = شمېر_slashes(ref->name);
	لكل (لکۍ = ref->name; *لکۍ && سمون_ref_slash < slash; )
		إذا (*لکۍ++ == '/')
			slash--;
	إذا (!*لکۍ)
		أرجع 0;
	إذا (wildmatch(سمون_ref_نمونه, لکۍ, 0))
		أرجع 0;
	إذا (starts_with(ref->name, "refs/heads/"))
		أرجع append_سر_ref(ref, cb_معلومات);
	إذا (starts_with(ref->name, "refs/tags/"))
		أرجع append_نښان_ref(ref->name, ref->oid, ref->flags, cb_معلومات);
	أرجع append_ref(ref->name, ref->oid, 0);
}

ساكن فراغ snarf_refs(صحيح سر, صحيح remotes)
{
	إذا (سر) {
		صحيح orig_cnt = ref_name_cnt;

		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_سر_ref, عدم);
		ترتیبول_ref_range(orig_cnt, ref_name_cnt);
	}
	إذا (remotes) {
		صحيح orig_cnt = ref_name_cnt;

		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_remote_ref, عدم);
		ترتیبول_ref_range(orig_cnt, ref_name_cnt);
	}
}

ساكن صحيح rev_is_سر(ثابت حرف *سر, ثابت حرف *name)
{
	إذا (!سر)
		أرجع 0;
	skip_مختاړی(سر, "refs/heads/", &سر);
	إذا (!skip_مختاړی(name, "refs/heads/", &name))
		skip_مختاړی(name, "heads/", &name);
	أرجع !strcmp(سر, name);
}

ساكن صحيح show_یوځای_بنسټ(ثابت بنية commit_list *کتل_شوی, صحيح عدد_rev)
{
	صحيح all_ماسک = ((1u << (REV_ښويدنه + عدد_rev)) - 1);
	صحيح all_revs = all_ماسک & ~((1u << REV_ښويدنه) - 1);
	صحيح exit_حالت = 1;

	لكل (ثابت بنية commit_list *s = کتل_شوی; s; s = s->بل) {
		بنية commit *commit = s->توکی;
		صحيح flags = commit->object.flags & all_ماسک;
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
			    صحيح عدد_rev,
			    بینښه صحيح *rev_ماسک)
{
	صحيح i;

	لكل (i = 0; i < عدد_rev; i++) {
		بنية commit *commit = rev[i];
		بینښه صحيح flag = rev_ماسک[i];

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

		سمون_ref_نمونه = av;
		سمون_ref_slash = شمېر_slashes(av);
		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_matching_ref, عدم);
		إذا (saved_matches == ref_name_cnt &&
		    ref_name_cnt < MAX_REVS)
			error(_("no matching refs with %s"), av);
		ترتیبول_ref_range(saved_matches, ref_name_cnt);
		أرجع;
	}
	die("bad sha1 reference %s", av);
}

ساكن صحيح git_show_branch_config(ثابت حرف *var, ثابت حرف *ارزښت,
				  ثابت بنية config_context *ctx, فراغ *cb)
{
	إذا (!strcmp(var, "showbranch.default")) {
		إذا (!ارزښت)
			أرجع config_error_nonbool(var);
		/*
		 * default_arg is now passed to parse_options(), so we need to
		 * mimic the real argv a bit better.
		 */
		إذا (!default_دلیلونه.nr)
			strvec_ټېل(&default_دلیلونه, "show-branch");
		strvec_ټېل(&default_دلیلونه, ارزښت);
		أرجع 0;
	}

	إذا (!strcmp(var, "color.showbranch")) {
		showbranch_use_color = git_config_colorbool(var, ارزښت);
		أرجع 0;
	}

	إذا (git_color_config(var, ارزښت, cb) < 0)
		أرجع -1;

	أرجع git_default_config(var, ارزښت, ctx, cb);
}

ساكن صحيح omit_دننه_dense(بنية commit *commit, بنية commit **rev, صحيح n)
{
	/* If the commit is tip of the named branches, do not
	 * omit it.
	 * Otherwise, if it is a merge that is reachable from only one
	 * tip, it is not that interesting.
	 */
	صحيح i, flag, شمېر;
	لكل (i = 0; i < n; i++)
		إذا (rev[i] == commit)
			أرجع 0;
	flag = commit->object.flags;
	لكل (i = شمېر = 0; i < n; i++) {
		إذا (flag & (1u << (i + REV_ښويدنه)))
			شمېر++;
	}
	إذا (شمېر == 1)
		أرجع 1;
	أرجع 0;
}

ساكن صحيح reflog = 0;

ساكن صحيح parse_reflog_param(ثابت بنية option *opt, ثابت حرف *arg,
			      صحيح unset)
{
	حرف *ep;
	ثابت حرف **بنسټ = (ثابت حرف **)opt->ارزښت;
	BUG_ON_OPT_NEG(unset);
	إذا (!arg)
		arg = "";
	reflog = strtoul(arg, &ep, 10);
	إذا (*ep == ',')
		*بنسټ = ep + 1;
	وإلا إذا (*ep)
		أرجع error("unrecognized reflog param '%s'", arg);
	وإلا
		*بنسټ = عدم;
	إذا (reflog <= 0)
		reflog = DEFAULT_REFLOG;
	أرجع 0;
}

صحيح cmd_show_branch(صحيح ac,
		ثابت حرف **av,
		ثابت حرف *مختاړی,
		بنية repository *repo ناکارول_شوی)
{
	بنية commit *rev[MAX_REVS], *commit;
	حرف *reflog_پیغام[MAX_REVS] = {0};
	بنية commit_list *کتل_شوی = عدم;
	بنية prio_کتار کتار = { compare_commits_by_commit_date };
	بینښه صحيح rev_ماسک[MAX_REVS];
	صحيح عدد_rev, i, extra = 0;
	صحيح all_heads = 0, all_remotes = 0;
	صحيح all_ماسک, all_revs;
	تعداد rev_ترتیبول_ترتیب ترتیبول_ترتیب = REV_ترتیبول_دننه_ګراف_ترتیب;
	حرف *سر;
	بنية object_id سر_oid;
	صحيح یوځای_بنسټ = 0;
	صحيح independent = 0;
	صحيح no_name = 0;
	صحيح sha1_name = 0;
	صحيح shown_یوځای_ټکی = 0;
	صحيح with_current_branch = 0;
	صحيح سر_at = -1;
	صحيح topics = 0;
	صحيح sparse = 0;
	ثابت حرف *reflog_بنسټ = عدم;
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
			.ارزښت = &extra,
			.precision = حجم(extra),
			.argh = N_("n"),
			.help = N_("show <n> more commits after the common ancestor"),
			.flags = PARSE_OPT_OPTARG,
			.defval = 1,
		},
		OPT_سېټ_INT(0, "list", &extra, N_("synonym to more=-1"), -1),
		OPT_BOOL(0, "no-name", &no_name, N_("suppress naming strings")),
		OPT_BOOL(0, "current", &with_current_branch,
			 N_("include the current branch")),
		OPT_BOOL(0, "sha1-name", &sha1_name,
			 N_("name commits with their object names")),
		OPT_BOOL(0, "merge-base", &یوځای_بنسټ,
			 N_("show possible merge bases")),
		OPT_BOOL(0, "independent", &independent,
			    N_("show refs unreachable from any other ref")),
		OPT_سېټ_INT_F(0, "topo-order", &ترتیبول_ترتیب,
			      N_("show commits in topological order"),
			      REV_ترتیبول_دننه_ګراف_ترتیب, PARSE_OPT_NONEG),
		OPT_BOOL(0, "topics", &topics,
			 N_("show only commits not on the first branch")),
		OPT_سېټ_INT(0, "sparse", &sparse,
			    N_("show merges reachable from only one tip"), 1),
		OPT_سېټ_INT_F(0, "date-order", &ترتیبول_ترتیب,
			      N_("topologically sort, maintaining date order "
				 "where possible"),
			      REV_ترتیبول_BY_COMMIT_DATE, PARSE_OPT_NONEG),
		OPT_CALLBACK_F('g', "reflog", &reflog_بنسټ, N_("<n>[,<base>]"),
			    N_("show <n> most recent ref-log entries starting at "
			       "base"),
			    PARSE_OPT_OPTARG | PARSE_OPT_NONEG,
			    parse_reflog_param),
		OPT_پای_ټکی()
	};
	ثابت حرف **دلیلونه_copy = عدم;
	صحيح ret;

	init_commit_name_slab(&name_slab);

	repo_config(the_repository, git_show_branch_config, عدم);

	/* If nothing is specified, try the default first */
	إذا (ac == 1 && default_دلیلونه.nr) {
		DUP_ARRAY(دلیلونه_copy, default_دلیلونه.v, default_دلیلونه.nr);
		ac = default_دلیلونه.nr;
		av = دلیلونه_copy;
	}

	ac = parse_options(ac, av, مختاړی, builtin_show_branch_options,
			   show_branch_usage, PARSE_OPT_STOP_AT_NON_OPTION);
	إذا (all_heads)
		all_remotes = 1;

	إذا (extra || reflog) {
		/* "listing" mode is incompatible with
		 * independent nor merge-base modes.
		 */
		إذا (independent || یوځای_بنسټ)
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
		صحيح بنسټ = 0;
		بینښه صحيح flags = 0;

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
		إذا (!repo_dwim_ref(the_repository, *av, مزي_اوږدوالی(*av), &oid,
				   &ref, 0))
			die(_("no such ref %s"), *av);

		/* Has the base been specified? */
		إذا (reflog_بنسټ) {
			حرف *ep;
			بنسټ = strtoul(reflog_بنسټ, &ep, 10);
			إذا (*ep) {
				/* Ah, that is a date spec... */
				timestamp_t at;
				at = approxidate(reflog_بنسټ);
				read_ref_at(get_main_ref_store(the_repository),
					    ref, flags, at, -1, &oid, عدم,
					    عدم, عدم, &بنسټ);
			}
		}

		لكل (i = 0; i < reflog; i++) {
			حرف *logmsg = عدم;
			حرف *nth_desc;
			ثابت حرف *پیغام;
			حرف *پای_ټکی;
			timestamp_t timestamp;
			صحيح tz;

			إذا (read_ref_at(get_main_ref_store(the_repository),
					ref, flags, 0, بنسټ + i, &oid, &logmsg,
					&timestamp, &tz, عدم)) {
				حرر(logmsg);
				reflog = i;
				اكسر;
			}

			پای_ټکی = strchr(logmsg, '\n');
			إذا (پای_ټکی)
				*پای_ټکی = '\0';

			پیغام = (*logmsg == '\0') ? "(none)" : logmsg;
			reflog_پیغام[i] = xstrfmt("(%s) %s",
						show_date(timestamp, tz,
							  DATE_ډېرپېښ(RELATIVE)),
						پیغام);
			حرر(logmsg);

			nth_desc = xstrfmt("%s@{%d}", *av, بنسټ+i);
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
			skip_مختاړی(name, "refs/heads/", &name);
			append_one_rev(name);
		}
	}

	إذا (!ref_name_cnt) {
		fprintf(stderr, "No revs to be shown.\n");
		ret = 0;
		اذهب بهر;
	}

	لكل (عدد_rev = 0; ref_name[عدد_rev]; عدد_rev++) {
		بنية object_id revkey;
		بینښه صحيح flag = 1u << (عدد_rev + REV_ښويدنه);

		إذا (MAX_REVS <= عدد_rev)
			die(Q_("cannot handle more than %d rev.",
			       "cannot handle more than %d revs.",
			       MAX_REVS), MAX_REVS);
		إذا (repo_get_oid(the_repository, ref_name[عدد_rev], &revkey))
			die(_("'%s' is not a valid ref."), ref_name[عدد_rev]);
		commit = lookup_commit_reference(the_repository, &revkey);
		إذا (!commit)
			die(_("cannot find commit %s (%s)"),
			    ref_name[عدد_rev], oid_منزل_hex(&revkey));
		repo_parse_commit(the_repository, commit);
		mark_کتل_شوی(commit, &کتل_شوی);

		/* rev#0 uses bit REV_SHIFT, rev#1 uses bit REV_SHIFT+1,
		 * and so on.  REV_SHIFT bits from bit 0 are used for
		 * internal bookkeeping.
		 */
		commit->object.flags |= flag;
		إذا (commit->object.flags == flag)
			prio_کتار_put(&کتار, commit);
		rev[عدد_rev] = commit;
	}
	لكل (i = 0; i < عدد_rev; i++)
		rev_ماسک[i] = rev[i]->object.flags;

	إذا (0 <= extra)
		join_revs(&کتار, &کتل_شوی, عدد_rev, extra);

	commit_list_ترتیبول_by_date(&کتل_شوی);

	إذا (یوځای_بنسټ) {
		ret = show_یوځای_بنسټ(کتل_شوی, عدد_rev);
		اذهب بهر;
	}

	إذا (independent) {
		ret = show_independent(rev, عدد_rev, rev_ماسک);
		اذهب بهر;
	}

	/* Show list; --more=-1 means list-only */
	إذا (1 < عدد_rev || extra < 0) {
		لكل (i = 0; i < عدد_rev; i++) {
			صحيح j;
			صحيح is_سر = rev_is_سر(سر, ref_name[i]) &&
				      oideq(&سر_oid, &rev[i]->object.oid);
			إذا (extra < 0)
				چاپ_کړه("%c [%s] ",
				       is_سر ? '*' : ' ', ref_name[i]);
			وإلا {
				لكل (j = 0; j < i; j++)
					توری_چاپ_کړه(' ');
				چاپ_کړه("%s%c%s [%s] ",
				       get_color_کوډ(i),
				       is_سر ? '*' : '!',
				       get_color_reset_کوډ(), ref_name[i]);
			}

			إذا (!reflog) {
				/* header lines never need name */
				show_one_commit(rev[i], 1);
			}
			وإلا
				puts(reflog_پیغام[i]);

			إذا (is_سر)
				سر_at = i;
		}
		إذا (0 <= extra) {
			لكل (i = 0; i < عدد_rev; i++)
				توری_چاپ_کړه('-');
			توری_چاپ_کړه('\n');
		}
	}
	إذا (extra < 0) {
		ret = 0;
		اذهب بهر;
	}

	/* Sort topologically */
	ترتیبول_دننه_topological_ترتیب(&کتل_شوی, ترتیبول_ترتیب);

	/* Give names to commits */
	إذا (!sha1_name && !no_name)
		name_commits(کتل_شوی, rev, ref_name, عدد_rev);

	all_ماسک = ((1u << (REV_ښويدنه + عدد_rev)) - 1);
	all_revs = all_ماسک & ~((1u << REV_ښويدنه) - 1);

	لكل (بنية commit_list *l = کتل_شوی; l; l = l->بل) {
		بنية commit *commit = l->توکی;
		صحيح this_flag = commit->object.flags;
		صحيح is_یوځای_ټکی = ((this_flag & all_revs) == all_revs);

		shown_یوځای_ټکی |= is_یوځای_ټکی;

		إذا (1 < عدد_rev) {
			صحيح is_یوځای = !!(commit->parents &&
					  commit->parents->بل);
			إذا (topics &&
			    !is_یوځای_ټکی &&
			    (this_flag & (1u << REV_ښويدنه)))
				استمر;
			إذا (!sparse && is_یوځای &&
			    omit_دننه_dense(commit, rev, عدد_rev))
				استمر;
			لكل (i = 0; i < عدد_rev; i++) {
				صحيح mark;
				إذا (!(this_flag & (1u << (i + REV_ښويدنه))))
					mark = ' ';
				وإلا إذا (is_یوځای)
					mark = '-';
				وإلا إذا (i == سر_at)
					mark = '*';
				وإلا
					mark = '+';
				إذا (mark == ' ')
					توری_چاپ_کړه(mark);
				وإلا
					چاپ_کړه("%s%c%s",
					       get_color_کوډ(i),
					       mark, get_color_reset_کوډ());
			}
			توری_چاپ_کړه(' ');
		}
		show_one_commit(commit, no_name);

		إذا (shown_یوځای_ټکی && --extra < 0)
			اكسر;
	}

	ret = 0;

بهر:
	لكل (اندازه_t i = 0; i < ARRAY_اندازه(reflog_پیغام); i++)
		حرر(reflog_پیغام[i]);
	commit_list_free(کتل_شوی);
	clear_prio_کتار(&کتار);
	حرر(دلیلونه_copy);
	حرر(سر);
	أرجع ret;
}
