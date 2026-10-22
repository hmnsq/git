/*
 * Builtin "git commit"
 *
 * Copyright (c) 2007 Kristian Høgsberg <krh@redhat.com>
 * Based on git-commit.sh by Junio C Hamano and Linus Torvalds
 */

#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#تعریف_کرو DISABLE_SIGN_COMPARE_WARNINGS

#شامل "builtin.h"
#شامل "advice.h"
#شامل "config.h"
#شامل "lockfile.h"
#شامل "cache-tree.h"
#شامل "color.h"
#شامل "dir.h"
#شامل "editor.h"
#شامل "environment.h"
#شامل "diff.h"
#شامل "commit.h"
#شامل "add-interactive.h"
#شامل "gettext.h"
#شامل "revision.h"
#شامل "wt-status.h"
#شامل "run-command.h"
#شامل "strbuf.h"
#شامل "object-name.h"
#شامل "parse-options.h"
#شامل "path.h"
#شامل "preload-index.h"
#شامل "read-cache.h"
#شامل "repository.h"
#شامل "string-list.h"
#شامل "rerere.h"
#شامل "unpack-trees.h"
#شامل "column.h"
#شامل "sequencer.h"
#شامل "sparse-index.h"
#شامل "mailmap.h"
#شامل "help.h"
#شامل "commit-reach.h"
#شامل "commit-graph.h"
#شامل "pretty.h"
#شامل "trailer.h"

ساکن ثابت حرف * ثابت builtin_commit_usage[] = {
	N_("git commit [-a | --interactive | --patch] [-s] [-v] [-u[<mode>]] [--amend]\n"
	   "           [--dry-run] [(-c | -C | --squash) <commit> | --fixup [(amend|reword):]<commit>]\n"
	   "           [-F <file> | -m <msg>] [--reset-author] [--allow-empty]\n"
	   "           [--allow-empty-message] [--no-verify] [-e] [--author=<author>]\n"
	   "           [--date=<date>] [--cleanup=<mode>] [--[no-]status]\n"
	   "           [-i | -o] [--pathspec-from-file=<file> [--pathspec-file-nul]]\n"
	   "           [(--trailer <token>[(=|:)<value>])...] [-S[<keyid>]]\n"
	   "           [--] [<pathspec>...]"),
	تہی
};

ساکن ثابت حرف * ثابت builtin_حالت_usage[] = {
	N_("git status [<options>] [--] [<pathspec>...]"),
	تہی
};

ساکن ثابت حرف خالی_خانے_amend_advice[] =
N_("You asked to amend the most recent commit, but doing so would make\n"
"it empty. You can repeat your command with --allow-empty, or you can\n"
"remove the commit entirely with \"git reset HEAD^\".\n");

ساکن ثابت حرف خالی_خانے_cherry_pick_advice[] =
N_("The previous cherry-pick is now empty, possibly due to conflict resolution.\n"
"If you wish to commit it anyway, use:\n"
"\n"
"    git commit --allow-empty\n"
"\n");

ساکن ثابت حرف خالی_خانے_rebase_pick_advice[] =
N_("Otherwise, please use 'git rebase --skip'\n");

ساکن ثابت حرف خالی_خانے_cherry_pick_advice_single[] =
N_("Otherwise, please use 'git cherry-pick --skip'\n");

ساکن ثابت حرف خالی_خانے_cherry_pick_advice_multi[] =
N_("and then use:\n"
"\n"
"    git cherry-pick --continue\n"
"\n"
"to resume cherry-picking the remaining commits.\n"
"If you wish to skip this commit, use:\n"
"\n"
"    git cherry-pick --skip\n"
"\n");

ساکن ثابت حرف *color_حالت_slots[] = {
	[WT_حالت_سرخی]	  = "header",
	[WT_حالت_UPDATED]	  = "updated",
	[WT_حالت_بدلا]	  = "changed",
	[WT_حالت_UNTRACKED]	  = "untracked",
	[WT_حالت_NOBRANCH]	  = "noBranch",
	[WT_حالت_UNMERGED]	  = "unmerged",
	[WT_حالت_LOCAL_BRANCH]  = "localBranch",
	[WT_حالت_REMOTE_BRANCH] = "remoteBranch",
	[WT_حالت_ONBRANCH]	  = "branch",
};

ساکن ثابت حرف *use_پیغام_بفر;
ساکن ساخت تالا_file اشاریہ_تالا; /* real index */
ساکن ساخت تالا_file false_تالا; /* used only for partial commits */
ساکن شمار {
	COMMIT_AS_IS = 1,
	COMMIT_معمول,
	COMMIT_PARTIAL
} commit_style;

ساکن ثابت حرف *force_author;
ساکن حرف *logfile;
ساکن حرف *template_file;
/*
 * The _message variables are commit names from which to take
 * the commit message and/or authorship.
 */
ساکن ثابت حرف *author_پیغام, *author_پیغام_بفر;
ساکن ثابت حرف *edit_پیغام, *use_پیغام;
ساکن حرف *fixup_پیغام, *fixup_commit, *squash_پیغام;
ساکن ثابت حرف *fixup_سابقہ;
ساکن صحیح all, also, interactive, patch_interactive, only, amend, signoff;
ساکن ساخت interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
ساکن صحیح edit_flag = -1; /* unspecified */
ساکن صحیح quiet, verbose, no_verify, allow_خالی_خانے, dry_تسلسل, renew_authorship;
ساکن صحیح config_commit_verbose = -1; /* unspecified */
ساکن صحیح no_post_rewrite, allow_خالی_خانے_پیغام, pathspec_file_nul;
ساکن ثابت حرف *untracked_files_arg, *force_date, *ignore_submodule_arg, *ignored_arg;
ساکن ثابت حرف *sign_commit, *pathspec_from_file;
ساکن ساخت strvec trailer_دلائل = STRVEC_INIT;

/*
 * The default commit message cleanup mode will remove the lines
 * beginning with # (shell comments) and leading and trailing
 * whitespaces (empty lines or containing only whitespaces)
 * if editor is used, and only the whitespaces if the message
 * is specified explicitly.
 */
ساکن شمار commit_پیغام_cleanup_منوال cleanup_منوال;
ساکن حرف *cleanup_config;

ساکن شمار commit_whence whence;
ساکن صحیح use_editor = 1, include_حالت = 1;
ساکن صحیح have_option_m;
ساکن ساخت strbuf پیغام_2 = STRBUF_INIT;

ساکن شمار wt_حالت_format حالت_format = حالت_FORMAT_UNSPECIFIED;

ساکن صحیح opt_parse_porcelain(ثابت ساخت option *opt, ثابت حرف *arg, صحیح unset)
{
	شمار wt_حالت_format *قدر = (شمار wt_حالت_format *)opt->قدر;
	اگر (unset)
		*قدر = حالت_FORMAT_NONE;
	ورنہ اگر (!arg)
		*قدر = حالت_FORMAT_PORCELAIN;
	ورنہ اگر (!strcmp(arg, "v1") || !strcmp(arg, "1"))
		*قدر = حالت_FORMAT_PORCELAIN;
	ورنہ اگر (!strcmp(arg, "v2") || !strcmp(arg, "2"))
		*قدر = حالت_FORMAT_PORCELAIN_V2;
	ورنہ
		die("unsupported porcelain version '%s'", arg);

	واپس 0;
}

ساکن صحیح opt_parse_m(ثابت ساخت option *opt, ثابت حرف *arg, صحیح unset)
{
	ساخت strbuf *buf = opt->قدر;
	اگر (unset) {
		have_option_m = 0;
		strbuf_setlen(buf, 0);
	} ورنہ {
		have_option_m = 1;
		اگر (buf->len)
			strbuf_addch(buf, '\n');
		strbuf_addstr(buf, arg);
		strbuf_complete_سطر(buf);
	}
	واپس 0;
}

ساکن صحیح opt_parse_rename_score(ثابت ساخت option *opt, ثابت حرف *arg, صحیح unset)
{
	ثابت حرف **قدر = opt->قدر;

	BUG_ON_OPT_NEG(unset);

	اگر (arg != تہی && *arg == '=')
		arg = arg + 1;

	*قدر = arg;
	واپس 0;
}

ساکن خالی determine_whence(ساخت wt_حالت *s)
{
	اگر (file_exists(git_راستہ_ملاؤ_سر(the_repository)))
		whence = FROM_ملاؤ;
	ورنہ اگر (!sequencer_determine_whence(the_repository, &whence))
		whence = FROM_COMMIT;
	اگر (s)
		s->whence = whence;
}

ساکن خالی حالت_init_config(ساخت wt_حالت *s, config_fn_t fn)
{
	wt_حالت_prepare(the_repository, s);
	init_diff_ui_defaults();
	repo_config(the_repository, fn, s);
	determine_whence(s);
	s->hints = advice_enabled(ADVICE_حالت_HINTS); /* must come after repo_config() */
}

ساکن خالی rollback_اشاریہ_files(خالی)
{
	انتخاب (commit_style) {
	حالت COMMIT_AS_IS:
		توڑو; /* nothing to do */
	حالت COMMIT_معمول:
		rollback_تالا_file(&اشاریہ_تالا);
		توڑو;
	حالت COMMIT_PARTIAL:
		rollback_تالا_file(&اشاریہ_تالا);
		rollback_تالا_file(&false_تالا);
		توڑو;
	}
}

ساکن صحیح commit_اشاریہ_files(خالی)
{
	صحیح err = 0;

	انتخاب (commit_style) {
	حالت COMMIT_AS_IS:
		توڑو; /* nothing to do */
	حالت COMMIT_معمول:
		err = commit_تالا_file(&اشاریہ_تالا);
		توڑو;
	حالت COMMIT_PARTIAL:
		err = commit_تالا_file(&اشاریہ_تالا);
		rollback_تالا_file(&false_تالا);
		توڑو;
	}

	واپس err;
}

/*
 * Take a union of paths in the index and the named tree (typically, "HEAD"),
 * and return the paths that match the given pattern in list.
 */
ساکن صحیح list_paths(ساخت string_list *list, ثابت حرف *with_درخت,
		      ثابت ساخت pathspec *نمونہ)
{
	صحیح i, ret;
	حرف *m;

	اگر (!نمونہ->nr)
		واپس 0;

	m = xcalloc(1, نمونہ->nr);

	اگر (with_درخت) {
		حرف *max_سابقہ = common_سابقہ(نمونہ);
		overlay_درخت_on_اشاریہ(the_repository->اشاریہ, with_درخت, max_سابقہ);
		آزاد(max_سابقہ);
	}

	/* TODO: audit for interaction with sparse-index. */
	ensure_بھرا_اشاریہ(the_repository->اشاریہ);
	برائے (i = 0; i < the_repository->اشاریہ->cache_nr; i++) {
		ثابت ساخت cache_اندراج *ce = the_repository->اشاریہ->cache[i];
		ساخت string_list_شے *شے;

		اگر (ce->ce_flags & CE_UPDATE)
			جاری;
		اگر (!ce_راستہ_مطابقت(the_repository->اشاریہ, ce, نمونہ, m))
			جاری;
		شے = string_list_داخل_کرو(list, ce->name);
		اگر (ce_skip_worktree(ce))
			شے->util = شے; /* better a valid pointer than a fake one */
	}

	ret = report_راستہ_error(m, نمونہ);
	آزاد(m);
	واپس ret;
}

ساکن خالی جوڑو_remove_files(ساخت string_list *list)
{
	صحیح i;
	برائے (i = 0; i < list->nr; i++) {
		ساخت stat st;
		ساخت string_list_شے *p = &(list->اشیاء[i]);

		/* p->util is skip-worktree */
		اگر (p->util)
			جاری;

		اگر (!lstat(p->string, &st)) {
			اگر (جوڑو_مقصد_اشاریہ(the_repository->اشاریہ, p->string, &st, 0))
				die(_("updating files failed"));
		} ورنہ
			remove_file_from_اشاریہ(the_repository->اشاریہ, p->string);
	}
}

ساکن خالی create_بنیاد_اشاریہ(ثابت ساخت commit *current_سر)
{
	ساخت درخت *درخت;
	ساخت unpack_trees_options opts;
	ساخت درخت_desc t;

	اگر (!current_سر) {
		discard_اشاریہ(the_repository->اشاریہ);
		واپس;
	}

	memset(&opts, 0, حجم(opts));
	opts.سر_idx = 1;
	opts.اشاریہ_only = 1;
	opts.ملاؤ = 1;
	opts.src_اشاریہ = the_repository->اشاریہ;
	opts.dst_اشاریہ = the_repository->اشاریہ;

	opts.fn = oneway_ملاؤ;
	درخت = repo_parse_درخت_بالواسطہ(the_repository,
					&current_سر->object.oid);
	اگر (!درخت)
		die(_("failed to unpack HEAD tree object"));
	اگر (repo_parse_درخت(the_repository, درخت) < 0)
		باہر_نکلو(128);
	init_درخت_desc(&t, &درخت->object.oid, درخت->بفر, درخت->سائز);
	اگر (unpack_trees(1, &t, &opts))
		باہر_نکلو(128); /* We've already reported the error, finish dying */
}

ساکن خالی refresh_cache_or_die(صحیح refresh_flags)
{
	/*
	 * refresh_flags contains REFRESH_QUIET, so the only errors
	 * are for unmerged entries.
	 */
	اگر (refresh_اشاریہ(the_repository->اشاریہ, refresh_flags | REFRESH_اندر_PORCELAIN, تہی, تہی, تہی))
		die_resolve_conflict("commit");
}

ساکن ثابت حرف *prepare_اشاریہ(ثابت حرف **argv, ثابت حرف *سابقہ,
				 ثابت ساخت commit *current_سر, صحیح is_حالت)
{
	ساخت string_list partial = STRING_LIST_INIT_DUP;
	ساخت pathspec pathspec;
	صحیح refresh_flags = REFRESH_QUIET;
	ثابت حرف *ret;
	حرف *راستہ = تہی;

	اگر (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	اگر (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	اگر (is_حالت)
		refresh_flags |= REFRESH_UNMERGED;
	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_بھرا,
		       سابقہ, argv);

	اگر (pathspec_from_file) {
		اگر (interactive)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--interactive/--patch");

		اگر (all)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "-a");

		اگر (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_بھرا,
				    سابقہ, pathspec_from_file, pathspec_file_nul);
	} ورنہ اگر (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	اگر (!pathspec.nr && (also || (only && !allow_خالی_خانے &&
	    (!amend || (fixup_پیغام && strcmp(fixup_سابقہ, "amend"))))))
		die(_("No paths with --include/--only does not make sense."));

	اگر (repo_read_اشاریہ_preload(the_repository, &pathspec, 0) < 0)
		die(_("index file corrupt"));

	اگر (interactive) {
		حرف *old_اشاریہ_env = تہی, *old_repo_اشاریہ_file;
		repo_hold_locked_اشاریہ(the_repository, &اشاریہ_تالا,
				       تالا_DIE_ON_ERROR);

		refresh_cache_or_die(refresh_flags);

		اگر (write_locked_اشاریہ(the_repository->اشاریہ, &اشاریہ_تالا, 0))
			die(_("unable to create temporary index"));

		old_repo_اشاریہ_file = the_repository->اشاریہ_file;
		the_repository->اشاریہ_file =
			(حرف *)get_تالا_file_راستہ(&اشاریہ_تالا);
		old_اشاریہ_env = xstrdup_or_null(getenv(اشاریہ_ENVIRONMENT));
		setenv(اشاریہ_ENVIRONMENT, the_repository->اشاریہ_file, 1);

		اگر (interactive_جوڑو(the_repository, argv, سابقہ, patch_interactive, &interactive_opts) != 0)
			die(_("interactive add failed"));

		the_repository->اشاریہ_file = old_repo_اشاریہ_file;
		اگر (old_اشاریہ_env && *old_اشاریہ_env)
			setenv(اشاریہ_ENVIRONMENT, old_اشاریہ_env, 1);
		ورنہ
			unsetenv(اشاریہ_ENVIRONMENT);
		FREE_AND_NULL(old_اشاریہ_env);

		discard_اشاریہ(the_repository->اشاریہ);
		read_اشاریہ_from(the_repository->اشاریہ, get_تالا_file_راستہ(&اشاریہ_تالا),
				repo_get_git_dir(the_repository));
		اگر (cache_درخت_update(the_repository->اشاریہ, WRITE_درخت_SILENT) == 0) {
			اگر (reopen_تالا_file(&اشاریہ_تالا) < 0)
				die(_("unable to write index file"));
			اگر (write_locked_اشاریہ(the_repository->اشاریہ, &اشاریہ_تالا, 0))
				die(_("unable to update temporary index"));
		} ورنہ
			warning(_("Failed to update main cache tree"));

		commit_style = COMMIT_معمول;
		ret = get_تالا_file_راستہ(&اشاریہ_تالا);
		جاؤ برآمد;
	} ورنہ {
		اگر (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--interactive/--patch");
		اگر (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--interactive/--patch");
	}

	/*
	 * Non partial, non as-is commit.
	 *
	 * (1) get the real index;
	 * (2) update the_index as necessary;
	 * (3) write the_index out to the real index (still locked);
	 * (4) return the name of the locked index file.
	 *
	 * The caller should run hooks on the locked real index, and
	 * (A) if all goes well, commit the real index;
	 * (B) on failure, rollback the real index.
	 */
	اگر (all || (also && pathspec.nr)) {
		حرف *ps_matched = xcalloc(pathspec.nr, 1);
		repo_hold_locked_اشاریہ(the_repository, &اشاریہ_تالا,
				       تالا_DIE_ON_ERROR);
		جوڑو_files_مقصد_cache(the_repository, also ? سابقہ : تہی,
				   &pathspec, ps_matched, 0, 0, 0 );
		اگر (!all && report_راستہ_error(ps_matched, &pathspec))
			باہر_نکلو(128);

		refresh_cache_or_die(refresh_flags);
		cache_درخت_update(the_repository->اشاریہ, WRITE_درخت_SILENT);
		اگر (write_locked_اشاریہ(the_repository->اشاریہ, &اشاریہ_تالا, 0))
			die(_("unable to write new index file"));
		commit_style = COMMIT_معمول;
		ret = get_تالا_file_راستہ(&اشاریہ_تالا);
		آزاد(ps_matched);
		جاؤ برآمد;
	}

	/*
	 * As-is commit.
	 *
	 * (1) return the name of the real index file.
	 *
	 * The caller should run hooks on the real index,
	 * and create commit from the_index.
	 * We still need to refresh the index here.
	 */
	اگر (!only && !pathspec.nr) {
		repo_hold_locked_اشاریہ(the_repository, &اشاریہ_تالا,
				       تالا_DIE_ON_ERROR);
		refresh_cache_or_die(refresh_flags);
		اگر (the_repository->اشاریہ->cache_بدلا
		    || !cache_درخت_fully_valid(the_repository->اشاریہ))
			cache_درخت_update(the_repository->اشاریہ, WRITE_درخت_SILENT);
		اگر (write_locked_اشاریہ(the_repository->اشاریہ, &اشاریہ_تالا,
				       COMMIT_تالا | SKIP_IF_UNCHANGED))
			die(_("unable to write new index file"));
		commit_style = COMMIT_AS_IS;
		ret = repo_get_اشاریہ_file(the_repository);
		جاؤ برآمد;
	}

	/*
	 * A partial commit.
	 *
	 * (0) find the set of affected paths;
	 * (1) get lock on the real index file;
	 * (2) update the_index with the given paths;
	 * (3) write the_index out to the real index (still locked);
	 * (4) get lock on the false index file;
	 * (5) reset the_index from HEAD;
	 * (6) update the_index the same way as (2);
	 * (7) write the_index out to the false index file;
	 * (8) return the name of the false index file (still locked);
	 *
	 * The caller should run hooks on the locked false index, and
	 * create commit from it.  Then
	 * (A) if all goes well, commit the real index;
	 * (B) on failure, rollback the real index;
	 * In either case, rollback the false index.
	 */
	commit_style = COMMIT_PARTIAL;

	اگر (whence != FROM_COMMIT) {
		اگر (whence == FROM_ملاؤ)
			die(_("cannot do a partial commit during a merge."));
		ورنہ اگر (is_from_cherry_pick(whence))
			die(_("cannot do a partial commit during a cherry-pick."));
		ورنہ اگر (is_from_rebase(whence))
			die(_("cannot do a partial commit during a rebase."));
	}

	اگر (list_paths(&partial, !current_سر ? تہی : "HEAD", &pathspec))
		باہر_نکلو(1);

	discard_اشاریہ(the_repository->اشاریہ);
	اگر (repo_read_اشاریہ(the_repository) < 0)
		die(_("cannot read the index"));

	repo_hold_locked_اشاریہ(the_repository, &اشاریہ_تالا, تالا_DIE_ON_ERROR);
	جوڑو_remove_files(&partial);
	refresh_اشاریہ(the_repository->اشاریہ, REFRESH_QUIET, تہی, تہی, تہی);
	cache_درخت_update(the_repository->اشاریہ, WRITE_درخت_SILENT);
	اگر (write_locked_اشاریہ(the_repository->اشاریہ, &اشاریہ_تالا, 0))
		die(_("unable to write new index file"));

	راستہ = repo_git_راستہ(the_repository, "next-index-%"PRIuMAX,
			     (uintmax_t) getpid());
	hold_تالا_file_for_update(&false_تالا, راستہ,
				  تالا_DIE_ON_ERROR);

	create_بنیاد_اشاریہ(current_سر);
	جوڑو_remove_files(&partial);
	refresh_اشاریہ(the_repository->اشاریہ, REFRESH_QUIET, تہی, تہی, تہی);

	اگر (write_locked_اشاریہ(the_repository->اشاریہ, &false_تالا, 0))
		die(_("unable to write temporary index file"));

	discard_اشاریہ(the_repository->اشاریہ);
	ret = get_تالا_file_راستہ(&false_تالا);
	read_اشاریہ_from(the_repository->اشاریہ, ret, repo_get_git_dir(the_repository));
برآمد:
	string_list_clear(&partial, 0);
	clear_pathspec(&pathspec);
	آزاد(راستہ);
	واپس ret;
}

ساکن صحیح تسلسل_حالت(FILE *fp, ثابت حرف *اشاریہ_file, ثابت حرف *سابقہ, صحیح nowarn,
		      ساخت wt_حالت *s)
{
	ساخت object_id oid;

	اگر (s->relative_paths)
		s->سابقہ = سابقہ;

	اگر (amend) {
		s->amend = 1;
		s->reference = "HEAD^1";
	}
	s->verbose = verbose;
	s->اشاریہ_file = اشاریہ_file;
	s->fp = fp;
	s->nowarn = nowarn;
	s->is_initial = repo_get_oid(the_repository, s->reference, &oid) ? 1 : 0;
	اگر (!s->is_initial)
		oidcpy(&s->oid_commit, &oid);
	s->حالت_format = حالت_format;
	s->ignore_submodule_arg = ignore_submodule_arg;

	wt_حالت_collect(s);
	wt_حالت_print(s);
	wt_حالت_collect_free_buffers(s);

	واپس s->committable;
}

ساکن صحیح is_a_ملاؤ(ثابت ساخت commit *current_سر)
{
	واپس !!(current_سر->parents && current_سر->parents->اگلا);
}

ساکن خالی assert_بانٹو_ident(ساخت ident_بانٹو *id, ثابت ساخت strbuf *buf)
{
	اگر (بانٹو_ident_سطر(id, buf->buf, buf->len) || !id->date_begin)
		BUG("unable to parse our own ident: %s", buf->buf);
}

ساکن خالی export_one(ثابت حرف *var, ثابت حرف *s, ثابت حرف *e, صحیح hack)
{
	ساخت strbuf buf = STRBUF_INIT;
	اگر (hack)
		strbuf_addch(&buf, hack);
	strbuf_جوڑو(&buf, s, e - s);
	setenv(var, buf.buf, 1);
	strbuf_release(&buf);
}

ساکن صحیح parse_force_date(ثابت حرف *اندر, ساخت strbuf *برآمد)
{
	strbuf_addch(برآمد, '@');

	اگر (parse_date(اندر, برآمد) < 0) {
		صحیح errors = 0;
		غیرعلامتی طویل t = approxidate_careful(اندر, &errors);
		اگر (errors)
			واپس -1;
		strbuf_addf(برآمد, "%lu", t);
	}

	واپس 0;
}

ساکن خالی سیٹ_ident_var(حرف **buf, حرف *val)
{
	آزاد(*buf);
	*buf = val;
}

ساکن خالی determine_author_info(ساخت strbuf *author_ident)
{
	حرف *name, *email, *date;
	ساخت ident_بانٹو author;

	name = xstrdup_or_null(getenv("GIT_AUTHOR_NAME"));
	email = xstrdup_or_null(getenv("GIT_AUTHOR_EMAIL"));
	date = xstrdup_or_null(getenv("GIT_AUTHOR_DATE"));

	اگر (author_پیغام) {
		ساخت ident_بانٹو ident;
		سائز_t len;
		ثابت حرف *a;

		a = ڈھونڈو_commit_سرخی(author_پیغام_بفر, "author", &len);
		اگر (!a)
			die(_("commit '%s' lacks author header"), author_پیغام);
		اگر (بانٹو_ident_سطر(&ident, a, len) < 0)
			die(_("commit '%s' has malformed author line"), author_پیغام);

		سیٹ_ident_var(&name, xmemdupz(ident.name_begin, ident.name_انتہا - ident.name_begin));
		سیٹ_ident_var(&email, xmemdupz(ident.mail_begin, ident.mail_انتہا - ident.mail_begin));

		اگر (ident.date_begin) {
			ساخت strbuf date_buf = STRBUF_INIT;
			strbuf_addch(&date_buf, '@');
			strbuf_جوڑو(&date_buf, ident.date_begin, ident.date_انتہا - ident.date_begin);
			strbuf_addch(&date_buf, ' ');
			strbuf_جوڑو(&date_buf, ident.tz_begin, ident.tz_انتہا - ident.tz_begin);
			سیٹ_ident_var(&date, strbuf_detach(&date_buf, تہی));
		}
	}

	اگر (force_author) {
		ساخت ident_بانٹو ident;

		اگر (بانٹو_ident_سطر(&ident, force_author, لڑی_لمبائی(force_author)) < 0)
			die(_("malformed --author parameter"));
		سیٹ_ident_var(&name, xmemdupz(ident.name_begin, ident.name_انتہا - ident.name_begin));
		سیٹ_ident_var(&email, xmemdupz(ident.mail_begin, ident.mail_انتہا - ident.mail_begin));
	}

	اگر (force_date) {
		ساخت strbuf date_buf = STRBUF_INIT;
		اگر (parse_force_date(force_date, &date_buf))
			die(_("invalid date format: %s"), force_date);
		سیٹ_ident_var(&date, strbuf_detach(&date_buf, تہی));
	}

	strbuf_addstr(author_ident, fmt_ident(name, email, WANT_AUTHOR_IDENT, date,
				IDENT_STRICT));
	assert_بانٹو_ident(&author, author_ident);
	export_one("GIT_AUTHOR_NAME", author.name_begin, author.name_انتہا, 0);
	export_one("GIT_AUTHOR_EMAIL", author.mail_begin, author.mail_انتہا, 0);
	export_one("GIT_AUTHOR_DATE", author.date_begin, author.tz_انتہا, '@');
	آزاد(name);
	آزاد(email);
	آزاد(date);
}

ساکن صحیح author_date_is_interesting(خالی)
{
	واپس author_پیغام || force_date;
}

#اگر_غیر_تعریف_شدہ WITH_BREAKING_CHANGES
ساکن خالی adjust_comment_سطر_char(ثابت ساخت strbuf *sb)
{
	حرف candidates[] = "#;@!$%^&|:";
	حرف *candidate;
	ثابت حرف *p;
	سائز_t cutoff;

	/* Ignore comment chars in trailing comments (e.g., Conflicts:) */
	cutoff = sb->len - ignored_log_پیغام_بائٹیں(sb->buf, sb->len);

	اگر (!memchr(sb->buf, candidates[0], sb->len)) {
		آزاد(comment_سطر_str_مقصد_free);
		comment_سطر_str = comment_سطر_str_مقصد_free =
			xstrfmt("%c", candidates[0]);
		واپس;
	}

	p = sb->buf;
	candidate = strchr(candidates, *p);
	اگر (candidate)
		*candidate = ' ';
	برائے (p = sb->buf; p + 1 < sb->buf + cutoff; p++) {
		اگر ((p[0] == '\n' || p[0] == '\r') && p[1]) {
			candidate = strchr(candidates, p[1]);
			اگر (candidate)
				*candidate = ' ';
		}
	}

	برائے (p = candidates; *p == ' '; p++)
		;
	اگر (!*p)
		die(_("unable to select a comment character that is not used\n"
		      "in the current commit message"));
	آزاد(comment_سطر_str_مقصد_free);
	comment_سطر_str = comment_سطر_str_مقصد_free = xstrfmt("%c", *p);
}
#اگر_ختم /* !WITH_BREAKING_CHANGES */

ساکن خالی prepare_amend_commit(ساخت commit *commit, ساخت strbuf *sb,
				ساخت pretty_print_context *ctx)
{
	ثابت حرف *بفر, *subject, *fmt;

	بفر = repo_get_commit_بفر(the_repository, commit, تہی);
	ڈھونڈو_commit_subject(بفر, &subject);
	/*
	 * If we amend the 'amend!' commit then we don't want to
	 * duplicate the subject line.
	 */
	fmt = starts_with(subject, "amend!") ? "%b" : "%B";
	repo_format_commit_پیغام(the_repository, commit, fmt, sb, ctx);
	repo_unuse_commit_بفر(the_repository, commit, بفر);
}

ساکن خالی change_ڈیٹا_free(خالی *util, ثابت حرف *str غیر_مستعمل)
{
	ساخت wt_حالت_change_ڈیٹا *d = util;
	آزاد(d->rename_ماخذ);
	آزاد(d);
}

ساکن صحیح prepare_مقصد_commit(ثابت حرف *اشاریہ_file, ثابت حرف *سابقہ,
			     ساخت commit *current_سر,
			     ساخت wt_حالت *s,
			     ساخت strbuf *author_ident)
{
	ساخت stat statbuf;
	ساخت strbuf committer_ident = STRBUF_INIT;
	صحیح committable;
	ساخت strbuf sb = STRBUF_INIT;
	ثابت حرف *hook_arg1 = تہی;
	ثابت حرف *hook_arg2 = تہی;
	صحیح clean_پیغام_contents = (cleanup_منوال != COMMIT_پیغام_CLEANUP_NONE);
	صحیح old_display_comment_سابقہ;
	صحیح invoked_hook;

	/* This checks and barfs if author is badly specified */
	determine_author_info(author_ident);

	اگر (!no_verify && تسلسل_commit_hook(use_editor, اشاریہ_file, &invoked_hook,
					  "pre-commit", تہی))
		واپس 0;

	اگر (squash_پیغام) {
		/*
		 * Insert the proper subject line before other commit
		 * message options add their content.
		 */
		اگر (use_پیغام && !strcmp(use_پیغام, squash_پیغام))
			strbuf_addstr(&sb, "squash! ");
		ورنہ {
			ساخت pretty_print_context ctx = {0};
			ساخت commit *c;
			c = lookup_commit_reference_by_name(squash_پیغام);
			اگر (!c)
				die(_("could not lookup commit '%s'"), squash_پیغام);
			ctx.output_encoding = get_commit_output_encoding();
			repo_format_commit_پیغام(the_repository, c,
						   "squash! %s\n\n", &sb,
						   &ctx);
		}
	}

	اگر (have_option_m && !fixup_پیغام) {
		strbuf_addbuf(&sb, &پیغام_2);
		hook_arg1 = "message";
	} ورنہ اگر (logfile && !strcmp(logfile, "-")) {
		اگر (isatty(0))
			fprintf(stderr, _("(reading log message from standard input)\n"));
		اگر (strbuf_read(&sb, 0, 0) < 0)
			die_errno(_("could not read log from standard input"));
		hook_arg1 = "message";
	} ورنہ اگر (logfile) {
		اگر (strbuf_read_file(&sb, logfile, 0) < 0)
			die_errno(_("could not read log file '%s'"),
				  logfile);
		hook_arg1 = "message";
	} ورنہ اگر (use_پیغام) {
		ثابت حرف *بفر;
		بفر = لڑی_تلاش(use_پیغام_بفر, "\n\n");
		اگر (بفر)
			strbuf_addstr(&sb, skip_blank_سطریں(بفر + 2));
		hook_arg1 = "commit";
		hook_arg2 = use_پیغام;
	} ورنہ اگر (fixup_پیغام) {
		ساخت pretty_print_context ctx = {0};
		ساخت commit *commit;
		حرف *fmt;
		commit = lookup_commit_reference_by_name(fixup_commit);
		اگر (!commit)
			die(_("could not lookup commit '%s'"), fixup_commit);
		ctx.output_encoding = get_commit_output_encoding();
		fmt = xstrfmt("%s! %%s\n\n", fixup_سابقہ);
		repo_format_commit_پیغام(the_repository, commit, fmt, &sb,
					   &ctx);
		آزاد(fmt);
		hook_arg1 = "message";

		/*
		 * Only `-m` commit message option is checked here, as
		 * it supports `--fixup` to append the commit message.
		 *
		 * The other commit message options `-c`/`-C`/`-F` are
		 * incompatible with all the forms of `--fixup` and
		 * have already errored out while parsing the `git commit`
		 * options.
		 */
		اگر (have_option_m && !strcmp(fixup_سابقہ, "fixup"))
			strbuf_addbuf(&sb, &پیغام_2);

		اگر (!strcmp(fixup_سابقہ, "amend")) {
			اگر (have_option_m)
				die(_("options '%s' and '%s:%s' cannot be used together"), "-m", "--fixup", fixup_پیغام);
			prepare_amend_commit(commit, &sb, &ctx);
		}
	} ورنہ اگر (!stat(git_راستہ_ملاؤ_پیغام(the_repository), &statbuf)) {
		سائز_t ملاؤ_پیغام_آغاز;

		/*
		 * prepend SQUASH_MSG here if it exists and a
		 * "merge --squash" was originally performed
		 */
		اگر (!stat(git_راستہ_squash_پیغام(the_repository), &statbuf)) {
			اگر (strbuf_read_file(&sb, git_راستہ_squash_پیغام(the_repository), 0) < 0)
				die_errno(_("could not read SQUASH_MSG"));
			hook_arg1 = "squash";
		} ورنہ
			hook_arg1 = "merge";

		ملاؤ_پیغام_آغاز = sb.len;
		اگر (strbuf_read_file(&sb, git_راستہ_ملاؤ_پیغام(the_repository), 0) < 0)
			die_errno(_("could not read MERGE_MSG"));

		اگر (cleanup_منوال == COMMIT_پیغام_CLEANUP_SCISSORS &&
		    wt_حالت_locate_انتہا(sb.buf + ملاؤ_پیغام_آغاز,
					 sb.len - ملاؤ_پیغام_آغاز) <
				sb.len - ملاؤ_پیغام_آغاز)
			s->added_cut_سطر = 1;
	} ورنہ اگر (!stat(git_راستہ_squash_پیغام(the_repository), &statbuf)) {
		اگر (strbuf_read_file(&sb, git_راستہ_squash_پیغام(the_repository), 0) < 0)
			die_errno(_("could not read SQUASH_MSG"));
		hook_arg1 = "squash";
	} ورنہ اگر (template_file) {
		اگر (strbuf_read_file(&sb, template_file, 0) < 0)
			die_errno(_("could not read '%s'"), template_file);
		hook_arg1 = "template";
		clean_پیغام_contents = 0;
	}

	/*
	 * The remaining cases don't modify the template message, but
	 * just set the argument(s) to the prepare-commit-msg hook.
	 */
	ورنہ اگر (whence == FROM_ملاؤ)
		hook_arg1 = "merge";
	ورنہ اگر (is_from_cherry_pick(whence) || whence == FROM_REBASE_PICK) {
		hook_arg1 = "commit";
		hook_arg2 = "CHERRY_PICK_HEAD";
	}

	اگر (squash_پیغام) {
		/*
		 * If squash_commit was used for the commit subject,
		 * then we're possibly hijacking other commit log options.
		 * Reset the hook args to tell the real story.
		 */
		hook_arg1 = "message";
		hook_arg2 = "";
	}

	s->fp = fopen_for_writing(git_راستہ_commit_editmsg());
	اگر (!s->fp)
		die_errno(_("could not open '%s'"), git_راستہ_commit_editmsg());

	/* Ignore status.displayCommentPrefix: we do need comments in COMMIT_EDITMSG. */
	old_display_comment_سابقہ = s->display_comment_سابقہ;
	s->display_comment_سابقہ = 1;

	/*
	 * Most hints are counter-productive when the commit has
	 * already started.
	 */
	s->hints = 0;

	اگر (clean_پیغام_contents)
		strbuf_stripspace(&sb, تہی);

	اگر (signoff)
		append_signoff(&sb, ignored_log_پیغام_بائٹیں(sb.buf, sb.len), 0);

	اگر (fwrite(sb.buf, 1, sb.len, s->fp) < sb.len)
		die_errno(_("could not write commit template"));

#اگر_غیر_تعریف_شدہ WITH_BREAKING_CHANGES
	اگر (auto_comment_سطر_char)
		adjust_comment_سطر_char(&sb);
#اگر_ختم /* !WITH_BREAKING_CHANGES */
	strbuf_release(&sb);

	/* This checks if committer ident is explicitly given */
	strbuf_addstr(&committer_ident, git_committer_info(IDENT_STRICT));
	اگر (use_editor && include_حالت) {
		صحیح ident_shown = 0;
		شمار git_colorbool saved_color_setting;
		ساخت ident_بانٹو ci, ai;
		ثابت حرف *hint_cleanup_all = allow_خالی_خانے_پیغام ?
			_("Please enter the commit message for your changes."
			  " Lines starting\nwith '%s' will be ignored.\n") :
			_("Please enter the commit message for your changes."
			  " Lines starting\nwith '%s' will be ignored, and an empty"
			  " message aborts the commit.\n");
		ثابت حرف *hint_cleanup_space = allow_خالی_خانے_پیغام ?
			_("Please enter the commit message for your changes."
			  " Lines starting\n"
			  "with '%s' will be kept; you may remove them"
			  " yourself if you want to.\n") :
			_("Please enter the commit message for your changes."
			  " Lines starting\n"
			  "with '%s' will be kept; you may remove them"
			  " yourself if you want to.\n"
			  "An empty message aborts the commit.\n");
		اگر (whence != FROM_COMMIT) {
			اگر (cleanup_منوال == COMMIT_پیغام_CLEANUP_SCISSORS)
				wt_حالت_جوڑو_cut_سطر(s);
			حالت_printf_ln(
				s, GIT_COLOR_معمول,
				whence == FROM_ملاؤ ?
					      _("\n"
					  "It looks like you may be committing a merge.\n"
					  "If this is not correct, please run\n"
					  "	git update-ref -d MERGE_HEAD\n"
					  "and try again.\n") :
					      _("\n"
					  "It looks like you may be committing a cherry-pick.\n"
					  "If this is not correct, please run\n"
					  "	git update-ref -d CHERRY_PICK_HEAD\n"
					  "and try again.\n"));
		}

		fprintf(s->fp, "\n");
		اگر (cleanup_منوال == COMMIT_پیغام_CLEANUP_ALL)
			حالت_printf(s, GIT_COLOR_معمول, hint_cleanup_all, comment_سطر_str);
		ورنہ اگر (cleanup_منوال == COMMIT_پیغام_CLEANUP_SCISSORS) {
			اگر (whence == FROM_COMMIT)
				wt_حالت_جوڑو_cut_سطر(s);
		} ورنہ /* COMMIT_MSG_CLEANUP_SPACE, that is. */
			حالت_printf(s, GIT_COLOR_معمول, hint_cleanup_space, comment_سطر_str);

		/*
		 * These should never fail because they come from our own
		 * fmt_ident. They may fail the sane_ident test, but we know
		 * that the name and mail pointers will at least be valid,
		 * which is enough for our tests and printing here.
		 */
		assert_بانٹو_ident(&ai, author_ident);
		assert_بانٹو_ident(&ci, &committer_ident);

		اگر (ident_cmp(&ai, &ci))
			حالت_printf_ln(s, GIT_COLOR_معمول,
				_("%s"
				"Author:    %.*s <%.*s>"),
				ident_shown++ ? "" : "\n",
				(صحیح)(ai.name_انتہا - ai.name_begin), ai.name_begin,
				(صحیح)(ai.mail_انتہا - ai.mail_begin), ai.mail_begin);

		اگر (author_date_is_interesting())
			حالت_printf_ln(s, GIT_COLOR_معمول,
				_("%s"
				"Date:      %s"),
				ident_shown++ ? "" : "\n",
				show_ident_date(&ai, DATE_منوال(معمول)));

		اگر (!committer_ident_sufficiently_given())
			حالت_printf_ln(s, GIT_COLOR_معمول,
				_("%s"
				"Committer: %.*s <%.*s>"),
				ident_shown++ ? "" : "\n",
				(صحیح)(ci.name_انتہا - ci.name_begin), ci.name_begin,
				(صحیح)(ci.mail_انتہا - ci.mail_begin), ci.mail_begin);

		حالت_printf_ln(s, GIT_COLOR_معمول, "%s", ""); /* Add new line for clarity */

		saved_color_setting = s->use_color;
		s->use_color = GIT_COLOR_NEVER;
		committable = تسلسل_حالت(s->fp, اشاریہ_file, سابقہ, 1, s);
		s->use_color = saved_color_setting;
		string_list_clear_func(&s->change, change_ڈیٹا_free);
	} ورنہ {
		ساخت object_id oid;
		ثابت حرف *والد = "HEAD";

		اگر (!the_repository->اشاریہ->initialized && repo_read_اشاریہ(the_repository) < 0)
			die(_("Cannot read index"));

		اگر (amend)
			والد = "HEAD^1";

		اگر (repo_get_oid(the_repository, والد, &oid)) {
			صحیح i, ita_nr = 0;

			/* TODO: audit for interaction with sparse-index. */
			ensure_بھرا_اشاریہ(the_repository->اشاریہ);
			برائے (i = 0; i < the_repository->اشاریہ->cache_nr; i++)
				اگر (ce_intent_مقصد_جوڑو(the_repository->اشاریہ->cache[i]))
					ita_nr++;
			committable = the_repository->اشاریہ->cache_nr > ita_nr;
		} ورنہ {
			/*
			 * Unless the user did explicitly request a submodule
			 * ignore mode by passing a command line option we do
			 * not ignore any changed submodule SHA-1s when
			 * comparing index and parent, no matter what is
			 * configured. Otherwise we won't commit any
			 * submodules which were manually staged, which would
			 * be really confusing.
			 */
			ساخت diff_flags flags = DIFF_FLAGS_INIT;
			flags.override_submodule_config = 1;
			اگر (ignore_submodule_arg &&
			    !strcmp(ignore_submodule_arg, "all"))
				flags.ignore_submodules = 1;
			committable = اشاریہ_differs_from(the_repository,
							 والد, &flags, 1);
		}
	}
	strbuf_release(&committer_ident);

	fclose(s->fp);

	اگر (trailer_دلائل.nr) {
		اگر (amend_file_with_trailers(git_راستہ_commit_editmsg(), &trailer_دلائل))
			die(_("unable to pass trailers to --trailers"));
		strvec_clear(&trailer_دلائل);
	}

	/*
	 * Reject an attempt to record a non-merge empty commit without
	 * explicit --allow-empty. In the cherry-pick case, it may be
	 * empty due to conflict resolution, which the user should okay.
	 */
	اگر (!committable && whence != FROM_ملاؤ && !allow_خالی_خانے &&
	    !(amend && is_a_ملاؤ(current_سر))) {
		s->hints = advice_enabled(ADVICE_حالت_HINTS);
		s->display_comment_سابقہ = old_display_comment_سابقہ;
		تسلسل_حالت(stdout, اشاریہ_file, سابقہ, 0, s);
		اگر (amend)
			fputs(_(خالی_خانے_amend_advice), stderr);
		ورنہ اگر (is_from_cherry_pick(whence) ||
			 whence == FROM_REBASE_PICK) {
			fputs(_(خالی_خانے_cherry_pick_advice), stderr);
			اگر (whence == FROM_CHERRY_PICK_SINGLE)
				fputs(_(خالی_خانے_cherry_pick_advice_single), stderr);
			ورنہ اگر (whence == FROM_CHERRY_PICK_MULTI)
				fputs(_(خالی_خانے_cherry_pick_advice_multi), stderr);
			ورنہ
				fputs(_(خالی_خانے_rebase_pick_advice), stderr);
		}
		واپس 0;
	}

	اگر (!no_verify && invoked_hook) {
		/*
		 * Re-read the index as the pre-commit-commit hook was invoked
		 * and could have updated it. We must do this before we invoke
		 * the editor and after we invoke run_status above.
		 */
		discard_اشاریہ(the_repository->اشاریہ);
	}
	read_اشاریہ_from(the_repository->اشاریہ, اشاریہ_file, repo_get_git_dir(the_repository));

	اگر (cache_درخت_update(the_repository->اشاریہ, 0)) {
		error(_("Error building trees"));
		واپس 0;
	}

	اگر (تسلسل_commit_hook(use_editor, اشاریہ_file, تہی, "prepare-commit-msg",
			    git_راستہ_commit_editmsg(), hook_arg1, hook_arg2, تہی))
		واپس 0;

	اگر (use_editor) {
		ساخت strvec env = STRVEC_INIT;

		strvec_pushf(&env, "GIT_INDEX_FILE=%s", اشاریہ_file);
		اگر (launch_editor(git_راستہ_commit_editmsg(), تہی, env.v)) {
			fprintf(stderr,
			_("Please supply the message using either -m or -F option.\n"));
			باہر_نکلو(1);
		}
		strvec_clear(&env);
	}

	اگر (!no_verify &&
	    تسلسل_commit_hook(use_editor, اشاریہ_file, تہی, "commit-msg",
			    git_راستہ_commit_editmsg(), تہی)) {
		واپس 0;
	}

	واپس 1;
}

ساکن ثابت حرف *ڈھونڈو_author_by_nickname(ثابت حرف *name)
{
	ساخت rev_info revs;
	ساخت commit *commit;
	ساخت strbuf buf = STRBUF_INIT;
	ثابت حرف *av[20];
	صحیح ac = 0;

	repo_init_revisions(the_repository, &revs, تہی);
	strbuf_addf(&buf, "--author=%s", name);
	av[++ac] = "--all";
	av[++ac] = "-i";
	av[++ac] = buf.buf;
	av[++ac] = تہی;
	تیاری_revisions(ac, av, &revs, تہی);
	revs.mailmap = xmalloc(حجم(ساخت string_list));
	string_list_init_nodup(revs.mailmap);
	read_mailmap(the_repository, revs.mailmap);

	اگر (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	commit = get_revision(&revs);
	اگر (commit) {
		ساخت pretty_print_context ctx = {0};
		ctx.date_منوال.type = DATE_معمول;
		strbuf_release(&buf);
		repo_format_commit_پیغام(the_repository, commit,
					   "%aN <%aE>", &buf, &ctx);
		release_revisions(&revs);
		واپس strbuf_detach(&buf, تہی);
	}
	die(_("--author '%s' is not 'Name <email>' and matches no existing author"), name);
}

ساکن خالی handle_ignored_arg(ساخت wt_حالت *s)
{
	اگر (!ignored_arg)
		; /* default already initialized */
	ورنہ اگر (!strcmp(ignored_arg, "traditional"))
		s->show_ignored_منوال = SHOW_TRADITIONAL_IGNORED;
	ورنہ اگر (!strcmp(ignored_arg, "no"))
		s->show_ignored_منوال = SHOW_NO_IGNORED;
	ورنہ اگر (!strcmp(ignored_arg, "matching"))
		s->show_ignored_منوال = SHOW_MATCHING_IGNORED;
	ورنہ
		die(_("Invalid ignored mode '%s'"), ignored_arg);
}

ساکن شمار untracked_حالت_type parse_untracked_setting_name(ثابت حرف *u)
{
	/*
	 * Please update $__git_untracked_file_modes in
	 * git-completion.bash when you add new options
	 */
	انتخاب (git_parse_maybe_bool(u)) {
	حالت 0:
		u = "no";
		توڑو;
	حالت 1:
		u = "normal";
		توڑو;
	طےشدہ:
		توڑو;
	}

	اگر (!strcmp(u, "no"))
		واپس SHOW_NO_UNTRACKED_FILES;
	ورنہ اگر (!strcmp(u, "normal"))
		واپس SHOW_معمول_UNTRACKED_FILES;
	ورنہ اگر (!strcmp(u, "all"))
		واپس SHOW_ALL_UNTRACKED_FILES;
	ورنہ
		واپس SHOW_UNTRACKED_FILES_ERROR;
}

ساکن خالی handle_untracked_files_arg(ساخت wt_حالت *s)
{
	شمار untracked_حالت_type u;

	اگر (!untracked_files_arg)
		واپس; /* default already initialized */

	u = parse_untracked_setting_name(untracked_files_arg);
	اگر (u == SHOW_UNTRACKED_FILES_ERROR)
		die(_("Invalid untracked files mode '%s'"),
		    untracked_files_arg);
	s->show_untracked_files = u;
}

ساکن ثابت حرف *read_commit_پیغام_2(ثابت حرف *name)
{
	ثابت حرف *برآمد_enc;
	ساخت commit *commit;

	commit = lookup_commit_reference_by_name(name);
	اگر (!commit)
		die(_("could not lookup commit '%s'"), name);
	برآمد_enc = get_commit_output_encoding();
	واپس repo_logmsg_reencode(the_repository, commit, تہی, برآمد_enc);
}

/*
 * Enumerate what needs to be propagated when --porcelain
 * is not in effect here.
 */
ساکن ساخت حالت_deferred_config {
	شمار wt_حالت_format حالت_format;
	صحیح show_branch;
	شمار ahead_behind_flags ahead_behind;
} حالت_deferred_config = {
	حالت_FORMAT_UNSPECIFIED,
	-1, /* unspecified */
	AHEAD_BEHIND_UNSPECIFIED,
};

ساکن خالی finalize_deferred_config(ساخت wt_حالت *s)
{
	صحیح use_deferred_config = (حالت_format != حالت_FORMAT_PORCELAIN &&
				   حالت_format != حالت_FORMAT_PORCELAIN_V2 &&
				   !s->null_termination);

	اگر (s->null_termination) {
		اگر (حالت_format == حالت_FORMAT_NONE ||
		    حالت_format == حالت_FORMAT_UNSPECIFIED)
			حالت_format = حالت_FORMAT_PORCELAIN;
		ورنہ اگر (حالت_format == حالت_FORMAT_LONG)
			die(_("options '%s' and '%s' cannot be used together"), "--long", "-z");
	}

	اگر (use_deferred_config && حالت_format == حالت_FORMAT_UNSPECIFIED)
		حالت_format = حالت_deferred_config.حالت_format;
	اگر (حالت_format == حالت_FORMAT_UNSPECIFIED)
		حالت_format = حالت_FORMAT_NONE;

	اگر (use_deferred_config && s->show_branch < 0)
		s->show_branch = حالت_deferred_config.show_branch;
	اگر (s->show_branch < 0)
		s->show_branch = 0;

	/*
	 * If the user did not give a "--[no]-ahead-behind" command
	 * line argument *AND* we will print in a human-readable format
	 * (short, long etc.) then we inherit from the status.aheadbehind
	 * config setting.  In all other cases (and porcelain V[12] formats
	 * in particular), we inherit _FULL for backwards compatibility.
	 */
	اگر (use_deferred_config &&
	    s->ahead_behind_flags == AHEAD_BEHIND_UNSPECIFIED)
		s->ahead_behind_flags = حالت_deferred_config.ahead_behind;

	اگر (s->ahead_behind_flags == AHEAD_BEHIND_UNSPECIFIED)
		s->ahead_behind_flags = AHEAD_BEHIND_بھرا;
}

ساکن خالی check_fixup_reword_options(صحیح argc, ثابت حرف *argv[]) {
	اگر (whence != FROM_COMMIT) {
		اگر (whence == FROM_ملاؤ)
			die(_("You are in the middle of a merge -- cannot reword."));
		ورنہ اگر (is_from_cherry_pick(whence))
			die(_("You are in the middle of a cherry-pick -- cannot reword."));
	}
	اگر (argc)
		die(_("reword option of '%s' and path '%s' cannot be used together"), "--fixup", *argv);
	اگر (patch_interactive || interactive || all || also || only)
		die(_("reword option of '%s' and '%s' cannot be used together"),
			"--fixup", "--patch/--interactive/--all/--include/--only");
}

ساکن صحیح parse_and_validate_options(صحیح argc, ثابت حرف *argv[],
				      ثابت ساخت option *options,
				      ثابت حرف * ثابت usage[],
				      ثابت حرف *سابقہ,
				      ساخت commit *current_سر,
				      ساخت wt_حالت *s)
{
	argc = parse_options(argc, argv, سابقہ, options, usage, 0);
	finalize_deferred_config(s);

	اگر (force_author && !strchr(force_author, '>'))
		force_author = ڈھونڈو_author_by_nickname(force_author);

	اگر (force_author && renew_authorship)
		die(_("options '%s' and '%s' cannot be used together"), "--reset-author", "--author");

	اگر (logfile || have_option_m || use_پیغام)
		use_editor = 0;

	/* Sanity check options */
	اگر (amend && !current_سر)
		die(_("You have nothing to amend."));
	اگر (amend && whence != FROM_COMMIT) {
		اگر (whence == FROM_ملاؤ)
			die(_("You are in the middle of a merge -- cannot amend."));
		ورنہ اگر (is_from_cherry_pick(whence))
			die(_("You are in the middle of a cherry-pick -- cannot amend."));
		ورنہ اگر (whence == FROM_REBASE_PICK)
			die(_("You are in the middle of a rebase -- cannot amend."));
	}
	اگر (fixup_پیغام && squash_پیغام)
		die(_("options '%s' and '%s' cannot be used together"), "--squash", "--fixup");
	die_for_incompatible_opt4(!!use_پیغام, "-C",
				  !!edit_پیغام, "-c",
				  !!logfile, "-F",
				  !!fixup_پیغام, "--fixup");
	die_for_incompatible_opt4(have_option_m, "-m",
				  !!edit_پیغام, "-c",
				  !!use_پیغام, "-C",
				  !!logfile, "-F");
	اگر (use_پیغام || edit_پیغام || logfile ||fixup_پیغام || have_option_m)
		FREE_AND_NULL(template_file);
	اگر (edit_پیغام)
		use_پیغام = edit_پیغام;
	اگر (amend && !use_پیغام && !fixup_پیغام)
		use_پیغام = "HEAD";
	اگر (!use_پیغام && !is_from_cherry_pick(whence) &&
	    !is_from_rebase(whence) && renew_authorship)
		die(_("--reset-author can be used only with -C, -c or --amend."));
	اگر (use_پیغام) {
		use_پیغام_بفر = read_commit_پیغام_2(use_پیغام);
		اگر (!renew_authorship) {
			author_پیغام = use_پیغام;
			author_پیغام_بفر = use_پیغام_بفر;
		}
	}
	اگر ((is_from_cherry_pick(whence) || whence == FROM_REBASE_PICK) &&
	    !renew_authorship) {
		author_پیغام = "CHERRY_PICK_HEAD";
		author_پیغام_بفر = read_commit_پیغام_2(author_پیغام);
	}

	اگر (patch_interactive)
		interactive = 1;

	die_for_incompatible_opt4(also, "-i/--include",
				  only, "-o/--only",
				  all, "-a/--all",
				  interactive, "--interactive/-p/--patch");
	اگر (fixup_پیغام) {
		/*
		 * We limit --fixup's suboptions to only alpha characters.
		 * If the first character after a run of alpha is colon,
		 * then the part before the colon may be a known suboption
		 * name like `amend` or `reword`, or a misspelt suboption
		 * name. In either case, we treat it as
		 * --fixup=<suboption>:<arg>.
		 *
		 * Otherwise, we are dealing with --fixup=<commit>.
		 */
		حرف *p = fixup_پیغام;
		جب (حرف_ہے(*p))
			p++;
		اگر (p > fixup_پیغام && *p == ':') {
			*p = '\0';
			fixup_commit = p + 1;
			اگر (!strcmp("amend", fixup_پیغام) ||
			    !strcmp("reword", fixup_پیغام)) {
				fixup_سابقہ = "amend";
				allow_خالی_خانے = 1;
				اگر (*fixup_پیغام == 'r') {
					check_fixup_reword_options(argc, argv);
					only = 1;
				}
			} ورنہ {
				die(_("unknown option: --fixup=%s:%s"), fixup_پیغام, fixup_commit);
			}
		} ورنہ {
			fixup_commit = fixup_پیغام;
			fixup_سابقہ = "fixup";
			use_editor = 0;
		}
	}

	اگر (0 <= edit_flag)
		use_editor = edit_flag;

	handle_untracked_files_arg(s);

	اگر (all && argc > 0)
		die(_("paths '%s ...' with -a does not make sense"),
		    argv[0]);

	اگر (حالت_format != حالت_FORMAT_NONE)
		dry_تسلسل = 1;

	واپس argc;
}

ساکن صحیح dry_تسلسل_commit(ثابت حرف **argv, ثابت حرف *سابقہ,
			  ثابت ساخت commit *current_سر, ساخت wt_حالت *s)
{
	صحیح committable;
	ثابت حرف *اشاریہ_file;

	اشاریہ_file = prepare_اشاریہ(argv, سابقہ, current_سر, 1);
	committable = تسلسل_حالت(stdout, اشاریہ_file, سابقہ, 0, s);
	rollback_اشاریہ_files();

	واپس committable ? 0 : 1;
}

define_list_config_array_extra(color_حالت_slots, {"added"});

ساکن صحیح parse_حالت_خانہ(ثابت حرف *خانہ)
{
	اگر (!strcasecmp(خانہ, "added"))
		واپس WT_حالت_UPDATED;

	واپس LOOKUP_CONFIG(color_حالت_slots, خانہ);
}

ساکن صحیح git_حالت_config(ثابت حرف *k, ثابت حرف *v,
			     ثابت ساخت config_context *ctx, خالی *cb)
{
	ساخت wt_حالت *s = cb;
	ثابت حرف *خانہ_name;

	اگر (starts_with(k, "column."))
		واپس git_عمود_config(k, v, "status", &s->colopts);
	اگر (!strcmp(k, "status.submodulesummary")) {
		صحیح is_bool;
		s->submodule_summary = git_config_bool_or_int(k, v, ctx->kvi,
							      &is_bool);
		اگر (is_bool && s->submodule_summary)
			s->submodule_summary = -1;
		واپس 0;
	}
	اگر (!strcmp(k, "status.short")) {
		اگر (git_config_bool(k, v))
			حالت_deferred_config.حالت_format = حالت_FORMAT_SHORT;
		ورنہ
			حالت_deferred_config.حالت_format = حالت_FORMAT_NONE;
		واپس 0;
	}
	اگر (!strcmp(k, "status.branch")) {
		حالت_deferred_config.show_branch = git_config_bool(k, v);
		واپس 0;
	}
	اگر (!strcmp(k, "status.aheadbehind")) {
		حالت_deferred_config.ahead_behind = git_config_bool(k, v);
		واپس 0;
	}
	اگر (!strcmp(k, "status.showstash")) {
		s->show_stash = git_config_bool(k, v);
		واپس 0;
	}
	اگر (!strcmp(k, "status.color") || !strcmp(k, "color.status")) {
		s->use_color = git_config_colorbool(k, v);
		واپس 0;
	}
	اگر (!strcmp(k, "status.displaycommentprefix")) {
		s->display_comment_سابقہ = git_config_bool(k, v);
		واپس 0;
	}
	اگر (skip_سابقہ(k, "status.color.", &خانہ_name) ||
	    skip_سابقہ(k, "color.status.", &خانہ_name)) {
		صحیح خانہ = parse_حالت_خانہ(خانہ_name);
		اگر (خانہ < 0)
			واپس 0;
		اگر (!v)
			واپس config_error_nonbool(k);
		واپس color_parse(v, s->color_palette[خانہ]);
	}
	اگر (!strcmp(k, "status.relativepaths")) {
		s->relative_paths = git_config_bool(k, v);
		واپس 0;
	}
	اگر (!strcmp(k, "status.showuntrackedfiles")) {
		شمار untracked_حالت_type u;

		u = parse_untracked_setting_name(v);
		اگر (u == SHOW_UNTRACKED_FILES_ERROR)
			واپس error(_("Invalid untracked files mode '%s'"), v);
		s->show_untracked_files = u;
		واپس 0;
	}
	اگر (!strcmp(k, "diff.renamelimit")) {
		اگر (s->rename_حد_بندی == -1)
			s->rename_حد_بندی = git_config_int(k, v, ctx->kvi);
		واپس 0;
	}
	اگر (!strcmp(k, "status.renamelimit")) {
		s->rename_حد_بندی = git_config_int(k, v, ctx->kvi);
		واپس 0;
	}
	اگر (!strcmp(k, "diff.renames")) {
		اگر (s->detect_rename == -1)
			s->detect_rename = git_config_rename(k, v);
		واپس 0;
	}
	اگر (!strcmp(k, "status.renames")) {
		s->detect_rename = git_config_rename(k, v);
		واپس 0;
	}
	واپس git_diff_ui_config(k, v, ctx, تہی);
}

صحیح cmd_حالت(صحیح argc,
ثابت حرف **argv,
ثابت حرف *سابقہ,
ساخت repository *repo غیر_مستعمل)
{
	ساکن صحیح no_renames = -1;
	ساکن ثابت حرف *rename_score_arg = (ثابت حرف *)-1;
	ساکن ساخت wt_حالت s;
	غیرعلامتی صحیح پیش_رفت_flag = 0;
	صحیح fd;
	ساخت object_id oid;
	ساکن ساخت option builtin_حالت_options[] = {
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT_سیٹ_INT('s', "short", &حالت_format,
			    N_("show status concisely"), حالت_FORMAT_SHORT),
		OPT_BOOL('b', "branch", &s.show_branch,
			 N_("show branch information")),
		OPT_BOOL(0, "show-stash", &s.show_stash,
			 N_("show stash information")),
		OPT_BOOL(0, "ahead-behind", &s.ahead_behind_flags,
			 N_("compute full ahead/behind values")),
		OPT_CALLBACK_F(0, "porcelain", &حالت_format,
		  N_("version"), N_("machine-readable output"),
		  PARSE_OPT_OPTARG, opt_parse_porcelain),
		OPT_سیٹ_INT(0, "long", &حالت_format,
			    N_("show status in long format (default)"),
			    حالت_FORMAT_LONG),
		OPT_BOOL('z', "null", &s.null_termination,
			 N_("terminate entries with NUL")),
		{
			.type = OPTION_STRING,
			.short_name = 'u',
			.long_name = "untracked-files",
			.قدر = &untracked_files_arg,
			.argh = N_("mode"),
			.help = N_("show untracked files, optional modes: all, normal, no. (Default: all)"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t)"all",
		},
		{
			.type = OPTION_STRING,
			.long_name = "ignored",
			.قدر = &ignored_arg,
			.argh = N_("mode"),
			.help = N_("show ignored files, optional modes: traditional, matching, no. (Default: traditional)"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t)"traditional",
		},
		{
			.type = OPTION_STRING,
			.long_name = "ignore-submodules",
			.قدر = &ignore_submodule_arg,
			.argh = N_("when"),
			.help = N_("ignore changes to submodules, optional when: all, dirty, untracked. (Default: all)"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t)"all",
		},
		OPT_عمود(0, "column", &s.colopts, N_("list untracked files in columns")),
		OPT_BOOL(0, "no-renames", &no_renames, N_("do not detect renames")),
		OPT_CALLBACK_F('M', "find-renames", &rename_score_arg,
		  N_("n"), N_("detect renames, optionally set similarity index"),
		  PARSE_OPT_OPTARG | PARSE_OPT_NONEG, opt_parse_rename_score),
		OPT_انتہا(),
	};

	show_usage_with_options_if_asked(argc, argv,
					 builtin_حالت_usage, builtin_حالت_options);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_بھرا_اشاریہ = 0;

	حالت_init_config(&s, git_حالت_config);
	argc = parse_options(argc, argv, سابقہ,
			     builtin_حالت_options,
			     builtin_حالت_usage, 0);
	finalize_colopts(&s.colopts, -1);
	finalize_deferred_config(&s);

	handle_untracked_files_arg(&s);
	handle_ignored_arg(&s);

	اگر (s.show_ignored_منوال == SHOW_MATCHING_IGNORED &&
	    s.show_untracked_files == SHOW_NO_UNTRACKED_FILES)
		die(_("Unsupported combination of ignored and untracked-files arguments"));

	parse_pathspec(&s.pathspec, 0,
		       PATHSPEC_PREFER_بھرا,
		       سابقہ, argv);

	اگر (حالت_format != حالت_FORMAT_PORCELAIN &&
	    حالت_format != حالت_FORMAT_PORCELAIN_V2)
		پیش_رفت_flag = REFRESH_پیش_رفت;
	repo_read_اشاریہ(the_repository);
	refresh_اشاریہ(the_repository->اشاریہ,
		      REFRESH_QUIET|REFRESH_UNMERGED|پیش_رفت_flag,
		      &s.pathspec, تہی, تہی);

	اگر (use_optional_locks())
		fd = repo_hold_locked_اشاریہ(the_repository, &اشاریہ_تالا, 0);
	ورنہ
		fd = -1;

	s.is_initial = repo_get_oid(the_repository, s.reference, &oid) ? 1 : 0;
	اگر (!s.is_initial)
		oidcpy(&s.oid_commit, &oid);

	s.ignore_submodule_arg = ignore_submodule_arg;
	s.حالت_format = حالت_format;
	s.verbose = verbose;
	اگر (no_renames != -1)
		s.detect_rename = !no_renames;
	اگر ((intptr_t)rename_score_arg != -1) {
		اگر (s.detect_rename < DIFF_DETECT_RENAME)
			s.detect_rename = DIFF_DETECT_RENAME;
		اگر (rename_score_arg)
			s.rename_score = parse_rename_score(&rename_score_arg);
	}

	wt_حالت_collect(&s);

	اگر (0 <= fd)
		repo_update_اشاریہ_if_able(the_repository, &اشاریہ_تالا);

	اگر (s.relative_paths)
		s.سابقہ = سابقہ;

	wt_حالت_print(&s);
	wt_حالت_collect_free_buffers(&s);

	واپس 0;
}

ساکن صحیح git_commit_config(ثابت حرف *k, ثابت حرف *v,
			     ثابت ساخت config_context *ctx, خالی *cb)
{
	ساخت wt_حالت *s = cb;

	اگر (!strcmp(k, "commit.template"))
		واپس git_config_pathname(&template_file, k, v);
	اگر (!strcmp(k, "commit.status")) {
		include_حالت = git_config_bool(k, v);
		واپس 0;
	}
	اگر (!strcmp(k, "commit.cleanup")) {
		FREE_AND_NULL(cleanup_config);
		واپس git_config_string(&cleanup_config, k, v);
	}
	اگر (!strcmp(k, "commit.gpgsign")) {
		sign_commit = git_config_bool(k, v) ? "" : تہی;
		واپس 0;
	}
	اگر (!strcmp(k, "commit.verbose")) {
		صحیح is_bool;
		config_commit_verbose = git_config_bool_or_int(k, v, ctx->kvi,
							       &is_bool);
		واپس 0;
	}

	واپس git_حالت_config(k, v, ctx, s);
}

صحیح cmd_commit(صحیح argc,
	       ثابت حرف **argv,
	       ثابت حرف *سابقہ,
	       ساخت repository *repo غیر_مستعمل)
{
	ساکن ساخت wt_حالت s;
	ساکن ثابت حرف *cleanup_arg = تہی;
	ساکن ساخت option builtin_commit_options[] = {
		OPT__QUIET(&quiet, N_("suppress summary after successful commit")),
		OPT__VERBOSE(&verbose, N_("show diff in commit message template")),

		OPT_گروہ(N_("Commit message options")),
		OPT_FILENAME('F', "file", &logfile, N_("read message from file")),
		OPT_STRING(0, "author", &force_author, N_("author"), N_("override author for commit")),
		OPT_STRING(0, "date", &force_date, N_("date"), N_("override date for commit")),
		OPT_CALLBACK('m', "message", &پیغام_2, N_("message"), N_("commit message"), opt_parse_m),
		OPT_STRING('c', "reedit-message", &edit_پیغام, N_("commit"), N_("reuse and edit message from specified commit")),
		OPT_STRING('C', "reuse-message", &use_پیغام, N_("commit"), N_("reuse message from specified commit")),
		/*
		 * TRANSLATORS: Leave "[(amend|reword):]" as-is,
		 * and only translate <commit>.
		 */
		OPT_STRING(0, "fixup", &fixup_پیغام, N_("[(amend|reword):]commit"), N_("use autosquash formatted message to fixup or amend/reword specified commit")),
		OPT_STRING(0, "squash", &squash_پیغام, N_("commit"), N_("use autosquash formatted message to squash specified commit")),
		OPT_BOOL(0, "reset-author", &renew_authorship, N_("the commit is authored by me now (used with -C/-c/--amend)")),
		OPT_STRVEC(0, "trailer", &trailer_دلائل, N_("trailer"),
			   N_("add custom trailer(s)")),
		OPT_BOOL('s', "signoff", &signoff, N_("add a Signed-off-by trailer")),
		OPT_FILENAME('t', "template", &template_file, N_("use specified template file")),
		OPT_BOOL('e', "edit", &edit_flag, N_("force edit of commit")),
		OPT_CLEANUP(&cleanup_arg),
		OPT_BOOL(0, "status", &include_حالت, N_("include status in commit message template")),
		{
			.type = OPTION_STRING,
			.short_name = 'S',
			.long_name = "gpg-sign",
			.قدر = &sign_commit,
			.argh = N_("key-id"),
			.help = N_("GPG sign commit"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "",
		},
		/* end commit message options */

		OPT_گروہ(N_("Commit contents options")),
		OPT_BOOL('a', "all", &all, N_("commit all changed files")),
		OPT_BOOL('i', "include", &also, N_("add specified files to index for commit")),
		OPT_BOOL(0, "interactive", &interactive, N_("interactively add files")),
		OPT_BOOL('p', "patch", &patch_interactive, N_("interactively add changes")),
		OPT_DIFF_UNIFIED(&interactive_opts.context),
		OPT_DIFF_INTERHUNK_CONTEXT(&interactive_opts.interhunkcontext),
		OPT_BOOL('o', "only", &only, N_("commit only specified files")),
		OPT_BOOL('n', "no-verify", &no_verify, N_("bypass pre-commit and commit-msg hooks")),
		OPT_BOOL(0, "dry-run", &dry_تسلسل, N_("show what would be committed")),
		OPT_سیٹ_INT(0, "short", &حالت_format, N_("show status concisely"),
			    حالت_FORMAT_SHORT),
		OPT_BOOL(0, "branch", &s.show_branch, N_("show branch information")),
		OPT_BOOL(0, "ahead-behind", &s.ahead_behind_flags,
			 N_("compute full ahead/behind values")),
		OPT_سیٹ_INT(0, "porcelain", &حالت_format,
			    N_("machine-readable output"), حالت_FORMAT_PORCELAIN),
		OPT_سیٹ_INT(0, "long", &حالت_format,
			    N_("show status in long format (default)"),
			    حالت_FORMAT_LONG),
		OPT_BOOL('z', "null", &s.null_termination,
			 N_("terminate entries with NUL")),
		OPT_BOOL(0, "amend", &amend, N_("amend previous commit")),
		OPT_BOOL(0, "no-post-rewrite", &no_post_rewrite, N_("bypass post-rewrite hook")),
		{
			.type = OPTION_STRING,
			.short_name = 'u',
			.long_name = "untracked-files",
			.قدر = &untracked_files_arg,
			.argh = N_("mode"),
			.help = N_("show untracked files, optional modes: all, normal, no. (Default: all)"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t)"all",
		},
		OPT_PATHSPEC_FROM_FILE(&pathspec_from_file),
		OPT_PATHSPEC_FILE_NUL(&pathspec_file_nul),
		/* end commit contents options */

		OPT_HIDDEN_BOOL(0, "allow-empty", &allow_خالی_خانے,
				N_("ok to record an empty change")),
		OPT_HIDDEN_BOOL(0, "allow-empty-message", &allow_خالی_خانے_پیغام,
				N_("ok to record a change with an empty message")),

		OPT_انتہا()
	};

	ساخت strbuf sb = STRBUF_INIT;
	ساخت strbuf author_ident = STRBUF_INIT;
	ثابت حرف *اشاریہ_file, *reflog_پیغام;
	ساخت object_id oid;
	ساخت commit_list *parents = تہی;
	ساخت stat statbuf;
	ساخت commit *current_سر = تہی;
	ساخت commit_extra_سرخی *extra = تہی;
	ساخت strbuf err = STRBUF_INIT;
	صحیح ret = 0;

	show_usage_with_options_if_asked(argc, argv,
					 builtin_commit_usage, builtin_commit_options);

#اگر_غیر_تعریف_شدہ WITH_BREAKING_CHANGES
	warn_on_auto_comment_char = true;
#اگر_ختم /* !WITH_BREAKING_CHANGES */
	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_بھرا_اشاریہ = 0;

	حالت_init_config(&s, git_commit_config);
	s.commit_template = 1;
	حالت_format = حالت_FORMAT_NONE; /* Ignore status.short */
	s.colopts = 0;

	اگر (repo_get_oid(the_repository, "HEAD", &oid))
		current_سر = تہی;
	ورنہ {
		current_سر = lookup_commit_or_die(&oid, "HEAD");
		اگر (repo_parse_commit(the_repository, current_سر))
			die(_("could not parse HEAD commit"));
	}
	verbose = -1; /* unspecified */
	argc = parse_and_validate_options(argc, argv, builtin_commit_options,
					  builtin_commit_usage,
					  سابقہ, current_سر, &s);
	اگر (trailer_دلائل.nr)
		trailer_config_init();

	اگر (verbose == -1)
		verbose = (config_commit_verbose < 0) ? 0 : config_commit_verbose;

	اگر (cleanup_arg) {
		آزاد(cleanup_config);
		cleanup_config = xstrdup(cleanup_arg);
	}
	cleanup_منوال = get_cleanup_منوال(cleanup_config, use_editor);

	اگر (dry_تسلسل)
		واپس dry_تسلسل_commit(argv, سابقہ, current_سر, &s);
	اشاریہ_file = prepare_اشاریہ(argv, سابقہ, current_سر, 0);

	/* Set up everything for writing the commit object.  This includes
	   running hooks, writing the trees, and interacting with the user.  */
	اگر (!prepare_مقصد_commit(اشاریہ_file, سابقہ,
			       current_سر, &s, &author_ident)) {
		ret = 1;
		rollback_اشاریہ_files();
		جاؤ cleanup;
	}

	/* Determine parents */
	reflog_پیغام = getenv("GIT_REFLOG_ACTION");
	اگر (!current_سر) {
		اگر (!reflog_پیغام)
			reflog_پیغام = "commit (initial)";
	} ورنہ اگر (amend) {
		اگر (!reflog_پیغام)
			reflog_پیغام = "commit (amend)";
		parents = commit_list_copy(current_سر->parents);
	} ورنہ اگر (whence == FROM_ملاؤ) {
		ساخت strbuf m = STRBUF_INIT;
		FILE *fp;
		صحیح allow_fast_forward = 1;
		ساخت commit_list **pptr = &parents;

		اگر (!reflog_پیغام)
			reflog_پیغام = "commit (merge)";
		pptr = commit_list_append(current_سر, pptr);
		fp = xfopen(git_راستہ_ملاؤ_سر(the_repository), "r");
		جب (strbuf_getline_lf(&m, fp) != EOF) {
			ساخت commit *والد;

			والد = get_ملاؤ_والد(m.buf);
			اگر (!والد)
				die(_("Corrupt MERGE_HEAD file (%s)"), m.buf);
			pptr = commit_list_append(والد, pptr);
		}
		fclose(fp);
		strbuf_release(&m);
		اگر (!stat(git_راستہ_ملاؤ_منوال(the_repository), &statbuf)) {
			اگر (strbuf_read_file(&sb, git_راستہ_ملاؤ_منوال(the_repository), 0) < 0)
				die_errno(_("could not read MERGE_MODE"));
			اگر (!strcmp(sb.buf, "no-ff"))
				allow_fast_forward = 0;
		}
		اگر (allow_fast_forward)
			reduce_heads_replace(&parents);
	} ورنہ {
		اگر (!reflog_پیغام)
			reflog_پیغام = is_from_cherry_pick(whence)
					? "commit (cherry-pick)"
					: is_from_rebase(whence)
					? "commit (rebase)"
					: "commit";
		commit_list_داخل_کرو(current_سر, &parents);
	}

	/* Finally, get the commit message */
	strbuf_reset(&sb);
	اگر (strbuf_read_file(&sb, git_راستہ_commit_editmsg(), 0) < 0) {
		صحیح saved_errno = errno;
		rollback_اشاریہ_files();
		die(_("could not read commit message: %s"), strerror(saved_errno));
	}

	cleanup_پیغام(&sb, cleanup_منوال, verbose);

	اگر (پیغام_is_خالی_خانے(&sb, cleanup_منوال) && !allow_خالی_خانے_پیغام) {
		rollback_اشاریہ_files();
		fprintf(stderr, _("Aborting commit due to empty commit message.\n"));
		باہر_نکلو(1);
	}
	اگر (template_untouched(&sb, template_file, cleanup_منوال) && !allow_خالی_خانے_پیغام) {
		rollback_اشاریہ_files();
		fprintf(stderr, _("Aborting commit; you did not edit the message.\n"));
		باہر_نکلو(1);
	}

	اگر (fixup_پیغام && starts_with(sb.buf, "amend! ") &&
	    !allow_خالی_خانے_پیغام) {
		ساخت strbuf body = STRBUF_INIT;
		سائز_t len = commit_subject_لمبائی(sb.buf);
		strbuf_addstr(&body, sb.buf + len);
		اگر (پیغام_is_خالی_خانے(&body, cleanup_منوال)) {
			rollback_اشاریہ_files();
			fprintf(stderr, _("Aborting commit due to empty commit message body.\n"));
			باہر_نکلو(1);
		}
		strbuf_release(&body);
	}

	اگر (amend) {
		ثابت حرف *exclude_gpgsig[3] = { "gpgsig", "gpgsig-sha256", تہی };
		extra = read_commit_extra_سرخیاں(current_سر, exclude_gpgsig);
	} ورنہ {
		ساخت commit_extra_سرخی **دم = &extra;
		append_ملاؤ_لیبل_سرخیاں(parents, &دم);
	}

	اگر (commit_درخت_extended(sb.buf, sb.len, &the_repository->اشاریہ->cache_درخت->oid,
				 parents, &oid, author_ident.buf, تہی,
				 sign_commit, extra)) {
		rollback_اشاریہ_files();
		die(_("failed to write commit object"));
	}

	اگر (update_سر_with_reflog(current_سر, &oid, reflog_پیغام, &sb,
				    &err)) {
		rollback_اشاریہ_files();
		die("%s", err.buf);
	}

	sequencer_post_commit_cleanup(the_repository, 0);
	unlink(git_راستہ_ملاؤ_سر(the_repository));
	unlink(git_راستہ_ملاؤ_پیغام(the_repository));
	unlink(git_راستہ_ملاؤ_منوال(the_repository));
	unlink(git_راستہ_squash_پیغام(the_repository));

	اگر (commit_اشاریہ_files())
		die(_("repository has been updated, but unable to write\n"
		      "new index file. Check that disk is not full and quota is\n"
		      "not exceeded, and then \"git restore --staged :/\" to recover."));

	git_test_write_commit_گراف_or_die(the_repository);

	repo_rerere(the_repository, 0);
	تسلسل_auto_maintenance(the_repository, quiet);
	تسلسل_commit_hook(use_editor, repo_get_اشاریہ_file(the_repository),
			تہی, "post-commit", تہی);
	اگر (amend && !no_post_rewrite) {
		commit_post_rewrite(the_repository, current_سر, &oid);
	}
	اگر (!quiet) {
		غیرعلامتی صحیح flags = 0;

		اگر (!current_سر)
			flags |= SUMMARY_INITIAL_COMMIT;
		اگر (author_date_is_interesting())
			flags |= SUMMARY_SHOW_AUTHOR_DATE;
		print_commit_summary(the_repository, سابقہ,
				     &oid, flags);
	}

	apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
			    تہی, تہی, تہی, تہی);

cleanup:
	free_commit_extra_سرخیاں(extra);
	commit_list_free(parents);
	strbuf_release(&author_ident);
	strbuf_release(&err);
	strbuf_release(&sb);
	آزاد(logfile);
	آزاد(template_file);
	واپس ret;
}
