#समावेश "builtin.h"
#समावेश "config.h"
#समावेश "fsck.h"
#समावेश "pack-refs.h"
#समावेश "parse-options.h"
#समावेश "refs.h"
#समावेश "strbuf.h"
#समावेश "worktree.h"
#समावेश "for-each-ref.h"
#समावेश "refs/refs-internal.h"

#घोषणा REFS_MIGRATE_USAGE \
	N_("git refs migrate --ref-storage-format=<format> [--no-reflog] [--dry-run]")

#घोषणा REFS_VERIFY_USAGE \
	N_("git refs verify [--strict] [--verbose]")

#घोषणा REFS_EXISTS_USAGE \
	N_("git refs exists <ref>")

#घोषणा REFS_OPTIMIZE_USAGE \
	N_("git refs optimize " PACK_REFS_OPTS)

#घोषणा REFS_CREATE_USAGE \
	N_("git refs create [--message=<reason>] [--no-deref] [--create-reflog] <ref> <new-value>")

#घोषणा REFS_DELETE_USAGE \
	N_("git refs delete [--message=<reason>] [--no-deref] <ref> [<old-value>]")

#घोषणा REFS_UPDATE_USAGE \
	N_("git refs update [--message=<reason>] [--no-deref] [--create-reflog] <ref> <new-value> [<old-value>]")

#घोषणा REFS_RENAME_USAGE \
	N_("git refs rename [--message=<reason>] <old-ref> <new-ref>")

अटल पूर्णांक cmd_refs_migrate(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
			    काष्ठा repository *repo)
{
	स्थिर अक्षर * स्थिर migrate_usage[] = {
		REFS_MIGRATE_USAGE,
		शून्य,
	};
	स्थिर अक्षर *format_str = शून्य;
	क्रमागत ref_storage_format format;
	अचिन्हित पूर्णांक flags = 0;
	काष्ठा option options[] = {
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
	काष्ठा strbuf errbuf = STRBUF_INIT;
	पूर्णांक err;

	argc = parse_options(argc, argv, prefix, options, migrate_usage, 0);
	अगर (argc)
		usage(_("too many arguments"));
	अगर (!format_str)
		usage(_("missing --ref-storage-format=<format>"));

	format = ref_storage_format_by_name(format_str);
	अगर (format == REF_STORAGE_FORMAT_UNKNOWN) {
		err = error(_("unknown ref storage format '%s'"), format_str);
		जाओ out;
	}

	अगर (repo->ref_storage_format == format) {
		err = error(_("repository already uses '%s' format"),
			    ref_storage_format_to_name(format));
		जाओ out;
	}

	अगर (repo_migrate_ref_storage_format(repo, format, flags, &errbuf) < 0) {
		err = error("%s", errbuf.buf);
		जाओ out;
	}

	err = 0;

out:
	strbuf_release(&errbuf);
	वापस err;
}

अटल पूर्णांक cmd_refs_verify(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
			   काष्ठा repository *repo)
{
	काष्ठा fsck_options fsck_refs_options;
	काष्ठा worktree **worktrees;
	स्थिर अक्षर * स्थिर verify_usage[] = {
		REFS_VERIFY_USAGE,
		शून्य,
	};
	काष्ठा option options[] = {
		OPT_BOOL(0, "verbose", &fsck_refs_options.verbose, N_("be verbose")),
		OPT_BOOL(0, "strict", &fsck_refs_options.strict, N_("enable strict checking")),
		OPT_END(),
	};
	पूर्णांक ret = 0;

	fsck_options_init(&fsck_refs_options, repo, FSCK_OPTIONS_REFS);

	argc = parse_options(argc, argv, prefix, options, verify_usage, 0);
	अगर (argc)
		usage(_("'git refs verify' takes no arguments"));

	repo_config(repo, git_fsck_config, &fsck_refs_options);
	prepare_repo_settings(repo);

	worktrees = get_worktrees_without_reading_शीर्ष(repo);
	क्रम (माप_प्रकार i = 0; worktrees[i]; i++)
		ret |= refs_fsck(get_worktree_ref_store(worktrees[i]),
				 &fsck_refs_options, worktrees[i]);

	fsck_options_clear(&fsck_refs_options);
	free_worktrees(worktrees);
	वापस ret;
}

अटल पूर्णांक cmd_refs_list(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
			   काष्ठा repository *repo)
{
	अटल अक्षर स्थिर * स्थिर refs_list_usage[] = {
		N_("git refs list " COMMON_USAGE_FOR_EACH_REF),
		शून्य
	};

	वापस for_each_ref_core(argc, argv, prefix, repo, refs_list_usage);
}

अटल पूर्णांक cmd_refs_exists(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
			   काष्ठा repository *repo)
{
	काष्ठा strbuf unused_referent = STRBUF_INIT;
	काष्ठा object_id unused_oid;
	अचिन्हित पूर्णांक unused_type;
	पूर्णांक failure_errno = 0;
	स्थिर अक्षर *ref;
	पूर्णांक ret = 0;
	स्थिर अक्षर * स्थिर exists_usage[] = {
		REFS_EXISTS_USAGE,
		शून्य,
	};
	काष्ठा option options[] = {
		OPT_END(),
	};

	argc = parse_options(argc, argv, prefix, options, exists_usage, 0);
	अगर (argc != 1)
		die(_("'git refs exists' requires a reference"));

	ref = *argv++;
	अगर (refs_read_raw_ref(get_main_ref_store(repo), ref,
			      &unused_oid, &unused_referent, &unused_type,
			      &failure_errno)) {
		अगर (failure_errno == ENOENT || failure_errno == EISDIR) {
			error(_("reference does not exist"));
			ret = 2;
		} अन्यथा {
			त्रुटि_सं = failure_errno;
			error_errno(_("failed to look up reference"));
			ret = 1;
		}

		जाओ out;
	}

out:
	strbuf_release(&unused_referent);
	वापस ret;
}

अटल पूर्णांक cmd_refs_optimize(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
			     काष्ठा repository *repo)
{
	अटल अक्षर स्थिर * स्थिर refs_optimize_usage[] = {
		REFS_OPTIMIZE_USAGE,
		शून्य
	};

	वापस pack_refs_core(argc, argv, prefix, repo, refs_optimize_usage);
}

अटल पूर्णांक cmd_refs_create(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
			   काष्ठा repository *repo)
{
	अटल अक्षर स्थिर * स्थिर refs_create_usage[] = {
		REFS_CREATE_USAGE,
		शून्य
	};
	स्थिर अक्षर *message = शून्य;
	अचिन्हित flags = 0;
	काष्ठा option opts[] = {
		OPT_STRING(0, "message", &message, N_("reason"),
			   N_("reason of the update")),
		OPT_BIT(0 ,"no-deref", &flags,
			N_("update <refname> not the one it points to"),
			REF_NO_DEREF),
		OPT_BIT(0, "create-reflog", &flags, N_("create a reflog"),
			REF_FORCE_CREATE_REFLOG),
		OPT_END(),
	};
	काष्ठा object_id newoid;
	स्थिर अक्षर *refname;
	पूर्णांक ret;

	argc = parse_options(argc, argv, prefix, opts, refs_create_usage, 0);
	अगर (argc != 2)
		usage(_("create requires reference name and an object ID"));

	अगर (message && !*message)
		die(_("refusing to perform update with empty message"));

	repo_config(repo, git_default_config, शून्य);

	refname = argv[0];
	अगर (repo_get_oid_with_flags(repo, argv[1], &newoid, GET_OID_SKIP_AMBIGUITY_CHECK))
		die(_("invalid object ID: '%s'"), argv[1]);
	अगर (is_null_oid(&newoid))
		die(_("cannot create reference with null new object ID"));

	ret = refs_update_ref(get_main_ref_store(repo), message, refname,
			      &newoid, null_oid(repo->hash_algo), flags,
			      UPDATE_REFS_संदेश_ON_ERR);

	अगर (ret < 0)
		ret = 1;
	वापस ret;
}

अटल पूर्णांक cmd_refs_delete(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
			   काष्ठा repository *repo)
{
	अटल अक्षर स्थिर * स्थिर refs_delete_usage[] = {
		REFS_DELETE_USAGE,
		शून्य
	};
	स्थिर अक्षर *message = शून्य;
	अचिन्हित flags = 0;
	काष्ठा option opts[] = {
		OPT_STRING(0, "message", &message, N_("reason"),
			   N_("reason of the update")),
		OPT_BIT(0 ,"no-deref", &flags,
			N_("update <refname> not the one it points to"),
			REF_NO_DEREF),
		OPT_END(),
	};
	काष्ठा object_id oldoid;
	स्थिर अक्षर *refname;
	पूर्णांक ret;

	argc = parse_options(argc, argv, prefix, opts, refs_delete_usage, 0);
	अगर (argc < 1 || argc > 2)
		usage(_("delete requires reference name and an optional old object ID"));

	अगर (message && !*message)
		die(_("refusing to perform update with empty message"));

	repo_config(repo, git_default_config, शून्य);

	refname = argv[0];
	अगर (argc == 2) {
		अगर (repo_get_oid_with_flags(repo, argv[1], &oldoid, GET_OID_SKIP_AMBIGUITY_CHECK))
			die(_("invalid old object ID: '%s'"), argv[1]);
		अगर (is_null_oid(&oldoid))
			die(_("cannot delete reference with null old object ID"));
	}

	ret = refs_delete_ref(get_main_ref_store(repo), message, refname,
			      argc == 2 ? &oldoid : शून्य, flags);

	अगर (ret < 0)
		ret = 1;
	वापस ret;
}

अटल पूर्णांक cmd_refs_update(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
			   काष्ठा repository *repo)
{
	अटल अक्षर स्थिर * स्थिर refs_update_usage[] = {
		REFS_UPDATE_USAGE,
		शून्य
	};
	स्थिर अक्षर *message = शून्य;
	अचिन्हित flags = 0;
	काष्ठा option opts[] = {
		OPT_STRING(0, "message", &message, N_("reason"),
			   N_("reason of the update")),
		OPT_BIT(0 ,"no-deref", &flags,
			N_("update <refname> not the one it points to"),
			REF_NO_DEREF),
		OPT_BIT(0, "create-reflog", &flags, N_("create a reflog"),
			REF_FORCE_CREATE_REFLOG),
		OPT_END(),
	};
	काष्ठा object_id newoid, oldoid;
	स्थिर अक्षर *refname;
	पूर्णांक ret;

	argc = parse_options(argc, argv, prefix, opts, refs_update_usage, 0);
	अगर (argc < 2 || argc > 3)
		usage(_("update requires reference name, new value and an optional old value"));

	अगर (message && !*message)
		die(_("refusing to perform update with empty message"));

	repo_config(repo, git_default_config, शून्य);

	refname = argv[0];
	अगर (repo_get_oid_with_flags(repo, argv[1], &newoid,
				    GET_OID_SKIP_AMBIGUITY_CHECK))
		die(_("invalid new object ID: '%s'"), argv[1]);
	अगर (argc == 3 &&
	    repo_get_oid_with_flags(repo, argv[2], &oldoid,
				    GET_OID_SKIP_AMBIGUITY_CHECK))
		die(_("invalid old object ID: '%s'"), argv[2]);

	ret = refs_update_ref(get_main_ref_store(repo), message, refname,
			      &newoid, argc == 3 ? &oldoid : शून्य, flags,
			      UPDATE_REFS_संदेश_ON_ERR);

	अगर (ret < 0)
		ret = 1;
	वापस ret;
}

अटल पूर्णांक cmd_refs_rename(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
			   काष्ठा repository *repo)
{
	अटल अक्षर स्थिर * स्थिर refs_rename_usage[] = {
		REFS_RENAME_USAGE,
		शून्य
	};
	स्थिर अक्षर *message = शून्य;
	काष्ठा option opts[] = {
		OPT_STRING(0, "message", &message, N_("reason"),
			   N_("reason of the update")),
		OPT_END(),
	};
	स्थिर अक्षर *oldref, *newref;
	पूर्णांक ret;

	argc = parse_options(argc, argv, prefix, opts, refs_rename_usage, 0);
	अगर (argc != 2)
		usage(_("rename requires old and new reference name"));
	अगर (message && !*message)
		die(_("refusing to perform update with empty message"));

	repo_config(repo, git_default_config, शून्य);

	oldref = argv[0];
	newref = argv[1];

	अगर (check_refname_format(oldref, 0))
		die(_("invalid ref format: '%s'"), oldref);
	अगर (check_refname_format(newref, 0))
		die(_("invalid ref format: '%s'"), newref);

	अगर (!refs_ref_exists(get_main_ref_store(repo), oldref))
		die(_("reference does not exist: '%s'"), oldref);
	अगर (refs_ref_exists(get_main_ref_store(repo), newref))
		die(_("reference already exists: '%s'"), newref);

	ret = refs_rename_ref(get_main_ref_store(repo), oldref, newref, message);

	अगर (ret < 0)
		ret = 1;
	वापस ret;
}

पूर्णांक cmd_refs(पूर्णांक argc,
	     स्थिर अक्षर **argv,
	     स्थिर अक्षर *prefix,
	     काष्ठा repository *repo)
{
	स्थिर अक्षर * स्थिर refs_usage[] = {
		REFS_MIGRATE_USAGE,
		REFS_VERIFY_USAGE,
		REFS_OPTIMIZE_USAGE,
		"git refs list " COMMON_USAGE_FOR_EACH_REF,
		REFS_EXISTS_USAGE,
		REFS_CREATE_USAGE,
		REFS_DELETE_USAGE,
		REFS_UPDATE_USAGE,
		REFS_RENAME_USAGE,
		शून्य,
	};
	parse_opt_subcommand_fn *fn = शून्य;
	काष्ठा option opts[] = {
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
	वापस fn(argc, argv, prefix, repo);
}
