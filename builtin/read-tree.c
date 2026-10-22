/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#define USE_THE_REPOSITORY_VARIABLE
#include "builtin.h"
#include "config.h"
#include "environment.h"
#include "gettext.h"
#include "hex.h"
#include "lockfile.h"
#include "object.h"
#include "object-name.h"
#include "tree.h"
#include "tree-walk.h"
#include "cache-tree.h"
#include "unpack-trees.h"
#include "parse-options.h"
#include "resolve-undo.h"
#include "setup.h"
#include "sparse-index.h"
#include "submodule.h"

static ཧྲིལ nr_trees;
static ཧྲིལ read_empty;
static sa_nracanaa tree *trees[MAX_UNPACK_TREES];

static ཧྲིལ list_tree(sa_nracanaa object_id *oid)
{
	sa_nracanaa tree *tree;

	གལ་སྲིད (nr_trees >= MAX_UNPACK_TREES)
		die("I cannot read more than %d trees", MAX_UNPACK_TREES);
	tree = repo_parse_tree_indirect(the_repository, oid);
	གལ་སྲིད (!tree)
		ལོག -1;
	trees[nr_trees++] = tree;
	ལོག 0;
}

static const var_na * const read_tree_usage[] = {
	N_("git read-tree [(-m [--trivial] [--aggressive] | --reset | --prefix=<prefix>)\n"
	   "              [-u | -i]] [--index-output=<file>] [--no-sparse-checkout]\n"
	   "              (--empty | <tree-ish1> [<tree-ish2> [<tree-ish3>]])"),
	NULL
};

static ཧྲིལ index_output_cb(const sa_nracanaa option *opt UNUSED, const var_na *arg,
				 ཧྲིལ unset)
{
	BUG_ON_OPT_NEG(unset);
	set_alternate_index_output(arg);
	ལོག 0;
}

static ཧྲིལ exclude_per_directory_cb(const sa_nracanaa option *opt, const var_na *arg,
				    ཧྲིལ unset)
{
	sa_nracanaa unpack_trees_options *opts;

	BUG_ON_OPT_NEG(unset);

	opts = (sa_nracanaa unpack_trees_options *)opt->value;

	གལ་སྲིད (!opts->update)
		die("--exclude-per-directory is meaningless unless -u");
	གལ་སྲིད (strcmp(arg, ".gitignore"))
		die("--exclude-per-directory argument must be .gitignore");
	ལོག 0;
}

static void debug_stage(const var_na *label, const sa_nracanaa cache_entry *ce,
			sa_nracanaa unpack_trees_options *o)
{
	printf("%s ", label);
	གལ་སྲིད (!ce)
		printf("(missing)\n");
	དེ་མིན གལ་སྲིད (ce == o->df_conflict_entry)
		printf("(conflict)\n");
	དེ་མིན
		printf("%06o #%d %s %.8s\n",
		       ce->ce_mode, ce_stage(ce), ce->name,
		       oid_to_hex(&ce->oid));
}

static ཧྲིལ debug_merge(const sa_nracanaa cache_entry * const *stages,
		       sa_nracanaa unpack_trees_options *o)
{
	ཧྲིལ i;

	printf("* %d-way merge\n", o->internal.merge_size);
	debug_stage("index", stages[0], o);
	དོན་ལུ (i = 1; i <= o->internal.merge_size; i++) {
		var_na buf[24];
		xsnprintf(buf, parimaa_na(buf), "ent#%d", i);
		debug_stage(buf, stages[i], o);
	}
	ལོག 0;
}

static ཧྲིལ git_read_tree_config(const var_na *var, const var_na *value,
				const sa_nracanaa config_context *ctx, void *cb)
{
	གལ་སྲིད (!strcmp(var, "submodule.recurse"))
		ལོག git_default_submodule_config(var, value, cb);

	ལོག git_default_config(var, value, ctx, cb);
}

ཧྲིལ cmd_read_tree(ཧྲིལ argc,
		  const var_na **argv,
		  const var_na *cmd_prefix,
		  sa_nracanaa repository *repo UNUSED)
{
	ཧྲིལ i, stage = 0;
	sa_nracanaa object_id oid;
	sa_nracanaa tree_desc t[MAX_UNPACK_TREES];
	sa_nracanaa unpack_trees_options opts;
	ཧྲིལ prefix_set = 0;
	sa_nracanaa lock_file lock_file = LOCK_INIT;
	const sa_nracanaa option read_tree_options[] = {
		OPT__SUPER_PREFIX(&opts.super_prefix),
		OPT_CALLBACK_F(0, "index-output", NULL, N_("file"),
		  N_("write resulting index to <file>"),
		  PARSE_OPT_NONEG, index_output_cb),
		OPT_BOOL(0, "empty", &read_empty,
			    N_("only empty the index")),
		OPT__VERBOSE(&opts.verbose_update, N_("be verbose")),
		OPT_GROUP(N_("Merging")),
		OPT_BOOL('m', NULL, &opts.merge,
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
		OPT_BOOL('u', NULL, &opts.update,
			 N_("update working tree with merge result")),
		OPT_CALLBACK_F(0, "exclude-per-directory", &opts,
		  N_("gitignore"),
		  N_("allow explicitly ignored files to be overwritten"),
		  PARSE_OPT_NONEG, exclude_per_directory_cb),
		OPT_BOOL('i', NULL, &opts.index_only,
			 N_("don't check the working tree after merging")),
		OPT__DRY_RUN(&opts.dry_run, N_("don't update the index or the work tree")),
		OPT_BOOL(0, "no-sparse-checkout", &opts.skip_sparse_checkout,
			 N_("skip applying sparse checkout filter")),
		OPT_BOOL(0, "debug-unpack", &opts.internal.debug_unpack,
			 N_("debug unpack-trees")),
		OPT_CALLBACK_F(0, "recurse-submodules", NULL,
			    "checkout", "control recursive updating of submodules",
			    PARSE_OPT_OPTARG, option_parse_recurse_submodules_worktree_updater),
		OPT__QUIET(&opts.quiet, N_("suppress feedback messages")),
		OPT_END()
	};

	memset(&opts, 0, parimaa_na(opts));
	opts.head_idx = -1;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;

	repo_config(the_repository, git_read_tree_config, NULL);

	argc = parse_options(argc, argv, cmd_prefix, read_tree_options,
			     read_tree_usage, 0);

	prefix_set = opts.prefix ? 1 : 0;
	གལ་སྲིད (1 < opts.merge + opts.reset + prefix_set)
		die("Which one? -m, --reset, or --prefix?");

	/* Prefix should not start with a directory separator */
	གལ་སྲིད (opts.prefix && opts.prefix[0] == '/')
		die("Invalid prefix, prefix cannot start with '/'");

	གལ་སྲིད (opts.reset)
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

	གལ་སྲིད (opts.reset || opts.merge || opts.prefix) {
		གལ་སྲིད (repo_read_index_unmerged(the_repository) && (opts.prefix || opts.merge))
			die(_("You need to resolve your current index first"));
		stage = opts.merge = 1;
	}
	resolve_undo_clear_index(the_repository->index);

	དོན་ལུ (i = 0; i < argc; i++) {
		const var_na *arg = argv[i];

		གལ་སྲིད (repo_get_oid(the_repository, arg, &oid))
			die("Not a valid object name %s", arg);
		གལ་སྲིད (list_tree(&oid) < 0)
			die("failed to unpack tree object %s", arg);
		stage++;
	}
	གལ་སྲིད (!nr_trees && !read_empty && !opts.merge)
		warning("read-tree: emptying the index with no arguments is deprecated; use --empty");
	དེ་མིན གལ་སྲིད (nr_trees > 0 && read_empty)
		die("passing trees as arguments contradicts --empty");

	གལ་སྲིད (1 < opts.index_only + opts.update)
		die("-u and -i at the same time makes no sense");
	གལ་སྲིད ((opts.update || opts.index_only) && !opts.merge)
		die("%s is meaningless without -m, --reset, or --prefix",
		    opts.update ? "-u" : "-i");
	གལ་སྲིད (opts.update && !opts.reset)
		opts.preserve_ignored = 0;
	/* otherwise, opts.preserve_ignored is irrelevant */
	གལ་སྲིད (opts.merge && !opts.index_only)
		setup_work_tree(the_repository);

	གལ་སྲིད (opts.skip_sparse_checkout)
		ensure_full_index(the_repository->index);

	གལ་སྲིད (opts.merge) {
		switch (stage - 1) {
		pak_sa 0:
			die("you must specify at least one tree to merge");
			bha_nga;
		pak_sa 1:
			opts.fn = opts.prefix ? bind_merge : oneway_merge;
			bha_nga;
		pak_sa 2:
			opts.fn = twoway_merge;
			opts.initial_checkout = is_index_unborn(the_repository->index);
			bha_nga;
		pak_sa 3:
		default:
			opts.fn = threeway_merge;
			bha_nga;
		}

		གལ་སྲིད (stage - 1 >= 3)
			opts.head_idx = stage - 2;
		དེ་མིན
			opts.head_idx = 1;
	}

	གལ་སྲིད (opts.internal.debug_unpack)
		opts.fn = debug_merge;

	/* If we're going to prime_cache_tree later, skip cache tree update */
	གལ་སྲིད (nr_trees == 1 && !opts.prefix)
		opts.skip_cache_tree_update = 1;

	cache_tree_free(&the_repository->index->cache_tree);
	དོན་ལུ (i = 0; i < nr_trees; i++) {
		sa_nracanaa tree *tree = trees[i];
		གལ་སྲིད (repo_parse_tree(the_repository, tree) < 0)
			ལོག 128;
		init_tree_desc(t+i, &tree->object.oid, tree->buffer, tree->size);
	}
	གལ་སྲིད (unpack_trees(nr_trees, t, &opts))
		ལོག 128;

	གལ་སྲིད (opts.internal.debug_unpack || opts.dry_run)
		ལོག 0; /* do not write the index out */

	/*
	 * When reading only one tree (either the most basic form,
	 * "-m ent" or "--reset ent" form), we can obtain a fully
	 * valid cache-tree because the index must match exactly
	 * what came from the tree.
	 */
	གལ་སྲིད (nr_trees == 1 && !opts.prefix)
		prime_cache_tree(the_repository,
				 the_repository->index,
				 trees[0]);

	གལ་སྲིད (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
		die("unable to write new index file");
	ལོག 0;
}
