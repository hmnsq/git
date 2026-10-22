#define USE_THE_REPOSITORY_VARIABLE

#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "hex.h"
#অন্তর্ভাব "odb.h"
#অন্তর্ভাব "revision.h"
#অন্তর্ভাব "reachable.h"
#অন্তর্ভাব "wildmatch.h"
#অন্তর্ভাব "worktree.h"
#অন্তর্ভাব "reflog.h"
#অন্তর্ভাব "refs.h"
#অন্তর্ভাব "parse-options.h"

#define BUILTIN_REFLOG_SHOW_USAGE \
	N_("git reflog [show] [<log-options>] [<ref>]")

#define BUILTIN_REFLOG_LIST_USAGE \
	N_("git reflog list")

#define BUILTIN_REFLOG_EXISTS_USAGE \
	N_("git reflog exists <ref>")

#define BUILTIN_REFLOG_WRITE_USAGE \
	N_("git reflog write <ref> <old-oid> <new-oid> <message>")

#define BUILTIN_REFLOG_DELETE_USAGE \
	N_("git reflog delete [--rewrite] [--updateref]\n" \
	   "                  [--dry-run | -n] [--verbose] <ref>@{<specifier>}...")

#define BUILTIN_REFLOG_DROP_USAGE \
	N_("git reflog drop [--all [--single-worktree] | <refs>...]")

#define BUILTIN_REFLOG_EXPIRE_USAGE \
	N_("git reflog expire [--expire=<time>] [--expire-unreachable=<time>]\n" \
	   "                  [--rewrite] [--updateref] [--stale-fix]\n" \
	   "                  [--dry-run | -n] [--verbose] [--all [--single-worktree] | <refs>...]")

স্থির ধ্রুবক অক্ষর *ধ্রুবক reflog_show_usage[] = {
	BUILTIN_REFLOG_SHOW_USAGE,
	ফাঁকা,
};

স্থির ধ্রুবক অক্ষর *ধ্রুবক reflog_list_usage[] = {
	BUILTIN_REFLOG_LIST_USAGE,
	ফাঁকা,
};

স্থির ধ্রুবক অক্ষর *ধ্রুবক reflog_exists_usage[] = {
	BUILTIN_REFLOG_EXISTS_USAGE,
	ফাঁকা,
};

স্থির ধ্রুবক অক্ষর *ধ্রুবক reflog_write_usage[] = {
	BUILTIN_REFLOG_WRITE_USAGE,
	ফাঁকা,
};

স্থির ধ্রুবক অক্ষর *ধ্রুবক reflog_delete_usage[] = {
	BUILTIN_REFLOG_DELETE_USAGE,
	ফাঁকা
};

স্থির ধ্রুবক অক্ষর *ধ্রুবক reflog_drop_usage[] = {
	BUILTIN_REFLOG_DROP_USAGE,
	ফাঁকা,
};

স্থির ধ্রুবক অক্ষর *ধ্রুবক reflog_expire_usage[] = {
	BUILTIN_REFLOG_EXPIRE_USAGE,
	ফাঁকা
};

স্থির ধ্রুবক অক্ষর *ধ্রুবক reflog_usage[] = {
	BUILTIN_REFLOG_SHOW_USAGE,
	BUILTIN_REFLOG_LIST_USAGE,
	BUILTIN_REFLOG_EXISTS_USAGE,
	BUILTIN_REFLOG_WRITE_USAGE,
	BUILTIN_REFLOG_DELETE_USAGE,
	BUILTIN_REFLOG_DROP_USAGE,
	BUILTIN_REFLOG_EXPIRE_USAGE,
	ফাঁকা
};

কাঠামো worktree_reflogs {
	কাঠামো worktree *worktree;
	কাঠামো string_list reflogs;
};

স্থির পূর্ণসংখ্যা collect_reflog(ধ্রুবক অক্ষর *ref, শূন্য *cb_তথ্য)
{
	কাঠামো worktree_reflogs *cb = cb_তথ্য;
	কাঠামো worktree *worktree = cb->worktree;
	কাঠামো strbuf newref = STRBUF_INIT;

	/*
	 * Avoid collecting the same shared ref multiple times because
	 * they are available via all worktrees.
	 */
	যদি (!worktree->is_current &&
	    parse_worktree_ref(ref, ফাঁকা, ফাঁকা, ফাঁকা) == REF_WORKTREE_SHARED)
		ফেরত 0;

	strbuf_worktree_ref(worktree, &newref, ref);
	string_list_append_nodup(&cb->reflogs, strbuf_detach(&newref, ফাঁকা));

	ফেরত 0;
}

স্থির পূর্ণসংখ্যা expire_unreachable_callback(ধ্রুবক কাঠামো option *opt,
				 ধ্রুবক অক্ষর *arg,
				 পূর্ণসংখ্যা unset)
{
	কাঠামো reflog_expire_options *opts = opt->মান;

	BUG_ON_OPT_NEG(unset);

	যদি (parse_expiry_date(arg, &opts->expire_unreachable))
		die(_("invalid timestamp '%s' given to '--%s'"),
		    arg, opt->long_name);

	opts->explicit_expiry |= REFLOG_EXPIRE_UNREACH;
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা expire_যোগ_callback(ধ্রুবক কাঠামো option *opt,
				 ধ্রুবক অক্ষর *arg,
				 পূর্ণসংখ্যা unset)
{
	কাঠামো reflog_expire_options *opts = opt->মান;

	BUG_ON_OPT_NEG(unset);

	যদি (parse_expiry_date(arg, &opts->expire_যোগ))
		die(_("invalid timestamp '%s' given to '--%s'"),
		    arg, opt->long_name);

	opts->explicit_expiry |= REFLOG_EXPIRE_যোগ;
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা cmd_reflog_show(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix,
			   কাঠামো repository *repo UNUSED)
{
	কাঠামো option options[] = {
		OPT_END()
	};

	parse_options(argc, argv, prefix, options, reflog_show_usage,
		      PARSE_OPT_KEEP_DASHDASH | PARSE_OPT_KEEP_ARGV0 |
		      PARSE_OPT_KEEP_UNKNOWN_OPT);

	ফেরত cmd_log_reflog(argc, argv, prefix, the_repository);
}

স্থির পূর্ণসংখ্যা show_reflog(ধ্রুবক অক্ষর *refname, শূন্য *cb_তথ্য UNUSED)
{
	printf("%s\n", refname);
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা cmd_reflog_list(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix,
			   কাঠামো repository *repo UNUSED)
{
	কাঠামো option options[] = {
		OPT_END()
	};
	কাঠামো ref_store *ref_store;

	argc = parse_options(argc, argv, prefix, options, reflog_list_usage, 0);
	যদি (argc)
		ফেরত error(_("%s does not accept arguments: '%s'"),
			     "list", argv[0]);

	ref_store = get_main_ref_store(the_repository);

	ফেরত refs_for_each_reflog(ref_store, show_reflog, ফাঁকা);
}

স্থির পূর্ণসংখ্যা cmd_reflog_expire(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix,
			     কাঠামো repository *repo UNUSED)
{
	timestamp_t now = time(ফাঁকা);
	কাঠামো reflog_expire_options opts = REFLOG_EXPIRE_OPTIONS_INIT(now);
	পূর্ণসংখ্যা i, status, do_all, single_worktree = 0;
	unsigned পূর্ণসংখ্যা flags = 0;
	পূর্ণসংখ্যা verbose = 0;
	reflog_expiry_should_prune_fn *should_prune_fn = should_expire_reflog_ent;
	ধ্রুবক কাঠামো option options[] = {
		OPT_BIT('n', "dry-run", &flags, N_("do not actually prune any entries"),
			EXPIRE_REFLOGS_DRY_RUN),
		OPT_BIT(0, "rewrite", &flags,
			N_("rewrite the old SHA1 with the new SHA1 of the entry that now precedes it"),
			EXPIRE_REFLOGS_REWRITE),
		OPT_BIT(0, "updateref", &flags,
			N_("update the reference to the value of the top reflog entry"),
			EXPIRE_REFLOGS_UPDATE_REF),
		OPT_BOOL(0, "verbose", &verbose, N_("print extra information on screen")),
		OPT_CALLBACK_F(0, "expire", &opts, N_("timestamp"),
			       N_("prune entries older than the specified time"),
			       PARSE_OPT_NONEG,
			       expire_যোগ_callback),
		OPT_CALLBACK_F(0, "expire-unreachable", &opts, N_("timestamp"),
			       N_("prune entries older than <time> that are not reachable from the current tip of the branch"),
			       PARSE_OPT_NONEG,
			       expire_unreachable_callback),
		OPT_BOOL(0, "stale-fix", &opts.stalefix,
			 N_("prune any reflog entries that point to broken commits")),
		OPT_BOOL(0, "all", &do_all, N_("process the reflogs of all references")),
		OPT_BOOL(0, "single-worktree", &single_worktree,
			 N_("limits processing to reflogs from the current worktree only")),
		OPT_END()
	};

	repo_config(the_repository, reflog_expire_config, &opts);

	save_commit_বাফার = 0;
	do_all = status = 0;

	argc = parse_options(argc, argv, prefix, options, reflog_expire_usage, 0);

	যদি (verbose)
		should_prune_fn = should_expire_reflog_ent_verbose;

	/*
	 * We can trust the commits and objects reachable from refs
	 * even in older repository.  We cannot trust what's reachable
	 * from reflog if the repository was pruned with older git.
	 */
	যদি (opts.stalefix) {
		কাঠামো rev_info revs;

		repo_init_revisions(the_repository, &revs, prefix);
		revs.do_not_die_on_missing_objects = 1;
		revs.ignore_missing = 1;
		revs.ignore_missing_links = 1;
		যদি (verbose)
			printf(_("Marking reachable objects..."));
		mark_reachable_objects(&revs, 0, 0, ফাঁকা);
		release_revisions(&revs);
		যদি (verbose)
			putchar('\n');
	}

	যদি (do_all) {
		কাঠামো worktree_reflogs collected = {
			.reflogs = STRING_LIST_INIT_DUP,
		};
		কাঠামো string_list_item *item;
		কাঠামো worktree **worktrees, **p;

		worktrees = get_worktrees(the_repository);
		জন্য (p = worktrees; *p; p++) {
			যদি (single_worktree && !(*p)->is_current)
				চালু;
			collected.worktree = *p;
			refs_for_each_reflog(get_worktree_ref_store(*p),
					     collect_reflog, &collected);
		}
		free_worktrees(worktrees);

		for_each_string_list_item(item, &collected.reflogs) {
			কাঠামো expire_reflog_policy_cb cb = {
				.opts = opts,
				.dry_run = !!(flags & EXPIRE_REFLOGS_DRY_RUN),
			};

			reflog_expire_options_set_refname(&cb.opts,  item->string);
			status |= refs_reflog_expire(get_main_ref_store(the_repository),
						     item->string, flags,
						     reflog_expiry_prepare,
						     should_prune_fn,
						     reflog_expiry_cleanup,
						     &cb);
		}
		string_list_clear(&collected.reflogs, 0);
	}

	জন্য (i = 0; i < argc; i++) {
		অক্ষর *ref;
		কাঠামো expire_reflog_policy_cb cb = { .opts = opts };

		যদি (!repo_dwim_log(the_repository, argv[i], strlen(argv[i]), ফাঁকা, &ref)) {
			status |= error(_("reflog could not be found: '%s'"), argv[i]);
			চালু;
		}
		reflog_expire_options_set_refname(&cb.opts, ref);
		status |= refs_reflog_expire(get_main_ref_store(the_repository),
					     ref, flags,
					     reflog_expiry_prepare,
					     should_prune_fn,
					     reflog_expiry_cleanup,
					     &cb);
		মুক্ত(ref);
	}

	reflog_clear_expire_config(&opts);

	ফেরত status;
}

স্থির পূর্ণসংখ্যা cmd_reflog_delete(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix,
			     কাঠামো repository *repo UNUSED)
{
	পূর্ণসংখ্যা i, status = 0;
	unsigned পূর্ণসংখ্যা flags = 0;
	পূর্ণসংখ্যা verbose = 0;

	ধ্রুবক কাঠামো option options[] = {
		OPT_BIT('n', "dry-run", &flags, N_("do not actually prune any entries"),
			EXPIRE_REFLOGS_DRY_RUN),
		OPT_BIT(0, "rewrite", &flags,
			N_("rewrite the old SHA1 with the new SHA1 of the entry that now precedes it"),
			EXPIRE_REFLOGS_REWRITE),
		OPT_BIT(0, "updateref", &flags,
			N_("update the reference to the value of the top reflog entry"),
			EXPIRE_REFLOGS_UPDATE_REF),
		OPT_BOOL(0, "verbose", &verbose, N_("print extra information on screen")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, reflog_delete_usage, 0);

	যদি (argc < 1)
		ফেরত error(_("no reflog specified to delete"));

	জন্য (i = 0; i < argc; i++)
		status |= reflog_delete(argv[i], flags, verbose);

	ফেরত status;
}

স্থির পূর্ণসংখ্যা cmd_reflog_exists(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix,
			     কাঠামো repository *repo UNUSED)
{
	কাঠামো option options[] = {
		OPT_END()
	};
	ধ্রুবক অক্ষর *refname;

	argc = parse_options(argc, argv, prefix, options, reflog_exists_usage,
			     0);
	যদি (!argc)
		usage_with_options(reflog_exists_usage, options);

	refname = argv[0];
	যদি (check_refname_format(refname, REFNAME_ALLOW_ONELEVEL))
		die(_("invalid ref format: %s"), refname);
	ফেরত !refs_reflog_exists(get_main_ref_store(the_repository),
				   refname);
}

স্থির পূর্ণসংখ্যা cmd_reflog_drop(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix,
			   কাঠামো repository *repo)
{
	পূর্ণসংখ্যা ret = 0, do_all = 0, single_worktree = 0;
	ধ্রুবক কাঠামো option options[] = {
		OPT_BOOL(0, "all", &do_all, N_("drop the reflogs of all references")),
		OPT_BOOL(0, "single-worktree", &single_worktree,
			 N_("drop reflogs from the current worktree only")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, reflog_drop_usage, 0);

	যদি (argc && do_all)
		usage(_("references specified along with --all"));

	যদি (do_all) {
		কাঠামো worktree_reflogs collected = {
			.reflogs = STRING_LIST_INIT_DUP,
		};
		কাঠামো string_list_item *item;
		কাঠামো worktree **worktrees, **p;

		worktrees = get_worktrees(the_repository);
		জন্য (p = worktrees; *p; p++) {
			যদি (single_worktree && !(*p)->is_current)
				চালু;
			collected.worktree = *p;
			refs_for_each_reflog(get_worktree_ref_store(*p),
					     collect_reflog, &collected);
		}
		free_worktrees(worktrees);

		for_each_string_list_item(item, &collected.reflogs)
			ret |= refs_delete_reflog(get_main_ref_store(repo),
						     item->string);
		string_list_clear(&collected.reflogs, 0);

		ফেরত ret;
	}

	জন্য (পূর্ণসংখ্যা i = 0; i < argc; i++) {
		অক্ষর *ref;
		যদি (!repo_dwim_log(repo, argv[i], strlen(argv[i]), ফাঁকা, &ref)) {
			ret |= error(_("reflog could not be found: '%s'"), argv[i]);
			চালু;
		}

		ret |= refs_delete_reflog(get_main_ref_store(repo), ref);
		মুক্ত(ref);
	}

	ফেরত ret;
}

স্থির পূর্ণসংখ্যা cmd_reflog_write(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix,
			    কাঠামো repository *repo)
{
	ধ্রুবক কাঠামো option options[] = {
		OPT_END()
	};
	কাঠামো object_id old_oid, new_oid;
	কাঠামো strbuf err = STRBUF_INIT;
	কাঠামো ref_transaction *tx;
	ধ্রুবক অক্ষর *ref, *message;
	পূর্ণসংখ্যা ret;

	repo_config(repo, git_ident_config, ফাঁকা);

	argc = parse_options(argc, argv, prefix, options, reflog_write_usage, 0);
	যদি (argc != 4)
		usage_with_options(reflog_write_usage, options);

	ref = argv[0];
	যদি (!is_মূল_ref(ref) && check_refname_format(ref, 0))
		die(_("invalid reference name: %s"), ref);

	ret = get_oid_hex_algop(argv[1], &old_oid, repo->hash_algo);
	যদি (ret)
		die(_("invalid old object ID: '%s'"), argv[1]);
	যদি (!is_null_oid(&old_oid) && !odb_has_object(repo->objects, &old_oid, 0))
		die(_("old object '%s' does not exist"), argv[1]);

	ret = get_oid_hex_algop(argv[2], &new_oid, repo->hash_algo);
	যদি (ret)
		die(_("invalid new object ID: '%s'"), argv[2]);
	যদি (!is_null_oid(&new_oid) && !odb_has_object(repo->objects, &new_oid, 0))
		die(_("new object '%s' does not exist"), argv[2]);

	message = argv[3];

	tx = ref_store_transaction_begin(get_main_ref_store(repo), 0, &err);
	যদি (!tx)
		die(_("cannot start transaction: %s"), err.buf);

	ret = ref_transaction_update_reflog(tx, ref, &new_oid, &old_oid,
					    git_committer_info(0),
					    message, 0, &err);
	যদি (ret)
		die(_("cannot queue reflog update: %s"), err.buf);

	ret = ref_transaction_commit(tx, &err);
	যদি (ret)
		die(_("cannot commit reflog update: %s"), err.buf);

	ref_transaction_free(tx);
	strbuf_release(&err);
	ফেরত 0;
}

/*
 * main "reflog"
 */
পূর্ণসংখ্যা cmd_reflog(পূর্ণসংখ্যা argc,
	       ধ্রুবক অক্ষর **argv,
	       ধ্রুবক অক্ষর *prefix,
	       কাঠামো repository *repository)
{
	parse_opt_subcommand_fn *fn = ফাঁকা;
	কাঠামো option options[] = {
		OPT_SUBCOMMAND("show", &fn, cmd_reflog_show),
		OPT_SUBCOMMAND("list", &fn, cmd_reflog_list),
		OPT_SUBCOMMAND("exists", &fn, cmd_reflog_exists),
		OPT_SUBCOMMAND("write", &fn, cmd_reflog_write),
		OPT_SUBCOMMAND("delete", &fn, cmd_reflog_delete),
		OPT_SUBCOMMAND("drop", &fn, cmd_reflog_drop),
		OPT_SUBCOMMAND("expire", &fn, cmd_reflog_expire),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, reflog_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL |
			     PARSE_OPT_KEEP_DASHDASH | PARSE_OPT_KEEP_ARGV0 |
			     PARSE_OPT_KEEP_UNKNOWN_OPT);
	যদি (fn)
		ফেরত fn(argc - 1, argv + 1, prefix, repository);
	অন্যথা
		ফেরত cmd_log_reflog(argc, argv, prefix, repository);
}
