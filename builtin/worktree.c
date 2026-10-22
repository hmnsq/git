#నిర్వచించు USE_THE_REPOSITORY_VARIABLE
#నిర్వచించు DISABLE_SIGN_COMPARE_WARNINGS

#అన్తర్భావ "builtin.h"
#అన్తర్భావ "abspath.h"
#అన్తర్భావ "advice.h"
#అన్తర్భావ "checkout.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "copy.h"
#అన్తర్భావ "dir.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "hex.h"
#అన్తర్భావ "object-file.h"
#అన్తర్భావ "object-name.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "path.h"
#అన్తర్భావ "strvec.h"
#అన్తర్భావ "branch.h"
#అన్తర్భావ "read-cache-ll.h"
#అన్తర్భావ "refs.h"
#అన్తర్భావ "remote.h"
#అన్తర్భావ "run-command.h"
#అన్తర్భావ "hook.h"
#అన్తర్భావ "sigchain.h"
#అన్తర్భావ "submodule.h"
#అన్తర్భావ "utf8.h"
#అన్తర్భావ "worktree.h"
#అన్తర్భావ "quote.h"

#నిర్వచించు BUILTIN_WORKTREE_కూడు_USAGE \
	N_("git worktree add [-f] [--detach] [--checkout] [--lock [--reason <string>]]\n" \
	   "                 [--orphan] [(-b | -B) <new-branch>] <path> [<commit-ish>]")

#నిర్వచించు BUILTIN_WORKTREE_LIST_USAGE \
	N_("git worktree list [-v | --porcelain [-z]]")
#నిర్వచించు BUILTIN_WORKTREE_తాళం_USAGE \
	N_("git worktree lock [--reason <string>] <worktree>")
#నిర్వచించు BUILTIN_WORKTREE_MOVE_USAGE \
	N_("git worktree move <worktree> <new-path>")
#నిర్వచించు BUILTIN_WORKTREE_PRUNE_USAGE \
	N_("git worktree prune [-n] [-v] [--expire <expire>]")
#నిర్వచించు BUILTIN_WORKTREE_REMOVE_USAGE \
	N_("git worktree remove [-f] <worktree>")
#నిర్వచించు BUILTIN_WORKTREE_REPAIR_USAGE \
	N_("git worktree repair [<path>...]")
#నిర్వచించు BUILTIN_WORKTREE_UNLOCK_USAGE \
	N_("git worktree unlock <worktree>")

#నిర్వచించు WORKTREE_కూడు_DWIM_ORPHAN_INFER_పాఠ్యం \
	_("No possible source branch, inferring '--orphan'")

#నిర్వచించు WORKTREE_కూడు_ORPHAN_WITH_DASH_B_HINT_పాఠ్యం \
	_("If you meant to create a worktree containing a new unborn branch\n" \
	"(branch with no commits) for this repository, you can do so\n" \
	"using the --orphan flag:\n" \
	"\n" \
	"    git worktree add --orphan -b %s %s\n")

#నిర్వచించు WORKTREE_కూడు_ORPHAN_NO_DASH_B_HINT_పాఠ్యం \
	_("If you meant to create a worktree containing a new unborn branch\n" \
	"(branch with no commits) for this repository, you can do so\n" \
	"using the --orphan flag:\n" \
	"\n" \
	"    git worktree add --orphan %s\n")

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి git_worktree_usage[] = {
	BUILTIN_WORKTREE_కూడు_USAGE,
	BUILTIN_WORKTREE_LIST_USAGE,
	BUILTIN_WORKTREE_తాళం_USAGE,
	BUILTIN_WORKTREE_MOVE_USAGE,
	BUILTIN_WORKTREE_PRUNE_USAGE,
	BUILTIN_WORKTREE_REMOVE_USAGE,
	BUILTIN_WORKTREE_REPAIR_USAGE,
	BUILTIN_WORKTREE_UNLOCK_USAGE,
	ఖాళీ
};

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి git_worktree_కూడు_usage[] = {
	BUILTIN_WORKTREE_కూడు_USAGE,
	ఖాళీ,
};

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి git_worktree_list_usage[] = {
	BUILTIN_WORKTREE_LIST_USAGE,
	ఖాళీ
};

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి git_worktree_తాళం_usage[] = {
	BUILTIN_WORKTREE_తాళం_USAGE,
	ఖాళీ
};

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి git_worktree_move_usage[] = {
	BUILTIN_WORKTREE_MOVE_USAGE,
	ఖాళీ
};

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి git_worktree_prune_usage[] = {
	BUILTIN_WORKTREE_PRUNE_USAGE,
	ఖాళీ
};

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి git_worktree_remove_usage[] = {
	BUILTIN_WORKTREE_REMOVE_USAGE,
	ఖాళీ
};

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి git_worktree_repair_usage[] = {
	BUILTIN_WORKTREE_REPAIR_USAGE,
	ఖాళీ
};

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి git_worktree_unlock_usage[] = {
	BUILTIN_WORKTREE_UNLOCK_USAGE,
	ఖాళీ
};

నిర్మాణం కూడు_opts {
	పూర్ణాంకం force;
	పూర్ణాంకం detach;
	పూర్ణాంకం quiet;
	పూర్ణాంకం checkout;
	పూర్ణాంకం orphan;
	పూర్ణాంకం relative_paths;
	స్థిరరాశి అక్షరం *keep_locked;
};

స్థిర పూర్ణాంకం show_only;
స్థిర పూర్ణాంకం verbose;
స్థిర పూర్ణాంకం guess_remote;
స్థిర పూర్ణాంకం use_relative_paths;
స్థిర timestamp_t expire;

స్థిర పూర్ణాంకం git_worktree_config(స్థిరరాశి అక్షరం *var, స్థిరరాశి అక్షరం *విలువ,
			       స్థిరరాశి నిర్మాణం config_context *ctx, శూన్య *cb)
{
	ఒకవేళ (!strcmp(var, "worktree.guessremote")) {
		guess_remote = git_config_bool(var, విలువ);
		తిరిగి 0;
	} లేకపోతే ఒకవేళ (!strcmp(var, "worktree.userelativepaths")) {
		use_relative_paths = git_config_bool(var, విలువ);
		తిరిగి 0;
	}

	తిరిగి git_default_config(var, విలువ, ctx, cb);
}

స్థిర పూర్ణాంకం delete_git_dir(స్థిరరాశి అక్షరం *id)
{
	నిర్మాణం strbuf sb = STRBUF_INIT;
	పూర్ణాంకం ret;

	repo_common_మార్గం_append(the_repository, &sb, "worktrees/%s", id);
	ret = remove_dir_recursively(&sb, 0);
	ఒకవేళ (ret < 0 && errno == ENOTDIR)
		ret = unlink(sb.buf);
	ఒకవేళ (ret)
		error_errno(_("failed to delete '%s'"), sb.buf);
	strbuf_release(&sb);
	తిరిగి ret;
}

స్థిర శూన్య delete_worktrees_dir_if_ఖాళీలు(శూన్య)
{
	అక్షరం *మార్గం = repo_git_మార్గం(the_repository, "worktrees");
	rmdir(మార్గం); /* ignore failed removal */
	విడుదల(మార్గం);
}

స్థిర శూన్య prune_worktree(స్థిరరాశి అక్షరం *id, స్థిరరాశి అక్షరం *reason)
{
	ఒకవేళ (show_only || verbose)
		fprintf_ln(stderr, _("Removing %s/%s: %s"), "worktrees", id, reason);
	ఒకవేళ (!show_only)
		delete_git_dir(id);
}

స్థిర పూర్ణాంకం prune_cmp(స్థిరరాశి శూన్య *a, స్థిరరాశి శూన్య *b)
{
	స్థిరరాశి నిర్మాణం string_list_అంశం *x = a;
	స్థిరరాశి నిర్మాణం string_list_అంశం *y = b;
	పూర్ణాంకం c;

	ఒకవేళ ((c = fspathcmp(x->string, y->string)))
	    తిరిగి c;
	/*
	 * paths same; prune_dupes() removes all but the first worktree entry
	 * having the same path, so sort main worktree ('util' is NULL) above
	 * linked worktrees ('util' not NULL) since main worktree can't be
	 * removed
	 */
	ఒకవేళ (!x->util)
		తిరిగి -1;
	ఒకవేళ (!y->util)
		తిరిగి 1;
	/* paths same; sort by .git/worktrees/<id> */
	తిరిగి strcmp(x->util, y->util);
}

స్థిర శూన్య prune_dups(నిర్మాణం string_list *l)
{
	పూర్ణాంకం i;

	QSORT(l->వస్తువులు, l->nr, prune_cmp);
	కొరకు (i = 1; i < l->nr; i++) {
		ఒకవేళ (!fspathcmp(l->వస్తువులు[i].string, l->వస్తువులు[i - 1].string))
			prune_worktree(l->వస్తువులు[i].util, "duplicate entry");
	}
}

స్థిర శూన్య prune_worktrees(శూన్య)
{
	నిర్మాణం strbuf reason = STRBUF_INIT;
	నిర్మాణం strbuf main_మార్గం = STRBUF_INIT;
	నిర్మాణం string_list kept = STRING_LIST_INIT_DUP;
	అక్షరం *మార్గం;
	DIR *dir;
	నిర్మాణం dirent *d;

	మార్గం = repo_git_మార్గం(the_repository, "worktrees");
	dir = opendir(మార్గం);
	విడుదల(మార్గం);
	ఒకవేళ (!dir)
		తిరిగి;
	అయితే ((d = readdir_skip_dot_and_dotdot(dir)) != ఖాళీ) {
		అక్షరం *మార్గం;
		strbuf_reset(&reason);
		ఒకవేళ (should_prune_worktree(the_repository, d->d_name, &reason, &మార్గం, expire))
			prune_worktree(d->d_name, reason.buf);
		లేకపోతే ఒకవేళ (మార్గం)
			string_list_append_nodup(&kept, మార్గం)->util = xstrdup(d->d_name);
	}
	closedir(dir);

	strbuf_కూడు_absolute_మార్గం(&main_మార్గం, repo_get_common_dir(the_repository));
	/* massage main worktree absolute path to match 'gitdir' content */
	strbuf_strip_suffix(&main_మార్గం, "/.");
	string_list_append_nodup(&kept, strbuf_detach(&main_మార్గం, ఖాళీ));
	prune_dups(&kept);
	string_list_clear(&kept, 1);

	ఒకవేళ (!show_only)
		delete_worktrees_dir_if_ఖాళీలు();
	strbuf_release(&reason);
}

స్థిర పూర్ణాంకం prune(పూర్ణాంకం ac, స్థిరరాశి అక్షరం **av, స్థిరరాశి అక్షరం *ఉపసర్గ,
		 నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం option options[] = {
		OPT_DRY_నిరంతరం(&show_only, N_("do not remove, show only")),
		OPT__VERBOSE(&verbose, N_("report pruned working trees")),
		OPT_EXPIRY_DATE(0, "expire", &expire,
				N_("prune missing working trees older than <time>")),
		OPT_చివర()
	};

	expire = సమయం_MAX;
	ac = parse_options(ac, av, ఉపసర్గ, options, git_worktree_prune_usage,
			   0);
	ఒకవేళ (ac)
		usage_with_options(git_worktree_prune_usage, options);
	prune_worktrees();
	తిరిగి 0;
}

స్థిర అక్షరం *junk_work_వృక్షం;
స్థిర అక్షరం *junk_git_dir;
స్థిర పూర్ణాంకం is_junk;
స్థిర pid_t junk_pid;

స్థిర శూన్య remove_junk(శూన్య)
{
	నిర్మాణం strbuf sb = STRBUF_INIT;
	ఒకవేళ (!is_junk || getpid() != junk_pid)
		తిరిగి;
	ఒకవేళ (junk_git_dir) {
		strbuf_addstr(&sb, junk_git_dir);
		remove_dir_recursively(&sb, 0);
		strbuf_reset(&sb);
	}
	ఒకవేళ (junk_work_వృక్షం) {
		strbuf_addstr(&sb, junk_work_వృక్షం);
		remove_dir_recursively(&sb, 0);
	}
	strbuf_release(&sb);
}

స్థిర శూన్య remove_junk_on_signal(పూర్ణాంకం signo)
{
	remove_junk();
	sigchain_తీయి(signo);
	లేపు(signo);
}

స్థిర అక్షరం *worktree_basename_dup(స్థిరరాశి అక్షరం *మార్గం)
{
	స్థిరరాశి అక్షరం *name;
	పూర్ణాంకం len;

	len = అక్షరశ్రేణి_పొడవు(మార్గం);
	అయితే (len && is_dir_sep(మార్గం[len - 1]))
		len--;

	name = మార్గం + len;
	అయితే (name > మార్గం && !is_dir_sep(name[-1]))
		name--;

	తిరిగి xmemdupz(name, మార్గం + len - name);
}

/* check that path is viable location for worktree */
స్థిర శూన్య check_candidate_మార్గం(స్థిరరాశి అక్షరం *మార్గం,
				 పూర్ణాంకం force,
				 నిర్మాణం worktree **worktrees,
				 స్థిరరాశి అక్షరం *cmd)
{
	నిర్మాణం worktree *wt;
	పూర్ణాంకం locked;

	ఒకవేళ (file_exists(మార్గం) && !is_ఖాళీలు_dir(మార్గం))
		die(_("'%s' already exists"), మార్గం);

	wt = కనుగొను_worktree_by_మార్గం(worktrees, మార్గం);
	ఒకవేళ (!wt)
		తిరిగి;

	locked = !!worktree_తాళం_reason(wt);
	ఒకవేళ ((!locked && force) || (locked && force > 1)) {
		ఒకవేళ (delete_git_dir(wt->id))
		    die(_("unusable worktree destination '%s'"), మార్గం);
		తిరిగి;
	}

	ఒకవేళ (locked)
		die(_("'%s' is a missing but locked worktree;\nuse '%s -f -f' to override, or 'unlock' and 'prune' or 'remove' to clear"), మార్గం, cmd);
	లేకపోతే
		die(_("'%s' is a missing but already registered worktree;\nuse '%s -f' to override, or 'prune' or 'remove' to clear"), మార్గం, cmd);
}

స్థిర శూన్య copy_sparse_checkout(స్థిరరాశి అక్షరం *worktree_git_dir)
{
	అక్షరం *from_file = repo_git_మార్గం(the_repository, "info/sparse-checkout");
	అక్షరం *గమ్యం_file = xstrfmt("%s/info/sparse-checkout", worktree_git_dir);

	ఒకవేళ (file_exists(from_file)) {
		ఒకవేళ (సురక్షిత_create_leading_directories(the_repository, గమ్యం_file) ||
			copy_file(the_repository, గమ్యం_file, from_file, 0666))
			error(_("failed to copy '%s' to '%s'; sparse-checkout may not work correctly"),
				from_file, గమ్యం_file);
	}

	విడుదల(from_file);
	విడుదల(గమ్యం_file);
}

స్థిర శూన్య copy_filtered_worktree_config(స్థిరరాశి అక్షరం *worktree_git_dir)
{
	అక్షరం *from_file = repo_git_మార్గం(the_repository, "config.worktree");
	అక్షరం *గమ్యం_file = xstrfmt("%s/config.worktree", worktree_git_dir);

	ఒకవేళ (file_exists(from_file)) {
		నిర్మాణం config_సమితి cs = { { 0 } };
		పూర్ణాంకం bare;

		ఒకవేళ (సురక్షిత_create_leading_directories(the_repository, గమ్యం_file) ||
			copy_file(the_repository, గమ్యం_file, from_file, 0666)) {
			error(_("failed to copy worktree config from '%s' to '%s'"),
				from_file, గమ్యం_file);
			వెళ్ళు worktree_copy_cleanup;
		}

		git_configset_init(&cs);
		git_configset_కూడు_file(&cs, from_file);

		ఒకవేళ (!git_configset_get_bool(&cs, "core.bare", &bare) &&
			bare &&
			repo_config_సమితి_multivar_లోపల_file_gently(the_repository,
				గమ్యం_file, "core.bare", ఖాళీ, "true", ఖాళీ, 0))
			error(_("failed to unset '%s' in '%s'"),
				"core.bare", గమ్యం_file);
		ఒకవేళ (!git_configset_get(&cs, "core.worktree") &&
			repo_config_సమితి_లోపల_file_gently(the_repository, గమ్యం_file,
						       "core.worktree", ఖాళీ, ఖాళీ))
			error(_("failed to unset '%s' in '%s'"),
				"core.worktree", గమ్యం_file);

		git_configset_clear(&cs);
	}

worktree_copy_cleanup:
	విడుదల(from_file);
	విడుదల(గమ్యం_file);
}

స్థిర పూర్ణాంకం checkout_worktree(స్థిరరాశి నిర్మాణం కూడు_opts *opts,
			     నిర్మాణం strvec *బిడ్డ_env)
{
	నిర్మాణం బిడ్డ_process cp = బిడ్డ_PROCESS_INIT;
	cp.git_cmd = 1;
	strvec_pushl(&cp.వాదనలు, "reset", "--hard", "--no-recurse-submodules", ఖాళీ);
	ఒకవేళ (opts->quiet)
		strvec_నెట్టు(&cp.వాదనలు, "--quiet");
	strvec_pushv(&cp.env, బిడ్డ_env->v);
	తిరిగి నిరంతరం_command(&cp);
}

స్థిర పూర్ణాంకం make_worktree_orphan(స్థిరరాశి అక్షరం * ref, స్థిరరాశి నిర్మాణం కూడు_opts *opts,
				నిర్మాణం strvec *బిడ్డ_env)
{
	నిర్మాణం strbuf symref = STRBUF_INIT;
	నిర్మాణం బిడ్డ_process cp = బిడ్డ_PROCESS_INIT;

	validate_new_branchname(ref, &symref, 0);
	strvec_pushl(&cp.వాదనలు, "symbolic-ref", "HEAD", symref.buf, ఖాళీ);
	ఒకవేళ (opts->quiet)
		strvec_నెట్టు(&cp.వాదనలు, "--quiet");
	strvec_pushv(&cp.env, బిడ్డ_env->v);
	strbuf_release(&symref);
	cp.git_cmd = 1;
	తిరిగి నిరంతరం_command(&cp);
}

/*
 * References for worktrees are generally stored in '$GIT_DIR/worktrees/<wt_id>'.
 * But when using alternate reference directories, we want to store the worktree
 * references in '$ALTERNATE_REFERENCE_DIR/worktrees/<wt_id>'.
 *
 * Create the necessary folder structure to facilitate the same. But to ensure
 * that the former path is still considered a Git directory, add stubs.
 */
స్థిర శూన్య అమరిక_alternate_ref_dir(నిర్మాణం worktree *wt, స్థిరరాశి అక్షరం *wt_git_మార్గం)
{
	నిర్మాణం strbuf sb = STRBUF_INIT;
	అక్షరం *మార్గం;

	మార్గం = wt->repo->ref_storage_payload;
	ఒకవేళ (!మార్గం)
		తిరిగి;

	ఒకవేళ (!is_absolute_మార్గం(మార్గం))
		strbuf_addf(&sb, "%s/", wt->repo->commondir);

	strbuf_addf(&sb, "%s/worktrees", మార్గం);
	సురక్షిత_create_dir(wt->repo, sb.buf, 1);
	strbuf_addf(&sb, "/%s", wt->id);
	సురక్షిత_create_dir(wt->repo, sb.buf, 1);
	strbuf_reset(&sb);

	strbuf_addf(&sb, "this worktree stores references in %s/worktrees/%s",
		    మార్గం, wt->id);
	refs_create_refdir_stubs(wt->repo, wt_git_మార్గం, sb.buf);

	strbuf_release(&sb);
}

స్థిర పూర్ణాంకం కూడు_worktree(స్థిరరాశి అక్షరం *మార్గం, స్థిరరాశి అక్షరం *refname,
			స్థిరరాశి నిర్మాణం కూడు_opts *opts)
{
	నిర్మాణం strbuf sb_git = STRBUF_INIT, sb_repo = STRBUF_INIT;
	నిర్మాణం strbuf sb = STRBUF_INIT;
	స్థిరరాశి అక్షరం *name;
	అక్షరం *name_గమ్యం_free = ఖాళీ;
	నిర్మాణం strvec బిడ్డ_env = STRVEC_INIT;
	గుర్తులేని పూర్ణాంకం counter = 0;
	పూర్ణాంకం len, ret;
	నిర్మాణం strbuf symref = STRBUF_INIT;
	నిర్మాణం commit *commit = ఖాళీ;
	పూర్ణాంకం is_branch = 0;
	నిర్మాణం strbuf sb_name = STRBUF_INIT;
	నిర్మాణం worktree **worktrees, *wt = ఖాళీ;
	నిర్మాణం ref_store *wt_refs;
	నిర్మాణం repo_config_విలువలు *cfg = repo_config_విలువలు(the_repository);

	worktrees = get_worktrees(the_repository);
	check_candidate_మార్గం(మార్గం, opts->force, worktrees, "add");
	free_worktrees(worktrees);
	worktrees = ఖాళీ;

	/* is 'refname' a branch or commit? */
	ఒకవేళ (!opts->detach && !check_branch_ref(the_repository, &symref, refname) &&
	    refs_ref_exists(get_main_ref_store(the_repository), symref.buf)) {
		is_branch = 1;
		ఒకవేళ (!opts->force)
			die_if_checked_బహిర్గతం(symref.buf, 0);
	}
	commit = lookup_commit_reference_by_name(refname);
	ఒకవేళ (!commit && !opts->orphan)
		die(_("invalid reference: %s"), refname);

	name = name_గమ్యం_free = worktree_basename_dup(మార్గం);
	ఒకవేళ (!*name)
		die(_("invalid path '%s'"), మార్గం);
	sanitize_refname_component(name, &sb_name);
	ఒకవేళ (!sb_name.len)
		BUG("How come '%s' becomes empty after sanitization?", name);
	name = sb_name.buf;
	repo_git_మార్గం_replace(the_repository, &sb_repo, "worktrees/%s", name);
	len = sb_repo.len;
	ఒకవేళ (సురక్షిత_create_leading_directories_const(the_repository, sb_repo.buf))
		die_errno(_("could not create leading directories of '%s'"),
			  sb_repo.buf);

	అయితే (mkdir(sb_repo.buf, 0777)) {
		counter++;
		ఒకవేళ ((errno != EEXIST) || !counter /*అధికప్రవాహం*/)
			die_errno(_("could not create directory of '%s'"),
				  sb_repo.buf);
		strbuf_setlen(&sb_repo, len);
		strbuf_addf(&sb_repo, "%d", counter);
	}
	name = strrchr(sb_repo.buf, '/') + 1;

	junk_pid = getpid();
	atexit(remove_junk);
	sigchain_నెట్టు_common(remove_junk_on_signal);

	junk_git_dir = xstrdup(sb_repo.buf);
	is_junk = 1;

	/*
	 * lock the incomplete repo so prune won't delete it, unlock
	 * after the preparation is over.
	 */
	strbuf_addf(&sb, "%s/locked", sb_repo.buf);
	ఒకవేళ (opts->keep_locked)
		write_file(sb.buf, "%s", opts->keep_locked);
	లేకపోతే
		write_file(sb.buf, _("initializing"));

	strbuf_addf(&sb_git, "%s/.git", మార్గం);
	ఒకవేళ (సురక్షిత_create_leading_directories_const(the_repository, sb_git.buf))
		die_errno(_("could not create leading directories of '%s'"),
			  sb_git.buf);
	junk_work_వృక్షం = xstrdup(మార్గం);

	strbuf_reset(&sb);
	strbuf_addf(&sb, "%s/gitdir", sb_repo.buf);
	write_worktree_linking_files(the_repository, sb_git.buf,
				     sb.buf, opts->relative_paths);
	strbuf_reset(&sb);
	strbuf_addf(&sb, "%s/commondir", sb_repo.buf);
	write_file(sb.buf, "../..");

	/*
	 * Set up the ref store of the worktree and create the HEAD reference.
	 */
	wt = get_linked_worktree(the_repository, name, 1);
	ఒకవేళ (!wt) {
		ret = error(_("could not find created worktree '%s'"), name);
		వెళ్ళు పూర్తి;
	}
	అమరిక_alternate_ref_dir(wt, sb_repo.buf);
	wt_refs = get_worktree_ref_store(wt);

	ret = ref_store_create_on_disk(wt_refs, REF_STORE_CREATE_ON_DISK_IS_WORKTREE, &sb);
	ఒకవేళ (ret)
		వెళ్ళు పూర్తి;

	ఒకవేళ (!is_branch && commit)
		ret = refs_update_ref(wt_refs, ఖాళీ, "HEAD", &commit->object.oid,
				      ఖాళీ, 0, UPDATE_REFS_సందేశం_ON_ERR);
	లేకపోతే
		ret = refs_update_symref(wt_refs, "HEAD", symref.buf, ఖాళీ);
	ఒకవేళ (ret)
		వెళ్ళు పూర్తి;

	/*
	 * If the current worktree has sparse-checkout enabled, then copy
	 * the sparse-checkout patterns from the current worktree.
	 */
	ఒకవేళ (cfg->apply_sparse_checkout)
		copy_sparse_checkout(sb_repo.buf);

	/*
	 * If we are using worktree config, then copy all current config
	 * values from the current worktree into the new one, that way the
	 * new worktree behaves the same as this one.
	 */
	ఒకవేళ (the_repository->repository_format_worktree_config)
		copy_filtered_worktree_config(sb_repo.buf);

	strvec_pushf(&బిడ్డ_env, "%s=%s", GIT_DIR_ENVIRONMENT, sb_git.buf);
	strvec_pushf(&బిడ్డ_env, "%s=%s", GIT_WORK_వృక్షం_ENVIRONMENT, మార్గం);

	ఒకవేళ (opts->orphan &&
	    (ret = make_worktree_orphan(refname, opts, &బిడ్డ_env)))
		వెళ్ళు పూర్తి;

	ఒకవేళ (opts->checkout &&
	    (ret = checkout_worktree(opts, &బిడ్డ_env)))
		వెళ్ళు పూర్తి;

	is_junk = 0;
	FREE_AND_NULL(junk_work_వృక్షం);
	FREE_AND_NULL(junk_git_dir);

పూర్తి:
	ఒకవేళ (ret || !opts->keep_locked) {
		strbuf_reset(&sb);
		strbuf_addf(&sb, "%s/locked", sb_repo.buf);
		unlink_or_warn(sb.buf);
	}

	/*
	 * Hook failure does not warrant worktree deletion, so run hook after
	 * is_junk is cleared, but do return appropriate code when hook fails.
	 */
	ఒకవేళ (!ret && opts->checkout && !opts->orphan) {
		నిర్మాణం నిరంతరం_hooks_opt opt = నిరంతరం_HOOKS_OPT_INIT_FORCE_SERIAL;

		strvec_pushl(&opt.env, "GIT_DIR", "GIT_WORK_TREE", ఖాళీ);
		strvec_pushl(&opt.వాదనలు,
			     oid_గమ్యం_hex(null_oid(the_hash_algo)),
			     oid_గమ్యం_hex(&commit->object.oid),
			     "1",
			     ఖాళీ);
		opt.dir = మార్గం;

		ret = నిరంతరం_hooks_opt(the_repository, "post-checkout", &opt);
	}

	strvec_clear(&బిడ్డ_env);
	strbuf_release(&sb);
	strbuf_release(&symref);
	strbuf_release(&sb_repo);
	strbuf_release(&sb_git);
	strbuf_release(&sb_name);
	free_worktree(wt);
	విడుదల(name_గమ్యం_free);
	తిరిగి ret;
}

స్థిర శూన్య print_preparing_worktree_పంక్తి(పూర్ణాంకం detach,
					  స్థిరరాశి అక్షరం *branch,
					  స్థిరరాశి అక్షరం *new_branch,
					  పూర్ణాంకం force_new_branch)
{
	ఒకవేళ (force_new_branch) {
		నిర్మాణం commit *commit = lookup_commit_reference_by_name(new_branch);
		ఒకవేళ (!commit)
			fprintf_ln(stderr, _("Preparing worktree (new branch '%s')"), new_branch);
		లేకపోతే
			fprintf_ln(stderr, _("Preparing worktree (resetting branch '%s'; was at %s)"),
				  new_branch,
				  repo_కనుగొను_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV));
	} లేకపోతే ఒకవేళ (new_branch) {
		fprintf_ln(stderr, _("Preparing worktree (new branch '%s')"), new_branch);
	} లేకపోతే {
		నిర్మాణం strbuf s = STRBUF_INIT;
		ఒకవేళ (!detach && !check_branch_ref(the_repository, &s, branch) &&
		    refs_ref_exists(get_main_ref_store(the_repository), s.buf))
			fprintf_ln(stderr, _("Preparing worktree (checking out '%s')"),
				  branch);
		లేకపోతే {
			నిర్మాణం commit *commit = lookup_commit_reference_by_name(branch);
			ఒకవేళ (!commit)
				BUG("unreachable: invalid reference: %s", branch);
			fprintf_ln(stderr, _("Preparing worktree (detached HEAD %s)"),
				  repo_కనుగొను_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV));
		}
		strbuf_release(&s);
	}
}

/**
 * Callback to short circuit iteration over refs on the first reference
 * corresponding to a valid oid.
 *
 * Returns 0 on failure and non-zero on success.
 */
స్థిర పూర్ణాంకం మొదటి_valid_ref(స్థిరరాశి నిర్మాణం reference *ref వాడని, శూన్య *cb_దత్తాంశం వాడని)
{
	తిరిగి 1;
}

/**
 * Verifies HEAD and determines whether there exist any valid local references.
 *
 * - Checks whether HEAD points to a valid reference.
 *
 * - Checks whether any valid local branches exist.
 *
 * - Emits a warning if there exist any valid branches but HEAD does not point
 *   to a valid reference.
 *
 * Returns 1 if any of the previous checks are true, otherwise returns 0.
 */
స్థిర పూర్ణాంకం can_use_local_refs(స్థిరరాశి నిర్మాణం కూడు_opts *opts)
{
	ఒకవేళ (refs_తల_ref(get_main_ref_store(the_repository), మొదటి_valid_ref, ఖాళీ)) {
		తిరిగి 1;
	} లేకపోతే ఒకవేళ (refs_for_each_branch_ref(get_main_ref_store(the_repository), మొదటి_valid_ref, ఖాళీ)) {
		ఒకవేళ (!opts->quiet)
			warning(_("HEAD points to an invalid (or orphaned) reference.\n"));
		తిరిగి 1;
	}
	తిరిగి 0;
}

/**
 * Reports whether the necessary flags were set and whether the repository has
 * remote references to attempt DWIM tracking of upstream branches.
 *
 * 1. Checks that `--guess-remote` was used or `worktree.guessRemote = true`.
 *
 * 2. Checks whether any valid remote branches exist.
 *
 * 3. Checks that there exists at least one remote and emits a warning/error
 *    if both checks 1. and 2. are false (can be bypassed with `--force`).
 *
 * Returns 1 if checks 1. and 2. are true, otherwise 0.
 */
స్థిర పూర్ణాంకం can_use_remote_refs(స్థిరరాశి నిర్మాణం కూడు_opts *opts)
{
	ఒకవేళ (!guess_remote) {
		తిరిగి 0;
	} లేకపోతే ఒకవేళ (refs_for_each_remote_ref(get_main_ref_store(the_repository), మొదటి_valid_ref, ఖాళీ)) {
		తిరిగి 1;
	} లేకపోతే ఒకవేళ (!opts->force && remote_get(ఖాళీ)) {
		die(_("No local or remote refs exist despite at least one remote\n"
		      "present, stopping; use 'add -f' to override or fetch a remote first"));
	}
	తిరిగి 0;
}

/**
 * Determines whether `--orphan` should be inferred in the evaluation of
 * `worktree add path/` or `worktree add -b branch path/` and emits an error
 * if the supplied arguments would produce an illegal combination when the
 * `--orphan` flag is included.
 *
 * `opts` and `opt_track` contain the other options & flags supplied to the
 * command.
 *
 * remote determines whether to check `can_use_remote_refs()` or not. This
 * is primarily to differentiate between the basic `add` DWIM and `add -b`.
 *
 * Returns 1 when inferring `--orphan`, 0 otherwise, and emits an error when
 * `--orphan` is inferred but doing so produces an illegal combination of
 * options and flags. Additionally produces an error when remote refs are
 * checked and the repo is in a state that looks like the user added a remote
 * but forgot to fetch (and did not override the warning with -f).
 */
స్థిర పూర్ణాంకం dwim_orphan(స్థిరరాశి నిర్మాణం కూడు_opts *opts, పూర్ణాంకం opt_track, పూర్ణాంకం remote)
{
	ఒకవేళ (can_use_local_refs(opts)) {
		తిరిగి 0;
	} లేకపోతే ఒకవేళ (remote && can_use_remote_refs(opts)) {
		తిరిగి 0;
	} లేకపోతే ఒకవేళ (!opts->quiet) {
		fprintf_ln(stderr, WORKTREE_కూడు_DWIM_ORPHAN_INFER_పాఠ్యం);
	}

	ఒకవేళ (opt_track) {
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--track");
	} లేకపోతే ఒకవేళ (!opts->checkout) {
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--no-checkout");
	}
	తిరిగి 1;
}

స్థిర శూన్య advise_disambiguating_remotes(స్థిరరాశి అక్షరం *మార్గం, స్థిరరాశి అక్షరం *branch,
					  స్థిరరాశి నిర్మాణం string_list *matched_remote_పేర్లు)
{
	నిర్మాణం string_list_అంశం *అంశం;

	advise(_("Branch name '%s' appears in multiple remotes:"), branch);
	for_each_string_list_అంశం(అంశం, matched_remote_పేర్లు) {
		advise(_("  %s"), అంశం->string);
	}
	advise(_("If you meant to create a worktree from a remote tracking branch on\n"
		 "<remote>, you can do so by:\n"
		 "\n"
		 "    git worktree add -b %s %s <remote>/%s\n"
		 "\n"
		 "If you'd like to always prefer some remote, e.g. 'origin',\n"
		 "consider setting checkout.defaultRemote=origin in your config."),
	       branch, మార్గం, branch);
}

స్థిర అక్షరం *dwim_branch(స్థిరరాశి నిర్మాణం కూడు_opts *opts, స్థిరరాశి అక్షరం *మార్గం, అక్షరం **new_branch)
{
	పూర్ణాంకం branch_exists;
	అక్షరం *branchname = worktree_basename_dup(మార్గం);
	నిర్మాణం strbuf ref = STRBUF_INIT;

	branch_exists = !check_branch_ref(the_repository, &ref, branchname) &&
			refs_ref_exists(get_main_ref_store(the_repository),
					ref.buf);
	strbuf_release(&ref);
	ఒకవేళ (branch_exists)
		తిరిగి branchname;

	*new_branch = branchname;
	ఒకవేళ (guess_remote) {
		నిర్మాణం object_id oid;
		అక్షరం *remote;
		పూర్ణాంకం అంకె_matches = 0;
		నిర్మాణం string_list matched_remote_పేర్లు = STRING_LIST_INIT_DUP;

		remote = unique_tracking_name(*new_branch, &oid, &అంకె_matches,
					      &matched_remote_పేర్లు);
		ఒకవేళ (!remote && అంకె_matches > 1) {
			ఒకవేళ (!opts->quiet &&
			    advice_enabled(ADVICE_CHECKOUT_AMBIGUOUS_REMOTE_BRANCH_NAME))
				advise_disambiguating_remotes(మార్గం, *new_branch,
							      &matched_remote_పేర్లు);
			die(_("'%s' matched multiple (%d) remote tracking branches"),
			    *new_branch, అంకె_matches);
		}
		string_list_clear(&matched_remote_పేర్లు, 0);
		తిరిగి remote;
	}
	తిరిగి ఖాళీ;
}

స్థిర పూర్ణాంకం కూడు(పూర్ణాంకం ac, స్థిరరాశి అక్షరం **av, స్థిరరాశి అక్షరం *ఉపసర్గ,
	       నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం కూడు_opts opts;
	స్థిరరాశి అక్షరం *new_branch_force = ఖాళీ;
	అక్షరం *మార్గం;
	స్థిరరాశి అక్షరం *branch;
	అక్షరం *branch_గమ్యం_free = ఖాళీ;
	అక్షరం *new_branch_గమ్యం_free = ఖాళీ;
	స్థిరరాశి అక్షరం *new_branch = ఖాళీ;
	అక్షరం *opt_track = ఖాళీ;
	స్థిరరాశి అక్షరం *తాళం_reason = ఖాళీ;
	పూర్ణాంకం keep_locked = 0;
	పూర్ణాంకం వాడిన_new_branch_options;
	నిర్మాణం option options[] = {
		OPT__FORCE(&opts.force,
			   N_("checkout <branch> even if already checked out in other worktree"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_STRING('b', ఖాళీ, &new_branch, N_("branch"),
			   N_("create a new branch")),
		OPT_STRING('B', ఖాళీ, &new_branch_force, N_("branch"),
			   N_("create or reset a branch")),
		OPT_BOOL(0, "orphan", &opts.orphan, N_("create unborn branch")),
		OPT_BOOL('d', "detach", &opts.detach, N_("detach HEAD at named commit")),
		OPT_BOOL(0, "checkout", &opts.checkout, N_("populate the new working tree")),
		OPT_BOOL(0, "lock", &keep_locked, N_("keep the new working tree locked")),
		OPT_STRING(0, "reason", &తాళం_reason, N_("string"),
			   N_("reason for locking")),
		OPT__QUIET(&opts.quiet, N_("suppress progress reporting")),
		OPT_PASSTHRU(0, "track", &opt_track, ఖాళీ,
			     N_("set up tracking mode (see git-branch(1))"),
			     PARSE_OPT_NOARG | PARSE_OPT_OPTARG),
		OPT_BOOL(0, "guess-remote", &guess_remote,
			 N_("try to match the new branch name with a remote-tracking branch")),
		OPT_BOOL(0, "relative-paths", &opts.relative_paths,
			 N_("use relative paths for worktrees")),
		OPT_చివర()
	};
	పూర్ణాంకం ret;

	memset(&opts, 0, పరిమాణం(opts));
	opts.checkout = 1;
	opts.relative_paths = use_relative_paths;
	ac = parse_options(ac, av, ఉపసర్గ, options, git_worktree_కూడు_usage, 0);
	ఒకవేళ (!!opts.detach + !!new_branch + !!new_branch_force > 1)
		die(_("options '%s', '%s', and '%s' cannot be used together"), "-b", "-B", "--detach");
	ఒకవేళ (opts.detach && opts.orphan)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--detach");
	ఒకవేళ (opts.orphan && opt_track)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--track");
	ఒకవేళ (opts.orphan && !opts.checkout)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--no-checkout");
	ఒకవేళ (opts.orphan && ac == 2)
		die(_("option '%s' and commit-ish cannot be used together"),
		    "--orphan");
	ఒకవేళ (తాళం_reason && !keep_locked)
		die(_("the option '%s' requires '%s'"), "--reason", "--lock");
	ఒకవేళ (తాళం_reason)
		opts.keep_locked = తాళం_reason;
	లేకపోతే ఒకవేళ (keep_locked)
		opts.keep_locked = _("added with --lock");

	ఒకవేళ (ac < 1 || ac > 2)
		usage_with_options(git_worktree_కూడు_usage, options);

	మార్గం = ఉపసర్గ_filename(ఉపసర్గ, av[0]);
	branch = ac < 2 ? "HEAD" : av[1];
	వాడిన_new_branch_options = new_branch || new_branch_force;

	ఒకవేళ (!strcmp(branch, "-"))
		branch = "@{-1}";

	ఒకవేళ (new_branch_force) {
		నిర్మాణం strbuf symref = STRBUF_INIT;

		new_branch = new_branch_force;

		ఒకవేళ (!opts.force &&
		    !check_branch_ref(the_repository, &symref, new_branch) &&
		    refs_ref_exists(get_main_ref_store(the_repository), symref.buf))
			die_if_checked_బహిర్గతం(symref.buf, 0);
		strbuf_release(&symref);
	}

	ఒకవేళ (opts.orphan && !new_branch) {
		new_branch = new_branch_గమ్యం_free = worktree_basename_dup(మార్గం);
	} లేకపోతే ఒకవేళ (opts.orphan) {
		; /* no-op */
	} లేకపోతే ఒకవేళ (opts.detach) {
		/* Check HEAD */
		ఒకవేళ (!strcmp(branch, "HEAD"))
			can_use_local_refs(&opts);
	} లేకపోతే ఒకవేళ (ac < 2 && new_branch) {
		/* DWIM: Infer --orphan when repo has no refs. */
		opts.orphan = dwim_orphan(&opts, !!opt_track, 0);
	} లేకపోతే ఒకవేళ (ac < 2) {
		/* DWIM: Guess branch name from path. */
		అక్షరం *s = dwim_branch(&opts, మార్గం, &new_branch_గమ్యం_free);
		ఒకవేళ (s)
			branch = branch_గమ్యం_free = s;
		new_branch = new_branch_గమ్యం_free;

		/* DWIM: Infer --orphan when repo has no refs. */
		opts.orphan = (!s) && dwim_orphan(&opts, !!opt_track, 1);
	} లేకపోతే ఒకవేళ (ac == 2 && new_branch) {
		ఒకవేళ (!strcmp(branch, "HEAD"))
			can_use_local_refs(&opts);
	} లేకపోతే ఒకవేళ (ac == 2) {
		నిర్మాణం commit *commit;

		commit = lookup_commit_reference_by_name(branch);
		ఒకవేళ (!commit) {
			నిర్మాణం object_id oid;
			అక్షరం *remote;
			పూర్ణాంకం అంకె_matches = 0;
			నిర్మాణం string_list matched_remote_పేర్లు = STRING_LIST_INIT_DUP;

			remote = unique_tracking_name(branch, &oid, &అంకె_matches,
						      &matched_remote_పేర్లు);
			ఒకవేళ (remote) {
				new_branch = branch;
				branch = new_branch_గమ్యం_free = remote;
			} లేకపోతే ఒకవేళ (అంకె_matches > 1) {
				ఒకవేళ (!opts.quiet &&
				    advice_enabled(ADVICE_CHECKOUT_AMBIGUOUS_REMOTE_BRANCH_NAME))
					advise_disambiguating_remotes(మార్గం, branch,
								      &matched_remote_పేర్లు);
				die(_("'%s' matched multiple (%d) remote tracking branches"),
				    branch, అంకె_matches);
			}
			string_list_clear(&matched_remote_పేర్లు, 0);
		}

		ఒకవేళ (!strcmp(branch, "HEAD"))
			can_use_local_refs(&opts);

	}

	ఒకవేళ (!opts.orphan && !lookup_commit_reference_by_name(branch)) {
		పూర్ణాంకం attempt_hint = !opts.quiet && (ac < 2);
		ఒకవేళ (attempt_hint && వాడిన_new_branch_options) {
			advise_if_enabled(ADVICE_WORKTREE_కూడు_ORPHAN,
				WORKTREE_కూడు_ORPHAN_WITH_DASH_B_HINT_పాఠ్యం,
				new_branch, మార్గం);
		} లేకపోతే ఒకవేళ (attempt_hint) {
			advise_if_enabled(ADVICE_WORKTREE_కూడు_ORPHAN,
				WORKTREE_కూడు_ORPHAN_NO_DASH_B_HINT_పాఠ్యం, మార్గం);
		}
		die(_("invalid reference: %s"), branch);
	}

	ఒకవేళ (!opts.quiet)
		print_preparing_worktree_పంక్తి(opts.detach, branch, new_branch, !!new_branch_force);

	ఒకవేళ (opts.orphan) {
		branch = new_branch;
	} లేకపోతే ఒకవేళ (new_branch) {
		నిర్మాణం బిడ్డ_process cp = బిడ్డ_PROCESS_INIT;
		cp.git_cmd = 1;
		strvec_నెట్టు(&cp.వాదనలు, "branch");
		ఒకవేళ (new_branch_force)
			strvec_నెట్టు(&cp.వాదనలు, "--force");
		ఒకవేళ (opts.quiet)
			strvec_నెట్టు(&cp.వాదనలు, "--quiet");
		strvec_నెట్టు(&cp.వాదనలు, new_branch);
		strvec_నెట్టు(&cp.వాదనలు, branch);
		ఒకవేళ (opt_track)
			strvec_నెట్టు(&cp.వాదనలు, opt_track);
		ఒకవేళ (నిరంతరం_command(&cp)) {
			ret = -1;
			వెళ్ళు cleanup;
		}
		branch = new_branch;
	} లేకపోతే ఒకవేళ (opt_track) {
		die(_("--[no-]track can only be used if a new branch is created"));
	}

	ret = కూడు_worktree(మార్గం, branch, &opts);
cleanup:
	విడుదల(మార్గం);
	విడుదల(opt_track);
	విడుదల(branch_గమ్యం_free);
	విడుదల(new_branch_గమ్యం_free);
	తిరిగి ret;
}

స్థిర శూన్య show_worktree_porcelain(నిర్మాణం worktree *wt, పూర్ణాంకం పంక్తి_terminator)
{
	స్థిరరాశి అక్షరం *reason;

	రాయి("worktree %s%c", wt->మార్గం, పంక్తి_terminator);
	ఒకవేళ (wt->is_bare)
		రాయి("bare%c", పంక్తి_terminator);
	లేకపోతే {
		రాయి("HEAD %s%c", oid_గమ్యం_hex(&wt->తల_oid), పంక్తి_terminator);
		ఒకవేళ (wt->is_detached)
			రాయి("detached%c", పంక్తి_terminator);
		లేకపోతే ఒకవేళ (wt->తల_ref)
			రాయి("branch %s%c", wt->తల_ref, పంక్తి_terminator);
	}

	reason = worktree_తాళం_reason(wt);
	ఒకవేళ (reason) {
		fputs("locked", stdout);
		ఒకవేళ (*reason) {
			fputc(' ', stdout);
			write_name_quoted(reason, stdout, పంక్తి_terminator);
		} లేకపోతే {
			fputc(పంక్తి_terminator, stdout);
		}
	}

	reason = worktree_prune_reason(wt, expire);
	ఒకవేళ (reason)
		రాయి("prunable %s%c", reason, పంక్తి_terminator);

	fputc(పంక్తి_terminator, stdout);
}

నిర్మాణం worktree_display {
	అక్షరం *మార్గం;
	పూర్ణాంకం వెడల్పు;
};

స్థిర శూన్య show_worktree(నిర్మాణం worktree *wt, నిర్మాణం worktree_display *display,
			  పూర్ణాంకం మార్గం_maxwidth, పూర్ణాంకం abbrev_len)
{
	నిర్మాణం strbuf sb = STRBUF_INIT;
	స్థిరరాశి అక్షరం *reason;

	strbuf_addf(&sb, "%s%*s", display->మార్గం, 1 + మార్గం_maxwidth - display->వెడల్పు, "");
	ఒకవేళ (wt->is_bare)
		strbuf_addstr(&sb, "(bare)");
	లేకపోతే {
		strbuf_addf(&sb, "%-*s ", abbrev_len,
				repo_కనుగొను_unique_abbrev(the_repository, &wt->తల_oid, DEFAULT_ABBREV));
		ఒకవేళ (wt->is_detached)
			strbuf_addstr(&sb, "(detached HEAD)");
		లేకపోతే ఒకవేళ (wt->తల_ref) {
			అక్షరం *ref = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
								 wt->తల_ref,
								 0);
			strbuf_addf(&sb, "[%s]", ref);
			విడుదల(ref);
		} లేకపోతే
			strbuf_addstr(&sb, "(error)");
	}

	reason = worktree_తాళం_reason(wt);
	ఒకవేళ (verbose && reason && *reason)
		strbuf_addf(&sb, "\n\tlocked: %s", reason);
	లేకపోతే ఒకవేళ (reason)
		strbuf_addstr(&sb, " locked");

	reason = worktree_prune_reason(wt, expire);
	ఒకవేళ (verbose && reason)
		strbuf_addf(&sb, "\n\tprunable: %s", reason);
	లేకపోతే ఒకవేళ (reason)
		strbuf_addstr(&sb, " prunable");

	రాయి("%s\n", sb.buf);
	strbuf_release(&sb);
}

స్థిర శూన్య measure_widths(నిర్మాణం worktree **wt, పూర్ణాంకం *abbrev,
			   నిర్మాణం worktree_display **d, పూర్ణాంకం *maxwidth)
{
	పూర్ణాంకం i, display_alloc = 0;
	నిర్మాణం worktree_display *display = ఖాళీ;
	నిర్మాణం strbuf buf = STRBUF_INIT;

	కొరకు (i = 0; wt[i]; i++) {
		పూర్ణాంకం sha1_len;
		ALLOC_GROW(display, i + 1, display_alloc);
		quote_మార్గం(wt[i]->మార్గం, ఖాళీ, &buf, 0);
		display[i].వెడల్పు = utf8_strwidth(buf.buf);
		display[i].మార్గం = strbuf_detach(&buf, ఖాళీ);

		ఒకవేళ (display[i].వెడల్పు > *maxwidth)
			*maxwidth = display[i].వెడల్పు;
		sha1_len = అక్షరశ్రేణి_పొడవు(repo_కనుగొను_unique_abbrev(the_repository, &wt[i]->తల_oid, *abbrev));
		ఒకవేళ (sha1_len > *abbrev)
			*abbrev = sha1_len;
	}
	*d = display;
}

స్థిర పూర్ణాంకం pathcmp(స్థిరరాశి శూన్య *a_, స్థిరరాశి శూన్య *b_)
{
	స్థిరరాశి నిర్మాణం worktree *స్థిరరాశి *a = a_;
	స్థిరరాశి నిర్మాణం worktree *స్థిరరాశి *b = b_;
	తిరిగి fspathcmp((*a)->మార్గం, (*b)->మార్గం);
}

స్థిర శూన్య pathsort(నిర్మాణం worktree **wt)
{
	పూర్ణాంకం n = 0;
	నిర్మాణం worktree **p = wt;

	అయితే (*p++)
		n++;
	QSORT(wt, n, pathcmp);
}

స్థిర పూర్ణాంకం list(పూర్ణాంకం ac, స్థిరరాశి అక్షరం **av, స్థిరరాశి అక్షరం *ఉపసర్గ,
		నిర్మాణం repository *repo వాడని)
{
	పూర్ణాంకం porcelain = 0;
	పూర్ణాంకం పంక్తి_terminator = '\n';

	నిర్మాణం option options[] = {
		OPT_BOOL(0, "porcelain", &porcelain, N_("machine-readable output")),
		OPT__VERBOSE(&verbose, N_("show extended annotations and reasons, if available")),
		OPT_EXPIRY_DATE(0, "expire", &expire,
				N_("add 'prunable' annotation to missing worktrees older than <time>")),
		OPT_సమితి_INT('z', ఖాళీ, &పంక్తి_terminator,
			    N_("terminate records with a NUL character"), '\0'),
		OPT_చివర()
	};

	expire = సమయం_MAX;
	ac = parse_options(ac, av, ఉపసర్గ, options, git_worktree_list_usage, 0);
	ఒకవేళ (ac)
		usage_with_options(git_worktree_list_usage, options);
	లేకపోతే ఒకవేళ (verbose && porcelain)
		die(_("options '%s' and '%s' cannot be used together"), "--verbose", "--porcelain");
	లేకపోతే ఒకవేళ (!పంక్తి_terminator && !porcelain)
		die(_("the option '%s' requires '%s'"), "-z", "--porcelain");
	లేకపోతే {
		నిర్మాణం worktree **worktrees = get_worktrees(the_repository);
		పూర్ణాంకం మార్గం_maxwidth = 0, abbrev = DEFAULT_ABBREV, i;
		నిర్మాణం worktree_display *display = ఖాళీ;

		/* sort worktrees by path but keep main worktree at top */
		pathsort(worktrees + 1);

		ఒకవేళ (!porcelain)
			measure_widths(worktrees, &abbrev,
				       &display, &మార్గం_maxwidth);

		కొరకు (i = 0; worktrees[i]; i++) {
			ఒకవేళ (porcelain)
				show_worktree_porcelain(worktrees[i],
							పంక్తి_terminator);
			లేకపోతే
				show_worktree(worktrees[i],
					      &display[i], మార్గం_maxwidth, abbrev);
		}
		కొరకు (i = 0; display && worktrees[i]; i++)
			విడుదల(display[i].మార్గం);
		విడుదల(display);
		free_worktrees(worktrees);
	}
	తిరిగి 0;
}

స్థిర పూర్ణాంకం తాళం_worktree(పూర్ణాంకం ac, స్థిరరాశి అక్షరం **av, స్థిరరాశి అక్షరం *ఉపసర్గ,
			 నిర్మాణం repository *repo వాడని)
{
	స్థిరరాశి అక్షరం *reason = "", *old_reason;
	నిర్మాణం option options[] = {
		OPT_STRING(0, "reason", &reason, N_("string"),
			   N_("reason for locking")),
		OPT_చివర()
	};
	నిర్మాణం worktree **worktrees, *wt;
	అక్షరం *మార్గం;

	ac = parse_options(ac, av, ఉపసర్గ, options, git_worktree_తాళం_usage, 0);
	ఒకవేళ (ac != 1)
		usage_with_options(git_worktree_తాళం_usage, options);

	worktrees = get_worktrees(the_repository);
	wt = కనుగొను_worktree(worktrees, ఉపసర్గ, av[0]);
	ఒకవేళ (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	ఒకవేళ (is_main_worktree(wt))
		die(_("The main working tree cannot be locked or unlocked"));

	old_reason = worktree_తాళం_reason(wt);
	ఒకవేళ (old_reason) {
		ఒకవేళ (*old_reason)
			die(_("'%s' is already locked, reason: %s"),
			    av[0], old_reason);
		die(_("'%s' is already locked"), av[0]);
	}

	మార్గం = repo_common_మార్గం(the_repository, "worktrees/%s/locked", wt->id);
	write_file(మార్గం, "%s", reason);

	free_worktrees(worktrees);
	విడుదల(మార్గం);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం unlock_worktree(పూర్ణాంకం ac, స్థిరరాశి అక్షరం **av, స్థిరరాశి అక్షరం *ఉపసర్గ,
			   నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం option options[] = {
		OPT_చివర()
	};
	నిర్మాణం worktree **worktrees, *wt;
	అక్షరం *మార్గం;
	పూర్ణాంకం ret;

	ac = parse_options(ac, av, ఉపసర్గ, options, git_worktree_unlock_usage, 0);
	ఒకవేళ (ac != 1)
		usage_with_options(git_worktree_unlock_usage, options);

	worktrees = get_worktrees(the_repository);
	wt = కనుగొను_worktree(worktrees, ఉపసర్గ, av[0]);
	ఒకవేళ (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	ఒకవేళ (is_main_worktree(wt))
		die(_("The main working tree cannot be locked or unlocked"));
	ఒకవేళ (!worktree_తాళం_reason(wt))
		die(_("'%s' is not locked"), av[0]);

	మార్గం = repo_common_మార్గం(the_repository, "worktrees/%s/locked", wt->id);
	ret = unlink_or_warn(మార్గం);

	free_worktrees(worktrees);
	విడుదల(మార్గం);
	తిరిగి ret;
}

స్థిర శూన్య validate_no_submodules(స్థిరరాశి నిర్మాణం worktree *wt)
{
	నిర్మాణం సూచీ_state istate = సూచీ_STATE_INIT(the_repository);
	నిర్మాణం strbuf మార్గం = STRBUF_INIT;
	పూర్ణాంకం i, దొరికింది_submodules = 0;
	అక్షరం *wt_gitdir;

	wt_gitdir = get_worktree_git_dir(wt);

	ఒకవేళ (is_directory(worktree_git_మార్గం(wt, "modules"))) {
		/*
		 * There could be false positives, e.g. the "modules"
		 * directory exists but is empty. But it's a rare case and
		 * this simpler check is probably good enough for now.
		 */
		దొరికింది_submodules = 1;
	} లేకపోతే ఒకవేళ (read_సూచీ_from(&istate, worktree_git_మార్గం(wt, "index"),
				   wt_gitdir) > 0) {
		కొరకు (i = 0; i < istate.cache_nr; i++) {
			నిర్మాణం cache_నమోదు *ce = istate.cache[i];
			పూర్ణాంకం err;

			ఒకవేళ (!S_ISGITLINK(ce->ce_బహుళకం))
				కొనసాగు;

			strbuf_reset(&మార్గం);
			strbuf_addf(&మార్గం, "%s/%s", wt->మార్గం, ce->name);
			ఒకవేళ (!is_submodule_populated_gently(మార్గం.buf, &err))
				కొనసాగు;

			దొరికింది_submodules = 1;
			ఆపు;
		}
	}
	discard_సూచీ(&istate);
	strbuf_release(&మార్గం);
	విడుదల(wt_gitdir);

	ఒకవేళ (దొరికింది_submodules)
		die(_("working trees containing submodules cannot be moved or removed"));
}

స్థిర పూర్ణాంకం move_worktree(పూర్ణాంకం ac, స్థిరరాశి అక్షరం **av, స్థిరరాశి అక్షరం *ఉపసర్గ,
			 నిర్మాణం repository *repo వాడని)
{
	పూర్ణాంకం force = 0;
	నిర్మాణం option options[] = {
		OPT__FORCE(&force,
			 N_("force move even if worktree is dirty or locked"),
			 PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "relative-paths", &use_relative_paths,
			 N_("use relative paths for worktrees")),
		OPT_చివర()
	};
	నిర్మాణం worktree **worktrees, *wt;
	నిర్మాణం strbuf dst = STRBUF_INIT;
	నిర్మాణం strbuf errmsg = STRBUF_INIT;
	స్థిరరాశి అక్షరం *reason = ఖాళీ;
	అక్షరం *మార్గం;

	ac = parse_options(ac, av, ఉపసర్గ, options, git_worktree_move_usage,
			   0);
	ఒకవేళ (ac != 2)
		usage_with_options(git_worktree_move_usage, options);

	మార్గం = ఉపసర్గ_filename(ఉపసర్గ, av[1]);
	strbuf_addstr(&dst, మార్గం);
	విడుదల(మార్గం);

	worktrees = get_worktrees(the_repository);
	wt = కనుగొను_worktree(worktrees, ఉపసర్గ, av[0]);
	ఒకవేళ (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	ఒకవేళ (is_main_worktree(wt))
		die(_("'%s' is a main working tree"), av[0]);
	ఒకవేళ (is_directory(dst.buf)) {
		స్థిరరాశి అక్షరం *sep = కనుగొను_చివరి_dir_sep(wt->మార్గం);

		ఒకవేళ (!sep)
			die(_("could not figure out destination name from '%s'"),
			    wt->మార్గం);
		strbuf_trim_trailing_dir_sep(&dst);
		strbuf_addstr(&dst, sep);
	}
	check_candidate_మార్గం(dst.buf, force, worktrees, "move");

	validate_no_submodules(wt);

	ఒకవేళ (force < 2)
		reason = worktree_తాళం_reason(wt);
	ఒకవేళ (reason) {
		ఒకవేళ (*reason)
			die(_("cannot move a locked working tree, lock reason: %s\nuse 'move -f -f' to override or unlock first"),
			    reason);
		die(_("cannot move a locked working tree;\nuse 'move -f -f' to override or unlock first"));
	}
	ఒకవేళ (validate_worktree(wt, &errmsg, 0))
		die(_("validation failed, cannot move working tree: %s"),
		    errmsg.buf);
	strbuf_release(&errmsg);

	ఒకవేళ (rename(wt->మార్గం, dst.buf) == -1)
		die_errno(_("failed to move '%s' to '%s'"), wt->మార్గం, dst.buf);

	update_worktree_location(wt, dst.buf, use_relative_paths);

	strbuf_release(&dst);
	free_worktrees(worktrees);
	తిరిగి 0;
}

/*
 * Note, "git status --porcelain" is used to determine if it's safe to
 * delete a whole worktree. "git status" does not ignore user
 * configuration, so if a normal "git status" shows "clean" for the
 * user, then it's ok to remove it.
 *
 * This assumption may be a bad one. We may want to ignore
 * (potentially bad) user settings and only delete a worktree when
 * it's absolutely safe to do so from _our_ point of view because we
 * know better.
 */
స్థిర శూన్య check_clean_worktree(నిర్మాణం worktree *wt,
				 స్థిరరాశి అక్షరం *original_మార్గం)
{
	నిర్మాణం బిడ్డ_process cp;
	అక్షరం buf[1];
	పూర్ణాంకం ret;

	/*
	 * Until we sort this out, all submodules are "dirty" and
	 * will abort this function.
	 */
	validate_no_submodules(wt);

	బిడ్డ_process_init(&cp);
	strvec_pushf(&cp.env, "%s=%s/.git",
		     GIT_DIR_ENVIRONMENT, wt->మార్గం);
	strvec_pushf(&cp.env, "%s=%s",
		     GIT_WORK_వృక్షం_ENVIRONMENT, wt->మార్గం);
	strvec_pushl(&cp.వాదనలు, "status",
		     "--porcelain", "--ignore-submodules=none",
		     ఖాళీ);
	cp.git_cmd = 1;
	cp.dir = wt->మార్గం;
	cp.బహిర్గతం = -1;
	ret = ప్రారంభం_command(&cp);
	ఒకవేళ (ret)
		die_errno(_("failed to run 'git status' on '%s'"),
			  original_మార్గం);
	ret = xread(cp.బహిర్గతం, buf, పరిమాణం(buf));
	ఒకవేళ (ret)
		die(_("'%s' contains modified or untracked files, use --force to delete it"),
		    original_మార్గం);
	close(cp.బహిర్గతం);
	ret = ముగింపు_command(&cp);
	ఒకవేళ (ret)
		die_errno(_("failed to run 'git status' on '%s', code %d"),
			  original_మార్గం, ret);
}

స్థిర పూర్ణాంకం delete_git_work_వృక్షం(నిర్మాణం worktree *wt)
{
	నిర్మాణం strbuf sb = STRBUF_INIT;
	పూర్ణాంకం ret = 0;

	strbuf_addstr(&sb, wt->మార్గం);
	ఒకవేళ (remove_dir_recursively(&sb, 0)) {
		error_errno(_("failed to delete '%s'"), sb.buf);
		ret = -1;
	}
	strbuf_release(&sb);
	తిరిగి ret;
}

స్థిర పూర్ణాంకం remove_worktree(పూర్ణాంకం ac, స్థిరరాశి అక్షరం **av, స్థిరరాశి అక్షరం *ఉపసర్గ,
			   నిర్మాణం repository *repo వాడని)
{
	పూర్ణాంకం force = 0;
	నిర్మాణం option options[] = {
		OPT__FORCE(&force,
			 N_("force removal even if worktree is dirty or locked"),
			 PARSE_OPT_NOCOMPLETE),
		OPT_చివర()
	};
	నిర్మాణం worktree **worktrees, *wt;
	నిర్మాణం strbuf errmsg = STRBUF_INIT;
	స్థిరరాశి అక్షరం *reason = ఖాళీ;
	పూర్ణాంకం ret = 0;

	ac = parse_options(ac, av, ఉపసర్గ, options, git_worktree_remove_usage, 0);
	ఒకవేళ (ac != 1)
		usage_with_options(git_worktree_remove_usage, options);

	worktrees = get_worktrees(the_repository);
	wt = కనుగొను_worktree(worktrees, ఉపసర్గ, av[0]);
	ఒకవేళ (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	ఒకవేళ (is_main_worktree(wt))
		die(_("'%s' is a main working tree"), av[0]);
	ఒకవేళ (force < 2)
		reason = worktree_తాళం_reason(wt);
	ఒకవేళ (reason) {
		ఒకవేళ (*reason)
			die(_("cannot remove a locked working tree, lock reason: %s\nuse 'remove -f -f' to override or unlock first"),
			    reason);
		die(_("cannot remove a locked working tree;\nuse 'remove -f -f' to override or unlock first"));
	}
	ఒకవేళ (validate_worktree(wt, &errmsg, WT_VALIDATE_WORKTREE_MISSING_OK))
		die(_("validation failed, cannot remove working tree: %s"),
		    errmsg.buf);
	strbuf_release(&errmsg);

	ఒకవేళ (file_exists(wt->మార్గం)) {
		ఒకవేళ (!force)
			check_clean_worktree(wt, av[0]);

		ret |= delete_git_work_వృక్షం(wt);
	}
	/*
	 * continue on even if ret is non-zero, there's no going back
	 * from here.
	 */
	ret |= delete_git_dir(wt->id);
	delete_worktrees_dir_if_ఖాళీలు();

	free_worktrees(worktrees);
	తిరిగి ret;
}

స్థిర శూన్య report_repair(పూర్ణాంకం iserr, స్థిరరాశి అక్షరం *మార్గం, స్థిరరాశి అక్షరం *సందేశం, శూన్య *cb_దత్తాంశం)
{
	ఒకవేళ (!iserr) {
		fprintf_ln(stderr, _("repair: %s: %s"), సందేశం, మార్గం);
	} లేకపోతే {
		పూర్ణాంకం *exit_స్థితి = (పూర్ణాంకం *)cb_దత్తాంశం;
		fprintf_ln(stderr, _("error: %s: %s"), సందేశం, మార్గం);
		*exit_స్థితి = 1;
	}
}

స్థిర పూర్ణాంకం repair(పూర్ణాంకం ac, స్థిరరాశి అక్షరం **av, స్థిరరాశి అక్షరం *ఉపసర్గ,
		  నిర్మాణం repository *repo వాడని)
{
	స్థిరరాశి అక్షరం **p;
	స్థిరరాశి అక్షరం *self[] = { ".", ఖాళీ };
	నిర్మాణం option options[] = {
		OPT_BOOL(0, "relative-paths", &use_relative_paths,
			 N_("use relative paths for worktrees")),
		OPT_చివర()
	};
	పూర్ణాంకం rc = 0;

	ac = parse_options(ac, av, ఉపసర్గ, options, git_worktree_repair_usage, 0);
	p = ac > 0 ? av : self;
	కొరకు (; *p; p++)
		repair_worktree_at_మార్గం(the_repository, *p, report_repair,
					&rc, use_relative_paths);
	repair_worktrees(the_repository, report_repair, &rc, use_relative_paths);
	తిరిగి rc;
}

పూర్ణాంకం cmd_worktree(పూర్ణాంకం ac,
		 స్థిరరాశి అక్షరం **av,
		 స్థిరరాశి అక్షరం *ఉపసర్గ,
		 నిర్మాణం repository *repo)
{
	parse_opt_subcommand_fn *fn = ఖాళీ;
	నిర్మాణం option options[] = {
		OPT_SUBCOMMAND("add", &fn, కూడు),
		OPT_SUBCOMMAND("prune", &fn, prune),
		OPT_SUBCOMMAND("list", &fn, list),
		OPT_SUBCOMMAND("lock", &fn, తాళం_worktree),
		OPT_SUBCOMMAND("unlock", &fn, unlock_worktree),
		OPT_SUBCOMMAND("move", &fn, move_worktree),
		OPT_SUBCOMMAND("remove", &fn, remove_worktree),
		OPT_SUBCOMMAND("repair", &fn, repair),
		OPT_చివర()
	};

	repo_config(the_repository, git_worktree_config, ఖాళీ);

	ఒకవేళ (!ఉపసర్గ)
		ఉపసర్గ = "";

	ac = parse_options(ac, av, ఉపసర్గ, options, git_worktree_usage, 0);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_నిండిన_సూచీ = 0;

	తిరిగి fn(ac, av, ఉపసర్గ, repo);
}
