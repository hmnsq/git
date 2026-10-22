#define USE_THE_REPOSITORY_VARIABLE
#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "abspath.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "midx.h"
#அந்தர்பாவ "strbuf.h"
#அந்தர்பாவ "trace2.h"
#அந்தர்பாவ "odb.h"
#அந்தர்பாவ "odb/source.h"
#அந்தர்பாவ "odb/source-files.h"
#அந்தர்பாவ "replace-object.h"
#அந்தர்பாவ "repository.h"

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

நிலையான எழுத்து மாறிலி * மாறிலி builtin_multi_pack_index_write_usage[] = {
	BUILTIN_MIDX_WRITE_USAGE,
	வெற்று
};
நிலையான எழுத்து மாறிலி * மாறிலி builtin_multi_pack_index_compact_usage[] = {
	BUILTIN_MIDX_COMPACT_USAGE,
	வெற்று
};
நிலையான எழுத்து மாறிலி * மாறிலி builtin_multi_pack_index_verify_usage[] = {
	BUILTIN_MIDX_VERIFY_USAGE,
	வெற்று
};
நிலையான எழுத்து மாறிலி * மாறிலி builtin_multi_pack_index_expire_usage[] = {
	BUILTIN_MIDX_EXPIRE_USAGE,
	வெற்று
};
நிலையான எழுத்து மாறிலி * மாறிலி builtin_multi_pack_index_repack_usage[] = {
	BUILTIN_MIDX_REPACK_USAGE,
	வெற்று
};
நிலையான எழுத்து மாறிலி * மாறிலி builtin_multi_pack_index_usage[] = {
	BUILTIN_MIDX_WRITE_USAGE,
	BUILTIN_MIDX_COMPACT_USAGE,
	BUILTIN_MIDX_VERIFY_USAGE,
	BUILTIN_MIDX_EXPIRE_USAGE,
	BUILTIN_MIDX_REPACK_USAGE,
	வெற்று
};

நிலையான கட்டமைப்பு opts_multi_pack_index {
	எழுத்து *object_dir;
	மாறிலி எழுத்து *preferred_pack;
	மாறிலி எழுத்து *incremental_base;
	எழுத்து *refs_snapshot;
	unsigned long batch_size;
	unsigned flags;
	முழு stdin_packs;
} opts;


நிலையான முழு parse_object_dir(மாறிலி கட்டமைப்பு option *opt, மாறிலி எழுத்து *arg,
			    முழு unset)
{
	எழுத்து **மதிப்பு = opt->மதிப்பு;
	விடுவி(*மதிப்பு);
	எனில் (unset)
		*மதிப்பு = xstrdup(the_repository->objects->source->path);
	இல்லையெனில்
		*மதிப்பு = real_pathdup(arg, 1);
	திரும்பு 0;
}

நிலையான கட்டமைப்பு odb_source_packed *handle_object_dir_option(கட்டமைப்பு repository *repo)
{
	கட்டமைப்பு odb_files_dir *dir = odb_source_files_find_dir(repo->objects, opts.object_dir);
	எனில் (!dir)
		die(_("object directory is not an alternate of the current repository: '%s'"),
		    opts.object_dir);
	திரும்பு dir->packed;
}

நிலையான கட்டமைப்பு option common_opts[] = {
	OPT_CALLBACK(0, "object-dir", &opts.object_dir,
	  N_("directory"),
	  N_("object directory containing set of packfile and pack-index pairs"),
	  parse_object_dir),
	OPT_BIT(0, "progress", &opts.flags, N_("force progress reporting"),
		MIDX_PROGRESS),
	OPT_END(),
};

நிலையான கட்டமைப்பு option *add_common_options(கட்டமைப்பு option *prev)
{
	திரும்பு parse_options_concat(common_opts, prev);
}

நிலையான முழு git_multi_pack_index_write_config(மாறிலி எழுத்து *var, மாறிலி எழுத்து *மதிப்பு,
					     மாறிலி கட்டமைப்பு config_context *ctx UNUSED,
					     வெறுமை *cb UNUSED)
{
	எனில் (!strcmp(var, "pack.writebitmaphashcache")) {
		எனில் (git_config_bool(var, மதிப்பு))
			opts.flags |= MIDX_WRITE_BITMAP_HASH_CACHE;
		இல்லையெனில்
			opts.flags &= ~MIDX_WRITE_BITMAP_HASH_CACHE;
	}

	எனில் (!strcmp(var, "pack.writebitmaplookuptable")) {
		எனில் (git_config_bool(var, மதிப்பு))
			opts.flags |= MIDX_WRITE_BITMAP_LOOKUP_TABLE;
		இல்லையெனில்
			opts.flags &= ~MIDX_WRITE_BITMAP_LOOKUP_TABLE;
	}

	/*
	 * We should never make a fall-back call to 'git_default_config', since
	 * this was already called in 'cmd_multi_pack_index()'.
	 */
	திரும்பு 0;
}

நிலையான வெறுமை read_packs_from_stdin(கட்டமைப்பு string_list *to)
{
	கட்டமைப்பு strbuf buf = STRBUF_INIT;
	போது (strbuf_getline(&buf, stdin) != EOF)
		string_list_append(to, buf.buf);
	string_list_sort(to);

	strbuf_release(&buf);
}

நிலையான முழு cmd_multi_pack_index_write(முழு argc, மாறிலி எழுத்து **argv,
				      மாறிலி எழுத்து *prefix,
				      கட்டமைப்பு repository *repo)
{
	கட்டமைப்பு option *options;
	நிலையான கட்டமைப்பு option builtin_multi_pack_index_write_options[] = {
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
	கட்டமைப்பு odb_source_packed *packed_source;
	முழு ret;

	opts.flags |= MIDX_WRITE_BITMAP_HASH_CACHE;

	repo_config(the_repository, git_multi_pack_index_write_config, வெற்று);

	options = add_common_options(builtin_multi_pack_index_write_options);

	trace2_cmd_mode(argv[0]);

	எனில் (isatty(2))
		opts.flags |= MIDX_PROGRESS;
	argc = parse_options(argc, argv, prefix,
			     options, builtin_multi_pack_index_write_usage,
			     0);
	எனில் (argc)
		usage_with_options(builtin_multi_pack_index_write_usage,
				   options);

	எனில் (opts.flags & MIDX_WRITE_NO_CHAIN &&
	    !(opts.flags & MIDX_WRITE_INCREMENTAL)) {
		error(_("cannot use %s without %s"),
		      "--no-write-chain-file", "--incremental");
		usage_with_options(builtin_multi_pack_index_write_usage,
				   options);
	}

	எனில் (opts.incremental_base &&
	    !(opts.flags & MIDX_WRITE_NO_CHAIN)) {
		error(_("cannot use --base without --no-write-chain-file"));
		usage_with_options(builtin_multi_pack_index_write_usage,
				   options);
	}

	packed_source = handle_object_dir_option(repo);

	FREE_AND_NULL(options);

	எனில் (opts.stdin_packs) {
		கட்டமைப்பு string_list packs = STRING_LIST_INIT_DUP;

		read_packs_from_stdin(&packs);

		ret = write_midx_file_only(packed_source, &packs,
					   opts.preferred_pack,
					   opts.refs_snapshot,
					   opts.incremental_base, opts.flags);

		string_list_clear(&packs, 0);
		விடுவி(opts.refs_snapshot);

		திரும்பு ret;

	}

	ret = write_midx_file(packed_source, opts.preferred_pack,
			      opts.refs_snapshot, opts.flags);

	விடுவி(opts.refs_snapshot);
	திரும்பு ret;
}

நிலையான முழு cmd_multi_pack_index_compact(முழு argc, மாறிலி எழுத்து **argv,
					மாறிலி எழுத்து *prefix,
					கட்டமைப்பு repository *repo)
{
	கட்டமைப்பு multi_pack_index *m, *cur;
	கட்டமைப்பு multi_pack_index *from_midx = வெற்று;
	கட்டமைப்பு multi_pack_index *to_midx = வெற்று;
	கட்டமைப்பு odb_source_packed *packed_source;
	முழு ret;

	கட்டமைப்பு option *options;
	நிலையான கட்டமைப்பு option builtin_multi_pack_index_compact_options[] = {
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

	repo_config(repo, git_multi_pack_index_write_config, வெற்று);

	options = add_common_options(builtin_multi_pack_index_compact_options);

	trace2_cmd_mode(argv[0]);

	எனில் (isatty(2))
		opts.flags |= MIDX_PROGRESS;
	argc = parse_options(argc, argv, prefix,
			     options, builtin_multi_pack_index_compact_usage,
			     0);

	எனில் (argc != 2)
		usage_with_options(builtin_multi_pack_index_compact_usage,
				   options);

	எனில் (opts.flags & MIDX_WRITE_NO_CHAIN &&
	    !(opts.flags & MIDX_WRITE_INCREMENTAL)) {
		error(_("cannot use %s without %s"),
		      "--no-write-chain-file", "--incremental");
		usage_with_options(builtin_multi_pack_index_compact_usage,
				   options);
	}

	packed_source = handle_object_dir_option(the_repository);

	FREE_AND_NULL(options);

	m = get_multi_pack_index(packed_source);

	க்கு (cur = m; cur && !(from_midx && to_midx); cur = cur->base_midx) {
		மாறிலி எழுத்து *midx_csum = midx_get_checksum_hex(cur);

		எனில் (!from_midx && !strcmp(midx_csum, argv[0]))
			from_midx = cur;
		எனில் (!to_midx && !strcmp(midx_csum, argv[1]))
			to_midx = cur;
	}

	எனில் (!from_midx)
		die(_("could not find MIDX: %s"), argv[0]);
	எனில் (!to_midx)
		die(_("could not find MIDX: %s"), argv[1]);
	எனில் (from_midx == to_midx)
		die(_("MIDX compaction endpoints must be unique"));

	க்கு (m = from_midx; m; m = m->base_midx) {
		எனில் (m == to_midx)
			die(_("MIDX %s must be an ancestor of %s"), argv[0], argv[1]);
	}

	ret = write_midx_file_compact(packed_source, from_midx, to_midx,
				      opts.incremental_base, opts.flags);

	திரும்பு ret;
}

நிலையான முழு cmd_multi_pack_index_verify(முழு argc, மாறிலி எழுத்து **argv,
				       மாறிலி எழுத்து *prefix,
				       கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு option *options;
	நிலையான கட்டமைப்பு option builtin_multi_pack_index_verify_options[] = {
		OPT_END(),
	};
	கட்டமைப்பு odb_source_packed *packed_source;

	options = add_common_options(builtin_multi_pack_index_verify_options);

	trace2_cmd_mode(argv[0]);

	எனில் (isatty(2))
		opts.flags |= MIDX_PROGRESS;
	argc = parse_options(argc, argv, prefix,
			     options, builtin_multi_pack_index_verify_usage,
			     0);
	எனில் (argc)
		usage_with_options(builtin_multi_pack_index_verify_usage,
				   options);
	packed_source = handle_object_dir_option(the_repository);

	FREE_AND_NULL(options);

	திரும்பு verify_midx_file(packed_source, opts.flags);
}

நிலையான முழு cmd_multi_pack_index_expire(முழு argc, மாறிலி எழுத்து **argv,
				       மாறிலி எழுத்து *prefix,
				       கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு option *options;
	நிலையான கட்டமைப்பு option builtin_multi_pack_index_expire_options[] = {
		OPT_END(),
	};
	கட்டமைப்பு odb_source_packed *packed_source;

	options = add_common_options(builtin_multi_pack_index_expire_options);

	trace2_cmd_mode(argv[0]);

	எனில் (isatty(2))
		opts.flags |= MIDX_PROGRESS;
	argc = parse_options(argc, argv, prefix,
			     options, builtin_multi_pack_index_expire_usage,
			     0);
	எனில் (argc)
		usage_with_options(builtin_multi_pack_index_expire_usage,
				   options);
	packed_source = handle_object_dir_option(the_repository);

	FREE_AND_NULL(options);

	திரும்பு expire_midx_packs(packed_source, opts.flags);
}

நிலையான முழு cmd_multi_pack_index_repack(முழு argc, மாறிலி எழுத்து **argv,
				       மாறிலி எழுத்து *prefix,
				       கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு option *options;
	நிலையான கட்டமைப்பு option builtin_multi_pack_index_repack_options[] = {
		OPT_UNSIGNED(0, "batch-size", &opts.batch_size,
		  N_("during repack, collect pack-files of smaller size into a batch that is larger than this size")),
		OPT_END(),
	};
	கட்டமைப்பு odb_source_packed *packed_source;

	options = add_common_options(builtin_multi_pack_index_repack_options);

	trace2_cmd_mode(argv[0]);

	எனில் (isatty(2))
		opts.flags |= MIDX_PROGRESS;
	argc = parse_options(argc, argv, prefix,
			     options,
			     builtin_multi_pack_index_repack_usage,
			     0);
	எனில் (argc)
		usage_with_options(builtin_multi_pack_index_repack_usage,
				   options);
	packed_source = handle_object_dir_option(the_repository);

	FREE_AND_NULL(options);

	திரும்பு midx_repack(packed_source, (size_t)opts.batch_size, opts.flags);
}

முழு cmd_multi_pack_index(முழு argc,
			 மாறிலி எழுத்து **argv,
			 மாறிலி எழுத்து *prefix,
			 கட்டமைப்பு repository *repo)
{
	முழு res;
	parse_opt_subcommand_fn *fn = வெற்று;
	கட்டமைப்பு option builtin_multi_pack_index_options[] = {
		OPT_SUBCOMMAND("repack", &fn, cmd_multi_pack_index_repack),
		OPT_SUBCOMMAND("write", &fn, cmd_multi_pack_index_write),
		OPT_SUBCOMMAND("compact", &fn, cmd_multi_pack_index_compact),
		OPT_SUBCOMMAND("verify", &fn, cmd_multi_pack_index_verify),
		OPT_SUBCOMMAND("expire", &fn, cmd_multi_pack_index_expire),
		OPT_END(),
	};
	கட்டமைப்பு option *options = parse_options_concat(builtin_multi_pack_index_options, common_opts);

	disable_replace_refs();

	repo_config(the_repository, git_default_config, வெற்று);

	எனில் (the_repository &&
	    the_repository->objects &&
	    the_repository->objects->source)
		opts.object_dir = xstrdup(the_repository->objects->source->path);

	argc = parse_options(argc, argv, prefix, options,
			     builtin_multi_pack_index_usage, 0);
	FREE_AND_NULL(options);

	res = fn(argc, argv, prefix, repo);

	விடுவி(opts.object_dir);
	திரும்பு res;
}
