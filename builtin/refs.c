#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "fsck.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "pack-refs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "refs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "strbuf.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "worktree.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "for-each-ref.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "refs/refs-internal.h"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ REFS_MIGRATE_USAGE \
	N_("git refs migrate --ref-storage-format=<format> [--no-reflog] [--dry-run]")

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ REFS_VERIFY_USAGE \
	N_("git refs verify [--strict] [--verbose]")

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ REFS_EXISTS_USAGE \
	N_("git refs exists <ref>")

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ REFS_OPTIMIZE_USAGE \
	N_("git refs optimize " PACK_REFS_OPTS)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ REFS_CREATE_USAGE \
	N_("git refs create [--message=<reason>] [--no-deref] [--create-reflog] <ref> <new-value>")

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ REFS_DELETE_USAGE \
	N_("git refs delete [--message=<reason>] [--no-deref] <ref> [<old-value>]")

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ REFS_UPDATE_USAGE \
	N_("git refs update [--message=<reason>] [--no-deref] [--create-reflog] <ref> <new-value> [<old-value>]")

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ REFS_RENAME_USAGE \
	N_("git refs rename [--message=<reason>] <old-ref> <new-ref>")

ਸਥਿਰ ਪੂਰਨ cmd_refs_migrate(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			    ਬਣਤਰ repository *repo)
{
	ਅਚਲ ਅੱਖਰ * ਅਚਲ migrate_usage[] = {
		REFS_MIGRATE_USAGE,
		ਨਿਰਾ,
	};
	ਅਚਲ ਅੱਖਰ *format_str = ਨਿਰਾ;
	ਗਿਣਤੀ ref_storage_format format;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags = 0;
	ਬਣਤਰ option options[] = {
		OPT_STRING_F(0, "ref-storage-format", &format_str, N_("format"),
			N_("specify the reference storage format to convert to"),
			PARSE_OPT_NONEG),
		OPT_ALIAS_F(0, "ref-format", "ref-storage-format", PARSE_OPT_HIDDEN),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ(0, "dry-run", &flags,
			N_("perform a non-destructive dry-run"),
			REPO_MIGRATE_REF_STORAGE_FORMAT_DRYRUN),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ(0, "no-reflog", &flags,
			N_("drop reflogs entirely during the migration"),
			REPO_MIGRATE_REF_STORAGE_FORMAT_SKIP_REFLOG),
		OPT_ਸਿਰਾ(),
	};
	ਬਣਤਰ strbuf errbuf = STRBUF_INIT;
	ਪੂਰਨ err;

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, migrate_usage, 0);
	ਜੇ (argc)
		usage(_("too many arguments"));
	ਜੇ (!format_str)
		usage(_("missing --ref-storage-format=<format>"));

	format = ref_storage_format_by_name(format_str);
	ਜੇ (format == REF_STORAGE_FORMAT_UNKNOWN) {
		err = error(_("unknown ref storage format '%s'"), format_str);
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	ਜੇ (repo->ref_storage_format == format) {
		err = error(_("repository already uses '%s' format"),
			    ref_storage_format_ਮੰਜ਼ਿਲ_name(format));
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	ਜੇ (repo_migrate_ref_storage_format(repo, format, flags, &errbuf) < 0) {
		err = error("%s", errbuf.buf);
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	err = 0;

ਬਾਹਰਲਾ:
	strbuf_release(&errbuf);
	ਵਾਪਸ err;
}

ਸਥਿਰ ਪੂਰਨ cmd_refs_verify(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			   ਬਣਤਰ repository *repo)
{
	ਬਣਤਰ fsck_options fsck_refs_options;
	ਬਣਤਰ worktree **worktrees;
	ਅਚਲ ਅੱਖਰ * ਅਚਲ verify_usage[] = {
		REFS_VERIFY_USAGE,
		ਨਿਰਾ,
	};
	ਬਣਤਰ option options[] = {
		OPT_BOOL(0, "verbose", &fsck_refs_options.verbose, N_("be verbose")),
		OPT_BOOL(0, "strict", &fsck_refs_options.strict, N_("enable strict checking")),
		OPT_ਸਿਰਾ(),
	};
	ਪੂਰਨ ret = 0;

	fsck_options_init(&fsck_refs_options, repo, FSCK_OPTIONS_REFS);

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, verify_usage, 0);
	ਜੇ (argc)
		usage(_("'git refs verify' takes no arguments"));

	repo_config(repo, git_fsck_config, &fsck_refs_options);
	prepare_repo_settings(repo);

	worktrees = get_worktrees_without_reading_ਸਿਰ(repo);
	ਲਈ (ਮਾਪ_t i = 0; worktrees[i]; i++)
		ret |= refs_fsck(get_worktree_ref_store(worktrees[i]),
				 &fsck_refs_options, worktrees[i]);

	fsck_options_clear(&fsck_refs_options);
	free_worktrees(worktrees);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ cmd_refs_list(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			   ਬਣਤਰ repository *repo)
{
	ਸਥਿਰ ਅੱਖਰ ਅਚਲ * ਅਚਲ refs_list_usage[] = {
		N_("git refs list " COMMON_USAGE_FOR_EACH_REF),
		ਨਿਰਾ
	};

	ਵਾਪਸ for_each_ref_core(argc, argv, ਅਗੇਤਰ, repo, refs_list_usage);
}

ਸਥਿਰ ਪੂਰਨ cmd_refs_exists(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			   ਬਣਤਰ repository *repo)
{
	ਬਣਤਰ strbuf ਅਣਵਰਤਿਆ_referent = STRBUF_INIT;
	ਬਣਤਰ object_id ਅਣਵਰਤਿਆ_oid;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅਣਵਰਤਿਆ_type;
	ਪੂਰਨ failure_errno = 0;
	ਅਚਲ ਅੱਖਰ *ref;
	ਪੂਰਨ ret = 0;
	ਅਚਲ ਅੱਖਰ * ਅਚਲ exists_usage[] = {
		REFS_EXISTS_USAGE,
		ਨਿਰਾ,
	};
	ਬਣਤਰ option options[] = {
		OPT_ਸਿਰਾ(),
	};

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, exists_usage, 0);
	ਜੇ (argc != 1)
		die(_("'git refs exists' requires a reference"));

	ref = *argv++;
	ਜੇ (refs_read_raw_ref(get_main_ref_store(repo), ref,
			      &ਅਣਵਰਤਿਆ_oid, &ਅਣਵਰਤਿਆ_referent, &ਅਣਵਰਤਿਆ_type,
			      &failure_errno)) {
		ਜੇ (failure_errno == ENOENT || failure_errno == EISDIR) {
			error(_("reference does not exist"));
			ret = 2;
		} ਨਹੀਂਤਾਂ {
			errno = failure_errno;
			error_errno(_("failed to look up reference"));
			ret = 1;
		}

		ਜਾਓ ਬਾਹਰਲਾ;
	}

ਬਾਹਰਲਾ:
	strbuf_release(&ਅਣਵਰਤਿਆ_referent);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ cmd_refs_optimize(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			     ਬਣਤਰ repository *repo)
{
	ਸਥਿਰ ਅੱਖਰ ਅਚਲ * ਅਚਲ refs_optimize_usage[] = {
		REFS_OPTIMIZE_USAGE,
		ਨਿਰਾ
	};

	ਵਾਪਸ pack_refs_core(argc, argv, ਅਗੇਤਰ, repo, refs_optimize_usage);
}

ਸਥਿਰ ਪੂਰਨ cmd_refs_create(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			   ਬਣਤਰ repository *repo)
{
	ਸਥਿਰ ਅੱਖਰ ਅਚਲ * ਅਚਲ refs_create_usage[] = {
		REFS_CREATE_USAGE,
		ਨਿਰਾ
	};
	ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ_2 = ਨਿਰਾ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ flags = 0;
	ਬਣਤਰ option opts[] = {
		OPT_STRING(0, "message", &ਸੁਨੇਹਾ_2, N_("reason"),
			   N_("reason of the update")),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ(0 ,"no-deref", &flags,
			N_("update <refname> not the one it points to"),
			REF_NO_DEREF),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ(0, "create-reflog", &flags, N_("create a reflog"),
			REF_FORCE_CREATE_REFLOG),
		OPT_ਸਿਰਾ(),
	};
	ਬਣਤਰ object_id newoid;
	ਅਚਲ ਅੱਖਰ *refname;
	ਪੂਰਨ ret;

	argc = parse_options(argc, argv, ਅਗੇਤਰ, opts, refs_create_usage, 0);
	ਜੇ (argc != 2)
		usage(_("create requires reference name and an object ID"));

	ਜੇ (ਸੁਨੇਹਾ_2 && !*ਸੁਨੇਹਾ_2)
		die(_("refusing to perform update with empty message"));

	repo_config(repo, git_default_config, ਨਿਰਾ);

	refname = argv[0];
	ਜੇ (repo_get_oid_with_flags(repo, argv[1], &newoid, GET_OID_SKIP_AMBIGUITY_CHECK))
		die(_("invalid object ID: '%s'"), argv[1]);
	ਜੇ (is_null_oid(&newoid))
		die(_("cannot create reference with null new object ID"));

	ret = refs_update_ref(get_main_ref_store(repo), ਸੁਨੇਹਾ_2, refname,
			      &newoid, null_oid(repo->hash_algo), flags,
			      UPDATE_REFS_ਸੁਨੇਹਾ_ON_ERR);

	ਜੇ (ret < 0)
		ret = 1;
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ cmd_refs_delete(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			   ਬਣਤਰ repository *repo)
{
	ਸਥਿਰ ਅੱਖਰ ਅਚਲ * ਅਚਲ refs_delete_usage[] = {
		REFS_DELETE_USAGE,
		ਨਿਰਾ
	};
	ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ_2 = ਨਿਰਾ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ flags = 0;
	ਬਣਤਰ option opts[] = {
		OPT_STRING(0, "message", &ਸੁਨੇਹਾ_2, N_("reason"),
			   N_("reason of the update")),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ(0 ,"no-deref", &flags,
			N_("update <refname> not the one it points to"),
			REF_NO_DEREF),
		OPT_ਸਿਰਾ(),
	};
	ਬਣਤਰ object_id oldoid;
	ਅਚਲ ਅੱਖਰ *refname;
	ਪੂਰਨ ret;

	argc = parse_options(argc, argv, ਅਗੇਤਰ, opts, refs_delete_usage, 0);
	ਜੇ (argc < 1 || argc > 2)
		usage(_("delete requires reference name and an optional old object ID"));

	ਜੇ (ਸੁਨੇਹਾ_2 && !*ਸੁਨੇਹਾ_2)
		die(_("refusing to perform update with empty message"));

	repo_config(repo, git_default_config, ਨਿਰਾ);

	refname = argv[0];
	ਜੇ (argc == 2) {
		ਜੇ (repo_get_oid_with_flags(repo, argv[1], &oldoid, GET_OID_SKIP_AMBIGUITY_CHECK))
			die(_("invalid old object ID: '%s'"), argv[1]);
		ਜੇ (is_null_oid(&oldoid))
			die(_("cannot delete reference with null old object ID"));
	}

	ret = refs_delete_ref(get_main_ref_store(repo), ਸੁਨੇਹਾ_2, refname,
			      argc == 2 ? &oldoid : ਨਿਰਾ, flags);

	ਜੇ (ret < 0)
		ret = 1;
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ cmd_refs_update(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			   ਬਣਤਰ repository *repo)
{
	ਸਥਿਰ ਅੱਖਰ ਅਚਲ * ਅਚਲ refs_update_usage[] = {
		REFS_UPDATE_USAGE,
		ਨਿਰਾ
	};
	ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ_2 = ਨਿਰਾ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ flags = 0;
	ਬਣਤਰ option opts[] = {
		OPT_STRING(0, "message", &ਸੁਨੇਹਾ_2, N_("reason"),
			   N_("reason of the update")),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ(0 ,"no-deref", &flags,
			N_("update <refname> not the one it points to"),
			REF_NO_DEREF),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ(0, "create-reflog", &flags, N_("create a reflog"),
			REF_FORCE_CREATE_REFLOG),
		OPT_ਸਿਰਾ(),
	};
	ਬਣਤਰ object_id newoid, oldoid;
	ਅਚਲ ਅੱਖਰ *refname;
	ਪੂਰਨ ret;

	argc = parse_options(argc, argv, ਅਗੇਤਰ, opts, refs_update_usage, 0);
	ਜੇ (argc < 2 || argc > 3)
		usage(_("update requires reference name, new value and an optional old value"));

	ਜੇ (ਸੁਨੇਹਾ_2 && !*ਸੁਨੇਹਾ_2)
		die(_("refusing to perform update with empty message"));

	repo_config(repo, git_default_config, ਨਿਰਾ);

	refname = argv[0];
	ਜੇ (repo_get_oid_with_flags(repo, argv[1], &newoid,
				    GET_OID_SKIP_AMBIGUITY_CHECK))
		die(_("invalid new object ID: '%s'"), argv[1]);
	ਜੇ (argc == 3 &&
	    repo_get_oid_with_flags(repo, argv[2], &oldoid,
				    GET_OID_SKIP_AMBIGUITY_CHECK))
		die(_("invalid old object ID: '%s'"), argv[2]);

	ret = refs_update_ref(get_main_ref_store(repo), ਸੁਨੇਹਾ_2, refname,
			      &newoid, argc == 3 ? &oldoid : ਨਿਰਾ, flags,
			      UPDATE_REFS_ਸੁਨੇਹਾ_ON_ERR);

	ਜੇ (ret < 0)
		ret = 1;
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ cmd_refs_rename(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			   ਬਣਤਰ repository *repo)
{
	ਸਥਿਰ ਅੱਖਰ ਅਚਲ * ਅਚਲ refs_rename_usage[] = {
		REFS_RENAME_USAGE,
		ਨਿਰਾ
	};
	ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ_2 = ਨਿਰਾ;
	ਬਣਤਰ option opts[] = {
		OPT_STRING(0, "message", &ਸੁਨੇਹਾ_2, N_("reason"),
			   N_("reason of the update")),
		OPT_ਸਿਰਾ(),
	};
	ਅਚਲ ਅੱਖਰ *oldref, *newref;
	ਪੂਰਨ ret;

	argc = parse_options(argc, argv, ਅਗੇਤਰ, opts, refs_rename_usage, 0);
	ਜੇ (argc != 2)
		usage(_("rename requires old and new reference name"));
	ਜੇ (ਸੁਨੇਹਾ_2 && !*ਸੁਨੇਹਾ_2)
		die(_("refusing to perform update with empty message"));

	repo_config(repo, git_default_config, ਨਿਰਾ);

	oldref = argv[0];
	newref = argv[1];

	ਜੇ (check_refname_format(oldref, 0))
		die(_("invalid ref format: '%s'"), oldref);
	ਜੇ (check_refname_format(newref, 0))
		die(_("invalid ref format: '%s'"), newref);

	ਜੇ (!refs_ref_exists(get_main_ref_store(repo), oldref))
		die(_("reference does not exist: '%s'"), oldref);
	ਜੇ (refs_ref_exists(get_main_ref_store(repo), newref))
		die(_("reference already exists: '%s'"), newref);

	ret = refs_rename_ref(get_main_ref_store(repo), oldref, newref, ਸੁਨੇਹਾ_2);

	ਜੇ (ret < 0)
		ret = 1;
	ਵਾਪਸ ret;
}

ਪੂਰਨ cmd_refs(ਪੂਰਨ argc,
	     ਅਚਲ ਅੱਖਰ **argv,
	     ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
	     ਬਣਤਰ repository *repo)
{
	ਅਚਲ ਅੱਖਰ * ਅਚਲ refs_usage[] = {
		REFS_MIGRATE_USAGE,
		REFS_VERIFY_USAGE,
		REFS_OPTIMIZE_USAGE,
		"git refs list " COMMON_USAGE_FOR_EACH_REF,
		REFS_EXISTS_USAGE,
		REFS_CREATE_USAGE,
		REFS_DELETE_USAGE,
		REFS_UPDATE_USAGE,
		REFS_RENAME_USAGE,
		ਨਿਰਾ,
	};
	parse_opt_subcommand_fn *fn = ਨਿਰਾ;
	ਬਣਤਰ option opts[] = {
		OPT_ਸਮੂਹ(N_("Reference database")),
		OPT_SUBCOMMAND_F("migrate", &fn, cmd_refs_migrate,
				 N_("migrate the reference database to a different format"), 0),
		OPT_SUBCOMMAND_F("verify", &fn, cmd_refs_verify,
				 N_("verify the consistency of the reference database"), 0),
		OPT_SUBCOMMAND_F("optimize", &fn, cmd_refs_optimize,
				 N_("optimize the reference database"), 0),
		OPT_ਸਮੂਹ(N_("Reading references")),
		OPT_SUBCOMMAND_F("list", &fn, cmd_refs_list,
				 N_("list references"), 0),
		OPT_SUBCOMMAND_F("exists", &fn, cmd_refs_exists,
				 N_("check whether a reference exists"), 0),
		OPT_ਸਮੂਹ(N_("Writing references")),
		OPT_SUBCOMMAND_F("create", &fn, cmd_refs_create,
				 N_("create a new reference"), 0),
		OPT_SUBCOMMAND_F("delete", &fn, cmd_refs_delete,
				 N_("delete a reference"), 0),
		OPT_SUBCOMMAND_F("update", &fn, cmd_refs_update,
				 N_("update an existing reference"), 0),
		OPT_SUBCOMMAND_F("rename", &fn, cmd_refs_rename,
				 N_("rename a reference"), 0),
		OPT_ਸਿਰਾ(),
	};

	argc = parse_options(argc, argv, ਅਗੇਤਰ, opts, refs_usage, 0);
	ਵਾਪਸ fn(argc, argv, ਅਗੇਤਰ, repo);
}
