/*
 * "git replay" builtin command
 */

#అన్తర్భావ "git-compat-util.h"

#అన్తర్భావ "builtin.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "hex.h"
#అన్తర్భావ "object-name.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "refs.h"
#అన్తర్భావ "replay.h"
#అన్తర్భావ "revision.h"

గణన ref_action_బహుళకం {
	REF_ACTION_UPDATE,
	REF_ACTION_PRINT,
};

స్థిర గణన ref_action_బహుళకం parse_ref_action_బహుళకం(స్థిరరాశి అక్షరం *ref_action, స్థిరరాశి అక్షరం *ఆధారం)
{
	ఒకవేళ (!ref_action || !strcmp(ref_action, "update"))
		తిరిగి REF_ACTION_UPDATE;
	ఒకవేళ (!strcmp(ref_action, "print"))
		తిరిగి REF_ACTION_PRINT;
	die(_("invalid %s value: '%s'"), ఆధారం, ref_action);
}

స్థిర గణన ref_action_బహుళకం get_ref_action_బహుళకం(నిర్మాణం repository *repo, స్థిరరాశి అక్షరం *ref_action)
{
	స్థిరరాశి అక్షరం *config_విలువ = ఖాళీ;

	/* Command line option takes precedence */
	ఒకవేళ (ref_action)
		తిరిగి parse_ref_action_బహుళకం(ref_action, "--ref-action");

	/* Check config value */
	ఒకవేళ (!repo_config_get_string_tmp(repo, "replay.refAction", &config_విలువ))
		తిరిగి parse_ref_action_బహుళకం(config_విలువ, "replay.refAction");

	/* Default to update mode */
	తిరిగి REF_ACTION_UPDATE;
}

స్థిర పూర్ణాంకం handle_ref_update(గణన ref_action_బహుళకం బహుళకం,
			     నిర్మాణం ref_transaction *transaction,
			     స్థిరరాశి అక్షరం *refname,
			     స్థిరరాశి నిర్మాణం object_id *new_oid,
			     స్థిరరాశి నిర్మాణం object_id *old_oid,
			     స్థిరరాశి అక్షరం *reflog_సందేశం,
			     నిర్మాణం strbuf *err)
{
	ఎంపిక (బహుళకం) {
	స్థితి REF_ACTION_PRINT:
		రాయి("update %s %s %s\n",
		       refname,
		       oid_గమ్యం_hex(new_oid),
		       oid_గమ్యం_hex(old_oid));
		తిరిగి 0;
	స్థితి REF_ACTION_UPDATE:
		తిరిగి ref_transaction_update(transaction, refname, new_oid, old_oid,
					      ఖాళీ, ఖాళీ, 0, reflog_సందేశం, err);
	అప్రమేయ:
		BUG("unknown ref_action_mode %d", బహుళకం);
	}
}

పూర్ణాంకం cmd_replay(పూర్ణాంకం argc,
	       స్థిరరాశి అక్షరం **argv,
	       స్థిరరాశి అక్షరం *ఉపసర్గ,
	       నిర్మాణం repository *repo)
{
	నిర్మాణం replay_revisions_options opts = { 0 };
	నిర్మాణం replay_ఫలితం ఫలితం = { 0 };
	స్థిరరాశి అక్షరం *ref_action = ఖాళీ;
	గణన ref_action_బహుళకం ref_బహుళకం;
	నిర్మాణం rev_info revs;
	నిర్మాణం ref_transaction *transaction = ఖాళీ;
	నిర్మాణం strbuf transaction_err = STRBUF_INIT;
	నిర్మాణం strbuf reflog_సందేశం = STRBUF_INIT;
	పూర్ణాంకం desired_తిరగేయి;
	పూర్ణాంకం ret = 0;

	స్థిరరాశి అక్షరం *స్థిరరాశి replay_usage[] = {
		N_("(EXPERIMENTAL!) git replay "
		   "([--contained] --onto=<newbase> | --advance=<branch> | --revert=<branch>)\n"
		   "[--ref=<ref>] [--ref-action=<mode>] [--linearize] <revision-range>"),
		ఖాళీ
	};
	నిర్మాణం option replay_options[] = {
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
		OPT_చివర()
	};

	argc = parse_options(argc, argv, ఉపసర్గ, replay_options, replay_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT);

	/* Exactly one mode must be specified */
	ఒకవేళ (!opts.onto && !opts.advance && !opts.revert) {
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
	ref_బహుళకం = get_ref_action_బహుళకం(repo, ref_action);

	/*
	 * Cherry-pick/rebase need oldest-first ordering so that each
	 * replayed commit can build on its already-replayed parent.
	 * Revert needs newest-first ordering (like git revert) to
	 * reduce conflicts by peeling off changes from the top.
	 */
	desired_తిరగేయి = !opts.revert;

	repo_init_revisions(repo, &revs, ఉపసర్గ);

	/*
	 * Set desired values for rev walking options here. If they
	 * are changed by some user specified option in setup_revisions()
	 * below, we will detect that below and then warn.
	 *
	 * TODO: In the future we might want to either die(), or allow
	 * some options changing these values if we think they could
	 * be useful.
	 */
	revs.తిరగేయి = desired_తిరగేయి;
	revs.క్రమీకరించు_క్రమం = REV_క్రమీకరించు_లోపల_లేఖాచిత్రం_క్రమం;
	revs.topo_క్రమం = 1;
	revs.simplify_history = 0;

	argc = అమరిక_revisions(argc, argv, &revs, ఖాళీ);
	ఒకవేళ (argc > 1) {
		ret = error(_("unrecognized argument: %s"), argv[1]);
		వెళ్ళు cleanup;
	}

	/*
	 * Detect and warn if we override some user specified rev
	 * walking options.
	 */
	ఒకవేళ (revs.తిరగేయి != desired_తిరగేయి) {
		warning(_("some rev walking options will be overridden as "
			  "'%s' bit in 'struct rev_info' will be forced"),
			"reverse");
		revs.తిరగేయి = desired_తిరగేయి;
	}
	ఒకవేళ (revs.క్రమీకరించు_క్రమం != REV_క్రమీకరించు_లోపల_లేఖాచిత్రం_క్రమం) {
		warning(_("some rev walking options will be overridden as "
			  "'%s' bit in 'struct rev_info' will be forced"),
			"sort_order");
		revs.క్రమీకరించు_క్రమం = REV_క్రమీకరించు_లోపల_లేఖాచిత్రం_క్రమం;
	}
	ఒకవేళ (revs.topo_క్రమం != 1) {
		warning(_("some rev walking options will be overridden as "
			  "'%s' bit in 'struct rev_info' will be forced"),
			"topo_order");
		revs.topo_క్రమం = 1;
	}
	ఒకవేళ (revs.simplify_history != 0) {
		warning(_("some rev walking options will be overridden as "
			  "'%s' bit in 'struct rev_info' will be forced"),
			"simplify_history");
		revs.simplify_history = 0;
	}

	ret = replay_revisions(&revs, &opts, &ఫలితం);
	ఒకవేళ (ret)
		వెళ్ళు cleanup;

	/* Build reflog message */
	ఒకవేళ (opts.revert) {
		strbuf_addf(&reflog_సందేశం, "replay --revert %s", opts.revert);
	} లేకపోతే ఒకవేళ (opts.advance) {
		strbuf_addf(&reflog_సందేశం, "replay --advance %s", opts.advance);
	} లేకపోతే {
		నిర్మాణం object_id oid;
		ఒకవేళ (repo_get_oid_committish(repo, opts.onto, &oid))
			BUG("--onto commit should have been resolved beforehand already");
		strbuf_addf(&reflog_సందేశం, "replay --onto %s", oid_గమ్యం_hex(&oid));
	}

	/* Initialize ref transaction if using update mode */
	ఒకవేళ (ref_బహుళకం == REF_ACTION_UPDATE) {
		transaction = ref_store_transaction_begin(get_main_ref_store(repo),
							  0, &transaction_err);
		ఒకవేళ (!transaction) {
			ret = error(_("failed to begin ref transaction: %s"),
				    transaction_err.buf);
			వెళ్ళు cleanup;
		}
	}

	కొరకు (కొలత_t i = 0; i < ఫలితం.updates_nr; i++) {
		ret = handle_ref_update(ref_బహుళకం, transaction, ఫలితం.updates[i].refname,
					&ఫలితం.updates[i].new_oid, &ఫలితం.updates[i].old_oid,
					reflog_సందేశం.buf, &transaction_err);
		ఒకవేళ (ret) {
			ret = error(_("failed to update ref '%s': %s"),
				    ఫలితం.updates[i].refname, transaction_err.buf);
			వెళ్ళు cleanup;
		}
	}

	/* Commit the ref transaction if we have one */
	ఒకవేళ (transaction) {
		ఒకవేళ (ref_transaction_commit(transaction, &transaction_err)) {
			ret = error(_("failed to commit ref transaction: %s"),
				    transaction_err.buf);
			వెళ్ళు cleanup;
		}
	}

	ret = 0;

cleanup:
	ఒకవేళ (transaction)
		ref_transaction_free(transaction);
	replay_ఫలితం_release(&ఫలితం);
	strbuf_release(&transaction_err);
	strbuf_release(&reflog_సందేశం);
	release_revisions(&revs);

	/* Return */
	ఒకవేళ (ret < 0)
		నిష్క్రమించు(128);
	తిరిగి ret;
}
