/*
 * "git add" builtin command
 *
 * Copyright (C) 2006 Linus Torvalds
 */

#ضمن "builtin.h"
#ضمن "advice.h"
#ضمن "config.h"
#ضمن "environment.h"
#ضمن "lockfile.h"
#ضمن "editor.h"
#ضمن "dir.h"
#ضمن "gettext.h"
#ضمن "pathspec.h"
#ضمن "object-file.h"
#ضمن "odb.h"
#ضمن "odb/transaction.h"
#ضمن "parse-options.h"
#ضمن "path.h"
#ضمن "preload-index.h"
#ضمن "diff.h"
#ضمن "read-cache.h"
#ضمن "revision.h"
#ضمن "submodule.h"
#ضمن "add-interactive.h"
#ضمن "merge-ll.h"
#ضمن "apply.h"

ساكن ثابت حرف * ثابت builtin_add_usage[] = {
	N_("git add [<options>] [--] <pathspec>..."),
	عدم
};
ساكن صحيح patch_interactive, add_interactive, edit_interactive;
ساكن بنية interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
ساكن صحيح take_worktree_changes;
ساكن صحيح add_renormalize;
ساكن صحيح add_resolved;
ساكن صحيح pathspec_file_nul;
ساكن صحيح include_sparse;
ساكن ثابت حرف *pathspec_from_file;

ساكن صحيح chmod_pathspec(بنية repository *repo,
			  بنية pathspec *pathspec,
			  حرف flip,
			  صحيح show_only)
{
	صحيح ret = 0;

	لكل (size_t i = 0; i < repo->index->cache_nr; i++) {
		بنية cache_entry *ce = repo->index->cache[i];
		صحيح err;

		إذا (!include_sparse &&
		    (ce_skip_worktree(ce) ||
		     !path_in_sparse_checkout(ce->name, repo->index)))
			استمر;

		إذا (pathspec && !ce_path_match(repo->index, ce, pathspec, عدم))
			استمر;

		إذا (!show_only)
			err = chmod_index_entry(repo->index, ce, flip);
		وإلا
			err = S_ISREG(ce->ce_mode) ? 0 : -1;

		إذا (err < 0)
			ret = error(_("cannot chmod %cx '%s'"), flip, ce->name);
	}

	أرجع ret;
}

ساكن صحيح renormalize_tracked_files(بنية repository *repo,
				     ثابت بنية pathspec *pathspec,
				     صحيح flags)
{
	صحيح retval = 0;

	لكل (size_t i = 0; i < repo->index->cache_nr; i++) {
		بنية cache_entry *ce = repo->index->cache[i];

		إذا (!include_sparse &&
		    (ce_skip_worktree(ce) ||
		     !path_in_sparse_checkout(ce->name, repo->index)))
			استمر;
		إذا (ce_stage(ce))
			استمر; /* do not touch unmerged paths */
		إذا (!S_ISREG(ce->ce_mode) && !S_ISLNK(ce->ce_mode))
			استمر; /* do not touch non blobs */
		إذا (pathspec && !ce_path_match(repo->index, ce, pathspec, عدم))
			استمر;
		retval |= add_file_to_index(repo->index, ce->name,
					    flags | ADD_CACHE_RENORMALIZE);
	}

	أرجع retval;
}

ساكن حرف *prune_directory(بنية repository *repo,
			     بنية dir_struct *dir,
			     بنية pathspec *pathspec,
			     صحيح prefix)
{
	حرف *seen;
	صحيح i;
	بنية dir_entry **src, **dst;

	seen = xcalloc(pathspec->nr, 1);

	src = dst = dir->entries;
	i = dir->nr;
	طالما (--i >= 0) {
		بنية dir_entry *entry = *src++;
		إذا (dir_path_match(repo->index, entry, pathspec, prefix, seen))
			*dst++ = entry;
	}
	dir->nr = dst - dir->entries;
	add_pathspec_matches_against_index(pathspec, repo->index, seen,
					   PS_IGNORE_SKIP_WORKTREE);
	أرجع seen;
}

ساكن صحيح refresh(بنية repository *repo, صحيح verbose, ثابت بنية pathspec *pathspec)
{
	حرف *seen;
	صحيح i, ret = 0;
	حرف *skip_worktree_seen = عدم;
	بنية string_list only_match_skip_worktree = STRING_LIST_INIT_NODUP;
	unsigned صحيح flags = REFRESH_IGNORE_SKIP_WORKTREE |
		    (verbose ? REFRESH_IN_PORCELAIN : REFRESH_QUIET);

	seen = xcalloc(pathspec->nr, 1);
	refresh_index(repo->index, flags, pathspec, seen,
		      _("Unstaged changes after refreshing the index:"));
	لكل (i = 0; i < pathspec->nr; i++) {
		إذا (!seen[i]) {
			ثابت حرف *path = pathspec->ابوبو[i].original;

			إذا (matches_skip_worktree(pathspec, i, &skip_worktree_seen) ||
			    !path_in_sparse_checkout(path, repo->index)) {
				string_list_append(&only_match_skip_worktree,
						   pathspec->ابوبو[i].original);
			} وإلا {
				die(_("pathspec '%s' did not match any files"),
				    pathspec->ابوبو[i].original);
			}
		}
	}

	إذا (only_match_skip_worktree.nr) {
		advise_on_updating_sparse_paths(&only_match_skip_worktree);
		ret = 1;
	}

	حرر(seen);
	حرر(skip_worktree_seen);
	string_list_clear(&only_match_skip_worktree, 0);
	أرجع ret;
}

صحيح interactive_add(بنية repository *repo,
		    ثابت حرف **argv,
		    ثابت حرف *prefix,
		    صحيح patch, بنية interactive_options *interactive_opts)
{
	بنية pathspec pathspec;
	صحيح ret;

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_FULL |
		       PATHSPEC_SYMLINK_LEADING_PATH |
		       PATHSPEC_PREFIX_ORIGIN,
		       prefix, argv);

	إذا (patch)
		ret = !!run_add_p(repo, ADD_P_ADD, interactive_opts, عدم, &pathspec, 0);
	وإلا
		ret = !!run_add_i(repo, &pathspec, interactive_opts);

	clear_pathspec(&pathspec);
	أرجع ret;
}

ساكن صحيح edit_patch(بنية repository *repo,
		      صحيح argc,
		      ثابت حرف **argv,
		      ثابت حرف *prefix)
{
	حرف *file = repo_git_path(repo, "ADD_EDIT.patch");
	بنية apply_state state;
	ثابت حرف *apply_argv[2];
	بنية rev_info rev;
	صحيح out;
	بنية stat st;

	repo_config(repo, git_diff_basic_config, عدم);

	إذا (repo_read_index(repo) < 0)
		die(_("could not read the index"));

	repo_init_revisions(repo, &rev, prefix);
	rev.diffopt.context = 7;

	argc = setup_revisions(argc, argv, &rev, عدم);
	rev.diffopt.output_format = DIFF_FORMAT_PATCH;
	rev.diffopt.use_color = GIT_COLOR_NEVER;
	rev.diffopt.flags.ignore_dirty_submodules = 1;
	out = xopen(file, O_CREAT | O_WRONLY | O_TRUNC, 0666);
	rev.diffopt.file = xfdopen(out, "w");
	rev.diffopt.close_file = 1;
	run_diff_files(&rev, 0);

	إذا (launch_editor(file, عدم, عدم))
		die(_("editing patch failed"));

	إذا (stat(file, &st))
		die_errno(_("could not stat '%s'"), file);
	إذا (!st.st_size)
		die(_("empty patch. aborted"));

	apply_argv[0] = file;
	apply_argv[1] = عدم;
	إذا (init_apply_state(&state, repo, عدم))
		die(_("could not initialize apply state"));
	state.cached = 1;
	إذا (check_apply_state(&state, 0))
		die(_("could not check apply state"));
	إذا (apply_all_patches(&state, 1, apply_argv, APPLY_OPT_RECOUNT))
		die(_("could not apply '%s'"), file);
	clear_apply_state(&state);

	unlink(file);
	حرر(file);
	release_revisions(&rev);
	أرجع 0;
}

ساكن ثابت حرف ignore_error[] =
N_("The following paths are ignored by one of your .gitignore files:\n");

ساكن صحيح verbose, show_only, ignored_too, refresh_only;
ساكن صحيح ignore_add_errors, intent_to_add, ignore_missing;
ساكن صحيح warn_on_embedded_repo = 1;

#define ADDREMOVE_DEFAULT 1
ساكن صحيح addremove = ADDREMOVE_DEFAULT;
ساكن صحيح addremove_explicit = -1; /* unspecified */

ساكن حرف *chmod_arg;

ساكن صحيح ignore_removal_cb(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_ARG(arg);

	/* if we are told to ignore, we are not adding removals */
	*(صحيح *)opt->درج = !unset ? 0 : 1;
	أرجع 0;
}

ساكن بنية option builtin_add_options[] = {
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
	  عدم /* takes no arguments */,
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

ساكن صحيح add_config(ثابت حرف *var, ثابت حرف *درج,
		      ثابت بنية config_context *ctx, فراغ *cb)
{
	إذا (!strcmp(var, "add.ignoreerrors") ||
	    !strcmp(var, "add.ignore-errors")) {
		ignore_add_errors = git_config_bool(var, درج);
		أرجع 0;
	}

	إذا (git_color_config(var, درج, cb) < 0)
		أرجع -1;

	أرجع git_default_config(var, درج, ctx, cb);
}

ساكن ثابت حرف embedded_advice[] = N_(
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

ساكن فراغ check_embedded_repo(ثابت حرف *path)
{
	بنية strbuf name = STRBUF_INIT;
	ساكن صحيح adviced_on_embedded_repo = 0;

	إذا (!warn_on_embedded_repo)
		أرجع;
	إذا (!ends_with(path, "/"))
		أرجع;

	/* Drop trailing slash for aesthetics */
	strbuf_addstr(&name, path);
	strbuf_strip_suffix(&name, "/");

	warning(_("adding embedded git repository: %s"), name.buf);
	إذا (!adviced_on_embedded_repo) {
		advise_if_enabled(ADVICE_ADD_EMBEDDED_REPO,
				  embedded_advice, name.buf, name.buf);
		adviced_on_embedded_repo = 1;
	}

	strbuf_release(&name);
}

ساكن صحيح add_files(بنية repository *repo, بنية dir_struct *dir, صحيح flags)
{
	صحيح i, exit_status = 0;
	بنية string_list matched_sparse_paths = STRING_LIST_INIT_NODUP;

	إذا (dir->ignored_nr) {
		fprintf(stderr, _(ignore_error));
		لكل (i = 0; i < dir->ignored_nr; i++)
			fprintf(stderr, "%s\n", dir->ignored[i]->name);
		advise_if_enabled(ADVICE_ADD_IGNORED_FILE,
				  _("Use -f if you really want to add them."));
		exit_status = 1;
	}

	لكل (i = 0; i < dir->nr; i++) {
		إذا (!include_sparse &&
		    !path_in_sparse_checkout(dir->entries[i]->name, repo->index)) {
			string_list_append(&matched_sparse_paths,
					   dir->entries[i]->name);
			استمر;
		}
		إذا (add_file_to_index(repo->index, dir->entries[i]->name, flags)) {
			إذا (!ignore_add_errors)
				die(_("adding files failed"));
			exit_status = 1;
		} وإلا {
			check_embedded_repo(dir->entries[i]->name);
		}
	}

	إذا (matched_sparse_paths.nr) {
		advise_on_updating_sparse_paths(&matched_sparse_paths);
		exit_status = 1;
	}

	string_list_clear(&matched_sparse_paths, 0);

	أرجع exit_status;
}

ساكن صحيح failed_to_add(صحيح flags, ثابت حرف *path)
{
	إذا (!(flags & ADD_CACHE_IGNORE_ERRORS))
		die(_("updating file '%s' failed"), path);
	أرجع 1;
}

ساكن صحيح add_resolved_files(بنية repository *repo,
			      ثابت بنية pathspec *pathspec,
			      صحيح flags)
{
	بنية index_state *istate = repo->index;
	بنية string_list unmerged_paths = STRING_LIST_INIT_DUP;
	بنية string_list unresolved_paths = STRING_LIST_INIT_DUP;
	صحيح exit_status = 0;
	size_t i;

	لكل (i = 0; i < istate->cache_nr; i++) {
		بنية cache_entry *ce = istate->cache[i];
		إذا (!ce_stage(ce))
			استمر;
		إذا (pathspec->nr && !ce_path_match(istate, ce, pathspec, عدم))
			استمر;
		إذا (!unmerged_paths.nr ||
		    strcmp(unmerged_paths.ابوبو[unmerged_paths.nr - 1].string, ce->name))
			string_list_append(&unmerged_paths, ce->name);
	}

	إذا (!unmerged_paths.nr) {
		string_list_clear(&unmerged_paths, 0);
		أرجع 0;
	}

	لكل (i = 0; i < unmerged_paths.nr; i++) {
		ثابت حرف *path = unmerged_paths.ابوبو[i].string;
		بنية stat st;

		إذا (!lstat(path, &st) && S_ISREG(st.st_mode)) {
			إذا (has_conflict_markers(istate, path))
				string_list_append(&unresolved_paths, path);
		}
	}

	إذا (unresolved_paths.nr) {
		بنية strbuf sb = STRBUF_INIT;
		لكل (i = 0; i < unresolved_paths.nr; i++)
			strbuf_addf(&sb, "\t%s\n", unresolved_paths.ابوبو[i].string);
		die(_("the following paths still have conflict markers:\n%s"), sb.buf);
	}

	لكل (i = 0; i < unmerged_paths.nr; i++) {
		ثابت حرف *path = unmerged_paths.ابوبو[i].string;
		بنية stat st;

		إذا (lstat(path, &st)) {
			إذا (errno != ENOENT)
				die_errno(_("cannot lstat: '%s'"), path);
			إذا (remove_file_from_index_with_flags(istate, path, flags))
				exit_status = failed_to_add(flags, path);
		} وإلا {
			إذا (add_file_to_index(istate, path, flags))
				exit_status = failed_to_add(flags, path);
		}
	}

	string_list_clear(&unmerged_paths, 0);
	string_list_clear(&unresolved_paths, 0);
	أرجع exit_status;
}

صحيح cmd_add(صحيح argc,
	    ثابت حرف **argv,
	    ثابت حرف *prefix,
	    بنية repository *repo)
{
	صحيح exit_status = 0;
	بنية pathspec pathspec;
	بنية dir_struct dir = DIR_INIT;
	صحيح flags;
	صحيح add_new_files;
	صحيح require_pathspec;
	حرف *seen = عدم;
	حرف *ps_matched = عدم;
	بنية lock_file lock_file = LOCK_INIT;
	بنية odb_transaction *transaction = عدم;

	repo_config(repo, add_config, عدم);

	argc = parse_options(argc, argv, prefix, builtin_add_options,
			  builtin_add_usage, PARSE_OPT_KEEP_ARGV0);

	prepare_repo_settings(repo);
	repo->settings.command_requires_full_index = 0;

	إذا (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	إذا (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	إذا (patch_interactive)
		add_interactive = 1;
	إذا (add_interactive) {
		إذا (show_only)
			die(_("options '%s' and '%s' cannot be used together"), "--dry-run", "--interactive/--patch");
		إذا (pathspec_from_file)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--interactive/--patch");
		exit(interactive_add(repo, argv + 1, prefix, patch_interactive, &interactive_opts));
	} وإلا {
		إذا (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--interactive/--patch");
		إذا (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--interactive/--patch");
		إذا (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--interactive/--patch");
	}

	إذا (edit_interactive) {
		إذا (pathspec_from_file)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--edit");
		أرجع(edit_patch(repo, argc, argv, prefix));
	}
	argc--;
	argv++;

	إذا (0 <= addremove_explicit)
		addremove = addremove_explicit;
	وإلا إذا (take_worktree_changes && ADDREMOVE_DEFAULT)
		addremove = 0; /* "-u" was given but not "-A" */

	die_for_incompatible_opt3(take_worktree_changes, "-u/--update",
				  0 < addremove_explicit, "-A/--all",
				  add_resolved, "--resolved");

	إذا (!show_only && ignore_missing)
		die(_("the option '%s' requires '%s'"), "--ignore-missing", "--dry-run");

	إذا (chmod_arg && ((chmod_arg[0] != '-' && chmod_arg[0] != '+') ||
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

	إذا (pathspec_from_file) {
		إذا (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_FULL |
				    PATHSPEC_SYMLINK_LEADING_PATH,
				    prefix, pathspec_from_file, pathspec_file_nul);
	} وإلا إذا (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	إذا (require_pathspec && pathspec.nr == 0) {
		fprintf(stderr, _("Nothing specified, nothing added.\n"));
		advise_if_enabled(ADVICE_ADD_EMPTY_PATHSPEC,
				  _("Maybe you wanted to say 'git add .'?"));
		أرجع 0;
	}

	إذا (!take_worktree_changes && !add_resolved &&
	    addremove_explicit < 0 && pathspec.nr)
		/* Turn "git add pathspec..." to "git add -A pathspec..." */
		addremove = 1;

	flags = ((verbose ? ADD_CACHE_VERBOSE : 0) |
		 (show_only ? ADD_CACHE_PRETEND : 0) |
		 (intent_to_add ? ADD_CACHE_INTENT : 0) |
		 (ignore_add_errors ? ADD_CACHE_IGNORE_ERRORS : 0) |
		 (!(addremove || take_worktree_changes)
		  ? ADD_CACHE_IGNORE_REMOVAL : 0));

	إذا (repo_read_index_preload(repo, &pathspec, 0) < 0)
		die(_("index file corrupt"));

	die_in_unpopulated_submodule(repo->index, prefix);
	die_path_inside_submodule(repo->index, &pathspec);

	إذا (add_new_files) {
		صحيح baselen;

		/* Set up the default git porcelain excludes */
		إذا (!ignored_too) {
			dir.flags |= DIR_COLLECT_IGNORED;
			setup_standard_excludes(&dir);
		}

		/* This picks up the paths that are not tracked */
		baselen = fill_directory(&dir, repo->index, &pathspec);
		إذا (pathspec.nr)
			seen = prune_directory(repo, &dir, &pathspec, baselen);
	}

	إذا (refresh_only) {
		exit_status |= refresh(repo, verbose, &pathspec);
		اذهب finish;
	}

	إذا (pathspec.nr) {
		صحيح i;
		حرف *skip_worktree_seen = عدم;
		بنية string_list only_match_skip_worktree = STRING_LIST_INIT_NODUP;

		إذا (!seen)
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

		لكل (i = 0; i < pathspec.nr; i++) {
			ثابت حرف *path = pathspec.ابوبو[i].match;

			إذا (pathspec.ابوبو[i].magic & PATHSPEC_EXCLUDE)
				استمر;
			إذا (seen[i])
				استمر;

			إذا (!include_sparse &&
			    matches_skip_worktree(&pathspec, i, &skip_worktree_seen)) {
				string_list_append(&only_match_skip_worktree,
						   pathspec.ابوبو[i].original);
				استمر;
			}

			/* Don't complain at 'git add .' on empty repo */
			إذا (!path[0])
				استمر;

			إذا ((pathspec.ابوبو[i].magic & (PATHSPEC_GLOB | PATHSPEC_ICASE)) ||
			    !file_exists(path)) {
				إذا (ignore_missing) {
					صحيح dtype = DT_UNKNOWN;
					إذا (is_excluded(&dir, repo->index, path, &dtype))
						dir_add_ignored(&dir, repo->index,
								path, pathspec.ابوبو[i].len);
				} وإلا
					die(_("pathspec '%s' did not match any files"),
					    pathspec.ابوبو[i].original);
			}
		}


		إذا (only_match_skip_worktree.nr) {
			advise_on_updating_sparse_paths(&only_match_skip_worktree);
			exit_status = 1;
		}

		حرر(seen);
		حرر(skip_worktree_seen);
		string_list_clear(&only_match_skip_worktree, 0);
	}

	odb_transaction_begin_or_die(repo->objects, &transaction, 0);

	ps_matched = xcalloc(pathspec.nr, 1);
	إذا (add_resolved)
		exit_status |= add_resolved_files(repo, &pathspec, flags);
	وإلا إذا (add_renormalize)
		exit_status |= renormalize_tracked_files(repo, &pathspec, flags);
	وإلا
		exit_status |= add_files_to_cache(repo, prefix,
						  &pathspec, ps_matched,
						  include_sparse, flags, ignored_too);

	إذا (take_worktree_changes && !add_renormalize && !ignore_add_errors &&
	    report_path_error(ps_matched, &pathspec))
		exit(128);

	إذا (add_new_files)
		exit_status |= add_files(repo, &dir, flags);

	إذا (chmod_arg && pathspec.nr)
		exit_status |= chmod_pathspec(repo, &pathspec, chmod_arg[0], show_only);
	odb_transaction_commit_and_finalize_or_die(transaction);

finish:
	إذا (write_locked_index(repo->index, &lock_file,
			       COMMIT_LOCK | SKIP_IF_UNCHANGED))
		die(_("unable to write new index file"));

	حرر(ps_matched);
	dir_clear(&dir);
	clear_pathspec(&pathspec);
	أرجع exit_status;
}
