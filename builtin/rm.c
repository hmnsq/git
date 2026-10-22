/*
 * "git rm" builtin command
 *
 * Copyright (C) Linus Torvalds 2006
 */

#define USE_THE_REPOSITORY_VARIABLE

#شامل "builtin.h"
#شامل "advice.h"
#شامل "config.h"
#شامل "environment.h"
#شامل "lockfile.h"
#شامل "dir.h"
#شامل "gettext.h"
#شامل "hash.h"
#شامل "tree-walk.h"
#شامل "object-name.h"
#شامل "parse-options.h"
#شامل "read-cache.h"

#شامل "string-list.h"
#شامل "setup.h"
#شامل "sparse-index.h"
#شامل "submodule.h"
#شامل "pathspec.h"

ایستا ثابت نویسه * ثابت builtin_rm_usage[] = {
	N_("git rm [-f | --force] [-n] [-r] [--cached] [--ignore-unmatch]\n"
	   "       [--quiet] [--pathspec-from-file=<file> [--pathspec-file-nul]]\n"
	   "       [--] [<pathspec>...]"),
	پوچ
};

ایستا ساختار {
	صحیح nr, alloc;
	ساختار {
		ثابت نویسه *name;
		نویسه is_submodule;
	} *entry;
} list;

ایستا صحیح get_ours_cache_pos(ثابت نویسه *path, unsigned صحیح pos)
{
	وقتی ((pos < the_repository->index->cache_nr) && !strcmp(the_repository->index->cache[pos]->name, path)) {
		اگر (ce_stage(the_repository->index->cache[pos]) == 2)
			بازگشت pos;
		pos++;
	}
	بازگشت -1;
}

ایستا تهی print_error_files(ساختار string_list *files_list,
			      ثابت نویسه *main_پیام,
			      ثابت نویسه *hints_پیام,
			      صحیح *errs)
{
	اگر (files_list->nr) {
		unsigned صحیح i;
		ساختار strbuf err_پیام = STRBUF_INIT;

		strbuf_addstr(&err_پیام, main_پیام);
		برای (i = 0; i < files_list->nr; i++)
			strbuf_addf(&err_پیام,
				    "\n    %s",
				    files_list->اقلام[i].string);
		اگر (advice_enabled(ADVICE_RM_HINTS))
			strbuf_addstr(&err_پیام, hints_پیام);
		*errs = error("%s", err_پیام.buf);
		strbuf_release(&err_پیام);
	}
}

ایستا تهی submodules_absorb_gitdir_if_needed(تهی)
{
	صحیح i;
	برای (i = 0; i < list.nr; i++) {
		ثابت نویسه *name = list.entry[i].name;
		صحیح pos;
		ثابت ساختار cache_entry *ce;

		pos = index_name_pos(the_repository->index, name, strlen(name));
		اگر (pos < 0) {
			pos = get_ours_cache_pos(name, -pos - 1);
			اگر (pos < 0)
				ادامه;
		}
		ce = the_repository->index->cache[pos];

		اگر (!S_ISGITLINK(ce->ce_mode) ||
		    !file_exists(ce->name) ||
		    is_empty_dir(name))
			ادامه;

		اگر (!submodule_uses_gitfile(name))
			absorb_git_dir_into_superproject(name, پوچ);
	}
}

ایستا صحیح check_local_mod(ساختار object_id *head, صحیح index_only)
{
	/*
	 * Items in list are already sorted in the cache order,
	 * so we could do this a lot more efficiently by using
	 * tree_desc based traversal if we wanted to, but I am
	 * lazy, and who cares if removal of files is a tad
	 * slower than the theoretical maximum speed?
	 */
	صحیح i, no_head;
	صحیح errs = 0;
	ساختار string_list files_staged = STRING_LIST_INIT_NODUP;
	ساختار string_list files_cached = STRING_LIST_INIT_NODUP;
	ساختار string_list files_local = STRING_LIST_INIT_NODUP;

	no_head = is_null_oid(head);
	برای (i = 0; i < list.nr; i++) {
		ساختار stat st;
		صحیح pos;
		ثابت ساختار cache_entry *ce;
		ثابت نویسه *name = list.entry[i].name;
		ساختار object_id oid;
		unsigned short mode;
		صحیح local_changes = 0;
		صحیح staged_changes = 0;

		pos = index_name_pos(the_repository->index, name, strlen(name));
		اگر (pos < 0) {
			/*
			 * Skip unmerged entries except for populated submodules
			 * that could lose history when removed.
			 */
			pos = get_ours_cache_pos(name, -pos - 1);
			اگر (pos < 0)
				ادامه;

			اگر (!S_ISGITLINK(the_repository->index->cache[pos]->ce_mode) ||
			    is_empty_dir(name))
				ادامه;
		}
		ce = the_repository->index->cache[pos];

		اگر (lstat(ce->name, &st) < 0) {
			اگر (!is_missing_file_error(errno))
				warning_errno(_("failed to stat '%s'"), ce->name);
			/* It already vanished from the working tree */
			ادامه;
		}
		وگرنه اگر (S_ISDIR(st.st_mode)) {
			/* if a file was removed and it is now a
			 * directory, that is the same as ENOENT as
			 * far as git is concerned; we do not track
			 * directories unless they are submodules.
			 */
			اگر (!S_ISGITLINK(ce->ce_mode))
				ادامه;
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
		اگر (ie_match_stat(the_repository->index, ce, &st, 0) ||
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
		اگر (no_head
		     || get_درخت_entry(the_repository, head, name, &oid, &mode)
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
		اگر (local_changes && staged_changes) {
			اگر (!index_only || !ce_intent_to_add(ce))
				string_list_append(&files_staged, name);
		}
		وگرنه اگر (!index_only) {
			اگر (staged_changes)
				string_list_append(&files_cached, name);
			اگر (local_changes)
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

	بازگشت errs;
}

ایستا صحیح show_only = 0, force = 0, index_only = 0, recursive = 0, quiet = 0;
ایستا صحیح ignore_unmatch = 0, pathspec_file_nul;
ایستا صحیح include_sparse;
ایستا نویسه *pathspec_from_file;

ایستا ساختار option builtin_rm_options[] = {
	OPT__DRY_RUN(&show_only, N_("dry run")),
	OPT__QUIET(&quiet, N_("do not list removed files")),
	OPT_BOOL( 0 , "cached",         &index_only, N_("only remove from the index")),
	OPT__FORCE(&force, N_("override the up-to-date check"), PARSE_OPT_NOCOMPLETE),
	OPT_BOOL('r', پوچ,             &recursive,  N_("allow recursive removal")),
	OPT_BOOL( 0 , "ignore-unmatch", &ignore_unmatch,
				N_("exit with a zero status even if nothing matched")),
	OPT_BOOL(0, "sparse", &include_sparse, N_("allow updating entries outside of the sparse-checkout cone")),
	OPT_PATHSPEC_FROM_FILE(&pathspec_from_file),
	OPT_PATHSPEC_FILE_NUL(&pathspec_file_nul),
	OPT_END(),
};

صحیح cmd_rm(صحیح argc,
	   ثابت نویسه **argv,
	   ثابت نویسه *prefix,
	   ساختار repository *repo UNUSED)
{
	ساختار lock_file lock_file = LOCK_INIT;
	صحیح i, ret = 0;
	ساختار pathspec pathspec;
	نویسه *seen;

	repo_config(the_repository, git_default_config, پوچ);

	argc = parse_options(argc, argv, prefix, builtin_rm_options,
			     builtin_rm_usage, 0);

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD,
		       prefix, argv);

	اگر (pathspec_from_file) {
		اگر (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_CWD,
				    prefix, pathspec_from_file, pathspec_file_nul);
	} وگرنه اگر (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	اگر (!pathspec.nr)
		die(_("No pathspec was given. Which files should I remove?"));

	اگر (!index_only)
		setup_work_درخت(the_repository);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;
	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);

	اگر (repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));

	refresh_index(the_repository->index, REFRESH_QUIET|REFRESH_UNMERGED, &pathspec, پوچ, پوچ);

	seen = xcalloc(pathspec.nr, 1);

	اگر (pathspec_needs_expanded_index(the_repository->index, &pathspec))
		ensure_full_index(the_repository->index);

	برای (unsigned صحیح i = 0; i < the_repository->index->cache_nr; i++) {
		ثابت ساختار cache_entry *ce = the_repository->index->cache[i];

		اگر (!include_sparse &&
		    (ce_skip_worktree(ce) ||
		     !path_in_sparse_checkout(ce->name, the_repository->index)))
			ادامه;
		اگر (!ce_path_match(the_repository->index, ce, &pathspec, seen))
			ادامه;
		ALLOC_GROW(list.entry, list.nr + 1, list.alloc);
		list.entry[list.nr].name = xstrdup(ce->name);
		list.entry[list.nr].is_submodule = S_ISGITLINK(ce->ce_mode);
		اگر (list.entry[list.nr++].is_submodule &&
		    !is_staging_gitmodules_ok(the_repository->index))
			die(_("please stage your changes to .gitmodules or stash them to proceed"));
	}

	اگر (pathspec.nr) {
		ثابت نویسه *original;
		صحیح seen_any = 0;
		نویسه *skip_worktree_seen = پوچ;
		ساختار string_list only_match_skip_worktree = STRING_LIST_INIT_NODUP;

		برای (i = 0; i < pathspec.nr; i++) {
			original = pathspec.اقلام[i].original;
			اگر (seen[i])
				seen_any = 1;
			وگرنه اگر (ignore_unmatch)
				ادامه;
			وگرنه اگر (!include_sparse &&
				 matches_skip_worktree(&pathspec, i, &skip_worktree_seen))
				string_list_append(&only_match_skip_worktree, original);
			وگرنه
				die(_("pathspec '%s' did not match any files"), original);

			اگر (!recursive && seen[i] == MATCHED_RECURSIVELY)
				die(_("not removing '%s' recursively without -r"),
				    *original ? original : ".");
		}

		اگر (only_match_skip_worktree.nr) {
			advise_on_updating_sparse_paths(&only_match_skip_worktree);
			ret = 1;
		}
		آزاد(skip_worktree_seen);
		string_list_clear(&only_match_skip_worktree, 0);

		اگر (!seen_any)
			exit(ret);
	}
	clear_pathspec(&pathspec);
	آزاد(seen);

	اگر (!index_only)
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
	اگر (!force) {
		ساختار object_id oid;
		اگر (repo_get_oid(the_repository, "HEAD", &oid))
			oidclr(&oid, the_repository->hash_algo);
		اگر (check_local_mod(&oid, index_only))
			exit(1);
	}

	/*
	 * First remove the names from the index: we won't commit
	 * the index unless all of them succeed.
	 */
	برای (i = 0; i < list.nr; i++) {
		ثابت نویسه *path = list.entry[i].name;
		اگر (!quiet)
			printf("rm '%s'\n", path);

		اگر (remove_file_from_index(the_repository->index, path))
			die(_("git rm: unable to remove %s"), path);
	}

	اگر (show_only)
		بازگشت 0;

	/*
	 * Then, unless we used "--cached", remove the filenames from
	 * the workspace. If we fail to remove the first one, we
	 * abort the "git rm" (but once we've successfully removed
	 * any file at all, we'll go ahead and commit to it all:
	 * by then we've already committed ourselves and can't fail
	 * in the middle)
	 */
	اگر (!index_only) {
		صحیح removed = 0, gitmodules_modified = 0;
		ساختار strbuf buf = STRBUF_INIT;
		صحیح flag = force ? REMOVE_DIR_PURGE_ORIGINAL_CWD : 0;
		برای (i = 0; i < list.nr; i++) {
			ثابت نویسه *path = list.entry[i].name;
			اگر (list.entry[i].is_submodule) {
				strbuf_reset(&buf);
				strbuf_addstr(&buf, path);
				اگر (remove_dir_recursively(&buf, flag))
					die(_("could not remove '%s'"), path);

				removed = 1;
				اگر (!remove_path_from_gitmodules(path))
					gitmodules_modified = 1;
				ادامه;
			}
			اگر (!remove_path(path)) {
				removed = 1;
				ادامه;
			}
			اگر (!removed)
				die_errno("git rm: '%s'", path);
		}
		strbuf_release(&buf);
		اگر (gitmodules_modified)
			stage_updated_gitmodules(the_repository->index);
	}

	اگر (write_locked_index(the_repository->index, &lock_file,
			       COMMIT_LOCK | SKIP_IF_UNCHANGED))
		die(_("Unable to write new index file"));

	بازگشت ret;
}
