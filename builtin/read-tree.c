/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#परिभाषय USE_THE_REPOSITORY_VARIABLE
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
स्थिर पूर्णाङ्क read_रिक्तानि;
स्थिर संरचना वृक्षः *trees[MAX_UNPACK_TREES];

स्थिर पूर्णाङ्क list_वृक्षः(संरचना object_id *oid)
{
	संरचना वृक्षः *वृक्षः;

	यदि (nr_trees >= MAX_UNPACK_TREES)
		die("I cannot read more than %d trees", MAX_UNPACK_TREES);
	वृक्षः = repo_parse_वृक्षः_परोक्षम्(the_repository, oid);
	यदि (!वृक्षः)
		प्रत्यागम -1;
	trees[nr_trees++] = वृक्षः;
	प्रत्यागम 0;
}

स्थिर ध्रुव वर्ण * ध्रुव read_वृक्षः_usage[] = {
	N_("git read-tree [(-m [--trivial] [--aggressive] | --reset | --prefix=<prefix>)\n"
	   "              [-u | -i]] [--index-output=<file>] [--no-sparse-checkout]\n"
	   "              (--empty | <tree-ish1> [<tree-ish2> [<tree-ish3>]])"),
	रिक्त
};

स्थिर पूर्णाङ्क सूचकांक_output_cb(ध्रुव संरचना option *opt अप्रयुक्तम्, ध्रुव वर्ण *arg,
				 पूर्णाङ्क unset)
{
	BUG_ON_OPT_NEG(unset);
	समुच्चयः_alternate_सूचकांक_output(arg);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क exclude_प्रत्येकम्_directory_cb(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg,
				    पूर्णाङ्क unset)
{
	संरचना unpack_trees_options *opts;

	BUG_ON_OPT_NEG(unset);

	opts = (संरचना unpack_trees_options *)opt->मानम्;

	यदि (!opts->update)
		die("--exclude-per-directory is meaningless unless -u");
	यदि (वाक्यं_तुलय(arg, ".gitignore"))
		die("--exclude-per-directory argument must be .gitignore");
	प्रत्यागम 0;
}

स्थिर शून्य debug_stage(ध्रुव वर्ण *label, ध्रुव संरचना cache_प्रविष्टिः *ce,
			संरचना unpack_trees_options *o)
{
	प्रारूप_लिख("%s ", label);
	यदि (!ce)
		प्रारूप_लिख("(missing)\n");
	अन्यथा यदि (ce == o->df_conflict_प्रविष्टिः)
		प्रारूप_लिख("(conflict)\n");
	अन्यथा
		प्रारूप_लिख("%06o #%d %s %.8s\n",
		       ce->ce_बहुलकम्, ce_stage(ce), ce->name,
		       oid_गन्तव्यम्_hex(&ce->oid));
}

स्थिर पूर्णाङ्क debug_merge(ध्रुव संरचना cache_प्रविष्टिः * ध्रुव *stages,
		       संरचना unpack_trees_options *o)
{
	पूर्णाङ्क i;

	प्रारूप_लिख("* %d-way merge\n", o->आन्तरिकम्.merge_आकार);
	debug_stage("index", stages[0], o);
	प्रति (i = 1; i <= o->आन्तरिकम्.merge_आकार; i++) {
		वर्ण buf[24];
		xsnprintf(buf, परिमाण(buf), "ent#%d", i);
		debug_stage(buf, stages[i], o);
	}
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क git_read_वृक्षः_config(ध्रुव वर्ण *var, ध्रुव वर्ण *मानम्,
				ध्रुव संरचना config_context *ctx, शून्य *cb)
{
	यदि (!वाक्यं_तुलय(var, "submodule.recurse"))
		प्रत्यागम git_default_submodule_config(var, मानम्, cb);

	प्रत्यागम git_default_config(var, मानम्, ctx, cb);
}

पूर्णाङ्क cmd_read_वृक्षः(पूर्णाङ्क argc,
		  ध्रुव वर्ण **argv,
		  ध्रुव वर्ण *cmd_उपसर्गः,
		  संरचना repository *repo अप्रयुक्तम्)
{
	पूर्णाङ्क i, stage = 0;
	संरचना object_id oid;
	संरचना वृक्षः_desc t[MAX_UNPACK_TREES];
	संरचना unpack_trees_options opts;
	पूर्णाङ्क उपसर्गः_समुच्चयः = 0;
	संरचना तालकम्_file तालकम्_file = तालकम्_INIT;
	ध्रुव संरचना option read_वृक्षः_options[] = {
		OPT_SUPER_उपसर्गः(&opts.super_उपसर्गः),
		OPT_CALLBACK_F(0, "index-output", रिक्त, N_("file"),
		  N_("write resulting index to <file>"),
		  PARSE_OPT_NONEG, सूचकांक_output_cb),
		OPT_BOOL(0, "empty", &read_रिक्तानि,
			    N_("only empty the index")),
		OPT__VERBOSE(&opts.verbose_update, N_("be verbose")),
		OPT_गणः(N_("Merging")),
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
			.मानम् = &opts.उपसर्गः,
			.argh = N_("<subdirectory>/"),
			.help = N_("read the tree into the index under <subdirectory>/"),
			.flags = PARSE_OPT_NONEG,
		},
		OPT_BOOL('u', रिक्त, &opts.update,
			 N_("update working tree with merge result")),
		OPT_CALLBACK_F(0, "exclude-per-directory", &opts,
		  N_("gitignore"),
		  N_("allow explicitly ignored files to be overwritten"),
		  PARSE_OPT_NONEG, exclude_प्रत्येकम्_directory_cb),
		OPT_BOOL('i', रिक्त, &opts.सूचकांक_only,
			 N_("don't check the working tree after merging")),
		OPT_DRY_सततम्(&opts.dry_सततम्, N_("don't update the index or the work tree")),
		OPT_BOOL(0, "no-sparse-checkout", &opts.skip_sparse_checkout,
			 N_("skip applying sparse checkout filter")),
		OPT_BOOL(0, "debug-unpack", &opts.आन्तरिकम्.debug_unpack,
			 N_("debug unpack-trees")),
		OPT_CALLBACK_F(0, "recurse-submodules", रिक्त,
			    "checkout", "control recursive updating of submodules",
			    PARSE_OPT_OPTARG, option_parse_recurse_submodules_worktree_updater),
		OPT__QUIET(&opts.quiet, N_("suppress feedback messages")),
		OPT_अन्तः()
	};

	स्मृतिं_पूरय(&opts, 0, परिमाण(opts));
	opts.शीर्ष_idx = -1;
	opts.src_सूचकांक = the_repository->सूचकांक;
	opts.dst_सूचकांक = the_repository->सूचकांक;

	repo_config(the_repository, git_read_वृक्षः_config, रिक्त);

	argc = parse_options(argc, argv, cmd_उपसर्गः, read_वृक्षः_options,
			     read_वृक्षः_usage, 0);

	उपसर्गः_समुच्चयः = opts.उपसर्गः ? 1 : 0;
	यदि (1 < opts.merge + opts.reset + उपसर्गः_समुच्चयः)
		die("Which one? -m, --reset, or --prefix?");

	/* Prefix should not start with a directory separator */
	यदि (opts.उपसर्गः && opts.उपसर्गः[0] == '/')
		die("Invalid prefix, prefix cannot start with '/'");

	यदि (opts.reset)
		opts.reset = UNPACK_RESET_OVERWRITE_UNTRACKED;

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_पूर्णम्_सूचकांक = 0;

	repo_hold_locked_सूचकांक(the_repository, &तालकम्_file, तालकम्_DIE_ON_ERROR);

	/*
	 * NEEDSWORK
	 *
	 * The old index should be read anyway even if we're going to
	 * destroy all index entries because we still need to preserve
	 * certain information such as index version or split-index
	 * mode.
	 */

	यदि (opts.reset || opts.merge || opts.उपसर्गः) {
		यदि (repo_read_सूचकांक_unmerged(the_repository) && (opts.उपसर्गः || opts.merge))
			die(_("You need to resolve your current index first"));
		stage = opts.merge = 1;
	}
	resolve_undo_clear_सूचकांक(the_repository->सूचकांक);

	प्रति (i = 0; i < argc; i++) {
		ध्रुव वर्ण *arg = argv[i];

		यदि (repo_get_oid(the_repository, arg, &oid))
			die("Not a valid object name %s", arg);
		यदि (list_वृक्षः(&oid) < 0)
			die("failed to unpack tree object %s", arg);
		stage++;
	}
	यदि (!nr_trees && !read_रिक्तानि && !opts.merge)
		warning("read-tree: emptying the index with no arguments is deprecated; use --empty");
	अन्यथा यदि (nr_trees > 0 && read_रिक्तानि)
		die("passing trees as arguments contradicts --empty");

	यदि (1 < opts.सूचकांक_only + opts.update)
		die("-u and -i at the same time makes no sense");
	यदि ((opts.update || opts.सूचकांक_only) && !opts.merge)
		die("%s is meaningless without -m, --reset, or --prefix",
		    opts.update ? "-u" : "-i");
	यदि (opts.update && !opts.reset)
		opts.preserve_ignored = 0;
	/* otherwise, opts.preserve_ignored is irrelevant */
	यदि (opts.merge && !opts.सूचकांक_only)
		सज्जता_work_वृक्षः(the_repository);

	यदि (opts.skip_sparse_checkout)
		ensure_पूर्णम्_सूचकांक(the_repository->सूचकांक);

	यदि (opts.merge) {
		विकल्प (stage - 1) {
		पक्ष 0:
			die("you must specify at least one tree to merge");
			भङ्ग;
		पक्ष 1:
			opts.fn = opts.उपसर्गः ? bind_merge : oneway_merge;
			भङ्ग;
		पक्ष 2:
			opts.fn = twoway_merge;
			opts.initial_checkout = is_सूचकांक_unborn(the_repository->सूचकांक);
			भङ्ग;
		पक्ष 3:
		स्वभाव:
			opts.fn = threeway_merge;
			भङ्ग;
		}

		यदि (stage - 1 >= 3)
			opts.शीर्ष_idx = stage - 2;
		अन्यथा
			opts.शीर्ष_idx = 1;
	}

	यदि (opts.आन्तरिकम्.debug_unpack)
		opts.fn = debug_merge;

	/* If we're going to prime_cache_tree later, skip cache tree update */
	यदि (nr_trees == 1 && !opts.उपसर्गः)
		opts.skip_cache_वृक्षः_update = 1;

	cache_वृक्षः_free(&the_repository->सूचकांक->cache_वृक्षः);
	प्रति (i = 0; i < nr_trees; i++) {
		संरचना वृक्षः *वृक्षः = trees[i];
		यदि (repo_parse_वृक्षः(the_repository, वृक्षः) < 0)
			प्रत्यागम 128;
		init_वृक्षः_desc(t+i, &वृक्षः->object.oid, वृक्षः->बफर, वृक्षः->आकार);
	}
	यदि (unpack_trees(nr_trees, t, &opts))
		प्रत्यागम 128;

	यदि (opts.आन्तरिकम्.debug_unpack || opts.dry_सततम्)
		प्रत्यागम 0; /* do not write the index out */

	/*
	 * When reading only one tree (either the most basic form,
	 * "-m ent" or "--reset ent" form), we can obtain a fully
	 * valid cache-tree because the index must match exactly
	 * what came from the tree.
	 */
	यदि (nr_trees == 1 && !opts.उपसर्गः)
		अविभाज्यः_cache_वृक्षः(the_repository,
				 the_repository->सूचकांक,
				 trees[0]);

	यदि (write_locked_सूचकांक(the_repository->सूचकांक, &तालकम्_file, COMMIT_तालकम्))
		die("unable to write new index file");
	प्रत्यागम 0;
}
