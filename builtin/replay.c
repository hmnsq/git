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

تعداد ref_action_گهڻائي {
	REF_ACTION_UPDATE,
	REF_ACTION_PRINT,
};

ساكن تعداد ref_action_گهڻائي parse_ref_action_گهڻائي(ثابت حرف *ref_action, ثابت حرف *ذريعو)
{
	إذا (!ref_action || !strcmp(ref_action, "update"))
		أرجع REF_ACTION_UPDATE;
	إذا (!strcmp(ref_action, "print"))
		أرجع REF_ACTION_PRINT;
	die(_("invalid %s value: '%s'"), ذريعو, ref_action);
}

ساكن تعداد ref_action_گهڻائي get_ref_action_گهڻائي(بنية repository *repo, ثابت حرف *ref_action)
{
	ثابت حرف *config_قدر = عدم;

	/* Command line option takes precedence */
	إذا (ref_action)
		أرجع parse_ref_action_گهڻائي(ref_action, "--ref-action");

	/* Check config value */
	إذا (!repo_config_get_string_tmp(repo, "replay.refAction", &config_قدر))
		أرجع parse_ref_action_گهڻائي(config_قدر, "replay.refAction");

	/* Default to update mode */
	أرجع REF_ACTION_UPDATE;
}

ساكن صحيح handle_ref_update(تعداد ref_action_گهڻائي گهڻائي,
			     بنية ref_transaction *transaction,
			     ثابت حرف *refname,
			     ثابت بنية object_id *new_oid,
			     ثابت بنية object_id *old_oid,
			     ثابت حرف *reflog_نياپو,
			     بنية strbuf *err)
{
	بدل (گهڻائي) {
	حالة REF_ACTION_PRINT:
		لکو("update %s %s %s\n",
		       refname,
		       oid_منزل_hex(new_oid),
		       oid_منزل_hex(old_oid));
		أرجع 0;
	حالة REF_ACTION_UPDATE:
		أرجع ref_transaction_update(transaction, refname, new_oid, old_oid,
					      عدم, عدم, 0, reflog_نياپو, err);
	افتراضي:
		BUG("unknown ref_action_mode %d", گهڻائي);
	}
}

صحيح cmd_replay(صحيح argc,
	       ثابت حرف **argv,
	       ثابت حرف *اڳياڙي,
	       بنية repository *repo)
{
	بنية replay_revisions_options opts = { 0 };
	بنية replay_نتيجو نتيجو = { 0 };
	ثابت حرف *ref_action = عدم;
	تعداد ref_action_گهڻائي ref_گهڻائي;
	بنية rev_info revs;
	بنية ref_transaction *transaction = عدم;
	بنية strbuf transaction_err = STRBUF_INIT;
	بنية strbuf reflog_نياپو = STRBUF_INIT;
	صحيح desired_ابتو;
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
		OPT_پڇاڙي()
	};

	argc = parse_options(argc, argv, اڳياڙي, replay_options, replay_usage,
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
	ref_گهڻائي = get_ref_action_گهڻائي(repo, ref_action);

	/*
	 * Cherry-pick/rebase need oldest-first ordering so that each
	 * replayed commit can build on its already-replayed parent.
	 * Revert needs newest-first ordering (like git revert) to
	 * reduce conflicts by peeling off changes from the top.
	 */
	desired_ابتو = !opts.revert;

	repo_init_revisions(repo, &revs, اڳياڙي);

	/*
	 * Set desired values for rev walking options here. If they
	 * are changed by some user specified option in setup_revisions()
	 * below, we will detect that below and then warn.
	 *
	 * TODO: In the future we might want to either die(), or allow
	 * some options changing these values if we think they could
	 * be useful.
	 */
	revs.ابتو = desired_ابتو;
	revs.ترتيب_ڏيو_ترتيب = REV_ترتيب_ڏيو_اندريون_گراف_ترتيب;
	revs.topo_ترتيب = 1;
	revs.simplify_history = 0;

	argc = تياري_revisions(argc, argv, &revs, عدم);
	إذا (argc > 1) {
		ret = error(_("unrecognized argument: %s"), argv[1]);
		اذهب cleanup;
	}

	/*
	 * Detect and warn if we override some user specified rev
	 * walking options.
	 */
	إذا (revs.ابتو != desired_ابتو) {
		warning(_("some rev walking options will be overridden as "
			  "'%s' bit in 'struct rev_info' will be forced"),
			"reverse");
		revs.ابتو = desired_ابتو;
	}
	إذا (revs.ترتيب_ڏيو_ترتيب != REV_ترتيب_ڏيو_اندريون_گراف_ترتيب) {
		warning(_("some rev walking options will be overridden as "
			  "'%s' bit in 'struct rev_info' will be forced"),
			"sort_order");
		revs.ترتيب_ڏيو_ترتيب = REV_ترتيب_ڏيو_اندريون_گراف_ترتيب;
	}
	إذا (revs.topo_ترتيب != 1) {
		warning(_("some rev walking options will be overridden as "
			  "'%s' bit in 'struct rev_info' will be forced"),
			"topo_order");
		revs.topo_ترتيب = 1;
	}
	إذا (revs.simplify_history != 0) {
		warning(_("some rev walking options will be overridden as "
			  "'%s' bit in 'struct rev_info' will be forced"),
			"simplify_history");
		revs.simplify_history = 0;
	}

	ret = replay_revisions(&revs, &opts, &نتيجو);
	إذا (ret)
		اذهب cleanup;

	/* Build reflog message */
	إذا (opts.revert) {
		strbuf_addf(&reflog_نياپو, "replay --revert %s", opts.revert);
	} وإلا إذا (opts.advance) {
		strbuf_addf(&reflog_نياپو, "replay --advance %s", opts.advance);
	} وإلا {
		بنية object_id oid;
		إذا (repo_get_oid_committish(repo, opts.onto, &oid))
			BUG("--onto commit should have been resolved beforehand already");
		strbuf_addf(&reflog_نياپو, "replay --onto %s", oid_منزل_hex(&oid));
	}

	/* Initialize ref transaction if using update mode */
	إذا (ref_گهڻائي == REF_ACTION_UPDATE) {
		transaction = ref_store_transaction_begin(get_main_ref_store(repo),
							  0, &transaction_err);
		إذا (!transaction) {
			ret = error(_("failed to begin ref transaction: %s"),
				    transaction_err.buf);
			اذهب cleanup;
		}
	}

	لكل (سائيز_t i = 0; i < نتيجو.updates_nr; i++) {
		ret = handle_ref_update(ref_گهڻائي, transaction, نتيجو.updates[i].refname,
					&نتيجو.updates[i].new_oid, &نتيجو.updates[i].old_oid,
					reflog_نياپو.buf, &transaction_err);
		إذا (ret) {
			ret = error(_("failed to update ref '%s': %s"),
				    نتيجو.updates[i].refname, transaction_err.buf);
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
	replay_نتيجو_release(&نتيجو);
	strbuf_release(&transaction_err);
	strbuf_release(&reflog_نياپو);
	release_revisions(&revs);

	/* Return */
	إذا (ret < 0)
		ٻاهر_نڪرو(128);
	أرجع ret;
}
