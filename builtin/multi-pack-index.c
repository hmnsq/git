#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#شامل "builtin.h"
#شامل "abspath.h"
#شامل "config.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "parse-options.h"
#شامل "midx.h"
#شامل "strbuf.h"
#شامل "trace2.h"
#شامل "odb.h"
#شامل "odb/source.h"
#شامل "odb/source-files.h"
#شامل "replace-object.h"
#شامل "repository.h"

#تعریف_کرو BUILTIN_MIDX_WRITE_USAGE \
	N_("git multi-pack-index [<options>] write [--preferred-pack=<pack>]\n" \
	   "  [--[no-]bitmap] [--[no-]incremental] [--[no-]stdin-packs]\n" \
	   "  [--refs-snapshot=<path>] [--[no-]write-chain-file]\n" \
	   "  [--base=<checksum>]")

#تعریف_کرو BUILTIN_MIDX_COMPACT_USAGE \
	N_("git multi-pack-index [<options>] compact [--[no-]incremental]\n" \
	   "  [--[no-]bitmap] [--base=<checksum>] [--[no-]write-chain-file]\n" \
	   "  <from> <to>")

#تعریف_کرو BUILTIN_MIDX_VERIFY_USAGE \
	N_("git multi-pack-index [<options>] verify")

#تعریف_کرو BUILTIN_MIDX_EXPIRE_USAGE \
	N_("git multi-pack-index [<options>] expire")

#تعریف_کرو BUILTIN_MIDX_REPACK_USAGE \
	N_("git multi-pack-index [<options>] repack [--batch-size=<size>]")

ساکن حرف ثابت * ثابت builtin_multi_pack_اشاریہ_write_usage[] = {
	BUILTIN_MIDX_WRITE_USAGE,
	تہی
};
ساکن حرف ثابت * ثابت builtin_multi_pack_اشاریہ_compact_usage[] = {
	BUILTIN_MIDX_COMPACT_USAGE,
	تہی
};
ساکن حرف ثابت * ثابت builtin_multi_pack_اشاریہ_verify_usage[] = {
	BUILTIN_MIDX_VERIFY_USAGE,
	تہی
};
ساکن حرف ثابت * ثابت builtin_multi_pack_اشاریہ_expire_usage[] = {
	BUILTIN_MIDX_EXPIRE_USAGE,
	تہی
};
ساکن حرف ثابت * ثابت builtin_multi_pack_اشاریہ_repack_usage[] = {
	BUILTIN_MIDX_REPACK_USAGE,
	تہی
};
ساکن حرف ثابت * ثابت builtin_multi_pack_اشاریہ_usage[] = {
	BUILTIN_MIDX_WRITE_USAGE,
	BUILTIN_MIDX_COMPACT_USAGE,
	BUILTIN_MIDX_VERIFY_USAGE,
	BUILTIN_MIDX_EXPIRE_USAGE,
	BUILTIN_MIDX_REPACK_USAGE,
	تہی
};

ساکن ساخت opts_multi_pack_اشاریہ {
	حرف *object_dir;
	ثابت حرف *preferred_pack;
	ثابت حرف *incremental_بنیاد;
	حرف *refs_snapshot;
	غیرعلامتی طویل batch_سائز;
	غیرعلامتی flags;
	صحیح stdin_packs;
} opts;


ساکن صحیح parse_object_dir(ثابت ساخت option *opt, ثابت حرف *arg,
			    صحیح unset)
{
	حرف **قدر = opt->قدر;
	آزاد(*قدر);
	اگر (unset)
		*قدر = xstrdup(the_repository->objects->ماخذ->راستہ);
	ورنہ
		*قدر = real_pathdup(arg, 1);
	واپس 0;
}

ساکن ساخت odb_ماخذ_packed *handle_object_dir_option(ساخت repository *repo)
{
	ساخت odb_files_dir *dir = odb_ماخذ_files_ڈھونڈو_dir(repo->objects, opts.object_dir);
	اگر (!dir)
		die(_("object directory is not an alternate of the current repository: '%s'"),
		    opts.object_dir);
	واپس dir->packed;
}

ساکن ساخت option common_opts[] = {
	OPT_CALLBACK(0, "object-dir", &opts.object_dir,
	  N_("directory"),
	  N_("object directory containing set of packfile and pack-index pairs"),
	  parse_object_dir),
	OPT_ثنائی_ہندسہ(0, "progress", &opts.flags, N_("force progress reporting"),
		MIDX_پیش_رفت),
	OPT_انتہا(),
};

ساکن ساخت option *جوڑو_common_options(ساخت option *پچھلا_2)
{
	واپس parse_options_concat(common_opts, پچھلا_2);
}

ساکن صحیح git_multi_pack_اشاریہ_write_config(ثابت حرف *var, ثابت حرف *قدر,
					     ثابت ساخت config_context *ctx غیر_مستعمل,
					     خالی *cb غیر_مستعمل)
{
	اگر (!strcmp(var, "pack.writebitmaphashcache")) {
		اگر (git_config_bool(var, قدر))
			opts.flags |= MIDX_WRITE_BITMAP_HASH_CACHE;
		ورنہ
			opts.flags &= ~MIDX_WRITE_BITMAP_HASH_CACHE;
	}

	اگر (!strcmp(var, "pack.writebitmaplookuptable")) {
		اگر (git_config_bool(var, قدر))
			opts.flags |= MIDX_WRITE_BITMAP_LOOKUP_جدول;
		ورنہ
			opts.flags &= ~MIDX_WRITE_BITMAP_LOOKUP_جدول;
	}

	/*
	 * We should never make a fall-back call to 'git_default_config', since
	 * this was already called in 'cmd_multi_pack_index()'.
	 */
	واپس 0;
}

ساکن خالی read_packs_from_stdin(ساخت string_list *مقصد)
{
	ساخت strbuf buf = STRBUF_INIT;
	جب (strbuf_getline(&buf, stdin) != EOF)
		string_list_append(مقصد, buf.buf);
	string_list_ترتیب_دو(مقصد);

	strbuf_release(&buf);
}

ساکن صحیح cmd_multi_pack_اشاریہ_write(صحیح argc, ثابت حرف **argv,
				      ثابت حرف *سابقہ,
				      ساخت repository *repo)
{
	ساخت option *options;
	ساکن ساخت option builtin_multi_pack_اشاریہ_write_options[] = {
		OPT_STRING(0, "preferred-pack", &opts.preferred_pack,
			   N_("preferred-pack"),
			   N_("pack for reuse when computing a multi-pack bitmap")),
		OPT_ثنائی_ہندسہ(0, "bitmap", &opts.flags, N_("write multi-pack bitmap"),
			MIDX_WRITE_BITMAP | MIDX_WRITE_REV_اشاریہ),
		OPT_STRING(0, "base", &opts.incremental_بنیاد, N_("checksum"),
			   N_("base MIDX for incremental writes")),
		OPT_ثنائی_ہندسہ(0, "incremental", &opts.flags,
			N_("write a new incremental MIDX"), MIDX_WRITE_INCREMENTAL),
		OPT_NEGBIT(0, "write-chain-file", &opts.flags,
			N_("write the multi-pack-index chain file"),
			MIDX_WRITE_NO_CHAIN),
		OPT_BOOL(0, "stdin-packs", &opts.stdin_packs,
			 N_("write multi-pack index containing only given indexes")),
		OPT_FILENAME(0, "refs-snapshot", &opts.refs_snapshot,
			     N_("refs snapshot for selecting bitmap commits")),
		OPT_انتہا(),
	};
	ساخت odb_ماخذ_packed *packed_ماخذ;
	صحیح ret;

	opts.flags |= MIDX_WRITE_BITMAP_HASH_CACHE;

	repo_config(the_repository, git_multi_pack_اشاریہ_write_config, تہی);

	options = جوڑو_common_options(builtin_multi_pack_اشاریہ_write_options);

	trace2_cmd_منوال(argv[0]);

	اگر (isatty(2))
		opts.flags |= MIDX_پیش_رفت;
	argc = parse_options(argc, argv, سابقہ,
			     options, builtin_multi_pack_اشاریہ_write_usage,
			     0);
	اگر (argc)
		usage_with_options(builtin_multi_pack_اشاریہ_write_usage,
				   options);

	اگر (opts.flags & MIDX_WRITE_NO_CHAIN &&
	    !(opts.flags & MIDX_WRITE_INCREMENTAL)) {
		error(_("cannot use %s without %s"),
		      "--no-write-chain-file", "--incremental");
		usage_with_options(builtin_multi_pack_اشاریہ_write_usage,
				   options);
	}

	اگر (opts.incremental_بنیاد &&
	    !(opts.flags & MIDX_WRITE_NO_CHAIN)) {
		error(_("cannot use --base without --no-write-chain-file"));
		usage_with_options(builtin_multi_pack_اشاریہ_write_usage,
				   options);
	}

	packed_ماخذ = handle_object_dir_option(repo);

	FREE_AND_NULL(options);

	اگر (opts.stdin_packs) {
		ساخت string_list packs = STRING_LIST_INIT_DUP;

		read_packs_from_stdin(&packs);

		ret = write_midx_file_only(packed_ماخذ, &packs,
					   opts.preferred_pack,
					   opts.refs_snapshot,
					   opts.incremental_بنیاد, opts.flags);

		string_list_clear(&packs, 0);
		آزاد(opts.refs_snapshot);

		واپس ret;

	}

	ret = write_midx_file(packed_ماخذ, opts.preferred_pack,
			      opts.refs_snapshot, opts.flags);

	آزاد(opts.refs_snapshot);
	واپس ret;
}

ساکن صحیح cmd_multi_pack_اشاریہ_compact(صحیح argc, ثابت حرف **argv,
					ثابت حرف *سابقہ,
					ساخت repository *repo)
{
	ساخت multi_pack_اشاریہ *m, *cur;
	ساخت multi_pack_اشاریہ *from_midx = تہی;
	ساخت multi_pack_اشاریہ *مقصد_midx = تہی;
	ساخت odb_ماخذ_packed *packed_ماخذ;
	صحیح ret;

	ساخت option *options;
	ساکن ساخت option builtin_multi_pack_اشاریہ_compact_options[] = {
		OPT_STRING(0, "base", &opts.incremental_بنیاد, N_("checksum"),
			   N_("base MIDX for incremental writes")),
		OPT_ثنائی_ہندسہ(0, "bitmap", &opts.flags, N_("write multi-pack bitmap"),
			MIDX_WRITE_BITMAP | MIDX_WRITE_REV_اشاریہ),
		OPT_ثنائی_ہندسہ(0, "incremental", &opts.flags,
			N_("write a new incremental MIDX"), MIDX_WRITE_INCREMENTAL),
		OPT_NEGBIT(0, "write-chain-file", &opts.flags,
			N_("write the multi-pack-index chain file"),
			MIDX_WRITE_NO_CHAIN),
		OPT_انتہا(),
	};

	repo_config(repo, git_multi_pack_اشاریہ_write_config, تہی);

	options = جوڑو_common_options(builtin_multi_pack_اشاریہ_compact_options);

	trace2_cmd_منوال(argv[0]);

	اگر (isatty(2))
		opts.flags |= MIDX_پیش_رفت;
	argc = parse_options(argc, argv, سابقہ,
			     options, builtin_multi_pack_اشاریہ_compact_usage,
			     0);

	اگر (argc != 2)
		usage_with_options(builtin_multi_pack_اشاریہ_compact_usage,
				   options);

	اگر (opts.flags & MIDX_WRITE_NO_CHAIN &&
	    !(opts.flags & MIDX_WRITE_INCREMENTAL)) {
		error(_("cannot use %s without %s"),
		      "--no-write-chain-file", "--incremental");
		usage_with_options(builtin_multi_pack_اشاریہ_compact_usage,
				   options);
	}

	packed_ماخذ = handle_object_dir_option(the_repository);

	FREE_AND_NULL(options);

	m = get_multi_pack_اشاریہ(packed_ماخذ);

	برائے (cur = m; cur && !(from_midx && مقصد_midx); cur = cur->بنیاد_midx) {
		ثابت حرف *midx_csum = midx_get_checksum_hex(cur);

		اگر (!from_midx && !strcmp(midx_csum, argv[0]))
			from_midx = cur;
		اگر (!مقصد_midx && !strcmp(midx_csum, argv[1]))
			مقصد_midx = cur;
	}

	اگر (!from_midx)
		die(_("could not find MIDX: %s"), argv[0]);
	اگر (!مقصد_midx)
		die(_("could not find MIDX: %s"), argv[1]);
	اگر (from_midx == مقصد_midx)
		die(_("MIDX compaction endpoints must be unique"));

	برائے (m = from_midx; m; m = m->بنیاد_midx) {
		اگر (m == مقصد_midx)
			die(_("MIDX %s must be an ancestor of %s"), argv[0], argv[1]);
	}

	ret = write_midx_file_compact(packed_ماخذ, from_midx, مقصد_midx,
				      opts.incremental_بنیاد, opts.flags);

	واپس ret;
}

ساکن صحیح cmd_multi_pack_اشاریہ_verify(صحیح argc, ثابت حرف **argv,
				       ثابت حرف *سابقہ,
				       ساخت repository *repo غیر_مستعمل)
{
	ساخت option *options;
	ساکن ساخت option builtin_multi_pack_اشاریہ_verify_options[] = {
		OPT_انتہا(),
	};
	ساخت odb_ماخذ_packed *packed_ماخذ;

	options = جوڑو_common_options(builtin_multi_pack_اشاریہ_verify_options);

	trace2_cmd_منوال(argv[0]);

	اگر (isatty(2))
		opts.flags |= MIDX_پیش_رفت;
	argc = parse_options(argc, argv, سابقہ,
			     options, builtin_multi_pack_اشاریہ_verify_usage,
			     0);
	اگر (argc)
		usage_with_options(builtin_multi_pack_اشاریہ_verify_usage,
				   options);
	packed_ماخذ = handle_object_dir_option(the_repository);

	FREE_AND_NULL(options);

	واپس verify_midx_file(packed_ماخذ, opts.flags);
}

ساکن صحیح cmd_multi_pack_اشاریہ_expire(صحیح argc, ثابت حرف **argv,
				       ثابت حرف *سابقہ,
				       ساخت repository *repo غیر_مستعمل)
{
	ساخت option *options;
	ساکن ساخت option builtin_multi_pack_اشاریہ_expire_options[] = {
		OPT_انتہا(),
	};
	ساخت odb_ماخذ_packed *packed_ماخذ;

	options = جوڑو_common_options(builtin_multi_pack_اشاریہ_expire_options);

	trace2_cmd_منوال(argv[0]);

	اگر (isatty(2))
		opts.flags |= MIDX_پیش_رفت;
	argc = parse_options(argc, argv, سابقہ,
			     options, builtin_multi_pack_اشاریہ_expire_usage,
			     0);
	اگر (argc)
		usage_with_options(builtin_multi_pack_اشاریہ_expire_usage,
				   options);
	packed_ماخذ = handle_object_dir_option(the_repository);

	FREE_AND_NULL(options);

	واپس expire_midx_packs(packed_ماخذ, opts.flags);
}

ساکن صحیح cmd_multi_pack_اشاریہ_repack(صحیح argc, ثابت حرف **argv,
				       ثابت حرف *سابقہ,
				       ساخت repository *repo غیر_مستعمل)
{
	ساخت option *options;
	ساکن ساخت option builtin_multi_pack_اشاریہ_repack_options[] = {
		OPT_UNSIGNED(0, "batch-size", &opts.batch_سائز,
		  N_("during repack, collect pack-files of smaller size into a batch that is larger than this size")),
		OPT_انتہا(),
	};
	ساخت odb_ماخذ_packed *packed_ماخذ;

	options = جوڑو_common_options(builtin_multi_pack_اشاریہ_repack_options);

	trace2_cmd_منوال(argv[0]);

	اگر (isatty(2))
		opts.flags |= MIDX_پیش_رفت;
	argc = parse_options(argc, argv, سابقہ,
			     options,
			     builtin_multi_pack_اشاریہ_repack_usage,
			     0);
	اگر (argc)
		usage_with_options(builtin_multi_pack_اشاریہ_repack_usage,
				   options);
	packed_ماخذ = handle_object_dir_option(the_repository);

	FREE_AND_NULL(options);

	واپس midx_repack(packed_ماخذ, (سائز_t)opts.batch_سائز, opts.flags);
}

صحیح cmd_multi_pack_اشاریہ(صحیح argc,
			 ثابت حرف **argv,
			 ثابت حرف *سابقہ,
			 ساخت repository *repo)
{
	صحیح res;
	parse_opt_subcommand_fn *fn = تہی;
	ساخت option builtin_multi_pack_اشاریہ_options[] = {
		OPT_SUBCOMMAND("repack", &fn, cmd_multi_pack_اشاریہ_repack),
		OPT_SUBCOMMAND("write", &fn, cmd_multi_pack_اشاریہ_write),
		OPT_SUBCOMMAND("compact", &fn, cmd_multi_pack_اشاریہ_compact),
		OPT_SUBCOMMAND("verify", &fn, cmd_multi_pack_اشاریہ_verify),
		OPT_SUBCOMMAND("expire", &fn, cmd_multi_pack_اشاریہ_expire),
		OPT_انتہا(),
	};
	ساخت option *options = parse_options_concat(builtin_multi_pack_اشاریہ_options, common_opts);

	disable_replace_refs();

	repo_config(the_repository, git_default_config, تہی);

	اگر (the_repository &&
	    the_repository->objects &&
	    the_repository->objects->ماخذ)
		opts.object_dir = xstrdup(the_repository->objects->ماخذ->راستہ);

	argc = parse_options(argc, argv, سابقہ, options,
			     builtin_multi_pack_اشاریہ_usage, 0);
	FREE_AND_NULL(options);

	res = fn(argc, argv, سابقہ, repo);

	آزاد(opts.object_dir);
	واپس res;
}
