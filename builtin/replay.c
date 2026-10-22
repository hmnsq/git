/*
 * "git replay" builtin command
 */

#include "git-compat-util.h"

#include "builtin.h"
#include "config.h"
#include "hex.h"
#include "object-name.h"
#include "parse-options.h"
#include "refs.h"
#include "replay.h"
#include "revision.h"

ga_nanaa ref_action_mode {
	REF_ACTION_UPDATE,
	REF_ACTION_PRINT,
};

static ga_nanaa ref_action_mode parse_ref_action_mode(const var_na *ref_action, const var_na *source)
{
	ຖ້າ (!ref_action || !strcmp(ref_action, "update"))
		ສົ່ງຄືນ REF_ACTION_UPDATE;
	ຖ້າ (!strcmp(ref_action, "print"))
		ສົ່ງຄືນ REF_ACTION_PRINT;
	die(_("invalid %s value: '%s'"), source, ref_action);
}

static ga_nanaa ref_action_mode get_ref_action_mode(sa_nracanaa repository *repo, const var_na *ref_action)
{
	const var_na *config_value = NULL;

	/* Command line option takes precedence */
	ຖ້າ (ref_action)
		ສົ່ງຄືນ parse_ref_action_mode(ref_action, "--ref-action");

	/* Check config value */
	ຖ້າ (!repo_config_get_string_tmp(repo, "replay.refAction", &config_value))
		ສົ່ງຄືນ parse_ref_action_mode(config_value, "replay.refAction");

	/* Default to update mode */
	ສົ່ງຄືນ REF_ACTION_UPDATE;
}

static ຈຳນວນເຕັມ handle_ref_update(ga_nanaa ref_action_mode mode,
			     sa_nracanaa ref_transaction *transaction,
			     const var_na *refname,
			     const sa_nracanaa object_id *new_oid,
			     const sa_nracanaa object_id *old_oid,
			     const var_na *reflog_msg,
			     sa_nracanaa strbuf *err)
{
	switch (mode) {
	pak_sa REF_ACTION_PRINT:
		printf("update %s %s %s\n",
		       refname,
		       oid_to_hex(new_oid),
		       oid_to_hex(old_oid));
		ສົ່ງຄືນ 0;
	pak_sa REF_ACTION_UPDATE:
		ສົ່ງຄືນ ref_transaction_update(transaction, refname, new_oid, old_oid,
					      NULL, NULL, 0, reflog_msg, err);
	default:
		BUG("unknown ref_action_mode %d", mode);
	}
}

ຈຳນວນເຕັມ cmd_replay(ຈຳນວນເຕັມ argc,
	       const var_na **argv,
	       const var_na *prefix,
	       sa_nracanaa repository *repo)
{
	sa_nracanaa replay_revisions_options opts = { 0 };
	sa_nracanaa replay_result result = { 0 };
	const var_na *ref_action = NULL;
	ga_nanaa ref_action_mode ref_mode;
	sa_nracanaa rev_info revs;
	sa_nracanaa ref_transaction *transaction = NULL;
	sa_nracanaa strbuf transaction_err = STRBUF_INIT;
	sa_nracanaa strbuf reflog_msg = STRBUF_INIT;
	ຈຳນວນເຕັມ desired_reverse;
	ຈຳນວນເຕັມ ret = 0;

	const var_na *const replay_usage[] = {
		N_("(EXPERIMENTAL!) git replay "
		   "([--contained] --onto=<newbase> | --advance=<branch> | --revert=<branch>)\n"
		   "[--ref=<ref>] [--ref-action=<mode>] [--linearize] <revision-range>"),
		NULL
	};
	sa_nracanaa option replay_options[] = {
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
	ຖ້າ (!opts.onto && !opts.advance && !opts.revert) {
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

	argc = setup_revisions(argc, argv, &revs, NULL);
	ຖ້າ (argc > 1) {
		ret = error(_("unrecognized argument: %s"), argv[1]);
		goto cleanup;
	}

	/*
	 * Detect and warn if we override some user specified rev
	 * walking options.
	 */
	ຖ້າ (revs.reverse != desired_reverse) {
		warning(_("some rev walking options will be overridden as "
			  "'%s' bit in 'struct rev_info' will be forced"),
			"reverse");
		revs.reverse = desired_reverse;
	}
	ຖ້າ (revs.sort_order != REV_SORT_IN_GRAPH_ORDER) {
		warning(_("some rev walking options will be overridden as "
			  "'%s' bit in 'struct rev_info' will be forced"),
			"sort_order");
		revs.sort_order = REV_SORT_IN_GRAPH_ORDER;
	}
	ຖ້າ (revs.topo_order != 1) {
		warning(_("some rev walking options will be overridden as "
			  "'%s' bit in 'struct rev_info' will be forced"),
			"topo_order");
		revs.topo_order = 1;
	}
	ຖ້າ (revs.simplify_history != 0) {
		warning(_("some rev walking options will be overridden as "
			  "'%s' bit in 'struct rev_info' will be forced"),
			"simplify_history");
		revs.simplify_history = 0;
	}

	ret = replay_revisions(&revs, &opts, &result);
	ຖ້າ (ret)
		goto cleanup;

	/* Build reflog message */
	ຖ້າ (opts.revert) {
		strbuf_addf(&reflog_msg, "replay --revert %s", opts.revert);
	} ບໍ່ດັ່ງນັ້ນ ຖ້າ (opts.advance) {
		strbuf_addf(&reflog_msg, "replay --advance %s", opts.advance);
	} ບໍ່ດັ່ງນັ້ນ {
		sa_nracanaa object_id oid;
		ຖ້າ (repo_get_oid_committish(repo, opts.onto, &oid))
			BUG("--onto commit should have been resolved beforehand already");
		strbuf_addf(&reflog_msg, "replay --onto %s", oid_to_hex(&oid));
	}

	/* Initialize ref transaction if using update mode */
	ຖ້າ (ref_mode == REF_ACTION_UPDATE) {
		transaction = ref_store_transaction_begin(get_main_ref_store(repo),
							  0, &transaction_err);
		ຖ້າ (!transaction) {
			ret = error(_("failed to begin ref transaction: %s"),
				    transaction_err.buf);
			goto cleanup;
		}
	}

	ສຳລັບ (size_t i = 0; i < result.updates_nr; i++) {
		ret = handle_ref_update(ref_mode, transaction, result.updates[i].refname,
					&result.updates[i].new_oid, &result.updates[i].old_oid,
					reflog_msg.buf, &transaction_err);
		ຖ້າ (ret) {
			ret = error(_("failed to update ref '%s': %s"),
				    result.updates[i].refname, transaction_err.buf);
			goto cleanup;
		}
	}

	/* Commit the ref transaction if we have one */
	ຖ້າ (transaction) {
		ຖ້າ (ref_transaction_commit(transaction, &transaction_err)) {
			ret = error(_("failed to commit ref transaction: %s"),
				    transaction_err.buf);
			goto cleanup;
		}
	}

	ret = 0;

cleanup:
	ຖ້າ (transaction)
		ref_transaction_free(transaction);
	replay_result_release(&result);
	strbuf_release(&transaction_err);
	strbuf_release(&reflog_msg);
	release_revisions(&revs);

	/* Return */
	ຖ້າ (ret < 0)
		exit(128);
	ສົ່ງຄືນ ret;
}
