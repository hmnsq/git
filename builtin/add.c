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

स्थिर ध्रुव वर्ण * ध्रुव builtin_add_usage[] = {
	N_("git add [<options>] [--] <pathspec>..."),
	रिक्त
};
स्थिर पूर्णाङ्क patch_interactive, add_interactive, edit_interactive;
स्थिर संरचना interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
स्थिर पूर्णाङ्क take_worktree_changes;
स्थिर पूर्णाङ्क add_renormalize;
स्थिर पूर्णाङ्क add_resolved;
स्थिर पूर्णाङ्क pathspec_file_nul;
स्थिर पूर्णाङ्क include_sparse;
स्थिर ध्रुव वर्ण *pathspec_from_file;

स्थिर पूर्णाङ्क chmod_pathspec(संरचना repository *repo,
			  संरचना pathspec *pathspec,
			  वर्ण flip,
			  पूर्णाङ्क show_only)
{
	पूर्णाङ्क ret = 0;

	अत्थाय (size_t i = 0; i < repo->index->cache_nr; i++) {
		संरचना cache_entry *ce = repo->index->cache[i];
		पूर्णाङ्क err;

		सचे (!include_sparse &&
		    (ce_skip_worktree(ce) ||
		     !path_in_sparse_checkout(ce->name, repo->index)))
			अनुवर्त;

		सचे (pathspec && !ce_path_match(repo->index, ce, pathspec, रिक्त))
			अनुवर्त;

		सचे (!show_only)
			err = chmod_index_entry(repo->index, ce, flip);
		अञ्ञथा
			err = S_ISREG(ce->ce_mode) ? 0 : -1;

		सचे (err < 0)
			ret = error(_("cannot chmod %cx '%s'"), flip, ce->name);
	}

	पटिनिवत्त ret;
}

स्थिर पूर्णाङ्क renormalize_tracked_files(संरचना repository *repo,
				     ध्रुव संरचना pathspec *pathspec,
				     पूर्णाङ्क flags)
{
	पूर्णाङ्क retval = 0;

	अत्थाय (size_t i = 0; i < repo->index->cache_nr; i++) {
		संरचना cache_entry *ce = repo->index->cache[i];

		सचे (!include_sparse &&
		    (ce_skip_worktree(ce) ||
		     !path_in_sparse_checkout(ce->name, repo->index)))
			अनुवर्त;
		सचे (ce_stage(ce))
			अनुवर्त; /* do not touch unmerged paths */
		सचे (!S_ISREG(ce->ce_mode) && !S_ISLNK(ce->ce_mode))
			अनुवर्त; /* do not touch non blobs */
		सचे (pathspec && !ce_path_match(repo->index, ce, pathspec, रिक्त))
			अनुवर्त;
		retval |= add_file_to_index(repo->index, ce->name,
					    flags | ADD_CACHE_RENORMALIZE);
	}

	पटिनिवत्त retval;
}

स्थिर वर्ण *prune_directory(संरचना repository *repo,
			     संरचना dir_struct *dir,
			     संरचना pathspec *pathspec,
			     पूर्णाङ्क prefix)
{
	वर्ण *seen;
	पूर्णाङ्क i;
	संरचना dir_entry **src, **dst;

	seen = xcalloc(pathspec->nr, 1);

	src = dst = dir->entries;
	i = dir->nr;
	याव (--i >= 0) {
		संरचना dir_entry *entry = *src++;
		सचे (dir_path_match(repo->index, entry, pathspec, prefix, seen))
			*dst++ = entry;
	}
	dir->nr = dst - dir->entries;
	add_pathspec_matches_against_index(pathspec, repo->index, seen,
					   PS_IGNORE_SKIP_WORKTREE);
	पटिनिवत्त seen;
}

स्थिर पूर्णाङ्क refresh(संरचना repository *repo, पूर्णाङ्क verbose, ध्रुव संरचना pathspec *pathspec)
{
	वर्ण *seen;
	पूर्णाङ्क i, ret = 0;
	वर्ण *skip_worktree_seen = रिक्त;
	संरचना string_list only_match_skip_worktree = STRING_LIST_INIT_NODUP;
	unsigned पूर्णाङ्क flags = REFRESH_IGNORE_SKIP_WORKTREE |
		    (verbose ? REFRESH_IN_PORCELAIN : REFRESH_QUIET);

	seen = xcalloc(pathspec->nr, 1);
	refresh_index(repo->index, flags, pathspec, seen,
		      _("Unstaged changes after refreshing the index:"));
	अत्थाय (i = 0; i < pathspec->nr; i++) {
		सचे (!seen[i]) {
			ध्रुव वर्ण *path = pathspec->वस्तूनि[i].original;

			सचे (matches_skip_worktree(pathspec, i, &skip_worktree_seen) ||
			    !path_in_sparse_checkout(path, repo->index)) {
				string_list_append(&only_match_skip_worktree,
						   pathspec->वस्तूनि[i].original);
			} अञ्ञथा {
				die(_("pathspec '%s' did not match any files"),
				    pathspec->वस्तूनि[i].original);
			}
		}
	}

	सचे (only_match_skip_worktree.nr) {
		advise_on_updating_sparse_paths(&only_match_skip_worktree);
		ret = 1;
	}

	मुक्त(seen);
	मुक्त(skip_worktree_seen);
	string_list_clear(&only_match_skip_worktree, 0);
	पटिनिवत्त ret;
}

पूर्णाङ्क interactive_add(संरचना repository *repo,
		    ध्रुव वर्ण **argv,
		    ध्रुव वर्ण *prefix,
		    पूर्णाङ्क patch, संरचना interactive_options *interactive_opts)
{
	संरचना pathspec pathspec;
	पूर्णाङ्क ret;

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_FULL |
		       PATHSPEC_SYMLINK_LEADING_PATH |
		       PATHSPEC_PREFIX_ORIGIN,
		       prefix, argv);

	सचे (patch)
		ret = !!run_add_p(repo, ADD_P_ADD, interactive_opts, रिक्त, &pathspec, 0);
	अञ्ञथा
		ret = !!run_add_i(repo, &pathspec, interactive_opts);

	clear_pathspec(&pathspec);
	पटिनिवत्त ret;
}

स्थिर पूर्णाङ्क edit_patch(संरचना repository *repo,
		      पूर्णाङ्क argc,
		      ध्रुव वर्ण **argv,
		      ध्रुव वर्ण *prefix)
{
	वर्ण *file = repo_git_path(repo, "ADD_EDIT.patch");
	संरचना apply_state state;
	ध्रुव वर्ण *apply_argv[2];
	संरचना rev_info rev;
	पूर्णाङ्क out;
	संरचना stat st;

	repo_config(repo, git_diff_basic_config, रिक्त);

	सचे (repo_read_index(repo) < 0)
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

	सचे (launch_editor(file, रिक्त, रिक्त))
		die(_("editing patch failed"));

	सचे (stat(file, &st))
		die_errno(_("could not stat '%s'"), file);
	सचे (!st.st_size)
		die(_("empty patch. aborted"));

	apply_argv[0] = file;
	apply_argv[1] = रिक्त;
	सचे (init_apply_state(&state, repo, रिक्त))
		die(_("could not initialize apply state"));
	state.cached = 1;
	सचे (check_apply_state(&state, 0))
		die(_("could not check apply state"));
	सचे (apply_all_patches(&state, 1, apply_argv, APPLY_OPT_RECOUNT))
		die(_("could not apply '%s'"), file);
	clear_apply_state(&state);

	unlink(file);
	मुक्त(file);
	release_revisions(&rev);
	पटिनिवत्त 0;
}

स्थिर ध्रुव वर्ण ignore_error[] =
N_("The following paths are ignored by one of your .gitignore files:\n");

स्थिर पूर्णाङ्क verbose, show_only, ignored_too, refresh_only;
स्थिर पूर्णाङ्क ignore_add_errors, intent_to_add, ignore_missing;
स्थिर पूर्णाङ्क warn_on_embedded_repo = 1;

#define ADDREMOVE_DEFAULT 1
स्थिर पूर्णाङ्क addremove = ADDREMOVE_DEFAULT;
स्थिर पूर्णाङ्क addremove_explicit = -1; /* unspecified */

स्थिर वर्ण *chmod_arg;

स्थिर पूर्णाङ्क ignore_removal_cb(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	BUG_ON_OPT_ARG(arg);

	/* if we are told to ignore, we are not adding removals */
	*(पूर्णाङ्क *)opt->मानम् = !unset ? 0 : 1;
	पटिनिवत्त 0;
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

स्थिर पूर्णाङ्क add_config(ध्रुव वर्ण *var, ध्रुव वर्ण *मानम्,
		      ध्रुव संरचना config_context *ctx, शून्य *cb)
{
	सचे (!strcmp(var, "add.ignoreerrors") ||
	    !strcmp(var, "add.ignore-errors")) {
		ignore_add_errors = git_config_bool(var, मानम्);
		पटिनिवत्त 0;
	}

	सचे (git_color_config(var, मानम्, cb) < 0)
		पटिनिवत्त -1;

	पटिनिवत्त git_default_config(var, मानम्, ctx, cb);
}

स्थिर ध्रुव वर्ण embedded_advice[] = N_(
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

स्थिर शून्य check_embedded_repo(ध्रुव वर्ण *path)
{
	संरचना strbuf name = STRBUF_INIT;
	स्थिर पूर्णाङ्क adviced_on_embedded_repo = 0;

	सचे (!warn_on_embedded_repo)
		पटिनिवत्त;
	सचे (!ends_with(path, "/"))
		पटिनिवत्त;

	/* Drop trailing slash for aesthetics */
	strbuf_addstr(&name, path);
	strbuf_strip_suffix(&name, "/");

	warning(_("adding embedded git repository: %s"), name.buf);
	सचे (!adviced_on_embedded_repo) {
		advise_if_enabled(ADVICE_ADD_EMBEDDED_REPO,
				  embedded_advice, name.buf, name.buf);
		adviced_on_embedded_repo = 1;
	}

	strbuf_release(&name);
}

स्थिर पूर्णाङ्क add_files(संरचना repository *repo, संरचना dir_struct *dir, पूर्णाङ्क flags)
{
	पूर्णाङ्क i, exit_status = 0;
	संरचना string_list matched_sparse_paths = STRING_LIST_INIT_NODUP;

	सचे (dir->ignored_nr) {
		fprintf(stderr, _(ignore_error));
		अत्थाय (i = 0; i < dir->ignored_nr; i++)
			fprintf(stderr, "%s\n", dir->ignored[i]->name);
		advise_if_enabled(ADVICE_ADD_IGNORED_FILE,
				  _("Use -f if you really want to add them."));
		exit_status = 1;
	}

	अत्थाय (i = 0; i < dir->nr; i++) {
		सचे (!include_sparse &&
		    !path_in_sparse_checkout(dir->entries[i]->name, repo->index)) {
			string_list_append(&matched_sparse_paths,
					   dir->entries[i]->name);
			अनुवर्त;
		}
		सचे (add_file_to_index(repo->index, dir->entries[i]->name, flags)) {
			सचे (!ignore_add_errors)
				die(_("adding files failed"));
			exit_status = 1;
		} अञ्ञथा {
			check_embedded_repo(dir->entries[i]->name);
		}
	}

	सचे (matched_sparse_paths.nr) {
		advise_on_updating_sparse_paths(&matched_sparse_paths);
		exit_status = 1;
	}

	string_list_clear(&matched_sparse_paths, 0);

	पटिनिवत्त exit_status;
}

स्थिर पूर्णाङ्क failed_to_add(पूर्णाङ्क flags, ध्रुव वर्ण *path)
{
	सचे (!(flags & ADD_CACHE_IGNORE_ERRORS))
		die(_("updating file '%s' failed"), path);
	पटिनिवत्त 1;
}

स्थिर पूर्णाङ्क add_resolved_files(संरचना repository *repo,
			      ध्रुव संरचना pathspec *pathspec,
			      पूर्णाङ्क flags)
{
	संरचना index_state *istate = repo->index;
	संरचना string_list unmerged_paths = STRING_LIST_INIT_DUP;
	संरचना string_list unresolved_paths = STRING_LIST_INIT_DUP;
	पूर्णाङ्क exit_status = 0;
	size_t i;

	अत्थाय (i = 0; i < istate->cache_nr; i++) {
		संरचना cache_entry *ce = istate->cache[i];
		सचे (!ce_stage(ce))
			अनुवर्त;
		सचे (pathspec->nr && !ce_path_match(istate, ce, pathspec, रिक्त))
			अनुवर्त;
		सचे (!unmerged_paths.nr ||
		    strcmp(unmerged_paths.वस्तूनि[unmerged_paths.nr - 1].string, ce->name))
			string_list_append(&unmerged_paths, ce->name);
	}

	सचे (!unmerged_paths.nr) {
		string_list_clear(&unmerged_paths, 0);
		पटिनिवत्त 0;
	}

	अत्थाय (i = 0; i < unmerged_paths.nr; i++) {
		ध्रुव वर्ण *path = unmerged_paths.वस्तूनि[i].string;
		संरचना stat st;

		सचे (!lstat(path, &st) && S_ISREG(st.st_mode)) {
			सचे (has_conflict_markers(istate, path))
				string_list_append(&unresolved_paths, path);
		}
	}

	सचे (unresolved_paths.nr) {
		संरचना strbuf sb = STRBUF_INIT;
		अत्थाय (i = 0; i < unresolved_paths.nr; i++)
			strbuf_addf(&sb, "\t%s\n", unresolved_paths.वस्तूनि[i].string);
		die(_("the following paths still have conflict markers:\n%s"), sb.buf);
	}

	अत्थाय (i = 0; i < unmerged_paths.nr; i++) {
		ध्रुव वर्ण *path = unmerged_paths.वस्तूनि[i].string;
		संरचना stat st;

		सचे (lstat(path, &st)) {
			सचे (errno != ENOENT)
				die_errno(_("cannot lstat: '%s'"), path);
			सचे (remove_file_from_index_with_flags(istate, path, flags))
				exit_status = failed_to_add(flags, path);
		} अञ्ञथा {
			सचे (add_file_to_index(istate, path, flags))
				exit_status = failed_to_add(flags, path);
		}
	}

	string_list_clear(&unmerged_paths, 0);
	string_list_clear(&unresolved_paths, 0);
	पटिनिवत्त exit_status;
}

पूर्णाङ्क cmd_add(पूर्णाङ्क argc,
	    ध्रुव वर्ण **argv,
	    ध्रुव वर्ण *prefix,
	    संरचना repository *repo)
{
	पूर्णाङ्क exit_status = 0;
	संरचना pathspec pathspec;
	संरचना dir_struct dir = DIR_INIT;
	पूर्णाङ्क flags;
	पूर्णाङ्क add_new_files;
	पूर्णाङ्क require_pathspec;
	वर्ण *seen = रिक्त;
	वर्ण *ps_matched = रिक्त;
	संरचना lock_file lock_file = LOCK_INIT;
	संरचना odb_transaction *transaction = रिक्त;

	repo_config(repo, add_config, रिक्त);

	argc = parse_options(argc, argv, prefix, builtin_add_options,
			  builtin_add_usage, PARSE_OPT_KEEP_ARGV0);

	prepare_repo_settings(repo);
	repo->settings.command_requires_full_index = 0;

	सचे (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	सचे (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	सचे (patch_interactive)
		add_interactive = 1;
	सचे (add_interactive) {
		सचे (show_only)
			die(_("options '%s' and '%s' cannot be used together"), "--dry-run", "--interactive/--patch");
		सचे (pathspec_from_file)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--interactive/--patch");
		exit(interactive_add(repo, argv + 1, prefix, patch_interactive, &interactive_opts));
	} अञ्ञथा {
		सचे (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--interactive/--patch");
		सचे (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--interactive/--patch");
		सचे (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--interactive/--patch");
	}

	सचे (edit_interactive) {
		सचे (pathspec_from_file)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--edit");
		पटिनिवत्त(edit_patch(repo, argc, argv, prefix));
	}
	argc--;
	argv++;

	सचे (0 <= addremove_explicit)
		addremove = addremove_explicit;
	अञ्ञथा सचे (take_worktree_changes && ADDREMOVE_DEFAULT)
		addremove = 0; /* "-u" was given but not "-A" */

	die_for_incompatible_opt3(take_worktree_changes, "-u/--update",
				  0 < addremove_explicit, "-A/--all",
				  add_resolved, "--resolved");

	सचे (!show_only && ignore_missing)
		die(_("the option '%s' requires '%s'"), "--ignore-missing", "--dry-run");

	सचे (chmod_arg && ((chmod_arg[0] != '-' && chmod_arg[0] != '+') ||
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

	सचे (pathspec_from_file) {
		सचे (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_FULL |
				    PATHSPEC_SYMLINK_LEADING_PATH,
				    prefix, pathspec_from_file, pathspec_file_nul);
	} अञ्ञथा सचे (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	सचे (require_pathspec && pathspec.nr == 0) {
		fprintf(stderr, _("Nothing specified, nothing added.\n"));
		advise_if_enabled(ADVICE_ADD_EMPTY_PATHSPEC,
				  _("Maybe you wanted to say 'git add .'?"));
		पटिनिवत्त 0;
	}

	सचे (!take_worktree_changes && !add_resolved &&
	    addremove_explicit < 0 && pathspec.nr)
		/* Turn "git add pathspec..." to "git add -A pathspec..." */
		addremove = 1;

	flags = ((verbose ? ADD_CACHE_VERBOSE : 0) |
		 (show_only ? ADD_CACHE_PRETEND : 0) |
		 (intent_to_add ? ADD_CACHE_INTENT : 0) |
		 (ignore_add_errors ? ADD_CACHE_IGNORE_ERRORS : 0) |
		 (!(addremove || take_worktree_changes)
		  ? ADD_CACHE_IGNORE_REMOVAL : 0));

	सचे (repo_read_index_preload(repo, &pathspec, 0) < 0)
		die(_("index file corrupt"));

	die_in_unpopulated_submodule(repo->index, prefix);
	die_path_inside_submodule(repo->index, &pathspec);

	सचे (add_new_files) {
		पूर्णाङ्क baselen;

		/* Set up the default git porcelain excludes */
		सचे (!ignored_too) {
			dir.flags |= DIR_COLLECT_IGNORED;
			setup_standard_excludes(&dir);
		}

		/* This picks up the paths that are not tracked */
		baselen = fill_directory(&dir, repo->index, &pathspec);
		सचे (pathspec.nr)
			seen = prune_directory(repo, &dir, &pathspec, baselen);
	}

	सचे (refresh_only) {
		exit_status |= refresh(repo, verbose, &pathspec);
		गच्छ finish;
	}

	सचे (pathspec.nr) {
		पूर्णाङ्क i;
		वर्ण *skip_worktree_seen = रिक्त;
		संरचना string_list only_match_skip_worktree = STRING_LIST_INIT_NODUP;

		सचे (!seen)
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

		अत्थाय (i = 0; i < pathspec.nr; i++) {
			ध्रुव वर्ण *path = pathspec.वस्तूनि[i].match;

			सचे (pathspec.वस्तूनि[i].magic & PATHSPEC_EXCLUDE)
				अनुवर्त;
			सचे (seen[i])
				अनुवर्त;

			सचे (!include_sparse &&
			    matches_skip_worktree(&pathspec, i, &skip_worktree_seen)) {
				string_list_append(&only_match_skip_worktree,
						   pathspec.वस्तूनि[i].original);
				अनुवर्त;
			}

			/* Don't complain at 'git add .' on empty repo */
			सचे (!path[0])
				अनुवर्त;

			सचे ((pathspec.वस्तूनि[i].magic & (PATHSPEC_GLOB | PATHSPEC_ICASE)) ||
			    !file_exists(path)) {
				सचे (ignore_missing) {
					पूर्णाङ्क dtype = DT_UNKNOWN;
					सचे (is_excluded(&dir, repo->index, path, &dtype))
						dir_add_ignored(&dir, repo->index,
								path, pathspec.वस्तूनि[i].len);
				} अञ्ञथा
					die(_("pathspec '%s' did not match any files"),
					    pathspec.वस्तूनि[i].original);
			}
		}


		सचे (only_match_skip_worktree.nr) {
			advise_on_updating_sparse_paths(&only_match_skip_worktree);
			exit_status = 1;
		}

		मुक्त(seen);
		मुक्त(skip_worktree_seen);
		string_list_clear(&only_match_skip_worktree, 0);
	}

	odb_transaction_begin_or_die(repo->objects, &transaction, 0);

	ps_matched = xcalloc(pathspec.nr, 1);
	सचे (add_resolved)
		exit_status |= add_resolved_files(repo, &pathspec, flags);
	अञ्ञथा सचे (add_renormalize)
		exit_status |= renormalize_tracked_files(repo, &pathspec, flags);
	अञ्ञथा
		exit_status |= add_files_to_cache(repo, prefix,
						  &pathspec, ps_matched,
						  include_sparse, flags, ignored_too);

	सचे (take_worktree_changes && !add_renormalize && !ignore_add_errors &&
	    report_path_error(ps_matched, &pathspec))
		exit(128);

	सचे (add_new_files)
		exit_status |= add_files(repo, &dir, flags);

	सचे (chmod_arg && pathspec.nr)
		exit_status |= chmod_pathspec(repo, &pathspec, chmod_arg[0], show_only);
	odb_transaction_commit_and_finalize_or_die(transaction);

finish:
	सचे (write_locked_index(repo->index, &lock_file,
			       COMMIT_LOCK | SKIP_IF_UNCHANGED))
		die(_("unable to write new index file"));

	मुक्त(ps_matched);
	dir_clear(&dir);
	clear_pathspec(&pathspec);
	पटिनिवत्त exit_status;
}
