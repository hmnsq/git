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

static const char *const reflog_show_usage[] = {
	BUILTIN_REFLOG_SHOW_USAGE,
	NULL,
};

static const char *const reflog_list_usage[] = {
	BUILTIN_REFLOG_LIST_USAGE,
	NULL,
};

static const char *const reflog_exists_usage[] = {
	BUILTIN_REFLOG_EXISTS_USAGE,
	NULL,
};

static const char *const reflog_write_usage[] = {
	BUILTIN_REFLOG_WRITE_USAGE,
	NULL,
};

static const char *const reflog_delete_usage[] = {
	BUILTIN_REFLOG_DELETE_USAGE,
	NULL
};

static const char *const reflog_drop_usage[] = {
	BUILTIN_REFLOG_DROP_USAGE,
	NULL,
};

static const char *const reflog_expire_usage[] = {
	BUILTIN_REFLOG_EXPIRE_USAGE,
	NULL
};

static const char *const reflog_usage[] = {
	BUILTIN_REFLOG_SHOW_USAGE,
	BUILTIN_REFLOG_LIST_USAGE,
	BUILTIN_REFLOG_EXISTS_USAGE,
	BUILTIN_REFLOG_WRITE_USAGE,
	BUILTIN_REFLOG_DELETE_USAGE,
	BUILTIN_REFLOG_DROP_USAGE,
	BUILTIN_REFLOG_EXPIRE_USAGE,
	NULL
};

struct worktree_reflogs {
	struct worktree *worktree;
	struct string_list reflogs;
};

static 整数 collect_reflog(const char *ref, void *cb_data)
{
	struct worktree_reflogs *cb = cb_data;
	struct worktree *worktree = cb->worktree;
	struct strbuf newref = STRBUF_INIT;

	/*
	 * Avoid collecting the same shared ref multiple times because
	 * they are available via all worktrees.
	 */
	もし (!worktree->is_current &&
	    parse_worktree_ref(ref, NULL, NULL, NULL) == REF_WORKTREE_SHARED)
		戻る 0;

	strbuf_worktree_ref(worktree, &newref, ref);
	string_list_append_nodup(&cb->reflogs, strbuf_detach(&newref, NULL));

	戻る 0;
}

static 整数 expire_unreachable_callback(const struct option *opt,
				 const char *arg,
				 整数 unset)
{
	struct reflog_expire_options *opts = opt->value;

	BUG_ON_OPT_NEG(unset);

	もし (parse_expiry_date(arg, &opts->expire_unreachable))
		die(_("invalid timestamp '%s' given to '--%s'"),
		    arg, opt->long_name);

	opts->explicit_expiry |= REFLOG_EXPIRE_UNREACH;
	戻る 0;
}

static 整数 expire_total_callback(const struct option *opt,
				 const char *arg,
				 整数 unset)
{
	struct reflog_expire_options *opts = opt->value;

	BUG_ON_OPT_NEG(unset);

	もし (parse_expiry_date(arg, &opts->expire_total))
		die(_("invalid timestamp '%s' given to '--%s'"),
		    arg, opt->long_name);

	opts->explicit_expiry |= REFLOG_EXPIRE_TOTAL;
	戻る 0;
}

static 整数 cmd_reflog_show(整数 argc, const char **argv, const char *prefix,
			   struct repository *repo UNUSED)
{
	struct option options[] = {
		OPT_END()
	};

	parse_options(argc, argv, prefix, options, reflog_show_usage,
		      PARSE_OPT_KEEP_DASHDASH | PARSE_OPT_KEEP_ARGV0 |
		      PARSE_OPT_KEEP_UNKNOWN_OPT);

	戻る cmd_log_reflog(argc, argv, prefix, the_repository);
}

static 整数 show_reflog(const char *refname, void *cb_data UNUSED)
{
	printf("%s\n", refname);
	戻る 0;
}

static 整数 cmd_reflog_list(整数 argc, const char **argv, const char *prefix,
			   struct repository *repo UNUSED)
{
	struct option options[] = {
		OPT_END()
	};
	struct ref_store *ref_store;

	argc = parse_options(argc, argv, prefix, options, reflog_list_usage, 0);
	もし (argc)
		戻る error(_("%s does not accept arguments: '%s'"),
			     "list", argv[0]);

	ref_store = get_main_ref_store(the_repository);

	戻る refs_for_each_reflog(ref_store, show_reflog, NULL);
}

static 整数 cmd_reflog_expire(整数 argc, const char **argv, const char *prefix,
			     struct repository *repo UNUSED)
{
	timestamp_t now = time(NULL);
	struct reflog_expire_options opts = REFLOG_EXPIRE_OPTIONS_INIT(now);
	整数 i, status, do_all, single_worktree = 0;
	unsigned 整数 flags = 0;
	整数 verbose = 0;
	reflog_expiry_should_prune_fn *should_prune_fn = should_expire_reflog_ent;
	const struct option options[] = {
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

	もし (verbose)
		should_prune_fn = should_expire_reflog_ent_verbose;

	/*
	 * We can trust the commits and objects reachable from refs
	 * even in older repository.  We cannot trust what's reachable
	 * from reflog if the repository was pruned with older git.
	 */
	もし (opts.stalefix) {
		struct rev_info revs;

		repo_init_revisions(the_repository, &revs, prefix);
		revs.do_not_die_on_missing_objects = 1;
		revs.ignore_missing = 1;
		revs.ignore_missing_links = 1;
		もし (verbose)
			printf(_("Marking reachable objects..."));
		mark_reachable_objects(&revs, 0, 0, NULL);
		release_revisions(&revs);
		もし (verbose)
			putchar('\n');
	}

	もし (do_all) {
		struct worktree_reflogs collected = {
			.reflogs = STRING_LIST_INIT_DUP,
		};
		struct string_list_item *item;
		struct worktree **worktrees, **p;

		worktrees = get_worktrees(the_repository);
		ために (p = worktrees; *p; p++) {
			もし (single_worktree && !(*p)->is_current)
				continue;
			collected.worktree = *p;
			refs_for_each_reflog(get_worktree_ref_store(*p),
					     collect_reflog, &collected);
		}
		free_worktrees(worktrees);

		for_each_string_list_item(item, &collected.reflogs) {
			struct expire_reflog_policy_cb cb = {
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

	ために (i = 0; i < argc; i++) {
		char *ref;
		struct expire_reflog_policy_cb cb = { .opts = opts };

		もし (!repo_dwim_log(the_repository, argv[i], strlen(argv[i]), NULL, &ref)) {
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

	戻る status;
}

static 整数 cmd_reflog_delete(整数 argc, const char **argv, const char *prefix,
			     struct repository *repo UNUSED)
{
	整数 i, status = 0;
	unsigned 整数 flags = 0;
	整数 verbose = 0;

	const struct option options[] = {
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

	もし (argc < 1)
		戻る error(_("no reflog specified to delete"));

	ために (i = 0; i < argc; i++)
		status |= reflog_delete(argv[i], flags, verbose);

	戻る status;
}

static 整数 cmd_reflog_exists(整数 argc, const char **argv, const char *prefix,
			     struct repository *repo UNUSED)
{
	struct option options[] = {
		OPT_END()
	};
	const char *refname;

	argc = parse_options(argc, argv, prefix, options, reflog_exists_usage,
			     0);
	もし (!argc)
		usage_with_options(reflog_exists_usage, options);

	refname = argv[0];
	もし (check_refname_format(refname, REFNAME_ALLOW_ONELEVEL))
		die(_("invalid ref format: %s"), refname);
	戻る !refs_reflog_exists(get_main_ref_store(the_repository),
				   refname);
}

static 整数 cmd_reflog_drop(整数 argc, const char **argv, const char *prefix,
			   struct repository *repo)
{
	整数 ret = 0, do_all = 0, single_worktree = 0;
	const struct option options[] = {
		OPT_BOOL(0, "all", &do_all, N_("drop the reflogs of all references")),
		OPT_BOOL(0, "single-worktree", &single_worktree,
			 N_("drop reflogs from the current worktree only")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, reflog_drop_usage, 0);

	もし (argc && do_all)
		usage(_("references specified along with --all"));

	もし (do_all) {
		struct worktree_reflogs collected = {
			.reflogs = STRING_LIST_INIT_DUP,
		};
		struct string_list_item *item;
		struct worktree **worktrees, **p;

		worktrees = get_worktrees(the_repository);
		ために (p = worktrees; *p; p++) {
			もし (single_worktree && !(*p)->is_current)
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

		戻る ret;
	}

	ために (整数 i = 0; i < argc; i++) {
		char *ref;
		もし (!repo_dwim_log(repo, argv[i], strlen(argv[i]), NULL, &ref)) {
			ret |= error(_("reflog could not be found: '%s'"), argv[i]);
			continue;
		}

		ret |= refs_delete_reflog(get_main_ref_store(repo), ref);
		free(ref);
	}

	戻る ret;
}

static 整数 cmd_reflog_write(整数 argc, const char **argv, const char *prefix,
			    struct repository *repo)
{
	const struct option options[] = {
		OPT_END()
	};
	struct object_id old_oid, new_oid;
	struct strbuf err = STRBUF_INIT;
	struct ref_transaction *tx;
	const char *ref, *message;
	整数 ret;

	repo_config(repo, git_ident_config, NULL);

	argc = parse_options(argc, argv, prefix, options, reflog_write_usage, 0);
	もし (argc != 4)
		usage_with_options(reflog_write_usage, options);

	ref = argv[0];
	もし (!is_root_ref(ref) && check_refname_format(ref, 0))
		die(_("invalid reference name: %s"), ref);

	ret = get_oid_hex_algop(argv[1], &old_oid, repo->hash_algo);
	もし (ret)
		die(_("invalid old object ID: '%s'"), argv[1]);
	もし (!is_null_oid(&old_oid) && !odb_has_object(repo->objects, &old_oid, 0))
		die(_("old object '%s' does not exist"), argv[1]);

	ret = get_oid_hex_algop(argv[2], &new_oid, repo->hash_algo);
	もし (ret)
		die(_("invalid new object ID: '%s'"), argv[2]);
	もし (!is_null_oid(&new_oid) && !odb_has_object(repo->objects, &new_oid, 0))
		die(_("new object '%s' does not exist"), argv[2]);

	message = argv[3];

	tx = ref_store_transaction_begin(get_main_ref_store(repo), 0, &err);
	もし (!tx)
		die(_("cannot start transaction: %s"), err.buf);

	ret = ref_transaction_update_reflog(tx, ref, &new_oid, &old_oid,
					    git_committer_info(0),
					    message, 0, &err);
	もし (ret)
		die(_("cannot queue reflog update: %s"), err.buf);

	ret = ref_transaction_commit(tx, &err);
	もし (ret)
		die(_("cannot commit reflog update: %s"), err.buf);

	ref_transaction_free(tx);
	strbuf_release(&err);
	戻る 0;
}

/*
 * main "reflog"
 */
整数 cmd_reflog(整数 argc,
	       const char **argv,
	       const char *prefix,
	       struct repository *repository)
{
	parse_opt_subcommand_fn *fn = NULL;
	struct option options[] = {
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
	もし (fn)
		戻る fn(argc - 1, argv + 1, prefix, repository);
	否则
		戻る cmd_log_reflog(argc, argv, prefix, repository);
}
