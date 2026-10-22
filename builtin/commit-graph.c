#define USE_THE_REPOSITORY_VARIABLE
#شامل "builtin.h"
#شامل "commit.h"
#شامل "config.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "hex.h"
#شامل "parse-options.h"
#شامل "commit-graph.h"
#شامل "odb.h"
#شامل "odb/source.h"
#شامل "odb/source-files.h"
#شامل "progress.h"
#شامل "replace-object.h"
#شامل "strbuf.h"
#شامل "tag.h"
#شامل "trace2.h"

#define BUILTIN_COMMIT_GRAPH_VERIFY_USAGE \
	N_("git commit-graph verify [--object-dir <dir>] [--shallow] [--[no-]progress]")

#define BUILTIN_COMMIT_GRAPH_WRITE_USAGE \
	N_("git commit-graph write [--object-dir <dir>] [--append]\n" \
	   "                       [--split[=<strategy>]] [--reachable | --stdin-packs | --stdin-commits]\n" \
	   "                       [--changed-paths] [--[no-]max-new-filters <n>] [--[no-]progress]\n" \
	   "                       <split-options>")

ایستا ثابت نویسه * ثابت builtin_commit_graph_verify_usage[] = {
	BUILTIN_COMMIT_GRAPH_VERIFY_USAGE,
	پوچ
};

ایستا ثابت نویسه * ثابت builtin_commit_graph_write_usage[] = {
	BUILTIN_COMMIT_GRAPH_WRITE_USAGE,
	پوچ
};

ایستا نویسه ثابت * ثابت builtin_commit_graph_usage[] = {
	BUILTIN_COMMIT_GRAPH_VERIFY_USAGE,
	BUILTIN_COMMIT_GRAPH_WRITE_USAGE,
	پوچ,
};

ایستا ساختار opts_commit_graph {
	ثابت نویسه *obj_dir;
	صحیح reachable;
	صحیح stdin_packs;
	صحیح stdin_commits;
	صحیح append;
	صحیح split;
	صحیح shallow;
	صحیح progress;
	صحیح enable_changed_paths;
} opts;

ایستا ساختار option common_opts[] = {
	OPT_STRING(0, "object-dir", &opts.obj_dir,
		   N_("dir"),
		   N_("the object directory to store the graph")),
	OPT_END()
};

ایستا ساختار option *add_common_options(ساختار option *to)
{
	بازگشت parse_options_concat(common_opts, to);
}

ایستا صحیح graph_verify(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
			ساختار repository *repo UNUSED)
{
	ساختار commit_graph *graph = پوچ;
	ساختار odb_files_dir *dir = پوچ;
	نویسه *graph_name;
	نویسه *chain_name;
	شمارش { OPENED_NONE, OPENED_GRAPH, OPENED_CHAIN } opened = OPENED_NONE;
	صحیح fd;
	ساختار stat st;
	صحیح flags = 0;
	صحیح incomplete_chain = 0;
	صحیح ret;

	ایستا ساختار option builtin_commit_graph_verify_options[] = {
		OPT_BOOL(0, "shallow", &opts.shallow,
			 N_("if the commit-graph is split, only verify the tip file")),
		OPT_BOOL(0, "progress", &opts.progress,
			 N_("force progress reporting")),
		OPT_END(),
	};
	ساختار option *options = add_common_options(builtin_commit_graph_verify_options);

	trace2_cmd_mode("verify");

	opts.progress = isatty(2);
	argc = parse_options(argc, argv, prefix,
			     options,
			     builtin_commit_graph_verify_usage, 0);
	اگر (argc)
		usage_with_options(builtin_commit_graph_verify_usage, options);

	اگر (!opts.obj_dir)
		opts.obj_dir = repo_get_object_directory(the_repository);
	اگر (opts.shallow)
		flags |= COMMIT_GRAPH_VERIFY_SHALLOW;
	اگر (opts.progress)
		flags |= COMMIT_GRAPH_WRITE_PROGRESS;

	dir = odb_source_files_find_dir(the_repository->objects, opts.obj_dir);
	اگر (!dir)
		die(_("could not find object directory matching %s"), opts.obj_dir);

	graph_name = get_commit_graph_filename(dir->abspath);
	chain_name = get_commit_graph_chain_filename(dir->abspath);
	اگر (open_commit_graph(graph_name, &fd, &st))
		opened = OPENED_GRAPH;
	وگرنه اگر (errno != ENOENT)
		die_errno(_("Could not open commit-graph '%s'"), graph_name);
	وگرنه اگر (open_commit_graph_chain(chain_name, &fd, &st,
					 the_repository->hash_algo))
		opened = OPENED_CHAIN;
	وگرنه اگر (errno != ENOENT)
		die_errno(_("could not open commit-graph chain '%s'"), chain_name);

	FREE_AND_NULL(graph_name);
	FREE_AND_NULL(chain_name);
	FREE_AND_NULL(options);

	اگر (opened == OPENED_NONE)
		بازگشت 0;
	وگرنه اگر (opened == OPENED_GRAPH)
		graph = load_commit_graph_one_fd_st(the_repository, dir->abspath, fd, &st);
	وگرنه
		graph = load_commit_graph_chain_fd_st(the_repository->objects, fd, &st,
						      &incomplete_chain);

	اگر (!graph)
		بازگشت 1;

	ret = verify_commit_graph(graph, flags);
	free_commit_graph(graph);

	اگر (incomplete_chain) {
		error("one or more commit-graph chain files could not be loaded");
		ret |= 1;
	}

	بازگشت ret;
}

بیرونی صحیح read_replace_refs;
ایستا ساختار commit_graph_opts write_opts;

ایستا صحیح write_option_parse_split(ثابت ساختار option *opt, ثابت نویسه *arg,
				    صحیح unset)
{
	شمارش commit_graph_split_flags *flags = opt->مقدار;

	BUG_ON_OPT_NEG(unset);

	opts.split = 1;
	اگر (!arg)
		بازگشت 0;

	اگر (!strcmp(arg, "no-merge"))
		*flags = COMMIT_GRAPH_SPLIT_MERGE_PROHIBITED;
	وگرنه اگر (!strcmp(arg, "replace"))
		*flags = COMMIT_GRAPH_SPLIT_REPLACE;
	وگرنه
		die(_("unrecognized --split argument, %s"), arg);

	بازگشت 0;
}

ایستا صحیح read_one_commit(ساختار oidset *commits, ساختار progress *progress,
			   ثابت نویسه *hash)
{
	ساختار object *result;
	ساختار object_id oid;
	ثابت نویسه *end;

	اگر (parse_oid_hex(hash, &oid, &end))
		بازگشت error(_("unexpected non-hex object ID: %s"), hash);

	result = deref_tag(the_repository, parse_object(the_repository, &oid),
			   پوچ, 0);
	اگر (!result)
		بازگشت error(_("invalid object: %s"), hash);
	وگرنه اگر (object_as_type(result, OBJ_COMMIT, 1))
		oidset_درج(commits, &result->oid);

	display_progress(progress, oidset_size(commits));

	بازگشت 0;
}

ایستا صحیح write_option_max_new_filters(ثابت ساختار option *opt,
					ثابت نویسه *arg,
					صحیح unset)
{
	صحیح *to = opt->مقدار;
	اگر (unset)
		*to = -1;
	وگرنه {
		ثابت نویسه *s;
		*to = strtol(arg, (نویسه **)&s, 10);
		اگر (*s)
			بازگشت error(_("option `%s' expects a numerical value"),
				     "max-new-filters");
	}
	بازگشت 0;
}

ایستا صحیح git_commit_graph_write_config(ثابت نویسه *var, ثابت نویسه *مقدار,
					 ثابت ساختار config_context *ctx,
					 تهی *cb UNUSED)
{
	اگر (!strcmp(var, "commitgraph.maxnewfilters"))
		write_opts.max_new_filters = git_config_int(var, مقدار, ctx->kvi);
	وگرنه اگر (!strcmp(var, "commitgraph.changedpaths"))
		opts.enable_changed_paths = git_config_bool(var, مقدار) ? 1 : -1;
	/*
	 * No need to fall-back to 'git_default_config', since this was already
	 * called in 'cmd_commit_graph()'.
	 */
	بازگشت 0;
}

ایستا صحیح graph_write(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
		       ساختار repository *repo UNUSED)
{
	ساختار string_list pack_indexes = STRING_LIST_INIT_DUP;
	ساختار strbuf buf = STRBUF_INIT;
	ساختار oidset commits = OIDSET_INIT;
	ساختار odb_files_dir *dir = پوچ;
	صحیح result = 0;
	شمارش commit_graph_write_flags flags = 0;
	ساختار progress *progress = پوچ;

	ایستا ساختار option builtin_commit_graph_write_options[] = {
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
		OPT_CALLBACK_F(0, "split", &write_opts.split_flags, پوچ,
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
			پوچ, N_("maximum number of changed-path Bloom filters to compute"),
			0, write_option_max_new_filters),
		OPT_BOOL(0, "progress", &opts.progress,
			 N_("force progress reporting")),
		OPT_END(),
	};
	ساختار option *options = add_common_options(builtin_commit_graph_write_options);

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
	اگر (argc)
		usage_with_options(builtin_commit_graph_write_usage, options);

	اگر (opts.reachable + opts.stdin_packs + opts.stdin_commits > 1)
		die(_("use at most one of --reachable, --stdin-commits, or --stdin-packs"));
	اگر (!opts.obj_dir)
		opts.obj_dir = repo_get_object_directory(the_repository);
	اگر (opts.append)
		flags |= COMMIT_GRAPH_WRITE_APPEND;
	اگر (opts.split)
		flags |= COMMIT_GRAPH_WRITE_SPLIT;
	اگر (opts.progress)
		flags |= COMMIT_GRAPH_WRITE_PROGRESS;
	اگر (!opts.enable_changed_paths)
		flags |= COMMIT_GRAPH_NO_WRITE_BLOOM_FILTERS;
	اگر (opts.enable_changed_paths == 1 ||
	    git_env_bool(GIT_TEST_COMMIT_GRAPH_CHANGED_PATHS, 0))
		flags |= COMMIT_GRAPH_WRITE_BLOOM_FILTERS;

	dir = odb_source_files_find_dir(the_repository->objects, opts.obj_dir);
	اگر (!dir)
		die(_("could not find object directory matching %s"), opts.obj_dir);

	اگر (opts.reachable) {
		اگر (write_commit_graph_reachable(the_repository, dir->abspath, flags, &write_opts))
			result = 1;
		برو cleanup;
	}

	اگر (opts.stdin_packs) {
		ساختار strbuf packname = STRBUF_INIT;
		size_t dirlen;

		strbuf_addf(&packname, "%s/pack/", dir->abspath);
		dirlen = packname.len;

		وقتی (strbuf_getline(&buf, stdin) != EOF) {
			strbuf_setlen(&packname, dirlen);
			strbuf_addbuf(&packname, &buf);
			string_list_append(&pack_indexes, packname.buf);
		}

		strbuf_release(&packname);
	} وگرنه اگر (opts.stdin_commits) {
		oidset_init(&commits, 0);
		اگر (opts.progress)
			progress = start_delayed_progress(
				the_repository,
				_("Collecting commits from input"), 0);

		وقتی (strbuf_getline(&buf, stdin) != EOF) {
			اگر (read_one_commit(&commits, progress, buf.buf)) {
				result = 1;
				stop_progress(&progress);
				برو cleanup;
			}
		}

		stop_progress(&progress);
	}

	اگر (write_commit_graph(the_repository, dir->abspath,
			       opts.stdin_packs ? &pack_indexes : پوچ,
			       opts.stdin_commits ? &commits : پوچ,
			       flags,
			       &write_opts))
		result = 1;

cleanup:
	FREE_AND_NULL(options);
	string_list_clear(&pack_indexes, 0);
	strbuf_release(&buf);
	oidset_clear(&commits);
	بازگشت result;
}

صحیح cmd_commit_graph(صحیح argc,
		     ثابت نویسه **argv,
		     ثابت نویسه *prefix,
		     ساختار repository *repo)
{
	parse_opt_subcommand_fn *fn = پوچ;
	ساختار option builtin_commit_graph_options[] = {
		OPT_SUBCOMMAND("verify", &fn, graph_verify),
		OPT_SUBCOMMAND("write", &fn, graph_write),
		OPT_END(),
	};
	ساختار option *options = parse_options_concat(builtin_commit_graph_options, common_opts);

	repo_config(the_repository, git_default_config, پوچ);

	disable_replace_refs();
	save_commit_buffer = 0;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_commit_graph_usage, 0);
	FREE_AND_NULL(options);

	بازگشت fn(argc, argv, prefix, repo);
}
