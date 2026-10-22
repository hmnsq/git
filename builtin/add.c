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

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ builtin_ಕೂಡಿಸು_usage[] = {
	N_("git add [<options>] [--] <pathspec>..."),
	ಖಾಲಿ
};
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ patch_interactive, ಕೂಡಿಸು_interactive, edit_interactive;
ಸ್ಥಿರ ರಚನೆ interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ take_worktree_changes;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಕೂಡಿಸು_renormalize;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಕೂಡಿಸು_resolved;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ pathspec_file_nul;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ include_sparse;
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *pathspec_from_file;

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ chmod_pathspec(ರಚನೆ repository *repo,
			  ರಚನೆ pathspec *pathspec,
			  ಅಕ್ಷರ flip,
			  ಪೂರ್ಣಾಂಕ show_only)
{
	ಪೂರ್ಣಾಂಕ ret = 0;

	ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < repo->ಸೂಚ್ಯಂಕ->cache_nr; i++) {
		ರಚನೆ cache_ನಮೂದು *ce = repo->ಸೂಚ್ಯಂಕ->cache[i];
		ಪೂರ್ಣಾಂಕ err;

		ಆದರೆ (!include_sparse &&
		    (ce_skip_worktree(ce) ||
		     !ಮಾರ್ಗ_ಒಳ_sparse_checkout(ce->name, repo->ಸೂಚ್ಯಂಕ)))
			ಮುಂದುವರಿ;

		ಆದರೆ (pathspec && !ce_ಮಾರ್ಗ_ಹೊಂದಿಕೆ(repo->ಸೂಚ್ಯಂಕ, ce, pathspec, ಖಾಲಿ))
			ಮುಂದುವರಿ;

		ಆದರೆ (!show_only)
			err = chmod_ಸೂಚ್ಯಂಕ_ನಮೂದು(repo->ಸೂಚ್ಯಂಕ, ce, flip);
		ಇಲ್ಲದಿದ್ದರೆ
			err = S_ISREG(ce->ce_ಬಹುಲಕ) ? 0 : -1;

		ಆದರೆ (err < 0)
			ret = error(_("cannot chmod %cx '%s'"), flip, ce->name);
	}

	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ renormalize_tracked_files(ರಚನೆ repository *repo,
				     ಸ್ಥಿರಾಂಕ ರಚನೆ pathspec *pathspec,
				     ಪೂರ್ಣಾಂಕ flags)
{
	ಪೂರ್ಣಾಂಕ retval = 0;

	ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < repo->ಸೂಚ್ಯಂಕ->cache_nr; i++) {
		ರಚನೆ cache_ನಮೂದು *ce = repo->ಸೂಚ್ಯಂಕ->cache[i];

		ಆದರೆ (!include_sparse &&
		    (ce_skip_worktree(ce) ||
		     !ಮಾರ್ಗ_ಒಳ_sparse_checkout(ce->name, repo->ಸೂಚ್ಯಂಕ)))
			ಮುಂದುವರಿ;
		ಆದರೆ (ce_stage(ce))
			ಮುಂದುವರಿ; /* do not touch unmerged paths */
		ಆದರೆ (!S_ISREG(ce->ce_ಬಹುಲಕ) && !S_ISLNK(ce->ce_ಬಹುಲಕ))
			ಮುಂದುವರಿ; /* do not touch non blobs */
		ಆದರೆ (pathspec && !ce_ಮಾರ್ಗ_ಹೊಂದಿಕೆ(repo->ಸೂಚ್ಯಂಕ, ce, pathspec, ಖಾಲಿ))
			ಮುಂದುವರಿ;
		retval |= ಕೂಡಿಸು_file_ಗಮ್ಯ_ಸೂಚ್ಯಂಕ(repo->ಸೂಚ್ಯಂಕ, ce->name,
					    flags | ಕೂಡಿಸು_CACHE_RENORMALIZE);
	}

	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ ಅಕ್ಷರ *prune_directory(ರಚನೆ repository *repo,
			     ರಚನೆ dir_struct *dir,
			     ರಚನೆ pathspec *pathspec,
			     ಪೂರ್ಣಾಂಕ ಪೂರ್ವಪ್ರತ್ಯಯ)
{
	ಅಕ್ಷರ *ನೋಡಿದ;
	ಪೂರ್ಣಾಂಕ i;
	ರಚನೆ dir_ನಮೂದು **src, **dst;

	ನೋಡಿದ = xcalloc(pathspec->nr, 1);

	src = dst = dir->entries;
	i = dir->nr;
	ಆಗ (--i >= 0) {
		ರಚನೆ dir_ನಮೂದು *ನಮೂದು = *src++;
		ಆದರೆ (dir_ಮಾರ್ಗ_ಹೊಂದಿಕೆ(repo->ಸೂಚ್ಯಂಕ, ನಮೂದು, pathspec, ಪೂರ್ವಪ್ರತ್ಯಯ, ನೋಡಿದ))
			*dst++ = ನಮೂದು;
	}
	dir->nr = dst - dir->entries;
	ಕೂಡಿಸು_pathspec_matches_against_ಸೂಚ್ಯಂಕ(pathspec, repo->ಸೂಚ್ಯಂಕ, ನೋಡಿದ,
					   PS_IGNORE_SKIP_WORKTREE);
	ಹಿಂತಿರುಗಿ ನೋಡಿದ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ refresh(ರಚನೆ repository *repo, ಪೂರ್ಣಾಂಕ verbose, ಸ್ಥಿರಾಂಕ ರಚನೆ pathspec *pathspec)
{
	ಅಕ್ಷರ *ನೋಡಿದ;
	ಪೂರ್ಣಾಂಕ i, ret = 0;
	ಅಕ್ಷರ *skip_worktree_ನೋಡಿದ = ಖಾಲಿ;
	ರಚನೆ string_list only_ಹೊಂದಿಕೆ_skip_worktree = STRING_LIST_INIT_NODUP;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags = REFRESH_IGNORE_SKIP_WORKTREE |
		    (verbose ? REFRESH_ಒಳ_PORCELAIN : REFRESH_QUIET);

	ನೋಡಿದ = xcalloc(pathspec->nr, 1);
	refresh_ಸೂಚ್ಯಂಕ(repo->ಸೂಚ್ಯಂಕ, flags, pathspec, ನೋಡಿದ,
		      _("Unstaged changes after refreshing the index:"));
	ಗಾಗಿ (i = 0; i < pathspec->nr; i++) {
		ಆದರೆ (!ನೋಡಿದ[i]) {
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ = pathspec->ವಸ್ತುಗಳು[i].original;

			ಆದರೆ (matches_skip_worktree(pathspec, i, &skip_worktree_ನೋಡಿದ) ||
			    !ಮಾರ್ಗ_ಒಳ_sparse_checkout(ಮಾರ್ಗ, repo->ಸೂಚ್ಯಂಕ)) {
				string_list_append(&only_ಹೊಂದಿಕೆ_skip_worktree,
						   pathspec->ವಸ್ತುಗಳು[i].original);
			} ಇಲ್ಲದಿದ್ದರೆ {
				die(_("pathspec '%s' did not match any files"),
				    pathspec->ವಸ್ತುಗಳು[i].original);
			}
		}
	}

	ಆದರೆ (only_ಹೊಂದಿಕೆ_skip_worktree.nr) {
		advise_on_updating_sparse_paths(&only_ಹೊಂದಿಕೆ_skip_worktree);
		ret = 1;
	}

	ಬಿಡುಗಡೆ(ನೋಡಿದ);
	ಬಿಡುಗಡೆ(skip_worktree_ನೋಡಿದ);
	string_list_clear(&only_ಹೊಂದಿಕೆ_skip_worktree, 0);
	ಹಿಂತಿರುಗಿ ret;
}

ಪೂರ್ಣಾಂಕ interactive_ಕೂಡಿಸು(ರಚನೆ repository *repo,
		    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
		    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		    ಪೂರ್ಣಾಂಕ patch, ರಚನೆ interactive_options *interactive_opts)
{
	ರಚನೆ pathspec pathspec;
	ಪೂರ್ಣಾಂಕ ret;

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_ತುಂಬಿದ |
		       PATHSPEC_SYMLINK_LEADING_ಮಾರ್ಗ |
		       PATHSPEC_ಪೂರ್ವಪ್ರತ್ಯಯ_ORIGIN,
		       ಪೂರ್ವಪ್ರತ್ಯಯ, argv);

	ಆದರೆ (patch)
		ret = !!ಸತತ_ಕೂಡಿಸು_p(repo, ಕೂಡಿಸು_P_ಕೂಡಿಸು, interactive_opts, ಖಾಲಿ, &pathspec, 0);
	ಇಲ್ಲದಿದ್ದರೆ
		ret = !!ಸತತ_ಕೂಡಿಸು_i(repo, &pathspec, interactive_opts);

	clear_pathspec(&pathspec);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ edit_patch(ರಚನೆ repository *repo,
		      ಪೂರ್ಣಾಂಕ argc,
		      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
		      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ)
{
	ಅಕ್ಷರ *file = repo_git_ಮಾರ್ಗ(repo, "ADD_EDIT.patch");
	ರಚನೆ apply_state state;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *apply_argv[2];
	ರಚನೆ rev_info rev;
	ಪೂರ್ಣಾಂಕ ಹೊರಹೋಗು;
	ರಚನೆ stat st;

	repo_config(repo, git_diff_basic_config, ಖಾಲಿ);

	ಆದರೆ (repo_read_ಸೂಚ್ಯಂಕ(repo) < 0)
		die(_("could not read the index"));

	repo_init_revisions(repo, &rev, ಪೂರ್ವಪ್ರತ್ಯಯ);
	rev.diffopt.context = 7;

	argc = ಸಿದ್ಧತೆ_revisions(argc, argv, &rev, ಖಾಲಿ);
	rev.diffopt.output_format = DIFF_FORMAT_PATCH;
	rev.diffopt.use_color = GIT_COLOR_NEVER;
	rev.diffopt.flags.ignore_dirty_submodules = 1;
	ಹೊರಹೋಗು = xopen(file, O_CREAT | O_WRONLY | O_TRUNC, 0666);
	rev.diffopt.file = xfdopen(ಹೊರಹೋಗು, "w");
	rev.diffopt.close_file = 1;
	ಸತತ_diff_files(&rev, 0);

	ಆದರೆ (launch_editor(file, ಖಾಲಿ, ಖಾಲಿ))
		die(_("editing patch failed"));

	ಆದರೆ (stat(file, &st))
		die_errno(_("could not stat '%s'"), file);
	ಆದರೆ (!st.st_ಗಾತ್ರ)
		die(_("empty patch. aborted"));

	apply_argv[0] = file;
	apply_argv[1] = ಖಾಲಿ;
	ಆದರೆ (init_apply_state(&state, repo, ಖಾಲಿ))
		die(_("could not initialize apply state"));
	state.cached = 1;
	ಆದರೆ (check_apply_state(&state, 0))
		die(_("could not check apply state"));
	ಆದರೆ (apply_all_patches(&state, 1, apply_argv, APPLY_OPT_RECOUNT))
		die(_("could not apply '%s'"), file);
	clear_apply_state(&state);

	unlink(file);
	ಬಿಡುಗಡೆ(file);
	release_revisions(&rev);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ ignore_error[] =
N_("The following paths are ignored by one of your .gitignore files:\n");

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ verbose, show_only, ignored_too, refresh_only;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ignore_ಕೂಡಿಸು_errors, intent_ಗಮ್ಯ_ಕೂಡಿಸು, ignore_missing;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ warn_on_embedded_repo = 1;

#ವ್ಯಾಖ್ಯಾನಿಸು ADDREMOVE_DEFAULT 1
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ addremove = ADDREMOVE_DEFAULT;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ addremove_explicit = -1; /* unspecified */

ಸ್ಥಿರ ಅಕ್ಷರ *chmod_arg;

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ignore_removal_cb(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	BUG_ON_OPT_ARG(arg);

	/* if we are told to ignore, we are not adding removals */
	*(ಪೂರ್ಣಾಂಕ *)opt->ಮೌಲ್ಯ = !unset ? 0 : 1;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ರಚನೆ option builtin_ಕೂಡಿಸು_options[] = {
	OPT_DRY_ಸತತ(&show_only, N_("dry run")),
	OPT__VERBOSE(&verbose, N_("be verbose")),
	OPT_ಗುಂಪು(""),
	OPT_BOOL('i', "interactive", &ಕೂಡಿಸು_interactive, N_("interactive picking")),
	OPT_BOOL('p', "patch", &patch_interactive, N_("select hunks interactively")),
	OPT_BOOL(0, "auto-advance", &interactive_opts.auto_advance,
		 N_("auto advance to the next file when selecting hunks interactively")),
	OPT_DIFF_UNIFIED(&interactive_opts.context),
	OPT_DIFF_INTERHUNK_CONTEXT(&interactive_opts.interhunkcontext),
	OPT_BOOL('e', "edit", &edit_interactive, N_("edit current diff and apply")),
	OPT__FORCE(&ignored_too, N_("allow adding otherwise ignored files"), 0),
	OPT_BOOL('u', "update", &take_worktree_changes, N_("update tracked files")),
	OPT_BOOL(0, "renormalize", &ಕೂಡಿಸು_renormalize, N_("renormalize EOL of tracked files (implies -u)")),
	OPT_BOOL(0, "resolved", &ಕೂಡಿಸು_resolved, N_("add conflict-resolved tracked files")),
	OPT_BOOL('N', "intent-to-add", &intent_ಗಮ್ಯ_ಕೂಡಿಸು, N_("record only the fact that the path will be added later")),
	OPT_BOOL('A', "all", &addremove_explicit, N_("add changes from all tracked and untracked files")),
	OPT_CALLBACK_F(0, "ignore-removal", &addremove_explicit,
	  ಖಾಲಿ /* takes no arguments */,
	  N_("ignore paths removed in the working tree (same as --no-all)"),
	  PARSE_OPT_NOARG, ignore_removal_cb),
	OPT_BOOL( 0 , "refresh", &refresh_only, N_("don't add, only refresh the index")),
	OPT_BOOL( 0 , "ignore-errors", &ignore_ಕೂಡಿಸು_errors, N_("just skip files which cannot be added because of errors")),
	OPT_BOOL( 0 , "ignore-missing", &ignore_missing, N_("check if - even missing - files are ignored in dry run")),
	OPT_BOOL(0, "sparse", &include_sparse, N_("allow updating entries outside of the sparse-checkout cone")),
	OPT_STRING(0, "chmod", &chmod_arg, "(+|-)x",
		   N_("override the executable bit of the listed files")),
	OPT_HIDDEN_BOOL(0, "warn-embedded-repo", &warn_on_embedded_repo,
			N_("warn when adding an embedded repository")),
	OPT_PATHSPEC_FROM_FILE(&pathspec_from_file),
	OPT_PATHSPEC_FILE_NUL(&pathspec_file_nul),
	OPT_ಕೊನೆ(),
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಕೂಡಿಸು_config(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *var, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ,
		      ಸ್ಥಿರಾಂಕ ರಚನೆ config_context *ctx, ಶೂನ್ಯ *cb)
{
	ಆದರೆ (!strcmp(var, "add.ignoreerrors") ||
	    !strcmp(var, "add.ignore-errors")) {
		ignore_ಕೂಡಿಸು_errors = git_config_bool(var, ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (git_color_config(var, ಮೌಲ್ಯ, cb) < 0)
		ಹಿಂತಿರುಗಿ -1;

	ಹಿಂತಿರುಗಿ git_default_config(var, ಮೌಲ್ಯ, ctx, cb);
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ embedded_advice[] = N_(
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

ಸ್ಥಿರ ಶೂನ್ಯ check_embedded_repo(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ)
{
	ರಚನೆ strbuf name = STRBUF_INIT;
	ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ adviced_on_embedded_repo = 0;

	ಆದರೆ (!warn_on_embedded_repo)
		ಹಿಂತಿರುಗಿ;
	ಆದರೆ (!ತುದಿಗಳು_with(ಮಾರ್ಗ, "/"))
		ಹಿಂತಿರುಗಿ;

	/* Drop trailing slash for aesthetics */
	strbuf_addstr(&name, ಮಾರ್ಗ);
	strbuf_strip_suffix(&name, "/");

	warning(_("adding embedded git repository: %s"), name.buf);
	ಆದರೆ (!adviced_on_embedded_repo) {
		advise_if_enabled(ADVICE_ಕೂಡಿಸು_EMBEDDED_REPO,
				  embedded_advice, name.buf, name.buf);
		adviced_on_embedded_repo = 1;
	}

	strbuf_release(&name);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಕೂಡಿಸು_files(ರಚನೆ repository *repo, ರಚನೆ dir_struct *dir, ಪೂರ್ಣಾಂಕ flags)
{
	ಪೂರ್ಣಾಂಕ i, exit_ಸ್ಥಿತಿ = 0;
	ರಚನೆ string_list matched_sparse_paths = STRING_LIST_INIT_NODUP;

	ಆದರೆ (dir->ignored_nr) {
		fprintf(stderr, _(ignore_error));
		ಗಾಗಿ (i = 0; i < dir->ignored_nr; i++)
			fprintf(stderr, "%s\n", dir->ignored[i]->name);
		advise_if_enabled(ADVICE_ಕೂಡಿಸು_IGNORED_FILE,
				  _("Use -f if you really want to add them."));
		exit_ಸ್ಥಿತಿ = 1;
	}

	ಗಾಗಿ (i = 0; i < dir->nr; i++) {
		ಆದರೆ (!include_sparse &&
		    !ಮಾರ್ಗ_ಒಳ_sparse_checkout(dir->entries[i]->name, repo->ಸೂಚ್ಯಂಕ)) {
			string_list_append(&matched_sparse_paths,
					   dir->entries[i]->name);
			ಮುಂದುವರಿ;
		}
		ಆದರೆ (ಕೂಡಿಸು_file_ಗಮ್ಯ_ಸೂಚ್ಯಂಕ(repo->ಸೂಚ್ಯಂಕ, dir->entries[i]->name, flags)) {
			ಆದರೆ (!ignore_ಕೂಡಿಸು_errors)
				die(_("adding files failed"));
			exit_ಸ್ಥಿತಿ = 1;
		} ಇಲ್ಲದಿದ್ದರೆ {
			check_embedded_repo(dir->entries[i]->name);
		}
	}

	ಆದರೆ (matched_sparse_paths.nr) {
		advise_on_updating_sparse_paths(&matched_sparse_paths);
		exit_ಸ್ಥಿತಿ = 1;
	}

	string_list_clear(&matched_sparse_paths, 0);

	ಹಿಂತಿರುಗಿ exit_ಸ್ಥಿತಿ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ failed_ಗಮ್ಯ_ಕೂಡಿಸು(ಪೂರ್ಣಾಂಕ flags, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ)
{
	ಆದರೆ (!(flags & ಕೂಡಿಸು_CACHE_IGNORE_ERRORS))
		die(_("updating file '%s' failed"), ಮಾರ್ಗ);
	ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಕೂಡಿಸು_resolved_files(ರಚನೆ repository *repo,
			      ಸ್ಥಿರಾಂಕ ರಚನೆ pathspec *pathspec,
			      ಪೂರ್ಣಾಂಕ flags)
{
	ರಚನೆ ಸೂಚ್ಯಂಕ_state *istate = repo->ಸೂಚ್ಯಂಕ;
	ರಚನೆ string_list unmerged_paths = STRING_LIST_INIT_DUP;
	ರಚನೆ string_list unresolved_paths = STRING_LIST_INIT_DUP;
	ಪೂರ್ಣಾಂಕ exit_ಸ್ಥಿತಿ = 0;
	ಗಾತ್ರ_t i;

	ಗಾಗಿ (i = 0; i < istate->cache_nr; i++) {
		ರಚನೆ cache_ನಮೂದು *ce = istate->cache[i];
		ಆದರೆ (!ce_stage(ce))
			ಮುಂದುವರಿ;
		ಆದರೆ (pathspec->nr && !ce_ಮಾರ್ಗ_ಹೊಂದಿಕೆ(istate, ce, pathspec, ಖಾಲಿ))
			ಮುಂದುವರಿ;
		ಆದರೆ (!unmerged_paths.nr ||
		    strcmp(unmerged_paths.ವಸ್ತುಗಳು[unmerged_paths.nr - 1].string, ce->name))
			string_list_append(&unmerged_paths, ce->name);
	}

	ಆದರೆ (!unmerged_paths.nr) {
		string_list_clear(&unmerged_paths, 0);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಗಾಗಿ (i = 0; i < unmerged_paths.nr; i++) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ = unmerged_paths.ವಸ್ತುಗಳು[i].string;
		ರಚನೆ stat st;

		ಆದರೆ (!lstat(ಮಾರ್ಗ, &st) && S_ISREG(st.st_ಬಹುಲಕ)) {
			ಆದರೆ (has_conflict_markers(istate, ಮಾರ್ಗ))
				string_list_append(&unresolved_paths, ಮಾರ್ಗ);
		}
	}

	ಆದರೆ (unresolved_paths.nr) {
		ರಚನೆ strbuf sb = STRBUF_INIT;
		ಗಾಗಿ (i = 0; i < unresolved_paths.nr; i++)
			strbuf_addf(&sb, "\t%s\n", unresolved_paths.ವಸ್ತುಗಳು[i].string);
		die(_("the following paths still have conflict markers:\n%s"), sb.buf);
	}

	ಗಾಗಿ (i = 0; i < unmerged_paths.nr; i++) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ = unmerged_paths.ವಸ್ತುಗಳು[i].string;
		ರಚನೆ stat st;

		ಆದರೆ (lstat(ಮಾರ್ಗ, &st)) {
			ಆದರೆ (errno != ENOENT)
				die_errno(_("cannot lstat: '%s'"), ಮಾರ್ಗ);
			ಆದರೆ (remove_file_from_ಸೂಚ್ಯಂಕ_with_flags(istate, ಮಾರ್ಗ, flags))
				exit_ಸ್ಥಿತಿ = failed_ಗಮ್ಯ_ಕೂಡಿಸು(flags, ಮಾರ್ಗ);
		} ಇಲ್ಲದಿದ್ದರೆ {
			ಆದರೆ (ಕೂಡಿಸು_file_ಗಮ್ಯ_ಸೂಚ್ಯಂಕ(istate, ಮಾರ್ಗ, flags))
				exit_ಸ್ಥಿತಿ = failed_ಗಮ್ಯ_ಕೂಡಿಸು(flags, ಮಾರ್ಗ);
		}
	}

	string_list_clear(&unmerged_paths, 0);
	string_list_clear(&unresolved_paths, 0);
	ಹಿಂತಿರುಗಿ exit_ಸ್ಥಿತಿ;
}

ಪೂರ್ಣಾಂಕ cmd_ಕೂಡಿಸು(ಪೂರ್ಣಾಂಕ argc,
	    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
	    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
	    ರಚನೆ repository *repo)
{
	ಪೂರ್ಣಾಂಕ exit_ಸ್ಥಿತಿ = 0;
	ರಚನೆ pathspec pathspec;
	ರಚನೆ dir_struct dir = DIR_INIT;
	ಪೂರ್ಣಾಂಕ flags;
	ಪೂರ್ಣಾಂಕ ಕೂಡಿಸು_new_files;
	ಪೂರ್ಣಾಂಕ require_pathspec;
	ಅಕ್ಷರ *ನೋಡಿದ = ಖಾಲಿ;
	ಅಕ್ಷರ *ps_matched = ಖಾಲಿ;
	ರಚನೆ ಬೀಗ_file ಬೀಗ_file = ಬೀಗ_INIT;
	ರಚನೆ odb_transaction *transaction = ಖಾಲಿ;

	repo_config(repo, ಕೂಡಿಸು_config, ಖಾಲಿ);

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, builtin_ಕೂಡಿಸು_options,
			  builtin_ಕೂಡಿಸು_usage, PARSE_OPT_KEEP_ARGV0);

	prepare_repo_settings(repo);
	repo->settings.command_requires_ತುಂಬಿದ_ಸೂಚ್ಯಂಕ = 0;

	ಆದರೆ (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	ಆದರೆ (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	ಆದರೆ (patch_interactive)
		ಕೂಡಿಸು_interactive = 1;
	ಆದರೆ (ಕೂಡಿಸು_interactive) {
		ಆದರೆ (show_only)
			die(_("options '%s' and '%s' cannot be used together"), "--dry-run", "--interactive/--patch");
		ಆದರೆ (pathspec_from_file)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--interactive/--patch");
		ನಿರ್ಗಮಿಸು(interactive_ಕೂಡಿಸು(repo, argv + 1, ಪೂರ್ವಪ್ರತ್ಯಯ, patch_interactive, &interactive_opts));
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಆದರೆ (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--interactive/--patch");
		ಆದರೆ (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--interactive/--patch");
		ಆದರೆ (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--interactive/--patch");
	}

	ಆದರೆ (edit_interactive) {
		ಆದರೆ (pathspec_from_file)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--edit");
		ಹಿಂತಿರುಗಿ(edit_patch(repo, argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ));
	}
	argc--;
	argv++;

	ಆದರೆ (0 <= addremove_explicit)
		addremove = addremove_explicit;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (take_worktree_changes && ADDREMOVE_DEFAULT)
		addremove = 0; /* "-u" was given but not "-A" */

	die_for_incompatible_opt3(take_worktree_changes, "-u/--update",
				  0 < addremove_explicit, "-A/--all",
				  ಕೂಡಿಸು_resolved, "--resolved");

	ಆದರೆ (!show_only && ignore_missing)
		die(_("the option '%s' requires '%s'"), "--ignore-missing", "--dry-run");

	ಆದರೆ (chmod_arg && ((chmod_arg[0] != '-' && chmod_arg[0] != '+') ||
			  chmod_arg[1] != 'x' || chmod_arg[2]))
		die(_("--chmod param '%s' must be either -x or +x"), chmod_arg);

	ಕೂಡಿಸು_new_files = !take_worktree_changes && !refresh_only &&
			!ಕೂಡಿಸು_renormalize && !ಕೂಡಿಸು_resolved;
	require_pathspec = !(take_worktree_changes ||
			     (0 < addremove_explicit) ||
			     ಕೂಡಿಸು_resolved);

	repo_hold_locked_ಸೂಚ್ಯಂಕ(repo, &ಬೀಗ_file, ಬೀಗ_DIE_ON_ERROR);

	/*
	 * Check the "pathspec '%s' did not match any files" block
	 * below before enabling new magic.
	 */
	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_ತುಂಬಿದ |
		       PATHSPEC_SYMLINK_LEADING_ಮಾರ್ಗ,
		       ಪೂರ್ವಪ್ರತ್ಯಯ, argv);

	ಆದರೆ (pathspec_from_file) {
		ಆದರೆ (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_ತುಂಬಿದ |
				    PATHSPEC_SYMLINK_LEADING_ಮಾರ್ಗ,
				    ಪೂರ್ವಪ್ರತ್ಯಯ, pathspec_from_file, pathspec_file_nul);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	ಆದರೆ (require_pathspec && pathspec.nr == 0) {
		fprintf(stderr, _("Nothing specified, nothing added.\n"));
		advise_if_enabled(ADVICE_ಕೂಡಿಸು_ಖಾಲಿಗಳು_PATHSPEC,
				  _("Maybe you wanted to say 'git add .'?"));
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (!take_worktree_changes && !ಕೂಡಿಸು_resolved &&
	    addremove_explicit < 0 && pathspec.nr)
		/* Turn "git add pathspec..." to "git add -A pathspec..." */
		addremove = 1;

	flags = ((verbose ? ಕೂಡಿಸು_CACHE_VERBOSE : 0) |
		 (show_only ? ಕೂಡಿಸು_CACHE_PRETEND : 0) |
		 (intent_ಗಮ್ಯ_ಕೂಡಿಸು ? ಕೂಡಿಸು_CACHE_INTENT : 0) |
		 (ignore_ಕೂಡಿಸು_errors ? ಕೂಡಿಸು_CACHE_IGNORE_ERRORS : 0) |
		 (!(addremove || take_worktree_changes)
		  ? ಕೂಡಿಸು_CACHE_IGNORE_REMOVAL : 0));

	ಆದರೆ (repo_read_ಸೂಚ್ಯಂಕ_preload(repo, &pathspec, 0) < 0)
		die(_("index file corrupt"));

	die_ಒಳ_unpopulated_submodule(repo->ಸೂಚ್ಯಂಕ, ಪೂರ್ವಪ್ರತ್ಯಯ);
	die_ಮಾರ್ಗ_inside_submodule(repo->ಸೂಚ್ಯಂಕ, &pathspec);

	ಆದರೆ (ಕೂಡಿಸು_new_files) {
		ಪೂರ್ಣಾಂಕ baselen;

		/* Set up the default git porcelain excludes */
		ಆದರೆ (!ignored_too) {
			dir.flags |= DIR_COLLECT_IGNORED;
			ಸಿದ್ಧತೆ_standard_excludes(&dir);
		}

		/* This picks up the paths that are not tracked */
		baselen = fill_directory(&dir, repo->ಸೂಚ್ಯಂಕ, &pathspec);
		ಆದರೆ (pathspec.nr)
			ನೋಡಿದ = prune_directory(repo, &dir, &pathspec, baselen);
	}

	ಆದರೆ (refresh_only) {
		exit_ಸ್ಥಿತಿ |= refresh(repo, verbose, &pathspec);
		ಹೋಗು ಮುಕ್ತಾಯ;
	}

	ಆದರೆ (pathspec.nr) {
		ಪೂರ್ಣಾಂಕ i;
		ಅಕ್ಷರ *skip_worktree_ನೋಡಿದ = ಖಾಲಿ;
		ರಚನೆ string_list only_ಹೊಂದಿಕೆ_skip_worktree = STRING_LIST_INIT_NODUP;

		ಆದರೆ (!ನೋಡಿದ)
			ನೋಡಿದ = ಕಂಡುಹಿಡಿ_pathspecs_matching_against_ಸೂಚ್ಯಂಕ(&pathspec,
					repo->ಸೂಚ್ಯಂಕ, PS_IGNORE_SKIP_WORKTREE);

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

		ಗಾಗಿ (i = 0; i < pathspec.nr; i++) {
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ = pathspec.ವಸ್ತುಗಳು[i].ಹೊಂದಿಕೆ;

			ಆದರೆ (pathspec.ವಸ್ತುಗಳು[i].magic & PATHSPEC_EXCLUDE)
				ಮುಂದುವರಿ;
			ಆದರೆ (ನೋಡಿದ[i])
				ಮುಂದುವರಿ;

			ಆದರೆ (!include_sparse &&
			    matches_skip_worktree(&pathspec, i, &skip_worktree_ನೋಡಿದ)) {
				string_list_append(&only_ಹೊಂದಿಕೆ_skip_worktree,
						   pathspec.ವಸ್ತುಗಳು[i].original);
				ಮುಂದುವರಿ;
			}

			/* Don't complain at 'git add .' on empty repo */
			ಆದರೆ (!ಮಾರ್ಗ[0])
				ಮುಂದುವರಿ;

			ಆದರೆ ((pathspec.ವಸ್ತುಗಳು[i].magic & (PATHSPEC_GLOB | PATHSPEC_ICASE)) ||
			    !file_exists(ಮಾರ್ಗ)) {
				ಆದರೆ (ignore_missing) {
					ಪೂರ್ಣಾಂಕ dtype = DT_UNKNOWN;
					ಆದರೆ (is_excluded(&dir, repo->ಸೂಚ್ಯಂಕ, ಮಾರ್ಗ, &dtype))
						dir_ಕೂಡಿಸು_ignored(&dir, repo->ಸೂಚ್ಯಂಕ,
								ಮಾರ್ಗ, pathspec.ವಸ್ತುಗಳು[i].len);
				} ಇಲ್ಲದಿದ್ದರೆ
					die(_("pathspec '%s' did not match any files"),
					    pathspec.ವಸ್ತುಗಳು[i].original);
			}
		}


		ಆದರೆ (only_ಹೊಂದಿಕೆ_skip_worktree.nr) {
			advise_on_updating_sparse_paths(&only_ಹೊಂದಿಕೆ_skip_worktree);
			exit_ಸ್ಥಿತಿ = 1;
		}

		ಬಿಡುಗಡೆ(ನೋಡಿದ);
		ಬಿಡುಗಡೆ(skip_worktree_ನೋಡಿದ);
		string_list_clear(&only_ಹೊಂದಿಕೆ_skip_worktree, 0);
	}

	odb_transaction_begin_or_die(repo->objects, &transaction, 0);

	ps_matched = xcalloc(pathspec.nr, 1);
	ಆದರೆ (ಕೂಡಿಸು_resolved)
		exit_ಸ್ಥಿತಿ |= ಕೂಡಿಸು_resolved_files(repo, &pathspec, flags);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಕೂಡಿಸು_renormalize)
		exit_ಸ್ಥಿತಿ |= renormalize_tracked_files(repo, &pathspec, flags);
	ಇಲ್ಲದಿದ್ದರೆ
		exit_ಸ್ಥಿತಿ |= ಕೂಡಿಸು_files_ಗಮ್ಯ_cache(repo, ಪೂರ್ವಪ್ರತ್ಯಯ,
						  &pathspec, ps_matched,
						  include_sparse, flags, ignored_too);

	ಆದರೆ (take_worktree_changes && !ಕೂಡಿಸು_renormalize && !ignore_ಕೂಡಿಸು_errors &&
	    report_ಮಾರ್ಗ_error(ps_matched, &pathspec))
		ನಿರ್ಗಮಿಸು(128);

	ಆದರೆ (ಕೂಡಿಸು_new_files)
		exit_ಸ್ಥಿತಿ |= ಕೂಡಿಸು_files(repo, &dir, flags);

	ಆದರೆ (chmod_arg && pathspec.nr)
		exit_ಸ್ಥಿತಿ |= chmod_pathspec(repo, &pathspec, chmod_arg[0], show_only);
	odb_transaction_commit_and_finalize_or_die(transaction);

ಮುಕ್ತಾಯ:
	ಆದರೆ (write_locked_ಸೂಚ್ಯಂಕ(repo->ಸೂಚ್ಯಂಕ, &ಬೀಗ_file,
			       COMMIT_ಬೀಗ | SKIP_IF_UNCHANGED))
		die(_("unable to write new index file"));

	ಬಿಡುಗಡೆ(ps_matched);
	dir_clear(&dir);
	clear_pathspec(&pathspec);
	ಹಿಂತಿರುಗಿ exit_ಸ್ಥಿತಿ;
}
