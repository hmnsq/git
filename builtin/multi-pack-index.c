#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE
#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "abspath.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "midx.h"
#ಅನ್ತರ್ಭಾವ "strbuf.h"
#ಅನ್ತರ್ಭಾವ "trace2.h"
#ಅನ್ತರ್ಭಾವ "odb.h"
#ಅನ್ತರ್ಭಾವ "odb/source.h"
#ಅನ್ತರ್ಭಾವ "odb/source-files.h"
#ಅನ್ತರ್ಭಾವ "replace-object.h"
#ಅನ್ತರ್ಭಾವ "repository.h"

#ವ್ಯಾಖ್ಯಾನಿಸು BUILTIN_MIDX_WRITE_USAGE \
	N_("git multi-pack-index [<options>] write [--preferred-pack=<pack>]\n" \
	   "  [--[no-]bitmap] [--[no-]incremental] [--[no-]stdin-packs]\n" \
	   "  [--refs-snapshot=<path>] [--[no-]write-chain-file]\n" \
	   "  [--base=<checksum>]")

#ವ್ಯಾಖ್ಯಾನಿಸು BUILTIN_MIDX_COMPACT_USAGE \
	N_("git multi-pack-index [<options>] compact [--[no-]incremental]\n" \
	   "  [--[no-]bitmap] [--base=<checksum>] [--[no-]write-chain-file]\n" \
	   "  <from> <to>")

#ವ್ಯಾಖ್ಯಾನಿಸು BUILTIN_MIDX_VERIFY_USAGE \
	N_("git multi-pack-index [<options>] verify")

#ವ್ಯಾಖ್ಯಾನಿಸು BUILTIN_MIDX_EXPIRE_USAGE \
	N_("git multi-pack-index [<options>] expire")

#ವ್ಯಾಖ್ಯಾನಿಸು BUILTIN_MIDX_REPACK_USAGE \
	N_("git multi-pack-index [<options>] repack [--batch-size=<size>]")

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ * ಸ್ಥಿರಾಂಕ builtin_multi_pack_ಸೂಚ್ಯಂಕ_write_usage[] = {
	BUILTIN_MIDX_WRITE_USAGE,
	ಖಾಲಿ
};
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ * ಸ್ಥಿರಾಂಕ builtin_multi_pack_ಸೂಚ್ಯಂಕ_compact_usage[] = {
	BUILTIN_MIDX_COMPACT_USAGE,
	ಖಾಲಿ
};
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ * ಸ್ಥಿರಾಂಕ builtin_multi_pack_ಸೂಚ್ಯಂಕ_verify_usage[] = {
	BUILTIN_MIDX_VERIFY_USAGE,
	ಖಾಲಿ
};
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ * ಸ್ಥಿರಾಂಕ builtin_multi_pack_ಸೂಚ್ಯಂಕ_expire_usage[] = {
	BUILTIN_MIDX_EXPIRE_USAGE,
	ಖಾಲಿ
};
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ * ಸ್ಥಿರಾಂಕ builtin_multi_pack_ಸೂಚ್ಯಂಕ_repack_usage[] = {
	BUILTIN_MIDX_REPACK_USAGE,
	ಖಾಲಿ
};
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ * ಸ್ಥಿರಾಂಕ builtin_multi_pack_ಸೂಚ್ಯಂಕ_usage[] = {
	BUILTIN_MIDX_WRITE_USAGE,
	BUILTIN_MIDX_COMPACT_USAGE,
	BUILTIN_MIDX_VERIFY_USAGE,
	BUILTIN_MIDX_EXPIRE_USAGE,
	BUILTIN_MIDX_REPACK_USAGE,
	ಖಾಲಿ
};

ಸ್ಥಿರ ರಚನೆ opts_multi_pack_ಸೂಚ್ಯಂಕ {
	ಅಕ್ಷರ *object_dir;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *preferred_pack;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *incremental_ತಳ;
	ಅಕ್ಷರ *refs_snapshot;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ batch_ಗಾತ್ರ;
	ಚಿಹ್ನೆರಹಿತ flags;
	ಪೂರ್ಣಾಂಕ stdin_packs;
} opts;


ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ parse_object_dir(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg,
			    ಪೂರ್ಣಾಂಕ unset)
{
	ಅಕ್ಷರ **ಮೌಲ್ಯ = opt->ಮೌಲ್ಯ;
	ಬಿಡುಗಡೆ(*ಮೌಲ್ಯ);
	ಆದರೆ (unset)
		*ಮೌಲ್ಯ = xstrdup(the_repository->objects->ಮೂಲ->ಮಾರ್ಗ);
	ಇಲ್ಲದಿದ್ದರೆ
		*ಮೌಲ್ಯ = real_pathdup(arg, 1);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ರಚನೆ odb_ಮೂಲ_packed *handle_object_dir_option(ರಚನೆ repository *repo)
{
	ರಚನೆ odb_files_dir *dir = odb_ಮೂಲ_files_ಕಂಡುಹಿಡಿ_dir(repo->objects, opts.object_dir);
	ಆದರೆ (!dir)
		die(_("object directory is not an alternate of the current repository: '%s'"),
		    opts.object_dir);
	ಹಿಂತಿರುಗಿ dir->packed;
}

ಸ್ಥಿರ ರಚನೆ option common_opts[] = {
	OPT_CALLBACK(0, "object-dir", &opts.object_dir,
	  N_("directory"),
	  N_("object directory containing set of packfile and pack-index pairs"),
	  parse_object_dir),
	OPT_ಬಿಟ್(0, "progress", &opts.flags, N_("force progress reporting"),
		MIDX_ಪ್ರಗತಿ),
	OPT_ಕೊನೆ(),
};

ಸ್ಥಿರ ರಚನೆ option *ಕೂಡಿಸು_common_options(ರಚನೆ option *prev)
{
	ಹಿಂತಿರುಗಿ parse_options_concat(common_opts, prev);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ git_multi_pack_ಸೂಚ್ಯಂಕ_write_config(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *var, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ,
					     ಸ್ಥಿರಾಂಕ ರಚನೆ config_context *ctx ಬಳಸದ,
					     ಶೂನ್ಯ *cb ಬಳಸದ)
{
	ಆದರೆ (!strcmp(var, "pack.writebitmaphashcache")) {
		ಆದರೆ (git_config_bool(var, ಮೌಲ್ಯ))
			opts.flags |= MIDX_WRITE_BITMAP_HASH_CACHE;
		ಇಲ್ಲದಿದ್ದರೆ
			opts.flags &= ~MIDX_WRITE_BITMAP_HASH_CACHE;
	}

	ಆದರೆ (!strcmp(var, "pack.writebitmaplookuptable")) {
		ಆದರೆ (git_config_bool(var, ಮೌಲ್ಯ))
			opts.flags |= MIDX_WRITE_BITMAP_LOOKUP_ಕೋಷ್ಟಕ;
		ಇಲ್ಲದಿದ್ದರೆ
			opts.flags &= ~MIDX_WRITE_BITMAP_LOOKUP_ಕೋಷ್ಟಕ;
	}

	/*
	 * We should never make a fall-back call to 'git_default_config', since
	 * this was already called in 'cmd_multi_pack_index()'.
	 */
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ read_packs_from_stdin(ರಚನೆ string_list *ಗಮ್ಯ)
{
	ರಚನೆ strbuf buf = STRBUF_INIT;
	ಆಗ (strbuf_getline(&buf, stdin) != EOF)
		string_list_append(ಗಮ್ಯ, buf.buf);
	string_list_ವಿಂಗಡಿಸು(ಗಮ್ಯ);

	strbuf_release(&buf);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cmd_multi_pack_ಸೂಚ್ಯಂಕ_write(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
				      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
				      ರಚನೆ repository *repo)
{
	ರಚನೆ option *options;
	ಸ್ಥಿರ ರಚನೆ option builtin_multi_pack_ಸೂಚ್ಯಂಕ_write_options[] = {
		OPT_STRING(0, "preferred-pack", &opts.preferred_pack,
			   N_("preferred-pack"),
			   N_("pack for reuse when computing a multi-pack bitmap")),
		OPT_ಬಿಟ್(0, "bitmap", &opts.flags, N_("write multi-pack bitmap"),
			MIDX_WRITE_BITMAP | MIDX_WRITE_REV_ಸೂಚ್ಯಂಕ),
		OPT_STRING(0, "base", &opts.incremental_ತಳ, N_("checksum"),
			   N_("base MIDX for incremental writes")),
		OPT_ಬಿಟ್(0, "incremental", &opts.flags,
			N_("write a new incremental MIDX"), MIDX_WRITE_INCREMENTAL),
		OPT_NEGBIT(0, "write-chain-file", &opts.flags,
			N_("write the multi-pack-index chain file"),
			MIDX_WRITE_NO_CHAIN),
		OPT_BOOL(0, "stdin-packs", &opts.stdin_packs,
			 N_("write multi-pack index containing only given indexes")),
		OPT_FILENAME(0, "refs-snapshot", &opts.refs_snapshot,
			     N_("refs snapshot for selecting bitmap commits")),
		OPT_ಕೊನೆ(),
	};
	ರಚನೆ odb_ಮೂಲ_packed *packed_ಮೂಲ;
	ಪೂರ್ಣಾಂಕ ret;

	opts.flags |= MIDX_WRITE_BITMAP_HASH_CACHE;

	repo_config(the_repository, git_multi_pack_ಸೂಚ್ಯಂಕ_write_config, ಖಾಲಿ);

	options = ಕೂಡಿಸು_common_options(builtin_multi_pack_ಸೂಚ್ಯಂಕ_write_options);

	trace2_cmd_ಬಹುಲಕ(argv[0]);

	ಆದರೆ (isatty(2))
		opts.flags |= MIDX_ಪ್ರಗತಿ;
	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ,
			     options, builtin_multi_pack_ಸೂಚ್ಯಂಕ_write_usage,
			     0);
	ಆದರೆ (argc)
		usage_with_options(builtin_multi_pack_ಸೂಚ್ಯಂಕ_write_usage,
				   options);

	ಆದರೆ (opts.flags & MIDX_WRITE_NO_CHAIN &&
	    !(opts.flags & MIDX_WRITE_INCREMENTAL)) {
		error(_("cannot use %s without %s"),
		      "--no-write-chain-file", "--incremental");
		usage_with_options(builtin_multi_pack_ಸೂಚ್ಯಂಕ_write_usage,
				   options);
	}

	ಆದರೆ (opts.incremental_ತಳ &&
	    !(opts.flags & MIDX_WRITE_NO_CHAIN)) {
		error(_("cannot use --base without --no-write-chain-file"));
		usage_with_options(builtin_multi_pack_ಸೂಚ್ಯಂಕ_write_usage,
				   options);
	}

	packed_ಮೂಲ = handle_object_dir_option(repo);

	FREE_AND_NULL(options);

	ಆದರೆ (opts.stdin_packs) {
		ರಚನೆ string_list packs = STRING_LIST_INIT_DUP;

		read_packs_from_stdin(&packs);

		ret = write_midx_file_only(packed_ಮೂಲ, &packs,
					   opts.preferred_pack,
					   opts.refs_snapshot,
					   opts.incremental_ತಳ, opts.flags);

		string_list_clear(&packs, 0);
		ಬಿಡುಗಡೆ(opts.refs_snapshot);

		ಹಿಂತಿರುಗಿ ret;

	}

	ret = write_midx_file(packed_ಮೂಲ, opts.preferred_pack,
			      opts.refs_snapshot, opts.flags);

	ಬಿಡುಗಡೆ(opts.refs_snapshot);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cmd_multi_pack_ಸೂಚ್ಯಂಕ_compact(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
					ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
					ರಚನೆ repository *repo)
{
	ರಚನೆ multi_pack_ಸೂಚ್ಯಂಕ *m, *cur;
	ರಚನೆ multi_pack_ಸೂಚ್ಯಂಕ *from_midx = ಖಾಲಿ;
	ರಚನೆ multi_pack_ಸೂಚ್ಯಂಕ *ಗಮ್ಯ_midx = ಖಾಲಿ;
	ರಚನೆ odb_ಮೂಲ_packed *packed_ಮೂಲ;
	ಪೂರ್ಣಾಂಕ ret;

	ರಚನೆ option *options;
	ಸ್ಥಿರ ರಚನೆ option builtin_multi_pack_ಸೂಚ್ಯಂಕ_compact_options[] = {
		OPT_STRING(0, "base", &opts.incremental_ತಳ, N_("checksum"),
			   N_("base MIDX for incremental writes")),
		OPT_ಬಿಟ್(0, "bitmap", &opts.flags, N_("write multi-pack bitmap"),
			MIDX_WRITE_BITMAP | MIDX_WRITE_REV_ಸೂಚ್ಯಂಕ),
		OPT_ಬಿಟ್(0, "incremental", &opts.flags,
			N_("write a new incremental MIDX"), MIDX_WRITE_INCREMENTAL),
		OPT_NEGBIT(0, "write-chain-file", &opts.flags,
			N_("write the multi-pack-index chain file"),
			MIDX_WRITE_NO_CHAIN),
		OPT_ಕೊನೆ(),
	};

	repo_config(repo, git_multi_pack_ಸೂಚ್ಯಂಕ_write_config, ಖಾಲಿ);

	options = ಕೂಡಿಸು_common_options(builtin_multi_pack_ಸೂಚ್ಯಂಕ_compact_options);

	trace2_cmd_ಬಹುಲಕ(argv[0]);

	ಆದರೆ (isatty(2))
		opts.flags |= MIDX_ಪ್ರಗತಿ;
	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ,
			     options, builtin_multi_pack_ಸೂಚ್ಯಂಕ_compact_usage,
			     0);

	ಆದರೆ (argc != 2)
		usage_with_options(builtin_multi_pack_ಸೂಚ್ಯಂಕ_compact_usage,
				   options);

	ಆದರೆ (opts.flags & MIDX_WRITE_NO_CHAIN &&
	    !(opts.flags & MIDX_WRITE_INCREMENTAL)) {
		error(_("cannot use %s without %s"),
		      "--no-write-chain-file", "--incremental");
		usage_with_options(builtin_multi_pack_ಸೂಚ್ಯಂಕ_compact_usage,
				   options);
	}

	packed_ಮೂಲ = handle_object_dir_option(the_repository);

	FREE_AND_NULL(options);

	m = get_multi_pack_ಸೂಚ್ಯಂಕ(packed_ಮೂಲ);

	ಗಾಗಿ (cur = m; cur && !(from_midx && ಗಮ್ಯ_midx); cur = cur->ತಳ_midx) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *midx_csum = midx_get_checksum_hex(cur);

		ಆದರೆ (!from_midx && !strcmp(midx_csum, argv[0]))
			from_midx = cur;
		ಆದರೆ (!ಗಮ್ಯ_midx && !strcmp(midx_csum, argv[1]))
			ಗಮ್ಯ_midx = cur;
	}

	ಆದರೆ (!from_midx)
		die(_("could not find MIDX: %s"), argv[0]);
	ಆದರೆ (!ಗಮ್ಯ_midx)
		die(_("could not find MIDX: %s"), argv[1]);
	ಆದರೆ (from_midx == ಗಮ್ಯ_midx)
		die(_("MIDX compaction endpoints must be unique"));

	ಗಾಗಿ (m = from_midx; m; m = m->ತಳ_midx) {
		ಆದರೆ (m == ಗಮ್ಯ_midx)
			die(_("MIDX %s must be an ancestor of %s"), argv[0], argv[1]);
	}

	ret = write_midx_file_compact(packed_ಮೂಲ, from_midx, ಗಮ್ಯ_midx,
				      opts.incremental_ತಳ, opts.flags);

	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cmd_multi_pack_ಸೂಚ್ಯಂಕ_verify(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
				       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
				       ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ option *options;
	ಸ್ಥಿರ ರಚನೆ option builtin_multi_pack_ಸೂಚ್ಯಂಕ_verify_options[] = {
		OPT_ಕೊನೆ(),
	};
	ರಚನೆ odb_ಮೂಲ_packed *packed_ಮೂಲ;

	options = ಕೂಡಿಸು_common_options(builtin_multi_pack_ಸೂಚ್ಯಂಕ_verify_options);

	trace2_cmd_ಬಹುಲಕ(argv[0]);

	ಆದರೆ (isatty(2))
		opts.flags |= MIDX_ಪ್ರಗತಿ;
	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ,
			     options, builtin_multi_pack_ಸೂಚ್ಯಂಕ_verify_usage,
			     0);
	ಆದರೆ (argc)
		usage_with_options(builtin_multi_pack_ಸೂಚ್ಯಂಕ_verify_usage,
				   options);
	packed_ಮೂಲ = handle_object_dir_option(the_repository);

	FREE_AND_NULL(options);

	ಹಿಂತಿರುಗಿ verify_midx_file(packed_ಮೂಲ, opts.flags);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cmd_multi_pack_ಸೂಚ್ಯಂಕ_expire(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
				       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
				       ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ option *options;
	ಸ್ಥಿರ ರಚನೆ option builtin_multi_pack_ಸೂಚ್ಯಂಕ_expire_options[] = {
		OPT_ಕೊನೆ(),
	};
	ರಚನೆ odb_ಮೂಲ_packed *packed_ಮೂಲ;

	options = ಕೂಡಿಸು_common_options(builtin_multi_pack_ಸೂಚ್ಯಂಕ_expire_options);

	trace2_cmd_ಬಹುಲಕ(argv[0]);

	ಆದರೆ (isatty(2))
		opts.flags |= MIDX_ಪ್ರಗತಿ;
	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ,
			     options, builtin_multi_pack_ಸೂಚ್ಯಂಕ_expire_usage,
			     0);
	ಆದರೆ (argc)
		usage_with_options(builtin_multi_pack_ಸೂಚ್ಯಂಕ_expire_usage,
				   options);
	packed_ಮೂಲ = handle_object_dir_option(the_repository);

	FREE_AND_NULL(options);

	ಹಿಂತಿರುಗಿ expire_midx_packs(packed_ಮೂಲ, opts.flags);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cmd_multi_pack_ಸೂಚ್ಯಂಕ_repack(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
				       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
				       ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ option *options;
	ಸ್ಥಿರ ರಚನೆ option builtin_multi_pack_ಸೂಚ್ಯಂಕ_repack_options[] = {
		OPT_UNSIGNED(0, "batch-size", &opts.batch_ಗಾತ್ರ,
		  N_("during repack, collect pack-files of smaller size into a batch that is larger than this size")),
		OPT_ಕೊನೆ(),
	};
	ರಚನೆ odb_ಮೂಲ_packed *packed_ಮೂಲ;

	options = ಕೂಡಿಸು_common_options(builtin_multi_pack_ಸೂಚ್ಯಂಕ_repack_options);

	trace2_cmd_ಬಹುಲಕ(argv[0]);

	ಆದರೆ (isatty(2))
		opts.flags |= MIDX_ಪ್ರಗತಿ;
	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ,
			     options,
			     builtin_multi_pack_ಸೂಚ್ಯಂಕ_repack_usage,
			     0);
	ಆದರೆ (argc)
		usage_with_options(builtin_multi_pack_ಸೂಚ್ಯಂಕ_repack_usage,
				   options);
	packed_ಮೂಲ = handle_object_dir_option(the_repository);

	FREE_AND_NULL(options);

	ಹಿಂತಿರುಗಿ midx_repack(packed_ಮೂಲ, (ಗಾತ್ರ_t)opts.batch_ಗಾತ್ರ, opts.flags);
}

ಪೂರ್ಣಾಂಕ cmd_multi_pack_ಸೂಚ್ಯಂಕ(ಪೂರ್ಣಾಂಕ argc,
			 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
			 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			 ರಚನೆ repository *repo)
{
	ಪೂರ್ಣಾಂಕ res;
	parse_opt_subcommand_fn *fn = ಖಾಲಿ;
	ರಚನೆ option builtin_multi_pack_ಸೂಚ್ಯಂಕ_options[] = {
		OPT_SUBCOMMAND("repack", &fn, cmd_multi_pack_ಸೂಚ್ಯಂಕ_repack),
		OPT_SUBCOMMAND("write", &fn, cmd_multi_pack_ಸೂಚ್ಯಂಕ_write),
		OPT_SUBCOMMAND("compact", &fn, cmd_multi_pack_ಸೂಚ್ಯಂಕ_compact),
		OPT_SUBCOMMAND("verify", &fn, cmd_multi_pack_ಸೂಚ್ಯಂಕ_verify),
		OPT_SUBCOMMAND("expire", &fn, cmd_multi_pack_ಸೂಚ್ಯಂಕ_expire),
		OPT_ಕೊನೆ(),
	};
	ರಚನೆ option *options = parse_options_concat(builtin_multi_pack_ಸೂಚ್ಯಂಕ_options, common_opts);

	disable_replace_refs();

	repo_config(the_repository, git_default_config, ಖಾಲಿ);

	ಆದರೆ (the_repository &&
	    the_repository->objects &&
	    the_repository->objects->ಮೂಲ)
		opts.object_dir = xstrdup(the_repository->objects->ಮೂಲ->ಮಾರ್ಗ);

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options,
			     builtin_multi_pack_ಸೂಚ್ಯಂಕ_usage, 0);
	FREE_AND_NULL(options);

	res = fn(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, repo);

	ಬಿಡುಗಡೆ(opts.object_dir);
	ಹಿಂತಿರುಗಿ res;
}
