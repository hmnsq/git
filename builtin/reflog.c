#define USE_THE_REPOSITORY_VARIABLE

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "odb.h"
#अन्तर्भाव "revision.h"
#अन्तर्भाव "reachable.h"
#अन्तर्भाव "wildmatch.h"
#अन्तर्भाव "worktree.h"
#अन्तर्भाव "reflog.h"
#अन्तर्भाव "refs.h"
#अन्तर्भाव "parse-options.h"

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

स्थिर अचर वर्ण *अचर reflog_show_usage[] = {
	BUILTIN_REFLOG_SHOW_USAGE,
	रिक्त,
};

स्थिर अचर वर्ण *अचर reflog_list_usage[] = {
	BUILTIN_REFLOG_LIST_USAGE,
	रिक्त,
};

स्थिर अचर वर्ण *अचर reflog_exists_usage[] = {
	BUILTIN_REFLOG_EXISTS_USAGE,
	रिक्त,
};

स्थिर अचर वर्ण *अचर reflog_write_usage[] = {
	BUILTIN_REFLOG_WRITE_USAGE,
	रिक्त,
};

स्थिर अचर वर्ण *अचर reflog_delete_usage[] = {
	BUILTIN_REFLOG_DELETE_USAGE,
	रिक्त
};

स्थिर अचर वर्ण *अचर reflog_drop_usage[] = {
	BUILTIN_REFLOG_DROP_USAGE,
	रिक्त,
};

स्थिर अचर वर्ण *अचर reflog_expire_usage[] = {
	BUILTIN_REFLOG_EXPIRE_USAGE,
	रिक्त
};

स्थिर अचर वर्ण *अचर reflog_usage[] = {
	BUILTIN_REFLOG_SHOW_USAGE,
	BUILTIN_REFLOG_LIST_USAGE,
	BUILTIN_REFLOG_EXISTS_USAGE,
	BUILTIN_REFLOG_WRITE_USAGE,
	BUILTIN_REFLOG_DELETE_USAGE,
	BUILTIN_REFLOG_DROP_USAGE,
	BUILTIN_REFLOG_EXPIRE_USAGE,
	रिक्त
};

संरचना worktree_reflogs {
	संरचना worktree *worktree;
	संरचना string_list reflogs;
};

स्थिर पूर्णाङ्क collect_reflog(अचर वर्ण *ref, शून्य *cb_data)
{
	संरचना worktree_reflogs *cb = cb_data;
	संरचना worktree *worktree = cb->worktree;
	संरचना strbuf newref = STRBUF_INIT;

	/*
	 * Avoid collecting the same shared ref multiple times because
	 * they are available via all worktrees.
	 */
	यदि (!worktree->is_current &&
	    parse_worktree_ref(ref, रिक्त, रिक्त, रिक्त) == REF_WORKTREE_SHARED)
		फर्काऊ 0;

	strbuf_worktree_ref(worktree, &newref, ref);
	string_list_append_nodup(&cb->reflogs, strbuf_detach(&newref, रिक्त));

	फर्काऊ 0;
}

स्थिर पूर्णाङ्क expire_unreachable_callback(अचर संरचना option *opt,
				 अचर वर्ण *arg,
				 पूर्णाङ्क unset)
{
	संरचना reflog_expire_options *opts = opt->मान;

	BUG_ON_OPT_NEG(unset);

	यदि (parse_expiry_date(arg, &opts->expire_unreachable))
		die(_("invalid timestamp '%s' given to '--%s'"),
		    arg, opt->long_name);

	opts->explicit_expiry |= REFLOG_EXPIRE_UNREACH;
	फर्काऊ 0;
}

स्थिर पूर्णाङ्क expire_total_callback(अचर संरचना option *opt,
				 अचर वर्ण *arg,
				 पूर्णाङ्क unset)
{
	संरचना reflog_expire_options *opts = opt->मान;

	BUG_ON_OPT_NEG(unset);

	यदि (parse_expiry_date(arg, &opts->expire_total))
		die(_("invalid timestamp '%s' given to '--%s'"),
		    arg, opt->long_name);

	opts->explicit_expiry |= REFLOG_EXPIRE_TOTAL;
	फर्काऊ 0;
}

स्थिर पूर्णाङ्क cmd_reflog_show(पूर्णाङ्क argc, अचर वर्ण **argv, अचर वर्ण *prefix,
			   संरचना repository *repo UNUSED)
{
	संरचना option options[] = {
		OPT_END()
	};

	parse_options(argc, argv, prefix, options, reflog_show_usage,
		      PARSE_OPT_KEEP_DASHDASH | PARSE_OPT_KEEP_ARGV0 |
		      PARSE_OPT_KEEP_UNKNOWN_OPT);

	फर्काऊ cmd_log_reflog(argc, argv, prefix, the_repository);
}

स्थिर पूर्णाङ्क show_reflog(अचर वर्ण *refname, शून्य *cb_data UNUSED)
{
	म_लिखो("%s\n", refname);
	फर्काऊ 0;
}

स्थिर पूर्णाङ्क cmd_reflog_list(पूर्णाङ्क argc, अचर वर्ण **argv, अचर वर्ण *prefix,
			   संरचना repository *repo UNUSED)
{
	संरचना option options[] = {
		OPT_END()
	};
	संरचना ref_store *ref_store;

	argc = parse_options(argc, argv, prefix, options, reflog_list_usage, 0);
	यदि (argc)
		फर्काऊ error(_("%s does not accept arguments: '%s'"),
			     "list", argv[0]);

	ref_store = get_main_ref_store(the_repository);

	फर्काऊ refs_for_each_reflog(ref_store, show_reflog, रिक्त);
}

स्थिर पूर्णाङ्क cmd_reflog_expire(पूर्णाङ्क argc, अचर वर्ण **argv, अचर वर्ण *prefix,
			     संरचना repository *repo UNUSED)
{
	timestamp_t now = समय(रिक्त);
	संरचना reflog_expire_options opts = REFLOG_EXPIRE_OPTIONS_INIT(now);
	पूर्णाङ्क i, status, do_all, single_worktree = 0;
	अचिन्हित पूर्णाङ्क flags = 0;
	पूर्णाङ्क verbose = 0;
	reflog_expiry_should_prune_fn *should_prune_fn = should_expire_reflog_ent;
	अचर संरचना option options[] = {
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

	यदि (verbose)
		should_prune_fn = should_expire_reflog_ent_verbose;

	/*
	 * We can trust the commits and objects reachable from refs
	 * even in older repository.  We cannot trust what's reachable
	 * from reflog if the repository was pruned with older git.
	 */
	यदि (opts.stalefix) {
		संरचना rev_info revs;

		repo_init_revisions(the_repository, &revs, prefix);
		revs.do_not_die_on_missing_objects = 1;
		revs.ignore_missing = 1;
		revs.ignore_missing_links = 1;
		यदि (verbose)
			म_लिखो(_("Marking reachable objects..."));
		mark_reachable_objects(&revs, 0, 0, रिक्त);
		release_revisions(&revs);
		यदि (verbose)
			अक्षर_दो('\n');
	}

	यदि (do_all) {
		संरचना worktree_reflogs collected = {
			.reflogs = STRING_LIST_INIT_DUP,
		};
		संरचना string_list_item *item;
		संरचना worktree **worktrees, **p;

		worktrees = get_worktrees(the_repository);
		लागि (p = worktrees; *p; p++) {
			यदि (single_worktree && !(*p)->is_current)
				जारी;
			collected.worktree = *p;
			refs_for_each_reflog(get_worktree_ref_store(*p),
					     collect_reflog, &collected);
		}
		free_worktrees(worktrees);

		for_each_string_list_item(item, &collected.reflogs) {
			संरचना expire_reflog_policy_cb cb = {
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

	लागि (i = 0; i < argc; i++) {
		वर्ण *ref;
		संरचना expire_reflog_policy_cb cb = { .opts = opts };

		यदि (!repo_dwim_log(the_repository, argv[i], म_माप(argv[i]), रिक्त, &ref)) {
			status |= error(_("reflog could not be found: '%s'"), argv[i]);
			जारी;
		}
		reflog_expire_options_set_refname(&cb.opts, ref);
		status |= refs_reflog_expire(get_main_ref_store(the_repository),
					     ref, flags,
					     reflog_expiry_prepare,
					     should_prune_fn,
					     reflog_expiry_cleanup,
					     &cb);
		मुक्त(ref);
	}

	reflog_clear_expire_config(&opts);

	फर्काऊ status;
}

स्थिर पूर्णाङ्क cmd_reflog_delete(पूर्णाङ्क argc, अचर वर्ण **argv, अचर वर्ण *prefix,
			     संरचना repository *repo UNUSED)
{
	पूर्णाङ्क i, status = 0;
	अचिन्हित पूर्णाङ्क flags = 0;
	पूर्णाङ्क verbose = 0;

	अचर संरचना option options[] = {
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

	यदि (argc < 1)
		फर्काऊ error(_("no reflog specified to delete"));

	लागि (i = 0; i < argc; i++)
		status |= reflog_delete(argv[i], flags, verbose);

	फर्काऊ status;
}

स्थिर पूर्णाङ्क cmd_reflog_exists(पूर्णाङ्क argc, अचर वर्ण **argv, अचर वर्ण *prefix,
			     संरचना repository *repo UNUSED)
{
	संरचना option options[] = {
		OPT_END()
	};
	अचर वर्ण *refname;

	argc = parse_options(argc, argv, prefix, options, reflog_exists_usage,
			     0);
	यदि (!argc)
		usage_with_options(reflog_exists_usage, options);

	refname = argv[0];
	यदि (check_refname_format(refname, REFNAME_ALLOW_ONELEVEL))
		die(_("invalid ref format: %s"), refname);
	फर्काऊ !refs_reflog_exists(get_main_ref_store(the_repository),
				   refname);
}

स्थिर पूर्णाङ्क cmd_reflog_drop(पूर्णाङ्क argc, अचर वर्ण **argv, अचर वर्ण *prefix,
			   संरचना repository *repo)
{
	पूर्णाङ्क ret = 0, do_all = 0, single_worktree = 0;
	अचर संरचना option options[] = {
		OPT_BOOL(0, "all", &do_all, N_("drop the reflogs of all references")),
		OPT_BOOL(0, "single-worktree", &single_worktree,
			 N_("drop reflogs from the current worktree only")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, reflog_drop_usage, 0);

	यदि (argc && do_all)
		usage(_("references specified along with --all"));

	यदि (do_all) {
		संरचना worktree_reflogs collected = {
			.reflogs = STRING_LIST_INIT_DUP,
		};
		संरचना string_list_item *item;
		संरचना worktree **worktrees, **p;

		worktrees = get_worktrees(the_repository);
		लागि (p = worktrees; *p; p++) {
			यदि (single_worktree && !(*p)->is_current)
				जारी;
			collected.worktree = *p;
			refs_for_each_reflog(get_worktree_ref_store(*p),
					     collect_reflog, &collected);
		}
		free_worktrees(worktrees);

		for_each_string_list_item(item, &collected.reflogs)
			ret |= refs_delete_reflog(get_main_ref_store(repo),
						     item->string);
		string_list_clear(&collected.reflogs, 0);

		फर्काऊ ret;
	}

	लागि (पूर्णाङ्क i = 0; i < argc; i++) {
		वर्ण *ref;
		यदि (!repo_dwim_log(repo, argv[i], म_माप(argv[i]), रिक्त, &ref)) {
			ret |= error(_("reflog could not be found: '%s'"), argv[i]);
			जारी;
		}

		ret |= refs_delete_reflog(get_main_ref_store(repo), ref);
		मुक्त(ref);
	}

	फर्काऊ ret;
}

स्थिर पूर्णाङ्क cmd_reflog_write(पूर्णाङ्क argc, अचर वर्ण **argv, अचर वर्ण *prefix,
			    संरचना repository *repo)
{
	अचर संरचना option options[] = {
		OPT_END()
	};
	संरचना object_id old_oid, new_oid;
	संरचना strbuf err = STRBUF_INIT;
	संरचना ref_transaction *tx;
	अचर वर्ण *ref, *message;
	पूर्णाङ्क ret;

	repo_config(repo, git_ident_config, रिक्त);

	argc = parse_options(argc, argv, prefix, options, reflog_write_usage, 0);
	यदि (argc != 4)
		usage_with_options(reflog_write_usage, options);

	ref = argv[0];
	यदि (!is_जरा_ref(ref) && check_refname_format(ref, 0))
		die(_("invalid reference name: %s"), ref);

	ret = get_oid_hex_algop(argv[1], &old_oid, repo->hash_algo);
	यदि (ret)
		die(_("invalid old object ID: '%s'"), argv[1]);
	यदि (!is_null_oid(&old_oid) && !odb_has_object(repo->objects, &old_oid, 0))
		die(_("old object '%s' does not exist"), argv[1]);

	ret = get_oid_hex_algop(argv[2], &new_oid, repo->hash_algo);
	यदि (ret)
		die(_("invalid new object ID: '%s'"), argv[2]);
	यदि (!is_null_oid(&new_oid) && !odb_has_object(repo->objects, &new_oid, 0))
		die(_("new object '%s' does not exist"), argv[2]);

	message = argv[3];

	tx = ref_store_transaction_begin(get_main_ref_store(repo), 0, &err);
	यदि (!tx)
		die(_("cannot start transaction: %s"), err.buf);

	ret = ref_transaction_update_reflog(tx, ref, &new_oid, &old_oid,
					    git_committer_info(0),
					    message, 0, &err);
	यदि (ret)
		die(_("cannot queue reflog update: %s"), err.buf);

	ret = ref_transaction_commit(tx, &err);
	यदि (ret)
		die(_("cannot commit reflog update: %s"), err.buf);

	ref_transaction_free(tx);
	strbuf_release(&err);
	फर्काऊ 0;
}

/*
 * main "reflog"
 */
पूर्णाङ्क cmd_reflog(पूर्णाङ्क argc,
	       अचर वर्ण **argv,
	       अचर वर्ण *prefix,
	       संरचना repository *repository)
{
	parse_opt_subcommand_fn *fn = रिक्त;
	संरचना option options[] = {
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
	यदि (fn)
		फर्काऊ fn(argc - 1, argv + 1, prefix, repository);
	अन्यथा
		फर्काऊ cmd_log_reflog(argc, argv, prefix, repository);
}
