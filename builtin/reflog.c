#నిర్వచించు USE_THE_REPOSITORY_VARIABLE

#అన్తర్భావ "builtin.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "hex.h"
#అన్తర్భావ "odb.h"
#అన్తర్భావ "revision.h"
#అన్తర్భావ "reachable.h"
#అన్తర్భావ "wildmatch.h"
#అన్తర్భావ "worktree.h"
#అన్తర్భావ "reflog.h"
#అన్తర్భావ "refs.h"
#అన్తర్భావ "parse-options.h"

#నిర్వచించు BUILTIN_REFLOG_SHOW_USAGE \
	N_("git reflog [show] [<log-options>] [<ref>]")

#నిర్వచించు BUILTIN_REFLOG_LIST_USAGE \
	N_("git reflog list")

#నిర్వచించు BUILTIN_REFLOG_EXISTS_USAGE \
	N_("git reflog exists <ref>")

#నిర్వచించు BUILTIN_REFLOG_WRITE_USAGE \
	N_("git reflog write <ref> <old-oid> <new-oid> <message>")

#నిర్వచించు BUILTIN_REFLOG_DELETE_USAGE \
	N_("git reflog delete [--rewrite] [--updateref]\n" \
	   "                  [--dry-run | -n] [--verbose] <ref>@{<specifier>}...")

#నిర్వచించు BUILTIN_REFLOG_DROP_USAGE \
	N_("git reflog drop [--all [--single-worktree] | <refs>...]")

#నిర్వచించు BUILTIN_REFLOG_EXPIRE_USAGE \
	N_("git reflog expire [--expire=<time>] [--expire-unreachable=<time>]\n" \
	   "                  [--rewrite] [--updateref] [--stale-fix]\n" \
	   "                  [--dry-run | -n] [--verbose] [--all [--single-worktree] | <refs>...]")

స్థిర స్థిరరాశి అక్షరం *స్థిరరాశి reflog_show_usage[] = {
	BUILTIN_REFLOG_SHOW_USAGE,
	ఖాళీ,
};

స్థిర స్థిరరాశి అక్షరం *స్థిరరాశి reflog_list_usage[] = {
	BUILTIN_REFLOG_LIST_USAGE,
	ఖాళీ,
};

స్థిర స్థిరరాశి అక్షరం *స్థిరరాశి reflog_exists_usage[] = {
	BUILTIN_REFLOG_EXISTS_USAGE,
	ఖాళీ,
};

స్థిర స్థిరరాశి అక్షరం *స్థిరరాశి reflog_write_usage[] = {
	BUILTIN_REFLOG_WRITE_USAGE,
	ఖాళీ,
};

స్థిర స్థిరరాశి అక్షరం *స్థిరరాశి reflog_delete_usage[] = {
	BUILTIN_REFLOG_DELETE_USAGE,
	ఖాళీ
};

స్థిర స్థిరరాశి అక్షరం *స్థిరరాశి reflog_drop_usage[] = {
	BUILTIN_REFLOG_DROP_USAGE,
	ఖాళీ,
};

స్థిర స్థిరరాశి అక్షరం *స్థిరరాశి reflog_expire_usage[] = {
	BUILTIN_REFLOG_EXPIRE_USAGE,
	ఖాళీ
};

స్థిర స్థిరరాశి అక్షరం *స్థిరరాశి reflog_usage[] = {
	BUILTIN_REFLOG_SHOW_USAGE,
	BUILTIN_REFLOG_LIST_USAGE,
	BUILTIN_REFLOG_EXISTS_USAGE,
	BUILTIN_REFLOG_WRITE_USAGE,
	BUILTIN_REFLOG_DELETE_USAGE,
	BUILTIN_REFLOG_DROP_USAGE,
	BUILTIN_REFLOG_EXPIRE_USAGE,
	ఖాళీ
};

నిర్మాణం worktree_reflogs {
	నిర్మాణం worktree *worktree;
	నిర్మాణం string_list reflogs;
};

స్థిర పూర్ణాంకం collect_reflog(స్థిరరాశి అక్షరం *ref, శూన్య *cb_దత్తాంశం)
{
	నిర్మాణం worktree_reflogs *cb = cb_దత్తాంశం;
	నిర్మాణం worktree *worktree = cb->worktree;
	నిర్మాణం strbuf newref = STRBUF_INIT;

	/*
	 * Avoid collecting the same shared ref multiple times because
	 * they are available via all worktrees.
	 */
	ఒకవేళ (!worktree->is_current &&
	    parse_worktree_ref(ref, ఖాళీ, ఖాళీ, ఖాళీ) == REF_WORKTREE_SHARED)
		తిరిగి 0;

	strbuf_worktree_ref(worktree, &newref, ref);
	string_list_append_nodup(&cb->reflogs, strbuf_detach(&newref, ఖాళీ));

	తిరిగి 0;
}

స్థిర పూర్ణాంకం expire_unreachable_callback(స్థిరరాశి నిర్మాణం option *opt,
				 స్థిరరాశి అక్షరం *arg,
				 పూర్ణాంకం unset)
{
	నిర్మాణం reflog_expire_options *opts = opt->విలువ;

	BUG_ON_OPT_NEG(unset);

	ఒకవేళ (parse_expiry_date(arg, &opts->expire_unreachable))
		die(_("invalid timestamp '%s' given to '--%s'"),
		    arg, opt->long_name);

	opts->explicit_expiry |= REFLOG_EXPIRE_UNREACH;
	తిరిగి 0;
}

స్థిర పూర్ణాంకం expire_మొత్తం_callback(స్థిరరాశి నిర్మాణం option *opt,
				 స్థిరరాశి అక్షరం *arg,
				 పూర్ణాంకం unset)
{
	నిర్మాణం reflog_expire_options *opts = opt->విలువ;

	BUG_ON_OPT_NEG(unset);

	ఒకవేళ (parse_expiry_date(arg, &opts->expire_మొత్తం))
		die(_("invalid timestamp '%s' given to '--%s'"),
		    arg, opt->long_name);

	opts->explicit_expiry |= REFLOG_EXPIRE_మొత్తం;
	తిరిగి 0;
}

స్థిర పూర్ణాంకం cmd_reflog_show(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
			   నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం option options[] = {
		OPT_చివర()
	};

	parse_options(argc, argv, ఉపసర్గ, options, reflog_show_usage,
		      PARSE_OPT_KEEP_DASHDASH | PARSE_OPT_KEEP_ARGV0 |
		      PARSE_OPT_KEEP_UNKNOWN_OPT);

	తిరిగి cmd_log_reflog(argc, argv, ఉపసర్గ, the_repository);
}

స్థిర పూర్ణాంకం show_reflog(స్థిరరాశి అక్షరం *refname, శూన్య *cb_దత్తాంశం వాడని)
{
	రాయి("%s\n", refname);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం cmd_reflog_list(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
			   నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం option options[] = {
		OPT_చివర()
	};
	నిర్మాణం ref_store *ref_store;

	argc = parse_options(argc, argv, ఉపసర్గ, options, reflog_list_usage, 0);
	ఒకవేళ (argc)
		తిరిగి error(_("%s does not accept arguments: '%s'"),
			     "list", argv[0]);

	ref_store = get_main_ref_store(the_repository);

	తిరిగి refs_for_each_reflog(ref_store, show_reflog, ఖాళీ);
}

స్థిర పూర్ణాంకం cmd_reflog_expire(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
			     నిర్మాణం repository *repo వాడని)
{
	timestamp_t now = సమయం(ఖాళీ);
	నిర్మాణం reflog_expire_options opts = REFLOG_EXPIRE_OPTIONS_INIT(now);
	పూర్ణాంకం i, స్థితి_2, do_all, single_worktree = 0;
	గుర్తులేని పూర్ణాంకం flags = 0;
	పూర్ణాంకం verbose = 0;
	reflog_expiry_should_prune_fn *should_prune_fn = should_expire_reflog_ent;
	స్థిరరాశి నిర్మాణం option options[] = {
		OPT_బిట్టు('n', "dry-run", &flags, N_("do not actually prune any entries"),
			EXPIRE_REFLOGS_DRY_నిరంతరం),
		OPT_బిట్టు(0, "rewrite", &flags,
			N_("rewrite the old SHA1 with the new SHA1 of the entry that now precedes it"),
			EXPIRE_REFLOGS_REWRITE),
		OPT_బిట్టు(0, "updateref", &flags,
			N_("update the reference to the value of the top reflog entry"),
			EXPIRE_REFLOGS_UPDATE_REF),
		OPT_BOOL(0, "verbose", &verbose, N_("print extra information on screen")),
		OPT_CALLBACK_F(0, "expire", &opts, N_("timestamp"),
			       N_("prune entries older than the specified time"),
			       PARSE_OPT_NONEG,
			       expire_మొత్తం_callback),
		OPT_CALLBACK_F(0, "expire-unreachable", &opts, N_("timestamp"),
			       N_("prune entries older than <time> that are not reachable from the current tip of the branch"),
			       PARSE_OPT_NONEG,
			       expire_unreachable_callback),
		OPT_BOOL(0, "stale-fix", &opts.stalefix,
			 N_("prune any reflog entries that point to broken commits")),
		OPT_BOOL(0, "all", &do_all, N_("process the reflogs of all references")),
		OPT_BOOL(0, "single-worktree", &single_worktree,
			 N_("limits processing to reflogs from the current worktree only")),
		OPT_చివర()
	};

	repo_config(the_repository, reflog_expire_config, &opts);

	save_commit_బఫరు = 0;
	do_all = స్థితి_2 = 0;

	argc = parse_options(argc, argv, ఉపసర్గ, options, reflog_expire_usage, 0);

	ఒకవేళ (verbose)
		should_prune_fn = should_expire_reflog_ent_verbose;

	/*
	 * We can trust the commits and objects reachable from refs
	 * even in older repository.  We cannot trust what's reachable
	 * from reflog if the repository was pruned with older git.
	 */
	ఒకవేళ (opts.stalefix) {
		నిర్మాణం rev_info revs;

		repo_init_revisions(the_repository, &revs, ఉపసర్గ);
		revs.do_not_die_on_missing_objects = 1;
		revs.ignore_missing = 1;
		revs.ignore_missing_links = 1;
		ఒకవేళ (verbose)
			రాయి(_("Marking reachable objects..."));
		mark_reachable_objects(&revs, 0, 0, ఖాళీ);
		release_revisions(&revs);
		ఒకవేళ (verbose)
			అక్షరం_రాయి('\n');
	}

	ఒకవేళ (do_all) {
		నిర్మాణం worktree_reflogs collected = {
			.reflogs = STRING_LIST_INIT_DUP,
		};
		నిర్మాణం string_list_అంశం *అంశం;
		నిర్మాణం worktree **worktrees, **p;

		worktrees = get_worktrees(the_repository);
		కొరకు (p = worktrees; *p; p++) {
			ఒకవేళ (single_worktree && !(*p)->is_current)
				కొనసాగు;
			collected.worktree = *p;
			refs_for_each_reflog(get_worktree_ref_store(*p),
					     collect_reflog, &collected);
		}
		free_worktrees(worktrees);

		for_each_string_list_అంశం(అంశం, &collected.reflogs) {
			నిర్మాణం expire_reflog_policy_cb cb = {
				.opts = opts,
				.dry_నిరంతరం = !!(flags & EXPIRE_REFLOGS_DRY_నిరంతరం),
			};

			reflog_expire_options_సమితి_refname(&cb.opts,  అంశం->string);
			స్థితి_2 |= refs_reflog_expire(get_main_ref_store(the_repository),
						     అంశం->string, flags,
						     reflog_expiry_prepare,
						     should_prune_fn,
						     reflog_expiry_cleanup,
						     &cb);
		}
		string_list_clear(&collected.reflogs, 0);
	}

	కొరకు (i = 0; i < argc; i++) {
		అక్షరం *ref;
		నిర్మాణం expire_reflog_policy_cb cb = { .opts = opts };

		ఒకవేళ (!repo_dwim_log(the_repository, argv[i], అక్షరశ్రేణి_పొడవు(argv[i]), ఖాళీ, &ref)) {
			స్థితి_2 |= error(_("reflog could not be found: '%s'"), argv[i]);
			కొనసాగు;
		}
		reflog_expire_options_సమితి_refname(&cb.opts, ref);
		స్థితి_2 |= refs_reflog_expire(get_main_ref_store(the_repository),
					     ref, flags,
					     reflog_expiry_prepare,
					     should_prune_fn,
					     reflog_expiry_cleanup,
					     &cb);
		విడుదల(ref);
	}

	reflog_clear_expire_config(&opts);

	తిరిగి స్థితి_2;
}

స్థిర పూర్ణాంకం cmd_reflog_delete(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
			     నిర్మాణం repository *repo వాడని)
{
	పూర్ణాంకం i, స్థితి_2 = 0;
	గుర్తులేని పూర్ణాంకం flags = 0;
	పూర్ణాంకం verbose = 0;

	స్థిరరాశి నిర్మాణం option options[] = {
		OPT_బిట్టు('n', "dry-run", &flags, N_("do not actually prune any entries"),
			EXPIRE_REFLOGS_DRY_నిరంతరం),
		OPT_బిట్టు(0, "rewrite", &flags,
			N_("rewrite the old SHA1 with the new SHA1 of the entry that now precedes it"),
			EXPIRE_REFLOGS_REWRITE),
		OPT_బిట్టు(0, "updateref", &flags,
			N_("update the reference to the value of the top reflog entry"),
			EXPIRE_REFLOGS_UPDATE_REF),
		OPT_BOOL(0, "verbose", &verbose, N_("print extra information on screen")),
		OPT_చివర()
	};

	argc = parse_options(argc, argv, ఉపసర్గ, options, reflog_delete_usage, 0);

	ఒకవేళ (argc < 1)
		తిరిగి error(_("no reflog specified to delete"));

	కొరకు (i = 0; i < argc; i++)
		స్థితి_2 |= reflog_delete(argv[i], flags, verbose);

	తిరిగి స్థితి_2;
}

స్థిర పూర్ణాంకం cmd_reflog_exists(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
			     నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం option options[] = {
		OPT_చివర()
	};
	స్థిరరాశి అక్షరం *refname;

	argc = parse_options(argc, argv, ఉపసర్గ, options, reflog_exists_usage,
			     0);
	ఒకవేళ (!argc)
		usage_with_options(reflog_exists_usage, options);

	refname = argv[0];
	ఒకవేళ (check_refname_format(refname, REFNAME_ALLOW_ONELEVEL))
		die(_("invalid ref format: %s"), refname);
	తిరిగి !refs_reflog_exists(get_main_ref_store(the_repository),
				   refname);
}

స్థిర పూర్ణాంకం cmd_reflog_drop(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
			   నిర్మాణం repository *repo)
{
	పూర్ణాంకం ret = 0, do_all = 0, single_worktree = 0;
	స్థిరరాశి నిర్మాణం option options[] = {
		OPT_BOOL(0, "all", &do_all, N_("drop the reflogs of all references")),
		OPT_BOOL(0, "single-worktree", &single_worktree,
			 N_("drop reflogs from the current worktree only")),
		OPT_చివర()
	};

	argc = parse_options(argc, argv, ఉపసర్గ, options, reflog_drop_usage, 0);

	ఒకవేళ (argc && do_all)
		usage(_("references specified along with --all"));

	ఒకవేళ (do_all) {
		నిర్మాణం worktree_reflogs collected = {
			.reflogs = STRING_LIST_INIT_DUP,
		};
		నిర్మాణం string_list_అంశం *అంశం;
		నిర్మాణం worktree **worktrees, **p;

		worktrees = get_worktrees(the_repository);
		కొరకు (p = worktrees; *p; p++) {
			ఒకవేళ (single_worktree && !(*p)->is_current)
				కొనసాగు;
			collected.worktree = *p;
			refs_for_each_reflog(get_worktree_ref_store(*p),
					     collect_reflog, &collected);
		}
		free_worktrees(worktrees);

		for_each_string_list_అంశం(అంశం, &collected.reflogs)
			ret |= refs_delete_reflog(get_main_ref_store(repo),
						     అంశం->string);
		string_list_clear(&collected.reflogs, 0);

		తిరిగి ret;
	}

	కొరకు (పూర్ణాంకం i = 0; i < argc; i++) {
		అక్షరం *ref;
		ఒకవేళ (!repo_dwim_log(repo, argv[i], అక్షరశ్రేణి_పొడవు(argv[i]), ఖాళీ, &ref)) {
			ret |= error(_("reflog could not be found: '%s'"), argv[i]);
			కొనసాగు;
		}

		ret |= refs_delete_reflog(get_main_ref_store(repo), ref);
		విడుదల(ref);
	}

	తిరిగి ret;
}

స్థిర పూర్ణాంకం cmd_reflog_write(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
			    నిర్మాణం repository *repo)
{
	స్థిరరాశి నిర్మాణం option options[] = {
		OPT_చివర()
	};
	నిర్మాణం object_id old_oid, new_oid;
	నిర్మాణం strbuf err = STRBUF_INIT;
	నిర్మాణం ref_transaction *tx;
	స్థిరరాశి అక్షరం *ref, *సందేశం_2;
	పూర్ణాంకం ret;

	repo_config(repo, git_ident_config, ఖాళీ);

	argc = parse_options(argc, argv, ఉపసర్గ, options, reflog_write_usage, 0);
	ఒకవేళ (argc != 4)
		usage_with_options(reflog_write_usage, options);

	ref = argv[0];
	ఒకవేళ (!is_మూలం_ref(ref) && check_refname_format(ref, 0))
		die(_("invalid reference name: %s"), ref);

	ret = get_oid_hex_algop(argv[1], &old_oid, repo->hash_algo);
	ఒకవేళ (ret)
		die(_("invalid old object ID: '%s'"), argv[1]);
	ఒకవేళ (!is_null_oid(&old_oid) && !odb_has_object(repo->objects, &old_oid, 0))
		die(_("old object '%s' does not exist"), argv[1]);

	ret = get_oid_hex_algop(argv[2], &new_oid, repo->hash_algo);
	ఒకవేళ (ret)
		die(_("invalid new object ID: '%s'"), argv[2]);
	ఒకవేళ (!is_null_oid(&new_oid) && !odb_has_object(repo->objects, &new_oid, 0))
		die(_("new object '%s' does not exist"), argv[2]);

	సందేశం_2 = argv[3];

	tx = ref_store_transaction_begin(get_main_ref_store(repo), 0, &err);
	ఒకవేళ (!tx)
		die(_("cannot start transaction: %s"), err.buf);

	ret = ref_transaction_update_reflog(tx, ref, &new_oid, &old_oid,
					    git_committer_info(0),
					    సందేశం_2, 0, &err);
	ఒకవేళ (ret)
		die(_("cannot queue reflog update: %s"), err.buf);

	ret = ref_transaction_commit(tx, &err);
	ఒకవేళ (ret)
		die(_("cannot commit reflog update: %s"), err.buf);

	ref_transaction_free(tx);
	strbuf_release(&err);
	తిరిగి 0;
}

/*
 * main "reflog"
 */
పూర్ణాంకం cmd_reflog(పూర్ణాంకం argc,
	       స్థిరరాశి అక్షరం **argv,
	       స్థిరరాశి అక్షరం *ఉపసర్గ,
	       నిర్మాణం repository *repository)
{
	parse_opt_subcommand_fn *fn = ఖాళీ;
	నిర్మాణం option options[] = {
		OPT_SUBCOMMAND("show", &fn, cmd_reflog_show),
		OPT_SUBCOMMAND("list", &fn, cmd_reflog_list),
		OPT_SUBCOMMAND("exists", &fn, cmd_reflog_exists),
		OPT_SUBCOMMAND("write", &fn, cmd_reflog_write),
		OPT_SUBCOMMAND("delete", &fn, cmd_reflog_delete),
		OPT_SUBCOMMAND("drop", &fn, cmd_reflog_drop),
		OPT_SUBCOMMAND("expire", &fn, cmd_reflog_expire),
		OPT_చివర()
	};

	argc = parse_options(argc, argv, ఉపసర్గ, options, reflog_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL |
			     PARSE_OPT_KEEP_DASHDASH | PARSE_OPT_KEEP_ARGV0 |
			     PARSE_OPT_KEEP_UNKNOWN_OPT);
	ఒకవేళ (fn)
		తిరిగి fn(argc - 1, argv + 1, ఉపసర్గ, repository);
	లేకపోతే
		తిరిగి cmd_log_reflog(argc, argv, ఉపసర్గ, repository);
}
