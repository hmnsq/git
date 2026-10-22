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

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmd_refs_migrate(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
			    ಸಂರಚನಾ repository *repo)
{
	ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ migrate_usage[] = {
		REFS_MIGRATE_USAGE,
		ರಿಕ್ತ,
	};
	ಧ್ರುವ ವರ್ಣ *format_str = ರಿಕ್ತ;
	ಗಣನಾ ref_storage_format format;
	unsigned ಪೂರ್ಣಾಙ್ಕ flags = 0;
	ಸಂರಚನಾ option options[] = {
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
	ಸಂರಚನಾ strbuf errbuf = STRBUF_INIT;
	ಪೂರ್ಣಾಙ್ಕ err;

	argc = parse_options(argc, argv, prefix, options, migrate_usage, 0);
	ಯದಿ (argc)
		usage(_("too many arguments"));
	ಯದಿ (!format_str)
		usage(_("missing --ref-storage-format=<format>"));

	format = ref_storage_format_by_name(format_str);
	ಯದಿ (format == REF_STORAGE_FORMAT_UNKNOWN) {
		err = error(_("unknown ref storage format '%s'"), format_str);
		ಗಚ್ಛ out;
	}

	ಯದಿ (repo->ref_storage_format == format) {
		err = error(_("repository already uses '%s' format"),
			    ref_storage_format_to_name(format));
		ಗಚ್ಛ out;
	}

	ಯದಿ (repo_migrate_ref_storage_format(repo, format, flags, &errbuf) < 0) {
		err = error("%s", errbuf.buf);
		ಗಚ್ಛ out;
	}

	err = 0;

out:
	strbuf_release(&errbuf);
	ಪ್ರತ್ಯಾಗಮ err;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmd_refs_verify(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
			   ಸಂರಚನಾ repository *repo)
{
	ಸಂರಚನಾ fsck_options fsck_refs_options;
	ಸಂರಚನಾ worktree **worktrees;
	ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ verify_usage[] = {
		REFS_VERIFY_USAGE,
		ರಿಕ್ತ,
	};
	ಸಂರಚನಾ option options[] = {
		OPT_BOOL(0, "verbose", &fsck_refs_options.verbose, N_("be verbose")),
		OPT_BOOL(0, "strict", &fsck_refs_options.strict, N_("enable strict checking")),
		OPT_END(),
	};
	ಪೂರ್ಣಾಙ್ಕ ret = 0;

	fsck_options_init(&fsck_refs_options, repo, FSCK_OPTIONS_REFS);

	argc = parse_options(argc, argv, prefix, options, verify_usage, 0);
	ಯದಿ (argc)
		usage(_("'git refs verify' takes no arguments"));

	repo_config(repo, git_fsck_config, &fsck_refs_options);
	prepare_repo_settings(repo);

	worktrees = get_worktrees_without_reading_head(repo);
	ಪ್ರತಿ (size_t i = 0; worktrees[i]; i++)
		ret |= refs_fsck(get_worktree_ref_store(worktrees[i]),
				 &fsck_refs_options, worktrees[i]);

	fsck_options_clear(&fsck_refs_options);
	free_worktrees(worktrees);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmd_refs_list(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
			   ಸಂರಚನಾ repository *repo)
{
	ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ * ಧ್ರುವ refs_list_usage[] = {
		N_("git refs list " COMMON_USAGE_FOR_EACH_REF),
		ರಿಕ್ತ
	};

	ಪ್ರತ್ಯಾಗಮ for_each_ref_core(argc, argv, prefix, repo, refs_list_usage);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmd_refs_exists(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
			   ಸಂರಚನಾ repository *repo)
{
	ಸಂರಚನಾ strbuf unused_referent = STRBUF_INIT;
	ಸಂರಚನಾ object_id unused_oid;
	unsigned ಪೂರ್ಣಾಙ್ಕ unused_type;
	ಪೂರ್ಣಾಙ್ಕ failure_errno = 0;
	ಧ್ರುವ ವರ್ಣ *ref;
	ಪೂರ್ಣಾಙ್ಕ ret = 0;
	ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ exists_usage[] = {
		REFS_EXISTS_USAGE,
		ರಿಕ್ತ,
	};
	ಸಂರಚನಾ option options[] = {
		OPT_END(),
	};

	argc = parse_options(argc, argv, prefix, options, exists_usage, 0);
	ಯದಿ (argc != 1)
		die(_("'git refs exists' requires a reference"));

	ref = *argv++;
	ಯದಿ (refs_read_raw_ref(get_main_ref_store(repo), ref,
			      &unused_oid, &unused_referent, &unused_type,
			      &failure_errno)) {
		ಯದಿ (failure_errno == ENOENT || failure_errno == EISDIR) {
			error(_("reference does not exist"));
			ret = 2;
		} ಅನ್ಯಥಾ {
			errno = failure_errno;
			error_errno(_("failed to look up reference"));
			ret = 1;
		}

		ಗಚ್ಛ out;
	}

out:
	strbuf_release(&unused_referent);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmd_refs_optimize(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
			     ಸಂರಚನಾ repository *repo)
{
	ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ * ಧ್ರುವ refs_optimize_usage[] = {
		REFS_OPTIMIZE_USAGE,
		ರಿಕ್ತ
	};

	ಪ್ರತ್ಯಾಗಮ pack_refs_core(argc, argv, prefix, repo, refs_optimize_usage);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmd_refs_create(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
			   ಸಂರಚನಾ repository *repo)
{
	ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ * ಧ್ರುವ refs_create_usage[] = {
		REFS_CREATE_USAGE,
		ರಿಕ್ತ
	};
	ಧ್ರುವ ವರ್ಣ *message = ರಿಕ್ತ;
	unsigned flags = 0;
	ಸಂರಚನಾ option opts[] = {
		OPT_STRING(0, "message", &message, N_("reason"),
			   N_("reason of the update")),
		OPT_BIT(0 ,"no-deref", &flags,
			N_("update <refname> not the one it points to"),
			REF_NO_DEREF),
		OPT_BIT(0, "create-reflog", &flags, N_("create a reflog"),
			REF_FORCE_CREATE_REFLOG),
		OPT_END(),
	};
	ಸಂರಚನಾ object_id newoid;
	ಧ್ರುವ ವರ್ಣ *refname;
	ಪೂರ್ಣಾಙ್ಕ ret;

	argc = parse_options(argc, argv, prefix, opts, refs_create_usage, 0);
	ಯದಿ (argc != 2)
		usage(_("create requires reference name and an object ID"));

	ಯದಿ (message && !*message)
		die(_("refusing to perform update with empty message"));

	repo_config(repo, git_default_config, ರಿಕ್ತ);

	refname = argv[0];
	ಯದಿ (repo_get_oid_with_flags(repo, argv[1], &newoid, GET_OID_SKIP_AMBIGUITY_CHECK))
		die(_("invalid object ID: '%s'"), argv[1]);
	ಯದಿ (is_null_oid(&newoid))
		die(_("cannot create reference with null new object ID"));

	ret = refs_update_ref(get_main_ref_store(repo), message, refname,
			      &newoid, null_oid(repo->hash_algo), flags,
			      UPDATE_REFS_ಸಂದೇಶ_ON_ERR);

	ಯದಿ (ret < 0)
		ret = 1;
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmd_refs_delete(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
			   ಸಂರಚನಾ repository *repo)
{
	ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ * ಧ್ರುವ refs_delete_usage[] = {
		REFS_DELETE_USAGE,
		ರಿಕ್ತ
	};
	ಧ್ರುವ ವರ್ಣ *message = ರಿಕ್ತ;
	unsigned flags = 0;
	ಸಂರಚನಾ option opts[] = {
		OPT_STRING(0, "message", &message, N_("reason"),
			   N_("reason of the update")),
		OPT_BIT(0 ,"no-deref", &flags,
			N_("update <refname> not the one it points to"),
			REF_NO_DEREF),
		OPT_END(),
	};
	ಸಂರಚನಾ object_id oldoid;
	ಧ್ರುವ ವರ್ಣ *refname;
	ಪೂರ್ಣಾಙ್ಕ ret;

	argc = parse_options(argc, argv, prefix, opts, refs_delete_usage, 0);
	ಯದಿ (argc < 1 || argc > 2)
		usage(_("delete requires reference name and an optional old object ID"));

	ಯದಿ (message && !*message)
		die(_("refusing to perform update with empty message"));

	repo_config(repo, git_default_config, ರಿಕ್ತ);

	refname = argv[0];
	ಯದಿ (argc == 2) {
		ಯದಿ (repo_get_oid_with_flags(repo, argv[1], &oldoid, GET_OID_SKIP_AMBIGUITY_CHECK))
			die(_("invalid old object ID: '%s'"), argv[1]);
		ಯದಿ (is_null_oid(&oldoid))
			die(_("cannot delete reference with null old object ID"));
	}

	ret = refs_delete_ref(get_main_ref_store(repo), message, refname,
			      argc == 2 ? &oldoid : ರಿಕ್ತ, flags);

	ಯದಿ (ret < 0)
		ret = 1;
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmd_refs_update(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
			   ಸಂರಚನಾ repository *repo)
{
	ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ * ಧ್ರುವ refs_update_usage[] = {
		REFS_UPDATE_USAGE,
		ರಿಕ್ತ
	};
	ಧ್ರುವ ವರ್ಣ *message = ರಿಕ್ತ;
	unsigned flags = 0;
	ಸಂರಚನಾ option opts[] = {
		OPT_STRING(0, "message", &message, N_("reason"),
			   N_("reason of the update")),
		OPT_BIT(0 ,"no-deref", &flags,
			N_("update <refname> not the one it points to"),
			REF_NO_DEREF),
		OPT_BIT(0, "create-reflog", &flags, N_("create a reflog"),
			REF_FORCE_CREATE_REFLOG),
		OPT_END(),
	};
	ಸಂರಚನಾ object_id newoid, oldoid;
	ಧ್ರುವ ವರ್ಣ *refname;
	ಪೂರ್ಣಾಙ್ಕ ret;

	argc = parse_options(argc, argv, prefix, opts, refs_update_usage, 0);
	ಯದಿ (argc < 2 || argc > 3)
		usage(_("update requires reference name, new value and an optional old value"));

	ಯದಿ (message && !*message)
		die(_("refusing to perform update with empty message"));

	repo_config(repo, git_default_config, ರಿಕ್ತ);

	refname = argv[0];
	ಯದಿ (repo_get_oid_with_flags(repo, argv[1], &newoid,
				    GET_OID_SKIP_AMBIGUITY_CHECK))
		die(_("invalid new object ID: '%s'"), argv[1]);
	ಯದಿ (argc == 3 &&
	    repo_get_oid_with_flags(repo, argv[2], &oldoid,
				    GET_OID_SKIP_AMBIGUITY_CHECK))
		die(_("invalid old object ID: '%s'"), argv[2]);

	ret = refs_update_ref(get_main_ref_store(repo), message, refname,
			      &newoid, argc == 3 ? &oldoid : ರಿಕ್ತ, flags,
			      UPDATE_REFS_ಸಂದೇಶ_ON_ERR);

	ಯದಿ (ret < 0)
		ret = 1;
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmd_refs_rename(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
			   ಸಂರಚನಾ repository *repo)
{
	ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ * ಧ್ರುವ refs_rename_usage[] = {
		REFS_RENAME_USAGE,
		ರಿಕ್ತ
	};
	ಧ್ರುವ ವರ್ಣ *message = ರಿಕ್ತ;
	ಸಂರಚನಾ option opts[] = {
		OPT_STRING(0, "message", &message, N_("reason"),
			   N_("reason of the update")),
		OPT_END(),
	};
	ಧ್ರುವ ವರ್ಣ *oldref, *newref;
	ಪೂರ್ಣಾಙ್ಕ ret;

	argc = parse_options(argc, argv, prefix, opts, refs_rename_usage, 0);
	ಯದಿ (argc != 2)
		usage(_("rename requires old and new reference name"));
	ಯದಿ (message && !*message)
		die(_("refusing to perform update with empty message"));

	repo_config(repo, git_default_config, ರಿಕ್ತ);

	oldref = argv[0];
	newref = argv[1];

	ಯದಿ (check_refname_format(oldref, 0))
		die(_("invalid ref format: '%s'"), oldref);
	ಯದಿ (check_refname_format(newref, 0))
		die(_("invalid ref format: '%s'"), newref);

	ಯದಿ (!refs_ref_exists(get_main_ref_store(repo), oldref))
		die(_("reference does not exist: '%s'"), oldref);
	ಯದಿ (refs_ref_exists(get_main_ref_store(repo), newref))
		die(_("reference already exists: '%s'"), newref);

	ret = refs_rename_ref(get_main_ref_store(repo), oldref, newref, message);

	ಯದಿ (ret < 0)
		ret = 1;
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಪೂರ್ಣಾಙ್ಕ cmd_refs(ಪೂರ್ಣಾಙ್ಕ argc,
	     ಧ್ರುವ ವರ್ಣ **argv,
	     ಧ್ರುವ ವರ್ಣ *prefix,
	     ಸಂರಚನಾ repository *repo)
{
	ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ refs_usage[] = {
		REFS_MIGRATE_USAGE,
		REFS_VERIFY_USAGE,
		REFS_OPTIMIZE_USAGE,
		"git refs list " COMMON_USAGE_FOR_EACH_REF,
		REFS_EXISTS_USAGE,
		REFS_CREATE_USAGE,
		REFS_DELETE_USAGE,
		REFS_UPDATE_USAGE,
		REFS_RENAME_USAGE,
		ರಿಕ್ತ,
	};
	parse_opt_subcommand_fn *fn = ರಿಕ್ತ;
	ಸಂರಚನಾ option opts[] = {
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
	ಪ್ರತ್ಯಾಗಮ fn(argc, argv, prefix, repo);
}
