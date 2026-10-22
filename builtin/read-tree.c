/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#define USE_THE_REPOSITORY_VARIABLE
#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "hex.h"
#અન્તર્ભાવ "lockfile.h"
#અન્તર્ભાવ "object.h"
#અન્તર્ભાવ "object-name.h"
#અન્તર્ભાવ "tree.h"
#અન્તર્ભાવ "tree-walk.h"
#અન્તર્ભાવ "cache-tree.h"
#અન્તર્ભાવ "unpack-trees.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "resolve-undo.h"
#અન્તર્ભાવ "setup.h"
#અન્તર્ભાવ "sparse-index.h"
#અન્તર્ભાવ "submodule.h"

સ્થિર પૂર્ણાંક nr_trees;
સ્થિર પૂર્ણાંક read_empty;
સ્થિર સંરચના વૃક્ષ *trees[MAX_UNPACK_TREES];

સ્થિર પૂર્ણાંક list_વૃક્ષ(સંરચના object_id *oid)
{
	સંરચના વૃક્ષ *વૃક્ષ;

	જો (nr_trees >= MAX_UNPACK_TREES)
		die("I cannot read more than %d trees", MAX_UNPACK_TREES);
	વૃક્ષ = repo_parse_વૃક્ષ_indirect(the_repository, oid);
	જો (!વૃક્ષ)
		પાછા -1;
	trees[nr_trees++] = વૃક્ષ;
	પાછા 0;
}

સ્થિર અચલ અક્ષર * અચલ read_વૃક્ષ_usage[] = {
	N_("git read-tree [(-m [--trivial] [--aggressive] | --reset | --prefix=<prefix>)\n"
	   "              [-u | -i]] [--index-output=<file>] [--no-sparse-checkout]\n"
	   "              (--empty | <tree-ish1> [<tree-ish2> [<tree-ish3>]])"),
	ખાલી
};

સ્થિર પૂર્ણાંક index_output_cb(અચલ સંરચના option *opt UNUSED, અચલ અક્ષર *arg,
				 પૂર્ણાંક unset)
{
	BUG_ON_OPT_NEG(unset);
	set_alternate_index_output(arg);
	પાછા 0;
}

સ્થિર પૂર્ણાંક exclude_per_directory_cb(અચલ સંરચના option *opt, અચલ અક્ષર *arg,
				    પૂર્ણાંક unset)
{
	સંરચના unpack_trees_options *opts;

	BUG_ON_OPT_NEG(unset);

	opts = (સંરચના unpack_trees_options *)opt->કિંમત;

	જો (!opts->update)
		die("--exclude-per-directory is meaningless unless -u");
	જો (strcmp(arg, ".gitignore"))
		die("--exclude-per-directory argument must be .gitignore");
	પાછા 0;
}

સ્થિર શૂન્ય debug_stage(અચલ અક્ષર *label, અચલ સંરચના cache_entry *ce,
			સંરચના unpack_trees_options *o)
{
	printf("%s ", label);
	જો (!ce)
		printf("(missing)\n");
	નહીંતર જો (ce == o->df_conflict_entry)
		printf("(conflict)\n");
	નહીંતર
		printf("%06o #%d %s %.8s\n",
		       ce->ce_mode, ce_stage(ce), ce->name,
		       oid_to_hex(&ce->oid));
}

સ્થિર પૂર્ણાંક debug_merge(અચલ સંરચના cache_entry * અચલ *stages,
		       સંરચના unpack_trees_options *o)
{
	પૂર્ણાંક i;

	printf("* %d-way merge\n", o->internal.merge_size);
	debug_stage("index", stages[0], o);
	માટે (i = 1; i <= o->internal.merge_size; i++) {
		અક્ષર buf[24];
		xsnprintf(buf, કદ(buf), "ent#%d", i);
		debug_stage(buf, stages[i], o);
	}
	પાછા 0;
}

સ્થિર પૂર્ણાંક git_read_વૃક્ષ_config(અચલ અક્ષર *var, અચલ અક્ષર *કિંમત,
				અચલ સંરચના config_context *ctx, શૂન્ય *cb)
{
	જો (!strcmp(var, "submodule.recurse"))
		પાછા git_default_submodule_config(var, કિંમત, cb);

	પાછા git_default_config(var, કિંમત, ctx, cb);
}

પૂર્ણાંક cmd_read_વૃક્ષ(પૂર્ણાંક argc,
		  અચલ અક્ષર **argv,
		  અચલ અક્ષર *cmd_prefix,
		  સંરચના repository *repo UNUSED)
{
	પૂર્ણાંક i, stage = 0;
	સંરચના object_id oid;
	સંરચના વૃક્ષ_desc t[MAX_UNPACK_TREES];
	સંરચના unpack_trees_options opts;
	પૂર્ણાંક prefix_set = 0;
	સંરચના lock_file lock_file = LOCK_INIT;
	અચલ સંરચના option read_વૃક્ષ_options[] = {
		OPT__SUPER_PREFIX(&opts.super_prefix),
		OPT_CALLBACK_F(0, "index-output", ખાલી, N_("file"),
		  N_("write resulting index to <file>"),
		  PARSE_OPT_NONEG, index_output_cb),
		OPT_BOOL(0, "empty", &read_empty,
			    N_("only empty the index")),
		OPT__VERBOSE(&opts.verbose_update, N_("be verbose")),
		OPT_GROUP(N_("Merging")),
		OPT_BOOL('m', ખાલી, &opts.merge,
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
			.કિંમત = &opts.prefix,
			.argh = N_("<subdirectory>/"),
			.help = N_("read the tree into the index under <subdirectory>/"),
			.flags = PARSE_OPT_NONEG,
		},
		OPT_BOOL('u', ખાલી, &opts.update,
			 N_("update working tree with merge result")),
		OPT_CALLBACK_F(0, "exclude-per-directory", &opts,
		  N_("gitignore"),
		  N_("allow explicitly ignored files to be overwritten"),
		  PARSE_OPT_NONEG, exclude_per_directory_cb),
		OPT_BOOL('i', ખાલી, &opts.index_only,
			 N_("don't check the working tree after merging")),
		OPT__DRY_RUN(&opts.dry_run, N_("don't update the index or the work tree")),
		OPT_BOOL(0, "no-sparse-checkout", &opts.skip_sparse_checkout,
			 N_("skip applying sparse checkout filter")),
		OPT_BOOL(0, "debug-unpack", &opts.internal.debug_unpack,
			 N_("debug unpack-trees")),
		OPT_CALLBACK_F(0, "recurse-submodules", ખાલી,
			    "checkout", "control recursive updating of submodules",
			    PARSE_OPT_OPTARG, option_parse_recurse_submodules_worktree_updater),
		OPT__QUIET(&opts.quiet, N_("suppress feedback messages")),
		OPT_END()
	};

	memset(&opts, 0, કદ(opts));
	opts.head_idx = -1;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;

	repo_config(the_repository, git_read_વૃક્ષ_config, ખાલી);

	argc = parse_options(argc, argv, cmd_prefix, read_વૃક્ષ_options,
			     read_વૃક્ષ_usage, 0);

	prefix_set = opts.prefix ? 1 : 0;
	જો (1 < opts.merge + opts.reset + prefix_set)
		die("Which one? -m, --reset, or --prefix?");

	/* Prefix should not start with a directory separator */
	જો (opts.prefix && opts.prefix[0] == '/')
		die("Invalid prefix, prefix cannot start with '/'");

	જો (opts.reset)
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

	જો (opts.reset || opts.merge || opts.prefix) {
		જો (repo_read_index_unmerged(the_repository) && (opts.prefix || opts.merge))
			die(_("You need to resolve your current index first"));
		stage = opts.merge = 1;
	}
	resolve_undo_clear_index(the_repository->index);

	માટે (i = 0; i < argc; i++) {
		અચલ અક્ષર *arg = argv[i];

		જો (repo_get_oid(the_repository, arg, &oid))
			die("Not a valid object name %s", arg);
		જો (list_વૃક્ષ(&oid) < 0)
			die("failed to unpack tree object %s", arg);
		stage++;
	}
	જો (!nr_trees && !read_empty && !opts.merge)
		warning("read-tree: emptying the index with no arguments is deprecated; use --empty");
	નહીંતર જો (nr_trees > 0 && read_empty)
		die("passing trees as arguments contradicts --empty");

	જો (1 < opts.index_only + opts.update)
		die("-u and -i at the same time makes no sense");
	જો ((opts.update || opts.index_only) && !opts.merge)
		die("%s is meaningless without -m, --reset, or --prefix",
		    opts.update ? "-u" : "-i");
	જો (opts.update && !opts.reset)
		opts.preserve_ignored = 0;
	/* otherwise, opts.preserve_ignored is irrelevant */
	જો (opts.merge && !opts.index_only)
		setup_work_વૃક્ષ(the_repository);

	જો (opts.skip_sparse_checkout)
		ensure_full_index(the_repository->index);

	જો (opts.merge) {
		પસંદગી (stage - 1) {
		સ્થિતિ 0:
			die("you must specify at least one tree to merge");
			તોડો;
		સ્થિતિ 1:
			opts.fn = opts.prefix ? bind_merge : oneway_merge;
			તોડો;
		સ્થિતિ 2:
			opts.fn = twoway_merge;
			opts.initial_checkout = is_index_unborn(the_repository->index);
			તોડો;
		સ્થિતિ 3:
		મૂળભૂત:
			opts.fn = threeway_merge;
			તોડો;
		}

		જો (stage - 1 >= 3)
			opts.head_idx = stage - 2;
		નહીંતર
			opts.head_idx = 1;
	}

	જો (opts.internal.debug_unpack)
		opts.fn = debug_merge;

	/* If we're going to prime_cache_tree later, skip cache tree update */
	જો (nr_trees == 1 && !opts.prefix)
		opts.skip_cache_વૃક્ષ_update = 1;

	cache_વૃક્ષ_free(&the_repository->index->cache_વૃક્ષ);
	માટે (i = 0; i < nr_trees; i++) {
		સંરચના વૃક્ષ *વૃક્ષ = trees[i];
		જો (repo_parse_વૃક્ષ(the_repository, વૃક્ષ) < 0)
			પાછા 128;
		init_વૃક્ષ_desc(t+i, &વૃક્ષ->object.oid, વૃક્ષ->buffer, વૃક્ષ->size);
	}
	જો (unpack_trees(nr_trees, t, &opts))
		પાછા 128;

	જો (opts.internal.debug_unpack || opts.dry_run)
		પાછા 0; /* do not write the index out */

	/*
	 * When reading only one tree (either the most basic form,
	 * "-m ent" or "--reset ent" form), we can obtain a fully
	 * valid cache-tree because the index must match exactly
	 * what came from the tree.
	 */
	જો (nr_trees == 1 && !opts.prefix)
		અવિભાજ્ય_cache_વૃક્ષ(the_repository,
				 the_repository->index,
				 trees[0]);

	જો (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
		die("unable to write new index file");
	પાછા 0;
}
