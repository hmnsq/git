/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#define USE_THE_REPOSITORY_VARIABLE
#অন্তর্ভাৱ "builtin.h"
#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ "environment.h"
#অন্তর্ভাৱ "gettext.h"
#অন্তর্ভাৱ "hex.h"
#অন্তর্ভাৱ "lockfile.h"
#অন্তর্ভাৱ "object.h"
#অন্তর্ভাৱ "object-name.h"
#অন্তর্ভাৱ "tree.h"
#অন্তর্ভাৱ "tree-walk.h"
#অন্তর্ভাৱ "cache-tree.h"
#অন্তর্ভাৱ "unpack-trees.h"
#অন্তর্ভাৱ "parse-options.h"
#অন্তর্ভাৱ "resolve-undo.h"
#অন্তর্ভাৱ "setup.h"
#অন্তর্ভাৱ "sparse-index.h"
#অন্তর্ভাৱ "submodule.h"

স্থির পূর্ণাঙ্ক nr_trees;
স্থির পূর্ণাঙ্ক read_empty;
স্থির সংরচনা বৃক্ষ *trees[MAX_UNPACK_TREES];

স্থির পূর্ণাঙ্ক list_বৃক্ষ(সংরচনা object_id *oid)
{
	সংরচনা বৃক্ষ *বৃক্ষ;

	যদি (nr_trees >= MAX_UNPACK_TREES)
		die("I cannot read more than %d trees", MAX_UNPACK_TREES);
	বৃক্ষ = repo_parse_বৃক্ষ_indirect(the_repository, oid);
	যদি (!বৃক্ষ)
		ঘূৰাই -1;
	trees[nr_trees++] = বৃক্ষ;
	ঘূৰাই 0;
}

স্থির ধ্রুৱ ৱর্ণ * ধ্রুৱ read_বৃক্ষ_usage[] = {
	N_("git read-tree [(-m [--trivial] [--aggressive] | --reset | --prefix=<prefix>)\n"
	   "              [-u | -i]] [--index-output=<file>] [--no-sparse-checkout]\n"
	   "              (--empty | <tree-ish1> [<tree-ish2> [<tree-ish3>]])"),
	রিক্ত
};

স্থির পূর্ণাঙ্ক index_output_cb(ধ্রুৱ সংরচনা option *opt UNUSED, ধ্রুৱ ৱর্ণ *arg,
				 পূর্ণাঙ্ক unset)
{
	BUG_ON_OPT_NEG(unset);
	set_alternate_index_output(arg);
	ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক exclude_per_directory_cb(ধ্রুৱ সংরচনা option *opt, ধ্রুৱ ৱর্ণ *arg,
				    পূর্ণাঙ্ক unset)
{
	সংরচনা unpack_trees_options *opts;

	BUG_ON_OPT_NEG(unset);

	opts = (সংরচনা unpack_trees_options *)opt->মান;

	যদি (!opts->update)
		die("--exclude-per-directory is meaningless unless -u");
	যদি (strcmp(arg, ".gitignore"))
		die("--exclude-per-directory argument must be .gitignore");
	ঘূৰাই 0;
}

স্থির শূন্য debug_stage(ধ্রুৱ ৱর্ণ *label, ধ্রুৱ সংরচনা cache_entry *ce,
			সংরচনা unpack_trees_options *o)
{
	printf("%s ", label);
	যদি (!ce)
		printf("(missing)\n");
	অন্যথা যদি (ce == o->df_conflict_entry)
		printf("(conflict)\n");
	অন্যথা
		printf("%06o #%d %s %.8s\n",
		       ce->ce_mode, ce_stage(ce), ce->name,
		       oid_to_hex(&ce->oid));
}

স্থির পূর্ণাঙ্ক debug_merge(ধ্রুৱ সংরচনা cache_entry * ধ্রুৱ *stages,
		       সংরচনা unpack_trees_options *o)
{
	পূর্ণাঙ্ক i;

	printf("* %d-way merge\n", o->internal.merge_size);
	debug_stage("index", stages[0], o);
	বাবে (i = 1; i <= o->internal.merge_size; i++) {
		ৱর্ণ buf[24];
		xsnprintf(buf, পরিমাণ(buf), "ent#%d", i);
		debug_stage(buf, stages[i], o);
	}
	ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক git_read_বৃক্ষ_config(ধ্রুৱ ৱর্ণ *var, ধ্রুৱ ৱর্ণ *মান,
				ধ্রুৱ সংরচনা config_context *ctx, শূন্য *cb)
{
	যদি (!strcmp(var, "submodule.recurse"))
		ঘূৰাই git_default_submodule_config(var, মান, cb);

	ঘূৰাই git_default_config(var, মান, ctx, cb);
}

পূর্ণাঙ্ক cmd_read_বৃক্ষ(পূর্ণাঙ্ক argc,
		  ধ্রুৱ ৱর্ণ **argv,
		  ধ্রুৱ ৱর্ণ *cmd_prefix,
		  সংরচনা repository *repo UNUSED)
{
	পূর্ণাঙ্ক i, stage = 0;
	সংরচনা object_id oid;
	সংরচনা বৃক্ষ_desc t[MAX_UNPACK_TREES];
	সংরচনা unpack_trees_options opts;
	পূর্ণাঙ্ক prefix_set = 0;
	সংরচনা lock_file lock_file = LOCK_INIT;
	ধ্রুৱ সংরচনা option read_বৃক্ষ_options[] = {
		OPT__SUPER_PREFIX(&opts.super_prefix),
		OPT_CALLBACK_F(0, "index-output", রিক্ত, N_("file"),
		  N_("write resulting index to <file>"),
		  PARSE_OPT_NONEG, index_output_cb),
		OPT_BOOL(0, "empty", &read_empty,
			    N_("only empty the index")),
		OPT__VERBOSE(&opts.verbose_update, N_("be verbose")),
		OPT_GROUP(N_("Merging")),
		OPT_BOOL('m', রিক্ত, &opts.merge,
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
			.মান = &opts.prefix,
			.argh = N_("<subdirectory>/"),
			.help = N_("read the tree into the index under <subdirectory>/"),
			.flags = PARSE_OPT_NONEG,
		},
		OPT_BOOL('u', রিক্ত, &opts.update,
			 N_("update working tree with merge result")),
		OPT_CALLBACK_F(0, "exclude-per-directory", &opts,
		  N_("gitignore"),
		  N_("allow explicitly ignored files to be overwritten"),
		  PARSE_OPT_NONEG, exclude_per_directory_cb),
		OPT_BOOL('i', রিক্ত, &opts.index_only,
			 N_("don't check the working tree after merging")),
		OPT__DRY_RUN(&opts.dry_run, N_("don't update the index or the work tree")),
		OPT_BOOL(0, "no-sparse-checkout", &opts.skip_sparse_checkout,
			 N_("skip applying sparse checkout filter")),
		OPT_BOOL(0, "debug-unpack", &opts.internal.debug_unpack,
			 N_("debug unpack-trees")),
		OPT_CALLBACK_F(0, "recurse-submodules", রিক্ত,
			    "checkout", "control recursive updating of submodules",
			    PARSE_OPT_OPTARG, option_parse_recurse_submodules_worktree_updater),
		OPT__QUIET(&opts.quiet, N_("suppress feedback messages")),
		OPT_END()
	};

	memset(&opts, 0, পরিমাণ(opts));
	opts.head_idx = -1;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;

	repo_config(the_repository, git_read_বৃক্ষ_config, রিক্ত);

	argc = parse_options(argc, argv, cmd_prefix, read_বৃক্ষ_options,
			     read_বৃক্ষ_usage, 0);

	prefix_set = opts.prefix ? 1 : 0;
	যদি (1 < opts.merge + opts.reset + prefix_set)
		die("Which one? -m, --reset, or --prefix?");

	/* Prefix should not start with a directory separator */
	যদি (opts.prefix && opts.prefix[0] == '/')
		die("Invalid prefix, prefix cannot start with '/'");

	যদি (opts.reset)
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

	যদি (opts.reset || opts.merge || opts.prefix) {
		যদি (repo_read_index_unmerged(the_repository) && (opts.prefix || opts.merge))
			die(_("You need to resolve your current index first"));
		stage = opts.merge = 1;
	}
	resolve_undo_clear_index(the_repository->index);

	বাবে (i = 0; i < argc; i++) {
		ধ্রুৱ ৱর্ণ *arg = argv[i];

		যদি (repo_get_oid(the_repository, arg, &oid))
			die("Not a valid object name %s", arg);
		যদি (list_বৃক্ষ(&oid) < 0)
			die("failed to unpack tree object %s", arg);
		stage++;
	}
	যদি (!nr_trees && !read_empty && !opts.merge)
		warning("read-tree: emptying the index with no arguments is deprecated; use --empty");
	অন্যথা যদি (nr_trees > 0 && read_empty)
		die("passing trees as arguments contradicts --empty");

	যদি (1 < opts.index_only + opts.update)
		die("-u and -i at the same time makes no sense");
	যদি ((opts.update || opts.index_only) && !opts.merge)
		die("%s is meaningless without -m, --reset, or --prefix",
		    opts.update ? "-u" : "-i");
	যদি (opts.update && !opts.reset)
		opts.preserve_ignored = 0;
	/* otherwise, opts.preserve_ignored is irrelevant */
	যদি (opts.merge && !opts.index_only)
		setup_work_বৃক্ষ(the_repository);

	যদি (opts.skip_sparse_checkout)
		ensure_full_index(the_repository->index);

	যদি (opts.merge) {
		ৱিকল্প (stage - 1) {
		পক্ষ 0:
			die("you must specify at least one tree to merge");
			ভঙ্গ;
		পক্ষ 1:
			opts.fn = opts.prefix ? bind_merge : oneway_merge;
			ভঙ্গ;
		পক্ষ 2:
			opts.fn = twoway_merge;
			opts.initial_checkout = is_index_unborn(the_repository->index);
			ভঙ্গ;
		পক্ষ 3:
		স্ৱভাৱ:
			opts.fn = threeway_merge;
			ভঙ্গ;
		}

		যদি (stage - 1 >= 3)
			opts.head_idx = stage - 2;
		অন্যথা
			opts.head_idx = 1;
	}

	যদি (opts.internal.debug_unpack)
		opts.fn = debug_merge;

	/* If we're going to prime_cache_tree later, skip cache tree update */
	যদি (nr_trees == 1 && !opts.prefix)
		opts.skip_cache_বৃক্ষ_update = 1;

	cache_বৃক্ষ_free(&the_repository->index->cache_বৃক্ষ);
	বাবে (i = 0; i < nr_trees; i++) {
		সংরচনা বৃক্ষ *বৃক্ষ = trees[i];
		যদি (repo_parse_বৃক্ষ(the_repository, বৃক্ষ) < 0)
			ঘূৰাই 128;
		init_বৃক্ষ_desc(t+i, &বৃক্ষ->object.oid, বৃক্ষ->buffer, বৃক্ষ->size);
	}
	যদি (unpack_trees(nr_trees, t, &opts))
		ঘূৰাই 128;

	যদি (opts.internal.debug_unpack || opts.dry_run)
		ঘূৰাই 0; /* do not write the index out */

	/*
	 * When reading only one tree (either the most basic form,
	 * "-m ent" or "--reset ent" form), we can obtain a fully
	 * valid cache-tree because the index must match exactly
	 * what came from the tree.
	 */
	যদি (nr_trees == 1 && !opts.prefix)
		মৌলিক_cache_বৃক্ষ(the_repository,
				 the_repository->index,
				 trees[0]);

	যদি (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
		die("unable to write new index file");
	ঘূৰাই 0;
}
