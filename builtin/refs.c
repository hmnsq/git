#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "fsck.h"
#ಅನ್ತರ್ಭಾವ "pack-refs.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "refs.h"
#ಅನ್ತರ್ಭಾವ "strbuf.h"
#ಅನ್ತರ್ಭಾವ "worktree.h"
#ಅನ್ತರ್ಭಾವ "for-each-ref.h"
#ಅನ್ತರ್ಭಾವ "refs/refs-internal.h"

#ವ್ಯಾಖ್ಯಾನಿಸು REFS_MIGRATE_USAGE \
	N_("git refs migrate --ref-storage-format=<format> [--no-reflog] [--dry-run]")

#ವ್ಯಾಖ್ಯಾನಿಸು REFS_VERIFY_USAGE \
	N_("git refs verify [--strict] [--verbose]")

#ವ್ಯಾಖ್ಯಾನಿಸು REFS_EXISTS_USAGE \
	N_("git refs exists <ref>")

#ವ್ಯಾಖ್ಯಾನಿಸು REFS_OPTIMIZE_USAGE \
	N_("git refs optimize " PACK_REFS_OPTS)

#ವ್ಯಾಖ್ಯಾನಿಸು REFS_CREATE_USAGE \
	N_("git refs create [--message=<reason>] [--no-deref] [--create-reflog] <ref> <new-value>")

#ವ್ಯಾಖ್ಯಾನಿಸು REFS_DELETE_USAGE \
	N_("git refs delete [--message=<reason>] [--no-deref] <ref> [<old-value>]")

#ವ್ಯಾಖ್ಯಾನಿಸು REFS_UPDATE_USAGE \
	N_("git refs update [--message=<reason>] [--no-deref] [--create-reflog] <ref> <new-value> [<old-value>]")

#ವ್ಯಾಖ್ಯಾನಿಸು REFS_RENAME_USAGE \
	N_("git refs rename [--message=<reason>] <old-ref> <new-ref>")

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cmd_refs_migrate(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			    ರಚನೆ repository *repo)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ migrate_usage[] = {
		REFS_MIGRATE_USAGE,
		ಖಾಲಿ,
	};
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *format_str = ಖಾಲಿ;
	ಎಣಿಕೆ ref_storage_format format;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags = 0;
	ರಚನೆ option options[] = {
		OPT_STRING_F(0, "ref-storage-format", &format_str, N_("format"),
			N_("specify the reference storage format to convert to"),
			PARSE_OPT_NONEG),
		OPT_ALIAS_F(0, "ref-format", "ref-storage-format", PARSE_OPT_HIDDEN),
		OPT_ಬಿಟ್(0, "dry-run", &flags,
			N_("perform a non-destructive dry-run"),
			REPO_MIGRATE_REF_STORAGE_FORMAT_DRYRUN),
		OPT_ಬಿಟ್(0, "no-reflog", &flags,
			N_("drop reflogs entirely during the migration"),
			REPO_MIGRATE_REF_STORAGE_FORMAT_SKIP_REFLOG),
		OPT_ಕೊನೆ(),
	};
	ರಚನೆ strbuf errbuf = STRBUF_INIT;
	ಪೂರ್ಣಾಂಕ err;

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options, migrate_usage, 0);
	ಆದರೆ (argc)
		usage(_("too many arguments"));
	ಆದರೆ (!format_str)
		usage(_("missing --ref-storage-format=<format>"));

	format = ref_storage_format_by_name(format_str);
	ಆದರೆ (format == REF_STORAGE_FORMAT_UNKNOWN) {
		err = error(_("unknown ref storage format '%s'"), format_str);
		ಹೋಗು ಹೊರಹೋಗು;
	}

	ಆದರೆ (repo->ref_storage_format == format) {
		err = error(_("repository already uses '%s' format"),
			    ref_storage_format_ಗಮ್ಯ_name(format));
		ಹೋಗು ಹೊರಹೋಗು;
	}

	ಆದರೆ (repo_migrate_ref_storage_format(repo, format, flags, &errbuf) < 0) {
		err = error("%s", errbuf.buf);
		ಹೋಗು ಹೊರಹೋಗು;
	}

	err = 0;

ಹೊರಹೋಗು:
	strbuf_release(&errbuf);
	ಹಿಂತಿರುಗಿ err;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cmd_refs_verify(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			   ರಚನೆ repository *repo)
{
	ರಚನೆ fsck_options fsck_refs_options;
	ರಚನೆ worktree **worktrees;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ verify_usage[] = {
		REFS_VERIFY_USAGE,
		ಖಾಲಿ,
	};
	ರಚನೆ option options[] = {
		OPT_BOOL(0, "verbose", &fsck_refs_options.verbose, N_("be verbose")),
		OPT_BOOL(0, "strict", &fsck_refs_options.strict, N_("enable strict checking")),
		OPT_ಕೊನೆ(),
	};
	ಪೂರ್ಣಾಂಕ ret = 0;

	fsck_options_init(&fsck_refs_options, repo, FSCK_OPTIONS_REFS);

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options, verify_usage, 0);
	ಆದರೆ (argc)
		usage(_("'git refs verify' takes no arguments"));

	repo_config(repo, git_fsck_config, &fsck_refs_options);
	prepare_repo_settings(repo);

	worktrees = get_worktrees_without_reading_ತಲೆ(repo);
	ಗಾಗಿ (ಗಾತ್ರ_t i = 0; worktrees[i]; i++)
		ret |= refs_fsck(get_worktree_ref_store(worktrees[i]),
				 &fsck_refs_options, worktrees[i]);

	fsck_options_clear(&fsck_refs_options);
	free_worktrees(worktrees);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cmd_refs_list(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			   ರಚನೆ repository *repo)
{
	ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ * ಸ್ಥಿರಾಂಕ refs_list_usage[] = {
		N_("git refs list " COMMON_USAGE_FOR_EACH_REF),
		ಖಾಲಿ
	};

	ಹಿಂತಿರುಗಿ for_each_ref_core(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, repo, refs_list_usage);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cmd_refs_exists(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			   ರಚನೆ repository *repo)
{
	ರಚನೆ strbuf ಬಳಸದ_referent = STRBUF_INIT;
	ರಚನೆ object_id ಬಳಸದ_oid;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಬಳಸದ_type;
	ಪೂರ್ಣಾಂಕ failure_errno = 0;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ref;
	ಪೂರ್ಣಾಂಕ ret = 0;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ exists_usage[] = {
		REFS_EXISTS_USAGE,
		ಖಾಲಿ,
	};
	ರಚನೆ option options[] = {
		OPT_ಕೊನೆ(),
	};

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options, exists_usage, 0);
	ಆದರೆ (argc != 1)
		die(_("'git refs exists' requires a reference"));

	ref = *argv++;
	ಆದರೆ (refs_read_raw_ref(get_main_ref_store(repo), ref,
			      &ಬಳಸದ_oid, &ಬಳಸದ_referent, &ಬಳಸದ_type,
			      &failure_errno)) {
		ಆದರೆ (failure_errno == ENOENT || failure_errno == EISDIR) {
			error(_("reference does not exist"));
			ret = 2;
		} ಇಲ್ಲದಿದ್ದರೆ {
			errno = failure_errno;
			error_errno(_("failed to look up reference"));
			ret = 1;
		}

		ಹೋಗು ಹೊರಹೋಗು;
	}

ಹೊರಹೋಗು:
	strbuf_release(&ಬಳಸದ_referent);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cmd_refs_optimize(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			     ರಚನೆ repository *repo)
{
	ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ * ಸ್ಥಿರಾಂಕ refs_optimize_usage[] = {
		REFS_OPTIMIZE_USAGE,
		ಖಾಲಿ
	};

	ಹಿಂತಿರುಗಿ pack_refs_core(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, repo, refs_optimize_usage);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cmd_refs_create(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			   ರಚನೆ repository *repo)
{
	ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ * ಸ್ಥಿರಾಂಕ refs_create_usage[] = {
		REFS_CREATE_USAGE,
		ಖಾಲಿ
	};
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ_2 = ಖಾಲಿ;
	ಚಿಹ್ನೆರಹಿತ flags = 0;
	ರಚನೆ option opts[] = {
		OPT_STRING(0, "message", &ಸಂದೇಶ_2, N_("reason"),
			   N_("reason of the update")),
		OPT_ಬಿಟ್(0 ,"no-deref", &flags,
			N_("update <refname> not the one it points to"),
			REF_NO_DEREF),
		OPT_ಬಿಟ್(0, "create-reflog", &flags, N_("create a reflog"),
			REF_FORCE_CREATE_REFLOG),
		OPT_ಕೊನೆ(),
	};
	ರಚನೆ object_id newoid;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *refname;
	ಪೂರ್ಣಾಂಕ ret;

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, opts, refs_create_usage, 0);
	ಆದರೆ (argc != 2)
		usage(_("create requires reference name and an object ID"));

	ಆದರೆ (ಸಂದೇಶ_2 && !*ಸಂದೇಶ_2)
		die(_("refusing to perform update with empty message"));

	repo_config(repo, git_default_config, ಖಾಲಿ);

	refname = argv[0];
	ಆದರೆ (repo_get_oid_with_flags(repo, argv[1], &newoid, GET_OID_SKIP_AMBIGUITY_CHECK))
		die(_("invalid object ID: '%s'"), argv[1]);
	ಆದರೆ (is_null_oid(&newoid))
		die(_("cannot create reference with null new object ID"));

	ret = refs_update_ref(get_main_ref_store(repo), ಸಂದೇಶ_2, refname,
			      &newoid, null_oid(repo->hash_algo), flags,
			      UPDATE_REFS_ಸಂದೇಶ_ON_ERR);

	ಆದರೆ (ret < 0)
		ret = 1;
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cmd_refs_delete(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			   ರಚನೆ repository *repo)
{
	ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ * ಸ್ಥಿರಾಂಕ refs_delete_usage[] = {
		REFS_DELETE_USAGE,
		ಖಾಲಿ
	};
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ_2 = ಖಾಲಿ;
	ಚಿಹ್ನೆರಹಿತ flags = 0;
	ರಚನೆ option opts[] = {
		OPT_STRING(0, "message", &ಸಂದೇಶ_2, N_("reason"),
			   N_("reason of the update")),
		OPT_ಬಿಟ್(0 ,"no-deref", &flags,
			N_("update <refname> not the one it points to"),
			REF_NO_DEREF),
		OPT_ಕೊನೆ(),
	};
	ರಚನೆ object_id oldoid;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *refname;
	ಪೂರ್ಣಾಂಕ ret;

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, opts, refs_delete_usage, 0);
	ಆದರೆ (argc < 1 || argc > 2)
		usage(_("delete requires reference name and an optional old object ID"));

	ಆದರೆ (ಸಂದೇಶ_2 && !*ಸಂದೇಶ_2)
		die(_("refusing to perform update with empty message"));

	repo_config(repo, git_default_config, ಖಾಲಿ);

	refname = argv[0];
	ಆದರೆ (argc == 2) {
		ಆದರೆ (repo_get_oid_with_flags(repo, argv[1], &oldoid, GET_OID_SKIP_AMBIGUITY_CHECK))
			die(_("invalid old object ID: '%s'"), argv[1]);
		ಆದರೆ (is_null_oid(&oldoid))
			die(_("cannot delete reference with null old object ID"));
	}

	ret = refs_delete_ref(get_main_ref_store(repo), ಸಂದೇಶ_2, refname,
			      argc == 2 ? &oldoid : ಖಾಲಿ, flags);

	ಆದರೆ (ret < 0)
		ret = 1;
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cmd_refs_update(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			   ರಚನೆ repository *repo)
{
	ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ * ಸ್ಥಿರಾಂಕ refs_update_usage[] = {
		REFS_UPDATE_USAGE,
		ಖಾಲಿ
	};
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ_2 = ಖಾಲಿ;
	ಚಿಹ್ನೆರಹಿತ flags = 0;
	ರಚನೆ option opts[] = {
		OPT_STRING(0, "message", &ಸಂದೇಶ_2, N_("reason"),
			   N_("reason of the update")),
		OPT_ಬಿಟ್(0 ,"no-deref", &flags,
			N_("update <refname> not the one it points to"),
			REF_NO_DEREF),
		OPT_ಬಿಟ್(0, "create-reflog", &flags, N_("create a reflog"),
			REF_FORCE_CREATE_REFLOG),
		OPT_ಕೊನೆ(),
	};
	ರಚನೆ object_id newoid, oldoid;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *refname;
	ಪೂರ್ಣಾಂಕ ret;

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, opts, refs_update_usage, 0);
	ಆದರೆ (argc < 2 || argc > 3)
		usage(_("update requires reference name, new value and an optional old value"));

	ಆದರೆ (ಸಂದೇಶ_2 && !*ಸಂದೇಶ_2)
		die(_("refusing to perform update with empty message"));

	repo_config(repo, git_default_config, ಖಾಲಿ);

	refname = argv[0];
	ಆದರೆ (repo_get_oid_with_flags(repo, argv[1], &newoid,
				    GET_OID_SKIP_AMBIGUITY_CHECK))
		die(_("invalid new object ID: '%s'"), argv[1]);
	ಆದರೆ (argc == 3 &&
	    repo_get_oid_with_flags(repo, argv[2], &oldoid,
				    GET_OID_SKIP_AMBIGUITY_CHECK))
		die(_("invalid old object ID: '%s'"), argv[2]);

	ret = refs_update_ref(get_main_ref_store(repo), ಸಂದೇಶ_2, refname,
			      &newoid, argc == 3 ? &oldoid : ಖಾಲಿ, flags,
			      UPDATE_REFS_ಸಂದೇಶ_ON_ERR);

	ಆದರೆ (ret < 0)
		ret = 1;
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cmd_refs_rename(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			   ರಚನೆ repository *repo)
{
	ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ * ಸ್ಥಿರಾಂಕ refs_rename_usage[] = {
		REFS_RENAME_USAGE,
		ಖಾಲಿ
	};
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ_2 = ಖಾಲಿ;
	ರಚನೆ option opts[] = {
		OPT_STRING(0, "message", &ಸಂದೇಶ_2, N_("reason"),
			   N_("reason of the update")),
		OPT_ಕೊನೆ(),
	};
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *oldref, *newref;
	ಪೂರ್ಣಾಂಕ ret;

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, opts, refs_rename_usage, 0);
	ಆದರೆ (argc != 2)
		usage(_("rename requires old and new reference name"));
	ಆದರೆ (ಸಂದೇಶ_2 && !*ಸಂದೇಶ_2)
		die(_("refusing to perform update with empty message"));

	repo_config(repo, git_default_config, ಖಾಲಿ);

	oldref = argv[0];
	newref = argv[1];

	ಆದರೆ (check_refname_format(oldref, 0))
		die(_("invalid ref format: '%s'"), oldref);
	ಆದರೆ (check_refname_format(newref, 0))
		die(_("invalid ref format: '%s'"), newref);

	ಆದರೆ (!refs_ref_exists(get_main_ref_store(repo), oldref))
		die(_("reference does not exist: '%s'"), oldref);
	ಆದರೆ (refs_ref_exists(get_main_ref_store(repo), newref))
		die(_("reference already exists: '%s'"), newref);

	ret = refs_rename_ref(get_main_ref_store(repo), oldref, newref, ಸಂದೇಶ_2);

	ಆದರೆ (ret < 0)
		ret = 1;
	ಹಿಂತಿರುಗಿ ret;
}

ಪೂರ್ಣಾಂಕ cmd_refs(ಪೂರ್ಣಾಂಕ argc,
	     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
	     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
	     ರಚನೆ repository *repo)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ refs_usage[] = {
		REFS_MIGRATE_USAGE,
		REFS_VERIFY_USAGE,
		REFS_OPTIMIZE_USAGE,
		"git refs list " COMMON_USAGE_FOR_EACH_REF,
		REFS_EXISTS_USAGE,
		REFS_CREATE_USAGE,
		REFS_DELETE_USAGE,
		REFS_UPDATE_USAGE,
		REFS_RENAME_USAGE,
		ಖಾಲಿ,
	};
	parse_opt_subcommand_fn *fn = ಖಾಲಿ;
	ರಚನೆ option opts[] = {
		OPT_ಗುಂಪು(N_("Reference database")),
		OPT_SUBCOMMAND_F("migrate", &fn, cmd_refs_migrate,
				 N_("migrate the reference database to a different format"), 0),
		OPT_SUBCOMMAND_F("verify", &fn, cmd_refs_verify,
				 N_("verify the consistency of the reference database"), 0),
		OPT_SUBCOMMAND_F("optimize", &fn, cmd_refs_optimize,
				 N_("optimize the reference database"), 0),
		OPT_ಗುಂಪು(N_("Reading references")),
		OPT_SUBCOMMAND_F("list", &fn, cmd_refs_list,
				 N_("list references"), 0),
		OPT_SUBCOMMAND_F("exists", &fn, cmd_refs_exists,
				 N_("check whether a reference exists"), 0),
		OPT_ಗುಂಪು(N_("Writing references")),
		OPT_SUBCOMMAND_F("create", &fn, cmd_refs_create,
				 N_("create a new reference"), 0),
		OPT_SUBCOMMAND_F("delete", &fn, cmd_refs_delete,
				 N_("delete a reference"), 0),
		OPT_SUBCOMMAND_F("update", &fn, cmd_refs_update,
				 N_("update an existing reference"), 0),
		OPT_SUBCOMMAND_F("rename", &fn, cmd_refs_rename,
				 N_("rename a reference"), 0),
		OPT_ಕೊನೆ(),
	};

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, opts, refs_usage, 0);
	ಹಿಂತಿರುಗಿ fn(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, repo);
}
