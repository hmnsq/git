/*
 * "git replay" builtin command
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "git-compat-util.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hex.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-name.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "refs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "replay.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "revision.h"

ਗਿਣਤੀ ref_action_ਬਹੁਲਕ {
	REF_ACTION_UPDATE,
	REF_ACTION_PRINT,
};

ਸਥਿਰ ਗਿਣਤੀ ref_action_ਬਹੁਲਕ parse_ref_action_ਬਹੁਲਕ(ਅਚਲ ਅੱਖਰ *ref_action, ਅਚਲ ਅੱਖਰ *ਸਰੋਤ)
{
	ਜੇ (!ref_action || !strcmp(ref_action, "update"))
		ਵਾਪਸ REF_ACTION_UPDATE;
	ਜੇ (!strcmp(ref_action, "print"))
		ਵਾਪਸ REF_ACTION_PRINT;
	die(_("invalid %s value: '%s'"), ਸਰੋਤ, ref_action);
}

ਸਥਿਰ ਗਿਣਤੀ ref_action_ਬਹੁਲਕ get_ref_action_ਬਹੁਲਕ(ਬਣਤਰ repository *repo, ਅਚਲ ਅੱਖਰ *ref_action)
{
	ਅਚਲ ਅੱਖਰ *config_ਮੁੱਲ = ਨਿਰਾ;

	/* Command line option takes precedence */
	ਜੇ (ref_action)
		ਵਾਪਸ parse_ref_action_ਬਹੁਲਕ(ref_action, "--ref-action");

	/* Check config value */
	ਜੇ (!repo_config_get_string_tmp(repo, "replay.refAction", &config_ਮੁੱਲ))
		ਵਾਪਸ parse_ref_action_ਬਹੁਲਕ(config_ਮੁੱਲ, "replay.refAction");

	/* Default to update mode */
	ਵਾਪਸ REF_ACTION_UPDATE;
}

ਸਥਿਰ ਪੂਰਨ handle_ref_update(ਗਿਣਤੀ ref_action_ਬਹੁਲਕ ਬਹੁਲਕ,
			     ਬਣਤਰ ref_transaction *transaction,
			     ਅਚਲ ਅੱਖਰ *refname,
			     ਅਚਲ ਬਣਤਰ object_id *new_oid,
			     ਅਚਲ ਬਣਤਰ object_id *old_oid,
			     ਅਚਲ ਅੱਖਰ *reflog_ਸੁਨੇਹਾ,
			     ਬਣਤਰ strbuf *err)
{
	ਚੋਣ (ਬਹੁਲਕ) {
	ਹਾਲਤ REF_ACTION_PRINT:
		ਲਿਖੋ("update %s %s %s\n",
		       refname,
		       oid_ਮੰਜ਼ਿਲ_hex(new_oid),
		       oid_ਮੰਜ਼ਿਲ_hex(old_oid));
		ਵਾਪਸ 0;
	ਹਾਲਤ REF_ACTION_UPDATE:
		ਵਾਪਸ ref_transaction_update(transaction, refname, new_oid, old_oid,
					      ਨਿਰਾ, ਨਿਰਾ, 0, reflog_ਸੁਨੇਹਾ, err);
	ਮੂਲ:
		BUG("unknown ref_action_mode %d", ਬਹੁਲਕ);
	}
}

ਪੂਰਨ cmd_replay(ਪੂਰਨ argc,
	       ਅਚਲ ਅੱਖਰ **argv,
	       ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
	       ਬਣਤਰ repository *repo)
{
	ਬਣਤਰ replay_revisions_options opts = { 0 };
	ਬਣਤਰ replay_ਨਤੀਜਾ ਨਤੀਜਾ = { 0 };
	ਅਚਲ ਅੱਖਰ *ref_action = ਨਿਰਾ;
	ਗਿਣਤੀ ref_action_ਬਹੁਲਕ ref_ਬਹੁਲਕ;
	ਬਣਤਰ rev_info revs;
	ਬਣਤਰ ref_transaction *transaction = ਨਿਰਾ;
	ਬਣਤਰ strbuf transaction_err = STRBUF_INIT;
	ਬਣਤਰ strbuf reflog_ਸੁਨੇਹਾ = STRBUF_INIT;
	ਪੂਰਨ desired_ਉਲਟ;
	ਪੂਰਨ ret = 0;

	ਅਚਲ ਅੱਖਰ *ਅਚਲ replay_usage[] = {
		N_("(EXPERIMENTAL!) git replay "
		   "([--contained] --onto=<newbase> | --advance=<branch> | --revert=<branch>)\n"
		   "[--ref=<ref>] [--ref-action=<mode>] [--linearize] <revision-range>"),
		ਨਿਰਾ
	};
	ਬਣਤਰ option replay_options[] = {
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
		OPT_ਸਿਰਾ()
	};

	argc = parse_options(argc, argv, ਅਗੇਤਰ, replay_options, replay_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT);

	/* Exactly one mode must be specified */
	ਜੇ (!opts.onto && !opts.advance && !opts.revert) {
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
	ref_ਬਹੁਲਕ = get_ref_action_ਬਹੁਲਕ(repo, ref_action);

	/*
	 * Cherry-pick/rebase need oldest-first ordering so that each
	 * replayed commit can build on its already-replayed parent.
	 * Revert needs newest-first ordering (like git revert) to
	 * reduce conflicts by peeling off changes from the top.
	 */
	desired_ਉਲਟ = !opts.revert;

	repo_init_revisions(repo, &revs, ਅਗੇਤਰ);

	/*
	 * Set desired values for rev walking options here. If they
	 * are changed by some user specified option in setup_revisions()
	 * below, we will detect that below and then warn.
	 *
	 * TODO: In the future we might want to either die(), or allow
	 * some options changing these values if we think they could
	 * be useful.
	 */
	revs.ਉਲਟ = desired_ਉਲਟ;
	revs.ਛਾਂਟੋ_ਕ੍ਰਮ = REV_ਛਾਂਟੋ_ਵਿੱਚ_ਗ੍ਰਾਫ_ਕ੍ਰਮ;
	revs.topo_ਕ੍ਰਮ = 1;
	revs.simplify_history = 0;

	argc = ਤਿਆਰੀ_revisions(argc, argv, &revs, ਨਿਰਾ);
	ਜੇ (argc > 1) {
		ret = error(_("unrecognized argument: %s"), argv[1]);
		ਜਾਓ cleanup;
	}

	/*
	 * Detect and warn if we override some user specified rev
	 * walking options.
	 */
	ਜੇ (revs.ਉਲਟ != desired_ਉਲਟ) {
		warning(_("some rev walking options will be overridden as "
			  "'%s' bit in 'struct rev_info' will be forced"),
			"reverse");
		revs.ਉਲਟ = desired_ਉਲਟ;
	}
	ਜੇ (revs.ਛਾਂਟੋ_ਕ੍ਰਮ != REV_ਛਾਂਟੋ_ਵਿੱਚ_ਗ੍ਰਾਫ_ਕ੍ਰਮ) {
		warning(_("some rev walking options will be overridden as "
			  "'%s' bit in 'struct rev_info' will be forced"),
			"sort_order");
		revs.ਛਾਂਟੋ_ਕ੍ਰਮ = REV_ਛਾਂਟੋ_ਵਿੱਚ_ਗ੍ਰਾਫ_ਕ੍ਰਮ;
	}
	ਜੇ (revs.topo_ਕ੍ਰਮ != 1) {
		warning(_("some rev walking options will be overridden as "
			  "'%s' bit in 'struct rev_info' will be forced"),
			"topo_order");
		revs.topo_ਕ੍ਰਮ = 1;
	}
	ਜੇ (revs.simplify_history != 0) {
		warning(_("some rev walking options will be overridden as "
			  "'%s' bit in 'struct rev_info' will be forced"),
			"simplify_history");
		revs.simplify_history = 0;
	}

	ret = replay_revisions(&revs, &opts, &ਨਤੀਜਾ);
	ਜੇ (ret)
		ਜਾਓ cleanup;

	/* Build reflog message */
	ਜੇ (opts.revert) {
		strbuf_addf(&reflog_ਸੁਨੇਹਾ, "replay --revert %s", opts.revert);
	} ਨਹੀਂਤਾਂ ਜੇ (opts.advance) {
		strbuf_addf(&reflog_ਸੁਨੇਹਾ, "replay --advance %s", opts.advance);
	} ਨਹੀਂਤਾਂ {
		ਬਣਤਰ object_id oid;
		ਜੇ (repo_get_oid_committish(repo, opts.onto, &oid))
			BUG("--onto commit should have been resolved beforehand already");
		strbuf_addf(&reflog_ਸੁਨੇਹਾ, "replay --onto %s", oid_ਮੰਜ਼ਿਲ_hex(&oid));
	}

	/* Initialize ref transaction if using update mode */
	ਜੇ (ref_ਬਹੁਲਕ == REF_ACTION_UPDATE) {
		transaction = ref_store_transaction_begin(get_main_ref_store(repo),
							  0, &transaction_err);
		ਜੇ (!transaction) {
			ret = error(_("failed to begin ref transaction: %s"),
				    transaction_err.buf);
			ਜਾਓ cleanup;
		}
	}

	ਲਈ (ਮਾਪ_t i = 0; i < ਨਤੀਜਾ.updates_nr; i++) {
		ret = handle_ref_update(ref_ਬਹੁਲਕ, transaction, ਨਤੀਜਾ.updates[i].refname,
					&ਨਤੀਜਾ.updates[i].new_oid, &ਨਤੀਜਾ.updates[i].old_oid,
					reflog_ਸੁਨੇਹਾ.buf, &transaction_err);
		ਜੇ (ret) {
			ret = error(_("failed to update ref '%s': %s"),
				    ਨਤੀਜਾ.updates[i].refname, transaction_err.buf);
			ਜਾਓ cleanup;
		}
	}

	/* Commit the ref transaction if we have one */
	ਜੇ (transaction) {
		ਜੇ (ref_transaction_commit(transaction, &transaction_err)) {
			ret = error(_("failed to commit ref transaction: %s"),
				    transaction_err.buf);
			ਜਾਓ cleanup;
		}
	}

	ret = 0;

cleanup:
	ਜੇ (transaction)
		ref_transaction_free(transaction);
	replay_ਨਤੀਜਾ_release(&ਨਤੀਜਾ);
	strbuf_release(&transaction_err);
	strbuf_release(&reflog_ਸੁਨੇਹਾ);
	release_revisions(&revs);

	/* Return */
	ਜੇ (ret < 0)
		ਬਾਹਰ_ਨਿਕਲੋ(128);
	ਵਾਪਸ ret;
}
