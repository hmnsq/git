#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
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

#تعریف_کرو BUILTIN_COMMIT_گراف_VERIFY_USAGE \
	N_("git commit-graph verify [--object-dir <dir>] [--shallow] [--[no-]progress]")

#تعریف_کرو BUILTIN_COMMIT_گراف_WRITE_USAGE \
	N_("git commit-graph write [--object-dir <dir>] [--append]\n" \
	   "                       [--split[=<strategy>]] [--reachable | --stdin-packs | --stdin-commits]\n" \
	   "                       [--changed-paths] [--[no-]max-new-filters <n>] [--[no-]progress]\n" \
	   "                       <split-options>")

ساکن ثابت حرف * ثابت builtin_commit_گراف_verify_usage[] = {
	BUILTIN_COMMIT_گراف_VERIFY_USAGE,
	تہی
};

ساکن ثابت حرف * ثابت builtin_commit_گراف_write_usage[] = {
	BUILTIN_COMMIT_گراف_WRITE_USAGE,
	تہی
};

ساکن حرف ثابت * ثابت builtin_commit_گراف_usage[] = {
	BUILTIN_COMMIT_گراف_VERIFY_USAGE,
	BUILTIN_COMMIT_گراف_WRITE_USAGE,
	تہی,
};

ساکن ساخت opts_commit_گراف {
	ثابت حرف *obj_dir;
	صحیح reachable;
	صحیح stdin_packs;
	صحیح stdin_commits;
	صحیح append;
	صحیح بانٹو;
	صحیح shallow;
	صحیح پیش_رفت;
	صحیح enable_بدلا_paths;
} opts;

ساکن ساخت option common_opts[] = {
	OPT_STRING(0, "object-dir", &opts.obj_dir,
		   N_("dir"),
		   N_("the object directory to store the graph")),
	OPT_انتہا()
};

ساکن ساخت option *جوڑو_common_options(ساخت option *مقصد)
{
	واپس parse_options_concat(common_opts, مقصد);
}

ساکن صحیح گراف_verify(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
			ساخت repository *repo غیر_مستعمل)
{
	ساخت commit_گراف *گراف = تہی;
	ساخت odb_files_dir *dir = تہی;
	حرف *گراف_name;
	حرف *chain_name;
	شمار { OPENED_NONE, OPENED_گراف, OPENED_CHAIN } opened = OPENED_NONE;
	صحیح fd;
	ساخت stat st;
	صحیح flags = 0;
	صحیح incomplete_chain = 0;
	صحیح ret;

	ساکن ساخت option builtin_commit_گراف_verify_options[] = {
		OPT_BOOL(0, "shallow", &opts.shallow,
			 N_("if the commit-graph is split, only verify the tip file")),
		OPT_BOOL(0, "progress", &opts.پیش_رفت,
			 N_("force progress reporting")),
		OPT_انتہا(),
	};
	ساخت option *options = جوڑو_common_options(builtin_commit_گراف_verify_options);

	trace2_cmd_منوال("verify");

	opts.پیش_رفت = isatty(2);
	argc = parse_options(argc, argv, سابقہ,
			     options,
			     builtin_commit_گراف_verify_usage, 0);
	اگر (argc)
		usage_with_options(builtin_commit_گراف_verify_usage, options);

	اگر (!opts.obj_dir)
		opts.obj_dir = repo_get_object_directory(the_repository);
	اگر (opts.shallow)
		flags |= COMMIT_گراف_VERIFY_SHALLOW;
	اگر (opts.پیش_رفت)
		flags |= COMMIT_گراف_WRITE_پیش_رفت;

	dir = odb_ماخذ_files_ڈھونڈو_dir(the_repository->objects, opts.obj_dir);
	اگر (!dir)
		die(_("could not find object directory matching %s"), opts.obj_dir);

	گراف_name = get_commit_گراف_filename(dir->abspath);
	chain_name = get_commit_گراف_chain_filename(dir->abspath);
	اگر (open_commit_گراف(گراف_name, &fd, &st))
		opened = OPENED_گراف;
	ورنہ اگر (errno != ENOENT)
		die_errno(_("Could not open commit-graph '%s'"), گراف_name);
	ورنہ اگر (open_commit_گراف_chain(chain_name, &fd, &st,
					 the_repository->hash_algo))
		opened = OPENED_CHAIN;
	ورنہ اگر (errno != ENOENT)
		die_errno(_("could not open commit-graph chain '%s'"), chain_name);

	FREE_AND_NULL(گراف_name);
	FREE_AND_NULL(chain_name);
	FREE_AND_NULL(options);

	اگر (opened == OPENED_NONE)
		واپس 0;
	ورنہ اگر (opened == OPENED_گراف)
		گراف = بھرو_commit_گراف_one_fd_st(the_repository, dir->abspath, fd, &st);
	ورنہ
		گراف = بھرو_commit_گراف_chain_fd_st(the_repository->objects, fd, &st,
						      &incomplete_chain);

	اگر (!گراف)
		واپس 1;

	ret = verify_commit_گراف(گراف, flags);
	free_commit_گراف(گراف);

	اگر (incomplete_chain) {
		error("one or more commit-graph chain files could not be loaded");
		ret |= 1;
	}

	واپس ret;
}

خارجی صحیح read_replace_refs;
ساکن ساخت commit_گراف_opts write_opts;

ساکن صحیح write_option_parse_بانٹو(ثابت ساخت option *opt, ثابت حرف *arg,
				    صحیح unset)
{
	شمار commit_گراف_بانٹو_flags *flags = opt->قدر;

	BUG_ON_OPT_NEG(unset);

	opts.بانٹو = 1;
	اگر (!arg)
		واپس 0;

	اگر (!strcmp(arg, "no-merge"))
		*flags = COMMIT_گراف_بانٹو_ملاؤ_PROHIBITED;
	ورنہ اگر (!strcmp(arg, "replace"))
		*flags = COMMIT_گراف_بانٹو_REPLACE;
	ورنہ
		die(_("unrecognized --split argument, %s"), arg);

	واپس 0;
}

ساکن صحیح read_one_commit(ساخت oidset *commits, ساخت پیش_رفت *پیش_رفت,
			   ثابت حرف *hash)
{
	ساخت object *نتیجہ;
	ساخت object_id oid;
	ثابت حرف *انتہا;

	اگر (parse_oid_hex(hash, &oid, &انتہا))
		واپس error(_("unexpected non-hex object ID: %s"), hash);

	نتیجہ = deref_لیبل(the_repository, parse_object(the_repository, &oid),
			   تہی, 0);
	اگر (!نتیجہ)
		واپس error(_("invalid object: %s"), hash);
	ورنہ اگر (object_as_type(نتیجہ, OBJ_COMMIT, 1))
		oidset_داخل_کرو(commits, &نتیجہ->oid);

	display_پیش_رفت(پیش_رفت, oidset_سائز(commits));

	واپس 0;
}

ساکن صحیح write_option_max_new_filters(ثابت ساخت option *opt,
					ثابت حرف *arg,
					صحیح unset)
{
	صحیح *مقصد = opt->قدر;
	اگر (unset)
		*مقصد = -1;
	ورنہ {
		ثابت حرف *s;
		*مقصد = strtol(arg, (حرف **)&s, 10);
		اگر (*s)
			واپس error(_("option `%s' expects a numerical value"),
				     "max-new-filters");
	}
	واپس 0;
}

ساکن صحیح git_commit_گراف_write_config(ثابت حرف *var, ثابت حرف *قدر,
					 ثابت ساخت config_context *ctx,
					 خالی *cb غیر_مستعمل)
{
	اگر (!strcmp(var, "commitgraph.maxnewfilters"))
		write_opts.max_new_filters = git_config_int(var, قدر, ctx->kvi);
	ورنہ اگر (!strcmp(var, "commitgraph.changedpaths"))
		opts.enable_بدلا_paths = git_config_bool(var, قدر) ? 1 : -1;
	/*
	 * No need to fall-back to 'git_default_config', since this was already
	 * called in 'cmd_commit_graph()'.
	 */
	واپس 0;
}

ساکن صحیح گراف_write(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
		       ساخت repository *repo غیر_مستعمل)
{
	ساخت string_list pack_indexes = STRING_LIST_INIT_DUP;
	ساخت strbuf buf = STRBUF_INIT;
	ساخت oidset commits = OIDSET_INIT;
	ساخت odb_files_dir *dir = تہی;
	صحیح نتیجہ = 0;
	شمار commit_گراف_write_flags flags = 0;
	ساخت پیش_رفت *پیش_رفت = تہی;

	ساکن ساخت option builtin_commit_گراف_write_options[] = {
		OPT_BOOL(0, "reachable", &opts.reachable,
			N_("start walk at all refs")),
		OPT_BOOL(0, "stdin-packs", &opts.stdin_packs,
			N_("scan pack-indexes listed by stdin for commits")),
		OPT_BOOL(0, "stdin-commits", &opts.stdin_commits,
			N_("start walk at commits listed by stdin")),
		OPT_BOOL(0, "append", &opts.append,
			N_("include all commits already in the commit-graph file")),
		OPT_BOOL(0, "changed-paths", &opts.enable_بدلا_paths,
			N_("enable computation for changed paths")),
		OPT_CALLBACK_F(0, "split", &write_opts.بانٹو_flags, تہی,
			N_("allow writing an incremental commit-graph file"),
			PARSE_OPT_OPTARG | PARSE_OPT_NONEG,
			write_option_parse_بانٹو),
		OPT_INTEGER(0, "max-commits", &write_opts.max_commits,
			N_("maximum number of commits in a non-base split commit-graph")),
		OPT_INTEGER(0, "size-multiple", &write_opts.سائز_مضرب,
			N_("maximum ratio between two levels of a split commit-graph")),
		OPT_EXPIRY_DATE(0, "expire-time", &write_opts.expire_وقت,
			N_("only expire files older than a given date-time")),
		OPT_CALLBACK_F(0, "max-new-filters", &write_opts.max_new_filters,
			تہی, N_("maximum number of changed-path Bloom filters to compute"),
			0, write_option_max_new_filters),
		OPT_BOOL(0, "progress", &opts.پیش_رفت,
			 N_("force progress reporting")),
		OPT_انتہا(),
	};
	ساخت option *options = جوڑو_common_options(builtin_commit_گراف_write_options);

	opts.پیش_رفت = isatty(2);
	opts.enable_بدلا_paths = -1;
	write_opts.سائز_مضرب = 2;
	write_opts.max_commits = 0;
	write_opts.expire_وقت = 0;
	write_opts.max_new_filters = -1;

	trace2_cmd_منوال("write");

	repo_config(the_repository, git_commit_گراف_write_config, &opts);

	argc = parse_options(argc, argv, سابقہ,
			     options,
			     builtin_commit_گراف_write_usage, 0);
	اگر (argc)
		usage_with_options(builtin_commit_گراف_write_usage, options);

	اگر (opts.reachable + opts.stdin_packs + opts.stdin_commits > 1)
		die(_("use at most one of --reachable, --stdin-commits, or --stdin-packs"));
	اگر (!opts.obj_dir)
		opts.obj_dir = repo_get_object_directory(the_repository);
	اگر (opts.append)
		flags |= COMMIT_گراف_WRITE_APPEND;
	اگر (opts.بانٹو)
		flags |= COMMIT_گراف_WRITE_بانٹو;
	اگر (opts.پیش_رفت)
		flags |= COMMIT_گراف_WRITE_پیش_رفت;
	اگر (!opts.enable_بدلا_paths)
		flags |= COMMIT_گراف_NO_WRITE_BLOOM_FILTERS;
	اگر (opts.enable_بدلا_paths == 1 ||
	    git_env_bool(GIT_TEST_COMMIT_گراف_بدلا_PATHS, 0))
		flags |= COMMIT_گراف_WRITE_BLOOM_FILTERS;

	dir = odb_ماخذ_files_ڈھونڈو_dir(the_repository->objects, opts.obj_dir);
	اگر (!dir)
		die(_("could not find object directory matching %s"), opts.obj_dir);

	اگر (opts.reachable) {
		اگر (write_commit_گراف_reachable(the_repository, dir->abspath, flags, &write_opts))
			نتیجہ = 1;
		جاؤ cleanup;
	}

	اگر (opts.stdin_packs) {
		ساخت strbuf packname = STRBUF_INIT;
		سائز_t dirlen;

		strbuf_addf(&packname, "%s/pack/", dir->abspath);
		dirlen = packname.len;

		جب (strbuf_getline(&buf, stdin) != EOF) {
			strbuf_setlen(&packname, dirlen);
			strbuf_addbuf(&packname, &buf);
			string_list_append(&pack_indexes, packname.buf);
		}

		strbuf_release(&packname);
	} ورنہ اگر (opts.stdin_commits) {
		oidset_init(&commits, 0);
		اگر (opts.پیش_رفت)
			پیش_رفت = آغاز_delayed_پیش_رفت(
				the_repository,
				_("Collecting commits from input"), 0);

		جب (strbuf_getline(&buf, stdin) != EOF) {
			اگر (read_one_commit(&commits, پیش_رفت, buf.buf)) {
				نتیجہ = 1;
				stop_پیش_رفت(&پیش_رفت);
				جاؤ cleanup;
			}
		}

		stop_پیش_رفت(&پیش_رفت);
	}

	اگر (write_commit_گراف(the_repository, dir->abspath,
			       opts.stdin_packs ? &pack_indexes : تہی,
			       opts.stdin_commits ? &commits : تہی,
			       flags,
			       &write_opts))
		نتیجہ = 1;

cleanup:
	FREE_AND_NULL(options);
	string_list_clear(&pack_indexes, 0);
	strbuf_release(&buf);
	oidset_clear(&commits);
	واپس نتیجہ;
}

صحیح cmd_commit_گراف(صحیح argc,
		     ثابت حرف **argv,
		     ثابت حرف *سابقہ,
		     ساخت repository *repo)
{
	parse_opt_subcommand_fn *fn = تہی;
	ساخت option builtin_commit_گراف_options[] = {
		OPT_SUBCOMMAND("verify", &fn, گراف_verify),
		OPT_SUBCOMMAND("write", &fn, گراف_write),
		OPT_انتہا(),
	};
	ساخت option *options = parse_options_concat(builtin_commit_گراف_options, common_opts);

	repo_config(the_repository, git_default_config, تہی);

	disable_replace_refs();
	save_commit_بفر = 0;

	argc = parse_options(argc, argv, سابقہ, options,
			     builtin_commit_گراف_usage, 0);
	FREE_AND_NULL(options);

	واپس fn(argc, argv, سابقہ, repo);
}
