/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#تعریف_کړه USE_THE_REPOSITORY_VARIABLE
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
ساكن صحيح read_تشه;
ساكن بنية ونه *trees[MAX_UNPACK_TREES];

ساكن صحيح list_ونه(بنية object_id *oid)
{
	بنية ونه *ونه;

	إذا (nr_trees >= MAX_UNPACK_TREES)
		die("I cannot read more than %d trees", MAX_UNPACK_TREES);
	ونه = repo_parse_ونه_غیرمستقیم(the_repository, oid);
	إذا (!ونه)
		أرجع -1;
	trees[nr_trees++] = ونه;
	أرجع 0;
}

ساكن ثابت حرف * ثابت read_ونه_usage[] = {
	N_("git read-tree [(-m [--trivial] [--aggressive] | --reset | --prefix=<prefix>)\n"
	   "              [-u | -i]] [--index-output=<file>] [--no-sparse-checkout]\n"
	   "              (--empty | <tree-ish1> [<tree-ish2> [<tree-ish3>]])"),
	عدم
};

ساكن صحيح شاخص_output_cb(ثابت بنية option *opt ناکارول_شوی, ثابت حرف *arg,
				 صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	سېټ_alternate_شاخص_output(arg);
	أرجع 0;
}

ساكن صحيح exclude_فی_directory_cb(ثابت بنية option *opt, ثابت حرف *arg,
				    صحيح unset)
{
	بنية unpack_trees_options *opts;

	BUG_ON_OPT_NEG(unset);

	opts = (بنية unpack_trees_options *)opt->ارزښت;

	إذا (!opts->update)
		die("--exclude-per-directory is meaningless unless -u");
	إذا (strcmp(arg, ".gitignore"))
		die("--exclude-per-directory argument must be .gitignore");
	أرجع 0;
}

ساكن فراغ debug_stage(ثابت حرف *label, ثابت بنية cache_ثبت *ce,
			بنية unpack_trees_options *o)
{
	چاپ_کړه("%s ", label);
	إذا (!ce)
		چاپ_کړه("(missing)\n");
	وإلا إذا (ce == o->df_conflict_ثبت)
		چاپ_کړه("(conflict)\n");
	وإلا
		چاپ_کړه("%06o #%d %s %.8s\n",
		       ce->ce_ډېرپېښ, ce_stage(ce), ce->name,
		       oid_منزل_hex(&ce->oid));
}

ساكن صحيح debug_یوځای(ثابت بنية cache_ثبت * ثابت *stages,
		       بنية unpack_trees_options *o)
{
	صحيح i;

	چاپ_کړه("* %d-way merge\n", o->دننی.یوځای_اندازه);
	debug_stage("index", stages[0], o);
	لكل (i = 1; i <= o->دننی.یوځای_اندازه; i++) {
		حرف buf[24];
		xsnprintf(buf, حجم(buf), "ent#%d", i);
		debug_stage(buf, stages[i], o);
	}
	أرجع 0;
}

ساكن صحيح git_read_ونه_config(ثابت حرف *var, ثابت حرف *ارزښت,
				ثابت بنية config_context *ctx, فراغ *cb)
{
	إذا (!strcmp(var, "submodule.recurse"))
		أرجع git_default_submodule_config(var, ارزښت, cb);

	أرجع git_default_config(var, ارزښت, ctx, cb);
}

صحيح cmd_read_ونه(صحيح argc,
		  ثابت حرف **argv,
		  ثابت حرف *cmd_مختاړی,
		  بنية repository *repo ناکارول_شوی)
{
	صحيح i, stage = 0;
	بنية object_id oid;
	بنية ونه_desc t[MAX_UNPACK_TREES];
	بنية unpack_trees_options opts;
	صحيح مختاړی_سېټ = 0;
	بنية قلف_file قلف_file = قلف_INIT;
	ثابت بنية option read_ونه_options[] = {
		OPT_SUPER_مختاړی(&opts.super_مختاړی),
		OPT_CALLBACK_F(0, "index-output", عدم, N_("file"),
		  N_("write resulting index to <file>"),
		  PARSE_OPT_NONEG, شاخص_output_cb),
		OPT_BOOL(0, "empty", &read_تشه,
			    N_("only empty the index")),
		OPT__VERBOSE(&opts.verbose_update, N_("be verbose")),
		OPT_ډله(N_("Merging")),
		OPT_BOOL('m', عدم, &opts.یوځای,
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
			.ارزښت = &opts.مختاړی,
			.argh = N_("<subdirectory>/"),
			.help = N_("read the tree into the index under <subdirectory>/"),
			.flags = PARSE_OPT_NONEG,
		},
		OPT_BOOL('u', عدم, &opts.update,
			 N_("update working tree with merge result")),
		OPT_CALLBACK_F(0, "exclude-per-directory", &opts,
		  N_("gitignore"),
		  N_("allow explicitly ignored files to be overwritten"),
		  PARSE_OPT_NONEG, exclude_فی_directory_cb),
		OPT_BOOL('i', عدم, &opts.شاخص_only,
			 N_("don't check the working tree after merging")),
		OPT_DRY_پرلپسې(&opts.dry_پرلپسې, N_("don't update the index or the work tree")),
		OPT_BOOL(0, "no-sparse-checkout", &opts.skip_sparse_checkout,
			 N_("skip applying sparse checkout filter")),
		OPT_BOOL(0, "debug-unpack", &opts.دننی.debug_unpack,
			 N_("debug unpack-trees")),
		OPT_CALLBACK_F(0, "recurse-submodules", عدم,
			    "checkout", "control recursive updating of submodules",
			    PARSE_OPT_OPTARG, option_parse_recurse_submodules_worktree_updater),
		OPT__QUIET(&opts.quiet, N_("suppress feedback messages")),
		OPT_پای_ټکی()
	};

	memset(&opts, 0, حجم(opts));
	opts.سر_idx = -1;
	opts.src_شاخص = the_repository->شاخص;
	opts.dst_شاخص = the_repository->شاخص;

	repo_config(the_repository, git_read_ونه_config, عدم);

	argc = parse_options(argc, argv, cmd_مختاړی, read_ونه_options,
			     read_ونه_usage, 0);

	مختاړی_سېټ = opts.مختاړی ? 1 : 0;
	إذا (1 < opts.یوځای + opts.reset + مختاړی_سېټ)
		die("Which one? -m, --reset, or --prefix?");

	/* Prefix should not start with a directory separator */
	إذا (opts.مختاړی && opts.مختاړی[0] == '/')
		die("Invalid prefix, prefix cannot start with '/'");

	إذا (opts.reset)
		opts.reset = UNPACK_RESET_OVERWRITE_UNTRACKED;

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_ډک_شاخص = 0;

	repo_hold_locked_شاخص(the_repository, &قلف_file, قلف_DIE_ON_ERROR);

	/*
	 * NEEDSWORK
	 *
	 * The old index should be read anyway even if we're going to
	 * destroy all index entries because we still need to preserve
	 * certain information such as index version or split-index
	 * mode.
	 */

	إذا (opts.reset || opts.یوځای || opts.مختاړی) {
		إذا (repo_read_شاخص_unmerged(the_repository) && (opts.مختاړی || opts.یوځای))
			die(_("You need to resolve your current index first"));
		stage = opts.یوځای = 1;
	}
	resolve_undo_clear_شاخص(the_repository->شاخص);

	لكل (i = 0; i < argc; i++) {
		ثابت حرف *arg = argv[i];

		إذا (repo_get_oid(the_repository, arg, &oid))
			die("Not a valid object name %s", arg);
		إذا (list_ونه(&oid) < 0)
			die("failed to unpack tree object %s", arg);
		stage++;
	}
	إذا (!nr_trees && !read_تشه && !opts.یوځای)
		warning("read-tree: emptying the index with no arguments is deprecated; use --empty");
	وإلا إذا (nr_trees > 0 && read_تشه)
		die("passing trees as arguments contradicts --empty");

	إذا (1 < opts.شاخص_only + opts.update)
		die("-u and -i at the same time makes no sense");
	إذا ((opts.update || opts.شاخص_only) && !opts.یوځای)
		die("%s is meaningless without -m, --reset, or --prefix",
		    opts.update ? "-u" : "-i");
	إذا (opts.update && !opts.reset)
		opts.preserve_ignored = 0;
	/* otherwise, opts.preserve_ignored is irrelevant */
	إذا (opts.یوځای && !opts.شاخص_only)
		تیاری_work_ونه(the_repository);

	إذا (opts.skip_sparse_checkout)
		ensure_ډک_شاخص(the_repository->شاخص);

	إذا (opts.یوځای) {
		بدل (stage - 1) {
		حالة 0:
			die("you must specify at least one tree to merge");
			اكسر;
		حالة 1:
			opts.fn = opts.مختاړی ? bind_یوځای : oneway_یوځای;
			اكسر;
		حالة 2:
			opts.fn = twoway_یوځای;
			opts.initial_checkout = is_شاخص_unborn(the_repository->شاخص);
			اكسر;
		حالة 3:
		افتراضي:
			opts.fn = threeway_یوځای;
			اكسر;
		}

		إذا (stage - 1 >= 3)
			opts.سر_idx = stage - 2;
		وإلا
			opts.سر_idx = 1;
	}

	إذا (opts.دننی.debug_unpack)
		opts.fn = debug_یوځای;

	/* If we're going to prime_cache_tree later, skip cache tree update */
	إذا (nr_trees == 1 && !opts.مختاړی)
		opts.skip_cache_ونه_update = 1;

	cache_ونه_free(&the_repository->شاخص->cache_ونه);
	لكل (i = 0; i < nr_trees; i++) {
		بنية ونه *ونه = trees[i];
		إذا (repo_parse_ونه(the_repository, ونه) < 0)
			أرجع 128;
		init_ونه_desc(t+i, &ونه->object.oid, ونه->بفر, ونه->اندازه);
	}
	إذا (unpack_trees(nr_trees, t, &opts))
		أرجع 128;

	إذا (opts.دننی.debug_unpack || opts.dry_پرلپسې)
		أرجع 0; /* do not write the index out */

	/*
	 * When reading only one tree (either the most basic form,
	 * "-m ent" or "--reset ent" form), we can obtain a fully
	 * valid cache-tree because the index must match exactly
	 * what came from the tree.
	 */
	إذا (nr_trees == 1 && !opts.مختاړی)
		اولي_cache_ونه(the_repository,
				 the_repository->شاخص,
				 trees[0]);

	إذا (write_locked_شاخص(the_repository->شاخص, &قلف_file, COMMIT_قلف))
		die("unable to write new index file");
	أرجع 0;
}
