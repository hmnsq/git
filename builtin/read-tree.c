/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#घोषणा USE_THE_REPOSITORY_VARIABLE
#समावेश "builtin.h"
#समावेश "config.h"
#समावेश "environment.h"
#समावेश "gettext.h"
#समावेश "hex.h"
#समावेश "lockfile.h"
#समावेश "object.h"
#समावेश "object-name.h"
#समावेश "tree.h"
#समावेश "tree-walk.h"
#समावेश "cache-tree.h"
#समावेश "unpack-trees.h"
#समावेश "parse-options.h"
#समावेश "resolve-undo.h"
#समावेश "setup.h"
#समावेश "sparse-index.h"
#समावेश "submodule.h"

अटल पूर्णांक nr_trees;
अटल पूर्णांक read_empty;
अटल काष्ठा वृक्ष *trees[MAX_UNPACK_TREES];

अटल पूर्णांक list_वृक्ष(काष्ठा object_id *oid)
{
	काष्ठा वृक्ष *वृक्ष;

	अगर (nr_trees >= MAX_UNPACK_TREES)
		die("I cannot read more than %d trees", MAX_UNPACK_TREES);
	वृक्ष = repo_parse_वृक्ष_indirect(the_repository, oid);
	अगर (!वृक्ष)
		वापस -1;
	trees[nr_trees++] = वृक्ष;
	वापस 0;
}

अटल स्थिर अक्षर * स्थिर read_वृक्ष_usage[] = {
	N_("git read-tree [(-m [--trivial] [--aggressive] | --reset | --prefix=<prefix>)\n"
	   "              [-u | -i]] [--index-output=<file>] [--no-sparse-checkout]\n"
	   "              (--empty | <tree-ish1> [<tree-ish2> [<tree-ish3>]])"),
	शून्य
};

अटल पूर्णांक सूचकांक_output_cb(स्थिर काष्ठा option *opt UNUSED, स्थिर अक्षर *arg,
				 पूर्णांक unset)
{
	BUG_ON_OPT_NEG(unset);
	set_alternate_सूचकांक_output(arg);
	वापस 0;
}

अटल पूर्णांक exclude_per_directory_cb(स्थिर काष्ठा option *opt, स्थिर अक्षर *arg,
				    पूर्णांक unset)
{
	काष्ठा unpack_trees_options *opts;

	BUG_ON_OPT_NEG(unset);

	opts = (काष्ठा unpack_trees_options *)opt->मूल्य;

	अगर (!opts->update)
		die("--exclude-per-directory is meaningless unless -u");
	अगर (म_भेद(arg, ".gitignore"))
		die("--exclude-per-directory argument must be .gitignore");
	वापस 0;
}

अटल व्योम debug_stage(स्थिर अक्षर *label, स्थिर काष्ठा cache_entry *ce,
			काष्ठा unpack_trees_options *o)
{
	म_लिखो("%s ", label);
	अगर (!ce)
		म_लिखो("(missing)\n");
	अन्यथा अगर (ce == o->df_conflict_entry)
		म_लिखो("(conflict)\n");
	अन्यथा
		म_लिखो("%06o #%d %s %.8s\n",
		       ce->ce_mode, ce_stage(ce), ce->name,
		       oid_to_hex(&ce->oid));
}

अटल पूर्णांक debug_merge(स्थिर काष्ठा cache_entry * स्थिर *stages,
		       काष्ठा unpack_trees_options *o)
{
	पूर्णांक i;

	म_लिखो("* %d-way merge\n", o->internal.merge_आकार);
	debug_stage("index", stages[0], o);
	क्रम (i = 1; i <= o->internal.merge_आकार; i++) {
		अक्षर buf[24];
		xsnprintf(buf, माप(buf), "ent#%d", i);
		debug_stage(buf, stages[i], o);
	}
	वापस 0;
}

अटल पूर्णांक git_read_वृक्ष_config(स्थिर अक्षर *var, स्थिर अक्षर *मूल्य,
				स्थिर काष्ठा config_context *ctx, व्योम *cb)
{
	अगर (!म_भेद(var, "submodule.recurse"))
		वापस git_default_submodule_config(var, मूल्य, cb);

	वापस git_default_config(var, मूल्य, ctx, cb);
}

पूर्णांक cmd_read_वृक्ष(पूर्णांक argc,
		  स्थिर अक्षर **argv,
		  स्थिर अक्षर *cmd_prefix,
		  काष्ठा repository *repo UNUSED)
{
	पूर्णांक i, stage = 0;
	काष्ठा object_id oid;
	काष्ठा वृक्ष_desc t[MAX_UNPACK_TREES];
	काष्ठा unpack_trees_options opts;
	पूर्णांक prefix_set = 0;
	काष्ठा lock_file lock_file = LOCK_INIT;
	स्थिर काष्ठा option read_वृक्ष_options[] = {
		OPT__SUPER_PREFIX(&opts.super_prefix),
		OPT_CALLBACK_F(0, "index-output", शून्य, N_("file"),
		  N_("write resulting index to <file>"),
		  PARSE_OPT_NONEG, सूचकांक_output_cb),
		OPT_BOOL(0, "empty", &read_empty,
			    N_("only empty the index")),
		OPT__VERBOSE(&opts.verbose_update, N_("be verbose")),
		OPT_GROUP(N_("Merging")),
		OPT_BOOL('m', शून्य, &opts.merge,
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
		OPT_BOOL('u', शून्य, &opts.update,
			 N_("update working tree with merge result")),
		OPT_CALLBACK_F(0, "exclude-per-directory", &opts,
		  N_("gitignore"),
		  N_("allow explicitly ignored files to be overwritten"),
		  PARSE_OPT_NONEG, exclude_per_directory_cb),
		OPT_BOOL('i', शून्य, &opts.सूचकांक_only,
			 N_("don't check the working tree after merging")),
		OPT__DRY_RUN(&opts.dry_run, N_("don't update the index or the work tree")),
		OPT_BOOL(0, "no-sparse-checkout", &opts.skip_sparse_checkout,
			 N_("skip applying sparse checkout filter")),
		OPT_BOOL(0, "debug-unpack", &opts.internal.debug_unpack,
			 N_("debug unpack-trees")),
		OPT_CALLBACK_F(0, "recurse-submodules", शून्य,
			    "checkout", "control recursive updating of submodules",
			    PARSE_OPT_OPTARG, option_parse_recurse_submodules_worktree_updater),
		OPT__QUIET(&opts.quiet, N_("suppress feedback messages")),
		OPT_END()
	};

	स_रखो(&opts, 0, माप(opts));
	opts.शीर्ष_idx = -1;
	opts.src_सूचकांक = the_repository->सूचकांक;
	opts.dst_सूचकांक = the_repository->सूचकांक;

	repo_config(the_repository, git_read_वृक्ष_config, शून्य);

	argc = parse_options(argc, argv, cmd_prefix, read_वृक्ष_options,
			     read_वृक्ष_usage, 0);

	prefix_set = opts.prefix ? 1 : 0;
	अगर (1 < opts.merge + opts.reset + prefix_set)
		die("Which one? -m, --reset, or --prefix?");

	/* Prefix should not start with a directory separator */
	अगर (opts.prefix && opts.prefix[0] == '/')
		die("Invalid prefix, prefix cannot start with '/'");

	अगर (opts.reset)
		opts.reset = UNPACK_RESET_OVERWRITE_UNTRACKED;

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_सूचकांक = 0;

	repo_hold_locked_सूचकांक(the_repository, &lock_file, LOCK_DIE_ON_ERROR);

	/*
	 * NEEDSWORK
	 *
	 * The old index should be read anyway even if we're going to
	 * destroy all index entries because we still need to preserve
	 * certain information such as index version or split-index
	 * mode.
	 */

	अगर (opts.reset || opts.merge || opts.prefix) {
		अगर (repo_read_सूचकांक_unmerged(the_repository) && (opts.prefix || opts.merge))
			die(_("You need to resolve your current index first"));
		stage = opts.merge = 1;
	}
	resolve_undo_clear_सूचकांक(the_repository->सूचकांक);

	क्रम (i = 0; i < argc; i++) {
		स्थिर अक्षर *arg = argv[i];

		अगर (repo_get_oid(the_repository, arg, &oid))
			die("Not a valid object name %s", arg);
		अगर (list_वृक्ष(&oid) < 0)
			die("failed to unpack tree object %s", arg);
		stage++;
	}
	अगर (!nr_trees && !read_empty && !opts.merge)
		warning("read-tree: emptying the index with no arguments is deprecated; use --empty");
	अन्यथा अगर (nr_trees > 0 && read_empty)
		die("passing trees as arguments contradicts --empty");

	अगर (1 < opts.सूचकांक_only + opts.update)
		die("-u and -i at the same time makes no sense");
	अगर ((opts.update || opts.सूचकांक_only) && !opts.merge)
		die("%s is meaningless without -m, --reset, or --prefix",
		    opts.update ? "-u" : "-i");
	अगर (opts.update && !opts.reset)
		opts.preserve_ignored = 0;
	/* otherwise, opts.preserve_ignored is irrelevant */
	अगर (opts.merge && !opts.सूचकांक_only)
		setup_work_वृक्ष(the_repository);

	अगर (opts.skip_sparse_checkout)
		ensure_full_सूचकांक(the_repository->सूचकांक);

	अगर (opts.merge) {
		चयन (stage - 1) {
		हाल 0:
			die("you must specify at least one tree to merge");
			अवरोध;
		हाल 1:
			opts.fn = opts.prefix ? bind_merge : oneway_merge;
			अवरोध;
		हाल 2:
			opts.fn = twoway_merge;
			opts.initial_checkout = is_सूचकांक_unborn(the_repository->सूचकांक);
			अवरोध;
		हाल 3:
		शेष:
			opts.fn = threeway_merge;
			अवरोध;
		}

		अगर (stage - 1 >= 3)
			opts.शीर्ष_idx = stage - 2;
		अन्यथा
			opts.शीर्ष_idx = 1;
	}

	अगर (opts.internal.debug_unpack)
		opts.fn = debug_merge;

	/* If we're going to prime_cache_tree later, skip cache tree update */
	अगर (nr_trees == 1 && !opts.prefix)
		opts.skip_cache_वृक्ष_update = 1;

	cache_वृक्ष_free(&the_repository->सूचकांक->cache_वृक्ष);
	क्रम (i = 0; i < nr_trees; i++) {
		काष्ठा वृक्ष *वृक्ष = trees[i];
		अगर (repo_parse_वृक्ष(the_repository, वृक्ष) < 0)
			वापस 128;
		init_वृक्ष_desc(t+i, &वृक्ष->object.oid, वृक्ष->बफर, वृक्ष->आकार);
	}
	अगर (unpack_trees(nr_trees, t, &opts))
		वापस 128;

	अगर (opts.internal.debug_unpack || opts.dry_run)
		वापस 0; /* do not write the index out */

	/*
	 * When reading only one tree (either the most basic form,
	 * "-m ent" or "--reset ent" form), we can obtain a fully
	 * valid cache-tree because the index must match exactly
	 * what came from the tree.
	 */
	अगर (nr_trees == 1 && !opts.prefix)
		अभाज्य_cache_वृक्ष(the_repository,
				 the_repository->सूचकांक,
				 trees[0]);

	अगर (write_locked_सूचकांक(the_repository->सूचकांक, &lock_file, COMMIT_LOCK))
		die("unable to write new index file");
	वापस 0;
}
