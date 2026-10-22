#अन्तर्भाव "builtin.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "fsck.h"
#अन्तर्भाव "pack-refs.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "refs.h"
#अन्तर्भाव "strbuf.h"
#अन्तर्भाव "worktree.h"
#अन्तर्भाव "for-each-ref.h"
#अन्तर्भाव "refs/refs-internal.h"

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

स्थिर पूर्णाङ्क cmd_refs_migrate(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix,
			    संरचना repository *repo)
{
	ध्रुव वर्ण * ध्रुव migrate_usage[] = {
		REFS_MIGRATE_USAGE,
		रिक्त,
	};
	ध्रुव वर्ण *format_str = रिक्त;
	गणना ref_storage_format format;
	unsigned पूर्णाङ्क flags = 0;
	संरचना option options[] = {
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
	संरचना strbuf errbuf = STRBUF_INIT;
	पूर्णाङ्क err;

	argc = parse_options(argc, argv, prefix, options, migrate_usage, 0);
	सचे (argc)
		usage(_("too many arguments"));
	सचे (!format_str)
		usage(_("missing --ref-storage-format=<format>"));

	format = ref_storage_format_by_name(format_str);
	सचे (format == REF_STORAGE_FORMAT_UNKNOWN) {
		err = error(_("unknown ref storage format '%s'"), format_str);
		गच्छ out;
	}

	सचे (repo->ref_storage_format == format) {
		err = error(_("repository already uses '%s' format"),
			    ref_storage_format_to_name(format));
		गच्छ out;
	}

	सचे (repo_migrate_ref_storage_format(repo, format, flags, &errbuf) < 0) {
		err = error("%s", errbuf.buf);
		गच्छ out;
	}

	err = 0;

out:
	strbuf_release(&errbuf);
	पटिनिवत्त err;
}

स्थिर पूर्णाङ्क cmd_refs_verify(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix,
			   संरचना repository *repo)
{
	संरचना fsck_options fsck_refs_options;
	संरचना worktree **worktrees;
	ध्रुव वर्ण * ध्रुव verify_usage[] = {
		REFS_VERIFY_USAGE,
		रिक्त,
	};
	संरचना option options[] = {
		OPT_BOOL(0, "verbose", &fsck_refs_options.verbose, N_("be verbose")),
		OPT_BOOL(0, "strict", &fsck_refs_options.strict, N_("enable strict checking")),
		OPT_END(),
	};
	पूर्णाङ्क ret = 0;

	fsck_options_init(&fsck_refs_options, repo, FSCK_OPTIONS_REFS);

	argc = parse_options(argc, argv, prefix, options, verify_usage, 0);
	सचे (argc)
		usage(_("'git refs verify' takes no arguments"));

	repo_config(repo, git_fsck_config, &fsck_refs_options);
	prepare_repo_settings(repo);

	worktrees = get_worktrees_without_reading_head(repo);
	अत्थाय (size_t i = 0; worktrees[i]; i++)
		ret |= refs_fsck(get_worktree_ref_store(worktrees[i]),
				 &fsck_refs_options, worktrees[i]);

	fsck_options_clear(&fsck_refs_options);
	free_worktrees(worktrees);
	पटिनिवत्त ret;
}

स्थिर पूर्णाङ्क cmd_refs_list(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix,
			   संरचना repository *repo)
{
	स्थिर वर्ण ध्रुव * ध्रुव refs_list_usage[] = {
		N_("git refs list " COMMON_USAGE_FOR_EACH_REF),
		रिक्त
	};

	पटिनिवत्त for_each_ref_core(argc, argv, prefix, repo, refs_list_usage);
}

स्थिर पूर्णाङ्क cmd_refs_exists(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix,
			   संरचना repository *repo)
{
	संरचना strbuf unused_referent = STRBUF_INIT;
	संरचना object_id unused_oid;
	unsigned पूर्णाङ्क unused_type;
	पूर्णाङ्क failure_errno = 0;
	ध्रुव वर्ण *ref;
	पूर्णाङ्क ret = 0;
	ध्रुव वर्ण * ध्रुव exists_usage[] = {
		REFS_EXISTS_USAGE,
		रिक्त,
	};
	संरचना option options[] = {
		OPT_END(),
	};

	argc = parse_options(argc, argv, prefix, options, exists_usage, 0);
	सचे (argc != 1)
		die(_("'git refs exists' requires a reference"));

	ref = *argv++;
	सचे (refs_read_raw_ref(get_main_ref_store(repo), ref,
			      &unused_oid, &unused_referent, &unused_type,
			      &failure_errno)) {
		सचे (failure_errno == ENOENT || failure_errno == EISDIR) {
			error(_("reference does not exist"));
			ret = 2;
		} अञ्ञथा {
			errno = failure_errno;
			error_errno(_("failed to look up reference"));
			ret = 1;
		}

		गच्छ out;
	}

out:
	strbuf_release(&unused_referent);
	पटिनिवत्त ret;
}

स्थिर पूर्णाङ्क cmd_refs_optimize(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix,
			     संरचना repository *repo)
{
	स्थिर वर्ण ध्रुव * ध्रुव refs_optimize_usage[] = {
		REFS_OPTIMIZE_USAGE,
		रिक्त
	};

	पटिनिवत्त pack_refs_core(argc, argv, prefix, repo, refs_optimize_usage);
}

स्थिर पूर्णाङ्क cmd_refs_create(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix,
			   संरचना repository *repo)
{
	स्थिर वर्ण ध्रुव * ध्रुव refs_create_usage[] = {
		REFS_CREATE_USAGE,
		रिक्त
	};
	ध्रुव वर्ण *message = रिक्त;
	unsigned flags = 0;
	संरचना option opts[] = {
		OPT_STRING(0, "message", &message, N_("reason"),
			   N_("reason of the update")),
		OPT_BIT(0 ,"no-deref", &flags,
			N_("update <refname> not the one it points to"),
			REF_NO_DEREF),
		OPT_BIT(0, "create-reflog", &flags, N_("create a reflog"),
			REF_FORCE_CREATE_REFLOG),
		OPT_END(),
	};
	संरचना object_id newoid;
	ध्रुव वर्ण *refname;
	पूर्णाङ्क ret;

	argc = parse_options(argc, argv, prefix, opts, refs_create_usage, 0);
	सचे (argc != 2)
		usage(_("create requires reference name and an object ID"));

	सचे (message && !*message)
		die(_("refusing to perform update with empty message"));

	repo_config(repo, git_default_config, रिक्त);

	refname = argv[0];
	सचे (repo_get_oid_with_flags(repo, argv[1], &newoid, GET_OID_SKIP_AMBIGUITY_CHECK))
		die(_("invalid object ID: '%s'"), argv[1]);
	सचे (is_null_oid(&newoid))
		die(_("cannot create reference with null new object ID"));

	ret = refs_update_ref(get_main_ref_store(repo), message, refname,
			      &newoid, null_oid(repo->hash_algo), flags,
			      UPDATE_REFS_सन्देशः_ON_ERR);

	सचे (ret < 0)
		ret = 1;
	पटिनिवत्त ret;
}

स्थिर पूर्णाङ्क cmd_refs_delete(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix,
			   संरचना repository *repo)
{
	स्थिर वर्ण ध्रुव * ध्रुव refs_delete_usage[] = {
		REFS_DELETE_USAGE,
		रिक्त
	};
	ध्रुव वर्ण *message = रिक्त;
	unsigned flags = 0;
	संरचना option opts[] = {
		OPT_STRING(0, "message", &message, N_("reason"),
			   N_("reason of the update")),
		OPT_BIT(0 ,"no-deref", &flags,
			N_("update <refname> not the one it points to"),
			REF_NO_DEREF),
		OPT_END(),
	};
	संरचना object_id oldoid;
	ध्रुव वर्ण *refname;
	पूर्णाङ्क ret;

	argc = parse_options(argc, argv, prefix, opts, refs_delete_usage, 0);
	सचे (argc < 1 || argc > 2)
		usage(_("delete requires reference name and an optional old object ID"));

	सचे (message && !*message)
		die(_("refusing to perform update with empty message"));

	repo_config(repo, git_default_config, रिक्त);

	refname = argv[0];
	सचे (argc == 2) {
		सचे (repo_get_oid_with_flags(repo, argv[1], &oldoid, GET_OID_SKIP_AMBIGUITY_CHECK))
			die(_("invalid old object ID: '%s'"), argv[1]);
		सचे (is_null_oid(&oldoid))
			die(_("cannot delete reference with null old object ID"));
	}

	ret = refs_delete_ref(get_main_ref_store(repo), message, refname,
			      argc == 2 ? &oldoid : रिक्त, flags);

	सचे (ret < 0)
		ret = 1;
	पटिनिवत्त ret;
}

स्थिर पूर्णाङ्क cmd_refs_update(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix,
			   संरचना repository *repo)
{
	स्थिर वर्ण ध्रुव * ध्रुव refs_update_usage[] = {
		REFS_UPDATE_USAGE,
		रिक्त
	};
	ध्रुव वर्ण *message = रिक्त;
	unsigned flags = 0;
	संरचना option opts[] = {
		OPT_STRING(0, "message", &message, N_("reason"),
			   N_("reason of the update")),
		OPT_BIT(0 ,"no-deref", &flags,
			N_("update <refname> not the one it points to"),
			REF_NO_DEREF),
		OPT_BIT(0, "create-reflog", &flags, N_("create a reflog"),
			REF_FORCE_CREATE_REFLOG),
		OPT_END(),
	};
	संरचना object_id newoid, oldoid;
	ध्रुव वर्ण *refname;
	पूर्णाङ्क ret;

	argc = parse_options(argc, argv, prefix, opts, refs_update_usage, 0);
	सचे (argc < 2 || argc > 3)
		usage(_("update requires reference name, new value and an optional old value"));

	सचे (message && !*message)
		die(_("refusing to perform update with empty message"));

	repo_config(repo, git_default_config, रिक्त);

	refname = argv[0];
	सचे (repo_get_oid_with_flags(repo, argv[1], &newoid,
				    GET_OID_SKIP_AMBIGUITY_CHECK))
		die(_("invalid new object ID: '%s'"), argv[1]);
	सचे (argc == 3 &&
	    repo_get_oid_with_flags(repo, argv[2], &oldoid,
				    GET_OID_SKIP_AMBIGUITY_CHECK))
		die(_("invalid old object ID: '%s'"), argv[2]);

	ret = refs_update_ref(get_main_ref_store(repo), message, refname,
			      &newoid, argc == 3 ? &oldoid : रिक्त, flags,
			      UPDATE_REFS_सन्देशः_ON_ERR);

	सचे (ret < 0)
		ret = 1;
	पटिनिवत्त ret;
}

स्थिर पूर्णाङ्क cmd_refs_rename(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix,
			   संरचना repository *repo)
{
	स्थिर वर्ण ध्रुव * ध्रुव refs_rename_usage[] = {
		REFS_RENAME_USAGE,
		रिक्त
	};
	ध्रुव वर्ण *message = रिक्त;
	संरचना option opts[] = {
		OPT_STRING(0, "message", &message, N_("reason"),
			   N_("reason of the update")),
		OPT_END(),
	};
	ध्रुव वर्ण *oldref, *newref;
	पूर्णाङ्क ret;

	argc = parse_options(argc, argv, prefix, opts, refs_rename_usage, 0);
	सचे (argc != 2)
		usage(_("rename requires old and new reference name"));
	सचे (message && !*message)
		die(_("refusing to perform update with empty message"));

	repo_config(repo, git_default_config, रिक्त);

	oldref = argv[0];
	newref = argv[1];

	सचे (check_refname_format(oldref, 0))
		die(_("invalid ref format: '%s'"), oldref);
	सचे (check_refname_format(newref, 0))
		die(_("invalid ref format: '%s'"), newref);

	सचे (!refs_ref_exists(get_main_ref_store(repo), oldref))
		die(_("reference does not exist: '%s'"), oldref);
	सचे (refs_ref_exists(get_main_ref_store(repo), newref))
		die(_("reference already exists: '%s'"), newref);

	ret = refs_rename_ref(get_main_ref_store(repo), oldref, newref, message);

	सचे (ret < 0)
		ret = 1;
	पटिनिवत्त ret;
}

पूर्णाङ्क cmd_refs(पूर्णाङ्क argc,
	     ध्रुव वर्ण **argv,
	     ध्रुव वर्ण *prefix,
	     संरचना repository *repo)
{
	ध्रुव वर्ण * ध्रुव refs_usage[] = {
		REFS_MIGRATE_USAGE,
		REFS_VERIFY_USAGE,
		REFS_OPTIMIZE_USAGE,
		"git refs list " COMMON_USAGE_FOR_EACH_REF,
		REFS_EXISTS_USAGE,
		REFS_CREATE_USAGE,
		REFS_DELETE_USAGE,
		REFS_UPDATE_USAGE,
		REFS_RENAME_USAGE,
		रिक्त,
	};
	parse_opt_subcommand_fn *fn = रिक्त;
	संरचना option opts[] = {
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
	पटिनिवत्त fn(argc, argv, prefix, repo);
}
