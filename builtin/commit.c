/*
 * Builtin "git commit"
 *
 * Copyright (c) 2007 Kristian Høgsberg <krh@redhat.com>
 * Based on git-commit.sh by Junio C Hamano and Linus Torvalds
 */

#నిర్వచించు USE_THE_REPOSITORY_VARIABLE
#నిర్వచించు DISABLE_SIGN_COMPARE_WARNINGS

#అన్తర్భావ "builtin.h"
#అన్తర్భావ "advice.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "lockfile.h"
#అన్తర్భావ "cache-tree.h"
#అన్తర్భావ "color.h"
#అన్తర్భావ "dir.h"
#అన్తర్భావ "editor.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "diff.h"
#అన్తర్భావ "commit.h"
#అన్తర్భావ "add-interactive.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "revision.h"
#అన్తర్భావ "wt-status.h"
#అన్తర్భావ "run-command.h"
#అన్తర్భావ "strbuf.h"
#అన్తర్భావ "object-name.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "path.h"
#అన్తర్భావ "preload-index.h"
#అన్తర్భావ "read-cache.h"
#అన్తర్భావ "repository.h"
#అన్తర్భావ "string-list.h"
#అన్తర్భావ "rerere.h"
#అన్తర్భావ "unpack-trees.h"
#అన్తర్భావ "column.h"
#అన్తర్భావ "sequencer.h"
#అన్తర్భావ "sparse-index.h"
#అన్తర్భావ "mailmap.h"
#అన్తర్భావ "help.h"
#అన్తర్భావ "commit-reach.h"
#అన్తర్భావ "commit-graph.h"
#అన్తర్భావ "pretty.h"
#అన్తర్భావ "trailer.h"

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి builtin_commit_usage[] = {
	N_("git commit [-a | --interactive | --patch] [-s] [-v] [-u[<mode>]] [--amend]\n"
	   "           [--dry-run] [(-c | -C | --squash) <commit> | --fixup [(amend|reword):]<commit>]\n"
	   "           [-F <file> | -m <msg>] [--reset-author] [--allow-empty]\n"
	   "           [--allow-empty-message] [--no-verify] [-e] [--author=<author>]\n"
	   "           [--date=<date>] [--cleanup=<mode>] [--[no-]status]\n"
	   "           [-i | -o] [--pathspec-from-file=<file> [--pathspec-file-nul]]\n"
	   "           [(--trailer <token>[(=|:)<value>])...] [-S[<keyid>]]\n"
	   "           [--] [<pathspec>...]"),
	ఖాళీ
};

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి builtin_స్థితి_usage[] = {
	N_("git status [<options>] [--] [<pathspec>...]"),
	ఖాళీ
};

స్థిర స్థిరరాశి అక్షరం ఖాళీలు_amend_advice[] =
N_("You asked to amend the most recent commit, but doing so would make\n"
"it empty. You can repeat your command with --allow-empty, or you can\n"
"remove the commit entirely with \"git reset HEAD^\".\n");

స్థిర స్థిరరాశి అక్షరం ఖాళీలు_cherry_pick_advice[] =
N_("The previous cherry-pick is now empty, possibly due to conflict resolution.\n"
"If you wish to commit it anyway, use:\n"
"\n"
"    git commit --allow-empty\n"
"\n");

స్థిర స్థిరరాశి అక్షరం ఖాళీలు_rebase_pick_advice[] =
N_("Otherwise, please use 'git rebase --skip'\n");

స్థిర స్థిరరాశి అక్షరం ఖాళీలు_cherry_pick_advice_single[] =
N_("Otherwise, please use 'git cherry-pick --skip'\n");

స్థిర స్థిరరాశి అక్షరం ఖాళీలు_cherry_pick_advice_multi[] =
N_("and then use:\n"
"\n"
"    git cherry-pick --continue\n"
"\n"
"to resume cherry-picking the remaining commits.\n"
"If you wish to skip this commit, use:\n"
"\n"
"    git cherry-pick --skip\n"
"\n");

స్థిర స్థిరరాశి అక్షరం *color_స్థితి_slots[] = {
	[WT_స్థితి_శీర్షిక]	  = "header",
	[WT_స్థితి_UPDATED]	  = "updated",
	[WT_స్థితి_మారింది]	  = "changed",
	[WT_స్థితి_UNTRACKED]	  = "untracked",
	[WT_స్థితి_NOBRANCH]	  = "noBranch",
	[WT_స్థితి_UNMERGED]	  = "unmerged",
	[WT_స్థితి_LOCAL_BRANCH]  = "localBranch",
	[WT_స్థితి_REMOTE_BRANCH] = "remoteBranch",
	[WT_స్థితి_ONBRANCH]	  = "branch",
};

స్థిర స్థిరరాశి అక్షరం *use_సందేశం_బఫరు;
స్థిర నిర్మాణం తాళం_file సూచీ_తాళం; /* real index */
స్థిర నిర్మాణం తాళం_file false_తాళం; /* used only for partial commits */
స్థిర గణన {
	COMMIT_AS_IS = 1,
	COMMIT_సాధారణ,
	COMMIT_PARTIAL
} commit_style;

స్థిర స్థిరరాశి అక్షరం *force_author;
స్థిర అక్షరం *logfile;
స్థిర అక్షరం *template_file;
/*
 * The _message variables are commit names from which to take
 * the commit message and/or authorship.
 */
స్థిర స్థిరరాశి అక్షరం *author_సందేశం, *author_సందేశం_బఫరు;
స్థిర స్థిరరాశి అక్షరం *edit_సందేశం, *use_సందేశం;
స్థిర అక్షరం *fixup_సందేశం, *fixup_commit, *squash_సందేశం;
స్థిర స్థిరరాశి అక్షరం *fixup_ఉపసర్గ;
స్థిర పూర్ణాంకం all, also, interactive, patch_interactive, only, amend, signoff;
స్థిర నిర్మాణం interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
స్థిర పూర్ణాంకం edit_flag = -1; /* unspecified */
స్థిర పూర్ణాంకం quiet, verbose, no_verify, allow_ఖాళీలు, dry_నిరంతరం, renew_authorship;
స్థిర పూర్ణాంకం config_commit_verbose = -1; /* unspecified */
స్థిర పూర్ణాంకం no_post_rewrite, allow_ఖాళీలు_సందేశం, pathspec_file_nul;
స్థిర స్థిరరాశి అక్షరం *untracked_files_arg, *force_date, *ignore_submodule_arg, *ignored_arg;
స్థిర స్థిరరాశి అక్షరం *sign_commit, *pathspec_from_file;
స్థిర నిర్మాణం strvec trailer_వాదనలు = STRVEC_INIT;

/*
 * The default commit message cleanup mode will remove the lines
 * beginning with # (shell comments) and leading and trailing
 * whitespaces (empty lines or containing only whitespaces)
 * if editor is used, and only the whitespaces if the message
 * is specified explicitly.
 */
స్థిర గణన commit_సందేశం_cleanup_బహుళకం cleanup_బహుళకం;
స్థిర అక్షరం *cleanup_config;

స్థిర గణన commit_whence whence;
స్థిర పూర్ణాంకం use_editor = 1, include_స్థితి = 1;
స్థిర పూర్ణాంకం have_option_m;
స్థిర నిర్మాణం strbuf సందేశం_2 = STRBUF_INIT;

స్థిర గణన wt_స్థితి_format స్థితి_format = స్థితి_FORMAT_UNSPECIFIED;

స్థిర పూర్ణాంకం opt_parse_porcelain(స్థిరరాశి నిర్మాణం option *opt, స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	గణన wt_స్థితి_format *విలువ = (గణన wt_స్థితి_format *)opt->విలువ;
	ఒకవేళ (unset)
		*విలువ = స్థితి_FORMAT_NONE;
	లేకపోతే ఒకవేళ (!arg)
		*విలువ = స్థితి_FORMAT_PORCELAIN;
	లేకపోతే ఒకవేళ (!strcmp(arg, "v1") || !strcmp(arg, "1"))
		*విలువ = స్థితి_FORMAT_PORCELAIN;
	లేకపోతే ఒకవేళ (!strcmp(arg, "v2") || !strcmp(arg, "2"))
		*విలువ = స్థితి_FORMAT_PORCELAIN_V2;
	లేకపోతే
		die("unsupported porcelain version '%s'", arg);

	తిరిగి 0;
}

స్థిర పూర్ణాంకం opt_parse_m(స్థిరరాశి నిర్మాణం option *opt, స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	నిర్మాణం strbuf *buf = opt->విలువ;
	ఒకవేళ (unset) {
		have_option_m = 0;
		strbuf_setlen(buf, 0);
	} లేకపోతే {
		have_option_m = 1;
		ఒకవేళ (buf->len)
			strbuf_addch(buf, '\n');
		strbuf_addstr(buf, arg);
		strbuf_complete_పంక్తి(buf);
	}
	తిరిగి 0;
}

స్థిర పూర్ణాంకం opt_parse_rename_score(స్థిరరాశి నిర్మాణం option *opt, స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	స్థిరరాశి అక్షరం **విలువ = opt->విలువ;

	BUG_ON_OPT_NEG(unset);

	ఒకవేళ (arg != ఖాళీ && *arg == '=')
		arg = arg + 1;

	*విలువ = arg;
	తిరిగి 0;
}

స్థిర శూన్య determine_whence(నిర్మాణం wt_స్థితి *s)
{
	ఒకవేళ (file_exists(git_మార్గం_విలీనం_తల(the_repository)))
		whence = FROM_విలీనం;
	లేకపోతే ఒకవేళ (!sequencer_determine_whence(the_repository, &whence))
		whence = FROM_COMMIT;
	ఒకవేళ (s)
		s->whence = whence;
}

స్థిర శూన్య స్థితి_init_config(నిర్మాణం wt_స్థితి *s, config_fn_t fn)
{
	wt_స్థితి_prepare(the_repository, s);
	init_diff_ui_defaults();
	repo_config(the_repository, fn, s);
	determine_whence(s);
	s->hints = advice_enabled(ADVICE_స్థితి_HINTS); /* must come after repo_config() */
}

స్థిర శూన్య rollback_సూచీ_files(శూన్య)
{
	ఎంపిక (commit_style) {
	స్థితి COMMIT_AS_IS:
		ఆపు; /* nothing to do */
	స్థితి COMMIT_సాధారణ:
		rollback_తాళం_file(&సూచీ_తాళం);
		ఆపు;
	స్థితి COMMIT_PARTIAL:
		rollback_తాళం_file(&సూచీ_తాళం);
		rollback_తాళం_file(&false_తాళం);
		ఆపు;
	}
}

స్థిర పూర్ణాంకం commit_సూచీ_files(శూన్య)
{
	పూర్ణాంకం err = 0;

	ఎంపిక (commit_style) {
	స్థితి COMMIT_AS_IS:
		ఆపు; /* nothing to do */
	స్థితి COMMIT_సాధారణ:
		err = commit_తాళం_file(&సూచీ_తాళం);
		ఆపు;
	స్థితి COMMIT_PARTIAL:
		err = commit_తాళం_file(&సూచీ_తాళం);
		rollback_తాళం_file(&false_తాళం);
		ఆపు;
	}

	తిరిగి err;
}

/*
 * Take a union of paths in the index and the named tree (typically, "HEAD"),
 * and return the paths that match the given pattern in list.
 */
స్థిర పూర్ణాంకం list_paths(నిర్మాణం string_list *list, స్థిరరాశి అక్షరం *with_వృక్షం,
		      స్థిరరాశి నిర్మాణం pathspec *నమూనా)
{
	పూర్ణాంకం i, ret;
	అక్షరం *m;

	ఒకవేళ (!నమూనా->nr)
		తిరిగి 0;

	m = xcalloc(1, నమూనా->nr);

	ఒకవేళ (with_వృక్షం) {
		అక్షరం *max_ఉపసర్గ = common_ఉపసర్గ(నమూనా);
		overlay_వృక్షం_on_సూచీ(the_repository->సూచీ, with_వృక్షం, max_ఉపసర్గ);
		విడుదల(max_ఉపసర్గ);
	}

	/* TODO: audit for interaction with sparse-index. */
	ensure_నిండిన_సూచీ(the_repository->సూచీ);
	కొరకు (i = 0; i < the_repository->సూచీ->cache_nr; i++) {
		స్థిరరాశి నిర్మాణం cache_నమోదు *ce = the_repository->సూచీ->cache[i];
		నిర్మాణం string_list_అంశం *అంశం;

		ఒకవేళ (ce->ce_flags & CE_UPDATE)
			కొనసాగు;
		ఒకవేళ (!ce_మార్గం_సరిపోలిక(the_repository->సూచీ, ce, నమూనా, m))
			కొనసాగు;
		అంశం = string_list_చొప్పించు(list, ce->name);
		ఒకవేళ (ce_skip_worktree(ce))
			అంశం->util = అంశం; /* better a valid pointer than a fake one */
	}

	ret = report_మార్గం_error(m, నమూనా);
	విడుదల(m);
	తిరిగి ret;
}

స్థిర శూన్య కూడు_remove_files(నిర్మాణం string_list *list)
{
	పూర్ణాంకం i;
	కొరకు (i = 0; i < list->nr; i++) {
		నిర్మాణం stat st;
		నిర్మాణం string_list_అంశం *p = &(list->వస్తువులు[i]);

		/* p->util is skip-worktree */
		ఒకవేళ (p->util)
			కొనసాగు;

		ఒకవేళ (!lstat(p->string, &st)) {
			ఒకవేళ (కూడు_గమ్యం_సూచీ(the_repository->సూచీ, p->string, &st, 0))
				die(_("updating files failed"));
		} లేకపోతే
			remove_file_from_సూచీ(the_repository->సూచీ, p->string);
	}
}

స్థిర శూన్య create_పునాది_సూచీ(స్థిరరాశి నిర్మాణం commit *current_తల)
{
	నిర్మాణం వృక్షం *వృక్షం;
	నిర్మాణం unpack_trees_options opts;
	నిర్మాణం వృక్షం_desc t;

	ఒకవేళ (!current_తల) {
		discard_సూచీ(the_repository->సూచీ);
		తిరిగి;
	}

	memset(&opts, 0, పరిమాణం(opts));
	opts.తల_idx = 1;
	opts.సూచీ_only = 1;
	opts.విలీనం = 1;
	opts.src_సూచీ = the_repository->సూచీ;
	opts.dst_సూచీ = the_repository->సూచీ;

	opts.fn = oneway_విలీనం;
	వృక్షం = repo_parse_వృక్షం_పరోక్ష(the_repository,
					&current_తల->object.oid);
	ఒకవేళ (!వృక్షం)
		die(_("failed to unpack HEAD tree object"));
	ఒకవేళ (repo_parse_వృక్షం(the_repository, వృక్షం) < 0)
		నిష్క్రమించు(128);
	init_వృక్షం_desc(&t, &వృక్షం->object.oid, వృక్షం->బఫరు, వృక్షం->కొలత);
	ఒకవేళ (unpack_trees(1, &t, &opts))
		నిష్క్రమించు(128); /* We've already reported the error, finish dying */
}

స్థిర శూన్య refresh_cache_or_die(పూర్ణాంకం refresh_flags)
{
	/*
	 * refresh_flags contains REFRESH_QUIET, so the only errors
	 * are for unmerged entries.
	 */
	ఒకవేళ (refresh_సూచీ(the_repository->సూచీ, refresh_flags | REFRESH_లోపల_PORCELAIN, ఖాళీ, ఖాళీ, ఖాళీ))
		die_resolve_conflict("commit");
}

స్థిర స్థిరరాశి అక్షరం *prepare_సూచీ(స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
				 స్థిరరాశి నిర్మాణం commit *current_తల, పూర్ణాంకం is_స్థితి)
{
	నిర్మాణం string_list partial = STRING_LIST_INIT_DUP;
	నిర్మాణం pathspec pathspec;
	పూర్ణాంకం refresh_flags = REFRESH_QUIET;
	స్థిరరాశి అక్షరం *ret;
	అక్షరం *మార్గం = ఖాళీ;

	ఒకవేళ (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	ఒకవేళ (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	ఒకవేళ (is_స్థితి)
		refresh_flags |= REFRESH_UNMERGED;
	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_నిండిన,
		       ఉపసర్గ, argv);

	ఒకవేళ (pathspec_from_file) {
		ఒకవేళ (interactive)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--interactive/--patch");

		ఒకవేళ (all)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "-a");

		ఒకవేళ (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_నిండిన,
				    ఉపసర్గ, pathspec_from_file, pathspec_file_nul);
	} లేకపోతే ఒకవేళ (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	ఒకవేళ (!pathspec.nr && (also || (only && !allow_ఖాళీలు &&
	    (!amend || (fixup_సందేశం && strcmp(fixup_ఉపసర్గ, "amend"))))))
		die(_("No paths with --include/--only does not make sense."));

	ఒకవేళ (repo_read_సూచీ_preload(the_repository, &pathspec, 0) < 0)
		die(_("index file corrupt"));

	ఒకవేళ (interactive) {
		అక్షరం *old_సూచీ_env = ఖాళీ, *old_repo_సూచీ_file;
		repo_hold_locked_సూచీ(the_repository, &సూచీ_తాళం,
				       తాళం_DIE_ON_ERROR);

		refresh_cache_or_die(refresh_flags);

		ఒకవేళ (write_locked_సూచీ(the_repository->సూచీ, &సూచీ_తాళం, 0))
			die(_("unable to create temporary index"));

		old_repo_సూచీ_file = the_repository->సూచీ_file;
		the_repository->సూచీ_file =
			(అక్షరం *)get_తాళం_file_మార్గం(&సూచీ_తాళం);
		old_సూచీ_env = xstrdup_or_null(getenv(సూచీ_ENVIRONMENT));
		setenv(సూచీ_ENVIRONMENT, the_repository->సూచీ_file, 1);

		ఒకవేళ (interactive_కూడు(the_repository, argv, ఉపసర్గ, patch_interactive, &interactive_opts) != 0)
			die(_("interactive add failed"));

		the_repository->సూచీ_file = old_repo_సూచీ_file;
		ఒకవేళ (old_సూచీ_env && *old_సూచీ_env)
			setenv(సూచీ_ENVIRONMENT, old_సూచీ_env, 1);
		లేకపోతే
			unsetenv(సూచీ_ENVIRONMENT);
		FREE_AND_NULL(old_సూచీ_env);

		discard_సూచీ(the_repository->సూచీ);
		read_సూచీ_from(the_repository->సూచీ, get_తాళం_file_మార్గం(&సూచీ_తాళం),
				repo_get_git_dir(the_repository));
		ఒకవేళ (cache_వృక్షం_update(the_repository->సూచీ, WRITE_వృక్షం_SILENT) == 0) {
			ఒకవేళ (reopen_తాళం_file(&సూచీ_తాళం) < 0)
				die(_("unable to write index file"));
			ఒకవేళ (write_locked_సూచీ(the_repository->సూచీ, &సూచీ_తాళం, 0))
				die(_("unable to update temporary index"));
		} లేకపోతే
			warning(_("Failed to update main cache tree"));

		commit_style = COMMIT_సాధారణ;
		ret = get_తాళం_file_మార్గం(&సూచీ_తాళం);
		వెళ్ళు బహిర్గతం;
	} లేకపోతే {
		ఒకవేళ (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--interactive/--patch");
		ఒకవేళ (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--interactive/--patch");
	}

	/*
	 * Non partial, non as-is commit.
	 *
	 * (1) get the real index;
	 * (2) update the_index as necessary;
	 * (3) write the_index out to the real index (still locked);
	 * (4) return the name of the locked index file.
	 *
	 * The caller should run hooks on the locked real index, and
	 * (A) if all goes well, commit the real index;
	 * (B) on failure, rollback the real index.
	 */
	ఒకవేళ (all || (also && pathspec.nr)) {
		అక్షరం *ps_matched = xcalloc(pathspec.nr, 1);
		repo_hold_locked_సూచీ(the_repository, &సూచీ_తాళం,
				       తాళం_DIE_ON_ERROR);
		కూడు_files_గమ్యం_cache(the_repository, also ? ఉపసర్గ : ఖాళీ,
				   &pathspec, ps_matched, 0, 0, 0 );
		ఒకవేళ (!all && report_మార్గం_error(ps_matched, &pathspec))
			నిష్క్రమించు(128);

		refresh_cache_or_die(refresh_flags);
		cache_వృక్షం_update(the_repository->సూచీ, WRITE_వృక్షం_SILENT);
		ఒకవేళ (write_locked_సూచీ(the_repository->సూచీ, &సూచీ_తాళం, 0))
			die(_("unable to write new index file"));
		commit_style = COMMIT_సాధారణ;
		ret = get_తాళం_file_మార్గం(&సూచీ_తాళం);
		విడుదల(ps_matched);
		వెళ్ళు బహిర్గతం;
	}

	/*
	 * As-is commit.
	 *
	 * (1) return the name of the real index file.
	 *
	 * The caller should run hooks on the real index,
	 * and create commit from the_index.
	 * We still need to refresh the index here.
	 */
	ఒకవేళ (!only && !pathspec.nr) {
		repo_hold_locked_సూచీ(the_repository, &సూచీ_తాళం,
				       తాళం_DIE_ON_ERROR);
		refresh_cache_or_die(refresh_flags);
		ఒకవేళ (the_repository->సూచీ->cache_మారింది
		    || !cache_వృక్షం_fully_valid(the_repository->సూచీ))
			cache_వృక్షం_update(the_repository->సూచీ, WRITE_వృక్షం_SILENT);
		ఒకవేళ (write_locked_సూచీ(the_repository->సూచీ, &సూచీ_తాళం,
				       COMMIT_తాళం | SKIP_IF_UNCHANGED))
			die(_("unable to write new index file"));
		commit_style = COMMIT_AS_IS;
		ret = repo_get_సూచీ_file(the_repository);
		వెళ్ళు బహిర్గతం;
	}

	/*
	 * A partial commit.
	 *
	 * (0) find the set of affected paths;
	 * (1) get lock on the real index file;
	 * (2) update the_index with the given paths;
	 * (3) write the_index out to the real index (still locked);
	 * (4) get lock on the false index file;
	 * (5) reset the_index from HEAD;
	 * (6) update the_index the same way as (2);
	 * (7) write the_index out to the false index file;
	 * (8) return the name of the false index file (still locked);
	 *
	 * The caller should run hooks on the locked false index, and
	 * create commit from it.  Then
	 * (A) if all goes well, commit the real index;
	 * (B) on failure, rollback the real index;
	 * In either case, rollback the false index.
	 */
	commit_style = COMMIT_PARTIAL;

	ఒకవేళ (whence != FROM_COMMIT) {
		ఒకవేళ (whence == FROM_విలీనం)
			die(_("cannot do a partial commit during a merge."));
		లేకపోతే ఒకవేళ (is_from_cherry_pick(whence))
			die(_("cannot do a partial commit during a cherry-pick."));
		లేకపోతే ఒకవేళ (is_from_rebase(whence))
			die(_("cannot do a partial commit during a rebase."));
	}

	ఒకవేళ (list_paths(&partial, !current_తల ? ఖాళీ : "HEAD", &pathspec))
		నిష్క్రమించు(1);

	discard_సూచీ(the_repository->సూచీ);
	ఒకవేళ (repo_read_సూచీ(the_repository) < 0)
		die(_("cannot read the index"));

	repo_hold_locked_సూచీ(the_repository, &సూచీ_తాళం, తాళం_DIE_ON_ERROR);
	కూడు_remove_files(&partial);
	refresh_సూచీ(the_repository->సూచీ, REFRESH_QUIET, ఖాళీ, ఖాళీ, ఖాళీ);
	cache_వృక్షం_update(the_repository->సూచీ, WRITE_వృక్షం_SILENT);
	ఒకవేళ (write_locked_సూచీ(the_repository->సూచీ, &సూచీ_తాళం, 0))
		die(_("unable to write new index file"));

	మార్గం = repo_git_మార్గం(the_repository, "next-index-%"PRIuMAX,
			     (uintmax_t) getpid());
	hold_తాళం_file_for_update(&false_తాళం, మార్గం,
				  తాళం_DIE_ON_ERROR);

	create_పునాది_సూచీ(current_తల);
	కూడు_remove_files(&partial);
	refresh_సూచీ(the_repository->సూచీ, REFRESH_QUIET, ఖాళీ, ఖాళీ, ఖాళీ);

	ఒకవేళ (write_locked_సూచీ(the_repository->సూచీ, &false_తాళం, 0))
		die(_("unable to write temporary index file"));

	discard_సూచీ(the_repository->సూచీ);
	ret = get_తాళం_file_మార్గం(&false_తాళం);
	read_సూచీ_from(the_repository->సూచీ, ret, repo_get_git_dir(the_repository));
బహిర్గతం:
	string_list_clear(&partial, 0);
	clear_pathspec(&pathspec);
	విడుదల(మార్గం);
	తిరిగి ret;
}

స్థిర పూర్ణాంకం నిరంతరం_స్థితి(FILE *fp, స్థిరరాశి అక్షరం *సూచీ_file, స్థిరరాశి అక్షరం *ఉపసర్గ, పూర్ణాంకం nowarn,
		      నిర్మాణం wt_స్థితి *s)
{
	నిర్మాణం object_id oid;

	ఒకవేళ (s->relative_paths)
		s->ఉపసర్గ = ఉపసర్గ;

	ఒకవేళ (amend) {
		s->amend = 1;
		s->reference = "HEAD^1";
	}
	s->verbose = verbose;
	s->సూచీ_file = సూచీ_file;
	s->fp = fp;
	s->nowarn = nowarn;
	s->is_initial = repo_get_oid(the_repository, s->reference, &oid) ? 1 : 0;
	ఒకవేళ (!s->is_initial)
		oidcpy(&s->oid_commit, &oid);
	s->స్థితి_format = స్థితి_format;
	s->ignore_submodule_arg = ignore_submodule_arg;

	wt_స్థితి_collect(s);
	wt_స్థితి_print(s);
	wt_స్థితి_collect_free_buffers(s);

	తిరిగి s->committable;
}

స్థిర పూర్ణాంకం is_a_విలీనం(స్థిరరాశి నిర్మాణం commit *current_తల)
{
	తిరిగి !!(current_తల->parents && current_తల->parents->తదుపరి);
}

స్థిర శూన్య assert_విభజించు_ident(నిర్మాణం ident_విభజించు *id, స్థిరరాశి నిర్మాణం strbuf *buf)
{
	ఒకవేళ (విభజించు_ident_పంక్తి(id, buf->buf, buf->len) || !id->date_begin)
		BUG("unable to parse our own ident: %s", buf->buf);
}

స్థిర శూన్య export_one(స్థిరరాశి అక్షరం *var, స్థిరరాశి అక్షరం *s, స్థిరరాశి అక్షరం *e, పూర్ణాంకం hack)
{
	నిర్మాణం strbuf buf = STRBUF_INIT;
	ఒకవేళ (hack)
		strbuf_addch(&buf, hack);
	strbuf_కూడు(&buf, s, e - s);
	setenv(var, buf.buf, 1);
	strbuf_release(&buf);
}

స్థిర పూర్ణాంకం parse_force_date(స్థిరరాశి అక్షరం *లోపల, నిర్మాణం strbuf *బహిర్గతం)
{
	strbuf_addch(బహిర్గతం, '@');

	ఒకవేళ (parse_date(లోపల, బహిర్గతం) < 0) {
		పూర్ణాంకం errors = 0;
		గుర్తులేని దీర్ఘ t = approxidate_careful(లోపల, &errors);
		ఒకవేళ (errors)
			తిరిగి -1;
		strbuf_addf(బహిర్గతం, "%lu", t);
	}

	తిరిగి 0;
}

స్థిర శూన్య సమితి_ident_var(అక్షరం **buf, అక్షరం *val)
{
	విడుదల(*buf);
	*buf = val;
}

స్థిర శూన్య determine_author_info(నిర్మాణం strbuf *author_ident)
{
	అక్షరం *name, *email, *date;
	నిర్మాణం ident_విభజించు author;

	name = xstrdup_or_null(getenv("GIT_AUTHOR_NAME"));
	email = xstrdup_or_null(getenv("GIT_AUTHOR_EMAIL"));
	date = xstrdup_or_null(getenv("GIT_AUTHOR_DATE"));

	ఒకవేళ (author_సందేశం) {
		నిర్మాణం ident_విభజించు ident;
		కొలత_t len;
		స్థిరరాశి అక్షరం *a;

		a = కనుగొను_commit_శీర్షిక(author_సందేశం_బఫరు, "author", &len);
		ఒకవేళ (!a)
			die(_("commit '%s' lacks author header"), author_సందేశం);
		ఒకవేళ (విభజించు_ident_పంక్తి(&ident, a, len) < 0)
			die(_("commit '%s' has malformed author line"), author_సందేశం);

		సమితి_ident_var(&name, xmemdupz(ident.name_begin, ident.name_చివర - ident.name_begin));
		సమితి_ident_var(&email, xmemdupz(ident.mail_begin, ident.mail_చివర - ident.mail_begin));

		ఒకవేళ (ident.date_begin) {
			నిర్మాణం strbuf date_buf = STRBUF_INIT;
			strbuf_addch(&date_buf, '@');
			strbuf_కూడు(&date_buf, ident.date_begin, ident.date_చివర - ident.date_begin);
			strbuf_addch(&date_buf, ' ');
			strbuf_కూడు(&date_buf, ident.tz_begin, ident.tz_చివర - ident.tz_begin);
			సమితి_ident_var(&date, strbuf_detach(&date_buf, ఖాళీ));
		}
	}

	ఒకవేళ (force_author) {
		నిర్మాణం ident_విభజించు ident;

		ఒకవేళ (విభజించు_ident_పంక్తి(&ident, force_author, అక్షరశ్రేణి_పొడవు(force_author)) < 0)
			die(_("malformed --author parameter"));
		సమితి_ident_var(&name, xmemdupz(ident.name_begin, ident.name_చివర - ident.name_begin));
		సమితి_ident_var(&email, xmemdupz(ident.mail_begin, ident.mail_చివర - ident.mail_begin));
	}

	ఒకవేళ (force_date) {
		నిర్మాణం strbuf date_buf = STRBUF_INIT;
		ఒకవేళ (parse_force_date(force_date, &date_buf))
			die(_("invalid date format: %s"), force_date);
		సమితి_ident_var(&date, strbuf_detach(&date_buf, ఖాళీ));
	}

	strbuf_addstr(author_ident, fmt_ident(name, email, WANT_AUTHOR_IDENT, date,
				IDENT_STRICT));
	assert_విభజించు_ident(&author, author_ident);
	export_one("GIT_AUTHOR_NAME", author.name_begin, author.name_చివర, 0);
	export_one("GIT_AUTHOR_EMAIL", author.mail_begin, author.mail_చివర, 0);
	export_one("GIT_AUTHOR_DATE", author.date_begin, author.tz_చివర, '@');
	విడుదల(name);
	విడుదల(email);
	విడుదల(date);
}

స్థిర పూర్ణాంకం author_date_is_interesting(శూన్య)
{
	తిరిగి author_సందేశం || force_date;
}

#నిర్వచితం_కాకపోతే WITH_BREAKING_CHANGES
స్థిర శూన్య adjust_comment_పంక్తి_char(స్థిరరాశి నిర్మాణం strbuf *sb)
{
	అక్షరం candidates[] = "#;@!$%^&|:";
	అక్షరం *candidate;
	స్థిరరాశి అక్షరం *p;
	కొలత_t cutoff;

	/* Ignore comment chars in trailing comments (e.g., Conflicts:) */
	cutoff = sb->len - ignored_log_సందేశం_బైట్లు(sb->buf, sb->len);

	ఒకవేళ (!memchr(sb->buf, candidates[0], sb->len)) {
		విడుదల(comment_పంక్తి_str_గమ్యం_free);
		comment_పంక్తి_str = comment_పంక్తి_str_గమ్యం_free =
			xstrfmt("%c", candidates[0]);
		తిరిగి;
	}

	p = sb->buf;
	candidate = strchr(candidates, *p);
	ఒకవేళ (candidate)
		*candidate = ' ';
	కొరకు (p = sb->buf; p + 1 < sb->buf + cutoff; p++) {
		ఒకవేళ ((p[0] == '\n' || p[0] == '\r') && p[1]) {
			candidate = strchr(candidates, p[1]);
			ఒకవేళ (candidate)
				*candidate = ' ';
		}
	}

	కొరకు (p = candidates; *p == ' '; p++)
		;
	ఒకవేళ (!*p)
		die(_("unable to select a comment character that is not used\n"
		      "in the current commit message"));
	విడుదల(comment_పంక్తి_str_గమ్యం_free);
	comment_పంక్తి_str = comment_పంక్తి_str_గమ్యం_free = xstrfmt("%c", *p);
}
#అయితే_ముగింపు /* !WITH_BREAKING_CHANGES */

స్థిర శూన్య prepare_amend_commit(నిర్మాణం commit *commit, నిర్మాణం strbuf *sb,
				నిర్మాణం pretty_print_context *ctx)
{
	స్థిరరాశి అక్షరం *బఫరు, *subject, *fmt;

	బఫరు = repo_get_commit_బఫరు(the_repository, commit, ఖాళీ);
	కనుగొను_commit_subject(బఫరు, &subject);
	/*
	 * If we amend the 'amend!' commit then we don't want to
	 * duplicate the subject line.
	 */
	fmt = starts_with(subject, "amend!") ? "%b" : "%B";
	repo_format_commit_సందేశం(the_repository, commit, fmt, sb, ctx);
	repo_unuse_commit_బఫరు(the_repository, commit, బఫరు);
}

స్థిర శూన్య change_దత్తాంశం_free(శూన్య *util, స్థిరరాశి అక్షరం *str వాడని)
{
	నిర్మాణం wt_స్థితి_change_దత్తాంశం *d = util;
	విడుదల(d->rename_ఆధారం);
	విడుదల(d);
}

స్థిర పూర్ణాంకం prepare_గమ్యం_commit(స్థిరరాశి అక్షరం *సూచీ_file, స్థిరరాశి అక్షరం *ఉపసర్గ,
			     నిర్మాణం commit *current_తల,
			     నిర్మాణం wt_స్థితి *s,
			     నిర్మాణం strbuf *author_ident)
{
	నిర్మాణం stat statbuf;
	నిర్మాణం strbuf committer_ident = STRBUF_INIT;
	పూర్ణాంకం committable;
	నిర్మాణం strbuf sb = STRBUF_INIT;
	స్థిరరాశి అక్షరం *hook_arg1 = ఖాళీ;
	స్థిరరాశి అక్షరం *hook_arg2 = ఖాళీ;
	పూర్ణాంకం clean_సందేశం_contents = (cleanup_బహుళకం != COMMIT_సందేశం_CLEANUP_NONE);
	పూర్ణాంకం old_display_comment_ఉపసర్గ;
	పూర్ణాంకం invoked_hook;

	/* This checks and barfs if author is badly specified */
	determine_author_info(author_ident);

	ఒకవేళ (!no_verify && నిరంతరం_commit_hook(use_editor, సూచీ_file, &invoked_hook,
					  "pre-commit", ఖాళీ))
		తిరిగి 0;

	ఒకవేళ (squash_సందేశం) {
		/*
		 * Insert the proper subject line before other commit
		 * message options add their content.
		 */
		ఒకవేళ (use_సందేశం && !strcmp(use_సందేశం, squash_సందేశం))
			strbuf_addstr(&sb, "squash! ");
		లేకపోతే {
			నిర్మాణం pretty_print_context ctx = {0};
			నిర్మాణం commit *c;
			c = lookup_commit_reference_by_name(squash_సందేశం);
			ఒకవేళ (!c)
				die(_("could not lookup commit '%s'"), squash_సందేశం);
			ctx.output_encoding = get_commit_output_encoding();
			repo_format_commit_సందేశం(the_repository, c,
						   "squash! %s\n\n", &sb,
						   &ctx);
		}
	}

	ఒకవేళ (have_option_m && !fixup_సందేశం) {
		strbuf_addbuf(&sb, &సందేశం_2);
		hook_arg1 = "message";
	} లేకపోతే ఒకవేళ (logfile && !strcmp(logfile, "-")) {
		ఒకవేళ (isatty(0))
			fprintf(stderr, _("(reading log message from standard input)\n"));
		ఒకవేళ (strbuf_read(&sb, 0, 0) < 0)
			die_errno(_("could not read log from standard input"));
		hook_arg1 = "message";
	} లేకపోతే ఒకవేళ (logfile) {
		ఒకవేళ (strbuf_read_file(&sb, logfile, 0) < 0)
			die_errno(_("could not read log file '%s'"),
				  logfile);
		hook_arg1 = "message";
	} లేకపోతే ఒకవేళ (use_సందేశం) {
		స్థిరరాశి అక్షరం *బఫరు;
		బఫరు = అక్షరశ్రేణి_శోధన(use_సందేశం_బఫరు, "\n\n");
		ఒకవేళ (బఫరు)
			strbuf_addstr(&sb, skip_blank_పంక్తులు(బఫరు + 2));
		hook_arg1 = "commit";
		hook_arg2 = use_సందేశం;
	} లేకపోతే ఒకవేళ (fixup_సందేశం) {
		నిర్మాణం pretty_print_context ctx = {0};
		నిర్మాణం commit *commit;
		అక్షరం *fmt;
		commit = lookup_commit_reference_by_name(fixup_commit);
		ఒకవేళ (!commit)
			die(_("could not lookup commit '%s'"), fixup_commit);
		ctx.output_encoding = get_commit_output_encoding();
		fmt = xstrfmt("%s! %%s\n\n", fixup_ఉపసర్గ);
		repo_format_commit_సందేశం(the_repository, commit, fmt, &sb,
					   &ctx);
		విడుదల(fmt);
		hook_arg1 = "message";

		/*
		 * Only `-m` commit message option is checked here, as
		 * it supports `--fixup` to append the commit message.
		 *
		 * The other commit message options `-c`/`-C`/`-F` are
		 * incompatible with all the forms of `--fixup` and
		 * have already errored out while parsing the `git commit`
		 * options.
		 */
		ఒకవేళ (have_option_m && !strcmp(fixup_ఉపసర్గ, "fixup"))
			strbuf_addbuf(&sb, &సందేశం_2);

		ఒకవేళ (!strcmp(fixup_ఉపసర్గ, "amend")) {
			ఒకవేళ (have_option_m)
				die(_("options '%s' and '%s:%s' cannot be used together"), "-m", "--fixup", fixup_సందేశం);
			prepare_amend_commit(commit, &sb, &ctx);
		}
	} లేకపోతే ఒకవేళ (!stat(git_మార్గం_విలీనం_సందేశం(the_repository), &statbuf)) {
		కొలత_t విలీనం_సందేశం_ప్రారంభం;

		/*
		 * prepend SQUASH_MSG here if it exists and a
		 * "merge --squash" was originally performed
		 */
		ఒకవేళ (!stat(git_మార్గం_squash_సందేశం(the_repository), &statbuf)) {
			ఒకవేళ (strbuf_read_file(&sb, git_మార్గం_squash_సందేశం(the_repository), 0) < 0)
				die_errno(_("could not read SQUASH_MSG"));
			hook_arg1 = "squash";
		} లేకపోతే
			hook_arg1 = "merge";

		విలీనం_సందేశం_ప్రారంభం = sb.len;
		ఒకవేళ (strbuf_read_file(&sb, git_మార్గం_విలీనం_సందేశం(the_repository), 0) < 0)
			die_errno(_("could not read MERGE_MSG"));

		ఒకవేళ (cleanup_బహుళకం == COMMIT_సందేశం_CLEANUP_SCISSORS &&
		    wt_స్థితి_locate_చివర(sb.buf + విలీనం_సందేశం_ప్రారంభం,
					 sb.len - విలీనం_సందేశం_ప్రారంభం) <
				sb.len - విలీనం_సందేశం_ప్రారంభం)
			s->added_cut_పంక్తి = 1;
	} లేకపోతే ఒకవేళ (!stat(git_మార్గం_squash_సందేశం(the_repository), &statbuf)) {
		ఒకవేళ (strbuf_read_file(&sb, git_మార్గం_squash_సందేశం(the_repository), 0) < 0)
			die_errno(_("could not read SQUASH_MSG"));
		hook_arg1 = "squash";
	} లేకపోతే ఒకవేళ (template_file) {
		ఒకవేళ (strbuf_read_file(&sb, template_file, 0) < 0)
			die_errno(_("could not read '%s'"), template_file);
		hook_arg1 = "template";
		clean_సందేశం_contents = 0;
	}

	/*
	 * The remaining cases don't modify the template message, but
	 * just set the argument(s) to the prepare-commit-msg hook.
	 */
	లేకపోతే ఒకవేళ (whence == FROM_విలీనం)
		hook_arg1 = "merge";
	లేకపోతే ఒకవేళ (is_from_cherry_pick(whence) || whence == FROM_REBASE_PICK) {
		hook_arg1 = "commit";
		hook_arg2 = "CHERRY_PICK_HEAD";
	}

	ఒకవేళ (squash_సందేశం) {
		/*
		 * If squash_commit was used for the commit subject,
		 * then we're possibly hijacking other commit log options.
		 * Reset the hook args to tell the real story.
		 */
		hook_arg1 = "message";
		hook_arg2 = "";
	}

	s->fp = fopen_for_writing(git_మార్గం_commit_editmsg());
	ఒకవేళ (!s->fp)
		die_errno(_("could not open '%s'"), git_మార్గం_commit_editmsg());

	/* Ignore status.displayCommentPrefix: we do need comments in COMMIT_EDITMSG. */
	old_display_comment_ఉపసర్గ = s->display_comment_ఉపసర్గ;
	s->display_comment_ఉపసర్గ = 1;

	/*
	 * Most hints are counter-productive when the commit has
	 * already started.
	 */
	s->hints = 0;

	ఒకవేళ (clean_సందేశం_contents)
		strbuf_stripspace(&sb, ఖాళీ);

	ఒకవేళ (signoff)
		append_signoff(&sb, ignored_log_సందేశం_బైట్లు(sb.buf, sb.len), 0);

	ఒకవేళ (fwrite(sb.buf, 1, sb.len, s->fp) < sb.len)
		die_errno(_("could not write commit template"));

#నిర్వచితం_కాకపోతే WITH_BREAKING_CHANGES
	ఒకవేళ (auto_comment_పంక్తి_char)
		adjust_comment_పంక్తి_char(&sb);
#అయితే_ముగింపు /* !WITH_BREAKING_CHANGES */
	strbuf_release(&sb);

	/* This checks if committer ident is explicitly given */
	strbuf_addstr(&committer_ident, git_committer_info(IDENT_STRICT));
	ఒకవేళ (use_editor && include_స్థితి) {
		పూర్ణాంకం ident_shown = 0;
		గణన git_colorbool saved_color_setting;
		నిర్మాణం ident_విభజించు ci, ai;
		స్థిరరాశి అక్షరం *hint_cleanup_all = allow_ఖాళీలు_సందేశం ?
			_("Please enter the commit message for your changes."
			  " Lines starting\nwith '%s' will be ignored.\n") :
			_("Please enter the commit message for your changes."
			  " Lines starting\nwith '%s' will be ignored, and an empty"
			  " message aborts the commit.\n");
		స్థిరరాశి అక్షరం *hint_cleanup_space = allow_ఖాళీలు_సందేశం ?
			_("Please enter the commit message for your changes."
			  " Lines starting\n"
			  "with '%s' will be kept; you may remove them"
			  " yourself if you want to.\n") :
			_("Please enter the commit message for your changes."
			  " Lines starting\n"
			  "with '%s' will be kept; you may remove them"
			  " yourself if you want to.\n"
			  "An empty message aborts the commit.\n");
		ఒకవేళ (whence != FROM_COMMIT) {
			ఒకవేళ (cleanup_బహుళకం == COMMIT_సందేశం_CLEANUP_SCISSORS)
				wt_స్థితి_కూడు_cut_పంక్తి(s);
			స్థితి_printf_ln(
				s, GIT_COLOR_సాధారణ,
				whence == FROM_విలీనం ?
					      _("\n"
					  "It looks like you may be committing a merge.\n"
					  "If this is not correct, please run\n"
					  "	git update-ref -d MERGE_HEAD\n"
					  "and try again.\n") :
					      _("\n"
					  "It looks like you may be committing a cherry-pick.\n"
					  "If this is not correct, please run\n"
					  "	git update-ref -d CHERRY_PICK_HEAD\n"
					  "and try again.\n"));
		}

		fprintf(s->fp, "\n");
		ఒకవేళ (cleanup_బహుళకం == COMMIT_సందేశం_CLEANUP_ALL)
			స్థితి_printf(s, GIT_COLOR_సాధారణ, hint_cleanup_all, comment_పంక్తి_str);
		లేకపోతే ఒకవేళ (cleanup_బహుళకం == COMMIT_సందేశం_CLEANUP_SCISSORS) {
			ఒకవేళ (whence == FROM_COMMIT)
				wt_స్థితి_కూడు_cut_పంక్తి(s);
		} లేకపోతే /* COMMIT_MSG_CLEANUP_SPACE, that is. */
			స్థితి_printf(s, GIT_COLOR_సాధారణ, hint_cleanup_space, comment_పంక్తి_str);

		/*
		 * These should never fail because they come from our own
		 * fmt_ident. They may fail the sane_ident test, but we know
		 * that the name and mail pointers will at least be valid,
		 * which is enough for our tests and printing here.
		 */
		assert_విభజించు_ident(&ai, author_ident);
		assert_విభజించు_ident(&ci, &committer_ident);

		ఒకవేళ (ident_cmp(&ai, &ci))
			స్థితి_printf_ln(s, GIT_COLOR_సాధారణ,
				_("%s"
				"Author:    %.*s <%.*s>"),
				ident_shown++ ? "" : "\n",
				(పూర్ణాంకం)(ai.name_చివర - ai.name_begin), ai.name_begin,
				(పూర్ణాంకం)(ai.mail_చివర - ai.mail_begin), ai.mail_begin);

		ఒకవేళ (author_date_is_interesting())
			స్థితి_printf_ln(s, GIT_COLOR_సాధారణ,
				_("%s"
				"Date:      %s"),
				ident_shown++ ? "" : "\n",
				show_ident_date(&ai, DATE_బహుళకం(సాధారణ)));

		ఒకవేళ (!committer_ident_sufficiently_given())
			స్థితి_printf_ln(s, GIT_COLOR_సాధారణ,
				_("%s"
				"Committer: %.*s <%.*s>"),
				ident_shown++ ? "" : "\n",
				(పూర్ణాంకం)(ci.name_చివర - ci.name_begin), ci.name_begin,
				(పూర్ణాంకం)(ci.mail_చివర - ci.mail_begin), ci.mail_begin);

		స్థితి_printf_ln(s, GIT_COLOR_సాధారణ, "%s", ""); /* Add new line for clarity */

		saved_color_setting = s->use_color;
		s->use_color = GIT_COLOR_NEVER;
		committable = నిరంతరం_స్థితి(s->fp, సూచీ_file, ఉపసర్గ, 1, s);
		s->use_color = saved_color_setting;
		string_list_clear_func(&s->change, change_దత్తాంశం_free);
	} లేకపోతే {
		నిర్మాణం object_id oid;
		స్థిరరాశి అక్షరం *మాతృ = "HEAD";

		ఒకవేళ (!the_repository->సూచీ->initialized && repo_read_సూచీ(the_repository) < 0)
			die(_("Cannot read index"));

		ఒకవేళ (amend)
			మాతృ = "HEAD^1";

		ఒకవేళ (repo_get_oid(the_repository, మాతృ, &oid)) {
			పూర్ణాంకం i, ita_nr = 0;

			/* TODO: audit for interaction with sparse-index. */
			ensure_నిండిన_సూచీ(the_repository->సూచీ);
			కొరకు (i = 0; i < the_repository->సూచీ->cache_nr; i++)
				ఒకవేళ (ce_intent_గమ్యం_కూడు(the_repository->సూచీ->cache[i]))
					ita_nr++;
			committable = the_repository->సూచీ->cache_nr > ita_nr;
		} లేకపోతే {
			/*
			 * Unless the user did explicitly request a submodule
			 * ignore mode by passing a command line option we do
			 * not ignore any changed submodule SHA-1s when
			 * comparing index and parent, no matter what is
			 * configured. Otherwise we won't commit any
			 * submodules which were manually staged, which would
			 * be really confusing.
			 */
			నిర్మాణం diff_flags flags = DIFF_FLAGS_INIT;
			flags.override_submodule_config = 1;
			ఒకవేళ (ignore_submodule_arg &&
			    !strcmp(ignore_submodule_arg, "all"))
				flags.ignore_submodules = 1;
			committable = సూచీ_differs_from(the_repository,
							 మాతృ, &flags, 1);
		}
	}
	strbuf_release(&committer_ident);

	fclose(s->fp);

	ఒకవేళ (trailer_వాదనలు.nr) {
		ఒకవేళ (amend_file_with_trailers(git_మార్గం_commit_editmsg(), &trailer_వాదనలు))
			die(_("unable to pass trailers to --trailers"));
		strvec_clear(&trailer_వాదనలు);
	}

	/*
	 * Reject an attempt to record a non-merge empty commit without
	 * explicit --allow-empty. In the cherry-pick case, it may be
	 * empty due to conflict resolution, which the user should okay.
	 */
	ఒకవేళ (!committable && whence != FROM_విలీనం && !allow_ఖాళీలు &&
	    !(amend && is_a_విలీనం(current_తల))) {
		s->hints = advice_enabled(ADVICE_స్థితి_HINTS);
		s->display_comment_ఉపసర్గ = old_display_comment_ఉపసర్గ;
		నిరంతరం_స్థితి(stdout, సూచీ_file, ఉపసర్గ, 0, s);
		ఒకవేళ (amend)
			fputs(_(ఖాళీలు_amend_advice), stderr);
		లేకపోతే ఒకవేళ (is_from_cherry_pick(whence) ||
			 whence == FROM_REBASE_PICK) {
			fputs(_(ఖాళీలు_cherry_pick_advice), stderr);
			ఒకవేళ (whence == FROM_CHERRY_PICK_SINGLE)
				fputs(_(ఖాళీలు_cherry_pick_advice_single), stderr);
			లేకపోతే ఒకవేళ (whence == FROM_CHERRY_PICK_MULTI)
				fputs(_(ఖాళీలు_cherry_pick_advice_multi), stderr);
			లేకపోతే
				fputs(_(ఖాళీలు_rebase_pick_advice), stderr);
		}
		తిరిగి 0;
	}

	ఒకవేళ (!no_verify && invoked_hook) {
		/*
		 * Re-read the index as the pre-commit-commit hook was invoked
		 * and could have updated it. We must do this before we invoke
		 * the editor and after we invoke run_status above.
		 */
		discard_సూచీ(the_repository->సూచీ);
	}
	read_సూచీ_from(the_repository->సూచీ, సూచీ_file, repo_get_git_dir(the_repository));

	ఒకవేళ (cache_వృక్షం_update(the_repository->సూచీ, 0)) {
		error(_("Error building trees"));
		తిరిగి 0;
	}

	ఒకవేళ (నిరంతరం_commit_hook(use_editor, సూచీ_file, ఖాళీ, "prepare-commit-msg",
			    git_మార్గం_commit_editmsg(), hook_arg1, hook_arg2, ఖాళీ))
		తిరిగి 0;

	ఒకవేళ (use_editor) {
		నిర్మాణం strvec env = STRVEC_INIT;

		strvec_pushf(&env, "GIT_INDEX_FILE=%s", సూచీ_file);
		ఒకవేళ (launch_editor(git_మార్గం_commit_editmsg(), ఖాళీ, env.v)) {
			fprintf(stderr,
			_("Please supply the message using either -m or -F option.\n"));
			నిష్క్రమించు(1);
		}
		strvec_clear(&env);
	}

	ఒకవేళ (!no_verify &&
	    నిరంతరం_commit_hook(use_editor, సూచీ_file, ఖాళీ, "commit-msg",
			    git_మార్గం_commit_editmsg(), ఖాళీ)) {
		తిరిగి 0;
	}

	తిరిగి 1;
}

స్థిర స్థిరరాశి అక్షరం *కనుగొను_author_by_nickname(స్థిరరాశి అక్షరం *name)
{
	నిర్మాణం rev_info revs;
	నిర్మాణం commit *commit;
	నిర్మాణం strbuf buf = STRBUF_INIT;
	స్థిరరాశి అక్షరం *av[20];
	పూర్ణాంకం ac = 0;

	repo_init_revisions(the_repository, &revs, ఖాళీ);
	strbuf_addf(&buf, "--author=%s", name);
	av[++ac] = "--all";
	av[++ac] = "-i";
	av[++ac] = buf.buf;
	av[++ac] = ఖాళీ;
	అమరిక_revisions(ac, av, &revs, ఖాళీ);
	revs.mailmap = xmalloc(పరిమాణం(నిర్మాణం string_list));
	string_list_init_nodup(revs.mailmap);
	read_mailmap(the_repository, revs.mailmap);

	ఒకవేళ (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	commit = get_revision(&revs);
	ఒకవేళ (commit) {
		నిర్మాణం pretty_print_context ctx = {0};
		ctx.date_బహుళకం.type = DATE_సాధారణ;
		strbuf_release(&buf);
		repo_format_commit_సందేశం(the_repository, commit,
					   "%aN <%aE>", &buf, &ctx);
		release_revisions(&revs);
		తిరిగి strbuf_detach(&buf, ఖాళీ);
	}
	die(_("--author '%s' is not 'Name <email>' and matches no existing author"), name);
}

స్థిర శూన్య handle_ignored_arg(నిర్మాణం wt_స్థితి *s)
{
	ఒకవేళ (!ignored_arg)
		; /* default already initialized */
	లేకపోతే ఒకవేళ (!strcmp(ignored_arg, "traditional"))
		s->show_ignored_బహుళకం = SHOW_TRADITIONAL_IGNORED;
	లేకపోతే ఒకవేళ (!strcmp(ignored_arg, "no"))
		s->show_ignored_బహుళకం = SHOW_NO_IGNORED;
	లేకపోతే ఒకవేళ (!strcmp(ignored_arg, "matching"))
		s->show_ignored_బహుళకం = SHOW_MATCHING_IGNORED;
	లేకపోతే
		die(_("Invalid ignored mode '%s'"), ignored_arg);
}

స్థిర గణన untracked_స్థితి_type parse_untracked_setting_name(స్థిరరాశి అక్షరం *u)
{
	/*
	 * Please update $__git_untracked_file_modes in
	 * git-completion.bash when you add new options
	 */
	ఎంపిక (git_parse_maybe_bool(u)) {
	స్థితి 0:
		u = "no";
		ఆపు;
	స్థితి 1:
		u = "normal";
		ఆపు;
	అప్రమేయ:
		ఆపు;
	}

	ఒకవేళ (!strcmp(u, "no"))
		తిరిగి SHOW_NO_UNTRACKED_FILES;
	లేకపోతే ఒకవేళ (!strcmp(u, "normal"))
		తిరిగి SHOW_సాధారణ_UNTRACKED_FILES;
	లేకపోతే ఒకవేళ (!strcmp(u, "all"))
		తిరిగి SHOW_ALL_UNTRACKED_FILES;
	లేకపోతే
		తిరిగి SHOW_UNTRACKED_FILES_ERROR;
}

స్థిర శూన్య handle_untracked_files_arg(నిర్మాణం wt_స్థితి *s)
{
	గణన untracked_స్థితి_type u;

	ఒకవేళ (!untracked_files_arg)
		తిరిగి; /* default already initialized */

	u = parse_untracked_setting_name(untracked_files_arg);
	ఒకవేళ (u == SHOW_UNTRACKED_FILES_ERROR)
		die(_("Invalid untracked files mode '%s'"),
		    untracked_files_arg);
	s->show_untracked_files = u;
}

స్థిర స్థిరరాశి అక్షరం *read_commit_సందేశం_2(స్థిరరాశి అక్షరం *name)
{
	స్థిరరాశి అక్షరం *బహిర్గతం_enc;
	నిర్మాణం commit *commit;

	commit = lookup_commit_reference_by_name(name);
	ఒకవేళ (!commit)
		die(_("could not lookup commit '%s'"), name);
	బహిర్గతం_enc = get_commit_output_encoding();
	తిరిగి repo_logmsg_reencode(the_repository, commit, ఖాళీ, బహిర్గతం_enc);
}

/*
 * Enumerate what needs to be propagated when --porcelain
 * is not in effect here.
 */
స్థిర నిర్మాణం స్థితి_deferred_config {
	గణన wt_స్థితి_format స్థితి_format;
	పూర్ణాంకం show_branch;
	గణన ahead_behind_flags ahead_behind;
} స్థితి_deferred_config = {
	స్థితి_FORMAT_UNSPECIFIED,
	-1, /* unspecified */
	AHEAD_BEHIND_UNSPECIFIED,
};

స్థిర శూన్య finalize_deferred_config(నిర్మాణం wt_స్థితి *s)
{
	పూర్ణాంకం use_deferred_config = (స్థితి_format != స్థితి_FORMAT_PORCELAIN &&
				   స్థితి_format != స్థితి_FORMAT_PORCELAIN_V2 &&
				   !s->null_termination);

	ఒకవేళ (s->null_termination) {
		ఒకవేళ (స్థితి_format == స్థితి_FORMAT_NONE ||
		    స్థితి_format == స్థితి_FORMAT_UNSPECIFIED)
			స్థితి_format = స్థితి_FORMAT_PORCELAIN;
		లేకపోతే ఒకవేళ (స్థితి_format == స్థితి_FORMAT_LONG)
			die(_("options '%s' and '%s' cannot be used together"), "--long", "-z");
	}

	ఒకవేళ (use_deferred_config && స్థితి_format == స్థితి_FORMAT_UNSPECIFIED)
		స్థితి_format = స్థితి_deferred_config.స్థితి_format;
	ఒకవేళ (స్థితి_format == స్థితి_FORMAT_UNSPECIFIED)
		స్థితి_format = స్థితి_FORMAT_NONE;

	ఒకవేళ (use_deferred_config && s->show_branch < 0)
		s->show_branch = స్థితి_deferred_config.show_branch;
	ఒకవేళ (s->show_branch < 0)
		s->show_branch = 0;

	/*
	 * If the user did not give a "--[no]-ahead-behind" command
	 * line argument *AND* we will print in a human-readable format
	 * (short, long etc.) then we inherit from the status.aheadbehind
	 * config setting.  In all other cases (and porcelain V[12] formats
	 * in particular), we inherit _FULL for backwards compatibility.
	 */
	ఒకవేళ (use_deferred_config &&
	    s->ahead_behind_flags == AHEAD_BEHIND_UNSPECIFIED)
		s->ahead_behind_flags = స్థితి_deferred_config.ahead_behind;

	ఒకవేళ (s->ahead_behind_flags == AHEAD_BEHIND_UNSPECIFIED)
		s->ahead_behind_flags = AHEAD_BEHIND_నిండిన;
}

స్థిర శూన్య check_fixup_reword_options(పూర్ణాంకం argc, స్థిరరాశి అక్షరం *argv[]) {
	ఒకవేళ (whence != FROM_COMMIT) {
		ఒకవేళ (whence == FROM_విలీనం)
			die(_("You are in the middle of a merge -- cannot reword."));
		లేకపోతే ఒకవేళ (is_from_cherry_pick(whence))
			die(_("You are in the middle of a cherry-pick -- cannot reword."));
	}
	ఒకవేళ (argc)
		die(_("reword option of '%s' and path '%s' cannot be used together"), "--fixup", *argv);
	ఒకవేళ (patch_interactive || interactive || all || also || only)
		die(_("reword option of '%s' and '%s' cannot be used together"),
			"--fixup", "--patch/--interactive/--all/--include/--only");
}

స్థిర పూర్ణాంకం parse_and_validate_options(పూర్ణాంకం argc, స్థిరరాశి అక్షరం *argv[],
				      స్థిరరాశి నిర్మాణం option *options,
				      స్థిరరాశి అక్షరం * స్థిరరాశి usage[],
				      స్థిరరాశి అక్షరం *ఉపసర్గ,
				      నిర్మాణం commit *current_తల,
				      నిర్మాణం wt_స్థితి *s)
{
	argc = parse_options(argc, argv, ఉపసర్గ, options, usage, 0);
	finalize_deferred_config(s);

	ఒకవేళ (force_author && !strchr(force_author, '>'))
		force_author = కనుగొను_author_by_nickname(force_author);

	ఒకవేళ (force_author && renew_authorship)
		die(_("options '%s' and '%s' cannot be used together"), "--reset-author", "--author");

	ఒకవేళ (logfile || have_option_m || use_సందేశం)
		use_editor = 0;

	/* Sanity check options */
	ఒకవేళ (amend && !current_తల)
		die(_("You have nothing to amend."));
	ఒకవేళ (amend && whence != FROM_COMMIT) {
		ఒకవేళ (whence == FROM_విలీనం)
			die(_("You are in the middle of a merge -- cannot amend."));
		లేకపోతే ఒకవేళ (is_from_cherry_pick(whence))
			die(_("You are in the middle of a cherry-pick -- cannot amend."));
		లేకపోతే ఒకవేళ (whence == FROM_REBASE_PICK)
			die(_("You are in the middle of a rebase -- cannot amend."));
	}
	ఒకవేళ (fixup_సందేశం && squash_సందేశం)
		die(_("options '%s' and '%s' cannot be used together"), "--squash", "--fixup");
	die_for_incompatible_opt4(!!use_సందేశం, "-C",
				  !!edit_సందేశం, "-c",
				  !!logfile, "-F",
				  !!fixup_సందేశం, "--fixup");
	die_for_incompatible_opt4(have_option_m, "-m",
				  !!edit_సందేశం, "-c",
				  !!use_సందేశం, "-C",
				  !!logfile, "-F");
	ఒకవేళ (use_సందేశం || edit_సందేశం || logfile ||fixup_సందేశం || have_option_m)
		FREE_AND_NULL(template_file);
	ఒకవేళ (edit_సందేశం)
		use_సందేశం = edit_సందేశం;
	ఒకవేళ (amend && !use_సందేశం && !fixup_సందేశం)
		use_సందేశం = "HEAD";
	ఒకవేళ (!use_సందేశం && !is_from_cherry_pick(whence) &&
	    !is_from_rebase(whence) && renew_authorship)
		die(_("--reset-author can be used only with -C, -c or --amend."));
	ఒకవేళ (use_సందేశం) {
		use_సందేశం_బఫరు = read_commit_సందేశం_2(use_సందేశం);
		ఒకవేళ (!renew_authorship) {
			author_సందేశం = use_సందేశం;
			author_సందేశం_బఫరు = use_సందేశం_బఫరు;
		}
	}
	ఒకవేళ ((is_from_cherry_pick(whence) || whence == FROM_REBASE_PICK) &&
	    !renew_authorship) {
		author_సందేశం = "CHERRY_PICK_HEAD";
		author_సందేశం_బఫరు = read_commit_సందేశం_2(author_సందేశం);
	}

	ఒకవేళ (patch_interactive)
		interactive = 1;

	die_for_incompatible_opt4(also, "-i/--include",
				  only, "-o/--only",
				  all, "-a/--all",
				  interactive, "--interactive/-p/--patch");
	ఒకవేళ (fixup_సందేశం) {
		/*
		 * We limit --fixup's suboptions to only alpha characters.
		 * If the first character after a run of alpha is colon,
		 * then the part before the colon may be a known suboption
		 * name like `amend` or `reword`, or a misspelt suboption
		 * name. In either case, we treat it as
		 * --fixup=<suboption>:<arg>.
		 *
		 * Otherwise, we are dealing with --fixup=<commit>.
		 */
		అక్షరం *p = fixup_సందేశం;
		అయితే (అక్షరమా(*p))
			p++;
		ఒకవేళ (p > fixup_సందేశం && *p == ':') {
			*p = '\0';
			fixup_commit = p + 1;
			ఒకవేళ (!strcmp("amend", fixup_సందేశం) ||
			    !strcmp("reword", fixup_సందేశం)) {
				fixup_ఉపసర్గ = "amend";
				allow_ఖాళీలు = 1;
				ఒకవేళ (*fixup_సందేశం == 'r') {
					check_fixup_reword_options(argc, argv);
					only = 1;
				}
			} లేకపోతే {
				die(_("unknown option: --fixup=%s:%s"), fixup_సందేశం, fixup_commit);
			}
		} లేకపోతే {
			fixup_commit = fixup_సందేశం;
			fixup_ఉపసర్గ = "fixup";
			use_editor = 0;
		}
	}

	ఒకవేళ (0 <= edit_flag)
		use_editor = edit_flag;

	handle_untracked_files_arg(s);

	ఒకవేళ (all && argc > 0)
		die(_("paths '%s ...' with -a does not make sense"),
		    argv[0]);

	ఒకవేళ (స్థితి_format != స్థితి_FORMAT_NONE)
		dry_నిరంతరం = 1;

	తిరిగి argc;
}

స్థిర పూర్ణాంకం dry_నిరంతరం_commit(స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
			  స్థిరరాశి నిర్మాణం commit *current_తల, నిర్మాణం wt_స్థితి *s)
{
	పూర్ణాంకం committable;
	స్థిరరాశి అక్షరం *సూచీ_file;

	సూచీ_file = prepare_సూచీ(argv, ఉపసర్గ, current_తల, 1);
	committable = నిరంతరం_స్థితి(stdout, సూచీ_file, ఉపసర్గ, 0, s);
	rollback_సూచీ_files();

	తిరిగి committable ? 0 : 1;
}

define_list_config_array_extra(color_స్థితి_slots, {"added"});

స్థిర పూర్ణాంకం parse_స్థితి_స్థానం(స్థిరరాశి అక్షరం *స్థానం)
{
	ఒకవేళ (!strcasecmp(స్థానం, "added"))
		తిరిగి WT_స్థితి_UPDATED;

	తిరిగి LOOKUP_CONFIG(color_స్థితి_slots, స్థానం);
}

స్థిర పూర్ణాంకం git_స్థితి_config(స్థిరరాశి అక్షరం *k, స్థిరరాశి అక్షరం *v,
			     స్థిరరాశి నిర్మాణం config_context *ctx, శూన్య *cb)
{
	నిర్మాణం wt_స్థితి *s = cb;
	స్థిరరాశి అక్షరం *స్థానం_name;

	ఒకవేళ (starts_with(k, "column."))
		తిరిగి git_స్తంభం_config(k, v, "status", &s->colopts);
	ఒకవేళ (!strcmp(k, "status.submodulesummary")) {
		పూర్ణాంకం is_bool;
		s->submodule_summary = git_config_bool_or_int(k, v, ctx->kvi,
							      &is_bool);
		ఒకవేళ (is_bool && s->submodule_summary)
			s->submodule_summary = -1;
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(k, "status.short")) {
		ఒకవేళ (git_config_bool(k, v))
			స్థితి_deferred_config.స్థితి_format = స్థితి_FORMAT_SHORT;
		లేకపోతే
			స్థితి_deferred_config.స్థితి_format = స్థితి_FORMAT_NONE;
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(k, "status.branch")) {
		స్థితి_deferred_config.show_branch = git_config_bool(k, v);
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(k, "status.aheadbehind")) {
		స్థితి_deferred_config.ahead_behind = git_config_bool(k, v);
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(k, "status.showstash")) {
		s->show_stash = git_config_bool(k, v);
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(k, "status.color") || !strcmp(k, "color.status")) {
		s->use_color = git_config_colorbool(k, v);
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(k, "status.displaycommentprefix")) {
		s->display_comment_ఉపసర్గ = git_config_bool(k, v);
		తిరిగి 0;
	}
	ఒకవేళ (skip_ఉపసర్గ(k, "status.color.", &స్థానం_name) ||
	    skip_ఉపసర్గ(k, "color.status.", &స్థానం_name)) {
		పూర్ణాంకం స్థానం = parse_స్థితి_స్థానం(స్థానం_name);
		ఒకవేళ (స్థానం < 0)
			తిరిగి 0;
		ఒకవేళ (!v)
			తిరిగి config_error_nonbool(k);
		తిరిగి color_parse(v, s->color_palette[స్థానం]);
	}
	ఒకవేళ (!strcmp(k, "status.relativepaths")) {
		s->relative_paths = git_config_bool(k, v);
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(k, "status.showuntrackedfiles")) {
		గణన untracked_స్థితి_type u;

		u = parse_untracked_setting_name(v);
		ఒకవేళ (u == SHOW_UNTRACKED_FILES_ERROR)
			తిరిగి error(_("Invalid untracked files mode '%s'"), v);
		s->show_untracked_files = u;
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(k, "diff.renamelimit")) {
		ఒకవేళ (s->rename_హద్దు == -1)
			s->rename_హద్దు = git_config_int(k, v, ctx->kvi);
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(k, "status.renamelimit")) {
		s->rename_హద్దు = git_config_int(k, v, ctx->kvi);
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(k, "diff.renames")) {
		ఒకవేళ (s->detect_rename == -1)
			s->detect_rename = git_config_rename(k, v);
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(k, "status.renames")) {
		s->detect_rename = git_config_rename(k, v);
		తిరిగి 0;
	}
	తిరిగి git_diff_ui_config(k, v, ctx, ఖాళీ);
}

పూర్ణాంకం cmd_స్థితి(పూర్ణాంకం argc,
స్థిరరాశి అక్షరం **argv,
స్థిరరాశి అక్షరం *ఉపసర్గ,
నిర్మాణం repository *repo వాడని)
{
	స్థిర పూర్ణాంకం no_renames = -1;
	స్థిర స్థిరరాశి అక్షరం *rename_score_arg = (స్థిరరాశి అక్షరం *)-1;
	స్థిర నిర్మాణం wt_స్థితి s;
	గుర్తులేని పూర్ణాంకం పురోగతి_flag = 0;
	పూర్ణాంకం fd;
	నిర్మాణం object_id oid;
	స్థిర నిర్మాణం option builtin_స్థితి_options[] = {
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT_సమితి_INT('s', "short", &స్థితి_format,
			    N_("show status concisely"), స్థితి_FORMAT_SHORT),
		OPT_BOOL('b', "branch", &s.show_branch,
			 N_("show branch information")),
		OPT_BOOL(0, "show-stash", &s.show_stash,
			 N_("show stash information")),
		OPT_BOOL(0, "ahead-behind", &s.ahead_behind_flags,
			 N_("compute full ahead/behind values")),
		OPT_CALLBACK_F(0, "porcelain", &స్థితి_format,
		  N_("version"), N_("machine-readable output"),
		  PARSE_OPT_OPTARG, opt_parse_porcelain),
		OPT_సమితి_INT(0, "long", &స్థితి_format,
			    N_("show status in long format (default)"),
			    స్థితి_FORMAT_LONG),
		OPT_BOOL('z', "null", &s.null_termination,
			 N_("terminate entries with NUL")),
		{
			.type = OPTION_STRING,
			.short_name = 'u',
			.long_name = "untracked-files",
			.విలువ = &untracked_files_arg,
			.argh = N_("mode"),
			.help = N_("show untracked files, optional modes: all, normal, no. (Default: all)"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t)"all",
		},
		{
			.type = OPTION_STRING,
			.long_name = "ignored",
			.విలువ = &ignored_arg,
			.argh = N_("mode"),
			.help = N_("show ignored files, optional modes: traditional, matching, no. (Default: traditional)"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t)"traditional",
		},
		{
			.type = OPTION_STRING,
			.long_name = "ignore-submodules",
			.విలువ = &ignore_submodule_arg,
			.argh = N_("when"),
			.help = N_("ignore changes to submodules, optional when: all, dirty, untracked. (Default: all)"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t)"all",
		},
		OPT_స్తంభం(0, "column", &s.colopts, N_("list untracked files in columns")),
		OPT_BOOL(0, "no-renames", &no_renames, N_("do not detect renames")),
		OPT_CALLBACK_F('M', "find-renames", &rename_score_arg,
		  N_("n"), N_("detect renames, optionally set similarity index"),
		  PARSE_OPT_OPTARG | PARSE_OPT_NONEG, opt_parse_rename_score),
		OPT_చివర(),
	};

	show_usage_with_options_if_asked(argc, argv,
					 builtin_స్థితి_usage, builtin_స్థితి_options);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_నిండిన_సూచీ = 0;

	స్థితి_init_config(&s, git_స్థితి_config);
	argc = parse_options(argc, argv, ఉపసర్గ,
			     builtin_స్థితి_options,
			     builtin_స్థితి_usage, 0);
	finalize_colopts(&s.colopts, -1);
	finalize_deferred_config(&s);

	handle_untracked_files_arg(&s);
	handle_ignored_arg(&s);

	ఒకవేళ (s.show_ignored_బహుళకం == SHOW_MATCHING_IGNORED &&
	    s.show_untracked_files == SHOW_NO_UNTRACKED_FILES)
		die(_("Unsupported combination of ignored and untracked-files arguments"));

	parse_pathspec(&s.pathspec, 0,
		       PATHSPEC_PREFER_నిండిన,
		       ఉపసర్గ, argv);

	ఒకవేళ (స్థితి_format != స్థితి_FORMAT_PORCELAIN &&
	    స్థితి_format != స్థితి_FORMAT_PORCELAIN_V2)
		పురోగతి_flag = REFRESH_పురోగతి;
	repo_read_సూచీ(the_repository);
	refresh_సూచీ(the_repository->సూచీ,
		      REFRESH_QUIET|REFRESH_UNMERGED|పురోగతి_flag,
		      &s.pathspec, ఖాళీ, ఖాళీ);

	ఒకవేళ (use_optional_locks())
		fd = repo_hold_locked_సూచీ(the_repository, &సూచీ_తాళం, 0);
	లేకపోతే
		fd = -1;

	s.is_initial = repo_get_oid(the_repository, s.reference, &oid) ? 1 : 0;
	ఒకవేళ (!s.is_initial)
		oidcpy(&s.oid_commit, &oid);

	s.ignore_submodule_arg = ignore_submodule_arg;
	s.స్థితి_format = స్థితి_format;
	s.verbose = verbose;
	ఒకవేళ (no_renames != -1)
		s.detect_rename = !no_renames;
	ఒకవేళ ((intptr_t)rename_score_arg != -1) {
		ఒకవేళ (s.detect_rename < DIFF_DETECT_RENAME)
			s.detect_rename = DIFF_DETECT_RENAME;
		ఒకవేళ (rename_score_arg)
			s.rename_score = parse_rename_score(&rename_score_arg);
	}

	wt_స్థితి_collect(&s);

	ఒకవేళ (0 <= fd)
		repo_update_సూచీ_if_able(the_repository, &సూచీ_తాళం);

	ఒకవేళ (s.relative_paths)
		s.ఉపసర్గ = ఉపసర్గ;

	wt_స్థితి_print(&s);
	wt_స్థితి_collect_free_buffers(&s);

	తిరిగి 0;
}

స్థిర పూర్ణాంకం git_commit_config(స్థిరరాశి అక్షరం *k, స్థిరరాశి అక్షరం *v,
			     స్థిరరాశి నిర్మాణం config_context *ctx, శూన్య *cb)
{
	నిర్మాణం wt_స్థితి *s = cb;

	ఒకవేళ (!strcmp(k, "commit.template"))
		తిరిగి git_config_pathname(&template_file, k, v);
	ఒకవేళ (!strcmp(k, "commit.status")) {
		include_స్థితి = git_config_bool(k, v);
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(k, "commit.cleanup")) {
		FREE_AND_NULL(cleanup_config);
		తిరిగి git_config_string(&cleanup_config, k, v);
	}
	ఒకవేళ (!strcmp(k, "commit.gpgsign")) {
		sign_commit = git_config_bool(k, v) ? "" : ఖాళీ;
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(k, "commit.verbose")) {
		పూర్ణాంకం is_bool;
		config_commit_verbose = git_config_bool_or_int(k, v, ctx->kvi,
							       &is_bool);
		తిరిగి 0;
	}

	తిరిగి git_స్థితి_config(k, v, ctx, s);
}

పూర్ణాంకం cmd_commit(పూర్ణాంకం argc,
	       స్థిరరాశి అక్షరం **argv,
	       స్థిరరాశి అక్షరం *ఉపసర్గ,
	       నిర్మాణం repository *repo వాడని)
{
	స్థిర నిర్మాణం wt_స్థితి s;
	స్థిర స్థిరరాశి అక్షరం *cleanup_arg = ఖాళీ;
	స్థిర నిర్మాణం option builtin_commit_options[] = {
		OPT__QUIET(&quiet, N_("suppress summary after successful commit")),
		OPT__VERBOSE(&verbose, N_("show diff in commit message template")),

		OPT_సమూహం(N_("Commit message options")),
		OPT_FILENAME('F', "file", &logfile, N_("read message from file")),
		OPT_STRING(0, "author", &force_author, N_("author"), N_("override author for commit")),
		OPT_STRING(0, "date", &force_date, N_("date"), N_("override date for commit")),
		OPT_CALLBACK('m', "message", &సందేశం_2, N_("message"), N_("commit message"), opt_parse_m),
		OPT_STRING('c', "reedit-message", &edit_సందేశం, N_("commit"), N_("reuse and edit message from specified commit")),
		OPT_STRING('C', "reuse-message", &use_సందేశం, N_("commit"), N_("reuse message from specified commit")),
		/*
		 * TRANSLATORS: Leave "[(amend|reword):]" as-is,
		 * and only translate <commit>.
		 */
		OPT_STRING(0, "fixup", &fixup_సందేశం, N_("[(amend|reword):]commit"), N_("use autosquash formatted message to fixup or amend/reword specified commit")),
		OPT_STRING(0, "squash", &squash_సందేశం, N_("commit"), N_("use autosquash formatted message to squash specified commit")),
		OPT_BOOL(0, "reset-author", &renew_authorship, N_("the commit is authored by me now (used with -C/-c/--amend)")),
		OPT_STRVEC(0, "trailer", &trailer_వాదనలు, N_("trailer"),
			   N_("add custom trailer(s)")),
		OPT_BOOL('s', "signoff", &signoff, N_("add a Signed-off-by trailer")),
		OPT_FILENAME('t', "template", &template_file, N_("use specified template file")),
		OPT_BOOL('e', "edit", &edit_flag, N_("force edit of commit")),
		OPT_CLEANUP(&cleanup_arg),
		OPT_BOOL(0, "status", &include_స్థితి, N_("include status in commit message template")),
		{
			.type = OPTION_STRING,
			.short_name = 'S',
			.long_name = "gpg-sign",
			.విలువ = &sign_commit,
			.argh = N_("key-id"),
			.help = N_("GPG sign commit"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "",
		},
		/* end commit message options */

		OPT_సమూహం(N_("Commit contents options")),
		OPT_BOOL('a', "all", &all, N_("commit all changed files")),
		OPT_BOOL('i', "include", &also, N_("add specified files to index for commit")),
		OPT_BOOL(0, "interactive", &interactive, N_("interactively add files")),
		OPT_BOOL('p', "patch", &patch_interactive, N_("interactively add changes")),
		OPT_DIFF_UNIFIED(&interactive_opts.context),
		OPT_DIFF_INTERHUNK_CONTEXT(&interactive_opts.interhunkcontext),
		OPT_BOOL('o', "only", &only, N_("commit only specified files")),
		OPT_BOOL('n', "no-verify", &no_verify, N_("bypass pre-commit and commit-msg hooks")),
		OPT_BOOL(0, "dry-run", &dry_నిరంతరం, N_("show what would be committed")),
		OPT_సమితి_INT(0, "short", &స్థితి_format, N_("show status concisely"),
			    స్థితి_FORMAT_SHORT),
		OPT_BOOL(0, "branch", &s.show_branch, N_("show branch information")),
		OPT_BOOL(0, "ahead-behind", &s.ahead_behind_flags,
			 N_("compute full ahead/behind values")),
		OPT_సమితి_INT(0, "porcelain", &స్థితి_format,
			    N_("machine-readable output"), స్థితి_FORMAT_PORCELAIN),
		OPT_సమితి_INT(0, "long", &స్థితి_format,
			    N_("show status in long format (default)"),
			    స్థితి_FORMAT_LONG),
		OPT_BOOL('z', "null", &s.null_termination,
			 N_("terminate entries with NUL")),
		OPT_BOOL(0, "amend", &amend, N_("amend previous commit")),
		OPT_BOOL(0, "no-post-rewrite", &no_post_rewrite, N_("bypass post-rewrite hook")),
		{
			.type = OPTION_STRING,
			.short_name = 'u',
			.long_name = "untracked-files",
			.విలువ = &untracked_files_arg,
			.argh = N_("mode"),
			.help = N_("show untracked files, optional modes: all, normal, no. (Default: all)"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t)"all",
		},
		OPT_PATHSPEC_FROM_FILE(&pathspec_from_file),
		OPT_PATHSPEC_FILE_NUL(&pathspec_file_nul),
		/* end commit contents options */

		OPT_HIDDEN_BOOL(0, "allow-empty", &allow_ఖాళీలు,
				N_("ok to record an empty change")),
		OPT_HIDDEN_BOOL(0, "allow-empty-message", &allow_ఖాళీలు_సందేశం,
				N_("ok to record a change with an empty message")),

		OPT_చివర()
	};

	నిర్మాణం strbuf sb = STRBUF_INIT;
	నిర్మాణం strbuf author_ident = STRBUF_INIT;
	స్థిరరాశి అక్షరం *సూచీ_file, *reflog_సందేశం;
	నిర్మాణం object_id oid;
	నిర్మాణం commit_list *parents = ఖాళీ;
	నిర్మాణం stat statbuf;
	నిర్మాణం commit *current_తల = ఖాళీ;
	నిర్మాణం commit_extra_శీర్షిక *extra = ఖాళీ;
	నిర్మాణం strbuf err = STRBUF_INIT;
	పూర్ణాంకం ret = 0;

	show_usage_with_options_if_asked(argc, argv,
					 builtin_commit_usage, builtin_commit_options);

#నిర్వచితం_కాకపోతే WITH_BREAKING_CHANGES
	warn_on_auto_comment_char = true;
#అయితే_ముగింపు /* !WITH_BREAKING_CHANGES */
	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_నిండిన_సూచీ = 0;

	స్థితి_init_config(&s, git_commit_config);
	s.commit_template = 1;
	స్థితి_format = స్థితి_FORMAT_NONE; /* Ignore status.short */
	s.colopts = 0;

	ఒకవేళ (repo_get_oid(the_repository, "HEAD", &oid))
		current_తల = ఖాళీ;
	లేకపోతే {
		current_తల = lookup_commit_or_die(&oid, "HEAD");
		ఒకవేళ (repo_parse_commit(the_repository, current_తల))
			die(_("could not parse HEAD commit"));
	}
	verbose = -1; /* unspecified */
	argc = parse_and_validate_options(argc, argv, builtin_commit_options,
					  builtin_commit_usage,
					  ఉపసర్గ, current_తల, &s);
	ఒకవేళ (trailer_వాదనలు.nr)
		trailer_config_init();

	ఒకవేళ (verbose == -1)
		verbose = (config_commit_verbose < 0) ? 0 : config_commit_verbose;

	ఒకవేళ (cleanup_arg) {
		విడుదల(cleanup_config);
		cleanup_config = xstrdup(cleanup_arg);
	}
	cleanup_బహుళకం = get_cleanup_బహుళకం(cleanup_config, use_editor);

	ఒకవేళ (dry_నిరంతరం)
		తిరిగి dry_నిరంతరం_commit(argv, ఉపసర్గ, current_తల, &s);
	సూచీ_file = prepare_సూచీ(argv, ఉపసర్గ, current_తల, 0);

	/* Set up everything for writing the commit object.  This includes
	   running hooks, writing the trees, and interacting with the user.  */
	ఒకవేళ (!prepare_గమ్యం_commit(సూచీ_file, ఉపసర్గ,
			       current_తల, &s, &author_ident)) {
		ret = 1;
		rollback_సూచీ_files();
		వెళ్ళు cleanup;
	}

	/* Determine parents */
	reflog_సందేశం = getenv("GIT_REFLOG_ACTION");
	ఒకవేళ (!current_తల) {
		ఒకవేళ (!reflog_సందేశం)
			reflog_సందేశం = "commit (initial)";
	} లేకపోతే ఒకవేళ (amend) {
		ఒకవేళ (!reflog_సందేశం)
			reflog_సందేశం = "commit (amend)";
		parents = commit_list_copy(current_తల->parents);
	} లేకపోతే ఒకవేళ (whence == FROM_విలీనం) {
		నిర్మాణం strbuf m = STRBUF_INIT;
		FILE *fp;
		పూర్ణాంకం allow_fast_forward = 1;
		నిర్మాణం commit_list **pptr = &parents;

		ఒకవేళ (!reflog_సందేశం)
			reflog_సందేశం = "commit (merge)";
		pptr = commit_list_append(current_తల, pptr);
		fp = xfopen(git_మార్గం_విలీనం_తల(the_repository), "r");
		అయితే (strbuf_getline_lf(&m, fp) != EOF) {
			నిర్మాణం commit *మాతృ;

			మాతృ = get_విలీనం_మాతృ(m.buf);
			ఒకవేళ (!మాతృ)
				die(_("Corrupt MERGE_HEAD file (%s)"), m.buf);
			pptr = commit_list_append(మాతృ, pptr);
		}
		fclose(fp);
		strbuf_release(&m);
		ఒకవేళ (!stat(git_మార్గం_విలీనం_బహుళకం(the_repository), &statbuf)) {
			ఒకవేళ (strbuf_read_file(&sb, git_మార్గం_విలీనం_బహుళకం(the_repository), 0) < 0)
				die_errno(_("could not read MERGE_MODE"));
			ఒకవేళ (!strcmp(sb.buf, "no-ff"))
				allow_fast_forward = 0;
		}
		ఒకవేళ (allow_fast_forward)
			reduce_heads_replace(&parents);
	} లేకపోతే {
		ఒకవేళ (!reflog_సందేశం)
			reflog_సందేశం = is_from_cherry_pick(whence)
					? "commit (cherry-pick)"
					: is_from_rebase(whence)
					? "commit (rebase)"
					: "commit";
		commit_list_చొప్పించు(current_తల, &parents);
	}

	/* Finally, get the commit message */
	strbuf_reset(&sb);
	ఒకవేళ (strbuf_read_file(&sb, git_మార్గం_commit_editmsg(), 0) < 0) {
		పూర్ణాంకం saved_errno = errno;
		rollback_సూచీ_files();
		die(_("could not read commit message: %s"), strerror(saved_errno));
	}

	cleanup_సందేశం(&sb, cleanup_బహుళకం, verbose);

	ఒకవేళ (సందేశం_is_ఖాళీలు(&sb, cleanup_బహుళకం) && !allow_ఖాళీలు_సందేశం) {
		rollback_సూచీ_files();
		fprintf(stderr, _("Aborting commit due to empty commit message.\n"));
		నిష్క్రమించు(1);
	}
	ఒకవేళ (template_untouched(&sb, template_file, cleanup_బహుళకం) && !allow_ఖాళీలు_సందేశం) {
		rollback_సూచీ_files();
		fprintf(stderr, _("Aborting commit; you did not edit the message.\n"));
		నిష్క్రమించు(1);
	}

	ఒకవేళ (fixup_సందేశం && starts_with(sb.buf, "amend! ") &&
	    !allow_ఖాళీలు_సందేశం) {
		నిర్మాణం strbuf body = STRBUF_INIT;
		కొలత_t len = commit_subject_నిడివి(sb.buf);
		strbuf_addstr(&body, sb.buf + len);
		ఒకవేళ (సందేశం_is_ఖాళీలు(&body, cleanup_బహుళకం)) {
			rollback_సూచీ_files();
			fprintf(stderr, _("Aborting commit due to empty commit message body.\n"));
			నిష్క్రమించు(1);
		}
		strbuf_release(&body);
	}

	ఒకవేళ (amend) {
		స్థిరరాశి అక్షరం *exclude_gpgsig[3] = { "gpgsig", "gpgsig-sha256", ఖాళీ };
		extra = read_commit_extra_శీర్షికలు(current_తల, exclude_gpgsig);
	} లేకపోతే {
		నిర్మాణం commit_extra_శీర్షిక **తోక = &extra;
		append_విలీనం_ట్యాగ్_శీర్షికలు(parents, &తోక);
	}

	ఒకవేళ (commit_వృక్షం_extended(sb.buf, sb.len, &the_repository->సూచీ->cache_వృక్షం->oid,
				 parents, &oid, author_ident.buf, ఖాళీ,
				 sign_commit, extra)) {
		rollback_సూచీ_files();
		die(_("failed to write commit object"));
	}

	ఒకవేళ (update_తల_with_reflog(current_తల, &oid, reflog_సందేశం, &sb,
				    &err)) {
		rollback_సూచీ_files();
		die("%s", err.buf);
	}

	sequencer_post_commit_cleanup(the_repository, 0);
	unlink(git_మార్గం_విలీనం_తల(the_repository));
	unlink(git_మార్గం_విలీనం_సందేశం(the_repository));
	unlink(git_మార్గం_విలీనం_బహుళకం(the_repository));
	unlink(git_మార్గం_squash_సందేశం(the_repository));

	ఒకవేళ (commit_సూచీ_files())
		die(_("repository has been updated, but unable to write\n"
		      "new index file. Check that disk is not full and quota is\n"
		      "not exceeded, and then \"git restore --staged :/\" to recover."));

	git_test_write_commit_లేఖాచిత్రం_or_die(the_repository);

	repo_rerere(the_repository, 0);
	నిరంతరం_auto_maintenance(the_repository, quiet);
	నిరంతరం_commit_hook(use_editor, repo_get_సూచీ_file(the_repository),
			ఖాళీ, "post-commit", ఖాళీ);
	ఒకవేళ (amend && !no_post_rewrite) {
		commit_post_rewrite(the_repository, current_తల, &oid);
	}
	ఒకవేళ (!quiet) {
		గుర్తులేని పూర్ణాంకం flags = 0;

		ఒకవేళ (!current_తల)
			flags |= SUMMARY_INITIAL_COMMIT;
		ఒకవేళ (author_date_is_interesting())
			flags |= SUMMARY_SHOW_AUTHOR_DATE;
		print_commit_summary(the_repository, ఉపసర్గ,
				     &oid, flags);
	}

	apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
			    ఖాళీ, ఖాళీ, ఖాళీ, ఖాళీ);

cleanup:
	free_commit_extra_శీర్షికలు(extra);
	commit_list_free(parents);
	strbuf_release(&author_ident);
	strbuf_release(&err);
	strbuf_release(&sb);
	విడుదల(logfile);
	విడుదల(template_file);
	తిరిగి ret;
}
