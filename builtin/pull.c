/*
 * Builtin "git pull"
 *
 * Based on git-pull.sh by Junio C Hamano
 *
 * Fetch one or more remote refs and merge it/them into the current HEAD.
 */

#define USE_THE_REPOSITORY_VARIABLE

#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "advice.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "hex.h"
#অন্তর্ভাব "merge.h"
#অন্তর্ভাব "object-name.h"
#অন্তর্ভাব "parse-options.h"
#অন্তর্ভাব "run-command.h"
#অন্তর্ভাব "oid-array.h"
#অন্তর্ভাব "remote.h"
#অন্তর্ভাব "dir.h"
#অন্তর্ভাব "path.h"
#অন্তর্ভাব "read-cache-ll.h"
#অন্তর্ভাব "rebase.h"
#অন্তর্ভাব "refs.h"
#অন্তর্ভাব "refspec.h"
#অন্তর্ভাব "submodule.h"
#অন্তর্ভাব "submodule-config.h"
#অন্তর্ভাব "wt-status.h"
#অন্তর্ভাব "commit-reach.h"
#অন্তর্ভাব "sequencer.h"

/**
 * Parses the value of --rebase. If value is a false value, returns
 * REBASE_FALSE. If value is a true value, returns REBASE_TRUE. If value is
 * "merges", returns REBASE_MERGES. If value is a invalid value, dies with
 * a fatal error if fatal is true, otherwise returns REBASE_INVALID.
 */
স্থির গণনা rebase_type parse_config_rebase(ধ্রুবক অক্ষর *চাবি, ধ্রুবক অক্ষর *মান,
		পূর্ণসংখ্যা fatal)
{
	গণনা rebase_type v = rebase_parse_মান(মান);
	যদি (v != REBASE_INVALID)
		ফেরত v;

	যদি (fatal)
		die(_("invalid value for '%s': '%s'"), চাবি, মান);
	অন্যথা
		error(_("invalid value for '%s': '%s'"), চাবি, মান);

	ফেরত REBASE_INVALID;
}

/**
 * Callback for --rebase, which parses arg with parse_config_rebase().
 */
স্থির পূর্ণসংখ্যা parse_opt_rebase(ধ্রুবক কাঠামো option *opt, ধ্রুবক অক্ষর *arg, পূর্ণসংখ্যা unset)
{
	গণনা rebase_type *মান = opt->মান;

	যদি (arg)
		*মান = parse_config_rebase("--rebase", arg, 0);
	অন্যথা
		*মান = unset ? REBASE_FALSE : REBASE_TRUE;
	ফেরত *মান == REBASE_INVALID ? -1 : 0;
}

স্থির ধ্রুবক অক্ষর * ধ্রুবক pull_usage[] = {
	N_("git pull [<options>] [<repository> [<refspec>...]]"),
	ফাঁকা
};

/* Shared options */
স্থির পূর্ণসংখ্যা opt_verbosity;
স্থির ধ্রুবক অক্ষর *opt_progress;
স্থির পূর্ণসংখ্যা recurse_submodules = RECURSE_SUBMODULES_DEFAULT;
স্থির পূর্ণসংখ্যা recurse_submodules_cli = RECURSE_SUBMODULES_DEFAULT;

/* Options passed to git-merge or git-rebase */
স্থির গণনা rebase_type opt_rebase = -1;
স্থির ধ্রুবক অক্ষর *opt_diffstat;
স্থির ধ্রুবক অক্ষর *opt_log;
স্থির ধ্রুবক অক্ষর *opt_signoff;
স্থির ধ্রুবক অক্ষর *opt_squash;
স্থির ধ্রুবক অক্ষর *opt_commit;
স্থির ধ্রুবক অক্ষর *opt_edit;
স্থির ধ্রুবক অক্ষর *cleanup_arg;
স্থির অক্ষর *opt_ff;
স্থির ধ্রুবক অক্ষর *opt_verify_signatures;
স্থির ধ্রুবক অক্ষর *opt_verify;
স্থির পূর্ণসংখ্যা opt_autostash = -1;
স্থির পূর্ণসংখ্যা config_rebase_autostash;
স্থির পূর্ণসংখ্যা config_pull_autostash = -1;
স্থির পূর্ণসংখ্যা check_trust_level = 1;
স্থির কাঠামো strvec opt_strategies = STRVEC_INIT;
স্থির কাঠামো strvec opt_strategy_opts = STRVEC_INIT;
স্থির ধ্রুবক অক্ষর *opt_gpg_sign;
স্থির পূর্ণসংখ্যা opt_allow_unrelated_histories;

/* Options passed to git-fetch */
স্থির ধ্রুবক অক্ষর *opt_all;
স্থির ধ্রুবক অক্ষর *opt_append;
স্থির ধ্রুবক অক্ষর *opt_upload_pack;
স্থির পূর্ণসংখ্যা opt_force;
স্থির ধ্রুবক অক্ষর *opt_tags;
স্থির ধ্রুবক অক্ষর *opt_prune;
স্থির ধ্রুবক অক্ষর *max_children;
স্থির পূর্ণসংখ্যা opt_dry_run;
স্থির ধ্রুবক অক্ষর *opt_keep;
স্থির ধ্রুবক অক্ষর *opt_depth;
স্থির ধ্রুবক অক্ষর *opt_unshallow;
স্থির ধ্রুবক অক্ষর *opt_update_shallow;
স্থির ধ্রুবক অক্ষর *opt_refmap;
স্থির ধ্রুবক অক্ষর *opt_ipv4;
স্থির ধ্রুবক অক্ষর *opt_ipv6;
স্থির পূর্ণসংখ্যা opt_show_forced_updates = -1;
স্থির ধ্রুবক অক্ষর *set_upstream;
স্থির কাঠামো strvec opt_fetch = STRVEC_INIT;

/**
 * Pushes "-q" or "-v" switches into arr to match the opt_verbosity level.
 */
স্থির শূন্য argv_ঠেলো_verbosity(কাঠামো strvec *arr)
{
	পূর্ণসংখ্যা verbosity;

	জন্য (verbosity = opt_verbosity; verbosity > 0; verbosity--)
		strvec_ঠেলো(arr, "-v");

	জন্য (verbosity = opt_verbosity; verbosity < 0; verbosity++)
		strvec_ঠেলো(arr, "-q");
}

/**
 * Pushes "-f" switches into arr to match the opt_force level.
 */
স্থির শূন্য argv_ঠেলো_force(কাঠামো strvec *arr)
{
	পূর্ণসংখ্যা force = opt_force;
	যখন (force-- > 0)
		strvec_ঠেলো(arr, "-f");
}

/**
 * Sets the GIT_REFLOG_ACTION environment variable to the concatenation of argv
 */
স্থির শূন্য set_reflog_message(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv)
{
	পূর্ণসংখ্যা i;
	কাঠামো strbuf বার্তা = STRBUF_INIT;

	জন্য (i = 0; i < argc; i++) {
		যদি (i)
			strbuf_addch(&বার্তা, ' ');
		strbuf_addstr(&বার্তা, argv[i]);
	}

	setenv("GIT_REFLOG_ACTION", বার্তা.buf, 0);

	strbuf_release(&বার্তা);
}

/**
 * If pull.ff is unset, returns NULL. If pull.ff is "true", returns "--ff". If
 * pull.ff is "false", returns "--no-ff". If pull.ff is "only", returns
 * "--ff-only". Otherwise, if pull.ff is set to an invalid value, die with an
 * error.
 */
স্থির ধ্রুবক অক্ষর *config_get_ff(শূন্য)
{
	ধ্রুবক অক্ষর *মান;

	যদি (repo_config_get_মান(the_repository, "pull.ff", &মান))
		ফেরত ফাঁকা;

	নির্বাচন (git_parse_maybe_bool(মান)) {
	ক্ষেত্র 0:
		ফেরত "--no-ff";
	ক্ষেত্র 1:
		ফেরত "--ff";
	}

	যদি (!strcmp(মান, "only"))
		ফেরত "--ff-only";

	die(_("invalid value for '%s': '%s'"), "pull.ff", মান);
}

/**
 * Returns the default configured value for --rebase. It first looks for the
 * value of "branch.$curr_branch.rebase", where $curr_branch is the current
 * branch, and if HEAD is detached or the configuration key does not exist,
 * looks for the value of "pull.rebase". If both configuration keys do not
 * exist, returns REBASE_FALSE.
 */
স্থির গণনা rebase_type config_get_rebase(পূর্ণসংখ্যা *rebase_unspecified)
{
	কাঠামো branch *curr_branch = branch_get("HEAD");
	ধ্রুবক অক্ষর *মান;

	যদি (curr_branch) {
		অক্ষর *চাবি = xstrfmt("branch.%s.rebase", curr_branch->name);

		যদি (!repo_config_get_মান(the_repository, চাবি, &মান)) {
			গণনা rebase_type ret = parse_config_rebase(চাবি, মান, 1);
			মুক্ত(চাবি);
			ফেরত ret;
		}

		মুক্ত(চাবি);
	}

	যদি (!repo_config_get_মান(the_repository, "pull.rebase", &মান))
		ফেরত parse_config_rebase("pull.rebase", মান, 1);

	*rebase_unspecified = 1;

	ফেরত REBASE_FALSE;
}

/**
 * Read config variables.
 */
স্থির পূর্ণসংখ্যা git_pull_config(ধ্রুবক অক্ষর *var, ধ্রুবক অক্ষর *মান,
			   ধ্রুবক কাঠামো config_context *ctx, শূন্য *cb)
{
	যদি (!strcmp(var, "rebase.autostash")) {
		/*
		 * run_rebase() also reads this option. The reason we handle it here is
		 * that when pull.rebase is true, a fast-forward may occur without
		 * invoking run_rebase(). We need to ensure that autostash is set even
		 * in the fast-forward case.
		 *
		 * run_merge() handles merge.autostash, so we don't handle it here.
		 */
		config_rebase_autostash = git_config_bool(var, মান);
		ফেরত 0;
	} অন্যথা যদি (!strcmp(var, "pull.autostash")) {
		config_pull_autostash = git_config_bool(var, মান);
		ফেরত 0;
	} অন্যথা যদি (!strcmp(var, "submodule.recurse")) {
		recurse_submodules = git_config_bool(var, মান) ?
			RECURSE_SUBMODULES_ON : RECURSE_SUBMODULES_OFF;
		ফেরত 0;
	} অন্যথা যদি (!strcmp(var, "gpg.mintrustlevel")) {
		check_trust_level = 0;
	}

	ফেরত git_default_config(var, মান, ctx, cb);
}

/**
 * Appends merge candidates from FETCH_HEAD that are not marked not-for-merge
 * into merge_heads.
 */
স্থির শূন্য get_merge_heads(কাঠামো oid_array *merge_heads)
{
	ধ্রুবক অক্ষর *filename = git_path_fetch_শীর্ষ(the_repository);
	FILE *fp;
	কাঠামো strbuf sb = STRBUF_INIT;
	কাঠামো object_id oid;

	fp = xfopen(filename, "r");
	যখন (strbuf_getline_lf(&sb, fp) != EOF) {
		ধ্রুবক অক্ষর *p;
		যদি (parse_oid_hex(sb.buf, &oid, &p))
			চালু;  /* invalid line: does not start with object ID */
		যদি (starts_with(p, "\tnot-for-merge\t"))
			চালু;  /* ref is not-for-merge */
		oid_array_append(merge_heads, &oid);
	}
	fclose(fp);
	strbuf_release(&sb);
}

/**
 * Used by die_no_merge_candidates() as a for_each_remote() callback to
 * retrieve the name of the remote if the repository only has one remote.
 */
স্থির পূর্ণসংখ্যা get_only_remote(কাঠামো remote *remote, শূন্য *cb_তথ্য)
{
	ধ্রুবক অক্ষর **remote_name = cb_তথ্য;

	যদি (*remote_name)
		ফেরত -1;

	*remote_name = remote->name;
	ফেরত 0;
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
স্থির শূন্য NORETURN die_no_merge_candidates(ধ্রুবক অক্ষর *repo, ধ্রুবক অক্ষর **refspecs)
{
	কাঠামো branch *curr_branch = branch_get("HEAD");
	ধ্রুবক অক্ষর *remote = curr_branch ? curr_branch->remote_name : ফাঁকা;

	যদি (*refspecs) {
		যদি (opt_rebase)
			fprintf_ln(stderr, _("There is no candidate for rebasing against among the refs that you just fetched."));
		অন্যথা
			fprintf_ln(stderr, _("There are no candidates for merging among the refs that you just fetched."));
		fprintf_ln(stderr, _("Generally this means that you provided a wildcard refspec which had no\n"
					"matches on the remote end."));
	} অন্যথা যদি (repo && curr_branch && (!remote || strcmp(repo, remote))) {
		fprintf_ln(stderr, _("You asked to pull from the remote '%s', but did not specify\n"
			"a branch. Because this is not the default configured remote\n"
			"for your current branch, you must specify a branch on the command line."),
			repo);
	} অন্যথা যদি (!curr_branch) {
		fprintf_ln(stderr, _("You are not currently on a branch."));
		যদি (opt_rebase)
			fprintf_ln(stderr, _("Please specify which branch you want to rebase against."));
		অন্যথা
			fprintf_ln(stderr, _("Please specify which branch you want to merge with."));
		fprintf_ln(stderr, _("See git-pull(1) for details."));
		fprintf(stderr, "\n");
		fprintf_ln(stderr, "    git pull %s %s", _("<remote>"), _("<branch>"));
		fprintf(stderr, "\n");
	} অন্যথা যদি (!curr_branch->merge_nr) {
		ধ্রুবক অক্ষর *remote_name = ফাঁকা;

		যদি (for_each_remote(get_only_remote, &remote_name) || !remote_name)
			remote_name = _("<remote>");

		fprintf_ln(stderr, _("There is no tracking information for the current branch."));
		যদি (opt_rebase)
			fprintf_ln(stderr, _("Please specify which branch you want to rebase against."));
		অন্যথা
			fprintf_ln(stderr, _("Please specify which branch you want to merge with."));
		fprintf_ln(stderr, _("See git-pull(1) for details."));
		fprintf(stderr, "\n");
		fprintf_ln(stderr, "    git pull %s %s", _("<remote>"), _("<branch>"));
		fprintf(stderr, "\n");
		fprintf_ln(stderr, _("If you wish to set tracking information for this branch you can do so with:"));
		fprintf(stderr, "\n");
		fprintf_ln(stderr, "    git branch --set-upstream-to=%s/%s %s\n",
				remote_name, _("<branch>"), curr_branch->name);
	} অন্যথা
		fprintf_ln(stderr, _("Your configuration specifies to merge with the ref '%s'\n"
			"from the remote, but no such ref was fetched."),
			curr_branch->merge[0]->src);
	exit(1);
}

/**
 * Parses argv into [<repo> [<refspecs>...]], returning their values in `repo`
 * as a string and `refspecs` as a null-terminated array of strings. If `repo`
 * is not provided in argv, it is set to NULL.
 */
স্থির শূন্য parse_repo_refspecs(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর **repo,
		ধ্রুবক অক্ষর ***refspecs)
{
	যদি (argc > 0) {
		*repo = *argv++;
		argc--;
	} অন্যথা
		*repo = ফাঁকা;
	*refspecs = argv;
}

/**
 * Runs git-fetch, returning its exit status. `repo` and `refspecs` are the
 * repository and refspecs to fetch, or NULL if they are not provided.
 */
স্থির পূর্ণসংখ্যা run_fetch(ধ্রুবক অক্ষর *repo, ধ্রুবক অক্ষর **refspecs)
{
	কাঠামো child_process cmd = CHILD_PROCESS_INIT;

	strvec_pushl(&cmd.args, "fetch", "--update-head-ok", ফাঁকা);

	/* Shared options */
	argv_ঠেলো_verbosity(&cmd.args);
	যদি (opt_progress)
		strvec_ঠেলো(&cmd.args, opt_progress);

	/* Options passed to git-fetch */
	যদি (opt_all)
		strvec_ঠেলো(&cmd.args, opt_all);
	যদি (opt_append)
		strvec_ঠেলো(&cmd.args, opt_append);
	যদি (opt_upload_pack)
		strvec_ঠেলো(&cmd.args, opt_upload_pack);
	argv_ঠেলো_force(&cmd.args);
	যদি (opt_tags)
		strvec_ঠেলো(&cmd.args, opt_tags);
	যদি (opt_prune)
		strvec_ঠেলো(&cmd.args, opt_prune);
	যদি (recurse_submodules_cli != RECURSE_SUBMODULES_DEFAULT)
		নির্বাচন (recurse_submodules_cli) {
		ক্ষেত্র RECURSE_SUBMODULES_ON:
			strvec_ঠেলো(&cmd.args, "--recurse-submodules=on");
			বিরতি;
		ক্ষেত্র RECURSE_SUBMODULES_OFF:
			strvec_ঠেলো(&cmd.args, "--recurse-submodules=no");
			বিরতি;
		ক্ষেত্র RECURSE_SUBMODULES_ON_DEMAND:
			strvec_ঠেলো(&cmd.args, "--recurse-submodules=on-demand");
			বিরতি;
		পূর্বনির্ধারিত:
			BUG("submodule recursion option not understood");
		}
	যদি (max_children)
		strvec_ঠেলো(&cmd.args, max_children);
	যদি (opt_dry_run)
		strvec_ঠেলো(&cmd.args, "--dry-run");
	যদি (opt_keep)
		strvec_ঠেলো(&cmd.args, opt_keep);
	যদি (opt_depth)
		strvec_ঠেলো(&cmd.args, opt_depth);
	যদি (opt_unshallow)
		strvec_ঠেলো(&cmd.args, opt_unshallow);
	যদি (opt_update_shallow)
		strvec_ঠেলো(&cmd.args, opt_update_shallow);
	যদি (opt_refmap)
		strvec_ঠেলো(&cmd.args, opt_refmap);
	যদি (opt_ipv4)
		strvec_ঠেলো(&cmd.args, opt_ipv4);
	যদি (opt_ipv6)
		strvec_ঠেলো(&cmd.args, opt_ipv6);
	যদি (opt_show_forced_updates > 0)
		strvec_ঠেলো(&cmd.args, "--show-forced-updates");
	অন্যথা যদি (opt_show_forced_updates == 0)
		strvec_ঠেলো(&cmd.args, "--no-show-forced-updates");
	যদি (set_upstream)
		strvec_ঠেলো(&cmd.args, set_upstream);
	strvec_pushv(&cmd.args, opt_fetch.v);

	যদি (repo) {
		strvec_ঠেলো(&cmd.args, repo);
		strvec_pushv(&cmd.args, refspecs);
	} অন্যথা যদি (*refspecs)
		BUG("refspecs without repo?");
	cmd.git_cmd = 1;
	cmd.odb_to_close = the_repository->objects;
	ফেরত run_command(&cmd);
}

/**
 * "Pulls into void" by branching off merge_head.
 */
স্থির পূর্ণসংখ্যা pull_into_void(ধ্রুবক কাঠামো object_id *merge_শীর্ষ,
		ধ্রুবক কাঠামো object_id *curr_শীর্ষ)
{
	যদি (opt_verify_signatures) {
		কাঠামো commit *commit;

		commit = lookup_commit(the_repository, merge_শীর্ষ);
		যদি (!commit)
			die(_("unable to access commit %s"),
			    oid_to_hex(merge_শীর্ষ));

		verify_merge_signature(commit, opt_verbosity,
				       check_trust_level);
	}

	/*
	 * Two-way merge: we treat the index as based on an empty tree,
	 * and try to fast-forward to HEAD. This ensures we will not lose
	 * index/worktree changes that the user already made on the unborn
	 * branch.
	 */
	যদি (checkout_fast_forward(the_repository,
				  the_hash_algo->empty_বৃক্ষ,
				  merge_শীর্ষ, 0))
		ফেরত 1;

	যদি (refs_update_ref(get_main_ref_store(the_repository), "initial pull", "HEAD", merge_শীর্ষ, curr_শীর্ষ, 0, UPDATE_REFS_DIE_ON_ERR))
		ফেরত 1;

	ফেরত 0;
}

স্থির পূর্ণসংখ্যা rebase_submodules(শূন্য)
{
	কাঠামো child_process cp = CHILD_PROCESS_INIT;

	cp.git_cmd = 1;
	cp.no_stdin = 1;
	strvec_pushl(&cp.args, "submodule", "update",
		     "--recursive", "--rebase", ফাঁকা);
	argv_ঠেলো_verbosity(&cp.args);

	ফেরত run_command(&cp);
}

স্থির পূর্ণসংখ্যা update_submodules(শূন্য)
{
	কাঠামো child_process cp = CHILD_PROCESS_INIT;

	cp.git_cmd = 1;
	cp.no_stdin = 1;
	strvec_pushl(&cp.args, "submodule", "update",
		     "--recursive", "--checkout", ফাঁকা);
	argv_ঠেলো_verbosity(&cp.args);

	ফেরত run_command(&cp);
}

/**
 * Runs git-merge, returning its exit status.
 */
স্থির পূর্ণসংখ্যা run_merge(শূন্য)
{
	কাঠামো child_process cmd = CHILD_PROCESS_INIT;

	strvec_pushl(&cmd.args, "merge", ফাঁকা);

	/* Shared options */
	argv_ঠেলো_verbosity(&cmd.args);
	যদি (opt_progress)
		strvec_ঠেলো(&cmd.args, opt_progress);

	/* Options passed to git-merge */
	যদি (opt_diffstat)
		strvec_ঠেলো(&cmd.args, opt_diffstat);
	যদি (opt_log)
		strvec_ঠেলো(&cmd.args, opt_log);
	যদি (opt_signoff)
		strvec_ঠেলো(&cmd.args, opt_signoff);
	যদি (opt_squash)
		strvec_ঠেলো(&cmd.args, opt_squash);
	যদি (opt_commit)
		strvec_ঠেলো(&cmd.args, opt_commit);
	যদি (opt_edit)
		strvec_ঠেলো(&cmd.args, opt_edit);
	যদি (cleanup_arg)
		strvec_pushf(&cmd.args, "--cleanup=%s", cleanup_arg);
	যদি (opt_ff)
		strvec_ঠেলো(&cmd.args, opt_ff);
	যদি (opt_verify)
		strvec_ঠেলো(&cmd.args, opt_verify);
	যদি (opt_verify_signatures)
		strvec_ঠেলো(&cmd.args, opt_verify_signatures);
	strvec_pushv(&cmd.args, opt_strategies.v);
	strvec_pushv(&cmd.args, opt_strategy_opts.v);
	যদি (opt_gpg_sign)
		strvec_ঠেলো(&cmd.args, opt_gpg_sign);
	যদি (opt_autostash == 0)
		strvec_ঠেলো(&cmd.args, "--no-autostash");
	অন্যথা যদি (opt_autostash == 1)
		strvec_ঠেলো(&cmd.args, "--autostash");
	যদি (opt_allow_unrelated_histories > 0)
		strvec_ঠেলো(&cmd.args, "--allow-unrelated-histories");

	strvec_ঠেলো(&cmd.args, "FETCH_HEAD");
	cmd.git_cmd = 1;
	ফেরত run_command(&cmd);
}

/**
 * Returns remote's upstream branch for the current branch. If remote is NULL,
 * the current branch's configured default remote is used. Returns NULL if
 * `remote` does not name a valid remote, HEAD does not point to a branch,
 * remote is not the branch's configured remote or the branch does not have any
 * configured upstream branch.
 */
স্থির ধ্রুবক অক্ষর *get_upstream_branch(ধ্রুবক অক্ষর *remote)
{
	কাঠামো remote *rm;
	কাঠামো branch *curr_branch;
	ধ্রুবক অক্ষর *curr_branch_remote;

	rm = remote_get(remote);
	যদি (!rm)
		ফেরত ফাঁকা;

	curr_branch = branch_get("HEAD");
	যদি (!curr_branch)
		ফেরত ফাঁকা;

	curr_branch_remote = remote_for_branch(curr_branch, ফাঁকা);
	প্রতিজ্ঞা(curr_branch_remote);

	যদি (strcmp(curr_branch_remote, rm->name))
		ফেরত ফাঁকা;

	ফেরত branch_get_upstream(curr_branch, ফাঁকা);
}

/**
 * Derives the remote-tracking branch from the remote and refspec.
 *
 * FIXME: The current implementation assumes the default mapping of
 * refs/heads/<branch_name> to refs/remotes/<remote_name>/<branch_name>.
 */
স্থির ধ্রুবক অক্ষর *get_tracking_branch(ধ্রুবক অক্ষর *remote, ধ্রুবক অক্ষর *refspec)
{
	কাঠামো refspec_item spec;
	ধ্রুবক অক্ষর *spec_src;
	ধ্রুবক অক্ষর *merge_branch;

	যদি (!refspec_item_init_fetch(&spec, refspec, the_hash_algo))
		die(_("invalid refspec '%s'"), refspec);
	spec_src = spec.src;
	যদি (!*spec_src || !strcmp(spec_src, "HEAD"))
		spec_src = "HEAD";
	অন্যথা যদি (skip_prefix(spec_src, "heads/", &spec_src))
		;
	অন্যথা যদি (skip_prefix(spec_src, "refs/heads/", &spec_src))
		;
	অন্যথা যদি (starts_with(spec_src, "refs/") ||
		starts_with(spec_src, "tags/") ||
		starts_with(spec_src, "remotes/"))
		spec_src = "";

	যদি (*spec_src) {
		যদি (!strcmp(remote, "."))
			merge_branch = mkpath("refs/heads/%s", spec_src);
		অন্যথা
			merge_branch = mkpath("refs/remotes/%s/%s", remote, spec_src);
	} অন্যথা
		merge_branch = ফাঁকা;

	refspec_item_clear(&spec);
	ফেরত merge_branch;
}

/**
 * Given the repo and refspecs, sets fork_point to the point at which the
 * current branch forked from its remote-tracking branch. Returns 0 on success,
 * -1 on failure.
 */
স্থির পূর্ণসংখ্যা get_rebase_fork_point(কাঠামো object_id *fork_point, ধ্রুবক অক্ষর *repo,
		ধ্রুবক অক্ষর *refspec)
{
	পূর্ণসংখ্যা ret;
	কাঠামো branch *curr_branch;
	ধ্রুবক অক্ষর *remote_branch;
	কাঠামো child_process cp = CHILD_PROCESS_INIT;
	কাঠামো strbuf sb = STRBUF_INIT;

	curr_branch = branch_get("HEAD");
	যদি (!curr_branch)
		ফেরত -1;

	যদি (refspec)
		remote_branch = get_tracking_branch(repo, refspec);
	অন্যথা
		remote_branch = get_upstream_branch(repo);

	যদি (!remote_branch)
		ফেরত -1;

	strvec_pushl(&cp.args, "merge-base", "--fork-point",
		     remote_branch, curr_branch->name, ফাঁকা);
	cp.no_stdin = 1;
	cp.no_stderr = 1;
	cp.git_cmd = 1;

	ret = capture_command(&cp, &sb, GIT_MAX_HEXSZ);
	যদি (ret)
		যাও cleanup;

	ret = get_oid_hex(sb.buf, fork_point);
	যদি (ret)
		যাও cleanup;

cleanup:
	strbuf_release(&sb);
	ফেরত ret ? -1 : 0;
}

/**
 * Sets merge_base to the octopus merge base of curr_head, merge_head and
 * fork_point. Returns 0 if a merge base is found, 1 otherwise.
 */
স্থির পূর্ণসংখ্যা get_octopus_merge_base(কাঠামো object_id *merge_base,
		ধ্রুবক কাঠামো object_id *curr_শীর্ষ,
		ধ্রুবক কাঠামো object_id *merge_শীর্ষ,
		ধ্রুবক কাঠামো object_id *fork_point)
{
	কাঠামো commit_list *revs = ফাঁকা, *ফলাফল = ফাঁকা;

	commit_list_ঢোকাও(lookup_commit_reference(the_repository, curr_শীর্ষ),
			   &revs);
	commit_list_ঢোকাও(lookup_commit_reference(the_repository, merge_শীর্ষ),
			   &revs);
	যদি (!is_null_oid(fork_point))
		commit_list_ঢোকাও(lookup_commit_reference(the_repository, fork_point),
				   &revs);

	যদি (get_octopus_merge_bases(revs, &ফলাফল) < 0)
		exit(128);
	commit_list_free(revs);
	reduce_heads_replace(&ফলাফল);

	যদি (!ফলাফল)
		ফেরত 1;

	oidcpy(merge_base, &ফলাফল->item->object.oid);
	commit_list_free(ফলাফল);
	ফেরত 0;
}

/**
 * Given the current HEAD oid, the merge head returned from git-fetch and the
 * fork point calculated by get_rebase_fork_point(), compute the <newbase> and
 * <upstream> arguments to use for the upcoming git-rebase invocation.
 */
স্থির পূর্ণসংখ্যা get_rebase_newbase_and_upstream(কাঠামো object_id *newbase,
		কাঠামো object_id *upstream,
		ধ্রুবক কাঠামো object_id *curr_শীর্ষ,
		ধ্রুবক কাঠামো object_id *merge_শীর্ষ,
		ধ্রুবক কাঠামো object_id *fork_point)
{
	কাঠামো object_id oct_merge_base;

	যদি (!get_octopus_merge_base(&oct_merge_base, curr_শীর্ষ, merge_শীর্ষ, fork_point))
		যদি (!is_null_oid(fork_point) && oideq(&oct_merge_base, fork_point))
			fork_point = ফাঁকা;

	যদি (fork_point && !is_null_oid(fork_point))
		oidcpy(upstream, fork_point);
	অন্যথা
		oidcpy(upstream, merge_শীর্ষ);

	oidcpy(newbase, merge_শীর্ষ);

	ফেরত 0;
}

/**
 * Given the <newbase> and <upstream> calculated by
 * get_rebase_newbase_and_upstream(), runs git-rebase with the
 * appropriate arguments and returns its exit status.
 */
স্থির পূর্ণসংখ্যা run_rebase(ধ্রুবক কাঠামো object_id *newbase,
		ধ্রুবক কাঠামো object_id *upstream)
{
	কাঠামো child_process cmd = CHILD_PROCESS_INIT;

	strvec_ঠেলো(&cmd.args, "rebase");

	/* Shared options */
	argv_ঠেলো_verbosity(&cmd.args);

	/* Options passed to git-rebase */
	যদি (opt_rebase == REBASE_MERGES)
		strvec_ঠেলো(&cmd.args, "--rebase-merges");
	অন্যথা যদি (opt_rebase == REBASE_INTERACTIVE)
		strvec_ঠেলো(&cmd.args, "--interactive");
	যদি (opt_diffstat)
		strvec_ঠেলো(&cmd.args, opt_diffstat);
	strvec_pushv(&cmd.args, opt_strategies.v);
	strvec_pushv(&cmd.args, opt_strategy_opts.v);
	যদি (opt_gpg_sign)
		strvec_ঠেলো(&cmd.args, opt_gpg_sign);
	যদি (opt_signoff)
		strvec_ঠেলো(&cmd.args, opt_signoff);
	যদি (opt_autostash == 0)
		strvec_ঠেলো(&cmd.args, "--no-autostash");
	অন্যথা যদি (opt_autostash == 1)
		strvec_ঠেলো(&cmd.args, "--autostash");
	যদি (opt_verify_signatures &&
	    !strcmp(opt_verify_signatures, "--verify-signatures"))
		warning(_("ignoring --verify-signatures for rebase"));

	strvec_ঠেলো(&cmd.args, "--onto");
	strvec_ঠেলো(&cmd.args, oid_to_hex(newbase));

	strvec_ঠেলো(&cmd.args, oid_to_hex(upstream));

	cmd.git_cmd = 1;
	ফেরত run_command(&cmd);
}

স্থির পূর্ণসংখ্যা get_can_ff(কাঠামো object_id *orig_শীর্ষ,
		      কাঠামো oid_array *merge_heads)
{
	পূর্ণসংখ্যা ret;
	কাঠামো commit_list *list = ফাঁকা;
	কাঠামো commit *merge_শীর্ষ, *শীর্ষ;
	কাঠামো object_id *orig_merge_শীর্ষ;

	যদি (merge_heads->nr > 1)
		ফেরত 0;

	orig_merge_শীর্ষ = &merge_heads->oid[0];
	শীর্ষ = lookup_commit_reference(the_repository, orig_শীর্ষ);
	যদি (!শীর্ষ)
		ফেরত 0;
	merge_শীর্ষ = lookup_commit_reference(the_repository, orig_merge_শীর্ষ);
	যদি (!merge_শীর্ষ)
		ফেরত 0;
	commit_list_ঢোকাও(শীর্ষ, &list);
	ret = repo_is_descendant_of(the_repository, merge_শীর্ষ, list);
	commit_list_free(list);
	যদি (ret < 0)
		exit(128);
	ফেরত ret;
}

/*
 * Is orig_head a descendant of _all_ merge_heads?
 * Unfortunately is_descendant_of() cannot be used as it asks
 * if orig_head is a descendant of at least one of them.
 */
স্থির পূর্ণসংখ্যা already_up_to_date(কাঠামো object_id *orig_শীর্ষ,
			      কাঠামো oid_array *merge_heads)
{
	কাঠামো commit *ours;

	ours = lookup_commit_reference(the_repository, orig_শীর্ষ);
	যদি (!ours)
		ফেরত 0;
	জন্য (আকার_t i = 0; i < merge_heads->nr; i++) {
		কাঠামো commit_list *list = ফাঁকা;
		কাঠামো commit *theirs;
		পূর্ণসংখ্যা ok;

		theirs = lookup_commit_reference(the_repository, &merge_heads->oid[i]);
		যদি (!theirs)
			ফেরত 0;
		commit_list_ঢোকাও(theirs, &list);
		ok = repo_is_descendant_of(the_repository, ours, list);
		commit_list_free(list);
		যদি (ok < 0)
			exit(128);
		যদি (!ok)
			ফেরত 0;
	}
	ফেরত 1;
}

স্থির শূন্য show_advice_pull_non_ff(শূন্য)
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

পূর্ণসংখ্যা cmd_pull(পূর্ণসংখ্যা argc,
	     ধ্রুবক অক্ষর **argv,
	     ধ্রুবক অক্ষর *prefix,
	     কাঠামো repository *repository UNUSED)
{
	ধ্রুবক অক্ষর *repo, **refspecs;
	কাঠামো oid_array merge_heads = OID_ARRAY_INIT;
	কাঠামো object_id orig_শীর্ষ, curr_শীর্ষ;
	কাঠামো object_id rebase_fork_point;
	পূর্ণসংখ্যা rebase_unspecified = 0;
	পূর্ণসংখ্যা can_ff;
	পূর্ণসংখ্যা divergent;
	পূর্ণসংখ্যা ret;
	স্থির কাঠামো option pull_options[] = {
		/* Shared options */
		OPT__VERBOSITY(&opt_verbosity),
		OPT_PASSTHRU(0, "progress", &opt_progress, ফাঁকা,
			N_("force progress reporting"),
			PARSE_OPT_NOARG),
		OPT_CALLBACK_F(0, "recurse-submodules",
			   &recurse_submodules_cli, N_("on-demand"),
			   N_("control for recursive fetching of submodules"),
			   PARSE_OPT_OPTARG, option_fetch_parse_recurse_submodules),

		/* Options passed to git-merge or git-rebase */
		OPT_GROUP(N_("Options related to merging")),
		OPT_CALLBACK_F('r', "rebase", &opt_rebase,
			"(false|true|merges|interactive)",
			N_("incorporate changes by rebasing rather than merging"),
			PARSE_OPT_OPTARG, parse_opt_rebase),
		OPT_PASSTHRU('n', ফাঁকা, &opt_diffstat, ফাঁকা,
			N_("do not show a diffstat at the end of the merge"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG),
		OPT_PASSTHRU(0, "stat", &opt_diffstat, ফাঁকা,
			N_("show a diffstat at the end of the merge"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "summary", &opt_diffstat, ফাঁকা,
			N_("(synonym to --stat)"),
			PARSE_OPT_NOARG | PARSE_OPT_HIDDEN),
		OPT_PASSTHRU(0, "compact-summary", &opt_diffstat, ফাঁকা,
			N_("show a compact-summary at the end of the merge"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "log", &opt_log, N_("n"),
			N_("add (at most <n>) entries from shortlog to merge commit message"),
			PARSE_OPT_OPTARG),
		OPT_PASSTHRU(0, "signoff", &opt_signoff, ফাঁকা,
			N_("add a Signed-off-by trailer"),
			PARSE_OPT_OPTARG),
		OPT_PASSTHRU(0, "squash", &opt_squash, ফাঁকা,
			N_("create a single commit instead of doing a merge"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "commit", &opt_commit, ফাঁকা,
			N_("perform a commit if the merge succeeds (default)"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "edit", &opt_edit, ফাঁকা,
			N_("edit message before committing"),
			PARSE_OPT_NOARG),
		OPT_CLEANUP(&cleanup_arg),
		OPT_PASSTHRU(0, "ff", &opt_ff, ফাঁকা,
			N_("allow fast-forward"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "ff-only", &opt_ff, ফাঁকা,
			N_("abort if fast-forward is not possible"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG),
		OPT_PASSTHRU(0, "verify", &opt_verify, ফাঁকা,
			N_("control use of pre-merge-commit and commit-msg hooks"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "verify-signatures", &opt_verify_signatures, ফাঁকা,
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
		OPT_SET_INT(0, "allow-unrelated-histories",
			    &opt_allow_unrelated_histories,
			    N_("allow merging unrelated histories"), 1),

		/* Options passed to git-fetch */
		OPT_GROUP(N_("Options related to fetching")),
		OPT_PASSTHRU(0, "all", &opt_all, ফাঁকা,
			N_("fetch from all remotes"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU('a', "append", &opt_append, ফাঁকা,
			N_("append to .git/FETCH_HEAD instead of overwriting"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "upload-pack", &opt_upload_pack, N_("path"),
			N_("path to upload pack on remote end"),
			0),
		OPT__FORCE(&opt_force, N_("force overwrite of local branch"), 0),
		OPT_PASSTHRU('t', "tags", &opt_tags, ফাঁকা,
			N_("fetch all tags and associated objects"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU('p', "prune", &opt_prune, ফাঁকা,
			N_("prune remote-tracking branches no longer on remote"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU('j', "jobs", &max_children, N_("n"),
			N_("number of submodules pulled in parallel"),
			PARSE_OPT_OPTARG),
		OPT_BOOL(0, "dry-run", &opt_dry_run,
			N_("dry run")),
		OPT_PASSTHRU('k', "keep", &opt_keep, ফাঁকা,
			N_("keep downloaded pack"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "depth", &opt_depth, N_("depth"),
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
		OPT_PASSTHRU(0, "unshallow", &opt_unshallow, ফাঁকা,
			N_("convert to a complete repository"),
			PARSE_OPT_NONEG | PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "update-shallow", &opt_update_shallow, ফাঁকা,
			N_("accept refs that update .git/shallow"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "refmap", &opt_refmap, N_("refmap"),
			N_("specify fetch refmap"),
			PARSE_OPT_NONEG),
		OPT_PASSTHRU_ARGV('o', "server-option", &opt_fetch,
			N_("server-specific"),
			N_("option to transmit"),
			0),
		OPT_PASSTHRU('4',  "ipv4", &opt_ipv4, ফাঁকা,
			N_("use IPv4 addresses only"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU('6',  "ipv6", &opt_ipv6, ফাঁকা,
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
		OPT_PASSTHRU(0, "set-upstream", &set_upstream, ফাঁকা,
			N_("set upstream for git pull/fetch"),
			PARSE_OPT_NOARG),

		OPT_END()
	};

	যদি (!getenv("GIT_REFLOG_ACTION"))
		set_reflog_message(argc, argv);

	repo_config(the_repository, git_pull_config, ফাঁকা);
	যদি (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_full_সূচক = 0;
	}

	argc = parse_options(argc, argv, prefix, pull_options, pull_usage, 0);
	যদি (opt_autostash == -1)
		opt_autostash = config_pull_autostash;

	যদি (recurse_submodules_cli != RECURSE_SUBMODULES_DEFAULT)
		recurse_submodules = recurse_submodules_cli;

	যদি (cleanup_arg)
		/*
		 * this only checks the validity of cleanup_arg; we don't need
		 * a valid value for use_editor
		 */
		get_cleanup_mode(cleanup_arg, 0);

	parse_repo_refspecs(argc, argv, &repo, &refspecs);

	যদি (!opt_ff) {
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
		যদি (opt_rebase >= 0 && opt_ff && !strcmp(opt_ff, "--ff-only")) {
			মুক্ত(opt_ff);
			opt_ff = xstrdup("--ff");
		}
	}

	যদি (opt_rebase < 0)
		opt_rebase = config_get_rebase(&rebase_unspecified);

	যদি (repo_read_সূচক_unmerged(the_repository))
		die_resolve_conflict("pull");

	যদি (file_exists(git_path_merge_শীর্ষ(the_repository)))
		die_conclude_merge();

	যদি (repo_get_oid(the_repository, "HEAD", &orig_শীর্ষ))
		oidclr(&orig_শীর্ষ, the_repository->hash_algo);

	যদি (opt_rebase) {
		যদি (opt_autostash == -1)
			opt_autostash = config_rebase_autostash;

		যদি (is_null_oid(&orig_শীর্ষ) && !is_সূচক_unborn(the_repository->সূচক))
			die(_("Updating an unborn branch with changes added to the index."));

		যদি (!opt_autostash)
			require_clean_work_বৃক্ষ(the_repository,
				N_("pull with rebase"),
				_("Please commit or stash them."), 1, 0);

		যদি (get_rebase_fork_point(&rebase_fork_point, repo, *refspecs))
			oidclr(&rebase_fork_point, the_repository->hash_algo);
	}

	যদি (run_fetch(repo, refspecs))
		ফেরত 1;

	যদি (opt_dry_run)
		ফেরত 0;

	যদি (repo_get_oid(the_repository, "HEAD", &curr_শীর্ষ))
		oidclr(&curr_শীর্ষ, the_repository->hash_algo);

	যদি (!is_null_oid(&orig_শীর্ষ) && !is_null_oid(&curr_শীর্ষ) &&
			!oideq(&orig_শীর্ষ, &curr_শীর্ষ)) {
		/*
		 * The fetch involved updating the current branch.
		 *
		 * The working tree and the index file are still based on
		 * orig_head commit, but we are merging into curr_head.
		 * Update the working tree to match curr_head.
		 */

		warning(_("fetch updated the current branch head.\n"
			"fast-forwarding your working tree from\n"
			"commit %s."), oid_to_hex(&orig_শীর্ষ));

		যদি (checkout_fast_forward(the_repository, &orig_শীর্ষ,
					  &curr_শীর্ষ, 0))
			die(_("Cannot fast-forward your working tree.\n"
				"After making sure that you saved anything precious from\n"
				"$ git diff %s\n"
				"output, run\n"
				"$ git reset --hard\n"
				"to recover."), oid_to_hex(&orig_শীর্ষ));
	}

	get_merge_heads(&merge_heads);

	যদি (!merge_heads.nr)
		die_no_merge_candidates(repo, refspecs);

	যদি (is_null_oid(&orig_শীর্ষ)) {
		যদি (merge_heads.nr > 1)
			die(_("Cannot merge multiple branches into empty head."));
		ret = pull_into_void(merge_heads.oid, &curr_শীর্ষ);
		যাও cleanup;
	}
	যদি (merge_heads.nr > 1) {
		যদি (opt_rebase)
			die(_("Cannot rebase onto multiple branches."));
		যদি (opt_ff && !strcmp(opt_ff, "--ff-only"))
			die(_("Cannot fast-forward to multiple branches."));
	}

	can_ff = get_can_ff(&orig_শীর্ষ, &merge_heads);
	divergent = !can_ff && !already_up_to_date(&orig_শীর্ষ, &merge_heads);

	/* ff-only takes precedence over rebase */
	যদি (opt_ff && !strcmp(opt_ff, "--ff-only")) {
		যদি (divergent)
			die_ff_impossible();
		opt_rebase = REBASE_FALSE;
	}
	/* If no action specified and we can't fast forward, then warn. */
	যদি (!opt_ff && rebase_unspecified && divergent) {
		show_advice_pull_non_ff();
		die(_("Need to specify how to reconcile divergent branches."));
	}

	যদি (opt_rebase) {
		কাঠামো object_id newbase;
		কাঠামো object_id upstream;
		get_rebase_newbase_and_upstream(&newbase, &upstream, &curr_শীর্ষ,
						merge_heads.oid, &rebase_fork_point);

		যদি ((recurse_submodules == RECURSE_SUBMODULES_ON ||
		     recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND) &&
		    submodule_touches_in_range(the_repository, &upstream, &curr_শীর্ষ))
			die(_("cannot rebase with locally recorded submodule modifications"));

		যদি (can_ff) {
			/* we can fast-forward this without invoking rebase */
			মুক্ত(opt_ff);
			opt_ff = xstrdup("--ff-only");
			ret = run_merge();
		} অন্যথা {
			ret = run_rebase(&newbase, &upstream);
		}

		যদি (!ret && (recurse_submodules == RECURSE_SUBMODULES_ON ||
			     recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND))
			ret = rebase_submodules();

		যাও cleanup;
	} অন্যথা {
		ret = run_merge();
		যদি (!ret && (recurse_submodules == RECURSE_SUBMODULES_ON ||
			     recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND))
			ret = update_submodules();
		যাও cleanup;
	}

cleanup:
	oid_array_clear(&merge_heads);
	ফেরত ret;
}
