/*
 * Builtin "git commit"
 *
 * Copyright (c) 2007 Kristian Høgsberg <krh@redhat.com>
 * Based on git-commit.sh by Junio C Hamano and Linus Torvalds
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "advice.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "lockfile.h"
#अन्तर्भाव "cache-tree.h"
#अन्तर्भाव "color.h"
#अन्तर्भाव "dir.h"
#अन्तर्भाव "editor.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "diff.h"
#अन्तर्भाव "commit.h"
#अन्तर्भाव "add-interactive.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "revision.h"
#अन्तर्भाव "wt-status.h"
#अन्तर्भाव "run-command.h"
#अन्तर्भाव "strbuf.h"
#अन्तर्भाव "object-name.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "path.h"
#अन्तर्भाव "preload-index.h"
#अन्तर्भाव "read-cache.h"
#अन्तर्भाव "repository.h"
#अन्तर्भाव "string-list.h"
#अन्तर्भाव "rerere.h"
#अन्तर्भाव "unpack-trees.h"
#अन्तर्भाव "column.h"
#अन्तर्भाव "sequencer.h"
#अन्तर्भाव "sparse-index.h"
#अन्तर्भाव "mailmap.h"
#अन्तर्भाव "help.h"
#अन्तर्भाव "commit-reach.h"
#अन्तर्भाव "commit-graph.h"
#अन्तर्भाव "pretty.h"
#अन्तर्भाव "trailer.h"

स्थिर ध्रुव वर्ण * ध्रुव builtin_commit_usage[] = {
	N_("git commit [-a | --interactive | --patch] [-s] [-v] [-u[<mode>]] [--amend]\n"
	   "           [--dry-run] [(-c | -C | --squash) <commit> | --fixup [(amend|reword):]<commit>]\n"
	   "           [-F <file> | -m <msg>] [--reset-author] [--allow-empty]\n"
	   "           [--allow-empty-message] [--no-verify] [-e] [--author=<author>]\n"
	   "           [--date=<date>] [--cleanup=<mode>] [--[no-]status]\n"
	   "           [-i | -o] [--pathspec-from-file=<file> [--pathspec-file-nul]]\n"
	   "           [(--trailer <token>[(=|:)<value>])...] [-S[<keyid>]]\n"
	   "           [--] [<pathspec>...]"),
	रिक्त
};

स्थिर ध्रुव वर्ण * ध्रुव builtin_status_usage[] = {
	N_("git status [<options>] [--] [<pathspec>...]"),
	रिक्त
};

स्थिर ध्रुव वर्ण empty_amend_advice[] =
N_("You asked to amend the most recent commit, but doing so would make\n"
"it empty. You can repeat your command with --allow-empty, or you can\n"
"remove the commit entirely with \"git reset HEAD^\".\n");

स्थिर ध्रुव वर्ण empty_cherry_pick_advice[] =
N_("The previous cherry-pick is now empty, possibly due to conflict resolution.\n"
"If you wish to commit it anyway, use:\n"
"\n"
"    git commit --allow-empty\n"
"\n");

स्थिर ध्रुव वर्ण empty_rebase_pick_advice[] =
N_("Otherwise, please use 'git rebase --skip'\n");

स्थिर ध्रुव वर्ण empty_cherry_pick_advice_single[] =
N_("Otherwise, please use 'git cherry-pick --skip'\n");

स्थिर ध्रुव वर्ण empty_cherry_pick_advice_multi[] =
N_("and then use:\n"
"\n"
"    git cherry-pick --continue\n"
"\n"
"to resume cherry-picking the remaining commits.\n"
"If you wish to skip this commit, use:\n"
"\n"
"    git cherry-pick --skip\n"
"\n");

स्थिर ध्रुव वर्ण *color_status_slots[] = {
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

स्थिर ध्रुव वर्ण *use_message_buffer;
स्थिर संरचना lock_file index_lock; /* real index */
स्थिर संरचना lock_file false_lock; /* used only for partial commits */
स्थिर गणना {
	COMMIT_AS_IS = 1,
	COMMIT_NORMAL,
	COMMIT_PARTIAL
} commit_style;

स्थिर ध्रुव वर्ण *force_author;
स्थिर वर्ण *logfile;
स्थिर वर्ण *template_file;
/*
 * The _message variables are commit names from which to take
 * the commit message and/or authorship.
 */
स्थिर ध्रुव वर्ण *author_message, *author_message_buffer;
स्थिर ध्रुव वर्ण *edit_message, *use_message;
स्थिर वर्ण *fixup_message, *fixup_commit, *squash_message;
स्थिर ध्रुव वर्ण *fixup_prefix;
स्थिर पूर्णाङ्क all, also, interactive, patch_interactive, only, amend, signoff;
स्थिर संरचना interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
स्थिर पूर्णाङ्क edit_flag = -1; /* unspecified */
स्थिर पूर्णाङ्क quiet, verbose, no_verify, allow_empty, dry_run, renew_authorship;
स्थिर पूर्णाङ्क config_commit_verbose = -1; /* unspecified */
स्थिर पूर्णाङ्क no_post_rewrite, allow_empty_message, pathspec_file_nul;
स्थिर ध्रुव वर्ण *untracked_files_arg, *force_date, *ignore_submodule_arg, *ignored_arg;
स्थिर ध्रुव वर्ण *sign_commit, *pathspec_from_file;
स्थिर संरचना strvec trailer_args = STRVEC_INIT;

/*
 * The default commit message cleanup mode will remove the lines
 * beginning with # (shell comments) and leading and trailing
 * whitespaces (empty lines or containing only whitespaces)
 * if editor is used, and only the whitespaces if the message
 * is specified explicitly.
 */
स्थिर गणना commit_संदेश_cleanup_mode cleanup_mode;
स्थिर वर्ण *cleanup_config;

स्थिर गणना commit_whence whence;
स्थिर पूर्णाङ्क use_editor = 1, include_status = 1;
स्थिर पूर्णाङ्क have_option_m;
स्थिर संरचना strbuf message = STRBUF_INIT;

स्थिर गणना wt_status_format status_format = STATUS_FORMAT_UNSPECIFIED;

स्थिर पूर्णाङ्क opt_parse_porcelain(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	गणना wt_status_format *मूल्य = (गणना wt_status_format *)opt->मूल्य;
	यदि (unset)
		*मूल्य = STATUS_FORMAT_NONE;
	अन्यथा यदि (!arg)
		*मूल्य = STATUS_FORMAT_PORCELAIN;
	अन्यथा यदि (!म_भेद(arg, "v1") || !म_भेद(arg, "1"))
		*मूल्य = STATUS_FORMAT_PORCELAIN;
	अन्यथा यदि (!म_भेद(arg, "v2") || !म_भेद(arg, "2"))
		*मूल्य = STATUS_FORMAT_PORCELAIN_V2;
	अन्यथा
		die("unsupported porcelain version '%s'", arg);

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क opt_parse_m(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना strbuf *buf = opt->मूल्य;
	यदि (unset) {
		have_option_m = 0;
		strbuf_setlen(buf, 0);
	} अन्यथा {
		have_option_m = 1;
		यदि (buf->len)
			strbuf_addch(buf, '\n');
		strbuf_addstr(buf, arg);
		strbuf_complete_पंक्ति(buf);
	}
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क opt_parse_rename_score(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	ध्रुव वर्ण **मूल्य = opt->मूल्य;

	BUG_ON_OPT_NEG(unset);

	यदि (arg != रिक्त && *arg == '=')
		arg = arg + 1;

	*मूल्य = arg;
	प्रत्यागम 0;
}

स्थिर शून्य determine_whence(संरचना wt_status *s)
{
	यदि (file_exists(git_path_merge_head(the_repository)))
		whence = FROM_MERGE;
	अन्यथा यदि (!sequencer_determine_whence(the_repository, &whence))
		whence = FROM_COMMIT;
	यदि (s)
		s->whence = whence;
}

स्थिर शून्य status_init_config(संरचना wt_status *s, config_fn_t fn)
{
	wt_status_prepare(the_repository, s);
	init_diff_ui_defaults();
	repo_config(the_repository, fn, s);
	determine_whence(s);
	s->hints = advice_enabled(ADVICE_STATUS_HINTS); /* must come after repo_config() */
}

स्थिर शून्य rollback_index_files(शून्य)
{
	विकल्प (commit_style) {
	पक्ष COMMIT_AS_IS:
		भङ्ग; /* nothing to do */
	पक्ष COMMIT_NORMAL:
		rollback_lock_file(&index_lock);
		भङ्ग;
	पक्ष COMMIT_PARTIAL:
		rollback_lock_file(&index_lock);
		rollback_lock_file(&false_lock);
		भङ्ग;
	}
}

स्थिर पूर्णाङ्क commit_index_files(शून्य)
{
	पूर्णाङ्क err = 0;

	विकल्प (commit_style) {
	पक्ष COMMIT_AS_IS:
		भङ्ग; /* nothing to do */
	पक्ष COMMIT_NORMAL:
		err = commit_lock_file(&index_lock);
		भङ्ग;
	पक्ष COMMIT_PARTIAL:
		err = commit_lock_file(&index_lock);
		rollback_lock_file(&false_lock);
		भङ्ग;
	}

	प्रत्यागम err;
}

/*
 * Take a union of paths in the index and the named tree (typically, "HEAD"),
 * and return the paths that match the given pattern in list.
 */
स्थिर पूर्णाङ्क list_paths(संरचना string_list *list, ध्रुव वर्ण *with_वृक्ष,
		      ध्रुव संरचना pathspec *pattern)
{
	पूर्णाङ्क i, ret;
	वर्ण *m;

	यदि (!pattern->nr)
		प्रत्यागम 0;

	m = xcalloc(1, pattern->nr);

	यदि (with_वृक्ष) {
		वर्ण *max_prefix = common_prefix(pattern);
		overlay_वृक्ष_on_index(the_repository->index, with_वृक्ष, max_prefix);
		मुक्त(max_prefix);
	}

	/* TODO: audit for interaction with sparse-index. */
	ensure_full_index(the_repository->index);
	प्रति (i = 0; i < the_repository->index->cache_nr; i++) {
		ध्रुव संरचना cache_entry *ce = the_repository->index->cache[i];
		संरचना string_list_item *item;

		यदि (ce->ce_flags & CE_UPDATE)
			अनुवर्त;
		यदि (!ce_path_match(the_repository->index, ce, pattern, m))
			अनुवर्त;
		item = string_list_जोड़ो(list, ce->name);
		यदि (ce_skip_worktree(ce))
			item->util = item; /* better a valid pointer than a fake one */
	}

	ret = report_path_error(m, pattern);
	मुक्त(m);
	प्रत्यागम ret;
}

स्थिर शून्य add_remove_files(संरचना string_list *list)
{
	पूर्णाङ्क i;
	प्रति (i = 0; i < list->nr; i++) {
		संरचना stat st;
		संरचना string_list_item *p = &(list->वस्तुएँ[i]);

		/* p->util is skip-worktree */
		यदि (p->util)
			अनुवर्त;

		यदि (!lstat(p->string, &st)) {
			यदि (add_to_index(the_repository->index, p->string, &st, 0))
				die(_("updating files failed"));
		} अन्यथा
			remove_file_from_index(the_repository->index, p->string);
	}
}

स्थिर शून्य create_base_index(ध्रुव संरचना commit *current_head)
{
	संरचना वृक्ष *वृक्ष;
	संरचना unpack_trees_options opts;
	संरचना वृक्ष_desc t;

	यदि (!current_head) {
		discard_index(the_repository->index);
		प्रत्यागम;
	}

	स_रखो(&opts, 0, परिमाण(opts));
	opts.head_idx = 1;
	opts.index_only = 1;
	opts.merge = 1;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;

	opts.fn = oneway_merge;
	वृक्ष = repo_parse_वृक्ष_indirect(the_repository,
					&current_head->object.oid);
	यदि (!वृक्ष)
		die(_("failed to unpack HEAD tree object"));
	यदि (repo_parse_वृक्ष(the_repository, वृक्ष) < 0)
		निकास(128);
	init_वृक्ष_desc(&t, &वृक्ष->object.oid, वृक्ष->buffer, वृक्ष->size);
	यदि (unpack_trees(1, &t, &opts))
		निकास(128); /* We've already reported the error, finish dying */
}

स्थिर शून्य refresh_cache_or_die(पूर्णाङ्क refresh_flags)
{
	/*
	 * refresh_flags contains REFRESH_QUIET, so the only errors
	 * are for unmerged entries.
	 */
	यदि (refresh_index(the_repository->index, refresh_flags | REFRESH_IN_PORCELAIN, रिक्त, रिक्त, रिक्त))
		die_resolve_conflict("commit");
}

स्थिर ध्रुव वर्ण *prepare_index(ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix,
				 ध्रुव संरचना commit *current_head, पूर्णाङ्क is_status)
{
	संरचना string_list partial = STRING_LIST_INIT_DUP;
	संरचना pathspec pathspec;
	पूर्णाङ्क refresh_flags = REFRESH_QUIET;
	ध्रुव वर्ण *ret;
	वर्ण *path = रिक्त;

	यदि (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	यदि (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	यदि (is_status)
		refresh_flags |= REFRESH_UNMERGED;
	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_FULL,
		       prefix, argv);

	यदि (pathspec_from_file) {
		यदि (interactive)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--interactive/--patch");

		यदि (all)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "-a");

		यदि (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_FULL,
				    prefix, pathspec_from_file, pathspec_file_nul);
	} अन्यथा यदि (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	यदि (!pathspec.nr && (also || (only && !allow_empty &&
	    (!amend || (fixup_message && म_भेद(fixup_prefix, "amend"))))))
		die(_("No paths with --include/--only does not make sense."));

	यदि (repo_read_index_preload(the_repository, &pathspec, 0) < 0)
		die(_("index file corrupt"));

	यदि (interactive) {
		वर्ण *old_index_env = रिक्त, *old_repo_index_file;
		repo_hold_locked_index(the_repository, &index_lock,
				       LOCK_DIE_ON_ERROR);

		refresh_cache_or_die(refresh_flags);

		यदि (write_locked_index(the_repository->index, &index_lock, 0))
			die(_("unable to create temporary index"));

		old_repo_index_file = the_repository->index_file;
		the_repository->index_file =
			(वर्ण *)get_lock_file_path(&index_lock);
		old_index_env = xstrdup_or_null(दो_पर्या(INDEX_ENVIRONMENT));
		setenv(INDEX_ENVIRONMENT, the_repository->index_file, 1);

		यदि (interactive_add(the_repository, argv, prefix, patch_interactive, &interactive_opts) != 0)
			die(_("interactive add failed"));

		the_repository->index_file = old_repo_index_file;
		यदि (old_index_env && *old_index_env)
			setenv(INDEX_ENVIRONMENT, old_index_env, 1);
		अन्यथा
			unsetenv(INDEX_ENVIRONMENT);
		FREE_AND_NULL(old_index_env);

		discard_index(the_repository->index);
		read_index_from(the_repository->index, get_lock_file_path(&index_lock),
				repo_get_git_dir(the_repository));
		यदि (cache_वृक्ष_update(the_repository->index, WRITE_वृक्ष_SILENT) == 0) {
			यदि (reopen_lock_file(&index_lock) < 0)
				die(_("unable to write index file"));
			यदि (write_locked_index(the_repository->index, &index_lock, 0))
				die(_("unable to update temporary index"));
		} अन्यथा
			warning(_("Failed to update main cache tree"));

		commit_style = COMMIT_NORMAL;
		ret = get_lock_file_path(&index_lock);
		गच्छ out;
	} अन्यथा {
		यदि (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--interactive/--patch");
		यदि (interactive_opts.interhunkcontext != -1)
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
	यदि (all || (also && pathspec.nr)) {
		वर्ण *ps_matched = xcalloc(pathspec.nr, 1);
		repo_hold_locked_index(the_repository, &index_lock,
				       LOCK_DIE_ON_ERROR);
		add_files_to_cache(the_repository, also ? prefix : रिक्त,
				   &pathspec, ps_matched, 0, 0, 0 );
		यदि (!all && report_path_error(ps_matched, &pathspec))
			निकास(128);

		refresh_cache_or_die(refresh_flags);
		cache_वृक्ष_update(the_repository->index, WRITE_वृक्ष_SILENT);
		यदि (write_locked_index(the_repository->index, &index_lock, 0))
			die(_("unable to write new index file"));
		commit_style = COMMIT_NORMAL;
		ret = get_lock_file_path(&index_lock);
		मुक्त(ps_matched);
		गच्छ out;
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
	यदि (!only && !pathspec.nr) {
		repo_hold_locked_index(the_repository, &index_lock,
				       LOCK_DIE_ON_ERROR);
		refresh_cache_or_die(refresh_flags);
		यदि (the_repository->index->cache_changed
		    || !cache_वृक्ष_fully_valid(the_repository->index))
			cache_वृक्ष_update(the_repository->index, WRITE_वृक्ष_SILENT);
		यदि (write_locked_index(the_repository->index, &index_lock,
				       COMMIT_LOCK | SKIP_IF_UNCHANGED))
			die(_("unable to write new index file"));
		commit_style = COMMIT_AS_IS;
		ret = repo_get_index_file(the_repository);
		गच्छ out;
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

	यदि (whence != FROM_COMMIT) {
		यदि (whence == FROM_MERGE)
			die(_("cannot do a partial commit during a merge."));
		अन्यथा यदि (is_from_cherry_pick(whence))
			die(_("cannot do a partial commit during a cherry-pick."));
		अन्यथा यदि (is_from_rebase(whence))
			die(_("cannot do a partial commit during a rebase."));
	}

	यदि (list_paths(&partial, !current_head ? रिक्त : "HEAD", &pathspec))
		निकास(1);

	discard_index(the_repository->index);
	यदि (repo_read_index(the_repository) < 0)
		die(_("cannot read the index"));

	repo_hold_locked_index(the_repository, &index_lock, LOCK_DIE_ON_ERROR);
	add_remove_files(&partial);
	refresh_index(the_repository->index, REFRESH_QUIET, रिक्त, रिक्त, रिक्त);
	cache_वृक्ष_update(the_repository->index, WRITE_वृक्ष_SILENT);
	यदि (write_locked_index(the_repository->index, &index_lock, 0))
		die(_("unable to write new index file"));

	path = repo_git_path(the_repository, "next-index-%"PRIuMAX,
			     (uintmax_t) getpid());
	hold_lock_file_for_update(&false_lock, path,
				  LOCK_DIE_ON_ERROR);

	create_base_index(current_head);
	add_remove_files(&partial);
	refresh_index(the_repository->index, REFRESH_QUIET, रिक्त, रिक्त, रिक्त);

	यदि (write_locked_index(the_repository->index, &false_lock, 0))
		die(_("unable to write temporary index file"));

	discard_index(the_repository->index);
	ret = get_lock_file_path(&false_lock);
	read_index_from(the_repository->index, ret, repo_get_git_dir(the_repository));
out:
	string_list_clear(&partial, 0);
	clear_pathspec(&pathspec);
	मुक्त(path);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क run_status(खाता *fp, ध्रुव वर्ण *index_file, ध्रुव वर्ण *prefix, पूर्णाङ्क nowarn,
		      संरचना wt_status *s)
{
	संरचना object_id oid;

	यदि (s->relative_paths)
		s->prefix = prefix;

	यदि (amend) {
		s->amend = 1;
		s->reference = "HEAD^1";
	}
	s->verbose = verbose;
	s->index_file = index_file;
	s->fp = fp;
	s->nowarn = nowarn;
	s->is_initial = repo_get_oid(the_repository, s->reference, &oid) ? 1 : 0;
	यदि (!s->is_initial)
		oidcpy(&s->oid_commit, &oid);
	s->status_format = status_format;
	s->ignore_submodule_arg = ignore_submodule_arg;

	wt_status_collect(s);
	wt_status_print(s);
	wt_status_collect_free_buffers(s);

	प्रत्यागम s->committable;
}

स्थिर पूर्णाङ्क is_a_merge(ध्रुव संरचना commit *current_head)
{
	प्रत्यागम !!(current_head->parents && current_head->parents->next);
}

स्थिर शून्य assert_split_ident(संरचना ident_split *id, ध्रुव संरचना strbuf *buf)
{
	यदि (split_ident_पंक्ति(id, buf->buf, buf->len) || !id->date_begin)
		BUG("unable to parse our own ident: %s", buf->buf);
}

स्थिर शून्य export_one(ध्रुव वर्ण *var, ध्रुव वर्ण *s, ध्रुव वर्ण *e, पूर्णाङ्क hack)
{
	संरचना strbuf buf = STRBUF_INIT;
	यदि (hack)
		strbuf_addch(&buf, hack);
	strbuf_add(&buf, s, e - s);
	setenv(var, buf.buf, 1);
	strbuf_release(&buf);
}

स्थिर पूर्णाङ्क parse_force_date(ध्रुव वर्ण *in, संरचना strbuf *out)
{
	strbuf_addch(out, '@');

	यदि (parse_date(in, out) < 0) {
		पूर्णाङ्क errors = 0;
		अचिन्हित दीर्घ t = approxidate_careful(in, &errors);
		यदि (errors)
			प्रत्यागम -1;
		strbuf_addf(out, "%lu", t);
	}

	प्रत्यागम 0;
}

स्थिर शून्य set_ident_var(वर्ण **buf, वर्ण *val)
{
	मुक्त(*buf);
	*buf = val;
}

स्थिर शून्य determine_author_info(संरचना strbuf *author_ident)
{
	वर्ण *name, *email, *date;
	संरचना ident_split author;

	name = xstrdup_or_null(दो_पर्या("GIT_AUTHOR_NAME"));
	email = xstrdup_or_null(दो_पर्या("GIT_AUTHOR_EMAIL"));
	date = xstrdup_or_null(दो_पर्या("GIT_AUTHOR_DATE"));

	यदि (author_message) {
		संरचना ident_split ident;
		माप_प्रकार len;
		ध्रुव वर्ण *a;

		a = find_commit_header(author_message_buffer, "author", &len);
		यदि (!a)
			die(_("commit '%s' lacks author header"), author_message);
		यदि (split_ident_पंक्ति(&ident, a, len) < 0)
			die(_("commit '%s' has malformed author line"), author_message);

		set_ident_var(&name, xmemdupz(ident.name_begin, ident.name_end - ident.name_begin));
		set_ident_var(&email, xmemdupz(ident.mail_begin, ident.mail_end - ident.mail_begin));

		यदि (ident.date_begin) {
			संरचना strbuf date_buf = STRBUF_INIT;
			strbuf_addch(&date_buf, '@');
			strbuf_add(&date_buf, ident.date_begin, ident.date_end - ident.date_begin);
			strbuf_addch(&date_buf, ' ');
			strbuf_add(&date_buf, ident.tz_begin, ident.tz_end - ident.tz_begin);
			set_ident_var(&date, strbuf_detach(&date_buf, रिक्त));
		}
	}

	यदि (force_author) {
		संरचना ident_split ident;

		यदि (split_ident_पंक्ति(&ident, force_author, म_माप(force_author)) < 0)
			die(_("malformed --author parameter"));
		set_ident_var(&name, xmemdupz(ident.name_begin, ident.name_end - ident.name_begin));
		set_ident_var(&email, xmemdupz(ident.mail_begin, ident.mail_end - ident.mail_begin));
	}

	यदि (force_date) {
		संरचना strbuf date_buf = STRBUF_INIT;
		यदि (parse_force_date(force_date, &date_buf))
			die(_("invalid date format: %s"), force_date);
		set_ident_var(&date, strbuf_detach(&date_buf, रिक्त));
	}

	strbuf_addstr(author_ident, fmt_ident(name, email, WANT_AUTHOR_IDENT, date,
				IDENT_STRICT));
	assert_split_ident(&author, author_ident);
	export_one("GIT_AUTHOR_NAME", author.name_begin, author.name_end, 0);
	export_one("GIT_AUTHOR_EMAIL", author.mail_begin, author.mail_end, 0);
	export_one("GIT_AUTHOR_DATE", author.date_begin, author.tz_end, '@');
	मुक्त(name);
	मुक्त(email);
	मुक्त(date);
}

स्थिर पूर्णाङ्क author_date_is_interesting(शून्य)
{
	प्रत्यागम author_message || force_date;
}

#ifndef WITH_BREAKING_CHANGES
स्थिर शून्य adjust_comment_पंक्ति_char(ध्रुव संरचना strbuf *sb)
{
	वर्ण candidates[] = "#;@!$%^&|:";
	वर्ण *candidate;
	ध्रुव वर्ण *p;
	माप_प्रकार cutoff;

	/* Ignore comment chars in trailing comments (e.g., Conflicts:) */
	cutoff = sb->len - ignored_log_message_bytes(sb->buf, sb->len);

	यदि (!स_प्रथम(sb->buf, candidates[0], sb->len)) {
		मुक्त(comment_पंक्ति_str_to_free);
		comment_पंक्ति_str = comment_पंक्ति_str_to_free =
			xstrfmt("%c", candidates[0]);
		प्रत्यागम;
	}

	p = sb->buf;
	candidate = म_अक्षर(candidates, *p);
	यदि (candidate)
		*candidate = ' ';
	प्रति (p = sb->buf; p + 1 < sb->buf + cutoff; p++) {
		यदि ((p[0] == '\n' || p[0] == '\r') && p[1]) {
			candidate = म_अक्षर(candidates, p[1]);
			यदि (candidate)
				*candidate = ' ';
		}
	}

	प्रति (p = candidates; *p == ' '; p++)
		;
	यदि (!*p)
		die(_("unable to select a comment character that is not used\n"
		      "in the current commit message"));
	मुक्त(comment_पंक्ति_str_to_free);
	comment_पंक्ति_str = comment_पंक्ति_str_to_free = xstrfmt("%c", *p);
}
#endif /* !WITH_BREAKING_CHANGES */

स्थिर शून्य prepare_amend_commit(संरचना commit *commit, संरचना strbuf *sb,
				संरचना pretty_print_context *ctx)
{
	ध्रुव वर्ण *buffer, *subject, *fmt;

	buffer = repo_get_commit_buffer(the_repository, commit, रिक्त);
	find_commit_subject(buffer, &subject);
	/*
	 * If we amend the 'amend!' commit then we don't want to
	 * duplicate the subject line.
	 */
	fmt = starts_with(subject, "amend!") ? "%b" : "%B";
	repo_format_commit_message(the_repository, commit, fmt, sb, ctx);
	repo_unuse_commit_buffer(the_repository, commit, buffer);
}

स्थिर शून्य change_data_free(शून्य *util, ध्रुव वर्ण *str UNUSED)
{
	संरचना wt_status_change_data *d = util;
	मुक्त(d->rename_source);
	मुक्त(d);
}

स्थिर पूर्णाङ्क prepare_to_commit(ध्रुव वर्ण *index_file, ध्रुव वर्ण *prefix,
			     संरचना commit *current_head,
			     संरचना wt_status *s,
			     संरचना strbuf *author_ident)
{
	संरचना stat statbuf;
	संरचना strbuf committer_ident = STRBUF_INIT;
	पूर्णाङ्क committable;
	संरचना strbuf sb = STRBUF_INIT;
	ध्रुव वर्ण *hook_arg1 = रिक्त;
	ध्रुव वर्ण *hook_arg2 = रिक्त;
	पूर्णाङ्क clean_message_contents = (cleanup_mode != COMMIT_संदेश_CLEANUP_NONE);
	पूर्णाङ्क old_display_comment_prefix;
	पूर्णाङ्क invoked_hook;

	/* This checks and barfs if author is badly specified */
	determine_author_info(author_ident);

	यदि (!no_verify && run_commit_hook(use_editor, index_file, &invoked_hook,
					  "pre-commit", रिक्त))
		प्रत्यागम 0;

	यदि (squash_message) {
		/*
		 * Insert the proper subject line before other commit
		 * message options add their content.
		 */
		यदि (use_message && !म_भेद(use_message, squash_message))
			strbuf_addstr(&sb, "squash! ");
		अन्यथा {
			संरचना pretty_print_context ctx = {0};
			संरचना commit *c;
			c = lookup_commit_reference_by_name(squash_message);
			यदि (!c)
				die(_("could not lookup commit '%s'"), squash_message);
			ctx.output_encoding = get_commit_output_encoding();
			repo_format_commit_message(the_repository, c,
						   "squash! %s\n\n", &sb,
						   &ctx);
		}
	}

	यदि (have_option_m && !fixup_message) {
		strbuf_addbuf(&sb, &message);
		hook_arg1 = "message";
	} अन्यथा यदि (logfile && !म_भेद(logfile, "-")) {
		यदि (isatty(0))
			ख_लिखो(मानक_त्रुटि, _("(reading log message from standard input)\n"));
		यदि (strbuf_read(&sb, 0, 0) < 0)
			die_errno(_("could not read log from standard input"));
		hook_arg1 = "message";
	} अन्यथा यदि (logfile) {
		यदि (strbuf_read_file(&sb, logfile, 0) < 0)
			die_errno(_("could not read log file '%s'"),
				  logfile);
		hook_arg1 = "message";
	} अन्यथा यदि (use_message) {
		ध्रुव वर्ण *buffer;
		buffer = म_माला(use_message_buffer, "\n\n");
		यदि (buffer)
			strbuf_addstr(&sb, skip_blank_पंक्तियाँ(buffer + 2));
		hook_arg1 = "commit";
		hook_arg2 = use_message;
	} अन्यथा यदि (fixup_message) {
		संरचना pretty_print_context ctx = {0};
		संरचना commit *commit;
		वर्ण *fmt;
		commit = lookup_commit_reference_by_name(fixup_commit);
		यदि (!commit)
			die(_("could not lookup commit '%s'"), fixup_commit);
		ctx.output_encoding = get_commit_output_encoding();
		fmt = xstrfmt("%s! %%s\n\n", fixup_prefix);
		repo_format_commit_message(the_repository, commit, fmt, &sb,
					   &ctx);
		मुक्त(fmt);
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
		यदि (have_option_m && !म_भेद(fixup_prefix, "fixup"))
			strbuf_addbuf(&sb, &message);

		यदि (!म_भेद(fixup_prefix, "amend")) {
			यदि (have_option_m)
				die(_("options '%s' and '%s:%s' cannot be used together"), "-m", "--fixup", fixup_message);
			prepare_amend_commit(commit, &sb, &ctx);
		}
	} अन्यथा यदि (!stat(git_path_merge_संदेश(the_repository), &statbuf)) {
		माप_प्रकार merge_संदेश_start;

		/*
		 * prepend SQUASH_MSG here if it exists and a
		 * "merge --squash" was originally performed
		 */
		यदि (!stat(git_path_squash_संदेश(the_repository), &statbuf)) {
			यदि (strbuf_read_file(&sb, git_path_squash_संदेश(the_repository), 0) < 0)
				die_errno(_("could not read SQUASH_MSG"));
			hook_arg1 = "squash";
		} अन्यथा
			hook_arg1 = "merge";

		merge_संदेश_start = sb.len;
		यदि (strbuf_read_file(&sb, git_path_merge_संदेश(the_repository), 0) < 0)
			die_errno(_("could not read MERGE_MSG"));

		यदि (cleanup_mode == COMMIT_संदेश_CLEANUP_SCISSORS &&
		    wt_status_locate_end(sb.buf + merge_संदेश_start,
					 sb.len - merge_संदेश_start) <
				sb.len - merge_संदेश_start)
			s->added_cut_पंक्ति = 1;
	} अन्यथा यदि (!stat(git_path_squash_संदेश(the_repository), &statbuf)) {
		यदि (strbuf_read_file(&sb, git_path_squash_संदेश(the_repository), 0) < 0)
			die_errno(_("could not read SQUASH_MSG"));
		hook_arg1 = "squash";
	} अन्यथा यदि (template_file) {
		यदि (strbuf_read_file(&sb, template_file, 0) < 0)
			die_errno(_("could not read '%s'"), template_file);
		hook_arg1 = "template";
		clean_message_contents = 0;
	}

	/*
	 * The remaining cases don't modify the template message, but
	 * just set the argument(s) to the prepare-commit-msg hook.
	 */
	अन्यथा यदि (whence == FROM_MERGE)
		hook_arg1 = "merge";
	अन्यथा यदि (is_from_cherry_pick(whence) || whence == FROM_REBASE_PICK) {
		hook_arg1 = "commit";
		hook_arg2 = "CHERRY_PICK_HEAD";
	}

	यदि (squash_message) {
		/*
		 * If squash_commit was used for the commit subject,
		 * then we're possibly hijacking other commit log options.
		 * Reset the hook args to tell the real story.
		 */
		hook_arg1 = "message";
		hook_arg2 = "";
	}

	s->fp = fopen_for_writing(git_path_commit_editmsg());
	यदि (!s->fp)
		die_errno(_("could not open '%s'"), git_path_commit_editmsg());

	/* Ignore status.displayCommentPrefix: we do need comments in COMMIT_EDITMSG. */
	old_display_comment_prefix = s->display_comment_prefix;
	s->display_comment_prefix = 1;

	/*
	 * Most hints are counter-productive when the commit has
	 * already started.
	 */
	s->hints = 0;

	यदि (clean_message_contents)
		strbuf_stripspace(&sb, रिक्त);

	यदि (signoff)
		append_signoff(&sb, ignored_log_message_bytes(sb.buf, sb.len), 0);

	यदि (ख_डालो(sb.buf, 1, sb.len, s->fp) < sb.len)
		die_errno(_("could not write commit template"));

#ifndef WITH_BREAKING_CHANGES
	यदि (auto_comment_पंक्ति_char)
		adjust_comment_पंक्ति_char(&sb);
#endif /* !WITH_BREAKING_CHANGES */
	strbuf_release(&sb);

	/* This checks if committer ident is explicitly given */
	strbuf_addstr(&committer_ident, git_committer_info(IDENT_STRICT));
	यदि (use_editor && include_status) {
		पूर्णाङ्क ident_shown = 0;
		गणना git_colorbool saved_color_setting;
		संरचना ident_split ci, ai;
		ध्रुव वर्ण *hint_cleanup_all = allow_empty_message ?
			_("Please enter the commit message for your changes."
			  " Lines starting\nwith '%s' will be ignored.\n") :
			_("Please enter the commit message for your changes."
			  " Lines starting\nwith '%s' will be ignored, and an empty"
			  " message aborts the commit.\n");
		ध्रुव वर्ण *hint_cleanup_space = allow_empty_message ?
			_("Please enter the commit message for your changes."
			  " Lines starting\n"
			  "with '%s' will be kept; you may remove them"
			  " yourself if you want to.\n") :
			_("Please enter the commit message for your changes."
			  " Lines starting\n"
			  "with '%s' will be kept; you may remove them"
			  " yourself if you want to.\n"
			  "An empty message aborts the commit.\n");
		यदि (whence != FROM_COMMIT) {
			यदि (cleanup_mode == COMMIT_संदेश_CLEANUP_SCISSORS)
				wt_status_add_cut_पंक्ति(s);
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

		ख_लिखो(s->fp, "\n");
		यदि (cleanup_mode == COMMIT_संदेश_CLEANUP_ALL)
			status_printf(s, GIT_COLOR_NORMAL, hint_cleanup_all, comment_पंक्ति_str);
		अन्यथा यदि (cleanup_mode == COMMIT_संदेश_CLEANUP_SCISSORS) {
			यदि (whence == FROM_COMMIT)
				wt_status_add_cut_पंक्ति(s);
		} अन्यथा /* COMMIT_MSG_CLEANUP_SPACE, that is. */
			status_printf(s, GIT_COLOR_NORMAL, hint_cleanup_space, comment_पंक्ति_str);

		/*
		 * These should never fail because they come from our own
		 * fmt_ident. They may fail the sane_ident test, but we know
		 * that the name and mail pointers will at least be valid,
		 * which is enough for our tests and printing here.
		 */
		assert_split_ident(&ai, author_ident);
		assert_split_ident(&ci, &committer_ident);

		यदि (ident_cmp(&ai, &ci))
			status_printf_ln(s, GIT_COLOR_NORMAL,
				_("%s"
				"Author:    %.*s <%.*s>"),
				ident_shown++ ? "" : "\n",
				(पूर्णाङ्क)(ai.name_end - ai.name_begin), ai.name_begin,
				(पूर्णाङ्क)(ai.mail_end - ai.mail_begin), ai.mail_begin);

		यदि (author_date_is_interesting())
			status_printf_ln(s, GIT_COLOR_NORMAL,
				_("%s"
				"Date:      %s"),
				ident_shown++ ? "" : "\n",
				show_ident_date(&ai, DATE_MODE(NORMAL)));

		यदि (!committer_ident_sufficiently_given())
			status_printf_ln(s, GIT_COLOR_NORMAL,
				_("%s"
				"Committer: %.*s <%.*s>"),
				ident_shown++ ? "" : "\n",
				(पूर्णाङ्क)(ci.name_end - ci.name_begin), ci.name_begin,
				(पूर्णाङ्क)(ci.mail_end - ci.mail_begin), ci.mail_begin);

		status_printf_ln(s, GIT_COLOR_NORMAL, "%s", ""); /* Add new line for clarity */

		saved_color_setting = s->use_color;
		s->use_color = GIT_COLOR_NEVER;
		committable = run_status(s->fp, index_file, prefix, 1, s);
		s->use_color = saved_color_setting;
		string_list_clear_func(&s->change, change_data_free);
	} अन्यथा {
		संरचना object_id oid;
		ध्रुव वर्ण *parent = "HEAD";

		यदि (!the_repository->index->initialized && repo_read_index(the_repository) < 0)
			die(_("Cannot read index"));

		यदि (amend)
			parent = "HEAD^1";

		यदि (repo_get_oid(the_repository, parent, &oid)) {
			पूर्णाङ्क i, ita_nr = 0;

			/* TODO: audit for interaction with sparse-index. */
			ensure_full_index(the_repository->index);
			प्रति (i = 0; i < the_repository->index->cache_nr; i++)
				यदि (ce_intent_to_add(the_repository->index->cache[i]))
					ita_nr++;
			committable = the_repository->index->cache_nr > ita_nr;
		} अन्यथा {
			/*
			 * Unless the user did explicitly request a submodule
			 * ignore mode by passing a command line option we do
			 * not ignore any changed submodule SHA-1s when
			 * comparing index and parent, no matter what is
			 * configured. Otherwise we won't commit any
			 * submodules which were manually staged, which would
			 * be really confusing.
			 */
			संरचना diff_flags flags = DIFF_FLAGS_INIT;
			flags.override_submodule_config = 1;
			यदि (ignore_submodule_arg &&
			    !म_भेद(ignore_submodule_arg, "all"))
				flags.ignore_submodules = 1;
			committable = index_differs_from(the_repository,
							 parent, &flags, 1);
		}
	}
	strbuf_release(&committer_ident);

	ख_बंद(s->fp);

	यदि (trailer_args.nr) {
		यदि (amend_file_with_trailers(git_path_commit_editmsg(), &trailer_args))
			die(_("unable to pass trailers to --trailers"));
		strvec_clear(&trailer_args);
	}

	/*
	 * Reject an attempt to record a non-merge empty commit without
	 * explicit --allow-empty. In the cherry-pick case, it may be
	 * empty due to conflict resolution, which the user should okay.
	 */
	यदि (!committable && whence != FROM_MERGE && !allow_empty &&
	    !(amend && is_a_merge(current_head))) {
		s->hints = advice_enabled(ADVICE_STATUS_HINTS);
		s->display_comment_prefix = old_display_comment_prefix;
		run_status(मानक_निकास, index_file, prefix, 0, s);
		यदि (amend)
			ख_माला_दो(_(empty_amend_advice), मानक_त्रुटि);
		अन्यथा यदि (is_from_cherry_pick(whence) ||
			 whence == FROM_REBASE_PICK) {
			ख_माला_दो(_(empty_cherry_pick_advice), मानक_त्रुटि);
			यदि (whence == FROM_CHERRY_PICK_SINGLE)
				ख_माला_दो(_(empty_cherry_pick_advice_single), मानक_त्रुटि);
			अन्यथा यदि (whence == FROM_CHERRY_PICK_MULTI)
				ख_माला_दो(_(empty_cherry_pick_advice_multi), मानक_त्रुटि);
			अन्यथा
				ख_माला_दो(_(empty_rebase_pick_advice), मानक_त्रुटि);
		}
		प्रत्यागम 0;
	}

	यदि (!no_verify && invoked_hook) {
		/*
		 * Re-read the index as the pre-commit-commit hook was invoked
		 * and could have updated it. We must do this before we invoke
		 * the editor and after we invoke run_status above.
		 */
		discard_index(the_repository->index);
	}
	read_index_from(the_repository->index, index_file, repo_get_git_dir(the_repository));

	यदि (cache_वृक्ष_update(the_repository->index, 0)) {
		error(_("Error building trees"));
		प्रत्यागम 0;
	}

	यदि (run_commit_hook(use_editor, index_file, रिक्त, "prepare-commit-msg",
			    git_path_commit_editmsg(), hook_arg1, hook_arg2, रिक्त))
		प्रत्यागम 0;

	यदि (use_editor) {
		संरचना strvec env = STRVEC_INIT;

		strvec_pushf(&env, "GIT_INDEX_FILE=%s", index_file);
		यदि (launch_editor(git_path_commit_editmsg(), रिक्त, env.v)) {
			ख_लिखो(मानक_त्रुटि,
			_("Please supply the message using either -m or -F option.\n"));
			निकास(1);
		}
		strvec_clear(&env);
	}

	यदि (!no_verify &&
	    run_commit_hook(use_editor, index_file, रिक्त, "commit-msg",
			    git_path_commit_editmsg(), रिक्त)) {
		प्रत्यागम 0;
	}

	प्रत्यागम 1;
}

स्थिर ध्रुव वर्ण *find_author_by_nickname(ध्रुव वर्ण *name)
{
	संरचना rev_info revs;
	संरचना commit *commit;
	संरचना strbuf buf = STRBUF_INIT;
	ध्रुव वर्ण *av[20];
	पूर्णाङ्क ac = 0;

	repo_init_revisions(the_repository, &revs, रिक्त);
	strbuf_addf(&buf, "--author=%s", name);
	av[++ac] = "--all";
	av[++ac] = "-i";
	av[++ac] = buf.buf;
	av[++ac] = रिक्त;
	setup_revisions(ac, av, &revs, रिक्त);
	revs.mailmap = xmalloc(परिमाण(संरचना string_list));
	string_list_init_nodup(revs.mailmap);
	read_mailmap(the_repository, revs.mailmap);

	यदि (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	commit = get_revision(&revs);
	यदि (commit) {
		संरचना pretty_print_context ctx = {0};
		ctx.date_mode.type = DATE_NORMAL;
		strbuf_release(&buf);
		repo_format_commit_message(the_repository, commit,
					   "%aN <%aE>", &buf, &ctx);
		release_revisions(&revs);
		प्रत्यागम strbuf_detach(&buf, रिक्त);
	}
	die(_("--author '%s' is not 'Name <email>' and matches no existing author"), name);
}

स्थिर शून्य handle_ignored_arg(संरचना wt_status *s)
{
	यदि (!ignored_arg)
		; /* default already initialized */
	अन्यथा यदि (!म_भेद(ignored_arg, "traditional"))
		s->show_ignored_mode = SHOW_TRADITIONAL_IGNORED;
	अन्यथा यदि (!म_भेद(ignored_arg, "no"))
		s->show_ignored_mode = SHOW_NO_IGNORED;
	अन्यथा यदि (!म_भेद(ignored_arg, "matching"))
		s->show_ignored_mode = SHOW_MATCHING_IGNORED;
	अन्यथा
		die(_("Invalid ignored mode '%s'"), ignored_arg);
}

स्थिर गणना untracked_status_type parse_untracked_setting_name(ध्रुव वर्ण *u)
{
	/*
	 * Please update $__git_untracked_file_modes in
	 * git-completion.bash when you add new options
	 */
	विकल्प (git_parse_maybe_bool(u)) {
	पक्ष 0:
		u = "no";
		भङ्ग;
	पक्ष 1:
		u = "normal";
		भङ्ग;
	स्वभाव:
		भङ्ग;
	}

	यदि (!म_भेद(u, "no"))
		प्रत्यागम SHOW_NO_UNTRACKED_FILES;
	अन्यथा यदि (!म_भेद(u, "normal"))
		प्रत्यागम SHOW_NORMAL_UNTRACKED_FILES;
	अन्यथा यदि (!म_भेद(u, "all"))
		प्रत्यागम SHOW_ALL_UNTRACKED_FILES;
	अन्यथा
		प्रत्यागम SHOW_UNTRACKED_FILES_ERROR;
}

स्थिर शून्य handle_untracked_files_arg(संरचना wt_status *s)
{
	गणना untracked_status_type u;

	यदि (!untracked_files_arg)
		प्रत्यागम; /* default already initialized */

	u = parse_untracked_setting_name(untracked_files_arg);
	यदि (u == SHOW_UNTRACKED_FILES_ERROR)
		die(_("Invalid untracked files mode '%s'"),
		    untracked_files_arg);
	s->show_untracked_files = u;
}

स्थिर ध्रुव वर्ण *read_commit_message(ध्रुव वर्ण *name)
{
	ध्रुव वर्ण *out_enc;
	संरचना commit *commit;

	commit = lookup_commit_reference_by_name(name);
	यदि (!commit)
		die(_("could not lookup commit '%s'"), name);
	out_enc = get_commit_output_encoding();
	प्रत्यागम repo_logmsg_reencode(the_repository, commit, रिक्त, out_enc);
}

/*
 * Enumerate what needs to be propagated when --porcelain
 * is not in effect here.
 */
स्थिर संरचना status_deferred_config {
	गणना wt_status_format status_format;
	पूर्णाङ्क show_branch;
	गणना ahead_behind_flags ahead_behind;
} status_deferred_config = {
	STATUS_FORMAT_UNSPECIFIED,
	-1, /* unspecified */
	AHEAD_BEHIND_UNSPECIFIED,
};

स्थिर शून्य finalize_deferred_config(संरचना wt_status *s)
{
	पूर्णाङ्क use_deferred_config = (status_format != STATUS_FORMAT_PORCELAIN &&
				   status_format != STATUS_FORMAT_PORCELAIN_V2 &&
				   !s->null_termination);

	यदि (s->null_termination) {
		यदि (status_format == STATUS_FORMAT_NONE ||
		    status_format == STATUS_FORMAT_UNSPECIFIED)
			status_format = STATUS_FORMAT_PORCELAIN;
		अन्यथा यदि (status_format == STATUS_FORMAT_LONG)
			die(_("options '%s' and '%s' cannot be used together"), "--long", "-z");
	}

	यदि (use_deferred_config && status_format == STATUS_FORMAT_UNSPECIFIED)
		status_format = status_deferred_config.status_format;
	यदि (status_format == STATUS_FORMAT_UNSPECIFIED)
		status_format = STATUS_FORMAT_NONE;

	यदि (use_deferred_config && s->show_branch < 0)
		s->show_branch = status_deferred_config.show_branch;
	यदि (s->show_branch < 0)
		s->show_branch = 0;

	/*
	 * If the user did not give a "--[no]-ahead-behind" command
	 * line argument *AND* we will print in a human-readable format
	 * (short, long etc.) then we inherit from the status.aheadbehind
	 * config setting.  In all other cases (and porcelain V[12] formats
	 * in particular), we inherit _FULL for backwards compatibility.
	 */
	यदि (use_deferred_config &&
	    s->ahead_behind_flags == AHEAD_BEHIND_UNSPECIFIED)
		s->ahead_behind_flags = status_deferred_config.ahead_behind;

	यदि (s->ahead_behind_flags == AHEAD_BEHIND_UNSPECIFIED)
		s->ahead_behind_flags = AHEAD_BEHIND_FULL;
}

स्थिर शून्य check_fixup_reword_options(पूर्णाङ्क argc, ध्रुव वर्ण *argv[]) {
	यदि (whence != FROM_COMMIT) {
		यदि (whence == FROM_MERGE)
			die(_("You are in the middle of a merge -- cannot reword."));
		अन्यथा यदि (is_from_cherry_pick(whence))
			die(_("You are in the middle of a cherry-pick -- cannot reword."));
	}
	यदि (argc)
		die(_("reword option of '%s' and path '%s' cannot be used together"), "--fixup", *argv);
	यदि (patch_interactive || interactive || all || also || only)
		die(_("reword option of '%s' and '%s' cannot be used together"),
			"--fixup", "--patch/--interactive/--all/--include/--only");
}

स्थिर पूर्णाङ्क parse_and_validate_options(पूर्णाङ्क argc, ध्रुव वर्ण *argv[],
				      ध्रुव संरचना option *options,
				      ध्रुव वर्ण * ध्रुव usage[],
				      ध्रुव वर्ण *prefix,
				      संरचना commit *current_head,
				      संरचना wt_status *s)
{
	argc = parse_options(argc, argv, prefix, options, usage, 0);
	finalize_deferred_config(s);

	यदि (force_author && !म_अक्षर(force_author, '>'))
		force_author = find_author_by_nickname(force_author);

	यदि (force_author && renew_authorship)
		die(_("options '%s' and '%s' cannot be used together"), "--reset-author", "--author");

	यदि (logfile || have_option_m || use_message)
		use_editor = 0;

	/* Sanity check options */
	यदि (amend && !current_head)
		die(_("You have nothing to amend."));
	यदि (amend && whence != FROM_COMMIT) {
		यदि (whence == FROM_MERGE)
			die(_("You are in the middle of a merge -- cannot amend."));
		अन्यथा यदि (is_from_cherry_pick(whence))
			die(_("You are in the middle of a cherry-pick -- cannot amend."));
		अन्यथा यदि (whence == FROM_REBASE_PICK)
			die(_("You are in the middle of a rebase -- cannot amend."));
	}
	यदि (fixup_message && squash_message)
		die(_("options '%s' and '%s' cannot be used together"), "--squash", "--fixup");
	die_for_incompatible_opt4(!!use_message, "-C",
				  !!edit_message, "-c",
				  !!logfile, "-F",
				  !!fixup_message, "--fixup");
	die_for_incompatible_opt4(have_option_m, "-m",
				  !!edit_message, "-c",
				  !!use_message, "-C",
				  !!logfile, "-F");
	यदि (use_message || edit_message || logfile ||fixup_message || have_option_m)
		FREE_AND_NULL(template_file);
	यदि (edit_message)
		use_message = edit_message;
	यदि (amend && !use_message && !fixup_message)
		use_message = "HEAD";
	यदि (!use_message && !is_from_cherry_pick(whence) &&
	    !is_from_rebase(whence) && renew_authorship)
		die(_("--reset-author can be used only with -C, -c or --amend."));
	यदि (use_message) {
		use_message_buffer = read_commit_message(use_message);
		यदि (!renew_authorship) {
			author_message = use_message;
			author_message_buffer = use_message_buffer;
		}
	}
	यदि ((is_from_cherry_pick(whence) || whence == FROM_REBASE_PICK) &&
	    !renew_authorship) {
		author_message = "CHERRY_PICK_HEAD";
		author_message_buffer = read_commit_message(author_message);
	}

	यदि (patch_interactive)
		interactive = 1;

	die_for_incompatible_opt4(also, "-i/--include",
				  only, "-o/--only",
				  all, "-a/--all",
				  interactive, "--interactive/-p/--patch");
	यदि (fixup_message) {
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
		वर्ण *p = fixup_message;
		यावत् (है_अक्षर(*p))
			p++;
		यदि (p > fixup_message && *p == ':') {
			*p = '\0';
			fixup_commit = p + 1;
			यदि (!म_भेद("amend", fixup_message) ||
			    !म_भेद("reword", fixup_message)) {
				fixup_prefix = "amend";
				allow_empty = 1;
				यदि (*fixup_message == 'r') {
					check_fixup_reword_options(argc, argv);
					only = 1;
				}
			} अन्यथा {
				die(_("unknown option: --fixup=%s:%s"), fixup_message, fixup_commit);
			}
		} अन्यथा {
			fixup_commit = fixup_message;
			fixup_prefix = "fixup";
			use_editor = 0;
		}
	}

	यदि (0 <= edit_flag)
		use_editor = edit_flag;

	handle_untracked_files_arg(s);

	यदि (all && argc > 0)
		die(_("paths '%s ...' with -a does not make sense"),
		    argv[0]);

	यदि (status_format != STATUS_FORMAT_NONE)
		dry_run = 1;

	प्रत्यागम argc;
}

स्थिर पूर्णाङ्क dry_run_commit(ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix,
			  ध्रुव संरचना commit *current_head, संरचना wt_status *s)
{
	पूर्णाङ्क committable;
	ध्रुव वर्ण *index_file;

	index_file = prepare_index(argv, prefix, current_head, 1);
	committable = run_status(मानक_निकास, index_file, prefix, 0, s);
	rollback_index_files();

	प्रत्यागम committable ? 0 : 1;
}

define_list_config_array_extra(color_status_slots, {"added"});

स्थिर पूर्णाङ्क parse_status_slot(ध्रुव वर्ण *slot)
{
	यदि (!strcasecmp(slot, "added"))
		प्रत्यागम WT_STATUS_UPDATED;

	प्रत्यागम LOOKUP_CONFIG(color_status_slots, slot);
}

स्थिर पूर्णाङ्क git_status_config(ध्रुव वर्ण *k, ध्रुव वर्ण *v,
			     ध्रुव संरचना config_context *ctx, शून्य *cb)
{
	संरचना wt_status *s = cb;
	ध्रुव वर्ण *slot_name;

	यदि (starts_with(k, "column."))
		प्रत्यागम git_column_config(k, v, "status", &s->colopts);
	यदि (!म_भेद(k, "status.submodulesummary")) {
		पूर्णाङ्क is_bool;
		s->submodule_summary = git_config_bool_or_int(k, v, ctx->kvi,
							      &is_bool);
		यदि (is_bool && s->submodule_summary)
			s->submodule_summary = -1;
		प्रत्यागम 0;
	}
	यदि (!म_भेद(k, "status.short")) {
		यदि (git_config_bool(k, v))
			status_deferred_config.status_format = STATUS_FORMAT_SHORT;
		अन्यथा
			status_deferred_config.status_format = STATUS_FORMAT_NONE;
		प्रत्यागम 0;
	}
	यदि (!म_भेद(k, "status.branch")) {
		status_deferred_config.show_branch = git_config_bool(k, v);
		प्रत्यागम 0;
	}
	यदि (!म_भेद(k, "status.aheadbehind")) {
		status_deferred_config.ahead_behind = git_config_bool(k, v);
		प्रत्यागम 0;
	}
	यदि (!म_भेद(k, "status.showstash")) {
		s->show_stash = git_config_bool(k, v);
		प्रत्यागम 0;
	}
	यदि (!म_भेद(k, "status.color") || !म_भेद(k, "color.status")) {
		s->use_color = git_config_colorbool(k, v);
		प्रत्यागम 0;
	}
	यदि (!म_भेद(k, "status.displaycommentprefix")) {
		s->display_comment_prefix = git_config_bool(k, v);
		प्रत्यागम 0;
	}
	यदि (skip_prefix(k, "status.color.", &slot_name) ||
	    skip_prefix(k, "color.status.", &slot_name)) {
		पूर्णाङ्क slot = parse_status_slot(slot_name);
		यदि (slot < 0)
			प्रत्यागम 0;
		यदि (!v)
			प्रत्यागम config_error_nonbool(k);
		प्रत्यागम color_parse(v, s->color_palette[slot]);
	}
	यदि (!म_भेद(k, "status.relativepaths")) {
		s->relative_paths = git_config_bool(k, v);
		प्रत्यागम 0;
	}
	यदि (!म_भेद(k, "status.showuntrackedfiles")) {
		गणना untracked_status_type u;

		u = parse_untracked_setting_name(v);
		यदि (u == SHOW_UNTRACKED_FILES_ERROR)
			प्रत्यागम error(_("Invalid untracked files mode '%s'"), v);
		s->show_untracked_files = u;
		प्रत्यागम 0;
	}
	यदि (!म_भेद(k, "diff.renamelimit")) {
		यदि (s->rename_limit == -1)
			s->rename_limit = git_config_int(k, v, ctx->kvi);
		प्रत्यागम 0;
	}
	यदि (!म_भेद(k, "status.renamelimit")) {
		s->rename_limit = git_config_int(k, v, ctx->kvi);
		प्रत्यागम 0;
	}
	यदि (!म_भेद(k, "diff.renames")) {
		यदि (s->detect_rename == -1)
			s->detect_rename = git_config_rename(k, v);
		प्रत्यागम 0;
	}
	यदि (!म_भेद(k, "status.renames")) {
		s->detect_rename = git_config_rename(k, v);
		प्रत्यागम 0;
	}
	प्रत्यागम git_diff_ui_config(k, v, ctx, रिक्त);
}

पूर्णाङ्क cmd_status(पूर्णाङ्क argc,
ध्रुव वर्ण **argv,
ध्रुव वर्ण *prefix,
संरचना repository *repo UNUSED)
{
	स्थिर पूर्णाङ्क no_renames = -1;
	स्थिर ध्रुव वर्ण *rename_score_arg = (ध्रुव वर्ण *)-1;
	स्थिर संरचना wt_status s;
	अचिन्हित पूर्णाङ्क progress_flag = 0;
	पूर्णाङ्क fd;
	संरचना object_id oid;
	स्थिर संरचना option builtin_status_options[] = {
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
			.मूल्य = &untracked_files_arg,
			.argh = N_("mode"),
			.help = N_("show untracked files, optional modes: all, normal, no. (Default: all)"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t)"all",
		},
		{
			.type = OPTION_STRING,
			.long_name = "ignored",
			.मूल्य = &ignored_arg,
			.argh = N_("mode"),
			.help = N_("show ignored files, optional modes: traditional, matching, no. (Default: traditional)"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t)"traditional",
		},
		{
			.type = OPTION_STRING,
			.long_name = "ignore-submodules",
			.मूल्य = &ignore_submodule_arg,
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

	यदि (s.show_ignored_mode == SHOW_MATCHING_IGNORED &&
	    s.show_untracked_files == SHOW_NO_UNTRACKED_FILES)
		die(_("Unsupported combination of ignored and untracked-files arguments"));

	parse_pathspec(&s.pathspec, 0,
		       PATHSPEC_PREFER_FULL,
		       prefix, argv);

	यदि (status_format != STATUS_FORMAT_PORCELAIN &&
	    status_format != STATUS_FORMAT_PORCELAIN_V2)
		progress_flag = REFRESH_PROGRESS;
	repo_read_index(the_repository);
	refresh_index(the_repository->index,
		      REFRESH_QUIET|REFRESH_UNMERGED|progress_flag,
		      &s.pathspec, रिक्त, रिक्त);

	यदि (use_optional_locks())
		fd = repo_hold_locked_index(the_repository, &index_lock, 0);
	अन्यथा
		fd = -1;

	s.is_initial = repo_get_oid(the_repository, s.reference, &oid) ? 1 : 0;
	यदि (!s.is_initial)
		oidcpy(&s.oid_commit, &oid);

	s.ignore_submodule_arg = ignore_submodule_arg;
	s.status_format = status_format;
	s.verbose = verbose;
	यदि (no_renames != -1)
		s.detect_rename = !no_renames;
	यदि ((intptr_t)rename_score_arg != -1) {
		यदि (s.detect_rename < DIFF_DETECT_RENAME)
			s.detect_rename = DIFF_DETECT_RENAME;
		यदि (rename_score_arg)
			s.rename_score = parse_rename_score(&rename_score_arg);
	}

	wt_status_collect(&s);

	यदि (0 <= fd)
		repo_update_index_if_able(the_repository, &index_lock);

	यदि (s.relative_paths)
		s.prefix = prefix;

	wt_status_print(&s);
	wt_status_collect_free_buffers(&s);

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क git_commit_config(ध्रुव वर्ण *k, ध्रुव वर्ण *v,
			     ध्रुव संरचना config_context *ctx, शून्य *cb)
{
	संरचना wt_status *s = cb;

	यदि (!म_भेद(k, "commit.template"))
		प्रत्यागम git_config_pathname(&template_file, k, v);
	यदि (!म_भेद(k, "commit.status")) {
		include_status = git_config_bool(k, v);
		प्रत्यागम 0;
	}
	यदि (!म_भेद(k, "commit.cleanup")) {
		FREE_AND_NULL(cleanup_config);
		प्रत्यागम git_config_string(&cleanup_config, k, v);
	}
	यदि (!म_भेद(k, "commit.gpgsign")) {
		sign_commit = git_config_bool(k, v) ? "" : रिक्त;
		प्रत्यागम 0;
	}
	यदि (!म_भेद(k, "commit.verbose")) {
		पूर्णाङ्क is_bool;
		config_commit_verbose = git_config_bool_or_int(k, v, ctx->kvi,
							       &is_bool);
		प्रत्यागम 0;
	}

	प्रत्यागम git_status_config(k, v, ctx, s);
}

पूर्णाङ्क cmd_commit(पूर्णाङ्क argc,
	       ध्रुव वर्ण **argv,
	       ध्रुव वर्ण *prefix,
	       संरचना repository *repo UNUSED)
{
	स्थिर संरचना wt_status s;
	स्थिर ध्रुव वर्ण *cleanup_arg = रिक्त;
	स्थिर संरचना option builtin_commit_options[] = {
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
			.मूल्य = &sign_commit,
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
			.मूल्य = &untracked_files_arg,
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

	संरचना strbuf sb = STRBUF_INIT;
	संरचना strbuf author_ident = STRBUF_INIT;
	ध्रुव वर्ण *index_file, *reflog_संदेश;
	संरचना object_id oid;
	संरचना commit_list *parents = रिक्त;
	संरचना stat statbuf;
	संरचना commit *current_head = रिक्त;
	संरचना commit_extra_header *extra = रिक्त;
	संरचना strbuf err = STRBUF_INIT;
	पूर्णाङ्क ret = 0;

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

	यदि (repo_get_oid(the_repository, "HEAD", &oid))
		current_head = रिक्त;
	अन्यथा {
		current_head = lookup_commit_or_die(&oid, "HEAD");
		यदि (repo_parse_commit(the_repository, current_head))
			die(_("could not parse HEAD commit"));
	}
	verbose = -1; /* unspecified */
	argc = parse_and_validate_options(argc, argv, builtin_commit_options,
					  builtin_commit_usage,
					  prefix, current_head, &s);
	यदि (trailer_args.nr)
		trailer_config_init();

	यदि (verbose == -1)
		verbose = (config_commit_verbose < 0) ? 0 : config_commit_verbose;

	यदि (cleanup_arg) {
		मुक्त(cleanup_config);
		cleanup_config = xstrdup(cleanup_arg);
	}
	cleanup_mode = get_cleanup_mode(cleanup_config, use_editor);

	यदि (dry_run)
		प्रत्यागम dry_run_commit(argv, prefix, current_head, &s);
	index_file = prepare_index(argv, prefix, current_head, 0);

	/* Set up everything for writing the commit object.  This includes
	   running hooks, writing the trees, and interacting with the user.  */
	यदि (!prepare_to_commit(index_file, prefix,
			       current_head, &s, &author_ident)) {
		ret = 1;
		rollback_index_files();
		गच्छ cleanup;
	}

	/* Determine parents */
	reflog_संदेश = दो_पर्या("GIT_REFLOG_ACTION");
	यदि (!current_head) {
		यदि (!reflog_संदेश)
			reflog_संदेश = "commit (initial)";
	} अन्यथा यदि (amend) {
		यदि (!reflog_संदेश)
			reflog_संदेश = "commit (amend)";
		parents = commit_list_copy(current_head->parents);
	} अन्यथा यदि (whence == FROM_MERGE) {
		संरचना strbuf m = STRBUF_INIT;
		खाता *fp;
		पूर्णाङ्क allow_fast_forward = 1;
		संरचना commit_list **pptr = &parents;

		यदि (!reflog_संदेश)
			reflog_संदेश = "commit (merge)";
		pptr = commit_list_append(current_head, pptr);
		fp = xfopen(git_path_merge_head(the_repository), "r");
		यावत् (strbuf_getline_lf(&m, fp) != खातापूर्ण) {
			संरचना commit *parent;

			parent = get_merge_parent(m.buf);
			यदि (!parent)
				die(_("Corrupt MERGE_HEAD file (%s)"), m.buf);
			pptr = commit_list_append(parent, pptr);
		}
		ख_बंद(fp);
		strbuf_release(&m);
		यदि (!stat(git_path_merge_mode(the_repository), &statbuf)) {
			यदि (strbuf_read_file(&sb, git_path_merge_mode(the_repository), 0) < 0)
				die_errno(_("could not read MERGE_MODE"));
			यदि (!म_भेद(sb.buf, "no-ff"))
				allow_fast_forward = 0;
		}
		यदि (allow_fast_forward)
			reduce_heads_replace(&parents);
	} अन्यथा {
		यदि (!reflog_संदेश)
			reflog_संदेश = is_from_cherry_pick(whence)
					? "commit (cherry-pick)"
					: is_from_rebase(whence)
					? "commit (rebase)"
					: "commit";
		commit_list_जोड़ो(current_head, &parents);
	}

	/* Finally, get the commit message */
	strbuf_reset(&sb);
	यदि (strbuf_read_file(&sb, git_path_commit_editmsg(), 0) < 0) {
		पूर्णाङ्क saved_errno = त्रुटि_सं;
		rollback_index_files();
		die(_("could not read commit message: %s"), म_त्रुटि(saved_errno));
	}

	cleanup_message(&sb, cleanup_mode, verbose);

	यदि (message_is_empty(&sb, cleanup_mode) && !allow_empty_message) {
		rollback_index_files();
		ख_लिखो(मानक_त्रुटि, _("Aborting commit due to empty commit message.\n"));
		निकास(1);
	}
	यदि (template_untouched(&sb, template_file, cleanup_mode) && !allow_empty_message) {
		rollback_index_files();
		ख_लिखो(मानक_त्रुटि, _("Aborting commit; you did not edit the message.\n"));
		निकास(1);
	}

	यदि (fixup_message && starts_with(sb.buf, "amend! ") &&
	    !allow_empty_message) {
		संरचना strbuf body = STRBUF_INIT;
		माप_प्रकार len = commit_subject_length(sb.buf);
		strbuf_addstr(&body, sb.buf + len);
		यदि (message_is_empty(&body, cleanup_mode)) {
			rollback_index_files();
			ख_लिखो(मानक_त्रुटि, _("Aborting commit due to empty commit message body.\n"));
			निकास(1);
		}
		strbuf_release(&body);
	}

	यदि (amend) {
		ध्रुव वर्ण *exclude_gpgsig[3] = { "gpgsig", "gpgsig-sha256", रिक्त };
		extra = read_commit_extra_headers(current_head, exclude_gpgsig);
	} अन्यथा {
		संरचना commit_extra_header **tail = &extra;
		append_merge_tag_headers(parents, &tail);
	}

	यदि (commit_वृक्ष_extended(sb.buf, sb.len, &the_repository->index->cache_वृक्ष->oid,
				 parents, &oid, author_ident.buf, रिक्त,
				 sign_commit, extra)) {
		rollback_index_files();
		die(_("failed to write commit object"));
	}

	यदि (update_head_with_reflog(current_head, &oid, reflog_संदेश, &sb,
				    &err)) {
		rollback_index_files();
		die("%s", err.buf);
	}

	sequencer_post_commit_cleanup(the_repository, 0);
	unlink(git_path_merge_head(the_repository));
	unlink(git_path_merge_संदेश(the_repository));
	unlink(git_path_merge_mode(the_repository));
	unlink(git_path_squash_संदेश(the_repository));

	यदि (commit_index_files())
		die(_("repository has been updated, but unable to write\n"
		      "new index file. Check that disk is not full and quota is\n"
		      "not exceeded, and then \"git restore --staged :/\" to recover."));

	git_test_write_commit_graph_or_die(the_repository);

	repo_rerere(the_repository, 0);
	run_auto_maintenance(the_repository, quiet);
	run_commit_hook(use_editor, repo_get_index_file(the_repository),
			रिक्त, "post-commit", रिक्त);
	यदि (amend && !no_post_rewrite) {
		commit_post_rewrite(the_repository, current_head, &oid);
	}
	यदि (!quiet) {
		अचिन्हित पूर्णाङ्क flags = 0;

		यदि (!current_head)
			flags |= SUMMARY_INITIAL_COMMIT;
		यदि (author_date_is_interesting())
			flags |= SUMMARY_SHOW_AUTHOR_DATE;
		print_commit_summary(the_repository, prefix,
				     &oid, flags);
	}

	apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
			    रिक्त, रिक्त, रिक्त, रिक्त);

cleanup:
	free_commit_extra_headers(extra);
	commit_list_free(parents);
	strbuf_release(&author_ident);
	strbuf_release(&err);
	strbuf_release(&sb);
	मुक्त(logfile);
	मुक्त(template_file);
	प्रत्यागम ret;
}
