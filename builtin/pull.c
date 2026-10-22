/*
 * Builtin "git pull"
 *
 * Based on git-pull.sh by Junio C Hamano
 *
 * Fetch one or more remote refs and merge it/them into the current HEAD.
 */

#define USE_THE_REPOSITORY_VARIABLE

#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "advice.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "hex.h"
#അന്തര്ഭാവ "merge.h"
#അന്തര്ഭാവ "object-name.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "run-command.h"
#അന്തര്ഭാവ "oid-array.h"
#അന്തര്ഭാവ "remote.h"
#അന്തര്ഭാവ "dir.h"
#അന്തര്ഭാവ "path.h"
#അന്തര്ഭാവ "read-cache-ll.h"
#അന്തര്ഭാവ "rebase.h"
#അന്തര്ഭാവ "refs.h"
#അന്തര്ഭാവ "refspec.h"
#അന്തര്ഭാവ "submodule.h"
#അന്തര്ഭാവ "submodule-config.h"
#അന്തര്ഭാവ "wt-status.h"
#അന്തര്ഭാവ "commit-reach.h"
#അന്തര്ഭാവ "sequencer.h"

/**
 * Parses the value of --rebase. If value is a false value, returns
 * REBASE_FALSE. If value is a true value, returns REBASE_TRUE. If value is
 * "merges", returns REBASE_MERGES. If value is a invalid value, dies with
 * a fatal error if fatal is true, otherwise returns REBASE_INVALID.
 */
സ്ഥിതം എണ്ണൽ rebase_type parse_config_rebase(സ്ഥിരം അക്ഷരം *താക്കോൽ, സ്ഥിരം അക്ഷരം *മൂല്യം,
		പൂർണ്ണസംഖ്യ fatal)
{
	എണ്ണൽ rebase_type v = rebase_parse_മൂല്യം(മൂല്യം);
	എങ്കിൽ (v != REBASE_INVALID)
		തിരികെ v;

	എങ്കിൽ (fatal)
		die(_("invalid value for '%s': '%s'"), താക്കോൽ, മൂല്യം);
	അല്ലെങ്കിൽ
		error(_("invalid value for '%s': '%s'"), താക്കോൽ, മൂല്യം);

	തിരികെ REBASE_INVALID;
}

/**
 * Callback for --rebase, which parses arg with parse_config_rebase().
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ parse_opt_rebase(സ്ഥിരം ഘടന option *opt, സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	എണ്ണൽ rebase_type *മൂല്യം = opt->മൂല്യം;

	എങ്കിൽ (arg)
		*മൂല്യം = parse_config_rebase("--rebase", arg, 0);
	അല്ലെങ്കിൽ
		*മൂല്യം = unset ? REBASE_FALSE : REBASE_TRUE;
	തിരികെ *മൂല്യം == REBASE_INVALID ? -1 : 0;
}

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം pull_usage[] = {
	N_("git pull [<options>] [<repository> [<refspec>...]]"),
	ശൂന്യ
};

/* Shared options */
സ്ഥിതം പൂർണ്ണസംഖ്യ opt_verbosity;
സ്ഥിതം സ്ഥിരം അക്ഷരം *opt_progress;
സ്ഥിതം പൂർണ്ണസംഖ്യ recurse_submodules = RECURSE_SUBMODULES_DEFAULT;
സ്ഥിതം പൂർണ്ണസംഖ്യ recurse_submodules_cli = RECURSE_SUBMODULES_DEFAULT;

/* Options passed to git-merge or git-rebase */
സ്ഥിതം എണ്ണൽ rebase_type opt_rebase = -1;
സ്ഥിതം സ്ഥിരം അക്ഷരം *opt_diffstat;
സ്ഥിതം സ്ഥിരം അക്ഷരം *opt_log;
സ്ഥിതം സ്ഥിരം അക്ഷരം *opt_signoff;
സ്ഥിതം സ്ഥിരം അക്ഷരം *opt_squash;
സ്ഥിതം സ്ഥിരം അക്ഷരം *opt_commit;
സ്ഥിതം സ്ഥിരം അക്ഷരം *opt_edit;
സ്ഥിതം സ്ഥിരം അക്ഷരം *cleanup_arg;
സ്ഥിതം അക്ഷരം *opt_ff;
സ്ഥിതം സ്ഥിരം അക്ഷരം *opt_verify_signatures;
സ്ഥിതം സ്ഥിരം അക്ഷരം *opt_verify;
സ്ഥിതം പൂർണ്ണസംഖ്യ opt_autostash = -1;
സ്ഥിതം പൂർണ്ണസംഖ്യ config_rebase_autostash;
സ്ഥിതം പൂർണ്ണസംഖ്യ config_pull_autostash = -1;
സ്ഥിതം പൂർണ്ണസംഖ്യ check_trust_level = 1;
സ്ഥിതം ഘടന strvec opt_strategies = STRVEC_INIT;
സ്ഥിതം ഘടന strvec opt_strategy_opts = STRVEC_INIT;
സ്ഥിതം സ്ഥിരം അക്ഷരം *opt_gpg_sign;
സ്ഥിതം പൂർണ്ണസംഖ്യ opt_allow_unrelated_histories;

/* Options passed to git-fetch */
സ്ഥിതം സ്ഥിരം അക്ഷരം *opt_all;
സ്ഥിതം സ്ഥിരം അക്ഷരം *opt_append;
സ്ഥിതം സ്ഥിരം അക്ഷരം *opt_upload_pack;
സ്ഥിതം പൂർണ്ണസംഖ്യ opt_force;
സ്ഥിതം സ്ഥിരം അക്ഷരം *opt_tags;
സ്ഥിതം സ്ഥിരം അക്ഷരം *opt_prune;
സ്ഥിതം സ്ഥിരം അക്ഷരം *max_children;
സ്ഥിതം പൂർണ്ണസംഖ്യ opt_dry_run;
സ്ഥിതം സ്ഥിരം അക്ഷരം *opt_keep;
സ്ഥിതം സ്ഥിരം അക്ഷരം *opt_depth;
സ്ഥിതം സ്ഥിരം അക്ഷരം *opt_unshallow;
സ്ഥിതം സ്ഥിരം അക്ഷരം *opt_update_shallow;
സ്ഥിതം സ്ഥിരം അക്ഷരം *opt_refmap;
സ്ഥിതം സ്ഥിരം അക്ഷരം *opt_ipv4;
സ്ഥിതം സ്ഥിരം അക്ഷരം *opt_ipv6;
സ്ഥിതം പൂർണ്ണസംഖ്യ opt_show_forced_updates = -1;
സ്ഥിതം സ്ഥിരം അക്ഷരം *set_upstream;
സ്ഥിതം ഘടന strvec opt_fetch = STRVEC_INIT;

/**
 * Pushes "-q" or "-v" switches into arr to match the opt_verbosity level.
 */
സ്ഥിതം ശൂന്യം argv_തള്ളുക_verbosity(ഘടന strvec *arr)
{
	പൂർണ്ണസംഖ്യ verbosity;

	വേണ്ടി (verbosity = opt_verbosity; verbosity > 0; verbosity--)
		strvec_തള്ളുക(arr, "-v");

	വേണ്ടി (verbosity = opt_verbosity; verbosity < 0; verbosity++)
		strvec_തള്ളുക(arr, "-q");
}

/**
 * Pushes "-f" switches into arr to match the opt_force level.
 */
സ്ഥിതം ശൂന്യം argv_തള്ളുക_force(ഘടന strvec *arr)
{
	പൂർണ്ണസംഖ്യ force = opt_force;
	ആകുമ്പോൾ (force-- > 0)
		strvec_തള്ളുക(arr, "-f");
}

/**
 * Sets the GIT_REFLOG_ACTION environment variable to the concatenation of argv
 */
സ്ഥിതം ശൂന്യം set_reflog_message(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv)
{
	പൂർണ്ണസംഖ്യ i;
	ഘടന strbuf സന്ദേശം = STRBUF_INIT;

	വേണ്ടി (i = 0; i < argc; i++) {
		എങ്കിൽ (i)
			strbuf_addch(&സന്ദേശം, ' ');
		strbuf_addstr(&സന്ദേശം, argv[i]);
	}

	setenv("GIT_REFLOG_ACTION", സന്ദേശം.buf, 0);

	strbuf_release(&സന്ദേശം);
}

/**
 * If pull.ff is unset, returns NULL. If pull.ff is "true", returns "--ff". If
 * pull.ff is "false", returns "--no-ff". If pull.ff is "only", returns
 * "--ff-only". Otherwise, if pull.ff is set to an invalid value, die with an
 * error.
 */
സ്ഥിതം സ്ഥിരം അക്ഷരം *config_get_ff(ശൂന്യം)
{
	സ്ഥിരം അക്ഷരം *മൂല്യം;

	എങ്കിൽ (repo_config_get_മൂല്യം(the_repository, "pull.ff", &മൂല്യം))
		തിരികെ ശൂന്യ;

	തിരഞ്ഞെടുപ്പ് (git_parse_maybe_bool(മൂല്യം)) {
	അവസ്ഥ 0:
		തിരികെ "--no-ff";
	അവസ്ഥ 1:
		തിരികെ "--ff";
	}

	എങ്കിൽ (!strcmp(മൂല്യം, "only"))
		തിരികെ "--ff-only";

	die(_("invalid value for '%s': '%s'"), "pull.ff", മൂല്യം);
}

/**
 * Returns the default configured value for --rebase. It first looks for the
 * value of "branch.$curr_branch.rebase", where $curr_branch is the current
 * branch, and if HEAD is detached or the configuration key does not exist,
 * looks for the value of "pull.rebase". If both configuration keys do not
 * exist, returns REBASE_FALSE.
 */
സ്ഥിതം എണ്ണൽ rebase_type config_get_rebase(പൂർണ്ണസംഖ്യ *rebase_unspecified)
{
	ഘടന branch *curr_branch = branch_get("HEAD");
	സ്ഥിരം അക്ഷരം *മൂല്യം;

	എങ്കിൽ (curr_branch) {
		അക്ഷരം *താക്കോൽ = xstrfmt("branch.%s.rebase", curr_branch->name);

		എങ്കിൽ (!repo_config_get_മൂല്യം(the_repository, താക്കോൽ, &മൂല്യം)) {
			എണ്ണൽ rebase_type ret = parse_config_rebase(താക്കോൽ, മൂല്യം, 1);
			സ്വതന്ത്രം(താക്കോൽ);
			തിരികെ ret;
		}

		സ്വതന്ത്രം(താക്കോൽ);
	}

	എങ്കിൽ (!repo_config_get_മൂല്യം(the_repository, "pull.rebase", &മൂല്യം))
		തിരികെ parse_config_rebase("pull.rebase", മൂല്യം, 1);

	*rebase_unspecified = 1;

	തിരികെ REBASE_FALSE;
}

/**
 * Read config variables.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ git_pull_config(സ്ഥിരം അക്ഷരം *var, സ്ഥിരം അക്ഷരം *മൂല്യം,
			   സ്ഥിരം ഘടന config_context *ctx, ശൂന്യം *cb)
{
	എങ്കിൽ (!strcmp(var, "rebase.autostash")) {
		/*
		 * run_rebase() also reads this option. The reason we handle it here is
		 * that when pull.rebase is true, a fast-forward may occur without
		 * invoking run_rebase(). We need to ensure that autostash is set even
		 * in the fast-forward case.
		 *
		 * run_merge() handles merge.autostash, so we don't handle it here.
		 */
		config_rebase_autostash = git_config_bool(var, മൂല്യം);
		തിരികെ 0;
	} അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(var, "pull.autostash")) {
		config_pull_autostash = git_config_bool(var, മൂല്യം);
		തിരികെ 0;
	} അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(var, "submodule.recurse")) {
		recurse_submodules = git_config_bool(var, മൂല്യം) ?
			RECURSE_SUBMODULES_ON : RECURSE_SUBMODULES_OFF;
		തിരികെ 0;
	} അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(var, "gpg.mintrustlevel")) {
		check_trust_level = 0;
	}

	തിരികെ git_default_config(var, മൂല്യം, ctx, cb);
}

/**
 * Appends merge candidates from FETCH_HEAD that are not marked not-for-merge
 * into merge_heads.
 */
സ്ഥിതം ശൂന്യം get_merge_heads(ഘടന oid_array *merge_heads)
{
	സ്ഥിരം അക്ഷരം *filename = git_path_fetch_head(the_repository);
	FILE *fp;
	ഘടന strbuf sb = STRBUF_INIT;
	ഘടന object_id oid;

	fp = xfopen(filename, "r");
	ആകുമ്പോൾ (strbuf_getline_lf(&sb, fp) != EOF) {
		സ്ഥിരം അക്ഷരം *p;
		എങ്കിൽ (parse_oid_hex(sb.buf, &oid, &p))
			തുടരുക;  /* invalid line: does not start with object ID */
		എങ്കിൽ (starts_with(p, "\tnot-for-merge\t"))
			തുടരുക;  /* ref is not-for-merge */
		oid_array_append(merge_heads, &oid);
	}
	fclose(fp);
	strbuf_release(&sb);
}

/**
 * Used by die_no_merge_candidates() as a for_each_remote() callback to
 * retrieve the name of the remote if the repository only has one remote.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ get_only_remote(ഘടന remote *remote, ശൂന്യം *cb_data)
{
	സ്ഥിരം അക്ഷരം **remote_name = cb_data;

	എങ്കിൽ (*remote_name)
		തിരികെ -1;

	*remote_name = remote->name;
	തിരികെ 0;
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
സ്ഥിതം ശൂന്യം NORETURN die_no_merge_candidates(സ്ഥിരം അക്ഷരം *repo, സ്ഥിരം അക്ഷരം **refspecs)
{
	ഘടന branch *curr_branch = branch_get("HEAD");
	സ്ഥിരം അക്ഷരം *remote = curr_branch ? curr_branch->remote_name : ശൂന്യ;

	എങ്കിൽ (*refspecs) {
		എങ്കിൽ (opt_rebase)
			fprintf_ln(stderr, _("There is no candidate for rebasing against among the refs that you just fetched."));
		അല്ലെങ്കിൽ
			fprintf_ln(stderr, _("There are no candidates for merging among the refs that you just fetched."));
		fprintf_ln(stderr, _("Generally this means that you provided a wildcard refspec which had no\n"
					"matches on the remote end."));
	} അല്ലെങ്കിൽ എങ്കിൽ (repo && curr_branch && (!remote || strcmp(repo, remote))) {
		fprintf_ln(stderr, _("You asked to pull from the remote '%s', but did not specify\n"
			"a branch. Because this is not the default configured remote\n"
			"for your current branch, you must specify a branch on the command line."),
			repo);
	} അല്ലെങ്കിൽ എങ്കിൽ (!curr_branch) {
		fprintf_ln(stderr, _("You are not currently on a branch."));
		എങ്കിൽ (opt_rebase)
			fprintf_ln(stderr, _("Please specify which branch you want to rebase against."));
		അല്ലെങ്കിൽ
			fprintf_ln(stderr, _("Please specify which branch you want to merge with."));
		fprintf_ln(stderr, _("See git-pull(1) for details."));
		fprintf(stderr, "\n");
		fprintf_ln(stderr, "    git pull %s %s", _("<remote>"), _("<branch>"));
		fprintf(stderr, "\n");
	} അല്ലെങ്കിൽ എങ്കിൽ (!curr_branch->merge_nr) {
		സ്ഥിരം അക്ഷരം *remote_name = ശൂന്യ;

		എങ്കിൽ (for_each_remote(get_only_remote, &remote_name) || !remote_name)
			remote_name = _("<remote>");

		fprintf_ln(stderr, _("There is no tracking information for the current branch."));
		എങ്കിൽ (opt_rebase)
			fprintf_ln(stderr, _("Please specify which branch you want to rebase against."));
		അല്ലെങ്കിൽ
			fprintf_ln(stderr, _("Please specify which branch you want to merge with."));
		fprintf_ln(stderr, _("See git-pull(1) for details."));
		fprintf(stderr, "\n");
		fprintf_ln(stderr, "    git pull %s %s", _("<remote>"), _("<branch>"));
		fprintf(stderr, "\n");
		fprintf_ln(stderr, _("If you wish to set tracking information for this branch you can do so with:"));
		fprintf(stderr, "\n");
		fprintf_ln(stderr, "    git branch --set-upstream-to=%s/%s %s\n",
				remote_name, _("<branch>"), curr_branch->name);
	} അല്ലെങ്കിൽ
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
സ്ഥിതം ശൂന്യം parse_repo_refspecs(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം **repo,
		സ്ഥിരം അക്ഷരം ***refspecs)
{
	എങ്കിൽ (argc > 0) {
		*repo = *argv++;
		argc--;
	} അല്ലെങ്കിൽ
		*repo = ശൂന്യ;
	*refspecs = argv;
}

/**
 * Runs git-fetch, returning its exit status. `repo` and `refspecs` are the
 * repository and refspecs to fetch, or NULL if they are not provided.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ run_fetch(സ്ഥിരം അക്ഷരം *repo, സ്ഥിരം അക്ഷരം **refspecs)
{
	ഘടന child_process cmd = CHILD_PROCESS_INIT;

	strvec_pushl(&cmd.args, "fetch", "--update-head-ok", ശൂന്യ);

	/* Shared options */
	argv_തള്ളുക_verbosity(&cmd.args);
	എങ്കിൽ (opt_progress)
		strvec_തള്ളുക(&cmd.args, opt_progress);

	/* Options passed to git-fetch */
	എങ്കിൽ (opt_all)
		strvec_തള്ളുക(&cmd.args, opt_all);
	എങ്കിൽ (opt_append)
		strvec_തള്ളുക(&cmd.args, opt_append);
	എങ്കിൽ (opt_upload_pack)
		strvec_തള്ളുക(&cmd.args, opt_upload_pack);
	argv_തള്ളുക_force(&cmd.args);
	എങ്കിൽ (opt_tags)
		strvec_തള്ളുക(&cmd.args, opt_tags);
	എങ്കിൽ (opt_prune)
		strvec_തള്ളുക(&cmd.args, opt_prune);
	എങ്കിൽ (recurse_submodules_cli != RECURSE_SUBMODULES_DEFAULT)
		തിരഞ്ഞെടുപ്പ് (recurse_submodules_cli) {
		അവസ്ഥ RECURSE_SUBMODULES_ON:
			strvec_തള്ളുക(&cmd.args, "--recurse-submodules=on");
			നിർത്തുക;
		അവസ്ഥ RECURSE_SUBMODULES_OFF:
			strvec_തള്ളുക(&cmd.args, "--recurse-submodules=no");
			നിർത്തുക;
		അവസ്ഥ RECURSE_SUBMODULES_ON_DEMAND:
			strvec_തള്ളുക(&cmd.args, "--recurse-submodules=on-demand");
			നിർത്തുക;
		സ്ഥിരസ്ഥിതി:
			BUG("submodule recursion option not understood");
		}
	എങ്കിൽ (max_children)
		strvec_തള്ളുക(&cmd.args, max_children);
	എങ്കിൽ (opt_dry_run)
		strvec_തള്ളുക(&cmd.args, "--dry-run");
	എങ്കിൽ (opt_keep)
		strvec_തള്ളുക(&cmd.args, opt_keep);
	എങ്കിൽ (opt_depth)
		strvec_തള്ളുക(&cmd.args, opt_depth);
	എങ്കിൽ (opt_unshallow)
		strvec_തള്ളുക(&cmd.args, opt_unshallow);
	എങ്കിൽ (opt_update_shallow)
		strvec_തള്ളുക(&cmd.args, opt_update_shallow);
	എങ്കിൽ (opt_refmap)
		strvec_തള്ളുക(&cmd.args, opt_refmap);
	എങ്കിൽ (opt_ipv4)
		strvec_തള്ളുക(&cmd.args, opt_ipv4);
	എങ്കിൽ (opt_ipv6)
		strvec_തള്ളുക(&cmd.args, opt_ipv6);
	എങ്കിൽ (opt_show_forced_updates > 0)
		strvec_തള്ളുക(&cmd.args, "--show-forced-updates");
	അല്ലെങ്കിൽ എങ്കിൽ (opt_show_forced_updates == 0)
		strvec_തള്ളുക(&cmd.args, "--no-show-forced-updates");
	എങ്കിൽ (set_upstream)
		strvec_തള്ളുക(&cmd.args, set_upstream);
	strvec_pushv(&cmd.args, opt_fetch.v);

	എങ്കിൽ (repo) {
		strvec_തള്ളുക(&cmd.args, repo);
		strvec_pushv(&cmd.args, refspecs);
	} അല്ലെങ്കിൽ എങ്കിൽ (*refspecs)
		BUG("refspecs without repo?");
	cmd.git_cmd = 1;
	cmd.odb_to_close = the_repository->objects;
	തിരികെ run_command(&cmd);
}

/**
 * "Pulls into void" by branching off merge_head.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ pull_into_void(സ്ഥിരം ഘടന object_id *merge_head,
		സ്ഥിരം ഘടന object_id *curr_head)
{
	എങ്കിൽ (opt_verify_signatures) {
		ഘടന commit *commit;

		commit = lookup_commit(the_repository, merge_head);
		എങ്കിൽ (!commit)
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
	എങ്കിൽ (checkout_fast_forward(the_repository,
				  the_hash_algo->empty_വൃക്ഷം,
				  merge_head, 0))
		തിരികെ 1;

	എങ്കിൽ (refs_update_ref(get_main_ref_store(the_repository), "initial pull", "HEAD", merge_head, curr_head, 0, UPDATE_REFS_DIE_ON_ERR))
		തിരികെ 1;

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ rebase_submodules(ശൂന്യം)
{
	ഘടന child_process cp = CHILD_PROCESS_INIT;

	cp.git_cmd = 1;
	cp.no_stdin = 1;
	strvec_pushl(&cp.args, "submodule", "update",
		     "--recursive", "--rebase", ശൂന്യ);
	argv_തള്ളുക_verbosity(&cp.args);

	തിരികെ run_command(&cp);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ update_submodules(ശൂന്യം)
{
	ഘടന child_process cp = CHILD_PROCESS_INIT;

	cp.git_cmd = 1;
	cp.no_stdin = 1;
	strvec_pushl(&cp.args, "submodule", "update",
		     "--recursive", "--checkout", ശൂന്യ);
	argv_തള്ളുക_verbosity(&cp.args);

	തിരികെ run_command(&cp);
}

/**
 * Runs git-merge, returning its exit status.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ run_merge(ശൂന്യം)
{
	ഘടന child_process cmd = CHILD_PROCESS_INIT;

	strvec_pushl(&cmd.args, "merge", ശൂന്യ);

	/* Shared options */
	argv_തള്ളുക_verbosity(&cmd.args);
	എങ്കിൽ (opt_progress)
		strvec_തള്ളുക(&cmd.args, opt_progress);

	/* Options passed to git-merge */
	എങ്കിൽ (opt_diffstat)
		strvec_തള്ളുക(&cmd.args, opt_diffstat);
	എങ്കിൽ (opt_log)
		strvec_തള്ളുക(&cmd.args, opt_log);
	എങ്കിൽ (opt_signoff)
		strvec_തള്ളുക(&cmd.args, opt_signoff);
	എങ്കിൽ (opt_squash)
		strvec_തള്ളുക(&cmd.args, opt_squash);
	എങ്കിൽ (opt_commit)
		strvec_തള്ളുക(&cmd.args, opt_commit);
	എങ്കിൽ (opt_edit)
		strvec_തള്ളുക(&cmd.args, opt_edit);
	എങ്കിൽ (cleanup_arg)
		strvec_pushf(&cmd.args, "--cleanup=%s", cleanup_arg);
	എങ്കിൽ (opt_ff)
		strvec_തള്ളുക(&cmd.args, opt_ff);
	എങ്കിൽ (opt_verify)
		strvec_തള്ളുക(&cmd.args, opt_verify);
	എങ്കിൽ (opt_verify_signatures)
		strvec_തള്ളുക(&cmd.args, opt_verify_signatures);
	strvec_pushv(&cmd.args, opt_strategies.v);
	strvec_pushv(&cmd.args, opt_strategy_opts.v);
	എങ്കിൽ (opt_gpg_sign)
		strvec_തള്ളുക(&cmd.args, opt_gpg_sign);
	എങ്കിൽ (opt_autostash == 0)
		strvec_തള്ളുക(&cmd.args, "--no-autostash");
	അല്ലെങ്കിൽ എങ്കിൽ (opt_autostash == 1)
		strvec_തള്ളുക(&cmd.args, "--autostash");
	എങ്കിൽ (opt_allow_unrelated_histories > 0)
		strvec_തള്ളുക(&cmd.args, "--allow-unrelated-histories");

	strvec_തള്ളുക(&cmd.args, "FETCH_HEAD");
	cmd.git_cmd = 1;
	തിരികെ run_command(&cmd);
}

/**
 * Returns remote's upstream branch for the current branch. If remote is NULL,
 * the current branch's configured default remote is used. Returns NULL if
 * `remote` does not name a valid remote, HEAD does not point to a branch,
 * remote is not the branch's configured remote or the branch does not have any
 * configured upstream branch.
 */
സ്ഥിതം സ്ഥിരം അക്ഷരം *get_upstream_branch(സ്ഥിരം അക്ഷരം *remote)
{
	ഘടന remote *rm;
	ഘടന branch *curr_branch;
	സ്ഥിരം അക്ഷരം *curr_branch_remote;

	rm = remote_get(remote);
	എങ്കിൽ (!rm)
		തിരികെ ശൂന്യ;

	curr_branch = branch_get("HEAD");
	എങ്കിൽ (!curr_branch)
		തിരികെ ശൂന്യ;

	curr_branch_remote = remote_for_branch(curr_branch, ശൂന്യ);
	പ്രതിജ്ഞാ(curr_branch_remote);

	എങ്കിൽ (strcmp(curr_branch_remote, rm->name))
		തിരികെ ശൂന്യ;

	തിരികെ branch_get_upstream(curr_branch, ശൂന്യ);
}

/**
 * Derives the remote-tracking branch from the remote and refspec.
 *
 * FIXME: The current implementation assumes the default mapping of
 * refs/heads/<branch_name> to refs/remotes/<remote_name>/<branch_name>.
 */
സ്ഥിതം സ്ഥിരം അക്ഷരം *get_tracking_branch(സ്ഥിരം അക്ഷരം *remote, സ്ഥിരം അക്ഷരം *refspec)
{
	ഘടന refspec_item spec;
	സ്ഥിരം അക്ഷരം *spec_src;
	സ്ഥിരം അക്ഷരം *merge_branch;

	എങ്കിൽ (!refspec_item_init_fetch(&spec, refspec, the_hash_algo))
		die(_("invalid refspec '%s'"), refspec);
	spec_src = spec.src;
	എങ്കിൽ (!*spec_src || !strcmp(spec_src, "HEAD"))
		spec_src = "HEAD";
	അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(spec_src, "heads/", &spec_src))
		;
	അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(spec_src, "refs/heads/", &spec_src))
		;
	അല്ലെങ്കിൽ എങ്കിൽ (starts_with(spec_src, "refs/") ||
		starts_with(spec_src, "tags/") ||
		starts_with(spec_src, "remotes/"))
		spec_src = "";

	എങ്കിൽ (*spec_src) {
		എങ്കിൽ (!strcmp(remote, "."))
			merge_branch = mkpath("refs/heads/%s", spec_src);
		അല്ലെങ്കിൽ
			merge_branch = mkpath("refs/remotes/%s/%s", remote, spec_src);
	} അല്ലെങ്കിൽ
		merge_branch = ശൂന്യ;

	refspec_item_clear(&spec);
	തിരികെ merge_branch;
}

/**
 * Given the repo and refspecs, sets fork_point to the point at which the
 * current branch forked from its remote-tracking branch. Returns 0 on success,
 * -1 on failure.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ get_rebase_fork_point(ഘടന object_id *fork_point, സ്ഥിരം അക്ഷരം *repo,
		സ്ഥിരം അക്ഷരം *refspec)
{
	പൂർണ്ണസംഖ്യ ret;
	ഘടന branch *curr_branch;
	സ്ഥിരം അക്ഷരം *remote_branch;
	ഘടന child_process cp = CHILD_PROCESS_INIT;
	ഘടന strbuf sb = STRBUF_INIT;

	curr_branch = branch_get("HEAD");
	എങ്കിൽ (!curr_branch)
		തിരികെ -1;

	എങ്കിൽ (refspec)
		remote_branch = get_tracking_branch(repo, refspec);
	അല്ലെങ്കിൽ
		remote_branch = get_upstream_branch(repo);

	എങ്കിൽ (!remote_branch)
		തിരികെ -1;

	strvec_pushl(&cp.args, "merge-base", "--fork-point",
		     remote_branch, curr_branch->name, ശൂന്യ);
	cp.no_stdin = 1;
	cp.no_stderr = 1;
	cp.git_cmd = 1;

	ret = capture_command(&cp, &sb, GIT_MAX_HEXSZ);
	എങ്കിൽ (ret)
		പോകുക cleanup;

	ret = get_oid_hex(sb.buf, fork_point);
	എങ്കിൽ (ret)
		പോകുക cleanup;

cleanup:
	strbuf_release(&sb);
	തിരികെ ret ? -1 : 0;
}

/**
 * Sets merge_base to the octopus merge base of curr_head, merge_head and
 * fork_point. Returns 0 if a merge base is found, 1 otherwise.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ get_octopus_merge_base(ഘടന object_id *merge_base,
		സ്ഥിരം ഘടന object_id *curr_head,
		സ്ഥിരം ഘടന object_id *merge_head,
		സ്ഥിരം ഘടന object_id *fork_point)
{
	ഘടന commit_list *revs = ശൂന്യ, *result = ശൂന്യ;

	commit_list_തിരുകുക(lookup_commit_reference(the_repository, curr_head),
			   &revs);
	commit_list_തിരുകുക(lookup_commit_reference(the_repository, merge_head),
			   &revs);
	എങ്കിൽ (!is_null_oid(fork_point))
		commit_list_തിരുകുക(lookup_commit_reference(the_repository, fork_point),
				   &revs);

	എങ്കിൽ (get_octopus_merge_bases(revs, &result) < 0)
		exit(128);
	commit_list_free(revs);
	reduce_heads_replace(&result);

	എങ്കിൽ (!result)
		തിരികെ 1;

	oidcpy(merge_base, &result->item->object.oid);
	commit_list_free(result);
	തിരികെ 0;
}

/**
 * Given the current HEAD oid, the merge head returned from git-fetch and the
 * fork point calculated by get_rebase_fork_point(), compute the <newbase> and
 * <upstream> arguments to use for the upcoming git-rebase invocation.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ get_rebase_newbase_and_upstream(ഘടന object_id *newbase,
		ഘടന object_id *upstream,
		സ്ഥിരം ഘടന object_id *curr_head,
		സ്ഥിരം ഘടന object_id *merge_head,
		സ്ഥിരം ഘടന object_id *fork_point)
{
	ഘടന object_id oct_merge_base;

	എങ്കിൽ (!get_octopus_merge_base(&oct_merge_base, curr_head, merge_head, fork_point))
		എങ്കിൽ (!is_null_oid(fork_point) && oideq(&oct_merge_base, fork_point))
			fork_point = ശൂന്യ;

	എങ്കിൽ (fork_point && !is_null_oid(fork_point))
		oidcpy(upstream, fork_point);
	അല്ലെങ്കിൽ
		oidcpy(upstream, merge_head);

	oidcpy(newbase, merge_head);

	തിരികെ 0;
}

/**
 * Given the <newbase> and <upstream> calculated by
 * get_rebase_newbase_and_upstream(), runs git-rebase with the
 * appropriate arguments and returns its exit status.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ run_rebase(സ്ഥിരം ഘടന object_id *newbase,
		സ്ഥിരം ഘടന object_id *upstream)
{
	ഘടന child_process cmd = CHILD_PROCESS_INIT;

	strvec_തള്ളുക(&cmd.args, "rebase");

	/* Shared options */
	argv_തള്ളുക_verbosity(&cmd.args);

	/* Options passed to git-rebase */
	എങ്കിൽ (opt_rebase == REBASE_MERGES)
		strvec_തള്ളുക(&cmd.args, "--rebase-merges");
	അല്ലെങ്കിൽ എങ്കിൽ (opt_rebase == REBASE_INTERACTIVE)
		strvec_തള്ളുക(&cmd.args, "--interactive");
	എങ്കിൽ (opt_diffstat)
		strvec_തള്ളുക(&cmd.args, opt_diffstat);
	strvec_pushv(&cmd.args, opt_strategies.v);
	strvec_pushv(&cmd.args, opt_strategy_opts.v);
	എങ്കിൽ (opt_gpg_sign)
		strvec_തള്ളുക(&cmd.args, opt_gpg_sign);
	എങ്കിൽ (opt_signoff)
		strvec_തള്ളുക(&cmd.args, opt_signoff);
	എങ്കിൽ (opt_autostash == 0)
		strvec_തള്ളുക(&cmd.args, "--no-autostash");
	അല്ലെങ്കിൽ എങ്കിൽ (opt_autostash == 1)
		strvec_തള്ളുക(&cmd.args, "--autostash");
	എങ്കിൽ (opt_verify_signatures &&
	    !strcmp(opt_verify_signatures, "--verify-signatures"))
		warning(_("ignoring --verify-signatures for rebase"));

	strvec_തള്ളുക(&cmd.args, "--onto");
	strvec_തള്ളുക(&cmd.args, oid_to_hex(newbase));

	strvec_തള്ളുക(&cmd.args, oid_to_hex(upstream));

	cmd.git_cmd = 1;
	തിരികെ run_command(&cmd);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ get_can_ff(ഘടന object_id *orig_head,
		      ഘടന oid_array *merge_heads)
{
	പൂർണ്ണസംഖ്യ ret;
	ഘടന commit_list *list = ശൂന്യ;
	ഘടന commit *merge_head, *head;
	ഘടന object_id *orig_merge_head;

	എങ്കിൽ (merge_heads->nr > 1)
		തിരികെ 0;

	orig_merge_head = &merge_heads->oid[0];
	head = lookup_commit_reference(the_repository, orig_head);
	എങ്കിൽ (!head)
		തിരികെ 0;
	merge_head = lookup_commit_reference(the_repository, orig_merge_head);
	എങ്കിൽ (!merge_head)
		തിരികെ 0;
	commit_list_തിരുകുക(head, &list);
	ret = repo_is_descendant_of(the_repository, merge_head, list);
	commit_list_free(list);
	എങ്കിൽ (ret < 0)
		exit(128);
	തിരികെ ret;
}

/*
 * Is orig_head a descendant of _all_ merge_heads?
 * Unfortunately is_descendant_of() cannot be used as it asks
 * if orig_head is a descendant of at least one of them.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ already_up_to_date(ഘടന object_id *orig_head,
			      ഘടന oid_array *merge_heads)
{
	ഘടന commit *ours;

	ours = lookup_commit_reference(the_repository, orig_head);
	എങ്കിൽ (!ours)
		തിരികെ 0;
	വേണ്ടി (size_t i = 0; i < merge_heads->nr; i++) {
		ഘടന commit_list *list = ശൂന്യ;
		ഘടന commit *theirs;
		പൂർണ്ണസംഖ്യ ok;

		theirs = lookup_commit_reference(the_repository, &merge_heads->oid[i]);
		എങ്കിൽ (!theirs)
			തിരികെ 0;
		commit_list_തിരുകുക(theirs, &list);
		ok = repo_is_descendant_of(the_repository, ours, list);
		commit_list_free(list);
		എങ്കിൽ (ok < 0)
			exit(128);
		എങ്കിൽ (!ok)
			തിരികെ 0;
	}
	തിരികെ 1;
}

സ്ഥിതം ശൂന്യം show_advice_pull_non_ff(ശൂന്യം)
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

പൂർണ്ണസംഖ്യ cmd_pull(പൂർണ്ണസംഖ്യ argc,
	     സ്ഥിരം അക്ഷരം **argv,
	     സ്ഥിരം അക്ഷരം *prefix,
	     ഘടന repository *repository UNUSED)
{
	സ്ഥിരം അക്ഷരം *repo, **refspecs;
	ഘടന oid_array merge_heads = OID_ARRAY_INIT;
	ഘടന object_id orig_head, curr_head;
	ഘടന object_id rebase_fork_point;
	പൂർണ്ണസംഖ്യ rebase_unspecified = 0;
	പൂർണ്ണസംഖ്യ can_ff;
	പൂർണ്ണസംഖ്യ divergent;
	പൂർണ്ണസംഖ്യ ret;
	സ്ഥിതം ഘടന option pull_options[] = {
		/* Shared options */
		OPT__VERBOSITY(&opt_verbosity),
		OPT_PASSTHRU(0, "progress", &opt_progress, ശൂന്യ,
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
		OPT_PASSTHRU('n', ശൂന്യ, &opt_diffstat, ശൂന്യ,
			N_("do not show a diffstat at the end of the merge"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG),
		OPT_PASSTHRU(0, "stat", &opt_diffstat, ശൂന്യ,
			N_("show a diffstat at the end of the merge"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "summary", &opt_diffstat, ശൂന്യ,
			N_("(synonym to --stat)"),
			PARSE_OPT_NOARG | PARSE_OPT_HIDDEN),
		OPT_PASSTHRU(0, "compact-summary", &opt_diffstat, ശൂന്യ,
			N_("show a compact-summary at the end of the merge"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "log", &opt_log, N_("n"),
			N_("add (at most <n>) entries from shortlog to merge commit message"),
			PARSE_OPT_OPTARG),
		OPT_PASSTHRU(0, "signoff", &opt_signoff, ശൂന്യ,
			N_("add a Signed-off-by trailer"),
			PARSE_OPT_OPTARG),
		OPT_PASSTHRU(0, "squash", &opt_squash, ശൂന്യ,
			N_("create a single commit instead of doing a merge"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "commit", &opt_commit, ശൂന്യ,
			N_("perform a commit if the merge succeeds (default)"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "edit", &opt_edit, ശൂന്യ,
			N_("edit message before committing"),
			PARSE_OPT_NOARG),
		OPT_CLEANUP(&cleanup_arg),
		OPT_PASSTHRU(0, "ff", &opt_ff, ശൂന്യ,
			N_("allow fast-forward"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "ff-only", &opt_ff, ശൂന്യ,
			N_("abort if fast-forward is not possible"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG),
		OPT_PASSTHRU(0, "verify", &opt_verify, ശൂന്യ,
			N_("control use of pre-merge-commit and commit-msg hooks"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "verify-signatures", &opt_verify_signatures, ശൂന്യ,
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
		OPT_PASSTHRU(0, "all", &opt_all, ശൂന്യ,
			N_("fetch from all remotes"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU('a', "append", &opt_append, ശൂന്യ,
			N_("append to .git/FETCH_HEAD instead of overwriting"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "upload-pack", &opt_upload_pack, N_("path"),
			N_("path to upload pack on remote end"),
			0),
		OPT__FORCE(&opt_force, N_("force overwrite of local branch"), 0),
		OPT_PASSTHRU('t', "tags", &opt_tags, ശൂന്യ,
			N_("fetch all tags and associated objects"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU('p', "prune", &opt_prune, ശൂന്യ,
			N_("prune remote-tracking branches no longer on remote"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU('j', "jobs", &max_children, N_("n"),
			N_("number of submodules pulled in parallel"),
			PARSE_OPT_OPTARG),
		OPT_BOOL(0, "dry-run", &opt_dry_run,
			N_("dry run")),
		OPT_PASSTHRU('k', "keep", &opt_keep, ശൂന്യ,
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
		OPT_PASSTHRU(0, "unshallow", &opt_unshallow, ശൂന്യ,
			N_("convert to a complete repository"),
			PARSE_OPT_NONEG | PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "update-shallow", &opt_update_shallow, ശൂന്യ,
			N_("accept refs that update .git/shallow"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "refmap", &opt_refmap, N_("refmap"),
			N_("specify fetch refmap"),
			PARSE_OPT_NONEG),
		OPT_PASSTHRU_ARGV('o', "server-option", &opt_fetch,
			N_("server-specific"),
			N_("option to transmit"),
			0),
		OPT_PASSTHRU('4',  "ipv4", &opt_ipv4, ശൂന്യ,
			N_("use IPv4 addresses only"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU('6',  "ipv6", &opt_ipv6, ശൂന്യ,
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
		OPT_PASSTHRU(0, "set-upstream", &set_upstream, ശൂന്യ,
			N_("set upstream for git pull/fetch"),
			PARSE_OPT_NOARG),

		OPT_END()
	};

	എങ്കിൽ (!getenv("GIT_REFLOG_ACTION"))
		set_reflog_message(argc, argv);

	repo_config(the_repository, git_pull_config, ശൂന്യ);
	എങ്കിൽ (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_full_index = 0;
	}

	argc = parse_options(argc, argv, prefix, pull_options, pull_usage, 0);
	എങ്കിൽ (opt_autostash == -1)
		opt_autostash = config_pull_autostash;

	എങ്കിൽ (recurse_submodules_cli != RECURSE_SUBMODULES_DEFAULT)
		recurse_submodules = recurse_submodules_cli;

	എങ്കിൽ (cleanup_arg)
		/*
		 * this only checks the validity of cleanup_arg; we don't need
		 * a valid value for use_editor
		 */
		get_cleanup_mode(cleanup_arg, 0);

	parse_repo_refspecs(argc, argv, &repo, &refspecs);

	എങ്കിൽ (!opt_ff) {
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
		എങ്കിൽ (opt_rebase >= 0 && opt_ff && !strcmp(opt_ff, "--ff-only")) {
			സ്വതന്ത്രം(opt_ff);
			opt_ff = xstrdup("--ff");
		}
	}

	എങ്കിൽ (opt_rebase < 0)
		opt_rebase = config_get_rebase(&rebase_unspecified);

	എങ്കിൽ (repo_read_index_unmerged(the_repository))
		die_resolve_conflict("pull");

	എങ്കിൽ (file_exists(git_path_merge_head(the_repository)))
		die_conclude_merge();

	എങ്കിൽ (repo_get_oid(the_repository, "HEAD", &orig_head))
		oidclr(&orig_head, the_repository->hash_algo);

	എങ്കിൽ (opt_rebase) {
		എങ്കിൽ (opt_autostash == -1)
			opt_autostash = config_rebase_autostash;

		എങ്കിൽ (is_null_oid(&orig_head) && !is_index_unborn(the_repository->index))
			die(_("Updating an unborn branch with changes added to the index."));

		എങ്കിൽ (!opt_autostash)
			require_clean_work_വൃക്ഷം(the_repository,
				N_("pull with rebase"),
				_("Please commit or stash them."), 1, 0);

		എങ്കിൽ (get_rebase_fork_point(&rebase_fork_point, repo, *refspecs))
			oidclr(&rebase_fork_point, the_repository->hash_algo);
	}

	എങ്കിൽ (run_fetch(repo, refspecs))
		തിരികെ 1;

	എങ്കിൽ (opt_dry_run)
		തിരികെ 0;

	എങ്കിൽ (repo_get_oid(the_repository, "HEAD", &curr_head))
		oidclr(&curr_head, the_repository->hash_algo);

	എങ്കിൽ (!is_null_oid(&orig_head) && !is_null_oid(&curr_head) &&
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

		എങ്കിൽ (checkout_fast_forward(the_repository, &orig_head,
					  &curr_head, 0))
			die(_("Cannot fast-forward your working tree.\n"
				"After making sure that you saved anything precious from\n"
				"$ git diff %s\n"
				"output, run\n"
				"$ git reset --hard\n"
				"to recover."), oid_to_hex(&orig_head));
	}

	get_merge_heads(&merge_heads);

	എങ്കിൽ (!merge_heads.nr)
		die_no_merge_candidates(repo, refspecs);

	എങ്കിൽ (is_null_oid(&orig_head)) {
		എങ്കിൽ (merge_heads.nr > 1)
			die(_("Cannot merge multiple branches into empty head."));
		ret = pull_into_void(merge_heads.oid, &curr_head);
		പോകുക cleanup;
	}
	എങ്കിൽ (merge_heads.nr > 1) {
		എങ്കിൽ (opt_rebase)
			die(_("Cannot rebase onto multiple branches."));
		എങ്കിൽ (opt_ff && !strcmp(opt_ff, "--ff-only"))
			die(_("Cannot fast-forward to multiple branches."));
	}

	can_ff = get_can_ff(&orig_head, &merge_heads);
	divergent = !can_ff && !already_up_to_date(&orig_head, &merge_heads);

	/* ff-only takes precedence over rebase */
	എങ്കിൽ (opt_ff && !strcmp(opt_ff, "--ff-only")) {
		എങ്കിൽ (divergent)
			die_ff_impossible();
		opt_rebase = REBASE_FALSE;
	}
	/* If no action specified and we can't fast forward, then warn. */
	എങ്കിൽ (!opt_ff && rebase_unspecified && divergent) {
		show_advice_pull_non_ff();
		die(_("Need to specify how to reconcile divergent branches."));
	}

	എങ്കിൽ (opt_rebase) {
		ഘടന object_id newbase;
		ഘടന object_id upstream;
		get_rebase_newbase_and_upstream(&newbase, &upstream, &curr_head,
						merge_heads.oid, &rebase_fork_point);

		എങ്കിൽ ((recurse_submodules == RECURSE_SUBMODULES_ON ||
		     recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND) &&
		    submodule_touches_in_range(the_repository, &upstream, &curr_head))
			die(_("cannot rebase with locally recorded submodule modifications"));

		എങ്കിൽ (can_ff) {
			/* we can fast-forward this without invoking rebase */
			സ്വതന്ത്രം(opt_ff);
			opt_ff = xstrdup("--ff-only");
			ret = run_merge();
		} അല്ലെങ്കിൽ {
			ret = run_rebase(&newbase, &upstream);
		}

		എങ്കിൽ (!ret && (recurse_submodules == RECURSE_SUBMODULES_ON ||
			     recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND))
			ret = rebase_submodules();

		പോകുക cleanup;
	} അല്ലെങ്കിൽ {
		ret = run_merge();
		എങ്കിൽ (!ret && (recurse_submodules == RECURSE_SUBMODULES_ON ||
			     recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND))
			ret = update_submodules();
		പോകുക cleanup;
	}

cleanup:
	oid_array_clear(&merge_heads);
	തിരികെ ret;
}
