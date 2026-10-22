#define USE_THE_REPOSITORY_VARIABLE
#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "abspath.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "midx.h"
#ଅନ୍ତର୍ଭାଵ "strbuf.h"
#ଅନ୍ତର୍ଭାଵ "trace2.h"
#ଅନ୍ତର୍ଭାଵ "odb.h"
#ଅନ୍ତର୍ଭାଵ "odb/source.h"
#ଅନ୍ତର୍ଭାଵ "odb/source-files.h"
#ଅନ୍ତର୍ଭାଵ "replace-object.h"
#ଅନ୍ତର୍ଭାଵ "repository.h"

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

ସ୍ଥିର ଅକ୍ଷର ଅଚଳ * ଅଚଳ builtin_multi_pack_index_write_usage[] = {
	BUILTIN_MIDX_WRITE_USAGE,
	ଖାଲି
};
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ * ଅଚଳ builtin_multi_pack_index_compact_usage[] = {
	BUILTIN_MIDX_COMPACT_USAGE,
	ଖାଲି
};
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ * ଅଚଳ builtin_multi_pack_index_verify_usage[] = {
	BUILTIN_MIDX_VERIFY_USAGE,
	ଖାଲି
};
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ * ଅଚଳ builtin_multi_pack_index_expire_usage[] = {
	BUILTIN_MIDX_EXPIRE_USAGE,
	ଖାଲି
};
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ * ଅଚଳ builtin_multi_pack_index_repack_usage[] = {
	BUILTIN_MIDX_REPACK_USAGE,
	ଖାଲି
};
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ * ଅଚଳ builtin_multi_pack_index_usage[] = {
	BUILTIN_MIDX_WRITE_USAGE,
	BUILTIN_MIDX_COMPACT_USAGE,
	BUILTIN_MIDX_VERIFY_USAGE,
	BUILTIN_MIDX_EXPIRE_USAGE,
	BUILTIN_MIDX_REPACK_USAGE,
	ଖାଲି
};

ସ୍ଥିର ସଂରଚନା opts_multi_pack_index {
	ଅକ୍ଷର *object_dir;
	ଅଚଳ ଅକ୍ଷର *preferred_pack;
	ଅଚଳ ଅକ୍ଷର *incremental_base;
	ଅକ୍ଷର *refs_snapshot;
	unsigned long batch_size;
	unsigned flags;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା stdin_packs;
} opts;


ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା parse_object_dir(ଅଚଳ ସଂରଚନା option *opt, ଅଚଳ ଅକ୍ଷର *arg,
			    ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ଅକ୍ଷର **ମୂଲ୍ୟ = opt->ମୂଲ୍ୟ;
	ମୁକ୍ତ(*ମୂଲ୍ୟ);
	ଯଦି (unset)
		*ମୂଲ୍ୟ = xstrdup(the_repository->objects->source->path);
	ଅନ୍ୟଥା
		*ମୂଲ୍ୟ = real_pathdup(arg, 1);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ସଂରଚନା odb_source_packed *handle_object_dir_option(ସଂରଚନା repository *repo)
{
	ସଂରଚନା odb_files_dir *dir = odb_source_files_find_dir(repo->objects, opts.object_dir);
	ଯଦି (!dir)
		die(_("object directory is not an alternate of the current repository: '%s'"),
		    opts.object_dir);
	ଫେରନ୍ତୁ dir->packed;
}

ସ୍ଥିର ସଂରଚନା option common_opts[] = {
	OPT_CALLBACK(0, "object-dir", &opts.object_dir,
	  N_("directory"),
	  N_("object directory containing set of packfile and pack-index pairs"),
	  parse_object_dir),
	OPT_BIT(0, "progress", &opts.flags, N_("force progress reporting"),
		MIDX_PROGRESS),
	OPT_END(),
};

ସ୍ଥିର ସଂରଚନା option *add_common_options(ସଂରଚନା option *prev)
{
	ଫେରନ୍ତୁ parse_options_concat(common_opts, prev);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା git_multi_pack_index_write_config(ଅଚଳ ଅକ୍ଷର *var, ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ,
					     ଅଚଳ ସଂରଚନା config_context *ctx UNUSED,
					     ଶୂନ୍ୟ *cb UNUSED)
{
	ଯଦି (!strcmp(var, "pack.writebitmaphashcache")) {
		ଯଦି (git_config_bool(var, ମୂଲ୍ୟ))
			opts.flags |= MIDX_WRITE_BITMAP_HASH_CACHE;
		ଅନ୍ୟଥା
			opts.flags &= ~MIDX_WRITE_BITMAP_HASH_CACHE;
	}

	ଯଦି (!strcmp(var, "pack.writebitmaplookuptable")) {
		ଯଦି (git_config_bool(var, ମୂଲ୍ୟ))
			opts.flags |= MIDX_WRITE_BITMAP_LOOKUP_TABLE;
		ଅନ୍ୟଥା
			opts.flags &= ~MIDX_WRITE_BITMAP_LOOKUP_TABLE;
	}

	/*
	 * We should never make a fall-back call to 'git_default_config', since
	 * this was already called in 'cmd_multi_pack_index()'.
	 */
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ read_packs_from_stdin(ସଂରଚନା string_list *to)
{
	ସଂରଚନା strbuf buf = STRBUF_INIT;
	ଯେତେବେଳେ (strbuf_getline(&buf, stdin) != EOF)
		string_list_append(to, buf.buf);
	string_list_sort(to);

	strbuf_release(&buf);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_multi_pack_index_write(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv,
				      ଅଚଳ ଅକ୍ଷର *prefix,
				      ସଂରଚନା repository *repo)
{
	ସଂରଚନା option *options;
	ସ୍ଥିର ସଂରଚନା option builtin_multi_pack_index_write_options[] = {
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
	ସଂରଚନା odb_source_packed *packed_source;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	opts.flags |= MIDX_WRITE_BITMAP_HASH_CACHE;

	repo_config(the_repository, git_multi_pack_index_write_config, ଖାଲି);

	options = add_common_options(builtin_multi_pack_index_write_options);

	trace2_cmd_mode(argv[0]);

	ଯଦି (isatty(2))
		opts.flags |= MIDX_PROGRESS;
	argc = parse_options(argc, argv, prefix,
			     options, builtin_multi_pack_index_write_usage,
			     0);
	ଯଦି (argc)
		usage_with_options(builtin_multi_pack_index_write_usage,
				   options);

	ଯଦି (opts.flags & MIDX_WRITE_NO_CHAIN &&
	    !(opts.flags & MIDX_WRITE_INCREMENTAL)) {
		error(_("cannot use %s without %s"),
		      "--no-write-chain-file", "--incremental");
		usage_with_options(builtin_multi_pack_index_write_usage,
				   options);
	}

	ଯଦି (opts.incremental_base &&
	    !(opts.flags & MIDX_WRITE_NO_CHAIN)) {
		error(_("cannot use --base without --no-write-chain-file"));
		usage_with_options(builtin_multi_pack_index_write_usage,
				   options);
	}

	packed_source = handle_object_dir_option(repo);

	FREE_AND_NULL(options);

	ଯଦି (opts.stdin_packs) {
		ସଂରଚନା string_list packs = STRING_LIST_INIT_DUP;

		read_packs_from_stdin(&packs);

		ret = write_midx_file_only(packed_source, &packs,
					   opts.preferred_pack,
					   opts.refs_snapshot,
					   opts.incremental_base, opts.flags);

		string_list_clear(&packs, 0);
		ମୁକ୍ତ(opts.refs_snapshot);

		ଫେରନ୍ତୁ ret;

	}

	ret = write_midx_file(packed_source, opts.preferred_pack,
			      opts.refs_snapshot, opts.flags);

	ମୁକ୍ତ(opts.refs_snapshot);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_multi_pack_index_compact(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv,
					ଅଚଳ ଅକ୍ଷର *prefix,
					ସଂରଚନା repository *repo)
{
	ସଂରଚନା multi_pack_index *m, *cur;
	ସଂରଚନା multi_pack_index *from_midx = ଖାଲି;
	ସଂରଚନା multi_pack_index *to_midx = ଖାଲି;
	ସଂରଚନା odb_source_packed *packed_source;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	ସଂରଚନା option *options;
	ସ୍ଥିର ସଂରଚନା option builtin_multi_pack_index_compact_options[] = {
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

	repo_config(repo, git_multi_pack_index_write_config, ଖାଲି);

	options = add_common_options(builtin_multi_pack_index_compact_options);

	trace2_cmd_mode(argv[0]);

	ଯଦି (isatty(2))
		opts.flags |= MIDX_PROGRESS;
	argc = parse_options(argc, argv, prefix,
			     options, builtin_multi_pack_index_compact_usage,
			     0);

	ଯଦି (argc != 2)
		usage_with_options(builtin_multi_pack_index_compact_usage,
				   options);

	ଯଦି (opts.flags & MIDX_WRITE_NO_CHAIN &&
	    !(opts.flags & MIDX_WRITE_INCREMENTAL)) {
		error(_("cannot use %s without %s"),
		      "--no-write-chain-file", "--incremental");
		usage_with_options(builtin_multi_pack_index_compact_usage,
				   options);
	}

	packed_source = handle_object_dir_option(the_repository);

	FREE_AND_NULL(options);

	m = get_multi_pack_index(packed_source);

	ପାଇଁ (cur = m; cur && !(from_midx && to_midx); cur = cur->base_midx) {
		ଅଚଳ ଅକ୍ଷର *midx_csum = midx_get_checksum_hex(cur);

		ଯଦି (!from_midx && !strcmp(midx_csum, argv[0]))
			from_midx = cur;
		ଯଦି (!to_midx && !strcmp(midx_csum, argv[1]))
			to_midx = cur;
	}

	ଯଦି (!from_midx)
		die(_("could not find MIDX: %s"), argv[0]);
	ଯଦି (!to_midx)
		die(_("could not find MIDX: %s"), argv[1]);
	ଯଦି (from_midx == to_midx)
		die(_("MIDX compaction endpoints must be unique"));

	ପାଇଁ (m = from_midx; m; m = m->base_midx) {
		ଯଦି (m == to_midx)
			die(_("MIDX %s must be an ancestor of %s"), argv[0], argv[1]);
	}

	ret = write_midx_file_compact(packed_source, from_midx, to_midx,
				      opts.incremental_base, opts.flags);

	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_multi_pack_index_verify(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv,
				       ଅଚଳ ଅକ୍ଷର *prefix,
				       ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା option *options;
	ସ୍ଥିର ସଂରଚନା option builtin_multi_pack_index_verify_options[] = {
		OPT_END(),
	};
	ସଂରଚନା odb_source_packed *packed_source;

	options = add_common_options(builtin_multi_pack_index_verify_options);

	trace2_cmd_mode(argv[0]);

	ଯଦି (isatty(2))
		opts.flags |= MIDX_PROGRESS;
	argc = parse_options(argc, argv, prefix,
			     options, builtin_multi_pack_index_verify_usage,
			     0);
	ଯଦି (argc)
		usage_with_options(builtin_multi_pack_index_verify_usage,
				   options);
	packed_source = handle_object_dir_option(the_repository);

	FREE_AND_NULL(options);

	ଫେରନ୍ତୁ verify_midx_file(packed_source, opts.flags);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_multi_pack_index_expire(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv,
				       ଅଚଳ ଅକ୍ଷର *prefix,
				       ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା option *options;
	ସ୍ଥିର ସଂରଚନା option builtin_multi_pack_index_expire_options[] = {
		OPT_END(),
	};
	ସଂରଚନା odb_source_packed *packed_source;

	options = add_common_options(builtin_multi_pack_index_expire_options);

	trace2_cmd_mode(argv[0]);

	ଯଦି (isatty(2))
		opts.flags |= MIDX_PROGRESS;
	argc = parse_options(argc, argv, prefix,
			     options, builtin_multi_pack_index_expire_usage,
			     0);
	ଯଦି (argc)
		usage_with_options(builtin_multi_pack_index_expire_usage,
				   options);
	packed_source = handle_object_dir_option(the_repository);

	FREE_AND_NULL(options);

	ଫେରନ୍ତୁ expire_midx_packs(packed_source, opts.flags);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_multi_pack_index_repack(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv,
				       ଅଚଳ ଅକ୍ଷର *prefix,
				       ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା option *options;
	ସ୍ଥିର ସଂରଚନା option builtin_multi_pack_index_repack_options[] = {
		OPT_UNSIGNED(0, "batch-size", &opts.batch_size,
		  N_("during repack, collect pack-files of smaller size into a batch that is larger than this size")),
		OPT_END(),
	};
	ସଂରଚନା odb_source_packed *packed_source;

	options = add_common_options(builtin_multi_pack_index_repack_options);

	trace2_cmd_mode(argv[0]);

	ଯଦି (isatty(2))
		opts.flags |= MIDX_PROGRESS;
	argc = parse_options(argc, argv, prefix,
			     options,
			     builtin_multi_pack_index_repack_usage,
			     0);
	ଯଦି (argc)
		usage_with_options(builtin_multi_pack_index_repack_usage,
				   options);
	packed_source = handle_object_dir_option(the_repository);

	FREE_AND_NULL(options);

	ଫେରନ୍ତୁ midx_repack(packed_source, (size_t)opts.batch_size, opts.flags);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_multi_pack_index(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
			 ଅଚଳ ଅକ୍ଷର **argv,
			 ଅଚଳ ଅକ୍ଷର *prefix,
			 ସଂରଚନା repository *repo)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା res;
	parse_opt_subcommand_fn *fn = ଖାଲି;
	ସଂରଚନା option builtin_multi_pack_index_options[] = {
		OPT_SUBCOMMAND("repack", &fn, cmd_multi_pack_index_repack),
		OPT_SUBCOMMAND("write", &fn, cmd_multi_pack_index_write),
		OPT_SUBCOMMAND("compact", &fn, cmd_multi_pack_index_compact),
		OPT_SUBCOMMAND("verify", &fn, cmd_multi_pack_index_verify),
		OPT_SUBCOMMAND("expire", &fn, cmd_multi_pack_index_expire),
		OPT_END(),
	};
	ସଂରଚନା option *options = parse_options_concat(builtin_multi_pack_index_options, common_opts);

	disable_replace_refs();

	repo_config(the_repository, git_default_config, ଖାଲି);

	ଯଦି (the_repository &&
	    the_repository->objects &&
	    the_repository->objects->source)
		opts.object_dir = xstrdup(the_repository->objects->source->path);

	argc = parse_options(argc, argv, prefix, options,
			     builtin_multi_pack_index_usage, 0);
	FREE_AND_NULL(options);

	res = fn(argc, argv, prefix, repo);

	ମୁକ୍ତ(opts.object_dir);
	ଫେରନ୍ତୁ res;
}
