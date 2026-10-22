#define USE_THE_REPOSITORY_VARIABLE

#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "hex.h"
#ଅନ୍ତର୍ଭାଵ "odb.h"
#ଅନ୍ତର୍ଭାଵ "revision.h"
#ଅନ୍ତର୍ଭାଵ "reachable.h"
#ଅନ୍ତର୍ଭାଵ "wildmatch.h"
#ଅନ୍ତର୍ଭାଵ "worktree.h"
#ଅନ୍ତର୍ଭାଵ "reflog.h"
#ଅନ୍ତର୍ଭାଵ "refs.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"

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

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *ଅଚଳ reflog_show_usage[] = {
	BUILTIN_REFLOG_SHOW_USAGE,
	ଖାଲି,
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *ଅଚଳ reflog_list_usage[] = {
	BUILTIN_REFLOG_LIST_USAGE,
	ଖାଲି,
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *ଅଚଳ reflog_exists_usage[] = {
	BUILTIN_REFLOG_EXISTS_USAGE,
	ଖାଲି,
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *ଅଚଳ reflog_write_usage[] = {
	BUILTIN_REFLOG_WRITE_USAGE,
	ଖାଲି,
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *ଅଚଳ reflog_delete_usage[] = {
	BUILTIN_REFLOG_DELETE_USAGE,
	ଖାଲି
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *ଅଚଳ reflog_drop_usage[] = {
	BUILTIN_REFLOG_DROP_USAGE,
	ଖାଲି,
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *ଅଚଳ reflog_expire_usage[] = {
	BUILTIN_REFLOG_EXPIRE_USAGE,
	ଖାଲି
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *ଅଚଳ reflog_usage[] = {
	BUILTIN_REFLOG_SHOW_USAGE,
	BUILTIN_REFLOG_LIST_USAGE,
	BUILTIN_REFLOG_EXISTS_USAGE,
	BUILTIN_REFLOG_WRITE_USAGE,
	BUILTIN_REFLOG_DELETE_USAGE,
	BUILTIN_REFLOG_DROP_USAGE,
	BUILTIN_REFLOG_EXPIRE_USAGE,
	ଖାଲି
};

ସଂରଚନା worktree_reflogs {
	ସଂରଚନା worktree *worktree;
	ସଂରଚନା string_list reflogs;
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା collect_reflog(ଅଚଳ ଅକ୍ଷର *ref, ଶୂନ୍ୟ *cb_data)
{
	ସଂରଚନା worktree_reflogs *cb = cb_data;
	ସଂରଚନା worktree *worktree = cb->worktree;
	ସଂରଚନା strbuf newref = STRBUF_INIT;

	/*
	 * Avoid collecting the same shared ref multiple times because
	 * they are available via all worktrees.
	 */
	ଯଦି (!worktree->is_current &&
	    parse_worktree_ref(ref, ଖାଲି, ଖାଲି, ଖାଲି) == REF_WORKTREE_SHARED)
		ଫେରନ୍ତୁ 0;

	strbuf_worktree_ref(worktree, &newref, ref);
	string_list_append_nodup(&cb->reflogs, strbuf_detach(&newref, ଖାଲି));

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା expire_unreachable_callback(ଅଚଳ ସଂରଚନା option *opt,
				 ଅଚଳ ଅକ୍ଷର *arg,
				 ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ସଂରଚନା reflog_expire_options *opts = opt->ମୂଲ୍ୟ;

	BUG_ON_OPT_NEG(unset);

	ଯଦି (parse_expiry_date(arg, &opts->expire_unreachable))
		die(_("invalid timestamp '%s' given to '--%s'"),
		    arg, opt->long_name);

	opts->explicit_expiry |= REFLOG_EXPIRE_UNREACH;
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା expire_total_callback(ଅଚଳ ସଂରଚନା option *opt,
				 ଅଚଳ ଅକ୍ଷର *arg,
				 ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ସଂରଚନା reflog_expire_options *opts = opt->ମୂଲ୍ୟ;

	BUG_ON_OPT_NEG(unset);

	ଯଦି (parse_expiry_date(arg, &opts->expire_total))
		die(_("invalid timestamp '%s' given to '--%s'"),
		    arg, opt->long_name);

	opts->explicit_expiry |= REFLOG_EXPIRE_TOTAL;
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_reflog_show(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
			   ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା option options[] = {
		OPT_END()
	};

	parse_options(argc, argv, prefix, options, reflog_show_usage,
		      PARSE_OPT_KEEP_DASHDASH | PARSE_OPT_KEEP_ARGV0 |
		      PARSE_OPT_KEEP_UNKNOWN_OPT);

	ଫେରନ୍ତୁ cmd_log_reflog(argc, argv, prefix, the_repository);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_reflog(ଅଚଳ ଅକ୍ଷର *refname, ଶୂନ୍ୟ *cb_data UNUSED)
{
	printf("%s\n", refname);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_reflog_list(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
			   ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା option options[] = {
		OPT_END()
	};
	ସଂରଚନା ref_store *ref_store;

	argc = parse_options(argc, argv, prefix, options, reflog_list_usage, 0);
	ଯଦି (argc)
		ଫେରନ୍ତୁ error(_("%s does not accept arguments: '%s'"),
			     "list", argv[0]);

	ref_store = get_main_ref_store(the_repository);

	ଫେରନ୍ତୁ refs_for_each_reflog(ref_store, show_reflog, ଖାଲି);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_reflog_expire(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
			     ସଂରଚନା repository *repo UNUSED)
{
	timestamp_t now = time(ଖାଲି);
	ସଂରଚନା reflog_expire_options opts = REFLOG_EXPIRE_OPTIONS_INIT(now);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, status, do_all, single_worktree = 0;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା verbose = 0;
	reflog_expiry_should_prune_fn *should_prune_fn = should_expire_reflog_ent;
	ଅଚଳ ସଂରଚନା option options[] = {
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

	ଯଦି (verbose)
		should_prune_fn = should_expire_reflog_ent_verbose;

	/*
	 * We can trust the commits and objects reachable from refs
	 * even in older repository.  We cannot trust what's reachable
	 * from reflog if the repository was pruned with older git.
	 */
	ଯଦି (opts.stalefix) {
		ସଂରଚନା rev_info revs;

		repo_init_revisions(the_repository, &revs, prefix);
		revs.do_not_die_on_missing_objects = 1;
		revs.ignore_missing = 1;
		revs.ignore_missing_links = 1;
		ଯଦି (verbose)
			printf(_("Marking reachable objects..."));
		mark_reachable_objects(&revs, 0, 0, ଖାଲି);
		release_revisions(&revs);
		ଯଦି (verbose)
			putchar('\n');
	}

	ଯଦି (do_all) {
		ସଂରଚନା worktree_reflogs collected = {
			.reflogs = STRING_LIST_INIT_DUP,
		};
		ସଂରଚନା string_list_item *item;
		ସଂରଚନା worktree **worktrees, **p;

		worktrees = get_worktrees(the_repository);
		ପାଇଁ (p = worktrees; *p; p++) {
			ଯଦି (single_worktree && !(*p)->is_current)
				ଜାରି;
			collected.worktree = *p;
			refs_for_each_reflog(get_worktree_ref_store(*p),
					     collect_reflog, &collected);
		}
		free_worktrees(worktrees);

		for_each_string_list_item(item, &collected.reflogs) {
			ସଂରଚନା expire_reflog_policy_cb cb = {
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

	ପାଇଁ (i = 0; i < argc; i++) {
		ଅକ୍ଷର *ref;
		ସଂରଚନା expire_reflog_policy_cb cb = { .opts = opts };

		ଯଦି (!repo_dwim_log(the_repository, argv[i], strlen(argv[i]), ଖାଲି, &ref)) {
			status |= error(_("reflog could not be found: '%s'"), argv[i]);
			ଜାରି;
		}
		reflog_expire_options_set_refname(&cb.opts, ref);
		status |= refs_reflog_expire(get_main_ref_store(the_repository),
					     ref, flags,
					     reflog_expiry_prepare,
					     should_prune_fn,
					     reflog_expiry_cleanup,
					     &cb);
		ମୁକ୍ତ(ref);
	}

	reflog_clear_expire_config(&opts);

	ଫେରନ୍ତୁ status;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_reflog_delete(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
			     ସଂରଚନା repository *repo UNUSED)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, status = 0;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା verbose = 0;

	ଅଚଳ ସଂରଚନା option options[] = {
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

	ଯଦି (argc < 1)
		ଫେରନ୍ତୁ error(_("no reflog specified to delete"));

	ପାଇଁ (i = 0; i < argc; i++)
		status |= reflog_delete(argv[i], flags, verbose);

	ଫେରନ୍ତୁ status;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_reflog_exists(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
			     ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା option options[] = {
		OPT_END()
	};
	ଅଚଳ ଅକ୍ଷର *refname;

	argc = parse_options(argc, argv, prefix, options, reflog_exists_usage,
			     0);
	ଯଦି (!argc)
		usage_with_options(reflog_exists_usage, options);

	refname = argv[0];
	ଯଦି (check_refname_format(refname, REFNAME_ALLOW_ONELEVEL))
		die(_("invalid ref format: %s"), refname);
	ଫେରନ୍ତୁ !refs_reflog_exists(get_main_ref_store(the_repository),
				   refname);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_reflog_drop(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
			   ସଂରଚନା repository *repo)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0, do_all = 0, single_worktree = 0;
	ଅଚଳ ସଂରଚନା option options[] = {
		OPT_BOOL(0, "all", &do_all, N_("drop the reflogs of all references")),
		OPT_BOOL(0, "single-worktree", &single_worktree,
			 N_("drop reflogs from the current worktree only")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, reflog_drop_usage, 0);

	ଯଦି (argc && do_all)
		usage(_("references specified along with --all"));

	ଯଦି (do_all) {
		ସଂରଚନା worktree_reflogs collected = {
			.reflogs = STRING_LIST_INIT_DUP,
		};
		ସଂରଚନା string_list_item *item;
		ସଂରଚନା worktree **worktrees, **p;

		worktrees = get_worktrees(the_repository);
		ପାଇଁ (p = worktrees; *p; p++) {
			ଯଦି (single_worktree && !(*p)->is_current)
				ଜାରି;
			collected.worktree = *p;
			refs_for_each_reflog(get_worktree_ref_store(*p),
					     collect_reflog, &collected);
		}
		free_worktrees(worktrees);

		for_each_string_list_item(item, &collected.reflogs)
			ret |= refs_delete_reflog(get_main_ref_store(repo),
						     item->string);
		string_list_clear(&collected.reflogs, 0);

		ଫେରନ୍ତୁ ret;
	}

	ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < argc; i++) {
		ଅକ୍ଷର *ref;
		ଯଦି (!repo_dwim_log(repo, argv[i], strlen(argv[i]), ଖାଲି, &ref)) {
			ret |= error(_("reflog could not be found: '%s'"), argv[i]);
			ଜାରି;
		}

		ret |= refs_delete_reflog(get_main_ref_store(repo), ref);
		ମୁକ୍ତ(ref);
	}

	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_reflog_write(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
			    ସଂରଚନା repository *repo)
{
	ଅଚଳ ସଂରଚନା option options[] = {
		OPT_END()
	};
	ସଂରଚନା object_id old_oid, new_oid;
	ସଂରଚନା strbuf err = STRBUF_INIT;
	ସଂରଚନା ref_transaction *tx;
	ଅଚଳ ଅକ୍ଷର *ref, *message;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	repo_config(repo, git_ident_config, ଖାଲି);

	argc = parse_options(argc, argv, prefix, options, reflog_write_usage, 0);
	ଯଦି (argc != 4)
		usage_with_options(reflog_write_usage, options);

	ref = argv[0];
	ଯଦି (!is_ମୂଳ_ref(ref) && check_refname_format(ref, 0))
		die(_("invalid reference name: %s"), ref);

	ret = get_oid_hex_algop(argv[1], &old_oid, repo->hash_algo);
	ଯଦି (ret)
		die(_("invalid old object ID: '%s'"), argv[1]);
	ଯଦି (!is_null_oid(&old_oid) && !odb_has_object(repo->objects, &old_oid, 0))
		die(_("old object '%s' does not exist"), argv[1]);

	ret = get_oid_hex_algop(argv[2], &new_oid, repo->hash_algo);
	ଯଦି (ret)
		die(_("invalid new object ID: '%s'"), argv[2]);
	ଯଦି (!is_null_oid(&new_oid) && !odb_has_object(repo->objects, &new_oid, 0))
		die(_("new object '%s' does not exist"), argv[2]);

	message = argv[3];

	tx = ref_store_transaction_begin(get_main_ref_store(repo), 0, &err);
	ଯଦି (!tx)
		die(_("cannot start transaction: %s"), err.buf);

	ret = ref_transaction_update_reflog(tx, ref, &new_oid, &old_oid,
					    git_committer_info(0),
					    message, 0, &err);
	ଯଦି (ret)
		die(_("cannot queue reflog update: %s"), err.buf);

	ret = ref_transaction_commit(tx, &err);
	ଯଦି (ret)
		die(_("cannot commit reflog update: %s"), err.buf);

	ref_transaction_free(tx);
	strbuf_release(&err);
	ଫେରନ୍ତୁ 0;
}

/*
 * main "reflog"
 */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_reflog(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
	       ଅଚଳ ଅକ୍ଷର **argv,
	       ଅଚଳ ଅକ୍ଷର *prefix,
	       ସଂରଚନା repository *repository)
{
	parse_opt_subcommand_fn *fn = ଖାଲି;
	ସଂରଚନା option options[] = {
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
	ଯଦି (fn)
		ଫେରନ୍ତୁ fn(argc - 1, argv + 1, prefix, repository);
	ଅନ୍ୟଥା
		ଫେରନ୍ତୁ cmd_log_reflog(argc, argv, prefix, repository);
}
