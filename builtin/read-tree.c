/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#define USE_THE_REPOSITORY_VARIABLE
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

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ nr_trees;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ read_empty;
ಸ್ಥಿರ ಸಂರಚನಾ ವೃಕ್ಷ *trees[MAX_UNPACK_TREES];

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ list_ವೃಕ್ಷ(ಸಂರಚನಾ object_id *oid)
{
	ಸಂರಚನಾ ವೃಕ್ಷ *ವೃಕ್ಷ;

	ಯದಿ (nr_trees >= MAX_UNPACK_TREES)
		die("I cannot read more than %d trees", MAX_UNPACK_TREES);
	ವೃಕ್ಷ = repo_parse_ವೃಕ್ಷ_indirect(the_repository, oid);
	ಯದಿ (!ವೃಕ್ಷ)
		ಪ್ರತ್ಯಾಗಮ -1;
	trees[nr_trees++] = ವೃಕ್ಷ;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ read_ವೃಕ್ಷ_usage[] = {
	N_("git read-tree [(-m [--trivial] [--aggressive] | --reset | --prefix=<prefix>)\n"
	   "              [-u | -i]] [--index-output=<file>] [--no-sparse-checkout]\n"
	   "              (--empty | <tree-ish1> [<tree-ish2> [<tree-ish3>]])"),
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ index_output_cb(ಧ್ರುವ ಸಂರಚನಾ option *opt UNUSED, ಧ್ರುವ ವರ್ಣ *arg,
				 ಪೂರ್ಣಾಙ್ಕ unset)
{
	BUG_ON_OPT_NEG(unset);
	set_alternate_index_output(arg);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ exclude_per_directory_cb(ಧ್ರುವ ಸಂರಚನಾ option *opt, ಧ್ರುವ ವರ್ಣ *arg,
				    ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಸಂರಚನಾ unpack_trees_options *opts;

	BUG_ON_OPT_NEG(unset);

	opts = (ಸಂರಚನಾ unpack_trees_options *)opt->ಮೌಲ್ಯ;

	ಯದಿ (!opts->update)
		die("--exclude-per-directory is meaningless unless -u");
	ಯದಿ (strcmp(arg, ".gitignore"))
		die("--exclude-per-directory argument must be .gitignore");
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ debug_stage(ಧ್ರುವ ವರ್ಣ *label, ಧ್ರುವ ಸಂರಚನಾ cache_entry *ce,
			ಸಂರಚನಾ unpack_trees_options *o)
{
	printf("%s ", label);
	ಯದಿ (!ce)
		printf("(missing)\n");
	ಅನ್ಯಥಾ ಯದಿ (ce == o->df_conflict_entry)
		printf("(conflict)\n");
	ಅನ್ಯಥಾ
		printf("%06o #%d %s %.8s\n",
		       ce->ce_mode, ce_stage(ce), ce->name,
		       oid_to_hex(&ce->oid));
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ debug_merge(ಧ್ರುವ ಸಂರಚನಾ cache_entry * ಧ್ರುವ *stages,
		       ಸಂರಚನಾ unpack_trees_options *o)
{
	ಪೂರ್ಣಾಙ್ಕ i;

	printf("* %d-way merge\n", o->internal.merge_size);
	debug_stage("index", stages[0], o);
	ಪ್ರತಿ (i = 1; i <= o->internal.merge_size; i++) {
		ವರ್ಣ buf[24];
		xsnprintf(buf, ಪರಿಮಾಣ(buf), "ent#%d", i);
		debug_stage(buf, stages[i], o);
	}
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ git_read_ವೃಕ್ಷ_config(ಧ್ರುವ ವರ್ಣ *var, ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ,
				ಧ್ರುವ ಸಂರಚನಾ config_context *ctx, ಶೂನ್ಯ *cb)
{
	ಯದಿ (!strcmp(var, "submodule.recurse"))
		ಪ್ರತ್ಯಾಗಮ git_default_submodule_config(var, ಮೌಲ್ಯ, cb);

	ಪ್ರತ್ಯಾಗಮ git_default_config(var, ಮೌಲ್ಯ, ctx, cb);
}

ಪೂರ್ಣಾಙ್ಕ cmd_read_ವೃಕ್ಷ(ಪೂರ್ಣಾಙ್ಕ argc,
		  ಧ್ರುವ ವರ್ಣ **argv,
		  ಧ್ರುವ ವರ್ಣ *cmd_prefix,
		  ಸಂರಚನಾ repository *repo UNUSED)
{
	ಪೂರ್ಣಾಙ್ಕ i, stage = 0;
	ಸಂರಚನಾ object_id oid;
	ಸಂರಚನಾ ವೃಕ್ಷ_desc t[MAX_UNPACK_TREES];
	ಸಂರಚನಾ unpack_trees_options opts;
	ಪೂರ್ಣಾಙ್ಕ prefix_set = 0;
	ಸಂರಚನಾ lock_file lock_file = LOCK_INIT;
	ಧ್ರುವ ಸಂರಚನಾ option read_ವೃಕ್ಷ_options[] = {
		OPT__SUPER_PREFIX(&opts.super_prefix),
		OPT_CALLBACK_F(0, "index-output", ರಿಕ್ತ, N_("file"),
		  N_("write resulting index to <file>"),
		  PARSE_OPT_NONEG, index_output_cb),
		OPT_BOOL(0, "empty", &read_empty,
			    N_("only empty the index")),
		OPT__VERBOSE(&opts.verbose_update, N_("be verbose")),
		OPT_GROUP(N_("Merging")),
		OPT_BOOL('m', ರಿಕ್ತ, &opts.merge,
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
			.ಮೌಲ್ಯ = &opts.prefix,
			.argh = N_("<subdirectory>/"),
			.help = N_("read the tree into the index under <subdirectory>/"),
			.flags = PARSE_OPT_NONEG,
		},
		OPT_BOOL('u', ರಿಕ್ತ, &opts.update,
			 N_("update working tree with merge result")),
		OPT_CALLBACK_F(0, "exclude-per-directory", &opts,
		  N_("gitignore"),
		  N_("allow explicitly ignored files to be overwritten"),
		  PARSE_OPT_NONEG, exclude_per_directory_cb),
		OPT_BOOL('i', ರಿಕ್ತ, &opts.index_only,
			 N_("don't check the working tree after merging")),
		OPT__DRY_RUN(&opts.dry_run, N_("don't update the index or the work tree")),
		OPT_BOOL(0, "no-sparse-checkout", &opts.skip_sparse_checkout,
			 N_("skip applying sparse checkout filter")),
		OPT_BOOL(0, "debug-unpack", &opts.internal.debug_unpack,
			 N_("debug unpack-trees")),
		OPT_CALLBACK_F(0, "recurse-submodules", ರಿಕ್ತ,
			    "checkout", "control recursive updating of submodules",
			    PARSE_OPT_OPTARG, option_parse_recurse_submodules_worktree_updater),
		OPT__QUIET(&opts.quiet, N_("suppress feedback messages")),
		OPT_END()
	};

	memset(&opts, 0, ಪರಿಮಾಣ(opts));
	opts.head_idx = -1;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;

	repo_config(the_repository, git_read_ವೃಕ್ಷ_config, ರಿಕ್ತ);

	argc = parse_options(argc, argv, cmd_prefix, read_ವೃಕ್ಷ_options,
			     read_ವೃಕ್ಷ_usage, 0);

	prefix_set = opts.prefix ? 1 : 0;
	ಯದಿ (1 < opts.merge + opts.reset + prefix_set)
		die("Which one? -m, --reset, or --prefix?");

	/* Prefix should not start with a directory separator */
	ಯದಿ (opts.prefix && opts.prefix[0] == '/')
		die("Invalid prefix, prefix cannot start with '/'");

	ಯದಿ (opts.reset)
		opts.reset = UNPACK_RESET_OVERWRITE_UNTRACKED;

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);

	/*
	 * NEEDSWORK
	 *
	 * The old index should be read anyway even if we're going to
	 * destroy all index entries because we still need to preserve
	 * certain information such as index version or split-index
	 * mode.
	 */

	ಯದಿ (opts.reset || opts.merge || opts.prefix) {
		ಯದಿ (repo_read_index_unmerged(the_repository) && (opts.prefix || opts.merge))
			die(_("You need to resolve your current index first"));
		stage = opts.merge = 1;
	}
	resolve_undo_clear_index(the_repository->index);

	ಪ್ರತಿ (i = 0; i < argc; i++) {
		ಧ್ರುವ ವರ್ಣ *arg = argv[i];

		ಯದಿ (repo_get_oid(the_repository, arg, &oid))
			die("Not a valid object name %s", arg);
		ಯದಿ (list_ವೃಕ್ಷ(&oid) < 0)
			die("failed to unpack tree object %s", arg);
		stage++;
	}
	ಯದಿ (!nr_trees && !read_empty && !opts.merge)
		warning("read-tree: emptying the index with no arguments is deprecated; use --empty");
	ಅನ್ಯಥಾ ಯದಿ (nr_trees > 0 && read_empty)
		die("passing trees as arguments contradicts --empty");

	ಯದಿ (1 < opts.index_only + opts.update)
		die("-u and -i at the same time makes no sense");
	ಯದಿ ((opts.update || opts.index_only) && !opts.merge)
		die("%s is meaningless without -m, --reset, or --prefix",
		    opts.update ? "-u" : "-i");
	ಯದಿ (opts.update && !opts.reset)
		opts.preserve_ignored = 0;
	/* otherwise, opts.preserve_ignored is irrelevant */
	ಯದಿ (opts.merge && !opts.index_only)
		setup_work_ವೃಕ್ಷ(the_repository);

	ಯದಿ (opts.skip_sparse_checkout)
		ensure_full_index(the_repository->index);

	ಯದಿ (opts.merge) {
		ವಿಕಲ್ಪ (stage - 1) {
		ಪಕ್ಷ 0:
			die("you must specify at least one tree to merge");
			ಭಙ್ಗ;
		ಪಕ್ಷ 1:
			opts.fn = opts.prefix ? bind_merge : oneway_merge;
			ಭಙ್ಗ;
		ಪಕ್ಷ 2:
			opts.fn = twoway_merge;
			opts.initial_checkout = is_index_unborn(the_repository->index);
			ಭಙ್ಗ;
		ಪಕ್ಷ 3:
		ಸ್ವಭಾವ:
			opts.fn = threeway_merge;
			ಭಙ್ಗ;
		}

		ಯದಿ (stage - 1 >= 3)
			opts.head_idx = stage - 2;
		ಅನ್ಯಥಾ
			opts.head_idx = 1;
	}

	ಯದಿ (opts.internal.debug_unpack)
		opts.fn = debug_merge;

	/* If we're going to prime_cache_tree later, skip cache tree update */
	ಯದಿ (nr_trees == 1 && !opts.prefix)
		opts.skip_cache_ವೃಕ್ಷ_update = 1;

	cache_ವೃಕ್ಷ_free(&the_repository->index->cache_ವೃಕ್ಷ);
	ಪ್ರತಿ (i = 0; i < nr_trees; i++) {
		ಸಂರಚನಾ ವೃಕ್ಷ *ವೃಕ್ಷ = trees[i];
		ಯದಿ (repo_parse_ವೃಕ್ಷ(the_repository, ವೃಕ್ಷ) < 0)
			ಪ್ರತ್ಯಾಗಮ 128;
		init_ವೃಕ್ಷ_desc(t+i, &ವೃಕ್ಷ->object.oid, ವೃಕ್ಷ->buffer, ವೃಕ್ಷ->size);
	}
	ಯದಿ (unpack_trees(nr_trees, t, &opts))
		ಪ್ರತ್ಯಾಗಮ 128;

	ಯದಿ (opts.internal.debug_unpack || opts.dry_run)
		ಪ್ರತ್ಯಾಗಮ 0; /* do not write the index out */

	/*
	 * When reading only one tree (either the most basic form,
	 * "-m ent" or "--reset ent" form), we can obtain a fully
	 * valid cache-tree because the index must match exactly
	 * what came from the tree.
	 */
	ಯದಿ (nr_trees == 1 && !opts.prefix)
		ಅವಿಭಾಜ್ಯ_cache_ವೃಕ್ಷ(the_repository,
				 the_repository->index,
				 trees[0]);

	ಯದಿ (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
		die("unable to write new index file");
	ಪ್ರತ್ಯಾಗಮ 0;
}
