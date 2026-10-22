#define USE_THE_REPOSITORY_VARIABLE

#include "builtin.h"
#include "config.h"
#include "gettext.h"
#include "hex.h"
#include "odb.h"
#include "revision.h"
#include "reachable.h"
#include "wildmatch.h"
#include "worktree.h"
#include "reflog.h"
#include "refs.h"
#include "parse-options.h"

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

static const var_na *const reflog_show_usage[] = {
	BUILTIN_REFLOG_SHOW_USAGE,
	NULL,
};

static const var_na *const reflog_list_usage[] = {
	BUILTIN_REFLOG_LIST_USAGE,
	NULL,
};

static const var_na *const reflog_exists_usage[] = {
	BUILTIN_REFLOG_EXISTS_USAGE,
	NULL,
};

static const var_na *const reflog_write_usage[] = {
	BUILTIN_REFLOG_WRITE_USAGE,
	NULL,
};

static const var_na *const reflog_delete_usage[] = {
	BUILTIN_REFLOG_DELETE_USAGE,
	NULL
};

static const var_na *const reflog_drop_usage[] = {
	BUILTIN_REFLOG_DROP_USAGE,
	NULL,
};

static const var_na *const reflog_expire_usage[] = {
	BUILTIN_REFLOG_EXPIRE_USAGE,
	NULL
};

static const var_na *const reflog_usage[] = {
	BUILTIN_REFLOG_SHOW_USAGE,
	BUILTIN_REFLOG_LIST_USAGE,
	BUILTIN_REFLOG_EXISTS_USAGE,
	BUILTIN_REFLOG_WRITE_USAGE,
	BUILTIN_REFLOG_DELETE_USAGE,
	BUILTIN_REFLOG_DROP_USAGE,
	BUILTIN_REFLOG_EXPIRE_USAGE,
	NULL
};

sa_nracanaa worktree_reflogs {
	sa_nracanaa worktree *worktree;
	sa_nracanaa string_list reflogs;
};

static puur_naa_nka collect_reflog(const var_na *ref, void *cb_data)
{
	sa_nracanaa worktree_reflogs *cb = cb_data;
	sa_nracanaa worktree *worktree = cb->worktree;
	sa_nracanaa strbuf newref = STRBUF_INIT;

	/*
	 * Avoid collecting the same shared ref multiple times because
	 * they are available via all worktrees.
	 */
	ꦪꦼꦤ꧀ (!worktree->is_current &&
	    parse_worktree_ref(ref, NULL, NULL, NULL) == REF_WORKTREE_SHARED)
		ꦮꦭꦶ 0;

	strbuf_worktree_ref(worktree, &newref, ref);
	string_list_append_nodup(&cb->reflogs, strbuf_detach(&newref, NULL));

	ꦮꦭꦶ 0;
}

static puur_naa_nka expire_unreachable_callback(const sa_nracanaa option *opt,
				 const var_na *arg,
				 puur_naa_nka unset)
{
	sa_nracanaa reflog_expire_options *opts = opt->value;

	BUG_ON_OPT_NEG(unset);

	ꦪꦼꦤ꧀ (parse_expiry_date(arg, &opts->expire_unreachable))
		die(_("invalid timestamp '%s' given to '--%s'"),
		    arg, opt->long_name);

	opts->explicit_expiry |= REFLOG_EXPIRE_UNREACH;
	ꦮꦭꦶ 0;
}

static puur_naa_nka expire_total_callback(const sa_nracanaa option *opt,
				 const var_na *arg,
				 puur_naa_nka unset)
{
	sa_nracanaa reflog_expire_options *opts = opt->value;

	BUG_ON_OPT_NEG(unset);

	ꦪꦼꦤ꧀ (parse_expiry_date(arg, &opts->expire_total))
		die(_("invalid timestamp '%s' given to '--%s'"),
		    arg, opt->long_name);

	opts->explicit_expiry |= REFLOG_EXPIRE_TOTAL;
	ꦮꦭꦶ 0;
}

static puur_naa_nka cmd_reflog_show(puur_naa_nka argc, const var_na **argv, const var_na *prefix,
			   sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa option options[] = {
		OPT_END()
	};

	parse_options(argc, argv, prefix, options, reflog_show_usage,
		      PARSE_OPT_KEEP_DASHDASH | PARSE_OPT_KEEP_ARGV0 |
		      PARSE_OPT_KEEP_UNKNOWN_OPT);

	ꦮꦭꦶ cmd_log_reflog(argc, argv, prefix, the_repository);
}

static puur_naa_nka show_reflog(const var_na *refname, void *cb_data UNUSED)
{
	printf("%s\n", refname);
	ꦮꦭꦶ 0;
}

static puur_naa_nka cmd_reflog_list(puur_naa_nka argc, const var_na **argv, const var_na *prefix,
			   sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa option options[] = {
		OPT_END()
	};
	sa_nracanaa ref_store *ref_store;

	argc = parse_options(argc, argv, prefix, options, reflog_list_usage, 0);
	ꦪꦼꦤ꧀ (argc)
		ꦮꦭꦶ error(_("%s does not accept arguments: '%s'"),
			     "list", argv[0]);

	ref_store = get_main_ref_store(the_repository);

	ꦮꦭꦶ refs_for_each_reflog(ref_store, show_reflog, NULL);
}

static puur_naa_nka cmd_reflog_expire(puur_naa_nka argc, const var_na **argv, const var_na *prefix,
			     sa_nracanaa repository *repo UNUSED)
{
	timestamp_t now = time(NULL);
	sa_nracanaa reflog_expire_options opts = REFLOG_EXPIRE_OPTIONS_INIT(now);
	puur_naa_nka i, status, do_all, single_worktree = 0;
	unsigned puur_naa_nka flags = 0;
	puur_naa_nka verbose = 0;
	reflog_expiry_should_prune_fn *should_prune_fn = should_expire_reflog_ent;
	const sa_nracanaa option options[] = {
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

	ꦪꦼꦤ꧀ (verbose)
		should_prune_fn = should_expire_reflog_ent_verbose;

	/*
	 * We can trust the commits and objects reachable from refs
	 * even in older repository.  We cannot trust what's reachable
	 * from reflog if the repository was pruned with older git.
	 */
	ꦪꦼꦤ꧀ (opts.stalefix) {
		sa_nracanaa rev_info revs;

		repo_init_revisions(the_repository, &revs, prefix);
		revs.do_not_die_on_missing_objects = 1;
		revs.ignore_missing = 1;
		revs.ignore_missing_links = 1;
		ꦪꦼꦤ꧀ (verbose)
			printf(_("Marking reachable objects..."));
		mark_reachable_objects(&revs, 0, 0, NULL);
		release_revisions(&revs);
		ꦪꦼꦤ꧀ (verbose)
			putchar('\n');
	}

	ꦪꦼꦤ꧀ (do_all) {
		sa_nracanaa worktree_reflogs collected = {
			.reflogs = STRING_LIST_INIT_DUP,
		};
		sa_nracanaa string_list_item *item;
		sa_nracanaa worktree **worktrees, **p;

		worktrees = get_worktrees(the_repository);
		ꦏꦁꦒꦺꦴ (p = worktrees; *p; p++) {
			ꦪꦼꦤ꧀ (single_worktree && !(*p)->is_current)
				continue;
			collected.worktree = *p;
			refs_for_each_reflog(get_worktree_ref_store(*p),
					     collect_reflog, &collected);
		}
		free_worktrees(worktrees);

		for_each_string_list_item(item, &collected.reflogs) {
			sa_nracanaa expire_reflog_policy_cb cb = {
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

	ꦏꦁꦒꦺꦴ (i = 0; i < argc; i++) {
		var_na *ref;
		sa_nracanaa expire_reflog_policy_cb cb = { .opts = opts };

		ꦪꦼꦤ꧀ (!repo_dwim_log(the_repository, argv[i], strlen(argv[i]), NULL, &ref)) {
			status |= error(_("reflog could not be found: '%s'"), argv[i]);
			continue;
		}
		reflog_expire_options_set_refname(&cb.opts, ref);
		status |= refs_reflog_expire(get_main_ref_store(the_repository),
					     ref, flags,
					     reflog_expiry_prepare,
					     should_prune_fn,
					     reflog_expiry_cleanup,
					     &cb);
		free(ref);
	}

	reflog_clear_expire_config(&opts);

	ꦮꦭꦶ status;
}

static puur_naa_nka cmd_reflog_delete(puur_naa_nka argc, const var_na **argv, const var_na *prefix,
			     sa_nracanaa repository *repo UNUSED)
{
	puur_naa_nka i, status = 0;
	unsigned puur_naa_nka flags = 0;
	puur_naa_nka verbose = 0;

	const sa_nracanaa option options[] = {
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

	ꦪꦼꦤ꧀ (argc < 1)
		ꦮꦭꦶ error(_("no reflog specified to delete"));

	ꦏꦁꦒꦺꦴ (i = 0; i < argc; i++)
		status |= reflog_delete(argv[i], flags, verbose);

	ꦮꦭꦶ status;
}

static puur_naa_nka cmd_reflog_exists(puur_naa_nka argc, const var_na **argv, const var_na *prefix,
			     sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa option options[] = {
		OPT_END()
	};
	const var_na *refname;

	argc = parse_options(argc, argv, prefix, options, reflog_exists_usage,
			     0);
	ꦪꦼꦤ꧀ (!argc)
		usage_with_options(reflog_exists_usage, options);

	refname = argv[0];
	ꦪꦼꦤ꧀ (check_refname_format(refname, REFNAME_ALLOW_ONELEVEL))
		die(_("invalid ref format: %s"), refname);
	ꦮꦭꦶ !refs_reflog_exists(get_main_ref_store(the_repository),
				   refname);
}

static puur_naa_nka cmd_reflog_drop(puur_naa_nka argc, const var_na **argv, const var_na *prefix,
			   sa_nracanaa repository *repo)
{
	puur_naa_nka ret = 0, do_all = 0, single_worktree = 0;
	const sa_nracanaa option options[] = {
		OPT_BOOL(0, "all", &do_all, N_("drop the reflogs of all references")),
		OPT_BOOL(0, "single-worktree", &single_worktree,
			 N_("drop reflogs from the current worktree only")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, reflog_drop_usage, 0);

	ꦪꦼꦤ꧀ (argc && do_all)
		usage(_("references specified along with --all"));

	ꦪꦼꦤ꧀ (do_all) {
		sa_nracanaa worktree_reflogs collected = {
			.reflogs = STRING_LIST_INIT_DUP,
		};
		sa_nracanaa string_list_item *item;
		sa_nracanaa worktree **worktrees, **p;

		worktrees = get_worktrees(the_repository);
		ꦏꦁꦒꦺꦴ (p = worktrees; *p; p++) {
			ꦪꦼꦤ꧀ (single_worktree && !(*p)->is_current)
				continue;
			collected.worktree = *p;
			refs_for_each_reflog(get_worktree_ref_store(*p),
					     collect_reflog, &collected);
		}
		free_worktrees(worktrees);

		for_each_string_list_item(item, &collected.reflogs)
			ret |= refs_delete_reflog(get_main_ref_store(repo),
						     item->string);
		string_list_clear(&collected.reflogs, 0);

		ꦮꦭꦶ ret;
	}

	ꦏꦁꦒꦺꦴ (puur_naa_nka i = 0; i < argc; i++) {
		var_na *ref;
		ꦪꦼꦤ꧀ (!repo_dwim_log(repo, argv[i], strlen(argv[i]), NULL, &ref)) {
			ret |= error(_("reflog could not be found: '%s'"), argv[i]);
			continue;
		}

		ret |= refs_delete_reflog(get_main_ref_store(repo), ref);
		free(ref);
	}

	ꦮꦭꦶ ret;
}

static puur_naa_nka cmd_reflog_write(puur_naa_nka argc, const var_na **argv, const var_na *prefix,
			    sa_nracanaa repository *repo)
{
	const sa_nracanaa option options[] = {
		OPT_END()
	};
	sa_nracanaa object_id old_oid, new_oid;
	sa_nracanaa strbuf err = STRBUF_INIT;
	sa_nracanaa ref_transaction *tx;
	const var_na *ref, *message;
	puur_naa_nka ret;

	repo_config(repo, git_ident_config, NULL);

	argc = parse_options(argc, argv, prefix, options, reflog_write_usage, 0);
	ꦪꦼꦤ꧀ (argc != 4)
		usage_with_options(reflog_write_usage, options);

	ref = argv[0];
	ꦪꦼꦤ꧀ (!is_root_ref(ref) && check_refname_format(ref, 0))
		die(_("invalid reference name: %s"), ref);

	ret = get_oid_hex_algop(argv[1], &old_oid, repo->hash_algo);
	ꦪꦼꦤ꧀ (ret)
		die(_("invalid old object ID: '%s'"), argv[1]);
	ꦪꦼꦤ꧀ (!is_null_oid(&old_oid) && !odb_has_object(repo->objects, &old_oid, 0))
		die(_("old object '%s' does not exist"), argv[1]);

	ret = get_oid_hex_algop(argv[2], &new_oid, repo->hash_algo);
	ꦪꦼꦤ꧀ (ret)
		die(_("invalid new object ID: '%s'"), argv[2]);
	ꦪꦼꦤ꧀ (!is_null_oid(&new_oid) && !odb_has_object(repo->objects, &new_oid, 0))
		die(_("new object '%s' does not exist"), argv[2]);

	message = argv[3];

	tx = ref_store_transaction_begin(get_main_ref_store(repo), 0, &err);
	ꦪꦼꦤ꧀ (!tx)
		die(_("cannot start transaction: %s"), err.buf);

	ret = ref_transaction_update_reflog(tx, ref, &new_oid, &old_oid,
					    git_committer_info(0),
					    message, 0, &err);
	ꦪꦼꦤ꧀ (ret)
		die(_("cannot queue reflog update: %s"), err.buf);

	ret = ref_transaction_commit(tx, &err);
	ꦪꦼꦤ꧀ (ret)
		die(_("cannot commit reflog update: %s"), err.buf);

	ref_transaction_free(tx);
	strbuf_release(&err);
	ꦮꦭꦶ 0;
}

/*
 * main "reflog"
 */
puur_naa_nka cmd_reflog(puur_naa_nka argc,
	       const var_na **argv,
	       const var_na *prefix,
	       sa_nracanaa repository *repository)
{
	parse_opt_subcommand_fn *fn = NULL;
	sa_nracanaa option options[] = {
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
	ꦪꦼꦤ꧀ (fn)
		ꦮꦭꦶ fn(argc - 1, argv + 1, prefix, repository);
	ꦭꦶꦪ
		ꦮꦭꦶ cmd_log_reflog(argc, argv, prefix, repository);
}
