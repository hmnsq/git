/*
 * Builtin "git pull"
 *
 * Based on git-pull.sh by Junio C Hamano
 *
 * Fetch one or more remote refs and merge it/them into the current HEAD.
 */

#تعریف_کرو USE_THE_REPOSITORY_VARIABLE

#شامل "builtin.h"
#شامل "advice.h"
#شامل "config.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "hex.h"
#شامل "merge.h"
#شامل "object-name.h"
#شامل "parse-options.h"
#شامل "run-command.h"
#شامل "oid-array.h"
#شامل "remote.h"
#شامل "dir.h"
#شامل "path.h"
#شامل "read-cache-ll.h"
#شامل "rebase.h"
#شامل "refs.h"
#شامل "refspec.h"
#شامل "submodule.h"
#شامل "submodule-config.h"
#شامل "wt-status.h"
#شامل "commit-reach.h"
#شامل "sequencer.h"

/**
 * Parses the value of --rebase. If value is a false value, returns
 * REBASE_FALSE. If value is a true value, returns REBASE_TRUE. If value is
 * "merges", returns REBASE_MERGES. If value is a invalid value, dies with
 * a fatal error if fatal is true, otherwise returns REBASE_INVALID.
 */
ساکن شمار rebase_type parse_config_rebase(ثابت حرف *کلید, ثابت حرف *قدر,
		صحیح fatal)
{
	شمار rebase_type v = rebase_parse_قدر(قدر);
	اگر (v != REBASE_INVALID)
		واپس v;

	اگر (fatal)
		die(_("invalid value for '%s': '%s'"), کلید, قدر);
	ورنہ
		error(_("invalid value for '%s': '%s'"), کلید, قدر);

	واپس REBASE_INVALID;
}

/**
 * Callback for --rebase, which parses arg with parse_config_rebase().
 */
ساکن صحیح parse_opt_rebase(ثابت ساخت option *opt, ثابت حرف *arg, صحیح unset)
{
	شمار rebase_type *قدر = opt->قدر;

	اگر (arg)
		*قدر = parse_config_rebase("--rebase", arg, 0);
	ورنہ
		*قدر = unset ? REBASE_FALSE : REBASE_TRUE;
	واپس *قدر == REBASE_INVALID ? -1 : 0;
}

ساکن ثابت حرف * ثابت pull_usage[] = {
	N_("git pull [<options>] [<repository> [<refspec>...]]"),
	تہی
};

/* Shared options */
ساکن صحیح opt_verbosity;
ساکن ثابت حرف *opt_پیش_رفت;
ساکن صحیح recurse_submodules = RECURSE_SUBMODULES_DEFAULT;
ساکن صحیح recurse_submodules_cli = RECURSE_SUBMODULES_DEFAULT;

/* Options passed to git-merge or git-rebase */
ساکن شمار rebase_type opt_rebase = -1;
ساکن ثابت حرف *opt_diffstat;
ساکن ثابت حرف *opt_log;
ساکن ثابت حرف *opt_signoff;
ساکن ثابت حرف *opt_squash;
ساکن ثابت حرف *opt_commit;
ساکن ثابت حرف *opt_edit;
ساکن ثابت حرف *cleanup_arg;
ساکن حرف *opt_ff;
ساکن ثابت حرف *opt_verify_signatures;
ساکن ثابت حرف *opt_verify;
ساکن صحیح opt_autostash = -1;
ساکن صحیح config_rebase_autostash;
ساکن صحیح config_pull_autostash = -1;
ساکن صحیح check_trust_level = 1;
ساکن ساخت strvec opt_strategies = STRVEC_INIT;
ساکن ساخت strvec opt_strategy_opts = STRVEC_INIT;
ساکن ثابت حرف *opt_gpg_sign;
ساکن صحیح opt_allow_unrelated_histories;

/* Options passed to git-fetch */
ساکن ثابت حرف *opt_all;
ساکن ثابت حرف *opt_append;
ساکن ثابت حرف *opt_upload_pack;
ساکن صحیح opt_force;
ساکن ثابت حرف *opt_لیبل_فہرست;
ساکن ثابت حرف *opt_prune;
ساکن ثابت حرف *max_children;
ساکن صحیح opt_dry_تسلسل;
ساکن ثابت حرف *opt_keep;
ساکن ثابت حرف *opt_گہرائی;
ساکن ثابت حرف *opt_unshallow;
ساکن ثابت حرف *opt_update_shallow;
ساکن ثابت حرف *opt_refmap;
ساکن ثابت حرف *opt_ipv4;
ساکن ثابت حرف *opt_ipv6;
ساکن صحیح opt_show_forced_updates = -1;
ساکن ثابت حرف *سیٹ_upstream;
ساکن ساخت strvec opt_fetch = STRVEC_INIT;

/**
 * Pushes "-q" or "-v" switches into arr to match the opt_verbosity level.
 */
ساکن خالی argv_ڈالو_verbosity(ساخت strvec *arr)
{
	صحیح verbosity;

	برائے (verbosity = opt_verbosity; verbosity > 0; verbosity--)
		strvec_ڈالو(arr, "-v");

	برائے (verbosity = opt_verbosity; verbosity < 0; verbosity++)
		strvec_ڈالو(arr, "-q");
}

/**
 * Pushes "-f" switches into arr to match the opt_force level.
 */
ساکن خالی argv_ڈالو_force(ساخت strvec *arr)
{
	صحیح force = opt_force;
	جب (force-- > 0)
		strvec_ڈالو(arr, "-f");
}

/**
 * Sets the GIT_REFLOG_ACTION environment variable to the concatenation of argv
 */
ساکن خالی سیٹ_reflog_پیغام(صحیح argc, ثابت حرف **argv)
{
	صحیح i;
	ساخت strbuf پیغام = STRBUF_INIT;

	برائے (i = 0; i < argc; i++) {
		اگر (i)
			strbuf_addch(&پیغام, ' ');
		strbuf_addstr(&پیغام, argv[i]);
	}

	setenv("GIT_REFLOG_ACTION", پیغام.buf, 0);

	strbuf_release(&پیغام);
}

/**
 * If pull.ff is unset, returns NULL. If pull.ff is "true", returns "--ff". If
 * pull.ff is "false", returns "--no-ff". If pull.ff is "only", returns
 * "--ff-only". Otherwise, if pull.ff is set to an invalid value, die with an
 * error.
 */
ساکن ثابت حرف *config_get_ff(خالی)
{
	ثابت حرف *قدر;

	اگر (repo_config_get_قدر(the_repository, "pull.ff", &قدر))
		واپس تہی;

	انتخاب (git_parse_maybe_bool(قدر)) {
	حالت 0:
		واپس "--no-ff";
	حالت 1:
		واپس "--ff";
	}

	اگر (!strcmp(قدر, "only"))
		واپس "--ff-only";

	die(_("invalid value for '%s': '%s'"), "pull.ff", قدر);
}

/**
 * Returns the default configured value for --rebase. It first looks for the
 * value of "branch.$curr_branch.rebase", where $curr_branch is the current
 * branch, and if HEAD is detached or the configuration key does not exist,
 * looks for the value of "pull.rebase". If both configuration keys do not
 * exist, returns REBASE_FALSE.
 */
ساکن شمار rebase_type config_get_rebase(صحیح *rebase_unspecified)
{
	ساخت branch *curr_branch = branch_get("HEAD");
	ثابت حرف *قدر;

	اگر (curr_branch) {
		حرف *کلید = xstrfmt("branch.%s.rebase", curr_branch->name);

		اگر (!repo_config_get_قدر(the_repository, کلید, &قدر)) {
			شمار rebase_type ret = parse_config_rebase(کلید, قدر, 1);
			آزاد(کلید);
			واپس ret;
		}

		آزاد(کلید);
	}

	اگر (!repo_config_get_قدر(the_repository, "pull.rebase", &قدر))
		واپس parse_config_rebase("pull.rebase", قدر, 1);

	*rebase_unspecified = 1;

	واپس REBASE_FALSE;
}

/**
 * Read config variables.
 */
ساکن صحیح git_pull_config(ثابت حرف *var, ثابت حرف *قدر,
			   ثابت ساخت config_context *ctx, خالی *cb)
{
	اگر (!strcmp(var, "rebase.autostash")) {
		/*
		 * run_rebase() also reads this option. The reason we handle it here is
		 * that when pull.rebase is true, a fast-forward may occur without
		 * invoking run_rebase(). We need to ensure that autostash is set even
		 * in the fast-forward case.
		 *
		 * run_merge() handles merge.autostash, so we don't handle it here.
		 */
		config_rebase_autostash = git_config_bool(var, قدر);
		واپس 0;
	} ورنہ اگر (!strcmp(var, "pull.autostash")) {
		config_pull_autostash = git_config_bool(var, قدر);
		واپس 0;
	} ورنہ اگر (!strcmp(var, "submodule.recurse")) {
		recurse_submodules = git_config_bool(var, قدر) ?
			RECURSE_SUBMODULES_ON : RECURSE_SUBMODULES_OFF;
		واپس 0;
	} ورنہ اگر (!strcmp(var, "gpg.mintrustlevel")) {
		check_trust_level = 0;
	}

	واپس git_default_config(var, قدر, ctx, cb);
}

/**
 * Appends merge candidates from FETCH_HEAD that are not marked not-for-merge
 * into merge_heads.
 */
ساکن خالی get_ملاؤ_heads(ساخت oid_array *ملاؤ_heads)
{
	ثابت حرف *filename = git_راستہ_fetch_سر(the_repository);
	FILE *fp;
	ساخت strbuf sb = STRBUF_INIT;
	ساخت object_id oid;

	fp = xfopen(filename, "r");
	جب (strbuf_getline_lf(&sb, fp) != EOF) {
		ثابت حرف *p;
		اگر (parse_oid_hex(sb.buf, &oid, &p))
			جاری;  /* invalid line: does not start with object ID */
		اگر (starts_with(p, "\tnot-for-merge\t"))
			جاری;  /* ref is not-for-merge */
		oid_array_append(ملاؤ_heads, &oid);
	}
	fclose(fp);
	strbuf_release(&sb);
}

/**
 * Used by die_no_merge_candidates() as a for_each_remote() callback to
 * retrieve the name of the remote if the repository only has one remote.
 */
ساکن صحیح get_only_remote(ساخت remote *remote, خالی *cb_ڈیٹا)
{
	ثابت حرف **remote_name = cb_ڈیٹا;

	اگر (*remote_name)
		واپس -1;

	*remote_name = remote->name;
	واپس 0;
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
ساکن خالی NORETURN die_no_ملاؤ_candidates(ثابت حرف *repo, ثابت حرف **refspecs)
{
	ساخت branch *curr_branch = branch_get("HEAD");
	ثابت حرف *remote = curr_branch ? curr_branch->remote_name : تہی;

	اگر (*refspecs) {
		اگر (opt_rebase)
			fprintf_ln(stderr, _("There is no candidate for rebasing against among the refs that you just fetched."));
		ورنہ
			fprintf_ln(stderr, _("There are no candidates for merging among the refs that you just fetched."));
		fprintf_ln(stderr, _("Generally this means that you provided a wildcard refspec which had no\n"
					"matches on the remote end."));
	} ورنہ اگر (repo && curr_branch && (!remote || strcmp(repo, remote))) {
		fprintf_ln(stderr, _("You asked to pull from the remote '%s', but did not specify\n"
			"a branch. Because this is not the default configured remote\n"
			"for your current branch, you must specify a branch on the command line."),
			repo);
	} ورنہ اگر (!curr_branch) {
		fprintf_ln(stderr, _("You are not currently on a branch."));
		اگر (opt_rebase)
			fprintf_ln(stderr, _("Please specify which branch you want to rebase against."));
		ورنہ
			fprintf_ln(stderr, _("Please specify which branch you want to merge with."));
		fprintf_ln(stderr, _("See git-pull(1) for details."));
		fprintf(stderr, "\n");
		fprintf_ln(stderr, "    git pull %s %s", _("<remote>"), _("<branch>"));
		fprintf(stderr, "\n");
	} ورنہ اگر (!curr_branch->ملاؤ_nr) {
		ثابت حرف *remote_name = تہی;

		اگر (for_each_remote(get_only_remote, &remote_name) || !remote_name)
			remote_name = _("<remote>");

		fprintf_ln(stderr, _("There is no tracking information for the current branch."));
		اگر (opt_rebase)
			fprintf_ln(stderr, _("Please specify which branch you want to rebase against."));
		ورنہ
			fprintf_ln(stderr, _("Please specify which branch you want to merge with."));
		fprintf_ln(stderr, _("See git-pull(1) for details."));
		fprintf(stderr, "\n");
		fprintf_ln(stderr, "    git pull %s %s", _("<remote>"), _("<branch>"));
		fprintf(stderr, "\n");
		fprintf_ln(stderr, _("If you wish to set tracking information for this branch you can do so with:"));
		fprintf(stderr, "\n");
		fprintf_ln(stderr, "    git branch --set-upstream-to=%s/%s %s\n",
				remote_name, _("<branch>"), curr_branch->name);
	} ورنہ
		fprintf_ln(stderr, _("Your configuration specifies to merge with the ref '%s'\n"
			"from the remote, but no such ref was fetched."),
			curr_branch->ملاؤ[0]->src);
	باہر_نکلو(1);
}

/**
 * Parses argv into [<repo> [<refspecs>...]], returning their values in `repo`
 * as a string and `refspecs` as a null-terminated array of strings. If `repo`
 * is not provided in argv, it is set to NULL.
 */
ساکن خالی parse_repo_refspecs(صحیح argc, ثابت حرف **argv, ثابت حرف **repo,
		ثابت حرف ***refspecs)
{
	اگر (argc > 0) {
		*repo = *argv++;
		argc--;
	} ورنہ
		*repo = تہی;
	*refspecs = argv;
}

/**
 * Runs git-fetch, returning its exit status. `repo` and `refspecs` are the
 * repository and refspecs to fetch, or NULL if they are not provided.
 */
ساکن صحیح تسلسل_fetch(ثابت حرف *repo, ثابت حرف **refspecs)
{
	ساخت بچہ_process cmd = بچہ_PROCESS_INIT;

	strvec_pushl(&cmd.دلائل, "fetch", "--update-head-ok", تہی);

	/* Shared options */
	argv_ڈالو_verbosity(&cmd.دلائل);
	اگر (opt_پیش_رفت)
		strvec_ڈالو(&cmd.دلائل, opt_پیش_رفت);

	/* Options passed to git-fetch */
	اگر (opt_all)
		strvec_ڈالو(&cmd.دلائل, opt_all);
	اگر (opt_append)
		strvec_ڈالو(&cmd.دلائل, opt_append);
	اگر (opt_upload_pack)
		strvec_ڈالو(&cmd.دلائل, opt_upload_pack);
	argv_ڈالو_force(&cmd.دلائل);
	اگر (opt_لیبل_فہرست)
		strvec_ڈالو(&cmd.دلائل, opt_لیبل_فہرست);
	اگر (opt_prune)
		strvec_ڈالو(&cmd.دلائل, opt_prune);
	اگر (recurse_submodules_cli != RECURSE_SUBMODULES_DEFAULT)
		انتخاب (recurse_submodules_cli) {
		حالت RECURSE_SUBMODULES_ON:
			strvec_ڈالو(&cmd.دلائل, "--recurse-submodules=on");
			توڑو;
		حالت RECURSE_SUBMODULES_OFF:
			strvec_ڈالو(&cmd.دلائل, "--recurse-submodules=no");
			توڑو;
		حالت RECURSE_SUBMODULES_ON_DEMAND:
			strvec_ڈالو(&cmd.دلائل, "--recurse-submodules=on-demand");
			توڑو;
		طےشدہ:
			BUG("submodule recursion option not understood");
		}
	اگر (max_children)
		strvec_ڈالو(&cmd.دلائل, max_children);
	اگر (opt_dry_تسلسل)
		strvec_ڈالو(&cmd.دلائل, "--dry-run");
	اگر (opt_keep)
		strvec_ڈالو(&cmd.دلائل, opt_keep);
	اگر (opt_گہرائی)
		strvec_ڈالو(&cmd.دلائل, opt_گہرائی);
	اگر (opt_unshallow)
		strvec_ڈالو(&cmd.دلائل, opt_unshallow);
	اگر (opt_update_shallow)
		strvec_ڈالو(&cmd.دلائل, opt_update_shallow);
	اگر (opt_refmap)
		strvec_ڈالو(&cmd.دلائل, opt_refmap);
	اگر (opt_ipv4)
		strvec_ڈالو(&cmd.دلائل, opt_ipv4);
	اگر (opt_ipv6)
		strvec_ڈالو(&cmd.دلائل, opt_ipv6);
	اگر (opt_show_forced_updates > 0)
		strvec_ڈالو(&cmd.دلائل, "--show-forced-updates");
	ورنہ اگر (opt_show_forced_updates == 0)
		strvec_ڈالو(&cmd.دلائل, "--no-show-forced-updates");
	اگر (سیٹ_upstream)
		strvec_ڈالو(&cmd.دلائل, سیٹ_upstream);
	strvec_pushv(&cmd.دلائل, opt_fetch.v);

	اگر (repo) {
		strvec_ڈالو(&cmd.دلائل, repo);
		strvec_pushv(&cmd.دلائل, refspecs);
	} ورنہ اگر (*refspecs)
		BUG("refspecs without repo?");
	cmd.git_cmd = 1;
	cmd.odb_مقصد_close = the_repository->objects;
	واپس تسلسل_command(&cmd);
}

/**
 * "Pulls into void" by branching off merge_head.
 */
ساکن صحیح pull_into_void(ثابت ساخت object_id *ملاؤ_سر,
		ثابت ساخت object_id *curr_سر)
{
	اگر (opt_verify_signatures) {
		ساخت commit *commit;

		commit = lookup_commit(the_repository, ملاؤ_سر);
		اگر (!commit)
			die(_("unable to access commit %s"),
			    oid_مقصد_hex(ملاؤ_سر));

		verify_ملاؤ_signature(commit, opt_verbosity,
				       check_trust_level);
	}

	/*
	 * Two-way merge: we treat the index as based on an empty tree,
	 * and try to fast-forward to HEAD. This ensures we will not lose
	 * index/worktree changes that the user already made on the unborn
	 * branch.
	 */
	اگر (checkout_fast_forward(the_repository,
				  the_hash_algo->خالی_خانے_درخت,
				  ملاؤ_سر, 0))
		واپس 1;

	اگر (refs_update_ref(get_main_ref_store(the_repository), "initial pull", "HEAD", ملاؤ_سر, curr_سر, 0, UPDATE_REFS_DIE_ON_ERR))
		واپس 1;

	واپس 0;
}

ساکن صحیح rebase_submodules(خالی)
{
	ساخت بچہ_process cp = بچہ_PROCESS_INIT;

	cp.git_cmd = 1;
	cp.no_stdin = 1;
	strvec_pushl(&cp.دلائل, "submodule", "update",
		     "--recursive", "--rebase", تہی);
	argv_ڈالو_verbosity(&cp.دلائل);

	واپس تسلسل_command(&cp);
}

ساکن صحیح update_submodules(خالی)
{
	ساخت بچہ_process cp = بچہ_PROCESS_INIT;

	cp.git_cmd = 1;
	cp.no_stdin = 1;
	strvec_pushl(&cp.دلائل, "submodule", "update",
		     "--recursive", "--checkout", تہی);
	argv_ڈالو_verbosity(&cp.دلائل);

	واپس تسلسل_command(&cp);
}

/**
 * Runs git-merge, returning its exit status.
 */
ساکن صحیح تسلسل_ملاؤ(خالی)
{
	ساخت بچہ_process cmd = بچہ_PROCESS_INIT;

	strvec_pushl(&cmd.دلائل, "merge", تہی);

	/* Shared options */
	argv_ڈالو_verbosity(&cmd.دلائل);
	اگر (opt_پیش_رفت)
		strvec_ڈالو(&cmd.دلائل, opt_پیش_رفت);

	/* Options passed to git-merge */
	اگر (opt_diffstat)
		strvec_ڈالو(&cmd.دلائل, opt_diffstat);
	اگر (opt_log)
		strvec_ڈالو(&cmd.دلائل, opt_log);
	اگر (opt_signoff)
		strvec_ڈالو(&cmd.دلائل, opt_signoff);
	اگر (opt_squash)
		strvec_ڈالو(&cmd.دلائل, opt_squash);
	اگر (opt_commit)
		strvec_ڈالو(&cmd.دلائل, opt_commit);
	اگر (opt_edit)
		strvec_ڈالو(&cmd.دلائل, opt_edit);
	اگر (cleanup_arg)
		strvec_pushf(&cmd.دلائل, "--cleanup=%s", cleanup_arg);
	اگر (opt_ff)
		strvec_ڈالو(&cmd.دلائل, opt_ff);
	اگر (opt_verify)
		strvec_ڈالو(&cmd.دلائل, opt_verify);
	اگر (opt_verify_signatures)
		strvec_ڈالو(&cmd.دلائل, opt_verify_signatures);
	strvec_pushv(&cmd.دلائل, opt_strategies.v);
	strvec_pushv(&cmd.دلائل, opt_strategy_opts.v);
	اگر (opt_gpg_sign)
		strvec_ڈالو(&cmd.دلائل, opt_gpg_sign);
	اگر (opt_autostash == 0)
		strvec_ڈالو(&cmd.دلائل, "--no-autostash");
	ورنہ اگر (opt_autostash == 1)
		strvec_ڈالو(&cmd.دلائل, "--autostash");
	اگر (opt_allow_unrelated_histories > 0)
		strvec_ڈالو(&cmd.دلائل, "--allow-unrelated-histories");

	strvec_ڈالو(&cmd.دلائل, "FETCH_HEAD");
	cmd.git_cmd = 1;
	واپس تسلسل_command(&cmd);
}

/**
 * Returns remote's upstream branch for the current branch. If remote is NULL,
 * the current branch's configured default remote is used. Returns NULL if
 * `remote` does not name a valid remote, HEAD does not point to a branch,
 * remote is not the branch's configured remote or the branch does not have any
 * configured upstream branch.
 */
ساکن ثابت حرف *get_upstream_branch(ثابت حرف *remote)
{
	ساخت remote *rm;
	ساخت branch *curr_branch;
	ثابت حرف *curr_branch_remote;

	rm = remote_get(remote);
	اگر (!rm)
		واپس تہی;

	curr_branch = branch_get("HEAD");
	اگر (!curr_branch)
		واپس تہی;

	curr_branch_remote = remote_for_branch(curr_branch, تہی);
	أكد(curr_branch_remote);

	اگر (strcmp(curr_branch_remote, rm->name))
		واپس تہی;

	واپس branch_get_upstream(curr_branch, تہی);
}

/**
 * Derives the remote-tracking branch from the remote and refspec.
 *
 * FIXME: The current implementation assumes the default mapping of
 * refs/heads/<branch_name> to refs/remotes/<remote_name>/<branch_name>.
 */
ساکن ثابت حرف *get_tracking_branch(ثابت حرف *remote, ثابت حرف *refspec)
{
	ساخت refspec_شے spec;
	ثابت حرف *spec_src;
	ثابت حرف *ملاؤ_branch;

	اگر (!refspec_شے_init_fetch(&spec, refspec, the_hash_algo))
		die(_("invalid refspec '%s'"), refspec);
	spec_src = spec.src;
	اگر (!*spec_src || !strcmp(spec_src, "HEAD"))
		spec_src = "HEAD";
	ورنہ اگر (skip_سابقہ(spec_src, "heads/", &spec_src))
		;
	ورنہ اگر (skip_سابقہ(spec_src, "refs/heads/", &spec_src))
		;
	ورنہ اگر (starts_with(spec_src, "refs/") ||
		starts_with(spec_src, "tags/") ||
		starts_with(spec_src, "remotes/"))
		spec_src = "";

	اگر (*spec_src) {
		اگر (!strcmp(remote, "."))
			ملاؤ_branch = mkpath("refs/heads/%s", spec_src);
		ورنہ
			ملاؤ_branch = mkpath("refs/remotes/%s/%s", remote, spec_src);
	} ورنہ
		ملاؤ_branch = تہی;

	refspec_شے_clear(&spec);
	واپس ملاؤ_branch;
}

/**
 * Given the repo and refspecs, sets fork_point to the point at which the
 * current branch forked from its remote-tracking branch. Returns 0 on success,
 * -1 on failure.
 */
ساکن صحیح get_rebase_fork_نقطہ(ساخت object_id *fork_نقطہ, ثابت حرف *repo,
		ثابت حرف *refspec)
{
	صحیح ret;
	ساخت branch *curr_branch;
	ثابت حرف *remote_branch;
	ساخت بچہ_process cp = بچہ_PROCESS_INIT;
	ساخت strbuf sb = STRBUF_INIT;

	curr_branch = branch_get("HEAD");
	اگر (!curr_branch)
		واپس -1;

	اگر (refspec)
		remote_branch = get_tracking_branch(repo, refspec);
	ورنہ
		remote_branch = get_upstream_branch(repo);

	اگر (!remote_branch)
		واپس -1;

	strvec_pushl(&cp.دلائل, "merge-base", "--fork-point",
		     remote_branch, curr_branch->name, تہی);
	cp.no_stdin = 1;
	cp.no_stderr = 1;
	cp.git_cmd = 1;

	ret = capture_command(&cp, &sb, GIT_MAX_HEXSZ);
	اگر (ret)
		جاؤ cleanup;

	ret = get_oid_hex(sb.buf, fork_نقطہ);
	اگر (ret)
		جاؤ cleanup;

cleanup:
	strbuf_release(&sb);
	واپس ret ? -1 : 0;
}

/**
 * Sets merge_base to the octopus merge base of curr_head, merge_head and
 * fork_point. Returns 0 if a merge base is found, 1 otherwise.
 */
ساکن صحیح get_octopus_ملاؤ_بنیاد(ساخت object_id *ملاؤ_بنیاد,
		ثابت ساخت object_id *curr_سر,
		ثابت ساخت object_id *ملاؤ_سر,
		ثابت ساخت object_id *fork_نقطہ)
{
	ساخت commit_list *revs = تہی, *نتیجہ = تہی;

	commit_list_داخل_کرو(lookup_commit_reference(the_repository, curr_سر),
			   &revs);
	commit_list_داخل_کرو(lookup_commit_reference(the_repository, ملاؤ_سر),
			   &revs);
	اگر (!is_null_oid(fork_نقطہ))
		commit_list_داخل_کرو(lookup_commit_reference(the_repository, fork_نقطہ),
				   &revs);

	اگر (get_octopus_ملاؤ_bases(revs, &نتیجہ) < 0)
		باہر_نکلو(128);
	commit_list_free(revs);
	reduce_heads_replace(&نتیجہ);

	اگر (!نتیجہ)
		واپس 1;

	oidcpy(ملاؤ_بنیاد, &نتیجہ->شے->object.oid);
	commit_list_free(نتیجہ);
	واپس 0;
}

/**
 * Given the current HEAD oid, the merge head returned from git-fetch and the
 * fork point calculated by get_rebase_fork_point(), compute the <newbase> and
 * <upstream> arguments to use for the upcoming git-rebase invocation.
 */
ساکن صحیح get_rebase_newbase_and_upstream(ساخت object_id *newbase,
		ساخت object_id *upstream,
		ثابت ساخت object_id *curr_سر,
		ثابت ساخت object_id *ملاؤ_سر,
		ثابت ساخت object_id *fork_نقطہ)
{
	ساخت object_id oct_ملاؤ_بنیاد;

	اگر (!get_octopus_ملاؤ_بنیاد(&oct_ملاؤ_بنیاد, curr_سر, ملاؤ_سر, fork_نقطہ))
		اگر (!is_null_oid(fork_نقطہ) && oideq(&oct_ملاؤ_بنیاد, fork_نقطہ))
			fork_نقطہ = تہی;

	اگر (fork_نقطہ && !is_null_oid(fork_نقطہ))
		oidcpy(upstream, fork_نقطہ);
	ورنہ
		oidcpy(upstream, ملاؤ_سر);

	oidcpy(newbase, ملاؤ_سر);

	واپس 0;
}

/**
 * Given the <newbase> and <upstream> calculated by
 * get_rebase_newbase_and_upstream(), runs git-rebase with the
 * appropriate arguments and returns its exit status.
 */
ساکن صحیح تسلسل_rebase(ثابت ساخت object_id *newbase,
		ثابت ساخت object_id *upstream)
{
	ساخت بچہ_process cmd = بچہ_PROCESS_INIT;

	strvec_ڈالو(&cmd.دلائل, "rebase");

	/* Shared options */
	argv_ڈالو_verbosity(&cmd.دلائل);

	/* Options passed to git-rebase */
	اگر (opt_rebase == REBASE_MERGES)
		strvec_ڈالو(&cmd.دلائل, "--rebase-merges");
	ورنہ اگر (opt_rebase == REBASE_INTERACTIVE)
		strvec_ڈالو(&cmd.دلائل, "--interactive");
	اگر (opt_diffstat)
		strvec_ڈالو(&cmd.دلائل, opt_diffstat);
	strvec_pushv(&cmd.دلائل, opt_strategies.v);
	strvec_pushv(&cmd.دلائل, opt_strategy_opts.v);
	اگر (opt_gpg_sign)
		strvec_ڈالو(&cmd.دلائل, opt_gpg_sign);
	اگر (opt_signoff)
		strvec_ڈالو(&cmd.دلائل, opt_signoff);
	اگر (opt_autostash == 0)
		strvec_ڈالو(&cmd.دلائل, "--no-autostash");
	ورنہ اگر (opt_autostash == 1)
		strvec_ڈالو(&cmd.دلائل, "--autostash");
	اگر (opt_verify_signatures &&
	    !strcmp(opt_verify_signatures, "--verify-signatures"))
		warning(_("ignoring --verify-signatures for rebase"));

	strvec_ڈالو(&cmd.دلائل, "--onto");
	strvec_ڈالو(&cmd.دلائل, oid_مقصد_hex(newbase));

	strvec_ڈالو(&cmd.دلائل, oid_مقصد_hex(upstream));

	cmd.git_cmd = 1;
	واپس تسلسل_command(&cmd);
}

ساکن صحیح get_can_ff(ساخت object_id *orig_سر,
		      ساخت oid_array *ملاؤ_heads)
{
	صحیح ret;
	ساخت commit_list *list = تہی;
	ساخت commit *ملاؤ_سر, *سر;
	ساخت object_id *orig_ملاؤ_سر;

	اگر (ملاؤ_heads->nr > 1)
		واپس 0;

	orig_ملاؤ_سر = &ملاؤ_heads->oid[0];
	سر = lookup_commit_reference(the_repository, orig_سر);
	اگر (!سر)
		واپس 0;
	ملاؤ_سر = lookup_commit_reference(the_repository, orig_ملاؤ_سر);
	اگر (!ملاؤ_سر)
		واپس 0;
	commit_list_داخل_کرو(سر, &list);
	ret = repo_is_descendant_کا(the_repository, ملاؤ_سر, list);
	commit_list_free(list);
	اگر (ret < 0)
		باہر_نکلو(128);
	واپس ret;
}

/*
 * Is orig_head a descendant of _all_ merge_heads?
 * Unfortunately is_descendant_of() cannot be used as it asks
 * if orig_head is a descendant of at least one of them.
 */
ساکن صحیح already_اوپر_مقصد_date(ساخت object_id *orig_سر,
			      ساخت oid_array *ملاؤ_heads)
{
	ساخت commit *ours;

	ours = lookup_commit_reference(the_repository, orig_سر);
	اگر (!ours)
		واپس 0;
	برائے (سائز_t i = 0; i < ملاؤ_heads->nr; i++) {
		ساخت commit_list *list = تہی;
		ساخت commit *theirs;
		صحیح ok;

		theirs = lookup_commit_reference(the_repository, &ملاؤ_heads->oid[i]);
		اگر (!theirs)
			واپس 0;
		commit_list_داخل_کرو(theirs, &list);
		ok = repo_is_descendant_کا(the_repository, ours, list);
		commit_list_free(list);
		اگر (ok < 0)
			باہر_نکلو(128);
		اگر (!ok)
			واپس 0;
	}
	واپس 1;
}

ساکن خالی show_advice_pull_non_ff(خالی)
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

صحیح cmd_pull(صحیح argc,
	     ثابت حرف **argv,
	     ثابت حرف *سابقہ,
	     ساخت repository *repository غیر_مستعمل)
{
	ثابت حرف *repo, **refspecs;
	ساخت oid_array ملاؤ_heads = OID_ARRAY_INIT;
	ساخت object_id orig_سر, curr_سر;
	ساخت object_id rebase_fork_نقطہ;
	صحیح rebase_unspecified = 0;
	صحیح can_ff;
	صحیح divergent;
	صحیح ret;
	ساکن ساخت option pull_options[] = {
		/* Shared options */
		OPT__VERBOSITY(&opt_verbosity),
		OPT_PASSTHRU(0, "progress", &opt_پیش_رفت, تہی,
			N_("force progress reporting"),
			PARSE_OPT_NOARG),
		OPT_CALLBACK_F(0, "recurse-submodules",
			   &recurse_submodules_cli, N_("on-demand"),
			   N_("control for recursive fetching of submodules"),
			   PARSE_OPT_OPTARG, option_fetch_parse_recurse_submodules),

		/* Options passed to git-merge or git-rebase */
		OPT_گروہ(N_("Options related to merging")),
		OPT_CALLBACK_F('r', "rebase", &opt_rebase,
			"(false|true|merges|interactive)",
			N_("incorporate changes by rebasing rather than merging"),
			PARSE_OPT_OPTARG, parse_opt_rebase),
		OPT_PASSTHRU('n', تہی, &opt_diffstat, تہی,
			N_("do not show a diffstat at the end of the merge"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG),
		OPT_PASSTHRU(0, "stat", &opt_diffstat, تہی,
			N_("show a diffstat at the end of the merge"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "summary", &opt_diffstat, تہی,
			N_("(synonym to --stat)"),
			PARSE_OPT_NOARG | PARSE_OPT_HIDDEN),
		OPT_PASSTHRU(0, "compact-summary", &opt_diffstat, تہی,
			N_("show a compact-summary at the end of the merge"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "log", &opt_log, N_("n"),
			N_("add (at most <n>) entries from shortlog to merge commit message"),
			PARSE_OPT_OPTARG),
		OPT_PASSTHRU(0, "signoff", &opt_signoff, تہی,
			N_("add a Signed-off-by trailer"),
			PARSE_OPT_OPTARG),
		OPT_PASSTHRU(0, "squash", &opt_squash, تہی,
			N_("create a single commit instead of doing a merge"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "commit", &opt_commit, تہی,
			N_("perform a commit if the merge succeeds (default)"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "edit", &opt_edit, تہی,
			N_("edit message before committing"),
			PARSE_OPT_NOARG),
		OPT_CLEANUP(&cleanup_arg),
		OPT_PASSTHRU(0, "ff", &opt_ff, تہی,
			N_("allow fast-forward"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "ff-only", &opt_ff, تہی,
			N_("abort if fast-forward is not possible"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG),
		OPT_PASSTHRU(0, "verify", &opt_verify, تہی,
			N_("control use of pre-merge-commit and commit-msg hooks"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "verify-signatures", &opt_verify_signatures, تہی,
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
		OPT_سیٹ_INT(0, "allow-unrelated-histories",
			    &opt_allow_unrelated_histories,
			    N_("allow merging unrelated histories"), 1),

		/* Options passed to git-fetch */
		OPT_گروہ(N_("Options related to fetching")),
		OPT_PASSTHRU(0, "all", &opt_all, تہی,
			N_("fetch from all remotes"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU('a', "append", &opt_append, تہی,
			N_("append to .git/FETCH_HEAD instead of overwriting"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "upload-pack", &opt_upload_pack, N_("path"),
			N_("path to upload pack on remote end"),
			0),
		OPT__FORCE(&opt_force, N_("force overwrite of local branch"), 0),
		OPT_PASSTHRU('t', "tags", &opt_لیبل_فہرست, تہی,
			N_("fetch all tags and associated objects"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU('p', "prune", &opt_prune, تہی,
			N_("prune remote-tracking branches no longer on remote"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU('j', "jobs", &max_children, N_("n"),
			N_("number of submodules pulled in parallel"),
			PARSE_OPT_OPTARG),
		OPT_BOOL(0, "dry-run", &opt_dry_تسلسل,
			N_("dry run")),
		OPT_PASSTHRU('k', "keep", &opt_keep, تہی,
			N_("keep downloaded pack"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "depth", &opt_گہرائی, N_("depth"),
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
		OPT_PASSTHRU(0, "unshallow", &opt_unshallow, تہی,
			N_("convert to a complete repository"),
			PARSE_OPT_NONEG | PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "update-shallow", &opt_update_shallow, تہی,
			N_("accept refs that update .git/shallow"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "refmap", &opt_refmap, N_("refmap"),
			N_("specify fetch refmap"),
			PARSE_OPT_NONEG),
		OPT_PASSTHRU_ARGV('o', "server-option", &opt_fetch,
			N_("server-specific"),
			N_("option to transmit"),
			0),
		OPT_PASSTHRU('4',  "ipv4", &opt_ipv4, تہی,
			N_("use IPv4 addresses only"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU('6',  "ipv6", &opt_ipv6, تہی,
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
		OPT_PASSTHRU(0, "set-upstream", &سیٹ_upstream, تہی,
			N_("set upstream for git pull/fetch"),
			PARSE_OPT_NOARG),

		OPT_انتہا()
	};

	اگر (!getenv("GIT_REFLOG_ACTION"))
		سیٹ_reflog_پیغام(argc, argv);

	repo_config(the_repository, git_pull_config, تہی);
	اگر (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_بھرا_اشاریہ = 0;
	}

	argc = parse_options(argc, argv, سابقہ, pull_options, pull_usage, 0);
	اگر (opt_autostash == -1)
		opt_autostash = config_pull_autostash;

	اگر (recurse_submodules_cli != RECURSE_SUBMODULES_DEFAULT)
		recurse_submodules = recurse_submodules_cli;

	اگر (cleanup_arg)
		/*
		 * this only checks the validity of cleanup_arg; we don't need
		 * a valid value for use_editor
		 */
		get_cleanup_منوال(cleanup_arg, 0);

	parse_repo_refspecs(argc, argv, &repo, &refspecs);

	اگر (!opt_ff) {
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
		اگر (opt_rebase >= 0 && opt_ff && !strcmp(opt_ff, "--ff-only")) {
			آزاد(opt_ff);
			opt_ff = xstrdup("--ff");
		}
	}

	اگر (opt_rebase < 0)
		opt_rebase = config_get_rebase(&rebase_unspecified);

	اگر (repo_read_اشاریہ_unmerged(the_repository))
		die_resolve_conflict("pull");

	اگر (file_exists(git_راستہ_ملاؤ_سر(the_repository)))
		die_conclude_ملاؤ();

	اگر (repo_get_oid(the_repository, "HEAD", &orig_سر))
		oidclr(&orig_سر, the_repository->hash_algo);

	اگر (opt_rebase) {
		اگر (opt_autostash == -1)
			opt_autostash = config_rebase_autostash;

		اگر (is_null_oid(&orig_سر) && !is_اشاریہ_unborn(the_repository->اشاریہ))
			die(_("Updating an unborn branch with changes added to the index."));

		اگر (!opt_autostash)
			require_clean_work_درخت(the_repository,
				N_("pull with rebase"),
				_("Please commit or stash them."), 1, 0);

		اگر (get_rebase_fork_نقطہ(&rebase_fork_نقطہ, repo, *refspecs))
			oidclr(&rebase_fork_نقطہ, the_repository->hash_algo);
	}

	اگر (تسلسل_fetch(repo, refspecs))
		واپس 1;

	اگر (opt_dry_تسلسل)
		واپس 0;

	اگر (repo_get_oid(the_repository, "HEAD", &curr_سر))
		oidclr(&curr_سر, the_repository->hash_algo);

	اگر (!is_null_oid(&orig_سر) && !is_null_oid(&curr_سر) &&
			!oideq(&orig_سر, &curr_سر)) {
		/*
		 * The fetch involved updating the current branch.
		 *
		 * The working tree and the index file are still based on
		 * orig_head commit, but we are merging into curr_head.
		 * Update the working tree to match curr_head.
		 */

		warning(_("fetch updated the current branch head.\n"
			"fast-forwarding your working tree from\n"
			"commit %s."), oid_مقصد_hex(&orig_سر));

		اگر (checkout_fast_forward(the_repository, &orig_سر,
					  &curr_سر, 0))
			die(_("Cannot fast-forward your working tree.\n"
				"After making sure that you saved anything precious from\n"
				"$ git diff %s\n"
				"output, run\n"
				"$ git reset --hard\n"
				"to recover."), oid_مقصد_hex(&orig_سر));
	}

	get_ملاؤ_heads(&ملاؤ_heads);

	اگر (!ملاؤ_heads.nr)
		die_no_ملاؤ_candidates(repo, refspecs);

	اگر (is_null_oid(&orig_سر)) {
		اگر (ملاؤ_heads.nr > 1)
			die(_("Cannot merge multiple branches into empty head."));
		ret = pull_into_void(ملاؤ_heads.oid, &curr_سر);
		جاؤ cleanup;
	}
	اگر (ملاؤ_heads.nr > 1) {
		اگر (opt_rebase)
			die(_("Cannot rebase onto multiple branches."));
		اگر (opt_ff && !strcmp(opt_ff, "--ff-only"))
			die(_("Cannot fast-forward to multiple branches."));
	}

	can_ff = get_can_ff(&orig_سر, &ملاؤ_heads);
	divergent = !can_ff && !already_اوپر_مقصد_date(&orig_سر, &ملاؤ_heads);

	/* ff-only takes precedence over rebase */
	اگر (opt_ff && !strcmp(opt_ff, "--ff-only")) {
		اگر (divergent)
			die_ff_impossible();
		opt_rebase = REBASE_FALSE;
	}
	/* If no action specified and we can't fast forward, then warn. */
	اگر (!opt_ff && rebase_unspecified && divergent) {
		show_advice_pull_non_ff();
		die(_("Need to specify how to reconcile divergent branches."));
	}

	اگر (opt_rebase) {
		ساخت object_id newbase;
		ساخت object_id upstream;
		get_rebase_newbase_and_upstream(&newbase, &upstream, &curr_سر,
						ملاؤ_heads.oid, &rebase_fork_نقطہ);

		اگر ((recurse_submodules == RECURSE_SUBMODULES_ON ||
		     recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND) &&
		    submodule_touches_اندر_range(the_repository, &upstream, &curr_سر))
			die(_("cannot rebase with locally recorded submodule modifications"));

		اگر (can_ff) {
			/* we can fast-forward this without invoking rebase */
			آزاد(opt_ff);
			opt_ff = xstrdup("--ff-only");
			ret = تسلسل_ملاؤ();
		} ورنہ {
			ret = تسلسل_rebase(&newbase, &upstream);
		}

		اگر (!ret && (recurse_submodules == RECURSE_SUBMODULES_ON ||
			     recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND))
			ret = rebase_submodules();

		جاؤ cleanup;
	} ورنہ {
		ret = تسلسل_ملاؤ();
		اگر (!ret && (recurse_submodules == RECURSE_SUBMODULES_ON ||
			     recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND))
			ret = update_submodules();
		جاؤ cleanup;
	}

cleanup:
	oid_array_clear(&ملاؤ_heads);
	واپس ret;
}
