#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "fsck.h"
#ضمن "pack-refs.h"
#ضمن "parse-options.h"
#ضمن "refs.h"
#ضمن "strbuf.h"
#ضمن "worktree.h"
#ضمن "for-each-ref.h"
#ضمن "refs/refs-internal.h"

#تعریف_کن REFS_MIGRATE_USAGE \
	N_("git refs migrate --ref-storage-format=<format> [--no-reflog] [--dry-run]")

#تعریف_کن REFS_VERIFY_USAGE \
	N_("git refs verify [--strict] [--verbose]")

#تعریف_کن REFS_EXISTS_USAGE \
	N_("git refs exists <ref>")

#تعریف_کن REFS_OPTIMIZE_USAGE \
	N_("git refs optimize " PACK_REFS_OPTS)

#تعریف_کن REFS_CREATE_USAGE \
	N_("git refs create [--message=<reason>] [--no-deref] [--create-reflog] <ref> <new-value>")

#تعریف_کن REFS_DELETE_USAGE \
	N_("git refs delete [--message=<reason>] [--no-deref] <ref> [<old-value>]")

#تعریف_کن REFS_UPDATE_USAGE \
	N_("git refs update [--message=<reason>] [--no-deref] [--create-reflog] <ref> <new-value> [<old-value>]")

#تعریف_کن REFS_RENAME_USAGE \
	N_("git refs rename [--message=<reason>] <old-ref> <new-ref>")

ساكن صحيح cmd_refs_migrate(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
			    بنية repository *repo)
{
	ثابت حرف * ثابت migrate_usage[] = {
		REFS_MIGRATE_USAGE,
		عدم,
	};
	ثابت حرف *format_str = عدم;
	تعداد ref_storage_format format;
	بیعلامت صحيح flags = 0;
	بنية option options[] = {
		OPT_STRING_F(0, "ref-storage-format", &format_str, N_("format"),
			N_("specify the reference storage format to convert to"),
			PARSE_OPT_NONEG),
		OPT_ALIAS_F(0, "ref-format", "ref-storage-format", PARSE_OPT_HIDDEN),
		OPT_بیت(0, "dry-run", &flags,
			N_("perform a non-destructive dry-run"),
			REPO_MIGRATE_REF_STORAGE_FORMAT_DRYRUN),
		OPT_بیت(0, "no-reflog", &flags,
			N_("drop reflogs entirely during the migration"),
			REPO_MIGRATE_REF_STORAGE_FORMAT_SKIP_REFLOG),
		OPT_انتها(),
	};
	بنية strbuf errbuf = STRBUF_INIT;
	صحيح err;

	argc = parse_options(argc, argv, پیشوند, options, migrate_usage, 0);
	إذا (argc)
		usage(_("too many arguments"));
	إذا (!format_str)
		usage(_("missing --ref-storage-format=<format>"));

	format = ref_storage_format_by_name(format_str);
	إذا (format == REF_STORAGE_FORMAT_UNKNOWN) {
		err = error(_("unknown ref storage format '%s'"), format_str);
		اذهب بیرون;
	}

	إذا (repo->ref_storage_format == format) {
		err = error(_("repository already uses '%s' format"),
			    ref_storage_format_مقصد_name(format));
		اذهب بیرون;
	}

	إذا (repo_migrate_ref_storage_format(repo, format, flags, &errbuf) < 0) {
		err = error("%s", errbuf.buf);
		اذهب بیرون;
	}

	err = 0;

بیرون:
	strbuf_release(&errbuf);
	أرجع err;
}

ساكن صحيح cmd_refs_verify(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
			   بنية repository *repo)
{
	بنية fsck_options fsck_refs_options;
	بنية worktree **worktrees;
	ثابت حرف * ثابت verify_usage[] = {
		REFS_VERIFY_USAGE,
		عدم,
	};
	بنية option options[] = {
		OPT_BOOL(0, "verbose", &fsck_refs_options.verbose, N_("be verbose")),
		OPT_BOOL(0, "strict", &fsck_refs_options.strict, N_("enable strict checking")),
		OPT_انتها(),
	};
	صحيح ret = 0;

	fsck_options_init(&fsck_refs_options, repo, FSCK_OPTIONS_REFS);

	argc = parse_options(argc, argv, پیشوند, options, verify_usage, 0);
	إذا (argc)
		usage(_("'git refs verify' takes no arguments"));

	repo_config(repo, git_fsck_config, &fsck_refs_options);
	prepare_repo_settings(repo);

	worktrees = get_worktrees_without_reading_سر(repo);
	لكل (اندازه_t i = 0; worktrees[i]; i++)
		ret |= refs_fsck(get_worktree_ref_store(worktrees[i]),
				 &fsck_refs_options, worktrees[i]);

	fsck_options_clear(&fsck_refs_options);
	free_worktrees(worktrees);
	أرجع ret;
}

ساكن صحيح cmd_refs_list(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
			   بنية repository *repo)
{
	ساكن حرف ثابت * ثابت refs_list_usage[] = {
		N_("git refs list " COMMON_USAGE_FOR_EACH_REF),
		عدم
	};

	أرجع for_each_ref_core(argc, argv, پیشوند, repo, refs_list_usage);
}

ساكن صحيح cmd_refs_exists(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
			   بنية repository *repo)
{
	بنية strbuf استفادهنشده_referent = STRBUF_INIT;
	بنية object_id استفادهنشده_oid;
	بیعلامت صحيح استفادهنشده_type;
	صحيح failure_errno = 0;
	ثابت حرف *ref;
	صحيح ret = 0;
	ثابت حرف * ثابت exists_usage[] = {
		REFS_EXISTS_USAGE,
		عدم,
	};
	بنية option options[] = {
		OPT_انتها(),
	};

	argc = parse_options(argc, argv, پیشوند, options, exists_usage, 0);
	إذا (argc != 1)
		die(_("'git refs exists' requires a reference"));

	ref = *argv++;
	إذا (refs_read_raw_ref(get_main_ref_store(repo), ref,
			      &استفادهنشده_oid, &استفادهنشده_referent, &استفادهنشده_type,
			      &failure_errno)) {
		إذا (failure_errno == ENOENT || failure_errno == EISDIR) {
			error(_("reference does not exist"));
			ret = 2;
		} وإلا {
			errno = failure_errno;
			error_errno(_("failed to look up reference"));
			ret = 1;
		}

		اذهب بیرون;
	}

بیرون:
	strbuf_release(&استفادهنشده_referent);
	أرجع ret;
}

ساكن صحيح cmd_refs_optimize(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
			     بنية repository *repo)
{
	ساكن حرف ثابت * ثابت refs_optimize_usage[] = {
		REFS_OPTIMIZE_USAGE,
		عدم
	};

	أرجع pack_refs_core(argc, argv, پیشوند, repo, refs_optimize_usage);
}

ساكن صحيح cmd_refs_create(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
			   بنية repository *repo)
{
	ساكن حرف ثابت * ثابت refs_create_usage[] = {
		REFS_CREATE_USAGE,
		عدم
	};
	ثابت حرف *پیام_2 = عدم;
	بیعلامت flags = 0;
	بنية option opts[] = {
		OPT_STRING(0, "message", &پیام_2, N_("reason"),
			   N_("reason of the update")),
		OPT_بیت(0 ,"no-deref", &flags,
			N_("update <refname> not the one it points to"),
			REF_NO_DEREF),
		OPT_بیت(0, "create-reflog", &flags, N_("create a reflog"),
			REF_FORCE_CREATE_REFLOG),
		OPT_انتها(),
	};
	بنية object_id newoid;
	ثابت حرف *refname;
	صحيح ret;

	argc = parse_options(argc, argv, پیشوند, opts, refs_create_usage, 0);
	إذا (argc != 2)
		usage(_("create requires reference name and an object ID"));

	إذا (پیام_2 && !*پیام_2)
		die(_("refusing to perform update with empty message"));

	repo_config(repo, git_default_config, عدم);

	refname = argv[0];
	إذا (repo_get_oid_with_flags(repo, argv[1], &newoid, GET_OID_SKIP_AMBIGUITY_CHECK))
		die(_("invalid object ID: '%s'"), argv[1]);
	إذا (is_null_oid(&newoid))
		die(_("cannot create reference with null new object ID"));

	ret = refs_update_ref(get_main_ref_store(repo), پیام_2, refname,
			      &newoid, null_oid(repo->hash_algo), flags,
			      UPDATE_REFS_پیام_ON_ERR);

	إذا (ret < 0)
		ret = 1;
	أرجع ret;
}

ساكن صحيح cmd_refs_delete(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
			   بنية repository *repo)
{
	ساكن حرف ثابت * ثابت refs_delete_usage[] = {
		REFS_DELETE_USAGE,
		عدم
	};
	ثابت حرف *پیام_2 = عدم;
	بیعلامت flags = 0;
	بنية option opts[] = {
		OPT_STRING(0, "message", &پیام_2, N_("reason"),
			   N_("reason of the update")),
		OPT_بیت(0 ,"no-deref", &flags,
			N_("update <refname> not the one it points to"),
			REF_NO_DEREF),
		OPT_انتها(),
	};
	بنية object_id oldoid;
	ثابت حرف *refname;
	صحيح ret;

	argc = parse_options(argc, argv, پیشوند, opts, refs_delete_usage, 0);
	إذا (argc < 1 || argc > 2)
		usage(_("delete requires reference name and an optional old object ID"));

	إذا (پیام_2 && !*پیام_2)
		die(_("refusing to perform update with empty message"));

	repo_config(repo, git_default_config, عدم);

	refname = argv[0];
	إذا (argc == 2) {
		إذا (repo_get_oid_with_flags(repo, argv[1], &oldoid, GET_OID_SKIP_AMBIGUITY_CHECK))
			die(_("invalid old object ID: '%s'"), argv[1]);
		إذا (is_null_oid(&oldoid))
			die(_("cannot delete reference with null old object ID"));
	}

	ret = refs_delete_ref(get_main_ref_store(repo), پیام_2, refname,
			      argc == 2 ? &oldoid : عدم, flags);

	إذا (ret < 0)
		ret = 1;
	أرجع ret;
}

ساكن صحيح cmd_refs_update(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
			   بنية repository *repo)
{
	ساكن حرف ثابت * ثابت refs_update_usage[] = {
		REFS_UPDATE_USAGE,
		عدم
	};
	ثابت حرف *پیام_2 = عدم;
	بیعلامت flags = 0;
	بنية option opts[] = {
		OPT_STRING(0, "message", &پیام_2, N_("reason"),
			   N_("reason of the update")),
		OPT_بیت(0 ,"no-deref", &flags,
			N_("update <refname> not the one it points to"),
			REF_NO_DEREF),
		OPT_بیت(0, "create-reflog", &flags, N_("create a reflog"),
			REF_FORCE_CREATE_REFLOG),
		OPT_انتها(),
	};
	بنية object_id newoid, oldoid;
	ثابت حرف *refname;
	صحيح ret;

	argc = parse_options(argc, argv, پیشوند, opts, refs_update_usage, 0);
	إذا (argc < 2 || argc > 3)
		usage(_("update requires reference name, new value and an optional old value"));

	إذا (پیام_2 && !*پیام_2)
		die(_("refusing to perform update with empty message"));

	repo_config(repo, git_default_config, عدم);

	refname = argv[0];
	إذا (repo_get_oid_with_flags(repo, argv[1], &newoid,
				    GET_OID_SKIP_AMBIGUITY_CHECK))
		die(_("invalid new object ID: '%s'"), argv[1]);
	إذا (argc == 3 &&
	    repo_get_oid_with_flags(repo, argv[2], &oldoid,
				    GET_OID_SKIP_AMBIGUITY_CHECK))
		die(_("invalid old object ID: '%s'"), argv[2]);

	ret = refs_update_ref(get_main_ref_store(repo), پیام_2, refname,
			      &newoid, argc == 3 ? &oldoid : عدم, flags,
			      UPDATE_REFS_پیام_ON_ERR);

	إذا (ret < 0)
		ret = 1;
	أرجع ret;
}

ساكن صحيح cmd_refs_rename(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
			   بنية repository *repo)
{
	ساكن حرف ثابت * ثابت refs_rename_usage[] = {
		REFS_RENAME_USAGE,
		عدم
	};
	ثابت حرف *پیام_2 = عدم;
	بنية option opts[] = {
		OPT_STRING(0, "message", &پیام_2, N_("reason"),
			   N_("reason of the update")),
		OPT_انتها(),
	};
	ثابت حرف *oldref, *newref;
	صحيح ret;

	argc = parse_options(argc, argv, پیشوند, opts, refs_rename_usage, 0);
	إذا (argc != 2)
		usage(_("rename requires old and new reference name"));
	إذا (پیام_2 && !*پیام_2)
		die(_("refusing to perform update with empty message"));

	repo_config(repo, git_default_config, عدم);

	oldref = argv[0];
	newref = argv[1];

	إذا (check_refname_format(oldref, 0))
		die(_("invalid ref format: '%s'"), oldref);
	إذا (check_refname_format(newref, 0))
		die(_("invalid ref format: '%s'"), newref);

	إذا (!refs_ref_exists(get_main_ref_store(repo), oldref))
		die(_("reference does not exist: '%s'"), oldref);
	إذا (refs_ref_exists(get_main_ref_store(repo), newref))
		die(_("reference already exists: '%s'"), newref);

	ret = refs_rename_ref(get_main_ref_store(repo), oldref, newref, پیام_2);

	إذا (ret < 0)
		ret = 1;
	أرجع ret;
}

صحيح cmd_refs(صحيح argc,
	     ثابت حرف **argv,
	     ثابت حرف *پیشوند,
	     بنية repository *repo)
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
		عدم,
	};
	parse_opt_subcommand_fn *fn = عدم;
	بنية option opts[] = {
		OPT_گروه(N_("Reference database")),
		OPT_SUBCOMMAND_F("migrate", &fn, cmd_refs_migrate,
				 N_("migrate the reference database to a different format"), 0),
		OPT_SUBCOMMAND_F("verify", &fn, cmd_refs_verify,
				 N_("verify the consistency of the reference database"), 0),
		OPT_SUBCOMMAND_F("optimize", &fn, cmd_refs_optimize,
				 N_("optimize the reference database"), 0),
		OPT_گروه(N_("Reading references")),
		OPT_SUBCOMMAND_F("list", &fn, cmd_refs_list,
				 N_("list references"), 0),
		OPT_SUBCOMMAND_F("exists", &fn, cmd_refs_exists,
				 N_("check whether a reference exists"), 0),
		OPT_گروه(N_("Writing references")),
		OPT_SUBCOMMAND_F("create", &fn, cmd_refs_create,
				 N_("create a new reference"), 0),
		OPT_SUBCOMMAND_F("delete", &fn, cmd_refs_delete,
				 N_("delete a reference"), 0),
		OPT_SUBCOMMAND_F("update", &fn, cmd_refs_update,
				 N_("update an existing reference"), 0),
		OPT_SUBCOMMAND_F("rename", &fn, cmd_refs_rename,
				 N_("rename a reference"), 0),
		OPT_انتها(),
	};

	argc = parse_options(argc, argv, پیشوند, opts, refs_usage, 0);
	أرجع fn(argc, argv, پیشوند, repo);
}
