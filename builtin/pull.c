/*
 * Builtin "git pull"
 *
 * Based on git-pull.sh by Junio C Hamano
 *
 * Fetch one or more remote refs and merge it/them into the current HEAD.
 */

#define USE_THE_REPOSITORY_VARIABLE

#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "advice.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "hex.h"
#અન્તર્ભાવ "merge.h"
#અન્તર્ભાવ "object-name.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "run-command.h"
#અન્તર્ભાવ "oid-array.h"
#અન્તર્ભાવ "remote.h"
#અન્તર્ભાવ "dir.h"
#અન્તર્ભાવ "path.h"
#અન્તર્ભાવ "read-cache-ll.h"
#અન્તર્ભાવ "rebase.h"
#અન્તર્ભાવ "refs.h"
#અન્તર્ભાવ "refspec.h"
#અન્તર્ભાવ "submodule.h"
#અન્તર્ભાવ "submodule-config.h"
#અન્તર્ભાવ "wt-status.h"
#અન્તર્ભાવ "commit-reach.h"
#અન્તર્ભાવ "sequencer.h"

/**
 * Parses the value of --rebase. If value is a false value, returns
 * REBASE_FALSE. If value is a true value, returns REBASE_TRUE. If value is
 * "merges", returns REBASE_MERGES. If value is a invalid value, dies with
 * a fatal error if fatal is true, otherwise returns REBASE_INVALID.
 */
સ્થિર ગણના rebase_type parse_config_rebase(અચલ અક્ષર *ચાવી, અચલ અક્ષર *કિંમત,
		પૂર્ણાંક fatal)
{
	ગણના rebase_type v = rebase_parse_કિંમત(કિંમત);
	જો (v != REBASE_INVALID)
		પાછા v;

	જો (fatal)
		die(_("invalid value for '%s': '%s'"), ચાવી, કિંમત);
	નહીંતર
		error(_("invalid value for '%s': '%s'"), ચાવી, કિંમત);

	પાછા REBASE_INVALID;
}

/**
 * Callback for --rebase, which parses arg with parse_config_rebase().
 */
સ્થિર પૂર્ણાંક parse_opt_rebase(અચલ સંરચના option *opt, અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	ગણના rebase_type *કિંમત = opt->કિંમત;

	જો (arg)
		*કિંમત = parse_config_rebase("--rebase", arg, 0);
	નહીંતર
		*કિંમત = unset ? REBASE_FALSE : REBASE_TRUE;
	પાછા *કિંમત == REBASE_INVALID ? -1 : 0;
}

સ્થિર અચલ અક્ષર * અચલ pull_usage[] = {
	N_("git pull [<options>] [<repository> [<refspec>...]]"),
	ખાલી
};

/* Shared options */
સ્થિર પૂર્ણાંક opt_verbosity;
સ્થિર અચલ અક્ષર *opt_progress;
સ્થિર પૂર્ણાંક recurse_submodules = RECURSE_SUBMODULES_DEFAULT;
સ્થિર પૂર્ણાંક recurse_submodules_cli = RECURSE_SUBMODULES_DEFAULT;

/* Options passed to git-merge or git-rebase */
સ્થિર ગણના rebase_type opt_rebase = -1;
સ્થિર અચલ અક્ષર *opt_diffstat;
સ્થિર અચલ અક્ષર *opt_log;
સ્થિર અચલ અક્ષર *opt_signoff;
સ્થિર અચલ અક્ષર *opt_squash;
સ્થિર અચલ અક્ષર *opt_commit;
સ્થિર અચલ અક્ષર *opt_edit;
સ્થિર અચલ અક્ષર *cleanup_arg;
સ્થિર અક્ષર *opt_ff;
સ્થિર અચલ અક્ષર *opt_verify_signatures;
સ્થિર અચલ અક્ષર *opt_verify;
સ્થિર પૂર્ણાંક opt_autostash = -1;
સ્થિર પૂર્ણાંક config_rebase_autostash;
સ્થિર પૂર્ણાંક config_pull_autostash = -1;
સ્થિર પૂર્ણાંક check_trust_level = 1;
સ્થિર સંરચના strvec opt_strategies = STRVEC_INIT;
સ્થિર સંરચના strvec opt_strategy_opts = STRVEC_INIT;
સ્થિર અચલ અક્ષર *opt_gpg_sign;
સ્થિર પૂર્ણાંક opt_allow_unrelated_histories;

/* Options passed to git-fetch */
સ્થિર અચલ અક્ષર *opt_all;
સ્થિર અચલ અક્ષર *opt_append;
સ્થિર અચલ અક્ષર *opt_upload_pack;
સ્થિર પૂર્ણાંક opt_force;
સ્થિર અચલ અક્ષર *opt_tags;
સ્થિર અચલ અક્ષર *opt_prune;
સ્થિર અચલ અક્ષર *max_children;
સ્થિર પૂર્ણાંક opt_dry_run;
સ્થિર અચલ અક્ષર *opt_keep;
સ્થિર અચલ અક્ષર *opt_depth;
સ્થિર અચલ અક્ષર *opt_unshallow;
સ્થિર અચલ અક્ષર *opt_update_shallow;
સ્થિર અચલ અક્ષર *opt_refmap;
સ્થિર અચલ અક્ષર *opt_ipv4;
સ્થિર અચલ અક્ષર *opt_ipv6;
સ્થિર પૂર્ણાંક opt_show_forced_updates = -1;
સ્થિર અચલ અક્ષર *set_upstream;
સ્થિર સંરચના strvec opt_fetch = STRVEC_INIT;

/**
 * Pushes "-q" or "-v" switches into arr to match the opt_verbosity level.
 */
સ્થિર શૂન્ય argv_ધકેલો_verbosity(સંરચના strvec *arr)
{
	પૂર્ણાંક verbosity;

	માટે (verbosity = opt_verbosity; verbosity > 0; verbosity--)
		strvec_ધકેલો(arr, "-v");

	માટે (verbosity = opt_verbosity; verbosity < 0; verbosity++)
		strvec_ધકેલો(arr, "-q");
}

/**
 * Pushes "-f" switches into arr to match the opt_force level.
 */
સ્થિર શૂન્ય argv_ધકેલો_force(સંરચના strvec *arr)
{
	પૂર્ણાંક force = opt_force;
	જ્યારે (force-- > 0)
		strvec_ધકેલો(arr, "-f");
}

/**
 * Sets the GIT_REFLOG_ACTION environment variable to the concatenation of argv
 */
સ્થિર શૂન્ય set_reflog_message(પૂર્ણાંક argc, અચલ અક્ષર **argv)
{
	પૂર્ણાંક i;
	સંરચના strbuf સંદેશ = STRBUF_INIT;

	માટે (i = 0; i < argc; i++) {
		જો (i)
			strbuf_addch(&સંદેશ, ' ');
		strbuf_addstr(&સંદેશ, argv[i]);
	}

	setenv("GIT_REFLOG_ACTION", સંદેશ.buf, 0);

	strbuf_release(&સંદેશ);
}

/**
 * If pull.ff is unset, returns NULL. If pull.ff is "true", returns "--ff". If
 * pull.ff is "false", returns "--no-ff". If pull.ff is "only", returns
 * "--ff-only". Otherwise, if pull.ff is set to an invalid value, die with an
 * error.
 */
સ્થિર અચલ અક્ષર *config_get_ff(શૂન્ય)
{
	અચલ અક્ષર *કિંમત;

	જો (repo_config_get_કિંમત(the_repository, "pull.ff", &કિંમત))
		પાછા ખાલી;

	પસંદગી (git_parse_maybe_bool(કિંમત)) {
	સ્થિતિ 0:
		પાછા "--no-ff";
	સ્થિતિ 1:
		પાછા "--ff";
	}

	જો (!strcmp(કિંમત, "only"))
		પાછા "--ff-only";

	die(_("invalid value for '%s': '%s'"), "pull.ff", કિંમત);
}

/**
 * Returns the default configured value for --rebase. It first looks for the
 * value of "branch.$curr_branch.rebase", where $curr_branch is the current
 * branch, and if HEAD is detached or the configuration key does not exist,
 * looks for the value of "pull.rebase". If both configuration keys do not
 * exist, returns REBASE_FALSE.
 */
સ્થિર ગણના rebase_type config_get_rebase(પૂર્ણાંક *rebase_unspecified)
{
	સંરચના branch *curr_branch = branch_get("HEAD");
	અચલ અક્ષર *કિંમત;

	જો (curr_branch) {
		અક્ષર *ચાવી = xstrfmt("branch.%s.rebase", curr_branch->name);

		જો (!repo_config_get_કિંમત(the_repository, ચાવી, &કિંમત)) {
			ગણના rebase_type ret = parse_config_rebase(ચાવી, કિંમત, 1);
			મુક્ત(ચાવી);
			પાછા ret;
		}

		મુક્ત(ચાવી);
	}

	જો (!repo_config_get_કિંમત(the_repository, "pull.rebase", &કિંમત))
		પાછા parse_config_rebase("pull.rebase", કિંમત, 1);

	*rebase_unspecified = 1;

	પાછા REBASE_FALSE;
}

/**
 * Read config variables.
 */
સ્થિર પૂર્ણાંક git_pull_config(અચલ અક્ષર *var, અચલ અક્ષર *કિંમત,
			   અચલ સંરચના config_context *ctx, શૂન્ય *cb)
{
	જો (!strcmp(var, "rebase.autostash")) {
		/*
		 * run_rebase() also reads this option. The reason we handle it here is
		 * that when pull.rebase is true, a fast-forward may occur without
		 * invoking run_rebase(). We need to ensure that autostash is set even
		 * in the fast-forward case.
		 *
		 * run_merge() handles merge.autostash, so we don't handle it here.
		 */
		config_rebase_autostash = git_config_bool(var, કિંમત);
		પાછા 0;
	} નહીંતર જો (!strcmp(var, "pull.autostash")) {
		config_pull_autostash = git_config_bool(var, કિંમત);
		પાછા 0;
	} નહીંતર જો (!strcmp(var, "submodule.recurse")) {
		recurse_submodules = git_config_bool(var, કિંમત) ?
			RECURSE_SUBMODULES_ON : RECURSE_SUBMODULES_OFF;
		પાછા 0;
	} નહીંતર જો (!strcmp(var, "gpg.mintrustlevel")) {
		check_trust_level = 0;
	}

	પાછા git_default_config(var, કિંમત, ctx, cb);
}

/**
 * Appends merge candidates from FETCH_HEAD that are not marked not-for-merge
 * into merge_heads.
 */
સ્થિર શૂન્ય get_merge_heads(સંરચના oid_array *merge_heads)
{
	અચલ અક્ષર *filename = git_path_fetch_head(the_repository);
	FILE *fp;
	સંરચના strbuf sb = STRBUF_INIT;
	સંરચના object_id oid;

	fp = xfopen(filename, "r");
	જ્યારે (strbuf_getline_lf(&sb, fp) != EOF) {
		અચલ અક્ષર *p;
		જો (parse_oid_hex(sb.buf, &oid, &p))
			ચાલુ;  /* invalid line: does not start with object ID */
		જો (starts_with(p, "\tnot-for-merge\t"))
			ચાલુ;  /* ref is not-for-merge */
		oid_array_append(merge_heads, &oid);
	}
	fclose(fp);
	strbuf_release(&sb);
}

/**
 * Used by die_no_merge_candidates() as a for_each_remote() callback to
 * retrieve the name of the remote if the repository only has one remote.
 */
સ્થિર પૂર્ણાંક get_only_remote(સંરચના remote *remote, શૂન્ય *cb_data)
{
	અચલ અક્ષર **remote_name = cb_data;

	જો (*remote_name)
		પાછા -1;

	*remote_name = remote->name;
	પાછા 0;
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
સ્થિર શૂન્ય NORETURN die_no_merge_candidates(અચલ અક્ષર *repo, અચલ અક્ષર **refspecs)
{
	સંરચના branch *curr_branch = branch_get("HEAD");
	અચલ અક્ષર *remote = curr_branch ? curr_branch->remote_name : ખાલી;

	જો (*refspecs) {
		જો (opt_rebase)
			fprintf_ln(stderr, _("There is no candidate for rebasing against among the refs that you just fetched."));
		નહીંતર
			fprintf_ln(stderr, _("There are no candidates for merging among the refs that you just fetched."));
		fprintf_ln(stderr, _("Generally this means that you provided a wildcard refspec which had no\n"
					"matches on the remote end."));
	} નહીંતર જો (repo && curr_branch && (!remote || strcmp(repo, remote))) {
		fprintf_ln(stderr, _("You asked to pull from the remote '%s', but did not specify\n"
			"a branch. Because this is not the default configured remote\n"
			"for your current branch, you must specify a branch on the command line."),
			repo);
	} નહીંતર જો (!curr_branch) {
		fprintf_ln(stderr, _("You are not currently on a branch."));
		જો (opt_rebase)
			fprintf_ln(stderr, _("Please specify which branch you want to rebase against."));
		નહીંતર
			fprintf_ln(stderr, _("Please specify which branch you want to merge with."));
		fprintf_ln(stderr, _("See git-pull(1) for details."));
		fprintf(stderr, "\n");
		fprintf_ln(stderr, "    git pull %s %s", _("<remote>"), _("<branch>"));
		fprintf(stderr, "\n");
	} નહીંતર જો (!curr_branch->merge_nr) {
		અચલ અક્ષર *remote_name = ખાલી;

		જો (for_each_remote(get_only_remote, &remote_name) || !remote_name)
			remote_name = _("<remote>");

		fprintf_ln(stderr, _("There is no tracking information for the current branch."));
		જો (opt_rebase)
			fprintf_ln(stderr, _("Please specify which branch you want to rebase against."));
		નહીંતર
			fprintf_ln(stderr, _("Please specify which branch you want to merge with."));
		fprintf_ln(stderr, _("See git-pull(1) for details."));
		fprintf(stderr, "\n");
		fprintf_ln(stderr, "    git pull %s %s", _("<remote>"), _("<branch>"));
		fprintf(stderr, "\n");
		fprintf_ln(stderr, _("If you wish to set tracking information for this branch you can do so with:"));
		fprintf(stderr, "\n");
		fprintf_ln(stderr, "    git branch --set-upstream-to=%s/%s %s\n",
				remote_name, _("<branch>"), curr_branch->name);
	} નહીંતર
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
સ્થિર શૂન્ય parse_repo_refspecs(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર **repo,
		અચલ અક્ષર ***refspecs)
{
	જો (argc > 0) {
		*repo = *argv++;
		argc--;
	} નહીંતર
		*repo = ખાલી;
	*refspecs = argv;
}

/**
 * Runs git-fetch, returning its exit status. `repo` and `refspecs` are the
 * repository and refspecs to fetch, or NULL if they are not provided.
 */
સ્થિર પૂર્ણાંક run_fetch(અચલ અક્ષર *repo, અચલ અક્ષર **refspecs)
{
	સંરચના child_process cmd = CHILD_PROCESS_INIT;

	strvec_pushl(&cmd.args, "fetch", "--update-head-ok", ખાલી);

	/* Shared options */
	argv_ધકેલો_verbosity(&cmd.args);
	જો (opt_progress)
		strvec_ધકેલો(&cmd.args, opt_progress);

	/* Options passed to git-fetch */
	જો (opt_all)
		strvec_ધકેલો(&cmd.args, opt_all);
	જો (opt_append)
		strvec_ધકેલો(&cmd.args, opt_append);
	જો (opt_upload_pack)
		strvec_ધકેલો(&cmd.args, opt_upload_pack);
	argv_ધકેલો_force(&cmd.args);
	જો (opt_tags)
		strvec_ધકેલો(&cmd.args, opt_tags);
	જો (opt_prune)
		strvec_ધકેલો(&cmd.args, opt_prune);
	જો (recurse_submodules_cli != RECURSE_SUBMODULES_DEFAULT)
		પસંદગી (recurse_submodules_cli) {
		સ્થિતિ RECURSE_SUBMODULES_ON:
			strvec_ધકેલો(&cmd.args, "--recurse-submodules=on");
			તોડો;
		સ્થિતિ RECURSE_SUBMODULES_OFF:
			strvec_ધકેલો(&cmd.args, "--recurse-submodules=no");
			તોડો;
		સ્થિતિ RECURSE_SUBMODULES_ON_DEMAND:
			strvec_ધકેલો(&cmd.args, "--recurse-submodules=on-demand");
			તોડો;
		મૂળભૂત:
			BUG("submodule recursion option not understood");
		}
	જો (max_children)
		strvec_ધકેલો(&cmd.args, max_children);
	જો (opt_dry_run)
		strvec_ધકેલો(&cmd.args, "--dry-run");
	જો (opt_keep)
		strvec_ધકેલો(&cmd.args, opt_keep);
	જો (opt_depth)
		strvec_ધકેલો(&cmd.args, opt_depth);
	જો (opt_unshallow)
		strvec_ધકેલો(&cmd.args, opt_unshallow);
	જો (opt_update_shallow)
		strvec_ધકેલો(&cmd.args, opt_update_shallow);
	જો (opt_refmap)
		strvec_ધકેલો(&cmd.args, opt_refmap);
	જો (opt_ipv4)
		strvec_ધકેલો(&cmd.args, opt_ipv4);
	જો (opt_ipv6)
		strvec_ધકેલો(&cmd.args, opt_ipv6);
	જો (opt_show_forced_updates > 0)
		strvec_ધકેલો(&cmd.args, "--show-forced-updates");
	નહીંતર જો (opt_show_forced_updates == 0)
		strvec_ધકેલો(&cmd.args, "--no-show-forced-updates");
	જો (set_upstream)
		strvec_ધકેલો(&cmd.args, set_upstream);
	strvec_pushv(&cmd.args, opt_fetch.v);

	જો (repo) {
		strvec_ધકેલો(&cmd.args, repo);
		strvec_pushv(&cmd.args, refspecs);
	} નહીંતર જો (*refspecs)
		BUG("refspecs without repo?");
	cmd.git_cmd = 1;
	cmd.odb_to_close = the_repository->objects;
	પાછા run_command(&cmd);
}

/**
 * "Pulls into void" by branching off merge_head.
 */
સ્થિર પૂર્ણાંક pull_into_void(અચલ સંરચના object_id *merge_head,
		અચલ સંરચના object_id *curr_head)
{
	જો (opt_verify_signatures) {
		સંરચના commit *commit;

		commit = lookup_commit(the_repository, merge_head);
		જો (!commit)
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
	જો (checkout_fast_forward(the_repository,
				  the_hash_algo->empty_વૃક્ષ,
				  merge_head, 0))
		પાછા 1;

	જો (refs_update_ref(get_main_ref_store(the_repository), "initial pull", "HEAD", merge_head, curr_head, 0, UPDATE_REFS_DIE_ON_ERR))
		પાછા 1;

	પાછા 0;
}

સ્થિર પૂર્ણાંક rebase_submodules(શૂન્ય)
{
	સંરચના child_process cp = CHILD_PROCESS_INIT;

	cp.git_cmd = 1;
	cp.no_stdin = 1;
	strvec_pushl(&cp.args, "submodule", "update",
		     "--recursive", "--rebase", ખાલી);
	argv_ધકેલો_verbosity(&cp.args);

	પાછા run_command(&cp);
}

સ્થિર પૂર્ણાંક update_submodules(શૂન્ય)
{
	સંરચના child_process cp = CHILD_PROCESS_INIT;

	cp.git_cmd = 1;
	cp.no_stdin = 1;
	strvec_pushl(&cp.args, "submodule", "update",
		     "--recursive", "--checkout", ખાલી);
	argv_ધકેલો_verbosity(&cp.args);

	પાછા run_command(&cp);
}

/**
 * Runs git-merge, returning its exit status.
 */
સ્થિર પૂર્ણાંક run_merge(શૂન્ય)
{
	સંરચના child_process cmd = CHILD_PROCESS_INIT;

	strvec_pushl(&cmd.args, "merge", ખાલી);

	/* Shared options */
	argv_ધકેલો_verbosity(&cmd.args);
	જો (opt_progress)
		strvec_ધકેલો(&cmd.args, opt_progress);

	/* Options passed to git-merge */
	જો (opt_diffstat)
		strvec_ધકેલો(&cmd.args, opt_diffstat);
	જો (opt_log)
		strvec_ધકેલો(&cmd.args, opt_log);
	જો (opt_signoff)
		strvec_ધકેલો(&cmd.args, opt_signoff);
	જો (opt_squash)
		strvec_ધકેલો(&cmd.args, opt_squash);
	જો (opt_commit)
		strvec_ધકેલો(&cmd.args, opt_commit);
	જો (opt_edit)
		strvec_ધકેલો(&cmd.args, opt_edit);
	જો (cleanup_arg)
		strvec_pushf(&cmd.args, "--cleanup=%s", cleanup_arg);
	જો (opt_ff)
		strvec_ધકેલો(&cmd.args, opt_ff);
	જો (opt_verify)
		strvec_ધકેલો(&cmd.args, opt_verify);
	જો (opt_verify_signatures)
		strvec_ધકેલો(&cmd.args, opt_verify_signatures);
	strvec_pushv(&cmd.args, opt_strategies.v);
	strvec_pushv(&cmd.args, opt_strategy_opts.v);
	જો (opt_gpg_sign)
		strvec_ધકેલો(&cmd.args, opt_gpg_sign);
	જો (opt_autostash == 0)
		strvec_ધકેલો(&cmd.args, "--no-autostash");
	નહીંતર જો (opt_autostash == 1)
		strvec_ધકેલો(&cmd.args, "--autostash");
	જો (opt_allow_unrelated_histories > 0)
		strvec_ધકેલો(&cmd.args, "--allow-unrelated-histories");

	strvec_ધકેલો(&cmd.args, "FETCH_HEAD");
	cmd.git_cmd = 1;
	પાછા run_command(&cmd);
}

/**
 * Returns remote's upstream branch for the current branch. If remote is NULL,
 * the current branch's configured default remote is used. Returns NULL if
 * `remote` does not name a valid remote, HEAD does not point to a branch,
 * remote is not the branch's configured remote or the branch does not have any
 * configured upstream branch.
 */
સ્થિર અચલ અક્ષર *get_upstream_branch(અચલ અક્ષર *remote)
{
	સંરચના remote *rm;
	સંરચના branch *curr_branch;
	અચલ અક્ષર *curr_branch_remote;

	rm = remote_get(remote);
	જો (!rm)
		પાછા ખાલી;

	curr_branch = branch_get("HEAD");
	જો (!curr_branch)
		પાછા ખાલી;

	curr_branch_remote = remote_for_branch(curr_branch, ખાલી);
	પ્રતિજ્ઞા(curr_branch_remote);

	જો (strcmp(curr_branch_remote, rm->name))
		પાછા ખાલી;

	પાછા branch_get_upstream(curr_branch, ખાલી);
}

/**
 * Derives the remote-tracking branch from the remote and refspec.
 *
 * FIXME: The current implementation assumes the default mapping of
 * refs/heads/<branch_name> to refs/remotes/<remote_name>/<branch_name>.
 */
સ્થિર અચલ અક્ષર *get_tracking_branch(અચલ અક્ષર *remote, અચલ અક્ષર *refspec)
{
	સંરચના refspec_item spec;
	અચલ અક્ષર *spec_src;
	અચલ અક્ષર *merge_branch;

	જો (!refspec_item_init_fetch(&spec, refspec, the_hash_algo))
		die(_("invalid refspec '%s'"), refspec);
	spec_src = spec.src;
	જો (!*spec_src || !strcmp(spec_src, "HEAD"))
		spec_src = "HEAD";
	નહીંતર જો (skip_prefix(spec_src, "heads/", &spec_src))
		;
	નહીંતર જો (skip_prefix(spec_src, "refs/heads/", &spec_src))
		;
	નહીંતર જો (starts_with(spec_src, "refs/") ||
		starts_with(spec_src, "tags/") ||
		starts_with(spec_src, "remotes/"))
		spec_src = "";

	જો (*spec_src) {
		જો (!strcmp(remote, "."))
			merge_branch = mkpath("refs/heads/%s", spec_src);
		નહીંતર
			merge_branch = mkpath("refs/remotes/%s/%s", remote, spec_src);
	} નહીંતર
		merge_branch = ખાલી;

	refspec_item_clear(&spec);
	પાછા merge_branch;
}

/**
 * Given the repo and refspecs, sets fork_point to the point at which the
 * current branch forked from its remote-tracking branch. Returns 0 on success,
 * -1 on failure.
 */
સ્થિર પૂર્ણાંક get_rebase_fork_point(સંરચના object_id *fork_point, અચલ અક્ષર *repo,
		અચલ અક્ષર *refspec)
{
	પૂર્ણાંક ret;
	સંરચના branch *curr_branch;
	અચલ અક્ષર *remote_branch;
	સંરચના child_process cp = CHILD_PROCESS_INIT;
	સંરચના strbuf sb = STRBUF_INIT;

	curr_branch = branch_get("HEAD");
	જો (!curr_branch)
		પાછા -1;

	જો (refspec)
		remote_branch = get_tracking_branch(repo, refspec);
	નહીંતર
		remote_branch = get_upstream_branch(repo);

	જો (!remote_branch)
		પાછા -1;

	strvec_pushl(&cp.args, "merge-base", "--fork-point",
		     remote_branch, curr_branch->name, ખાલી);
	cp.no_stdin = 1;
	cp.no_stderr = 1;
	cp.git_cmd = 1;

	ret = capture_command(&cp, &sb, GIT_MAX_HEXSZ);
	જો (ret)
		જાઓ cleanup;

	ret = get_oid_hex(sb.buf, fork_point);
	જો (ret)
		જાઓ cleanup;

cleanup:
	strbuf_release(&sb);
	પાછા ret ? -1 : 0;
}

/**
 * Sets merge_base to the octopus merge base of curr_head, merge_head and
 * fork_point. Returns 0 if a merge base is found, 1 otherwise.
 */
સ્થિર પૂર્ણાંક get_octopus_merge_base(સંરચના object_id *merge_base,
		અચલ સંરચના object_id *curr_head,
		અચલ સંરચના object_id *merge_head,
		અચલ સંરચના object_id *fork_point)
{
	સંરચના commit_list *revs = ખાલી, *result = ખાલી;

	commit_list_ઉમેરો(lookup_commit_reference(the_repository, curr_head),
			   &revs);
	commit_list_ઉમેરો(lookup_commit_reference(the_repository, merge_head),
			   &revs);
	જો (!is_null_oid(fork_point))
		commit_list_ઉમેરો(lookup_commit_reference(the_repository, fork_point),
				   &revs);

	જો (get_octopus_merge_bases(revs, &result) < 0)
		exit(128);
	commit_list_free(revs);
	reduce_heads_replace(&result);

	જો (!result)
		પાછા 1;

	oidcpy(merge_base, &result->item->object.oid);
	commit_list_free(result);
	પાછા 0;
}

/**
 * Given the current HEAD oid, the merge head returned from git-fetch and the
 * fork point calculated by get_rebase_fork_point(), compute the <newbase> and
 * <upstream> arguments to use for the upcoming git-rebase invocation.
 */
સ્થિર પૂર્ણાંક get_rebase_newbase_and_upstream(સંરચના object_id *newbase,
		સંરચના object_id *upstream,
		અચલ સંરચના object_id *curr_head,
		અચલ સંરચના object_id *merge_head,
		અચલ સંરચના object_id *fork_point)
{
	સંરચના object_id oct_merge_base;

	જો (!get_octopus_merge_base(&oct_merge_base, curr_head, merge_head, fork_point))
		જો (!is_null_oid(fork_point) && oideq(&oct_merge_base, fork_point))
			fork_point = ખાલી;

	જો (fork_point && !is_null_oid(fork_point))
		oidcpy(upstream, fork_point);
	નહીંતર
		oidcpy(upstream, merge_head);

	oidcpy(newbase, merge_head);

	પાછા 0;
}

/**
 * Given the <newbase> and <upstream> calculated by
 * get_rebase_newbase_and_upstream(), runs git-rebase with the
 * appropriate arguments and returns its exit status.
 */
સ્થિર પૂર્ણાંક run_rebase(અચલ સંરચના object_id *newbase,
		અચલ સંરચના object_id *upstream)
{
	સંરચના child_process cmd = CHILD_PROCESS_INIT;

	strvec_ધકેલો(&cmd.args, "rebase");

	/* Shared options */
	argv_ધકેલો_verbosity(&cmd.args);

	/* Options passed to git-rebase */
	જો (opt_rebase == REBASE_MERGES)
		strvec_ધકેલો(&cmd.args, "--rebase-merges");
	નહીંતર જો (opt_rebase == REBASE_INTERACTIVE)
		strvec_ધકેલો(&cmd.args, "--interactive");
	જો (opt_diffstat)
		strvec_ધકેલો(&cmd.args, opt_diffstat);
	strvec_pushv(&cmd.args, opt_strategies.v);
	strvec_pushv(&cmd.args, opt_strategy_opts.v);
	જો (opt_gpg_sign)
		strvec_ધકેલો(&cmd.args, opt_gpg_sign);
	જો (opt_signoff)
		strvec_ધકેલો(&cmd.args, opt_signoff);
	જો (opt_autostash == 0)
		strvec_ધકેલો(&cmd.args, "--no-autostash");
	નહીંતર જો (opt_autostash == 1)
		strvec_ધકેલો(&cmd.args, "--autostash");
	જો (opt_verify_signatures &&
	    !strcmp(opt_verify_signatures, "--verify-signatures"))
		warning(_("ignoring --verify-signatures for rebase"));

	strvec_ધકેલો(&cmd.args, "--onto");
	strvec_ધકેલો(&cmd.args, oid_to_hex(newbase));

	strvec_ધકેલો(&cmd.args, oid_to_hex(upstream));

	cmd.git_cmd = 1;
	પાછા run_command(&cmd);
}

સ્થિર પૂર્ણાંક get_can_ff(સંરચના object_id *orig_head,
		      સંરચના oid_array *merge_heads)
{
	પૂર્ણાંક ret;
	સંરચના commit_list *list = ખાલી;
	સંરચના commit *merge_head, *head;
	સંરચના object_id *orig_merge_head;

	જો (merge_heads->nr > 1)
		પાછા 0;

	orig_merge_head = &merge_heads->oid[0];
	head = lookup_commit_reference(the_repository, orig_head);
	જો (!head)
		પાછા 0;
	merge_head = lookup_commit_reference(the_repository, orig_merge_head);
	જો (!merge_head)
		પાછા 0;
	commit_list_ઉમેરો(head, &list);
	ret = repo_is_descendant_of(the_repository, merge_head, list);
	commit_list_free(list);
	જો (ret < 0)
		exit(128);
	પાછા ret;
}

/*
 * Is orig_head a descendant of _all_ merge_heads?
 * Unfortunately is_descendant_of() cannot be used as it asks
 * if orig_head is a descendant of at least one of them.
 */
સ્થિર પૂર્ણાંક already_up_to_date(સંરચના object_id *orig_head,
			      સંરચના oid_array *merge_heads)
{
	સંરચના commit *ours;

	ours = lookup_commit_reference(the_repository, orig_head);
	જો (!ours)
		પાછા 0;
	માટે (size_t i = 0; i < merge_heads->nr; i++) {
		સંરચના commit_list *list = ખાલી;
		સંરચના commit *theirs;
		પૂર્ણાંક ok;

		theirs = lookup_commit_reference(the_repository, &merge_heads->oid[i]);
		જો (!theirs)
			પાછા 0;
		commit_list_ઉમેરો(theirs, &list);
		ok = repo_is_descendant_of(the_repository, ours, list);
		commit_list_free(list);
		જો (ok < 0)
			exit(128);
		જો (!ok)
			પાછા 0;
	}
	પાછા 1;
}

સ્થિર શૂન્ય show_advice_pull_non_ff(શૂન્ય)
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

પૂર્ણાંક cmd_pull(પૂર્ણાંક argc,
	     અચલ અક્ષર **argv,
	     અચલ અક્ષર *prefix,
	     સંરચના repository *repository UNUSED)
{
	અચલ અક્ષર *repo, **refspecs;
	સંરચના oid_array merge_heads = OID_ARRAY_INIT;
	સંરચના object_id orig_head, curr_head;
	સંરચના object_id rebase_fork_point;
	પૂર્ણાંક rebase_unspecified = 0;
	પૂર્ણાંક can_ff;
	પૂર્ણાંક divergent;
	પૂર્ણાંક ret;
	સ્થિર સંરચના option pull_options[] = {
		/* Shared options */
		OPT__VERBOSITY(&opt_verbosity),
		OPT_PASSTHRU(0, "progress", &opt_progress, ખાલી,
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
		OPT_PASSTHRU('n', ખાલી, &opt_diffstat, ખાલી,
			N_("do not show a diffstat at the end of the merge"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG),
		OPT_PASSTHRU(0, "stat", &opt_diffstat, ખાલી,
			N_("show a diffstat at the end of the merge"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "summary", &opt_diffstat, ખાલી,
			N_("(synonym to --stat)"),
			PARSE_OPT_NOARG | PARSE_OPT_HIDDEN),
		OPT_PASSTHRU(0, "compact-summary", &opt_diffstat, ખાલી,
			N_("show a compact-summary at the end of the merge"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "log", &opt_log, N_("n"),
			N_("add (at most <n>) entries from shortlog to merge commit message"),
			PARSE_OPT_OPTARG),
		OPT_PASSTHRU(0, "signoff", &opt_signoff, ખાલી,
			N_("add a Signed-off-by trailer"),
			PARSE_OPT_OPTARG),
		OPT_PASSTHRU(0, "squash", &opt_squash, ખાલી,
			N_("create a single commit instead of doing a merge"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "commit", &opt_commit, ખાલી,
			N_("perform a commit if the merge succeeds (default)"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "edit", &opt_edit, ખાલી,
			N_("edit message before committing"),
			PARSE_OPT_NOARG),
		OPT_CLEANUP(&cleanup_arg),
		OPT_PASSTHRU(0, "ff", &opt_ff, ખાલી,
			N_("allow fast-forward"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "ff-only", &opt_ff, ખાલી,
			N_("abort if fast-forward is not possible"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG),
		OPT_PASSTHRU(0, "verify", &opt_verify, ખાલી,
			N_("control use of pre-merge-commit and commit-msg hooks"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "verify-signatures", &opt_verify_signatures, ખાલી,
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
		OPT_PASSTHRU(0, "all", &opt_all, ખાલી,
			N_("fetch from all remotes"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU('a', "append", &opt_append, ખાલી,
			N_("append to .git/FETCH_HEAD instead of overwriting"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "upload-pack", &opt_upload_pack, N_("path"),
			N_("path to upload pack on remote end"),
			0),
		OPT__FORCE(&opt_force, N_("force overwrite of local branch"), 0),
		OPT_PASSTHRU('t', "tags", &opt_tags, ખાલી,
			N_("fetch all tags and associated objects"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU('p', "prune", &opt_prune, ખાલી,
			N_("prune remote-tracking branches no longer on remote"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU('j', "jobs", &max_children, N_("n"),
			N_("number of submodules pulled in parallel"),
			PARSE_OPT_OPTARG),
		OPT_BOOL(0, "dry-run", &opt_dry_run,
			N_("dry run")),
		OPT_PASSTHRU('k', "keep", &opt_keep, ખાલી,
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
		OPT_PASSTHRU(0, "unshallow", &opt_unshallow, ખાલી,
			N_("convert to a complete repository"),
			PARSE_OPT_NONEG | PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "update-shallow", &opt_update_shallow, ખાલી,
			N_("accept refs that update .git/shallow"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "refmap", &opt_refmap, N_("refmap"),
			N_("specify fetch refmap"),
			PARSE_OPT_NONEG),
		OPT_PASSTHRU_ARGV('o', "server-option", &opt_fetch,
			N_("server-specific"),
			N_("option to transmit"),
			0),
		OPT_PASSTHRU('4',  "ipv4", &opt_ipv4, ખાલી,
			N_("use IPv4 addresses only"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU('6',  "ipv6", &opt_ipv6, ખાલી,
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
		OPT_PASSTHRU(0, "set-upstream", &set_upstream, ખાલી,
			N_("set upstream for git pull/fetch"),
			PARSE_OPT_NOARG),

		OPT_END()
	};

	જો (!getenv("GIT_REFLOG_ACTION"))
		set_reflog_message(argc, argv);

	repo_config(the_repository, git_pull_config, ખાલી);
	જો (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_full_index = 0;
	}

	argc = parse_options(argc, argv, prefix, pull_options, pull_usage, 0);
	જો (opt_autostash == -1)
		opt_autostash = config_pull_autostash;

	જો (recurse_submodules_cli != RECURSE_SUBMODULES_DEFAULT)
		recurse_submodules = recurse_submodules_cli;

	જો (cleanup_arg)
		/*
		 * this only checks the validity of cleanup_arg; we don't need
		 * a valid value for use_editor
		 */
		get_cleanup_mode(cleanup_arg, 0);

	parse_repo_refspecs(argc, argv, &repo, &refspecs);

	જો (!opt_ff) {
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
		જો (opt_rebase >= 0 && opt_ff && !strcmp(opt_ff, "--ff-only")) {
			મુક્ત(opt_ff);
			opt_ff = xstrdup("--ff");
		}
	}

	જો (opt_rebase < 0)
		opt_rebase = config_get_rebase(&rebase_unspecified);

	જો (repo_read_index_unmerged(the_repository))
		die_resolve_conflict("pull");

	જો (file_exists(git_path_merge_head(the_repository)))
		die_conclude_merge();

	જો (repo_get_oid(the_repository, "HEAD", &orig_head))
		oidclr(&orig_head, the_repository->hash_algo);

	જો (opt_rebase) {
		જો (opt_autostash == -1)
			opt_autostash = config_rebase_autostash;

		જો (is_null_oid(&orig_head) && !is_index_unborn(the_repository->index))
			die(_("Updating an unborn branch with changes added to the index."));

		જો (!opt_autostash)
			require_clean_work_વૃક્ષ(the_repository,
				N_("pull with rebase"),
				_("Please commit or stash them."), 1, 0);

		જો (get_rebase_fork_point(&rebase_fork_point, repo, *refspecs))
			oidclr(&rebase_fork_point, the_repository->hash_algo);
	}

	જો (run_fetch(repo, refspecs))
		પાછા 1;

	જો (opt_dry_run)
		પાછા 0;

	જો (repo_get_oid(the_repository, "HEAD", &curr_head))
		oidclr(&curr_head, the_repository->hash_algo);

	જો (!is_null_oid(&orig_head) && !is_null_oid(&curr_head) &&
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

		જો (checkout_fast_forward(the_repository, &orig_head,
					  &curr_head, 0))
			die(_("Cannot fast-forward your working tree.\n"
				"After making sure that you saved anything precious from\n"
				"$ git diff %s\n"
				"output, run\n"
				"$ git reset --hard\n"
				"to recover."), oid_to_hex(&orig_head));
	}

	get_merge_heads(&merge_heads);

	જો (!merge_heads.nr)
		die_no_merge_candidates(repo, refspecs);

	જો (is_null_oid(&orig_head)) {
		જો (merge_heads.nr > 1)
			die(_("Cannot merge multiple branches into empty head."));
		ret = pull_into_void(merge_heads.oid, &curr_head);
		જાઓ cleanup;
	}
	જો (merge_heads.nr > 1) {
		જો (opt_rebase)
			die(_("Cannot rebase onto multiple branches."));
		જો (opt_ff && !strcmp(opt_ff, "--ff-only"))
			die(_("Cannot fast-forward to multiple branches."));
	}

	can_ff = get_can_ff(&orig_head, &merge_heads);
	divergent = !can_ff && !already_up_to_date(&orig_head, &merge_heads);

	/* ff-only takes precedence over rebase */
	જો (opt_ff && !strcmp(opt_ff, "--ff-only")) {
		જો (divergent)
			die_ff_impossible();
		opt_rebase = REBASE_FALSE;
	}
	/* If no action specified and we can't fast forward, then warn. */
	જો (!opt_ff && rebase_unspecified && divergent) {
		show_advice_pull_non_ff();
		die(_("Need to specify how to reconcile divergent branches."));
	}

	જો (opt_rebase) {
		સંરચના object_id newbase;
		સંરચના object_id upstream;
		get_rebase_newbase_and_upstream(&newbase, &upstream, &curr_head,
						merge_heads.oid, &rebase_fork_point);

		જો ((recurse_submodules == RECURSE_SUBMODULES_ON ||
		     recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND) &&
		    submodule_touches_in_range(the_repository, &upstream, &curr_head))
			die(_("cannot rebase with locally recorded submodule modifications"));

		જો (can_ff) {
			/* we can fast-forward this without invoking rebase */
			મુક્ત(opt_ff);
			opt_ff = xstrdup("--ff-only");
			ret = run_merge();
		} નહીંતર {
			ret = run_rebase(&newbase, &upstream);
		}

		જો (!ret && (recurse_submodules == RECURSE_SUBMODULES_ON ||
			     recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND))
			ret = rebase_submodules();

		જાઓ cleanup;
	} નહીંતર {
		ret = run_merge();
		જો (!ret && (recurse_submodules == RECURSE_SUBMODULES_ON ||
			     recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND))
			ret = update_submodules();
		જાઓ cleanup;
	}

cleanup:
	oid_array_clear(&merge_heads);
	પાછા ret;
}
