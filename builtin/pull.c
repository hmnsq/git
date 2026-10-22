/*
 * Builtin "git pull"
 *
 * Based on git-pull.sh by Junio C Hamano
 *
 * Fetch one or more remote refs and merge it/them into the current HEAD.
 */

#define USE_THE_REPOSITORY_VARIABLE

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
ಸ್ಥಿರ ಗಣನಾ rebase_type parse_config_rebase(ಧ್ರುವ ವರ್ಣ *ಕೀಲಿ, ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ,
		ಪೂರ್ಣಾಙ್ಕ fatal)
{
	ಗಣನಾ rebase_type v = rebase_parse_ಮೌಲ್ಯ(ಮೌಲ್ಯ);
	ಯದಿ (v != REBASE_INVALID)
		ಪ್ರತ್ಯಾಗಮ v;

	ಯದಿ (fatal)
		die(_("invalid value for '%s': '%s'"), ಕೀಲಿ, ಮೌಲ್ಯ);
	ಅನ್ಯಥಾ
		error(_("invalid value for '%s': '%s'"), ಕೀಲಿ, ಮೌಲ್ಯ);

	ಪ್ರತ್ಯಾಗಮ REBASE_INVALID;
}

/**
 * Callback for --rebase, which parses arg with parse_config_rebase().
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ parse_opt_rebase(ಧ್ರುವ ಸಂರಚನಾ option *opt, ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಗಣನಾ rebase_type *ಮೌಲ್ಯ = opt->ಮೌಲ್ಯ;

	ಯದಿ (arg)
		*ಮೌಲ್ಯ = parse_config_rebase("--rebase", arg, 0);
	ಅನ್ಯಥಾ
		*ಮೌಲ್ಯ = unset ? REBASE_FALSE : REBASE_TRUE;
	ಪ್ರತ್ಯಾಗಮ *ಮೌಲ್ಯ == REBASE_INVALID ? -1 : 0;
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ pull_usage[] = {
	N_("git pull [<options>] [<repository> [<refspec>...]]"),
	ರಿಕ್ತ
};

/* Shared options */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ opt_verbosity;
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *opt_progress;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ recurse_submodules = RECURSE_SUBMODULES_DEFAULT;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ recurse_submodules_cli = RECURSE_SUBMODULES_DEFAULT;

/* Options passed to git-merge or git-rebase */
ಸ್ಥಿರ ಗಣನಾ rebase_type opt_rebase = -1;
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *opt_diffstat;
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *opt_log;
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *opt_signoff;
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *opt_squash;
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *opt_commit;
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *opt_edit;
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *cleanup_arg;
ಸ್ಥಿರ ವರ್ಣ *opt_ff;
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *opt_verify_signatures;
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *opt_verify;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ opt_autostash = -1;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ config_rebase_autostash;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ config_pull_autostash = -1;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ check_trust_level = 1;
ಸ್ಥಿರ ಸಂರಚನಾ strvec opt_strategies = STRVEC_INIT;
ಸ್ಥಿರ ಸಂರಚನಾ strvec opt_strategy_opts = STRVEC_INIT;
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *opt_gpg_sign;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ opt_allow_unrelated_histories;

/* Options passed to git-fetch */
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *opt_all;
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *opt_append;
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *opt_upload_pack;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ opt_force;
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *opt_tags;
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *opt_prune;
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *max_children;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ opt_dry_run;
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *opt_keep;
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *opt_depth;
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *opt_unshallow;
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *opt_update_shallow;
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *opt_refmap;
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *opt_ipv4;
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *opt_ipv6;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ opt_show_forced_updates = -1;
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *set_upstream;
ಸ್ಥಿರ ಸಂರಚನಾ strvec opt_fetch = STRVEC_INIT;

/**
 * Pushes "-q" or "-v" switches into arr to match the opt_verbosity level.
 */
ಸ್ಥಿರ ಶೂನ್ಯ argv_ತಳ್ಳು_verbosity(ಸಂರಚನಾ strvec *arr)
{
	ಪೂರ್ಣಾಙ್ಕ verbosity;

	ಪ್ರತಿ (verbosity = opt_verbosity; verbosity > 0; verbosity--)
		strvec_ತಳ್ಳು(arr, "-v");

	ಪ್ರತಿ (verbosity = opt_verbosity; verbosity < 0; verbosity++)
		strvec_ತಳ್ಳು(arr, "-q");
}

/**
 * Pushes "-f" switches into arr to match the opt_force level.
 */
ಸ್ಥಿರ ಶೂನ್ಯ argv_ತಳ್ಳು_force(ಸಂರಚನಾ strvec *arr)
{
	ಪೂರ್ಣಾಙ್ಕ force = opt_force;
	ಯಾವತ್ (force-- > 0)
		strvec_ತಳ್ಳು(arr, "-f");
}

/**
 * Sets the GIT_REFLOG_ACTION environment variable to the concatenation of argv
 */
ಸ್ಥಿರ ಶೂನ್ಯ set_reflog_message(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv)
{
	ಪೂರ್ಣಾಙ್ಕ i;
	ಸಂರಚನಾ strbuf ಸಂದೇಶ = STRBUF_INIT;

	ಪ್ರತಿ (i = 0; i < argc; i++) {
		ಯದಿ (i)
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
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *config_get_ff(ಶೂನ್ಯ)
{
	ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ;

	ಯದಿ (repo_config_get_ಮೌಲ್ಯ(the_repository, "pull.ff", &ಮೌಲ್ಯ))
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

	ವಿಕಲ್ಪ (git_parse_maybe_bool(ಮೌಲ್ಯ)) {
	ಪಕ್ಷ 0:
		ಪ್ರತ್ಯಾಗಮ "--no-ff";
	ಪಕ್ಷ 1:
		ಪ್ರತ್ಯಾಗಮ "--ff";
	}

	ಯದಿ (!strcmp(ಮೌಲ್ಯ, "only"))
		ಪ್ರತ್ಯಾಗಮ "--ff-only";

	die(_("invalid value for '%s': '%s'"), "pull.ff", ಮೌಲ್ಯ);
}

/**
 * Returns the default configured value for --rebase. It first looks for the
 * value of "branch.$curr_branch.rebase", where $curr_branch is the current
 * branch, and if HEAD is detached or the configuration key does not exist,
 * looks for the value of "pull.rebase". If both configuration keys do not
 * exist, returns REBASE_FALSE.
 */
ಸ್ಥಿರ ಗಣನಾ rebase_type config_get_rebase(ಪೂರ್ಣಾಙ್ಕ *rebase_unspecified)
{
	ಸಂರಚನಾ branch *curr_branch = branch_get("HEAD");
	ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ;

	ಯದಿ (curr_branch) {
		ವರ್ಣ *ಕೀಲಿ = xstrfmt("branch.%s.rebase", curr_branch->name);

		ಯದಿ (!repo_config_get_ಮೌಲ್ಯ(the_repository, ಕೀಲಿ, &ಮೌಲ್ಯ)) {
			ಗಣನಾ rebase_type ret = parse_config_rebase(ಕೀಲಿ, ಮೌಲ್ಯ, 1);
			ಮುಕ್ತ(ಕೀಲಿ);
			ಪ್ರತ್ಯಾಗಮ ret;
		}

		ಮುಕ್ತ(ಕೀಲಿ);
	}

	ಯದಿ (!repo_config_get_ಮೌಲ್ಯ(the_repository, "pull.rebase", &ಮೌಲ್ಯ))
		ಪ್ರತ್ಯಾಗಮ parse_config_rebase("pull.rebase", ಮೌಲ್ಯ, 1);

	*rebase_unspecified = 1;

	ಪ್ರತ್ಯಾಗಮ REBASE_FALSE;
}

/**
 * Read config variables.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ git_pull_config(ಧ್ರುವ ವರ್ಣ *var, ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ,
			   ಧ್ರುವ ಸಂರಚನಾ config_context *ctx, ಶೂನ್ಯ *cb)
{
	ಯದಿ (!strcmp(var, "rebase.autostash")) {
		/*
		 * run_rebase() also reads this option. The reason we handle it here is
		 * that when pull.rebase is true, a fast-forward may occur without
		 * invoking run_rebase(). We need to ensure that autostash is set even
		 * in the fast-forward case.
		 *
		 * run_merge() handles merge.autostash, so we don't handle it here.
		 */
		config_rebase_autostash = git_config_bool(var, ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	} ಅನ್ಯಥಾ ಯದಿ (!strcmp(var, "pull.autostash")) {
		config_pull_autostash = git_config_bool(var, ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	} ಅನ್ಯಥಾ ಯದಿ (!strcmp(var, "submodule.recurse")) {
		recurse_submodules = git_config_bool(var, ಮೌಲ್ಯ) ?
			RECURSE_SUBMODULES_ON : RECURSE_SUBMODULES_OFF;
		ಪ್ರತ್ಯಾಗಮ 0;
	} ಅನ್ಯಥಾ ಯದಿ (!strcmp(var, "gpg.mintrustlevel")) {
		check_trust_level = 0;
	}

	ಪ್ರತ್ಯಾಗಮ git_default_config(var, ಮೌಲ್ಯ, ctx, cb);
}

/**
 * Appends merge candidates from FETCH_HEAD that are not marked not-for-merge
 * into merge_heads.
 */
ಸ್ಥಿರ ಶೂನ್ಯ get_merge_heads(ಸಂರಚನಾ oid_array *merge_heads)
{
	ಧ್ರುವ ವರ್ಣ *filename = git_path_fetch_head(the_repository);
	FILE *fp;
	ಸಂರಚನಾ strbuf sb = STRBUF_INIT;
	ಸಂರಚನಾ object_id oid;

	fp = xfopen(filename, "r");
	ಯಾವತ್ (strbuf_getline_lf(&sb, fp) != EOF) {
		ಧ್ರುವ ವರ್ಣ *p;
		ಯದಿ (parse_oid_hex(sb.buf, &oid, &p))
			ಅನುವರ್ತ;  /* invalid line: does not start with object ID */
		ಯದಿ (starts_with(p, "\tnot-for-merge\t"))
			ಅನುವರ್ತ;  /* ref is not-for-merge */
		oid_array_append(merge_heads, &oid);
	}
	fclose(fp);
	strbuf_release(&sb);
}

/**
 * Used by die_no_merge_candidates() as a for_each_remote() callback to
 * retrieve the name of the remote if the repository only has one remote.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ get_only_remote(ಸಂರಚನಾ remote *remote, ಶೂನ್ಯ *cb_data)
{
	ಧ್ರುವ ವರ್ಣ **remote_name = cb_data;

	ಯದಿ (*remote_name)
		ಪ್ರತ್ಯಾಗಮ -1;

	*remote_name = remote->name;
	ಪ್ರತ್ಯಾಗಮ 0;
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
ಸ್ಥಿರ ಶೂನ್ಯ NORETURN die_no_merge_candidates(ಧ್ರುವ ವರ್ಣ *repo, ಧ್ರುವ ವರ್ಣ **refspecs)
{
	ಸಂರಚನಾ branch *curr_branch = branch_get("HEAD");
	ಧ್ರುವ ವರ್ಣ *remote = curr_branch ? curr_branch->remote_name : ರಿಕ್ತ;

	ಯದಿ (*refspecs) {
		ಯದಿ (opt_rebase)
			fprintf_ln(stderr, _("There is no candidate for rebasing against among the refs that you just fetched."));
		ಅನ್ಯಥಾ
			fprintf_ln(stderr, _("There are no candidates for merging among the refs that you just fetched."));
		fprintf_ln(stderr, _("Generally this means that you provided a wildcard refspec which had no\n"
					"matches on the remote end."));
	} ಅನ್ಯಥಾ ಯದಿ (repo && curr_branch && (!remote || strcmp(repo, remote))) {
		fprintf_ln(stderr, _("You asked to pull from the remote '%s', but did not specify\n"
			"a branch. Because this is not the default configured remote\n"
			"for your current branch, you must specify a branch on the command line."),
			repo);
	} ಅನ್ಯಥಾ ಯದಿ (!curr_branch) {
		fprintf_ln(stderr, _("You are not currently on a branch."));
		ಯದಿ (opt_rebase)
			fprintf_ln(stderr, _("Please specify which branch you want to rebase against."));
		ಅನ್ಯಥಾ
			fprintf_ln(stderr, _("Please specify which branch you want to merge with."));
		fprintf_ln(stderr, _("See git-pull(1) for details."));
		fprintf(stderr, "\n");
		fprintf_ln(stderr, "    git pull %s %s", _("<remote>"), _("<branch>"));
		fprintf(stderr, "\n");
	} ಅನ್ಯಥಾ ಯದಿ (!curr_branch->merge_nr) {
		ಧ್ರುವ ವರ್ಣ *remote_name = ರಿಕ್ತ;

		ಯದಿ (for_each_remote(get_only_remote, &remote_name) || !remote_name)
			remote_name = _("<remote>");

		fprintf_ln(stderr, _("There is no tracking information for the current branch."));
		ಯದಿ (opt_rebase)
			fprintf_ln(stderr, _("Please specify which branch you want to rebase against."));
		ಅನ್ಯಥಾ
			fprintf_ln(stderr, _("Please specify which branch you want to merge with."));
		fprintf_ln(stderr, _("See git-pull(1) for details."));
		fprintf(stderr, "\n");
		fprintf_ln(stderr, "    git pull %s %s", _("<remote>"), _("<branch>"));
		fprintf(stderr, "\n");
		fprintf_ln(stderr, _("If you wish to set tracking information for this branch you can do so with:"));
		fprintf(stderr, "\n");
		fprintf_ln(stderr, "    git branch --set-upstream-to=%s/%s %s\n",
				remote_name, _("<branch>"), curr_branch->name);
	} ಅನ್ಯಥಾ
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
ಸ್ಥಿರ ಶೂನ್ಯ parse_repo_refspecs(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ **repo,
		ಧ್ರುವ ವರ್ಣ ***refspecs)
{
	ಯದಿ (argc > 0) {
		*repo = *argv++;
		argc--;
	} ಅನ್ಯಥಾ
		*repo = ರಿಕ್ತ;
	*refspecs = argv;
}

/**
 * Runs git-fetch, returning its exit status. `repo` and `refspecs` are the
 * repository and refspecs to fetch, or NULL if they are not provided.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ run_fetch(ಧ್ರುವ ವರ್ಣ *repo, ಧ್ರುವ ವರ್ಣ **refspecs)
{
	ಸಂರಚನಾ child_process cmd = CHILD_PROCESS_INIT;

	strvec_pushl(&cmd.args, "fetch", "--update-head-ok", ರಿಕ್ತ);

	/* Shared options */
	argv_ತಳ್ಳು_verbosity(&cmd.args);
	ಯದಿ (opt_progress)
		strvec_ತಳ್ಳು(&cmd.args, opt_progress);

	/* Options passed to git-fetch */
	ಯದಿ (opt_all)
		strvec_ತಳ್ಳು(&cmd.args, opt_all);
	ಯದಿ (opt_append)
		strvec_ತಳ್ಳು(&cmd.args, opt_append);
	ಯದಿ (opt_upload_pack)
		strvec_ತಳ್ಳು(&cmd.args, opt_upload_pack);
	argv_ತಳ್ಳು_force(&cmd.args);
	ಯದಿ (opt_tags)
		strvec_ತಳ್ಳು(&cmd.args, opt_tags);
	ಯದಿ (opt_prune)
		strvec_ತಳ್ಳು(&cmd.args, opt_prune);
	ಯದಿ (recurse_submodules_cli != RECURSE_SUBMODULES_DEFAULT)
		ವಿಕಲ್ಪ (recurse_submodules_cli) {
		ಪಕ್ಷ RECURSE_SUBMODULES_ON:
			strvec_ತಳ್ಳು(&cmd.args, "--recurse-submodules=on");
			ಭಙ್ಗ;
		ಪಕ್ಷ RECURSE_SUBMODULES_OFF:
			strvec_ತಳ್ಳು(&cmd.args, "--recurse-submodules=no");
			ಭಙ್ಗ;
		ಪಕ್ಷ RECURSE_SUBMODULES_ON_DEMAND:
			strvec_ತಳ್ಳು(&cmd.args, "--recurse-submodules=on-demand");
			ಭಙ್ಗ;
		ಸ್ವಭಾವ:
			BUG("submodule recursion option not understood");
		}
	ಯದಿ (max_children)
		strvec_ತಳ್ಳು(&cmd.args, max_children);
	ಯದಿ (opt_dry_run)
		strvec_ತಳ್ಳು(&cmd.args, "--dry-run");
	ಯದಿ (opt_keep)
		strvec_ತಳ್ಳು(&cmd.args, opt_keep);
	ಯದಿ (opt_depth)
		strvec_ತಳ್ಳು(&cmd.args, opt_depth);
	ಯದಿ (opt_unshallow)
		strvec_ತಳ್ಳು(&cmd.args, opt_unshallow);
	ಯದಿ (opt_update_shallow)
		strvec_ತಳ್ಳು(&cmd.args, opt_update_shallow);
	ಯದಿ (opt_refmap)
		strvec_ತಳ್ಳು(&cmd.args, opt_refmap);
	ಯದಿ (opt_ipv4)
		strvec_ತಳ್ಳು(&cmd.args, opt_ipv4);
	ಯದಿ (opt_ipv6)
		strvec_ತಳ್ಳು(&cmd.args, opt_ipv6);
	ಯದಿ (opt_show_forced_updates > 0)
		strvec_ತಳ್ಳು(&cmd.args, "--show-forced-updates");
	ಅನ್ಯಥಾ ಯದಿ (opt_show_forced_updates == 0)
		strvec_ತಳ್ಳು(&cmd.args, "--no-show-forced-updates");
	ಯದಿ (set_upstream)
		strvec_ತಳ್ಳು(&cmd.args, set_upstream);
	strvec_pushv(&cmd.args, opt_fetch.v);

	ಯದಿ (repo) {
		strvec_ತಳ್ಳು(&cmd.args, repo);
		strvec_pushv(&cmd.args, refspecs);
	} ಅನ್ಯಥಾ ಯದಿ (*refspecs)
		BUG("refspecs without repo?");
	cmd.git_cmd = 1;
	cmd.odb_to_close = the_repository->objects;
	ಪ್ರತ್ಯಾಗಮ run_command(&cmd);
}

/**
 * "Pulls into void" by branching off merge_head.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ pull_into_void(ಧ್ರುವ ಸಂರಚನಾ object_id *merge_head,
		ಧ್ರುವ ಸಂರಚನಾ object_id *curr_head)
{
	ಯದಿ (opt_verify_signatures) {
		ಸಂರಚನಾ commit *commit;

		commit = lookup_commit(the_repository, merge_head);
		ಯದಿ (!commit)
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
	ಯದಿ (checkout_fast_forward(the_repository,
				  the_hash_algo->empty_ವೃಕ್ಷ,
				  merge_head, 0))
		ಪ್ರತ್ಯಾಗಮ 1;

	ಯದಿ (refs_update_ref(get_main_ref_store(the_repository), "initial pull", "HEAD", merge_head, curr_head, 0, UPDATE_REFS_DIE_ON_ERR))
		ಪ್ರತ್ಯಾಗಮ 1;

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ rebase_submodules(ಶೂನ್ಯ)
{
	ಸಂರಚನಾ child_process cp = CHILD_PROCESS_INIT;

	cp.git_cmd = 1;
	cp.no_stdin = 1;
	strvec_pushl(&cp.args, "submodule", "update",
		     "--recursive", "--rebase", ರಿಕ್ತ);
	argv_ತಳ್ಳು_verbosity(&cp.args);

	ಪ್ರತ್ಯಾಗಮ run_command(&cp);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ update_submodules(ಶೂನ್ಯ)
{
	ಸಂರಚನಾ child_process cp = CHILD_PROCESS_INIT;

	cp.git_cmd = 1;
	cp.no_stdin = 1;
	strvec_pushl(&cp.args, "submodule", "update",
		     "--recursive", "--checkout", ರಿಕ್ತ);
	argv_ತಳ್ಳು_verbosity(&cp.args);

	ಪ್ರತ್ಯಾಗಮ run_command(&cp);
}

/**
 * Runs git-merge, returning its exit status.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ run_merge(ಶೂನ್ಯ)
{
	ಸಂರಚನಾ child_process cmd = CHILD_PROCESS_INIT;

	strvec_pushl(&cmd.args, "merge", ರಿಕ್ತ);

	/* Shared options */
	argv_ತಳ್ಳು_verbosity(&cmd.args);
	ಯದಿ (opt_progress)
		strvec_ತಳ್ಳು(&cmd.args, opt_progress);

	/* Options passed to git-merge */
	ಯದಿ (opt_diffstat)
		strvec_ತಳ್ಳು(&cmd.args, opt_diffstat);
	ಯದಿ (opt_log)
		strvec_ತಳ್ಳು(&cmd.args, opt_log);
	ಯದಿ (opt_signoff)
		strvec_ತಳ್ಳು(&cmd.args, opt_signoff);
	ಯದಿ (opt_squash)
		strvec_ತಳ್ಳು(&cmd.args, opt_squash);
	ಯದಿ (opt_commit)
		strvec_ತಳ್ಳು(&cmd.args, opt_commit);
	ಯದಿ (opt_edit)
		strvec_ತಳ್ಳು(&cmd.args, opt_edit);
	ಯದಿ (cleanup_arg)
		strvec_pushf(&cmd.args, "--cleanup=%s", cleanup_arg);
	ಯದಿ (opt_ff)
		strvec_ತಳ್ಳು(&cmd.args, opt_ff);
	ಯದಿ (opt_verify)
		strvec_ತಳ್ಳು(&cmd.args, opt_verify);
	ಯದಿ (opt_verify_signatures)
		strvec_ತಳ್ಳು(&cmd.args, opt_verify_signatures);
	strvec_pushv(&cmd.args, opt_strategies.v);
	strvec_pushv(&cmd.args, opt_strategy_opts.v);
	ಯದಿ (opt_gpg_sign)
		strvec_ತಳ್ಳು(&cmd.args, opt_gpg_sign);
	ಯದಿ (opt_autostash == 0)
		strvec_ತಳ್ಳು(&cmd.args, "--no-autostash");
	ಅನ್ಯಥಾ ಯದಿ (opt_autostash == 1)
		strvec_ತಳ್ಳು(&cmd.args, "--autostash");
	ಯದಿ (opt_allow_unrelated_histories > 0)
		strvec_ತಳ್ಳು(&cmd.args, "--allow-unrelated-histories");

	strvec_ತಳ್ಳು(&cmd.args, "FETCH_HEAD");
	cmd.git_cmd = 1;
	ಪ್ರತ್ಯಾಗಮ run_command(&cmd);
}

/**
 * Returns remote's upstream branch for the current branch. If remote is NULL,
 * the current branch's configured default remote is used. Returns NULL if
 * `remote` does not name a valid remote, HEAD does not point to a branch,
 * remote is not the branch's configured remote or the branch does not have any
 * configured upstream branch.
 */
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *get_upstream_branch(ಧ್ರುವ ವರ್ಣ *remote)
{
	ಸಂರಚನಾ remote *rm;
	ಸಂರಚನಾ branch *curr_branch;
	ಧ್ರುವ ವರ್ಣ *curr_branch_remote;

	rm = remote_get(remote);
	ಯದಿ (!rm)
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

	curr_branch = branch_get("HEAD");
	ಯದಿ (!curr_branch)
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

	curr_branch_remote = remote_for_branch(curr_branch, ರಿಕ್ತ);
	ಪ್ರತಿಜ್ಞಾ(curr_branch_remote);

	ಯದಿ (strcmp(curr_branch_remote, rm->name))
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

	ಪ್ರತ್ಯಾಗಮ branch_get_upstream(curr_branch, ರಿಕ್ತ);
}

/**
 * Derives the remote-tracking branch from the remote and refspec.
 *
 * FIXME: The current implementation assumes the default mapping of
 * refs/heads/<branch_name> to refs/remotes/<remote_name>/<branch_name>.
 */
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *get_tracking_branch(ಧ್ರುವ ವರ್ಣ *remote, ಧ್ರುವ ವರ್ಣ *refspec)
{
	ಸಂರಚನಾ refspec_item spec;
	ಧ್ರುವ ವರ್ಣ *spec_src;
	ಧ್ರುವ ವರ್ಣ *merge_branch;

	ಯದಿ (!refspec_item_init_fetch(&spec, refspec, the_hash_algo))
		die(_("invalid refspec '%s'"), refspec);
	spec_src = spec.src;
	ಯದಿ (!*spec_src || !strcmp(spec_src, "HEAD"))
		spec_src = "HEAD";
	ಅನ್ಯಥಾ ಯದಿ (skip_prefix(spec_src, "heads/", &spec_src))
		;
	ಅನ್ಯಥಾ ಯದಿ (skip_prefix(spec_src, "refs/heads/", &spec_src))
		;
	ಅನ್ಯಥಾ ಯದಿ (starts_with(spec_src, "refs/") ||
		starts_with(spec_src, "tags/") ||
		starts_with(spec_src, "remotes/"))
		spec_src = "";

	ಯದಿ (*spec_src) {
		ಯದಿ (!strcmp(remote, "."))
			merge_branch = mkpath("refs/heads/%s", spec_src);
		ಅನ್ಯಥಾ
			merge_branch = mkpath("refs/remotes/%s/%s", remote, spec_src);
	} ಅನ್ಯಥಾ
		merge_branch = ರಿಕ್ತ;

	refspec_item_clear(&spec);
	ಪ್ರತ್ಯಾಗಮ merge_branch;
}

/**
 * Given the repo and refspecs, sets fork_point to the point at which the
 * current branch forked from its remote-tracking branch. Returns 0 on success,
 * -1 on failure.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ get_rebase_fork_point(ಸಂರಚನಾ object_id *fork_point, ಧ್ರುವ ವರ್ಣ *repo,
		ಧ್ರುವ ವರ್ಣ *refspec)
{
	ಪೂರ್ಣಾಙ್ಕ ret;
	ಸಂರಚನಾ branch *curr_branch;
	ಧ್ರುವ ವರ್ಣ *remote_branch;
	ಸಂರಚನಾ child_process cp = CHILD_PROCESS_INIT;
	ಸಂರಚನಾ strbuf sb = STRBUF_INIT;

	curr_branch = branch_get("HEAD");
	ಯದಿ (!curr_branch)
		ಪ್ರತ್ಯಾಗಮ -1;

	ಯದಿ (refspec)
		remote_branch = get_tracking_branch(repo, refspec);
	ಅನ್ಯಥಾ
		remote_branch = get_upstream_branch(repo);

	ಯದಿ (!remote_branch)
		ಪ್ರತ್ಯಾಗಮ -1;

	strvec_pushl(&cp.args, "merge-base", "--fork-point",
		     remote_branch, curr_branch->name, ರಿಕ್ತ);
	cp.no_stdin = 1;
	cp.no_stderr = 1;
	cp.git_cmd = 1;

	ret = capture_command(&cp, &sb, GIT_MAX_HEXSZ);
	ಯದಿ (ret)
		ಗಚ್ಛ cleanup;

	ret = get_oid_hex(sb.buf, fork_point);
	ಯದಿ (ret)
		ಗಚ್ಛ cleanup;

cleanup:
	strbuf_release(&sb);
	ಪ್ರತ್ಯಾಗಮ ret ? -1 : 0;
}

/**
 * Sets merge_base to the octopus merge base of curr_head, merge_head and
 * fork_point. Returns 0 if a merge base is found, 1 otherwise.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ get_octopus_merge_base(ಸಂರಚನಾ object_id *merge_base,
		ಧ್ರುವ ಸಂರಚನಾ object_id *curr_head,
		ಧ್ರುವ ಸಂರಚನಾ object_id *merge_head,
		ಧ್ರುವ ಸಂರಚನಾ object_id *fork_point)
{
	ಸಂರಚನಾ commit_list *revs = ರಿಕ್ತ, *result = ರಿಕ್ತ;

	commit_list_ಸೇರಿಸು(lookup_commit_reference(the_repository, curr_head),
			   &revs);
	commit_list_ಸೇರಿಸು(lookup_commit_reference(the_repository, merge_head),
			   &revs);
	ಯದಿ (!is_null_oid(fork_point))
		commit_list_ಸೇರಿಸು(lookup_commit_reference(the_repository, fork_point),
				   &revs);

	ಯದಿ (get_octopus_merge_bases(revs, &result) < 0)
		exit(128);
	commit_list_free(revs);
	reduce_heads_replace(&result);

	ಯದಿ (!result)
		ಪ್ರತ್ಯಾಗಮ 1;

	oidcpy(merge_base, &result->item->object.oid);
	commit_list_free(result);
	ಪ್ರತ್ಯಾಗಮ 0;
}

/**
 * Given the current HEAD oid, the merge head returned from git-fetch and the
 * fork point calculated by get_rebase_fork_point(), compute the <newbase> and
 * <upstream> arguments to use for the upcoming git-rebase invocation.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ get_rebase_newbase_and_upstream(ಸಂರಚನಾ object_id *newbase,
		ಸಂರಚನಾ object_id *upstream,
		ಧ್ರುವ ಸಂರಚನಾ object_id *curr_head,
		ಧ್ರುವ ಸಂರಚನಾ object_id *merge_head,
		ಧ್ರುವ ಸಂರಚನಾ object_id *fork_point)
{
	ಸಂರಚನಾ object_id oct_merge_base;

	ಯದಿ (!get_octopus_merge_base(&oct_merge_base, curr_head, merge_head, fork_point))
		ಯದಿ (!is_null_oid(fork_point) && oideq(&oct_merge_base, fork_point))
			fork_point = ರಿಕ್ತ;

	ಯದಿ (fork_point && !is_null_oid(fork_point))
		oidcpy(upstream, fork_point);
	ಅನ್ಯಥಾ
		oidcpy(upstream, merge_head);

	oidcpy(newbase, merge_head);

	ಪ್ರತ್ಯಾಗಮ 0;
}

/**
 * Given the <newbase> and <upstream> calculated by
 * get_rebase_newbase_and_upstream(), runs git-rebase with the
 * appropriate arguments and returns its exit status.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ run_rebase(ಧ್ರುವ ಸಂರಚನಾ object_id *newbase,
		ಧ್ರುವ ಸಂರಚನಾ object_id *upstream)
{
	ಸಂರಚನಾ child_process cmd = CHILD_PROCESS_INIT;

	strvec_ತಳ್ಳು(&cmd.args, "rebase");

	/* Shared options */
	argv_ತಳ್ಳು_verbosity(&cmd.args);

	/* Options passed to git-rebase */
	ಯದಿ (opt_rebase == REBASE_MERGES)
		strvec_ತಳ್ಳು(&cmd.args, "--rebase-merges");
	ಅನ್ಯಥಾ ಯದಿ (opt_rebase == REBASE_INTERACTIVE)
		strvec_ತಳ್ಳು(&cmd.args, "--interactive");
	ಯದಿ (opt_diffstat)
		strvec_ತಳ್ಳು(&cmd.args, opt_diffstat);
	strvec_pushv(&cmd.args, opt_strategies.v);
	strvec_pushv(&cmd.args, opt_strategy_opts.v);
	ಯದಿ (opt_gpg_sign)
		strvec_ತಳ್ಳು(&cmd.args, opt_gpg_sign);
	ಯದಿ (opt_signoff)
		strvec_ತಳ್ಳು(&cmd.args, opt_signoff);
	ಯದಿ (opt_autostash == 0)
		strvec_ತಳ್ಳು(&cmd.args, "--no-autostash");
	ಅನ್ಯಥಾ ಯದಿ (opt_autostash == 1)
		strvec_ತಳ್ಳು(&cmd.args, "--autostash");
	ಯದಿ (opt_verify_signatures &&
	    !strcmp(opt_verify_signatures, "--verify-signatures"))
		warning(_("ignoring --verify-signatures for rebase"));

	strvec_ತಳ್ಳು(&cmd.args, "--onto");
	strvec_ತಳ್ಳು(&cmd.args, oid_to_hex(newbase));

	strvec_ತಳ್ಳು(&cmd.args, oid_to_hex(upstream));

	cmd.git_cmd = 1;
	ಪ್ರತ್ಯಾಗಮ run_command(&cmd);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ get_can_ff(ಸಂರಚನಾ object_id *orig_head,
		      ಸಂರಚನಾ oid_array *merge_heads)
{
	ಪೂರ್ಣಾಙ್ಕ ret;
	ಸಂರಚನಾ commit_list *list = ರಿಕ್ತ;
	ಸಂರಚನಾ commit *merge_head, *head;
	ಸಂರಚನಾ object_id *orig_merge_head;

	ಯದಿ (merge_heads->nr > 1)
		ಪ್ರತ್ಯಾಗಮ 0;

	orig_merge_head = &merge_heads->oid[0];
	head = lookup_commit_reference(the_repository, orig_head);
	ಯದಿ (!head)
		ಪ್ರತ್ಯಾಗಮ 0;
	merge_head = lookup_commit_reference(the_repository, orig_merge_head);
	ಯದಿ (!merge_head)
		ಪ್ರತ್ಯಾಗಮ 0;
	commit_list_ಸೇರಿಸು(head, &list);
	ret = repo_is_descendant_of(the_repository, merge_head, list);
	commit_list_free(list);
	ಯದಿ (ret < 0)
		exit(128);
	ಪ್ರತ್ಯಾಗಮ ret;
}

/*
 * Is orig_head a descendant of _all_ merge_heads?
 * Unfortunately is_descendant_of() cannot be used as it asks
 * if orig_head is a descendant of at least one of them.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ already_up_to_date(ಸಂರಚನಾ object_id *orig_head,
			      ಸಂರಚನಾ oid_array *merge_heads)
{
	ಸಂರಚನಾ commit *ours;

	ours = lookup_commit_reference(the_repository, orig_head);
	ಯದಿ (!ours)
		ಪ್ರತ್ಯಾಗಮ 0;
	ಪ್ರತಿ (size_t i = 0; i < merge_heads->nr; i++) {
		ಸಂರಚನಾ commit_list *list = ರಿಕ್ತ;
		ಸಂರಚನಾ commit *theirs;
		ಪೂರ್ಣಾಙ್ಕ ok;

		theirs = lookup_commit_reference(the_repository, &merge_heads->oid[i]);
		ಯದಿ (!theirs)
			ಪ್ರತ್ಯಾಗಮ 0;
		commit_list_ಸೇರಿಸು(theirs, &list);
		ok = repo_is_descendant_of(the_repository, ours, list);
		commit_list_free(list);
		ಯದಿ (ok < 0)
			exit(128);
		ಯದಿ (!ok)
			ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಪ್ರತ್ಯಾಗಮ 1;
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

ಪೂರ್ಣಾಙ್ಕ cmd_pull(ಪೂರ್ಣಾಙ್ಕ argc,
	     ಧ್ರುವ ವರ್ಣ **argv,
	     ಧ್ರುವ ವರ್ಣ *prefix,
	     ಸಂರಚನಾ repository *repository UNUSED)
{
	ಧ್ರುವ ವರ್ಣ *repo, **refspecs;
	ಸಂರಚನಾ oid_array merge_heads = OID_ARRAY_INIT;
	ಸಂರಚನಾ object_id orig_head, curr_head;
	ಸಂರಚನಾ object_id rebase_fork_point;
	ಪೂರ್ಣಾಙ್ಕ rebase_unspecified = 0;
	ಪೂರ್ಣಾಙ್ಕ can_ff;
	ಪೂರ್ಣಾಙ್ಕ divergent;
	ಪೂರ್ಣಾಙ್ಕ ret;
	ಸ್ಥಿರ ಸಂರಚನಾ option pull_options[] = {
		/* Shared options */
		OPT__VERBOSITY(&opt_verbosity),
		OPT_PASSTHRU(0, "progress", &opt_progress, ರಿಕ್ತ,
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
		OPT_PASSTHRU('n', ರಿಕ್ತ, &opt_diffstat, ರಿಕ್ತ,
			N_("do not show a diffstat at the end of the merge"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG),
		OPT_PASSTHRU(0, "stat", &opt_diffstat, ರಿಕ್ತ,
			N_("show a diffstat at the end of the merge"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "summary", &opt_diffstat, ರಿಕ್ತ,
			N_("(synonym to --stat)"),
			PARSE_OPT_NOARG | PARSE_OPT_HIDDEN),
		OPT_PASSTHRU(0, "compact-summary", &opt_diffstat, ರಿಕ್ತ,
			N_("show a compact-summary at the end of the merge"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "log", &opt_log, N_("n"),
			N_("add (at most <n>) entries from shortlog to merge commit message"),
			PARSE_OPT_OPTARG),
		OPT_PASSTHRU(0, "signoff", &opt_signoff, ರಿಕ್ತ,
			N_("add a Signed-off-by trailer"),
			PARSE_OPT_OPTARG),
		OPT_PASSTHRU(0, "squash", &opt_squash, ರಿಕ್ತ,
			N_("create a single commit instead of doing a merge"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "commit", &opt_commit, ರಿಕ್ತ,
			N_("perform a commit if the merge succeeds (default)"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "edit", &opt_edit, ರಿಕ್ತ,
			N_("edit message before committing"),
			PARSE_OPT_NOARG),
		OPT_CLEANUP(&cleanup_arg),
		OPT_PASSTHRU(0, "ff", &opt_ff, ರಿಕ್ತ,
			N_("allow fast-forward"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "ff-only", &opt_ff, ರಿಕ್ತ,
			N_("abort if fast-forward is not possible"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG),
		OPT_PASSTHRU(0, "verify", &opt_verify, ರಿಕ್ತ,
			N_("control use of pre-merge-commit and commit-msg hooks"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "verify-signatures", &opt_verify_signatures, ರಿಕ್ತ,
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
		OPT_PASSTHRU(0, "all", &opt_all, ರಿಕ್ತ,
			N_("fetch from all remotes"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU('a', "append", &opt_append, ರಿಕ್ತ,
			N_("append to .git/FETCH_HEAD instead of overwriting"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "upload-pack", &opt_upload_pack, N_("path"),
			N_("path to upload pack on remote end"),
			0),
		OPT__FORCE(&opt_force, N_("force overwrite of local branch"), 0),
		OPT_PASSTHRU('t', "tags", &opt_tags, ರಿಕ್ತ,
			N_("fetch all tags and associated objects"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU('p', "prune", &opt_prune, ರಿಕ್ತ,
			N_("prune remote-tracking branches no longer on remote"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU('j', "jobs", &max_children, N_("n"),
			N_("number of submodules pulled in parallel"),
			PARSE_OPT_OPTARG),
		OPT_BOOL(0, "dry-run", &opt_dry_run,
			N_("dry run")),
		OPT_PASSTHRU('k', "keep", &opt_keep, ರಿಕ್ತ,
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
		OPT_PASSTHRU(0, "unshallow", &opt_unshallow, ರಿಕ್ತ,
			N_("convert to a complete repository"),
			PARSE_OPT_NONEG | PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "update-shallow", &opt_update_shallow, ರಿಕ್ತ,
			N_("accept refs that update .git/shallow"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "refmap", &opt_refmap, N_("refmap"),
			N_("specify fetch refmap"),
			PARSE_OPT_NONEG),
		OPT_PASSTHRU_ARGV('o', "server-option", &opt_fetch,
			N_("server-specific"),
			N_("option to transmit"),
			0),
		OPT_PASSTHRU('4',  "ipv4", &opt_ipv4, ರಿಕ್ತ,
			N_("use IPv4 addresses only"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU('6',  "ipv6", &opt_ipv6, ರಿಕ್ತ,
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
		OPT_PASSTHRU(0, "set-upstream", &set_upstream, ರಿಕ್ತ,
			N_("set upstream for git pull/fetch"),
			PARSE_OPT_NOARG),

		OPT_END()
	};

	ಯದಿ (!getenv("GIT_REFLOG_ACTION"))
		set_reflog_message(argc, argv);

	repo_config(the_repository, git_pull_config, ರಿಕ್ತ);
	ಯದಿ (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_full_index = 0;
	}

	argc = parse_options(argc, argv, prefix, pull_options, pull_usage, 0);
	ಯದಿ (opt_autostash == -1)
		opt_autostash = config_pull_autostash;

	ಯದಿ (recurse_submodules_cli != RECURSE_SUBMODULES_DEFAULT)
		recurse_submodules = recurse_submodules_cli;

	ಯದಿ (cleanup_arg)
		/*
		 * this only checks the validity of cleanup_arg; we don't need
		 * a valid value for use_editor
		 */
		get_cleanup_mode(cleanup_arg, 0);

	parse_repo_refspecs(argc, argv, &repo, &refspecs);

	ಯದಿ (!opt_ff) {
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
		ಯದಿ (opt_rebase >= 0 && opt_ff && !strcmp(opt_ff, "--ff-only")) {
			ಮುಕ್ತ(opt_ff);
			opt_ff = xstrdup("--ff");
		}
	}

	ಯದಿ (opt_rebase < 0)
		opt_rebase = config_get_rebase(&rebase_unspecified);

	ಯದಿ (repo_read_index_unmerged(the_repository))
		die_resolve_conflict("pull");

	ಯದಿ (file_exists(git_path_merge_head(the_repository)))
		die_conclude_merge();

	ಯದಿ (repo_get_oid(the_repository, "HEAD", &orig_head))
		oidclr(&orig_head, the_repository->hash_algo);

	ಯದಿ (opt_rebase) {
		ಯದಿ (opt_autostash == -1)
			opt_autostash = config_rebase_autostash;

		ಯದಿ (is_null_oid(&orig_head) && !is_index_unborn(the_repository->index))
			die(_("Updating an unborn branch with changes added to the index."));

		ಯದಿ (!opt_autostash)
			require_clean_work_ವೃಕ್ಷ(the_repository,
				N_("pull with rebase"),
				_("Please commit or stash them."), 1, 0);

		ಯದಿ (get_rebase_fork_point(&rebase_fork_point, repo, *refspecs))
			oidclr(&rebase_fork_point, the_repository->hash_algo);
	}

	ಯದಿ (run_fetch(repo, refspecs))
		ಪ್ರತ್ಯಾಗಮ 1;

	ಯದಿ (opt_dry_run)
		ಪ್ರತ್ಯಾಗಮ 0;

	ಯದಿ (repo_get_oid(the_repository, "HEAD", &curr_head))
		oidclr(&curr_head, the_repository->hash_algo);

	ಯದಿ (!is_null_oid(&orig_head) && !is_null_oid(&curr_head) &&
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

		ಯದಿ (checkout_fast_forward(the_repository, &orig_head,
					  &curr_head, 0))
			die(_("Cannot fast-forward your working tree.\n"
				"After making sure that you saved anything precious from\n"
				"$ git diff %s\n"
				"output, run\n"
				"$ git reset --hard\n"
				"to recover."), oid_to_hex(&orig_head));
	}

	get_merge_heads(&merge_heads);

	ಯದಿ (!merge_heads.nr)
		die_no_merge_candidates(repo, refspecs);

	ಯದಿ (is_null_oid(&orig_head)) {
		ಯದಿ (merge_heads.nr > 1)
			die(_("Cannot merge multiple branches into empty head."));
		ret = pull_into_void(merge_heads.oid, &curr_head);
		ಗಚ್ಛ cleanup;
	}
	ಯದಿ (merge_heads.nr > 1) {
		ಯದಿ (opt_rebase)
			die(_("Cannot rebase onto multiple branches."));
		ಯದಿ (opt_ff && !strcmp(opt_ff, "--ff-only"))
			die(_("Cannot fast-forward to multiple branches."));
	}

	can_ff = get_can_ff(&orig_head, &merge_heads);
	divergent = !can_ff && !already_up_to_date(&orig_head, &merge_heads);

	/* ff-only takes precedence over rebase */
	ಯದಿ (opt_ff && !strcmp(opt_ff, "--ff-only")) {
		ಯದಿ (divergent)
			die_ff_impossible();
		opt_rebase = REBASE_FALSE;
	}
	/* If no action specified and we can't fast forward, then warn. */
	ಯದಿ (!opt_ff && rebase_unspecified && divergent) {
		show_advice_pull_non_ff();
		die(_("Need to specify how to reconcile divergent branches."));
	}

	ಯದಿ (opt_rebase) {
		ಸಂರಚನಾ object_id newbase;
		ಸಂರಚನಾ object_id upstream;
		get_rebase_newbase_and_upstream(&newbase, &upstream, &curr_head,
						merge_heads.oid, &rebase_fork_point);

		ಯದಿ ((recurse_submodules == RECURSE_SUBMODULES_ON ||
		     recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND) &&
		    submodule_touches_in_range(the_repository, &upstream, &curr_head))
			die(_("cannot rebase with locally recorded submodule modifications"));

		ಯದಿ (can_ff) {
			/* we can fast-forward this without invoking rebase */
			ಮುಕ್ತ(opt_ff);
			opt_ff = xstrdup("--ff-only");
			ret = run_merge();
		} ಅನ್ಯಥಾ {
			ret = run_rebase(&newbase, &upstream);
		}

		ಯದಿ (!ret && (recurse_submodules == RECURSE_SUBMODULES_ON ||
			     recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND))
			ret = rebase_submodules();

		ಗಚ್ಛ cleanup;
	} ಅನ್ಯಥಾ {
		ret = run_merge();
		ಯದಿ (!ret && (recurse_submodules == RECURSE_SUBMODULES_ON ||
			     recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND))
			ret = update_submodules();
		ಗಚ್ಛ cleanup;
	}

cleanup:
	oid_array_clear(&merge_heads);
	ಪ್ರತ್ಯಾಗಮ ret;
}
