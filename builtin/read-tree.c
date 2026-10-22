/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#define USE_THE_REPOSITORY_VARIABLE
#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "hex.h"
#அந்தர்பாவ "lockfile.h"
#அந்தர்பாவ "object.h"
#அந்தர்பாவ "object-name.h"
#அந்தர்பாவ "tree.h"
#அந்தர்பாவ "tree-walk.h"
#அந்தர்பாவ "cache-tree.h"
#அந்தர்பாவ "unpack-trees.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "resolve-undo.h"
#அந்தர்பாவ "setup.h"
#அந்தர்பாவ "sparse-index.h"
#அந்தர்பாவ "submodule.h"

நிலையான முழு nr_trees;
நிலையான முழு read_empty;
நிலையான கட்டமைப்பு மரம் *trees[MAX_UNPACK_TREES];

நிலையான முழு list_மரம்(கட்டமைப்பு object_id *oid)
{
	கட்டமைப்பு மரம் *மரம்;

	எனில் (nr_trees >= MAX_UNPACK_TREES)
		die("I cannot read more than %d trees", MAX_UNPACK_TREES);
	மரம் = repo_parse_மரம்_indirect(the_repository, oid);
	எனில் (!மரம்)
		திரும்பு -1;
	trees[nr_trees++] = மரம்;
	திரும்பு 0;
}

நிலையான மாறிலி எழுத்து * மாறிலி read_மரம்_usage[] = {
	N_("git read-tree [(-m [--trivial] [--aggressive] | --reset | --prefix=<prefix>)\n"
	   "              [-u | -i]] [--index-output=<file>] [--no-sparse-checkout]\n"
	   "              (--empty | <tree-ish1> [<tree-ish2> [<tree-ish3>]])"),
	வெற்று
};

நிலையான முழு index_output_cb(மாறிலி கட்டமைப்பு option *opt UNUSED, மாறிலி எழுத்து *arg,
				 முழு unset)
{
	BUG_ON_OPT_NEG(unset);
	set_alternate_index_output(arg);
	திரும்பு 0;
}

நிலையான முழு exclude_per_directory_cb(மாறிலி கட்டமைப்பு option *opt, மாறிலி எழுத்து *arg,
				    முழு unset)
{
	கட்டமைப்பு unpack_trees_options *opts;

	BUG_ON_OPT_NEG(unset);

	opts = (கட்டமைப்பு unpack_trees_options *)opt->மதிப்பு;

	எனில் (!opts->update)
		die("--exclude-per-directory is meaningless unless -u");
	எனில் (strcmp(arg, ".gitignore"))
		die("--exclude-per-directory argument must be .gitignore");
	திரும்பு 0;
}

நிலையான வெறுமை debug_stage(மாறிலி எழுத்து *label, மாறிலி கட்டமைப்பு cache_entry *ce,
			கட்டமைப்பு unpack_trees_options *o)
{
	printf("%s ", label);
	எனில் (!ce)
		printf("(missing)\n");
	இல்லையெனில் எனில் (ce == o->df_conflict_entry)
		printf("(conflict)\n");
	இல்லையெனில்
		printf("%06o #%d %s %.8s\n",
		       ce->ce_mode, ce_stage(ce), ce->name,
		       oid_to_hex(&ce->oid));
}

நிலையான முழு debug_merge(மாறிலி கட்டமைப்பு cache_entry * மாறிலி *stages,
		       கட்டமைப்பு unpack_trees_options *o)
{
	முழு i;

	printf("* %d-way merge\n", o->internal.merge_size);
	debug_stage("index", stages[0], o);
	க்கு (i = 1; i <= o->internal.merge_size; i++) {
		எழுத்து buf[24];
		xsnprintf(buf, அளவு(buf), "ent#%d", i);
		debug_stage(buf, stages[i], o);
	}
	திரும்பு 0;
}

நிலையான முழு git_read_மரம்_config(மாறிலி எழுத்து *var, மாறிலி எழுத்து *மதிப்பு,
				மாறிலி கட்டமைப்பு config_context *ctx, வெறுமை *cb)
{
	எனில் (!strcmp(var, "submodule.recurse"))
		திரும்பு git_default_submodule_config(var, மதிப்பு, cb);

	திரும்பு git_default_config(var, மதிப்பு, ctx, cb);
}

முழு cmd_read_மரம்(முழு argc,
		  மாறிலி எழுத்து **argv,
		  மாறிலி எழுத்து *cmd_prefix,
		  கட்டமைப்பு repository *repo UNUSED)
{
	முழு i, stage = 0;
	கட்டமைப்பு object_id oid;
	கட்டமைப்பு மரம்_desc t[MAX_UNPACK_TREES];
	கட்டமைப்பு unpack_trees_options opts;
	முழு prefix_set = 0;
	கட்டமைப்பு lock_file lock_file = LOCK_INIT;
	மாறிலி கட்டமைப்பு option read_மரம்_options[] = {
		OPT__SUPER_PREFIX(&opts.super_prefix),
		OPT_CALLBACK_F(0, "index-output", வெற்று, N_("file"),
		  N_("write resulting index to <file>"),
		  PARSE_OPT_NONEG, index_output_cb),
		OPT_BOOL(0, "empty", &read_empty,
			    N_("only empty the index")),
		OPT__VERBOSE(&opts.verbose_update, N_("be verbose")),
		OPT_GROUP(N_("Merging")),
		OPT_BOOL('m', வெற்று, &opts.merge,
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
			.மதிப்பு = &opts.prefix,
			.argh = N_("<subdirectory>/"),
			.help = N_("read the tree into the index under <subdirectory>/"),
			.flags = PARSE_OPT_NONEG,
		},
		OPT_BOOL('u', வெற்று, &opts.update,
			 N_("update working tree with merge result")),
		OPT_CALLBACK_F(0, "exclude-per-directory", &opts,
		  N_("gitignore"),
		  N_("allow explicitly ignored files to be overwritten"),
		  PARSE_OPT_NONEG, exclude_per_directory_cb),
		OPT_BOOL('i', வெற்று, &opts.index_only,
			 N_("don't check the working tree after merging")),
		OPT__DRY_RUN(&opts.dry_run, N_("don't update the index or the work tree")),
		OPT_BOOL(0, "no-sparse-checkout", &opts.skip_sparse_checkout,
			 N_("skip applying sparse checkout filter")),
		OPT_BOOL(0, "debug-unpack", &opts.internal.debug_unpack,
			 N_("debug unpack-trees")),
		OPT_CALLBACK_F(0, "recurse-submodules", வெற்று,
			    "checkout", "control recursive updating of submodules",
			    PARSE_OPT_OPTARG, option_parse_recurse_submodules_worktree_updater),
		OPT__QUIET(&opts.quiet, N_("suppress feedback messages")),
		OPT_END()
	};

	memset(&opts, 0, அளவு(opts));
	opts.head_idx = -1;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;

	repo_config(the_repository, git_read_மரம்_config, வெற்று);

	argc = parse_options(argc, argv, cmd_prefix, read_மரம்_options,
			     read_மரம்_usage, 0);

	prefix_set = opts.prefix ? 1 : 0;
	எனில் (1 < opts.merge + opts.reset + prefix_set)
		die("Which one? -m, --reset, or --prefix?");

	/* Prefix should not start with a directory separator */
	எனில் (opts.prefix && opts.prefix[0] == '/')
		die("Invalid prefix, prefix cannot start with '/'");

	எனில் (opts.reset)
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

	எனில் (opts.reset || opts.merge || opts.prefix) {
		எனில் (repo_read_index_unmerged(the_repository) && (opts.prefix || opts.merge))
			die(_("You need to resolve your current index first"));
		stage = opts.merge = 1;
	}
	resolve_undo_clear_index(the_repository->index);

	க்கு (i = 0; i < argc; i++) {
		மாறிலி எழுத்து *arg = argv[i];

		எனில் (repo_get_oid(the_repository, arg, &oid))
			die("Not a valid object name %s", arg);
		எனில் (list_மரம்(&oid) < 0)
			die("failed to unpack tree object %s", arg);
		stage++;
	}
	எனில் (!nr_trees && !read_empty && !opts.merge)
		warning("read-tree: emptying the index with no arguments is deprecated; use --empty");
	இல்லையெனில் எனில் (nr_trees > 0 && read_empty)
		die("passing trees as arguments contradicts --empty");

	எனில் (1 < opts.index_only + opts.update)
		die("-u and -i at the same time makes no sense");
	எனில் ((opts.update || opts.index_only) && !opts.merge)
		die("%s is meaningless without -m, --reset, or --prefix",
		    opts.update ? "-u" : "-i");
	எனில் (opts.update && !opts.reset)
		opts.preserve_ignored = 0;
	/* otherwise, opts.preserve_ignored is irrelevant */
	எனில் (opts.merge && !opts.index_only)
		setup_work_மரம்(the_repository);

	எனில் (opts.skip_sparse_checkout)
		ensure_full_index(the_repository->index);

	எனில் (opts.merge) {
		தேர்வு (stage - 1) {
		நிலை 0:
			die("you must specify at least one tree to merge");
			நிறுத்து;
		நிலை 1:
			opts.fn = opts.prefix ? bind_merge : oneway_merge;
			நிறுத்து;
		நிலை 2:
			opts.fn = twoway_merge;
			opts.initial_checkout = is_index_unborn(the_repository->index);
			நிறுத்து;
		நிலை 3:
		இயல்பு:
			opts.fn = threeway_merge;
			நிறுத்து;
		}

		எனில் (stage - 1 >= 3)
			opts.head_idx = stage - 2;
		இல்லையெனில்
			opts.head_idx = 1;
	}

	எனில் (opts.internal.debug_unpack)
		opts.fn = debug_merge;

	/* If we're going to prime_cache_tree later, skip cache tree update */
	எனில் (nr_trees == 1 && !opts.prefix)
		opts.skip_cache_மரம்_update = 1;

	cache_மரம்_free(&the_repository->index->cache_மரம்);
	க்கு (i = 0; i < nr_trees; i++) {
		கட்டமைப்பு மரம் *மரம் = trees[i];
		எனில் (repo_parse_மரம்(the_repository, மரம்) < 0)
			திரும்பு 128;
		init_மரம்_desc(t+i, &மரம்->object.oid, மரம்->buffer, மரம்->size);
	}
	எனில் (unpack_trees(nr_trees, t, &opts))
		திரும்பு 128;

	எனில் (opts.internal.debug_unpack || opts.dry_run)
		திரும்பு 0; /* do not write the index out */

	/*
	 * When reading only one tree (either the most basic form,
	 * "-m ent" or "--reset ent" form), we can obtain a fully
	 * valid cache-tree because the index must match exactly
	 * what came from the tree.
	 */
	எனில் (nr_trees == 1 && !opts.prefix)
		பகாஎண்_cache_மரம்(the_repository,
				 the_repository->index,
				 trees[0]);

	எனில் (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
		die("unable to write new index file");
	திரும்பு 0;
}
