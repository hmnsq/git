/*
 * Builtin "git pull"
 *
 * Based on git-pull.sh by Junio C Hamano
 *
 * Fetch one or more remote refs and merge it/them into the current HEAD.
 */

#define USE_THE_REPOSITORY_VARIABLE

#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "advice.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "hex.h"
#ଅନ୍ତର୍ଭାଵ "merge.h"
#ଅନ୍ତର୍ଭାଵ "object-name.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "run-command.h"
#ଅନ୍ତର୍ଭାଵ "oid-array.h"
#ଅନ୍ତର୍ଭାଵ "remote.h"
#ଅନ୍ତର୍ଭାଵ "dir.h"
#ଅନ୍ତର୍ଭାଵ "path.h"
#ଅନ୍ତର୍ଭାଵ "read-cache-ll.h"
#ଅନ୍ତର୍ଭାଵ "rebase.h"
#ଅନ୍ତର୍ଭାଵ "refs.h"
#ଅନ୍ତର୍ଭାଵ "refspec.h"
#ଅନ୍ତର୍ଭାଵ "submodule.h"
#ଅନ୍ତର୍ଭାଵ "submodule-config.h"
#ଅନ୍ତର୍ଭାଵ "wt-status.h"
#ଅନ୍ତର୍ଭାଵ "commit-reach.h"
#ଅନ୍ତର୍ଭାଵ "sequencer.h"

/**
 * Parses the value of --rebase. If value is a false value, returns
 * REBASE_FALSE. If value is a true value, returns REBASE_TRUE. If value is
 * "merges", returns REBASE_MERGES. If value is a invalid value, dies with
 * a fatal error if fatal is true, otherwise returns REBASE_INVALID.
 */
ସ୍ଥିର ଗଣନା rebase_type parse_config_rebase(ଅଚଳ ଅକ୍ଷର *ଚାବି, ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ,
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା fatal)
{
	ଗଣନା rebase_type v = rebase_parse_ମୂଲ୍ୟ(ମୂଲ୍ୟ);
	ଯଦି (v != REBASE_INVALID)
		ଫେରନ୍ତୁ v;

	ଯଦି (fatal)
		die(_("invalid value for '%s': '%s'"), ଚାବି, ମୂଲ୍ୟ);
	ଅନ୍ୟଥା
		error(_("invalid value for '%s': '%s'"), ଚାବି, ମୂଲ୍ୟ);

	ଫେରନ୍ତୁ REBASE_INVALID;
}

/**
 * Callback for --rebase, which parses arg with parse_config_rebase().
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା parse_opt_rebase(ଅଚଳ ସଂରଚନା option *opt, ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ଗଣନା rebase_type *ମୂଲ୍ୟ = opt->ମୂଲ୍ୟ;

	ଯଦି (arg)
		*ମୂଲ୍ୟ = parse_config_rebase("--rebase", arg, 0);
	ଅନ୍ୟଥା
		*ମୂଲ୍ୟ = unset ? REBASE_FALSE : REBASE_TRUE;
	ଫେରନ୍ତୁ *ମୂଲ୍ୟ == REBASE_INVALID ? -1 : 0;
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ pull_usage[] = {
	N_("git pull [<options>] [<repository> [<refspec>...]]"),
	ଖାଲି
};

/* Shared options */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା opt_verbosity;
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *opt_progress;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା recurse_submodules = RECURSE_SUBMODULES_DEFAULT;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା recurse_submodules_cli = RECURSE_SUBMODULES_DEFAULT;

/* Options passed to git-merge or git-rebase */
ସ୍ଥିର ଗଣନା rebase_type opt_rebase = -1;
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *opt_diffstat;
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *opt_log;
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *opt_signoff;
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *opt_squash;
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *opt_commit;
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *opt_edit;
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *cleanup_arg;
ସ୍ଥିର ଅକ୍ଷର *opt_ff;
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *opt_verify_signatures;
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *opt_verify;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା opt_autostash = -1;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା config_rebase_autostash;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା config_pull_autostash = -1;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା check_trust_level = 1;
ସ୍ଥିର ସଂରଚନା strvec opt_strategies = STRVEC_INIT;
ସ୍ଥିର ସଂରଚନା strvec opt_strategy_opts = STRVEC_INIT;
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *opt_gpg_sign;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା opt_allow_unrelated_histories;

/* Options passed to git-fetch */
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *opt_all;
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *opt_append;
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *opt_upload_pack;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା opt_force;
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *opt_tags;
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *opt_prune;
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *max_children;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା opt_dry_run;
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *opt_keep;
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *opt_depth;
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *opt_unshallow;
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *opt_update_shallow;
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *opt_refmap;
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *opt_ipv4;
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *opt_ipv6;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା opt_show_forced_updates = -1;
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *set_upstream;
ସ୍ଥିର ସଂରଚନା strvec opt_fetch = STRVEC_INIT;

/**
 * Pushes "-q" or "-v" switches into arr to match the opt_verbosity level.
 */
ସ୍ଥିର ଶୂନ୍ୟ argv_ଠେଲ_verbosity(ସଂରଚନା strvec *arr)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା verbosity;

	ପାଇଁ (verbosity = opt_verbosity; verbosity > 0; verbosity--)
		strvec_ଠେଲ(arr, "-v");

	ପାଇଁ (verbosity = opt_verbosity; verbosity < 0; verbosity++)
		strvec_ଠେଲ(arr, "-q");
}

/**
 * Pushes "-f" switches into arr to match the opt_force level.
 */
ସ୍ଥିର ଶୂନ୍ୟ argv_ଠେଲ_force(ସଂରଚନା strvec *arr)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା force = opt_force;
	ଯେତେବେଳେ (force-- > 0)
		strvec_ଠେଲ(arr, "-f");
}

/**
 * Sets the GIT_REFLOG_ACTION environment variable to the concatenation of argv
 */
ସ୍ଥିର ଶୂନ୍ୟ set_reflog_message(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	ସଂରଚନା strbuf ସନ୍ଦେଶ = STRBUF_INIT;

	ପାଇଁ (i = 0; i < argc; i++) {
		ଯଦି (i)
			strbuf_addch(&ସନ୍ଦେଶ, ' ');
		strbuf_addstr(&ସନ୍ଦେଶ, argv[i]);
	}

	setenv("GIT_REFLOG_ACTION", ସନ୍ଦେଶ.buf, 0);

	strbuf_release(&ସନ୍ଦେଶ);
}

/**
 * If pull.ff is unset, returns NULL. If pull.ff is "true", returns "--ff". If
 * pull.ff is "false", returns "--no-ff". If pull.ff is "only", returns
 * "--ff-only". Otherwise, if pull.ff is set to an invalid value, die with an
 * error.
 */
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *config_get_ff(ଶୂନ୍ୟ)
{
	ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ;

	ଯଦି (repo_config_get_ମୂଲ୍ୟ(the_repository, "pull.ff", &ମୂଲ୍ୟ))
		ଫେରନ୍ତୁ ଖାଲି;

	ବାଛ (git_parse_maybe_bool(ମୂଲ୍ୟ)) {
	ସ୍ଥିତି 0:
		ଫେରନ୍ତୁ "--no-ff";
	ସ୍ଥିତି 1:
		ଫେରନ୍ତୁ "--ff";
	}

	ଯଦି (!strcmp(ମୂଲ୍ୟ, "only"))
		ଫେରନ୍ତୁ "--ff-only";

	die(_("invalid value for '%s': '%s'"), "pull.ff", ମୂଲ୍ୟ);
}

/**
 * Returns the default configured value for --rebase. It first looks for the
 * value of "branch.$curr_branch.rebase", where $curr_branch is the current
 * branch, and if HEAD is detached or the configuration key does not exist,
 * looks for the value of "pull.rebase". If both configuration keys do not
 * exist, returns REBASE_FALSE.
 */
ସ୍ଥିର ଗଣନା rebase_type config_get_rebase(ପୂର୍ଣ୍ଣସଂଖ୍ୟା *rebase_unspecified)
{
	ସଂରଚନା branch *curr_branch = branch_get("HEAD");
	ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ;

	ଯଦି (curr_branch) {
		ଅକ୍ଷର *ଚାବି = xstrfmt("branch.%s.rebase", curr_branch->name);

		ଯଦି (!repo_config_get_ମୂଲ୍ୟ(the_repository, ଚାବି, &ମୂଲ୍ୟ)) {
			ଗଣନା rebase_type ret = parse_config_rebase(ଚାବି, ମୂଲ୍ୟ, 1);
			ମୁକ୍ତ(ଚାବି);
			ଫେରନ୍ତୁ ret;
		}

		ମୁକ୍ତ(ଚାବି);
	}

	ଯଦି (!repo_config_get_ମୂଲ୍ୟ(the_repository, "pull.rebase", &ମୂଲ୍ୟ))
		ଫେରନ୍ତୁ parse_config_rebase("pull.rebase", ମୂଲ୍ୟ, 1);

	*rebase_unspecified = 1;

	ଫେରନ୍ତୁ REBASE_FALSE;
}

/**
 * Read config variables.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା git_pull_config(ଅଚଳ ଅକ୍ଷର *var, ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ,
			   ଅଚଳ ସଂରଚନା config_context *ctx, ଶୂନ୍ୟ *cb)
{
	ଯଦି (!strcmp(var, "rebase.autostash")) {
		/*
		 * run_rebase() also reads this option. The reason we handle it here is
		 * that when pull.rebase is true, a fast-forward may occur without
		 * invoking run_rebase(). We need to ensure that autostash is set even
		 * in the fast-forward case.
		 *
		 * run_merge() handles merge.autostash, so we don't handle it here.
		 */
		config_rebase_autostash = git_config_bool(var, ମୂଲ୍ୟ);
		ଫେରନ୍ତୁ 0;
	} ଅନ୍ୟଥା ଯଦି (!strcmp(var, "pull.autostash")) {
		config_pull_autostash = git_config_bool(var, ମୂଲ୍ୟ);
		ଫେରନ୍ତୁ 0;
	} ଅନ୍ୟଥା ଯଦି (!strcmp(var, "submodule.recurse")) {
		recurse_submodules = git_config_bool(var, ମୂଲ୍ୟ) ?
			RECURSE_SUBMODULES_ON : RECURSE_SUBMODULES_OFF;
		ଫେରନ୍ତୁ 0;
	} ଅନ୍ୟଥା ଯଦି (!strcmp(var, "gpg.mintrustlevel")) {
		check_trust_level = 0;
	}

	ଫେରନ୍ତୁ git_default_config(var, ମୂଲ୍ୟ, ctx, cb);
}

/**
 * Appends merge candidates from FETCH_HEAD that are not marked not-for-merge
 * into merge_heads.
 */
ସ୍ଥିର ଶୂନ୍ୟ get_merge_heads(ସଂରଚନା oid_array *merge_heads)
{
	ଅଚଳ ଅକ୍ଷର *filename = git_path_fetch_head(the_repository);
	FILE *fp;
	ସଂରଚନା strbuf sb = STRBUF_INIT;
	ସଂରଚନା object_id oid;

	fp = xfopen(filename, "r");
	ଯେତେବେଳେ (strbuf_getline_lf(&sb, fp) != EOF) {
		ଅଚଳ ଅକ୍ଷର *p;
		ଯଦି (parse_oid_hex(sb.buf, &oid, &p))
			ଜାରି;  /* invalid line: does not start with object ID */
		ଯଦି (starts_with(p, "\tnot-for-merge\t"))
			ଜାରି;  /* ref is not-for-merge */
		oid_array_append(merge_heads, &oid);
	}
	fclose(fp);
	strbuf_release(&sb);
}

/**
 * Used by die_no_merge_candidates() as a for_each_remote() callback to
 * retrieve the name of the remote if the repository only has one remote.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_only_remote(ସଂରଚନା remote *remote, ଶୂନ୍ୟ *cb_data)
{
	ଅଚଳ ଅକ୍ଷର **remote_name = cb_data;

	ଯଦି (*remote_name)
		ଫେରନ୍ତୁ -1;

	*remote_name = remote->name;
	ଫେରନ୍ତୁ 0;
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
ସ୍ଥିର ଶୂନ୍ୟ NORETURN die_no_merge_candidates(ଅଚଳ ଅକ୍ଷର *repo, ଅଚଳ ଅକ୍ଷର **refspecs)
{
	ସଂରଚନା branch *curr_branch = branch_get("HEAD");
	ଅଚଳ ଅକ୍ଷର *remote = curr_branch ? curr_branch->remote_name : ଖାଲି;

	ଯଦି (*refspecs) {
		ଯଦି (opt_rebase)
			fprintf_ln(stderr, _("There is no candidate for rebasing against among the refs that you just fetched."));
		ଅନ୍ୟଥା
			fprintf_ln(stderr, _("There are no candidates for merging among the refs that you just fetched."));
		fprintf_ln(stderr, _("Generally this means that you provided a wildcard refspec which had no\n"
					"matches on the remote end."));
	} ଅନ୍ୟଥା ଯଦି (repo && curr_branch && (!remote || strcmp(repo, remote))) {
		fprintf_ln(stderr, _("You asked to pull from the remote '%s', but did not specify\n"
			"a branch. Because this is not the default configured remote\n"
			"for your current branch, you must specify a branch on the command line."),
			repo);
	} ଅନ୍ୟଥା ଯଦି (!curr_branch) {
		fprintf_ln(stderr, _("You are not currently on a branch."));
		ଯଦି (opt_rebase)
			fprintf_ln(stderr, _("Please specify which branch you want to rebase against."));
		ଅନ୍ୟଥା
			fprintf_ln(stderr, _("Please specify which branch you want to merge with."));
		fprintf_ln(stderr, _("See git-pull(1) for details."));
		fprintf(stderr, "\n");
		fprintf_ln(stderr, "    git pull %s %s", _("<remote>"), _("<branch>"));
		fprintf(stderr, "\n");
	} ଅନ୍ୟଥା ଯଦି (!curr_branch->merge_nr) {
		ଅଚଳ ଅକ୍ଷର *remote_name = ଖାଲି;

		ଯଦି (for_each_remote(get_only_remote, &remote_name) || !remote_name)
			remote_name = _("<remote>");

		fprintf_ln(stderr, _("There is no tracking information for the current branch."));
		ଯଦି (opt_rebase)
			fprintf_ln(stderr, _("Please specify which branch you want to rebase against."));
		ଅନ୍ୟଥା
			fprintf_ln(stderr, _("Please specify which branch you want to merge with."));
		fprintf_ln(stderr, _("See git-pull(1) for details."));
		fprintf(stderr, "\n");
		fprintf_ln(stderr, "    git pull %s %s", _("<remote>"), _("<branch>"));
		fprintf(stderr, "\n");
		fprintf_ln(stderr, _("If you wish to set tracking information for this branch you can do so with:"));
		fprintf(stderr, "\n");
		fprintf_ln(stderr, "    git branch --set-upstream-to=%s/%s %s\n",
				remote_name, _("<branch>"), curr_branch->name);
	} ଅନ୍ୟଥା
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
ସ୍ଥିର ଶୂନ୍ୟ parse_repo_refspecs(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର **repo,
		ଅଚଳ ଅକ୍ଷର ***refspecs)
{
	ଯଦି (argc > 0) {
		*repo = *argv++;
		argc--;
	} ଅନ୍ୟଥା
		*repo = ଖାଲି;
	*refspecs = argv;
}

/**
 * Runs git-fetch, returning its exit status. `repo` and `refspecs` are the
 * repository and refspecs to fetch, or NULL if they are not provided.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା run_fetch(ଅଚଳ ଅକ୍ଷର *repo, ଅଚଳ ଅକ୍ଷର **refspecs)
{
	ସଂରଚନା child_process cmd = CHILD_PROCESS_INIT;

	strvec_pushl(&cmd.args, "fetch", "--update-head-ok", ଖାଲି);

	/* Shared options */
	argv_ଠେଲ_verbosity(&cmd.args);
	ଯଦି (opt_progress)
		strvec_ଠେଲ(&cmd.args, opt_progress);

	/* Options passed to git-fetch */
	ଯଦି (opt_all)
		strvec_ଠେଲ(&cmd.args, opt_all);
	ଯଦି (opt_append)
		strvec_ଠେଲ(&cmd.args, opt_append);
	ଯଦି (opt_upload_pack)
		strvec_ଠେଲ(&cmd.args, opt_upload_pack);
	argv_ଠେଲ_force(&cmd.args);
	ଯଦି (opt_tags)
		strvec_ଠେଲ(&cmd.args, opt_tags);
	ଯଦି (opt_prune)
		strvec_ଠେଲ(&cmd.args, opt_prune);
	ଯଦି (recurse_submodules_cli != RECURSE_SUBMODULES_DEFAULT)
		ବାଛ (recurse_submodules_cli) {
		ସ୍ଥିତି RECURSE_SUBMODULES_ON:
			strvec_ଠେଲ(&cmd.args, "--recurse-submodules=on");
			ଭାଙ୍ଗ;
		ସ୍ଥିତି RECURSE_SUBMODULES_OFF:
			strvec_ଠେଲ(&cmd.args, "--recurse-submodules=no");
			ଭାଙ୍ଗ;
		ସ୍ଥିତି RECURSE_SUBMODULES_ON_DEMAND:
			strvec_ଠେଲ(&cmd.args, "--recurse-submodules=on-demand");
			ଭାଙ୍ଗ;
		ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
			BUG("submodule recursion option not understood");
		}
	ଯଦି (max_children)
		strvec_ଠେଲ(&cmd.args, max_children);
	ଯଦି (opt_dry_run)
		strvec_ଠେଲ(&cmd.args, "--dry-run");
	ଯଦି (opt_keep)
		strvec_ଠେଲ(&cmd.args, opt_keep);
	ଯଦି (opt_depth)
		strvec_ଠେଲ(&cmd.args, opt_depth);
	ଯଦି (opt_unshallow)
		strvec_ଠେଲ(&cmd.args, opt_unshallow);
	ଯଦି (opt_update_shallow)
		strvec_ଠେଲ(&cmd.args, opt_update_shallow);
	ଯଦି (opt_refmap)
		strvec_ଠେଲ(&cmd.args, opt_refmap);
	ଯଦି (opt_ipv4)
		strvec_ଠେଲ(&cmd.args, opt_ipv4);
	ଯଦି (opt_ipv6)
		strvec_ଠେଲ(&cmd.args, opt_ipv6);
	ଯଦି (opt_show_forced_updates > 0)
		strvec_ଠେଲ(&cmd.args, "--show-forced-updates");
	ଅନ୍ୟଥା ଯଦି (opt_show_forced_updates == 0)
		strvec_ଠେଲ(&cmd.args, "--no-show-forced-updates");
	ଯଦି (set_upstream)
		strvec_ଠେଲ(&cmd.args, set_upstream);
	strvec_pushv(&cmd.args, opt_fetch.v);

	ଯଦି (repo) {
		strvec_ଠେଲ(&cmd.args, repo);
		strvec_pushv(&cmd.args, refspecs);
	} ଅନ୍ୟଥା ଯଦି (*refspecs)
		BUG("refspecs without repo?");
	cmd.git_cmd = 1;
	cmd.odb_to_close = the_repository->objects;
	ଫେରନ୍ତୁ run_command(&cmd);
}

/**
 * "Pulls into void" by branching off merge_head.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା pull_into_void(ଅଚଳ ସଂରଚନା object_id *merge_head,
		ଅଚଳ ସଂରଚନା object_id *curr_head)
{
	ଯଦି (opt_verify_signatures) {
		ସଂରଚନା commit *commit;

		commit = lookup_commit(the_repository, merge_head);
		ଯଦି (!commit)
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
	ଯଦି (checkout_fast_forward(the_repository,
				  the_hash_algo->empty_ବୃକ୍ଷ,
				  merge_head, 0))
		ଫେରନ୍ତୁ 1;

	ଯଦି (refs_update_ref(get_main_ref_store(the_repository), "initial pull", "HEAD", merge_head, curr_head, 0, UPDATE_REFS_DIE_ON_ERR))
		ଫେରନ୍ତୁ 1;

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା rebase_submodules(ଶୂନ୍ୟ)
{
	ସଂରଚନା child_process cp = CHILD_PROCESS_INIT;

	cp.git_cmd = 1;
	cp.no_stdin = 1;
	strvec_pushl(&cp.args, "submodule", "update",
		     "--recursive", "--rebase", ଖାଲି);
	argv_ଠେଲ_verbosity(&cp.args);

	ଫେରନ୍ତୁ run_command(&cp);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା update_submodules(ଶୂନ୍ୟ)
{
	ସଂରଚନା child_process cp = CHILD_PROCESS_INIT;

	cp.git_cmd = 1;
	cp.no_stdin = 1;
	strvec_pushl(&cp.args, "submodule", "update",
		     "--recursive", "--checkout", ଖାଲି);
	argv_ଠେଲ_verbosity(&cp.args);

	ଫେରନ୍ତୁ run_command(&cp);
}

/**
 * Runs git-merge, returning its exit status.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା run_merge(ଶୂନ୍ୟ)
{
	ସଂରଚନା child_process cmd = CHILD_PROCESS_INIT;

	strvec_pushl(&cmd.args, "merge", ଖାଲି);

	/* Shared options */
	argv_ଠେଲ_verbosity(&cmd.args);
	ଯଦି (opt_progress)
		strvec_ଠେଲ(&cmd.args, opt_progress);

	/* Options passed to git-merge */
	ଯଦି (opt_diffstat)
		strvec_ଠେଲ(&cmd.args, opt_diffstat);
	ଯଦି (opt_log)
		strvec_ଠେଲ(&cmd.args, opt_log);
	ଯଦି (opt_signoff)
		strvec_ଠେଲ(&cmd.args, opt_signoff);
	ଯଦି (opt_squash)
		strvec_ଠେଲ(&cmd.args, opt_squash);
	ଯଦି (opt_commit)
		strvec_ଠେଲ(&cmd.args, opt_commit);
	ଯଦି (opt_edit)
		strvec_ଠେଲ(&cmd.args, opt_edit);
	ଯଦି (cleanup_arg)
		strvec_pushf(&cmd.args, "--cleanup=%s", cleanup_arg);
	ଯଦି (opt_ff)
		strvec_ଠେଲ(&cmd.args, opt_ff);
	ଯଦି (opt_verify)
		strvec_ଠେଲ(&cmd.args, opt_verify);
	ଯଦି (opt_verify_signatures)
		strvec_ଠେଲ(&cmd.args, opt_verify_signatures);
	strvec_pushv(&cmd.args, opt_strategies.v);
	strvec_pushv(&cmd.args, opt_strategy_opts.v);
	ଯଦି (opt_gpg_sign)
		strvec_ଠେଲ(&cmd.args, opt_gpg_sign);
	ଯଦି (opt_autostash == 0)
		strvec_ଠେଲ(&cmd.args, "--no-autostash");
	ଅନ୍ୟଥା ଯଦି (opt_autostash == 1)
		strvec_ଠେଲ(&cmd.args, "--autostash");
	ଯଦି (opt_allow_unrelated_histories > 0)
		strvec_ଠେଲ(&cmd.args, "--allow-unrelated-histories");

	strvec_ଠେଲ(&cmd.args, "FETCH_HEAD");
	cmd.git_cmd = 1;
	ଫେରନ୍ତୁ run_command(&cmd);
}

/**
 * Returns remote's upstream branch for the current branch. If remote is NULL,
 * the current branch's configured default remote is used. Returns NULL if
 * `remote` does not name a valid remote, HEAD does not point to a branch,
 * remote is not the branch's configured remote or the branch does not have any
 * configured upstream branch.
 */
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *get_upstream_branch(ଅଚଳ ଅକ୍ଷର *remote)
{
	ସଂରଚନା remote *rm;
	ସଂରଚନା branch *curr_branch;
	ଅଚଳ ଅକ୍ଷର *curr_branch_remote;

	rm = remote_get(remote);
	ଯଦି (!rm)
		ଫେରନ୍ତୁ ଖାଲି;

	curr_branch = branch_get("HEAD");
	ଯଦି (!curr_branch)
		ଫେରନ୍ତୁ ଖାଲି;

	curr_branch_remote = remote_for_branch(curr_branch, ଖାଲି);
	ପ୍ରତିଜ୍ଞା(curr_branch_remote);

	ଯଦି (strcmp(curr_branch_remote, rm->name))
		ଫେରନ୍ତୁ ଖାଲି;

	ଫେରନ୍ତୁ branch_get_upstream(curr_branch, ଖାଲି);
}

/**
 * Derives the remote-tracking branch from the remote and refspec.
 *
 * FIXME: The current implementation assumes the default mapping of
 * refs/heads/<branch_name> to refs/remotes/<remote_name>/<branch_name>.
 */
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *get_tracking_branch(ଅଚଳ ଅକ୍ଷର *remote, ଅଚଳ ଅକ୍ଷର *refspec)
{
	ସଂରଚନା refspec_item spec;
	ଅଚଳ ଅକ୍ଷର *spec_src;
	ଅଚଳ ଅକ୍ଷର *merge_branch;

	ଯଦି (!refspec_item_init_fetch(&spec, refspec, the_hash_algo))
		die(_("invalid refspec '%s'"), refspec);
	spec_src = spec.src;
	ଯଦି (!*spec_src || !strcmp(spec_src, "HEAD"))
		spec_src = "HEAD";
	ଅନ୍ୟଥା ଯଦି (skip_prefix(spec_src, "heads/", &spec_src))
		;
	ଅନ୍ୟଥା ଯଦି (skip_prefix(spec_src, "refs/heads/", &spec_src))
		;
	ଅନ୍ୟଥା ଯଦି (starts_with(spec_src, "refs/") ||
		starts_with(spec_src, "tags/") ||
		starts_with(spec_src, "remotes/"))
		spec_src = "";

	ଯଦି (*spec_src) {
		ଯଦି (!strcmp(remote, "."))
			merge_branch = mkpath("refs/heads/%s", spec_src);
		ଅନ୍ୟଥା
			merge_branch = mkpath("refs/remotes/%s/%s", remote, spec_src);
	} ଅନ୍ୟଥା
		merge_branch = ଖାଲି;

	refspec_item_clear(&spec);
	ଫେରନ୍ତୁ merge_branch;
}

/**
 * Given the repo and refspecs, sets fork_point to the point at which the
 * current branch forked from its remote-tracking branch. Returns 0 on success,
 * -1 on failure.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_rebase_fork_point(ସଂରଚନା object_id *fork_point, ଅଚଳ ଅକ୍ଷର *repo,
		ଅଚଳ ଅକ୍ଷର *refspec)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
	ସଂରଚନା branch *curr_branch;
	ଅଚଳ ଅକ୍ଷର *remote_branch;
	ସଂରଚନା child_process cp = CHILD_PROCESS_INIT;
	ସଂରଚନା strbuf sb = STRBUF_INIT;

	curr_branch = branch_get("HEAD");
	ଯଦି (!curr_branch)
		ଫେରନ୍ତୁ -1;

	ଯଦି (refspec)
		remote_branch = get_tracking_branch(repo, refspec);
	ଅନ୍ୟଥା
		remote_branch = get_upstream_branch(repo);

	ଯଦି (!remote_branch)
		ଫେରନ୍ତୁ -1;

	strvec_pushl(&cp.args, "merge-base", "--fork-point",
		     remote_branch, curr_branch->name, ଖାଲି);
	cp.no_stdin = 1;
	cp.no_stderr = 1;
	cp.git_cmd = 1;

	ret = capture_command(&cp, &sb, GIT_MAX_HEXSZ);
	ଯଦି (ret)
		ଯାଅ cleanup;

	ret = get_oid_hex(sb.buf, fork_point);
	ଯଦି (ret)
		ଯାଅ cleanup;

cleanup:
	strbuf_release(&sb);
	ଫେରନ୍ତୁ ret ? -1 : 0;
}

/**
 * Sets merge_base to the octopus merge base of curr_head, merge_head and
 * fork_point. Returns 0 if a merge base is found, 1 otherwise.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_octopus_merge_base(ସଂରଚନା object_id *merge_base,
		ଅଚଳ ସଂରଚନା object_id *curr_head,
		ଅଚଳ ସଂରଚନା object_id *merge_head,
		ଅଚଳ ସଂରଚନା object_id *fork_point)
{
	ସଂରଚନା commit_list *revs = ଖାଲି, *result = ଖାଲି;

	commit_list_ଭର୍ତ୍ତି(lookup_commit_reference(the_repository, curr_head),
			   &revs);
	commit_list_ଭର୍ତ୍ତି(lookup_commit_reference(the_repository, merge_head),
			   &revs);
	ଯଦି (!is_null_oid(fork_point))
		commit_list_ଭର୍ତ୍ତି(lookup_commit_reference(the_repository, fork_point),
				   &revs);

	ଯଦି (get_octopus_merge_bases(revs, &result) < 0)
		exit(128);
	commit_list_free(revs);
	reduce_heads_replace(&result);

	ଯଦି (!result)
		ଫେରନ୍ତୁ 1;

	oidcpy(merge_base, &result->item->object.oid);
	commit_list_free(result);
	ଫେରନ୍ତୁ 0;
}

/**
 * Given the current HEAD oid, the merge head returned from git-fetch and the
 * fork point calculated by get_rebase_fork_point(), compute the <newbase> and
 * <upstream> arguments to use for the upcoming git-rebase invocation.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_rebase_newbase_and_upstream(ସଂରଚନା object_id *newbase,
		ସଂରଚନା object_id *upstream,
		ଅଚଳ ସଂରଚନା object_id *curr_head,
		ଅଚଳ ସଂରଚନା object_id *merge_head,
		ଅଚଳ ସଂରଚନା object_id *fork_point)
{
	ସଂରଚନା object_id oct_merge_base;

	ଯଦି (!get_octopus_merge_base(&oct_merge_base, curr_head, merge_head, fork_point))
		ଯଦି (!is_null_oid(fork_point) && oideq(&oct_merge_base, fork_point))
			fork_point = ଖାଲି;

	ଯଦି (fork_point && !is_null_oid(fork_point))
		oidcpy(upstream, fork_point);
	ଅନ୍ୟଥା
		oidcpy(upstream, merge_head);

	oidcpy(newbase, merge_head);

	ଫେରନ୍ତୁ 0;
}

/**
 * Given the <newbase> and <upstream> calculated by
 * get_rebase_newbase_and_upstream(), runs git-rebase with the
 * appropriate arguments and returns its exit status.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା run_rebase(ଅଚଳ ସଂରଚନା object_id *newbase,
		ଅଚଳ ସଂରଚନା object_id *upstream)
{
	ସଂରଚନା child_process cmd = CHILD_PROCESS_INIT;

	strvec_ଠେଲ(&cmd.args, "rebase");

	/* Shared options */
	argv_ଠେଲ_verbosity(&cmd.args);

	/* Options passed to git-rebase */
	ଯଦି (opt_rebase == REBASE_MERGES)
		strvec_ଠେଲ(&cmd.args, "--rebase-merges");
	ଅନ୍ୟଥା ଯଦି (opt_rebase == REBASE_INTERACTIVE)
		strvec_ଠେଲ(&cmd.args, "--interactive");
	ଯଦି (opt_diffstat)
		strvec_ଠେଲ(&cmd.args, opt_diffstat);
	strvec_pushv(&cmd.args, opt_strategies.v);
	strvec_pushv(&cmd.args, opt_strategy_opts.v);
	ଯଦି (opt_gpg_sign)
		strvec_ଠେଲ(&cmd.args, opt_gpg_sign);
	ଯଦି (opt_signoff)
		strvec_ଠେଲ(&cmd.args, opt_signoff);
	ଯଦି (opt_autostash == 0)
		strvec_ଠେଲ(&cmd.args, "--no-autostash");
	ଅନ୍ୟଥା ଯଦି (opt_autostash == 1)
		strvec_ଠେଲ(&cmd.args, "--autostash");
	ଯଦି (opt_verify_signatures &&
	    !strcmp(opt_verify_signatures, "--verify-signatures"))
		warning(_("ignoring --verify-signatures for rebase"));

	strvec_ଠେଲ(&cmd.args, "--onto");
	strvec_ଠେଲ(&cmd.args, oid_to_hex(newbase));

	strvec_ଠେଲ(&cmd.args, oid_to_hex(upstream));

	cmd.git_cmd = 1;
	ଫେରନ୍ତୁ run_command(&cmd);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_can_ff(ସଂରଚନା object_id *orig_head,
		      ସଂରଚନା oid_array *merge_heads)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
	ସଂରଚନା commit_list *list = ଖାଲି;
	ସଂରଚନା commit *merge_head, *head;
	ସଂରଚନା object_id *orig_merge_head;

	ଯଦି (merge_heads->nr > 1)
		ଫେରନ୍ତୁ 0;

	orig_merge_head = &merge_heads->oid[0];
	head = lookup_commit_reference(the_repository, orig_head);
	ଯଦି (!head)
		ଫେରନ୍ତୁ 0;
	merge_head = lookup_commit_reference(the_repository, orig_merge_head);
	ଯଦି (!merge_head)
		ଫେରନ୍ତୁ 0;
	commit_list_ଭର୍ତ୍ତି(head, &list);
	ret = repo_is_descendant_of(the_repository, merge_head, list);
	commit_list_free(list);
	ଯଦି (ret < 0)
		exit(128);
	ଫେରନ୍ତୁ ret;
}

/*
 * Is orig_head a descendant of _all_ merge_heads?
 * Unfortunately is_descendant_of() cannot be used as it asks
 * if orig_head is a descendant of at least one of them.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା already_up_to_date(ସଂରଚନା object_id *orig_head,
			      ସଂରଚନା oid_array *merge_heads)
{
	ସଂରଚନା commit *ours;

	ours = lookup_commit_reference(the_repository, orig_head);
	ଯଦି (!ours)
		ଫେରନ୍ତୁ 0;
	ପାଇଁ (size_t i = 0; i < merge_heads->nr; i++) {
		ସଂରଚନା commit_list *list = ଖାଲି;
		ସଂରଚନା commit *theirs;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା ok;

		theirs = lookup_commit_reference(the_repository, &merge_heads->oid[i]);
		ଯଦି (!theirs)
			ଫେରନ୍ତୁ 0;
		commit_list_ଭର୍ତ୍ତି(theirs, &list);
		ok = repo_is_descendant_of(the_repository, ours, list);
		commit_list_free(list);
		ଯଦି (ok < 0)
			exit(128);
		ଯଦି (!ok)
			ଫେରନ୍ତୁ 0;
	}
	ଫେରନ୍ତୁ 1;
}

ସ୍ଥିର ଶୂନ୍ୟ show_advice_pull_non_ff(ଶୂନ୍ୟ)
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

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_pull(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
	     ଅଚଳ ଅକ୍ଷର **argv,
	     ଅଚଳ ଅକ୍ଷର *prefix,
	     ସଂରଚନା repository *repository UNUSED)
{
	ଅଚଳ ଅକ୍ଷର *repo, **refspecs;
	ସଂରଚନା oid_array merge_heads = OID_ARRAY_INIT;
	ସଂରଚନା object_id orig_head, curr_head;
	ସଂରଚନା object_id rebase_fork_point;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା rebase_unspecified = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା can_ff;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା divergent;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
	ସ୍ଥିର ସଂରଚନା option pull_options[] = {
		/* Shared options */
		OPT__VERBOSITY(&opt_verbosity),
		OPT_PASSTHRU(0, "progress", &opt_progress, ଖାଲି,
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
		OPT_PASSTHRU('n', ଖାଲି, &opt_diffstat, ଖାଲି,
			N_("do not show a diffstat at the end of the merge"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG),
		OPT_PASSTHRU(0, "stat", &opt_diffstat, ଖାଲି,
			N_("show a diffstat at the end of the merge"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "summary", &opt_diffstat, ଖାଲି,
			N_("(synonym to --stat)"),
			PARSE_OPT_NOARG | PARSE_OPT_HIDDEN),
		OPT_PASSTHRU(0, "compact-summary", &opt_diffstat, ଖାଲି,
			N_("show a compact-summary at the end of the merge"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "log", &opt_log, N_("n"),
			N_("add (at most <n>) entries from shortlog to merge commit message"),
			PARSE_OPT_OPTARG),
		OPT_PASSTHRU(0, "signoff", &opt_signoff, ଖାଲି,
			N_("add a Signed-off-by trailer"),
			PARSE_OPT_OPTARG),
		OPT_PASSTHRU(0, "squash", &opt_squash, ଖାଲି,
			N_("create a single commit instead of doing a merge"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "commit", &opt_commit, ଖାଲି,
			N_("perform a commit if the merge succeeds (default)"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "edit", &opt_edit, ଖାଲି,
			N_("edit message before committing"),
			PARSE_OPT_NOARG),
		OPT_CLEANUP(&cleanup_arg),
		OPT_PASSTHRU(0, "ff", &opt_ff, ଖାଲି,
			N_("allow fast-forward"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "ff-only", &opt_ff, ଖାଲି,
			N_("abort if fast-forward is not possible"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG),
		OPT_PASSTHRU(0, "verify", &opt_verify, ଖାଲି,
			N_("control use of pre-merge-commit and commit-msg hooks"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "verify-signatures", &opt_verify_signatures, ଖାଲି,
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
		OPT_PASSTHRU(0, "all", &opt_all, ଖାଲି,
			N_("fetch from all remotes"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU('a', "append", &opt_append, ଖାଲି,
			N_("append to .git/FETCH_HEAD instead of overwriting"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "upload-pack", &opt_upload_pack, N_("path"),
			N_("path to upload pack on remote end"),
			0),
		OPT__FORCE(&opt_force, N_("force overwrite of local branch"), 0),
		OPT_PASSTHRU('t', "tags", &opt_tags, ଖାଲି,
			N_("fetch all tags and associated objects"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU('p', "prune", &opt_prune, ଖାଲି,
			N_("prune remote-tracking branches no longer on remote"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU('j', "jobs", &max_children, N_("n"),
			N_("number of submodules pulled in parallel"),
			PARSE_OPT_OPTARG),
		OPT_BOOL(0, "dry-run", &opt_dry_run,
			N_("dry run")),
		OPT_PASSTHRU('k', "keep", &opt_keep, ଖାଲି,
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
		OPT_PASSTHRU(0, "unshallow", &opt_unshallow, ଖାଲି,
			N_("convert to a complete repository"),
			PARSE_OPT_NONEG | PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "update-shallow", &opt_update_shallow, ଖାଲି,
			N_("accept refs that update .git/shallow"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "refmap", &opt_refmap, N_("refmap"),
			N_("specify fetch refmap"),
			PARSE_OPT_NONEG),
		OPT_PASSTHRU_ARGV('o', "server-option", &opt_fetch,
			N_("server-specific"),
			N_("option to transmit"),
			0),
		OPT_PASSTHRU('4',  "ipv4", &opt_ipv4, ଖାଲି,
			N_("use IPv4 addresses only"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU('6',  "ipv6", &opt_ipv6, ଖାଲି,
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
		OPT_PASSTHRU(0, "set-upstream", &set_upstream, ଖାଲି,
			N_("set upstream for git pull/fetch"),
			PARSE_OPT_NOARG),

		OPT_END()
	};

	ଯଦି (!getenv("GIT_REFLOG_ACTION"))
		set_reflog_message(argc, argv);

	repo_config(the_repository, git_pull_config, ଖାଲି);
	ଯଦି (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_full_index = 0;
	}

	argc = parse_options(argc, argv, prefix, pull_options, pull_usage, 0);
	ଯଦି (opt_autostash == -1)
		opt_autostash = config_pull_autostash;

	ଯଦି (recurse_submodules_cli != RECURSE_SUBMODULES_DEFAULT)
		recurse_submodules = recurse_submodules_cli;

	ଯଦି (cleanup_arg)
		/*
		 * this only checks the validity of cleanup_arg; we don't need
		 * a valid value for use_editor
		 */
		get_cleanup_mode(cleanup_arg, 0);

	parse_repo_refspecs(argc, argv, &repo, &refspecs);

	ଯଦି (!opt_ff) {
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
		ଯଦି (opt_rebase >= 0 && opt_ff && !strcmp(opt_ff, "--ff-only")) {
			ମୁକ୍ତ(opt_ff);
			opt_ff = xstrdup("--ff");
		}
	}

	ଯଦି (opt_rebase < 0)
		opt_rebase = config_get_rebase(&rebase_unspecified);

	ଯଦି (repo_read_index_unmerged(the_repository))
		die_resolve_conflict("pull");

	ଯଦି (file_exists(git_path_merge_head(the_repository)))
		die_conclude_merge();

	ଯଦି (repo_get_oid(the_repository, "HEAD", &orig_head))
		oidclr(&orig_head, the_repository->hash_algo);

	ଯଦି (opt_rebase) {
		ଯଦି (opt_autostash == -1)
			opt_autostash = config_rebase_autostash;

		ଯଦି (is_null_oid(&orig_head) && !is_index_unborn(the_repository->index))
			die(_("Updating an unborn branch with changes added to the index."));

		ଯଦି (!opt_autostash)
			require_clean_work_ବୃକ୍ଷ(the_repository,
				N_("pull with rebase"),
				_("Please commit or stash them."), 1, 0);

		ଯଦି (get_rebase_fork_point(&rebase_fork_point, repo, *refspecs))
			oidclr(&rebase_fork_point, the_repository->hash_algo);
	}

	ଯଦି (run_fetch(repo, refspecs))
		ଫେରନ୍ତୁ 1;

	ଯଦି (opt_dry_run)
		ଫେରନ୍ତୁ 0;

	ଯଦି (repo_get_oid(the_repository, "HEAD", &curr_head))
		oidclr(&curr_head, the_repository->hash_algo);

	ଯଦି (!is_null_oid(&orig_head) && !is_null_oid(&curr_head) &&
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

		ଯଦି (checkout_fast_forward(the_repository, &orig_head,
					  &curr_head, 0))
			die(_("Cannot fast-forward your working tree.\n"
				"After making sure that you saved anything precious from\n"
				"$ git diff %s\n"
				"output, run\n"
				"$ git reset --hard\n"
				"to recover."), oid_to_hex(&orig_head));
	}

	get_merge_heads(&merge_heads);

	ଯଦି (!merge_heads.nr)
		die_no_merge_candidates(repo, refspecs);

	ଯଦି (is_null_oid(&orig_head)) {
		ଯଦି (merge_heads.nr > 1)
			die(_("Cannot merge multiple branches into empty head."));
		ret = pull_into_void(merge_heads.oid, &curr_head);
		ଯାଅ cleanup;
	}
	ଯଦି (merge_heads.nr > 1) {
		ଯଦି (opt_rebase)
			die(_("Cannot rebase onto multiple branches."));
		ଯଦି (opt_ff && !strcmp(opt_ff, "--ff-only"))
			die(_("Cannot fast-forward to multiple branches."));
	}

	can_ff = get_can_ff(&orig_head, &merge_heads);
	divergent = !can_ff && !already_up_to_date(&orig_head, &merge_heads);

	/* ff-only takes precedence over rebase */
	ଯଦି (opt_ff && !strcmp(opt_ff, "--ff-only")) {
		ଯଦି (divergent)
			die_ff_impossible();
		opt_rebase = REBASE_FALSE;
	}
	/* If no action specified and we can't fast forward, then warn. */
	ଯଦି (!opt_ff && rebase_unspecified && divergent) {
		show_advice_pull_non_ff();
		die(_("Need to specify how to reconcile divergent branches."));
	}

	ଯଦି (opt_rebase) {
		ସଂରଚନା object_id newbase;
		ସଂରଚନା object_id upstream;
		get_rebase_newbase_and_upstream(&newbase, &upstream, &curr_head,
						merge_heads.oid, &rebase_fork_point);

		ଯଦି ((recurse_submodules == RECURSE_SUBMODULES_ON ||
		     recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND) &&
		    submodule_touches_in_range(the_repository, &upstream, &curr_head))
			die(_("cannot rebase with locally recorded submodule modifications"));

		ଯଦି (can_ff) {
			/* we can fast-forward this without invoking rebase */
			ମୁକ୍ତ(opt_ff);
			opt_ff = xstrdup("--ff-only");
			ret = run_merge();
		} ଅନ୍ୟଥା {
			ret = run_rebase(&newbase, &upstream);
		}

		ଯଦି (!ret && (recurse_submodules == RECURSE_SUBMODULES_ON ||
			     recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND))
			ret = rebase_submodules();

		ଯାଅ cleanup;
	} ଅନ୍ୟଥା {
		ret = run_merge();
		ଯଦି (!ret && (recurse_submodules == RECURSE_SUBMODULES_ON ||
			     recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND))
			ret = update_submodules();
		ଯାଅ cleanup;
	}

cleanup:
	oid_array_clear(&merge_heads);
	ଫେରନ୍ତୁ ret;
}
