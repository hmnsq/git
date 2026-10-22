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

static const char * const builtin_commit_graph_verify_usage[] = {
	BUILTIN_COMMIT_GRAPH_VERIFY_USAGE,
	NULL
};

static const char * const builtin_commit_graph_write_usage[] = {
	BUILTIN_COMMIT_GRAPH_WRITE_USAGE,
	NULL
};

static char const * const builtin_commit_graph_usage[] = {
	BUILTIN_COMMIT_GRAPH_VERIFY_USAGE,
	BUILTIN_COMMIT_GRAPH_WRITE_USAGE,
	NULL,
};

static struct opts_commit_graph {
	const char *obj_dir;
	цео reachable;
	цео stdin_packs;
	цео stdin_commits;
	цео append;
	цео split;
	цео shallow;
	цео progress;
	цео enable_changed_paths;
} opts;

static struct option common_opts[] = {
	OPT_STRING(0, "object-dir", &opts.obj_dir,
		   N_("dir"),
		   N_("the object directory to store the graph")),
	OPT_END()
};

static struct option *add_common_options(struct option *to)
{
	врати parse_options_concat(common_opts, to);
}

static цео graph_verify(цео argc, const char **argv, const char *prefix,
			struct repository *repo UNUSED)
{
	struct commit_graph *graph = NULL;
	struct odb_files_dir *dir = NULL;
	char *graph_name;
	char *chain_name;
	enum { OPENED_NONE, OPENED_GRAPH, OPENED_CHAIN } opened = OPENED_NONE;
	цео fd;
	struct stat st;
	цео flags = 0;
	цео incomplete_chain = 0;
	цео ret;

	static struct option builtin_commit_graph_verify_options[] = {
		OPT_BOOL(0, "shallow", &opts.shallow,
			 N_("if the commit-graph is split, only verify the tip file")),
		OPT_BOOL(0, "progress", &opts.progress,
			 N_("force progress reporting")),
		OPT_END(),
	};
	struct option *options = add_common_options(builtin_commit_graph_verify_options);

	trace2_cmd_mode("verify");

	opts.progress = isatty(2);
	argc = parse_options(argc, argv, prefix,
			     options,
			     builtin_commit_graph_verify_usage, 0);
	ако (argc)
		usage_with_options(builtin_commit_graph_verify_usage, options);

	ако (!opts.obj_dir)
		opts.obj_dir = repo_get_object_directory(the_repository);
	ако (opts.shallow)
		flags |= COMMIT_GRAPH_VERIFY_SHALLOW;
	ако (opts.progress)
		flags |= COMMIT_GRAPH_WRITE_PROGRESS;

	dir = odb_source_files_find_dir(the_repository->objects, opts.obj_dir);
	ако (!dir)
		die(_("could not find object directory matching %s"), opts.obj_dir);

	graph_name = get_commit_graph_filename(dir->abspath);
	chain_name = get_commit_graph_chain_filename(dir->abspath);
	ако (open_commit_graph(graph_name, &fd, &st))
		opened = OPENED_GRAPH;
	инако ако (errno != ENOENT)
		die_errno(_("Could not open commit-graph '%s'"), graph_name);
	инако ако (open_commit_graph_chain(chain_name, &fd, &st,
					 the_repository->hash_algo))
		opened = OPENED_CHAIN;
	инако ако (errno != ENOENT)
		die_errno(_("could not open commit-graph chain '%s'"), chain_name);

	FREE_AND_NULL(graph_name);
	FREE_AND_NULL(chain_name);
	FREE_AND_NULL(options);

	ако (opened == OPENED_NONE)
		врати 0;
	инако ако (opened == OPENED_GRAPH)
		graph = load_commit_graph_one_fd_st(the_repository, dir->abspath, fd, &st);
	инако
		graph = load_commit_graph_chain_fd_st(the_repository->objects, fd, &st,
						      &incomplete_chain);

	ако (!graph)
		врати 1;

	ret = verify_commit_graph(graph, flags);
	free_commit_graph(graph);

	ако (incomplete_chain) {
		error("one or more commit-graph chain files could not be loaded");
		ret |= 1;
	}

	врати ret;
}

extern цео read_replace_refs;
static struct commit_graph_opts write_opts;

static цео write_option_parse_split(const struct option *opt, const char *arg,
				    цео unset)
{
	enum commit_graph_split_flags *flags = opt->value;

	BUG_ON_OPT_NEG(unset);

	opts.split = 1;
	ако (!arg)
		врати 0;

	ако (!strcmp(arg, "no-merge"))
		*flags = COMMIT_GRAPH_SPLIT_MERGE_PROHIBITED;
	инако ако (!strcmp(arg, "replace"))
		*flags = COMMIT_GRAPH_SPLIT_REPLACE;
	инако
		die(_("unrecognized --split argument, %s"), arg);

	врати 0;
}

static цео read_one_commit(struct oidset *commits, struct progress *progress,
			   const char *hash)
{
	struct object *result;
	struct object_id oid;
	const char *end;

	ако (parse_oid_hex(hash, &oid, &end))
		врати error(_("unexpected non-hex object ID: %s"), hash);

	result = deref_tag(the_repository, parse_object(the_repository, &oid),
			   NULL, 0);
	ако (!result)
		врати error(_("invalid object: %s"), hash);
	инако ако (object_as_type(result, OBJ_COMMIT, 1))
		oidset_insert(commits, &result->oid);

	display_progress(progress, oidset_size(commits));

	врати 0;
}

static цео write_option_max_new_filters(const struct option *opt,
					const char *arg,
					цео unset)
{
	цео *to = opt->value;
	ако (unset)
		*to = -1;
	инако {
		const char *s;
		*to = strtol(arg, (char **)&s, 10);
		ако (*s)
			врати error(_("option `%s' expects a numerical value"),
				     "max-new-filters");
	}
	врати 0;
}

static цео git_commit_graph_write_config(const char *var, const char *value,
					 const struct config_context *ctx,
					 void *cb UNUSED)
{
	ако (!strcmp(var, "commitgraph.maxnewfilters"))
		write_opts.max_new_filters = git_config_int(var, value, ctx->kvi);
	инако ако (!strcmp(var, "commitgraph.changedpaths"))
		opts.enable_changed_paths = git_config_bool(var, value) ? 1 : -1;
	/*
	 * No need to fall-back to 'git_default_config', since this was already
	 * called in 'cmd_commit_graph()'.
	 */
	врати 0;
}

static цео graph_write(цео argc, const char **argv, const char *prefix,
		       struct repository *repo UNUSED)
{
	struct string_list pack_indexes = STRING_LIST_INIT_DUP;
	struct strbuf buf = STRBUF_INIT;
	struct oidset commits = OIDSET_INIT;
	struct odb_files_dir *dir = NULL;
	цео result = 0;
	enum commit_graph_write_flags flags = 0;
	struct progress *progress = NULL;

	static struct option builtin_commit_graph_write_options[] = {
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
	struct option *options = add_common_options(builtin_commit_graph_write_options);

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
	ако (argc)
		usage_with_options(builtin_commit_graph_write_usage, options);

	ако (opts.reachable + opts.stdin_packs + opts.stdin_commits > 1)
		die(_("use at most one of --reachable, --stdin-commits, or --stdin-packs"));
	ако (!opts.obj_dir)
		opts.obj_dir = repo_get_object_directory(the_repository);
	ако (opts.append)
		flags |= COMMIT_GRAPH_WRITE_APPEND;
	ако (opts.split)
		flags |= COMMIT_GRAPH_WRITE_SPLIT;
	ако (opts.progress)
		flags |= COMMIT_GRAPH_WRITE_PROGRESS;
	ако (!opts.enable_changed_paths)
		flags |= COMMIT_GRAPH_NO_WRITE_BLOOM_FILTERS;
	ако (opts.enable_changed_paths == 1 ||
	    git_env_bool(GIT_TEST_COMMIT_GRAPH_CHANGED_PATHS, 0))
		flags |= COMMIT_GRAPH_WRITE_BLOOM_FILTERS;

	dir = odb_source_files_find_dir(the_repository->objects, opts.obj_dir);
	ако (!dir)
		die(_("could not find object directory matching %s"), opts.obj_dir);

	ако (opts.reachable) {
		ако (write_commit_graph_reachable(the_repository, dir->abspath, flags, &write_opts))
			result = 1;
		goto cleanup;
	}

	ако (opts.stdin_packs) {
		struct strbuf packname = STRBUF_INIT;
		size_t dirlen;

		strbuf_addf(&packname, "%s/pack/", dir->abspath);
		dirlen = packname.len;

		док (strbuf_getline(&buf, stdin) != EOF) {
			strbuf_setlen(&packname, dirlen);
			strbuf_addbuf(&packname, &buf);
			string_list_append(&pack_indexes, packname.buf);
		}

		strbuf_release(&packname);
	} инако ако (opts.stdin_commits) {
		oidset_init(&commits, 0);
		ако (opts.progress)
			progress = start_delayed_progress(
				the_repository,
				_("Collecting commits from input"), 0);

		док (strbuf_getline(&buf, stdin) != EOF) {
			ако (read_one_commit(&commits, progress, buf.buf)) {
				result = 1;
				stop_progress(&progress);
				goto cleanup;
			}
		}

		stop_progress(&progress);
	}

	ако (write_commit_graph(the_repository, dir->abspath,
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
	врати result;
}

цео cmd_commit_graph(цео argc,
		     const char **argv,
		     const char *prefix,
		     struct repository *repo)
{
	parse_opt_subcommand_fn *fn = NULL;
	struct option builtin_commit_graph_options[] = {
		OPT_SUBCOMMAND("verify", &fn, graph_verify),
		OPT_SUBCOMMAND("write", &fn, graph_write),
		OPT_END(),
	};
	struct option *options = parse_options_concat(builtin_commit_graph_options, common_opts);

	repo_config(the_repository, git_default_config, NULL);

	disable_replace_refs();
	save_commit_buffer = 0;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_commit_graph_usage, 0);
	FREE_AND_NULL(options);

	врати fn(argc, argv, prefix, repo);
}
