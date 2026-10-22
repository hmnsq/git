/*
 * "git replay" builtin command
 */

#அந்தர்பாவ "git-compat-util.h"

#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "hex.h"
#அந்தர்பாவ "object-name.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "refs.h"
#அந்தர்பாவ "replay.h"
#அந்தர்பாவ "revision.h"

எண்ணிக்கை ref_action_mode {
	REF_ACTION_UPDATE,
	REF_ACTION_PRINT,
};

நிலையான எண்ணிக்கை ref_action_mode parse_ref_action_mode(மாறிலி எழுத்து *ref_action, மாறிலி எழுத்து *source)
{
	எனில் (!ref_action || !strcmp(ref_action, "update"))
		திரும்பு REF_ACTION_UPDATE;
	எனில் (!strcmp(ref_action, "print"))
		திரும்பு REF_ACTION_PRINT;
	die(_("invalid %s value: '%s'"), source, ref_action);
}

நிலையான எண்ணிக்கை ref_action_mode get_ref_action_mode(கட்டமைப்பு repository *repo, மாறிலி எழுத்து *ref_action)
{
	மாறிலி எழுத்து *config_மதிப்பு = வெற்று;

	/* Command line option takes precedence */
	எனில் (ref_action)
		திரும்பு parse_ref_action_mode(ref_action, "--ref-action");

	/* Check config value */
	எனில் (!repo_config_get_string_tmp(repo, "replay.refAction", &config_மதிப்பு))
		திரும்பு parse_ref_action_mode(config_மதிப்பு, "replay.refAction");

	/* Default to update mode */
	திரும்பு REF_ACTION_UPDATE;
}

நிலையான முழு handle_ref_update(எண்ணிக்கை ref_action_mode mode,
			     கட்டமைப்பு ref_transaction *transaction,
			     மாறிலி எழுத்து *refname,
			     மாறிலி கட்டமைப்பு object_id *new_oid,
			     மாறிலி கட்டமைப்பு object_id *old_oid,
			     மாறிலி எழுத்து *reflog_செய்தி,
			     கட்டமைப்பு strbuf *err)
{
	தேர்வு (mode) {
	நிலை REF_ACTION_PRINT:
		printf("update %s %s %s\n",
		       refname,
		       oid_to_hex(new_oid),
		       oid_to_hex(old_oid));
		திரும்பு 0;
	நிலை REF_ACTION_UPDATE:
		திரும்பு ref_transaction_update(transaction, refname, new_oid, old_oid,
					      வெற்று, வெற்று, 0, reflog_செய்தி, err);
	இயல்பு:
		BUG("unknown ref_action_mode %d", mode);
	}
}

முழு cmd_replay(முழு argc,
	       மாறிலி எழுத்து **argv,
	       மாறிலி எழுத்து *prefix,
	       கட்டமைப்பு repository *repo)
{
	கட்டமைப்பு replay_revisions_options opts = { 0 };
	கட்டமைப்பு replay_result result = { 0 };
	மாறிலி எழுத்து *ref_action = வெற்று;
	எண்ணிக்கை ref_action_mode ref_mode;
	கட்டமைப்பு rev_info revs;
	கட்டமைப்பு ref_transaction *transaction = வெற்று;
	கட்டமைப்பு strbuf transaction_err = STRBUF_INIT;
	கட்டமைப்பு strbuf reflog_செய்தி = STRBUF_INIT;
	முழு desired_reverse;
	முழு ret = 0;

	மாறிலி எழுத்து *மாறிலி replay_usage[] = {
		N_("(EXPERIMENTAL!) git replay "
		   "([--contained] --onto=<newbase> | --advance=<branch> | --revert=<branch>)\n"
		   "[--ref=<ref>] [--ref-action=<mode>] [--linearize] <revision-range>"),
		வெற்று
	};
	கட்டமைப்பு option replay_options[] = {
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
	எனில் (!opts.onto && !opts.advance && !opts.revert) {
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

	argc = setup_revisions(argc, argv, &revs, வெற்று);
	எனில் (argc > 1) {
		ret = error(_("unrecognized argument: %s"), argv[1]);
		செல் cleanup;
	}

	/*
	 * Detect and warn if we override some user specified rev
	 * walking options.
	 */
	எனில் (revs.reverse != desired_reverse) {
		warning(_("some rev walking options will be overridden as "
			  "'%s' bit in 'struct rev_info' will be forced"),
			"reverse");
		revs.reverse = desired_reverse;
	}
	எனில் (revs.sort_order != REV_SORT_IN_GRAPH_ORDER) {
		warning(_("some rev walking options will be overridden as "
			  "'%s' bit in 'struct rev_info' will be forced"),
			"sort_order");
		revs.sort_order = REV_SORT_IN_GRAPH_ORDER;
	}
	எனில் (revs.topo_order != 1) {
		warning(_("some rev walking options will be overridden as "
			  "'%s' bit in 'struct rev_info' will be forced"),
			"topo_order");
		revs.topo_order = 1;
	}
	எனில் (revs.simplify_history != 0) {
		warning(_("some rev walking options will be overridden as "
			  "'%s' bit in 'struct rev_info' will be forced"),
			"simplify_history");
		revs.simplify_history = 0;
	}

	ret = replay_revisions(&revs, &opts, &result);
	எனில் (ret)
		செல் cleanup;

	/* Build reflog message */
	எனில் (opts.revert) {
		strbuf_addf(&reflog_செய்தி, "replay --revert %s", opts.revert);
	} இல்லையெனில் எனில் (opts.advance) {
		strbuf_addf(&reflog_செய்தி, "replay --advance %s", opts.advance);
	} இல்லையெனில் {
		கட்டமைப்பு object_id oid;
		எனில் (repo_get_oid_committish(repo, opts.onto, &oid))
			BUG("--onto commit should have been resolved beforehand already");
		strbuf_addf(&reflog_செய்தி, "replay --onto %s", oid_to_hex(&oid));
	}

	/* Initialize ref transaction if using update mode */
	எனில் (ref_mode == REF_ACTION_UPDATE) {
		transaction = ref_store_transaction_begin(get_main_ref_store(repo),
							  0, &transaction_err);
		எனில் (!transaction) {
			ret = error(_("failed to begin ref transaction: %s"),
				    transaction_err.buf);
			செல் cleanup;
		}
	}

	க்கு (size_t i = 0; i < result.updates_nr; i++) {
		ret = handle_ref_update(ref_mode, transaction, result.updates[i].refname,
					&result.updates[i].new_oid, &result.updates[i].old_oid,
					reflog_செய்தி.buf, &transaction_err);
		எனில் (ret) {
			ret = error(_("failed to update ref '%s': %s"),
				    result.updates[i].refname, transaction_err.buf);
			செல் cleanup;
		}
	}

	/* Commit the ref transaction if we have one */
	எனில் (transaction) {
		எனில் (ref_transaction_commit(transaction, &transaction_err)) {
			ret = error(_("failed to commit ref transaction: %s"),
				    transaction_err.buf);
			செல் cleanup;
		}
	}

	ret = 0;

cleanup:
	எனில் (transaction)
		ref_transaction_free(transaction);
	replay_result_release(&result);
	strbuf_release(&transaction_err);
	strbuf_release(&reflog_செய்தி);
	release_revisions(&revs);

	/* Return */
	எனில் (ret < 0)
		exit(128);
	திரும்பு ret;
}
