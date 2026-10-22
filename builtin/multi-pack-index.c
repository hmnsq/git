#నిర్వచించు USE_THE_REPOSITORY_VARIABLE
#అన్తర్భావ "builtin.h"
#అన్తర్భావ "abspath.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "midx.h"
#అన్తర్భావ "strbuf.h"
#అన్తర్భావ "trace2.h"
#అన్తర్భావ "odb.h"
#అన్తర్భావ "odb/source.h"
#అన్తర్భావ "odb/source-files.h"
#అన్తర్భావ "replace-object.h"
#అన్తర్భావ "repository.h"

#నిర్వచించు BUILTIN_MIDX_WRITE_USAGE \
	N_("git multi-pack-index [<options>] write [--preferred-pack=<pack>]\n" \
	   "  [--[no-]bitmap] [--[no-]incremental] [--[no-]stdin-packs]\n" \
	   "  [--refs-snapshot=<path>] [--[no-]write-chain-file]\n" \
	   "  [--base=<checksum>]")

#నిర్వచించు BUILTIN_MIDX_COMPACT_USAGE \
	N_("git multi-pack-index [<options>] compact [--[no-]incremental]\n" \
	   "  [--[no-]bitmap] [--base=<checksum>] [--[no-]write-chain-file]\n" \
	   "  <from> <to>")

#నిర్వచించు BUILTIN_MIDX_VERIFY_USAGE \
	N_("git multi-pack-index [<options>] verify")

#నిర్వచించు BUILTIN_MIDX_EXPIRE_USAGE \
	N_("git multi-pack-index [<options>] expire")

#నిర్వచించు BUILTIN_MIDX_REPACK_USAGE \
	N_("git multi-pack-index [<options>] repack [--batch-size=<size>]")

స్థిర అక్షరం స్థిరరాశి * స్థిరరాశి builtin_multi_pack_సూచీ_write_usage[] = {
	BUILTIN_MIDX_WRITE_USAGE,
	ఖాళీ
};
స్థిర అక్షరం స్థిరరాశి * స్థిరరాశి builtin_multi_pack_సూచీ_compact_usage[] = {
	BUILTIN_MIDX_COMPACT_USAGE,
	ఖాళీ
};
స్థిర అక్షరం స్థిరరాశి * స్థిరరాశి builtin_multi_pack_సూచీ_verify_usage[] = {
	BUILTIN_MIDX_VERIFY_USAGE,
	ఖాళీ
};
స్థిర అక్షరం స్థిరరాశి * స్థిరరాశి builtin_multi_pack_సూచీ_expire_usage[] = {
	BUILTIN_MIDX_EXPIRE_USAGE,
	ఖాళీ
};
స్థిర అక్షరం స్థిరరాశి * స్థిరరాశి builtin_multi_pack_సూచీ_repack_usage[] = {
	BUILTIN_MIDX_REPACK_USAGE,
	ఖాళీ
};
స్థిర అక్షరం స్థిరరాశి * స్థిరరాశి builtin_multi_pack_సూచీ_usage[] = {
	BUILTIN_MIDX_WRITE_USAGE,
	BUILTIN_MIDX_COMPACT_USAGE,
	BUILTIN_MIDX_VERIFY_USAGE,
	BUILTIN_MIDX_EXPIRE_USAGE,
	BUILTIN_MIDX_REPACK_USAGE,
	ఖాళీ
};

స్థిర నిర్మాణం opts_multi_pack_సూచీ {
	అక్షరం *object_dir;
	స్థిరరాశి అక్షరం *preferred_pack;
	స్థిరరాశి అక్షరం *incremental_పునాది;
	అక్షరం *refs_snapshot;
	గుర్తులేని దీర్ఘ batch_కొలత;
	గుర్తులేని flags;
	పూర్ణాంకం stdin_packs;
} opts;


స్థిర పూర్ణాంకం parse_object_dir(స్థిరరాశి నిర్మాణం option *opt, స్థిరరాశి అక్షరం *arg,
			    పూర్ణాంకం unset)
{
	అక్షరం **విలువ = opt->విలువ;
	విడుదల(*విలువ);
	ఒకవేళ (unset)
		*విలువ = xstrdup(the_repository->objects->ఆధారం->మార్గం);
	లేకపోతే
		*విలువ = real_pathdup(arg, 1);
	తిరిగి 0;
}

స్థిర నిర్మాణం odb_ఆధారం_packed *handle_object_dir_option(నిర్మాణం repository *repo)
{
	నిర్మాణం odb_files_dir *dir = odb_ఆధారం_files_కనుగొను_dir(repo->objects, opts.object_dir);
	ఒకవేళ (!dir)
		die(_("object directory is not an alternate of the current repository: '%s'"),
		    opts.object_dir);
	తిరిగి dir->packed;
}

స్థిర నిర్మాణం option common_opts[] = {
	OPT_CALLBACK(0, "object-dir", &opts.object_dir,
	  N_("directory"),
	  N_("object directory containing set of packfile and pack-index pairs"),
	  parse_object_dir),
	OPT_బిట్టు(0, "progress", &opts.flags, N_("force progress reporting"),
		MIDX_పురోగతి),
	OPT_చివర(),
};

స్థిర నిర్మాణం option *కూడు_common_options(నిర్మాణం option *prev)
{
	తిరిగి parse_options_concat(common_opts, prev);
}

స్థిర పూర్ణాంకం git_multi_pack_సూచీ_write_config(స్థిరరాశి అక్షరం *var, స్థిరరాశి అక్షరం *విలువ,
					     స్థిరరాశి నిర్మాణం config_context *ctx వాడని,
					     శూన్య *cb వాడని)
{
	ఒకవేళ (!strcmp(var, "pack.writebitmaphashcache")) {
		ఒకవేళ (git_config_bool(var, విలువ))
			opts.flags |= MIDX_WRITE_BITMAP_HASH_CACHE;
		లేకపోతే
			opts.flags &= ~MIDX_WRITE_BITMAP_HASH_CACHE;
	}

	ఒకవేళ (!strcmp(var, "pack.writebitmaplookuptable")) {
		ఒకవేళ (git_config_bool(var, విలువ))
			opts.flags |= MIDX_WRITE_BITMAP_LOOKUP_పట్టిక;
		లేకపోతే
			opts.flags &= ~MIDX_WRITE_BITMAP_LOOKUP_పట్టిక;
	}

	/*
	 * We should never make a fall-back call to 'git_default_config', since
	 * this was already called in 'cmd_multi_pack_index()'.
	 */
	తిరిగి 0;
}

స్థిర శూన్య read_packs_from_stdin(నిర్మాణం string_list *గమ్యం)
{
	నిర్మాణం strbuf buf = STRBUF_INIT;
	అయితే (strbuf_getline(&buf, stdin) != EOF)
		string_list_append(గమ్యం, buf.buf);
	string_list_క్రమీకరించు(గమ్యం);

	strbuf_release(&buf);
}

స్థిర పూర్ణాంకం cmd_multi_pack_సూచీ_write(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv,
				      స్థిరరాశి అక్షరం *ఉపసర్గ,
				      నిర్మాణం repository *repo)
{
	నిర్మాణం option *options;
	స్థిర నిర్మాణం option builtin_multi_pack_సూచీ_write_options[] = {
		OPT_STRING(0, "preferred-pack", &opts.preferred_pack,
			   N_("preferred-pack"),
			   N_("pack for reuse when computing a multi-pack bitmap")),
		OPT_బిట్టు(0, "bitmap", &opts.flags, N_("write multi-pack bitmap"),
			MIDX_WRITE_BITMAP | MIDX_WRITE_REV_సూచీ),
		OPT_STRING(0, "base", &opts.incremental_పునాది, N_("checksum"),
			   N_("base MIDX for incremental writes")),
		OPT_బిట్టు(0, "incremental", &opts.flags,
			N_("write a new incremental MIDX"), MIDX_WRITE_INCREMENTAL),
		OPT_NEGBIT(0, "write-chain-file", &opts.flags,
			N_("write the multi-pack-index chain file"),
			MIDX_WRITE_NO_CHAIN),
		OPT_BOOL(0, "stdin-packs", &opts.stdin_packs,
			 N_("write multi-pack index containing only given indexes")),
		OPT_FILENAME(0, "refs-snapshot", &opts.refs_snapshot,
			     N_("refs snapshot for selecting bitmap commits")),
		OPT_చివర(),
	};
	నిర్మాణం odb_ఆధారం_packed *packed_ఆధారం;
	పూర్ణాంకం ret;

	opts.flags |= MIDX_WRITE_BITMAP_HASH_CACHE;

	repo_config(the_repository, git_multi_pack_సూచీ_write_config, ఖాళీ);

	options = కూడు_common_options(builtin_multi_pack_సూచీ_write_options);

	trace2_cmd_బహుళకం(argv[0]);

	ఒకవేళ (isatty(2))
		opts.flags |= MIDX_పురోగతి;
	argc = parse_options(argc, argv, ఉపసర్గ,
			     options, builtin_multi_pack_సూచీ_write_usage,
			     0);
	ఒకవేళ (argc)
		usage_with_options(builtin_multi_pack_సూచీ_write_usage,
				   options);

	ఒకవేళ (opts.flags & MIDX_WRITE_NO_CHAIN &&
	    !(opts.flags & MIDX_WRITE_INCREMENTAL)) {
		error(_("cannot use %s without %s"),
		      "--no-write-chain-file", "--incremental");
		usage_with_options(builtin_multi_pack_సూచీ_write_usage,
				   options);
	}

	ఒకవేళ (opts.incremental_పునాది &&
	    !(opts.flags & MIDX_WRITE_NO_CHAIN)) {
		error(_("cannot use --base without --no-write-chain-file"));
		usage_with_options(builtin_multi_pack_సూచీ_write_usage,
				   options);
	}

	packed_ఆధారం = handle_object_dir_option(repo);

	FREE_AND_NULL(options);

	ఒకవేళ (opts.stdin_packs) {
		నిర్మాణం string_list packs = STRING_LIST_INIT_DUP;

		read_packs_from_stdin(&packs);

		ret = write_midx_file_only(packed_ఆధారం, &packs,
					   opts.preferred_pack,
					   opts.refs_snapshot,
					   opts.incremental_పునాది, opts.flags);

		string_list_clear(&packs, 0);
		విడుదల(opts.refs_snapshot);

		తిరిగి ret;

	}

	ret = write_midx_file(packed_ఆధారం, opts.preferred_pack,
			      opts.refs_snapshot, opts.flags);

	విడుదల(opts.refs_snapshot);
	తిరిగి ret;
}

స్థిర పూర్ణాంకం cmd_multi_pack_సూచీ_compact(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv,
					స్థిరరాశి అక్షరం *ఉపసర్గ,
					నిర్మాణం repository *repo)
{
	నిర్మాణం multi_pack_సూచీ *m, *cur;
	నిర్మాణం multi_pack_సూచీ *from_midx = ఖాళీ;
	నిర్మాణం multi_pack_సూచీ *గమ్యం_midx = ఖాళీ;
	నిర్మాణం odb_ఆధారం_packed *packed_ఆధారం;
	పూర్ణాంకం ret;

	నిర్మాణం option *options;
	స్థిర నిర్మాణం option builtin_multi_pack_సూచీ_compact_options[] = {
		OPT_STRING(0, "base", &opts.incremental_పునాది, N_("checksum"),
			   N_("base MIDX for incremental writes")),
		OPT_బిట్టు(0, "bitmap", &opts.flags, N_("write multi-pack bitmap"),
			MIDX_WRITE_BITMAP | MIDX_WRITE_REV_సూచీ),
		OPT_బిట్టు(0, "incremental", &opts.flags,
			N_("write a new incremental MIDX"), MIDX_WRITE_INCREMENTAL),
		OPT_NEGBIT(0, "write-chain-file", &opts.flags,
			N_("write the multi-pack-index chain file"),
			MIDX_WRITE_NO_CHAIN),
		OPT_చివర(),
	};

	repo_config(repo, git_multi_pack_సూచీ_write_config, ఖాళీ);

	options = కూడు_common_options(builtin_multi_pack_సూచీ_compact_options);

	trace2_cmd_బహుళకం(argv[0]);

	ఒకవేళ (isatty(2))
		opts.flags |= MIDX_పురోగతి;
	argc = parse_options(argc, argv, ఉపసర్గ,
			     options, builtin_multi_pack_సూచీ_compact_usage,
			     0);

	ఒకవేళ (argc != 2)
		usage_with_options(builtin_multi_pack_సూచీ_compact_usage,
				   options);

	ఒకవేళ (opts.flags & MIDX_WRITE_NO_CHAIN &&
	    !(opts.flags & MIDX_WRITE_INCREMENTAL)) {
		error(_("cannot use %s without %s"),
		      "--no-write-chain-file", "--incremental");
		usage_with_options(builtin_multi_pack_సూచీ_compact_usage,
				   options);
	}

	packed_ఆధారం = handle_object_dir_option(the_repository);

	FREE_AND_NULL(options);

	m = get_multi_pack_సూచీ(packed_ఆధారం);

	కొరకు (cur = m; cur && !(from_midx && గమ్యం_midx); cur = cur->పునాది_midx) {
		స్థిరరాశి అక్షరం *midx_csum = midx_get_checksum_hex(cur);

		ఒకవేళ (!from_midx && !strcmp(midx_csum, argv[0]))
			from_midx = cur;
		ఒకవేళ (!గమ్యం_midx && !strcmp(midx_csum, argv[1]))
			గమ్యం_midx = cur;
	}

	ఒకవేళ (!from_midx)
		die(_("could not find MIDX: %s"), argv[0]);
	ఒకవేళ (!గమ్యం_midx)
		die(_("could not find MIDX: %s"), argv[1]);
	ఒకవేళ (from_midx == గమ్యం_midx)
		die(_("MIDX compaction endpoints must be unique"));

	కొరకు (m = from_midx; m; m = m->పునాది_midx) {
		ఒకవేళ (m == గమ్యం_midx)
			die(_("MIDX %s must be an ancestor of %s"), argv[0], argv[1]);
	}

	ret = write_midx_file_compact(packed_ఆధారం, from_midx, గమ్యం_midx,
				      opts.incremental_పునాది, opts.flags);

	తిరిగి ret;
}

స్థిర పూర్ణాంకం cmd_multi_pack_సూచీ_verify(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv,
				       స్థిరరాశి అక్షరం *ఉపసర్గ,
				       నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం option *options;
	స్థిర నిర్మాణం option builtin_multi_pack_సూచీ_verify_options[] = {
		OPT_చివర(),
	};
	నిర్మాణం odb_ఆధారం_packed *packed_ఆధారం;

	options = కూడు_common_options(builtin_multi_pack_సూచీ_verify_options);

	trace2_cmd_బహుళకం(argv[0]);

	ఒకవేళ (isatty(2))
		opts.flags |= MIDX_పురోగతి;
	argc = parse_options(argc, argv, ఉపసర్గ,
			     options, builtin_multi_pack_సూచీ_verify_usage,
			     0);
	ఒకవేళ (argc)
		usage_with_options(builtin_multi_pack_సూచీ_verify_usage,
				   options);
	packed_ఆధారం = handle_object_dir_option(the_repository);

	FREE_AND_NULL(options);

	తిరిగి verify_midx_file(packed_ఆధారం, opts.flags);
}

స్థిర పూర్ణాంకం cmd_multi_pack_సూచీ_expire(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv,
				       స్థిరరాశి అక్షరం *ఉపసర్గ,
				       నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం option *options;
	స్థిర నిర్మాణం option builtin_multi_pack_సూచీ_expire_options[] = {
		OPT_చివర(),
	};
	నిర్మాణం odb_ఆధారం_packed *packed_ఆధారం;

	options = కూడు_common_options(builtin_multi_pack_సూచీ_expire_options);

	trace2_cmd_బహుళకం(argv[0]);

	ఒకవేళ (isatty(2))
		opts.flags |= MIDX_పురోగతి;
	argc = parse_options(argc, argv, ఉపసర్గ,
			     options, builtin_multi_pack_సూచీ_expire_usage,
			     0);
	ఒకవేళ (argc)
		usage_with_options(builtin_multi_pack_సూచీ_expire_usage,
				   options);
	packed_ఆధారం = handle_object_dir_option(the_repository);

	FREE_AND_NULL(options);

	తిరిగి expire_midx_packs(packed_ఆధారం, opts.flags);
}

స్థిర పూర్ణాంకం cmd_multi_pack_సూచీ_repack(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv,
				       స్థిరరాశి అక్షరం *ఉపసర్గ,
				       నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం option *options;
	స్థిర నిర్మాణం option builtin_multi_pack_సూచీ_repack_options[] = {
		OPT_UNSIGNED(0, "batch-size", &opts.batch_కొలత,
		  N_("during repack, collect pack-files of smaller size into a batch that is larger than this size")),
		OPT_చివర(),
	};
	నిర్మాణం odb_ఆధారం_packed *packed_ఆధారం;

	options = కూడు_common_options(builtin_multi_pack_సూచీ_repack_options);

	trace2_cmd_బహుళకం(argv[0]);

	ఒకవేళ (isatty(2))
		opts.flags |= MIDX_పురోగతి;
	argc = parse_options(argc, argv, ఉపసర్గ,
			     options,
			     builtin_multi_pack_సూచీ_repack_usage,
			     0);
	ఒకవేళ (argc)
		usage_with_options(builtin_multi_pack_సూచీ_repack_usage,
				   options);
	packed_ఆధారం = handle_object_dir_option(the_repository);

	FREE_AND_NULL(options);

	తిరిగి midx_repack(packed_ఆధారం, (కొలత_t)opts.batch_కొలత, opts.flags);
}

పూర్ణాంకం cmd_multi_pack_సూచీ(పూర్ణాంకం argc,
			 స్థిరరాశి అక్షరం **argv,
			 స్థిరరాశి అక్షరం *ఉపసర్గ,
			 నిర్మాణం repository *repo)
{
	పూర్ణాంకం res;
	parse_opt_subcommand_fn *fn = ఖాళీ;
	నిర్మాణం option builtin_multi_pack_సూచీ_options[] = {
		OPT_SUBCOMMAND("repack", &fn, cmd_multi_pack_సూచీ_repack),
		OPT_SUBCOMMAND("write", &fn, cmd_multi_pack_సూచీ_write),
		OPT_SUBCOMMAND("compact", &fn, cmd_multi_pack_సూచీ_compact),
		OPT_SUBCOMMAND("verify", &fn, cmd_multi_pack_సూచీ_verify),
		OPT_SUBCOMMAND("expire", &fn, cmd_multi_pack_సూచీ_expire),
		OPT_చివర(),
	};
	నిర్మాణం option *options = parse_options_concat(builtin_multi_pack_సూచీ_options, common_opts);

	disable_replace_refs();

	repo_config(the_repository, git_default_config, ఖాళీ);

	ఒకవేళ (the_repository &&
	    the_repository->objects &&
	    the_repository->objects->ఆధారం)
		opts.object_dir = xstrdup(the_repository->objects->ఆధారం->మార్గం);

	argc = parse_options(argc, argv, ఉపసర్గ, options,
			     builtin_multi_pack_సూచీ_usage, 0);
	FREE_AND_NULL(options);

	res = fn(argc, argv, ఉపసర్గ, repo);

	విడుదల(opts.object_dir);
	తిరిగి res;
}
