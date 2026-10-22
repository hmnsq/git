/*
 * Builtin "git commit"
 *
 * Copyright (c) 2007 Kristian Høgsberg <krh@redhat.com>
 * Based on git-commit.sh by Junio C Hamano and Linus Torvalds
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "advice.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "lockfile.h"
#ಅನ್ತರ್ಭಾವ "cache-tree.h"
#ಅನ್ತರ್ಭಾವ "color.h"
#ಅನ್ತರ್ಭಾವ "dir.h"
#ಅನ್ತರ್ಭಾವ "editor.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "diff.h"
#ಅನ್ತರ್ಭಾವ "commit.h"
#ಅನ್ತರ್ಭಾವ "add-interactive.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "revision.h"
#ಅನ್ತರ್ಭಾವ "wt-status.h"
#ಅನ್ತರ್ಭಾವ "run-command.h"
#ಅನ್ತರ್ಭಾವ "strbuf.h"
#ಅನ್ತರ್ಭಾವ "object-name.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "path.h"
#ಅನ್ತರ್ಭಾವ "preload-index.h"
#ಅನ್ತರ್ಭಾವ "read-cache.h"
#ಅನ್ತರ್ಭಾವ "repository.h"
#ಅನ್ತರ್ಭಾವ "string-list.h"
#ಅನ್ತರ್ಭಾವ "rerere.h"
#ಅನ್ತರ್ಭಾವ "unpack-trees.h"
#ಅನ್ತರ್ಭಾವ "column.h"
#ಅನ್ತರ್ಭಾವ "sequencer.h"
#ಅನ್ತರ್ಭಾವ "sparse-index.h"
#ಅನ್ತರ್ಭಾವ "mailmap.h"
#ಅನ್ತರ್ಭಾವ "help.h"
#ಅನ್ತರ್ಭಾವ "commit-reach.h"
#ಅನ್ತರ್ಭಾವ "commit-graph.h"
#ಅನ್ತರ್ಭಾವ "pretty.h"
#ಅನ್ತರ್ಭಾವ "trailer.h"

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ builtin_commit_usage[] = {
	N_("git commit [-a | --interactive | --patch] [-s] [-v] [-u[<mode>]] [--amend]\n"
	   "           [--dry-run] [(-c | -C | --squash) <commit> | --fixup [(amend|reword):]<commit>]\n"
	   "           [-F <file> | -m <msg>] [--reset-author] [--allow-empty]\n"
	   "           [--allow-empty-message] [--no-verify] [-e] [--author=<author>]\n"
	   "           [--date=<date>] [--cleanup=<mode>] [--[no-]status]\n"
	   "           [-i | -o] [--pathspec-from-file=<file> [--pathspec-file-nul]]\n"
	   "           [(--trailer <token>[(=|:)<value>])...] [-S[<keyid>]]\n"
	   "           [--] [<pathspec>...]"),
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ builtin_status_usage[] = {
	N_("git status [<options>] [--] [<pathspec>...]"),
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ empty_amend_advice[] =
N_("You asked to amend the most recent commit, but doing so would make\n"
"it empty. You can repeat your command with --allow-empty, or you can\n"
"remove the commit entirely with \"git reset HEAD^\".\n");

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ empty_cherry_pick_advice[] =
N_("The previous cherry-pick is now empty, possibly due to conflict resolution.\n"
"If you wish to commit it anyway, use:\n"
"\n"
"    git commit --allow-empty\n"
"\n");

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ empty_rebase_pick_advice[] =
N_("Otherwise, please use 'git rebase --skip'\n");

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ empty_cherry_pick_advice_single[] =
N_("Otherwise, please use 'git cherry-pick --skip'\n");

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ empty_cherry_pick_advice_multi[] =
N_("and then use:\n"
"\n"
"    git cherry-pick --continue\n"
"\n"
"to resume cherry-picking the remaining commits.\n"
"If you wish to skip this commit, use:\n"
"\n"
"    git cherry-pick --skip\n"
"\n");

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *color_status_slots[] = {
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

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *use_message_buffer;
ಸ್ಥಿರ ಸಂರಚನಾ lock_file index_lock; /* real index */
ಸ್ಥಿರ ಸಂರಚನಾ lock_file false_lock; /* used only for partial commits */
ಸ್ಥಿರ ಗಣನಾ {
	COMMIT_AS_IS = 1,
	COMMIT_NORMAL,
	COMMIT_PARTIAL
} commit_style;

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *force_author;
ಸ್ಥಿರ ವರ್ಣ *logfile;
ಸ್ಥಿರ ವರ್ಣ *template_file;
/*
 * The _message variables are commit names from which to take
 * the commit message and/or authorship.
 */
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *author_message, *author_message_buffer;
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *edit_message, *use_message;
ಸ್ಥಿರ ವರ್ಣ *fixup_message, *fixup_commit, *squash_message;
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *fixup_prefix;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ all, also, interactive, patch_interactive, only, amend, signoff;
ಸ್ಥಿರ ಸಂರಚನಾ interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ edit_flag = -1; /* unspecified */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ quiet, verbose, no_verify, allow_empty, dry_run, renew_authorship;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ config_commit_verbose = -1; /* unspecified */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ no_post_rewrite, allow_empty_message, pathspec_file_nul;
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *untracked_files_arg, *force_date, *ignore_submodule_arg, *ignored_arg;
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *sign_commit, *pathspec_from_file;
ಸ್ಥಿರ ಸಂರಚನಾ strvec trailer_args = STRVEC_INIT;

/*
 * The default commit message cleanup mode will remove the lines
 * beginning with # (shell comments) and leading and trailing
 * whitespaces (empty lines or containing only whitespaces)
 * if editor is used, and only the whitespaces if the message
 * is specified explicitly.
 */
ಸ್ಥಿರ ಗಣನಾ commit_ಸಂದೇಶ_cleanup_mode cleanup_mode;
ಸ್ಥಿರ ವರ್ಣ *cleanup_config;

ಸ್ಥಿರ ಗಣನಾ commit_whence whence;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ use_editor = 1, include_status = 1;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ have_option_m;
ಸ್ಥಿರ ಸಂರಚನಾ strbuf message = STRBUF_INIT;

ಸ್ಥಿರ ಗಣನಾ wt_status_format status_format = STATUS_FORMAT_UNSPECIFIED;

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ opt_parse_porcelain(ಧ್ರುವ ಸಂರಚನಾ option *opt, ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಗಣನಾ wt_status_format *ಮೌಲ್ಯ = (ಗಣನಾ wt_status_format *)opt->ಮೌಲ್ಯ;
	ಯದಿ (unset)
		*ಮೌಲ್ಯ = STATUS_FORMAT_NONE;
	ಅನ್ಯಥಾ ಯದಿ (!arg)
		*ಮೌಲ್ಯ = STATUS_FORMAT_PORCELAIN;
	ಅನ್ಯಥಾ ಯದಿ (!strcmp(arg, "v1") || !strcmp(arg, "1"))
		*ಮೌಲ್ಯ = STATUS_FORMAT_PORCELAIN;
	ಅನ್ಯಥಾ ಯದಿ (!strcmp(arg, "v2") || !strcmp(arg, "2"))
		*ಮೌಲ್ಯ = STATUS_FORMAT_PORCELAIN_V2;
	ಅನ್ಯಥಾ
		die("unsupported porcelain version '%s'", arg);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ opt_parse_m(ಧ್ರುವ ಸಂರಚನಾ option *opt, ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಸಂರಚನಾ strbuf *buf = opt->ಮೌಲ್ಯ;
	ಯದಿ (unset) {
		have_option_m = 0;
		strbuf_setlen(buf, 0);
	} ಅನ್ಯಥಾ {
		have_option_m = 1;
		ಯದಿ (buf->len)
			strbuf_addch(buf, '\n');
		strbuf_addstr(buf, arg);
		strbuf_complete_ಸಾಲು(buf);
	}
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ opt_parse_rename_score(ಧ್ರುವ ಸಂರಚನಾ option *opt, ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಧ್ರುವ ವರ್ಣ **ಮೌಲ್ಯ = opt->ಮೌಲ್ಯ;

	BUG_ON_OPT_NEG(unset);

	ಯದಿ (arg != ರಿಕ್ತ && *arg == '=')
		arg = arg + 1;

	*ಮೌಲ್ಯ = arg;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ determine_whence(ಸಂರಚನಾ wt_status *s)
{
	ಯದಿ (file_exists(git_path_merge_head(the_repository)))
		whence = FROM_MERGE;
	ಅನ್ಯಥಾ ಯದಿ (!sequencer_determine_whence(the_repository, &whence))
		whence = FROM_COMMIT;
	ಯದಿ (s)
		s->whence = whence;
}

ಸ್ಥಿರ ಶೂನ್ಯ status_init_config(ಸಂರಚನಾ wt_status *s, config_fn_t fn)
{
	wt_status_prepare(the_repository, s);
	init_diff_ui_defaults();
	repo_config(the_repository, fn, s);
	determine_whence(s);
	s->hints = advice_enabled(ADVICE_STATUS_HINTS); /* must come after repo_config() */
}

ಸ್ಥಿರ ಶೂನ್ಯ rollback_index_files(ಶೂನ್ಯ)
{
	ವಿಕಲ್ಪ (commit_style) {
	ಪಕ್ಷ COMMIT_AS_IS:
		ಭಙ್ಗ; /* nothing to do */
	ಪಕ್ಷ COMMIT_NORMAL:
		rollback_lock_file(&index_lock);
		ಭಙ್ಗ;
	ಪಕ್ಷ COMMIT_PARTIAL:
		rollback_lock_file(&index_lock);
		rollback_lock_file(&false_lock);
		ಭಙ್ಗ;
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ commit_index_files(ಶೂನ್ಯ)
{
	ಪೂರ್ಣಾಙ್ಕ err = 0;

	ವಿಕಲ್ಪ (commit_style) {
	ಪಕ್ಷ COMMIT_AS_IS:
		ಭಙ್ಗ; /* nothing to do */
	ಪಕ್ಷ COMMIT_NORMAL:
		err = commit_lock_file(&index_lock);
		ಭಙ್ಗ;
	ಪಕ್ಷ COMMIT_PARTIAL:
		err = commit_lock_file(&index_lock);
		rollback_lock_file(&false_lock);
		ಭಙ್ಗ;
	}

	ಪ್ರತ್ಯಾಗಮ err;
}

/*
 * Take a union of paths in the index and the named tree (typically, "HEAD"),
 * and return the paths that match the given pattern in list.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ list_paths(ಸಂರಚನಾ string_list *list, ಧ್ರುವ ವರ್ಣ *with_ವೃಕ್ಷ,
		      ಧ್ರುವ ಸಂರಚನಾ pathspec *pattern)
{
	ಪೂರ್ಣಾಙ್ಕ i, ret;
	ವರ್ಣ *m;

	ಯದಿ (!pattern->nr)
		ಪ್ರತ್ಯಾಗಮ 0;

	m = xcalloc(1, pattern->nr);

	ಯದಿ (with_ವೃಕ್ಷ) {
		ವರ್ಣ *max_prefix = common_prefix(pattern);
		overlay_ವೃಕ್ಷ_on_index(the_repository->index, with_ವೃಕ್ಷ, max_prefix);
		ಮುಕ್ತ(max_prefix);
	}

	/* TODO: audit for interaction with sparse-index. */
	ensure_full_index(the_repository->index);
	ಪ್ರತಿ (i = 0; i < the_repository->index->cache_nr; i++) {
		ಧ್ರುವ ಸಂರಚನಾ cache_entry *ce = the_repository->index->cache[i];
		ಸಂರಚನಾ string_list_item *item;

		ಯದಿ (ce->ce_flags & CE_UPDATE)
			ಅನುವರ್ತ;
		ಯದಿ (!ce_path_match(the_repository->index, ce, pattern, m))
			ಅನುವರ್ತ;
		item = string_list_ಸೇರಿಸು(list, ce->name);
		ಯದಿ (ce_skip_worktree(ce))
			item->util = item; /* better a valid pointer than a fake one */
	}

	ret = report_path_error(m, pattern);
	ಮುಕ್ತ(m);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಶೂನ್ಯ add_remove_files(ಸಂರಚನಾ string_list *list)
{
	ಪೂರ್ಣಾಙ್ಕ i;
	ಪ್ರತಿ (i = 0; i < list->nr; i++) {
		ಸಂರಚನಾ stat st;
		ಸಂರಚನಾ string_list_item *p = &(list->ವಸ್ತುಗಳು[i]);

		/* p->util is skip-worktree */
		ಯದಿ (p->util)
			ಅನುವರ್ತ;

		ಯದಿ (!lstat(p->string, &st)) {
			ಯದಿ (add_to_index(the_repository->index, p->string, &st, 0))
				die(_("updating files failed"));
		} ಅನ್ಯಥಾ
			remove_file_from_index(the_repository->index, p->string);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ create_base_index(ಧ್ರುವ ಸಂರಚನಾ commit *current_head)
{
	ಸಂರಚನಾ ವೃಕ್ಷ *ವೃಕ್ಷ;
	ಸಂರಚನಾ unpack_trees_options opts;
	ಸಂರಚನಾ ವೃಕ್ಷ_desc t;

	ಯದಿ (!current_head) {
		discard_index(the_repository->index);
		ಪ್ರತ್ಯಾಗಮ;
	}

	memset(&opts, 0, ಪರಿಮಾಣ(opts));
	opts.head_idx = 1;
	opts.index_only = 1;
	opts.merge = 1;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;

	opts.fn = oneway_merge;
	ವೃಕ್ಷ = repo_parse_ವೃಕ್ಷ_indirect(the_repository,
					&current_head->object.oid);
	ಯದಿ (!ವೃಕ್ಷ)
		die(_("failed to unpack HEAD tree object"));
	ಯದಿ (repo_parse_ವೃಕ್ಷ(the_repository, ವೃಕ್ಷ) < 0)
		exit(128);
	init_ವೃಕ್ಷ_desc(&t, &ವೃಕ್ಷ->object.oid, ವೃಕ್ಷ->buffer, ವೃಕ್ಷ->size);
	ಯದಿ (unpack_trees(1, &t, &opts))
		exit(128); /* We've already reported the error, finish dying */
}

ಸ್ಥಿರ ಶೂನ್ಯ refresh_cache_or_die(ಪೂರ್ಣಾಙ್ಕ refresh_flags)
{
	/*
	 * refresh_flags contains REFRESH_QUIET, so the only errors
	 * are for unmerged entries.
	 */
	ಯದಿ (refresh_index(the_repository->index, refresh_flags | REFRESH_IN_PORCELAIN, ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ))
		die_resolve_conflict("commit");
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *prepare_index(ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
				 ಧ್ರುವ ಸಂರಚನಾ commit *current_head, ಪೂರ್ಣಾಙ್ಕ is_status)
{
	ಸಂರಚನಾ string_list partial = STRING_LIST_INIT_DUP;
	ಸಂರಚನಾ pathspec pathspec;
	ಪೂರ್ಣಾಙ್ಕ refresh_flags = REFRESH_QUIET;
	ಧ್ರುವ ವರ್ಣ *ret;
	ವರ್ಣ *path = ರಿಕ್ತ;

	ಯದಿ (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	ಯದಿ (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	ಯದಿ (is_status)
		refresh_flags |= REFRESH_UNMERGED;
	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_FULL,
		       prefix, argv);

	ಯದಿ (pathspec_from_file) {
		ಯದಿ (interactive)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--interactive/--patch");

		ಯದಿ (all)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "-a");

		ಯದಿ (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_FULL,
				    prefix, pathspec_from_file, pathspec_file_nul);
	} ಅನ್ಯಥಾ ಯದಿ (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	ಯದಿ (!pathspec.nr && (also || (only && !allow_empty &&
	    (!amend || (fixup_message && strcmp(fixup_prefix, "amend"))))))
		die(_("No paths with --include/--only does not make sense."));

	ಯದಿ (repo_read_index_preload(the_repository, &pathspec, 0) < 0)
		die(_("index file corrupt"));

	ಯದಿ (interactive) {
		ವರ್ಣ *old_index_env = ರಿಕ್ತ, *old_repo_index_file;
		repo_hold_locked_index(the_repository, &index_lock,
				       LOCK_DIE_ON_ERROR);

		refresh_cache_or_die(refresh_flags);

		ಯದಿ (write_locked_index(the_repository->index, &index_lock, 0))
			die(_("unable to create temporary index"));

		old_repo_index_file = the_repository->index_file;
		the_repository->index_file =
			(ವರ್ಣ *)get_lock_file_path(&index_lock);
		old_index_env = xstrdup_or_null(getenv(INDEX_ENVIRONMENT));
		setenv(INDEX_ENVIRONMENT, the_repository->index_file, 1);

		ಯದಿ (interactive_add(the_repository, argv, prefix, patch_interactive, &interactive_opts) != 0)
			die(_("interactive add failed"));

		the_repository->index_file = old_repo_index_file;
		ಯದಿ (old_index_env && *old_index_env)
			setenv(INDEX_ENVIRONMENT, old_index_env, 1);
		ಅನ್ಯಥಾ
			unsetenv(INDEX_ENVIRONMENT);
		FREE_AND_NULL(old_index_env);

		discard_index(the_repository->index);
		read_index_from(the_repository->index, get_lock_file_path(&index_lock),
				repo_get_git_dir(the_repository));
		ಯದಿ (cache_ವೃಕ್ಷ_update(the_repository->index, WRITE_ವೃಕ್ಷ_SILENT) == 0) {
			ಯದಿ (reopen_lock_file(&index_lock) < 0)
				die(_("unable to write index file"));
			ಯದಿ (write_locked_index(the_repository->index, &index_lock, 0))
				die(_("unable to update temporary index"));
		} ಅನ್ಯಥಾ
			warning(_("Failed to update main cache tree"));

		commit_style = COMMIT_NORMAL;
		ret = get_lock_file_path(&index_lock);
		ಗಚ್ಛ out;
	} ಅನ್ಯಥಾ {
		ಯದಿ (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--interactive/--patch");
		ಯದಿ (interactive_opts.interhunkcontext != -1)
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
	ಯದಿ (all || (also && pathspec.nr)) {
		ವರ್ಣ *ps_matched = xcalloc(pathspec.nr, 1);
		repo_hold_locked_index(the_repository, &index_lock,
				       LOCK_DIE_ON_ERROR);
		add_files_to_cache(the_repository, also ? prefix : ರಿಕ್ತ,
				   &pathspec, ps_matched, 0, 0, 0 );
		ಯದಿ (!all && report_path_error(ps_matched, &pathspec))
			exit(128);

		refresh_cache_or_die(refresh_flags);
		cache_ವೃಕ್ಷ_update(the_repository->index, WRITE_ವೃಕ್ಷ_SILENT);
		ಯದಿ (write_locked_index(the_repository->index, &index_lock, 0))
			die(_("unable to write new index file"));
		commit_style = COMMIT_NORMAL;
		ret = get_lock_file_path(&index_lock);
		ಮುಕ್ತ(ps_matched);
		ಗಚ್ಛ out;
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
	ಯದಿ (!only && !pathspec.nr) {
		repo_hold_locked_index(the_repository, &index_lock,
				       LOCK_DIE_ON_ERROR);
		refresh_cache_or_die(refresh_flags);
		ಯದಿ (the_repository->index->cache_changed
		    || !cache_ವೃಕ್ಷ_fully_valid(the_repository->index))
			cache_ವೃಕ್ಷ_update(the_repository->index, WRITE_ವೃಕ್ಷ_SILENT);
		ಯದಿ (write_locked_index(the_repository->index, &index_lock,
				       COMMIT_LOCK | SKIP_IF_UNCHANGED))
			die(_("unable to write new index file"));
		commit_style = COMMIT_AS_IS;
		ret = repo_get_index_file(the_repository);
		ಗಚ್ಛ out;
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

	ಯದಿ (whence != FROM_COMMIT) {
		ಯದಿ (whence == FROM_MERGE)
			die(_("cannot do a partial commit during a merge."));
		ಅನ್ಯಥಾ ಯದಿ (is_from_cherry_pick(whence))
			die(_("cannot do a partial commit during a cherry-pick."));
		ಅನ್ಯಥಾ ಯದಿ (is_from_rebase(whence))
			die(_("cannot do a partial commit during a rebase."));
	}

	ಯದಿ (list_paths(&partial, !current_head ? ರಿಕ್ತ : "HEAD", &pathspec))
		exit(1);

	discard_index(the_repository->index);
	ಯದಿ (repo_read_index(the_repository) < 0)
		die(_("cannot read the index"));

	repo_hold_locked_index(the_repository, &index_lock, LOCK_DIE_ON_ERROR);
	add_remove_files(&partial);
	refresh_index(the_repository->index, REFRESH_QUIET, ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ);
	cache_ವೃಕ್ಷ_update(the_repository->index, WRITE_ವೃಕ್ಷ_SILENT);
	ಯದಿ (write_locked_index(the_repository->index, &index_lock, 0))
		die(_("unable to write new index file"));

	path = repo_git_path(the_repository, "next-index-%"PRIuMAX,
			     (uintmax_t) getpid());
	hold_lock_file_for_update(&false_lock, path,
				  LOCK_DIE_ON_ERROR);

	create_base_index(current_head);
	add_remove_files(&partial);
	refresh_index(the_repository->index, REFRESH_QUIET, ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ);

	ಯದಿ (write_locked_index(the_repository->index, &false_lock, 0))
		die(_("unable to write temporary index file"));

	discard_index(the_repository->index);
	ret = get_lock_file_path(&false_lock);
	read_index_from(the_repository->index, ret, repo_get_git_dir(the_repository));
out:
	string_list_clear(&partial, 0);
	clear_pathspec(&pathspec);
	ಮುಕ್ತ(path);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ run_status(FILE *fp, ಧ್ರುವ ವರ್ಣ *index_file, ಧ್ರುವ ವರ್ಣ *prefix, ಪೂರ್ಣಾಙ್ಕ nowarn,
		      ಸಂರಚನಾ wt_status *s)
{
	ಸಂರಚನಾ object_id oid;

	ಯದಿ (s->relative_paths)
		s->prefix = prefix;

	ಯದಿ (amend) {
		s->amend = 1;
		s->reference = "HEAD^1";
	}
	s->verbose = verbose;
	s->index_file = index_file;
	s->fp = fp;
	s->nowarn = nowarn;
	s->is_initial = repo_get_oid(the_repository, s->reference, &oid) ? 1 : 0;
	ಯದಿ (!s->is_initial)
		oidcpy(&s->oid_commit, &oid);
	s->status_format = status_format;
	s->ignore_submodule_arg = ignore_submodule_arg;

	wt_status_collect(s);
	wt_status_print(s);
	wt_status_collect_free_buffers(s);

	ಪ್ರತ್ಯಾಗಮ s->committable;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ is_a_merge(ಧ್ರುವ ಸಂರಚನಾ commit *current_head)
{
	ಪ್ರತ್ಯಾಗಮ !!(current_head->parents && current_head->parents->next);
}

ಸ್ಥಿರ ಶೂನ್ಯ assert_split_ident(ಸಂರಚನಾ ident_split *id, ಧ್ರುವ ಸಂರಚನಾ strbuf *buf)
{
	ಯದಿ (split_ident_ಸಾಲು(id, buf->buf, buf->len) || !id->date_begin)
		BUG("unable to parse our own ident: %s", buf->buf);
}

ಸ್ಥಿರ ಶೂನ್ಯ export_one(ಧ್ರುವ ವರ್ಣ *var, ಧ್ರುವ ವರ್ಣ *s, ಧ್ರುವ ವರ್ಣ *e, ಪೂರ್ಣಾಙ್ಕ hack)
{
	ಸಂರಚನಾ strbuf buf = STRBUF_INIT;
	ಯದಿ (hack)
		strbuf_addch(&buf, hack);
	strbuf_add(&buf, s, e - s);
	setenv(var, buf.buf, 1);
	strbuf_release(&buf);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ parse_force_date(ಧ್ರುವ ವರ್ಣ *in, ಸಂರಚನಾ strbuf *out)
{
	strbuf_addch(out, '@');

	ಯದಿ (parse_date(in, out) < 0) {
		ಪೂರ್ಣಾಙ್ಕ errors = 0;
		unsigned long t = approxidate_careful(in, &errors);
		ಯದಿ (errors)
			ಪ್ರತ್ಯಾಗಮ -1;
		strbuf_addf(out, "%lu", t);
	}

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ set_ident_var(ವರ್ಣ **buf, ವರ್ಣ *val)
{
	ಮುಕ್ತ(*buf);
	*buf = val;
}

ಸ್ಥಿರ ಶೂನ್ಯ determine_author_info(ಸಂರಚನಾ strbuf *author_ident)
{
	ವರ್ಣ *name, *email, *date;
	ಸಂರಚನಾ ident_split author;

	name = xstrdup_or_null(getenv("GIT_AUTHOR_NAME"));
	email = xstrdup_or_null(getenv("GIT_AUTHOR_EMAIL"));
	date = xstrdup_or_null(getenv("GIT_AUTHOR_DATE"));

	ಯದಿ (author_message) {
		ಸಂರಚನಾ ident_split ident;
		size_t len;
		ಧ್ರುವ ವರ್ಣ *a;

		a = find_commit_header(author_message_buffer, "author", &len);
		ಯದಿ (!a)
			die(_("commit '%s' lacks author header"), author_message);
		ಯದಿ (split_ident_ಸಾಲು(&ident, a, len) < 0)
			die(_("commit '%s' has malformed author line"), author_message);

		set_ident_var(&name, xmemdupz(ident.name_begin, ident.name_end - ident.name_begin));
		set_ident_var(&email, xmemdupz(ident.mail_begin, ident.mail_end - ident.mail_begin));

		ಯದಿ (ident.date_begin) {
			ಸಂರಚನಾ strbuf date_buf = STRBUF_INIT;
			strbuf_addch(&date_buf, '@');
			strbuf_add(&date_buf, ident.date_begin, ident.date_end - ident.date_begin);
			strbuf_addch(&date_buf, ' ');
			strbuf_add(&date_buf, ident.tz_begin, ident.tz_end - ident.tz_begin);
			set_ident_var(&date, strbuf_detach(&date_buf, ರಿಕ್ತ));
		}
	}

	ಯದಿ (force_author) {
		ಸಂರಚನಾ ident_split ident;

		ಯದಿ (split_ident_ಸಾಲು(&ident, force_author, strlen(force_author)) < 0)
			die(_("malformed --author parameter"));
		set_ident_var(&name, xmemdupz(ident.name_begin, ident.name_end - ident.name_begin));
		set_ident_var(&email, xmemdupz(ident.mail_begin, ident.mail_end - ident.mail_begin));
	}

	ಯದಿ (force_date) {
		ಸಂರಚನಾ strbuf date_buf = STRBUF_INIT;
		ಯದಿ (parse_force_date(force_date, &date_buf))
			die(_("invalid date format: %s"), force_date);
		set_ident_var(&date, strbuf_detach(&date_buf, ರಿಕ್ತ));
	}

	strbuf_addstr(author_ident, fmt_ident(name, email, WANT_AUTHOR_IDENT, date,
				IDENT_STRICT));
	assert_split_ident(&author, author_ident);
	export_one("GIT_AUTHOR_NAME", author.name_begin, author.name_end, 0);
	export_one("GIT_AUTHOR_EMAIL", author.mail_begin, author.mail_end, 0);
	export_one("GIT_AUTHOR_DATE", author.date_begin, author.tz_end, '@');
	ಮುಕ್ತ(name);
	ಮುಕ್ತ(email);
	ಮುಕ್ತ(date);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ author_date_is_interesting(ಶೂನ್ಯ)
{
	ಪ್ರತ್ಯಾಗಮ author_message || force_date;
}

#ifndef WITH_BREAKING_CHANGES
ಸ್ಥಿರ ಶೂನ್ಯ adjust_comment_ಸಾಲು_char(ಧ್ರುವ ಸಂರಚನಾ strbuf *sb)
{
	ವರ್ಣ candidates[] = "#;@!$%^&|:";
	ವರ್ಣ *candidate;
	ಧ್ರುವ ವರ್ಣ *p;
	size_t cutoff;

	/* Ignore comment chars in trailing comments (e.g., Conflicts:) */
	cutoff = sb->len - ignored_log_message_bytes(sb->buf, sb->len);

	ಯದಿ (!memchr(sb->buf, candidates[0], sb->len)) {
		ಮುಕ್ತ(comment_ಸಾಲು_str_to_free);
		comment_ಸಾಲು_str = comment_ಸಾಲು_str_to_free =
			xstrfmt("%c", candidates[0]);
		ಪ್ರತ್ಯಾಗಮ;
	}

	p = sb->buf;
	candidate = strchr(candidates, *p);
	ಯದಿ (candidate)
		*candidate = ' ';
	ಪ್ರತಿ (p = sb->buf; p + 1 < sb->buf + cutoff; p++) {
		ಯದಿ ((p[0] == '\n' || p[0] == '\r') && p[1]) {
			candidate = strchr(candidates, p[1]);
			ಯದಿ (candidate)
				*candidate = ' ';
		}
	}

	ಪ್ರತಿ (p = candidates; *p == ' '; p++)
		;
	ಯದಿ (!*p)
		die(_("unable to select a comment character that is not used\n"
		      "in the current commit message"));
	ಮುಕ್ತ(comment_ಸಾಲು_str_to_free);
	comment_ಸಾಲು_str = comment_ಸಾಲು_str_to_free = xstrfmt("%c", *p);
}
#endif /* !WITH_BREAKING_CHANGES */

ಸ್ಥಿರ ಶೂನ್ಯ prepare_amend_commit(ಸಂರಚನಾ commit *commit, ಸಂರಚನಾ strbuf *sb,
				ಸಂರಚನಾ pretty_print_context *ctx)
{
	ಧ್ರುವ ವರ್ಣ *buffer, *subject, *fmt;

	buffer = repo_get_commit_buffer(the_repository, commit, ರಿಕ್ತ);
	find_commit_subject(buffer, &subject);
	/*
	 * If we amend the 'amend!' commit then we don't want to
	 * duplicate the subject line.
	 */
	fmt = starts_with(subject, "amend!") ? "%b" : "%B";
	repo_format_commit_message(the_repository, commit, fmt, sb, ctx);
	repo_unuse_commit_buffer(the_repository, commit, buffer);
}

ಸ್ಥಿರ ಶೂನ್ಯ change_data_free(ಶೂನ್ಯ *util, ಧ್ರುವ ವರ್ಣ *str UNUSED)
{
	ಸಂರಚನಾ wt_status_change_data *d = util;
	ಮುಕ್ತ(d->rename_source);
	ಮುಕ್ತ(d);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ prepare_to_commit(ಧ್ರುವ ವರ್ಣ *index_file, ಧ್ರುವ ವರ್ಣ *prefix,
			     ಸಂರಚನಾ commit *current_head,
			     ಸಂರಚನಾ wt_status *s,
			     ಸಂರಚನಾ strbuf *author_ident)
{
	ಸಂರಚನಾ stat statbuf;
	ಸಂರಚನಾ strbuf committer_ident = STRBUF_INIT;
	ಪೂರ್ಣಾಙ್ಕ committable;
	ಸಂರಚನಾ strbuf sb = STRBUF_INIT;
	ಧ್ರುವ ವರ್ಣ *hook_arg1 = ರಿಕ್ತ;
	ಧ್ರುವ ವರ್ಣ *hook_arg2 = ರಿಕ್ತ;
	ಪೂರ್ಣಾಙ್ಕ clean_message_contents = (cleanup_mode != COMMIT_ಸಂದೇಶ_CLEANUP_NONE);
	ಪೂರ್ಣಾಙ್ಕ old_display_comment_prefix;
	ಪೂರ್ಣಾಙ್ಕ invoked_hook;

	/* This checks and barfs if author is badly specified */
	determine_author_info(author_ident);

	ಯದಿ (!no_verify && run_commit_hook(use_editor, index_file, &invoked_hook,
					  "pre-commit", ರಿಕ್ತ))
		ಪ್ರತ್ಯಾಗಮ 0;

	ಯದಿ (squash_message) {
		/*
		 * Insert the proper subject line before other commit
		 * message options add their content.
		 */
		ಯದಿ (use_message && !strcmp(use_message, squash_message))
			strbuf_addstr(&sb, "squash! ");
		ಅನ್ಯಥಾ {
			ಸಂರಚನಾ pretty_print_context ctx = {0};
			ಸಂರಚನಾ commit *c;
			c = lookup_commit_reference_by_name(squash_message);
			ಯದಿ (!c)
				die(_("could not lookup commit '%s'"), squash_message);
			ctx.output_encoding = get_commit_output_encoding();
			repo_format_commit_message(the_repository, c,
						   "squash! %s\n\n", &sb,
						   &ctx);
		}
	}

	ಯದಿ (have_option_m && !fixup_message) {
		strbuf_addbuf(&sb, &message);
		hook_arg1 = "message";
	} ಅನ್ಯಥಾ ಯದಿ (logfile && !strcmp(logfile, "-")) {
		ಯದಿ (isatty(0))
			fprintf(stderr, _("(reading log message from standard input)\n"));
		ಯದಿ (strbuf_read(&sb, 0, 0) < 0)
			die_errno(_("could not read log from standard input"));
		hook_arg1 = "message";
	} ಅನ್ಯಥಾ ಯದಿ (logfile) {
		ಯದಿ (strbuf_read_file(&sb, logfile, 0) < 0)
			die_errno(_("could not read log file '%s'"),
				  logfile);
		hook_arg1 = "message";
	} ಅನ್ಯಥಾ ಯದಿ (use_message) {
		ಧ್ರುವ ವರ್ಣ *buffer;
		buffer = strstr(use_message_buffer, "\n\n");
		ಯದಿ (buffer)
			strbuf_addstr(&sb, skip_blank_ಸಾಲುಗಳು(buffer + 2));
		hook_arg1 = "commit";
		hook_arg2 = use_message;
	} ಅನ್ಯಥಾ ಯದಿ (fixup_message) {
		ಸಂರಚನಾ pretty_print_context ctx = {0};
		ಸಂರಚನಾ commit *commit;
		ವರ್ಣ *fmt;
		commit = lookup_commit_reference_by_name(fixup_commit);
		ಯದಿ (!commit)
			die(_("could not lookup commit '%s'"), fixup_commit);
		ctx.output_encoding = get_commit_output_encoding();
		fmt = xstrfmt("%s! %%s\n\n", fixup_prefix);
		repo_format_commit_message(the_repository, commit, fmt, &sb,
					   &ctx);
		ಮುಕ್ತ(fmt);
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
		ಯದಿ (have_option_m && !strcmp(fixup_prefix, "fixup"))
			strbuf_addbuf(&sb, &message);

		ಯದಿ (!strcmp(fixup_prefix, "amend")) {
			ಯದಿ (have_option_m)
				die(_("options '%s' and '%s:%s' cannot be used together"), "-m", "--fixup", fixup_message);
			prepare_amend_commit(commit, &sb, &ctx);
		}
	} ಅನ್ಯಥಾ ಯದಿ (!stat(git_path_merge_ಸಂದೇಶ(the_repository), &statbuf)) {
		size_t merge_ಸಂದೇಶ_start;

		/*
		 * prepend SQUASH_MSG here if it exists and a
		 * "merge --squash" was originally performed
		 */
		ಯದಿ (!stat(git_path_squash_ಸಂದೇಶ(the_repository), &statbuf)) {
			ಯದಿ (strbuf_read_file(&sb, git_path_squash_ಸಂದೇಶ(the_repository), 0) < 0)
				die_errno(_("could not read SQUASH_MSG"));
			hook_arg1 = "squash";
		} ಅನ್ಯಥಾ
			hook_arg1 = "merge";

		merge_ಸಂದೇಶ_start = sb.len;
		ಯದಿ (strbuf_read_file(&sb, git_path_merge_ಸಂದೇಶ(the_repository), 0) < 0)
			die_errno(_("could not read MERGE_MSG"));

		ಯದಿ (cleanup_mode == COMMIT_ಸಂದೇಶ_CLEANUP_SCISSORS &&
		    wt_status_locate_end(sb.buf + merge_ಸಂದೇಶ_start,
					 sb.len - merge_ಸಂದೇಶ_start) <
				sb.len - merge_ಸಂದೇಶ_start)
			s->added_cut_ಸಾಲು = 1;
	} ಅನ್ಯಥಾ ಯದಿ (!stat(git_path_squash_ಸಂದೇಶ(the_repository), &statbuf)) {
		ಯದಿ (strbuf_read_file(&sb, git_path_squash_ಸಂದೇಶ(the_repository), 0) < 0)
			die_errno(_("could not read SQUASH_MSG"));
		hook_arg1 = "squash";
	} ಅನ್ಯಥಾ ಯದಿ (template_file) {
		ಯದಿ (strbuf_read_file(&sb, template_file, 0) < 0)
			die_errno(_("could not read '%s'"), template_file);
		hook_arg1 = "template";
		clean_message_contents = 0;
	}

	/*
	 * The remaining cases don't modify the template message, but
	 * just set the argument(s) to the prepare-commit-msg hook.
	 */
	ಅನ್ಯಥಾ ಯದಿ (whence == FROM_MERGE)
		hook_arg1 = "merge";
	ಅನ್ಯಥಾ ಯದಿ (is_from_cherry_pick(whence) || whence == FROM_REBASE_PICK) {
		hook_arg1 = "commit";
		hook_arg2 = "CHERRY_PICK_HEAD";
	}

	ಯದಿ (squash_message) {
		/*
		 * If squash_commit was used for the commit subject,
		 * then we're possibly hijacking other commit log options.
		 * Reset the hook args to tell the real story.
		 */
		hook_arg1 = "message";
		hook_arg2 = "";
	}

	s->fp = fopen_for_writing(git_path_commit_editmsg());
	ಯದಿ (!s->fp)
		die_errno(_("could not open '%s'"), git_path_commit_editmsg());

	/* Ignore status.displayCommentPrefix: we do need comments in COMMIT_EDITMSG. */
	old_display_comment_prefix = s->display_comment_prefix;
	s->display_comment_prefix = 1;

	/*
	 * Most hints are counter-productive when the commit has
	 * already started.
	 */
	s->hints = 0;

	ಯದಿ (clean_message_contents)
		strbuf_stripspace(&sb, ರಿಕ್ತ);

	ಯದಿ (signoff)
		append_signoff(&sb, ignored_log_message_bytes(sb.buf, sb.len), 0);

	ಯದಿ (fwrite(sb.buf, 1, sb.len, s->fp) < sb.len)
		die_errno(_("could not write commit template"));

#ifndef WITH_BREAKING_CHANGES
	ಯದಿ (auto_comment_ಸಾಲು_char)
		adjust_comment_ಸಾಲು_char(&sb);
#endif /* !WITH_BREAKING_CHANGES */
	strbuf_release(&sb);

	/* This checks if committer ident is explicitly given */
	strbuf_addstr(&committer_ident, git_committer_info(IDENT_STRICT));
	ಯದಿ (use_editor && include_status) {
		ಪೂರ್ಣಾಙ್ಕ ident_shown = 0;
		ಗಣನಾ git_colorbool saved_color_setting;
		ಸಂರಚನಾ ident_split ci, ai;
		ಧ್ರುವ ವರ್ಣ *hint_cleanup_all = allow_empty_message ?
			_("Please enter the commit message for your changes."
			  " Lines starting\nwith '%s' will be ignored.\n") :
			_("Please enter the commit message for your changes."
			  " Lines starting\nwith '%s' will be ignored, and an empty"
			  " message aborts the commit.\n");
		ಧ್ರುವ ವರ್ಣ *hint_cleanup_space = allow_empty_message ?
			_("Please enter the commit message for your changes."
			  " Lines starting\n"
			  "with '%s' will be kept; you may remove them"
			  " yourself if you want to.\n") :
			_("Please enter the commit message for your changes."
			  " Lines starting\n"
			  "with '%s' will be kept; you may remove them"
			  " yourself if you want to.\n"
			  "An empty message aborts the commit.\n");
		ಯದಿ (whence != FROM_COMMIT) {
			ಯದಿ (cleanup_mode == COMMIT_ಸಂದೇಶ_CLEANUP_SCISSORS)
				wt_status_add_cut_ಸಾಲು(s);
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
		ಯದಿ (cleanup_mode == COMMIT_ಸಂದೇಶ_CLEANUP_ALL)
			status_printf(s, GIT_COLOR_NORMAL, hint_cleanup_all, comment_ಸಾಲು_str);
		ಅನ್ಯಥಾ ಯದಿ (cleanup_mode == COMMIT_ಸಂದೇಶ_CLEANUP_SCISSORS) {
			ಯದಿ (whence == FROM_COMMIT)
				wt_status_add_cut_ಸಾಲು(s);
		} ಅನ್ಯಥಾ /* COMMIT_MSG_CLEANUP_SPACE, that is. */
			status_printf(s, GIT_COLOR_NORMAL, hint_cleanup_space, comment_ಸಾಲು_str);

		/*
		 * These should never fail because they come from our own
		 * fmt_ident. They may fail the sane_ident test, but we know
		 * that the name and mail pointers will at least be valid,
		 * which is enough for our tests and printing here.
		 */
		assert_split_ident(&ai, author_ident);
		assert_split_ident(&ci, &committer_ident);

		ಯದಿ (ident_cmp(&ai, &ci))
			status_printf_ln(s, GIT_COLOR_NORMAL,
				_("%s"
				"Author:    %.*s <%.*s>"),
				ident_shown++ ? "" : "\n",
				(ಪೂರ್ಣಾಙ್ಕ)(ai.name_end - ai.name_begin), ai.name_begin,
				(ಪೂರ್ಣಾಙ್ಕ)(ai.mail_end - ai.mail_begin), ai.mail_begin);

		ಯದಿ (author_date_is_interesting())
			status_printf_ln(s, GIT_COLOR_NORMAL,
				_("%s"
				"Date:      %s"),
				ident_shown++ ? "" : "\n",
				show_ident_date(&ai, DATE_MODE(NORMAL)));

		ಯದಿ (!committer_ident_sufficiently_given())
			status_printf_ln(s, GIT_COLOR_NORMAL,
				_("%s"
				"Committer: %.*s <%.*s>"),
				ident_shown++ ? "" : "\n",
				(ಪೂರ್ಣಾಙ್ಕ)(ci.name_end - ci.name_begin), ci.name_begin,
				(ಪೂರ್ಣಾಙ್ಕ)(ci.mail_end - ci.mail_begin), ci.mail_begin);

		status_printf_ln(s, GIT_COLOR_NORMAL, "%s", ""); /* Add new line for clarity */

		saved_color_setting = s->use_color;
		s->use_color = GIT_COLOR_NEVER;
		committable = run_status(s->fp, index_file, prefix, 1, s);
		s->use_color = saved_color_setting;
		string_list_clear_func(&s->change, change_data_free);
	} ಅನ್ಯಥಾ {
		ಸಂರಚನಾ object_id oid;
		ಧ್ರುವ ವರ್ಣ *parent = "HEAD";

		ಯದಿ (!the_repository->index->initialized && repo_read_index(the_repository) < 0)
			die(_("Cannot read index"));

		ಯದಿ (amend)
			parent = "HEAD^1";

		ಯದಿ (repo_get_oid(the_repository, parent, &oid)) {
			ಪೂರ್ಣಾಙ್ಕ i, ita_nr = 0;

			/* TODO: audit for interaction with sparse-index. */
			ensure_full_index(the_repository->index);
			ಪ್ರತಿ (i = 0; i < the_repository->index->cache_nr; i++)
				ಯದಿ (ce_intent_to_add(the_repository->index->cache[i]))
					ita_nr++;
			committable = the_repository->index->cache_nr > ita_nr;
		} ಅನ್ಯಥಾ {
			/*
			 * Unless the user did explicitly request a submodule
			 * ignore mode by passing a command line option we do
			 * not ignore any changed submodule SHA-1s when
			 * comparing index and parent, no matter what is
			 * configured. Otherwise we won't commit any
			 * submodules which were manually staged, which would
			 * be really confusing.
			 */
			ಸಂರಚನಾ diff_flags flags = DIFF_FLAGS_INIT;
			flags.override_submodule_config = 1;
			ಯದಿ (ignore_submodule_arg &&
			    !strcmp(ignore_submodule_arg, "all"))
				flags.ignore_submodules = 1;
			committable = index_differs_from(the_repository,
							 parent, &flags, 1);
		}
	}
	strbuf_release(&committer_ident);

	fclose(s->fp);

	ಯದಿ (trailer_args.nr) {
		ಯದಿ (amend_file_with_trailers(git_path_commit_editmsg(), &trailer_args))
			die(_("unable to pass trailers to --trailers"));
		strvec_clear(&trailer_args);
	}

	/*
	 * Reject an attempt to record a non-merge empty commit without
	 * explicit --allow-empty. In the cherry-pick case, it may be
	 * empty due to conflict resolution, which the user should okay.
	 */
	ಯದಿ (!committable && whence != FROM_MERGE && !allow_empty &&
	    !(amend && is_a_merge(current_head))) {
		s->hints = advice_enabled(ADVICE_STATUS_HINTS);
		s->display_comment_prefix = old_display_comment_prefix;
		run_status(stdout, index_file, prefix, 0, s);
		ಯದಿ (amend)
			fputs(_(empty_amend_advice), stderr);
		ಅನ್ಯಥಾ ಯದಿ (is_from_cherry_pick(whence) ||
			 whence == FROM_REBASE_PICK) {
			fputs(_(empty_cherry_pick_advice), stderr);
			ಯದಿ (whence == FROM_CHERRY_PICK_SINGLE)
				fputs(_(empty_cherry_pick_advice_single), stderr);
			ಅನ್ಯಥಾ ಯದಿ (whence == FROM_CHERRY_PICK_MULTI)
				fputs(_(empty_cherry_pick_advice_multi), stderr);
			ಅನ್ಯಥಾ
				fputs(_(empty_rebase_pick_advice), stderr);
		}
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (!no_verify && invoked_hook) {
		/*
		 * Re-read the index as the pre-commit-commit hook was invoked
		 * and could have updated it. We must do this before we invoke
		 * the editor and after we invoke run_status above.
		 */
		discard_index(the_repository->index);
	}
	read_index_from(the_repository->index, index_file, repo_get_git_dir(the_repository));

	ಯದಿ (cache_ವೃಕ್ಷ_update(the_repository->index, 0)) {
		error(_("Error building trees"));
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (run_commit_hook(use_editor, index_file, ರಿಕ್ತ, "prepare-commit-msg",
			    git_path_commit_editmsg(), hook_arg1, hook_arg2, ರಿಕ್ತ))
		ಪ್ರತ್ಯಾಗಮ 0;

	ಯದಿ (use_editor) {
		ಸಂರಚನಾ strvec env = STRVEC_INIT;

		strvec_pushf(&env, "GIT_INDEX_FILE=%s", index_file);
		ಯದಿ (launch_editor(git_path_commit_editmsg(), ರಿಕ್ತ, env.v)) {
			fprintf(stderr,
			_("Please supply the message using either -m or -F option.\n"));
			exit(1);
		}
		strvec_clear(&env);
	}

	ಯದಿ (!no_verify &&
	    run_commit_hook(use_editor, index_file, ರಿಕ್ತ, "commit-msg",
			    git_path_commit_editmsg(), ರಿಕ್ತ)) {
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಪ್ರತ್ಯಾಗಮ 1;
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *find_author_by_nickname(ಧ್ರುವ ವರ್ಣ *name)
{
	ಸಂರಚನಾ rev_info revs;
	ಸಂರಚನಾ commit *commit;
	ಸಂರಚನಾ strbuf buf = STRBUF_INIT;
	ಧ್ರುವ ವರ್ಣ *av[20];
	ಪೂರ್ಣಾಙ್ಕ ac = 0;

	repo_init_revisions(the_repository, &revs, ರಿಕ್ತ);
	strbuf_addf(&buf, "--author=%s", name);
	av[++ac] = "--all";
	av[++ac] = "-i";
	av[++ac] = buf.buf;
	av[++ac] = ರಿಕ್ತ;
	setup_revisions(ac, av, &revs, ರಿಕ್ತ);
	revs.mailmap = xmalloc(ಪರಿಮಾಣ(ಸಂರಚನಾ string_list));
	string_list_init_nodup(revs.mailmap);
	read_mailmap(the_repository, revs.mailmap);

	ಯದಿ (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	commit = get_revision(&revs);
	ಯದಿ (commit) {
		ಸಂರಚನಾ pretty_print_context ctx = {0};
		ctx.date_mode.type = DATE_NORMAL;
		strbuf_release(&buf);
		repo_format_commit_message(the_repository, commit,
					   "%aN <%aE>", &buf, &ctx);
		release_revisions(&revs);
		ಪ್ರತ್ಯಾಗಮ strbuf_detach(&buf, ರಿಕ್ತ);
	}
	die(_("--author '%s' is not 'Name <email>' and matches no existing author"), name);
}

ಸ್ಥಿರ ಶೂನ್ಯ handle_ignored_arg(ಸಂರಚನಾ wt_status *s)
{
	ಯದಿ (!ignored_arg)
		; /* default already initialized */
	ಅನ್ಯಥಾ ಯದಿ (!strcmp(ignored_arg, "traditional"))
		s->show_ignored_mode = SHOW_TRADITIONAL_IGNORED;
	ಅನ್ಯಥಾ ಯದಿ (!strcmp(ignored_arg, "no"))
		s->show_ignored_mode = SHOW_NO_IGNORED;
	ಅನ್ಯಥಾ ಯದಿ (!strcmp(ignored_arg, "matching"))
		s->show_ignored_mode = SHOW_MATCHING_IGNORED;
	ಅನ್ಯಥಾ
		die(_("Invalid ignored mode '%s'"), ignored_arg);
}

ಸ್ಥಿರ ಗಣನಾ untracked_status_type parse_untracked_setting_name(ಧ್ರುವ ವರ್ಣ *u)
{
	/*
	 * Please update $__git_untracked_file_modes in
	 * git-completion.bash when you add new options
	 */
	ವಿಕಲ್ಪ (git_parse_maybe_bool(u)) {
	ಪಕ್ಷ 0:
		u = "no";
		ಭಙ್ಗ;
	ಪಕ್ಷ 1:
		u = "normal";
		ಭಙ್ಗ;
	ಸ್ವಭಾವ:
		ಭಙ್ಗ;
	}

	ಯದಿ (!strcmp(u, "no"))
		ಪ್ರತ್ಯಾಗಮ SHOW_NO_UNTRACKED_FILES;
	ಅನ್ಯಥಾ ಯದಿ (!strcmp(u, "normal"))
		ಪ್ರತ್ಯಾಗಮ SHOW_NORMAL_UNTRACKED_FILES;
	ಅನ್ಯಥಾ ಯದಿ (!strcmp(u, "all"))
		ಪ್ರತ್ಯಾಗಮ SHOW_ALL_UNTRACKED_FILES;
	ಅನ್ಯಥಾ
		ಪ್ರತ್ಯಾಗಮ SHOW_UNTRACKED_FILES_ERROR;
}

ಸ್ಥಿರ ಶೂನ್ಯ handle_untracked_files_arg(ಸಂರಚನಾ wt_status *s)
{
	ಗಣನಾ untracked_status_type u;

	ಯದಿ (!untracked_files_arg)
		ಪ್ರತ್ಯಾಗಮ; /* default already initialized */

	u = parse_untracked_setting_name(untracked_files_arg);
	ಯದಿ (u == SHOW_UNTRACKED_FILES_ERROR)
		die(_("Invalid untracked files mode '%s'"),
		    untracked_files_arg);
	s->show_untracked_files = u;
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *read_commit_message(ಧ್ರುವ ವರ್ಣ *name)
{
	ಧ್ರುವ ವರ್ಣ *out_enc;
	ಸಂರಚನಾ commit *commit;

	commit = lookup_commit_reference_by_name(name);
	ಯದಿ (!commit)
		die(_("could not lookup commit '%s'"), name);
	out_enc = get_commit_output_encoding();
	ಪ್ರತ್ಯಾಗಮ repo_logmsg_reencode(the_repository, commit, ರಿಕ್ತ, out_enc);
}

/*
 * Enumerate what needs to be propagated when --porcelain
 * is not in effect here.
 */
ಸ್ಥಿರ ಸಂರಚನಾ status_deferred_config {
	ಗಣನಾ wt_status_format status_format;
	ಪೂರ್ಣಾಙ್ಕ show_branch;
	ಗಣನಾ ahead_behind_flags ahead_behind;
} status_deferred_config = {
	STATUS_FORMAT_UNSPECIFIED,
	-1, /* unspecified */
	AHEAD_BEHIND_UNSPECIFIED,
};

ಸ್ಥಿರ ಶೂನ್ಯ finalize_deferred_config(ಸಂರಚನಾ wt_status *s)
{
	ಪೂರ್ಣಾಙ್ಕ use_deferred_config = (status_format != STATUS_FORMAT_PORCELAIN &&
				   status_format != STATUS_FORMAT_PORCELAIN_V2 &&
				   !s->null_termination);

	ಯದಿ (s->null_termination) {
		ಯದಿ (status_format == STATUS_FORMAT_NONE ||
		    status_format == STATUS_FORMAT_UNSPECIFIED)
			status_format = STATUS_FORMAT_PORCELAIN;
		ಅನ್ಯಥಾ ಯದಿ (status_format == STATUS_FORMAT_LONG)
			die(_("options '%s' and '%s' cannot be used together"), "--long", "-z");
	}

	ಯದಿ (use_deferred_config && status_format == STATUS_FORMAT_UNSPECIFIED)
		status_format = status_deferred_config.status_format;
	ಯದಿ (status_format == STATUS_FORMAT_UNSPECIFIED)
		status_format = STATUS_FORMAT_NONE;

	ಯದಿ (use_deferred_config && s->show_branch < 0)
		s->show_branch = status_deferred_config.show_branch;
	ಯದಿ (s->show_branch < 0)
		s->show_branch = 0;

	/*
	 * If the user did not give a "--[no]-ahead-behind" command
	 * line argument *AND* we will print in a human-readable format
	 * (short, long etc.) then we inherit from the status.aheadbehind
	 * config setting.  In all other cases (and porcelain V[12] formats
	 * in particular), we inherit _FULL for backwards compatibility.
	 */
	ಯದಿ (use_deferred_config &&
	    s->ahead_behind_flags == AHEAD_BEHIND_UNSPECIFIED)
		s->ahead_behind_flags = status_deferred_config.ahead_behind;

	ಯದಿ (s->ahead_behind_flags == AHEAD_BEHIND_UNSPECIFIED)
		s->ahead_behind_flags = AHEAD_BEHIND_FULL;
}

ಸ್ಥಿರ ಶೂನ್ಯ check_fixup_reword_options(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ *argv[]) {
	ಯದಿ (whence != FROM_COMMIT) {
		ಯದಿ (whence == FROM_MERGE)
			die(_("You are in the middle of a merge -- cannot reword."));
		ಅನ್ಯಥಾ ಯದಿ (is_from_cherry_pick(whence))
			die(_("You are in the middle of a cherry-pick -- cannot reword."));
	}
	ಯದಿ (argc)
		die(_("reword option of '%s' and path '%s' cannot be used together"), "--fixup", *argv);
	ಯದಿ (patch_interactive || interactive || all || also || only)
		die(_("reword option of '%s' and '%s' cannot be used together"),
			"--fixup", "--patch/--interactive/--all/--include/--only");
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ parse_and_validate_options(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ *argv[],
				      ಧ್ರುವ ಸಂರಚನಾ option *options,
				      ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ usage[],
				      ಧ್ರುವ ವರ್ಣ *prefix,
				      ಸಂರಚನಾ commit *current_head,
				      ಸಂರಚನಾ wt_status *s)
{
	argc = parse_options(argc, argv, prefix, options, usage, 0);
	finalize_deferred_config(s);

	ಯದಿ (force_author && !strchr(force_author, '>'))
		force_author = find_author_by_nickname(force_author);

	ಯದಿ (force_author && renew_authorship)
		die(_("options '%s' and '%s' cannot be used together"), "--reset-author", "--author");

	ಯದಿ (logfile || have_option_m || use_message)
		use_editor = 0;

	/* Sanity check options */
	ಯದಿ (amend && !current_head)
		die(_("You have nothing to amend."));
	ಯದಿ (amend && whence != FROM_COMMIT) {
		ಯದಿ (whence == FROM_MERGE)
			die(_("You are in the middle of a merge -- cannot amend."));
		ಅನ್ಯಥಾ ಯದಿ (is_from_cherry_pick(whence))
			die(_("You are in the middle of a cherry-pick -- cannot amend."));
		ಅನ್ಯಥಾ ಯದಿ (whence == FROM_REBASE_PICK)
			die(_("You are in the middle of a rebase -- cannot amend."));
	}
	ಯದಿ (fixup_message && squash_message)
		die(_("options '%s' and '%s' cannot be used together"), "--squash", "--fixup");
	die_for_incompatible_opt4(!!use_message, "-C",
				  !!edit_message, "-c",
				  !!logfile, "-F",
				  !!fixup_message, "--fixup");
	die_for_incompatible_opt4(have_option_m, "-m",
				  !!edit_message, "-c",
				  !!use_message, "-C",
				  !!logfile, "-F");
	ಯದಿ (use_message || edit_message || logfile ||fixup_message || have_option_m)
		FREE_AND_NULL(template_file);
	ಯದಿ (edit_message)
		use_message = edit_message;
	ಯದಿ (amend && !use_message && !fixup_message)
		use_message = "HEAD";
	ಯದಿ (!use_message && !is_from_cherry_pick(whence) &&
	    !is_from_rebase(whence) && renew_authorship)
		die(_("--reset-author can be used only with -C, -c or --amend."));
	ಯದಿ (use_message) {
		use_message_buffer = read_commit_message(use_message);
		ಯದಿ (!renew_authorship) {
			author_message = use_message;
			author_message_buffer = use_message_buffer;
		}
	}
	ಯದಿ ((is_from_cherry_pick(whence) || whence == FROM_REBASE_PICK) &&
	    !renew_authorship) {
		author_message = "CHERRY_PICK_HEAD";
		author_message_buffer = read_commit_message(author_message);
	}

	ಯದಿ (patch_interactive)
		interactive = 1;

	die_for_incompatible_opt4(also, "-i/--include",
				  only, "-o/--only",
				  all, "-a/--all",
				  interactive, "--interactive/-p/--patch");
	ಯದಿ (fixup_message) {
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
		ವರ್ಣ *p = fixup_message;
		ಯಾವತ್ (isalpha(*p))
			p++;
		ಯದಿ (p > fixup_message && *p == ':') {
			*p = '\0';
			fixup_commit = p + 1;
			ಯದಿ (!strcmp("amend", fixup_message) ||
			    !strcmp("reword", fixup_message)) {
				fixup_prefix = "amend";
				allow_empty = 1;
				ಯದಿ (*fixup_message == 'r') {
					check_fixup_reword_options(argc, argv);
					only = 1;
				}
			} ಅನ್ಯಥಾ {
				die(_("unknown option: --fixup=%s:%s"), fixup_message, fixup_commit);
			}
		} ಅನ್ಯಥಾ {
			fixup_commit = fixup_message;
			fixup_prefix = "fixup";
			use_editor = 0;
		}
	}

	ಯದಿ (0 <= edit_flag)
		use_editor = edit_flag;

	handle_untracked_files_arg(s);

	ಯದಿ (all && argc > 0)
		die(_("paths '%s ...' with -a does not make sense"),
		    argv[0]);

	ಯದಿ (status_format != STATUS_FORMAT_NONE)
		dry_run = 1;

	ಪ್ರತ್ಯಾಗಮ argc;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ dry_run_commit(ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
			  ಧ್ರುವ ಸಂರಚನಾ commit *current_head, ಸಂರಚನಾ wt_status *s)
{
	ಪೂರ್ಣಾಙ್ಕ committable;
	ಧ್ರುವ ವರ್ಣ *index_file;

	index_file = prepare_index(argv, prefix, current_head, 1);
	committable = run_status(stdout, index_file, prefix, 0, s);
	rollback_index_files();

	ಪ್ರತ್ಯಾಗಮ committable ? 0 : 1;
}

define_list_config_array_extra(color_status_slots, {"added"});

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ parse_status_slot(ಧ್ರುವ ವರ್ಣ *slot)
{
	ಯದಿ (!strcasecmp(slot, "added"))
		ಪ್ರತ್ಯಾಗಮ WT_STATUS_UPDATED;

	ಪ್ರತ್ಯಾಗಮ LOOKUP_CONFIG(color_status_slots, slot);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ git_status_config(ಧ್ರುವ ವರ್ಣ *k, ಧ್ರುವ ವರ್ಣ *v,
			     ಧ್ರುವ ಸಂರಚನಾ config_context *ctx, ಶೂನ್ಯ *cb)
{
	ಸಂರಚನಾ wt_status *s = cb;
	ಧ್ರುವ ವರ್ಣ *slot_name;

	ಯದಿ (starts_with(k, "column."))
		ಪ್ರತ್ಯಾಗಮ git_column_config(k, v, "status", &s->colopts);
	ಯದಿ (!strcmp(k, "status.submodulesummary")) {
		ಪೂರ್ಣಾಙ್ಕ is_bool;
		s->submodule_summary = git_config_bool_or_int(k, v, ctx->kvi,
							      &is_bool);
		ಯದಿ (is_bool && s->submodule_summary)
			s->submodule_summary = -1;
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!strcmp(k, "status.short")) {
		ಯದಿ (git_config_bool(k, v))
			status_deferred_config.status_format = STATUS_FORMAT_SHORT;
		ಅನ್ಯಥಾ
			status_deferred_config.status_format = STATUS_FORMAT_NONE;
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!strcmp(k, "status.branch")) {
		status_deferred_config.show_branch = git_config_bool(k, v);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!strcmp(k, "status.aheadbehind")) {
		status_deferred_config.ahead_behind = git_config_bool(k, v);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!strcmp(k, "status.showstash")) {
		s->show_stash = git_config_bool(k, v);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!strcmp(k, "status.color") || !strcmp(k, "color.status")) {
		s->use_color = git_config_colorbool(k, v);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!strcmp(k, "status.displaycommentprefix")) {
		s->display_comment_prefix = git_config_bool(k, v);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (skip_prefix(k, "status.color.", &slot_name) ||
	    skip_prefix(k, "color.status.", &slot_name)) {
		ಪೂರ್ಣಾಙ್ಕ slot = parse_status_slot(slot_name);
		ಯದಿ (slot < 0)
			ಪ್ರತ್ಯಾಗಮ 0;
		ಯದಿ (!v)
			ಪ್ರತ್ಯಾಗಮ config_error_nonbool(k);
		ಪ್ರತ್ಯಾಗಮ color_parse(v, s->color_palette[slot]);
	}
	ಯದಿ (!strcmp(k, "status.relativepaths")) {
		s->relative_paths = git_config_bool(k, v);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!strcmp(k, "status.showuntrackedfiles")) {
		ಗಣನಾ untracked_status_type u;

		u = parse_untracked_setting_name(v);
		ಯದಿ (u == SHOW_UNTRACKED_FILES_ERROR)
			ಪ್ರತ್ಯಾಗಮ error(_("Invalid untracked files mode '%s'"), v);
		s->show_untracked_files = u;
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!strcmp(k, "diff.renamelimit")) {
		ಯದಿ (s->rename_limit == -1)
			s->rename_limit = git_config_int(k, v, ctx->kvi);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!strcmp(k, "status.renamelimit")) {
		s->rename_limit = git_config_int(k, v, ctx->kvi);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!strcmp(k, "diff.renames")) {
		ಯದಿ (s->detect_rename == -1)
			s->detect_rename = git_config_rename(k, v);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!strcmp(k, "status.renames")) {
		s->detect_rename = git_config_rename(k, v);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಪ್ರತ್ಯಾಗಮ git_diff_ui_config(k, v, ctx, ರಿಕ್ತ);
}

ಪೂರ್ಣಾಙ್ಕ cmd_status(ಪೂರ್ಣಾಙ್ಕ argc,
ಧ್ರುವ ವರ್ಣ **argv,
ಧ್ರುವ ವರ್ಣ *prefix,
ಸಂರಚನಾ repository *repo UNUSED)
{
	ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ no_renames = -1;
	ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *rename_score_arg = (ಧ್ರುವ ವರ್ಣ *)-1;
	ಸ್ಥಿರ ಸಂರಚನಾ wt_status s;
	unsigned ಪೂರ್ಣಾಙ್ಕ progress_flag = 0;
	ಪೂರ್ಣಾಙ್ಕ fd;
	ಸಂರಚನಾ object_id oid;
	ಸ್ಥಿರ ಸಂರಚನಾ option builtin_status_options[] = {
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
			.ಮೌಲ್ಯ = &untracked_files_arg,
			.argh = N_("mode"),
			.help = N_("show untracked files, optional modes: all, normal, no. (Default: all)"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t)"all",
		},
		{
			.type = OPTION_STRING,
			.long_name = "ignored",
			.ಮೌಲ್ಯ = &ignored_arg,
			.argh = N_("mode"),
			.help = N_("show ignored files, optional modes: traditional, matching, no. (Default: traditional)"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t)"traditional",
		},
		{
			.type = OPTION_STRING,
			.long_name = "ignore-submodules",
			.ಮೌಲ್ಯ = &ignore_submodule_arg,
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
	the_repository->settings.command_requires_full_index = 0;

	status_init_config(&s, git_status_config);
	argc = parse_options(argc, argv, prefix,
			     builtin_status_options,
			     builtin_status_usage, 0);
	finalize_colopts(&s.colopts, -1);
	finalize_deferred_config(&s);

	handle_untracked_files_arg(&s);
	handle_ignored_arg(&s);

	ಯದಿ (s.show_ignored_mode == SHOW_MATCHING_IGNORED &&
	    s.show_untracked_files == SHOW_NO_UNTRACKED_FILES)
		die(_("Unsupported combination of ignored and untracked-files arguments"));

	parse_pathspec(&s.pathspec, 0,
		       PATHSPEC_PREFER_FULL,
		       prefix, argv);

	ಯದಿ (status_format != STATUS_FORMAT_PORCELAIN &&
	    status_format != STATUS_FORMAT_PORCELAIN_V2)
		progress_flag = REFRESH_PROGRESS;
	repo_read_index(the_repository);
	refresh_index(the_repository->index,
		      REFRESH_QUIET|REFRESH_UNMERGED|progress_flag,
		      &s.pathspec, ರಿಕ್ತ, ರಿಕ್ತ);

	ಯದಿ (use_optional_locks())
		fd = repo_hold_locked_index(the_repository, &index_lock, 0);
	ಅನ್ಯಥಾ
		fd = -1;

	s.is_initial = repo_get_oid(the_repository, s.reference, &oid) ? 1 : 0;
	ಯದಿ (!s.is_initial)
		oidcpy(&s.oid_commit, &oid);

	s.ignore_submodule_arg = ignore_submodule_arg;
	s.status_format = status_format;
	s.verbose = verbose;
	ಯದಿ (no_renames != -1)
		s.detect_rename = !no_renames;
	ಯದಿ ((intptr_t)rename_score_arg != -1) {
		ಯದಿ (s.detect_rename < DIFF_DETECT_RENAME)
			s.detect_rename = DIFF_DETECT_RENAME;
		ಯದಿ (rename_score_arg)
			s.rename_score = parse_rename_score(&rename_score_arg);
	}

	wt_status_collect(&s);

	ಯದಿ (0 <= fd)
		repo_update_index_if_able(the_repository, &index_lock);

	ಯದಿ (s.relative_paths)
		s.prefix = prefix;

	wt_status_print(&s);
	wt_status_collect_free_buffers(&s);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ git_commit_config(ಧ್ರುವ ವರ್ಣ *k, ಧ್ರುವ ವರ್ಣ *v,
			     ಧ್ರುವ ಸಂರಚನಾ config_context *ctx, ಶೂನ್ಯ *cb)
{
	ಸಂರಚನಾ wt_status *s = cb;

	ಯದಿ (!strcmp(k, "commit.template"))
		ಪ್ರತ್ಯಾಗಮ git_config_pathname(&template_file, k, v);
	ಯದಿ (!strcmp(k, "commit.status")) {
		include_status = git_config_bool(k, v);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!strcmp(k, "commit.cleanup")) {
		FREE_AND_NULL(cleanup_config);
		ಪ್ರತ್ಯಾಗಮ git_config_string(&cleanup_config, k, v);
	}
	ಯದಿ (!strcmp(k, "commit.gpgsign")) {
		sign_commit = git_config_bool(k, v) ? "" : ರಿಕ್ತ;
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!strcmp(k, "commit.verbose")) {
		ಪೂರ್ಣಾಙ್ಕ is_bool;
		config_commit_verbose = git_config_bool_or_int(k, v, ctx->kvi,
							       &is_bool);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಪ್ರತ್ಯಾಗಮ git_status_config(k, v, ctx, s);
}

ಪೂರ್ಣಾಙ್ಕ cmd_commit(ಪೂರ್ಣಾಙ್ಕ argc,
	       ಧ್ರುವ ವರ್ಣ **argv,
	       ಧ್ರುವ ವರ್ಣ *prefix,
	       ಸಂರಚನಾ repository *repo UNUSED)
{
	ಸ್ಥಿರ ಸಂರಚನಾ wt_status s;
	ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *cleanup_arg = ರಿಕ್ತ;
	ಸ್ಥಿರ ಸಂರಚನಾ option builtin_commit_options[] = {
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
			.ಮೌಲ್ಯ = &sign_commit,
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
			.ಮೌಲ್ಯ = &untracked_files_arg,
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

	ಸಂರಚನಾ strbuf sb = STRBUF_INIT;
	ಸಂರಚನಾ strbuf author_ident = STRBUF_INIT;
	ಧ್ರುವ ವರ್ಣ *index_file, *reflog_ಸಂದೇಶ;
	ಸಂರಚನಾ object_id oid;
	ಸಂರಚನಾ commit_list *parents = ರಿಕ್ತ;
	ಸಂರಚನಾ stat statbuf;
	ಸಂರಚನಾ commit *current_head = ರಿಕ್ತ;
	ಸಂರಚನಾ commit_extra_header *extra = ರಿಕ್ತ;
	ಸಂರಚನಾ strbuf err = STRBUF_INIT;
	ಪೂರ್ಣಾಙ್ಕ ret = 0;

	show_usage_with_options_if_asked(argc, argv,
					 builtin_commit_usage, builtin_commit_options);

#ifndef WITH_BREAKING_CHANGES
	warn_on_auto_comment_char = true;
#endif /* !WITH_BREAKING_CHANGES */
	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	status_init_config(&s, git_commit_config);
	s.commit_template = 1;
	status_format = STATUS_FORMAT_NONE; /* Ignore status.short */
	s.colopts = 0;

	ಯದಿ (repo_get_oid(the_repository, "HEAD", &oid))
		current_head = ರಿಕ್ತ;
	ಅನ್ಯಥಾ {
		current_head = lookup_commit_or_die(&oid, "HEAD");
		ಯದಿ (repo_parse_commit(the_repository, current_head))
			die(_("could not parse HEAD commit"));
	}
	verbose = -1; /* unspecified */
	argc = parse_and_validate_options(argc, argv, builtin_commit_options,
					  builtin_commit_usage,
					  prefix, current_head, &s);
	ಯದಿ (trailer_args.nr)
		trailer_config_init();

	ಯದಿ (verbose == -1)
		verbose = (config_commit_verbose < 0) ? 0 : config_commit_verbose;

	ಯದಿ (cleanup_arg) {
		ಮುಕ್ತ(cleanup_config);
		cleanup_config = xstrdup(cleanup_arg);
	}
	cleanup_mode = get_cleanup_mode(cleanup_config, use_editor);

	ಯದಿ (dry_run)
		ಪ್ರತ್ಯಾಗಮ dry_run_commit(argv, prefix, current_head, &s);
	index_file = prepare_index(argv, prefix, current_head, 0);

	/* Set up everything for writing the commit object.  This includes
	   running hooks, writing the trees, and interacting with the user.  */
	ಯದಿ (!prepare_to_commit(index_file, prefix,
			       current_head, &s, &author_ident)) {
		ret = 1;
		rollback_index_files();
		ಗಚ್ಛ cleanup;
	}

	/* Determine parents */
	reflog_ಸಂದೇಶ = getenv("GIT_REFLOG_ACTION");
	ಯದಿ (!current_head) {
		ಯದಿ (!reflog_ಸಂದೇಶ)
			reflog_ಸಂದೇಶ = "commit (initial)";
	} ಅನ್ಯಥಾ ಯದಿ (amend) {
		ಯದಿ (!reflog_ಸಂದೇಶ)
			reflog_ಸಂದೇಶ = "commit (amend)";
		parents = commit_list_copy(current_head->parents);
	} ಅನ್ಯಥಾ ಯದಿ (whence == FROM_MERGE) {
		ಸಂರಚನಾ strbuf m = STRBUF_INIT;
		FILE *fp;
		ಪೂರ್ಣಾಙ್ಕ allow_fast_forward = 1;
		ಸಂರಚನಾ commit_list **pptr = &parents;

		ಯದಿ (!reflog_ಸಂದೇಶ)
			reflog_ಸಂದೇಶ = "commit (merge)";
		pptr = commit_list_append(current_head, pptr);
		fp = xfopen(git_path_merge_head(the_repository), "r");
		ಯಾವತ್ (strbuf_getline_lf(&m, fp) != EOF) {
			ಸಂರಚನಾ commit *parent;

			parent = get_merge_parent(m.buf);
			ಯದಿ (!parent)
				die(_("Corrupt MERGE_HEAD file (%s)"), m.buf);
			pptr = commit_list_append(parent, pptr);
		}
		fclose(fp);
		strbuf_release(&m);
		ಯದಿ (!stat(git_path_merge_mode(the_repository), &statbuf)) {
			ಯದಿ (strbuf_read_file(&sb, git_path_merge_mode(the_repository), 0) < 0)
				die_errno(_("could not read MERGE_MODE"));
			ಯದಿ (!strcmp(sb.buf, "no-ff"))
				allow_fast_forward = 0;
		}
		ಯದಿ (allow_fast_forward)
			reduce_heads_replace(&parents);
	} ಅನ್ಯಥಾ {
		ಯದಿ (!reflog_ಸಂದೇಶ)
			reflog_ಸಂದೇಶ = is_from_cherry_pick(whence)
					? "commit (cherry-pick)"
					: is_from_rebase(whence)
					? "commit (rebase)"
					: "commit";
		commit_list_ಸೇರಿಸು(current_head, &parents);
	}

	/* Finally, get the commit message */
	strbuf_reset(&sb);
	ಯದಿ (strbuf_read_file(&sb, git_path_commit_editmsg(), 0) < 0) {
		ಪೂರ್ಣಾಙ್ಕ saved_errno = errno;
		rollback_index_files();
		die(_("could not read commit message: %s"), strerror(saved_errno));
	}

	cleanup_message(&sb, cleanup_mode, verbose);

	ಯದಿ (message_is_empty(&sb, cleanup_mode) && !allow_empty_message) {
		rollback_index_files();
		fprintf(stderr, _("Aborting commit due to empty commit message.\n"));
		exit(1);
	}
	ಯದಿ (template_untouched(&sb, template_file, cleanup_mode) && !allow_empty_message) {
		rollback_index_files();
		fprintf(stderr, _("Aborting commit; you did not edit the message.\n"));
		exit(1);
	}

	ಯದಿ (fixup_message && starts_with(sb.buf, "amend! ") &&
	    !allow_empty_message) {
		ಸಂರಚನಾ strbuf body = STRBUF_INIT;
		size_t len = commit_subject_length(sb.buf);
		strbuf_addstr(&body, sb.buf + len);
		ಯದಿ (message_is_empty(&body, cleanup_mode)) {
			rollback_index_files();
			fprintf(stderr, _("Aborting commit due to empty commit message body.\n"));
			exit(1);
		}
		strbuf_release(&body);
	}

	ಯದಿ (amend) {
		ಧ್ರುವ ವರ್ಣ *exclude_gpgsig[3] = { "gpgsig", "gpgsig-sha256", ರಿಕ್ತ };
		extra = read_commit_extra_headers(current_head, exclude_gpgsig);
	} ಅನ್ಯಥಾ {
		ಸಂರಚನಾ commit_extra_header **tail = &extra;
		append_merge_tag_headers(parents, &tail);
	}

	ಯದಿ (commit_ವೃಕ್ಷ_extended(sb.buf, sb.len, &the_repository->index->cache_ವೃಕ್ಷ->oid,
				 parents, &oid, author_ident.buf, ರಿಕ್ತ,
				 sign_commit, extra)) {
		rollback_index_files();
		die(_("failed to write commit object"));
	}

	ಯದಿ (update_head_with_reflog(current_head, &oid, reflog_ಸಂದೇಶ, &sb,
				    &err)) {
		rollback_index_files();
		die("%s", err.buf);
	}

	sequencer_post_commit_cleanup(the_repository, 0);
	unlink(git_path_merge_head(the_repository));
	unlink(git_path_merge_ಸಂದೇಶ(the_repository));
	unlink(git_path_merge_mode(the_repository));
	unlink(git_path_squash_ಸಂದೇಶ(the_repository));

	ಯದಿ (commit_index_files())
		die(_("repository has been updated, but unable to write\n"
		      "new index file. Check that disk is not full and quota is\n"
		      "not exceeded, and then \"git restore --staged :/\" to recover."));

	git_test_write_commit_graph_or_die(the_repository);

	repo_rerere(the_repository, 0);
	run_auto_maintenance(the_repository, quiet);
	run_commit_hook(use_editor, repo_get_index_file(the_repository),
			ರಿಕ್ತ, "post-commit", ರಿಕ್ತ);
	ಯದಿ (amend && !no_post_rewrite) {
		commit_post_rewrite(the_repository, current_head, &oid);
	}
	ಯದಿ (!quiet) {
		unsigned ಪೂರ್ಣಾಙ್ಕ flags = 0;

		ಯದಿ (!current_head)
			flags |= SUMMARY_INITIAL_COMMIT;
		ಯದಿ (author_date_is_interesting())
			flags |= SUMMARY_SHOW_AUTHOR_DATE;
		print_commit_summary(the_repository, prefix,
				     &oid, flags);
	}

	apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
			    ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ);

cleanup:
	free_commit_extra_headers(extra);
	commit_list_free(parents);
	strbuf_release(&author_ident);
	strbuf_release(&err);
	strbuf_release(&sb);
	ಮುಕ್ತ(logfile);
	ಮುಕ್ತ(template_file);
	ಪ್ರತ್ಯಾಗಮ ret;
}
