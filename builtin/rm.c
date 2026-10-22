/*
 * "git rm" builtin command
 *
 * Copyright (C) Linus Torvalds 2006
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "advice.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "lockfile.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "dir.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hash.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "tree-walk.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-name.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "read-cache.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "string-list.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "setup.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "sparse-index.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "submodule.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "pathspec.h"

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ builtin_rm_usage[] = {
	N_("git rm [-f | --force] [-n] [-r] [--cached] [--ignore-unmatch]\n"
	   "       [--quiet] [--pathspec-from-file=<file> [--pathspec-file-nul]]\n"
	   "       [--] [<pathspec>...]"),
	ਨਿਰਾ
};

ਸਥਿਰ ਬਣਤਰ {
	ਪੂਰਨ nr, alloc;
	ਬਣਤਰ {
		ਅਚਲ ਅੱਖਰ *name;
		ਅੱਖਰ is_submodule;
	} *ਇੰਦਰਾਜ;
} list;

ਸਥਿਰ ਪੂਰਨ get_ours_cache_pos(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pos)
{
	ਜਦੋਂ ((pos < the_repository->ਸੂਚਕਾਂਕ->cache_nr) && !strcmp(the_repository->ਸੂਚਕਾਂਕ->cache[pos]->name, ਰਸਤਾ)) {
		ਜੇ (ce_stage(the_repository->ਸੂਚਕਾਂਕ->cache[pos]) == 2)
			ਵਾਪਸ pos;
		pos++;
	}
	ਵਾਪਸ -1;
}

ਸਥਿਰ ਖਾਲੀ print_error_files(ਬਣਤਰ string_list *files_list,
			      ਅਚਲ ਅੱਖਰ *main_ਸੁਨੇਹਾ,
			      ਅਚਲ ਅੱਖਰ *hints_ਸੁਨੇਹਾ,
			      ਪੂਰਨ *errs)
{
	ਜੇ (files_list->nr) {
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i;
		ਬਣਤਰ strbuf err_ਸੁਨੇਹਾ = STRBUF_INIT;

		strbuf_addstr(&err_ਸੁਨੇਹਾ, main_ਸੁਨੇਹਾ);
		ਲਈ (i = 0; i < files_list->nr; i++)
			strbuf_addf(&err_ਸੁਨੇਹਾ,
				    "\n    %s",
				    files_list->ਚੀਜ਼ਾਂ[i].string);
		ਜੇ (advice_enabled(ADVICE_RM_HINTS))
			strbuf_addstr(&err_ਸੁਨੇਹਾ, hints_ਸੁਨੇਹਾ);
		*errs = error("%s", err_ਸੁਨੇਹਾ.buf);
		strbuf_release(&err_ਸੁਨੇਹਾ);
	}
}

ਸਥਿਰ ਖਾਲੀ submodules_absorb_gitdir_if_needed(ਖਾਲੀ)
{
	ਪੂਰਨ i;
	ਲਈ (i = 0; i < list.nr; i++) {
		ਅਚਲ ਅੱਖਰ *name = list.ਇੰਦਰਾਜ[i].name;
		ਪੂਰਨ pos;
		ਅਚਲ ਬਣਤਰ cache_ਇੰਦਰਾਜ *ce;

		pos = ਸੂਚਕਾਂਕ_name_pos(the_repository->ਸੂਚਕਾਂਕ, name, ਲੜੀ_ਲੰਬਾਈ(name));
		ਜੇ (pos < 0) {
			pos = get_ours_cache_pos(name, -pos - 1);
			ਜੇ (pos < 0)
				ਜਾਰੀ;
		}
		ce = the_repository->ਸੂਚਕਾਂਕ->cache[pos];

		ਜੇ (!S_ISGITLINK(ce->ce_ਬਹੁਲਕ) ||
		    !file_exists(ce->name) ||
		    is_ਖਾਲੀ_ਥਾਵਾਂ_dir(name))
			ਜਾਰੀ;

		ਜੇ (!submodule_ਵਰਤਦਾ_gitfile(name))
			absorb_git_dir_into_superproject(name, ਨਿਰਾ);
	}
}

ਸਥਿਰ ਪੂਰਨ check_local_mod(ਬਣਤਰ object_id *ਸਿਰ, ਪੂਰਨ ਸੂਚਕਾਂਕ_only)
{
	/*
	 * Items in list are already sorted in the cache order,
	 * so we could do this a lot more efficiently by using
	 * tree_desc based traversal if we wanted to, but I am
	 * lazy, and who cares if removal of files is a tad
	 * slower than the theoretical maximum speed?
	 */
	ਪੂਰਨ i, no_ਸਿਰ;
	ਪੂਰਨ errs = 0;
	ਬਣਤਰ string_list files_staged = STRING_LIST_INIT_NODUP;
	ਬਣਤਰ string_list files_cached = STRING_LIST_INIT_NODUP;
	ਬਣਤਰ string_list files_local = STRING_LIST_INIT_NODUP;

	no_ਸਿਰ = is_null_oid(ਸਿਰ);
	ਲਈ (i = 0; i < list.nr; i++) {
		ਬਣਤਰ stat st;
		ਪੂਰਨ pos;
		ਅਚਲ ਬਣਤਰ cache_ਇੰਦਰਾਜ *ce;
		ਅਚਲ ਅੱਖਰ *name = list.ਇੰਦਰਾਜ[i].name;
		ਬਣਤਰ object_id oid;
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਛੋਟਾ ਬਹੁਲਕ;
		ਪੂਰਨ local_changes = 0;
		ਪੂਰਨ staged_changes = 0;

		pos = ਸੂਚਕਾਂਕ_name_pos(the_repository->ਸੂਚਕਾਂਕ, name, ਲੜੀ_ਲੰਬਾਈ(name));
		ਜੇ (pos < 0) {
			/*
			 * Skip unmerged entries except for populated submodules
			 * that could lose history when removed.
			 */
			pos = get_ours_cache_pos(name, -pos - 1);
			ਜੇ (pos < 0)
				ਜਾਰੀ;

			ਜੇ (!S_ISGITLINK(the_repository->ਸੂਚਕਾਂਕ->cache[pos]->ce_ਬਹੁਲਕ) ||
			    is_ਖਾਲੀ_ਥਾਵਾਂ_dir(name))
				ਜਾਰੀ;
		}
		ce = the_repository->ਸੂਚਕਾਂਕ->cache[pos];

		ਜੇ (lstat(ce->name, &st) < 0) {
			ਜੇ (!is_missing_file_error(errno))
				warning_errno(_("failed to stat '%s'"), ce->name);
			/* It already vanished from the working tree */
			ਜਾਰੀ;
		}
		ਨਹੀਂਤਾਂ ਜੇ (S_ISDIR(st.st_ਬਹੁਲਕ)) {
			/* if a file was removed and it is now a
			 * directory, that is the same as ENOENT as
			 * far as git is concerned; we do not track
			 * directories unless they are submodules.
			 */
			ਜੇ (!S_ISGITLINK(ce->ce_ਬਹੁਲਕ))
				ਜਾਰੀ;
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
		ਜੇ (ie_ਮੇਲ_stat(the_repository->ਸੂਚਕਾਂਕ, ce, &st, 0) ||
		    (S_ISGITLINK(ce->ce_ਬਹੁਲਕ) &&
		     bad_ਮੰਜ਼ਿਲ_remove_submodule(ce->name,
				SUBMODULE_REMOVAL_DIE_ON_ERROR |
				SUBMODULE_REMOVAL_IGNORE_IGNORED_UNTRACKED)))
			local_changes = 1;

		/*
		 * Is the index different from the HEAD commit?  By
		 * definition, before the very initial commit,
		 * anything staged in the index is treated by the same
		 * way as changed from the HEAD.
		 */
		ਜੇ (no_ਸਿਰ
		     || get_ਰੁੱਖ_ਇੰਦਰਾਜ(the_repository, ਸਿਰ, name, &oid, &ਬਹੁਲਕ)
		     || ce->ce_ਬਹੁਲਕ != create_ce_ਬਹੁਲਕ(ਬਹੁਲਕ)
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
		ਜੇ (local_changes && staged_changes) {
			ਜੇ (!ਸੂਚਕਾਂਕ_only || !ce_intent_ਮੰਜ਼ਿਲ_ਜੋੜੋ(ce))
				string_list_append(&files_staged, name);
		}
		ਨਹੀਂਤਾਂ ਜੇ (!ਸੂਚਕਾਂਕ_only) {
			ਜੇ (staged_changes)
				string_list_append(&files_cached, name);
			ਜੇ (local_changes)
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

	ਵਾਪਸ errs;
}

ਸਥਿਰ ਪੂਰਨ show_only = 0, force = 0, ਸੂਚਕਾਂਕ_only = 0, recursive = 0, quiet = 0;
ਸਥਿਰ ਪੂਰਨ ignore_unmatch = 0, pathspec_file_nul;
ਸਥਿਰ ਪੂਰਨ include_sparse;
ਸਥਿਰ ਅੱਖਰ *pathspec_from_file;

ਸਥਿਰ ਬਣਤਰ option builtin_rm_options[] = {
	OPT_DRY_ਲੜੀਵਾਰ(&show_only, N_("dry run")),
	OPT__QUIET(&quiet, N_("do not list removed files")),
	OPT_BOOL( 0 , "cached",         &ਸੂਚਕਾਂਕ_only, N_("only remove from the index")),
	OPT__FORCE(&force, N_("override the up-to-date check"), PARSE_OPT_NOCOMPLETE),
	OPT_BOOL('r', ਨਿਰਾ,             &recursive,  N_("allow recursive removal")),
	OPT_BOOL( 0 , "ignore-unmatch", &ignore_unmatch,
				N_("exit with a zero status even if nothing matched")),
	OPT_BOOL(0, "sparse", &include_sparse, N_("allow updating entries outside of the sparse-checkout cone")),
	OPT_PATHSPEC_FROM_FILE(&pathspec_from_file),
	OPT_PATHSPEC_FILE_NUL(&pathspec_file_nul),
	OPT_ਸਿਰਾ(),
};

ਪੂਰਨ cmd_rm(ਪੂਰਨ argc,
	   ਅਚਲ ਅੱਖਰ **argv,
	   ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
	   ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ ਤਾਲਾ_file ਤਾਲਾ_file = ਤਾਲਾ_INIT;
	ਪੂਰਨ i, ret = 0;
	ਬਣਤਰ pathspec pathspec;
	ਅੱਖਰ *ਵੇਖਿਆ;

	repo_config(the_repository, git_default_config, ਨਿਰਾ);

	argc = parse_options(argc, argv, ਅਗੇਤਰ, builtin_rm_options,
			     builtin_rm_usage, 0);

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD,
		       ਅਗੇਤਰ, argv);

	ਜੇ (pathspec_from_file) {
		ਜੇ (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_CWD,
				    ਅਗੇਤਰ, pathspec_from_file, pathspec_file_nul);
	} ਨਹੀਂਤਾਂ ਜੇ (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	ਜੇ (!pathspec.nr)
		die(_("No pathspec was given. Which files should I remove?"));

	ਜੇ (!ਸੂਚਕਾਂਕ_only)
		ਤਿਆਰੀ_work_ਰੁੱਖ(the_repository);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_ਭਰਿਆ_ਸੂਚਕਾਂਕ = 0;
	repo_hold_locked_ਸੂਚਕਾਂਕ(the_repository, &ਤਾਲਾ_file, ਤਾਲਾ_DIE_ON_ERROR);

	ਜੇ (repo_read_ਸੂਚਕਾਂਕ(the_repository) < 0)
		die(_("index file corrupt"));

	refresh_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ, REFRESH_QUIET|REFRESH_UNMERGED, &pathspec, ਨਿਰਾ, ਨਿਰਾ);

	ਵੇਖਿਆ = xcalloc(pathspec.nr, 1);

	ਜੇ (pathspec_needs_expanded_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ, &pathspec))
		ensure_ਭਰਿਆ_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ);

	ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < the_repository->ਸੂਚਕਾਂਕ->cache_nr; i++) {
		ਅਚਲ ਬਣਤਰ cache_ਇੰਦਰਾਜ *ce = the_repository->ਸੂਚਕਾਂਕ->cache[i];

		ਜੇ (!include_sparse &&
		    (ce_skip_worktree(ce) ||
		     !ਰਸਤਾ_ਵਿੱਚ_sparse_checkout(ce->name, the_repository->ਸੂਚਕਾਂਕ)))
			ਜਾਰੀ;
		ਜੇ (!ce_ਰਸਤਾ_ਮੇਲ(the_repository->ਸੂਚਕਾਂਕ, ce, &pathspec, ਵੇਖਿਆ))
			ਜਾਰੀ;
		ALLOC_GROW(list.ਇੰਦਰਾਜ, list.nr + 1, list.alloc);
		list.ਇੰਦਰਾਜ[list.nr].name = xstrdup(ce->name);
		list.ਇੰਦਰਾਜ[list.nr].is_submodule = S_ISGITLINK(ce->ce_ਬਹੁਲਕ);
		ਜੇ (list.ਇੰਦਰਾਜ[list.nr++].is_submodule &&
		    !is_staging_gitmodules_ok(the_repository->ਸੂਚਕਾਂਕ))
			die(_("please stage your changes to .gitmodules or stash them to proceed"));
	}

	ਜੇ (pathspec.nr) {
		ਅਚਲ ਅੱਖਰ *original;
		ਪੂਰਨ ਵੇਖਿਆ_any = 0;
		ਅੱਖਰ *skip_worktree_ਵੇਖਿਆ = ਨਿਰਾ;
		ਬਣਤਰ string_list only_ਮੇਲ_skip_worktree = STRING_LIST_INIT_NODUP;

		ਲਈ (i = 0; i < pathspec.nr; i++) {
			original = pathspec.ਚੀਜ਼ਾਂ[i].original;
			ਜੇ (ਵੇਖਿਆ[i])
				ਵੇਖਿਆ_any = 1;
			ਨਹੀਂਤਾਂ ਜੇ (ignore_unmatch)
				ਜਾਰੀ;
			ਨਹੀਂਤਾਂ ਜੇ (!include_sparse &&
				 matches_skip_worktree(&pathspec, i, &skip_worktree_ਵੇਖਿਆ))
				string_list_append(&only_ਮੇਲ_skip_worktree, original);
			ਨਹੀਂਤਾਂ
				die(_("pathspec '%s' did not match any files"), original);

			ਜੇ (!recursive && ਵੇਖਿਆ[i] == MATCHED_RECURSIVELY)
				die(_("not removing '%s' recursively without -r"),
				    *original ? original : ".");
		}

		ਜੇ (only_ਮੇਲ_skip_worktree.nr) {
			advise_on_updating_sparse_paths(&only_ਮੇਲ_skip_worktree);
			ret = 1;
		}
		ਮੁਕਤ(skip_worktree_ਵੇਖਿਆ);
		string_list_clear(&only_ਮੇਲ_skip_worktree, 0);

		ਜੇ (!ਵੇਖਿਆ_any)
			ਬਾਹਰ_ਨਿਕਲੋ(ret);
	}
	clear_pathspec(&pathspec);
	ਮੁਕਤ(ਵੇਖਿਆ);

	ਜੇ (!ਸੂਚਕਾਂਕ_only)
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
	ਜੇ (!force) {
		ਬਣਤਰ object_id oid;
		ਜੇ (repo_get_oid(the_repository, "HEAD", &oid))
			oidclr(&oid, the_repository->hash_algo);
		ਜੇ (check_local_mod(&oid, ਸੂਚਕਾਂਕ_only))
			ਬਾਹਰ_ਨਿਕਲੋ(1);
	}

	/*
	 * First remove the names from the index: we won't commit
	 * the index unless all of them succeed.
	 */
	ਲਈ (i = 0; i < list.nr; i++) {
		ਅਚਲ ਅੱਖਰ *ਰਸਤਾ = list.ਇੰਦਰਾਜ[i].name;
		ਜੇ (!quiet)
			ਲਿਖੋ("rm '%s'\n", ਰਸਤਾ);

		ਜੇ (remove_file_from_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ, ਰਸਤਾ))
			die(_("git rm: unable to remove %s"), ਰਸਤਾ);
	}

	ਜੇ (show_only)
		ਵਾਪਸ 0;

	/*
	 * Then, unless we used "--cached", remove the filenames from
	 * the workspace. If we fail to remove the first one, we
	 * abort the "git rm" (but once we've successfully removed
	 * any file at all, we'll go ahead and commit to it all:
	 * by then we've already committed ourselves and can't fail
	 * in the middle)
	 */
	ਜੇ (!ਸੂਚਕਾਂਕ_only) {
		ਪੂਰਨ removed = 0, gitmodules_modified = 0;
		ਬਣਤਰ strbuf buf = STRBUF_INIT;
		ਪੂਰਨ flag = force ? REMOVE_DIR_PURGE_ORIGINAL_CWD : 0;
		ਲਈ (i = 0; i < list.nr; i++) {
			ਅਚਲ ਅੱਖਰ *ਰਸਤਾ = list.ਇੰਦਰਾਜ[i].name;
			ਜੇ (list.ਇੰਦਰਾਜ[i].is_submodule) {
				strbuf_reset(&buf);
				strbuf_addstr(&buf, ਰਸਤਾ);
				ਜੇ (remove_dir_recursively(&buf, flag))
					die(_("could not remove '%s'"), ਰਸਤਾ);

				removed = 1;
				ਜੇ (!remove_ਰਸਤਾ_from_gitmodules(ਰਸਤਾ))
					gitmodules_modified = 1;
				ਜਾਰੀ;
			}
			ਜੇ (!remove_ਰਸਤਾ(ਰਸਤਾ)) {
				removed = 1;
				ਜਾਰੀ;
			}
			ਜੇ (!removed)
				die_errno("git rm: '%s'", ਰਸਤਾ);
		}
		strbuf_release(&buf);
		ਜੇ (gitmodules_modified)
			stage_updated_gitmodules(the_repository->ਸੂਚਕਾਂਕ);
	}

	ਜੇ (write_locked_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ, &ਤਾਲਾ_file,
			       COMMIT_ਤਾਲਾ | SKIP_IF_UNCHANGED))
		die(_("Unable to write new index file"));

	ਵਾਪਸ ret;
}
