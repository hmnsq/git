/*
 * Builtin "git pull"
 *
 * Based on git-pull.sh by Junio C Hamano
 *
 * Fetch one or more remote refs and merge it/them into the current HEAD.
 */

#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "advice.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "merge.h"
#ಅನ್ತರ್ಭಾವ "object-name.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "run-command.h"
#ಅನ್ತರ್ಭಾವ "oid-array.h"
#ಅನ್ತರ್ಭಾವ "remote.h"
#ಅನ್ತರ್ಭಾವ "dir.h"
#ಅನ್ತರ್ಭಾವ "path.h"
#ಅನ್ತರ್ಭಾವ "read-cache-ll.h"
#ಅನ್ತರ್ಭಾವ "rebase.h"
#ಅನ್ತರ್ಭಾವ "refs.h"
#ಅನ್ತರ್ಭಾವ "refspec.h"
#ಅನ್ತರ್ಭಾವ "submodule.h"
#ಅನ್ತರ್ಭಾವ "submodule-config.h"
#ಅನ್ತರ್ಭಾವ "wt-status.h"
#ಅನ್ತರ್ಭಾವ "commit-reach.h"
#ಅನ್ತರ್ಭಾವ "sequencer.h"

/**
 * Parses the value of --rebase. If value is a false value, returns
 * REBASE_FALSE. If value is a true value, returns REBASE_TRUE. If value is
 * "merges", returns REBASE_MERGES. If value is a invalid value, dies with
 * a fatal error if fatal is true, otherwise returns REBASE_INVALID.
 */
ಸ್ಥಿರ ಎಣಿಕೆ rebase_type parse_config_rebase(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೀಲಿ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ,
		ಪೂರ್ಣಾಂಕ fatal)
{
	ಎಣಿಕೆ rebase_type v = rebase_parse_ಮೌಲ್ಯ(ಮೌಲ್ಯ);
	ಆದರೆ (v != REBASE_INVALID)
		ಹಿಂತಿರುಗಿ v;

	ಆದರೆ (fatal)
		die(_("invalid value for '%s': '%s'"), ಕೀಲಿ, ಮೌಲ್ಯ);
	ಇಲ್ಲದಿದ್ದರೆ
		error(_("invalid value for '%s': '%s'"), ಕೀಲಿ, ಮೌಲ್ಯ);

	ಹಿಂತಿರುಗಿ REBASE_INVALID;
}

/**
 * Callback for --rebase, which parses arg with parse_config_rebase().
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ parse_opt_rebase(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ಎಣಿಕೆ rebase_type *ಮೌಲ್ಯ = opt->ಮೌಲ್ಯ;

	ಆದರೆ (arg)
		*ಮೌಲ್ಯ = parse_config_rebase("--rebase", arg, 0);
	ಇಲ್ಲದಿದ್ದರೆ
		*ಮೌಲ್ಯ = unset ? REBASE_FALSE : REBASE_TRUE;
	ಹಿಂತಿರುಗಿ *ಮೌಲ್ಯ == REBASE_INVALID ? -1 : 0;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ pull_usage[] = {
	N_("git pull [<options>] [<repository> [<refspec>...]]"),
	ಖಾಲಿ
};

/* Shared options */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ opt_verbosity;
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *opt_ಪ್ರಗತಿ;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ recurse_submodules = RECURSE_SUBMODULES_DEFAULT;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ recurse_submodules_cli = RECURSE_SUBMODULES_DEFAULT;

/* Options passed to git-merge or git-rebase */
ಸ್ಥಿರ ಎಣಿಕೆ rebase_type opt_rebase = -1;
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *opt_diffstat;
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *opt_log;
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *opt_signoff;
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *opt_squash;
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *opt_commit;
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *opt_edit;
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cleanup_arg;
ಸ್ಥಿರ ಅಕ್ಷರ *opt_ff;
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *opt_verify_signatures;
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *opt_verify;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ opt_autostash = -1;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ config_rebase_autostash;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ config_pull_autostash = -1;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ check_trust_level = 1;
ಸ್ಥಿರ ರಚನೆ strvec opt_strategies = STRVEC_INIT;
ಸ್ಥಿರ ರಚನೆ strvec opt_strategy_opts = STRVEC_INIT;
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *opt_gpg_sign;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ opt_allow_unrelated_histories;

/* Options passed to git-fetch */
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *opt_all;
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *opt_append;
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *opt_upload_pack;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ opt_force;
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *opt_ಟ್ಯಾಗುಗಳು;
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *opt_prune;
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *max_children;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ opt_dry_ಸತತ;
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *opt_keep;
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *opt_ಆಳ;
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *opt_unshallow;
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *opt_update_shallow;
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *opt_refmap;
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *opt_ipv4;
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *opt_ipv6;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ opt_show_forced_updates = -1;
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಗಣ_upstream;
ಸ್ಥಿರ ರಚನೆ strvec opt_fetch = STRVEC_INIT;

/**
 * Pushes "-q" or "-v" switches into arr to match the opt_verbosity level.
 */
ಸ್ಥಿರ ಶೂನ್ಯ argv_ತಳ್ಳು_verbosity(ರಚನೆ strvec *arr)
{
	ಪೂರ್ಣಾಂಕ verbosity;

	ಗಾಗಿ (verbosity = opt_verbosity; verbosity > 0; verbosity--)
		strvec_ತಳ್ಳು(arr, "-v");

	ಗಾಗಿ (verbosity = opt_verbosity; verbosity < 0; verbosity++)
		strvec_ತಳ್ಳು(arr, "-q");
}

/**
 * Pushes "-f" switches into arr to match the opt_force level.
 */
ಸ್ಥಿರ ಶೂನ್ಯ argv_ತಳ್ಳು_force(ರಚನೆ strvec *arr)
{
	ಪೂರ್ಣಾಂಕ force = opt_force;
	ಆಗ (force-- > 0)
		strvec_ತಳ್ಳು(arr, "-f");
}

/**
 * Sets the GIT_REFLOG_ACTION environment variable to the concatenation of argv
 */
ಸ್ಥಿರ ಶೂನ್ಯ ಗಣ_reflog_ಸಂದೇಶ(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv)
{
	ಪೂರ್ಣಾಂಕ i;
	ರಚನೆ strbuf ಸಂದೇಶ = STRBUF_INIT;

	ಗಾಗಿ (i = 0; i < argc; i++) {
		ಆದರೆ (i)
			strbuf_addch(&ಸಂದೇಶ, ' ');
		strbuf_addstr(&ಸಂದೇಶ, argv[i]);
	}

	setenv("GIT_REFLOG_ACTION", ಸಂದೇಶ.buf, 0);

	strbuf_release(&ಸಂದೇಶ);
}

/**
 * If pull.ff is unset, returns NULL. If pull.ff is "true", returns "--ff". If
 * pull.ff is "false", returns "--no-ff". If pull.ff is "only", returns
 * "--ff-only". Otherwise, if pull.ff is set to an invalid value, die with an
 * error.
 */
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *config_get_ff(ಶೂನ್ಯ)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ;

	ಆದರೆ (repo_config_get_ಮೌಲ್ಯ(the_repository, "pull.ff", &ಮೌಲ್ಯ))
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;

	ಆಯ್ಕೆ (git_parse_maybe_bool(ಮೌಲ್ಯ)) {
	ಸ್ಥಿತಿ 0:
		ಹಿಂತಿರುಗಿ "--no-ff";
	ಸ್ಥಿತಿ 1:
		ಹಿಂತಿರುಗಿ "--ff";
	}

	ಆದರೆ (!strcmp(ಮೌಲ್ಯ, "only"))
		ಹಿಂತಿರುಗಿ "--ff-only";

	die(_("invalid value for '%s': '%s'"), "pull.ff", ಮೌಲ್ಯ);
}

/**
 * Returns the default configured value for --rebase. It first looks for the
 * value of "branch.$curr_branch.rebase", where $curr_branch is the current
 * branch, and if HEAD is detached or the configuration key does not exist,
 * looks for the value of "pull.rebase". If both configuration keys do not
 * exist, returns REBASE_FALSE.
 */
ಸ್ಥಿರ ಎಣಿಕೆ rebase_type config_get_rebase(ಪೂರ್ಣಾಂಕ *rebase_unspecified)
{
	ರಚನೆ branch *curr_branch = branch_get("HEAD");
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ;

	ಆದರೆ (curr_branch) {
		ಅಕ್ಷರ *ಕೀಲಿ = xstrfmt("branch.%s.rebase", curr_branch->name);

		ಆದರೆ (!repo_config_get_ಮೌಲ್ಯ(the_repository, ಕೀಲಿ, &ಮೌಲ್ಯ)) {
			ಎಣಿಕೆ rebase_type ret = parse_config_rebase(ಕೀಲಿ, ಮೌಲ್ಯ, 1);
			ಬಿಡುಗಡೆ(ಕೀಲಿ);
			ಹಿಂತಿರುಗಿ ret;
		}

		ಬಿಡುಗಡೆ(ಕೀಲಿ);
	}

	ಆದರೆ (!repo_config_get_ಮೌಲ್ಯ(the_repository, "pull.rebase", &ಮೌಲ್ಯ))
		ಹಿಂತಿರುಗಿ parse_config_rebase("pull.rebase", ಮೌಲ್ಯ, 1);

	*rebase_unspecified = 1;

	ಹಿಂತಿರುಗಿ REBASE_FALSE;
}

/**
 * Read config variables.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ git_pull_config(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *var, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ,
			   ಸ್ಥಿರಾಂಕ ರಚನೆ config_context *ctx, ಶೂನ್ಯ *cb)
{
	ಆದರೆ (!strcmp(var, "rebase.autostash")) {
		/*
		 * run_rebase() also reads this option. The reason we handle it here is
		 * that when pull.rebase is true, a fast-forward may occur without
		 * invoking run_rebase(). We need to ensure that autostash is set even
		 * in the fast-forward case.
		 *
		 * run_merge() handles merge.autostash, so we don't handle it here.
		 */
		config_rebase_autostash = git_config_bool(var, ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(var, "pull.autostash")) {
		config_pull_autostash = git_config_bool(var, ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(var, "submodule.recurse")) {
		recurse_submodules = git_config_bool(var, ಮೌಲ್ಯ) ?
			RECURSE_SUBMODULES_ON : RECURSE_SUBMODULES_OFF;
		ಹಿಂತಿರುಗಿ 0;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(var, "gpg.mintrustlevel")) {
		check_trust_level = 0;
	}

	ಹಿಂತಿರುಗಿ git_default_config(var, ಮೌಲ್ಯ, ctx, cb);
}

/**
 * Appends merge candidates from FETCH_HEAD that are not marked not-for-merge
 * into merge_heads.
 */
ಸ್ಥಿರ ಶೂನ್ಯ get_ವಿಲೀನ_heads(ರಚನೆ oid_array *ವಿಲೀನ_heads)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *filename = git_ಮಾರ್ಗ_fetch_ತಲೆ(the_repository);
	FILE *fp;
	ರಚನೆ strbuf sb = STRBUF_INIT;
	ರಚನೆ object_id oid;

	fp = xfopen(filename, "r");
	ಆಗ (strbuf_getline_lf(&sb, fp) != EOF) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *p;
		ಆದರೆ (parse_oid_hex(sb.buf, &oid, &p))
			ಮುಂದುವರಿ;  /* invalid line: does not start with object ID */
		ಆದರೆ (starts_with(p, "\tnot-for-merge\t"))
			ಮುಂದುವರಿ;  /* ref is not-for-merge */
		oid_array_append(ವಿಲೀನ_heads, &oid);
	}
	fclose(fp);
	strbuf_release(&sb);
}

/**
 * Used by die_no_merge_candidates() as a for_each_remote() callback to
 * retrieve the name of the remote if the repository only has one remote.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ get_only_remote(ರಚನೆ remote *remote, ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **remote_name = cb_ದತ್ತಾಂಶ;

	ಆದರೆ (*remote_name)
		ಹಿಂತಿರುಗಿ -1;

	*remote_name = remote->name;
	ಹಿಂತಿರುಗಿ 0;
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
ಸ್ಥಿರ ಶೂನ್ಯ NORETURN die_no_ವಿಲೀನ_candidates(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *repo, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **refspecs)
{
	ರಚನೆ branch *curr_branch = branch_get("HEAD");
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *remote = curr_branch ? curr_branch->remote_name : ಖಾಲಿ;

	ಆದರೆ (*refspecs) {
		ಆದರೆ (opt_rebase)
			fprintf_ln(stderr, _("There is no candidate for rebasing against among the refs that you just fetched."));
		ಇಲ್ಲದಿದ್ದರೆ
			fprintf_ln(stderr, _("There are no candidates for merging among the refs that you just fetched."));
		fprintf_ln(stderr, _("Generally this means that you provided a wildcard refspec which had no\n"
					"matches on the remote end."));
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (repo && curr_branch && (!remote || strcmp(repo, remote))) {
		fprintf_ln(stderr, _("You asked to pull from the remote '%s', but did not specify\n"
			"a branch. Because this is not the default configured remote\n"
			"for your current branch, you must specify a branch on the command line."),
			repo);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!curr_branch) {
		fprintf_ln(stderr, _("You are not currently on a branch."));
		ಆದರೆ (opt_rebase)
			fprintf_ln(stderr, _("Please specify which branch you want to rebase against."));
		ಇಲ್ಲದಿದ್ದರೆ
			fprintf_ln(stderr, _("Please specify which branch you want to merge with."));
		fprintf_ln(stderr, _("See git-pull(1) for details."));
		fprintf(stderr, "\n");
		fprintf_ln(stderr, "    git pull %s %s", _("<remote>"), _("<branch>"));
		fprintf(stderr, "\n");
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!curr_branch->ವಿಲೀನ_nr) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *remote_name = ಖಾಲಿ;

		ಆದರೆ (for_each_remote(get_only_remote, &remote_name) || !remote_name)
			remote_name = _("<remote>");

		fprintf_ln(stderr, _("There is no tracking information for the current branch."));
		ಆದರೆ (opt_rebase)
			fprintf_ln(stderr, _("Please specify which branch you want to rebase against."));
		ಇಲ್ಲದಿದ್ದರೆ
			fprintf_ln(stderr, _("Please specify which branch you want to merge with."));
		fprintf_ln(stderr, _("See git-pull(1) for details."));
		fprintf(stderr, "\n");
		fprintf_ln(stderr, "    git pull %s %s", _("<remote>"), _("<branch>"));
		fprintf(stderr, "\n");
		fprintf_ln(stderr, _("If you wish to set tracking information for this branch you can do so with:"));
		fprintf(stderr, "\n");
		fprintf_ln(stderr, "    git branch --set-upstream-to=%s/%s %s\n",
				remote_name, _("<branch>"), curr_branch->name);
	} ಇಲ್ಲದಿದ್ದರೆ
		fprintf_ln(stderr, _("Your configuration specifies to merge with the ref '%s'\n"
			"from the remote, but no such ref was fetched."),
			curr_branch->ವಿಲೀನ[0]->src);
	ನಿರ್ಗಮಿಸು(1);
}

/**
 * Parses argv into [<repo> [<refspecs>...]], returning their values in `repo`
 * as a string and `refspecs` as a null-terminated array of strings. If `repo`
 * is not provided in argv, it is set to NULL.
 */
ಸ್ಥಿರ ಶೂನ್ಯ parse_repo_refspecs(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **repo,
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ ***refspecs)
{
	ಆದರೆ (argc > 0) {
		*repo = *argv++;
		argc--;
	} ಇಲ್ಲದಿದ್ದರೆ
		*repo = ಖಾಲಿ;
	*refspecs = argv;
}

/**
 * Runs git-fetch, returning its exit status. `repo` and `refspecs` are the
 * repository and refspecs to fetch, or NULL if they are not provided.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಸತತ_fetch(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *repo, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **refspecs)
{
	ರಚನೆ ಮಗು_process cmd = ಮಗು_PROCESS_INIT;

	strvec_pushl(&cmd.ವಾದಗಳು, "fetch", "--update-head-ok", ಖಾಲಿ);

	/* Shared options */
	argv_ತಳ್ಳು_verbosity(&cmd.ವಾದಗಳು);
	ಆದರೆ (opt_ಪ್ರಗತಿ)
		strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, opt_ಪ್ರಗತಿ);

	/* Options passed to git-fetch */
	ಆದರೆ (opt_all)
		strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, opt_all);
	ಆದರೆ (opt_append)
		strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, opt_append);
	ಆದರೆ (opt_upload_pack)
		strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, opt_upload_pack);
	argv_ತಳ್ಳು_force(&cmd.ವಾದಗಳು);
	ಆದರೆ (opt_ಟ್ಯಾಗುಗಳು)
		strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, opt_ಟ್ಯಾಗುಗಳು);
	ಆದರೆ (opt_prune)
		strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, opt_prune);
	ಆದರೆ (recurse_submodules_cli != RECURSE_SUBMODULES_DEFAULT)
		ಆಯ್ಕೆ (recurse_submodules_cli) {
		ಸ್ಥಿತಿ RECURSE_SUBMODULES_ON:
			strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, "--recurse-submodules=on");
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ RECURSE_SUBMODULES_OFF:
			strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, "--recurse-submodules=no");
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ RECURSE_SUBMODULES_ON_DEMAND:
			strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, "--recurse-submodules=on-demand");
			ನಿಲ್ಲಿಸು;
		ಪೂರ್ವನಿಯೋಜಿತ:
			BUG("submodule recursion option not understood");
		}
	ಆದರೆ (max_children)
		strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, max_children);
	ಆದರೆ (opt_dry_ಸತತ)
		strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, "--dry-run");
	ಆದರೆ (opt_keep)
		strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, opt_keep);
	ಆದರೆ (opt_ಆಳ)
		strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, opt_ಆಳ);
	ಆದರೆ (opt_unshallow)
		strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, opt_unshallow);
	ಆದರೆ (opt_update_shallow)
		strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, opt_update_shallow);
	ಆದರೆ (opt_refmap)
		strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, opt_refmap);
	ಆದರೆ (opt_ipv4)
		strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, opt_ipv4);
	ಆದರೆ (opt_ipv6)
		strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, opt_ipv6);
	ಆದರೆ (opt_show_forced_updates > 0)
		strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, "--show-forced-updates");
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (opt_show_forced_updates == 0)
		strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, "--no-show-forced-updates");
	ಆದರೆ (ಗಣ_upstream)
		strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, ಗಣ_upstream);
	strvec_pushv(&cmd.ವಾದಗಳು, opt_fetch.v);

	ಆದರೆ (repo) {
		strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, repo);
		strvec_pushv(&cmd.ವಾದಗಳು, refspecs);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (*refspecs)
		BUG("refspecs without repo?");
	cmd.git_cmd = 1;
	cmd.odb_ಗಮ್ಯ_close = the_repository->objects;
	ಹಿಂತಿರುಗಿ ಸತತ_command(&cmd);
}

/**
 * "Pulls into void" by branching off merge_head.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ pull_into_void(ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *ವಿಲೀನ_ತಲೆ,
		ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *curr_ತಲೆ)
{
	ಆದರೆ (opt_verify_signatures) {
		ರಚನೆ commit *commit;

		commit = lookup_commit(the_repository, ವಿಲೀನ_ತಲೆ);
		ಆದರೆ (!commit)
			die(_("unable to access commit %s"),
			    oid_ಗಮ್ಯ_hex(ವಿಲೀನ_ತಲೆ));

		verify_ವಿಲೀನ_signature(commit, opt_verbosity,
				       check_trust_level);
	}

	/*
	 * Two-way merge: we treat the index as based on an empty tree,
	 * and try to fast-forward to HEAD. This ensures we will not lose
	 * index/worktree changes that the user already made on the unborn
	 * branch.
	 */
	ಆದರೆ (checkout_fast_forward(the_repository,
				  the_hash_algo->ಖಾಲಿಗಳು_ವೃಕ್ಷ,
				  ವಿಲೀನ_ತಲೆ, 0))
		ಹಿಂತಿರುಗಿ 1;

	ಆದರೆ (refs_update_ref(get_main_ref_store(the_repository), "initial pull", "HEAD", ವಿಲೀನ_ತಲೆ, curr_ತಲೆ, 0, UPDATE_REFS_DIE_ON_ERR))
		ಹಿಂತಿರುಗಿ 1;

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ rebase_submodules(ಶೂನ್ಯ)
{
	ರಚನೆ ಮಗು_process cp = ಮಗು_PROCESS_INIT;

	cp.git_cmd = 1;
	cp.no_stdin = 1;
	strvec_pushl(&cp.ವಾದಗಳು, "submodule", "update",
		     "--recursive", "--rebase", ಖಾಲಿ);
	argv_ತಳ್ಳು_verbosity(&cp.ವಾದಗಳು);

	ಹಿಂತಿರುಗಿ ಸತತ_command(&cp);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ update_submodules(ಶೂನ್ಯ)
{
	ರಚನೆ ಮಗು_process cp = ಮಗು_PROCESS_INIT;

	cp.git_cmd = 1;
	cp.no_stdin = 1;
	strvec_pushl(&cp.ವಾದಗಳು, "submodule", "update",
		     "--recursive", "--checkout", ಖಾಲಿ);
	argv_ತಳ್ಳು_verbosity(&cp.ವಾದಗಳು);

	ಹಿಂತಿರುಗಿ ಸತತ_command(&cp);
}

/**
 * Runs git-merge, returning its exit status.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಸತತ_ವಿಲೀನ(ಶೂನ್ಯ)
{
	ರಚನೆ ಮಗು_process cmd = ಮಗು_PROCESS_INIT;

	strvec_pushl(&cmd.ವಾದಗಳು, "merge", ಖಾಲಿ);

	/* Shared options */
	argv_ತಳ್ಳು_verbosity(&cmd.ವಾದಗಳು);
	ಆದರೆ (opt_ಪ್ರಗತಿ)
		strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, opt_ಪ್ರಗತಿ);

	/* Options passed to git-merge */
	ಆದರೆ (opt_diffstat)
		strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, opt_diffstat);
	ಆದರೆ (opt_log)
		strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, opt_log);
	ಆದರೆ (opt_signoff)
		strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, opt_signoff);
	ಆದರೆ (opt_squash)
		strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, opt_squash);
	ಆದರೆ (opt_commit)
		strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, opt_commit);
	ಆದರೆ (opt_edit)
		strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, opt_edit);
	ಆದರೆ (cleanup_arg)
		strvec_pushf(&cmd.ವಾದಗಳು, "--cleanup=%s", cleanup_arg);
	ಆದರೆ (opt_ff)
		strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, opt_ff);
	ಆದರೆ (opt_verify)
		strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, opt_verify);
	ಆದರೆ (opt_verify_signatures)
		strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, opt_verify_signatures);
	strvec_pushv(&cmd.ವಾದಗಳು, opt_strategies.v);
	strvec_pushv(&cmd.ವಾದಗಳು, opt_strategy_opts.v);
	ಆದರೆ (opt_gpg_sign)
		strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, opt_gpg_sign);
	ಆದರೆ (opt_autostash == 0)
		strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, "--no-autostash");
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (opt_autostash == 1)
		strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, "--autostash");
	ಆದರೆ (opt_allow_unrelated_histories > 0)
		strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, "--allow-unrelated-histories");

	strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, "FETCH_HEAD");
	cmd.git_cmd = 1;
	ಹಿಂತಿರುಗಿ ಸತತ_command(&cmd);
}

/**
 * Returns remote's upstream branch for the current branch. If remote is NULL,
 * the current branch's configured default remote is used. Returns NULL if
 * `remote` does not name a valid remote, HEAD does not point to a branch,
 * remote is not the branch's configured remote or the branch does not have any
 * configured upstream branch.
 */
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *get_upstream_branch(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *remote)
{
	ರಚನೆ remote *rm;
	ರಚನೆ branch *curr_branch;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *curr_branch_remote;

	rm = remote_get(remote);
	ಆದರೆ (!rm)
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;

	curr_branch = branch_get("HEAD");
	ಆದರೆ (!curr_branch)
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;

	curr_branch_remote = remote_for_branch(curr_branch, ಖಾಲಿ);
	ಪ್ರತಿಜ್ಞಾ(curr_branch_remote);

	ಆದರೆ (strcmp(curr_branch_remote, rm->name))
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;

	ಹಿಂತಿರುಗಿ branch_get_upstream(curr_branch, ಖಾಲಿ);
}

/**
 * Derives the remote-tracking branch from the remote and refspec.
 *
 * FIXME: The current implementation assumes the default mapping of
 * refs/heads/<branch_name> to refs/remotes/<remote_name>/<branch_name>.
 */
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *get_tracking_branch(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *remote, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *refspec)
{
	ರಚನೆ refspec_ಅಂಶ spec;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *spec_src;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ವಿಲೀನ_branch;

	ಆದರೆ (!refspec_ಅಂಶ_init_fetch(&spec, refspec, the_hash_algo))
		die(_("invalid refspec '%s'"), refspec);
	spec_src = spec.src;
	ಆದರೆ (!*spec_src || !strcmp(spec_src, "HEAD"))
		spec_src = "HEAD";
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(spec_src, "heads/", &spec_src))
		;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(spec_src, "refs/heads/", &spec_src))
		;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (starts_with(spec_src, "refs/") ||
		starts_with(spec_src, "tags/") ||
		starts_with(spec_src, "remotes/"))
		spec_src = "";

	ಆದರೆ (*spec_src) {
		ಆದರೆ (!strcmp(remote, "."))
			ವಿಲೀನ_branch = mkpath("refs/heads/%s", spec_src);
		ಇಲ್ಲದಿದ್ದರೆ
			ವಿಲೀನ_branch = mkpath("refs/remotes/%s/%s", remote, spec_src);
	} ಇಲ್ಲದಿದ್ದರೆ
		ವಿಲೀನ_branch = ಖಾಲಿ;

	refspec_ಅಂಶ_clear(&spec);
	ಹಿಂತಿರುಗಿ ವಿಲೀನ_branch;
}

/**
 * Given the repo and refspecs, sets fork_point to the point at which the
 * current branch forked from its remote-tracking branch. Returns 0 on success,
 * -1 on failure.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ get_rebase_fork_ಬಿಂದು(ರಚನೆ object_id *fork_ಬಿಂದು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *repo,
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *refspec)
{
	ಪೂರ್ಣಾಂಕ ret;
	ರಚನೆ branch *curr_branch;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *remote_branch;
	ರಚನೆ ಮಗು_process cp = ಮಗು_PROCESS_INIT;
	ರಚನೆ strbuf sb = STRBUF_INIT;

	curr_branch = branch_get("HEAD");
	ಆದರೆ (!curr_branch)
		ಹಿಂತಿರುಗಿ -1;

	ಆದರೆ (refspec)
		remote_branch = get_tracking_branch(repo, refspec);
	ಇಲ್ಲದಿದ್ದರೆ
		remote_branch = get_upstream_branch(repo);

	ಆದರೆ (!remote_branch)
		ಹಿಂತಿರುಗಿ -1;

	strvec_pushl(&cp.ವಾದಗಳು, "merge-base", "--fork-point",
		     remote_branch, curr_branch->name, ಖಾಲಿ);
	cp.no_stdin = 1;
	cp.no_stderr = 1;
	cp.git_cmd = 1;

	ret = capture_command(&cp, &sb, GIT_MAX_HEXSZ);
	ಆದರೆ (ret)
		ಹೋಗು cleanup;

	ret = get_oid_hex(sb.buf, fork_ಬಿಂದು);
	ಆದರೆ (ret)
		ಹೋಗು cleanup;

cleanup:
	strbuf_release(&sb);
	ಹಿಂತಿರುಗಿ ret ? -1 : 0;
}

/**
 * Sets merge_base to the octopus merge base of curr_head, merge_head and
 * fork_point. Returns 0 if a merge base is found, 1 otherwise.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ get_octopus_ವಿಲೀನ_ತಳ(ರಚನೆ object_id *ವಿಲೀನ_ತಳ,
		ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *curr_ತಲೆ,
		ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *ವಿಲೀನ_ತಲೆ,
		ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *fork_ಬಿಂದು)
{
	ರಚನೆ commit_list *revs = ಖಾಲಿ, *ಫಲಿತಾಂಶ = ಖಾಲಿ;

	commit_list_ಸೇರಿಸು(lookup_commit_reference(the_repository, curr_ತಲೆ),
			   &revs);
	commit_list_ಸೇರಿಸು(lookup_commit_reference(the_repository, ವಿಲೀನ_ತಲೆ),
			   &revs);
	ಆದರೆ (!is_null_oid(fork_ಬಿಂದು))
		commit_list_ಸೇರಿಸು(lookup_commit_reference(the_repository, fork_ಬಿಂದು),
				   &revs);

	ಆದರೆ (get_octopus_ವಿಲೀನ_bases(revs, &ಫಲಿತಾಂಶ) < 0)
		ನಿರ್ಗಮಿಸು(128);
	commit_list_free(revs);
	reduce_heads_replace(&ಫಲಿತಾಂಶ);

	ಆದರೆ (!ಫಲಿತಾಂಶ)
		ಹಿಂತಿರುಗಿ 1;

	oidcpy(ವಿಲೀನ_ತಳ, &ಫಲಿತಾಂಶ->ಅಂಶ->object.oid);
	commit_list_free(ಫಲಿತಾಂಶ);
	ಹಿಂತಿರುಗಿ 0;
}

/**
 * Given the current HEAD oid, the merge head returned from git-fetch and the
 * fork point calculated by get_rebase_fork_point(), compute the <newbase> and
 * <upstream> arguments to use for the upcoming git-rebase invocation.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ get_rebase_newbase_and_upstream(ರಚನೆ object_id *newbase,
		ರಚನೆ object_id *upstream,
		ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *curr_ತಲೆ,
		ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *ವಿಲೀನ_ತಲೆ,
		ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *fork_ಬಿಂದು)
{
	ರಚನೆ object_id oct_ವಿಲೀನ_ತಳ;

	ಆದರೆ (!get_octopus_ವಿಲೀನ_ತಳ(&oct_ವಿಲೀನ_ತಳ, curr_ತಲೆ, ವಿಲೀನ_ತಲೆ, fork_ಬಿಂದು))
		ಆದರೆ (!is_null_oid(fork_ಬಿಂದು) && oideq(&oct_ವಿಲೀನ_ತಳ, fork_ಬಿಂದು))
			fork_ಬಿಂದು = ಖಾಲಿ;

	ಆದರೆ (fork_ಬಿಂದು && !is_null_oid(fork_ಬಿಂದು))
		oidcpy(upstream, fork_ಬಿಂದು);
	ಇಲ್ಲದಿದ್ದರೆ
		oidcpy(upstream, ವಿಲೀನ_ತಲೆ);

	oidcpy(newbase, ವಿಲೀನ_ತಲೆ);

	ಹಿಂತಿರುಗಿ 0;
}

/**
 * Given the <newbase> and <upstream> calculated by
 * get_rebase_newbase_and_upstream(), runs git-rebase with the
 * appropriate arguments and returns its exit status.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಸತತ_rebase(ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *newbase,
		ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *upstream)
{
	ರಚನೆ ಮಗು_process cmd = ಮಗು_PROCESS_INIT;

	strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, "rebase");

	/* Shared options */
	argv_ತಳ್ಳು_verbosity(&cmd.ವಾದಗಳು);

	/* Options passed to git-rebase */
	ಆದರೆ (opt_rebase == REBASE_MERGES)
		strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, "--rebase-merges");
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (opt_rebase == REBASE_INTERACTIVE)
		strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, "--interactive");
	ಆದರೆ (opt_diffstat)
		strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, opt_diffstat);
	strvec_pushv(&cmd.ವಾದಗಳು, opt_strategies.v);
	strvec_pushv(&cmd.ವಾದಗಳು, opt_strategy_opts.v);
	ಆದರೆ (opt_gpg_sign)
		strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, opt_gpg_sign);
	ಆದರೆ (opt_signoff)
		strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, opt_signoff);
	ಆದರೆ (opt_autostash == 0)
		strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, "--no-autostash");
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (opt_autostash == 1)
		strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, "--autostash");
	ಆದರೆ (opt_verify_signatures &&
	    !strcmp(opt_verify_signatures, "--verify-signatures"))
		warning(_("ignoring --verify-signatures for rebase"));

	strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, "--onto");
	strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, oid_ಗಮ್ಯ_hex(newbase));

	strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, oid_ಗಮ್ಯ_hex(upstream));

	cmd.git_cmd = 1;
	ಹಿಂತಿರುಗಿ ಸತತ_command(&cmd);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ get_can_ff(ರಚನೆ object_id *orig_ತಲೆ,
		      ರಚನೆ oid_array *ವಿಲೀನ_heads)
{
	ಪೂರ್ಣಾಂಕ ret;
	ರಚನೆ commit_list *list = ಖಾಲಿ;
	ರಚನೆ commit *ವಿಲೀನ_ತಲೆ, *ತಲೆ;
	ರಚನೆ object_id *orig_ವಿಲೀನ_ತಲೆ;

	ಆದರೆ (ವಿಲೀನ_heads->nr > 1)
		ಹಿಂತಿರುಗಿ 0;

	orig_ವಿಲೀನ_ತಲೆ = &ವಿಲೀನ_heads->oid[0];
	ತಲೆ = lookup_commit_reference(the_repository, orig_ತಲೆ);
	ಆದರೆ (!ತಲೆ)
		ಹಿಂತಿರುಗಿ 0;
	ವಿಲೀನ_ತಲೆ = lookup_commit_reference(the_repository, orig_ವಿಲೀನ_ತಲೆ);
	ಆದರೆ (!ವಿಲೀನ_ತಲೆ)
		ಹಿಂತಿರುಗಿ 0;
	commit_list_ಸೇರಿಸು(ತಲೆ, &list);
	ret = repo_is_descendant_ದ(the_repository, ವಿಲೀನ_ತಲೆ, list);
	commit_list_free(list);
	ಆದರೆ (ret < 0)
		ನಿರ್ಗಮಿಸು(128);
	ಹಿಂತಿರುಗಿ ret;
}

/*
 * Is orig_head a descendant of _all_ merge_heads?
 * Unfortunately is_descendant_of() cannot be used as it asks
 * if orig_head is a descendant of at least one of them.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ already_ಮೇಲೆ_ಗಮ್ಯ_date(ರಚನೆ object_id *orig_ತಲೆ,
			      ರಚನೆ oid_array *ವಿಲೀನ_heads)
{
	ರಚನೆ commit *ours;

	ours = lookup_commit_reference(the_repository, orig_ತಲೆ);
	ಆದರೆ (!ours)
		ಹಿಂತಿರುಗಿ 0;
	ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < ವಿಲೀನ_heads->nr; i++) {
		ರಚನೆ commit_list *list = ಖಾಲಿ;
		ರಚನೆ commit *theirs;
		ಪೂರ್ಣಾಂಕ ok;

		theirs = lookup_commit_reference(the_repository, &ವಿಲೀನ_heads->oid[i]);
		ಆದರೆ (!theirs)
			ಹಿಂತಿರುಗಿ 0;
		commit_list_ಸೇರಿಸು(theirs, &list);
		ok = repo_is_descendant_ದ(the_repository, ours, list);
		commit_list_free(list);
		ಆದರೆ (ok < 0)
			ನಿರ್ಗಮಿಸು(128);
		ಆದರೆ (!ok)
			ಹಿಂತಿರುಗಿ 0;
	}
	ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ ಶೂನ್ಯ show_advice_pull_non_ff(ಶೂನ್ಯ)
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

ಪೂರ್ಣಾಂಕ cmd_pull(ಪೂರ್ಣಾಂಕ argc,
	     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
	     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
	     ರಚನೆ repository *repository ಬಳಸದ)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *repo, **refspecs;
	ರಚನೆ oid_array ವಿಲೀನ_heads = OID_ARRAY_INIT;
	ರಚನೆ object_id orig_ತಲೆ, curr_ತಲೆ;
	ರಚನೆ object_id rebase_fork_ಬಿಂದು;
	ಪೂರ್ಣಾಂಕ rebase_unspecified = 0;
	ಪೂರ್ಣಾಂಕ can_ff;
	ಪೂರ್ಣಾಂಕ divergent;
	ಪೂರ್ಣಾಂಕ ret;
	ಸ್ಥಿರ ರಚನೆ option pull_options[] = {
		/* Shared options */
		OPT__VERBOSITY(&opt_verbosity),
		OPT_PASSTHRU(0, "progress", &opt_ಪ್ರಗತಿ, ಖಾಲಿ,
			N_("force progress reporting"),
			PARSE_OPT_NOARG),
		OPT_CALLBACK_F(0, "recurse-submodules",
			   &recurse_submodules_cli, N_("on-demand"),
			   N_("control for recursive fetching of submodules"),
			   PARSE_OPT_OPTARG, option_fetch_parse_recurse_submodules),

		/* Options passed to git-merge or git-rebase */
		OPT_ಗುಂಪು(N_("Options related to merging")),
		OPT_CALLBACK_F('r', "rebase", &opt_rebase,
			"(false|true|merges|interactive)",
			N_("incorporate changes by rebasing rather than merging"),
			PARSE_OPT_OPTARG, parse_opt_rebase),
		OPT_PASSTHRU('n', ಖಾಲಿ, &opt_diffstat, ಖಾಲಿ,
			N_("do not show a diffstat at the end of the merge"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG),
		OPT_PASSTHRU(0, "stat", &opt_diffstat, ಖಾಲಿ,
			N_("show a diffstat at the end of the merge"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "summary", &opt_diffstat, ಖಾಲಿ,
			N_("(synonym to --stat)"),
			PARSE_OPT_NOARG | PARSE_OPT_HIDDEN),
		OPT_PASSTHRU(0, "compact-summary", &opt_diffstat, ಖಾಲಿ,
			N_("show a compact-summary at the end of the merge"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "log", &opt_log, N_("n"),
			N_("add (at most <n>) entries from shortlog to merge commit message"),
			PARSE_OPT_OPTARG),
		OPT_PASSTHRU(0, "signoff", &opt_signoff, ಖಾಲಿ,
			N_("add a Signed-off-by trailer"),
			PARSE_OPT_OPTARG),
		OPT_PASSTHRU(0, "squash", &opt_squash, ಖಾಲಿ,
			N_("create a single commit instead of doing a merge"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "commit", &opt_commit, ಖಾಲಿ,
			N_("perform a commit if the merge succeeds (default)"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "edit", &opt_edit, ಖಾಲಿ,
			N_("edit message before committing"),
			PARSE_OPT_NOARG),
		OPT_CLEANUP(&cleanup_arg),
		OPT_PASSTHRU(0, "ff", &opt_ff, ಖಾಲಿ,
			N_("allow fast-forward"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "ff-only", &opt_ff, ಖಾಲಿ,
			N_("abort if fast-forward is not possible"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG),
		OPT_PASSTHRU(0, "verify", &opt_verify, ಖಾಲಿ,
			N_("control use of pre-merge-commit and commit-msg hooks"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "verify-signatures", &opt_verify_signatures, ಖಾಲಿ,
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
		OPT_ಗಣ_INT(0, "allow-unrelated-histories",
			    &opt_allow_unrelated_histories,
			    N_("allow merging unrelated histories"), 1),

		/* Options passed to git-fetch */
		OPT_ಗುಂಪು(N_("Options related to fetching")),
		OPT_PASSTHRU(0, "all", &opt_all, ಖಾಲಿ,
			N_("fetch from all remotes"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU('a', "append", &opt_append, ಖಾಲಿ,
			N_("append to .git/FETCH_HEAD instead of overwriting"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "upload-pack", &opt_upload_pack, N_("path"),
			N_("path to upload pack on remote end"),
			0),
		OPT__FORCE(&opt_force, N_("force overwrite of local branch"), 0),
		OPT_PASSTHRU('t', "tags", &opt_ಟ್ಯಾಗುಗಳು, ಖಾಲಿ,
			N_("fetch all tags and associated objects"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU('p', "prune", &opt_prune, ಖಾಲಿ,
			N_("prune remote-tracking branches no longer on remote"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU('j', "jobs", &max_children, N_("n"),
			N_("number of submodules pulled in parallel"),
			PARSE_OPT_OPTARG),
		OPT_BOOL(0, "dry-run", &opt_dry_ಸತತ,
			N_("dry run")),
		OPT_PASSTHRU('k', "keep", &opt_keep, ಖಾಲಿ,
			N_("keep downloaded pack"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "depth", &opt_ಆಳ, N_("depth"),
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
		OPT_PASSTHRU(0, "unshallow", &opt_unshallow, ಖಾಲಿ,
			N_("convert to a complete repository"),
			PARSE_OPT_NONEG | PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "update-shallow", &opt_update_shallow, ಖಾಲಿ,
			N_("accept refs that update .git/shallow"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "refmap", &opt_refmap, N_("refmap"),
			N_("specify fetch refmap"),
			PARSE_OPT_NONEG),
		OPT_PASSTHRU_ARGV('o', "server-option", &opt_fetch,
			N_("server-specific"),
			N_("option to transmit"),
			0),
		OPT_PASSTHRU('4',  "ipv4", &opt_ipv4, ಖಾಲಿ,
			N_("use IPv4 addresses only"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU('6',  "ipv6", &opt_ipv6, ಖಾಲಿ,
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
		OPT_PASSTHRU(0, "set-upstream", &ಗಣ_upstream, ಖಾಲಿ,
			N_("set upstream for git pull/fetch"),
			PARSE_OPT_NOARG),

		OPT_ಕೊನೆ()
	};

	ಆದರೆ (!getenv("GIT_REFLOG_ACTION"))
		ಗಣ_reflog_ಸಂದೇಶ(argc, argv);

	repo_config(the_repository, git_pull_config, ಖಾಲಿ);
	ಆದರೆ (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_ತುಂಬಿದ_ಸೂಚ್ಯಂಕ = 0;
	}

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, pull_options, pull_usage, 0);
	ಆದರೆ (opt_autostash == -1)
		opt_autostash = config_pull_autostash;

	ಆದರೆ (recurse_submodules_cli != RECURSE_SUBMODULES_DEFAULT)
		recurse_submodules = recurse_submodules_cli;

	ಆದರೆ (cleanup_arg)
		/*
		 * this only checks the validity of cleanup_arg; we don't need
		 * a valid value for use_editor
		 */
		get_cleanup_ಬಹುಲಕ(cleanup_arg, 0);

	parse_repo_refspecs(argc, argv, &repo, &refspecs);

	ಆದರೆ (!opt_ff) {
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
		ಆದರೆ (opt_rebase >= 0 && opt_ff && !strcmp(opt_ff, "--ff-only")) {
			ಬಿಡುಗಡೆ(opt_ff);
			opt_ff = xstrdup("--ff");
		}
	}

	ಆದರೆ (opt_rebase < 0)
		opt_rebase = config_get_rebase(&rebase_unspecified);

	ಆದರೆ (repo_read_ಸೂಚ್ಯಂಕ_unmerged(the_repository))
		die_resolve_conflict("pull");

	ಆದರೆ (file_exists(git_ಮಾರ್ಗ_ವಿಲೀನ_ತಲೆ(the_repository)))
		die_conclude_ವಿಲೀನ();

	ಆದರೆ (repo_get_oid(the_repository, "HEAD", &orig_ತಲೆ))
		oidclr(&orig_ತಲೆ, the_repository->hash_algo);

	ಆದರೆ (opt_rebase) {
		ಆದರೆ (opt_autostash == -1)
			opt_autostash = config_rebase_autostash;

		ಆದರೆ (is_null_oid(&orig_ತಲೆ) && !is_ಸೂಚ್ಯಂಕ_unborn(the_repository->ಸೂಚ್ಯಂಕ))
			die(_("Updating an unborn branch with changes added to the index."));

		ಆದರೆ (!opt_autostash)
			require_clean_work_ವೃಕ್ಷ(the_repository,
				N_("pull with rebase"),
				_("Please commit or stash them."), 1, 0);

		ಆದರೆ (get_rebase_fork_ಬಿಂದು(&rebase_fork_ಬಿಂದು, repo, *refspecs))
			oidclr(&rebase_fork_ಬಿಂದು, the_repository->hash_algo);
	}

	ಆದರೆ (ಸತತ_fetch(repo, refspecs))
		ಹಿಂತಿರುಗಿ 1;

	ಆದರೆ (opt_dry_ಸತತ)
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (repo_get_oid(the_repository, "HEAD", &curr_ತಲೆ))
		oidclr(&curr_ತಲೆ, the_repository->hash_algo);

	ಆದರೆ (!is_null_oid(&orig_ತಲೆ) && !is_null_oid(&curr_ತಲೆ) &&
			!oideq(&orig_ತಲೆ, &curr_ತಲೆ)) {
		/*
		 * The fetch involved updating the current branch.
		 *
		 * The working tree and the index file are still based on
		 * orig_head commit, but we are merging into curr_head.
		 * Update the working tree to match curr_head.
		 */

		warning(_("fetch updated the current branch head.\n"
			"fast-forwarding your working tree from\n"
			"commit %s."), oid_ಗಮ್ಯ_hex(&orig_ತಲೆ));

		ಆದರೆ (checkout_fast_forward(the_repository, &orig_ತಲೆ,
					  &curr_ತಲೆ, 0))
			die(_("Cannot fast-forward your working tree.\n"
				"After making sure that you saved anything precious from\n"
				"$ git diff %s\n"
				"output, run\n"
				"$ git reset --hard\n"
				"to recover."), oid_ಗಮ್ಯ_hex(&orig_ತಲೆ));
	}

	get_ವಿಲೀನ_heads(&ವಿಲೀನ_heads);

	ಆದರೆ (!ವಿಲೀನ_heads.nr)
		die_no_ವಿಲೀನ_candidates(repo, refspecs);

	ಆದರೆ (is_null_oid(&orig_ತಲೆ)) {
		ಆದರೆ (ವಿಲೀನ_heads.nr > 1)
			die(_("Cannot merge multiple branches into empty head."));
		ret = pull_into_void(ವಿಲೀನ_heads.oid, &curr_ತಲೆ);
		ಹೋಗು cleanup;
	}
	ಆದರೆ (ವಿಲೀನ_heads.nr > 1) {
		ಆದರೆ (opt_rebase)
			die(_("Cannot rebase onto multiple branches."));
		ಆದರೆ (opt_ff && !strcmp(opt_ff, "--ff-only"))
			die(_("Cannot fast-forward to multiple branches."));
	}

	can_ff = get_can_ff(&orig_ತಲೆ, &ವಿಲೀನ_heads);
	divergent = !can_ff && !already_ಮೇಲೆ_ಗಮ್ಯ_date(&orig_ತಲೆ, &ವಿಲೀನ_heads);

	/* ff-only takes precedence over rebase */
	ಆದರೆ (opt_ff && !strcmp(opt_ff, "--ff-only")) {
		ಆದರೆ (divergent)
			die_ff_impossible();
		opt_rebase = REBASE_FALSE;
	}
	/* If no action specified and we can't fast forward, then warn. */
	ಆದರೆ (!opt_ff && rebase_unspecified && divergent) {
		show_advice_pull_non_ff();
		die(_("Need to specify how to reconcile divergent branches."));
	}

	ಆದರೆ (opt_rebase) {
		ರಚನೆ object_id newbase;
		ರಚನೆ object_id upstream;
		get_rebase_newbase_and_upstream(&newbase, &upstream, &curr_ತಲೆ,
						ವಿಲೀನ_heads.oid, &rebase_fork_ಬಿಂದು);

		ಆದರೆ ((recurse_submodules == RECURSE_SUBMODULES_ON ||
		     recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND) &&
		    submodule_touches_ಒಳ_range(the_repository, &upstream, &curr_ತಲೆ))
			die(_("cannot rebase with locally recorded submodule modifications"));

		ಆದರೆ (can_ff) {
			/* we can fast-forward this without invoking rebase */
			ಬಿಡುಗಡೆ(opt_ff);
			opt_ff = xstrdup("--ff-only");
			ret = ಸತತ_ವಿಲೀನ();
		} ಇಲ್ಲದಿದ್ದರೆ {
			ret = ಸತತ_rebase(&newbase, &upstream);
		}

		ಆದರೆ (!ret && (recurse_submodules == RECURSE_SUBMODULES_ON ||
			     recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND))
			ret = rebase_submodules();

		ಹೋಗು cleanup;
	} ಇಲ್ಲದಿದ್ದರೆ {
		ret = ಸತತ_ವಿಲೀನ();
		ಆದರೆ (!ret && (recurse_submodules == RECURSE_SUBMODULES_ON ||
			     recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND))
			ret = update_submodules();
		ಹೋಗು cleanup;
	}

cleanup:
	oid_array_clear(&ವಿಲೀನ_heads);
	ಹಿಂತಿರುಗಿ ret;
}
