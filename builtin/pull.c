/*
 * Builtin "git pull"
 *
 * Based on git-pull.sh by Junio C Hamano
 *
 * Fetch one or more remote refs and merge it/them into the current HEAD.
 */

#تعریف_کن USE_THE_REPOSITORY_VARIABLE

#ضمن "builtin.h"
#ضمن "advice.h"
#ضمن "config.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "hex.h"
#ضمن "merge.h"
#ضمن "object-name.h"
#ضمن "parse-options.h"
#ضمن "run-command.h"
#ضمن "oid-array.h"
#ضمن "remote.h"
#ضمن "dir.h"
#ضمن "path.h"
#ضمن "read-cache-ll.h"
#ضمن "rebase.h"
#ضمن "refs.h"
#ضمن "refspec.h"
#ضمن "submodule.h"
#ضمن "submodule-config.h"
#ضمن "wt-status.h"
#ضمن "commit-reach.h"
#ضمن "sequencer.h"

/**
 * Parses the value of --rebase. If value is a false value, returns
 * REBASE_FALSE. If value is a true value, returns REBASE_TRUE. If value is
 * "merges", returns REBASE_MERGES. If value is a invalid value, dies with
 * a fatal error if fatal is true, otherwise returns REBASE_INVALID.
 */
ساكن تعداد rebase_type parse_config_rebase(ثابت حرف *کلید, ثابت حرف *مقدار,
		صحيح fatal)
{
	تعداد rebase_type v = rebase_parse_مقدار(مقدار);
	إذا (v != REBASE_INVALID)
		أرجع v;

	إذا (fatal)
		die(_("invalid value for '%s': '%s'"), کلید, مقدار);
	وإلا
		error(_("invalid value for '%s': '%s'"), کلید, مقدار);

	أرجع REBASE_INVALID;
}

/**
 * Callback for --rebase, which parses arg with parse_config_rebase().
 */
ساكن صحيح parse_opt_rebase(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	تعداد rebase_type *مقدار = opt->مقدار;

	إذا (arg)
		*مقدار = parse_config_rebase("--rebase", arg, 0);
	وإلا
		*مقدار = unset ? REBASE_FALSE : REBASE_TRUE;
	أرجع *مقدار == REBASE_INVALID ? -1 : 0;
}

ساكن ثابت حرف * ثابت pull_usage[] = {
	N_("git pull [<options>] [<repository> [<refspec>...]]"),
	عدم
};

/* Shared options */
ساكن صحيح opt_verbosity;
ساكن ثابت حرف *opt_پیشرفت;
ساكن صحيح recurse_submodules = RECURSE_SUBMODULES_DEFAULT;
ساكن صحيح recurse_submodules_cli = RECURSE_SUBMODULES_DEFAULT;

/* Options passed to git-merge or git-rebase */
ساكن تعداد rebase_type opt_rebase = -1;
ساكن ثابت حرف *opt_diffstat;
ساكن ثابت حرف *opt_log;
ساكن ثابت حرف *opt_signoff;
ساكن ثابت حرف *opt_squash;
ساكن ثابت حرف *opt_commit;
ساكن ثابت حرف *opt_edit;
ساكن ثابت حرف *cleanup_arg;
ساكن حرف *opt_ff;
ساكن ثابت حرف *opt_verify_signatures;
ساكن ثابت حرف *opt_verify;
ساكن صحيح opt_autostash = -1;
ساكن صحيح config_rebase_autostash;
ساكن صحيح config_pull_autostash = -1;
ساكن صحيح check_trust_level = 1;
ساكن بنية strvec opt_strategies = STRVEC_INIT;
ساكن بنية strvec opt_strategy_opts = STRVEC_INIT;
ساكن ثابت حرف *opt_gpg_sign;
ساكن صحيح opt_allow_unrelated_histories;

/* Options passed to git-fetch */
ساكن ثابت حرف *opt_all;
ساكن ثابت حرف *opt_append;
ساكن ثابت حرف *opt_upload_pack;
ساكن صحيح opt_force;
ساكن ثابت حرف *opt_برچسبها;
ساكن ثابت حرف *opt_prune;
ساكن ثابت حرف *max_children;
ساكن صحيح opt_dry_پیاپی;
ساكن ثابت حرف *opt_keep;
ساكن ثابت حرف *opt_عمق;
ساكن ثابت حرف *opt_unshallow;
ساكن ثابت حرف *opt_update_shallow;
ساكن ثابت حرف *opt_refmap;
ساكن ثابت حرف *opt_ipv4;
ساكن ثابت حرف *opt_ipv6;
ساكن صحيح opt_show_forced_updates = -1;
ساكن ثابت حرف *مجموعه_upstream;
ساكن بنية strvec opt_fetch = STRVEC_INIT;

/**
 * Pushes "-q" or "-v" switches into arr to match the opt_verbosity level.
 */
ساكن فراغ argv_افزودن_verbosity(بنية strvec *arr)
{
	صحيح verbosity;

	لكل (verbosity = opt_verbosity; verbosity > 0; verbosity--)
		strvec_افزودن(arr, "-v");

	لكل (verbosity = opt_verbosity; verbosity < 0; verbosity++)
		strvec_افزودن(arr, "-q");
}

/**
 * Pushes "-f" switches into arr to match the opt_force level.
 */
ساكن فراغ argv_افزودن_force(بنية strvec *arr)
{
	صحيح force = opt_force;
	طالما (force-- > 0)
		strvec_افزودن(arr, "-f");
}

/**
 * Sets the GIT_REFLOG_ACTION environment variable to the concatenation of argv
 */
ساكن فراغ مجموعه_reflog_پیام(صحيح argc, ثابت حرف **argv)
{
	صحيح i;
	بنية strbuf پیام = STRBUF_INIT;

	لكل (i = 0; i < argc; i++) {
		إذا (i)
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
ساكن ثابت حرف *config_get_ff(فراغ)
{
	ثابت حرف *مقدار;

	إذا (repo_config_get_مقدار(the_repository, "pull.ff", &مقدار))
		أرجع عدم;

	بدل (git_parse_maybe_bool(مقدار)) {
	حالة 0:
		أرجع "--no-ff";
	حالة 1:
		أرجع "--ff";
	}

	إذا (!strcmp(مقدار, "only"))
		أرجع "--ff-only";

	die(_("invalid value for '%s': '%s'"), "pull.ff", مقدار);
}

/**
 * Returns the default configured value for --rebase. It first looks for the
 * value of "branch.$curr_branch.rebase", where $curr_branch is the current
 * branch, and if HEAD is detached or the configuration key does not exist,
 * looks for the value of "pull.rebase". If both configuration keys do not
 * exist, returns REBASE_FALSE.
 */
ساكن تعداد rebase_type config_get_rebase(صحيح *rebase_unspecified)
{
	بنية branch *curr_branch = branch_get("HEAD");
	ثابت حرف *مقدار;

	إذا (curr_branch) {
		حرف *کلید = xstrfmt("branch.%s.rebase", curr_branch->name);

		إذا (!repo_config_get_مقدار(the_repository, کلید, &مقدار)) {
			تعداد rebase_type ret = parse_config_rebase(کلید, مقدار, 1);
			حرر(کلید);
			أرجع ret;
		}

		حرر(کلید);
	}

	إذا (!repo_config_get_مقدار(the_repository, "pull.rebase", &مقدار))
		أرجع parse_config_rebase("pull.rebase", مقدار, 1);

	*rebase_unspecified = 1;

	أرجع REBASE_FALSE;
}

/**
 * Read config variables.
 */
ساكن صحيح git_pull_config(ثابت حرف *var, ثابت حرف *مقدار,
			   ثابت بنية config_context *ctx, فراغ *cb)
{
	إذا (!strcmp(var, "rebase.autostash")) {
		/*
		 * run_rebase() also reads this option. The reason we handle it here is
		 * that when pull.rebase is true, a fast-forward may occur without
		 * invoking run_rebase(). We need to ensure that autostash is set even
		 * in the fast-forward case.
		 *
		 * run_merge() handles merge.autostash, so we don't handle it here.
		 */
		config_rebase_autostash = git_config_bool(var, مقدار);
		أرجع 0;
	} وإلا إذا (!strcmp(var, "pull.autostash")) {
		config_pull_autostash = git_config_bool(var, مقدار);
		أرجع 0;
	} وإلا إذا (!strcmp(var, "submodule.recurse")) {
		recurse_submodules = git_config_bool(var, مقدار) ?
			RECURSE_SUBMODULES_ON : RECURSE_SUBMODULES_OFF;
		أرجع 0;
	} وإلا إذا (!strcmp(var, "gpg.mintrustlevel")) {
		check_trust_level = 0;
	}

	أرجع git_default_config(var, مقدار, ctx, cb);
}

/**
 * Appends merge candidates from FETCH_HEAD that are not marked not-for-merge
 * into merge_heads.
 */
ساكن فراغ get_ادغام_heads(بنية oid_array *ادغام_heads)
{
	ثابت حرف *filename = git_مسیر_fetch_سر(the_repository);
	FILE *fp;
	بنية strbuf sb = STRBUF_INIT;
	بنية object_id oid;

	fp = xfopen(filename, "r");
	طالما (strbuf_getline_lf(&sb, fp) != EOF) {
		ثابت حرف *p;
		إذا (parse_oid_hex(sb.buf, &oid, &p))
			استمر;  /* invalid line: does not start with object ID */
		إذا (starts_with(p, "\tnot-for-merge\t"))
			استمر;  /* ref is not-for-merge */
		oid_array_append(ادغام_heads, &oid);
	}
	fclose(fp);
	strbuf_release(&sb);
}

/**
 * Used by die_no_merge_candidates() as a for_each_remote() callback to
 * retrieve the name of the remote if the repository only has one remote.
 */
ساكن صحيح get_only_remote(بنية remote *remote, فراغ *cb_داده)
{
	ثابت حرف **remote_name = cb_داده;

	إذا (*remote_name)
		أرجع -1;

	*remote_name = remote->name;
	أرجع 0;
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
ساكن فراغ NORETURN die_no_ادغام_candidates(ثابت حرف *repo, ثابت حرف **refspecs)
{
	بنية branch *curr_branch = branch_get("HEAD");
	ثابت حرف *remote = curr_branch ? curr_branch->remote_name : عدم;

	إذا (*refspecs) {
		إذا (opt_rebase)
			fprintf_ln(stderr, _("There is no candidate for rebasing against among the refs that you just fetched."));
		وإلا
			fprintf_ln(stderr, _("There are no candidates for merging among the refs that you just fetched."));
		fprintf_ln(stderr, _("Generally this means that you provided a wildcard refspec which had no\n"
					"matches on the remote end."));
	} وإلا إذا (repo && curr_branch && (!remote || strcmp(repo, remote))) {
		fprintf_ln(stderr, _("You asked to pull from the remote '%s', but did not specify\n"
			"a branch. Because this is not the default configured remote\n"
			"for your current branch, you must specify a branch on the command line."),
			repo);
	} وإلا إذا (!curr_branch) {
		fprintf_ln(stderr, _("You are not currently on a branch."));
		إذا (opt_rebase)
			fprintf_ln(stderr, _("Please specify which branch you want to rebase against."));
		وإلا
			fprintf_ln(stderr, _("Please specify which branch you want to merge with."));
		fprintf_ln(stderr, _("See git-pull(1) for details."));
		fprintf(stderr, "\n");
		fprintf_ln(stderr, "    git pull %s %s", _("<remote>"), _("<branch>"));
		fprintf(stderr, "\n");
	} وإلا إذا (!curr_branch->ادغام_nr) {
		ثابت حرف *remote_name = عدم;

		إذا (for_each_remote(get_only_remote, &remote_name) || !remote_name)
			remote_name = _("<remote>");

		fprintf_ln(stderr, _("There is no tracking information for the current branch."));
		إذا (opt_rebase)
			fprintf_ln(stderr, _("Please specify which branch you want to rebase against."));
		وإلا
			fprintf_ln(stderr, _("Please specify which branch you want to merge with."));
		fprintf_ln(stderr, _("See git-pull(1) for details."));
		fprintf(stderr, "\n");
		fprintf_ln(stderr, "    git pull %s %s", _("<remote>"), _("<branch>"));
		fprintf(stderr, "\n");
		fprintf_ln(stderr, _("If you wish to set tracking information for this branch you can do so with:"));
		fprintf(stderr, "\n");
		fprintf_ln(stderr, "    git branch --set-upstream-to=%s/%s %s\n",
				remote_name, _("<branch>"), curr_branch->name);
	} وإلا
		fprintf_ln(stderr, _("Your configuration specifies to merge with the ref '%s'\n"
			"from the remote, but no such ref was fetched."),
			curr_branch->ادغام[0]->src);
	خروج(1);
}

/**
 * Parses argv into [<repo> [<refspecs>...]], returning their values in `repo`
 * as a string and `refspecs` as a null-terminated array of strings. If `repo`
 * is not provided in argv, it is set to NULL.
 */
ساكن فراغ parse_repo_refspecs(صحيح argc, ثابت حرف **argv, ثابت حرف **repo,
		ثابت حرف ***refspecs)
{
	إذا (argc > 0) {
		*repo = *argv++;
		argc--;
	} وإلا
		*repo = عدم;
	*refspecs = argv;
}

/**
 * Runs git-fetch, returning its exit status. `repo` and `refspecs` are the
 * repository and refspecs to fetch, or NULL if they are not provided.
 */
ساكن صحيح پیاپی_fetch(ثابت حرف *repo, ثابت حرف **refspecs)
{
	بنية فرزند_process cmd = فرزند_PROCESS_INIT;

	strvec_pushl(&cmd.آرگومانها, "fetch", "--update-head-ok", عدم);

	/* Shared options */
	argv_افزودن_verbosity(&cmd.آرگومانها);
	إذا (opt_پیشرفت)
		strvec_افزودن(&cmd.آرگومانها, opt_پیشرفت);

	/* Options passed to git-fetch */
	إذا (opt_all)
		strvec_افزودن(&cmd.آرگومانها, opt_all);
	إذا (opt_append)
		strvec_افزودن(&cmd.آرگومانها, opt_append);
	إذا (opt_upload_pack)
		strvec_افزودن(&cmd.آرگومانها, opt_upload_pack);
	argv_افزودن_force(&cmd.آرگومانها);
	إذا (opt_برچسبها)
		strvec_افزودن(&cmd.آرگومانها, opt_برچسبها);
	إذا (opt_prune)
		strvec_افزودن(&cmd.آرگومانها, opt_prune);
	إذا (recurse_submodules_cli != RECURSE_SUBMODULES_DEFAULT)
		بدل (recurse_submodules_cli) {
		حالة RECURSE_SUBMODULES_ON:
			strvec_افزودن(&cmd.آرگومانها, "--recurse-submodules=on");
			اكسر;
		حالة RECURSE_SUBMODULES_OFF:
			strvec_افزودن(&cmd.آرگومانها, "--recurse-submodules=no");
			اكسر;
		حالة RECURSE_SUBMODULES_ON_DEMAND:
			strvec_افزودن(&cmd.آرگومانها, "--recurse-submodules=on-demand");
			اكسر;
		افتراضي:
			BUG("submodule recursion option not understood");
		}
	إذا (max_children)
		strvec_افزودن(&cmd.آرگومانها, max_children);
	إذا (opt_dry_پیاپی)
		strvec_افزودن(&cmd.آرگومانها, "--dry-run");
	إذا (opt_keep)
		strvec_افزودن(&cmd.آرگومانها, opt_keep);
	إذا (opt_عمق)
		strvec_افزودن(&cmd.آرگومانها, opt_عمق);
	إذا (opt_unshallow)
		strvec_افزودن(&cmd.آرگومانها, opt_unshallow);
	إذا (opt_update_shallow)
		strvec_افزودن(&cmd.آرگومانها, opt_update_shallow);
	إذا (opt_refmap)
		strvec_افزودن(&cmd.آرگومانها, opt_refmap);
	إذا (opt_ipv4)
		strvec_افزودن(&cmd.آرگومانها, opt_ipv4);
	إذا (opt_ipv6)
		strvec_افزودن(&cmd.آرگومانها, opt_ipv6);
	إذا (opt_show_forced_updates > 0)
		strvec_افزودن(&cmd.آرگومانها, "--show-forced-updates");
	وإلا إذا (opt_show_forced_updates == 0)
		strvec_افزودن(&cmd.آرگومانها, "--no-show-forced-updates");
	إذا (مجموعه_upstream)
		strvec_افزودن(&cmd.آرگومانها, مجموعه_upstream);
	strvec_pushv(&cmd.آرگومانها, opt_fetch.v);

	إذا (repo) {
		strvec_افزودن(&cmd.آرگومانها, repo);
		strvec_pushv(&cmd.آرگومانها, refspecs);
	} وإلا إذا (*refspecs)
		BUG("refspecs without repo?");
	cmd.git_cmd = 1;
	cmd.odb_مقصد_close = the_repository->objects;
	أرجع پیاپی_command(&cmd);
}

/**
 * "Pulls into void" by branching off merge_head.
 */
ساكن صحيح pull_into_void(ثابت بنية object_id *ادغام_سر,
		ثابت بنية object_id *curr_سر)
{
	إذا (opt_verify_signatures) {
		بنية commit *commit;

		commit = lookup_commit(the_repository, ادغام_سر);
		إذا (!commit)
			die(_("unable to access commit %s"),
			    oid_مقصد_hex(ادغام_سر));

		verify_ادغام_signature(commit, opt_verbosity,
				       check_trust_level);
	}

	/*
	 * Two-way merge: we treat the index as based on an empty tree,
	 * and try to fast-forward to HEAD. This ensures we will not lose
	 * index/worktree changes that the user already made on the unborn
	 * branch.
	 */
	إذا (checkout_fast_forward(the_repository,
				  the_hash_algo->خالی_درخت,
				  ادغام_سر, 0))
		أرجع 1;

	إذا (refs_update_ref(get_main_ref_store(the_repository), "initial pull", "HEAD", ادغام_سر, curr_سر, 0, UPDATE_REFS_DIE_ON_ERR))
		أرجع 1;

	أرجع 0;
}

ساكن صحيح rebase_submodules(فراغ)
{
	بنية فرزند_process cp = فرزند_PROCESS_INIT;

	cp.git_cmd = 1;
	cp.no_stdin = 1;
	strvec_pushl(&cp.آرگومانها, "submodule", "update",
		     "--recursive", "--rebase", عدم);
	argv_افزودن_verbosity(&cp.آرگومانها);

	أرجع پیاپی_command(&cp);
}

ساكن صحيح update_submodules(فراغ)
{
	بنية فرزند_process cp = فرزند_PROCESS_INIT;

	cp.git_cmd = 1;
	cp.no_stdin = 1;
	strvec_pushl(&cp.آرگومانها, "submodule", "update",
		     "--recursive", "--checkout", عدم);
	argv_افزودن_verbosity(&cp.آرگومانها);

	أرجع پیاپی_command(&cp);
}

/**
 * Runs git-merge, returning its exit status.
 */
ساكن صحيح پیاپی_ادغام(فراغ)
{
	بنية فرزند_process cmd = فرزند_PROCESS_INIT;

	strvec_pushl(&cmd.آرگومانها, "merge", عدم);

	/* Shared options */
	argv_افزودن_verbosity(&cmd.آرگومانها);
	إذا (opt_پیشرفت)
		strvec_افزودن(&cmd.آرگومانها, opt_پیشرفت);

	/* Options passed to git-merge */
	إذا (opt_diffstat)
		strvec_افزودن(&cmd.آرگومانها, opt_diffstat);
	إذا (opt_log)
		strvec_افزودن(&cmd.آرگومانها, opt_log);
	إذا (opt_signoff)
		strvec_افزودن(&cmd.آرگومانها, opt_signoff);
	إذا (opt_squash)
		strvec_افزودن(&cmd.آرگومانها, opt_squash);
	إذا (opt_commit)
		strvec_افزودن(&cmd.آرگومانها, opt_commit);
	إذا (opt_edit)
		strvec_افزودن(&cmd.آرگومانها, opt_edit);
	إذا (cleanup_arg)
		strvec_pushf(&cmd.آرگومانها, "--cleanup=%s", cleanup_arg);
	إذا (opt_ff)
		strvec_افزودن(&cmd.آرگومانها, opt_ff);
	إذا (opt_verify)
		strvec_افزودن(&cmd.آرگومانها, opt_verify);
	إذا (opt_verify_signatures)
		strvec_افزودن(&cmd.آرگومانها, opt_verify_signatures);
	strvec_pushv(&cmd.آرگومانها, opt_strategies.v);
	strvec_pushv(&cmd.آرگومانها, opt_strategy_opts.v);
	إذا (opt_gpg_sign)
		strvec_افزودن(&cmd.آرگومانها, opt_gpg_sign);
	إذا (opt_autostash == 0)
		strvec_افزودن(&cmd.آرگومانها, "--no-autostash");
	وإلا إذا (opt_autostash == 1)
		strvec_افزودن(&cmd.آرگومانها, "--autostash");
	إذا (opt_allow_unrelated_histories > 0)
		strvec_افزودن(&cmd.آرگومانها, "--allow-unrelated-histories");

	strvec_افزودن(&cmd.آرگومانها, "FETCH_HEAD");
	cmd.git_cmd = 1;
	أرجع پیاپی_command(&cmd);
}

/**
 * Returns remote's upstream branch for the current branch. If remote is NULL,
 * the current branch's configured default remote is used. Returns NULL if
 * `remote` does not name a valid remote, HEAD does not point to a branch,
 * remote is not the branch's configured remote or the branch does not have any
 * configured upstream branch.
 */
ساكن ثابت حرف *get_upstream_branch(ثابت حرف *remote)
{
	بنية remote *rm;
	بنية branch *curr_branch;
	ثابت حرف *curr_branch_remote;

	rm = remote_get(remote);
	إذا (!rm)
		أرجع عدم;

	curr_branch = branch_get("HEAD");
	إذا (!curr_branch)
		أرجع عدم;

	curr_branch_remote = remote_for_branch(curr_branch, عدم);
	أكد(curr_branch_remote);

	إذا (strcmp(curr_branch_remote, rm->name))
		أرجع عدم;

	أرجع branch_get_upstream(curr_branch, عدم);
}

/**
 * Derives the remote-tracking branch from the remote and refspec.
 *
 * FIXME: The current implementation assumes the default mapping of
 * refs/heads/<branch_name> to refs/remotes/<remote_name>/<branch_name>.
 */
ساكن ثابت حرف *get_tracking_branch(ثابت حرف *remote, ثابت حرف *refspec)
{
	بنية refspec_قلم spec;
	ثابت حرف *spec_src;
	ثابت حرف *ادغام_branch;

	إذا (!refspec_قلم_init_fetch(&spec, refspec, the_hash_algo))
		die(_("invalid refspec '%s'"), refspec);
	spec_src = spec.src;
	إذا (!*spec_src || !strcmp(spec_src, "HEAD"))
		spec_src = "HEAD";
	وإلا إذا (skip_پیشوند(spec_src, "heads/", &spec_src))
		;
	وإلا إذا (skip_پیشوند(spec_src, "refs/heads/", &spec_src))
		;
	وإلا إذا (starts_with(spec_src, "refs/") ||
		starts_with(spec_src, "tags/") ||
		starts_with(spec_src, "remotes/"))
		spec_src = "";

	إذا (*spec_src) {
		إذا (!strcmp(remote, "."))
			ادغام_branch = mkpath("refs/heads/%s", spec_src);
		وإلا
			ادغام_branch = mkpath("refs/remotes/%s/%s", remote, spec_src);
	} وإلا
		ادغام_branch = عدم;

	refspec_قلم_clear(&spec);
	أرجع ادغام_branch;
}

/**
 * Given the repo and refspecs, sets fork_point to the point at which the
 * current branch forked from its remote-tracking branch. Returns 0 on success,
 * -1 on failure.
 */
ساكن صحيح get_rebase_fork_نقطه(بنية object_id *fork_نقطه, ثابت حرف *repo,
		ثابت حرف *refspec)
{
	صحيح ret;
	بنية branch *curr_branch;
	ثابت حرف *remote_branch;
	بنية فرزند_process cp = فرزند_PROCESS_INIT;
	بنية strbuf sb = STRBUF_INIT;

	curr_branch = branch_get("HEAD");
	إذا (!curr_branch)
		أرجع -1;

	إذا (refspec)
		remote_branch = get_tracking_branch(repo, refspec);
	وإلا
		remote_branch = get_upstream_branch(repo);

	إذا (!remote_branch)
		أرجع -1;

	strvec_pushl(&cp.آرگومانها, "merge-base", "--fork-point",
		     remote_branch, curr_branch->name, عدم);
	cp.no_stdin = 1;
	cp.no_stderr = 1;
	cp.git_cmd = 1;

	ret = capture_command(&cp, &sb, GIT_MAX_HEXSZ);
	إذا (ret)
		اذهب cleanup;

	ret = get_oid_hex(sb.buf, fork_نقطه);
	إذا (ret)
		اذهب cleanup;

cleanup:
	strbuf_release(&sb);
	أرجع ret ? -1 : 0;
}

/**
 * Sets merge_base to the octopus merge base of curr_head, merge_head and
 * fork_point. Returns 0 if a merge base is found, 1 otherwise.
 */
ساكن صحيح get_octopus_ادغام_پایه(بنية object_id *ادغام_پایه,
		ثابت بنية object_id *curr_سر,
		ثابت بنية object_id *ادغام_سر,
		ثابت بنية object_id *fork_نقطه)
{
	بنية commit_list *revs = عدم, *نتیجه = عدم;

	commit_list_درج(lookup_commit_reference(the_repository, curr_سر),
			   &revs);
	commit_list_درج(lookup_commit_reference(the_repository, ادغام_سر),
			   &revs);
	إذا (!is_null_oid(fork_نقطه))
		commit_list_درج(lookup_commit_reference(the_repository, fork_نقطه),
				   &revs);

	إذا (get_octopus_ادغام_bases(revs, &نتیجه) < 0)
		خروج(128);
	commit_list_free(revs);
	reduce_heads_replace(&نتیجه);

	إذا (!نتیجه)
		أرجع 1;

	oidcpy(ادغام_پایه, &نتیجه->قلم->object.oid);
	commit_list_free(نتیجه);
	أرجع 0;
}

/**
 * Given the current HEAD oid, the merge head returned from git-fetch and the
 * fork point calculated by get_rebase_fork_point(), compute the <newbase> and
 * <upstream> arguments to use for the upcoming git-rebase invocation.
 */
ساكن صحيح get_rebase_newbase_and_upstream(بنية object_id *newbase,
		بنية object_id *upstream,
		ثابت بنية object_id *curr_سر,
		ثابت بنية object_id *ادغام_سر,
		ثابت بنية object_id *fork_نقطه)
{
	بنية object_id oct_ادغام_پایه;

	إذا (!get_octopus_ادغام_پایه(&oct_ادغام_پایه, curr_سر, ادغام_سر, fork_نقطه))
		إذا (!is_null_oid(fork_نقطه) && oideq(&oct_ادغام_پایه, fork_نقطه))
			fork_نقطه = عدم;

	إذا (fork_نقطه && !is_null_oid(fork_نقطه))
		oidcpy(upstream, fork_نقطه);
	وإلا
		oidcpy(upstream, ادغام_سر);

	oidcpy(newbase, ادغام_سر);

	أرجع 0;
}

/**
 * Given the <newbase> and <upstream> calculated by
 * get_rebase_newbase_and_upstream(), runs git-rebase with the
 * appropriate arguments and returns its exit status.
 */
ساكن صحيح پیاپی_rebase(ثابت بنية object_id *newbase,
		ثابت بنية object_id *upstream)
{
	بنية فرزند_process cmd = فرزند_PROCESS_INIT;

	strvec_افزودن(&cmd.آرگومانها, "rebase");

	/* Shared options */
	argv_افزودن_verbosity(&cmd.آرگومانها);

	/* Options passed to git-rebase */
	إذا (opt_rebase == REBASE_MERGES)
		strvec_افزودن(&cmd.آرگومانها, "--rebase-merges");
	وإلا إذا (opt_rebase == REBASE_INTERACTIVE)
		strvec_افزودن(&cmd.آرگومانها, "--interactive");
	إذا (opt_diffstat)
		strvec_افزودن(&cmd.آرگومانها, opt_diffstat);
	strvec_pushv(&cmd.آرگومانها, opt_strategies.v);
	strvec_pushv(&cmd.آرگومانها, opt_strategy_opts.v);
	إذا (opt_gpg_sign)
		strvec_افزودن(&cmd.آرگومانها, opt_gpg_sign);
	إذا (opt_signoff)
		strvec_افزودن(&cmd.آرگومانها, opt_signoff);
	إذا (opt_autostash == 0)
		strvec_افزودن(&cmd.آرگومانها, "--no-autostash");
	وإلا إذا (opt_autostash == 1)
		strvec_افزودن(&cmd.آرگومانها, "--autostash");
	إذا (opt_verify_signatures &&
	    !strcmp(opt_verify_signatures, "--verify-signatures"))
		warning(_("ignoring --verify-signatures for rebase"));

	strvec_افزودن(&cmd.آرگومانها, "--onto");
	strvec_افزودن(&cmd.آرگومانها, oid_مقصد_hex(newbase));

	strvec_افزودن(&cmd.آرگومانها, oid_مقصد_hex(upstream));

	cmd.git_cmd = 1;
	أرجع پیاپی_command(&cmd);
}

ساكن صحيح get_can_ff(بنية object_id *orig_سر,
		      بنية oid_array *ادغام_heads)
{
	صحيح ret;
	بنية commit_list *list = عدم;
	بنية commit *ادغام_سر, *سر;
	بنية object_id *orig_ادغام_سر;

	إذا (ادغام_heads->nr > 1)
		أرجع 0;

	orig_ادغام_سر = &ادغام_heads->oid[0];
	سر = lookup_commit_reference(the_repository, orig_سر);
	إذا (!سر)
		أرجع 0;
	ادغام_سر = lookup_commit_reference(the_repository, orig_ادغام_سر);
	إذا (!ادغام_سر)
		أرجع 0;
	commit_list_درج(سر, &list);
	ret = repo_is_descendant_از(the_repository, ادغام_سر, list);
	commit_list_free(list);
	إذا (ret < 0)
		خروج(128);
	أرجع ret;
}

/*
 * Is orig_head a descendant of _all_ merge_heads?
 * Unfortunately is_descendant_of() cannot be used as it asks
 * if orig_head is a descendant of at least one of them.
 */
ساكن صحيح already_به_بالا_مقصد_date(بنية object_id *orig_سر,
			      بنية oid_array *ادغام_heads)
{
	بنية commit *ours;

	ours = lookup_commit_reference(the_repository, orig_سر);
	إذا (!ours)
		أرجع 0;
	لكل (اندازه_t i = 0; i < ادغام_heads->nr; i++) {
		بنية commit_list *list = عدم;
		بنية commit *theirs;
		صحيح ok;

		theirs = lookup_commit_reference(the_repository, &ادغام_heads->oid[i]);
		إذا (!theirs)
			أرجع 0;
		commit_list_درج(theirs, &list);
		ok = repo_is_descendant_از(the_repository, ours, list);
		commit_list_free(list);
		إذا (ok < 0)
			خروج(128);
		إذا (!ok)
			أرجع 0;
	}
	أرجع 1;
}

ساكن فراغ show_advice_pull_non_ff(فراغ)
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

صحيح cmd_pull(صحيح argc,
	     ثابت حرف **argv,
	     ثابت حرف *پیشوند,
	     بنية repository *repository استفادهنشده)
{
	ثابت حرف *repo, **refspecs;
	بنية oid_array ادغام_heads = OID_ARRAY_INIT;
	بنية object_id orig_سر, curr_سر;
	بنية object_id rebase_fork_نقطه;
	صحيح rebase_unspecified = 0;
	صحيح can_ff;
	صحيح divergent;
	صحيح ret;
	ساكن بنية option pull_options[] = {
		/* Shared options */
		OPT__VERBOSITY(&opt_verbosity),
		OPT_PASSTHRU(0, "progress", &opt_پیشرفت, عدم,
			N_("force progress reporting"),
			PARSE_OPT_NOARG),
		OPT_CALLBACK_F(0, "recurse-submodules",
			   &recurse_submodules_cli, N_("on-demand"),
			   N_("control for recursive fetching of submodules"),
			   PARSE_OPT_OPTARG, option_fetch_parse_recurse_submodules),

		/* Options passed to git-merge or git-rebase */
		OPT_گروه(N_("Options related to merging")),
		OPT_CALLBACK_F('r', "rebase", &opt_rebase,
			"(false|true|merges|interactive)",
			N_("incorporate changes by rebasing rather than merging"),
			PARSE_OPT_OPTARG, parse_opt_rebase),
		OPT_PASSTHRU('n', عدم, &opt_diffstat, عدم,
			N_("do not show a diffstat at the end of the merge"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG),
		OPT_PASSTHRU(0, "stat", &opt_diffstat, عدم,
			N_("show a diffstat at the end of the merge"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "summary", &opt_diffstat, عدم,
			N_("(synonym to --stat)"),
			PARSE_OPT_NOARG | PARSE_OPT_HIDDEN),
		OPT_PASSTHRU(0, "compact-summary", &opt_diffstat, عدم,
			N_("show a compact-summary at the end of the merge"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "log", &opt_log, N_("n"),
			N_("add (at most <n>) entries from shortlog to merge commit message"),
			PARSE_OPT_OPTARG),
		OPT_PASSTHRU(0, "signoff", &opt_signoff, عدم,
			N_("add a Signed-off-by trailer"),
			PARSE_OPT_OPTARG),
		OPT_PASSTHRU(0, "squash", &opt_squash, عدم,
			N_("create a single commit instead of doing a merge"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "commit", &opt_commit, عدم,
			N_("perform a commit if the merge succeeds (default)"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "edit", &opt_edit, عدم,
			N_("edit message before committing"),
			PARSE_OPT_NOARG),
		OPT_CLEANUP(&cleanup_arg),
		OPT_PASSTHRU(0, "ff", &opt_ff, عدم,
			N_("allow fast-forward"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "ff-only", &opt_ff, عدم,
			N_("abort if fast-forward is not possible"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG),
		OPT_PASSTHRU(0, "verify", &opt_verify, عدم,
			N_("control use of pre-merge-commit and commit-msg hooks"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "verify-signatures", &opt_verify_signatures, عدم,
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
		OPT_مجموعه_INT(0, "allow-unrelated-histories",
			    &opt_allow_unrelated_histories,
			    N_("allow merging unrelated histories"), 1),

		/* Options passed to git-fetch */
		OPT_گروه(N_("Options related to fetching")),
		OPT_PASSTHRU(0, "all", &opt_all, عدم,
			N_("fetch from all remotes"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU('a', "append", &opt_append, عدم,
			N_("append to .git/FETCH_HEAD instead of overwriting"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "upload-pack", &opt_upload_pack, N_("path"),
			N_("path to upload pack on remote end"),
			0),
		OPT__FORCE(&opt_force, N_("force overwrite of local branch"), 0),
		OPT_PASSTHRU('t', "tags", &opt_برچسبها, عدم,
			N_("fetch all tags and associated objects"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU('p', "prune", &opt_prune, عدم,
			N_("prune remote-tracking branches no longer on remote"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU('j', "jobs", &max_children, N_("n"),
			N_("number of submodules pulled in parallel"),
			PARSE_OPT_OPTARG),
		OPT_BOOL(0, "dry-run", &opt_dry_پیاپی,
			N_("dry run")),
		OPT_PASSTHRU('k', "keep", &opt_keep, عدم,
			N_("keep downloaded pack"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "depth", &opt_عمق, N_("depth"),
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
		OPT_PASSTHRU(0, "unshallow", &opt_unshallow, عدم,
			N_("convert to a complete repository"),
			PARSE_OPT_NONEG | PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "update-shallow", &opt_update_shallow, عدم,
			N_("accept refs that update .git/shallow"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "refmap", &opt_refmap, N_("refmap"),
			N_("specify fetch refmap"),
			PARSE_OPT_NONEG),
		OPT_PASSTHRU_ARGV('o', "server-option", &opt_fetch,
			N_("server-specific"),
			N_("option to transmit"),
			0),
		OPT_PASSTHRU('4',  "ipv4", &opt_ipv4, عدم,
			N_("use IPv4 addresses only"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU('6',  "ipv6", &opt_ipv6, عدم,
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
		OPT_PASSTHRU(0, "set-upstream", &مجموعه_upstream, عدم,
			N_("set upstream for git pull/fetch"),
			PARSE_OPT_NOARG),

		OPT_انتها()
	};

	إذا (!getenv("GIT_REFLOG_ACTION"))
		مجموعه_reflog_پیام(argc, argv);

	repo_config(the_repository, git_pull_config, عدم);
	إذا (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_پر_شاخص = 0;
	}

	argc = parse_options(argc, argv, پیشوند, pull_options, pull_usage, 0);
	إذا (opt_autostash == -1)
		opt_autostash = config_pull_autostash;

	إذا (recurse_submodules_cli != RECURSE_SUBMODULES_DEFAULT)
		recurse_submodules = recurse_submodules_cli;

	إذا (cleanup_arg)
		/*
		 * this only checks the validity of cleanup_arg; we don't need
		 * a valid value for use_editor
		 */
		get_cleanup_نما(cleanup_arg, 0);

	parse_repo_refspecs(argc, argv, &repo, &refspecs);

	إذا (!opt_ff) {
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
		إذا (opt_rebase >= 0 && opt_ff && !strcmp(opt_ff, "--ff-only")) {
			حرر(opt_ff);
			opt_ff = xstrdup("--ff");
		}
	}

	إذا (opt_rebase < 0)
		opt_rebase = config_get_rebase(&rebase_unspecified);

	إذا (repo_read_شاخص_unmerged(the_repository))
		die_resolve_conflict("pull");

	إذا (file_exists(git_مسیر_ادغام_سر(the_repository)))
		die_conclude_ادغام();

	إذا (repo_get_oid(the_repository, "HEAD", &orig_سر))
		oidclr(&orig_سر, the_repository->hash_algo);

	إذا (opt_rebase) {
		إذا (opt_autostash == -1)
			opt_autostash = config_rebase_autostash;

		إذا (is_null_oid(&orig_سر) && !is_شاخص_unborn(the_repository->شاخص))
			die(_("Updating an unborn branch with changes added to the index."));

		إذا (!opt_autostash)
			require_clean_work_درخت(the_repository,
				N_("pull with rebase"),
				_("Please commit or stash them."), 1, 0);

		إذا (get_rebase_fork_نقطه(&rebase_fork_نقطه, repo, *refspecs))
			oidclr(&rebase_fork_نقطه, the_repository->hash_algo);
	}

	إذا (پیاپی_fetch(repo, refspecs))
		أرجع 1;

	إذا (opt_dry_پیاپی)
		أرجع 0;

	إذا (repo_get_oid(the_repository, "HEAD", &curr_سر))
		oidclr(&curr_سر, the_repository->hash_algo);

	إذا (!is_null_oid(&orig_سر) && !is_null_oid(&curr_سر) &&
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

		إذا (checkout_fast_forward(the_repository, &orig_سر,
					  &curr_سر, 0))
			die(_("Cannot fast-forward your working tree.\n"
				"After making sure that you saved anything precious from\n"
				"$ git diff %s\n"
				"output, run\n"
				"$ git reset --hard\n"
				"to recover."), oid_مقصد_hex(&orig_سر));
	}

	get_ادغام_heads(&ادغام_heads);

	إذا (!ادغام_heads.nr)
		die_no_ادغام_candidates(repo, refspecs);

	إذا (is_null_oid(&orig_سر)) {
		إذا (ادغام_heads.nr > 1)
			die(_("Cannot merge multiple branches into empty head."));
		ret = pull_into_void(ادغام_heads.oid, &curr_سر);
		اذهب cleanup;
	}
	إذا (ادغام_heads.nr > 1) {
		إذا (opt_rebase)
			die(_("Cannot rebase onto multiple branches."));
		إذا (opt_ff && !strcmp(opt_ff, "--ff-only"))
			die(_("Cannot fast-forward to multiple branches."));
	}

	can_ff = get_can_ff(&orig_سر, &ادغام_heads);
	divergent = !can_ff && !already_به_بالا_مقصد_date(&orig_سر, &ادغام_heads);

	/* ff-only takes precedence over rebase */
	إذا (opt_ff && !strcmp(opt_ff, "--ff-only")) {
		إذا (divergent)
			die_ff_impossible();
		opt_rebase = REBASE_FALSE;
	}
	/* If no action specified and we can't fast forward, then warn. */
	إذا (!opt_ff && rebase_unspecified && divergent) {
		show_advice_pull_non_ff();
		die(_("Need to specify how to reconcile divergent branches."));
	}

	إذا (opt_rebase) {
		بنية object_id newbase;
		بنية object_id upstream;
		get_rebase_newbase_and_upstream(&newbase, &upstream, &curr_سر,
						ادغام_heads.oid, &rebase_fork_نقطه);

		إذا ((recurse_submodules == RECURSE_SUBMODULES_ON ||
		     recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND) &&
		    submodule_touches_درون_range(the_repository, &upstream, &curr_سر))
			die(_("cannot rebase with locally recorded submodule modifications"));

		إذا (can_ff) {
			/* we can fast-forward this without invoking rebase */
			حرر(opt_ff);
			opt_ff = xstrdup("--ff-only");
			ret = پیاپی_ادغام();
		} وإلا {
			ret = پیاپی_rebase(&newbase, &upstream);
		}

		إذا (!ret && (recurse_submodules == RECURSE_SUBMODULES_ON ||
			     recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND))
			ret = rebase_submodules();

		اذهب cleanup;
	} وإلا {
		ret = پیاپی_ادغام();
		إذا (!ret && (recurse_submodules == RECURSE_SUBMODULES_ON ||
			     recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND))
			ret = update_submodules();
		اذهب cleanup;
	}

cleanup:
	oid_array_clear(&ادغام_heads);
	أرجع ret;
}
