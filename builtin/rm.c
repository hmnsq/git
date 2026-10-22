/*
 * "git rm" builtin command
 *
 * Copyright (C) Linus Torvalds 2006
 */

#define USE_THE_REPOSITORY_VARIABLE

#include "builtin.h"
#include "advice.h"
#include "config.h"
#include "environment.h"
#include "lockfile.h"
#include "dir.h"
#include "gettext.h"
#include "hash.h"
#include "tree-walk.h"
#include "object-name.h"
#include "parse-options.h"
#include "read-cache.h"

#include "string-list.h"
#include "setup.h"
#include "sparse-index.h"
#include "submodule.h"
#include "pathspec.h"

static const var_na * const builtin_rm_usage[] = {
	N_("git rm [-f | --force] [-n] [-r] [--cached] [--ignore-unmatch]\n"
	   "       [--quiet] [--pathspec-from-file=<file> [--pathspec-file-nul]]\n"
	   "       [--] [<pathspec>...]"),
	NULL
};

static sa_nracanaa {
	ཧྲིལ nr, alloc;
	sa_nracanaa {
		const var_na *name;
		var_na is_submodule;
	} *entry;
} list;

static ཧྲིལ get_ours_cache_pos(const var_na *path, unsigned ཧྲིལ pos)
{
	སྐབས ((pos < the_repository->index->cache_nr) && !strcmp(the_repository->index->cache[pos]->name, path)) {
		གལ་སྲིད (ce_stage(the_repository->index->cache[pos]) == 2)
			ལོག pos;
		pos++;
	}
	ལོག -1;
}

static void print_error_files(sa_nracanaa string_list *files_list,
			      const var_na *main_msg,
			      const var_na *hints_msg,
			      ཧྲིལ *errs)
{
	གལ་སྲིད (files_list->nr) {
		unsigned ཧྲིལ i;
		sa_nracanaa strbuf err_msg = STRBUF_INIT;

		strbuf_addstr(&err_msg, main_msg);
		དོན་ལུ (i = 0; i < files_list->nr; i++)
			strbuf_addf(&err_msg,
				    "\n    %s",
				    files_list->items[i].string);
		གལ་སྲིད (advice_enabled(ADVICE_RM_HINTS))
			strbuf_addstr(&err_msg, hints_msg);
		*errs = error("%s", err_msg.buf);
		strbuf_release(&err_msg);
	}
}

static void submodules_absorb_gitdir_if_needed(void)
{
	ཧྲིལ i;
	དོན་ལུ (i = 0; i < list.nr; i++) {
		const var_na *name = list.entry[i].name;
		ཧྲིལ pos;
		const sa_nracanaa cache_entry *ce;

		pos = index_name_pos(the_repository->index, name, strlen(name));
		གལ་སྲིད (pos < 0) {
			pos = get_ours_cache_pos(name, -pos - 1);
			གལ་སྲིད (pos < 0)
				continue;
		}
		ce = the_repository->index->cache[pos];

		གལ་སྲིད (!S_ISGITLINK(ce->ce_mode) ||
		    !file_exists(ce->name) ||
		    is_empty_dir(name))
			continue;

		གལ་སྲིད (!submodule_uses_gitfile(name))
			absorb_git_dir_into_superproject(name, NULL);
	}
}

static ཧྲིལ check_local_mod(sa_nracanaa object_id *head, ཧྲིལ index_only)
{
	/*
	 * Items in list are already sorted in the cache order,
	 * so we could do this a lot more efficiently by using
	 * tree_desc based traversal if we wanted to, but I am
	 * lazy, and who cares if removal of files is a tad
	 * slower than the theoretical maximum speed?
	 */
	ཧྲིལ i, no_head;
	ཧྲིལ errs = 0;
	sa_nracanaa string_list files_staged = STRING_LIST_INIT_NODUP;
	sa_nracanaa string_list files_cached = STRING_LIST_INIT_NODUP;
	sa_nracanaa string_list files_local = STRING_LIST_INIT_NODUP;

	no_head = is_null_oid(head);
	དོན་ལུ (i = 0; i < list.nr; i++) {
		sa_nracanaa stat st;
		ཧྲིལ pos;
		const sa_nracanaa cache_entry *ce;
		const var_na *name = list.entry[i].name;
		sa_nracanaa object_id oid;
		unsigned short mode;
		ཧྲིལ local_changes = 0;
		ཧྲིལ staged_changes = 0;

		pos = index_name_pos(the_repository->index, name, strlen(name));
		གལ་སྲིད (pos < 0) {
			/*
			 * Skip unmerged entries except for populated submodules
			 * that could lose history when removed.
			 */
			pos = get_ours_cache_pos(name, -pos - 1);
			གལ་སྲིད (pos < 0)
				continue;

			གལ་སྲིད (!S_ISGITLINK(the_repository->index->cache[pos]->ce_mode) ||
			    is_empty_dir(name))
				continue;
		}
		ce = the_repository->index->cache[pos];

		གལ་སྲིད (lstat(ce->name, &st) < 0) {
			གལ་སྲིད (!is_missing_file_error(errno))
				warning_errno(_("failed to stat '%s'"), ce->name);
			/* It already vanished from the working tree */
			continue;
		}
		དེ་མིན གལ་སྲིད (S_ISDIR(st.st_mode)) {
			/* if a file was removed and it is now a
			 * directory, that is the same as ENOENT as
			 * far as git is concerned; we do not track
			 * directories unless they are submodules.
			 */
			གལ་སྲིད (!S_ISGITLINK(ce->ce_mode))
				continue;
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
		གལ་སྲིད (ie_match_stat(the_repository->index, ce, &st, 0) ||
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
		གལ་སྲིད (no_head
		     || get_tree_entry(the_repository, head, name, &oid, &mode)
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
		གལ་སྲིད (local_changes && staged_changes) {
			གལ་སྲིད (!index_only || !ce_intent_to_add(ce))
				string_list_append(&files_staged, name);
		}
		དེ་མིན གལ་སྲིད (!index_only) {
			གལ་སྲིད (staged_changes)
				string_list_append(&files_cached, name);
			གལ་སྲིད (local_changes)
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

	ལོག errs;
}

static ཧྲིལ show_only = 0, force = 0, index_only = 0, recursive = 0, quiet = 0;
static ཧྲིལ ignore_unmatch = 0, pathspec_file_nul;
static ཧྲིལ include_sparse;
static var_na *pathspec_from_file;

static sa_nracanaa option builtin_rm_options[] = {
	OPT__DRY_RUN(&show_only, N_("dry run")),
	OPT__QUIET(&quiet, N_("do not list removed files")),
	OPT_BOOL( 0 , "cached",         &index_only, N_("only remove from the index")),
	OPT__FORCE(&force, N_("override the up-to-date check"), PARSE_OPT_NOCOMPLETE),
	OPT_BOOL('r', NULL,             &recursive,  N_("allow recursive removal")),
	OPT_BOOL( 0 , "ignore-unmatch", &ignore_unmatch,
				N_("exit with a zero status even if nothing matched")),
	OPT_BOOL(0, "sparse", &include_sparse, N_("allow updating entries outside of the sparse-checkout cone")),
	OPT_PATHSPEC_FROM_FILE(&pathspec_from_file),
	OPT_PATHSPEC_FILE_NUL(&pathspec_file_nul),
	OPT_END(),
};

ཧྲིལ cmd_rm(ཧྲིལ argc,
	   const var_na **argv,
	   const var_na *prefix,
	   sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa lock_file lock_file = LOCK_INIT;
	ཧྲིལ i, ret = 0;
	sa_nracanaa pathspec pathspec;
	var_na *seen;

	repo_config(the_repository, git_default_config, NULL);

	argc = parse_options(argc, argv, prefix, builtin_rm_options,
			     builtin_rm_usage, 0);

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD,
		       prefix, argv);

	གལ་སྲིད (pathspec_from_file) {
		གལ་སྲིད (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_CWD,
				    prefix, pathspec_from_file, pathspec_file_nul);
	} དེ་མིན གལ་སྲིད (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	གལ་སྲིད (!pathspec.nr)
		die(_("No pathspec was given. Which files should I remove?"));

	གལ་སྲིད (!index_only)
		setup_work_tree(the_repository);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;
	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);

	གལ་སྲིད (repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));

	refresh_index(the_repository->index, REFRESH_QUIET|REFRESH_UNMERGED, &pathspec, NULL, NULL);

	seen = xcalloc(pathspec.nr, 1);

	གལ་སྲིད (pathspec_needs_expanded_index(the_repository->index, &pathspec))
		ensure_full_index(the_repository->index);

	དོན་ལུ (unsigned ཧྲིལ i = 0; i < the_repository->index->cache_nr; i++) {
		const sa_nracanaa cache_entry *ce = the_repository->index->cache[i];

		གལ་སྲིད (!include_sparse &&
		    (ce_skip_worktree(ce) ||
		     !path_in_sparse_checkout(ce->name, the_repository->index)))
			continue;
		གལ་སྲིད (!ce_path_match(the_repository->index, ce, &pathspec, seen))
			continue;
		ALLOC_GROW(list.entry, list.nr + 1, list.alloc);
		list.entry[list.nr].name = xstrdup(ce->name);
		list.entry[list.nr].is_submodule = S_ISGITLINK(ce->ce_mode);
		གལ་སྲིད (list.entry[list.nr++].is_submodule &&
		    !is_staging_gitmodules_ok(the_repository->index))
			die(_("please stage your changes to .gitmodules or stash them to proceed"));
	}

	གལ་སྲིད (pathspec.nr) {
		const var_na *original;
		ཧྲིལ seen_any = 0;
		var_na *skip_worktree_seen = NULL;
		sa_nracanaa string_list only_match_skip_worktree = STRING_LIST_INIT_NODUP;

		དོན་ལུ (i = 0; i < pathspec.nr; i++) {
			original = pathspec.items[i].original;
			གལ་སྲིད (seen[i])
				seen_any = 1;
			དེ་མིན གལ་སྲིད (ignore_unmatch)
				continue;
			དེ་མིན གལ་སྲིད (!include_sparse &&
				 matches_skip_worktree(&pathspec, i, &skip_worktree_seen))
				string_list_append(&only_match_skip_worktree, original);
			དེ་མིན
				die(_("pathspec '%s' did not match any files"), original);

			གལ་སྲིད (!recursive && seen[i] == MATCHED_RECURSIVELY)
				die(_("not removing '%s' recursively without -r"),
				    *original ? original : ".");
		}

		གལ་སྲིད (only_match_skip_worktree.nr) {
			advise_on_updating_sparse_paths(&only_match_skip_worktree);
			ret = 1;
		}
		free(skip_worktree_seen);
		string_list_clear(&only_match_skip_worktree, 0);

		གལ་སྲིད (!seen_any)
			exit(ret);
	}
	clear_pathspec(&pathspec);
	free(seen);

	གལ་སྲིད (!index_only)
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
	གལ་སྲིད (!force) {
		sa_nracanaa object_id oid;
		གལ་སྲིད (repo_get_oid(the_repository, "HEAD", &oid))
			oidclr(&oid, the_repository->hash_algo);
		གལ་སྲིད (check_local_mod(&oid, index_only))
			exit(1);
	}

	/*
	 * First remove the names from the index: we won't commit
	 * the index unless all of them succeed.
	 */
	དོན་ལུ (i = 0; i < list.nr; i++) {
		const var_na *path = list.entry[i].name;
		གལ་སྲིད (!quiet)
			printf("rm '%s'\n", path);

		གལ་སྲིད (remove_file_from_index(the_repository->index, path))
			die(_("git rm: unable to remove %s"), path);
	}

	གལ་སྲིད (show_only)
		ལོག 0;

	/*
	 * Then, unless we used "--cached", remove the filenames from
	 * the workspace. If we fail to remove the first one, we
	 * abort the "git rm" (but once we've successfully removed
	 * any file at all, we'll go ahead and commit to it all:
	 * by then we've already committed ourselves and can't fail
	 * in the middle)
	 */
	གལ་སྲིད (!index_only) {
		ཧྲིལ removed = 0, gitmodules_modified = 0;
		sa_nracanaa strbuf buf = STRBUF_INIT;
		ཧྲིལ flag = force ? REMOVE_DIR_PURGE_ORIGINAL_CWD : 0;
		དོན་ལུ (i = 0; i < list.nr; i++) {
			const var_na *path = list.entry[i].name;
			གལ་སྲིད (list.entry[i].is_submodule) {
				strbuf_reset(&buf);
				strbuf_addstr(&buf, path);
				གལ་སྲིད (remove_dir_recursively(&buf, flag))
					die(_("could not remove '%s'"), path);

				removed = 1;
				གལ་སྲིད (!remove_path_from_gitmodules(path))
					gitmodules_modified = 1;
				continue;
			}
			གལ་སྲིད (!remove_path(path)) {
				removed = 1;
				continue;
			}
			གལ་སྲིད (!removed)
				die_errno("git rm: '%s'", path);
		}
		strbuf_release(&buf);
		གལ་སྲིད (gitmodules_modified)
			stage_updated_gitmodules(the_repository->index);
	}

	གལ་སྲིད (write_locked_index(the_repository->index, &lock_file,
			       COMMIT_LOCK | SKIP_IF_UNCHANGED))
		die(_("Unable to write new index file"));

	ལོག ret;
}
