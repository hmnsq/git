/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#define USE_THE_REPOSITORY_VARIABLE
#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "hex.h"
#അന്തര്ഭാവ "lockfile.h"
#അന്തര്ഭാവ "object.h"
#അന്തര്ഭാവ "object-name.h"
#അന്തര്ഭാവ "tree.h"
#അന്തര്ഭാവ "tree-walk.h"
#അന്തര്ഭാവ "cache-tree.h"
#അന്തര്ഭാവ "unpack-trees.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "resolve-undo.h"
#അന്തര്ഭാവ "setup.h"
#അന്തര്ഭാവ "sparse-index.h"
#അന്തര്ഭാവ "submodule.h"

സ്ഥിതം പൂർണ്ണസംഖ്യ nr_trees;
സ്ഥിതം പൂർണ്ണസംഖ്യ read_empty;
സ്ഥിതം ഘടന വൃക്ഷം *trees[MAX_UNPACK_TREES];

സ്ഥിതം പൂർണ്ണസംഖ്യ list_വൃക്ഷം(ഘടന object_id *oid)
{
	ഘടന വൃക്ഷം *വൃക്ഷം;

	എങ്കിൽ (nr_trees >= MAX_UNPACK_TREES)
		die("I cannot read more than %d trees", MAX_UNPACK_TREES);
	വൃക്ഷം = repo_parse_വൃക്ഷം_indirect(the_repository, oid);
	എങ്കിൽ (!വൃക്ഷം)
		തിരികെ -1;
	trees[nr_trees++] = വൃക്ഷം;
	തിരികെ 0;
}

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം read_വൃക്ഷം_usage[] = {
	N_("git read-tree [(-m [--trivial] [--aggressive] | --reset | --prefix=<prefix>)\n"
	   "              [-u | -i]] [--index-output=<file>] [--no-sparse-checkout]\n"
	   "              (--empty | <tree-ish1> [<tree-ish2> [<tree-ish3>]])"),
	ശൂന്യ
};

സ്ഥിതം പൂർണ്ണസംഖ്യ index_output_cb(സ്ഥിരം ഘടന option *opt UNUSED, സ്ഥിരം അക്ഷരം *arg,
				 പൂർണ്ണസംഖ്യ unset)
{
	BUG_ON_OPT_NEG(unset);
	set_alternate_index_output(arg);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ exclude_per_directory_cb(സ്ഥിരം ഘടന option *opt, സ്ഥിരം അക്ഷരം *arg,
				    പൂർണ്ണസംഖ്യ unset)
{
	ഘടന unpack_trees_options *opts;

	BUG_ON_OPT_NEG(unset);

	opts = (ഘടന unpack_trees_options *)opt->മൂല്യം;

	എങ്കിൽ (!opts->update)
		die("--exclude-per-directory is meaningless unless -u");
	എങ്കിൽ (strcmp(arg, ".gitignore"))
		die("--exclude-per-directory argument must be .gitignore");
	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം debug_stage(സ്ഥിരം അക്ഷരം *label, സ്ഥിരം ഘടന cache_entry *ce,
			ഘടന unpack_trees_options *o)
{
	printf("%s ", label);
	എങ്കിൽ (!ce)
		printf("(missing)\n");
	അല്ലെങ്കിൽ എങ്കിൽ (ce == o->df_conflict_entry)
		printf("(conflict)\n");
	അല്ലെങ്കിൽ
		printf("%06o #%d %s %.8s\n",
		       ce->ce_mode, ce_stage(ce), ce->name,
		       oid_to_hex(&ce->oid));
}

സ്ഥിതം പൂർണ്ണസംഖ്യ debug_merge(സ്ഥിരം ഘടന cache_entry * സ്ഥിരം *stages,
		       ഘടന unpack_trees_options *o)
{
	പൂർണ്ണസംഖ്യ i;

	printf("* %d-way merge\n", o->internal.merge_size);
	debug_stage("index", stages[0], o);
	വേണ്ടി (i = 1; i <= o->internal.merge_size; i++) {
		അക്ഷരം buf[24];
		xsnprintf(buf, വലുപ്പം(buf), "ent#%d", i);
		debug_stage(buf, stages[i], o);
	}
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ git_read_വൃക്ഷം_config(സ്ഥിരം അക്ഷരം *var, സ്ഥിരം അക്ഷരം *മൂല്യം,
				സ്ഥിരം ഘടന config_context *ctx, ശൂന്യം *cb)
{
	എങ്കിൽ (!strcmp(var, "submodule.recurse"))
		തിരികെ git_default_submodule_config(var, മൂല്യം, cb);

	തിരികെ git_default_config(var, മൂല്യം, ctx, cb);
}

പൂർണ്ണസംഖ്യ cmd_read_വൃക്ഷം(പൂർണ്ണസംഖ്യ argc,
		  സ്ഥിരം അക്ഷരം **argv,
		  സ്ഥിരം അക്ഷരം *cmd_prefix,
		  ഘടന repository *repo UNUSED)
{
	പൂർണ്ണസംഖ്യ i, stage = 0;
	ഘടന object_id oid;
	ഘടന വൃക്ഷം_desc t[MAX_UNPACK_TREES];
	ഘടന unpack_trees_options opts;
	പൂർണ്ണസംഖ്യ prefix_set = 0;
	ഘടന lock_file lock_file = LOCK_INIT;
	സ്ഥിരം ഘടന option read_വൃക്ഷം_options[] = {
		OPT__SUPER_PREFIX(&opts.super_prefix),
		OPT_CALLBACK_F(0, "index-output", ശൂന്യ, N_("file"),
		  N_("write resulting index to <file>"),
		  PARSE_OPT_NONEG, index_output_cb),
		OPT_BOOL(0, "empty", &read_empty,
			    N_("only empty the index")),
		OPT__VERBOSE(&opts.verbose_update, N_("be verbose")),
		OPT_GROUP(N_("Merging")),
		OPT_BOOL('m', ശൂന്യ, &opts.merge,
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
			.മൂല്യം = &opts.prefix,
			.argh = N_("<subdirectory>/"),
			.help = N_("read the tree into the index under <subdirectory>/"),
			.flags = PARSE_OPT_NONEG,
		},
		OPT_BOOL('u', ശൂന്യ, &opts.update,
			 N_("update working tree with merge result")),
		OPT_CALLBACK_F(0, "exclude-per-directory", &opts,
		  N_("gitignore"),
		  N_("allow explicitly ignored files to be overwritten"),
		  PARSE_OPT_NONEG, exclude_per_directory_cb),
		OPT_BOOL('i', ശൂന്യ, &opts.index_only,
			 N_("don't check the working tree after merging")),
		OPT__DRY_RUN(&opts.dry_run, N_("don't update the index or the work tree")),
		OPT_BOOL(0, "no-sparse-checkout", &opts.skip_sparse_checkout,
			 N_("skip applying sparse checkout filter")),
		OPT_BOOL(0, "debug-unpack", &opts.internal.debug_unpack,
			 N_("debug unpack-trees")),
		OPT_CALLBACK_F(0, "recurse-submodules", ശൂന്യ,
			    "checkout", "control recursive updating of submodules",
			    PARSE_OPT_OPTARG, option_parse_recurse_submodules_worktree_updater),
		OPT__QUIET(&opts.quiet, N_("suppress feedback messages")),
		OPT_END()
	};

	memset(&opts, 0, വലുപ്പം(opts));
	opts.head_idx = -1;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;

	repo_config(the_repository, git_read_വൃക്ഷം_config, ശൂന്യ);

	argc = parse_options(argc, argv, cmd_prefix, read_വൃക്ഷം_options,
			     read_വൃക്ഷം_usage, 0);

	prefix_set = opts.prefix ? 1 : 0;
	എങ്കിൽ (1 < opts.merge + opts.reset + prefix_set)
		die("Which one? -m, --reset, or --prefix?");

	/* Prefix should not start with a directory separator */
	എങ്കിൽ (opts.prefix && opts.prefix[0] == '/')
		die("Invalid prefix, prefix cannot start with '/'");

	എങ്കിൽ (opts.reset)
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

	എങ്കിൽ (opts.reset || opts.merge || opts.prefix) {
		എങ്കിൽ (repo_read_index_unmerged(the_repository) && (opts.prefix || opts.merge))
			die(_("You need to resolve your current index first"));
		stage = opts.merge = 1;
	}
	resolve_undo_clear_index(the_repository->index);

	വേണ്ടി (i = 0; i < argc; i++) {
		സ്ഥിരം അക്ഷരം *arg = argv[i];

		എങ്കിൽ (repo_get_oid(the_repository, arg, &oid))
			die("Not a valid object name %s", arg);
		എങ്കിൽ (list_വൃക്ഷം(&oid) < 0)
			die("failed to unpack tree object %s", arg);
		stage++;
	}
	എങ്കിൽ (!nr_trees && !read_empty && !opts.merge)
		warning("read-tree: emptying the index with no arguments is deprecated; use --empty");
	അല്ലെങ്കിൽ എങ്കിൽ (nr_trees > 0 && read_empty)
		die("passing trees as arguments contradicts --empty");

	എങ്കിൽ (1 < opts.index_only + opts.update)
		die("-u and -i at the same time makes no sense");
	എങ്കിൽ ((opts.update || opts.index_only) && !opts.merge)
		die("%s is meaningless without -m, --reset, or --prefix",
		    opts.update ? "-u" : "-i");
	എങ്കിൽ (opts.update && !opts.reset)
		opts.preserve_ignored = 0;
	/* otherwise, opts.preserve_ignored is irrelevant */
	എങ്കിൽ (opts.merge && !opts.index_only)
		setup_work_വൃക്ഷം(the_repository);

	എങ്കിൽ (opts.skip_sparse_checkout)
		ensure_full_index(the_repository->index);

	എങ്കിൽ (opts.merge) {
		തിരഞ്ഞെടുപ്പ് (stage - 1) {
		അവസ്ഥ 0:
			die("you must specify at least one tree to merge");
			നിർത്തുക;
		അവസ്ഥ 1:
			opts.fn = opts.prefix ? bind_merge : oneway_merge;
			നിർത്തുക;
		അവസ്ഥ 2:
			opts.fn = twoway_merge;
			opts.initial_checkout = is_index_unborn(the_repository->index);
			നിർത്തുക;
		അവസ്ഥ 3:
		സ്ഥിരസ്ഥിതി:
			opts.fn = threeway_merge;
			നിർത്തുക;
		}

		എങ്കിൽ (stage - 1 >= 3)
			opts.head_idx = stage - 2;
		അല്ലെങ്കിൽ
			opts.head_idx = 1;
	}

	എങ്കിൽ (opts.internal.debug_unpack)
		opts.fn = debug_merge;

	/* If we're going to prime_cache_tree later, skip cache tree update */
	എങ്കിൽ (nr_trees == 1 && !opts.prefix)
		opts.skip_cache_വൃക്ഷം_update = 1;

	cache_വൃക്ഷം_free(&the_repository->index->cache_വൃക്ഷം);
	വേണ്ടി (i = 0; i < nr_trees; i++) {
		ഘടന വൃക്ഷം *വൃക്ഷം = trees[i];
		എങ്കിൽ (repo_parse_വൃക്ഷം(the_repository, വൃക്ഷം) < 0)
			തിരികെ 128;
		init_വൃക്ഷം_desc(t+i, &വൃക്ഷം->object.oid, വൃക്ഷം->buffer, വൃക്ഷം->size);
	}
	എങ്കിൽ (unpack_trees(nr_trees, t, &opts))
		തിരികെ 128;

	എങ്കിൽ (opts.internal.debug_unpack || opts.dry_run)
		തിരികെ 0; /* do not write the index out */

	/*
	 * When reading only one tree (either the most basic form,
	 * "-m ent" or "--reset ent" form), we can obtain a fully
	 * valid cache-tree because the index must match exactly
	 * what came from the tree.
	 */
	എങ്കിൽ (nr_trees == 1 && !opts.prefix)
		അഭാജ്യം_cache_വൃക്ഷം(the_repository,
				 the_repository->index,
				 trees[0]);

	എങ്കിൽ (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
		die("unable to write new index file");
	തിരികെ 0;
}
