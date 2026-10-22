/*
 * "git rm" builtin command
 *
 * Copyright (C) Linus Torvalds 2006
 */

#define USE_THE_REPOSITORY_VARIABLE

#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "advice.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "lockfile.h"
#അന്തര്ഭാവ "dir.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "hash.h"
#അന്തര്ഭാവ "tree-walk.h"
#അന്തര്ഭാവ "object-name.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "read-cache.h"

#അന്തര്ഭാവ "string-list.h"
#അന്തര്ഭാവ "setup.h"
#അന്തര്ഭാവ "sparse-index.h"
#അന്തര്ഭാവ "submodule.h"
#അന്തര്ഭാവ "pathspec.h"

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം builtin_rm_usage[] = {
	N_("git rm [-f | --force] [-n] [-r] [--cached] [--ignore-unmatch]\n"
	   "       [--quiet] [--pathspec-from-file=<file> [--pathspec-file-nul]]\n"
	   "       [--] [<pathspec>...]"),
	ശൂന്യ
};

സ്ഥിതം ഘടന {
	പൂർണ്ണസംഖ്യ nr, alloc;
	ഘടന {
		സ്ഥിരം അക്ഷരം *name;
		അക്ഷരം is_submodule;
	} *entry;
} list;

സ്ഥിതം പൂർണ്ണസംഖ്യ get_ours_cache_pos(സ്ഥിരം അക്ഷരം *path, unsigned പൂർണ്ണസംഖ്യ pos)
{
	ആകുമ്പോൾ ((pos < the_repository->index->cache_nr) && !strcmp(the_repository->index->cache[pos]->name, path)) {
		എങ്കിൽ (ce_stage(the_repository->index->cache[pos]) == 2)
			തിരികെ pos;
		pos++;
	}
	തിരികെ -1;
}

സ്ഥിതം ശൂന്യം print_error_files(ഘടന string_list *files_list,
			      സ്ഥിരം അക്ഷരം *main_സന്ദേശം,
			      സ്ഥിരം അക്ഷരം *hints_സന്ദേശം,
			      പൂർണ്ണസംഖ്യ *errs)
{
	എങ്കിൽ (files_list->nr) {
		unsigned പൂർണ്ണസംഖ്യ i;
		ഘടന strbuf err_സന്ദേശം = STRBUF_INIT;

		strbuf_addstr(&err_സന്ദേശം, main_സന്ദേശം);
		വേണ്ടി (i = 0; i < files_list->nr; i++)
			strbuf_addf(&err_സന്ദേശം,
				    "\n    %s",
				    files_list->ഇനങ്ങൾ[i].string);
		എങ്കിൽ (advice_enabled(ADVICE_RM_HINTS))
			strbuf_addstr(&err_സന്ദേശം, hints_സന്ദേശം);
		*errs = error("%s", err_സന്ദേശം.buf);
		strbuf_release(&err_സന്ദേശം);
	}
}

സ്ഥിതം ശൂന്യം submodules_absorb_gitdir_if_needed(ശൂന്യം)
{
	പൂർണ്ണസംഖ്യ i;
	വേണ്ടി (i = 0; i < list.nr; i++) {
		സ്ഥിരം അക്ഷരം *name = list.entry[i].name;
		പൂർണ്ണസംഖ്യ pos;
		സ്ഥിരം ഘടന cache_entry *ce;

		pos = index_name_pos(the_repository->index, name, strlen(name));
		എങ്കിൽ (pos < 0) {
			pos = get_ours_cache_pos(name, -pos - 1);
			എങ്കിൽ (pos < 0)
				തുടരുക;
		}
		ce = the_repository->index->cache[pos];

		എങ്കിൽ (!S_ISGITLINK(ce->ce_mode) ||
		    !file_exists(ce->name) ||
		    is_empty_dir(name))
			തുടരുക;

		എങ്കിൽ (!submodule_uses_gitfile(name))
			absorb_git_dir_into_superproject(name, ശൂന്യ);
	}
}

സ്ഥിതം പൂർണ്ണസംഖ്യ check_local_mod(ഘടന object_id *head, പൂർണ്ണസംഖ്യ index_only)
{
	/*
	 * Items in list are already sorted in the cache order,
	 * so we could do this a lot more efficiently by using
	 * tree_desc based traversal if we wanted to, but I am
	 * lazy, and who cares if removal of files is a tad
	 * slower than the theoretical maximum speed?
	 */
	പൂർണ്ണസംഖ്യ i, no_head;
	പൂർണ്ണസംഖ്യ errs = 0;
	ഘടന string_list files_staged = STRING_LIST_INIT_NODUP;
	ഘടന string_list files_cached = STRING_LIST_INIT_NODUP;
	ഘടന string_list files_local = STRING_LIST_INIT_NODUP;

	no_head = is_null_oid(head);
	വേണ്ടി (i = 0; i < list.nr; i++) {
		ഘടന stat st;
		പൂർണ്ണസംഖ്യ pos;
		സ്ഥിരം ഘടന cache_entry *ce;
		സ്ഥിരം അക്ഷരം *name = list.entry[i].name;
		ഘടന object_id oid;
		unsigned short mode;
		പൂർണ്ണസംഖ്യ local_changes = 0;
		പൂർണ്ണസംഖ്യ staged_changes = 0;

		pos = index_name_pos(the_repository->index, name, strlen(name));
		എങ്കിൽ (pos < 0) {
			/*
			 * Skip unmerged entries except for populated submodules
			 * that could lose history when removed.
			 */
			pos = get_ours_cache_pos(name, -pos - 1);
			എങ്കിൽ (pos < 0)
				തുടരുക;

			എങ്കിൽ (!S_ISGITLINK(the_repository->index->cache[pos]->ce_mode) ||
			    is_empty_dir(name))
				തുടരുക;
		}
		ce = the_repository->index->cache[pos];

		എങ്കിൽ (lstat(ce->name, &st) < 0) {
			എങ്കിൽ (!is_missing_file_error(errno))
				warning_errno(_("failed to stat '%s'"), ce->name);
			/* It already vanished from the working tree */
			തുടരുക;
		}
		അല്ലെങ്കിൽ എങ്കിൽ (S_ISDIR(st.st_mode)) {
			/* if a file was removed and it is now a
			 * directory, that is the same as ENOENT as
			 * far as git is concerned; we do not track
			 * directories unless they are submodules.
			 */
			എങ്കിൽ (!S_ISGITLINK(ce->ce_mode))
				തുടരുക;
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
		എങ്കിൽ (ie_match_stat(the_repository->index, ce, &st, 0) ||
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
		എങ്കിൽ (no_head
		     || get_വൃക്ഷം_entry(the_repository, head, name, &oid, &mode)
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
		എങ്കിൽ (local_changes && staged_changes) {
			എങ്കിൽ (!index_only || !ce_intent_to_add(ce))
				string_list_append(&files_staged, name);
		}
		അല്ലെങ്കിൽ എങ്കിൽ (!index_only) {
			എങ്കിൽ (staged_changes)
				string_list_append(&files_cached, name);
			എങ്കിൽ (local_changes)
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

	തിരികെ errs;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ show_only = 0, force = 0, index_only = 0, recursive = 0, quiet = 0;
സ്ഥിതം പൂർണ്ണസംഖ്യ ignore_unmatch = 0, pathspec_file_nul;
സ്ഥിതം പൂർണ്ണസംഖ്യ include_sparse;
സ്ഥിതം അക്ഷരം *pathspec_from_file;

സ്ഥിതം ഘടന option builtin_rm_options[] = {
	OPT__DRY_RUN(&show_only, N_("dry run")),
	OPT__QUIET(&quiet, N_("do not list removed files")),
	OPT_BOOL( 0 , "cached",         &index_only, N_("only remove from the index")),
	OPT__FORCE(&force, N_("override the up-to-date check"), PARSE_OPT_NOCOMPLETE),
	OPT_BOOL('r', ശൂന്യ,             &recursive,  N_("allow recursive removal")),
	OPT_BOOL( 0 , "ignore-unmatch", &ignore_unmatch,
				N_("exit with a zero status even if nothing matched")),
	OPT_BOOL(0, "sparse", &include_sparse, N_("allow updating entries outside of the sparse-checkout cone")),
	OPT_PATHSPEC_FROM_FILE(&pathspec_from_file),
	OPT_PATHSPEC_FILE_NUL(&pathspec_file_nul),
	OPT_END(),
};

പൂർണ്ണസംഖ്യ cmd_rm(പൂർണ്ണസംഖ്യ argc,
	   സ്ഥിരം അക്ഷരം **argv,
	   സ്ഥിരം അക്ഷരം *prefix,
	   ഘടന repository *repo UNUSED)
{
	ഘടന lock_file lock_file = LOCK_INIT;
	പൂർണ്ണസംഖ്യ i, ret = 0;
	ഘടന pathspec pathspec;
	അക്ഷരം *seen;

	repo_config(the_repository, git_default_config, ശൂന്യ);

	argc = parse_options(argc, argv, prefix, builtin_rm_options,
			     builtin_rm_usage, 0);

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD,
		       prefix, argv);

	എങ്കിൽ (pathspec_from_file) {
		എങ്കിൽ (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_CWD,
				    prefix, pathspec_from_file, pathspec_file_nul);
	} അല്ലെങ്കിൽ എങ്കിൽ (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	എങ്കിൽ (!pathspec.nr)
		die(_("No pathspec was given. Which files should I remove?"));

	എങ്കിൽ (!index_only)
		setup_work_വൃക്ഷം(the_repository);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;
	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);

	എങ്കിൽ (repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));

	refresh_index(the_repository->index, REFRESH_QUIET|REFRESH_UNMERGED, &pathspec, ശൂന്യ, ശൂന്യ);

	seen = xcalloc(pathspec.nr, 1);

	എങ്കിൽ (pathspec_needs_expanded_index(the_repository->index, &pathspec))
		ensure_full_index(the_repository->index);

	വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < the_repository->index->cache_nr; i++) {
		സ്ഥിരം ഘടന cache_entry *ce = the_repository->index->cache[i];

		എങ്കിൽ (!include_sparse &&
		    (ce_skip_worktree(ce) ||
		     !path_in_sparse_checkout(ce->name, the_repository->index)))
			തുടരുക;
		എങ്കിൽ (!ce_path_match(the_repository->index, ce, &pathspec, seen))
			തുടരുക;
		ALLOC_GROW(list.entry, list.nr + 1, list.alloc);
		list.entry[list.nr].name = xstrdup(ce->name);
		list.entry[list.nr].is_submodule = S_ISGITLINK(ce->ce_mode);
		എങ്കിൽ (list.entry[list.nr++].is_submodule &&
		    !is_staging_gitmodules_ok(the_repository->index))
			die(_("please stage your changes to .gitmodules or stash them to proceed"));
	}

	എങ്കിൽ (pathspec.nr) {
		സ്ഥിരം അക്ഷരം *original;
		പൂർണ്ണസംഖ്യ seen_any = 0;
		അക്ഷരം *skip_worktree_seen = ശൂന്യ;
		ഘടന string_list only_match_skip_worktree = STRING_LIST_INIT_NODUP;

		വേണ്ടി (i = 0; i < pathspec.nr; i++) {
			original = pathspec.ഇനങ്ങൾ[i].original;
			എങ്കിൽ (seen[i])
				seen_any = 1;
			അല്ലെങ്കിൽ എങ്കിൽ (ignore_unmatch)
				തുടരുക;
			അല്ലെങ്കിൽ എങ്കിൽ (!include_sparse &&
				 matches_skip_worktree(&pathspec, i, &skip_worktree_seen))
				string_list_append(&only_match_skip_worktree, original);
			അല്ലെങ്കിൽ
				die(_("pathspec '%s' did not match any files"), original);

			എങ്കിൽ (!recursive && seen[i] == MATCHED_RECURSIVELY)
				die(_("not removing '%s' recursively without -r"),
				    *original ? original : ".");
		}

		എങ്കിൽ (only_match_skip_worktree.nr) {
			advise_on_updating_sparse_paths(&only_match_skip_worktree);
			ret = 1;
		}
		സ്വതന്ത്രം(skip_worktree_seen);
		string_list_clear(&only_match_skip_worktree, 0);

		എങ്കിൽ (!seen_any)
			exit(ret);
	}
	clear_pathspec(&pathspec);
	സ്വതന്ത്രം(seen);

	എങ്കിൽ (!index_only)
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
	എങ്കിൽ (!force) {
		ഘടന object_id oid;
		എങ്കിൽ (repo_get_oid(the_repository, "HEAD", &oid))
			oidclr(&oid, the_repository->hash_algo);
		എങ്കിൽ (check_local_mod(&oid, index_only))
			exit(1);
	}

	/*
	 * First remove the names from the index: we won't commit
	 * the index unless all of them succeed.
	 */
	വേണ്ടി (i = 0; i < list.nr; i++) {
		സ്ഥിരം അക്ഷരം *path = list.entry[i].name;
		എങ്കിൽ (!quiet)
			printf("rm '%s'\n", path);

		എങ്കിൽ (remove_file_from_index(the_repository->index, path))
			die(_("git rm: unable to remove %s"), path);
	}

	എങ്കിൽ (show_only)
		തിരികെ 0;

	/*
	 * Then, unless we used "--cached", remove the filenames from
	 * the workspace. If we fail to remove the first one, we
	 * abort the "git rm" (but once we've successfully removed
	 * any file at all, we'll go ahead and commit to it all:
	 * by then we've already committed ourselves and can't fail
	 * in the middle)
	 */
	എങ്കിൽ (!index_only) {
		പൂർണ്ണസംഖ്യ removed = 0, gitmodules_modified = 0;
		ഘടന strbuf buf = STRBUF_INIT;
		പൂർണ്ണസംഖ്യ flag = force ? REMOVE_DIR_PURGE_ORIGINAL_CWD : 0;
		വേണ്ടി (i = 0; i < list.nr; i++) {
			സ്ഥിരം അക്ഷരം *path = list.entry[i].name;
			എങ്കിൽ (list.entry[i].is_submodule) {
				strbuf_reset(&buf);
				strbuf_addstr(&buf, path);
				എങ്കിൽ (remove_dir_recursively(&buf, flag))
					die(_("could not remove '%s'"), path);

				removed = 1;
				എങ്കിൽ (!remove_path_from_gitmodules(path))
					gitmodules_modified = 1;
				തുടരുക;
			}
			എങ്കിൽ (!remove_path(path)) {
				removed = 1;
				തുടരുക;
			}
			എങ്കിൽ (!removed)
				die_errno("git rm: '%s'", path);
		}
		strbuf_release(&buf);
		എങ്കിൽ (gitmodules_modified)
			stage_updated_gitmodules(the_repository->index);
	}

	എങ്കിൽ (write_locked_index(the_repository->index, &lock_file,
			       COMMIT_LOCK | SKIP_IF_UNCHANGED))
		die(_("Unable to write new index file"));

	തിരികെ ret;
}
