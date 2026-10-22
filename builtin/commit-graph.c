#घोषणा USE_THE_REPOSITORY_VARIABLE
#समावेश "builtin.h"
#समावेश "commit.h"
#समावेश "config.h"
#समावेश "environment.h"
#समावेश "gettext.h"
#समावेश "hex.h"
#समावेश "parse-options.h"
#समावेश "commit-graph.h"
#समावेश "odb.h"
#समावेश "odb/source.h"
#समावेश "odb/source-files.h"
#समावेश "progress.h"
#समावेश "replace-object.h"
#समावेश "strbuf.h"
#समावेश "tag.h"
#समावेश "trace2.h"

#घोषणा BUILTIN_COMMIT_GRAPH_VERIFY_USAGE \
	N_("git commit-graph verify [--object-dir <dir>] [--shallow] [--[no-]progress]")

#घोषणा BUILTIN_COMMIT_GRAPH_WRITE_USAGE \
	N_("git commit-graph write [--object-dir <dir>] [--append]\n" \
	   "                       [--split[=<strategy>]] [--reachable | --stdin-packs | --stdin-commits]\n" \
	   "                       [--changed-paths] [--[no-]max-new-filters <n>] [--[no-]progress]\n" \
	   "                       <split-options>")

अटल स्थिर अक्षर * स्थिर builtin_commit_graph_verify_usage[] = {
	BUILTIN_COMMIT_GRAPH_VERIFY_USAGE,
	शून्य
};

अटल स्थिर अक्षर * स्थिर builtin_commit_graph_write_usage[] = {
	BUILTIN_COMMIT_GRAPH_WRITE_USAGE,
	शून्य
};

अटल अक्षर स्थिर * स्थिर builtin_commit_graph_usage[] = {
	BUILTIN_COMMIT_GRAPH_VERIFY_USAGE,
	BUILTIN_COMMIT_GRAPH_WRITE_USAGE,
	शून्य,
};

अटल काष्ठा opts_commit_graph {
	स्थिर अक्षर *obj_dir;
	पूर्णांक reachable;
	पूर्णांक stdin_packs;
	पूर्णांक stdin_commits;
	पूर्णांक append;
	पूर्णांक split;
	पूर्णांक shallow;
	पूर्णांक progress;
	पूर्णांक enable_changed_paths;
} opts;

अटल काष्ठा option common_opts[] = {
	OPT_STRING(0, "object-dir", &opts.obj_dir,
		   N_("dir"),
		   N_("the object directory to store the graph")),
	OPT_END()
};

अटल काष्ठा option *add_common_options(काष्ठा option *to)
{
	वापस parse_options_concat(common_opts, to);
}

अटल पूर्णांक graph_verify(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
			काष्ठा repository *repo UNUSED)
{
	काष्ठा commit_graph *graph = शून्य;
	काष्ठा odb_files_dir *dir = शून्य;
	अक्षर *graph_name;
	अक्षर *chain_name;
	क्रमागत { OPENED_NONE, OPENED_GRAPH, OPENED_CHAIN } opened = OPENED_NONE;
	पूर्णांक fd;
	काष्ठा stat st;
	पूर्णांक flags = 0;
	पूर्णांक incomplete_chain = 0;
	पूर्णांक ret;

	अटल काष्ठा option builtin_commit_graph_verify_options[] = {
		OPT_BOOL(0, "shallow", &opts.shallow,
			 N_("if the commit-graph is split, only verify the tip file")),
		OPT_BOOL(0, "progress", &opts.progress,
			 N_("force progress reporting")),
		OPT_END(),
	};
	काष्ठा option *options = add_common_options(builtin_commit_graph_verify_options);

	trace2_cmd_mode("verify");

	opts.progress = isatty(2);
	argc = parse_options(argc, argv, prefix,
			     options,
			     builtin_commit_graph_verify_usage, 0);
	अगर (argc)
		usage_with_options(builtin_commit_graph_verify_usage, options);

	अगर (!opts.obj_dir)
		opts.obj_dir = repo_get_object_directory(the_repository);
	अगर (opts.shallow)
		flags |= COMMIT_GRAPH_VERIFY_SHALLOW;
	अगर (opts.progress)
		flags |= COMMIT_GRAPH_WRITE_PROGRESS;

	dir = odb_source_files_find_dir(the_repository->objects, opts.obj_dir);
	अगर (!dir)
		die(_("could not find object directory matching %s"), opts.obj_dir);

	graph_name = get_commit_graph_filename(dir->abspath);
	chain_name = get_commit_graph_chain_filename(dir->abspath);
	अगर (open_commit_graph(graph_name, &fd, &st))
		opened = OPENED_GRAPH;
	अन्यथा अगर (त्रुटि_सं != ENOENT)
		die_errno(_("Could not open commit-graph '%s'"), graph_name);
	अन्यथा अगर (open_commit_graph_chain(chain_name, &fd, &st,
					 the_repository->hash_algo))
		opened = OPENED_CHAIN;
	अन्यथा अगर (त्रुटि_सं != ENOENT)
		die_errno(_("could not open commit-graph chain '%s'"), chain_name);

	FREE_AND_NULL(graph_name);
	FREE_AND_NULL(chain_name);
	FREE_AND_NULL(options);

	अगर (opened == OPENED_NONE)
		वापस 0;
	अन्यथा अगर (opened == OPENED_GRAPH)
		graph = load_commit_graph_one_fd_st(the_repository, dir->abspath, fd, &st);
	अन्यथा
		graph = load_commit_graph_chain_fd_st(the_repository->objects, fd, &st,
						      &incomplete_chain);

	अगर (!graph)
		वापस 1;

	ret = verify_commit_graph(graph, flags);
	free_commit_graph(graph);

	अगर (incomplete_chain) {
		error("one or more commit-graph chain files could not be loaded");
		ret |= 1;
	}

	वापस ret;
}

बाह्य पूर्णांक read_replace_refs;
अटल काष्ठा commit_graph_opts write_opts;

अटल पूर्णांक write_option_parse_split(स्थिर काष्ठा option *opt, स्थिर अक्षर *arg,
				    पूर्णांक unset)
{
	क्रमागत commit_graph_split_flags *flags = opt->मूल्य;

	BUG_ON_OPT_NEG(unset);

	opts.split = 1;
	अगर (!arg)
		वापस 0;

	अगर (!म_भेद(arg, "no-merge"))
		*flags = COMMIT_GRAPH_SPLIT_MERGE_PROHIBITED;
	अन्यथा अगर (!म_भेद(arg, "replace"))
		*flags = COMMIT_GRAPH_SPLIT_REPLACE;
	अन्यथा
		die(_("unrecognized --split argument, %s"), arg);

	वापस 0;
}

अटल पूर्णांक read_one_commit(काष्ठा oidset *commits, काष्ठा progress *progress,
			   स्थिर अक्षर *hash)
{
	काष्ठा object *परिणाम;
	काष्ठा object_id oid;
	स्थिर अक्षर *end;

	अगर (parse_oid_hex(hash, &oid, &end))
		वापस error(_("unexpected non-hex object ID: %s"), hash);

	परिणाम = deref_tag(the_repository, parse_object(the_repository, &oid),
			   शून्य, 0);
	अगर (!परिणाम)
		वापस error(_("invalid object: %s"), hash);
	अन्यथा अगर (object_as_type(परिणाम, OBJ_COMMIT, 1))
		oidset_जोड़ो(commits, &परिणाम->oid);

	display_progress(progress, oidset_आकार(commits));

	वापस 0;
}

अटल पूर्णांक write_option_max_new_filters(स्थिर काष्ठा option *opt,
					स्थिर अक्षर *arg,
					पूर्णांक unset)
{
	पूर्णांक *to = opt->मूल्य;
	अगर (unset)
		*to = -1;
	अन्यथा {
		स्थिर अक्षर *s;
		*to = म_से_दीर्घ(arg, (अक्षर **)&s, 10);
		अगर (*s)
			वापस error(_("option `%s' expects a numerical value"),
				     "max-new-filters");
	}
	वापस 0;
}

अटल पूर्णांक git_commit_graph_write_config(स्थिर अक्षर *var, स्थिर अक्षर *मूल्य,
					 स्थिर काष्ठा config_context *ctx,
					 व्योम *cb UNUSED)
{
	अगर (!म_भेद(var, "commitgraph.maxnewfilters"))
		write_opts.max_new_filters = git_config_int(var, मूल्य, ctx->kvi);
	अन्यथा अगर (!म_भेद(var, "commitgraph.changedpaths"))
		opts.enable_changed_paths = git_config_bool(var, मूल्य) ? 1 : -1;
	/*
	 * No need to fall-back to 'git_default_config', since this was already
	 * called in 'cmd_commit_graph()'.
	 */
	वापस 0;
}

अटल पूर्णांक graph_write(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
		       काष्ठा repository *repo UNUSED)
{
	काष्ठा string_list pack_indexes = STRING_LIST_INIT_DUP;
	काष्ठा strbuf buf = STRBUF_INIT;
	काष्ठा oidset commits = OIDSET_INIT;
	काष्ठा odb_files_dir *dir = शून्य;
	पूर्णांक परिणाम = 0;
	क्रमागत commit_graph_write_flags flags = 0;
	काष्ठा progress *progress = शून्य;

	अटल काष्ठा option builtin_commit_graph_write_options[] = {
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
		OPT_CALLBACK_F(0, "split", &write_opts.split_flags, शून्य,
			N_("allow writing an incremental commit-graph file"),
			PARSE_OPT_OPTARG | PARSE_OPT_NONEG,
			write_option_parse_split),
		OPT_INTEGER(0, "max-commits", &write_opts.max_commits,
			N_("maximum number of commits in a non-base split commit-graph")),
		OPT_INTEGER(0, "size-multiple", &write_opts.आकार_multiple,
			N_("maximum ratio between two levels of a split commit-graph")),
		OPT_EXPIRY_DATE(0, "expire-time", &write_opts.expire_time,
			N_("only expire files older than a given date-time")),
		OPT_CALLBACK_F(0, "max-new-filters", &write_opts.max_new_filters,
			शून्य, N_("maximum number of changed-path Bloom filters to compute"),
			0, write_option_max_new_filters),
		OPT_BOOL(0, "progress", &opts.progress,
			 N_("force progress reporting")),
		OPT_END(),
	};
	काष्ठा option *options = add_common_options(builtin_commit_graph_write_options);

	opts.progress = isatty(2);
	opts.enable_changed_paths = -1;
	write_opts.आकार_multiple = 2;
	write_opts.max_commits = 0;
	write_opts.expire_time = 0;
	write_opts.max_new_filters = -1;

	trace2_cmd_mode("write");

	repo_config(the_repository, git_commit_graph_write_config, &opts);

	argc = parse_options(argc, argv, prefix,
			     options,
			     builtin_commit_graph_write_usage, 0);
	अगर (argc)
		usage_with_options(builtin_commit_graph_write_usage, options);

	अगर (opts.reachable + opts.stdin_packs + opts.stdin_commits > 1)
		die(_("use at most one of --reachable, --stdin-commits, or --stdin-packs"));
	अगर (!opts.obj_dir)
		opts.obj_dir = repo_get_object_directory(the_repository);
	अगर (opts.append)
		flags |= COMMIT_GRAPH_WRITE_APPEND;
	अगर (opts.split)
		flags |= COMMIT_GRAPH_WRITE_SPLIT;
	अगर (opts.progress)
		flags |= COMMIT_GRAPH_WRITE_PROGRESS;
	अगर (!opts.enable_changed_paths)
		flags |= COMMIT_GRAPH_NO_WRITE_BLOOM_FILTERS;
	अगर (opts.enable_changed_paths == 1 ||
	    git_env_bool(GIT_TEST_COMMIT_GRAPH_CHANGED_PATHS, 0))
		flags |= COMMIT_GRAPH_WRITE_BLOOM_FILTERS;

	dir = odb_source_files_find_dir(the_repository->objects, opts.obj_dir);
	अगर (!dir)
		die(_("could not find object directory matching %s"), opts.obj_dir);

	अगर (opts.reachable) {
		अगर (write_commit_graph_reachable(the_repository, dir->abspath, flags, &write_opts))
			परिणाम = 1;
		जाओ cleanup;
	}

	अगर (opts.stdin_packs) {
		काष्ठा strbuf packname = STRBUF_INIT;
		माप_प्रकार dirlen;

		strbuf_addf(&packname, "%s/pack/", dir->abspath);
		dirlen = packname.len;

		जबतक (strbuf_getline(&buf, मानक_निवेश) != खातापूर्ण) {
			strbuf_setlen(&packname, dirlen);
			strbuf_addbuf(&packname, &buf);
			string_list_append(&pack_indexes, packname.buf);
		}

		strbuf_release(&packname);
	} अन्यथा अगर (opts.stdin_commits) {
		oidset_init(&commits, 0);
		अगर (opts.progress)
			progress = start_delayed_progress(
				the_repository,
				_("Collecting commits from input"), 0);

		जबतक (strbuf_getline(&buf, मानक_निवेश) != खातापूर्ण) {
			अगर (read_one_commit(&commits, progress, buf.buf)) {
				परिणाम = 1;
				stop_progress(&progress);
				जाओ cleanup;
			}
		}

		stop_progress(&progress);
	}

	अगर (write_commit_graph(the_repository, dir->abspath,
			       opts.stdin_packs ? &pack_indexes : शून्य,
			       opts.stdin_commits ? &commits : शून्य,
			       flags,
			       &write_opts))
		परिणाम = 1;

cleanup:
	FREE_AND_NULL(options);
	string_list_clear(&pack_indexes, 0);
	strbuf_release(&buf);
	oidset_clear(&commits);
	वापस परिणाम;
}

पूर्णांक cmd_commit_graph(पूर्णांक argc,
		     स्थिर अक्षर **argv,
		     स्थिर अक्षर *prefix,
		     काष्ठा repository *repo)
{
	parse_opt_subcommand_fn *fn = शून्य;
	काष्ठा option builtin_commit_graph_options[] = {
		OPT_SUBCOMMAND("verify", &fn, graph_verify),
		OPT_SUBCOMMAND("write", &fn, graph_write),
		OPT_END(),
	};
	काष्ठा option *options = parse_options_concat(builtin_commit_graph_options, common_opts);

	repo_config(the_repository, git_default_config, शून्य);

	disable_replace_refs();
	save_commit_बफर = 0;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_commit_graph_usage, 0);
	FREE_AND_NULL(options);

	वापस fn(argc, argv, prefix, repo);
}
