/*
 * Builtin "git pull"
 *
 * Based on git-pull.sh by Junio C Hamano
 *
 * Fetch one or more remote refs and merge it/them into the current HEAD.
 */

#घोषणा USE_THE_REPOSITORY_VARIABLE

#समावेश "builtin.h"
#समावेश "advice.h"
#समावेश "config.h"
#समावेश "environment.h"
#समावेश "gettext.h"
#समावेश "hex.h"
#समावेश "merge.h"
#समावेश "object-name.h"
#समावेश "parse-options.h"
#समावेश "run-command.h"
#समावेश "oid-array.h"
#समावेश "remote.h"
#समावेश "dir.h"
#समावेश "path.h"
#समावेश "read-cache-ll.h"
#समावेश "rebase.h"
#समावेश "refs.h"
#समावेश "refspec.h"
#समावेश "submodule.h"
#समावेश "submodule-config.h"
#समावेश "wt-status.h"
#समावेश "commit-reach.h"
#समावेश "sequencer.h"

/**
 * Parses the value of --rebase. If value is a false value, returns
 * REBASE_FALSE. If value is a true value, returns REBASE_TRUE. If value is
 * "merges", returns REBASE_MERGES. If value is a invalid value, dies with
 * a fatal error if fatal is true, otherwise returns REBASE_INVALID.
 */
अटल क्रमागत rebase_type parse_config_rebase(स्थिर अक्षर *कुंजी, स्थिर अक्षर *मूल्य,
		पूर्णांक fatal)
{
	क्रमागत rebase_type v = rebase_parse_मूल्य(मूल्य);
	अगर (v != REBASE_INVALID)
		वापस v;

	अगर (fatal)
		die(_("invalid value for '%s': '%s'"), कुंजी, मूल्य);
	अन्यथा
		error(_("invalid value for '%s': '%s'"), कुंजी, मूल्य);

	वापस REBASE_INVALID;
}

/**
 * Callback for --rebase, which parses arg with parse_config_rebase().
 */
अटल पूर्णांक parse_opt_rebase(स्थिर काष्ठा option *opt, स्थिर अक्षर *arg, पूर्णांक unset)
{
	क्रमागत rebase_type *मूल्य = opt->मूल्य;

	अगर (arg)
		*मूल्य = parse_config_rebase("--rebase", arg, 0);
	अन्यथा
		*मूल्य = unset ? REBASE_FALSE : REBASE_TRUE;
	वापस *मूल्य == REBASE_INVALID ? -1 : 0;
}

अटल स्थिर अक्षर * स्थिर pull_usage[] = {
	N_("git pull [<options>] [<repository> [<refspec>...]]"),
	शून्य
};

/* Shared options */
अटल पूर्णांक opt_verbosity;
अटल स्थिर अक्षर *opt_progress;
अटल पूर्णांक recurse_submodules = RECURSE_SUBMODULES_DEFAULT;
अटल पूर्णांक recurse_submodules_cli = RECURSE_SUBMODULES_DEFAULT;

/* Options passed to git-merge or git-rebase */
अटल क्रमागत rebase_type opt_rebase = -1;
अटल स्थिर अक्षर *opt_diffstat;
अटल स्थिर अक्षर *opt_log;
अटल स्थिर अक्षर *opt_signoff;
अटल स्थिर अक्षर *opt_squash;
अटल स्थिर अक्षर *opt_commit;
अटल स्थिर अक्षर *opt_edit;
अटल स्थिर अक्षर *cleanup_arg;
अटल अक्षर *opt_ff;
अटल स्थिर अक्षर *opt_verify_signatures;
अटल स्थिर अक्षर *opt_verify;
अटल पूर्णांक opt_autostash = -1;
अटल पूर्णांक config_rebase_autostash;
अटल पूर्णांक config_pull_autostash = -1;
अटल पूर्णांक check_trust_level = 1;
अटल काष्ठा strvec opt_strategies = STRVEC_INIT;
अटल काष्ठा strvec opt_strategy_opts = STRVEC_INIT;
अटल स्थिर अक्षर *opt_gpg_sign;
अटल पूर्णांक opt_allow_unrelated_histories;

/* Options passed to git-fetch */
अटल स्थिर अक्षर *opt_all;
अटल स्थिर अक्षर *opt_append;
अटल स्थिर अक्षर *opt_upload_pack;
अटल पूर्णांक opt_force;
अटल स्थिर अक्षर *opt_tags;
अटल स्थिर अक्षर *opt_prune;
अटल स्थिर अक्षर *max_children;
अटल पूर्णांक opt_dry_run;
अटल स्थिर अक्षर *opt_keep;
अटल स्थिर अक्षर *opt_depth;
अटल स्थिर अक्षर *opt_unshallow;
अटल स्थिर अक्षर *opt_update_shallow;
अटल स्थिर अक्षर *opt_refmap;
अटल स्थिर अक्षर *opt_ipv4;
अटल स्थिर अक्षर *opt_ipv6;
अटल पूर्णांक opt_show_forced_updates = -1;
अटल स्थिर अक्षर *set_upstream;
अटल काष्ठा strvec opt_fetch = STRVEC_INIT;

/**
 * Pushes "-q" or "-v" switches into arr to match the opt_verbosity level.
 */
अटल व्योम argv_धकेलो_verbosity(काष्ठा strvec *arr)
{
	पूर्णांक verbosity;

	क्रम (verbosity = opt_verbosity; verbosity > 0; verbosity--)
		strvec_धकेलो(arr, "-v");

	क्रम (verbosity = opt_verbosity; verbosity < 0; verbosity++)
		strvec_धकेलो(arr, "-q");
}

/**
 * Pushes "-f" switches into arr to match the opt_force level.
 */
अटल व्योम argv_धकेलो_force(काष्ठा strvec *arr)
{
	पूर्णांक force = opt_force;
	जबतक (force-- > 0)
		strvec_धकेलो(arr, "-f");
}

/**
 * Sets the GIT_REFLOG_ACTION environment variable to the concatenation of argv
 */
अटल व्योम set_reflog_message(पूर्णांक argc, स्थिर अक्षर **argv)
{
	पूर्णांक i;
	काष्ठा strbuf संदेश = STRBUF_INIT;

	क्रम (i = 0; i < argc; i++) {
		अगर (i)
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
अटल स्थिर अक्षर *config_get_ff(व्योम)
{
	स्थिर अक्षर *मूल्य;

	अगर (repo_config_get_मूल्य(the_repository, "pull.ff", &मूल्य))
		वापस शून्य;

	चयन (git_parse_maybe_bool(मूल्य)) {
	हाल 0:
		वापस "--no-ff";
	हाल 1:
		वापस "--ff";
	}

	अगर (!म_भेद(मूल्य, "only"))
		वापस "--ff-only";

	die(_("invalid value for '%s': '%s'"), "pull.ff", मूल्य);
}

/**
 * Returns the default configured value for --rebase. It first looks for the
 * value of "branch.$curr_branch.rebase", where $curr_branch is the current
 * branch, and if HEAD is detached or the configuration key does not exist,
 * looks for the value of "pull.rebase". If both configuration keys do not
 * exist, returns REBASE_FALSE.
 */
अटल क्रमागत rebase_type config_get_rebase(पूर्णांक *rebase_unspecified)
{
	काष्ठा branch *curr_branch = branch_get("HEAD");
	स्थिर अक्षर *मूल्य;

	अगर (curr_branch) {
		अक्षर *कुंजी = xstrfmt("branch.%s.rebase", curr_branch->name);

		अगर (!repo_config_get_मूल्य(the_repository, कुंजी, &मूल्य)) {
			क्रमागत rebase_type ret = parse_config_rebase(कुंजी, मूल्य, 1);
			मुक्त(कुंजी);
			वापस ret;
		}

		मुक्त(कुंजी);
	}

	अगर (!repo_config_get_मूल्य(the_repository, "pull.rebase", &मूल्य))
		वापस parse_config_rebase("pull.rebase", मूल्य, 1);

	*rebase_unspecified = 1;

	वापस REBASE_FALSE;
}

/**
 * Read config variables.
 */
अटल पूर्णांक git_pull_config(स्थिर अक्षर *var, स्थिर अक्षर *मूल्य,
			   स्थिर काष्ठा config_context *ctx, व्योम *cb)
{
	अगर (!म_भेद(var, "rebase.autostash")) {
		/*
		 * run_rebase() also reads this option. The reason we handle it here is
		 * that when pull.rebase is true, a fast-forward may occur without
		 * invoking run_rebase(). We need to ensure that autostash is set even
		 * in the fast-forward case.
		 *
		 * run_merge() handles merge.autostash, so we don't handle it here.
		 */
		config_rebase_autostash = git_config_bool(var, मूल्य);
		वापस 0;
	} अन्यथा अगर (!म_भेद(var, "pull.autostash")) {
		config_pull_autostash = git_config_bool(var, मूल्य);
		वापस 0;
	} अन्यथा अगर (!म_भेद(var, "submodule.recurse")) {
		recurse_submodules = git_config_bool(var, मूल्य) ?
			RECURSE_SUBMODULES_ON : RECURSE_SUBMODULES_OFF;
		वापस 0;
	} अन्यथा अगर (!म_भेद(var, "gpg.mintrustlevel")) {
		check_trust_level = 0;
	}

	वापस git_default_config(var, मूल्य, ctx, cb);
}

/**
 * Appends merge candidates from FETCH_HEAD that are not marked not-for-merge
 * into merge_heads.
 */
अटल व्योम get_merge_heads(काष्ठा oid_array *merge_heads)
{
	स्थिर अक्षर *filename = git_path_fetch_शीर्ष(the_repository);
	खाता *fp;
	काष्ठा strbuf sb = STRBUF_INIT;
	काष्ठा object_id oid;

	fp = xfopen(filename, "r");
	जबतक (strbuf_getline_lf(&sb, fp) != खातापूर्ण) {
		स्थिर अक्षर *p;
		अगर (parse_oid_hex(sb.buf, &oid, &p))
			जारी;  /* invalid line: does not start with object ID */
		अगर (starts_with(p, "\tnot-for-merge\t"))
			जारी;  /* ref is not-for-merge */
		oid_array_append(merge_heads, &oid);
	}
	ख_बंद(fp);
	strbuf_release(&sb);
}

/**
 * Used by die_no_merge_candidates() as a for_each_remote() callback to
 * retrieve the name of the remote if the repository only has one remote.
 */
अटल पूर्णांक get_only_remote(काष्ठा remote *remote, व्योम *cb_तथ्य)
{
	स्थिर अक्षर **remote_name = cb_तथ्य;

	अगर (*remote_name)
		वापस -1;

	*remote_name = remote->name;
	वापस 0;
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
अटल व्योम NORETURN die_no_merge_candidates(स्थिर अक्षर *repo, स्थिर अक्षर **refspecs)
{
	काष्ठा branch *curr_branch = branch_get("HEAD");
	स्थिर अक्षर *remote = curr_branch ? curr_branch->remote_name : शून्य;

	अगर (*refspecs) {
		अगर (opt_rebase)
			fprintf_ln(मानक_त्रुटि, _("There is no candidate for rebasing against among the refs that you just fetched."));
		अन्यथा
			fprintf_ln(मानक_त्रुटि, _("There are no candidates for merging among the refs that you just fetched."));
		fprintf_ln(मानक_त्रुटि, _("Generally this means that you provided a wildcard refspec which had no\n"
					"matches on the remote end."));
	} अन्यथा अगर (repo && curr_branch && (!remote || म_भेद(repo, remote))) {
		fprintf_ln(मानक_त्रुटि, _("You asked to pull from the remote '%s', but did not specify\n"
			"a branch. Because this is not the default configured remote\n"
			"for your current branch, you must specify a branch on the command line."),
			repo);
	} अन्यथा अगर (!curr_branch) {
		fprintf_ln(मानक_त्रुटि, _("You are not currently on a branch."));
		अगर (opt_rebase)
			fprintf_ln(मानक_त्रुटि, _("Please specify which branch you want to rebase against."));
		अन्यथा
			fprintf_ln(मानक_त्रुटि, _("Please specify which branch you want to merge with."));
		fprintf_ln(मानक_त्रुटि, _("See git-pull(1) for details."));
		ख_लिखो(मानक_त्रुटि, "\n");
		fprintf_ln(मानक_त्रुटि, "    git pull %s %s", _("<remote>"), _("<branch>"));
		ख_लिखो(मानक_त्रुटि, "\n");
	} अन्यथा अगर (!curr_branch->merge_nr) {
		स्थिर अक्षर *remote_name = शून्य;

		अगर (for_each_remote(get_only_remote, &remote_name) || !remote_name)
			remote_name = _("<remote>");

		fprintf_ln(मानक_त्रुटि, _("There is no tracking information for the current branch."));
		अगर (opt_rebase)
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
अटल व्योम parse_repo_refspecs(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर **repo,
		स्थिर अक्षर ***refspecs)
{
	अगर (argc > 0) {
		*repo = *argv++;
		argc--;
	} अन्यथा
		*repo = शून्य;
	*refspecs = argv;
}

/**
 * Runs git-fetch, returning its exit status. `repo` and `refspecs` are the
 * repository and refspecs to fetch, or NULL if they are not provided.
 */
अटल पूर्णांक run_fetch(स्थिर अक्षर *repo, स्थिर अक्षर **refspecs)
{
	काष्ठा child_process cmd = CHILD_PROCESS_INIT;

	strvec_pushl(&cmd.args, "fetch", "--update-head-ok", शून्य);

	/* Shared options */
	argv_धकेलो_verbosity(&cmd.args);
	अगर (opt_progress)
		strvec_धकेलो(&cmd.args, opt_progress);

	/* Options passed to git-fetch */
	अगर (opt_all)
		strvec_धकेलो(&cmd.args, opt_all);
	अगर (opt_append)
		strvec_धकेलो(&cmd.args, opt_append);
	अगर (opt_upload_pack)
		strvec_धकेलो(&cmd.args, opt_upload_pack);
	argv_धकेलो_force(&cmd.args);
	अगर (opt_tags)
		strvec_धकेलो(&cmd.args, opt_tags);
	अगर (opt_prune)
		strvec_धकेलो(&cmd.args, opt_prune);
	अगर (recurse_submodules_cli != RECURSE_SUBMODULES_DEFAULT)
		चयन (recurse_submodules_cli) {
		हाल RECURSE_SUBMODULES_ON:
			strvec_धकेलो(&cmd.args, "--recurse-submodules=on");
			अवरोध;
		हाल RECURSE_SUBMODULES_OFF:
			strvec_धकेलो(&cmd.args, "--recurse-submodules=no");
			अवरोध;
		हाल RECURSE_SUBMODULES_ON_DEMAND:
			strvec_धकेलो(&cmd.args, "--recurse-submodules=on-demand");
			अवरोध;
		शेष:
			BUG("submodule recursion option not understood");
		}
	अगर (max_children)
		strvec_धकेलो(&cmd.args, max_children);
	अगर (opt_dry_run)
		strvec_धकेलो(&cmd.args, "--dry-run");
	अगर (opt_keep)
		strvec_धकेलो(&cmd.args, opt_keep);
	अगर (opt_depth)
		strvec_धकेलो(&cmd.args, opt_depth);
	अगर (opt_unshallow)
		strvec_धकेलो(&cmd.args, opt_unshallow);
	अगर (opt_update_shallow)
		strvec_धकेलो(&cmd.args, opt_update_shallow);
	अगर (opt_refmap)
		strvec_धकेलो(&cmd.args, opt_refmap);
	अगर (opt_ipv4)
		strvec_धकेलो(&cmd.args, opt_ipv4);
	अगर (opt_ipv6)
		strvec_धकेलो(&cmd.args, opt_ipv6);
	अगर (opt_show_forced_updates > 0)
		strvec_धकेलो(&cmd.args, "--show-forced-updates");
	अन्यथा अगर (opt_show_forced_updates == 0)
		strvec_धकेलो(&cmd.args, "--no-show-forced-updates");
	अगर (set_upstream)
		strvec_धकेलो(&cmd.args, set_upstream);
	strvec_pushv(&cmd.args, opt_fetch.v);

	अगर (repo) {
		strvec_धकेलो(&cmd.args, repo);
		strvec_pushv(&cmd.args, refspecs);
	} अन्यथा अगर (*refspecs)
		BUG("refspecs without repo?");
	cmd.git_cmd = 1;
	cmd.odb_to_close = the_repository->objects;
	वापस run_command(&cmd);
}

/**
 * "Pulls into void" by branching off merge_head.
 */
अटल पूर्णांक pull_into_void(स्थिर काष्ठा object_id *merge_शीर्ष,
		स्थिर काष्ठा object_id *curr_शीर्ष)
{
	अगर (opt_verify_signatures) {
		काष्ठा commit *commit;

		commit = lookup_commit(the_repository, merge_शीर्ष);
		अगर (!commit)
			die(_("unable to access commit %s"),
			    oid_to_hex(merge_शीर्ष));

		verify_merge_signature(commit, opt_verbosity,
				       check_trust_level);
	}

	/*
	 * Two-way merge: we treat the index as based on an empty tree,
	 * and try to fast-forward to HEAD. This ensures we will not lose
	 * index/worktree changes that the user already made on the unborn
	 * branch.
	 */
	अगर (checkout_fast_forward(the_repository,
				  the_hash_algo->empty_वृक्ष,
				  merge_शीर्ष, 0))
		वापस 1;

	अगर (refs_update_ref(get_main_ref_store(the_repository), "initial pull", "HEAD", merge_शीर्ष, curr_शीर्ष, 0, UPDATE_REFS_DIE_ON_ERR))
		वापस 1;

	वापस 0;
}

अटल पूर्णांक rebase_submodules(व्योम)
{
	काष्ठा child_process cp = CHILD_PROCESS_INIT;

	cp.git_cmd = 1;
	cp.no_stdin = 1;
	strvec_pushl(&cp.args, "submodule", "update",
		     "--recursive", "--rebase", शून्य);
	argv_धकेलो_verbosity(&cp.args);

	वापस run_command(&cp);
}

अटल पूर्णांक update_submodules(व्योम)
{
	काष्ठा child_process cp = CHILD_PROCESS_INIT;

	cp.git_cmd = 1;
	cp.no_stdin = 1;
	strvec_pushl(&cp.args, "submodule", "update",
		     "--recursive", "--checkout", शून्य);
	argv_धकेलो_verbosity(&cp.args);

	वापस run_command(&cp);
}

/**
 * Runs git-merge, returning its exit status.
 */
अटल पूर्णांक run_merge(व्योम)
{
	काष्ठा child_process cmd = CHILD_PROCESS_INIT;

	strvec_pushl(&cmd.args, "merge", शून्य);

	/* Shared options */
	argv_धकेलो_verbosity(&cmd.args);
	अगर (opt_progress)
		strvec_धकेलो(&cmd.args, opt_progress);

	/* Options passed to git-merge */
	अगर (opt_diffstat)
		strvec_धकेलो(&cmd.args, opt_diffstat);
	अगर (opt_log)
		strvec_धकेलो(&cmd.args, opt_log);
	अगर (opt_signoff)
		strvec_धकेलो(&cmd.args, opt_signoff);
	अगर (opt_squash)
		strvec_धकेलो(&cmd.args, opt_squash);
	अगर (opt_commit)
		strvec_धकेलो(&cmd.args, opt_commit);
	अगर (opt_edit)
		strvec_धकेलो(&cmd.args, opt_edit);
	अगर (cleanup_arg)
		strvec_pushf(&cmd.args, "--cleanup=%s", cleanup_arg);
	अगर (opt_ff)
		strvec_धकेलो(&cmd.args, opt_ff);
	अगर (opt_verify)
		strvec_धकेलो(&cmd.args, opt_verify);
	अगर (opt_verify_signatures)
		strvec_धकेलो(&cmd.args, opt_verify_signatures);
	strvec_pushv(&cmd.args, opt_strategies.v);
	strvec_pushv(&cmd.args, opt_strategy_opts.v);
	अगर (opt_gpg_sign)
		strvec_धकेलो(&cmd.args, opt_gpg_sign);
	अगर (opt_autostash == 0)
		strvec_धकेलो(&cmd.args, "--no-autostash");
	अन्यथा अगर (opt_autostash == 1)
		strvec_धकेलो(&cmd.args, "--autostash");
	अगर (opt_allow_unrelated_histories > 0)
		strvec_धकेलो(&cmd.args, "--allow-unrelated-histories");

	strvec_धकेलो(&cmd.args, "FETCH_HEAD");
	cmd.git_cmd = 1;
	वापस run_command(&cmd);
}

/**
 * Returns remote's upstream branch for the current branch. If remote is NULL,
 * the current branch's configured default remote is used. Returns NULL if
 * `remote` does not name a valid remote, HEAD does not point to a branch,
 * remote is not the branch's configured remote or the branch does not have any
 * configured upstream branch.
 */
अटल स्थिर अक्षर *get_upstream_branch(स्थिर अक्षर *remote)
{
	काष्ठा remote *rm;
	काष्ठा branch *curr_branch;
	स्थिर अक्षर *curr_branch_remote;

	rm = remote_get(remote);
	अगर (!rm)
		वापस शून्य;

	curr_branch = branch_get("HEAD");
	अगर (!curr_branch)
		वापस शून्य;

	curr_branch_remote = remote_for_branch(curr_branch, शून्य);
	निश्चित(curr_branch_remote);

	अगर (म_भेद(curr_branch_remote, rm->name))
		वापस शून्य;

	वापस branch_get_upstream(curr_branch, शून्य);
}

/**
 * Derives the remote-tracking branch from the remote and refspec.
 *
 * FIXME: The current implementation assumes the default mapping of
 * refs/heads/<branch_name> to refs/remotes/<remote_name>/<branch_name>.
 */
अटल स्थिर अक्षर *get_tracking_branch(स्थिर अक्षर *remote, स्थिर अक्षर *refspec)
{
	काष्ठा refspec_item spec;
	स्थिर अक्षर *spec_src;
	स्थिर अक्षर *merge_branch;

	अगर (!refspec_item_init_fetch(&spec, refspec, the_hash_algo))
		die(_("invalid refspec '%s'"), refspec);
	spec_src = spec.src;
	अगर (!*spec_src || !म_भेद(spec_src, "HEAD"))
		spec_src = "HEAD";
	अन्यथा अगर (skip_prefix(spec_src, "heads/", &spec_src))
		;
	अन्यथा अगर (skip_prefix(spec_src, "refs/heads/", &spec_src))
		;
	अन्यथा अगर (starts_with(spec_src, "refs/") ||
		starts_with(spec_src, "tags/") ||
		starts_with(spec_src, "remotes/"))
		spec_src = "";

	अगर (*spec_src) {
		अगर (!म_भेद(remote, "."))
			merge_branch = mkpath("refs/heads/%s", spec_src);
		अन्यथा
			merge_branch = mkpath("refs/remotes/%s/%s", remote, spec_src);
	} अन्यथा
		merge_branch = शून्य;

	refspec_item_clear(&spec);
	वापस merge_branch;
}

/**
 * Given the repo and refspecs, sets fork_point to the point at which the
 * current branch forked from its remote-tracking branch. Returns 0 on success,
 * -1 on failure.
 */
अटल पूर्णांक get_rebase_fork_point(काष्ठा object_id *fork_point, स्थिर अक्षर *repo,
		स्थिर अक्षर *refspec)
{
	पूर्णांक ret;
	काष्ठा branch *curr_branch;
	स्थिर अक्षर *remote_branch;
	काष्ठा child_process cp = CHILD_PROCESS_INIT;
	काष्ठा strbuf sb = STRBUF_INIT;

	curr_branch = branch_get("HEAD");
	अगर (!curr_branch)
		वापस -1;

	अगर (refspec)
		remote_branch = get_tracking_branch(repo, refspec);
	अन्यथा
		remote_branch = get_upstream_branch(repo);

	अगर (!remote_branch)
		वापस -1;

	strvec_pushl(&cp.args, "merge-base", "--fork-point",
		     remote_branch, curr_branch->name, शून्य);
	cp.no_stdin = 1;
	cp.no_stderr = 1;
	cp.git_cmd = 1;

	ret = capture_command(&cp, &sb, GIT_MAX_HEXSZ);
	अगर (ret)
		जाओ cleanup;

	ret = get_oid_hex(sb.buf, fork_point);
	अगर (ret)
		जाओ cleanup;

cleanup:
	strbuf_release(&sb);
	वापस ret ? -1 : 0;
}

/**
 * Sets merge_base to the octopus merge base of curr_head, merge_head and
 * fork_point. Returns 0 if a merge base is found, 1 otherwise.
 */
अटल पूर्णांक get_octopus_merge_base(काष्ठा object_id *merge_base,
		स्थिर काष्ठा object_id *curr_शीर्ष,
		स्थिर काष्ठा object_id *merge_शीर्ष,
		स्थिर काष्ठा object_id *fork_point)
{
	काष्ठा commit_list *revs = शून्य, *परिणाम = शून्य;

	commit_list_जोड़ो(lookup_commit_reference(the_repository, curr_शीर्ष),
			   &revs);
	commit_list_जोड़ो(lookup_commit_reference(the_repository, merge_शीर्ष),
			   &revs);
	अगर (!is_null_oid(fork_point))
		commit_list_जोड़ो(lookup_commit_reference(the_repository, fork_point),
				   &revs);

	अगर (get_octopus_merge_bases(revs, &परिणाम) < 0)
		निकास(128);
	commit_list_free(revs);
	reduce_heads_replace(&परिणाम);

	अगर (!परिणाम)
		वापस 1;

	oidcpy(merge_base, &परिणाम->item->object.oid);
	commit_list_free(परिणाम);
	वापस 0;
}

/**
 * Given the current HEAD oid, the merge head returned from git-fetch and the
 * fork point calculated by get_rebase_fork_point(), compute the <newbase> and
 * <upstream> arguments to use for the upcoming git-rebase invocation.
 */
अटल पूर्णांक get_rebase_newbase_and_upstream(काष्ठा object_id *newbase,
		काष्ठा object_id *upstream,
		स्थिर काष्ठा object_id *curr_शीर्ष,
		स्थिर काष्ठा object_id *merge_शीर्ष,
		स्थिर काष्ठा object_id *fork_point)
{
	काष्ठा object_id oct_merge_base;

	अगर (!get_octopus_merge_base(&oct_merge_base, curr_शीर्ष, merge_शीर्ष, fork_point))
		अगर (!is_null_oid(fork_point) && oideq(&oct_merge_base, fork_point))
			fork_point = शून्य;

	अगर (fork_point && !is_null_oid(fork_point))
		oidcpy(upstream, fork_point);
	अन्यथा
		oidcpy(upstream, merge_शीर्ष);

	oidcpy(newbase, merge_शीर्ष);

	वापस 0;
}

/**
 * Given the <newbase> and <upstream> calculated by
 * get_rebase_newbase_and_upstream(), runs git-rebase with the
 * appropriate arguments and returns its exit status.
 */
अटल पूर्णांक run_rebase(स्थिर काष्ठा object_id *newbase,
		स्थिर काष्ठा object_id *upstream)
{
	काष्ठा child_process cmd = CHILD_PROCESS_INIT;

	strvec_धकेलो(&cmd.args, "rebase");

	/* Shared options */
	argv_धकेलो_verbosity(&cmd.args);

	/* Options passed to git-rebase */
	अगर (opt_rebase == REBASE_MERGES)
		strvec_धकेलो(&cmd.args, "--rebase-merges");
	अन्यथा अगर (opt_rebase == REBASE_INTERACTIVE)
		strvec_धकेलो(&cmd.args, "--interactive");
	अगर (opt_diffstat)
		strvec_धकेलो(&cmd.args, opt_diffstat);
	strvec_pushv(&cmd.args, opt_strategies.v);
	strvec_pushv(&cmd.args, opt_strategy_opts.v);
	अगर (opt_gpg_sign)
		strvec_धकेलो(&cmd.args, opt_gpg_sign);
	अगर (opt_signoff)
		strvec_धकेलो(&cmd.args, opt_signoff);
	अगर (opt_autostash == 0)
		strvec_धकेलो(&cmd.args, "--no-autostash");
	अन्यथा अगर (opt_autostash == 1)
		strvec_धकेलो(&cmd.args, "--autostash");
	अगर (opt_verify_signatures &&
	    !म_भेद(opt_verify_signatures, "--verify-signatures"))
		warning(_("ignoring --verify-signatures for rebase"));

	strvec_धकेलो(&cmd.args, "--onto");
	strvec_धकेलो(&cmd.args, oid_to_hex(newbase));

	strvec_धकेलो(&cmd.args, oid_to_hex(upstream));

	cmd.git_cmd = 1;
	वापस run_command(&cmd);
}

अटल पूर्णांक get_can_ff(काष्ठा object_id *orig_शीर्ष,
		      काष्ठा oid_array *merge_heads)
{
	पूर्णांक ret;
	काष्ठा commit_list *list = शून्य;
	काष्ठा commit *merge_शीर्ष, *शीर्ष;
	काष्ठा object_id *orig_merge_शीर्ष;

	अगर (merge_heads->nr > 1)
		वापस 0;

	orig_merge_शीर्ष = &merge_heads->oid[0];
	शीर्ष = lookup_commit_reference(the_repository, orig_शीर्ष);
	अगर (!शीर्ष)
		वापस 0;
	merge_शीर्ष = lookup_commit_reference(the_repository, orig_merge_शीर्ष);
	अगर (!merge_शीर्ष)
		वापस 0;
	commit_list_जोड़ो(शीर्ष, &list);
	ret = repo_is_descendant_of(the_repository, merge_शीर्ष, list);
	commit_list_free(list);
	अगर (ret < 0)
		निकास(128);
	वापस ret;
}

/*
 * Is orig_head a descendant of _all_ merge_heads?
 * Unfortunately is_descendant_of() cannot be used as it asks
 * if orig_head is a descendant of at least one of them.
 */
अटल पूर्णांक already_up_to_date(काष्ठा object_id *orig_शीर्ष,
			      काष्ठा oid_array *merge_heads)
{
	काष्ठा commit *ours;

	ours = lookup_commit_reference(the_repository, orig_शीर्ष);
	अगर (!ours)
		वापस 0;
	क्रम (माप_प्रकार i = 0; i < merge_heads->nr; i++) {
		काष्ठा commit_list *list = शून्य;
		काष्ठा commit *theirs;
		पूर्णांक ok;

		theirs = lookup_commit_reference(the_repository, &merge_heads->oid[i]);
		अगर (!theirs)
			वापस 0;
		commit_list_जोड़ो(theirs, &list);
		ok = repo_is_descendant_of(the_repository, ours, list);
		commit_list_free(list);
		अगर (ok < 0)
			निकास(128);
		अगर (!ok)
			वापस 0;
	}
	वापस 1;
}

अटल व्योम show_advice_pull_non_ff(व्योम)
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
	     स्थिर अक्षर **argv,
	     स्थिर अक्षर *prefix,
	     काष्ठा repository *repository UNUSED)
{
	स्थिर अक्षर *repo, **refspecs;
	काष्ठा oid_array merge_heads = OID_ARRAY_INIT;
	काष्ठा object_id orig_शीर्ष, curr_शीर्ष;
	काष्ठा object_id rebase_fork_point;
	पूर्णांक rebase_unspecified = 0;
	पूर्णांक can_ff;
	पूर्णांक divergent;
	पूर्णांक ret;
	अटल काष्ठा option pull_options[] = {
		/* Shared options */
		OPT__VERBOSITY(&opt_verbosity),
		OPT_PASSTHRU(0, "progress", &opt_progress, शून्य,
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
		OPT_PASSTHRU('n', शून्य, &opt_diffstat, शून्य,
			N_("do not show a diffstat at the end of the merge"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG),
		OPT_PASSTHRU(0, "stat", &opt_diffstat, शून्य,
			N_("show a diffstat at the end of the merge"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "summary", &opt_diffstat, शून्य,
			N_("(synonym to --stat)"),
			PARSE_OPT_NOARG | PARSE_OPT_HIDDEN),
		OPT_PASSTHRU(0, "compact-summary", &opt_diffstat, शून्य,
			N_("show a compact-summary at the end of the merge"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "log", &opt_log, N_("n"),
			N_("add (at most <n>) entries from shortlog to merge commit message"),
			PARSE_OPT_OPTARG),
		OPT_PASSTHRU(0, "signoff", &opt_signoff, शून्य,
			N_("add a Signed-off-by trailer"),
			PARSE_OPT_OPTARG),
		OPT_PASSTHRU(0, "squash", &opt_squash, शून्य,
			N_("create a single commit instead of doing a merge"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "commit", &opt_commit, शून्य,
			N_("perform a commit if the merge succeeds (default)"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "edit", &opt_edit, शून्य,
			N_("edit message before committing"),
			PARSE_OPT_NOARG),
		OPT_CLEANUP(&cleanup_arg),
		OPT_PASSTHRU(0, "ff", &opt_ff, शून्य,
			N_("allow fast-forward"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "ff-only", &opt_ff, शून्य,
			N_("abort if fast-forward is not possible"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG),
		OPT_PASSTHRU(0, "verify", &opt_verify, शून्य,
			N_("control use of pre-merge-commit and commit-msg hooks"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "verify-signatures", &opt_verify_signatures, शून्य,
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
		OPT_PASSTHRU(0, "all", &opt_all, शून्य,
			N_("fetch from all remotes"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU('a', "append", &opt_append, शून्य,
			N_("append to .git/FETCH_HEAD instead of overwriting"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "upload-pack", &opt_upload_pack, N_("path"),
			N_("path to upload pack on remote end"),
			0),
		OPT__FORCE(&opt_force, N_("force overwrite of local branch"), 0),
		OPT_PASSTHRU('t', "tags", &opt_tags, शून्य,
			N_("fetch all tags and associated objects"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU('p', "prune", &opt_prune, शून्य,
			N_("prune remote-tracking branches no longer on remote"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU('j', "jobs", &max_children, N_("n"),
			N_("number of submodules pulled in parallel"),
			PARSE_OPT_OPTARG),
		OPT_BOOL(0, "dry-run", &opt_dry_run,
			N_("dry run")),
		OPT_PASSTHRU('k', "keep", &opt_keep, शून्य,
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
		OPT_PASSTHRU(0, "unshallow", &opt_unshallow, शून्य,
			N_("convert to a complete repository"),
			PARSE_OPT_NONEG | PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "update-shallow", &opt_update_shallow, शून्य,
			N_("accept refs that update .git/shallow"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "refmap", &opt_refmap, N_("refmap"),
			N_("specify fetch refmap"),
			PARSE_OPT_NONEG),
		OPT_PASSTHRU_ARGV('o', "server-option", &opt_fetch,
			N_("server-specific"),
			N_("option to transmit"),
			0),
		OPT_PASSTHRU('4',  "ipv4", &opt_ipv4, शून्य,
			N_("use IPv4 addresses only"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU('6',  "ipv6", &opt_ipv6, शून्य,
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
		OPT_PASSTHRU(0, "set-upstream", &set_upstream, शून्य,
			N_("set upstream for git pull/fetch"),
			PARSE_OPT_NOARG),

		OPT_END()
	};

	अगर (!दो_पर्या("GIT_REFLOG_ACTION"))
		set_reflog_message(argc, argv);

	repo_config(the_repository, git_pull_config, शून्य);
	अगर (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_full_सूचकांक = 0;
	}

	argc = parse_options(argc, argv, prefix, pull_options, pull_usage, 0);
	अगर (opt_autostash == -1)
		opt_autostash = config_pull_autostash;

	अगर (recurse_submodules_cli != RECURSE_SUBMODULES_DEFAULT)
		recurse_submodules = recurse_submodules_cli;

	अगर (cleanup_arg)
		/*
		 * this only checks the validity of cleanup_arg; we don't need
		 * a valid value for use_editor
		 */
		get_cleanup_mode(cleanup_arg, 0);

	parse_repo_refspecs(argc, argv, &repo, &refspecs);

	अगर (!opt_ff) {
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
		अगर (opt_rebase >= 0 && opt_ff && !म_भेद(opt_ff, "--ff-only")) {
			मुक्त(opt_ff);
			opt_ff = xstrdup("--ff");
		}
	}

	अगर (opt_rebase < 0)
		opt_rebase = config_get_rebase(&rebase_unspecified);

	अगर (repo_read_सूचकांक_unmerged(the_repository))
		die_resolve_conflict("pull");

	अगर (file_exists(git_path_merge_शीर्ष(the_repository)))
		die_conclude_merge();

	अगर (repo_get_oid(the_repository, "HEAD", &orig_शीर्ष))
		oidclr(&orig_शीर्ष, the_repository->hash_algo);

	अगर (opt_rebase) {
		अगर (opt_autostash == -1)
			opt_autostash = config_rebase_autostash;

		अगर (is_null_oid(&orig_शीर्ष) && !is_सूचकांक_unborn(the_repository->सूचकांक))
			die(_("Updating an unborn branch with changes added to the index."));

		अगर (!opt_autostash)
			require_clean_work_वृक्ष(the_repository,
				N_("pull with rebase"),
				_("Please commit or stash them."), 1, 0);

		अगर (get_rebase_fork_point(&rebase_fork_point, repo, *refspecs))
			oidclr(&rebase_fork_point, the_repository->hash_algo);
	}

	अगर (run_fetch(repo, refspecs))
		वापस 1;

	अगर (opt_dry_run)
		वापस 0;

	अगर (repo_get_oid(the_repository, "HEAD", &curr_शीर्ष))
		oidclr(&curr_शीर्ष, the_repository->hash_algo);

	अगर (!is_null_oid(&orig_शीर्ष) && !is_null_oid(&curr_शीर्ष) &&
			!oideq(&orig_शीर्ष, &curr_शीर्ष)) {
		/*
		 * The fetch involved updating the current branch.
		 *
		 * The working tree and the index file are still based on
		 * orig_head commit, but we are merging into curr_head.
		 * Update the working tree to match curr_head.
		 */

		warning(_("fetch updated the current branch head.\n"
			"fast-forwarding your working tree from\n"
			"commit %s."), oid_to_hex(&orig_शीर्ष));

		अगर (checkout_fast_forward(the_repository, &orig_शीर्ष,
					  &curr_शीर्ष, 0))
			die(_("Cannot fast-forward your working tree.\n"
				"After making sure that you saved anything precious from\n"
				"$ git diff %s\n"
				"output, run\n"
				"$ git reset --hard\n"
				"to recover."), oid_to_hex(&orig_शीर्ष));
	}

	get_merge_heads(&merge_heads);

	अगर (!merge_heads.nr)
		die_no_merge_candidates(repo, refspecs);

	अगर (is_null_oid(&orig_शीर्ष)) {
		अगर (merge_heads.nr > 1)
			die(_("Cannot merge multiple branches into empty head."));
		ret = pull_into_void(merge_heads.oid, &curr_शीर्ष);
		जाओ cleanup;
	}
	अगर (merge_heads.nr > 1) {
		अगर (opt_rebase)
			die(_("Cannot rebase onto multiple branches."));
		अगर (opt_ff && !म_भेद(opt_ff, "--ff-only"))
			die(_("Cannot fast-forward to multiple branches."));
	}

	can_ff = get_can_ff(&orig_शीर्ष, &merge_heads);
	divergent = !can_ff && !already_up_to_date(&orig_शीर्ष, &merge_heads);

	/* ff-only takes precedence over rebase */
	अगर (opt_ff && !म_भेद(opt_ff, "--ff-only")) {
		अगर (divergent)
			die_ff_impossible();
		opt_rebase = REBASE_FALSE;
	}
	/* If no action specified and we can't fast forward, then warn. */
	अगर (!opt_ff && rebase_unspecified && divergent) {
		show_advice_pull_non_ff();
		die(_("Need to specify how to reconcile divergent branches."));
	}

	अगर (opt_rebase) {
		काष्ठा object_id newbase;
		काष्ठा object_id upstream;
		get_rebase_newbase_and_upstream(&newbase, &upstream, &curr_शीर्ष,
						merge_heads.oid, &rebase_fork_point);

		अगर ((recurse_submodules == RECURSE_SUBMODULES_ON ||
		     recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND) &&
		    submodule_touches_in_range(the_repository, &upstream, &curr_शीर्ष))
			die(_("cannot rebase with locally recorded submodule modifications"));

		अगर (can_ff) {
			/* we can fast-forward this without invoking rebase */
			मुक्त(opt_ff);
			opt_ff = xstrdup("--ff-only");
			ret = run_merge();
		} अन्यथा {
			ret = run_rebase(&newbase, &upstream);
		}

		अगर (!ret && (recurse_submodules == RECURSE_SUBMODULES_ON ||
			     recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND))
			ret = rebase_submodules();

		जाओ cleanup;
	} अन्यथा {
		ret = run_merge();
		अगर (!ret && (recurse_submodules == RECURSE_SUBMODULES_ON ||
			     recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND))
			ret = update_submodules();
		जाओ cleanup;
	}

cleanup:
	oid_array_clear(&merge_heads);
	वापस ret;
}
