/*
 * "git rm" builtin command
 *
 * Copyright (C) Linus Torvalds 2006
 */

#define USE_THE_REPOSITORY_VARIABLE

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

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ builtin_rm_usage[] = {
	N_("git rm [-f | --force] [-n] [-r] [--cached] [--ignore-unmatch]\n"
	   "       [--quiet] [--pathspec-from-file=<file> [--pathspec-file-nul]]\n"
	   "       [--] [<pathspec>...]"),
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಸಂರಚನಾ {
	ಪೂರ್ಣಾಙ್ಕ nr, alloc;
	ಸಂರಚನಾ {
		ಧ್ರುವ ವರ್ಣ *name;
		ವರ್ಣ is_submodule;
	} *entry;
} list;

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ get_ours_cache_pos(ಧ್ರುವ ವರ್ಣ *path, unsigned ಪೂರ್ಣಾಙ್ಕ pos)
{
	ಯಾವತ್ ((pos < the_repository->index->cache_nr) && !strcmp(the_repository->index->cache[pos]->name, path)) {
		ಯದಿ (ce_stage(the_repository->index->cache[pos]) == 2)
			ಪ್ರತ್ಯಾಗಮ pos;
		pos++;
	}
	ಪ್ರತ್ಯಾಗಮ -1;
}

ಸ್ಥಿರ ಶೂನ್ಯ print_error_files(ಸಂರಚನಾ string_list *files_list,
			      ಧ್ರುವ ವರ್ಣ *main_ಸಂದೇಶ,
			      ಧ್ರುವ ವರ್ಣ *hints_ಸಂದೇಶ,
			      ಪೂರ್ಣಾಙ್ಕ *errs)
{
	ಯದಿ (files_list->nr) {
		unsigned ಪೂರ್ಣಾಙ್ಕ i;
		ಸಂರಚನಾ strbuf err_ಸಂದೇಶ = STRBUF_INIT;

		strbuf_addstr(&err_ಸಂದೇಶ, main_ಸಂದೇಶ);
		ಪ್ರತಿ (i = 0; i < files_list->nr; i++)
			strbuf_addf(&err_ಸಂದೇಶ,
				    "\n    %s",
				    files_list->ವಸ್ತುಗಳು[i].string);
		ಯದಿ (advice_enabled(ADVICE_RM_HINTS))
			strbuf_addstr(&err_ಸಂದೇಶ, hints_ಸಂದೇಶ);
		*errs = error("%s", err_ಸಂದೇಶ.buf);
		strbuf_release(&err_ಸಂದೇಶ);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ submodules_absorb_gitdir_if_needed(ಶೂನ್ಯ)
{
	ಪೂರ್ಣಾಙ್ಕ i;
	ಪ್ರತಿ (i = 0; i < list.nr; i++) {
		ಧ್ರುವ ವರ್ಣ *name = list.entry[i].name;
		ಪೂರ್ಣಾಙ್ಕ pos;
		ಧ್ರುವ ಸಂರಚನಾ cache_entry *ce;

		pos = index_name_pos(the_repository->index, name, strlen(name));
		ಯದಿ (pos < 0) {
			pos = get_ours_cache_pos(name, -pos - 1);
			ಯದಿ (pos < 0)
				ಅನುವರ್ತ;
		}
		ce = the_repository->index->cache[pos];

		ಯದಿ (!S_ISGITLINK(ce->ce_mode) ||
		    !file_exists(ce->name) ||
		    is_empty_dir(name))
			ಅನುವರ್ತ;

		ಯದಿ (!submodule_uses_gitfile(name))
			absorb_git_dir_into_superproject(name, ರಿಕ್ತ);
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ check_local_mod(ಸಂರಚನಾ object_id *head, ಪೂರ್ಣಾಙ್ಕ index_only)
{
	/*
	 * Items in list are already sorted in the cache order,
	 * so we could do this a lot more efficiently by using
	 * tree_desc based traversal if we wanted to, but I am
	 * lazy, and who cares if removal of files is a tad
	 * slower than the theoretical maximum speed?
	 */
	ಪೂರ್ಣಾಙ್ಕ i, no_head;
	ಪೂರ್ಣಾಙ್ಕ errs = 0;
	ಸಂರಚನಾ string_list files_staged = STRING_LIST_INIT_NODUP;
	ಸಂರಚನಾ string_list files_cached = STRING_LIST_INIT_NODUP;
	ಸಂರಚನಾ string_list files_local = STRING_LIST_INIT_NODUP;

	no_head = is_null_oid(head);
	ಪ್ರತಿ (i = 0; i < list.nr; i++) {
		ಸಂರಚನಾ stat st;
		ಪೂರ್ಣಾಙ್ಕ pos;
		ಧ್ರುವ ಸಂರಚನಾ cache_entry *ce;
		ಧ್ರುವ ವರ್ಣ *name = list.entry[i].name;
		ಸಂರಚನಾ object_id oid;
		unsigned short mode;
		ಪೂರ್ಣಾಙ್ಕ local_changes = 0;
		ಪೂರ್ಣಾಙ್ಕ staged_changes = 0;

		pos = index_name_pos(the_repository->index, name, strlen(name));
		ಯದಿ (pos < 0) {
			/*
			 * Skip unmerged entries except for populated submodules
			 * that could lose history when removed.
			 */
			pos = get_ours_cache_pos(name, -pos - 1);
			ಯದಿ (pos < 0)
				ಅನುವರ್ತ;

			ಯದಿ (!S_ISGITLINK(the_repository->index->cache[pos]->ce_mode) ||
			    is_empty_dir(name))
				ಅನುವರ್ತ;
		}
		ce = the_repository->index->cache[pos];

		ಯದಿ (lstat(ce->name, &st) < 0) {
			ಯದಿ (!is_missing_file_error(errno))
				warning_errno(_("failed to stat '%s'"), ce->name);
			/* It already vanished from the working tree */
			ಅನುವರ್ತ;
		}
		ಅನ್ಯಥಾ ಯದಿ (S_ISDIR(st.st_mode)) {
			/* if a file was removed and it is now a
			 * directory, that is the same as ENOENT as
			 * far as git is concerned; we do not track
			 * directories unless they are submodules.
			 */
			ಯದಿ (!S_ISGITLINK(ce->ce_mode))
				ಅನುವರ್ತ;
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
		ಯದಿ (ie_match_stat(the_repository->index, ce, &st, 0) ||
		    (S_ISGITLINK(ce->ce_mode) &&
		     bad_to_remove_submodule(ce->name,
				SUBMODULE_REMOVAL_DIE_ON_ERROR |
				SUBMODULE_REMOVAL_IGNORE_IGNORED_UNTRACKED)))
			local_changes = 1;

		/*
		 * Is the index different from the HEAD commit?  By
		 * definition, before the very initial commit,
		 * anything staged in the index is treated by the same
		 * way as changed from the HEAD.
		 */
		ಯದಿ (no_head
		     || get_ವೃಕ್ಷ_entry(the_repository, head, name, &oid, &mode)
		     || ce->ce_mode != create_ce_mode(mode)
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
		ಯದಿ (local_changes && staged_changes) {
			ಯದಿ (!index_only || !ce_intent_to_add(ce))
				string_list_append(&files_staged, name);
		}
		ಅನ್ಯಥಾ ಯದಿ (!index_only) {
			ಯದಿ (staged_changes)
				string_list_append(&files_cached, name);
			ಯದಿ (local_changes)
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

	ಪ್ರತ್ಯಾಗಮ errs;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_only = 0, force = 0, index_only = 0, recursive = 0, quiet = 0;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ ignore_unmatch = 0, pathspec_file_nul;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ include_sparse;
ಸ್ಥಿರ ವರ್ಣ *pathspec_from_file;

ಸ್ಥಿರ ಸಂರಚನಾ option builtin_rm_options[] = {
	OPT__DRY_RUN(&show_only, N_("dry run")),
	OPT__QUIET(&quiet, N_("do not list removed files")),
	OPT_BOOL( 0 , "cached",         &index_only, N_("only remove from the index")),
	OPT__FORCE(&force, N_("override the up-to-date check"), PARSE_OPT_NOCOMPLETE),
	OPT_BOOL('r', ರಿಕ್ತ,             &recursive,  N_("allow recursive removal")),
	OPT_BOOL( 0 , "ignore-unmatch", &ignore_unmatch,
				N_("exit with a zero status even if nothing matched")),
	OPT_BOOL(0, "sparse", &include_sparse, N_("allow updating entries outside of the sparse-checkout cone")),
	OPT_PATHSPEC_FROM_FILE(&pathspec_from_file),
	OPT_PATHSPEC_FILE_NUL(&pathspec_file_nul),
	OPT_END(),
};

ಪೂರ್ಣಾಙ್ಕ cmd_rm(ಪೂರ್ಣಾಙ್ಕ argc,
	   ಧ್ರುವ ವರ್ಣ **argv,
	   ಧ್ರುವ ವರ್ಣ *prefix,
	   ಸಂರಚನಾ repository *repo UNUSED)
{
	ಸಂರಚನಾ lock_file lock_file = LOCK_INIT;
	ಪೂರ್ಣಾಙ್ಕ i, ret = 0;
	ಸಂರಚನಾ pathspec pathspec;
	ವರ್ಣ *seen;

	repo_config(the_repository, git_default_config, ರಿಕ್ತ);

	argc = parse_options(argc, argv, prefix, builtin_rm_options,
			     builtin_rm_usage, 0);

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD,
		       prefix, argv);

	ಯದಿ (pathspec_from_file) {
		ಯದಿ (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_CWD,
				    prefix, pathspec_from_file, pathspec_file_nul);
	} ಅನ್ಯಥಾ ಯದಿ (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	ಯದಿ (!pathspec.nr)
		die(_("No pathspec was given. Which files should I remove?"));

	ಯದಿ (!index_only)
		setup_work_ವೃಕ್ಷ(the_repository);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;
	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);

	ಯದಿ (repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));

	refresh_index(the_repository->index, REFRESH_QUIET|REFRESH_UNMERGED, &pathspec, ರಿಕ್ತ, ರಿಕ್ತ);

	seen = xcalloc(pathspec.nr, 1);

	ಯದಿ (pathspec_needs_expanded_index(the_repository->index, &pathspec))
		ensure_full_index(the_repository->index);

	ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < the_repository->index->cache_nr; i++) {
		ಧ್ರುವ ಸಂರಚನಾ cache_entry *ce = the_repository->index->cache[i];

		ಯದಿ (!include_sparse &&
		    (ce_skip_worktree(ce) ||
		     !path_in_sparse_checkout(ce->name, the_repository->index)))
			ಅನುವರ್ತ;
		ಯದಿ (!ce_path_match(the_repository->index, ce, &pathspec, seen))
			ಅನುವರ್ತ;
		ALLOC_GROW(list.entry, list.nr + 1, list.alloc);
		list.entry[list.nr].name = xstrdup(ce->name);
		list.entry[list.nr].is_submodule = S_ISGITLINK(ce->ce_mode);
		ಯದಿ (list.entry[list.nr++].is_submodule &&
		    !is_staging_gitmodules_ok(the_repository->index))
			die(_("please stage your changes to .gitmodules or stash them to proceed"));
	}

	ಯದಿ (pathspec.nr) {
		ಧ್ರುವ ವರ್ಣ *original;
		ಪೂರ್ಣಾಙ್ಕ seen_any = 0;
		ವರ್ಣ *skip_worktree_seen = ರಿಕ್ತ;
		ಸಂರಚನಾ string_list only_match_skip_worktree = STRING_LIST_INIT_NODUP;

		ಪ್ರತಿ (i = 0; i < pathspec.nr; i++) {
			original = pathspec.ವಸ್ತುಗಳು[i].original;
			ಯದಿ (seen[i])
				seen_any = 1;
			ಅನ್ಯಥಾ ಯದಿ (ignore_unmatch)
				ಅನುವರ್ತ;
			ಅನ್ಯಥಾ ಯದಿ (!include_sparse &&
				 matches_skip_worktree(&pathspec, i, &skip_worktree_seen))
				string_list_append(&only_match_skip_worktree, original);
			ಅನ್ಯಥಾ
				die(_("pathspec '%s' did not match any files"), original);

			ಯದಿ (!recursive && seen[i] == MATCHED_RECURSIVELY)
				die(_("not removing '%s' recursively without -r"),
				    *original ? original : ".");
		}

		ಯದಿ (only_match_skip_worktree.nr) {
			advise_on_updating_sparse_paths(&only_match_skip_worktree);
			ret = 1;
		}
		ಮುಕ್ತ(skip_worktree_seen);
		string_list_clear(&only_match_skip_worktree, 0);

		ಯದಿ (!seen_any)
			exit(ret);
	}
	clear_pathspec(&pathspec);
	ಮುಕ್ತ(seen);

	ಯದಿ (!index_only)
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
	ಯದಿ (!force) {
		ಸಂರಚನಾ object_id oid;
		ಯದಿ (repo_get_oid(the_repository, "HEAD", &oid))
			oidclr(&oid, the_repository->hash_algo);
		ಯದಿ (check_local_mod(&oid, index_only))
			exit(1);
	}

	/*
	 * First remove the names from the index: we won't commit
	 * the index unless all of them succeed.
	 */
	ಪ್ರತಿ (i = 0; i < list.nr; i++) {
		ಧ್ರುವ ವರ್ಣ *path = list.entry[i].name;
		ಯದಿ (!quiet)
			printf("rm '%s'\n", path);

		ಯದಿ (remove_file_from_index(the_repository->index, path))
			die(_("git rm: unable to remove %s"), path);
	}

	ಯದಿ (show_only)
		ಪ್ರತ್ಯಾಗಮ 0;

	/*
	 * Then, unless we used "--cached", remove the filenames from
	 * the workspace. If we fail to remove the first one, we
	 * abort the "git rm" (but once we've successfully removed
	 * any file at all, we'll go ahead and commit to it all:
	 * by then we've already committed ourselves and can't fail
	 * in the middle)
	 */
	ಯದಿ (!index_only) {
		ಪೂರ್ಣಾಙ್ಕ removed = 0, gitmodules_modified = 0;
		ಸಂರಚನಾ strbuf buf = STRBUF_INIT;
		ಪೂರ್ಣಾಙ್ಕ flag = force ? REMOVE_DIR_PURGE_ORIGINAL_CWD : 0;
		ಪ್ರತಿ (i = 0; i < list.nr; i++) {
			ಧ್ರುವ ವರ್ಣ *path = list.entry[i].name;
			ಯದಿ (list.entry[i].is_submodule) {
				strbuf_reset(&buf);
				strbuf_addstr(&buf, path);
				ಯದಿ (remove_dir_recursively(&buf, flag))
					die(_("could not remove '%s'"), path);

				removed = 1;
				ಯದಿ (!remove_path_from_gitmodules(path))
					gitmodules_modified = 1;
				ಅನುವರ್ತ;
			}
			ಯದಿ (!remove_path(path)) {
				removed = 1;
				ಅನುವರ್ತ;
			}
			ಯದಿ (!removed)
				die_errno("git rm: '%s'", path);
		}
		strbuf_release(&buf);
		ಯದಿ (gitmodules_modified)
			stage_updated_gitmodules(the_repository->index);
	}

	ಯದಿ (write_locked_index(the_repository->index, &lock_file,
			       COMMIT_LOCK | SKIP_IF_UNCHANGED))
		die(_("Unable to write new index file"));

	ಪ್ರತ್ಯಾಗಮ ret;
}
