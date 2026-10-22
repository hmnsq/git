/*
 * "git reset" builtin command
 *
 * Copyright (c) 2007 Carlos Rica
 *
 * Based on git-reset.sh, which is
 *
 * Copyright (c) 2005, 2006 Linus Torvalds and Junio C Hamano
 */

#నిర్వచించు USE_THE_REPOSITORY_VARIABLE

#అన్తర్భావ "builtin.h"
#అన్తర్భావ "advice.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "hash.h"
#అన్తర్భావ "hex.h"
#అన్తర్భావ "lockfile.h"
#అన్తర్భావ "object.h"
#అన్తర్భావ "pretty.h"
#అన్తర్భావ "refs.h"
#అన్తర్భావ "diff.h"
#అన్తర్భావ "diffcore.h"
#అన్తర్భావ "tree.h"
#అన్తర్భావ "branch.h"
#అన్తర్భావ "object-name.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "path.h"
#అన్తర్భావ "repository.h"
#అన్తర్భావ "unpack-trees.h"
#అన్తర్భావ "cache-tree.h"
#అన్తర్భావ "setup.h"
#అన్తర్భావ "sparse-index.h"
#అన్తర్భావ "submodule.h"
#అన్తర్భావ "trace.h"
#అన్తర్భావ "trace2.h"
#అన్తర్భావ "dir.h"
#అన్తర్భావ "add-interactive.h"

#నిర్వచించు REFRESH_సూచీ_DELAY_WARNING_లోపల_MS (2 * 1000)

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి git_reset_usage[] = {
	N_("git reset [--mixed | --soft | --hard | --merge | --keep] [-q] [<commit>]"),
	N_("git reset [-q] [<tree-ish>] [--] <pathspec>..."),
	N_("git reset [-q] [--pathspec-from-file [--pathspec-file-nul]] [<tree-ish>]"),
	N_("git reset --patch [<tree-ish>] [--] [<pathspec>...]"),
	ఖాళీ
};

గణన reset_type { MIXED, SOFT, HARD, విలీనం_2, KEEP, NONE };
స్థిర స్థిరరాశి అక్షరం *reset_type_పేర్లు[] = {
	N_("mixed"), N_("soft"), N_("hard"), N_("merge"), N_("keep"), ఖాళీ
};

స్థిర అంతఃవరుస పూర్ణాంకం is_విలీనం(శూన్య)
{
	తిరిగి !access(git_మార్గం_విలీనం_తల(the_repository), F_OK);
}

స్థిర పూర్ణాంకం reset_సూచీ(స్థిరరాశి అక్షరం *ref, స్థిరరాశి నిర్మాణం object_id *oid, పూర్ణాంకం reset_type, పూర్ణాంకం quiet)
{
	పూర్ణాంకం i, nr = 0;
	నిర్మాణం వృక్షం_desc desc[2];
	నిర్మాణం వృక్షం *వృక్షం;
	నిర్మాణం unpack_trees_options opts;
	పూర్ణాంకం ret = -1;

	memset(&opts, 0, పరిమాణం(opts));
	opts.తల_idx = 1;
	opts.src_సూచీ = the_repository->సూచీ;
	opts.dst_సూచీ = the_repository->సూచీ;
	opts.fn = oneway_విలీనం;
	opts.విలీనం = 1;
	init_checkout_metadata(&opts.meta, ref, oid, ఖాళీ);
	ఒకవేళ (!quiet)
		opts.verbose_update = 1;
	ఎంపిక (reset_type) {
	స్థితి KEEP:
	స్థితి విలీనం_2:
		opts.update = 1;
		opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
		ఆపు;
	స్థితి HARD:
		opts.update = 1;
		opts.reset = UNPACK_RESET_OVERWRITE_UNTRACKED;
		opts.skip_cache_వృక్షం_update = 1;
		ఆపు;
	స్థితి MIXED:
		opts.reset = UNPACK_RESET_PROTECT_UNTRACKED;
		opts.skip_cache_వృక్షం_update = 1;
		/* but opts.update=0, so working tree not updated */
		ఆపు;
	అప్రమేయ:
		BUG("invalid reset_type passed to reset_index");
	}

	repo_read_సూచీ_unmerged(the_repository);

	ఒకవేళ (reset_type == KEEP) {
		నిర్మాణం object_id తల_oid;
		ఒకవేళ (repo_get_oid(the_repository, "HEAD", &తల_oid))
			తిరిగి error(_("You do not have a valid HEAD."));
		ఒకవేళ (!fill_వృక్షం_descriptor(the_repository, desc + nr, &తల_oid))
			తిరిగి error(_("Failed to find tree of HEAD."));
		nr++;
		opts.fn = twoway_విలీనం;
	}

	ఒకవేళ (!fill_వృక్షం_descriptor(the_repository, desc + nr, oid)) {
		error(_("Failed to find tree of %s."), oid_గమ్యం_hex(oid));
		వెళ్ళు బహిర్గతం;
	}
	nr++;

	ఒకవేళ (unpack_trees(nr, desc, &opts))
		వెళ్ళు బహిర్గతం;

	ఒకవేళ (reset_type == MIXED || reset_type == HARD) {
		వృక్షం = repo_parse_వృక్షం_పరోక్ష(the_repository, oid);
		ఒకవేళ (!వృక్షం) {
			error(_("unable to read tree (%s)"), oid_గమ్యం_hex(oid));
			వెళ్ళు బహిర్గతం;
		}
		ప్రధానసంఖ్య_cache_వృక్షం(the_repository, the_repository->సూచీ, వృక్షం);
	}

	ret = 0;

బహిర్గతం:
	కొరకు (i = 0; i < nr; i++)
		విడుదల((శూన్య *)desc[i].బఫరు);
	తిరిగి ret;
}

స్థిర శూన్య print_new_తల_పంక్తి(నిర్మాణం commit *commit)
{
	నిర్మాణం strbuf buf = STRBUF_INIT;

	రాయి(_("HEAD is now at %s"),
		repo_కనుగొను_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV));

	pp_commit_easy(CMIT_FMT_ONELINE, commit, &buf);
	ఒకవేళ (buf.len > 0)
		రాయి(" %s", buf.buf);
	అక్షరం_రాయి('\n');
	strbuf_release(&buf);
}

స్థిర శూన్య update_సూచీ_from_diff(నిర్మాణం diff_వరుస_struct *q,
				   నిర్మాణం diff_options *opt వాడని,
				   శూన్య *దత్తాంశం)
{
	పూర్ణాంకం i;
	పూర్ణాంకం intent_గమ్యం_కూడు = *(పూర్ణాంకం *)దత్తాంశం;

	కొరకు (i = 0; i < q->nr; i++) {
		పూర్ణాంకం pos;
		నిర్మాణం diff_filespec *one = q->వరుస[i]->one;
		పూర్ణాంకం is_లోపల_reset_వృక్షం = one->బహుళకం && !is_null_oid(&one->oid);
		నిర్మాణం cache_నమోదు *ce;

		ఒకవేళ (!is_లోపల_reset_వృక్షం && !intent_గమ్యం_కూడు) {
			remove_file_from_సూచీ(the_repository->సూచీ, one->మార్గం);
			కొనసాగు;
		}

		ce = make_cache_నమోదు(the_repository->సూచీ, one->బహుళకం, &one->oid, one->మార్గం,
				      0, 0);

		/*
		 * If the file 1) corresponds to an existing index entry with
		 * skip-worktree set, or 2) does not exist in the index but is
		 * outside the sparse checkout definition, add a skip-worktree bit
		 * to the new index entry. Note that a sparse index will be expanded
		 * if this entry is outside the sparse cone - this is necessary
		 * to properly construct the reset sparse directory.
		 */
		pos = సూచీ_name_pos(the_repository->సూచీ, one->మార్గం, అక్షరశ్రేణి_పొడవు(one->మార్గం));
		ఒకవేళ ((pos >= 0 && ce_skip_worktree(the_repository->సూచీ->cache[pos])) ||
		    (pos < 0 && !మార్గం_లోపల_sparse_checkout(one->మార్గం, the_repository->సూచీ)))
			ce->ce_flags |= CE_SKIP_WORKTREE;

		ఒకవేళ (!ce)
			die(_("make_cache_entry failed for path '%s'"),
			    one->మార్గం);
		ఒకవేళ (!is_లోపల_reset_వృక్షం) {
			ce->ce_flags |= CE_INTENT_గమ్యం_కూడు;
			సమితి_object_name_for_intent_గమ్యం_కూడు_నమోదు(ce);
		}
		కూడు_సూచీ_నమోదు(the_repository->సూచీ, ce,
				కూడు_CACHE_OK_గమ్యం_కూడు | కూడు_CACHE_OK_గమ్యం_REPLACE);
	}
}

స్థిర పూర్ణాంకం read_from_వృక్షం(స్థిరరాశి నిర్మాణం pathspec *pathspec,
			  నిర్మాణం object_id *వృక్షం_oid,
			  పూర్ణాంకం intent_గమ్యం_కూడు)
{
	నిర్మాణం diff_options opt;

	memset(&opt, 0, పరిమాణం(opt));
	copy_pathspec(&opt.pathspec, pathspec);
	opt.output_format = DIFF_FORMAT_CALLBACK;
	opt.format_callback = update_సూచీ_from_diff;
	opt.format_callback_దత్తాంశం = &intent_గమ్యం_కూడు;
	opt.flags.override_submodule_config = 1;
	opt.flags.recursive = 1;
	opt.repo = the_repository;
	opt.change = diff_change;
	opt.కూడు_remove = diff_addremove;

	ఒకవేళ (pathspec->nr && pathspec_needs_expanded_సూచీ(the_repository->సూచీ, pathspec))
		ensure_నిండిన_సూచీ(the_repository->సూచీ);

	ఒకవేళ (do_diff_cache(వృక్షం_oid, &opt))
		తిరిగి 1;
	diffcore_std(&opt);
	diff_flush(&opt);

	తిరిగి 0;
}

స్థిర శూన్య సమితి_reflog_సందేశం(నిర్మాణం strbuf *sb, స్థిరరాశి అక్షరం *action,
			       స్థిరరాశి అక్షరం *rev)
{
	స్థిరరాశి అక్షరం *rla = getenv("GIT_REFLOG_ACTION");

	strbuf_reset(sb);
	ఒకవేళ (rla)
		strbuf_addf(sb, "%s: %s", rla, action);
	లేకపోతే ఒకవేళ (rev)
		strbuf_addf(sb, "reset: moving to %s", rev);
	లేకపోతే
		strbuf_addf(sb, "reset: %s", action);
}

స్థిర శూన్య die_if_unmerged_cache(పూర్ణాంకం reset_type)
{
	ఒకవేళ (is_విలీనం() || unmerged_సూచీ(the_repository->సూచీ))
		die(_("Cannot do a %s reset in the middle of a merge."),
		    _(reset_type_పేర్లు[reset_type]));

}

స్థిర శూన్య parse_వాదనలు(నిర్మాణం pathspec *pathspec,
		       స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
		       పూర్ణాంకం patch_బహుళకం,
		       స్థిరరాశి అక్షరం **rev_ret)
{
	స్థిరరాశి అక్షరం *rev = "HEAD";
	నిర్మాణం object_id వాడని_2;
	/*
	 * Possible arguments are:
	 *
	 * git reset [-opts] [<rev>]
	 * git reset [-opts] <tree> [<paths>...]
	 * git reset [-opts] <tree> -- [<paths>...]
	 * git reset [-opts] -- [<paths>...]
	 * git reset [-opts] <paths>...
	 *
	 * At this point, argv points immediately after [-opts].
	 */

	ఒకవేళ (argv[0]) {
		ఒకవేళ (!strcmp(argv[0], "--")) {
			argv++; /* reset to HEAD, possibly with paths */
		} లేకపోతే ఒకవేళ (argv[1] && !strcmp(argv[1], "--")) {
			rev = argv[0];
			argv += 2;
		}
		/*
		 * Otherwise, argv[0] could be either <rev> or <paths> and
		 * has to be unambiguous. If there is a single argument, it
		 * can not be a tree
		 */
		లేకపోతే ఒకవేళ ((!argv[1] && !repo_get_oid_committish(the_repository, argv[0], &వాడని_2)) ||
			 (argv[1] && !repo_get_oid_treeish(the_repository, argv[0], &వాడని_2))) {
			/*
			 * Ok, argv[0] looks like a commit/tree; it should not
			 * be a filename.
			 */
			verify_non_filename(the_repository, ఉపసర్గ, argv[0]);
			rev = *argv++;
		} లేకపోతే {
			/* Otherwise we treat this as a filename */
			verify_filename(the_repository, ఉపసర్గ, argv[0], 1);
		}
	}

	/* treat '@' as a shortcut for 'HEAD' */
	*rev_ret = !strcmp("@", rev) ? "HEAD" : rev;

	parse_pathspec(pathspec, 0,
		       PATHSPEC_PREFER_నిండిన |
		       (patch_బహుళకం ? PATHSPEC_ఉపసర్గ_ORIGIN : 0),
		       ఉపసర్గ, argv);
}

స్థిర పూర్ణాంకం reset_refs(స్థిరరాశి అక్షరం *rev, స్థిరరాశి నిర్మాణం object_id *oid)
{
	పూర్ణాంకం update_ref_స్థితి;
	నిర్మాణం strbuf సందేశం = STRBUF_INIT;
	నిర్మాణం object_id *orig = ఖాళీ, oid_orig,
		*old_orig = ఖాళీ, oid_old_orig;

	ఒకవేళ (!repo_get_oid(the_repository, "ORIG_HEAD", &oid_old_orig))
		old_orig = &oid_old_orig;
	ఒకవేళ (!repo_get_oid(the_repository, "HEAD", &oid_orig)) {
		orig = &oid_orig;
		సమితి_reflog_సందేశం(&సందేశం, "updating ORIG_HEAD", ఖాళీ);
		refs_update_ref(get_main_ref_store(the_repository), సందేశం.buf,
				"ORIG_HEAD", orig, old_orig, 0,
				UPDATE_REFS_సందేశం_ON_ERR);
	} లేకపోతే ఒకవేళ (old_orig)
		refs_delete_ref(get_main_ref_store(the_repository), ఖాళీ,
				"ORIG_HEAD", old_orig, 0);
	సమితి_reflog_సందేశం(&సందేశం, "updating HEAD", rev);
	update_ref_స్థితి = refs_update_ref(get_main_ref_store(the_repository),
					    సందేశం.buf, "HEAD", oid, orig, 0,
					    UPDATE_REFS_సందేశం_ON_ERR);
	strbuf_release(&సందేశం);
	తిరిగి update_ref_స్థితి;
}

స్థిర పూర్ణాంకం git_reset_config(స్థిరరాశి అక్షరం *var, స్థిరరాశి అక్షరం *విలువ,
			    స్థిరరాశి నిర్మాణం config_context *ctx, శూన్య *cb)
{
	ఒకవేళ (!strcmp(var, "submodule.recurse"))
		తిరిగి git_default_submodule_config(var, విలువ, cb);

	తిరిగి git_default_config(var, విలువ, ctx, cb);
}

పూర్ణాంకం cmd_reset(పూర్ణాంకం argc,
	      స్థిరరాశి అక్షరం **argv,
	      స్థిరరాశి అక్షరం *ఉపసర్గ,
	      నిర్మాణం repository *repo వాడని)
{
	పూర్ణాంకం reset_type = NONE, update_ref_స్థితి = 0, quiet = 0;
	పూర్ణాంకం no_refresh = 0;
	పూర్ణాంకం patch_బహుళకం = 0, pathspec_file_nul = 0, unborn;
	స్థిరరాశి అక్షరం *rev;
	అక్షరం *pathspec_from_file = ఖాళీ;
	నిర్మాణం object_id oid;
	నిర్మాణం pathspec pathspec;
	పూర్ణాంకం intent_గమ్యం_కూడు = 0;
	నిర్మాణం interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	స్థిరరాశి నిర్మాణం option options[] = {
		OPT__QUIET(&quiet, N_("be quiet, only report errors")),
		OPT_BOOL(0, "no-refresh", &no_refresh,
				N_("skip refreshing the index after reset")),
		OPT_సమితి_INT_F(0, "mixed", &reset_type,
			      N_("reset HEAD and index"),
			      MIXED, PARSE_OPT_NONEG),
		OPT_సమితి_INT_F(0, "soft", &reset_type,
			      N_("reset only HEAD"),
			      SOFT, PARSE_OPT_NONEG),
		OPT_సమితి_INT_F(0, "hard", &reset_type,
			      N_("reset HEAD, index and working tree"),
			      HARD, PARSE_OPT_NONEG),
		OPT_సమితి_INT_F(0, "merge", &reset_type,
			      N_("reset HEAD, index and working tree"),
			      విలీనం_2, PARSE_OPT_NONEG),
		OPT_సమితి_INT_F(0, "keep", &reset_type,
			      N_("reset HEAD but keep local changes"),
			      KEEP, PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "recurse-submodules", ఖాళీ,
			       "reset", "control recursive updating of submodules",
			       PARSE_OPT_OPTARG,
			       option_parse_recurse_submodules_worktree_updater),
		OPT_BOOL('p', "patch", &patch_బహుళకం, N_("select hunks interactively")),
		OPT_BOOL(0, "auto-advance", &interactive_opts.auto_advance,
			 N_("auto advance to the next file when selecting hunks interactively")),
		OPT_DIFF_UNIFIED(&interactive_opts.context),
		OPT_DIFF_INTERHUNK_CONTEXT(&interactive_opts.interhunkcontext),
		OPT_BOOL('N', "intent-to-add", &intent_గమ్యం_కూడు,
				N_("record only the fact that removed paths will be added later")),
		OPT_PATHSPEC_FROM_FILE(&pathspec_from_file),
		OPT_PATHSPEC_FILE_NUL(&pathspec_file_nul),
		OPT_చివర()
	};

	repo_config(the_repository, git_reset_config, ఖాళీ);

	argc = parse_options(argc, argv, ఉపసర్గ, options, git_reset_usage,
						PARSE_OPT_KEEP_DASHDASH);
	parse_వాదనలు(&pathspec, argv, ఉపసర్గ, patch_బహుళకం, &rev);

	ఒకవేళ (pathspec_from_file) {
		ఒకవేళ (patch_బహుళకం)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--patch");

		ఒకవేళ (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_నిండిన,
				    ఉపసర్గ, pathspec_from_file, pathspec_file_nul);
	} లేకపోతే ఒకవేళ (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	unborn = !strcmp(rev, "HEAD") && repo_get_oid(the_repository, "HEAD",
						      &oid);
	ఒకవేళ (unborn) {
		/* reset on unborn branch: treat as reset to empty tree */
		oidcpy(&oid, the_hash_algo->ఖాళీలు_వృక్షం);
	} లేకపోతే ఒకవేళ (!pathspec.nr && !patch_బహుళకం) {
		నిర్మాణం commit *commit;
		ఒకవేళ (repo_get_oid_committish(the_repository, rev, &oid))
			die(_("Failed to resolve '%s' as a valid revision."), rev);
		commit = lookup_commit_reference(the_repository, &oid);
		ఒకవేళ (!commit)
			die(_("Could not parse object '%s'."), rev);
		oidcpy(&oid, &commit->object.oid);
	} లేకపోతే {
		నిర్మాణం వృక్షం *వృక్షం;
		ఒకవేళ (repo_get_oid_treeish(the_repository, rev, &oid))
			die(_("Failed to resolve '%s' as a valid tree."), rev);
		వృక్షం = repo_parse_వృక్షం_పరోక్ష(the_repository, &oid);
		ఒకవేళ (!వృక్షం)
			die(_("Could not parse object '%s'."), rev);
		oidcpy(&oid, &వృక్షం->object.oid);
	}

	ఒకవేళ (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	ఒకవేళ (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_నిండిన_సూచీ = 0;

	ఒకవేళ (patch_బహుళకం) {
		ఒకవేళ (reset_type != NONE)
			die(_("options '%s' and '%s' cannot be used together"), "--patch", "--{hard,mixed,soft}");
		trace2_cmd_బహుళకం("patch-interactive");
		update_ref_స్థితి = !!నిరంతరం_కూడు_p(the_repository, కూడు_P_RESET,
						&interactive_opts, rev, &pathspec, 0);
		వెళ్ళు cleanup;
	} లేకపోతే {
		ఒకవేళ (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--patch");
		ఒకవేళ (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--patch");
		ఒకవేళ (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--patch");
	}

	/* git reset tree [--] paths... can be used to
	 * load chosen paths from the tree into the index without
	 * affecting the working tree nor HEAD. */
	ఒకవేళ (pathspec.nr) {
		ఒకవేళ (reset_type == MIXED)
			warning(_("--mixed with paths is deprecated; use 'git reset -- <paths>' instead."));
		లేకపోతే ఒకవేళ (reset_type != NONE)
			die(_("Cannot do %s reset with paths."),
					_(reset_type_పేర్లు[reset_type]));
	}
	ఒకవేళ (reset_type == NONE)
		reset_type = MIXED; /* by default */

	ఒకవేళ (pathspec.nr)
		trace2_cmd_బహుళకం("path");
	లేకపోతే
		trace2_cmd_బహుళకం(reset_type_పేర్లు[reset_type]);

	ఒకవేళ (reset_type != SOFT && (reset_type != MIXED || repo_get_work_వృక్షం(the_repository)))
		అమరిక_work_వృక్షం(the_repository);

	ఒకవేళ (reset_type == MIXED && is_bare_repository(the_repository))
		die(_("%s reset is not allowed in a bare repository"),
		    _(reset_type_పేర్లు[reset_type]));

	ఒకవేళ (intent_గమ్యం_కూడు && reset_type != MIXED)
		die(_("the option '%s' requires '%s'"), "-N", "--mixed");

	ఒకవేళ (repo_read_సూచీ(the_repository) < 0)
		die(_("index file corrupt"));

	/* Soft reset does not touch the index file nor the working tree
	 * at all, but requires them in a good order.  Other resets reset
	 * the index file to the tree object we are switching to. */
	ఒకవేళ (reset_type == SOFT || reset_type == KEEP)
		die_if_unmerged_cache(reset_type);

	ఒకవేళ (reset_type != SOFT) {
		నిర్మాణం తాళం_file తాళం = తాళం_INIT;
		repo_hold_locked_సూచీ(the_repository, &తాళం,
				       తాళం_DIE_ON_ERROR);
		ఒకవేళ (reset_type == MIXED) {
			పూర్ణాంకం flags = quiet ? REFRESH_QUIET : REFRESH_లోపల_PORCELAIN;
			ఒకవేళ (read_from_వృక్షం(&pathspec, &oid, intent_గమ్యం_కూడు)) {
				update_ref_స్థితి = 1;
				వెళ్ళు cleanup;
			}
			the_repository->సూచీ->updated_skipworktree = 1;
			ఒకవేళ (!no_refresh && repo_get_work_వృక్షం(the_repository)) {
				uint64_t t_begin, t_delta_లోపల_ms;

				t_begin = getnanotime();
				refresh_సూచీ(the_repository->సూచీ, flags, ఖాళీ, ఖాళీ,
					      _("Unstaged changes after reset:"));
				t_delta_లోపల_ms = (getnanotime() - t_begin) / 1000000;
				ఒకవేళ (!quiet && advice_enabled(ADVICE_RESET_NO_REFRESH_WARNING) && t_delta_లోపల_ms > REFRESH_సూచీ_DELAY_WARNING_లోపల_MS) {
					advise(_("It took %.2f seconds to refresh the index after reset.  You can use\n"
						 "'--no-refresh' to avoid this."), t_delta_లోపల_ms / 1000.0);
				}
			}
		} లేకపోతే {
			నిర్మాణం object_id dummy;
			అక్షరం *ref = ఖాళీ;
			పూర్ణాంకం err;

			repo_dwim_ref(the_repository, rev, అక్షరశ్రేణి_పొడవు(rev),
				      &dummy, &ref, 0);
			ఒకవేళ (ref && !starts_with(ref, "refs/"))
				FREE_AND_NULL(ref);

			err = reset_సూచీ(ref, &oid, reset_type, quiet);
			ఒకవేళ (reset_type == KEEP && !err)
				err = reset_సూచీ(ref, &oid, MIXED, quiet);
			ఒకవేళ (err)
				die(_("Could not reset index file to revision '%s'."), rev);
			విడుదల(ref);
		}

		ఒకవేళ (write_locked_సూచీ(the_repository->సూచీ, &తాళం, COMMIT_తాళం))
			die(_("Could not write new index file."));
	}

	ఒకవేళ (!pathspec.nr && !unborn) {
		/* Any resets without paths update HEAD to the head being
		 * switched to, saving the previous head in ORIG_HEAD before. */
		update_ref_స్థితి = reset_refs(rev, &oid);

		ఒకవేళ (reset_type == HARD && !update_ref_స్థితి && !quiet)
			print_new_తల_పంక్తి(lookup_commit_reference(the_repository, &oid));
	}
	ఒకవేళ (!pathspec.nr)
		remove_branch_state(the_repository, 0);

	discard_సూచీ(the_repository->సూచీ);

cleanup:
	clear_pathspec(&pathspec);
	విడుదల(pathspec_from_file);
	తిరిగి update_ref_స్థితి;
}
