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

गणना ref_action_mode {
	REF_ACTION_UPDATE,
	REF_ACTION_PRINT,
};

स्थिर गणना ref_action_mode parse_ref_action_mode(स्थिरांक अक्षर *ref_action, स्थिरांक अक्षर *source)
{
	जर (!ref_action || !म_भेद(ref_action, "update"))
		परत REF_ACTION_UPDATE;
	जर (!म_भेद(ref_action, "print"))
		परत REF_ACTION_PRINT;
	die(_("invalid %s value: '%s'"), source, ref_action);
}

स्थिर गणना ref_action_mode get_ref_action_mode(संरचना repository *repo, स्थिरांक अक्षर *ref_action)
{
	स्थिरांक अक्षर *config_मूल्य = रिक्त;

	/* Command line option takes precedence */
	जर (ref_action)
		परत parse_ref_action_mode(ref_action, "--ref-action");

	/* Check config value */
	जर (!repo_config_get_string_tmp(repo, "replay.refAction", &config_मूल्य))
		परत parse_ref_action_mode(config_मूल्य, "replay.refAction");

	/* Default to update mode */
	परत REF_ACTION_UPDATE;
}

स्थिर पूर्णांक handle_ref_update(गणना ref_action_mode mode,
			     संरचना ref_transaction *transaction,
			     स्थिरांक अक्षर *refname,
			     स्थिरांक संरचना object_id *new_oid,
			     स्थिरांक संरचना object_id *old_oid,
			     स्थिरांक अक्षर *reflog_संदेश,
			     संरचना strbuf *err)
{
	निवड (mode) {
	स्थिती REF_ACTION_PRINT:
		म_लिखो("update %s %s %s\n",
		       refname,
		       oid_to_hex(new_oid),
		       oid_to_hex(old_oid));
		परत 0;
	स्थिती REF_ACTION_UPDATE:
		परत ref_transaction_update(transaction, refname, new_oid, old_oid,
					      रिक्त, रिक्त, 0, reflog_संदेश, err);
	पूर्वनिर्धारित:
		BUG("unknown ref_action_mode %d", mode);
	}
}

पूर्णांक cmd_replay(पूर्णांक argc,
	       स्थिरांक अक्षर **argv,
	       स्थिरांक अक्षर *prefix,
	       संरचना repository *repo)
{
	संरचना replay_revisions_options opts = { 0 };
	संरचना replay_result result = { 0 };
	स्थिरांक अक्षर *ref_action = रिक्त;
	गणना ref_action_mode ref_mode;
	संरचना rev_info revs;
	संरचना ref_transaction *transaction = रिक्त;
	संरचना strbuf transaction_err = STRBUF_INIT;
	संरचना strbuf reflog_संदेश = STRBUF_INIT;
	पूर्णांक desired_reverse;
	पूर्णांक ret = 0;

	स्थिरांक अक्षर *स्थिरांक replay_usage[] = {
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
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, replay_options, replay_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT);

	/* Exactly one mode must be specified */
	जर (!opts.onto && !opts.advance && !opts.revert) {
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

	argc = setup_revisions(argc, argv, &revs, रिक्त);
	जर (argc > 1) {
		ret = error(_("unrecognized argument: %s"), argv[1]);
		जा cleanup;
	}

	/*
	 * Detect and warn if we override some user specified rev
	 * walking options.
	 */
	जर (revs.reverse != desired_reverse) {
		warning(_("some rev walking options will be overridden as "
			  "'%s' bit in 'struct rev_info' will be forced"),
			"reverse");
		revs.reverse = desired_reverse;
	}
	जर (revs.sort_order != REV_SORT_IN_GRAPH_ORDER) {
		warning(_("some rev walking options will be overridden as "
			  "'%s' bit in 'struct rev_info' will be forced"),
			"sort_order");
		revs.sort_order = REV_SORT_IN_GRAPH_ORDER;
	}
	जर (revs.topo_order != 1) {
		warning(_("some rev walking options will be overridden as "
			  "'%s' bit in 'struct rev_info' will be forced"),
			"topo_order");
		revs.topo_order = 1;
	}
	जर (revs.simplify_history != 0) {
		warning(_("some rev walking options will be overridden as "
			  "'%s' bit in 'struct rev_info' will be forced"),
			"simplify_history");
		revs.simplify_history = 0;
	}

	ret = replay_revisions(&revs, &opts, &result);
	जर (ret)
		जा cleanup;

	/* Build reflog message */
	जर (opts.revert) {
		strbuf_addf(&reflog_संदेश, "replay --revert %s", opts.revert);
	} अन्यथा जर (opts.advance) {
		strbuf_addf(&reflog_संदेश, "replay --advance %s", opts.advance);
	} अन्यथा {
		संरचना object_id oid;
		जर (repo_get_oid_committish(repo, opts.onto, &oid))
			BUG("--onto commit should have been resolved beforehand already");
		strbuf_addf(&reflog_संदेश, "replay --onto %s", oid_to_hex(&oid));
	}

	/* Initialize ref transaction if using update mode */
	जर (ref_mode == REF_ACTION_UPDATE) {
		transaction = ref_store_transaction_begin(get_main_ref_store(repo),
							  0, &transaction_err);
		जर (!transaction) {
			ret = error(_("failed to begin ref transaction: %s"),
				    transaction_err.buf);
			जा cleanup;
		}
	}

	साठी (माप_प्रकार i = 0; i < result.updates_nr; i++) {
		ret = handle_ref_update(ref_mode, transaction, result.updates[i].refname,
					&result.updates[i].new_oid, &result.updates[i].old_oid,
					reflog_संदेश.buf, &transaction_err);
		जर (ret) {
			ret = error(_("failed to update ref '%s': %s"),
				    result.updates[i].refname, transaction_err.buf);
			जा cleanup;
		}
	}

	/* Commit the ref transaction if we have one */
	जर (transaction) {
		जर (ref_transaction_commit(transaction, &transaction_err)) {
			ret = error(_("failed to commit ref transaction: %s"),
				    transaction_err.buf);
			जा cleanup;
		}
	}

	ret = 0;

cleanup:
	जर (transaction)
		ref_transaction_free(transaction);
	replay_result_release(&result);
	strbuf_release(&transaction_err);
	strbuf_release(&reflog_संदेश);
	release_revisions(&revs);

	/* Return */
	जर (ret < 0)
		निकास(128);
	परत ret;
}
