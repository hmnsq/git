/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
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

ساکن صحیح nr_trees;
ساکن صحیح read_خالی_خانے;
ساکن ساخت درخت *trees[MAX_UNPACK_TREES];

ساکن صحیح list_درخت(ساخت object_id *oid)
{
	ساخت درخت *درخت;

	اگر (nr_trees >= MAX_UNPACK_TREES)
		die("I cannot read more than %d trees", MAX_UNPACK_TREES);
	درخت = repo_parse_درخت_بالواسطہ(the_repository, oid);
	اگر (!درخت)
		واپس -1;
	trees[nr_trees++] = درخت;
	واپس 0;
}

ساکن ثابت حرف * ثابت read_درخت_usage[] = {
	N_("git read-tree [(-m [--trivial] [--aggressive] | --reset | --prefix=<prefix>)\n"
	   "              [-u | -i]] [--index-output=<file>] [--no-sparse-checkout]\n"
	   "              (--empty | <tree-ish1> [<tree-ish2> [<tree-ish3>]])"),
	تہی
};

ساکن صحیح اشاریہ_output_cb(ثابت ساخت option *opt غیر_مستعمل, ثابت حرف *arg,
				 صحیح unset)
{
	BUG_ON_OPT_NEG(unset);
	سیٹ_alternate_اشاریہ_output(arg);
	واپس 0;
}

ساکن صحیح exclude_فی_directory_cb(ثابت ساخت option *opt, ثابت حرف *arg,
				    صحیح unset)
{
	ساخت unpack_trees_options *opts;

	BUG_ON_OPT_NEG(unset);

	opts = (ساخت unpack_trees_options *)opt->قدر;

	اگر (!opts->update)
		die("--exclude-per-directory is meaningless unless -u");
	اگر (strcmp(arg, ".gitignore"))
		die("--exclude-per-directory argument must be .gitignore");
	واپس 0;
}

ساکن خالی debug_stage(ثابت حرف *label, ثابت ساخت cache_اندراج *ce,
			ساخت unpack_trees_options *o)
{
	لکھو("%s ", label);
	اگر (!ce)
		لکھو("(missing)\n");
	ورنہ اگر (ce == o->df_conflict_اندراج)
		لکھو("(conflict)\n");
	ورنہ
		لکھو("%06o #%d %s %.8s\n",
		       ce->ce_منوال, ce_stage(ce), ce->name,
		       oid_مقصد_hex(&ce->oid));
}

ساکن صحیح debug_ملاؤ(ثابت ساخت cache_اندراج * ثابت *stages,
		       ساخت unpack_trees_options *o)
{
	صحیح i;

	لکھو("* %d-way merge\n", o->اندرونی_2.ملاؤ_سائز);
	debug_stage("index", stages[0], o);
	برائے (i = 1; i <= o->اندرونی_2.ملاؤ_سائز; i++) {
		حرف buf[24];
		xsnprintf(buf, حجم(buf), "ent#%d", i);
		debug_stage(buf, stages[i], o);
	}
	واپس 0;
}

ساکن صحیح git_read_درخت_config(ثابت حرف *var, ثابت حرف *قدر,
				ثابت ساخت config_context *ctx, خالی *cb)
{
	اگر (!strcmp(var, "submodule.recurse"))
		واپس git_default_submodule_config(var, قدر, cb);

	واپس git_default_config(var, قدر, ctx, cb);
}

صحیح cmd_read_درخت(صحیح argc,
		  ثابت حرف **argv,
		  ثابت حرف *cmd_سابقہ,
		  ساخت repository *repo غیر_مستعمل)
{
	صحیح i, stage = 0;
	ساخت object_id oid;
	ساخت درخت_desc t[MAX_UNPACK_TREES];
	ساخت unpack_trees_options opts;
	صحیح سابقہ_سیٹ = 0;
	ساخت تالا_file تالا_file = تالا_INIT;
	ثابت ساخت option read_درخت_options[] = {
		OPT_SUPER_سابقہ(&opts.super_سابقہ),
		OPT_CALLBACK_F(0, "index-output", تہی, N_("file"),
		  N_("write resulting index to <file>"),
		  PARSE_OPT_NONEG, اشاریہ_output_cb),
		OPT_BOOL(0, "empty", &read_خالی_خانے,
			    N_("only empty the index")),
		OPT__VERBOSE(&opts.verbose_update, N_("be verbose")),
		OPT_گروہ(N_("Merging")),
		OPT_BOOL('m', تہی, &opts.ملاؤ,
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
			.قدر = &opts.سابقہ,
			.argh = N_("<subdirectory>/"),
			.help = N_("read the tree into the index under <subdirectory>/"),
			.flags = PARSE_OPT_NONEG,
		},
		OPT_BOOL('u', تہی, &opts.update,
			 N_("update working tree with merge result")),
		OPT_CALLBACK_F(0, "exclude-per-directory", &opts,
		  N_("gitignore"),
		  N_("allow explicitly ignored files to be overwritten"),
		  PARSE_OPT_NONEG, exclude_فی_directory_cb),
		OPT_BOOL('i', تہی, &opts.اشاریہ_only,
			 N_("don't check the working tree after merging")),
		OPT_DRY_تسلسل(&opts.dry_تسلسل, N_("don't update the index or the work tree")),
		OPT_BOOL(0, "no-sparse-checkout", &opts.skip_sparse_checkout,
			 N_("skip applying sparse checkout filter")),
		OPT_BOOL(0, "debug-unpack", &opts.اندرونی_2.debug_unpack,
			 N_("debug unpack-trees")),
		OPT_CALLBACK_F(0, "recurse-submodules", تہی,
			    "checkout", "control recursive updating of submodules",
			    PARSE_OPT_OPTARG, option_parse_recurse_submodules_worktree_updater),
		OPT__QUIET(&opts.quiet, N_("suppress feedback messages")),
		OPT_انتہا()
	};

	memset(&opts, 0, حجم(opts));
	opts.سر_idx = -1;
	opts.src_اشاریہ = the_repository->اشاریہ;
	opts.dst_اشاریہ = the_repository->اشاریہ;

	repo_config(the_repository, git_read_درخت_config, تہی);

	argc = parse_options(argc, argv, cmd_سابقہ, read_درخت_options,
			     read_درخت_usage, 0);

	سابقہ_سیٹ = opts.سابقہ ? 1 : 0;
	اگر (1 < opts.ملاؤ + opts.reset + سابقہ_سیٹ)
		die("Which one? -m, --reset, or --prefix?");

	/* Prefix should not start with a directory separator */
	اگر (opts.سابقہ && opts.سابقہ[0] == '/')
		die("Invalid prefix, prefix cannot start with '/'");

	اگر (opts.reset)
		opts.reset = UNPACK_RESET_OVERWRITE_UNTRACKED;

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_بھرا_اشاریہ = 0;

	repo_hold_locked_اشاریہ(the_repository, &تالا_file, تالا_DIE_ON_ERROR);

	/*
	 * NEEDSWORK
	 *
	 * The old index should be read anyway even if we're going to
	 * destroy all index entries because we still need to preserve
	 * certain information such as index version or split-index
	 * mode.
	 */

	اگر (opts.reset || opts.ملاؤ || opts.سابقہ) {
		اگر (repo_read_اشاریہ_unmerged(the_repository) && (opts.سابقہ || opts.ملاؤ))
			die(_("You need to resolve your current index first"));
		stage = opts.ملاؤ = 1;
	}
	resolve_undo_clear_اشاریہ(the_repository->اشاریہ);

	برائے (i = 0; i < argc; i++) {
		ثابت حرف *arg = argv[i];

		اگر (repo_get_oid(the_repository, arg, &oid))
			die("Not a valid object name %s", arg);
		اگر (list_درخت(&oid) < 0)
			die("failed to unpack tree object %s", arg);
		stage++;
	}
	اگر (!nr_trees && !read_خالی_خانے && !opts.ملاؤ)
		warning("read-tree: emptying the index with no arguments is deprecated; use --empty");
	ورنہ اگر (nr_trees > 0 && read_خالی_خانے)
		die("passing trees as arguments contradicts --empty");

	اگر (1 < opts.اشاریہ_only + opts.update)
		die("-u and -i at the same time makes no sense");
	اگر ((opts.update || opts.اشاریہ_only) && !opts.ملاؤ)
		die("%s is meaningless without -m, --reset, or --prefix",
		    opts.update ? "-u" : "-i");
	اگر (opts.update && !opts.reset)
		opts.preserve_ignored = 0;
	/* otherwise, opts.preserve_ignored is irrelevant */
	اگر (opts.ملاؤ && !opts.اشاریہ_only)
		تیاری_work_درخت(the_repository);

	اگر (opts.skip_sparse_checkout)
		ensure_بھرا_اشاریہ(the_repository->اشاریہ);

	اگر (opts.ملاؤ) {
		انتخاب (stage - 1) {
		حالت 0:
			die("you must specify at least one tree to merge");
			توڑو;
		حالت 1:
			opts.fn = opts.سابقہ ? bind_ملاؤ : oneway_ملاؤ;
			توڑو;
		حالت 2:
			opts.fn = twoway_ملاؤ;
			opts.initial_checkout = is_اشاریہ_unborn(the_repository->اشاریہ);
			توڑو;
		حالت 3:
		طےشدہ:
			opts.fn = threeway_ملاؤ;
			توڑو;
		}

		اگر (stage - 1 >= 3)
			opts.سر_idx = stage - 2;
		ورنہ
			opts.سر_idx = 1;
	}

	اگر (opts.اندرونی_2.debug_unpack)
		opts.fn = debug_ملاؤ;

	/* If we're going to prime_cache_tree later, skip cache tree update */
	اگر (nr_trees == 1 && !opts.سابقہ)
		opts.skip_cache_درخت_update = 1;

	cache_درخت_free(&the_repository->اشاریہ->cache_درخت);
	برائے (i = 0; i < nr_trees; i++) {
		ساخت درخت *درخت = trees[i];
		اگر (repo_parse_درخت(the_repository, درخت) < 0)
			واپس 128;
		init_درخت_desc(t+i, &درخت->object.oid, درخت->بفر, درخت->سائز);
	}
	اگر (unpack_trees(nr_trees, t, &opts))
		واپس 128;

	اگر (opts.اندرونی_2.debug_unpack || opts.dry_تسلسل)
		واپس 0; /* do not write the index out */

	/*
	 * When reading only one tree (either the most basic form,
	 * "-m ent" or "--reset ent" form), we can obtain a fully
	 * valid cache-tree because the index must match exactly
	 * what came from the tree.
	 */
	اگر (nr_trees == 1 && !opts.سابقہ)
		مفرد_cache_درخت(the_repository,
				 the_repository->اشاریہ,
				 trees[0]);

	اگر (write_locked_اشاریہ(the_repository->اشاریہ, &تالا_file, COMMIT_تالا))
		die("unable to write new index file");
	واپس 0;
}
