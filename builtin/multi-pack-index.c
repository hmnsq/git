#परिभाषय USE_THE_REPOSITORY_VARIABLE
#अन्तर्भाव "builtin.h"
#अन्तर्भाव "abspath.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "midx.h"
#अन्तर्भाव "strbuf.h"
#अन्तर्भाव "trace2.h"
#अन्तर्भाव "odb.h"
#अन्तर्भाव "odb/source.h"
#अन्तर्भाव "odb/source-files.h"
#अन्तर्भाव "replace-object.h"
#अन्तर्भाव "repository.h"

#परिभाषय BUILTIN_MIDX_WRITE_USAGE \
	N_("git multi-pack-index [<options>] write [--preferred-pack=<pack>]\n" \
	   "  [--[no-]bitmap] [--[no-]incremental] [--[no-]stdin-packs]\n" \
	   "  [--refs-snapshot=<path>] [--[no-]write-chain-file]\n" \
	   "  [--base=<checksum>]")

#परिभाषय BUILTIN_MIDX_COMPACT_USAGE \
	N_("git multi-pack-index [<options>] compact [--[no-]incremental]\n" \
	   "  [--[no-]bitmap] [--base=<checksum>] [--[no-]write-chain-file]\n" \
	   "  <from> <to>")

#परिभाषय BUILTIN_MIDX_VERIFY_USAGE \
	N_("git multi-pack-index [<options>] verify")

#परिभाषय BUILTIN_MIDX_EXPIRE_USAGE \
	N_("git multi-pack-index [<options>] expire")

#परिभाषय BUILTIN_MIDX_REPACK_USAGE \
	N_("git multi-pack-index [<options>] repack [--batch-size=<size>]")

स्थिर वर्ण ध्रुव * ध्रुव builtin_multi_pack_सूचकांक_write_usage[] = {
	BUILTIN_MIDX_WRITE_USAGE,
	रिक्त
};
स्थिर वर्ण ध्रुव * ध्रुव builtin_multi_pack_सूचकांक_compact_usage[] = {
	BUILTIN_MIDX_COMPACT_USAGE,
	रिक्त
};
स्थिर वर्ण ध्रुव * ध्रुव builtin_multi_pack_सूचकांक_verify_usage[] = {
	BUILTIN_MIDX_VERIFY_USAGE,
	रिक्त
};
स्थिर वर्ण ध्रुव * ध्रुव builtin_multi_pack_सूचकांक_expire_usage[] = {
	BUILTIN_MIDX_EXPIRE_USAGE,
	रिक्त
};
स्थिर वर्ण ध्रुव * ध्रुव builtin_multi_pack_सूचकांक_repack_usage[] = {
	BUILTIN_MIDX_REPACK_USAGE,
	रिक्त
};
स्थिर वर्ण ध्रुव * ध्रुव builtin_multi_pack_सूचकांक_usage[] = {
	BUILTIN_MIDX_WRITE_USAGE,
	BUILTIN_MIDX_COMPACT_USAGE,
	BUILTIN_MIDX_VERIFY_USAGE,
	BUILTIN_MIDX_EXPIRE_USAGE,
	BUILTIN_MIDX_REPACK_USAGE,
	रिक्त
};

स्थिर संरचना opts_multi_pack_सूचकांक {
	वर्ण *object_dir;
	ध्रुव वर्ण *preferred_pack;
	ध्रुव वर्ण *incremental_आधारः;
	वर्ण *refs_snapshot;
	अचिह्नित दीर्घ batch_आकार;
	अचिह्नित flags;
	पूर्णाङ्क stdin_packs;
} opts;


स्थिर पूर्णाङ्क parse_object_dir(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg,
			    पूर्णाङ्क unset)
{
	वर्ण **मानम् = opt->मानम्;
	मुक्त(*मानम्);
	यदि (unset)
		*मानम् = xstrdup(the_repository->objects->source->मार्गः);
	अन्यथा
		*मानम् = real_pathdup(arg, 1);
	प्रत्यागम 0;
}

स्थिर संरचना odb_source_packed *handle_object_dir_option(संरचना repository *repo)
{
	संरचना odb_files_dir *dir = odb_source_files_find_dir(repo->objects, opts.object_dir);
	यदि (!dir)
		die(_("object directory is not an alternate of the current repository: '%s'"),
		    opts.object_dir);
	प्रत्यागम dir->packed;
}

स्थिर संरचना option common_opts[] = {
	OPT_CALLBACK(0, "object-dir", &opts.object_dir,
	  N_("directory"),
	  N_("object directory containing set of packfile and pack-index pairs"),
	  parse_object_dir),
	OPT_BIT(0, "progress", &opts.flags, N_("force progress reporting"),
		MIDX_प्रगतिः),
	OPT_अन्तः(),
};

स्थिर संरचना option *सङ्कलय_common_options(संरचना option *पूर्व)
{
	प्रत्यागम parse_options_concat(common_opts, पूर्व);
}

स्थिर पूर्णाङ्क git_multi_pack_सूचकांक_write_config(ध्रुव वर्ण *var, ध्रुव वर्ण *मानम्,
					     ध्रुव संरचना config_context *ctx अप्रयुक्तम्,
					     शून्य *cb अप्रयुक्तम्)
{
	यदि (!वाक्यं_तुलय(var, "pack.writebitmaphashcache")) {
		यदि (git_config_bool(var, मानम्))
			opts.flags |= MIDX_WRITE_BITMAP_HASH_CACHE;
		अन्यथा
			opts.flags &= ~MIDX_WRITE_BITMAP_HASH_CACHE;
	}

	यदि (!वाक्यं_तुलय(var, "pack.writebitmaplookuptable")) {
		यदि (git_config_bool(var, मानम्))
			opts.flags |= MIDX_WRITE_BITMAP_LOOKUP_सारणी;
		अन्यथा
			opts.flags &= ~MIDX_WRITE_BITMAP_LOOKUP_सारणी;
	}

	/*
	 * We should never make a fall-back call to 'git_default_config', since
	 * this was already called in 'cmd_multi_pack_index()'.
	 */
	प्रत्यागम 0;
}

स्थिर शून्य read_packs_from_stdin(संरचना string_list *गन्तव्यम्)
{
	संरचना strbuf buf = STRBUF_INIT;
	यावत् (strbuf_getline(&buf, stdin) != EOF)
		string_list_append(गन्तव्यम्, buf.buf);
	string_list_क्रमय(गन्तव्यम्);

	strbuf_release(&buf);
}

स्थिर पूर्णाङ्क cmd_multi_pack_सूचकांक_write(पूर्णाङ्क argc, ध्रुव वर्ण **argv,
				      ध्रुव वर्ण *उपसर्गः,
				      संरचना repository *repo)
{
	संरचना option *options;
	स्थिर संरचना option builtin_multi_pack_सूचकांक_write_options[] = {
		OPT_STRING(0, "preferred-pack", &opts.preferred_pack,
			   N_("preferred-pack"),
			   N_("pack for reuse when computing a multi-pack bitmap")),
		OPT_BIT(0, "bitmap", &opts.flags, N_("write multi-pack bitmap"),
			MIDX_WRITE_BITMAP | MIDX_WRITE_REV_सूचकांक),
		OPT_STRING(0, "base", &opts.incremental_आधारः, N_("checksum"),
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
		OPT_अन्तः(),
	};
	संरचना odb_source_packed *packed_source;
	पूर्णाङ्क ret;

	opts.flags |= MIDX_WRITE_BITMAP_HASH_CACHE;

	repo_config(the_repository, git_multi_pack_सूचकांक_write_config, रिक्त);

	options = सङ्कलय_common_options(builtin_multi_pack_सूचकांक_write_options);

	trace2_cmd_बहुलकम्(argv[0]);

	यदि (isatty(2))
		opts.flags |= MIDX_प्रगतिः;
	argc = parse_options(argc, argv, उपसर्गः,
			     options, builtin_multi_pack_सूचकांक_write_usage,
			     0);
	यदि (argc)
		usage_with_options(builtin_multi_pack_सूचकांक_write_usage,
				   options);

	यदि (opts.flags & MIDX_WRITE_NO_CHAIN &&
	    !(opts.flags & MIDX_WRITE_INCREMENTAL)) {
		error(_("cannot use %s without %s"),
		      "--no-write-chain-file", "--incremental");
		usage_with_options(builtin_multi_pack_सूचकांक_write_usage,
				   options);
	}

	यदि (opts.incremental_आधारः &&
	    !(opts.flags & MIDX_WRITE_NO_CHAIN)) {
		error(_("cannot use --base without --no-write-chain-file"));
		usage_with_options(builtin_multi_pack_सूचकांक_write_usage,
				   options);
	}

	packed_source = handle_object_dir_option(repo);

	FREE_AND_NULL(options);

	यदि (opts.stdin_packs) {
		संरचना string_list packs = STRING_LIST_INIT_DUP;

		read_packs_from_stdin(&packs);

		ret = write_midx_file_only(packed_source, &packs,
					   opts.preferred_pack,
					   opts.refs_snapshot,
					   opts.incremental_आधारः, opts.flags);

		string_list_clear(&packs, 0);
		मुक्त(opts.refs_snapshot);

		प्रत्यागम ret;

	}

	ret = write_midx_file(packed_source, opts.preferred_pack,
			      opts.refs_snapshot, opts.flags);

	मुक्त(opts.refs_snapshot);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क cmd_multi_pack_सूचकांक_compact(पूर्णाङ्क argc, ध्रुव वर्ण **argv,
					ध्रुव वर्ण *उपसर्गः,
					संरचना repository *repo)
{
	संरचना multi_pack_सूचकांक *m, *cur;
	संरचना multi_pack_सूचकांक *from_midx = रिक्त;
	संरचना multi_pack_सूचकांक *गन्तव्यम्_midx = रिक्त;
	संरचना odb_source_packed *packed_source;
	पूर्णाङ्क ret;

	संरचना option *options;
	स्थिर संरचना option builtin_multi_pack_सूचकांक_compact_options[] = {
		OPT_STRING(0, "base", &opts.incremental_आधारः, N_("checksum"),
			   N_("base MIDX for incremental writes")),
		OPT_BIT(0, "bitmap", &opts.flags, N_("write multi-pack bitmap"),
			MIDX_WRITE_BITMAP | MIDX_WRITE_REV_सूचकांक),
		OPT_BIT(0, "incremental", &opts.flags,
			N_("write a new incremental MIDX"), MIDX_WRITE_INCREMENTAL),
		OPT_NEGBIT(0, "write-chain-file", &opts.flags,
			N_("write the multi-pack-index chain file"),
			MIDX_WRITE_NO_CHAIN),
		OPT_अन्तः(),
	};

	repo_config(repo, git_multi_pack_सूचकांक_write_config, रिक्त);

	options = सङ्कलय_common_options(builtin_multi_pack_सूचकांक_compact_options);

	trace2_cmd_बहुलकम्(argv[0]);

	यदि (isatty(2))
		opts.flags |= MIDX_प्रगतिः;
	argc = parse_options(argc, argv, उपसर्गः,
			     options, builtin_multi_pack_सूचकांक_compact_usage,
			     0);

	यदि (argc != 2)
		usage_with_options(builtin_multi_pack_सूचकांक_compact_usage,
				   options);

	यदि (opts.flags & MIDX_WRITE_NO_CHAIN &&
	    !(opts.flags & MIDX_WRITE_INCREMENTAL)) {
		error(_("cannot use %s without %s"),
		      "--no-write-chain-file", "--incremental");
		usage_with_options(builtin_multi_pack_सूचकांक_compact_usage,
				   options);
	}

	packed_source = handle_object_dir_option(the_repository);

	FREE_AND_NULL(options);

	m = get_multi_pack_सूचकांक(packed_source);

	प्रति (cur = m; cur && !(from_midx && गन्तव्यम्_midx); cur = cur->आधारः_midx) {
		ध्रुव वर्ण *midx_csum = midx_get_checksum_hex(cur);

		यदि (!from_midx && !वाक्यं_तुलय(midx_csum, argv[0]))
			from_midx = cur;
		यदि (!गन्तव्यम्_midx && !वाक्यं_तुलय(midx_csum, argv[1]))
			गन्तव्यम्_midx = cur;
	}

	यदि (!from_midx)
		die(_("could not find MIDX: %s"), argv[0]);
	यदि (!गन्तव्यम्_midx)
		die(_("could not find MIDX: %s"), argv[1]);
	यदि (from_midx == गन्तव्यम्_midx)
		die(_("MIDX compaction endpoints must be unique"));

	प्रति (m = from_midx; m; m = m->आधारः_midx) {
		यदि (m == गन्तव्यम्_midx)
			die(_("MIDX %s must be an ancestor of %s"), argv[0], argv[1]);
	}

	ret = write_midx_file_compact(packed_source, from_midx, गन्तव्यम्_midx,
				      opts.incremental_आधारः, opts.flags);

	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क cmd_multi_pack_सूचकांक_verify(पूर्णाङ्क argc, ध्रुव वर्ण **argv,
				       ध्रुव वर्ण *उपसर्गः,
				       संरचना repository *repo अप्रयुक्तम्)
{
	संरचना option *options;
	स्थिर संरचना option builtin_multi_pack_सूचकांक_verify_options[] = {
		OPT_अन्तः(),
	};
	संरचना odb_source_packed *packed_source;

	options = सङ्कलय_common_options(builtin_multi_pack_सूचकांक_verify_options);

	trace2_cmd_बहुलकम्(argv[0]);

	यदि (isatty(2))
		opts.flags |= MIDX_प्रगतिः;
	argc = parse_options(argc, argv, उपसर्गः,
			     options, builtin_multi_pack_सूचकांक_verify_usage,
			     0);
	यदि (argc)
		usage_with_options(builtin_multi_pack_सूचकांक_verify_usage,
				   options);
	packed_source = handle_object_dir_option(the_repository);

	FREE_AND_NULL(options);

	प्रत्यागम verify_midx_file(packed_source, opts.flags);
}

स्थिर पूर्णाङ्क cmd_multi_pack_सूचकांक_expire(पूर्णाङ्क argc, ध्रुव वर्ण **argv,
				       ध्रुव वर्ण *उपसर्गः,
				       संरचना repository *repo अप्रयुक्तम्)
{
	संरचना option *options;
	स्थिर संरचना option builtin_multi_pack_सूचकांक_expire_options[] = {
		OPT_अन्तः(),
	};
	संरचना odb_source_packed *packed_source;

	options = सङ्कलय_common_options(builtin_multi_pack_सूचकांक_expire_options);

	trace2_cmd_बहुलकम्(argv[0]);

	यदि (isatty(2))
		opts.flags |= MIDX_प्रगतिः;
	argc = parse_options(argc, argv, उपसर्गः,
			     options, builtin_multi_pack_सूचकांक_expire_usage,
			     0);
	यदि (argc)
		usage_with_options(builtin_multi_pack_सूचकांक_expire_usage,
				   options);
	packed_source = handle_object_dir_option(the_repository);

	FREE_AND_NULL(options);

	प्रत्यागम expire_midx_packs(packed_source, opts.flags);
}

स्थिर पूर्णाङ्क cmd_multi_pack_सूचकांक_repack(पूर्णाङ्क argc, ध्रुव वर्ण **argv,
				       ध्रुव वर्ण *उपसर्गः,
				       संरचना repository *repo अप्रयुक्तम्)
{
	संरचना option *options;
	स्थिर संरचना option builtin_multi_pack_सूचकांक_repack_options[] = {
		OPT_UNSIGNED(0, "batch-size", &opts.batch_आकार,
		  N_("during repack, collect pack-files of smaller size into a batch that is larger than this size")),
		OPT_अन्तः(),
	};
	संरचना odb_source_packed *packed_source;

	options = सङ्कलय_common_options(builtin_multi_pack_सूचकांक_repack_options);

	trace2_cmd_बहुलकम्(argv[0]);

	यदि (isatty(2))
		opts.flags |= MIDX_प्रगतिः;
	argc = parse_options(argc, argv, उपसर्गः,
			     options,
			     builtin_multi_pack_सूचकांक_repack_usage,
			     0);
	यदि (argc)
		usage_with_options(builtin_multi_pack_सूचकांक_repack_usage,
				   options);
	packed_source = handle_object_dir_option(the_repository);

	FREE_AND_NULL(options);

	प्रत्यागम midx_repack(packed_source, (आकार_t)opts.batch_आकार, opts.flags);
}

पूर्णाङ्क cmd_multi_pack_सूचकांक(पूर्णाङ्क argc,
			 ध्रुव वर्ण **argv,
			 ध्रुव वर्ण *उपसर्गः,
			 संरचना repository *repo)
{
	पूर्णाङ्क res;
	parse_opt_subcommand_fn *fn = रिक्त;
	संरचना option builtin_multi_pack_सूचकांक_options[] = {
		OPT_SUBCOMMAND("repack", &fn, cmd_multi_pack_सूचकांक_repack),
		OPT_SUBCOMMAND("write", &fn, cmd_multi_pack_सूचकांक_write),
		OPT_SUBCOMMAND("compact", &fn, cmd_multi_pack_सूचकांक_compact),
		OPT_SUBCOMMAND("verify", &fn, cmd_multi_pack_सूचकांक_verify),
		OPT_SUBCOMMAND("expire", &fn, cmd_multi_pack_सूचकांक_expire),
		OPT_अन्तः(),
	};
	संरचना option *options = parse_options_concat(builtin_multi_pack_सूचकांक_options, common_opts);

	disable_replace_refs();

	repo_config(the_repository, git_default_config, रिक्त);

	यदि (the_repository &&
	    the_repository->objects &&
	    the_repository->objects->source)
		opts.object_dir = xstrdup(the_repository->objects->source->मार्गः);

	argc = parse_options(argc, argv, उपसर्गः, options,
			     builtin_multi_pack_सूचकांक_usage, 0);
	FREE_AND_NULL(options);

	res = fn(argc, argv, उपसर्गः, repo);

	मुक्त(opts.object_dir);
	प्रत्यागम res;
}
