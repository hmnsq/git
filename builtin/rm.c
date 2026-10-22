/*
 * "git rm" builtin command
 *
 * Copyright (C) Linus Torvalds 2006
 */

#నిర్వచించు USE_THE_REPOSITORY_VARIABLE

#అన్తర్భావ "builtin.h"
#అన్తర్భావ "advice.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "lockfile.h"
#అన్తర్భావ "dir.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "hash.h"
#అన్తర్భావ "tree-walk.h"
#అన్తర్భావ "object-name.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "read-cache.h"

#అన్తర్భావ "string-list.h"
#అన్తర్భావ "setup.h"
#అన్తర్భావ "sparse-index.h"
#అన్తర్భావ "submodule.h"
#అన్తర్భావ "pathspec.h"

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి builtin_rm_usage[] = {
	N_("git rm [-f | --force] [-n] [-r] [--cached] [--ignore-unmatch]\n"
	   "       [--quiet] [--pathspec-from-file=<file> [--pathspec-file-nul]]\n"
	   "       [--] [<pathspec>...]"),
	ఖాళీ
};

స్థిర నిర్మాణం {
	పూర్ణాంకం nr, alloc;
	నిర్మాణం {
		స్థిరరాశి అక్షరం *name;
		అక్షరం is_submodule;
	} *నమోదు_2;
} list;

స్థిర పూర్ణాంకం get_ours_cache_pos(స్థిరరాశి అక్షరం *మార్గం, గుర్తులేని పూర్ణాంకం pos)
{
	అయితే ((pos < the_repository->సూచీ->cache_nr) && !strcmp(the_repository->సూచీ->cache[pos]->name, మార్గం)) {
		ఒకవేళ (ce_stage(the_repository->సూచీ->cache[pos]) == 2)
			తిరిగి pos;
		pos++;
	}
	తిరిగి -1;
}

స్థిర శూన్య print_error_files(నిర్మాణం string_list *files_list,
			      స్థిరరాశి అక్షరం *main_సందేశం,
			      స్థిరరాశి అక్షరం *hints_సందేశం,
			      పూర్ణాంకం *errs)
{
	ఒకవేళ (files_list->nr) {
		గుర్తులేని పూర్ణాంకం i;
		నిర్మాణం strbuf err_సందేశం = STRBUF_INIT;

		strbuf_addstr(&err_సందేశం, main_సందేశం);
		కొరకు (i = 0; i < files_list->nr; i++)
			strbuf_addf(&err_సందేశం,
				    "\n    %s",
				    files_list->వస్తువులు[i].string);
		ఒకవేళ (advice_enabled(ADVICE_RM_HINTS))
			strbuf_addstr(&err_సందేశం, hints_సందేశం);
		*errs = error("%s", err_సందేశం.buf);
		strbuf_release(&err_సందేశం);
	}
}

స్థిర శూన్య submodules_absorb_gitdir_if_needed(శూన్య)
{
	పూర్ణాంకం i;
	కొరకు (i = 0; i < list.nr; i++) {
		స్థిరరాశి అక్షరం *name = list.నమోదు_2[i].name;
		పూర్ణాంకం pos;
		స్థిరరాశి నిర్మాణం cache_నమోదు *ce;

		pos = సూచీ_name_pos(the_repository->సూచీ, name, అక్షరశ్రేణి_పొడవు(name));
		ఒకవేళ (pos < 0) {
			pos = get_ours_cache_pos(name, -pos - 1);
			ఒకవేళ (pos < 0)
				కొనసాగు;
		}
		ce = the_repository->సూచీ->cache[pos];

		ఒకవేళ (!S_ISGITLINK(ce->ce_బహుళకం) ||
		    !file_exists(ce->name) ||
		    is_ఖాళీలు_dir(name))
			కొనసాగు;

		ఒకవేళ (!submodule_ఉపయోగిస్తుంది_gitfile(name))
			absorb_git_dir_into_superproject(name, ఖాళీ);
	}
}

స్థిర పూర్ణాంకం check_local_mod(నిర్మాణం object_id *తల, పూర్ణాంకం సూచీ_only)
{
	/*
	 * Items in list are already sorted in the cache order,
	 * so we could do this a lot more efficiently by using
	 * tree_desc based traversal if we wanted to, but I am
	 * lazy, and who cares if removal of files is a tad
	 * slower than the theoretical maximum speed?
	 */
	పూర్ణాంకం i, no_తల;
	పూర్ణాంకం errs = 0;
	నిర్మాణం string_list files_staged = STRING_LIST_INIT_NODUP;
	నిర్మాణం string_list files_cached = STRING_LIST_INIT_NODUP;
	నిర్మాణం string_list files_local = STRING_LIST_INIT_NODUP;

	no_తల = is_null_oid(తల);
	కొరకు (i = 0; i < list.nr; i++) {
		నిర్మాణం stat st;
		పూర్ణాంకం pos;
		స్థిరరాశి నిర్మాణం cache_నమోదు *ce;
		స్థిరరాశి అక్షరం *name = list.నమోదు_2[i].name;
		నిర్మాణం object_id oid;
		గుర్తులేని పొట్టి బహుళకం;
		పూర్ణాంకం local_changes = 0;
		పూర్ణాంకం staged_changes = 0;

		pos = సూచీ_name_pos(the_repository->సూచీ, name, అక్షరశ్రేణి_పొడవు(name));
		ఒకవేళ (pos < 0) {
			/*
			 * Skip unmerged entries except for populated submodules
			 * that could lose history when removed.
			 */
			pos = get_ours_cache_pos(name, -pos - 1);
			ఒకవేళ (pos < 0)
				కొనసాగు;

			ఒకవేళ (!S_ISGITLINK(the_repository->సూచీ->cache[pos]->ce_బహుళకం) ||
			    is_ఖాళీలు_dir(name))
				కొనసాగు;
		}
		ce = the_repository->సూచీ->cache[pos];

		ఒకవేళ (lstat(ce->name, &st) < 0) {
			ఒకవేళ (!is_missing_file_error(errno))
				warning_errno(_("failed to stat '%s'"), ce->name);
			/* It already vanished from the working tree */
			కొనసాగు;
		}
		లేకపోతే ఒకవేళ (S_ISDIR(st.st_బహుళకం)) {
			/* if a file was removed and it is now a
			 * directory, that is the same as ENOENT as
			 * far as git is concerned; we do not track
			 * directories unless they are submodules.
			 */
			ఒకవేళ (!S_ISGITLINK(ce->ce_బహుళకం))
				కొనసాగు;
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
		ఒకవేళ (ie_సరిపోలిక_stat(the_repository->సూచీ, ce, &st, 0) ||
		    (S_ISGITLINK(ce->ce_బహుళకం) &&
		     bad_గమ్యం_remove_submodule(ce->name,
				SUBMODULE_REMOVAL_DIE_ON_ERROR |
				SUBMODULE_REMOVAL_IGNORE_IGNORED_UNTRACKED)))
			local_changes = 1;

		/*
		 * Is the index different from the HEAD commit?  By
		 * definition, before the very initial commit,
		 * anything staged in the index is treated by the same
		 * way as changed from the HEAD.
		 */
		ఒకవేళ (no_తల
		     || get_వృక్షం_నమోదు(the_repository, తల, name, &oid, &బహుళకం)
		     || ce->ce_బహుళకం != create_ce_బహుళకం(బహుళకం)
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
		ఒకవేళ (local_changes && staged_changes) {
			ఒకవేళ (!సూచీ_only || !ce_intent_గమ్యం_కూడు(ce))
				string_list_append(&files_staged, name);
		}
		లేకపోతే ఒకవేళ (!సూచీ_only) {
			ఒకవేళ (staged_changes)
				string_list_append(&files_cached, name);
			ఒకవేళ (local_changes)
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

	తిరిగి errs;
}

స్థిర పూర్ణాంకం show_only = 0, force = 0, సూచీ_only = 0, recursive = 0, quiet = 0;
స్థిర పూర్ణాంకం ignore_unmatch = 0, pathspec_file_nul;
స్థిర పూర్ణాంకం include_sparse;
స్థిర అక్షరం *pathspec_from_file;

స్థిర నిర్మాణం option builtin_rm_options[] = {
	OPT_DRY_నిరంతరం(&show_only, N_("dry run")),
	OPT__QUIET(&quiet, N_("do not list removed files")),
	OPT_BOOL( 0 , "cached",         &సూచీ_only, N_("only remove from the index")),
	OPT__FORCE(&force, N_("override the up-to-date check"), PARSE_OPT_NOCOMPLETE),
	OPT_BOOL('r', ఖాళీ,             &recursive,  N_("allow recursive removal")),
	OPT_BOOL( 0 , "ignore-unmatch", &ignore_unmatch,
				N_("exit with a zero status even if nothing matched")),
	OPT_BOOL(0, "sparse", &include_sparse, N_("allow updating entries outside of the sparse-checkout cone")),
	OPT_PATHSPEC_FROM_FILE(&pathspec_from_file),
	OPT_PATHSPEC_FILE_NUL(&pathspec_file_nul),
	OPT_చివర(),
};

పూర్ణాంకం cmd_rm(పూర్ణాంకం argc,
	   స్థిరరాశి అక్షరం **argv,
	   స్థిరరాశి అక్షరం *ఉపసర్గ,
	   నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం తాళం_file తాళం_file = తాళం_INIT;
	పూర్ణాంకం i, ret = 0;
	నిర్మాణం pathspec pathspec;
	అక్షరం *చూసిన;

	repo_config(the_repository, git_default_config, ఖాళీ);

	argc = parse_options(argc, argv, ఉపసర్గ, builtin_rm_options,
			     builtin_rm_usage, 0);

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD,
		       ఉపసర్గ, argv);

	ఒకవేళ (pathspec_from_file) {
		ఒకవేళ (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_CWD,
				    ఉపసర్గ, pathspec_from_file, pathspec_file_nul);
	} లేకపోతే ఒకవేళ (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	ఒకవేళ (!pathspec.nr)
		die(_("No pathspec was given. Which files should I remove?"));

	ఒకవేళ (!సూచీ_only)
		అమరిక_work_వృక్షం(the_repository);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_నిండిన_సూచీ = 0;
	repo_hold_locked_సూచీ(the_repository, &తాళం_file, తాళం_DIE_ON_ERROR);

	ఒకవేళ (repo_read_సూచీ(the_repository) < 0)
		die(_("index file corrupt"));

	refresh_సూచీ(the_repository->సూచీ, REFRESH_QUIET|REFRESH_UNMERGED, &pathspec, ఖాళీ, ఖాళీ);

	చూసిన = xcalloc(pathspec.nr, 1);

	ఒకవేళ (pathspec_needs_expanded_సూచీ(the_repository->సూచీ, &pathspec))
		ensure_నిండిన_సూచీ(the_repository->సూచీ);

	కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < the_repository->సూచీ->cache_nr; i++) {
		స్థిరరాశి నిర్మాణం cache_నమోదు *ce = the_repository->సూచీ->cache[i];

		ఒకవేళ (!include_sparse &&
		    (ce_skip_worktree(ce) ||
		     !మార్గం_లోపల_sparse_checkout(ce->name, the_repository->సూచీ)))
			కొనసాగు;
		ఒకవేళ (!ce_మార్గం_సరిపోలిక(the_repository->సూచీ, ce, &pathspec, చూసిన))
			కొనసాగు;
		ALLOC_GROW(list.నమోదు_2, list.nr + 1, list.alloc);
		list.నమోదు_2[list.nr].name = xstrdup(ce->name);
		list.నమోదు_2[list.nr].is_submodule = S_ISGITLINK(ce->ce_బహుళకం);
		ఒకవేళ (list.నమోదు_2[list.nr++].is_submodule &&
		    !is_staging_gitmodules_ok(the_repository->సూచీ))
			die(_("please stage your changes to .gitmodules or stash them to proceed"));
	}

	ఒకవేళ (pathspec.nr) {
		స్థిరరాశి అక్షరం *original;
		పూర్ణాంకం చూసిన_any = 0;
		అక్షరం *skip_worktree_చూసిన = ఖాళీ;
		నిర్మాణం string_list only_సరిపోలిక_skip_worktree = STRING_LIST_INIT_NODUP;

		కొరకు (i = 0; i < pathspec.nr; i++) {
			original = pathspec.వస్తువులు[i].original;
			ఒకవేళ (చూసిన[i])
				చూసిన_any = 1;
			లేకపోతే ఒకవేళ (ignore_unmatch)
				కొనసాగు;
			లేకపోతే ఒకవేళ (!include_sparse &&
				 matches_skip_worktree(&pathspec, i, &skip_worktree_చూసిన))
				string_list_append(&only_సరిపోలిక_skip_worktree, original);
			లేకపోతే
				die(_("pathspec '%s' did not match any files"), original);

			ఒకవేళ (!recursive && చూసిన[i] == MATCHED_RECURSIVELY)
				die(_("not removing '%s' recursively without -r"),
				    *original ? original : ".");
		}

		ఒకవేళ (only_సరిపోలిక_skip_worktree.nr) {
			advise_on_updating_sparse_paths(&only_సరిపోలిక_skip_worktree);
			ret = 1;
		}
		విడుదల(skip_worktree_చూసిన);
		string_list_clear(&only_సరిపోలిక_skip_worktree, 0);

		ఒకవేళ (!చూసిన_any)
			నిష్క్రమించు(ret);
	}
	clear_pathspec(&pathspec);
	విడుదల(చూసిన);

	ఒకవేళ (!సూచీ_only)
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
	ఒకవేళ (!force) {
		నిర్మాణం object_id oid;
		ఒకవేళ (repo_get_oid(the_repository, "HEAD", &oid))
			oidclr(&oid, the_repository->hash_algo);
		ఒకవేళ (check_local_mod(&oid, సూచీ_only))
			నిష్క్రమించు(1);
	}

	/*
	 * First remove the names from the index: we won't commit
	 * the index unless all of them succeed.
	 */
	కొరకు (i = 0; i < list.nr; i++) {
		స్థిరరాశి అక్షరం *మార్గం = list.నమోదు_2[i].name;
		ఒకవేళ (!quiet)
			రాయి("rm '%s'\n", మార్గం);

		ఒకవేళ (remove_file_from_సూచీ(the_repository->సూచీ, మార్గం))
			die(_("git rm: unable to remove %s"), మార్గం);
	}

	ఒకవేళ (show_only)
		తిరిగి 0;

	/*
	 * Then, unless we used "--cached", remove the filenames from
	 * the workspace. If we fail to remove the first one, we
	 * abort the "git rm" (but once we've successfully removed
	 * any file at all, we'll go ahead and commit to it all:
	 * by then we've already committed ourselves and can't fail
	 * in the middle)
	 */
	ఒకవేళ (!సూచీ_only) {
		పూర్ణాంకం removed = 0, gitmodules_modified = 0;
		నిర్మాణం strbuf buf = STRBUF_INIT;
		పూర్ణాంకం flag = force ? REMOVE_DIR_PURGE_ORIGINAL_CWD : 0;
		కొరకు (i = 0; i < list.nr; i++) {
			స్థిరరాశి అక్షరం *మార్గం = list.నమోదు_2[i].name;
			ఒకవేళ (list.నమోదు_2[i].is_submodule) {
				strbuf_reset(&buf);
				strbuf_addstr(&buf, మార్గం);
				ఒకవేళ (remove_dir_recursively(&buf, flag))
					die(_("could not remove '%s'"), మార్గం);

				removed = 1;
				ఒకవేళ (!remove_మార్గం_from_gitmodules(మార్గం))
					gitmodules_modified = 1;
				కొనసాగు;
			}
			ఒకవేళ (!remove_మార్గం(మార్గం)) {
				removed = 1;
				కొనసాగు;
			}
			ఒకవేళ (!removed)
				die_errno("git rm: '%s'", మార్గం);
		}
		strbuf_release(&buf);
		ఒకవేళ (gitmodules_modified)
			stage_updated_gitmodules(the_repository->సూచీ);
	}

	ఒకవేళ (write_locked_సూచీ(the_repository->సూచీ, &తాళం_file,
			       COMMIT_తాళం | SKIP_IF_UNCHANGED))
		die(_("Unable to write new index file"));

	తిరిగి ret;
}
