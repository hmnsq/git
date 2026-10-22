/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#define USE_THE_REPOSITORY_VARIABLE
#شامل "builtin.h"
#شامل "config.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "hex.h"
#شامل "lockfile.h"
#شامل "object.h"
#شامل "object-name.h"
#شامل "tree.h"
#شامل "tree-walk.h"
#شامل "cache-tree.h"
#شامل "unpack-trees.h"
#شامل "parse-options.h"
#شامل "resolve-undo.h"
#شامل "setup.h"
#شامل "sparse-index.h"
#شامل "submodule.h"

ایستا صحیح nr_trees;
ایستا صحیح read_empty;
ایستا ساختار درخت *trees[MAX_UNPACK_TREES];

ایستا صحیح list_درخت(ساختار object_id *oid)
{
	ساختار درخت *درخت;

	اگر (nr_trees >= MAX_UNPACK_TREES)
		die("I cannot read more than %d trees", MAX_UNPACK_TREES);
	درخت = repo_parse_درخت_indirect(the_repository, oid);
	اگر (!درخت)
		بازگشت -1;
	trees[nr_trees++] = درخت;
	بازگشت 0;
}

ایستا ثابت نویسه * ثابت read_درخت_usage[] = {
	N_("git read-tree [(-m [--trivial] [--aggressive] | --reset | --prefix=<prefix>)\n"
	   "              [-u | -i]] [--index-output=<file>] [--no-sparse-checkout]\n"
	   "              (--empty | <tree-ish1> [<tree-ish2> [<tree-ish3>]])"),
	پوچ
};

ایستا صحیح index_output_cb(ثابت ساختار option *opt UNUSED, ثابت نویسه *arg,
				 صحیح unset)
{
	BUG_ON_OPT_NEG(unset);
	set_alternate_index_output(arg);
	بازگشت 0;
}

ایستا صحیح exclude_per_directory_cb(ثابت ساختار option *opt, ثابت نویسه *arg,
				    صحیح unset)
{
	ساختار unpack_trees_options *opts;

	BUG_ON_OPT_NEG(unset);

	opts = (ساختار unpack_trees_options *)opt->مقدار;

	اگر (!opts->update)
		die("--exclude-per-directory is meaningless unless -u");
	اگر (strcmp(arg, ".gitignore"))
		die("--exclude-per-directory argument must be .gitignore");
	بازگشت 0;
}

ایستا تهی debug_stage(ثابت نویسه *label, ثابت ساختار cache_entry *ce,
			ساختار unpack_trees_options *o)
{
	printf("%s ", label);
	اگر (!ce)
		printf("(missing)\n");
	وگرنه اگر (ce == o->df_conflict_entry)
		printf("(conflict)\n");
	وگرنه
		printf("%06o #%d %s %.8s\n",
		       ce->ce_mode, ce_stage(ce), ce->name,
		       oid_to_hex(&ce->oid));
}

ایستا صحیح debug_merge(ثابت ساختار cache_entry * ثابت *stages,
		       ساختار unpack_trees_options *o)
{
	صحیح i;

	printf("* %d-way merge\n", o->internal.merge_size);
	debug_stage("index", stages[0], o);
	برای (i = 1; i <= o->internal.merge_size; i++) {
		نویسه buf[24];
		xsnprintf(buf, اندازه(buf), "ent#%d", i);
		debug_stage(buf, stages[i], o);
	}
	بازگشت 0;
}

ایستا صحیح git_read_درخت_config(ثابت نویسه *var, ثابت نویسه *مقدار,
				ثابت ساختار config_context *ctx, تهی *cb)
{
	اگر (!strcmp(var, "submodule.recurse"))
		بازگشت git_default_submodule_config(var, مقدار, cb);

	بازگشت git_default_config(var, مقدار, ctx, cb);
}

صحیح cmd_read_درخت(صحیح argc,
		  ثابت نویسه **argv,
		  ثابت نویسه *cmd_prefix,
		  ساختار repository *repo UNUSED)
{
	صحیح i, stage = 0;
	ساختار object_id oid;
	ساختار درخت_desc t[MAX_UNPACK_TREES];
	ساختار unpack_trees_options opts;
	صحیح prefix_set = 0;
	ساختار lock_file lock_file = LOCK_INIT;
	ثابت ساختار option read_درخت_options[] = {
		OPT__SUPER_PREFIX(&opts.super_prefix),
		OPT_CALLBACK_F(0, "index-output", پوچ, N_("file"),
		  N_("write resulting index to <file>"),
		  PARSE_OPT_NONEG, index_output_cb),
		OPT_BOOL(0, "empty", &read_empty,
			    N_("only empty the index")),
		OPT__VERBOSE(&opts.verbose_update, N_("be verbose")),
		OPT_GROUP(N_("Merging")),
		OPT_BOOL('m', پوچ, &opts.merge,
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
			.مقدار = &opts.prefix,
			.argh = N_("<subdirectory>/"),
			.help = N_("read the tree into the index under <subdirectory>/"),
			.flags = PARSE_OPT_NONEG,
		},
		OPT_BOOL('u', پوچ, &opts.update,
			 N_("update working tree with merge result")),
		OPT_CALLBACK_F(0, "exclude-per-directory", &opts,
		  N_("gitignore"),
		  N_("allow explicitly ignored files to be overwritten"),
		  PARSE_OPT_NONEG, exclude_per_directory_cb),
		OPT_BOOL('i', پوچ, &opts.index_only,
			 N_("don't check the working tree after merging")),
		OPT__DRY_RUN(&opts.dry_run, N_("don't update the index or the work tree")),
		OPT_BOOL(0, "no-sparse-checkout", &opts.skip_sparse_checkout,
			 N_("skip applying sparse checkout filter")),
		OPT_BOOL(0, "debug-unpack", &opts.internal.debug_unpack,
			 N_("debug unpack-trees")),
		OPT_CALLBACK_F(0, "recurse-submodules", پوچ,
			    "checkout", "control recursive updating of submodules",
			    PARSE_OPT_OPTARG, option_parse_recurse_submodules_worktree_updater),
		OPT__QUIET(&opts.quiet, N_("suppress feedback messages")),
		OPT_END()
	};

	memset(&opts, 0, اندازه(opts));
	opts.head_idx = -1;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;

	repo_config(the_repository, git_read_درخت_config, پوچ);

	argc = parse_options(argc, argv, cmd_prefix, read_درخت_options,
			     read_درخت_usage, 0);

	prefix_set = opts.prefix ? 1 : 0;
	اگر (1 < opts.merge + opts.reset + prefix_set)
		die("Which one? -m, --reset, or --prefix?");

	/* Prefix should not start with a directory separator */
	اگر (opts.prefix && opts.prefix[0] == '/')
		die("Invalid prefix, prefix cannot start with '/'");

	اگر (opts.reset)
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

	اگر (opts.reset || opts.merge || opts.prefix) {
		اگر (repo_read_index_unmerged(the_repository) && (opts.prefix || opts.merge))
			die(_("You need to resolve your current index first"));
		stage = opts.merge = 1;
	}
	resolve_undo_clear_index(the_repository->index);

	برای (i = 0; i < argc; i++) {
		ثابت نویسه *arg = argv[i];

		اگر (repo_get_oid(the_repository, arg, &oid))
			die("Not a valid object name %s", arg);
		اگر (list_درخت(&oid) < 0)
			die("failed to unpack tree object %s", arg);
		stage++;
	}
	اگر (!nr_trees && !read_empty && !opts.merge)
		warning("read-tree: emptying the index with no arguments is deprecated; use --empty");
	وگرنه اگر (nr_trees > 0 && read_empty)
		die("passing trees as arguments contradicts --empty");

	اگر (1 < opts.index_only + opts.update)
		die("-u and -i at the same time makes no sense");
	اگر ((opts.update || opts.index_only) && !opts.merge)
		die("%s is meaningless without -m, --reset, or --prefix",
		    opts.update ? "-u" : "-i");
	اگر (opts.update && !opts.reset)
		opts.preserve_ignored = 0;
	/* otherwise, opts.preserve_ignored is irrelevant */
	اگر (opts.merge && !opts.index_only)
		setup_work_درخت(the_repository);

	اگر (opts.skip_sparse_checkout)
		ensure_full_index(the_repository->index);

	اگر (opts.merge) {
		انتخاب (stage - 1) {
		حالت 0:
			die("you must specify at least one tree to merge");
			بشکن;
		حالت 1:
			opts.fn = opts.prefix ? bind_merge : oneway_merge;
			بشکن;
		حالت 2:
			opts.fn = twoway_merge;
			opts.initial_checkout = is_index_unborn(the_repository->index);
			بشکن;
		حالت 3:
		پیشفرض:
			opts.fn = threeway_merge;
			بشکن;
		}

		اگر (stage - 1 >= 3)
			opts.head_idx = stage - 2;
		وگرنه
			opts.head_idx = 1;
	}

	اگر (opts.internal.debug_unpack)
		opts.fn = debug_merge;

	/* If we're going to prime_cache_tree later, skip cache tree update */
	اگر (nr_trees == 1 && !opts.prefix)
		opts.skip_cache_درخت_update = 1;

	cache_درخت_free(&the_repository->index->cache_درخت);
	برای (i = 0; i < nr_trees; i++) {
		ساختار درخت *درخت = trees[i];
		اگر (repo_parse_درخت(the_repository, درخت) < 0)
			بازگشت 128;
		init_درخت_desc(t+i, &درخت->object.oid, درخت->buffer, درخت->size);
	}
	اگر (unpack_trees(nr_trees, t, &opts))
		بازگشت 128;

	اگر (opts.internal.debug_unpack || opts.dry_run)
		بازگشت 0; /* do not write the index out */

	/*
	 * When reading only one tree (either the most basic form,
	 * "-m ent" or "--reset ent" form), we can obtain a fully
	 * valid cache-tree because the index must match exactly
	 * what came from the tree.
	 */
	اگر (nr_trees == 1 && !opts.prefix)
		اول_cache_درخت(the_repository,
				 the_repository->index,
				 trees[0]);

	اگر (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
		die("unable to write new index file");
	بازگشت 0;
}
