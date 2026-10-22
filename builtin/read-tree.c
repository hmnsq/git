/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#నిర్వచించు USE_THE_REPOSITORY_VARIABLE
#అన్తర్భావ "builtin.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "hex.h"
#అన్తర్భావ "lockfile.h"
#అన్తర్భావ "object.h"
#అన్తర్భావ "object-name.h"
#అన్తర్భావ "tree.h"
#అన్తర్భావ "tree-walk.h"
#అన్తర్భావ "cache-tree.h"
#అన్తర్భావ "unpack-trees.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "resolve-undo.h"
#అన్తర్భావ "setup.h"
#అన్తర్భావ "sparse-index.h"
#అన్తర్భావ "submodule.h"

స్థిర పూర్ణాంకం nr_trees;
స్థిర పూర్ణాంకం read_ఖాళీలు;
స్థిర నిర్మాణం వృక్షం *trees[MAX_UNPACK_TREES];

స్థిర పూర్ణాంకం list_వృక్షం(నిర్మాణం object_id *oid)
{
	నిర్మాణం వృక్షం *వృక్షం;

	ఒకవేళ (nr_trees >= MAX_UNPACK_TREES)
		die("I cannot read more than %d trees", MAX_UNPACK_TREES);
	వృక్షం = repo_parse_వృక్షం_పరోక్ష(the_repository, oid);
	ఒకవేళ (!వృక్షం)
		తిరిగి -1;
	trees[nr_trees++] = వృక్షం;
	తిరిగి 0;
}

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి read_వృక్షం_usage[] = {
	N_("git read-tree [(-m [--trivial] [--aggressive] | --reset | --prefix=<prefix>)\n"
	   "              [-u | -i]] [--index-output=<file>] [--no-sparse-checkout]\n"
	   "              (--empty | <tree-ish1> [<tree-ish2> [<tree-ish3>]])"),
	ఖాళీ
};

స్థిర పూర్ణాంకం సూచీ_output_cb(స్థిరరాశి నిర్మాణం option *opt వాడని, స్థిరరాశి అక్షరం *arg,
				 పూర్ణాంకం unset)
{
	BUG_ON_OPT_NEG(unset);
	సమితి_alternate_సూచీ_output(arg);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం exclude_చొప్పున_directory_cb(స్థిరరాశి నిర్మాణం option *opt, స్థిరరాశి అక్షరం *arg,
				    పూర్ణాంకం unset)
{
	నిర్మాణం unpack_trees_options *opts;

	BUG_ON_OPT_NEG(unset);

	opts = (నిర్మాణం unpack_trees_options *)opt->విలువ;

	ఒకవేళ (!opts->update)
		die("--exclude-per-directory is meaningless unless -u");
	ఒకవేళ (strcmp(arg, ".gitignore"))
		die("--exclude-per-directory argument must be .gitignore");
	తిరిగి 0;
}

స్థిర శూన్య debug_stage(స్థిరరాశి అక్షరం *label, స్థిరరాశి నిర్మాణం cache_నమోదు *ce,
			నిర్మాణం unpack_trees_options *o)
{
	రాయి("%s ", label);
	ఒకవేళ (!ce)
		రాయి("(missing)\n");
	లేకపోతే ఒకవేళ (ce == o->df_conflict_నమోదు)
		రాయి("(conflict)\n");
	లేకపోతే
		రాయి("%06o #%d %s %.8s\n",
		       ce->ce_బహుళకం, ce_stage(ce), ce->name,
		       oid_గమ్యం_hex(&ce->oid));
}

స్థిర పూర్ణాంకం debug_విలీనం(స్థిరరాశి నిర్మాణం cache_నమోదు * స్థిరరాశి *stages,
		       నిర్మాణం unpack_trees_options *o)
{
	పూర్ణాంకం i;

	రాయి("* %d-way merge\n", o->అంతర్గత.విలీనం_కొలత);
	debug_stage("index", stages[0], o);
	కొరకు (i = 1; i <= o->అంతర్గత.విలీనం_కొలత; i++) {
		అక్షరం buf[24];
		xsnprintf(buf, పరిమాణం(buf), "ent#%d", i);
		debug_stage(buf, stages[i], o);
	}
	తిరిగి 0;
}

స్థిర పూర్ణాంకం git_read_వృక్షం_config(స్థిరరాశి అక్షరం *var, స్థిరరాశి అక్షరం *విలువ,
				స్థిరరాశి నిర్మాణం config_context *ctx, శూన్య *cb)
{
	ఒకవేళ (!strcmp(var, "submodule.recurse"))
		తిరిగి git_default_submodule_config(var, విలువ, cb);

	తిరిగి git_default_config(var, విలువ, ctx, cb);
}

పూర్ణాంకం cmd_read_వృక్షం(పూర్ణాంకం argc,
		  స్థిరరాశి అక్షరం **argv,
		  స్థిరరాశి అక్షరం *cmd_ఉపసర్గ,
		  నిర్మాణం repository *repo వాడని)
{
	పూర్ణాంకం i, stage = 0;
	నిర్మాణం object_id oid;
	నిర్మాణం వృక్షం_desc t[MAX_UNPACK_TREES];
	నిర్మాణం unpack_trees_options opts;
	పూర్ణాంకం ఉపసర్గ_సమితి = 0;
	నిర్మాణం తాళం_file తాళం_file = తాళం_INIT;
	స్థిరరాశి నిర్మాణం option read_వృక్షం_options[] = {
		OPT_SUPER_ఉపసర్గ(&opts.super_ఉపసర్గ),
		OPT_CALLBACK_F(0, "index-output", ఖాళీ, N_("file"),
		  N_("write resulting index to <file>"),
		  PARSE_OPT_NONEG, సూచీ_output_cb),
		OPT_BOOL(0, "empty", &read_ఖాళీలు,
			    N_("only empty the index")),
		OPT__VERBOSE(&opts.verbose_update, N_("be verbose")),
		OPT_సమూహం(N_("Merging")),
		OPT_BOOL('m', ఖాళీ, &opts.విలీనం,
			 N_("perform a merge in addition to a read")),
		OPT_BOOL(0, "trivial", &opts.trivial_merges_only,
			 N_("3-way merge if no file level merging required")),
		OPT_BOOL(0, "aggressive", &opts.aggressive,
			 N_("3-way merge in presence of adds and removes")),
		OPT_BOOL(0, "reset", &opts.reset,
			 N_("same as -m, but discard unmerged entries")),
		{
			.type = OPTION_STRING,
			.long_name = "prefix",
			.విలువ = &opts.ఉపసర్గ,
			.argh = N_("<subdirectory>/"),
			.help = N_("read the tree into the index under <subdirectory>/"),
			.flags = PARSE_OPT_NONEG,
		},
		OPT_BOOL('u', ఖాళీ, &opts.update,
			 N_("update working tree with merge result")),
		OPT_CALLBACK_F(0, "exclude-per-directory", &opts,
		  N_("gitignore"),
		  N_("allow explicitly ignored files to be overwritten"),
		  PARSE_OPT_NONEG, exclude_చొప్పున_directory_cb),
		OPT_BOOL('i', ఖాళీ, &opts.సూచీ_only,
			 N_("don't check the working tree after merging")),
		OPT_DRY_నిరంతరం(&opts.dry_నిరంతరం, N_("don't update the index or the work tree")),
		OPT_BOOL(0, "no-sparse-checkout", &opts.skip_sparse_checkout,
			 N_("skip applying sparse checkout filter")),
		OPT_BOOL(0, "debug-unpack", &opts.అంతర్గత.debug_unpack,
			 N_("debug unpack-trees")),
		OPT_CALLBACK_F(0, "recurse-submodules", ఖాళీ,
			    "checkout", "control recursive updating of submodules",
			    PARSE_OPT_OPTARG, option_parse_recurse_submodules_worktree_updater),
		OPT__QUIET(&opts.quiet, N_("suppress feedback messages")),
		OPT_చివర()
	};

	memset(&opts, 0, పరిమాణం(opts));
	opts.తల_idx = -1;
	opts.src_సూచీ = the_repository->సూచీ;
	opts.dst_సూచీ = the_repository->సూచీ;

	repo_config(the_repository, git_read_వృక్షం_config, ఖాళీ);

	argc = parse_options(argc, argv, cmd_ఉపసర్గ, read_వృక్షం_options,
			     read_వృక్షం_usage, 0);

	ఉపసర్గ_సమితి = opts.ఉపసర్గ ? 1 : 0;
	ఒకవేళ (1 < opts.విలీనం + opts.reset + ఉపసర్గ_సమితి)
		die("Which one? -m, --reset, or --prefix?");

	/* Prefix should not start with a directory separator */
	ఒకవేళ (opts.ఉపసర్గ && opts.ఉపసర్గ[0] == '/')
		die("Invalid prefix, prefix cannot start with '/'");

	ఒకవేళ (opts.reset)
		opts.reset = UNPACK_RESET_OVERWRITE_UNTRACKED;

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_నిండిన_సూచీ = 0;

	repo_hold_locked_సూచీ(the_repository, &తాళం_file, తాళం_DIE_ON_ERROR);

	/*
	 * NEEDSWORK
	 *
	 * The old index should be read anyway even if we're going to
	 * destroy all index entries because we still need to preserve
	 * certain information such as index version or split-index
	 * mode.
	 */

	ఒకవేళ (opts.reset || opts.విలీనం || opts.ఉపసర్గ) {
		ఒకవేళ (repo_read_సూచీ_unmerged(the_repository) && (opts.ఉపసర్గ || opts.విలీనం))
			die(_("You need to resolve your current index first"));
		stage = opts.విలీనం = 1;
	}
	resolve_undo_clear_సూచీ(the_repository->సూచీ);

	కొరకు (i = 0; i < argc; i++) {
		స్థిరరాశి అక్షరం *arg = argv[i];

		ఒకవేళ (repo_get_oid(the_repository, arg, &oid))
			die("Not a valid object name %s", arg);
		ఒకవేళ (list_వృక్షం(&oid) < 0)
			die("failed to unpack tree object %s", arg);
		stage++;
	}
	ఒకవేళ (!nr_trees && !read_ఖాళీలు && !opts.విలీనం)
		warning("read-tree: emptying the index with no arguments is deprecated; use --empty");
	లేకపోతే ఒకవేళ (nr_trees > 0 && read_ఖాళీలు)
		die("passing trees as arguments contradicts --empty");

	ఒకవేళ (1 < opts.సూచీ_only + opts.update)
		die("-u and -i at the same time makes no sense");
	ఒకవేళ ((opts.update || opts.సూచీ_only) && !opts.విలీనం)
		die("%s is meaningless without -m, --reset, or --prefix",
		    opts.update ? "-u" : "-i");
	ఒకవేళ (opts.update && !opts.reset)
		opts.preserve_ignored = 0;
	/* otherwise, opts.preserve_ignored is irrelevant */
	ఒకవేళ (opts.విలీనం && !opts.సూచీ_only)
		అమరిక_work_వృక్షం(the_repository);

	ఒకవేళ (opts.skip_sparse_checkout)
		ensure_నిండిన_సూచీ(the_repository->సూచీ);

	ఒకవేళ (opts.విలీనం) {
		ఎంపిక (stage - 1) {
		స్థితి 0:
			die("you must specify at least one tree to merge");
			ఆపు;
		స్థితి 1:
			opts.fn = opts.ఉపసర్గ ? bind_విలీనం : oneway_విలీనం;
			ఆపు;
		స్థితి 2:
			opts.fn = twoway_విలీనం;
			opts.initial_checkout = is_సూచీ_unborn(the_repository->సూచీ);
			ఆపు;
		స్థితి 3:
		అప్రమేయ:
			opts.fn = threeway_విలీనం;
			ఆపు;
		}

		ఒకవేళ (stage - 1 >= 3)
			opts.తల_idx = stage - 2;
		లేకపోతే
			opts.తల_idx = 1;
	}

	ఒకవేళ (opts.అంతర్గత.debug_unpack)
		opts.fn = debug_విలీనం;

	/* If we're going to prime_cache_tree later, skip cache tree update */
	ఒకవేళ (nr_trees == 1 && !opts.ఉపసర్గ)
		opts.skip_cache_వృక్షం_update = 1;

	cache_వృక్షం_free(&the_repository->సూచీ->cache_వృక్షం);
	కొరకు (i = 0; i < nr_trees; i++) {
		నిర్మాణం వృక్షం *వృక్షం = trees[i];
		ఒకవేళ (repo_parse_వృక్షం(the_repository, వృక్షం) < 0)
			తిరిగి 128;
		init_వృక్షం_desc(t+i, &వృక్షం->object.oid, వృక్షం->బఫరు, వృక్షం->కొలత);
	}
	ఒకవేళ (unpack_trees(nr_trees, t, &opts))
		తిరిగి 128;

	ఒకవేళ (opts.అంతర్గత.debug_unpack || opts.dry_నిరంతరం)
		తిరిగి 0; /* do not write the index out */

	/*
	 * When reading only one tree (either the most basic form,
	 * "-m ent" or "--reset ent" form), we can obtain a fully
	 * valid cache-tree because the index must match exactly
	 * what came from the tree.
	 */
	ఒకవేళ (nr_trees == 1 && !opts.ఉపసర్గ)
		ప్రధానసంఖ్య_cache_వృక్షం(the_repository,
				 the_repository->సూచీ,
				 trees[0]);

	ఒకవేళ (write_locked_సూచీ(the_repository->సూచీ, &తాళం_file, COMMIT_తాళం))
		die("unable to write new index file");
	తిరిగి 0;
}
