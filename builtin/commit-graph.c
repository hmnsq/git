#تعریف_کړه USE_THE_REPOSITORY_VARIABLE
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

#تعریف_کړه BUILTIN_COMMIT_ګراف_VERIFY_USAGE \
	N_("git commit-graph verify [--object-dir <dir>] [--shallow] [--[no-]progress]")

#تعریف_کړه BUILTIN_COMMIT_ګراف_WRITE_USAGE \
	N_("git commit-graph write [--object-dir <dir>] [--append]\n" \
	   "                       [--split[=<strategy>]] [--reachable | --stdin-packs | --stdin-commits]\n" \
	   "                       [--changed-paths] [--[no-]max-new-filters <n>] [--[no-]progress]\n" \
	   "                       <split-options>")

ساكن ثابت حرف * ثابت builtin_commit_ګراف_verify_usage[] = {
	BUILTIN_COMMIT_ګراف_VERIFY_USAGE,
	عدم
};

ساكن ثابت حرف * ثابت builtin_commit_ګراف_write_usage[] = {
	BUILTIN_COMMIT_ګراف_WRITE_USAGE,
	عدم
};

ساكن حرف ثابت * ثابت builtin_commit_ګراف_usage[] = {
	BUILTIN_COMMIT_ګراف_VERIFY_USAGE,
	BUILTIN_COMMIT_ګراف_WRITE_USAGE,
	عدم,
};

ساكن بنية opts_commit_ګراف {
	ثابت حرف *obj_dir;
	صحيح reachable;
	صحيح stdin_packs;
	صحيح stdin_commits;
	صحيح append;
	صحيح وېشل;
	صحيح shallow;
	صحيح پرمختګ;
	صحيح enable_بدل_شوی_paths;
} opts;

ساكن بنية option common_opts[] = {
	OPT_STRING(0, "object-dir", &opts.obj_dir,
		   N_("dir"),
		   N_("the object directory to store the graph")),
	OPT_پای_ټکی()
};

ساكن بنية option *جمع_کړه_common_options(بنية option *منزل)
{
	أرجع parse_options_concat(common_opts, منزل);
}

ساكن صحيح ګراف_verify(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی,
			بنية repository *repo ناکارول_شوی)
{
	بنية commit_ګراف *ګراف = عدم;
	بنية odb_files_dir *dir = عدم;
	حرف *ګراف_name;
	حرف *chain_name;
	تعداد { OPENED_NONE, OPENED_ګراف, OPENED_CHAIN } opened = OPENED_NONE;
	صحيح fd;
	بنية stat st;
	صحيح flags = 0;
	صحيح incomplete_chain = 0;
	صحيح ret;

	ساكن بنية option builtin_commit_ګراف_verify_options[] = {
		OPT_BOOL(0, "shallow", &opts.shallow,
			 N_("if the commit-graph is split, only verify the tip file")),
		OPT_BOOL(0, "progress", &opts.پرمختګ,
			 N_("force progress reporting")),
		OPT_پای_ټکی(),
	};
	بنية option *options = جمع_کړه_common_options(builtin_commit_ګراف_verify_options);

	trace2_cmd_ډېرپېښ("verify");

	opts.پرمختګ = isatty(2);
	argc = parse_options(argc, argv, مختاړی,
			     options,
			     builtin_commit_ګراف_verify_usage, 0);
	إذا (argc)
		usage_with_options(builtin_commit_ګراف_verify_usage, options);

	إذا (!opts.obj_dir)
		opts.obj_dir = repo_get_object_directory(the_repository);
	إذا (opts.shallow)
		flags |= COMMIT_ګراف_VERIFY_SHALLOW;
	إذا (opts.پرمختګ)
		flags |= COMMIT_ګراف_WRITE_پرمختګ;

	dir = odb_سرچینه_files_پیدا_کړه_dir(the_repository->objects, opts.obj_dir);
	إذا (!dir)
		die(_("could not find object directory matching %s"), opts.obj_dir);

	ګراف_name = get_commit_ګراف_filename(dir->abspath);
	chain_name = get_commit_ګراف_chain_filename(dir->abspath);
	إذا (open_commit_ګراف(ګراف_name, &fd, &st))
		opened = OPENED_ګراف;
	وإلا إذا (errno != ENOENT)
		die_errno(_("Could not open commit-graph '%s'"), ګراف_name);
	وإلا إذا (open_commit_ګراف_chain(chain_name, &fd, &st,
					 the_repository->hash_algo))
		opened = OPENED_CHAIN;
	وإلا إذا (errno != ENOENT)
		die_errno(_("could not open commit-graph chain '%s'"), chain_name);

	FREE_AND_NULL(ګراف_name);
	FREE_AND_NULL(chain_name);
	FREE_AND_NULL(options);

	إذا (opened == OPENED_NONE)
		أرجع 0;
	وإلا إذا (opened == OPENED_ګراف)
		ګراف = بارول_commit_ګراف_one_fd_st(the_repository, dir->abspath, fd, &st);
	وإلا
		ګراف = بارول_commit_ګراف_chain_fd_st(the_repository->objects, fd, &st,
						      &incomplete_chain);

	إذا (!ګراف)
		أرجع 1;

	ret = verify_commit_ګراف(ګراف, flags);
	free_commit_ګراف(ګراف);

	إذا (incomplete_chain) {
		error("one or more commit-graph chain files could not be loaded");
		ret |= 1;
	}

	أرجع ret;
}

خارجي صحيح read_replace_refs;
ساكن بنية commit_ګراف_opts write_opts;

ساكن صحيح write_option_parse_وېشل(ثابت بنية option *opt, ثابت حرف *arg,
				    صحيح unset)
{
	تعداد commit_ګراف_وېشل_flags *flags = opt->ارزښت;

	BUG_ON_OPT_NEG(unset);

	opts.وېشل = 1;
	إذا (!arg)
		أرجع 0;

	إذا (!strcmp(arg, "no-merge"))
		*flags = COMMIT_ګراف_وېشل_یوځای_PROHIBITED;
	وإلا إذا (!strcmp(arg, "replace"))
		*flags = COMMIT_ګراف_وېشل_REPLACE;
	وإلا
		die(_("unrecognized --split argument, %s"), arg);

	أرجع 0;
}

ساكن صحيح read_one_commit(بنية oidset *commits, بنية پرمختګ *پرمختګ,
			   ثابت حرف *hash)
{
	بنية object *پایله;
	بنية object_id oid;
	ثابت حرف *پای_ټکی;

	إذا (parse_oid_hex(hash, &oid, &پای_ټکی))
		أرجع error(_("unexpected non-hex object ID: %s"), hash);

	پایله = deref_نښان(the_repository, parse_object(the_repository, &oid),
			   عدم, 0);
	إذا (!پایله)
		أرجع error(_("invalid object: %s"), hash);
	وإلا إذا (object_as_type(پایله, OBJ_COMMIT, 1))
		oidset_ورننباسه(commits, &پایله->oid);

	display_پرمختګ(پرمختګ, oidset_اندازه(commits));

	أرجع 0;
}

ساكن صحيح write_option_max_new_filters(ثابت بنية option *opt,
					ثابت حرف *arg,
					صحيح unset)
{
	صحيح *منزل = opt->ارزښت;
	إذا (unset)
		*منزل = -1;
	وإلا {
		ثابت حرف *s;
		*منزل = strtol(arg, (حرف **)&s, 10);
		إذا (*s)
			أرجع error(_("option `%s' expects a numerical value"),
				     "max-new-filters");
	}
	أرجع 0;
}

ساكن صحيح git_commit_ګراف_write_config(ثابت حرف *var, ثابت حرف *ارزښت,
					 ثابت بنية config_context *ctx,
					 فراغ *cb ناکارول_شوی)
{
	إذا (!strcmp(var, "commitgraph.maxnewfilters"))
		write_opts.max_new_filters = git_config_int(var, ارزښت, ctx->kvi);
	وإلا إذا (!strcmp(var, "commitgraph.changedpaths"))
		opts.enable_بدل_شوی_paths = git_config_bool(var, ارزښت) ? 1 : -1;
	/*
	 * No need to fall-back to 'git_default_config', since this was already
	 * called in 'cmd_commit_graph()'.
	 */
	أرجع 0;
}

ساكن صحيح ګراف_write(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی,
		       بنية repository *repo ناکارول_شوی)
{
	بنية string_list pack_indexes = STRING_LIST_INIT_DUP;
	بنية strbuf buf = STRBUF_INIT;
	بنية oidset commits = OIDSET_INIT;
	بنية odb_files_dir *dir = عدم;
	صحيح پایله = 0;
	تعداد commit_ګراف_write_flags flags = 0;
	بنية پرمختګ *پرمختګ = عدم;

	ساكن بنية option builtin_commit_ګراف_write_options[] = {
		OPT_BOOL(0, "reachable", &opts.reachable,
			N_("start walk at all refs")),
		OPT_BOOL(0, "stdin-packs", &opts.stdin_packs,
			N_("scan pack-indexes listed by stdin for commits")),
		OPT_BOOL(0, "stdin-commits", &opts.stdin_commits,
			N_("start walk at commits listed by stdin")),
		OPT_BOOL(0, "append", &opts.append,
			N_("include all commits already in the commit-graph file")),
		OPT_BOOL(0, "changed-paths", &opts.enable_بدل_شوی_paths,
			N_("enable computation for changed paths")),
		OPT_CALLBACK_F(0, "split", &write_opts.وېشل_flags, عدم,
			N_("allow writing an incremental commit-graph file"),
			PARSE_OPT_OPTARG | PARSE_OPT_NONEG,
			write_option_parse_وېشل),
		OPT_INTEGER(0, "max-commits", &write_opts.max_commits,
			N_("maximum number of commits in a non-base split commit-graph")),
		OPT_INTEGER(0, "size-multiple", &write_opts.اندازه_مضرب,
			N_("maximum ratio between two levels of a split commit-graph")),
		OPT_EXPIRY_DATE(0, "expire-time", &write_opts.expire_وخت,
			N_("only expire files older than a given date-time")),
		OPT_CALLBACK_F(0, "max-new-filters", &write_opts.max_new_filters,
			عدم, N_("maximum number of changed-path Bloom filters to compute"),
			0, write_option_max_new_filters),
		OPT_BOOL(0, "progress", &opts.پرمختګ,
			 N_("force progress reporting")),
		OPT_پای_ټکی(),
	};
	بنية option *options = جمع_کړه_common_options(builtin_commit_ګراف_write_options);

	opts.پرمختګ = isatty(2);
	opts.enable_بدل_شوی_paths = -1;
	write_opts.اندازه_مضرب = 2;
	write_opts.max_commits = 0;
	write_opts.expire_وخت = 0;
	write_opts.max_new_filters = -1;

	trace2_cmd_ډېرپېښ("write");

	repo_config(the_repository, git_commit_ګراف_write_config, &opts);

	argc = parse_options(argc, argv, مختاړی,
			     options,
			     builtin_commit_ګراف_write_usage, 0);
	إذا (argc)
		usage_with_options(builtin_commit_ګراف_write_usage, options);

	إذا (opts.reachable + opts.stdin_packs + opts.stdin_commits > 1)
		die(_("use at most one of --reachable, --stdin-commits, or --stdin-packs"));
	إذا (!opts.obj_dir)
		opts.obj_dir = repo_get_object_directory(the_repository);
	إذا (opts.append)
		flags |= COMMIT_ګراف_WRITE_APPEND;
	إذا (opts.وېشل)
		flags |= COMMIT_ګراف_WRITE_وېشل;
	إذا (opts.پرمختګ)
		flags |= COMMIT_ګراف_WRITE_پرمختګ;
	إذا (!opts.enable_بدل_شوی_paths)
		flags |= COMMIT_ګراف_NO_WRITE_BLOOM_FILTERS;
	إذا (opts.enable_بدل_شوی_paths == 1 ||
	    git_env_bool(GIT_TEST_COMMIT_ګراف_بدل_شوی_PATHS, 0))
		flags |= COMMIT_ګراف_WRITE_BLOOM_FILTERS;

	dir = odb_سرچینه_files_پیدا_کړه_dir(the_repository->objects, opts.obj_dir);
	إذا (!dir)
		die(_("could not find object directory matching %s"), opts.obj_dir);

	إذا (opts.reachable) {
		إذا (write_commit_ګراف_reachable(the_repository, dir->abspath, flags, &write_opts))
			پایله = 1;
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
		إذا (opts.پرمختګ)
			پرمختګ = پیل_delayed_پرمختګ(
				the_repository,
				_("Collecting commits from input"), 0);

		طالما (strbuf_getline(&buf, stdin) != EOF) {
			إذا (read_one_commit(&commits, پرمختګ, buf.buf)) {
				پایله = 1;
				stop_پرمختګ(&پرمختګ);
				اذهب cleanup;
			}
		}

		stop_پرمختګ(&پرمختګ);
	}

	إذا (write_commit_ګراف(the_repository, dir->abspath,
			       opts.stdin_packs ? &pack_indexes : عدم,
			       opts.stdin_commits ? &commits : عدم,
			       flags,
			       &write_opts))
		پایله = 1;

cleanup:
	FREE_AND_NULL(options);
	string_list_clear(&pack_indexes, 0);
	strbuf_release(&buf);
	oidset_clear(&commits);
	أرجع پایله;
}

صحيح cmd_commit_ګراف(صحيح argc,
		     ثابت حرف **argv,
		     ثابت حرف *مختاړی,
		     بنية repository *repo)
{
	parse_opt_subcommand_fn *fn = عدم;
	بنية option builtin_commit_ګراف_options[] = {
		OPT_SUBCOMMAND("verify", &fn, ګراف_verify),
		OPT_SUBCOMMAND("write", &fn, ګراف_write),
		OPT_پای_ټکی(),
	};
	بنية option *options = parse_options_concat(builtin_commit_ګراف_options, common_opts);

	repo_config(the_repository, git_default_config, عدم);

	disable_replace_refs();
	save_commit_بفر = 0;

	argc = parse_options(argc, argv, مختاړی, options,
			     builtin_commit_ګراف_usage, 0);
	FREE_AND_NULL(options);

	أرجع fn(argc, argv, مختاړی, repo);
}
