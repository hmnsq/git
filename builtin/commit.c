/*
 * Builtin "git commit"
 *
 * Copyright (c) 2007 Kristian Høgsberg <krh@redhat.com>
 * Based on git-commit.sh by Junio C Hamano and Linus Torvalds
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "advice.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "lockfile.h"
#অন্তর্ভাব "cache-tree.h"
#অন্তর্ভাব "color.h"
#অন্তর্ভাব "dir.h"
#অন্তর্ভাব "editor.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "diff.h"
#অন্তর্ভাব "commit.h"
#অন্তর্ভাব "add-interactive.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "revision.h"
#অন্তর্ভাব "wt-status.h"
#অন্তর্ভাব "run-command.h"
#অন্তর্ভাব "strbuf.h"
#অন্তর্ভাব "object-name.h"
#অন্তর্ভাব "parse-options.h"
#অন্তর্ভাব "path.h"
#অন্তর্ভাব "preload-index.h"
#অন্তর্ভাব "read-cache.h"
#অন্তর্ভাব "repository.h"
#অন্তর্ভাব "string-list.h"
#অন্তর্ভাব "rerere.h"
#অন্তর্ভাব "unpack-trees.h"
#অন্তর্ভাব "column.h"
#অন্তর্ভাব "sequencer.h"
#অন্তর্ভাব "sparse-index.h"
#অন্তর্ভাব "mailmap.h"
#অন্তর্ভাব "help.h"
#অন্তর্ভাব "commit-reach.h"
#অন্তর্ভাব "commit-graph.h"
#অন্তর্ভাব "pretty.h"
#অন্তর্ভাব "trailer.h"

স্থির ধ্রুবক অক্ষর * ধ্রুবক builtin_commit_usage[] = {
	N_("git commit [-a | --interactive | --patch] [-s] [-v] [-u[<mode>]] [--amend]\n"
	   "           [--dry-run] [(-c | -C | --squash) <commit> | --fixup [(amend|reword):]<commit>]\n"
	   "           [-F <file> | -m <msg>] [--reset-author] [--allow-empty]\n"
	   "           [--allow-empty-message] [--no-verify] [-e] [--author=<author>]\n"
	   "           [--date=<date>] [--cleanup=<mode>] [--[no-]status]\n"
	   "           [-i | -o] [--pathspec-from-file=<file> [--pathspec-file-nul]]\n"
	   "           [(--trailer <token>[(=|:)<value>])...] [-S[<keyid>]]\n"
	   "           [--] [<pathspec>...]"),
	ফাঁকা
};

স্থির ধ্রুবক অক্ষর * ধ্রুবক builtin_status_usage[] = {
	N_("git status [<options>] [--] [<pathspec>...]"),
	ফাঁকা
};

স্থির ধ্রুবক অক্ষর empty_amend_advice[] =
N_("You asked to amend the most recent commit, but doing so would make\n"
"it empty. You can repeat your command with --allow-empty, or you can\n"
"remove the commit entirely with \"git reset HEAD^\".\n");

স্থির ধ্রুবক অক্ষর empty_cherry_pick_advice[] =
N_("The previous cherry-pick is now empty, possibly due to conflict resolution.\n"
"If you wish to commit it anyway, use:\n"
"\n"
"    git commit --allow-empty\n"
"\n");

স্থির ধ্রুবক অক্ষর empty_rebase_pick_advice[] =
N_("Otherwise, please use 'git rebase --skip'\n");

স্থির ধ্রুবক অক্ষর empty_cherry_pick_advice_single[] =
N_("Otherwise, please use 'git cherry-pick --skip'\n");

স্থির ধ্রুবক অক্ষর empty_cherry_pick_advice_multi[] =
N_("and then use:\n"
"\n"
"    git cherry-pick --continue\n"
"\n"
"to resume cherry-picking the remaining commits.\n"
"If you wish to skip this commit, use:\n"
"\n"
"    git cherry-pick --skip\n"
"\n");

স্থির ধ্রুবক অক্ষর *color_status_slots[] = {
	[WT_STATUS_HEADER]	  = "header",
	[WT_STATUS_UPDATED]	  = "updated",
	[WT_STATUS_CHANGED]	  = "changed",
	[WT_STATUS_UNTRACKED]	  = "untracked",
	[WT_STATUS_NOBRANCH]	  = "noBranch",
	[WT_STATUS_UNMERGED]	  = "unmerged",
	[WT_STATUS_LOCAL_BRANCH]  = "localBranch",
	[WT_STATUS_REMOTE_BRANCH] = "remoteBranch",
	[WT_STATUS_ONBRANCH]	  = "branch",
};

স্থির ধ্রুবক অক্ষর *use_message_বাফার;
স্থির কাঠামো lock_file সূচক_lock; /* real index */
স্থির কাঠামো lock_file false_lock; /* used only for partial commits */
স্থির গণনা {
	COMMIT_AS_IS = 1,
	COMMIT_NORMAL,
	COMMIT_PARTIAL
} commit_style;

স্থির ধ্রুবক অক্ষর *force_author;
স্থির অক্ষর *logfile;
স্থির অক্ষর *template_file;
/*
 * The _message variables are commit names from which to take
 * the commit message and/or authorship.
 */
স্থির ধ্রুবক অক্ষর *author_message, *author_message_বাফার;
স্থির ধ্রুবক অক্ষর *edit_message, *use_message;
স্থির অক্ষর *fixup_message, *fixup_commit, *squash_message;
স্থির ধ্রুবক অক্ষর *fixup_prefix;
স্থির পূর্ণসংখ্যা all, also, interactive, patch_interactive, only, amend, signoff;
স্থির কাঠামো interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
স্থির পূর্ণসংখ্যা edit_flag = -1; /* unspecified */
স্থির পূর্ণসংখ্যা quiet, verbose, no_verify, allow_empty, dry_run, renew_authorship;
স্থির পূর্ণসংখ্যা config_commit_verbose = -1; /* unspecified */
স্থির পূর্ণসংখ্যা no_post_rewrite, allow_empty_message, pathspec_file_nul;
স্থির ধ্রুবক অক্ষর *untracked_files_arg, *force_date, *ignore_submodule_arg, *ignored_arg;
স্থির ধ্রুবক অক্ষর *sign_commit, *pathspec_from_file;
স্থির কাঠামো strvec trailer_args = STRVEC_INIT;

/*
 * The default commit message cleanup mode will remove the lines
 * beginning with # (shell comments) and leading and trailing
 * whitespaces (empty lines or containing only whitespaces)
 * if editor is used, and only the whitespaces if the message
 * is specified explicitly.
 */
স্থির গণনা commit_বার্তা_cleanup_mode cleanup_mode;
স্থির অক্ষর *cleanup_config;

স্থির গণনা commit_whence whence;
স্থির পূর্ণসংখ্যা use_editor = 1, include_status = 1;
স্থির পূর্ণসংখ্যা have_option_m;
স্থির কাঠামো strbuf message = STRBUF_INIT;

স্থির গণনা wt_status_format status_format = STATUS_FORMAT_UNSPECIFIED;

স্থির পূর্ণসংখ্যা opt_parse_porcelain(ধ্রুবক কাঠামো option *opt, ধ্রুবক অক্ষর *arg, পূর্ণসংখ্যা unset)
{
	গণনা wt_status_format *মান = (গণনা wt_status_format *)opt->মান;
	যদি (unset)
		*মান = STATUS_FORMAT_NONE;
	অন্যথা যদি (!arg)
		*মান = STATUS_FORMAT_PORCELAIN;
	অন্যথা যদি (!strcmp(arg, "v1") || !strcmp(arg, "1"))
		*মান = STATUS_FORMAT_PORCELAIN;
	অন্যথা যদি (!strcmp(arg, "v2") || !strcmp(arg, "2"))
		*মান = STATUS_FORMAT_PORCELAIN_V2;
	অন্যথা
		die("unsupported porcelain version '%s'", arg);

	ফেরত 0;
}

স্থির পূর্ণসংখ্যা opt_parse_m(ধ্রুবক কাঠামো option *opt, ধ্রুবক অক্ষর *arg, পূর্ণসংখ্যা unset)
{
	কাঠামো strbuf *buf = opt->মান;
	যদি (unset) {
		have_option_m = 0;
		strbuf_setlen(buf, 0);
	} অন্যথা {
		have_option_m = 1;
		যদি (buf->len)
			strbuf_addch(buf, '\n');
		strbuf_addstr(buf, arg);
		strbuf_complete_সারি(buf);
	}
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা opt_parse_rename_score(ধ্রুবক কাঠামো option *opt, ধ্রুবক অক্ষর *arg, পূর্ণসংখ্যা unset)
{
	ধ্রুবক অক্ষর **মান = opt->মান;

	BUG_ON_OPT_NEG(unset);

	যদি (arg != ফাঁকা && *arg == '=')
		arg = arg + 1;

	*মান = arg;
	ফেরত 0;
}

স্থির শূন্য determine_whence(কাঠামো wt_status *s)
{
	যদি (file_exists(git_path_merge_শীর্ষ(the_repository)))
		whence = FROM_MERGE;
	অন্যথা যদি (!sequencer_determine_whence(the_repository, &whence))
		whence = FROM_COMMIT;
	যদি (s)
		s->whence = whence;
}

স্থির শূন্য status_init_config(কাঠামো wt_status *s, config_fn_t fn)
{
	wt_status_prepare(the_repository, s);
	init_diff_ui_defaults();
	repo_config(the_repository, fn, s);
	determine_whence(s);
	s->hints = advice_enabled(ADVICE_STATUS_HINTS); /* must come after repo_config() */
}

স্থির শূন্য rollback_সূচক_files(শূন্য)
{
	নির্বাচন (commit_style) {
	ক্ষেত্র COMMIT_AS_IS:
		বিরতি; /* nothing to do */
	ক্ষেত্র COMMIT_NORMAL:
		rollback_lock_file(&সূচক_lock);
		বিরতি;
	ক্ষেত্র COMMIT_PARTIAL:
		rollback_lock_file(&সূচক_lock);
		rollback_lock_file(&false_lock);
		বিরতি;
	}
}

স্থির পূর্ণসংখ্যা commit_সূচক_files(শূন্য)
{
	পূর্ণসংখ্যা err = 0;

	নির্বাচন (commit_style) {
	ক্ষেত্র COMMIT_AS_IS:
		বিরতি; /* nothing to do */
	ক্ষেত্র COMMIT_NORMAL:
		err = commit_lock_file(&সূচক_lock);
		বিরতি;
	ক্ষেত্র COMMIT_PARTIAL:
		err = commit_lock_file(&সূচক_lock);
		rollback_lock_file(&false_lock);
		বিরতি;
	}

	ফেরত err;
}

/*
 * Take a union of paths in the index and the named tree (typically, "HEAD"),
 * and return the paths that match the given pattern in list.
 */
স্থির পূর্ণসংখ্যা list_paths(কাঠামো string_list *list, ধ্রুবক অক্ষর *with_বৃক্ষ,
		      ধ্রুবক কাঠামো pathspec *pattern)
{
	পূর্ণসংখ্যা i, ret;
	অক্ষর *m;

	যদি (!pattern->nr)
		ফেরত 0;

	m = xcalloc(1, pattern->nr);

	যদি (with_বৃক্ষ) {
		অক্ষর *max_prefix = common_prefix(pattern);
		overlay_বৃক্ষ_on_সূচক(the_repository->সূচক, with_বৃক্ষ, max_prefix);
		মুক্ত(max_prefix);
	}

	/* TODO: audit for interaction with sparse-index. */
	ensure_full_সূচক(the_repository->সূচক);
	জন্য (i = 0; i < the_repository->সূচক->cache_nr; i++) {
		ধ্রুবক কাঠামো cache_entry *ce = the_repository->সূচক->cache[i];
		কাঠামো string_list_item *item;

		যদি (ce->ce_flags & CE_UPDATE)
			চালু;
		যদি (!ce_path_match(the_repository->সূচক, ce, pattern, m))
			চালু;
		item = string_list_ঢোকাও(list, ce->name);
		যদি (ce_skip_worktree(ce))
			item->util = item; /* better a valid pointer than a fake one */
	}

	ret = report_path_error(m, pattern);
	মুক্ত(m);
	ফেরত ret;
}

স্থির শূন্য add_remove_files(কাঠামো string_list *list)
{
	পূর্ণসংখ্যা i;
	জন্য (i = 0; i < list->nr; i++) {
		কাঠামো stat st;
		কাঠামো string_list_item *p = &(list->বস্তুসমূহ[i]);

		/* p->util is skip-worktree */
		যদি (p->util)
			চালু;

		যদি (!lstat(p->string, &st)) {
			যদি (add_to_সূচক(the_repository->সূচক, p->string, &st, 0))
				die(_("updating files failed"));
		} অন্যথা
			remove_file_from_সূচক(the_repository->সূচক, p->string);
	}
}

স্থির শূন্য create_base_সূচক(ধ্রুবক কাঠামো commit *current_শীর্ষ)
{
	কাঠামো বৃক্ষ *বৃক্ষ;
	কাঠামো unpack_trees_options opts;
	কাঠামো বৃক্ষ_desc t;

	যদি (!current_শীর্ষ) {
		discard_সূচক(the_repository->সূচক);
		ফেরত;
	}

	memset(&opts, 0, আকার(opts));
	opts.শীর্ষ_idx = 1;
	opts.সূচক_only = 1;
	opts.merge = 1;
	opts.src_সূচক = the_repository->সূচক;
	opts.dst_সূচক = the_repository->সূচক;

	opts.fn = oneway_merge;
	বৃক্ষ = repo_parse_বৃক্ষ_indirect(the_repository,
					&current_শীর্ষ->object.oid);
	যদি (!বৃক্ষ)
		die(_("failed to unpack HEAD tree object"));
	যদি (repo_parse_বৃক্ষ(the_repository, বৃক্ষ) < 0)
		exit(128);
	init_বৃক্ষ_desc(&t, &বৃক্ষ->object.oid, বৃক্ষ->বাফার, বৃক্ষ->আকার_2);
	যদি (unpack_trees(1, &t, &opts))
		exit(128); /* We've already reported the error, finish dying */
}

স্থির শূন্য refresh_cache_or_die(পূর্ণসংখ্যা refresh_flags)
{
	/*
	 * refresh_flags contains REFRESH_QUIET, so the only errors
	 * are for unmerged entries.
	 */
	যদি (refresh_সূচক(the_repository->সূচক, refresh_flags | REFRESH_IN_PORCELAIN, ফাঁকা, ফাঁকা, ফাঁকা))
		die_resolve_conflict("commit");
}

স্থির ধ্রুবক অক্ষর *prepare_সূচক(ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix,
				 ধ্রুবক কাঠামো commit *current_শীর্ষ, পূর্ণসংখ্যা is_status)
{
	কাঠামো string_list partial = STRING_LIST_INIT_DUP;
	কাঠামো pathspec pathspec;
	পূর্ণসংখ্যা refresh_flags = REFRESH_QUIET;
	ধ্রুবক অক্ষর *ret;
	অক্ষর *path = ফাঁকা;

	যদি (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	যদি (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	যদি (is_status)
		refresh_flags |= REFRESH_UNMERGED;
	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_FULL,
		       prefix, argv);

	যদি (pathspec_from_file) {
		যদি (interactive)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--interactive/--patch");

		যদি (all)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "-a");

		যদি (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_FULL,
				    prefix, pathspec_from_file, pathspec_file_nul);
	} অন্যথা যদি (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	যদি (!pathspec.nr && (also || (only && !allow_empty &&
	    (!amend || (fixup_message && strcmp(fixup_prefix, "amend"))))))
		die(_("No paths with --include/--only does not make sense."));

	যদি (repo_read_সূচক_preload(the_repository, &pathspec, 0) < 0)
		die(_("index file corrupt"));

	যদি (interactive) {
		অক্ষর *old_সূচক_env = ফাঁকা, *old_repo_সূচক_file;
		repo_hold_locked_সূচক(the_repository, &সূচক_lock,
				       LOCK_DIE_ON_ERROR);

		refresh_cache_or_die(refresh_flags);

		যদি (write_locked_সূচক(the_repository->সূচক, &সূচক_lock, 0))
			die(_("unable to create temporary index"));

		old_repo_সূচক_file = the_repository->সূচক_file;
		the_repository->সূচক_file =
			(অক্ষর *)get_lock_file_path(&সূচক_lock);
		old_সূচক_env = xstrdup_or_null(getenv(সূচক_ENVIRONMENT));
		setenv(সূচক_ENVIRONMENT, the_repository->সূচক_file, 1);

		যদি (interactive_add(the_repository, argv, prefix, patch_interactive, &interactive_opts) != 0)
			die(_("interactive add failed"));

		the_repository->সূচক_file = old_repo_সূচক_file;
		যদি (old_সূচক_env && *old_সূচক_env)
			setenv(সূচক_ENVIRONMENT, old_সূচক_env, 1);
		অন্যথা
			unsetenv(সূচক_ENVIRONMENT);
		FREE_AND_NULL(old_সূচক_env);

		discard_সূচক(the_repository->সূচক);
		read_সূচক_from(the_repository->সূচক, get_lock_file_path(&সূচক_lock),
				repo_get_git_dir(the_repository));
		যদি (cache_বৃক্ষ_update(the_repository->সূচক, WRITE_বৃক্ষ_SILENT) == 0) {
			যদি (reopen_lock_file(&সূচক_lock) < 0)
				die(_("unable to write index file"));
			যদি (write_locked_সূচক(the_repository->সূচক, &সূচক_lock, 0))
				die(_("unable to update temporary index"));
		} অন্যথা
			warning(_("Failed to update main cache tree"));

		commit_style = COMMIT_NORMAL;
		ret = get_lock_file_path(&সূচক_lock);
		যাও out;
	} অন্যথা {
		যদি (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--interactive/--patch");
		যদি (interactive_opts.interhunkcontext != -1)
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
	যদি (all || (also && pathspec.nr)) {
		অক্ষর *ps_matched = xcalloc(pathspec.nr, 1);
		repo_hold_locked_সূচক(the_repository, &সূচক_lock,
				       LOCK_DIE_ON_ERROR);
		add_files_to_cache(the_repository, also ? prefix : ফাঁকা,
				   &pathspec, ps_matched, 0, 0, 0 );
		যদি (!all && report_path_error(ps_matched, &pathspec))
			exit(128);

		refresh_cache_or_die(refresh_flags);
		cache_বৃক্ষ_update(the_repository->সূচক, WRITE_বৃক্ষ_SILENT);
		যদি (write_locked_সূচক(the_repository->সূচক, &সূচক_lock, 0))
			die(_("unable to write new index file"));
		commit_style = COMMIT_NORMAL;
		ret = get_lock_file_path(&সূচক_lock);
		মুক্ত(ps_matched);
		যাও out;
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
	যদি (!only && !pathspec.nr) {
		repo_hold_locked_সূচক(the_repository, &সূচক_lock,
				       LOCK_DIE_ON_ERROR);
		refresh_cache_or_die(refresh_flags);
		যদি (the_repository->সূচক->cache_changed
		    || !cache_বৃক্ষ_fully_valid(the_repository->সূচক))
			cache_বৃক্ষ_update(the_repository->সূচক, WRITE_বৃক্ষ_SILENT);
		যদি (write_locked_সূচক(the_repository->সূচক, &সূচক_lock,
				       COMMIT_LOCK | SKIP_IF_UNCHANGED))
			die(_("unable to write new index file"));
		commit_style = COMMIT_AS_IS;
		ret = repo_get_সূচক_file(the_repository);
		যাও out;
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

	যদি (whence != FROM_COMMIT) {
		যদি (whence == FROM_MERGE)
			die(_("cannot do a partial commit during a merge."));
		অন্যথা যদি (is_from_cherry_pick(whence))
			die(_("cannot do a partial commit during a cherry-pick."));
		অন্যথা যদি (is_from_rebase(whence))
			die(_("cannot do a partial commit during a rebase."));
	}

	যদি (list_paths(&partial, !current_শীর্ষ ? ফাঁকা : "HEAD", &pathspec))
		exit(1);

	discard_সূচক(the_repository->সূচক);
	যদি (repo_read_সূচক(the_repository) < 0)
		die(_("cannot read the index"));

	repo_hold_locked_সূচক(the_repository, &সূচক_lock, LOCK_DIE_ON_ERROR);
	add_remove_files(&partial);
	refresh_সূচক(the_repository->সূচক, REFRESH_QUIET, ফাঁকা, ফাঁকা, ফাঁকা);
	cache_বৃক্ষ_update(the_repository->সূচক, WRITE_বৃক্ষ_SILENT);
	যদি (write_locked_সূচক(the_repository->সূচক, &সূচক_lock, 0))
		die(_("unable to write new index file"));

	path = repo_git_path(the_repository, "next-index-%"PRIuMAX,
			     (uintmax_t) getpid());
	hold_lock_file_for_update(&false_lock, path,
				  LOCK_DIE_ON_ERROR);

	create_base_সূচক(current_শীর্ষ);
	add_remove_files(&partial);
	refresh_সূচক(the_repository->সূচক, REFRESH_QUIET, ফাঁকা, ফাঁকা, ফাঁকা);

	যদি (write_locked_সূচক(the_repository->সূচক, &false_lock, 0))
		die(_("unable to write temporary index file"));

	discard_সূচক(the_repository->সূচক);
	ret = get_lock_file_path(&false_lock);
	read_সূচক_from(the_repository->সূচক, ret, repo_get_git_dir(the_repository));
out:
	string_list_clear(&partial, 0);
	clear_pathspec(&pathspec);
	মুক্ত(path);
	ফেরত ret;
}

স্থির পূর্ণসংখ্যা run_status(FILE *fp, ধ্রুবক অক্ষর *সূচক_file, ধ্রুবক অক্ষর *prefix, পূর্ণসংখ্যা nowarn,
		      কাঠামো wt_status *s)
{
	কাঠামো object_id oid;

	যদি (s->relative_paths)
		s->prefix = prefix;

	যদি (amend) {
		s->amend = 1;
		s->reference = "HEAD^1";
	}
	s->verbose = verbose;
	s->সূচক_file = সূচক_file;
	s->fp = fp;
	s->nowarn = nowarn;
	s->is_initial = repo_get_oid(the_repository, s->reference, &oid) ? 1 : 0;
	যদি (!s->is_initial)
		oidcpy(&s->oid_commit, &oid);
	s->status_format = status_format;
	s->ignore_submodule_arg = ignore_submodule_arg;

	wt_status_collect(s);
	wt_status_print(s);
	wt_status_collect_free_buffers(s);

	ফেরত s->committable;
}

স্থির পূর্ণসংখ্যা is_a_merge(ধ্রুবক কাঠামো commit *current_শীর্ষ)
{
	ফেরত !!(current_শীর্ষ->parents && current_শীর্ষ->parents->পরবর্তী);
}

স্থির শূন্য assert_split_ident(কাঠামো ident_split *id, ধ্রুবক কাঠামো strbuf *buf)
{
	যদি (split_ident_সারি(id, buf->buf, buf->len) || !id->date_begin)
		BUG("unable to parse our own ident: %s", buf->buf);
}

স্থির শূন্য export_one(ধ্রুবক অক্ষর *var, ধ্রুবক অক্ষর *s, ধ্রুবক অক্ষর *e, পূর্ণসংখ্যা hack)
{
	কাঠামো strbuf buf = STRBUF_INIT;
	যদি (hack)
		strbuf_addch(&buf, hack);
	strbuf_add(&buf, s, e - s);
	setenv(var, buf.buf, 1);
	strbuf_release(&buf);
}

স্থির পূর্ণসংখ্যা parse_force_date(ধ্রুবক অক্ষর *in, কাঠামো strbuf *out)
{
	strbuf_addch(out, '@');

	যদি (parse_date(in, out) < 0) {
		পূর্ণসংখ্যা errors = 0;
		unsigned long t = approxidate_careful(in, &errors);
		যদি (errors)
			ফেরত -1;
		strbuf_addf(out, "%lu", t);
	}

	ফেরত 0;
}

স্থির শূন্য set_ident_var(অক্ষর **buf, অক্ষর *val)
{
	মুক্ত(*buf);
	*buf = val;
}

স্থির শূন্য determine_author_info(কাঠামো strbuf *author_ident)
{
	অক্ষর *name, *email, *date;
	কাঠামো ident_split author;

	name = xstrdup_or_null(getenv("GIT_AUTHOR_NAME"));
	email = xstrdup_or_null(getenv("GIT_AUTHOR_EMAIL"));
	date = xstrdup_or_null(getenv("GIT_AUTHOR_DATE"));

	যদি (author_message) {
		কাঠামো ident_split ident;
		আকার_t len;
		ধ্রুবক অক্ষর *a;

		a = find_commit_header(author_message_বাফার, "author", &len);
		যদি (!a)
			die(_("commit '%s' lacks author header"), author_message);
		যদি (split_ident_সারি(&ident, a, len) < 0)
			die(_("commit '%s' has malformed author line"), author_message);

		set_ident_var(&name, xmemdupz(ident.name_begin, ident.name_end - ident.name_begin));
		set_ident_var(&email, xmemdupz(ident.mail_begin, ident.mail_end - ident.mail_begin));

		যদি (ident.date_begin) {
			কাঠামো strbuf date_buf = STRBUF_INIT;
			strbuf_addch(&date_buf, '@');
			strbuf_add(&date_buf, ident.date_begin, ident.date_end - ident.date_begin);
			strbuf_addch(&date_buf, ' ');
			strbuf_add(&date_buf, ident.tz_begin, ident.tz_end - ident.tz_begin);
			set_ident_var(&date, strbuf_detach(&date_buf, ফাঁকা));
		}
	}

	যদি (force_author) {
		কাঠামো ident_split ident;

		যদি (split_ident_সারি(&ident, force_author, strlen(force_author)) < 0)
			die(_("malformed --author parameter"));
		set_ident_var(&name, xmemdupz(ident.name_begin, ident.name_end - ident.name_begin));
		set_ident_var(&email, xmemdupz(ident.mail_begin, ident.mail_end - ident.mail_begin));
	}

	যদি (force_date) {
		কাঠামো strbuf date_buf = STRBUF_INIT;
		যদি (parse_force_date(force_date, &date_buf))
			die(_("invalid date format: %s"), force_date);
		set_ident_var(&date, strbuf_detach(&date_buf, ফাঁকা));
	}

	strbuf_addstr(author_ident, fmt_ident(name, email, WANT_AUTHOR_IDENT, date,
				IDENT_STRICT));
	assert_split_ident(&author, author_ident);
	export_one("GIT_AUTHOR_NAME", author.name_begin, author.name_end, 0);
	export_one("GIT_AUTHOR_EMAIL", author.mail_begin, author.mail_end, 0);
	export_one("GIT_AUTHOR_DATE", author.date_begin, author.tz_end, '@');
	মুক্ত(name);
	মুক্ত(email);
	মুক্ত(date);
}

স্থির পূর্ণসংখ্যা author_date_is_interesting(শূন্য)
{
	ফেরত author_message || force_date;
}

#ifndef WITH_BREAKING_CHANGES
স্থির শূন্য adjust_comment_সারি_char(ধ্রুবক কাঠামো strbuf *sb)
{
	অক্ষর candidates[] = "#;@!$%^&|:";
	অক্ষর *candidate;
	ধ্রুবক অক্ষর *p;
	আকার_t cutoff;

	/* Ignore comment chars in trailing comments (e.g., Conflicts:) */
	cutoff = sb->len - ignored_log_message_bytes(sb->buf, sb->len);

	যদি (!memchr(sb->buf, candidates[0], sb->len)) {
		মুক্ত(comment_সারি_str_to_free);
		comment_সারি_str = comment_সারি_str_to_free =
			xstrfmt("%c", candidates[0]);
		ফেরত;
	}

	p = sb->buf;
	candidate = strchr(candidates, *p);
	যদি (candidate)
		*candidate = ' ';
	জন্য (p = sb->buf; p + 1 < sb->buf + cutoff; p++) {
		যদি ((p[0] == '\n' || p[0] == '\r') && p[1]) {
			candidate = strchr(candidates, p[1]);
			যদি (candidate)
				*candidate = ' ';
		}
	}

	জন্য (p = candidates; *p == ' '; p++)
		;
	যদি (!*p)
		die(_("unable to select a comment character that is not used\n"
		      "in the current commit message"));
	মুক্ত(comment_সারি_str_to_free);
	comment_সারি_str = comment_সারি_str_to_free = xstrfmt("%c", *p);
}
#endif /* !WITH_BREAKING_CHANGES */

স্থির শূন্য prepare_amend_commit(কাঠামো commit *commit, কাঠামো strbuf *sb,
				কাঠামো pretty_print_context *ctx)
{
	ধ্রুবক অক্ষর *বাফার, *subject, *fmt;

	বাফার = repo_get_commit_বাফার(the_repository, commit, ফাঁকা);
	find_commit_subject(বাফার, &subject);
	/*
	 * If we amend the 'amend!' commit then we don't want to
	 * duplicate the subject line.
	 */
	fmt = starts_with(subject, "amend!") ? "%b" : "%B";
	repo_format_commit_message(the_repository, commit, fmt, sb, ctx);
	repo_unuse_commit_বাফার(the_repository, commit, বাফার);
}

স্থির শূন্য change_তথ্য_free(শূন্য *util, ধ্রুবক অক্ষর *str UNUSED)
{
	কাঠামো wt_status_change_তথ্য *d = util;
	মুক্ত(d->rename_source);
	মুক্ত(d);
}

স্থির পূর্ণসংখ্যা prepare_to_commit(ধ্রুবক অক্ষর *সূচক_file, ধ্রুবক অক্ষর *prefix,
			     কাঠামো commit *current_শীর্ষ,
			     কাঠামো wt_status *s,
			     কাঠামো strbuf *author_ident)
{
	কাঠামো stat statbuf;
	কাঠামো strbuf committer_ident = STRBUF_INIT;
	পূর্ণসংখ্যা committable;
	কাঠামো strbuf sb = STRBUF_INIT;
	ধ্রুবক অক্ষর *hook_arg1 = ফাঁকা;
	ধ্রুবক অক্ষর *hook_arg2 = ফাঁকা;
	পূর্ণসংখ্যা clean_message_contents = (cleanup_mode != COMMIT_বার্তা_CLEANUP_NONE);
	পূর্ণসংখ্যা old_display_comment_prefix;
	পূর্ণসংখ্যা invoked_hook;

	/* This checks and barfs if author is badly specified */
	determine_author_info(author_ident);

	যদি (!no_verify && run_commit_hook(use_editor, সূচক_file, &invoked_hook,
					  "pre-commit", ফাঁকা))
		ফেরত 0;

	যদি (squash_message) {
		/*
		 * Insert the proper subject line before other commit
		 * message options add their content.
		 */
		যদি (use_message && !strcmp(use_message, squash_message))
			strbuf_addstr(&sb, "squash! ");
		অন্যথা {
			কাঠামো pretty_print_context ctx = {0};
			কাঠামো commit *c;
			c = lookup_commit_reference_by_name(squash_message);
			যদি (!c)
				die(_("could not lookup commit '%s'"), squash_message);
			ctx.output_encoding = get_commit_output_encoding();
			repo_format_commit_message(the_repository, c,
						   "squash! %s\n\n", &sb,
						   &ctx);
		}
	}

	যদি (have_option_m && !fixup_message) {
		strbuf_addbuf(&sb, &message);
		hook_arg1 = "message";
	} অন্যথা যদি (logfile && !strcmp(logfile, "-")) {
		যদি (isatty(0))
			fprintf(stderr, _("(reading log message from standard input)\n"));
		যদি (strbuf_read(&sb, 0, 0) < 0)
			die_errno(_("could not read log from standard input"));
		hook_arg1 = "message";
	} অন্যথা যদি (logfile) {
		যদি (strbuf_read_file(&sb, logfile, 0) < 0)
			die_errno(_("could not read log file '%s'"),
				  logfile);
		hook_arg1 = "message";
	} অন্যথা যদি (use_message) {
		ধ্রুবক অক্ষর *বাফার;
		বাফার = strstr(use_message_বাফার, "\n\n");
		যদি (বাফার)
			strbuf_addstr(&sb, skip_blank_সারিগুলি(বাফার + 2));
		hook_arg1 = "commit";
		hook_arg2 = use_message;
	} অন্যথা যদি (fixup_message) {
		কাঠামো pretty_print_context ctx = {0};
		কাঠামো commit *commit;
		অক্ষর *fmt;
		commit = lookup_commit_reference_by_name(fixup_commit);
		যদি (!commit)
			die(_("could not lookup commit '%s'"), fixup_commit);
		ctx.output_encoding = get_commit_output_encoding();
		fmt = xstrfmt("%s! %%s\n\n", fixup_prefix);
		repo_format_commit_message(the_repository, commit, fmt, &sb,
					   &ctx);
		মুক্ত(fmt);
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
		যদি (have_option_m && !strcmp(fixup_prefix, "fixup"))
			strbuf_addbuf(&sb, &message);

		যদি (!strcmp(fixup_prefix, "amend")) {
			যদি (have_option_m)
				die(_("options '%s' and '%s:%s' cannot be used together"), "-m", "--fixup", fixup_message);
			prepare_amend_commit(commit, &sb, &ctx);
		}
	} অন্যথা যদি (!stat(git_path_merge_বার্তা(the_repository), &statbuf)) {
		আকার_t merge_বার্তা_start;

		/*
		 * prepend SQUASH_MSG here if it exists and a
		 * "merge --squash" was originally performed
		 */
		যদি (!stat(git_path_squash_বার্তা(the_repository), &statbuf)) {
			যদি (strbuf_read_file(&sb, git_path_squash_বার্তা(the_repository), 0) < 0)
				die_errno(_("could not read SQUASH_MSG"));
			hook_arg1 = "squash";
		} অন্যথা
			hook_arg1 = "merge";

		merge_বার্তা_start = sb.len;
		যদি (strbuf_read_file(&sb, git_path_merge_বার্তা(the_repository), 0) < 0)
			die_errno(_("could not read MERGE_MSG"));

		যদি (cleanup_mode == COMMIT_বার্তা_CLEANUP_SCISSORS &&
		    wt_status_locate_end(sb.buf + merge_বার্তা_start,
					 sb.len - merge_বার্তা_start) <
				sb.len - merge_বার্তা_start)
			s->added_cut_সারি = 1;
	} অন্যথা যদি (!stat(git_path_squash_বার্তা(the_repository), &statbuf)) {
		যদি (strbuf_read_file(&sb, git_path_squash_বার্তা(the_repository), 0) < 0)
			die_errno(_("could not read SQUASH_MSG"));
		hook_arg1 = "squash";
	} অন্যথা যদি (template_file) {
		যদি (strbuf_read_file(&sb, template_file, 0) < 0)
			die_errno(_("could not read '%s'"), template_file);
		hook_arg1 = "template";
		clean_message_contents = 0;
	}

	/*
	 * The remaining cases don't modify the template message, but
	 * just set the argument(s) to the prepare-commit-msg hook.
	 */
	অন্যথা যদি (whence == FROM_MERGE)
		hook_arg1 = "merge";
	অন্যথা যদি (is_from_cherry_pick(whence) || whence == FROM_REBASE_PICK) {
		hook_arg1 = "commit";
		hook_arg2 = "CHERRY_PICK_HEAD";
	}

	যদি (squash_message) {
		/*
		 * If squash_commit was used for the commit subject,
		 * then we're possibly hijacking other commit log options.
		 * Reset the hook args to tell the real story.
		 */
		hook_arg1 = "message";
		hook_arg2 = "";
	}

	s->fp = fopen_for_writing(git_path_commit_editmsg());
	যদি (!s->fp)
		die_errno(_("could not open '%s'"), git_path_commit_editmsg());

	/* Ignore status.displayCommentPrefix: we do need comments in COMMIT_EDITMSG. */
	old_display_comment_prefix = s->display_comment_prefix;
	s->display_comment_prefix = 1;

	/*
	 * Most hints are counter-productive when the commit has
	 * already started.
	 */
	s->hints = 0;

	যদি (clean_message_contents)
		strbuf_stripspace(&sb, ফাঁকা);

	যদি (signoff)
		append_signoff(&sb, ignored_log_message_bytes(sb.buf, sb.len), 0);

	যদি (fwrite(sb.buf, 1, sb.len, s->fp) < sb.len)
		die_errno(_("could not write commit template"));

#ifndef WITH_BREAKING_CHANGES
	যদি (auto_comment_সারি_char)
		adjust_comment_সারি_char(&sb);
#endif /* !WITH_BREAKING_CHANGES */
	strbuf_release(&sb);

	/* This checks if committer ident is explicitly given */
	strbuf_addstr(&committer_ident, git_committer_info(IDENT_STRICT));
	যদি (use_editor && include_status) {
		পূর্ণসংখ্যা ident_shown = 0;
		গণনা git_colorbool saved_color_setting;
		কাঠামো ident_split ci, ai;
		ধ্রুবক অক্ষর *hint_cleanup_all = allow_empty_message ?
			_("Please enter the commit message for your changes."
			  " Lines starting\nwith '%s' will be ignored.\n") :
			_("Please enter the commit message for your changes."
			  " Lines starting\nwith '%s' will be ignored, and an empty"
			  " message aborts the commit.\n");
		ধ্রুবক অক্ষর *hint_cleanup_space = allow_empty_message ?
			_("Please enter the commit message for your changes."
			  " Lines starting\n"
			  "with '%s' will be kept; you may remove them"
			  " yourself if you want to.\n") :
			_("Please enter the commit message for your changes."
			  " Lines starting\n"
			  "with '%s' will be kept; you may remove them"
			  " yourself if you want to.\n"
			  "An empty message aborts the commit.\n");
		যদি (whence != FROM_COMMIT) {
			যদি (cleanup_mode == COMMIT_বার্তা_CLEANUP_SCISSORS)
				wt_status_add_cut_সারি(s);
			status_printf_ln(
				s, GIT_COLOR_NORMAL,
				whence == FROM_MERGE ?
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
		যদি (cleanup_mode == COMMIT_বার্তা_CLEANUP_ALL)
			status_printf(s, GIT_COLOR_NORMAL, hint_cleanup_all, comment_সারি_str);
		অন্যথা যদি (cleanup_mode == COMMIT_বার্তা_CLEANUP_SCISSORS) {
			যদি (whence == FROM_COMMIT)
				wt_status_add_cut_সারি(s);
		} অন্যথা /* COMMIT_MSG_CLEANUP_SPACE, that is. */
			status_printf(s, GIT_COLOR_NORMAL, hint_cleanup_space, comment_সারি_str);

		/*
		 * These should never fail because they come from our own
		 * fmt_ident. They may fail the sane_ident test, but we know
		 * that the name and mail pointers will at least be valid,
		 * which is enough for our tests and printing here.
		 */
		assert_split_ident(&ai, author_ident);
		assert_split_ident(&ci, &committer_ident);

		যদি (ident_cmp(&ai, &ci))
			status_printf_ln(s, GIT_COLOR_NORMAL,
				_("%s"
				"Author:    %.*s <%.*s>"),
				ident_shown++ ? "" : "\n",
				(পূর্ণসংখ্যা)(ai.name_end - ai.name_begin), ai.name_begin,
				(পূর্ণসংখ্যা)(ai.mail_end - ai.mail_begin), ai.mail_begin);

		যদি (author_date_is_interesting())
			status_printf_ln(s, GIT_COLOR_NORMAL,
				_("%s"
				"Date:      %s"),
				ident_shown++ ? "" : "\n",
				show_ident_date(&ai, DATE_MODE(NORMAL)));

		যদি (!committer_ident_sufficiently_given())
			status_printf_ln(s, GIT_COLOR_NORMAL,
				_("%s"
				"Committer: %.*s <%.*s>"),
				ident_shown++ ? "" : "\n",
				(পূর্ণসংখ্যা)(ci.name_end - ci.name_begin), ci.name_begin,
				(পূর্ণসংখ্যা)(ci.mail_end - ci.mail_begin), ci.mail_begin);

		status_printf_ln(s, GIT_COLOR_NORMAL, "%s", ""); /* Add new line for clarity */

		saved_color_setting = s->use_color;
		s->use_color = GIT_COLOR_NEVER;
		committable = run_status(s->fp, সূচক_file, prefix, 1, s);
		s->use_color = saved_color_setting;
		string_list_clear_func(&s->change, change_তথ্য_free);
	} অন্যথা {
		কাঠামো object_id oid;
		ধ্রুবক অক্ষর *parent = "HEAD";

		যদি (!the_repository->সূচক->initialized && repo_read_সূচক(the_repository) < 0)
			die(_("Cannot read index"));

		যদি (amend)
			parent = "HEAD^1";

		যদি (repo_get_oid(the_repository, parent, &oid)) {
			পূর্ণসংখ্যা i, ita_nr = 0;

			/* TODO: audit for interaction with sparse-index. */
			ensure_full_সূচক(the_repository->সূচক);
			জন্য (i = 0; i < the_repository->সূচক->cache_nr; i++)
				যদি (ce_intent_to_add(the_repository->সূচক->cache[i]))
					ita_nr++;
			committable = the_repository->সূচক->cache_nr > ita_nr;
		} অন্যথা {
			/*
			 * Unless the user did explicitly request a submodule
			 * ignore mode by passing a command line option we do
			 * not ignore any changed submodule SHA-1s when
			 * comparing index and parent, no matter what is
			 * configured. Otherwise we won't commit any
			 * submodules which were manually staged, which would
			 * be really confusing.
			 */
			কাঠামো diff_flags flags = DIFF_FLAGS_INIT;
			flags.override_submodule_config = 1;
			যদি (ignore_submodule_arg &&
			    !strcmp(ignore_submodule_arg, "all"))
				flags.ignore_submodules = 1;
			committable = সূচক_differs_from(the_repository,
							 parent, &flags, 1);
		}
	}
	strbuf_release(&committer_ident);

	fclose(s->fp);

	যদি (trailer_args.nr) {
		যদি (amend_file_with_trailers(git_path_commit_editmsg(), &trailer_args))
			die(_("unable to pass trailers to --trailers"));
		strvec_clear(&trailer_args);
	}

	/*
	 * Reject an attempt to record a non-merge empty commit without
	 * explicit --allow-empty. In the cherry-pick case, it may be
	 * empty due to conflict resolution, which the user should okay.
	 */
	যদি (!committable && whence != FROM_MERGE && !allow_empty &&
	    !(amend && is_a_merge(current_শীর্ষ))) {
		s->hints = advice_enabled(ADVICE_STATUS_HINTS);
		s->display_comment_prefix = old_display_comment_prefix;
		run_status(stdout, সূচক_file, prefix, 0, s);
		যদি (amend)
			fputs(_(empty_amend_advice), stderr);
		অন্যথা যদি (is_from_cherry_pick(whence) ||
			 whence == FROM_REBASE_PICK) {
			fputs(_(empty_cherry_pick_advice), stderr);
			যদি (whence == FROM_CHERRY_PICK_SINGLE)
				fputs(_(empty_cherry_pick_advice_single), stderr);
			অন্যথা যদি (whence == FROM_CHERRY_PICK_MULTI)
				fputs(_(empty_cherry_pick_advice_multi), stderr);
			অন্যথা
				fputs(_(empty_rebase_pick_advice), stderr);
		}
		ফেরত 0;
	}

	যদি (!no_verify && invoked_hook) {
		/*
		 * Re-read the index as the pre-commit-commit hook was invoked
		 * and could have updated it. We must do this before we invoke
		 * the editor and after we invoke run_status above.
		 */
		discard_সূচক(the_repository->সূচক);
	}
	read_সূচক_from(the_repository->সূচক, সূচক_file, repo_get_git_dir(the_repository));

	যদি (cache_বৃক্ষ_update(the_repository->সূচক, 0)) {
		error(_("Error building trees"));
		ফেরত 0;
	}

	যদি (run_commit_hook(use_editor, সূচক_file, ফাঁকা, "prepare-commit-msg",
			    git_path_commit_editmsg(), hook_arg1, hook_arg2, ফাঁকা))
		ফেরত 0;

	যদি (use_editor) {
		কাঠামো strvec env = STRVEC_INIT;

		strvec_pushf(&env, "GIT_INDEX_FILE=%s", সূচক_file);
		যদি (launch_editor(git_path_commit_editmsg(), ফাঁকা, env.v)) {
			fprintf(stderr,
			_("Please supply the message using either -m or -F option.\n"));
			exit(1);
		}
		strvec_clear(&env);
	}

	যদি (!no_verify &&
	    run_commit_hook(use_editor, সূচক_file, ফাঁকা, "commit-msg",
			    git_path_commit_editmsg(), ফাঁকা)) {
		ফেরত 0;
	}

	ফেরত 1;
}

স্থির ধ্রুবক অক্ষর *find_author_by_nickname(ধ্রুবক অক্ষর *name)
{
	কাঠামো rev_info revs;
	কাঠামো commit *commit;
	কাঠামো strbuf buf = STRBUF_INIT;
	ধ্রুবক অক্ষর *av[20];
	পূর্ণসংখ্যা ac = 0;

	repo_init_revisions(the_repository, &revs, ফাঁকা);
	strbuf_addf(&buf, "--author=%s", name);
	av[++ac] = "--all";
	av[++ac] = "-i";
	av[++ac] = buf.buf;
	av[++ac] = ফাঁকা;
	setup_revisions(ac, av, &revs, ফাঁকা);
	revs.mailmap = xmalloc(আকার(কাঠামো string_list));
	string_list_init_nodup(revs.mailmap);
	read_mailmap(the_repository, revs.mailmap);

	যদি (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	commit = get_revision(&revs);
	যদি (commit) {
		কাঠামো pretty_print_context ctx = {0};
		ctx.date_mode.type = DATE_NORMAL;
		strbuf_release(&buf);
		repo_format_commit_message(the_repository, commit,
					   "%aN <%aE>", &buf, &ctx);
		release_revisions(&revs);
		ফেরত strbuf_detach(&buf, ফাঁকা);
	}
	die(_("--author '%s' is not 'Name <email>' and matches no existing author"), name);
}

স্থির শূন্য handle_ignored_arg(কাঠামো wt_status *s)
{
	যদি (!ignored_arg)
		; /* default already initialized */
	অন্যথা যদি (!strcmp(ignored_arg, "traditional"))
		s->show_ignored_mode = SHOW_TRADITIONAL_IGNORED;
	অন্যথা যদি (!strcmp(ignored_arg, "no"))
		s->show_ignored_mode = SHOW_NO_IGNORED;
	অন্যথা যদি (!strcmp(ignored_arg, "matching"))
		s->show_ignored_mode = SHOW_MATCHING_IGNORED;
	অন্যথা
		die(_("Invalid ignored mode '%s'"), ignored_arg);
}

স্থির গণনা untracked_status_type parse_untracked_setting_name(ধ্রুবক অক্ষর *u)
{
	/*
	 * Please update $__git_untracked_file_modes in
	 * git-completion.bash when you add new options
	 */
	নির্বাচন (git_parse_maybe_bool(u)) {
	ক্ষেত্র 0:
		u = "no";
		বিরতি;
	ক্ষেত্র 1:
		u = "normal";
		বিরতি;
	পূর্বনির্ধারিত:
		বিরতি;
	}

	যদি (!strcmp(u, "no"))
		ফেরত SHOW_NO_UNTRACKED_FILES;
	অন্যথা যদি (!strcmp(u, "normal"))
		ফেরত SHOW_NORMAL_UNTRACKED_FILES;
	অন্যথা যদি (!strcmp(u, "all"))
		ফেরত SHOW_ALL_UNTRACKED_FILES;
	অন্যথা
		ফেরত SHOW_UNTRACKED_FILES_ERROR;
}

স্থির শূন্য handle_untracked_files_arg(কাঠামো wt_status *s)
{
	গণনা untracked_status_type u;

	যদি (!untracked_files_arg)
		ফেরত; /* default already initialized */

	u = parse_untracked_setting_name(untracked_files_arg);
	যদি (u == SHOW_UNTRACKED_FILES_ERROR)
		die(_("Invalid untracked files mode '%s'"),
		    untracked_files_arg);
	s->show_untracked_files = u;
}

স্থির ধ্রুবক অক্ষর *read_commit_message(ধ্রুবক অক্ষর *name)
{
	ধ্রুবক অক্ষর *out_enc;
	কাঠামো commit *commit;

	commit = lookup_commit_reference_by_name(name);
	যদি (!commit)
		die(_("could not lookup commit '%s'"), name);
	out_enc = get_commit_output_encoding();
	ফেরত repo_logmsg_reencode(the_repository, commit, ফাঁকা, out_enc);
}

/*
 * Enumerate what needs to be propagated when --porcelain
 * is not in effect here.
 */
স্থির কাঠামো status_deferred_config {
	গণনা wt_status_format status_format;
	পূর্ণসংখ্যা show_branch;
	গণনা ahead_behind_flags ahead_behind;
} status_deferred_config = {
	STATUS_FORMAT_UNSPECIFIED,
	-1, /* unspecified */
	AHEAD_BEHIND_UNSPECIFIED,
};

স্থির শূন্য finalize_deferred_config(কাঠামো wt_status *s)
{
	পূর্ণসংখ্যা use_deferred_config = (status_format != STATUS_FORMAT_PORCELAIN &&
				   status_format != STATUS_FORMAT_PORCELAIN_V2 &&
				   !s->null_termination);

	যদি (s->null_termination) {
		যদি (status_format == STATUS_FORMAT_NONE ||
		    status_format == STATUS_FORMAT_UNSPECIFIED)
			status_format = STATUS_FORMAT_PORCELAIN;
		অন্যথা যদি (status_format == STATUS_FORMAT_LONG)
			die(_("options '%s' and '%s' cannot be used together"), "--long", "-z");
	}

	যদি (use_deferred_config && status_format == STATUS_FORMAT_UNSPECIFIED)
		status_format = status_deferred_config.status_format;
	যদি (status_format == STATUS_FORMAT_UNSPECIFIED)
		status_format = STATUS_FORMAT_NONE;

	যদি (use_deferred_config && s->show_branch < 0)
		s->show_branch = status_deferred_config.show_branch;
	যদি (s->show_branch < 0)
		s->show_branch = 0;

	/*
	 * If the user did not give a "--[no]-ahead-behind" command
	 * line argument *AND* we will print in a human-readable format
	 * (short, long etc.) then we inherit from the status.aheadbehind
	 * config setting.  In all other cases (and porcelain V[12] formats
	 * in particular), we inherit _FULL for backwards compatibility.
	 */
	যদি (use_deferred_config &&
	    s->ahead_behind_flags == AHEAD_BEHIND_UNSPECIFIED)
		s->ahead_behind_flags = status_deferred_config.ahead_behind;

	যদি (s->ahead_behind_flags == AHEAD_BEHIND_UNSPECIFIED)
		s->ahead_behind_flags = AHEAD_BEHIND_FULL;
}

স্থির শূন্য check_fixup_reword_options(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর *argv[]) {
	যদি (whence != FROM_COMMIT) {
		যদি (whence == FROM_MERGE)
			die(_("You are in the middle of a merge -- cannot reword."));
		অন্যথা যদি (is_from_cherry_pick(whence))
			die(_("You are in the middle of a cherry-pick -- cannot reword."));
	}
	যদি (argc)
		die(_("reword option of '%s' and path '%s' cannot be used together"), "--fixup", *argv);
	যদি (patch_interactive || interactive || all || also || only)
		die(_("reword option of '%s' and '%s' cannot be used together"),
			"--fixup", "--patch/--interactive/--all/--include/--only");
}

স্থির পূর্ণসংখ্যা parse_and_validate_options(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর *argv[],
				      ধ্রুবক কাঠামো option *options,
				      ধ্রুবক অক্ষর * ধ্রুবক usage[],
				      ধ্রুবক অক্ষর *prefix,
				      কাঠামো commit *current_শীর্ষ,
				      কাঠামো wt_status *s)
{
	argc = parse_options(argc, argv, prefix, options, usage, 0);
	finalize_deferred_config(s);

	যদি (force_author && !strchr(force_author, '>'))
		force_author = find_author_by_nickname(force_author);

	যদি (force_author && renew_authorship)
		die(_("options '%s' and '%s' cannot be used together"), "--reset-author", "--author");

	যদি (logfile || have_option_m || use_message)
		use_editor = 0;

	/* Sanity check options */
	যদি (amend && !current_শীর্ষ)
		die(_("You have nothing to amend."));
	যদি (amend && whence != FROM_COMMIT) {
		যদি (whence == FROM_MERGE)
			die(_("You are in the middle of a merge -- cannot amend."));
		অন্যথা যদি (is_from_cherry_pick(whence))
			die(_("You are in the middle of a cherry-pick -- cannot amend."));
		অন্যথা যদি (whence == FROM_REBASE_PICK)
			die(_("You are in the middle of a rebase -- cannot amend."));
	}
	যদি (fixup_message && squash_message)
		die(_("options '%s' and '%s' cannot be used together"), "--squash", "--fixup");
	die_for_incompatible_opt4(!!use_message, "-C",
				  !!edit_message, "-c",
				  !!logfile, "-F",
				  !!fixup_message, "--fixup");
	die_for_incompatible_opt4(have_option_m, "-m",
				  !!edit_message, "-c",
				  !!use_message, "-C",
				  !!logfile, "-F");
	যদি (use_message || edit_message || logfile ||fixup_message || have_option_m)
		FREE_AND_NULL(template_file);
	যদি (edit_message)
		use_message = edit_message;
	যদি (amend && !use_message && !fixup_message)
		use_message = "HEAD";
	যদি (!use_message && !is_from_cherry_pick(whence) &&
	    !is_from_rebase(whence) && renew_authorship)
		die(_("--reset-author can be used only with -C, -c or --amend."));
	যদি (use_message) {
		use_message_বাফার = read_commit_message(use_message);
		যদি (!renew_authorship) {
			author_message = use_message;
			author_message_বাফার = use_message_বাফার;
		}
	}
	যদি ((is_from_cherry_pick(whence) || whence == FROM_REBASE_PICK) &&
	    !renew_authorship) {
		author_message = "CHERRY_PICK_HEAD";
		author_message_বাফার = read_commit_message(author_message);
	}

	যদি (patch_interactive)
		interactive = 1;

	die_for_incompatible_opt4(also, "-i/--include",
				  only, "-o/--only",
				  all, "-a/--all",
				  interactive, "--interactive/-p/--patch");
	যদি (fixup_message) {
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
		অক্ষর *p = fixup_message;
		যখন (isalpha(*p))
			p++;
		যদি (p > fixup_message && *p == ':') {
			*p = '\0';
			fixup_commit = p + 1;
			যদি (!strcmp("amend", fixup_message) ||
			    !strcmp("reword", fixup_message)) {
				fixup_prefix = "amend";
				allow_empty = 1;
				যদি (*fixup_message == 'r') {
					check_fixup_reword_options(argc, argv);
					only = 1;
				}
			} অন্যথা {
				die(_("unknown option: --fixup=%s:%s"), fixup_message, fixup_commit);
			}
		} অন্যথা {
			fixup_commit = fixup_message;
			fixup_prefix = "fixup";
			use_editor = 0;
		}
	}

	যদি (0 <= edit_flag)
		use_editor = edit_flag;

	handle_untracked_files_arg(s);

	যদি (all && argc > 0)
		die(_("paths '%s ...' with -a does not make sense"),
		    argv[0]);

	যদি (status_format != STATUS_FORMAT_NONE)
		dry_run = 1;

	ফেরত argc;
}

স্থির পূর্ণসংখ্যা dry_run_commit(ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix,
			  ধ্রুবক কাঠামো commit *current_শীর্ষ, কাঠামো wt_status *s)
{
	পূর্ণসংখ্যা committable;
	ধ্রুবক অক্ষর *সূচক_file;

	সূচক_file = prepare_সূচক(argv, prefix, current_শীর্ষ, 1);
	committable = run_status(stdout, সূচক_file, prefix, 0, s);
	rollback_সূচক_files();

	ফেরত committable ? 0 : 1;
}

define_list_config_array_extra(color_status_slots, {"added"});

স্থির পূর্ণসংখ্যা parse_status_slot(ধ্রুবক অক্ষর *slot)
{
	যদি (!strcasecmp(slot, "added"))
		ফেরত WT_STATUS_UPDATED;

	ফেরত LOOKUP_CONFIG(color_status_slots, slot);
}

স্থির পূর্ণসংখ্যা git_status_config(ধ্রুবক অক্ষর *k, ধ্রুবক অক্ষর *v,
			     ধ্রুবক কাঠামো config_context *ctx, শূন্য *cb)
{
	কাঠামো wt_status *s = cb;
	ধ্রুবক অক্ষর *slot_name;

	যদি (starts_with(k, "column."))
		ফেরত git_column_config(k, v, "status", &s->colopts);
	যদি (!strcmp(k, "status.submodulesummary")) {
		পূর্ণসংখ্যা is_bool;
		s->submodule_summary = git_config_bool_or_int(k, v, ctx->kvi,
							      &is_bool);
		যদি (is_bool && s->submodule_summary)
			s->submodule_summary = -1;
		ফেরত 0;
	}
	যদি (!strcmp(k, "status.short")) {
		যদি (git_config_bool(k, v))
			status_deferred_config.status_format = STATUS_FORMAT_SHORT;
		অন্যথা
			status_deferred_config.status_format = STATUS_FORMAT_NONE;
		ফেরত 0;
	}
	যদি (!strcmp(k, "status.branch")) {
		status_deferred_config.show_branch = git_config_bool(k, v);
		ফেরত 0;
	}
	যদি (!strcmp(k, "status.aheadbehind")) {
		status_deferred_config.ahead_behind = git_config_bool(k, v);
		ফেরত 0;
	}
	যদি (!strcmp(k, "status.showstash")) {
		s->show_stash = git_config_bool(k, v);
		ফেরত 0;
	}
	যদি (!strcmp(k, "status.color") || !strcmp(k, "color.status")) {
		s->use_color = git_config_colorbool(k, v);
		ফেরত 0;
	}
	যদি (!strcmp(k, "status.displaycommentprefix")) {
		s->display_comment_prefix = git_config_bool(k, v);
		ফেরত 0;
	}
	যদি (skip_prefix(k, "status.color.", &slot_name) ||
	    skip_prefix(k, "color.status.", &slot_name)) {
		পূর্ণসংখ্যা slot = parse_status_slot(slot_name);
		যদি (slot < 0)
			ফেরত 0;
		যদি (!v)
			ফেরত config_error_nonbool(k);
		ফেরত color_parse(v, s->color_palette[slot]);
	}
	যদি (!strcmp(k, "status.relativepaths")) {
		s->relative_paths = git_config_bool(k, v);
		ফেরত 0;
	}
	যদি (!strcmp(k, "status.showuntrackedfiles")) {
		গণনা untracked_status_type u;

		u = parse_untracked_setting_name(v);
		যদি (u == SHOW_UNTRACKED_FILES_ERROR)
			ফেরত error(_("Invalid untracked files mode '%s'"), v);
		s->show_untracked_files = u;
		ফেরত 0;
	}
	যদি (!strcmp(k, "diff.renamelimit")) {
		যদি (s->rename_limit == -1)
			s->rename_limit = git_config_int(k, v, ctx->kvi);
		ফেরত 0;
	}
	যদি (!strcmp(k, "status.renamelimit")) {
		s->rename_limit = git_config_int(k, v, ctx->kvi);
		ফেরত 0;
	}
	যদি (!strcmp(k, "diff.renames")) {
		যদি (s->detect_rename == -1)
			s->detect_rename = git_config_rename(k, v);
		ফেরত 0;
	}
	যদি (!strcmp(k, "status.renames")) {
		s->detect_rename = git_config_rename(k, v);
		ফেরত 0;
	}
	ফেরত git_diff_ui_config(k, v, ctx, ফাঁকা);
}

পূর্ণসংখ্যা cmd_status(পূর্ণসংখ্যা argc,
ধ্রুবক অক্ষর **argv,
ধ্রুবক অক্ষর *prefix,
কাঠামো repository *repo UNUSED)
{
	স্থির পূর্ণসংখ্যা no_renames = -1;
	স্থির ধ্রুবক অক্ষর *rename_score_arg = (ধ্রুবক অক্ষর *)-1;
	স্থির কাঠামো wt_status s;
	unsigned পূর্ণসংখ্যা progress_flag = 0;
	পূর্ণসংখ্যা fd;
	কাঠামো object_id oid;
	স্থির কাঠামো option builtin_status_options[] = {
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT_SET_INT('s', "short", &status_format,
			    N_("show status concisely"), STATUS_FORMAT_SHORT),
		OPT_BOOL('b', "branch", &s.show_branch,
			 N_("show branch information")),
		OPT_BOOL(0, "show-stash", &s.show_stash,
			 N_("show stash information")),
		OPT_BOOL(0, "ahead-behind", &s.ahead_behind_flags,
			 N_("compute full ahead/behind values")),
		OPT_CALLBACK_F(0, "porcelain", &status_format,
		  N_("version"), N_("machine-readable output"),
		  PARSE_OPT_OPTARG, opt_parse_porcelain),
		OPT_SET_INT(0, "long", &status_format,
			    N_("show status in long format (default)"),
			    STATUS_FORMAT_LONG),
		OPT_BOOL('z', "null", &s.null_termination,
			 N_("terminate entries with NUL")),
		{
			.type = OPTION_STRING,
			.short_name = 'u',
			.long_name = "untracked-files",
			.মান = &untracked_files_arg,
			.argh = N_("mode"),
			.help = N_("show untracked files, optional modes: all, normal, no. (Default: all)"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t)"all",
		},
		{
			.type = OPTION_STRING,
			.long_name = "ignored",
			.মান = &ignored_arg,
			.argh = N_("mode"),
			.help = N_("show ignored files, optional modes: traditional, matching, no. (Default: traditional)"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t)"traditional",
		},
		{
			.type = OPTION_STRING,
			.long_name = "ignore-submodules",
			.মান = &ignore_submodule_arg,
			.argh = N_("when"),
			.help = N_("ignore changes to submodules, optional when: all, dirty, untracked. (Default: all)"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t)"all",
		},
		OPT_COLUMN(0, "column", &s.colopts, N_("list untracked files in columns")),
		OPT_BOOL(0, "no-renames", &no_renames, N_("do not detect renames")),
		OPT_CALLBACK_F('M', "find-renames", &rename_score_arg,
		  N_("n"), N_("detect renames, optionally set similarity index"),
		  PARSE_OPT_OPTARG | PARSE_OPT_NONEG, opt_parse_rename_score),
		OPT_END(),
	};

	show_usage_with_options_if_asked(argc, argv,
					 builtin_status_usage, builtin_status_options);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_সূচক = 0;

	status_init_config(&s, git_status_config);
	argc = parse_options(argc, argv, prefix,
			     builtin_status_options,
			     builtin_status_usage, 0);
	finalize_colopts(&s.colopts, -1);
	finalize_deferred_config(&s);

	handle_untracked_files_arg(&s);
	handle_ignored_arg(&s);

	যদি (s.show_ignored_mode == SHOW_MATCHING_IGNORED &&
	    s.show_untracked_files == SHOW_NO_UNTRACKED_FILES)
		die(_("Unsupported combination of ignored and untracked-files arguments"));

	parse_pathspec(&s.pathspec, 0,
		       PATHSPEC_PREFER_FULL,
		       prefix, argv);

	যদি (status_format != STATUS_FORMAT_PORCELAIN &&
	    status_format != STATUS_FORMAT_PORCELAIN_V2)
		progress_flag = REFRESH_PROGRESS;
	repo_read_সূচক(the_repository);
	refresh_সূচক(the_repository->সূচক,
		      REFRESH_QUIET|REFRESH_UNMERGED|progress_flag,
		      &s.pathspec, ফাঁকা, ফাঁকা);

	যদি (use_optional_locks())
		fd = repo_hold_locked_সূচক(the_repository, &সূচক_lock, 0);
	অন্যথা
		fd = -1;

	s.is_initial = repo_get_oid(the_repository, s.reference, &oid) ? 1 : 0;
	যদি (!s.is_initial)
		oidcpy(&s.oid_commit, &oid);

	s.ignore_submodule_arg = ignore_submodule_arg;
	s.status_format = status_format;
	s.verbose = verbose;
	যদি (no_renames != -1)
		s.detect_rename = !no_renames;
	যদি ((intptr_t)rename_score_arg != -1) {
		যদি (s.detect_rename < DIFF_DETECT_RENAME)
			s.detect_rename = DIFF_DETECT_RENAME;
		যদি (rename_score_arg)
			s.rename_score = parse_rename_score(&rename_score_arg);
	}

	wt_status_collect(&s);

	যদি (0 <= fd)
		repo_update_সূচক_if_able(the_repository, &সূচক_lock);

	যদি (s.relative_paths)
		s.prefix = prefix;

	wt_status_print(&s);
	wt_status_collect_free_buffers(&s);

	ফেরত 0;
}

স্থির পূর্ণসংখ্যা git_commit_config(ধ্রুবক অক্ষর *k, ধ্রুবক অক্ষর *v,
			     ধ্রুবক কাঠামো config_context *ctx, শূন্য *cb)
{
	কাঠামো wt_status *s = cb;

	যদি (!strcmp(k, "commit.template"))
		ফেরত git_config_pathname(&template_file, k, v);
	যদি (!strcmp(k, "commit.status")) {
		include_status = git_config_bool(k, v);
		ফেরত 0;
	}
	যদি (!strcmp(k, "commit.cleanup")) {
		FREE_AND_NULL(cleanup_config);
		ফেরত git_config_string(&cleanup_config, k, v);
	}
	যদি (!strcmp(k, "commit.gpgsign")) {
		sign_commit = git_config_bool(k, v) ? "" : ফাঁকা;
		ফেরত 0;
	}
	যদি (!strcmp(k, "commit.verbose")) {
		পূর্ণসংখ্যা is_bool;
		config_commit_verbose = git_config_bool_or_int(k, v, ctx->kvi,
							       &is_bool);
		ফেরত 0;
	}

	ফেরত git_status_config(k, v, ctx, s);
}

পূর্ণসংখ্যা cmd_commit(পূর্ণসংখ্যা argc,
	       ধ্রুবক অক্ষর **argv,
	       ধ্রুবক অক্ষর *prefix,
	       কাঠামো repository *repo UNUSED)
{
	স্থির কাঠামো wt_status s;
	স্থির ধ্রুবক অক্ষর *cleanup_arg = ফাঁকা;
	স্থির কাঠামো option builtin_commit_options[] = {
		OPT__QUIET(&quiet, N_("suppress summary after successful commit")),
		OPT__VERBOSE(&verbose, N_("show diff in commit message template")),

		OPT_GROUP(N_("Commit message options")),
		OPT_FILENAME('F', "file", &logfile, N_("read message from file")),
		OPT_STRING(0, "author", &force_author, N_("author"), N_("override author for commit")),
		OPT_STRING(0, "date", &force_date, N_("date"), N_("override date for commit")),
		OPT_CALLBACK('m', "message", &message, N_("message"), N_("commit message"), opt_parse_m),
		OPT_STRING('c', "reedit-message", &edit_message, N_("commit"), N_("reuse and edit message from specified commit")),
		OPT_STRING('C', "reuse-message", &use_message, N_("commit"), N_("reuse message from specified commit")),
		/*
		 * TRANSLATORS: Leave "[(amend|reword):]" as-is,
		 * and only translate <commit>.
		 */
		OPT_STRING(0, "fixup", &fixup_message, N_("[(amend|reword):]commit"), N_("use autosquash formatted message to fixup or amend/reword specified commit")),
		OPT_STRING(0, "squash", &squash_message, N_("commit"), N_("use autosquash formatted message to squash specified commit")),
		OPT_BOOL(0, "reset-author", &renew_authorship, N_("the commit is authored by me now (used with -C/-c/--amend)")),
		OPT_STRVEC(0, "trailer", &trailer_args, N_("trailer"),
			   N_("add custom trailer(s)")),
		OPT_BOOL('s', "signoff", &signoff, N_("add a Signed-off-by trailer")),
		OPT_FILENAME('t', "template", &template_file, N_("use specified template file")),
		OPT_BOOL('e', "edit", &edit_flag, N_("force edit of commit")),
		OPT_CLEANUP(&cleanup_arg),
		OPT_BOOL(0, "status", &include_status, N_("include status in commit message template")),
		{
			.type = OPTION_STRING,
			.short_name = 'S',
			.long_name = "gpg-sign",
			.মান = &sign_commit,
			.argh = N_("key-id"),
			.help = N_("GPG sign commit"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "",
		},
		/* end commit message options */

		OPT_GROUP(N_("Commit contents options")),
		OPT_BOOL('a', "all", &all, N_("commit all changed files")),
		OPT_BOOL('i', "include", &also, N_("add specified files to index for commit")),
		OPT_BOOL(0, "interactive", &interactive, N_("interactively add files")),
		OPT_BOOL('p', "patch", &patch_interactive, N_("interactively add changes")),
		OPT_DIFF_UNIFIED(&interactive_opts.context),
		OPT_DIFF_INTERHUNK_CONTEXT(&interactive_opts.interhunkcontext),
		OPT_BOOL('o', "only", &only, N_("commit only specified files")),
		OPT_BOOL('n', "no-verify", &no_verify, N_("bypass pre-commit and commit-msg hooks")),
		OPT_BOOL(0, "dry-run", &dry_run, N_("show what would be committed")),
		OPT_SET_INT(0, "short", &status_format, N_("show status concisely"),
			    STATUS_FORMAT_SHORT),
		OPT_BOOL(0, "branch", &s.show_branch, N_("show branch information")),
		OPT_BOOL(0, "ahead-behind", &s.ahead_behind_flags,
			 N_("compute full ahead/behind values")),
		OPT_SET_INT(0, "porcelain", &status_format,
			    N_("machine-readable output"), STATUS_FORMAT_PORCELAIN),
		OPT_SET_INT(0, "long", &status_format,
			    N_("show status in long format (default)"),
			    STATUS_FORMAT_LONG),
		OPT_BOOL('z', "null", &s.null_termination,
			 N_("terminate entries with NUL")),
		OPT_BOOL(0, "amend", &amend, N_("amend previous commit")),
		OPT_BOOL(0, "no-post-rewrite", &no_post_rewrite, N_("bypass post-rewrite hook")),
		{
			.type = OPTION_STRING,
			.short_name = 'u',
			.long_name = "untracked-files",
			.মান = &untracked_files_arg,
			.argh = N_("mode"),
			.help = N_("show untracked files, optional modes: all, normal, no. (Default: all)"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t)"all",
		},
		OPT_PATHSPEC_FROM_FILE(&pathspec_from_file),
		OPT_PATHSPEC_FILE_NUL(&pathspec_file_nul),
		/* end commit contents options */

		OPT_HIDDEN_BOOL(0, "allow-empty", &allow_empty,
				N_("ok to record an empty change")),
		OPT_HIDDEN_BOOL(0, "allow-empty-message", &allow_empty_message,
				N_("ok to record a change with an empty message")),

		OPT_END()
	};

	কাঠামো strbuf sb = STRBUF_INIT;
	কাঠামো strbuf author_ident = STRBUF_INIT;
	ধ্রুবক অক্ষর *সূচক_file, *reflog_বার্তা;
	কাঠামো object_id oid;
	কাঠামো commit_list *parents = ফাঁকা;
	কাঠামো stat statbuf;
	কাঠামো commit *current_শীর্ষ = ফাঁকা;
	কাঠামো commit_extra_header *extra = ফাঁকা;
	কাঠামো strbuf err = STRBUF_INIT;
	পূর্ণসংখ্যা ret = 0;

	show_usage_with_options_if_asked(argc, argv,
					 builtin_commit_usage, builtin_commit_options);

#ifndef WITH_BREAKING_CHANGES
	warn_on_auto_comment_char = true;
#endif /* !WITH_BREAKING_CHANGES */
	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_সূচক = 0;

	status_init_config(&s, git_commit_config);
	s.commit_template = 1;
	status_format = STATUS_FORMAT_NONE; /* Ignore status.short */
	s.colopts = 0;

	যদি (repo_get_oid(the_repository, "HEAD", &oid))
		current_শীর্ষ = ফাঁকা;
	অন্যথা {
		current_শীর্ষ = lookup_commit_or_die(&oid, "HEAD");
		যদি (repo_parse_commit(the_repository, current_শীর্ষ))
			die(_("could not parse HEAD commit"));
	}
	verbose = -1; /* unspecified */
	argc = parse_and_validate_options(argc, argv, builtin_commit_options,
					  builtin_commit_usage,
					  prefix, current_শীর্ষ, &s);
	যদি (trailer_args.nr)
		trailer_config_init();

	যদি (verbose == -1)
		verbose = (config_commit_verbose < 0) ? 0 : config_commit_verbose;

	যদি (cleanup_arg) {
		মুক্ত(cleanup_config);
		cleanup_config = xstrdup(cleanup_arg);
	}
	cleanup_mode = get_cleanup_mode(cleanup_config, use_editor);

	যদি (dry_run)
		ফেরত dry_run_commit(argv, prefix, current_শীর্ষ, &s);
	সূচক_file = prepare_সূচক(argv, prefix, current_শীর্ষ, 0);

	/* Set up everything for writing the commit object.  This includes
	   running hooks, writing the trees, and interacting with the user.  */
	যদি (!prepare_to_commit(সূচক_file, prefix,
			       current_শীর্ষ, &s, &author_ident)) {
		ret = 1;
		rollback_সূচক_files();
		যাও cleanup;
	}

	/* Determine parents */
	reflog_বার্তা = getenv("GIT_REFLOG_ACTION");
	যদি (!current_শীর্ষ) {
		যদি (!reflog_বার্তা)
			reflog_বার্তা = "commit (initial)";
	} অন্যথা যদি (amend) {
		যদি (!reflog_বার্তা)
			reflog_বার্তা = "commit (amend)";
		parents = commit_list_copy(current_শীর্ষ->parents);
	} অন্যথা যদি (whence == FROM_MERGE) {
		কাঠামো strbuf m = STRBUF_INIT;
		FILE *fp;
		পূর্ণসংখ্যা allow_fast_forward = 1;
		কাঠামো commit_list **pptr = &parents;

		যদি (!reflog_বার্তা)
			reflog_বার্তা = "commit (merge)";
		pptr = commit_list_append(current_শীর্ষ, pptr);
		fp = xfopen(git_path_merge_শীর্ষ(the_repository), "r");
		যখন (strbuf_getline_lf(&m, fp) != EOF) {
			কাঠামো commit *parent;

			parent = get_merge_parent(m.buf);
			যদি (!parent)
				die(_("Corrupt MERGE_HEAD file (%s)"), m.buf);
			pptr = commit_list_append(parent, pptr);
		}
		fclose(fp);
		strbuf_release(&m);
		যদি (!stat(git_path_merge_mode(the_repository), &statbuf)) {
			যদি (strbuf_read_file(&sb, git_path_merge_mode(the_repository), 0) < 0)
				die_errno(_("could not read MERGE_MODE"));
			যদি (!strcmp(sb.buf, "no-ff"))
				allow_fast_forward = 0;
		}
		যদি (allow_fast_forward)
			reduce_heads_replace(&parents);
	} অন্যথা {
		যদি (!reflog_বার্তা)
			reflog_বার্তা = is_from_cherry_pick(whence)
					? "commit (cherry-pick)"
					: is_from_rebase(whence)
					? "commit (rebase)"
					: "commit";
		commit_list_ঢোকাও(current_শীর্ষ, &parents);
	}

	/* Finally, get the commit message */
	strbuf_reset(&sb);
	যদি (strbuf_read_file(&sb, git_path_commit_editmsg(), 0) < 0) {
		পূর্ণসংখ্যা saved_errno = errno;
		rollback_সূচক_files();
		die(_("could not read commit message: %s"), strerror(saved_errno));
	}

	cleanup_message(&sb, cleanup_mode, verbose);

	যদি (message_is_empty(&sb, cleanup_mode) && !allow_empty_message) {
		rollback_সূচক_files();
		fprintf(stderr, _("Aborting commit due to empty commit message.\n"));
		exit(1);
	}
	যদি (template_untouched(&sb, template_file, cleanup_mode) && !allow_empty_message) {
		rollback_সূচক_files();
		fprintf(stderr, _("Aborting commit; you did not edit the message.\n"));
		exit(1);
	}

	যদি (fixup_message && starts_with(sb.buf, "amend! ") &&
	    !allow_empty_message) {
		কাঠামো strbuf body = STRBUF_INIT;
		আকার_t len = commit_subject_দৈর্ঘ্য(sb.buf);
		strbuf_addstr(&body, sb.buf + len);
		যদি (message_is_empty(&body, cleanup_mode)) {
			rollback_সূচক_files();
			fprintf(stderr, _("Aborting commit due to empty commit message body.\n"));
			exit(1);
		}
		strbuf_release(&body);
	}

	যদি (amend) {
		ধ্রুবক অক্ষর *exclude_gpgsig[3] = { "gpgsig", "gpgsig-sha256", ফাঁকা };
		extra = read_commit_extra_headers(current_শীর্ষ, exclude_gpgsig);
	} অন্যথা {
		কাঠামো commit_extra_header **লেজ = &extra;
		append_merge_tag_headers(parents, &লেজ);
	}

	যদি (commit_বৃক্ষ_extended(sb.buf, sb.len, &the_repository->সূচক->cache_বৃক্ষ->oid,
				 parents, &oid, author_ident.buf, ফাঁকা,
				 sign_commit, extra)) {
		rollback_সূচক_files();
		die(_("failed to write commit object"));
	}

	যদি (update_শীর্ষ_with_reflog(current_শীর্ষ, &oid, reflog_বার্তা, &sb,
				    &err)) {
		rollback_সূচক_files();
		die("%s", err.buf);
	}

	sequencer_post_commit_cleanup(the_repository, 0);
	unlink(git_path_merge_শীর্ষ(the_repository));
	unlink(git_path_merge_বার্তা(the_repository));
	unlink(git_path_merge_mode(the_repository));
	unlink(git_path_squash_বার্তা(the_repository));

	যদি (commit_সূচক_files())
		die(_("repository has been updated, but unable to write\n"
		      "new index file. Check that disk is not full and quota is\n"
		      "not exceeded, and then \"git restore --staged :/\" to recover."));

	git_test_write_commit_graph_or_die(the_repository);

	repo_rerere(the_repository, 0);
	run_auto_maintenance(the_repository, quiet);
	run_commit_hook(use_editor, repo_get_সূচক_file(the_repository),
			ফাঁকা, "post-commit", ফাঁকা);
	যদি (amend && !no_post_rewrite) {
		commit_post_rewrite(the_repository, current_শীর্ষ, &oid);
	}
	যদি (!quiet) {
		unsigned পূর্ণসংখ্যা flags = 0;

		যদি (!current_শীর্ষ)
			flags |= SUMMARY_INITIAL_COMMIT;
		যদি (author_date_is_interesting())
			flags |= SUMMARY_SHOW_AUTHOR_DATE;
		print_commit_summary(the_repository, prefix,
				     &oid, flags);
	}

	apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
			    ফাঁকা, ফাঁকা, ফাঁকা, ফাঁকা);

cleanup:
	free_commit_extra_headers(extra);
	commit_list_free(parents);
	strbuf_release(&author_ident);
	strbuf_release(&err);
	strbuf_release(&sb);
	মুক্ত(logfile);
	মুক্ত(template_file);
	ফেরত ret;
}
