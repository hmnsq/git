/*
 * Builtin "git pull"
 *
 * Based on git-pull.sh by Junio C Hamano
 *
 * Fetch one or more remote refs and merge it/them into the current HEAD.
 */

#define USE_THE_REPOSITORY_VARIABLE

#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "advice.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "hex.h"
#அந்தர்பாவ "merge.h"
#அந்தர்பாவ "object-name.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "run-command.h"
#அந்தர்பாவ "oid-array.h"
#அந்தர்பாவ "remote.h"
#அந்தர்பாவ "dir.h"
#அந்தர்பாவ "path.h"
#அந்தர்பாவ "read-cache-ll.h"
#அந்தர்பாவ "rebase.h"
#அந்தர்பாவ "refs.h"
#அந்தர்பாவ "refspec.h"
#அந்தர்பாவ "submodule.h"
#அந்தர்பாவ "submodule-config.h"
#அந்தர்பாவ "wt-status.h"
#அந்தர்பாவ "commit-reach.h"
#அந்தர்பாவ "sequencer.h"

/**
 * Parses the value of --rebase. If value is a false value, returns
 * REBASE_FALSE. If value is a true value, returns REBASE_TRUE. If value is
 * "merges", returns REBASE_MERGES. If value is a invalid value, dies with
 * a fatal error if fatal is true, otherwise returns REBASE_INVALID.
 */
நிலையான எண்ணிக்கை rebase_type parse_config_rebase(மாறிலி எழுத்து *சாவி, மாறிலி எழுத்து *மதிப்பு,
		முழு fatal)
{
	எண்ணிக்கை rebase_type v = rebase_parse_மதிப்பு(மதிப்பு);
	எனில் (v != REBASE_INVALID)
		திரும்பு v;

	எனில் (fatal)
		die(_("invalid value for '%s': '%s'"), சாவி, மதிப்பு);
	இல்லையெனில்
		error(_("invalid value for '%s': '%s'"), சாவி, மதிப்பு);

	திரும்பு REBASE_INVALID;
}

/**
 * Callback for --rebase, which parses arg with parse_config_rebase().
 */
நிலையான முழு parse_opt_rebase(மாறிலி கட்டமைப்பு option *opt, மாறிலி எழுத்து *arg, முழு unset)
{
	எண்ணிக்கை rebase_type *மதிப்பு = opt->மதிப்பு;

	எனில் (arg)
		*மதிப்பு = parse_config_rebase("--rebase", arg, 0);
	இல்லையெனில்
		*மதிப்பு = unset ? REBASE_FALSE : REBASE_TRUE;
	திரும்பு *மதிப்பு == REBASE_INVALID ? -1 : 0;
}

நிலையான மாறிலி எழுத்து * மாறிலி pull_usage[] = {
	N_("git pull [<options>] [<repository> [<refspec>...]]"),
	வெற்று
};

/* Shared options */
நிலையான முழு opt_verbosity;
நிலையான மாறிலி எழுத்து *opt_progress;
நிலையான முழு recurse_submodules = RECURSE_SUBMODULES_DEFAULT;
நிலையான முழு recurse_submodules_cli = RECURSE_SUBMODULES_DEFAULT;

/* Options passed to git-merge or git-rebase */
நிலையான எண்ணிக்கை rebase_type opt_rebase = -1;
நிலையான மாறிலி எழுத்து *opt_diffstat;
நிலையான மாறிலி எழுத்து *opt_log;
நிலையான மாறிலி எழுத்து *opt_signoff;
நிலையான மாறிலி எழுத்து *opt_squash;
நிலையான மாறிலி எழுத்து *opt_commit;
நிலையான மாறிலி எழுத்து *opt_edit;
நிலையான மாறிலி எழுத்து *cleanup_arg;
நிலையான எழுத்து *opt_ff;
நிலையான மாறிலி எழுத்து *opt_verify_signatures;
நிலையான மாறிலி எழுத்து *opt_verify;
நிலையான முழு opt_autostash = -1;
நிலையான முழு config_rebase_autostash;
நிலையான முழு config_pull_autostash = -1;
நிலையான முழு check_trust_level = 1;
நிலையான கட்டமைப்பு strvec opt_strategies = STRVEC_INIT;
நிலையான கட்டமைப்பு strvec opt_strategy_opts = STRVEC_INIT;
நிலையான மாறிலி எழுத்து *opt_gpg_sign;
நிலையான முழு opt_allow_unrelated_histories;

/* Options passed to git-fetch */
நிலையான மாறிலி எழுத்து *opt_all;
நிலையான மாறிலி எழுத்து *opt_append;
நிலையான மாறிலி எழுத்து *opt_upload_pack;
நிலையான முழு opt_force;
நிலையான மாறிலி எழுத்து *opt_tags;
நிலையான மாறிலி எழுத்து *opt_prune;
நிலையான மாறிலி எழுத்து *max_children;
நிலையான முழு opt_dry_run;
நிலையான மாறிலி எழுத்து *opt_keep;
நிலையான மாறிலி எழுத்து *opt_depth;
நிலையான மாறிலி எழுத்து *opt_unshallow;
நிலையான மாறிலி எழுத்து *opt_update_shallow;
நிலையான மாறிலி எழுத்து *opt_refmap;
நிலையான மாறிலி எழுத்து *opt_ipv4;
நிலையான மாறிலி எழுத்து *opt_ipv6;
நிலையான முழு opt_show_forced_updates = -1;
நிலையான மாறிலி எழுத்து *set_upstream;
நிலையான கட்டமைப்பு strvec opt_fetch = STRVEC_INIT;

/**
 * Pushes "-q" or "-v" switches into arr to match the opt_verbosity level.
 */
நிலையான வெறுமை argv_தள்ளு_verbosity(கட்டமைப்பு strvec *arr)
{
	முழு verbosity;

	க்கு (verbosity = opt_verbosity; verbosity > 0; verbosity--)
		strvec_தள்ளு(arr, "-v");

	க்கு (verbosity = opt_verbosity; verbosity < 0; verbosity++)
		strvec_தள்ளு(arr, "-q");
}

/**
 * Pushes "-f" switches into arr to match the opt_force level.
 */
நிலையான வெறுமை argv_தள்ளு_force(கட்டமைப்பு strvec *arr)
{
	முழு force = opt_force;
	போது (force-- > 0)
		strvec_தள்ளு(arr, "-f");
}

/**
 * Sets the GIT_REFLOG_ACTION environment variable to the concatenation of argv
 */
நிலையான வெறுமை set_reflog_message(முழு argc, மாறிலி எழுத்து **argv)
{
	முழு i;
	கட்டமைப்பு strbuf செய்தி = STRBUF_INIT;

	க்கு (i = 0; i < argc; i++) {
		எனில் (i)
			strbuf_addch(&செய்தி, ' ');
		strbuf_addstr(&செய்தி, argv[i]);
	}

	setenv("GIT_REFLOG_ACTION", செய்தி.buf, 0);

	strbuf_release(&செய்தி);
}

/**
 * If pull.ff is unset, returns NULL. If pull.ff is "true", returns "--ff". If
 * pull.ff is "false", returns "--no-ff". If pull.ff is "only", returns
 * "--ff-only". Otherwise, if pull.ff is set to an invalid value, die with an
 * error.
 */
நிலையான மாறிலி எழுத்து *config_get_ff(வெறுமை)
{
	மாறிலி எழுத்து *மதிப்பு;

	எனில் (repo_config_get_மதிப்பு(the_repository, "pull.ff", &மதிப்பு))
		திரும்பு வெற்று;

	தேர்வு (git_parse_maybe_bool(மதிப்பு)) {
	நிலை 0:
		திரும்பு "--no-ff";
	நிலை 1:
		திரும்பு "--ff";
	}

	எனில் (!strcmp(மதிப்பு, "only"))
		திரும்பு "--ff-only";

	die(_("invalid value for '%s': '%s'"), "pull.ff", மதிப்பு);
}

/**
 * Returns the default configured value for --rebase. It first looks for the
 * value of "branch.$curr_branch.rebase", where $curr_branch is the current
 * branch, and if HEAD is detached or the configuration key does not exist,
 * looks for the value of "pull.rebase". If both configuration keys do not
 * exist, returns REBASE_FALSE.
 */
நிலையான எண்ணிக்கை rebase_type config_get_rebase(முழு *rebase_unspecified)
{
	கட்டமைப்பு branch *curr_branch = branch_get("HEAD");
	மாறிலி எழுத்து *மதிப்பு;

	எனில் (curr_branch) {
		எழுத்து *சாவி = xstrfmt("branch.%s.rebase", curr_branch->name);

		எனில் (!repo_config_get_மதிப்பு(the_repository, சாவி, &மதிப்பு)) {
			எண்ணிக்கை rebase_type ret = parse_config_rebase(சாவி, மதிப்பு, 1);
			விடுவி(சாவி);
			திரும்பு ret;
		}

		விடுவி(சாவி);
	}

	எனில் (!repo_config_get_மதிப்பு(the_repository, "pull.rebase", &மதிப்பு))
		திரும்பு parse_config_rebase("pull.rebase", மதிப்பு, 1);

	*rebase_unspecified = 1;

	திரும்பு REBASE_FALSE;
}

/**
 * Read config variables.
 */
நிலையான முழு git_pull_config(மாறிலி எழுத்து *var, மாறிலி எழுத்து *மதிப்பு,
			   மாறிலி கட்டமைப்பு config_context *ctx, வெறுமை *cb)
{
	எனில் (!strcmp(var, "rebase.autostash")) {
		/*
		 * run_rebase() also reads this option. The reason we handle it here is
		 * that when pull.rebase is true, a fast-forward may occur without
		 * invoking run_rebase(). We need to ensure that autostash is set even
		 * in the fast-forward case.
		 *
		 * run_merge() handles merge.autostash, so we don't handle it here.
		 */
		config_rebase_autostash = git_config_bool(var, மதிப்பு);
		திரும்பு 0;
	} இல்லையெனில் எனில் (!strcmp(var, "pull.autostash")) {
		config_pull_autostash = git_config_bool(var, மதிப்பு);
		திரும்பு 0;
	} இல்லையெனில் எனில் (!strcmp(var, "submodule.recurse")) {
		recurse_submodules = git_config_bool(var, மதிப்பு) ?
			RECURSE_SUBMODULES_ON : RECURSE_SUBMODULES_OFF;
		திரும்பு 0;
	} இல்லையெனில் எனில் (!strcmp(var, "gpg.mintrustlevel")) {
		check_trust_level = 0;
	}

	திரும்பு git_default_config(var, மதிப்பு, ctx, cb);
}

/**
 * Appends merge candidates from FETCH_HEAD that are not marked not-for-merge
 * into merge_heads.
 */
நிலையான வெறுமை get_merge_heads(கட்டமைப்பு oid_array *merge_heads)
{
	மாறிலி எழுத்து *filename = git_path_fetch_head(the_repository);
	FILE *fp;
	கட்டமைப்பு strbuf sb = STRBUF_INIT;
	கட்டமைப்பு object_id oid;

	fp = xfopen(filename, "r");
	போது (strbuf_getline_lf(&sb, fp) != EOF) {
		மாறிலி எழுத்து *p;
		எனில் (parse_oid_hex(sb.buf, &oid, &p))
			தொடர்;  /* invalid line: does not start with object ID */
		எனில் (starts_with(p, "\tnot-for-merge\t"))
			தொடர்;  /* ref is not-for-merge */
		oid_array_append(merge_heads, &oid);
	}
	fclose(fp);
	strbuf_release(&sb);
}

/**
 * Used by die_no_merge_candidates() as a for_each_remote() callback to
 * retrieve the name of the remote if the repository only has one remote.
 */
நிலையான முழு get_only_remote(கட்டமைப்பு remote *remote, வெறுமை *cb_data)
{
	மாறிலி எழுத்து **remote_name = cb_data;

	எனில் (*remote_name)
		திரும்பு -1;

	*remote_name = remote->name;
	திரும்பு 0;
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
நிலையான வெறுமை NORETURN die_no_merge_candidates(மாறிலி எழுத்து *repo, மாறிலி எழுத்து **refspecs)
{
	கட்டமைப்பு branch *curr_branch = branch_get("HEAD");
	மாறிலி எழுத்து *remote = curr_branch ? curr_branch->remote_name : வெற்று;

	எனில் (*refspecs) {
		எனில் (opt_rebase)
			fprintf_ln(stderr, _("There is no candidate for rebasing against among the refs that you just fetched."));
		இல்லையெனில்
			fprintf_ln(stderr, _("There are no candidates for merging among the refs that you just fetched."));
		fprintf_ln(stderr, _("Generally this means that you provided a wildcard refspec which had no\n"
					"matches on the remote end."));
	} இல்லையெனில் எனில் (repo && curr_branch && (!remote || strcmp(repo, remote))) {
		fprintf_ln(stderr, _("You asked to pull from the remote '%s', but did not specify\n"
			"a branch. Because this is not the default configured remote\n"
			"for your current branch, you must specify a branch on the command line."),
			repo);
	} இல்லையெனில் எனில் (!curr_branch) {
		fprintf_ln(stderr, _("You are not currently on a branch."));
		எனில் (opt_rebase)
			fprintf_ln(stderr, _("Please specify which branch you want to rebase against."));
		இல்லையெனில்
			fprintf_ln(stderr, _("Please specify which branch you want to merge with."));
		fprintf_ln(stderr, _("See git-pull(1) for details."));
		fprintf(stderr, "\n");
		fprintf_ln(stderr, "    git pull %s %s", _("<remote>"), _("<branch>"));
		fprintf(stderr, "\n");
	} இல்லையெனில் எனில் (!curr_branch->merge_nr) {
		மாறிலி எழுத்து *remote_name = வெற்று;

		எனில் (for_each_remote(get_only_remote, &remote_name) || !remote_name)
			remote_name = _("<remote>");

		fprintf_ln(stderr, _("There is no tracking information for the current branch."));
		எனில் (opt_rebase)
			fprintf_ln(stderr, _("Please specify which branch you want to rebase against."));
		இல்லையெனில்
			fprintf_ln(stderr, _("Please specify which branch you want to merge with."));
		fprintf_ln(stderr, _("See git-pull(1) for details."));
		fprintf(stderr, "\n");
		fprintf_ln(stderr, "    git pull %s %s", _("<remote>"), _("<branch>"));
		fprintf(stderr, "\n");
		fprintf_ln(stderr, _("If you wish to set tracking information for this branch you can do so with:"));
		fprintf(stderr, "\n");
		fprintf_ln(stderr, "    git branch --set-upstream-to=%s/%s %s\n",
				remote_name, _("<branch>"), curr_branch->name);
	} இல்லையெனில்
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
நிலையான வெறுமை parse_repo_refspecs(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து **repo,
		மாறிலி எழுத்து ***refspecs)
{
	எனில் (argc > 0) {
		*repo = *argv++;
		argc--;
	} இல்லையெனில்
		*repo = வெற்று;
	*refspecs = argv;
}

/**
 * Runs git-fetch, returning its exit status. `repo` and `refspecs` are the
 * repository and refspecs to fetch, or NULL if they are not provided.
 */
நிலையான முழு run_fetch(மாறிலி எழுத்து *repo, மாறிலி எழுத்து **refspecs)
{
	கட்டமைப்பு child_process cmd = CHILD_PROCESS_INIT;

	strvec_pushl(&cmd.args, "fetch", "--update-head-ok", வெற்று);

	/* Shared options */
	argv_தள்ளு_verbosity(&cmd.args);
	எனில் (opt_progress)
		strvec_தள்ளு(&cmd.args, opt_progress);

	/* Options passed to git-fetch */
	எனில் (opt_all)
		strvec_தள்ளு(&cmd.args, opt_all);
	எனில் (opt_append)
		strvec_தள்ளு(&cmd.args, opt_append);
	எனில் (opt_upload_pack)
		strvec_தள்ளு(&cmd.args, opt_upload_pack);
	argv_தள்ளு_force(&cmd.args);
	எனில் (opt_tags)
		strvec_தள்ளு(&cmd.args, opt_tags);
	எனில் (opt_prune)
		strvec_தள்ளு(&cmd.args, opt_prune);
	எனில் (recurse_submodules_cli != RECURSE_SUBMODULES_DEFAULT)
		தேர்வு (recurse_submodules_cli) {
		நிலை RECURSE_SUBMODULES_ON:
			strvec_தள்ளு(&cmd.args, "--recurse-submodules=on");
			நிறுத்து;
		நிலை RECURSE_SUBMODULES_OFF:
			strvec_தள்ளு(&cmd.args, "--recurse-submodules=no");
			நிறுத்து;
		நிலை RECURSE_SUBMODULES_ON_DEMAND:
			strvec_தள்ளு(&cmd.args, "--recurse-submodules=on-demand");
			நிறுத்து;
		இயல்பு:
			BUG("submodule recursion option not understood");
		}
	எனில் (max_children)
		strvec_தள்ளு(&cmd.args, max_children);
	எனில் (opt_dry_run)
		strvec_தள்ளு(&cmd.args, "--dry-run");
	எனில் (opt_keep)
		strvec_தள்ளு(&cmd.args, opt_keep);
	எனில் (opt_depth)
		strvec_தள்ளு(&cmd.args, opt_depth);
	எனில் (opt_unshallow)
		strvec_தள்ளு(&cmd.args, opt_unshallow);
	எனில் (opt_update_shallow)
		strvec_தள்ளு(&cmd.args, opt_update_shallow);
	எனில் (opt_refmap)
		strvec_தள்ளு(&cmd.args, opt_refmap);
	எனில் (opt_ipv4)
		strvec_தள்ளு(&cmd.args, opt_ipv4);
	எனில் (opt_ipv6)
		strvec_தள்ளு(&cmd.args, opt_ipv6);
	எனில் (opt_show_forced_updates > 0)
		strvec_தள்ளு(&cmd.args, "--show-forced-updates");
	இல்லையெனில் எனில் (opt_show_forced_updates == 0)
		strvec_தள்ளு(&cmd.args, "--no-show-forced-updates");
	எனில் (set_upstream)
		strvec_தள்ளு(&cmd.args, set_upstream);
	strvec_pushv(&cmd.args, opt_fetch.v);

	எனில் (repo) {
		strvec_தள்ளு(&cmd.args, repo);
		strvec_pushv(&cmd.args, refspecs);
	} இல்லையெனில் எனில் (*refspecs)
		BUG("refspecs without repo?");
	cmd.git_cmd = 1;
	cmd.odb_to_close = the_repository->objects;
	திரும்பு run_command(&cmd);
}

/**
 * "Pulls into void" by branching off merge_head.
 */
நிலையான முழு pull_into_void(மாறிலி கட்டமைப்பு object_id *merge_head,
		மாறிலி கட்டமைப்பு object_id *curr_head)
{
	எனில் (opt_verify_signatures) {
		கட்டமைப்பு commit *commit;

		commit = lookup_commit(the_repository, merge_head);
		எனில் (!commit)
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
	எனில் (checkout_fast_forward(the_repository,
				  the_hash_algo->empty_மரம்,
				  merge_head, 0))
		திரும்பு 1;

	எனில் (refs_update_ref(get_main_ref_store(the_repository), "initial pull", "HEAD", merge_head, curr_head, 0, UPDATE_REFS_DIE_ON_ERR))
		திரும்பு 1;

	திரும்பு 0;
}

நிலையான முழு rebase_submodules(வெறுமை)
{
	கட்டமைப்பு child_process cp = CHILD_PROCESS_INIT;

	cp.git_cmd = 1;
	cp.no_stdin = 1;
	strvec_pushl(&cp.args, "submodule", "update",
		     "--recursive", "--rebase", வெற்று);
	argv_தள்ளு_verbosity(&cp.args);

	திரும்பு run_command(&cp);
}

நிலையான முழு update_submodules(வெறுமை)
{
	கட்டமைப்பு child_process cp = CHILD_PROCESS_INIT;

	cp.git_cmd = 1;
	cp.no_stdin = 1;
	strvec_pushl(&cp.args, "submodule", "update",
		     "--recursive", "--checkout", வெற்று);
	argv_தள்ளு_verbosity(&cp.args);

	திரும்பு run_command(&cp);
}

/**
 * Runs git-merge, returning its exit status.
 */
நிலையான முழு run_merge(வெறுமை)
{
	கட்டமைப்பு child_process cmd = CHILD_PROCESS_INIT;

	strvec_pushl(&cmd.args, "merge", வெற்று);

	/* Shared options */
	argv_தள்ளு_verbosity(&cmd.args);
	எனில் (opt_progress)
		strvec_தள்ளு(&cmd.args, opt_progress);

	/* Options passed to git-merge */
	எனில் (opt_diffstat)
		strvec_தள்ளு(&cmd.args, opt_diffstat);
	எனில் (opt_log)
		strvec_தள்ளு(&cmd.args, opt_log);
	எனில் (opt_signoff)
		strvec_தள்ளு(&cmd.args, opt_signoff);
	எனில் (opt_squash)
		strvec_தள்ளு(&cmd.args, opt_squash);
	எனில் (opt_commit)
		strvec_தள்ளு(&cmd.args, opt_commit);
	எனில் (opt_edit)
		strvec_தள்ளு(&cmd.args, opt_edit);
	எனில் (cleanup_arg)
		strvec_pushf(&cmd.args, "--cleanup=%s", cleanup_arg);
	எனில் (opt_ff)
		strvec_தள்ளு(&cmd.args, opt_ff);
	எனில் (opt_verify)
		strvec_தள்ளு(&cmd.args, opt_verify);
	எனில் (opt_verify_signatures)
		strvec_தள்ளு(&cmd.args, opt_verify_signatures);
	strvec_pushv(&cmd.args, opt_strategies.v);
	strvec_pushv(&cmd.args, opt_strategy_opts.v);
	எனில் (opt_gpg_sign)
		strvec_தள்ளு(&cmd.args, opt_gpg_sign);
	எனில் (opt_autostash == 0)
		strvec_தள்ளு(&cmd.args, "--no-autostash");
	இல்லையெனில் எனில் (opt_autostash == 1)
		strvec_தள்ளு(&cmd.args, "--autostash");
	எனில் (opt_allow_unrelated_histories > 0)
		strvec_தள்ளு(&cmd.args, "--allow-unrelated-histories");

	strvec_தள்ளு(&cmd.args, "FETCH_HEAD");
	cmd.git_cmd = 1;
	திரும்பு run_command(&cmd);
}

/**
 * Returns remote's upstream branch for the current branch. If remote is NULL,
 * the current branch's configured default remote is used. Returns NULL if
 * `remote` does not name a valid remote, HEAD does not point to a branch,
 * remote is not the branch's configured remote or the branch does not have any
 * configured upstream branch.
 */
நிலையான மாறிலி எழுத்து *get_upstream_branch(மாறிலி எழுத்து *remote)
{
	கட்டமைப்பு remote *rm;
	கட்டமைப்பு branch *curr_branch;
	மாறிலி எழுத்து *curr_branch_remote;

	rm = remote_get(remote);
	எனில் (!rm)
		திரும்பு வெற்று;

	curr_branch = branch_get("HEAD");
	எனில் (!curr_branch)
		திரும்பு வெற்று;

	curr_branch_remote = remote_for_branch(curr_branch, வெற்று);
	ப்ரதிஜ்ஞா(curr_branch_remote);

	எனில் (strcmp(curr_branch_remote, rm->name))
		திரும்பு வெற்று;

	திரும்பு branch_get_upstream(curr_branch, வெற்று);
}

/**
 * Derives the remote-tracking branch from the remote and refspec.
 *
 * FIXME: The current implementation assumes the default mapping of
 * refs/heads/<branch_name> to refs/remotes/<remote_name>/<branch_name>.
 */
நிலையான மாறிலி எழுத்து *get_tracking_branch(மாறிலி எழுத்து *remote, மாறிலி எழுத்து *refspec)
{
	கட்டமைப்பு refspec_item spec;
	மாறிலி எழுத்து *spec_src;
	மாறிலி எழுத்து *merge_branch;

	எனில் (!refspec_item_init_fetch(&spec, refspec, the_hash_algo))
		die(_("invalid refspec '%s'"), refspec);
	spec_src = spec.src;
	எனில் (!*spec_src || !strcmp(spec_src, "HEAD"))
		spec_src = "HEAD";
	இல்லையெனில் எனில் (skip_prefix(spec_src, "heads/", &spec_src))
		;
	இல்லையெனில் எனில் (skip_prefix(spec_src, "refs/heads/", &spec_src))
		;
	இல்லையெனில் எனில் (starts_with(spec_src, "refs/") ||
		starts_with(spec_src, "tags/") ||
		starts_with(spec_src, "remotes/"))
		spec_src = "";

	எனில் (*spec_src) {
		எனில் (!strcmp(remote, "."))
			merge_branch = mkpath("refs/heads/%s", spec_src);
		இல்லையெனில்
			merge_branch = mkpath("refs/remotes/%s/%s", remote, spec_src);
	} இல்லையெனில்
		merge_branch = வெற்று;

	refspec_item_clear(&spec);
	திரும்பு merge_branch;
}

/**
 * Given the repo and refspecs, sets fork_point to the point at which the
 * current branch forked from its remote-tracking branch. Returns 0 on success,
 * -1 on failure.
 */
நிலையான முழு get_rebase_fork_point(கட்டமைப்பு object_id *fork_point, மாறிலி எழுத்து *repo,
		மாறிலி எழுத்து *refspec)
{
	முழு ret;
	கட்டமைப்பு branch *curr_branch;
	மாறிலி எழுத்து *remote_branch;
	கட்டமைப்பு child_process cp = CHILD_PROCESS_INIT;
	கட்டமைப்பு strbuf sb = STRBUF_INIT;

	curr_branch = branch_get("HEAD");
	எனில் (!curr_branch)
		திரும்பு -1;

	எனில் (refspec)
		remote_branch = get_tracking_branch(repo, refspec);
	இல்லையெனில்
		remote_branch = get_upstream_branch(repo);

	எனில் (!remote_branch)
		திரும்பு -1;

	strvec_pushl(&cp.args, "merge-base", "--fork-point",
		     remote_branch, curr_branch->name, வெற்று);
	cp.no_stdin = 1;
	cp.no_stderr = 1;
	cp.git_cmd = 1;

	ret = capture_command(&cp, &sb, GIT_MAX_HEXSZ);
	எனில் (ret)
		செல் cleanup;

	ret = get_oid_hex(sb.buf, fork_point);
	எனில் (ret)
		செல் cleanup;

cleanup:
	strbuf_release(&sb);
	திரும்பு ret ? -1 : 0;
}

/**
 * Sets merge_base to the octopus merge base of curr_head, merge_head and
 * fork_point. Returns 0 if a merge base is found, 1 otherwise.
 */
நிலையான முழு get_octopus_merge_base(கட்டமைப்பு object_id *merge_base,
		மாறிலி கட்டமைப்பு object_id *curr_head,
		மாறிலி கட்டமைப்பு object_id *merge_head,
		மாறிலி கட்டமைப்பு object_id *fork_point)
{
	கட்டமைப்பு commit_list *revs = வெற்று, *result = வெற்று;

	commit_list_செருகு(lookup_commit_reference(the_repository, curr_head),
			   &revs);
	commit_list_செருகு(lookup_commit_reference(the_repository, merge_head),
			   &revs);
	எனில் (!is_null_oid(fork_point))
		commit_list_செருகு(lookup_commit_reference(the_repository, fork_point),
				   &revs);

	எனில் (get_octopus_merge_bases(revs, &result) < 0)
		exit(128);
	commit_list_free(revs);
	reduce_heads_replace(&result);

	எனில் (!result)
		திரும்பு 1;

	oidcpy(merge_base, &result->item->object.oid);
	commit_list_free(result);
	திரும்பு 0;
}

/**
 * Given the current HEAD oid, the merge head returned from git-fetch and the
 * fork point calculated by get_rebase_fork_point(), compute the <newbase> and
 * <upstream> arguments to use for the upcoming git-rebase invocation.
 */
நிலையான முழு get_rebase_newbase_and_upstream(கட்டமைப்பு object_id *newbase,
		கட்டமைப்பு object_id *upstream,
		மாறிலி கட்டமைப்பு object_id *curr_head,
		மாறிலி கட்டமைப்பு object_id *merge_head,
		மாறிலி கட்டமைப்பு object_id *fork_point)
{
	கட்டமைப்பு object_id oct_merge_base;

	எனில் (!get_octopus_merge_base(&oct_merge_base, curr_head, merge_head, fork_point))
		எனில் (!is_null_oid(fork_point) && oideq(&oct_merge_base, fork_point))
			fork_point = வெற்று;

	எனில் (fork_point && !is_null_oid(fork_point))
		oidcpy(upstream, fork_point);
	இல்லையெனில்
		oidcpy(upstream, merge_head);

	oidcpy(newbase, merge_head);

	திரும்பு 0;
}

/**
 * Given the <newbase> and <upstream> calculated by
 * get_rebase_newbase_and_upstream(), runs git-rebase with the
 * appropriate arguments and returns its exit status.
 */
நிலையான முழு run_rebase(மாறிலி கட்டமைப்பு object_id *newbase,
		மாறிலி கட்டமைப்பு object_id *upstream)
{
	கட்டமைப்பு child_process cmd = CHILD_PROCESS_INIT;

	strvec_தள்ளு(&cmd.args, "rebase");

	/* Shared options */
	argv_தள்ளு_verbosity(&cmd.args);

	/* Options passed to git-rebase */
	எனில் (opt_rebase == REBASE_MERGES)
		strvec_தள்ளு(&cmd.args, "--rebase-merges");
	இல்லையெனில் எனில் (opt_rebase == REBASE_INTERACTIVE)
		strvec_தள்ளு(&cmd.args, "--interactive");
	எனில் (opt_diffstat)
		strvec_தள்ளு(&cmd.args, opt_diffstat);
	strvec_pushv(&cmd.args, opt_strategies.v);
	strvec_pushv(&cmd.args, opt_strategy_opts.v);
	எனில் (opt_gpg_sign)
		strvec_தள்ளு(&cmd.args, opt_gpg_sign);
	எனில் (opt_signoff)
		strvec_தள்ளு(&cmd.args, opt_signoff);
	எனில் (opt_autostash == 0)
		strvec_தள்ளு(&cmd.args, "--no-autostash");
	இல்லையெனில் எனில் (opt_autostash == 1)
		strvec_தள்ளு(&cmd.args, "--autostash");
	எனில் (opt_verify_signatures &&
	    !strcmp(opt_verify_signatures, "--verify-signatures"))
		warning(_("ignoring --verify-signatures for rebase"));

	strvec_தள்ளு(&cmd.args, "--onto");
	strvec_தள்ளு(&cmd.args, oid_to_hex(newbase));

	strvec_தள்ளு(&cmd.args, oid_to_hex(upstream));

	cmd.git_cmd = 1;
	திரும்பு run_command(&cmd);
}

நிலையான முழு get_can_ff(கட்டமைப்பு object_id *orig_head,
		      கட்டமைப்பு oid_array *merge_heads)
{
	முழு ret;
	கட்டமைப்பு commit_list *list = வெற்று;
	கட்டமைப்பு commit *merge_head, *head;
	கட்டமைப்பு object_id *orig_merge_head;

	எனில் (merge_heads->nr > 1)
		திரும்பு 0;

	orig_merge_head = &merge_heads->oid[0];
	head = lookup_commit_reference(the_repository, orig_head);
	எனில் (!head)
		திரும்பு 0;
	merge_head = lookup_commit_reference(the_repository, orig_merge_head);
	எனில் (!merge_head)
		திரும்பு 0;
	commit_list_செருகு(head, &list);
	ret = repo_is_descendant_of(the_repository, merge_head, list);
	commit_list_free(list);
	எனில் (ret < 0)
		exit(128);
	திரும்பு ret;
}

/*
 * Is orig_head a descendant of _all_ merge_heads?
 * Unfortunately is_descendant_of() cannot be used as it asks
 * if orig_head is a descendant of at least one of them.
 */
நிலையான முழு already_up_to_date(கட்டமைப்பு object_id *orig_head,
			      கட்டமைப்பு oid_array *merge_heads)
{
	கட்டமைப்பு commit *ours;

	ours = lookup_commit_reference(the_repository, orig_head);
	எனில் (!ours)
		திரும்பு 0;
	க்கு (size_t i = 0; i < merge_heads->nr; i++) {
		கட்டமைப்பு commit_list *list = வெற்று;
		கட்டமைப்பு commit *theirs;
		முழு ok;

		theirs = lookup_commit_reference(the_repository, &merge_heads->oid[i]);
		எனில் (!theirs)
			திரும்பு 0;
		commit_list_செருகு(theirs, &list);
		ok = repo_is_descendant_of(the_repository, ours, list);
		commit_list_free(list);
		எனில் (ok < 0)
			exit(128);
		எனில் (!ok)
			திரும்பு 0;
	}
	திரும்பு 1;
}

நிலையான வெறுமை show_advice_pull_non_ff(வெறுமை)
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

முழு cmd_pull(முழு argc,
	     மாறிலி எழுத்து **argv,
	     மாறிலி எழுத்து *prefix,
	     கட்டமைப்பு repository *repository UNUSED)
{
	மாறிலி எழுத்து *repo, **refspecs;
	கட்டமைப்பு oid_array merge_heads = OID_ARRAY_INIT;
	கட்டமைப்பு object_id orig_head, curr_head;
	கட்டமைப்பு object_id rebase_fork_point;
	முழு rebase_unspecified = 0;
	முழு can_ff;
	முழு divergent;
	முழு ret;
	நிலையான கட்டமைப்பு option pull_options[] = {
		/* Shared options */
		OPT__VERBOSITY(&opt_verbosity),
		OPT_PASSTHRU(0, "progress", &opt_progress, வெற்று,
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
		OPT_PASSTHRU('n', வெற்று, &opt_diffstat, வெற்று,
			N_("do not show a diffstat at the end of the merge"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG),
		OPT_PASSTHRU(0, "stat", &opt_diffstat, வெற்று,
			N_("show a diffstat at the end of the merge"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "summary", &opt_diffstat, வெற்று,
			N_("(synonym to --stat)"),
			PARSE_OPT_NOARG | PARSE_OPT_HIDDEN),
		OPT_PASSTHRU(0, "compact-summary", &opt_diffstat, வெற்று,
			N_("show a compact-summary at the end of the merge"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "log", &opt_log, N_("n"),
			N_("add (at most <n>) entries from shortlog to merge commit message"),
			PARSE_OPT_OPTARG),
		OPT_PASSTHRU(0, "signoff", &opt_signoff, வெற்று,
			N_("add a Signed-off-by trailer"),
			PARSE_OPT_OPTARG),
		OPT_PASSTHRU(0, "squash", &opt_squash, வெற்று,
			N_("create a single commit instead of doing a merge"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "commit", &opt_commit, வெற்று,
			N_("perform a commit if the merge succeeds (default)"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "edit", &opt_edit, வெற்று,
			N_("edit message before committing"),
			PARSE_OPT_NOARG),
		OPT_CLEANUP(&cleanup_arg),
		OPT_PASSTHRU(0, "ff", &opt_ff, வெற்று,
			N_("allow fast-forward"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "ff-only", &opt_ff, வெற்று,
			N_("abort if fast-forward is not possible"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG),
		OPT_PASSTHRU(0, "verify", &opt_verify, வெற்று,
			N_("control use of pre-merge-commit and commit-msg hooks"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "verify-signatures", &opt_verify_signatures, வெற்று,
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
		OPT_PASSTHRU(0, "all", &opt_all, வெற்று,
			N_("fetch from all remotes"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU('a', "append", &opt_append, வெற்று,
			N_("append to .git/FETCH_HEAD instead of overwriting"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "upload-pack", &opt_upload_pack, N_("path"),
			N_("path to upload pack on remote end"),
			0),
		OPT__FORCE(&opt_force, N_("force overwrite of local branch"), 0),
		OPT_PASSTHRU('t', "tags", &opt_tags, வெற்று,
			N_("fetch all tags and associated objects"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU('p', "prune", &opt_prune, வெற்று,
			N_("prune remote-tracking branches no longer on remote"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU('j', "jobs", &max_children, N_("n"),
			N_("number of submodules pulled in parallel"),
			PARSE_OPT_OPTARG),
		OPT_BOOL(0, "dry-run", &opt_dry_run,
			N_("dry run")),
		OPT_PASSTHRU('k', "keep", &opt_keep, வெற்று,
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
		OPT_PASSTHRU(0, "unshallow", &opt_unshallow, வெற்று,
			N_("convert to a complete repository"),
			PARSE_OPT_NONEG | PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "update-shallow", &opt_update_shallow, வெற்று,
			N_("accept refs that update .git/shallow"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "refmap", &opt_refmap, N_("refmap"),
			N_("specify fetch refmap"),
			PARSE_OPT_NONEG),
		OPT_PASSTHRU_ARGV('o', "server-option", &opt_fetch,
			N_("server-specific"),
			N_("option to transmit"),
			0),
		OPT_PASSTHRU('4',  "ipv4", &opt_ipv4, வெற்று,
			N_("use IPv4 addresses only"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU('6',  "ipv6", &opt_ipv6, வெற்று,
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
		OPT_PASSTHRU(0, "set-upstream", &set_upstream, வெற்று,
			N_("set upstream for git pull/fetch"),
			PARSE_OPT_NOARG),

		OPT_END()
	};

	எனில் (!getenv("GIT_REFLOG_ACTION"))
		set_reflog_message(argc, argv);

	repo_config(the_repository, git_pull_config, வெற்று);
	எனில் (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_full_index = 0;
	}

	argc = parse_options(argc, argv, prefix, pull_options, pull_usage, 0);
	எனில் (opt_autostash == -1)
		opt_autostash = config_pull_autostash;

	எனில் (recurse_submodules_cli != RECURSE_SUBMODULES_DEFAULT)
		recurse_submodules = recurse_submodules_cli;

	எனில் (cleanup_arg)
		/*
		 * this only checks the validity of cleanup_arg; we don't need
		 * a valid value for use_editor
		 */
		get_cleanup_mode(cleanup_arg, 0);

	parse_repo_refspecs(argc, argv, &repo, &refspecs);

	எனில் (!opt_ff) {
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
		எனில் (opt_rebase >= 0 && opt_ff && !strcmp(opt_ff, "--ff-only")) {
			விடுவி(opt_ff);
			opt_ff = xstrdup("--ff");
		}
	}

	எனில் (opt_rebase < 0)
		opt_rebase = config_get_rebase(&rebase_unspecified);

	எனில் (repo_read_index_unmerged(the_repository))
		die_resolve_conflict("pull");

	எனில் (file_exists(git_path_merge_head(the_repository)))
		die_conclude_merge();

	எனில் (repo_get_oid(the_repository, "HEAD", &orig_head))
		oidclr(&orig_head, the_repository->hash_algo);

	எனில் (opt_rebase) {
		எனில் (opt_autostash == -1)
			opt_autostash = config_rebase_autostash;

		எனில் (is_null_oid(&orig_head) && !is_index_unborn(the_repository->index))
			die(_("Updating an unborn branch with changes added to the index."));

		எனில் (!opt_autostash)
			require_clean_work_மரம்(the_repository,
				N_("pull with rebase"),
				_("Please commit or stash them."), 1, 0);

		எனில் (get_rebase_fork_point(&rebase_fork_point, repo, *refspecs))
			oidclr(&rebase_fork_point, the_repository->hash_algo);
	}

	எனில் (run_fetch(repo, refspecs))
		திரும்பு 1;

	எனில் (opt_dry_run)
		திரும்பு 0;

	எனில் (repo_get_oid(the_repository, "HEAD", &curr_head))
		oidclr(&curr_head, the_repository->hash_algo);

	எனில் (!is_null_oid(&orig_head) && !is_null_oid(&curr_head) &&
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

		எனில் (checkout_fast_forward(the_repository, &orig_head,
					  &curr_head, 0))
			die(_("Cannot fast-forward your working tree.\n"
				"After making sure that you saved anything precious from\n"
				"$ git diff %s\n"
				"output, run\n"
				"$ git reset --hard\n"
				"to recover."), oid_to_hex(&orig_head));
	}

	get_merge_heads(&merge_heads);

	எனில் (!merge_heads.nr)
		die_no_merge_candidates(repo, refspecs);

	எனில் (is_null_oid(&orig_head)) {
		எனில் (merge_heads.nr > 1)
			die(_("Cannot merge multiple branches into empty head."));
		ret = pull_into_void(merge_heads.oid, &curr_head);
		செல் cleanup;
	}
	எனில் (merge_heads.nr > 1) {
		எனில் (opt_rebase)
			die(_("Cannot rebase onto multiple branches."));
		எனில் (opt_ff && !strcmp(opt_ff, "--ff-only"))
			die(_("Cannot fast-forward to multiple branches."));
	}

	can_ff = get_can_ff(&orig_head, &merge_heads);
	divergent = !can_ff && !already_up_to_date(&orig_head, &merge_heads);

	/* ff-only takes precedence over rebase */
	எனில் (opt_ff && !strcmp(opt_ff, "--ff-only")) {
		எனில் (divergent)
			die_ff_impossible();
		opt_rebase = REBASE_FALSE;
	}
	/* If no action specified and we can't fast forward, then warn. */
	எனில் (!opt_ff && rebase_unspecified && divergent) {
		show_advice_pull_non_ff();
		die(_("Need to specify how to reconcile divergent branches."));
	}

	எனில் (opt_rebase) {
		கட்டமைப்பு object_id newbase;
		கட்டமைப்பு object_id upstream;
		get_rebase_newbase_and_upstream(&newbase, &upstream, &curr_head,
						merge_heads.oid, &rebase_fork_point);

		எனில் ((recurse_submodules == RECURSE_SUBMODULES_ON ||
		     recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND) &&
		    submodule_touches_in_range(the_repository, &upstream, &curr_head))
			die(_("cannot rebase with locally recorded submodule modifications"));

		எனில் (can_ff) {
			/* we can fast-forward this without invoking rebase */
			விடுவி(opt_ff);
			opt_ff = xstrdup("--ff-only");
			ret = run_merge();
		} இல்லையெனில் {
			ret = run_rebase(&newbase, &upstream);
		}

		எனில் (!ret && (recurse_submodules == RECURSE_SUBMODULES_ON ||
			     recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND))
			ret = rebase_submodules();

		செல் cleanup;
	} இல்லையெனில் {
		ret = run_merge();
		எனில் (!ret && (recurse_submodules == RECURSE_SUBMODULES_ON ||
			     recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND))
			ret = update_submodules();
		செல் cleanup;
	}

cleanup:
	oid_array_clear(&merge_heads);
	திரும்பு ret;
}
