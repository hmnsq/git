#define USE_THE_REPOSITORY_VARIABLE

#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "hex.h"
#અન્તર્ભાવ "odb.h"
#અન્તર્ભાવ "revision.h"
#અન્તર્ભાવ "reachable.h"
#અન્તર્ભાવ "wildmatch.h"
#અન્તર્ભાવ "worktree.h"
#અન્તર્ભાવ "reflog.h"
#અન્તર્ભાવ "refs.h"
#અન્તર્ભાવ "parse-options.h"

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

સ્થિર અચલ અક્ષર *અચલ reflog_show_usage[] = {
	BUILTIN_REFLOG_SHOW_USAGE,
	ખાલી,
};

સ્થિર અચલ અક્ષર *અચલ reflog_list_usage[] = {
	BUILTIN_REFLOG_LIST_USAGE,
	ખાલી,
};

સ્થિર અચલ અક્ષર *અચલ reflog_exists_usage[] = {
	BUILTIN_REFLOG_EXISTS_USAGE,
	ખાલી,
};

સ્થિર અચલ અક્ષર *અચલ reflog_write_usage[] = {
	BUILTIN_REFLOG_WRITE_USAGE,
	ખાલી,
};

સ્થિર અચલ અક્ષર *અચલ reflog_delete_usage[] = {
	BUILTIN_REFLOG_DELETE_USAGE,
	ખાલી
};

સ્થિર અચલ અક્ષર *અચલ reflog_drop_usage[] = {
	BUILTIN_REFLOG_DROP_USAGE,
	ખાલી,
};

સ્થિર અચલ અક્ષર *અચલ reflog_expire_usage[] = {
	BUILTIN_REFLOG_EXPIRE_USAGE,
	ખાલી
};

સ્થિર અચલ અક્ષર *અચલ reflog_usage[] = {
	BUILTIN_REFLOG_SHOW_USAGE,
	BUILTIN_REFLOG_LIST_USAGE,
	BUILTIN_REFLOG_EXISTS_USAGE,
	BUILTIN_REFLOG_WRITE_USAGE,
	BUILTIN_REFLOG_DELETE_USAGE,
	BUILTIN_REFLOG_DROP_USAGE,
	BUILTIN_REFLOG_EXPIRE_USAGE,
	ખાલી
};

સંરચના worktree_reflogs {
	સંરચના worktree *worktree;
	સંરચના string_list reflogs;
};

સ્થિર પૂર્ણાંક collect_reflog(અચલ અક્ષર *ref, શૂન્ય *cb_data)
{
	સંરચના worktree_reflogs *cb = cb_data;
	સંરચના worktree *worktree = cb->worktree;
	સંરચના strbuf newref = STRBUF_INIT;

	/*
	 * Avoid collecting the same shared ref multiple times because
	 * they are available via all worktrees.
	 */
	જો (!worktree->is_current &&
	    parse_worktree_ref(ref, ખાલી, ખાલી, ખાલી) == REF_WORKTREE_SHARED)
		પાછા 0;

	strbuf_worktree_ref(worktree, &newref, ref);
	string_list_append_nodup(&cb->reflogs, strbuf_detach(&newref, ખાલી));

	પાછા 0;
}

સ્થિર પૂર્ણાંક expire_unreachable_callback(અચલ સંરચના option *opt,
				 અચલ અક્ષર *arg,
				 પૂર્ણાંક unset)
{
	સંરચના reflog_expire_options *opts = opt->કિંમત;

	BUG_ON_OPT_NEG(unset);

	જો (parse_expiry_date(arg, &opts->expire_unreachable))
		die(_("invalid timestamp '%s' given to '--%s'"),
		    arg, opt->long_name);

	opts->explicit_expiry |= REFLOG_EXPIRE_UNREACH;
	પાછા 0;
}

સ્થિર પૂર્ણાંક expire_total_callback(અચલ સંરચના option *opt,
				 અચલ અક્ષર *arg,
				 પૂર્ણાંક unset)
{
	સંરચના reflog_expire_options *opts = opt->કિંમત;

	BUG_ON_OPT_NEG(unset);

	જો (parse_expiry_date(arg, &opts->expire_total))
		die(_("invalid timestamp '%s' given to '--%s'"),
		    arg, opt->long_name);

	opts->explicit_expiry |= REFLOG_EXPIRE_TOTAL;
	પાછા 0;
}

સ્થિર પૂર્ણાંક cmd_reflog_show(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
			   સંરચના repository *repo UNUSED)
{
	સંરચના option options[] = {
		OPT_END()
	};

	parse_options(argc, argv, prefix, options, reflog_show_usage,
		      PARSE_OPT_KEEP_DASHDASH | PARSE_OPT_KEEP_ARGV0 |
		      PARSE_OPT_KEEP_UNKNOWN_OPT);

	પાછા cmd_log_reflog(argc, argv, prefix, the_repository);
}

સ્થિર પૂર્ણાંક show_reflog(અચલ અક્ષર *refname, શૂન્ય *cb_data UNUSED)
{
	printf("%s\n", refname);
	પાછા 0;
}

સ્થિર પૂર્ણાંક cmd_reflog_list(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
			   સંરચના repository *repo UNUSED)
{
	સંરચના option options[] = {
		OPT_END()
	};
	સંરચના ref_store *ref_store;

	argc = parse_options(argc, argv, prefix, options, reflog_list_usage, 0);
	જો (argc)
		પાછા error(_("%s does not accept arguments: '%s'"),
			     "list", argv[0]);

	ref_store = get_main_ref_store(the_repository);

	પાછા refs_for_each_reflog(ref_store, show_reflog, ખાલી);
}

સ્થિર પૂર્ણાંક cmd_reflog_expire(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
			     સંરચના repository *repo UNUSED)
{
	timestamp_t now = time(ખાલી);
	સંરચના reflog_expire_options opts = REFLOG_EXPIRE_OPTIONS_INIT(now);
	પૂર્ણાંક i, status, do_all, single_worktree = 0;
	unsigned પૂર્ણાંક flags = 0;
	પૂર્ણાંક verbose = 0;
	reflog_expiry_should_prune_fn *should_prune_fn = should_expire_reflog_ent;
	અચલ સંરચના option options[] = {
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

	જો (verbose)
		should_prune_fn = should_expire_reflog_ent_verbose;

	/*
	 * We can trust the commits and objects reachable from refs
	 * even in older repository.  We cannot trust what's reachable
	 * from reflog if the repository was pruned with older git.
	 */
	જો (opts.stalefix) {
		સંરચના rev_info revs;

		repo_init_revisions(the_repository, &revs, prefix);
		revs.do_not_die_on_missing_objects = 1;
		revs.ignore_missing = 1;
		revs.ignore_missing_links = 1;
		જો (verbose)
			printf(_("Marking reachable objects..."));
		mark_reachable_objects(&revs, 0, 0, ખાલી);
		release_revisions(&revs);
		જો (verbose)
			putchar('\n');
	}

	જો (do_all) {
		સંરચના worktree_reflogs collected = {
			.reflogs = STRING_LIST_INIT_DUP,
		};
		સંરચના string_list_item *item;
		સંરચના worktree **worktrees, **p;

		worktrees = get_worktrees(the_repository);
		માટે (p = worktrees; *p; p++) {
			જો (single_worktree && !(*p)->is_current)
				ચાલુ;
			collected.worktree = *p;
			refs_for_each_reflog(get_worktree_ref_store(*p),
					     collect_reflog, &collected);
		}
		free_worktrees(worktrees);

		for_each_string_list_item(item, &collected.reflogs) {
			સંરચના expire_reflog_policy_cb cb = {
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

	માટે (i = 0; i < argc; i++) {
		અક્ષર *ref;
		સંરચના expire_reflog_policy_cb cb = { .opts = opts };

		જો (!repo_dwim_log(the_repository, argv[i], strlen(argv[i]), ખાલી, &ref)) {
			status |= error(_("reflog could not be found: '%s'"), argv[i]);
			ચાલુ;
		}
		reflog_expire_options_set_refname(&cb.opts, ref);
		status |= refs_reflog_expire(get_main_ref_store(the_repository),
					     ref, flags,
					     reflog_expiry_prepare,
					     should_prune_fn,
					     reflog_expiry_cleanup,
					     &cb);
		મુક્ત(ref);
	}

	reflog_clear_expire_config(&opts);

	પાછા status;
}

સ્થિર પૂર્ણાંક cmd_reflog_delete(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
			     સંરચના repository *repo UNUSED)
{
	પૂર્ણાંક i, status = 0;
	unsigned પૂર્ણાંક flags = 0;
	પૂર્ણાંક verbose = 0;

	અચલ સંરચના option options[] = {
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

	જો (argc < 1)
		પાછા error(_("no reflog specified to delete"));

	માટે (i = 0; i < argc; i++)
		status |= reflog_delete(argv[i], flags, verbose);

	પાછા status;
}

સ્થિર પૂર્ણાંક cmd_reflog_exists(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
			     સંરચના repository *repo UNUSED)
{
	સંરચના option options[] = {
		OPT_END()
	};
	અચલ અક્ષર *refname;

	argc = parse_options(argc, argv, prefix, options, reflog_exists_usage,
			     0);
	જો (!argc)
		usage_with_options(reflog_exists_usage, options);

	refname = argv[0];
	જો (check_refname_format(refname, REFNAME_ALLOW_ONELEVEL))
		die(_("invalid ref format: %s"), refname);
	પાછા !refs_reflog_exists(get_main_ref_store(the_repository),
				   refname);
}

સ્થિર પૂર્ણાંક cmd_reflog_drop(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
			   સંરચના repository *repo)
{
	પૂર્ણાંક ret = 0, do_all = 0, single_worktree = 0;
	અચલ સંરચના option options[] = {
		OPT_BOOL(0, "all", &do_all, N_("drop the reflogs of all references")),
		OPT_BOOL(0, "single-worktree", &single_worktree,
			 N_("drop reflogs from the current worktree only")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, reflog_drop_usage, 0);

	જો (argc && do_all)
		usage(_("references specified along with --all"));

	જો (do_all) {
		સંરચના worktree_reflogs collected = {
			.reflogs = STRING_LIST_INIT_DUP,
		};
		સંરચના string_list_item *item;
		સંરચના worktree **worktrees, **p;

		worktrees = get_worktrees(the_repository);
		માટે (p = worktrees; *p; p++) {
			જો (single_worktree && !(*p)->is_current)
				ચાલુ;
			collected.worktree = *p;
			refs_for_each_reflog(get_worktree_ref_store(*p),
					     collect_reflog, &collected);
		}
		free_worktrees(worktrees);

		for_each_string_list_item(item, &collected.reflogs)
			ret |= refs_delete_reflog(get_main_ref_store(repo),
						     item->string);
		string_list_clear(&collected.reflogs, 0);

		પાછા ret;
	}

	માટે (પૂર્ણાંક i = 0; i < argc; i++) {
		અક્ષર *ref;
		જો (!repo_dwim_log(repo, argv[i], strlen(argv[i]), ખાલી, &ref)) {
			ret |= error(_("reflog could not be found: '%s'"), argv[i]);
			ચાલુ;
		}

		ret |= refs_delete_reflog(get_main_ref_store(repo), ref);
		મુક્ત(ref);
	}

	પાછા ret;
}

સ્થિર પૂર્ણાંક cmd_reflog_write(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
			    સંરચના repository *repo)
{
	અચલ સંરચના option options[] = {
		OPT_END()
	};
	સંરચના object_id old_oid, new_oid;
	સંરચના strbuf err = STRBUF_INIT;
	સંરચના ref_transaction *tx;
	અચલ અક્ષર *ref, *message;
	પૂર્ણાંક ret;

	repo_config(repo, git_ident_config, ખાલી);

	argc = parse_options(argc, argv, prefix, options, reflog_write_usage, 0);
	જો (argc != 4)
		usage_with_options(reflog_write_usage, options);

	ref = argv[0];
	જો (!is_મૂળ_ref(ref) && check_refname_format(ref, 0))
		die(_("invalid reference name: %s"), ref);

	ret = get_oid_hex_algop(argv[1], &old_oid, repo->hash_algo);
	જો (ret)
		die(_("invalid old object ID: '%s'"), argv[1]);
	જો (!is_null_oid(&old_oid) && !odb_has_object(repo->objects, &old_oid, 0))
		die(_("old object '%s' does not exist"), argv[1]);

	ret = get_oid_hex_algop(argv[2], &new_oid, repo->hash_algo);
	જો (ret)
		die(_("invalid new object ID: '%s'"), argv[2]);
	જો (!is_null_oid(&new_oid) && !odb_has_object(repo->objects, &new_oid, 0))
		die(_("new object '%s' does not exist"), argv[2]);

	message = argv[3];

	tx = ref_store_transaction_begin(get_main_ref_store(repo), 0, &err);
	જો (!tx)
		die(_("cannot start transaction: %s"), err.buf);

	ret = ref_transaction_update_reflog(tx, ref, &new_oid, &old_oid,
					    git_committer_info(0),
					    message, 0, &err);
	જો (ret)
		die(_("cannot queue reflog update: %s"), err.buf);

	ret = ref_transaction_commit(tx, &err);
	જો (ret)
		die(_("cannot commit reflog update: %s"), err.buf);

	ref_transaction_free(tx);
	strbuf_release(&err);
	પાછા 0;
}

/*
 * main "reflog"
 */
પૂર્ણાંક cmd_reflog(પૂર્ણાંક argc,
	       અચલ અક્ષર **argv,
	       અચલ અક્ષર *prefix,
	       સંરચના repository *repository)
{
	parse_opt_subcommand_fn *fn = ખાલી;
	સંરચના option options[] = {
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
	જો (fn)
		પાછા fn(argc - 1, argv + 1, prefix, repository);
	નહીંતર
		પાછા cmd_log_reflog(argc, argv, prefix, repository);
}
