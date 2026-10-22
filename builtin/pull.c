/*
 * Builtin "git pull"
 *
 * Based on git-pull.sh by Junio C Hamano
 *
 * Fetch one or more remote refs and merge it/them into the current HEAD.
 */

#define USE_THE_REPOSITORY_VARIABLE

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "advice.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "merge.h"
#अन्तर्भाव "object-name.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "run-command.h"
#अन्तर्भाव "oid-array.h"
#अन्तर्भाव "remote.h"
#अन्तर्भाव "dir.h"
#अन्तर्भाव "path.h"
#अन्तर्भाव "read-cache-ll.h"
#अन्तर्भाव "rebase.h"
#अन्तर्भाव "refs.h"
#अन्तर्भाव "refspec.h"
#अन्तर्भाव "submodule.h"
#अन्तर्भाव "submodule-config.h"
#अन्तर्भाव "wt-status.h"
#अन्तर्भाव "commit-reach.h"
#अन्तर्भाव "sequencer.h"

/**
 * Parses the value of --rebase. If value is a false value, returns
 * REBASE_FALSE. If value is a true value, returns REBASE_TRUE. If value is
 * "merges", returns REBASE_MERGES. If value is a invalid value, dies with
 * a fatal error if fatal is true, otherwise returns REBASE_INVALID.
 */
स्थिर गणना rebase_type parse_config_rebase(स्थिरांक अक्षर *किल्ली, स्थिरांक अक्षर *मूल्य,
		पूर्णांक fatal)
{
	गणना rebase_type v = rebase_parse_मूल्य(मूल्य);
	जर (v != REBASE_INVALID)
		परत v;

	जर (fatal)
		die(_("invalid value for '%s': '%s'"), किल्ली, मूल्य);
	अन्यथा
		error(_("invalid value for '%s': '%s'"), किल्ली, मूल्य);

	परत REBASE_INVALID;
}

/**
 * Callback for --rebase, which parses arg with parse_config_rebase().
 */
स्थिर पूर्णांक parse_opt_rebase(स्थिरांक संरचना option *opt, स्थिरांक अक्षर *arg, पूर्णांक unset)
{
	गणना rebase_type *मूल्य = opt->मूल्य;

	जर (arg)
		*मूल्य = parse_config_rebase("--rebase", arg, 0);
	अन्यथा
		*मूल्य = unset ? REBASE_FALSE : REBASE_TRUE;
	परत *मूल्य == REBASE_INVALID ? -1 : 0;
}

स्थिर स्थिरांक अक्षर * स्थिरांक pull_usage[] = {
	N_("git pull [<options>] [<repository> [<refspec>...]]"),
	रिक्त
};

/* Shared options */
स्थिर पूर्णांक opt_verbosity;
स्थिर स्थिरांक अक्षर *opt_progress;
स्थिर पूर्णांक recurse_submodules = RECURSE_SUBMODULES_DEFAULT;
स्थिर पूर्णांक recurse_submodules_cli = RECURSE_SUBMODULES_DEFAULT;

/* Options passed to git-merge or git-rebase */
स्थिर गणना rebase_type opt_rebase = -1;
स्थिर स्थिरांक अक्षर *opt_diffstat;
स्थिर स्थिरांक अक्षर *opt_log;
स्थिर स्थिरांक अक्षर *opt_signoff;
स्थिर स्थिरांक अक्षर *opt_squash;
स्थिर स्थिरांक अक्षर *opt_commit;
स्थिर स्थिरांक अक्षर *opt_edit;
स्थिर स्थिरांक अक्षर *cleanup_arg;
स्थिर अक्षर *opt_ff;
स्थिर स्थिरांक अक्षर *opt_verify_signatures;
स्थिर स्थिरांक अक्षर *opt_verify;
स्थिर पूर्णांक opt_autostash = -1;
स्थिर पूर्णांक config_rebase_autostash;
स्थिर पूर्णांक config_pull_autostash = -1;
स्थिर पूर्णांक check_trust_level = 1;
स्थिर संरचना strvec opt_strategies = STRVEC_INIT;
स्थिर संरचना strvec opt_strategy_opts = STRVEC_INIT;
स्थिर स्थिरांक अक्षर *opt_gpg_sign;
स्थिर पूर्णांक opt_allow_unrelated_histories;

/* Options passed to git-fetch */
स्थिर स्थिरांक अक्षर *opt_all;
स्थिर स्थिरांक अक्षर *opt_append;
स्थिर स्थिरांक अक्षर *opt_upload_pack;
स्थिर पूर्णांक opt_force;
स्थिर स्थिरांक अक्षर *opt_tags;
स्थिर स्थिरांक अक्षर *opt_prune;
स्थिर स्थिरांक अक्षर *max_children;
स्थिर पूर्णांक opt_dry_run;
स्थिर स्थिरांक अक्षर *opt_keep;
स्थिर स्थिरांक अक्षर *opt_depth;
स्थिर स्थिरांक अक्षर *opt_unshallow;
स्थिर स्थिरांक अक्षर *opt_update_shallow;
स्थिर स्थिरांक अक्षर *opt_refmap;
स्थिर स्थिरांक अक्षर *opt_ipv4;
स्थिर स्थिरांक अक्षर *opt_ipv6;
स्थिर पूर्णांक opt_show_forced_updates = -1;
स्थिर स्थिरांक अक्षर *set_upstream;
स्थिर संरचना strvec opt_fetch = STRVEC_INIT;

/**
 * Pushes "-q" or "-v" switches into arr to match the opt_verbosity level.
 */
स्थिर शून्य argv_ढकला_verbosity(संरचना strvec *arr)
{
	पूर्णांक verbosity;

	साठी (verbosity = opt_verbosity; verbosity > 0; verbosity--)
		strvec_ढकला(arr, "-v");

	साठी (verbosity = opt_verbosity; verbosity < 0; verbosity++)
		strvec_ढकला(arr, "-q");
}

/**
 * Pushes "-f" switches into arr to match the opt_force level.
 */
स्थिर शून्य argv_ढकला_force(संरचना strvec *arr)
{
	पूर्णांक force = opt_force;
	पर्यंत (force-- > 0)
		strvec_ढकला(arr, "-f");
}

/**
 * Sets the GIT_REFLOG_ACTION environment variable to the concatenation of argv
 */
स्थिर शून्य set_reflog_message(पूर्णांक argc, स्थिरांक अक्षर **argv)
{
	पूर्णांक i;
	संरचना strbuf संदेश = STRBUF_INIT;

	साठी (i = 0; i < argc; i++) {
		जर (i)
			strbuf_addch(&संदेश, ' ');
		strbuf_addstr(&संदेश, argv[i]);
	}

	setenv("GIT_REFLOG_ACTION", संदेश.buf, 0);

	strbuf_release(&संदेश);
}

/**
 * If pull.ff is unset, returns NULL. If pull.ff is "true", returns "--ff". If
 * pull.ff is "false", returns "--no-ff". If pull.ff is "only", returns
 * "--ff-only". Otherwise, if pull.ff is set to an invalid value, die with an
 * error.
 */
स्थिर स्थिरांक अक्षर *config_get_ff(शून्य)
{
	स्थिरांक अक्षर *मूल्य;

	जर (repo_config_get_मूल्य(the_repository, "pull.ff", &मूल्य))
		परत रिक्त;

	निवड (git_parse_maybe_bool(मूल्य)) {
	स्थिती 0:
		परत "--no-ff";
	स्थिती 1:
		परत "--ff";
	}

	जर (!म_भेद(मूल्य, "only"))
		परत "--ff-only";

	die(_("invalid value for '%s': '%s'"), "pull.ff", मूल्य);
}

/**
 * Returns the default configured value for --rebase. It first looks for the
 * value of "branch.$curr_branch.rebase", where $curr_branch is the current
 * branch, and if HEAD is detached or the configuration key does not exist,
 * looks for the value of "pull.rebase". If both configuration keys do not
 * exist, returns REBASE_FALSE.
 */
स्थिर गणना rebase_type config_get_rebase(पूर्णांक *rebase_unspecified)
{
	संरचना branch *curr_branch = branch_get("HEAD");
	स्थिरांक अक्षर *मूल्य;

	जर (curr_branch) {
		अक्षर *किल्ली = xstrfmt("branch.%s.rebase", curr_branch->name);

		जर (!repo_config_get_मूल्य(the_repository, किल्ली, &मूल्य)) {
			गणना rebase_type ret = parse_config_rebase(किल्ली, मूल्य, 1);
			मुक्त(किल्ली);
			परत ret;
		}

		मुक्त(किल्ली);
	}

	जर (!repo_config_get_मूल्य(the_repository, "pull.rebase", &मूल्य))
		परत parse_config_rebase("pull.rebase", मूल्य, 1);

	*rebase_unspecified = 1;

	परत REBASE_FALSE;
}

/**
 * Read config variables.
 */
स्थिर पूर्णांक git_pull_config(स्थिरांक अक्षर *var, स्थिरांक अक्षर *मूल्य,
			   स्थिरांक संरचना config_context *ctx, शून्य *cb)
{
	जर (!म_भेद(var, "rebase.autostash")) {
		/*
		 * run_rebase() also reads this option. The reason we handle it here is
		 * that when pull.rebase is true, a fast-forward may occur without
		 * invoking run_rebase(). We need to ensure that autostash is set even
		 * in the fast-forward case.
		 *
		 * run_merge() handles merge.autostash, so we don't handle it here.
		 */
		config_rebase_autostash = git_config_bool(var, मूल्य);
		परत 0;
	} अन्यथा जर (!म_भेद(var, "pull.autostash")) {
		config_pull_autostash = git_config_bool(var, मूल्य);
		परत 0;
	} अन्यथा जर (!म_भेद(var, "submodule.recurse")) {
		recurse_submodules = git_config_bool(var, मूल्य) ?
			RECURSE_SUBMODULES_ON : RECURSE_SUBMODULES_OFF;
		परत 0;
	} अन्यथा जर (!म_भेद(var, "gpg.mintrustlevel")) {
		check_trust_level = 0;
	}

	परत git_default_config(var, मूल्य, ctx, cb);
}

/**
 * Appends merge candidates from FETCH_HEAD that are not marked not-for-merge
 * into merge_heads.
 */
स्थिर शून्य get_merge_heads(संरचना oid_array *merge_heads)
{
	स्थिरांक अक्षर *filename = git_path_fetch_head(the_repository);
	खाता *fp;
	संरचना strbuf sb = STRBUF_INIT;
	संरचना object_id oid;

	fp = xfopen(filename, "r");
	पर्यंत (strbuf_getline_lf(&sb, fp) != खातापूर्ण) {
		स्थिरांक अक्षर *p;
		जर (parse_oid_hex(sb.buf, &oid, &p))
			सुरूठेवा;  /* invalid line: does not start with object ID */
		जर (starts_with(p, "\tnot-for-merge\t"))
			सुरूठेवा;  /* ref is not-for-merge */
		oid_array_append(merge_heads, &oid);
	}
	ख_बंद(fp);
	strbuf_release(&sb);
}

/**
 * Used by die_no_merge_candidates() as a for_each_remote() callback to
 * retrieve the name of the remote if the repository only has one remote.
 */
स्थिर पूर्णांक get_only_remote(संरचना remote *remote, शून्य *cb_data)
{
	स्थिरांक अक्षर **remote_name = cb_data;

	जर (*remote_name)
		परत -1;

	*remote_name = remote->name;
	परत 0;
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
स्थिर शून्य NORETURN die_no_merge_candidates(स्थिरांक अक्षर *repo, स्थिरांक अक्षर **refspecs)
{
	संरचना branch *curr_branch = branch_get("HEAD");
	स्थिरांक अक्षर *remote = curr_branch ? curr_branch->remote_name : रिक्त;

	जर (*refspecs) {
		जर (opt_rebase)
			fprintf_ln(मानक_त्रुटि, _("There is no candidate for rebasing against among the refs that you just fetched."));
		अन्यथा
			fprintf_ln(मानक_त्रुटि, _("There are no candidates for merging among the refs that you just fetched."));
		fprintf_ln(मानक_त्रुटि, _("Generally this means that you provided a wildcard refspec which had no\n"
					"matches on the remote end."));
	} अन्यथा जर (repo && curr_branch && (!remote || म_भेद(repo, remote))) {
		fprintf_ln(मानक_त्रुटि, _("You asked to pull from the remote '%s', but did not specify\n"
			"a branch. Because this is not the default configured remote\n"
			"for your current branch, you must specify a branch on the command line."),
			repo);
	} अन्यथा जर (!curr_branch) {
		fprintf_ln(मानक_त्रुटि, _("You are not currently on a branch."));
		जर (opt_rebase)
			fprintf_ln(मानक_त्रुटि, _("Please specify which branch you want to rebase against."));
		अन्यथा
			fprintf_ln(मानक_त्रुटि, _("Please specify which branch you want to merge with."));
		fprintf_ln(मानक_त्रुटि, _("See git-pull(1) for details."));
		ख_लिखो(मानक_त्रुटि, "\n");
		fprintf_ln(मानक_त्रुटि, "    git pull %s %s", _("<remote>"), _("<branch>"));
		ख_लिखो(मानक_त्रुटि, "\n");
	} अन्यथा जर (!curr_branch->merge_nr) {
		स्थिरांक अक्षर *remote_name = रिक्त;

		जर (for_each_remote(get_only_remote, &remote_name) || !remote_name)
			remote_name = _("<remote>");

		fprintf_ln(मानक_त्रुटि, _("There is no tracking information for the current branch."));
		जर (opt_rebase)
			fprintf_ln(मानक_त्रुटि, _("Please specify which branch you want to rebase against."));
		अन्यथा
			fprintf_ln(मानक_त्रुटि, _("Please specify which branch you want to merge with."));
		fprintf_ln(मानक_त्रुटि, _("See git-pull(1) for details."));
		ख_लिखो(मानक_त्रुटि, "\n");
		fprintf_ln(मानक_त्रुटि, "    git pull %s %s", _("<remote>"), _("<branch>"));
		ख_लिखो(मानक_त्रुटि, "\n");
		fprintf_ln(मानक_त्रुटि, _("If you wish to set tracking information for this branch you can do so with:"));
		ख_लिखो(मानक_त्रुटि, "\n");
		fprintf_ln(मानक_त्रुटि, "    git branch --set-upstream-to=%s/%s %s\n",
				remote_name, _("<branch>"), curr_branch->name);
	} अन्यथा
		fprintf_ln(मानक_त्रुटि, _("Your configuration specifies to merge with the ref '%s'\n"
			"from the remote, but no such ref was fetched."),
			curr_branch->merge[0]->src);
	निकास(1);
}

/**
 * Parses argv into [<repo> [<refspecs>...]], returning their values in `repo`
 * as a string and `refspecs` as a null-terminated array of strings. If `repo`
 * is not provided in argv, it is set to NULL.
 */
स्थिर शून्य parse_repo_refspecs(पूर्णांक argc, स्थिरांक अक्षर **argv, स्थिरांक अक्षर **repo,
		स्थिरांक अक्षर ***refspecs)
{
	जर (argc > 0) {
		*repo = *argv++;
		argc--;
	} अन्यथा
		*repo = रिक्त;
	*refspecs = argv;
}

/**
 * Runs git-fetch, returning its exit status. `repo` and `refspecs` are the
 * repository and refspecs to fetch, or NULL if they are not provided.
 */
स्थिर पूर्णांक run_fetch(स्थिरांक अक्षर *repo, स्थिरांक अक्षर **refspecs)
{
	संरचना child_process cmd = CHILD_PROCESS_INIT;

	strvec_pushl(&cmd.args, "fetch", "--update-head-ok", रिक्त);

	/* Shared options */
	argv_ढकला_verbosity(&cmd.args);
	जर (opt_progress)
		strvec_ढकला(&cmd.args, opt_progress);

	/* Options passed to git-fetch */
	जर (opt_all)
		strvec_ढकला(&cmd.args, opt_all);
	जर (opt_append)
		strvec_ढकला(&cmd.args, opt_append);
	जर (opt_upload_pack)
		strvec_ढकला(&cmd.args, opt_upload_pack);
	argv_ढकला_force(&cmd.args);
	जर (opt_tags)
		strvec_ढकला(&cmd.args, opt_tags);
	जर (opt_prune)
		strvec_ढकला(&cmd.args, opt_prune);
	जर (recurse_submodules_cli != RECURSE_SUBMODULES_DEFAULT)
		निवड (recurse_submodules_cli) {
		स्थिती RECURSE_SUBMODULES_ON:
			strvec_ढकला(&cmd.args, "--recurse-submodules=on");
			थांबा;
		स्थिती RECURSE_SUBMODULES_OFF:
			strvec_ढकला(&cmd.args, "--recurse-submodules=no");
			थांबा;
		स्थिती RECURSE_SUBMODULES_ON_DEMAND:
			strvec_ढकला(&cmd.args, "--recurse-submodules=on-demand");
			थांबा;
		पूर्वनिर्धारित:
			BUG("submodule recursion option not understood");
		}
	जर (max_children)
		strvec_ढकला(&cmd.args, max_children);
	जर (opt_dry_run)
		strvec_ढकला(&cmd.args, "--dry-run");
	जर (opt_keep)
		strvec_ढकला(&cmd.args, opt_keep);
	जर (opt_depth)
		strvec_ढकला(&cmd.args, opt_depth);
	जर (opt_unshallow)
		strvec_ढकला(&cmd.args, opt_unshallow);
	जर (opt_update_shallow)
		strvec_ढकला(&cmd.args, opt_update_shallow);
	जर (opt_refmap)
		strvec_ढकला(&cmd.args, opt_refmap);
	जर (opt_ipv4)
		strvec_ढकला(&cmd.args, opt_ipv4);
	जर (opt_ipv6)
		strvec_ढकला(&cmd.args, opt_ipv6);
	जर (opt_show_forced_updates > 0)
		strvec_ढकला(&cmd.args, "--show-forced-updates");
	अन्यथा जर (opt_show_forced_updates == 0)
		strvec_ढकला(&cmd.args, "--no-show-forced-updates");
	जर (set_upstream)
		strvec_ढकला(&cmd.args, set_upstream);
	strvec_pushv(&cmd.args, opt_fetch.v);

	जर (repo) {
		strvec_ढकला(&cmd.args, repo);
		strvec_pushv(&cmd.args, refspecs);
	} अन्यथा जर (*refspecs)
		BUG("refspecs without repo?");
	cmd.git_cmd = 1;
	cmd.odb_to_close = the_repository->objects;
	परत run_command(&cmd);
}

/**
 * "Pulls into void" by branching off merge_head.
 */
स्थिर पूर्णांक pull_into_void(स्थिरांक संरचना object_id *merge_head,
		स्थिरांक संरचना object_id *curr_head)
{
	जर (opt_verify_signatures) {
		संरचना commit *commit;

		commit = lookup_commit(the_repository, merge_head);
		जर (!commit)
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
	जर (checkout_fast_forward(the_repository,
				  the_hash_algo->empty_वृक्ष,
				  merge_head, 0))
		परत 1;

	जर (refs_update_ref(get_main_ref_store(the_repository), "initial pull", "HEAD", merge_head, curr_head, 0, UPDATE_REFS_DIE_ON_ERR))
		परत 1;

	परत 0;
}

स्थिर पूर्णांक rebase_submodules(शून्य)
{
	संरचना child_process cp = CHILD_PROCESS_INIT;

	cp.git_cmd = 1;
	cp.no_stdin = 1;
	strvec_pushl(&cp.args, "submodule", "update",
		     "--recursive", "--rebase", रिक्त);
	argv_ढकला_verbosity(&cp.args);

	परत run_command(&cp);
}

स्थिर पूर्णांक update_submodules(शून्य)
{
	संरचना child_process cp = CHILD_PROCESS_INIT;

	cp.git_cmd = 1;
	cp.no_stdin = 1;
	strvec_pushl(&cp.args, "submodule", "update",
		     "--recursive", "--checkout", रिक्त);
	argv_ढकला_verbosity(&cp.args);

	परत run_command(&cp);
}

/**
 * Runs git-merge, returning its exit status.
 */
स्थिर पूर्णांक run_merge(शून्य)
{
	संरचना child_process cmd = CHILD_PROCESS_INIT;

	strvec_pushl(&cmd.args, "merge", रिक्त);

	/* Shared options */
	argv_ढकला_verbosity(&cmd.args);
	जर (opt_progress)
		strvec_ढकला(&cmd.args, opt_progress);

	/* Options passed to git-merge */
	जर (opt_diffstat)
		strvec_ढकला(&cmd.args, opt_diffstat);
	जर (opt_log)
		strvec_ढकला(&cmd.args, opt_log);
	जर (opt_signoff)
		strvec_ढकला(&cmd.args, opt_signoff);
	जर (opt_squash)
		strvec_ढकला(&cmd.args, opt_squash);
	जर (opt_commit)
		strvec_ढकला(&cmd.args, opt_commit);
	जर (opt_edit)
		strvec_ढकला(&cmd.args, opt_edit);
	जर (cleanup_arg)
		strvec_pushf(&cmd.args, "--cleanup=%s", cleanup_arg);
	जर (opt_ff)
		strvec_ढकला(&cmd.args, opt_ff);
	जर (opt_verify)
		strvec_ढकला(&cmd.args, opt_verify);
	जर (opt_verify_signatures)
		strvec_ढकला(&cmd.args, opt_verify_signatures);
	strvec_pushv(&cmd.args, opt_strategies.v);
	strvec_pushv(&cmd.args, opt_strategy_opts.v);
	जर (opt_gpg_sign)
		strvec_ढकला(&cmd.args, opt_gpg_sign);
	जर (opt_autostash == 0)
		strvec_ढकला(&cmd.args, "--no-autostash");
	अन्यथा जर (opt_autostash == 1)
		strvec_ढकला(&cmd.args, "--autostash");
	जर (opt_allow_unrelated_histories > 0)
		strvec_ढकला(&cmd.args, "--allow-unrelated-histories");

	strvec_ढकला(&cmd.args, "FETCH_HEAD");
	cmd.git_cmd = 1;
	परत run_command(&cmd);
}

/**
 * Returns remote's upstream branch for the current branch. If remote is NULL,
 * the current branch's configured default remote is used. Returns NULL if
 * `remote` does not name a valid remote, HEAD does not point to a branch,
 * remote is not the branch's configured remote or the branch does not have any
 * configured upstream branch.
 */
स्थिर स्थिरांक अक्षर *get_upstream_branch(स्थिरांक अक्षर *remote)
{
	संरचना remote *rm;
	संरचना branch *curr_branch;
	स्थिरांक अक्षर *curr_branch_remote;

	rm = remote_get(remote);
	जर (!rm)
		परत रिक्त;

	curr_branch = branch_get("HEAD");
	जर (!curr_branch)
		परत रिक्त;

	curr_branch_remote = remote_for_branch(curr_branch, रिक्त);
	प्रतिज्ञा(curr_branch_remote);

	जर (म_भेद(curr_branch_remote, rm->name))
		परत रिक्त;

	परत branch_get_upstream(curr_branch, रिक्त);
}

/**
 * Derives the remote-tracking branch from the remote and refspec.
 *
 * FIXME: The current implementation assumes the default mapping of
 * refs/heads/<branch_name> to refs/remotes/<remote_name>/<branch_name>.
 */
स्थिर स्थिरांक अक्षर *get_tracking_branch(स्थिरांक अक्षर *remote, स्थिरांक अक्षर *refspec)
{
	संरचना refspec_item spec;
	स्थिरांक अक्षर *spec_src;
	स्थिरांक अक्षर *merge_branch;

	जर (!refspec_item_init_fetch(&spec, refspec, the_hash_algo))
		die(_("invalid refspec '%s'"), refspec);
	spec_src = spec.src;
	जर (!*spec_src || !म_भेद(spec_src, "HEAD"))
		spec_src = "HEAD";
	अन्यथा जर (skip_prefix(spec_src, "heads/", &spec_src))
		;
	अन्यथा जर (skip_prefix(spec_src, "refs/heads/", &spec_src))
		;
	अन्यथा जर (starts_with(spec_src, "refs/") ||
		starts_with(spec_src, "tags/") ||
		starts_with(spec_src, "remotes/"))
		spec_src = "";

	जर (*spec_src) {
		जर (!म_भेद(remote, "."))
			merge_branch = mkpath("refs/heads/%s", spec_src);
		अन्यथा
			merge_branch = mkpath("refs/remotes/%s/%s", remote, spec_src);
	} अन्यथा
		merge_branch = रिक्त;

	refspec_item_clear(&spec);
	परत merge_branch;
}

/**
 * Given the repo and refspecs, sets fork_point to the point at which the
 * current branch forked from its remote-tracking branch. Returns 0 on success,
 * -1 on failure.
 */
स्थिर पूर्णांक get_rebase_fork_point(संरचना object_id *fork_point, स्थिरांक अक्षर *repo,
		स्थिरांक अक्षर *refspec)
{
	पूर्णांक ret;
	संरचना branch *curr_branch;
	स्थिरांक अक्षर *remote_branch;
	संरचना child_process cp = CHILD_PROCESS_INIT;
	संरचना strbuf sb = STRBUF_INIT;

	curr_branch = branch_get("HEAD");
	जर (!curr_branch)
		परत -1;

	जर (refspec)
		remote_branch = get_tracking_branch(repo, refspec);
	अन्यथा
		remote_branch = get_upstream_branch(repo);

	जर (!remote_branch)
		परत -1;

	strvec_pushl(&cp.args, "merge-base", "--fork-point",
		     remote_branch, curr_branch->name, रिक्त);
	cp.no_stdin = 1;
	cp.no_stderr = 1;
	cp.git_cmd = 1;

	ret = capture_command(&cp, &sb, GIT_MAX_HEXSZ);
	जर (ret)
		जा cleanup;

	ret = get_oid_hex(sb.buf, fork_point);
	जर (ret)
		जा cleanup;

cleanup:
	strbuf_release(&sb);
	परत ret ? -1 : 0;
}

/**
 * Sets merge_base to the octopus merge base of curr_head, merge_head and
 * fork_point. Returns 0 if a merge base is found, 1 otherwise.
 */
स्थिर पूर्णांक get_octopus_merge_base(संरचना object_id *merge_base,
		स्थिरांक संरचना object_id *curr_head,
		स्थिरांक संरचना object_id *merge_head,
		स्थिरांक संरचना object_id *fork_point)
{
	संरचना commit_list *revs = रिक्त, *result = रिक्त;

	commit_list_घाला(lookup_commit_reference(the_repository, curr_head),
			   &revs);
	commit_list_घाला(lookup_commit_reference(the_repository, merge_head),
			   &revs);
	जर (!is_null_oid(fork_point))
		commit_list_घाला(lookup_commit_reference(the_repository, fork_point),
				   &revs);

	जर (get_octopus_merge_bases(revs, &result) < 0)
		निकास(128);
	commit_list_free(revs);
	reduce_heads_replace(&result);

	जर (!result)
		परत 1;

	oidcpy(merge_base, &result->item->object.oid);
	commit_list_free(result);
	परत 0;
}

/**
 * Given the current HEAD oid, the merge head returned from git-fetch and the
 * fork point calculated by get_rebase_fork_point(), compute the <newbase> and
 * <upstream> arguments to use for the upcoming git-rebase invocation.
 */
स्थिर पूर्णांक get_rebase_newbase_and_upstream(संरचना object_id *newbase,
		संरचना object_id *upstream,
		स्थिरांक संरचना object_id *curr_head,
		स्थिरांक संरचना object_id *merge_head,
		स्थिरांक संरचना object_id *fork_point)
{
	संरचना object_id oct_merge_base;

	जर (!get_octopus_merge_base(&oct_merge_base, curr_head, merge_head, fork_point))
		जर (!is_null_oid(fork_point) && oideq(&oct_merge_base, fork_point))
			fork_point = रिक्त;

	जर (fork_point && !is_null_oid(fork_point))
		oidcpy(upstream, fork_point);
	अन्यथा
		oidcpy(upstream, merge_head);

	oidcpy(newbase, merge_head);

	परत 0;
}

/**
 * Given the <newbase> and <upstream> calculated by
 * get_rebase_newbase_and_upstream(), runs git-rebase with the
 * appropriate arguments and returns its exit status.
 */
स्थिर पूर्णांक run_rebase(स्थिरांक संरचना object_id *newbase,
		स्थिरांक संरचना object_id *upstream)
{
	संरचना child_process cmd = CHILD_PROCESS_INIT;

	strvec_ढकला(&cmd.args, "rebase");

	/* Shared options */
	argv_ढकला_verbosity(&cmd.args);

	/* Options passed to git-rebase */
	जर (opt_rebase == REBASE_MERGES)
		strvec_ढकला(&cmd.args, "--rebase-merges");
	अन्यथा जर (opt_rebase == REBASE_INTERACTIVE)
		strvec_ढकला(&cmd.args, "--interactive");
	जर (opt_diffstat)
		strvec_ढकला(&cmd.args, opt_diffstat);
	strvec_pushv(&cmd.args, opt_strategies.v);
	strvec_pushv(&cmd.args, opt_strategy_opts.v);
	जर (opt_gpg_sign)
		strvec_ढकला(&cmd.args, opt_gpg_sign);
	जर (opt_signoff)
		strvec_ढकला(&cmd.args, opt_signoff);
	जर (opt_autostash == 0)
		strvec_ढकला(&cmd.args, "--no-autostash");
	अन्यथा जर (opt_autostash == 1)
		strvec_ढकला(&cmd.args, "--autostash");
	जर (opt_verify_signatures &&
	    !म_भेद(opt_verify_signatures, "--verify-signatures"))
		warning(_("ignoring --verify-signatures for rebase"));

	strvec_ढकला(&cmd.args, "--onto");
	strvec_ढकला(&cmd.args, oid_to_hex(newbase));

	strvec_ढकला(&cmd.args, oid_to_hex(upstream));

	cmd.git_cmd = 1;
	परत run_command(&cmd);
}

स्थिर पूर्णांक get_can_ff(संरचना object_id *orig_head,
		      संरचना oid_array *merge_heads)
{
	पूर्णांक ret;
	संरचना commit_list *list = रिक्त;
	संरचना commit *merge_head, *head;
	संरचना object_id *orig_merge_head;

	जर (merge_heads->nr > 1)
		परत 0;

	orig_merge_head = &merge_heads->oid[0];
	head = lookup_commit_reference(the_repository, orig_head);
	जर (!head)
		परत 0;
	merge_head = lookup_commit_reference(the_repository, orig_merge_head);
	जर (!merge_head)
		परत 0;
	commit_list_घाला(head, &list);
	ret = repo_is_descendant_of(the_repository, merge_head, list);
	commit_list_free(list);
	जर (ret < 0)
		निकास(128);
	परत ret;
}

/*
 * Is orig_head a descendant of _all_ merge_heads?
 * Unfortunately is_descendant_of() cannot be used as it asks
 * if orig_head is a descendant of at least one of them.
 */
स्थिर पूर्णांक already_up_to_date(संरचना object_id *orig_head,
			      संरचना oid_array *merge_heads)
{
	संरचना commit *ours;

	ours = lookup_commit_reference(the_repository, orig_head);
	जर (!ours)
		परत 0;
	साठी (माप_प्रकार i = 0; i < merge_heads->nr; i++) {
		संरचना commit_list *list = रिक्त;
		संरचना commit *theirs;
		पूर्णांक ok;

		theirs = lookup_commit_reference(the_repository, &merge_heads->oid[i]);
		जर (!theirs)
			परत 0;
		commit_list_घाला(theirs, &list);
		ok = repo_is_descendant_of(the_repository, ours, list);
		commit_list_free(list);
		जर (ok < 0)
			निकास(128);
		जर (!ok)
			परत 0;
	}
	परत 1;
}

स्थिर शून्य show_advice_pull_non_ff(शून्य)
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

पूर्णांक cmd_pull(पूर्णांक argc,
	     स्थिरांक अक्षर **argv,
	     स्थिरांक अक्षर *prefix,
	     संरचना repository *repository UNUSED)
{
	स्थिरांक अक्षर *repo, **refspecs;
	संरचना oid_array merge_heads = OID_ARRAY_INIT;
	संरचना object_id orig_head, curr_head;
	संरचना object_id rebase_fork_point;
	पूर्णांक rebase_unspecified = 0;
	पूर्णांक can_ff;
	पूर्णांक divergent;
	पूर्णांक ret;
	स्थिर संरचना option pull_options[] = {
		/* Shared options */
		OPT__VERBOSITY(&opt_verbosity),
		OPT_PASSTHRU(0, "progress", &opt_progress, रिक्त,
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
		OPT_PASSTHRU('n', रिक्त, &opt_diffstat, रिक्त,
			N_("do not show a diffstat at the end of the merge"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG),
		OPT_PASSTHRU(0, "stat", &opt_diffstat, रिक्त,
			N_("show a diffstat at the end of the merge"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "summary", &opt_diffstat, रिक्त,
			N_("(synonym to --stat)"),
			PARSE_OPT_NOARG | PARSE_OPT_HIDDEN),
		OPT_PASSTHRU(0, "compact-summary", &opt_diffstat, रिक्त,
			N_("show a compact-summary at the end of the merge"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "log", &opt_log, N_("n"),
			N_("add (at most <n>) entries from shortlog to merge commit message"),
			PARSE_OPT_OPTARG),
		OPT_PASSTHRU(0, "signoff", &opt_signoff, रिक्त,
			N_("add a Signed-off-by trailer"),
			PARSE_OPT_OPTARG),
		OPT_PASSTHRU(0, "squash", &opt_squash, रिक्त,
			N_("create a single commit instead of doing a merge"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "commit", &opt_commit, रिक्त,
			N_("perform a commit if the merge succeeds (default)"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "edit", &opt_edit, रिक्त,
			N_("edit message before committing"),
			PARSE_OPT_NOARG),
		OPT_CLEANUP(&cleanup_arg),
		OPT_PASSTHRU(0, "ff", &opt_ff, रिक्त,
			N_("allow fast-forward"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "ff-only", &opt_ff, रिक्त,
			N_("abort if fast-forward is not possible"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG),
		OPT_PASSTHRU(0, "verify", &opt_verify, रिक्त,
			N_("control use of pre-merge-commit and commit-msg hooks"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "verify-signatures", &opt_verify_signatures, रिक्त,
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
		OPT_PASSTHRU(0, "all", &opt_all, रिक्त,
			N_("fetch from all remotes"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU('a', "append", &opt_append, रिक्त,
			N_("append to .git/FETCH_HEAD instead of overwriting"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "upload-pack", &opt_upload_pack, N_("path"),
			N_("path to upload pack on remote end"),
			0),
		OPT__FORCE(&opt_force, N_("force overwrite of local branch"), 0),
		OPT_PASSTHRU('t', "tags", &opt_tags, रिक्त,
			N_("fetch all tags and associated objects"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU('p', "prune", &opt_prune, रिक्त,
			N_("prune remote-tracking branches no longer on remote"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU('j', "jobs", &max_children, N_("n"),
			N_("number of submodules pulled in parallel"),
			PARSE_OPT_OPTARG),
		OPT_BOOL(0, "dry-run", &opt_dry_run,
			N_("dry run")),
		OPT_PASSTHRU('k', "keep", &opt_keep, रिक्त,
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
		OPT_PASSTHRU(0, "unshallow", &opt_unshallow, रिक्त,
			N_("convert to a complete repository"),
			PARSE_OPT_NONEG | PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "update-shallow", &opt_update_shallow, रिक्त,
			N_("accept refs that update .git/shallow"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "refmap", &opt_refmap, N_("refmap"),
			N_("specify fetch refmap"),
			PARSE_OPT_NONEG),
		OPT_PASSTHRU_ARGV('o', "server-option", &opt_fetch,
			N_("server-specific"),
			N_("option to transmit"),
			0),
		OPT_PASSTHRU('4',  "ipv4", &opt_ipv4, रिक्त,
			N_("use IPv4 addresses only"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU('6',  "ipv6", &opt_ipv6, रिक्त,
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
		OPT_PASSTHRU(0, "set-upstream", &set_upstream, रिक्त,
			N_("set upstream for git pull/fetch"),
			PARSE_OPT_NOARG),

		OPT_END()
	};

	जर (!दो_पर्या("GIT_REFLOG_ACTION"))
		set_reflog_message(argc, argv);

	repo_config(the_repository, git_pull_config, रिक्त);
	जर (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_full_index = 0;
	}

	argc = parse_options(argc, argv, prefix, pull_options, pull_usage, 0);
	जर (opt_autostash == -1)
		opt_autostash = config_pull_autostash;

	जर (recurse_submodules_cli != RECURSE_SUBMODULES_DEFAULT)
		recurse_submodules = recurse_submodules_cli;

	जर (cleanup_arg)
		/*
		 * this only checks the validity of cleanup_arg; we don't need
		 * a valid value for use_editor
		 */
		get_cleanup_mode(cleanup_arg, 0);

	parse_repo_refspecs(argc, argv, &repo, &refspecs);

	जर (!opt_ff) {
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
		जर (opt_rebase >= 0 && opt_ff && !म_भेद(opt_ff, "--ff-only")) {
			मुक्त(opt_ff);
			opt_ff = xstrdup("--ff");
		}
	}

	जर (opt_rebase < 0)
		opt_rebase = config_get_rebase(&rebase_unspecified);

	जर (repo_read_index_unmerged(the_repository))
		die_resolve_conflict("pull");

	जर (file_exists(git_path_merge_head(the_repository)))
		die_conclude_merge();

	जर (repo_get_oid(the_repository, "HEAD", &orig_head))
		oidclr(&orig_head, the_repository->hash_algo);

	जर (opt_rebase) {
		जर (opt_autostash == -1)
			opt_autostash = config_rebase_autostash;

		जर (is_null_oid(&orig_head) && !is_index_unborn(the_repository->index))
			die(_("Updating an unborn branch with changes added to the index."));

		जर (!opt_autostash)
			require_clean_work_वृक्ष(the_repository,
				N_("pull with rebase"),
				_("Please commit or stash them."), 1, 0);

		जर (get_rebase_fork_point(&rebase_fork_point, repo, *refspecs))
			oidclr(&rebase_fork_point, the_repository->hash_algo);
	}

	जर (run_fetch(repo, refspecs))
		परत 1;

	जर (opt_dry_run)
		परत 0;

	जर (repo_get_oid(the_repository, "HEAD", &curr_head))
		oidclr(&curr_head, the_repository->hash_algo);

	जर (!is_null_oid(&orig_head) && !is_null_oid(&curr_head) &&
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

		जर (checkout_fast_forward(the_repository, &orig_head,
					  &curr_head, 0))
			die(_("Cannot fast-forward your working tree.\n"
				"After making sure that you saved anything precious from\n"
				"$ git diff %s\n"
				"output, run\n"
				"$ git reset --hard\n"
				"to recover."), oid_to_hex(&orig_head));
	}

	get_merge_heads(&merge_heads);

	जर (!merge_heads.nr)
		die_no_merge_candidates(repo, refspecs);

	जर (is_null_oid(&orig_head)) {
		जर (merge_heads.nr > 1)
			die(_("Cannot merge multiple branches into empty head."));
		ret = pull_into_void(merge_heads.oid, &curr_head);
		जा cleanup;
	}
	जर (merge_heads.nr > 1) {
		जर (opt_rebase)
			die(_("Cannot rebase onto multiple branches."));
		जर (opt_ff && !म_भेद(opt_ff, "--ff-only"))
			die(_("Cannot fast-forward to multiple branches."));
	}

	can_ff = get_can_ff(&orig_head, &merge_heads);
	divergent = !can_ff && !already_up_to_date(&orig_head, &merge_heads);

	/* ff-only takes precedence over rebase */
	जर (opt_ff && !म_भेद(opt_ff, "--ff-only")) {
		जर (divergent)
			die_ff_impossible();
		opt_rebase = REBASE_FALSE;
	}
	/* If no action specified and we can't fast forward, then warn. */
	जर (!opt_ff && rebase_unspecified && divergent) {
		show_advice_pull_non_ff();
		die(_("Need to specify how to reconcile divergent branches."));
	}

	जर (opt_rebase) {
		संरचना object_id newbase;
		संरचना object_id upstream;
		get_rebase_newbase_and_upstream(&newbase, &upstream, &curr_head,
						merge_heads.oid, &rebase_fork_point);

		जर ((recurse_submodules == RECURSE_SUBMODULES_ON ||
		     recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND) &&
		    submodule_touches_in_range(the_repository, &upstream, &curr_head))
			die(_("cannot rebase with locally recorded submodule modifications"));

		जर (can_ff) {
			/* we can fast-forward this without invoking rebase */
			मुक्त(opt_ff);
			opt_ff = xstrdup("--ff-only");
			ret = run_merge();
		} अन्यथा {
			ret = run_rebase(&newbase, &upstream);
		}

		जर (!ret && (recurse_submodules == RECURSE_SUBMODULES_ON ||
			     recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND))
			ret = rebase_submodules();

		जा cleanup;
	} अन्यथा {
		ret = run_merge();
		जर (!ret && (recurse_submodules == RECURSE_SUBMODULES_ON ||
			     recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND))
			ret = update_submodules();
		जा cleanup;
	}

cleanup:
	oid_array_clear(&merge_heads);
	परत ret;
}
