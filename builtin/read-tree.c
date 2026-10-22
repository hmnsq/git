/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#وصف_ڪريو USE_THE_REPOSITORY_VARIABLE
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
ساكن صحيح read_خالي_جايون;
ساكن بنية وڻ *trees[MAX_UNPACK_TREES];

ساكن صحيح list_وڻ(بنية object_id *oid)
{
	بنية وڻ *وڻ;

	إذا (nr_trees >= MAX_UNPACK_TREES)
		die("I cannot read more than %d trees", MAX_UNPACK_TREES);
	وڻ = repo_parse_وڻ_اڻسڌو(the_repository, oid);
	إذا (!وڻ)
		أرجع -1;
	trees[nr_trees++] = وڻ;
	أرجع 0;
}

ساكن ثابت حرف * ثابت read_وڻ_usage[] = {
	N_("git read-tree [(-m [--trivial] [--aggressive] | --reset | --prefix=<prefix>)\n"
	   "              [-u | -i]] [--index-output=<file>] [--no-sparse-checkout]\n"
	   "              (--empty | <tree-ish1> [<tree-ish2> [<tree-ish3>]])"),
	عدم
};

ساكن صحيح اشاريو_output_cb(ثابت بنية option *opt اڻ_استعمال_ٿيل, ثابت حرف *arg,
				 صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	سيٽ_alternate_اشاريو_output(arg);
	أرجع 0;
}

ساكن صحيح exclude_في_directory_cb(ثابت بنية option *opt, ثابت حرف *arg,
				    صحيح unset)
{
	بنية unpack_trees_options *opts;

	BUG_ON_OPT_NEG(unset);

	opts = (بنية unpack_trees_options *)opt->قدر;

	إذا (!opts->update)
		die("--exclude-per-directory is meaningless unless -u");
	إذا (strcmp(arg, ".gitignore"))
		die("--exclude-per-directory argument must be .gitignore");
	أرجع 0;
}

ساكن فراغ debug_stage(ثابت حرف *label, ثابت بنية cache_داخلا *ce,
			بنية unpack_trees_options *o)
{
	لکو("%s ", label);
	إذا (!ce)
		لکو("(missing)\n");
	وإلا إذا (ce == o->df_conflict_داخلا)
		لکو("(conflict)\n");
	وإلا
		لکو("%06o #%d %s %.8s\n",
		       ce->ce_گهڻائي, ce_stage(ce), ce->name,
		       oid_منزل_hex(&ce->oid));
}

ساكن صحيح debug_ملايو(ثابت بنية cache_داخلا * ثابت *stages,
		       بنية unpack_trees_options *o)
{
	صحيح i;

	لکو("* %d-way merge\n", o->اندروني.ملايو_سائيز);
	debug_stage("index", stages[0], o);
	لكل (i = 1; i <= o->اندروني.ملايو_سائيز; i++) {
		حرف buf[24];
		xsnprintf(buf, حجم(buf), "ent#%d", i);
		debug_stage(buf, stages[i], o);
	}
	أرجع 0;
}

ساكن صحيح git_read_وڻ_config(ثابت حرف *var, ثابت حرف *قدر,
				ثابت بنية config_context *ctx, فراغ *cb)
{
	إذا (!strcmp(var, "submodule.recurse"))
		أرجع git_default_submodule_config(var, قدر, cb);

	أرجع git_default_config(var, قدر, ctx, cb);
}

صحيح cmd_read_وڻ(صحيح argc,
		  ثابت حرف **argv,
		  ثابت حرف *cmd_اڳياڙي,
		  بنية repository *repo اڻ_استعمال_ٿيل)
{
	صحيح i, stage = 0;
	بنية object_id oid;
	بنية وڻ_desc t[MAX_UNPACK_TREES];
	بنية unpack_trees_options opts;
	صحيح اڳياڙي_سيٽ = 0;
	بنية تالو_file تالو_file = تالو_INIT;
	ثابت بنية option read_وڻ_options[] = {
		OPT_SUPER_اڳياڙي(&opts.super_اڳياڙي),
		OPT_CALLBACK_F(0, "index-output", عدم, N_("file"),
		  N_("write resulting index to <file>"),
		  PARSE_OPT_NONEG, اشاريو_output_cb),
		OPT_BOOL(0, "empty", &read_خالي_جايون,
			    N_("only empty the index")),
		OPT__VERBOSE(&opts.verbose_update, N_("be verbose")),
		OPT_گروهه(N_("Merging")),
		OPT_BOOL('m', عدم, &opts.ملايو,
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
			.قدر = &opts.اڳياڙي,
			.argh = N_("<subdirectory>/"),
			.help = N_("read the tree into the index under <subdirectory>/"),
			.flags = PARSE_OPT_NONEG,
		},
		OPT_BOOL('u', عدم, &opts.update,
			 N_("update working tree with merge result")),
		OPT_CALLBACK_F(0, "exclude-per-directory", &opts,
		  N_("gitignore"),
		  N_("allow explicitly ignored files to be overwritten"),
		  PARSE_OPT_NONEG, exclude_في_directory_cb),
		OPT_BOOL('i', عدم, &opts.اشاريو_only,
			 N_("don't check the working tree after merging")),
		OPT_DRY_لاڳيتو(&opts.dry_لاڳيتو, N_("don't update the index or the work tree")),
		OPT_BOOL(0, "no-sparse-checkout", &opts.skip_sparse_checkout,
			 N_("skip applying sparse checkout filter")),
		OPT_BOOL(0, "debug-unpack", &opts.اندروني.debug_unpack,
			 N_("debug unpack-trees")),
		OPT_CALLBACK_F(0, "recurse-submodules", عدم,
			    "checkout", "control recursive updating of submodules",
			    PARSE_OPT_OPTARG, option_parse_recurse_submodules_worktree_updater),
		OPT__QUIET(&opts.quiet, N_("suppress feedback messages")),
		OPT_پڇاڙي()
	};

	memset(&opts, 0, حجم(opts));
	opts.سر_idx = -1;
	opts.src_اشاريو = the_repository->اشاريو;
	opts.dst_اشاريو = the_repository->اشاريو;

	repo_config(the_repository, git_read_وڻ_config, عدم);

	argc = parse_options(argc, argv, cmd_اڳياڙي, read_وڻ_options,
			     read_وڻ_usage, 0);

	اڳياڙي_سيٽ = opts.اڳياڙي ? 1 : 0;
	إذا (1 < opts.ملايو + opts.reset + اڳياڙي_سيٽ)
		die("Which one? -m, --reset, or --prefix?");

	/* Prefix should not start with a directory separator */
	إذا (opts.اڳياڙي && opts.اڳياڙي[0] == '/')
		die("Invalid prefix, prefix cannot start with '/'");

	إذا (opts.reset)
		opts.reset = UNPACK_RESET_OVERWRITE_UNTRACKED;

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_ڀريل_اشاريو = 0;

	repo_hold_locked_اشاريو(the_repository, &تالو_file, تالو_DIE_ON_ERROR);

	/*
	 * NEEDSWORK
	 *
	 * The old index should be read anyway even if we're going to
	 * destroy all index entries because we still need to preserve
	 * certain information such as index version or split-index
	 * mode.
	 */

	إذا (opts.reset || opts.ملايو || opts.اڳياڙي) {
		إذا (repo_read_اشاريو_unmerged(the_repository) && (opts.اڳياڙي || opts.ملايو))
			die(_("You need to resolve your current index first"));
		stage = opts.ملايو = 1;
	}
	resolve_undo_clear_اشاريو(the_repository->اشاريو);

	لكل (i = 0; i < argc; i++) {
		ثابت حرف *arg = argv[i];

		إذا (repo_get_oid(the_repository, arg, &oid))
			die("Not a valid object name %s", arg);
		إذا (list_وڻ(&oid) < 0)
			die("failed to unpack tree object %s", arg);
		stage++;
	}
	إذا (!nr_trees && !read_خالي_جايون && !opts.ملايو)
		warning("read-tree: emptying the index with no arguments is deprecated; use --empty");
	وإلا إذا (nr_trees > 0 && read_خالي_جايون)
		die("passing trees as arguments contradicts --empty");

	إذا (1 < opts.اشاريو_only + opts.update)
		die("-u and -i at the same time makes no sense");
	إذا ((opts.update || opts.اشاريو_only) && !opts.ملايو)
		die("%s is meaningless without -m, --reset, or --prefix",
		    opts.update ? "-u" : "-i");
	إذا (opts.update && !opts.reset)
		opts.preserve_ignored = 0;
	/* otherwise, opts.preserve_ignored is irrelevant */
	إذا (opts.ملايو && !opts.اشاريو_only)
		تياري_work_وڻ(the_repository);

	إذا (opts.skip_sparse_checkout)
		ensure_ڀريل_اشاريو(the_repository->اشاريو);

	إذا (opts.ملايو) {
		بدل (stage - 1) {
		حالة 0:
			die("you must specify at least one tree to merge");
			اكسر;
		حالة 1:
			opts.fn = opts.اڳياڙي ? bind_ملايو : oneway_ملايو;
			اكسر;
		حالة 2:
			opts.fn = twoway_ملايو;
			opts.initial_checkout = is_اشاريو_unborn(the_repository->اشاريو);
			اكسر;
		حالة 3:
		افتراضي:
			opts.fn = threeway_ملايو;
			اكسر;
		}

		إذا (stage - 1 >= 3)
			opts.سر_idx = stage - 2;
		وإلا
			opts.سر_idx = 1;
	}

	إذا (opts.اندروني.debug_unpack)
		opts.fn = debug_ملايو;

	/* If we're going to prime_cache_tree later, skip cache tree update */
	إذا (nr_trees == 1 && !opts.اڳياڙي)
		opts.skip_cache_وڻ_update = 1;

	cache_وڻ_free(&the_repository->اشاريو->cache_وڻ);
	لكل (i = 0; i < nr_trees; i++) {
		بنية وڻ *وڻ = trees[i];
		إذا (repo_parse_وڻ(the_repository, وڻ) < 0)
			أرجع 128;
		init_وڻ_desc(t+i, &وڻ->object.oid, وڻ->بفر, وڻ->سائيز);
	}
	إذا (unpack_trees(nr_trees, t, &opts))
		أرجع 128;

	إذا (opts.اندروني.debug_unpack || opts.dry_لاڳيتو)
		أرجع 0; /* do not write the index out */

	/*
	 * When reading only one tree (either the most basic form,
	 * "-m ent" or "--reset ent" form), we can obtain a fully
	 * valid cache-tree because the index must match exactly
	 * what came from the tree.
	 */
	إذا (nr_trees == 1 && !opts.اڳياڙي)
		مفرد_cache_وڻ(the_repository,
				 the_repository->اشاريو,
				 trees[0]);

	إذا (write_locked_اشاريو(the_repository->اشاريو, &تالو_file, COMMIT_تالو))
		die("unable to write new index file");
	أرجع 0;
}
