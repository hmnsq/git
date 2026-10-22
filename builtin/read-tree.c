/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE
#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "lockfile.h"
#ಅನ್ತರ್ಭಾವ "object.h"
#ಅನ್ತರ್ಭಾವ "object-name.h"
#ಅನ್ತರ್ಭಾವ "tree.h"
#ಅನ್ತರ್ಭಾವ "tree-walk.h"
#ಅನ್ತರ್ಭಾವ "cache-tree.h"
#ಅನ್ತರ್ಭಾವ "unpack-trees.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "resolve-undo.h"
#ಅನ್ತರ್ಭಾವ "setup.h"
#ಅನ್ತರ್ಭಾವ "sparse-index.h"
#ಅನ್ತರ್ಭಾವ "submodule.h"

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ nr_trees;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ read_ಖಾಲಿಗಳು;
ಸ್ಥಿರ ರಚನೆ ವೃಕ್ಷ *trees[MAX_UNPACK_TREES];

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ list_ವೃಕ್ಷ(ರಚನೆ object_id *oid)
{
	ರಚನೆ ವೃಕ್ಷ *ವೃಕ್ಷ;

	ಆದರೆ (nr_trees >= MAX_UNPACK_TREES)
		die("I cannot read more than %d trees", MAX_UNPACK_TREES);
	ವೃಕ್ಷ = repo_parse_ವೃಕ್ಷ_ಪರೋಕ್ಷ(the_repository, oid);
	ಆದರೆ (!ವೃಕ್ಷ)
		ಹಿಂತಿರುಗಿ -1;
	trees[nr_trees++] = ವೃಕ್ಷ;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ read_ವೃಕ್ಷ_usage[] = {
	N_("git read-tree [(-m [--trivial] [--aggressive] | --reset | --prefix=<prefix>)\n"
	   "              [-u | -i]] [--index-output=<file>] [--no-sparse-checkout]\n"
	   "              (--empty | <tree-ish1> [<tree-ish2> [<tree-ish3>]])"),
	ಖಾಲಿ
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ_output_cb(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt ಬಳಸದ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg,
				 ಪೂರ್ಣಾಂಕ unset)
{
	BUG_ON_OPT_NEG(unset);
	ಗಣ_alternate_ಸೂಚ್ಯಂಕ_output(arg);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ exclude_ಪ್ರತಿ_directory_cb(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg,
				    ಪೂರ್ಣಾಂಕ unset)
{
	ರಚನೆ unpack_trees_options *opts;

	BUG_ON_OPT_NEG(unset);

	opts = (ರಚನೆ unpack_trees_options *)opt->ಮೌಲ್ಯ;

	ಆದರೆ (!opts->update)
		die("--exclude-per-directory is meaningless unless -u");
	ಆದರೆ (strcmp(arg, ".gitignore"))
		die("--exclude-per-directory argument must be .gitignore");
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ debug_stage(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *label, ಸ್ಥಿರಾಂಕ ರಚನೆ cache_ನಮೂದು *ce,
			ರಚನೆ unpack_trees_options *o)
{
	ಬರೆ("%s ", label);
	ಆದರೆ (!ce)
		ಬರೆ("(missing)\n");
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ce == o->df_conflict_ನಮೂದು)
		ಬರೆ("(conflict)\n");
	ಇಲ್ಲದಿದ್ದರೆ
		ಬರೆ("%06o #%d %s %.8s\n",
		       ce->ce_ಬಹುಲಕ, ce_stage(ce), ce->name,
		       oid_ಗಮ್ಯ_hex(&ce->oid));
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ debug_ವಿಲೀನ(ಸ್ಥಿರಾಂಕ ರಚನೆ cache_ನಮೂದು * ಸ್ಥಿರಾಂಕ *stages,
		       ರಚನೆ unpack_trees_options *o)
{
	ಪೂರ್ಣಾಂಕ i;

	ಬರೆ("* %d-way merge\n", o->ಆಂತರಿಕ.ವಿಲೀನ_ಗಾತ್ರ);
	debug_stage("index", stages[0], o);
	ಗಾಗಿ (i = 1; i <= o->ಆಂತರಿಕ.ವಿಲೀನ_ಗಾತ್ರ; i++) {
		ಅಕ್ಷರ buf[24];
		xsnprintf(buf, ಗಾತ್ರ(buf), "ent#%d", i);
		debug_stage(buf, stages[i], o);
	}
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ git_read_ವೃಕ್ಷ_config(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *var, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ,
				ಸ್ಥಿರಾಂಕ ರಚನೆ config_context *ctx, ಶೂನ್ಯ *cb)
{
	ಆದರೆ (!strcmp(var, "submodule.recurse"))
		ಹಿಂತಿರುಗಿ git_default_submodule_config(var, ಮೌಲ್ಯ, cb);

	ಹಿಂತಿರುಗಿ git_default_config(var, ಮೌಲ್ಯ, ctx, cb);
}

ಪೂರ್ಣಾಂಕ cmd_read_ವೃಕ್ಷ(ಪೂರ್ಣಾಂಕ argc,
		  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
		  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cmd_ಪೂರ್ವಪ್ರತ್ಯಯ,
		  ರಚನೆ repository *repo ಬಳಸದ)
{
	ಪೂರ್ಣಾಂಕ i, stage = 0;
	ರಚನೆ object_id oid;
	ರಚನೆ ವೃಕ್ಷ_desc t[MAX_UNPACK_TREES];
	ರಚನೆ unpack_trees_options opts;
	ಪೂರ್ಣಾಂಕ ಪೂರ್ವಪ್ರತ್ಯಯ_ಗಣ = 0;
	ರಚನೆ ಬೀಗ_file ಬೀಗ_file = ಬೀಗ_INIT;
	ಸ್ಥಿರಾಂಕ ರಚನೆ option read_ವೃಕ್ಷ_options[] = {
		OPT_SUPER_ಪೂರ್ವಪ್ರತ್ಯಯ(&opts.super_ಪೂರ್ವಪ್ರತ್ಯಯ),
		OPT_CALLBACK_F(0, "index-output", ಖಾಲಿ, N_("file"),
		  N_("write resulting index to <file>"),
		  PARSE_OPT_NONEG, ಸೂಚ್ಯಂಕ_output_cb),
		OPT_BOOL(0, "empty", &read_ಖಾಲಿಗಳು,
			    N_("only empty the index")),
		OPT__VERBOSE(&opts.verbose_update, N_("be verbose")),
		OPT_ಗುಂಪು(N_("Merging")),
		OPT_BOOL('m', ಖಾಲಿ, &opts.ವಿಲೀನ,
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
			.ಮೌಲ್ಯ = &opts.ಪೂರ್ವಪ್ರತ್ಯಯ,
			.argh = N_("<subdirectory>/"),
			.help = N_("read the tree into the index under <subdirectory>/"),
			.flags = PARSE_OPT_NONEG,
		},
		OPT_BOOL('u', ಖಾಲಿ, &opts.update,
			 N_("update working tree with merge result")),
		OPT_CALLBACK_F(0, "exclude-per-directory", &opts,
		  N_("gitignore"),
		  N_("allow explicitly ignored files to be overwritten"),
		  PARSE_OPT_NONEG, exclude_ಪ್ರತಿ_directory_cb),
		OPT_BOOL('i', ಖಾಲಿ, &opts.ಸೂಚ್ಯಂಕ_only,
			 N_("don't check the working tree after merging")),
		OPT_DRY_ಸತತ(&opts.dry_ಸತತ, N_("don't update the index or the work tree")),
		OPT_BOOL(0, "no-sparse-checkout", &opts.skip_sparse_checkout,
			 N_("skip applying sparse checkout filter")),
		OPT_BOOL(0, "debug-unpack", &opts.ಆಂತರಿಕ.debug_unpack,
			 N_("debug unpack-trees")),
		OPT_CALLBACK_F(0, "recurse-submodules", ಖಾಲಿ,
			    "checkout", "control recursive updating of submodules",
			    PARSE_OPT_OPTARG, option_parse_recurse_submodules_worktree_updater),
		OPT__QUIET(&opts.quiet, N_("suppress feedback messages")),
		OPT_ಕೊನೆ()
	};

	memset(&opts, 0, ಗಾತ್ರ(opts));
	opts.ತಲೆ_idx = -1;
	opts.src_ಸೂಚ್ಯಂಕ = the_repository->ಸೂಚ್ಯಂಕ;
	opts.dst_ಸೂಚ್ಯಂಕ = the_repository->ಸೂಚ್ಯಂಕ;

	repo_config(the_repository, git_read_ವೃಕ್ಷ_config, ಖಾಲಿ);

	argc = parse_options(argc, argv, cmd_ಪೂರ್ವಪ್ರತ್ಯಯ, read_ವೃಕ್ಷ_options,
			     read_ವೃಕ್ಷ_usage, 0);

	ಪೂರ್ವಪ್ರತ್ಯಯ_ಗಣ = opts.ಪೂರ್ವಪ್ರತ್ಯಯ ? 1 : 0;
	ಆದರೆ (1 < opts.ವಿಲೀನ + opts.reset + ಪೂರ್ವಪ್ರತ್ಯಯ_ಗಣ)
		die("Which one? -m, --reset, or --prefix?");

	/* Prefix should not start with a directory separator */
	ಆದರೆ (opts.ಪೂರ್ವಪ್ರತ್ಯಯ && opts.ಪೂರ್ವಪ್ರತ್ಯಯ[0] == '/')
		die("Invalid prefix, prefix cannot start with '/'");

	ಆದರೆ (opts.reset)
		opts.reset = UNPACK_RESET_OVERWRITE_UNTRACKED;

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_ತುಂಬಿದ_ಸೂಚ್ಯಂಕ = 0;

	repo_hold_locked_ಸೂಚ್ಯಂಕ(the_repository, &ಬೀಗ_file, ಬೀಗ_DIE_ON_ERROR);

	/*
	 * NEEDSWORK
	 *
	 * The old index should be read anyway even if we're going to
	 * destroy all index entries because we still need to preserve
	 * certain information such as index version or split-index
	 * mode.
	 */

	ಆದರೆ (opts.reset || opts.ವಿಲೀನ || opts.ಪೂರ್ವಪ್ರತ್ಯಯ) {
		ಆದರೆ (repo_read_ಸೂಚ್ಯಂಕ_unmerged(the_repository) && (opts.ಪೂರ್ವಪ್ರತ್ಯಯ || opts.ವಿಲೀನ))
			die(_("You need to resolve your current index first"));
		stage = opts.ವಿಲೀನ = 1;
	}
	resolve_undo_clear_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ);

	ಗಾಗಿ (i = 0; i < argc; i++) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg = argv[i];

		ಆದರೆ (repo_get_oid(the_repository, arg, &oid))
			die("Not a valid object name %s", arg);
		ಆದರೆ (list_ವೃಕ್ಷ(&oid) < 0)
			die("failed to unpack tree object %s", arg);
		stage++;
	}
	ಆದರೆ (!nr_trees && !read_ಖಾಲಿಗಳು && !opts.ವಿಲೀನ)
		warning("read-tree: emptying the index with no arguments is deprecated; use --empty");
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (nr_trees > 0 && read_ಖಾಲಿಗಳು)
		die("passing trees as arguments contradicts --empty");

	ಆದರೆ (1 < opts.ಸೂಚ್ಯಂಕ_only + opts.update)
		die("-u and -i at the same time makes no sense");
	ಆದರೆ ((opts.update || opts.ಸೂಚ್ಯಂಕ_only) && !opts.ವಿಲೀನ)
		die("%s is meaningless without -m, --reset, or --prefix",
		    opts.update ? "-u" : "-i");
	ಆದರೆ (opts.update && !opts.reset)
		opts.preserve_ignored = 0;
	/* otherwise, opts.preserve_ignored is irrelevant */
	ಆದರೆ (opts.ವಿಲೀನ && !opts.ಸೂಚ್ಯಂಕ_only)
		ಸಿದ್ಧತೆ_work_ವೃಕ್ಷ(the_repository);

	ಆದರೆ (opts.skip_sparse_checkout)
		ensure_ತುಂಬಿದ_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ);

	ಆದರೆ (opts.ವಿಲೀನ) {
		ಆಯ್ಕೆ (stage - 1) {
		ಸ್ಥಿತಿ 0:
			die("you must specify at least one tree to merge");
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ 1:
			opts.fn = opts.ಪೂರ್ವಪ್ರತ್ಯಯ ? bind_ವಿಲೀನ : oneway_ವಿಲೀನ;
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ 2:
			opts.fn = twoway_ವಿಲೀನ;
			opts.initial_checkout = is_ಸೂಚ್ಯಂಕ_unborn(the_repository->ಸೂಚ್ಯಂಕ);
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ 3:
		ಪೂರ್ವನಿಯೋಜಿತ:
			opts.fn = threeway_ವಿಲೀನ;
			ನಿಲ್ಲಿಸು;
		}

		ಆದರೆ (stage - 1 >= 3)
			opts.ತಲೆ_idx = stage - 2;
		ಇಲ್ಲದಿದ್ದರೆ
			opts.ತಲೆ_idx = 1;
	}

	ಆದರೆ (opts.ಆಂತರಿಕ.debug_unpack)
		opts.fn = debug_ವಿಲೀನ;

	/* If we're going to prime_cache_tree later, skip cache tree update */
	ಆದರೆ (nr_trees == 1 && !opts.ಪೂರ್ವಪ್ರತ್ಯಯ)
		opts.skip_cache_ವೃಕ್ಷ_update = 1;

	cache_ವೃಕ್ಷ_free(&the_repository->ಸೂಚ್ಯಂಕ->cache_ವೃಕ್ಷ);
	ಗಾಗಿ (i = 0; i < nr_trees; i++) {
		ರಚನೆ ವೃಕ್ಷ *ವೃಕ್ಷ = trees[i];
		ಆದರೆ (repo_parse_ವೃಕ್ಷ(the_repository, ವೃಕ್ಷ) < 0)
			ಹಿಂತಿರುಗಿ 128;
		init_ವೃಕ್ಷ_desc(t+i, &ವೃಕ್ಷ->object.oid, ವೃಕ್ಷ->ಬಫರ್, ವೃಕ್ಷ->ಗಾತ್ರ_2);
	}
	ಆದರೆ (unpack_trees(nr_trees, t, &opts))
		ಹಿಂತಿರುಗಿ 128;

	ಆದರೆ (opts.ಆಂತರಿಕ.debug_unpack || opts.dry_ಸತತ)
		ಹಿಂತಿರುಗಿ 0; /* do not write the index out */

	/*
	 * When reading only one tree (either the most basic form,
	 * "-m ent" or "--reset ent" form), we can obtain a fully
	 * valid cache-tree because the index must match exactly
	 * what came from the tree.
	 */
	ಆದರೆ (nr_trees == 1 && !opts.ಪೂರ್ವಪ್ರತ್ಯಯ)
		ಅವಿಭಾಜ್ಯ_cache_ವೃಕ್ಷ(the_repository,
				 the_repository->ಸೂಚ್ಯಂಕ,
				 trees[0]);

	ಆದರೆ (write_locked_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ, &ಬೀಗ_file, COMMIT_ಬೀಗ))
		die("unable to write new index file");
	ಹಿಂತಿರುಗಿ 0;
}
