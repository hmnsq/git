#define USE_THE_REPOSITORY_VARIABLE
#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "commit.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "hex.h"
#অন্তর্ভাব "parse-options.h"
#অন্তর্ভাব "commit-graph.h"
#অন্তর্ভাব "odb.h"
#অন্তর্ভাব "odb/source.h"
#অন্তর্ভাব "odb/source-files.h"
#অন্তর্ভাব "progress.h"
#অন্তর্ভাব "replace-object.h"
#অন্তর্ভাব "strbuf.h"
#অন্তর্ভাব "tag.h"
#অন্তর্ভাব "trace2.h"

#define BUILTIN_COMMIT_GRAPH_VERIFY_USAGE \
	N_("git commit-graph verify [--object-dir <dir>] [--shallow] [--[no-]progress]")

#define BUILTIN_COMMIT_GRAPH_WRITE_USAGE \
	N_("git commit-graph write [--object-dir <dir>] [--append]\n" \
	   "                       [--split[=<strategy>]] [--reachable | --stdin-packs | --stdin-commits]\n" \
	   "                       [--changed-paths] [--[no-]max-new-filters <n>] [--[no-]progress]\n" \
	   "                       <split-options>")

স্থির ধ্রুবক অক্ষর * ধ্রুবক builtin_commit_graph_verify_usage[] = {
	BUILTIN_COMMIT_GRAPH_VERIFY_USAGE,
	ফাঁকা
};

স্থির ধ্রুবক অক্ষর * ধ্রুবক builtin_commit_graph_write_usage[] = {
	BUILTIN_COMMIT_GRAPH_WRITE_USAGE,
	ফাঁকা
};

স্থির অক্ষর ধ্রুবক * ধ্রুবক builtin_commit_graph_usage[] = {
	BUILTIN_COMMIT_GRAPH_VERIFY_USAGE,
	BUILTIN_COMMIT_GRAPH_WRITE_USAGE,
	ফাঁকা,
};

স্থির কাঠামো opts_commit_graph {
	ধ্রুবক অক্ষর *obj_dir;
	পূর্ণসংখ্যা reachable;
	পূর্ণসংখ্যা stdin_packs;
	পূর্ণসংখ্যা stdin_commits;
	পূর্ণসংখ্যা append;
	পূর্ণসংখ্যা split;
	পূর্ণসংখ্যা shallow;
	পূর্ণসংখ্যা progress;
	পূর্ণসংখ্যা enable_changed_paths;
} opts;

স্থির কাঠামো option common_opts[] = {
	OPT_STRING(0, "object-dir", &opts.obj_dir,
		   N_("dir"),
		   N_("the object directory to store the graph")),
	OPT_END()
};

স্থির কাঠামো option *add_common_options(কাঠামো option *to)
{
	ফেরত parse_options_concat(common_opts, to);
}

স্থির পূর্ণসংখ্যা graph_verify(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix,
			কাঠামো repository *repo UNUSED)
{
	কাঠামো commit_graph *graph = ফাঁকা;
	কাঠামো odb_files_dir *dir = ফাঁকা;
	অক্ষর *graph_name;
	অক্ষর *chain_name;
	গণনা { OPENED_NONE, OPENED_GRAPH, OPENED_CHAIN } opened = OPENED_NONE;
	পূর্ণসংখ্যা fd;
	কাঠামো stat st;
	পূর্ণসংখ্যা flags = 0;
	পূর্ণসংখ্যা incomplete_chain = 0;
	পূর্ণসংখ্যা ret;

	স্থির কাঠামো option builtin_commit_graph_verify_options[] = {
		OPT_BOOL(0, "shallow", &opts.shallow,
			 N_("if the commit-graph is split, only verify the tip file")),
		OPT_BOOL(0, "progress", &opts.progress,
			 N_("force progress reporting")),
		OPT_END(),
	};
	কাঠামো option *options = add_common_options(builtin_commit_graph_verify_options);

	trace2_cmd_mode("verify");

	opts.progress = isatty(2);
	argc = parse_options(argc, argv, prefix,
			     options,
			     builtin_commit_graph_verify_usage, 0);
	যদি (argc)
		usage_with_options(builtin_commit_graph_verify_usage, options);

	যদি (!opts.obj_dir)
		opts.obj_dir = repo_get_object_directory(the_repository);
	যদি (opts.shallow)
		flags |= COMMIT_GRAPH_VERIFY_SHALLOW;
	যদি (opts.progress)
		flags |= COMMIT_GRAPH_WRITE_PROGRESS;

	dir = odb_source_files_find_dir(the_repository->objects, opts.obj_dir);
	যদি (!dir)
		die(_("could not find object directory matching %s"), opts.obj_dir);

	graph_name = get_commit_graph_filename(dir->abspath);
	chain_name = get_commit_graph_chain_filename(dir->abspath);
	যদি (open_commit_graph(graph_name, &fd, &st))
		opened = OPENED_GRAPH;
	অন্যথা যদি (errno != ENOENT)
		die_errno(_("Could not open commit-graph '%s'"), graph_name);
	অন্যথা যদি (open_commit_graph_chain(chain_name, &fd, &st,
					 the_repository->hash_algo))
		opened = OPENED_CHAIN;
	অন্যথা যদি (errno != ENOENT)
		die_errno(_("could not open commit-graph chain '%s'"), chain_name);

	FREE_AND_NULL(graph_name);
	FREE_AND_NULL(chain_name);
	FREE_AND_NULL(options);

	যদি (opened == OPENED_NONE)
		ফেরত 0;
	অন্যথা যদি (opened == OPENED_GRAPH)
		graph = load_commit_graph_one_fd_st(the_repository, dir->abspath, fd, &st);
	অন্যথা
		graph = load_commit_graph_chain_fd_st(the_repository->objects, fd, &st,
						      &incomplete_chain);

	যদি (!graph)
		ফেরত 1;

	ret = verify_commit_graph(graph, flags);
	free_commit_graph(graph);

	যদি (incomplete_chain) {
		error("one or more commit-graph chain files could not be loaded");
		ret |= 1;
	}

	ফেরত ret;
}

বাহ্যিক পূর্ণসংখ্যা read_replace_refs;
স্থির কাঠামো commit_graph_opts write_opts;

স্থির পূর্ণসংখ্যা write_option_parse_split(ধ্রুবক কাঠামো option *opt, ধ্রুবক অক্ষর *arg,
				    পূর্ণসংখ্যা unset)
{
	গণনা commit_graph_split_flags *flags = opt->মান;

	BUG_ON_OPT_NEG(unset);

	opts.split = 1;
	যদি (!arg)
		ফেরত 0;

	যদি (!strcmp(arg, "no-merge"))
		*flags = COMMIT_GRAPH_SPLIT_MERGE_PROHIBITED;
	অন্যথা যদি (!strcmp(arg, "replace"))
		*flags = COMMIT_GRAPH_SPLIT_REPLACE;
	অন্যথা
		die(_("unrecognized --split argument, %s"), arg);

	ফেরত 0;
}

স্থির পূর্ণসংখ্যা read_one_commit(কাঠামো oidset *commits, কাঠামো progress *progress,
			   ধ্রুবক অক্ষর *hash)
{
	কাঠামো object *ফলাফল;
	কাঠামো object_id oid;
	ধ্রুবক অক্ষর *end;

	যদি (parse_oid_hex(hash, &oid, &end))
		ফেরত error(_("unexpected non-hex object ID: %s"), hash);

	ফলাফল = deref_tag(the_repository, parse_object(the_repository, &oid),
			   ফাঁকা, 0);
	যদি (!ফলাফল)
		ফেরত error(_("invalid object: %s"), hash);
	অন্যথা যদি (object_as_type(ফলাফল, OBJ_COMMIT, 1))
		oidset_ঢোকাও(commits, &ফলাফল->oid);

	display_progress(progress, oidset_আকার(commits));

	ফেরত 0;
}

স্থির পূর্ণসংখ্যা write_option_max_new_filters(ধ্রুবক কাঠামো option *opt,
					ধ্রুবক অক্ষর *arg,
					পূর্ণসংখ্যা unset)
{
	পূর্ণসংখ্যা *to = opt->মান;
	যদি (unset)
		*to = -1;
	অন্যথা {
		ধ্রুবক অক্ষর *s;
		*to = strtol(arg, (অক্ষর **)&s, 10);
		যদি (*s)
			ফেরত error(_("option `%s' expects a numerical value"),
				     "max-new-filters");
	}
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা git_commit_graph_write_config(ধ্রুবক অক্ষর *var, ধ্রুবক অক্ষর *মান,
					 ধ্রুবক কাঠামো config_context *ctx,
					 শূন্য *cb UNUSED)
{
	যদি (!strcmp(var, "commitgraph.maxnewfilters"))
		write_opts.max_new_filters = git_config_int(var, মান, ctx->kvi);
	অন্যথা যদি (!strcmp(var, "commitgraph.changedpaths"))
		opts.enable_changed_paths = git_config_bool(var, মান) ? 1 : -1;
	/*
	 * No need to fall-back to 'git_default_config', since this was already
	 * called in 'cmd_commit_graph()'.
	 */
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা graph_write(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix,
		       কাঠামো repository *repo UNUSED)
{
	কাঠামো string_list pack_indexes = STRING_LIST_INIT_DUP;
	কাঠামো strbuf buf = STRBUF_INIT;
	কাঠামো oidset commits = OIDSET_INIT;
	কাঠামো odb_files_dir *dir = ফাঁকা;
	পূর্ণসংখ্যা ফলাফল = 0;
	গণনা commit_graph_write_flags flags = 0;
	কাঠামো progress *progress = ফাঁকা;

	স্থির কাঠামো option builtin_commit_graph_write_options[] = {
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
		OPT_CALLBACK_F(0, "split", &write_opts.split_flags, ফাঁকা,
			N_("allow writing an incremental commit-graph file"),
			PARSE_OPT_OPTARG | PARSE_OPT_NONEG,
			write_option_parse_split),
		OPT_INTEGER(0, "max-commits", &write_opts.max_commits,
			N_("maximum number of commits in a non-base split commit-graph")),
		OPT_INTEGER(0, "size-multiple", &write_opts.আকার_multiple,
			N_("maximum ratio between two levels of a split commit-graph")),
		OPT_EXPIRY_DATE(0, "expire-time", &write_opts.expire_time,
			N_("only expire files older than a given date-time")),
		OPT_CALLBACK_F(0, "max-new-filters", &write_opts.max_new_filters,
			ফাঁকা, N_("maximum number of changed-path Bloom filters to compute"),
			0, write_option_max_new_filters),
		OPT_BOOL(0, "progress", &opts.progress,
			 N_("force progress reporting")),
		OPT_END(),
	};
	কাঠামো option *options = add_common_options(builtin_commit_graph_write_options);

	opts.progress = isatty(2);
	opts.enable_changed_paths = -1;
	write_opts.আকার_multiple = 2;
	write_opts.max_commits = 0;
	write_opts.expire_time = 0;
	write_opts.max_new_filters = -1;

	trace2_cmd_mode("write");

	repo_config(the_repository, git_commit_graph_write_config, &opts);

	argc = parse_options(argc, argv, prefix,
			     options,
			     builtin_commit_graph_write_usage, 0);
	যদি (argc)
		usage_with_options(builtin_commit_graph_write_usage, options);

	যদি (opts.reachable + opts.stdin_packs + opts.stdin_commits > 1)
		die(_("use at most one of --reachable, --stdin-commits, or --stdin-packs"));
	যদি (!opts.obj_dir)
		opts.obj_dir = repo_get_object_directory(the_repository);
	যদি (opts.append)
		flags |= COMMIT_GRAPH_WRITE_APPEND;
	যদি (opts.split)
		flags |= COMMIT_GRAPH_WRITE_SPLIT;
	যদি (opts.progress)
		flags |= COMMIT_GRAPH_WRITE_PROGRESS;
	যদি (!opts.enable_changed_paths)
		flags |= COMMIT_GRAPH_NO_WRITE_BLOOM_FILTERS;
	যদি (opts.enable_changed_paths == 1 ||
	    git_env_bool(GIT_TEST_COMMIT_GRAPH_CHANGED_PATHS, 0))
		flags |= COMMIT_GRAPH_WRITE_BLOOM_FILTERS;

	dir = odb_source_files_find_dir(the_repository->objects, opts.obj_dir);
	যদি (!dir)
		die(_("could not find object directory matching %s"), opts.obj_dir);

	যদি (opts.reachable) {
		যদি (write_commit_graph_reachable(the_repository, dir->abspath, flags, &write_opts))
			ফলাফল = 1;
		যাও cleanup;
	}

	যদি (opts.stdin_packs) {
		কাঠামো strbuf packname = STRBUF_INIT;
		আকার_t dirlen;

		strbuf_addf(&packname, "%s/pack/", dir->abspath);
		dirlen = packname.len;

		যখন (strbuf_getline(&buf, stdin) != EOF) {
			strbuf_setlen(&packname, dirlen);
			strbuf_addbuf(&packname, &buf);
			string_list_append(&pack_indexes, packname.buf);
		}

		strbuf_release(&packname);
	} অন্যথা যদি (opts.stdin_commits) {
		oidset_init(&commits, 0);
		যদি (opts.progress)
			progress = start_delayed_progress(
				the_repository,
				_("Collecting commits from input"), 0);

		যখন (strbuf_getline(&buf, stdin) != EOF) {
			যদি (read_one_commit(&commits, progress, buf.buf)) {
				ফলাফল = 1;
				stop_progress(&progress);
				যাও cleanup;
			}
		}

		stop_progress(&progress);
	}

	যদি (write_commit_graph(the_repository, dir->abspath,
			       opts.stdin_packs ? &pack_indexes : ফাঁকা,
			       opts.stdin_commits ? &commits : ফাঁকা,
			       flags,
			       &write_opts))
		ফলাফল = 1;

cleanup:
	FREE_AND_NULL(options);
	string_list_clear(&pack_indexes, 0);
	strbuf_release(&buf);
	oidset_clear(&commits);
	ফেরত ফলাফল;
}

পূর্ণসংখ্যা cmd_commit_graph(পূর্ণসংখ্যা argc,
		     ধ্রুবক অক্ষর **argv,
		     ধ্রুবক অক্ষর *prefix,
		     কাঠামো repository *repo)
{
	parse_opt_subcommand_fn *fn = ফাঁকা;
	কাঠামো option builtin_commit_graph_options[] = {
		OPT_SUBCOMMAND("verify", &fn, graph_verify),
		OPT_SUBCOMMAND("write", &fn, graph_write),
		OPT_END(),
	};
	কাঠামো option *options = parse_options_concat(builtin_commit_graph_options, common_opts);

	repo_config(the_repository, git_default_config, ফাঁকা);

	disable_replace_refs();
	save_commit_বাফার = 0;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_commit_graph_usage, 0);
	FREE_AND_NULL(options);

	ফেরত fn(argc, argv, prefix, repo);
}
