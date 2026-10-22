#define USE_THE_REPOSITORY_VARIABLE

#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "hex.h"
#அந்தர்பாவ "odb.h"
#அந்தர்பாவ "revision.h"
#அந்தர்பாவ "reachable.h"
#அந்தர்பாவ "wildmatch.h"
#அந்தர்பாவ "worktree.h"
#அந்தர்பாவ "reflog.h"
#அந்தர்பாவ "refs.h"
#அந்தர்பாவ "parse-options.h"

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

நிலையான மாறிலி எழுத்து *மாறிலி reflog_show_usage[] = {
	BUILTIN_REFLOG_SHOW_USAGE,
	வெற்று,
};

நிலையான மாறிலி எழுத்து *மாறிலி reflog_list_usage[] = {
	BUILTIN_REFLOG_LIST_USAGE,
	வெற்று,
};

நிலையான மாறிலி எழுத்து *மாறிலி reflog_exists_usage[] = {
	BUILTIN_REFLOG_EXISTS_USAGE,
	வெற்று,
};

நிலையான மாறிலி எழுத்து *மாறிலி reflog_write_usage[] = {
	BUILTIN_REFLOG_WRITE_USAGE,
	வெற்று,
};

நிலையான மாறிலி எழுத்து *மாறிலி reflog_delete_usage[] = {
	BUILTIN_REFLOG_DELETE_USAGE,
	வெற்று
};

நிலையான மாறிலி எழுத்து *மாறிலி reflog_drop_usage[] = {
	BUILTIN_REFLOG_DROP_USAGE,
	வெற்று,
};

நிலையான மாறிலி எழுத்து *மாறிலி reflog_expire_usage[] = {
	BUILTIN_REFLOG_EXPIRE_USAGE,
	வெற்று
};

நிலையான மாறிலி எழுத்து *மாறிலி reflog_usage[] = {
	BUILTIN_REFLOG_SHOW_USAGE,
	BUILTIN_REFLOG_LIST_USAGE,
	BUILTIN_REFLOG_EXISTS_USAGE,
	BUILTIN_REFLOG_WRITE_USAGE,
	BUILTIN_REFLOG_DELETE_USAGE,
	BUILTIN_REFLOG_DROP_USAGE,
	BUILTIN_REFLOG_EXPIRE_USAGE,
	வெற்று
};

கட்டமைப்பு worktree_reflogs {
	கட்டமைப்பு worktree *worktree;
	கட்டமைப்பு string_list reflogs;
};

நிலையான முழு collect_reflog(மாறிலி எழுத்து *ref, வெறுமை *cb_data)
{
	கட்டமைப்பு worktree_reflogs *cb = cb_data;
	கட்டமைப்பு worktree *worktree = cb->worktree;
	கட்டமைப்பு strbuf newref = STRBUF_INIT;

	/*
	 * Avoid collecting the same shared ref multiple times because
	 * they are available via all worktrees.
	 */
	எனில் (!worktree->is_current &&
	    parse_worktree_ref(ref, வெற்று, வெற்று, வெற்று) == REF_WORKTREE_SHARED)
		திரும்பு 0;

	strbuf_worktree_ref(worktree, &newref, ref);
	string_list_append_nodup(&cb->reflogs, strbuf_detach(&newref, வெற்று));

	திரும்பு 0;
}

நிலையான முழு expire_unreachable_callback(மாறிலி கட்டமைப்பு option *opt,
				 மாறிலி எழுத்து *arg,
				 முழு unset)
{
	கட்டமைப்பு reflog_expire_options *opts = opt->மதிப்பு;

	BUG_ON_OPT_NEG(unset);

	எனில் (parse_expiry_date(arg, &opts->expire_unreachable))
		die(_("invalid timestamp '%s' given to '--%s'"),
		    arg, opt->long_name);

	opts->explicit_expiry |= REFLOG_EXPIRE_UNREACH;
	திரும்பு 0;
}

நிலையான முழு expire_total_callback(மாறிலி கட்டமைப்பு option *opt,
				 மாறிலி எழுத்து *arg,
				 முழு unset)
{
	கட்டமைப்பு reflog_expire_options *opts = opt->மதிப்பு;

	BUG_ON_OPT_NEG(unset);

	எனில் (parse_expiry_date(arg, &opts->expire_total))
		die(_("invalid timestamp '%s' given to '--%s'"),
		    arg, opt->long_name);

	opts->explicit_expiry |= REFLOG_EXPIRE_TOTAL;
	திரும்பு 0;
}

நிலையான முழு cmd_reflog_show(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
			   கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு option options[] = {
		OPT_END()
	};

	parse_options(argc, argv, prefix, options, reflog_show_usage,
		      PARSE_OPT_KEEP_DASHDASH | PARSE_OPT_KEEP_ARGV0 |
		      PARSE_OPT_KEEP_UNKNOWN_OPT);

	திரும்பு cmd_log_reflog(argc, argv, prefix, the_repository);
}

நிலையான முழு show_reflog(மாறிலி எழுத்து *refname, வெறுமை *cb_data UNUSED)
{
	printf("%s\n", refname);
	திரும்பு 0;
}

நிலையான முழு cmd_reflog_list(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
			   கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு option options[] = {
		OPT_END()
	};
	கட்டமைப்பு ref_store *ref_store;

	argc = parse_options(argc, argv, prefix, options, reflog_list_usage, 0);
	எனில் (argc)
		திரும்பு error(_("%s does not accept arguments: '%s'"),
			     "list", argv[0]);

	ref_store = get_main_ref_store(the_repository);

	திரும்பு refs_for_each_reflog(ref_store, show_reflog, வெற்று);
}

நிலையான முழு cmd_reflog_expire(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
			     கட்டமைப்பு repository *repo UNUSED)
{
	timestamp_t now = time(வெற்று);
	கட்டமைப்பு reflog_expire_options opts = REFLOG_EXPIRE_OPTIONS_INIT(now);
	முழு i, status, do_all, single_worktree = 0;
	unsigned முழு flags = 0;
	முழு verbose = 0;
	reflog_expiry_should_prune_fn *should_prune_fn = should_expire_reflog_ent;
	மாறிலி கட்டமைப்பு option options[] = {
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

	எனில் (verbose)
		should_prune_fn = should_expire_reflog_ent_verbose;

	/*
	 * We can trust the commits and objects reachable from refs
	 * even in older repository.  We cannot trust what's reachable
	 * from reflog if the repository was pruned with older git.
	 */
	எனில் (opts.stalefix) {
		கட்டமைப்பு rev_info revs;

		repo_init_revisions(the_repository, &revs, prefix);
		revs.do_not_die_on_missing_objects = 1;
		revs.ignore_missing = 1;
		revs.ignore_missing_links = 1;
		எனில் (verbose)
			printf(_("Marking reachable objects..."));
		mark_reachable_objects(&revs, 0, 0, வெற்று);
		release_revisions(&revs);
		எனில் (verbose)
			putchar('\n');
	}

	எனில் (do_all) {
		கட்டமைப்பு worktree_reflogs collected = {
			.reflogs = STRING_LIST_INIT_DUP,
		};
		கட்டமைப்பு string_list_item *item;
		கட்டமைப்பு worktree **worktrees, **p;

		worktrees = get_worktrees(the_repository);
		க்கு (p = worktrees; *p; p++) {
			எனில் (single_worktree && !(*p)->is_current)
				தொடர்;
			collected.worktree = *p;
			refs_for_each_reflog(get_worktree_ref_store(*p),
					     collect_reflog, &collected);
		}
		free_worktrees(worktrees);

		for_each_string_list_item(item, &collected.reflogs) {
			கட்டமைப்பு expire_reflog_policy_cb cb = {
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

	க்கு (i = 0; i < argc; i++) {
		எழுத்து *ref;
		கட்டமைப்பு expire_reflog_policy_cb cb = { .opts = opts };

		எனில் (!repo_dwim_log(the_repository, argv[i], strlen(argv[i]), வெற்று, &ref)) {
			status |= error(_("reflog could not be found: '%s'"), argv[i]);
			தொடர்;
		}
		reflog_expire_options_set_refname(&cb.opts, ref);
		status |= refs_reflog_expire(get_main_ref_store(the_repository),
					     ref, flags,
					     reflog_expiry_prepare,
					     should_prune_fn,
					     reflog_expiry_cleanup,
					     &cb);
		விடுவி(ref);
	}

	reflog_clear_expire_config(&opts);

	திரும்பு status;
}

நிலையான முழு cmd_reflog_delete(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
			     கட்டமைப்பு repository *repo UNUSED)
{
	முழு i, status = 0;
	unsigned முழு flags = 0;
	முழு verbose = 0;

	மாறிலி கட்டமைப்பு option options[] = {
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

	எனில் (argc < 1)
		திரும்பு error(_("no reflog specified to delete"));

	க்கு (i = 0; i < argc; i++)
		status |= reflog_delete(argv[i], flags, verbose);

	திரும்பு status;
}

நிலையான முழு cmd_reflog_exists(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
			     கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு option options[] = {
		OPT_END()
	};
	மாறிலி எழுத்து *refname;

	argc = parse_options(argc, argv, prefix, options, reflog_exists_usage,
			     0);
	எனில் (!argc)
		usage_with_options(reflog_exists_usage, options);

	refname = argv[0];
	எனில் (check_refname_format(refname, REFNAME_ALLOW_ONELEVEL))
		die(_("invalid ref format: %s"), refname);
	திரும்பு !refs_reflog_exists(get_main_ref_store(the_repository),
				   refname);
}

நிலையான முழு cmd_reflog_drop(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
			   கட்டமைப்பு repository *repo)
{
	முழு ret = 0, do_all = 0, single_worktree = 0;
	மாறிலி கட்டமைப்பு option options[] = {
		OPT_BOOL(0, "all", &do_all, N_("drop the reflogs of all references")),
		OPT_BOOL(0, "single-worktree", &single_worktree,
			 N_("drop reflogs from the current worktree only")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, reflog_drop_usage, 0);

	எனில் (argc && do_all)
		usage(_("references specified along with --all"));

	எனில் (do_all) {
		கட்டமைப்பு worktree_reflogs collected = {
			.reflogs = STRING_LIST_INIT_DUP,
		};
		கட்டமைப்பு string_list_item *item;
		கட்டமைப்பு worktree **worktrees, **p;

		worktrees = get_worktrees(the_repository);
		க்கு (p = worktrees; *p; p++) {
			எனில் (single_worktree && !(*p)->is_current)
				தொடர்;
			collected.worktree = *p;
			refs_for_each_reflog(get_worktree_ref_store(*p),
					     collect_reflog, &collected);
		}
		free_worktrees(worktrees);

		for_each_string_list_item(item, &collected.reflogs)
			ret |= refs_delete_reflog(get_main_ref_store(repo),
						     item->string);
		string_list_clear(&collected.reflogs, 0);

		திரும்பு ret;
	}

	க்கு (முழு i = 0; i < argc; i++) {
		எழுத்து *ref;
		எனில் (!repo_dwim_log(repo, argv[i], strlen(argv[i]), வெற்று, &ref)) {
			ret |= error(_("reflog could not be found: '%s'"), argv[i]);
			தொடர்;
		}

		ret |= refs_delete_reflog(get_main_ref_store(repo), ref);
		விடுவி(ref);
	}

	திரும்பு ret;
}

நிலையான முழு cmd_reflog_write(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
			    கட்டமைப்பு repository *repo)
{
	மாறிலி கட்டமைப்பு option options[] = {
		OPT_END()
	};
	கட்டமைப்பு object_id old_oid, new_oid;
	கட்டமைப்பு strbuf err = STRBUF_INIT;
	கட்டமைப்பு ref_transaction *tx;
	மாறிலி எழுத்து *ref, *message;
	முழு ret;

	repo_config(repo, git_ident_config, வெற்று);

	argc = parse_options(argc, argv, prefix, options, reflog_write_usage, 0);
	எனில் (argc != 4)
		usage_with_options(reflog_write_usage, options);

	ref = argv[0];
	எனில் (!is_வேர்_ref(ref) && check_refname_format(ref, 0))
		die(_("invalid reference name: %s"), ref);

	ret = get_oid_hex_algop(argv[1], &old_oid, repo->hash_algo);
	எனில் (ret)
		die(_("invalid old object ID: '%s'"), argv[1]);
	எனில் (!is_null_oid(&old_oid) && !odb_has_object(repo->objects, &old_oid, 0))
		die(_("old object '%s' does not exist"), argv[1]);

	ret = get_oid_hex_algop(argv[2], &new_oid, repo->hash_algo);
	எனில் (ret)
		die(_("invalid new object ID: '%s'"), argv[2]);
	எனில் (!is_null_oid(&new_oid) && !odb_has_object(repo->objects, &new_oid, 0))
		die(_("new object '%s' does not exist"), argv[2]);

	message = argv[3];

	tx = ref_store_transaction_begin(get_main_ref_store(repo), 0, &err);
	எனில் (!tx)
		die(_("cannot start transaction: %s"), err.buf);

	ret = ref_transaction_update_reflog(tx, ref, &new_oid, &old_oid,
					    git_committer_info(0),
					    message, 0, &err);
	எனில் (ret)
		die(_("cannot queue reflog update: %s"), err.buf);

	ret = ref_transaction_commit(tx, &err);
	எனில் (ret)
		die(_("cannot commit reflog update: %s"), err.buf);

	ref_transaction_free(tx);
	strbuf_release(&err);
	திரும்பு 0;
}

/*
 * main "reflog"
 */
முழு cmd_reflog(முழு argc,
	       மாறிலி எழுத்து **argv,
	       மாறிலி எழுத்து *prefix,
	       கட்டமைப்பு repository *repository)
{
	parse_opt_subcommand_fn *fn = வெற்று;
	கட்டமைப்பு option options[] = {
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
	எனில் (fn)
		திரும்பு fn(argc - 1, argv + 1, prefix, repository);
	இல்லையெனில்
		திரும்பு cmd_log_reflog(argc, argv, prefix, repository);
}
