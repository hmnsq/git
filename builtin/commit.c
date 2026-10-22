/*
 * Builtin "git commit"
 *
 * Copyright (c) 2007 Kristian Høgsberg <krh@redhat.com>
 * Based on git-commit.sh by Junio C Hamano and Linus Torvalds
 */

#घोषणा USE_THE_REPOSITORY_VARIABLE
#घोषणा DISABLE_SIGN_COMPARE_WARNINGS

#समावेश "builtin.h"
#समावेश "advice.h"
#समावेश "config.h"
#समावेश "lockfile.h"
#समावेश "cache-tree.h"
#समावेश "color.h"
#समावेश "dir.h"
#समावेश "editor.h"
#समावेश "environment.h"
#समावेश "diff.h"
#समावेश "commit.h"
#समावेश "add-interactive.h"
#समावेश "gettext.h"
#समावेश "revision.h"
#समावेश "wt-status.h"
#समावेश "run-command.h"
#समावेश "strbuf.h"
#समावेश "object-name.h"
#समावेश "parse-options.h"
#समावेश "path.h"
#समावेश "preload-index.h"
#समावेश "read-cache.h"
#समावेश "repository.h"
#समावेश "string-list.h"
#समावेश "rerere.h"
#समावेश "unpack-trees.h"
#समावेश "column.h"
#समावेश "sequencer.h"
#समावेश "sparse-index.h"
#समावेश "mailmap.h"
#समावेश "help.h"
#समावेश "commit-reach.h"
#समावेश "commit-graph.h"
#समावेश "pretty.h"
#समावेश "trailer.h"

अटल स्थिर अक्षर * स्थिर builtin_commit_usage[] = {
	N_("git commit [-a | --interactive | --patch] [-s] [-v] [-u[<mode>]] [--amend]\n"
	   "           [--dry-run] [(-c | -C | --squash) <commit> | --fixup [(amend|reword):]<commit>]\n"
	   "           [-F <file> | -m <msg>] [--reset-author] [--allow-empty]\n"
	   "           [--allow-empty-message] [--no-verify] [-e] [--author=<author>]\n"
	   "           [--date=<date>] [--cleanup=<mode>] [--[no-]status]\n"
	   "           [-i | -o] [--pathspec-from-file=<file> [--pathspec-file-nul]]\n"
	   "           [(--trailer <token>[(=|:)<value>])...] [-S[<keyid>]]\n"
	   "           [--] [<pathspec>...]"),
	शून्य
};

अटल स्थिर अक्षर * स्थिर builtin_status_usage[] = {
	N_("git status [<options>] [--] [<pathspec>...]"),
	शून्य
};

अटल स्थिर अक्षर empty_amend_advice[] =
N_("You asked to amend the most recent commit, but doing so would make\n"
"it empty. You can repeat your command with --allow-empty, or you can\n"
"remove the commit entirely with \"git reset HEAD^\".\n");

अटल स्थिर अक्षर empty_cherry_pick_advice[] =
N_("The previous cherry-pick is now empty, possibly due to conflict resolution.\n"
"If you wish to commit it anyway, use:\n"
"\n"
"    git commit --allow-empty\n"
"\n");

अटल स्थिर अक्षर empty_rebase_pick_advice[] =
N_("Otherwise, please use 'git rebase --skip'\n");

अटल स्थिर अक्षर empty_cherry_pick_advice_single[] =
N_("Otherwise, please use 'git cherry-pick --skip'\n");

अटल स्थिर अक्षर empty_cherry_pick_advice_multi[] =
N_("and then use:\n"
"\n"
"    git cherry-pick --continue\n"
"\n"
"to resume cherry-picking the remaining commits.\n"
"If you wish to skip this commit, use:\n"
"\n"
"    git cherry-pick --skip\n"
"\n");

अटल स्थिर अक्षर *color_status_slots[] = {
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

अटल स्थिर अक्षर *use_message_बफर;
अटल काष्ठा lock_file सूचकांक_lock; /* real index */
अटल काष्ठा lock_file false_lock; /* used only for partial commits */
अटल क्रमागत {
	COMMIT_AS_IS = 1,
	COMMIT_NORMAL,
	COMMIT_PARTIAL
} commit_style;

अटल स्थिर अक्षर *force_author;
अटल अक्षर *logfile;
अटल अक्षर *template_file;
/*
 * The _message variables are commit names from which to take
 * the commit message and/or authorship.
 */
अटल स्थिर अक्षर *author_message, *author_message_बफर;
अटल स्थिर अक्षर *edit_message, *use_message;
अटल अक्षर *fixup_message, *fixup_commit, *squash_message;
अटल स्थिर अक्षर *fixup_prefix;
अटल पूर्णांक all, also, interactive, patch_interactive, only, amend, signoff;
अटल काष्ठा interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
अटल पूर्णांक edit_flag = -1; /* unspecified */
अटल पूर्णांक quiet, verbose, no_verify, allow_empty, dry_run, renew_authorship;
अटल पूर्णांक config_commit_verbose = -1; /* unspecified */
अटल पूर्णांक no_post_rewrite, allow_empty_message, pathspec_file_nul;
अटल स्थिर अक्षर *untracked_files_arg, *force_date, *ignore_submodule_arg, *ignored_arg;
अटल स्थिर अक्षर *sign_commit, *pathspec_from_file;
अटल काष्ठा strvec trailer_args = STRVEC_INIT;

/*
 * The default commit message cleanup mode will remove the lines
 * beginning with # (shell comments) and leading and trailing
 * whitespaces (empty lines or containing only whitespaces)
 * if editor is used, and only the whitespaces if the message
 * is specified explicitly.
 */
अटल क्रमागत commit_संदेश_cleanup_mode cleanup_mode;
अटल अक्षर *cleanup_config;

अटल क्रमागत commit_whence whence;
अटल पूर्णांक use_editor = 1, include_status = 1;
अटल पूर्णांक have_option_m;
अटल काष्ठा strbuf message = STRBUF_INIT;

अटल क्रमागत wt_status_format status_format = STATUS_FORMAT_UNSPECIFIED;

अटल पूर्णांक opt_parse_porcelain(स्थिर काष्ठा option *opt, स्थिर अक्षर *arg, पूर्णांक unset)
{
	क्रमागत wt_status_format *मूल्य = (क्रमागत wt_status_format *)opt->मूल्य;
	अगर (unset)
		*मूल्य = STATUS_FORMAT_NONE;
	अन्यथा अगर (!arg)
		*मूल्य = STATUS_FORMAT_PORCELAIN;
	अन्यथा अगर (!म_भेद(arg, "v1") || !म_भेद(arg, "1"))
		*मूल्य = STATUS_FORMAT_PORCELAIN;
	अन्यथा अगर (!म_भेद(arg, "v2") || !म_भेद(arg, "2"))
		*मूल्य = STATUS_FORMAT_PORCELAIN_V2;
	अन्यथा
		die("unsupported porcelain version '%s'", arg);

	वापस 0;
}

अटल पूर्णांक opt_parse_m(स्थिर काष्ठा option *opt, स्थिर अक्षर *arg, पूर्णांक unset)
{
	काष्ठा strbuf *buf = opt->मूल्य;
	अगर (unset) {
		have_option_m = 0;
		strbuf_setlen(buf, 0);
	} अन्यथा {
		have_option_m = 1;
		अगर (buf->len)
			strbuf_addch(buf, '\n');
		strbuf_addstr(buf, arg);
		strbuf_complete_पंक्ति(buf);
	}
	वापस 0;
}

अटल पूर्णांक opt_parse_rename_score(स्थिर काष्ठा option *opt, स्थिर अक्षर *arg, पूर्णांक unset)
{
	स्थिर अक्षर **मूल्य = opt->मूल्य;

	BUG_ON_OPT_NEG(unset);

	अगर (arg != शून्य && *arg == '=')
		arg = arg + 1;

	*मूल्य = arg;
	वापस 0;
}

अटल व्योम determine_whence(काष्ठा wt_status *s)
{
	अगर (file_exists(git_path_merge_शीर्ष(the_repository)))
		whence = FROM_MERGE;
	अन्यथा अगर (!sequencer_determine_whence(the_repository, &whence))
		whence = FROM_COMMIT;
	अगर (s)
		s->whence = whence;
}

अटल व्योम status_init_config(काष्ठा wt_status *s, config_fn_t fn)
{
	wt_status_prepare(the_repository, s);
	init_diff_ui_defaults();
	repo_config(the_repository, fn, s);
	determine_whence(s);
	s->hints = advice_enabled(ADVICE_STATUS_HINTS); /* must come after repo_config() */
}

अटल व्योम rollback_सूचकांक_files(व्योम)
{
	चयन (commit_style) {
	हाल COMMIT_AS_IS:
		अवरोध; /* nothing to do */
	हाल COMMIT_NORMAL:
		rollback_lock_file(&सूचकांक_lock);
		अवरोध;
	हाल COMMIT_PARTIAL:
		rollback_lock_file(&सूचकांक_lock);
		rollback_lock_file(&false_lock);
		अवरोध;
	}
}

अटल पूर्णांक commit_सूचकांक_files(व्योम)
{
	पूर्णांक err = 0;

	चयन (commit_style) {
	हाल COMMIT_AS_IS:
		अवरोध; /* nothing to do */
	हाल COMMIT_NORMAL:
		err = commit_lock_file(&सूचकांक_lock);
		अवरोध;
	हाल COMMIT_PARTIAL:
		err = commit_lock_file(&सूचकांक_lock);
		rollback_lock_file(&false_lock);
		अवरोध;
	}

	वापस err;
}

/*
 * Take a union of paths in the index and the named tree (typically, "HEAD"),
 * and return the paths that match the given pattern in list.
 */
अटल पूर्णांक list_paths(काष्ठा string_list *list, स्थिर अक्षर *with_वृक्ष,
		      स्थिर काष्ठा pathspec *pattern)
{
	पूर्णांक i, ret;
	अक्षर *m;

	अगर (!pattern->nr)
		वापस 0;

	m = xcalloc(1, pattern->nr);

	अगर (with_वृक्ष) {
		अक्षर *max_prefix = common_prefix(pattern);
		overlay_वृक्ष_on_सूचकांक(the_repository->सूचकांक, with_वृक्ष, max_prefix);
		मुक्त(max_prefix);
	}

	/* TODO: audit for interaction with sparse-index. */
	ensure_full_सूचकांक(the_repository->सूचकांक);
	क्रम (i = 0; i < the_repository->सूचकांक->cache_nr; i++) {
		स्थिर काष्ठा cache_entry *ce = the_repository->सूचकांक->cache[i];
		काष्ठा string_list_item *item;

		अगर (ce->ce_flags & CE_UPDATE)
			जारी;
		अगर (!ce_path_match(the_repository->सूचकांक, ce, pattern, m))
			जारी;
		item = string_list_जोड़ो(list, ce->name);
		अगर (ce_skip_worktree(ce))
			item->util = item; /* better a valid pointer than a fake one */
	}

	ret = report_path_error(m, pattern);
	मुक्त(m);
	वापस ret;
}

अटल व्योम add_remove_files(काष्ठा string_list *list)
{
	पूर्णांक i;
	क्रम (i = 0; i < list->nr; i++) {
		काष्ठा stat st;
		काष्ठा string_list_item *p = &(list->वस्तुएँ[i]);

		/* p->util is skip-worktree */
		अगर (p->util)
			जारी;

		अगर (!lstat(p->string, &st)) {
			अगर (add_to_सूचकांक(the_repository->सूचकांक, p->string, &st, 0))
				die(_("updating files failed"));
		} अन्यथा
			remove_file_from_सूचकांक(the_repository->सूचकांक, p->string);
	}
}

अटल व्योम create_base_सूचकांक(स्थिर काष्ठा commit *current_शीर्ष)
{
	काष्ठा वृक्ष *वृक्ष;
	काष्ठा unpack_trees_options opts;
	काष्ठा वृक्ष_desc t;

	अगर (!current_शीर्ष) {
		discard_सूचकांक(the_repository->सूचकांक);
		वापस;
	}

	स_रखो(&opts, 0, माप(opts));
	opts.शीर्ष_idx = 1;
	opts.सूचकांक_only = 1;
	opts.merge = 1;
	opts.src_सूचकांक = the_repository->सूचकांक;
	opts.dst_सूचकांक = the_repository->सूचकांक;

	opts.fn = oneway_merge;
	वृक्ष = repo_parse_वृक्ष_indirect(the_repository,
					&current_शीर्ष->object.oid);
	अगर (!वृक्ष)
		die(_("failed to unpack HEAD tree object"));
	अगर (repo_parse_वृक्ष(the_repository, वृक्ष) < 0)
		निकास(128);
	init_वृक्ष_desc(&t, &वृक्ष->object.oid, वृक्ष->बफर, वृक्ष->आकार);
	अगर (unpack_trees(1, &t, &opts))
		निकास(128); /* We've already reported the error, finish dying */
}

अटल व्योम refresh_cache_or_die(पूर्णांक refresh_flags)
{
	/*
	 * refresh_flags contains REFRESH_QUIET, so the only errors
	 * are for unmerged entries.
	 */
	अगर (refresh_सूचकांक(the_repository->सूचकांक, refresh_flags | REFRESH_IN_PORCELAIN, शून्य, शून्य, शून्य))
		die_resolve_conflict("commit");
}

अटल स्थिर अक्षर *prepare_सूचकांक(स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
				 स्थिर काष्ठा commit *current_शीर्ष, पूर्णांक is_status)
{
	काष्ठा string_list partial = STRING_LIST_INIT_DUP;
	काष्ठा pathspec pathspec;
	पूर्णांक refresh_flags = REFRESH_QUIET;
	स्थिर अक्षर *ret;
	अक्षर *path = शून्य;

	अगर (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	अगर (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	अगर (is_status)
		refresh_flags |= REFRESH_UNMERGED;
	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_FULL,
		       prefix, argv);

	अगर (pathspec_from_file) {
		अगर (interactive)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--interactive/--patch");

		अगर (all)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "-a");

		अगर (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_FULL,
				    prefix, pathspec_from_file, pathspec_file_nul);
	} अन्यथा अगर (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	अगर (!pathspec.nr && (also || (only && !allow_empty &&
	    (!amend || (fixup_message && म_भेद(fixup_prefix, "amend"))))))
		die(_("No paths with --include/--only does not make sense."));

	अगर (repo_read_सूचकांक_preload(the_repository, &pathspec, 0) < 0)
		die(_("index file corrupt"));

	अगर (interactive) {
		अक्षर *old_सूचकांक_env = शून्य, *old_repo_सूचकांक_file;
		repo_hold_locked_सूचकांक(the_repository, &सूचकांक_lock,
				       LOCK_DIE_ON_ERROR);

		refresh_cache_or_die(refresh_flags);

		अगर (write_locked_सूचकांक(the_repository->सूचकांक, &सूचकांक_lock, 0))
			die(_("unable to create temporary index"));

		old_repo_सूचकांक_file = the_repository->सूचकांक_file;
		the_repository->सूचकांक_file =
			(अक्षर *)get_lock_file_path(&सूचकांक_lock);
		old_सूचकांक_env = xstrdup_or_null(दो_पर्या(सूचकांक_ENVIRONMENT));
		setenv(सूचकांक_ENVIRONMENT, the_repository->सूचकांक_file, 1);

		अगर (interactive_add(the_repository, argv, prefix, patch_interactive, &interactive_opts) != 0)
			die(_("interactive add failed"));

		the_repository->सूचकांक_file = old_repo_सूचकांक_file;
		अगर (old_सूचकांक_env && *old_सूचकांक_env)
			setenv(सूचकांक_ENVIRONMENT, old_सूचकांक_env, 1);
		अन्यथा
			unsetenv(सूचकांक_ENVIRONMENT);
		FREE_AND_NULL(old_सूचकांक_env);

		discard_सूचकांक(the_repository->सूचकांक);
		read_सूचकांक_from(the_repository->सूचकांक, get_lock_file_path(&सूचकांक_lock),
				repo_get_git_dir(the_repository));
		अगर (cache_वृक्ष_update(the_repository->सूचकांक, WRITE_वृक्ष_SILENT) == 0) {
			अगर (reopen_lock_file(&सूचकांक_lock) < 0)
				die(_("unable to write index file"));
			अगर (write_locked_सूचकांक(the_repository->सूचकांक, &सूचकांक_lock, 0))
				die(_("unable to update temporary index"));
		} अन्यथा
			warning(_("Failed to update main cache tree"));

		commit_style = COMMIT_NORMAL;
		ret = get_lock_file_path(&सूचकांक_lock);
		जाओ out;
	} अन्यथा {
		अगर (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--interactive/--patch");
		अगर (interactive_opts.interhunkcontext != -1)
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
	अगर (all || (also && pathspec.nr)) {
		अक्षर *ps_matched = xcalloc(pathspec.nr, 1);
		repo_hold_locked_सूचकांक(the_repository, &सूचकांक_lock,
				       LOCK_DIE_ON_ERROR);
		add_files_to_cache(the_repository, also ? prefix : शून्य,
				   &pathspec, ps_matched, 0, 0, 0 );
		अगर (!all && report_path_error(ps_matched, &pathspec))
			निकास(128);

		refresh_cache_or_die(refresh_flags);
		cache_वृक्ष_update(the_repository->सूचकांक, WRITE_वृक्ष_SILENT);
		अगर (write_locked_सूचकांक(the_repository->सूचकांक, &सूचकांक_lock, 0))
			die(_("unable to write new index file"));
		commit_style = COMMIT_NORMAL;
		ret = get_lock_file_path(&सूचकांक_lock);
		मुक्त(ps_matched);
		जाओ out;
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
	अगर (!only && !pathspec.nr) {
		repo_hold_locked_सूचकांक(the_repository, &सूचकांक_lock,
				       LOCK_DIE_ON_ERROR);
		refresh_cache_or_die(refresh_flags);
		अगर (the_repository->सूचकांक->cache_changed
		    || !cache_वृक्ष_fully_valid(the_repository->सूचकांक))
			cache_वृक्ष_update(the_repository->सूचकांक, WRITE_वृक्ष_SILENT);
		अगर (write_locked_सूचकांक(the_repository->सूचकांक, &सूचकांक_lock,
				       COMMIT_LOCK | SKIP_IF_UNCHANGED))
			die(_("unable to write new index file"));
		commit_style = COMMIT_AS_IS;
		ret = repo_get_सूचकांक_file(the_repository);
		जाओ out;
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

	अगर (whence != FROM_COMMIT) {
		अगर (whence == FROM_MERGE)
			die(_("cannot do a partial commit during a merge."));
		अन्यथा अगर (is_from_cherry_pick(whence))
			die(_("cannot do a partial commit during a cherry-pick."));
		अन्यथा अगर (is_from_rebase(whence))
			die(_("cannot do a partial commit during a rebase."));
	}

	अगर (list_paths(&partial, !current_शीर्ष ? शून्य : "HEAD", &pathspec))
		निकास(1);

	discard_सूचकांक(the_repository->सूचकांक);
	अगर (repo_read_सूचकांक(the_repository) < 0)
		die(_("cannot read the index"));

	repo_hold_locked_सूचकांक(the_repository, &सूचकांक_lock, LOCK_DIE_ON_ERROR);
	add_remove_files(&partial);
	refresh_सूचकांक(the_repository->सूचकांक, REFRESH_QUIET, शून्य, शून्य, शून्य);
	cache_वृक्ष_update(the_repository->सूचकांक, WRITE_वृक्ष_SILENT);
	अगर (write_locked_सूचकांक(the_repository->सूचकांक, &सूचकांक_lock, 0))
		die(_("unable to write new index file"));

	path = repo_git_path(the_repository, "next-index-%"PRIuMAX,
			     (uintmax_t) getpid());
	hold_lock_file_for_update(&false_lock, path,
				  LOCK_DIE_ON_ERROR);

	create_base_सूचकांक(current_शीर्ष);
	add_remove_files(&partial);
	refresh_सूचकांक(the_repository->सूचकांक, REFRESH_QUIET, शून्य, शून्य, शून्य);

	अगर (write_locked_सूचकांक(the_repository->सूचकांक, &false_lock, 0))
		die(_("unable to write temporary index file"));

	discard_सूचकांक(the_repository->सूचकांक);
	ret = get_lock_file_path(&false_lock);
	read_सूचकांक_from(the_repository->सूचकांक, ret, repo_get_git_dir(the_repository));
out:
	string_list_clear(&partial, 0);
	clear_pathspec(&pathspec);
	मुक्त(path);
	वापस ret;
}

अटल पूर्णांक run_status(खाता *fp, स्थिर अक्षर *सूचकांक_file, स्थिर अक्षर *prefix, पूर्णांक nowarn,
		      काष्ठा wt_status *s)
{
	काष्ठा object_id oid;

	अगर (s->relative_paths)
		s->prefix = prefix;

	अगर (amend) {
		s->amend = 1;
		s->reference = "HEAD^1";
	}
	s->verbose = verbose;
	s->सूचकांक_file = सूचकांक_file;
	s->fp = fp;
	s->nowarn = nowarn;
	s->is_initial = repo_get_oid(the_repository, s->reference, &oid) ? 1 : 0;
	अगर (!s->is_initial)
		oidcpy(&s->oid_commit, &oid);
	s->status_format = status_format;
	s->ignore_submodule_arg = ignore_submodule_arg;

	wt_status_collect(s);
	wt_status_print(s);
	wt_status_collect_free_buffers(s);

	वापस s->committable;
}

अटल पूर्णांक is_a_merge(स्थिर काष्ठा commit *current_शीर्ष)
{
	वापस !!(current_शीर्ष->parents && current_शीर्ष->parents->अग्र);
}

अटल व्योम assert_split_ident(काष्ठा ident_split *id, स्थिर काष्ठा strbuf *buf)
{
	अगर (split_ident_पंक्ति(id, buf->buf, buf->len) || !id->date_begin)
		BUG("unable to parse our own ident: %s", buf->buf);
}

अटल व्योम export_one(स्थिर अक्षर *var, स्थिर अक्षर *s, स्थिर अक्षर *e, पूर्णांक hack)
{
	काष्ठा strbuf buf = STRBUF_INIT;
	अगर (hack)
		strbuf_addch(&buf, hack);
	strbuf_add(&buf, s, e - s);
	setenv(var, buf.buf, 1);
	strbuf_release(&buf);
}

अटल पूर्णांक parse_force_date(स्थिर अक्षर *in, काष्ठा strbuf *out)
{
	strbuf_addch(out, '@');

	अगर (parse_date(in, out) < 0) {
		पूर्णांक errors = 0;
		अचिन्हित दीर्घ t = approxidate_careful(in, &errors);
		अगर (errors)
			वापस -1;
		strbuf_addf(out, "%lu", t);
	}

	वापस 0;
}

अटल व्योम set_ident_var(अक्षर **buf, अक्षर *val)
{
	मुक्त(*buf);
	*buf = val;
}

अटल व्योम determine_author_info(काष्ठा strbuf *author_ident)
{
	अक्षर *name, *email, *date;
	काष्ठा ident_split author;

	name = xstrdup_or_null(दो_पर्या("GIT_AUTHOR_NAME"));
	email = xstrdup_or_null(दो_पर्या("GIT_AUTHOR_EMAIL"));
	date = xstrdup_or_null(दो_पर्या("GIT_AUTHOR_DATE"));

	अगर (author_message) {
		काष्ठा ident_split ident;
		माप_प्रकार len;
		स्थिर अक्षर *a;

		a = find_commit_header(author_message_बफर, "author", &len);
		अगर (!a)
			die(_("commit '%s' lacks author header"), author_message);
		अगर (split_ident_पंक्ति(&ident, a, len) < 0)
			die(_("commit '%s' has malformed author line"), author_message);

		set_ident_var(&name, xmemdupz(ident.name_begin, ident.name_end - ident.name_begin));
		set_ident_var(&email, xmemdupz(ident.mail_begin, ident.mail_end - ident.mail_begin));

		अगर (ident.date_begin) {
			काष्ठा strbuf date_buf = STRBUF_INIT;
			strbuf_addch(&date_buf, '@');
			strbuf_add(&date_buf, ident.date_begin, ident.date_end - ident.date_begin);
			strbuf_addch(&date_buf, ' ');
			strbuf_add(&date_buf, ident.tz_begin, ident.tz_end - ident.tz_begin);
			set_ident_var(&date, strbuf_detach(&date_buf, शून्य));
		}
	}

	अगर (force_author) {
		काष्ठा ident_split ident;

		अगर (split_ident_पंक्ति(&ident, force_author, म_माप(force_author)) < 0)
			die(_("malformed --author parameter"));
		set_ident_var(&name, xmemdupz(ident.name_begin, ident.name_end - ident.name_begin));
		set_ident_var(&email, xmemdupz(ident.mail_begin, ident.mail_end - ident.mail_begin));
	}

	अगर (force_date) {
		काष्ठा strbuf date_buf = STRBUF_INIT;
		अगर (parse_force_date(force_date, &date_buf))
			die(_("invalid date format: %s"), force_date);
		set_ident_var(&date, strbuf_detach(&date_buf, शून्य));
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

अटल पूर्णांक author_date_is_interesting(व्योम)
{
	वापस author_message || force_date;
}

#अगर_अघोषित WITH_BREAKING_CHANGES
अटल व्योम adjust_comment_पंक्ति_char(स्थिर काष्ठा strbuf *sb)
{
	अक्षर candidates[] = "#;@!$%^&|:";
	अक्षर *candidate;
	स्थिर अक्षर *p;
	माप_प्रकार cutoff;

	/* Ignore comment chars in trailing comments (e.g., Conflicts:) */
	cutoff = sb->len - ignored_log_message_bytes(sb->buf, sb->len);

	अगर (!स_प्रथम(sb->buf, candidates[0], sb->len)) {
		मुक्त(comment_पंक्ति_str_to_free);
		comment_पंक्ति_str = comment_पंक्ति_str_to_free =
			xstrfmt("%c", candidates[0]);
		वापस;
	}

	p = sb->buf;
	candidate = म_अक्षर(candidates, *p);
	अगर (candidate)
		*candidate = ' ';
	क्रम (p = sb->buf; p + 1 < sb->buf + cutoff; p++) {
		अगर ((p[0] == '\n' || p[0] == '\r') && p[1]) {
			candidate = म_अक्षर(candidates, p[1]);
			अगर (candidate)
				*candidate = ' ';
		}
	}

	क्रम (p = candidates; *p == ' '; p++)
		;
	अगर (!*p)
		die(_("unable to select a comment character that is not used\n"
		      "in the current commit message"));
	मुक्त(comment_पंक्ति_str_to_free);
	comment_पंक्ति_str = comment_पंक्ति_str_to_free = xstrfmt("%c", *p);
}
#पूर्ण_अगर /* !WITH_BREAKING_CHANGES */

अटल व्योम prepare_amend_commit(काष्ठा commit *commit, काष्ठा strbuf *sb,
				काष्ठा pretty_print_context *ctx)
{
	स्थिर अक्षर *बफर, *subject, *fmt;

	बफर = repo_get_commit_बफर(the_repository, commit, शून्य);
	find_commit_subject(बफर, &subject);
	/*
	 * If we amend the 'amend!' commit then we don't want to
	 * duplicate the subject line.
	 */
	fmt = starts_with(subject, "amend!") ? "%b" : "%B";
	repo_format_commit_message(the_repository, commit, fmt, sb, ctx);
	repo_unuse_commit_बफर(the_repository, commit, बफर);
}

अटल व्योम change_तथ्य_free(व्योम *util, स्थिर अक्षर *str UNUSED)
{
	काष्ठा wt_status_change_तथ्य *d = util;
	मुक्त(d->rename_source);
	मुक्त(d);
}

अटल पूर्णांक prepare_to_commit(स्थिर अक्षर *सूचकांक_file, स्थिर अक्षर *prefix,
			     काष्ठा commit *current_शीर्ष,
			     काष्ठा wt_status *s,
			     काष्ठा strbuf *author_ident)
{
	काष्ठा stat statbuf;
	काष्ठा strbuf committer_ident = STRBUF_INIT;
	पूर्णांक committable;
	काष्ठा strbuf sb = STRBUF_INIT;
	स्थिर अक्षर *hook_arg1 = शून्य;
	स्थिर अक्षर *hook_arg2 = शून्य;
	पूर्णांक clean_message_contents = (cleanup_mode != COMMIT_संदेश_CLEANUP_NONE);
	पूर्णांक old_display_comment_prefix;
	पूर्णांक invoked_hook;

	/* This checks and barfs if author is badly specified */
	determine_author_info(author_ident);

	अगर (!no_verify && run_commit_hook(use_editor, सूचकांक_file, &invoked_hook,
					  "pre-commit", शून्य))
		वापस 0;

	अगर (squash_message) {
		/*
		 * Insert the proper subject line before other commit
		 * message options add their content.
		 */
		अगर (use_message && !म_भेद(use_message, squash_message))
			strbuf_addstr(&sb, "squash! ");
		अन्यथा {
			काष्ठा pretty_print_context ctx = {0};
			काष्ठा commit *c;
			c = lookup_commit_reference_by_name(squash_message);
			अगर (!c)
				die(_("could not lookup commit '%s'"), squash_message);
			ctx.output_encoding = get_commit_output_encoding();
			repo_format_commit_message(the_repository, c,
						   "squash! %s\n\n", &sb,
						   &ctx);
		}
	}

	अगर (have_option_m && !fixup_message) {
		strbuf_addbuf(&sb, &message);
		hook_arg1 = "message";
	} अन्यथा अगर (logfile && !म_भेद(logfile, "-")) {
		अगर (isatty(0))
			ख_लिखो(मानक_त्रुटि, _("(reading log message from standard input)\n"));
		अगर (strbuf_read(&sb, 0, 0) < 0)
			die_errno(_("could not read log from standard input"));
		hook_arg1 = "message";
	} अन्यथा अगर (logfile) {
		अगर (strbuf_read_file(&sb, logfile, 0) < 0)
			die_errno(_("could not read log file '%s'"),
				  logfile);
		hook_arg1 = "message";
	} अन्यथा अगर (use_message) {
		स्थिर अक्षर *बफर;
		बफर = म_माला(use_message_बफर, "\n\n");
		अगर (बफर)
			strbuf_addstr(&sb, skip_blank_पंक्तियाँ(बफर + 2));
		hook_arg1 = "commit";
		hook_arg2 = use_message;
	} अन्यथा अगर (fixup_message) {
		काष्ठा pretty_print_context ctx = {0};
		काष्ठा commit *commit;
		अक्षर *fmt;
		commit = lookup_commit_reference_by_name(fixup_commit);
		अगर (!commit)
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
		अगर (have_option_m && !म_भेद(fixup_prefix, "fixup"))
			strbuf_addbuf(&sb, &message);

		अगर (!म_भेद(fixup_prefix, "amend")) {
			अगर (have_option_m)
				die(_("options '%s' and '%s:%s' cannot be used together"), "-m", "--fixup", fixup_message);
			prepare_amend_commit(commit, &sb, &ctx);
		}
	} अन्यथा अगर (!stat(git_path_merge_संदेश(the_repository), &statbuf)) {
		माप_प्रकार merge_संदेश_start;

		/*
		 * prepend SQUASH_MSG here if it exists and a
		 * "merge --squash" was originally performed
		 */
		अगर (!stat(git_path_squash_संदेश(the_repository), &statbuf)) {
			अगर (strbuf_read_file(&sb, git_path_squash_संदेश(the_repository), 0) < 0)
				die_errno(_("could not read SQUASH_MSG"));
			hook_arg1 = "squash";
		} अन्यथा
			hook_arg1 = "merge";

		merge_संदेश_start = sb.len;
		अगर (strbuf_read_file(&sb, git_path_merge_संदेश(the_repository), 0) < 0)
			die_errno(_("could not read MERGE_MSG"));

		अगर (cleanup_mode == COMMIT_संदेश_CLEANUP_SCISSORS &&
		    wt_status_locate_end(sb.buf + merge_संदेश_start,
					 sb.len - merge_संदेश_start) <
				sb.len - merge_संदेश_start)
			s->added_cut_पंक्ति = 1;
	} अन्यथा अगर (!stat(git_path_squash_संदेश(the_repository), &statbuf)) {
		अगर (strbuf_read_file(&sb, git_path_squash_संदेश(the_repository), 0) < 0)
			die_errno(_("could not read SQUASH_MSG"));
		hook_arg1 = "squash";
	} अन्यथा अगर (template_file) {
		अगर (strbuf_read_file(&sb, template_file, 0) < 0)
			die_errno(_("could not read '%s'"), template_file);
		hook_arg1 = "template";
		clean_message_contents = 0;
	}

	/*
	 * The remaining cases don't modify the template message, but
	 * just set the argument(s) to the prepare-commit-msg hook.
	 */
	अन्यथा अगर (whence == FROM_MERGE)
		hook_arg1 = "merge";
	अन्यथा अगर (is_from_cherry_pick(whence) || whence == FROM_REBASE_PICK) {
		hook_arg1 = "commit";
		hook_arg2 = "CHERRY_PICK_HEAD";
	}

	अगर (squash_message) {
		/*
		 * If squash_commit was used for the commit subject,
		 * then we're possibly hijacking other commit log options.
		 * Reset the hook args to tell the real story.
		 */
		hook_arg1 = "message";
		hook_arg2 = "";
	}

	s->fp = fopen_for_writing(git_path_commit_editmsg());
	अगर (!s->fp)
		die_errno(_("could not open '%s'"), git_path_commit_editmsg());

	/* Ignore status.displayCommentPrefix: we do need comments in COMMIT_EDITMSG. */
	old_display_comment_prefix = s->display_comment_prefix;
	s->display_comment_prefix = 1;

	/*
	 * Most hints are counter-productive when the commit has
	 * already started.
	 */
	s->hints = 0;

	अगर (clean_message_contents)
		strbuf_stripspace(&sb, शून्य);

	अगर (signoff)
		append_signoff(&sb, ignored_log_message_bytes(sb.buf, sb.len), 0);

	अगर (ख_डालो(sb.buf, 1, sb.len, s->fp) < sb.len)
		die_errno(_("could not write commit template"));

#अगर_अघोषित WITH_BREAKING_CHANGES
	अगर (auto_comment_पंक्ति_char)
		adjust_comment_पंक्ति_char(&sb);
#पूर्ण_अगर /* !WITH_BREAKING_CHANGES */
	strbuf_release(&sb);

	/* This checks if committer ident is explicitly given */
	strbuf_addstr(&committer_ident, git_committer_info(IDENT_STRICT));
	अगर (use_editor && include_status) {
		पूर्णांक ident_shown = 0;
		क्रमागत git_colorbool saved_color_setting;
		काष्ठा ident_split ci, ai;
		स्थिर अक्षर *hint_cleanup_all = allow_empty_message ?
			_("Please enter the commit message for your changes."
			  " Lines starting\nwith '%s' will be ignored.\n") :
			_("Please enter the commit message for your changes."
			  " Lines starting\nwith '%s' will be ignored, and an empty"
			  " message aborts the commit.\n");
		स्थिर अक्षर *hint_cleanup_space = allow_empty_message ?
			_("Please enter the commit message for your changes."
			  " Lines starting\n"
			  "with '%s' will be kept; you may remove them"
			  " yourself if you want to.\n") :
			_("Please enter the commit message for your changes."
			  " Lines starting\n"
			  "with '%s' will be kept; you may remove them"
			  " yourself if you want to.\n"
			  "An empty message aborts the commit.\n");
		अगर (whence != FROM_COMMIT) {
			अगर (cleanup_mode == COMMIT_संदेश_CLEANUP_SCISSORS)
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
		अगर (cleanup_mode == COMMIT_संदेश_CLEANUP_ALL)
			status_printf(s, GIT_COLOR_NORMAL, hint_cleanup_all, comment_पंक्ति_str);
		अन्यथा अगर (cleanup_mode == COMMIT_संदेश_CLEANUP_SCISSORS) {
			अगर (whence == FROM_COMMIT)
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

		अगर (ident_cmp(&ai, &ci))
			status_printf_ln(s, GIT_COLOR_NORMAL,
				_("%s"
				"Author:    %.*s <%.*s>"),
				ident_shown++ ? "" : "\n",
				(पूर्णांक)(ai.name_end - ai.name_begin), ai.name_begin,
				(पूर्णांक)(ai.mail_end - ai.mail_begin), ai.mail_begin);

		अगर (author_date_is_interesting())
			status_printf_ln(s, GIT_COLOR_NORMAL,
				_("%s"
				"Date:      %s"),
				ident_shown++ ? "" : "\n",
				show_ident_date(&ai, DATE_MODE(NORMAL)));

		अगर (!committer_ident_sufficiently_given())
			status_printf_ln(s, GIT_COLOR_NORMAL,
				_("%s"
				"Committer: %.*s <%.*s>"),
				ident_shown++ ? "" : "\n",
				(पूर्णांक)(ci.name_end - ci.name_begin), ci.name_begin,
				(पूर्णांक)(ci.mail_end - ci.mail_begin), ci.mail_begin);

		status_printf_ln(s, GIT_COLOR_NORMAL, "%s", ""); /* Add new line for clarity */

		saved_color_setting = s->use_color;
		s->use_color = GIT_COLOR_NEVER;
		committable = run_status(s->fp, सूचकांक_file, prefix, 1, s);
		s->use_color = saved_color_setting;
		string_list_clear_func(&s->change, change_तथ्य_free);
	} अन्यथा {
		काष्ठा object_id oid;
		स्थिर अक्षर *parent = "HEAD";

		अगर (!the_repository->सूचकांक->initialized && repo_read_सूचकांक(the_repository) < 0)
			die(_("Cannot read index"));

		अगर (amend)
			parent = "HEAD^1";

		अगर (repo_get_oid(the_repository, parent, &oid)) {
			पूर्णांक i, ita_nr = 0;

			/* TODO: audit for interaction with sparse-index. */
			ensure_full_सूचकांक(the_repository->सूचकांक);
			क्रम (i = 0; i < the_repository->सूचकांक->cache_nr; i++)
				अगर (ce_intent_to_add(the_repository->सूचकांक->cache[i]))
					ita_nr++;
			committable = the_repository->सूचकांक->cache_nr > ita_nr;
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
			काष्ठा diff_flags flags = DIFF_FLAGS_INIT;
			flags.override_submodule_config = 1;
			अगर (ignore_submodule_arg &&
			    !म_भेद(ignore_submodule_arg, "all"))
				flags.ignore_submodules = 1;
			committable = सूचकांक_differs_from(the_repository,
							 parent, &flags, 1);
		}
	}
	strbuf_release(&committer_ident);

	ख_बंद(s->fp);

	अगर (trailer_args.nr) {
		अगर (amend_file_with_trailers(git_path_commit_editmsg(), &trailer_args))
			die(_("unable to pass trailers to --trailers"));
		strvec_clear(&trailer_args);
	}

	/*
	 * Reject an attempt to record a non-merge empty commit without
	 * explicit --allow-empty. In the cherry-pick case, it may be
	 * empty due to conflict resolution, which the user should okay.
	 */
	अगर (!committable && whence != FROM_MERGE && !allow_empty &&
	    !(amend && is_a_merge(current_शीर्ष))) {
		s->hints = advice_enabled(ADVICE_STATUS_HINTS);
		s->display_comment_prefix = old_display_comment_prefix;
		run_status(मानक_निकास, सूचकांक_file, prefix, 0, s);
		अगर (amend)
			ख_माला_दो(_(empty_amend_advice), मानक_त्रुटि);
		अन्यथा अगर (is_from_cherry_pick(whence) ||
			 whence == FROM_REBASE_PICK) {
			ख_माला_दो(_(empty_cherry_pick_advice), मानक_त्रुटि);
			अगर (whence == FROM_CHERRY_PICK_SINGLE)
				ख_माला_दो(_(empty_cherry_pick_advice_single), मानक_त्रुटि);
			अन्यथा अगर (whence == FROM_CHERRY_PICK_MULTI)
				ख_माला_दो(_(empty_cherry_pick_advice_multi), मानक_त्रुटि);
			अन्यथा
				ख_माला_दो(_(empty_rebase_pick_advice), मानक_त्रुटि);
		}
		वापस 0;
	}

	अगर (!no_verify && invoked_hook) {
		/*
		 * Re-read the index as the pre-commit-commit hook was invoked
		 * and could have updated it. We must do this before we invoke
		 * the editor and after we invoke run_status above.
		 */
		discard_सूचकांक(the_repository->सूचकांक);
	}
	read_सूचकांक_from(the_repository->सूचकांक, सूचकांक_file, repo_get_git_dir(the_repository));

	अगर (cache_वृक्ष_update(the_repository->सूचकांक, 0)) {
		error(_("Error building trees"));
		वापस 0;
	}

	अगर (run_commit_hook(use_editor, सूचकांक_file, शून्य, "prepare-commit-msg",
			    git_path_commit_editmsg(), hook_arg1, hook_arg2, शून्य))
		वापस 0;

	अगर (use_editor) {
		काष्ठा strvec env = STRVEC_INIT;

		strvec_pushf(&env, "GIT_INDEX_FILE=%s", सूचकांक_file);
		अगर (launch_editor(git_path_commit_editmsg(), शून्य, env.v)) {
			ख_लिखो(मानक_त्रुटि,
			_("Please supply the message using either -m or -F option.\n"));
			निकास(1);
		}
		strvec_clear(&env);
	}

	अगर (!no_verify &&
	    run_commit_hook(use_editor, सूचकांक_file, शून्य, "commit-msg",
			    git_path_commit_editmsg(), शून्य)) {
		वापस 0;
	}

	वापस 1;
}

अटल स्थिर अक्षर *find_author_by_nickname(स्थिर अक्षर *name)
{
	काष्ठा rev_info revs;
	काष्ठा commit *commit;
	काष्ठा strbuf buf = STRBUF_INIT;
	स्थिर अक्षर *av[20];
	पूर्णांक ac = 0;

	repo_init_revisions(the_repository, &revs, शून्य);
	strbuf_addf(&buf, "--author=%s", name);
	av[++ac] = "--all";
	av[++ac] = "-i";
	av[++ac] = buf.buf;
	av[++ac] = शून्य;
	setup_revisions(ac, av, &revs, शून्य);
	revs.mailmap = xmalloc(माप(काष्ठा string_list));
	string_list_init_nodup(revs.mailmap);
	read_mailmap(the_repository, revs.mailmap);

	अगर (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	commit = get_revision(&revs);
	अगर (commit) {
		काष्ठा pretty_print_context ctx = {0};
		ctx.date_mode.type = DATE_NORMAL;
		strbuf_release(&buf);
		repo_format_commit_message(the_repository, commit,
					   "%aN <%aE>", &buf, &ctx);
		release_revisions(&revs);
		वापस strbuf_detach(&buf, शून्य);
	}
	die(_("--author '%s' is not 'Name <email>' and matches no existing author"), name);
}

अटल व्योम handle_ignored_arg(काष्ठा wt_status *s)
{
	अगर (!ignored_arg)
		; /* default already initialized */
	अन्यथा अगर (!म_भेद(ignored_arg, "traditional"))
		s->show_ignored_mode = SHOW_TRADITIONAL_IGNORED;
	अन्यथा अगर (!म_भेद(ignored_arg, "no"))
		s->show_ignored_mode = SHOW_NO_IGNORED;
	अन्यथा अगर (!म_भेद(ignored_arg, "matching"))
		s->show_ignored_mode = SHOW_MATCHING_IGNORED;
	अन्यथा
		die(_("Invalid ignored mode '%s'"), ignored_arg);
}

अटल क्रमागत untracked_status_type parse_untracked_setting_name(स्थिर अक्षर *u)
{
	/*
	 * Please update $__git_untracked_file_modes in
	 * git-completion.bash when you add new options
	 */
	चयन (git_parse_maybe_bool(u)) {
	हाल 0:
		u = "no";
		अवरोध;
	हाल 1:
		u = "normal";
		अवरोध;
	शेष:
		अवरोध;
	}

	अगर (!म_भेद(u, "no"))
		वापस SHOW_NO_UNTRACKED_FILES;
	अन्यथा अगर (!म_भेद(u, "normal"))
		वापस SHOW_NORMAL_UNTRACKED_FILES;
	अन्यथा अगर (!म_भेद(u, "all"))
		वापस SHOW_ALL_UNTRACKED_FILES;
	अन्यथा
		वापस SHOW_UNTRACKED_FILES_ERROR;
}

अटल व्योम handle_untracked_files_arg(काष्ठा wt_status *s)
{
	क्रमागत untracked_status_type u;

	अगर (!untracked_files_arg)
		वापस; /* default already initialized */

	u = parse_untracked_setting_name(untracked_files_arg);
	अगर (u == SHOW_UNTRACKED_FILES_ERROR)
		die(_("Invalid untracked files mode '%s'"),
		    untracked_files_arg);
	s->show_untracked_files = u;
}

अटल स्थिर अक्षर *read_commit_message(स्थिर अक्षर *name)
{
	स्थिर अक्षर *out_enc;
	काष्ठा commit *commit;

	commit = lookup_commit_reference_by_name(name);
	अगर (!commit)
		die(_("could not lookup commit '%s'"), name);
	out_enc = get_commit_output_encoding();
	वापस repo_logmsg_reencode(the_repository, commit, शून्य, out_enc);
}

/*
 * Enumerate what needs to be propagated when --porcelain
 * is not in effect here.
 */
अटल काष्ठा status_deferred_config {
	क्रमागत wt_status_format status_format;
	पूर्णांक show_branch;
	क्रमागत ahead_behind_flags ahead_behind;
} status_deferred_config = {
	STATUS_FORMAT_UNSPECIFIED,
	-1, /* unspecified */
	AHEAD_BEHIND_UNSPECIFIED,
};

अटल व्योम finalize_deferred_config(काष्ठा wt_status *s)
{
	पूर्णांक use_deferred_config = (status_format != STATUS_FORMAT_PORCELAIN &&
				   status_format != STATUS_FORMAT_PORCELAIN_V2 &&
				   !s->null_termination);

	अगर (s->null_termination) {
		अगर (status_format == STATUS_FORMAT_NONE ||
		    status_format == STATUS_FORMAT_UNSPECIFIED)
			status_format = STATUS_FORMAT_PORCELAIN;
		अन्यथा अगर (status_format == STATUS_FORMAT_LONG)
			die(_("options '%s' and '%s' cannot be used together"), "--long", "-z");
	}

	अगर (use_deferred_config && status_format == STATUS_FORMAT_UNSPECIFIED)
		status_format = status_deferred_config.status_format;
	अगर (status_format == STATUS_FORMAT_UNSPECIFIED)
		status_format = STATUS_FORMAT_NONE;

	अगर (use_deferred_config && s->show_branch < 0)
		s->show_branch = status_deferred_config.show_branch;
	अगर (s->show_branch < 0)
		s->show_branch = 0;

	/*
	 * If the user did not give a "--[no]-ahead-behind" command
	 * line argument *AND* we will print in a human-readable format
	 * (short, long etc.) then we inherit from the status.aheadbehind
	 * config setting.  In all other cases (and porcelain V[12] formats
	 * in particular), we inherit _FULL for backwards compatibility.
	 */
	अगर (use_deferred_config &&
	    s->ahead_behind_flags == AHEAD_BEHIND_UNSPECIFIED)
		s->ahead_behind_flags = status_deferred_config.ahead_behind;

	अगर (s->ahead_behind_flags == AHEAD_BEHIND_UNSPECIFIED)
		s->ahead_behind_flags = AHEAD_BEHIND_FULL;
}

अटल व्योम check_fixup_reword_options(पूर्णांक argc, स्थिर अक्षर *argv[]) {
	अगर (whence != FROM_COMMIT) {
		अगर (whence == FROM_MERGE)
			die(_("You are in the middle of a merge -- cannot reword."));
		अन्यथा अगर (is_from_cherry_pick(whence))
			die(_("You are in the middle of a cherry-pick -- cannot reword."));
	}
	अगर (argc)
		die(_("reword option of '%s' and path '%s' cannot be used together"), "--fixup", *argv);
	अगर (patch_interactive || interactive || all || also || only)
		die(_("reword option of '%s' and '%s' cannot be used together"),
			"--fixup", "--patch/--interactive/--all/--include/--only");
}

अटल पूर्णांक parse_and_validate_options(पूर्णांक argc, स्थिर अक्षर *argv[],
				      स्थिर काष्ठा option *options,
				      स्थिर अक्षर * स्थिर usage[],
				      स्थिर अक्षर *prefix,
				      काष्ठा commit *current_शीर्ष,
				      काष्ठा wt_status *s)
{
	argc = parse_options(argc, argv, prefix, options, usage, 0);
	finalize_deferred_config(s);

	अगर (force_author && !म_अक्षर(force_author, '>'))
		force_author = find_author_by_nickname(force_author);

	अगर (force_author && renew_authorship)
		die(_("options '%s' and '%s' cannot be used together"), "--reset-author", "--author");

	अगर (logfile || have_option_m || use_message)
		use_editor = 0;

	/* Sanity check options */
	अगर (amend && !current_शीर्ष)
		die(_("You have nothing to amend."));
	अगर (amend && whence != FROM_COMMIT) {
		अगर (whence == FROM_MERGE)
			die(_("You are in the middle of a merge -- cannot amend."));
		अन्यथा अगर (is_from_cherry_pick(whence))
			die(_("You are in the middle of a cherry-pick -- cannot amend."));
		अन्यथा अगर (whence == FROM_REBASE_PICK)
			die(_("You are in the middle of a rebase -- cannot amend."));
	}
	अगर (fixup_message && squash_message)
		die(_("options '%s' and '%s' cannot be used together"), "--squash", "--fixup");
	die_for_incompatible_opt4(!!use_message, "-C",
				  !!edit_message, "-c",
				  !!logfile, "-F",
				  !!fixup_message, "--fixup");
	die_for_incompatible_opt4(have_option_m, "-m",
				  !!edit_message, "-c",
				  !!use_message, "-C",
				  !!logfile, "-F");
	अगर (use_message || edit_message || logfile ||fixup_message || have_option_m)
		FREE_AND_NULL(template_file);
	अगर (edit_message)
		use_message = edit_message;
	अगर (amend && !use_message && !fixup_message)
		use_message = "HEAD";
	अगर (!use_message && !is_from_cherry_pick(whence) &&
	    !is_from_rebase(whence) && renew_authorship)
		die(_("--reset-author can be used only with -C, -c or --amend."));
	अगर (use_message) {
		use_message_बफर = read_commit_message(use_message);
		अगर (!renew_authorship) {
			author_message = use_message;
			author_message_बफर = use_message_बफर;
		}
	}
	अगर ((is_from_cherry_pick(whence) || whence == FROM_REBASE_PICK) &&
	    !renew_authorship) {
		author_message = "CHERRY_PICK_HEAD";
		author_message_बफर = read_commit_message(author_message);
	}

	अगर (patch_interactive)
		interactive = 1;

	die_for_incompatible_opt4(also, "-i/--include",
				  only, "-o/--only",
				  all, "-a/--all",
				  interactive, "--interactive/-p/--patch");
	अगर (fixup_message) {
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
		अक्षर *p = fixup_message;
		जबतक (है_अक्षर(*p))
			p++;
		अगर (p > fixup_message && *p == ':') {
			*p = '\0';
			fixup_commit = p + 1;
			अगर (!म_भेद("amend", fixup_message) ||
			    !म_भेद("reword", fixup_message)) {
				fixup_prefix = "amend";
				allow_empty = 1;
				अगर (*fixup_message == 'r') {
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

	अगर (0 <= edit_flag)
		use_editor = edit_flag;

	handle_untracked_files_arg(s);

	अगर (all && argc > 0)
		die(_("paths '%s ...' with -a does not make sense"),
		    argv[0]);

	अगर (status_format != STATUS_FORMAT_NONE)
		dry_run = 1;

	वापस argc;
}

अटल पूर्णांक dry_run_commit(स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
			  स्थिर काष्ठा commit *current_शीर्ष, काष्ठा wt_status *s)
{
	पूर्णांक committable;
	स्थिर अक्षर *सूचकांक_file;

	सूचकांक_file = prepare_सूचकांक(argv, prefix, current_शीर्ष, 1);
	committable = run_status(मानक_निकास, सूचकांक_file, prefix, 0, s);
	rollback_सूचकांक_files();

	वापस committable ? 0 : 1;
}

define_list_config_array_extra(color_status_slots, {"added"});

अटल पूर्णांक parse_status_slot(स्थिर अक्षर *slot)
{
	अगर (!strcasecmp(slot, "added"))
		वापस WT_STATUS_UPDATED;

	वापस LOOKUP_CONFIG(color_status_slots, slot);
}

अटल पूर्णांक git_status_config(स्थिर अक्षर *k, स्थिर अक्षर *v,
			     स्थिर काष्ठा config_context *ctx, व्योम *cb)
{
	काष्ठा wt_status *s = cb;
	स्थिर अक्षर *slot_name;

	अगर (starts_with(k, "column."))
		वापस git_column_config(k, v, "status", &s->colopts);
	अगर (!म_भेद(k, "status.submodulesummary")) {
		पूर्णांक is_bool;
		s->submodule_summary = git_config_bool_or_int(k, v, ctx->kvi,
							      &is_bool);
		अगर (is_bool && s->submodule_summary)
			s->submodule_summary = -1;
		वापस 0;
	}
	अगर (!म_भेद(k, "status.short")) {
		अगर (git_config_bool(k, v))
			status_deferred_config.status_format = STATUS_FORMAT_SHORT;
		अन्यथा
			status_deferred_config.status_format = STATUS_FORMAT_NONE;
		वापस 0;
	}
	अगर (!म_भेद(k, "status.branch")) {
		status_deferred_config.show_branch = git_config_bool(k, v);
		वापस 0;
	}
	अगर (!म_भेद(k, "status.aheadbehind")) {
		status_deferred_config.ahead_behind = git_config_bool(k, v);
		वापस 0;
	}
	अगर (!म_भेद(k, "status.showstash")) {
		s->show_stash = git_config_bool(k, v);
		वापस 0;
	}
	अगर (!म_भेद(k, "status.color") || !म_भेद(k, "color.status")) {
		s->use_color = git_config_colorbool(k, v);
		वापस 0;
	}
	अगर (!म_भेद(k, "status.displaycommentprefix")) {
		s->display_comment_prefix = git_config_bool(k, v);
		वापस 0;
	}
	अगर (skip_prefix(k, "status.color.", &slot_name) ||
	    skip_prefix(k, "color.status.", &slot_name)) {
		पूर्णांक slot = parse_status_slot(slot_name);
		अगर (slot < 0)
			वापस 0;
		अगर (!v)
			वापस config_error_nonbool(k);
		वापस color_parse(v, s->color_palette[slot]);
	}
	अगर (!म_भेद(k, "status.relativepaths")) {
		s->relative_paths = git_config_bool(k, v);
		वापस 0;
	}
	अगर (!म_भेद(k, "status.showuntrackedfiles")) {
		क्रमागत untracked_status_type u;

		u = parse_untracked_setting_name(v);
		अगर (u == SHOW_UNTRACKED_FILES_ERROR)
			वापस error(_("Invalid untracked files mode '%s'"), v);
		s->show_untracked_files = u;
		वापस 0;
	}
	अगर (!म_भेद(k, "diff.renamelimit")) {
		अगर (s->rename_limit == -1)
			s->rename_limit = git_config_int(k, v, ctx->kvi);
		वापस 0;
	}
	अगर (!म_भेद(k, "status.renamelimit")) {
		s->rename_limit = git_config_int(k, v, ctx->kvi);
		वापस 0;
	}
	अगर (!म_भेद(k, "diff.renames")) {
		अगर (s->detect_rename == -1)
			s->detect_rename = git_config_rename(k, v);
		वापस 0;
	}
	अगर (!म_भेद(k, "status.renames")) {
		s->detect_rename = git_config_rename(k, v);
		वापस 0;
	}
	वापस git_diff_ui_config(k, v, ctx, शून्य);
}

पूर्णांक cmd_status(पूर्णांक argc,
स्थिर अक्षर **argv,
स्थिर अक्षर *prefix,
काष्ठा repository *repo UNUSED)
{
	अटल पूर्णांक no_renames = -1;
	अटल स्थिर अक्षर *rename_score_arg = (स्थिर अक्षर *)-1;
	अटल काष्ठा wt_status s;
	अचिन्हित पूर्णांक progress_flag = 0;
	पूर्णांक fd;
	काष्ठा object_id oid;
	अटल काष्ठा option builtin_status_options[] = {
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
	the_repository->settings.command_requires_full_सूचकांक = 0;

	status_init_config(&s, git_status_config);
	argc = parse_options(argc, argv, prefix,
			     builtin_status_options,
			     builtin_status_usage, 0);
	finalize_colopts(&s.colopts, -1);
	finalize_deferred_config(&s);

	handle_untracked_files_arg(&s);
	handle_ignored_arg(&s);

	अगर (s.show_ignored_mode == SHOW_MATCHING_IGNORED &&
	    s.show_untracked_files == SHOW_NO_UNTRACKED_FILES)
		die(_("Unsupported combination of ignored and untracked-files arguments"));

	parse_pathspec(&s.pathspec, 0,
		       PATHSPEC_PREFER_FULL,
		       prefix, argv);

	अगर (status_format != STATUS_FORMAT_PORCELAIN &&
	    status_format != STATUS_FORMAT_PORCELAIN_V2)
		progress_flag = REFRESH_PROGRESS;
	repo_read_सूचकांक(the_repository);
	refresh_सूचकांक(the_repository->सूचकांक,
		      REFRESH_QUIET|REFRESH_UNMERGED|progress_flag,
		      &s.pathspec, शून्य, शून्य);

	अगर (use_optional_locks())
		fd = repo_hold_locked_सूचकांक(the_repository, &सूचकांक_lock, 0);
	अन्यथा
		fd = -1;

	s.is_initial = repo_get_oid(the_repository, s.reference, &oid) ? 1 : 0;
	अगर (!s.is_initial)
		oidcpy(&s.oid_commit, &oid);

	s.ignore_submodule_arg = ignore_submodule_arg;
	s.status_format = status_format;
	s.verbose = verbose;
	अगर (no_renames != -1)
		s.detect_rename = !no_renames;
	अगर ((intptr_t)rename_score_arg != -1) {
		अगर (s.detect_rename < DIFF_DETECT_RENAME)
			s.detect_rename = DIFF_DETECT_RENAME;
		अगर (rename_score_arg)
			s.rename_score = parse_rename_score(&rename_score_arg);
	}

	wt_status_collect(&s);

	अगर (0 <= fd)
		repo_update_सूचकांक_if_able(the_repository, &सूचकांक_lock);

	अगर (s.relative_paths)
		s.prefix = prefix;

	wt_status_print(&s);
	wt_status_collect_free_buffers(&s);

	वापस 0;
}

अटल पूर्णांक git_commit_config(स्थिर अक्षर *k, स्थिर अक्षर *v,
			     स्थिर काष्ठा config_context *ctx, व्योम *cb)
{
	काष्ठा wt_status *s = cb;

	अगर (!म_भेद(k, "commit.template"))
		वापस git_config_pathname(&template_file, k, v);
	अगर (!म_भेद(k, "commit.status")) {
		include_status = git_config_bool(k, v);
		वापस 0;
	}
	अगर (!म_भेद(k, "commit.cleanup")) {
		FREE_AND_NULL(cleanup_config);
		वापस git_config_string(&cleanup_config, k, v);
	}
	अगर (!म_भेद(k, "commit.gpgsign")) {
		sign_commit = git_config_bool(k, v) ? "" : शून्य;
		वापस 0;
	}
	अगर (!म_भेद(k, "commit.verbose")) {
		पूर्णांक is_bool;
		config_commit_verbose = git_config_bool_or_int(k, v, ctx->kvi,
							       &is_bool);
		वापस 0;
	}

	वापस git_status_config(k, v, ctx, s);
}

पूर्णांक cmd_commit(पूर्णांक argc,
	       स्थिर अक्षर **argv,
	       स्थिर अक्षर *prefix,
	       काष्ठा repository *repo UNUSED)
{
	अटल काष्ठा wt_status s;
	अटल स्थिर अक्षर *cleanup_arg = शून्य;
	अटल काष्ठा option builtin_commit_options[] = {
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

	काष्ठा strbuf sb = STRBUF_INIT;
	काष्ठा strbuf author_ident = STRBUF_INIT;
	स्थिर अक्षर *सूचकांक_file, *reflog_संदेश;
	काष्ठा object_id oid;
	काष्ठा commit_list *parents = शून्य;
	काष्ठा stat statbuf;
	काष्ठा commit *current_शीर्ष = शून्य;
	काष्ठा commit_extra_header *extra = शून्य;
	काष्ठा strbuf err = STRBUF_INIT;
	पूर्णांक ret = 0;

	show_usage_with_options_if_asked(argc, argv,
					 builtin_commit_usage, builtin_commit_options);

#अगर_अघोषित WITH_BREAKING_CHANGES
	warn_on_auto_comment_char = true;
#पूर्ण_अगर /* !WITH_BREAKING_CHANGES */
	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_सूचकांक = 0;

	status_init_config(&s, git_commit_config);
	s.commit_template = 1;
	status_format = STATUS_FORMAT_NONE; /* Ignore status.short */
	s.colopts = 0;

	अगर (repo_get_oid(the_repository, "HEAD", &oid))
		current_शीर्ष = शून्य;
	अन्यथा {
		current_शीर्ष = lookup_commit_or_die(&oid, "HEAD");
		अगर (repo_parse_commit(the_repository, current_शीर्ष))
			die(_("could not parse HEAD commit"));
	}
	verbose = -1; /* unspecified */
	argc = parse_and_validate_options(argc, argv, builtin_commit_options,
					  builtin_commit_usage,
					  prefix, current_शीर्ष, &s);
	अगर (trailer_args.nr)
		trailer_config_init();

	अगर (verbose == -1)
		verbose = (config_commit_verbose < 0) ? 0 : config_commit_verbose;

	अगर (cleanup_arg) {
		मुक्त(cleanup_config);
		cleanup_config = xstrdup(cleanup_arg);
	}
	cleanup_mode = get_cleanup_mode(cleanup_config, use_editor);

	अगर (dry_run)
		वापस dry_run_commit(argv, prefix, current_शीर्ष, &s);
	सूचकांक_file = prepare_सूचकांक(argv, prefix, current_शीर्ष, 0);

	/* Set up everything for writing the commit object.  This includes
	   running hooks, writing the trees, and interacting with the user.  */
	अगर (!prepare_to_commit(सूचकांक_file, prefix,
			       current_शीर्ष, &s, &author_ident)) {
		ret = 1;
		rollback_सूचकांक_files();
		जाओ cleanup;
	}

	/* Determine parents */
	reflog_संदेश = दो_पर्या("GIT_REFLOG_ACTION");
	अगर (!current_शीर्ष) {
		अगर (!reflog_संदेश)
			reflog_संदेश = "commit (initial)";
	} अन्यथा अगर (amend) {
		अगर (!reflog_संदेश)
			reflog_संदेश = "commit (amend)";
		parents = commit_list_copy(current_शीर्ष->parents);
	} अन्यथा अगर (whence == FROM_MERGE) {
		काष्ठा strbuf m = STRBUF_INIT;
		खाता *fp;
		पूर्णांक allow_fast_forward = 1;
		काष्ठा commit_list **pptr = &parents;

		अगर (!reflog_संदेश)
			reflog_संदेश = "commit (merge)";
		pptr = commit_list_append(current_शीर्ष, pptr);
		fp = xfopen(git_path_merge_शीर्ष(the_repository), "r");
		जबतक (strbuf_getline_lf(&m, fp) != खातापूर्ण) {
			काष्ठा commit *parent;

			parent = get_merge_parent(m.buf);
			अगर (!parent)
				die(_("Corrupt MERGE_HEAD file (%s)"), m.buf);
			pptr = commit_list_append(parent, pptr);
		}
		ख_बंद(fp);
		strbuf_release(&m);
		अगर (!stat(git_path_merge_mode(the_repository), &statbuf)) {
			अगर (strbuf_read_file(&sb, git_path_merge_mode(the_repository), 0) < 0)
				die_errno(_("could not read MERGE_MODE"));
			अगर (!म_भेद(sb.buf, "no-ff"))
				allow_fast_forward = 0;
		}
		अगर (allow_fast_forward)
			reduce_heads_replace(&parents);
	} अन्यथा {
		अगर (!reflog_संदेश)
			reflog_संदेश = is_from_cherry_pick(whence)
					? "commit (cherry-pick)"
					: is_from_rebase(whence)
					? "commit (rebase)"
					: "commit";
		commit_list_जोड़ो(current_शीर्ष, &parents);
	}

	/* Finally, get the commit message */
	strbuf_reset(&sb);
	अगर (strbuf_read_file(&sb, git_path_commit_editmsg(), 0) < 0) {
		पूर्णांक saved_errno = त्रुटि_सं;
		rollback_सूचकांक_files();
		die(_("could not read commit message: %s"), म_त्रुटि(saved_errno));
	}

	cleanup_message(&sb, cleanup_mode, verbose);

	अगर (message_is_empty(&sb, cleanup_mode) && !allow_empty_message) {
		rollback_सूचकांक_files();
		ख_लिखो(मानक_त्रुटि, _("Aborting commit due to empty commit message.\n"));
		निकास(1);
	}
	अगर (template_untouched(&sb, template_file, cleanup_mode) && !allow_empty_message) {
		rollback_सूचकांक_files();
		ख_लिखो(मानक_त्रुटि, _("Aborting commit; you did not edit the message.\n"));
		निकास(1);
	}

	अगर (fixup_message && starts_with(sb.buf, "amend! ") &&
	    !allow_empty_message) {
		काष्ठा strbuf body = STRBUF_INIT;
		माप_प्रकार len = commit_subject_लंबाई(sb.buf);
		strbuf_addstr(&body, sb.buf + len);
		अगर (message_is_empty(&body, cleanup_mode)) {
			rollback_सूचकांक_files();
			ख_लिखो(मानक_त्रुटि, _("Aborting commit due to empty commit message body.\n"));
			निकास(1);
		}
		strbuf_release(&body);
	}

	अगर (amend) {
		स्थिर अक्षर *exclude_gpgsig[3] = { "gpgsig", "gpgsig-sha256", शून्य };
		extra = read_commit_extra_headers(current_शीर्ष, exclude_gpgsig);
	} अन्यथा {
		काष्ठा commit_extra_header **पुच्छ = &extra;
		append_merge_tag_headers(parents, &पुच्छ);
	}

	अगर (commit_वृक्ष_extended(sb.buf, sb.len, &the_repository->सूचकांक->cache_वृक्ष->oid,
				 parents, &oid, author_ident.buf, शून्य,
				 sign_commit, extra)) {
		rollback_सूचकांक_files();
		die(_("failed to write commit object"));
	}

	अगर (update_शीर्ष_with_reflog(current_शीर्ष, &oid, reflog_संदेश, &sb,
				    &err)) {
		rollback_सूचकांक_files();
		die("%s", err.buf);
	}

	sequencer_post_commit_cleanup(the_repository, 0);
	unlink(git_path_merge_शीर्ष(the_repository));
	unlink(git_path_merge_संदेश(the_repository));
	unlink(git_path_merge_mode(the_repository));
	unlink(git_path_squash_संदेश(the_repository));

	अगर (commit_सूचकांक_files())
		die(_("repository has been updated, but unable to write\n"
		      "new index file. Check that disk is not full and quota is\n"
		      "not exceeded, and then \"git restore --staged :/\" to recover."));

	git_test_write_commit_graph_or_die(the_repository);

	repo_rerere(the_repository, 0);
	run_auto_maintenance(the_repository, quiet);
	run_commit_hook(use_editor, repo_get_सूचकांक_file(the_repository),
			शून्य, "post-commit", शून्य);
	अगर (amend && !no_post_rewrite) {
		commit_post_rewrite(the_repository, current_शीर्ष, &oid);
	}
	अगर (!quiet) {
		अचिन्हित पूर्णांक flags = 0;

		अगर (!current_शीर्ष)
			flags |= SUMMARY_INITIAL_COMMIT;
		अगर (author_date_is_interesting())
			flags |= SUMMARY_SHOW_AUTHOR_DATE;
		print_commit_summary(the_repository, prefix,
				     &oid, flags);
	}

	apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
			    शून्य, शून्य, शून्य, शून्य);

cleanup:
	free_commit_extra_headers(extra);
	commit_list_free(parents);
	strbuf_release(&author_ident);
	strbuf_release(&err);
	strbuf_release(&sb);
	मुक्त(logfile);
	मुक्त(template_file);
	वापस ret;
}
