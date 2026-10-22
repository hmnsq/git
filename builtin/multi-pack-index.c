#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "abspath.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "midx.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "strbuf.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "trace2.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "odb.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "odb/source.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "odb/source-files.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "replace-object.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "repository.h"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BUILTIN_MIDX_WRITE_USAGE \
	N_("git multi-pack-index [<options>] write [--preferred-pack=<pack>]\n" \
	   "  [--[no-]bitmap] [--[no-]incremental] [--[no-]stdin-packs]\n" \
	   "  [--refs-snapshot=<path>] [--[no-]write-chain-file]\n" \
	   "  [--base=<checksum>]")

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BUILTIN_MIDX_COMPACT_USAGE \
	N_("git multi-pack-index [<options>] compact [--[no-]incremental]\n" \
	   "  [--[no-]bitmap] [--base=<checksum>] [--[no-]write-chain-file]\n" \
	   "  <from> <to>")

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BUILTIN_MIDX_VERIFY_USAGE \
	N_("git multi-pack-index [<options>] verify")

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BUILTIN_MIDX_EXPIRE_USAGE \
	N_("git multi-pack-index [<options>] expire")

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BUILTIN_MIDX_REPACK_USAGE \
	N_("git multi-pack-index [<options>] repack [--batch-size=<size>]")

ਸਥਿਰ ਅੱਖਰ ਅਚਲ * ਅਚਲ builtin_multi_pack_ਸੂਚਕਾਂਕ_write_usage[] = {
	BUILTIN_MIDX_WRITE_USAGE,
	ਨਿਰਾ
};
ਸਥਿਰ ਅੱਖਰ ਅਚਲ * ਅਚਲ builtin_multi_pack_ਸੂਚਕਾਂਕ_compact_usage[] = {
	BUILTIN_MIDX_COMPACT_USAGE,
	ਨਿਰਾ
};
ਸਥਿਰ ਅੱਖਰ ਅਚਲ * ਅਚਲ builtin_multi_pack_ਸੂਚਕਾਂਕ_verify_usage[] = {
	BUILTIN_MIDX_VERIFY_USAGE,
	ਨਿਰਾ
};
ਸਥਿਰ ਅੱਖਰ ਅਚਲ * ਅਚਲ builtin_multi_pack_ਸੂਚਕਾਂਕ_expire_usage[] = {
	BUILTIN_MIDX_EXPIRE_USAGE,
	ਨਿਰਾ
};
ਸਥਿਰ ਅੱਖਰ ਅਚਲ * ਅਚਲ builtin_multi_pack_ਸੂਚਕਾਂਕ_repack_usage[] = {
	BUILTIN_MIDX_REPACK_USAGE,
	ਨਿਰਾ
};
ਸਥਿਰ ਅੱਖਰ ਅਚਲ * ਅਚਲ builtin_multi_pack_ਸੂਚਕਾਂਕ_usage[] = {
	BUILTIN_MIDX_WRITE_USAGE,
	BUILTIN_MIDX_COMPACT_USAGE,
	BUILTIN_MIDX_VERIFY_USAGE,
	BUILTIN_MIDX_EXPIRE_USAGE,
	BUILTIN_MIDX_REPACK_USAGE,
	ਨਿਰਾ
};

ਸਥਿਰ ਬਣਤਰ opts_multi_pack_ਸੂਚਕਾਂਕ {
	ਅੱਖਰ *object_dir;
	ਅਚਲ ਅੱਖਰ *preferred_pack;
	ਅਚਲ ਅੱਖਰ *incremental_ਅਧਾਰ;
	ਅੱਖਰ *refs_snapshot;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ batch_ਮਾਪ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ flags;
	ਪੂਰਨ stdin_packs;
} opts;


ਸਥਿਰ ਪੂਰਨ parse_object_dir(ਅਚਲ ਬਣਤਰ option *opt, ਅਚਲ ਅੱਖਰ *arg,
			    ਪੂਰਨ unset)
{
	ਅੱਖਰ **ਮੁੱਲ = opt->ਮੁੱਲ;
	ਮੁਕਤ(*ਮੁੱਲ);
	ਜੇ (unset)
		*ਮੁੱਲ = xstrdup(the_repository->objects->ਸਰੋਤ->ਰਸਤਾ);
	ਨਹੀਂਤਾਂ
		*ਮੁੱਲ = real_pathdup(arg, 1);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਬਣਤਰ odb_ਸਰੋਤ_packed *handle_object_dir_option(ਬਣਤਰ repository *repo)
{
	ਬਣਤਰ odb_files_dir *dir = odb_ਸਰੋਤ_files_ਲੱਭੋ_dir(repo->objects, opts.object_dir);
	ਜੇ (!dir)
		die(_("object directory is not an alternate of the current repository: '%s'"),
		    opts.object_dir);
	ਵਾਪਸ dir->packed;
}

ਸਥਿਰ ਬਣਤਰ option common_opts[] = {
	OPT_CALLBACK(0, "object-dir", &opts.object_dir,
	  N_("directory"),
	  N_("object directory containing set of packfile and pack-index pairs"),
	  parse_object_dir),
	OPT_ਦੋਆਧਾਰੀ_ਅੰਕ(0, "progress", &opts.flags, N_("force progress reporting"),
		MIDX_ਤਰੱਕੀ),
	OPT_ਸਿਰਾ(),
};

ਸਥਿਰ ਬਣਤਰ option *ਜੋੜੋ_common_options(ਬਣਤਰ option *prev)
{
	ਵਾਪਸ parse_options_concat(common_opts, prev);
}

ਸਥਿਰ ਪੂਰਨ git_multi_pack_ਸੂਚਕਾਂਕ_write_config(ਅਚਲ ਅੱਖਰ *var, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ,
					     ਅਚਲ ਬਣਤਰ config_context *ctx ਅਣਵਰਤਿਆ,
					     ਖਾਲੀ *cb ਅਣਵਰਤਿਆ)
{
	ਜੇ (!strcmp(var, "pack.writebitmaphashcache")) {
		ਜੇ (git_config_bool(var, ਮੁੱਲ))
			opts.flags |= MIDX_WRITE_BITMAP_HASH_CACHE;
		ਨਹੀਂਤਾਂ
			opts.flags &= ~MIDX_WRITE_BITMAP_HASH_CACHE;
	}

	ਜੇ (!strcmp(var, "pack.writebitmaplookuptable")) {
		ਜੇ (git_config_bool(var, ਮੁੱਲ))
			opts.flags |= MIDX_WRITE_BITMAP_LOOKUP_ਸਾਰਣੀ;
		ਨਹੀਂਤਾਂ
			opts.flags &= ~MIDX_WRITE_BITMAP_LOOKUP_ਸਾਰਣੀ;
	}

	/*
	 * We should never make a fall-back call to 'git_default_config', since
	 * this was already called in 'cmd_multi_pack_index()'.
	 */
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ read_packs_from_stdin(ਬਣਤਰ string_list *ਮੰਜ਼ਿਲ)
{
	ਬਣਤਰ strbuf buf = STRBUF_INIT;
	ਜਦੋਂ (strbuf_getline(&buf, stdin) != EOF)
		string_list_append(ਮੰਜ਼ਿਲ, buf.buf);
	string_list_ਛਾਂਟੋ(ਮੰਜ਼ਿਲ);

	strbuf_release(&buf);
}

ਸਥਿਰ ਪੂਰਨ cmd_multi_pack_ਸੂਚਕਾਂਕ_write(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv,
				      ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
				      ਬਣਤਰ repository *repo)
{
	ਬਣਤਰ option *options;
	ਸਥਿਰ ਬਣਤਰ option builtin_multi_pack_ਸੂਚਕਾਂਕ_write_options[] = {
		OPT_STRING(0, "preferred-pack", &opts.preferred_pack,
			   N_("preferred-pack"),
			   N_("pack for reuse when computing a multi-pack bitmap")),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ(0, "bitmap", &opts.flags, N_("write multi-pack bitmap"),
			MIDX_WRITE_BITMAP | MIDX_WRITE_REV_ਸੂਚਕਾਂਕ),
		OPT_STRING(0, "base", &opts.incremental_ਅਧਾਰ, N_("checksum"),
			   N_("base MIDX for incremental writes")),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ(0, "incremental", &opts.flags,
			N_("write a new incremental MIDX"), MIDX_WRITE_INCREMENTAL),
		OPT_NEGBIT(0, "write-chain-file", &opts.flags,
			N_("write the multi-pack-index chain file"),
			MIDX_WRITE_NO_CHAIN),
		OPT_BOOL(0, "stdin-packs", &opts.stdin_packs,
			 N_("write multi-pack index containing only given indexes")),
		OPT_FILENAME(0, "refs-snapshot", &opts.refs_snapshot,
			     N_("refs snapshot for selecting bitmap commits")),
		OPT_ਸਿਰਾ(),
	};
	ਬਣਤਰ odb_ਸਰੋਤ_packed *packed_ਸਰੋਤ;
	ਪੂਰਨ ret;

	opts.flags |= MIDX_WRITE_BITMAP_HASH_CACHE;

	repo_config(the_repository, git_multi_pack_ਸੂਚਕਾਂਕ_write_config, ਨਿਰਾ);

	options = ਜੋੜੋ_common_options(builtin_multi_pack_ਸੂਚਕਾਂਕ_write_options);

	trace2_cmd_ਬਹੁਲਕ(argv[0]);

	ਜੇ (isatty(2))
		opts.flags |= MIDX_ਤਰੱਕੀ;
	argc = parse_options(argc, argv, ਅਗੇਤਰ,
			     options, builtin_multi_pack_ਸੂਚਕਾਂਕ_write_usage,
			     0);
	ਜੇ (argc)
		usage_with_options(builtin_multi_pack_ਸੂਚਕਾਂਕ_write_usage,
				   options);

	ਜੇ (opts.flags & MIDX_WRITE_NO_CHAIN &&
	    !(opts.flags & MIDX_WRITE_INCREMENTAL)) {
		error(_("cannot use %s without %s"),
		      "--no-write-chain-file", "--incremental");
		usage_with_options(builtin_multi_pack_ਸੂਚਕਾਂਕ_write_usage,
				   options);
	}

	ਜੇ (opts.incremental_ਅਧਾਰ &&
	    !(opts.flags & MIDX_WRITE_NO_CHAIN)) {
		error(_("cannot use --base without --no-write-chain-file"));
		usage_with_options(builtin_multi_pack_ਸੂਚਕਾਂਕ_write_usage,
				   options);
	}

	packed_ਸਰੋਤ = handle_object_dir_option(repo);

	FREE_AND_NULL(options);

	ਜੇ (opts.stdin_packs) {
		ਬਣਤਰ string_list packs = STRING_LIST_INIT_DUP;

		read_packs_from_stdin(&packs);

		ret = write_midx_file_only(packed_ਸਰੋਤ, &packs,
					   opts.preferred_pack,
					   opts.refs_snapshot,
					   opts.incremental_ਅਧਾਰ, opts.flags);

		string_list_clear(&packs, 0);
		ਮੁਕਤ(opts.refs_snapshot);

		ਵਾਪਸ ret;

	}

	ret = write_midx_file(packed_ਸਰੋਤ, opts.preferred_pack,
			      opts.refs_snapshot, opts.flags);

	ਮੁਕਤ(opts.refs_snapshot);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ cmd_multi_pack_ਸੂਚਕਾਂਕ_compact(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv,
					ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
					ਬਣਤਰ repository *repo)
{
	ਬਣਤਰ multi_pack_ਸੂਚਕਾਂਕ *m, *cur;
	ਬਣਤਰ multi_pack_ਸੂਚਕਾਂਕ *from_midx = ਨਿਰਾ;
	ਬਣਤਰ multi_pack_ਸੂਚਕਾਂਕ *ਮੰਜ਼ਿਲ_midx = ਨਿਰਾ;
	ਬਣਤਰ odb_ਸਰੋਤ_packed *packed_ਸਰੋਤ;
	ਪੂਰਨ ret;

	ਬਣਤਰ option *options;
	ਸਥਿਰ ਬਣਤਰ option builtin_multi_pack_ਸੂਚਕਾਂਕ_compact_options[] = {
		OPT_STRING(0, "base", &opts.incremental_ਅਧਾਰ, N_("checksum"),
			   N_("base MIDX for incremental writes")),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ(0, "bitmap", &opts.flags, N_("write multi-pack bitmap"),
			MIDX_WRITE_BITMAP | MIDX_WRITE_REV_ਸੂਚਕਾਂਕ),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ(0, "incremental", &opts.flags,
			N_("write a new incremental MIDX"), MIDX_WRITE_INCREMENTAL),
		OPT_NEGBIT(0, "write-chain-file", &opts.flags,
			N_("write the multi-pack-index chain file"),
			MIDX_WRITE_NO_CHAIN),
		OPT_ਸਿਰਾ(),
	};

	repo_config(repo, git_multi_pack_ਸੂਚਕਾਂਕ_write_config, ਨਿਰਾ);

	options = ਜੋੜੋ_common_options(builtin_multi_pack_ਸੂਚਕਾਂਕ_compact_options);

	trace2_cmd_ਬਹੁਲਕ(argv[0]);

	ਜੇ (isatty(2))
		opts.flags |= MIDX_ਤਰੱਕੀ;
	argc = parse_options(argc, argv, ਅਗੇਤਰ,
			     options, builtin_multi_pack_ਸੂਚਕਾਂਕ_compact_usage,
			     0);

	ਜੇ (argc != 2)
		usage_with_options(builtin_multi_pack_ਸੂਚਕਾਂਕ_compact_usage,
				   options);

	ਜੇ (opts.flags & MIDX_WRITE_NO_CHAIN &&
	    !(opts.flags & MIDX_WRITE_INCREMENTAL)) {
		error(_("cannot use %s without %s"),
		      "--no-write-chain-file", "--incremental");
		usage_with_options(builtin_multi_pack_ਸੂਚਕਾਂਕ_compact_usage,
				   options);
	}

	packed_ਸਰੋਤ = handle_object_dir_option(the_repository);

	FREE_AND_NULL(options);

	m = get_multi_pack_ਸੂਚਕਾਂਕ(packed_ਸਰੋਤ);

	ਲਈ (cur = m; cur && !(from_midx && ਮੰਜ਼ਿਲ_midx); cur = cur->ਅਧਾਰ_midx) {
		ਅਚਲ ਅੱਖਰ *midx_csum = midx_get_checksum_hex(cur);

		ਜੇ (!from_midx && !strcmp(midx_csum, argv[0]))
			from_midx = cur;
		ਜੇ (!ਮੰਜ਼ਿਲ_midx && !strcmp(midx_csum, argv[1]))
			ਮੰਜ਼ਿਲ_midx = cur;
	}

	ਜੇ (!from_midx)
		die(_("could not find MIDX: %s"), argv[0]);
	ਜੇ (!ਮੰਜ਼ਿਲ_midx)
		die(_("could not find MIDX: %s"), argv[1]);
	ਜੇ (from_midx == ਮੰਜ਼ਿਲ_midx)
		die(_("MIDX compaction endpoints must be unique"));

	ਲਈ (m = from_midx; m; m = m->ਅਧਾਰ_midx) {
		ਜੇ (m == ਮੰਜ਼ਿਲ_midx)
			die(_("MIDX %s must be an ancestor of %s"), argv[0], argv[1]);
	}

	ret = write_midx_file_compact(packed_ਸਰੋਤ, from_midx, ਮੰਜ਼ਿਲ_midx,
				      opts.incremental_ਅਧਾਰ, opts.flags);

	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ cmd_multi_pack_ਸੂਚਕਾਂਕ_verify(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv,
				       ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
				       ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ option *options;
	ਸਥਿਰ ਬਣਤਰ option builtin_multi_pack_ਸੂਚਕਾਂਕ_verify_options[] = {
		OPT_ਸਿਰਾ(),
	};
	ਬਣਤਰ odb_ਸਰੋਤ_packed *packed_ਸਰੋਤ;

	options = ਜੋੜੋ_common_options(builtin_multi_pack_ਸੂਚਕਾਂਕ_verify_options);

	trace2_cmd_ਬਹੁਲਕ(argv[0]);

	ਜੇ (isatty(2))
		opts.flags |= MIDX_ਤਰੱਕੀ;
	argc = parse_options(argc, argv, ਅਗੇਤਰ,
			     options, builtin_multi_pack_ਸੂਚਕਾਂਕ_verify_usage,
			     0);
	ਜੇ (argc)
		usage_with_options(builtin_multi_pack_ਸੂਚਕਾਂਕ_verify_usage,
				   options);
	packed_ਸਰੋਤ = handle_object_dir_option(the_repository);

	FREE_AND_NULL(options);

	ਵਾਪਸ verify_midx_file(packed_ਸਰੋਤ, opts.flags);
}

ਸਥਿਰ ਪੂਰਨ cmd_multi_pack_ਸੂਚਕਾਂਕ_expire(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv,
				       ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
				       ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ option *options;
	ਸਥਿਰ ਬਣਤਰ option builtin_multi_pack_ਸੂਚਕਾਂਕ_expire_options[] = {
		OPT_ਸਿਰਾ(),
	};
	ਬਣਤਰ odb_ਸਰੋਤ_packed *packed_ਸਰੋਤ;

	options = ਜੋੜੋ_common_options(builtin_multi_pack_ਸੂਚਕਾਂਕ_expire_options);

	trace2_cmd_ਬਹੁਲਕ(argv[0]);

	ਜੇ (isatty(2))
		opts.flags |= MIDX_ਤਰੱਕੀ;
	argc = parse_options(argc, argv, ਅਗੇਤਰ,
			     options, builtin_multi_pack_ਸੂਚਕਾਂਕ_expire_usage,
			     0);
	ਜੇ (argc)
		usage_with_options(builtin_multi_pack_ਸੂਚਕਾਂਕ_expire_usage,
				   options);
	packed_ਸਰੋਤ = handle_object_dir_option(the_repository);

	FREE_AND_NULL(options);

	ਵਾਪਸ expire_midx_packs(packed_ਸਰੋਤ, opts.flags);
}

ਸਥਿਰ ਪੂਰਨ cmd_multi_pack_ਸੂਚਕਾਂਕ_repack(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv,
				       ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
				       ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ option *options;
	ਸਥਿਰ ਬਣਤਰ option builtin_multi_pack_ਸੂਚਕਾਂਕ_repack_options[] = {
		OPT_UNSIGNED(0, "batch-size", &opts.batch_ਮਾਪ,
		  N_("during repack, collect pack-files of smaller size into a batch that is larger than this size")),
		OPT_ਸਿਰਾ(),
	};
	ਬਣਤਰ odb_ਸਰੋਤ_packed *packed_ਸਰੋਤ;

	options = ਜੋੜੋ_common_options(builtin_multi_pack_ਸੂਚਕਾਂਕ_repack_options);

	trace2_cmd_ਬਹੁਲਕ(argv[0]);

	ਜੇ (isatty(2))
		opts.flags |= MIDX_ਤਰੱਕੀ;
	argc = parse_options(argc, argv, ਅਗੇਤਰ,
			     options,
			     builtin_multi_pack_ਸੂਚਕਾਂਕ_repack_usage,
			     0);
	ਜੇ (argc)
		usage_with_options(builtin_multi_pack_ਸੂਚਕਾਂਕ_repack_usage,
				   options);
	packed_ਸਰੋਤ = handle_object_dir_option(the_repository);

	FREE_AND_NULL(options);

	ਵਾਪਸ midx_repack(packed_ਸਰੋਤ, (ਮਾਪ_t)opts.batch_ਮਾਪ, opts.flags);
}

ਪੂਰਨ cmd_multi_pack_ਸੂਚਕਾਂਕ(ਪੂਰਨ argc,
			 ਅਚਲ ਅੱਖਰ **argv,
			 ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			 ਬਣਤਰ repository *repo)
{
	ਪੂਰਨ res;
	parse_opt_subcommand_fn *fn = ਨਿਰਾ;
	ਬਣਤਰ option builtin_multi_pack_ਸੂਚਕਾਂਕ_options[] = {
		OPT_SUBCOMMAND("repack", &fn, cmd_multi_pack_ਸੂਚਕਾਂਕ_repack),
		OPT_SUBCOMMAND("write", &fn, cmd_multi_pack_ਸੂਚਕਾਂਕ_write),
		OPT_SUBCOMMAND("compact", &fn, cmd_multi_pack_ਸੂਚਕਾਂਕ_compact),
		OPT_SUBCOMMAND("verify", &fn, cmd_multi_pack_ਸੂਚਕਾਂਕ_verify),
		OPT_SUBCOMMAND("expire", &fn, cmd_multi_pack_ਸੂਚਕਾਂਕ_expire),
		OPT_ਸਿਰਾ(),
	};
	ਬਣਤਰ option *options = parse_options_concat(builtin_multi_pack_ਸੂਚਕਾਂਕ_options, common_opts);

	disable_replace_refs();

	repo_config(the_repository, git_default_config, ਨਿਰਾ);

	ਜੇ (the_repository &&
	    the_repository->objects &&
	    the_repository->objects->ਸਰੋਤ)
		opts.object_dir = xstrdup(the_repository->objects->ਸਰੋਤ->ਰਸਤਾ);

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options,
			     builtin_multi_pack_ਸੂਚਕਾਂਕ_usage, 0);
	FREE_AND_NULL(options);

	res = fn(argc, argv, ਅਗੇਤਰ, repo);

	ਮੁਕਤ(opts.object_dir);
	ਵਾਪਸ res;
}
