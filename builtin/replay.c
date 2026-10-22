/*
 * "git replay" builtin command
 */

#ضمن "git-compat-util.h"

#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "hex.h"
#ضمن "object-name.h"
#ضمن "parse-options.h"
#ضمن "refs.h"
#ضمن "replay.h"
#ضمن "revision.h"

تعداد ref_action_منوال {
	REF_ACTION_UPDATE,
	REF_ACTION_PRINT,
};

ساكن تعداد ref_action_منوال parse_ref_action_منوال(ثابت حرف *ref_action, ثابت حرف *سروت)
{
	إذا (!ref_action || !strcmp(ref_action, "update"))
		أرجع REF_ACTION_UPDATE;
	إذا (!strcmp(ref_action, "print"))
		أرجع REF_ACTION_PRINT;
	die(_("invalid %s value: '%s'"), سروت, ref_action);
}

ساكن تعداد ref_action_منوال get_ref_action_منوال(بنية repository *repo, ثابت حرف *ref_action)
{
	ثابت حرف *config_قدر = عدم;

	/* Command line option takes precedence */
	إذا (ref_action)
		أرجع parse_ref_action_منوال(ref_action, "--ref-action");

	/* Check config value */
	إذا (!repo_config_get_string_tmp(repo, "replay.refAction", &config_قدر))
		أرجع parse_ref_action_منوال(config_قدر, "replay.refAction");

	/* Default to update mode */
	أرجع REF_ACTION_UPDATE;
}

ساكن صحيح handle_ref_update(تعداد ref_action_منوال منوال,
			     بنية ref_transaction *transaction,
			     ثابت حرف *refname,
			     ثابت بنية object_id *new_oid,
			     ثابت بنية object_id *old_oid,
			     ثابت حرف *reflog_پیغام,
			     بنية strbuf *err)
{
	بدل (منوال) {
	حالة REF_ACTION_PRINT:
		لکھو("update %s %s %s\n",
		       refname,
		       oid_منزل_hex(new_oid),
		       oid_منزل_hex(old_oid));
		أرجع 0;
	حالة REF_ACTION_UPDATE:
		أرجع ref_transaction_update(transaction, refname, new_oid, old_oid,
					      عدم, عدم, 0, reflog_پیغام, err);
	افتراضي:
		BUG("unknown ref_action_mode %d", منوال);
	}
}

صحيح cmd_replay(صحيح argc,
	       ثابت حرف **argv,
	       ثابت حرف *اگیتر,
	       بنية repository *repo)
{
	بنية replay_revisions_options opts = { 0 };
	بنية replay_نتیجہ نتیجہ = { 0 };
	ثابت حرف *ref_action = عدم;
	تعداد ref_action_منوال ref_منوال;
	بنية rev_info revs;
	بنية ref_transaction *transaction = عدم;
	بنية strbuf transaction_err = STRBUF_INIT;
	بنية strbuf reflog_پیغام = STRBUF_INIT;
	صحيح desired_الٹ;
	صحيح ret = 0;

	ثابت حرف *ثابت replay_usage[] = {
		N_("(EXPERIMENTAL!) git replay "
		   "([--contained] --onto=<newbase> | --advance=<branch> | --revert=<branch>)\n"
		   "[--ref=<ref>] [--ref-action=<mode>] [--linearize] <revision-range>"),
		عدم
	};
	بنية option replay_options[] = {
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
		OPT_سرا()
	};

	argc = parse_options(argc, argv, اگیتر, replay_options, replay_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT);

	/* Exactly one mode must be specified */
	إذا (!opts.onto && !opts.advance && !opts.revert) {
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

	repo_init_revisions(repo, &revs, اگیتر);

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
	revs.ترتیب_دیو_ترتیب = REV_ترتیب_دیو_وچ_گراف_ترتیب;
	revs.topo_ترتیب = 1;
	revs.simplify_history = 0;

	argc = تیاری_revisions(argc, argv, &revs, عدم);
	إذا (argc > 1) {
		ret = error(_("unrecognized argument: %s"), argv[1]);
		اذهب cleanup;
	}

	/*
	 * Detect and warn if we override some user specified rev
	 * walking options.
	 */
	إذا (revs.الٹ != desired_الٹ) {
		warning(_("some rev walking options will be overridden as "
			  "'%s' bit in 'struct rev_info' will be forced"),
			"reverse");
		revs.الٹ = desired_الٹ;
	}
	إذا (revs.ترتیب_دیو_ترتیب != REV_ترتیب_دیو_وچ_گراف_ترتیب) {
		warning(_("some rev walking options will be overridden as "
			  "'%s' bit in 'struct rev_info' will be forced"),
			"sort_order");
		revs.ترتیب_دیو_ترتیب = REV_ترتیب_دیو_وچ_گراف_ترتیب;
	}
	إذا (revs.topo_ترتیب != 1) {
		warning(_("some rev walking options will be overridden as "
			  "'%s' bit in 'struct rev_info' will be forced"),
			"topo_order");
		revs.topo_ترتیب = 1;
	}
	إذا (revs.simplify_history != 0) {
		warning(_("some rev walking options will be overridden as "
			  "'%s' bit in 'struct rev_info' will be forced"),
			"simplify_history");
		revs.simplify_history = 0;
	}

	ret = replay_revisions(&revs, &opts, &نتیجہ);
	إذا (ret)
		اذهب cleanup;

	/* Build reflog message */
	إذا (opts.revert) {
		strbuf_addf(&reflog_پیغام, "replay --revert %s", opts.revert);
	} وإلا إذا (opts.advance) {
		strbuf_addf(&reflog_پیغام, "replay --advance %s", opts.advance);
	} وإلا {
		بنية object_id oid;
		إذا (repo_get_oid_committish(repo, opts.onto, &oid))
			BUG("--onto commit should have been resolved beforehand already");
		strbuf_addf(&reflog_پیغام, "replay --onto %s", oid_منزل_hex(&oid));
	}

	/* Initialize ref transaction if using update mode */
	إذا (ref_منوال == REF_ACTION_UPDATE) {
		transaction = ref_store_transaction_begin(get_main_ref_store(repo),
							  0, &transaction_err);
		إذا (!transaction) {
			ret = error(_("failed to begin ref transaction: %s"),
				    transaction_err.buf);
			اذهب cleanup;
		}
	}

	لكل (ناپ_t i = 0; i < نتیجہ.updates_nr; i++) {
		ret = handle_ref_update(ref_منوال, transaction, نتیجہ.updates[i].refname,
					&نتیجہ.updates[i].new_oid, &نتیجہ.updates[i].old_oid,
					reflog_پیغام.buf, &transaction_err);
		إذا (ret) {
			ret = error(_("failed to update ref '%s': %s"),
				    نتیجہ.updates[i].refname, transaction_err.buf);
			اذهب cleanup;
		}
	}

	/* Commit the ref transaction if we have one */
	إذا (transaction) {
		إذا (ref_transaction_commit(transaction, &transaction_err)) {
			ret = error(_("failed to commit ref transaction: %s"),
				    transaction_err.buf);
			اذهب cleanup;
		}
	}

	ret = 0;

cleanup:
	إذا (transaction)
		ref_transaction_free(transaction);
	replay_نتیجہ_release(&نتیجہ);
	strbuf_release(&transaction_err);
	strbuf_release(&reflog_پیغام);
	release_revisions(&revs);

	/* Return */
	إذا (ret < 0)
		باہر_نکلو(128);
	أرجع ret;
}
