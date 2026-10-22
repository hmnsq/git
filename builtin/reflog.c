#define USE_THE_REPOSITORY_VARIABLE

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "odb.h"
#ಅನ್ತರ್ಭಾವ "revision.h"
#ಅನ್ತರ್ಭಾವ "reachable.h"
#ಅನ್ತರ್ಭಾವ "wildmatch.h"
#ಅನ್ತರ್ಭಾವ "worktree.h"
#ಅನ್ತರ್ಭಾವ "reflog.h"
#ಅನ್ತರ್ಭಾವ "refs.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"

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

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ reflog_show_usage[] = {
	BUILTIN_REFLOG_SHOW_USAGE,
	ರಿಕ್ತ,
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ reflog_list_usage[] = {
	BUILTIN_REFLOG_LIST_USAGE,
	ರಿಕ್ತ,
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ reflog_exists_usage[] = {
	BUILTIN_REFLOG_EXISTS_USAGE,
	ರಿಕ್ತ,
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ reflog_write_usage[] = {
	BUILTIN_REFLOG_WRITE_USAGE,
	ರಿಕ್ತ,
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ reflog_delete_usage[] = {
	BUILTIN_REFLOG_DELETE_USAGE,
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ reflog_drop_usage[] = {
	BUILTIN_REFLOG_DROP_USAGE,
	ರಿಕ್ತ,
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ reflog_expire_usage[] = {
	BUILTIN_REFLOG_EXPIRE_USAGE,
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ reflog_usage[] = {
	BUILTIN_REFLOG_SHOW_USAGE,
	BUILTIN_REFLOG_LIST_USAGE,
	BUILTIN_REFLOG_EXISTS_USAGE,
	BUILTIN_REFLOG_WRITE_USAGE,
	BUILTIN_REFLOG_DELETE_USAGE,
	BUILTIN_REFLOG_DROP_USAGE,
	BUILTIN_REFLOG_EXPIRE_USAGE,
	ರಿಕ್ತ
};

ಸಂರಚನಾ worktree_reflogs {
	ಸಂರಚನಾ worktree *worktree;
	ಸಂರಚನಾ string_list reflogs;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ collect_reflog(ಧ್ರುವ ವರ್ಣ *ref, ಶೂನ್ಯ *cb_data)
{
	ಸಂರಚನಾ worktree_reflogs *cb = cb_data;
	ಸಂರಚನಾ worktree *worktree = cb->worktree;
	ಸಂರಚನಾ strbuf newref = STRBUF_INIT;

	/*
	 * Avoid collecting the same shared ref multiple times because
	 * they are available via all worktrees.
	 */
	ಯದಿ (!worktree->is_current &&
	    parse_worktree_ref(ref, ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ) == REF_WORKTREE_SHARED)
		ಪ್ರತ್ಯಾಗಮ 0;

	strbuf_worktree_ref(worktree, &newref, ref);
	string_list_append_nodup(&cb->reflogs, strbuf_detach(&newref, ರಿಕ್ತ));

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ expire_unreachable_callback(ಧ್ರುವ ಸಂರಚನಾ option *opt,
				 ಧ್ರುವ ವರ್ಣ *arg,
				 ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಸಂರಚನಾ reflog_expire_options *opts = opt->ಮೌಲ್ಯ;

	BUG_ON_OPT_NEG(unset);

	ಯದಿ (parse_expiry_date(arg, &opts->expire_unreachable))
		die(_("invalid timestamp '%s' given to '--%s'"),
		    arg, opt->long_name);

	opts->explicit_expiry |= REFLOG_EXPIRE_UNREACH;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ expire_total_callback(ಧ್ರುವ ಸಂರಚನಾ option *opt,
				 ಧ್ರುವ ವರ್ಣ *arg,
				 ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಸಂರಚನಾ reflog_expire_options *opts = opt->ಮೌಲ್ಯ;

	BUG_ON_OPT_NEG(unset);

	ಯದಿ (parse_expiry_date(arg, &opts->expire_total))
		die(_("invalid timestamp '%s' given to '--%s'"),
		    arg, opt->long_name);

	opts->explicit_expiry |= REFLOG_EXPIRE_TOTAL;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmd_reflog_show(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
			   ಸಂರಚನಾ repository *repo UNUSED)
{
	ಸಂರಚನಾ option options[] = {
		OPT_END()
	};

	parse_options(argc, argv, prefix, options, reflog_show_usage,
		      PARSE_OPT_KEEP_DASHDASH | PARSE_OPT_KEEP_ARGV0 |
		      PARSE_OPT_KEEP_UNKNOWN_OPT);

	ಪ್ರತ್ಯಾಗಮ cmd_log_reflog(argc, argv, prefix, the_repository);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_reflog(ಧ್ರುವ ವರ್ಣ *refname, ಶೂನ್ಯ *cb_data UNUSED)
{
	printf("%s\n", refname);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmd_reflog_list(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
			   ಸಂರಚನಾ repository *repo UNUSED)
{
	ಸಂರಚನಾ option options[] = {
		OPT_END()
	};
	ಸಂರಚನಾ ref_store *ref_store;

	argc = parse_options(argc, argv, prefix, options, reflog_list_usage, 0);
	ಯದಿ (argc)
		ಪ್ರತ್ಯಾಗಮ error(_("%s does not accept arguments: '%s'"),
			     "list", argv[0]);

	ref_store = get_main_ref_store(the_repository);

	ಪ್ರತ್ಯಾಗಮ refs_for_each_reflog(ref_store, show_reflog, ರಿಕ್ತ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmd_reflog_expire(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
			     ಸಂರಚನಾ repository *repo UNUSED)
{
	timestamp_t now = time(ರಿಕ್ತ);
	ಸಂರಚನಾ reflog_expire_options opts = REFLOG_EXPIRE_OPTIONS_INIT(now);
	ಪೂರ್ಣಾಙ್ಕ i, status, do_all, single_worktree = 0;
	unsigned ಪೂರ್ಣಾಙ್ಕ flags = 0;
	ಪೂರ್ಣಾಙ್ಕ verbose = 0;
	reflog_expiry_should_prune_fn *should_prune_fn = should_expire_reflog_ent;
	ಧ್ರುವ ಸಂರಚನಾ option options[] = {
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

	ಯದಿ (verbose)
		should_prune_fn = should_expire_reflog_ent_verbose;

	/*
	 * We can trust the commits and objects reachable from refs
	 * even in older repository.  We cannot trust what's reachable
	 * from reflog if the repository was pruned with older git.
	 */
	ಯದಿ (opts.stalefix) {
		ಸಂರಚನಾ rev_info revs;

		repo_init_revisions(the_repository, &revs, prefix);
		revs.do_not_die_on_missing_objects = 1;
		revs.ignore_missing = 1;
		revs.ignore_missing_links = 1;
		ಯದಿ (verbose)
			printf(_("Marking reachable objects..."));
		mark_reachable_objects(&revs, 0, 0, ರಿಕ್ತ);
		release_revisions(&revs);
		ಯದಿ (verbose)
			putchar('\n');
	}

	ಯದಿ (do_all) {
		ಸಂರಚನಾ worktree_reflogs collected = {
			.reflogs = STRING_LIST_INIT_DUP,
		};
		ಸಂರಚನಾ string_list_item *item;
		ಸಂರಚನಾ worktree **worktrees, **p;

		worktrees = get_worktrees(the_repository);
		ಪ್ರತಿ (p = worktrees; *p; p++) {
			ಯದಿ (single_worktree && !(*p)->is_current)
				ಅನುವರ್ತ;
			collected.worktree = *p;
			refs_for_each_reflog(get_worktree_ref_store(*p),
					     collect_reflog, &collected);
		}
		free_worktrees(worktrees);

		for_each_string_list_item(item, &collected.reflogs) {
			ಸಂರಚನಾ expire_reflog_policy_cb cb = {
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

	ಪ್ರತಿ (i = 0; i < argc; i++) {
		ವರ್ಣ *ref;
		ಸಂರಚನಾ expire_reflog_policy_cb cb = { .opts = opts };

		ಯದಿ (!repo_dwim_log(the_repository, argv[i], strlen(argv[i]), ರಿಕ್ತ, &ref)) {
			status |= error(_("reflog could not be found: '%s'"), argv[i]);
			ಅನುವರ್ತ;
		}
		reflog_expire_options_set_refname(&cb.opts, ref);
		status |= refs_reflog_expire(get_main_ref_store(the_repository),
					     ref, flags,
					     reflog_expiry_prepare,
					     should_prune_fn,
					     reflog_expiry_cleanup,
					     &cb);
		ಮುಕ್ತ(ref);
	}

	reflog_clear_expire_config(&opts);

	ಪ್ರತ್ಯಾಗಮ status;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmd_reflog_delete(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
			     ಸಂರಚನಾ repository *repo UNUSED)
{
	ಪೂರ್ಣಾಙ್ಕ i, status = 0;
	unsigned ಪೂರ್ಣಾಙ್ಕ flags = 0;
	ಪೂರ್ಣಾಙ್ಕ verbose = 0;

	ಧ್ರುವ ಸಂರಚನಾ option options[] = {
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

	ಯದಿ (argc < 1)
		ಪ್ರತ್ಯಾಗಮ error(_("no reflog specified to delete"));

	ಪ್ರತಿ (i = 0; i < argc; i++)
		status |= reflog_delete(argv[i], flags, verbose);

	ಪ್ರತ್ಯಾಗಮ status;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmd_reflog_exists(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
			     ಸಂರಚನಾ repository *repo UNUSED)
{
	ಸಂರಚನಾ option options[] = {
		OPT_END()
	};
	ಧ್ರುವ ವರ್ಣ *refname;

	argc = parse_options(argc, argv, prefix, options, reflog_exists_usage,
			     0);
	ಯದಿ (!argc)
		usage_with_options(reflog_exists_usage, options);

	refname = argv[0];
	ಯದಿ (check_refname_format(refname, REFNAME_ALLOW_ONELEVEL))
		die(_("invalid ref format: %s"), refname);
	ಪ್ರತ್ಯಾಗಮ !refs_reflog_exists(get_main_ref_store(the_repository),
				   refname);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmd_reflog_drop(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
			   ಸಂರಚನಾ repository *repo)
{
	ಪೂರ್ಣಾಙ್ಕ ret = 0, do_all = 0, single_worktree = 0;
	ಧ್ರುವ ಸಂರಚನಾ option options[] = {
		OPT_BOOL(0, "all", &do_all, N_("drop the reflogs of all references")),
		OPT_BOOL(0, "single-worktree", &single_worktree,
			 N_("drop reflogs from the current worktree only")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, reflog_drop_usage, 0);

	ಯದಿ (argc && do_all)
		usage(_("references specified along with --all"));

	ಯದಿ (do_all) {
		ಸಂರಚನಾ worktree_reflogs collected = {
			.reflogs = STRING_LIST_INIT_DUP,
		};
		ಸಂರಚನಾ string_list_item *item;
		ಸಂರಚನಾ worktree **worktrees, **p;

		worktrees = get_worktrees(the_repository);
		ಪ್ರತಿ (p = worktrees; *p; p++) {
			ಯದಿ (single_worktree && !(*p)->is_current)
				ಅನುವರ್ತ;
			collected.worktree = *p;
			refs_for_each_reflog(get_worktree_ref_store(*p),
					     collect_reflog, &collected);
		}
		free_worktrees(worktrees);

		for_each_string_list_item(item, &collected.reflogs)
			ret |= refs_delete_reflog(get_main_ref_store(repo),
						     item->string);
		string_list_clear(&collected.reflogs, 0);

		ಪ್ರತ್ಯಾಗಮ ret;
	}

	ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < argc; i++) {
		ವರ್ಣ *ref;
		ಯದಿ (!repo_dwim_log(repo, argv[i], strlen(argv[i]), ರಿಕ್ತ, &ref)) {
			ret |= error(_("reflog could not be found: '%s'"), argv[i]);
			ಅನುವರ್ತ;
		}

		ret |= refs_delete_reflog(get_main_ref_store(repo), ref);
		ಮುಕ್ತ(ref);
	}

	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmd_reflog_write(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
			    ಸಂರಚನಾ repository *repo)
{
	ಧ್ರುವ ಸಂರಚನಾ option options[] = {
		OPT_END()
	};
	ಸಂರಚನಾ object_id old_oid, new_oid;
	ಸಂರಚನಾ strbuf err = STRBUF_INIT;
	ಸಂರಚನಾ ref_transaction *tx;
	ಧ್ರುವ ವರ್ಣ *ref, *message;
	ಪೂರ್ಣಾಙ್ಕ ret;

	repo_config(repo, git_ident_config, ರಿಕ್ತ);

	argc = parse_options(argc, argv, prefix, options, reflog_write_usage, 0);
	ಯದಿ (argc != 4)
		usage_with_options(reflog_write_usage, options);

	ref = argv[0];
	ಯದಿ (!is_ಬೇರು_ref(ref) && check_refname_format(ref, 0))
		die(_("invalid reference name: %s"), ref);

	ret = get_oid_hex_algop(argv[1], &old_oid, repo->hash_algo);
	ಯದಿ (ret)
		die(_("invalid old object ID: '%s'"), argv[1]);
	ಯದಿ (!is_null_oid(&old_oid) && !odb_has_object(repo->objects, &old_oid, 0))
		die(_("old object '%s' does not exist"), argv[1]);

	ret = get_oid_hex_algop(argv[2], &new_oid, repo->hash_algo);
	ಯದಿ (ret)
		die(_("invalid new object ID: '%s'"), argv[2]);
	ಯದಿ (!is_null_oid(&new_oid) && !odb_has_object(repo->objects, &new_oid, 0))
		die(_("new object '%s' does not exist"), argv[2]);

	message = argv[3];

	tx = ref_store_transaction_begin(get_main_ref_store(repo), 0, &err);
	ಯದಿ (!tx)
		die(_("cannot start transaction: %s"), err.buf);

	ret = ref_transaction_update_reflog(tx, ref, &new_oid, &old_oid,
					    git_committer_info(0),
					    message, 0, &err);
	ಯದಿ (ret)
		die(_("cannot queue reflog update: %s"), err.buf);

	ret = ref_transaction_commit(tx, &err);
	ಯದಿ (ret)
		die(_("cannot commit reflog update: %s"), err.buf);

	ref_transaction_free(tx);
	strbuf_release(&err);
	ಪ್ರತ್ಯಾಗಮ 0;
}

/*
 * main "reflog"
 */
ಪೂರ್ಣಾಙ್ಕ cmd_reflog(ಪೂರ್ಣಾಙ್ಕ argc,
	       ಧ್ರುವ ವರ್ಣ **argv,
	       ಧ್ರುವ ವರ್ಣ *prefix,
	       ಸಂರಚನಾ repository *repository)
{
	parse_opt_subcommand_fn *fn = ರಿಕ್ತ;
	ಸಂರಚನಾ option options[] = {
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
	ಯದಿ (fn)
		ಪ್ರತ್ಯಾಗಮ fn(argc - 1, argv + 1, prefix, repository);
	ಅನ್ಯಥಾ
		ಪ್ರತ್ಯಾಗಮ cmd_log_reflog(argc, argv, prefix, repository);
}
