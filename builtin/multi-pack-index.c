#وصف_ڪريو USE_THE_REPOSITORY_VARIABLE
#ضمن "builtin.h"
#ضمن "abspath.h"
#ضمن "config.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "parse-options.h"
#ضمن "midx.h"
#ضمن "strbuf.h"
#ضمن "trace2.h"
#ضمن "odb.h"
#ضمن "odb/source.h"
#ضمن "odb/source-files.h"
#ضمن "replace-object.h"
#ضمن "repository.h"

#وصف_ڪريو BUILTIN_MIDX_WRITE_USAGE \
	N_("git multi-pack-index [<options>] write [--preferred-pack=<pack>]\n" \
	   "  [--[no-]bitmap] [--[no-]incremental] [--[no-]stdin-packs]\n" \
	   "  [--refs-snapshot=<path>] [--[no-]write-chain-file]\n" \
	   "  [--base=<checksum>]")

#وصف_ڪريو BUILTIN_MIDX_COMPACT_USAGE \
	N_("git multi-pack-index [<options>] compact [--[no-]incremental]\n" \
	   "  [--[no-]bitmap] [--base=<checksum>] [--[no-]write-chain-file]\n" \
	   "  <from> <to>")

#وصف_ڪريو BUILTIN_MIDX_VERIFY_USAGE \
	N_("git multi-pack-index [<options>] verify")

#وصف_ڪريو BUILTIN_MIDX_EXPIRE_USAGE \
	N_("git multi-pack-index [<options>] expire")

#وصف_ڪريو BUILTIN_MIDX_REPACK_USAGE \
	N_("git multi-pack-index [<options>] repack [--batch-size=<size>]")

ساكن حرف ثابت * ثابت builtin_multi_pack_اشاريو_write_usage[] = {
	BUILTIN_MIDX_WRITE_USAGE,
	عدم
};
ساكن حرف ثابت * ثابت builtin_multi_pack_اشاريو_compact_usage[] = {
	BUILTIN_MIDX_COMPACT_USAGE,
	عدم
};
ساكن حرف ثابت * ثابت builtin_multi_pack_اشاريو_verify_usage[] = {
	BUILTIN_MIDX_VERIFY_USAGE,
	عدم
};
ساكن حرف ثابت * ثابت builtin_multi_pack_اشاريو_expire_usage[] = {
	BUILTIN_MIDX_EXPIRE_USAGE,
	عدم
};
ساكن حرف ثابت * ثابت builtin_multi_pack_اشاريو_repack_usage[] = {
	BUILTIN_MIDX_REPACK_USAGE,
	عدم
};
ساكن حرف ثابت * ثابت builtin_multi_pack_اشاريو_usage[] = {
	BUILTIN_MIDX_WRITE_USAGE,
	BUILTIN_MIDX_COMPACT_USAGE,
	BUILTIN_MIDX_VERIFY_USAGE,
	BUILTIN_MIDX_EXPIRE_USAGE,
	BUILTIN_MIDX_REPACK_USAGE,
	عدم
};

ساكن بنية opts_multi_pack_اشاريو {
	حرف *object_dir;
	ثابت حرف *preferred_pack;
	ثابت حرف *incremental_بنياد;
	حرف *refs_snapshot;
	بنانشان ڊگهو batch_سائيز;
	بنانشان flags;
	صحيح stdin_packs;
} opts;


ساكن صحيح parse_object_dir(ثابت بنية option *opt, ثابت حرف *arg,
			    صحيح unset)
{
	حرف **قدر = opt->قدر;
	حرر(*قدر);
	إذا (unset)
		*قدر = xstrdup(the_repository->objects->ذريعو->رستو);
	وإلا
		*قدر = real_pathdup(arg, 1);
	أرجع 0;
}

ساكن بنية odb_ذريعو_packed *handle_object_dir_option(بنية repository *repo)
{
	بنية odb_files_dir *dir = odb_ذريعو_files_ڳوليو_dir(repo->objects, opts.object_dir);
	إذا (!dir)
		die(_("object directory is not an alternate of the current repository: '%s'"),
		    opts.object_dir);
	أرجع dir->packed;
}

ساكن بنية option common_opts[] = {
	OPT_CALLBACK(0, "object-dir", &opts.object_dir,
	  N_("directory"),
	  N_("object directory containing set of packfile and pack-index pairs"),
	  parse_object_dir),
	OPT_ٻٽو_انگ(0, "progress", &opts.flags, N_("force progress reporting"),
		MIDX_ترقي),
	OPT_پڇاڙي(),
};

ساكن بنية option *جوڙيو_common_options(بنية option *prev)
{
	أرجع parse_options_concat(common_opts, prev);
}

ساكن صحيح git_multi_pack_اشاريو_write_config(ثابت حرف *var, ثابت حرف *قدر,
					     ثابت بنية config_context *ctx اڻ_استعمال_ٿيل,
					     فراغ *cb اڻ_استعمال_ٿيل)
{
	إذا (!strcmp(var, "pack.writebitmaphashcache")) {
		إذا (git_config_bool(var, قدر))
			opts.flags |= MIDX_WRITE_BITMAP_HASH_CACHE;
		وإلا
			opts.flags &= ~MIDX_WRITE_BITMAP_HASH_CACHE;
	}

	إذا (!strcmp(var, "pack.writebitmaplookuptable")) {
		إذا (git_config_bool(var, قدر))
			opts.flags |= MIDX_WRITE_BITMAP_LOOKUP_جدول;
		وإلا
			opts.flags &= ~MIDX_WRITE_BITMAP_LOOKUP_جدول;
	}

	/*
	 * We should never make a fall-back call to 'git_default_config', since
	 * this was already called in 'cmd_multi_pack_index()'.
	 */
	أرجع 0;
}

ساكن فراغ read_packs_from_stdin(بنية string_list *منزل)
{
	بنية strbuf buf = STRBUF_INIT;
	طالما (strbuf_getline(&buf, stdin) != EOF)
		string_list_append(منزل, buf.buf);
	string_list_ترتيب_ڏيو(منزل);

	strbuf_release(&buf);
}

ساكن صحيح cmd_multi_pack_اشاريو_write(صحيح argc, ثابت حرف **argv,
				      ثابت حرف *اڳياڙي,
				      بنية repository *repo)
{
	بنية option *options;
	ساكن بنية option builtin_multi_pack_اشاريو_write_options[] = {
		OPT_STRING(0, "preferred-pack", &opts.preferred_pack,
			   N_("preferred-pack"),
			   N_("pack for reuse when computing a multi-pack bitmap")),
		OPT_ٻٽو_انگ(0, "bitmap", &opts.flags, N_("write multi-pack bitmap"),
			MIDX_WRITE_BITMAP | MIDX_WRITE_REV_اشاريو),
		OPT_STRING(0, "base", &opts.incremental_بنياد, N_("checksum"),
			   N_("base MIDX for incremental writes")),
		OPT_ٻٽو_انگ(0, "incremental", &opts.flags,
			N_("write a new incremental MIDX"), MIDX_WRITE_INCREMENTAL),
		OPT_NEGBIT(0, "write-chain-file", &opts.flags,
			N_("write the multi-pack-index chain file"),
			MIDX_WRITE_NO_CHAIN),
		OPT_BOOL(0, "stdin-packs", &opts.stdin_packs,
			 N_("write multi-pack index containing only given indexes")),
		OPT_FILENAME(0, "refs-snapshot", &opts.refs_snapshot,
			     N_("refs snapshot for selecting bitmap commits")),
		OPT_پڇاڙي(),
	};
	بنية odb_ذريعو_packed *packed_ذريعو;
	صحيح ret;

	opts.flags |= MIDX_WRITE_BITMAP_HASH_CACHE;

	repo_config(the_repository, git_multi_pack_اشاريو_write_config, عدم);

	options = جوڙيو_common_options(builtin_multi_pack_اشاريو_write_options);

	trace2_cmd_گهڻائي(argv[0]);

	إذا (isatty(2))
		opts.flags |= MIDX_ترقي;
	argc = parse_options(argc, argv, اڳياڙي,
			     options, builtin_multi_pack_اشاريو_write_usage,
			     0);
	إذا (argc)
		usage_with_options(builtin_multi_pack_اشاريو_write_usage,
				   options);

	إذا (opts.flags & MIDX_WRITE_NO_CHAIN &&
	    !(opts.flags & MIDX_WRITE_INCREMENTAL)) {
		error(_("cannot use %s without %s"),
		      "--no-write-chain-file", "--incremental");
		usage_with_options(builtin_multi_pack_اشاريو_write_usage,
				   options);
	}

	إذا (opts.incremental_بنياد &&
	    !(opts.flags & MIDX_WRITE_NO_CHAIN)) {
		error(_("cannot use --base without --no-write-chain-file"));
		usage_with_options(builtin_multi_pack_اشاريو_write_usage,
				   options);
	}

	packed_ذريعو = handle_object_dir_option(repo);

	FREE_AND_NULL(options);

	إذا (opts.stdin_packs) {
		بنية string_list packs = STRING_LIST_INIT_DUP;

		read_packs_from_stdin(&packs);

		ret = write_midx_file_only(packed_ذريعو, &packs,
					   opts.preferred_pack,
					   opts.refs_snapshot,
					   opts.incremental_بنياد, opts.flags);

		string_list_clear(&packs, 0);
		حرر(opts.refs_snapshot);

		أرجع ret;

	}

	ret = write_midx_file(packed_ذريعو, opts.preferred_pack,
			      opts.refs_snapshot, opts.flags);

	حرر(opts.refs_snapshot);
	أرجع ret;
}

ساكن صحيح cmd_multi_pack_اشاريو_compact(صحيح argc, ثابت حرف **argv,
					ثابت حرف *اڳياڙي,
					بنية repository *repo)
{
	بنية multi_pack_اشاريو *m, *cur;
	بنية multi_pack_اشاريو *from_midx = عدم;
	بنية multi_pack_اشاريو *منزل_midx = عدم;
	بنية odb_ذريعو_packed *packed_ذريعو;
	صحيح ret;

	بنية option *options;
	ساكن بنية option builtin_multi_pack_اشاريو_compact_options[] = {
		OPT_STRING(0, "base", &opts.incremental_بنياد, N_("checksum"),
			   N_("base MIDX for incremental writes")),
		OPT_ٻٽو_انگ(0, "bitmap", &opts.flags, N_("write multi-pack bitmap"),
			MIDX_WRITE_BITMAP | MIDX_WRITE_REV_اشاريو),
		OPT_ٻٽو_انگ(0, "incremental", &opts.flags,
			N_("write a new incremental MIDX"), MIDX_WRITE_INCREMENTAL),
		OPT_NEGBIT(0, "write-chain-file", &opts.flags,
			N_("write the multi-pack-index chain file"),
			MIDX_WRITE_NO_CHAIN),
		OPT_پڇاڙي(),
	};

	repo_config(repo, git_multi_pack_اشاريو_write_config, عدم);

	options = جوڙيو_common_options(builtin_multi_pack_اشاريو_compact_options);

	trace2_cmd_گهڻائي(argv[0]);

	إذا (isatty(2))
		opts.flags |= MIDX_ترقي;
	argc = parse_options(argc, argv, اڳياڙي,
			     options, builtin_multi_pack_اشاريو_compact_usage,
			     0);

	إذا (argc != 2)
		usage_with_options(builtin_multi_pack_اشاريو_compact_usage,
				   options);

	إذا (opts.flags & MIDX_WRITE_NO_CHAIN &&
	    !(opts.flags & MIDX_WRITE_INCREMENTAL)) {
		error(_("cannot use %s without %s"),
		      "--no-write-chain-file", "--incremental");
		usage_with_options(builtin_multi_pack_اشاريو_compact_usage,
				   options);
	}

	packed_ذريعو = handle_object_dir_option(the_repository);

	FREE_AND_NULL(options);

	m = get_multi_pack_اشاريو(packed_ذريعو);

	لكل (cur = m; cur && !(from_midx && منزل_midx); cur = cur->بنياد_midx) {
		ثابت حرف *midx_csum = midx_get_checksum_hex(cur);

		إذا (!from_midx && !strcmp(midx_csum, argv[0]))
			from_midx = cur;
		إذا (!منزل_midx && !strcmp(midx_csum, argv[1]))
			منزل_midx = cur;
	}

	إذا (!from_midx)
		die(_("could not find MIDX: %s"), argv[0]);
	إذا (!منزل_midx)
		die(_("could not find MIDX: %s"), argv[1]);
	إذا (from_midx == منزل_midx)
		die(_("MIDX compaction endpoints must be unique"));

	لكل (m = from_midx; m; m = m->بنياد_midx) {
		إذا (m == منزل_midx)
			die(_("MIDX %s must be an ancestor of %s"), argv[0], argv[1]);
	}

	ret = write_midx_file_compact(packed_ذريعو, from_midx, منزل_midx,
				      opts.incremental_بنياد, opts.flags);

	أرجع ret;
}

ساكن صحيح cmd_multi_pack_اشاريو_verify(صحيح argc, ثابت حرف **argv,
				       ثابت حرف *اڳياڙي,
				       بنية repository *repo اڻ_استعمال_ٿيل)
{
	بنية option *options;
	ساكن بنية option builtin_multi_pack_اشاريو_verify_options[] = {
		OPT_پڇاڙي(),
	};
	بنية odb_ذريعو_packed *packed_ذريعو;

	options = جوڙيو_common_options(builtin_multi_pack_اشاريو_verify_options);

	trace2_cmd_گهڻائي(argv[0]);

	إذا (isatty(2))
		opts.flags |= MIDX_ترقي;
	argc = parse_options(argc, argv, اڳياڙي,
			     options, builtin_multi_pack_اشاريو_verify_usage,
			     0);
	إذا (argc)
		usage_with_options(builtin_multi_pack_اشاريو_verify_usage,
				   options);
	packed_ذريعو = handle_object_dir_option(the_repository);

	FREE_AND_NULL(options);

	أرجع verify_midx_file(packed_ذريعو, opts.flags);
}

ساكن صحيح cmd_multi_pack_اشاريو_expire(صحيح argc, ثابت حرف **argv,
				       ثابت حرف *اڳياڙي,
				       بنية repository *repo اڻ_استعمال_ٿيل)
{
	بنية option *options;
	ساكن بنية option builtin_multi_pack_اشاريو_expire_options[] = {
		OPT_پڇاڙي(),
	};
	بنية odb_ذريعو_packed *packed_ذريعو;

	options = جوڙيو_common_options(builtin_multi_pack_اشاريو_expire_options);

	trace2_cmd_گهڻائي(argv[0]);

	إذا (isatty(2))
		opts.flags |= MIDX_ترقي;
	argc = parse_options(argc, argv, اڳياڙي,
			     options, builtin_multi_pack_اشاريو_expire_usage,
			     0);
	إذا (argc)
		usage_with_options(builtin_multi_pack_اشاريو_expire_usage,
				   options);
	packed_ذريعو = handle_object_dir_option(the_repository);

	FREE_AND_NULL(options);

	أرجع expire_midx_packs(packed_ذريعو, opts.flags);
}

ساكن صحيح cmd_multi_pack_اشاريو_repack(صحيح argc, ثابت حرف **argv,
				       ثابت حرف *اڳياڙي,
				       بنية repository *repo اڻ_استعمال_ٿيل)
{
	بنية option *options;
	ساكن بنية option builtin_multi_pack_اشاريو_repack_options[] = {
		OPT_UNSIGNED(0, "batch-size", &opts.batch_سائيز,
		  N_("during repack, collect pack-files of smaller size into a batch that is larger than this size")),
		OPT_پڇاڙي(),
	};
	بنية odb_ذريعو_packed *packed_ذريعو;

	options = جوڙيو_common_options(builtin_multi_pack_اشاريو_repack_options);

	trace2_cmd_گهڻائي(argv[0]);

	إذا (isatty(2))
		opts.flags |= MIDX_ترقي;
	argc = parse_options(argc, argv, اڳياڙي,
			     options,
			     builtin_multi_pack_اشاريو_repack_usage,
			     0);
	إذا (argc)
		usage_with_options(builtin_multi_pack_اشاريو_repack_usage,
				   options);
	packed_ذريعو = handle_object_dir_option(the_repository);

	FREE_AND_NULL(options);

	أرجع midx_repack(packed_ذريعو, (سائيز_t)opts.batch_سائيز, opts.flags);
}

صحيح cmd_multi_pack_اشاريو(صحيح argc,
			 ثابت حرف **argv,
			 ثابت حرف *اڳياڙي,
			 بنية repository *repo)
{
	صحيح res;
	parse_opt_subcommand_fn *fn = عدم;
	بنية option builtin_multi_pack_اشاريو_options[] = {
		OPT_SUBCOMMAND("repack", &fn, cmd_multi_pack_اشاريو_repack),
		OPT_SUBCOMMAND("write", &fn, cmd_multi_pack_اشاريو_write),
		OPT_SUBCOMMAND("compact", &fn, cmd_multi_pack_اشاريو_compact),
		OPT_SUBCOMMAND("verify", &fn, cmd_multi_pack_اشاريو_verify),
		OPT_SUBCOMMAND("expire", &fn, cmd_multi_pack_اشاريو_expire),
		OPT_پڇاڙي(),
	};
	بنية option *options = parse_options_concat(builtin_multi_pack_اشاريو_options, common_opts);

	disable_replace_refs();

	repo_config(the_repository, git_default_config, عدم);

	إذا (the_repository &&
	    the_repository->objects &&
	    the_repository->objects->ذريعو)
		opts.object_dir = xstrdup(the_repository->objects->ذريعو->رستو);

	argc = parse_options(argc, argv, اڳياڙي, options,
			     builtin_multi_pack_اشاريو_usage, 0);
	FREE_AND_NULL(options);

	res = fn(argc, argv, اڳياڙي, repo);

	حرر(opts.object_dir);
	أرجع res;
}
