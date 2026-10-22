/*
 * Builtin "git pull"
 *
 * Based on git-pull.sh by Junio C Hamano
 *
 * Fetch one or more remote refs and merge it/them into the current HEAD.
 */

#define USE_THE_REPOSITORY_VARIABLE

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
ایستا شمارش rebase_type parse_config_rebase(ثابت نویسه *کلید, ثابت نویسه *مقدار,
		صحیح fatal)
{
	شمارش rebase_type v = rebase_parse_مقدار(مقدار);
	اگر (v != REBASE_INVALID)
		بازگشت v;

	اگر (fatal)
		die(_("invalid value for '%s': '%s'"), کلید, مقدار);
	وگرنه
		error(_("invalid value for '%s': '%s'"), کلید, مقدار);

	بازگشت REBASE_INVALID;
}

/**
 * Callback for --rebase, which parses arg with parse_config_rebase().
 */
ایستا صحیح parse_opt_rebase(ثابت ساختار option *opt, ثابت نویسه *arg, صحیح unset)
{
	شمارش rebase_type *مقدار = opt->مقدار;

	اگر (arg)
		*مقدار = parse_config_rebase("--rebase", arg, 0);
	وگرنه
		*مقدار = unset ? REBASE_FALSE : REBASE_TRUE;
	بازگشت *مقدار == REBASE_INVALID ? -1 : 0;
}

ایستا ثابت نویسه * ثابت pull_usage[] = {
	N_("git pull [<options>] [<repository> [<refspec>...]]"),
	پوچ
};

/* Shared options */
ایستا صحیح opt_verbosity;
ایستا ثابت نویسه *opt_progress;
ایستا صحیح recurse_submodules = RECURSE_SUBMODULES_DEFAULT;
ایستا صحیح recurse_submodules_cli = RECURSE_SUBMODULES_DEFAULT;

/* Options passed to git-merge or git-rebase */
ایستا شمارش rebase_type opt_rebase = -1;
ایستا ثابت نویسه *opt_diffstat;
ایستا ثابت نویسه *opt_log;
ایستا ثابت نویسه *opt_signoff;
ایستا ثابت نویسه *opt_squash;
ایستا ثابت نویسه *opt_commit;
ایستا ثابت نویسه *opt_edit;
ایستا ثابت نویسه *cleanup_arg;
ایستا نویسه *opt_ff;
ایستا ثابت نویسه *opt_verify_signatures;
ایستا ثابت نویسه *opt_verify;
ایستا صحیح opt_autostash = -1;
ایستا صحیح config_rebase_autostash;
ایستا صحیح config_pull_autostash = -1;
ایستا صحیح check_trust_level = 1;
ایستا ساختار strvec opt_strategies = STRVEC_INIT;
ایستا ساختار strvec opt_strategy_opts = STRVEC_INIT;
ایستا ثابت نویسه *opt_gpg_sign;
ایستا صحیح opt_allow_unrelated_histories;

/* Options passed to git-fetch */
ایستا ثابت نویسه *opt_all;
ایستا ثابت نویسه *opt_append;
ایستا ثابت نویسه *opt_upload_pack;
ایستا صحیح opt_force;
ایستا ثابت نویسه *opt_tags;
ایستا ثابت نویسه *opt_prune;
ایستا ثابت نویسه *max_children;
ایستا صحیح opt_dry_run;
ایستا ثابت نویسه *opt_keep;
ایستا ثابت نویسه *opt_depth;
ایستا ثابت نویسه *opt_unshallow;
ایستا ثابت نویسه *opt_update_shallow;
ایستا ثابت نویسه *opt_refmap;
ایستا ثابت نویسه *opt_ipv4;
ایستا ثابت نویسه *opt_ipv6;
ایستا صحیح opt_show_forced_updates = -1;
ایستا ثابت نویسه *set_upstream;
ایستا ساختار strvec opt_fetch = STRVEC_INIT;

/**
 * Pushes "-q" or "-v" switches into arr to match the opt_verbosity level.
 */
ایستا تهی argv_افزودن_verbosity(ساختار strvec *arr)
{
	صحیح verbosity;

	برای (verbosity = opt_verbosity; verbosity > 0; verbosity--)
		strvec_افزودن(arr, "-v");

	برای (verbosity = opt_verbosity; verbosity < 0; verbosity++)
		strvec_افزودن(arr, "-q");
}

/**
 * Pushes "-f" switches into arr to match the opt_force level.
 */
ایستا تهی argv_افزودن_force(ساختار strvec *arr)
{
	صحیح force = opt_force;
	وقتی (force-- > 0)
		strvec_افزودن(arr, "-f");
}

/**
 * Sets the GIT_REFLOG_ACTION environment variable to the concatenation of argv
 */
ایستا تهی set_reflog_message(صحیح argc, ثابت نویسه **argv)
{
	صحیح i;
	ساختار strbuf پیام = STRBUF_INIT;

	برای (i = 0; i < argc; i++) {
		اگر (i)
			strbuf_addch(&پیام, ' ');
		strbuf_addstr(&پیام, argv[i]);
	}

	setenv("GIT_REFLOG_ACTION", پیام.buf, 0);

	strbuf_release(&پیام);
}

/**
 * If pull.ff is unset, returns NULL. If pull.ff is "true", returns "--ff". If
 * pull.ff is "false", returns "--no-ff". If pull.ff is "only", returns
 * "--ff-only". Otherwise, if pull.ff is set to an invalid value, die with an
 * error.
 */
ایستا ثابت نویسه *config_get_ff(تهی)
{
	ثابت نویسه *مقدار;

	اگر (repo_config_get_مقدار(the_repository, "pull.ff", &مقدار))
		بازگشت پوچ;

	انتخاب (git_parse_maybe_bool(مقدار)) {
	حالت 0:
		بازگشت "--no-ff";
	حالت 1:
		بازگشت "--ff";
	}

	اگر (!strcmp(مقدار, "only"))
		بازگشت "--ff-only";

	die(_("invalid value for '%s': '%s'"), "pull.ff", مقدار);
}

/**
 * Returns the default configured value for --rebase. It first looks for the
 * value of "branch.$curr_branch.rebase", where $curr_branch is the current
 * branch, and if HEAD is detached or the configuration key does not exist,
 * looks for the value of "pull.rebase". If both configuration keys do not
 * exist, returns REBASE_FALSE.
 */
ایستا شمارش rebase_type config_get_rebase(صحیح *rebase_unspecified)
{
	ساختار branch *curr_branch = branch_get("HEAD");
	ثابت نویسه *مقدار;

	اگر (curr_branch) {
		نویسه *کلید = xstrfmt("branch.%s.rebase", curr_branch->name);

		اگر (!repo_config_get_مقدار(the_repository, کلید, &مقدار)) {
			شمارش rebase_type ret = parse_config_rebase(کلید, مقدار, 1);
			آزاد(کلید);
			بازگشت ret;
		}

		آزاد(کلید);
	}

	اگر (!repo_config_get_مقدار(the_repository, "pull.rebase", &مقدار))
		بازگشت parse_config_rebase("pull.rebase", مقدار, 1);

	*rebase_unspecified = 1;

	بازگشت REBASE_FALSE;
}

/**
 * Read config variables.
 */
ایستا صحیح git_pull_config(ثابت نویسه *var, ثابت نویسه *مقدار,
			   ثابت ساختار config_context *ctx, تهی *cb)
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
		config_rebase_autostash = git_config_bool(var, مقدار);
		بازگشت 0;
	} وگرنه اگر (!strcmp(var, "pull.autostash")) {
		config_pull_autostash = git_config_bool(var, مقدار);
		بازگشت 0;
	} وگرنه اگر (!strcmp(var, "submodule.recurse")) {
		recurse_submodules = git_config_bool(var, مقدار) ?
			RECURSE_SUBMODULES_ON : RECURSE_SUBMODULES_OFF;
		بازگشت 0;
	} وگرنه اگر (!strcmp(var, "gpg.mintrustlevel")) {
		check_trust_level = 0;
	}

	بازگشت git_default_config(var, مقدار, ctx, cb);
}

/**
 * Appends merge candidates from FETCH_HEAD that are not marked not-for-merge
 * into merge_heads.
 */
ایستا تهی get_merge_heads(ساختار oid_array *merge_heads)
{
	ثابت نویسه *filename = git_path_fetch_head(the_repository);
	FILE *fp;
	ساختار strbuf sb = STRBUF_INIT;
	ساختار object_id oid;

	fp = xfopen(filename, "r");
	وقتی (strbuf_getline_lf(&sb, fp) != EOF) {
		ثابت نویسه *p;
		اگر (parse_oid_hex(sb.buf, &oid, &p))
			ادامه;  /* invalid line: does not start with object ID */
		اگر (starts_with(p, "\tnot-for-merge\t"))
			ادامه;  /* ref is not-for-merge */
		oid_array_append(merge_heads, &oid);
	}
	fclose(fp);
	strbuf_release(&sb);
}

/**
 * Used by die_no_merge_candidates() as a for_each_remote() callback to
 * retrieve the name of the remote if the repository only has one remote.
 */
ایستا صحیح get_only_remote(ساختار remote *remote, تهی *cb_data)
{
	ثابت نویسه **remote_name = cb_data;

	اگر (*remote_name)
		بازگشت -1;

	*remote_name = remote->name;
	بازگشت 0;
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
ایستا تهی NORETURN die_no_merge_candidates(ثابت نویسه *repo, ثابت نویسه **refspecs)
{
	ساختار branch *curr_branch = branch_get("HEAD");
	ثابت نویسه *remote = curr_branch ? curr_branch->remote_name : پوچ;

	اگر (*refspecs) {
		اگر (opt_rebase)
			fprintf_ln(stderr, _("There is no candidate for rebasing against among the refs that you just fetched."));
		وگرنه
			fprintf_ln(stderr, _("There are no candidates for merging among the refs that you just fetched."));
		fprintf_ln(stderr, _("Generally this means that you provided a wildcard refspec which had no\n"
					"matches on the remote end."));
	} وگرنه اگر (repo && curr_branch && (!remote || strcmp(repo, remote))) {
		fprintf_ln(stderr, _("You asked to pull from the remote '%s', but did not specify\n"
			"a branch. Because this is not the default configured remote\n"
			"for your current branch, you must specify a branch on the command line."),
			repo);
	} وگرنه اگر (!curr_branch) {
		fprintf_ln(stderr, _("You are not currently on a branch."));
		اگر (opt_rebase)
			fprintf_ln(stderr, _("Please specify which branch you want to rebase against."));
		وگرنه
			fprintf_ln(stderr, _("Please specify which branch you want to merge with."));
		fprintf_ln(stderr, _("See git-pull(1) for details."));
		fprintf(stderr, "\n");
		fprintf_ln(stderr, "    git pull %s %s", _("<remote>"), _("<branch>"));
		fprintf(stderr, "\n");
	} وگرنه اگر (!curr_branch->merge_nr) {
		ثابت نویسه *remote_name = پوچ;

		اگر (for_each_remote(get_only_remote, &remote_name) || !remote_name)
			remote_name = _("<remote>");

		fprintf_ln(stderr, _("There is no tracking information for the current branch."));
		اگر (opt_rebase)
			fprintf_ln(stderr, _("Please specify which branch you want to rebase against."));
		وگرنه
			fprintf_ln(stderr, _("Please specify which branch you want to merge with."));
		fprintf_ln(stderr, _("See git-pull(1) for details."));
		fprintf(stderr, "\n");
		fprintf_ln(stderr, "    git pull %s %s", _("<remote>"), _("<branch>"));
		fprintf(stderr, "\n");
		fprintf_ln(stderr, _("If you wish to set tracking information for this branch you can do so with:"));
		fprintf(stderr, "\n");
		fprintf_ln(stderr, "    git branch --set-upstream-to=%s/%s %s\n",
				remote_name, _("<branch>"), curr_branch->name);
	} وگرنه
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
ایستا تهی parse_repo_refspecs(صحیح argc, ثابت نویسه **argv, ثابت نویسه **repo,
		ثابت نویسه ***refspecs)
{
	اگر (argc > 0) {
		*repo = *argv++;
		argc--;
	} وگرنه
		*repo = پوچ;
	*refspecs = argv;
}

/**
 * Runs git-fetch, returning its exit status. `repo` and `refspecs` are the
 * repository and refspecs to fetch, or NULL if they are not provided.
 */
ایستا صحیح run_fetch(ثابت نویسه *repo, ثابت نویسه **refspecs)
{
	ساختار child_process cmd = CHILD_PROCESS_INIT;

	strvec_pushl(&cmd.args, "fetch", "--update-head-ok", پوچ);

	/* Shared options */
	argv_افزودن_verbosity(&cmd.args);
	اگر (opt_progress)
		strvec_افزودن(&cmd.args, opt_progress);

	/* Options passed to git-fetch */
	اگر (opt_all)
		strvec_افزودن(&cmd.args, opt_all);
	اگر (opt_append)
		strvec_افزودن(&cmd.args, opt_append);
	اگر (opt_upload_pack)
		strvec_افزودن(&cmd.args, opt_upload_pack);
	argv_افزودن_force(&cmd.args);
	اگر (opt_tags)
		strvec_افزودن(&cmd.args, opt_tags);
	اگر (opt_prune)
		strvec_افزودن(&cmd.args, opt_prune);
	اگر (recurse_submodules_cli != RECURSE_SUBMODULES_DEFAULT)
		انتخاب (recurse_submodules_cli) {
		حالت RECURSE_SUBMODULES_ON:
			strvec_افزودن(&cmd.args, "--recurse-submodules=on");
			بشکن;
		حالت RECURSE_SUBMODULES_OFF:
			strvec_افزودن(&cmd.args, "--recurse-submodules=no");
			بشکن;
		حالت RECURSE_SUBMODULES_ON_DEMAND:
			strvec_افزودن(&cmd.args, "--recurse-submodules=on-demand");
			بشکن;
		پیشفرض:
			BUG("submodule recursion option not understood");
		}
	اگر (max_children)
		strvec_افزودن(&cmd.args, max_children);
	اگر (opt_dry_run)
		strvec_افزودن(&cmd.args, "--dry-run");
	اگر (opt_keep)
		strvec_افزودن(&cmd.args, opt_keep);
	اگر (opt_depth)
		strvec_افزودن(&cmd.args, opt_depth);
	اگر (opt_unshallow)
		strvec_افزودن(&cmd.args, opt_unshallow);
	اگر (opt_update_shallow)
		strvec_افزودن(&cmd.args, opt_update_shallow);
	اگر (opt_refmap)
		strvec_افزودن(&cmd.args, opt_refmap);
	اگر (opt_ipv4)
		strvec_افزودن(&cmd.args, opt_ipv4);
	اگر (opt_ipv6)
		strvec_افزودن(&cmd.args, opt_ipv6);
	اگر (opt_show_forced_updates > 0)
		strvec_افزودن(&cmd.args, "--show-forced-updates");
	وگرنه اگر (opt_show_forced_updates == 0)
		strvec_افزودن(&cmd.args, "--no-show-forced-updates");
	اگر (set_upstream)
		strvec_افزودن(&cmd.args, set_upstream);
	strvec_pushv(&cmd.args, opt_fetch.v);

	اگر (repo) {
		strvec_افزودن(&cmd.args, repo);
		strvec_pushv(&cmd.args, refspecs);
	} وگرنه اگر (*refspecs)
		BUG("refspecs without repo?");
	cmd.git_cmd = 1;
	cmd.odb_to_close = the_repository->objects;
	بازگشت run_command(&cmd);
}

/**
 * "Pulls into void" by branching off merge_head.
 */
ایستا صحیح pull_into_void(ثابت ساختار object_id *merge_head,
		ثابت ساختار object_id *curr_head)
{
	اگر (opt_verify_signatures) {
		ساختار commit *commit;

		commit = lookup_commit(the_repository, merge_head);
		اگر (!commit)
			die(_("unable to access commit %s"),
			    oid_to_hex(merge_head));

		verify_merge_signature(commit, opt_verbosity,
				       check_trust_level);
	}

	/*
	 * Two-way merge: we treat the index as based on an empty tree,
	 * and try to fast-forward to HEAD. This ensures we will not lose
	 * index/worktree changes that the user already made on the unborn
	 * branch.
	 */
	اگر (checkout_fast_forward(the_repository,
				  the_hash_algo->empty_درخت,
				  merge_head, 0))
		بازگشت 1;

	اگر (refs_update_ref(get_main_ref_store(the_repository), "initial pull", "HEAD", merge_head, curr_head, 0, UPDATE_REFS_DIE_ON_ERR))
		بازگشت 1;

	بازگشت 0;
}

ایستا صحیح rebase_submodules(تهی)
{
	ساختار child_process cp = CHILD_PROCESS_INIT;

	cp.git_cmd = 1;
	cp.no_stdin = 1;
	strvec_pushl(&cp.args, "submodule", "update",
		     "--recursive", "--rebase", پوچ);
	argv_افزودن_verbosity(&cp.args);

	بازگشت run_command(&cp);
}

ایستا صحیح update_submodules(تهی)
{
	ساختار child_process cp = CHILD_PROCESS_INIT;

	cp.git_cmd = 1;
	cp.no_stdin = 1;
	strvec_pushl(&cp.args, "submodule", "update",
		     "--recursive", "--checkout", پوچ);
	argv_افزودن_verbosity(&cp.args);

	بازگشت run_command(&cp);
}

/**
 * Runs git-merge, returning its exit status.
 */
ایستا صحیح run_merge(تهی)
{
	ساختار child_process cmd = CHILD_PROCESS_INIT;

	strvec_pushl(&cmd.args, "merge", پوچ);

	/* Shared options */
	argv_افزودن_verbosity(&cmd.args);
	اگر (opt_progress)
		strvec_افزودن(&cmd.args, opt_progress);

	/* Options passed to git-merge */
	اگر (opt_diffstat)
		strvec_افزودن(&cmd.args, opt_diffstat);
	اگر (opt_log)
		strvec_افزودن(&cmd.args, opt_log);
	اگر (opt_signoff)
		strvec_افزودن(&cmd.args, opt_signoff);
	اگر (opt_squash)
		strvec_افزودن(&cmd.args, opt_squash);
	اگر (opt_commit)
		strvec_افزودن(&cmd.args, opt_commit);
	اگر (opt_edit)
		strvec_افزودن(&cmd.args, opt_edit);
	اگر (cleanup_arg)
		strvec_pushf(&cmd.args, "--cleanup=%s", cleanup_arg);
	اگر (opt_ff)
		strvec_افزودن(&cmd.args, opt_ff);
	اگر (opt_verify)
		strvec_افزودن(&cmd.args, opt_verify);
	اگر (opt_verify_signatures)
		strvec_افزودن(&cmd.args, opt_verify_signatures);
	strvec_pushv(&cmd.args, opt_strategies.v);
	strvec_pushv(&cmd.args, opt_strategy_opts.v);
	اگر (opt_gpg_sign)
		strvec_افزودن(&cmd.args, opt_gpg_sign);
	اگر (opt_autostash == 0)
		strvec_افزودن(&cmd.args, "--no-autostash");
	وگرنه اگر (opt_autostash == 1)
		strvec_افزودن(&cmd.args, "--autostash");
	اگر (opt_allow_unrelated_histories > 0)
		strvec_افزودن(&cmd.args, "--allow-unrelated-histories");

	strvec_افزودن(&cmd.args, "FETCH_HEAD");
	cmd.git_cmd = 1;
	بازگشت run_command(&cmd);
}

/**
 * Returns remote's upstream branch for the current branch. If remote is NULL,
 * the current branch's configured default remote is used. Returns NULL if
 * `remote` does not name a valid remote, HEAD does not point to a branch,
 * remote is not the branch's configured remote or the branch does not have any
 * configured upstream branch.
 */
ایستا ثابت نویسه *get_upstream_branch(ثابت نویسه *remote)
{
	ساختار remote *rm;
	ساختار branch *curr_branch;
	ثابت نویسه *curr_branch_remote;

	rm = remote_get(remote);
	اگر (!rm)
		بازگشت پوچ;

	curr_branch = branch_get("HEAD");
	اگر (!curr_branch)
		بازگشت پوچ;

	curr_branch_remote = remote_for_branch(curr_branch, پوچ);
	أكد(curr_branch_remote);

	اگر (strcmp(curr_branch_remote, rm->name))
		بازگشت پوچ;

	بازگشت branch_get_upstream(curr_branch, پوچ);
}

/**
 * Derives the remote-tracking branch from the remote and refspec.
 *
 * FIXME: The current implementation assumes the default mapping of
 * refs/heads/<branch_name> to refs/remotes/<remote_name>/<branch_name>.
 */
ایستا ثابت نویسه *get_tracking_branch(ثابت نویسه *remote, ثابت نویسه *refspec)
{
	ساختار refspec_item spec;
	ثابت نویسه *spec_src;
	ثابت نویسه *merge_branch;

	اگر (!refspec_item_init_fetch(&spec, refspec, the_hash_algo))
		die(_("invalid refspec '%s'"), refspec);
	spec_src = spec.src;
	اگر (!*spec_src || !strcmp(spec_src, "HEAD"))
		spec_src = "HEAD";
	وگرنه اگر (skip_prefix(spec_src, "heads/", &spec_src))
		;
	وگرنه اگر (skip_prefix(spec_src, "refs/heads/", &spec_src))
		;
	وگرنه اگر (starts_with(spec_src, "refs/") ||
		starts_with(spec_src, "tags/") ||
		starts_with(spec_src, "remotes/"))
		spec_src = "";

	اگر (*spec_src) {
		اگر (!strcmp(remote, "."))
			merge_branch = mkpath("refs/heads/%s", spec_src);
		وگرنه
			merge_branch = mkpath("refs/remotes/%s/%s", remote, spec_src);
	} وگرنه
		merge_branch = پوچ;

	refspec_item_clear(&spec);
	بازگشت merge_branch;
}

/**
 * Given the repo and refspecs, sets fork_point to the point at which the
 * current branch forked from its remote-tracking branch. Returns 0 on success,
 * -1 on failure.
 */
ایستا صحیح get_rebase_fork_point(ساختار object_id *fork_point, ثابت نویسه *repo,
		ثابت نویسه *refspec)
{
	صحیح ret;
	ساختار branch *curr_branch;
	ثابت نویسه *remote_branch;
	ساختار child_process cp = CHILD_PROCESS_INIT;
	ساختار strbuf sb = STRBUF_INIT;

	curr_branch = branch_get("HEAD");
	اگر (!curr_branch)
		بازگشت -1;

	اگر (refspec)
		remote_branch = get_tracking_branch(repo, refspec);
	وگرنه
		remote_branch = get_upstream_branch(repo);

	اگر (!remote_branch)
		بازگشت -1;

	strvec_pushl(&cp.args, "merge-base", "--fork-point",
		     remote_branch, curr_branch->name, پوچ);
	cp.no_stdin = 1;
	cp.no_stderr = 1;
	cp.git_cmd = 1;

	ret = capture_command(&cp, &sb, GIT_MAX_HEXSZ);
	اگر (ret)
		برو cleanup;

	ret = get_oid_hex(sb.buf, fork_point);
	اگر (ret)
		برو cleanup;

cleanup:
	strbuf_release(&sb);
	بازگشت ret ? -1 : 0;
}

/**
 * Sets merge_base to the octopus merge base of curr_head, merge_head and
 * fork_point. Returns 0 if a merge base is found, 1 otherwise.
 */
ایستا صحیح get_octopus_merge_base(ساختار object_id *merge_base,
		ثابت ساختار object_id *curr_head,
		ثابت ساختار object_id *merge_head,
		ثابت ساختار object_id *fork_point)
{
	ساختار commit_list *revs = پوچ, *result = پوچ;

	commit_list_درج(lookup_commit_reference(the_repository, curr_head),
			   &revs);
	commit_list_درج(lookup_commit_reference(the_repository, merge_head),
			   &revs);
	اگر (!is_null_oid(fork_point))
		commit_list_درج(lookup_commit_reference(the_repository, fork_point),
				   &revs);

	اگر (get_octopus_merge_bases(revs, &result) < 0)
		exit(128);
	commit_list_free(revs);
	reduce_heads_replace(&result);

	اگر (!result)
		بازگشت 1;

	oidcpy(merge_base, &result->item->object.oid);
	commit_list_free(result);
	بازگشت 0;
}

/**
 * Given the current HEAD oid, the merge head returned from git-fetch and the
 * fork point calculated by get_rebase_fork_point(), compute the <newbase> and
 * <upstream> arguments to use for the upcoming git-rebase invocation.
 */
ایستا صحیح get_rebase_newbase_and_upstream(ساختار object_id *newbase,
		ساختار object_id *upstream,
		ثابت ساختار object_id *curr_head,
		ثابت ساختار object_id *merge_head,
		ثابت ساختار object_id *fork_point)
{
	ساختار object_id oct_merge_base;

	اگر (!get_octopus_merge_base(&oct_merge_base, curr_head, merge_head, fork_point))
		اگر (!is_null_oid(fork_point) && oideq(&oct_merge_base, fork_point))
			fork_point = پوچ;

	اگر (fork_point && !is_null_oid(fork_point))
		oidcpy(upstream, fork_point);
	وگرنه
		oidcpy(upstream, merge_head);

	oidcpy(newbase, merge_head);

	بازگشت 0;
}

/**
 * Given the <newbase> and <upstream> calculated by
 * get_rebase_newbase_and_upstream(), runs git-rebase with the
 * appropriate arguments and returns its exit status.
 */
ایستا صحیح run_rebase(ثابت ساختار object_id *newbase,
		ثابت ساختار object_id *upstream)
{
	ساختار child_process cmd = CHILD_PROCESS_INIT;

	strvec_افزودن(&cmd.args, "rebase");

	/* Shared options */
	argv_افزودن_verbosity(&cmd.args);

	/* Options passed to git-rebase */
	اگر (opt_rebase == REBASE_MERGES)
		strvec_افزودن(&cmd.args, "--rebase-merges");
	وگرنه اگر (opt_rebase == REBASE_INTERACTIVE)
		strvec_افزودن(&cmd.args, "--interactive");
	اگر (opt_diffstat)
		strvec_افزودن(&cmd.args, opt_diffstat);
	strvec_pushv(&cmd.args, opt_strategies.v);
	strvec_pushv(&cmd.args, opt_strategy_opts.v);
	اگر (opt_gpg_sign)
		strvec_افزودن(&cmd.args, opt_gpg_sign);
	اگر (opt_signoff)
		strvec_افزودن(&cmd.args, opt_signoff);
	اگر (opt_autostash == 0)
		strvec_افزودن(&cmd.args, "--no-autostash");
	وگرنه اگر (opt_autostash == 1)
		strvec_افزودن(&cmd.args, "--autostash");
	اگر (opt_verify_signatures &&
	    !strcmp(opt_verify_signatures, "--verify-signatures"))
		warning(_("ignoring --verify-signatures for rebase"));

	strvec_افزودن(&cmd.args, "--onto");
	strvec_افزودن(&cmd.args, oid_to_hex(newbase));

	strvec_افزودن(&cmd.args, oid_to_hex(upstream));

	cmd.git_cmd = 1;
	بازگشت run_command(&cmd);
}

ایستا صحیح get_can_ff(ساختار object_id *orig_head,
		      ساختار oid_array *merge_heads)
{
	صحیح ret;
	ساختار commit_list *list = پوچ;
	ساختار commit *merge_head, *head;
	ساختار object_id *orig_merge_head;

	اگر (merge_heads->nr > 1)
		بازگشت 0;

	orig_merge_head = &merge_heads->oid[0];
	head = lookup_commit_reference(the_repository, orig_head);
	اگر (!head)
		بازگشت 0;
	merge_head = lookup_commit_reference(the_repository, orig_merge_head);
	اگر (!merge_head)
		بازگشت 0;
	commit_list_درج(head, &list);
	ret = repo_is_descendant_of(the_repository, merge_head, list);
	commit_list_free(list);
	اگر (ret < 0)
		exit(128);
	بازگشت ret;
}

/*
 * Is orig_head a descendant of _all_ merge_heads?
 * Unfortunately is_descendant_of() cannot be used as it asks
 * if orig_head is a descendant of at least one of them.
 */
ایستا صحیح already_up_to_date(ساختار object_id *orig_head,
			      ساختار oid_array *merge_heads)
{
	ساختار commit *ours;

	ours = lookup_commit_reference(the_repository, orig_head);
	اگر (!ours)
		بازگشت 0;
	برای (size_t i = 0; i < merge_heads->nr; i++) {
		ساختار commit_list *list = پوچ;
		ساختار commit *theirs;
		صحیح ok;

		theirs = lookup_commit_reference(the_repository, &merge_heads->oid[i]);
		اگر (!theirs)
			بازگشت 0;
		commit_list_درج(theirs, &list);
		ok = repo_is_descendant_of(the_repository, ours, list);
		commit_list_free(list);
		اگر (ok < 0)
			exit(128);
		اگر (!ok)
			بازگشت 0;
	}
	بازگشت 1;
}

ایستا تهی show_advice_pull_non_ff(تهی)
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
	     ثابت نویسه **argv,
	     ثابت نویسه *prefix,
	     ساختار repository *repository UNUSED)
{
	ثابت نویسه *repo, **refspecs;
	ساختار oid_array merge_heads = OID_ARRAY_INIT;
	ساختار object_id orig_head, curr_head;
	ساختار object_id rebase_fork_point;
	صحیح rebase_unspecified = 0;
	صحیح can_ff;
	صحیح divergent;
	صحیح ret;
	ایستا ساختار option pull_options[] = {
		/* Shared options */
		OPT__VERBOSITY(&opt_verbosity),
		OPT_PASSTHRU(0, "progress", &opt_progress, پوچ,
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
		OPT_PASSTHRU('n', پوچ, &opt_diffstat, پوچ,
			N_("do not show a diffstat at the end of the merge"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG),
		OPT_PASSTHRU(0, "stat", &opt_diffstat, پوچ,
			N_("show a diffstat at the end of the merge"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "summary", &opt_diffstat, پوچ,
			N_("(synonym to --stat)"),
			PARSE_OPT_NOARG | PARSE_OPT_HIDDEN),
		OPT_PASSTHRU(0, "compact-summary", &opt_diffstat, پوچ,
			N_("show a compact-summary at the end of the merge"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "log", &opt_log, N_("n"),
			N_("add (at most <n>) entries from shortlog to merge commit message"),
			PARSE_OPT_OPTARG),
		OPT_PASSTHRU(0, "signoff", &opt_signoff, پوچ,
			N_("add a Signed-off-by trailer"),
			PARSE_OPT_OPTARG),
		OPT_PASSTHRU(0, "squash", &opt_squash, پوچ,
			N_("create a single commit instead of doing a merge"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "commit", &opt_commit, پوچ,
			N_("perform a commit if the merge succeeds (default)"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "edit", &opt_edit, پوچ,
			N_("edit message before committing"),
			PARSE_OPT_NOARG),
		OPT_CLEANUP(&cleanup_arg),
		OPT_PASSTHRU(0, "ff", &opt_ff, پوچ,
			N_("allow fast-forward"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "ff-only", &opt_ff, پوچ,
			N_("abort if fast-forward is not possible"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG),
		OPT_PASSTHRU(0, "verify", &opt_verify, پوچ,
			N_("control use of pre-merge-commit and commit-msg hooks"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "verify-signatures", &opt_verify_signatures, پوچ,
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
		OPT_PASSTHRU(0, "all", &opt_all, پوچ,
			N_("fetch from all remotes"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU('a', "append", &opt_append, پوچ,
			N_("append to .git/FETCH_HEAD instead of overwriting"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "upload-pack", &opt_upload_pack, N_("path"),
			N_("path to upload pack on remote end"),
			0),
		OPT__FORCE(&opt_force, N_("force overwrite of local branch"), 0),
		OPT_PASSTHRU('t', "tags", &opt_tags, پوچ,
			N_("fetch all tags and associated objects"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU('p', "prune", &opt_prune, پوچ,
			N_("prune remote-tracking branches no longer on remote"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU('j', "jobs", &max_children, N_("n"),
			N_("number of submodules pulled in parallel"),
			PARSE_OPT_OPTARG),
		OPT_BOOL(0, "dry-run", &opt_dry_run,
			N_("dry run")),
		OPT_PASSTHRU('k', "keep", &opt_keep, پوچ,
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
		OPT_PASSTHRU(0, "unshallow", &opt_unshallow, پوچ,
			N_("convert to a complete repository"),
			PARSE_OPT_NONEG | PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "update-shallow", &opt_update_shallow, پوچ,
			N_("accept refs that update .git/shallow"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "refmap", &opt_refmap, N_("refmap"),
			N_("specify fetch refmap"),
			PARSE_OPT_NONEG),
		OPT_PASSTHRU_ARGV('o', "server-option", &opt_fetch,
			N_("server-specific"),
			N_("option to transmit"),
			0),
		OPT_PASSTHRU('4',  "ipv4", &opt_ipv4, پوچ,
			N_("use IPv4 addresses only"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU('6',  "ipv6", &opt_ipv6, پوچ,
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
		OPT_PASSTHRU(0, "set-upstream", &set_upstream, پوچ,
			N_("set upstream for git pull/fetch"),
			PARSE_OPT_NOARG),

		OPT_END()
	};

	اگر (!getenv("GIT_REFLOG_ACTION"))
		set_reflog_message(argc, argv);

	repo_config(the_repository, git_pull_config, پوچ);
	اگر (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_full_index = 0;
	}

	argc = parse_options(argc, argv, prefix, pull_options, pull_usage, 0);
	اگر (opt_autostash == -1)
		opt_autostash = config_pull_autostash;

	اگر (recurse_submodules_cli != RECURSE_SUBMODULES_DEFAULT)
		recurse_submodules = recurse_submodules_cli;

	اگر (cleanup_arg)
		/*
		 * this only checks the validity of cleanup_arg; we don't need
		 * a valid value for use_editor
		 */
		get_cleanup_mode(cleanup_arg, 0);

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

	اگر (repo_read_index_unmerged(the_repository))
		die_resolve_conflict("pull");

	اگر (file_exists(git_path_merge_head(the_repository)))
		die_conclude_merge();

	اگر (repo_get_oid(the_repository, "HEAD", &orig_head))
		oidclr(&orig_head, the_repository->hash_algo);

	اگر (opt_rebase) {
		اگر (opt_autostash == -1)
			opt_autostash = config_rebase_autostash;

		اگر (is_null_oid(&orig_head) && !is_index_unborn(the_repository->index))
			die(_("Updating an unborn branch with changes added to the index."));

		اگر (!opt_autostash)
			require_clean_work_درخت(the_repository,
				N_("pull with rebase"),
				_("Please commit or stash them."), 1, 0);

		اگر (get_rebase_fork_point(&rebase_fork_point, repo, *refspecs))
			oidclr(&rebase_fork_point, the_repository->hash_algo);
	}

	اگر (run_fetch(repo, refspecs))
		بازگشت 1;

	اگر (opt_dry_run)
		بازگشت 0;

	اگر (repo_get_oid(the_repository, "HEAD", &curr_head))
		oidclr(&curr_head, the_repository->hash_algo);

	اگر (!is_null_oid(&orig_head) && !is_null_oid(&curr_head) &&
			!oideq(&orig_head, &curr_head)) {
		/*
		 * The fetch involved updating the current branch.
		 *
		 * The working tree and the index file are still based on
		 * orig_head commit, but we are merging into curr_head.
		 * Update the working tree to match curr_head.
		 */

		warning(_("fetch updated the current branch head.\n"
			"fast-forwarding your working tree from\n"
			"commit %s."), oid_to_hex(&orig_head));

		اگر (checkout_fast_forward(the_repository, &orig_head,
					  &curr_head, 0))
			die(_("Cannot fast-forward your working tree.\n"
				"After making sure that you saved anything precious from\n"
				"$ git diff %s\n"
				"output, run\n"
				"$ git reset --hard\n"
				"to recover."), oid_to_hex(&orig_head));
	}

	get_merge_heads(&merge_heads);

	اگر (!merge_heads.nr)
		die_no_merge_candidates(repo, refspecs);

	اگر (is_null_oid(&orig_head)) {
		اگر (merge_heads.nr > 1)
			die(_("Cannot merge multiple branches into empty head."));
		ret = pull_into_void(merge_heads.oid, &curr_head);
		برو cleanup;
	}
	اگر (merge_heads.nr > 1) {
		اگر (opt_rebase)
			die(_("Cannot rebase onto multiple branches."));
		اگر (opt_ff && !strcmp(opt_ff, "--ff-only"))
			die(_("Cannot fast-forward to multiple branches."));
	}

	can_ff = get_can_ff(&orig_head, &merge_heads);
	divergent = !can_ff && !already_up_to_date(&orig_head, &merge_heads);

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
		ساختار object_id newbase;
		ساختار object_id upstream;
		get_rebase_newbase_and_upstream(&newbase, &upstream, &curr_head,
						merge_heads.oid, &rebase_fork_point);

		اگر ((recurse_submodules == RECURSE_SUBMODULES_ON ||
		     recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND) &&
		    submodule_touches_in_range(the_repository, &upstream, &curr_head))
			die(_("cannot rebase with locally recorded submodule modifications"));

		اگر (can_ff) {
			/* we can fast-forward this without invoking rebase */
			آزاد(opt_ff);
			opt_ff = xstrdup("--ff-only");
			ret = run_merge();
		} وگرنه {
			ret = run_rebase(&newbase, &upstream);
		}

		اگر (!ret && (recurse_submodules == RECURSE_SUBMODULES_ON ||
			     recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND))
			ret = rebase_submodules();

		برو cleanup;
	} وگرنه {
		ret = run_merge();
		اگر (!ret && (recurse_submodules == RECURSE_SUBMODULES_ON ||
			     recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND))
			ret = update_submodules();
		برو cleanup;
	}

cleanup:
	oid_array_clear(&merge_heads);
	بازگشت ret;
}
