#define USE_THE_REPOSITORY_VARIABLE
#अन्तर्भाव "builtin.h"
#अन्तर्भाव "commit.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "commit-graph.h"
#अन्तर्भाव "odb.h"
#अन्तर्भाव "odb/source.h"
#अन्तर्भाव "odb/source-files.h"
#अन्तर्भाव "progress.h"
#अन्तर्भाव "replace-object.h"
#अन्तर्भाव "strbuf.h"
#अन्तर्भाव "tag.h"
#अन्तर्भाव "trace2.h"

#define BUILTIN_COMMIT_GRAPH_VERIFY_USAGE \
	N_("git commit-graph verify [--object-dir <dir>] [--shallow] [--[no-]progress]")

#define BUILTIN_COMMIT_GRAPH_WRITE_USAGE \
	N_("git commit-graph write [--object-dir <dir>] [--append]\n" \
	   "                       [--split[=<strategy>]] [--reachable | --stdin-packs | --stdin-commits]\n" \
	   "                       [--changed-paths] [--[no-]max-new-filters <n>] [--[no-]progress]\n" \
	   "                       <split-options>")

स्थिर स्थिरांक अक्षर * स्थिरांक builtin_commit_graph_verify_usage[] = {
	BUILTIN_COMMIT_GRAPH_VERIFY_USAGE,
	रिक्त
};

स्थिर स्थिरांक अक्षर * स्थिरांक builtin_commit_graph_write_usage[] = {
	BUILTIN_COMMIT_GRAPH_WRITE_USAGE,
	रिक्त
};

स्थिर अक्षर स्थिरांक * स्थिरांक builtin_commit_graph_usage[] = {
	BUILTIN_COMMIT_GRAPH_VERIFY_USAGE,
	BUILTIN_COMMIT_GRAPH_WRITE_USAGE,
	रिक्त,
};

स्थिर संरचना opts_commit_graph {
	स्थिरांक अक्षर *obj_dir;
	पूर्णांक reachable;
	पूर्णांक stdin_packs;
	पूर्णांक stdin_commits;
	पूर्णांक append;
	पूर्णांक split;
	पूर्णांक shallow;
	पूर्णांक progress;
	पूर्णांक enable_changed_paths;
} opts;

स्थिर संरचना option common_opts[] = {
	OPT_STRING(0, "object-dir", &opts.obj_dir,
		   N_("dir"),
		   N_("the object directory to store the graph")),
	OPT_END()
};

स्थिर संरचना option *add_common_options(संरचना option *to)
{
	परत parse_options_concat(common_opts, to);
}

स्थिर पूर्णांक graph_verify(पूर्णांक argc, स्थिरांक अक्षर **argv, स्थिरांक अक्षर *prefix,
			संरचना repository *repo UNUSED)
{
	संरचना commit_graph *graph = रिक्त;
	संरचना odb_files_dir *dir = रिक्त;
	अक्षर *graph_name;
	अक्षर *chain_name;
	गणना { OPENED_NONE, OPENED_GRAPH, OPENED_CHAIN } opened = OPENED_NONE;
	पूर्णांक fd;
	संरचना stat st;
	पूर्णांक flags = 0;
	पूर्णांक incomplete_chain = 0;
	पूर्णांक ret;

	स्थिर संरचना option builtin_commit_graph_verify_options[] = {
		OPT_BOOL(0, "shallow", &opts.shallow,
			 N_("if the commit-graph is split, only verify the tip file")),
		OPT_BOOL(0, "progress", &opts.progress,
			 N_("force progress reporting")),
		OPT_END(),
	};
	संरचना option *options = add_common_options(builtin_commit_graph_verify_options);

	trace2_cmd_mode("verify");

	opts.progress = isatty(2);
	argc = parse_options(argc, argv, prefix,
			     options,
			     builtin_commit_graph_verify_usage, 0);
	जर (argc)
		usage_with_options(builtin_commit_graph_verify_usage, options);

	जर (!opts.obj_dir)
		opts.obj_dir = repo_get_object_directory(the_repository);
	जर (opts.shallow)
		flags |= COMMIT_GRAPH_VERIFY_SHALLOW;
	जर (opts.progress)
		flags |= COMMIT_GRAPH_WRITE_PROGRESS;

	dir = odb_source_files_find_dir(the_repository->objects, opts.obj_dir);
	जर (!dir)
		die(_("could not find object directory matching %s"), opts.obj_dir);

	graph_name = get_commit_graph_filename(dir->abspath);
	chain_name = get_commit_graph_chain_filename(dir->abspath);
	जर (open_commit_graph(graph_name, &fd, &st))
		opened = OPENED_GRAPH;
	अन्यथा जर (त्रुटि_सं != ENOENT)
		die_errno(_("Could not open commit-graph '%s'"), graph_name);
	अन्यथा जर (open_commit_graph_chain(chain_name, &fd, &st,
					 the_repository->hash_algo))
		opened = OPENED_CHAIN;
	अन्यथा जर (त्रुटि_सं != ENOENT)
		die_errno(_("could not open commit-graph chain '%s'"), chain_name);

	FREE_AND_NULL(graph_name);
	FREE_AND_NULL(chain_name);
	FREE_AND_NULL(options);

	जर (opened == OPENED_NONE)
		परत 0;
	अन्यथा जर (opened == OPENED_GRAPH)
		graph = load_commit_graph_one_fd_st(the_repository, dir->abspath, fd, &st);
	अन्यथा
		graph = load_commit_graph_chain_fd_st(the_repository->objects, fd, &st,
						      &incomplete_chain);

	जर (!graph)
		परत 1;

	ret = verify_commit_graph(graph, flags);
	free_commit_graph(graph);

	जर (incomplete_chain) {
		error("one or more commit-graph chain files could not be loaded");
		ret |= 1;
	}

	परत ret;
}

बाह्य पूर्णांक read_replace_refs;
स्थिर संरचना commit_graph_opts write_opts;

स्थिर पूर्णांक write_option_parse_split(स्थिरांक संरचना option *opt, स्थिरांक अक्षर *arg,
				    पूर्णांक unset)
{
	गणना commit_graph_split_flags *flags = opt->मूल्य;

	BUG_ON_OPT_NEG(unset);

	opts.split = 1;
	जर (!arg)
		परत 0;

	जर (!म_भेद(arg, "no-merge"))
		*flags = COMMIT_GRAPH_SPLIT_MERGE_PROHIBITED;
	अन्यथा जर (!म_भेद(arg, "replace"))
		*flags = COMMIT_GRAPH_SPLIT_REPLACE;
	अन्यथा
		die(_("unrecognized --split argument, %s"), arg);

	परत 0;
}

स्थिर पूर्णांक read_one_commit(संरचना oidset *commits, संरचना progress *progress,
			   स्थिरांक अक्षर *hash)
{
	संरचना object *result;
	संरचना object_id oid;
	स्थिरांक अक्षर *end;

	जर (parse_oid_hex(hash, &oid, &end))
		परत error(_("unexpected non-hex object ID: %s"), hash);

	result = deref_tag(the_repository, parse_object(the_repository, &oid),
			   रिक्त, 0);
	जर (!result)
		परत error(_("invalid object: %s"), hash);
	अन्यथा जर (object_as_type(result, OBJ_COMMIT, 1))
		oidset_घाला(commits, &result->oid);

	display_progress(progress, oidset_size(commits));

	परत 0;
}

स्थिर पूर्णांक write_option_max_new_filters(स्थिरांक संरचना option *opt,
					स्थिरांक अक्षर *arg,
					पूर्णांक unset)
{
	पूर्णांक *to = opt->मूल्य;
	जर (unset)
		*to = -1;
	अन्यथा {
		स्थिरांक अक्षर *s;
		*to = म_से_दीर्घ(arg, (अक्षर **)&s, 10);
		जर (*s)
			परत error(_("option `%s' expects a numerical value"),
				     "max-new-filters");
	}
	परत 0;
}

स्थिर पूर्णांक git_commit_graph_write_config(स्थिरांक अक्षर *var, स्थिरांक अक्षर *मूल्य,
					 स्थिरांक संरचना config_context *ctx,
					 शून्य *cb UNUSED)
{
	जर (!म_भेद(var, "commitgraph.maxnewfilters"))
		write_opts.max_new_filters = git_config_int(var, मूल्य, ctx->kvi);
	अन्यथा जर (!म_भेद(var, "commitgraph.changedpaths"))
		opts.enable_changed_paths = git_config_bool(var, मूल्य) ? 1 : -1;
	/*
	 * No need to fall-back to 'git_default_config', since this was already
	 * called in 'cmd_commit_graph()'.
	 */
	परत 0;
}

स्थिर पूर्णांक graph_write(पूर्णांक argc, स्थिरांक अक्षर **argv, स्थिरांक अक्षर *prefix,
		       संरचना repository *repo UNUSED)
{
	संरचना string_list pack_indexes = STRING_LIST_INIT_DUP;
	संरचना strbuf buf = STRBUF_INIT;
	संरचना oidset commits = OIDSET_INIT;
	संरचना odb_files_dir *dir = रिक्त;
	पूर्णांक result = 0;
	गणना commit_graph_write_flags flags = 0;
	संरचना progress *progress = रिक्त;

	स्थिर संरचना option builtin_commit_graph_write_options[] = {
		OPT_BOOL(0, "reachable", &opts.reachable,
			N_("start walk at all refs")),
		OPT_BOOL(0, "stdin-packs", &opts.stdin_packs,
			N_("scan pack-indexes listed by stdin for commits")),
		OPT_BOOL(0, "stdin-commits", &opts.stdin_commits,
			N_("start walk at commits listed by stdin")),
		OPT_BOOL(0, "append", &opts.append,
			N_("include all commits already in the commit-graph file")),
		OPT_BOOL(0, "changed-paths", &opts.enable_changed_paths,
			N_("enable computation for changed paths")),
		OPT_CALLBACK_F(0, "split", &write_opts.split_flags, रिक्त,
			N_("allow writing an incremental commit-graph file"),
			PARSE_OPT_OPTARG | PARSE_OPT_NONEG,
			write_option_parse_split),
		OPT_INTEGER(0, "max-commits", &write_opts.max_commits,
			N_("maximum number of commits in a non-base split commit-graph")),
		OPT_INTEGER(0, "size-multiple", &write_opts.size_multiple,
			N_("maximum ratio between two levels of a split commit-graph")),
		OPT_EXPIRY_DATE(0, "expire-time", &write_opts.expire_time,
			N_("only expire files older than a given date-time")),
		OPT_CALLBACK_F(0, "max-new-filters", &write_opts.max_new_filters,
			रिक्त, N_("maximum number of changed-path Bloom filters to compute"),
			0, write_option_max_new_filters),
		OPT_BOOL(0, "progress", &opts.progress,
			 N_("force progress reporting")),
		OPT_END(),
	};
	संरचना option *options = add_common_options(builtin_commit_graph_write_options);

	opts.progress = isatty(2);
	opts.enable_changed_paths = -1;
	write_opts.size_multiple = 2;
	write_opts.max_commits = 0;
	write_opts.expire_time = 0;
	write_opts.max_new_filters = -1;

	trace2_cmd_mode("write");

	repo_config(the_repository, git_commit_graph_write_config, &opts);

	argc = parse_options(argc, argv, prefix,
			     options,
			     builtin_commit_graph_write_usage, 0);
	जर (argc)
		usage_with_options(builtin_commit_graph_write_usage, options);

	जर (opts.reachable + opts.stdin_packs + opts.stdin_commits > 1)
		die(_("use at most one of --reachable, --stdin-commits, or --stdin-packs"));
	जर (!opts.obj_dir)
		opts.obj_dir = repo_get_object_directory(the_repository);
	जर (opts.append)
		flags |= COMMIT_GRAPH_WRITE_APPEND;
	जर (opts.split)
		flags |= COMMIT_GRAPH_WRITE_SPLIT;
	जर (opts.progress)
		flags |= COMMIT_GRAPH_WRITE_PROGRESS;
	जर (!opts.enable_changed_paths)
		flags |= COMMIT_GRAPH_NO_WRITE_BLOOM_FILTERS;
	जर (opts.enable_changed_paths == 1 ||
	    git_env_bool(GIT_TEST_COMMIT_GRAPH_CHANGED_PATHS, 0))
		flags |= COMMIT_GRAPH_WRITE_BLOOM_FILTERS;

	dir = odb_source_files_find_dir(the_repository->objects, opts.obj_dir);
	जर (!dir)
		die(_("could not find object directory matching %s"), opts.obj_dir);

	जर (opts.reachable) {
		जर (write_commit_graph_reachable(the_repository, dir->abspath, flags, &write_opts))
			result = 1;
		जा cleanup;
	}

	जर (opts.stdin_packs) {
		संरचना strbuf packname = STRBUF_INIT;
		माप_प्रकार dirlen;

		strbuf_addf(&packname, "%s/pack/", dir->abspath);
		dirlen = packname.len;

		पर्यंत (strbuf_getline(&buf, मानक_निवेश) != खातापूर्ण) {
			strbuf_setlen(&packname, dirlen);
			strbuf_addbuf(&packname, &buf);
			string_list_append(&pack_indexes, packname.buf);
		}

		strbuf_release(&packname);
	} अन्यथा जर (opts.stdin_commits) {
		oidset_init(&commits, 0);
		जर (opts.progress)
			progress = start_delayed_progress(
				the_repository,
				_("Collecting commits from input"), 0);

		पर्यंत (strbuf_getline(&buf, मानक_निवेश) != खातापूर्ण) {
			जर (read_one_commit(&commits, progress, buf.buf)) {
				result = 1;
				stop_progress(&progress);
				जा cleanup;
			}
		}

		stop_progress(&progress);
	}

	जर (write_commit_graph(the_repository, dir->abspath,
			       opts.stdin_packs ? &pack_indexes : रिक्त,
			       opts.stdin_commits ? &commits : रिक्त,
			       flags,
			       &write_opts))
		result = 1;

cleanup:
	FREE_AND_NULL(options);
	string_list_clear(&pack_indexes, 0);
	strbuf_release(&buf);
	oidset_clear(&commits);
	परत result;
}

पूर्णांक cmd_commit_graph(पूर्णांक argc,
		     स्थिरांक अक्षर **argv,
		     स्थिरांक अक्षर *prefix,
		     संरचना repository *repo)
{
	parse_opt_subcommand_fn *fn = रिक्त;
	संरचना option builtin_commit_graph_options[] = {
		OPT_SUBCOMMAND("verify", &fn, graph_verify),
		OPT_SUBCOMMAND("write", &fn, graph_write),
		OPT_END(),
	};
	संरचना option *options = parse_options_concat(builtin_commit_graph_options, common_opts);

	repo_config(the_repository, git_default_config, रिक्त);

	disable_replace_refs();
	save_commit_buffer = 0;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_commit_graph_usage, 0);
	FREE_AND_NULL(options);

	परत fn(argc, argv, prefix, repo);
}
