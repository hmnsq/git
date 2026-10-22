#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hex.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "odb.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "revision.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "reachable.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "wildmatch.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "worktree.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "reflog.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "refs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BUILTIN_REFLOG_SHOW_USAGE \
	N_("git reflog [show] [<log-options>] [<ref>]")

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BUILTIN_REFLOG_LIST_USAGE \
	N_("git reflog list")

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BUILTIN_REFLOG_EXISTS_USAGE \
	N_("git reflog exists <ref>")

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BUILTIN_REFLOG_WRITE_USAGE \
	N_("git reflog write <ref> <old-oid> <new-oid> <message>")

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BUILTIN_REFLOG_DELETE_USAGE \
	N_("git reflog delete [--rewrite] [--updateref]\n" \
	   "                  [--dry-run | -n] [--verbose] <ref>@{<specifier>}...")

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BUILTIN_REFLOG_DROP_USAGE \
	N_("git reflog drop [--all [--single-worktree] | <refs>...]")

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BUILTIN_REFLOG_EXPIRE_USAGE \
	N_("git reflog expire [--expire=<time>] [--expire-unreachable=<time>]\n" \
	   "                  [--rewrite] [--updateref] [--stale-fix]\n" \
	   "                  [--dry-run | -n] [--verbose] [--all [--single-worktree] | <refs>...]")

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਅਚਲ reflog_show_usage[] = {
	BUILTIN_REFLOG_SHOW_USAGE,
	ਨਿਰਾ,
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਅਚਲ reflog_list_usage[] = {
	BUILTIN_REFLOG_LIST_USAGE,
	ਨਿਰਾ,
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਅਚਲ reflog_exists_usage[] = {
	BUILTIN_REFLOG_EXISTS_USAGE,
	ਨਿਰਾ,
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਅਚਲ reflog_write_usage[] = {
	BUILTIN_REFLOG_WRITE_USAGE,
	ਨਿਰਾ,
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਅਚਲ reflog_delete_usage[] = {
	BUILTIN_REFLOG_DELETE_USAGE,
	ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਅਚਲ reflog_drop_usage[] = {
	BUILTIN_REFLOG_DROP_USAGE,
	ਨਿਰਾ,
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਅਚਲ reflog_expire_usage[] = {
	BUILTIN_REFLOG_EXPIRE_USAGE,
	ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਅਚਲ reflog_usage[] = {
	BUILTIN_REFLOG_SHOW_USAGE,
	BUILTIN_REFLOG_LIST_USAGE,
	BUILTIN_REFLOG_EXISTS_USAGE,
	BUILTIN_REFLOG_WRITE_USAGE,
	BUILTIN_REFLOG_DELETE_USAGE,
	BUILTIN_REFLOG_DROP_USAGE,
	BUILTIN_REFLOG_EXPIRE_USAGE,
	ਨਿਰਾ
};

ਬਣਤਰ worktree_reflogs {
	ਬਣਤਰ worktree *worktree;
	ਬਣਤਰ string_list reflogs;
};

ਸਥਿਰ ਪੂਰਨ collect_reflog(ਅਚਲ ਅੱਖਰ *ref, ਖਾਲੀ *cb_ਅੰਕੜੇ)
{
	ਬਣਤਰ worktree_reflogs *cb = cb_ਅੰਕੜੇ;
	ਬਣਤਰ worktree *worktree = cb->worktree;
	ਬਣਤਰ strbuf newref = STRBUF_INIT;

	/*
	 * Avoid collecting the same shared ref multiple times because
	 * they are available via all worktrees.
	 */
	ਜੇ (!worktree->is_current &&
	    parse_worktree_ref(ref, ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ) == REF_WORKTREE_SHARED)
		ਵਾਪਸ 0;

	strbuf_worktree_ref(worktree, &newref, ref);
	string_list_append_nodup(&cb->reflogs, strbuf_detach(&newref, ਨਿਰਾ));

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ expire_unreachable_callback(ਅਚਲ ਬਣਤਰ option *opt,
				 ਅਚਲ ਅੱਖਰ *arg,
				 ਪੂਰਨ unset)
{
	ਬਣਤਰ reflog_expire_options *opts = opt->ਮੁੱਲ;

	BUG_ON_OPT_NEG(unset);

	ਜੇ (parse_expiry_date(arg, &opts->expire_unreachable))
		die(_("invalid timestamp '%s' given to '--%s'"),
		    arg, opt->long_name);

	opts->explicit_expiry |= REFLOG_EXPIRE_UNREACH;
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ expire_ਕੁੱਲ_callback(ਅਚਲ ਬਣਤਰ option *opt,
				 ਅਚਲ ਅੱਖਰ *arg,
				 ਪੂਰਨ unset)
{
	ਬਣਤਰ reflog_expire_options *opts = opt->ਮੁੱਲ;

	BUG_ON_OPT_NEG(unset);

	ਜੇ (parse_expiry_date(arg, &opts->expire_ਕੁੱਲ))
		die(_("invalid timestamp '%s' given to '--%s'"),
		    arg, opt->long_name);

	opts->explicit_expiry |= REFLOG_EXPIRE_ਕੁੱਲ;
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ cmd_reflog_show(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			   ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ option options[] = {
		OPT_ਸਿਰਾ()
	};

	parse_options(argc, argv, ਅਗੇਤਰ, options, reflog_show_usage,
		      PARSE_OPT_KEEP_DASHDASH | PARSE_OPT_KEEP_ARGV0 |
		      PARSE_OPT_KEEP_UNKNOWN_OPT);

	ਵਾਪਸ cmd_log_reflog(argc, argv, ਅਗੇਤਰ, the_repository);
}

ਸਥਿਰ ਪੂਰਨ show_reflog(ਅਚਲ ਅੱਖਰ *refname, ਖਾਲੀ *cb_ਅੰਕੜੇ ਅਣਵਰਤਿਆ)
{
	ਲਿਖੋ("%s\n", refname);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ cmd_reflog_list(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			   ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ option options[] = {
		OPT_ਸਿਰਾ()
	};
	ਬਣਤਰ ref_store *ref_store;

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, reflog_list_usage, 0);
	ਜੇ (argc)
		ਵਾਪਸ error(_("%s does not accept arguments: '%s'"),
			     "list", argv[0]);

	ref_store = get_main_ref_store(the_repository);

	ਵਾਪਸ refs_for_each_reflog(ref_store, show_reflog, ਨਿਰਾ);
}

ਸਥਿਰ ਪੂਰਨ cmd_reflog_expire(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			     ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	timestamp_t now = ਸਮਾਂ(ਨਿਰਾ);
	ਬਣਤਰ reflog_expire_options opts = REFLOG_EXPIRE_OPTIONS_INIT(now);
	ਪੂਰਨ i, ਸਥਿਤੀ, do_all, single_worktree = 0;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags = 0;
	ਪੂਰਨ verbose = 0;
	reflog_expiry_should_prune_fn *should_prune_fn = should_expire_reflog_ent;
	ਅਚਲ ਬਣਤਰ option options[] = {
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ('n', "dry-run", &flags, N_("do not actually prune any entries"),
			EXPIRE_REFLOGS_DRY_ਲੜੀਵਾਰ),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ(0, "rewrite", &flags,
			N_("rewrite the old SHA1 with the new SHA1 of the entry that now precedes it"),
			EXPIRE_REFLOGS_REWRITE),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ(0, "updateref", &flags,
			N_("update the reference to the value of the top reflog entry"),
			EXPIRE_REFLOGS_UPDATE_REF),
		OPT_BOOL(0, "verbose", &verbose, N_("print extra information on screen")),
		OPT_CALLBACK_F(0, "expire", &opts, N_("timestamp"),
			       N_("prune entries older than the specified time"),
			       PARSE_OPT_NONEG,
			       expire_ਕੁੱਲ_callback),
		OPT_CALLBACK_F(0, "expire-unreachable", &opts, N_("timestamp"),
			       N_("prune entries older than <time> that are not reachable from the current tip of the branch"),
			       PARSE_OPT_NONEG,
			       expire_unreachable_callback),
		OPT_BOOL(0, "stale-fix", &opts.stalefix,
			 N_("prune any reflog entries that point to broken commits")),
		OPT_BOOL(0, "all", &do_all, N_("process the reflogs of all references")),
		OPT_BOOL(0, "single-worktree", &single_worktree,
			 N_("limits processing to reflogs from the current worktree only")),
		OPT_ਸਿਰਾ()
	};

	repo_config(the_repository, reflog_expire_config, &opts);

	save_commit_ਬਫਰ = 0;
	do_all = ਸਥਿਤੀ = 0;

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, reflog_expire_usage, 0);

	ਜੇ (verbose)
		should_prune_fn = should_expire_reflog_ent_verbose;

	/*
	 * We can trust the commits and objects reachable from refs
	 * even in older repository.  We cannot trust what's reachable
	 * from reflog if the repository was pruned with older git.
	 */
	ਜੇ (opts.stalefix) {
		ਬਣਤਰ rev_info revs;

		repo_init_revisions(the_repository, &revs, ਅਗੇਤਰ);
		revs.do_not_die_on_missing_objects = 1;
		revs.ignore_missing = 1;
		revs.ignore_missing_links = 1;
		ਜੇ (verbose)
			ਲਿਖੋ(_("Marking reachable objects..."));
		mark_reachable_objects(&revs, 0, 0, ਨਿਰਾ);
		release_revisions(&revs);
		ਜੇ (verbose)
			ਅੱਖਰ_ਲਿਖੋ('\n');
	}

	ਜੇ (do_all) {
		ਬਣਤਰ worktree_reflogs collected = {
			.reflogs = STRING_LIST_INIT_DUP,
		};
		ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ;
		ਬਣਤਰ worktree **worktrees, **p;

		worktrees = get_worktrees(the_repository);
		ਲਈ (p = worktrees; *p; p++) {
			ਜੇ (single_worktree && !(*p)->is_current)
				ਜਾਰੀ;
			collected.worktree = *p;
			refs_for_each_reflog(get_worktree_ref_store(*p),
					     collect_reflog, &collected);
		}
		free_worktrees(worktrees);

		for_each_string_list_ਵਸਤੂ(ਵਸਤੂ, &collected.reflogs) {
			ਬਣਤਰ expire_reflog_policy_cb cb = {
				.opts = opts,
				.dry_ਲੜੀਵਾਰ = !!(flags & EXPIRE_REFLOGS_DRY_ਲੜੀਵਾਰ),
			};

			reflog_expire_options_ਸੈੱਟ_refname(&cb.opts,  ਵਸਤੂ->string);
			ਸਥਿਤੀ |= refs_reflog_expire(get_main_ref_store(the_repository),
						     ਵਸਤੂ->string, flags,
						     reflog_expiry_prepare,
						     should_prune_fn,
						     reflog_expiry_cleanup,
						     &cb);
		}
		string_list_clear(&collected.reflogs, 0);
	}

	ਲਈ (i = 0; i < argc; i++) {
		ਅੱਖਰ *ref;
		ਬਣਤਰ expire_reflog_policy_cb cb = { .opts = opts };

		ਜੇ (!repo_dwim_log(the_repository, argv[i], ਲੜੀ_ਲੰਬਾਈ(argv[i]), ਨਿਰਾ, &ref)) {
			ਸਥਿਤੀ |= error(_("reflog could not be found: '%s'"), argv[i]);
			ਜਾਰੀ;
		}
		reflog_expire_options_ਸੈੱਟ_refname(&cb.opts, ref);
		ਸਥਿਤੀ |= refs_reflog_expire(get_main_ref_store(the_repository),
					     ref, flags,
					     reflog_expiry_prepare,
					     should_prune_fn,
					     reflog_expiry_cleanup,
					     &cb);
		ਮੁਕਤ(ref);
	}

	reflog_clear_expire_config(&opts);

	ਵਾਪਸ ਸਥਿਤੀ;
}

ਸਥਿਰ ਪੂਰਨ cmd_reflog_delete(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			     ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ i, ਸਥਿਤੀ = 0;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags = 0;
	ਪੂਰਨ verbose = 0;

	ਅਚਲ ਬਣਤਰ option options[] = {
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ('n', "dry-run", &flags, N_("do not actually prune any entries"),
			EXPIRE_REFLOGS_DRY_ਲੜੀਵਾਰ),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ(0, "rewrite", &flags,
			N_("rewrite the old SHA1 with the new SHA1 of the entry that now precedes it"),
			EXPIRE_REFLOGS_REWRITE),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ(0, "updateref", &flags,
			N_("update the reference to the value of the top reflog entry"),
			EXPIRE_REFLOGS_UPDATE_REF),
		OPT_BOOL(0, "verbose", &verbose, N_("print extra information on screen")),
		OPT_ਸਿਰਾ()
	};

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, reflog_delete_usage, 0);

	ਜੇ (argc < 1)
		ਵਾਪਸ error(_("no reflog specified to delete"));

	ਲਈ (i = 0; i < argc; i++)
		ਸਥਿਤੀ |= reflog_delete(argv[i], flags, verbose);

	ਵਾਪਸ ਸਥਿਤੀ;
}

ਸਥਿਰ ਪੂਰਨ cmd_reflog_exists(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			     ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ option options[] = {
		OPT_ਸਿਰਾ()
	};
	ਅਚਲ ਅੱਖਰ *refname;

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, reflog_exists_usage,
			     0);
	ਜੇ (!argc)
		usage_with_options(reflog_exists_usage, options);

	refname = argv[0];
	ਜੇ (check_refname_format(refname, REFNAME_ALLOW_ONELEVEL))
		die(_("invalid ref format: %s"), refname);
	ਵਾਪਸ !refs_reflog_exists(get_main_ref_store(the_repository),
				   refname);
}

ਸਥਿਰ ਪੂਰਨ cmd_reflog_drop(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			   ਬਣਤਰ repository *repo)
{
	ਪੂਰਨ ret = 0, do_all = 0, single_worktree = 0;
	ਅਚਲ ਬਣਤਰ option options[] = {
		OPT_BOOL(0, "all", &do_all, N_("drop the reflogs of all references")),
		OPT_BOOL(0, "single-worktree", &single_worktree,
			 N_("drop reflogs from the current worktree only")),
		OPT_ਸਿਰਾ()
	};

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, reflog_drop_usage, 0);

	ਜੇ (argc && do_all)
		usage(_("references specified along with --all"));

	ਜੇ (do_all) {
		ਬਣਤਰ worktree_reflogs collected = {
			.reflogs = STRING_LIST_INIT_DUP,
		};
		ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ;
		ਬਣਤਰ worktree **worktrees, **p;

		worktrees = get_worktrees(the_repository);
		ਲਈ (p = worktrees; *p; p++) {
			ਜੇ (single_worktree && !(*p)->is_current)
				ਜਾਰੀ;
			collected.worktree = *p;
			refs_for_each_reflog(get_worktree_ref_store(*p),
					     collect_reflog, &collected);
		}
		free_worktrees(worktrees);

		for_each_string_list_ਵਸਤੂ(ਵਸਤੂ, &collected.reflogs)
			ret |= refs_delete_reflog(get_main_ref_store(repo),
						     ਵਸਤੂ->string);
		string_list_clear(&collected.reflogs, 0);

		ਵਾਪਸ ret;
	}

	ਲਈ (ਪੂਰਨ i = 0; i < argc; i++) {
		ਅੱਖਰ *ref;
		ਜੇ (!repo_dwim_log(repo, argv[i], ਲੜੀ_ਲੰਬਾਈ(argv[i]), ਨਿਰਾ, &ref)) {
			ret |= error(_("reflog could not be found: '%s'"), argv[i]);
			ਜਾਰੀ;
		}

		ret |= refs_delete_reflog(get_main_ref_store(repo), ref);
		ਮੁਕਤ(ref);
	}

	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ cmd_reflog_write(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			    ਬਣਤਰ repository *repo)
{
	ਅਚਲ ਬਣਤਰ option options[] = {
		OPT_ਸਿਰਾ()
	};
	ਬਣਤਰ object_id old_oid, new_oid;
	ਬਣਤਰ strbuf err = STRBUF_INIT;
	ਬਣਤਰ ref_transaction *tx;
	ਅਚਲ ਅੱਖਰ *ref, *ਸੁਨੇਹਾ_2;
	ਪੂਰਨ ret;

	repo_config(repo, git_ident_config, ਨਿਰਾ);

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, reflog_write_usage, 0);
	ਜੇ (argc != 4)
		usage_with_options(reflog_write_usage, options);

	ref = argv[0];
	ਜੇ (!is_ਜੜ੍ਹ_ref(ref) && check_refname_format(ref, 0))
		die(_("invalid reference name: %s"), ref);

	ret = get_oid_hex_algop(argv[1], &old_oid, repo->hash_algo);
	ਜੇ (ret)
		die(_("invalid old object ID: '%s'"), argv[1]);
	ਜੇ (!is_null_oid(&old_oid) && !odb_has_object(repo->objects, &old_oid, 0))
		die(_("old object '%s' does not exist"), argv[1]);

	ret = get_oid_hex_algop(argv[2], &new_oid, repo->hash_algo);
	ਜੇ (ret)
		die(_("invalid new object ID: '%s'"), argv[2]);
	ਜੇ (!is_null_oid(&new_oid) && !odb_has_object(repo->objects, &new_oid, 0))
		die(_("new object '%s' does not exist"), argv[2]);

	ਸੁਨੇਹਾ_2 = argv[3];

	tx = ref_store_transaction_begin(get_main_ref_store(repo), 0, &err);
	ਜੇ (!tx)
		die(_("cannot start transaction: %s"), err.buf);

	ret = ref_transaction_update_reflog(tx, ref, &new_oid, &old_oid,
					    git_committer_info(0),
					    ਸੁਨੇਹਾ_2, 0, &err);
	ਜੇ (ret)
		die(_("cannot queue reflog update: %s"), err.buf);

	ret = ref_transaction_commit(tx, &err);
	ਜੇ (ret)
		die(_("cannot commit reflog update: %s"), err.buf);

	ref_transaction_free(tx);
	strbuf_release(&err);
	ਵਾਪਸ 0;
}

/*
 * main "reflog"
 */
ਪੂਰਨ cmd_reflog(ਪੂਰਨ argc,
	       ਅਚਲ ਅੱਖਰ **argv,
	       ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
	       ਬਣਤਰ repository *repository)
{
	parse_opt_subcommand_fn *fn = ਨਿਰਾ;
	ਬਣਤਰ option options[] = {
		OPT_SUBCOMMAND("show", &fn, cmd_reflog_show),
		OPT_SUBCOMMAND("list", &fn, cmd_reflog_list),
		OPT_SUBCOMMAND("exists", &fn, cmd_reflog_exists),
		OPT_SUBCOMMAND("write", &fn, cmd_reflog_write),
		OPT_SUBCOMMAND("delete", &fn, cmd_reflog_delete),
		OPT_SUBCOMMAND("drop", &fn, cmd_reflog_drop),
		OPT_SUBCOMMAND("expire", &fn, cmd_reflog_expire),
		OPT_ਸਿਰਾ()
	};

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, reflog_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL |
			     PARSE_OPT_KEEP_DASHDASH | PARSE_OPT_KEEP_ARGV0 |
			     PARSE_OPT_KEEP_UNKNOWN_OPT);
	ਜੇ (fn)
		ਵਾਪਸ fn(argc - 1, argv + 1, ਅਗੇਤਰ, repository);
	ਨਹੀਂਤਾਂ
		ਵਾਪਸ cmd_log_reflog(argc, argv, ਅਗੇਤਰ, repository);
}
