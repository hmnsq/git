#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "fsck.h"
#അന്തര്ഭാവ "pack-refs.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "refs.h"
#അന്തര്ഭാവ "strbuf.h"
#അന്തര്ഭാവ "worktree.h"
#അന്തര്ഭാവ "for-each-ref.h"
#അന്തര്ഭാവ "refs/refs-internal.h"

#define REFS_MIGRATE_USAGE \
	N_("git refs migrate --ref-storage-format=<format> [--no-reflog] [--dry-run]")

#define REFS_VERIFY_USAGE \
	N_("git refs verify [--strict] [--verbose]")

#define REFS_EXISTS_USAGE \
	N_("git refs exists <ref>")

#define REFS_OPTIMIZE_USAGE \
	N_("git refs optimize " PACK_REFS_OPTS)

#define REFS_CREATE_USAGE \
	N_("git refs create [--message=<reason>] [--no-deref] [--create-reflog] <ref> <new-value>")

#define REFS_DELETE_USAGE \
	N_("git refs delete [--message=<reason>] [--no-deref] <ref> [<old-value>]")

#define REFS_UPDATE_USAGE \
	N_("git refs update [--message=<reason>] [--no-deref] [--create-reflog] <ref> <new-value> [<old-value>]")

#define REFS_RENAME_USAGE \
	N_("git refs rename [--message=<reason>] <old-ref> <new-ref>")

സ്ഥിതം പൂർണ്ണസംഖ്യ cmd_refs_migrate(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
			    ഘടന repository *repo)
{
	സ്ഥിരം അക്ഷരം * സ്ഥിരം migrate_usage[] = {
		REFS_MIGRATE_USAGE,
		ശൂന്യ,
	};
	സ്ഥിരം അക്ഷരം *format_str = ശൂന്യ;
	എണ്ണൽ ref_storage_format format;
	unsigned പൂർണ്ണസംഖ്യ flags = 0;
	ഘടന option options[] = {
		OPT_STRING_F(0, "ref-storage-format", &format_str, N_("format"),
			N_("specify the reference storage format to convert to"),
			PARSE_OPT_NONEG),
		OPT_ALIAS_F(0, "ref-format", "ref-storage-format", PARSE_OPT_HIDDEN),
		OPT_BIT(0, "dry-run", &flags,
			N_("perform a non-destructive dry-run"),
			REPO_MIGRATE_REF_STORAGE_FORMAT_DRYRUN),
		OPT_BIT(0, "no-reflog", &flags,
			N_("drop reflogs entirely during the migration"),
			REPO_MIGRATE_REF_STORAGE_FORMAT_SKIP_REFLOG),
		OPT_END(),
	};
	ഘടന strbuf errbuf = STRBUF_INIT;
	പൂർണ്ണസംഖ്യ err;

	argc = parse_options(argc, argv, prefix, options, migrate_usage, 0);
	എങ്കിൽ (argc)
		usage(_("too many arguments"));
	എങ്കിൽ (!format_str)
		usage(_("missing --ref-storage-format=<format>"));

	format = ref_storage_format_by_name(format_str);
	എങ്കിൽ (format == REF_STORAGE_FORMAT_UNKNOWN) {
		err = error(_("unknown ref storage format '%s'"), format_str);
		പോകുക out;
	}

	എങ്കിൽ (repo->ref_storage_format == format) {
		err = error(_("repository already uses '%s' format"),
			    ref_storage_format_to_name(format));
		പോകുക out;
	}

	എങ്കിൽ (repo_migrate_ref_storage_format(repo, format, flags, &errbuf) < 0) {
		err = error("%s", errbuf.buf);
		പോകുക out;
	}

	err = 0;

out:
	strbuf_release(&errbuf);
	തിരികെ err;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cmd_refs_verify(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
			   ഘടന repository *repo)
{
	ഘടന fsck_options fsck_refs_options;
	ഘടന worktree **worktrees;
	സ്ഥിരം അക്ഷരം * സ്ഥിരം verify_usage[] = {
		REFS_VERIFY_USAGE,
		ശൂന്യ,
	};
	ഘടന option options[] = {
		OPT_BOOL(0, "verbose", &fsck_refs_options.verbose, N_("be verbose")),
		OPT_BOOL(0, "strict", &fsck_refs_options.strict, N_("enable strict checking")),
		OPT_END(),
	};
	പൂർണ്ണസംഖ്യ ret = 0;

	fsck_options_init(&fsck_refs_options, repo, FSCK_OPTIONS_REFS);

	argc = parse_options(argc, argv, prefix, options, verify_usage, 0);
	എങ്കിൽ (argc)
		usage(_("'git refs verify' takes no arguments"));

	repo_config(repo, git_fsck_config, &fsck_refs_options);
	prepare_repo_settings(repo);

	worktrees = get_worktrees_without_reading_head(repo);
	വേണ്ടി (size_t i = 0; worktrees[i]; i++)
		ret |= refs_fsck(get_worktree_ref_store(worktrees[i]),
				 &fsck_refs_options, worktrees[i]);

	fsck_options_clear(&fsck_refs_options);
	free_worktrees(worktrees);
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cmd_refs_list(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
			   ഘടന repository *repo)
{
	സ്ഥിതം അക്ഷരം സ്ഥിരം * സ്ഥിരം refs_list_usage[] = {
		N_("git refs list " COMMON_USAGE_FOR_EACH_REF),
		ശൂന്യ
	};

	തിരികെ for_each_ref_core(argc, argv, prefix, repo, refs_list_usage);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cmd_refs_exists(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
			   ഘടന repository *repo)
{
	ഘടന strbuf unused_referent = STRBUF_INIT;
	ഘടന object_id unused_oid;
	unsigned പൂർണ്ണസംഖ്യ unused_type;
	പൂർണ്ണസംഖ്യ failure_errno = 0;
	സ്ഥിരം അക്ഷരം *ref;
	പൂർണ്ണസംഖ്യ ret = 0;
	സ്ഥിരം അക്ഷരം * സ്ഥിരം exists_usage[] = {
		REFS_EXISTS_USAGE,
		ശൂന്യ,
	};
	ഘടന option options[] = {
		OPT_END(),
	};

	argc = parse_options(argc, argv, prefix, options, exists_usage, 0);
	എങ്കിൽ (argc != 1)
		die(_("'git refs exists' requires a reference"));

	ref = *argv++;
	എങ്കിൽ (refs_read_raw_ref(get_main_ref_store(repo), ref,
			      &unused_oid, &unused_referent, &unused_type,
			      &failure_errno)) {
		എങ്കിൽ (failure_errno == ENOENT || failure_errno == EISDIR) {
			error(_("reference does not exist"));
			ret = 2;
		} അല്ലെങ്കിൽ {
			errno = failure_errno;
			error_errno(_("failed to look up reference"));
			ret = 1;
		}

		പോകുക out;
	}

out:
	strbuf_release(&unused_referent);
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cmd_refs_optimize(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
			     ഘടന repository *repo)
{
	സ്ഥിതം അക്ഷരം സ്ഥിരം * സ്ഥിരം refs_optimize_usage[] = {
		REFS_OPTIMIZE_USAGE,
		ശൂന്യ
	};

	തിരികെ pack_refs_core(argc, argv, prefix, repo, refs_optimize_usage);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cmd_refs_create(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
			   ഘടന repository *repo)
{
	സ്ഥിതം അക്ഷരം സ്ഥിരം * സ്ഥിരം refs_create_usage[] = {
		REFS_CREATE_USAGE,
		ശൂന്യ
	};
	സ്ഥിരം അക്ഷരം *message = ശൂന്യ;
	unsigned flags = 0;
	ഘടന option opts[] = {
		OPT_STRING(0, "message", &message, N_("reason"),
			   N_("reason of the update")),
		OPT_BIT(0 ,"no-deref", &flags,
			N_("update <refname> not the one it points to"),
			REF_NO_DEREF),
		OPT_BIT(0, "create-reflog", &flags, N_("create a reflog"),
			REF_FORCE_CREATE_REFLOG),
		OPT_END(),
	};
	ഘടന object_id newoid;
	സ്ഥിരം അക്ഷരം *refname;
	പൂർണ്ണസംഖ്യ ret;

	argc = parse_options(argc, argv, prefix, opts, refs_create_usage, 0);
	എങ്കിൽ (argc != 2)
		usage(_("create requires reference name and an object ID"));

	എങ്കിൽ (message && !*message)
		die(_("refusing to perform update with empty message"));

	repo_config(repo, git_default_config, ശൂന്യ);

	refname = argv[0];
	എങ്കിൽ (repo_get_oid_with_flags(repo, argv[1], &newoid, GET_OID_SKIP_AMBIGUITY_CHECK))
		die(_("invalid object ID: '%s'"), argv[1]);
	എങ്കിൽ (is_null_oid(&newoid))
		die(_("cannot create reference with null new object ID"));

	ret = refs_update_ref(get_main_ref_store(repo), message, refname,
			      &newoid, null_oid(repo->hash_algo), flags,
			      UPDATE_REFS_സന്ദേശം_ON_ERR);

	എങ്കിൽ (ret < 0)
		ret = 1;
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cmd_refs_delete(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
			   ഘടന repository *repo)
{
	സ്ഥിതം അക്ഷരം സ്ഥിരം * സ്ഥിരം refs_delete_usage[] = {
		REFS_DELETE_USAGE,
		ശൂന്യ
	};
	സ്ഥിരം അക്ഷരം *message = ശൂന്യ;
	unsigned flags = 0;
	ഘടന option opts[] = {
		OPT_STRING(0, "message", &message, N_("reason"),
			   N_("reason of the update")),
		OPT_BIT(0 ,"no-deref", &flags,
			N_("update <refname> not the one it points to"),
			REF_NO_DEREF),
		OPT_END(),
	};
	ഘടന object_id oldoid;
	സ്ഥിരം അക്ഷരം *refname;
	പൂർണ്ണസംഖ്യ ret;

	argc = parse_options(argc, argv, prefix, opts, refs_delete_usage, 0);
	എങ്കിൽ (argc < 1 || argc > 2)
		usage(_("delete requires reference name and an optional old object ID"));

	എങ്കിൽ (message && !*message)
		die(_("refusing to perform update with empty message"));

	repo_config(repo, git_default_config, ശൂന്യ);

	refname = argv[0];
	എങ്കിൽ (argc == 2) {
		എങ്കിൽ (repo_get_oid_with_flags(repo, argv[1], &oldoid, GET_OID_SKIP_AMBIGUITY_CHECK))
			die(_("invalid old object ID: '%s'"), argv[1]);
		എങ്കിൽ (is_null_oid(&oldoid))
			die(_("cannot delete reference with null old object ID"));
	}

	ret = refs_delete_ref(get_main_ref_store(repo), message, refname,
			      argc == 2 ? &oldoid : ശൂന്യ, flags);

	എങ്കിൽ (ret < 0)
		ret = 1;
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cmd_refs_update(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
			   ഘടന repository *repo)
{
	സ്ഥിതം അക്ഷരം സ്ഥിരം * സ്ഥിരം refs_update_usage[] = {
		REFS_UPDATE_USAGE,
		ശൂന്യ
	};
	സ്ഥിരം അക്ഷരം *message = ശൂന്യ;
	unsigned flags = 0;
	ഘടന option opts[] = {
		OPT_STRING(0, "message", &message, N_("reason"),
			   N_("reason of the update")),
		OPT_BIT(0 ,"no-deref", &flags,
			N_("update <refname> not the one it points to"),
			REF_NO_DEREF),
		OPT_BIT(0, "create-reflog", &flags, N_("create a reflog"),
			REF_FORCE_CREATE_REFLOG),
		OPT_END(),
	};
	ഘടന object_id newoid, oldoid;
	സ്ഥിരം അക്ഷരം *refname;
	പൂർണ്ണസംഖ്യ ret;

	argc = parse_options(argc, argv, prefix, opts, refs_update_usage, 0);
	എങ്കിൽ (argc < 2 || argc > 3)
		usage(_("update requires reference name, new value and an optional old value"));

	എങ്കിൽ (message && !*message)
		die(_("refusing to perform update with empty message"));

	repo_config(repo, git_default_config, ശൂന്യ);

	refname = argv[0];
	എങ്കിൽ (repo_get_oid_with_flags(repo, argv[1], &newoid,
				    GET_OID_SKIP_AMBIGUITY_CHECK))
		die(_("invalid new object ID: '%s'"), argv[1]);
	എങ്കിൽ (argc == 3 &&
	    repo_get_oid_with_flags(repo, argv[2], &oldoid,
				    GET_OID_SKIP_AMBIGUITY_CHECK))
		die(_("invalid old object ID: '%s'"), argv[2]);

	ret = refs_update_ref(get_main_ref_store(repo), message, refname,
			      &newoid, argc == 3 ? &oldoid : ശൂന്യ, flags,
			      UPDATE_REFS_സന്ദേശം_ON_ERR);

	എങ്കിൽ (ret < 0)
		ret = 1;
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cmd_refs_rename(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
			   ഘടന repository *repo)
{
	സ്ഥിതം അക്ഷരം സ്ഥിരം * സ്ഥിരം refs_rename_usage[] = {
		REFS_RENAME_USAGE,
		ശൂന്യ
	};
	സ്ഥിരം അക്ഷരം *message = ശൂന്യ;
	ഘടന option opts[] = {
		OPT_STRING(0, "message", &message, N_("reason"),
			   N_("reason of the update")),
		OPT_END(),
	};
	സ്ഥിരം അക്ഷരം *oldref, *newref;
	പൂർണ്ണസംഖ്യ ret;

	argc = parse_options(argc, argv, prefix, opts, refs_rename_usage, 0);
	എങ്കിൽ (argc != 2)
		usage(_("rename requires old and new reference name"));
	എങ്കിൽ (message && !*message)
		die(_("refusing to perform update with empty message"));

	repo_config(repo, git_default_config, ശൂന്യ);

	oldref = argv[0];
	newref = argv[1];

	എങ്കിൽ (check_refname_format(oldref, 0))
		die(_("invalid ref format: '%s'"), oldref);
	എങ്കിൽ (check_refname_format(newref, 0))
		die(_("invalid ref format: '%s'"), newref);

	എങ്കിൽ (!refs_ref_exists(get_main_ref_store(repo), oldref))
		die(_("reference does not exist: '%s'"), oldref);
	എങ്കിൽ (refs_ref_exists(get_main_ref_store(repo), newref))
		die(_("reference already exists: '%s'"), newref);

	ret = refs_rename_ref(get_main_ref_store(repo), oldref, newref, message);

	എങ്കിൽ (ret < 0)
		ret = 1;
	തിരികെ ret;
}

പൂർണ്ണസംഖ്യ cmd_refs(പൂർണ്ണസംഖ്യ argc,
	     സ്ഥിരം അക്ഷരം **argv,
	     സ്ഥിരം അക്ഷരം *prefix,
	     ഘടന repository *repo)
{
	സ്ഥിരം അക്ഷരം * സ്ഥിരം refs_usage[] = {
		REFS_MIGRATE_USAGE,
		REFS_VERIFY_USAGE,
		REFS_OPTIMIZE_USAGE,
		"git refs list " COMMON_USAGE_FOR_EACH_REF,
		REFS_EXISTS_USAGE,
		REFS_CREATE_USAGE,
		REFS_DELETE_USAGE,
		REFS_UPDATE_USAGE,
		REFS_RENAME_USAGE,
		ശൂന്യ,
	};
	parse_opt_subcommand_fn *fn = ശൂന്യ;
	ഘടന option opts[] = {
		OPT_GROUP(N_("Reference database")),
		OPT_SUBCOMMAND_F("migrate", &fn, cmd_refs_migrate,
				 N_("migrate the reference database to a different format"), 0),
		OPT_SUBCOMMAND_F("verify", &fn, cmd_refs_verify,
				 N_("verify the consistency of the reference database"), 0),
		OPT_SUBCOMMAND_F("optimize", &fn, cmd_refs_optimize,
				 N_("optimize the reference database"), 0),
		OPT_GROUP(N_("Reading references")),
		OPT_SUBCOMMAND_F("list", &fn, cmd_refs_list,
				 N_("list references"), 0),
		OPT_SUBCOMMAND_F("exists", &fn, cmd_refs_exists,
				 N_("check whether a reference exists"), 0),
		OPT_GROUP(N_("Writing references")),
		OPT_SUBCOMMAND_F("create", &fn, cmd_refs_create,
				 N_("create a new reference"), 0),
		OPT_SUBCOMMAND_F("delete", &fn, cmd_refs_delete,
				 N_("delete a reference"), 0),
		OPT_SUBCOMMAND_F("update", &fn, cmd_refs_update,
				 N_("update an existing reference"), 0),
		OPT_SUBCOMMAND_F("rename", &fn, cmd_refs_rename,
				 N_("rename a reference"), 0),
		OPT_END(),
	};

	argc = parse_options(argc, argv, prefix, opts, refs_usage, 0);
	തിരികെ fn(argc, argv, prefix, repo);
}
