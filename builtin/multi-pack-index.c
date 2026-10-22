#define USE_THE_REPOSITORY_VARIABLE
#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "abspath.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "midx.h"
#അന്തര്ഭാവ "strbuf.h"
#അന്തര്ഭാവ "trace2.h"
#അന്തര്ഭാവ "odb.h"
#അന്തര്ഭാവ "odb/source.h"
#അന്തര്ഭാവ "odb/source-files.h"
#അന്തര്ഭാവ "replace-object.h"
#അന്തര്ഭാവ "repository.h"

#define BUILTIN_MIDX_WRITE_USAGE \
	N_("git multi-pack-index [<options>] write [--preferred-pack=<pack>]\n" \
	   "  [--[no-]bitmap] [--[no-]incremental] [--[no-]stdin-packs]\n" \
	   "  [--refs-snapshot=<path>] [--[no-]write-chain-file]\n" \
	   "  [--base=<checksum>]")

#define BUILTIN_MIDX_COMPACT_USAGE \
	N_("git multi-pack-index [<options>] compact [--[no-]incremental]\n" \
	   "  [--[no-]bitmap] [--base=<checksum>] [--[no-]write-chain-file]\n" \
	   "  <from> <to>")

#define BUILTIN_MIDX_VERIFY_USAGE \
	N_("git multi-pack-index [<options>] verify")

#define BUILTIN_MIDX_EXPIRE_USAGE \
	N_("git multi-pack-index [<options>] expire")

#define BUILTIN_MIDX_REPACK_USAGE \
	N_("git multi-pack-index [<options>] repack [--batch-size=<size>]")

സ്ഥിതം അക്ഷരം സ്ഥിരം * സ്ഥിരം builtin_multi_pack_index_write_usage[] = {
	BUILTIN_MIDX_WRITE_USAGE,
	ശൂന്യ
};
സ്ഥിതം അക്ഷരം സ്ഥിരം * സ്ഥിരം builtin_multi_pack_index_compact_usage[] = {
	BUILTIN_MIDX_COMPACT_USAGE,
	ശൂന്യ
};
സ്ഥിതം അക്ഷരം സ്ഥിരം * സ്ഥിരം builtin_multi_pack_index_verify_usage[] = {
	BUILTIN_MIDX_VERIFY_USAGE,
	ശൂന്യ
};
സ്ഥിതം അക്ഷരം സ്ഥിരം * സ്ഥിരം builtin_multi_pack_index_expire_usage[] = {
	BUILTIN_MIDX_EXPIRE_USAGE,
	ശൂന്യ
};
സ്ഥിതം അക്ഷരം സ്ഥിരം * സ്ഥിരം builtin_multi_pack_index_repack_usage[] = {
	BUILTIN_MIDX_REPACK_USAGE,
	ശൂന്യ
};
സ്ഥിതം അക്ഷരം സ്ഥിരം * സ്ഥിരം builtin_multi_pack_index_usage[] = {
	BUILTIN_MIDX_WRITE_USAGE,
	BUILTIN_MIDX_COMPACT_USAGE,
	BUILTIN_MIDX_VERIFY_USAGE,
	BUILTIN_MIDX_EXPIRE_USAGE,
	BUILTIN_MIDX_REPACK_USAGE,
	ശൂന്യ
};

സ്ഥിതം ഘടന opts_multi_pack_index {
	അക്ഷരം *object_dir;
	സ്ഥിരം അക്ഷരം *preferred_pack;
	സ്ഥിരം അക്ഷരം *incremental_base;
	അക്ഷരം *refs_snapshot;
	unsigned long batch_size;
	unsigned flags;
	പൂർണ്ണസംഖ്യ stdin_packs;
} opts;


സ്ഥിതം പൂർണ്ണസംഖ്യ parse_object_dir(സ്ഥിരം ഘടന option *opt, സ്ഥിരം അക്ഷരം *arg,
			    പൂർണ്ണസംഖ്യ unset)
{
	അക്ഷരം **മൂല്യം = opt->മൂല്യം;
	സ്വതന്ത്രം(*മൂല്യം);
	എങ്കിൽ (unset)
		*മൂല്യം = xstrdup(the_repository->objects->source->path);
	അല്ലെങ്കിൽ
		*മൂല്യം = real_pathdup(arg, 1);
	തിരികെ 0;
}

സ്ഥിതം ഘടന odb_source_packed *handle_object_dir_option(ഘടന repository *repo)
{
	ഘടന odb_files_dir *dir = odb_source_files_find_dir(repo->objects, opts.object_dir);
	എങ്കിൽ (!dir)
		die(_("object directory is not an alternate of the current repository: '%s'"),
		    opts.object_dir);
	തിരികെ dir->packed;
}

സ്ഥിതം ഘടന option common_opts[] = {
	OPT_CALLBACK(0, "object-dir", &opts.object_dir,
	  N_("directory"),
	  N_("object directory containing set of packfile and pack-index pairs"),
	  parse_object_dir),
	OPT_BIT(0, "progress", &opts.flags, N_("force progress reporting"),
		MIDX_PROGRESS),
	OPT_END(),
};

സ്ഥിതം ഘടന option *add_common_options(ഘടന option *prev)
{
	തിരികെ parse_options_concat(common_opts, prev);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ git_multi_pack_index_write_config(സ്ഥിരം അക്ഷരം *var, സ്ഥിരം അക്ഷരം *മൂല്യം,
					     സ്ഥിരം ഘടന config_context *ctx UNUSED,
					     ശൂന്യം *cb UNUSED)
{
	എങ്കിൽ (!strcmp(var, "pack.writebitmaphashcache")) {
		എങ്കിൽ (git_config_bool(var, മൂല്യം))
			opts.flags |= MIDX_WRITE_BITMAP_HASH_CACHE;
		അല്ലെങ്കിൽ
			opts.flags &= ~MIDX_WRITE_BITMAP_HASH_CACHE;
	}

	എങ്കിൽ (!strcmp(var, "pack.writebitmaplookuptable")) {
		എങ്കിൽ (git_config_bool(var, മൂല്യം))
			opts.flags |= MIDX_WRITE_BITMAP_LOOKUP_TABLE;
		അല്ലെങ്കിൽ
			opts.flags &= ~MIDX_WRITE_BITMAP_LOOKUP_TABLE;
	}

	/*
	 * We should never make a fall-back call to 'git_default_config', since
	 * this was already called in 'cmd_multi_pack_index()'.
	 */
	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം read_packs_from_stdin(ഘടന string_list *to)
{
	ഘടന strbuf buf = STRBUF_INIT;
	ആകുമ്പോൾ (strbuf_getline(&buf, stdin) != EOF)
		string_list_append(to, buf.buf);
	string_list_sort(to);

	strbuf_release(&buf);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cmd_multi_pack_index_write(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv,
				      സ്ഥിരം അക്ഷരം *prefix,
				      ഘടന repository *repo)
{
	ഘടന option *options;
	സ്ഥിതം ഘടന option builtin_multi_pack_index_write_options[] = {
		OPT_STRING(0, "preferred-pack", &opts.preferred_pack,
			   N_("preferred-pack"),
			   N_("pack for reuse when computing a multi-pack bitmap")),
		OPT_BIT(0, "bitmap", &opts.flags, N_("write multi-pack bitmap"),
			MIDX_WRITE_BITMAP | MIDX_WRITE_REV_INDEX),
		OPT_STRING(0, "base", &opts.incremental_base, N_("checksum"),
			   N_("base MIDX for incremental writes")),
		OPT_BIT(0, "incremental", &opts.flags,
			N_("write a new incremental MIDX"), MIDX_WRITE_INCREMENTAL),
		OPT_NEGBIT(0, "write-chain-file", &opts.flags,
			N_("write the multi-pack-index chain file"),
			MIDX_WRITE_NO_CHAIN),
		OPT_BOOL(0, "stdin-packs", &opts.stdin_packs,
			 N_("write multi-pack index containing only given indexes")),
		OPT_FILENAME(0, "refs-snapshot", &opts.refs_snapshot,
			     N_("refs snapshot for selecting bitmap commits")),
		OPT_END(),
	};
	ഘടന odb_source_packed *packed_source;
	പൂർണ്ണസംഖ്യ ret;

	opts.flags |= MIDX_WRITE_BITMAP_HASH_CACHE;

	repo_config(the_repository, git_multi_pack_index_write_config, ശൂന്യ);

	options = add_common_options(builtin_multi_pack_index_write_options);

	trace2_cmd_mode(argv[0]);

	എങ്കിൽ (isatty(2))
		opts.flags |= MIDX_PROGRESS;
	argc = parse_options(argc, argv, prefix,
			     options, builtin_multi_pack_index_write_usage,
			     0);
	എങ്കിൽ (argc)
		usage_with_options(builtin_multi_pack_index_write_usage,
				   options);

	എങ്കിൽ (opts.flags & MIDX_WRITE_NO_CHAIN &&
	    !(opts.flags & MIDX_WRITE_INCREMENTAL)) {
		error(_("cannot use %s without %s"),
		      "--no-write-chain-file", "--incremental");
		usage_with_options(builtin_multi_pack_index_write_usage,
				   options);
	}

	എങ്കിൽ (opts.incremental_base &&
	    !(opts.flags & MIDX_WRITE_NO_CHAIN)) {
		error(_("cannot use --base without --no-write-chain-file"));
		usage_with_options(builtin_multi_pack_index_write_usage,
				   options);
	}

	packed_source = handle_object_dir_option(repo);

	FREE_AND_NULL(options);

	എങ്കിൽ (opts.stdin_packs) {
		ഘടന string_list packs = STRING_LIST_INIT_DUP;

		read_packs_from_stdin(&packs);

		ret = write_midx_file_only(packed_source, &packs,
					   opts.preferred_pack,
					   opts.refs_snapshot,
					   opts.incremental_base, opts.flags);

		string_list_clear(&packs, 0);
		സ്വതന്ത്രം(opts.refs_snapshot);

		തിരികെ ret;

	}

	ret = write_midx_file(packed_source, opts.preferred_pack,
			      opts.refs_snapshot, opts.flags);

	സ്വതന്ത്രം(opts.refs_snapshot);
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cmd_multi_pack_index_compact(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv,
					സ്ഥിരം അക്ഷരം *prefix,
					ഘടന repository *repo)
{
	ഘടന multi_pack_index *m, *cur;
	ഘടന multi_pack_index *from_midx = ശൂന്യ;
	ഘടന multi_pack_index *to_midx = ശൂന്യ;
	ഘടന odb_source_packed *packed_source;
	പൂർണ്ണസംഖ്യ ret;

	ഘടന option *options;
	സ്ഥിതം ഘടന option builtin_multi_pack_index_compact_options[] = {
		OPT_STRING(0, "base", &opts.incremental_base, N_("checksum"),
			   N_("base MIDX for incremental writes")),
		OPT_BIT(0, "bitmap", &opts.flags, N_("write multi-pack bitmap"),
			MIDX_WRITE_BITMAP | MIDX_WRITE_REV_INDEX),
		OPT_BIT(0, "incremental", &opts.flags,
			N_("write a new incremental MIDX"), MIDX_WRITE_INCREMENTAL),
		OPT_NEGBIT(0, "write-chain-file", &opts.flags,
			N_("write the multi-pack-index chain file"),
			MIDX_WRITE_NO_CHAIN),
		OPT_END(),
	};

	repo_config(repo, git_multi_pack_index_write_config, ശൂന്യ);

	options = add_common_options(builtin_multi_pack_index_compact_options);

	trace2_cmd_mode(argv[0]);

	എങ്കിൽ (isatty(2))
		opts.flags |= MIDX_PROGRESS;
	argc = parse_options(argc, argv, prefix,
			     options, builtin_multi_pack_index_compact_usage,
			     0);

	എങ്കിൽ (argc != 2)
		usage_with_options(builtin_multi_pack_index_compact_usage,
				   options);

	എങ്കിൽ (opts.flags & MIDX_WRITE_NO_CHAIN &&
	    !(opts.flags & MIDX_WRITE_INCREMENTAL)) {
		error(_("cannot use %s without %s"),
		      "--no-write-chain-file", "--incremental");
		usage_with_options(builtin_multi_pack_index_compact_usage,
				   options);
	}

	packed_source = handle_object_dir_option(the_repository);

	FREE_AND_NULL(options);

	m = get_multi_pack_index(packed_source);

	വേണ്ടി (cur = m; cur && !(from_midx && to_midx); cur = cur->base_midx) {
		സ്ഥിരം അക്ഷരം *midx_csum = midx_get_checksum_hex(cur);

		എങ്കിൽ (!from_midx && !strcmp(midx_csum, argv[0]))
			from_midx = cur;
		എങ്കിൽ (!to_midx && !strcmp(midx_csum, argv[1]))
			to_midx = cur;
	}

	എങ്കിൽ (!from_midx)
		die(_("could not find MIDX: %s"), argv[0]);
	എങ്കിൽ (!to_midx)
		die(_("could not find MIDX: %s"), argv[1]);
	എങ്കിൽ (from_midx == to_midx)
		die(_("MIDX compaction endpoints must be unique"));

	വേണ്ടി (m = from_midx; m; m = m->base_midx) {
		എങ്കിൽ (m == to_midx)
			die(_("MIDX %s must be an ancestor of %s"), argv[0], argv[1]);
	}

	ret = write_midx_file_compact(packed_source, from_midx, to_midx,
				      opts.incremental_base, opts.flags);

	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cmd_multi_pack_index_verify(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv,
				       സ്ഥിരം അക്ഷരം *prefix,
				       ഘടന repository *repo UNUSED)
{
	ഘടന option *options;
	സ്ഥിതം ഘടന option builtin_multi_pack_index_verify_options[] = {
		OPT_END(),
	};
	ഘടന odb_source_packed *packed_source;

	options = add_common_options(builtin_multi_pack_index_verify_options);

	trace2_cmd_mode(argv[0]);

	എങ്കിൽ (isatty(2))
		opts.flags |= MIDX_PROGRESS;
	argc = parse_options(argc, argv, prefix,
			     options, builtin_multi_pack_index_verify_usage,
			     0);
	എങ്കിൽ (argc)
		usage_with_options(builtin_multi_pack_index_verify_usage,
				   options);
	packed_source = handle_object_dir_option(the_repository);

	FREE_AND_NULL(options);

	തിരികെ verify_midx_file(packed_source, opts.flags);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cmd_multi_pack_index_expire(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv,
				       സ്ഥിരം അക്ഷരം *prefix,
				       ഘടന repository *repo UNUSED)
{
	ഘടന option *options;
	സ്ഥിതം ഘടന option builtin_multi_pack_index_expire_options[] = {
		OPT_END(),
	};
	ഘടന odb_source_packed *packed_source;

	options = add_common_options(builtin_multi_pack_index_expire_options);

	trace2_cmd_mode(argv[0]);

	എങ്കിൽ (isatty(2))
		opts.flags |= MIDX_PROGRESS;
	argc = parse_options(argc, argv, prefix,
			     options, builtin_multi_pack_index_expire_usage,
			     0);
	എങ്കിൽ (argc)
		usage_with_options(builtin_multi_pack_index_expire_usage,
				   options);
	packed_source = handle_object_dir_option(the_repository);

	FREE_AND_NULL(options);

	തിരികെ expire_midx_packs(packed_source, opts.flags);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cmd_multi_pack_index_repack(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv,
				       സ്ഥിരം അക്ഷരം *prefix,
				       ഘടന repository *repo UNUSED)
{
	ഘടന option *options;
	സ്ഥിതം ഘടന option builtin_multi_pack_index_repack_options[] = {
		OPT_UNSIGNED(0, "batch-size", &opts.batch_size,
		  N_("during repack, collect pack-files of smaller size into a batch that is larger than this size")),
		OPT_END(),
	};
	ഘടന odb_source_packed *packed_source;

	options = add_common_options(builtin_multi_pack_index_repack_options);

	trace2_cmd_mode(argv[0]);

	എങ്കിൽ (isatty(2))
		opts.flags |= MIDX_PROGRESS;
	argc = parse_options(argc, argv, prefix,
			     options,
			     builtin_multi_pack_index_repack_usage,
			     0);
	എങ്കിൽ (argc)
		usage_with_options(builtin_multi_pack_index_repack_usage,
				   options);
	packed_source = handle_object_dir_option(the_repository);

	FREE_AND_NULL(options);

	തിരികെ midx_repack(packed_source, (size_t)opts.batch_size, opts.flags);
}

പൂർണ്ണസംഖ്യ cmd_multi_pack_index(പൂർണ്ണസംഖ്യ argc,
			 സ്ഥിരം അക്ഷരം **argv,
			 സ്ഥിരം അക്ഷരം *prefix,
			 ഘടന repository *repo)
{
	പൂർണ്ണസംഖ്യ res;
	parse_opt_subcommand_fn *fn = ശൂന്യ;
	ഘടന option builtin_multi_pack_index_options[] = {
		OPT_SUBCOMMAND("repack", &fn, cmd_multi_pack_index_repack),
		OPT_SUBCOMMAND("write", &fn, cmd_multi_pack_index_write),
		OPT_SUBCOMMAND("compact", &fn, cmd_multi_pack_index_compact),
		OPT_SUBCOMMAND("verify", &fn, cmd_multi_pack_index_verify),
		OPT_SUBCOMMAND("expire", &fn, cmd_multi_pack_index_expire),
		OPT_END(),
	};
	ഘടന option *options = parse_options_concat(builtin_multi_pack_index_options, common_opts);

	disable_replace_refs();

	repo_config(the_repository, git_default_config, ശൂന്യ);

	എങ്കിൽ (the_repository &&
	    the_repository->objects &&
	    the_repository->objects->source)
		opts.object_dir = xstrdup(the_repository->objects->source->path);

	argc = parse_options(argc, argv, prefix, options,
			     builtin_multi_pack_index_usage, 0);
	FREE_AND_NULL(options);

	res = fn(argc, argv, prefix, repo);

	സ്വതന്ത്രം(opts.object_dir);
	തിരികെ res;
}
