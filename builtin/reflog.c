#घोषणा USE_THE_REPOSITORY_VARIABLE

#समावेश "builtin.h"
#समावेश "config.h"
#समावेश "gettext.h"
#समावेश "hex.h"
#समावेश "odb.h"
#समावेश "revision.h"
#समावेश "reachable.h"
#समावेश "wildmatch.h"
#समावेश "worktree.h"
#समावेश "reflog.h"
#समावेश "refs.h"
#समावेश "parse-options.h"

#घोषणा BUILTIN_REFLOG_SHOW_USAGE \
	N_("git reflog [show] [<log-options>] [<ref>]")

#घोषणा BUILTIN_REFLOG_LIST_USAGE \
	N_("git reflog list")

#घोषणा BUILTIN_REFLOG_EXISTS_USAGE \
	N_("git reflog exists <ref>")

#घोषणा BUILTIN_REFLOG_WRITE_USAGE \
	N_("git reflog write <ref> <old-oid> <new-oid> <message>")

#घोषणा BUILTIN_REFLOG_DELETE_USAGE \
	N_("git reflog delete [--rewrite] [--updateref]\n" \
	   "                  [--dry-run | -n] [--verbose] <ref>@{<specifier>}...")

#घोषणा BUILTIN_REFLOG_DROP_USAGE \
	N_("git reflog drop [--all [--single-worktree] | <refs>...]")

#घोषणा BUILTIN_REFLOG_EXPIRE_USAGE \
	N_("git reflog expire [--expire=<time>] [--expire-unreachable=<time>]\n" \
	   "                  [--rewrite] [--updateref] [--stale-fix]\n" \
	   "                  [--dry-run | -n] [--verbose] [--all [--single-worktree] | <refs>...]")

अटल स्थिर अक्षर *स्थिर reflog_show_usage[] = {
	BUILTIN_REFLOG_SHOW_USAGE,
	शून्य,
};

अटल स्थिर अक्षर *स्थिर reflog_list_usage[] = {
	BUILTIN_REFLOG_LIST_USAGE,
	शून्य,
};

अटल स्थिर अक्षर *स्थिर reflog_exists_usage[] = {
	BUILTIN_REFLOG_EXISTS_USAGE,
	शून्य,
};

अटल स्थिर अक्षर *स्थिर reflog_write_usage[] = {
	BUILTIN_REFLOG_WRITE_USAGE,
	शून्य,
};

अटल स्थिर अक्षर *स्थिर reflog_delete_usage[] = {
	BUILTIN_REFLOG_DELETE_USAGE,
	शून्य
};

अटल स्थिर अक्षर *स्थिर reflog_drop_usage[] = {
	BUILTIN_REFLOG_DROP_USAGE,
	शून्य,
};

अटल स्थिर अक्षर *स्थिर reflog_expire_usage[] = {
	BUILTIN_REFLOG_EXPIRE_USAGE,
	शून्य
};

अटल स्थिर अक्षर *स्थिर reflog_usage[] = {
	BUILTIN_REFLOG_SHOW_USAGE,
	BUILTIN_REFLOG_LIST_USAGE,
	BUILTIN_REFLOG_EXISTS_USAGE,
	BUILTIN_REFLOG_WRITE_USAGE,
	BUILTIN_REFLOG_DELETE_USAGE,
	BUILTIN_REFLOG_DROP_USAGE,
	BUILTIN_REFLOG_EXPIRE_USAGE,
	शून्य
};

काष्ठा worktree_reflogs {
	काष्ठा worktree *worktree;
	काष्ठा string_list reflogs;
};

अटल पूर्णांक collect_reflog(स्थिर अक्षर *ref, व्योम *cb_तथ्य)
{
	काष्ठा worktree_reflogs *cb = cb_तथ्य;
	काष्ठा worktree *worktree = cb->worktree;
	काष्ठा strbuf newref = STRBUF_INIT;

	/*
	 * Avoid collecting the same shared ref multiple times because
	 * they are available via all worktrees.
	 */
	अगर (!worktree->is_current &&
	    parse_worktree_ref(ref, शून्य, शून्य, शून्य) == REF_WORKTREE_SHARED)
		वापस 0;

	strbuf_worktree_ref(worktree, &newref, ref);
	string_list_append_nodup(&cb->reflogs, strbuf_detach(&newref, शून्य));

	वापस 0;
}

अटल पूर्णांक expire_unreachable_callback(स्थिर काष्ठा option *opt,
				 स्थिर अक्षर *arg,
				 पूर्णांक unset)
{
	काष्ठा reflog_expire_options *opts = opt->मूल्य;

	BUG_ON_OPT_NEG(unset);

	अगर (parse_expiry_date(arg, &opts->expire_unreachable))
		die(_("invalid timestamp '%s' given to '--%s'"),
		    arg, opt->long_name);

	opts->explicit_expiry |= REFLOG_EXPIRE_UNREACH;
	वापस 0;
}

अटल पूर्णांक expire_योग_callback(स्थिर काष्ठा option *opt,
				 स्थिर अक्षर *arg,
				 पूर्णांक unset)
{
	काष्ठा reflog_expire_options *opts = opt->मूल्य;

	BUG_ON_OPT_NEG(unset);

	अगर (parse_expiry_date(arg, &opts->expire_योग))
		die(_("invalid timestamp '%s' given to '--%s'"),
		    arg, opt->long_name);

	opts->explicit_expiry |= REFLOG_EXPIRE_योग;
	वापस 0;
}

अटल पूर्णांक cmd_reflog_show(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
			   काष्ठा repository *repo UNUSED)
{
	काष्ठा option options[] = {
		OPT_END()
	};

	parse_options(argc, argv, prefix, options, reflog_show_usage,
		      PARSE_OPT_KEEP_DASHDASH | PARSE_OPT_KEEP_ARGV0 |
		      PARSE_OPT_KEEP_UNKNOWN_OPT);

	वापस cmd_log_reflog(argc, argv, prefix, the_repository);
}

अटल पूर्णांक show_reflog(स्थिर अक्षर *refname, व्योम *cb_तथ्य UNUSED)
{
	म_लिखो("%s\n", refname);
	वापस 0;
}

अटल पूर्णांक cmd_reflog_list(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
			   काष्ठा repository *repo UNUSED)
{
	काष्ठा option options[] = {
		OPT_END()
	};
	काष्ठा ref_store *ref_store;

	argc = parse_options(argc, argv, prefix, options, reflog_list_usage, 0);
	अगर (argc)
		वापस error(_("%s does not accept arguments: '%s'"),
			     "list", argv[0]);

	ref_store = get_main_ref_store(the_repository);

	वापस refs_for_each_reflog(ref_store, show_reflog, शून्य);
}

अटल पूर्णांक cmd_reflog_expire(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
			     काष्ठा repository *repo UNUSED)
{
	timestamp_t now = समय(शून्य);
	काष्ठा reflog_expire_options opts = REFLOG_EXPIRE_OPTIONS_INIT(now);
	पूर्णांक i, status, do_all, single_worktree = 0;
	अचिन्हित पूर्णांक flags = 0;
	पूर्णांक verbose = 0;
	reflog_expiry_should_prune_fn *should_prune_fn = should_expire_reflog_ent;
	स्थिर काष्ठा option options[] = {
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
			       expire_योग_callback),
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

	save_commit_बफर = 0;
	do_all = status = 0;

	argc = parse_options(argc, argv, prefix, options, reflog_expire_usage, 0);

	अगर (verbose)
		should_prune_fn = should_expire_reflog_ent_verbose;

	/*
	 * We can trust the commits and objects reachable from refs
	 * even in older repository.  We cannot trust what's reachable
	 * from reflog if the repository was pruned with older git.
	 */
	अगर (opts.stalefix) {
		काष्ठा rev_info revs;

		repo_init_revisions(the_repository, &revs, prefix);
		revs.do_not_die_on_missing_objects = 1;
		revs.ignore_missing = 1;
		revs.ignore_missing_links = 1;
		अगर (verbose)
			म_लिखो(_("Marking reachable objects..."));
		mark_reachable_objects(&revs, 0, 0, शून्य);
		release_revisions(&revs);
		अगर (verbose)
			अक्षर_दो('\n');
	}

	अगर (do_all) {
		काष्ठा worktree_reflogs collected = {
			.reflogs = STRING_LIST_INIT_DUP,
		};
		काष्ठा string_list_item *item;
		काष्ठा worktree **worktrees, **p;

		worktrees = get_worktrees(the_repository);
		क्रम (p = worktrees; *p; p++) {
			अगर (single_worktree && !(*p)->is_current)
				जारी;
			collected.worktree = *p;
			refs_for_each_reflog(get_worktree_ref_store(*p),
					     collect_reflog, &collected);
		}
		free_worktrees(worktrees);

		for_each_string_list_item(item, &collected.reflogs) {
			काष्ठा expire_reflog_policy_cb cb = {
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

	क्रम (i = 0; i < argc; i++) {
		अक्षर *ref;
		काष्ठा expire_reflog_policy_cb cb = { .opts = opts };

		अगर (!repo_dwim_log(the_repository, argv[i], म_माप(argv[i]), शून्य, &ref)) {
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

	वापस status;
}

अटल पूर्णांक cmd_reflog_delete(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
			     काष्ठा repository *repo UNUSED)
{
	पूर्णांक i, status = 0;
	अचिन्हित पूर्णांक flags = 0;
	पूर्णांक verbose = 0;

	स्थिर काष्ठा option options[] = {
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

	अगर (argc < 1)
		वापस error(_("no reflog specified to delete"));

	क्रम (i = 0; i < argc; i++)
		status |= reflog_delete(argv[i], flags, verbose);

	वापस status;
}

अटल पूर्णांक cmd_reflog_exists(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
			     काष्ठा repository *repo UNUSED)
{
	काष्ठा option options[] = {
		OPT_END()
	};
	स्थिर अक्षर *refname;

	argc = parse_options(argc, argv, prefix, options, reflog_exists_usage,
			     0);
	अगर (!argc)
		usage_with_options(reflog_exists_usage, options);

	refname = argv[0];
	अगर (check_refname_format(refname, REFNAME_ALLOW_ONELEVEL))
		die(_("invalid ref format: %s"), refname);
	वापस !refs_reflog_exists(get_main_ref_store(the_repository),
				   refname);
}

अटल पूर्णांक cmd_reflog_drop(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
			   काष्ठा repository *repo)
{
	पूर्णांक ret = 0, do_all = 0, single_worktree = 0;
	स्थिर काष्ठा option options[] = {
		OPT_BOOL(0, "all", &do_all, N_("drop the reflogs of all references")),
		OPT_BOOL(0, "single-worktree", &single_worktree,
			 N_("drop reflogs from the current worktree only")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, reflog_drop_usage, 0);

	अगर (argc && do_all)
		usage(_("references specified along with --all"));

	अगर (do_all) {
		काष्ठा worktree_reflogs collected = {
			.reflogs = STRING_LIST_INIT_DUP,
		};
		काष्ठा string_list_item *item;
		काष्ठा worktree **worktrees, **p;

		worktrees = get_worktrees(the_repository);
		क्रम (p = worktrees; *p; p++) {
			अगर (single_worktree && !(*p)->is_current)
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

		वापस ret;
	}

	क्रम (पूर्णांक i = 0; i < argc; i++) {
		अक्षर *ref;
		अगर (!repo_dwim_log(repo, argv[i], म_माप(argv[i]), शून्य, &ref)) {
			ret |= error(_("reflog could not be found: '%s'"), argv[i]);
			जारी;
		}

		ret |= refs_delete_reflog(get_main_ref_store(repo), ref);
		मुक्त(ref);
	}

	वापस ret;
}

अटल पूर्णांक cmd_reflog_write(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
			    काष्ठा repository *repo)
{
	स्थिर काष्ठा option options[] = {
		OPT_END()
	};
	काष्ठा object_id old_oid, new_oid;
	काष्ठा strbuf err = STRBUF_INIT;
	काष्ठा ref_transaction *tx;
	स्थिर अक्षर *ref, *message;
	पूर्णांक ret;

	repo_config(repo, git_ident_config, शून्य);

	argc = parse_options(argc, argv, prefix, options, reflog_write_usage, 0);
	अगर (argc != 4)
		usage_with_options(reflog_write_usage, options);

	ref = argv[0];
	अगर (!is_मूल_ref(ref) && check_refname_format(ref, 0))
		die(_("invalid reference name: %s"), ref);

	ret = get_oid_hex_algop(argv[1], &old_oid, repo->hash_algo);
	अगर (ret)
		die(_("invalid old object ID: '%s'"), argv[1]);
	अगर (!is_null_oid(&old_oid) && !odb_has_object(repo->objects, &old_oid, 0))
		die(_("old object '%s' does not exist"), argv[1]);

	ret = get_oid_hex_algop(argv[2], &new_oid, repo->hash_algo);
	अगर (ret)
		die(_("invalid new object ID: '%s'"), argv[2]);
	अगर (!is_null_oid(&new_oid) && !odb_has_object(repo->objects, &new_oid, 0))
		die(_("new object '%s' does not exist"), argv[2]);

	message = argv[3];

	tx = ref_store_transaction_begin(get_main_ref_store(repo), 0, &err);
	अगर (!tx)
		die(_("cannot start transaction: %s"), err.buf);

	ret = ref_transaction_update_reflog(tx, ref, &new_oid, &old_oid,
					    git_committer_info(0),
					    message, 0, &err);
	अगर (ret)
		die(_("cannot queue reflog update: %s"), err.buf);

	ret = ref_transaction_commit(tx, &err);
	अगर (ret)
		die(_("cannot commit reflog update: %s"), err.buf);

	ref_transaction_free(tx);
	strbuf_release(&err);
	वापस 0;
}

/*
 * main "reflog"
 */
पूर्णांक cmd_reflog(पूर्णांक argc,
	       स्थिर अक्षर **argv,
	       स्थिर अक्षर *prefix,
	       काष्ठा repository *repository)
{
	parse_opt_subcommand_fn *fn = शून्य;
	काष्ठा option options[] = {
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
	अगर (fn)
		वापस fn(argc - 1, argv + 1, prefix, repository);
	अन्यथा
		वापस cmd_log_reflog(argc, argv, prefix, repository);
}
