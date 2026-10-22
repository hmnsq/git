#నిర్వచించు USE_THE_REPOSITORY_VARIABLE
#అన్తర్భావ "builtin.h"
#అన్తర్భావ "commit.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "hex.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "commit-graph.h"
#అన్తర్భావ "odb.h"
#అన్తర్భావ "odb/source.h"
#అన్తర్భావ "odb/source-files.h"
#అన్తర్భావ "progress.h"
#అన్తర్భావ "replace-object.h"
#అన్తర్భావ "strbuf.h"
#అన్తర్భావ "tag.h"
#అన్తర్భావ "trace2.h"

#నిర్వచించు BUILTIN_COMMIT_లేఖాచిత్రం_VERIFY_USAGE \
	N_("git commit-graph verify [--object-dir <dir>] [--shallow] [--[no-]progress]")

#నిర్వచించు BUILTIN_COMMIT_లేఖాచిత్రం_WRITE_USAGE \
	N_("git commit-graph write [--object-dir <dir>] [--append]\n" \
	   "                       [--split[=<strategy>]] [--reachable | --stdin-packs | --stdin-commits]\n" \
	   "                       [--changed-paths] [--[no-]max-new-filters <n>] [--[no-]progress]\n" \
	   "                       <split-options>")

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి builtin_commit_లేఖాచిత్రం_verify_usage[] = {
	BUILTIN_COMMIT_లేఖాచిత్రం_VERIFY_USAGE,
	ఖాళీ
};

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి builtin_commit_లేఖాచిత్రం_write_usage[] = {
	BUILTIN_COMMIT_లేఖాచిత్రం_WRITE_USAGE,
	ఖాళీ
};

స్థిర అక్షరం స్థిరరాశి * స్థిరరాశి builtin_commit_లేఖాచిత్రం_usage[] = {
	BUILTIN_COMMIT_లేఖాచిత్రం_VERIFY_USAGE,
	BUILTIN_COMMIT_లేఖాచిత్రం_WRITE_USAGE,
	ఖాళీ,
};

స్థిర నిర్మాణం opts_commit_లేఖాచిత్రం {
	స్థిరరాశి అక్షరం *obj_dir;
	పూర్ణాంకం reachable;
	పూర్ణాంకం stdin_packs;
	పూర్ణాంకం stdin_commits;
	పూర్ణాంకం append;
	పూర్ణాంకం విభజించు;
	పూర్ణాంకం shallow;
	పూర్ణాంకం పురోగతి;
	పూర్ణాంకం enable_మారింది_paths;
} opts;

స్థిర నిర్మాణం option common_opts[] = {
	OPT_STRING(0, "object-dir", &opts.obj_dir,
		   N_("dir"),
		   N_("the object directory to store the graph")),
	OPT_చివర()
};

స్థిర నిర్మాణం option *కూడు_common_options(నిర్మాణం option *గమ్యం)
{
	తిరిగి parse_options_concat(common_opts, గమ్యం);
}

స్థిర పూర్ణాంకం లేఖాచిత్రం_verify(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
			నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం commit_లేఖాచిత్రం *లేఖాచిత్రం = ఖాళీ;
	నిర్మాణం odb_files_dir *dir = ఖాళీ;
	అక్షరం *లేఖాచిత్రం_name;
	అక్షరం *chain_name;
	గణన { OPENED_NONE, OPENED_లేఖాచిత్రం, OPENED_CHAIN } opened = OPENED_NONE;
	పూర్ణాంకం fd;
	నిర్మాణం stat st;
	పూర్ణాంకం flags = 0;
	పూర్ణాంకం incomplete_chain = 0;
	పూర్ణాంకం ret;

	స్థిర నిర్మాణం option builtin_commit_లేఖాచిత్రం_verify_options[] = {
		OPT_BOOL(0, "shallow", &opts.shallow,
			 N_("if the commit-graph is split, only verify the tip file")),
		OPT_BOOL(0, "progress", &opts.పురోగతి,
			 N_("force progress reporting")),
		OPT_చివర(),
	};
	నిర్మాణం option *options = కూడు_common_options(builtin_commit_లేఖాచిత్రం_verify_options);

	trace2_cmd_బహుళకం("verify");

	opts.పురోగతి = isatty(2);
	argc = parse_options(argc, argv, ఉపసర్గ,
			     options,
			     builtin_commit_లేఖాచిత్రం_verify_usage, 0);
	ఒకవేళ (argc)
		usage_with_options(builtin_commit_లేఖాచిత్రం_verify_usage, options);

	ఒకవేళ (!opts.obj_dir)
		opts.obj_dir = repo_get_object_directory(the_repository);
	ఒకవేళ (opts.shallow)
		flags |= COMMIT_లేఖాచిత్రం_VERIFY_SHALLOW;
	ఒకవేళ (opts.పురోగతి)
		flags |= COMMIT_లేఖాచిత్రం_WRITE_పురోగతి;

	dir = odb_ఆధారం_files_కనుగొను_dir(the_repository->objects, opts.obj_dir);
	ఒకవేళ (!dir)
		die(_("could not find object directory matching %s"), opts.obj_dir);

	లేఖాచిత్రం_name = get_commit_లేఖాచిత్రం_filename(dir->abspath);
	chain_name = get_commit_లేఖాచిత్రం_chain_filename(dir->abspath);
	ఒకవేళ (open_commit_లేఖాచిత్రం(లేఖాచిత్రం_name, &fd, &st))
		opened = OPENED_లేఖాచిత్రం;
	లేకపోతే ఒకవేళ (errno != ENOENT)
		die_errno(_("Could not open commit-graph '%s'"), లేఖాచిత్రం_name);
	లేకపోతే ఒకవేళ (open_commit_లేఖాచిత్రం_chain(chain_name, &fd, &st,
					 the_repository->hash_algo))
		opened = OPENED_CHAIN;
	లేకపోతే ఒకవేళ (errno != ENOENT)
		die_errno(_("could not open commit-graph chain '%s'"), chain_name);

	FREE_AND_NULL(లేఖాచిత్రం_name);
	FREE_AND_NULL(chain_name);
	FREE_AND_NULL(options);

	ఒకవేళ (opened == OPENED_NONE)
		తిరిగి 0;
	లేకపోతే ఒకవేళ (opened == OPENED_లేఖాచిత్రం)
		లేఖాచిత్రం = ఎక్కించు_commit_లేఖాచిత్రం_one_fd_st(the_repository, dir->abspath, fd, &st);
	లేకపోతే
		లేఖాచిత్రం = ఎక్కించు_commit_లేఖాచిత్రం_chain_fd_st(the_repository->objects, fd, &st,
						      &incomplete_chain);

	ఒకవేళ (!లేఖాచిత్రం)
		తిరిగి 1;

	ret = verify_commit_లేఖాచిత్రం(లేఖాచిత్రం, flags);
	free_commit_లేఖాచిత్రం(లేఖాచిత్రం);

	ఒకవేళ (incomplete_chain) {
		error("one or more commit-graph chain files could not be loaded");
		ret |= 1;
	}

	తిరిగి ret;
}

బాహ్య పూర్ణాంకం read_replace_refs;
స్థిర నిర్మాణం commit_లేఖాచిత్రం_opts write_opts;

స్థిర పూర్ణాంకం write_option_parse_విభజించు(స్థిరరాశి నిర్మాణం option *opt, స్థిరరాశి అక్షరం *arg,
				    పూర్ణాంకం unset)
{
	గణన commit_లేఖాచిత్రం_విభజించు_flags *flags = opt->విలువ;

	BUG_ON_OPT_NEG(unset);

	opts.విభజించు = 1;
	ఒకవేళ (!arg)
		తిరిగి 0;

	ఒకవేళ (!strcmp(arg, "no-merge"))
		*flags = COMMIT_లేఖాచిత్రం_విభజించు_విలీనం_PROHIBITED;
	లేకపోతే ఒకవేళ (!strcmp(arg, "replace"))
		*flags = COMMIT_లేఖాచిత్రం_విభజించు_REPLACE;
	లేకపోతే
		die(_("unrecognized --split argument, %s"), arg);

	తిరిగి 0;
}

స్థిర పూర్ణాంకం read_one_commit(నిర్మాణం oidset *commits, నిర్మాణం పురోగతి *పురోగతి,
			   స్థిరరాశి అక్షరం *hash)
{
	నిర్మాణం object *ఫలితం;
	నిర్మాణం object_id oid;
	స్థిరరాశి అక్షరం *చివర;

	ఒకవేళ (parse_oid_hex(hash, &oid, &చివర))
		తిరిగి error(_("unexpected non-hex object ID: %s"), hash);

	ఫలితం = deref_ట్యాగ్(the_repository, parse_object(the_repository, &oid),
			   ఖాళీ, 0);
	ఒకవేళ (!ఫలితం)
		తిరిగి error(_("invalid object: %s"), hash);
	లేకపోతే ఒకవేళ (object_as_type(ఫలితం, OBJ_COMMIT, 1))
		oidset_చొప్పించు(commits, &ఫలితం->oid);

	display_పురోగతి(పురోగతి, oidset_కొలత(commits));

	తిరిగి 0;
}

స్థిర పూర్ణాంకం write_option_max_new_filters(స్థిరరాశి నిర్మాణం option *opt,
					స్థిరరాశి అక్షరం *arg,
					పూర్ణాంకం unset)
{
	పూర్ణాంకం *గమ్యం = opt->విలువ;
	ఒకవేళ (unset)
		*గమ్యం = -1;
	లేకపోతే {
		స్థిరరాశి అక్షరం *s;
		*గమ్యం = strtol(arg, (అక్షరం **)&s, 10);
		ఒకవేళ (*s)
			తిరిగి error(_("option `%s' expects a numerical value"),
				     "max-new-filters");
	}
	తిరిగి 0;
}

స్థిర పూర్ణాంకం git_commit_లేఖాచిత్రం_write_config(స్థిరరాశి అక్షరం *var, స్థిరరాశి అక్షరం *విలువ,
					 స్థిరరాశి నిర్మాణం config_context *ctx,
					 శూన్య *cb వాడని)
{
	ఒకవేళ (!strcmp(var, "commitgraph.maxnewfilters"))
		write_opts.max_new_filters = git_config_int(var, విలువ, ctx->kvi);
	లేకపోతే ఒకవేళ (!strcmp(var, "commitgraph.changedpaths"))
		opts.enable_మారింది_paths = git_config_bool(var, విలువ) ? 1 : -1;
	/*
	 * No need to fall-back to 'git_default_config', since this was already
	 * called in 'cmd_commit_graph()'.
	 */
	తిరిగి 0;
}

స్థిర పూర్ణాంకం లేఖాచిత్రం_write(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
		       నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం string_list pack_indexes = STRING_LIST_INIT_DUP;
	నిర్మాణం strbuf buf = STRBUF_INIT;
	నిర్మాణం oidset commits = OIDSET_INIT;
	నిర్మాణం odb_files_dir *dir = ఖాళీ;
	పూర్ణాంకం ఫలితం = 0;
	గణన commit_లేఖాచిత్రం_write_flags flags = 0;
	నిర్మాణం పురోగతి *పురోగతి = ఖాళీ;

	స్థిర నిర్మాణం option builtin_commit_లేఖాచిత్రం_write_options[] = {
		OPT_BOOL(0, "reachable", &opts.reachable,
			N_("start walk at all refs")),
		OPT_BOOL(0, "stdin-packs", &opts.stdin_packs,
			N_("scan pack-indexes listed by stdin for commits")),
		OPT_BOOL(0, "stdin-commits", &opts.stdin_commits,
			N_("start walk at commits listed by stdin")),
		OPT_BOOL(0, "append", &opts.append,
			N_("include all commits already in the commit-graph file")),
		OPT_BOOL(0, "changed-paths", &opts.enable_మారింది_paths,
			N_("enable computation for changed paths")),
		OPT_CALLBACK_F(0, "split", &write_opts.విభజించు_flags, ఖాళీ,
			N_("allow writing an incremental commit-graph file"),
			PARSE_OPT_OPTARG | PARSE_OPT_NONEG,
			write_option_parse_విభజించు),
		OPT_INTEGER(0, "max-commits", &write_opts.max_commits,
			N_("maximum number of commits in a non-base split commit-graph")),
		OPT_INTEGER(0, "size-multiple", &write_opts.కొలత_గుణిజం,
			N_("maximum ratio between two levels of a split commit-graph")),
		OPT_EXPIRY_DATE(0, "expire-time", &write_opts.expire_సమయం,
			N_("only expire files older than a given date-time")),
		OPT_CALLBACK_F(0, "max-new-filters", &write_opts.max_new_filters,
			ఖాళీ, N_("maximum number of changed-path Bloom filters to compute"),
			0, write_option_max_new_filters),
		OPT_BOOL(0, "progress", &opts.పురోగతి,
			 N_("force progress reporting")),
		OPT_చివర(),
	};
	నిర్మాణం option *options = కూడు_common_options(builtin_commit_లేఖాచిత్రం_write_options);

	opts.పురోగతి = isatty(2);
	opts.enable_మారింది_paths = -1;
	write_opts.కొలత_గుణిజం = 2;
	write_opts.max_commits = 0;
	write_opts.expire_సమయం = 0;
	write_opts.max_new_filters = -1;

	trace2_cmd_బహుళకం("write");

	repo_config(the_repository, git_commit_లేఖాచిత్రం_write_config, &opts);

	argc = parse_options(argc, argv, ఉపసర్గ,
			     options,
			     builtin_commit_లేఖాచిత్రం_write_usage, 0);
	ఒకవేళ (argc)
		usage_with_options(builtin_commit_లేఖాచిత్రం_write_usage, options);

	ఒకవేళ (opts.reachable + opts.stdin_packs + opts.stdin_commits > 1)
		die(_("use at most one of --reachable, --stdin-commits, or --stdin-packs"));
	ఒకవేళ (!opts.obj_dir)
		opts.obj_dir = repo_get_object_directory(the_repository);
	ఒకవేళ (opts.append)
		flags |= COMMIT_లేఖాచిత్రం_WRITE_APPEND;
	ఒకవేళ (opts.విభజించు)
		flags |= COMMIT_లేఖాచిత్రం_WRITE_విభజించు;
	ఒకవేళ (opts.పురోగతి)
		flags |= COMMIT_లేఖాచిత్రం_WRITE_పురోగతి;
	ఒకవేళ (!opts.enable_మారింది_paths)
		flags |= COMMIT_లేఖాచిత్రం_NO_WRITE_BLOOM_FILTERS;
	ఒకవేళ (opts.enable_మారింది_paths == 1 ||
	    git_env_bool(GIT_TEST_COMMIT_లేఖాచిత్రం_మారింది_PATHS, 0))
		flags |= COMMIT_లేఖాచిత్రం_WRITE_BLOOM_FILTERS;

	dir = odb_ఆధారం_files_కనుగొను_dir(the_repository->objects, opts.obj_dir);
	ఒకవేళ (!dir)
		die(_("could not find object directory matching %s"), opts.obj_dir);

	ఒకవేళ (opts.reachable) {
		ఒకవేళ (write_commit_లేఖాచిత్రం_reachable(the_repository, dir->abspath, flags, &write_opts))
			ఫలితం = 1;
		వెళ్ళు cleanup;
	}

	ఒకవేళ (opts.stdin_packs) {
		నిర్మాణం strbuf packname = STRBUF_INIT;
		కొలత_t dirlen;

		strbuf_addf(&packname, "%s/pack/", dir->abspath);
		dirlen = packname.len;

		అయితే (strbuf_getline(&buf, stdin) != EOF) {
			strbuf_setlen(&packname, dirlen);
			strbuf_addbuf(&packname, &buf);
			string_list_append(&pack_indexes, packname.buf);
		}

		strbuf_release(&packname);
	} లేకపోతే ఒకవేళ (opts.stdin_commits) {
		oidset_init(&commits, 0);
		ఒకవేళ (opts.పురోగతి)
			పురోగతి = ప్రారంభం_delayed_పురోగతి(
				the_repository,
				_("Collecting commits from input"), 0);

		అయితే (strbuf_getline(&buf, stdin) != EOF) {
			ఒకవేళ (read_one_commit(&commits, పురోగతి, buf.buf)) {
				ఫలితం = 1;
				stop_పురోగతి(&పురోగతి);
				వెళ్ళు cleanup;
			}
		}

		stop_పురోగతి(&పురోగతి);
	}

	ఒకవేళ (write_commit_లేఖాచిత్రం(the_repository, dir->abspath,
			       opts.stdin_packs ? &pack_indexes : ఖాళీ,
			       opts.stdin_commits ? &commits : ఖాళీ,
			       flags,
			       &write_opts))
		ఫలితం = 1;

cleanup:
	FREE_AND_NULL(options);
	string_list_clear(&pack_indexes, 0);
	strbuf_release(&buf);
	oidset_clear(&commits);
	తిరిగి ఫలితం;
}

పూర్ణాంకం cmd_commit_లేఖాచిత్రం(పూర్ణాంకం argc,
		     స్థిరరాశి అక్షరం **argv,
		     స్థిరరాశి అక్షరం *ఉపసర్గ,
		     నిర్మాణం repository *repo)
{
	parse_opt_subcommand_fn *fn = ఖాళీ;
	నిర్మాణం option builtin_commit_లేఖాచిత్రం_options[] = {
		OPT_SUBCOMMAND("verify", &fn, లేఖాచిత్రం_verify),
		OPT_SUBCOMMAND("write", &fn, లేఖాచిత్రం_write),
		OPT_చివర(),
	};
	నిర్మాణం option *options = parse_options_concat(builtin_commit_లేఖాచిత్రం_options, common_opts);

	repo_config(the_repository, git_default_config, ఖాళీ);

	disable_replace_refs();
	save_commit_బఫరు = 0;

	argc = parse_options(argc, argv, ఉపసర్గ, options,
			     builtin_commit_లేఖాచిత్రం_usage, 0);
	FREE_AND_NULL(options);

	తిరిగి fn(argc, argv, ఉపసర్గ, repo);
}
