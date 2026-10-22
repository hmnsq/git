#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "commit.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hex.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "commit-graph.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "odb.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "odb/source.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "odb/source-files.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "progress.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "replace-object.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "strbuf.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "tag.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "trace2.h"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BUILTIN_COMMIT_ਗ੍ਰਾਫ_VERIFY_USAGE \
	N_("git commit-graph verify [--object-dir <dir>] [--shallow] [--[no-]progress]")

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BUILTIN_COMMIT_ਗ੍ਰਾਫ_WRITE_USAGE \
	N_("git commit-graph write [--object-dir <dir>] [--append]\n" \
	   "                       [--split[=<strategy>]] [--reachable | --stdin-packs | --stdin-commits]\n" \
	   "                       [--changed-paths] [--[no-]max-new-filters <n>] [--[no-]progress]\n" \
	   "                       <split-options>")

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ builtin_commit_ਗ੍ਰਾਫ_verify_usage[] = {
	BUILTIN_COMMIT_ਗ੍ਰਾਫ_VERIFY_USAGE,
	ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ builtin_commit_ਗ੍ਰਾਫ_write_usage[] = {
	BUILTIN_COMMIT_ਗ੍ਰਾਫ_WRITE_USAGE,
	ਨਿਰਾ
};

ਸਥਿਰ ਅੱਖਰ ਅਚਲ * ਅਚਲ builtin_commit_ਗ੍ਰਾਫ_usage[] = {
	BUILTIN_COMMIT_ਗ੍ਰਾਫ_VERIFY_USAGE,
	BUILTIN_COMMIT_ਗ੍ਰਾਫ_WRITE_USAGE,
	ਨਿਰਾ,
};

ਸਥਿਰ ਬਣਤਰ opts_commit_ਗ੍ਰਾਫ {
	ਅਚਲ ਅੱਖਰ *obj_dir;
	ਪੂਰਨ reachable;
	ਪੂਰਨ stdin_packs;
	ਪੂਰਨ stdin_commits;
	ਪੂਰਨ append;
	ਪੂਰਨ ਵੰਡੋ;
	ਪੂਰਨ shallow;
	ਪੂਰਨ ਤਰੱਕੀ;
	ਪੂਰਨ enable_ਬਦਲਿਆ_paths;
} opts;

ਸਥਿਰ ਬਣਤਰ option common_opts[] = {
	OPT_STRING(0, "object-dir", &opts.obj_dir,
		   N_("dir"),
		   N_("the object directory to store the graph")),
	OPT_ਸਿਰਾ()
};

ਸਥਿਰ ਬਣਤਰ option *ਜੋੜੋ_common_options(ਬਣਤਰ option *ਮੰਜ਼ਿਲ)
{
	ਵਾਪਸ parse_options_concat(common_opts, ਮੰਜ਼ਿਲ);
}

ਸਥਿਰ ਪੂਰਨ ਗ੍ਰਾਫ_verify(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ commit_ਗ੍ਰਾਫ *ਗ੍ਰਾਫ = ਨਿਰਾ;
	ਬਣਤਰ odb_files_dir *dir = ਨਿਰਾ;
	ਅੱਖਰ *ਗ੍ਰਾਫ_name;
	ਅੱਖਰ *chain_name;
	ਗਿਣਤੀ { OPENED_NONE, OPENED_ਗ੍ਰਾਫ, OPENED_CHAIN } opened = OPENED_NONE;
	ਪੂਰਨ fd;
	ਬਣਤਰ stat st;
	ਪੂਰਨ flags = 0;
	ਪੂਰਨ incomplete_chain = 0;
	ਪੂਰਨ ret;

	ਸਥਿਰ ਬਣਤਰ option builtin_commit_ਗ੍ਰਾਫ_verify_options[] = {
		OPT_BOOL(0, "shallow", &opts.shallow,
			 N_("if the commit-graph is split, only verify the tip file")),
		OPT_BOOL(0, "progress", &opts.ਤਰੱਕੀ,
			 N_("force progress reporting")),
		OPT_ਸਿਰਾ(),
	};
	ਬਣਤਰ option *options = ਜੋੜੋ_common_options(builtin_commit_ਗ੍ਰਾਫ_verify_options);

	trace2_cmd_ਬਹੁਲਕ("verify");

	opts.ਤਰੱਕੀ = isatty(2);
	argc = parse_options(argc, argv, ਅਗੇਤਰ,
			     options,
			     builtin_commit_ਗ੍ਰਾਫ_verify_usage, 0);
	ਜੇ (argc)
		usage_with_options(builtin_commit_ਗ੍ਰਾਫ_verify_usage, options);

	ਜੇ (!opts.obj_dir)
		opts.obj_dir = repo_get_object_directory(the_repository);
	ਜੇ (opts.shallow)
		flags |= COMMIT_ਗ੍ਰਾਫ_VERIFY_SHALLOW;
	ਜੇ (opts.ਤਰੱਕੀ)
		flags |= COMMIT_ਗ੍ਰਾਫ_WRITE_ਤਰੱਕੀ;

	dir = odb_ਸਰੋਤ_files_ਲੱਭੋ_dir(the_repository->objects, opts.obj_dir);
	ਜੇ (!dir)
		die(_("could not find object directory matching %s"), opts.obj_dir);

	ਗ੍ਰਾਫ_name = get_commit_ਗ੍ਰਾਫ_filename(dir->abspath);
	chain_name = get_commit_ਗ੍ਰਾਫ_chain_filename(dir->abspath);
	ਜੇ (open_commit_ਗ੍ਰਾਫ(ਗ੍ਰਾਫ_name, &fd, &st))
		opened = OPENED_ਗ੍ਰਾਫ;
	ਨਹੀਂਤਾਂ ਜੇ (errno != ENOENT)
		die_errno(_("Could not open commit-graph '%s'"), ਗ੍ਰਾਫ_name);
	ਨਹੀਂਤਾਂ ਜੇ (open_commit_ਗ੍ਰਾਫ_chain(chain_name, &fd, &st,
					 the_repository->hash_algo))
		opened = OPENED_CHAIN;
	ਨਹੀਂਤਾਂ ਜੇ (errno != ENOENT)
		die_errno(_("could not open commit-graph chain '%s'"), chain_name);

	FREE_AND_NULL(ਗ੍ਰਾਫ_name);
	FREE_AND_NULL(chain_name);
	FREE_AND_NULL(options);

	ਜੇ (opened == OPENED_NONE)
		ਵਾਪਸ 0;
	ਨਹੀਂਤਾਂ ਜੇ (opened == OPENED_ਗ੍ਰਾਫ)
		ਗ੍ਰਾਫ = ਲੱਦੋ_commit_ਗ੍ਰਾਫ_one_fd_st(the_repository, dir->abspath, fd, &st);
	ਨਹੀਂਤਾਂ
		ਗ੍ਰਾਫ = ਲੱਦੋ_commit_ਗ੍ਰਾਫ_chain_fd_st(the_repository->objects, fd, &st,
						      &incomplete_chain);

	ਜੇ (!ਗ੍ਰਾਫ)
		ਵਾਪਸ 1;

	ret = verify_commit_ਗ੍ਰਾਫ(ਗ੍ਰਾਫ, flags);
	free_commit_ਗ੍ਰਾਫ(ਗ੍ਰਾਫ);

	ਜੇ (incomplete_chain) {
		error("one or more commit-graph chain files could not be loaded");
		ret |= 1;
	}

	ਵਾਪਸ ret;
}

ਬਾਹਰੀ ਪੂਰਨ read_replace_refs;
ਸਥਿਰ ਬਣਤਰ commit_ਗ੍ਰਾਫ_opts write_opts;

ਸਥਿਰ ਪੂਰਨ write_option_parse_ਵੰਡੋ(ਅਚਲ ਬਣਤਰ option *opt, ਅਚਲ ਅੱਖਰ *arg,
				    ਪੂਰਨ unset)
{
	ਗਿਣਤੀ commit_ਗ੍ਰਾਫ_ਵੰਡੋ_flags *flags = opt->ਮੁੱਲ;

	BUG_ON_OPT_NEG(unset);

	opts.ਵੰਡੋ = 1;
	ਜੇ (!arg)
		ਵਾਪਸ 0;

	ਜੇ (!strcmp(arg, "no-merge"))
		*flags = COMMIT_ਗ੍ਰਾਫ_ਵੰਡੋ_ਮਿਲਾਓ_PROHIBITED;
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(arg, "replace"))
		*flags = COMMIT_ਗ੍ਰਾਫ_ਵੰਡੋ_REPLACE;
	ਨਹੀਂਤਾਂ
		die(_("unrecognized --split argument, %s"), arg);

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ read_one_commit(ਬਣਤਰ oidset *commits, ਬਣਤਰ ਤਰੱਕੀ *ਤਰੱਕੀ,
			   ਅਚਲ ਅੱਖਰ *hash)
{
	ਬਣਤਰ object *ਨਤੀਜਾ;
	ਬਣਤਰ object_id oid;
	ਅਚਲ ਅੱਖਰ *ਸਿਰਾ;

	ਜੇ (parse_oid_hex(hash, &oid, &ਸਿਰਾ))
		ਵਾਪਸ error(_("unexpected non-hex object ID: %s"), hash);

	ਨਤੀਜਾ = deref_ਟੈਗ(the_repository, parse_object(the_repository, &oid),
			   ਨਿਰਾ, 0);
	ਜੇ (!ਨਤੀਜਾ)
		ਵਾਪਸ error(_("invalid object: %s"), hash);
	ਨਹੀਂਤਾਂ ਜੇ (object_as_type(ਨਤੀਜਾ, OBJ_COMMIT, 1))
		oidset_ਪਾਓ(commits, &ਨਤੀਜਾ->oid);

	display_ਤਰੱਕੀ(ਤਰੱਕੀ, oidset_ਮਾਪ(commits));

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ write_option_max_new_filters(ਅਚਲ ਬਣਤਰ option *opt,
					ਅਚਲ ਅੱਖਰ *arg,
					ਪੂਰਨ unset)
{
	ਪੂਰਨ *ਮੰਜ਼ਿਲ = opt->ਮੁੱਲ;
	ਜੇ (unset)
		*ਮੰਜ਼ਿਲ = -1;
	ਨਹੀਂਤਾਂ {
		ਅਚਲ ਅੱਖਰ *s;
		*ਮੰਜ਼ਿਲ = strtol(arg, (ਅੱਖਰ **)&s, 10);
		ਜੇ (*s)
			ਵਾਪਸ error(_("option `%s' expects a numerical value"),
				     "max-new-filters");
	}
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ git_commit_ਗ੍ਰਾਫ_write_config(ਅਚਲ ਅੱਖਰ *var, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ,
					 ਅਚਲ ਬਣਤਰ config_context *ctx,
					 ਖਾਲੀ *cb ਅਣਵਰਤਿਆ)
{
	ਜੇ (!strcmp(var, "commitgraph.maxnewfilters"))
		write_opts.max_new_filters = git_config_int(var, ਮੁੱਲ, ctx->kvi);
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(var, "commitgraph.changedpaths"))
		opts.enable_ਬਦਲਿਆ_paths = git_config_bool(var, ਮੁੱਲ) ? 1 : -1;
	/*
	 * No need to fall-back to 'git_default_config', since this was already
	 * called in 'cmd_commit_graph()'.
	 */
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ ਗ੍ਰਾਫ_write(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		       ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ string_list pack_indexes = STRING_LIST_INIT_DUP;
	ਬਣਤਰ strbuf buf = STRBUF_INIT;
	ਬਣਤਰ oidset commits = OIDSET_INIT;
	ਬਣਤਰ odb_files_dir *dir = ਨਿਰਾ;
	ਪੂਰਨ ਨਤੀਜਾ = 0;
	ਗਿਣਤੀ commit_ਗ੍ਰਾਫ_write_flags flags = 0;
	ਬਣਤਰ ਤਰੱਕੀ *ਤਰੱਕੀ = ਨਿਰਾ;

	ਸਥਿਰ ਬਣਤਰ option builtin_commit_ਗ੍ਰਾਫ_write_options[] = {
		OPT_BOOL(0, "reachable", &opts.reachable,
			N_("start walk at all refs")),
		OPT_BOOL(0, "stdin-packs", &opts.stdin_packs,
			N_("scan pack-indexes listed by stdin for commits")),
		OPT_BOOL(0, "stdin-commits", &opts.stdin_commits,
			N_("start walk at commits listed by stdin")),
		OPT_BOOL(0, "append", &opts.append,
			N_("include all commits already in the commit-graph file")),
		OPT_BOOL(0, "changed-paths", &opts.enable_ਬਦਲਿਆ_paths,
			N_("enable computation for changed paths")),
		OPT_CALLBACK_F(0, "split", &write_opts.ਵੰਡੋ_flags, ਨਿਰਾ,
			N_("allow writing an incremental commit-graph file"),
			PARSE_OPT_OPTARG | PARSE_OPT_NONEG,
			write_option_parse_ਵੰਡੋ),
		OPT_INTEGER(0, "max-commits", &write_opts.max_commits,
			N_("maximum number of commits in a non-base split commit-graph")),
		OPT_INTEGER(0, "size-multiple", &write_opts.ਮਾਪ_ਗੁਣਜ,
			N_("maximum ratio between two levels of a split commit-graph")),
		OPT_EXPIRY_DATE(0, "expire-time", &write_opts.expire_ਸਮਾਂ,
			N_("only expire files older than a given date-time")),
		OPT_CALLBACK_F(0, "max-new-filters", &write_opts.max_new_filters,
			ਨਿਰਾ, N_("maximum number of changed-path Bloom filters to compute"),
			0, write_option_max_new_filters),
		OPT_BOOL(0, "progress", &opts.ਤਰੱਕੀ,
			 N_("force progress reporting")),
		OPT_ਸਿਰਾ(),
	};
	ਬਣਤਰ option *options = ਜੋੜੋ_common_options(builtin_commit_ਗ੍ਰਾਫ_write_options);

	opts.ਤਰੱਕੀ = isatty(2);
	opts.enable_ਬਦਲਿਆ_paths = -1;
	write_opts.ਮਾਪ_ਗੁਣਜ = 2;
	write_opts.max_commits = 0;
	write_opts.expire_ਸਮਾਂ = 0;
	write_opts.max_new_filters = -1;

	trace2_cmd_ਬਹੁਲਕ("write");

	repo_config(the_repository, git_commit_ਗ੍ਰਾਫ_write_config, &opts);

	argc = parse_options(argc, argv, ਅਗੇਤਰ,
			     options,
			     builtin_commit_ਗ੍ਰਾਫ_write_usage, 0);
	ਜੇ (argc)
		usage_with_options(builtin_commit_ਗ੍ਰਾਫ_write_usage, options);

	ਜੇ (opts.reachable + opts.stdin_packs + opts.stdin_commits > 1)
		die(_("use at most one of --reachable, --stdin-commits, or --stdin-packs"));
	ਜੇ (!opts.obj_dir)
		opts.obj_dir = repo_get_object_directory(the_repository);
	ਜੇ (opts.append)
		flags |= COMMIT_ਗ੍ਰਾਫ_WRITE_APPEND;
	ਜੇ (opts.ਵੰਡੋ)
		flags |= COMMIT_ਗ੍ਰਾਫ_WRITE_ਵੰਡੋ;
	ਜੇ (opts.ਤਰੱਕੀ)
		flags |= COMMIT_ਗ੍ਰਾਫ_WRITE_ਤਰੱਕੀ;
	ਜੇ (!opts.enable_ਬਦਲਿਆ_paths)
		flags |= COMMIT_ਗ੍ਰਾਫ_NO_WRITE_BLOOM_FILTERS;
	ਜੇ (opts.enable_ਬਦਲਿਆ_paths == 1 ||
	    git_env_bool(GIT_TEST_COMMIT_ਗ੍ਰਾਫ_ਬਦਲਿਆ_PATHS, 0))
		flags |= COMMIT_ਗ੍ਰਾਫ_WRITE_BLOOM_FILTERS;

	dir = odb_ਸਰੋਤ_files_ਲੱਭੋ_dir(the_repository->objects, opts.obj_dir);
	ਜੇ (!dir)
		die(_("could not find object directory matching %s"), opts.obj_dir);

	ਜੇ (opts.reachable) {
		ਜੇ (write_commit_ਗ੍ਰਾਫ_reachable(the_repository, dir->abspath, flags, &write_opts))
			ਨਤੀਜਾ = 1;
		ਜਾਓ cleanup;
	}

	ਜੇ (opts.stdin_packs) {
		ਬਣਤਰ strbuf packname = STRBUF_INIT;
		ਮਾਪ_t dirlen;

		strbuf_addf(&packname, "%s/pack/", dir->abspath);
		dirlen = packname.len;

		ਜਦੋਂ (strbuf_getline(&buf, stdin) != EOF) {
			strbuf_setlen(&packname, dirlen);
			strbuf_addbuf(&packname, &buf);
			string_list_append(&pack_indexes, packname.buf);
		}

		strbuf_release(&packname);
	} ਨਹੀਂਤਾਂ ਜੇ (opts.stdin_commits) {
		oidset_init(&commits, 0);
		ਜੇ (opts.ਤਰੱਕੀ)
			ਤਰੱਕੀ = ਸ਼ੁਰੂ_delayed_ਤਰੱਕੀ(
				the_repository,
				_("Collecting commits from input"), 0);

		ਜਦੋਂ (strbuf_getline(&buf, stdin) != EOF) {
			ਜੇ (read_one_commit(&commits, ਤਰੱਕੀ, buf.buf)) {
				ਨਤੀਜਾ = 1;
				stop_ਤਰੱਕੀ(&ਤਰੱਕੀ);
				ਜਾਓ cleanup;
			}
		}

		stop_ਤਰੱਕੀ(&ਤਰੱਕੀ);
	}

	ਜੇ (write_commit_ਗ੍ਰਾਫ(the_repository, dir->abspath,
			       opts.stdin_packs ? &pack_indexes : ਨਿਰਾ,
			       opts.stdin_commits ? &commits : ਨਿਰਾ,
			       flags,
			       &write_opts))
		ਨਤੀਜਾ = 1;

cleanup:
	FREE_AND_NULL(options);
	string_list_clear(&pack_indexes, 0);
	strbuf_release(&buf);
	oidset_clear(&commits);
	ਵਾਪਸ ਨਤੀਜਾ;
}

ਪੂਰਨ cmd_commit_ਗ੍ਰਾਫ(ਪੂਰਨ argc,
		     ਅਚਲ ਅੱਖਰ **argv,
		     ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		     ਬਣਤਰ repository *repo)
{
	parse_opt_subcommand_fn *fn = ਨਿਰਾ;
	ਬਣਤਰ option builtin_commit_ਗ੍ਰਾਫ_options[] = {
		OPT_SUBCOMMAND("verify", &fn, ਗ੍ਰਾਫ_verify),
		OPT_SUBCOMMAND("write", &fn, ਗ੍ਰਾਫ_write),
		OPT_ਸਿਰਾ(),
	};
	ਬਣਤਰ option *options = parse_options_concat(builtin_commit_ਗ੍ਰਾਫ_options, common_opts);

	repo_config(the_repository, git_default_config, ਨਿਰਾ);

	disable_replace_refs();
	save_commit_ਬਫਰ = 0;

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options,
			     builtin_commit_ਗ੍ਰਾਫ_usage, 0);
	FREE_AND_NULL(options);

	ਵਾਪਸ fn(argc, argv, ਅਗੇਤਰ, repo);
}
