/*
 * Builtin "git commit"
 *
 * Copyright (c) 2007 Kristian Høgsberg <krh@redhat.com>
 * Based on git-commit.sh by Junio C Hamano and Linus Torvalds
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "advice.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "lockfile.h"
#അന്തര്ഭാവ "cache-tree.h"
#അന്തര്ഭാവ "color.h"
#അന്തര്ഭാവ "dir.h"
#അന്തര്ഭാവ "editor.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "diff.h"
#അന്തര്ഭാവ "commit.h"
#അന്തര്ഭാവ "add-interactive.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "revision.h"
#അന്തര്ഭാവ "wt-status.h"
#അന്തര്ഭാവ "run-command.h"
#അന്തര്ഭാവ "strbuf.h"
#അന്തര്ഭാവ "object-name.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "path.h"
#അന്തര്ഭാവ "preload-index.h"
#അന്തര്ഭാവ "read-cache.h"
#അന്തര്ഭാവ "repository.h"
#അന്തര്ഭാവ "string-list.h"
#അന്തര്ഭാവ "rerere.h"
#അന്തര്ഭാവ "unpack-trees.h"
#അന്തര്ഭാവ "column.h"
#അന്തര്ഭാവ "sequencer.h"
#അന്തര്ഭാവ "sparse-index.h"
#അന്തര്ഭാവ "mailmap.h"
#അന്തര്ഭാവ "help.h"
#അന്തര്ഭാവ "commit-reach.h"
#അന്തര്ഭാവ "commit-graph.h"
#അന്തര്ഭാവ "pretty.h"
#അന്തര്ഭാവ "trailer.h"

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം builtin_commit_usage[] = {
	N_("git commit [-a | --interactive | --patch] [-s] [-v] [-u[<mode>]] [--amend]\n"
	   "           [--dry-run] [(-c | -C | --squash) <commit> | --fixup [(amend|reword):]<commit>]\n"
	   "           [-F <file> | -m <msg>] [--reset-author] [--allow-empty]\n"
	   "           [--allow-empty-message] [--no-verify] [-e] [--author=<author>]\n"
	   "           [--date=<date>] [--cleanup=<mode>] [--[no-]status]\n"
	   "           [-i | -o] [--pathspec-from-file=<file> [--pathspec-file-nul]]\n"
	   "           [(--trailer <token>[(=|:)<value>])...] [-S[<keyid>]]\n"
	   "           [--] [<pathspec>...]"),
	ശൂന്യ
};

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം builtin_status_usage[] = {
	N_("git status [<options>] [--] [<pathspec>...]"),
	ശൂന്യ
};

സ്ഥിതം സ്ഥിരം അക്ഷരം empty_amend_advice[] =
N_("You asked to amend the most recent commit, but doing so would make\n"
"it empty. You can repeat your command with --allow-empty, or you can\n"
"remove the commit entirely with \"git reset HEAD^\".\n");

സ്ഥിതം സ്ഥിരം അക്ഷരം empty_cherry_pick_advice[] =
N_("The previous cherry-pick is now empty, possibly due to conflict resolution.\n"
"If you wish to commit it anyway, use:\n"
"\n"
"    git commit --allow-empty\n"
"\n");

സ്ഥിതം സ്ഥിരം അക്ഷരം empty_rebase_pick_advice[] =
N_("Otherwise, please use 'git rebase --skip'\n");

സ്ഥിതം സ്ഥിരം അക്ഷരം empty_cherry_pick_advice_single[] =
N_("Otherwise, please use 'git cherry-pick --skip'\n");

സ്ഥിതം സ്ഥിരം അക്ഷരം empty_cherry_pick_advice_multi[] =
N_("and then use:\n"
"\n"
"    git cherry-pick --continue\n"
"\n"
"to resume cherry-picking the remaining commits.\n"
"If you wish to skip this commit, use:\n"
"\n"
"    git cherry-pick --skip\n"
"\n");

സ്ഥിതം സ്ഥിരം അക്ഷരം *color_status_slots[] = {
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

സ്ഥിതം സ്ഥിരം അക്ഷരം *use_message_buffer;
സ്ഥിതം ഘടന lock_file index_lock; /* real index */
സ്ഥിതം ഘടന lock_file false_lock; /* used only for partial commits */
സ്ഥിതം എണ്ണൽ {
	COMMIT_AS_IS = 1,
	COMMIT_NORMAL,
	COMMIT_PARTIAL
} commit_style;

സ്ഥിതം സ്ഥിരം അക്ഷരം *force_author;
സ്ഥിതം അക്ഷരം *logfile;
സ്ഥിതം അക്ഷരം *template_file;
/*
 * The _message variables are commit names from which to take
 * the commit message and/or authorship.
 */
സ്ഥിതം സ്ഥിരം അക്ഷരം *author_message, *author_message_buffer;
സ്ഥിതം സ്ഥിരം അക്ഷരം *edit_message, *use_message;
സ്ഥിതം അക്ഷരം *fixup_message, *fixup_commit, *squash_message;
സ്ഥിതം സ്ഥിരം അക്ഷരം *fixup_prefix;
സ്ഥിതം പൂർണ്ണസംഖ്യ all, also, interactive, patch_interactive, only, amend, signoff;
സ്ഥിതം ഘടന interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
സ്ഥിതം പൂർണ്ണസംഖ്യ edit_flag = -1; /* unspecified */
സ്ഥിതം പൂർണ്ണസംഖ്യ quiet, verbose, no_verify, allow_empty, dry_run, renew_authorship;
സ്ഥിതം പൂർണ്ണസംഖ്യ config_commit_verbose = -1; /* unspecified */
സ്ഥിതം പൂർണ്ണസംഖ്യ no_post_rewrite, allow_empty_message, pathspec_file_nul;
സ്ഥിതം സ്ഥിരം അക്ഷരം *untracked_files_arg, *force_date, *ignore_submodule_arg, *ignored_arg;
സ്ഥിതം സ്ഥിരം അക്ഷരം *sign_commit, *pathspec_from_file;
സ്ഥിതം ഘടന strvec trailer_args = STRVEC_INIT;

/*
 * The default commit message cleanup mode will remove the lines
 * beginning with # (shell comments) and leading and trailing
 * whitespaces (empty lines or containing only whitespaces)
 * if editor is used, and only the whitespaces if the message
 * is specified explicitly.
 */
സ്ഥിതം എണ്ണൽ commit_സന്ദേശം_cleanup_mode cleanup_mode;
സ്ഥിതം അക്ഷരം *cleanup_config;

സ്ഥിതം എണ്ണൽ commit_whence whence;
സ്ഥിതം പൂർണ്ണസംഖ്യ use_editor = 1, include_status = 1;
സ്ഥിതം പൂർണ്ണസംഖ്യ have_option_m;
സ്ഥിതം ഘടന strbuf message = STRBUF_INIT;

സ്ഥിതം എണ്ണൽ wt_status_format status_format = STATUS_FORMAT_UNSPECIFIED;

സ്ഥിതം പൂർണ്ണസംഖ്യ opt_parse_porcelain(സ്ഥിരം ഘടന option *opt, സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	എണ്ണൽ wt_status_format *മൂല്യം = (എണ്ണൽ wt_status_format *)opt->മൂല്യം;
	എങ്കിൽ (unset)
		*മൂല്യം = STATUS_FORMAT_NONE;
	അല്ലെങ്കിൽ എങ്കിൽ (!arg)
		*മൂല്യം = STATUS_FORMAT_PORCELAIN;
	അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(arg, "v1") || !strcmp(arg, "1"))
		*മൂല്യം = STATUS_FORMAT_PORCELAIN;
	അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(arg, "v2") || !strcmp(arg, "2"))
		*മൂല്യം = STATUS_FORMAT_PORCELAIN_V2;
	അല്ലെങ്കിൽ
		die("unsupported porcelain version '%s'", arg);

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ opt_parse_m(സ്ഥിരം ഘടന option *opt, സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	ഘടന strbuf *buf = opt->മൂല്യം;
	എങ്കിൽ (unset) {
		have_option_m = 0;
		strbuf_setlen(buf, 0);
	} അല്ലെങ്കിൽ {
		have_option_m = 1;
		എങ്കിൽ (buf->len)
			strbuf_addch(buf, '\n');
		strbuf_addstr(buf, arg);
		strbuf_complete_വരി(buf);
	}
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ opt_parse_rename_score(സ്ഥിരം ഘടന option *opt, സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	സ്ഥിരം അക്ഷരം **മൂല്യം = opt->മൂല്യം;

	BUG_ON_OPT_NEG(unset);

	എങ്കിൽ (arg != ശൂന്യ && *arg == '=')
		arg = arg + 1;

	*മൂല്യം = arg;
	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം determine_whence(ഘടന wt_status *s)
{
	എങ്കിൽ (file_exists(git_path_merge_head(the_repository)))
		whence = FROM_MERGE;
	അല്ലെങ്കിൽ എങ്കിൽ (!sequencer_determine_whence(the_repository, &whence))
		whence = FROM_COMMIT;
	എങ്കിൽ (s)
		s->whence = whence;
}

സ്ഥിതം ശൂന്യം status_init_config(ഘടന wt_status *s, config_fn_t fn)
{
	wt_status_prepare(the_repository, s);
	init_diff_ui_defaults();
	repo_config(the_repository, fn, s);
	determine_whence(s);
	s->hints = advice_enabled(ADVICE_STATUS_HINTS); /* must come after repo_config() */
}

സ്ഥിതം ശൂന്യം rollback_index_files(ശൂന്യം)
{
	തിരഞ്ഞെടുപ്പ് (commit_style) {
	അവസ്ഥ COMMIT_AS_IS:
		നിർത്തുക; /* nothing to do */
	അവസ്ഥ COMMIT_NORMAL:
		rollback_lock_file(&index_lock);
		നിർത്തുക;
	അവസ്ഥ COMMIT_PARTIAL:
		rollback_lock_file(&index_lock);
		rollback_lock_file(&false_lock);
		നിർത്തുക;
	}
}

സ്ഥിതം പൂർണ്ണസംഖ്യ commit_index_files(ശൂന്യം)
{
	പൂർണ്ണസംഖ്യ err = 0;

	തിരഞ്ഞെടുപ്പ് (commit_style) {
	അവസ്ഥ COMMIT_AS_IS:
		നിർത്തുക; /* nothing to do */
	അവസ്ഥ COMMIT_NORMAL:
		err = commit_lock_file(&index_lock);
		നിർത്തുക;
	അവസ്ഥ COMMIT_PARTIAL:
		err = commit_lock_file(&index_lock);
		rollback_lock_file(&false_lock);
		നിർത്തുക;
	}

	തിരികെ err;
}

/*
 * Take a union of paths in the index and the named tree (typically, "HEAD"),
 * and return the paths that match the given pattern in list.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ list_paths(ഘടന string_list *list, സ്ഥിരം അക്ഷരം *with_വൃക്ഷം,
		      സ്ഥിരം ഘടന pathspec *pattern)
{
	പൂർണ്ണസംഖ്യ i, ret;
	അക്ഷരം *m;

	എങ്കിൽ (!pattern->nr)
		തിരികെ 0;

	m = xcalloc(1, pattern->nr);

	എങ്കിൽ (with_വൃക്ഷം) {
		അക്ഷരം *max_prefix = common_prefix(pattern);
		overlay_വൃക്ഷം_on_index(the_repository->index, with_വൃക്ഷം, max_prefix);
		സ്വതന്ത്രം(max_prefix);
	}

	/* TODO: audit for interaction with sparse-index. */
	ensure_full_index(the_repository->index);
	വേണ്ടി (i = 0; i < the_repository->index->cache_nr; i++) {
		സ്ഥിരം ഘടന cache_entry *ce = the_repository->index->cache[i];
		ഘടന string_list_item *item;

		എങ്കിൽ (ce->ce_flags & CE_UPDATE)
			തുടരുക;
		എങ്കിൽ (!ce_path_match(the_repository->index, ce, pattern, m))
			തുടരുക;
		item = string_list_തിരുകുക(list, ce->name);
		എങ്കിൽ (ce_skip_worktree(ce))
			item->util = item; /* better a valid pointer than a fake one */
	}

	ret = report_path_error(m, pattern);
	സ്വതന്ത്രം(m);
	തിരികെ ret;
}

സ്ഥിതം ശൂന്യം add_remove_files(ഘടന string_list *list)
{
	പൂർണ്ണസംഖ്യ i;
	വേണ്ടി (i = 0; i < list->nr; i++) {
		ഘടന stat st;
		ഘടന string_list_item *p = &(list->ഇനങ്ങൾ[i]);

		/* p->util is skip-worktree */
		എങ്കിൽ (p->util)
			തുടരുക;

		എങ്കിൽ (!lstat(p->string, &st)) {
			എങ്കിൽ (add_to_index(the_repository->index, p->string, &st, 0))
				die(_("updating files failed"));
		} അല്ലെങ്കിൽ
			remove_file_from_index(the_repository->index, p->string);
	}
}

സ്ഥിതം ശൂന്യം create_base_index(സ്ഥിരം ഘടന commit *current_head)
{
	ഘടന വൃക്ഷം *വൃക്ഷം;
	ഘടന unpack_trees_options opts;
	ഘടന വൃക്ഷം_desc t;

	എങ്കിൽ (!current_head) {
		discard_index(the_repository->index);
		തിരികെ;
	}

	memset(&opts, 0, വലുപ്പം(opts));
	opts.head_idx = 1;
	opts.index_only = 1;
	opts.merge = 1;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;

	opts.fn = oneway_merge;
	വൃക്ഷം = repo_parse_വൃക്ഷം_indirect(the_repository,
					&current_head->object.oid);
	എങ്കിൽ (!വൃക്ഷം)
		die(_("failed to unpack HEAD tree object"));
	എങ്കിൽ (repo_parse_വൃക്ഷം(the_repository, വൃക്ഷം) < 0)
		exit(128);
	init_വൃക്ഷം_desc(&t, &വൃക്ഷം->object.oid, വൃക്ഷം->buffer, വൃക്ഷം->size);
	എങ്കിൽ (unpack_trees(1, &t, &opts))
		exit(128); /* We've already reported the error, finish dying */
}

സ്ഥിതം ശൂന്യം refresh_cache_or_die(പൂർണ്ണസംഖ്യ refresh_flags)
{
	/*
	 * refresh_flags contains REFRESH_QUIET, so the only errors
	 * are for unmerged entries.
	 */
	എങ്കിൽ (refresh_index(the_repository->index, refresh_flags | REFRESH_IN_PORCELAIN, ശൂന്യ, ശൂന്യ, ശൂന്യ))
		die_resolve_conflict("commit");
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *prepare_index(സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
				 സ്ഥിരം ഘടന commit *current_head, പൂർണ്ണസംഖ്യ is_status)
{
	ഘടന string_list partial = STRING_LIST_INIT_DUP;
	ഘടന pathspec pathspec;
	പൂർണ്ണസംഖ്യ refresh_flags = REFRESH_QUIET;
	സ്ഥിരം അക്ഷരം *ret;
	അക്ഷരം *path = ശൂന്യ;

	എങ്കിൽ (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	എങ്കിൽ (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	എങ്കിൽ (is_status)
		refresh_flags |= REFRESH_UNMERGED;
	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_FULL,
		       prefix, argv);

	എങ്കിൽ (pathspec_from_file) {
		എങ്കിൽ (interactive)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--interactive/--patch");

		എങ്കിൽ (all)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "-a");

		എങ്കിൽ (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_FULL,
				    prefix, pathspec_from_file, pathspec_file_nul);
	} അല്ലെങ്കിൽ എങ്കിൽ (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	എങ്കിൽ (!pathspec.nr && (also || (only && !allow_empty &&
	    (!amend || (fixup_message && strcmp(fixup_prefix, "amend"))))))
		die(_("No paths with --include/--only does not make sense."));

	എങ്കിൽ (repo_read_index_preload(the_repository, &pathspec, 0) < 0)
		die(_("index file corrupt"));

	എങ്കിൽ (interactive) {
		അക്ഷരം *old_index_env = ശൂന്യ, *old_repo_index_file;
		repo_hold_locked_index(the_repository, &index_lock,
				       LOCK_DIE_ON_ERROR);

		refresh_cache_or_die(refresh_flags);

		എങ്കിൽ (write_locked_index(the_repository->index, &index_lock, 0))
			die(_("unable to create temporary index"));

		old_repo_index_file = the_repository->index_file;
		the_repository->index_file =
			(അക്ഷരം *)get_lock_file_path(&index_lock);
		old_index_env = xstrdup_or_null(getenv(INDEX_ENVIRONMENT));
		setenv(INDEX_ENVIRONMENT, the_repository->index_file, 1);

		എങ്കിൽ (interactive_add(the_repository, argv, prefix, patch_interactive, &interactive_opts) != 0)
			die(_("interactive add failed"));

		the_repository->index_file = old_repo_index_file;
		എങ്കിൽ (old_index_env && *old_index_env)
			setenv(INDEX_ENVIRONMENT, old_index_env, 1);
		അല്ലെങ്കിൽ
			unsetenv(INDEX_ENVIRONMENT);
		FREE_AND_NULL(old_index_env);

		discard_index(the_repository->index);
		read_index_from(the_repository->index, get_lock_file_path(&index_lock),
				repo_get_git_dir(the_repository));
		എങ്കിൽ (cache_വൃക്ഷം_update(the_repository->index, WRITE_വൃക്ഷം_SILENT) == 0) {
			എങ്കിൽ (reopen_lock_file(&index_lock) < 0)
				die(_("unable to write index file"));
			എങ്കിൽ (write_locked_index(the_repository->index, &index_lock, 0))
				die(_("unable to update temporary index"));
		} അല്ലെങ്കിൽ
			warning(_("Failed to update main cache tree"));

		commit_style = COMMIT_NORMAL;
		ret = get_lock_file_path(&index_lock);
		പോകുക out;
	} അല്ലെങ്കിൽ {
		എങ്കിൽ (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--interactive/--patch");
		എങ്കിൽ (interactive_opts.interhunkcontext != -1)
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
	എങ്കിൽ (all || (also && pathspec.nr)) {
		അക്ഷരം *ps_matched = xcalloc(pathspec.nr, 1);
		repo_hold_locked_index(the_repository, &index_lock,
				       LOCK_DIE_ON_ERROR);
		add_files_to_cache(the_repository, also ? prefix : ശൂന്യ,
				   &pathspec, ps_matched, 0, 0, 0 );
		എങ്കിൽ (!all && report_path_error(ps_matched, &pathspec))
			exit(128);

		refresh_cache_or_die(refresh_flags);
		cache_വൃക്ഷം_update(the_repository->index, WRITE_വൃക്ഷം_SILENT);
		എങ്കിൽ (write_locked_index(the_repository->index, &index_lock, 0))
			die(_("unable to write new index file"));
		commit_style = COMMIT_NORMAL;
		ret = get_lock_file_path(&index_lock);
		സ്വതന്ത്രം(ps_matched);
		പോകുക out;
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
	എങ്കിൽ (!only && !pathspec.nr) {
		repo_hold_locked_index(the_repository, &index_lock,
				       LOCK_DIE_ON_ERROR);
		refresh_cache_or_die(refresh_flags);
		എങ്കിൽ (the_repository->index->cache_changed
		    || !cache_വൃക്ഷം_fully_valid(the_repository->index))
			cache_വൃക്ഷം_update(the_repository->index, WRITE_വൃക്ഷം_SILENT);
		എങ്കിൽ (write_locked_index(the_repository->index, &index_lock,
				       COMMIT_LOCK | SKIP_IF_UNCHANGED))
			die(_("unable to write new index file"));
		commit_style = COMMIT_AS_IS;
		ret = repo_get_index_file(the_repository);
		പോകുക out;
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

	എങ്കിൽ (whence != FROM_COMMIT) {
		എങ്കിൽ (whence == FROM_MERGE)
			die(_("cannot do a partial commit during a merge."));
		അല്ലെങ്കിൽ എങ്കിൽ (is_from_cherry_pick(whence))
			die(_("cannot do a partial commit during a cherry-pick."));
		അല്ലെങ്കിൽ എങ്കിൽ (is_from_rebase(whence))
			die(_("cannot do a partial commit during a rebase."));
	}

	എങ്കിൽ (list_paths(&partial, !current_head ? ശൂന്യ : "HEAD", &pathspec))
		exit(1);

	discard_index(the_repository->index);
	എങ്കിൽ (repo_read_index(the_repository) < 0)
		die(_("cannot read the index"));

	repo_hold_locked_index(the_repository, &index_lock, LOCK_DIE_ON_ERROR);
	add_remove_files(&partial);
	refresh_index(the_repository->index, REFRESH_QUIET, ശൂന്യ, ശൂന്യ, ശൂന്യ);
	cache_വൃക്ഷം_update(the_repository->index, WRITE_വൃക്ഷം_SILENT);
	എങ്കിൽ (write_locked_index(the_repository->index, &index_lock, 0))
		die(_("unable to write new index file"));

	path = repo_git_path(the_repository, "next-index-%"PRIuMAX,
			     (uintmax_t) getpid());
	hold_lock_file_for_update(&false_lock, path,
				  LOCK_DIE_ON_ERROR);

	create_base_index(current_head);
	add_remove_files(&partial);
	refresh_index(the_repository->index, REFRESH_QUIET, ശൂന്യ, ശൂന്യ, ശൂന്യ);

	എങ്കിൽ (write_locked_index(the_repository->index, &false_lock, 0))
		die(_("unable to write temporary index file"));

	discard_index(the_repository->index);
	ret = get_lock_file_path(&false_lock);
	read_index_from(the_repository->index, ret, repo_get_git_dir(the_repository));
out:
	string_list_clear(&partial, 0);
	clear_pathspec(&pathspec);
	സ്വതന്ത്രം(path);
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ run_status(FILE *fp, സ്ഥിരം അക്ഷരം *index_file, സ്ഥിരം അക്ഷരം *prefix, പൂർണ്ണസംഖ്യ nowarn,
		      ഘടന wt_status *s)
{
	ഘടന object_id oid;

	എങ്കിൽ (s->relative_paths)
		s->prefix = prefix;

	എങ്കിൽ (amend) {
		s->amend = 1;
		s->reference = "HEAD^1";
	}
	s->verbose = verbose;
	s->index_file = index_file;
	s->fp = fp;
	s->nowarn = nowarn;
	s->is_initial = repo_get_oid(the_repository, s->reference, &oid) ? 1 : 0;
	എങ്കിൽ (!s->is_initial)
		oidcpy(&s->oid_commit, &oid);
	s->status_format = status_format;
	s->ignore_submodule_arg = ignore_submodule_arg;

	wt_status_collect(s);
	wt_status_print(s);
	wt_status_collect_free_buffers(s);

	തിരികെ s->committable;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ is_a_merge(സ്ഥിരം ഘടന commit *current_head)
{
	തിരികെ !!(current_head->parents && current_head->parents->next);
}

സ്ഥിതം ശൂന്യം assert_split_ident(ഘടന ident_split *id, സ്ഥിരം ഘടന strbuf *buf)
{
	എങ്കിൽ (split_ident_വരി(id, buf->buf, buf->len) || !id->date_begin)
		BUG("unable to parse our own ident: %s", buf->buf);
}

സ്ഥിതം ശൂന്യം export_one(സ്ഥിരം അക്ഷരം *var, സ്ഥിരം അക്ഷരം *s, സ്ഥിരം അക്ഷരം *e, പൂർണ്ണസംഖ്യ hack)
{
	ഘടന strbuf buf = STRBUF_INIT;
	എങ്കിൽ (hack)
		strbuf_addch(&buf, hack);
	strbuf_add(&buf, s, e - s);
	setenv(var, buf.buf, 1);
	strbuf_release(&buf);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ parse_force_date(സ്ഥിരം അക്ഷരം *in, ഘടന strbuf *out)
{
	strbuf_addch(out, '@');

	എങ്കിൽ (parse_date(in, out) < 0) {
		പൂർണ്ണസംഖ്യ errors = 0;
		unsigned long t = approxidate_careful(in, &errors);
		എങ്കിൽ (errors)
			തിരികെ -1;
		strbuf_addf(out, "%lu", t);
	}

	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം set_ident_var(അക്ഷരം **buf, അക്ഷരം *val)
{
	സ്വതന്ത്രം(*buf);
	*buf = val;
}

സ്ഥിതം ശൂന്യം determine_author_info(ഘടന strbuf *author_ident)
{
	അക്ഷരം *name, *email, *date;
	ഘടന ident_split author;

	name = xstrdup_or_null(getenv("GIT_AUTHOR_NAME"));
	email = xstrdup_or_null(getenv("GIT_AUTHOR_EMAIL"));
	date = xstrdup_or_null(getenv("GIT_AUTHOR_DATE"));

	എങ്കിൽ (author_message) {
		ഘടന ident_split ident;
		size_t len;
		സ്ഥിരം അക്ഷരം *a;

		a = find_commit_header(author_message_buffer, "author", &len);
		എങ്കിൽ (!a)
			die(_("commit '%s' lacks author header"), author_message);
		എങ്കിൽ (split_ident_വരി(&ident, a, len) < 0)
			die(_("commit '%s' has malformed author line"), author_message);

		set_ident_var(&name, xmemdupz(ident.name_begin, ident.name_end - ident.name_begin));
		set_ident_var(&email, xmemdupz(ident.mail_begin, ident.mail_end - ident.mail_begin));

		എങ്കിൽ (ident.date_begin) {
			ഘടന strbuf date_buf = STRBUF_INIT;
			strbuf_addch(&date_buf, '@');
			strbuf_add(&date_buf, ident.date_begin, ident.date_end - ident.date_begin);
			strbuf_addch(&date_buf, ' ');
			strbuf_add(&date_buf, ident.tz_begin, ident.tz_end - ident.tz_begin);
			set_ident_var(&date, strbuf_detach(&date_buf, ശൂന്യ));
		}
	}

	എങ്കിൽ (force_author) {
		ഘടന ident_split ident;

		എങ്കിൽ (split_ident_വരി(&ident, force_author, strlen(force_author)) < 0)
			die(_("malformed --author parameter"));
		set_ident_var(&name, xmemdupz(ident.name_begin, ident.name_end - ident.name_begin));
		set_ident_var(&email, xmemdupz(ident.mail_begin, ident.mail_end - ident.mail_begin));
	}

	എങ്കിൽ (force_date) {
		ഘടന strbuf date_buf = STRBUF_INIT;
		എങ്കിൽ (parse_force_date(force_date, &date_buf))
			die(_("invalid date format: %s"), force_date);
		set_ident_var(&date, strbuf_detach(&date_buf, ശൂന്യ));
	}

	strbuf_addstr(author_ident, fmt_ident(name, email, WANT_AUTHOR_IDENT, date,
				IDENT_STRICT));
	assert_split_ident(&author, author_ident);
	export_one("GIT_AUTHOR_NAME", author.name_begin, author.name_end, 0);
	export_one("GIT_AUTHOR_EMAIL", author.mail_begin, author.mail_end, 0);
	export_one("GIT_AUTHOR_DATE", author.date_begin, author.tz_end, '@');
	സ്വതന്ത്രം(name);
	സ്വതന്ത്രം(email);
	സ്വതന്ത്രം(date);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ author_date_is_interesting(ശൂന്യം)
{
	തിരികെ author_message || force_date;
}

#ifndef WITH_BREAKING_CHANGES
സ്ഥിതം ശൂന്യം adjust_comment_വരി_char(സ്ഥിരം ഘടന strbuf *sb)
{
	അക്ഷരം candidates[] = "#;@!$%^&|:";
	അക്ഷരം *candidate;
	സ്ഥിരം അക്ഷരം *p;
	size_t cutoff;

	/* Ignore comment chars in trailing comments (e.g., Conflicts:) */
	cutoff = sb->len - ignored_log_message_bytes(sb->buf, sb->len);

	എങ്കിൽ (!memchr(sb->buf, candidates[0], sb->len)) {
		സ്വതന്ത്രം(comment_വരി_str_to_free);
		comment_വരി_str = comment_വരി_str_to_free =
			xstrfmt("%c", candidates[0]);
		തിരികെ;
	}

	p = sb->buf;
	candidate = strchr(candidates, *p);
	എങ്കിൽ (candidate)
		*candidate = ' ';
	വേണ്ടി (p = sb->buf; p + 1 < sb->buf + cutoff; p++) {
		എങ്കിൽ ((p[0] == '\n' || p[0] == '\r') && p[1]) {
			candidate = strchr(candidates, p[1]);
			എങ്കിൽ (candidate)
				*candidate = ' ';
		}
	}

	വേണ്ടി (p = candidates; *p == ' '; p++)
		;
	എങ്കിൽ (!*p)
		die(_("unable to select a comment character that is not used\n"
		      "in the current commit message"));
	സ്വതന്ത്രം(comment_വരി_str_to_free);
	comment_വരി_str = comment_വരി_str_to_free = xstrfmt("%c", *p);
}
#endif /* !WITH_BREAKING_CHANGES */

സ്ഥിതം ശൂന്യം prepare_amend_commit(ഘടന commit *commit, ഘടന strbuf *sb,
				ഘടന pretty_print_context *ctx)
{
	സ്ഥിരം അക്ഷരം *buffer, *subject, *fmt;

	buffer = repo_get_commit_buffer(the_repository, commit, ശൂന്യ);
	find_commit_subject(buffer, &subject);
	/*
	 * If we amend the 'amend!' commit then we don't want to
	 * duplicate the subject line.
	 */
	fmt = starts_with(subject, "amend!") ? "%b" : "%B";
	repo_format_commit_message(the_repository, commit, fmt, sb, ctx);
	repo_unuse_commit_buffer(the_repository, commit, buffer);
}

സ്ഥിതം ശൂന്യം change_data_free(ശൂന്യം *util, സ്ഥിരം അക്ഷരം *str UNUSED)
{
	ഘടന wt_status_change_data *d = util;
	സ്വതന്ത്രം(d->rename_source);
	സ്വതന്ത്രം(d);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ prepare_to_commit(സ്ഥിരം അക്ഷരം *index_file, സ്ഥിരം അക്ഷരം *prefix,
			     ഘടന commit *current_head,
			     ഘടന wt_status *s,
			     ഘടന strbuf *author_ident)
{
	ഘടന stat statbuf;
	ഘടന strbuf committer_ident = STRBUF_INIT;
	പൂർണ്ണസംഖ്യ committable;
	ഘടന strbuf sb = STRBUF_INIT;
	സ്ഥിരം അക്ഷരം *hook_arg1 = ശൂന്യ;
	സ്ഥിരം അക്ഷരം *hook_arg2 = ശൂന്യ;
	പൂർണ്ണസംഖ്യ clean_message_contents = (cleanup_mode != COMMIT_സന്ദേശം_CLEANUP_NONE);
	പൂർണ്ണസംഖ്യ old_display_comment_prefix;
	പൂർണ്ണസംഖ്യ invoked_hook;

	/* This checks and barfs if author is badly specified */
	determine_author_info(author_ident);

	എങ്കിൽ (!no_verify && run_commit_hook(use_editor, index_file, &invoked_hook,
					  "pre-commit", ശൂന്യ))
		തിരികെ 0;

	എങ്കിൽ (squash_message) {
		/*
		 * Insert the proper subject line before other commit
		 * message options add their content.
		 */
		എങ്കിൽ (use_message && !strcmp(use_message, squash_message))
			strbuf_addstr(&sb, "squash! ");
		അല്ലെങ്കിൽ {
			ഘടന pretty_print_context ctx = {0};
			ഘടന commit *c;
			c = lookup_commit_reference_by_name(squash_message);
			എങ്കിൽ (!c)
				die(_("could not lookup commit '%s'"), squash_message);
			ctx.output_encoding = get_commit_output_encoding();
			repo_format_commit_message(the_repository, c,
						   "squash! %s\n\n", &sb,
						   &ctx);
		}
	}

	എങ്കിൽ (have_option_m && !fixup_message) {
		strbuf_addbuf(&sb, &message);
		hook_arg1 = "message";
	} അല്ലെങ്കിൽ എങ്കിൽ (logfile && !strcmp(logfile, "-")) {
		എങ്കിൽ (isatty(0))
			fprintf(stderr, _("(reading log message from standard input)\n"));
		എങ്കിൽ (strbuf_read(&sb, 0, 0) < 0)
			die_errno(_("could not read log from standard input"));
		hook_arg1 = "message";
	} അല്ലെങ്കിൽ എങ്കിൽ (logfile) {
		എങ്കിൽ (strbuf_read_file(&sb, logfile, 0) < 0)
			die_errno(_("could not read log file '%s'"),
				  logfile);
		hook_arg1 = "message";
	} അല്ലെങ്കിൽ എങ്കിൽ (use_message) {
		സ്ഥിരം അക്ഷരം *buffer;
		buffer = strstr(use_message_buffer, "\n\n");
		എങ്കിൽ (buffer)
			strbuf_addstr(&sb, skip_blank_വരികൾ(buffer + 2));
		hook_arg1 = "commit";
		hook_arg2 = use_message;
	} അല്ലെങ്കിൽ എങ്കിൽ (fixup_message) {
		ഘടന pretty_print_context ctx = {0};
		ഘടന commit *commit;
		അക്ഷരം *fmt;
		commit = lookup_commit_reference_by_name(fixup_commit);
		എങ്കിൽ (!commit)
			die(_("could not lookup commit '%s'"), fixup_commit);
		ctx.output_encoding = get_commit_output_encoding();
		fmt = xstrfmt("%s! %%s\n\n", fixup_prefix);
		repo_format_commit_message(the_repository, commit, fmt, &sb,
					   &ctx);
		സ്വതന്ത്രം(fmt);
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
		എങ്കിൽ (have_option_m && !strcmp(fixup_prefix, "fixup"))
			strbuf_addbuf(&sb, &message);

		എങ്കിൽ (!strcmp(fixup_prefix, "amend")) {
			എങ്കിൽ (have_option_m)
				die(_("options '%s' and '%s:%s' cannot be used together"), "-m", "--fixup", fixup_message);
			prepare_amend_commit(commit, &sb, &ctx);
		}
	} അല്ലെങ്കിൽ എങ്കിൽ (!stat(git_path_merge_സന്ദേശം(the_repository), &statbuf)) {
		size_t merge_സന്ദേശം_start;

		/*
		 * prepend SQUASH_MSG here if it exists and a
		 * "merge --squash" was originally performed
		 */
		എങ്കിൽ (!stat(git_path_squash_സന്ദേശം(the_repository), &statbuf)) {
			എങ്കിൽ (strbuf_read_file(&sb, git_path_squash_സന്ദേശം(the_repository), 0) < 0)
				die_errno(_("could not read SQUASH_MSG"));
			hook_arg1 = "squash";
		} അല്ലെങ്കിൽ
			hook_arg1 = "merge";

		merge_സന്ദേശം_start = sb.len;
		എങ്കിൽ (strbuf_read_file(&sb, git_path_merge_സന്ദേശം(the_repository), 0) < 0)
			die_errno(_("could not read MERGE_MSG"));

		എങ്കിൽ (cleanup_mode == COMMIT_സന്ദേശം_CLEANUP_SCISSORS &&
		    wt_status_locate_end(sb.buf + merge_സന്ദേശം_start,
					 sb.len - merge_സന്ദേശം_start) <
				sb.len - merge_സന്ദേശം_start)
			s->added_cut_വരി = 1;
	} അല്ലെങ്കിൽ എങ്കിൽ (!stat(git_path_squash_സന്ദേശം(the_repository), &statbuf)) {
		എങ്കിൽ (strbuf_read_file(&sb, git_path_squash_സന്ദേശം(the_repository), 0) < 0)
			die_errno(_("could not read SQUASH_MSG"));
		hook_arg1 = "squash";
	} അല്ലെങ്കിൽ എങ്കിൽ (template_file) {
		എങ്കിൽ (strbuf_read_file(&sb, template_file, 0) < 0)
			die_errno(_("could not read '%s'"), template_file);
		hook_arg1 = "template";
		clean_message_contents = 0;
	}

	/*
	 * The remaining cases don't modify the template message, but
	 * just set the argument(s) to the prepare-commit-msg hook.
	 */
	അല്ലെങ്കിൽ എങ്കിൽ (whence == FROM_MERGE)
		hook_arg1 = "merge";
	അല്ലെങ്കിൽ എങ്കിൽ (is_from_cherry_pick(whence) || whence == FROM_REBASE_PICK) {
		hook_arg1 = "commit";
		hook_arg2 = "CHERRY_PICK_HEAD";
	}

	എങ്കിൽ (squash_message) {
		/*
		 * If squash_commit was used for the commit subject,
		 * then we're possibly hijacking other commit log options.
		 * Reset the hook args to tell the real story.
		 */
		hook_arg1 = "message";
		hook_arg2 = "";
	}

	s->fp = fopen_for_writing(git_path_commit_editmsg());
	എങ്കിൽ (!s->fp)
		die_errno(_("could not open '%s'"), git_path_commit_editmsg());

	/* Ignore status.displayCommentPrefix: we do need comments in COMMIT_EDITMSG. */
	old_display_comment_prefix = s->display_comment_prefix;
	s->display_comment_prefix = 1;

	/*
	 * Most hints are counter-productive when the commit has
	 * already started.
	 */
	s->hints = 0;

	എങ്കിൽ (clean_message_contents)
		strbuf_stripspace(&sb, ശൂന്യ);

	എങ്കിൽ (signoff)
		append_signoff(&sb, ignored_log_message_bytes(sb.buf, sb.len), 0);

	എങ്കിൽ (fwrite(sb.buf, 1, sb.len, s->fp) < sb.len)
		die_errno(_("could not write commit template"));

#ifndef WITH_BREAKING_CHANGES
	എങ്കിൽ (auto_comment_വരി_char)
		adjust_comment_വരി_char(&sb);
#endif /* !WITH_BREAKING_CHANGES */
	strbuf_release(&sb);

	/* This checks if committer ident is explicitly given */
	strbuf_addstr(&committer_ident, git_committer_info(IDENT_STRICT));
	എങ്കിൽ (use_editor && include_status) {
		പൂർണ്ണസംഖ്യ ident_shown = 0;
		എണ്ണൽ git_colorbool saved_color_setting;
		ഘടന ident_split ci, ai;
		സ്ഥിരം അക്ഷരം *hint_cleanup_all = allow_empty_message ?
			_("Please enter the commit message for your changes."
			  " Lines starting\nwith '%s' will be ignored.\n") :
			_("Please enter the commit message for your changes."
			  " Lines starting\nwith '%s' will be ignored, and an empty"
			  " message aborts the commit.\n");
		സ്ഥിരം അക്ഷരം *hint_cleanup_space = allow_empty_message ?
			_("Please enter the commit message for your changes."
			  " Lines starting\n"
			  "with '%s' will be kept; you may remove them"
			  " yourself if you want to.\n") :
			_("Please enter the commit message for your changes."
			  " Lines starting\n"
			  "with '%s' will be kept; you may remove them"
			  " yourself if you want to.\n"
			  "An empty message aborts the commit.\n");
		എങ്കിൽ (whence != FROM_COMMIT) {
			എങ്കിൽ (cleanup_mode == COMMIT_സന്ദേശം_CLEANUP_SCISSORS)
				wt_status_add_cut_വരി(s);
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
		എങ്കിൽ (cleanup_mode == COMMIT_സന്ദേശം_CLEANUP_ALL)
			status_printf(s, GIT_COLOR_NORMAL, hint_cleanup_all, comment_വരി_str);
		അല്ലെങ്കിൽ എങ്കിൽ (cleanup_mode == COMMIT_സന്ദേശം_CLEANUP_SCISSORS) {
			എങ്കിൽ (whence == FROM_COMMIT)
				wt_status_add_cut_വരി(s);
		} അല്ലെങ്കിൽ /* COMMIT_MSG_CLEANUP_SPACE, that is. */
			status_printf(s, GIT_COLOR_NORMAL, hint_cleanup_space, comment_വരി_str);

		/*
		 * These should never fail because they come from our own
		 * fmt_ident. They may fail the sane_ident test, but we know
		 * that the name and mail pointers will at least be valid,
		 * which is enough for our tests and printing here.
		 */
		assert_split_ident(&ai, author_ident);
		assert_split_ident(&ci, &committer_ident);

		എങ്കിൽ (ident_cmp(&ai, &ci))
			status_printf_ln(s, GIT_COLOR_NORMAL,
				_("%s"
				"Author:    %.*s <%.*s>"),
				ident_shown++ ? "" : "\n",
				(പൂർണ്ണസംഖ്യ)(ai.name_end - ai.name_begin), ai.name_begin,
				(പൂർണ്ണസംഖ്യ)(ai.mail_end - ai.mail_begin), ai.mail_begin);

		എങ്കിൽ (author_date_is_interesting())
			status_printf_ln(s, GIT_COLOR_NORMAL,
				_("%s"
				"Date:      %s"),
				ident_shown++ ? "" : "\n",
				show_ident_date(&ai, DATE_MODE(NORMAL)));

		എങ്കിൽ (!committer_ident_sufficiently_given())
			status_printf_ln(s, GIT_COLOR_NORMAL,
				_("%s"
				"Committer: %.*s <%.*s>"),
				ident_shown++ ? "" : "\n",
				(പൂർണ്ണസംഖ്യ)(ci.name_end - ci.name_begin), ci.name_begin,
				(പൂർണ്ണസംഖ്യ)(ci.mail_end - ci.mail_begin), ci.mail_begin);

		status_printf_ln(s, GIT_COLOR_NORMAL, "%s", ""); /* Add new line for clarity */

		saved_color_setting = s->use_color;
		s->use_color = GIT_COLOR_NEVER;
		committable = run_status(s->fp, index_file, prefix, 1, s);
		s->use_color = saved_color_setting;
		string_list_clear_func(&s->change, change_data_free);
	} അല്ലെങ്കിൽ {
		ഘടന object_id oid;
		സ്ഥിരം അക്ഷരം *parent = "HEAD";

		എങ്കിൽ (!the_repository->index->initialized && repo_read_index(the_repository) < 0)
			die(_("Cannot read index"));

		എങ്കിൽ (amend)
			parent = "HEAD^1";

		എങ്കിൽ (repo_get_oid(the_repository, parent, &oid)) {
			പൂർണ്ണസംഖ്യ i, ita_nr = 0;

			/* TODO: audit for interaction with sparse-index. */
			ensure_full_index(the_repository->index);
			വേണ്ടി (i = 0; i < the_repository->index->cache_nr; i++)
				എങ്കിൽ (ce_intent_to_add(the_repository->index->cache[i]))
					ita_nr++;
			committable = the_repository->index->cache_nr > ita_nr;
		} അല്ലെങ്കിൽ {
			/*
			 * Unless the user did explicitly request a submodule
			 * ignore mode by passing a command line option we do
			 * not ignore any changed submodule SHA-1s when
			 * comparing index and parent, no matter what is
			 * configured. Otherwise we won't commit any
			 * submodules which were manually staged, which would
			 * be really confusing.
			 */
			ഘടന diff_flags flags = DIFF_FLAGS_INIT;
			flags.override_submodule_config = 1;
			എങ്കിൽ (ignore_submodule_arg &&
			    !strcmp(ignore_submodule_arg, "all"))
				flags.ignore_submodules = 1;
			committable = index_differs_from(the_repository,
							 parent, &flags, 1);
		}
	}
	strbuf_release(&committer_ident);

	fclose(s->fp);

	എങ്കിൽ (trailer_args.nr) {
		എങ്കിൽ (amend_file_with_trailers(git_path_commit_editmsg(), &trailer_args))
			die(_("unable to pass trailers to --trailers"));
		strvec_clear(&trailer_args);
	}

	/*
	 * Reject an attempt to record a non-merge empty commit without
	 * explicit --allow-empty. In the cherry-pick case, it may be
	 * empty due to conflict resolution, which the user should okay.
	 */
	എങ്കിൽ (!committable && whence != FROM_MERGE && !allow_empty &&
	    !(amend && is_a_merge(current_head))) {
		s->hints = advice_enabled(ADVICE_STATUS_HINTS);
		s->display_comment_prefix = old_display_comment_prefix;
		run_status(stdout, index_file, prefix, 0, s);
		എങ്കിൽ (amend)
			fputs(_(empty_amend_advice), stderr);
		അല്ലെങ്കിൽ എങ്കിൽ (is_from_cherry_pick(whence) ||
			 whence == FROM_REBASE_PICK) {
			fputs(_(empty_cherry_pick_advice), stderr);
			എങ്കിൽ (whence == FROM_CHERRY_PICK_SINGLE)
				fputs(_(empty_cherry_pick_advice_single), stderr);
			അല്ലെങ്കിൽ എങ്കിൽ (whence == FROM_CHERRY_PICK_MULTI)
				fputs(_(empty_cherry_pick_advice_multi), stderr);
			അല്ലെങ്കിൽ
				fputs(_(empty_rebase_pick_advice), stderr);
		}
		തിരികെ 0;
	}

	എങ്കിൽ (!no_verify && invoked_hook) {
		/*
		 * Re-read the index as the pre-commit-commit hook was invoked
		 * and could have updated it. We must do this before we invoke
		 * the editor and after we invoke run_status above.
		 */
		discard_index(the_repository->index);
	}
	read_index_from(the_repository->index, index_file, repo_get_git_dir(the_repository));

	എങ്കിൽ (cache_വൃക്ഷം_update(the_repository->index, 0)) {
		error(_("Error building trees"));
		തിരികെ 0;
	}

	എങ്കിൽ (run_commit_hook(use_editor, index_file, ശൂന്യ, "prepare-commit-msg",
			    git_path_commit_editmsg(), hook_arg1, hook_arg2, ശൂന്യ))
		തിരികെ 0;

	എങ്കിൽ (use_editor) {
		ഘടന strvec env = STRVEC_INIT;

		strvec_pushf(&env, "GIT_INDEX_FILE=%s", index_file);
		എങ്കിൽ (launch_editor(git_path_commit_editmsg(), ശൂന്യ, env.v)) {
			fprintf(stderr,
			_("Please supply the message using either -m or -F option.\n"));
			exit(1);
		}
		strvec_clear(&env);
	}

	എങ്കിൽ (!no_verify &&
	    run_commit_hook(use_editor, index_file, ശൂന്യ, "commit-msg",
			    git_path_commit_editmsg(), ശൂന്യ)) {
		തിരികെ 0;
	}

	തിരികെ 1;
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *find_author_by_nickname(സ്ഥിരം അക്ഷരം *name)
{
	ഘടന rev_info revs;
	ഘടന commit *commit;
	ഘടന strbuf buf = STRBUF_INIT;
	സ്ഥിരം അക്ഷരം *av[20];
	പൂർണ്ണസംഖ്യ ac = 0;

	repo_init_revisions(the_repository, &revs, ശൂന്യ);
	strbuf_addf(&buf, "--author=%s", name);
	av[++ac] = "--all";
	av[++ac] = "-i";
	av[++ac] = buf.buf;
	av[++ac] = ശൂന്യ;
	setup_revisions(ac, av, &revs, ശൂന്യ);
	revs.mailmap = xmalloc(വലുപ്പം(ഘടന string_list));
	string_list_init_nodup(revs.mailmap);
	read_mailmap(the_repository, revs.mailmap);

	എങ്കിൽ (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	commit = get_revision(&revs);
	എങ്കിൽ (commit) {
		ഘടന pretty_print_context ctx = {0};
		ctx.date_mode.type = DATE_NORMAL;
		strbuf_release(&buf);
		repo_format_commit_message(the_repository, commit,
					   "%aN <%aE>", &buf, &ctx);
		release_revisions(&revs);
		തിരികെ strbuf_detach(&buf, ശൂന്യ);
	}
	die(_("--author '%s' is not 'Name <email>' and matches no existing author"), name);
}

സ്ഥിതം ശൂന്യം handle_ignored_arg(ഘടന wt_status *s)
{
	എങ്കിൽ (!ignored_arg)
		; /* default already initialized */
	അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(ignored_arg, "traditional"))
		s->show_ignored_mode = SHOW_TRADITIONAL_IGNORED;
	അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(ignored_arg, "no"))
		s->show_ignored_mode = SHOW_NO_IGNORED;
	അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(ignored_arg, "matching"))
		s->show_ignored_mode = SHOW_MATCHING_IGNORED;
	അല്ലെങ്കിൽ
		die(_("Invalid ignored mode '%s'"), ignored_arg);
}

സ്ഥിതം എണ്ണൽ untracked_status_type parse_untracked_setting_name(സ്ഥിരം അക്ഷരം *u)
{
	/*
	 * Please update $__git_untracked_file_modes in
	 * git-completion.bash when you add new options
	 */
	തിരഞ്ഞെടുപ്പ് (git_parse_maybe_bool(u)) {
	അവസ്ഥ 0:
		u = "no";
		നിർത്തുക;
	അവസ്ഥ 1:
		u = "normal";
		നിർത്തുക;
	സ്ഥിരസ്ഥിതി:
		നിർത്തുക;
	}

	എങ്കിൽ (!strcmp(u, "no"))
		തിരികെ SHOW_NO_UNTRACKED_FILES;
	അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(u, "normal"))
		തിരികെ SHOW_NORMAL_UNTRACKED_FILES;
	അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(u, "all"))
		തിരികെ SHOW_ALL_UNTRACKED_FILES;
	അല്ലെങ്കിൽ
		തിരികെ SHOW_UNTRACKED_FILES_ERROR;
}

സ്ഥിതം ശൂന്യം handle_untracked_files_arg(ഘടന wt_status *s)
{
	എണ്ണൽ untracked_status_type u;

	എങ്കിൽ (!untracked_files_arg)
		തിരികെ; /* default already initialized */

	u = parse_untracked_setting_name(untracked_files_arg);
	എങ്കിൽ (u == SHOW_UNTRACKED_FILES_ERROR)
		die(_("Invalid untracked files mode '%s'"),
		    untracked_files_arg);
	s->show_untracked_files = u;
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *read_commit_message(സ്ഥിരം അക്ഷരം *name)
{
	സ്ഥിരം അക്ഷരം *out_enc;
	ഘടന commit *commit;

	commit = lookup_commit_reference_by_name(name);
	എങ്കിൽ (!commit)
		die(_("could not lookup commit '%s'"), name);
	out_enc = get_commit_output_encoding();
	തിരികെ repo_logmsg_reencode(the_repository, commit, ശൂന്യ, out_enc);
}

/*
 * Enumerate what needs to be propagated when --porcelain
 * is not in effect here.
 */
സ്ഥിതം ഘടന status_deferred_config {
	എണ്ണൽ wt_status_format status_format;
	പൂർണ്ണസംഖ്യ show_branch;
	എണ്ണൽ ahead_behind_flags ahead_behind;
} status_deferred_config = {
	STATUS_FORMAT_UNSPECIFIED,
	-1, /* unspecified */
	AHEAD_BEHIND_UNSPECIFIED,
};

സ്ഥിതം ശൂന്യം finalize_deferred_config(ഘടന wt_status *s)
{
	പൂർണ്ണസംഖ്യ use_deferred_config = (status_format != STATUS_FORMAT_PORCELAIN &&
				   status_format != STATUS_FORMAT_PORCELAIN_V2 &&
				   !s->null_termination);

	എങ്കിൽ (s->null_termination) {
		എങ്കിൽ (status_format == STATUS_FORMAT_NONE ||
		    status_format == STATUS_FORMAT_UNSPECIFIED)
			status_format = STATUS_FORMAT_PORCELAIN;
		അല്ലെങ്കിൽ എങ്കിൽ (status_format == STATUS_FORMAT_LONG)
			die(_("options '%s' and '%s' cannot be used together"), "--long", "-z");
	}

	എങ്കിൽ (use_deferred_config && status_format == STATUS_FORMAT_UNSPECIFIED)
		status_format = status_deferred_config.status_format;
	എങ്കിൽ (status_format == STATUS_FORMAT_UNSPECIFIED)
		status_format = STATUS_FORMAT_NONE;

	എങ്കിൽ (use_deferred_config && s->show_branch < 0)
		s->show_branch = status_deferred_config.show_branch;
	എങ്കിൽ (s->show_branch < 0)
		s->show_branch = 0;

	/*
	 * If the user did not give a "--[no]-ahead-behind" command
	 * line argument *AND* we will print in a human-readable format
	 * (short, long etc.) then we inherit from the status.aheadbehind
	 * config setting.  In all other cases (and porcelain V[12] formats
	 * in particular), we inherit _FULL for backwards compatibility.
	 */
	എങ്കിൽ (use_deferred_config &&
	    s->ahead_behind_flags == AHEAD_BEHIND_UNSPECIFIED)
		s->ahead_behind_flags = status_deferred_config.ahead_behind;

	എങ്കിൽ (s->ahead_behind_flags == AHEAD_BEHIND_UNSPECIFIED)
		s->ahead_behind_flags = AHEAD_BEHIND_FULL;
}

സ്ഥിതം ശൂന്യം check_fixup_reword_options(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം *argv[]) {
	എങ്കിൽ (whence != FROM_COMMIT) {
		എങ്കിൽ (whence == FROM_MERGE)
			die(_("You are in the middle of a merge -- cannot reword."));
		അല്ലെങ്കിൽ എങ്കിൽ (is_from_cherry_pick(whence))
			die(_("You are in the middle of a cherry-pick -- cannot reword."));
	}
	എങ്കിൽ (argc)
		die(_("reword option of '%s' and path '%s' cannot be used together"), "--fixup", *argv);
	എങ്കിൽ (patch_interactive || interactive || all || also || only)
		die(_("reword option of '%s' and '%s' cannot be used together"),
			"--fixup", "--patch/--interactive/--all/--include/--only");
}

സ്ഥിതം പൂർണ്ണസംഖ്യ parse_and_validate_options(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം *argv[],
				      സ്ഥിരം ഘടന option *options,
				      സ്ഥിരം അക്ഷരം * സ്ഥിരം usage[],
				      സ്ഥിരം അക്ഷരം *prefix,
				      ഘടന commit *current_head,
				      ഘടന wt_status *s)
{
	argc = parse_options(argc, argv, prefix, options, usage, 0);
	finalize_deferred_config(s);

	എങ്കിൽ (force_author && !strchr(force_author, '>'))
		force_author = find_author_by_nickname(force_author);

	എങ്കിൽ (force_author && renew_authorship)
		die(_("options '%s' and '%s' cannot be used together"), "--reset-author", "--author");

	എങ്കിൽ (logfile || have_option_m || use_message)
		use_editor = 0;

	/* Sanity check options */
	എങ്കിൽ (amend && !current_head)
		die(_("You have nothing to amend."));
	എങ്കിൽ (amend && whence != FROM_COMMIT) {
		എങ്കിൽ (whence == FROM_MERGE)
			die(_("You are in the middle of a merge -- cannot amend."));
		അല്ലെങ്കിൽ എങ്കിൽ (is_from_cherry_pick(whence))
			die(_("You are in the middle of a cherry-pick -- cannot amend."));
		അല്ലെങ്കിൽ എങ്കിൽ (whence == FROM_REBASE_PICK)
			die(_("You are in the middle of a rebase -- cannot amend."));
	}
	എങ്കിൽ (fixup_message && squash_message)
		die(_("options '%s' and '%s' cannot be used together"), "--squash", "--fixup");
	die_for_incompatible_opt4(!!use_message, "-C",
				  !!edit_message, "-c",
				  !!logfile, "-F",
				  !!fixup_message, "--fixup");
	die_for_incompatible_opt4(have_option_m, "-m",
				  !!edit_message, "-c",
				  !!use_message, "-C",
				  !!logfile, "-F");
	എങ്കിൽ (use_message || edit_message || logfile ||fixup_message || have_option_m)
		FREE_AND_NULL(template_file);
	എങ്കിൽ (edit_message)
		use_message = edit_message;
	എങ്കിൽ (amend && !use_message && !fixup_message)
		use_message = "HEAD";
	എങ്കിൽ (!use_message && !is_from_cherry_pick(whence) &&
	    !is_from_rebase(whence) && renew_authorship)
		die(_("--reset-author can be used only with -C, -c or --amend."));
	എങ്കിൽ (use_message) {
		use_message_buffer = read_commit_message(use_message);
		എങ്കിൽ (!renew_authorship) {
			author_message = use_message;
			author_message_buffer = use_message_buffer;
		}
	}
	എങ്കിൽ ((is_from_cherry_pick(whence) || whence == FROM_REBASE_PICK) &&
	    !renew_authorship) {
		author_message = "CHERRY_PICK_HEAD";
		author_message_buffer = read_commit_message(author_message);
	}

	എങ്കിൽ (patch_interactive)
		interactive = 1;

	die_for_incompatible_opt4(also, "-i/--include",
				  only, "-o/--only",
				  all, "-a/--all",
				  interactive, "--interactive/-p/--patch");
	എങ്കിൽ (fixup_message) {
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
		അക്ഷരം *p = fixup_message;
		ആകുമ്പോൾ (isalpha(*p))
			p++;
		എങ്കിൽ (p > fixup_message && *p == ':') {
			*p = '\0';
			fixup_commit = p + 1;
			എങ്കിൽ (!strcmp("amend", fixup_message) ||
			    !strcmp("reword", fixup_message)) {
				fixup_prefix = "amend";
				allow_empty = 1;
				എങ്കിൽ (*fixup_message == 'r') {
					check_fixup_reword_options(argc, argv);
					only = 1;
				}
			} അല്ലെങ്കിൽ {
				die(_("unknown option: --fixup=%s:%s"), fixup_message, fixup_commit);
			}
		} അല്ലെങ്കിൽ {
			fixup_commit = fixup_message;
			fixup_prefix = "fixup";
			use_editor = 0;
		}
	}

	എങ്കിൽ (0 <= edit_flag)
		use_editor = edit_flag;

	handle_untracked_files_arg(s);

	എങ്കിൽ (all && argc > 0)
		die(_("paths '%s ...' with -a does not make sense"),
		    argv[0]);

	എങ്കിൽ (status_format != STATUS_FORMAT_NONE)
		dry_run = 1;

	തിരികെ argc;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ dry_run_commit(സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
			  സ്ഥിരം ഘടന commit *current_head, ഘടന wt_status *s)
{
	പൂർണ്ണസംഖ്യ committable;
	സ്ഥിരം അക്ഷരം *index_file;

	index_file = prepare_index(argv, prefix, current_head, 1);
	committable = run_status(stdout, index_file, prefix, 0, s);
	rollback_index_files();

	തിരികെ committable ? 0 : 1;
}

define_list_config_array_extra(color_status_slots, {"added"});

സ്ഥിതം പൂർണ്ണസംഖ്യ parse_status_slot(സ്ഥിരം അക്ഷരം *slot)
{
	എങ്കിൽ (!strcasecmp(slot, "added"))
		തിരികെ WT_STATUS_UPDATED;

	തിരികെ LOOKUP_CONFIG(color_status_slots, slot);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ git_status_config(സ്ഥിരം അക്ഷരം *k, സ്ഥിരം അക്ഷരം *v,
			     സ്ഥിരം ഘടന config_context *ctx, ശൂന്യം *cb)
{
	ഘടന wt_status *s = cb;
	സ്ഥിരം അക്ഷരം *slot_name;

	എങ്കിൽ (starts_with(k, "column."))
		തിരികെ git_column_config(k, v, "status", &s->colopts);
	എങ്കിൽ (!strcmp(k, "status.submodulesummary")) {
		പൂർണ്ണസംഖ്യ is_bool;
		s->submodule_summary = git_config_bool_or_int(k, v, ctx->kvi,
							      &is_bool);
		എങ്കിൽ (is_bool && s->submodule_summary)
			s->submodule_summary = -1;
		തിരികെ 0;
	}
	എങ്കിൽ (!strcmp(k, "status.short")) {
		എങ്കിൽ (git_config_bool(k, v))
			status_deferred_config.status_format = STATUS_FORMAT_SHORT;
		അല്ലെങ്കിൽ
			status_deferred_config.status_format = STATUS_FORMAT_NONE;
		തിരികെ 0;
	}
	എങ്കിൽ (!strcmp(k, "status.branch")) {
		status_deferred_config.show_branch = git_config_bool(k, v);
		തിരികെ 0;
	}
	എങ്കിൽ (!strcmp(k, "status.aheadbehind")) {
		status_deferred_config.ahead_behind = git_config_bool(k, v);
		തിരികെ 0;
	}
	എങ്കിൽ (!strcmp(k, "status.showstash")) {
		s->show_stash = git_config_bool(k, v);
		തിരികെ 0;
	}
	എങ്കിൽ (!strcmp(k, "status.color") || !strcmp(k, "color.status")) {
		s->use_color = git_config_colorbool(k, v);
		തിരികെ 0;
	}
	എങ്കിൽ (!strcmp(k, "status.displaycommentprefix")) {
		s->display_comment_prefix = git_config_bool(k, v);
		തിരികെ 0;
	}
	എങ്കിൽ (skip_prefix(k, "status.color.", &slot_name) ||
	    skip_prefix(k, "color.status.", &slot_name)) {
		പൂർണ്ണസംഖ്യ slot = parse_status_slot(slot_name);
		എങ്കിൽ (slot < 0)
			തിരികെ 0;
		എങ്കിൽ (!v)
			തിരികെ config_error_nonbool(k);
		തിരികെ color_parse(v, s->color_palette[slot]);
	}
	എങ്കിൽ (!strcmp(k, "status.relativepaths")) {
		s->relative_paths = git_config_bool(k, v);
		തിരികെ 0;
	}
	എങ്കിൽ (!strcmp(k, "status.showuntrackedfiles")) {
		എണ്ണൽ untracked_status_type u;

		u = parse_untracked_setting_name(v);
		എങ്കിൽ (u == SHOW_UNTRACKED_FILES_ERROR)
			തിരികെ error(_("Invalid untracked files mode '%s'"), v);
		s->show_untracked_files = u;
		തിരികെ 0;
	}
	എങ്കിൽ (!strcmp(k, "diff.renamelimit")) {
		എങ്കിൽ (s->rename_limit == -1)
			s->rename_limit = git_config_int(k, v, ctx->kvi);
		തിരികെ 0;
	}
	എങ്കിൽ (!strcmp(k, "status.renamelimit")) {
		s->rename_limit = git_config_int(k, v, ctx->kvi);
		തിരികെ 0;
	}
	എങ്കിൽ (!strcmp(k, "diff.renames")) {
		എങ്കിൽ (s->detect_rename == -1)
			s->detect_rename = git_config_rename(k, v);
		തിരികെ 0;
	}
	എങ്കിൽ (!strcmp(k, "status.renames")) {
		s->detect_rename = git_config_rename(k, v);
		തിരികെ 0;
	}
	തിരികെ git_diff_ui_config(k, v, ctx, ശൂന്യ);
}

പൂർണ്ണസംഖ്യ cmd_status(പൂർണ്ണസംഖ്യ argc,
സ്ഥിരം അക്ഷരം **argv,
സ്ഥിരം അക്ഷരം *prefix,
ഘടന repository *repo UNUSED)
{
	സ്ഥിതം പൂർണ്ണസംഖ്യ no_renames = -1;
	സ്ഥിതം സ്ഥിരം അക്ഷരം *rename_score_arg = (സ്ഥിരം അക്ഷരം *)-1;
	സ്ഥിതം ഘടന wt_status s;
	unsigned പൂർണ്ണസംഖ്യ progress_flag = 0;
	പൂർണ്ണസംഖ്യ fd;
	ഘടന object_id oid;
	സ്ഥിതം ഘടന option builtin_status_options[] = {
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
			.മൂല്യം = &untracked_files_arg,
			.argh = N_("mode"),
			.help = N_("show untracked files, optional modes: all, normal, no. (Default: all)"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t)"all",
		},
		{
			.type = OPTION_STRING,
			.long_name = "ignored",
			.മൂല്യം = &ignored_arg,
			.argh = N_("mode"),
			.help = N_("show ignored files, optional modes: traditional, matching, no. (Default: traditional)"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t)"traditional",
		},
		{
			.type = OPTION_STRING,
			.long_name = "ignore-submodules",
			.മൂല്യം = &ignore_submodule_arg,
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

	എങ്കിൽ (s.show_ignored_mode == SHOW_MATCHING_IGNORED &&
	    s.show_untracked_files == SHOW_NO_UNTRACKED_FILES)
		die(_("Unsupported combination of ignored and untracked-files arguments"));

	parse_pathspec(&s.pathspec, 0,
		       PATHSPEC_PREFER_FULL,
		       prefix, argv);

	എങ്കിൽ (status_format != STATUS_FORMAT_PORCELAIN &&
	    status_format != STATUS_FORMAT_PORCELAIN_V2)
		progress_flag = REFRESH_PROGRESS;
	repo_read_index(the_repository);
	refresh_index(the_repository->index,
		      REFRESH_QUIET|REFRESH_UNMERGED|progress_flag,
		      &s.pathspec, ശൂന്യ, ശൂന്യ);

	എങ്കിൽ (use_optional_locks())
		fd = repo_hold_locked_index(the_repository, &index_lock, 0);
	അല്ലെങ്കിൽ
		fd = -1;

	s.is_initial = repo_get_oid(the_repository, s.reference, &oid) ? 1 : 0;
	എങ്കിൽ (!s.is_initial)
		oidcpy(&s.oid_commit, &oid);

	s.ignore_submodule_arg = ignore_submodule_arg;
	s.status_format = status_format;
	s.verbose = verbose;
	എങ്കിൽ (no_renames != -1)
		s.detect_rename = !no_renames;
	എങ്കിൽ ((intptr_t)rename_score_arg != -1) {
		എങ്കിൽ (s.detect_rename < DIFF_DETECT_RENAME)
			s.detect_rename = DIFF_DETECT_RENAME;
		എങ്കിൽ (rename_score_arg)
			s.rename_score = parse_rename_score(&rename_score_arg);
	}

	wt_status_collect(&s);

	എങ്കിൽ (0 <= fd)
		repo_update_index_if_able(the_repository, &index_lock);

	എങ്കിൽ (s.relative_paths)
		s.prefix = prefix;

	wt_status_print(&s);
	wt_status_collect_free_buffers(&s);

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ git_commit_config(സ്ഥിരം അക്ഷരം *k, സ്ഥിരം അക്ഷരം *v,
			     സ്ഥിരം ഘടന config_context *ctx, ശൂന്യം *cb)
{
	ഘടന wt_status *s = cb;

	എങ്കിൽ (!strcmp(k, "commit.template"))
		തിരികെ git_config_pathname(&template_file, k, v);
	എങ്കിൽ (!strcmp(k, "commit.status")) {
		include_status = git_config_bool(k, v);
		തിരികെ 0;
	}
	എങ്കിൽ (!strcmp(k, "commit.cleanup")) {
		FREE_AND_NULL(cleanup_config);
		തിരികെ git_config_string(&cleanup_config, k, v);
	}
	എങ്കിൽ (!strcmp(k, "commit.gpgsign")) {
		sign_commit = git_config_bool(k, v) ? "" : ശൂന്യ;
		തിരികെ 0;
	}
	എങ്കിൽ (!strcmp(k, "commit.verbose")) {
		പൂർണ്ണസംഖ്യ is_bool;
		config_commit_verbose = git_config_bool_or_int(k, v, ctx->kvi,
							       &is_bool);
		തിരികെ 0;
	}

	തിരികെ git_status_config(k, v, ctx, s);
}

പൂർണ്ണസംഖ്യ cmd_commit(പൂർണ്ണസംഖ്യ argc,
	       സ്ഥിരം അക്ഷരം **argv,
	       സ്ഥിരം അക്ഷരം *prefix,
	       ഘടന repository *repo UNUSED)
{
	സ്ഥിതം ഘടന wt_status s;
	സ്ഥിതം സ്ഥിരം അക്ഷരം *cleanup_arg = ശൂന്യ;
	സ്ഥിതം ഘടന option builtin_commit_options[] = {
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
			.മൂല്യം = &sign_commit,
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
			.മൂല്യം = &untracked_files_arg,
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

	ഘടന strbuf sb = STRBUF_INIT;
	ഘടന strbuf author_ident = STRBUF_INIT;
	സ്ഥിരം അക്ഷരം *index_file, *reflog_സന്ദേശം;
	ഘടന object_id oid;
	ഘടന commit_list *parents = ശൂന്യ;
	ഘടന stat statbuf;
	ഘടന commit *current_head = ശൂന്യ;
	ഘടന commit_extra_header *extra = ശൂന്യ;
	ഘടന strbuf err = STRBUF_INIT;
	പൂർണ്ണസംഖ്യ ret = 0;

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

	എങ്കിൽ (repo_get_oid(the_repository, "HEAD", &oid))
		current_head = ശൂന്യ;
	അല്ലെങ്കിൽ {
		current_head = lookup_commit_or_die(&oid, "HEAD");
		എങ്കിൽ (repo_parse_commit(the_repository, current_head))
			die(_("could not parse HEAD commit"));
	}
	verbose = -1; /* unspecified */
	argc = parse_and_validate_options(argc, argv, builtin_commit_options,
					  builtin_commit_usage,
					  prefix, current_head, &s);
	എങ്കിൽ (trailer_args.nr)
		trailer_config_init();

	എങ്കിൽ (verbose == -1)
		verbose = (config_commit_verbose < 0) ? 0 : config_commit_verbose;

	എങ്കിൽ (cleanup_arg) {
		സ്വതന്ത്രം(cleanup_config);
		cleanup_config = xstrdup(cleanup_arg);
	}
	cleanup_mode = get_cleanup_mode(cleanup_config, use_editor);

	എങ്കിൽ (dry_run)
		തിരികെ dry_run_commit(argv, prefix, current_head, &s);
	index_file = prepare_index(argv, prefix, current_head, 0);

	/* Set up everything for writing the commit object.  This includes
	   running hooks, writing the trees, and interacting with the user.  */
	എങ്കിൽ (!prepare_to_commit(index_file, prefix,
			       current_head, &s, &author_ident)) {
		ret = 1;
		rollback_index_files();
		പോകുക cleanup;
	}

	/* Determine parents */
	reflog_സന്ദേശം = getenv("GIT_REFLOG_ACTION");
	എങ്കിൽ (!current_head) {
		എങ്കിൽ (!reflog_സന്ദേശം)
			reflog_സന്ദേശം = "commit (initial)";
	} അല്ലെങ്കിൽ എങ്കിൽ (amend) {
		എങ്കിൽ (!reflog_സന്ദേശം)
			reflog_സന്ദേശം = "commit (amend)";
		parents = commit_list_copy(current_head->parents);
	} അല്ലെങ്കിൽ എങ്കിൽ (whence == FROM_MERGE) {
		ഘടന strbuf m = STRBUF_INIT;
		FILE *fp;
		പൂർണ്ണസംഖ്യ allow_fast_forward = 1;
		ഘടന commit_list **pptr = &parents;

		എങ്കിൽ (!reflog_സന്ദേശം)
			reflog_സന്ദേശം = "commit (merge)";
		pptr = commit_list_append(current_head, pptr);
		fp = xfopen(git_path_merge_head(the_repository), "r");
		ആകുമ്പോൾ (strbuf_getline_lf(&m, fp) != EOF) {
			ഘടന commit *parent;

			parent = get_merge_parent(m.buf);
			എങ്കിൽ (!parent)
				die(_("Corrupt MERGE_HEAD file (%s)"), m.buf);
			pptr = commit_list_append(parent, pptr);
		}
		fclose(fp);
		strbuf_release(&m);
		എങ്കിൽ (!stat(git_path_merge_mode(the_repository), &statbuf)) {
			എങ്കിൽ (strbuf_read_file(&sb, git_path_merge_mode(the_repository), 0) < 0)
				die_errno(_("could not read MERGE_MODE"));
			എങ്കിൽ (!strcmp(sb.buf, "no-ff"))
				allow_fast_forward = 0;
		}
		എങ്കിൽ (allow_fast_forward)
			reduce_heads_replace(&parents);
	} അല്ലെങ്കിൽ {
		എങ്കിൽ (!reflog_സന്ദേശം)
			reflog_സന്ദേശം = is_from_cherry_pick(whence)
					? "commit (cherry-pick)"
					: is_from_rebase(whence)
					? "commit (rebase)"
					: "commit";
		commit_list_തിരുകുക(current_head, &parents);
	}

	/* Finally, get the commit message */
	strbuf_reset(&sb);
	എങ്കിൽ (strbuf_read_file(&sb, git_path_commit_editmsg(), 0) < 0) {
		പൂർണ്ണസംഖ്യ saved_errno = errno;
		rollback_index_files();
		die(_("could not read commit message: %s"), strerror(saved_errno));
	}

	cleanup_message(&sb, cleanup_mode, verbose);

	എങ്കിൽ (message_is_empty(&sb, cleanup_mode) && !allow_empty_message) {
		rollback_index_files();
		fprintf(stderr, _("Aborting commit due to empty commit message.\n"));
		exit(1);
	}
	എങ്കിൽ (template_untouched(&sb, template_file, cleanup_mode) && !allow_empty_message) {
		rollback_index_files();
		fprintf(stderr, _("Aborting commit; you did not edit the message.\n"));
		exit(1);
	}

	എങ്കിൽ (fixup_message && starts_with(sb.buf, "amend! ") &&
	    !allow_empty_message) {
		ഘടന strbuf body = STRBUF_INIT;
		size_t len = commit_subject_length(sb.buf);
		strbuf_addstr(&body, sb.buf + len);
		എങ്കിൽ (message_is_empty(&body, cleanup_mode)) {
			rollback_index_files();
			fprintf(stderr, _("Aborting commit due to empty commit message body.\n"));
			exit(1);
		}
		strbuf_release(&body);
	}

	എങ്കിൽ (amend) {
		സ്ഥിരം അക്ഷരം *exclude_gpgsig[3] = { "gpgsig", "gpgsig-sha256", ശൂന്യ };
		extra = read_commit_extra_headers(current_head, exclude_gpgsig);
	} അല്ലെങ്കിൽ {
		ഘടന commit_extra_header **tail = &extra;
		append_merge_tag_headers(parents, &tail);
	}

	എങ്കിൽ (commit_വൃക്ഷം_extended(sb.buf, sb.len, &the_repository->index->cache_വൃക്ഷം->oid,
				 parents, &oid, author_ident.buf, ശൂന്യ,
				 sign_commit, extra)) {
		rollback_index_files();
		die(_("failed to write commit object"));
	}

	എങ്കിൽ (update_head_with_reflog(current_head, &oid, reflog_സന്ദേശം, &sb,
				    &err)) {
		rollback_index_files();
		die("%s", err.buf);
	}

	sequencer_post_commit_cleanup(the_repository, 0);
	unlink(git_path_merge_head(the_repository));
	unlink(git_path_merge_സന്ദേശം(the_repository));
	unlink(git_path_merge_mode(the_repository));
	unlink(git_path_squash_സന്ദേശം(the_repository));

	എങ്കിൽ (commit_index_files())
		die(_("repository has been updated, but unable to write\n"
		      "new index file. Check that disk is not full and quota is\n"
		      "not exceeded, and then \"git restore --staged :/\" to recover."));

	git_test_write_commit_graph_or_die(the_repository);

	repo_rerere(the_repository, 0);
	run_auto_maintenance(the_repository, quiet);
	run_commit_hook(use_editor, repo_get_index_file(the_repository),
			ശൂന്യ, "post-commit", ശൂന്യ);
	എങ്കിൽ (amend && !no_post_rewrite) {
		commit_post_rewrite(the_repository, current_head, &oid);
	}
	എങ്കിൽ (!quiet) {
		unsigned പൂർണ്ണസംഖ്യ flags = 0;

		എങ്കിൽ (!current_head)
			flags |= SUMMARY_INITIAL_COMMIT;
		എങ്കിൽ (author_date_is_interesting())
			flags |= SUMMARY_SHOW_AUTHOR_DATE;
		print_commit_summary(the_repository, prefix,
				     &oid, flags);
	}

	apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
			    ശൂന്യ, ശൂന്യ, ശൂന്യ, ശൂന്യ);

cleanup:
	free_commit_extra_headers(extra);
	commit_list_free(parents);
	strbuf_release(&author_ident);
	strbuf_release(&err);
	strbuf_release(&sb);
	സ്വതന്ത്രം(logfile);
	സ്വതന്ത്രം(template_file);
	തിരികെ ret;
}
