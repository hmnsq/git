/*
 * "git rm" builtin command
 *
 * Copyright (C) Linus Torvalds 2006
 */

#define USE_THE_REPOSITORY_VARIABLE

#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "advice.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "lockfile.h"
#ଅନ୍ତର୍ଭାଵ "dir.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "hash.h"
#ଅନ୍ତର୍ଭାଵ "tree-walk.h"
#ଅନ୍ତର୍ଭାଵ "object-name.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "read-cache.h"

#ଅନ୍ତର୍ଭାଵ "string-list.h"
#ଅନ୍ତର୍ଭାଵ "setup.h"
#ଅନ୍ତର୍ଭାଵ "sparse-index.h"
#ଅନ୍ତର୍ଭାଵ "submodule.h"
#ଅନ୍ତର୍ଭାଵ "pathspec.h"

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ builtin_rm_usage[] = {
	N_("git rm [-f | --force] [-n] [-r] [--cached] [--ignore-unmatch]\n"
	   "       [--quiet] [--pathspec-from-file=<file> [--pathspec-file-nul]]\n"
	   "       [--] [<pathspec>...]"),
	ଖାଲି
};

ସ୍ଥିର ସଂରଚନା {
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା nr, alloc;
	ସଂରଚନା {
		ଅଚଳ ଅକ୍ଷର *name;
		ଅକ୍ଷର is_submodule;
	} *entry;
} list;

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_ours_cache_pos(ଅଚଳ ଅକ୍ଷର *path, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା pos)
{
	ଯେତେବେଳେ ((pos < the_repository->index->cache_nr) && !strcmp(the_repository->index->cache[pos]->name, path)) {
		ଯଦି (ce_stage(the_repository->index->cache[pos]) == 2)
			ଫେରନ୍ତୁ pos;
		pos++;
	}
	ଫେରନ୍ତୁ -1;
}

ସ୍ଥିର ଶୂନ୍ୟ print_error_files(ସଂରଚନା string_list *files_list,
			      ଅଚଳ ଅକ୍ଷର *main_ସନ୍ଦେଶ,
			      ଅଚଳ ଅକ୍ଷର *hints_ସନ୍ଦେଶ,
			      ପୂର୍ଣ୍ଣସଂଖ୍ୟା *errs)
{
	ଯଦି (files_list->nr) {
		unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
		ସଂରଚନା strbuf err_ସନ୍ଦେଶ = STRBUF_INIT;

		strbuf_addstr(&err_ସନ୍ଦେଶ, main_ସନ୍ଦେଶ);
		ପାଇଁ (i = 0; i < files_list->nr; i++)
			strbuf_addf(&err_ସନ୍ଦେଶ,
				    "\n    %s",
				    files_list->ବସ୍ତୁଗୁଡ଼ିକ[i].string);
		ଯଦି (advice_enabled(ADVICE_RM_HINTS))
			strbuf_addstr(&err_ସନ୍ଦେଶ, hints_ସନ୍ଦେଶ);
		*errs = error("%s", err_ସନ୍ଦେଶ.buf);
		strbuf_release(&err_ସନ୍ଦେଶ);
	}
}

ସ୍ଥିର ଶୂନ୍ୟ submodules_absorb_gitdir_if_needed(ଶୂନ୍ୟ)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	ପାଇଁ (i = 0; i < list.nr; i++) {
		ଅଚଳ ଅକ୍ଷର *name = list.entry[i].name;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା pos;
		ଅଚଳ ସଂରଚନା cache_entry *ce;

		pos = index_name_pos(the_repository->index, name, strlen(name));
		ଯଦି (pos < 0) {
			pos = get_ours_cache_pos(name, -pos - 1);
			ଯଦି (pos < 0)
				ଜାରି;
		}
		ce = the_repository->index->cache[pos];

		ଯଦି (!S_ISGITLINK(ce->ce_mode) ||
		    !file_exists(ce->name) ||
		    is_empty_dir(name))
			ଜାରି;

		ଯଦି (!submodule_uses_gitfile(name))
			absorb_git_dir_into_superproject(name, ଖାଲି);
	}
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା check_local_mod(ସଂରଚନା object_id *head, ପୂର୍ଣ୍ଣସଂଖ୍ୟା index_only)
{
	/*
	 * Items in list are already sorted in the cache order,
	 * so we could do this a lot more efficiently by using
	 * tree_desc based traversal if we wanted to, but I am
	 * lazy, and who cares if removal of files is a tad
	 * slower than the theoretical maximum speed?
	 */
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, no_head;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା errs = 0;
	ସଂରଚନା string_list files_staged = STRING_LIST_INIT_NODUP;
	ସଂରଚନା string_list files_cached = STRING_LIST_INIT_NODUP;
	ସଂରଚନା string_list files_local = STRING_LIST_INIT_NODUP;

	no_head = is_null_oid(head);
	ପାଇଁ (i = 0; i < list.nr; i++) {
		ସଂରଚନା stat st;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା pos;
		ଅଚଳ ସଂରଚନା cache_entry *ce;
		ଅଚଳ ଅକ୍ଷର *name = list.entry[i].name;
		ସଂରଚନା object_id oid;
		unsigned short mode;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା local_changes = 0;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା staged_changes = 0;

		pos = index_name_pos(the_repository->index, name, strlen(name));
		ଯଦି (pos < 0) {
			/*
			 * Skip unmerged entries except for populated submodules
			 * that could lose history when removed.
			 */
			pos = get_ours_cache_pos(name, -pos - 1);
			ଯଦି (pos < 0)
				ଜାରି;

			ଯଦି (!S_ISGITLINK(the_repository->index->cache[pos]->ce_mode) ||
			    is_empty_dir(name))
				ଜାରି;
		}
		ce = the_repository->index->cache[pos];

		ଯଦି (lstat(ce->name, &st) < 0) {
			ଯଦି (!is_missing_file_error(errno))
				warning_errno(_("failed to stat '%s'"), ce->name);
			/* It already vanished from the working tree */
			ଜାରି;
		}
		ଅନ୍ୟଥା ଯଦି (S_ISDIR(st.st_mode)) {
			/* if a file was removed and it is now a
			 * directory, that is the same as ENOENT as
			 * far as git is concerned; we do not track
			 * directories unless they are submodules.
			 */
			ଯଦି (!S_ISGITLINK(ce->ce_mode))
				ଜାରି;
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
		ଯଦି (ie_match_stat(the_repository->index, ce, &st, 0) ||
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
		ଯଦି (no_head
		     || get_ବୃକ୍ଷ_entry(the_repository, head, name, &oid, &mode)
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
		ଯଦି (local_changes && staged_changes) {
			ଯଦି (!index_only || !ce_intent_to_add(ce))
				string_list_append(&files_staged, name);
		}
		ଅନ୍ୟଥା ଯଦି (!index_only) {
			ଯଦି (staged_changes)
				string_list_append(&files_cached, name);
			ଯଦି (local_changes)
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

	ଫେରନ୍ତୁ errs;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_only = 0, force = 0, index_only = 0, recursive = 0, quiet = 0;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା ignore_unmatch = 0, pathspec_file_nul;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା include_sparse;
ସ୍ଥିର ଅକ୍ଷର *pathspec_from_file;

ସ୍ଥିର ସଂରଚନା option builtin_rm_options[] = {
	OPT__DRY_RUN(&show_only, N_("dry run")),
	OPT__QUIET(&quiet, N_("do not list removed files")),
	OPT_BOOL( 0 , "cached",         &index_only, N_("only remove from the index")),
	OPT__FORCE(&force, N_("override the up-to-date check"), PARSE_OPT_NOCOMPLETE),
	OPT_BOOL('r', ଖାଲି,             &recursive,  N_("allow recursive removal")),
	OPT_BOOL( 0 , "ignore-unmatch", &ignore_unmatch,
				N_("exit with a zero status even if nothing matched")),
	OPT_BOOL(0, "sparse", &include_sparse, N_("allow updating entries outside of the sparse-checkout cone")),
	OPT_PATHSPEC_FROM_FILE(&pathspec_from_file),
	OPT_PATHSPEC_FILE_NUL(&pathspec_file_nul),
	OPT_END(),
};

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_rm(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
	   ଅଚଳ ଅକ୍ଷର **argv,
	   ଅଚଳ ଅକ୍ଷର *prefix,
	   ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା lock_file lock_file = LOCK_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, ret = 0;
	ସଂରଚନା pathspec pathspec;
	ଅକ୍ଷର *seen;

	repo_config(the_repository, git_default_config, ଖାଲି);

	argc = parse_options(argc, argv, prefix, builtin_rm_options,
			     builtin_rm_usage, 0);

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD,
		       prefix, argv);

	ଯଦି (pathspec_from_file) {
		ଯଦି (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_CWD,
				    prefix, pathspec_from_file, pathspec_file_nul);
	} ଅନ୍ୟଥା ଯଦି (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	ଯଦି (!pathspec.nr)
		die(_("No pathspec was given. Which files should I remove?"));

	ଯଦି (!index_only)
		setup_work_ବୃକ୍ଷ(the_repository);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;
	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);

	ଯଦି (repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));

	refresh_index(the_repository->index, REFRESH_QUIET|REFRESH_UNMERGED, &pathspec, ଖାଲି, ଖାଲି);

	seen = xcalloc(pathspec.nr, 1);

	ଯଦି (pathspec_needs_expanded_index(the_repository->index, &pathspec))
		ensure_full_index(the_repository->index);

	ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < the_repository->index->cache_nr; i++) {
		ଅଚଳ ସଂରଚନା cache_entry *ce = the_repository->index->cache[i];

		ଯଦି (!include_sparse &&
		    (ce_skip_worktree(ce) ||
		     !path_in_sparse_checkout(ce->name, the_repository->index)))
			ଜାରି;
		ଯଦି (!ce_path_match(the_repository->index, ce, &pathspec, seen))
			ଜାରି;
		ALLOC_GROW(list.entry, list.nr + 1, list.alloc);
		list.entry[list.nr].name = xstrdup(ce->name);
		list.entry[list.nr].is_submodule = S_ISGITLINK(ce->ce_mode);
		ଯଦି (list.entry[list.nr++].is_submodule &&
		    !is_staging_gitmodules_ok(the_repository->index))
			die(_("please stage your changes to .gitmodules or stash them to proceed"));
	}

	ଯଦି (pathspec.nr) {
		ଅଚଳ ଅକ୍ଷର *original;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା seen_any = 0;
		ଅକ୍ଷର *skip_worktree_seen = ଖାଲି;
		ସଂରଚନା string_list only_match_skip_worktree = STRING_LIST_INIT_NODUP;

		ପାଇଁ (i = 0; i < pathspec.nr; i++) {
			original = pathspec.ବସ୍ତୁଗୁଡ଼ିକ[i].original;
			ଯଦି (seen[i])
				seen_any = 1;
			ଅନ୍ୟଥା ଯଦି (ignore_unmatch)
				ଜାରି;
			ଅନ୍ୟଥା ଯଦି (!include_sparse &&
				 matches_skip_worktree(&pathspec, i, &skip_worktree_seen))
				string_list_append(&only_match_skip_worktree, original);
			ଅନ୍ୟଥା
				die(_("pathspec '%s' did not match any files"), original);

			ଯଦି (!recursive && seen[i] == MATCHED_RECURSIVELY)
				die(_("not removing '%s' recursively without -r"),
				    *original ? original : ".");
		}

		ଯଦି (only_match_skip_worktree.nr) {
			advise_on_updating_sparse_paths(&only_match_skip_worktree);
			ret = 1;
		}
		ମୁକ୍ତ(skip_worktree_seen);
		string_list_clear(&only_match_skip_worktree, 0);

		ଯଦି (!seen_any)
			exit(ret);
	}
	clear_pathspec(&pathspec);
	ମୁକ୍ତ(seen);

	ଯଦି (!index_only)
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
	ଯଦି (!force) {
		ସଂରଚନା object_id oid;
		ଯଦି (repo_get_oid(the_repository, "HEAD", &oid))
			oidclr(&oid, the_repository->hash_algo);
		ଯଦି (check_local_mod(&oid, index_only))
			exit(1);
	}

	/*
	 * First remove the names from the index: we won't commit
	 * the index unless all of them succeed.
	 */
	ପାଇଁ (i = 0; i < list.nr; i++) {
		ଅଚଳ ଅକ୍ଷର *path = list.entry[i].name;
		ଯଦି (!quiet)
			printf("rm '%s'\n", path);

		ଯଦି (remove_file_from_index(the_repository->index, path))
			die(_("git rm: unable to remove %s"), path);
	}

	ଯଦି (show_only)
		ଫେରନ୍ତୁ 0;

	/*
	 * Then, unless we used "--cached", remove the filenames from
	 * the workspace. If we fail to remove the first one, we
	 * abort the "git rm" (but once we've successfully removed
	 * any file at all, we'll go ahead and commit to it all:
	 * by then we've already committed ourselves and can't fail
	 * in the middle)
	 */
	ଯଦି (!index_only) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା removed = 0, gitmodules_modified = 0;
		ସଂରଚନା strbuf buf = STRBUF_INIT;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା flag = force ? REMOVE_DIR_PURGE_ORIGINAL_CWD : 0;
		ପାଇଁ (i = 0; i < list.nr; i++) {
			ଅଚଳ ଅକ୍ଷର *path = list.entry[i].name;
			ଯଦି (list.entry[i].is_submodule) {
				strbuf_reset(&buf);
				strbuf_addstr(&buf, path);
				ଯଦି (remove_dir_recursively(&buf, flag))
					die(_("could not remove '%s'"), path);

				removed = 1;
				ଯଦି (!remove_path_from_gitmodules(path))
					gitmodules_modified = 1;
				ଜାରି;
			}
			ଯଦି (!remove_path(path)) {
				removed = 1;
				ଜାରି;
			}
			ଯଦି (!removed)
				die_errno("git rm: '%s'", path);
		}
		strbuf_release(&buf);
		ଯଦି (gitmodules_modified)
			stage_updated_gitmodules(the_repository->index);
	}

	ଯଦି (write_locked_index(the_repository->index, &lock_file,
			       COMMIT_LOCK | SKIP_IF_UNCHANGED))
		die(_("Unable to write new index file"));

	ଫେରନ୍ତୁ ret;
}
