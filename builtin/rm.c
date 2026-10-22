/*
 * "git rm" builtin command
 *
 * Copyright (C) Linus Torvalds 2006
 */

#define USE_THE_REPOSITORY_VARIABLE

#ضمن "builtin.h"
#ضمن "advice.h"
#ضمن "config.h"
#ضمن "environment.h"
#ضمن "lockfile.h"
#ضمن "dir.h"
#ضمن "gettext.h"
#ضمن "hash.h"
#ضمن "tree-walk.h"
#ضمن "object-name.h"
#ضمن "parse-options.h"
#ضمن "read-cache.h"

#ضمن "string-list.h"
#ضمن "setup.h"
#ضمن "sparse-index.h"
#ضمن "submodule.h"
#ضمن "pathspec.h"

ساكن ثابت حرف * ثابت builtin_rm_usage[] = {
	N_("git rm [-f | --force] [-n] [-r] [--cached] [--ignore-unmatch]\n"
	   "       [--quiet] [--pathspec-from-file=<file> [--pathspec-file-nul]]\n"
	   "       [--] [<pathspec>...]"),
	عدم
};

ساكن بنية {
	صحيح nr, alloc;
	بنية {
		ثابت حرف *name;
		حرف is_submodule;
	} *entry;
} list;

ساكن صحيح get_ours_cache_pos(ثابت حرف *path, unsigned صحيح pos)
{
	بينما ((pos < the_repository->index->cache_nr) && !strcmp(the_repository->index->cache[pos]->name, path)) {
		إذا (ce_stage(the_repository->index->cache[pos]) == 2)
			إرجاع pos;
		pos++;
	}
	إرجاع -1;
}

ساكن فراغ print_error_files(بنية string_list *files_list,
			      ثابت حرف *main_رسالة,
			      ثابت حرف *hints_رسالة,
			      صحيح *errs)
{
	إذا (files_list->nr) {
		unsigned صحيح i;
		بنية strbuf err_رسالة = STRBUF_INIT;

		strbuf_addstr(&err_رسالة, main_رسالة);
		لكل (i = 0; i < files_list->nr; i++)
			strbuf_addf(&err_رسالة,
				    "\n    %s",
				    files_list->عناصر[i].string);
		إذا (advice_enabled(ADVICE_RM_HINTS))
			strbuf_addstr(&err_رسالة, hints_رسالة);
		*errs = error("%s", err_رسالة.buf);
		strbuf_release(&err_رسالة);
	}
}

ساكن فراغ submodules_absorb_gitdir_if_needed(فراغ)
{
	صحيح i;
	لكل (i = 0; i < list.nr; i++) {
		ثابت حرف *name = list.entry[i].name;
		صحيح pos;
		ثابت بنية cache_entry *ce;

		pos = index_name_pos(the_repository->index, name, strlen(name));
		إذا (pos < 0) {
			pos = get_ours_cache_pos(name, -pos - 1);
			إذا (pos < 0)
				استمر;
		}
		ce = the_repository->index->cache[pos];

		إذا (!S_ISGITLINK(ce->ce_mode) ||
		    !file_exists(ce->name) ||
		    is_empty_dir(name))
			استمر;

		إذا (!submodule_uses_gitfile(name))
			absorb_git_dir_into_superproject(name, عدم);
	}
}

ساكن صحيح check_local_mod(بنية object_id *head, صحيح index_only)
{
	/*
	 * Items in list are already sorted in the cache order,
	 * so we could do this a lot more efficiently by using
	 * tree_desc based traversal if we wanted to, but I am
	 * lazy, and who cares if removal of files is a tad
	 * slower than the theoretical maximum speed?
	 */
	صحيح i, no_head;
	صحيح errs = 0;
	بنية string_list files_staged = STRING_LIST_INIT_NODUP;
	بنية string_list files_cached = STRING_LIST_INIT_NODUP;
	بنية string_list files_local = STRING_LIST_INIT_NODUP;

	no_head = is_null_oid(head);
	لكل (i = 0; i < list.nr; i++) {
		بنية stat st;
		صحيح pos;
		ثابت بنية cache_entry *ce;
		ثابت حرف *name = list.entry[i].name;
		بنية object_id oid;
		unsigned short mode;
		صحيح local_changes = 0;
		صحيح staged_changes = 0;

		pos = index_name_pos(the_repository->index, name, strlen(name));
		إذا (pos < 0) {
			/*
			 * Skip unmerged entries except for populated submodules
			 * that could lose history when removed.
			 */
			pos = get_ours_cache_pos(name, -pos - 1);
			إذا (pos < 0)
				استمر;

			إذا (!S_ISGITLINK(the_repository->index->cache[pos]->ce_mode) ||
			    is_empty_dir(name))
				استمر;
		}
		ce = the_repository->index->cache[pos];

		إذا (lstat(ce->name, &st) < 0) {
			إذا (!is_missing_file_error(errno))
				warning_errno(_("failed to stat '%s'"), ce->name);
			/* It already vanished from the working tree */
			استمر;
		}
		وإلا إذا (S_ISDIR(st.st_mode)) {
			/* if a file was removed and it is now a
			 * directory, that is the same as ENOENT as
			 * far as git is concerned; we do not track
			 * directories unless they are submodules.
			 */
			إذا (!S_ISGITLINK(ce->ce_mode))
				استمر;
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
		إذا (ie_match_stat(the_repository->index, ce, &st, 0) ||
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
		إذا (no_head
		     || get_شجرة_entry(the_repository, head, name, &oid, &mode)
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
		إذا (local_changes && staged_changes) {
			إذا (!index_only || !ce_intent_to_add(ce))
				string_list_append(&files_staged, name);
		}
		وإلا إذا (!index_only) {
			إذا (staged_changes)
				string_list_append(&files_cached, name);
			إذا (local_changes)
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

	إرجاع errs;
}

ساكن صحيح show_only = 0, force = 0, index_only = 0, recursive = 0, quiet = 0;
ساكن صحيح ignore_unmatch = 0, pathspec_file_nul;
ساكن صحيح include_sparse;
ساكن حرف *pathspec_from_file;

ساكن بنية option builtin_rm_options[] = {
	OPT__DRY_RUN(&show_only, N_("dry run")),
	OPT__QUIET(&quiet, N_("do not list removed files")),
	OPT_BOOL( 0 , "cached",         &index_only, N_("only remove from the index")),
	OPT__FORCE(&force, N_("override the up-to-date check"), PARSE_OPT_NOCOMPLETE),
	OPT_BOOL('r', عدم,             &recursive,  N_("allow recursive removal")),
	OPT_BOOL( 0 , "ignore-unmatch", &ignore_unmatch,
				N_("exit with a zero status even if nothing matched")),
	OPT_BOOL(0, "sparse", &include_sparse, N_("allow updating entries outside of the sparse-checkout cone")),
	OPT_PATHSPEC_FROM_FILE(&pathspec_from_file),
	OPT_PATHSPEC_FILE_NUL(&pathspec_file_nul),
	OPT_END(),
};

صحيح cmd_rm(صحيح argc,
	   ثابت حرف **argv,
	   ثابت حرف *prefix,
	   بنية repository *repo UNUSED)
{
	بنية lock_file lock_file = LOCK_INIT;
	صحيح i, ret = 0;
	بنية pathspec pathspec;
	حرف *seen;

	repo_config(the_repository, git_default_config, عدم);

	argc = parse_options(argc, argv, prefix, builtin_rm_options,
			     builtin_rm_usage, 0);

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD,
		       prefix, argv);

	إذا (pathspec_from_file) {
		إذا (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_CWD,
				    prefix, pathspec_from_file, pathspec_file_nul);
	} وإلا إذا (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	إذا (!pathspec.nr)
		die(_("No pathspec was given. Which files should I remove?"));

	إذا (!index_only)
		setup_work_شجرة(the_repository);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;
	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);

	إذا (repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));

	refresh_index(the_repository->index, REFRESH_QUIET|REFRESH_UNMERGED, &pathspec, عدم, عدم);

	seen = xcalloc(pathspec.nr, 1);

	إذا (pathspec_needs_expanded_index(the_repository->index, &pathspec))
		ensure_full_index(the_repository->index);

	لكل (unsigned صحيح i = 0; i < the_repository->index->cache_nr; i++) {
		ثابت بنية cache_entry *ce = the_repository->index->cache[i];

		إذا (!include_sparse &&
		    (ce_skip_worktree(ce) ||
		     !path_in_sparse_checkout(ce->name, the_repository->index)))
			استمر;
		إذا (!ce_path_match(the_repository->index, ce, &pathspec, seen))
			استمر;
		ALLOC_GROW(list.entry, list.nr + 1, list.alloc);
		list.entry[list.nr].name = xstrdup(ce->name);
		list.entry[list.nr].is_submodule = S_ISGITLINK(ce->ce_mode);
		إذا (list.entry[list.nr++].is_submodule &&
		    !is_staging_gitmodules_ok(the_repository->index))
			die(_("please stage your changes to .gitmodules or stash them to proceed"));
	}

	إذا (pathspec.nr) {
		ثابت حرف *original;
		صحيح seen_any = 0;
		حرف *skip_worktree_seen = عدم;
		بنية string_list only_match_skip_worktree = STRING_LIST_INIT_NODUP;

		لكل (i = 0; i < pathspec.nr; i++) {
			original = pathspec.عناصر[i].original;
			إذا (seen[i])
				seen_any = 1;
			وإلا إذا (ignore_unmatch)
				استمر;
			وإلا إذا (!include_sparse &&
				 matches_skip_worktree(&pathspec, i, &skip_worktree_seen))
				string_list_append(&only_match_skip_worktree, original);
			وإلا
				die(_("pathspec '%s' did not match any files"), original);

			إذا (!recursive && seen[i] == MATCHED_RECURSIVELY)
				die(_("not removing '%s' recursively without -r"),
				    *original ? original : ".");
		}

		إذا (only_match_skip_worktree.nr) {
			advise_on_updating_sparse_paths(&only_match_skip_worktree);
			ret = 1;
		}
		حرر(skip_worktree_seen);
		string_list_clear(&only_match_skip_worktree, 0);

		إذا (!seen_any)
			exit(ret);
	}
	clear_pathspec(&pathspec);
	حرر(seen);

	إذا (!index_only)
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
	إذا (!force) {
		بنية object_id oid;
		إذا (repo_get_oid(the_repository, "HEAD", &oid))
			oidclr(&oid, the_repository->hash_algo);
		إذا (check_local_mod(&oid, index_only))
			exit(1);
	}

	/*
	 * First remove the names from the index: we won't commit
	 * the index unless all of them succeed.
	 */
	لكل (i = 0; i < list.nr; i++) {
		ثابت حرف *path = list.entry[i].name;
		إذا (!quiet)
			printf("rm '%s'\n", path);

		إذا (remove_file_from_index(the_repository->index, path))
			die(_("git rm: unable to remove %s"), path);
	}

	إذا (show_only)
		إرجاع 0;

	/*
	 * Then, unless we used "--cached", remove the filenames from
	 * the workspace. If we fail to remove the first one, we
	 * abort the "git rm" (but once we've successfully removed
	 * any file at all, we'll go ahead and commit to it all:
	 * by then we've already committed ourselves and can't fail
	 * in the middle)
	 */
	إذا (!index_only) {
		صحيح removed = 0, gitmodules_modified = 0;
		بنية strbuf buf = STRBUF_INIT;
		صحيح flag = force ? REMOVE_DIR_PURGE_ORIGINAL_CWD : 0;
		لكل (i = 0; i < list.nr; i++) {
			ثابت حرف *path = list.entry[i].name;
			إذا (list.entry[i].is_submodule) {
				strbuf_reset(&buf);
				strbuf_addstr(&buf, path);
				إذا (remove_dir_recursively(&buf, flag))
					die(_("could not remove '%s'"), path);

				removed = 1;
				إذا (!remove_path_from_gitmodules(path))
					gitmodules_modified = 1;
				استمر;
			}
			إذا (!remove_path(path)) {
				removed = 1;
				استمر;
			}
			إذا (!removed)
				die_errno("git rm: '%s'", path);
		}
		strbuf_release(&buf);
		إذا (gitmodules_modified)
			stage_updated_gitmodules(the_repository->index);
	}

	إذا (write_locked_index(the_repository->index, &lock_file,
			       COMMIT_LOCK | SKIP_IF_UNCHANGED))
		die(_("Unable to write new index file"));

	إرجاع ret;
}
