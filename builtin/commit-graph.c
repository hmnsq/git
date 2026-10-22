#تعریف_کن USE_THE_REPOSITORY_VARIABLE
#ضمن "builtin.h"
#ضمن "commit.h"
#ضمن "config.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "hex.h"
#ضمن "parse-options.h"
#ضمن "commit-graph.h"
#ضمن "odb.h"
#ضمن "odb/source.h"
#ضمن "odb/source-files.h"
#ضمن "progress.h"
#ضمن "replace-object.h"
#ضمن "strbuf.h"
#ضمن "tag.h"
#ضمن "trace2.h"

#تعریف_کن BUILTIN_COMMIT_گراف_VERIFY_USAGE \
	N_("git commit-graph verify [--object-dir <dir>] [--shallow] [--[no-]progress]")

#تعریف_کن BUILTIN_COMMIT_گراف_WRITE_USAGE \
	N_("git commit-graph write [--object-dir <dir>] [--append]\n" \
	   "                       [--split[=<strategy>]] [--reachable | --stdin-packs | --stdin-commits]\n" \
	   "                       [--changed-paths] [--[no-]max-new-filters <n>] [--[no-]progress]\n" \
	   "                       <split-options>")

ساكن ثابت حرف * ثابت builtin_commit_گراف_verify_usage[] = {
	BUILTIN_COMMIT_گراف_VERIFY_USAGE,
	عدم
};

ساكن ثابت حرف * ثابت builtin_commit_گراف_write_usage[] = {
	BUILTIN_COMMIT_گراف_WRITE_USAGE,
	عدم
};

ساكن حرف ثابت * ثابت builtin_commit_گراف_usage[] = {
	BUILTIN_COMMIT_گراف_VERIFY_USAGE,
	BUILTIN_COMMIT_گراف_WRITE_USAGE,
	عدم,
};

ساكن بنية opts_commit_گراف {
	ثابت حرف *obj_dir;
	صحيح reachable;
	صحيح stdin_packs;
	صحيح stdin_commits;
	صحيح append;
	صحيح تقسیم;
	صحيح shallow;
	صحيح پیشرفت;
	صحيح enable_تغییریافته_paths;
} opts;

ساكن بنية option common_opts[] = {
	OPT_STRING(0, "object-dir", &opts.obj_dir,
		   N_("dir"),
		   N_("the object directory to store the graph")),
	OPT_انتها()
};

ساكن بنية option *جمع_کن_common_options(بنية option *مقصد)
{
	أرجع parse_options_concat(common_opts, مقصد);
}

ساكن صحيح گراف_verify(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
			بنية repository *repo استفادهنشده)
{
	بنية commit_گراف *گراف = عدم;
	بنية odb_files_dir *dir = عدم;
	حرف *گراف_name;
	حرف *chain_name;
	تعداد { OPENED_NONE, OPENED_گراف, OPENED_CHAIN } opened = OPENED_NONE;
	صحيح fd;
	بنية stat st;
	صحيح flags = 0;
	صحيح incomplete_chain = 0;
	صحيح ret;

	ساكن بنية option builtin_commit_گراف_verify_options[] = {
		OPT_BOOL(0, "shallow", &opts.shallow,
			 N_("if the commit-graph is split, only verify the tip file")),
		OPT_BOOL(0, "progress", &opts.پیشرفت,
			 N_("force progress reporting")),
		OPT_انتها(),
	};
	بنية option *options = جمع_کن_common_options(builtin_commit_گراف_verify_options);

	trace2_cmd_نما("verify");

	opts.پیشرفت = isatty(2);
	argc = parse_options(argc, argv, پیشوند,
			     options,
			     builtin_commit_گراف_verify_usage, 0);
	إذا (argc)
		usage_with_options(builtin_commit_گراف_verify_usage, options);

	إذا (!opts.obj_dir)
		opts.obj_dir = repo_get_object_directory(the_repository);
	إذا (opts.shallow)
		flags |= COMMIT_گراف_VERIFY_SHALLOW;
	إذا (opts.پیشرفت)
		flags |= COMMIT_گراف_WRITE_پیشرفت;

	dir = odb_منبع_files_پیدا_کن_dir(the_repository->objects, opts.obj_dir);
	إذا (!dir)
		die(_("could not find object directory matching %s"), opts.obj_dir);

	گراف_name = get_commit_گراف_filename(dir->abspath);
	chain_name = get_commit_گراف_chain_filename(dir->abspath);
	إذا (open_commit_گراف(گراف_name, &fd, &st))
		opened = OPENED_گراف;
	وإلا إذا (errno != ENOENT)
		die_errno(_("Could not open commit-graph '%s'"), گراف_name);
	وإلا إذا (open_commit_گراف_chain(chain_name, &fd, &st,
					 the_repository->hash_algo))
		opened = OPENED_CHAIN;
	وإلا إذا (errno != ENOENT)
		die_errno(_("could not open commit-graph chain '%s'"), chain_name);

	FREE_AND_NULL(گراف_name);
	FREE_AND_NULL(chain_name);
	FREE_AND_NULL(options);

	إذا (opened == OPENED_NONE)
		أرجع 0;
	وإلا إذا (opened == OPENED_گراف)
		گراف = بارگیری_commit_گراف_one_fd_st(the_repository, dir->abspath, fd, &st);
	وإلا
		گراف = بارگیری_commit_گراف_chain_fd_st(the_repository->objects, fd, &st,
						      &incomplete_chain);

	إذا (!گراف)
		أرجع 1;

	ret = verify_commit_گراف(گراف, flags);
	free_commit_گراف(گراف);

	إذا (incomplete_chain) {
		error("one or more commit-graph chain files could not be loaded");
		ret |= 1;
	}

	أرجع ret;
}

خارجي صحيح read_replace_refs;
ساكن بنية commit_گراف_opts write_opts;

ساكن صحيح write_option_parse_تقسیم(ثابت بنية option *opt, ثابت حرف *arg,
				    صحيح unset)
{
	تعداد commit_گراف_تقسیم_flags *flags = opt->مقدار;

	BUG_ON_OPT_NEG(unset);

	opts.تقسیم = 1;
	إذا (!arg)
		أرجع 0;

	إذا (!strcmp(arg, "no-merge"))
		*flags = COMMIT_گراف_تقسیم_ادغام_PROHIBITED;
	وإلا إذا (!strcmp(arg, "replace"))
		*flags = COMMIT_گراف_تقسیم_REPLACE;
	وإلا
		die(_("unrecognized --split argument, %s"), arg);

	أرجع 0;
}

ساكن صحيح read_one_commit(بنية oidset *commits, بنية پیشرفت *پیشرفت,
			   ثابت حرف *hash)
{
	بنية object *نتیجه;
	بنية object_id oid;
	ثابت حرف *انتها;

	إذا (parse_oid_hex(hash, &oid, &انتها))
		أرجع error(_("unexpected non-hex object ID: %s"), hash);

	نتیجه = deref_برچسب(the_repository, parse_object(the_repository, &oid),
			   عدم, 0);
	إذا (!نتیجه)
		أرجع error(_("invalid object: %s"), hash);
	وإلا إذا (object_as_type(نتیجه, OBJ_COMMIT, 1))
		oidset_درج(commits, &نتیجه->oid);

	display_پیشرفت(پیشرفت, oidset_اندازه(commits));

	أرجع 0;
}

ساكن صحيح write_option_max_new_filters(ثابت بنية option *opt,
					ثابت حرف *arg,
					صحيح unset)
{
	صحيح *مقصد = opt->مقدار;
	إذا (unset)
		*مقصد = -1;
	وإلا {
		ثابت حرف *s;
		*مقصد = strtol(arg, (حرف **)&s, 10);
		إذا (*s)
			أرجع error(_("option `%s' expects a numerical value"),
				     "max-new-filters");
	}
	أرجع 0;
}

ساكن صحيح git_commit_گراف_write_config(ثابت حرف *var, ثابت حرف *مقدار,
					 ثابت بنية config_context *ctx,
					 فراغ *cb استفادهنشده)
{
	إذا (!strcmp(var, "commitgraph.maxnewfilters"))
		write_opts.max_new_filters = git_config_int(var, مقدار, ctx->kvi);
	وإلا إذا (!strcmp(var, "commitgraph.changedpaths"))
		opts.enable_تغییریافته_paths = git_config_bool(var, مقدار) ? 1 : -1;
	/*
	 * No need to fall-back to 'git_default_config', since this was already
	 * called in 'cmd_commit_graph()'.
	 */
	أرجع 0;
}

ساكن صحيح گراف_write(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
		       بنية repository *repo استفادهنشده)
{
	بنية string_list pack_indexes = STRING_LIST_INIT_DUP;
	بنية strbuf buf = STRBUF_INIT;
	بنية oidset commits = OIDSET_INIT;
	بنية odb_files_dir *dir = عدم;
	صحيح نتیجه = 0;
	تعداد commit_گراف_write_flags flags = 0;
	بنية پیشرفت *پیشرفت = عدم;

	ساكن بنية option builtin_commit_گراف_write_options[] = {
		OPT_BOOL(0, "reachable", &opts.reachable,
			N_("start walk at all refs")),
		OPT_BOOL(0, "stdin-packs", &opts.stdin_packs,
			N_("scan pack-indexes listed by stdin for commits")),
		OPT_BOOL(0, "stdin-commits", &opts.stdin_commits,
			N_("start walk at commits listed by stdin")),
		OPT_BOOL(0, "append", &opts.append,
			N_("include all commits already in the commit-graph file")),
		OPT_BOOL(0, "changed-paths", &opts.enable_تغییریافته_paths,
			N_("enable computation for changed paths")),
		OPT_CALLBACK_F(0, "split", &write_opts.تقسیم_flags, عدم,
			N_("allow writing an incremental commit-graph file"),
			PARSE_OPT_OPTARG | PARSE_OPT_NONEG,
			write_option_parse_تقسیم),
		OPT_INTEGER(0, "max-commits", &write_opts.max_commits,
			N_("maximum number of commits in a non-base split commit-graph")),
		OPT_INTEGER(0, "size-multiple", &write_opts.اندازه_مضرب,
			N_("maximum ratio between two levels of a split commit-graph")),
		OPT_EXPIRY_DATE(0, "expire-time", &write_opts.expire_زمان,
			N_("only expire files older than a given date-time")),
		OPT_CALLBACK_F(0, "max-new-filters", &write_opts.max_new_filters,
			عدم, N_("maximum number of changed-path Bloom filters to compute"),
			0, write_option_max_new_filters),
		OPT_BOOL(0, "progress", &opts.پیشرفت,
			 N_("force progress reporting")),
		OPT_انتها(),
	};
	بنية option *options = جمع_کن_common_options(builtin_commit_گراف_write_options);

	opts.پیشرفت = isatty(2);
	opts.enable_تغییریافته_paths = -1;
	write_opts.اندازه_مضرب = 2;
	write_opts.max_commits = 0;
	write_opts.expire_زمان = 0;
	write_opts.max_new_filters = -1;

	trace2_cmd_نما("write");

	repo_config(the_repository, git_commit_گراف_write_config, &opts);

	argc = parse_options(argc, argv, پیشوند,
			     options,
			     builtin_commit_گراف_write_usage, 0);
	إذا (argc)
		usage_with_options(builtin_commit_گراف_write_usage, options);

	إذا (opts.reachable + opts.stdin_packs + opts.stdin_commits > 1)
		die(_("use at most one of --reachable, --stdin-commits, or --stdin-packs"));
	إذا (!opts.obj_dir)
		opts.obj_dir = repo_get_object_directory(the_repository);
	إذا (opts.append)
		flags |= COMMIT_گراف_WRITE_APPEND;
	إذا (opts.تقسیم)
		flags |= COMMIT_گراف_WRITE_تقسیم;
	إذا (opts.پیشرفت)
		flags |= COMMIT_گراف_WRITE_پیشرفت;
	إذا (!opts.enable_تغییریافته_paths)
		flags |= COMMIT_گراف_NO_WRITE_BLOOM_FILTERS;
	إذا (opts.enable_تغییریافته_paths == 1 ||
	    git_env_bool(GIT_TEST_COMMIT_گراف_تغییریافته_PATHS, 0))
		flags |= COMMIT_گراف_WRITE_BLOOM_FILTERS;

	dir = odb_منبع_files_پیدا_کن_dir(the_repository->objects, opts.obj_dir);
	إذا (!dir)
		die(_("could not find object directory matching %s"), opts.obj_dir);

	إذا (opts.reachable) {
		إذا (write_commit_گراف_reachable(the_repository, dir->abspath, flags, &write_opts))
			نتیجه = 1;
		اذهب cleanup;
	}

	إذا (opts.stdin_packs) {
		بنية strbuf packname = STRBUF_INIT;
		اندازه_t dirlen;

		strbuf_addf(&packname, "%s/pack/", dir->abspath);
		dirlen = packname.len;

		طالما (strbuf_getline(&buf, stdin) != EOF) {
			strbuf_setlen(&packname, dirlen);
			strbuf_addbuf(&packname, &buf);
			string_list_append(&pack_indexes, packname.buf);
		}

		strbuf_release(&packname);
	} وإلا إذا (opts.stdin_commits) {
		oidset_init(&commits, 0);
		إذا (opts.پیشرفت)
			پیشرفت = شروع_delayed_پیشرفت(
				the_repository,
				_("Collecting commits from input"), 0);

		طالما (strbuf_getline(&buf, stdin) != EOF) {
			إذا (read_one_commit(&commits, پیشرفت, buf.buf)) {
				نتیجه = 1;
				stop_پیشرفت(&پیشرفت);
				اذهب cleanup;
			}
		}

		stop_پیشرفت(&پیشرفت);
	}

	إذا (write_commit_گراف(the_repository, dir->abspath,
			       opts.stdin_packs ? &pack_indexes : عدم,
			       opts.stdin_commits ? &commits : عدم,
			       flags,
			       &write_opts))
		نتیجه = 1;

cleanup:
	FREE_AND_NULL(options);
	string_list_clear(&pack_indexes, 0);
	strbuf_release(&buf);
	oidset_clear(&commits);
	أرجع نتیجه;
}

صحيح cmd_commit_گراف(صحيح argc,
		     ثابت حرف **argv,
		     ثابت حرف *پیشوند,
		     بنية repository *repo)
{
	parse_opt_subcommand_fn *fn = عدم;
	بنية option builtin_commit_گراف_options[] = {
		OPT_SUBCOMMAND("verify", &fn, گراف_verify),
		OPT_SUBCOMMAND("write", &fn, گراف_write),
		OPT_انتها(),
	};
	بنية option *options = parse_options_concat(builtin_commit_گراف_options, common_opts);

	repo_config(the_repository, git_default_config, عدم);

	disable_replace_refs();
	save_commit_میانگیر = 0;

	argc = parse_options(argc, argv, پیشوند, options,
			     builtin_commit_گراف_usage, 0);
	FREE_AND_NULL(options);

	أرجع fn(argc, argv, پیشوند, repo);
}
