/*
 * "git add" builtin command
 *
 * Copyright (C) 2006 Linus Torvalds
 */

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "advice.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "lockfile.h"
#अन्तर्भाव "editor.h"
#अन्तर्भाव "dir.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "pathspec.h"
#अन्तर्भाव "object-file.h"
#अन्तर्भाव "odb.h"
#अन्तर्भाव "odb/transaction.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "path.h"
#अन्तर्भाव "preload-index.h"
#अन्तर्भाव "diff.h"
#अन्तर्भाव "read-cache.h"
#अन्तर्भाव "revision.h"
#अन्तर्भाव "submodule.h"
#अन्तर्भाव "add-interactive.h"
#अन्तर्भाव "merge-ll.h"
#अन्तर्भाव "apply.h"

स्थिर स्थिरांक अक्षर * स्थिरांक builtin_add_usage[] = {
	N_("git add [<options>] [--] <pathspec>..."),
	रिक्त
};
स्थिर पूर्णांक patch_interactive, add_interactive, edit_interactive;
स्थिर संरचना interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
स्थिर पूर्णांक take_worktree_changes;
स्थिर पूर्णांक add_renormalize;
स्थिर पूर्णांक add_resolved;
स्थिर पूर्णांक pathspec_file_nul;
स्थिर पूर्णांक include_sparse;
स्थिर स्थिरांक अक्षर *pathspec_from_file;

स्थिर पूर्णांक chmod_pathspec(संरचना repository *repo,
			  संरचना pathspec *pathspec,
			  अक्षर flip,
			  पूर्णांक show_only)
{
	पूर्णांक ret = 0;

	साठी (माप_प्रकार i = 0; i < repo->index->cache_nr; i++) {
		संरचना cache_entry *ce = repo->index->cache[i];
		पूर्णांक err;

		जर (!include_sparse &&
		    (ce_skip_worktree(ce) ||
		     !path_in_sparse_checkout(ce->name, repo->index)))
			सुरूठेवा;

		जर (pathspec && !ce_path_match(repo->index, ce, pathspec, रिक्त))
			सुरूठेवा;

		जर (!show_only)
			err = chmod_index_entry(repo->index, ce, flip);
		अन्यथा
			err = S_ISREG(ce->ce_mode) ? 0 : -1;

		जर (err < 0)
			ret = error(_("cannot chmod %cx '%s'"), flip, ce->name);
	}

	परत ret;
}

स्थिर पूर्णांक renormalize_tracked_files(संरचना repository *repo,
				     स्थिरांक संरचना pathspec *pathspec,
				     पूर्णांक flags)
{
	पूर्णांक retval = 0;

	साठी (माप_प्रकार i = 0; i < repo->index->cache_nr; i++) {
		संरचना cache_entry *ce = repo->index->cache[i];

		जर (!include_sparse &&
		    (ce_skip_worktree(ce) ||
		     !path_in_sparse_checkout(ce->name, repo->index)))
			सुरूठेवा;
		जर (ce_stage(ce))
			सुरूठेवा; /* do not touch unmerged paths */
		जर (!S_ISREG(ce->ce_mode) && !S_ISLNK(ce->ce_mode))
			सुरूठेवा; /* do not touch non blobs */
		जर (pathspec && !ce_path_match(repo->index, ce, pathspec, रिक्त))
			सुरूठेवा;
		retval |= add_file_to_index(repo->index, ce->name,
					    flags | ADD_CACHE_RENORMALIZE);
	}

	परत retval;
}

स्थिर अक्षर *prune_directory(संरचना repository *repo,
			     संरचना dir_struct *dir,
			     संरचना pathspec *pathspec,
			     पूर्णांक prefix)
{
	अक्षर *seen;
	पूर्णांक i;
	संरचना dir_entry **src, **dst;

	seen = xcalloc(pathspec->nr, 1);

	src = dst = dir->entries;
	i = dir->nr;
	पर्यंत (--i >= 0) {
		संरचना dir_entry *entry = *src++;
		जर (dir_path_match(repo->index, entry, pathspec, prefix, seen))
			*dst++ = entry;
	}
	dir->nr = dst - dir->entries;
	add_pathspec_matches_against_index(pathspec, repo->index, seen,
					   PS_IGNORE_SKIP_WORKTREE);
	परत seen;
}

स्थिर पूर्णांक refresh(संरचना repository *repo, पूर्णांक verbose, स्थिरांक संरचना pathspec *pathspec)
{
	अक्षर *seen;
	पूर्णांक i, ret = 0;
	अक्षर *skip_worktree_seen = रिक्त;
	संरचना string_list only_match_skip_worktree = STRING_LIST_INIT_NODUP;
	अचिन्हित पूर्णांक flags = REFRESH_IGNORE_SKIP_WORKTREE |
		    (verbose ? REFRESH_IN_PORCELAIN : REFRESH_QUIET);

	seen = xcalloc(pathspec->nr, 1);
	refresh_index(repo->index, flags, pathspec, seen,
		      _("Unstaged changes after refreshing the index:"));
	साठी (i = 0; i < pathspec->nr; i++) {
		जर (!seen[i]) {
			स्थिरांक अक्षर *path = pathspec->वस्तुसंच[i].original;

			जर (matches_skip_worktree(pathspec, i, &skip_worktree_seen) ||
			    !path_in_sparse_checkout(path, repo->index)) {
				string_list_append(&only_match_skip_worktree,
						   pathspec->वस्तुसंच[i].original);
			} अन्यथा {
				die(_("pathspec '%s' did not match any files"),
				    pathspec->वस्तुसंच[i].original);
			}
		}
	}

	जर (only_match_skip_worktree.nr) {
		advise_on_updating_sparse_paths(&only_match_skip_worktree);
		ret = 1;
	}

	मुक्त(seen);
	मुक्त(skip_worktree_seen);
	string_list_clear(&only_match_skip_worktree, 0);
	परत ret;
}

पूर्णांक interactive_add(संरचना repository *repo,
		    स्थिरांक अक्षर **argv,
		    स्थिरांक अक्षर *prefix,
		    पूर्णांक patch, संरचना interactive_options *interactive_opts)
{
	संरचना pathspec pathspec;
	पूर्णांक ret;

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_FULL |
		       PATHSPEC_SYMLINK_LEADING_PATH |
		       PATHSPEC_PREFIX_ORIGIN,
		       prefix, argv);

	जर (patch)
		ret = !!run_add_p(repo, ADD_P_ADD, interactive_opts, रिक्त, &pathspec, 0);
	अन्यथा
		ret = !!run_add_i(repo, &pathspec, interactive_opts);

	clear_pathspec(&pathspec);
	परत ret;
}

स्थिर पूर्णांक edit_patch(संरचना repository *repo,
		      पूर्णांक argc,
		      स्थिरांक अक्षर **argv,
		      स्थिरांक अक्षर *prefix)
{
	अक्षर *file = repo_git_path(repo, "ADD_EDIT.patch");
	संरचना apply_state state;
	स्थिरांक अक्षर *apply_argv[2];
	संरचना rev_info rev;
	पूर्णांक out;
	संरचना stat st;

	repo_config(repo, git_diff_basic_config, रिक्त);

	जर (repo_read_index(repo) < 0)
		die(_("could not read the index"));

	repo_init_revisions(repo, &rev, prefix);
	rev.diffopt.context = 7;

	argc = setup_revisions(argc, argv, &rev, रिक्त);
	rev.diffopt.output_format = DIFF_FORMAT_PATCH;
	rev.diffopt.use_color = GIT_COLOR_NEVER;
	rev.diffopt.flags.ignore_dirty_submodules = 1;
	out = xopen(file, O_CREAT | O_WRONLY | O_TRUNC, 0666);
	rev.diffopt.file = xfdopen(out, "w");
	rev.diffopt.close_file = 1;
	run_diff_files(&rev, 0);

	जर (launch_editor(file, रिक्त, रिक्त))
		die(_("editing patch failed"));

	जर (stat(file, &st))
		die_errno(_("could not stat '%s'"), file);
	जर (!st.st_size)
		die(_("empty patch. aborted"));

	apply_argv[0] = file;
	apply_argv[1] = रिक्त;
	जर (init_apply_state(&state, repo, रिक्त))
		die(_("could not initialize apply state"));
	state.cached = 1;
	जर (check_apply_state(&state, 0))
		die(_("could not check apply state"));
	जर (apply_all_patches(&state, 1, apply_argv, APPLY_OPT_RECOUNT))
		die(_("could not apply '%s'"), file);
	clear_apply_state(&state);

	unlink(file);
	मुक्त(file);
	release_revisions(&rev);
	परत 0;
}

स्थिर स्थिरांक अक्षर ignore_error[] =
N_("The following paths are ignored by one of your .gitignore files:\n");

स्थिर पूर्णांक verbose, show_only, ignored_too, refresh_only;
स्थिर पूर्णांक ignore_add_errors, intent_to_add, ignore_missing;
स्थिर पूर्णांक warn_on_embedded_repo = 1;

#define ADDREMOVE_DEFAULT 1
स्थिर पूर्णांक addremove = ADDREMOVE_DEFAULT;
स्थिर पूर्णांक addremove_explicit = -1; /* unspecified */

स्थिर अक्षर *chmod_arg;

स्थिर पूर्णांक ignore_removal_cb(स्थिरांक संरचना option *opt, स्थिरांक अक्षर *arg, पूर्णांक unset)
{
	BUG_ON_OPT_ARG(arg);

	/* if we are told to ignore, we are not adding removals */
	*(पूर्णांक *)opt->मूल्य = !unset ? 0 : 1;
	परत 0;
}

स्थिर संरचना option builtin_add_options[] = {
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
	  रिक्त /* takes no arguments */,
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

स्थिर पूर्णांक add_config(स्थिरांक अक्षर *var, स्थिरांक अक्षर *मूल्य,
		      स्थिरांक संरचना config_context *ctx, शून्य *cb)
{
	जर (!म_भेद(var, "add.ignoreerrors") ||
	    !म_भेद(var, "add.ignore-errors")) {
		ignore_add_errors = git_config_bool(var, मूल्य);
		परत 0;
	}

	जर (git_color_config(var, मूल्य, cb) < 0)
		परत -1;

	परत git_default_config(var, मूल्य, ctx, cb);
}

स्थिर स्थिरांक अक्षर embedded_advice[] = N_(
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

स्थिर शून्य check_embedded_repo(स्थिरांक अक्षर *path)
{
	संरचना strbuf name = STRBUF_INIT;
	स्थिर पूर्णांक adviced_on_embedded_repo = 0;

	जर (!warn_on_embedded_repo)
		परत;
	जर (!ends_with(path, "/"))
		परत;

	/* Drop trailing slash for aesthetics */
	strbuf_addstr(&name, path);
	strbuf_strip_suffix(&name, "/");

	warning(_("adding embedded git repository: %s"), name.buf);
	जर (!adviced_on_embedded_repo) {
		advise_if_enabled(ADVICE_ADD_EMBEDDED_REPO,
				  embedded_advice, name.buf, name.buf);
		adviced_on_embedded_repo = 1;
	}

	strbuf_release(&name);
}

स्थिर पूर्णांक add_files(संरचना repository *repo, संरचना dir_struct *dir, पूर्णांक flags)
{
	पूर्णांक i, exit_status = 0;
	संरचना string_list matched_sparse_paths = STRING_LIST_INIT_NODUP;

	जर (dir->ignored_nr) {
		ख_लिखो(मानक_त्रुटि, _(ignore_error));
		साठी (i = 0; i < dir->ignored_nr; i++)
			ख_लिखो(मानक_त्रुटि, "%s\n", dir->ignored[i]->name);
		advise_if_enabled(ADVICE_ADD_IGNORED_FILE,
				  _("Use -f if you really want to add them."));
		exit_status = 1;
	}

	साठी (i = 0; i < dir->nr; i++) {
		जर (!include_sparse &&
		    !path_in_sparse_checkout(dir->entries[i]->name, repo->index)) {
			string_list_append(&matched_sparse_paths,
					   dir->entries[i]->name);
			सुरूठेवा;
		}
		जर (add_file_to_index(repo->index, dir->entries[i]->name, flags)) {
			जर (!ignore_add_errors)
				die(_("adding files failed"));
			exit_status = 1;
		} अन्यथा {
			check_embedded_repo(dir->entries[i]->name);
		}
	}

	जर (matched_sparse_paths.nr) {
		advise_on_updating_sparse_paths(&matched_sparse_paths);
		exit_status = 1;
	}

	string_list_clear(&matched_sparse_paths, 0);

	परत exit_status;
}

स्थिर पूर्णांक failed_to_add(पूर्णांक flags, स्थिरांक अक्षर *path)
{
	जर (!(flags & ADD_CACHE_IGNORE_ERRORS))
		die(_("updating file '%s' failed"), path);
	परत 1;
}

स्थिर पूर्णांक add_resolved_files(संरचना repository *repo,
			      स्थिरांक संरचना pathspec *pathspec,
			      पूर्णांक flags)
{
	संरचना index_state *istate = repo->index;
	संरचना string_list unmerged_paths = STRING_LIST_INIT_DUP;
	संरचना string_list unresolved_paths = STRING_LIST_INIT_DUP;
	पूर्णांक exit_status = 0;
	माप_प्रकार i;

	साठी (i = 0; i < istate->cache_nr; i++) {
		संरचना cache_entry *ce = istate->cache[i];
		जर (!ce_stage(ce))
			सुरूठेवा;
		जर (pathspec->nr && !ce_path_match(istate, ce, pathspec, रिक्त))
			सुरूठेवा;
		जर (!unmerged_paths.nr ||
		    म_भेद(unmerged_paths.वस्तुसंच[unmerged_paths.nr - 1].string, ce->name))
			string_list_append(&unmerged_paths, ce->name);
	}

	जर (!unmerged_paths.nr) {
		string_list_clear(&unmerged_paths, 0);
		परत 0;
	}

	साठी (i = 0; i < unmerged_paths.nr; i++) {
		स्थिरांक अक्षर *path = unmerged_paths.वस्तुसंच[i].string;
		संरचना stat st;

		जर (!lstat(path, &st) && S_ISREG(st.st_mode)) {
			जर (has_conflict_markers(istate, path))
				string_list_append(&unresolved_paths, path);
		}
	}

	जर (unresolved_paths.nr) {
		संरचना strbuf sb = STRBUF_INIT;
		साठी (i = 0; i < unresolved_paths.nr; i++)
			strbuf_addf(&sb, "\t%s\n", unresolved_paths.वस्तुसंच[i].string);
		die(_("the following paths still have conflict markers:\n%s"), sb.buf);
	}

	साठी (i = 0; i < unmerged_paths.nr; i++) {
		स्थिरांक अक्षर *path = unmerged_paths.वस्तुसंच[i].string;
		संरचना stat st;

		जर (lstat(path, &st)) {
			जर (त्रुटि_सं != ENOENT)
				die_errno(_("cannot lstat: '%s'"), path);
			जर (remove_file_from_index_with_flags(istate, path, flags))
				exit_status = failed_to_add(flags, path);
		} अन्यथा {
			जर (add_file_to_index(istate, path, flags))
				exit_status = failed_to_add(flags, path);
		}
	}

	string_list_clear(&unmerged_paths, 0);
	string_list_clear(&unresolved_paths, 0);
	परत exit_status;
}

पूर्णांक cmd_add(पूर्णांक argc,
	    स्थिरांक अक्षर **argv,
	    स्थिरांक अक्षर *prefix,
	    संरचना repository *repo)
{
	पूर्णांक exit_status = 0;
	संरचना pathspec pathspec;
	संरचना dir_struct dir = DIR_INIT;
	पूर्णांक flags;
	पूर्णांक add_new_files;
	पूर्णांक require_pathspec;
	अक्षर *seen = रिक्त;
	अक्षर *ps_matched = रिक्त;
	संरचना lock_file lock_file = LOCK_INIT;
	संरचना odb_transaction *transaction = रिक्त;

	repo_config(repo, add_config, रिक्त);

	argc = parse_options(argc, argv, prefix, builtin_add_options,
			  builtin_add_usage, PARSE_OPT_KEEP_ARGV0);

	prepare_repo_settings(repo);
	repo->settings.command_requires_full_index = 0;

	जर (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	जर (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	जर (patch_interactive)
		add_interactive = 1;
	जर (add_interactive) {
		जर (show_only)
			die(_("options '%s' and '%s' cannot be used together"), "--dry-run", "--interactive/--patch");
		जर (pathspec_from_file)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--interactive/--patch");
		निकास(interactive_add(repo, argv + 1, prefix, patch_interactive, &interactive_opts));
	} अन्यथा {
		जर (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--interactive/--patch");
		जर (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--interactive/--patch");
		जर (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--interactive/--patch");
	}

	जर (edit_interactive) {
		जर (pathspec_from_file)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--edit");
		परत(edit_patch(repo, argc, argv, prefix));
	}
	argc--;
	argv++;

	जर (0 <= addremove_explicit)
		addremove = addremove_explicit;
	अन्यथा जर (take_worktree_changes && ADDREMOVE_DEFAULT)
		addremove = 0; /* "-u" was given but not "-A" */

	die_for_incompatible_opt3(take_worktree_changes, "-u/--update",
				  0 < addremove_explicit, "-A/--all",
				  add_resolved, "--resolved");

	जर (!show_only && ignore_missing)
		die(_("the option '%s' requires '%s'"), "--ignore-missing", "--dry-run");

	जर (chmod_arg && ((chmod_arg[0] != '-' && chmod_arg[0] != '+') ||
			  chmod_arg[1] != 'x' || chmod_arg[2]))
		die(_("--chmod param '%s' must be either -x or +x"), chmod_arg);

	add_new_files = !take_worktree_changes && !refresh_only &&
			!add_renormalize && !add_resolved;
	require_pathspec = !(take_worktree_changes ||
			     (0 < addremove_explicit) ||
			     add_resolved);

	repo_hold_locked_index(repo, &lock_file, LOCK_DIE_ON_ERROR);

	/*
	 * Check the "pathspec '%s' did not match any files" block
	 * below before enabling new magic.
	 */
	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_FULL |
		       PATHSPEC_SYMLINK_LEADING_PATH,
		       prefix, argv);

	जर (pathspec_from_file) {
		जर (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_FULL |
				    PATHSPEC_SYMLINK_LEADING_PATH,
				    prefix, pathspec_from_file, pathspec_file_nul);
	} अन्यथा जर (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	जर (require_pathspec && pathspec.nr == 0) {
		ख_लिखो(मानक_त्रुटि, _("Nothing specified, nothing added.\n"));
		advise_if_enabled(ADVICE_ADD_EMPTY_PATHSPEC,
				  _("Maybe you wanted to say 'git add .'?"));
		परत 0;
	}

	जर (!take_worktree_changes && !add_resolved &&
	    addremove_explicit < 0 && pathspec.nr)
		/* Turn "git add pathspec..." to "git add -A pathspec..." */
		addremove = 1;

	flags = ((verbose ? ADD_CACHE_VERBOSE : 0) |
		 (show_only ? ADD_CACHE_PRETEND : 0) |
		 (intent_to_add ? ADD_CACHE_INTENT : 0) |
		 (ignore_add_errors ? ADD_CACHE_IGNORE_ERRORS : 0) |
		 (!(addremove || take_worktree_changes)
		  ? ADD_CACHE_IGNORE_REMOVAL : 0));

	जर (repo_read_index_preload(repo, &pathspec, 0) < 0)
		die(_("index file corrupt"));

	die_in_unpopulated_submodule(repo->index, prefix);
	die_path_inside_submodule(repo->index, &pathspec);

	जर (add_new_files) {
		पूर्णांक baselen;

		/* Set up the default git porcelain excludes */
		जर (!ignored_too) {
			dir.flags |= DIR_COLLECT_IGNORED;
			setup_standard_excludes(&dir);
		}

		/* This picks up the paths that are not tracked */
		baselen = fill_directory(&dir, repo->index, &pathspec);
		जर (pathspec.nr)
			seen = prune_directory(repo, &dir, &pathspec, baselen);
	}

	जर (refresh_only) {
		exit_status |= refresh(repo, verbose, &pathspec);
		जा finish;
	}

	जर (pathspec.nr) {
		पूर्णांक i;
		अक्षर *skip_worktree_seen = रिक्त;
		संरचना string_list only_match_skip_worktree = STRING_LIST_INIT_NODUP;

		जर (!seen)
			seen = find_pathspecs_matching_against_index(&pathspec,
					repo->index, PS_IGNORE_SKIP_WORKTREE);

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

		साठी (i = 0; i < pathspec.nr; i++) {
			स्थिरांक अक्षर *path = pathspec.वस्तुसंच[i].match;

			जर (pathspec.वस्तुसंच[i].magic & PATHSPEC_EXCLUDE)
				सुरूठेवा;
			जर (seen[i])
				सुरूठेवा;

			जर (!include_sparse &&
			    matches_skip_worktree(&pathspec, i, &skip_worktree_seen)) {
				string_list_append(&only_match_skip_worktree,
						   pathspec.वस्तुसंच[i].original);
				सुरूठेवा;
			}

			/* Don't complain at 'git add .' on empty repo */
			जर (!path[0])
				सुरूठेवा;

			जर ((pathspec.वस्तुसंच[i].magic & (PATHSPEC_GLOB | PATHSPEC_ICASE)) ||
			    !file_exists(path)) {
				जर (ignore_missing) {
					पूर्णांक dtype = DT_UNKNOWN;
					जर (is_excluded(&dir, repo->index, path, &dtype))
						dir_add_ignored(&dir, repo->index,
								path, pathspec.वस्तुसंच[i].len);
				} अन्यथा
					die(_("pathspec '%s' did not match any files"),
					    pathspec.वस्तुसंच[i].original);
			}
		}


		जर (only_match_skip_worktree.nr) {
			advise_on_updating_sparse_paths(&only_match_skip_worktree);
			exit_status = 1;
		}

		मुक्त(seen);
		मुक्त(skip_worktree_seen);
		string_list_clear(&only_match_skip_worktree, 0);
	}

	odb_transaction_begin_or_die(repo->objects, &transaction, 0);

	ps_matched = xcalloc(pathspec.nr, 1);
	जर (add_resolved)
		exit_status |= add_resolved_files(repo, &pathspec, flags);
	अन्यथा जर (add_renormalize)
		exit_status |= renormalize_tracked_files(repo, &pathspec, flags);
	अन्यथा
		exit_status |= add_files_to_cache(repo, prefix,
						  &pathspec, ps_matched,
						  include_sparse, flags, ignored_too);

	जर (take_worktree_changes && !add_renormalize && !ignore_add_errors &&
	    report_path_error(ps_matched, &pathspec))
		निकास(128);

	जर (add_new_files)
		exit_status |= add_files(repo, &dir, flags);

	जर (chmod_arg && pathspec.nr)
		exit_status |= chmod_pathspec(repo, &pathspec, chmod_arg[0], show_only);
	odb_transaction_commit_and_finalize_or_die(transaction);

finish:
	जर (write_locked_index(repo->index, &lock_file,
			       COMMIT_LOCK | SKIP_IF_UNCHANGED))
		die(_("unable to write new index file"));

	मुक्त(ps_matched);
	dir_clear(&dir);
	clear_pathspec(&pathspec);
	परत exit_status;
}
