/*
 * "git add" builtin command
 *
 * Copyright (C) 2006 Linus Torvalds
 */

#అన్తర్భావ "builtin.h"
#అన్తర్భావ "advice.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "lockfile.h"
#అన్తర్భావ "editor.h"
#అన్తర్భావ "dir.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "pathspec.h"
#అన్తర్భావ "object-file.h"
#అన్తర్భావ "odb.h"
#అన్తర్భావ "odb/transaction.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "path.h"
#అన్తర్భావ "preload-index.h"
#అన్తర్భావ "diff.h"
#అన్తర్భావ "read-cache.h"
#అన్తర్భావ "revision.h"
#అన్తర్భావ "submodule.h"
#అన్తర్భావ "add-interactive.h"
#అన్తర్భావ "merge-ll.h"
#అన్తర్భావ "apply.h"

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి builtin_కూడు_usage[] = {
	N_("git add [<options>] [--] <pathspec>..."),
	ఖాళీ
};
స్థిర పూర్ణాంకం patch_interactive, కూడు_interactive, edit_interactive;
స్థిర నిర్మాణం interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
స్థిర పూర్ణాంకం take_worktree_changes;
స్థిర పూర్ణాంకం కూడు_renormalize;
స్థిర పూర్ణాంకం కూడు_resolved;
స్థిర పూర్ణాంకం pathspec_file_nul;
స్థిర పూర్ణాంకం include_sparse;
స్థిర స్థిరరాశి అక్షరం *pathspec_from_file;

స్థిర పూర్ణాంకం chmod_pathspec(నిర్మాణం repository *repo,
			  నిర్మాణం pathspec *pathspec,
			  అక్షరం flip,
			  పూర్ణాంకం show_only)
{
	పూర్ణాంకం ret = 0;

	కొరకు (కొలత_t i = 0; i < repo->సూచీ->cache_nr; i++) {
		నిర్మాణం cache_నమోదు *ce = repo->సూచీ->cache[i];
		పూర్ణాంకం err;

		ఒకవేళ (!include_sparse &&
		    (ce_skip_worktree(ce) ||
		     !మార్గం_లోపల_sparse_checkout(ce->name, repo->సూచీ)))
			కొనసాగు;

		ఒకవేళ (pathspec && !ce_మార్గం_సరిపోలిక(repo->సూచీ, ce, pathspec, ఖాళీ))
			కొనసాగు;

		ఒకవేళ (!show_only)
			err = chmod_సూచీ_నమోదు(repo->సూచీ, ce, flip);
		లేకపోతే
			err = S_ISREG(ce->ce_బహుళకం) ? 0 : -1;

		ఒకవేళ (err < 0)
			ret = error(_("cannot chmod %cx '%s'"), flip, ce->name);
	}

	తిరిగి ret;
}

స్థిర పూర్ణాంకం renormalize_tracked_files(నిర్మాణం repository *repo,
				     స్థిరరాశి నిర్మాణం pathspec *pathspec,
				     పూర్ణాంకం flags)
{
	పూర్ణాంకం retval = 0;

	కొరకు (కొలత_t i = 0; i < repo->సూచీ->cache_nr; i++) {
		నిర్మాణం cache_నమోదు *ce = repo->సూచీ->cache[i];

		ఒకవేళ (!include_sparse &&
		    (ce_skip_worktree(ce) ||
		     !మార్గం_లోపల_sparse_checkout(ce->name, repo->సూచీ)))
			కొనసాగు;
		ఒకవేళ (ce_stage(ce))
			కొనసాగు; /* do not touch unmerged paths */
		ఒకవేళ (!S_ISREG(ce->ce_బహుళకం) && !S_ISLNK(ce->ce_బహుళకం))
			కొనసాగు; /* do not touch non blobs */
		ఒకవేళ (pathspec && !ce_మార్గం_సరిపోలిక(repo->సూచీ, ce, pathspec, ఖాళీ))
			కొనసాగు;
		retval |= కూడు_file_గమ్యం_సూచీ(repo->సూచీ, ce->name,
					    flags | కూడు_CACHE_RENORMALIZE);
	}

	తిరిగి retval;
}

స్థిర అక్షరం *prune_directory(నిర్మాణం repository *repo,
			     నిర్మాణం dir_struct *dir,
			     నిర్మాణం pathspec *pathspec,
			     పూర్ణాంకం ఉపసర్గ)
{
	అక్షరం *చూసిన;
	పూర్ణాంకం i;
	నిర్మాణం dir_నమోదు **src, **dst;

	చూసిన = xcalloc(pathspec->nr, 1);

	src = dst = dir->entries;
	i = dir->nr;
	అయితే (--i >= 0) {
		నిర్మాణం dir_నమోదు *నమోదు_2 = *src++;
		ఒకవేళ (dir_మార్గం_సరిపోలిక(repo->సూచీ, నమోదు_2, pathspec, ఉపసర్గ, చూసిన))
			*dst++ = నమోదు_2;
	}
	dir->nr = dst - dir->entries;
	కూడు_pathspec_matches_against_సూచీ(pathspec, repo->సూచీ, చూసిన,
					   PS_IGNORE_SKIP_WORKTREE);
	తిరిగి చూసిన;
}

స్థిర పూర్ణాంకం refresh(నిర్మాణం repository *repo, పూర్ణాంకం verbose, స్థిరరాశి నిర్మాణం pathspec *pathspec)
{
	అక్షరం *చూసిన;
	పూర్ణాంకం i, ret = 0;
	అక్షరం *skip_worktree_చూసిన = ఖాళీ;
	నిర్మాణం string_list only_సరిపోలిక_skip_worktree = STRING_LIST_INIT_NODUP;
	గుర్తులేని పూర్ణాంకం flags = REFRESH_IGNORE_SKIP_WORKTREE |
		    (verbose ? REFRESH_లోపల_PORCELAIN : REFRESH_QUIET);

	చూసిన = xcalloc(pathspec->nr, 1);
	refresh_సూచీ(repo->సూచీ, flags, pathspec, చూసిన,
		      _("Unstaged changes after refreshing the index:"));
	కొరకు (i = 0; i < pathspec->nr; i++) {
		ఒకవేళ (!చూసిన[i]) {
			స్థిరరాశి అక్షరం *మార్గం = pathspec->వస్తువులు[i].original;

			ఒకవేళ (matches_skip_worktree(pathspec, i, &skip_worktree_చూసిన) ||
			    !మార్గం_లోపల_sparse_checkout(మార్గం, repo->సూచీ)) {
				string_list_append(&only_సరిపోలిక_skip_worktree,
						   pathspec->వస్తువులు[i].original);
			} లేకపోతే {
				die(_("pathspec '%s' did not match any files"),
				    pathspec->వస్తువులు[i].original);
			}
		}
	}

	ఒకవేళ (only_సరిపోలిక_skip_worktree.nr) {
		advise_on_updating_sparse_paths(&only_సరిపోలిక_skip_worktree);
		ret = 1;
	}

	విడుదల(చూసిన);
	విడుదల(skip_worktree_చూసిన);
	string_list_clear(&only_సరిపోలిక_skip_worktree, 0);
	తిరిగి ret;
}

పూర్ణాంకం interactive_కూడు(నిర్మాణం repository *repo,
		    స్థిరరాశి అక్షరం **argv,
		    స్థిరరాశి అక్షరం *ఉపసర్గ,
		    పూర్ణాంకం patch, నిర్మాణం interactive_options *interactive_opts)
{
	నిర్మాణం pathspec pathspec;
	పూర్ణాంకం ret;

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_నిండిన |
		       PATHSPEC_SYMLINK_LEADING_మార్గం |
		       PATHSPEC_ఉపసర్గ_ORIGIN,
		       ఉపసర్గ, argv);

	ఒకవేళ (patch)
		ret = !!నిరంతరం_కూడు_p(repo, కూడు_P_కూడు, interactive_opts, ఖాళీ, &pathspec, 0);
	లేకపోతే
		ret = !!నిరంతరం_కూడు_i(repo, &pathspec, interactive_opts);

	clear_pathspec(&pathspec);
	తిరిగి ret;
}

స్థిర పూర్ణాంకం edit_patch(నిర్మాణం repository *repo,
		      పూర్ణాంకం argc,
		      స్థిరరాశి అక్షరం **argv,
		      స్థిరరాశి అక్షరం *ఉపసర్గ)
{
	అక్షరం *file = repo_git_మార్గం(repo, "ADD_EDIT.patch");
	నిర్మాణం apply_state state;
	స్థిరరాశి అక్షరం *apply_argv[2];
	నిర్మాణం rev_info rev;
	పూర్ణాంకం బహిర్గతం;
	నిర్మాణం stat st;

	repo_config(repo, git_diff_basic_config, ఖాళీ);

	ఒకవేళ (repo_read_సూచీ(repo) < 0)
		die(_("could not read the index"));

	repo_init_revisions(repo, &rev, ఉపసర్గ);
	rev.diffopt.context = 7;

	argc = అమరిక_revisions(argc, argv, &rev, ఖాళీ);
	rev.diffopt.output_format = DIFF_FORMAT_PATCH;
	rev.diffopt.use_color = GIT_COLOR_NEVER;
	rev.diffopt.flags.ignore_dirty_submodules = 1;
	బహిర్గతం = xopen(file, O_CREAT | O_WRONLY | O_TRUNC, 0666);
	rev.diffopt.file = xfdopen(బహిర్గతం, "w");
	rev.diffopt.close_file = 1;
	నిరంతరం_diff_files(&rev, 0);

	ఒకవేళ (launch_editor(file, ఖాళీ, ఖాళీ))
		die(_("editing patch failed"));

	ఒకవేళ (stat(file, &st))
		die_errno(_("could not stat '%s'"), file);
	ఒకవేళ (!st.st_కొలత)
		die(_("empty patch. aborted"));

	apply_argv[0] = file;
	apply_argv[1] = ఖాళీ;
	ఒకవేళ (init_apply_state(&state, repo, ఖాళీ))
		die(_("could not initialize apply state"));
	state.cached = 1;
	ఒకవేళ (check_apply_state(&state, 0))
		die(_("could not check apply state"));
	ఒకవేళ (apply_all_patches(&state, 1, apply_argv, APPLY_OPT_RECOUNT))
		die(_("could not apply '%s'"), file);
	clear_apply_state(&state);

	unlink(file);
	విడుదల(file);
	release_revisions(&rev);
	తిరిగి 0;
}

స్థిర స్థిరరాశి అక్షరం ignore_error[] =
N_("The following paths are ignored by one of your .gitignore files:\n");

స్థిర పూర్ణాంకం verbose, show_only, ignored_too, refresh_only;
స్థిర పూర్ణాంకం ignore_కూడు_errors, intent_గమ్యం_కూడు, ignore_missing;
స్థిర పూర్ణాంకం warn_on_embedded_repo = 1;

#నిర్వచించు ADDREMOVE_DEFAULT 1
స్థిర పూర్ణాంకం addremove = ADDREMOVE_DEFAULT;
స్థిర పూర్ణాంకం addremove_explicit = -1; /* unspecified */

స్థిర అక్షరం *chmod_arg;

స్థిర పూర్ణాంకం ignore_removal_cb(స్థిరరాశి నిర్మాణం option *opt, స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	BUG_ON_OPT_ARG(arg);

	/* if we are told to ignore, we are not adding removals */
	*(పూర్ణాంకం *)opt->విలువ = !unset ? 0 : 1;
	తిరిగి 0;
}

స్థిర నిర్మాణం option builtin_కూడు_options[] = {
	OPT_DRY_నిరంతరం(&show_only, N_("dry run")),
	OPT__VERBOSE(&verbose, N_("be verbose")),
	OPT_సమూహం(""),
	OPT_BOOL('i', "interactive", &కూడు_interactive, N_("interactive picking")),
	OPT_BOOL('p', "patch", &patch_interactive, N_("select hunks interactively")),
	OPT_BOOL(0, "auto-advance", &interactive_opts.auto_advance,
		 N_("auto advance to the next file when selecting hunks interactively")),
	OPT_DIFF_UNIFIED(&interactive_opts.context),
	OPT_DIFF_INTERHUNK_CONTEXT(&interactive_opts.interhunkcontext),
	OPT_BOOL('e', "edit", &edit_interactive, N_("edit current diff and apply")),
	OPT__FORCE(&ignored_too, N_("allow adding otherwise ignored files"), 0),
	OPT_BOOL('u', "update", &take_worktree_changes, N_("update tracked files")),
	OPT_BOOL(0, "renormalize", &కూడు_renormalize, N_("renormalize EOL of tracked files (implies -u)")),
	OPT_BOOL(0, "resolved", &కూడు_resolved, N_("add conflict-resolved tracked files")),
	OPT_BOOL('N', "intent-to-add", &intent_గమ్యం_కూడు, N_("record only the fact that the path will be added later")),
	OPT_BOOL('A', "all", &addremove_explicit, N_("add changes from all tracked and untracked files")),
	OPT_CALLBACK_F(0, "ignore-removal", &addremove_explicit,
	  ఖాళీ /* takes no arguments */,
	  N_("ignore paths removed in the working tree (same as --no-all)"),
	  PARSE_OPT_NOARG, ignore_removal_cb),
	OPT_BOOL( 0 , "refresh", &refresh_only, N_("don't add, only refresh the index")),
	OPT_BOOL( 0 , "ignore-errors", &ignore_కూడు_errors, N_("just skip files which cannot be added because of errors")),
	OPT_BOOL( 0 , "ignore-missing", &ignore_missing, N_("check if - even missing - files are ignored in dry run")),
	OPT_BOOL(0, "sparse", &include_sparse, N_("allow updating entries outside of the sparse-checkout cone")),
	OPT_STRING(0, "chmod", &chmod_arg, "(+|-)x",
		   N_("override the executable bit of the listed files")),
	OPT_HIDDEN_BOOL(0, "warn-embedded-repo", &warn_on_embedded_repo,
			N_("warn when adding an embedded repository")),
	OPT_PATHSPEC_FROM_FILE(&pathspec_from_file),
	OPT_PATHSPEC_FILE_NUL(&pathspec_file_nul),
	OPT_చివర(),
};

స్థిర పూర్ణాంకం కూడు_config(స్థిరరాశి అక్షరం *var, స్థిరరాశి అక్షరం *విలువ,
		      స్థిరరాశి నిర్మాణం config_context *ctx, శూన్య *cb)
{
	ఒకవేళ (!strcmp(var, "add.ignoreerrors") ||
	    !strcmp(var, "add.ignore-errors")) {
		ignore_కూడు_errors = git_config_bool(var, విలువ);
		తిరిగి 0;
	}

	ఒకవేళ (git_color_config(var, విలువ, cb) < 0)
		తిరిగి -1;

	తిరిగి git_default_config(var, విలువ, ctx, cb);
}

స్థిర స్థిరరాశి అక్షరం embedded_advice[] = N_(
"You've added another git repository inside your current repository.\n"
"Clones of the outer repository will not contain the contents of\n"
"the embedded repository and will not know how to obtain it.\n"
"If you meant to add a submodule, use:\n"
"\n"
"	git submodule add <url> %s\n"
"\n"
"If you added this path by mistake, you can remove it from the\n"
"index with:\n"
"\n"
"	git rm --cached %s\n"
"\n"
"See \"git help submodule\" for more information."
);

స్థిర శూన్య check_embedded_repo(స్థిరరాశి అక్షరం *మార్గం)
{
	నిర్మాణం strbuf name = STRBUF_INIT;
	స్థిర పూర్ణాంకం adviced_on_embedded_repo = 0;

	ఒకవేళ (!warn_on_embedded_repo)
		తిరిగి;
	ఒకవేళ (!చివర్లు_with(మార్గం, "/"))
		తిరిగి;

	/* Drop trailing slash for aesthetics */
	strbuf_addstr(&name, మార్గం);
	strbuf_strip_suffix(&name, "/");

	warning(_("adding embedded git repository: %s"), name.buf);
	ఒకవేళ (!adviced_on_embedded_repo) {
		advise_if_enabled(ADVICE_కూడు_EMBEDDED_REPO,
				  embedded_advice, name.buf, name.buf);
		adviced_on_embedded_repo = 1;
	}

	strbuf_release(&name);
}

స్థిర పూర్ణాంకం కూడు_files(నిర్మాణం repository *repo, నిర్మాణం dir_struct *dir, పూర్ణాంకం flags)
{
	పూర్ణాంకం i, exit_స్థితి = 0;
	నిర్మాణం string_list matched_sparse_paths = STRING_LIST_INIT_NODUP;

	ఒకవేళ (dir->ignored_nr) {
		fprintf(stderr, _(ignore_error));
		కొరకు (i = 0; i < dir->ignored_nr; i++)
			fprintf(stderr, "%s\n", dir->ignored[i]->name);
		advise_if_enabled(ADVICE_కూడు_IGNORED_FILE,
				  _("Use -f if you really want to add them."));
		exit_స్థితి = 1;
	}

	కొరకు (i = 0; i < dir->nr; i++) {
		ఒకవేళ (!include_sparse &&
		    !మార్గం_లోపల_sparse_checkout(dir->entries[i]->name, repo->సూచీ)) {
			string_list_append(&matched_sparse_paths,
					   dir->entries[i]->name);
			కొనసాగు;
		}
		ఒకవేళ (కూడు_file_గమ్యం_సూచీ(repo->సూచీ, dir->entries[i]->name, flags)) {
			ఒకవేళ (!ignore_కూడు_errors)
				die(_("adding files failed"));
			exit_స్థితి = 1;
		} లేకపోతే {
			check_embedded_repo(dir->entries[i]->name);
		}
	}

	ఒకవేళ (matched_sparse_paths.nr) {
		advise_on_updating_sparse_paths(&matched_sparse_paths);
		exit_స్థితి = 1;
	}

	string_list_clear(&matched_sparse_paths, 0);

	తిరిగి exit_స్థితి;
}

స్థిర పూర్ణాంకం failed_గమ్యం_కూడు(పూర్ణాంకం flags, స్థిరరాశి అక్షరం *మార్గం)
{
	ఒకవేళ (!(flags & కూడు_CACHE_IGNORE_ERRORS))
		die(_("updating file '%s' failed"), మార్గం);
	తిరిగి 1;
}

స్థిర పూర్ణాంకం కూడు_resolved_files(నిర్మాణం repository *repo,
			      స్థిరరాశి నిర్మాణం pathspec *pathspec,
			      పూర్ణాంకం flags)
{
	నిర్మాణం సూచీ_state *istate = repo->సూచీ;
	నిర్మాణం string_list unmerged_paths = STRING_LIST_INIT_DUP;
	నిర్మాణం string_list unresolved_paths = STRING_LIST_INIT_DUP;
	పూర్ణాంకం exit_స్థితి = 0;
	కొలత_t i;

	కొరకు (i = 0; i < istate->cache_nr; i++) {
		నిర్మాణం cache_నమోదు *ce = istate->cache[i];
		ఒకవేళ (!ce_stage(ce))
			కొనసాగు;
		ఒకవేళ (pathspec->nr && !ce_మార్గం_సరిపోలిక(istate, ce, pathspec, ఖాళీ))
			కొనసాగు;
		ఒకవేళ (!unmerged_paths.nr ||
		    strcmp(unmerged_paths.వస్తువులు[unmerged_paths.nr - 1].string, ce->name))
			string_list_append(&unmerged_paths, ce->name);
	}

	ఒకవేళ (!unmerged_paths.nr) {
		string_list_clear(&unmerged_paths, 0);
		తిరిగి 0;
	}

	కొరకు (i = 0; i < unmerged_paths.nr; i++) {
		స్థిరరాశి అక్షరం *మార్గం = unmerged_paths.వస్తువులు[i].string;
		నిర్మాణం stat st;

		ఒకవేళ (!lstat(మార్గం, &st) && S_ISREG(st.st_బహుళకం)) {
			ఒకవేళ (has_conflict_markers(istate, మార్గం))
				string_list_append(&unresolved_paths, మార్గం);
		}
	}

	ఒకవేళ (unresolved_paths.nr) {
		నిర్మాణం strbuf sb = STRBUF_INIT;
		కొరకు (i = 0; i < unresolved_paths.nr; i++)
			strbuf_addf(&sb, "\t%s\n", unresolved_paths.వస్తువులు[i].string);
		die(_("the following paths still have conflict markers:\n%s"), sb.buf);
	}

	కొరకు (i = 0; i < unmerged_paths.nr; i++) {
		స్థిరరాశి అక్షరం *మార్గం = unmerged_paths.వస్తువులు[i].string;
		నిర్మాణం stat st;

		ఒకవేళ (lstat(మార్గం, &st)) {
			ఒకవేళ (errno != ENOENT)
				die_errno(_("cannot lstat: '%s'"), మార్గం);
			ఒకవేళ (remove_file_from_సూచీ_with_flags(istate, మార్గం, flags))
				exit_స్థితి = failed_గమ్యం_కూడు(flags, మార్గం);
		} లేకపోతే {
			ఒకవేళ (కూడు_file_గమ్యం_సూచీ(istate, మార్గం, flags))
				exit_స్థితి = failed_గమ్యం_కూడు(flags, మార్గం);
		}
	}

	string_list_clear(&unmerged_paths, 0);
	string_list_clear(&unresolved_paths, 0);
	తిరిగి exit_స్థితి;
}

పూర్ణాంకం cmd_కూడు(పూర్ణాంకం argc,
	    స్థిరరాశి అక్షరం **argv,
	    స్థిరరాశి అక్షరం *ఉపసర్గ,
	    నిర్మాణం repository *repo)
{
	పూర్ణాంకం exit_స్థితి = 0;
	నిర్మాణం pathspec pathspec;
	నిర్మాణం dir_struct dir = DIR_INIT;
	పూర్ణాంకం flags;
	పూర్ణాంకం కూడు_new_files;
	పూర్ణాంకం require_pathspec;
	అక్షరం *చూసిన = ఖాళీ;
	అక్షరం *ps_matched = ఖాళీ;
	నిర్మాణం తాళం_file తాళం_file = తాళం_INIT;
	నిర్మాణం odb_transaction *transaction = ఖాళీ;

	repo_config(repo, కూడు_config, ఖాళీ);

	argc = parse_options(argc, argv, ఉపసర్గ, builtin_కూడు_options,
			  builtin_కూడు_usage, PARSE_OPT_KEEP_ARGV0);

	prepare_repo_settings(repo);
	repo->settings.command_requires_నిండిన_సూచీ = 0;

	ఒకవేళ (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	ఒకవేళ (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	ఒకవేళ (patch_interactive)
		కూడు_interactive = 1;
	ఒకవేళ (కూడు_interactive) {
		ఒకవేళ (show_only)
			die(_("options '%s' and '%s' cannot be used together"), "--dry-run", "--interactive/--patch");
		ఒకవేళ (pathspec_from_file)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--interactive/--patch");
		నిష్క్రమించు(interactive_కూడు(repo, argv + 1, ఉపసర్గ, patch_interactive, &interactive_opts));
	} లేకపోతే {
		ఒకవేళ (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--interactive/--patch");
		ఒకవేళ (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--interactive/--patch");
		ఒకవేళ (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--interactive/--patch");
	}

	ఒకవేళ (edit_interactive) {
		ఒకవేళ (pathspec_from_file)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--edit");
		తిరిగి(edit_patch(repo, argc, argv, ఉపసర్గ));
	}
	argc--;
	argv++;

	ఒకవేళ (0 <= addremove_explicit)
		addremove = addremove_explicit;
	లేకపోతే ఒకవేళ (take_worktree_changes && ADDREMOVE_DEFAULT)
		addremove = 0; /* "-u" was given but not "-A" */

	die_for_incompatible_opt3(take_worktree_changes, "-u/--update",
				  0 < addremove_explicit, "-A/--all",
				  కూడు_resolved, "--resolved");

	ఒకవేళ (!show_only && ignore_missing)
		die(_("the option '%s' requires '%s'"), "--ignore-missing", "--dry-run");

	ఒకవేళ (chmod_arg && ((chmod_arg[0] != '-' && chmod_arg[0] != '+') ||
			  chmod_arg[1] != 'x' || chmod_arg[2]))
		die(_("--chmod param '%s' must be either -x or +x"), chmod_arg);

	కూడు_new_files = !take_worktree_changes && !refresh_only &&
			!కూడు_renormalize && !కూడు_resolved;
	require_pathspec = !(take_worktree_changes ||
			     (0 < addremove_explicit) ||
			     కూడు_resolved);

	repo_hold_locked_సూచీ(repo, &తాళం_file, తాళం_DIE_ON_ERROR);

	/*
	 * Check the "pathspec '%s' did not match any files" block
	 * below before enabling new magic.
	 */
	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_నిండిన |
		       PATHSPEC_SYMLINK_LEADING_మార్గం,
		       ఉపసర్గ, argv);

	ఒకవేళ (pathspec_from_file) {
		ఒకవేళ (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_నిండిన |
				    PATHSPEC_SYMLINK_LEADING_మార్గం,
				    ఉపసర్గ, pathspec_from_file, pathspec_file_nul);
	} లేకపోతే ఒకవేళ (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	ఒకవేళ (require_pathspec && pathspec.nr == 0) {
		fprintf(stderr, _("Nothing specified, nothing added.\n"));
		advise_if_enabled(ADVICE_కూడు_ఖాళీలు_PATHSPEC,
				  _("Maybe you wanted to say 'git add .'?"));
		తిరిగి 0;
	}

	ఒకవేళ (!take_worktree_changes && !కూడు_resolved &&
	    addremove_explicit < 0 && pathspec.nr)
		/* Turn "git add pathspec..." to "git add -A pathspec..." */
		addremove = 1;

	flags = ((verbose ? కూడు_CACHE_VERBOSE : 0) |
		 (show_only ? కూడు_CACHE_PRETEND : 0) |
		 (intent_గమ్యం_కూడు ? కూడు_CACHE_INTENT : 0) |
		 (ignore_కూడు_errors ? కూడు_CACHE_IGNORE_ERRORS : 0) |
		 (!(addremove || take_worktree_changes)
		  ? కూడు_CACHE_IGNORE_REMOVAL : 0));

	ఒకవేళ (repo_read_సూచీ_preload(repo, &pathspec, 0) < 0)
		die(_("index file corrupt"));

	die_లోపల_unpopulated_submodule(repo->సూచీ, ఉపసర్గ);
	die_మార్గం_inside_submodule(repo->సూచీ, &pathspec);

	ఒకవేళ (కూడు_new_files) {
		పూర్ణాంకం baselen;

		/* Set up the default git porcelain excludes */
		ఒకవేళ (!ignored_too) {
			dir.flags |= DIR_COLLECT_IGNORED;
			అమరిక_standard_excludes(&dir);
		}

		/* This picks up the paths that are not tracked */
		baselen = fill_directory(&dir, repo->సూచీ, &pathspec);
		ఒకవేళ (pathspec.nr)
			చూసిన = prune_directory(repo, &dir, &pathspec, baselen);
	}

	ఒకవేళ (refresh_only) {
		exit_స్థితి |= refresh(repo, verbose, &pathspec);
		వెళ్ళు ముగింపు;
	}

	ఒకవేళ (pathspec.nr) {
		పూర్ణాంకం i;
		అక్షరం *skip_worktree_చూసిన = ఖాళీ;
		నిర్మాణం string_list only_సరిపోలిక_skip_worktree = STRING_LIST_INIT_NODUP;

		ఒకవేళ (!చూసిన)
			చూసిన = కనుగొను_pathspecs_matching_against_సూచీ(&pathspec,
					repo->సూచీ, PS_IGNORE_SKIP_WORKTREE);

		/*
		 * file_exists() assumes exact match
		 */
		GUARD_PATHSPEC(&pathspec,
			       PATHSPEC_FROMTOP |
			       PATHSPEC_LITERAL |
			       PATHSPEC_GLOB |
			       PATHSPEC_ICASE |
			       PATHSPEC_EXCLUDE |
			       PATHSPEC_ATTR);

		కొరకు (i = 0; i < pathspec.nr; i++) {
			స్థిరరాశి అక్షరం *మార్గం = pathspec.వస్తువులు[i].సరిపోలిక;

			ఒకవేళ (pathspec.వస్తువులు[i].magic & PATHSPEC_EXCLUDE)
				కొనసాగు;
			ఒకవేళ (చూసిన[i])
				కొనసాగు;

			ఒకవేళ (!include_sparse &&
			    matches_skip_worktree(&pathspec, i, &skip_worktree_చూసిన)) {
				string_list_append(&only_సరిపోలిక_skip_worktree,
						   pathspec.వస్తువులు[i].original);
				కొనసాగు;
			}

			/* Don't complain at 'git add .' on empty repo */
			ఒకవేళ (!మార్గం[0])
				కొనసాగు;

			ఒకవేళ ((pathspec.వస్తువులు[i].magic & (PATHSPEC_GLOB | PATHSPEC_ICASE)) ||
			    !file_exists(మార్గం)) {
				ఒకవేళ (ignore_missing) {
					పూర్ణాంకం dtype = DT_UNKNOWN;
					ఒకవేళ (is_excluded(&dir, repo->సూచీ, మార్గం, &dtype))
						dir_కూడు_ignored(&dir, repo->సూచీ,
								మార్గం, pathspec.వస్తువులు[i].len);
				} లేకపోతే
					die(_("pathspec '%s' did not match any files"),
					    pathspec.వస్తువులు[i].original);
			}
		}


		ఒకవేళ (only_సరిపోలిక_skip_worktree.nr) {
			advise_on_updating_sparse_paths(&only_సరిపోలిక_skip_worktree);
			exit_స్థితి = 1;
		}

		విడుదల(చూసిన);
		విడుదల(skip_worktree_చూసిన);
		string_list_clear(&only_సరిపోలిక_skip_worktree, 0);
	}

	odb_transaction_begin_or_die(repo->objects, &transaction, 0);

	ps_matched = xcalloc(pathspec.nr, 1);
	ఒకవేళ (కూడు_resolved)
		exit_స్థితి |= కూడు_resolved_files(repo, &pathspec, flags);
	లేకపోతే ఒకవేళ (కూడు_renormalize)
		exit_స్థితి |= renormalize_tracked_files(repo, &pathspec, flags);
	లేకపోతే
		exit_స్థితి |= కూడు_files_గమ్యం_cache(repo, ఉపసర్గ,
						  &pathspec, ps_matched,
						  include_sparse, flags, ignored_too);

	ఒకవేళ (take_worktree_changes && !కూడు_renormalize && !ignore_కూడు_errors &&
	    report_మార్గం_error(ps_matched, &pathspec))
		నిష్క్రమించు(128);

	ఒకవేళ (కూడు_new_files)
		exit_స్థితి |= కూడు_files(repo, &dir, flags);

	ఒకవేళ (chmod_arg && pathspec.nr)
		exit_స్థితి |= chmod_pathspec(repo, &pathspec, chmod_arg[0], show_only);
	odb_transaction_commit_and_finalize_or_die(transaction);

ముగింపు:
	ఒకవేళ (write_locked_సూచీ(repo->సూచీ, &తాళం_file,
			       COMMIT_తాళం | SKIP_IF_UNCHANGED))
		die(_("unable to write new index file"));

	విడుదల(ps_matched);
	dir_clear(&dir);
	clear_pathspec(&pathspec);
	తిరిగి exit_స్థితి;
}
