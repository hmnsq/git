/*
 * "git rm" builtin command
 *
 * Copyright (C) Linus Torvalds 2006
 */

#define USE_THE_REPOSITORY_VARIABLE

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "advice.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "lockfile.h"
#अन्तर्भाव "dir.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hash.h"
#अन्तर्भाव "tree-walk.h"
#अन्तर्भाव "object-name.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "read-cache.h"

#अन्तर्भाव "string-list.h"
#अन्तर्भाव "setup.h"
#अन्तर्भाव "sparse-index.h"
#अन्तर्भाव "submodule.h"
#अन्तर्भाव "pathspec.h"

स्थिर ध्रुव वर्ण * ध्रुव builtin_rm_usage[] = {
	N_("git rm [-f | --force] [-n] [-r] [--cached] [--ignore-unmatch]\n"
	   "       [--quiet] [--pathspec-from-file=<file> [--pathspec-file-nul]]\n"
	   "       [--] [<pathspec>...]"),
	रिक्त
};

स्थिर संरचना {
	पूर्णाङ्क nr, alloc;
	संरचना {
		ध्रुव वर्ण *name;
		वर्ण is_submodule;
	} *entry;
} list;

स्थिर पूर्णाङ्क get_ours_cache_pos(ध्रुव वर्ण *path, अचिन्हित पूर्णाङ्क pos)
{
	यावत् ((pos < the_repository->index->cache_nr) && !म_भेद(the_repository->index->cache[pos]->name, path)) {
		यदि (ce_stage(the_repository->index->cache[pos]) == 2)
			प्रत्यागम pos;
		pos++;
	}
	प्रत्यागम -1;
}

स्थिर शून्य print_error_files(संरचना string_list *files_list,
			      ध्रुव वर्ण *main_संदेश,
			      ध्रुव वर्ण *hints_संदेश,
			      पूर्णाङ्क *errs)
{
	यदि (files_list->nr) {
		अचिन्हित पूर्णाङ्क i;
		संरचना strbuf err_संदेश = STRBUF_INIT;

		strbuf_addstr(&err_संदेश, main_संदेश);
		प्रति (i = 0; i < files_list->nr; i++)
			strbuf_addf(&err_संदेश,
				    "\n    %s",
				    files_list->वस्तुएँ[i].string);
		यदि (advice_enabled(ADVICE_RM_HINTS))
			strbuf_addstr(&err_संदेश, hints_संदेश);
		*errs = error("%s", err_संदेश.buf);
		strbuf_release(&err_संदेश);
	}
}

स्थिर शून्य submodules_absorb_gitdir_if_needed(शून्य)
{
	पूर्णाङ्क i;
	प्रति (i = 0; i < list.nr; i++) {
		ध्रुव वर्ण *name = list.entry[i].name;
		पूर्णाङ्क pos;
		ध्रुव संरचना cache_entry *ce;

		pos = index_name_pos(the_repository->index, name, म_माप(name));
		यदि (pos < 0) {
			pos = get_ours_cache_pos(name, -pos - 1);
			यदि (pos < 0)
				अनुवर्त;
		}
		ce = the_repository->index->cache[pos];

		यदि (!S_ISGITLINK(ce->ce_mode) ||
		    !file_exists(ce->name) ||
		    is_empty_dir(name))
			अनुवर्त;

		यदि (!submodule_uses_gitfile(name))
			absorb_git_dir_into_superproject(name, रिक्त);
	}
}

स्थिर पूर्णाङ्क check_local_mod(संरचना object_id *head, पूर्णाङ्क index_only)
{
	/*
	 * Items in list are already sorted in the cache order,
	 * so we could do this a lot more efficiently by using
	 * tree_desc based traversal if we wanted to, but I am
	 * lazy, and who cares if removal of files is a tad
	 * slower than the theoretical maximum speed?
	 */
	पूर्णाङ्क i, no_head;
	पूर्णाङ्क errs = 0;
	संरचना string_list files_staged = STRING_LIST_INIT_NODUP;
	संरचना string_list files_cached = STRING_LIST_INIT_NODUP;
	संरचना string_list files_local = STRING_LIST_INIT_NODUP;

	no_head = is_null_oid(head);
	प्रति (i = 0; i < list.nr; i++) {
		संरचना stat st;
		पूर्णाङ्क pos;
		ध्रुव संरचना cache_entry *ce;
		ध्रुव वर्ण *name = list.entry[i].name;
		संरचना object_id oid;
		अचिन्हित लघु mode;
		पूर्णाङ्क local_changes = 0;
		पूर्णाङ्क staged_changes = 0;

		pos = index_name_pos(the_repository->index, name, म_माप(name));
		यदि (pos < 0) {
			/*
			 * Skip unmerged entries except for populated submodules
			 * that could lose history when removed.
			 */
			pos = get_ours_cache_pos(name, -pos - 1);
			यदि (pos < 0)
				अनुवर्त;

			यदि (!S_ISGITLINK(the_repository->index->cache[pos]->ce_mode) ||
			    is_empty_dir(name))
				अनुवर्त;
		}
		ce = the_repository->index->cache[pos];

		यदि (lstat(ce->name, &st) < 0) {
			यदि (!is_missing_file_error(त्रुटि_सं))
				warning_errno(_("failed to stat '%s'"), ce->name);
			/* It already vanished from the working tree */
			अनुवर्त;
		}
		अन्यथा यदि (S_ISDIR(st.st_mode)) {
			/* if a file was removed and it is now a
			 * directory, that is the same as ENOENT as
			 * far as git is concerned; we do not track
			 * directories unless they are submodules.
			 */
			यदि (!S_ISGITLINK(ce->ce_mode))
				अनुवर्त;
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
		यदि (ie_match_stat(the_repository->index, ce, &st, 0) ||
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
		यदि (no_head
		     || get_वृक्ष_entry(the_repository, head, name, &oid, &mode)
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
		यदि (local_changes && staged_changes) {
			यदि (!index_only || !ce_intent_to_add(ce))
				string_list_append(&files_staged, name);
		}
		अन्यथा यदि (!index_only) {
			यदि (staged_changes)
				string_list_append(&files_cached, name);
			यदि (local_changes)
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

	प्रत्यागम errs;
}

स्थिर पूर्णाङ्क show_only = 0, force = 0, index_only = 0, recursive = 0, quiet = 0;
स्थिर पूर्णाङ्क ignore_unmatch = 0, pathspec_file_nul;
स्थिर पूर्णाङ्क include_sparse;
स्थिर वर्ण *pathspec_from_file;

स्थिर संरचना option builtin_rm_options[] = {
	OPT__DRY_RUN(&show_only, N_("dry run")),
	OPT__QUIET(&quiet, N_("do not list removed files")),
	OPT_BOOL( 0 , "cached",         &index_only, N_("only remove from the index")),
	OPT__FORCE(&force, N_("override the up-to-date check"), PARSE_OPT_NOCOMPLETE),
	OPT_BOOL('r', रिक्त,             &recursive,  N_("allow recursive removal")),
	OPT_BOOL( 0 , "ignore-unmatch", &ignore_unmatch,
				N_("exit with a zero status even if nothing matched")),
	OPT_BOOL(0, "sparse", &include_sparse, N_("allow updating entries outside of the sparse-checkout cone")),
	OPT_PATHSPEC_FROM_FILE(&pathspec_from_file),
	OPT_PATHSPEC_FILE_NUL(&pathspec_file_nul),
	OPT_END(),
};

पूर्णाङ्क cmd_rm(पूर्णाङ्क argc,
	   ध्रुव वर्ण **argv,
	   ध्रुव वर्ण *prefix,
	   संरचना repository *repo UNUSED)
{
	संरचना lock_file lock_file = LOCK_INIT;
	पूर्णाङ्क i, ret = 0;
	संरचना pathspec pathspec;
	वर्ण *seen;

	repo_config(the_repository, git_default_config, रिक्त);

	argc = parse_options(argc, argv, prefix, builtin_rm_options,
			     builtin_rm_usage, 0);

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD,
		       prefix, argv);

	यदि (pathspec_from_file) {
		यदि (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_CWD,
				    prefix, pathspec_from_file, pathspec_file_nul);
	} अन्यथा यदि (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	यदि (!pathspec.nr)
		die(_("No pathspec was given. Which files should I remove?"));

	यदि (!index_only)
		setup_work_वृक्ष(the_repository);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;
	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);

	यदि (repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));

	refresh_index(the_repository->index, REFRESH_QUIET|REFRESH_UNMERGED, &pathspec, रिक्त, रिक्त);

	seen = xcalloc(pathspec.nr, 1);

	यदि (pathspec_needs_expanded_index(the_repository->index, &pathspec))
		ensure_full_index(the_repository->index);

	प्रति (अचिन्हित पूर्णाङ्क i = 0; i < the_repository->index->cache_nr; i++) {
		ध्रुव संरचना cache_entry *ce = the_repository->index->cache[i];

		यदि (!include_sparse &&
		    (ce_skip_worktree(ce) ||
		     !path_in_sparse_checkout(ce->name, the_repository->index)))
			अनुवर्त;
		यदि (!ce_path_match(the_repository->index, ce, &pathspec, seen))
			अनुवर्त;
		ALLOC_GROW(list.entry, list.nr + 1, list.alloc);
		list.entry[list.nr].name = xstrdup(ce->name);
		list.entry[list.nr].is_submodule = S_ISGITLINK(ce->ce_mode);
		यदि (list.entry[list.nr++].is_submodule &&
		    !is_staging_gitmodules_ok(the_repository->index))
			die(_("please stage your changes to .gitmodules or stash them to proceed"));
	}

	यदि (pathspec.nr) {
		ध्रुव वर्ण *original;
		पूर्णाङ्क seen_any = 0;
		वर्ण *skip_worktree_seen = रिक्त;
		संरचना string_list only_match_skip_worktree = STRING_LIST_INIT_NODUP;

		प्रति (i = 0; i < pathspec.nr; i++) {
			original = pathspec.वस्तुएँ[i].original;
			यदि (seen[i])
				seen_any = 1;
			अन्यथा यदि (ignore_unmatch)
				अनुवर्त;
			अन्यथा यदि (!include_sparse &&
				 matches_skip_worktree(&pathspec, i, &skip_worktree_seen))
				string_list_append(&only_match_skip_worktree, original);
			अन्यथा
				die(_("pathspec '%s' did not match any files"), original);

			यदि (!recursive && seen[i] == MATCHED_RECURSIVELY)
				die(_("not removing '%s' recursively without -r"),
				    *original ? original : ".");
		}

		यदि (only_match_skip_worktree.nr) {
			advise_on_updating_sparse_paths(&only_match_skip_worktree);
			ret = 1;
		}
		मुक्त(skip_worktree_seen);
		string_list_clear(&only_match_skip_worktree, 0);

		यदि (!seen_any)
			निकास(ret);
	}
	clear_pathspec(&pathspec);
	मुक्त(seen);

	यदि (!index_only)
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
	यदि (!force) {
		संरचना object_id oid;
		यदि (repo_get_oid(the_repository, "HEAD", &oid))
			oidclr(&oid, the_repository->hash_algo);
		यदि (check_local_mod(&oid, index_only))
			निकास(1);
	}

	/*
	 * First remove the names from the index: we won't commit
	 * the index unless all of them succeed.
	 */
	प्रति (i = 0; i < list.nr; i++) {
		ध्रुव वर्ण *path = list.entry[i].name;
		यदि (!quiet)
			म_लिखो("rm '%s'\n", path);

		यदि (remove_file_from_index(the_repository->index, path))
			die(_("git rm: unable to remove %s"), path);
	}

	यदि (show_only)
		प्रत्यागम 0;

	/*
	 * Then, unless we used "--cached", remove the filenames from
	 * the workspace. If we fail to remove the first one, we
	 * abort the "git rm" (but once we've successfully removed
	 * any file at all, we'll go ahead and commit to it all:
	 * by then we've already committed ourselves and can't fail
	 * in the middle)
	 */
	यदि (!index_only) {
		पूर्णाङ्क removed = 0, gitmodules_modified = 0;
		संरचना strbuf buf = STRBUF_INIT;
		पूर्णाङ्क flag = force ? REMOVE_DIR_PURGE_ORIGINAL_CWD : 0;
		प्रति (i = 0; i < list.nr; i++) {
			ध्रुव वर्ण *path = list.entry[i].name;
			यदि (list.entry[i].is_submodule) {
				strbuf_reset(&buf);
				strbuf_addstr(&buf, path);
				यदि (remove_dir_recursively(&buf, flag))
					die(_("could not remove '%s'"), path);

				removed = 1;
				यदि (!remove_path_from_gitmodules(path))
					gitmodules_modified = 1;
				अनुवर्त;
			}
			यदि (!remove_path(path)) {
				removed = 1;
				अनुवर्त;
			}
			यदि (!removed)
				die_errno("git rm: '%s'", path);
		}
		strbuf_release(&buf);
		यदि (gitmodules_modified)
			stage_updated_gitmodules(the_repository->index);
	}

	यदि (write_locked_index(the_repository->index, &lock_file,
			       COMMIT_LOCK | SKIP_IF_UNCHANGED))
		die(_("Unable to write new index file"));

	प्रत्यागम ret;
}
