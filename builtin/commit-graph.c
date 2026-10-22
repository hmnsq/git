#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE
#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "commit.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "commit-graph.h"
#ಅನ್ತರ್ಭಾವ "odb.h"
#ಅನ್ತರ್ಭಾವ "odb/source.h"
#ಅನ್ತರ್ಭಾವ "odb/source-files.h"
#ಅನ್ತರ್ಭಾವ "progress.h"
#ಅನ್ತರ್ಭಾವ "replace-object.h"
#ಅನ್ತರ್ಭಾವ "strbuf.h"
#ಅನ್ತರ್ಭಾವ "tag.h"
#ಅನ್ತರ್ಭಾವ "trace2.h"

#ವ್ಯಾಖ್ಯಾನಿಸು BUILTIN_COMMIT_ನಕ್ಷೆ_VERIFY_USAGE \
	N_("git commit-graph verify [--object-dir <dir>] [--shallow] [--[no-]progress]")

#ವ್ಯಾಖ್ಯಾನಿಸು BUILTIN_COMMIT_ನಕ್ಷೆ_WRITE_USAGE \
	N_("git commit-graph write [--object-dir <dir>] [--append]\n" \
	   "                       [--split[=<strategy>]] [--reachable | --stdin-packs | --stdin-commits]\n" \
	   "                       [--changed-paths] [--[no-]max-new-filters <n>] [--[no-]progress]\n" \
	   "                       <split-options>")

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ builtin_commit_ನಕ್ಷೆ_verify_usage[] = {
	BUILTIN_COMMIT_ನಕ್ಷೆ_VERIFY_USAGE,
	ಖಾಲಿ
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ builtin_commit_ನಕ್ಷೆ_write_usage[] = {
	BUILTIN_COMMIT_ನಕ್ಷೆ_WRITE_USAGE,
	ಖಾಲಿ
};

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ * ಸ್ಥಿರಾಂಕ builtin_commit_ನಕ್ಷೆ_usage[] = {
	BUILTIN_COMMIT_ನಕ್ಷೆ_VERIFY_USAGE,
	BUILTIN_COMMIT_ನಕ್ಷೆ_WRITE_USAGE,
	ಖಾಲಿ,
};

ಸ್ಥಿರ ರಚನೆ opts_commit_ನಕ್ಷೆ {
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *obj_dir;
	ಪೂರ್ಣಾಂಕ reachable;
	ಪೂರ್ಣಾಂಕ stdin_packs;
	ಪೂರ್ಣಾಂಕ stdin_commits;
	ಪೂರ್ಣಾಂಕ append;
	ಪೂರ್ಣಾಂಕ ವಿಭಜಿಸು;
	ಪೂರ್ಣಾಂಕ shallow;
	ಪೂರ್ಣಾಂಕ ಪ್ರಗತಿ;
	ಪೂರ್ಣಾಂಕ enable_ಬದಲಾಯಿತು_paths;
} opts;

ಸ್ಥಿರ ರಚನೆ option common_opts[] = {
	OPT_STRING(0, "object-dir", &opts.obj_dir,
		   N_("dir"),
		   N_("the object directory to store the graph")),
	OPT_ಕೊನೆ()
};

ಸ್ಥಿರ ರಚನೆ option *ಕೂಡಿಸು_common_options(ರಚನೆ option *ಗಮ್ಯ)
{
	ಹಿಂತಿರುಗಿ parse_options_concat(common_opts, ಗಮ್ಯ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ನಕ್ಷೆ_verify(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ commit_ನಕ್ಷೆ *ನಕ್ಷೆ = ಖಾಲಿ;
	ರಚನೆ odb_files_dir *dir = ಖಾಲಿ;
	ಅಕ್ಷರ *ನಕ್ಷೆ_name;
	ಅಕ್ಷರ *chain_name;
	ಎಣಿಕೆ { OPENED_NONE, OPENED_ನಕ್ಷೆ, OPENED_CHAIN } opened = OPENED_NONE;
	ಪೂರ್ಣಾಂಕ fd;
	ರಚನೆ stat st;
	ಪೂರ್ಣಾಂಕ flags = 0;
	ಪೂರ್ಣಾಂಕ incomplete_chain = 0;
	ಪೂರ್ಣಾಂಕ ret;

	ಸ್ಥಿರ ರಚನೆ option builtin_commit_ನಕ್ಷೆ_verify_options[] = {
		OPT_BOOL(0, "shallow", &opts.shallow,
			 N_("if the commit-graph is split, only verify the tip file")),
		OPT_BOOL(0, "progress", &opts.ಪ್ರಗತಿ,
			 N_("force progress reporting")),
		OPT_ಕೊನೆ(),
	};
	ರಚನೆ option *options = ಕೂಡಿಸು_common_options(builtin_commit_ನಕ್ಷೆ_verify_options);

	trace2_cmd_ಬಹುಲಕ("verify");

	opts.ಪ್ರಗತಿ = isatty(2);
	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ,
			     options,
			     builtin_commit_ನಕ್ಷೆ_verify_usage, 0);
	ಆದರೆ (argc)
		usage_with_options(builtin_commit_ನಕ್ಷೆ_verify_usage, options);

	ಆದರೆ (!opts.obj_dir)
		opts.obj_dir = repo_get_object_directory(the_repository);
	ಆದರೆ (opts.shallow)
		flags |= COMMIT_ನಕ್ಷೆ_VERIFY_SHALLOW;
	ಆದರೆ (opts.ಪ್ರಗತಿ)
		flags |= COMMIT_ನಕ್ಷೆ_WRITE_ಪ್ರಗತಿ;

	dir = odb_ಮೂಲ_files_ಕಂಡುಹಿಡಿ_dir(the_repository->objects, opts.obj_dir);
	ಆದರೆ (!dir)
		die(_("could not find object directory matching %s"), opts.obj_dir);

	ನಕ್ಷೆ_name = get_commit_ನಕ್ಷೆ_filename(dir->abspath);
	chain_name = get_commit_ನಕ್ಷೆ_chain_filename(dir->abspath);
	ಆದರೆ (open_commit_ನಕ್ಷೆ(ನಕ್ಷೆ_name, &fd, &st))
		opened = OPENED_ನಕ್ಷೆ;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (errno != ENOENT)
		die_errno(_("Could not open commit-graph '%s'"), ನಕ್ಷೆ_name);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (open_commit_ನಕ್ಷೆ_chain(chain_name, &fd, &st,
					 the_repository->hash_algo))
		opened = OPENED_CHAIN;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (errno != ENOENT)
		die_errno(_("could not open commit-graph chain '%s'"), chain_name);

	FREE_AND_NULL(ನಕ್ಷೆ_name);
	FREE_AND_NULL(chain_name);
	FREE_AND_NULL(options);

	ಆದರೆ (opened == OPENED_NONE)
		ಹಿಂತಿರುಗಿ 0;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (opened == OPENED_ನಕ್ಷೆ)
		ನಕ್ಷೆ = ತುಂಬು_commit_ನಕ್ಷೆ_one_fd_st(the_repository, dir->abspath, fd, &st);
	ಇಲ್ಲದಿದ್ದರೆ
		ನಕ್ಷೆ = ತುಂಬು_commit_ನಕ್ಷೆ_chain_fd_st(the_repository->objects, fd, &st,
						      &incomplete_chain);

	ಆದರೆ (!ನಕ್ಷೆ)
		ಹಿಂತಿರುಗಿ 1;

	ret = verify_commit_ನಕ್ಷೆ(ನಕ್ಷೆ, flags);
	free_commit_ನಕ್ಷೆ(ನಕ್ಷೆ);

	ಆದರೆ (incomplete_chain) {
		error("one or more commit-graph chain files could not be loaded");
		ret |= 1;
	}

	ಹಿಂತಿರುಗಿ ret;
}

ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ read_replace_refs;
ಸ್ಥಿರ ರಚನೆ commit_ನಕ್ಷೆ_opts write_opts;

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ write_option_parse_ವಿಭಜಿಸು(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg,
				    ಪೂರ್ಣಾಂಕ unset)
{
	ಎಣಿಕೆ commit_ನಕ್ಷೆ_ವಿಭಜಿಸು_flags *flags = opt->ಮೌಲ್ಯ;

	BUG_ON_OPT_NEG(unset);

	opts.ವಿಭಜಿಸು = 1;
	ಆದರೆ (!arg)
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (!strcmp(arg, "no-merge"))
		*flags = COMMIT_ನಕ್ಷೆ_ವಿಭಜಿಸು_ವಿಲೀನ_PROHIBITED;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(arg, "replace"))
		*flags = COMMIT_ನಕ್ಷೆ_ವಿಭಜಿಸು_REPLACE;
	ಇಲ್ಲದಿದ್ದರೆ
		die(_("unrecognized --split argument, %s"), arg);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ read_one_commit(ರಚನೆ oidset *commits, ರಚನೆ ಪ್ರಗತಿ *ಪ್ರಗತಿ,
			   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *hash)
{
	ರಚನೆ object *ಫಲಿತಾಂಶ;
	ರಚನೆ object_id oid;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೊನೆ;

	ಆದರೆ (parse_oid_hex(hash, &oid, &ಕೊನೆ))
		ಹಿಂತಿರುಗಿ error(_("unexpected non-hex object ID: %s"), hash);

	ಫಲಿತಾಂಶ = deref_ಟ್ಯಾಗ್(the_repository, parse_object(the_repository, &oid),
			   ಖಾಲಿ, 0);
	ಆದರೆ (!ಫಲಿತಾಂಶ)
		ಹಿಂತಿರುಗಿ error(_("invalid object: %s"), hash);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (object_as_type(ಫಲಿತಾಂಶ, OBJ_COMMIT, 1))
		oidset_ಸೇರಿಸು(commits, &ಫಲಿತಾಂಶ->oid);

	display_ಪ್ರಗತಿ(ಪ್ರಗತಿ, oidset_ಗಾತ್ರ(commits));

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ write_option_max_new_filters(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt,
					ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg,
					ಪೂರ್ಣಾಂಕ unset)
{
	ಪೂರ್ಣಾಂಕ *ಗಮ್ಯ = opt->ಮೌಲ್ಯ;
	ಆದರೆ (unset)
		*ಗಮ್ಯ = -1;
	ಇಲ್ಲದಿದ್ದರೆ {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s;
		*ಗಮ್ಯ = strtol(arg, (ಅಕ್ಷರ **)&s, 10);
		ಆದರೆ (*s)
			ಹಿಂತಿರುಗಿ error(_("option `%s' expects a numerical value"),
				     "max-new-filters");
	}
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ git_commit_ನಕ್ಷೆ_write_config(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *var, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ,
					 ಸ್ಥಿರಾಂಕ ರಚನೆ config_context *ctx,
					 ಶೂನ್ಯ *cb ಬಳಸದ)
{
	ಆದರೆ (!strcmp(var, "commitgraph.maxnewfilters"))
		write_opts.max_new_filters = git_config_int(var, ಮೌಲ್ಯ, ctx->kvi);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(var, "commitgraph.changedpaths"))
		opts.enable_ಬದಲಾಯಿತು_paths = git_config_bool(var, ಮೌಲ್ಯ) ? 1 : -1;
	/*
	 * No need to fall-back to 'git_default_config', since this was already
	 * called in 'cmd_commit_graph()'.
	 */
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ನಕ್ಷೆ_write(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		       ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ string_list pack_indexes = STRING_LIST_INIT_DUP;
	ರಚನೆ strbuf buf = STRBUF_INIT;
	ರಚನೆ oidset commits = OIDSET_INIT;
	ರಚನೆ odb_files_dir *dir = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ = 0;
	ಎಣಿಕೆ commit_ನಕ್ಷೆ_write_flags flags = 0;
	ರಚನೆ ಪ್ರಗತಿ *ಪ್ರಗತಿ = ಖಾಲಿ;

	ಸ್ಥಿರ ರಚನೆ option builtin_commit_ನಕ್ಷೆ_write_options[] = {
		OPT_BOOL(0, "reachable", &opts.reachable,
			N_("start walk at all refs")),
		OPT_BOOL(0, "stdin-packs", &opts.stdin_packs,
			N_("scan pack-indexes listed by stdin for commits")),
		OPT_BOOL(0, "stdin-commits", &opts.stdin_commits,
			N_("start walk at commits listed by stdin")),
		OPT_BOOL(0, "append", &opts.append,
			N_("include all commits already in the commit-graph file")),
		OPT_BOOL(0, "changed-paths", &opts.enable_ಬದಲಾಯಿತು_paths,
			N_("enable computation for changed paths")),
		OPT_CALLBACK_F(0, "split", &write_opts.ವಿಭಜಿಸು_flags, ಖಾಲಿ,
			N_("allow writing an incremental commit-graph file"),
			PARSE_OPT_OPTARG | PARSE_OPT_NONEG,
			write_option_parse_ವಿಭಜಿಸು),
		OPT_INTEGER(0, "max-commits", &write_opts.max_commits,
			N_("maximum number of commits in a non-base split commit-graph")),
		OPT_INTEGER(0, "size-multiple", &write_opts.ಗಾತ್ರ_ಅಪವರ್ತ್ಯ,
			N_("maximum ratio between two levels of a split commit-graph")),
		OPT_EXPIRY_DATE(0, "expire-time", &write_opts.expire_ಸಮಯ,
			N_("only expire files older than a given date-time")),
		OPT_CALLBACK_F(0, "max-new-filters", &write_opts.max_new_filters,
			ಖಾಲಿ, N_("maximum number of changed-path Bloom filters to compute"),
			0, write_option_max_new_filters),
		OPT_BOOL(0, "progress", &opts.ಪ್ರಗತಿ,
			 N_("force progress reporting")),
		OPT_ಕೊನೆ(),
	};
	ರಚನೆ option *options = ಕೂಡಿಸು_common_options(builtin_commit_ನಕ್ಷೆ_write_options);

	opts.ಪ್ರಗತಿ = isatty(2);
	opts.enable_ಬದಲಾಯಿತು_paths = -1;
	write_opts.ಗಾತ್ರ_ಅಪವರ್ತ್ಯ = 2;
	write_opts.max_commits = 0;
	write_opts.expire_ಸಮಯ = 0;
	write_opts.max_new_filters = -1;

	trace2_cmd_ಬಹುಲಕ("write");

	repo_config(the_repository, git_commit_ನಕ್ಷೆ_write_config, &opts);

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ,
			     options,
			     builtin_commit_ನಕ್ಷೆ_write_usage, 0);
	ಆದರೆ (argc)
		usage_with_options(builtin_commit_ನಕ್ಷೆ_write_usage, options);

	ಆದರೆ (opts.reachable + opts.stdin_packs + opts.stdin_commits > 1)
		die(_("use at most one of --reachable, --stdin-commits, or --stdin-packs"));
	ಆದರೆ (!opts.obj_dir)
		opts.obj_dir = repo_get_object_directory(the_repository);
	ಆದರೆ (opts.append)
		flags |= COMMIT_ನಕ್ಷೆ_WRITE_APPEND;
	ಆದರೆ (opts.ವಿಭಜಿಸು)
		flags |= COMMIT_ನಕ್ಷೆ_WRITE_ವಿಭಜಿಸು;
	ಆದರೆ (opts.ಪ್ರಗತಿ)
		flags |= COMMIT_ನಕ್ಷೆ_WRITE_ಪ್ರಗತಿ;
	ಆದರೆ (!opts.enable_ಬದಲಾಯಿತು_paths)
		flags |= COMMIT_ನಕ್ಷೆ_NO_WRITE_BLOOM_FILTERS;
	ಆದರೆ (opts.enable_ಬದಲಾಯಿತು_paths == 1 ||
	    git_env_bool(GIT_TEST_COMMIT_ನಕ್ಷೆ_ಬದಲಾಯಿತು_PATHS, 0))
		flags |= COMMIT_ನಕ್ಷೆ_WRITE_BLOOM_FILTERS;

	dir = odb_ಮೂಲ_files_ಕಂಡುಹಿಡಿ_dir(the_repository->objects, opts.obj_dir);
	ಆದರೆ (!dir)
		die(_("could not find object directory matching %s"), opts.obj_dir);

	ಆದರೆ (opts.reachable) {
		ಆದರೆ (write_commit_ನಕ್ಷೆ_reachable(the_repository, dir->abspath, flags, &write_opts))
			ಫಲಿತಾಂಶ = 1;
		ಹೋಗು cleanup;
	}

	ಆದರೆ (opts.stdin_packs) {
		ರಚನೆ strbuf packname = STRBUF_INIT;
		ಗಾತ್ರ_t dirlen;

		strbuf_addf(&packname, "%s/pack/", dir->abspath);
		dirlen = packname.len;

		ಆಗ (strbuf_getline(&buf, stdin) != EOF) {
			strbuf_setlen(&packname, dirlen);
			strbuf_addbuf(&packname, &buf);
			string_list_append(&pack_indexes, packname.buf);
		}

		strbuf_release(&packname);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (opts.stdin_commits) {
		oidset_init(&commits, 0);
		ಆದರೆ (opts.ಪ್ರಗತಿ)
			ಪ್ರಗತಿ = ಆರಂಭ_delayed_ಪ್ರಗತಿ(
				the_repository,
				_("Collecting commits from input"), 0);

		ಆಗ (strbuf_getline(&buf, stdin) != EOF) {
			ಆದರೆ (read_one_commit(&commits, ಪ್ರಗತಿ, buf.buf)) {
				ಫಲಿತಾಂಶ = 1;
				stop_ಪ್ರಗತಿ(&ಪ್ರಗತಿ);
				ಹೋಗು cleanup;
			}
		}

		stop_ಪ್ರಗತಿ(&ಪ್ರಗತಿ);
	}

	ಆದರೆ (write_commit_ನಕ್ಷೆ(the_repository, dir->abspath,
			       opts.stdin_packs ? &pack_indexes : ಖಾಲಿ,
			       opts.stdin_commits ? &commits : ಖಾಲಿ,
			       flags,
			       &write_opts))
		ಫಲಿತಾಂಶ = 1;

cleanup:
	FREE_AND_NULL(options);
	string_list_clear(&pack_indexes, 0);
	strbuf_release(&buf);
	oidset_clear(&commits);
	ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

ಪೂರ್ಣಾಂಕ cmd_commit_ನಕ್ಷೆ(ಪೂರ್ಣಾಂಕ argc,
		     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
		     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		     ರಚನೆ repository *repo)
{
	parse_opt_subcommand_fn *fn = ಖಾಲಿ;
	ರಚನೆ option builtin_commit_ನಕ್ಷೆ_options[] = {
		OPT_SUBCOMMAND("verify", &fn, ನಕ್ಷೆ_verify),
		OPT_SUBCOMMAND("write", &fn, ನಕ್ಷೆ_write),
		OPT_ಕೊನೆ(),
	};
	ರಚನೆ option *options = parse_options_concat(builtin_commit_ನಕ್ಷೆ_options, common_opts);

	repo_config(the_repository, git_default_config, ಖಾಲಿ);

	disable_replace_refs();
	save_commit_ಬಫರ್ = 0;

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options,
			     builtin_commit_ನಕ್ಷೆ_usage, 0);
	FREE_AND_NULL(options);

	ಹಿಂತಿರುಗಿ fn(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, repo);
}
