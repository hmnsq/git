#تعریف_کړه USE_THE_REPOSITORY_VARIABLE

#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "gettext.h"
#ضمن "hex.h"
#ضمن "odb.h"
#ضمن "revision.h"
#ضمن "reachable.h"
#ضمن "wildmatch.h"
#ضمن "worktree.h"
#ضمن "reflog.h"
#ضمن "refs.h"
#ضمن "parse-options.h"

#تعریف_کړه BUILTIN_REFLOG_SHOW_USAGE \
	N_("git reflog [show] [<log-options>] [<ref>]")

#تعریف_کړه BUILTIN_REFLOG_LIST_USAGE \
	N_("git reflog list")

#تعریف_کړه BUILTIN_REFLOG_EXISTS_USAGE \
	N_("git reflog exists <ref>")

#تعریف_کړه BUILTIN_REFLOG_WRITE_USAGE \
	N_("git reflog write <ref> <old-oid> <new-oid> <message>")

#تعریف_کړه BUILTIN_REFLOG_DELETE_USAGE \
	N_("git reflog delete [--rewrite] [--updateref]\n" \
	   "                  [--dry-run | -n] [--verbose] <ref>@{<specifier>}...")

#تعریف_کړه BUILTIN_REFLOG_DROP_USAGE \
	N_("git reflog drop [--all [--single-worktree] | <refs>...]")

#تعریف_کړه BUILTIN_REFLOG_EXPIRE_USAGE \
	N_("git reflog expire [--expire=<time>] [--expire-unreachable=<time>]\n" \
	   "                  [--rewrite] [--updateref] [--stale-fix]\n" \
	   "                  [--dry-run | -n] [--verbose] [--all [--single-worktree] | <refs>...]")

ساكن ثابت حرف *ثابت reflog_show_usage[] = {
	BUILTIN_REFLOG_SHOW_USAGE,
	عدم,
};

ساكن ثابت حرف *ثابت reflog_list_usage[] = {
	BUILTIN_REFLOG_LIST_USAGE,
	عدم,
};

ساكن ثابت حرف *ثابت reflog_exists_usage[] = {
	BUILTIN_REFLOG_EXISTS_USAGE,
	عدم,
};

ساكن ثابت حرف *ثابت reflog_write_usage[] = {
	BUILTIN_REFLOG_WRITE_USAGE,
	عدم,
};

ساكن ثابت حرف *ثابت reflog_delete_usage[] = {
	BUILTIN_REFLOG_DELETE_USAGE,
	عدم
};

ساكن ثابت حرف *ثابت reflog_drop_usage[] = {
	BUILTIN_REFLOG_DROP_USAGE,
	عدم,
};

ساكن ثابت حرف *ثابت reflog_expire_usage[] = {
	BUILTIN_REFLOG_EXPIRE_USAGE,
	عدم
};

ساكن ثابت حرف *ثابت reflog_usage[] = {
	BUILTIN_REFLOG_SHOW_USAGE,
	BUILTIN_REFLOG_LIST_USAGE,
	BUILTIN_REFLOG_EXISTS_USAGE,
	BUILTIN_REFLOG_WRITE_USAGE,
	BUILTIN_REFLOG_DELETE_USAGE,
	BUILTIN_REFLOG_DROP_USAGE,
	BUILTIN_REFLOG_EXPIRE_USAGE,
	عدم
};

بنية worktree_reflogs {
	بنية worktree *worktree;
	بنية string_list reflogs;
};

ساكن صحيح collect_reflog(ثابت حرف *ref, فراغ *cb_معلومات)
{
	بنية worktree_reflogs *cb = cb_معلومات;
	بنية worktree *worktree = cb->worktree;
	بنية strbuf newref = STRBUF_INIT;

	/*
	 * Avoid collecting the same shared ref multiple times because
	 * they are available via all worktrees.
	 */
	إذا (!worktree->is_current &&
	    parse_worktree_ref(ref, عدم, عدم, عدم) == REF_WORKTREE_SHARED)
		أرجع 0;

	strbuf_worktree_ref(worktree, &newref, ref);
	string_list_append_nodup(&cb->reflogs, strbuf_detach(&newref, عدم));

	أرجع 0;
}

ساكن صحيح expire_unreachable_callback(ثابت بنية option *opt,
				 ثابت حرف *arg,
				 صحيح unset)
{
	بنية reflog_expire_options *opts = opt->ارزښت;

	BUG_ON_OPT_NEG(unset);

	إذا (parse_expiry_date(arg, &opts->expire_unreachable))
		die(_("invalid timestamp '%s' given to '--%s'"),
		    arg, opt->long_name);

	opts->explicit_expiry |= REFLOG_EXPIRE_UNREACH;
	أرجع 0;
}

ساكن صحيح expire_ټول_callback(ثابت بنية option *opt,
				 ثابت حرف *arg,
				 صحيح unset)
{
	بنية reflog_expire_options *opts = opt->ارزښت;

	BUG_ON_OPT_NEG(unset);

	إذا (parse_expiry_date(arg, &opts->expire_ټول))
		die(_("invalid timestamp '%s' given to '--%s'"),
		    arg, opt->long_name);

	opts->explicit_expiry |= REFLOG_EXPIRE_ټول;
	أرجع 0;
}

ساكن صحيح cmd_reflog_show(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی,
			   بنية repository *repo ناکارول_شوی)
{
	بنية option options[] = {
		OPT_پای_ټکی()
	};

	parse_options(argc, argv, مختاړی, options, reflog_show_usage,
		      PARSE_OPT_KEEP_DASHDASH | PARSE_OPT_KEEP_ARGV0 |
		      PARSE_OPT_KEEP_UNKNOWN_OPT);

	أرجع cmd_log_reflog(argc, argv, مختاړی, the_repository);
}

ساكن صحيح show_reflog(ثابت حرف *refname, فراغ *cb_معلومات ناکارول_شوی)
{
	چاپ_کړه("%s\n", refname);
	أرجع 0;
}

ساكن صحيح cmd_reflog_list(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی,
			   بنية repository *repo ناکارول_شوی)
{
	بنية option options[] = {
		OPT_پای_ټکی()
	};
	بنية ref_store *ref_store;

	argc = parse_options(argc, argv, مختاړی, options, reflog_list_usage, 0);
	إذا (argc)
		أرجع error(_("%s does not accept arguments: '%s'"),
			     "list", argv[0]);

	ref_store = get_main_ref_store(the_repository);

	أرجع refs_for_each_reflog(ref_store, show_reflog, عدم);
}

ساكن صحيح cmd_reflog_expire(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی,
			     بنية repository *repo ناکارول_شوی)
{
	timestamp_t now = وخت(عدم);
	بنية reflog_expire_options opts = REFLOG_EXPIRE_OPTIONS_INIT(now);
	صحيح i, حالت, do_all, single_worktree = 0;
	بینښه صحيح flags = 0;
	صحيح verbose = 0;
	reflog_expiry_should_prune_fn *should_prune_fn = should_expire_reflog_ent;
	ثابت بنية option options[] = {
		OPT_بټ('n', "dry-run", &flags, N_("do not actually prune any entries"),
			EXPIRE_REFLOGS_DRY_پرلپسې),
		OPT_بټ(0, "rewrite", &flags,
			N_("rewrite the old SHA1 with the new SHA1 of the entry that now precedes it"),
			EXPIRE_REFLOGS_REWRITE),
		OPT_بټ(0, "updateref", &flags,
			N_("update the reference to the value of the top reflog entry"),
			EXPIRE_REFLOGS_UPDATE_REF),
		OPT_BOOL(0, "verbose", &verbose, N_("print extra information on screen")),
		OPT_CALLBACK_F(0, "expire", &opts, N_("timestamp"),
			       N_("prune entries older than the specified time"),
			       PARSE_OPT_NONEG,
			       expire_ټول_callback),
		OPT_CALLBACK_F(0, "expire-unreachable", &opts, N_("timestamp"),
			       N_("prune entries older than <time> that are not reachable from the current tip of the branch"),
			       PARSE_OPT_NONEG,
			       expire_unreachable_callback),
		OPT_BOOL(0, "stale-fix", &opts.stalefix,
			 N_("prune any reflog entries that point to broken commits")),
		OPT_BOOL(0, "all", &do_all, N_("process the reflogs of all references")),
		OPT_BOOL(0, "single-worktree", &single_worktree,
			 N_("limits processing to reflogs from the current worktree only")),
		OPT_پای_ټکی()
	};

	repo_config(the_repository, reflog_expire_config, &opts);

	save_commit_بفر = 0;
	do_all = حالت = 0;

	argc = parse_options(argc, argv, مختاړی, options, reflog_expire_usage, 0);

	إذا (verbose)
		should_prune_fn = should_expire_reflog_ent_verbose;

	/*
	 * We can trust the commits and objects reachable from refs
	 * even in older repository.  We cannot trust what's reachable
	 * from reflog if the repository was pruned with older git.
	 */
	إذا (opts.stalefix) {
		بنية rev_info revs;

		repo_init_revisions(the_repository, &revs, مختاړی);
		revs.do_not_die_on_missing_objects = 1;
		revs.ignore_missing = 1;
		revs.ignore_missing_links = 1;
		إذا (verbose)
			چاپ_کړه(_("Marking reachable objects..."));
		mark_reachable_objects(&revs, 0, 0, عدم);
		release_revisions(&revs);
		إذا (verbose)
			توری_چاپ_کړه('\n');
	}

	إذا (do_all) {
		بنية worktree_reflogs collected = {
			.reflogs = STRING_LIST_INIT_DUP,
		};
		بنية string_list_توکی *توکی;
		بنية worktree **worktrees, **p;

		worktrees = get_worktrees(the_repository);
		لكل (p = worktrees; *p; p++) {
			إذا (single_worktree && !(*p)->is_current)
				استمر;
			collected.worktree = *p;
			refs_for_each_reflog(get_worktree_ref_store(*p),
					     collect_reflog, &collected);
		}
		free_worktrees(worktrees);

		for_each_string_list_توکی(توکی, &collected.reflogs) {
			بنية expire_reflog_policy_cb cb = {
				.opts = opts,
				.dry_پرلپسې = !!(flags & EXPIRE_REFLOGS_DRY_پرلپسې),
			};

			reflog_expire_options_سېټ_refname(&cb.opts,  توکی->string);
			حالت |= refs_reflog_expire(get_main_ref_store(the_repository),
						     توکی->string, flags,
						     reflog_expiry_prepare,
						     should_prune_fn,
						     reflog_expiry_cleanup,
						     &cb);
		}
		string_list_clear(&collected.reflogs, 0);
	}

	لكل (i = 0; i < argc; i++) {
		حرف *ref;
		بنية expire_reflog_policy_cb cb = { .opts = opts };

		إذا (!repo_dwim_log(the_repository, argv[i], مزي_اوږدوالی(argv[i]), عدم, &ref)) {
			حالت |= error(_("reflog could not be found: '%s'"), argv[i]);
			استمر;
		}
		reflog_expire_options_سېټ_refname(&cb.opts, ref);
		حالت |= refs_reflog_expire(get_main_ref_store(the_repository),
					     ref, flags,
					     reflog_expiry_prepare,
					     should_prune_fn,
					     reflog_expiry_cleanup,
					     &cb);
		حرر(ref);
	}

	reflog_clear_expire_config(&opts);

	أرجع حالت;
}

ساكن صحيح cmd_reflog_delete(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی,
			     بنية repository *repo ناکارول_شوی)
{
	صحيح i, حالت = 0;
	بینښه صحيح flags = 0;
	صحيح verbose = 0;

	ثابت بنية option options[] = {
		OPT_بټ('n', "dry-run", &flags, N_("do not actually prune any entries"),
			EXPIRE_REFLOGS_DRY_پرلپسې),
		OPT_بټ(0, "rewrite", &flags,
			N_("rewrite the old SHA1 with the new SHA1 of the entry that now precedes it"),
			EXPIRE_REFLOGS_REWRITE),
		OPT_بټ(0, "updateref", &flags,
			N_("update the reference to the value of the top reflog entry"),
			EXPIRE_REFLOGS_UPDATE_REF),
		OPT_BOOL(0, "verbose", &verbose, N_("print extra information on screen")),
		OPT_پای_ټکی()
	};

	argc = parse_options(argc, argv, مختاړی, options, reflog_delete_usage, 0);

	إذا (argc < 1)
		أرجع error(_("no reflog specified to delete"));

	لكل (i = 0; i < argc; i++)
		حالت |= reflog_delete(argv[i], flags, verbose);

	أرجع حالت;
}

ساكن صحيح cmd_reflog_exists(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی,
			     بنية repository *repo ناکارول_شوی)
{
	بنية option options[] = {
		OPT_پای_ټکی()
	};
	ثابت حرف *refname;

	argc = parse_options(argc, argv, مختاړی, options, reflog_exists_usage,
			     0);
	إذا (!argc)
		usage_with_options(reflog_exists_usage, options);

	refname = argv[0];
	إذا (check_refname_format(refname, REFNAME_ALLOW_ONELEVEL))
		die(_("invalid ref format: %s"), refname);
	أرجع !refs_reflog_exists(get_main_ref_store(the_repository),
				   refname);
}

ساكن صحيح cmd_reflog_drop(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی,
			   بنية repository *repo)
{
	صحيح ret = 0, do_all = 0, single_worktree = 0;
	ثابت بنية option options[] = {
		OPT_BOOL(0, "all", &do_all, N_("drop the reflogs of all references")),
		OPT_BOOL(0, "single-worktree", &single_worktree,
			 N_("drop reflogs from the current worktree only")),
		OPT_پای_ټکی()
	};

	argc = parse_options(argc, argv, مختاړی, options, reflog_drop_usage, 0);

	إذا (argc && do_all)
		usage(_("references specified along with --all"));

	إذا (do_all) {
		بنية worktree_reflogs collected = {
			.reflogs = STRING_LIST_INIT_DUP,
		};
		بنية string_list_توکی *توکی;
		بنية worktree **worktrees, **p;

		worktrees = get_worktrees(the_repository);
		لكل (p = worktrees; *p; p++) {
			إذا (single_worktree && !(*p)->is_current)
				استمر;
			collected.worktree = *p;
			refs_for_each_reflog(get_worktree_ref_store(*p),
					     collect_reflog, &collected);
		}
		free_worktrees(worktrees);

		for_each_string_list_توکی(توکی, &collected.reflogs)
			ret |= refs_delete_reflog(get_main_ref_store(repo),
						     توکی->string);
		string_list_clear(&collected.reflogs, 0);

		أرجع ret;
	}

	لكل (صحيح i = 0; i < argc; i++) {
		حرف *ref;
		إذا (!repo_dwim_log(repo, argv[i], مزي_اوږدوالی(argv[i]), عدم, &ref)) {
			ret |= error(_("reflog could not be found: '%s'"), argv[i]);
			استمر;
		}

		ret |= refs_delete_reflog(get_main_ref_store(repo), ref);
		حرر(ref);
	}

	أرجع ret;
}

ساكن صحيح cmd_reflog_write(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی,
			    بنية repository *repo)
{
	ثابت بنية option options[] = {
		OPT_پای_ټکی()
	};
	بنية object_id old_oid, new_oid;
	بنية strbuf err = STRBUF_INIT;
	بنية ref_transaction *tx;
	ثابت حرف *ref, *پیغام_2;
	صحيح ret;

	repo_config(repo, git_ident_config, عدم);

	argc = parse_options(argc, argv, مختاړی, options, reflog_write_usage, 0);
	إذا (argc != 4)
		usage_with_options(reflog_write_usage, options);

	ref = argv[0];
	إذا (!is_ریښه_ref(ref) && check_refname_format(ref, 0))
		die(_("invalid reference name: %s"), ref);

	ret = get_oid_hex_algop(argv[1], &old_oid, repo->hash_algo);
	إذا (ret)
		die(_("invalid old object ID: '%s'"), argv[1]);
	إذا (!is_null_oid(&old_oid) && !odb_has_object(repo->objects, &old_oid, 0))
		die(_("old object '%s' does not exist"), argv[1]);

	ret = get_oid_hex_algop(argv[2], &new_oid, repo->hash_algo);
	إذا (ret)
		die(_("invalid new object ID: '%s'"), argv[2]);
	إذا (!is_null_oid(&new_oid) && !odb_has_object(repo->objects, &new_oid, 0))
		die(_("new object '%s' does not exist"), argv[2]);

	پیغام_2 = argv[3];

	tx = ref_store_transaction_begin(get_main_ref_store(repo), 0, &err);
	إذا (!tx)
		die(_("cannot start transaction: %s"), err.buf);

	ret = ref_transaction_update_reflog(tx, ref, &new_oid, &old_oid,
					    git_committer_info(0),
					    پیغام_2, 0, &err);
	إذا (ret)
		die(_("cannot queue reflog update: %s"), err.buf);

	ret = ref_transaction_commit(tx, &err);
	إذا (ret)
		die(_("cannot commit reflog update: %s"), err.buf);

	ref_transaction_free(tx);
	strbuf_release(&err);
	أرجع 0;
}

/*
 * main "reflog"
 */
صحيح cmd_reflog(صحيح argc,
	       ثابت حرف **argv,
	       ثابت حرف *مختاړی,
	       بنية repository *repository)
{
	parse_opt_subcommand_fn *fn = عدم;
	بنية option options[] = {
		OPT_SUBCOMMAND("show", &fn, cmd_reflog_show),
		OPT_SUBCOMMAND("list", &fn, cmd_reflog_list),
		OPT_SUBCOMMAND("exists", &fn, cmd_reflog_exists),
		OPT_SUBCOMMAND("write", &fn, cmd_reflog_write),
		OPT_SUBCOMMAND("delete", &fn, cmd_reflog_delete),
		OPT_SUBCOMMAND("drop", &fn, cmd_reflog_drop),
		OPT_SUBCOMMAND("expire", &fn, cmd_reflog_expire),
		OPT_پای_ټکی()
	};

	argc = parse_options(argc, argv, مختاړی, options, reflog_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL |
			     PARSE_OPT_KEEP_DASHDASH | PARSE_OPT_KEEP_ARGV0 |
			     PARSE_OPT_KEEP_UNKNOWN_OPT);
	إذا (fn)
		أرجع fn(argc - 1, argv + 1, مختاړی, repository);
	وإلا
		أرجع cmd_log_reflog(argc, argv, مختاړی, repository);
}
