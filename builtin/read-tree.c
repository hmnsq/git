/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#define USE_THE_REPOSITORY_VARIABLE
#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "hex.h"
#ଅନ୍ତର୍ଭାଵ "lockfile.h"
#ଅନ୍ତର୍ଭାଵ "object.h"
#ଅନ୍ତର୍ଭାଵ "object-name.h"
#ଅନ୍ତର୍ଭାଵ "tree.h"
#ଅନ୍ତର୍ଭାଵ "tree-walk.h"
#ଅନ୍ତର୍ଭାଵ "cache-tree.h"
#ଅନ୍ତର୍ଭାଵ "unpack-trees.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "resolve-undo.h"
#ଅନ୍ତର୍ଭାଵ "setup.h"
#ଅନ୍ତର୍ଭାଵ "sparse-index.h"
#ଅନ୍ତର୍ଭାଵ "submodule.h"

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା nr_trees;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା read_empty;
ସ୍ଥିର ସଂରଚନା ବୃକ୍ଷ *trees[MAX_UNPACK_TREES];

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା list_ବୃକ୍ଷ(ସଂରଚନା object_id *oid)
{
	ସଂରଚନା ବୃକ୍ଷ *ବୃକ୍ଷ;

	ଯଦି (nr_trees >= MAX_UNPACK_TREES)
		die("I cannot read more than %d trees", MAX_UNPACK_TREES);
	ବୃକ୍ଷ = repo_parse_ବୃକ୍ଷ_indirect(the_repository, oid);
	ଯଦି (!ବୃକ୍ଷ)
		ଫେରନ୍ତୁ -1;
	trees[nr_trees++] = ବୃକ୍ଷ;
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ read_ବୃକ୍ଷ_usage[] = {
	N_("git read-tree [(-m [--trivial] [--aggressive] | --reset | --prefix=<prefix>)\n"
	   "              [-u | -i]] [--index-output=<file>] [--no-sparse-checkout]\n"
	   "              (--empty | <tree-ish1> [<tree-ish2> [<tree-ish3>]])"),
	ଖାଲି
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା index_output_cb(ଅଚଳ ସଂରଚନା option *opt UNUSED, ଅଚଳ ଅକ୍ଷର *arg,
				 ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	BUG_ON_OPT_NEG(unset);
	set_alternate_index_output(arg);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା exclude_per_directory_cb(ଅଚଳ ସଂରଚନା option *opt, ଅଚଳ ଅକ୍ଷର *arg,
				    ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ସଂରଚନା unpack_trees_options *opts;

	BUG_ON_OPT_NEG(unset);

	opts = (ସଂରଚନା unpack_trees_options *)opt->ମୂଲ୍ୟ;

	ଯଦି (!opts->update)
		die("--exclude-per-directory is meaningless unless -u");
	ଯଦି (strcmp(arg, ".gitignore"))
		die("--exclude-per-directory argument must be .gitignore");
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ debug_stage(ଅଚଳ ଅକ୍ଷର *label, ଅଚଳ ସଂରଚନା cache_entry *ce,
			ସଂରଚନା unpack_trees_options *o)
{
	printf("%s ", label);
	ଯଦି (!ce)
		printf("(missing)\n");
	ଅନ୍ୟଥା ଯଦି (ce == o->df_conflict_entry)
		printf("(conflict)\n");
	ଅନ୍ୟଥା
		printf("%06o #%d %s %.8s\n",
		       ce->ce_mode, ce_stage(ce), ce->name,
		       oid_to_hex(&ce->oid));
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା debug_merge(ଅଚଳ ସଂରଚନା cache_entry * ଅଚଳ *stages,
		       ସଂରଚନା unpack_trees_options *o)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	printf("* %d-way merge\n", o->internal.merge_size);
	debug_stage("index", stages[0], o);
	ପାଇଁ (i = 1; i <= o->internal.merge_size; i++) {
		ଅକ୍ଷର buf[24];
		xsnprintf(buf, ଆକାର(buf), "ent#%d", i);
		debug_stage(buf, stages[i], o);
	}
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା git_read_ବୃକ୍ଷ_config(ଅଚଳ ଅକ୍ଷର *var, ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ,
				ଅଚଳ ସଂରଚନା config_context *ctx, ଶୂନ୍ୟ *cb)
{
	ଯଦି (!strcmp(var, "submodule.recurse"))
		ଫେରନ୍ତୁ git_default_submodule_config(var, ମୂଲ୍ୟ, cb);

	ଫେରନ୍ତୁ git_default_config(var, ମୂଲ୍ୟ, ctx, cb);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_read_ବୃକ୍ଷ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
		  ଅଚଳ ଅକ୍ଷର **argv,
		  ଅଚଳ ଅକ୍ଷର *cmd_prefix,
		  ସଂରଚନା repository *repo UNUSED)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, stage = 0;
	ସଂରଚନା object_id oid;
	ସଂରଚନା ବୃକ୍ଷ_desc t[MAX_UNPACK_TREES];
	ସଂରଚନା unpack_trees_options opts;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା prefix_set = 0;
	ସଂରଚନା lock_file lock_file = LOCK_INIT;
	ଅଚଳ ସଂରଚନା option read_ବୃକ୍ଷ_options[] = {
		OPT__SUPER_PREFIX(&opts.super_prefix),
		OPT_CALLBACK_F(0, "index-output", ଖାଲି, N_("file"),
		  N_("write resulting index to <file>"),
		  PARSE_OPT_NONEG, index_output_cb),
		OPT_BOOL(0, "empty", &read_empty,
			    N_("only empty the index")),
		OPT__VERBOSE(&opts.verbose_update, N_("be verbose")),
		OPT_GROUP(N_("Merging")),
		OPT_BOOL('m', ଖାଲି, &opts.merge,
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
			.ମୂଲ୍ୟ = &opts.prefix,
			.argh = N_("<subdirectory>/"),
			.help = N_("read the tree into the index under <subdirectory>/"),
			.flags = PARSE_OPT_NONEG,
		},
		OPT_BOOL('u', ଖାଲି, &opts.update,
			 N_("update working tree with merge result")),
		OPT_CALLBACK_F(0, "exclude-per-directory", &opts,
		  N_("gitignore"),
		  N_("allow explicitly ignored files to be overwritten"),
		  PARSE_OPT_NONEG, exclude_per_directory_cb),
		OPT_BOOL('i', ଖାଲି, &opts.index_only,
			 N_("don't check the working tree after merging")),
		OPT__DRY_RUN(&opts.dry_run, N_("don't update the index or the work tree")),
		OPT_BOOL(0, "no-sparse-checkout", &opts.skip_sparse_checkout,
			 N_("skip applying sparse checkout filter")),
		OPT_BOOL(0, "debug-unpack", &opts.internal.debug_unpack,
			 N_("debug unpack-trees")),
		OPT_CALLBACK_F(0, "recurse-submodules", ଖାଲି,
			    "checkout", "control recursive updating of submodules",
			    PARSE_OPT_OPTARG, option_parse_recurse_submodules_worktree_updater),
		OPT__QUIET(&opts.quiet, N_("suppress feedback messages")),
		OPT_END()
	};

	memset(&opts, 0, ଆକାର(opts));
	opts.head_idx = -1;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;

	repo_config(the_repository, git_read_ବୃକ୍ଷ_config, ଖାଲି);

	argc = parse_options(argc, argv, cmd_prefix, read_ବୃକ୍ଷ_options,
			     read_ବୃକ୍ଷ_usage, 0);

	prefix_set = opts.prefix ? 1 : 0;
	ଯଦି (1 < opts.merge + opts.reset + prefix_set)
		die("Which one? -m, --reset, or --prefix?");

	/* Prefix should not start with a directory separator */
	ଯଦି (opts.prefix && opts.prefix[0] == '/')
		die("Invalid prefix, prefix cannot start with '/'");

	ଯଦି (opts.reset)
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

	ଯଦି (opts.reset || opts.merge || opts.prefix) {
		ଯଦି (repo_read_index_unmerged(the_repository) && (opts.prefix || opts.merge))
			die(_("You need to resolve your current index first"));
		stage = opts.merge = 1;
	}
	resolve_undo_clear_index(the_repository->index);

	ପାଇଁ (i = 0; i < argc; i++) {
		ଅଚଳ ଅକ୍ଷର *arg = argv[i];

		ଯଦି (repo_get_oid(the_repository, arg, &oid))
			die("Not a valid object name %s", arg);
		ଯଦି (list_ବୃକ୍ଷ(&oid) < 0)
			die("failed to unpack tree object %s", arg);
		stage++;
	}
	ଯଦି (!nr_trees && !read_empty && !opts.merge)
		warning("read-tree: emptying the index with no arguments is deprecated; use --empty");
	ଅନ୍ୟଥା ଯଦି (nr_trees > 0 && read_empty)
		die("passing trees as arguments contradicts --empty");

	ଯଦି (1 < opts.index_only + opts.update)
		die("-u and -i at the same time makes no sense");
	ଯଦି ((opts.update || opts.index_only) && !opts.merge)
		die("%s is meaningless without -m, --reset, or --prefix",
		    opts.update ? "-u" : "-i");
	ଯଦି (opts.update && !opts.reset)
		opts.preserve_ignored = 0;
	/* otherwise, opts.preserve_ignored is irrelevant */
	ଯଦି (opts.merge && !opts.index_only)
		setup_work_ବୃକ୍ଷ(the_repository);

	ଯଦି (opts.skip_sparse_checkout)
		ensure_full_index(the_repository->index);

	ଯଦି (opts.merge) {
		ବାଛ (stage - 1) {
		ସ୍ଥିତି 0:
			die("you must specify at least one tree to merge");
			ଭାଙ୍ଗ;
		ସ୍ଥିତି 1:
			opts.fn = opts.prefix ? bind_merge : oneway_merge;
			ଭାଙ୍ଗ;
		ସ୍ଥିତି 2:
			opts.fn = twoway_merge;
			opts.initial_checkout = is_index_unborn(the_repository->index);
			ଭାଙ୍ଗ;
		ସ୍ଥିତି 3:
		ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
			opts.fn = threeway_merge;
			ଭାଙ୍ଗ;
		}

		ଯଦି (stage - 1 >= 3)
			opts.head_idx = stage - 2;
		ଅନ୍ୟଥା
			opts.head_idx = 1;
	}

	ଯଦି (opts.internal.debug_unpack)
		opts.fn = debug_merge;

	/* If we're going to prime_cache_tree later, skip cache tree update */
	ଯଦି (nr_trees == 1 && !opts.prefix)
		opts.skip_cache_ବୃକ୍ଷ_update = 1;

	cache_ବୃକ୍ଷ_free(&the_repository->index->cache_ବୃକ୍ଷ);
	ପାଇଁ (i = 0; i < nr_trees; i++) {
		ସଂରଚନା ବୃକ୍ଷ *ବୃକ୍ଷ = trees[i];
		ଯଦି (repo_parse_ବୃକ୍ଷ(the_repository, ବୃକ୍ଷ) < 0)
			ଫେରନ୍ତୁ 128;
		init_ବୃକ୍ଷ_desc(t+i, &ବୃକ୍ଷ->object.oid, ବୃକ୍ଷ->buffer, ବୃକ୍ଷ->size);
	}
	ଯଦି (unpack_trees(nr_trees, t, &opts))
		ଫେରନ୍ତୁ 128;

	ଯଦି (opts.internal.debug_unpack || opts.dry_run)
		ଫେରନ୍ତୁ 0; /* do not write the index out */

	/*
	 * When reading only one tree (either the most basic form,
	 * "-m ent" or "--reset ent" form), we can obtain a fully
	 * valid cache-tree because the index must match exactly
	 * what came from the tree.
	 */
	ଯଦି (nr_trees == 1 && !opts.prefix)
		ମୌଳିକ_cache_ବୃକ୍ଷ(the_repository,
				 the_repository->index,
				 trees[0]);

	ଯଦି (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
		die("unable to write new index file");
	ଫେରନ୍ତୁ 0;
}
