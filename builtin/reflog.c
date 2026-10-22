#تعریف_کرو USE_THE_REPOSITORY_VARIABLE

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

#تعریف_کرو BUILTIN_REFLOG_SHOW_USAGE \
	N_("git reflog [show] [<log-options>] [<ref>]")

#تعریف_کرو BUILTIN_REFLOG_LIST_USAGE \
	N_("git reflog list")

#تعریف_کرو BUILTIN_REFLOG_EXISTS_USAGE \
	N_("git reflog exists <ref>")

#تعریف_کرو BUILTIN_REFLOG_WRITE_USAGE \
	N_("git reflog write <ref> <old-oid> <new-oid> <message>")

#تعریف_کرو BUILTIN_REFLOG_DELETE_USAGE \
	N_("git reflog delete [--rewrite] [--updateref]\n" \
	   "                  [--dry-run | -n] [--verbose] <ref>@{<specifier>}...")

#تعریف_کرو BUILTIN_REFLOG_DROP_USAGE \
	N_("git reflog drop [--all [--single-worktree] | <refs>...]")

#تعریف_کرو BUILTIN_REFLOG_EXPIRE_USAGE \
	N_("git reflog expire [--expire=<time>] [--expire-unreachable=<time>]\n" \
	   "                  [--rewrite] [--updateref] [--stale-fix]\n" \
	   "                  [--dry-run | -n] [--verbose] [--all [--single-worktree] | <refs>...]")

ساکن ثابت حرف *ثابت reflog_show_usage[] = {
	BUILTIN_REFLOG_SHOW_USAGE,
	تہی,
};

ساکن ثابت حرف *ثابت reflog_list_usage[] = {
	BUILTIN_REFLOG_LIST_USAGE,
	تہی,
};

ساکن ثابت حرف *ثابت reflog_exists_usage[] = {
	BUILTIN_REFLOG_EXISTS_USAGE,
	تہی,
};

ساکن ثابت حرف *ثابت reflog_write_usage[] = {
	BUILTIN_REFLOG_WRITE_USAGE,
	تہی,
};

ساکن ثابت حرف *ثابت reflog_delete_usage[] = {
	BUILTIN_REFLOG_DELETE_USAGE,
	تہی
};

ساکن ثابت حرف *ثابت reflog_drop_usage[] = {
	BUILTIN_REFLOG_DROP_USAGE,
	تہی,
};

ساکن ثابت حرف *ثابت reflog_expire_usage[] = {
	BUILTIN_REFLOG_EXPIRE_USAGE,
	تہی
};

ساکن ثابت حرف *ثابت reflog_usage[] = {
	BUILTIN_REFLOG_SHOW_USAGE,
	BUILTIN_REFLOG_LIST_USAGE,
	BUILTIN_REFLOG_EXISTS_USAGE,
	BUILTIN_REFLOG_WRITE_USAGE,
	BUILTIN_REFLOG_DELETE_USAGE,
	BUILTIN_REFLOG_DROP_USAGE,
	BUILTIN_REFLOG_EXPIRE_USAGE,
	تہی
};

ساخت worktree_reflogs {
	ساخت worktree *worktree;
	ساخت string_list reflogs;
};

ساکن صحیح collect_reflog(ثابت حرف *ref, خالی *cb_ڈیٹا)
{
	ساخت worktree_reflogs *cb = cb_ڈیٹا;
	ساخت worktree *worktree = cb->worktree;
	ساخت strbuf newref = STRBUF_INIT;

	/*
	 * Avoid collecting the same shared ref multiple times because
	 * they are available via all worktrees.
	 */
	اگر (!worktree->is_current &&
	    parse_worktree_ref(ref, تہی, تہی, تہی) == REF_WORKTREE_SHARED)
		واپس 0;

	strbuf_worktree_ref(worktree, &newref, ref);
	string_list_append_nodup(&cb->reflogs, strbuf_detach(&newref, تہی));

	واپس 0;
}

ساکن صحیح expire_unreachable_callback(ثابت ساخت option *opt,
				 ثابت حرف *arg,
				 صحیح unset)
{
	ساخت reflog_expire_options *opts = opt->قدر;

	BUG_ON_OPT_NEG(unset);

	اگر (parse_expiry_date(arg, &opts->expire_unreachable))
		die(_("invalid timestamp '%s' given to '--%s'"),
		    arg, opt->long_name);

	opts->explicit_expiry |= REFLOG_EXPIRE_UNREACH;
	واپس 0;
}

ساکن صحیح expire_کل_callback(ثابت ساخت option *opt,
				 ثابت حرف *arg,
				 صحیح unset)
{
	ساخت reflog_expire_options *opts = opt->قدر;

	BUG_ON_OPT_NEG(unset);

	اگر (parse_expiry_date(arg, &opts->expire_کل))
		die(_("invalid timestamp '%s' given to '--%s'"),
		    arg, opt->long_name);

	opts->explicit_expiry |= REFLOG_EXPIRE_کل;
	واپس 0;
}

ساکن صحیح cmd_reflog_show(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
			   ساخت repository *repo غیر_مستعمل)
{
	ساخت option options[] = {
		OPT_انتہا()
	};

	parse_options(argc, argv, سابقہ, options, reflog_show_usage,
		      PARSE_OPT_KEEP_DASHDASH | PARSE_OPT_KEEP_ARGV0 |
		      PARSE_OPT_KEEP_UNKNOWN_OPT);

	واپس cmd_log_reflog(argc, argv, سابقہ, the_repository);
}

ساکن صحیح show_reflog(ثابت حرف *refname, خالی *cb_ڈیٹا غیر_مستعمل)
{
	لکھو("%s\n", refname);
	واپس 0;
}

ساکن صحیح cmd_reflog_list(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
			   ساخت repository *repo غیر_مستعمل)
{
	ساخت option options[] = {
		OPT_انتہا()
	};
	ساخت ref_store *ref_store;

	argc = parse_options(argc, argv, سابقہ, options, reflog_list_usage, 0);
	اگر (argc)
		واپس error(_("%s does not accept arguments: '%s'"),
			     "list", argv[0]);

	ref_store = get_main_ref_store(the_repository);

	واپس refs_for_each_reflog(ref_store, show_reflog, تہی);
}

ساکن صحیح cmd_reflog_expire(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
			     ساخت repository *repo غیر_مستعمل)
{
	timestamp_t now = وقت(تہی);
	ساخت reflog_expire_options opts = REFLOG_EXPIRE_OPTIONS_INIT(now);
	صحیح i, حالت_2, do_all, single_worktree = 0;
	غیرعلامتی صحیح flags = 0;
	صحیح verbose = 0;
	reflog_expiry_should_prune_fn *should_prune_fn = should_expire_reflog_ent;
	ثابت ساخت option options[] = {
		OPT_ثنائی_ہندسہ('n', "dry-run", &flags, N_("do not actually prune any entries"),
			EXPIRE_REFLOGS_DRY_تسلسل),
		OPT_ثنائی_ہندسہ(0, "rewrite", &flags,
			N_("rewrite the old SHA1 with the new SHA1 of the entry that now precedes it"),
			EXPIRE_REFLOGS_REWRITE),
		OPT_ثنائی_ہندسہ(0, "updateref", &flags,
			N_("update the reference to the value of the top reflog entry"),
			EXPIRE_REFLOGS_UPDATE_REF),
		OPT_BOOL(0, "verbose", &verbose, N_("print extra information on screen")),
		OPT_CALLBACK_F(0, "expire", &opts, N_("timestamp"),
			       N_("prune entries older than the specified time"),
			       PARSE_OPT_NONEG,
			       expire_کل_callback),
		OPT_CALLBACK_F(0, "expire-unreachable", &opts, N_("timestamp"),
			       N_("prune entries older than <time> that are not reachable from the current tip of the branch"),
			       PARSE_OPT_NONEG,
			       expire_unreachable_callback),
		OPT_BOOL(0, "stale-fix", &opts.stalefix,
			 N_("prune any reflog entries that point to broken commits")),
		OPT_BOOL(0, "all", &do_all, N_("process the reflogs of all references")),
		OPT_BOOL(0, "single-worktree", &single_worktree,
			 N_("limits processing to reflogs from the current worktree only")),
		OPT_انتہا()
	};

	repo_config(the_repository, reflog_expire_config, &opts);

	save_commit_بفر = 0;
	do_all = حالت_2 = 0;

	argc = parse_options(argc, argv, سابقہ, options, reflog_expire_usage, 0);

	اگر (verbose)
		should_prune_fn = should_expire_reflog_ent_verbose;

	/*
	 * We can trust the commits and objects reachable from refs
	 * even in older repository.  We cannot trust what's reachable
	 * from reflog if the repository was pruned with older git.
	 */
	اگر (opts.stalefix) {
		ساخت rev_info revs;

		repo_init_revisions(the_repository, &revs, سابقہ);
		revs.do_not_die_on_missing_objects = 1;
		revs.ignore_missing = 1;
		revs.ignore_missing_links = 1;
		اگر (verbose)
			لکھو(_("Marking reachable objects..."));
		mark_reachable_objects(&revs, 0, 0, تہی);
		release_revisions(&revs);
		اگر (verbose)
			حرف_لکھو('\n');
	}

	اگر (do_all) {
		ساخت worktree_reflogs collected = {
			.reflogs = STRING_LIST_INIT_DUP,
		};
		ساخت string_list_شے *شے;
		ساخت worktree **worktrees, **p;

		worktrees = get_worktrees(the_repository);
		برائے (p = worktrees; *p; p++) {
			اگر (single_worktree && !(*p)->is_current)
				جاری;
			collected.worktree = *p;
			refs_for_each_reflog(get_worktree_ref_store(*p),
					     collect_reflog, &collected);
		}
		free_worktrees(worktrees);

		for_each_string_list_شے(شے, &collected.reflogs) {
			ساخت expire_reflog_policy_cb cb = {
				.opts = opts,
				.dry_تسلسل = !!(flags & EXPIRE_REFLOGS_DRY_تسلسل),
			};

			reflog_expire_options_سیٹ_refname(&cb.opts,  شے->string);
			حالت_2 |= refs_reflog_expire(get_main_ref_store(the_repository),
						     شے->string, flags,
						     reflog_expiry_prepare,
						     should_prune_fn,
						     reflog_expiry_cleanup,
						     &cb);
		}
		string_list_clear(&collected.reflogs, 0);
	}

	برائے (i = 0; i < argc; i++) {
		حرف *ref;
		ساخت expire_reflog_policy_cb cb = { .opts = opts };

		اگر (!repo_dwim_log(the_repository, argv[i], لڑی_لمبائی(argv[i]), تہی, &ref)) {
			حالت_2 |= error(_("reflog could not be found: '%s'"), argv[i]);
			جاری;
		}
		reflog_expire_options_سیٹ_refname(&cb.opts, ref);
		حالت_2 |= refs_reflog_expire(get_main_ref_store(the_repository),
					     ref, flags,
					     reflog_expiry_prepare,
					     should_prune_fn,
					     reflog_expiry_cleanup,
					     &cb);
		آزاد(ref);
	}

	reflog_clear_expire_config(&opts);

	واپس حالت_2;
}

ساکن صحیح cmd_reflog_delete(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
			     ساخت repository *repo غیر_مستعمل)
{
	صحیح i, حالت_2 = 0;
	غیرعلامتی صحیح flags = 0;
	صحیح verbose = 0;

	ثابت ساخت option options[] = {
		OPT_ثنائی_ہندسہ('n', "dry-run", &flags, N_("do not actually prune any entries"),
			EXPIRE_REFLOGS_DRY_تسلسل),
		OPT_ثنائی_ہندسہ(0, "rewrite", &flags,
			N_("rewrite the old SHA1 with the new SHA1 of the entry that now precedes it"),
			EXPIRE_REFLOGS_REWRITE),
		OPT_ثنائی_ہندسہ(0, "updateref", &flags,
			N_("update the reference to the value of the top reflog entry"),
			EXPIRE_REFLOGS_UPDATE_REF),
		OPT_BOOL(0, "verbose", &verbose, N_("print extra information on screen")),
		OPT_انتہا()
	};

	argc = parse_options(argc, argv, سابقہ, options, reflog_delete_usage, 0);

	اگر (argc < 1)
		واپس error(_("no reflog specified to delete"));

	برائے (i = 0; i < argc; i++)
		حالت_2 |= reflog_delete(argv[i], flags, verbose);

	واپس حالت_2;
}

ساکن صحیح cmd_reflog_exists(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
			     ساخت repository *repo غیر_مستعمل)
{
	ساخت option options[] = {
		OPT_انتہا()
	};
	ثابت حرف *refname;

	argc = parse_options(argc, argv, سابقہ, options, reflog_exists_usage,
			     0);
	اگر (!argc)
		usage_with_options(reflog_exists_usage, options);

	refname = argv[0];
	اگر (check_refname_format(refname, REFNAME_ALLOW_ONELEVEL))
		die(_("invalid ref format: %s"), refname);
	واپس !refs_reflog_exists(get_main_ref_store(the_repository),
				   refname);
}

ساکن صحیح cmd_reflog_drop(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
			   ساخت repository *repo)
{
	صحیح ret = 0, do_all = 0, single_worktree = 0;
	ثابت ساخت option options[] = {
		OPT_BOOL(0, "all", &do_all, N_("drop the reflogs of all references")),
		OPT_BOOL(0, "single-worktree", &single_worktree,
			 N_("drop reflogs from the current worktree only")),
		OPT_انتہا()
	};

	argc = parse_options(argc, argv, سابقہ, options, reflog_drop_usage, 0);

	اگر (argc && do_all)
		usage(_("references specified along with --all"));

	اگر (do_all) {
		ساخت worktree_reflogs collected = {
			.reflogs = STRING_LIST_INIT_DUP,
		};
		ساخت string_list_شے *شے;
		ساخت worktree **worktrees, **p;

		worktrees = get_worktrees(the_repository);
		برائے (p = worktrees; *p; p++) {
			اگر (single_worktree && !(*p)->is_current)
				جاری;
			collected.worktree = *p;
			refs_for_each_reflog(get_worktree_ref_store(*p),
					     collect_reflog, &collected);
		}
		free_worktrees(worktrees);

		for_each_string_list_شے(شے, &collected.reflogs)
			ret |= refs_delete_reflog(get_main_ref_store(repo),
						     شے->string);
		string_list_clear(&collected.reflogs, 0);

		واپس ret;
	}

	برائے (صحیح i = 0; i < argc; i++) {
		حرف *ref;
		اگر (!repo_dwim_log(repo, argv[i], لڑی_لمبائی(argv[i]), تہی, &ref)) {
			ret |= error(_("reflog could not be found: '%s'"), argv[i]);
			جاری;
		}

		ret |= refs_delete_reflog(get_main_ref_store(repo), ref);
		آزاد(ref);
	}

	واپس ret;
}

ساکن صحیح cmd_reflog_write(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
			    ساخت repository *repo)
{
	ثابت ساخت option options[] = {
		OPT_انتہا()
	};
	ساخت object_id old_oid, new_oid;
	ساخت strbuf err = STRBUF_INIT;
	ساخت ref_transaction *tx;
	ثابت حرف *ref, *پیغام_2;
	صحیح ret;

	repo_config(repo, git_ident_config, تہی);

	argc = parse_options(argc, argv, سابقہ, options, reflog_write_usage, 0);
	اگر (argc != 4)
		usage_with_options(reflog_write_usage, options);

	ref = argv[0];
	اگر (!is_جڑ_ref(ref) && check_refname_format(ref, 0))
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

	پیغام_2 = argv[3];

	tx = ref_store_transaction_begin(get_main_ref_store(repo), 0, &err);
	اگر (!tx)
		die(_("cannot start transaction: %s"), err.buf);

	ret = ref_transaction_update_reflog(tx, ref, &new_oid, &old_oid,
					    git_committer_info(0),
					    پیغام_2, 0, &err);
	اگر (ret)
		die(_("cannot queue reflog update: %s"), err.buf);

	ret = ref_transaction_commit(tx, &err);
	اگر (ret)
		die(_("cannot commit reflog update: %s"), err.buf);

	ref_transaction_free(tx);
	strbuf_release(&err);
	واپس 0;
}

/*
 * main "reflog"
 */
صحیح cmd_reflog(صحیح argc,
	       ثابت حرف **argv,
	       ثابت حرف *سابقہ,
	       ساخت repository *repository)
{
	parse_opt_subcommand_fn *fn = تہی;
	ساخت option options[] = {
		OPT_SUBCOMMAND("show", &fn, cmd_reflog_show),
		OPT_SUBCOMMAND("list", &fn, cmd_reflog_list),
		OPT_SUBCOMMAND("exists", &fn, cmd_reflog_exists),
		OPT_SUBCOMMAND("write", &fn, cmd_reflog_write),
		OPT_SUBCOMMAND("delete", &fn, cmd_reflog_delete),
		OPT_SUBCOMMAND("drop", &fn, cmd_reflog_drop),
		OPT_SUBCOMMAND("expire", &fn, cmd_reflog_expire),
		OPT_انتہا()
	};

	argc = parse_options(argc, argv, سابقہ, options, reflog_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL |
			     PARSE_OPT_KEEP_DASHDASH | PARSE_OPT_KEEP_ARGV0 |
			     PARSE_OPT_KEEP_UNKNOWN_OPT);
	اگر (fn)
		واپس fn(argc - 1, argv + 1, سابقہ, repository);
	ورنہ
		واپس cmd_log_reflog(argc, argv, سابقہ, repository);
}
