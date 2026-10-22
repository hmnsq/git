/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hex.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "lockfile.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-name.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "tree.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "tree-walk.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cache-tree.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "unpack-trees.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "resolve-undo.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "setup.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "sparse-index.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "submodule.h"

ਸਥਿਰ ਪੂਰਨ nr_trees;
ਸਥਿਰ ਪੂਰਨ read_ਖਾਲੀ_ਥਾਵਾਂ;
ਸਥਿਰ ਬਣਤਰ ਰੁੱਖ *trees[MAX_UNPACK_TREES];

ਸਥਿਰ ਪੂਰਨ list_ਰੁੱਖ(ਬਣਤਰ object_id *oid)
{
	ਬਣਤਰ ਰੁੱਖ *ਰੁੱਖ;

	ਜੇ (nr_trees >= MAX_UNPACK_TREES)
		die("I cannot read more than %d trees", MAX_UNPACK_TREES);
	ਰੁੱਖ = repo_parse_ਰੁੱਖ_ਅਸਿੱਧਾ(the_repository, oid);
	ਜੇ (!ਰੁੱਖ)
		ਵਾਪਸ -1;
	trees[nr_trees++] = ਰੁੱਖ;
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ read_ਰੁੱਖ_usage[] = {
	N_("git read-tree [(-m [--trivial] [--aggressive] | --reset | --prefix=<prefix>)\n"
	   "              [-u | -i]] [--index-output=<file>] [--no-sparse-checkout]\n"
	   "              (--empty | <tree-ish1> [<tree-ish2> [<tree-ish3>]])"),
	ਨਿਰਾ
};

ਸਥਿਰ ਪੂਰਨ ਸੂਚਕਾਂਕ_output_cb(ਅਚਲ ਬਣਤਰ option *opt ਅਣਵਰਤਿਆ, ਅਚਲ ਅੱਖਰ *arg,
				 ਪੂਰਨ unset)
{
	BUG_ON_OPT_NEG(unset);
	ਸੈੱਟ_alternate_ਸੂਚਕਾਂਕ_output(arg);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ exclude_ਪ੍ਰਤੀ_directory_cb(ਅਚਲ ਬਣਤਰ option *opt, ਅਚਲ ਅੱਖਰ *arg,
				    ਪੂਰਨ unset)
{
	ਬਣਤਰ unpack_trees_options *opts;

	BUG_ON_OPT_NEG(unset);

	opts = (ਬਣਤਰ unpack_trees_options *)opt->ਮੁੱਲ;

	ਜੇ (!opts->update)
		die("--exclude-per-directory is meaningless unless -u");
	ਜੇ (strcmp(arg, ".gitignore"))
		die("--exclude-per-directory argument must be .gitignore");
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ debug_stage(ਅਚਲ ਅੱਖਰ *label, ਅਚਲ ਬਣਤਰ cache_ਇੰਦਰਾਜ *ce,
			ਬਣਤਰ unpack_trees_options *o)
{
	ਲਿਖੋ("%s ", label);
	ਜੇ (!ce)
		ਲਿਖੋ("(missing)\n");
	ਨਹੀਂਤਾਂ ਜੇ (ce == o->df_conflict_ਇੰਦਰਾਜ)
		ਲਿਖੋ("(conflict)\n");
	ਨਹੀਂਤਾਂ
		ਲਿਖੋ("%06o #%d %s %.8s\n",
		       ce->ce_ਬਹੁਲਕ, ce_stage(ce), ce->name,
		       oid_ਮੰਜ਼ਿਲ_hex(&ce->oid));
}

ਸਥਿਰ ਪੂਰਨ debug_ਮਿਲਾਓ(ਅਚਲ ਬਣਤਰ cache_ਇੰਦਰਾਜ * ਅਚਲ *stages,
		       ਬਣਤਰ unpack_trees_options *o)
{
	ਪੂਰਨ i;

	ਲਿਖੋ("* %d-way merge\n", o->ਅੰਦਰੂਨੀ.ਮਿਲਾਓ_ਮਾਪ);
	debug_stage("index", stages[0], o);
	ਲਈ (i = 1; i <= o->ਅੰਦਰੂਨੀ.ਮਿਲਾਓ_ਮਾਪ; i++) {
		ਅੱਖਰ buf[24];
		xsnprintf(buf, ਆਕਾਰ(buf), "ent#%d", i);
		debug_stage(buf, stages[i], o);
	}
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ git_read_ਰੁੱਖ_config(ਅਚਲ ਅੱਖਰ *var, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ,
				ਅਚਲ ਬਣਤਰ config_context *ctx, ਖਾਲੀ *cb)
{
	ਜੇ (!strcmp(var, "submodule.recurse"))
		ਵਾਪਸ git_default_submodule_config(var, ਮੁੱਲ, cb);

	ਵਾਪਸ git_default_config(var, ਮੁੱਲ, ctx, cb);
}

ਪੂਰਨ cmd_read_ਰੁੱਖ(ਪੂਰਨ argc,
		  ਅਚਲ ਅੱਖਰ **argv,
		  ਅਚਲ ਅੱਖਰ *cmd_ਅਗੇਤਰ,
		  ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ i, stage = 0;
	ਬਣਤਰ object_id oid;
	ਬਣਤਰ ਰੁੱਖ_desc t[MAX_UNPACK_TREES];
	ਬਣਤਰ unpack_trees_options opts;
	ਪੂਰਨ ਅਗੇਤਰ_ਸੈੱਟ = 0;
	ਬਣਤਰ ਤਾਲਾ_file ਤਾਲਾ_file = ਤਾਲਾ_INIT;
	ਅਚਲ ਬਣਤਰ option read_ਰੁੱਖ_options[] = {
		OPT_SUPER_ਅਗੇਤਰ(&opts.super_ਅਗੇਤਰ),
		OPT_CALLBACK_F(0, "index-output", ਨਿਰਾ, N_("file"),
		  N_("write resulting index to <file>"),
		  PARSE_OPT_NONEG, ਸੂਚਕਾਂਕ_output_cb),
		OPT_BOOL(0, "empty", &read_ਖਾਲੀ_ਥਾਵਾਂ,
			    N_("only empty the index")),
		OPT__VERBOSE(&opts.verbose_update, N_("be verbose")),
		OPT_ਸਮੂਹ(N_("Merging")),
		OPT_BOOL('m', ਨਿਰਾ, &opts.ਮਿਲਾਓ,
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
			.ਮੁੱਲ = &opts.ਅਗੇਤਰ,
			.argh = N_("<subdirectory>/"),
			.help = N_("read the tree into the index under <subdirectory>/"),
			.flags = PARSE_OPT_NONEG,
		},
		OPT_BOOL('u', ਨਿਰਾ, &opts.update,
			 N_("update working tree with merge result")),
		OPT_CALLBACK_F(0, "exclude-per-directory", &opts,
		  N_("gitignore"),
		  N_("allow explicitly ignored files to be overwritten"),
		  PARSE_OPT_NONEG, exclude_ਪ੍ਰਤੀ_directory_cb),
		OPT_BOOL('i', ਨਿਰਾ, &opts.ਸੂਚਕਾਂਕ_only,
			 N_("don't check the working tree after merging")),
		OPT_DRY_ਲੜੀਵਾਰ(&opts.dry_ਲੜੀਵਾਰ, N_("don't update the index or the work tree")),
		OPT_BOOL(0, "no-sparse-checkout", &opts.skip_sparse_checkout,
			 N_("skip applying sparse checkout filter")),
		OPT_BOOL(0, "debug-unpack", &opts.ਅੰਦਰੂਨੀ.debug_unpack,
			 N_("debug unpack-trees")),
		OPT_CALLBACK_F(0, "recurse-submodules", ਨਿਰਾ,
			    "checkout", "control recursive updating of submodules",
			    PARSE_OPT_OPTARG, option_parse_recurse_submodules_worktree_updater),
		OPT__QUIET(&opts.quiet, N_("suppress feedback messages")),
		OPT_ਸਿਰਾ()
	};

	memset(&opts, 0, ਆਕਾਰ(opts));
	opts.ਸਿਰ_idx = -1;
	opts.src_ਸੂਚਕਾਂਕ = the_repository->ਸੂਚਕਾਂਕ;
	opts.dst_ਸੂਚਕਾਂਕ = the_repository->ਸੂਚਕਾਂਕ;

	repo_config(the_repository, git_read_ਰੁੱਖ_config, ਨਿਰਾ);

	argc = parse_options(argc, argv, cmd_ਅਗੇਤਰ, read_ਰੁੱਖ_options,
			     read_ਰੁੱਖ_usage, 0);

	ਅਗੇਤਰ_ਸੈੱਟ = opts.ਅਗੇਤਰ ? 1 : 0;
	ਜੇ (1 < opts.ਮਿਲਾਓ + opts.reset + ਅਗੇਤਰ_ਸੈੱਟ)
		die("Which one? -m, --reset, or --prefix?");

	/* Prefix should not start with a directory separator */
	ਜੇ (opts.ਅਗੇਤਰ && opts.ਅਗੇਤਰ[0] == '/')
		die("Invalid prefix, prefix cannot start with '/'");

	ਜੇ (opts.reset)
		opts.reset = UNPACK_RESET_OVERWRITE_UNTRACKED;

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_ਭਰਿਆ_ਸੂਚਕਾਂਕ = 0;

	repo_hold_locked_ਸੂਚਕਾਂਕ(the_repository, &ਤਾਲਾ_file, ਤਾਲਾ_DIE_ON_ERROR);

	/*
	 * NEEDSWORK
	 *
	 * The old index should be read anyway even if we're going to
	 * destroy all index entries because we still need to preserve
	 * certain information such as index version or split-index
	 * mode.
	 */

	ਜੇ (opts.reset || opts.ਮਿਲਾਓ || opts.ਅਗੇਤਰ) {
		ਜੇ (repo_read_ਸੂਚਕਾਂਕ_unmerged(the_repository) && (opts.ਅਗੇਤਰ || opts.ਮਿਲਾਓ))
			die(_("You need to resolve your current index first"));
		stage = opts.ਮਿਲਾਓ = 1;
	}
	resolve_undo_clear_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ);

	ਲਈ (i = 0; i < argc; i++) {
		ਅਚਲ ਅੱਖਰ *arg = argv[i];

		ਜੇ (repo_get_oid(the_repository, arg, &oid))
			die("Not a valid object name %s", arg);
		ਜੇ (list_ਰੁੱਖ(&oid) < 0)
			die("failed to unpack tree object %s", arg);
		stage++;
	}
	ਜੇ (!nr_trees && !read_ਖਾਲੀ_ਥਾਵਾਂ && !opts.ਮਿਲਾਓ)
		warning("read-tree: emptying the index with no arguments is deprecated; use --empty");
	ਨਹੀਂਤਾਂ ਜੇ (nr_trees > 0 && read_ਖਾਲੀ_ਥਾਵਾਂ)
		die("passing trees as arguments contradicts --empty");

	ਜੇ (1 < opts.ਸੂਚਕਾਂਕ_only + opts.update)
		die("-u and -i at the same time makes no sense");
	ਜੇ ((opts.update || opts.ਸੂਚਕਾਂਕ_only) && !opts.ਮਿਲਾਓ)
		die("%s is meaningless without -m, --reset, or --prefix",
		    opts.update ? "-u" : "-i");
	ਜੇ (opts.update && !opts.reset)
		opts.preserve_ignored = 0;
	/* otherwise, opts.preserve_ignored is irrelevant */
	ਜੇ (opts.ਮਿਲਾਓ && !opts.ਸੂਚਕਾਂਕ_only)
		ਤਿਆਰੀ_work_ਰੁੱਖ(the_repository);

	ਜੇ (opts.skip_sparse_checkout)
		ensure_ਭਰਿਆ_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ);

	ਜੇ (opts.ਮਿਲਾਓ) {
		ਚੋਣ (stage - 1) {
		ਹਾਲਤ 0:
			die("you must specify at least one tree to merge");
			ਰੋਕੋ;
		ਹਾਲਤ 1:
			opts.fn = opts.ਅਗੇਤਰ ? bind_ਮਿਲਾਓ : oneway_ਮਿਲਾਓ;
			ਰੋਕੋ;
		ਹਾਲਤ 2:
			opts.fn = twoway_ਮਿਲਾਓ;
			opts.initial_checkout = is_ਸੂਚਕਾਂਕ_unborn(the_repository->ਸੂਚਕਾਂਕ);
			ਰੋਕੋ;
		ਹਾਲਤ 3:
		ਮੂਲ:
			opts.fn = threeway_ਮਿਲਾਓ;
			ਰੋਕੋ;
		}

		ਜੇ (stage - 1 >= 3)
			opts.ਸਿਰ_idx = stage - 2;
		ਨਹੀਂਤਾਂ
			opts.ਸਿਰ_idx = 1;
	}

	ਜੇ (opts.ਅੰਦਰੂਨੀ.debug_unpack)
		opts.fn = debug_ਮਿਲਾਓ;

	/* If we're going to prime_cache_tree later, skip cache tree update */
	ਜੇ (nr_trees == 1 && !opts.ਅਗੇਤਰ)
		opts.skip_cache_ਰੁੱਖ_update = 1;

	cache_ਰੁੱਖ_free(&the_repository->ਸੂਚਕਾਂਕ->cache_ਰੁੱਖ);
	ਲਈ (i = 0; i < nr_trees; i++) {
		ਬਣਤਰ ਰੁੱਖ *ਰੁੱਖ = trees[i];
		ਜੇ (repo_parse_ਰੁੱਖ(the_repository, ਰੁੱਖ) < 0)
			ਵਾਪਸ 128;
		init_ਰੁੱਖ_desc(t+i, &ਰੁੱਖ->object.oid, ਰੁੱਖ->ਬਫਰ, ਰੁੱਖ->ਮਾਪ);
	}
	ਜੇ (unpack_trees(nr_trees, t, &opts))
		ਵਾਪਸ 128;

	ਜੇ (opts.ਅੰਦਰੂਨੀ.debug_unpack || opts.dry_ਲੜੀਵਾਰ)
		ਵਾਪਸ 0; /* do not write the index out */

	/*
	 * When reading only one tree (either the most basic form,
	 * "-m ent" or "--reset ent" form), we can obtain a fully
	 * valid cache-tree because the index must match exactly
	 * what came from the tree.
	 */
	ਜੇ (nr_trees == 1 && !opts.ਅਗੇਤਰ)
		ਅਭਾਜ_cache_ਰੁੱਖ(the_repository,
				 the_repository->ਸੂਚਕਾਂਕ,
				 trees[0]);

	ਜੇ (write_locked_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ, &ਤਾਲਾ_file, COMMIT_ਤਾਲਾ))
		die("unable to write new index file");
	ਵਾਪਸ 0;
}
