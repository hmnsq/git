#অন্তর্ভাৱ "builtin.h"
#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ "fsck.h"
#অন্তর্ভাৱ "pack-refs.h"
#অন্তর্ভাৱ "parse-options.h"
#অন্তর্ভাৱ "refs.h"
#অন্তর্ভাৱ "strbuf.h"
#অন্তর্ভাৱ "worktree.h"
#অন্তর্ভাৱ "for-each-ref.h"
#অন্তর্ভাৱ "refs/refs-internal.h"

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

স্থির পূর্ণাঙ্ক cmd_refs_migrate(পূর্ণাঙ্ক argc, ধ্রুৱ ৱর্ণ **argv, ধ্রুৱ ৱর্ণ *prefix,
			    সংরচনা repository *repo)
{
	ধ্রুৱ ৱর্ণ * ধ্রুৱ migrate_usage[] = {
		REFS_MIGRATE_USAGE,
		রিক্ত,
	};
	ধ্রুৱ ৱর্ণ *format_str = রিক্ত;
	গণনা ref_storage_format format;
	unsigned পূর্ণাঙ্ক flags = 0;
	সংরচনা option options[] = {
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
	সংরচনা strbuf errbuf = STRBUF_INIT;
	পূর্ণাঙ্ক err;

	argc = parse_options(argc, argv, prefix, options, migrate_usage, 0);
	যদি (argc)
		usage(_("too many arguments"));
	যদি (!format_str)
		usage(_("missing --ref-storage-format=<format>"));

	format = ref_storage_format_by_name(format_str);
	যদি (format == REF_STORAGE_FORMAT_UNKNOWN) {
		err = error(_("unknown ref storage format '%s'"), format_str);
		গচ্ছ out;
	}

	যদি (repo->ref_storage_format == format) {
		err = error(_("repository already uses '%s' format"),
			    ref_storage_format_to_name(format));
		গচ্ছ out;
	}

	যদি (repo_migrate_ref_storage_format(repo, format, flags, &errbuf) < 0) {
		err = error("%s", errbuf.buf);
		গচ্ছ out;
	}

	err = 0;

out:
	strbuf_release(&errbuf);
	ঘূৰাই err;
}

স্থির পূর্ণাঙ্ক cmd_refs_verify(পূর্ণাঙ্ক argc, ধ্রুৱ ৱর্ণ **argv, ধ্রুৱ ৱর্ণ *prefix,
			   সংরচনা repository *repo)
{
	সংরচনা fsck_options fsck_refs_options;
	সংরচনা worktree **worktrees;
	ধ্রুৱ ৱর্ণ * ধ্রুৱ verify_usage[] = {
		REFS_VERIFY_USAGE,
		রিক্ত,
	};
	সংরচনা option options[] = {
		OPT_BOOL(0, "verbose", &fsck_refs_options.verbose, N_("be verbose")),
		OPT_BOOL(0, "strict", &fsck_refs_options.strict, N_("enable strict checking")),
		OPT_END(),
	};
	পূর্ণাঙ্ক ret = 0;

	fsck_options_init(&fsck_refs_options, repo, FSCK_OPTIONS_REFS);

	argc = parse_options(argc, argv, prefix, options, verify_usage, 0);
	যদি (argc)
		usage(_("'git refs verify' takes no arguments"));

	repo_config(repo, git_fsck_config, &fsck_refs_options);
	prepare_repo_settings(repo);

	worktrees = get_worktrees_without_reading_head(repo);
	বাবে (size_t i = 0; worktrees[i]; i++)
		ret |= refs_fsck(get_worktree_ref_store(worktrees[i]),
				 &fsck_refs_options, worktrees[i]);

	fsck_options_clear(&fsck_refs_options);
	free_worktrees(worktrees);
	ঘূৰাই ret;
}

স্থির পূর্ণাঙ্ক cmd_refs_list(পূর্ণাঙ্ক argc, ধ্রুৱ ৱর্ণ **argv, ধ্রুৱ ৱর্ণ *prefix,
			   সংরচনা repository *repo)
{
	স্থির ৱর্ণ ধ্রুৱ * ধ্রুৱ refs_list_usage[] = {
		N_("git refs list " COMMON_USAGE_FOR_EACH_REF),
		রিক্ত
	};

	ঘূৰাই for_each_ref_core(argc, argv, prefix, repo, refs_list_usage);
}

স্থির পূর্ণাঙ্ক cmd_refs_exists(পূর্ণাঙ্ক argc, ধ্রুৱ ৱর্ণ **argv, ধ্রুৱ ৱর্ণ *prefix,
			   সংরচনা repository *repo)
{
	সংরচনা strbuf unused_referent = STRBUF_INIT;
	সংরচনা object_id unused_oid;
	unsigned পূর্ণাঙ্ক unused_type;
	পূর্ণাঙ্ক failure_errno = 0;
	ধ্রুৱ ৱর্ণ *ref;
	পূর্ণাঙ্ক ret = 0;
	ধ্রুৱ ৱর্ণ * ধ্রুৱ exists_usage[] = {
		REFS_EXISTS_USAGE,
		রিক্ত,
	};
	সংরচনা option options[] = {
		OPT_END(),
	};

	argc = parse_options(argc, argv, prefix, options, exists_usage, 0);
	যদি (argc != 1)
		die(_("'git refs exists' requires a reference"));

	ref = *argv++;
	যদি (refs_read_raw_ref(get_main_ref_store(repo), ref,
			      &unused_oid, &unused_referent, &unused_type,
			      &failure_errno)) {
		যদি (failure_errno == ENOENT || failure_errno == EISDIR) {
			error(_("reference does not exist"));
			ret = 2;
		} অন্যথা {
			errno = failure_errno;
			error_errno(_("failed to look up reference"));
			ret = 1;
		}

		গচ্ছ out;
	}

out:
	strbuf_release(&unused_referent);
	ঘূৰাই ret;
}

স্থির পূর্ণাঙ্ক cmd_refs_optimize(পূর্ণাঙ্ক argc, ধ্রুৱ ৱর্ণ **argv, ধ্রুৱ ৱর্ণ *prefix,
			     সংরচনা repository *repo)
{
	স্থির ৱর্ণ ধ্রুৱ * ধ্রুৱ refs_optimize_usage[] = {
		REFS_OPTIMIZE_USAGE,
		রিক্ত
	};

	ঘূৰাই pack_refs_core(argc, argv, prefix, repo, refs_optimize_usage);
}

স্থির পূর্ণাঙ্ক cmd_refs_create(পূর্ণাঙ্ক argc, ধ্রুৱ ৱর্ণ **argv, ধ্রুৱ ৱর্ণ *prefix,
			   সংরচনা repository *repo)
{
	স্থির ৱর্ণ ধ্রুৱ * ধ্রুৱ refs_create_usage[] = {
		REFS_CREATE_USAGE,
		রিক্ত
	};
	ধ্রুৱ ৱর্ণ *message = রিক্ত;
	unsigned flags = 0;
	সংরচনা option opts[] = {
		OPT_STRING(0, "message", &message, N_("reason"),
			   N_("reason of the update")),
		OPT_BIT(0 ,"no-deref", &flags,
			N_("update <refname> not the one it points to"),
			REF_NO_DEREF),
		OPT_BIT(0, "create-reflog", &flags, N_("create a reflog"),
			REF_FORCE_CREATE_REFLOG),
		OPT_END(),
	};
	সংরচনা object_id newoid;
	ধ্রুৱ ৱর্ণ *refname;
	পূর্ণাঙ্ক ret;

	argc = parse_options(argc, argv, prefix, opts, refs_create_usage, 0);
	যদি (argc != 2)
		usage(_("create requires reference name and an object ID"));

	যদি (message && !*message)
		die(_("refusing to perform update with empty message"));

	repo_config(repo, git_default_config, রিক্ত);

	refname = argv[0];
	যদি (repo_get_oid_with_flags(repo, argv[1], &newoid, GET_OID_SKIP_AMBIGUITY_CHECK))
		die(_("invalid object ID: '%s'"), argv[1]);
	যদি (is_null_oid(&newoid))
		die(_("cannot create reference with null new object ID"));

	ret = refs_update_ref(get_main_ref_store(repo), message, refname,
			      &newoid, null_oid(repo->hash_algo), flags,
			      UPDATE_REFS_বাৰ্তা_ON_ERR);

	যদি (ret < 0)
		ret = 1;
	ঘূৰাই ret;
}

স্থির পূর্ণাঙ্ক cmd_refs_delete(পূর্ণাঙ্ক argc, ধ্রুৱ ৱর্ণ **argv, ধ্রুৱ ৱর্ণ *prefix,
			   সংরচনা repository *repo)
{
	স্থির ৱর্ণ ধ্রুৱ * ধ্রুৱ refs_delete_usage[] = {
		REFS_DELETE_USAGE,
		রিক্ত
	};
	ধ্রুৱ ৱর্ণ *message = রিক্ত;
	unsigned flags = 0;
	সংরচনা option opts[] = {
		OPT_STRING(0, "message", &message, N_("reason"),
			   N_("reason of the update")),
		OPT_BIT(0 ,"no-deref", &flags,
			N_("update <refname> not the one it points to"),
			REF_NO_DEREF),
		OPT_END(),
	};
	সংরচনা object_id oldoid;
	ধ্রুৱ ৱর্ণ *refname;
	পূর্ণাঙ্ক ret;

	argc = parse_options(argc, argv, prefix, opts, refs_delete_usage, 0);
	যদি (argc < 1 || argc > 2)
		usage(_("delete requires reference name and an optional old object ID"));

	যদি (message && !*message)
		die(_("refusing to perform update with empty message"));

	repo_config(repo, git_default_config, রিক্ত);

	refname = argv[0];
	যদি (argc == 2) {
		যদি (repo_get_oid_with_flags(repo, argv[1], &oldoid, GET_OID_SKIP_AMBIGUITY_CHECK))
			die(_("invalid old object ID: '%s'"), argv[1]);
		যদি (is_null_oid(&oldoid))
			die(_("cannot delete reference with null old object ID"));
	}

	ret = refs_delete_ref(get_main_ref_store(repo), message, refname,
			      argc == 2 ? &oldoid : রিক্ত, flags);

	যদি (ret < 0)
		ret = 1;
	ঘূৰাই ret;
}

স্থির পূর্ণাঙ্ক cmd_refs_update(পূর্ণাঙ্ক argc, ধ্রুৱ ৱর্ণ **argv, ধ্রুৱ ৱর্ণ *prefix,
			   সংরচনা repository *repo)
{
	স্থির ৱর্ণ ধ্রুৱ * ধ্রুৱ refs_update_usage[] = {
		REFS_UPDATE_USAGE,
		রিক্ত
	};
	ধ্রুৱ ৱর্ণ *message = রিক্ত;
	unsigned flags = 0;
	সংরচনা option opts[] = {
		OPT_STRING(0, "message", &message, N_("reason"),
			   N_("reason of the update")),
		OPT_BIT(0 ,"no-deref", &flags,
			N_("update <refname> not the one it points to"),
			REF_NO_DEREF),
		OPT_BIT(0, "create-reflog", &flags, N_("create a reflog"),
			REF_FORCE_CREATE_REFLOG),
		OPT_END(),
	};
	সংরচনা object_id newoid, oldoid;
	ধ্রুৱ ৱর্ণ *refname;
	পূর্ণাঙ্ক ret;

	argc = parse_options(argc, argv, prefix, opts, refs_update_usage, 0);
	যদি (argc < 2 || argc > 3)
		usage(_("update requires reference name, new value and an optional old value"));

	যদি (message && !*message)
		die(_("refusing to perform update with empty message"));

	repo_config(repo, git_default_config, রিক্ত);

	refname = argv[0];
	যদি (repo_get_oid_with_flags(repo, argv[1], &newoid,
				    GET_OID_SKIP_AMBIGUITY_CHECK))
		die(_("invalid new object ID: '%s'"), argv[1]);
	যদি (argc == 3 &&
	    repo_get_oid_with_flags(repo, argv[2], &oldoid,
				    GET_OID_SKIP_AMBIGUITY_CHECK))
		die(_("invalid old object ID: '%s'"), argv[2]);

	ret = refs_update_ref(get_main_ref_store(repo), message, refname,
			      &newoid, argc == 3 ? &oldoid : রিক্ত, flags,
			      UPDATE_REFS_বাৰ্তা_ON_ERR);

	যদি (ret < 0)
		ret = 1;
	ঘূৰাই ret;
}

স্থির পূর্ণাঙ্ক cmd_refs_rename(পূর্ণাঙ্ক argc, ধ্রুৱ ৱর্ণ **argv, ধ্রুৱ ৱর্ণ *prefix,
			   সংরচনা repository *repo)
{
	স্থির ৱর্ণ ধ্রুৱ * ধ্রুৱ refs_rename_usage[] = {
		REFS_RENAME_USAGE,
		রিক্ত
	};
	ধ্রুৱ ৱর্ণ *message = রিক্ত;
	সংরচনা option opts[] = {
		OPT_STRING(0, "message", &message, N_("reason"),
			   N_("reason of the update")),
		OPT_END(),
	};
	ধ্রুৱ ৱর্ণ *oldref, *newref;
	পূর্ণাঙ্ক ret;

	argc = parse_options(argc, argv, prefix, opts, refs_rename_usage, 0);
	যদি (argc != 2)
		usage(_("rename requires old and new reference name"));
	যদি (message && !*message)
		die(_("refusing to perform update with empty message"));

	repo_config(repo, git_default_config, রিক্ত);

	oldref = argv[0];
	newref = argv[1];

	যদি (check_refname_format(oldref, 0))
		die(_("invalid ref format: '%s'"), oldref);
	যদি (check_refname_format(newref, 0))
		die(_("invalid ref format: '%s'"), newref);

	যদি (!refs_ref_exists(get_main_ref_store(repo), oldref))
		die(_("reference does not exist: '%s'"), oldref);
	যদি (refs_ref_exists(get_main_ref_store(repo), newref))
		die(_("reference already exists: '%s'"), newref);

	ret = refs_rename_ref(get_main_ref_store(repo), oldref, newref, message);

	যদি (ret < 0)
		ret = 1;
	ঘূৰাই ret;
}

পূর্ণাঙ্ক cmd_refs(পূর্ণাঙ্ক argc,
	     ধ্রুৱ ৱর্ণ **argv,
	     ধ্রুৱ ৱর্ণ *prefix,
	     সংরচনা repository *repo)
{
	ধ্রুৱ ৱর্ণ * ধ্রুৱ refs_usage[] = {
		REFS_MIGRATE_USAGE,
		REFS_VERIFY_USAGE,
		REFS_OPTIMIZE_USAGE,
		"git refs list " COMMON_USAGE_FOR_EACH_REF,
		REFS_EXISTS_USAGE,
		REFS_CREATE_USAGE,
		REFS_DELETE_USAGE,
		REFS_UPDATE_USAGE,
		REFS_RENAME_USAGE,
		রিক্ত,
	};
	parse_opt_subcommand_fn *fn = রিক্ত;
	সংরচনা option opts[] = {
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
	ঘূৰাই fn(argc, argv, prefix, repo);
}
