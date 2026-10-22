#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "fsck.h"
#ଅନ୍ତର୍ଭାଵ "pack-refs.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "refs.h"
#ଅନ୍ତର୍ଭାଵ "strbuf.h"
#ଅନ୍ତର୍ଭାଵ "worktree.h"
#ଅନ୍ତର୍ଭାଵ "for-each-ref.h"
#ଅନ୍ତର୍ଭାଵ "refs/refs-internal.h"

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

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_refs_migrate(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
			    ସଂରଚନା repository *repo)
{
	ଅଚଳ ଅକ୍ଷର * ଅଚଳ migrate_usage[] = {
		REFS_MIGRATE_USAGE,
		ଖାଲି,
	};
	ଅଚଳ ଅକ୍ଷର *format_str = ଖାଲି;
	ଗଣନା ref_storage_format format;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags = 0;
	ସଂରଚନା option options[] = {
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
	ସଂରଚନା strbuf errbuf = STRBUF_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା err;

	argc = parse_options(argc, argv, prefix, options, migrate_usage, 0);
	ଯଦି (argc)
		usage(_("too many arguments"));
	ଯଦି (!format_str)
		usage(_("missing --ref-storage-format=<format>"));

	format = ref_storage_format_by_name(format_str);
	ଯଦି (format == REF_STORAGE_FORMAT_UNKNOWN) {
		err = error(_("unknown ref storage format '%s'"), format_str);
		ଯାଅ out;
	}

	ଯଦି (repo->ref_storage_format == format) {
		err = error(_("repository already uses '%s' format"),
			    ref_storage_format_to_name(format));
		ଯାଅ out;
	}

	ଯଦି (repo_migrate_ref_storage_format(repo, format, flags, &errbuf) < 0) {
		err = error("%s", errbuf.buf);
		ଯାଅ out;
	}

	err = 0;

out:
	strbuf_release(&errbuf);
	ଫେରନ୍ତୁ err;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_refs_verify(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
			   ସଂରଚନା repository *repo)
{
	ସଂରଚନା fsck_options fsck_refs_options;
	ସଂରଚନା worktree **worktrees;
	ଅଚଳ ଅକ୍ଷର * ଅଚଳ verify_usage[] = {
		REFS_VERIFY_USAGE,
		ଖାଲି,
	};
	ସଂରଚନା option options[] = {
		OPT_BOOL(0, "verbose", &fsck_refs_options.verbose, N_("be verbose")),
		OPT_BOOL(0, "strict", &fsck_refs_options.strict, N_("enable strict checking")),
		OPT_END(),
	};
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;

	fsck_options_init(&fsck_refs_options, repo, FSCK_OPTIONS_REFS);

	argc = parse_options(argc, argv, prefix, options, verify_usage, 0);
	ଯଦି (argc)
		usage(_("'git refs verify' takes no arguments"));

	repo_config(repo, git_fsck_config, &fsck_refs_options);
	prepare_repo_settings(repo);

	worktrees = get_worktrees_without_reading_head(repo);
	ପାଇଁ (size_t i = 0; worktrees[i]; i++)
		ret |= refs_fsck(get_worktree_ref_store(worktrees[i]),
				 &fsck_refs_options, worktrees[i]);

	fsck_options_clear(&fsck_refs_options);
	free_worktrees(worktrees);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_refs_list(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
			   ସଂରଚନା repository *repo)
{
	ସ୍ଥିର ଅକ୍ଷର ଅଚଳ * ଅଚଳ refs_list_usage[] = {
		N_("git refs list " COMMON_USAGE_FOR_EACH_REF),
		ଖାଲି
	};

	ଫେରନ୍ତୁ for_each_ref_core(argc, argv, prefix, repo, refs_list_usage);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_refs_exists(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
			   ସଂରଚନା repository *repo)
{
	ସଂରଚନା strbuf unused_referent = STRBUF_INIT;
	ସଂରଚନା object_id unused_oid;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା unused_type;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା failure_errno = 0;
	ଅଚଳ ଅକ୍ଷର *ref;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;
	ଅଚଳ ଅକ୍ଷର * ଅଚଳ exists_usage[] = {
		REFS_EXISTS_USAGE,
		ଖାଲି,
	};
	ସଂରଚନା option options[] = {
		OPT_END(),
	};

	argc = parse_options(argc, argv, prefix, options, exists_usage, 0);
	ଯଦି (argc != 1)
		die(_("'git refs exists' requires a reference"));

	ref = *argv++;
	ଯଦି (refs_read_raw_ref(get_main_ref_store(repo), ref,
			      &unused_oid, &unused_referent, &unused_type,
			      &failure_errno)) {
		ଯଦି (failure_errno == ENOENT || failure_errno == EISDIR) {
			error(_("reference does not exist"));
			ret = 2;
		} ଅନ୍ୟଥା {
			errno = failure_errno;
			error_errno(_("failed to look up reference"));
			ret = 1;
		}

		ଯାଅ out;
	}

out:
	strbuf_release(&unused_referent);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_refs_optimize(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
			     ସଂରଚନା repository *repo)
{
	ସ୍ଥିର ଅକ୍ଷର ଅଚଳ * ଅଚଳ refs_optimize_usage[] = {
		REFS_OPTIMIZE_USAGE,
		ଖାଲି
	};

	ଫେରନ୍ତୁ pack_refs_core(argc, argv, prefix, repo, refs_optimize_usage);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_refs_create(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
			   ସଂରଚନା repository *repo)
{
	ସ୍ଥିର ଅକ୍ଷର ଅଚଳ * ଅଚଳ refs_create_usage[] = {
		REFS_CREATE_USAGE,
		ଖାଲି
	};
	ଅଚଳ ଅକ୍ଷର *message = ଖାଲି;
	unsigned flags = 0;
	ସଂରଚନା option opts[] = {
		OPT_STRING(0, "message", &message, N_("reason"),
			   N_("reason of the update")),
		OPT_BIT(0 ,"no-deref", &flags,
			N_("update <refname> not the one it points to"),
			REF_NO_DEREF),
		OPT_BIT(0, "create-reflog", &flags, N_("create a reflog"),
			REF_FORCE_CREATE_REFLOG),
		OPT_END(),
	};
	ସଂରଚନା object_id newoid;
	ଅଚଳ ଅକ୍ଷର *refname;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	argc = parse_options(argc, argv, prefix, opts, refs_create_usage, 0);
	ଯଦି (argc != 2)
		usage(_("create requires reference name and an object ID"));

	ଯଦି (message && !*message)
		die(_("refusing to perform update with empty message"));

	repo_config(repo, git_default_config, ଖାଲି);

	refname = argv[0];
	ଯଦି (repo_get_oid_with_flags(repo, argv[1], &newoid, GET_OID_SKIP_AMBIGUITY_CHECK))
		die(_("invalid object ID: '%s'"), argv[1]);
	ଯଦି (is_null_oid(&newoid))
		die(_("cannot create reference with null new object ID"));

	ret = refs_update_ref(get_main_ref_store(repo), message, refname,
			      &newoid, null_oid(repo->hash_algo), flags,
			      UPDATE_REFS_ସନ୍ଦେଶ_ON_ERR);

	ଯଦି (ret < 0)
		ret = 1;
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_refs_delete(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
			   ସଂରଚନା repository *repo)
{
	ସ୍ଥିର ଅକ୍ଷର ଅଚଳ * ଅଚଳ refs_delete_usage[] = {
		REFS_DELETE_USAGE,
		ଖାଲି
	};
	ଅଚଳ ଅକ୍ଷର *message = ଖାଲି;
	unsigned flags = 0;
	ସଂରଚନା option opts[] = {
		OPT_STRING(0, "message", &message, N_("reason"),
			   N_("reason of the update")),
		OPT_BIT(0 ,"no-deref", &flags,
			N_("update <refname> not the one it points to"),
			REF_NO_DEREF),
		OPT_END(),
	};
	ସଂରଚନା object_id oldoid;
	ଅଚଳ ଅକ୍ଷର *refname;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	argc = parse_options(argc, argv, prefix, opts, refs_delete_usage, 0);
	ଯଦି (argc < 1 || argc > 2)
		usage(_("delete requires reference name and an optional old object ID"));

	ଯଦି (message && !*message)
		die(_("refusing to perform update with empty message"));

	repo_config(repo, git_default_config, ଖାଲି);

	refname = argv[0];
	ଯଦି (argc == 2) {
		ଯଦି (repo_get_oid_with_flags(repo, argv[1], &oldoid, GET_OID_SKIP_AMBIGUITY_CHECK))
			die(_("invalid old object ID: '%s'"), argv[1]);
		ଯଦି (is_null_oid(&oldoid))
			die(_("cannot delete reference with null old object ID"));
	}

	ret = refs_delete_ref(get_main_ref_store(repo), message, refname,
			      argc == 2 ? &oldoid : ଖାଲି, flags);

	ଯଦି (ret < 0)
		ret = 1;
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_refs_update(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
			   ସଂରଚନା repository *repo)
{
	ସ୍ଥିର ଅକ୍ଷର ଅଚଳ * ଅଚଳ refs_update_usage[] = {
		REFS_UPDATE_USAGE,
		ଖାଲି
	};
	ଅଚଳ ଅକ୍ଷର *message = ଖାଲି;
	unsigned flags = 0;
	ସଂରଚନା option opts[] = {
		OPT_STRING(0, "message", &message, N_("reason"),
			   N_("reason of the update")),
		OPT_BIT(0 ,"no-deref", &flags,
			N_("update <refname> not the one it points to"),
			REF_NO_DEREF),
		OPT_BIT(0, "create-reflog", &flags, N_("create a reflog"),
			REF_FORCE_CREATE_REFLOG),
		OPT_END(),
	};
	ସଂରଚନା object_id newoid, oldoid;
	ଅଚଳ ଅକ୍ଷର *refname;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	argc = parse_options(argc, argv, prefix, opts, refs_update_usage, 0);
	ଯଦି (argc < 2 || argc > 3)
		usage(_("update requires reference name, new value and an optional old value"));

	ଯଦି (message && !*message)
		die(_("refusing to perform update with empty message"));

	repo_config(repo, git_default_config, ଖାଲି);

	refname = argv[0];
	ଯଦି (repo_get_oid_with_flags(repo, argv[1], &newoid,
				    GET_OID_SKIP_AMBIGUITY_CHECK))
		die(_("invalid new object ID: '%s'"), argv[1]);
	ଯଦି (argc == 3 &&
	    repo_get_oid_with_flags(repo, argv[2], &oldoid,
				    GET_OID_SKIP_AMBIGUITY_CHECK))
		die(_("invalid old object ID: '%s'"), argv[2]);

	ret = refs_update_ref(get_main_ref_store(repo), message, refname,
			      &newoid, argc == 3 ? &oldoid : ଖାଲି, flags,
			      UPDATE_REFS_ସନ୍ଦେଶ_ON_ERR);

	ଯଦି (ret < 0)
		ret = 1;
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_refs_rename(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
			   ସଂରଚନା repository *repo)
{
	ସ୍ଥିର ଅକ୍ଷର ଅଚଳ * ଅଚଳ refs_rename_usage[] = {
		REFS_RENAME_USAGE,
		ଖାଲି
	};
	ଅଚଳ ଅକ୍ଷର *message = ଖାଲି;
	ସଂରଚନା option opts[] = {
		OPT_STRING(0, "message", &message, N_("reason"),
			   N_("reason of the update")),
		OPT_END(),
	};
	ଅଚଳ ଅକ୍ଷର *oldref, *newref;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	argc = parse_options(argc, argv, prefix, opts, refs_rename_usage, 0);
	ଯଦି (argc != 2)
		usage(_("rename requires old and new reference name"));
	ଯଦି (message && !*message)
		die(_("refusing to perform update with empty message"));

	repo_config(repo, git_default_config, ଖାଲି);

	oldref = argv[0];
	newref = argv[1];

	ଯଦି (check_refname_format(oldref, 0))
		die(_("invalid ref format: '%s'"), oldref);
	ଯଦି (check_refname_format(newref, 0))
		die(_("invalid ref format: '%s'"), newref);

	ଯଦି (!refs_ref_exists(get_main_ref_store(repo), oldref))
		die(_("reference does not exist: '%s'"), oldref);
	ଯଦି (refs_ref_exists(get_main_ref_store(repo), newref))
		die(_("reference already exists: '%s'"), newref);

	ret = refs_rename_ref(get_main_ref_store(repo), oldref, newref, message);

	ଯଦି (ret < 0)
		ret = 1;
	ଫେରନ୍ତୁ ret;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_refs(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
	     ଅଚଳ ଅକ୍ଷର **argv,
	     ଅଚଳ ଅକ୍ଷର *prefix,
	     ସଂରଚନା repository *repo)
{
	ଅଚଳ ଅକ୍ଷର * ଅଚଳ refs_usage[] = {
		REFS_MIGRATE_USAGE,
		REFS_VERIFY_USAGE,
		REFS_OPTIMIZE_USAGE,
		"git refs list " COMMON_USAGE_FOR_EACH_REF,
		REFS_EXISTS_USAGE,
		REFS_CREATE_USAGE,
		REFS_DELETE_USAGE,
		REFS_UPDATE_USAGE,
		REFS_RENAME_USAGE,
		ଖାଲି,
	};
	parse_opt_subcommand_fn *fn = ଖାଲି;
	ସଂରଚନା option opts[] = {
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
	ଫେରନ୍ତୁ fn(argc, argv, prefix, repo);
}
