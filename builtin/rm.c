/*
 * "git rm" builtin command
 *
 * Copyright (C) Linus Torvalds 2006
 */

#define USE_THE_REPOSITORY_VARIABLE

#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "advice.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "lockfile.h"
#அந்தர்பாவ "dir.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "hash.h"
#அந்தர்பாவ "tree-walk.h"
#அந்தர்பாவ "object-name.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "read-cache.h"

#அந்தர்பாவ "string-list.h"
#அந்தர்பாவ "setup.h"
#அந்தர்பாவ "sparse-index.h"
#அந்தர்பாவ "submodule.h"
#அந்தர்பாவ "pathspec.h"

நிலையான மாறிலி எழுத்து * மாறிலி builtin_rm_usage[] = {
	N_("git rm [-f | --force] [-n] [-r] [--cached] [--ignore-unmatch]\n"
	   "       [--quiet] [--pathspec-from-file=<file> [--pathspec-file-nul]]\n"
	   "       [--] [<pathspec>...]"),
	வெற்று
};

நிலையான கட்டமைப்பு {
	முழு nr, alloc;
	கட்டமைப்பு {
		மாறிலி எழுத்து *name;
		எழுத்து is_submodule;
	} *entry;
} list;

நிலையான முழு get_ours_cache_pos(மாறிலி எழுத்து *path, unsigned முழு pos)
{
	போது ((pos < the_repository->index->cache_nr) && !strcmp(the_repository->index->cache[pos]->name, path)) {
		எனில் (ce_stage(the_repository->index->cache[pos]) == 2)
			திரும்பு pos;
		pos++;
	}
	திரும்பு -1;
}

நிலையான வெறுமை print_error_files(கட்டமைப்பு string_list *files_list,
			      மாறிலி எழுத்து *main_செய்தி,
			      மாறிலி எழுத்து *hints_செய்தி,
			      முழு *errs)
{
	எனில் (files_list->nr) {
		unsigned முழு i;
		கட்டமைப்பு strbuf err_செய்தி = STRBUF_INIT;

		strbuf_addstr(&err_செய்தி, main_செய்தி);
		க்கு (i = 0; i < files_list->nr; i++)
			strbuf_addf(&err_செய்தி,
				    "\n    %s",
				    files_list->பொருட்கள்[i].string);
		எனில் (advice_enabled(ADVICE_RM_HINTS))
			strbuf_addstr(&err_செய்தி, hints_செய்தி);
		*errs = error("%s", err_செய்தி.buf);
		strbuf_release(&err_செய்தி);
	}
}

நிலையான வெறுமை submodules_absorb_gitdir_if_needed(வெறுமை)
{
	முழு i;
	க்கு (i = 0; i < list.nr; i++) {
		மாறிலி எழுத்து *name = list.entry[i].name;
		முழு pos;
		மாறிலி கட்டமைப்பு cache_entry *ce;

		pos = index_name_pos(the_repository->index, name, strlen(name));
		எனில் (pos < 0) {
			pos = get_ours_cache_pos(name, -pos - 1);
			எனில் (pos < 0)
				தொடர்;
		}
		ce = the_repository->index->cache[pos];

		எனில் (!S_ISGITLINK(ce->ce_mode) ||
		    !file_exists(ce->name) ||
		    is_empty_dir(name))
			தொடர்;

		எனில் (!submodule_uses_gitfile(name))
			absorb_git_dir_into_superproject(name, வெற்று);
	}
}

நிலையான முழு check_local_mod(கட்டமைப்பு object_id *head, முழு index_only)
{
	/*
	 * Items in list are already sorted in the cache order,
	 * so we could do this a lot more efficiently by using
	 * tree_desc based traversal if we wanted to, but I am
	 * lazy, and who cares if removal of files is a tad
	 * slower than the theoretical maximum speed?
	 */
	முழு i, no_head;
	முழு errs = 0;
	கட்டமைப்பு string_list files_staged = STRING_LIST_INIT_NODUP;
	கட்டமைப்பு string_list files_cached = STRING_LIST_INIT_NODUP;
	கட்டமைப்பு string_list files_local = STRING_LIST_INIT_NODUP;

	no_head = is_null_oid(head);
	க்கு (i = 0; i < list.nr; i++) {
		கட்டமைப்பு stat st;
		முழு pos;
		மாறிலி கட்டமைப்பு cache_entry *ce;
		மாறிலி எழுத்து *name = list.entry[i].name;
		கட்டமைப்பு object_id oid;
		unsigned short mode;
		முழு local_changes = 0;
		முழு staged_changes = 0;

		pos = index_name_pos(the_repository->index, name, strlen(name));
		எனில் (pos < 0) {
			/*
			 * Skip unmerged entries except for populated submodules
			 * that could lose history when removed.
			 */
			pos = get_ours_cache_pos(name, -pos - 1);
			எனில் (pos < 0)
				தொடர்;

			எனில் (!S_ISGITLINK(the_repository->index->cache[pos]->ce_mode) ||
			    is_empty_dir(name))
				தொடர்;
		}
		ce = the_repository->index->cache[pos];

		எனில் (lstat(ce->name, &st) < 0) {
			எனில் (!is_missing_file_error(errno))
				warning_errno(_("failed to stat '%s'"), ce->name);
			/* It already vanished from the working tree */
			தொடர்;
		}
		இல்லையெனில் எனில் (S_ISDIR(st.st_mode)) {
			/* if a file was removed and it is now a
			 * directory, that is the same as ENOENT as
			 * far as git is concerned; we do not track
			 * directories unless they are submodules.
			 */
			எனில் (!S_ISGITLINK(ce->ce_mode))
				தொடர்;
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
		எனில் (ie_match_stat(the_repository->index, ce, &st, 0) ||
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
		எனில் (no_head
		     || get_மரம்_entry(the_repository, head, name, &oid, &mode)
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
		எனில் (local_changes && staged_changes) {
			எனில் (!index_only || !ce_intent_to_add(ce))
				string_list_append(&files_staged, name);
		}
		இல்லையெனில் எனில் (!index_only) {
			எனில் (staged_changes)
				string_list_append(&files_cached, name);
			எனில் (local_changes)
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

	திரும்பு errs;
}

நிலையான முழு show_only = 0, force = 0, index_only = 0, recursive = 0, quiet = 0;
நிலையான முழு ignore_unmatch = 0, pathspec_file_nul;
நிலையான முழு include_sparse;
நிலையான எழுத்து *pathspec_from_file;

நிலையான கட்டமைப்பு option builtin_rm_options[] = {
	OPT__DRY_RUN(&show_only, N_("dry run")),
	OPT__QUIET(&quiet, N_("do not list removed files")),
	OPT_BOOL( 0 , "cached",         &index_only, N_("only remove from the index")),
	OPT__FORCE(&force, N_("override the up-to-date check"), PARSE_OPT_NOCOMPLETE),
	OPT_BOOL('r', வெற்று,             &recursive,  N_("allow recursive removal")),
	OPT_BOOL( 0 , "ignore-unmatch", &ignore_unmatch,
				N_("exit with a zero status even if nothing matched")),
	OPT_BOOL(0, "sparse", &include_sparse, N_("allow updating entries outside of the sparse-checkout cone")),
	OPT_PATHSPEC_FROM_FILE(&pathspec_from_file),
	OPT_PATHSPEC_FILE_NUL(&pathspec_file_nul),
	OPT_END(),
};

முழு cmd_rm(முழு argc,
	   மாறிலி எழுத்து **argv,
	   மாறிலி எழுத்து *prefix,
	   கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு lock_file lock_file = LOCK_INIT;
	முழு i, ret = 0;
	கட்டமைப்பு pathspec pathspec;
	எழுத்து *seen;

	repo_config(the_repository, git_default_config, வெற்று);

	argc = parse_options(argc, argv, prefix, builtin_rm_options,
			     builtin_rm_usage, 0);

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD,
		       prefix, argv);

	எனில் (pathspec_from_file) {
		எனில் (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_CWD,
				    prefix, pathspec_from_file, pathspec_file_nul);
	} இல்லையெனில் எனில் (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	எனில் (!pathspec.nr)
		die(_("No pathspec was given. Which files should I remove?"));

	எனில் (!index_only)
		setup_work_மரம்(the_repository);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;
	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);

	எனில் (repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));

	refresh_index(the_repository->index, REFRESH_QUIET|REFRESH_UNMERGED, &pathspec, வெற்று, வெற்று);

	seen = xcalloc(pathspec.nr, 1);

	எனில் (pathspec_needs_expanded_index(the_repository->index, &pathspec))
		ensure_full_index(the_repository->index);

	க்கு (unsigned முழு i = 0; i < the_repository->index->cache_nr; i++) {
		மாறிலி கட்டமைப்பு cache_entry *ce = the_repository->index->cache[i];

		எனில் (!include_sparse &&
		    (ce_skip_worktree(ce) ||
		     !path_in_sparse_checkout(ce->name, the_repository->index)))
			தொடர்;
		எனில் (!ce_path_match(the_repository->index, ce, &pathspec, seen))
			தொடர்;
		ALLOC_GROW(list.entry, list.nr + 1, list.alloc);
		list.entry[list.nr].name = xstrdup(ce->name);
		list.entry[list.nr].is_submodule = S_ISGITLINK(ce->ce_mode);
		எனில் (list.entry[list.nr++].is_submodule &&
		    !is_staging_gitmodules_ok(the_repository->index))
			die(_("please stage your changes to .gitmodules or stash them to proceed"));
	}

	எனில் (pathspec.nr) {
		மாறிலி எழுத்து *original;
		முழு seen_any = 0;
		எழுத்து *skip_worktree_seen = வெற்று;
		கட்டமைப்பு string_list only_match_skip_worktree = STRING_LIST_INIT_NODUP;

		க்கு (i = 0; i < pathspec.nr; i++) {
			original = pathspec.பொருட்கள்[i].original;
			எனில் (seen[i])
				seen_any = 1;
			இல்லையெனில் எனில் (ignore_unmatch)
				தொடர்;
			இல்லையெனில் எனில் (!include_sparse &&
				 matches_skip_worktree(&pathspec, i, &skip_worktree_seen))
				string_list_append(&only_match_skip_worktree, original);
			இல்லையெனில்
				die(_("pathspec '%s' did not match any files"), original);

			எனில் (!recursive && seen[i] == MATCHED_RECURSIVELY)
				die(_("not removing '%s' recursively without -r"),
				    *original ? original : ".");
		}

		எனில் (only_match_skip_worktree.nr) {
			advise_on_updating_sparse_paths(&only_match_skip_worktree);
			ret = 1;
		}
		விடுவி(skip_worktree_seen);
		string_list_clear(&only_match_skip_worktree, 0);

		எனில் (!seen_any)
			exit(ret);
	}
	clear_pathspec(&pathspec);
	விடுவி(seen);

	எனில் (!index_only)
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
	எனில் (!force) {
		கட்டமைப்பு object_id oid;
		எனில் (repo_get_oid(the_repository, "HEAD", &oid))
			oidclr(&oid, the_repository->hash_algo);
		எனில் (check_local_mod(&oid, index_only))
			exit(1);
	}

	/*
	 * First remove the names from the index: we won't commit
	 * the index unless all of them succeed.
	 */
	க்கு (i = 0; i < list.nr; i++) {
		மாறிலி எழுத்து *path = list.entry[i].name;
		எனில் (!quiet)
			printf("rm '%s'\n", path);

		எனில் (remove_file_from_index(the_repository->index, path))
			die(_("git rm: unable to remove %s"), path);
	}

	எனில் (show_only)
		திரும்பு 0;

	/*
	 * Then, unless we used "--cached", remove the filenames from
	 * the workspace. If we fail to remove the first one, we
	 * abort the "git rm" (but once we've successfully removed
	 * any file at all, we'll go ahead and commit to it all:
	 * by then we've already committed ourselves and can't fail
	 * in the middle)
	 */
	எனில் (!index_only) {
		முழு removed = 0, gitmodules_modified = 0;
		கட்டமைப்பு strbuf buf = STRBUF_INIT;
		முழு flag = force ? REMOVE_DIR_PURGE_ORIGINAL_CWD : 0;
		க்கு (i = 0; i < list.nr; i++) {
			மாறிலி எழுத்து *path = list.entry[i].name;
			எனில் (list.entry[i].is_submodule) {
				strbuf_reset(&buf);
				strbuf_addstr(&buf, path);
				எனில் (remove_dir_recursively(&buf, flag))
					die(_("could not remove '%s'"), path);

				removed = 1;
				எனில் (!remove_path_from_gitmodules(path))
					gitmodules_modified = 1;
				தொடர்;
			}
			எனில் (!remove_path(path)) {
				removed = 1;
				தொடர்;
			}
			எனில் (!removed)
				die_errno("git rm: '%s'", path);
		}
		strbuf_release(&buf);
		எனில் (gitmodules_modified)
			stage_updated_gitmodules(the_repository->index);
	}

	எனில் (write_locked_index(the_repository->index, &lock_file,
			       COMMIT_LOCK | SKIP_IF_UNCHANGED))
		die(_("Unable to write new index file"));

	திரும்பு ret;
}
