/*
 * "git rm" builtin command
 *
 * Copyright (C) Linus Torvalds 2006
 */

#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "advice.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "lockfile.h"
#ಅನ್ತರ್ಭಾವ "dir.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "hash.h"
#ಅನ್ತರ್ಭಾವ "tree-walk.h"
#ಅನ್ತರ್ಭಾವ "object-name.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "read-cache.h"

#ಅನ್ತರ್ಭಾವ "string-list.h"
#ಅನ್ತರ್ಭಾವ "setup.h"
#ಅನ್ತರ್ಭಾವ "sparse-index.h"
#ಅನ್ತರ್ಭಾವ "submodule.h"
#ಅನ್ತರ್ಭಾವ "pathspec.h"

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ builtin_rm_usage[] = {
	N_("git rm [-f | --force] [-n] [-r] [--cached] [--ignore-unmatch]\n"
	   "       [--quiet] [--pathspec-from-file=<file> [--pathspec-file-nul]]\n"
	   "       [--] [<pathspec>...]"),
	ಖಾಲಿ
};

ಸ್ಥಿರ ರಚನೆ {
	ಪೂರ್ಣಾಂಕ nr, alloc;
	ರಚನೆ {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name;
		ಅಕ್ಷರ is_submodule;
	} *ನಮೂದು;
} list;

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ get_ours_cache_pos(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ pos)
{
	ಆಗ ((pos < the_repository->ಸೂಚ್ಯಂಕ->cache_nr) && !strcmp(the_repository->ಸೂಚ್ಯಂಕ->cache[pos]->name, ಮಾರ್ಗ)) {
		ಆದರೆ (ce_stage(the_repository->ಸೂಚ್ಯಂಕ->cache[pos]) == 2)
			ಹಿಂತಿರುಗಿ pos;
		pos++;
	}
	ಹಿಂತಿರುಗಿ -1;
}

ಸ್ಥಿರ ಶೂನ್ಯ print_error_files(ರಚನೆ string_list *files_list,
			      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *main_ಸಂದೇಶ,
			      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *hints_ಸಂದೇಶ,
			      ಪೂರ್ಣಾಂಕ *errs)
{
	ಆದರೆ (files_list->nr) {
		ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ i;
		ರಚನೆ strbuf err_ಸಂದೇಶ = STRBUF_INIT;

		strbuf_addstr(&err_ಸಂದೇಶ, main_ಸಂದೇಶ);
		ಗಾಗಿ (i = 0; i < files_list->nr; i++)
			strbuf_addf(&err_ಸಂದೇಶ,
				    "\n    %s",
				    files_list->ವಸ್ತುಗಳು[i].string);
		ಆದರೆ (advice_enabled(ADVICE_RM_HINTS))
			strbuf_addstr(&err_ಸಂದೇಶ, hints_ಸಂದೇಶ);
		*errs = error("%s", err_ಸಂದೇಶ.buf);
		strbuf_release(&err_ಸಂದೇಶ);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ submodules_absorb_gitdir_if_needed(ಶೂನ್ಯ)
{
	ಪೂರ್ಣಾಂಕ i;
	ಗಾಗಿ (i = 0; i < list.nr; i++) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name = list.ನಮೂದು[i].name;
		ಪೂರ್ಣಾಂಕ pos;
		ಸ್ಥಿರಾಂಕ ರಚನೆ cache_ನಮೂದು *ce;

		pos = ಸೂಚ್ಯಂಕ_name_pos(the_repository->ಸೂಚ್ಯಂಕ, name, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(name));
		ಆದರೆ (pos < 0) {
			pos = get_ours_cache_pos(name, -pos - 1);
			ಆದರೆ (pos < 0)
				ಮುಂದುವರಿ;
		}
		ce = the_repository->ಸೂಚ್ಯಂಕ->cache[pos];

		ಆದರೆ (!S_ISGITLINK(ce->ce_ಬಹುಲಕ) ||
		    !file_exists(ce->name) ||
		    is_ಖಾಲಿಗಳು_dir(name))
			ಮುಂದುವರಿ;

		ಆದರೆ (!submodule_ಬಳಸುತ್ತದೆ_gitfile(name))
			absorb_git_dir_into_superproject(name, ಖಾಲಿ);
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ check_local_mod(ರಚನೆ object_id *ತಲೆ, ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ_only)
{
	/*
	 * Items in list are already sorted in the cache order,
	 * so we could do this a lot more efficiently by using
	 * tree_desc based traversal if we wanted to, but I am
	 * lazy, and who cares if removal of files is a tad
	 * slower than the theoretical maximum speed?
	 */
	ಪೂರ್ಣಾಂಕ i, no_ತಲೆ;
	ಪೂರ್ಣಾಂಕ errs = 0;
	ರಚನೆ string_list files_staged = STRING_LIST_INIT_NODUP;
	ರಚನೆ string_list files_cached = STRING_LIST_INIT_NODUP;
	ರಚನೆ string_list files_local = STRING_LIST_INIT_NODUP;

	no_ತಲೆ = is_null_oid(ತಲೆ);
	ಗಾಗಿ (i = 0; i < list.nr; i++) {
		ರಚನೆ stat st;
		ಪೂರ್ಣಾಂಕ pos;
		ಸ್ಥಿರಾಂಕ ರಚನೆ cache_ನಮೂದು *ce;
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name = list.ನಮೂದು[i].name;
		ರಚನೆ object_id oid;
		ಚಿಹ್ನೆರಹಿತ ಗಿಡ್ಡ ಬಹುಲಕ;
		ಪೂರ್ಣಾಂಕ local_changes = 0;
		ಪೂರ್ಣಾಂಕ staged_changes = 0;

		pos = ಸೂಚ್ಯಂಕ_name_pos(the_repository->ಸೂಚ್ಯಂಕ, name, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(name));
		ಆದರೆ (pos < 0) {
			/*
			 * Skip unmerged entries except for populated submodules
			 * that could lose history when removed.
			 */
			pos = get_ours_cache_pos(name, -pos - 1);
			ಆದರೆ (pos < 0)
				ಮುಂದುವರಿ;

			ಆದರೆ (!S_ISGITLINK(the_repository->ಸೂಚ್ಯಂಕ->cache[pos]->ce_ಬಹುಲಕ) ||
			    is_ಖಾಲಿಗಳು_dir(name))
				ಮುಂದುವರಿ;
		}
		ce = the_repository->ಸೂಚ್ಯಂಕ->cache[pos];

		ಆದರೆ (lstat(ce->name, &st) < 0) {
			ಆದರೆ (!is_missing_file_error(errno))
				warning_errno(_("failed to stat '%s'"), ce->name);
			/* It already vanished from the working tree */
			ಮುಂದುವರಿ;
		}
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (S_ISDIR(st.st_ಬಹುಲಕ)) {
			/* if a file was removed and it is now a
			 * directory, that is the same as ENOENT as
			 * far as git is concerned; we do not track
			 * directories unless they are submodules.
			 */
			ಆದರೆ (!S_ISGITLINK(ce->ce_ಬಹುಲಕ))
				ಮುಂದುವರಿ;
		}

		/*
		 * "rm" of a path that has changes need to be treated
		 * carefully not to allow losing local changes
		 * accidentally.  A local change could be (1) file in
		 * work tree is different since the index; and/or (2)
		 * the user staged a content that is different from
		 * the current commit in the index.
		 *
		 * In such a case, you would need to --force the
		 * removal.  However, "rm --cached" (remove only from
		 * the index) is safe if the index matches the file in
		 * the work tree or the HEAD commit, as it means that
		 * the content being removed is available elsewhere.
		 */

		/*
		 * Is the index different from the file in the work tree?
		 * If it's a submodule, is its work tree modified?
		 */
		ಆದರೆ (ie_ಹೊಂದಿಕೆ_stat(the_repository->ಸೂಚ್ಯಂಕ, ce, &st, 0) ||
		    (S_ISGITLINK(ce->ce_ಬಹುಲಕ) &&
		     bad_ಗಮ್ಯ_remove_submodule(ce->name,
				SUBMODULE_REMOVAL_DIE_ON_ERROR |
				SUBMODULE_REMOVAL_IGNORE_IGNORED_UNTRACKED)))
			local_changes = 1;

		/*
		 * Is the index different from the HEAD commit?  By
		 * definition, before the very initial commit,
		 * anything staged in the index is treated by the same
		 * way as changed from the HEAD.
		 */
		ಆದರೆ (no_ತಲೆ
		     || get_ವೃಕ್ಷ_ನಮೂದು(the_repository, ತಲೆ, name, &oid, &ಬಹುಲಕ)
		     || ce->ce_ಬಹುಲಕ != create_ce_ಬಹುಲಕ(ಬಹುಲಕ)
		     || !oideq(&ce->oid, &oid))
			staged_changes = 1;

		/*
		 * If the index does not match the file in the work
		 * tree and if it does not match the HEAD commit
		 * either, (1) "git rm" without --cached definitely
		 * will lose information; (2) "git rm --cached" will
		 * lose information unless it is about removing an
		 * "intent to add" entry.
		 */
		ಆದರೆ (local_changes && staged_changes) {
			ಆದರೆ (!ಸೂಚ್ಯಂಕ_only || !ce_intent_ಗಮ್ಯ_ಕೂಡಿಸು(ce))
				string_list_append(&files_staged, name);
		}
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!ಸೂಚ್ಯಂಕ_only) {
			ಆದರೆ (staged_changes)
				string_list_append(&files_cached, name);
			ಆದರೆ (local_changes)
				string_list_append(&files_local, name);
		}
	}
	print_error_files(&files_staged,
			  Q_("the following file has staged content different "
			     "from both the\nfile and the HEAD:",
			     "the following files have staged content different"
			     " from both the\nfile and the HEAD:",
			     files_staged.nr),
			  _("\n(use -f to force removal)"),
			  &errs);
	string_list_clear(&files_staged, 0);
	print_error_files(&files_cached,
			  Q_("the following file has changes "
			     "staged in the index:",
			     "the following files have changes "
			     "staged in the index:", files_cached.nr),
			  _("\n(use --cached to keep the file,"
			    " or -f to force removal)"),
			  &errs);
	string_list_clear(&files_cached, 0);

	print_error_files(&files_local,
			  Q_("the following file has local modifications:",
			     "the following files have local modifications:",
			     files_local.nr),
			  _("\n(use --cached to keep the file,"
			    " or -f to force removal)"),
			  &errs);
	string_list_clear(&files_local, 0);

	ಹಿಂತಿರುಗಿ errs;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_only = 0, force = 0, ಸೂಚ್ಯಂಕ_only = 0, recursive = 0, quiet = 0;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ignore_unmatch = 0, pathspec_file_nul;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ include_sparse;
ಸ್ಥಿರ ಅಕ್ಷರ *pathspec_from_file;

ಸ್ಥಿರ ರಚನೆ option builtin_rm_options[] = {
	OPT_DRY_ಸತತ(&show_only, N_("dry run")),
	OPT__QUIET(&quiet, N_("do not list removed files")),
	OPT_BOOL( 0 , "cached",         &ಸೂಚ್ಯಂಕ_only, N_("only remove from the index")),
	OPT__FORCE(&force, N_("override the up-to-date check"), PARSE_OPT_NOCOMPLETE),
	OPT_BOOL('r', ಖಾಲಿ,             &recursive,  N_("allow recursive removal")),
	OPT_BOOL( 0 , "ignore-unmatch", &ignore_unmatch,
				N_("exit with a zero status even if nothing matched")),
	OPT_BOOL(0, "sparse", &include_sparse, N_("allow updating entries outside of the sparse-checkout cone")),
	OPT_PATHSPEC_FROM_FILE(&pathspec_from_file),
	OPT_PATHSPEC_FILE_NUL(&pathspec_file_nul),
	OPT_ಕೊನೆ(),
};

ಪೂರ್ಣಾಂಕ cmd_rm(ಪೂರ್ಣಾಂಕ argc,
	   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
	   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
	   ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ ಬೀಗ_file ಬೀಗ_file = ಬೀಗ_INIT;
	ಪೂರ್ಣಾಂಕ i, ret = 0;
	ರಚನೆ pathspec pathspec;
	ಅಕ್ಷರ *ನೋಡಿದ;

	repo_config(the_repository, git_default_config, ಖಾಲಿ);

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, builtin_rm_options,
			     builtin_rm_usage, 0);

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD,
		       ಪೂರ್ವಪ್ರತ್ಯಯ, argv);

	ಆದರೆ (pathspec_from_file) {
		ಆದರೆ (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_CWD,
				    ಪೂರ್ವಪ್ರತ್ಯಯ, pathspec_from_file, pathspec_file_nul);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	ಆದರೆ (!pathspec.nr)
		die(_("No pathspec was given. Which files should I remove?"));

	ಆದರೆ (!ಸೂಚ್ಯಂಕ_only)
		ಸಿದ್ಧತೆ_work_ವೃಕ್ಷ(the_repository);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_ತುಂಬಿದ_ಸೂಚ್ಯಂಕ = 0;
	repo_hold_locked_ಸೂಚ್ಯಂಕ(the_repository, &ಬೀಗ_file, ಬೀಗ_DIE_ON_ERROR);

	ಆದರೆ (repo_read_ಸೂಚ್ಯಂಕ(the_repository) < 0)
		die(_("index file corrupt"));

	refresh_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ, REFRESH_QUIET|REFRESH_UNMERGED, &pathspec, ಖಾಲಿ, ಖಾಲಿ);

	ನೋಡಿದ = xcalloc(pathspec.nr, 1);

	ಆದರೆ (pathspec_needs_expanded_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ, &pathspec))
		ensure_ತುಂಬಿದ_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ);

	ಗಾಗಿ (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ i = 0; i < the_repository->ಸೂಚ್ಯಂಕ->cache_nr; i++) {
		ಸ್ಥಿರಾಂಕ ರಚನೆ cache_ನಮೂದು *ce = the_repository->ಸೂಚ್ಯಂಕ->cache[i];

		ಆದರೆ (!include_sparse &&
		    (ce_skip_worktree(ce) ||
		     !ಮಾರ್ಗ_ಒಳ_sparse_checkout(ce->name, the_repository->ಸೂಚ್ಯಂಕ)))
			ಮುಂದುವರಿ;
		ಆದರೆ (!ce_ಮಾರ್ಗ_ಹೊಂದಿಕೆ(the_repository->ಸೂಚ್ಯಂಕ, ce, &pathspec, ನೋಡಿದ))
			ಮುಂದುವರಿ;
		ALLOC_GROW(list.ನಮೂದು, list.nr + 1, list.alloc);
		list.ನಮೂದು[list.nr].name = xstrdup(ce->name);
		list.ನಮೂದು[list.nr].is_submodule = S_ISGITLINK(ce->ce_ಬಹುಲಕ);
		ಆದರೆ (list.ನಮೂದು[list.nr++].is_submodule &&
		    !is_staging_gitmodules_ok(the_repository->ಸೂಚ್ಯಂಕ))
			die(_("please stage your changes to .gitmodules or stash them to proceed"));
	}

	ಆದರೆ (pathspec.nr) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *original;
		ಪೂರ್ಣಾಂಕ ನೋಡಿದ_any = 0;
		ಅಕ್ಷರ *skip_worktree_ನೋಡಿದ = ಖಾಲಿ;
		ರಚನೆ string_list only_ಹೊಂದಿಕೆ_skip_worktree = STRING_LIST_INIT_NODUP;

		ಗಾಗಿ (i = 0; i < pathspec.nr; i++) {
			original = pathspec.ವಸ್ತುಗಳು[i].original;
			ಆದರೆ (ನೋಡಿದ[i])
				ನೋಡಿದ_any = 1;
			ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ignore_unmatch)
				ಮುಂದುವರಿ;
			ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!include_sparse &&
				 matches_skip_worktree(&pathspec, i, &skip_worktree_ನೋಡಿದ))
				string_list_append(&only_ಹೊಂದಿಕೆ_skip_worktree, original);
			ಇಲ್ಲದಿದ್ದರೆ
				die(_("pathspec '%s' did not match any files"), original);

			ಆದರೆ (!recursive && ನೋಡಿದ[i] == MATCHED_RECURSIVELY)
				die(_("not removing '%s' recursively without -r"),
				    *original ? original : ".");
		}

		ಆದರೆ (only_ಹೊಂದಿಕೆ_skip_worktree.nr) {
			advise_on_updating_sparse_paths(&only_ಹೊಂದಿಕೆ_skip_worktree);
			ret = 1;
		}
		ಬಿಡುಗಡೆ(skip_worktree_ನೋಡಿದ);
		string_list_clear(&only_ಹೊಂದಿಕೆ_skip_worktree, 0);

		ಆದರೆ (!ನೋಡಿದ_any)
			ನಿರ್ಗಮಿಸು(ret);
	}
	clear_pathspec(&pathspec);
	ಬಿಡುಗಡೆ(ನೋಡಿದ);

	ಆದರೆ (!ಸೂಚ್ಯಂಕ_only)
		submodules_absorb_gitdir_if_needed();

	/*
	 * If not forced, the file, the index and the HEAD (if exists)
	 * must match; but the file can already been removed, since
	 * this sequence is a natural "novice" way:
	 *
	 *	rm F; git rm F
	 *
	 * Further, if HEAD commit exists, "diff-index --cached" must
	 * report no changes unless forced.
	 */
	ಆದರೆ (!force) {
		ರಚನೆ object_id oid;
		ಆದರೆ (repo_get_oid(the_repository, "HEAD", &oid))
			oidclr(&oid, the_repository->hash_algo);
		ಆದರೆ (check_local_mod(&oid, ಸೂಚ್ಯಂಕ_only))
			ನಿರ್ಗಮಿಸು(1);
	}

	/*
	 * First remove the names from the index: we won't commit
	 * the index unless all of them succeed.
	 */
	ಗಾಗಿ (i = 0; i < list.nr; i++) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ = list.ನಮೂದು[i].name;
		ಆದರೆ (!quiet)
			ಬರೆ("rm '%s'\n", ಮಾರ್ಗ);

		ಆದರೆ (remove_file_from_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ, ಮಾರ್ಗ))
			die(_("git rm: unable to remove %s"), ಮಾರ್ಗ);
	}

	ಆದರೆ (show_only)
		ಹಿಂತಿರುಗಿ 0;

	/*
	 * Then, unless we used "--cached", remove the filenames from
	 * the workspace. If we fail to remove the first one, we
	 * abort the "git rm" (but once we've successfully removed
	 * any file at all, we'll go ahead and commit to it all:
	 * by then we've already committed ourselves and can't fail
	 * in the middle)
	 */
	ಆದರೆ (!ಸೂಚ್ಯಂಕ_only) {
		ಪೂರ್ಣಾಂಕ removed = 0, gitmodules_modified = 0;
		ರಚನೆ strbuf buf = STRBUF_INIT;
		ಪೂರ್ಣಾಂಕ flag = force ? REMOVE_DIR_PURGE_ORIGINAL_CWD : 0;
		ಗಾಗಿ (i = 0; i < list.nr; i++) {
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ = list.ನಮೂದು[i].name;
			ಆದರೆ (list.ನಮೂದು[i].is_submodule) {
				strbuf_reset(&buf);
				strbuf_addstr(&buf, ಮಾರ್ಗ);
				ಆದರೆ (remove_dir_recursively(&buf, flag))
					die(_("could not remove '%s'"), ಮಾರ್ಗ);

				removed = 1;
				ಆದರೆ (!remove_ಮಾರ್ಗ_from_gitmodules(ಮಾರ್ಗ))
					gitmodules_modified = 1;
				ಮುಂದುವರಿ;
			}
			ಆದರೆ (!remove_ಮಾರ್ಗ(ಮಾರ್ಗ)) {
				removed = 1;
				ಮುಂದುವರಿ;
			}
			ಆದರೆ (!removed)
				die_errno("git rm: '%s'", ಮಾರ್ಗ);
		}
		strbuf_release(&buf);
		ಆದರೆ (gitmodules_modified)
			stage_updated_gitmodules(the_repository->ಸೂಚ್ಯಂಕ);
	}

	ಆದರೆ (write_locked_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ, &ಬೀಗ_file,
			       COMMIT_ಬೀಗ | SKIP_IF_UNCHANGED))
		die(_("Unable to write new index file"));

	ಹಿಂತಿರುಗಿ ret;
}
