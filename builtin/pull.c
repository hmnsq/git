/*
 * Builtin "git pull"
 *
 * Based on git-pull.sh by Junio C Hamano
 *
 * Fetch one or more remote refs and merge it/them into the current HEAD.
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "advice.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hex.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "merge.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-name.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "run-command.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "oid-array.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "remote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "dir.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "path.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "read-cache-ll.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "rebase.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "refs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "refspec.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "submodule.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "submodule-config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "wt-status.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "commit-reach.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "sequencer.h"

/**
 * Parses the value of --rebase. If value is a false value, returns
 * REBASE_FALSE. If value is a true value, returns REBASE_TRUE. If value is
 * "merges", returns REBASE_MERGES. If value is a invalid value, dies with
 * a fatal error if fatal is true, otherwise returns REBASE_INVALID.
 */
ਸਥਿਰ ਗਿਣਤੀ rebase_type parse_config_rebase(ਅਚਲ ਅੱਖਰ *ਕੁੰਜੀ, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ,
		ਪੂਰਨ fatal)
{
	ਗਿਣਤੀ rebase_type v = rebase_parse_ਮੁੱਲ(ਮੁੱਲ);
	ਜੇ (v != REBASE_INVALID)
		ਵਾਪਸ v;

	ਜੇ (fatal)
		die(_("invalid value for '%s': '%s'"), ਕੁੰਜੀ, ਮੁੱਲ);
	ਨਹੀਂਤਾਂ
		error(_("invalid value for '%s': '%s'"), ਕੁੰਜੀ, ਮੁੱਲ);

	ਵਾਪਸ REBASE_INVALID;
}

/**
 * Callback for --rebase, which parses arg with parse_config_rebase().
 */
ਸਥਿਰ ਪੂਰਨ parse_opt_rebase(ਅਚਲ ਬਣਤਰ option *opt, ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਗਿਣਤੀ rebase_type *ਮੁੱਲ = opt->ਮੁੱਲ;

	ਜੇ (arg)
		*ਮੁੱਲ = parse_config_rebase("--rebase", arg, 0);
	ਨਹੀਂਤਾਂ
		*ਮੁੱਲ = unset ? REBASE_FALSE : REBASE_TRUE;
	ਵਾਪਸ *ਮੁੱਲ == REBASE_INVALID ? -1 : 0;
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ pull_usage[] = {
	N_("git pull [<options>] [<repository> [<refspec>...]]"),
	ਨਿਰਾ
};

/* Shared options */
ਸਥਿਰ ਪੂਰਨ opt_verbosity;
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *opt_ਤਰੱਕੀ;
ਸਥਿਰ ਪੂਰਨ recurse_submodules = RECURSE_SUBMODULES_DEFAULT;
ਸਥਿਰ ਪੂਰਨ recurse_submodules_cli = RECURSE_SUBMODULES_DEFAULT;

/* Options passed to git-merge or git-rebase */
ਸਥਿਰ ਗਿਣਤੀ rebase_type opt_rebase = -1;
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *opt_diffstat;
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *opt_log;
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *opt_signoff;
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *opt_squash;
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *opt_commit;
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *opt_edit;
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *cleanup_arg;
ਸਥਿਰ ਅੱਖਰ *opt_ff;
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *opt_verify_signatures;
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *opt_verify;
ਸਥਿਰ ਪੂਰਨ opt_autostash = -1;
ਸਥਿਰ ਪੂਰਨ config_rebase_autostash;
ਸਥਿਰ ਪੂਰਨ config_pull_autostash = -1;
ਸਥਿਰ ਪੂਰਨ check_trust_level = 1;
ਸਥਿਰ ਬਣਤਰ strvec opt_strategies = STRVEC_INIT;
ਸਥਿਰ ਬਣਤਰ strvec opt_strategy_opts = STRVEC_INIT;
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *opt_gpg_sign;
ਸਥਿਰ ਪੂਰਨ opt_allow_unrelated_histories;

/* Options passed to git-fetch */
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *opt_all;
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *opt_append;
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *opt_upload_pack;
ਸਥਿਰ ਪੂਰਨ opt_force;
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *opt_ਟੈਗ_ਸੂਚੀ;
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *opt_prune;
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *max_children;
ਸਥਿਰ ਪੂਰਨ opt_dry_ਲੜੀਵਾਰ;
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *opt_keep;
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *opt_ਡੂੰਘਾਈ;
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *opt_unshallow;
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *opt_update_shallow;
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *opt_refmap;
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *opt_ipv4;
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *opt_ipv6;
ਸਥਿਰ ਪੂਰਨ opt_show_forced_updates = -1;
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਸੈੱਟ_upstream;
ਸਥਿਰ ਬਣਤਰ strvec opt_fetch = STRVEC_INIT;

/**
 * Pushes "-q" or "-v" switches into arr to match the opt_verbosity level.
 */
ਸਥਿਰ ਖਾਲੀ argv_ਧੱਕੋ_verbosity(ਬਣਤਰ strvec *arr)
{
	ਪੂਰਨ verbosity;

	ਲਈ (verbosity = opt_verbosity; verbosity > 0; verbosity--)
		strvec_ਧੱਕੋ(arr, "-v");

	ਲਈ (verbosity = opt_verbosity; verbosity < 0; verbosity++)
		strvec_ਧੱਕੋ(arr, "-q");
}

/**
 * Pushes "-f" switches into arr to match the opt_force level.
 */
ਸਥਿਰ ਖਾਲੀ argv_ਧੱਕੋ_force(ਬਣਤਰ strvec *arr)
{
	ਪੂਰਨ force = opt_force;
	ਜਦੋਂ (force-- > 0)
		strvec_ਧੱਕੋ(arr, "-f");
}

/**
 * Sets the GIT_REFLOG_ACTION environment variable to the concatenation of argv
 */
ਸਥਿਰ ਖਾਲੀ ਸੈੱਟ_reflog_ਸੁਨੇਹਾ(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv)
{
	ਪੂਰਨ i;
	ਬਣਤਰ strbuf ਸੁਨੇਹਾ = STRBUF_INIT;

	ਲਈ (i = 0; i < argc; i++) {
		ਜੇ (i)
			strbuf_addch(&ਸੁਨੇਹਾ, ' ');
		strbuf_addstr(&ਸੁਨੇਹਾ, argv[i]);
	}

	setenv("GIT_REFLOG_ACTION", ਸੁਨੇਹਾ.buf, 0);

	strbuf_release(&ਸੁਨੇਹਾ);
}

/**
 * If pull.ff is unset, returns NULL. If pull.ff is "true", returns "--ff". If
 * pull.ff is "false", returns "--no-ff". If pull.ff is "only", returns
 * "--ff-only". Otherwise, if pull.ff is set to an invalid value, die with an
 * error.
 */
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *config_get_ff(ਖਾਲੀ)
{
	ਅਚਲ ਅੱਖਰ *ਮੁੱਲ;

	ਜੇ (repo_config_get_ਮੁੱਲ(the_repository, "pull.ff", &ਮੁੱਲ))
		ਵਾਪਸ ਨਿਰਾ;

	ਚੋਣ (git_parse_maybe_bool(ਮੁੱਲ)) {
	ਹਾਲਤ 0:
		ਵਾਪਸ "--no-ff";
	ਹਾਲਤ 1:
		ਵਾਪਸ "--ff";
	}

	ਜੇ (!strcmp(ਮੁੱਲ, "only"))
		ਵਾਪਸ "--ff-only";

	die(_("invalid value for '%s': '%s'"), "pull.ff", ਮੁੱਲ);
}

/**
 * Returns the default configured value for --rebase. It first looks for the
 * value of "branch.$curr_branch.rebase", where $curr_branch is the current
 * branch, and if HEAD is detached or the configuration key does not exist,
 * looks for the value of "pull.rebase". If both configuration keys do not
 * exist, returns REBASE_FALSE.
 */
ਸਥਿਰ ਗਿਣਤੀ rebase_type config_get_rebase(ਪੂਰਨ *rebase_unspecified)
{
	ਬਣਤਰ branch *curr_branch = branch_get("HEAD");
	ਅਚਲ ਅੱਖਰ *ਮੁੱਲ;

	ਜੇ (curr_branch) {
		ਅੱਖਰ *ਕੁੰਜੀ = xstrfmt("branch.%s.rebase", curr_branch->name);

		ਜੇ (!repo_config_get_ਮੁੱਲ(the_repository, ਕੁੰਜੀ, &ਮੁੱਲ)) {
			ਗਿਣਤੀ rebase_type ret = parse_config_rebase(ਕੁੰਜੀ, ਮੁੱਲ, 1);
			ਮੁਕਤ(ਕੁੰਜੀ);
			ਵਾਪਸ ret;
		}

		ਮੁਕਤ(ਕੁੰਜੀ);
	}

	ਜੇ (!repo_config_get_ਮੁੱਲ(the_repository, "pull.rebase", &ਮੁੱਲ))
		ਵਾਪਸ parse_config_rebase("pull.rebase", ਮੁੱਲ, 1);

	*rebase_unspecified = 1;

	ਵਾਪਸ REBASE_FALSE;
}

/**
 * Read config variables.
 */
ਸਥਿਰ ਪੂਰਨ git_pull_config(ਅਚਲ ਅੱਖਰ *var, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ,
			   ਅਚਲ ਬਣਤਰ config_context *ctx, ਖਾਲੀ *cb)
{
	ਜੇ (!strcmp(var, "rebase.autostash")) {
		/*
		 * run_rebase() also reads this option. The reason we handle it here is
		 * that when pull.rebase is true, a fast-forward may occur without
		 * invoking run_rebase(). We need to ensure that autostash is set even
		 * in the fast-forward case.
		 *
		 * run_merge() handles merge.autostash, so we don't handle it here.
		 */
		config_rebase_autostash = git_config_bool(var, ਮੁੱਲ);
		ਵਾਪਸ 0;
	} ਨਹੀਂਤਾਂ ਜੇ (!strcmp(var, "pull.autostash")) {
		config_pull_autostash = git_config_bool(var, ਮੁੱਲ);
		ਵਾਪਸ 0;
	} ਨਹੀਂਤਾਂ ਜੇ (!strcmp(var, "submodule.recurse")) {
		recurse_submodules = git_config_bool(var, ਮੁੱਲ) ?
			RECURSE_SUBMODULES_ON : RECURSE_SUBMODULES_OFF;
		ਵਾਪਸ 0;
	} ਨਹੀਂਤਾਂ ਜੇ (!strcmp(var, "gpg.mintrustlevel")) {
		check_trust_level = 0;
	}

	ਵਾਪਸ git_default_config(var, ਮੁੱਲ, ctx, cb);
}

/**
 * Appends merge candidates from FETCH_HEAD that are not marked not-for-merge
 * into merge_heads.
 */
ਸਥਿਰ ਖਾਲੀ get_ਮਿਲਾਓ_heads(ਬਣਤਰ oid_array *ਮਿਲਾਓ_heads)
{
	ਅਚਲ ਅੱਖਰ *filename = git_ਰਸਤਾ_fetch_ਸਿਰ(the_repository);
	FILE *fp;
	ਬਣਤਰ strbuf sb = STRBUF_INIT;
	ਬਣਤਰ object_id oid;

	fp = xfopen(filename, "r");
	ਜਦੋਂ (strbuf_getline_lf(&sb, fp) != EOF) {
		ਅਚਲ ਅੱਖਰ *p;
		ਜੇ (parse_oid_hex(sb.buf, &oid, &p))
			ਜਾਰੀ;  /* invalid line: does not start with object ID */
		ਜੇ (starts_with(p, "\tnot-for-merge\t"))
			ਜਾਰੀ;  /* ref is not-for-merge */
		oid_array_append(ਮਿਲਾਓ_heads, &oid);
	}
	fclose(fp);
	strbuf_release(&sb);
}

/**
 * Used by die_no_merge_candidates() as a for_each_remote() callback to
 * retrieve the name of the remote if the repository only has one remote.
 */
ਸਥਿਰ ਪੂਰਨ get_only_remote(ਬਣਤਰ remote *remote, ਖਾਲੀ *cb_ਅੰਕੜੇ)
{
	ਅਚਲ ਅੱਖਰ **remote_name = cb_ਅੰਕੜੇ;

	ਜੇ (*remote_name)
		ਵਾਪਸ -1;

	*remote_name = remote->name;
	ਵਾਪਸ 0;
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
ਸਥਿਰ ਖਾਲੀ NORETURN die_no_ਮਿਲਾਓ_candidates(ਅਚਲ ਅੱਖਰ *repo, ਅਚਲ ਅੱਖਰ **refspecs)
{
	ਬਣਤਰ branch *curr_branch = branch_get("HEAD");
	ਅਚਲ ਅੱਖਰ *remote = curr_branch ? curr_branch->remote_name : ਨਿਰਾ;

	ਜੇ (*refspecs) {
		ਜੇ (opt_rebase)
			fprintf_ln(stderr, _("There is no candidate for rebasing against among the refs that you just fetched."));
		ਨਹੀਂਤਾਂ
			fprintf_ln(stderr, _("There are no candidates for merging among the refs that you just fetched."));
		fprintf_ln(stderr, _("Generally this means that you provided a wildcard refspec which had no\n"
					"matches on the remote end."));
	} ਨਹੀਂਤਾਂ ਜੇ (repo && curr_branch && (!remote || strcmp(repo, remote))) {
		fprintf_ln(stderr, _("You asked to pull from the remote '%s', but did not specify\n"
			"a branch. Because this is not the default configured remote\n"
			"for your current branch, you must specify a branch on the command line."),
			repo);
	} ਨਹੀਂਤਾਂ ਜੇ (!curr_branch) {
		fprintf_ln(stderr, _("You are not currently on a branch."));
		ਜੇ (opt_rebase)
			fprintf_ln(stderr, _("Please specify which branch you want to rebase against."));
		ਨਹੀਂਤਾਂ
			fprintf_ln(stderr, _("Please specify which branch you want to merge with."));
		fprintf_ln(stderr, _("See git-pull(1) for details."));
		fprintf(stderr, "\n");
		fprintf_ln(stderr, "    git pull %s %s", _("<remote>"), _("<branch>"));
		fprintf(stderr, "\n");
	} ਨਹੀਂਤਾਂ ਜੇ (!curr_branch->ਮਿਲਾਓ_nr) {
		ਅਚਲ ਅੱਖਰ *remote_name = ਨਿਰਾ;

		ਜੇ (for_each_remote(get_only_remote, &remote_name) || !remote_name)
			remote_name = _("<remote>");

		fprintf_ln(stderr, _("There is no tracking information for the current branch."));
		ਜੇ (opt_rebase)
			fprintf_ln(stderr, _("Please specify which branch you want to rebase against."));
		ਨਹੀਂਤਾਂ
			fprintf_ln(stderr, _("Please specify which branch you want to merge with."));
		fprintf_ln(stderr, _("See git-pull(1) for details."));
		fprintf(stderr, "\n");
		fprintf_ln(stderr, "    git pull %s %s", _("<remote>"), _("<branch>"));
		fprintf(stderr, "\n");
		fprintf_ln(stderr, _("If you wish to set tracking information for this branch you can do so with:"));
		fprintf(stderr, "\n");
		fprintf_ln(stderr, "    git branch --set-upstream-to=%s/%s %s\n",
				remote_name, _("<branch>"), curr_branch->name);
	} ਨਹੀਂਤਾਂ
		fprintf_ln(stderr, _("Your configuration specifies to merge with the ref '%s'\n"
			"from the remote, but no such ref was fetched."),
			curr_branch->ਮਿਲਾਓ[0]->src);
	ਬਾਹਰ_ਨਿਕਲੋ(1);
}

/**
 * Parses argv into [<repo> [<refspecs>...]], returning their values in `repo`
 * as a string and `refspecs` as a null-terminated array of strings. If `repo`
 * is not provided in argv, it is set to NULL.
 */
ਸਥਿਰ ਖਾਲੀ parse_repo_refspecs(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ **repo,
		ਅਚਲ ਅੱਖਰ ***refspecs)
{
	ਜੇ (argc > 0) {
		*repo = *argv++;
		argc--;
	} ਨਹੀਂਤਾਂ
		*repo = ਨਿਰਾ;
	*refspecs = argv;
}

/**
 * Runs git-fetch, returning its exit status. `repo` and `refspecs` are the
 * repository and refspecs to fetch, or NULL if they are not provided.
 */
ਸਥਿਰ ਪੂਰਨ ਲੜੀਵਾਰ_fetch(ਅਚਲ ਅੱਖਰ *repo, ਅਚਲ ਅੱਖਰ **refspecs)
{
	ਬਣਤਰ ਬੱਚਾ_process cmd = ਬੱਚਾ_PROCESS_INIT;

	strvec_pushl(&cmd.ਦਲੀਲਾਂ, "fetch", "--update-head-ok", ਨਿਰਾ);

	/* Shared options */
	argv_ਧੱਕੋ_verbosity(&cmd.ਦਲੀਲਾਂ);
	ਜੇ (opt_ਤਰੱਕੀ)
		strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, opt_ਤਰੱਕੀ);

	/* Options passed to git-fetch */
	ਜੇ (opt_all)
		strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, opt_all);
	ਜੇ (opt_append)
		strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, opt_append);
	ਜੇ (opt_upload_pack)
		strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, opt_upload_pack);
	argv_ਧੱਕੋ_force(&cmd.ਦਲੀਲਾਂ);
	ਜੇ (opt_ਟੈਗ_ਸੂਚੀ)
		strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, opt_ਟੈਗ_ਸੂਚੀ);
	ਜੇ (opt_prune)
		strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, opt_prune);
	ਜੇ (recurse_submodules_cli != RECURSE_SUBMODULES_DEFAULT)
		ਚੋਣ (recurse_submodules_cli) {
		ਹਾਲਤ RECURSE_SUBMODULES_ON:
			strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, "--recurse-submodules=on");
			ਰੋਕੋ;
		ਹਾਲਤ RECURSE_SUBMODULES_OFF:
			strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, "--recurse-submodules=no");
			ਰੋਕੋ;
		ਹਾਲਤ RECURSE_SUBMODULES_ON_DEMAND:
			strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, "--recurse-submodules=on-demand");
			ਰੋਕੋ;
		ਮੂਲ:
			BUG("submodule recursion option not understood");
		}
	ਜੇ (max_children)
		strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, max_children);
	ਜੇ (opt_dry_ਲੜੀਵਾਰ)
		strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, "--dry-run");
	ਜੇ (opt_keep)
		strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, opt_keep);
	ਜੇ (opt_ਡੂੰਘਾਈ)
		strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, opt_ਡੂੰਘਾਈ);
	ਜੇ (opt_unshallow)
		strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, opt_unshallow);
	ਜੇ (opt_update_shallow)
		strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, opt_update_shallow);
	ਜੇ (opt_refmap)
		strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, opt_refmap);
	ਜੇ (opt_ipv4)
		strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, opt_ipv4);
	ਜੇ (opt_ipv6)
		strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, opt_ipv6);
	ਜੇ (opt_show_forced_updates > 0)
		strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, "--show-forced-updates");
	ਨਹੀਂਤਾਂ ਜੇ (opt_show_forced_updates == 0)
		strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, "--no-show-forced-updates");
	ਜੇ (ਸੈੱਟ_upstream)
		strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, ਸੈੱਟ_upstream);
	strvec_pushv(&cmd.ਦਲੀਲਾਂ, opt_fetch.v);

	ਜੇ (repo) {
		strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, repo);
		strvec_pushv(&cmd.ਦਲੀਲਾਂ, refspecs);
	} ਨਹੀਂਤਾਂ ਜੇ (*refspecs)
		BUG("refspecs without repo?");
	cmd.git_cmd = 1;
	cmd.odb_ਮੰਜ਼ਿਲ_close = the_repository->objects;
	ਵਾਪਸ ਲੜੀਵਾਰ_command(&cmd);
}

/**
 * "Pulls into void" by branching off merge_head.
 */
ਸਥਿਰ ਪੂਰਨ pull_into_void(ਅਚਲ ਬਣਤਰ object_id *ਮਿਲਾਓ_ਸਿਰ,
		ਅਚਲ ਬਣਤਰ object_id *curr_ਸਿਰ)
{
	ਜੇ (opt_verify_signatures) {
		ਬਣਤਰ commit *commit;

		commit = lookup_commit(the_repository, ਮਿਲਾਓ_ਸਿਰ);
		ਜੇ (!commit)
			die(_("unable to access commit %s"),
			    oid_ਮੰਜ਼ਿਲ_hex(ਮਿਲਾਓ_ਸਿਰ));

		verify_ਮਿਲਾਓ_signature(commit, opt_verbosity,
				       check_trust_level);
	}

	/*
	 * Two-way merge: we treat the index as based on an empty tree,
	 * and try to fast-forward to HEAD. This ensures we will not lose
	 * index/worktree changes that the user already made on the unborn
	 * branch.
	 */
	ਜੇ (checkout_fast_forward(the_repository,
				  the_hash_algo->ਖਾਲੀ_ਥਾਵਾਂ_ਰੁੱਖ,
				  ਮਿਲਾਓ_ਸਿਰ, 0))
		ਵਾਪਸ 1;

	ਜੇ (refs_update_ref(get_main_ref_store(the_repository), "initial pull", "HEAD", ਮਿਲਾਓ_ਸਿਰ, curr_ਸਿਰ, 0, UPDATE_REFS_DIE_ON_ERR))
		ਵਾਪਸ 1;

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ rebase_submodules(ਖਾਲੀ)
{
	ਬਣਤਰ ਬੱਚਾ_process cp = ਬੱਚਾ_PROCESS_INIT;

	cp.git_cmd = 1;
	cp.no_stdin = 1;
	strvec_pushl(&cp.ਦਲੀਲਾਂ, "submodule", "update",
		     "--recursive", "--rebase", ਨਿਰਾ);
	argv_ਧੱਕੋ_verbosity(&cp.ਦਲੀਲਾਂ);

	ਵਾਪਸ ਲੜੀਵਾਰ_command(&cp);
}

ਸਥਿਰ ਪੂਰਨ update_submodules(ਖਾਲੀ)
{
	ਬਣਤਰ ਬੱਚਾ_process cp = ਬੱਚਾ_PROCESS_INIT;

	cp.git_cmd = 1;
	cp.no_stdin = 1;
	strvec_pushl(&cp.ਦਲੀਲਾਂ, "submodule", "update",
		     "--recursive", "--checkout", ਨਿਰਾ);
	argv_ਧੱਕੋ_verbosity(&cp.ਦਲੀਲਾਂ);

	ਵਾਪਸ ਲੜੀਵਾਰ_command(&cp);
}

/**
 * Runs git-merge, returning its exit status.
 */
ਸਥਿਰ ਪੂਰਨ ਲੜੀਵਾਰ_ਮਿਲਾਓ(ਖਾਲੀ)
{
	ਬਣਤਰ ਬੱਚਾ_process cmd = ਬੱਚਾ_PROCESS_INIT;

	strvec_pushl(&cmd.ਦਲੀਲਾਂ, "merge", ਨਿਰਾ);

	/* Shared options */
	argv_ਧੱਕੋ_verbosity(&cmd.ਦਲੀਲਾਂ);
	ਜੇ (opt_ਤਰੱਕੀ)
		strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, opt_ਤਰੱਕੀ);

	/* Options passed to git-merge */
	ਜੇ (opt_diffstat)
		strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, opt_diffstat);
	ਜੇ (opt_log)
		strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, opt_log);
	ਜੇ (opt_signoff)
		strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, opt_signoff);
	ਜੇ (opt_squash)
		strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, opt_squash);
	ਜੇ (opt_commit)
		strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, opt_commit);
	ਜੇ (opt_edit)
		strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, opt_edit);
	ਜੇ (cleanup_arg)
		strvec_pushf(&cmd.ਦਲੀਲਾਂ, "--cleanup=%s", cleanup_arg);
	ਜੇ (opt_ff)
		strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, opt_ff);
	ਜੇ (opt_verify)
		strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, opt_verify);
	ਜੇ (opt_verify_signatures)
		strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, opt_verify_signatures);
	strvec_pushv(&cmd.ਦਲੀਲਾਂ, opt_strategies.v);
	strvec_pushv(&cmd.ਦਲੀਲਾਂ, opt_strategy_opts.v);
	ਜੇ (opt_gpg_sign)
		strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, opt_gpg_sign);
	ਜੇ (opt_autostash == 0)
		strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, "--no-autostash");
	ਨਹੀਂਤਾਂ ਜੇ (opt_autostash == 1)
		strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, "--autostash");
	ਜੇ (opt_allow_unrelated_histories > 0)
		strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, "--allow-unrelated-histories");

	strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, "FETCH_HEAD");
	cmd.git_cmd = 1;
	ਵਾਪਸ ਲੜੀਵਾਰ_command(&cmd);
}

/**
 * Returns remote's upstream branch for the current branch. If remote is NULL,
 * the current branch's configured default remote is used. Returns NULL if
 * `remote` does not name a valid remote, HEAD does not point to a branch,
 * remote is not the branch's configured remote or the branch does not have any
 * configured upstream branch.
 */
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *get_upstream_branch(ਅਚਲ ਅੱਖਰ *remote)
{
	ਬਣਤਰ remote *rm;
	ਬਣਤਰ branch *curr_branch;
	ਅਚਲ ਅੱਖਰ *curr_branch_remote;

	rm = remote_get(remote);
	ਜੇ (!rm)
		ਵਾਪਸ ਨਿਰਾ;

	curr_branch = branch_get("HEAD");
	ਜੇ (!curr_branch)
		ਵਾਪਸ ਨਿਰਾ;

	curr_branch_remote = remote_for_branch(curr_branch, ਨਿਰਾ);
	ਪ੍ਰਤਿਜ੍ਞਾ(curr_branch_remote);

	ਜੇ (strcmp(curr_branch_remote, rm->name))
		ਵਾਪਸ ਨਿਰਾ;

	ਵਾਪਸ branch_get_upstream(curr_branch, ਨਿਰਾ);
}

/**
 * Derives the remote-tracking branch from the remote and refspec.
 *
 * FIXME: The current implementation assumes the default mapping of
 * refs/heads/<branch_name> to refs/remotes/<remote_name>/<branch_name>.
 */
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *get_tracking_branch(ਅਚਲ ਅੱਖਰ *remote, ਅਚਲ ਅੱਖਰ *refspec)
{
	ਬਣਤਰ refspec_ਵਸਤੂ spec;
	ਅਚਲ ਅੱਖਰ *spec_src;
	ਅਚਲ ਅੱਖਰ *ਮਿਲਾਓ_branch;

	ਜੇ (!refspec_ਵਸਤੂ_init_fetch(&spec, refspec, the_hash_algo))
		die(_("invalid refspec '%s'"), refspec);
	spec_src = spec.src;
	ਜੇ (!*spec_src || !strcmp(spec_src, "HEAD"))
		spec_src = "HEAD";
	ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(spec_src, "heads/", &spec_src))
		;
	ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(spec_src, "refs/heads/", &spec_src))
		;
	ਨਹੀਂਤਾਂ ਜੇ (starts_with(spec_src, "refs/") ||
		starts_with(spec_src, "tags/") ||
		starts_with(spec_src, "remotes/"))
		spec_src = "";

	ਜੇ (*spec_src) {
		ਜੇ (!strcmp(remote, "."))
			ਮਿਲਾਓ_branch = mkpath("refs/heads/%s", spec_src);
		ਨਹੀਂਤਾਂ
			ਮਿਲਾਓ_branch = mkpath("refs/remotes/%s/%s", remote, spec_src);
	} ਨਹੀਂਤਾਂ
		ਮਿਲਾਓ_branch = ਨਿਰਾ;

	refspec_ਵਸਤੂ_clear(&spec);
	ਵਾਪਸ ਮਿਲਾਓ_branch;
}

/**
 * Given the repo and refspecs, sets fork_point to the point at which the
 * current branch forked from its remote-tracking branch. Returns 0 on success,
 * -1 on failure.
 */
ਸਥਿਰ ਪੂਰਨ get_rebase_fork_ਬਿੰਦੂ(ਬਣਤਰ object_id *fork_ਬਿੰਦੂ, ਅਚਲ ਅੱਖਰ *repo,
		ਅਚਲ ਅੱਖਰ *refspec)
{
	ਪੂਰਨ ret;
	ਬਣਤਰ branch *curr_branch;
	ਅਚਲ ਅੱਖਰ *remote_branch;
	ਬਣਤਰ ਬੱਚਾ_process cp = ਬੱਚਾ_PROCESS_INIT;
	ਬਣਤਰ strbuf sb = STRBUF_INIT;

	curr_branch = branch_get("HEAD");
	ਜੇ (!curr_branch)
		ਵਾਪਸ -1;

	ਜੇ (refspec)
		remote_branch = get_tracking_branch(repo, refspec);
	ਨਹੀਂਤਾਂ
		remote_branch = get_upstream_branch(repo);

	ਜੇ (!remote_branch)
		ਵਾਪਸ -1;

	strvec_pushl(&cp.ਦਲੀਲਾਂ, "merge-base", "--fork-point",
		     remote_branch, curr_branch->name, ਨਿਰਾ);
	cp.no_stdin = 1;
	cp.no_stderr = 1;
	cp.git_cmd = 1;

	ret = capture_command(&cp, &sb, GIT_MAX_HEXSZ);
	ਜੇ (ret)
		ਜਾਓ cleanup;

	ret = get_oid_hex(sb.buf, fork_ਬਿੰਦੂ);
	ਜੇ (ret)
		ਜਾਓ cleanup;

cleanup:
	strbuf_release(&sb);
	ਵਾਪਸ ret ? -1 : 0;
}

/**
 * Sets merge_base to the octopus merge base of curr_head, merge_head and
 * fork_point. Returns 0 if a merge base is found, 1 otherwise.
 */
ਸਥਿਰ ਪੂਰਨ get_octopus_ਮਿਲਾਓ_ਅਧਾਰ(ਬਣਤਰ object_id *ਮਿਲਾਓ_ਅਧਾਰ,
		ਅਚਲ ਬਣਤਰ object_id *curr_ਸਿਰ,
		ਅਚਲ ਬਣਤਰ object_id *ਮਿਲਾਓ_ਸਿਰ,
		ਅਚਲ ਬਣਤਰ object_id *fork_ਬਿੰਦੂ)
{
	ਬਣਤਰ commit_list *revs = ਨਿਰਾ, *ਨਤੀਜਾ = ਨਿਰਾ;

	commit_list_ਪਾਓ(lookup_commit_reference(the_repository, curr_ਸਿਰ),
			   &revs);
	commit_list_ਪਾਓ(lookup_commit_reference(the_repository, ਮਿਲਾਓ_ਸਿਰ),
			   &revs);
	ਜੇ (!is_null_oid(fork_ਬਿੰਦੂ))
		commit_list_ਪਾਓ(lookup_commit_reference(the_repository, fork_ਬਿੰਦੂ),
				   &revs);

	ਜੇ (get_octopus_ਮਿਲਾਓ_bases(revs, &ਨਤੀਜਾ) < 0)
		ਬਾਹਰ_ਨਿਕਲੋ(128);
	commit_list_free(revs);
	reduce_heads_replace(&ਨਤੀਜਾ);

	ਜੇ (!ਨਤੀਜਾ)
		ਵਾਪਸ 1;

	oidcpy(ਮਿਲਾਓ_ਅਧਾਰ, &ਨਤੀਜਾ->ਵਸਤੂ->object.oid);
	commit_list_free(ਨਤੀਜਾ);
	ਵਾਪਸ 0;
}

/**
 * Given the current HEAD oid, the merge head returned from git-fetch and the
 * fork point calculated by get_rebase_fork_point(), compute the <newbase> and
 * <upstream> arguments to use for the upcoming git-rebase invocation.
 */
ਸਥਿਰ ਪੂਰਨ get_rebase_newbase_and_upstream(ਬਣਤਰ object_id *newbase,
		ਬਣਤਰ object_id *upstream,
		ਅਚਲ ਬਣਤਰ object_id *curr_ਸਿਰ,
		ਅਚਲ ਬਣਤਰ object_id *ਮਿਲਾਓ_ਸਿਰ,
		ਅਚਲ ਬਣਤਰ object_id *fork_ਬਿੰਦੂ)
{
	ਬਣਤਰ object_id oct_ਮਿਲਾਓ_ਅਧਾਰ;

	ਜੇ (!get_octopus_ਮਿਲਾਓ_ਅਧਾਰ(&oct_ਮਿਲਾਓ_ਅਧਾਰ, curr_ਸਿਰ, ਮਿਲਾਓ_ਸਿਰ, fork_ਬਿੰਦੂ))
		ਜੇ (!is_null_oid(fork_ਬਿੰਦੂ) && oideq(&oct_ਮਿਲਾਓ_ਅਧਾਰ, fork_ਬਿੰਦੂ))
			fork_ਬਿੰਦੂ = ਨਿਰਾ;

	ਜੇ (fork_ਬਿੰਦੂ && !is_null_oid(fork_ਬਿੰਦੂ))
		oidcpy(upstream, fork_ਬਿੰਦੂ);
	ਨਹੀਂਤਾਂ
		oidcpy(upstream, ਮਿਲਾਓ_ਸਿਰ);

	oidcpy(newbase, ਮਿਲਾਓ_ਸਿਰ);

	ਵਾਪਸ 0;
}

/**
 * Given the <newbase> and <upstream> calculated by
 * get_rebase_newbase_and_upstream(), runs git-rebase with the
 * appropriate arguments and returns its exit status.
 */
ਸਥਿਰ ਪੂਰਨ ਲੜੀਵਾਰ_rebase(ਅਚਲ ਬਣਤਰ object_id *newbase,
		ਅਚਲ ਬਣਤਰ object_id *upstream)
{
	ਬਣਤਰ ਬੱਚਾ_process cmd = ਬੱਚਾ_PROCESS_INIT;

	strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, "rebase");

	/* Shared options */
	argv_ਧੱਕੋ_verbosity(&cmd.ਦਲੀਲਾਂ);

	/* Options passed to git-rebase */
	ਜੇ (opt_rebase == REBASE_MERGES)
		strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, "--rebase-merges");
	ਨਹੀਂਤਾਂ ਜੇ (opt_rebase == REBASE_INTERACTIVE)
		strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, "--interactive");
	ਜੇ (opt_diffstat)
		strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, opt_diffstat);
	strvec_pushv(&cmd.ਦਲੀਲਾਂ, opt_strategies.v);
	strvec_pushv(&cmd.ਦਲੀਲਾਂ, opt_strategy_opts.v);
	ਜੇ (opt_gpg_sign)
		strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, opt_gpg_sign);
	ਜੇ (opt_signoff)
		strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, opt_signoff);
	ਜੇ (opt_autostash == 0)
		strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, "--no-autostash");
	ਨਹੀਂਤਾਂ ਜੇ (opt_autostash == 1)
		strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, "--autostash");
	ਜੇ (opt_verify_signatures &&
	    !strcmp(opt_verify_signatures, "--verify-signatures"))
		warning(_("ignoring --verify-signatures for rebase"));

	strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, "--onto");
	strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, oid_ਮੰਜ਼ਿਲ_hex(newbase));

	strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, oid_ਮੰਜ਼ਿਲ_hex(upstream));

	cmd.git_cmd = 1;
	ਵਾਪਸ ਲੜੀਵਾਰ_command(&cmd);
}

ਸਥਿਰ ਪੂਰਨ get_can_ff(ਬਣਤਰ object_id *orig_ਸਿਰ,
		      ਬਣਤਰ oid_array *ਮਿਲਾਓ_heads)
{
	ਪੂਰਨ ret;
	ਬਣਤਰ commit_list *list = ਨਿਰਾ;
	ਬਣਤਰ commit *ਮਿਲਾਓ_ਸਿਰ, *ਸਿਰ;
	ਬਣਤਰ object_id *orig_ਮਿਲਾਓ_ਸਿਰ;

	ਜੇ (ਮਿਲਾਓ_heads->nr > 1)
		ਵਾਪਸ 0;

	orig_ਮਿਲਾਓ_ਸਿਰ = &ਮਿਲਾਓ_heads->oid[0];
	ਸਿਰ = lookup_commit_reference(the_repository, orig_ਸਿਰ);
	ਜੇ (!ਸਿਰ)
		ਵਾਪਸ 0;
	ਮਿਲਾਓ_ਸਿਰ = lookup_commit_reference(the_repository, orig_ਮਿਲਾਓ_ਸਿਰ);
	ਜੇ (!ਮਿਲਾਓ_ਸਿਰ)
		ਵਾਪਸ 0;
	commit_list_ਪਾਓ(ਸਿਰ, &list);
	ret = repo_is_descendant_ਦਾ(the_repository, ਮਿਲਾਓ_ਸਿਰ, list);
	commit_list_free(list);
	ਜੇ (ret < 0)
		ਬਾਹਰ_ਨਿਕਲੋ(128);
	ਵਾਪਸ ret;
}

/*
 * Is orig_head a descendant of _all_ merge_heads?
 * Unfortunately is_descendant_of() cannot be used as it asks
 * if orig_head is a descendant of at least one of them.
 */
ਸਥਿਰ ਪੂਰਨ already_ਉੱਪਰ_ਮੰਜ਼ਿਲ_date(ਬਣਤਰ object_id *orig_ਸਿਰ,
			      ਬਣਤਰ oid_array *ਮਿਲਾਓ_heads)
{
	ਬਣਤਰ commit *ours;

	ours = lookup_commit_reference(the_repository, orig_ਸਿਰ);
	ਜੇ (!ours)
		ਵਾਪਸ 0;
	ਲਈ (ਮਾਪ_t i = 0; i < ਮਿਲਾਓ_heads->nr; i++) {
		ਬਣਤਰ commit_list *list = ਨਿਰਾ;
		ਬਣਤਰ commit *theirs;
		ਪੂਰਨ ok;

		theirs = lookup_commit_reference(the_repository, &ਮਿਲਾਓ_heads->oid[i]);
		ਜੇ (!theirs)
			ਵਾਪਸ 0;
		commit_list_ਪਾਓ(theirs, &list);
		ok = repo_is_descendant_ਦਾ(the_repository, ours, list);
		commit_list_free(list);
		ਜੇ (ok < 0)
			ਬਾਹਰ_ਨਿਕਲੋ(128);
		ਜੇ (!ok)
			ਵਾਪਸ 0;
	}
	ਵਾਪਸ 1;
}

ਸਥਿਰ ਖਾਲੀ show_advice_pull_non_ff(ਖਾਲੀ)
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

ਪੂਰਨ cmd_pull(ਪੂਰਨ argc,
	     ਅਚਲ ਅੱਖਰ **argv,
	     ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
	     ਬਣਤਰ repository *repository ਅਣਵਰਤਿਆ)
{
	ਅਚਲ ਅੱਖਰ *repo, **refspecs;
	ਬਣਤਰ oid_array ਮਿਲਾਓ_heads = OID_ARRAY_INIT;
	ਬਣਤਰ object_id orig_ਸਿਰ, curr_ਸਿਰ;
	ਬਣਤਰ object_id rebase_fork_ਬਿੰਦੂ;
	ਪੂਰਨ rebase_unspecified = 0;
	ਪੂਰਨ can_ff;
	ਪੂਰਨ divergent;
	ਪੂਰਨ ret;
	ਸਥਿਰ ਬਣਤਰ option pull_options[] = {
		/* Shared options */
		OPT__VERBOSITY(&opt_verbosity),
		OPT_PASSTHRU(0, "progress", &opt_ਤਰੱਕੀ, ਨਿਰਾ,
			N_("force progress reporting"),
			PARSE_OPT_NOARG),
		OPT_CALLBACK_F(0, "recurse-submodules",
			   &recurse_submodules_cli, N_("on-demand"),
			   N_("control for recursive fetching of submodules"),
			   PARSE_OPT_OPTARG, option_fetch_parse_recurse_submodules),

		/* Options passed to git-merge or git-rebase */
		OPT_ਸਮੂਹ(N_("Options related to merging")),
		OPT_CALLBACK_F('r', "rebase", &opt_rebase,
			"(false|true|merges|interactive)",
			N_("incorporate changes by rebasing rather than merging"),
			PARSE_OPT_OPTARG, parse_opt_rebase),
		OPT_PASSTHRU('n', ਨਿਰਾ, &opt_diffstat, ਨਿਰਾ,
			N_("do not show a diffstat at the end of the merge"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG),
		OPT_PASSTHRU(0, "stat", &opt_diffstat, ਨਿਰਾ,
			N_("show a diffstat at the end of the merge"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "summary", &opt_diffstat, ਨਿਰਾ,
			N_("(synonym to --stat)"),
			PARSE_OPT_NOARG | PARSE_OPT_HIDDEN),
		OPT_PASSTHRU(0, "compact-summary", &opt_diffstat, ਨਿਰਾ,
			N_("show a compact-summary at the end of the merge"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "log", &opt_log, N_("n"),
			N_("add (at most <n>) entries from shortlog to merge commit message"),
			PARSE_OPT_OPTARG),
		OPT_PASSTHRU(0, "signoff", &opt_signoff, ਨਿਰਾ,
			N_("add a Signed-off-by trailer"),
			PARSE_OPT_OPTARG),
		OPT_PASSTHRU(0, "squash", &opt_squash, ਨਿਰਾ,
			N_("create a single commit instead of doing a merge"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "commit", &opt_commit, ਨਿਰਾ,
			N_("perform a commit if the merge succeeds (default)"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "edit", &opt_edit, ਨਿਰਾ,
			N_("edit message before committing"),
			PARSE_OPT_NOARG),
		OPT_CLEANUP(&cleanup_arg),
		OPT_PASSTHRU(0, "ff", &opt_ff, ਨਿਰਾ,
			N_("allow fast-forward"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "ff-only", &opt_ff, ਨਿਰਾ,
			N_("abort if fast-forward is not possible"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG),
		OPT_PASSTHRU(0, "verify", &opt_verify, ਨਿਰਾ,
			N_("control use of pre-merge-commit and commit-msg hooks"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "verify-signatures", &opt_verify_signatures, ਨਿਰਾ,
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
		OPT_ਸੈੱਟ_INT(0, "allow-unrelated-histories",
			    &opt_allow_unrelated_histories,
			    N_("allow merging unrelated histories"), 1),

		/* Options passed to git-fetch */
		OPT_ਸਮੂਹ(N_("Options related to fetching")),
		OPT_PASSTHRU(0, "all", &opt_all, ਨਿਰਾ,
			N_("fetch from all remotes"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU('a', "append", &opt_append, ਨਿਰਾ,
			N_("append to .git/FETCH_HEAD instead of overwriting"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "upload-pack", &opt_upload_pack, N_("path"),
			N_("path to upload pack on remote end"),
			0),
		OPT__FORCE(&opt_force, N_("force overwrite of local branch"), 0),
		OPT_PASSTHRU('t', "tags", &opt_ਟੈਗ_ਸੂਚੀ, ਨਿਰਾ,
			N_("fetch all tags and associated objects"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU('p', "prune", &opt_prune, ਨਿਰਾ,
			N_("prune remote-tracking branches no longer on remote"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU('j', "jobs", &max_children, N_("n"),
			N_("number of submodules pulled in parallel"),
			PARSE_OPT_OPTARG),
		OPT_BOOL(0, "dry-run", &opt_dry_ਲੜੀਵਾਰ,
			N_("dry run")),
		OPT_PASSTHRU('k', "keep", &opt_keep, ਨਿਰਾ,
			N_("keep downloaded pack"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "depth", &opt_ਡੂੰਘਾਈ, N_("depth"),
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
		OPT_PASSTHRU(0, "unshallow", &opt_unshallow, ਨਿਰਾ,
			N_("convert to a complete repository"),
			PARSE_OPT_NONEG | PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "update-shallow", &opt_update_shallow, ਨਿਰਾ,
			N_("accept refs that update .git/shallow"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "refmap", &opt_refmap, N_("refmap"),
			N_("specify fetch refmap"),
			PARSE_OPT_NONEG),
		OPT_PASSTHRU_ARGV('o', "server-option", &opt_fetch,
			N_("server-specific"),
			N_("option to transmit"),
			0),
		OPT_PASSTHRU('4',  "ipv4", &opt_ipv4, ਨਿਰਾ,
			N_("use IPv4 addresses only"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU('6',  "ipv6", &opt_ipv6, ਨਿਰਾ,
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
		OPT_PASSTHRU(0, "set-upstream", &ਸੈੱਟ_upstream, ਨਿਰਾ,
			N_("set upstream for git pull/fetch"),
			PARSE_OPT_NOARG),

		OPT_ਸਿਰਾ()
	};

	ਜੇ (!getenv("GIT_REFLOG_ACTION"))
		ਸੈੱਟ_reflog_ਸੁਨੇਹਾ(argc, argv);

	repo_config(the_repository, git_pull_config, ਨਿਰਾ);
	ਜੇ (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_ਭਰਿਆ_ਸੂਚਕਾਂਕ = 0;
	}

	argc = parse_options(argc, argv, ਅਗੇਤਰ, pull_options, pull_usage, 0);
	ਜੇ (opt_autostash == -1)
		opt_autostash = config_pull_autostash;

	ਜੇ (recurse_submodules_cli != RECURSE_SUBMODULES_DEFAULT)
		recurse_submodules = recurse_submodules_cli;

	ਜੇ (cleanup_arg)
		/*
		 * this only checks the validity of cleanup_arg; we don't need
		 * a valid value for use_editor
		 */
		get_cleanup_ਬਹੁਲਕ(cleanup_arg, 0);

	parse_repo_refspecs(argc, argv, &repo, &refspecs);

	ਜੇ (!opt_ff) {
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
		ਜੇ (opt_rebase >= 0 && opt_ff && !strcmp(opt_ff, "--ff-only")) {
			ਮੁਕਤ(opt_ff);
			opt_ff = xstrdup("--ff");
		}
	}

	ਜੇ (opt_rebase < 0)
		opt_rebase = config_get_rebase(&rebase_unspecified);

	ਜੇ (repo_read_ਸੂਚਕਾਂਕ_unmerged(the_repository))
		die_resolve_conflict("pull");

	ਜੇ (file_exists(git_ਰਸਤਾ_ਮਿਲਾਓ_ਸਿਰ(the_repository)))
		die_conclude_ਮਿਲਾਓ();

	ਜੇ (repo_get_oid(the_repository, "HEAD", &orig_ਸਿਰ))
		oidclr(&orig_ਸਿਰ, the_repository->hash_algo);

	ਜੇ (opt_rebase) {
		ਜੇ (opt_autostash == -1)
			opt_autostash = config_rebase_autostash;

		ਜੇ (is_null_oid(&orig_ਸਿਰ) && !is_ਸੂਚਕਾਂਕ_unborn(the_repository->ਸੂਚਕਾਂਕ))
			die(_("Updating an unborn branch with changes added to the index."));

		ਜੇ (!opt_autostash)
			require_clean_work_ਰੁੱਖ(the_repository,
				N_("pull with rebase"),
				_("Please commit or stash them."), 1, 0);

		ਜੇ (get_rebase_fork_ਬਿੰਦੂ(&rebase_fork_ਬਿੰਦੂ, repo, *refspecs))
			oidclr(&rebase_fork_ਬਿੰਦੂ, the_repository->hash_algo);
	}

	ਜੇ (ਲੜੀਵਾਰ_fetch(repo, refspecs))
		ਵਾਪਸ 1;

	ਜੇ (opt_dry_ਲੜੀਵਾਰ)
		ਵਾਪਸ 0;

	ਜੇ (repo_get_oid(the_repository, "HEAD", &curr_ਸਿਰ))
		oidclr(&curr_ਸਿਰ, the_repository->hash_algo);

	ਜੇ (!is_null_oid(&orig_ਸਿਰ) && !is_null_oid(&curr_ਸਿਰ) &&
			!oideq(&orig_ਸਿਰ, &curr_ਸਿਰ)) {
		/*
		 * The fetch involved updating the current branch.
		 *
		 * The working tree and the index file are still based on
		 * orig_head commit, but we are merging into curr_head.
		 * Update the working tree to match curr_head.
		 */

		warning(_("fetch updated the current branch head.\n"
			"fast-forwarding your working tree from\n"
			"commit %s."), oid_ਮੰਜ਼ਿਲ_hex(&orig_ਸਿਰ));

		ਜੇ (checkout_fast_forward(the_repository, &orig_ਸਿਰ,
					  &curr_ਸਿਰ, 0))
			die(_("Cannot fast-forward your working tree.\n"
				"After making sure that you saved anything precious from\n"
				"$ git diff %s\n"
				"output, run\n"
				"$ git reset --hard\n"
				"to recover."), oid_ਮੰਜ਼ਿਲ_hex(&orig_ਸਿਰ));
	}

	get_ਮਿਲਾਓ_heads(&ਮਿਲਾਓ_heads);

	ਜੇ (!ਮਿਲਾਓ_heads.nr)
		die_no_ਮਿਲਾਓ_candidates(repo, refspecs);

	ਜੇ (is_null_oid(&orig_ਸਿਰ)) {
		ਜੇ (ਮਿਲਾਓ_heads.nr > 1)
			die(_("Cannot merge multiple branches into empty head."));
		ret = pull_into_void(ਮਿਲਾਓ_heads.oid, &curr_ਸਿਰ);
		ਜਾਓ cleanup;
	}
	ਜੇ (ਮਿਲਾਓ_heads.nr > 1) {
		ਜੇ (opt_rebase)
			die(_("Cannot rebase onto multiple branches."));
		ਜੇ (opt_ff && !strcmp(opt_ff, "--ff-only"))
			die(_("Cannot fast-forward to multiple branches."));
	}

	can_ff = get_can_ff(&orig_ਸਿਰ, &ਮਿਲਾਓ_heads);
	divergent = !can_ff && !already_ਉੱਪਰ_ਮੰਜ਼ਿਲ_date(&orig_ਸਿਰ, &ਮਿਲਾਓ_heads);

	/* ff-only takes precedence over rebase */
	ਜੇ (opt_ff && !strcmp(opt_ff, "--ff-only")) {
		ਜੇ (divergent)
			die_ff_impossible();
		opt_rebase = REBASE_FALSE;
	}
	/* If no action specified and we can't fast forward, then warn. */
	ਜੇ (!opt_ff && rebase_unspecified && divergent) {
		show_advice_pull_non_ff();
		die(_("Need to specify how to reconcile divergent branches."));
	}

	ਜੇ (opt_rebase) {
		ਬਣਤਰ object_id newbase;
		ਬਣਤਰ object_id upstream;
		get_rebase_newbase_and_upstream(&newbase, &upstream, &curr_ਸਿਰ,
						ਮਿਲਾਓ_heads.oid, &rebase_fork_ਬਿੰਦੂ);

		ਜੇ ((recurse_submodules == RECURSE_SUBMODULES_ON ||
		     recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND) &&
		    submodule_touches_ਵਿੱਚ_range(the_repository, &upstream, &curr_ਸਿਰ))
			die(_("cannot rebase with locally recorded submodule modifications"));

		ਜੇ (can_ff) {
			/* we can fast-forward this without invoking rebase */
			ਮੁਕਤ(opt_ff);
			opt_ff = xstrdup("--ff-only");
			ret = ਲੜੀਵਾਰ_ਮਿਲਾਓ();
		} ਨਹੀਂਤਾਂ {
			ret = ਲੜੀਵਾਰ_rebase(&newbase, &upstream);
		}

		ਜੇ (!ret && (recurse_submodules == RECURSE_SUBMODULES_ON ||
			     recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND))
			ret = rebase_submodules();

		ਜਾਓ cleanup;
	} ਨਹੀਂਤਾਂ {
		ret = ਲੜੀਵਾਰ_ਮਿਲਾਓ();
		ਜੇ (!ret && (recurse_submodules == RECURSE_SUBMODULES_ON ||
			     recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND))
			ret = update_submodules();
		ਜਾਓ cleanup;
	}

cleanup:
	oid_array_clear(&ਮਿਲਾਓ_heads);
	ਵਾਪਸ ret;
}
