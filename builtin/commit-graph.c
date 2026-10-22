#define USE_THE_REPOSITORY_VARIABLE
#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "commit.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "hex.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "commit-graph.h"
#અન્તર્ભાવ "odb.h"
#અન્તર્ભાવ "odb/source.h"
#અન્તર્ભાવ "odb/source-files.h"
#અન્તર્ભાવ "progress.h"
#અન્તર્ભાવ "replace-object.h"
#અન્તર્ભાવ "strbuf.h"
#અન્તર્ભાવ "tag.h"
#અન્તર્ભાવ "trace2.h"

#define BUILTIN_COMMIT_GRAPH_VERIFY_USAGE \
	N_("git commit-graph verify [--object-dir <dir>] [--shallow] [--[no-]progress]")

#define BUILTIN_COMMIT_GRAPH_WRITE_USAGE \
	N_("git commit-graph write [--object-dir <dir>] [--append]\n" \
	   "                       [--split[=<strategy>]] [--reachable | --stdin-packs | --stdin-commits]\n" \
	   "                       [--changed-paths] [--[no-]max-new-filters <n>] [--[no-]progress]\n" \
	   "                       <split-options>")

સ્થિર અચલ અક્ષર * અચલ builtin_commit_graph_verify_usage[] = {
	BUILTIN_COMMIT_GRAPH_VERIFY_USAGE,
	ખાલી
};

સ્થિર અચલ અક્ષર * અચલ builtin_commit_graph_write_usage[] = {
	BUILTIN_COMMIT_GRAPH_WRITE_USAGE,
	ખાલી
};

સ્થિર અક્ષર અચલ * અચલ builtin_commit_graph_usage[] = {
	BUILTIN_COMMIT_GRAPH_VERIFY_USAGE,
	BUILTIN_COMMIT_GRAPH_WRITE_USAGE,
	ખાલી,
};

સ્થિર સંરચના opts_commit_graph {
	અચલ અક્ષર *obj_dir;
	પૂર્ણાંક reachable;
	પૂર્ણાંક stdin_packs;
	પૂર્ણાંક stdin_commits;
	પૂર્ણાંક append;
	પૂર્ણાંક split;
	પૂર્ણાંક shallow;
	પૂર્ણાંક progress;
	પૂર્ણાંક enable_changed_paths;
} opts;

સ્થિર સંરચના option common_opts[] = {
	OPT_STRING(0, "object-dir", &opts.obj_dir,
		   N_("dir"),
		   N_("the object directory to store the graph")),
	OPT_END()
};

સ્થિર સંરચના option *add_common_options(સંરચના option *to)
{
	પાછા parse_options_concat(common_opts, to);
}

સ્થિર પૂર્ણાંક graph_verify(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
			સંરચના repository *repo UNUSED)
{
	સંરચના commit_graph *graph = ખાલી;
	સંરચના odb_files_dir *dir = ખાલી;
	અક્ષર *graph_name;
	અક્ષર *chain_name;
	ગણના { OPENED_NONE, OPENED_GRAPH, OPENED_CHAIN } opened = OPENED_NONE;
	પૂર્ણાંક fd;
	સંરચના stat st;
	પૂર્ણાંક flags = 0;
	પૂર્ણાંક incomplete_chain = 0;
	પૂર્ણાંક ret;

	સ્થિર સંરચના option builtin_commit_graph_verify_options[] = {
		OPT_BOOL(0, "shallow", &opts.shallow,
			 N_("if the commit-graph is split, only verify the tip file")),
		OPT_BOOL(0, "progress", &opts.progress,
			 N_("force progress reporting")),
		OPT_END(),
	};
	સંરચના option *options = add_common_options(builtin_commit_graph_verify_options);

	trace2_cmd_mode("verify");

	opts.progress = isatty(2);
	argc = parse_options(argc, argv, prefix,
			     options,
			     builtin_commit_graph_verify_usage, 0);
	જો (argc)
		usage_with_options(builtin_commit_graph_verify_usage, options);

	જો (!opts.obj_dir)
		opts.obj_dir = repo_get_object_directory(the_repository);
	જો (opts.shallow)
		flags |= COMMIT_GRAPH_VERIFY_SHALLOW;
	જો (opts.progress)
		flags |= COMMIT_GRAPH_WRITE_PROGRESS;

	dir = odb_source_files_find_dir(the_repository->objects, opts.obj_dir);
	જો (!dir)
		die(_("could not find object directory matching %s"), opts.obj_dir);

	graph_name = get_commit_graph_filename(dir->abspath);
	chain_name = get_commit_graph_chain_filename(dir->abspath);
	જો (open_commit_graph(graph_name, &fd, &st))
		opened = OPENED_GRAPH;
	નહીંતર જો (errno != ENOENT)
		die_errno(_("Could not open commit-graph '%s'"), graph_name);
	નહીંતર જો (open_commit_graph_chain(chain_name, &fd, &st,
					 the_repository->hash_algo))
		opened = OPENED_CHAIN;
	નહીંતર જો (errno != ENOENT)
		die_errno(_("could not open commit-graph chain '%s'"), chain_name);

	FREE_AND_NULL(graph_name);
	FREE_AND_NULL(chain_name);
	FREE_AND_NULL(options);

	જો (opened == OPENED_NONE)
		પાછા 0;
	નહીંતર જો (opened == OPENED_GRAPH)
		graph = load_commit_graph_one_fd_st(the_repository, dir->abspath, fd, &st);
	નહીંતર
		graph = load_commit_graph_chain_fd_st(the_repository->objects, fd, &st,
						      &incomplete_chain);

	જો (!graph)
		પાછા 1;

	ret = verify_commit_graph(graph, flags);
	free_commit_graph(graph);

	જો (incomplete_chain) {
		error("one or more commit-graph chain files could not be loaded");
		ret |= 1;
	}

	પાછા ret;
}

બાહ્ય પૂર્ણાંક read_replace_refs;
સ્થિર સંરચના commit_graph_opts write_opts;

સ્થિર પૂર્ણાંક write_option_parse_split(અચલ સંરચના option *opt, અચલ અક્ષર *arg,
				    પૂર્ણાંક unset)
{
	ગણના commit_graph_split_flags *flags = opt->કિંમત;

	BUG_ON_OPT_NEG(unset);

	opts.split = 1;
	જો (!arg)
		પાછા 0;

	જો (!strcmp(arg, "no-merge"))
		*flags = COMMIT_GRAPH_SPLIT_MERGE_PROHIBITED;
	નહીંતર જો (!strcmp(arg, "replace"))
		*flags = COMMIT_GRAPH_SPLIT_REPLACE;
	નહીંતર
		die(_("unrecognized --split argument, %s"), arg);

	પાછા 0;
}

સ્થિર પૂર્ણાંક read_one_commit(સંરચના oidset *commits, સંરચના progress *progress,
			   અચલ અક્ષર *hash)
{
	સંરચના object *result;
	સંરચના object_id oid;
	અચલ અક્ષર *end;

	જો (parse_oid_hex(hash, &oid, &end))
		પાછા error(_("unexpected non-hex object ID: %s"), hash);

	result = deref_tag(the_repository, parse_object(the_repository, &oid),
			   ખાલી, 0);
	જો (!result)
		પાછા error(_("invalid object: %s"), hash);
	નહીંતર જો (object_as_type(result, OBJ_COMMIT, 1))
		oidset_ઉમેરો(commits, &result->oid);

	display_progress(progress, oidset_size(commits));

	પાછા 0;
}

સ્થિર પૂર્ણાંક write_option_max_new_filters(અચલ સંરચના option *opt,
					અચલ અક્ષર *arg,
					પૂર્ણાંક unset)
{
	પૂર્ણાંક *to = opt->કિંમત;
	જો (unset)
		*to = -1;
	નહીંતર {
		અચલ અક્ષર *s;
		*to = strtol(arg, (અક્ષર **)&s, 10);
		જો (*s)
			પાછા error(_("option `%s' expects a numerical value"),
				     "max-new-filters");
	}
	પાછા 0;
}

સ્થિર પૂર્ણાંક git_commit_graph_write_config(અચલ અક્ષર *var, અચલ અક્ષર *કિંમત,
					 અચલ સંરચના config_context *ctx,
					 શૂન્ય *cb UNUSED)
{
	જો (!strcmp(var, "commitgraph.maxnewfilters"))
		write_opts.max_new_filters = git_config_int(var, કિંમત, ctx->kvi);
	નહીંતર જો (!strcmp(var, "commitgraph.changedpaths"))
		opts.enable_changed_paths = git_config_bool(var, કિંમત) ? 1 : -1;
	/*
	 * No need to fall-back to 'git_default_config', since this was already
	 * called in 'cmd_commit_graph()'.
	 */
	પાછા 0;
}

સ્થિર પૂર્ણાંક graph_write(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
		       સંરચના repository *repo UNUSED)
{
	સંરચના string_list pack_indexes = STRING_LIST_INIT_DUP;
	સંરચના strbuf buf = STRBUF_INIT;
	સંરચના oidset commits = OIDSET_INIT;
	સંરચના odb_files_dir *dir = ખાલી;
	પૂર્ણાંક result = 0;
	ગણના commit_graph_write_flags flags = 0;
	સંરચના progress *progress = ખાલી;

	સ્થિર સંરચના option builtin_commit_graph_write_options[] = {
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
		OPT_CALLBACK_F(0, "split", &write_opts.split_flags, ખાલી,
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
			ખાલી, N_("maximum number of changed-path Bloom filters to compute"),
			0, write_option_max_new_filters),
		OPT_BOOL(0, "progress", &opts.progress,
			 N_("force progress reporting")),
		OPT_END(),
	};
	સંરચના option *options = add_common_options(builtin_commit_graph_write_options);

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
	જો (argc)
		usage_with_options(builtin_commit_graph_write_usage, options);

	જો (opts.reachable + opts.stdin_packs + opts.stdin_commits > 1)
		die(_("use at most one of --reachable, --stdin-commits, or --stdin-packs"));
	જો (!opts.obj_dir)
		opts.obj_dir = repo_get_object_directory(the_repository);
	જો (opts.append)
		flags |= COMMIT_GRAPH_WRITE_APPEND;
	જો (opts.split)
		flags |= COMMIT_GRAPH_WRITE_SPLIT;
	જો (opts.progress)
		flags |= COMMIT_GRAPH_WRITE_PROGRESS;
	જો (!opts.enable_changed_paths)
		flags |= COMMIT_GRAPH_NO_WRITE_BLOOM_FILTERS;
	જો (opts.enable_changed_paths == 1 ||
	    git_env_bool(GIT_TEST_COMMIT_GRAPH_CHANGED_PATHS, 0))
		flags |= COMMIT_GRAPH_WRITE_BLOOM_FILTERS;

	dir = odb_source_files_find_dir(the_repository->objects, opts.obj_dir);
	જો (!dir)
		die(_("could not find object directory matching %s"), opts.obj_dir);

	જો (opts.reachable) {
		જો (write_commit_graph_reachable(the_repository, dir->abspath, flags, &write_opts))
			result = 1;
		જાઓ cleanup;
	}

	જો (opts.stdin_packs) {
		સંરચના strbuf packname = STRBUF_INIT;
		size_t dirlen;

		strbuf_addf(&packname, "%s/pack/", dir->abspath);
		dirlen = packname.len;

		જ્યારે (strbuf_getline(&buf, stdin) != EOF) {
			strbuf_setlen(&packname, dirlen);
			strbuf_addbuf(&packname, &buf);
			string_list_append(&pack_indexes, packname.buf);
		}

		strbuf_release(&packname);
	} નહીંતર જો (opts.stdin_commits) {
		oidset_init(&commits, 0);
		જો (opts.progress)
			progress = start_delayed_progress(
				the_repository,
				_("Collecting commits from input"), 0);

		જ્યારે (strbuf_getline(&buf, stdin) != EOF) {
			જો (read_one_commit(&commits, progress, buf.buf)) {
				result = 1;
				stop_progress(&progress);
				જાઓ cleanup;
			}
		}

		stop_progress(&progress);
	}

	જો (write_commit_graph(the_repository, dir->abspath,
			       opts.stdin_packs ? &pack_indexes : ખાલી,
			       opts.stdin_commits ? &commits : ખાલી,
			       flags,
			       &write_opts))
		result = 1;

cleanup:
	FREE_AND_NULL(options);
	string_list_clear(&pack_indexes, 0);
	strbuf_release(&buf);
	oidset_clear(&commits);
	પાછા result;
}

પૂર્ણાંક cmd_commit_graph(પૂર્ણાંક argc,
		     અચલ અક્ષર **argv,
		     અચલ અક્ષર *prefix,
		     સંરચના repository *repo)
{
	parse_opt_subcommand_fn *fn = ખાલી;
	સંરચના option builtin_commit_graph_options[] = {
		OPT_SUBCOMMAND("verify", &fn, graph_verify),
		OPT_SUBCOMMAND("write", &fn, graph_write),
		OPT_END(),
	};
	સંરચના option *options = parse_options_concat(builtin_commit_graph_options, common_opts);

	repo_config(the_repository, git_default_config, ખાલી);

	disable_replace_refs();
	save_commit_buffer = 0;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_commit_graph_usage, 0);
	FREE_AND_NULL(options);

	પાછા fn(argc, argv, prefix, repo);
}
