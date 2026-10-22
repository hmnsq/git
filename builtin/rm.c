/*
 * "git rm" builtin command
 *
 * Copyright (C) Linus Torvalds 2006
 */

#घोषणा USE_THE_REPOSITORY_VARIABLE

#समावेश "builtin.h"
#समावेश "advice.h"
#समावेश "config.h"
#समावेश "environment.h"
#समावेश "lockfile.h"
#समावेश "dir.h"
#समावेश "gettext.h"
#समावेश "hash.h"
#समावेश "tree-walk.h"
#समावेश "object-name.h"
#समावेश "parse-options.h"
#समावेश "read-cache.h"

#समावेश "string-list.h"
#समावेश "setup.h"
#समावेश "sparse-index.h"
#समावेश "submodule.h"
#समावेश "pathspec.h"

अटल स्थिर अक्षर * स्थिर builtin_rm_usage[] = {
	N_("git rm [-f | --force] [-n] [-r] [--cached] [--ignore-unmatch]\n"
	   "       [--quiet] [--pathspec-from-file=<file> [--pathspec-file-nul]]\n"
	   "       [--] [<pathspec>...]"),
	शून्य
};

अटल काष्ठा {
	पूर्णांक nr, alloc;
	काष्ठा {
		स्थिर अक्षर *name;
		अक्षर is_submodule;
	} *entry;
} list;

अटल पूर्णांक get_ours_cache_pos(स्थिर अक्षर *path, अचिन्हित पूर्णांक pos)
{
	जबतक ((pos < the_repository->सूचकांक->cache_nr) && !म_भेद(the_repository->सूचकांक->cache[pos]->name, path)) {
		अगर (ce_stage(the_repository->सूचकांक->cache[pos]) == 2)
			वापस pos;
		pos++;
	}
	वापस -1;
}

अटल व्योम print_error_files(काष्ठा string_list *files_list,
			      स्थिर अक्षर *main_संदेश,
			      स्थिर अक्षर *hints_संदेश,
			      पूर्णांक *errs)
{
	अगर (files_list->nr) {
		अचिन्हित पूर्णांक i;
		काष्ठा strbuf err_संदेश = STRBUF_INIT;

		strbuf_addstr(&err_संदेश, main_संदेश);
		क्रम (i = 0; i < files_list->nr; i++)
			strbuf_addf(&err_संदेश,
				    "\n    %s",
				    files_list->वस्तुएँ[i].string);
		अगर (advice_enabled(ADVICE_RM_HINTS))
			strbuf_addstr(&err_संदेश, hints_संदेश);
		*errs = error("%s", err_संदेश.buf);
		strbuf_release(&err_संदेश);
	}
}

अटल व्योम submodules_absorb_gitdir_if_needed(व्योम)
{
	पूर्णांक i;
	क्रम (i = 0; i < list.nr; i++) {
		स्थिर अक्षर *name = list.entry[i].name;
		पूर्णांक pos;
		स्थिर काष्ठा cache_entry *ce;

		pos = सूचकांक_name_pos(the_repository->सूचकांक, name, म_माप(name));
		अगर (pos < 0) {
			pos = get_ours_cache_pos(name, -pos - 1);
			अगर (pos < 0)
				जारी;
		}
		ce = the_repository->सूचकांक->cache[pos];

		अगर (!S_ISGITLINK(ce->ce_mode) ||
		    !file_exists(ce->name) ||
		    is_empty_dir(name))
			जारी;

		अगर (!submodule_uses_gitfile(name))
			absorb_git_dir_into_superproject(name, शून्य);
	}
}

अटल पूर्णांक check_local_mod(काष्ठा object_id *शीर्ष, पूर्णांक सूचकांक_only)
{
	/*
	 * Items in list are already sorted in the cache order,
	 * so we could do this a lot more efficiently by using
	 * tree_desc based traversal if we wanted to, but I am
	 * lazy, and who cares if removal of files is a tad
	 * slower than the theoretical maximum speed?
	 */
	पूर्णांक i, no_शीर्ष;
	पूर्णांक errs = 0;
	काष्ठा string_list files_staged = STRING_LIST_INIT_NODUP;
	काष्ठा string_list files_cached = STRING_LIST_INIT_NODUP;
	काष्ठा string_list files_local = STRING_LIST_INIT_NODUP;

	no_शीर्ष = is_null_oid(शीर्ष);
	क्रम (i = 0; i < list.nr; i++) {
		काष्ठा stat st;
		पूर्णांक pos;
		स्थिर काष्ठा cache_entry *ce;
		स्थिर अक्षर *name = list.entry[i].name;
		काष्ठा object_id oid;
		अचिन्हित लघु mode;
		पूर्णांक local_changes = 0;
		पूर्णांक staged_changes = 0;

		pos = सूचकांक_name_pos(the_repository->सूचकांक, name, म_माप(name));
		अगर (pos < 0) {
			/*
			 * Skip unmerged entries except for populated submodules
			 * that could lose history when removed.
			 */
			pos = get_ours_cache_pos(name, -pos - 1);
			अगर (pos < 0)
				जारी;

			अगर (!S_ISGITLINK(the_repository->सूचकांक->cache[pos]->ce_mode) ||
			    is_empty_dir(name))
				जारी;
		}
		ce = the_repository->सूचकांक->cache[pos];

		अगर (lstat(ce->name, &st) < 0) {
			अगर (!is_missing_file_error(त्रुटि_सं))
				warning_errno(_("failed to stat '%s'"), ce->name);
			/* It already vanished from the working tree */
			जारी;
		}
		अन्यथा अगर (S_ISDIR(st.st_mode)) {
			/* if a file was removed and it is now a
			 * directory, that is the same as ENOENT as
			 * far as git is concerned; we do not track
			 * directories unless they are submodules.
			 */
			अगर (!S_ISGITLINK(ce->ce_mode))
				जारी;
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
		अगर (ie_match_stat(the_repository->सूचकांक, ce, &st, 0) ||
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
		अगर (no_शीर्ष
		     || get_वृक्ष_entry(the_repository, शीर्ष, name, &oid, &mode)
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
		अगर (local_changes && staged_changes) {
			अगर (!सूचकांक_only || !ce_intent_to_add(ce))
				string_list_append(&files_staged, name);
		}
		अन्यथा अगर (!सूचकांक_only) {
			अगर (staged_changes)
				string_list_append(&files_cached, name);
			अगर (local_changes)
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

	वापस errs;
}

अटल पूर्णांक show_only = 0, force = 0, सूचकांक_only = 0, recursive = 0, quiet = 0;
अटल पूर्णांक ignore_unmatch = 0, pathspec_file_nul;
अटल पूर्णांक include_sparse;
अटल अक्षर *pathspec_from_file;

अटल काष्ठा option builtin_rm_options[] = {
	OPT__DRY_RUN(&show_only, N_("dry run")),
	OPT__QUIET(&quiet, N_("do not list removed files")),
	OPT_BOOL( 0 , "cached",         &सूचकांक_only, N_("only remove from the index")),
	OPT__FORCE(&force, N_("override the up-to-date check"), PARSE_OPT_NOCOMPLETE),
	OPT_BOOL('r', शून्य,             &recursive,  N_("allow recursive removal")),
	OPT_BOOL( 0 , "ignore-unmatch", &ignore_unmatch,
				N_("exit with a zero status even if nothing matched")),
	OPT_BOOL(0, "sparse", &include_sparse, N_("allow updating entries outside of the sparse-checkout cone")),
	OPT_PATHSPEC_FROM_FILE(&pathspec_from_file),
	OPT_PATHSPEC_FILE_NUL(&pathspec_file_nul),
	OPT_END(),
};

पूर्णांक cmd_rm(पूर्णांक argc,
	   स्थिर अक्षर **argv,
	   स्थिर अक्षर *prefix,
	   काष्ठा repository *repo UNUSED)
{
	काष्ठा lock_file lock_file = LOCK_INIT;
	पूर्णांक i, ret = 0;
	काष्ठा pathspec pathspec;
	अक्षर *seen;

	repo_config(the_repository, git_default_config, शून्य);

	argc = parse_options(argc, argv, prefix, builtin_rm_options,
			     builtin_rm_usage, 0);

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD,
		       prefix, argv);

	अगर (pathspec_from_file) {
		अगर (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_CWD,
				    prefix, pathspec_from_file, pathspec_file_nul);
	} अन्यथा अगर (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	अगर (!pathspec.nr)
		die(_("No pathspec was given. Which files should I remove?"));

	अगर (!सूचकांक_only)
		setup_work_वृक्ष(the_repository);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_सूचकांक = 0;
	repo_hold_locked_सूचकांक(the_repository, &lock_file, LOCK_DIE_ON_ERROR);

	अगर (repo_read_सूचकांक(the_repository) < 0)
		die(_("index file corrupt"));

	refresh_सूचकांक(the_repository->सूचकांक, REFRESH_QUIET|REFRESH_UNMERGED, &pathspec, शून्य, शून्य);

	seen = xcalloc(pathspec.nr, 1);

	अगर (pathspec_needs_expanded_सूचकांक(the_repository->सूचकांक, &pathspec))
		ensure_full_सूचकांक(the_repository->सूचकांक);

	क्रम (अचिन्हित पूर्णांक i = 0; i < the_repository->सूचकांक->cache_nr; i++) {
		स्थिर काष्ठा cache_entry *ce = the_repository->सूचकांक->cache[i];

		अगर (!include_sparse &&
		    (ce_skip_worktree(ce) ||
		     !path_in_sparse_checkout(ce->name, the_repository->सूचकांक)))
			जारी;
		अगर (!ce_path_match(the_repository->सूचकांक, ce, &pathspec, seen))
			जारी;
		ALLOC_GROW(list.entry, list.nr + 1, list.alloc);
		list.entry[list.nr].name = xstrdup(ce->name);
		list.entry[list.nr].is_submodule = S_ISGITLINK(ce->ce_mode);
		अगर (list.entry[list.nr++].is_submodule &&
		    !is_staging_gitmodules_ok(the_repository->सूचकांक))
			die(_("please stage your changes to .gitmodules or stash them to proceed"));
	}

	अगर (pathspec.nr) {
		स्थिर अक्षर *original;
		पूर्णांक seen_any = 0;
		अक्षर *skip_worktree_seen = शून्य;
		काष्ठा string_list only_match_skip_worktree = STRING_LIST_INIT_NODUP;

		क्रम (i = 0; i < pathspec.nr; i++) {
			original = pathspec.वस्तुएँ[i].original;
			अगर (seen[i])
				seen_any = 1;
			अन्यथा अगर (ignore_unmatch)
				जारी;
			अन्यथा अगर (!include_sparse &&
				 matches_skip_worktree(&pathspec, i, &skip_worktree_seen))
				string_list_append(&only_match_skip_worktree, original);
			अन्यथा
				die(_("pathspec '%s' did not match any files"), original);

			अगर (!recursive && seen[i] == MATCHED_RECURSIVELY)
				die(_("not removing '%s' recursively without -r"),
				    *original ? original : ".");
		}

		अगर (only_match_skip_worktree.nr) {
			advise_on_updating_sparse_paths(&only_match_skip_worktree);
			ret = 1;
		}
		मुक्त(skip_worktree_seen);
		string_list_clear(&only_match_skip_worktree, 0);

		अगर (!seen_any)
			निकास(ret);
	}
	clear_pathspec(&pathspec);
	मुक्त(seen);

	अगर (!सूचकांक_only)
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
	अगर (!force) {
		काष्ठा object_id oid;
		अगर (repo_get_oid(the_repository, "HEAD", &oid))
			oidclr(&oid, the_repository->hash_algo);
		अगर (check_local_mod(&oid, सूचकांक_only))
			निकास(1);
	}

	/*
	 * First remove the names from the index: we won't commit
	 * the index unless all of them succeed.
	 */
	क्रम (i = 0; i < list.nr; i++) {
		स्थिर अक्षर *path = list.entry[i].name;
		अगर (!quiet)
			म_लिखो("rm '%s'\n", path);

		अगर (remove_file_from_सूचकांक(the_repository->सूचकांक, path))
			die(_("git rm: unable to remove %s"), path);
	}

	अगर (show_only)
		वापस 0;

	/*
	 * Then, unless we used "--cached", remove the filenames from
	 * the workspace. If we fail to remove the first one, we
	 * abort the "git rm" (but once we've successfully removed
	 * any file at all, we'll go ahead and commit to it all:
	 * by then we've already committed ourselves and can't fail
	 * in the middle)
	 */
	अगर (!सूचकांक_only) {
		पूर्णांक removed = 0, gitmodules_modified = 0;
		काष्ठा strbuf buf = STRBUF_INIT;
		पूर्णांक flag = force ? REMOVE_DIR_PURGE_ORIGINAL_CWD : 0;
		क्रम (i = 0; i < list.nr; i++) {
			स्थिर अक्षर *path = list.entry[i].name;
			अगर (list.entry[i].is_submodule) {
				strbuf_reset(&buf);
				strbuf_addstr(&buf, path);
				अगर (remove_dir_recursively(&buf, flag))
					die(_("could not remove '%s'"), path);

				removed = 1;
				अगर (!remove_path_from_gitmodules(path))
					gitmodules_modified = 1;
				जारी;
			}
			अगर (!remove_path(path)) {
				removed = 1;
				जारी;
			}
			अगर (!removed)
				die_errno("git rm: '%s'", path);
		}
		strbuf_release(&buf);
		अगर (gitmodules_modified)
			stage_updated_gitmodules(the_repository->सूचकांक);
	}

	अगर (write_locked_सूचकांक(the_repository->सूचकांक, &lock_file,
			       COMMIT_LOCK | SKIP_IF_UNCHANGED))
		die(_("Unable to write new index file"));

	वापस ret;
}
