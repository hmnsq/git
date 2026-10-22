/*
 * "git replay" builtin command
 */

#अन्तर्भाव "git-compat-util.h"

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "object-name.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "refs.h"
#अन्तर्भाव "replay.h"
#अन्तर्भाव "revision.h"

गणना ref_action_बहुलकम् {
	REF_ACTION_UPDATE,
	REF_ACTION_PRINT,
};

स्थिर गणना ref_action_बहुलकम् parse_ref_action_बहुलकम्(ध्रुव वर्ण *ref_action, ध्रुव वर्ण *source)
{
	यदि (!ref_action || !वाक्यं_तुलय(ref_action, "update"))
		प्रत्यागम REF_ACTION_UPDATE;
	यदि (!वाक्यं_तुलय(ref_action, "print"))
		प्रत्यागम REF_ACTION_PRINT;
	die(_("invalid %s value: '%s'"), source, ref_action);
}

स्थिर गणना ref_action_बहुलकम् get_ref_action_बहुलकम्(संरचना repository *repo, ध्रुव वर्ण *ref_action)
{
	ध्रुव वर्ण *config_मानम् = रिक्त;

	/* Command line option takes precedence */
	यदि (ref_action)
		प्रत्यागम parse_ref_action_बहुलकम्(ref_action, "--ref-action");

	/* Check config value */
	यदि (!repo_config_get_string_tmp(repo, "replay.refAction", &config_मानम्))
		प्रत्यागम parse_ref_action_बहुलकम्(config_मानम्, "replay.refAction");

	/* Default to update mode */
	प्रत्यागम REF_ACTION_UPDATE;
}

स्थिर पूर्णाङ्क handle_ref_update(गणना ref_action_बहुलकम् बहुलकम्,
			     संरचना ref_transaction *transaction,
			     ध्रुव वर्ण *refname,
			     ध्रुव संरचना object_id *new_oid,
			     ध्रुव संरचना object_id *old_oid,
			     ध्रुव वर्ण *reflog_सन्देशः,
			     संरचना strbuf *err)
{
	विकल्प (बहुलकम्) {
	पक्ष REF_ACTION_PRINT:
		प्रारूप_लिख("update %s %s %s\n",
		       refname,
		       oid_गन्तव्यम्_hex(new_oid),
		       oid_गन्तव्यम्_hex(old_oid));
		प्रत्यागम 0;
	पक्ष REF_ACTION_UPDATE:
		प्रत्यागम ref_transaction_update(transaction, refname, new_oid, old_oid,
					      रिक्त, रिक्त, 0, reflog_सन्देशः, err);
	स्वभाव:
		BUG("unknown ref_action_mode %d", बहुलकम्);
	}
}

पूर्णाङ्क cmd_replay(पूर्णाङ्क argc,
	       ध्रुव वर्ण **argv,
	       ध्रुव वर्ण *उपसर्गः,
	       संरचना repository *repo)
{
	संरचना replay_revisions_options opts = { 0 };
	संरचना replay_परिणाम परिणाम = { 0 };
	ध्रुव वर्ण *ref_action = रिक्त;
	गणना ref_action_बहुलकम् ref_बहुलकम्;
	संरचना rev_info revs;
	संरचना ref_transaction *transaction = रिक्त;
	संरचना strbuf transaction_err = STRBUF_INIT;
	संरचना strbuf reflog_सन्देशः = STRBUF_INIT;
	पूर्णाङ्क desired_reverse;
	पूर्णाङ्क ret = 0;

	ध्रुव वर्ण *ध्रुव replay_usage[] = {
		N_("(EXPERIMENTAL!) git replay "
		   "([--contained] --onto=<newbase> | --advance=<branch> | --revert=<branch>)\n"
		   "[--ref=<ref>] [--ref-action=<mode>] [--linearize] <revision-range>"),
		रिक्त
	};
	संरचना option replay_options[] = {
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
		OPT_अन्तः()
	};

	argc = parse_options(argc, argv, उपसर्गः, replay_options, replay_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT);

	/* Exactly one mode must be specified */
	यदि (!opts.onto && !opts.advance && !opts.revert) {
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
	ref_बहुलकम् = get_ref_action_बहुलकम्(repo, ref_action);

	/*
	 * Cherry-pick/rebase need oldest-first ordering so that each
	 * replayed commit can build on its already-replayed parent.
	 * Revert needs newest-first ordering (like git revert) to
	 * reduce conflicts by peeling off changes from the top.
	 */
	desired_reverse = !opts.revert;

	repo_init_revisions(repo, &revs, उपसर्गः);

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
	revs.क्रमय_क्रमः = REV_क्रमय_अन्तर्_GRAPH_क्रमः;
	revs.topo_क्रमः = 1;
	revs.simplify_history = 0;

	argc = सज्जता_revisions(argc, argv, &revs, रिक्त);
	यदि (argc > 1) {
		ret = error(_("unrecognized argument: %s"), argv[1]);
		गच्छ cleanup;
	}

	/*
	 * Detect and warn if we override some user specified rev
	 * walking options.
	 */
	यदि (revs.reverse != desired_reverse) {
		warning(_("some rev walking options will be overridden as "
			  "'%s' bit in 'struct rev_info' will be forced"),
			"reverse");
		revs.reverse = desired_reverse;
	}
	यदि (revs.क्रमय_क्रमः != REV_क्रमय_अन्तर्_GRAPH_क्रमः) {
		warning(_("some rev walking options will be overridden as "
			  "'%s' bit in 'struct rev_info' will be forced"),
			"sort_order");
		revs.क्रमय_क्रमः = REV_क्रमय_अन्तर्_GRAPH_क्रमः;
	}
	यदि (revs.topo_क्रमः != 1) {
		warning(_("some rev walking options will be overridden as "
			  "'%s' bit in 'struct rev_info' will be forced"),
			"topo_order");
		revs.topo_क्रमः = 1;
	}
	यदि (revs.simplify_history != 0) {
		warning(_("some rev walking options will be overridden as "
			  "'%s' bit in 'struct rev_info' will be forced"),
			"simplify_history");
		revs.simplify_history = 0;
	}

	ret = replay_revisions(&revs, &opts, &परिणाम);
	यदि (ret)
		गच्छ cleanup;

	/* Build reflog message */
	यदि (opts.revert) {
		strbuf_addf(&reflog_सन्देशः, "replay --revert %s", opts.revert);
	} अन्यथा यदि (opts.advance) {
		strbuf_addf(&reflog_सन्देशः, "replay --advance %s", opts.advance);
	} अन्यथा {
		संरचना object_id oid;
		यदि (repo_get_oid_committish(repo, opts.onto, &oid))
			BUG("--onto commit should have been resolved beforehand already");
		strbuf_addf(&reflog_सन्देशः, "replay --onto %s", oid_गन्तव्यम्_hex(&oid));
	}

	/* Initialize ref transaction if using update mode */
	यदि (ref_बहुलकम् == REF_ACTION_UPDATE) {
		transaction = ref_store_transaction_begin(get_main_ref_store(repo),
							  0, &transaction_err);
		यदि (!transaction) {
			ret = error(_("failed to begin ref transaction: %s"),
				    transaction_err.buf);
			गच्छ cleanup;
		}
	}

	प्रति (आकार_t i = 0; i < परिणाम.updates_nr; i++) {
		ret = handle_ref_update(ref_बहुलकम्, transaction, परिणाम.updates[i].refname,
					&परिणाम.updates[i].new_oid, &परिणाम.updates[i].old_oid,
					reflog_सन्देशः.buf, &transaction_err);
		यदि (ret) {
			ret = error(_("failed to update ref '%s': %s"),
				    परिणाम.updates[i].refname, transaction_err.buf);
			गच्छ cleanup;
		}
	}

	/* Commit the ref transaction if we have one */
	यदि (transaction) {
		यदि (ref_transaction_commit(transaction, &transaction_err)) {
			ret = error(_("failed to commit ref transaction: %s"),
				    transaction_err.buf);
			गच्छ cleanup;
		}
	}

	ret = 0;

cleanup:
	यदि (transaction)
		ref_transaction_free(transaction);
	replay_परिणाम_release(&परिणाम);
	strbuf_release(&transaction_err);
	strbuf_release(&reflog_सन्देशः);
	release_revisions(&revs);

	/* Return */
	यदि (ret < 0)
		निर्गच्छ(128);
	प्रत्यागम ret;
}
