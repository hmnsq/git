#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE

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

#ವ್ಯಾಖ್ಯಾನಿಸು BUILTIN_REFLOG_SHOW_USAGE \
	N_("git reflog [show] [<log-options>] [<ref>]")

#ವ್ಯಾಖ್ಯಾನಿಸು BUILTIN_REFLOG_LIST_USAGE \
	N_("git reflog list")

#ವ್ಯಾಖ್ಯಾನಿಸು BUILTIN_REFLOG_EXISTS_USAGE \
	N_("git reflog exists <ref>")

#ವ್ಯಾಖ್ಯಾನಿಸು BUILTIN_REFLOG_WRITE_USAGE \
	N_("git reflog write <ref> <old-oid> <new-oid> <message>")

#ವ್ಯಾಖ್ಯಾನಿಸು BUILTIN_REFLOG_DELETE_USAGE \
	N_("git reflog delete [--rewrite] [--updateref]\n" \
	   "                  [--dry-run | -n] [--verbose] <ref>@{<specifier>}...")

#ವ್ಯಾಖ್ಯಾನಿಸು BUILTIN_REFLOG_DROP_USAGE \
	N_("git reflog drop [--all [--single-worktree] | <refs>...]")

#ವ್ಯಾಖ್ಯಾನಿಸು BUILTIN_REFLOG_EXPIRE_USAGE \
	N_("git reflog expire [--expire=<time>] [--expire-unreachable=<time>]\n" \
	   "                  [--rewrite] [--updateref] [--stale-fix]\n" \
	   "                  [--dry-run | -n] [--verbose] [--all [--single-worktree] | <refs>...]")

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ reflog_show_usage[] = {
	BUILTIN_REFLOG_SHOW_USAGE,
	ಖಾಲಿ,
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ reflog_list_usage[] = {
	BUILTIN_REFLOG_LIST_USAGE,
	ಖಾಲಿ,
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ reflog_exists_usage[] = {
	BUILTIN_REFLOG_EXISTS_USAGE,
	ಖಾಲಿ,
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ reflog_write_usage[] = {
	BUILTIN_REFLOG_WRITE_USAGE,
	ಖಾಲಿ,
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ reflog_delete_usage[] = {
	BUILTIN_REFLOG_DELETE_USAGE,
	ಖಾಲಿ
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ reflog_drop_usage[] = {
	BUILTIN_REFLOG_DROP_USAGE,
	ಖಾಲಿ,
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ reflog_expire_usage[] = {
	BUILTIN_REFLOG_EXPIRE_USAGE,
	ಖಾಲಿ
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ reflog_usage[] = {
	BUILTIN_REFLOG_SHOW_USAGE,
	BUILTIN_REFLOG_LIST_USAGE,
	BUILTIN_REFLOG_EXISTS_USAGE,
	BUILTIN_REFLOG_WRITE_USAGE,
	BUILTIN_REFLOG_DELETE_USAGE,
	BUILTIN_REFLOG_DROP_USAGE,
	BUILTIN_REFLOG_EXPIRE_USAGE,
	ಖಾಲಿ
};

ರಚನೆ worktree_reflogs {
	ರಚನೆ worktree *worktree;
	ರಚನೆ string_list reflogs;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ collect_reflog(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ref, ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ)
{
	ರಚನೆ worktree_reflogs *cb = cb_ದತ್ತಾಂಶ;
	ರಚನೆ worktree *worktree = cb->worktree;
	ರಚನೆ strbuf newref = STRBUF_INIT;

	/*
	 * Avoid collecting the same shared ref multiple times because
	 * they are available via all worktrees.
	 */
	ಆದರೆ (!worktree->is_current &&
	    parse_worktree_ref(ref, ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ) == REF_WORKTREE_SHARED)
		ಹಿಂತಿರುಗಿ 0;

	strbuf_worktree_ref(worktree, &newref, ref);
	string_list_append_nodup(&cb->reflogs, strbuf_detach(&newref, ಖಾಲಿ));

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ expire_unreachable_callback(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt,
				 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg,
				 ಪೂರ್ಣಾಂಕ unset)
{
	ರಚನೆ reflog_expire_options *opts = opt->ಮೌಲ್ಯ;

	BUG_ON_OPT_NEG(unset);

	ಆದರೆ (parse_expiry_date(arg, &opts->expire_unreachable))
		die(_("invalid timestamp '%s' given to '--%s'"),
		    arg, opt->long_name);

	opts->explicit_expiry |= REFLOG_EXPIRE_UNREACH;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ expire_ಒಟ್ಟು_callback(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt,
				 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg,
				 ಪೂರ್ಣಾಂಕ unset)
{
	ರಚನೆ reflog_expire_options *opts = opt->ಮೌಲ್ಯ;

	BUG_ON_OPT_NEG(unset);

	ಆದರೆ (parse_expiry_date(arg, &opts->expire_ಒಟ್ಟು))
		die(_("invalid timestamp '%s' given to '--%s'"),
		    arg, opt->long_name);

	opts->explicit_expiry |= REFLOG_EXPIRE_ಒಟ್ಟು;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cmd_reflog_show(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			   ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ option options[] = {
		OPT_ಕೊನೆ()
	};

	parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options, reflog_show_usage,
		      PARSE_OPT_KEEP_DASHDASH | PARSE_OPT_KEEP_ARGV0 |
		      PARSE_OPT_KEEP_UNKNOWN_OPT);

	ಹಿಂತಿರುಗಿ cmd_log_reflog(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, the_repository);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_reflog(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *refname, ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ ಬಳಸದ)
{
	ಬರೆ("%s\n", refname);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cmd_reflog_list(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			   ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ option options[] = {
		OPT_ಕೊನೆ()
	};
	ರಚನೆ ref_store *ref_store;

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options, reflog_list_usage, 0);
	ಆದರೆ (argc)
		ಹಿಂತಿರುಗಿ error(_("%s does not accept arguments: '%s'"),
			     "list", argv[0]);

	ref_store = get_main_ref_store(the_repository);

	ಹಿಂತಿರುಗಿ refs_for_each_reflog(ref_store, show_reflog, ಖಾಲಿ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cmd_reflog_expire(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			     ರಚನೆ repository *repo ಬಳಸದ)
{
	timestamp_t now = ಸಮಯ(ಖಾಲಿ);
	ರಚನೆ reflog_expire_options opts = REFLOG_EXPIRE_OPTIONS_INIT(now);
	ಪೂರ್ಣಾಂಕ i, ಸ್ಥಿತಿ_2, do_all, single_worktree = 0;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags = 0;
	ಪೂರ್ಣಾಂಕ verbose = 0;
	reflog_expiry_should_prune_fn *should_prune_fn = should_expire_reflog_ent;
	ಸ್ಥಿರಾಂಕ ರಚನೆ option options[] = {
		OPT_ಬಿಟ್('n', "dry-run", &flags, N_("do not actually prune any entries"),
			EXPIRE_REFLOGS_DRY_ಸತತ),
		OPT_ಬಿಟ್(0, "rewrite", &flags,
			N_("rewrite the old SHA1 with the new SHA1 of the entry that now precedes it"),
			EXPIRE_REFLOGS_REWRITE),
		OPT_ಬಿಟ್(0, "updateref", &flags,
			N_("update the reference to the value of the top reflog entry"),
			EXPIRE_REFLOGS_UPDATE_REF),
		OPT_BOOL(0, "verbose", &verbose, N_("print extra information on screen")),
		OPT_CALLBACK_F(0, "expire", &opts, N_("timestamp"),
			       N_("prune entries older than the specified time"),
			       PARSE_OPT_NONEG,
			       expire_ಒಟ್ಟು_callback),
		OPT_CALLBACK_F(0, "expire-unreachable", &opts, N_("timestamp"),
			       N_("prune entries older than <time> that are not reachable from the current tip of the branch"),
			       PARSE_OPT_NONEG,
			       expire_unreachable_callback),
		OPT_BOOL(0, "stale-fix", &opts.stalefix,
			 N_("prune any reflog entries that point to broken commits")),
		OPT_BOOL(0, "all", &do_all, N_("process the reflogs of all references")),
		OPT_BOOL(0, "single-worktree", &single_worktree,
			 N_("limits processing to reflogs from the current worktree only")),
		OPT_ಕೊನೆ()
	};

	repo_config(the_repository, reflog_expire_config, &opts);

	save_commit_ಬಫರ್ = 0;
	do_all = ಸ್ಥಿತಿ_2 = 0;

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options, reflog_expire_usage, 0);

	ಆದರೆ (verbose)
		should_prune_fn = should_expire_reflog_ent_verbose;

	/*
	 * We can trust the commits and objects reachable from refs
	 * even in older repository.  We cannot trust what's reachable
	 * from reflog if the repository was pruned with older git.
	 */
	ಆದರೆ (opts.stalefix) {
		ರಚನೆ rev_info revs;

		repo_init_revisions(the_repository, &revs, ಪೂರ್ವಪ್ರತ್ಯಯ);
		revs.do_not_die_on_missing_objects = 1;
		revs.ignore_missing = 1;
		revs.ignore_missing_links = 1;
		ಆದರೆ (verbose)
			ಬರೆ(_("Marking reachable objects..."));
		mark_reachable_objects(&revs, 0, 0, ಖಾಲಿ);
		release_revisions(&revs);
		ಆದರೆ (verbose)
			ಅಕ್ಷರ_ಬರೆ('\n');
	}

	ಆದರೆ (do_all) {
		ರಚನೆ worktree_reflogs collected = {
			.reflogs = STRING_LIST_INIT_DUP,
		};
		ರಚನೆ string_list_ಅಂಶ *ಅಂಶ;
		ರಚನೆ worktree **worktrees, **p;

		worktrees = get_worktrees(the_repository);
		ಗಾಗಿ (p = worktrees; *p; p++) {
			ಆದರೆ (single_worktree && !(*p)->is_current)
				ಮುಂದುವರಿ;
			collected.worktree = *p;
			refs_for_each_reflog(get_worktree_ref_store(*p),
					     collect_reflog, &collected);
		}
		free_worktrees(worktrees);

		for_each_string_list_ಅಂಶ(ಅಂಶ, &collected.reflogs) {
			ರಚನೆ expire_reflog_policy_cb cb = {
				.opts = opts,
				.dry_ಸತತ = !!(flags & EXPIRE_REFLOGS_DRY_ಸತತ),
			};

			reflog_expire_options_ಗಣ_refname(&cb.opts,  ಅಂಶ->string);
			ಸ್ಥಿತಿ_2 |= refs_reflog_expire(get_main_ref_store(the_repository),
						     ಅಂಶ->string, flags,
						     reflog_expiry_prepare,
						     should_prune_fn,
						     reflog_expiry_cleanup,
						     &cb);
		}
		string_list_clear(&collected.reflogs, 0);
	}

	ಗಾಗಿ (i = 0; i < argc; i++) {
		ಅಕ್ಷರ *ref;
		ರಚನೆ expire_reflog_policy_cb cb = { .opts = opts };

		ಆದರೆ (!repo_dwim_log(the_repository, argv[i], ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(argv[i]), ಖಾಲಿ, &ref)) {
			ಸ್ಥಿತಿ_2 |= error(_("reflog could not be found: '%s'"), argv[i]);
			ಮುಂದುವರಿ;
		}
		reflog_expire_options_ಗಣ_refname(&cb.opts, ref);
		ಸ್ಥಿತಿ_2 |= refs_reflog_expire(get_main_ref_store(the_repository),
					     ref, flags,
					     reflog_expiry_prepare,
					     should_prune_fn,
					     reflog_expiry_cleanup,
					     &cb);
		ಬಿಡುಗಡೆ(ref);
	}

	reflog_clear_expire_config(&opts);

	ಹಿಂತಿರುಗಿ ಸ್ಥಿತಿ_2;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cmd_reflog_delete(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			     ರಚನೆ repository *repo ಬಳಸದ)
{
	ಪೂರ್ಣಾಂಕ i, ಸ್ಥಿತಿ_2 = 0;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags = 0;
	ಪೂರ್ಣಾಂಕ verbose = 0;

	ಸ್ಥಿರಾಂಕ ರಚನೆ option options[] = {
		OPT_ಬಿಟ್('n', "dry-run", &flags, N_("do not actually prune any entries"),
			EXPIRE_REFLOGS_DRY_ಸತತ),
		OPT_ಬಿಟ್(0, "rewrite", &flags,
			N_("rewrite the old SHA1 with the new SHA1 of the entry that now precedes it"),
			EXPIRE_REFLOGS_REWRITE),
		OPT_ಬಿಟ್(0, "updateref", &flags,
			N_("update the reference to the value of the top reflog entry"),
			EXPIRE_REFLOGS_UPDATE_REF),
		OPT_BOOL(0, "verbose", &verbose, N_("print extra information on screen")),
		OPT_ಕೊನೆ()
	};

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options, reflog_delete_usage, 0);

	ಆದರೆ (argc < 1)
		ಹಿಂತಿರುಗಿ error(_("no reflog specified to delete"));

	ಗಾಗಿ (i = 0; i < argc; i++)
		ಸ್ಥಿತಿ_2 |= reflog_delete(argv[i], flags, verbose);

	ಹಿಂತಿರುಗಿ ಸ್ಥಿತಿ_2;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cmd_reflog_exists(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			     ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ option options[] = {
		OPT_ಕೊನೆ()
	};
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *refname;

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options, reflog_exists_usage,
			     0);
	ಆದರೆ (!argc)
		usage_with_options(reflog_exists_usage, options);

	refname = argv[0];
	ಆದರೆ (check_refname_format(refname, REFNAME_ALLOW_ONELEVEL))
		die(_("invalid ref format: %s"), refname);
	ಹಿಂತಿರುಗಿ !refs_reflog_exists(get_main_ref_store(the_repository),
				   refname);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cmd_reflog_drop(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			   ರಚನೆ repository *repo)
{
	ಪೂರ್ಣಾಂಕ ret = 0, do_all = 0, single_worktree = 0;
	ಸ್ಥಿರಾಂಕ ರಚನೆ option options[] = {
		OPT_BOOL(0, "all", &do_all, N_("drop the reflogs of all references")),
		OPT_BOOL(0, "single-worktree", &single_worktree,
			 N_("drop reflogs from the current worktree only")),
		OPT_ಕೊನೆ()
	};

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options, reflog_drop_usage, 0);

	ಆದರೆ (argc && do_all)
		usage(_("references specified along with --all"));

	ಆದರೆ (do_all) {
		ರಚನೆ worktree_reflogs collected = {
			.reflogs = STRING_LIST_INIT_DUP,
		};
		ರಚನೆ string_list_ಅಂಶ *ಅಂಶ;
		ರಚನೆ worktree **worktrees, **p;

		worktrees = get_worktrees(the_repository);
		ಗಾಗಿ (p = worktrees; *p; p++) {
			ಆದರೆ (single_worktree && !(*p)->is_current)
				ಮುಂದುವರಿ;
			collected.worktree = *p;
			refs_for_each_reflog(get_worktree_ref_store(*p),
					     collect_reflog, &collected);
		}
		free_worktrees(worktrees);

		for_each_string_list_ಅಂಶ(ಅಂಶ, &collected.reflogs)
			ret |= refs_delete_reflog(get_main_ref_store(repo),
						     ಅಂಶ->string);
		string_list_clear(&collected.reflogs, 0);

		ಹಿಂತಿರುಗಿ ret;
	}

	ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < argc; i++) {
		ಅಕ್ಷರ *ref;
		ಆದರೆ (!repo_dwim_log(repo, argv[i], ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(argv[i]), ಖಾಲಿ, &ref)) {
			ret |= error(_("reflog could not be found: '%s'"), argv[i]);
			ಮುಂದುವರಿ;
		}

		ret |= refs_delete_reflog(get_main_ref_store(repo), ref);
		ಬಿಡುಗಡೆ(ref);
	}

	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cmd_reflog_write(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			    ರಚನೆ repository *repo)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ option options[] = {
		OPT_ಕೊನೆ()
	};
	ರಚನೆ object_id old_oid, new_oid;
	ರಚನೆ strbuf err = STRBUF_INIT;
	ರಚನೆ ref_transaction *tx;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ref, *ಸಂದೇಶ_2;
	ಪೂರ್ಣಾಂಕ ret;

	repo_config(repo, git_ident_config, ಖಾಲಿ);

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options, reflog_write_usage, 0);
	ಆದರೆ (argc != 4)
		usage_with_options(reflog_write_usage, options);

	ref = argv[0];
	ಆದರೆ (!is_ಬೇರು_ref(ref) && check_refname_format(ref, 0))
		die(_("invalid reference name: %s"), ref);

	ret = get_oid_hex_algop(argv[1], &old_oid, repo->hash_algo);
	ಆದರೆ (ret)
		die(_("invalid old object ID: '%s'"), argv[1]);
	ಆದರೆ (!is_null_oid(&old_oid) && !odb_has_object(repo->objects, &old_oid, 0))
		die(_("old object '%s' does not exist"), argv[1]);

	ret = get_oid_hex_algop(argv[2], &new_oid, repo->hash_algo);
	ಆದರೆ (ret)
		die(_("invalid new object ID: '%s'"), argv[2]);
	ಆದರೆ (!is_null_oid(&new_oid) && !odb_has_object(repo->objects, &new_oid, 0))
		die(_("new object '%s' does not exist"), argv[2]);

	ಸಂದೇಶ_2 = argv[3];

	tx = ref_store_transaction_begin(get_main_ref_store(repo), 0, &err);
	ಆದರೆ (!tx)
		die(_("cannot start transaction: %s"), err.buf);

	ret = ref_transaction_update_reflog(tx, ref, &new_oid, &old_oid,
					    git_committer_info(0),
					    ಸಂದೇಶ_2, 0, &err);
	ಆದರೆ (ret)
		die(_("cannot queue reflog update: %s"), err.buf);

	ret = ref_transaction_commit(tx, &err);
	ಆದರೆ (ret)
		die(_("cannot commit reflog update: %s"), err.buf);

	ref_transaction_free(tx);
	strbuf_release(&err);
	ಹಿಂತಿರುಗಿ 0;
}

/*
 * main "reflog"
 */
ಪೂರ್ಣಾಂಕ cmd_reflog(ಪೂರ್ಣಾಂಕ argc,
	       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
	       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
	       ರಚನೆ repository *repository)
{
	parse_opt_subcommand_fn *fn = ಖಾಲಿ;
	ರಚನೆ option options[] = {
		OPT_SUBCOMMAND("show", &fn, cmd_reflog_show),
		OPT_SUBCOMMAND("list", &fn, cmd_reflog_list),
		OPT_SUBCOMMAND("exists", &fn, cmd_reflog_exists),
		OPT_SUBCOMMAND("write", &fn, cmd_reflog_write),
		OPT_SUBCOMMAND("delete", &fn, cmd_reflog_delete),
		OPT_SUBCOMMAND("drop", &fn, cmd_reflog_drop),
		OPT_SUBCOMMAND("expire", &fn, cmd_reflog_expire),
		OPT_ಕೊನೆ()
	};

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options, reflog_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL |
			     PARSE_OPT_KEEP_DASHDASH | PARSE_OPT_KEEP_ARGV0 |
			     PARSE_OPT_KEEP_UNKNOWN_OPT);
	ಆದರೆ (fn)
		ಹಿಂತಿರುಗಿ fn(argc - 1, argv + 1, ಪೂರ್ವಪ್ರತ್ಯಯ, repository);
	ಇಲ್ಲದಿದ್ದರೆ
		ಹಿಂತಿರುಗಿ cmd_log_reflog(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, repository);
}
