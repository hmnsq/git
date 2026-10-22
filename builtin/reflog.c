#define USE_THE_REPOSITORY_VARIABLE

#شامل "builtin.h"
#شامل "config.h"
#شامل "gettext.h"
#شامل "hex.h"
#شامل "odb.h"
#شامل "revision.h"
#شامل "reachable.h"
#شامل "wildmatch.h"
#شامل "worktree.h"
#شامل "reflog.h"
#شامل "refs.h"
#شامل "parse-options.h"

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

ایستا ثابت نویسه *ثابت reflog_show_usage[] = {
	BUILTIN_REFLOG_SHOW_USAGE,
	پوچ,
};

ایستا ثابت نویسه *ثابت reflog_list_usage[] = {
	BUILTIN_REFLOG_LIST_USAGE,
	پوچ,
};

ایستا ثابت نویسه *ثابت reflog_exists_usage[] = {
	BUILTIN_REFLOG_EXISTS_USAGE,
	پوچ,
};

ایستا ثابت نویسه *ثابت reflog_write_usage[] = {
	BUILTIN_REFLOG_WRITE_USAGE,
	پوچ,
};

ایستا ثابت نویسه *ثابت reflog_delete_usage[] = {
	BUILTIN_REFLOG_DELETE_USAGE,
	پوچ
};

ایستا ثابت نویسه *ثابت reflog_drop_usage[] = {
	BUILTIN_REFLOG_DROP_USAGE,
	پوچ,
};

ایستا ثابت نویسه *ثابت reflog_expire_usage[] = {
	BUILTIN_REFLOG_EXPIRE_USAGE,
	پوچ
};

ایستا ثابت نویسه *ثابت reflog_usage[] = {
	BUILTIN_REFLOG_SHOW_USAGE,
	BUILTIN_REFLOG_LIST_USAGE,
	BUILTIN_REFLOG_EXISTS_USAGE,
	BUILTIN_REFLOG_WRITE_USAGE,
	BUILTIN_REFLOG_DELETE_USAGE,
	BUILTIN_REFLOG_DROP_USAGE,
	BUILTIN_REFLOG_EXPIRE_USAGE,
	پوچ
};

ساختار worktree_reflogs {
	ساختار worktree *worktree;
	ساختار string_list reflogs;
};

ایستا صحیح collect_reflog(ثابت نویسه *ref, تهی *cb_data)
{
	ساختار worktree_reflogs *cb = cb_data;
	ساختار worktree *worktree = cb->worktree;
	ساختار strbuf newref = STRBUF_INIT;

	/*
	 * Avoid collecting the same shared ref multiple times because
	 * they are available via all worktrees.
	 */
	اگر (!worktree->is_current &&
	    parse_worktree_ref(ref, پوچ, پوچ, پوچ) == REF_WORKTREE_SHARED)
		بازگشت 0;

	strbuf_worktree_ref(worktree, &newref, ref);
	string_list_append_nodup(&cb->reflogs, strbuf_detach(&newref, پوچ));

	بازگشت 0;
}

ایستا صحیح expire_unreachable_callback(ثابت ساختار option *opt,
				 ثابت نویسه *arg,
				 صحیح unset)
{
	ساختار reflog_expire_options *opts = opt->مقدار;

	BUG_ON_OPT_NEG(unset);

	اگر (parse_expiry_date(arg, &opts->expire_unreachable))
		die(_("invalid timestamp '%s' given to '--%s'"),
		    arg, opt->long_name);

	opts->explicit_expiry |= REFLOG_EXPIRE_UNREACH;
	بازگشت 0;
}

ایستا صحیح expire_total_callback(ثابت ساختار option *opt,
				 ثابت نویسه *arg,
				 صحیح unset)
{
	ساختار reflog_expire_options *opts = opt->مقدار;

	BUG_ON_OPT_NEG(unset);

	اگر (parse_expiry_date(arg, &opts->expire_total))
		die(_("invalid timestamp '%s' given to '--%s'"),
		    arg, opt->long_name);

	opts->explicit_expiry |= REFLOG_EXPIRE_TOTAL;
	بازگشت 0;
}

ایستا صحیح cmd_reflog_show(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
			   ساختار repository *repo UNUSED)
{
	ساختار option options[] = {
		OPT_END()
	};

	parse_options(argc, argv, prefix, options, reflog_show_usage,
		      PARSE_OPT_KEEP_DASHDASH | PARSE_OPT_KEEP_ARGV0 |
		      PARSE_OPT_KEEP_UNKNOWN_OPT);

	بازگشت cmd_log_reflog(argc, argv, prefix, the_repository);
}

ایستا صحیح show_reflog(ثابت نویسه *refname, تهی *cb_data UNUSED)
{
	printf("%s\n", refname);
	بازگشت 0;
}

ایستا صحیح cmd_reflog_list(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
			   ساختار repository *repo UNUSED)
{
	ساختار option options[] = {
		OPT_END()
	};
	ساختار ref_store *ref_store;

	argc = parse_options(argc, argv, prefix, options, reflog_list_usage, 0);
	اگر (argc)
		بازگشت error(_("%s does not accept arguments: '%s'"),
			     "list", argv[0]);

	ref_store = get_main_ref_store(the_repository);

	بازگشت refs_for_each_reflog(ref_store, show_reflog, پوچ);
}

ایستا صحیح cmd_reflog_expire(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
			     ساختار repository *repo UNUSED)
{
	timestamp_t now = time(پوچ);
	ساختار reflog_expire_options opts = REFLOG_EXPIRE_OPTIONS_INIT(now);
	صحیح i, status, do_all, single_worktree = 0;
	unsigned صحیح flags = 0;
	صحیح verbose = 0;
	reflog_expiry_should_prune_fn *should_prune_fn = should_expire_reflog_ent;
	ثابت ساختار option options[] = {
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

	اگر (verbose)
		should_prune_fn = should_expire_reflog_ent_verbose;

	/*
	 * We can trust the commits and objects reachable from refs
	 * even in older repository.  We cannot trust what's reachable
	 * from reflog if the repository was pruned with older git.
	 */
	اگر (opts.stalefix) {
		ساختار rev_info revs;

		repo_init_revisions(the_repository, &revs, prefix);
		revs.do_not_die_on_missing_objects = 1;
		revs.ignore_missing = 1;
		revs.ignore_missing_links = 1;
		اگر (verbose)
			printf(_("Marking reachable objects..."));
		mark_reachable_objects(&revs, 0, 0, پوچ);
		release_revisions(&revs);
		اگر (verbose)
			putchar('\n');
	}

	اگر (do_all) {
		ساختار worktree_reflogs collected = {
			.reflogs = STRING_LIST_INIT_DUP,
		};
		ساختار string_list_item *item;
		ساختار worktree **worktrees, **p;

		worktrees = get_worktrees(the_repository);
		برای (p = worktrees; *p; p++) {
			اگر (single_worktree && !(*p)->is_current)
				ادامه;
			collected.worktree = *p;
			refs_for_each_reflog(get_worktree_ref_store(*p),
					     collect_reflog, &collected);
		}
		free_worktrees(worktrees);

		for_each_string_list_item(item, &collected.reflogs) {
			ساختار expire_reflog_policy_cb cb = {
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

	برای (i = 0; i < argc; i++) {
		نویسه *ref;
		ساختار expire_reflog_policy_cb cb = { .opts = opts };

		اگر (!repo_dwim_log(the_repository, argv[i], strlen(argv[i]), پوچ, &ref)) {
			status |= error(_("reflog could not be found: '%s'"), argv[i]);
			ادامه;
		}
		reflog_expire_options_set_refname(&cb.opts, ref);
		status |= refs_reflog_expire(get_main_ref_store(the_repository),
					     ref, flags,
					     reflog_expiry_prepare,
					     should_prune_fn,
					     reflog_expiry_cleanup,
					     &cb);
		آزاد(ref);
	}

	reflog_clear_expire_config(&opts);

	بازگشت status;
}

ایستا صحیح cmd_reflog_delete(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
			     ساختار repository *repo UNUSED)
{
	صحیح i, status = 0;
	unsigned صحیح flags = 0;
	صحیح verbose = 0;

	ثابت ساختار option options[] = {
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

	اگر (argc < 1)
		بازگشت error(_("no reflog specified to delete"));

	برای (i = 0; i < argc; i++)
		status |= reflog_delete(argv[i], flags, verbose);

	بازگشت status;
}

ایستا صحیح cmd_reflog_exists(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
			     ساختار repository *repo UNUSED)
{
	ساختار option options[] = {
		OPT_END()
	};
	ثابت نویسه *refname;

	argc = parse_options(argc, argv, prefix, options, reflog_exists_usage,
			     0);
	اگر (!argc)
		usage_with_options(reflog_exists_usage, options);

	refname = argv[0];
	اگر (check_refname_format(refname, REFNAME_ALLOW_ONELEVEL))
		die(_("invalid ref format: %s"), refname);
	بازگشت !refs_reflog_exists(get_main_ref_store(the_repository),
				   refname);
}

ایستا صحیح cmd_reflog_drop(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
			   ساختار repository *repo)
{
	صحیح ret = 0, do_all = 0, single_worktree = 0;
	ثابت ساختار option options[] = {
		OPT_BOOL(0, "all", &do_all, N_("drop the reflogs of all references")),
		OPT_BOOL(0, "single-worktree", &single_worktree,
			 N_("drop reflogs from the current worktree only")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, reflog_drop_usage, 0);

	اگر (argc && do_all)
		usage(_("references specified along with --all"));

	اگر (do_all) {
		ساختار worktree_reflogs collected = {
			.reflogs = STRING_LIST_INIT_DUP,
		};
		ساختار string_list_item *item;
		ساختار worktree **worktrees, **p;

		worktrees = get_worktrees(the_repository);
		برای (p = worktrees; *p; p++) {
			اگر (single_worktree && !(*p)->is_current)
				ادامه;
			collected.worktree = *p;
			refs_for_each_reflog(get_worktree_ref_store(*p),
					     collect_reflog, &collected);
		}
		free_worktrees(worktrees);

		for_each_string_list_item(item, &collected.reflogs)
			ret |= refs_delete_reflog(get_main_ref_store(repo),
						     item->string);
		string_list_clear(&collected.reflogs, 0);

		بازگشت ret;
	}

	برای (صحیح i = 0; i < argc; i++) {
		نویسه *ref;
		اگر (!repo_dwim_log(repo, argv[i], strlen(argv[i]), پوچ, &ref)) {
			ret |= error(_("reflog could not be found: '%s'"), argv[i]);
			ادامه;
		}

		ret |= refs_delete_reflog(get_main_ref_store(repo), ref);
		آزاد(ref);
	}

	بازگشت ret;
}

ایستا صحیح cmd_reflog_write(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
			    ساختار repository *repo)
{
	ثابت ساختار option options[] = {
		OPT_END()
	};
	ساختار object_id old_oid, new_oid;
	ساختار strbuf err = STRBUF_INIT;
	ساختار ref_transaction *tx;
	ثابت نویسه *ref, *message;
	صحیح ret;

	repo_config(repo, git_ident_config, پوچ);

	argc = parse_options(argc, argv, prefix, options, reflog_write_usage, 0);
	اگر (argc != 4)
		usage_with_options(reflog_write_usage, options);

	ref = argv[0];
	اگر (!is_ریشه_ref(ref) && check_refname_format(ref, 0))
		die(_("invalid reference name: %s"), ref);

	ret = get_oid_hex_algop(argv[1], &old_oid, repo->hash_algo);
	اگر (ret)
		die(_("invalid old object ID: '%s'"), argv[1]);
	اگر (!is_null_oid(&old_oid) && !odb_has_object(repo->objects, &old_oid, 0))
		die(_("old object '%s' does not exist"), argv[1]);

	ret = get_oid_hex_algop(argv[2], &new_oid, repo->hash_algo);
	اگر (ret)
		die(_("invalid new object ID: '%s'"), argv[2]);
	اگر (!is_null_oid(&new_oid) && !odb_has_object(repo->objects, &new_oid, 0))
		die(_("new object '%s' does not exist"), argv[2]);

	message = argv[3];

	tx = ref_store_transaction_begin(get_main_ref_store(repo), 0, &err);
	اگر (!tx)
		die(_("cannot start transaction: %s"), err.buf);

	ret = ref_transaction_update_reflog(tx, ref, &new_oid, &old_oid,
					    git_committer_info(0),
					    message, 0, &err);
	اگر (ret)
		die(_("cannot queue reflog update: %s"), err.buf);

	ret = ref_transaction_commit(tx, &err);
	اگر (ret)
		die(_("cannot commit reflog update: %s"), err.buf);

	ref_transaction_free(tx);
	strbuf_release(&err);
	بازگشت 0;
}

/*
 * main "reflog"
 */
صحیح cmd_reflog(صحیح argc,
	       ثابت نویسه **argv,
	       ثابت نویسه *prefix,
	       ساختار repository *repository)
{
	parse_opt_subcommand_fn *fn = پوچ;
	ساختار option options[] = {
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
	اگر (fn)
		بازگشت fn(argc - 1, argv + 1, prefix, repository);
	وگرنه
		بازگشت cmd_log_reflog(argc, argv, prefix, repository);
}
