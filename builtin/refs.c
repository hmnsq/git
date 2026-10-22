#అన్తర్భావ "builtin.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "fsck.h"
#అన్తర్భావ "pack-refs.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "refs.h"
#అన్తర్భావ "strbuf.h"
#అన్తర్భావ "worktree.h"
#అన్తర్భావ "for-each-ref.h"
#అన్తర్భావ "refs/refs-internal.h"

#నిర్వచించు REFS_MIGRATE_USAGE \
	N_("git refs migrate --ref-storage-format=<format> [--no-reflog] [--dry-run]")

#నిర్వచించు REFS_VERIFY_USAGE \
	N_("git refs verify [--strict] [--verbose]")

#నిర్వచించు REFS_EXISTS_USAGE \
	N_("git refs exists <ref>")

#నిర్వచించు REFS_OPTIMIZE_USAGE \
	N_("git refs optimize " PACK_REFS_OPTS)

#నిర్వచించు REFS_CREATE_USAGE \
	N_("git refs create [--message=<reason>] [--no-deref] [--create-reflog] <ref> <new-value>")

#నిర్వచించు REFS_DELETE_USAGE \
	N_("git refs delete [--message=<reason>] [--no-deref] <ref> [<old-value>]")

#నిర్వచించు REFS_UPDATE_USAGE \
	N_("git refs update [--message=<reason>] [--no-deref] [--create-reflog] <ref> <new-value> [<old-value>]")

#నిర్వచించు REFS_RENAME_USAGE \
	N_("git refs rename [--message=<reason>] <old-ref> <new-ref>")

స్థిర పూర్ణాంకం cmd_refs_migrate(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
			    నిర్మాణం repository *repo)
{
	స్థిరరాశి అక్షరం * స్థిరరాశి migrate_usage[] = {
		REFS_MIGRATE_USAGE,
		ఖాళీ,
	};
	స్థిరరాశి అక్షరం *format_str = ఖాళీ;
	గణన ref_storage_format format;
	గుర్తులేని పూర్ణాంకం flags = 0;
	నిర్మాణం option options[] = {
		OPT_STRING_F(0, "ref-storage-format", &format_str, N_("format"),
			N_("specify the reference storage format to convert to"),
			PARSE_OPT_NONEG),
		OPT_ALIAS_F(0, "ref-format", "ref-storage-format", PARSE_OPT_HIDDEN),
		OPT_బిట్టు(0, "dry-run", &flags,
			N_("perform a non-destructive dry-run"),
			REPO_MIGRATE_REF_STORAGE_FORMAT_DRYRUN),
		OPT_బిట్టు(0, "no-reflog", &flags,
			N_("drop reflogs entirely during the migration"),
			REPO_MIGRATE_REF_STORAGE_FORMAT_SKIP_REFLOG),
		OPT_చివర(),
	};
	నిర్మాణం strbuf errbuf = STRBUF_INIT;
	పూర్ణాంకం err;

	argc = parse_options(argc, argv, ఉపసర్గ, options, migrate_usage, 0);
	ఒకవేళ (argc)
		usage(_("too many arguments"));
	ఒకవేళ (!format_str)
		usage(_("missing --ref-storage-format=<format>"));

	format = ref_storage_format_by_name(format_str);
	ఒకవేళ (format == REF_STORAGE_FORMAT_UNKNOWN) {
		err = error(_("unknown ref storage format '%s'"), format_str);
		వెళ్ళు బహిర్గతం;
	}

	ఒకవేళ (repo->ref_storage_format == format) {
		err = error(_("repository already uses '%s' format"),
			    ref_storage_format_గమ్యం_name(format));
		వెళ్ళు బహిర్గతం;
	}

	ఒకవేళ (repo_migrate_ref_storage_format(repo, format, flags, &errbuf) < 0) {
		err = error("%s", errbuf.buf);
		వెళ్ళు బహిర్గతం;
	}

	err = 0;

బహిర్గతం:
	strbuf_release(&errbuf);
	తిరిగి err;
}

స్థిర పూర్ణాంకం cmd_refs_verify(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
			   నిర్మాణం repository *repo)
{
	నిర్మాణం fsck_options fsck_refs_options;
	నిర్మాణం worktree **worktrees;
	స్థిరరాశి అక్షరం * స్థిరరాశి verify_usage[] = {
		REFS_VERIFY_USAGE,
		ఖాళీ,
	};
	నిర్మాణం option options[] = {
		OPT_BOOL(0, "verbose", &fsck_refs_options.verbose, N_("be verbose")),
		OPT_BOOL(0, "strict", &fsck_refs_options.strict, N_("enable strict checking")),
		OPT_చివర(),
	};
	పూర్ణాంకం ret = 0;

	fsck_options_init(&fsck_refs_options, repo, FSCK_OPTIONS_REFS);

	argc = parse_options(argc, argv, ఉపసర్గ, options, verify_usage, 0);
	ఒకవేళ (argc)
		usage(_("'git refs verify' takes no arguments"));

	repo_config(repo, git_fsck_config, &fsck_refs_options);
	prepare_repo_settings(repo);

	worktrees = get_worktrees_without_reading_తల(repo);
	కొరకు (కొలత_t i = 0; worktrees[i]; i++)
		ret |= refs_fsck(get_worktree_ref_store(worktrees[i]),
				 &fsck_refs_options, worktrees[i]);

	fsck_options_clear(&fsck_refs_options);
	free_worktrees(worktrees);
	తిరిగి ret;
}

స్థిర పూర్ణాంకం cmd_refs_list(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
			   నిర్మాణం repository *repo)
{
	స్థిర అక్షరం స్థిరరాశి * స్థిరరాశి refs_list_usage[] = {
		N_("git refs list " COMMON_USAGE_FOR_EACH_REF),
		ఖాళీ
	};

	తిరిగి for_each_ref_core(argc, argv, ఉపసర్గ, repo, refs_list_usage);
}

స్థిర పూర్ణాంకం cmd_refs_exists(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
			   నిర్మాణం repository *repo)
{
	నిర్మాణం strbuf వాడని_referent = STRBUF_INIT;
	నిర్మాణం object_id వాడని_oid;
	గుర్తులేని పూర్ణాంకం వాడని_type;
	పూర్ణాంకం failure_errno = 0;
	స్థిరరాశి అక్షరం *ref;
	పూర్ణాంకం ret = 0;
	స్థిరరాశి అక్షరం * స్థిరరాశి exists_usage[] = {
		REFS_EXISTS_USAGE,
		ఖాళీ,
	};
	నిర్మాణం option options[] = {
		OPT_చివర(),
	};

	argc = parse_options(argc, argv, ఉపసర్గ, options, exists_usage, 0);
	ఒకవేళ (argc != 1)
		die(_("'git refs exists' requires a reference"));

	ref = *argv++;
	ఒకవేళ (refs_read_raw_ref(get_main_ref_store(repo), ref,
			      &వాడని_oid, &వాడని_referent, &వాడని_type,
			      &failure_errno)) {
		ఒకవేళ (failure_errno == ENOENT || failure_errno == EISDIR) {
			error(_("reference does not exist"));
			ret = 2;
		} లేకపోతే {
			errno = failure_errno;
			error_errno(_("failed to look up reference"));
			ret = 1;
		}

		వెళ్ళు బహిర్గతం;
	}

బహిర్గతం:
	strbuf_release(&వాడని_referent);
	తిరిగి ret;
}

స్థిర పూర్ణాంకం cmd_refs_optimize(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
			     నిర్మాణం repository *repo)
{
	స్థిర అక్షరం స్థిరరాశి * స్థిరరాశి refs_optimize_usage[] = {
		REFS_OPTIMIZE_USAGE,
		ఖాళీ
	};

	తిరిగి pack_refs_core(argc, argv, ఉపసర్గ, repo, refs_optimize_usage);
}

స్థిర పూర్ణాంకం cmd_refs_create(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
			   నిర్మాణం repository *repo)
{
	స్థిర అక్షరం స్థిరరాశి * స్థిరరాశి refs_create_usage[] = {
		REFS_CREATE_USAGE,
		ఖాళీ
	};
	స్థిరరాశి అక్షరం *సందేశం_2 = ఖాళీ;
	గుర్తులేని flags = 0;
	నిర్మాణం option opts[] = {
		OPT_STRING(0, "message", &సందేశం_2, N_("reason"),
			   N_("reason of the update")),
		OPT_బిట్టు(0 ,"no-deref", &flags,
			N_("update <refname> not the one it points to"),
			REF_NO_DEREF),
		OPT_బిట్టు(0, "create-reflog", &flags, N_("create a reflog"),
			REF_FORCE_CREATE_REFLOG),
		OPT_చివర(),
	};
	నిర్మాణం object_id newoid;
	స్థిరరాశి అక్షరం *refname;
	పూర్ణాంకం ret;

	argc = parse_options(argc, argv, ఉపసర్గ, opts, refs_create_usage, 0);
	ఒకవేళ (argc != 2)
		usage(_("create requires reference name and an object ID"));

	ఒకవేళ (సందేశం_2 && !*సందేశం_2)
		die(_("refusing to perform update with empty message"));

	repo_config(repo, git_default_config, ఖాళీ);

	refname = argv[0];
	ఒకవేళ (repo_get_oid_with_flags(repo, argv[1], &newoid, GET_OID_SKIP_AMBIGUITY_CHECK))
		die(_("invalid object ID: '%s'"), argv[1]);
	ఒకవేళ (is_null_oid(&newoid))
		die(_("cannot create reference with null new object ID"));

	ret = refs_update_ref(get_main_ref_store(repo), సందేశం_2, refname,
			      &newoid, null_oid(repo->hash_algo), flags,
			      UPDATE_REFS_సందేశం_ON_ERR);

	ఒకవేళ (ret < 0)
		ret = 1;
	తిరిగి ret;
}

స్థిర పూర్ణాంకం cmd_refs_delete(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
			   నిర్మాణం repository *repo)
{
	స్థిర అక్షరం స్థిరరాశి * స్థిరరాశి refs_delete_usage[] = {
		REFS_DELETE_USAGE,
		ఖాళీ
	};
	స్థిరరాశి అక్షరం *సందేశం_2 = ఖాళీ;
	గుర్తులేని flags = 0;
	నిర్మాణం option opts[] = {
		OPT_STRING(0, "message", &సందేశం_2, N_("reason"),
			   N_("reason of the update")),
		OPT_బిట్టు(0 ,"no-deref", &flags,
			N_("update <refname> not the one it points to"),
			REF_NO_DEREF),
		OPT_చివర(),
	};
	నిర్మాణం object_id oldoid;
	స్థిరరాశి అక్షరం *refname;
	పూర్ణాంకం ret;

	argc = parse_options(argc, argv, ఉపసర్గ, opts, refs_delete_usage, 0);
	ఒకవేళ (argc < 1 || argc > 2)
		usage(_("delete requires reference name and an optional old object ID"));

	ఒకవేళ (సందేశం_2 && !*సందేశం_2)
		die(_("refusing to perform update with empty message"));

	repo_config(repo, git_default_config, ఖాళీ);

	refname = argv[0];
	ఒకవేళ (argc == 2) {
		ఒకవేళ (repo_get_oid_with_flags(repo, argv[1], &oldoid, GET_OID_SKIP_AMBIGUITY_CHECK))
			die(_("invalid old object ID: '%s'"), argv[1]);
		ఒకవేళ (is_null_oid(&oldoid))
			die(_("cannot delete reference with null old object ID"));
	}

	ret = refs_delete_ref(get_main_ref_store(repo), సందేశం_2, refname,
			      argc == 2 ? &oldoid : ఖాళీ, flags);

	ఒకవేళ (ret < 0)
		ret = 1;
	తిరిగి ret;
}

స్థిర పూర్ణాంకం cmd_refs_update(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
			   నిర్మాణం repository *repo)
{
	స్థిర అక్షరం స్థిరరాశి * స్థిరరాశి refs_update_usage[] = {
		REFS_UPDATE_USAGE,
		ఖాళీ
	};
	స్థిరరాశి అక్షరం *సందేశం_2 = ఖాళీ;
	గుర్తులేని flags = 0;
	నిర్మాణం option opts[] = {
		OPT_STRING(0, "message", &సందేశం_2, N_("reason"),
			   N_("reason of the update")),
		OPT_బిట్టు(0 ,"no-deref", &flags,
			N_("update <refname> not the one it points to"),
			REF_NO_DEREF),
		OPT_బిట్టు(0, "create-reflog", &flags, N_("create a reflog"),
			REF_FORCE_CREATE_REFLOG),
		OPT_చివర(),
	};
	నిర్మాణం object_id newoid, oldoid;
	స్థిరరాశి అక్షరం *refname;
	పూర్ణాంకం ret;

	argc = parse_options(argc, argv, ఉపసర్గ, opts, refs_update_usage, 0);
	ఒకవేళ (argc < 2 || argc > 3)
		usage(_("update requires reference name, new value and an optional old value"));

	ఒకవేళ (సందేశం_2 && !*సందేశం_2)
		die(_("refusing to perform update with empty message"));

	repo_config(repo, git_default_config, ఖాళీ);

	refname = argv[0];
	ఒకవేళ (repo_get_oid_with_flags(repo, argv[1], &newoid,
				    GET_OID_SKIP_AMBIGUITY_CHECK))
		die(_("invalid new object ID: '%s'"), argv[1]);
	ఒకవేళ (argc == 3 &&
	    repo_get_oid_with_flags(repo, argv[2], &oldoid,
				    GET_OID_SKIP_AMBIGUITY_CHECK))
		die(_("invalid old object ID: '%s'"), argv[2]);

	ret = refs_update_ref(get_main_ref_store(repo), సందేశం_2, refname,
			      &newoid, argc == 3 ? &oldoid : ఖాళీ, flags,
			      UPDATE_REFS_సందేశం_ON_ERR);

	ఒకవేళ (ret < 0)
		ret = 1;
	తిరిగి ret;
}

స్థిర పూర్ణాంకం cmd_refs_rename(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
			   నిర్మాణం repository *repo)
{
	స్థిర అక్షరం స్థిరరాశి * స్థిరరాశి refs_rename_usage[] = {
		REFS_RENAME_USAGE,
		ఖాళీ
	};
	స్థిరరాశి అక్షరం *సందేశం_2 = ఖాళీ;
	నిర్మాణం option opts[] = {
		OPT_STRING(0, "message", &సందేశం_2, N_("reason"),
			   N_("reason of the update")),
		OPT_చివర(),
	};
	స్థిరరాశి అక్షరం *oldref, *newref;
	పూర్ణాంకం ret;

	argc = parse_options(argc, argv, ఉపసర్గ, opts, refs_rename_usage, 0);
	ఒకవేళ (argc != 2)
		usage(_("rename requires old and new reference name"));
	ఒకవేళ (సందేశం_2 && !*సందేశం_2)
		die(_("refusing to perform update with empty message"));

	repo_config(repo, git_default_config, ఖాళీ);

	oldref = argv[0];
	newref = argv[1];

	ఒకవేళ (check_refname_format(oldref, 0))
		die(_("invalid ref format: '%s'"), oldref);
	ఒకవేళ (check_refname_format(newref, 0))
		die(_("invalid ref format: '%s'"), newref);

	ఒకవేళ (!refs_ref_exists(get_main_ref_store(repo), oldref))
		die(_("reference does not exist: '%s'"), oldref);
	ఒకవేళ (refs_ref_exists(get_main_ref_store(repo), newref))
		die(_("reference already exists: '%s'"), newref);

	ret = refs_rename_ref(get_main_ref_store(repo), oldref, newref, సందేశం_2);

	ఒకవేళ (ret < 0)
		ret = 1;
	తిరిగి ret;
}

పూర్ణాంకం cmd_refs(పూర్ణాంకం argc,
	     స్థిరరాశి అక్షరం **argv,
	     స్థిరరాశి అక్షరం *ఉపసర్గ,
	     నిర్మాణం repository *repo)
{
	స్థిరరాశి అక్షరం * స్థిరరాశి refs_usage[] = {
		REFS_MIGRATE_USAGE,
		REFS_VERIFY_USAGE,
		REFS_OPTIMIZE_USAGE,
		"git refs list " COMMON_USAGE_FOR_EACH_REF,
		REFS_EXISTS_USAGE,
		REFS_CREATE_USAGE,
		REFS_DELETE_USAGE,
		REFS_UPDATE_USAGE,
		REFS_RENAME_USAGE,
		ఖాళీ,
	};
	parse_opt_subcommand_fn *fn = ఖాళీ;
	నిర్మాణం option opts[] = {
		OPT_సమూహం(N_("Reference database")),
		OPT_SUBCOMMAND_F("migrate", &fn, cmd_refs_migrate,
				 N_("migrate the reference database to a different format"), 0),
		OPT_SUBCOMMAND_F("verify", &fn, cmd_refs_verify,
				 N_("verify the consistency of the reference database"), 0),
		OPT_SUBCOMMAND_F("optimize", &fn, cmd_refs_optimize,
				 N_("optimize the reference database"), 0),
		OPT_సమూహం(N_("Reading references")),
		OPT_SUBCOMMAND_F("list", &fn, cmd_refs_list,
				 N_("list references"), 0),
		OPT_SUBCOMMAND_F("exists", &fn, cmd_refs_exists,
				 N_("check whether a reference exists"), 0),
		OPT_సమూహం(N_("Writing references")),
		OPT_SUBCOMMAND_F("create", &fn, cmd_refs_create,
				 N_("create a new reference"), 0),
		OPT_SUBCOMMAND_F("delete", &fn, cmd_refs_delete,
				 N_("delete a reference"), 0),
		OPT_SUBCOMMAND_F("update", &fn, cmd_refs_update,
				 N_("update an existing reference"), 0),
		OPT_SUBCOMMAND_F("rename", &fn, cmd_refs_rename,
				 N_("rename a reference"), 0),
		OPT_చివర(),
	};

	argc = parse_options(argc, argv, ఉపసర్గ, opts, refs_usage, 0);
	తిరిగి fn(argc, argv, ఉపసర్గ, repo);
}
