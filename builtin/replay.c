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

شمار ref_action_منوال {
	REF_ACTION_UPDATE,
	REF_ACTION_PRINT,
};

ساکن شمار ref_action_منوال parse_ref_action_منوال(ثابت حرف *ref_action, ثابت حرف *ماخذ)
{
	اگر (!ref_action || !strcmp(ref_action, "update"))
		واپس REF_ACTION_UPDATE;
	اگر (!strcmp(ref_action, "print"))
		واپس REF_ACTION_PRINT;
	die(_("invalid %s value: '%s'"), ماخذ, ref_action);
}

ساکن شمار ref_action_منوال get_ref_action_منوال(ساخت repository *repo, ثابت حرف *ref_action)
{
	ثابت حرف *config_قدر = تہی;

	/* Command line option takes precedence */
	اگر (ref_action)
		واپس parse_ref_action_منوال(ref_action, "--ref-action");

	/* Check config value */
	اگر (!repo_config_get_string_tmp(repo, "replay.refAction", &config_قدر))
		واپس parse_ref_action_منوال(config_قدر, "replay.refAction");

	/* Default to update mode */
	واپس REF_ACTION_UPDATE;
}

ساکن صحیح handle_ref_update(شمار ref_action_منوال منوال,
			     ساخت ref_transaction *transaction,
			     ثابت حرف *refname,
			     ثابت ساخت object_id *new_oid,
			     ثابت ساخت object_id *old_oid,
			     ثابت حرف *reflog_پیغام,
			     ساخت strbuf *err)
{
	انتخاب (منوال) {
	حالت REF_ACTION_PRINT:
		لکھو("update %s %s %s\n",
		       refname,
		       oid_مقصد_hex(new_oid),
		       oid_مقصد_hex(old_oid));
		واپس 0;
	حالت REF_ACTION_UPDATE:
		واپس ref_transaction_update(transaction, refname, new_oid, old_oid,
					      تہی, تہی, 0, reflog_پیغام, err);
	طےشدہ:
		BUG("unknown ref_action_mode %d", منوال);
	}
}

صحیح cmd_replay(صحیح argc,
	       ثابت حرف **argv,
	       ثابت حرف *سابقہ,
	       ساخت repository *repo)
{
	ساخت replay_revisions_options opts = { 0 };
	ساخت replay_نتیجہ نتیجہ = { 0 };
	ثابت حرف *ref_action = تہی;
	شمار ref_action_منوال ref_منوال;
	ساخت rev_info revs;
	ساخت ref_transaction *transaction = تہی;
	ساخت strbuf transaction_err = STRBUF_INIT;
	ساخت strbuf reflog_پیغام = STRBUF_INIT;
	صحیح desired_الٹ;
	صحیح ret = 0;

	ثابت حرف *ثابت replay_usage[] = {
		N_("(EXPERIMENTAL!) git replay "
		   "([--contained] --onto=<newbase> | --advance=<branch> | --revert=<branch>)\n"
		   "[--ref=<ref>] [--ref-action=<mode>] [--linearize] <revision-range>"),
		تہی
	};
	ساخت option replay_options[] = {
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
		OPT_انتہا()
	};

	argc = parse_options(argc, argv, سابقہ, replay_options, replay_usage,
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
	ref_منوال = get_ref_action_منوال(repo, ref_action);

	/*
	 * Cherry-pick/rebase need oldest-first ordering so that each
	 * replayed commit can build on its already-replayed parent.
	 * Revert needs newest-first ordering (like git revert) to
	 * reduce conflicts by peeling off changes from the top.
	 */
	desired_الٹ = !opts.revert;

	repo_init_revisions(repo, &revs, سابقہ);

	/*
	 * Set desired values for rev walking options here. If they
	 * are changed by some user specified option in setup_revisions()
	 * below, we will detect that below and then warn.
	 *
	 * TODO: In the future we might want to either die(), or allow
	 * some options changing these values if we think they could
	 * be useful.
	 */
	revs.الٹ = desired_الٹ;
	revs.ترتیب_دو_ترتیب = REV_ترتیب_دو_اندر_گراف_ترتیب;
	revs.topo_ترتیب = 1;
	revs.simplify_history = 0;

	argc = تیاری_revisions(argc, argv, &revs, تہی);
	اگر (argc > 1) {
		ret = error(_("unrecognized argument: %s"), argv[1]);
		جاؤ cleanup;
	}

	/*
	 * Detect and warn if we override some user specified rev
	 * walking options.
	 */
	اگر (revs.الٹ != desired_الٹ) {
		warning(_("some rev walking options will be overridden as "
			  "'%s' bit in 'struct rev_info' will be forced"),
			"reverse");
		revs.الٹ = desired_الٹ;
	}
	اگر (revs.ترتیب_دو_ترتیب != REV_ترتیب_دو_اندر_گراف_ترتیب) {
		warning(_("some rev walking options will be overridden as "
			  "'%s' bit in 'struct rev_info' will be forced"),
			"sort_order");
		revs.ترتیب_دو_ترتیب = REV_ترتیب_دو_اندر_گراف_ترتیب;
	}
	اگر (revs.topo_ترتیب != 1) {
		warning(_("some rev walking options will be overridden as "
			  "'%s' bit in 'struct rev_info' will be forced"),
			"topo_order");
		revs.topo_ترتیب = 1;
	}
	اگر (revs.simplify_history != 0) {
		warning(_("some rev walking options will be overridden as "
			  "'%s' bit in 'struct rev_info' will be forced"),
			"simplify_history");
		revs.simplify_history = 0;
	}

	ret = replay_revisions(&revs, &opts, &نتیجہ);
	اگر (ret)
		جاؤ cleanup;

	/* Build reflog message */
	اگر (opts.revert) {
		strbuf_addf(&reflog_پیغام, "replay --revert %s", opts.revert);
	} ورنہ اگر (opts.advance) {
		strbuf_addf(&reflog_پیغام, "replay --advance %s", opts.advance);
	} ورنہ {
		ساخت object_id oid;
		اگر (repo_get_oid_committish(repo, opts.onto, &oid))
			BUG("--onto commit should have been resolved beforehand already");
		strbuf_addf(&reflog_پیغام, "replay --onto %s", oid_مقصد_hex(&oid));
	}

	/* Initialize ref transaction if using update mode */
	اگر (ref_منوال == REF_ACTION_UPDATE) {
		transaction = ref_store_transaction_begin(get_main_ref_store(repo),
							  0, &transaction_err);
		اگر (!transaction) {
			ret = error(_("failed to begin ref transaction: %s"),
				    transaction_err.buf);
			جاؤ cleanup;
		}
	}

	برائے (سائز_t i = 0; i < نتیجہ.updates_nr; i++) {
		ret = handle_ref_update(ref_منوال, transaction, نتیجہ.updates[i].refname,
					&نتیجہ.updates[i].new_oid, &نتیجہ.updates[i].old_oid,
					reflog_پیغام.buf, &transaction_err);
		اگر (ret) {
			ret = error(_("failed to update ref '%s': %s"),
				    نتیجہ.updates[i].refname, transaction_err.buf);
			جاؤ cleanup;
		}
	}

	/* Commit the ref transaction if we have one */
	اگر (transaction) {
		اگر (ref_transaction_commit(transaction, &transaction_err)) {
			ret = error(_("failed to commit ref transaction: %s"),
				    transaction_err.buf);
			جاؤ cleanup;
		}
	}

	ret = 0;

cleanup:
	اگر (transaction)
		ref_transaction_free(transaction);
	replay_نتیجہ_release(&نتیجہ);
	strbuf_release(&transaction_err);
	strbuf_release(&reflog_پیغام);
	release_revisions(&revs);

	/* Return */
	اگر (ret < 0)
		باہر_نکلو(128);
	واپس ret;
}
