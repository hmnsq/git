#define USE_THE_REPOSITORY_VARIABLE
#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "commit.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "hex.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "commit-graph.h"
#அந்தர்பாவ "odb.h"
#அந்தர்பாவ "odb/source.h"
#அந்தர்பாவ "odb/source-files.h"
#அந்தர்பாவ "progress.h"
#அந்தர்பாவ "replace-object.h"
#அந்தர்பாவ "strbuf.h"
#அந்தர்பாவ "tag.h"
#அந்தர்பாவ "trace2.h"

#define BUILTIN_COMMIT_GRAPH_VERIFY_USAGE \
	N_("git commit-graph verify [--object-dir <dir>] [--shallow] [--[no-]progress]")

#define BUILTIN_COMMIT_GRAPH_WRITE_USAGE \
	N_("git commit-graph write [--object-dir <dir>] [--append]\n" \
	   "                       [--split[=<strategy>]] [--reachable | --stdin-packs | --stdin-commits]\n" \
	   "                       [--changed-paths] [--[no-]max-new-filters <n>] [--[no-]progress]\n" \
	   "                       <split-options>")

நிலையான மாறிலி எழுத்து * மாறிலி builtin_commit_graph_verify_usage[] = {
	BUILTIN_COMMIT_GRAPH_VERIFY_USAGE,
	வெற்று
};

நிலையான மாறிலி எழுத்து * மாறிலி builtin_commit_graph_write_usage[] = {
	BUILTIN_COMMIT_GRAPH_WRITE_USAGE,
	வெற்று
};

நிலையான எழுத்து மாறிலி * மாறிலி builtin_commit_graph_usage[] = {
	BUILTIN_COMMIT_GRAPH_VERIFY_USAGE,
	BUILTIN_COMMIT_GRAPH_WRITE_USAGE,
	வெற்று,
};

நிலையான கட்டமைப்பு opts_commit_graph {
	மாறிலி எழுத்து *obj_dir;
	முழு reachable;
	முழு stdin_packs;
	முழு stdin_commits;
	முழு append;
	முழு split;
	முழு shallow;
	முழு progress;
	முழு enable_changed_paths;
} opts;

நிலையான கட்டமைப்பு option common_opts[] = {
	OPT_STRING(0, "object-dir", &opts.obj_dir,
		   N_("dir"),
		   N_("the object directory to store the graph")),
	OPT_END()
};

நிலையான கட்டமைப்பு option *add_common_options(கட்டமைப்பு option *to)
{
	திரும்பு parse_options_concat(common_opts, to);
}

நிலையான முழு graph_verify(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
			கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு commit_graph *graph = வெற்று;
	கட்டமைப்பு odb_files_dir *dir = வெற்று;
	எழுத்து *graph_name;
	எழுத்து *chain_name;
	எண்ணிக்கை { OPENED_NONE, OPENED_GRAPH, OPENED_CHAIN } opened = OPENED_NONE;
	முழு fd;
	கட்டமைப்பு stat st;
	முழு flags = 0;
	முழு incomplete_chain = 0;
	முழு ret;

	நிலையான கட்டமைப்பு option builtin_commit_graph_verify_options[] = {
		OPT_BOOL(0, "shallow", &opts.shallow,
			 N_("if the commit-graph is split, only verify the tip file")),
		OPT_BOOL(0, "progress", &opts.progress,
			 N_("force progress reporting")),
		OPT_END(),
	};
	கட்டமைப்பு option *options = add_common_options(builtin_commit_graph_verify_options);

	trace2_cmd_mode("verify");

	opts.progress = isatty(2);
	argc = parse_options(argc, argv, prefix,
			     options,
			     builtin_commit_graph_verify_usage, 0);
	எனில் (argc)
		usage_with_options(builtin_commit_graph_verify_usage, options);

	எனில் (!opts.obj_dir)
		opts.obj_dir = repo_get_object_directory(the_repository);
	எனில் (opts.shallow)
		flags |= COMMIT_GRAPH_VERIFY_SHALLOW;
	எனில் (opts.progress)
		flags |= COMMIT_GRAPH_WRITE_PROGRESS;

	dir = odb_source_files_find_dir(the_repository->objects, opts.obj_dir);
	எனில் (!dir)
		die(_("could not find object directory matching %s"), opts.obj_dir);

	graph_name = get_commit_graph_filename(dir->abspath);
	chain_name = get_commit_graph_chain_filename(dir->abspath);
	எனில் (open_commit_graph(graph_name, &fd, &st))
		opened = OPENED_GRAPH;
	இல்லையெனில் எனில் (errno != ENOENT)
		die_errno(_("Could not open commit-graph '%s'"), graph_name);
	இல்லையெனில் எனில் (open_commit_graph_chain(chain_name, &fd, &st,
					 the_repository->hash_algo))
		opened = OPENED_CHAIN;
	இல்லையெனில் எனில் (errno != ENOENT)
		die_errno(_("could not open commit-graph chain '%s'"), chain_name);

	FREE_AND_NULL(graph_name);
	FREE_AND_NULL(chain_name);
	FREE_AND_NULL(options);

	எனில் (opened == OPENED_NONE)
		திரும்பு 0;
	இல்லையெனில் எனில் (opened == OPENED_GRAPH)
		graph = load_commit_graph_one_fd_st(the_repository, dir->abspath, fd, &st);
	இல்லையெனில்
		graph = load_commit_graph_chain_fd_st(the_repository->objects, fd, &st,
						      &incomplete_chain);

	எனில் (!graph)
		திரும்பு 1;

	ret = verify_commit_graph(graph, flags);
	free_commit_graph(graph);

	எனில் (incomplete_chain) {
		error("one or more commit-graph chain files could not be loaded");
		ret |= 1;
	}

	திரும்பு ret;
}

வெளி முழு read_replace_refs;
நிலையான கட்டமைப்பு commit_graph_opts write_opts;

நிலையான முழு write_option_parse_split(மாறிலி கட்டமைப்பு option *opt, மாறிலி எழுத்து *arg,
				    முழு unset)
{
	எண்ணிக்கை commit_graph_split_flags *flags = opt->மதிப்பு;

	BUG_ON_OPT_NEG(unset);

	opts.split = 1;
	எனில் (!arg)
		திரும்பு 0;

	எனில் (!strcmp(arg, "no-merge"))
		*flags = COMMIT_GRAPH_SPLIT_MERGE_PROHIBITED;
	இல்லையெனில் எனில் (!strcmp(arg, "replace"))
		*flags = COMMIT_GRAPH_SPLIT_REPLACE;
	இல்லையெனில்
		die(_("unrecognized --split argument, %s"), arg);

	திரும்பு 0;
}

நிலையான முழு read_one_commit(கட்டமைப்பு oidset *commits, கட்டமைப்பு progress *progress,
			   மாறிலி எழுத்து *hash)
{
	கட்டமைப்பு object *result;
	கட்டமைப்பு object_id oid;
	மாறிலி எழுத்து *end;

	எனில் (parse_oid_hex(hash, &oid, &end))
		திரும்பு error(_("unexpected non-hex object ID: %s"), hash);

	result = deref_tag(the_repository, parse_object(the_repository, &oid),
			   வெற்று, 0);
	எனில் (!result)
		திரும்பு error(_("invalid object: %s"), hash);
	இல்லையெனில் எனில் (object_as_type(result, OBJ_COMMIT, 1))
		oidset_செருகு(commits, &result->oid);

	display_progress(progress, oidset_size(commits));

	திரும்பு 0;
}

நிலையான முழு write_option_max_new_filters(மாறிலி கட்டமைப்பு option *opt,
					மாறிலி எழுத்து *arg,
					முழு unset)
{
	முழு *to = opt->மதிப்பு;
	எனில் (unset)
		*to = -1;
	இல்லையெனில் {
		மாறிலி எழுத்து *s;
		*to = strtol(arg, (எழுத்து **)&s, 10);
		எனில் (*s)
			திரும்பு error(_("option `%s' expects a numerical value"),
				     "max-new-filters");
	}
	திரும்பு 0;
}

நிலையான முழு git_commit_graph_write_config(மாறிலி எழுத்து *var, மாறிலி எழுத்து *மதிப்பு,
					 மாறிலி கட்டமைப்பு config_context *ctx,
					 வெறுமை *cb UNUSED)
{
	எனில் (!strcmp(var, "commitgraph.maxnewfilters"))
		write_opts.max_new_filters = git_config_int(var, மதிப்பு, ctx->kvi);
	இல்லையெனில் எனில் (!strcmp(var, "commitgraph.changedpaths"))
		opts.enable_changed_paths = git_config_bool(var, மதிப்பு) ? 1 : -1;
	/*
	 * No need to fall-back to 'git_default_config', since this was already
	 * called in 'cmd_commit_graph()'.
	 */
	திரும்பு 0;
}

நிலையான முழு graph_write(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
		       கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு string_list pack_indexes = STRING_LIST_INIT_DUP;
	கட்டமைப்பு strbuf buf = STRBUF_INIT;
	கட்டமைப்பு oidset commits = OIDSET_INIT;
	கட்டமைப்பு odb_files_dir *dir = வெற்று;
	முழு result = 0;
	எண்ணிக்கை commit_graph_write_flags flags = 0;
	கட்டமைப்பு progress *progress = வெற்று;

	நிலையான கட்டமைப்பு option builtin_commit_graph_write_options[] = {
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
		OPT_CALLBACK_F(0, "split", &write_opts.split_flags, வெற்று,
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
			வெற்று, N_("maximum number of changed-path Bloom filters to compute"),
			0, write_option_max_new_filters),
		OPT_BOOL(0, "progress", &opts.progress,
			 N_("force progress reporting")),
		OPT_END(),
	};
	கட்டமைப்பு option *options = add_common_options(builtin_commit_graph_write_options);

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
	எனில் (argc)
		usage_with_options(builtin_commit_graph_write_usage, options);

	எனில் (opts.reachable + opts.stdin_packs + opts.stdin_commits > 1)
		die(_("use at most one of --reachable, --stdin-commits, or --stdin-packs"));
	எனில் (!opts.obj_dir)
		opts.obj_dir = repo_get_object_directory(the_repository);
	எனில் (opts.append)
		flags |= COMMIT_GRAPH_WRITE_APPEND;
	எனில் (opts.split)
		flags |= COMMIT_GRAPH_WRITE_SPLIT;
	எனில் (opts.progress)
		flags |= COMMIT_GRAPH_WRITE_PROGRESS;
	எனில் (!opts.enable_changed_paths)
		flags |= COMMIT_GRAPH_NO_WRITE_BLOOM_FILTERS;
	எனில் (opts.enable_changed_paths == 1 ||
	    git_env_bool(GIT_TEST_COMMIT_GRAPH_CHANGED_PATHS, 0))
		flags |= COMMIT_GRAPH_WRITE_BLOOM_FILTERS;

	dir = odb_source_files_find_dir(the_repository->objects, opts.obj_dir);
	எனில் (!dir)
		die(_("could not find object directory matching %s"), opts.obj_dir);

	எனில் (opts.reachable) {
		எனில் (write_commit_graph_reachable(the_repository, dir->abspath, flags, &write_opts))
			result = 1;
		செல் cleanup;
	}

	எனில் (opts.stdin_packs) {
		கட்டமைப்பு strbuf packname = STRBUF_INIT;
		size_t dirlen;

		strbuf_addf(&packname, "%s/pack/", dir->abspath);
		dirlen = packname.len;

		போது (strbuf_getline(&buf, stdin) != EOF) {
			strbuf_setlen(&packname, dirlen);
			strbuf_addbuf(&packname, &buf);
			string_list_append(&pack_indexes, packname.buf);
		}

		strbuf_release(&packname);
	} இல்லையெனில் எனில் (opts.stdin_commits) {
		oidset_init(&commits, 0);
		எனில் (opts.progress)
			progress = start_delayed_progress(
				the_repository,
				_("Collecting commits from input"), 0);

		போது (strbuf_getline(&buf, stdin) != EOF) {
			எனில் (read_one_commit(&commits, progress, buf.buf)) {
				result = 1;
				stop_progress(&progress);
				செல் cleanup;
			}
		}

		stop_progress(&progress);
	}

	எனில் (write_commit_graph(the_repository, dir->abspath,
			       opts.stdin_packs ? &pack_indexes : வெற்று,
			       opts.stdin_commits ? &commits : வெற்று,
			       flags,
			       &write_opts))
		result = 1;

cleanup:
	FREE_AND_NULL(options);
	string_list_clear(&pack_indexes, 0);
	strbuf_release(&buf);
	oidset_clear(&commits);
	திரும்பு result;
}

முழு cmd_commit_graph(முழு argc,
		     மாறிலி எழுத்து **argv,
		     மாறிலி எழுத்து *prefix,
		     கட்டமைப்பு repository *repo)
{
	parse_opt_subcommand_fn *fn = வெற்று;
	கட்டமைப்பு option builtin_commit_graph_options[] = {
		OPT_SUBCOMMAND("verify", &fn, graph_verify),
		OPT_SUBCOMMAND("write", &fn, graph_write),
		OPT_END(),
	};
	கட்டமைப்பு option *options = parse_options_concat(builtin_commit_graph_options, common_opts);

	repo_config(the_repository, git_default_config, வெற்று);

	disable_replace_refs();
	save_commit_buffer = 0;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_commit_graph_usage, 0);
	FREE_AND_NULL(options);

	திரும்பு fn(argc, argv, prefix, repo);
}
