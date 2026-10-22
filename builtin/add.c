/*
 * "git add" builtin command
 *
 * Copyright (C) 2006 Linus Torvalds
 */

#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "advice.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "lockfile.h"
#അന്തര്ഭാവ "editor.h"
#അന്തര്ഭാവ "dir.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "pathspec.h"
#അന്തര്ഭാവ "object-file.h"
#അന്തര്ഭാവ "odb.h"
#അന്തര്ഭാവ "odb/transaction.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "path.h"
#അന്തര്ഭാവ "preload-index.h"
#അന്തര്ഭാവ "diff.h"
#അന്തര്ഭാവ "read-cache.h"
#അന്തര്ഭാവ "revision.h"
#അന്തര്ഭാവ "submodule.h"
#അന്തര്ഭാവ "add-interactive.h"
#അന്തര്ഭാവ "merge-ll.h"
#അന്തര്ഭാവ "apply.h"

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം builtin_add_usage[] = {
	N_("git add [<options>] [--] <pathspec>..."),
	ശൂന്യ
};
സ്ഥിതം പൂർണ്ണസംഖ്യ patch_interactive, add_interactive, edit_interactive;
സ്ഥിതം ഘടന interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
സ്ഥിതം പൂർണ്ണസംഖ്യ take_worktree_changes;
സ്ഥിതം പൂർണ്ണസംഖ്യ add_renormalize;
സ്ഥിതം പൂർണ്ണസംഖ്യ add_resolved;
സ്ഥിതം പൂർണ്ണസംഖ്യ pathspec_file_nul;
സ്ഥിതം പൂർണ്ണസംഖ്യ include_sparse;
സ്ഥിതം സ്ഥിരം അക്ഷരം *pathspec_from_file;

സ്ഥിതം പൂർണ്ണസംഖ്യ chmod_pathspec(ഘടന repository *repo,
			  ഘടന pathspec *pathspec,
			  അക്ഷരം flip,
			  പൂർണ്ണസംഖ്യ show_only)
{
	പൂർണ്ണസംഖ്യ ret = 0;

	വേണ്ടി (size_t i = 0; i < repo->index->cache_nr; i++) {
		ഘടന cache_entry *ce = repo->index->cache[i];
		പൂർണ്ണസംഖ്യ err;

		എങ്കിൽ (!include_sparse &&
		    (ce_skip_worktree(ce) ||
		     !path_in_sparse_checkout(ce->name, repo->index)))
			തുടരുക;

		എങ്കിൽ (pathspec && !ce_path_match(repo->index, ce, pathspec, ശൂന്യ))
			തുടരുക;

		എങ്കിൽ (!show_only)
			err = chmod_index_entry(repo->index, ce, flip);
		അല്ലെങ്കിൽ
			err = S_ISREG(ce->ce_mode) ? 0 : -1;

		എങ്കിൽ (err < 0)
			ret = error(_("cannot chmod %cx '%s'"), flip, ce->name);
	}

	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ renormalize_tracked_files(ഘടന repository *repo,
				     സ്ഥിരം ഘടന pathspec *pathspec,
				     പൂർണ്ണസംഖ്യ flags)
{
	പൂർണ്ണസംഖ്യ retval = 0;

	വേണ്ടി (size_t i = 0; i < repo->index->cache_nr; i++) {
		ഘടന cache_entry *ce = repo->index->cache[i];

		എങ്കിൽ (!include_sparse &&
		    (ce_skip_worktree(ce) ||
		     !path_in_sparse_checkout(ce->name, repo->index)))
			തുടരുക;
		എങ്കിൽ (ce_stage(ce))
			തുടരുക; /* do not touch unmerged paths */
		എങ്കിൽ (!S_ISREG(ce->ce_mode) && !S_ISLNK(ce->ce_mode))
			തുടരുക; /* do not touch non blobs */
		എങ്കിൽ (pathspec && !ce_path_match(repo->index, ce, pathspec, ശൂന്യ))
			തുടരുക;
		retval |= add_file_to_index(repo->index, ce->name,
					    flags | ADD_CACHE_RENORMALIZE);
	}

	തിരികെ retval;
}

സ്ഥിതം അക്ഷരം *prune_directory(ഘടന repository *repo,
			     ഘടന dir_struct *dir,
			     ഘടന pathspec *pathspec,
			     പൂർണ്ണസംഖ്യ prefix)
{
	അക്ഷരം *seen;
	പൂർണ്ണസംഖ്യ i;
	ഘടന dir_entry **src, **dst;

	seen = xcalloc(pathspec->nr, 1);

	src = dst = dir->entries;
	i = dir->nr;
	ആകുമ്പോൾ (--i >= 0) {
		ഘടന dir_entry *entry = *src++;
		എങ്കിൽ (dir_path_match(repo->index, entry, pathspec, prefix, seen))
			*dst++ = entry;
	}
	dir->nr = dst - dir->entries;
	add_pathspec_matches_against_index(pathspec, repo->index, seen,
					   PS_IGNORE_SKIP_WORKTREE);
	തിരികെ seen;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ refresh(ഘടന repository *repo, പൂർണ്ണസംഖ്യ verbose, സ്ഥിരം ഘടന pathspec *pathspec)
{
	അക്ഷരം *seen;
	പൂർണ്ണസംഖ്യ i, ret = 0;
	അക്ഷരം *skip_worktree_seen = ശൂന്യ;
	ഘടന string_list only_match_skip_worktree = STRING_LIST_INIT_NODUP;
	unsigned പൂർണ്ണസംഖ്യ flags = REFRESH_IGNORE_SKIP_WORKTREE |
		    (verbose ? REFRESH_IN_PORCELAIN : REFRESH_QUIET);

	seen = xcalloc(pathspec->nr, 1);
	refresh_index(repo->index, flags, pathspec, seen,
		      _("Unstaged changes after refreshing the index:"));
	വേണ്ടി (i = 0; i < pathspec->nr; i++) {
		എങ്കിൽ (!seen[i]) {
			സ്ഥിരം അക്ഷരം *path = pathspec->ഇനങ്ങൾ[i].original;

			എങ്കിൽ (matches_skip_worktree(pathspec, i, &skip_worktree_seen) ||
			    !path_in_sparse_checkout(path, repo->index)) {
				string_list_append(&only_match_skip_worktree,
						   pathspec->ഇനങ്ങൾ[i].original);
			} അല്ലെങ്കിൽ {
				die(_("pathspec '%s' did not match any files"),
				    pathspec->ഇനങ്ങൾ[i].original);
			}
		}
	}

	എങ്കിൽ (only_match_skip_worktree.nr) {
		advise_on_updating_sparse_paths(&only_match_skip_worktree);
		ret = 1;
	}

	സ്വതന്ത്രം(seen);
	സ്വതന്ത്രം(skip_worktree_seen);
	string_list_clear(&only_match_skip_worktree, 0);
	തിരികെ ret;
}

പൂർണ്ണസംഖ്യ interactive_add(ഘടന repository *repo,
		    സ്ഥിരം അക്ഷരം **argv,
		    സ്ഥിരം അക്ഷരം *prefix,
		    പൂർണ്ണസംഖ്യ patch, ഘടന interactive_options *interactive_opts)
{
	ഘടന pathspec pathspec;
	പൂർണ്ണസംഖ്യ ret;

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_FULL |
		       PATHSPEC_SYMLINK_LEADING_PATH |
		       PATHSPEC_PREFIX_ORIGIN,
		       prefix, argv);

	എങ്കിൽ (patch)
		ret = !!run_add_p(repo, ADD_P_ADD, interactive_opts, ശൂന്യ, &pathspec, 0);
	അല്ലെങ്കിൽ
		ret = !!run_add_i(repo, &pathspec, interactive_opts);

	clear_pathspec(&pathspec);
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ edit_patch(ഘടന repository *repo,
		      പൂർണ്ണസംഖ്യ argc,
		      സ്ഥിരം അക്ഷരം **argv,
		      സ്ഥിരം അക്ഷരം *prefix)
{
	അക്ഷരം *file = repo_git_path(repo, "ADD_EDIT.patch");
	ഘടന apply_state state;
	സ്ഥിരം അക്ഷരം *apply_argv[2];
	ഘടന rev_info rev;
	പൂർണ്ണസംഖ്യ out;
	ഘടന stat st;

	repo_config(repo, git_diff_basic_config, ശൂന്യ);

	എങ്കിൽ (repo_read_index(repo) < 0)
		die(_("could not read the index"));

	repo_init_revisions(repo, &rev, prefix);
	rev.diffopt.context = 7;

	argc = setup_revisions(argc, argv, &rev, ശൂന്യ);
	rev.diffopt.output_format = DIFF_FORMAT_PATCH;
	rev.diffopt.use_color = GIT_COLOR_NEVER;
	rev.diffopt.flags.ignore_dirty_submodules = 1;
	out = xopen(file, O_CREAT | O_WRONLY | O_TRUNC, 0666);
	rev.diffopt.file = xfdopen(out, "w");
	rev.diffopt.close_file = 1;
	run_diff_files(&rev, 0);

	എങ്കിൽ (launch_editor(file, ശൂന്യ, ശൂന്യ))
		die(_("editing patch failed"));

	എങ്കിൽ (stat(file, &st))
		die_errno(_("could not stat '%s'"), file);
	എങ്കിൽ (!st.st_size)
		die(_("empty patch. aborted"));

	apply_argv[0] = file;
	apply_argv[1] = ശൂന്യ;
	എങ്കിൽ (init_apply_state(&state, repo, ശൂന്യ))
		die(_("could not initialize apply state"));
	state.cached = 1;
	എങ്കിൽ (check_apply_state(&state, 0))
		die(_("could not check apply state"));
	എങ്കിൽ (apply_all_patches(&state, 1, apply_argv, APPLY_OPT_RECOUNT))
		die(_("could not apply '%s'"), file);
	clear_apply_state(&state);

	unlink(file);
	സ്വതന്ത്രം(file);
	release_revisions(&rev);
	തിരികെ 0;
}

സ്ഥിതം സ്ഥിരം അക്ഷരം ignore_error[] =
N_("The following paths are ignored by one of your .gitignore files:\n");

സ്ഥിതം പൂർണ്ണസംഖ്യ verbose, show_only, ignored_too, refresh_only;
സ്ഥിതം പൂർണ്ണസംഖ്യ ignore_add_errors, intent_to_add, ignore_missing;
സ്ഥിതം പൂർണ്ണസംഖ്യ warn_on_embedded_repo = 1;

#define ADDREMOVE_DEFAULT 1
സ്ഥിതം പൂർണ്ണസംഖ്യ addremove = ADDREMOVE_DEFAULT;
സ്ഥിതം പൂർണ്ണസംഖ്യ addremove_explicit = -1; /* unspecified */

സ്ഥിതം അക്ഷരം *chmod_arg;

സ്ഥിതം പൂർണ്ണസംഖ്യ ignore_removal_cb(സ്ഥിരം ഘടന option *opt, സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	BUG_ON_OPT_ARG(arg);

	/* if we are told to ignore, we are not adding removals */
	*(പൂർണ്ണസംഖ്യ *)opt->മൂല്യം = !unset ? 0 : 1;
	തിരികെ 0;
}

സ്ഥിതം ഘടന option builtin_add_options[] = {
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
	  ശൂന്യ /* takes no arguments */,
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

സ്ഥിതം പൂർണ്ണസംഖ്യ add_config(സ്ഥിരം അക്ഷരം *var, സ്ഥിരം അക്ഷരം *മൂല്യം,
		      സ്ഥിരം ഘടന config_context *ctx, ശൂന്യം *cb)
{
	എങ്കിൽ (!strcmp(var, "add.ignoreerrors") ||
	    !strcmp(var, "add.ignore-errors")) {
		ignore_add_errors = git_config_bool(var, മൂല്യം);
		തിരികെ 0;
	}

	എങ്കിൽ (git_color_config(var, മൂല്യം, cb) < 0)
		തിരികെ -1;

	തിരികെ git_default_config(var, മൂല്യം, ctx, cb);
}

സ്ഥിതം സ്ഥിരം അക്ഷരം embedded_advice[] = N_(
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

സ്ഥിതം ശൂന്യം check_embedded_repo(സ്ഥിരം അക്ഷരം *path)
{
	ഘടന strbuf name = STRBUF_INIT;
	സ്ഥിതം പൂർണ്ണസംഖ്യ adviced_on_embedded_repo = 0;

	എങ്കിൽ (!warn_on_embedded_repo)
		തിരികെ;
	എങ്കിൽ (!ends_with(path, "/"))
		തിരികെ;

	/* Drop trailing slash for aesthetics */
	strbuf_addstr(&name, path);
	strbuf_strip_suffix(&name, "/");

	warning(_("adding embedded git repository: %s"), name.buf);
	എങ്കിൽ (!adviced_on_embedded_repo) {
		advise_if_enabled(ADVICE_ADD_EMBEDDED_REPO,
				  embedded_advice, name.buf, name.buf);
		adviced_on_embedded_repo = 1;
	}

	strbuf_release(&name);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ add_files(ഘടന repository *repo, ഘടന dir_struct *dir, പൂർണ്ണസംഖ്യ flags)
{
	പൂർണ്ണസംഖ്യ i, exit_status = 0;
	ഘടന string_list matched_sparse_paths = STRING_LIST_INIT_NODUP;

	എങ്കിൽ (dir->ignored_nr) {
		fprintf(stderr, _(ignore_error));
		വേണ്ടി (i = 0; i < dir->ignored_nr; i++)
			fprintf(stderr, "%s\n", dir->ignored[i]->name);
		advise_if_enabled(ADVICE_ADD_IGNORED_FILE,
				  _("Use -f if you really want to add them."));
		exit_status = 1;
	}

	വേണ്ടി (i = 0; i < dir->nr; i++) {
		എങ്കിൽ (!include_sparse &&
		    !path_in_sparse_checkout(dir->entries[i]->name, repo->index)) {
			string_list_append(&matched_sparse_paths,
					   dir->entries[i]->name);
			തുടരുക;
		}
		എങ്കിൽ (add_file_to_index(repo->index, dir->entries[i]->name, flags)) {
			എങ്കിൽ (!ignore_add_errors)
				die(_("adding files failed"));
			exit_status = 1;
		} അല്ലെങ്കിൽ {
			check_embedded_repo(dir->entries[i]->name);
		}
	}

	എങ്കിൽ (matched_sparse_paths.nr) {
		advise_on_updating_sparse_paths(&matched_sparse_paths);
		exit_status = 1;
	}

	string_list_clear(&matched_sparse_paths, 0);

	തിരികെ exit_status;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ failed_to_add(പൂർണ്ണസംഖ്യ flags, സ്ഥിരം അക്ഷരം *path)
{
	എങ്കിൽ (!(flags & ADD_CACHE_IGNORE_ERRORS))
		die(_("updating file '%s' failed"), path);
	തിരികെ 1;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ add_resolved_files(ഘടന repository *repo,
			      സ്ഥിരം ഘടന pathspec *pathspec,
			      പൂർണ്ണസംഖ്യ flags)
{
	ഘടന index_state *istate = repo->index;
	ഘടന string_list unmerged_paths = STRING_LIST_INIT_DUP;
	ഘടന string_list unresolved_paths = STRING_LIST_INIT_DUP;
	പൂർണ്ണസംഖ്യ exit_status = 0;
	size_t i;

	വേണ്ടി (i = 0; i < istate->cache_nr; i++) {
		ഘടന cache_entry *ce = istate->cache[i];
		എങ്കിൽ (!ce_stage(ce))
			തുടരുക;
		എങ്കിൽ (pathspec->nr && !ce_path_match(istate, ce, pathspec, ശൂന്യ))
			തുടരുക;
		എങ്കിൽ (!unmerged_paths.nr ||
		    strcmp(unmerged_paths.ഇനങ്ങൾ[unmerged_paths.nr - 1].string, ce->name))
			string_list_append(&unmerged_paths, ce->name);
	}

	എങ്കിൽ (!unmerged_paths.nr) {
		string_list_clear(&unmerged_paths, 0);
		തിരികെ 0;
	}

	വേണ്ടി (i = 0; i < unmerged_paths.nr; i++) {
		സ്ഥിരം അക്ഷരം *path = unmerged_paths.ഇനങ്ങൾ[i].string;
		ഘടന stat st;

		എങ്കിൽ (!lstat(path, &st) && S_ISREG(st.st_mode)) {
			എങ്കിൽ (has_conflict_markers(istate, path))
				string_list_append(&unresolved_paths, path);
		}
	}

	എങ്കിൽ (unresolved_paths.nr) {
		ഘടന strbuf sb = STRBUF_INIT;
		വേണ്ടി (i = 0; i < unresolved_paths.nr; i++)
			strbuf_addf(&sb, "\t%s\n", unresolved_paths.ഇനങ്ങൾ[i].string);
		die(_("the following paths still have conflict markers:\n%s"), sb.buf);
	}

	വേണ്ടി (i = 0; i < unmerged_paths.nr; i++) {
		സ്ഥിരം അക്ഷരം *path = unmerged_paths.ഇനങ്ങൾ[i].string;
		ഘടന stat st;

		എങ്കിൽ (lstat(path, &st)) {
			എങ്കിൽ (errno != ENOENT)
				die_errno(_("cannot lstat: '%s'"), path);
			എങ്കിൽ (remove_file_from_index_with_flags(istate, path, flags))
				exit_status = failed_to_add(flags, path);
		} അല്ലെങ്കിൽ {
			എങ്കിൽ (add_file_to_index(istate, path, flags))
				exit_status = failed_to_add(flags, path);
		}
	}

	string_list_clear(&unmerged_paths, 0);
	string_list_clear(&unresolved_paths, 0);
	തിരികെ exit_status;
}

പൂർണ്ണസംഖ്യ cmd_add(പൂർണ്ണസംഖ്യ argc,
	    സ്ഥിരം അക്ഷരം **argv,
	    സ്ഥിരം അക്ഷരം *prefix,
	    ഘടന repository *repo)
{
	പൂർണ്ണസംഖ്യ exit_status = 0;
	ഘടന pathspec pathspec;
	ഘടന dir_struct dir = DIR_INIT;
	പൂർണ്ണസംഖ്യ flags;
	പൂർണ്ണസംഖ്യ add_new_files;
	പൂർണ്ണസംഖ്യ require_pathspec;
	അക്ഷരം *seen = ശൂന്യ;
	അക്ഷരം *ps_matched = ശൂന്യ;
	ഘടന lock_file lock_file = LOCK_INIT;
	ഘടന odb_transaction *transaction = ശൂന്യ;

	repo_config(repo, add_config, ശൂന്യ);

	argc = parse_options(argc, argv, prefix, builtin_add_options,
			  builtin_add_usage, PARSE_OPT_KEEP_ARGV0);

	prepare_repo_settings(repo);
	repo->settings.command_requires_full_index = 0;

	എങ്കിൽ (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	എങ്കിൽ (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	എങ്കിൽ (patch_interactive)
		add_interactive = 1;
	എങ്കിൽ (add_interactive) {
		എങ്കിൽ (show_only)
			die(_("options '%s' and '%s' cannot be used together"), "--dry-run", "--interactive/--patch");
		എങ്കിൽ (pathspec_from_file)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--interactive/--patch");
		exit(interactive_add(repo, argv + 1, prefix, patch_interactive, &interactive_opts));
	} അല്ലെങ്കിൽ {
		എങ്കിൽ (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--interactive/--patch");
		എങ്കിൽ (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--interactive/--patch");
		എങ്കിൽ (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--interactive/--patch");
	}

	എങ്കിൽ (edit_interactive) {
		എങ്കിൽ (pathspec_from_file)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--edit");
		തിരികെ(edit_patch(repo, argc, argv, prefix));
	}
	argc--;
	argv++;

	എങ്കിൽ (0 <= addremove_explicit)
		addremove = addremove_explicit;
	അല്ലെങ്കിൽ എങ്കിൽ (take_worktree_changes && ADDREMOVE_DEFAULT)
		addremove = 0; /* "-u" was given but not "-A" */

	die_for_incompatible_opt3(take_worktree_changes, "-u/--update",
				  0 < addremove_explicit, "-A/--all",
				  add_resolved, "--resolved");

	എങ്കിൽ (!show_only && ignore_missing)
		die(_("the option '%s' requires '%s'"), "--ignore-missing", "--dry-run");

	എങ്കിൽ (chmod_arg && ((chmod_arg[0] != '-' && chmod_arg[0] != '+') ||
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

	എങ്കിൽ (pathspec_from_file) {
		എങ്കിൽ (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_FULL |
				    PATHSPEC_SYMLINK_LEADING_PATH,
				    prefix, pathspec_from_file, pathspec_file_nul);
	} അല്ലെങ്കിൽ എങ്കിൽ (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	എങ്കിൽ (require_pathspec && pathspec.nr == 0) {
		fprintf(stderr, _("Nothing specified, nothing added.\n"));
		advise_if_enabled(ADVICE_ADD_EMPTY_PATHSPEC,
				  _("Maybe you wanted to say 'git add .'?"));
		തിരികെ 0;
	}

	എങ്കിൽ (!take_worktree_changes && !add_resolved &&
	    addremove_explicit < 0 && pathspec.nr)
		/* Turn "git add pathspec..." to "git add -A pathspec..." */
		addremove = 1;

	flags = ((verbose ? ADD_CACHE_VERBOSE : 0) |
		 (show_only ? ADD_CACHE_PRETEND : 0) |
		 (intent_to_add ? ADD_CACHE_INTENT : 0) |
		 (ignore_add_errors ? ADD_CACHE_IGNORE_ERRORS : 0) |
		 (!(addremove || take_worktree_changes)
		  ? ADD_CACHE_IGNORE_REMOVAL : 0));

	എങ്കിൽ (repo_read_index_preload(repo, &pathspec, 0) < 0)
		die(_("index file corrupt"));

	die_in_unpopulated_submodule(repo->index, prefix);
	die_path_inside_submodule(repo->index, &pathspec);

	എങ്കിൽ (add_new_files) {
		പൂർണ്ണസംഖ്യ baselen;

		/* Set up the default git porcelain excludes */
		എങ്കിൽ (!ignored_too) {
			dir.flags |= DIR_COLLECT_IGNORED;
			setup_standard_excludes(&dir);
		}

		/* This picks up the paths that are not tracked */
		baselen = fill_directory(&dir, repo->index, &pathspec);
		എങ്കിൽ (pathspec.nr)
			seen = prune_directory(repo, &dir, &pathspec, baselen);
	}

	എങ്കിൽ (refresh_only) {
		exit_status |= refresh(repo, verbose, &pathspec);
		പോകുക finish;
	}

	എങ്കിൽ (pathspec.nr) {
		പൂർണ്ണസംഖ്യ i;
		അക്ഷരം *skip_worktree_seen = ശൂന്യ;
		ഘടന string_list only_match_skip_worktree = STRING_LIST_INIT_NODUP;

		എങ്കിൽ (!seen)
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

		വേണ്ടി (i = 0; i < pathspec.nr; i++) {
			സ്ഥിരം അക്ഷരം *path = pathspec.ഇനങ്ങൾ[i].match;

			എങ്കിൽ (pathspec.ഇനങ്ങൾ[i].magic & PATHSPEC_EXCLUDE)
				തുടരുക;
			എങ്കിൽ (seen[i])
				തുടരുക;

			എങ്കിൽ (!include_sparse &&
			    matches_skip_worktree(&pathspec, i, &skip_worktree_seen)) {
				string_list_append(&only_match_skip_worktree,
						   pathspec.ഇനങ്ങൾ[i].original);
				തുടരുക;
			}

			/* Don't complain at 'git add .' on empty repo */
			എങ്കിൽ (!path[0])
				തുടരുക;

			എങ്കിൽ ((pathspec.ഇനങ്ങൾ[i].magic & (PATHSPEC_GLOB | PATHSPEC_ICASE)) ||
			    !file_exists(path)) {
				എങ്കിൽ (ignore_missing) {
					പൂർണ്ണസംഖ്യ dtype = DT_UNKNOWN;
					എങ്കിൽ (is_excluded(&dir, repo->index, path, &dtype))
						dir_add_ignored(&dir, repo->index,
								path, pathspec.ഇനങ്ങൾ[i].len);
				} അല്ലെങ്കിൽ
					die(_("pathspec '%s' did not match any files"),
					    pathspec.ഇനങ്ങൾ[i].original);
			}
		}


		എങ്കിൽ (only_match_skip_worktree.nr) {
			advise_on_updating_sparse_paths(&only_match_skip_worktree);
			exit_status = 1;
		}

		സ്വതന്ത്രം(seen);
		സ്വതന്ത്രം(skip_worktree_seen);
		string_list_clear(&only_match_skip_worktree, 0);
	}

	odb_transaction_begin_or_die(repo->objects, &transaction, 0);

	ps_matched = xcalloc(pathspec.nr, 1);
	എങ്കിൽ (add_resolved)
		exit_status |= add_resolved_files(repo, &pathspec, flags);
	അല്ലെങ്കിൽ എങ്കിൽ (add_renormalize)
		exit_status |= renormalize_tracked_files(repo, &pathspec, flags);
	അല്ലെങ്കിൽ
		exit_status |= add_files_to_cache(repo, prefix,
						  &pathspec, ps_matched,
						  include_sparse, flags, ignored_too);

	എങ്കിൽ (take_worktree_changes && !add_renormalize && !ignore_add_errors &&
	    report_path_error(ps_matched, &pathspec))
		exit(128);

	എങ്കിൽ (add_new_files)
		exit_status |= add_files(repo, &dir, flags);

	എങ്കിൽ (chmod_arg && pathspec.nr)
		exit_status |= chmod_pathspec(repo, &pathspec, chmod_arg[0], show_only);
	odb_transaction_commit_and_finalize_or_die(transaction);

finish:
	എങ്കിൽ (write_locked_index(repo->index, &lock_file,
			       COMMIT_LOCK | SKIP_IF_UNCHANGED))
		die(_("unable to write new index file"));

	സ്വതന്ത്രം(ps_matched);
	dir_clear(&dir);
	clear_pathspec(&pathspec);
	തിരികെ exit_status;
}
