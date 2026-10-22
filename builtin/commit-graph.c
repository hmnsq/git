#define USE_THE_REPOSITORY_VARIABLE
#include "builtin.h"
#include "commit.h"
#include "config.h"
#include "environment.h"
#include "gettext.h"
#include "hex.h"
#include "parse-options.h"
#include "commit-graph.h"
#include "odb.h"
#include "odb/source.h"
#include "odb/source-files.h"
#include "progress.h"
#include "replace-object.h"
#include "strbuf.h"
#include "tag.h"
#include "trace2.h"

#define BUILTIN_COMMIT_GRAPH_VERIFY_USAGE \
	N_("git commit-graph verify [--object-dir <dir>] [--shallow] [--[no-]progress]")

#define BUILTIN_COMMIT_GRAPH_WRITE_USAGE \
	N_("git commit-graph write [--object-dir <dir>] [--append]\n" \
	   "                       [--split[=<strategy>]] [--reachable | --stdin-packs | --stdin-commits]\n" \
	   "                       [--changed-paths] [--[no-]max-new-filters <n>] [--[no-]progress]\n" \
	   "                       <split-options>")

static const var_na * const builtin_commit_graph_verify_usage[] = {
	BUILTIN_COMMIT_GRAPH_VERIFY_USAGE,
	NULL
};

static const var_na * const builtin_commit_graph_write_usage[] = {
	BUILTIN_COMMIT_GRAPH_WRITE_USAGE,
	NULL
};

static var_na const * const builtin_commit_graph_usage[] = {
	BUILTIN_COMMIT_GRAPH_VERIFY_USAGE,
	BUILTIN_COMMIT_GRAPH_WRITE_USAGE,
	NULL,
};

static sa_nracanaa opts_commit_graph {
	const var_na *obj_dir;
	puur_naa_nka reachable;
	puur_naa_nka stdin_packs;
	puur_naa_nka stdin_commits;
	puur_naa_nka append;
	puur_naa_nka split;
	puur_naa_nka shallow;
	puur_naa_nka progress;
	puur_naa_nka enable_changed_paths;
} opts;

static sa_nracanaa option common_opts[] = {
	OPT_STRING(0, "object-dir", &opts.obj_dir,
		   N_("dir"),
		   N_("the object directory to store the graph")),
	OPT_END()
};

static sa_nracanaa option *add_common_options(sa_nracanaa option *to)
{
	ᱨᱩᱣᱟᱹᱲ parse_options_concat(common_opts, to);
}

static puur_naa_nka graph_verify(puur_naa_nka argc, const var_na **argv, const var_na *prefix,
			sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa commit_graph *graph = NULL;
	sa_nracanaa odb_files_dir *dir = NULL;
	var_na *graph_name;
	var_na *chain_name;
	ga_nanaa { OPENED_NONE, OPENED_GRAPH, OPENED_CHAIN } opened = OPENED_NONE;
	puur_naa_nka fd;
	sa_nracanaa stat st;
	puur_naa_nka flags = 0;
	puur_naa_nka incomplete_chain = 0;
	puur_naa_nka ret;

	static sa_nracanaa option builtin_commit_graph_verify_options[] = {
		OPT_BOOL(0, "shallow", &opts.shallow,
			 N_("if the commit-graph is split, only verify the tip file")),
		OPT_BOOL(0, "progress", &opts.progress,
			 N_("force progress reporting")),
		OPT_END(),
	};
	sa_nracanaa option *options = add_common_options(builtin_commit_graph_verify_options);

	trace2_cmd_mode("verify");

	opts.progress = isatty(2);
	argc = parse_options(argc, argv, prefix,
			     options,
			     builtin_commit_graph_verify_usage, 0);
	ᱡᱩᱫᱤ (argc)
		usage_with_options(builtin_commit_graph_verify_usage, options);

	ᱡᱩᱫᱤ (!opts.obj_dir)
		opts.obj_dir = repo_get_object_directory(the_repository);
	ᱡᱩᱫᱤ (opts.shallow)
		flags |= COMMIT_GRAPH_VERIFY_SHALLOW;
	ᱡᱩᱫᱤ (opts.progress)
		flags |= COMMIT_GRAPH_WRITE_PROGRESS;

	dir = odb_source_files_find_dir(the_repository->objects, opts.obj_dir);
	ᱡᱩᱫᱤ (!dir)
		die(_("could not find object directory matching %s"), opts.obj_dir);

	graph_name = get_commit_graph_filename(dir->abspath);
	chain_name = get_commit_graph_chain_filename(dir->abspath);
	ᱡᱩᱫᱤ (open_commit_graph(graph_name, &fd, &st))
		opened = OPENED_GRAPH;
	ᱵᱟᱝ ᱡᱩᱫᱤ (errno != ENOENT)
		die_errno(_("Could not open commit-graph '%s'"), graph_name);
	ᱵᱟᱝ ᱡᱩᱫᱤ (open_commit_graph_chain(chain_name, &fd, &st,
					 the_repository->hash_algo))
		opened = OPENED_CHAIN;
	ᱵᱟᱝ ᱡᱩᱫᱤ (errno != ENOENT)
		die_errno(_("could not open commit-graph chain '%s'"), chain_name);

	FREE_AND_NULL(graph_name);
	FREE_AND_NULL(chain_name);
	FREE_AND_NULL(options);

	ᱡᱩᱫᱤ (opened == OPENED_NONE)
		ᱨᱩᱣᱟᱹᱲ 0;
	ᱵᱟᱝ ᱡᱩᱫᱤ (opened == OPENED_GRAPH)
		graph = load_commit_graph_one_fd_st(the_repository, dir->abspath, fd, &st);
	ᱵᱟᱝ
		graph = load_commit_graph_chain_fd_st(the_repository->objects, fd, &st,
						      &incomplete_chain);

	ᱡᱩᱫᱤ (!graph)
		ᱨᱩᱣᱟᱹᱲ 1;

	ret = verify_commit_graph(graph, flags);
	free_commit_graph(graph);

	ᱡᱩᱫᱤ (incomplete_chain) {
		error("one or more commit-graph chain files could not be loaded");
		ret |= 1;
	}

	ᱨᱩᱣᱟᱹᱲ ret;
}

extern puur_naa_nka read_replace_refs;
static sa_nracanaa commit_graph_opts write_opts;

static puur_naa_nka write_option_parse_split(const sa_nracanaa option *opt, const var_na *arg,
				    puur_naa_nka unset)
{
	ga_nanaa commit_graph_split_flags *flags = opt->value;

	BUG_ON_OPT_NEG(unset);

	opts.split = 1;
	ᱡᱩᱫᱤ (!arg)
		ᱨᱩᱣᱟᱹᱲ 0;

	ᱡᱩᱫᱤ (!strcmp(arg, "no-merge"))
		*flags = COMMIT_GRAPH_SPLIT_MERGE_PROHIBITED;
	ᱵᱟᱝ ᱡᱩᱫᱤ (!strcmp(arg, "replace"))
		*flags = COMMIT_GRAPH_SPLIT_REPLACE;
	ᱵᱟᱝ
		die(_("unrecognized --split argument, %s"), arg);

	ᱨᱩᱣᱟᱹᱲ 0;
}

static puur_naa_nka read_one_commit(sa_nracanaa oidset *commits, sa_nracanaa progress *progress,
			   const var_na *hash)
{
	sa_nracanaa object *result;
	sa_nracanaa object_id oid;
	const var_na *end;

	ᱡᱩᱫᱤ (parse_oid_hex(hash, &oid, &end))
		ᱨᱩᱣᱟᱹᱲ error(_("unexpected non-hex object ID: %s"), hash);

	result = deref_tag(the_repository, parse_object(the_repository, &oid),
			   NULL, 0);
	ᱡᱩᱫᱤ (!result)
		ᱨᱩᱣᱟᱹᱲ error(_("invalid object: %s"), hash);
	ᱵᱟᱝ ᱡᱩᱫᱤ (object_as_type(result, OBJ_COMMIT, 1))
		oidset_insert(commits, &result->oid);

	display_progress(progress, oidset_size(commits));

	ᱨᱩᱣᱟᱹᱲ 0;
}

static puur_naa_nka write_option_max_new_filters(const sa_nracanaa option *opt,
					const var_na *arg,
					puur_naa_nka unset)
{
	puur_naa_nka *to = opt->value;
	ᱡᱩᱫᱤ (unset)
		*to = -1;
	ᱵᱟᱝ {
		const var_na *s;
		*to = strtol(arg, (var_na **)&s, 10);
		ᱡᱩᱫᱤ (*s)
			ᱨᱩᱣᱟᱹᱲ error(_("option `%s' expects a numerical value"),
				     "max-new-filters");
	}
	ᱨᱩᱣᱟᱹᱲ 0;
}

static puur_naa_nka git_commit_graph_write_config(const var_na *var, const var_na *value,
					 const sa_nracanaa config_context *ctx,
					 void *cb UNUSED)
{
	ᱡᱩᱫᱤ (!strcmp(var, "commitgraph.maxnewfilters"))
		write_opts.max_new_filters = git_config_int(var, value, ctx->kvi);
	ᱵᱟᱝ ᱡᱩᱫᱤ (!strcmp(var, "commitgraph.changedpaths"))
		opts.enable_changed_paths = git_config_bool(var, value) ? 1 : -1;
	/*
	 * No need to fall-back to 'git_default_config', since this was already
	 * called in 'cmd_commit_graph()'.
	 */
	ᱨᱩᱣᱟᱹᱲ 0;
}

static puur_naa_nka graph_write(puur_naa_nka argc, const var_na **argv, const var_na *prefix,
		       sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa string_list pack_indexes = STRING_LIST_INIT_DUP;
	sa_nracanaa strbuf buf = STRBUF_INIT;
	sa_nracanaa oidset commits = OIDSET_INIT;
	sa_nracanaa odb_files_dir *dir = NULL;
	puur_naa_nka result = 0;
	ga_nanaa commit_graph_write_flags flags = 0;
	sa_nracanaa progress *progress = NULL;

	static sa_nracanaa option builtin_commit_graph_write_options[] = {
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
		OPT_CALLBACK_F(0, "split", &write_opts.split_flags, NULL,
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
			NULL, N_("maximum number of changed-path Bloom filters to compute"),
			0, write_option_max_new_filters),
		OPT_BOOL(0, "progress", &opts.progress,
			 N_("force progress reporting")),
		OPT_END(),
	};
	sa_nracanaa option *options = add_common_options(builtin_commit_graph_write_options);

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
	ᱡᱩᱫᱤ (argc)
		usage_with_options(builtin_commit_graph_write_usage, options);

	ᱡᱩᱫᱤ (opts.reachable + opts.stdin_packs + opts.stdin_commits > 1)
		die(_("use at most one of --reachable, --stdin-commits, or --stdin-packs"));
	ᱡᱩᱫᱤ (!opts.obj_dir)
		opts.obj_dir = repo_get_object_directory(the_repository);
	ᱡᱩᱫᱤ (opts.append)
		flags |= COMMIT_GRAPH_WRITE_APPEND;
	ᱡᱩᱫᱤ (opts.split)
		flags |= COMMIT_GRAPH_WRITE_SPLIT;
	ᱡᱩᱫᱤ (opts.progress)
		flags |= COMMIT_GRAPH_WRITE_PROGRESS;
	ᱡᱩᱫᱤ (!opts.enable_changed_paths)
		flags |= COMMIT_GRAPH_NO_WRITE_BLOOM_FILTERS;
	ᱡᱩᱫᱤ (opts.enable_changed_paths == 1 ||
	    git_env_bool(GIT_TEST_COMMIT_GRAPH_CHANGED_PATHS, 0))
		flags |= COMMIT_GRAPH_WRITE_BLOOM_FILTERS;

	dir = odb_source_files_find_dir(the_repository->objects, opts.obj_dir);
	ᱡᱩᱫᱤ (!dir)
		die(_("could not find object directory matching %s"), opts.obj_dir);

	ᱡᱩᱫᱤ (opts.reachable) {
		ᱡᱩᱫᱤ (write_commit_graph_reachable(the_repository, dir->abspath, flags, &write_opts))
			result = 1;
		goto cleanup;
	}

	ᱡᱩᱫᱤ (opts.stdin_packs) {
		sa_nracanaa strbuf packname = STRBUF_INIT;
		size_t dirlen;

		strbuf_addf(&packname, "%s/pack/", dir->abspath);
		dirlen = packname.len;

		ᱡᱚᱠᱷᱚᱱ (strbuf_getline(&buf, stdin) != EOF) {
			strbuf_setlen(&packname, dirlen);
			strbuf_addbuf(&packname, &buf);
			string_list_append(&pack_indexes, packname.buf);
		}

		strbuf_release(&packname);
	} ᱵᱟᱝ ᱡᱩᱫᱤ (opts.stdin_commits) {
		oidset_init(&commits, 0);
		ᱡᱩᱫᱤ (opts.progress)
			progress = start_delayed_progress(
				the_repository,
				_("Collecting commits from input"), 0);

		ᱡᱚᱠᱷᱚᱱ (strbuf_getline(&buf, stdin) != EOF) {
			ᱡᱩᱫᱤ (read_one_commit(&commits, progress, buf.buf)) {
				result = 1;
				stop_progress(&progress);
				goto cleanup;
			}
		}

		stop_progress(&progress);
	}

	ᱡᱩᱫᱤ (write_commit_graph(the_repository, dir->abspath,
			       opts.stdin_packs ? &pack_indexes : NULL,
			       opts.stdin_commits ? &commits : NULL,
			       flags,
			       &write_opts))
		result = 1;

cleanup:
	FREE_AND_NULL(options);
	string_list_clear(&pack_indexes, 0);
	strbuf_release(&buf);
	oidset_clear(&commits);
	ᱨᱩᱣᱟᱹᱲ result;
}

puur_naa_nka cmd_commit_graph(puur_naa_nka argc,
		     const var_na **argv,
		     const var_na *prefix,
		     sa_nracanaa repository *repo)
{
	parse_opt_subcommand_fn *fn = NULL;
	sa_nracanaa option builtin_commit_graph_options[] = {
		OPT_SUBCOMMAND("verify", &fn, graph_verify),
		OPT_SUBCOMMAND("write", &fn, graph_write),
		OPT_END(),
	};
	sa_nracanaa option *options = parse_options_concat(builtin_commit_graph_options, common_opts);

	repo_config(the_repository, git_default_config, NULL);

	disable_replace_refs();
	save_commit_buffer = 0;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_commit_graph_usage, 0);
	FREE_AND_NULL(options);

	ᱨᱩᱣᱟᱹᱲ fn(argc, argv, prefix, repo);
}
