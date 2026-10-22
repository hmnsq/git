/*
 * Builtin "git commit"
 *
 * Copyright (c) 2007 Kristian Høgsberg <krh@redhat.com>
 * Based on git-commit.sh by Junio C Hamano and Linus Torvalds
 */

#تعریف_کړه USE_THE_REPOSITORY_VARIABLE
#تعریف_کړه DISABLE_SIGN_COMPARE_WARNINGS

#ضمن "builtin.h"
#ضمن "advice.h"
#ضمن "config.h"
#ضمن "lockfile.h"
#ضمن "cache-tree.h"
#ضمن "color.h"
#ضمن "dir.h"
#ضمن "editor.h"
#ضمن "environment.h"
#ضمن "diff.h"
#ضمن "commit.h"
#ضمن "add-interactive.h"
#ضمن "gettext.h"
#ضمن "revision.h"
#ضمن "wt-status.h"
#ضمن "run-command.h"
#ضمن "strbuf.h"
#ضمن "object-name.h"
#ضمن "parse-options.h"
#ضمن "path.h"
#ضمن "preload-index.h"
#ضمن "read-cache.h"
#ضمن "repository.h"
#ضمن "string-list.h"
#ضمن "rerere.h"
#ضمن "unpack-trees.h"
#ضمن "column.h"
#ضمن "sequencer.h"
#ضمن "sparse-index.h"
#ضمن "mailmap.h"
#ضمن "help.h"
#ضمن "commit-reach.h"
#ضمن "commit-graph.h"
#ضمن "pretty.h"
#ضمن "trailer.h"

ساكن ثابت حرف * ثابت builtin_commit_usage[] = {
	N_("git commit [-a | --interactive | --patch] [-s] [-v] [-u[<mode>]] [--amend]\n"
	   "           [--dry-run] [(-c | -C | --squash) <commit> | --fixup [(amend|reword):]<commit>]\n"
	   "           [-F <file> | -m <msg>] [--reset-author] [--allow-empty]\n"
	   "           [--allow-empty-message] [--no-verify] [-e] [--author=<author>]\n"
	   "           [--date=<date>] [--cleanup=<mode>] [--[no-]status]\n"
	   "           [-i | -o] [--pathspec-from-file=<file> [--pathspec-file-nul]]\n"
	   "           [(--trailer <token>[(=|:)<value>])...] [-S[<keyid>]]\n"
	   "           [--] [<pathspec>...]"),
	عدم
};

ساكن ثابت حرف * ثابت builtin_حالت_usage[] = {
	N_("git status [<options>] [--] [<pathspec>...]"),
	عدم
};

ساكن ثابت حرف تشه_amend_advice[] =
N_("You asked to amend the most recent commit, but doing so would make\n"
"it empty. You can repeat your command with --allow-empty, or you can\n"
"remove the commit entirely with \"git reset HEAD^\".\n");

ساكن ثابت حرف تشه_cherry_pick_advice[] =
N_("The previous cherry-pick is now empty, possibly due to conflict resolution.\n"
"If you wish to commit it anyway, use:\n"
"\n"
"    git commit --allow-empty\n"
"\n");

ساكن ثابت حرف تشه_rebase_pick_advice[] =
N_("Otherwise, please use 'git rebase --skip'\n");

ساكن ثابت حرف تشه_cherry_pick_advice_single[] =
N_("Otherwise, please use 'git cherry-pick --skip'\n");

ساكن ثابت حرف تشه_cherry_pick_advice_multi[] =
N_("and then use:\n"
"\n"
"    git cherry-pick --continue\n"
"\n"
"to resume cherry-picking the remaining commits.\n"
"If you wish to skip this commit, use:\n"
"\n"
"    git cherry-pick --skip\n"
"\n");

ساكن ثابت حرف *color_حالت_slots[] = {
	[WT_حالت_سرلیک]	  = "header",
	[WT_حالت_UPDATED]	  = "updated",
	[WT_حالت_بدل_شوی]	  = "changed",
	[WT_حالت_UNTRACKED]	  = "untracked",
	[WT_حالت_NOBRANCH]	  = "noBranch",
	[WT_حالت_UNMERGED]	  = "unmerged",
	[WT_حالت_LOCAL_BRANCH]  = "localBranch",
	[WT_حالت_REMOTE_BRANCH] = "remoteBranch",
	[WT_حالت_ONBRANCH]	  = "branch",
};

ساكن ثابت حرف *use_پیغام_بفر;
ساكن بنية قلف_file شاخص_قلف; /* real index */
ساكن بنية قلف_file false_قلف; /* used only for partial commits */
ساكن تعداد {
	COMMIT_AS_IS = 1,
	COMMIT_نورمال,
	COMMIT_PARTIAL
} commit_style;

ساكن ثابت حرف *force_author;
ساكن حرف *logfile;
ساكن حرف *template_file;
/*
 * The _message variables are commit names from which to take
 * the commit message and/or authorship.
 */
ساكن ثابت حرف *author_پیغام, *author_پیغام_بفر;
ساكن ثابت حرف *edit_پیغام, *use_پیغام;
ساكن حرف *fixup_پیغام, *fixup_commit, *squash_پیغام;
ساكن ثابت حرف *fixup_مختاړی;
ساكن صحيح all, also, interactive, patch_interactive, only, amend, signoff;
ساكن بنية interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
ساكن صحيح edit_flag = -1; /* unspecified */
ساكن صحيح quiet, verbose, no_verify, allow_تشه, dry_پرلپسې, renew_authorship;
ساكن صحيح config_commit_verbose = -1; /* unspecified */
ساكن صحيح no_post_rewrite, allow_تشه_پیغام, pathspec_file_nul;
ساكن ثابت حرف *untracked_files_arg, *force_date, *ignore_submodule_arg, *ignored_arg;
ساكن ثابت حرف *sign_commit, *pathspec_from_file;
ساكن بنية strvec trailer_دلیلونه = STRVEC_INIT;

/*
 * The default commit message cleanup mode will remove the lines
 * beginning with # (shell comments) and leading and trailing
 * whitespaces (empty lines or containing only whitespaces)
 * if editor is used, and only the whitespaces if the message
 * is specified explicitly.
 */
ساكن تعداد commit_پیغام_cleanup_ډېرپېښ cleanup_ډېرپېښ;
ساكن حرف *cleanup_config;

ساكن تعداد commit_whence whence;
ساكن صحيح use_editor = 1, include_حالت = 1;
ساكن صحيح have_option_m;
ساكن بنية strbuf پیغام_2 = STRBUF_INIT;

ساكن تعداد wt_حالت_format حالت_format = حالت_FORMAT_UNSPECIFIED;

ساكن صحيح opt_parse_porcelain(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	تعداد wt_حالت_format *ارزښت = (تعداد wt_حالت_format *)opt->ارزښت;
	إذا (unset)
		*ارزښت = حالت_FORMAT_NONE;
	وإلا إذا (!arg)
		*ارزښت = حالت_FORMAT_PORCELAIN;
	وإلا إذا (!strcmp(arg, "v1") || !strcmp(arg, "1"))
		*ارزښت = حالت_FORMAT_PORCELAIN;
	وإلا إذا (!strcmp(arg, "v2") || !strcmp(arg, "2"))
		*ارزښت = حالت_FORMAT_PORCELAIN_V2;
	وإلا
		die("unsupported porcelain version '%s'", arg);

	أرجع 0;
}

ساكن صحيح opt_parse_m(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	بنية strbuf *buf = opt->ارزښت;
	إذا (unset) {
		have_option_m = 0;
		strbuf_setlen(buf, 0);
	} وإلا {
		have_option_m = 1;
		إذا (buf->len)
			strbuf_addch(buf, '\n');
		strbuf_addstr(buf, arg);
		strbuf_complete_کرښه(buf);
	}
	أرجع 0;
}

ساكن صحيح opt_parse_rename_score(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	ثابت حرف **ارزښت = opt->ارزښت;

	BUG_ON_OPT_NEG(unset);

	إذا (arg != عدم && *arg == '=')
		arg = arg + 1;

	*ارزښت = arg;
	أرجع 0;
}

ساكن فراغ determine_whence(بنية wt_حالت *s)
{
	إذا (file_exists(git_لار_یوځای_سر(the_repository)))
		whence = FROM_یوځای;
	وإلا إذا (!sequencer_determine_whence(the_repository, &whence))
		whence = FROM_COMMIT;
	إذا (s)
		s->whence = whence;
}

ساكن فراغ حالت_init_config(بنية wt_حالت *s, config_fn_t fn)
{
	wt_حالت_prepare(the_repository, s);
	init_diff_ui_defaults();
	repo_config(the_repository, fn, s);
	determine_whence(s);
	s->hints = advice_enabled(ADVICE_حالت_HINTS); /* must come after repo_config() */
}

ساكن فراغ rollback_شاخص_files(فراغ)
{
	بدل (commit_style) {
	حالة COMMIT_AS_IS:
		اكسر; /* nothing to do */
	حالة COMMIT_نورمال:
		rollback_قلف_file(&شاخص_قلف);
		اكسر;
	حالة COMMIT_PARTIAL:
		rollback_قلف_file(&شاخص_قلف);
		rollback_قلف_file(&false_قلف);
		اكسر;
	}
}

ساكن صحيح commit_شاخص_files(فراغ)
{
	صحيح err = 0;

	بدل (commit_style) {
	حالة COMMIT_AS_IS:
		اكسر; /* nothing to do */
	حالة COMMIT_نورمال:
		err = commit_قلف_file(&شاخص_قلف);
		اكسر;
	حالة COMMIT_PARTIAL:
		err = commit_قلف_file(&شاخص_قلف);
		rollback_قلف_file(&false_قلف);
		اكسر;
	}

	أرجع err;
}

/*
 * Take a union of paths in the index and the named tree (typically, "HEAD"),
 * and return the paths that match the given pattern in list.
 */
ساكن صحيح list_paths(بنية string_list *list, ثابت حرف *with_ونه,
		      ثابت بنية pathspec *نمونه)
{
	صحيح i, ret;
	حرف *m;

	إذا (!نمونه->nr)
		أرجع 0;

	m = xcalloc(1, نمونه->nr);

	إذا (with_ونه) {
		حرف *max_مختاړی = common_مختاړی(نمونه);
		overlay_ونه_on_شاخص(the_repository->شاخص, with_ونه, max_مختاړی);
		حرر(max_مختاړی);
	}

	/* TODO: audit for interaction with sparse-index. */
	ensure_ډک_شاخص(the_repository->شاخص);
	لكل (i = 0; i < the_repository->شاخص->cache_nr; i++) {
		ثابت بنية cache_ثبت *ce = the_repository->شاخص->cache[i];
		بنية string_list_توکی *توکی;

		إذا (ce->ce_flags & CE_UPDATE)
			استمر;
		إذا (!ce_لار_سمون(the_repository->شاخص, ce, نمونه, m))
			استمر;
		توکی = string_list_ورننباسه(list, ce->name);
		إذا (ce_skip_worktree(ce))
			توکی->util = توکی; /* better a valid pointer than a fake one */
	}

	ret = report_لار_error(m, نمونه);
	حرر(m);
	أرجع ret;
}

ساكن فراغ جمع_کړه_remove_files(بنية string_list *list)
{
	صحيح i;
	لكل (i = 0; i < list->nr; i++) {
		بنية stat st;
		بنية string_list_توکی *p = &(list->توکي[i]);

		/* p->util is skip-worktree */
		إذا (p->util)
			استمر;

		إذا (!lstat(p->string, &st)) {
			إذا (جمع_کړه_منزل_شاخص(the_repository->شاخص, p->string, &st, 0))
				die(_("updating files failed"));
		} وإلا
			remove_file_from_شاخص(the_repository->شاخص, p->string);
	}
}

ساكن فراغ create_بنسټ_شاخص(ثابت بنية commit *current_سر)
{
	بنية ونه *ونه;
	بنية unpack_trees_options opts;
	بنية ونه_desc t;

	إذا (!current_سر) {
		discard_شاخص(the_repository->شاخص);
		أرجع;
	}

	memset(&opts, 0, حجم(opts));
	opts.سر_idx = 1;
	opts.شاخص_only = 1;
	opts.یوځای = 1;
	opts.src_شاخص = the_repository->شاخص;
	opts.dst_شاخص = the_repository->شاخص;

	opts.fn = oneway_یوځای;
	ونه = repo_parse_ونه_غیرمستقیم(the_repository,
					&current_سر->object.oid);
	إذا (!ونه)
		die(_("failed to unpack HEAD tree object"));
	إذا (repo_parse_ونه(the_repository, ونه) < 0)
		وتل(128);
	init_ونه_desc(&t, &ونه->object.oid, ونه->بفر, ونه->اندازه);
	إذا (unpack_trees(1, &t, &opts))
		وتل(128); /* We've already reported the error, finish dying */
}

ساكن فراغ refresh_cache_or_die(صحيح refresh_flags)
{
	/*
	 * refresh_flags contains REFRESH_QUIET, so the only errors
	 * are for unmerged entries.
	 */
	إذا (refresh_شاخص(the_repository->شاخص, refresh_flags | REFRESH_دننه_PORCELAIN, عدم, عدم, عدم))
		die_resolve_conflict("commit");
}

ساكن ثابت حرف *prepare_شاخص(ثابت حرف **argv, ثابت حرف *مختاړی,
				 ثابت بنية commit *current_سر, صحيح is_حالت)
{
	بنية string_list partial = STRING_LIST_INIT_DUP;
	بنية pathspec pathspec;
	صحيح refresh_flags = REFRESH_QUIET;
	ثابت حرف *ret;
	حرف *لار = عدم;

	إذا (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	إذا (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	إذا (is_حالت)
		refresh_flags |= REFRESH_UNMERGED;
	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_ډک,
		       مختاړی, argv);

	إذا (pathspec_from_file) {
		إذا (interactive)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--interactive/--patch");

		إذا (all)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "-a");

		إذا (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_ډک,
				    مختاړی, pathspec_from_file, pathspec_file_nul);
	} وإلا إذا (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	إذا (!pathspec.nr && (also || (only && !allow_تشه &&
	    (!amend || (fixup_پیغام && strcmp(fixup_مختاړی, "amend"))))))
		die(_("No paths with --include/--only does not make sense."));

	إذا (repo_read_شاخص_preload(the_repository, &pathspec, 0) < 0)
		die(_("index file corrupt"));

	إذا (interactive) {
		حرف *old_شاخص_env = عدم, *old_repo_شاخص_file;
		repo_hold_locked_شاخص(the_repository, &شاخص_قلف,
				       قلف_DIE_ON_ERROR);

		refresh_cache_or_die(refresh_flags);

		إذا (write_locked_شاخص(the_repository->شاخص, &شاخص_قلف, 0))
			die(_("unable to create temporary index"));

		old_repo_شاخص_file = the_repository->شاخص_file;
		the_repository->شاخص_file =
			(حرف *)get_قلف_file_لار(&شاخص_قلف);
		old_شاخص_env = xstrdup_or_null(getenv(شاخص_ENVIRONMENT));
		setenv(شاخص_ENVIRONMENT, the_repository->شاخص_file, 1);

		إذا (interactive_جمع_کړه(the_repository, argv, مختاړی, patch_interactive, &interactive_opts) != 0)
			die(_("interactive add failed"));

		the_repository->شاخص_file = old_repo_شاخص_file;
		إذا (old_شاخص_env && *old_شاخص_env)
			setenv(شاخص_ENVIRONMENT, old_شاخص_env, 1);
		وإلا
			unsetenv(شاخص_ENVIRONMENT);
		FREE_AND_NULL(old_شاخص_env);

		discard_شاخص(the_repository->شاخص);
		read_شاخص_from(the_repository->شاخص, get_قلف_file_لار(&شاخص_قلف),
				repo_get_git_dir(the_repository));
		إذا (cache_ونه_update(the_repository->شاخص, WRITE_ونه_SILENT) == 0) {
			إذا (reopen_قلف_file(&شاخص_قلف) < 0)
				die(_("unable to write index file"));
			إذا (write_locked_شاخص(the_repository->شاخص, &شاخص_قلف, 0))
				die(_("unable to update temporary index"));
		} وإلا
			warning(_("Failed to update main cache tree"));

		commit_style = COMMIT_نورمال;
		ret = get_قلف_file_لار(&شاخص_قلف);
		اذهب بهر;
	} وإلا {
		إذا (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--interactive/--patch");
		إذا (interactive_opts.interhunkcontext != -1)
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
	إذا (all || (also && pathspec.nr)) {
		حرف *ps_matched = xcalloc(pathspec.nr, 1);
		repo_hold_locked_شاخص(the_repository, &شاخص_قلف,
				       قلف_DIE_ON_ERROR);
		جمع_کړه_files_منزل_cache(the_repository, also ? مختاړی : عدم,
				   &pathspec, ps_matched, 0, 0, 0 );
		إذا (!all && report_لار_error(ps_matched, &pathspec))
			وتل(128);

		refresh_cache_or_die(refresh_flags);
		cache_ونه_update(the_repository->شاخص, WRITE_ونه_SILENT);
		إذا (write_locked_شاخص(the_repository->شاخص, &شاخص_قلف, 0))
			die(_("unable to write new index file"));
		commit_style = COMMIT_نورمال;
		ret = get_قلف_file_لار(&شاخص_قلف);
		حرر(ps_matched);
		اذهب بهر;
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
	إذا (!only && !pathspec.nr) {
		repo_hold_locked_شاخص(the_repository, &شاخص_قلف,
				       قلف_DIE_ON_ERROR);
		refresh_cache_or_die(refresh_flags);
		إذا (the_repository->شاخص->cache_بدل_شوی
		    || !cache_ونه_fully_valid(the_repository->شاخص))
			cache_ونه_update(the_repository->شاخص, WRITE_ونه_SILENT);
		إذا (write_locked_شاخص(the_repository->شاخص, &شاخص_قلف,
				       COMMIT_قلف | SKIP_IF_UNCHANGED))
			die(_("unable to write new index file"));
		commit_style = COMMIT_AS_IS;
		ret = repo_get_شاخص_file(the_repository);
		اذهب بهر;
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

	إذا (whence != FROM_COMMIT) {
		إذا (whence == FROM_یوځای)
			die(_("cannot do a partial commit during a merge."));
		وإلا إذا (is_from_cherry_pick(whence))
			die(_("cannot do a partial commit during a cherry-pick."));
		وإلا إذا (is_from_rebase(whence))
			die(_("cannot do a partial commit during a rebase."));
	}

	إذا (list_paths(&partial, !current_سر ? عدم : "HEAD", &pathspec))
		وتل(1);

	discard_شاخص(the_repository->شاخص);
	إذا (repo_read_شاخص(the_repository) < 0)
		die(_("cannot read the index"));

	repo_hold_locked_شاخص(the_repository, &شاخص_قلف, قلف_DIE_ON_ERROR);
	جمع_کړه_remove_files(&partial);
	refresh_شاخص(the_repository->شاخص, REFRESH_QUIET, عدم, عدم, عدم);
	cache_ونه_update(the_repository->شاخص, WRITE_ونه_SILENT);
	إذا (write_locked_شاخص(the_repository->شاخص, &شاخص_قلف, 0))
		die(_("unable to write new index file"));

	لار = repo_git_لار(the_repository, "next-index-%"PRIuMAX,
			     (uintmax_t) getpid());
	hold_قلف_file_for_update(&false_قلف, لار,
				  قلف_DIE_ON_ERROR);

	create_بنسټ_شاخص(current_سر);
	جمع_کړه_remove_files(&partial);
	refresh_شاخص(the_repository->شاخص, REFRESH_QUIET, عدم, عدم, عدم);

	إذا (write_locked_شاخص(the_repository->شاخص, &false_قلف, 0))
		die(_("unable to write temporary index file"));

	discard_شاخص(the_repository->شاخص);
	ret = get_قلف_file_لار(&false_قلف);
	read_شاخص_from(the_repository->شاخص, ret, repo_get_git_dir(the_repository));
بهر:
	string_list_clear(&partial, 0);
	clear_pathspec(&pathspec);
	حرر(لار);
	أرجع ret;
}

ساكن صحيح پرلپسې_حالت(FILE *fp, ثابت حرف *شاخص_file, ثابت حرف *مختاړی, صحيح nowarn,
		      بنية wt_حالت *s)
{
	بنية object_id oid;

	إذا (s->relative_paths)
		s->مختاړی = مختاړی;

	إذا (amend) {
		s->amend = 1;
		s->reference = "HEAD^1";
	}
	s->verbose = verbose;
	s->شاخص_file = شاخص_file;
	s->fp = fp;
	s->nowarn = nowarn;
	s->is_initial = repo_get_oid(the_repository, s->reference, &oid) ? 1 : 0;
	إذا (!s->is_initial)
		oidcpy(&s->oid_commit, &oid);
	s->حالت_format = حالت_format;
	s->ignore_submodule_arg = ignore_submodule_arg;

	wt_حالت_collect(s);
	wt_حالت_print(s);
	wt_حالت_collect_free_buffers(s);

	أرجع s->committable;
}

ساكن صحيح is_a_یوځای(ثابت بنية commit *current_سر)
{
	أرجع !!(current_سر->parents && current_سر->parents->بل);
}

ساكن فراغ assert_وېشل_ident(بنية ident_وېشل *id, ثابت بنية strbuf *buf)
{
	إذا (وېشل_ident_کرښه(id, buf->buf, buf->len) || !id->date_begin)
		BUG("unable to parse our own ident: %s", buf->buf);
}

ساكن فراغ export_one(ثابت حرف *var, ثابت حرف *s, ثابت حرف *e, صحيح hack)
{
	بنية strbuf buf = STRBUF_INIT;
	إذا (hack)
		strbuf_addch(&buf, hack);
	strbuf_جمع_کړه(&buf, s, e - s);
	setenv(var, buf.buf, 1);
	strbuf_release(&buf);
}

ساكن صحيح parse_force_date(ثابت حرف *دننه, بنية strbuf *بهر)
{
	strbuf_addch(بهر, '@');

	إذا (parse_date(دننه, بهر) < 0) {
		صحيح errors = 0;
		بینښه اوږد t = approxidate_careful(دننه, &errors);
		إذا (errors)
			أرجع -1;
		strbuf_addf(بهر, "%lu", t);
	}

	أرجع 0;
}

ساكن فراغ سېټ_ident_var(حرف **buf, حرف *val)
{
	حرر(*buf);
	*buf = val;
}

ساكن فراغ determine_author_info(بنية strbuf *author_ident)
{
	حرف *name, *email, *date;
	بنية ident_وېشل author;

	name = xstrdup_or_null(getenv("GIT_AUTHOR_NAME"));
	email = xstrdup_or_null(getenv("GIT_AUTHOR_EMAIL"));
	date = xstrdup_or_null(getenv("GIT_AUTHOR_DATE"));

	إذا (author_پیغام) {
		بنية ident_وېشل ident;
		اندازه_t len;
		ثابت حرف *a;

		a = پیدا_کړه_commit_سرلیک(author_پیغام_بفر, "author", &len);
		إذا (!a)
			die(_("commit '%s' lacks author header"), author_پیغام);
		إذا (وېشل_ident_کرښه(&ident, a, len) < 0)
			die(_("commit '%s' has malformed author line"), author_پیغام);

		سېټ_ident_var(&name, xmemdupz(ident.name_begin, ident.name_پای_ټکی - ident.name_begin));
		سېټ_ident_var(&email, xmemdupz(ident.mail_begin, ident.mail_پای_ټکی - ident.mail_begin));

		إذا (ident.date_begin) {
			بنية strbuf date_buf = STRBUF_INIT;
			strbuf_addch(&date_buf, '@');
			strbuf_جمع_کړه(&date_buf, ident.date_begin, ident.date_پای_ټکی - ident.date_begin);
			strbuf_addch(&date_buf, ' ');
			strbuf_جمع_کړه(&date_buf, ident.tz_begin, ident.tz_پای_ټکی - ident.tz_begin);
			سېټ_ident_var(&date, strbuf_detach(&date_buf, عدم));
		}
	}

	إذا (force_author) {
		بنية ident_وېشل ident;

		إذا (وېشل_ident_کرښه(&ident, force_author, مزي_اوږدوالی(force_author)) < 0)
			die(_("malformed --author parameter"));
		سېټ_ident_var(&name, xmemdupz(ident.name_begin, ident.name_پای_ټکی - ident.name_begin));
		سېټ_ident_var(&email, xmemdupz(ident.mail_begin, ident.mail_پای_ټکی - ident.mail_begin));
	}

	إذا (force_date) {
		بنية strbuf date_buf = STRBUF_INIT;
		إذا (parse_force_date(force_date, &date_buf))
			die(_("invalid date format: %s"), force_date);
		سېټ_ident_var(&date, strbuf_detach(&date_buf, عدم));
	}

	strbuf_addstr(author_ident, fmt_ident(name, email, WANT_AUTHOR_IDENT, date,
				IDENT_STRICT));
	assert_وېشل_ident(&author, author_ident);
	export_one("GIT_AUTHOR_NAME", author.name_begin, author.name_پای_ټکی, 0);
	export_one("GIT_AUTHOR_EMAIL", author.mail_begin, author.mail_پای_ټکی, 0);
	export_one("GIT_AUTHOR_DATE", author.date_begin, author.tz_پای_ټکی, '@');
	حرر(name);
	حرر(email);
	حرر(date);
}

ساكن صحيح author_date_is_interesting(فراغ)
{
	أرجع author_پیغام || force_date;
}

#که_نه_تعریف_شوی WITH_BREAKING_CHANGES
ساكن فراغ adjust_comment_کرښه_char(ثابت بنية strbuf *sb)
{
	حرف candidates[] = "#;@!$%^&|:";
	حرف *candidate;
	ثابت حرف *p;
	اندازه_t cutoff;

	/* Ignore comment chars in trailing comments (e.g., Conflicts:) */
	cutoff = sb->len - ignored_log_پیغام_بایټونه(sb->buf, sb->len);

	إذا (!memchr(sb->buf, candidates[0], sb->len)) {
		حرر(comment_کرښه_str_منزل_free);
		comment_کرښه_str = comment_کرښه_str_منزل_free =
			xstrfmt("%c", candidates[0]);
		أرجع;
	}

	p = sb->buf;
	candidate = strchr(candidates, *p);
	إذا (candidate)
		*candidate = ' ';
	لكل (p = sb->buf; p + 1 < sb->buf + cutoff; p++) {
		إذا ((p[0] == '\n' || p[0] == '\r') && p[1]) {
			candidate = strchr(candidates, p[1]);
			إذا (candidate)
				*candidate = ' ';
		}
	}

	لكل (p = candidates; *p == ' '; p++)
		;
	إذا (!*p)
		die(_("unable to select a comment character that is not used\n"
		      "in the current commit message"));
	حرر(comment_کرښه_str_منزل_free);
	comment_کرښه_str = comment_کرښه_str_منزل_free = xstrfmt("%c", *p);
}
#د_که_پای /* !WITH_BREAKING_CHANGES */

ساكن فراغ prepare_amend_commit(بنية commit *commit, بنية strbuf *sb,
				بنية pretty_print_context *ctx)
{
	ثابت حرف *بفر, *subject, *fmt;

	بفر = repo_get_commit_بفر(the_repository, commit, عدم);
	پیدا_کړه_commit_subject(بفر, &subject);
	/*
	 * If we amend the 'amend!' commit then we don't want to
	 * duplicate the subject line.
	 */
	fmt = starts_with(subject, "amend!") ? "%b" : "%B";
	repo_format_commit_پیغام(the_repository, commit, fmt, sb, ctx);
	repo_unuse_commit_بفر(the_repository, commit, بفر);
}

ساكن فراغ change_معلومات_free(فراغ *util, ثابت حرف *str ناکارول_شوی)
{
	بنية wt_حالت_change_معلومات *d = util;
	حرر(d->rename_سرچینه);
	حرر(d);
}

ساكن صحيح prepare_منزل_commit(ثابت حرف *شاخص_file, ثابت حرف *مختاړی,
			     بنية commit *current_سر,
			     بنية wt_حالت *s,
			     بنية strbuf *author_ident)
{
	بنية stat statbuf;
	بنية strbuf committer_ident = STRBUF_INIT;
	صحيح committable;
	بنية strbuf sb = STRBUF_INIT;
	ثابت حرف *hook_arg1 = عدم;
	ثابت حرف *hook_arg2 = عدم;
	صحيح clean_پیغام_contents = (cleanup_ډېرپېښ != COMMIT_پیغام_CLEANUP_NONE);
	صحيح old_display_comment_مختاړی;
	صحيح invoked_hook;

	/* This checks and barfs if author is badly specified */
	determine_author_info(author_ident);

	إذا (!no_verify && پرلپسې_commit_hook(use_editor, شاخص_file, &invoked_hook,
					  "pre-commit", عدم))
		أرجع 0;

	إذا (squash_پیغام) {
		/*
		 * Insert the proper subject line before other commit
		 * message options add their content.
		 */
		إذا (use_پیغام && !strcmp(use_پیغام, squash_پیغام))
			strbuf_addstr(&sb, "squash! ");
		وإلا {
			بنية pretty_print_context ctx = {0};
			بنية commit *c;
			c = lookup_commit_reference_by_name(squash_پیغام);
			إذا (!c)
				die(_("could not lookup commit '%s'"), squash_پیغام);
			ctx.output_encoding = get_commit_output_encoding();
			repo_format_commit_پیغام(the_repository, c,
						   "squash! %s\n\n", &sb,
						   &ctx);
		}
	}

	إذا (have_option_m && !fixup_پیغام) {
		strbuf_addbuf(&sb, &پیغام_2);
		hook_arg1 = "message";
	} وإلا إذا (logfile && !strcmp(logfile, "-")) {
		إذا (isatty(0))
			fprintf(stderr, _("(reading log message from standard input)\n"));
		إذا (strbuf_read(&sb, 0, 0) < 0)
			die_errno(_("could not read log from standard input"));
		hook_arg1 = "message";
	} وإلا إذا (logfile) {
		إذا (strbuf_read_file(&sb, logfile, 0) < 0)
			die_errno(_("could not read log file '%s'"),
				  logfile);
		hook_arg1 = "message";
	} وإلا إذا (use_پیغام) {
		ثابت حرف *بفر;
		بفر = مزي_پلټنه(use_پیغام_بفر, "\n\n");
		إذا (بفر)
			strbuf_addstr(&sb, skip_blank_کرښې(بفر + 2));
		hook_arg1 = "commit";
		hook_arg2 = use_پیغام;
	} وإلا إذا (fixup_پیغام) {
		بنية pretty_print_context ctx = {0};
		بنية commit *commit;
		حرف *fmt;
		commit = lookup_commit_reference_by_name(fixup_commit);
		إذا (!commit)
			die(_("could not lookup commit '%s'"), fixup_commit);
		ctx.output_encoding = get_commit_output_encoding();
		fmt = xstrfmt("%s! %%s\n\n", fixup_مختاړی);
		repo_format_commit_پیغام(the_repository, commit, fmt, &sb,
					   &ctx);
		حرر(fmt);
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
		إذا (have_option_m && !strcmp(fixup_مختاړی, "fixup"))
			strbuf_addbuf(&sb, &پیغام_2);

		إذا (!strcmp(fixup_مختاړی, "amend")) {
			إذا (have_option_m)
				die(_("options '%s' and '%s:%s' cannot be used together"), "-m", "--fixup", fixup_پیغام);
			prepare_amend_commit(commit, &sb, &ctx);
		}
	} وإلا إذا (!stat(git_لار_یوځای_پیغام(the_repository), &statbuf)) {
		اندازه_t یوځای_پیغام_پیل;

		/*
		 * prepend SQUASH_MSG here if it exists and a
		 * "merge --squash" was originally performed
		 */
		إذا (!stat(git_لار_squash_پیغام(the_repository), &statbuf)) {
			إذا (strbuf_read_file(&sb, git_لار_squash_پیغام(the_repository), 0) < 0)
				die_errno(_("could not read SQUASH_MSG"));
			hook_arg1 = "squash";
		} وإلا
			hook_arg1 = "merge";

		یوځای_پیغام_پیل = sb.len;
		إذا (strbuf_read_file(&sb, git_لار_یوځای_پیغام(the_repository), 0) < 0)
			die_errno(_("could not read MERGE_MSG"));

		إذا (cleanup_ډېرپېښ == COMMIT_پیغام_CLEANUP_SCISSORS &&
		    wt_حالت_locate_پای_ټکی(sb.buf + یوځای_پیغام_پیل,
					 sb.len - یوځای_پیغام_پیل) <
				sb.len - یوځای_پیغام_پیل)
			s->added_cut_کرښه = 1;
	} وإلا إذا (!stat(git_لار_squash_پیغام(the_repository), &statbuf)) {
		إذا (strbuf_read_file(&sb, git_لار_squash_پیغام(the_repository), 0) < 0)
			die_errno(_("could not read SQUASH_MSG"));
		hook_arg1 = "squash";
	} وإلا إذا (template_file) {
		إذا (strbuf_read_file(&sb, template_file, 0) < 0)
			die_errno(_("could not read '%s'"), template_file);
		hook_arg1 = "template";
		clean_پیغام_contents = 0;
	}

	/*
	 * The remaining cases don't modify the template message, but
	 * just set the argument(s) to the prepare-commit-msg hook.
	 */
	وإلا إذا (whence == FROM_یوځای)
		hook_arg1 = "merge";
	وإلا إذا (is_from_cherry_pick(whence) || whence == FROM_REBASE_PICK) {
		hook_arg1 = "commit";
		hook_arg2 = "CHERRY_PICK_HEAD";
	}

	إذا (squash_پیغام) {
		/*
		 * If squash_commit was used for the commit subject,
		 * then we're possibly hijacking other commit log options.
		 * Reset the hook args to tell the real story.
		 */
		hook_arg1 = "message";
		hook_arg2 = "";
	}

	s->fp = fopen_for_writing(git_لار_commit_editmsg());
	إذا (!s->fp)
		die_errno(_("could not open '%s'"), git_لار_commit_editmsg());

	/* Ignore status.displayCommentPrefix: we do need comments in COMMIT_EDITMSG. */
	old_display_comment_مختاړی = s->display_comment_مختاړی;
	s->display_comment_مختاړی = 1;

	/*
	 * Most hints are counter-productive when the commit has
	 * already started.
	 */
	s->hints = 0;

	إذا (clean_پیغام_contents)
		strbuf_stripspace(&sb, عدم);

	إذا (signoff)
		append_signoff(&sb, ignored_log_پیغام_بایټونه(sb.buf, sb.len), 0);

	إذا (fwrite(sb.buf, 1, sb.len, s->fp) < sb.len)
		die_errno(_("could not write commit template"));

#که_نه_تعریف_شوی WITH_BREAKING_CHANGES
	إذا (auto_comment_کرښه_char)
		adjust_comment_کرښه_char(&sb);
#د_که_پای /* !WITH_BREAKING_CHANGES */
	strbuf_release(&sb);

	/* This checks if committer ident is explicitly given */
	strbuf_addstr(&committer_ident, git_committer_info(IDENT_STRICT));
	إذا (use_editor && include_حالت) {
		صحيح ident_shown = 0;
		تعداد git_colorbool saved_color_setting;
		بنية ident_وېشل ci, ai;
		ثابت حرف *hint_cleanup_all = allow_تشه_پیغام ?
			_("Please enter the commit message for your changes."
			  " Lines starting\nwith '%s' will be ignored.\n") :
			_("Please enter the commit message for your changes."
			  " Lines starting\nwith '%s' will be ignored, and an empty"
			  " message aborts the commit.\n");
		ثابت حرف *hint_cleanup_space = allow_تشه_پیغام ?
			_("Please enter the commit message for your changes."
			  " Lines starting\n"
			  "with '%s' will be kept; you may remove them"
			  " yourself if you want to.\n") :
			_("Please enter the commit message for your changes."
			  " Lines starting\n"
			  "with '%s' will be kept; you may remove them"
			  " yourself if you want to.\n"
			  "An empty message aborts the commit.\n");
		إذا (whence != FROM_COMMIT) {
			إذا (cleanup_ډېرپېښ == COMMIT_پیغام_CLEANUP_SCISSORS)
				wt_حالت_جمع_کړه_cut_کرښه(s);
			حالت_printf_ln(
				s, GIT_COLOR_نورمال,
				whence == FROM_یوځای ?
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
		إذا (cleanup_ډېرپېښ == COMMIT_پیغام_CLEANUP_ALL)
			حالت_printf(s, GIT_COLOR_نورمال, hint_cleanup_all, comment_کرښه_str);
		وإلا إذا (cleanup_ډېرپېښ == COMMIT_پیغام_CLEANUP_SCISSORS) {
			إذا (whence == FROM_COMMIT)
				wt_حالت_جمع_کړه_cut_کرښه(s);
		} وإلا /* COMMIT_MSG_CLEANUP_SPACE, that is. */
			حالت_printf(s, GIT_COLOR_نورمال, hint_cleanup_space, comment_کرښه_str);

		/*
		 * These should never fail because they come from our own
		 * fmt_ident. They may fail the sane_ident test, but we know
		 * that the name and mail pointers will at least be valid,
		 * which is enough for our tests and printing here.
		 */
		assert_وېشل_ident(&ai, author_ident);
		assert_وېشل_ident(&ci, &committer_ident);

		إذا (ident_cmp(&ai, &ci))
			حالت_printf_ln(s, GIT_COLOR_نورمال,
				_("%s"
				"Author:    %.*s <%.*s>"),
				ident_shown++ ? "" : "\n",
				(صحيح)(ai.name_پای_ټکی - ai.name_begin), ai.name_begin,
				(صحيح)(ai.mail_پای_ټکی - ai.mail_begin), ai.mail_begin);

		إذا (author_date_is_interesting())
			حالت_printf_ln(s, GIT_COLOR_نورمال,
				_("%s"
				"Date:      %s"),
				ident_shown++ ? "" : "\n",
				show_ident_date(&ai, DATE_ډېرپېښ(نورمال)));

		إذا (!committer_ident_sufficiently_given())
			حالت_printf_ln(s, GIT_COLOR_نورمال,
				_("%s"
				"Committer: %.*s <%.*s>"),
				ident_shown++ ? "" : "\n",
				(صحيح)(ci.name_پای_ټکی - ci.name_begin), ci.name_begin,
				(صحيح)(ci.mail_پای_ټکی - ci.mail_begin), ci.mail_begin);

		حالت_printf_ln(s, GIT_COLOR_نورمال, "%s", ""); /* Add new line for clarity */

		saved_color_setting = s->use_color;
		s->use_color = GIT_COLOR_NEVER;
		committable = پرلپسې_حالت(s->fp, شاخص_file, مختاړی, 1, s);
		s->use_color = saved_color_setting;
		string_list_clear_func(&s->change, change_معلومات_free);
	} وإلا {
		بنية object_id oid;
		ثابت حرف *پلار = "HEAD";

		إذا (!the_repository->شاخص->initialized && repo_read_شاخص(the_repository) < 0)
			die(_("Cannot read index"));

		إذا (amend)
			پلار = "HEAD^1";

		إذا (repo_get_oid(the_repository, پلار, &oid)) {
			صحيح i, ita_nr = 0;

			/* TODO: audit for interaction with sparse-index. */
			ensure_ډک_شاخص(the_repository->شاخص);
			لكل (i = 0; i < the_repository->شاخص->cache_nr; i++)
				إذا (ce_intent_منزل_جمع_کړه(the_repository->شاخص->cache[i]))
					ita_nr++;
			committable = the_repository->شاخص->cache_nr > ita_nr;
		} وإلا {
			/*
			 * Unless the user did explicitly request a submodule
			 * ignore mode by passing a command line option we do
			 * not ignore any changed submodule SHA-1s when
			 * comparing index and parent, no matter what is
			 * configured. Otherwise we won't commit any
			 * submodules which were manually staged, which would
			 * be really confusing.
			 */
			بنية diff_flags flags = DIFF_FLAGS_INIT;
			flags.override_submodule_config = 1;
			إذا (ignore_submodule_arg &&
			    !strcmp(ignore_submodule_arg, "all"))
				flags.ignore_submodules = 1;
			committable = شاخص_differs_from(the_repository,
							 پلار, &flags, 1);
		}
	}
	strbuf_release(&committer_ident);

	fclose(s->fp);

	إذا (trailer_دلیلونه.nr) {
		إذا (amend_file_with_trailers(git_لار_commit_editmsg(), &trailer_دلیلونه))
			die(_("unable to pass trailers to --trailers"));
		strvec_clear(&trailer_دلیلونه);
	}

	/*
	 * Reject an attempt to record a non-merge empty commit without
	 * explicit --allow-empty. In the cherry-pick case, it may be
	 * empty due to conflict resolution, which the user should okay.
	 */
	إذا (!committable && whence != FROM_یوځای && !allow_تشه &&
	    !(amend && is_a_یوځای(current_سر))) {
		s->hints = advice_enabled(ADVICE_حالت_HINTS);
		s->display_comment_مختاړی = old_display_comment_مختاړی;
		پرلپسې_حالت(stdout, شاخص_file, مختاړی, 0, s);
		إذا (amend)
			fputs(_(تشه_amend_advice), stderr);
		وإلا إذا (is_from_cherry_pick(whence) ||
			 whence == FROM_REBASE_PICK) {
			fputs(_(تشه_cherry_pick_advice), stderr);
			إذا (whence == FROM_CHERRY_PICK_SINGLE)
				fputs(_(تشه_cherry_pick_advice_single), stderr);
			وإلا إذا (whence == FROM_CHERRY_PICK_MULTI)
				fputs(_(تشه_cherry_pick_advice_multi), stderr);
			وإلا
				fputs(_(تشه_rebase_pick_advice), stderr);
		}
		أرجع 0;
	}

	إذا (!no_verify && invoked_hook) {
		/*
		 * Re-read the index as the pre-commit-commit hook was invoked
		 * and could have updated it. We must do this before we invoke
		 * the editor and after we invoke run_status above.
		 */
		discard_شاخص(the_repository->شاخص);
	}
	read_شاخص_from(the_repository->شاخص, شاخص_file, repo_get_git_dir(the_repository));

	إذا (cache_ونه_update(the_repository->شاخص, 0)) {
		error(_("Error building trees"));
		أرجع 0;
	}

	إذا (پرلپسې_commit_hook(use_editor, شاخص_file, عدم, "prepare-commit-msg",
			    git_لار_commit_editmsg(), hook_arg1, hook_arg2, عدم))
		أرجع 0;

	إذا (use_editor) {
		بنية strvec env = STRVEC_INIT;

		strvec_pushf(&env, "GIT_INDEX_FILE=%s", شاخص_file);
		إذا (launch_editor(git_لار_commit_editmsg(), عدم, env.v)) {
			fprintf(stderr,
			_("Please supply the message using either -m or -F option.\n"));
			وتل(1);
		}
		strvec_clear(&env);
	}

	إذا (!no_verify &&
	    پرلپسې_commit_hook(use_editor, شاخص_file, عدم, "commit-msg",
			    git_لار_commit_editmsg(), عدم)) {
		أرجع 0;
	}

	أرجع 1;
}

ساكن ثابت حرف *پیدا_کړه_author_by_nickname(ثابت حرف *name)
{
	بنية rev_info revs;
	بنية commit *commit;
	بنية strbuf buf = STRBUF_INIT;
	ثابت حرف *av[20];
	صحيح ac = 0;

	repo_init_revisions(the_repository, &revs, عدم);
	strbuf_addf(&buf, "--author=%s", name);
	av[++ac] = "--all";
	av[++ac] = "-i";
	av[++ac] = buf.buf;
	av[++ac] = عدم;
	تیاری_revisions(ac, av, &revs, عدم);
	revs.mailmap = xmalloc(حجم(بنية string_list));
	string_list_init_nodup(revs.mailmap);
	read_mailmap(the_repository, revs.mailmap);

	إذا (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	commit = get_revision(&revs);
	إذا (commit) {
		بنية pretty_print_context ctx = {0};
		ctx.date_ډېرپېښ.type = DATE_نورمال;
		strbuf_release(&buf);
		repo_format_commit_پیغام(the_repository, commit,
					   "%aN <%aE>", &buf, &ctx);
		release_revisions(&revs);
		أرجع strbuf_detach(&buf, عدم);
	}
	die(_("--author '%s' is not 'Name <email>' and matches no existing author"), name);
}

ساكن فراغ handle_ignored_arg(بنية wt_حالت *s)
{
	إذا (!ignored_arg)
		; /* default already initialized */
	وإلا إذا (!strcmp(ignored_arg, "traditional"))
		s->show_ignored_ډېرپېښ = SHOW_TRADITIONAL_IGNORED;
	وإلا إذا (!strcmp(ignored_arg, "no"))
		s->show_ignored_ډېرپېښ = SHOW_NO_IGNORED;
	وإلا إذا (!strcmp(ignored_arg, "matching"))
		s->show_ignored_ډېرپېښ = SHOW_MATCHING_IGNORED;
	وإلا
		die(_("Invalid ignored mode '%s'"), ignored_arg);
}

ساكن تعداد untracked_حالت_type parse_untracked_setting_name(ثابت حرف *u)
{
	/*
	 * Please update $__git_untracked_file_modes in
	 * git-completion.bash when you add new options
	 */
	بدل (git_parse_maybe_bool(u)) {
	حالة 0:
		u = "no";
		اكسر;
	حالة 1:
		u = "normal";
		اكسر;
	افتراضي:
		اكسر;
	}

	إذا (!strcmp(u, "no"))
		أرجع SHOW_NO_UNTRACKED_FILES;
	وإلا إذا (!strcmp(u, "normal"))
		أرجع SHOW_نورمال_UNTRACKED_FILES;
	وإلا إذا (!strcmp(u, "all"))
		أرجع SHOW_ALL_UNTRACKED_FILES;
	وإلا
		أرجع SHOW_UNTRACKED_FILES_ERROR;
}

ساكن فراغ handle_untracked_files_arg(بنية wt_حالت *s)
{
	تعداد untracked_حالت_type u;

	إذا (!untracked_files_arg)
		أرجع; /* default already initialized */

	u = parse_untracked_setting_name(untracked_files_arg);
	إذا (u == SHOW_UNTRACKED_FILES_ERROR)
		die(_("Invalid untracked files mode '%s'"),
		    untracked_files_arg);
	s->show_untracked_files = u;
}

ساكن ثابت حرف *read_commit_پیغام_2(ثابت حرف *name)
{
	ثابت حرف *بهر_enc;
	بنية commit *commit;

	commit = lookup_commit_reference_by_name(name);
	إذا (!commit)
		die(_("could not lookup commit '%s'"), name);
	بهر_enc = get_commit_output_encoding();
	أرجع repo_logmsg_reencode(the_repository, commit, عدم, بهر_enc);
}

/*
 * Enumerate what needs to be propagated when --porcelain
 * is not in effect here.
 */
ساكن بنية حالت_deferred_config {
	تعداد wt_حالت_format حالت_format;
	صحيح show_branch;
	تعداد ahead_behind_flags ahead_behind;
} حالت_deferred_config = {
	حالت_FORMAT_UNSPECIFIED,
	-1, /* unspecified */
	AHEAD_BEHIND_UNSPECIFIED,
};

ساكن فراغ finalize_deferred_config(بنية wt_حالت *s)
{
	صحيح use_deferred_config = (حالت_format != حالت_FORMAT_PORCELAIN &&
				   حالت_format != حالت_FORMAT_PORCELAIN_V2 &&
				   !s->null_termination);

	إذا (s->null_termination) {
		إذا (حالت_format == حالت_FORMAT_NONE ||
		    حالت_format == حالت_FORMAT_UNSPECIFIED)
			حالت_format = حالت_FORMAT_PORCELAIN;
		وإلا إذا (حالت_format == حالت_FORMAT_LONG)
			die(_("options '%s' and '%s' cannot be used together"), "--long", "-z");
	}

	إذا (use_deferred_config && حالت_format == حالت_FORMAT_UNSPECIFIED)
		حالت_format = حالت_deferred_config.حالت_format;
	إذا (حالت_format == حالت_FORMAT_UNSPECIFIED)
		حالت_format = حالت_FORMAT_NONE;

	إذا (use_deferred_config && s->show_branch < 0)
		s->show_branch = حالت_deferred_config.show_branch;
	إذا (s->show_branch < 0)
		s->show_branch = 0;

	/*
	 * If the user did not give a "--[no]-ahead-behind" command
	 * line argument *AND* we will print in a human-readable format
	 * (short, long etc.) then we inherit from the status.aheadbehind
	 * config setting.  In all other cases (and porcelain V[12] formats
	 * in particular), we inherit _FULL for backwards compatibility.
	 */
	إذا (use_deferred_config &&
	    s->ahead_behind_flags == AHEAD_BEHIND_UNSPECIFIED)
		s->ahead_behind_flags = حالت_deferred_config.ahead_behind;

	إذا (s->ahead_behind_flags == AHEAD_BEHIND_UNSPECIFIED)
		s->ahead_behind_flags = AHEAD_BEHIND_ډک;
}

ساكن فراغ check_fixup_reword_options(صحيح argc, ثابت حرف *argv[]) {
	إذا (whence != FROM_COMMIT) {
		إذا (whence == FROM_یوځای)
			die(_("You are in the middle of a merge -- cannot reword."));
		وإلا إذا (is_from_cherry_pick(whence))
			die(_("You are in the middle of a cherry-pick -- cannot reword."));
	}
	إذا (argc)
		die(_("reword option of '%s' and path '%s' cannot be used together"), "--fixup", *argv);
	إذا (patch_interactive || interactive || all || also || only)
		die(_("reword option of '%s' and '%s' cannot be used together"),
			"--fixup", "--patch/--interactive/--all/--include/--only");
}

ساكن صحيح parse_and_validate_options(صحيح argc, ثابت حرف *argv[],
				      ثابت بنية option *options,
				      ثابت حرف * ثابت usage[],
				      ثابت حرف *مختاړی,
				      بنية commit *current_سر,
				      بنية wt_حالت *s)
{
	argc = parse_options(argc, argv, مختاړی, options, usage, 0);
	finalize_deferred_config(s);

	إذا (force_author && !strchr(force_author, '>'))
		force_author = پیدا_کړه_author_by_nickname(force_author);

	إذا (force_author && renew_authorship)
		die(_("options '%s' and '%s' cannot be used together"), "--reset-author", "--author");

	إذا (logfile || have_option_m || use_پیغام)
		use_editor = 0;

	/* Sanity check options */
	إذا (amend && !current_سر)
		die(_("You have nothing to amend."));
	إذا (amend && whence != FROM_COMMIT) {
		إذا (whence == FROM_یوځای)
			die(_("You are in the middle of a merge -- cannot amend."));
		وإلا إذا (is_from_cherry_pick(whence))
			die(_("You are in the middle of a cherry-pick -- cannot amend."));
		وإلا إذا (whence == FROM_REBASE_PICK)
			die(_("You are in the middle of a rebase -- cannot amend."));
	}
	إذا (fixup_پیغام && squash_پیغام)
		die(_("options '%s' and '%s' cannot be used together"), "--squash", "--fixup");
	die_for_incompatible_opt4(!!use_پیغام, "-C",
				  !!edit_پیغام, "-c",
				  !!logfile, "-F",
				  !!fixup_پیغام, "--fixup");
	die_for_incompatible_opt4(have_option_m, "-m",
				  !!edit_پیغام, "-c",
				  !!use_پیغام, "-C",
				  !!logfile, "-F");
	إذا (use_پیغام || edit_پیغام || logfile ||fixup_پیغام || have_option_m)
		FREE_AND_NULL(template_file);
	إذا (edit_پیغام)
		use_پیغام = edit_پیغام;
	إذا (amend && !use_پیغام && !fixup_پیغام)
		use_پیغام = "HEAD";
	إذا (!use_پیغام && !is_from_cherry_pick(whence) &&
	    !is_from_rebase(whence) && renew_authorship)
		die(_("--reset-author can be used only with -C, -c or --amend."));
	إذا (use_پیغام) {
		use_پیغام_بفر = read_commit_پیغام_2(use_پیغام);
		إذا (!renew_authorship) {
			author_پیغام = use_پیغام;
			author_پیغام_بفر = use_پیغام_بفر;
		}
	}
	إذا ((is_from_cherry_pick(whence) || whence == FROM_REBASE_PICK) &&
	    !renew_authorship) {
		author_پیغام = "CHERRY_PICK_HEAD";
		author_پیغام_بفر = read_commit_پیغام_2(author_پیغام);
	}

	إذا (patch_interactive)
		interactive = 1;

	die_for_incompatible_opt4(also, "-i/--include",
				  only, "-o/--only",
				  all, "-a/--all",
				  interactive, "--interactive/-p/--patch");
	إذا (fixup_پیغام) {
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
		طالما (که_توری(*p))
			p++;
		إذا (p > fixup_پیغام && *p == ':') {
			*p = '\0';
			fixup_commit = p + 1;
			إذا (!strcmp("amend", fixup_پیغام) ||
			    !strcmp("reword", fixup_پیغام)) {
				fixup_مختاړی = "amend";
				allow_تشه = 1;
				إذا (*fixup_پیغام == 'r') {
					check_fixup_reword_options(argc, argv);
					only = 1;
				}
			} وإلا {
				die(_("unknown option: --fixup=%s:%s"), fixup_پیغام, fixup_commit);
			}
		} وإلا {
			fixup_commit = fixup_پیغام;
			fixup_مختاړی = "fixup";
			use_editor = 0;
		}
	}

	إذا (0 <= edit_flag)
		use_editor = edit_flag;

	handle_untracked_files_arg(s);

	إذا (all && argc > 0)
		die(_("paths '%s ...' with -a does not make sense"),
		    argv[0]);

	إذا (حالت_format != حالت_FORMAT_NONE)
		dry_پرلپسې = 1;

	أرجع argc;
}

ساكن صحيح dry_پرلپسې_commit(ثابت حرف **argv, ثابت حرف *مختاړی,
			  ثابت بنية commit *current_سر, بنية wt_حالت *s)
{
	صحيح committable;
	ثابت حرف *شاخص_file;

	شاخص_file = prepare_شاخص(argv, مختاړی, current_سر, 1);
	committable = پرلپسې_حالت(stdout, شاخص_file, مختاړی, 0, s);
	rollback_شاخص_files();

	أرجع committable ? 0 : 1;
}

define_list_config_array_extra(color_حالت_slots, {"added"});

ساكن صحيح parse_حالت_خونه(ثابت حرف *خونه)
{
	إذا (!strcasecmp(خونه, "added"))
		أرجع WT_حالت_UPDATED;

	أرجع LOOKUP_CONFIG(color_حالت_slots, خونه);
}

ساكن صحيح git_حالت_config(ثابت حرف *k, ثابت حرف *v,
			     ثابت بنية config_context *ctx, فراغ *cb)
{
	بنية wt_حالت *s = cb;
	ثابت حرف *خونه_name;

	إذا (starts_with(k, "column."))
		أرجع git_ستنه_config(k, v, "status", &s->colopts);
	إذا (!strcmp(k, "status.submodulesummary")) {
		صحيح is_bool;
		s->submodule_summary = git_config_bool_or_int(k, v, ctx->kvi,
							      &is_bool);
		إذا (is_bool && s->submodule_summary)
			s->submodule_summary = -1;
		أرجع 0;
	}
	إذا (!strcmp(k, "status.short")) {
		إذا (git_config_bool(k, v))
			حالت_deferred_config.حالت_format = حالت_FORMAT_SHORT;
		وإلا
			حالت_deferred_config.حالت_format = حالت_FORMAT_NONE;
		أرجع 0;
	}
	إذا (!strcmp(k, "status.branch")) {
		حالت_deferred_config.show_branch = git_config_bool(k, v);
		أرجع 0;
	}
	إذا (!strcmp(k, "status.aheadbehind")) {
		حالت_deferred_config.ahead_behind = git_config_bool(k, v);
		أرجع 0;
	}
	إذا (!strcmp(k, "status.showstash")) {
		s->show_stash = git_config_bool(k, v);
		أرجع 0;
	}
	إذا (!strcmp(k, "status.color") || !strcmp(k, "color.status")) {
		s->use_color = git_config_colorbool(k, v);
		أرجع 0;
	}
	إذا (!strcmp(k, "status.displaycommentprefix")) {
		s->display_comment_مختاړی = git_config_bool(k, v);
		أرجع 0;
	}
	إذا (skip_مختاړی(k, "status.color.", &خونه_name) ||
	    skip_مختاړی(k, "color.status.", &خونه_name)) {
		صحيح خونه = parse_حالت_خونه(خونه_name);
		إذا (خونه < 0)
			أرجع 0;
		إذا (!v)
			أرجع config_error_nonbool(k);
		أرجع color_parse(v, s->color_palette[خونه]);
	}
	إذا (!strcmp(k, "status.relativepaths")) {
		s->relative_paths = git_config_bool(k, v);
		أرجع 0;
	}
	إذا (!strcmp(k, "status.showuntrackedfiles")) {
		تعداد untracked_حالت_type u;

		u = parse_untracked_setting_name(v);
		إذا (u == SHOW_UNTRACKED_FILES_ERROR)
			أرجع error(_("Invalid untracked files mode '%s'"), v);
		s->show_untracked_files = u;
		أرجع 0;
	}
	إذا (!strcmp(k, "diff.renamelimit")) {
		إذا (s->rename_حد == -1)
			s->rename_حد = git_config_int(k, v, ctx->kvi);
		أرجع 0;
	}
	إذا (!strcmp(k, "status.renamelimit")) {
		s->rename_حد = git_config_int(k, v, ctx->kvi);
		أرجع 0;
	}
	إذا (!strcmp(k, "diff.renames")) {
		إذا (s->detect_rename == -1)
			s->detect_rename = git_config_rename(k, v);
		أرجع 0;
	}
	إذا (!strcmp(k, "status.renames")) {
		s->detect_rename = git_config_rename(k, v);
		أرجع 0;
	}
	أرجع git_diff_ui_config(k, v, ctx, عدم);
}

صحيح cmd_حالت(صحيح argc,
ثابت حرف **argv,
ثابت حرف *مختاړی,
بنية repository *repo ناکارول_شوی)
{
	ساكن صحيح no_renames = -1;
	ساكن ثابت حرف *rename_score_arg = (ثابت حرف *)-1;
	ساكن بنية wt_حالت s;
	بینښه صحيح پرمختګ_flag = 0;
	صحيح fd;
	بنية object_id oid;
	ساكن بنية option builtin_حالت_options[] = {
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT_سېټ_INT('s', "short", &حالت_format,
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
		OPT_سېټ_INT(0, "long", &حالت_format,
			    N_("show status in long format (default)"),
			    حالت_FORMAT_LONG),
		OPT_BOOL('z', "null", &s.null_termination,
			 N_("terminate entries with NUL")),
		{
			.type = OPTION_STRING,
			.short_name = 'u',
			.long_name = "untracked-files",
			.ارزښت = &untracked_files_arg,
			.argh = N_("mode"),
			.help = N_("show untracked files, optional modes: all, normal, no. (Default: all)"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t)"all",
		},
		{
			.type = OPTION_STRING,
			.long_name = "ignored",
			.ارزښت = &ignored_arg,
			.argh = N_("mode"),
			.help = N_("show ignored files, optional modes: traditional, matching, no. (Default: traditional)"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t)"traditional",
		},
		{
			.type = OPTION_STRING,
			.long_name = "ignore-submodules",
			.ارزښت = &ignore_submodule_arg,
			.argh = N_("when"),
			.help = N_("ignore changes to submodules, optional when: all, dirty, untracked. (Default: all)"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t)"all",
		},
		OPT_ستنه(0, "column", &s.colopts, N_("list untracked files in columns")),
		OPT_BOOL(0, "no-renames", &no_renames, N_("do not detect renames")),
		OPT_CALLBACK_F('M', "find-renames", &rename_score_arg,
		  N_("n"), N_("detect renames, optionally set similarity index"),
		  PARSE_OPT_OPTARG | PARSE_OPT_NONEG, opt_parse_rename_score),
		OPT_پای_ټکی(),
	};

	show_usage_with_options_if_asked(argc, argv,
					 builtin_حالت_usage, builtin_حالت_options);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_ډک_شاخص = 0;

	حالت_init_config(&s, git_حالت_config);
	argc = parse_options(argc, argv, مختاړی,
			     builtin_حالت_options,
			     builtin_حالت_usage, 0);
	finalize_colopts(&s.colopts, -1);
	finalize_deferred_config(&s);

	handle_untracked_files_arg(&s);
	handle_ignored_arg(&s);

	إذا (s.show_ignored_ډېرپېښ == SHOW_MATCHING_IGNORED &&
	    s.show_untracked_files == SHOW_NO_UNTRACKED_FILES)
		die(_("Unsupported combination of ignored and untracked-files arguments"));

	parse_pathspec(&s.pathspec, 0,
		       PATHSPEC_PREFER_ډک,
		       مختاړی, argv);

	إذا (حالت_format != حالت_FORMAT_PORCELAIN &&
	    حالت_format != حالت_FORMAT_PORCELAIN_V2)
		پرمختګ_flag = REFRESH_پرمختګ;
	repo_read_شاخص(the_repository);
	refresh_شاخص(the_repository->شاخص,
		      REFRESH_QUIET|REFRESH_UNMERGED|پرمختګ_flag,
		      &s.pathspec, عدم, عدم);

	إذا (use_optional_locks())
		fd = repo_hold_locked_شاخص(the_repository, &شاخص_قلف, 0);
	وإلا
		fd = -1;

	s.is_initial = repo_get_oid(the_repository, s.reference, &oid) ? 1 : 0;
	إذا (!s.is_initial)
		oidcpy(&s.oid_commit, &oid);

	s.ignore_submodule_arg = ignore_submodule_arg;
	s.حالت_format = حالت_format;
	s.verbose = verbose;
	إذا (no_renames != -1)
		s.detect_rename = !no_renames;
	إذا ((intptr_t)rename_score_arg != -1) {
		إذا (s.detect_rename < DIFF_DETECT_RENAME)
			s.detect_rename = DIFF_DETECT_RENAME;
		إذا (rename_score_arg)
			s.rename_score = parse_rename_score(&rename_score_arg);
	}

	wt_حالت_collect(&s);

	إذا (0 <= fd)
		repo_update_شاخص_if_able(the_repository, &شاخص_قلف);

	إذا (s.relative_paths)
		s.مختاړی = مختاړی;

	wt_حالت_print(&s);
	wt_حالت_collect_free_buffers(&s);

	أرجع 0;
}

ساكن صحيح git_commit_config(ثابت حرف *k, ثابت حرف *v,
			     ثابت بنية config_context *ctx, فراغ *cb)
{
	بنية wt_حالت *s = cb;

	إذا (!strcmp(k, "commit.template"))
		أرجع git_config_pathname(&template_file, k, v);
	إذا (!strcmp(k, "commit.status")) {
		include_حالت = git_config_bool(k, v);
		أرجع 0;
	}
	إذا (!strcmp(k, "commit.cleanup")) {
		FREE_AND_NULL(cleanup_config);
		أرجع git_config_string(&cleanup_config, k, v);
	}
	إذا (!strcmp(k, "commit.gpgsign")) {
		sign_commit = git_config_bool(k, v) ? "" : عدم;
		أرجع 0;
	}
	إذا (!strcmp(k, "commit.verbose")) {
		صحيح is_bool;
		config_commit_verbose = git_config_bool_or_int(k, v, ctx->kvi,
							       &is_bool);
		أرجع 0;
	}

	أرجع git_حالت_config(k, v, ctx, s);
}

صحيح cmd_commit(صحيح argc,
	       ثابت حرف **argv,
	       ثابت حرف *مختاړی,
	       بنية repository *repo ناکارول_شوی)
{
	ساكن بنية wt_حالت s;
	ساكن ثابت حرف *cleanup_arg = عدم;
	ساكن بنية option builtin_commit_options[] = {
		OPT__QUIET(&quiet, N_("suppress summary after successful commit")),
		OPT__VERBOSE(&verbose, N_("show diff in commit message template")),

		OPT_ډله(N_("Commit message options")),
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
		OPT_STRVEC(0, "trailer", &trailer_دلیلونه, N_("trailer"),
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
			.ارزښت = &sign_commit,
			.argh = N_("key-id"),
			.help = N_("GPG sign commit"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "",
		},
		/* end commit message options */

		OPT_ډله(N_("Commit contents options")),
		OPT_BOOL('a', "all", &all, N_("commit all changed files")),
		OPT_BOOL('i', "include", &also, N_("add specified files to index for commit")),
		OPT_BOOL(0, "interactive", &interactive, N_("interactively add files")),
		OPT_BOOL('p', "patch", &patch_interactive, N_("interactively add changes")),
		OPT_DIFF_UNIFIED(&interactive_opts.context),
		OPT_DIFF_INTERHUNK_CONTEXT(&interactive_opts.interhunkcontext),
		OPT_BOOL('o', "only", &only, N_("commit only specified files")),
		OPT_BOOL('n', "no-verify", &no_verify, N_("bypass pre-commit and commit-msg hooks")),
		OPT_BOOL(0, "dry-run", &dry_پرلپسې, N_("show what would be committed")),
		OPT_سېټ_INT(0, "short", &حالت_format, N_("show status concisely"),
			    حالت_FORMAT_SHORT),
		OPT_BOOL(0, "branch", &s.show_branch, N_("show branch information")),
		OPT_BOOL(0, "ahead-behind", &s.ahead_behind_flags,
			 N_("compute full ahead/behind values")),
		OPT_سېټ_INT(0, "porcelain", &حالت_format,
			    N_("machine-readable output"), حالت_FORMAT_PORCELAIN),
		OPT_سېټ_INT(0, "long", &حالت_format,
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
			.ارزښت = &untracked_files_arg,
			.argh = N_("mode"),
			.help = N_("show untracked files, optional modes: all, normal, no. (Default: all)"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t)"all",
		},
		OPT_PATHSPEC_FROM_FILE(&pathspec_from_file),
		OPT_PATHSPEC_FILE_NUL(&pathspec_file_nul),
		/* end commit contents options */

		OPT_HIDDEN_BOOL(0, "allow-empty", &allow_تشه,
				N_("ok to record an empty change")),
		OPT_HIDDEN_BOOL(0, "allow-empty-message", &allow_تشه_پیغام,
				N_("ok to record a change with an empty message")),

		OPT_پای_ټکی()
	};

	بنية strbuf sb = STRBUF_INIT;
	بنية strbuf author_ident = STRBUF_INIT;
	ثابت حرف *شاخص_file, *reflog_پیغام;
	بنية object_id oid;
	بنية commit_list *parents = عدم;
	بنية stat statbuf;
	بنية commit *current_سر = عدم;
	بنية commit_extra_سرلیک *extra = عدم;
	بنية strbuf err = STRBUF_INIT;
	صحيح ret = 0;

	show_usage_with_options_if_asked(argc, argv,
					 builtin_commit_usage, builtin_commit_options);

#که_نه_تعریف_شوی WITH_BREAKING_CHANGES
	warn_on_auto_comment_char = true;
#د_که_پای /* !WITH_BREAKING_CHANGES */
	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_ډک_شاخص = 0;

	حالت_init_config(&s, git_commit_config);
	s.commit_template = 1;
	حالت_format = حالت_FORMAT_NONE; /* Ignore status.short */
	s.colopts = 0;

	إذا (repo_get_oid(the_repository, "HEAD", &oid))
		current_سر = عدم;
	وإلا {
		current_سر = lookup_commit_or_die(&oid, "HEAD");
		إذا (repo_parse_commit(the_repository, current_سر))
			die(_("could not parse HEAD commit"));
	}
	verbose = -1; /* unspecified */
	argc = parse_and_validate_options(argc, argv, builtin_commit_options,
					  builtin_commit_usage,
					  مختاړی, current_سر, &s);
	إذا (trailer_دلیلونه.nr)
		trailer_config_init();

	إذا (verbose == -1)
		verbose = (config_commit_verbose < 0) ? 0 : config_commit_verbose;

	إذا (cleanup_arg) {
		حرر(cleanup_config);
		cleanup_config = xstrdup(cleanup_arg);
	}
	cleanup_ډېرپېښ = get_cleanup_ډېرپېښ(cleanup_config, use_editor);

	إذا (dry_پرلپسې)
		أرجع dry_پرلپسې_commit(argv, مختاړی, current_سر, &s);
	شاخص_file = prepare_شاخص(argv, مختاړی, current_سر, 0);

	/* Set up everything for writing the commit object.  This includes
	   running hooks, writing the trees, and interacting with the user.  */
	إذا (!prepare_منزل_commit(شاخص_file, مختاړی,
			       current_سر, &s, &author_ident)) {
		ret = 1;
		rollback_شاخص_files();
		اذهب cleanup;
	}

	/* Determine parents */
	reflog_پیغام = getenv("GIT_REFLOG_ACTION");
	إذا (!current_سر) {
		إذا (!reflog_پیغام)
			reflog_پیغام = "commit (initial)";
	} وإلا إذا (amend) {
		إذا (!reflog_پیغام)
			reflog_پیغام = "commit (amend)";
		parents = commit_list_copy(current_سر->parents);
	} وإلا إذا (whence == FROM_یوځای) {
		بنية strbuf m = STRBUF_INIT;
		FILE *fp;
		صحيح allow_fast_forward = 1;
		بنية commit_list **pptr = &parents;

		إذا (!reflog_پیغام)
			reflog_پیغام = "commit (merge)";
		pptr = commit_list_append(current_سر, pptr);
		fp = xfopen(git_لار_یوځای_سر(the_repository), "r");
		طالما (strbuf_getline_lf(&m, fp) != EOF) {
			بنية commit *پلار;

			پلار = get_یوځای_پلار(m.buf);
			إذا (!پلار)
				die(_("Corrupt MERGE_HEAD file (%s)"), m.buf);
			pptr = commit_list_append(پلار, pptr);
		}
		fclose(fp);
		strbuf_release(&m);
		إذا (!stat(git_لار_یوځای_ډېرپېښ(the_repository), &statbuf)) {
			إذا (strbuf_read_file(&sb, git_لار_یوځای_ډېرپېښ(the_repository), 0) < 0)
				die_errno(_("could not read MERGE_MODE"));
			إذا (!strcmp(sb.buf, "no-ff"))
				allow_fast_forward = 0;
		}
		إذا (allow_fast_forward)
			reduce_heads_replace(&parents);
	} وإلا {
		إذا (!reflog_پیغام)
			reflog_پیغام = is_from_cherry_pick(whence)
					? "commit (cherry-pick)"
					: is_from_rebase(whence)
					? "commit (rebase)"
					: "commit";
		commit_list_ورننباسه(current_سر, &parents);
	}

	/* Finally, get the commit message */
	strbuf_reset(&sb);
	إذا (strbuf_read_file(&sb, git_لار_commit_editmsg(), 0) < 0) {
		صحيح saved_errno = errno;
		rollback_شاخص_files();
		die(_("could not read commit message: %s"), strerror(saved_errno));
	}

	cleanup_پیغام(&sb, cleanup_ډېرپېښ, verbose);

	إذا (پیغام_is_تشه(&sb, cleanup_ډېرپېښ) && !allow_تشه_پیغام) {
		rollback_شاخص_files();
		fprintf(stderr, _("Aborting commit due to empty commit message.\n"));
		وتل(1);
	}
	إذا (template_untouched(&sb, template_file, cleanup_ډېرپېښ) && !allow_تشه_پیغام) {
		rollback_شاخص_files();
		fprintf(stderr, _("Aborting commit; you did not edit the message.\n"));
		وتل(1);
	}

	إذا (fixup_پیغام && starts_with(sb.buf, "amend! ") &&
	    !allow_تشه_پیغام) {
		بنية strbuf body = STRBUF_INIT;
		اندازه_t len = commit_subject_اوږدتیا(sb.buf);
		strbuf_addstr(&body, sb.buf + len);
		إذا (پیغام_is_تشه(&body, cleanup_ډېرپېښ)) {
			rollback_شاخص_files();
			fprintf(stderr, _("Aborting commit due to empty commit message body.\n"));
			وتل(1);
		}
		strbuf_release(&body);
	}

	إذا (amend) {
		ثابت حرف *exclude_gpgsig[3] = { "gpgsig", "gpgsig-sha256", عدم };
		extra = read_commit_extra_سرلیکونه(current_سر, exclude_gpgsig);
	} وإلا {
		بنية commit_extra_سرلیک **لکۍ = &extra;
		append_یوځای_نښان_سرلیکونه(parents, &لکۍ);
	}

	إذا (commit_ونه_extended(sb.buf, sb.len, &the_repository->شاخص->cache_ونه->oid,
				 parents, &oid, author_ident.buf, عدم,
				 sign_commit, extra)) {
		rollback_شاخص_files();
		die(_("failed to write commit object"));
	}

	إذا (update_سر_with_reflog(current_سر, &oid, reflog_پیغام, &sb,
				    &err)) {
		rollback_شاخص_files();
		die("%s", err.buf);
	}

	sequencer_post_commit_cleanup(the_repository, 0);
	unlink(git_لار_یوځای_سر(the_repository));
	unlink(git_لار_یوځای_پیغام(the_repository));
	unlink(git_لار_یوځای_ډېرپېښ(the_repository));
	unlink(git_لار_squash_پیغام(the_repository));

	إذا (commit_شاخص_files())
		die(_("repository has been updated, but unable to write\n"
		      "new index file. Check that disk is not full and quota is\n"
		      "not exceeded, and then \"git restore --staged :/\" to recover."));

	git_test_write_commit_ګراف_or_die(the_repository);

	repo_rerere(the_repository, 0);
	پرلپسې_auto_maintenance(the_repository, quiet);
	پرلپسې_commit_hook(use_editor, repo_get_شاخص_file(the_repository),
			عدم, "post-commit", عدم);
	إذا (amend && !no_post_rewrite) {
		commit_post_rewrite(the_repository, current_سر, &oid);
	}
	إذا (!quiet) {
		بینښه صحيح flags = 0;

		إذا (!current_سر)
			flags |= SUMMARY_INITIAL_COMMIT;
		إذا (author_date_is_interesting())
			flags |= SUMMARY_SHOW_AUTHOR_DATE;
		print_commit_summary(the_repository, مختاړی,
				     &oid, flags);
	}

	apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
			    عدم, عدم, عدم, عدم);

cleanup:
	free_commit_extra_سرلیکونه(extra);
	commit_list_free(parents);
	strbuf_release(&author_ident);
	strbuf_release(&err);
	strbuf_release(&sb);
	حرر(logfile);
	حرر(template_file);
	أرجع ret;
}
