#define USE_THE_REPOSITORY_VARIABLE
#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "abspath.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "midx.h"
#અન્તર્ભાવ "strbuf.h"
#અન્તર્ભાવ "trace2.h"
#અન્તર્ભાવ "odb.h"
#અન્તર્ભાવ "odb/source.h"
#અન્તર્ભાવ "odb/source-files.h"
#અન્તર્ભાવ "replace-object.h"
#અન્તર્ભાવ "repository.h"

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

સ્થિર અક્ષર અચલ * અચલ builtin_multi_pack_index_write_usage[] = {
	BUILTIN_MIDX_WRITE_USAGE,
	ખાલી
};
સ્થિર અક્ષર અચલ * અચલ builtin_multi_pack_index_compact_usage[] = {
	BUILTIN_MIDX_COMPACT_USAGE,
	ખાલી
};
સ્થિર અક્ષર અચલ * અચલ builtin_multi_pack_index_verify_usage[] = {
	BUILTIN_MIDX_VERIFY_USAGE,
	ખાલી
};
સ્થિર અક્ષર અચલ * અચલ builtin_multi_pack_index_expire_usage[] = {
	BUILTIN_MIDX_EXPIRE_USAGE,
	ખાલી
};
સ્થિર અક્ષર અચલ * અચલ builtin_multi_pack_index_repack_usage[] = {
	BUILTIN_MIDX_REPACK_USAGE,
	ખાલી
};
સ્થિર અક્ષર અચલ * અચલ builtin_multi_pack_index_usage[] = {
	BUILTIN_MIDX_WRITE_USAGE,
	BUILTIN_MIDX_COMPACT_USAGE,
	BUILTIN_MIDX_VERIFY_USAGE,
	BUILTIN_MIDX_EXPIRE_USAGE,
	BUILTIN_MIDX_REPACK_USAGE,
	ખાલી
};

સ્થિર સંરચના opts_multi_pack_index {
	અક્ષર *object_dir;
	અચલ અક્ષર *preferred_pack;
	અચલ અક્ષર *incremental_base;
	અક્ષર *refs_snapshot;
	unsigned long batch_size;
	unsigned flags;
	પૂર્ણાંક stdin_packs;
} opts;


સ્થિર પૂર્ણાંક parse_object_dir(અચલ સંરચના option *opt, અચલ અક્ષર *arg,
			    પૂર્ણાંક unset)
{
	અક્ષર **કિંમત = opt->કિંમત;
	મુક્ત(*કિંમત);
	જો (unset)
		*કિંમત = xstrdup(the_repository->objects->source->path);
	નહીંતર
		*કિંમત = real_pathdup(arg, 1);
	પાછા 0;
}

સ્થિર સંરચના odb_source_packed *handle_object_dir_option(સંરચના repository *repo)
{
	સંરચના odb_files_dir *dir = odb_source_files_find_dir(repo->objects, opts.object_dir);
	જો (!dir)
		die(_("object directory is not an alternate of the current repository: '%s'"),
		    opts.object_dir);
	પાછા dir->packed;
}

સ્થિર સંરચના option common_opts[] = {
	OPT_CALLBACK(0, "object-dir", &opts.object_dir,
	  N_("directory"),
	  N_("object directory containing set of packfile and pack-index pairs"),
	  parse_object_dir),
	OPT_BIT(0, "progress", &opts.flags, N_("force progress reporting"),
		MIDX_PROGRESS),
	OPT_END(),
};

સ્થિર સંરચના option *add_common_options(સંરચના option *prev)
{
	પાછા parse_options_concat(common_opts, prev);
}

સ્થિર પૂર્ણાંક git_multi_pack_index_write_config(અચલ અક્ષર *var, અચલ અક્ષર *કિંમત,
					     અચલ સંરચના config_context *ctx UNUSED,
					     શૂન્ય *cb UNUSED)
{
	જો (!strcmp(var, "pack.writebitmaphashcache")) {
		જો (git_config_bool(var, કિંમત))
			opts.flags |= MIDX_WRITE_BITMAP_HASH_CACHE;
		નહીંતર
			opts.flags &= ~MIDX_WRITE_BITMAP_HASH_CACHE;
	}

	જો (!strcmp(var, "pack.writebitmaplookuptable")) {
		જો (git_config_bool(var, કિંમત))
			opts.flags |= MIDX_WRITE_BITMAP_LOOKUP_TABLE;
		નહીંતર
			opts.flags &= ~MIDX_WRITE_BITMAP_LOOKUP_TABLE;
	}

	/*
	 * We should never make a fall-back call to 'git_default_config', since
	 * this was already called in 'cmd_multi_pack_index()'.
	 */
	પાછા 0;
}

સ્થિર શૂન્ય read_packs_from_stdin(સંરચના string_list *to)
{
	સંરચના strbuf buf = STRBUF_INIT;
	જ્યારે (strbuf_getline(&buf, stdin) != EOF)
		string_list_append(to, buf.buf);
	string_list_sort(to);

	strbuf_release(&buf);
}

સ્થિર પૂર્ણાંક cmd_multi_pack_index_write(પૂર્ણાંક argc, અચલ અક્ષર **argv,
				      અચલ અક્ષર *prefix,
				      સંરચના repository *repo)
{
	સંરચના option *options;
	સ્થિર સંરચના option builtin_multi_pack_index_write_options[] = {
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
	સંરચના odb_source_packed *packed_source;
	પૂર્ણાંક ret;

	opts.flags |= MIDX_WRITE_BITMAP_HASH_CACHE;

	repo_config(the_repository, git_multi_pack_index_write_config, ખાલી);

	options = add_common_options(builtin_multi_pack_index_write_options);

	trace2_cmd_mode(argv[0]);

	જો (isatty(2))
		opts.flags |= MIDX_PROGRESS;
	argc = parse_options(argc, argv, prefix,
			     options, builtin_multi_pack_index_write_usage,
			     0);
	જો (argc)
		usage_with_options(builtin_multi_pack_index_write_usage,
				   options);

	જો (opts.flags & MIDX_WRITE_NO_CHAIN &&
	    !(opts.flags & MIDX_WRITE_INCREMENTAL)) {
		error(_("cannot use %s without %s"),
		      "--no-write-chain-file", "--incremental");
		usage_with_options(builtin_multi_pack_index_write_usage,
				   options);
	}

	જો (opts.incremental_base &&
	    !(opts.flags & MIDX_WRITE_NO_CHAIN)) {
		error(_("cannot use --base without --no-write-chain-file"));
		usage_with_options(builtin_multi_pack_index_write_usage,
				   options);
	}

	packed_source = handle_object_dir_option(repo);

	FREE_AND_NULL(options);

	જો (opts.stdin_packs) {
		સંરચના string_list packs = STRING_LIST_INIT_DUP;

		read_packs_from_stdin(&packs);

		ret = write_midx_file_only(packed_source, &packs,
					   opts.preferred_pack,
					   opts.refs_snapshot,
					   opts.incremental_base, opts.flags);

		string_list_clear(&packs, 0);
		મુક્ત(opts.refs_snapshot);

		પાછા ret;

	}

	ret = write_midx_file(packed_source, opts.preferred_pack,
			      opts.refs_snapshot, opts.flags);

	મુક્ત(opts.refs_snapshot);
	પાછા ret;
}

સ્થિર પૂર્ણાંક cmd_multi_pack_index_compact(પૂર્ણાંક argc, અચલ અક્ષર **argv,
					અચલ અક્ષર *prefix,
					સંરચના repository *repo)
{
	સંરચના multi_pack_index *m, *cur;
	સંરચના multi_pack_index *from_midx = ખાલી;
	સંરચના multi_pack_index *to_midx = ખાલી;
	સંરચના odb_source_packed *packed_source;
	પૂર્ણાંક ret;

	સંરચના option *options;
	સ્થિર સંરચના option builtin_multi_pack_index_compact_options[] = {
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

	repo_config(repo, git_multi_pack_index_write_config, ખાલી);

	options = add_common_options(builtin_multi_pack_index_compact_options);

	trace2_cmd_mode(argv[0]);

	જો (isatty(2))
		opts.flags |= MIDX_PROGRESS;
	argc = parse_options(argc, argv, prefix,
			     options, builtin_multi_pack_index_compact_usage,
			     0);

	જો (argc != 2)
		usage_with_options(builtin_multi_pack_index_compact_usage,
				   options);

	જો (opts.flags & MIDX_WRITE_NO_CHAIN &&
	    !(opts.flags & MIDX_WRITE_INCREMENTAL)) {
		error(_("cannot use %s without %s"),
		      "--no-write-chain-file", "--incremental");
		usage_with_options(builtin_multi_pack_index_compact_usage,
				   options);
	}

	packed_source = handle_object_dir_option(the_repository);

	FREE_AND_NULL(options);

	m = get_multi_pack_index(packed_source);

	માટે (cur = m; cur && !(from_midx && to_midx); cur = cur->base_midx) {
		અચલ અક્ષર *midx_csum = midx_get_checksum_hex(cur);

		જો (!from_midx && !strcmp(midx_csum, argv[0]))
			from_midx = cur;
		જો (!to_midx && !strcmp(midx_csum, argv[1]))
			to_midx = cur;
	}

	જો (!from_midx)
		die(_("could not find MIDX: %s"), argv[0]);
	જો (!to_midx)
		die(_("could not find MIDX: %s"), argv[1]);
	જો (from_midx == to_midx)
		die(_("MIDX compaction endpoints must be unique"));

	માટે (m = from_midx; m; m = m->base_midx) {
		જો (m == to_midx)
			die(_("MIDX %s must be an ancestor of %s"), argv[0], argv[1]);
	}

	ret = write_midx_file_compact(packed_source, from_midx, to_midx,
				      opts.incremental_base, opts.flags);

	પાછા ret;
}

સ્થિર પૂર્ણાંક cmd_multi_pack_index_verify(પૂર્ણાંક argc, અચલ અક્ષર **argv,
				       અચલ અક્ષર *prefix,
				       સંરચના repository *repo UNUSED)
{
	સંરચના option *options;
	સ્થિર સંરચના option builtin_multi_pack_index_verify_options[] = {
		OPT_END(),
	};
	સંરચના odb_source_packed *packed_source;

	options = add_common_options(builtin_multi_pack_index_verify_options);

	trace2_cmd_mode(argv[0]);

	જો (isatty(2))
		opts.flags |= MIDX_PROGRESS;
	argc = parse_options(argc, argv, prefix,
			     options, builtin_multi_pack_index_verify_usage,
			     0);
	જો (argc)
		usage_with_options(builtin_multi_pack_index_verify_usage,
				   options);
	packed_source = handle_object_dir_option(the_repository);

	FREE_AND_NULL(options);

	પાછા verify_midx_file(packed_source, opts.flags);
}

સ્થિર પૂર્ણાંક cmd_multi_pack_index_expire(પૂર્ણાંક argc, અચલ અક્ષર **argv,
				       અચલ અક્ષર *prefix,
				       સંરચના repository *repo UNUSED)
{
	સંરચના option *options;
	સ્થિર સંરચના option builtin_multi_pack_index_expire_options[] = {
		OPT_END(),
	};
	સંરચના odb_source_packed *packed_source;

	options = add_common_options(builtin_multi_pack_index_expire_options);

	trace2_cmd_mode(argv[0]);

	જો (isatty(2))
		opts.flags |= MIDX_PROGRESS;
	argc = parse_options(argc, argv, prefix,
			     options, builtin_multi_pack_index_expire_usage,
			     0);
	જો (argc)
		usage_with_options(builtin_multi_pack_index_expire_usage,
				   options);
	packed_source = handle_object_dir_option(the_repository);

	FREE_AND_NULL(options);

	પાછા expire_midx_packs(packed_source, opts.flags);
}

સ્થિર પૂર્ણાંક cmd_multi_pack_index_repack(પૂર્ણાંક argc, અચલ અક્ષર **argv,
				       અચલ અક્ષર *prefix,
				       સંરચના repository *repo UNUSED)
{
	સંરચના option *options;
	સ્થિર સંરચના option builtin_multi_pack_index_repack_options[] = {
		OPT_UNSIGNED(0, "batch-size", &opts.batch_size,
		  N_("during repack, collect pack-files of smaller size into a batch that is larger than this size")),
		OPT_END(),
	};
	સંરચના odb_source_packed *packed_source;

	options = add_common_options(builtin_multi_pack_index_repack_options);

	trace2_cmd_mode(argv[0]);

	જો (isatty(2))
		opts.flags |= MIDX_PROGRESS;
	argc = parse_options(argc, argv, prefix,
			     options,
			     builtin_multi_pack_index_repack_usage,
			     0);
	જો (argc)
		usage_with_options(builtin_multi_pack_index_repack_usage,
				   options);
	packed_source = handle_object_dir_option(the_repository);

	FREE_AND_NULL(options);

	પાછા midx_repack(packed_source, (size_t)opts.batch_size, opts.flags);
}

પૂર્ણાંક cmd_multi_pack_index(પૂર્ણાંક argc,
			 અચલ અક્ષર **argv,
			 અચલ અક્ષર *prefix,
			 સંરચના repository *repo)
{
	પૂર્ણાંક res;
	parse_opt_subcommand_fn *fn = ખાલી;
	સંરચના option builtin_multi_pack_index_options[] = {
		OPT_SUBCOMMAND("repack", &fn, cmd_multi_pack_index_repack),
		OPT_SUBCOMMAND("write", &fn, cmd_multi_pack_index_write),
		OPT_SUBCOMMAND("compact", &fn, cmd_multi_pack_index_compact),
		OPT_SUBCOMMAND("verify", &fn, cmd_multi_pack_index_verify),
		OPT_SUBCOMMAND("expire", &fn, cmd_multi_pack_index_expire),
		OPT_END(),
	};
	સંરચના option *options = parse_options_concat(builtin_multi_pack_index_options, common_opts);

	disable_replace_refs();

	repo_config(the_repository, git_default_config, ખાલી);

	જો (the_repository &&
	    the_repository->objects &&
	    the_repository->objects->source)
		opts.object_dir = xstrdup(the_repository->objects->source->path);

	argc = parse_options(argc, argv, prefix, options,
			     builtin_multi_pack_index_usage, 0);
	FREE_AND_NULL(options);

	res = fn(argc, argv, prefix, repo);

	મુક્ત(opts.object_dir);
	પાછા res;
}
