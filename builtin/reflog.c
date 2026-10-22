#define USE_THE_REPOSITORY_VARIABLE

#অন্তর্ভাৱ "builtin.h"
#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ "gettext.h"
#অন্তর্ভাৱ "hex.h"
#অন্তর্ভাৱ "odb.h"
#অন্তর্ভাৱ "revision.h"
#অন্তর্ভাৱ "reachable.h"
#অন্তর্ভাৱ "wildmatch.h"
#অন্তর্ভাৱ "worktree.h"
#অন্তর্ভাৱ "reflog.h"
#অন্তর্ভাৱ "refs.h"
#অন্তর্ভাৱ "parse-options.h"

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

স্থির ধ্রুৱ ৱর্ণ *ধ্রুৱ reflog_show_usage[] = {
	BUILTIN_REFLOG_SHOW_USAGE,
	রিক্ত,
};

স্থির ধ্রুৱ ৱর্ণ *ধ্রুৱ reflog_list_usage[] = {
	BUILTIN_REFLOG_LIST_USAGE,
	রিক্ত,
};

স্থির ধ্রুৱ ৱর্ণ *ধ্রুৱ reflog_exists_usage[] = {
	BUILTIN_REFLOG_EXISTS_USAGE,
	রিক্ত,
};

স্থির ধ্রুৱ ৱর্ণ *ধ্রুৱ reflog_write_usage[] = {
	BUILTIN_REFLOG_WRITE_USAGE,
	রিক্ত,
};

স্থির ধ্রুৱ ৱর্ণ *ধ্রুৱ reflog_delete_usage[] = {
	BUILTIN_REFLOG_DELETE_USAGE,
	রিক্ত
};

স্থির ধ্রুৱ ৱর্ণ *ধ্রুৱ reflog_drop_usage[] = {
	BUILTIN_REFLOG_DROP_USAGE,
	রিক্ত,
};

স্থির ধ্রুৱ ৱর্ণ *ধ্রুৱ reflog_expire_usage[] = {
	BUILTIN_REFLOG_EXPIRE_USAGE,
	রিক্ত
};

স্থির ধ্রুৱ ৱর্ণ *ধ্রুৱ reflog_usage[] = {
	BUILTIN_REFLOG_SHOW_USAGE,
	BUILTIN_REFLOG_LIST_USAGE,
	BUILTIN_REFLOG_EXISTS_USAGE,
	BUILTIN_REFLOG_WRITE_USAGE,
	BUILTIN_REFLOG_DELETE_USAGE,
	BUILTIN_REFLOG_DROP_USAGE,
	BUILTIN_REFLOG_EXPIRE_USAGE,
	রিক্ত
};

সংরচনা worktree_reflogs {
	সংরচনা worktree *worktree;
	সংরচনা string_list reflogs;
};

স্থির পূর্ণাঙ্ক collect_reflog(ধ্রুৱ ৱর্ণ *ref, শূন্য *cb_data)
{
	সংরচনা worktree_reflogs *cb = cb_data;
	সংরচনা worktree *worktree = cb->worktree;
	সংরচনা strbuf newref = STRBUF_INIT;

	/*
	 * Avoid collecting the same shared ref multiple times because
	 * they are available via all worktrees.
	 */
	যদি (!worktree->is_current &&
	    parse_worktree_ref(ref, রিক্ত, রিক্ত, রিক্ত) == REF_WORKTREE_SHARED)
		ঘূৰাই 0;

	strbuf_worktree_ref(worktree, &newref, ref);
	string_list_append_nodup(&cb->reflogs, strbuf_detach(&newref, রিক্ত));

	ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক expire_unreachable_callback(ধ্রুৱ সংরচনা option *opt,
				 ধ্রুৱ ৱর্ণ *arg,
				 পূর্ণাঙ্ক unset)
{
	সংরচনা reflog_expire_options *opts = opt->মান;

	BUG_ON_OPT_NEG(unset);

	যদি (parse_expiry_date(arg, &opts->expire_unreachable))
		die(_("invalid timestamp '%s' given to '--%s'"),
		    arg, opt->long_name);

	opts->explicit_expiry |= REFLOG_EXPIRE_UNREACH;
	ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক expire_total_callback(ধ্রুৱ সংরচনা option *opt,
				 ধ্রুৱ ৱর্ণ *arg,
				 পূর্ণাঙ্ক unset)
{
	সংরচনা reflog_expire_options *opts = opt->মান;

	BUG_ON_OPT_NEG(unset);

	যদি (parse_expiry_date(arg, &opts->expire_total))
		die(_("invalid timestamp '%s' given to '--%s'"),
		    arg, opt->long_name);

	opts->explicit_expiry |= REFLOG_EXPIRE_TOTAL;
	ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক cmd_reflog_show(পূর্ণাঙ্ক argc, ধ্রুৱ ৱর্ণ **argv, ধ্রুৱ ৱর্ণ *prefix,
			   সংরচনা repository *repo UNUSED)
{
	সংরচনা option options[] = {
		OPT_END()
	};

	parse_options(argc, argv, prefix, options, reflog_show_usage,
		      PARSE_OPT_KEEP_DASHDASH | PARSE_OPT_KEEP_ARGV0 |
		      PARSE_OPT_KEEP_UNKNOWN_OPT);

	ঘূৰাই cmd_log_reflog(argc, argv, prefix, the_repository);
}

স্থির পূর্ণাঙ্ক show_reflog(ধ্রুৱ ৱর্ণ *refname, শূন্য *cb_data UNUSED)
{
	printf("%s\n", refname);
	ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক cmd_reflog_list(পূর্ণাঙ্ক argc, ধ্রুৱ ৱর্ণ **argv, ধ্রুৱ ৱর্ণ *prefix,
			   সংরচনা repository *repo UNUSED)
{
	সংরচনা option options[] = {
		OPT_END()
	};
	সংরচনা ref_store *ref_store;

	argc = parse_options(argc, argv, prefix, options, reflog_list_usage, 0);
	যদি (argc)
		ঘূৰাই error(_("%s does not accept arguments: '%s'"),
			     "list", argv[0]);

	ref_store = get_main_ref_store(the_repository);

	ঘূৰাই refs_for_each_reflog(ref_store, show_reflog, রিক্ত);
}

স্থির পূর্ণাঙ্ক cmd_reflog_expire(পূর্ণাঙ্ক argc, ধ্রুৱ ৱর্ণ **argv, ধ্রুৱ ৱর্ণ *prefix,
			     সংরচনা repository *repo UNUSED)
{
	timestamp_t now = time(রিক্ত);
	সংরচনা reflog_expire_options opts = REFLOG_EXPIRE_OPTIONS_INIT(now);
	পূর্ণাঙ্ক i, status, do_all, single_worktree = 0;
	unsigned পূর্ণাঙ্ক flags = 0;
	পূর্ণাঙ্ক verbose = 0;
	reflog_expiry_should_prune_fn *should_prune_fn = should_expire_reflog_ent;
	ধ্রুৱ সংরচনা option options[] = {
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
			       expire_total_callback),
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

	save_commit_buffer = 0;
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
		সংরচনা rev_info revs;

		repo_init_revisions(the_repository, &revs, prefix);
		revs.do_not_die_on_missing_objects = 1;
		revs.ignore_missing = 1;
		revs.ignore_missing_links = 1;
		যদি (verbose)
			printf(_("Marking reachable objects..."));
		mark_reachable_objects(&revs, 0, 0, রিক্ত);
		release_revisions(&revs);
		যদি (verbose)
			putchar('\n');
	}

	যদি (do_all) {
		সংরচনা worktree_reflogs collected = {
			.reflogs = STRING_LIST_INIT_DUP,
		};
		সংরচনা string_list_item *item;
		সংরচনা worktree **worktrees, **p;

		worktrees = get_worktrees(the_repository);
		বাবে (p = worktrees; *p; p++) {
			যদি (single_worktree && !(*p)->is_current)
				অনুৱর্ত;
			collected.worktree = *p;
			refs_for_each_reflog(get_worktree_ref_store(*p),
					     collect_reflog, &collected);
		}
		free_worktrees(worktrees);

		for_each_string_list_item(item, &collected.reflogs) {
			সংরচনা expire_reflog_policy_cb cb = {
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

	বাবে (i = 0; i < argc; i++) {
		ৱর্ণ *ref;
		সংরচনা expire_reflog_policy_cb cb = { .opts = opts };

		যদি (!repo_dwim_log(the_repository, argv[i], strlen(argv[i]), রিক্ত, &ref)) {
			status |= error(_("reflog could not be found: '%s'"), argv[i]);
			অনুৱর্ত;
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

	ঘূৰাই status;
}

স্থির পূর্ণাঙ্ক cmd_reflog_delete(পূর্ণাঙ্ক argc, ধ্রুৱ ৱর্ণ **argv, ধ্রুৱ ৱর্ণ *prefix,
			     সংরচনা repository *repo UNUSED)
{
	পূর্ণাঙ্ক i, status = 0;
	unsigned পূর্ণাঙ্ক flags = 0;
	পূর্ণাঙ্ক verbose = 0;

	ধ্রুৱ সংরচনা option options[] = {
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
		ঘূৰাই error(_("no reflog specified to delete"));

	বাবে (i = 0; i < argc; i++)
		status |= reflog_delete(argv[i], flags, verbose);

	ঘূৰাই status;
}

স্থির পূর্ণাঙ্ক cmd_reflog_exists(পূর্ণাঙ্ক argc, ধ্রুৱ ৱর্ণ **argv, ধ্রুৱ ৱর্ণ *prefix,
			     সংরচনা repository *repo UNUSED)
{
	সংরচনা option options[] = {
		OPT_END()
	};
	ধ্রুৱ ৱর্ণ *refname;

	argc = parse_options(argc, argv, prefix, options, reflog_exists_usage,
			     0);
	যদি (!argc)
		usage_with_options(reflog_exists_usage, options);

	refname = argv[0];
	যদি (check_refname_format(refname, REFNAME_ALLOW_ONELEVEL))
		die(_("invalid ref format: %s"), refname);
	ঘূৰাই !refs_reflog_exists(get_main_ref_store(the_repository),
				   refname);
}

স্থির পূর্ণাঙ্ক cmd_reflog_drop(পূর্ণাঙ্ক argc, ধ্রুৱ ৱর্ণ **argv, ধ্রুৱ ৱর্ণ *prefix,
			   সংরচনা repository *repo)
{
	পূর্ণাঙ্ক ret = 0, do_all = 0, single_worktree = 0;
	ধ্রুৱ সংরচনা option options[] = {
		OPT_BOOL(0, "all", &do_all, N_("drop the reflogs of all references")),
		OPT_BOOL(0, "single-worktree", &single_worktree,
			 N_("drop reflogs from the current worktree only")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, reflog_drop_usage, 0);

	যদি (argc && do_all)
		usage(_("references specified along with --all"));

	যদি (do_all) {
		সংরচনা worktree_reflogs collected = {
			.reflogs = STRING_LIST_INIT_DUP,
		};
		সংরচনা string_list_item *item;
		সংরচনা worktree **worktrees, **p;

		worktrees = get_worktrees(the_repository);
		বাবে (p = worktrees; *p; p++) {
			যদি (single_worktree && !(*p)->is_current)
				অনুৱর্ত;
			collected.worktree = *p;
			refs_for_each_reflog(get_worktree_ref_store(*p),
					     collect_reflog, &collected);
		}
		free_worktrees(worktrees);

		for_each_string_list_item(item, &collected.reflogs)
			ret |= refs_delete_reflog(get_main_ref_store(repo),
						     item->string);
		string_list_clear(&collected.reflogs, 0);

		ঘূৰাই ret;
	}

	বাবে (পূর্ণাঙ্ক i = 0; i < argc; i++) {
		ৱর্ণ *ref;
		যদি (!repo_dwim_log(repo, argv[i], strlen(argv[i]), রিক্ত, &ref)) {
			ret |= error(_("reflog could not be found: '%s'"), argv[i]);
			অনুৱর্ত;
		}

		ret |= refs_delete_reflog(get_main_ref_store(repo), ref);
		মুক্ত(ref);
	}

	ঘূৰাই ret;
}

স্থির পূর্ণাঙ্ক cmd_reflog_write(পূর্ণাঙ্ক argc, ধ্রুৱ ৱর্ণ **argv, ধ্রুৱ ৱর্ণ *prefix,
			    সংরচনা repository *repo)
{
	ধ্রুৱ সংরচনা option options[] = {
		OPT_END()
	};
	সংরচনা object_id old_oid, new_oid;
	সংরচনা strbuf err = STRBUF_INIT;
	সংরচনা ref_transaction *tx;
	ধ্রুৱ ৱর্ণ *ref, *message;
	পূর্ণাঙ্ক ret;

	repo_config(repo, git_ident_config, রিক্ত);

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
	ঘূৰাই 0;
}

/*
 * main "reflog"
 */
পূর্ণাঙ্ক cmd_reflog(পূর্ণাঙ্ক argc,
	       ধ্রুৱ ৱর্ণ **argv,
	       ধ্রুৱ ৱর্ণ *prefix,
	       সংরচনা repository *repository)
{
	parse_opt_subcommand_fn *fn = রিক্ত;
	সংরচনা option options[] = {
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
		ঘূৰাই fn(argc - 1, argv + 1, prefix, repository);
	অন্যথা
		ঘূৰাই cmd_log_reflog(argc, argv, prefix, repository);
}
