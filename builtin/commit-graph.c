#define USE_THE_REPOSITORY_VARIABLE
#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "commit.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "hex.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "commit-graph.h"
#ଅନ୍ତର୍ଭାଵ "odb.h"
#ଅନ୍ତର୍ଭାଵ "odb/source.h"
#ଅନ୍ତର୍ଭାଵ "odb/source-files.h"
#ଅନ୍ତର୍ଭାଵ "progress.h"
#ଅନ୍ତର୍ଭାଵ "replace-object.h"
#ଅନ୍ତର୍ଭାଵ "strbuf.h"
#ଅନ୍ତର୍ଭାଵ "tag.h"
#ଅନ୍ତର୍ଭାଵ "trace2.h"

#define BUILTIN_COMMIT_GRAPH_VERIFY_USAGE \
	N_("git commit-graph verify [--object-dir <dir>] [--shallow] [--[no-]progress]")

#define BUILTIN_COMMIT_GRAPH_WRITE_USAGE \
	N_("git commit-graph write [--object-dir <dir>] [--append]\n" \
	   "                       [--split[=<strategy>]] [--reachable | --stdin-packs | --stdin-commits]\n" \
	   "                       [--changed-paths] [--[no-]max-new-filters <n>] [--[no-]progress]\n" \
	   "                       <split-options>")

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ builtin_commit_graph_verify_usage[] = {
	BUILTIN_COMMIT_GRAPH_VERIFY_USAGE,
	ଖାଲି
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ builtin_commit_graph_write_usage[] = {
	BUILTIN_COMMIT_GRAPH_WRITE_USAGE,
	ଖାଲି
};

ସ୍ଥିର ଅକ୍ଷର ଅଚଳ * ଅଚଳ builtin_commit_graph_usage[] = {
	BUILTIN_COMMIT_GRAPH_VERIFY_USAGE,
	BUILTIN_COMMIT_GRAPH_WRITE_USAGE,
	ଖାଲି,
};

ସ୍ଥିର ସଂରଚନା opts_commit_graph {
	ଅଚଳ ଅକ୍ଷର *obj_dir;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା reachable;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା stdin_packs;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା stdin_commits;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା append;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା split;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା shallow;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା progress;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା enable_changed_paths;
} opts;

ସ୍ଥିର ସଂରଚନା option common_opts[] = {
	OPT_STRING(0, "object-dir", &opts.obj_dir,
		   N_("dir"),
		   N_("the object directory to store the graph")),
	OPT_END()
};

ସ୍ଥିର ସଂରଚନା option *add_common_options(ସଂରଚନା option *to)
{
	ଫେରନ୍ତୁ parse_options_concat(common_opts, to);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା graph_verify(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
			ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା commit_graph *graph = ଖାଲି;
	ସଂରଚନା odb_files_dir *dir = ଖାଲି;
	ଅକ୍ଷର *graph_name;
	ଅକ୍ଷର *chain_name;
	ଗଣନା { OPENED_NONE, OPENED_GRAPH, OPENED_CHAIN } opened = OPENED_NONE;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd;
	ସଂରଚନା stat st;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା incomplete_chain = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	ସ୍ଥିର ସଂରଚନା option builtin_commit_graph_verify_options[] = {
		OPT_BOOL(0, "shallow", &opts.shallow,
			 N_("if the commit-graph is split, only verify the tip file")),
		OPT_BOOL(0, "progress", &opts.progress,
			 N_("force progress reporting")),
		OPT_END(),
	};
	ସଂରଚନା option *options = add_common_options(builtin_commit_graph_verify_options);

	trace2_cmd_mode("verify");

	opts.progress = isatty(2);
	argc = parse_options(argc, argv, prefix,
			     options,
			     builtin_commit_graph_verify_usage, 0);
	ଯଦି (argc)
		usage_with_options(builtin_commit_graph_verify_usage, options);

	ଯଦି (!opts.obj_dir)
		opts.obj_dir = repo_get_object_directory(the_repository);
	ଯଦି (opts.shallow)
		flags |= COMMIT_GRAPH_VERIFY_SHALLOW;
	ଯଦି (opts.progress)
		flags |= COMMIT_GRAPH_WRITE_PROGRESS;

	dir = odb_source_files_find_dir(the_repository->objects, opts.obj_dir);
	ଯଦି (!dir)
		die(_("could not find object directory matching %s"), opts.obj_dir);

	graph_name = get_commit_graph_filename(dir->abspath);
	chain_name = get_commit_graph_chain_filename(dir->abspath);
	ଯଦି (open_commit_graph(graph_name, &fd, &st))
		opened = OPENED_GRAPH;
	ଅନ୍ୟଥା ଯଦି (errno != ENOENT)
		die_errno(_("Could not open commit-graph '%s'"), graph_name);
	ଅନ୍ୟଥା ଯଦି (open_commit_graph_chain(chain_name, &fd, &st,
					 the_repository->hash_algo))
		opened = OPENED_CHAIN;
	ଅନ୍ୟଥା ଯଦି (errno != ENOENT)
		die_errno(_("could not open commit-graph chain '%s'"), chain_name);

	FREE_AND_NULL(graph_name);
	FREE_AND_NULL(chain_name);
	FREE_AND_NULL(options);

	ଯଦି (opened == OPENED_NONE)
		ଫେରନ୍ତୁ 0;
	ଅନ୍ୟଥା ଯଦି (opened == OPENED_GRAPH)
		graph = load_commit_graph_one_fd_st(the_repository, dir->abspath, fd, &st);
	ଅନ୍ୟଥା
		graph = load_commit_graph_chain_fd_st(the_repository->objects, fd, &st,
						      &incomplete_chain);

	ଯଦି (!graph)
		ଫେରନ୍ତୁ 1;

	ret = verify_commit_graph(graph, flags);
	free_commit_graph(graph);

	ଯଦି (incomplete_chain) {
		error("one or more commit-graph chain files could not be loaded");
		ret |= 1;
	}

	ଫେରନ୍ତୁ ret;
}

ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା read_replace_refs;
ସ୍ଥିର ସଂରଚନା commit_graph_opts write_opts;

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା write_option_parse_split(ଅଚଳ ସଂରଚନା option *opt, ଅଚଳ ଅକ୍ଷର *arg,
				    ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ଗଣନା commit_graph_split_flags *flags = opt->ମୂଲ୍ୟ;

	BUG_ON_OPT_NEG(unset);

	opts.split = 1;
	ଯଦି (!arg)
		ଫେରନ୍ତୁ 0;

	ଯଦି (!strcmp(arg, "no-merge"))
		*flags = COMMIT_GRAPH_SPLIT_MERGE_PROHIBITED;
	ଅନ୍ୟଥା ଯଦି (!strcmp(arg, "replace"))
		*flags = COMMIT_GRAPH_SPLIT_REPLACE;
	ଅନ୍ୟଥା
		die(_("unrecognized --split argument, %s"), arg);

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା read_one_commit(ସଂରଚନା oidset *commits, ସଂରଚନା progress *progress,
			   ଅଚଳ ଅକ୍ଷର *hash)
{
	ସଂରଚନା object *result;
	ସଂରଚନା object_id oid;
	ଅଚଳ ଅକ୍ଷର *end;

	ଯଦି (parse_oid_hex(hash, &oid, &end))
		ଫେରନ୍ତୁ error(_("unexpected non-hex object ID: %s"), hash);

	result = deref_tag(the_repository, parse_object(the_repository, &oid),
			   ଖାଲି, 0);
	ଯଦି (!result)
		ଫେରନ୍ତୁ error(_("invalid object: %s"), hash);
	ଅନ୍ୟଥା ଯଦି (object_as_type(result, OBJ_COMMIT, 1))
		oidset_ଭର୍ତ୍ତି(commits, &result->oid);

	display_progress(progress, oidset_size(commits));

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା write_option_max_new_filters(ଅଚଳ ସଂରଚନା option *opt,
					ଅଚଳ ଅକ୍ଷର *arg,
					ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା *to = opt->ମୂଲ୍ୟ;
	ଯଦି (unset)
		*to = -1;
	ଅନ୍ୟଥା {
		ଅଚଳ ଅକ୍ଷର *s;
		*to = strtol(arg, (ଅକ୍ଷର **)&s, 10);
		ଯଦି (*s)
			ଫେରନ୍ତୁ error(_("option `%s' expects a numerical value"),
				     "max-new-filters");
	}
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା git_commit_graph_write_config(ଅଚଳ ଅକ୍ଷର *var, ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ,
					 ଅଚଳ ସଂରଚନା config_context *ctx,
					 ଶୂନ୍ୟ *cb UNUSED)
{
	ଯଦି (!strcmp(var, "commitgraph.maxnewfilters"))
		write_opts.max_new_filters = git_config_int(var, ମୂଲ୍ୟ, ctx->kvi);
	ଅନ୍ୟଥା ଯଦି (!strcmp(var, "commitgraph.changedpaths"))
		opts.enable_changed_paths = git_config_bool(var, ମୂଲ୍ୟ) ? 1 : -1;
	/*
	 * No need to fall-back to 'git_default_config', since this was already
	 * called in 'cmd_commit_graph()'.
	 */
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା graph_write(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
		       ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା string_list pack_indexes = STRING_LIST_INIT_DUP;
	ସଂରଚନା strbuf buf = STRBUF_INIT;
	ସଂରଚନା oidset commits = OIDSET_INIT;
	ସଂରଚନା odb_files_dir *dir = ଖାଲି;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା result = 0;
	ଗଣନା commit_graph_write_flags flags = 0;
	ସଂରଚନା progress *progress = ଖାଲି;

	ସ୍ଥିର ସଂରଚନା option builtin_commit_graph_write_options[] = {
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
		OPT_CALLBACK_F(0, "split", &write_opts.split_flags, ଖାଲି,
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
			ଖାଲି, N_("maximum number of changed-path Bloom filters to compute"),
			0, write_option_max_new_filters),
		OPT_BOOL(0, "progress", &opts.progress,
			 N_("force progress reporting")),
		OPT_END(),
	};
	ସଂରଚନା option *options = add_common_options(builtin_commit_graph_write_options);

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
	ଯଦି (argc)
		usage_with_options(builtin_commit_graph_write_usage, options);

	ଯଦି (opts.reachable + opts.stdin_packs + opts.stdin_commits > 1)
		die(_("use at most one of --reachable, --stdin-commits, or --stdin-packs"));
	ଯଦି (!opts.obj_dir)
		opts.obj_dir = repo_get_object_directory(the_repository);
	ଯଦି (opts.append)
		flags |= COMMIT_GRAPH_WRITE_APPEND;
	ଯଦି (opts.split)
		flags |= COMMIT_GRAPH_WRITE_SPLIT;
	ଯଦି (opts.progress)
		flags |= COMMIT_GRAPH_WRITE_PROGRESS;
	ଯଦି (!opts.enable_changed_paths)
		flags |= COMMIT_GRAPH_NO_WRITE_BLOOM_FILTERS;
	ଯଦି (opts.enable_changed_paths == 1 ||
	    git_env_bool(GIT_TEST_COMMIT_GRAPH_CHANGED_PATHS, 0))
		flags |= COMMIT_GRAPH_WRITE_BLOOM_FILTERS;

	dir = odb_source_files_find_dir(the_repository->objects, opts.obj_dir);
	ଯଦି (!dir)
		die(_("could not find object directory matching %s"), opts.obj_dir);

	ଯଦି (opts.reachable) {
		ଯଦି (write_commit_graph_reachable(the_repository, dir->abspath, flags, &write_opts))
			result = 1;
		ଯାଅ cleanup;
	}

	ଯଦି (opts.stdin_packs) {
		ସଂରଚନା strbuf packname = STRBUF_INIT;
		size_t dirlen;

		strbuf_addf(&packname, "%s/pack/", dir->abspath);
		dirlen = packname.len;

		ଯେତେବେଳେ (strbuf_getline(&buf, stdin) != EOF) {
			strbuf_setlen(&packname, dirlen);
			strbuf_addbuf(&packname, &buf);
			string_list_append(&pack_indexes, packname.buf);
		}

		strbuf_release(&packname);
	} ଅନ୍ୟଥା ଯଦି (opts.stdin_commits) {
		oidset_init(&commits, 0);
		ଯଦି (opts.progress)
			progress = start_delayed_progress(
				the_repository,
				_("Collecting commits from input"), 0);

		ଯେତେବେଳେ (strbuf_getline(&buf, stdin) != EOF) {
			ଯଦି (read_one_commit(&commits, progress, buf.buf)) {
				result = 1;
				stop_progress(&progress);
				ଯାଅ cleanup;
			}
		}

		stop_progress(&progress);
	}

	ଯଦି (write_commit_graph(the_repository, dir->abspath,
			       opts.stdin_packs ? &pack_indexes : ଖାଲି,
			       opts.stdin_commits ? &commits : ଖାଲି,
			       flags,
			       &write_opts))
		result = 1;

cleanup:
	FREE_AND_NULL(options);
	string_list_clear(&pack_indexes, 0);
	strbuf_release(&buf);
	oidset_clear(&commits);
	ଫେରନ୍ତୁ result;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_commit_graph(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
		     ଅଚଳ ଅକ୍ଷର **argv,
		     ଅଚଳ ଅକ୍ଷର *prefix,
		     ସଂରଚନା repository *repo)
{
	parse_opt_subcommand_fn *fn = ଖାଲି;
	ସଂରଚନା option builtin_commit_graph_options[] = {
		OPT_SUBCOMMAND("verify", &fn, graph_verify),
		OPT_SUBCOMMAND("write", &fn, graph_write),
		OPT_END(),
	};
	ସଂରଚନା option *options = parse_options_concat(builtin_commit_graph_options, common_opts);

	repo_config(the_repository, git_default_config, ଖାଲି);

	disable_replace_refs();
	save_commit_buffer = 0;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_commit_graph_usage, 0);
	FREE_AND_NULL(options);

	ଫେରନ୍ତୁ fn(argc, argv, prefix, repo);
}
