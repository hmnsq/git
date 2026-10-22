/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#define USE_THE_REPOSITORY_VARIABLE
#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "hex.h"
#ضمن "lockfile.h"
#ضمن "object.h"
#ضمن "object-name.h"
#ضمن "tree.h"
#ضمن "tree-walk.h"
#ضمن "cache-tree.h"
#ضمن "unpack-trees.h"
#ضمن "parse-options.h"
#ضمن "resolve-undo.h"
#ضمن "setup.h"
#ضمن "sparse-index.h"
#ضمن "submodule.h"

ساكن صحيح nr_trees;
ساكن صحيح read_empty;
ساكن بنية tree *trees[MAX_UNPACK_TREES];

ساكن صحيح list_tree(بنية object_id *oid)
{
	بنية tree *tree;

	𐡄𐡍 (nr_trees >= MAX_UNPACK_TREES)
		die("I cannot read more than %d trees", MAX_UNPACK_TREES);
	tree = repo_parse_tree_indirect(the_repository, oid);
	𐡄𐡍 (!tree)
		أرجع -1;
	trees[nr_trees++] = tree;
	أرجع 0;
}

ساكن ثابت حرف * ثابت read_tree_usage[] = {
	N_("git read-tree [(-m [--trivial] [--aggressive] | --reset | --prefix=<prefix>)\n"
	   "              [-u | -i]] [--index-output=<file>] [--no-sparse-checkout]\n"
	   "              (--empty | <tree-ish1> [<tree-ish2> [<tree-ish3>]])"),
	عدم
};

ساكن صحيح index_output_cb(ثابت بنية option *opt UNUSED, ثابت حرف *arg,
				 صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	set_alternate_index_output(arg);
	أرجع 0;
}

ساكن صحيح exclude_per_directory_cb(ثابت بنية option *opt, ثابت حرف *arg,
				    صحيح unset)
{
	بنية unpack_trees_options *opts;

	BUG_ON_OPT_NEG(unset);

	opts = (بنية unpack_trees_options *)opt->value;

	𐡄𐡍 (!opts->update)
		die("--exclude-per-directory is meaningless unless -u");
	𐡄𐡍 (strcmp(arg, ".gitignore"))
		die("--exclude-per-directory argument must be .gitignore");
	أرجع 0;
}

ساكن فراغ debug_stage(ثابت حرف *label, ثابت بنية cache_entry *ce,
			بنية unpack_trees_options *o)
{
	printf("%s ", label);
	𐡄𐡍 (!ce)
		printf("(missing)\n");
	وإلا 𐡄𐡍 (ce == o->df_conflict_entry)
		printf("(conflict)\n");
	وإلا
		printf("%06o #%d %s %.8s\n",
		       ce->ce_mode, ce_stage(ce), ce->name,
		       oid_to_hex(&ce->oid));
}

ساكن صحيح debug_merge(ثابت بنية cache_entry * ثابت *stages,
		       بنية unpack_trees_options *o)
{
	صحيح i;

	printf("* %d-way merge\n", o->internal.merge_size);
	debug_stage("index", stages[0], o);
	لكل (i = 1; i <= o->internal.merge_size; i++) {
		حرف buf[24];
		xsnprintf(buf, حجم(buf), "ent#%d", i);
		debug_stage(buf, stages[i], o);
	}
	أرجع 0;
}

ساكن صحيح git_read_tree_config(ثابت حرف *var, ثابت حرف *value,
				ثابت بنية config_context *ctx, فراغ *cb)
{
	𐡄𐡍 (!strcmp(var, "submodule.recurse"))
		أرجع git_default_submodule_config(var, value, cb);

	أرجع git_default_config(var, value, ctx, cb);
}

صحيح cmd_read_tree(صحيح argc,
		  ثابت حرف **argv,
		  ثابت حرف *cmd_prefix,
		  بنية repository *repo UNUSED)
{
	صحيح i, stage = 0;
	بنية object_id oid;
	بنية tree_desc t[MAX_UNPACK_TREES];
	بنية unpack_trees_options opts;
	صحيح prefix_set = 0;
	بنية lock_file lock_file = LOCK_INIT;
	ثابت بنية option read_tree_options[] = {
		OPT__SUPER_PREFIX(&opts.super_prefix),
		OPT_CALLBACK_F(0, "index-output", عدم, N_("file"),
		  N_("write resulting index to <file>"),
		  PARSE_OPT_NONEG, index_output_cb),
		OPT_BOOL(0, "empty", &read_empty,
			    N_("only empty the index")),
		OPT__VERBOSE(&opts.verbose_update, N_("be verbose")),
		OPT_GROUP(N_("Merging")),
		OPT_BOOL('m', عدم, &opts.merge,
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
			.value = &opts.prefix,
			.argh = N_("<subdirectory>/"),
			.help = N_("read the tree into the index under <subdirectory>/"),
			.flags = PARSE_OPT_NONEG,
		},
		OPT_BOOL('u', عدم, &opts.update,
			 N_("update working tree with merge result")),
		OPT_CALLBACK_F(0, "exclude-per-directory", &opts,
		  N_("gitignore"),
		  N_("allow explicitly ignored files to be overwritten"),
		  PARSE_OPT_NONEG, exclude_per_directory_cb),
		OPT_BOOL('i', عدم, &opts.index_only,
			 N_("don't check the working tree after merging")),
		OPT__DRY_RUN(&opts.dry_run, N_("don't update the index or the work tree")),
		OPT_BOOL(0, "no-sparse-checkout", &opts.skip_sparse_checkout,
			 N_("skip applying sparse checkout filter")),
		OPT_BOOL(0, "debug-unpack", &opts.internal.debug_unpack,
			 N_("debug unpack-trees")),
		OPT_CALLBACK_F(0, "recurse-submodules", عدم,
			    "checkout", "control recursive updating of submodules",
			    PARSE_OPT_OPTARG, option_parse_recurse_submodules_worktree_updater),
		OPT__QUIET(&opts.quiet, N_("suppress feedback messages")),
		OPT_END()
	};

	memset(&opts, 0, حجم(opts));
	opts.head_idx = -1;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;

	repo_config(the_repository, git_read_tree_config, عدم);

	argc = parse_options(argc, argv, cmd_prefix, read_tree_options,
			     read_tree_usage, 0);

	prefix_set = opts.prefix ? 1 : 0;
	𐡄𐡍 (1 < opts.merge + opts.reset + prefix_set)
		die("Which one? -m, --reset, or --prefix?");

	/* Prefix should not start with a directory separator */
	𐡄𐡍 (opts.prefix && opts.prefix[0] == '/')
		die("Invalid prefix, prefix cannot start with '/'");

	𐡄𐡍 (opts.reset)
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

	𐡄𐡍 (opts.reset || opts.merge || opts.prefix) {
		𐡄𐡍 (repo_read_index_unmerged(the_repository) && (opts.prefix || opts.merge))
			die(_("You need to resolve your current index first"));
		stage = opts.merge = 1;
	}
	resolve_undo_clear_index(the_repository->index);

	لكل (i = 0; i < argc; i++) {
		ثابت حرف *arg = argv[i];

		𐡄𐡍 (repo_get_oid(the_repository, arg, &oid))
			die("Not a valid object name %s", arg);
		𐡄𐡍 (list_tree(&oid) < 0)
			die("failed to unpack tree object %s", arg);
		stage++;
	}
	𐡄𐡍 (!nr_trees && !read_empty && !opts.merge)
		warning("read-tree: emptying the index with no arguments is deprecated; use --empty");
	وإلا 𐡄𐡍 (nr_trees > 0 && read_empty)
		die("passing trees as arguments contradicts --empty");

	𐡄𐡍 (1 < opts.index_only + opts.update)
		die("-u and -i at the same time makes no sense");
	𐡄𐡍 ((opts.update || opts.index_only) && !opts.merge)
		die("%s is meaningless without -m, --reset, or --prefix",
		    opts.update ? "-u" : "-i");
	𐡄𐡍 (opts.update && !opts.reset)
		opts.preserve_ignored = 0;
	/* otherwise, opts.preserve_ignored is irrelevant */
	𐡄𐡍 (opts.merge && !opts.index_only)
		setup_work_tree(the_repository);

	𐡄𐡍 (opts.skip_sparse_checkout)
		ensure_full_index(the_repository->index);

	𐡄𐡍 (opts.merge) {
		بدل (stage - 1) {
		حالة 0:
			die("you must specify at least one tree to merge");
			اكسر;
		حالة 1:
			opts.fn = opts.prefix ? bind_merge : oneway_merge;
			اكسر;
		حالة 2:
			opts.fn = twoway_merge;
			opts.initial_checkout = is_index_unborn(the_repository->index);
			اكسر;
		حالة 3:
		افتراضي:
			opts.fn = threeway_merge;
			اكسر;
		}

		𐡄𐡍 (stage - 1 >= 3)
			opts.head_idx = stage - 2;
		وإلا
			opts.head_idx = 1;
	}

	𐡄𐡍 (opts.internal.debug_unpack)
		opts.fn = debug_merge;

	/* If we're going to prime_cache_tree later, skip cache tree update */
	𐡄𐡍 (nr_trees == 1 && !opts.prefix)
		opts.skip_cache_tree_update = 1;

	cache_tree_free(&the_repository->index->cache_tree);
	لكل (i = 0; i < nr_trees; i++) {
		بنية tree *tree = trees[i];
		𐡄𐡍 (repo_parse_tree(the_repository, tree) < 0)
			أرجع 128;
		init_tree_desc(t+i, &tree->object.oid, tree->buffer, tree->size);
	}
	𐡄𐡍 (unpack_trees(nr_trees, t, &opts))
		أرجع 128;

	𐡄𐡍 (opts.internal.debug_unpack || opts.dry_run)
		أرجع 0; /* do not write the index out */

	/*
	 * When reading only one tree (either the most basic form,
	 * "-m ent" or "--reset ent" form), we can obtain a fully
	 * valid cache-tree because the index must match exactly
	 * what came from the tree.
	 */
	𐡄𐡍 (nr_trees == 1 && !opts.prefix)
		prime_cache_tree(the_repository,
				 the_repository->index,
				 trees[0]);

	𐡄𐡍 (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
		die("unable to write new index file");
	أرجع 0;
}
