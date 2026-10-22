/*
 * "git add" builtin command
 *
 * Copyright (C) 2006 Linus Torvalds
 */

#समावेश "builtin.h"
#समावेश "advice.h"
#समावेश "config.h"
#समावेश "environment.h"
#समावेश "lockfile.h"
#समावेश "editor.h"
#समावेश "dir.h"
#समावेश "gettext.h"
#समावेश "pathspec.h"
#समावेश "object-file.h"
#समावेश "odb.h"
#समावेश "odb/transaction.h"
#समावेश "parse-options.h"
#समावेश "path.h"
#समावेश "preload-index.h"
#समावेश "diff.h"
#समावेश "read-cache.h"
#समावेश "revision.h"
#समावेश "submodule.h"
#समावेश "add-interactive.h"
#समावेश "merge-ll.h"
#समावेश "apply.h"

अटल स्थिर अक्षर * स्थिर builtin_add_usage[] = {
	N_("git add [<options>] [--] <pathspec>..."),
	शून्य
};
अटल पूर्णांक patch_interactive, add_interactive, edit_interactive;
अटल काष्ठा interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
अटल पूर्णांक take_worktree_changes;
अटल पूर्णांक add_renormalize;
अटल पूर्णांक add_resolved;
अटल पूर्णांक pathspec_file_nul;
अटल पूर्णांक include_sparse;
अटल स्थिर अक्षर *pathspec_from_file;

अटल पूर्णांक chmod_pathspec(काष्ठा repository *repo,
			  काष्ठा pathspec *pathspec,
			  अक्षर flip,
			  पूर्णांक show_only)
{
	पूर्णांक ret = 0;

	क्रम (माप_प्रकार i = 0; i < repo->सूचकांक->cache_nr; i++) {
		काष्ठा cache_entry *ce = repo->सूचकांक->cache[i];
		पूर्णांक err;

		अगर (!include_sparse &&
		    (ce_skip_worktree(ce) ||
		     !path_in_sparse_checkout(ce->name, repo->सूचकांक)))
			जारी;

		अगर (pathspec && !ce_path_match(repo->सूचकांक, ce, pathspec, शून्य))
			जारी;

		अगर (!show_only)
			err = chmod_सूचकांक_entry(repo->सूचकांक, ce, flip);
		अन्यथा
			err = S_ISREG(ce->ce_mode) ? 0 : -1;

		अगर (err < 0)
			ret = error(_("cannot chmod %cx '%s'"), flip, ce->name);
	}

	वापस ret;
}

अटल पूर्णांक renormalize_tracked_files(काष्ठा repository *repo,
				     स्थिर काष्ठा pathspec *pathspec,
				     पूर्णांक flags)
{
	पूर्णांक retval = 0;

	क्रम (माप_प्रकार i = 0; i < repo->सूचकांक->cache_nr; i++) {
		काष्ठा cache_entry *ce = repo->सूचकांक->cache[i];

		अगर (!include_sparse &&
		    (ce_skip_worktree(ce) ||
		     !path_in_sparse_checkout(ce->name, repo->सूचकांक)))
			जारी;
		अगर (ce_stage(ce))
			जारी; /* do not touch unmerged paths */
		अगर (!S_ISREG(ce->ce_mode) && !S_ISLNK(ce->ce_mode))
			जारी; /* do not touch non blobs */
		अगर (pathspec && !ce_path_match(repo->सूचकांक, ce, pathspec, शून्य))
			जारी;
		retval |= add_file_to_सूचकांक(repo->सूचकांक, ce->name,
					    flags | ADD_CACHE_RENORMALIZE);
	}

	वापस retval;
}

अटल अक्षर *prune_directory(काष्ठा repository *repo,
			     काष्ठा dir_struct *dir,
			     काष्ठा pathspec *pathspec,
			     पूर्णांक prefix)
{
	अक्षर *seen;
	पूर्णांक i;
	काष्ठा dir_entry **src, **dst;

	seen = xcalloc(pathspec->nr, 1);

	src = dst = dir->entries;
	i = dir->nr;
	जबतक (--i >= 0) {
		काष्ठा dir_entry *entry = *src++;
		अगर (dir_path_match(repo->सूचकांक, entry, pathspec, prefix, seen))
			*dst++ = entry;
	}
	dir->nr = dst - dir->entries;
	add_pathspec_matches_against_सूचकांक(pathspec, repo->सूचकांक, seen,
					   PS_IGNORE_SKIP_WORKTREE);
	वापस seen;
}

अटल पूर्णांक refresh(काष्ठा repository *repo, पूर्णांक verbose, स्थिर काष्ठा pathspec *pathspec)
{
	अक्षर *seen;
	पूर्णांक i, ret = 0;
	अक्षर *skip_worktree_seen = शून्य;
	काष्ठा string_list only_match_skip_worktree = STRING_LIST_INIT_NODUP;
	अचिन्हित पूर्णांक flags = REFRESH_IGNORE_SKIP_WORKTREE |
		    (verbose ? REFRESH_IN_PORCELAIN : REFRESH_QUIET);

	seen = xcalloc(pathspec->nr, 1);
	refresh_सूचकांक(repo->सूचकांक, flags, pathspec, seen,
		      _("Unstaged changes after refreshing the index:"));
	क्रम (i = 0; i < pathspec->nr; i++) {
		अगर (!seen[i]) {
			स्थिर अक्षर *path = pathspec->वस्तुएँ[i].original;

			अगर (matches_skip_worktree(pathspec, i, &skip_worktree_seen) ||
			    !path_in_sparse_checkout(path, repo->सूचकांक)) {
				string_list_append(&only_match_skip_worktree,
						   pathspec->वस्तुएँ[i].original);
			} अन्यथा {
				die(_("pathspec '%s' did not match any files"),
				    pathspec->वस्तुएँ[i].original);
			}
		}
	}

	अगर (only_match_skip_worktree.nr) {
		advise_on_updating_sparse_paths(&only_match_skip_worktree);
		ret = 1;
	}

	मुक्त(seen);
	मुक्त(skip_worktree_seen);
	string_list_clear(&only_match_skip_worktree, 0);
	वापस ret;
}

पूर्णांक interactive_add(काष्ठा repository *repo,
		    स्थिर अक्षर **argv,
		    स्थिर अक्षर *prefix,
		    पूर्णांक patch, काष्ठा interactive_options *interactive_opts)
{
	काष्ठा pathspec pathspec;
	पूर्णांक ret;

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_FULL |
		       PATHSPEC_SYMLINK_LEADING_PATH |
		       PATHSPEC_PREFIX_ORIGIN,
		       prefix, argv);

	अगर (patch)
		ret = !!run_add_p(repo, ADD_P_ADD, interactive_opts, शून्य, &pathspec, 0);
	अन्यथा
		ret = !!run_add_i(repo, &pathspec, interactive_opts);

	clear_pathspec(&pathspec);
	वापस ret;
}

अटल पूर्णांक edit_patch(काष्ठा repository *repo,
		      पूर्णांक argc,
		      स्थिर अक्षर **argv,
		      स्थिर अक्षर *prefix)
{
	अक्षर *file = repo_git_path(repo, "ADD_EDIT.patch");
	काष्ठा apply_state state;
	स्थिर अक्षर *apply_argv[2];
	काष्ठा rev_info rev;
	पूर्णांक out;
	काष्ठा stat st;

	repo_config(repo, git_diff_basic_config, शून्य);

	अगर (repo_read_सूचकांक(repo) < 0)
		die(_("could not read the index"));

	repo_init_revisions(repo, &rev, prefix);
	rev.diffopt.context = 7;

	argc = setup_revisions(argc, argv, &rev, शून्य);
	rev.diffopt.output_format = DIFF_FORMAT_PATCH;
	rev.diffopt.use_color = GIT_COLOR_NEVER;
	rev.diffopt.flags.ignore_dirty_submodules = 1;
	out = xopen(file, O_CREAT | O_WRONLY | O_TRUNC, 0666);
	rev.diffopt.file = xfdopen(out, "w");
	rev.diffopt.close_file = 1;
	run_diff_files(&rev, 0);

	अगर (launch_editor(file, शून्य, शून्य))
		die(_("editing patch failed"));

	अगर (stat(file, &st))
		die_errno(_("could not stat '%s'"), file);
	अगर (!st.st_आकार)
		die(_("empty patch. aborted"));

	apply_argv[0] = file;
	apply_argv[1] = शून्य;
	अगर (init_apply_state(&state, repo, शून्य))
		die(_("could not initialize apply state"));
	state.cached = 1;
	अगर (check_apply_state(&state, 0))
		die(_("could not check apply state"));
	अगर (apply_all_patches(&state, 1, apply_argv, APPLY_OPT_RECOUNT))
		die(_("could not apply '%s'"), file);
	clear_apply_state(&state);

	unlink(file);
	मुक्त(file);
	release_revisions(&rev);
	वापस 0;
}

अटल स्थिर अक्षर ignore_error[] =
N_("The following paths are ignored by one of your .gitignore files:\n");

अटल पूर्णांक verbose, show_only, ignored_too, refresh_only;
अटल पूर्णांक ignore_add_errors, intent_to_add, ignore_missing;
अटल पूर्णांक warn_on_embedded_repo = 1;

#घोषणा ADDREMOVE_DEFAULT 1
अटल पूर्णांक addremove = ADDREMOVE_DEFAULT;
अटल पूर्णांक addremove_explicit = -1; /* unspecified */

अटल अक्षर *chmod_arg;

अटल पूर्णांक ignore_removal_cb(स्थिर काष्ठा option *opt, स्थिर अक्षर *arg, पूर्णांक unset)
{
	BUG_ON_OPT_ARG(arg);

	/* if we are told to ignore, we are not adding removals */
	*(पूर्णांक *)opt->मूल्य = !unset ? 0 : 1;
	वापस 0;
}

अटल काष्ठा option builtin_add_options[] = {
	OPT__DRY_RUN(&show_only, N_("dry run")),
	OPT__VERBOSE(&verbose, N_("be verbose")),
	OPT_GROUP(""),
	OPT_BOOL('i', "interactive", &add_interactive, N_("interactive picking")),
	OPT_BOOL('p', "patch", &patch_interactive, N_("select hunks interactively")),
	OPT_BOOL(0, "auto-advance", &interactive_opts.auto_advance,
		 N_("auto advance to the next file when selecting hunks interactively")),
	OPT_DIFF_UNIFIED(&interactive_opts.context),
	OPT_DIFF_INTERHUNK_CONTEXT(&interactive_opts.interhunkcontext),
	OPT_BOOL('e', "edit", &edit_interactive, N_("edit current diff and apply")),
	OPT__FORCE(&ignored_too, N_("allow adding otherwise ignored files"), 0),
	OPT_BOOL('u', "update", &take_worktree_changes, N_("update tracked files")),
	OPT_BOOL(0, "renormalize", &add_renormalize, N_("renormalize EOL of tracked files (implies -u)")),
	OPT_BOOL(0, "resolved", &add_resolved, N_("add conflict-resolved tracked files")),
	OPT_BOOL('N', "intent-to-add", &intent_to_add, N_("record only the fact that the path will be added later")),
	OPT_BOOL('A', "all", &addremove_explicit, N_("add changes from all tracked and untracked files")),
	OPT_CALLBACK_F(0, "ignore-removal", &addremove_explicit,
	  शून्य /* takes no arguments */,
	  N_("ignore paths removed in the working tree (same as --no-all)"),
	  PARSE_OPT_NOARG, ignore_removal_cb),
	OPT_BOOL( 0 , "refresh", &refresh_only, N_("don't add, only refresh the index")),
	OPT_BOOL( 0 , "ignore-errors", &ignore_add_errors, N_("just skip files which cannot be added because of errors")),
	OPT_BOOL( 0 , "ignore-missing", &ignore_missing, N_("check if - even missing - files are ignored in dry run")),
	OPT_BOOL(0, "sparse", &include_sparse, N_("allow updating entries outside of the sparse-checkout cone")),
	OPT_STRING(0, "chmod", &chmod_arg, "(+|-)x",
		   N_("override the executable bit of the listed files")),
	OPT_HIDDEN_BOOL(0, "warn-embedded-repo", &warn_on_embedded_repo,
			N_("warn when adding an embedded repository")),
	OPT_PATHSPEC_FROM_FILE(&pathspec_from_file),
	OPT_PATHSPEC_FILE_NUL(&pathspec_file_nul),
	OPT_END(),
};

अटल पूर्णांक add_config(स्थिर अक्षर *var, स्थिर अक्षर *मूल्य,
		      स्थिर काष्ठा config_context *ctx, व्योम *cb)
{
	अगर (!म_भेद(var, "add.ignoreerrors") ||
	    !म_भेद(var, "add.ignore-errors")) {
		ignore_add_errors = git_config_bool(var, मूल्य);
		वापस 0;
	}

	अगर (git_color_config(var, मूल्य, cb) < 0)
		वापस -1;

	वापस git_default_config(var, मूल्य, ctx, cb);
}

अटल स्थिर अक्षर embedded_advice[] = N_(
"You've added another git repository inside your current repository.\n"
"Clones of the outer repository will not contain the contents of\n"
"the embedded repository and will not know how to obtain it.\n"
"If you meant to add a submodule, use:\n"
"\n"
"	git submodule add <url> %s\n"
"\n"
"If you added this path by mistake, you can remove it from the\n"
"index with:\n"
"\n"
"	git rm --cached %s\n"
"\n"
"See \"git help submodule\" for more information."
);

अटल व्योम check_embedded_repo(स्थिर अक्षर *path)
{
	काष्ठा strbuf name = STRBUF_INIT;
	अटल पूर्णांक adviced_on_embedded_repo = 0;

	अगर (!warn_on_embedded_repo)
		वापस;
	अगर (!ends_with(path, "/"))
		वापस;

	/* Drop trailing slash for aesthetics */
	strbuf_addstr(&name, path);
	strbuf_strip_suffix(&name, "/");

	warning(_("adding embedded git repository: %s"), name.buf);
	अगर (!adviced_on_embedded_repo) {
		advise_if_enabled(ADVICE_ADD_EMBEDDED_REPO,
				  embedded_advice, name.buf, name.buf);
		adviced_on_embedded_repo = 1;
	}

	strbuf_release(&name);
}

अटल पूर्णांक add_files(काष्ठा repository *repo, काष्ठा dir_struct *dir, पूर्णांक flags)
{
	पूर्णांक i, exit_status = 0;
	काष्ठा string_list matched_sparse_paths = STRING_LIST_INIT_NODUP;

	अगर (dir->ignored_nr) {
		ख_लिखो(मानक_त्रुटि, _(ignore_error));
		क्रम (i = 0; i < dir->ignored_nr; i++)
			ख_लिखो(मानक_त्रुटि, "%s\n", dir->ignored[i]->name);
		advise_if_enabled(ADVICE_ADD_IGNORED_FILE,
				  _("Use -f if you really want to add them."));
		exit_status = 1;
	}

	क्रम (i = 0; i < dir->nr; i++) {
		अगर (!include_sparse &&
		    !path_in_sparse_checkout(dir->entries[i]->name, repo->सूचकांक)) {
			string_list_append(&matched_sparse_paths,
					   dir->entries[i]->name);
			जारी;
		}
		अगर (add_file_to_सूचकांक(repo->सूचकांक, dir->entries[i]->name, flags)) {
			अगर (!ignore_add_errors)
				die(_("adding files failed"));
			exit_status = 1;
		} अन्यथा {
			check_embedded_repo(dir->entries[i]->name);
		}
	}

	अगर (matched_sparse_paths.nr) {
		advise_on_updating_sparse_paths(&matched_sparse_paths);
		exit_status = 1;
	}

	string_list_clear(&matched_sparse_paths, 0);

	वापस exit_status;
}

अटल पूर्णांक failed_to_add(पूर्णांक flags, स्थिर अक्षर *path)
{
	अगर (!(flags & ADD_CACHE_IGNORE_ERRORS))
		die(_("updating file '%s' failed"), path);
	वापस 1;
}

अटल पूर्णांक add_resolved_files(काष्ठा repository *repo,
			      स्थिर काष्ठा pathspec *pathspec,
			      पूर्णांक flags)
{
	काष्ठा सूचकांक_state *istate = repo->सूचकांक;
	काष्ठा string_list unmerged_paths = STRING_LIST_INIT_DUP;
	काष्ठा string_list unresolved_paths = STRING_LIST_INIT_DUP;
	पूर्णांक exit_status = 0;
	माप_प्रकार i;

	क्रम (i = 0; i < istate->cache_nr; i++) {
		काष्ठा cache_entry *ce = istate->cache[i];
		अगर (!ce_stage(ce))
			जारी;
		अगर (pathspec->nr && !ce_path_match(istate, ce, pathspec, शून्य))
			जारी;
		अगर (!unmerged_paths.nr ||
		    म_भेद(unmerged_paths.वस्तुएँ[unmerged_paths.nr - 1].string, ce->name))
			string_list_append(&unmerged_paths, ce->name);
	}

	अगर (!unmerged_paths.nr) {
		string_list_clear(&unmerged_paths, 0);
		वापस 0;
	}

	क्रम (i = 0; i < unmerged_paths.nr; i++) {
		स्थिर अक्षर *path = unmerged_paths.वस्तुएँ[i].string;
		काष्ठा stat st;

		अगर (!lstat(path, &st) && S_ISREG(st.st_mode)) {
			अगर (has_conflict_markers(istate, path))
				string_list_append(&unresolved_paths, path);
		}
	}

	अगर (unresolved_paths.nr) {
		काष्ठा strbuf sb = STRBUF_INIT;
		क्रम (i = 0; i < unresolved_paths.nr; i++)
			strbuf_addf(&sb, "\t%s\n", unresolved_paths.वस्तुएँ[i].string);
		die(_("the following paths still have conflict markers:\n%s"), sb.buf);
	}

	क्रम (i = 0; i < unmerged_paths.nr; i++) {
		स्थिर अक्षर *path = unmerged_paths.वस्तुएँ[i].string;
		काष्ठा stat st;

		अगर (lstat(path, &st)) {
			अगर (त्रुटि_सं != ENOENT)
				die_errno(_("cannot lstat: '%s'"), path);
			अगर (remove_file_from_सूचकांक_with_flags(istate, path, flags))
				exit_status = failed_to_add(flags, path);
		} अन्यथा {
			अगर (add_file_to_सूचकांक(istate, path, flags))
				exit_status = failed_to_add(flags, path);
		}
	}

	string_list_clear(&unmerged_paths, 0);
	string_list_clear(&unresolved_paths, 0);
	वापस exit_status;
}

पूर्णांक cmd_add(पूर्णांक argc,
	    स्थिर अक्षर **argv,
	    स्थिर अक्षर *prefix,
	    काष्ठा repository *repo)
{
	पूर्णांक exit_status = 0;
	काष्ठा pathspec pathspec;
	काष्ठा dir_struct dir = DIR_INIT;
	पूर्णांक flags;
	पूर्णांक add_new_files;
	पूर्णांक require_pathspec;
	अक्षर *seen = शून्य;
	अक्षर *ps_matched = शून्य;
	काष्ठा lock_file lock_file = LOCK_INIT;
	काष्ठा odb_transaction *transaction = शून्य;

	repo_config(repo, add_config, शून्य);

	argc = parse_options(argc, argv, prefix, builtin_add_options,
			  builtin_add_usage, PARSE_OPT_KEEP_ARGV0);

	prepare_repo_settings(repo);
	repo->settings.command_requires_full_सूचकांक = 0;

	अगर (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	अगर (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	अगर (patch_interactive)
		add_interactive = 1;
	अगर (add_interactive) {
		अगर (show_only)
			die(_("options '%s' and '%s' cannot be used together"), "--dry-run", "--interactive/--patch");
		अगर (pathspec_from_file)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--interactive/--patch");
		निकास(interactive_add(repo, argv + 1, prefix, patch_interactive, &interactive_opts));
	} अन्यथा {
		अगर (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--interactive/--patch");
		अगर (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--interactive/--patch");
		अगर (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--interactive/--patch");
	}

	अगर (edit_interactive) {
		अगर (pathspec_from_file)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--edit");
		वापस(edit_patch(repo, argc, argv, prefix));
	}
	argc--;
	argv++;

	अगर (0 <= addremove_explicit)
		addremove = addremove_explicit;
	अन्यथा अगर (take_worktree_changes && ADDREMOVE_DEFAULT)
		addremove = 0; /* "-u" was given but not "-A" */

	die_for_incompatible_opt3(take_worktree_changes, "-u/--update",
				  0 < addremove_explicit, "-A/--all",
				  add_resolved, "--resolved");

	अगर (!show_only && ignore_missing)
		die(_("the option '%s' requires '%s'"), "--ignore-missing", "--dry-run");

	अगर (chmod_arg && ((chmod_arg[0] != '-' && chmod_arg[0] != '+') ||
			  chmod_arg[1] != 'x' || chmod_arg[2]))
		die(_("--chmod param '%s' must be either -x or +x"), chmod_arg);

	add_new_files = !take_worktree_changes && !refresh_only &&
			!add_renormalize && !add_resolved;
	require_pathspec = !(take_worktree_changes ||
			     (0 < addremove_explicit) ||
			     add_resolved);

	repo_hold_locked_सूचकांक(repo, &lock_file, LOCK_DIE_ON_ERROR);

	/*
	 * Check the "pathspec '%s' did not match any files" block
	 * below before enabling new magic.
	 */
	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_FULL |
		       PATHSPEC_SYMLINK_LEADING_PATH,
		       prefix, argv);

	अगर (pathspec_from_file) {
		अगर (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_FULL |
				    PATHSPEC_SYMLINK_LEADING_PATH,
				    prefix, pathspec_from_file, pathspec_file_nul);
	} अन्यथा अगर (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	अगर (require_pathspec && pathspec.nr == 0) {
		ख_लिखो(मानक_त्रुटि, _("Nothing specified, nothing added.\n"));
		advise_if_enabled(ADVICE_ADD_EMPTY_PATHSPEC,
				  _("Maybe you wanted to say 'git add .'?"));
		वापस 0;
	}

	अगर (!take_worktree_changes && !add_resolved &&
	    addremove_explicit < 0 && pathspec.nr)
		/* Turn "git add pathspec..." to "git add -A pathspec..." */
		addremove = 1;

	flags = ((verbose ? ADD_CACHE_VERBOSE : 0) |
		 (show_only ? ADD_CACHE_PRETEND : 0) |
		 (intent_to_add ? ADD_CACHE_INTENT : 0) |
		 (ignore_add_errors ? ADD_CACHE_IGNORE_ERRORS : 0) |
		 (!(addremove || take_worktree_changes)
		  ? ADD_CACHE_IGNORE_REMOVAL : 0));

	अगर (repo_read_सूचकांक_preload(repo, &pathspec, 0) < 0)
		die(_("index file corrupt"));

	die_in_unpopulated_submodule(repo->सूचकांक, prefix);
	die_path_inside_submodule(repo->सूचकांक, &pathspec);

	अगर (add_new_files) {
		पूर्णांक baselen;

		/* Set up the default git porcelain excludes */
		अगर (!ignored_too) {
			dir.flags |= DIR_COLLECT_IGNORED;
			setup_standard_excludes(&dir);
		}

		/* This picks up the paths that are not tracked */
		baselen = fill_directory(&dir, repo->सूचकांक, &pathspec);
		अगर (pathspec.nr)
			seen = prune_directory(repo, &dir, &pathspec, baselen);
	}

	अगर (refresh_only) {
		exit_status |= refresh(repo, verbose, &pathspec);
		जाओ finish;
	}

	अगर (pathspec.nr) {
		पूर्णांक i;
		अक्षर *skip_worktree_seen = शून्य;
		काष्ठा string_list only_match_skip_worktree = STRING_LIST_INIT_NODUP;

		अगर (!seen)
			seen = find_pathspecs_matching_against_सूचकांक(&pathspec,
					repo->सूचकांक, PS_IGNORE_SKIP_WORKTREE);

		/*
		 * file_exists() assumes exact match
		 */
		GUARD_PATHSPEC(&pathspec,
			       PATHSPEC_FROMTOP |
			       PATHSPEC_LITERAL |
			       PATHSPEC_GLOB |
			       PATHSPEC_ICASE |
			       PATHSPEC_EXCLUDE |
			       PATHSPEC_ATTR);

		क्रम (i = 0; i < pathspec.nr; i++) {
			स्थिर अक्षर *path = pathspec.वस्तुएँ[i].match;

			अगर (pathspec.वस्तुएँ[i].magic & PATHSPEC_EXCLUDE)
				जारी;
			अगर (seen[i])
				जारी;

			अगर (!include_sparse &&
			    matches_skip_worktree(&pathspec, i, &skip_worktree_seen)) {
				string_list_append(&only_match_skip_worktree,
						   pathspec.वस्तुएँ[i].original);
				जारी;
			}

			/* Don't complain at 'git add .' on empty repo */
			अगर (!path[0])
				जारी;

			अगर ((pathspec.वस्तुएँ[i].magic & (PATHSPEC_GLOB | PATHSPEC_ICASE)) ||
			    !file_exists(path)) {
				अगर (ignore_missing) {
					पूर्णांक dtype = DT_UNKNOWN;
					अगर (is_excluded(&dir, repo->सूचकांक, path, &dtype))
						dir_add_ignored(&dir, repo->सूचकांक,
								path, pathspec.वस्तुएँ[i].len);
				} अन्यथा
					die(_("pathspec '%s' did not match any files"),
					    pathspec.वस्तुएँ[i].original);
			}
		}


		अगर (only_match_skip_worktree.nr) {
			advise_on_updating_sparse_paths(&only_match_skip_worktree);
			exit_status = 1;
		}

		मुक्त(seen);
		मुक्त(skip_worktree_seen);
		string_list_clear(&only_match_skip_worktree, 0);
	}

	odb_transaction_begin_or_die(repo->objects, &transaction, 0);

	ps_matched = xcalloc(pathspec.nr, 1);
	अगर (add_resolved)
		exit_status |= add_resolved_files(repo, &pathspec, flags);
	अन्यथा अगर (add_renormalize)
		exit_status |= renormalize_tracked_files(repo, &pathspec, flags);
	अन्यथा
		exit_status |= add_files_to_cache(repo, prefix,
						  &pathspec, ps_matched,
						  include_sparse, flags, ignored_too);

	अगर (take_worktree_changes && !add_renormalize && !ignore_add_errors &&
	    report_path_error(ps_matched, &pathspec))
		निकास(128);

	अगर (add_new_files)
		exit_status |= add_files(repo, &dir, flags);

	अगर (chmod_arg && pathspec.nr)
		exit_status |= chmod_pathspec(repo, &pathspec, chmod_arg[0], show_only);
	odb_transaction_commit_and_finalize_or_die(transaction);

finish:
	अगर (write_locked_सूचकांक(repo->सूचकांक, &lock_file,
			       COMMIT_LOCK | SKIP_IF_UNCHANGED))
		die(_("unable to write new index file"));

	मुक्त(ps_matched);
	dir_clear(&dir);
	clear_pathspec(&pathspec);
	वापस exit_status;
}
