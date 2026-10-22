/*
 * "git replay" builtin command
 */

#شامل "git-compat-util.h"

#شامل "builtin.h"
#شامل "config.h"
#شامل "hex.h"
#شامل "object-name.h"
#شامل "parse-options.h"
#شامل "refs.h"
#شامل "replay.h"
#شامل "revision.h"

شمارش ref_action_mode {
	REF_ACTION_UPDATE,
	REF_ACTION_PRINT,
};

ایستا شمارش ref_action_mode parse_ref_action_mode(ثابت نویسه *ref_action, ثابت نویسه *source)
{
	اگر (!ref_action || !strcmp(ref_action, "update"))
		بازگشت REF_ACTION_UPDATE;
	اگر (!strcmp(ref_action, "print"))
		بازگشت REF_ACTION_PRINT;
	die(_("invalid %s value: '%s'"), source, ref_action);
}

ایستا شمارش ref_action_mode get_ref_action_mode(ساختار repository *repo, ثابت نویسه *ref_action)
{
	ثابت نویسه *config_مقدار = پوچ;

	/* Command line option takes precedence */
	اگر (ref_action)
		بازگشت parse_ref_action_mode(ref_action, "--ref-action");

	/* Check config value */
	اگر (!repo_config_get_string_tmp(repo, "replay.refAction", &config_مقدار))
		بازگشت parse_ref_action_mode(config_مقدار, "replay.refAction");

	/* Default to update mode */
	بازگشت REF_ACTION_UPDATE;
}

ایستا صحیح handle_ref_update(شمارش ref_action_mode mode,
			     ساختار ref_transaction *transaction,
			     ثابت نویسه *refname,
			     ثابت ساختار object_id *new_oid,
			     ثابت ساختار object_id *old_oid,
			     ثابت نویسه *reflog_پیام,
			     ساختار strbuf *err)
{
	انتخاب (mode) {
	حالت REF_ACTION_PRINT:
		printf("update %s %s %s\n",
		       refname,
		       oid_to_hex(new_oid),
		       oid_to_hex(old_oid));
		بازگشت 0;
	حالت REF_ACTION_UPDATE:
		بازگشت ref_transaction_update(transaction, refname, new_oid, old_oid,
					      پوچ, پوچ, 0, reflog_پیام, err);
	پیشفرض:
		BUG("unknown ref_action_mode %d", mode);
	}
}

صحیح cmd_replay(صحیح argc,
	       ثابت نویسه **argv,
	       ثابت نویسه *prefix,
	       ساختار repository *repo)
{
	ساختار replay_revisions_options opts = { 0 };
	ساختار replay_result result = { 0 };
	ثابت نویسه *ref_action = پوچ;
	شمارش ref_action_mode ref_mode;
	ساختار rev_info revs;
	ساختار ref_transaction *transaction = پوچ;
	ساختار strbuf transaction_err = STRBUF_INIT;
	ساختار strbuf reflog_پیام = STRBUF_INIT;
	صحیح desired_reverse;
	صحیح ret = 0;

	ثابت نویسه *ثابت replay_usage[] = {
		N_("(EXPERIMENTAL!) git replay "
		   "([--contained] --onto=<newbase> | --advance=<branch> | --revert=<branch>)\n"
		   "[--ref=<ref>] [--ref-action=<mode>] [--linearize] <revision-range>"),
		پوچ
	};
	ساختار option replay_options[] = {
		OPT_BOOL(0, "contained", &opts.contained,
			 N_("update all branches that point at commits in <revision-range>")),
		OPT_STRING_F(0, "onto", &opts.onto,
			     N_("revision"),
			     N_("replay onto given commit"),
			     PARSE_OPT_NONEG),
		OPT_STRING_F(0, "advance", &opts.advance,
			     N_("branch"),
			     N_("make replay advance given branch"),
			     PARSE_OPT_NONEG),
		OPT_STRING_F(0, "revert", &opts.revert,
			     N_("branch"),
			     N_("revert commits onto given branch"),
			     PARSE_OPT_NONEG),
		OPT_STRING_F(0, "ref", &opts.ref,
			     N_("branch"),
			     N_("reference to update with result"),
			     PARSE_OPT_NONEG),
		OPT_STRING_F(0, "ref-action", &ref_action,
			     N_("mode"),
			     N_("control ref update behavior (update|print)"),
			     PARSE_OPT_NONEG),
		OPT_BOOL(0, "linearize", &opts.linearize,
			 N_("drop merge commits, replaying only non-merge commits")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, replay_options, replay_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT);

	/* Exactly one mode must be specified */
	اگر (!opts.onto && !opts.advance && !opts.revert) {
		error(_("exactly one of --onto, --advance, or --revert is required"));
		usage_with_options(replay_usage, replay_options);
	}

	die_for_incompatible_opt3(!!opts.onto, "--onto",
				  !!opts.advance, "--advance",
				  !!opts.revert, "--revert");
	die_for_incompatible_opt2(!!opts.advance, "--advance",
				  opts.contained, "--contained");
	die_for_incompatible_opt2(!!opts.revert, "--revert",
				  opts.contained, "--contained");
	die_for_incompatible_opt2(!!opts.ref, "--ref",
				  !!opts.contained, "--contained");
	die_for_incompatible_opt2(opts.linearize, "--linearize",
				  !!opts.contained, "--contained");

	/* Parse ref action mode from command line or config */
	ref_mode = get_ref_action_mode(repo, ref_action);

	/*
	 * Cherry-pick/rebase need oldest-first ordering so that each
	 * replayed commit can build on its already-replayed parent.
	 * Revert needs newest-first ordering (like git revert) to
	 * reduce conflicts by peeling off changes from the top.
	 */
	desired_reverse = !opts.revert;

	repo_init_revisions(repo, &revs, prefix);

	/*
	 * Set desired values for rev walking options here. If they
	 * are changed by some user specified option in setup_revisions()
	 * below, we will detect that below and then warn.
	 *
	 * TODO: In the future we might want to either die(), or allow
	 * some options changing these values if we think they could
	 * be useful.
	 */
	revs.reverse = desired_reverse;
	revs.sort_order = REV_SORT_IN_GRAPH_ORDER;
	revs.topo_order = 1;
	revs.simplify_history = 0;

	argc = setup_revisions(argc, argv, &revs, پوچ);
	اگر (argc > 1) {
		ret = error(_("unrecognized argument: %s"), argv[1]);
		برو cleanup;
	}

	/*
	 * Detect and warn if we override some user specified rev
	 * walking options.
	 */
	اگر (revs.reverse != desired_reverse) {
		warning(_("some rev walking options will be overridden as "
			  "'%s' bit in 'struct rev_info' will be forced"),
			"reverse");
		revs.reverse = desired_reverse;
	}
	اگر (revs.sort_order != REV_SORT_IN_GRAPH_ORDER) {
		warning(_("some rev walking options will be overridden as "
			  "'%s' bit in 'struct rev_info' will be forced"),
			"sort_order");
		revs.sort_order = REV_SORT_IN_GRAPH_ORDER;
	}
	اگر (revs.topo_order != 1) {
		warning(_("some rev walking options will be overridden as "
			  "'%s' bit in 'struct rev_info' will be forced"),
			"topo_order");
		revs.topo_order = 1;
	}
	اگر (revs.simplify_history != 0) {
		warning(_("some rev walking options will be overridden as "
			  "'%s' bit in 'struct rev_info' will be forced"),
			"simplify_history");
		revs.simplify_history = 0;
	}

	ret = replay_revisions(&revs, &opts, &result);
	اگر (ret)
		برو cleanup;

	/* Build reflog message */
	اگر (opts.revert) {
		strbuf_addf(&reflog_پیام, "replay --revert %s", opts.revert);
	} وگرنه اگر (opts.advance) {
		strbuf_addf(&reflog_پیام, "replay --advance %s", opts.advance);
	} وگرنه {
		ساختار object_id oid;
		اگر (repo_get_oid_committish(repo, opts.onto, &oid))
			BUG("--onto commit should have been resolved beforehand already");
		strbuf_addf(&reflog_پیام, "replay --onto %s", oid_to_hex(&oid));
	}

	/* Initialize ref transaction if using update mode */
	اگر (ref_mode == REF_ACTION_UPDATE) {
		transaction = ref_store_transaction_begin(get_main_ref_store(repo),
							  0, &transaction_err);
		اگر (!transaction) {
			ret = error(_("failed to begin ref transaction: %s"),
				    transaction_err.buf);
			برو cleanup;
		}
	}

	برای (size_t i = 0; i < result.updates_nr; i++) {
		ret = handle_ref_update(ref_mode, transaction, result.updates[i].refname,
					&result.updates[i].new_oid, &result.updates[i].old_oid,
					reflog_پیام.buf, &transaction_err);
		اگر (ret) {
			ret = error(_("failed to update ref '%s': %s"),
				    result.updates[i].refname, transaction_err.buf);
			برو cleanup;
		}
	}

	/* Commit the ref transaction if we have one */
	اگر (transaction) {
		اگر (ref_transaction_commit(transaction, &transaction_err)) {
			ret = error(_("failed to commit ref transaction: %s"),
				    transaction_err.buf);
			برو cleanup;
		}
	}

	ret = 0;

cleanup:
	اگر (transaction)
		ref_transaction_free(transaction);
	replay_result_release(&result);
	strbuf_release(&transaction_err);
	strbuf_release(&reflog_پیام);
	release_revisions(&revs);

	/* Return */
	اگر (ret < 0)
		exit(128);
	بازگشت ret;
}
