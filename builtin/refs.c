#شامل "builtin.h"
#شامل "config.h"
#شامل "fsck.h"
#شامل "pack-refs.h"
#شامل "parse-options.h"
#شامل "refs.h"
#شامل "strbuf.h"
#شامل "worktree.h"
#شامل "for-each-ref.h"
#شامل "refs/refs-internal.h"

#تعریف_کرو REFS_MIGRATE_USAGE \
	N_("git refs migrate --ref-storage-format=<format> [--no-reflog] [--dry-run]")

#تعریف_کرو REFS_VERIFY_USAGE \
	N_("git refs verify [--strict] [--verbose]")

#تعریف_کرو REFS_EXISTS_USAGE \
	N_("git refs exists <ref>")

#تعریف_کرو REFS_OPTIMIZE_USAGE \
	N_("git refs optimize " PACK_REFS_OPTS)

#تعریف_کرو REFS_CREATE_USAGE \
	N_("git refs create [--message=<reason>] [--no-deref] [--create-reflog] <ref> <new-value>")

#تعریف_کرو REFS_DELETE_USAGE \
	N_("git refs delete [--message=<reason>] [--no-deref] <ref> [<old-value>]")

#تعریف_کرو REFS_UPDATE_USAGE \
	N_("git refs update [--message=<reason>] [--no-deref] [--create-reflog] <ref> <new-value> [<old-value>]")

#تعریف_کرو REFS_RENAME_USAGE \
	N_("git refs rename [--message=<reason>] <old-ref> <new-ref>")

ساکن صحیح cmd_refs_migrate(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
			    ساخت repository *repo)
{
	ثابت حرف * ثابت migrate_usage[] = {
		REFS_MIGRATE_USAGE,
		تہی,
	};
	ثابت حرف *format_str = تہی;
	شمار ref_storage_format format;
	غیرعلامتی صحیح flags = 0;
	ساخت option options[] = {
		OPT_STRING_F(0, "ref-storage-format", &format_str, N_("format"),
			N_("specify the reference storage format to convert to"),
			PARSE_OPT_NONEG),
		OPT_ALIAS_F(0, "ref-format", "ref-storage-format", PARSE_OPT_HIDDEN),
		OPT_ثنائی_ہندسہ(0, "dry-run", &flags,
			N_("perform a non-destructive dry-run"),
			REPO_MIGRATE_REF_STORAGE_FORMAT_DRYRUN),
		OPT_ثنائی_ہندسہ(0, "no-reflog", &flags,
			N_("drop reflogs entirely during the migration"),
			REPO_MIGRATE_REF_STORAGE_FORMAT_SKIP_REFLOG),
		OPT_انتہا(),
	};
	ساخت strbuf errbuf = STRBUF_INIT;
	صحیح err;

	argc = parse_options(argc, argv, سابقہ, options, migrate_usage, 0);
	اگر (argc)
		usage(_("too many arguments"));
	اگر (!format_str)
		usage(_("missing --ref-storage-format=<format>"));

	format = ref_storage_format_by_name(format_str);
	اگر (format == REF_STORAGE_FORMAT_UNKNOWN) {
		err = error(_("unknown ref storage format '%s'"), format_str);
		جاؤ برآمد;
	}

	اگر (repo->ref_storage_format == format) {
		err = error(_("repository already uses '%s' format"),
			    ref_storage_format_مقصد_name(format));
		جاؤ برآمد;
	}

	اگر (repo_migrate_ref_storage_format(repo, format, flags, &errbuf) < 0) {
		err = error("%s", errbuf.buf);
		جاؤ برآمد;
	}

	err = 0;

برآمد:
	strbuf_release(&errbuf);
	واپس err;
}

ساکن صحیح cmd_refs_verify(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
			   ساخت repository *repo)
{
	ساخت fsck_options fsck_refs_options;
	ساخت worktree **worktrees;
	ثابت حرف * ثابت verify_usage[] = {
		REFS_VERIFY_USAGE,
		تہی,
	};
	ساخت option options[] = {
		OPT_BOOL(0, "verbose", &fsck_refs_options.verbose, N_("be verbose")),
		OPT_BOOL(0, "strict", &fsck_refs_options.strict, N_("enable strict checking")),
		OPT_انتہا(),
	};
	صحیح ret = 0;

	fsck_options_init(&fsck_refs_options, repo, FSCK_OPTIONS_REFS);

	argc = parse_options(argc, argv, سابقہ, options, verify_usage, 0);
	اگر (argc)
		usage(_("'git refs verify' takes no arguments"));

	repo_config(repo, git_fsck_config, &fsck_refs_options);
	prepare_repo_settings(repo);

	worktrees = get_worktrees_without_reading_سر(repo);
	برائے (سائز_t i = 0; worktrees[i]; i++)
		ret |= refs_fsck(get_worktree_ref_store(worktrees[i]),
				 &fsck_refs_options, worktrees[i]);

	fsck_options_clear(&fsck_refs_options);
	free_worktrees(worktrees);
	واپس ret;
}

ساکن صحیح cmd_refs_list(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
			   ساخت repository *repo)
{
	ساکن حرف ثابت * ثابت refs_list_usage[] = {
		N_("git refs list " COMMON_USAGE_FOR_EACH_REF),
		تہی
	};

	واپس for_each_ref_core(argc, argv, سابقہ, repo, refs_list_usage);
}

ساکن صحیح cmd_refs_exists(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
			   ساخت repository *repo)
{
	ساخت strbuf غیر_مستعمل_referent = STRBUF_INIT;
	ساخت object_id غیر_مستعمل_oid;
	غیرعلامتی صحیح غیر_مستعمل_type;
	صحیح failure_errno = 0;
	ثابت حرف *ref;
	صحیح ret = 0;
	ثابت حرف * ثابت exists_usage[] = {
		REFS_EXISTS_USAGE,
		تہی,
	};
	ساخت option options[] = {
		OPT_انتہا(),
	};

	argc = parse_options(argc, argv, سابقہ, options, exists_usage, 0);
	اگر (argc != 1)
		die(_("'git refs exists' requires a reference"));

	ref = *argv++;
	اگر (refs_read_raw_ref(get_main_ref_store(repo), ref,
			      &غیر_مستعمل_oid, &غیر_مستعمل_referent, &غیر_مستعمل_type,
			      &failure_errno)) {
		اگر (failure_errno == ENOENT || failure_errno == EISDIR) {
			error(_("reference does not exist"));
			ret = 2;
		} ورنہ {
			errno = failure_errno;
			error_errno(_("failed to look up reference"));
			ret = 1;
		}

		جاؤ برآمد;
	}

برآمد:
	strbuf_release(&غیر_مستعمل_referent);
	واپس ret;
}

ساکن صحیح cmd_refs_optimize(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
			     ساخت repository *repo)
{
	ساکن حرف ثابت * ثابت refs_optimize_usage[] = {
		REFS_OPTIMIZE_USAGE,
		تہی
	};

	واپس pack_refs_core(argc, argv, سابقہ, repo, refs_optimize_usage);
}

ساکن صحیح cmd_refs_create(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
			   ساخت repository *repo)
{
	ساکن حرف ثابت * ثابت refs_create_usage[] = {
		REFS_CREATE_USAGE,
		تہی
	};
	ثابت حرف *پیغام_2 = تہی;
	غیرعلامتی flags = 0;
	ساخت option opts[] = {
		OPT_STRING(0, "message", &پیغام_2, N_("reason"),
			   N_("reason of the update")),
		OPT_ثنائی_ہندسہ(0 ,"no-deref", &flags,
			N_("update <refname> not the one it points to"),
			REF_NO_DEREF),
		OPT_ثنائی_ہندسہ(0, "create-reflog", &flags, N_("create a reflog"),
			REF_FORCE_CREATE_REFLOG),
		OPT_انتہا(),
	};
	ساخت object_id newoid;
	ثابت حرف *refname;
	صحیح ret;

	argc = parse_options(argc, argv, سابقہ, opts, refs_create_usage, 0);
	اگر (argc != 2)
		usage(_("create requires reference name and an object ID"));

	اگر (پیغام_2 && !*پیغام_2)
		die(_("refusing to perform update with empty message"));

	repo_config(repo, git_default_config, تہی);

	refname = argv[0];
	اگر (repo_get_oid_with_flags(repo, argv[1], &newoid, GET_OID_SKIP_AMBIGUITY_CHECK))
		die(_("invalid object ID: '%s'"), argv[1]);
	اگر (is_null_oid(&newoid))
		die(_("cannot create reference with null new object ID"));

	ret = refs_update_ref(get_main_ref_store(repo), پیغام_2, refname,
			      &newoid, null_oid(repo->hash_algo), flags,
			      UPDATE_REFS_پیغام_ON_ERR);

	اگر (ret < 0)
		ret = 1;
	واپس ret;
}

ساکن صحیح cmd_refs_delete(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
			   ساخت repository *repo)
{
	ساکن حرف ثابت * ثابت refs_delete_usage[] = {
		REFS_DELETE_USAGE,
		تہی
	};
	ثابت حرف *پیغام_2 = تہی;
	غیرعلامتی flags = 0;
	ساخت option opts[] = {
		OPT_STRING(0, "message", &پیغام_2, N_("reason"),
			   N_("reason of the update")),
		OPT_ثنائی_ہندسہ(0 ,"no-deref", &flags,
			N_("update <refname> not the one it points to"),
			REF_NO_DEREF),
		OPT_انتہا(),
	};
	ساخت object_id oldoid;
	ثابت حرف *refname;
	صحیح ret;

	argc = parse_options(argc, argv, سابقہ, opts, refs_delete_usage, 0);
	اگر (argc < 1 || argc > 2)
		usage(_("delete requires reference name and an optional old object ID"));

	اگر (پیغام_2 && !*پیغام_2)
		die(_("refusing to perform update with empty message"));

	repo_config(repo, git_default_config, تہی);

	refname = argv[0];
	اگر (argc == 2) {
		اگر (repo_get_oid_with_flags(repo, argv[1], &oldoid, GET_OID_SKIP_AMBIGUITY_CHECK))
			die(_("invalid old object ID: '%s'"), argv[1]);
		اگر (is_null_oid(&oldoid))
			die(_("cannot delete reference with null old object ID"));
	}

	ret = refs_delete_ref(get_main_ref_store(repo), پیغام_2, refname,
			      argc == 2 ? &oldoid : تہی, flags);

	اگر (ret < 0)
		ret = 1;
	واپس ret;
}

ساکن صحیح cmd_refs_update(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
			   ساخت repository *repo)
{
	ساکن حرف ثابت * ثابت refs_update_usage[] = {
		REFS_UPDATE_USAGE,
		تہی
	};
	ثابت حرف *پیغام_2 = تہی;
	غیرعلامتی flags = 0;
	ساخت option opts[] = {
		OPT_STRING(0, "message", &پیغام_2, N_("reason"),
			   N_("reason of the update")),
		OPT_ثنائی_ہندسہ(0 ,"no-deref", &flags,
			N_("update <refname> not the one it points to"),
			REF_NO_DEREF),
		OPT_ثنائی_ہندسہ(0, "create-reflog", &flags, N_("create a reflog"),
			REF_FORCE_CREATE_REFLOG),
		OPT_انتہا(),
	};
	ساخت object_id newoid, oldoid;
	ثابت حرف *refname;
	صحیح ret;

	argc = parse_options(argc, argv, سابقہ, opts, refs_update_usage, 0);
	اگر (argc < 2 || argc > 3)
		usage(_("update requires reference name, new value and an optional old value"));

	اگر (پیغام_2 && !*پیغام_2)
		die(_("refusing to perform update with empty message"));

	repo_config(repo, git_default_config, تہی);

	refname = argv[0];
	اگر (repo_get_oid_with_flags(repo, argv[1], &newoid,
				    GET_OID_SKIP_AMBIGUITY_CHECK))
		die(_("invalid new object ID: '%s'"), argv[1]);
	اگر (argc == 3 &&
	    repo_get_oid_with_flags(repo, argv[2], &oldoid,
				    GET_OID_SKIP_AMBIGUITY_CHECK))
		die(_("invalid old object ID: '%s'"), argv[2]);

	ret = refs_update_ref(get_main_ref_store(repo), پیغام_2, refname,
			      &newoid, argc == 3 ? &oldoid : تہی, flags,
			      UPDATE_REFS_پیغام_ON_ERR);

	اگر (ret < 0)
		ret = 1;
	واپس ret;
}

ساکن صحیح cmd_refs_rename(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
			   ساخت repository *repo)
{
	ساکن حرف ثابت * ثابت refs_rename_usage[] = {
		REFS_RENAME_USAGE,
		تہی
	};
	ثابت حرف *پیغام_2 = تہی;
	ساخت option opts[] = {
		OPT_STRING(0, "message", &پیغام_2, N_("reason"),
			   N_("reason of the update")),
		OPT_انتہا(),
	};
	ثابت حرف *oldref, *newref;
	صحیح ret;

	argc = parse_options(argc, argv, سابقہ, opts, refs_rename_usage, 0);
	اگر (argc != 2)
		usage(_("rename requires old and new reference name"));
	اگر (پیغام_2 && !*پیغام_2)
		die(_("refusing to perform update with empty message"));

	repo_config(repo, git_default_config, تہی);

	oldref = argv[0];
	newref = argv[1];

	اگر (check_refname_format(oldref, 0))
		die(_("invalid ref format: '%s'"), oldref);
	اگر (check_refname_format(newref, 0))
		die(_("invalid ref format: '%s'"), newref);

	اگر (!refs_ref_exists(get_main_ref_store(repo), oldref))
		die(_("reference does not exist: '%s'"), oldref);
	اگر (refs_ref_exists(get_main_ref_store(repo), newref))
		die(_("reference already exists: '%s'"), newref);

	ret = refs_rename_ref(get_main_ref_store(repo), oldref, newref, پیغام_2);

	اگر (ret < 0)
		ret = 1;
	واپس ret;
}

صحیح cmd_refs(صحیح argc,
	     ثابت حرف **argv,
	     ثابت حرف *سابقہ,
	     ساخت repository *repo)
{
	ثابت حرف * ثابت refs_usage[] = {
		REFS_MIGRATE_USAGE,
		REFS_VERIFY_USAGE,
		REFS_OPTIMIZE_USAGE,
		"git refs list " COMMON_USAGE_FOR_EACH_REF,
		REFS_EXISTS_USAGE,
		REFS_CREATE_USAGE,
		REFS_DELETE_USAGE,
		REFS_UPDATE_USAGE,
		REFS_RENAME_USAGE,
		تہی,
	};
	parse_opt_subcommand_fn *fn = تہی;
	ساخت option opts[] = {
		OPT_گروہ(N_("Reference database")),
		OPT_SUBCOMMAND_F("migrate", &fn, cmd_refs_migrate,
				 N_("migrate the reference database to a different format"), 0),
		OPT_SUBCOMMAND_F("verify", &fn, cmd_refs_verify,
				 N_("verify the consistency of the reference database"), 0),
		OPT_SUBCOMMAND_F("optimize", &fn, cmd_refs_optimize,
				 N_("optimize the reference database"), 0),
		OPT_گروہ(N_("Reading references")),
		OPT_SUBCOMMAND_F("list", &fn, cmd_refs_list,
				 N_("list references"), 0),
		OPT_SUBCOMMAND_F("exists", &fn, cmd_refs_exists,
				 N_("check whether a reference exists"), 0),
		OPT_گروہ(N_("Writing references")),
		OPT_SUBCOMMAND_F("create", &fn, cmd_refs_create,
				 N_("create a new reference"), 0),
		OPT_SUBCOMMAND_F("delete", &fn, cmd_refs_delete,
				 N_("delete a reference"), 0),
		OPT_SUBCOMMAND_F("update", &fn, cmd_refs_update,
				 N_("update an existing reference"), 0),
		OPT_SUBCOMMAND_F("rename", &fn, cmd_refs_rename,
				 N_("rename a reference"), 0),
		OPT_انتہا(),
	};

	argc = parse_options(argc, argv, سابقہ, opts, refs_usage, 0);
	واپس fn(argc, argv, سابقہ, repo);
}
