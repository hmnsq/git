/*
 * Builtin "git pull"
 *
 * Based on git-pull.sh by Junio C Hamano
 *
 * Fetch one or more remote refs and merge it/them into the current HEAD.
 */

#నిర్వచించు USE_THE_REPOSITORY_VARIABLE

#అన్తర్భావ "builtin.h"
#అన్తర్భావ "advice.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "hex.h"
#అన్తర్భావ "merge.h"
#అన్తర్భావ "object-name.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "run-command.h"
#అన్తర్భావ "oid-array.h"
#అన్తర్భావ "remote.h"
#అన్తర్భావ "dir.h"
#అన్తర్భావ "path.h"
#అన్తర్భావ "read-cache-ll.h"
#అన్తర్భావ "rebase.h"
#అన్తర్భావ "refs.h"
#అన్తర్భావ "refspec.h"
#అన్తర్భావ "submodule.h"
#అన్తర్భావ "submodule-config.h"
#అన్తర్భావ "wt-status.h"
#అన్తర్భావ "commit-reach.h"
#అన్తర్భావ "sequencer.h"

/**
 * Parses the value of --rebase. If value is a false value, returns
 * REBASE_FALSE. If value is a true value, returns REBASE_TRUE. If value is
 * "merges", returns REBASE_MERGES. If value is a invalid value, dies with
 * a fatal error if fatal is true, otherwise returns REBASE_INVALID.
 */
స్థిర గణన rebase_type parse_config_rebase(స్థిరరాశి అక్షరం *కీలకం, స్థిరరాశి అక్షరం *విలువ,
		పూర్ణాంకం fatal)
{
	గణన rebase_type v = rebase_parse_విలువ(విలువ);
	ఒకవేళ (v != REBASE_INVALID)
		తిరిగి v;

	ఒకవేళ (fatal)
		die(_("invalid value for '%s': '%s'"), కీలకం, విలువ);
	లేకపోతే
		error(_("invalid value for '%s': '%s'"), కీలకం, విలువ);

	తిరిగి REBASE_INVALID;
}

/**
 * Callback for --rebase, which parses arg with parse_config_rebase().
 */
స్థిర పూర్ణాంకం parse_opt_rebase(స్థిరరాశి నిర్మాణం option *opt, స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	గణన rebase_type *విలువ = opt->విలువ;

	ఒకవేళ (arg)
		*విలువ = parse_config_rebase("--rebase", arg, 0);
	లేకపోతే
		*విలువ = unset ? REBASE_FALSE : REBASE_TRUE;
	తిరిగి *విలువ == REBASE_INVALID ? -1 : 0;
}

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి pull_usage[] = {
	N_("git pull [<options>] [<repository> [<refspec>...]]"),
	ఖాళీ
};

/* Shared options */
స్థిర పూర్ణాంకం opt_verbosity;
స్థిర స్థిరరాశి అక్షరం *opt_పురోగతి;
స్థిర పూర్ణాంకం recurse_submodules = RECURSE_SUBMODULES_DEFAULT;
స్థిర పూర్ణాంకం recurse_submodules_cli = RECURSE_SUBMODULES_DEFAULT;

/* Options passed to git-merge or git-rebase */
స్థిర గణన rebase_type opt_rebase = -1;
స్థిర స్థిరరాశి అక్షరం *opt_diffstat;
స్థిర స్థిరరాశి అక్షరం *opt_log;
స్థిర స్థిరరాశి అక్షరం *opt_signoff;
స్థిర స్థిరరాశి అక్షరం *opt_squash;
స్థిర స్థిరరాశి అక్షరం *opt_commit;
స్థిర స్థిరరాశి అక్షరం *opt_edit;
స్థిర స్థిరరాశి అక్షరం *cleanup_arg;
స్థిర అక్షరం *opt_ff;
స్థిర స్థిరరాశి అక్షరం *opt_verify_signatures;
స్థిర స్థిరరాశి అక్షరం *opt_verify;
స్థిర పూర్ణాంకం opt_autostash = -1;
స్థిర పూర్ణాంకం config_rebase_autostash;
స్థిర పూర్ణాంకం config_pull_autostash = -1;
స్థిర పూర్ణాంకం check_trust_level = 1;
స్థిర నిర్మాణం strvec opt_strategies = STRVEC_INIT;
స్థిర నిర్మాణం strvec opt_strategy_opts = STRVEC_INIT;
స్థిర స్థిరరాశి అక్షరం *opt_gpg_sign;
స్థిర పూర్ణాంకం opt_allow_unrelated_histories;

/* Options passed to git-fetch */
స్థిర స్థిరరాశి అక్షరం *opt_all;
స్థిర స్థిరరాశి అక్షరం *opt_append;
స్థిర స్థిరరాశి అక్షరం *opt_upload_pack;
స్థిర పూర్ణాంకం opt_force;
స్థిర స్థిరరాశి అక్షరం *opt_ట్యాగులు;
స్థిర స్థిరరాశి అక్షరం *opt_prune;
స్థిర స్థిరరాశి అక్షరం *max_children;
స్థిర పూర్ణాంకం opt_dry_నిరంతరం;
స్థిర స్థిరరాశి అక్షరం *opt_keep;
స్థిర స్థిరరాశి అక్షరం *opt_లోతు;
స్థిర స్థిరరాశి అక్షరం *opt_unshallow;
స్థిర స్థిరరాశి అక్షరం *opt_update_shallow;
స్థిర స్థిరరాశి అక్షరం *opt_refmap;
స్థిర స్థిరరాశి అక్షరం *opt_ipv4;
స్థిర స్థిరరాశి అక్షరం *opt_ipv6;
స్థిర పూర్ణాంకం opt_show_forced_updates = -1;
స్థిర స్థిరరాశి అక్షరం *సమితి_upstream;
స్థిర నిర్మాణం strvec opt_fetch = STRVEC_INIT;

/**
 * Pushes "-q" or "-v" switches into arr to match the opt_verbosity level.
 */
స్థిర శూన్య argv_నెట్టు_verbosity(నిర్మాణం strvec *arr)
{
	పూర్ణాంకం verbosity;

	కొరకు (verbosity = opt_verbosity; verbosity > 0; verbosity--)
		strvec_నెట్టు(arr, "-v");

	కొరకు (verbosity = opt_verbosity; verbosity < 0; verbosity++)
		strvec_నెట్టు(arr, "-q");
}

/**
 * Pushes "-f" switches into arr to match the opt_force level.
 */
స్థిర శూన్య argv_నెట్టు_force(నిర్మాణం strvec *arr)
{
	పూర్ణాంకం force = opt_force;
	అయితే (force-- > 0)
		strvec_నెట్టు(arr, "-f");
}

/**
 * Sets the GIT_REFLOG_ACTION environment variable to the concatenation of argv
 */
స్థిర శూన్య సమితి_reflog_సందేశం(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv)
{
	పూర్ణాంకం i;
	నిర్మాణం strbuf సందేశం = STRBUF_INIT;

	కొరకు (i = 0; i < argc; i++) {
		ఒకవేళ (i)
			strbuf_addch(&సందేశం, ' ');
		strbuf_addstr(&సందేశం, argv[i]);
	}

	setenv("GIT_REFLOG_ACTION", సందేశం.buf, 0);

	strbuf_release(&సందేశం);
}

/**
 * If pull.ff is unset, returns NULL. If pull.ff is "true", returns "--ff". If
 * pull.ff is "false", returns "--no-ff". If pull.ff is "only", returns
 * "--ff-only". Otherwise, if pull.ff is set to an invalid value, die with an
 * error.
 */
స్థిర స్థిరరాశి అక్షరం *config_get_ff(శూన్య)
{
	స్థిరరాశి అక్షరం *విలువ;

	ఒకవేళ (repo_config_get_విలువ(the_repository, "pull.ff", &విలువ))
		తిరిగి ఖాళీ;

	ఎంపిక (git_parse_maybe_bool(విలువ)) {
	స్థితి 0:
		తిరిగి "--no-ff";
	స్థితి 1:
		తిరిగి "--ff";
	}

	ఒకవేళ (!strcmp(విలువ, "only"))
		తిరిగి "--ff-only";

	die(_("invalid value for '%s': '%s'"), "pull.ff", విలువ);
}

/**
 * Returns the default configured value for --rebase. It first looks for the
 * value of "branch.$curr_branch.rebase", where $curr_branch is the current
 * branch, and if HEAD is detached or the configuration key does not exist,
 * looks for the value of "pull.rebase". If both configuration keys do not
 * exist, returns REBASE_FALSE.
 */
స్థిర గణన rebase_type config_get_rebase(పూర్ణాంకం *rebase_unspecified)
{
	నిర్మాణం branch *curr_branch = branch_get("HEAD");
	స్థిరరాశి అక్షరం *విలువ;

	ఒకవేళ (curr_branch) {
		అక్షరం *కీలకం = xstrfmt("branch.%s.rebase", curr_branch->name);

		ఒకవేళ (!repo_config_get_విలువ(the_repository, కీలకం, &విలువ)) {
			గణన rebase_type ret = parse_config_rebase(కీలకం, విలువ, 1);
			విడుదల(కీలకం);
			తిరిగి ret;
		}

		విడుదల(కీలకం);
	}

	ఒకవేళ (!repo_config_get_విలువ(the_repository, "pull.rebase", &విలువ))
		తిరిగి parse_config_rebase("pull.rebase", విలువ, 1);

	*rebase_unspecified = 1;

	తిరిగి REBASE_FALSE;
}

/**
 * Read config variables.
 */
స్థిర పూర్ణాంకం git_pull_config(స్థిరరాశి అక్షరం *var, స్థిరరాశి అక్షరం *విలువ,
			   స్థిరరాశి నిర్మాణం config_context *ctx, శూన్య *cb)
{
	ఒకవేళ (!strcmp(var, "rebase.autostash")) {
		/*
		 * run_rebase() also reads this option. The reason we handle it here is
		 * that when pull.rebase is true, a fast-forward may occur without
		 * invoking run_rebase(). We need to ensure that autostash is set even
		 * in the fast-forward case.
		 *
		 * run_merge() handles merge.autostash, so we don't handle it here.
		 */
		config_rebase_autostash = git_config_bool(var, విలువ);
		తిరిగి 0;
	} లేకపోతే ఒకవేళ (!strcmp(var, "pull.autostash")) {
		config_pull_autostash = git_config_bool(var, విలువ);
		తిరిగి 0;
	} లేకపోతే ఒకవేళ (!strcmp(var, "submodule.recurse")) {
		recurse_submodules = git_config_bool(var, విలువ) ?
			RECURSE_SUBMODULES_ON : RECURSE_SUBMODULES_OFF;
		తిరిగి 0;
	} లేకపోతే ఒకవేళ (!strcmp(var, "gpg.mintrustlevel")) {
		check_trust_level = 0;
	}

	తిరిగి git_default_config(var, విలువ, ctx, cb);
}

/**
 * Appends merge candidates from FETCH_HEAD that are not marked not-for-merge
 * into merge_heads.
 */
స్థిర శూన్య get_విలీనం_heads(నిర్మాణం oid_array *విలీనం_heads)
{
	స్థిరరాశి అక్షరం *filename = git_మార్గం_fetch_తల(the_repository);
	FILE *fp;
	నిర్మాణం strbuf sb = STRBUF_INIT;
	నిర్మాణం object_id oid;

	fp = xfopen(filename, "r");
	అయితే (strbuf_getline_lf(&sb, fp) != EOF) {
		స్థిరరాశి అక్షరం *p;
		ఒకవేళ (parse_oid_hex(sb.buf, &oid, &p))
			కొనసాగు;  /* invalid line: does not start with object ID */
		ఒకవేళ (starts_with(p, "\tnot-for-merge\t"))
			కొనసాగు;  /* ref is not-for-merge */
		oid_array_append(విలీనం_heads, &oid);
	}
	fclose(fp);
	strbuf_release(&sb);
}

/**
 * Used by die_no_merge_candidates() as a for_each_remote() callback to
 * retrieve the name of the remote if the repository only has one remote.
 */
స్థిర పూర్ణాంకం get_only_remote(నిర్మాణం remote *remote, శూన్య *cb_దత్తాంశం)
{
	స్థిరరాశి అక్షరం **remote_name = cb_దత్తాంశం;

	ఒకవేళ (*remote_name)
		తిరిగి -1;

	*remote_name = remote->name;
	తిరిగి 0;
}

/**
 * Dies with the appropriate reason for why there are no merge candidates:
 *
 * 1. We fetched from a specific remote, and a refspec was given, but it ended
 *    up not fetching anything. This is usually because the user provided a
 *    wildcard refspec which had no matches on the remote end.
 *
 * 2. We fetched from a non-default remote, but didn't specify a branch to
 *    merge. We can't use the configured one because it applies to the default
 *    remote, thus the user must specify the branches to merge.
 *
 * 3. We fetched from the branch's or repo's default remote, but:
 *
 *    a. We are not on a branch, so there will never be a configured branch to
 *       merge with.
 *
 *    b. We are on a branch, but there is no configured branch to merge with.
 *
 * 4. We fetched from the branch's or repo's default remote, but the configured
 *    branch to merge didn't get fetched. (Either it doesn't exist, or wasn't
 *    part of the configured fetch refspec.)
 */
స్థిర శూన్య NORETURN die_no_విలీనం_candidates(స్థిరరాశి అక్షరం *repo, స్థిరరాశి అక్షరం **refspecs)
{
	నిర్మాణం branch *curr_branch = branch_get("HEAD");
	స్థిరరాశి అక్షరం *remote = curr_branch ? curr_branch->remote_name : ఖాళీ;

	ఒకవేళ (*refspecs) {
		ఒకవేళ (opt_rebase)
			fprintf_ln(stderr, _("There is no candidate for rebasing against among the refs that you just fetched."));
		లేకపోతే
			fprintf_ln(stderr, _("There are no candidates for merging among the refs that you just fetched."));
		fprintf_ln(stderr, _("Generally this means that you provided a wildcard refspec which had no\n"
					"matches on the remote end."));
	} లేకపోతే ఒకవేళ (repo && curr_branch && (!remote || strcmp(repo, remote))) {
		fprintf_ln(stderr, _("You asked to pull from the remote '%s', but did not specify\n"
			"a branch. Because this is not the default configured remote\n"
			"for your current branch, you must specify a branch on the command line."),
			repo);
	} లేకపోతే ఒకవేళ (!curr_branch) {
		fprintf_ln(stderr, _("You are not currently on a branch."));
		ఒకవేళ (opt_rebase)
			fprintf_ln(stderr, _("Please specify which branch you want to rebase against."));
		లేకపోతే
			fprintf_ln(stderr, _("Please specify which branch you want to merge with."));
		fprintf_ln(stderr, _("See git-pull(1) for details."));
		fprintf(stderr, "\n");
		fprintf_ln(stderr, "    git pull %s %s", _("<remote>"), _("<branch>"));
		fprintf(stderr, "\n");
	} లేకపోతే ఒకవేళ (!curr_branch->విలీనం_nr) {
		స్థిరరాశి అక్షరం *remote_name = ఖాళీ;

		ఒకవేళ (for_each_remote(get_only_remote, &remote_name) || !remote_name)
			remote_name = _("<remote>");

		fprintf_ln(stderr, _("There is no tracking information for the current branch."));
		ఒకవేళ (opt_rebase)
			fprintf_ln(stderr, _("Please specify which branch you want to rebase against."));
		లేకపోతే
			fprintf_ln(stderr, _("Please specify which branch you want to merge with."));
		fprintf_ln(stderr, _("See git-pull(1) for details."));
		fprintf(stderr, "\n");
		fprintf_ln(stderr, "    git pull %s %s", _("<remote>"), _("<branch>"));
		fprintf(stderr, "\n");
		fprintf_ln(stderr, _("If you wish to set tracking information for this branch you can do so with:"));
		fprintf(stderr, "\n");
		fprintf_ln(stderr, "    git branch --set-upstream-to=%s/%s %s\n",
				remote_name, _("<branch>"), curr_branch->name);
	} లేకపోతే
		fprintf_ln(stderr, _("Your configuration specifies to merge with the ref '%s'\n"
			"from the remote, but no such ref was fetched."),
			curr_branch->విలీనం[0]->src);
	నిష్క్రమించు(1);
}

/**
 * Parses argv into [<repo> [<refspecs>...]], returning their values in `repo`
 * as a string and `refspecs` as a null-terminated array of strings. If `repo`
 * is not provided in argv, it is set to NULL.
 */
స్థిర శూన్య parse_repo_refspecs(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం **repo,
		స్థిరరాశి అక్షరం ***refspecs)
{
	ఒకవేళ (argc > 0) {
		*repo = *argv++;
		argc--;
	} లేకపోతే
		*repo = ఖాళీ;
	*refspecs = argv;
}

/**
 * Runs git-fetch, returning its exit status. `repo` and `refspecs` are the
 * repository and refspecs to fetch, or NULL if they are not provided.
 */
స్థిర పూర్ణాంకం నిరంతరం_fetch(స్థిరరాశి అక్షరం *repo, స్థిరరాశి అక్షరం **refspecs)
{
	నిర్మాణం బిడ్డ_process cmd = బిడ్డ_PROCESS_INIT;

	strvec_pushl(&cmd.వాదనలు, "fetch", "--update-head-ok", ఖాళీ);

	/* Shared options */
	argv_నెట్టు_verbosity(&cmd.వాదనలు);
	ఒకవేళ (opt_పురోగతి)
		strvec_నెట్టు(&cmd.వాదనలు, opt_పురోగతి);

	/* Options passed to git-fetch */
	ఒకవేళ (opt_all)
		strvec_నెట్టు(&cmd.వాదనలు, opt_all);
	ఒకవేళ (opt_append)
		strvec_నెట్టు(&cmd.వాదనలు, opt_append);
	ఒకవేళ (opt_upload_pack)
		strvec_నెట్టు(&cmd.వాదనలు, opt_upload_pack);
	argv_నెట్టు_force(&cmd.వాదనలు);
	ఒకవేళ (opt_ట్యాగులు)
		strvec_నెట్టు(&cmd.వాదనలు, opt_ట్యాగులు);
	ఒకవేళ (opt_prune)
		strvec_నెట్టు(&cmd.వాదనలు, opt_prune);
	ఒకవేళ (recurse_submodules_cli != RECURSE_SUBMODULES_DEFAULT)
		ఎంపిక (recurse_submodules_cli) {
		స్థితి RECURSE_SUBMODULES_ON:
			strvec_నెట్టు(&cmd.వాదనలు, "--recurse-submodules=on");
			ఆపు;
		స్థితి RECURSE_SUBMODULES_OFF:
			strvec_నెట్టు(&cmd.వాదనలు, "--recurse-submodules=no");
			ఆపు;
		స్థితి RECURSE_SUBMODULES_ON_DEMAND:
			strvec_నెట్టు(&cmd.వాదనలు, "--recurse-submodules=on-demand");
			ఆపు;
		అప్రమేయ:
			BUG("submodule recursion option not understood");
		}
	ఒకవేళ (max_children)
		strvec_నెట్టు(&cmd.వాదనలు, max_children);
	ఒకవేళ (opt_dry_నిరంతరం)
		strvec_నెట్టు(&cmd.వాదనలు, "--dry-run");
	ఒకవేళ (opt_keep)
		strvec_నెట్టు(&cmd.వాదనలు, opt_keep);
	ఒకవేళ (opt_లోతు)
		strvec_నెట్టు(&cmd.వాదనలు, opt_లోతు);
	ఒకవేళ (opt_unshallow)
		strvec_నెట్టు(&cmd.వాదనలు, opt_unshallow);
	ఒకవేళ (opt_update_shallow)
		strvec_నెట్టు(&cmd.వాదనలు, opt_update_shallow);
	ఒకవేళ (opt_refmap)
		strvec_నెట్టు(&cmd.వాదనలు, opt_refmap);
	ఒకవేళ (opt_ipv4)
		strvec_నెట్టు(&cmd.వాదనలు, opt_ipv4);
	ఒకవేళ (opt_ipv6)
		strvec_నెట్టు(&cmd.వాదనలు, opt_ipv6);
	ఒకవేళ (opt_show_forced_updates > 0)
		strvec_నెట్టు(&cmd.వాదనలు, "--show-forced-updates");
	లేకపోతే ఒకవేళ (opt_show_forced_updates == 0)
		strvec_నెట్టు(&cmd.వాదనలు, "--no-show-forced-updates");
	ఒకవేళ (సమితి_upstream)
		strvec_నెట్టు(&cmd.వాదనలు, సమితి_upstream);
	strvec_pushv(&cmd.వాదనలు, opt_fetch.v);

	ఒకవేళ (repo) {
		strvec_నెట్టు(&cmd.వాదనలు, repo);
		strvec_pushv(&cmd.వాదనలు, refspecs);
	} లేకపోతే ఒకవేళ (*refspecs)
		BUG("refspecs without repo?");
	cmd.git_cmd = 1;
	cmd.odb_గమ్యం_close = the_repository->objects;
	తిరిగి నిరంతరం_command(&cmd);
}

/**
 * "Pulls into void" by branching off merge_head.
 */
స్థిర పూర్ణాంకం pull_into_void(స్థిరరాశి నిర్మాణం object_id *విలీనం_తల,
		స్థిరరాశి నిర్మాణం object_id *curr_తల)
{
	ఒకవేళ (opt_verify_signatures) {
		నిర్మాణం commit *commit;

		commit = lookup_commit(the_repository, విలీనం_తల);
		ఒకవేళ (!commit)
			die(_("unable to access commit %s"),
			    oid_గమ్యం_hex(విలీనం_తల));

		verify_విలీనం_signature(commit, opt_verbosity,
				       check_trust_level);
	}

	/*
	 * Two-way merge: we treat the index as based on an empty tree,
	 * and try to fast-forward to HEAD. This ensures we will not lose
	 * index/worktree changes that the user already made on the unborn
	 * branch.
	 */
	ఒకవేళ (checkout_fast_forward(the_repository,
				  the_hash_algo->ఖాళీలు_వృక్షం,
				  విలీనం_తల, 0))
		తిరిగి 1;

	ఒకవేళ (refs_update_ref(get_main_ref_store(the_repository), "initial pull", "HEAD", విలీనం_తల, curr_తల, 0, UPDATE_REFS_DIE_ON_ERR))
		తిరిగి 1;

	తిరిగి 0;
}

స్థిర పూర్ణాంకం rebase_submodules(శూన్య)
{
	నిర్మాణం బిడ్డ_process cp = బిడ్డ_PROCESS_INIT;

	cp.git_cmd = 1;
	cp.no_stdin = 1;
	strvec_pushl(&cp.వాదనలు, "submodule", "update",
		     "--recursive", "--rebase", ఖాళీ);
	argv_నెట్టు_verbosity(&cp.వాదనలు);

	తిరిగి నిరంతరం_command(&cp);
}

స్థిర పూర్ణాంకం update_submodules(శూన్య)
{
	నిర్మాణం బిడ్డ_process cp = బిడ్డ_PROCESS_INIT;

	cp.git_cmd = 1;
	cp.no_stdin = 1;
	strvec_pushl(&cp.వాదనలు, "submodule", "update",
		     "--recursive", "--checkout", ఖాళీ);
	argv_నెట్టు_verbosity(&cp.వాదనలు);

	తిరిగి నిరంతరం_command(&cp);
}

/**
 * Runs git-merge, returning its exit status.
 */
స్థిర పూర్ణాంకం నిరంతరం_విలీనం(శూన్య)
{
	నిర్మాణం బిడ్డ_process cmd = బిడ్డ_PROCESS_INIT;

	strvec_pushl(&cmd.వాదనలు, "merge", ఖాళీ);

	/* Shared options */
	argv_నెట్టు_verbosity(&cmd.వాదనలు);
	ఒకవేళ (opt_పురోగతి)
		strvec_నెట్టు(&cmd.వాదనలు, opt_పురోగతి);

	/* Options passed to git-merge */
	ఒకవేళ (opt_diffstat)
		strvec_నెట్టు(&cmd.వాదనలు, opt_diffstat);
	ఒకవేళ (opt_log)
		strvec_నెట్టు(&cmd.వాదనలు, opt_log);
	ఒకవేళ (opt_signoff)
		strvec_నెట్టు(&cmd.వాదనలు, opt_signoff);
	ఒకవేళ (opt_squash)
		strvec_నెట్టు(&cmd.వాదనలు, opt_squash);
	ఒకవేళ (opt_commit)
		strvec_నెట్టు(&cmd.వాదనలు, opt_commit);
	ఒకవేళ (opt_edit)
		strvec_నెట్టు(&cmd.వాదనలు, opt_edit);
	ఒకవేళ (cleanup_arg)
		strvec_pushf(&cmd.వాదనలు, "--cleanup=%s", cleanup_arg);
	ఒకవేళ (opt_ff)
		strvec_నెట్టు(&cmd.వాదనలు, opt_ff);
	ఒకవేళ (opt_verify)
		strvec_నెట్టు(&cmd.వాదనలు, opt_verify);
	ఒకవేళ (opt_verify_signatures)
		strvec_నెట్టు(&cmd.వాదనలు, opt_verify_signatures);
	strvec_pushv(&cmd.వాదనలు, opt_strategies.v);
	strvec_pushv(&cmd.వాదనలు, opt_strategy_opts.v);
	ఒకవేళ (opt_gpg_sign)
		strvec_నెట్టు(&cmd.వాదనలు, opt_gpg_sign);
	ఒకవేళ (opt_autostash == 0)
		strvec_నెట్టు(&cmd.వాదనలు, "--no-autostash");
	లేకపోతే ఒకవేళ (opt_autostash == 1)
		strvec_నెట్టు(&cmd.వాదనలు, "--autostash");
	ఒకవేళ (opt_allow_unrelated_histories > 0)
		strvec_నెట్టు(&cmd.వాదనలు, "--allow-unrelated-histories");

	strvec_నెట్టు(&cmd.వాదనలు, "FETCH_HEAD");
	cmd.git_cmd = 1;
	తిరిగి నిరంతరం_command(&cmd);
}

/**
 * Returns remote's upstream branch for the current branch. If remote is NULL,
 * the current branch's configured default remote is used. Returns NULL if
 * `remote` does not name a valid remote, HEAD does not point to a branch,
 * remote is not the branch's configured remote or the branch does not have any
 * configured upstream branch.
 */
స్థిర స్థిరరాశి అక్షరం *get_upstream_branch(స్థిరరాశి అక్షరం *remote)
{
	నిర్మాణం remote *rm;
	నిర్మాణం branch *curr_branch;
	స్థిరరాశి అక్షరం *curr_branch_remote;

	rm = remote_get(remote);
	ఒకవేళ (!rm)
		తిరిగి ఖాళీ;

	curr_branch = branch_get("HEAD");
	ఒకవేళ (!curr_branch)
		తిరిగి ఖాళీ;

	curr_branch_remote = remote_for_branch(curr_branch, ఖాళీ);
	ప్రతిజ్ఞా(curr_branch_remote);

	ఒకవేళ (strcmp(curr_branch_remote, rm->name))
		తిరిగి ఖాళీ;

	తిరిగి branch_get_upstream(curr_branch, ఖాళీ);
}

/**
 * Derives the remote-tracking branch from the remote and refspec.
 *
 * FIXME: The current implementation assumes the default mapping of
 * refs/heads/<branch_name> to refs/remotes/<remote_name>/<branch_name>.
 */
స్థిర స్థిరరాశి అక్షరం *get_tracking_branch(స్థిరరాశి అక్షరం *remote, స్థిరరాశి అక్షరం *refspec)
{
	నిర్మాణం refspec_అంశం spec;
	స్థిరరాశి అక్షరం *spec_src;
	స్థిరరాశి అక్షరం *విలీనం_branch;

	ఒకవేళ (!refspec_అంశం_init_fetch(&spec, refspec, the_hash_algo))
		die(_("invalid refspec '%s'"), refspec);
	spec_src = spec.src;
	ఒకవేళ (!*spec_src || !strcmp(spec_src, "HEAD"))
		spec_src = "HEAD";
	లేకపోతే ఒకవేళ (skip_ఉపసర్గ(spec_src, "heads/", &spec_src))
		;
	లేకపోతే ఒకవేళ (skip_ఉపసర్గ(spec_src, "refs/heads/", &spec_src))
		;
	లేకపోతే ఒకవేళ (starts_with(spec_src, "refs/") ||
		starts_with(spec_src, "tags/") ||
		starts_with(spec_src, "remotes/"))
		spec_src = "";

	ఒకవేళ (*spec_src) {
		ఒకవేళ (!strcmp(remote, "."))
			విలీనం_branch = mkpath("refs/heads/%s", spec_src);
		లేకపోతే
			విలీనం_branch = mkpath("refs/remotes/%s/%s", remote, spec_src);
	} లేకపోతే
		విలీనం_branch = ఖాళీ;

	refspec_అంశం_clear(&spec);
	తిరిగి విలీనం_branch;
}

/**
 * Given the repo and refspecs, sets fork_point to the point at which the
 * current branch forked from its remote-tracking branch. Returns 0 on success,
 * -1 on failure.
 */
స్థిర పూర్ణాంకం get_rebase_fork_బిందువు(నిర్మాణం object_id *fork_బిందువు, స్థిరరాశి అక్షరం *repo,
		స్థిరరాశి అక్షరం *refspec)
{
	పూర్ణాంకం ret;
	నిర్మాణం branch *curr_branch;
	స్థిరరాశి అక్షరం *remote_branch;
	నిర్మాణం బిడ్డ_process cp = బిడ్డ_PROCESS_INIT;
	నిర్మాణం strbuf sb = STRBUF_INIT;

	curr_branch = branch_get("HEAD");
	ఒకవేళ (!curr_branch)
		తిరిగి -1;

	ఒకవేళ (refspec)
		remote_branch = get_tracking_branch(repo, refspec);
	లేకపోతే
		remote_branch = get_upstream_branch(repo);

	ఒకవేళ (!remote_branch)
		తిరిగి -1;

	strvec_pushl(&cp.వాదనలు, "merge-base", "--fork-point",
		     remote_branch, curr_branch->name, ఖాళీ);
	cp.no_stdin = 1;
	cp.no_stderr = 1;
	cp.git_cmd = 1;

	ret = capture_command(&cp, &sb, GIT_MAX_HEXSZ);
	ఒకవేళ (ret)
		వెళ్ళు cleanup;

	ret = get_oid_hex(sb.buf, fork_బిందువు);
	ఒకవేళ (ret)
		వెళ్ళు cleanup;

cleanup:
	strbuf_release(&sb);
	తిరిగి ret ? -1 : 0;
}

/**
 * Sets merge_base to the octopus merge base of curr_head, merge_head and
 * fork_point. Returns 0 if a merge base is found, 1 otherwise.
 */
స్థిర పూర్ణాంకం get_octopus_విలీనం_పునాది(నిర్మాణం object_id *విలీనం_పునాది,
		స్థిరరాశి నిర్మాణం object_id *curr_తల,
		స్థిరరాశి నిర్మాణం object_id *విలీనం_తల,
		స్థిరరాశి నిర్మాణం object_id *fork_బిందువు)
{
	నిర్మాణం commit_list *revs = ఖాళీ, *ఫలితం = ఖాళీ;

	commit_list_చొప్పించు(lookup_commit_reference(the_repository, curr_తల),
			   &revs);
	commit_list_చొప్పించు(lookup_commit_reference(the_repository, విలీనం_తల),
			   &revs);
	ఒకవేళ (!is_null_oid(fork_బిందువు))
		commit_list_చొప్పించు(lookup_commit_reference(the_repository, fork_బిందువు),
				   &revs);

	ఒకవేళ (get_octopus_విలీనం_bases(revs, &ఫలితం) < 0)
		నిష్క్రమించు(128);
	commit_list_free(revs);
	reduce_heads_replace(&ఫలితం);

	ఒకవేళ (!ఫలితం)
		తిరిగి 1;

	oidcpy(విలీనం_పునాది, &ఫలితం->అంశం->object.oid);
	commit_list_free(ఫలితం);
	తిరిగి 0;
}

/**
 * Given the current HEAD oid, the merge head returned from git-fetch and the
 * fork point calculated by get_rebase_fork_point(), compute the <newbase> and
 * <upstream> arguments to use for the upcoming git-rebase invocation.
 */
స్థిర పూర్ణాంకం get_rebase_newbase_and_upstream(నిర్మాణం object_id *newbase,
		నిర్మాణం object_id *upstream,
		స్థిరరాశి నిర్మాణం object_id *curr_తల,
		స్థిరరాశి నిర్మాణం object_id *విలీనం_తల,
		స్థిరరాశి నిర్మాణం object_id *fork_బిందువు)
{
	నిర్మాణం object_id oct_విలీనం_పునాది;

	ఒకవేళ (!get_octopus_విలీనం_పునాది(&oct_విలీనం_పునాది, curr_తల, విలీనం_తల, fork_బిందువు))
		ఒకవేళ (!is_null_oid(fork_బిందువు) && oideq(&oct_విలీనం_పునాది, fork_బిందువు))
			fork_బిందువు = ఖాళీ;

	ఒకవేళ (fork_బిందువు && !is_null_oid(fork_బిందువు))
		oidcpy(upstream, fork_బిందువు);
	లేకపోతే
		oidcpy(upstream, విలీనం_తల);

	oidcpy(newbase, విలీనం_తల);

	తిరిగి 0;
}

/**
 * Given the <newbase> and <upstream> calculated by
 * get_rebase_newbase_and_upstream(), runs git-rebase with the
 * appropriate arguments and returns its exit status.
 */
స్థిర పూర్ణాంకం నిరంతరం_rebase(స్థిరరాశి నిర్మాణం object_id *newbase,
		స్థిరరాశి నిర్మాణం object_id *upstream)
{
	నిర్మాణం బిడ్డ_process cmd = బిడ్డ_PROCESS_INIT;

	strvec_నెట్టు(&cmd.వాదనలు, "rebase");

	/* Shared options */
	argv_నెట్టు_verbosity(&cmd.వాదనలు);

	/* Options passed to git-rebase */
	ఒకవేళ (opt_rebase == REBASE_MERGES)
		strvec_నెట్టు(&cmd.వాదనలు, "--rebase-merges");
	లేకపోతే ఒకవేళ (opt_rebase == REBASE_INTERACTIVE)
		strvec_నెట్టు(&cmd.వాదనలు, "--interactive");
	ఒకవేళ (opt_diffstat)
		strvec_నెట్టు(&cmd.వాదనలు, opt_diffstat);
	strvec_pushv(&cmd.వాదనలు, opt_strategies.v);
	strvec_pushv(&cmd.వాదనలు, opt_strategy_opts.v);
	ఒకవేళ (opt_gpg_sign)
		strvec_నెట్టు(&cmd.వాదనలు, opt_gpg_sign);
	ఒకవేళ (opt_signoff)
		strvec_నెట్టు(&cmd.వాదనలు, opt_signoff);
	ఒకవేళ (opt_autostash == 0)
		strvec_నెట్టు(&cmd.వాదనలు, "--no-autostash");
	లేకపోతే ఒకవేళ (opt_autostash == 1)
		strvec_నెట్టు(&cmd.వాదనలు, "--autostash");
	ఒకవేళ (opt_verify_signatures &&
	    !strcmp(opt_verify_signatures, "--verify-signatures"))
		warning(_("ignoring --verify-signatures for rebase"));

	strvec_నెట్టు(&cmd.వాదనలు, "--onto");
	strvec_నెట్టు(&cmd.వాదనలు, oid_గమ్యం_hex(newbase));

	strvec_నెట్టు(&cmd.వాదనలు, oid_గమ్యం_hex(upstream));

	cmd.git_cmd = 1;
	తిరిగి నిరంతరం_command(&cmd);
}

స్థిర పూర్ణాంకం get_can_ff(నిర్మాణం object_id *orig_తల,
		      నిర్మాణం oid_array *విలీనం_heads)
{
	పూర్ణాంకం ret;
	నిర్మాణం commit_list *list = ఖాళీ;
	నిర్మాణం commit *విలీనం_తల, *తల;
	నిర్మాణం object_id *orig_విలీనం_తల;

	ఒకవేళ (విలీనం_heads->nr > 1)
		తిరిగి 0;

	orig_విలీనం_తల = &విలీనం_heads->oid[0];
	తల = lookup_commit_reference(the_repository, orig_తల);
	ఒకవేళ (!తల)
		తిరిగి 0;
	విలీనం_తల = lookup_commit_reference(the_repository, orig_విలీనం_తల);
	ఒకవేళ (!విలీనం_తల)
		తిరిగి 0;
	commit_list_చొప్పించు(తల, &list);
	ret = repo_is_descendant_యొక్క(the_repository, విలీనం_తల, list);
	commit_list_free(list);
	ఒకవేళ (ret < 0)
		నిష్క్రమించు(128);
	తిరిగి ret;
}

/*
 * Is orig_head a descendant of _all_ merge_heads?
 * Unfortunately is_descendant_of() cannot be used as it asks
 * if orig_head is a descendant of at least one of them.
 */
స్థిర పూర్ణాంకం already_పైకి_గమ్యం_date(నిర్మాణం object_id *orig_తల,
			      నిర్మాణం oid_array *విలీనం_heads)
{
	నిర్మాణం commit *ours;

	ours = lookup_commit_reference(the_repository, orig_తల);
	ఒకవేళ (!ours)
		తిరిగి 0;
	కొరకు (కొలత_t i = 0; i < విలీనం_heads->nr; i++) {
		నిర్మాణం commit_list *list = ఖాళీ;
		నిర్మాణం commit *theirs;
		పూర్ణాంకం ok;

		theirs = lookup_commit_reference(the_repository, &విలీనం_heads->oid[i]);
		ఒకవేళ (!theirs)
			తిరిగి 0;
		commit_list_చొప్పించు(theirs, &list);
		ok = repo_is_descendant_యొక్క(the_repository, ours, list);
		commit_list_free(list);
		ఒకవేళ (ok < 0)
			నిష్క్రమించు(128);
		ఒకవేళ (!ok)
			తిరిగి 0;
	}
	తిరిగి 1;
}

స్థిర శూన్య show_advice_pull_non_ff(శూన్య)
{
	advise(_("You have divergent branches and need to specify how to reconcile them.\n"
		 "You can do so by running one of the following commands sometime before\n"
		 "your next pull:\n"
		 "\n"
		 "  git config pull.rebase false  # merge\n"
		 "  git config pull.rebase true   # rebase\n"
		 "  git config pull.ff only       # fast-forward only\n"
		 "\n"
		 "You can replace \"git config\" with \"git config --global\" to set a default\n"
		 "preference for all repositories. You can also pass --rebase, --no-rebase,\n"
		 "or --ff-only on the command line to override the configured default per\n"
		 "invocation.\n"));
}

పూర్ణాంకం cmd_pull(పూర్ణాంకం argc,
	     స్థిరరాశి అక్షరం **argv,
	     స్థిరరాశి అక్షరం *ఉపసర్గ,
	     నిర్మాణం repository *repository వాడని)
{
	స్థిరరాశి అక్షరం *repo, **refspecs;
	నిర్మాణం oid_array విలీనం_heads = OID_ARRAY_INIT;
	నిర్మాణం object_id orig_తల, curr_తల;
	నిర్మాణం object_id rebase_fork_బిందువు;
	పూర్ణాంకం rebase_unspecified = 0;
	పూర్ణాంకం can_ff;
	పూర్ణాంకం divergent;
	పూర్ణాంకం ret;
	స్థిర నిర్మాణం option pull_options[] = {
		/* Shared options */
		OPT__VERBOSITY(&opt_verbosity),
		OPT_PASSTHRU(0, "progress", &opt_పురోగతి, ఖాళీ,
			N_("force progress reporting"),
			PARSE_OPT_NOARG),
		OPT_CALLBACK_F(0, "recurse-submodules",
			   &recurse_submodules_cli, N_("on-demand"),
			   N_("control for recursive fetching of submodules"),
			   PARSE_OPT_OPTARG, option_fetch_parse_recurse_submodules),

		/* Options passed to git-merge or git-rebase */
		OPT_సమూహం(N_("Options related to merging")),
		OPT_CALLBACK_F('r', "rebase", &opt_rebase,
			"(false|true|merges|interactive)",
			N_("incorporate changes by rebasing rather than merging"),
			PARSE_OPT_OPTARG, parse_opt_rebase),
		OPT_PASSTHRU('n', ఖాళీ, &opt_diffstat, ఖాళీ,
			N_("do not show a diffstat at the end of the merge"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG),
		OPT_PASSTHRU(0, "stat", &opt_diffstat, ఖాళీ,
			N_("show a diffstat at the end of the merge"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "summary", &opt_diffstat, ఖాళీ,
			N_("(synonym to --stat)"),
			PARSE_OPT_NOARG | PARSE_OPT_HIDDEN),
		OPT_PASSTHRU(0, "compact-summary", &opt_diffstat, ఖాళీ,
			N_("show a compact-summary at the end of the merge"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "log", &opt_log, N_("n"),
			N_("add (at most <n>) entries from shortlog to merge commit message"),
			PARSE_OPT_OPTARG),
		OPT_PASSTHRU(0, "signoff", &opt_signoff, ఖాళీ,
			N_("add a Signed-off-by trailer"),
			PARSE_OPT_OPTARG),
		OPT_PASSTHRU(0, "squash", &opt_squash, ఖాళీ,
			N_("create a single commit instead of doing a merge"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "commit", &opt_commit, ఖాళీ,
			N_("perform a commit if the merge succeeds (default)"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "edit", &opt_edit, ఖాళీ,
			N_("edit message before committing"),
			PARSE_OPT_NOARG),
		OPT_CLEANUP(&cleanup_arg),
		OPT_PASSTHRU(0, "ff", &opt_ff, ఖాళీ,
			N_("allow fast-forward"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "ff-only", &opt_ff, ఖాళీ,
			N_("abort if fast-forward is not possible"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG),
		OPT_PASSTHRU(0, "verify", &opt_verify, ఖాళీ,
			N_("control use of pre-merge-commit and commit-msg hooks"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "verify-signatures", &opt_verify_signatures, ఖాళీ,
			N_("verify that the named commit has a valid GPG signature"),
			PARSE_OPT_NOARG),
		OPT_BOOL(0, "autostash", &opt_autostash,
			N_("automatically stash/stash pop before and after")),
		OPT_PASSTHRU_ARGV('s', "strategy", &opt_strategies, N_("strategy"),
			N_("merge strategy to use"),
			0),
		OPT_PASSTHRU_ARGV('X', "strategy-option", &opt_strategy_opts,
			N_("option=value"),
			N_("option for selected merge strategy"),
			0),
		OPT_PASSTHRU('S', "gpg-sign", &opt_gpg_sign, N_("key-id"),
			N_("GPG sign commit"),
			PARSE_OPT_OPTARG),
		OPT_సమితి_INT(0, "allow-unrelated-histories",
			    &opt_allow_unrelated_histories,
			    N_("allow merging unrelated histories"), 1),

		/* Options passed to git-fetch */
		OPT_సమూహం(N_("Options related to fetching")),
		OPT_PASSTHRU(0, "all", &opt_all, ఖాళీ,
			N_("fetch from all remotes"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU('a', "append", &opt_append, ఖాళీ,
			N_("append to .git/FETCH_HEAD instead of overwriting"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "upload-pack", &opt_upload_pack, N_("path"),
			N_("path to upload pack on remote end"),
			0),
		OPT__FORCE(&opt_force, N_("force overwrite of local branch"), 0),
		OPT_PASSTHRU('t', "tags", &opt_ట్యాగులు, ఖాళీ,
			N_("fetch all tags and associated objects"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU('p', "prune", &opt_prune, ఖాళీ,
			N_("prune remote-tracking branches no longer on remote"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU('j', "jobs", &max_children, N_("n"),
			N_("number of submodules pulled in parallel"),
			PARSE_OPT_OPTARG),
		OPT_BOOL(0, "dry-run", &opt_dry_నిరంతరం,
			N_("dry run")),
		OPT_PASSTHRU('k', "keep", &opt_keep, ఖాళీ,
			N_("keep downloaded pack"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "depth", &opt_లోతు, N_("depth"),
			N_("deepen history of shallow clone"),
			0),
		OPT_PASSTHRU_ARGV(0, "shallow-since", &opt_fetch, N_("time"),
			N_("deepen history of shallow repository based on time"),
			0),
		OPT_PASSTHRU_ARGV(0, "shallow-exclude", &opt_fetch, N_("ref"),
			N_("deepen history of shallow clone, excluding ref"),
			0),
		OPT_PASSTHRU_ARGV(0, "deepen", &opt_fetch, N_("n"),
			N_("deepen history of shallow clone"),
			0),
		OPT_PASSTHRU(0, "unshallow", &opt_unshallow, ఖాళీ,
			N_("convert to a complete repository"),
			PARSE_OPT_NONEG | PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "update-shallow", &opt_update_shallow, ఖాళీ,
			N_("accept refs that update .git/shallow"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "refmap", &opt_refmap, N_("refmap"),
			N_("specify fetch refmap"),
			PARSE_OPT_NONEG),
		OPT_PASSTHRU_ARGV('o', "server-option", &opt_fetch,
			N_("server-specific"),
			N_("option to transmit"),
			0),
		OPT_PASSTHRU('4',  "ipv4", &opt_ipv4, ఖాళీ,
			N_("use IPv4 addresses only"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU('6',  "ipv6", &opt_ipv6, ఖాళీ,
			N_("use IPv6 addresses only"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU_ARGV(0, "negotiation-restrict", &opt_fetch, N_("revision"),
			N_("report that we have only objects reachable from this object"),
			0),
		OPT_ALIAS(0, "negotiation-tip", "negotiation-restrict"),
		OPT_PASSTHRU_ARGV(0, "negotiation-include", &opt_fetch, N_("revision"),
			N_("ensure this ref is always sent as a negotiation have"),
			0),
		OPT_BOOL(0, "show-forced-updates", &opt_show_forced_updates,
			 N_("check for forced-updates on all updated branches")),
		OPT_PASSTHRU(0, "set-upstream", &సమితి_upstream, ఖాళీ,
			N_("set upstream for git pull/fetch"),
			PARSE_OPT_NOARG),

		OPT_చివర()
	};

	ఒకవేళ (!getenv("GIT_REFLOG_ACTION"))
		సమితి_reflog_సందేశం(argc, argv);

	repo_config(the_repository, git_pull_config, ఖాళీ);
	ఒకవేళ (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_నిండిన_సూచీ = 0;
	}

	argc = parse_options(argc, argv, ఉపసర్గ, pull_options, pull_usage, 0);
	ఒకవేళ (opt_autostash == -1)
		opt_autostash = config_pull_autostash;

	ఒకవేళ (recurse_submodules_cli != RECURSE_SUBMODULES_DEFAULT)
		recurse_submodules = recurse_submodules_cli;

	ఒకవేళ (cleanup_arg)
		/*
		 * this only checks the validity of cleanup_arg; we don't need
		 * a valid value for use_editor
		 */
		get_cleanup_బహుళకం(cleanup_arg, 0);

	parse_repo_refspecs(argc, argv, &repo, &refspecs);

	ఒకవేళ (!opt_ff) {
		opt_ff = xstrdup_or_null(config_get_ff());
		/*
		 * A subtle point: opt_ff was set on the line above via
		 * reading from config.  opt_rebase, in contrast, is set
		 * before this point via command line options.  The setting
		 * of opt_rebase via reading from config (using
		 * config_get_rebase()) does not happen until later.  We
		 * are relying on the next if-condition happening before
		 * the config_get_rebase() call so that an explicit
		 * "--rebase" can override a config setting of
		 * pull.ff=only.
		 */
		ఒకవేళ (opt_rebase >= 0 && opt_ff && !strcmp(opt_ff, "--ff-only")) {
			విడుదల(opt_ff);
			opt_ff = xstrdup("--ff");
		}
	}

	ఒకవేళ (opt_rebase < 0)
		opt_rebase = config_get_rebase(&rebase_unspecified);

	ఒకవేళ (repo_read_సూచీ_unmerged(the_repository))
		die_resolve_conflict("pull");

	ఒకవేళ (file_exists(git_మార్గం_విలీనం_తల(the_repository)))
		die_conclude_విలీనం();

	ఒకవేళ (repo_get_oid(the_repository, "HEAD", &orig_తల))
		oidclr(&orig_తల, the_repository->hash_algo);

	ఒకవేళ (opt_rebase) {
		ఒకవేళ (opt_autostash == -1)
			opt_autostash = config_rebase_autostash;

		ఒకవేళ (is_null_oid(&orig_తల) && !is_సూచీ_unborn(the_repository->సూచీ))
			die(_("Updating an unborn branch with changes added to the index."));

		ఒకవేళ (!opt_autostash)
			require_clean_work_వృక్షం(the_repository,
				N_("pull with rebase"),
				_("Please commit or stash them."), 1, 0);

		ఒకవేళ (get_rebase_fork_బిందువు(&rebase_fork_బిందువు, repo, *refspecs))
			oidclr(&rebase_fork_బిందువు, the_repository->hash_algo);
	}

	ఒకవేళ (నిరంతరం_fetch(repo, refspecs))
		తిరిగి 1;

	ఒకవేళ (opt_dry_నిరంతరం)
		తిరిగి 0;

	ఒకవేళ (repo_get_oid(the_repository, "HEAD", &curr_తల))
		oidclr(&curr_తల, the_repository->hash_algo);

	ఒకవేళ (!is_null_oid(&orig_తల) && !is_null_oid(&curr_తల) &&
			!oideq(&orig_తల, &curr_తల)) {
		/*
		 * The fetch involved updating the current branch.
		 *
		 * The working tree and the index file are still based on
		 * orig_head commit, but we are merging into curr_head.
		 * Update the working tree to match curr_head.
		 */

		warning(_("fetch updated the current branch head.\n"
			"fast-forwarding your working tree from\n"
			"commit %s."), oid_గమ్యం_hex(&orig_తల));

		ఒకవేళ (checkout_fast_forward(the_repository, &orig_తల,
					  &curr_తల, 0))
			die(_("Cannot fast-forward your working tree.\n"
				"After making sure that you saved anything precious from\n"
				"$ git diff %s\n"
				"output, run\n"
				"$ git reset --hard\n"
				"to recover."), oid_గమ్యం_hex(&orig_తల));
	}

	get_విలీనం_heads(&విలీనం_heads);

	ఒకవేళ (!విలీనం_heads.nr)
		die_no_విలీనం_candidates(repo, refspecs);

	ఒకవేళ (is_null_oid(&orig_తల)) {
		ఒకవేళ (విలీనం_heads.nr > 1)
			die(_("Cannot merge multiple branches into empty head."));
		ret = pull_into_void(విలీనం_heads.oid, &curr_తల);
		వెళ్ళు cleanup;
	}
	ఒకవేళ (విలీనం_heads.nr > 1) {
		ఒకవేళ (opt_rebase)
			die(_("Cannot rebase onto multiple branches."));
		ఒకవేళ (opt_ff && !strcmp(opt_ff, "--ff-only"))
			die(_("Cannot fast-forward to multiple branches."));
	}

	can_ff = get_can_ff(&orig_తల, &విలీనం_heads);
	divergent = !can_ff && !already_పైకి_గమ్యం_date(&orig_తల, &విలీనం_heads);

	/* ff-only takes precedence over rebase */
	ఒకవేళ (opt_ff && !strcmp(opt_ff, "--ff-only")) {
		ఒకవేళ (divergent)
			die_ff_impossible();
		opt_rebase = REBASE_FALSE;
	}
	/* If no action specified and we can't fast forward, then warn. */
	ఒకవేళ (!opt_ff && rebase_unspecified && divergent) {
		show_advice_pull_non_ff();
		die(_("Need to specify how to reconcile divergent branches."));
	}

	ఒకవేళ (opt_rebase) {
		నిర్మాణం object_id newbase;
		నిర్మాణం object_id upstream;
		get_rebase_newbase_and_upstream(&newbase, &upstream, &curr_తల,
						విలీనం_heads.oid, &rebase_fork_బిందువు);

		ఒకవేళ ((recurse_submodules == RECURSE_SUBMODULES_ON ||
		     recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND) &&
		    submodule_touches_లోపల_range(the_repository, &upstream, &curr_తల))
			die(_("cannot rebase with locally recorded submodule modifications"));

		ఒకవేళ (can_ff) {
			/* we can fast-forward this without invoking rebase */
			విడుదల(opt_ff);
			opt_ff = xstrdup("--ff-only");
			ret = నిరంతరం_విలీనం();
		} లేకపోతే {
			ret = నిరంతరం_rebase(&newbase, &upstream);
		}

		ఒకవేళ (!ret && (recurse_submodules == RECURSE_SUBMODULES_ON ||
			     recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND))
			ret = rebase_submodules();

		వెళ్ళు cleanup;
	} లేకపోతే {
		ret = నిరంతరం_విలీనం();
		ఒకవేళ (!ret && (recurse_submodules == RECURSE_SUBMODULES_ON ||
			     recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND))
			ret = update_submodules();
		వెళ్ళు cleanup;
	}

cleanup:
	oid_array_clear(&విలీనం_heads);
	తిరిగి ret;
}
