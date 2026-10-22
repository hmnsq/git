/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#define USE_THE_REPOSITORY_VARIABLE
#अन्तर्भाव "builtin.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "lockfile.h"
#अन्तर्भाव "object.h"
#अन्तर्भाव "object-name.h"
#अन्तर्भाव "tree.h"
#अन्तर्भाव "tree-walk.h"
#अन्तर्भाव "cache-tree.h"
#अन्तर्भाव "unpack-trees.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "resolve-undo.h"
#अन्तर्भाव "setup.h"
#अन्तर्भाव "sparse-index.h"
#अन्तर्भाव "submodule.h"

स्थिर पूर्णाङ्क nr_trees;
स्थिर पूर्णाङ्क read_empty;
स्थिर संरचना वृक्ष *trees[MAX_UNPACK_TREES];

स्थिर पूर्णाङ्क list_वृक्ष(संरचना object_id *oid)
{
	संरचना वृक्ष *वृक्ष;

	यदि (nr_trees >= MAX_UNPACK_TREES)
		die("I cannot read more than %d trees", MAX_UNPACK_TREES);
	वृक्ष = repo_parse_वृक्ष_indirect(the_repository, oid);
	यदि (!वृक्ष)
		प्रत्यागम -1;
	trees[nr_trees++] = वृक्ष;
	प्रत्यागम 0;
}

स्थिर ध्रुव वर्ण * ध्रुव read_वृक्ष_usage[] = {
	N_("git read-tree [(-m [--trivial] [--aggressive] | --reset | --prefix=<prefix>)\n"
	   "              [-u | -i]] [--index-output=<file>] [--no-sparse-checkout]\n"
	   "              (--empty | <tree-ish1> [<tree-ish2> [<tree-ish3>]])"),
	रिक्त
};

स्थिर पूर्णाङ्क index_output_cb(ध्रुव संरचना option *opt UNUSED, ध्रुव वर्ण *arg,
				 पूर्णाङ्क unset)
{
	BUG_ON_OPT_NEG(unset);
	set_alternate_index_output(arg);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क exclude_per_directory_cb(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg,
				    पूर्णाङ्क unset)
{
	संरचना unpack_trees_options *opts;

	BUG_ON_OPT_NEG(unset);

	opts = (संरचना unpack_trees_options *)opt->मूल्य;

	यदि (!opts->update)
		die("--exclude-per-directory is meaningless unless -u");
	यदि (strcmp(arg, ".gitignore"))
		die("--exclude-per-directory argument must be .gitignore");
	प्रत्यागम 0;
}

स्थिर शून्य debug_stage(ध्रुव वर्ण *label, ध्रुव संरचना cache_entry *ce,
			संरचना unpack_trees_options *o)
{
	printf("%s ", label);
	यदि (!ce)
		printf("(missing)\n");
	अन्यथा यदि (ce == o->df_conflict_entry)
		printf("(conflict)\n");
	अन्यथा
		printf("%06o #%d %s %.8s\n",
		       ce->ce_mode, ce_stage(ce), ce->name,
		       oid_to_hex(&ce->oid));
}

स्थिर पूर्णाङ्क debug_merge(ध्रुव संरचना cache_entry * ध्रुव *stages,
		       संरचना unpack_trees_options *o)
{
	पूर्णाङ्क i;

	printf("* %d-way merge\n", o->internal.merge_size);
	debug_stage("index", stages[0], o);
	प्रति (i = 1; i <= o->internal.merge_size; i++) {
		वर्ण buf[24];
		xsnprintf(buf, परिमाण(buf), "ent#%d", i);
		debug_stage(buf, stages[i], o);
	}
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क git_read_वृक्ष_config(ध्रुव वर्ण *var, ध्रुव वर्ण *मूल्य,
				ध्रुव संरचना config_context *ctx, शून्य *cb)
{
	यदि (!strcmp(var, "submodule.recurse"))
		प्रत्यागम git_default_submodule_config(var, मूल्य, cb);

	प्रत्यागम git_default_config(var, मूल्य, ctx, cb);
}

पूर्णाङ्क cmd_read_वृक्ष(पूर्णाङ्क argc,
		  ध्रुव वर्ण **argv,
		  ध्रुव वर्ण *cmd_prefix,
		  संरचना repository *repo UNUSED)
{
	पूर्णाङ्क i, stage = 0;
	संरचना object_id oid;
	संरचना वृक्ष_desc t[MAX_UNPACK_TREES];
	संरचना unpack_trees_options opts;
	पूर्णाङ्क prefix_set = 0;
	संरचना lock_file lock_file = LOCK_INIT;
	ध्रुव संरचना option read_वृक्ष_options[] = {
		OPT__SUPER_PREFIX(&opts.super_prefix),
		OPT_CALLBACK_F(0, "index-output", रिक्त, N_("file"),
		  N_("write resulting index to <file>"),
		  PARSE_OPT_NONEG, index_output_cb),
		OPT_BOOL(0, "empty", &read_empty,
			    N_("only empty the index")),
		OPT__VERBOSE(&opts.verbose_update, N_("be verbose")),
		OPT_GROUP(N_("Merging")),
		OPT_BOOL('m', रिक्त, &opts.merge,
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
			.मूल्य = &opts.prefix,
			.argh = N_("<subdirectory>/"),
			.help = N_("read the tree into the index under <subdirectory>/"),
			.flags = PARSE_OPT_NONEG,
		},
		OPT_BOOL('u', रिक्त, &opts.update,
			 N_("update working tree with merge result")),
		OPT_CALLBACK_F(0, "exclude-per-directory", &opts,
		  N_("gitignore"),
		  N_("allow explicitly ignored files to be overwritten"),
		  PARSE_OPT_NONEG, exclude_per_directory_cb),
		OPT_BOOL('i', रिक्त, &opts.index_only,
			 N_("don't check the working tree after merging")),
		OPT__DRY_RUN(&opts.dry_run, N_("don't update the index or the work tree")),
		OPT_BOOL(0, "no-sparse-checkout", &opts.skip_sparse_checkout,
			 N_("skip applying sparse checkout filter")),
		OPT_BOOL(0, "debug-unpack", &opts.internal.debug_unpack,
			 N_("debug unpack-trees")),
		OPT_CALLBACK_F(0, "recurse-submodules", रिक्त,
			    "checkout", "control recursive updating of submodules",
			    PARSE_OPT_OPTARG, option_parse_recurse_submodules_worktree_updater),
		OPT__QUIET(&opts.quiet, N_("suppress feedback messages")),
		OPT_END()
	};

	memset(&opts, 0, परिमाण(opts));
	opts.head_idx = -1;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;

	repo_config(the_repository, git_read_वृक्ष_config, रिक्त);

	argc = parse_options(argc, argv, cmd_prefix, read_वृक्ष_options,
			     read_वृक्ष_usage, 0);

	prefix_set = opts.prefix ? 1 : 0;
	यदि (1 < opts.merge + opts.reset + prefix_set)
		die("Which one? -m, --reset, or --prefix?");

	/* Prefix should not start with a directory separator */
	यदि (opts.prefix && opts.prefix[0] == '/')
		die("Invalid prefix, prefix cannot start with '/'");

	यदि (opts.reset)
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

	यदि (opts.reset || opts.merge || opts.prefix) {
		यदि (repo_read_index_unmerged(the_repository) && (opts.prefix || opts.merge))
			die(_("You need to resolve your current index first"));
		stage = opts.merge = 1;
	}
	resolve_undo_clear_index(the_repository->index);

	प्रति (i = 0; i < argc; i++) {
		ध्रुव वर्ण *arg = argv[i];

		यदि (repo_get_oid(the_repository, arg, &oid))
			die("Not a valid object name %s", arg);
		यदि (list_वृक्ष(&oid) < 0)
			die("failed to unpack tree object %s", arg);
		stage++;
	}
	यदि (!nr_trees && !read_empty && !opts.merge)
		warning("read-tree: emptying the index with no arguments is deprecated; use --empty");
	अन्यथा यदि (nr_trees > 0 && read_empty)
		die("passing trees as arguments contradicts --empty");

	यदि (1 < opts.index_only + opts.update)
		die("-u and -i at the same time makes no sense");
	यदि ((opts.update || opts.index_only) && !opts.merge)
		die("%s is meaningless without -m, --reset, or --prefix",
		    opts.update ? "-u" : "-i");
	यदि (opts.update && !opts.reset)
		opts.preserve_ignored = 0;
	/* otherwise, opts.preserve_ignored is irrelevant */
	यदि (opts.merge && !opts.index_only)
		setup_work_वृक्ष(the_repository);

	यदि (opts.skip_sparse_checkout)
		ensure_full_index(the_repository->index);

	यदि (opts.merge) {
		विकल्प (stage - 1) {
		पक्ष 0:
			die("you must specify at least one tree to merge");
			भङ्ग;
		पक्ष 1:
			opts.fn = opts.prefix ? bind_merge : oneway_merge;
			भङ्ग;
		पक्ष 2:
			opts.fn = twoway_merge;
			opts.initial_checkout = is_index_unborn(the_repository->index);
			भङ्ग;
		पक्ष 3:
		स्वभाव:
			opts.fn = threeway_merge;
			भङ्ग;
		}

		यदि (stage - 1 >= 3)
			opts.head_idx = stage - 2;
		अन्यथा
			opts.head_idx = 1;
	}

	यदि (opts.internal.debug_unpack)
		opts.fn = debug_merge;

	/* If we're going to prime_cache_tree later, skip cache tree update */
	यदि (nr_trees == 1 && !opts.prefix)
		opts.skip_cache_वृक्ष_update = 1;

	cache_वृक्ष_free(&the_repository->index->cache_वृक्ष);
	प्रति (i = 0; i < nr_trees; i++) {
		संरचना वृक्ष *वृक्ष = trees[i];
		यदि (repo_parse_वृक्ष(the_repository, वृक्ष) < 0)
			प्रत्यागम 128;
		init_वृक्ष_desc(t+i, &वृक्ष->object.oid, वृक्ष->buffer, वृक्ष->size);
	}
	यदि (unpack_trees(nr_trees, t, &opts))
		प्रत्यागम 128;

	यदि (opts.internal.debug_unpack || opts.dry_run)
		प्रत्यागम 0; /* do not write the index out */

	/*
	 * When reading only one tree (either the most basic form,
	 * "-m ent" or "--reset ent" form), we can obtain a fully
	 * valid cache-tree because the index must match exactly
	 * what came from the tree.
	 */
	यदि (nr_trees == 1 && !opts.prefix)
		अभाज्य_cache_वृक्ष(the_repository,
				 the_repository->index,
				 trees[0]);

	यदि (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
		die("unable to write new index file");
	प्रत्यागम 0;
}
