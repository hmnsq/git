/*
 * Builtin "git pull"
 *
 * Based on git-pull.sh by Junio C Hamano
 *
 * Fetch one or more remote refs and merge it/them into the current HEAD.
 */

#परिभाषय USE_THE_REPOSITORY_VARIABLE

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
स्थिर गणना rebase_type parse_config_rebase(ध्रुव वर्ण *कुञ्चिका, ध्रुव वर्ण *मानम्,
		पूर्णाङ्क fatal)
{
	गणना rebase_type v = rebase_parse_मानम्(मानम्);
	यदि (v != REBASE_INVALID)
		प्रत्यागम v;

	यदि (fatal)
		die(_("invalid value for '%s': '%s'"), कुञ्चिका, मानम्);
	अन्यथा
		error(_("invalid value for '%s': '%s'"), कुञ्चिका, मानम्);

	प्रत्यागम REBASE_INVALID;
}

/**
 * Callback for --rebase, which parses arg with parse_config_rebase().
 */
स्थिर पूर्णाङ्क parse_opt_rebase(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	गणना rebase_type *मानम् = opt->मानम्;

	यदि (arg)
		*मानम् = parse_config_rebase("--rebase", arg, 0);
	अन्यथा
		*मानम् = unset ? REBASE_FALSE : REBASE_TRUE;
	प्रत्यागम *मानम् == REBASE_INVALID ? -1 : 0;
}

स्थिर ध्रुव वर्ण * ध्रुव pull_usage[] = {
	N_("git pull [<options>] [<repository> [<refspec>...]]"),
	रिक्त
};

/* Shared options */
स्थिर पूर्णाङ्क opt_verbosity;
स्थिर ध्रुव वर्ण *opt_प्रगतिः;
स्थिर पूर्णाङ्क recurse_submodules = RECURSE_SUBMODULES_DEFAULT;
स्थिर पूर्णाङ्क recurse_submodules_cli = RECURSE_SUBMODULES_DEFAULT;

/* Options passed to git-merge or git-rebase */
स्थिर गणना rebase_type opt_rebase = -1;
स्थिर ध्रुव वर्ण *opt_diffstat;
स्थिर ध्रुव वर्ण *opt_log;
स्थिर ध्रुव वर्ण *opt_signoff;
स्थिर ध्रुव वर्ण *opt_squash;
स्थिर ध्रुव वर्ण *opt_commit;
स्थिर ध्रुव वर्ण *opt_edit;
स्थिर ध्रुव वर्ण *cleanup_arg;
स्थिर वर्ण *opt_ff;
स्थिर ध्रुव वर्ण *opt_verify_signatures;
स्थिर ध्रुव वर्ण *opt_verify;
स्थिर पूर्णाङ्क opt_autostash = -1;
स्थिर पूर्णाङ्क config_rebase_autostash;
स्थिर पूर्णाङ्क config_pull_autostash = -1;
स्थिर पूर्णाङ्क check_trust_level = 1;
स्थिर संरचना strvec opt_strategies = STRVEC_INIT;
स्थिर संरचना strvec opt_strategy_opts = STRVEC_INIT;
स्थिर ध्रुव वर्ण *opt_gpg_sign;
स्थिर पूर्णाङ्क opt_allow_unrelated_histories;

/* Options passed to git-fetch */
स्थिर ध्रुव वर्ण *opt_all;
स्थिर ध्रुव वर्ण *opt_append;
स्थिर ध्रुव वर्ण *opt_upload_pack;
स्थिर पूर्णाङ्क opt_force;
स्थिर ध्रुव वर्ण *opt_चिह्नकानि;
स्थिर ध्रुव वर्ण *opt_prune;
स्थिर ध्रुव वर्ण *max_children;
स्थिर पूर्णाङ्क opt_dry_सततम्;
स्थिर ध्रुव वर्ण *opt_keep;
स्थिर ध्रुव वर्ण *opt_गभीरता;
स्थिर ध्रुव वर्ण *opt_unshallow;
स्थिर ध्रुव वर्ण *opt_update_shallow;
स्थिर ध्रुव वर्ण *opt_refmap;
स्थिर ध्रुव वर्ण *opt_ipv4;
स्थिर ध्रुव वर्ण *opt_ipv6;
स्थिर पूर्णाङ्क opt_show_forced_updates = -1;
स्थिर ध्रुव वर्ण *समुच्चयः_upstream;
स्थिर संरचना strvec opt_fetch = STRVEC_INIT;

/**
 * Pushes "-q" or "-v" switches into arr to match the opt_verbosity level.
 */
स्थिर शून्य argv_प्रक्षिप_verbosity(संरचना strvec *arr)
{
	पूर्णाङ्क verbosity;

	प्रति (verbosity = opt_verbosity; verbosity > 0; verbosity--)
		strvec_प्रक्षिप(arr, "-v");

	प्रति (verbosity = opt_verbosity; verbosity < 0; verbosity++)
		strvec_प्रक्षिप(arr, "-q");
}

/**
 * Pushes "-f" switches into arr to match the opt_force level.
 */
स्थिर शून्य argv_प्रक्षिप_force(संरचना strvec *arr)
{
	पूर्णाङ्क force = opt_force;
	यावत् (force-- > 0)
		strvec_प्रक्षिप(arr, "-f");
}

/**
 * Sets the GIT_REFLOG_ACTION environment variable to the concatenation of argv
 */
स्थिर शून्य समुच्चयः_reflog_सन्देशः(पूर्णाङ्क argc, ध्रुव वर्ण **argv)
{
	पूर्णाङ्क i;
	संरचना strbuf सन्देशः = STRBUF_INIT;

	प्रति (i = 0; i < argc; i++) {
		यदि (i)
			strbuf_addch(&सन्देशः, ' ');
		strbuf_addstr(&सन्देशः, argv[i]);
	}

	setenv("GIT_REFLOG_ACTION", सन्देशः.buf, 0);

	strbuf_release(&सन्देशः);
}

/**
 * If pull.ff is unset, returns NULL. If pull.ff is "true", returns "--ff". If
 * pull.ff is "false", returns "--no-ff". If pull.ff is "only", returns
 * "--ff-only". Otherwise, if pull.ff is set to an invalid value, die with an
 * error.
 */
स्थिर ध्रुव वर्ण *config_get_ff(शून्य)
{
	ध्रुव वर्ण *मानम्;

	यदि (repo_config_get_मानम्(the_repository, "pull.ff", &मानम्))
		प्रत्यागम रिक्त;

	विकल्प (git_parse_maybe_bool(मानम्)) {
	पक्ष 0:
		प्रत्यागम "--no-ff";
	पक्ष 1:
		प्रत्यागम "--ff";
	}

	यदि (!वाक्यं_तुलय(मानम्, "only"))
		प्रत्यागम "--ff-only";

	die(_("invalid value for '%s': '%s'"), "pull.ff", मानम्);
}

/**
 * Returns the default configured value for --rebase. It first looks for the
 * value of "branch.$curr_branch.rebase", where $curr_branch is the current
 * branch, and if HEAD is detached or the configuration key does not exist,
 * looks for the value of "pull.rebase". If both configuration keys do not
 * exist, returns REBASE_FALSE.
 */
स्थिर गणना rebase_type config_get_rebase(पूर्णाङ्क *rebase_unspecified)
{
	संरचना branch *curr_branch = branch_get("HEAD");
	ध्रुव वर्ण *मानम्;

	यदि (curr_branch) {
		वर्ण *कुञ्चिका = xstrfmt("branch.%s.rebase", curr_branch->name);

		यदि (!repo_config_get_मानम्(the_repository, कुञ्चिका, &मानम्)) {
			गणना rebase_type ret = parse_config_rebase(कुञ्चिका, मानम्, 1);
			मुक्त(कुञ्चिका);
			प्रत्यागम ret;
		}

		मुक्त(कुञ्चिका);
	}

	यदि (!repo_config_get_मानम्(the_repository, "pull.rebase", &मानम्))
		प्रत्यागम parse_config_rebase("pull.rebase", मानम्, 1);

	*rebase_unspecified = 1;

	प्रत्यागम REBASE_FALSE;
}

/**
 * Read config variables.
 */
स्थिर पूर्णाङ्क git_pull_config(ध्रुव वर्ण *var, ध्रुव वर्ण *मानम्,
			   ध्रुव संरचना config_context *ctx, शून्य *cb)
{
	यदि (!वाक्यं_तुलय(var, "rebase.autostash")) {
		/*
		 * run_rebase() also reads this option. The reason we handle it here is
		 * that when pull.rebase is true, a fast-forward may occur without
		 * invoking run_rebase(). We need to ensure that autostash is set even
		 * in the fast-forward case.
		 *
		 * run_merge() handles merge.autostash, so we don't handle it here.
		 */
		config_rebase_autostash = git_config_bool(var, मानम्);
		प्रत्यागम 0;
	} अन्यथा यदि (!वाक्यं_तुलय(var, "pull.autostash")) {
		config_pull_autostash = git_config_bool(var, मानम्);
		प्रत्यागम 0;
	} अन्यथा यदि (!वाक्यं_तुलय(var, "submodule.recurse")) {
		recurse_submodules = git_config_bool(var, मानम्) ?
			RECURSE_SUBMODULES_ON : RECURSE_SUBMODULES_OFF;
		प्रत्यागम 0;
	} अन्यथा यदि (!वाक्यं_तुलय(var, "gpg.mintrustlevel")) {
		check_trust_level = 0;
	}

	प्रत्यागम git_default_config(var, मानम्, ctx, cb);
}

/**
 * Appends merge candidates from FETCH_HEAD that are not marked not-for-merge
 * into merge_heads.
 */
स्थिर शून्य get_merge_heads(संरचना oid_array *merge_heads)
{
	ध्रुव वर्ण *filename = git_मार्गः_fetch_शीर्ष(the_repository);
	FILE *fp;
	संरचना strbuf sb = STRBUF_INIT;
	संरचना object_id oid;

	fp = xfopen(filename, "r");
	यावत् (strbuf_getline_lf(&sb, fp) != EOF) {
		ध्रुव वर्ण *p;
		यदि (parse_oid_hex(sb.buf, &oid, &p))
			अनुवर्त;  /* invalid line: does not start with object ID */
		यदि (starts_with(p, "\tnot-for-merge\t"))
			अनुवर्त;  /* ref is not-for-merge */
		oid_array_append(merge_heads, &oid);
	}
	fclose(fp);
	strbuf_release(&sb);
}

/**
 * Used by die_no_merge_candidates() as a for_each_remote() callback to
 * retrieve the name of the remote if the repository only has one remote.
 */
स्थिर पूर्णाङ्क get_only_remote(संरचना remote *remote, शून्य *cb_दत्त)
{
	ध्रुव वर्ण **remote_name = cb_दत्त;

	यदि (*remote_name)
		प्रत्यागम -1;

	*remote_name = remote->name;
	प्रत्यागम 0;
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
स्थिर शून्य NORETURN die_no_merge_candidates(ध्रुव वर्ण *repo, ध्रुव वर्ण **refspecs)
{
	संरचना branch *curr_branch = branch_get("HEAD");
	ध्रुव वर्ण *remote = curr_branch ? curr_branch->remote_name : रिक्त;

	यदि (*refspecs) {
		यदि (opt_rebase)
			fprintf_ln(stderr, _("There is no candidate for rebasing against among the refs that you just fetched."));
		अन्यथा
			fprintf_ln(stderr, _("There are no candidates for merging among the refs that you just fetched."));
		fprintf_ln(stderr, _("Generally this means that you provided a wildcard refspec which had no\n"
					"matches on the remote end."));
	} अन्यथा यदि (repo && curr_branch && (!remote || वाक्यं_तुलय(repo, remote))) {
		fprintf_ln(stderr, _("You asked to pull from the remote '%s', but did not specify\n"
			"a branch. Because this is not the default configured remote\n"
			"for your current branch, you must specify a branch on the command line."),
			repo);
	} अन्यथा यदि (!curr_branch) {
		fprintf_ln(stderr, _("You are not currently on a branch."));
		यदि (opt_rebase)
			fprintf_ln(stderr, _("Please specify which branch you want to rebase against."));
		अन्यथा
			fprintf_ln(stderr, _("Please specify which branch you want to merge with."));
		fprintf_ln(stderr, _("See git-pull(1) for details."));
		fprintf(stderr, "\n");
		fprintf_ln(stderr, "    git pull %s %s", _("<remote>"), _("<branch>"));
		fprintf(stderr, "\n");
	} अन्यथा यदि (!curr_branch->merge_nr) {
		ध्रुव वर्ण *remote_name = रिक्त;

		यदि (for_each_remote(get_only_remote, &remote_name) || !remote_name)
			remote_name = _("<remote>");

		fprintf_ln(stderr, _("There is no tracking information for the current branch."));
		यदि (opt_rebase)
			fprintf_ln(stderr, _("Please specify which branch you want to rebase against."));
		अन्यथा
			fprintf_ln(stderr, _("Please specify which branch you want to merge with."));
		fprintf_ln(stderr, _("See git-pull(1) for details."));
		fprintf(stderr, "\n");
		fprintf_ln(stderr, "    git pull %s %s", _("<remote>"), _("<branch>"));
		fprintf(stderr, "\n");
		fprintf_ln(stderr, _("If you wish to set tracking information for this branch you can do so with:"));
		fprintf(stderr, "\n");
		fprintf_ln(stderr, "    git branch --set-upstream-to=%s/%s %s\n",
				remote_name, _("<branch>"), curr_branch->name);
	} अन्यथा
		fprintf_ln(stderr, _("Your configuration specifies to merge with the ref '%s'\n"
			"from the remote, but no such ref was fetched."),
			curr_branch->merge[0]->src);
	निर्गच्छ(1);
}

/**
 * Parses argv into [<repo> [<refspecs>...]], returning their values in `repo`
 * as a string and `refspecs` as a null-terminated array of strings. If `repo`
 * is not provided in argv, it is set to NULL.
 */
स्थिर शून्य parse_repo_refspecs(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण **repo,
		ध्रुव वर्ण ***refspecs)
{
	यदि (argc > 0) {
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
स्थिर पूर्णाङ्क सततम्_fetch(ध्रुव वर्ण *repo, ध्रुव वर्ण **refspecs)
{
	संरचना child_process cmd = CHILD_PROCESS_INIT;

	strvec_pushl(&cmd.तर्काः, "fetch", "--update-head-ok", रिक्त);

	/* Shared options */
	argv_प्रक्षिप_verbosity(&cmd.तर्काः);
	यदि (opt_प्रगतिः)
		strvec_प्रक्षिप(&cmd.तर्काः, opt_प्रगतिः);

	/* Options passed to git-fetch */
	यदि (opt_all)
		strvec_प्रक्षिप(&cmd.तर्काः, opt_all);
	यदि (opt_append)
		strvec_प्रक्षिप(&cmd.तर्काः, opt_append);
	यदि (opt_upload_pack)
		strvec_प्रक्षिप(&cmd.तर्काः, opt_upload_pack);
	argv_प्रक्षिप_force(&cmd.तर्काः);
	यदि (opt_चिह्नकानि)
		strvec_प्रक्षिप(&cmd.तर्काः, opt_चिह्नकानि);
	यदि (opt_prune)
		strvec_प्रक्षिप(&cmd.तर्काः, opt_prune);
	यदि (recurse_submodules_cli != RECURSE_SUBMODULES_DEFAULT)
		विकल्प (recurse_submodules_cli) {
		पक्ष RECURSE_SUBMODULES_ON:
			strvec_प्रक्षिप(&cmd.तर्काः, "--recurse-submodules=on");
			भङ्ग;
		पक्ष RECURSE_SUBMODULES_OFF:
			strvec_प्रक्षिप(&cmd.तर्काः, "--recurse-submodules=no");
			भङ्ग;
		पक्ष RECURSE_SUBMODULES_ON_DEMAND:
			strvec_प्रक्षिप(&cmd.तर्काः, "--recurse-submodules=on-demand");
			भङ्ग;
		स्वभाव:
			BUG("submodule recursion option not understood");
		}
	यदि (max_children)
		strvec_प्रक्षिप(&cmd.तर्काः, max_children);
	यदि (opt_dry_सततम्)
		strvec_प्रक्षिप(&cmd.तर्काः, "--dry-run");
	यदि (opt_keep)
		strvec_प्रक्षिप(&cmd.तर्काः, opt_keep);
	यदि (opt_गभीरता)
		strvec_प्रक्षिप(&cmd.तर्काः, opt_गभीरता);
	यदि (opt_unshallow)
		strvec_प्रक्षिप(&cmd.तर्काः, opt_unshallow);
	यदि (opt_update_shallow)
		strvec_प्रक्षिप(&cmd.तर्काः, opt_update_shallow);
	यदि (opt_refmap)
		strvec_प्रक्षिप(&cmd.तर्काः, opt_refmap);
	यदि (opt_ipv4)
		strvec_प्रक्षिप(&cmd.तर्काः, opt_ipv4);
	यदि (opt_ipv6)
		strvec_प्रक्षिप(&cmd.तर्काः, opt_ipv6);
	यदि (opt_show_forced_updates > 0)
		strvec_प्रक्षिप(&cmd.तर्काः, "--show-forced-updates");
	अन्यथा यदि (opt_show_forced_updates == 0)
		strvec_प्रक्षिप(&cmd.तर्काः, "--no-show-forced-updates");
	यदि (समुच्चयः_upstream)
		strvec_प्रक्षिप(&cmd.तर्काः, समुच्चयः_upstream);
	strvec_pushv(&cmd.तर्काः, opt_fetch.v);

	यदि (repo) {
		strvec_प्रक्षिप(&cmd.तर्काः, repo);
		strvec_pushv(&cmd.तर्काः, refspecs);
	} अन्यथा यदि (*refspecs)
		BUG("refspecs without repo?");
	cmd.git_cmd = 1;
	cmd.odb_गन्तव्यम्_close = the_repository->objects;
	प्रत्यागम सततम्_command(&cmd);
}

/**
 * "Pulls into void" by branching off merge_head.
 */
स्थिर पूर्णाङ्क pull_into_void(ध्रुव संरचना object_id *merge_शीर्ष,
		ध्रुव संरचना object_id *curr_शीर्ष)
{
	यदि (opt_verify_signatures) {
		संरचना commit *commit;

		commit = lookup_commit(the_repository, merge_शीर्ष);
		यदि (!commit)
			die(_("unable to access commit %s"),
			    oid_गन्तव्यम्_hex(merge_शीर्ष));

		verify_merge_signature(commit, opt_verbosity,
				       check_trust_level);
	}

	/*
	 * Two-way merge: we treat the index as based on an empty tree,
	 * and try to fast-forward to HEAD. This ensures we will not lose
	 * index/worktree changes that the user already made on the unborn
	 * branch.
	 */
	यदि (checkout_fast_forward(the_repository,
				  the_hash_algo->रिक्तानि_वृक्षः,
				  merge_शीर्ष, 0))
		प्रत्यागम 1;

	यदि (refs_update_ref(get_main_ref_store(the_repository), "initial pull", "HEAD", merge_शीर्ष, curr_शीर्ष, 0, UPDATE_REFS_DIE_ON_ERR))
		प्रत्यागम 1;

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क rebase_submodules(शून्य)
{
	संरचना child_process cp = CHILD_PROCESS_INIT;

	cp.git_cmd = 1;
	cp.no_stdin = 1;
	strvec_pushl(&cp.तर्काः, "submodule", "update",
		     "--recursive", "--rebase", रिक्त);
	argv_प्रक्षिप_verbosity(&cp.तर्काः);

	प्रत्यागम सततम्_command(&cp);
}

स्थिर पूर्णाङ्क update_submodules(शून्य)
{
	संरचना child_process cp = CHILD_PROCESS_INIT;

	cp.git_cmd = 1;
	cp.no_stdin = 1;
	strvec_pushl(&cp.तर्काः, "submodule", "update",
		     "--recursive", "--checkout", रिक्त);
	argv_प्रक्षिप_verbosity(&cp.तर्काः);

	प्रत्यागम सततम्_command(&cp);
}

/**
 * Runs git-merge, returning its exit status.
 */
स्थिर पूर्णाङ्क सततम्_merge(शून्य)
{
	संरचना child_process cmd = CHILD_PROCESS_INIT;

	strvec_pushl(&cmd.तर्काः, "merge", रिक्त);

	/* Shared options */
	argv_प्रक्षिप_verbosity(&cmd.तर्काः);
	यदि (opt_प्रगतिः)
		strvec_प्रक्षिप(&cmd.तर्काः, opt_प्रगतिः);

	/* Options passed to git-merge */
	यदि (opt_diffstat)
		strvec_प्रक्षिप(&cmd.तर्काः, opt_diffstat);
	यदि (opt_log)
		strvec_प्रक्षिप(&cmd.तर्काः, opt_log);
	यदि (opt_signoff)
		strvec_प्रक्षिप(&cmd.तर्काः, opt_signoff);
	यदि (opt_squash)
		strvec_प्रक्षिप(&cmd.तर्काः, opt_squash);
	यदि (opt_commit)
		strvec_प्रक्षिप(&cmd.तर्काः, opt_commit);
	यदि (opt_edit)
		strvec_प्रक्षिप(&cmd.तर्काः, opt_edit);
	यदि (cleanup_arg)
		strvec_pushf(&cmd.तर्काः, "--cleanup=%s", cleanup_arg);
	यदि (opt_ff)
		strvec_प्रक्षिप(&cmd.तर्काः, opt_ff);
	यदि (opt_verify)
		strvec_प्रक्षिप(&cmd.तर्काः, opt_verify);
	यदि (opt_verify_signatures)
		strvec_प्रक्षिप(&cmd.तर्काः, opt_verify_signatures);
	strvec_pushv(&cmd.तर्काः, opt_strategies.v);
	strvec_pushv(&cmd.तर्काः, opt_strategy_opts.v);
	यदि (opt_gpg_sign)
		strvec_प्रक्षिप(&cmd.तर्काः, opt_gpg_sign);
	यदि (opt_autostash == 0)
		strvec_प्रक्षिप(&cmd.तर्काः, "--no-autostash");
	अन्यथा यदि (opt_autostash == 1)
		strvec_प्रक्षिप(&cmd.तर्काः, "--autostash");
	यदि (opt_allow_unrelated_histories > 0)
		strvec_प्रक्षिप(&cmd.तर्काः, "--allow-unrelated-histories");

	strvec_प्रक्षिप(&cmd.तर्काः, "FETCH_HEAD");
	cmd.git_cmd = 1;
	प्रत्यागम सततम्_command(&cmd);
}

/**
 * Returns remote's upstream branch for the current branch. If remote is NULL,
 * the current branch's configured default remote is used. Returns NULL if
 * `remote` does not name a valid remote, HEAD does not point to a branch,
 * remote is not the branch's configured remote or the branch does not have any
 * configured upstream branch.
 */
स्थिर ध्रुव वर्ण *get_upstream_branch(ध्रुव वर्ण *remote)
{
	संरचना remote *rm;
	संरचना branch *curr_branch;
	ध्रुव वर्ण *curr_branch_remote;

	rm = remote_get(remote);
	यदि (!rm)
		प्रत्यागम रिक्त;

	curr_branch = branch_get("HEAD");
	यदि (!curr_branch)
		प्रत्यागम रिक्त;

	curr_branch_remote = remote_for_branch(curr_branch, रिक्त);
	प्रतिज्ञा(curr_branch_remote);

	यदि (वाक्यं_तुलय(curr_branch_remote, rm->name))
		प्रत्यागम रिक्त;

	प्रत्यागम branch_get_upstream(curr_branch, रिक्त);
}

/**
 * Derives the remote-tracking branch from the remote and refspec.
 *
 * FIXME: The current implementation assumes the default mapping of
 * refs/heads/<branch_name> to refs/remotes/<remote_name>/<branch_name>.
 */
स्थिर ध्रुव वर्ण *get_tracking_branch(ध्रुव वर्ण *remote, ध्रुव वर्ण *refspec)
{
	संरचना refspec_item spec;
	ध्रुव वर्ण *spec_src;
	ध्रुव वर्ण *merge_branch;

	यदि (!refspec_item_init_fetch(&spec, refspec, the_hash_algo))
		die(_("invalid refspec '%s'"), refspec);
	spec_src = spec.src;
	यदि (!*spec_src || !वाक्यं_तुलय(spec_src, "HEAD"))
		spec_src = "HEAD";
	अन्यथा यदि (skip_उपसर्गः(spec_src, "heads/", &spec_src))
		;
	अन्यथा यदि (skip_उपसर्गः(spec_src, "refs/heads/", &spec_src))
		;
	अन्यथा यदि (starts_with(spec_src, "refs/") ||
		starts_with(spec_src, "tags/") ||
		starts_with(spec_src, "remotes/"))
		spec_src = "";

	यदि (*spec_src) {
		यदि (!वाक्यं_तुलय(remote, "."))
			merge_branch = mkpath("refs/heads/%s", spec_src);
		अन्यथा
			merge_branch = mkpath("refs/remotes/%s/%s", remote, spec_src);
	} अन्यथा
		merge_branch = रिक्त;

	refspec_item_clear(&spec);
	प्रत्यागम merge_branch;
}

/**
 * Given the repo and refspecs, sets fork_point to the point at which the
 * current branch forked from its remote-tracking branch. Returns 0 on success,
 * -1 on failure.
 */
स्थिर पूर्णाङ्क get_rebase_fork_point(संरचना object_id *fork_point, ध्रुव वर्ण *repo,
		ध्रुव वर्ण *refspec)
{
	पूर्णाङ्क ret;
	संरचना branch *curr_branch;
	ध्रुव वर्ण *remote_branch;
	संरचना child_process cp = CHILD_PROCESS_INIT;
	संरचना strbuf sb = STRBUF_INIT;

	curr_branch = branch_get("HEAD");
	यदि (!curr_branch)
		प्रत्यागम -1;

	यदि (refspec)
		remote_branch = get_tracking_branch(repo, refspec);
	अन्यथा
		remote_branch = get_upstream_branch(repo);

	यदि (!remote_branch)
		प्रत्यागम -1;

	strvec_pushl(&cp.तर्काः, "merge-base", "--fork-point",
		     remote_branch, curr_branch->name, रिक्त);
	cp.no_stdin = 1;
	cp.no_stderr = 1;
	cp.git_cmd = 1;

	ret = capture_command(&cp, &sb, GIT_MAX_HEXSZ);
	यदि (ret)
		गच्छ cleanup;

	ret = get_oid_hex(sb.buf, fork_point);
	यदि (ret)
		गच्छ cleanup;

cleanup:
	strbuf_release(&sb);
	प्रत्यागम ret ? -1 : 0;
}

/**
 * Sets merge_base to the octopus merge base of curr_head, merge_head and
 * fork_point. Returns 0 if a merge base is found, 1 otherwise.
 */
स्थिर पूर्णाङ्क get_octopus_merge_आधारः(संरचना object_id *merge_आधारः,
		ध्रुव संरचना object_id *curr_शीर्ष,
		ध्रुव संरचना object_id *merge_शीर्ष,
		ध्रुव संरचना object_id *fork_point)
{
	संरचना commit_list *revs = रिक्त, *परिणाम = रिक्त;

	commit_list_निवेशय(lookup_commit_reference(the_repository, curr_शीर्ष),
			   &revs);
	commit_list_निवेशय(lookup_commit_reference(the_repository, merge_शीर्ष),
			   &revs);
	यदि (!is_null_oid(fork_point))
		commit_list_निवेशय(lookup_commit_reference(the_repository, fork_point),
				   &revs);

	यदि (get_octopus_merge_bases(revs, &परिणाम) < 0)
		निर्गच्छ(128);
	commit_list_free(revs);
	reduce_heads_replace(&परिणाम);

	यदि (!परिणाम)
		प्रत्यागम 1;

	oidcpy(merge_आधारः, &परिणाम->item->object.oid);
	commit_list_free(परिणाम);
	प्रत्यागम 0;
}

/**
 * Given the current HEAD oid, the merge head returned from git-fetch and the
 * fork point calculated by get_rebase_fork_point(), compute the <newbase> and
 * <upstream> arguments to use for the upcoming git-rebase invocation.
 */
स्थिर पूर्णाङ्क get_rebase_newbase_and_upstream(संरचना object_id *newbase,
		संरचना object_id *upstream,
		ध्रुव संरचना object_id *curr_शीर्ष,
		ध्रुव संरचना object_id *merge_शीर्ष,
		ध्रुव संरचना object_id *fork_point)
{
	संरचना object_id oct_merge_आधारः;

	यदि (!get_octopus_merge_आधारः(&oct_merge_आधारः, curr_शीर्ष, merge_शीर्ष, fork_point))
		यदि (!is_null_oid(fork_point) && oideq(&oct_merge_आधारः, fork_point))
			fork_point = रिक्त;

	यदि (fork_point && !is_null_oid(fork_point))
		oidcpy(upstream, fork_point);
	अन्यथा
		oidcpy(upstream, merge_शीर्ष);

	oidcpy(newbase, merge_शीर्ष);

	प्रत्यागम 0;
}

/**
 * Given the <newbase> and <upstream> calculated by
 * get_rebase_newbase_and_upstream(), runs git-rebase with the
 * appropriate arguments and returns its exit status.
 */
स्थिर पूर्णाङ्क सततम्_rebase(ध्रुव संरचना object_id *newbase,
		ध्रुव संरचना object_id *upstream)
{
	संरचना child_process cmd = CHILD_PROCESS_INIT;

	strvec_प्रक्षिप(&cmd.तर्काः, "rebase");

	/* Shared options */
	argv_प्रक्षिप_verbosity(&cmd.तर्काः);

	/* Options passed to git-rebase */
	यदि (opt_rebase == REBASE_MERGES)
		strvec_प्रक्षिप(&cmd.तर्काः, "--rebase-merges");
	अन्यथा यदि (opt_rebase == REBASE_INTERACTIVE)
		strvec_प्रक्षिप(&cmd.तर्काः, "--interactive");
	यदि (opt_diffstat)
		strvec_प्रक्षिप(&cmd.तर्काः, opt_diffstat);
	strvec_pushv(&cmd.तर्काः, opt_strategies.v);
	strvec_pushv(&cmd.तर्काः, opt_strategy_opts.v);
	यदि (opt_gpg_sign)
		strvec_प्रक्षिप(&cmd.तर्काः, opt_gpg_sign);
	यदि (opt_signoff)
		strvec_प्रक्षिप(&cmd.तर्काः, opt_signoff);
	यदि (opt_autostash == 0)
		strvec_प्रक्षिप(&cmd.तर्काः, "--no-autostash");
	अन्यथा यदि (opt_autostash == 1)
		strvec_प्रक्षिप(&cmd.तर्काः, "--autostash");
	यदि (opt_verify_signatures &&
	    !वाक्यं_तुलय(opt_verify_signatures, "--verify-signatures"))
		warning(_("ignoring --verify-signatures for rebase"));

	strvec_प्रक्षिप(&cmd.तर्काः, "--onto");
	strvec_प्रक्षिप(&cmd.तर्काः, oid_गन्तव्यम्_hex(newbase));

	strvec_प्रक्षिप(&cmd.तर्काः, oid_गन्तव्यम्_hex(upstream));

	cmd.git_cmd = 1;
	प्रत्यागम सततम्_command(&cmd);
}

स्थिर पूर्णाङ्क get_can_ff(संरचना object_id *orig_शीर्ष,
		      संरचना oid_array *merge_heads)
{
	पूर्णाङ्क ret;
	संरचना commit_list *list = रिक्त;
	संरचना commit *merge_शीर्ष, *शीर्ष;
	संरचना object_id *orig_merge_शीर्ष;

	यदि (merge_heads->nr > 1)
		प्रत्यागम 0;

	orig_merge_शीर्ष = &merge_heads->oid[0];
	शीर्ष = lookup_commit_reference(the_repository, orig_शीर्ष);
	यदि (!शीर्ष)
		प्रत्यागम 0;
	merge_शीर्ष = lookup_commit_reference(the_repository, orig_merge_शीर्ष);
	यदि (!merge_शीर्ष)
		प्रत्यागम 0;
	commit_list_निवेशय(शीर्ष, &list);
	ret = repo_is_descendant_सम्बन्धि(the_repository, merge_शीर्ष, list);
	commit_list_free(list);
	यदि (ret < 0)
		निर्गच्छ(128);
	प्रत्यागम ret;
}

/*
 * Is orig_head a descendant of _all_ merge_heads?
 * Unfortunately is_descendant_of() cannot be used as it asks
 * if orig_head is a descendant of at least one of them.
 */
स्थिर पूर्णाङ्क already_ऊर्ध्वम्_गन्तव्यम्_date(संरचना object_id *orig_शीर्ष,
			      संरचना oid_array *merge_heads)
{
	संरचना commit *ours;

	ours = lookup_commit_reference(the_repository, orig_शीर्ष);
	यदि (!ours)
		प्रत्यागम 0;
	प्रति (आकार_t i = 0; i < merge_heads->nr; i++) {
		संरचना commit_list *list = रिक्त;
		संरचना commit *theirs;
		पूर्णाङ्क ok;

		theirs = lookup_commit_reference(the_repository, &merge_heads->oid[i]);
		यदि (!theirs)
			प्रत्यागम 0;
		commit_list_निवेशय(theirs, &list);
		ok = repo_is_descendant_सम्बन्धि(the_repository, ours, list);
		commit_list_free(list);
		यदि (ok < 0)
			निर्गच्छ(128);
		यदि (!ok)
			प्रत्यागम 0;
	}
	प्रत्यागम 1;
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

पूर्णाङ्क cmd_pull(पूर्णाङ्क argc,
	     ध्रुव वर्ण **argv,
	     ध्रुव वर्ण *उपसर्गः,
	     संरचना repository *repository अप्रयुक्तम्)
{
	ध्रुव वर्ण *repo, **refspecs;
	संरचना oid_array merge_heads = OID_ARRAY_INIT;
	संरचना object_id orig_शीर्ष, curr_शीर्ष;
	संरचना object_id rebase_fork_point;
	पूर्णाङ्क rebase_unspecified = 0;
	पूर्णाङ्क can_ff;
	पूर्णाङ्क divergent;
	पूर्णाङ्क ret;
	स्थिर संरचना option pull_options[] = {
		/* Shared options */
		OPT__VERBOSITY(&opt_verbosity),
		OPT_PASSTHRU(0, "progress", &opt_प्रगतिः, रिक्त,
			N_("force progress reporting"),
			PARSE_OPT_NOARG),
		OPT_CALLBACK_F(0, "recurse-submodules",
			   &recurse_submodules_cli, N_("on-demand"),
			   N_("control for recursive fetching of submodules"),
			   PARSE_OPT_OPTARG, option_fetch_parse_recurse_submodules),

		/* Options passed to git-merge or git-rebase */
		OPT_गणः(N_("Options related to merging")),
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
		OPT_समुच्चयः_INT(0, "allow-unrelated-histories",
			    &opt_allow_unrelated_histories,
			    N_("allow merging unrelated histories"), 1),

		/* Options passed to git-fetch */
		OPT_गणः(N_("Options related to fetching")),
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
		OPT_PASSTHRU('t', "tags", &opt_चिह्नकानि, रिक्त,
			N_("fetch all tags and associated objects"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU('p', "prune", &opt_prune, रिक्त,
			N_("prune remote-tracking branches no longer on remote"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU('j', "jobs", &max_children, N_("n"),
			N_("number of submodules pulled in parallel"),
			PARSE_OPT_OPTARG),
		OPT_BOOL(0, "dry-run", &opt_dry_सततम्,
			N_("dry run")),
		OPT_PASSTHRU('k', "keep", &opt_keep, रिक्त,
			N_("keep downloaded pack"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU(0, "depth", &opt_गभीरता, N_("depth"),
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
		OPT_PASSTHRU(0, "set-upstream", &समुच्चयः_upstream, रिक्त,
			N_("set upstream for git pull/fetch"),
			PARSE_OPT_NOARG),

		OPT_अन्तः()
	};

	यदि (!getenv("GIT_REFLOG_ACTION"))
		समुच्चयः_reflog_सन्देशः(argc, argv);

	repo_config(the_repository, git_pull_config, रिक्त);
	यदि (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_पूर्णम्_सूचकांक = 0;
	}

	argc = parse_options(argc, argv, उपसर्गः, pull_options, pull_usage, 0);
	यदि (opt_autostash == -1)
		opt_autostash = config_pull_autostash;

	यदि (recurse_submodules_cli != RECURSE_SUBMODULES_DEFAULT)
		recurse_submodules = recurse_submodules_cli;

	यदि (cleanup_arg)
		/*
		 * this only checks the validity of cleanup_arg; we don't need
		 * a valid value for use_editor
		 */
		get_cleanup_बहुलकम्(cleanup_arg, 0);

	parse_repo_refspecs(argc, argv, &repo, &refspecs);

	यदि (!opt_ff) {
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
		यदि (opt_rebase >= 0 && opt_ff && !वाक्यं_तुलय(opt_ff, "--ff-only")) {
			मुक्त(opt_ff);
			opt_ff = xstrdup("--ff");
		}
	}

	यदि (opt_rebase < 0)
		opt_rebase = config_get_rebase(&rebase_unspecified);

	यदि (repo_read_सूचकांक_unmerged(the_repository))
		die_resolve_conflict("pull");

	यदि (file_exists(git_मार्गः_merge_शीर्ष(the_repository)))
		die_conclude_merge();

	यदि (repo_get_oid(the_repository, "HEAD", &orig_शीर्ष))
		oidclr(&orig_शीर्ष, the_repository->hash_algo);

	यदि (opt_rebase) {
		यदि (opt_autostash == -1)
			opt_autostash = config_rebase_autostash;

		यदि (is_null_oid(&orig_शीर्ष) && !is_सूचकांक_unborn(the_repository->सूचकांक))
			die(_("Updating an unborn branch with changes added to the index."));

		यदि (!opt_autostash)
			require_clean_work_वृक्षः(the_repository,
				N_("pull with rebase"),
				_("Please commit or stash them."), 1, 0);

		यदि (get_rebase_fork_point(&rebase_fork_point, repo, *refspecs))
			oidclr(&rebase_fork_point, the_repository->hash_algo);
	}

	यदि (सततम्_fetch(repo, refspecs))
		प्रत्यागम 1;

	यदि (opt_dry_सततम्)
		प्रत्यागम 0;

	यदि (repo_get_oid(the_repository, "HEAD", &curr_शीर्ष))
		oidclr(&curr_शीर्ष, the_repository->hash_algo);

	यदि (!is_null_oid(&orig_शीर्ष) && !is_null_oid(&curr_शीर्ष) &&
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
			"commit %s."), oid_गन्तव्यम्_hex(&orig_शीर्ष));

		यदि (checkout_fast_forward(the_repository, &orig_शीर्ष,
					  &curr_शीर्ष, 0))
			die(_("Cannot fast-forward your working tree.\n"
				"After making sure that you saved anything precious from\n"
				"$ git diff %s\n"
				"output, run\n"
				"$ git reset --hard\n"
				"to recover."), oid_गन्तव्यम्_hex(&orig_शीर्ष));
	}

	get_merge_heads(&merge_heads);

	यदि (!merge_heads.nr)
		die_no_merge_candidates(repo, refspecs);

	यदि (is_null_oid(&orig_शीर्ष)) {
		यदि (merge_heads.nr > 1)
			die(_("Cannot merge multiple branches into empty head."));
		ret = pull_into_void(merge_heads.oid, &curr_शीर्ष);
		गच्छ cleanup;
	}
	यदि (merge_heads.nr > 1) {
		यदि (opt_rebase)
			die(_("Cannot rebase onto multiple branches."));
		यदि (opt_ff && !वाक्यं_तुलय(opt_ff, "--ff-only"))
			die(_("Cannot fast-forward to multiple branches."));
	}

	can_ff = get_can_ff(&orig_शीर्ष, &merge_heads);
	divergent = !can_ff && !already_ऊर्ध्वम्_गन्तव्यम्_date(&orig_शीर्ष, &merge_heads);

	/* ff-only takes precedence over rebase */
	यदि (opt_ff && !वाक्यं_तुलय(opt_ff, "--ff-only")) {
		यदि (divergent)
			die_ff_impossible();
		opt_rebase = REBASE_FALSE;
	}
	/* If no action specified and we can't fast forward, then warn. */
	यदि (!opt_ff && rebase_unspecified && divergent) {
		show_advice_pull_non_ff();
		die(_("Need to specify how to reconcile divergent branches."));
	}

	यदि (opt_rebase) {
		संरचना object_id newbase;
		संरचना object_id upstream;
		get_rebase_newbase_and_upstream(&newbase, &upstream, &curr_शीर्ष,
						merge_heads.oid, &rebase_fork_point);

		यदि ((recurse_submodules == RECURSE_SUBMODULES_ON ||
		     recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND) &&
		    submodule_touches_अन्तर्_range(the_repository, &upstream, &curr_शीर्ष))
			die(_("cannot rebase with locally recorded submodule modifications"));

		यदि (can_ff) {
			/* we can fast-forward this without invoking rebase */
			मुक्त(opt_ff);
			opt_ff = xstrdup("--ff-only");
			ret = सततम्_merge();
		} अन्यथा {
			ret = सततम्_rebase(&newbase, &upstream);
		}

		यदि (!ret && (recurse_submodules == RECURSE_SUBMODULES_ON ||
			     recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND))
			ret = rebase_submodules();

		गच्छ cleanup;
	} अन्यथा {
		ret = सततम्_merge();
		यदि (!ret && (recurse_submodules == RECURSE_SUBMODULES_ON ||
			     recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND))
			ret = update_submodules();
		गच्छ cleanup;
	}

cleanup:
	oid_array_clear(&merge_heads);
	प्रत्यागम ret;
}
