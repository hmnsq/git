/*
 * "git add" builtin command
 *
 * Copyright (C) 2006 Linus Torvalds
 */

#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "advice.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "lockfile.h"
#அந்தர்பாவ "editor.h"
#அந்தர்பாவ "dir.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "pathspec.h"
#அந்தர்பாவ "object-file.h"
#அந்தர்பாவ "odb.h"
#அந்தர்பாவ "odb/transaction.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "path.h"
#அந்தர்பாவ "preload-index.h"
#அந்தர்பாவ "diff.h"
#அந்தர்பாவ "read-cache.h"
#அந்தர்பாவ "revision.h"
#அந்தர்பாவ "submodule.h"
#அந்தர்பாவ "add-interactive.h"
#அந்தர்பாவ "merge-ll.h"
#அந்தர்பாவ "apply.h"

நிலையான மாறிலி எழுத்து * மாறிலி builtin_add_usage[] = {
	N_("git add [<options>] [--] <pathspec>..."),
	வெற்று
};
நிலையான முழு patch_interactive, add_interactive, edit_interactive;
நிலையான கட்டமைப்பு interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
நிலையான முழு take_worktree_changes;
நிலையான முழு add_renormalize;
நிலையான முழு add_resolved;
நிலையான முழு pathspec_file_nul;
நிலையான முழு include_sparse;
நிலையான மாறிலி எழுத்து *pathspec_from_file;

நிலையான முழு chmod_pathspec(கட்டமைப்பு repository *repo,
			  கட்டமைப்பு pathspec *pathspec,
			  எழுத்து flip,
			  முழு show_only)
{
	முழு ret = 0;

	க்கு (size_t i = 0; i < repo->index->cache_nr; i++) {
		கட்டமைப்பு cache_entry *ce = repo->index->cache[i];
		முழு err;

		எனில் (!include_sparse &&
		    (ce_skip_worktree(ce) ||
		     !path_in_sparse_checkout(ce->name, repo->index)))
			தொடர்;

		எனில் (pathspec && !ce_path_match(repo->index, ce, pathspec, வெற்று))
			தொடர்;

		எனில் (!show_only)
			err = chmod_index_entry(repo->index, ce, flip);
		இல்லையெனில்
			err = S_ISREG(ce->ce_mode) ? 0 : -1;

		எனில் (err < 0)
			ret = error(_("cannot chmod %cx '%s'"), flip, ce->name);
	}

	திரும்பு ret;
}

நிலையான முழு renormalize_tracked_files(கட்டமைப்பு repository *repo,
				     மாறிலி கட்டமைப்பு pathspec *pathspec,
				     முழு flags)
{
	முழு retval = 0;

	க்கு (size_t i = 0; i < repo->index->cache_nr; i++) {
		கட்டமைப்பு cache_entry *ce = repo->index->cache[i];

		எனில் (!include_sparse &&
		    (ce_skip_worktree(ce) ||
		     !path_in_sparse_checkout(ce->name, repo->index)))
			தொடர்;
		எனில் (ce_stage(ce))
			தொடர்; /* do not touch unmerged paths */
		எனில் (!S_ISREG(ce->ce_mode) && !S_ISLNK(ce->ce_mode))
			தொடர்; /* do not touch non blobs */
		எனில் (pathspec && !ce_path_match(repo->index, ce, pathspec, வெற்று))
			தொடர்;
		retval |= add_file_to_index(repo->index, ce->name,
					    flags | ADD_CACHE_RENORMALIZE);
	}

	திரும்பு retval;
}

நிலையான எழுத்து *prune_directory(கட்டமைப்பு repository *repo,
			     கட்டமைப்பு dir_struct *dir,
			     கட்டமைப்பு pathspec *pathspec,
			     முழு prefix)
{
	எழுத்து *seen;
	முழு i;
	கட்டமைப்பு dir_entry **src, **dst;

	seen = xcalloc(pathspec->nr, 1);

	src = dst = dir->entries;
	i = dir->nr;
	போது (--i >= 0) {
		கட்டமைப்பு dir_entry *entry = *src++;
		எனில் (dir_path_match(repo->index, entry, pathspec, prefix, seen))
			*dst++ = entry;
	}
	dir->nr = dst - dir->entries;
	add_pathspec_matches_against_index(pathspec, repo->index, seen,
					   PS_IGNORE_SKIP_WORKTREE);
	திரும்பு seen;
}

நிலையான முழு refresh(கட்டமைப்பு repository *repo, முழு verbose, மாறிலி கட்டமைப்பு pathspec *pathspec)
{
	எழுத்து *seen;
	முழு i, ret = 0;
	எழுத்து *skip_worktree_seen = வெற்று;
	கட்டமைப்பு string_list only_match_skip_worktree = STRING_LIST_INIT_NODUP;
	unsigned முழு flags = REFRESH_IGNORE_SKIP_WORKTREE |
		    (verbose ? REFRESH_IN_PORCELAIN : REFRESH_QUIET);

	seen = xcalloc(pathspec->nr, 1);
	refresh_index(repo->index, flags, pathspec, seen,
		      _("Unstaged changes after refreshing the index:"));
	க்கு (i = 0; i < pathspec->nr; i++) {
		எனில் (!seen[i]) {
			மாறிலி எழுத்து *path = pathspec->பொருட்கள்[i].original;

			எனில் (matches_skip_worktree(pathspec, i, &skip_worktree_seen) ||
			    !path_in_sparse_checkout(path, repo->index)) {
				string_list_append(&only_match_skip_worktree,
						   pathspec->பொருட்கள்[i].original);
			} இல்லையெனில் {
				die(_("pathspec '%s' did not match any files"),
				    pathspec->பொருட்கள்[i].original);
			}
		}
	}

	எனில் (only_match_skip_worktree.nr) {
		advise_on_updating_sparse_paths(&only_match_skip_worktree);
		ret = 1;
	}

	விடுவி(seen);
	விடுவி(skip_worktree_seen);
	string_list_clear(&only_match_skip_worktree, 0);
	திரும்பு ret;
}

முழு interactive_add(கட்டமைப்பு repository *repo,
		    மாறிலி எழுத்து **argv,
		    மாறிலி எழுத்து *prefix,
		    முழு patch, கட்டமைப்பு interactive_options *interactive_opts)
{
	கட்டமைப்பு pathspec pathspec;
	முழு ret;

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_FULL |
		       PATHSPEC_SYMLINK_LEADING_PATH |
		       PATHSPEC_PREFIX_ORIGIN,
		       prefix, argv);

	எனில் (patch)
		ret = !!run_add_p(repo, ADD_P_ADD, interactive_opts, வெற்று, &pathspec, 0);
	இல்லையெனில்
		ret = !!run_add_i(repo, &pathspec, interactive_opts);

	clear_pathspec(&pathspec);
	திரும்பு ret;
}

நிலையான முழு edit_patch(கட்டமைப்பு repository *repo,
		      முழு argc,
		      மாறிலி எழுத்து **argv,
		      மாறிலி எழுத்து *prefix)
{
	எழுத்து *file = repo_git_path(repo, "ADD_EDIT.patch");
	கட்டமைப்பு apply_state state;
	மாறிலி எழுத்து *apply_argv[2];
	கட்டமைப்பு rev_info rev;
	முழு out;
	கட்டமைப்பு stat st;

	repo_config(repo, git_diff_basic_config, வெற்று);

	எனில் (repo_read_index(repo) < 0)
		die(_("could not read the index"));

	repo_init_revisions(repo, &rev, prefix);
	rev.diffopt.context = 7;

	argc = setup_revisions(argc, argv, &rev, வெற்று);
	rev.diffopt.output_format = DIFF_FORMAT_PATCH;
	rev.diffopt.use_color = GIT_COLOR_NEVER;
	rev.diffopt.flags.ignore_dirty_submodules = 1;
	out = xopen(file, O_CREAT | O_WRONLY | O_TRUNC, 0666);
	rev.diffopt.file = xfdopen(out, "w");
	rev.diffopt.close_file = 1;
	run_diff_files(&rev, 0);

	எனில் (launch_editor(file, வெற்று, வெற்று))
		die(_("editing patch failed"));

	எனில் (stat(file, &st))
		die_errno(_("could not stat '%s'"), file);
	எனில் (!st.st_size)
		die(_("empty patch. aborted"));

	apply_argv[0] = file;
	apply_argv[1] = வெற்று;
	எனில் (init_apply_state(&state, repo, வெற்று))
		die(_("could not initialize apply state"));
	state.cached = 1;
	எனில் (check_apply_state(&state, 0))
		die(_("could not check apply state"));
	எனில் (apply_all_patches(&state, 1, apply_argv, APPLY_OPT_RECOUNT))
		die(_("could not apply '%s'"), file);
	clear_apply_state(&state);

	unlink(file);
	விடுவி(file);
	release_revisions(&rev);
	திரும்பு 0;
}

நிலையான மாறிலி எழுத்து ignore_error[] =
N_("The following paths are ignored by one of your .gitignore files:\n");

நிலையான முழு verbose, show_only, ignored_too, refresh_only;
நிலையான முழு ignore_add_errors, intent_to_add, ignore_missing;
நிலையான முழு warn_on_embedded_repo = 1;

#define ADDREMOVE_DEFAULT 1
நிலையான முழு addremove = ADDREMOVE_DEFAULT;
நிலையான முழு addremove_explicit = -1; /* unspecified */

நிலையான எழுத்து *chmod_arg;

நிலையான முழு ignore_removal_cb(மாறிலி கட்டமைப்பு option *opt, மாறிலி எழுத்து *arg, முழு unset)
{
	BUG_ON_OPT_ARG(arg);

	/* if we are told to ignore, we are not adding removals */
	*(முழு *)opt->மதிப்பு = !unset ? 0 : 1;
	திரும்பு 0;
}

நிலையான கட்டமைப்பு option builtin_add_options[] = {
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
	  வெற்று /* takes no arguments */,
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

நிலையான முழு add_config(மாறிலி எழுத்து *var, மாறிலி எழுத்து *மதிப்பு,
		      மாறிலி கட்டமைப்பு config_context *ctx, வெறுமை *cb)
{
	எனில் (!strcmp(var, "add.ignoreerrors") ||
	    !strcmp(var, "add.ignore-errors")) {
		ignore_add_errors = git_config_bool(var, மதிப்பு);
		திரும்பு 0;
	}

	எனில் (git_color_config(var, மதிப்பு, cb) < 0)
		திரும்பு -1;

	திரும்பு git_default_config(var, மதிப்பு, ctx, cb);
}

நிலையான மாறிலி எழுத்து embedded_advice[] = N_(
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

நிலையான வெறுமை check_embedded_repo(மாறிலி எழுத்து *path)
{
	கட்டமைப்பு strbuf name = STRBUF_INIT;
	நிலையான முழு adviced_on_embedded_repo = 0;

	எனில் (!warn_on_embedded_repo)
		திரும்பு;
	எனில் (!ends_with(path, "/"))
		திரும்பு;

	/* Drop trailing slash for aesthetics */
	strbuf_addstr(&name, path);
	strbuf_strip_suffix(&name, "/");

	warning(_("adding embedded git repository: %s"), name.buf);
	எனில் (!adviced_on_embedded_repo) {
		advise_if_enabled(ADVICE_ADD_EMBEDDED_REPO,
				  embedded_advice, name.buf, name.buf);
		adviced_on_embedded_repo = 1;
	}

	strbuf_release(&name);
}

நிலையான முழு add_files(கட்டமைப்பு repository *repo, கட்டமைப்பு dir_struct *dir, முழு flags)
{
	முழு i, exit_status = 0;
	கட்டமைப்பு string_list matched_sparse_paths = STRING_LIST_INIT_NODUP;

	எனில் (dir->ignored_nr) {
		fprintf(stderr, _(ignore_error));
		க்கு (i = 0; i < dir->ignored_nr; i++)
			fprintf(stderr, "%s\n", dir->ignored[i]->name);
		advise_if_enabled(ADVICE_ADD_IGNORED_FILE,
				  _("Use -f if you really want to add them."));
		exit_status = 1;
	}

	க்கு (i = 0; i < dir->nr; i++) {
		எனில் (!include_sparse &&
		    !path_in_sparse_checkout(dir->entries[i]->name, repo->index)) {
			string_list_append(&matched_sparse_paths,
					   dir->entries[i]->name);
			தொடர்;
		}
		எனில் (add_file_to_index(repo->index, dir->entries[i]->name, flags)) {
			எனில் (!ignore_add_errors)
				die(_("adding files failed"));
			exit_status = 1;
		} இல்லையெனில் {
			check_embedded_repo(dir->entries[i]->name);
		}
	}

	எனில் (matched_sparse_paths.nr) {
		advise_on_updating_sparse_paths(&matched_sparse_paths);
		exit_status = 1;
	}

	string_list_clear(&matched_sparse_paths, 0);

	திரும்பு exit_status;
}

நிலையான முழு failed_to_add(முழு flags, மாறிலி எழுத்து *path)
{
	எனில் (!(flags & ADD_CACHE_IGNORE_ERRORS))
		die(_("updating file '%s' failed"), path);
	திரும்பு 1;
}

நிலையான முழு add_resolved_files(கட்டமைப்பு repository *repo,
			      மாறிலி கட்டமைப்பு pathspec *pathspec,
			      முழு flags)
{
	கட்டமைப்பு index_state *istate = repo->index;
	கட்டமைப்பு string_list unmerged_paths = STRING_LIST_INIT_DUP;
	கட்டமைப்பு string_list unresolved_paths = STRING_LIST_INIT_DUP;
	முழு exit_status = 0;
	size_t i;

	க்கு (i = 0; i < istate->cache_nr; i++) {
		கட்டமைப்பு cache_entry *ce = istate->cache[i];
		எனில் (!ce_stage(ce))
			தொடர்;
		எனில் (pathspec->nr && !ce_path_match(istate, ce, pathspec, வெற்று))
			தொடர்;
		எனில் (!unmerged_paths.nr ||
		    strcmp(unmerged_paths.பொருட்கள்[unmerged_paths.nr - 1].string, ce->name))
			string_list_append(&unmerged_paths, ce->name);
	}

	எனில் (!unmerged_paths.nr) {
		string_list_clear(&unmerged_paths, 0);
		திரும்பு 0;
	}

	க்கு (i = 0; i < unmerged_paths.nr; i++) {
		மாறிலி எழுத்து *path = unmerged_paths.பொருட்கள்[i].string;
		கட்டமைப்பு stat st;

		எனில் (!lstat(path, &st) && S_ISREG(st.st_mode)) {
			எனில் (has_conflict_markers(istate, path))
				string_list_append(&unresolved_paths, path);
		}
	}

	எனில் (unresolved_paths.nr) {
		கட்டமைப்பு strbuf sb = STRBUF_INIT;
		க்கு (i = 0; i < unresolved_paths.nr; i++)
			strbuf_addf(&sb, "\t%s\n", unresolved_paths.பொருட்கள்[i].string);
		die(_("the following paths still have conflict markers:\n%s"), sb.buf);
	}

	க்கு (i = 0; i < unmerged_paths.nr; i++) {
		மாறிலி எழுத்து *path = unmerged_paths.பொருட்கள்[i].string;
		கட்டமைப்பு stat st;

		எனில் (lstat(path, &st)) {
			எனில் (errno != ENOENT)
				die_errno(_("cannot lstat: '%s'"), path);
			எனில் (remove_file_from_index_with_flags(istate, path, flags))
				exit_status = failed_to_add(flags, path);
		} இல்லையெனில் {
			எனில் (add_file_to_index(istate, path, flags))
				exit_status = failed_to_add(flags, path);
		}
	}

	string_list_clear(&unmerged_paths, 0);
	string_list_clear(&unresolved_paths, 0);
	திரும்பு exit_status;
}

முழு cmd_add(முழு argc,
	    மாறிலி எழுத்து **argv,
	    மாறிலி எழுத்து *prefix,
	    கட்டமைப்பு repository *repo)
{
	முழு exit_status = 0;
	கட்டமைப்பு pathspec pathspec;
	கட்டமைப்பு dir_struct dir = DIR_INIT;
	முழு flags;
	முழு add_new_files;
	முழு require_pathspec;
	எழுத்து *seen = வெற்று;
	எழுத்து *ps_matched = வெற்று;
	கட்டமைப்பு lock_file lock_file = LOCK_INIT;
	கட்டமைப்பு odb_transaction *transaction = வெற்று;

	repo_config(repo, add_config, வெற்று);

	argc = parse_options(argc, argv, prefix, builtin_add_options,
			  builtin_add_usage, PARSE_OPT_KEEP_ARGV0);

	prepare_repo_settings(repo);
	repo->settings.command_requires_full_index = 0;

	எனில் (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	எனில் (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	எனில் (patch_interactive)
		add_interactive = 1;
	எனில் (add_interactive) {
		எனில் (show_only)
			die(_("options '%s' and '%s' cannot be used together"), "--dry-run", "--interactive/--patch");
		எனில் (pathspec_from_file)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--interactive/--patch");
		exit(interactive_add(repo, argv + 1, prefix, patch_interactive, &interactive_opts));
	} இல்லையெனில் {
		எனில் (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--interactive/--patch");
		எனில் (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--interactive/--patch");
		எனில் (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--interactive/--patch");
	}

	எனில் (edit_interactive) {
		எனில் (pathspec_from_file)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--edit");
		திரும்பு(edit_patch(repo, argc, argv, prefix));
	}
	argc--;
	argv++;

	எனில் (0 <= addremove_explicit)
		addremove = addremove_explicit;
	இல்லையெனில் எனில் (take_worktree_changes && ADDREMOVE_DEFAULT)
		addremove = 0; /* "-u" was given but not "-A" */

	die_for_incompatible_opt3(take_worktree_changes, "-u/--update",
				  0 < addremove_explicit, "-A/--all",
				  add_resolved, "--resolved");

	எனில் (!show_only && ignore_missing)
		die(_("the option '%s' requires '%s'"), "--ignore-missing", "--dry-run");

	எனில் (chmod_arg && ((chmod_arg[0] != '-' && chmod_arg[0] != '+') ||
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

	எனில் (pathspec_from_file) {
		எனில் (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_FULL |
				    PATHSPEC_SYMLINK_LEADING_PATH,
				    prefix, pathspec_from_file, pathspec_file_nul);
	} இல்லையெனில் எனில் (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	எனில் (require_pathspec && pathspec.nr == 0) {
		fprintf(stderr, _("Nothing specified, nothing added.\n"));
		advise_if_enabled(ADVICE_ADD_EMPTY_PATHSPEC,
				  _("Maybe you wanted to say 'git add .'?"));
		திரும்பு 0;
	}

	எனில் (!take_worktree_changes && !add_resolved &&
	    addremove_explicit < 0 && pathspec.nr)
		/* Turn "git add pathspec..." to "git add -A pathspec..." */
		addremove = 1;

	flags = ((verbose ? ADD_CACHE_VERBOSE : 0) |
		 (show_only ? ADD_CACHE_PRETEND : 0) |
		 (intent_to_add ? ADD_CACHE_INTENT : 0) |
		 (ignore_add_errors ? ADD_CACHE_IGNORE_ERRORS : 0) |
		 (!(addremove || take_worktree_changes)
		  ? ADD_CACHE_IGNORE_REMOVAL : 0));

	எனில் (repo_read_index_preload(repo, &pathspec, 0) < 0)
		die(_("index file corrupt"));

	die_in_unpopulated_submodule(repo->index, prefix);
	die_path_inside_submodule(repo->index, &pathspec);

	எனில் (add_new_files) {
		முழு baselen;

		/* Set up the default git porcelain excludes */
		எனில் (!ignored_too) {
			dir.flags |= DIR_COLLECT_IGNORED;
			setup_standard_excludes(&dir);
		}

		/* This picks up the paths that are not tracked */
		baselen = fill_directory(&dir, repo->index, &pathspec);
		எனில் (pathspec.nr)
			seen = prune_directory(repo, &dir, &pathspec, baselen);
	}

	எனில் (refresh_only) {
		exit_status |= refresh(repo, verbose, &pathspec);
		செல் finish;
	}

	எனில் (pathspec.nr) {
		முழு i;
		எழுத்து *skip_worktree_seen = வெற்று;
		கட்டமைப்பு string_list only_match_skip_worktree = STRING_LIST_INIT_NODUP;

		எனில் (!seen)
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

		க்கு (i = 0; i < pathspec.nr; i++) {
			மாறிலி எழுத்து *path = pathspec.பொருட்கள்[i].match;

			எனில் (pathspec.பொருட்கள்[i].magic & PATHSPEC_EXCLUDE)
				தொடர்;
			எனில் (seen[i])
				தொடர்;

			எனில் (!include_sparse &&
			    matches_skip_worktree(&pathspec, i, &skip_worktree_seen)) {
				string_list_append(&only_match_skip_worktree,
						   pathspec.பொருட்கள்[i].original);
				தொடர்;
			}

			/* Don't complain at 'git add .' on empty repo */
			எனில் (!path[0])
				தொடர்;

			எனில் ((pathspec.பொருட்கள்[i].magic & (PATHSPEC_GLOB | PATHSPEC_ICASE)) ||
			    !file_exists(path)) {
				எனில் (ignore_missing) {
					முழு dtype = DT_UNKNOWN;
					எனில் (is_excluded(&dir, repo->index, path, &dtype))
						dir_add_ignored(&dir, repo->index,
								path, pathspec.பொருட்கள்[i].len);
				} இல்லையெனில்
					die(_("pathspec '%s' did not match any files"),
					    pathspec.பொருட்கள்[i].original);
			}
		}


		எனில் (only_match_skip_worktree.nr) {
			advise_on_updating_sparse_paths(&only_match_skip_worktree);
			exit_status = 1;
		}

		விடுவி(seen);
		விடுவி(skip_worktree_seen);
		string_list_clear(&only_match_skip_worktree, 0);
	}

	odb_transaction_begin_or_die(repo->objects, &transaction, 0);

	ps_matched = xcalloc(pathspec.nr, 1);
	எனில் (add_resolved)
		exit_status |= add_resolved_files(repo, &pathspec, flags);
	இல்லையெனில் எனில் (add_renormalize)
		exit_status |= renormalize_tracked_files(repo, &pathspec, flags);
	இல்லையெனில்
		exit_status |= add_files_to_cache(repo, prefix,
						  &pathspec, ps_matched,
						  include_sparse, flags, ignored_too);

	எனில் (take_worktree_changes && !add_renormalize && !ignore_add_errors &&
	    report_path_error(ps_matched, &pathspec))
		exit(128);

	எனில் (add_new_files)
		exit_status |= add_files(repo, &dir, flags);

	எனில் (chmod_arg && pathspec.nr)
		exit_status |= chmod_pathspec(repo, &pathspec, chmod_arg[0], show_only);
	odb_transaction_commit_and_finalize_or_die(transaction);

finish:
	எனில் (write_locked_index(repo->index, &lock_file,
			       COMMIT_LOCK | SKIP_IF_UNCHANGED))
		die(_("unable to write new index file"));

	விடுவி(ps_matched);
	dir_clear(&dir);
	clear_pathspec(&pathspec);
	திரும்பு exit_status;
}
