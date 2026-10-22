/*
 * "git rm" builtin command
 *
 * Copyright (C) Linus Torvalds 2006
 */

#define USE_THE_REPOSITORY_VARIABLE

#অন্তর্ভাৱ "builtin.h"
#অন্তর্ভাৱ "advice.h"
#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ "environment.h"
#অন্তর্ভাৱ "lockfile.h"
#অন্তর্ভাৱ "dir.h"
#অন্তর্ভাৱ "gettext.h"
#অন্তর্ভাৱ "hash.h"
#অন্তর্ভাৱ "tree-walk.h"
#অন্তর্ভাৱ "object-name.h"
#অন্তর্ভাৱ "parse-options.h"
#অন্তর্ভাৱ "read-cache.h"

#অন্তর্ভাৱ "string-list.h"
#অন্তর্ভাৱ "setup.h"
#অন্তর্ভাৱ "sparse-index.h"
#অন্তর্ভাৱ "submodule.h"
#অন্তর্ভাৱ "pathspec.h"

স্থির ধ্রুৱ ৱর্ণ * ধ্রুৱ builtin_rm_usage[] = {
	N_("git rm [-f | --force] [-n] [-r] [--cached] [--ignore-unmatch]\n"
	   "       [--quiet] [--pathspec-from-file=<file> [--pathspec-file-nul]]\n"
	   "       [--] [<pathspec>...]"),
	রিক্ত
};

স্থির সংরচনা {
	পূর্ণাঙ্ক nr, alloc;
	সংরচনা {
		ধ্রুৱ ৱর্ণ *name;
		ৱর্ণ is_submodule;
	} *entry;
} list;

স্থির পূর্ণাঙ্ক get_ours_cache_pos(ধ্রুৱ ৱর্ণ *path, unsigned পূর্ণাঙ্ক pos)
{
	যেতিয়ালৈ ((pos < the_repository->index->cache_nr) && !strcmp(the_repository->index->cache[pos]->name, path)) {
		যদি (ce_stage(the_repository->index->cache[pos]) == 2)
			ঘূৰাই pos;
		pos++;
	}
	ঘূৰাই -1;
}

স্থির শূন্য print_error_files(সংরচনা string_list *files_list,
			      ধ্রুৱ ৱর্ণ *main_বাৰ্তা,
			      ধ্রুৱ ৱর্ণ *hints_বাৰ্তা,
			      পূর্ণাঙ্ক *errs)
{
	যদি (files_list->nr) {
		unsigned পূর্ণাঙ্ক i;
		সংরচনা strbuf err_বাৰ্তা = STRBUF_INIT;

		strbuf_addstr(&err_বাৰ্তা, main_বাৰ্তা);
		বাবে (i = 0; i < files_list->nr; i++)
			strbuf_addf(&err_বাৰ্তা,
				    "\n    %s",
				    files_list->বস্তুসমূহ[i].string);
		যদি (advice_enabled(ADVICE_RM_HINTS))
			strbuf_addstr(&err_বাৰ্তা, hints_বাৰ্তা);
		*errs = error("%s", err_বাৰ্তা.buf);
		strbuf_release(&err_বাৰ্তা);
	}
}

স্থির শূন্য submodules_absorb_gitdir_if_needed(শূন্য)
{
	পূর্ণাঙ্ক i;
	বাবে (i = 0; i < list.nr; i++) {
		ধ্রুৱ ৱর্ণ *name = list.entry[i].name;
		পূর্ণাঙ্ক pos;
		ধ্রুৱ সংরচনা cache_entry *ce;

		pos = index_name_pos(the_repository->index, name, strlen(name));
		যদি (pos < 0) {
			pos = get_ours_cache_pos(name, -pos - 1);
			যদি (pos < 0)
				অনুৱর্ত;
		}
		ce = the_repository->index->cache[pos];

		যদি (!S_ISGITLINK(ce->ce_mode) ||
		    !file_exists(ce->name) ||
		    is_empty_dir(name))
			অনুৱর্ত;

		যদি (!submodule_uses_gitfile(name))
			absorb_git_dir_into_superproject(name, রিক্ত);
	}
}

স্থির পূর্ণাঙ্ক check_local_mod(সংরচনা object_id *head, পূর্ণাঙ্ক index_only)
{
	/*
	 * Items in list are already sorted in the cache order,
	 * so we could do this a lot more efficiently by using
	 * tree_desc based traversal if we wanted to, but I am
	 * lazy, and who cares if removal of files is a tad
	 * slower than the theoretical maximum speed?
	 */
	পূর্ণাঙ্ক i, no_head;
	পূর্ণাঙ্ক errs = 0;
	সংরচনা string_list files_staged = STRING_LIST_INIT_NODUP;
	সংরচনা string_list files_cached = STRING_LIST_INIT_NODUP;
	সংরচনা string_list files_local = STRING_LIST_INIT_NODUP;

	no_head = is_null_oid(head);
	বাবে (i = 0; i < list.nr; i++) {
		সংরচনা stat st;
		পূর্ণাঙ্ক pos;
		ধ্রুৱ সংরচনা cache_entry *ce;
		ধ্রুৱ ৱর্ণ *name = list.entry[i].name;
		সংরচনা object_id oid;
		unsigned short mode;
		পূর্ণাঙ্ক local_changes = 0;
		পূর্ণাঙ্ক staged_changes = 0;

		pos = index_name_pos(the_repository->index, name, strlen(name));
		যদি (pos < 0) {
			/*
			 * Skip unmerged entries except for populated submodules
			 * that could lose history when removed.
			 */
			pos = get_ours_cache_pos(name, -pos - 1);
			যদি (pos < 0)
				অনুৱর্ত;

			যদি (!S_ISGITLINK(the_repository->index->cache[pos]->ce_mode) ||
			    is_empty_dir(name))
				অনুৱর্ত;
		}
		ce = the_repository->index->cache[pos];

		যদি (lstat(ce->name, &st) < 0) {
			যদি (!is_missing_file_error(errno))
				warning_errno(_("failed to stat '%s'"), ce->name);
			/* It already vanished from the working tree */
			অনুৱর্ত;
		}
		অন্যথা যদি (S_ISDIR(st.st_mode)) {
			/* if a file was removed and it is now a
			 * directory, that is the same as ENOENT as
			 * far as git is concerned; we do not track
			 * directories unless they are submodules.
			 */
			যদি (!S_ISGITLINK(ce->ce_mode))
				অনুৱর্ত;
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
		যদি (ie_match_stat(the_repository->index, ce, &st, 0) ||
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
		যদি (no_head
		     || get_বৃক্ষ_entry(the_repository, head, name, &oid, &mode)
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
		যদি (local_changes && staged_changes) {
			যদি (!index_only || !ce_intent_to_add(ce))
				string_list_append(&files_staged, name);
		}
		অন্যথা যদি (!index_only) {
			যদি (staged_changes)
				string_list_append(&files_cached, name);
			যদি (local_changes)
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

	ঘূৰাই errs;
}

স্থির পূর্ণাঙ্ক show_only = 0, force = 0, index_only = 0, recursive = 0, quiet = 0;
স্থির পূর্ণাঙ্ক ignore_unmatch = 0, pathspec_file_nul;
স্থির পূর্ণাঙ্ক include_sparse;
স্থির ৱর্ণ *pathspec_from_file;

স্থির সংরচনা option builtin_rm_options[] = {
	OPT__DRY_RUN(&show_only, N_("dry run")),
	OPT__QUIET(&quiet, N_("do not list removed files")),
	OPT_BOOL( 0 , "cached",         &index_only, N_("only remove from the index")),
	OPT__FORCE(&force, N_("override the up-to-date check"), PARSE_OPT_NOCOMPLETE),
	OPT_BOOL('r', রিক্ত,             &recursive,  N_("allow recursive removal")),
	OPT_BOOL( 0 , "ignore-unmatch", &ignore_unmatch,
				N_("exit with a zero status even if nothing matched")),
	OPT_BOOL(0, "sparse", &include_sparse, N_("allow updating entries outside of the sparse-checkout cone")),
	OPT_PATHSPEC_FROM_FILE(&pathspec_from_file),
	OPT_PATHSPEC_FILE_NUL(&pathspec_file_nul),
	OPT_END(),
};

পূর্ণাঙ্ক cmd_rm(পূর্ণাঙ্ক argc,
	   ধ্রুৱ ৱর্ণ **argv,
	   ধ্রুৱ ৱর্ণ *prefix,
	   সংরচনা repository *repo UNUSED)
{
	সংরচনা lock_file lock_file = LOCK_INIT;
	পূর্ণাঙ্ক i, ret = 0;
	সংরচনা pathspec pathspec;
	ৱর্ণ *seen;

	repo_config(the_repository, git_default_config, রিক্ত);

	argc = parse_options(argc, argv, prefix, builtin_rm_options,
			     builtin_rm_usage, 0);

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD,
		       prefix, argv);

	যদি (pathspec_from_file) {
		যদি (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_CWD,
				    prefix, pathspec_from_file, pathspec_file_nul);
	} অন্যথা যদি (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	যদি (!pathspec.nr)
		die(_("No pathspec was given. Which files should I remove?"));

	যদি (!index_only)
		setup_work_বৃক্ষ(the_repository);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;
	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);

	যদি (repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));

	refresh_index(the_repository->index, REFRESH_QUIET|REFRESH_UNMERGED, &pathspec, রিক্ত, রিক্ত);

	seen = xcalloc(pathspec.nr, 1);

	যদি (pathspec_needs_expanded_index(the_repository->index, &pathspec))
		ensure_full_index(the_repository->index);

	বাবে (unsigned পূর্ণাঙ্ক i = 0; i < the_repository->index->cache_nr; i++) {
		ধ্রুৱ সংরচনা cache_entry *ce = the_repository->index->cache[i];

		যদি (!include_sparse &&
		    (ce_skip_worktree(ce) ||
		     !path_in_sparse_checkout(ce->name, the_repository->index)))
			অনুৱর্ত;
		যদি (!ce_path_match(the_repository->index, ce, &pathspec, seen))
			অনুৱর্ত;
		ALLOC_GROW(list.entry, list.nr + 1, list.alloc);
		list.entry[list.nr].name = xstrdup(ce->name);
		list.entry[list.nr].is_submodule = S_ISGITLINK(ce->ce_mode);
		যদি (list.entry[list.nr++].is_submodule &&
		    !is_staging_gitmodules_ok(the_repository->index))
			die(_("please stage your changes to .gitmodules or stash them to proceed"));
	}

	যদি (pathspec.nr) {
		ধ্রুৱ ৱর্ণ *original;
		পূর্ণাঙ্ক seen_any = 0;
		ৱর্ণ *skip_worktree_seen = রিক্ত;
		সংরচনা string_list only_match_skip_worktree = STRING_LIST_INIT_NODUP;

		বাবে (i = 0; i < pathspec.nr; i++) {
			original = pathspec.বস্তুসমূহ[i].original;
			যদি (seen[i])
				seen_any = 1;
			অন্যথা যদি (ignore_unmatch)
				অনুৱর্ত;
			অন্যথা যদি (!include_sparse &&
				 matches_skip_worktree(&pathspec, i, &skip_worktree_seen))
				string_list_append(&only_match_skip_worktree, original);
			অন্যথা
				die(_("pathspec '%s' did not match any files"), original);

			যদি (!recursive && seen[i] == MATCHED_RECURSIVELY)
				die(_("not removing '%s' recursively without -r"),
				    *original ? original : ".");
		}

		যদি (only_match_skip_worktree.nr) {
			advise_on_updating_sparse_paths(&only_match_skip_worktree);
			ret = 1;
		}
		মুক্ত(skip_worktree_seen);
		string_list_clear(&only_match_skip_worktree, 0);

		যদি (!seen_any)
			exit(ret);
	}
	clear_pathspec(&pathspec);
	মুক্ত(seen);

	যদি (!index_only)
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
	যদি (!force) {
		সংরচনা object_id oid;
		যদি (repo_get_oid(the_repository, "HEAD", &oid))
			oidclr(&oid, the_repository->hash_algo);
		যদি (check_local_mod(&oid, index_only))
			exit(1);
	}

	/*
	 * First remove the names from the index: we won't commit
	 * the index unless all of them succeed.
	 */
	বাবে (i = 0; i < list.nr; i++) {
		ধ্রুৱ ৱর্ণ *path = list.entry[i].name;
		যদি (!quiet)
			printf("rm '%s'\n", path);

		যদি (remove_file_from_index(the_repository->index, path))
			die(_("git rm: unable to remove %s"), path);
	}

	যদি (show_only)
		ঘূৰাই 0;

	/*
	 * Then, unless we used "--cached", remove the filenames from
	 * the workspace. If we fail to remove the first one, we
	 * abort the "git rm" (but once we've successfully removed
	 * any file at all, we'll go ahead and commit to it all:
	 * by then we've already committed ourselves and can't fail
	 * in the middle)
	 */
	যদি (!index_only) {
		পূর্ণাঙ্ক removed = 0, gitmodules_modified = 0;
		সংরচনা strbuf buf = STRBUF_INIT;
		পূর্ণাঙ্ক flag = force ? REMOVE_DIR_PURGE_ORIGINAL_CWD : 0;
		বাবে (i = 0; i < list.nr; i++) {
			ধ্রুৱ ৱর্ণ *path = list.entry[i].name;
			যদি (list.entry[i].is_submodule) {
				strbuf_reset(&buf);
				strbuf_addstr(&buf, path);
				যদি (remove_dir_recursively(&buf, flag))
					die(_("could not remove '%s'"), path);

				removed = 1;
				যদি (!remove_path_from_gitmodules(path))
					gitmodules_modified = 1;
				অনুৱর্ত;
			}
			যদি (!remove_path(path)) {
				removed = 1;
				অনুৱর্ত;
			}
			যদি (!removed)
				die_errno("git rm: '%s'", path);
		}
		strbuf_release(&buf);
		যদি (gitmodules_modified)
			stage_updated_gitmodules(the_repository->index);
	}

	যদি (write_locked_index(the_repository->index, &lock_file,
			       COMMIT_LOCK | SKIP_IF_UNCHANGED))
		die(_("Unable to write new index file"));

	ঘূৰাই ret;
}
