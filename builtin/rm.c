/*
 * "git rm" builtin command
 *
 * Copyright (C) Linus Torvalds 2006
 */

#define USE_THE_REPOSITORY_VARIABLE

#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "advice.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "lockfile.h"
#અન્તર્ભાવ "dir.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "hash.h"
#અન્તર્ભાવ "tree-walk.h"
#અન્તર્ભાવ "object-name.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "read-cache.h"

#અન્તર્ભાવ "string-list.h"
#અન્તર્ભાવ "setup.h"
#અન્તર્ભાવ "sparse-index.h"
#અન્તર્ભાવ "submodule.h"
#અન્તર્ભાવ "pathspec.h"

સ્થિર અચલ અક્ષર * અચલ builtin_rm_usage[] = {
	N_("git rm [-f | --force] [-n] [-r] [--cached] [--ignore-unmatch]\n"
	   "       [--quiet] [--pathspec-from-file=<file> [--pathspec-file-nul]]\n"
	   "       [--] [<pathspec>...]"),
	ખાલી
};

સ્થિર સંરચના {
	પૂર્ણાંક nr, alloc;
	સંરચના {
		અચલ અક્ષર *name;
		અક્ષર is_submodule;
	} *entry;
} list;

સ્થિર પૂર્ણાંક get_ours_cache_pos(અચલ અક્ષર *path, unsigned પૂર્ણાંક pos)
{
	જ્યારે ((pos < the_repository->index->cache_nr) && !strcmp(the_repository->index->cache[pos]->name, path)) {
		જો (ce_stage(the_repository->index->cache[pos]) == 2)
			પાછા pos;
		pos++;
	}
	પાછા -1;
}

સ્થિર શૂન્ય print_error_files(સંરચના string_list *files_list,
			      અચલ અક્ષર *main_સંદેશ,
			      અચલ અક્ષર *hints_સંદેશ,
			      પૂર્ણાંક *errs)
{
	જો (files_list->nr) {
		unsigned પૂર્ણાંક i;
		સંરચના strbuf err_સંદેશ = STRBUF_INIT;

		strbuf_addstr(&err_સંદેશ, main_સંદેશ);
		માટે (i = 0; i < files_list->nr; i++)
			strbuf_addf(&err_સંદેશ,
				    "\n    %s",
				    files_list->વસ્તુઓ[i].string);
		જો (advice_enabled(ADVICE_RM_HINTS))
			strbuf_addstr(&err_સંદેશ, hints_સંદેશ);
		*errs = error("%s", err_સંદેશ.buf);
		strbuf_release(&err_સંદેશ);
	}
}

સ્થિર શૂન્ય submodules_absorb_gitdir_if_needed(શૂન્ય)
{
	પૂર્ણાંક i;
	માટે (i = 0; i < list.nr; i++) {
		અચલ અક્ષર *name = list.entry[i].name;
		પૂર્ણાંક pos;
		અચલ સંરચના cache_entry *ce;

		pos = index_name_pos(the_repository->index, name, strlen(name));
		જો (pos < 0) {
			pos = get_ours_cache_pos(name, -pos - 1);
			જો (pos < 0)
				ચાલુ;
		}
		ce = the_repository->index->cache[pos];

		જો (!S_ISGITLINK(ce->ce_mode) ||
		    !file_exists(ce->name) ||
		    is_empty_dir(name))
			ચાલુ;

		જો (!submodule_uses_gitfile(name))
			absorb_git_dir_into_superproject(name, ખાલી);
	}
}

સ્થિર પૂર્ણાંક check_local_mod(સંરચના object_id *head, પૂર્ણાંક index_only)
{
	/*
	 * Items in list are already sorted in the cache order,
	 * so we could do this a lot more efficiently by using
	 * tree_desc based traversal if we wanted to, but I am
	 * lazy, and who cares if removal of files is a tad
	 * slower than the theoretical maximum speed?
	 */
	પૂર્ણાંક i, no_head;
	પૂર્ણાંક errs = 0;
	સંરચના string_list files_staged = STRING_LIST_INIT_NODUP;
	સંરચના string_list files_cached = STRING_LIST_INIT_NODUP;
	સંરચના string_list files_local = STRING_LIST_INIT_NODUP;

	no_head = is_null_oid(head);
	માટે (i = 0; i < list.nr; i++) {
		સંરચના stat st;
		પૂર્ણાંક pos;
		અચલ સંરચના cache_entry *ce;
		અચલ અક્ષર *name = list.entry[i].name;
		સંરચના object_id oid;
		unsigned short mode;
		પૂર્ણાંક local_changes = 0;
		પૂર્ણાંક staged_changes = 0;

		pos = index_name_pos(the_repository->index, name, strlen(name));
		જો (pos < 0) {
			/*
			 * Skip unmerged entries except for populated submodules
			 * that could lose history when removed.
			 */
			pos = get_ours_cache_pos(name, -pos - 1);
			જો (pos < 0)
				ચાલુ;

			જો (!S_ISGITLINK(the_repository->index->cache[pos]->ce_mode) ||
			    is_empty_dir(name))
				ચાલુ;
		}
		ce = the_repository->index->cache[pos];

		જો (lstat(ce->name, &st) < 0) {
			જો (!is_missing_file_error(errno))
				warning_errno(_("failed to stat '%s'"), ce->name);
			/* It already vanished from the working tree */
			ચાલુ;
		}
		નહીંતર જો (S_ISDIR(st.st_mode)) {
			/* if a file was removed and it is now a
			 * directory, that is the same as ENOENT as
			 * far as git is concerned; we do not track
			 * directories unless they are submodules.
			 */
			જો (!S_ISGITLINK(ce->ce_mode))
				ચાલુ;
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
		જો (ie_match_stat(the_repository->index, ce, &st, 0) ||
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
		જો (no_head
		     || get_વૃક્ષ_entry(the_repository, head, name, &oid, &mode)
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
		જો (local_changes && staged_changes) {
			જો (!index_only || !ce_intent_to_add(ce))
				string_list_append(&files_staged, name);
		}
		નહીંતર જો (!index_only) {
			જો (staged_changes)
				string_list_append(&files_cached, name);
			જો (local_changes)
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

	પાછા errs;
}

સ્થિર પૂર્ણાંક show_only = 0, force = 0, index_only = 0, recursive = 0, quiet = 0;
સ્થિર પૂર્ણાંક ignore_unmatch = 0, pathspec_file_nul;
સ્થિર પૂર્ણાંક include_sparse;
સ્થિર અક્ષર *pathspec_from_file;

સ્થિર સંરચના option builtin_rm_options[] = {
	OPT__DRY_RUN(&show_only, N_("dry run")),
	OPT__QUIET(&quiet, N_("do not list removed files")),
	OPT_BOOL( 0 , "cached",         &index_only, N_("only remove from the index")),
	OPT__FORCE(&force, N_("override the up-to-date check"), PARSE_OPT_NOCOMPLETE),
	OPT_BOOL('r', ખાલી,             &recursive,  N_("allow recursive removal")),
	OPT_BOOL( 0 , "ignore-unmatch", &ignore_unmatch,
				N_("exit with a zero status even if nothing matched")),
	OPT_BOOL(0, "sparse", &include_sparse, N_("allow updating entries outside of the sparse-checkout cone")),
	OPT_PATHSPEC_FROM_FILE(&pathspec_from_file),
	OPT_PATHSPEC_FILE_NUL(&pathspec_file_nul),
	OPT_END(),
};

પૂર્ણાંક cmd_rm(પૂર્ણાંક argc,
	   અચલ અક્ષર **argv,
	   અચલ અક્ષર *prefix,
	   સંરચના repository *repo UNUSED)
{
	સંરચના lock_file lock_file = LOCK_INIT;
	પૂર્ણાંક i, ret = 0;
	સંરચના pathspec pathspec;
	અક્ષર *seen;

	repo_config(the_repository, git_default_config, ખાલી);

	argc = parse_options(argc, argv, prefix, builtin_rm_options,
			     builtin_rm_usage, 0);

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD,
		       prefix, argv);

	જો (pathspec_from_file) {
		જો (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_CWD,
				    prefix, pathspec_from_file, pathspec_file_nul);
	} નહીંતર જો (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	જો (!pathspec.nr)
		die(_("No pathspec was given. Which files should I remove?"));

	જો (!index_only)
		setup_work_વૃક્ષ(the_repository);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;
	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);

	જો (repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));

	refresh_index(the_repository->index, REFRESH_QUIET|REFRESH_UNMERGED, &pathspec, ખાલી, ખાલી);

	seen = xcalloc(pathspec.nr, 1);

	જો (pathspec_needs_expanded_index(the_repository->index, &pathspec))
		ensure_full_index(the_repository->index);

	માટે (unsigned પૂર્ણાંક i = 0; i < the_repository->index->cache_nr; i++) {
		અચલ સંરચના cache_entry *ce = the_repository->index->cache[i];

		જો (!include_sparse &&
		    (ce_skip_worktree(ce) ||
		     !path_in_sparse_checkout(ce->name, the_repository->index)))
			ચાલુ;
		જો (!ce_path_match(the_repository->index, ce, &pathspec, seen))
			ચાલુ;
		ALLOC_GROW(list.entry, list.nr + 1, list.alloc);
		list.entry[list.nr].name = xstrdup(ce->name);
		list.entry[list.nr].is_submodule = S_ISGITLINK(ce->ce_mode);
		જો (list.entry[list.nr++].is_submodule &&
		    !is_staging_gitmodules_ok(the_repository->index))
			die(_("please stage your changes to .gitmodules or stash them to proceed"));
	}

	જો (pathspec.nr) {
		અચલ અક્ષર *original;
		પૂર્ણાંક seen_any = 0;
		અક્ષર *skip_worktree_seen = ખાલી;
		સંરચના string_list only_match_skip_worktree = STRING_LIST_INIT_NODUP;

		માટે (i = 0; i < pathspec.nr; i++) {
			original = pathspec.વસ્તુઓ[i].original;
			જો (seen[i])
				seen_any = 1;
			નહીંતર જો (ignore_unmatch)
				ચાલુ;
			નહીંતર જો (!include_sparse &&
				 matches_skip_worktree(&pathspec, i, &skip_worktree_seen))
				string_list_append(&only_match_skip_worktree, original);
			નહીંતર
				die(_("pathspec '%s' did not match any files"), original);

			જો (!recursive && seen[i] == MATCHED_RECURSIVELY)
				die(_("not removing '%s' recursively without -r"),
				    *original ? original : ".");
		}

		જો (only_match_skip_worktree.nr) {
			advise_on_updating_sparse_paths(&only_match_skip_worktree);
			ret = 1;
		}
		મુક્ત(skip_worktree_seen);
		string_list_clear(&only_match_skip_worktree, 0);

		જો (!seen_any)
			exit(ret);
	}
	clear_pathspec(&pathspec);
	મુક્ત(seen);

	જો (!index_only)
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
	જો (!force) {
		સંરચના object_id oid;
		જો (repo_get_oid(the_repository, "HEAD", &oid))
			oidclr(&oid, the_repository->hash_algo);
		જો (check_local_mod(&oid, index_only))
			exit(1);
	}

	/*
	 * First remove the names from the index: we won't commit
	 * the index unless all of them succeed.
	 */
	માટે (i = 0; i < list.nr; i++) {
		અચલ અક્ષર *path = list.entry[i].name;
		જો (!quiet)
			printf("rm '%s'\n", path);

		જો (remove_file_from_index(the_repository->index, path))
			die(_("git rm: unable to remove %s"), path);
	}

	જો (show_only)
		પાછા 0;

	/*
	 * Then, unless we used "--cached", remove the filenames from
	 * the workspace. If we fail to remove the first one, we
	 * abort the "git rm" (but once we've successfully removed
	 * any file at all, we'll go ahead and commit to it all:
	 * by then we've already committed ourselves and can't fail
	 * in the middle)
	 */
	જો (!index_only) {
		પૂર્ણાંક removed = 0, gitmodules_modified = 0;
		સંરચના strbuf buf = STRBUF_INIT;
		પૂર્ણાંક flag = force ? REMOVE_DIR_PURGE_ORIGINAL_CWD : 0;
		માટે (i = 0; i < list.nr; i++) {
			અચલ અક્ષર *path = list.entry[i].name;
			જો (list.entry[i].is_submodule) {
				strbuf_reset(&buf);
				strbuf_addstr(&buf, path);
				જો (remove_dir_recursively(&buf, flag))
					die(_("could not remove '%s'"), path);

				removed = 1;
				જો (!remove_path_from_gitmodules(path))
					gitmodules_modified = 1;
				ચાલુ;
			}
			જો (!remove_path(path)) {
				removed = 1;
				ચાલુ;
			}
			જો (!removed)
				die_errno("git rm: '%s'", path);
		}
		strbuf_release(&buf);
		જો (gitmodules_modified)
			stage_updated_gitmodules(the_repository->index);
	}

	જો (write_locked_index(the_repository->index, &lock_file,
			       COMMIT_LOCK | SKIP_IF_UNCHANGED))
		die(_("Unable to write new index file"));

	પાછા ret;
}
