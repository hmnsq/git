/*
 * "git rm" builtin command
 *
 * Copyright (C) Linus Torvalds 2006
 */

#تعریف_کرو USE_THE_REPOSITORY_VARIABLE

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

ساکن ثابت حرف * ثابت builtin_rm_usage[] = {
	N_("git rm [-f | --force] [-n] [-r] [--cached] [--ignore-unmatch]\n"
	   "       [--quiet] [--pathspec-from-file=<file> [--pathspec-file-nul]]\n"
	   "       [--] [<pathspec>...]"),
	تہی
};

ساکن ساخت {
	صحیح nr, alloc;
	ساخت {
		ثابت حرف *name;
		حرف is_submodule;
	} *اندراج_2;
} list;

ساکن صحیح get_ours_cache_pos(ثابت حرف *راستہ, غیرعلامتی صحیح pos)
{
	جب ((pos < the_repository->اشاریہ->cache_nr) && !strcmp(the_repository->اشاریہ->cache[pos]->name, راستہ)) {
		اگر (ce_stage(the_repository->اشاریہ->cache[pos]) == 2)
			واپس pos;
		pos++;
	}
	واپس -1;
}

ساکن خالی print_error_files(ساخت string_list *files_list,
			      ثابت حرف *main_پیغام,
			      ثابت حرف *hints_پیغام,
			      صحیح *errs)
{
	اگر (files_list->nr) {
		غیرعلامتی صحیح i;
		ساخت strbuf err_پیغام = STRBUF_INIT;

		strbuf_addstr(&err_پیغام, main_پیغام);
		برائے (i = 0; i < files_list->nr; i++)
			strbuf_addf(&err_پیغام,
				    "\n    %s",
				    files_list->اشیاء[i].string);
		اگر (advice_enabled(ADVICE_RM_HINTS))
			strbuf_addstr(&err_پیغام, hints_پیغام);
		*errs = error("%s", err_پیغام.buf);
		strbuf_release(&err_پیغام);
	}
}

ساکن خالی submodules_absorb_gitdir_if_needed(خالی)
{
	صحیح i;
	برائے (i = 0; i < list.nr; i++) {
		ثابت حرف *name = list.اندراج_2[i].name;
		صحیح pos;
		ثابت ساخت cache_اندراج *ce;

		pos = اشاریہ_name_pos(the_repository->اشاریہ, name, لڑی_لمبائی(name));
		اگر (pos < 0) {
			pos = get_ours_cache_pos(name, -pos - 1);
			اگر (pos < 0)
				جاری;
		}
		ce = the_repository->اشاریہ->cache[pos];

		اگر (!S_ISGITLINK(ce->ce_منوال) ||
		    !file_exists(ce->name) ||
		    is_خالی_خانے_dir(name))
			جاری;

		اگر (!submodule_استعمال_کرتا_gitfile(name))
			absorb_git_dir_into_superproject(name, تہی);
	}
}

ساکن صحیح check_local_mod(ساخت object_id *سر, صحیح اشاریہ_only)
{
	/*
	 * Items in list are already sorted in the cache order,
	 * so we could do this a lot more efficiently by using
	 * tree_desc based traversal if we wanted to, but I am
	 * lazy, and who cares if removal of files is a tad
	 * slower than the theoretical maximum speed?
	 */
	صحیح i, no_سر;
	صحیح errs = 0;
	ساخت string_list files_staged = STRING_LIST_INIT_NODUP;
	ساخت string_list files_cached = STRING_LIST_INIT_NODUP;
	ساخت string_list files_local = STRING_LIST_INIT_NODUP;

	no_سر = is_null_oid(سر);
	برائے (i = 0; i < list.nr; i++) {
		ساخت stat st;
		صحیح pos;
		ثابت ساخت cache_اندراج *ce;
		ثابت حرف *name = list.اندراج_2[i].name;
		ساخت object_id oid;
		غیرعلامتی مختصر منوال;
		صحیح local_changes = 0;
		صحیح staged_changes = 0;

		pos = اشاریہ_name_pos(the_repository->اشاریہ, name, لڑی_لمبائی(name));
		اگر (pos < 0) {
			/*
			 * Skip unmerged entries except for populated submodules
			 * that could lose history when removed.
			 */
			pos = get_ours_cache_pos(name, -pos - 1);
			اگر (pos < 0)
				جاری;

			اگر (!S_ISGITLINK(the_repository->اشاریہ->cache[pos]->ce_منوال) ||
			    is_خالی_خانے_dir(name))
				جاری;
		}
		ce = the_repository->اشاریہ->cache[pos];

		اگر (lstat(ce->name, &st) < 0) {
			اگر (!is_missing_file_error(errno))
				warning_errno(_("failed to stat '%s'"), ce->name);
			/* It already vanished from the working tree */
			جاری;
		}
		ورنہ اگر (S_ISDIR(st.st_منوال)) {
			/* if a file was removed and it is now a
			 * directory, that is the same as ENOENT as
			 * far as git is concerned; we do not track
			 * directories unless they are submodules.
			 */
			اگر (!S_ISGITLINK(ce->ce_منوال))
				جاری;
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
		اگر (ie_مطابقت_stat(the_repository->اشاریہ, ce, &st, 0) ||
		    (S_ISGITLINK(ce->ce_منوال) &&
		     bad_مقصد_remove_submodule(ce->name,
				SUBMODULE_REMOVAL_DIE_ON_ERROR |
				SUBMODULE_REMOVAL_IGNORE_IGNORED_UNTRACKED)))
			local_changes = 1;

		/*
		 * Is the index different from the HEAD commit?  By
		 * definition, before the very initial commit,
		 * anything staged in the index is treated by the same
		 * way as changed from the HEAD.
		 */
		اگر (no_سر
		     || get_درخت_اندراج(the_repository, سر, name, &oid, &منوال)
		     || ce->ce_منوال != create_ce_منوال(منوال)
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
			اگر (!اشاریہ_only || !ce_intent_مقصد_جوڑو(ce))
				string_list_append(&files_staged, name);
		}
		ورنہ اگر (!اشاریہ_only) {
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

	واپس errs;
}

ساکن صحیح show_only = 0, force = 0, اشاریہ_only = 0, recursive = 0, quiet = 0;
ساکن صحیح ignore_unmatch = 0, pathspec_file_nul;
ساکن صحیح include_sparse;
ساکن حرف *pathspec_from_file;

ساکن ساخت option builtin_rm_options[] = {
	OPT_DRY_تسلسل(&show_only, N_("dry run")),
	OPT__QUIET(&quiet, N_("do not list removed files")),
	OPT_BOOL( 0 , "cached",         &اشاریہ_only, N_("only remove from the index")),
	OPT__FORCE(&force, N_("override the up-to-date check"), PARSE_OPT_NOCOMPLETE),
	OPT_BOOL('r', تہی,             &recursive,  N_("allow recursive removal")),
	OPT_BOOL( 0 , "ignore-unmatch", &ignore_unmatch,
				N_("exit with a zero status even if nothing matched")),
	OPT_BOOL(0, "sparse", &include_sparse, N_("allow updating entries outside of the sparse-checkout cone")),
	OPT_PATHSPEC_FROM_FILE(&pathspec_from_file),
	OPT_PATHSPEC_FILE_NUL(&pathspec_file_nul),
	OPT_انتہا(),
};

صحیح cmd_rm(صحیح argc,
	   ثابت حرف **argv,
	   ثابت حرف *سابقہ,
	   ساخت repository *repo غیر_مستعمل)
{
	ساخت تالا_file تالا_file = تالا_INIT;
	صحیح i, ret = 0;
	ساخت pathspec pathspec;
	حرف *نظر_آیا;

	repo_config(the_repository, git_default_config, تہی);

	argc = parse_options(argc, argv, سابقہ, builtin_rm_options,
			     builtin_rm_usage, 0);

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD,
		       سابقہ, argv);

	اگر (pathspec_from_file) {
		اگر (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_CWD,
				    سابقہ, pathspec_from_file, pathspec_file_nul);
	} ورنہ اگر (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	اگر (!pathspec.nr)
		die(_("No pathspec was given. Which files should I remove?"));

	اگر (!اشاریہ_only)
		تیاری_work_درخت(the_repository);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_بھرا_اشاریہ = 0;
	repo_hold_locked_اشاریہ(the_repository, &تالا_file, تالا_DIE_ON_ERROR);

	اگر (repo_read_اشاریہ(the_repository) < 0)
		die(_("index file corrupt"));

	refresh_اشاریہ(the_repository->اشاریہ, REFRESH_QUIET|REFRESH_UNMERGED, &pathspec, تہی, تہی);

	نظر_آیا = xcalloc(pathspec.nr, 1);

	اگر (pathspec_needs_expanded_اشاریہ(the_repository->اشاریہ, &pathspec))
		ensure_بھرا_اشاریہ(the_repository->اشاریہ);

	برائے (غیرعلامتی صحیح i = 0; i < the_repository->اشاریہ->cache_nr; i++) {
		ثابت ساخت cache_اندراج *ce = the_repository->اشاریہ->cache[i];

		اگر (!include_sparse &&
		    (ce_skip_worktree(ce) ||
		     !راستہ_اندر_sparse_checkout(ce->name, the_repository->اشاریہ)))
			جاری;
		اگر (!ce_راستہ_مطابقت(the_repository->اشاریہ, ce, &pathspec, نظر_آیا))
			جاری;
		ALLOC_GROW(list.اندراج_2, list.nr + 1, list.alloc);
		list.اندراج_2[list.nr].name = xstrdup(ce->name);
		list.اندراج_2[list.nr].is_submodule = S_ISGITLINK(ce->ce_منوال);
		اگر (list.اندراج_2[list.nr++].is_submodule &&
		    !is_staging_gitmodules_ok(the_repository->اشاریہ))
			die(_("please stage your changes to .gitmodules or stash them to proceed"));
	}

	اگر (pathspec.nr) {
		ثابت حرف *original;
		صحیح نظر_آیا_any = 0;
		حرف *skip_worktree_نظر_آیا = تہی;
		ساخت string_list only_مطابقت_skip_worktree = STRING_LIST_INIT_NODUP;

		برائے (i = 0; i < pathspec.nr; i++) {
			original = pathspec.اشیاء[i].original;
			اگر (نظر_آیا[i])
				نظر_آیا_any = 1;
			ورنہ اگر (ignore_unmatch)
				جاری;
			ورنہ اگر (!include_sparse &&
				 matches_skip_worktree(&pathspec, i, &skip_worktree_نظر_آیا))
				string_list_append(&only_مطابقت_skip_worktree, original);
			ورنہ
				die(_("pathspec '%s' did not match any files"), original);

			اگر (!recursive && نظر_آیا[i] == MATCHED_RECURSIVELY)
				die(_("not removing '%s' recursively without -r"),
				    *original ? original : ".");
		}

		اگر (only_مطابقت_skip_worktree.nr) {
			advise_on_updating_sparse_paths(&only_مطابقت_skip_worktree);
			ret = 1;
		}
		آزاد(skip_worktree_نظر_آیا);
		string_list_clear(&only_مطابقت_skip_worktree, 0);

		اگر (!نظر_آیا_any)
			باہر_نکلو(ret);
	}
	clear_pathspec(&pathspec);
	آزاد(نظر_آیا);

	اگر (!اشاریہ_only)
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
		ساخت object_id oid;
		اگر (repo_get_oid(the_repository, "HEAD", &oid))
			oidclr(&oid, the_repository->hash_algo);
		اگر (check_local_mod(&oid, اشاریہ_only))
			باہر_نکلو(1);
	}

	/*
	 * First remove the names from the index: we won't commit
	 * the index unless all of them succeed.
	 */
	برائے (i = 0; i < list.nr; i++) {
		ثابت حرف *راستہ = list.اندراج_2[i].name;
		اگر (!quiet)
			لکھو("rm '%s'\n", راستہ);

		اگر (remove_file_from_اشاریہ(the_repository->اشاریہ, راستہ))
			die(_("git rm: unable to remove %s"), راستہ);
	}

	اگر (show_only)
		واپس 0;

	/*
	 * Then, unless we used "--cached", remove the filenames from
	 * the workspace. If we fail to remove the first one, we
	 * abort the "git rm" (but once we've successfully removed
	 * any file at all, we'll go ahead and commit to it all:
	 * by then we've already committed ourselves and can't fail
	 * in the middle)
	 */
	اگر (!اشاریہ_only) {
		صحیح removed = 0, gitmodules_modified = 0;
		ساخت strbuf buf = STRBUF_INIT;
		صحیح flag = force ? REMOVE_DIR_PURGE_ORIGINAL_CWD : 0;
		برائے (i = 0; i < list.nr; i++) {
			ثابت حرف *راستہ = list.اندراج_2[i].name;
			اگر (list.اندراج_2[i].is_submodule) {
				strbuf_reset(&buf);
				strbuf_addstr(&buf, راستہ);
				اگر (remove_dir_recursively(&buf, flag))
					die(_("could not remove '%s'"), راستہ);

				removed = 1;
				اگر (!remove_راستہ_from_gitmodules(راستہ))
					gitmodules_modified = 1;
				جاری;
			}
			اگر (!remove_راستہ(راستہ)) {
				removed = 1;
				جاری;
			}
			اگر (!removed)
				die_errno("git rm: '%s'", راستہ);
		}
		strbuf_release(&buf);
		اگر (gitmodules_modified)
			stage_updated_gitmodules(the_repository->اشاریہ);
	}

	اگر (write_locked_اشاریہ(the_repository->اشاریہ, &تالا_file,
			       COMMIT_تالا | SKIP_IF_UNCHANGED))
		die(_("Unable to write new index file"));

	واپس ret;
}
