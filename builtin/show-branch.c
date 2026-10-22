#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#تعریف_کرو DISABLE_SIGN_COMPARE_WARNINGS

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

ساکن ثابت حرف*ثابت show_branch_usage[] = {
    N_("git show-branch [-a | --all] [-r | --remotes] [--topo-order | --date-order]\n"
       "                [--current] [--color[=<when>] | --no-color] [--sparse]\n"
       "                [--more=<n> | --list | --independent | --merge-base]\n"
       "                [--no-name | --sha1-name] [--topics]\n"
       "                [(<rev> | <glob>)...]"),
    N_("git show-branch (-g | --reflog)[=<n>[,<base>]] [--list] [<ref>]"),
    تہی
};

ساکن شمار git_colorbool showbranch_use_color = GIT_COLOR_UNKNOWN;

ساکن ساخت strvec default_دلائل = STRVEC_INIT;

/*
 * TODO: convert this use of commit->object.flags to commit-slab
 * instead to store a pointer to ref name directly. Then use the same
 * UNINTERESTING definition from revision.h here.
 */
#تعریف_کرو UNINTERESTING	01

#تعریف_کرو REV_سرکاؤ	 2
#تعریف_کرو MAX_REVS	(FLAG_بٹ - REV_سرکاؤ) /* should not exceed bits_per_int - REV_SHIFT */

#تعریف_کرو DEFAULT_REFLOG	4

ساکن ثابت حرف *get_color_رمز(صحیح idx)
{
	اگر (want_color(showbranch_use_color))
		واپس عمود_colors_ansi[idx % عمود_colors_ansi_max];
	واپس "";
}

ساکن ثابت حرف *get_color_reset_رمز(خالی)
{
	اگر (want_color(showbranch_use_color))
		واپس GIT_COLOR_RESET;
	واپس "";
}

ساکن ساخت commit *interesting(ساخت prio_قطار *قطار)
{
	ساخت commit *commit;
	prio_قطار_for_each(قطار, commit) {
		اگر (!(commit->object.flags & UNINTERESTING))
			واپس commit;
	}
	واپس تہی;
}

ساخت commit_name {
	ثابت حرف *سر_name; /* which head's ancestor? */
	صحیح generation; /* how many parents away from head_name */
};

define_commit_slab(commit_name_slab, ساخت commit_name *);
ساکن ساخت commit_name_slab name_slab;

ساکن ساخت commit_name *commit_مقصد_name(ساخت commit *commit)
{
	واپس *commit_name_slab_at(&name_slab, commit);
}


/* Name the commit as nth generation ancestor of head_name;
 * we count only the first-parent relationship for naming purposes.
 */
ساکن خالی name_commit(ساخت commit *commit, ثابت حرف *سر_name, صحیح nth)
{
	ساخت commit_name *name;

	name = *commit_name_slab_at(&name_slab, commit);
	اگر (!name) {
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
ساکن خالی name_والد(ساخت commit *commit, ساخت commit *والد)
{
	ساخت commit_name *commit_name = commit_مقصد_name(commit);
	ساخت commit_name *والد_name = commit_مقصد_name(والد);
	اگر (!commit_name)
		واپس;
	اگر (!والد_name ||
	    commit_name->generation + 1 < والد_name->generation)
		name_commit(والد, commit_name->سر_name,
			    commit_name->generation + 1);
}

ساکن صحیح name_پہلا_والد_chain(ساخت commit *c)
{
	صحیح i = 0;
	جب (c) {
		ساخت commit *p;
		اگر (!commit_مقصد_name(c))
			توڑو;
		اگر (!c->parents)
			توڑو;
		p = c->parents->شے;
		اگر (!commit_مقصد_name(p)) {
			name_والد(c, p);
			i++;
		}
		ورنہ
			توڑو;
		c = p;
	}
	واپس i;
}

ساکن خالی name_commits(ساخت commit_list *list,
			 ساخت commit **rev,
			 حرف **ref_name,
			 صحیح عدد_rev)
{
	ساخت commit_list *cl;
	ساخت commit *c;
	صحیح i;

	/* First give names to the given heads */
	برائے (cl = list; cl; cl = cl->اگلا) {
		c = cl->شے;
		اگر (commit_مقصد_name(c))
			جاری;
		برائے (i = 0; i < عدد_rev; i++) {
			اگر (rev[i] == c) {
				name_commit(c, ref_name[i], 0);
				توڑو;
			}
		}
	}

	/* Then commits on the first parent ancestry chain */
	کرو {
		i = 0;
		برائے (cl = list; cl; cl = cl->اگلا) {
			i += name_پہلا_والد_chain(cl->شے);
		}
	} جب (i);

	/* Finally, any unnamed commits */
	کرو {
		i = 0;
		برائے (cl = list; cl; cl = cl->اگلا) {
			ساخت commit_list *parents;
			ساخت commit_name *n;
			صحیح nth;
			c = cl->شے;
			اگر (!commit_مقصد_name(c))
				جاری;
			n = commit_مقصد_name(c);
			parents = c->parents;
			nth = 0;
			جب (parents) {
				ساخت commit *p = parents->شے;
				ساخت strbuf newname = STRBUF_INIT;
				parents = parents->اگلا;
				nth++;
				اگر (commit_مقصد_name(p))
					جاری;
				انتخاب (n->generation) {
				حالت 0:
					strbuf_addstr(&newname, n->سر_name);
					توڑو;
				حالت 1:
					strbuf_addf(&newname, "%s^", n->سر_name);
					توڑو;
				طےشدہ:
					strbuf_addf(&newname, "%s~%d",
						    n->سر_name, n->generation);
					توڑو;
				}
				اگر (nth == 1)
					strbuf_addch(&newname, '^');
				ورنہ
					strbuf_addf(&newname, "^%d", nth);
				name_commit(p, strbuf_detach(&newname, تہی), 0);
				i++;
				name_پہلا_والد_chain(p);
			}
		}
	} جب (i);
}

ساکن صحیح mark_نظر_آیا(ساخت commit *commit, ساخت commit_list **نظر_آیا_p)
{
	اگر (!commit->object.flags) {
		commit_list_داخل_کرو(commit, نظر_آیا_p);
		واپس 1;
	}
	واپس 0;
}

ساکن خالی join_revs(ساخت prio_قطار *قطار,
		      ساخت commit_list **نظر_آیا_p,
		      صحیح عدد_rev, صحیح extra)
{
	صحیح all_نقاب = ((1u << (REV_سرکاؤ + عدد_rev)) - 1);
	صحیح all_revs = all_نقاب & ~((1u << REV_سرکاؤ) - 1);
	ساخت commit *commit;

	جب ((commit = prio_قطار_peek(قطار))) {
		ساخت commit_list *parents;
		صحیح still_interesting = !!interesting(قطار);
		صحیح flags = commit->object.flags & all_نقاب;

		اگر (!still_interesting && extra <= 0)
			توڑو;

		prio_قطار_get(قطار);

		mark_نظر_آیا(commit, نظر_آیا_p);
		اگر ((flags & all_revs) == all_revs)
			flags |= UNINTERESTING;
		parents = commit->parents;

		جب (parents) {
			ساخت commit *p = parents->شے;
			صحیح this_flag = p->object.flags;
			parents = parents->اگلا;
			اگر ((this_flag & flags) == flags)
				جاری;
			repo_parse_commit(the_repository, p);
			اگر (mark_نظر_آیا(p, نظر_آیا_p) && !still_interesting)
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
	برائے (;;) {
		صحیح بدلا = 0;
		ساخت commit_list *s;
		برائے (s = *نظر_آیا_p; s; s = s->اگلا) {
			ساخت commit *c = s->شے;
			ساخت commit_list *parents;

			اگر (((c->object.flags & all_revs) != all_revs) &&
			    !(c->object.flags & UNINTERESTING))
				جاری;

			/* The current commit is either a merge base or
			 * already uninteresting one.  Mark its parents
			 * as uninteresting commits _only_ if they are
			 * already parsed.  No reason to find new ones
			 * here.
			 */
			parents = c->parents;
			جب (parents) {
				ساخت commit *p = parents->شے;
				parents = parents->اگلا;
				اگر (!(p->object.flags & UNINTERESTING)) {
					p->object.flags |= UNINTERESTING;
					بدلا = 1;
				}
			}
		}
		اگر (!بدلا)
			توڑو;
	}
}

ساکن خالی show_one_commit(ساخت commit *commit, صحیح no_name)
{
	ساخت strbuf pretty = STRBUF_INIT;
	ثابت حرف *pretty_str = "(unavailable)";
	ساخت commit_name *name = commit_مقصد_name(commit);

	اگر (commit->object.parsed) {
		pp_commit_easy(CMIT_FMT_ONELINE, commit, &pretty);
		pretty_str = pretty.buf;
	}
	skip_سابقہ(pretty_str, "[PATCH] ", &pretty_str);

	اگر (!no_name) {
		اگر (name && name->سر_name) {
			لکھو("[%s", name->سر_name);
			اگر (name->generation) {
				اگر (name->generation == 1)
					لکھو("^");
				ورنہ
					لکھو("~%d", name->generation);
			}
			لکھو("] ");
		}
		ورنہ
			لکھو("[%s] ",
			       repo_ڈھونڈو_unique_abbrev(the_repository, &commit->object.oid,
						       DEFAULT_ABBREV));
	}
	puts(pretty_str);
	strbuf_release(&pretty);
}

ساکن حرف *ref_name[MAX_REVS + 1];
ساکن صحیح ref_name_cnt;

ساکن ثابت حرف *ڈھونڈو_digit_سابقہ(ثابت حرف *s, صحیح *v)
{
	ثابت حرف *p;
	صحیح ver;
	حرف ch;

	برائے (p = s, ver = 0;
	     '0' <= (ch = *p) && ch <= '9';
	     p++)
		ver = ver * 10 + ch - '0';
	*v = ver;
	واپس p;
}


ساکن صحیح نسخہ_cmp(ثابت حرف *a, ثابت حرف *b)
{
	جب (1) {
		صحیح va, vb;

		a = ڈھونڈو_digit_سابقہ(a, &va);
		b = ڈھونڈو_digit_سابقہ(b, &vb);
		اگر (va != vb)
			واپس va - vb;

		جب (1) {
			صحیح ca = *a;
			صحیح cb = *b;
			اگر ('0' <= ca && ca <= '9')
				ca = 0;
			اگر ('0' <= cb && cb <= '9')
				cb = 0;
			اگر (ca != cb)
				واپس ca - cb;
			اگر (!ca)
				توڑو;
			a++;
			b++;
		}
		اگر (!*a && !*b)
			واپس 0;
	}
}

ساکن صحیح compare_ref_name(ثابت خالی *a_, ثابت خالی *b_)
{
	ثابت حرف * ثابت*a = a_, * ثابت*b = b_;
	واپس نسخہ_cmp(*a, *b);
}

ساکن خالی ترتیب_دو_ref_range(صحیح bottom, صحیح چوٹی)
{
	QSORT(ref_name + bottom, چوٹی - bottom, compare_ref_name);
}

ساکن صحیح append_ref(ثابت حرف *refname, ثابت ساخت object_id *oid,
		      صحیح allow_dups)
{
	ساخت commit *commit = lookup_commit_reference_gently(the_repository,
							       oid, 1);
	صحیح i;

	اگر (!commit)
		واپس 0;

	اگر (!allow_dups) {
		/* Avoid adding the same thing twice */
		برائے (i = 0; i < ref_name_cnt; i++)
			اگر (!strcmp(refname, ref_name[i]))
				واپس 0;
	}
	اگر (MAX_REVS <= ref_name_cnt) {
		warning(Q_("ignoring %s; cannot handle more than %d ref",
			   "ignoring %s; cannot handle more than %d refs",
			   MAX_REVS), refname, MAX_REVS);
		واپس 0;
	}
	ref_name[ref_name_cnt++] = xstrdup(refname);
	ref_name[ref_name_cnt] = تہی;
	واپس 0;
}

ساکن صحیح append_سر_ref(ثابت ساخت reference *ref, خالی *cb_ڈیٹا غیر_مستعمل)
{
	ساخت object_id tmp;
	صحیح ofs = 11;
	اگر (!starts_with(ref->name, "refs/heads/"))
		واپس 0;
	/* If both heads/foo and tags/foo exists, get_sha1 would
	 * get confused.
	 */
	اگر (repo_get_oid(the_repository, ref->name + ofs, &tmp) || !oideq(&tmp, ref->oid))
		ofs = 5;
	واپس append_ref(ref->name + ofs, ref->oid, 0);
}

ساکن صحیح append_remote_ref(ثابت ساخت reference *ref, خالی *cb_ڈیٹا غیر_مستعمل)
{
	ساخت object_id tmp;
	صحیح ofs = 13;
	اگر (!starts_with(ref->name, "refs/remotes/"))
		واپس 0;
	/* If both heads/foo and tags/foo exists, get_sha1 would
	 * get confused.
	 */
	اگر (repo_get_oid(the_repository, ref->name + ofs, &tmp) || !oideq(&tmp, ref->oid))
		ofs = 5;
	واپس append_ref(ref->name + ofs, ref->oid, 0);
}

ساکن صحیح append_لیبل_ref(ثابت حرف *refname, ثابت ساخت object_id *oid,
			  صحیح flag غیر_مستعمل, خالی *cb_ڈیٹا غیر_مستعمل)
{
	اگر (!starts_with(refname, "refs/tags/"))
		واپس 0;
	واپس append_ref(refname + 5, oid, 0);
}

ساکن ثابت حرف *مطابقت_ref_نمونہ = تہی;
ساکن صحیح مطابقت_ref_slash = 0;

ساکن صحیح append_matching_ref(ثابت ساخت reference *ref, خالی *cb_ڈیٹا)
{
	/* we want to allow pattern hold/<asterisk> to show all
	 * branches under refs/heads/hold/, and v0.99.9? to show
	 * refs/tags/v0.99.9a and friends.
	 */
	ثابت حرف *دم;
	صحیح slash = گنتی_slashes(ref->name);
	برائے (دم = ref->name; *دم && مطابقت_ref_slash < slash; )
		اگر (*دم++ == '/')
			slash--;
	اگر (!*دم)
		واپس 0;
	اگر (wildmatch(مطابقت_ref_نمونہ, دم, 0))
		واپس 0;
	اگر (starts_with(ref->name, "refs/heads/"))
		واپس append_سر_ref(ref, cb_ڈیٹا);
	اگر (starts_with(ref->name, "refs/tags/"))
		واپس append_لیبل_ref(ref->name, ref->oid, ref->flags, cb_ڈیٹا);
	واپس append_ref(ref->name, ref->oid, 0);
}

ساکن خالی snarf_refs(صحیح سر, صحیح remotes)
{
	اگر (سر) {
		صحیح orig_cnt = ref_name_cnt;

		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_سر_ref, تہی);
		ترتیب_دو_ref_range(orig_cnt, ref_name_cnt);
	}
	اگر (remotes) {
		صحیح orig_cnt = ref_name_cnt;

		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_remote_ref, تہی);
		ترتیب_دو_ref_range(orig_cnt, ref_name_cnt);
	}
}

ساکن صحیح rev_is_سر(ثابت حرف *سر, ثابت حرف *name)
{
	اگر (!سر)
		واپس 0;
	skip_سابقہ(سر, "refs/heads/", &سر);
	اگر (!skip_سابقہ(name, "refs/heads/", &name))
		skip_سابقہ(name, "heads/", &name);
	واپس !strcmp(سر, name);
}

ساکن صحیح show_ملاؤ_بنیاد(ثابت ساخت commit_list *نظر_آیا, صحیح عدد_rev)
{
	صحیح all_نقاب = ((1u << (REV_سرکاؤ + عدد_rev)) - 1);
	صحیح all_revs = all_نقاب & ~((1u << REV_سرکاؤ) - 1);
	صحیح exit_حالت = 1;

	برائے (ثابت ساخت commit_list *s = نظر_آیا; s; s = s->اگلا) {
		ساخت commit *commit = s->شے;
		صحیح flags = commit->object.flags & all_نقاب;
		اگر (!(flags & UNINTERESTING) &&
		    ((flags & all_revs) == all_revs)) {
			puts(oid_مقصد_hex(&commit->object.oid));
			exit_حالت = 0;
			commit->object.flags |= UNINTERESTING;
		}
	}
	واپس exit_حالت;
}

ساکن صحیح show_independent(ساخت commit **rev,
			    صحیح عدد_rev,
			    غیرعلامتی صحیح *rev_نقاب)
{
	صحیح i;

	برائے (i = 0; i < عدد_rev; i++) {
		ساخت commit *commit = rev[i];
		غیرعلامتی صحیح flag = rev_نقاب[i];

		اگر (commit->object.flags == flag)
			puts(oid_مقصد_hex(&commit->object.oid));
		commit->object.flags |= UNINTERESTING;
	}
	واپس 0;
}

ساکن خالی append_one_rev(ثابت حرف *av)
{
	ساخت object_id revkey;
	اگر (!repo_get_oid(the_repository, av, &revkey)) {
		append_ref(av, &revkey, 0);
		واپس;
	}
	اگر (strpbrk(av, "*?[")) {
		/* glob style match */
		صحیح saved_matches = ref_name_cnt;

		مطابقت_ref_نمونہ = av;
		مطابقت_ref_slash = گنتی_slashes(av);
		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_matching_ref, تہی);
		اگر (saved_matches == ref_name_cnt &&
		    ref_name_cnt < MAX_REVS)
			error(_("no matching refs with %s"), av);
		ترتیب_دو_ref_range(saved_matches, ref_name_cnt);
		واپس;
	}
	die("bad sha1 reference %s", av);
}

ساکن صحیح git_show_branch_config(ثابت حرف *var, ثابت حرف *قدر,
				  ثابت ساخت config_context *ctx, خالی *cb)
{
	اگر (!strcmp(var, "showbranch.default")) {
		اگر (!قدر)
			واپس config_error_nonbool(var);
		/*
		 * default_arg is now passed to parse_options(), so we need to
		 * mimic the real argv a bit better.
		 */
		اگر (!default_دلائل.nr)
			strvec_ڈالو(&default_دلائل, "show-branch");
		strvec_ڈالو(&default_دلائل, قدر);
		واپس 0;
	}

	اگر (!strcmp(var, "color.showbranch")) {
		showbranch_use_color = git_config_colorbool(var, قدر);
		واپس 0;
	}

	اگر (git_color_config(var, قدر, cb) < 0)
		واپس -1;

	واپس git_default_config(var, قدر, ctx, cb);
}

ساکن صحیح omit_اندر_dense(ساخت commit *commit, ساخت commit **rev, صحیح n)
{
	/* If the commit is tip of the named branches, do not
	 * omit it.
	 * Otherwise, if it is a merge that is reachable from only one
	 * tip, it is not that interesting.
	 */
	صحیح i, flag, گنتی;
	برائے (i = 0; i < n; i++)
		اگر (rev[i] == commit)
			واپس 0;
	flag = commit->object.flags;
	برائے (i = گنتی = 0; i < n; i++) {
		اگر (flag & (1u << (i + REV_سرکاؤ)))
			گنتی++;
	}
	اگر (گنتی == 1)
		واپس 1;
	واپس 0;
}

ساکن صحیح reflog = 0;

ساکن صحیح parse_reflog_param(ثابت ساخت option *opt, ثابت حرف *arg,
			      صحیح unset)
{
	حرف *ep;
	ثابت حرف **بنیاد = (ثابت حرف **)opt->قدر;
	BUG_ON_OPT_NEG(unset);
	اگر (!arg)
		arg = "";
	reflog = strtoul(arg, &ep, 10);
	اگر (*ep == ',')
		*بنیاد = ep + 1;
	ورنہ اگر (*ep)
		واپس error("unrecognized reflog param '%s'", arg);
	ورنہ
		*بنیاد = تہی;
	اگر (reflog <= 0)
		reflog = DEFAULT_REFLOG;
	واپس 0;
}

صحیح cmd_show_branch(صحیح ac,
		ثابت حرف **av,
		ثابت حرف *سابقہ,
		ساخت repository *repo غیر_مستعمل)
{
	ساخت commit *rev[MAX_REVS], *commit;
	حرف *reflog_پیغام[MAX_REVS] = {0};
	ساخت commit_list *نظر_آیا = تہی;
	ساخت prio_قطار قطار = { compare_commits_by_commit_date };
	غیرعلامتی صحیح rev_نقاب[MAX_REVS];
	صحیح عدد_rev, i, extra = 0;
	صحیح all_heads = 0, all_remotes = 0;
	صحیح all_نقاب, all_revs;
	شمار rev_ترتیب_دو_ترتیب ترتیب_دو_ترتیب = REV_ترتیب_دو_اندر_گراف_ترتیب;
	حرف *سر;
	ساخت object_id سر_oid;
	صحیح ملاؤ_بنیاد = 0;
	صحیح independent = 0;
	صحیح no_name = 0;
	صحیح sha1_name = 0;
	صحیح shown_ملاؤ_نقطہ = 0;
	صحیح with_current_branch = 0;
	صحیح سر_at = -1;
	صحیح topics = 0;
	صحیح sparse = 0;
	ثابت حرف *reflog_بنیاد = تہی;
	ساخت option builtin_show_branch_options[] = {
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
		OPT_سیٹ_INT(0, "list", &extra, N_("synonym to more=-1"), -1),
		OPT_BOOL(0, "no-name", &no_name, N_("suppress naming strings")),
		OPT_BOOL(0, "current", &with_current_branch,
			 N_("include the current branch")),
		OPT_BOOL(0, "sha1-name", &sha1_name,
			 N_("name commits with their object names")),
		OPT_BOOL(0, "merge-base", &ملاؤ_بنیاد,
			 N_("show possible merge bases")),
		OPT_BOOL(0, "independent", &independent,
			    N_("show refs unreachable from any other ref")),
		OPT_سیٹ_INT_F(0, "topo-order", &ترتیب_دو_ترتیب,
			      N_("show commits in topological order"),
			      REV_ترتیب_دو_اندر_گراف_ترتیب, PARSE_OPT_NONEG),
		OPT_BOOL(0, "topics", &topics,
			 N_("show only commits not on the first branch")),
		OPT_سیٹ_INT(0, "sparse", &sparse,
			    N_("show merges reachable from only one tip"), 1),
		OPT_سیٹ_INT_F(0, "date-order", &ترتیب_دو_ترتیب,
			      N_("topologically sort, maintaining date order "
				 "where possible"),
			      REV_ترتیب_دو_BY_COMMIT_DATE, PARSE_OPT_NONEG),
		OPT_CALLBACK_F('g', "reflog", &reflog_بنیاد, N_("<n>[,<base>]"),
			    N_("show <n> most recent ref-log entries starting at "
			       "base"),
			    PARSE_OPT_OPTARG | PARSE_OPT_NONEG,
			    parse_reflog_param),
		OPT_انتہا()
	};
	ثابت حرف **دلائل_copy = تہی;
	صحیح ret;

	init_commit_name_slab(&name_slab);

	repo_config(the_repository, git_show_branch_config, تہی);

	/* If nothing is specified, try the default first */
	اگر (ac == 1 && default_دلائل.nr) {
		DUP_ARRAY(دلائل_copy, default_دلائل.v, default_دلائل.nr);
		ac = default_دلائل.nr;
		av = دلائل_copy;
	}

	ac = parse_options(ac, av, سابقہ, builtin_show_branch_options,
			   show_branch_usage, PARSE_OPT_STOP_AT_NON_OPTION);
	اگر (all_heads)
		all_remotes = 1;

	اگر (extra || reflog) {
		/* "listing" mode is incompatible with
		 * independent nor merge-base modes.
		 */
		اگر (independent || ملاؤ_بنیاد)
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
		ساخت object_id oid;
		حرف *ref;
		صحیح بنیاد = 0;
		غیرعلامتی صحیح flags = 0;

		اگر (ac == 0) {
			ساکن ثابت حرف *fake_av[2];

			fake_av[0] = refs_resolve_refdup(get_main_ref_store(the_repository),
							 "HEAD",
							 RESOLVE_REF_READING,
							 &oid,
							 تہی);
			fake_av[1] = تہی;
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
		اگر (!repo_dwim_ref(the_repository, *av, لڑی_لمبائی(*av), &oid,
				   &ref, 0))
			die(_("no such ref %s"), *av);

		/* Has the base been specified? */
		اگر (reflog_بنیاد) {
			حرف *ep;
			بنیاد = strtoul(reflog_بنیاد, &ep, 10);
			اگر (*ep) {
				/* Ah, that is a date spec... */
				timestamp_t at;
				at = approxidate(reflog_بنیاد);
				read_ref_at(get_main_ref_store(the_repository),
					    ref, flags, at, -1, &oid, تہی,
					    تہی, تہی, &بنیاد);
			}
		}

		برائے (i = 0; i < reflog; i++) {
			حرف *logmsg = تہی;
			حرف *nth_desc;
			ثابت حرف *پیغام;
			حرف *انتہا;
			timestamp_t timestamp;
			صحیح tz;

			اگر (read_ref_at(get_main_ref_store(the_repository),
					ref, flags, 0, بنیاد + i, &oid, &logmsg,
					&timestamp, &tz, تہی)) {
				آزاد(logmsg);
				reflog = i;
				توڑو;
			}

			انتہا = strchr(logmsg, '\n');
			اگر (انتہا)
				*انتہا = '\0';

			پیغام = (*logmsg == '\0') ? "(none)" : logmsg;
			reflog_پیغام[i] = xstrfmt("(%s) %s",
						show_date(timestamp, tz,
							  DATE_منوال(RELATIVE)),
						پیغام);
			آزاد(logmsg);

			nth_desc = xstrfmt("%s@{%d}", *av, بنیاد+i);
			append_ref(nth_desc, &oid, 1);
			آزاد(nth_desc);
		}
		آزاد(ref);
	}
	ورنہ {
		جب (0 < ac) {
			append_one_rev(*av);
			ac--; av++;
		}
		اگر (all_heads + all_remotes)
			snarf_refs(all_heads, all_remotes);
	}

	سر = refs_resolve_refdup(get_main_ref_store(the_repository), "HEAD",
				   RESOLVE_REF_READING,
				   &سر_oid, تہی);

	اگر (with_current_branch && سر) {
		صحیح has_سر = 0;
		برائے (i = 0; !has_سر && i < ref_name_cnt; i++) {
			/* We are only interested in adding the branch
			 * HEAD points at.
			 */
			اگر (rev_is_سر(سر, ref_name[i]))
				has_سر++;
		}
		اگر (!has_سر) {
			ثابت حرف *name = سر;
			skip_سابقہ(name, "refs/heads/", &name);
			append_one_rev(name);
		}
	}

	اگر (!ref_name_cnt) {
		fprintf(stderr, "No revs to be shown.\n");
		ret = 0;
		جاؤ برآمد;
	}

	برائے (عدد_rev = 0; ref_name[عدد_rev]; عدد_rev++) {
		ساخت object_id revkey;
		غیرعلامتی صحیح flag = 1u << (عدد_rev + REV_سرکاؤ);

		اگر (MAX_REVS <= عدد_rev)
			die(Q_("cannot handle more than %d rev.",
			       "cannot handle more than %d revs.",
			       MAX_REVS), MAX_REVS);
		اگر (repo_get_oid(the_repository, ref_name[عدد_rev], &revkey))
			die(_("'%s' is not a valid ref."), ref_name[عدد_rev]);
		commit = lookup_commit_reference(the_repository, &revkey);
		اگر (!commit)
			die(_("cannot find commit %s (%s)"),
			    ref_name[عدد_rev], oid_مقصد_hex(&revkey));
		repo_parse_commit(the_repository, commit);
		mark_نظر_آیا(commit, &نظر_آیا);

		/* rev#0 uses bit REV_SHIFT, rev#1 uses bit REV_SHIFT+1,
		 * and so on.  REV_SHIFT bits from bit 0 are used for
		 * internal bookkeeping.
		 */
		commit->object.flags |= flag;
		اگر (commit->object.flags == flag)
			prio_قطار_put(&قطار, commit);
		rev[عدد_rev] = commit;
	}
	برائے (i = 0; i < عدد_rev; i++)
		rev_نقاب[i] = rev[i]->object.flags;

	اگر (0 <= extra)
		join_revs(&قطار, &نظر_آیا, عدد_rev, extra);

	commit_list_ترتیب_دو_by_date(&نظر_آیا);

	اگر (ملاؤ_بنیاد) {
		ret = show_ملاؤ_بنیاد(نظر_آیا, عدد_rev);
		جاؤ برآمد;
	}

	اگر (independent) {
		ret = show_independent(rev, عدد_rev, rev_نقاب);
		جاؤ برآمد;
	}

	/* Show list; --more=-1 means list-only */
	اگر (1 < عدد_rev || extra < 0) {
		برائے (i = 0; i < عدد_rev; i++) {
			صحیح j;
			صحیح is_سر = rev_is_سر(سر, ref_name[i]) &&
				      oideq(&سر_oid, &rev[i]->object.oid);
			اگر (extra < 0)
				لکھو("%c [%s] ",
				       is_سر ? '*' : ' ', ref_name[i]);
			ورنہ {
				برائے (j = 0; j < i; j++)
					حرف_لکھو(' ');
				لکھو("%s%c%s [%s] ",
				       get_color_رمز(i),
				       is_سر ? '*' : '!',
				       get_color_reset_رمز(), ref_name[i]);
			}

			اگر (!reflog) {
				/* header lines never need name */
				show_one_commit(rev[i], 1);
			}
			ورنہ
				puts(reflog_پیغام[i]);

			اگر (is_سر)
				سر_at = i;
		}
		اگر (0 <= extra) {
			برائے (i = 0; i < عدد_rev; i++)
				حرف_لکھو('-');
			حرف_لکھو('\n');
		}
	}
	اگر (extra < 0) {
		ret = 0;
		جاؤ برآمد;
	}

	/* Sort topologically */
	ترتیب_دو_اندر_topological_ترتیب(&نظر_آیا, ترتیب_دو_ترتیب);

	/* Give names to commits */
	اگر (!sha1_name && !no_name)
		name_commits(نظر_آیا, rev, ref_name, عدد_rev);

	all_نقاب = ((1u << (REV_سرکاؤ + عدد_rev)) - 1);
	all_revs = all_نقاب & ~((1u << REV_سرکاؤ) - 1);

	برائے (ساخت commit_list *l = نظر_آیا; l; l = l->اگلا) {
		ساخت commit *commit = l->شے;
		صحیح this_flag = commit->object.flags;
		صحیح is_ملاؤ_نقطہ = ((this_flag & all_revs) == all_revs);

		shown_ملاؤ_نقطہ |= is_ملاؤ_نقطہ;

		اگر (1 < عدد_rev) {
			صحیح is_ملاؤ = !!(commit->parents &&
					  commit->parents->اگلا);
			اگر (topics &&
			    !is_ملاؤ_نقطہ &&
			    (this_flag & (1u << REV_سرکاؤ)))
				جاری;
			اگر (!sparse && is_ملاؤ &&
			    omit_اندر_dense(commit, rev, عدد_rev))
				جاری;
			برائے (i = 0; i < عدد_rev; i++) {
				صحیح mark;
				اگر (!(this_flag & (1u << (i + REV_سرکاؤ))))
					mark = ' ';
				ورنہ اگر (is_ملاؤ)
					mark = '-';
				ورنہ اگر (i == سر_at)
					mark = '*';
				ورنہ
					mark = '+';
				اگر (mark == ' ')
					حرف_لکھو(mark);
				ورنہ
					لکھو("%s%c%s",
					       get_color_رمز(i),
					       mark, get_color_reset_رمز());
			}
			حرف_لکھو(' ');
		}
		show_one_commit(commit, no_name);

		اگر (shown_ملاؤ_نقطہ && --extra < 0)
			توڑو;
	}

	ret = 0;

برآمد:
	برائے (سائز_t i = 0; i < ARRAY_سائز(reflog_پیغام); i++)
		آزاد(reflog_پیغام[i]);
	commit_list_free(نظر_آیا);
	clear_prio_قطار(&قطار);
	آزاد(دلائل_copy);
	آزاد(سر);
	واپس ret;
}
