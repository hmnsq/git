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
	số_nguyên reachable;
	số_nguyên stdin_packs;
	số_nguyên stdin_commits;
	số_nguyên append;
	số_nguyên split;
	số_nguyên shallow;
	số_nguyên progress;
	số_nguyên enable_changed_paths;
} opts;

static struct option common_opts[] = {
	OPT_STRING(0, "object-dir", &opts.obj_dir,
		   N_("dir"),
		   N_("the object directory to store the graph")),
	OPT_END()
};

static struct option *add_common_options(struct option *to)
{
	trả_về parse_options_concat(common_opts, to);
}

static số_nguyên graph_verify(số_nguyên argc, const char **argv, const char *prefix,
			struct repository *repo UNUSED)
{
	struct commit_graph *graph = NULL;
	struct odb_files_dir *dir = NULL;
	char *graph_name;
	char *chain_name;
	enum { OPENED_NONE, OPENED_GRAPH, OPENED_CHAIN } opened = OPENED_NONE;
	số_nguyên fd;
	struct stat st;
	số_nguyên flags = 0;
	số_nguyên incomplete_chain = 0;
	số_nguyên ret;

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
	nếu (argc)
		usage_with_options(builtin_commit_graph_verify_usage, options);

	nếu (!opts.obj_dir)
		opts.obj_dir = repo_get_object_directory(the_repository);
	nếu (opts.shallow)
		flags |= COMMIT_GRAPH_VERIFY_SHALLOW;
	nếu (opts.progress)
		flags |= COMMIT_GRAPH_WRITE_PROGRESS;

	dir = odb_source_files_find_dir(the_repository->objects, opts.obj_dir);
	nếu (!dir)
		die(_("could not find object directory matching %s"), opts.obj_dir);

	graph_name = get_commit_graph_filename(dir->abspath);
	chain_name = get_commit_graph_chain_filename(dir->abspath);
	nếu (open_commit_graph(graph_name, &fd, &st))
		opened = OPENED_GRAPH;
	không_thì nếu (errno != ENOENT)
		die_errno(_("Could not open commit-graph '%s'"), graph_name);
	không_thì nếu (open_commit_graph_chain(chain_name, &fd, &st,
					 the_repository->hash_algo))
		opened = OPENED_CHAIN;
	không_thì nếu (errno != ENOENT)
		die_errno(_("could not open commit-graph chain '%s'"), chain_name);

	FREE_AND_NULL(graph_name);
	FREE_AND_NULL(chain_name);
	FREE_AND_NULL(options);

	nếu (opened == OPENED_NONE)
		trả_về 0;
	không_thì nếu (opened == OPENED_GRAPH)
		graph = load_commit_graph_one_fd_st(the_repository, dir->abspath, fd, &st);
	không_thì
		graph = load_commit_graph_chain_fd_st(the_repository->objects, fd, &st,
						      &incomplete_chain);

	nếu (!graph)
		trả_về 1;

	ret = verify_commit_graph(graph, flags);
	free_commit_graph(graph);

	nếu (incomplete_chain) {
		error("one or more commit-graph chain files could not be loaded");
		ret |= 1;
	}

	trả_về ret;
}

extern số_nguyên read_replace_refs;
static struct commit_graph_opts write_opts;

static số_nguyên write_option_parse_split(const struct option *opt, const char *arg,
				    số_nguyên unset)
{
	enum commit_graph_split_flags *flags = opt->value;

	BUG_ON_OPT_NEG(unset);

	opts.split = 1;
	nếu (!arg)
		trả_về 0;

	nếu (!strcmp(arg, "no-merge"))
		*flags = COMMIT_GRAPH_SPLIT_MERGE_PROHIBITED;
	không_thì nếu (!strcmp(arg, "replace"))
		*flags = COMMIT_GRAPH_SPLIT_REPLACE;
	không_thì
		die(_("unrecognized --split argument, %s"), arg);

	trả_về 0;
}

static số_nguyên read_one_commit(struct oidset *commits, struct progress *progress,
			   const char *hash)
{
	struct object *result;
	struct object_id oid;
	const char *end;

	nếu (parse_oid_hex(hash, &oid, &end))
		trả_về error(_("unexpected non-hex object ID: %s"), hash);

	result = deref_tag(the_repository, parse_object(the_repository, &oid),
			   NULL, 0);
	nếu (!result)
		trả_về error(_("invalid object: %s"), hash);
	không_thì nếu (object_as_type(result, OBJ_COMMIT, 1))
		oidset_insert(commits, &result->oid);

	display_progress(progress, oidset_size(commits));

	trả_về 0;
}

static số_nguyên write_option_max_new_filters(const struct option *opt,
					const char *arg,
					số_nguyên unset)
{
	số_nguyên *to = opt->value;
	nếu (unset)
		*to = -1;
	không_thì {
		const char *s;
		*to = strtol(arg, (char **)&s, 10);
		nếu (*s)
			trả_về error(_("option `%s' expects a numerical value"),
				     "max-new-filters");
	}
	trả_về 0;
}

static số_nguyên git_commit_graph_write_config(const char *var, const char *value,
					 const struct config_context *ctx,
					 void *cb UNUSED)
{
	nếu (!strcmp(var, "commitgraph.maxnewfilters"))
		write_opts.max_new_filters = git_config_int(var, value, ctx->kvi);
	không_thì nếu (!strcmp(var, "commitgraph.changedpaths"))
		opts.enable_changed_paths = git_config_bool(var, value) ? 1 : -1;
	/*
	 * No need to fall-back to 'git_default_config', since this was already
	 * called in 'cmd_commit_graph()'.
	 */
	trả_về 0;
}

static số_nguyên graph_write(số_nguyên argc, const char **argv, const char *prefix,
		       struct repository *repo UNUSED)
{
	struct string_list pack_indexes = STRING_LIST_INIT_DUP;
	struct strbuf buf = STRBUF_INIT;
	struct oidset commits = OIDSET_INIT;
	struct odb_files_dir *dir = NULL;
	số_nguyên result = 0;
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
	nếu (argc)
		usage_with_options(builtin_commit_graph_write_usage, options);

	nếu (opts.reachable + opts.stdin_packs + opts.stdin_commits > 1)
		die(_("use at most one of --reachable, --stdin-commits, or --stdin-packs"));
	nếu (!opts.obj_dir)
		opts.obj_dir = repo_get_object_directory(the_repository);
	nếu (opts.append)
		flags |= COMMIT_GRAPH_WRITE_APPEND;
	nếu (opts.split)
		flags |= COMMIT_GRAPH_WRITE_SPLIT;
	nếu (opts.progress)
		flags |= COMMIT_GRAPH_WRITE_PROGRESS;
	nếu (!opts.enable_changed_paths)
		flags |= COMMIT_GRAPH_NO_WRITE_BLOOM_FILTERS;
	nếu (opts.enable_changed_paths == 1 ||
	    git_env_bool(GIT_TEST_COMMIT_GRAPH_CHANGED_PATHS, 0))
		flags |= COMMIT_GRAPH_WRITE_BLOOM_FILTERS;

	dir = odb_source_files_find_dir(the_repository->objects, opts.obj_dir);
	nếu (!dir)
		die(_("could not find object directory matching %s"), opts.obj_dir);

	nếu (opts.reachable) {
		nếu (write_commit_graph_reachable(the_repository, dir->abspath, flags, &write_opts))
			result = 1;
		goto cleanup;
	}

	nếu (opts.stdin_packs) {
		struct strbuf packname = STRBUF_INIT;
		size_t dirlen;

		strbuf_addf(&packname, "%s/pack/", dir->abspath);
		dirlen = packname.len;

		trong_khi (strbuf_getline(&buf, stdin) != EOF) {
			strbuf_setlen(&packname, dirlen);
			strbuf_addbuf(&packname, &buf);
			string_list_append(&pack_indexes, packname.buf);
		}

		strbuf_release(&packname);
	} không_thì nếu (opts.stdin_commits) {
		oidset_init(&commits, 0);
		nếu (opts.progress)
			progress = start_delayed_progress(
				the_repository,
				_("Collecting commits from input"), 0);

		trong_khi (strbuf_getline(&buf, stdin) != EOF) {
			nếu (read_one_commit(&commits, progress, buf.buf)) {
				result = 1;
				stop_progress(&progress);
				goto cleanup;
			}
		}

		stop_progress(&progress);
	}

	nếu (write_commit_graph(the_repository, dir->abspath,
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
	trả_về result;
}

số_nguyên cmd_commit_graph(số_nguyên argc,
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

	trả_về fn(argc, argv, prefix, repo);
}
