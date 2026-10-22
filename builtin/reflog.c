#define USE_THE_REPOSITORY_VARIABLE

#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "hex.h"
#അന്തര്ഭാവ "odb.h"
#അന്തര്ഭാവ "revision.h"
#അന്തര്ഭാവ "reachable.h"
#അന്തര്ഭാവ "wildmatch.h"
#അന്തര്ഭാവ "worktree.h"
#അന്തര്ഭാവ "reflog.h"
#അന്തര്ഭാവ "refs.h"
#അന്തര്ഭാവ "parse-options.h"

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

സ്ഥിതം സ്ഥിരം അക്ഷരം *സ്ഥിരം reflog_show_usage[] = {
	BUILTIN_REFLOG_SHOW_USAGE,
	ശൂന്യ,
};

സ്ഥിതം സ്ഥിരം അക്ഷരം *സ്ഥിരം reflog_list_usage[] = {
	BUILTIN_REFLOG_LIST_USAGE,
	ശൂന്യ,
};

സ്ഥിതം സ്ഥിരം അക്ഷരം *സ്ഥിരം reflog_exists_usage[] = {
	BUILTIN_REFLOG_EXISTS_USAGE,
	ശൂന്യ,
};

സ്ഥിതം സ്ഥിരം അക്ഷരം *സ്ഥിരം reflog_write_usage[] = {
	BUILTIN_REFLOG_WRITE_USAGE,
	ശൂന്യ,
};

സ്ഥിതം സ്ഥിരം അക്ഷരം *സ്ഥിരം reflog_delete_usage[] = {
	BUILTIN_REFLOG_DELETE_USAGE,
	ശൂന്യ
};

സ്ഥിതം സ്ഥിരം അക്ഷരം *സ്ഥിരം reflog_drop_usage[] = {
	BUILTIN_REFLOG_DROP_USAGE,
	ശൂന്യ,
};

സ്ഥിതം സ്ഥിരം അക്ഷരം *സ്ഥിരം reflog_expire_usage[] = {
	BUILTIN_REFLOG_EXPIRE_USAGE,
	ശൂന്യ
};

സ്ഥിതം സ്ഥിരം അക്ഷരം *സ്ഥിരം reflog_usage[] = {
	BUILTIN_REFLOG_SHOW_USAGE,
	BUILTIN_REFLOG_LIST_USAGE,
	BUILTIN_REFLOG_EXISTS_USAGE,
	BUILTIN_REFLOG_WRITE_USAGE,
	BUILTIN_REFLOG_DELETE_USAGE,
	BUILTIN_REFLOG_DROP_USAGE,
	BUILTIN_REFLOG_EXPIRE_USAGE,
	ശൂന്യ
};

ഘടന worktree_reflogs {
	ഘടന worktree *worktree;
	ഘടന string_list reflogs;
};

സ്ഥിതം പൂർണ്ണസംഖ്യ collect_reflog(സ്ഥിരം അക്ഷരം *ref, ശൂന്യം *cb_data)
{
	ഘടന worktree_reflogs *cb = cb_data;
	ഘടന worktree *worktree = cb->worktree;
	ഘടന strbuf newref = STRBUF_INIT;

	/*
	 * Avoid collecting the same shared ref multiple times because
	 * they are available via all worktrees.
	 */
	എങ്കിൽ (!worktree->is_current &&
	    parse_worktree_ref(ref, ശൂന്യ, ശൂന്യ, ശൂന്യ) == REF_WORKTREE_SHARED)
		തിരികെ 0;

	strbuf_worktree_ref(worktree, &newref, ref);
	string_list_append_nodup(&cb->reflogs, strbuf_detach(&newref, ശൂന്യ));

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ expire_unreachable_callback(സ്ഥിരം ഘടന option *opt,
				 സ്ഥിരം അക്ഷരം *arg,
				 പൂർണ്ണസംഖ്യ unset)
{
	ഘടന reflog_expire_options *opts = opt->മൂല്യം;

	BUG_ON_OPT_NEG(unset);

	എങ്കിൽ (parse_expiry_date(arg, &opts->expire_unreachable))
		die(_("invalid timestamp '%s' given to '--%s'"),
		    arg, opt->long_name);

	opts->explicit_expiry |= REFLOG_EXPIRE_UNREACH;
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ expire_total_callback(സ്ഥിരം ഘടന option *opt,
				 സ്ഥിരം അക്ഷരം *arg,
				 പൂർണ്ണസംഖ്യ unset)
{
	ഘടന reflog_expire_options *opts = opt->മൂല്യം;

	BUG_ON_OPT_NEG(unset);

	എങ്കിൽ (parse_expiry_date(arg, &opts->expire_total))
		die(_("invalid timestamp '%s' given to '--%s'"),
		    arg, opt->long_name);

	opts->explicit_expiry |= REFLOG_EXPIRE_TOTAL;
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cmd_reflog_show(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
			   ഘടന repository *repo UNUSED)
{
	ഘടന option options[] = {
		OPT_END()
	};

	parse_options(argc, argv, prefix, options, reflog_show_usage,
		      PARSE_OPT_KEEP_DASHDASH | PARSE_OPT_KEEP_ARGV0 |
		      PARSE_OPT_KEEP_UNKNOWN_OPT);

	തിരികെ cmd_log_reflog(argc, argv, prefix, the_repository);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ show_reflog(സ്ഥിരം അക്ഷരം *refname, ശൂന്യം *cb_data UNUSED)
{
	printf("%s\n", refname);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cmd_reflog_list(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
			   ഘടന repository *repo UNUSED)
{
	ഘടന option options[] = {
		OPT_END()
	};
	ഘടന ref_store *ref_store;

	argc = parse_options(argc, argv, prefix, options, reflog_list_usage, 0);
	എങ്കിൽ (argc)
		തിരികെ error(_("%s does not accept arguments: '%s'"),
			     "list", argv[0]);

	ref_store = get_main_ref_store(the_repository);

	തിരികെ refs_for_each_reflog(ref_store, show_reflog, ശൂന്യ);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cmd_reflog_expire(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
			     ഘടന repository *repo UNUSED)
{
	timestamp_t now = time(ശൂന്യ);
	ഘടന reflog_expire_options opts = REFLOG_EXPIRE_OPTIONS_INIT(now);
	പൂർണ്ണസംഖ്യ i, status, do_all, single_worktree = 0;
	unsigned പൂർണ്ണസംഖ്യ flags = 0;
	പൂർണ്ണസംഖ്യ verbose = 0;
	reflog_expiry_should_prune_fn *should_prune_fn = should_expire_reflog_ent;
	സ്ഥിരം ഘടന option options[] = {
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

	എങ്കിൽ (verbose)
		should_prune_fn = should_expire_reflog_ent_verbose;

	/*
	 * We can trust the commits and objects reachable from refs
	 * even in older repository.  We cannot trust what's reachable
	 * from reflog if the repository was pruned with older git.
	 */
	എങ്കിൽ (opts.stalefix) {
		ഘടന rev_info revs;

		repo_init_revisions(the_repository, &revs, prefix);
		revs.do_not_die_on_missing_objects = 1;
		revs.ignore_missing = 1;
		revs.ignore_missing_links = 1;
		എങ്കിൽ (verbose)
			printf(_("Marking reachable objects..."));
		mark_reachable_objects(&revs, 0, 0, ശൂന്യ);
		release_revisions(&revs);
		എങ്കിൽ (verbose)
			putchar('\n');
	}

	എങ്കിൽ (do_all) {
		ഘടന worktree_reflogs collected = {
			.reflogs = STRING_LIST_INIT_DUP,
		};
		ഘടന string_list_item *item;
		ഘടന worktree **worktrees, **p;

		worktrees = get_worktrees(the_repository);
		വേണ്ടി (p = worktrees; *p; p++) {
			എങ്കിൽ (single_worktree && !(*p)->is_current)
				തുടരുക;
			collected.worktree = *p;
			refs_for_each_reflog(get_worktree_ref_store(*p),
					     collect_reflog, &collected);
		}
		free_worktrees(worktrees);

		for_each_string_list_item(item, &collected.reflogs) {
			ഘടന expire_reflog_policy_cb cb = {
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

	വേണ്ടി (i = 0; i < argc; i++) {
		അക്ഷരം *ref;
		ഘടന expire_reflog_policy_cb cb = { .opts = opts };

		എങ്കിൽ (!repo_dwim_log(the_repository, argv[i], strlen(argv[i]), ശൂന്യ, &ref)) {
			status |= error(_("reflog could not be found: '%s'"), argv[i]);
			തുടരുക;
		}
		reflog_expire_options_set_refname(&cb.opts, ref);
		status |= refs_reflog_expire(get_main_ref_store(the_repository),
					     ref, flags,
					     reflog_expiry_prepare,
					     should_prune_fn,
					     reflog_expiry_cleanup,
					     &cb);
		സ്വതന്ത്രം(ref);
	}

	reflog_clear_expire_config(&opts);

	തിരികെ status;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cmd_reflog_delete(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
			     ഘടന repository *repo UNUSED)
{
	പൂർണ്ണസംഖ്യ i, status = 0;
	unsigned പൂർണ്ണസംഖ്യ flags = 0;
	പൂർണ്ണസംഖ്യ verbose = 0;

	സ്ഥിരം ഘടന option options[] = {
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

	എങ്കിൽ (argc < 1)
		തിരികെ error(_("no reflog specified to delete"));

	വേണ്ടി (i = 0; i < argc; i++)
		status |= reflog_delete(argv[i], flags, verbose);

	തിരികെ status;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cmd_reflog_exists(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
			     ഘടന repository *repo UNUSED)
{
	ഘടന option options[] = {
		OPT_END()
	};
	സ്ഥിരം അക്ഷരം *refname;

	argc = parse_options(argc, argv, prefix, options, reflog_exists_usage,
			     0);
	എങ്കിൽ (!argc)
		usage_with_options(reflog_exists_usage, options);

	refname = argv[0];
	എങ്കിൽ (check_refname_format(refname, REFNAME_ALLOW_ONELEVEL))
		die(_("invalid ref format: %s"), refname);
	തിരികെ !refs_reflog_exists(get_main_ref_store(the_repository),
				   refname);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cmd_reflog_drop(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
			   ഘടന repository *repo)
{
	പൂർണ്ണസംഖ്യ ret = 0, do_all = 0, single_worktree = 0;
	സ്ഥിരം ഘടന option options[] = {
		OPT_BOOL(0, "all", &do_all, N_("drop the reflogs of all references")),
		OPT_BOOL(0, "single-worktree", &single_worktree,
			 N_("drop reflogs from the current worktree only")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, reflog_drop_usage, 0);

	എങ്കിൽ (argc && do_all)
		usage(_("references specified along with --all"));

	എങ്കിൽ (do_all) {
		ഘടന worktree_reflogs collected = {
			.reflogs = STRING_LIST_INIT_DUP,
		};
		ഘടന string_list_item *item;
		ഘടന worktree **worktrees, **p;

		worktrees = get_worktrees(the_repository);
		വേണ്ടി (p = worktrees; *p; p++) {
			എങ്കിൽ (single_worktree && !(*p)->is_current)
				തുടരുക;
			collected.worktree = *p;
			refs_for_each_reflog(get_worktree_ref_store(*p),
					     collect_reflog, &collected);
		}
		free_worktrees(worktrees);

		for_each_string_list_item(item, &collected.reflogs)
			ret |= refs_delete_reflog(get_main_ref_store(repo),
						     item->string);
		string_list_clear(&collected.reflogs, 0);

		തിരികെ ret;
	}

	വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < argc; i++) {
		അക്ഷരം *ref;
		എങ്കിൽ (!repo_dwim_log(repo, argv[i], strlen(argv[i]), ശൂന്യ, &ref)) {
			ret |= error(_("reflog could not be found: '%s'"), argv[i]);
			തുടരുക;
		}

		ret |= refs_delete_reflog(get_main_ref_store(repo), ref);
		സ്വതന്ത്രം(ref);
	}

	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cmd_reflog_write(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
			    ഘടന repository *repo)
{
	സ്ഥിരം ഘടന option options[] = {
		OPT_END()
	};
	ഘടന object_id old_oid, new_oid;
	ഘടന strbuf err = STRBUF_INIT;
	ഘടന ref_transaction *tx;
	സ്ഥിരം അക്ഷരം *ref, *message;
	പൂർണ്ണസംഖ്യ ret;

	repo_config(repo, git_ident_config, ശൂന്യ);

	argc = parse_options(argc, argv, prefix, options, reflog_write_usage, 0);
	എങ്കിൽ (argc != 4)
		usage_with_options(reflog_write_usage, options);

	ref = argv[0];
	എങ്കിൽ (!is_വേര്_ref(ref) && check_refname_format(ref, 0))
		die(_("invalid reference name: %s"), ref);

	ret = get_oid_hex_algop(argv[1], &old_oid, repo->hash_algo);
	എങ്കിൽ (ret)
		die(_("invalid old object ID: '%s'"), argv[1]);
	എങ്കിൽ (!is_null_oid(&old_oid) && !odb_has_object(repo->objects, &old_oid, 0))
		die(_("old object '%s' does not exist"), argv[1]);

	ret = get_oid_hex_algop(argv[2], &new_oid, repo->hash_algo);
	എങ്കിൽ (ret)
		die(_("invalid new object ID: '%s'"), argv[2]);
	എങ്കിൽ (!is_null_oid(&new_oid) && !odb_has_object(repo->objects, &new_oid, 0))
		die(_("new object '%s' does not exist"), argv[2]);

	message = argv[3];

	tx = ref_store_transaction_begin(get_main_ref_store(repo), 0, &err);
	എങ്കിൽ (!tx)
		die(_("cannot start transaction: %s"), err.buf);

	ret = ref_transaction_update_reflog(tx, ref, &new_oid, &old_oid,
					    git_committer_info(0),
					    message, 0, &err);
	എങ്കിൽ (ret)
		die(_("cannot queue reflog update: %s"), err.buf);

	ret = ref_transaction_commit(tx, &err);
	എങ്കിൽ (ret)
		die(_("cannot commit reflog update: %s"), err.buf);

	ref_transaction_free(tx);
	strbuf_release(&err);
	തിരികെ 0;
}

/*
 * main "reflog"
 */
പൂർണ്ണസംഖ്യ cmd_reflog(പൂർണ്ണസംഖ്യ argc,
	       സ്ഥിരം അക്ഷരം **argv,
	       സ്ഥിരം അക്ഷരം *prefix,
	       ഘടന repository *repository)
{
	parse_opt_subcommand_fn *fn = ശൂന്യ;
	ഘടന option options[] = {
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
	എങ്കിൽ (fn)
		തിരികെ fn(argc - 1, argv + 1, prefix, repository);
	അല്ലെങ്കിൽ
		തിരികെ cmd_log_reflog(argc, argv, prefix, repository);
}
