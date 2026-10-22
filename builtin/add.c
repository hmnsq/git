/*
 * "git add" builtin command
 *
 * Copyright (C) 2006 Linus Torvalds
 */

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "advice.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "lockfile.h"
#ಅನ್ತರ್ಭಾವ "editor.h"
#ಅನ್ತರ್ಭಾವ "dir.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "pathspec.h"
#ಅನ್ತರ್ಭಾವ "object-file.h"
#ಅನ್ತರ್ಭಾವ "odb.h"
#ಅನ್ತರ್ಭಾವ "odb/transaction.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "path.h"
#ಅನ್ತರ್ಭಾವ "preload-index.h"
#ಅನ್ತರ್ಭಾವ "diff.h"
#ಅನ್ತರ್ಭಾವ "read-cache.h"
#ಅನ್ತರ್ಭಾವ "revision.h"
#ಅನ್ತರ್ಭಾವ "submodule.h"
#ಅನ್ತರ್ಭಾವ "add-interactive.h"
#ಅನ್ತರ್ಭಾವ "merge-ll.h"
#ಅನ್ತರ್ಭಾವ "apply.h"

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ builtin_add_usage[] = {
	N_("git add [<options>] [--] <pathspec>..."),
	ರಿಕ್ತ
};
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ patch_interactive, add_interactive, edit_interactive;
ಸ್ಥಿರ ಸಂರಚನಾ interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ take_worktree_changes;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ add_renormalize;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ add_resolved;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ pathspec_file_nul;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ include_sparse;
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *pathspec_from_file;

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ chmod_pathspec(ಸಂರಚನಾ repository *repo,
			  ಸಂರಚನಾ pathspec *pathspec,
			  ವರ್ಣ flip,
			  ಪೂರ್ಣಾಙ್ಕ show_only)
{
	ಪೂರ್ಣಾಙ್ಕ ret = 0;

	ಪ್ರತಿ (size_t i = 0; i < repo->index->cache_nr; i++) {
		ಸಂರಚನಾ cache_entry *ce = repo->index->cache[i];
		ಪೂರ್ಣಾಙ್ಕ err;

		ಯದಿ (!include_sparse &&
		    (ce_skip_worktree(ce) ||
		     !path_in_sparse_checkout(ce->name, repo->index)))
			ಅನುವರ್ತ;

		ಯದಿ (pathspec && !ce_path_match(repo->index, ce, pathspec, ರಿಕ್ತ))
			ಅನುವರ್ತ;

		ಯದಿ (!show_only)
			err = chmod_index_entry(repo->index, ce, flip);
		ಅನ್ಯಥಾ
			err = S_ISREG(ce->ce_mode) ? 0 : -1;

		ಯದಿ (err < 0)
			ret = error(_("cannot chmod %cx '%s'"), flip, ce->name);
	}

	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ renormalize_tracked_files(ಸಂರಚನಾ repository *repo,
				     ಧ್ರುವ ಸಂರಚನಾ pathspec *pathspec,
				     ಪೂರ್ಣಾಙ್ಕ flags)
{
	ಪೂರ್ಣಾಙ್ಕ retval = 0;

	ಪ್ರತಿ (size_t i = 0; i < repo->index->cache_nr; i++) {
		ಸಂರಚನಾ cache_entry *ce = repo->index->cache[i];

		ಯದಿ (!include_sparse &&
		    (ce_skip_worktree(ce) ||
		     !path_in_sparse_checkout(ce->name, repo->index)))
			ಅನುವರ್ತ;
		ಯದಿ (ce_stage(ce))
			ಅನುವರ್ತ; /* do not touch unmerged paths */
		ಯದಿ (!S_ISREG(ce->ce_mode) && !S_ISLNK(ce->ce_mode))
			ಅನುವರ್ತ; /* do not touch non blobs */
		ಯದಿ (pathspec && !ce_path_match(repo->index, ce, pathspec, ರಿಕ್ತ))
			ಅನುವರ್ತ;
		retval |= add_file_to_index(repo->index, ce->name,
					    flags | ADD_CACHE_RENORMALIZE);
	}

	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸ್ಥಿರ ವರ್ಣ *prune_directory(ಸಂರಚನಾ repository *repo,
			     ಸಂರಚನಾ dir_struct *dir,
			     ಸಂರಚನಾ pathspec *pathspec,
			     ಪೂರ್ಣಾಙ್ಕ prefix)
{
	ವರ್ಣ *seen;
	ಪೂರ್ಣಾಙ್ಕ i;
	ಸಂರಚನಾ dir_entry **src, **dst;

	seen = xcalloc(pathspec->nr, 1);

	src = dst = dir->entries;
	i = dir->nr;
	ಯಾವತ್ (--i >= 0) {
		ಸಂರಚನಾ dir_entry *entry = *src++;
		ಯದಿ (dir_path_match(repo->index, entry, pathspec, prefix, seen))
			*dst++ = entry;
	}
	dir->nr = dst - dir->entries;
	add_pathspec_matches_against_index(pathspec, repo->index, seen,
					   PS_IGNORE_SKIP_WORKTREE);
	ಪ್ರತ್ಯಾಗಮ seen;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ refresh(ಸಂರಚನಾ repository *repo, ಪೂರ್ಣಾಙ್ಕ verbose, ಧ್ರುವ ಸಂರಚನಾ pathspec *pathspec)
{
	ವರ್ಣ *seen;
	ಪೂರ್ಣಾಙ್ಕ i, ret = 0;
	ವರ್ಣ *skip_worktree_seen = ರಿಕ್ತ;
	ಸಂರಚನಾ string_list only_match_skip_worktree = STRING_LIST_INIT_NODUP;
	unsigned ಪೂರ್ಣಾಙ್ಕ flags = REFRESH_IGNORE_SKIP_WORKTREE |
		    (verbose ? REFRESH_IN_PORCELAIN : REFRESH_QUIET);

	seen = xcalloc(pathspec->nr, 1);
	refresh_index(repo->index, flags, pathspec, seen,
		      _("Unstaged changes after refreshing the index:"));
	ಪ್ರತಿ (i = 0; i < pathspec->nr; i++) {
		ಯದಿ (!seen[i]) {
			ಧ್ರುವ ವರ್ಣ *path = pathspec->ವಸ್ತುಗಳು[i].original;

			ಯದಿ (matches_skip_worktree(pathspec, i, &skip_worktree_seen) ||
			    !path_in_sparse_checkout(path, repo->index)) {
				string_list_append(&only_match_skip_worktree,
						   pathspec->ವಸ್ತುಗಳು[i].original);
			} ಅನ್ಯಥಾ {
				die(_("pathspec '%s' did not match any files"),
				    pathspec->ವಸ್ತುಗಳು[i].original);
			}
		}
	}

	ಯದಿ (only_match_skip_worktree.nr) {
		advise_on_updating_sparse_paths(&only_match_skip_worktree);
		ret = 1;
	}

	ಮುಕ್ತ(seen);
	ಮುಕ್ತ(skip_worktree_seen);
	string_list_clear(&only_match_skip_worktree, 0);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಪೂರ್ಣಾಙ್ಕ interactive_add(ಸಂರಚನಾ repository *repo,
		    ಧ್ರುವ ವರ್ಣ **argv,
		    ಧ್ರುವ ವರ್ಣ *prefix,
		    ಪೂರ್ಣಾಙ್ಕ patch, ಸಂರಚನಾ interactive_options *interactive_opts)
{
	ಸಂರಚನಾ pathspec pathspec;
	ಪೂರ್ಣಾಙ್ಕ ret;

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_FULL |
		       PATHSPEC_SYMLINK_LEADING_PATH |
		       PATHSPEC_PREFIX_ORIGIN,
		       prefix, argv);

	ಯದಿ (patch)
		ret = !!run_add_p(repo, ADD_P_ADD, interactive_opts, ರಿಕ್ತ, &pathspec, 0);
	ಅನ್ಯಥಾ
		ret = !!run_add_i(repo, &pathspec, interactive_opts);

	clear_pathspec(&pathspec);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ edit_patch(ಸಂರಚನಾ repository *repo,
		      ಪೂರ್ಣಾಙ್ಕ argc,
		      ಧ್ರುವ ವರ್ಣ **argv,
		      ಧ್ರುವ ವರ್ಣ *prefix)
{
	ವರ್ಣ *file = repo_git_path(repo, "ADD_EDIT.patch");
	ಸಂರಚನಾ apply_state state;
	ಧ್ರುವ ವರ್ಣ *apply_argv[2];
	ಸಂರಚನಾ rev_info rev;
	ಪೂರ್ಣಾಙ್ಕ out;
	ಸಂರಚನಾ stat st;

	repo_config(repo, git_diff_basic_config, ರಿಕ್ತ);

	ಯದಿ (repo_read_index(repo) < 0)
		die(_("could not read the index"));

	repo_init_revisions(repo, &rev, prefix);
	rev.diffopt.context = 7;

	argc = setup_revisions(argc, argv, &rev, ರಿಕ್ತ);
	rev.diffopt.output_format = DIFF_FORMAT_PATCH;
	rev.diffopt.use_color = GIT_COLOR_NEVER;
	rev.diffopt.flags.ignore_dirty_submodules = 1;
	out = xopen(file, O_CREAT | O_WRONLY | O_TRUNC, 0666);
	rev.diffopt.file = xfdopen(out, "w");
	rev.diffopt.close_file = 1;
	run_diff_files(&rev, 0);

	ಯದಿ (launch_editor(file, ರಿಕ್ತ, ರಿಕ್ತ))
		die(_("editing patch failed"));

	ಯದಿ (stat(file, &st))
		die_errno(_("could not stat '%s'"), file);
	ಯದಿ (!st.st_size)
		die(_("empty patch. aborted"));

	apply_argv[0] = file;
	apply_argv[1] = ರಿಕ್ತ;
	ಯದಿ (init_apply_state(&state, repo, ರಿಕ್ತ))
		die(_("could not initialize apply state"));
	state.cached = 1;
	ಯದಿ (check_apply_state(&state, 0))
		die(_("could not check apply state"));
	ಯದಿ (apply_all_patches(&state, 1, apply_argv, APPLY_OPT_RECOUNT))
		die(_("could not apply '%s'"), file);
	clear_apply_state(&state);

	unlink(file);
	ಮುಕ್ತ(file);
	release_revisions(&rev);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ ignore_error[] =
N_("The following paths are ignored by one of your .gitignore files:\n");

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ verbose, show_only, ignored_too, refresh_only;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ ignore_add_errors, intent_to_add, ignore_missing;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ warn_on_embedded_repo = 1;

#define ADDREMOVE_DEFAULT 1
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ addremove = ADDREMOVE_DEFAULT;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ addremove_explicit = -1; /* unspecified */

ಸ್ಥಿರ ವರ್ಣ *chmod_arg;

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ ignore_removal_cb(ಧ್ರುವ ಸಂರಚನಾ option *opt, ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	BUG_ON_OPT_ARG(arg);

	/* if we are told to ignore, we are not adding removals */
	*(ಪೂರ್ಣಾಙ್ಕ *)opt->ಮೌಲ್ಯ = !unset ? 0 : 1;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಸಂರಚನಾ option builtin_add_options[] = {
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
	  ರಿಕ್ತ /* takes no arguments */,
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

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ add_config(ಧ್ರುವ ವರ್ಣ *var, ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ,
		      ಧ್ರುವ ಸಂರಚನಾ config_context *ctx, ಶೂನ್ಯ *cb)
{
	ಯದಿ (!strcmp(var, "add.ignoreerrors") ||
	    !strcmp(var, "add.ignore-errors")) {
		ignore_add_errors = git_config_bool(var, ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (git_color_config(var, ಮೌಲ್ಯ, cb) < 0)
		ಪ್ರತ್ಯಾಗಮ -1;

	ಪ್ರತ್ಯಾಗಮ git_default_config(var, ಮೌಲ್ಯ, ctx, cb);
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ embedded_advice[] = N_(
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

ಸ್ಥಿರ ಶೂನ್ಯ check_embedded_repo(ಧ್ರುವ ವರ್ಣ *path)
{
	ಸಂರಚನಾ strbuf name = STRBUF_INIT;
	ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ adviced_on_embedded_repo = 0;

	ಯದಿ (!warn_on_embedded_repo)
		ಪ್ರತ್ಯಾಗಮ;
	ಯದಿ (!ends_with(path, "/"))
		ಪ್ರತ್ಯಾಗಮ;

	/* Drop trailing slash for aesthetics */
	strbuf_addstr(&name, path);
	strbuf_strip_suffix(&name, "/");

	warning(_("adding embedded git repository: %s"), name.buf);
	ಯದಿ (!adviced_on_embedded_repo) {
		advise_if_enabled(ADVICE_ADD_EMBEDDED_REPO,
				  embedded_advice, name.buf, name.buf);
		adviced_on_embedded_repo = 1;
	}

	strbuf_release(&name);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ add_files(ಸಂರಚನಾ repository *repo, ಸಂರಚನಾ dir_struct *dir, ಪೂರ್ಣಾಙ್ಕ flags)
{
	ಪೂರ್ಣಾಙ್ಕ i, exit_status = 0;
	ಸಂರಚನಾ string_list matched_sparse_paths = STRING_LIST_INIT_NODUP;

	ಯದಿ (dir->ignored_nr) {
		fprintf(stderr, _(ignore_error));
		ಪ್ರತಿ (i = 0; i < dir->ignored_nr; i++)
			fprintf(stderr, "%s\n", dir->ignored[i]->name);
		advise_if_enabled(ADVICE_ADD_IGNORED_FILE,
				  _("Use -f if you really want to add them."));
		exit_status = 1;
	}

	ಪ್ರತಿ (i = 0; i < dir->nr; i++) {
		ಯದಿ (!include_sparse &&
		    !path_in_sparse_checkout(dir->entries[i]->name, repo->index)) {
			string_list_append(&matched_sparse_paths,
					   dir->entries[i]->name);
			ಅನುವರ್ತ;
		}
		ಯದಿ (add_file_to_index(repo->index, dir->entries[i]->name, flags)) {
			ಯದಿ (!ignore_add_errors)
				die(_("adding files failed"));
			exit_status = 1;
		} ಅನ್ಯಥಾ {
			check_embedded_repo(dir->entries[i]->name);
		}
	}

	ಯದಿ (matched_sparse_paths.nr) {
		advise_on_updating_sparse_paths(&matched_sparse_paths);
		exit_status = 1;
	}

	string_list_clear(&matched_sparse_paths, 0);

	ಪ್ರತ್ಯಾಗಮ exit_status;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ failed_to_add(ಪೂರ್ಣಾಙ್ಕ flags, ಧ್ರುವ ವರ್ಣ *path)
{
	ಯದಿ (!(flags & ADD_CACHE_IGNORE_ERRORS))
		die(_("updating file '%s' failed"), path);
	ಪ್ರತ್ಯಾಗಮ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ add_resolved_files(ಸಂರಚನಾ repository *repo,
			      ಧ್ರುವ ಸಂರಚನಾ pathspec *pathspec,
			      ಪೂರ್ಣಾಙ್ಕ flags)
{
	ಸಂರಚನಾ index_state *istate = repo->index;
	ಸಂರಚನಾ string_list unmerged_paths = STRING_LIST_INIT_DUP;
	ಸಂರಚನಾ string_list unresolved_paths = STRING_LIST_INIT_DUP;
	ಪೂರ್ಣಾಙ್ಕ exit_status = 0;
	size_t i;

	ಪ್ರತಿ (i = 0; i < istate->cache_nr; i++) {
		ಸಂರಚನಾ cache_entry *ce = istate->cache[i];
		ಯದಿ (!ce_stage(ce))
			ಅನುವರ್ತ;
		ಯದಿ (pathspec->nr && !ce_path_match(istate, ce, pathspec, ರಿಕ್ತ))
			ಅನುವರ್ತ;
		ಯದಿ (!unmerged_paths.nr ||
		    strcmp(unmerged_paths.ವಸ್ತುಗಳು[unmerged_paths.nr - 1].string, ce->name))
			string_list_append(&unmerged_paths, ce->name);
	}

	ಯದಿ (!unmerged_paths.nr) {
		string_list_clear(&unmerged_paths, 0);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಪ್ರತಿ (i = 0; i < unmerged_paths.nr; i++) {
		ಧ್ರುವ ವರ್ಣ *path = unmerged_paths.ವಸ್ತುಗಳು[i].string;
		ಸಂರಚನಾ stat st;

		ಯದಿ (!lstat(path, &st) && S_ISREG(st.st_mode)) {
			ಯದಿ (has_conflict_markers(istate, path))
				string_list_append(&unresolved_paths, path);
		}
	}

	ಯದಿ (unresolved_paths.nr) {
		ಸಂರಚನಾ strbuf sb = STRBUF_INIT;
		ಪ್ರತಿ (i = 0; i < unresolved_paths.nr; i++)
			strbuf_addf(&sb, "\t%s\n", unresolved_paths.ವಸ್ತುಗಳು[i].string);
		die(_("the following paths still have conflict markers:\n%s"), sb.buf);
	}

	ಪ್ರತಿ (i = 0; i < unmerged_paths.nr; i++) {
		ಧ್ರುವ ವರ್ಣ *path = unmerged_paths.ವಸ್ತುಗಳು[i].string;
		ಸಂರಚನಾ stat st;

		ಯದಿ (lstat(path, &st)) {
			ಯದಿ (errno != ENOENT)
				die_errno(_("cannot lstat: '%s'"), path);
			ಯದಿ (remove_file_from_index_with_flags(istate, path, flags))
				exit_status = failed_to_add(flags, path);
		} ಅನ್ಯಥಾ {
			ಯದಿ (add_file_to_index(istate, path, flags))
				exit_status = failed_to_add(flags, path);
		}
	}

	string_list_clear(&unmerged_paths, 0);
	string_list_clear(&unresolved_paths, 0);
	ಪ್ರತ್ಯಾಗಮ exit_status;
}

ಪೂರ್ಣಾಙ್ಕ cmd_add(ಪೂರ್ಣಾಙ್ಕ argc,
	    ಧ್ರುವ ವರ್ಣ **argv,
	    ಧ್ರುವ ವರ್ಣ *prefix,
	    ಸಂರಚನಾ repository *repo)
{
	ಪೂರ್ಣಾಙ್ಕ exit_status = 0;
	ಸಂರಚನಾ pathspec pathspec;
	ಸಂರಚನಾ dir_struct dir = DIR_INIT;
	ಪೂರ್ಣಾಙ್ಕ flags;
	ಪೂರ್ಣಾಙ್ಕ add_new_files;
	ಪೂರ್ಣಾಙ್ಕ require_pathspec;
	ವರ್ಣ *seen = ರಿಕ್ತ;
	ವರ್ಣ *ps_matched = ರಿಕ್ತ;
	ಸಂರಚನಾ lock_file lock_file = LOCK_INIT;
	ಸಂರಚನಾ odb_transaction *transaction = ರಿಕ್ತ;

	repo_config(repo, add_config, ರಿಕ್ತ);

	argc = parse_options(argc, argv, prefix, builtin_add_options,
			  builtin_add_usage, PARSE_OPT_KEEP_ARGV0);

	prepare_repo_settings(repo);
	repo->settings.command_requires_full_index = 0;

	ಯದಿ (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	ಯದಿ (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	ಯದಿ (patch_interactive)
		add_interactive = 1;
	ಯದಿ (add_interactive) {
		ಯದಿ (show_only)
			die(_("options '%s' and '%s' cannot be used together"), "--dry-run", "--interactive/--patch");
		ಯದಿ (pathspec_from_file)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--interactive/--patch");
		exit(interactive_add(repo, argv + 1, prefix, patch_interactive, &interactive_opts));
	} ಅನ್ಯಥಾ {
		ಯದಿ (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--interactive/--patch");
		ಯದಿ (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--interactive/--patch");
		ಯದಿ (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--interactive/--patch");
	}

	ಯದಿ (edit_interactive) {
		ಯದಿ (pathspec_from_file)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--edit");
		ಪ್ರತ್ಯಾಗಮ(edit_patch(repo, argc, argv, prefix));
	}
	argc--;
	argv++;

	ಯದಿ (0 <= addremove_explicit)
		addremove = addremove_explicit;
	ಅನ್ಯಥಾ ಯದಿ (take_worktree_changes && ADDREMOVE_DEFAULT)
		addremove = 0; /* "-u" was given but not "-A" */

	die_for_incompatible_opt3(take_worktree_changes, "-u/--update",
				  0 < addremove_explicit, "-A/--all",
				  add_resolved, "--resolved");

	ಯದಿ (!show_only && ignore_missing)
		die(_("the option '%s' requires '%s'"), "--ignore-missing", "--dry-run");

	ಯದಿ (chmod_arg && ((chmod_arg[0] != '-' && chmod_arg[0] != '+') ||
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

	ಯದಿ (pathspec_from_file) {
		ಯದಿ (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_FULL |
				    PATHSPEC_SYMLINK_LEADING_PATH,
				    prefix, pathspec_from_file, pathspec_file_nul);
	} ಅನ್ಯಥಾ ಯದಿ (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	ಯದಿ (require_pathspec && pathspec.nr == 0) {
		fprintf(stderr, _("Nothing specified, nothing added.\n"));
		advise_if_enabled(ADVICE_ADD_EMPTY_PATHSPEC,
				  _("Maybe you wanted to say 'git add .'?"));
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (!take_worktree_changes && !add_resolved &&
	    addremove_explicit < 0 && pathspec.nr)
		/* Turn "git add pathspec..." to "git add -A pathspec..." */
		addremove = 1;

	flags = ((verbose ? ADD_CACHE_VERBOSE : 0) |
		 (show_only ? ADD_CACHE_PRETEND : 0) |
		 (intent_to_add ? ADD_CACHE_INTENT : 0) |
		 (ignore_add_errors ? ADD_CACHE_IGNORE_ERRORS : 0) |
		 (!(addremove || take_worktree_changes)
		  ? ADD_CACHE_IGNORE_REMOVAL : 0));

	ಯದಿ (repo_read_index_preload(repo, &pathspec, 0) < 0)
		die(_("index file corrupt"));

	die_in_unpopulated_submodule(repo->index, prefix);
	die_path_inside_submodule(repo->index, &pathspec);

	ಯದಿ (add_new_files) {
		ಪೂರ್ಣಾಙ್ಕ baselen;

		/* Set up the default git porcelain excludes */
		ಯದಿ (!ignored_too) {
			dir.flags |= DIR_COLLECT_IGNORED;
			setup_standard_excludes(&dir);
		}

		/* This picks up the paths that are not tracked */
		baselen = fill_directory(&dir, repo->index, &pathspec);
		ಯದಿ (pathspec.nr)
			seen = prune_directory(repo, &dir, &pathspec, baselen);
	}

	ಯದಿ (refresh_only) {
		exit_status |= refresh(repo, verbose, &pathspec);
		ಗಚ್ಛ finish;
	}

	ಯದಿ (pathspec.nr) {
		ಪೂರ್ಣಾಙ್ಕ i;
		ವರ್ಣ *skip_worktree_seen = ರಿಕ್ತ;
		ಸಂರಚನಾ string_list only_match_skip_worktree = STRING_LIST_INIT_NODUP;

		ಯದಿ (!seen)
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

		ಪ್ರತಿ (i = 0; i < pathspec.nr; i++) {
			ಧ್ರುವ ವರ್ಣ *path = pathspec.ವಸ್ತುಗಳು[i].match;

			ಯದಿ (pathspec.ವಸ್ತುಗಳು[i].magic & PATHSPEC_EXCLUDE)
				ಅನುವರ್ತ;
			ಯದಿ (seen[i])
				ಅನುವರ್ತ;

			ಯದಿ (!include_sparse &&
			    matches_skip_worktree(&pathspec, i, &skip_worktree_seen)) {
				string_list_append(&only_match_skip_worktree,
						   pathspec.ವಸ್ತುಗಳು[i].original);
				ಅನುವರ್ತ;
			}

			/* Don't complain at 'git add .' on empty repo */
			ಯದಿ (!path[0])
				ಅನುವರ್ತ;

			ಯದಿ ((pathspec.ವಸ್ತುಗಳು[i].magic & (PATHSPEC_GLOB | PATHSPEC_ICASE)) ||
			    !file_exists(path)) {
				ಯದಿ (ignore_missing) {
					ಪೂರ್ಣಾಙ್ಕ dtype = DT_UNKNOWN;
					ಯದಿ (is_excluded(&dir, repo->index, path, &dtype))
						dir_add_ignored(&dir, repo->index,
								path, pathspec.ವಸ್ತುಗಳು[i].len);
				} ಅನ್ಯಥಾ
					die(_("pathspec '%s' did not match any files"),
					    pathspec.ವಸ್ತುಗಳು[i].original);
			}
		}


		ಯದಿ (only_match_skip_worktree.nr) {
			advise_on_updating_sparse_paths(&only_match_skip_worktree);
			exit_status = 1;
		}

		ಮುಕ್ತ(seen);
		ಮುಕ್ತ(skip_worktree_seen);
		string_list_clear(&only_match_skip_worktree, 0);
	}

	odb_transaction_begin_or_die(repo->objects, &transaction, 0);

	ps_matched = xcalloc(pathspec.nr, 1);
	ಯದಿ (add_resolved)
		exit_status |= add_resolved_files(repo, &pathspec, flags);
	ಅನ್ಯಥಾ ಯದಿ (add_renormalize)
		exit_status |= renormalize_tracked_files(repo, &pathspec, flags);
	ಅನ್ಯಥಾ
		exit_status |= add_files_to_cache(repo, prefix,
						  &pathspec, ps_matched,
						  include_sparse, flags, ignored_too);

	ಯದಿ (take_worktree_changes && !add_renormalize && !ignore_add_errors &&
	    report_path_error(ps_matched, &pathspec))
		exit(128);

	ಯದಿ (add_new_files)
		exit_status |= add_files(repo, &dir, flags);

	ಯದಿ (chmod_arg && pathspec.nr)
		exit_status |= chmod_pathspec(repo, &pathspec, chmod_arg[0], show_only);
	odb_transaction_commit_and_finalize_or_die(transaction);

finish:
	ಯದಿ (write_locked_index(repo->index, &lock_file,
			       COMMIT_LOCK | SKIP_IF_UNCHANGED))
		die(_("unable to write new index file"));

	ಮುಕ್ತ(ps_matched);
	dir_clear(&dir);
	clear_pathspec(&pathspec);
	ಪ್ರತ್ಯಾಗಮ exit_status;
}
