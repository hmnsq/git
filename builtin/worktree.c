#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "abspath.h"
#ಅನ್ತರ್ಭಾವ "advice.h"
#ಅನ್ತರ್ಭಾವ "checkout.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "copy.h"
#ಅನ್ತರ್ಭಾವ "dir.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "object-file.h"
#ಅನ್ತರ್ಭಾವ "object-name.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "path.h"
#ಅನ್ತರ್ಭಾವ "strvec.h"
#ಅನ್ತರ್ಭಾವ "branch.h"
#ಅನ್ತರ್ಭಾವ "read-cache-ll.h"
#ಅನ್ತರ್ಭಾವ "refs.h"
#ಅನ್ತರ್ಭಾವ "remote.h"
#ಅನ್ತರ್ಭಾವ "run-command.h"
#ಅನ್ತರ್ಭಾವ "hook.h"
#ಅನ್ತರ್ಭಾವ "sigchain.h"
#ಅನ್ತರ್ಭಾವ "submodule.h"
#ಅನ್ತರ್ಭಾವ "utf8.h"
#ಅನ್ತರ್ಭಾವ "worktree.h"
#ಅನ್ತರ್ಭಾವ "quote.h"

#define BUILTIN_WORKTREE_ADD_USAGE \
	N_("git worktree add [-f] [--detach] [--checkout] [--lock [--reason <string>]]\n" \
	   "                 [--orphan] [(-b | -B) <new-branch>] <path> [<commit-ish>]")

#define BUILTIN_WORKTREE_LIST_USAGE \
	N_("git worktree list [-v | --porcelain [-z]]")
#define BUILTIN_WORKTREE_LOCK_USAGE \
	N_("git worktree lock [--reason <string>] <worktree>")
#define BUILTIN_WORKTREE_MOVE_USAGE \
	N_("git worktree move <worktree> <new-path>")
#define BUILTIN_WORKTREE_PRUNE_USAGE \
	N_("git worktree prune [-n] [-v] [--expire <expire>]")
#define BUILTIN_WORKTREE_REMOVE_USAGE \
	N_("git worktree remove [-f] <worktree>")
#define BUILTIN_WORKTREE_REPAIR_USAGE \
	N_("git worktree repair [<path>...]")
#define BUILTIN_WORKTREE_UNLOCK_USAGE \
	N_("git worktree unlock <worktree>")

#define WORKTREE_ADD_DWIM_ORPHAN_INFER_TEXT \
	_("No possible source branch, inferring '--orphan'")

#define WORKTREE_ADD_ORPHAN_WITH_DASH_B_HINT_TEXT \
	_("If you meant to create a worktree containing a new unborn branch\n" \
	"(branch with no commits) for this repository, you can do so\n" \
	"using the --orphan flag:\n" \
	"\n" \
	"    git worktree add --orphan -b %s %s\n")

#define WORKTREE_ADD_ORPHAN_NO_DASH_B_HINT_TEXT \
	_("If you meant to create a worktree containing a new unborn branch\n" \
	"(branch with no commits) for this repository, you can do so\n" \
	"using the --orphan flag:\n" \
	"\n" \
	"    git worktree add --orphan %s\n")

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ git_worktree_usage[] = {
	BUILTIN_WORKTREE_ADD_USAGE,
	BUILTIN_WORKTREE_LIST_USAGE,
	BUILTIN_WORKTREE_LOCK_USAGE,
	BUILTIN_WORKTREE_MOVE_USAGE,
	BUILTIN_WORKTREE_PRUNE_USAGE,
	BUILTIN_WORKTREE_REMOVE_USAGE,
	BUILTIN_WORKTREE_REPAIR_USAGE,
	BUILTIN_WORKTREE_UNLOCK_USAGE,
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ git_worktree_add_usage[] = {
	BUILTIN_WORKTREE_ADD_USAGE,
	ರಿಕ್ತ,
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ git_worktree_list_usage[] = {
	BUILTIN_WORKTREE_LIST_USAGE,
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ git_worktree_lock_usage[] = {
	BUILTIN_WORKTREE_LOCK_USAGE,
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ git_worktree_move_usage[] = {
	BUILTIN_WORKTREE_MOVE_USAGE,
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ git_worktree_prune_usage[] = {
	BUILTIN_WORKTREE_PRUNE_USAGE,
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ git_worktree_remove_usage[] = {
	BUILTIN_WORKTREE_REMOVE_USAGE,
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ git_worktree_repair_usage[] = {
	BUILTIN_WORKTREE_REPAIR_USAGE,
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ git_worktree_unlock_usage[] = {
	BUILTIN_WORKTREE_UNLOCK_USAGE,
	ರಿಕ್ತ
};

ಸಂರಚನಾ add_opts {
	ಪೂರ್ಣಾಙ್ಕ force;
	ಪೂರ್ಣಾಙ್ಕ detach;
	ಪೂರ್ಣಾಙ್ಕ quiet;
	ಪೂರ್ಣಾಙ್ಕ checkout;
	ಪೂರ್ಣಾಙ್ಕ orphan;
	ಪೂರ್ಣಾಙ್ಕ relative_paths;
	ಧ್ರುವ ವರ್ಣ *keep_locked;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_only;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ verbose;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ guess_remote;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ use_relative_paths;
ಸ್ಥಿರ timestamp_t expire;

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ git_worktree_config(ಧ್ರುವ ವರ್ಣ *var, ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ,
			       ಧ್ರುವ ಸಂರಚನಾ config_context *ctx, ಶೂನ್ಯ *cb)
{
	ಯದಿ (!strcmp(var, "worktree.guessremote")) {
		guess_remote = git_config_bool(var, ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	} ಅನ್ಯಥಾ ಯದಿ (!strcmp(var, "worktree.userelativepaths")) {
		use_relative_paths = git_config_bool(var, ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಪ್ರತ್ಯಾಗಮ git_default_config(var, ಮೌಲ್ಯ, ctx, cb);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ delete_git_dir(ಧ್ರುವ ವರ್ಣ *id)
{
	ಸಂರಚನಾ strbuf sb = STRBUF_INIT;
	ಪೂರ್ಣಾಙ್ಕ ret;

	repo_common_path_append(the_repository, &sb, "worktrees/%s", id);
	ret = remove_dir_recursively(&sb, 0);
	ಯದಿ (ret < 0 && errno == ENOTDIR)
		ret = unlink(sb.buf);
	ಯದಿ (ret)
		error_errno(_("failed to delete '%s'"), sb.buf);
	strbuf_release(&sb);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಶೂನ್ಯ delete_worktrees_dir_if_empty(ಶೂನ್ಯ)
{
	ವರ್ಣ *path = repo_git_path(the_repository, "worktrees");
	rmdir(path); /* ignore failed removal */
	ಮುಕ್ತ(path);
}

ಸ್ಥಿರ ಶೂನ್ಯ prune_worktree(ಧ್ರುವ ವರ್ಣ *id, ಧ್ರುವ ವರ್ಣ *reason)
{
	ಯದಿ (show_only || verbose)
		fprintf_ln(stderr, _("Removing %s/%s: %s"), "worktrees", id, reason);
	ಯದಿ (!show_only)
		delete_git_dir(id);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ prune_cmp(ಧ್ರುವ ಶೂನ್ಯ *a, ಧ್ರುವ ಶೂನ್ಯ *b)
{
	ಧ್ರುವ ಸಂರಚನಾ string_list_item *x = a;
	ಧ್ರುವ ಸಂರಚನಾ string_list_item *y = b;
	ಪೂರ್ಣಾಙ್ಕ c;

	ಯದಿ ((c = fspathcmp(x->string, y->string)))
	    ಪ್ರತ್ಯಾಗಮ c;
	/*
	 * paths same; prune_dupes() removes all but the first worktree entry
	 * having the same path, so sort main worktree ('util' is NULL) above
	 * linked worktrees ('util' not NULL) since main worktree can't be
	 * removed
	 */
	ಯದಿ (!x->util)
		ಪ್ರತ್ಯಾಗಮ -1;
	ಯದಿ (!y->util)
		ಪ್ರತ್ಯಾಗಮ 1;
	/* paths same; sort by .git/worktrees/<id> */
	ಪ್ರತ್ಯಾಗಮ strcmp(x->util, y->util);
}

ಸ್ಥಿರ ಶೂನ್ಯ prune_dups(ಸಂರಚನಾ string_list *l)
{
	ಪೂರ್ಣಾಙ್ಕ i;

	QSORT(l->ವಸ್ತುಗಳು, l->nr, prune_cmp);
	ಪ್ರತಿ (i = 1; i < l->nr; i++) {
		ಯದಿ (!fspathcmp(l->ವಸ್ತುಗಳು[i].string, l->ವಸ್ತುಗಳು[i - 1].string))
			prune_worktree(l->ವಸ್ತುಗಳು[i].util, "duplicate entry");
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ prune_worktrees(ಶೂನ್ಯ)
{
	ಸಂರಚನಾ strbuf reason = STRBUF_INIT;
	ಸಂರಚನಾ strbuf main_path = STRBUF_INIT;
	ಸಂರಚನಾ string_list kept = STRING_LIST_INIT_DUP;
	ವರ್ಣ *path;
	DIR *dir;
	ಸಂರಚನಾ dirent *d;

	path = repo_git_path(the_repository, "worktrees");
	dir = opendir(path);
	ಮುಕ್ತ(path);
	ಯದಿ (!dir)
		ಪ್ರತ್ಯಾಗಮ;
	ಯಾವತ್ ((d = readdir_skip_dot_and_dotdot(dir)) != ರಿಕ್ತ) {
		ವರ್ಣ *path;
		strbuf_reset(&reason);
		ಯದಿ (should_prune_worktree(the_repository, d->d_name, &reason, &path, expire))
			prune_worktree(d->d_name, reason.buf);
		ಅನ್ಯಥಾ ಯದಿ (path)
			string_list_append_nodup(&kept, path)->util = xstrdup(d->d_name);
	}
	closedir(dir);

	strbuf_add_absolute_path(&main_path, repo_get_common_dir(the_repository));
	/* massage main worktree absolute path to match 'gitdir' content */
	strbuf_strip_suffix(&main_path, "/.");
	string_list_append_nodup(&kept, strbuf_detach(&main_path, ರಿಕ್ತ));
	prune_dups(&kept);
	string_list_clear(&kept, 1);

	ಯದಿ (!show_only)
		delete_worktrees_dir_if_empty();
	strbuf_release(&reason);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ prune(ಪೂರ್ಣಾಙ್ಕ ac, ಧ್ರುವ ವರ್ಣ **av, ಧ್ರುವ ವರ್ಣ *prefix,
		 ಸಂರಚನಾ repository *repo UNUSED)
{
	ಸಂರಚನಾ option options[] = {
		OPT__DRY_RUN(&show_only, N_("do not remove, show only")),
		OPT__VERBOSE(&verbose, N_("report pruned working trees")),
		OPT_EXPIRY_DATE(0, "expire", &expire,
				N_("prune missing working trees older than <time>")),
		OPT_END()
	};

	expire = TIME_MAX;
	ac = parse_options(ac, av, prefix, options, git_worktree_prune_usage,
			   0);
	ಯದಿ (ac)
		usage_with_options(git_worktree_prune_usage, options);
	prune_worktrees();
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ವರ್ಣ *junk_work_ವೃಕ್ಷ;
ಸ್ಥಿರ ವರ್ಣ *junk_git_dir;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ is_junk;
ಸ್ಥಿರ pid_t junk_pid;

ಸ್ಥಿರ ಶೂನ್ಯ remove_junk(ಶೂನ್ಯ)
{
	ಸಂರಚನಾ strbuf sb = STRBUF_INIT;
	ಯದಿ (!is_junk || getpid() != junk_pid)
		ಪ್ರತ್ಯಾಗಮ;
	ಯದಿ (junk_git_dir) {
		strbuf_addstr(&sb, junk_git_dir);
		remove_dir_recursively(&sb, 0);
		strbuf_reset(&sb);
	}
	ಯದಿ (junk_work_ವೃಕ್ಷ) {
		strbuf_addstr(&sb, junk_work_ವೃಕ್ಷ);
		remove_dir_recursively(&sb, 0);
	}
	strbuf_release(&sb);
}

ಸ್ಥಿರ ಶೂನ್ಯ remove_junk_on_signal(ಪೂರ್ಣಾಙ್ಕ signo)
{
	remove_junk();
	sigchain_ತೆಗೆ(signo);
	ಉತ್ಥಾಪಯ(signo);
}

ಸ್ಥಿರ ವರ್ಣ *worktree_basename_dup(ಧ್ರುವ ವರ್ಣ *path)
{
	ಧ್ರುವ ವರ್ಣ *name;
	ಪೂರ್ಣಾಙ್ಕ len;

	len = strlen(path);
	ಯಾವತ್ (len && is_dir_sep(path[len - 1]))
		len--;

	name = path + len;
	ಯಾವತ್ (name > path && !is_dir_sep(name[-1]))
		name--;

	ಪ್ರತ್ಯಾಗಮ xmemdupz(name, path + len - name);
}

/* check that path is viable location for worktree */
ಸ್ಥಿರ ಶೂನ್ಯ check_candidate_path(ಧ್ರುವ ವರ್ಣ *path,
				 ಪೂರ್ಣಾಙ್ಕ force,
				 ಸಂರಚನಾ worktree **worktrees,
				 ಧ್ರುವ ವರ್ಣ *cmd)
{
	ಸಂರಚನಾ worktree *wt;
	ಪೂರ್ಣಾಙ್ಕ locked;

	ಯದಿ (file_exists(path) && !is_empty_dir(path))
		die(_("'%s' already exists"), path);

	wt = find_worktree_by_path(worktrees, path);
	ಯದಿ (!wt)
		ಪ್ರತ್ಯಾಗಮ;

	locked = !!worktree_lock_reason(wt);
	ಯದಿ ((!locked && force) || (locked && force > 1)) {
		ಯದಿ (delete_git_dir(wt->id))
		    die(_("unusable worktree destination '%s'"), path);
		ಪ್ರತ್ಯಾಗಮ;
	}

	ಯದಿ (locked)
		die(_("'%s' is a missing but locked worktree;\nuse '%s -f -f' to override, or 'unlock' and 'prune' or 'remove' to clear"), path, cmd);
	ಅನ್ಯಥಾ
		die(_("'%s' is a missing but already registered worktree;\nuse '%s -f' to override, or 'prune' or 'remove' to clear"), path, cmd);
}

ಸ್ಥಿರ ಶೂನ್ಯ copy_sparse_checkout(ಧ್ರುವ ವರ್ಣ *worktree_git_dir)
{
	ವರ್ಣ *from_file = repo_git_path(the_repository, "info/sparse-checkout");
	ವರ್ಣ *to_file = xstrfmt("%s/info/sparse-checkout", worktree_git_dir);

	ಯದಿ (file_exists(from_file)) {
		ಯದಿ (safe_create_leading_directories(the_repository, to_file) ||
			copy_file(the_repository, to_file, from_file, 0666))
			error(_("failed to copy '%s' to '%s'; sparse-checkout may not work correctly"),
				from_file, to_file);
	}

	ಮುಕ್ತ(from_file);
	ಮುಕ್ತ(to_file);
}

ಸ್ಥಿರ ಶೂನ್ಯ copy_filtered_worktree_config(ಧ್ರುವ ವರ್ಣ *worktree_git_dir)
{
	ವರ್ಣ *from_file = repo_git_path(the_repository, "config.worktree");
	ವರ್ಣ *to_file = xstrfmt("%s/config.worktree", worktree_git_dir);

	ಯದಿ (file_exists(from_file)) {
		ಸಂರಚನಾ config_set cs = { { 0 } };
		ಪೂರ್ಣಾಙ್ಕ bare;

		ಯದಿ (safe_create_leading_directories(the_repository, to_file) ||
			copy_file(the_repository, to_file, from_file, 0666)) {
			error(_("failed to copy worktree config from '%s' to '%s'"),
				from_file, to_file);
			ಗಚ್ಛ worktree_copy_cleanup;
		}

		git_configset_init(&cs);
		git_configset_add_file(&cs, from_file);

		ಯದಿ (!git_configset_get_bool(&cs, "core.bare", &bare) &&
			bare &&
			repo_config_set_multivar_in_file_gently(the_repository,
				to_file, "core.bare", ರಿಕ್ತ, "true", ರಿಕ್ತ, 0))
			error(_("failed to unset '%s' in '%s'"),
				"core.bare", to_file);
		ಯದಿ (!git_configset_get(&cs, "core.worktree") &&
			repo_config_set_in_file_gently(the_repository, to_file,
						       "core.worktree", ರಿಕ್ತ, ರಿಕ್ತ))
			error(_("failed to unset '%s' in '%s'"),
				"core.worktree", to_file);

		git_configset_clear(&cs);
	}

worktree_copy_cleanup:
	ಮುಕ್ತ(from_file);
	ಮುಕ್ತ(to_file);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ checkout_worktree(ಧ್ರುವ ಸಂರಚನಾ add_opts *opts,
			     ಸಂರಚನಾ strvec *child_env)
{
	ಸಂರಚನಾ child_process cp = CHILD_PROCESS_INIT;
	cp.git_cmd = 1;
	strvec_pushl(&cp.args, "reset", "--hard", "--no-recurse-submodules", ರಿಕ್ತ);
	ಯದಿ (opts->quiet)
		strvec_ತಳ್ಳು(&cp.args, "--quiet");
	strvec_pushv(&cp.env, child_env->v);
	ಪ್ರತ್ಯಾಗಮ run_command(&cp);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ make_worktree_orphan(ಧ್ರುವ ವರ್ಣ * ref, ಧ್ರುವ ಸಂರಚನಾ add_opts *opts,
				ಸಂರಚನಾ strvec *child_env)
{
	ಸಂರಚನಾ strbuf symref = STRBUF_INIT;
	ಸಂರಚನಾ child_process cp = CHILD_PROCESS_INIT;

	validate_new_branchname(ref, &symref, 0);
	strvec_pushl(&cp.args, "symbolic-ref", "HEAD", symref.buf, ರಿಕ್ತ);
	ಯದಿ (opts->quiet)
		strvec_ತಳ್ಳು(&cp.args, "--quiet");
	strvec_pushv(&cp.env, child_env->v);
	strbuf_release(&symref);
	cp.git_cmd = 1;
	ಪ್ರತ್ಯಾಗಮ run_command(&cp);
}

/*
 * References for worktrees are generally stored in '$GIT_DIR/worktrees/<wt_id>'.
 * But when using alternate reference directories, we want to store the worktree
 * references in '$ALTERNATE_REFERENCE_DIR/worktrees/<wt_id>'.
 *
 * Create the necessary folder structure to facilitate the same. But to ensure
 * that the former path is still considered a Git directory, add stubs.
 */
ಸ್ಥಿರ ಶೂನ್ಯ setup_alternate_ref_dir(ಸಂರಚನಾ worktree *wt, ಧ್ರುವ ವರ್ಣ *wt_git_path)
{
	ಸಂರಚನಾ strbuf sb = STRBUF_INIT;
	ವರ್ಣ *path;

	path = wt->repo->ref_storage_payload;
	ಯದಿ (!path)
		ಪ್ರತ್ಯಾಗಮ;

	ಯದಿ (!is_absolute_path(path))
		strbuf_addf(&sb, "%s/", wt->repo->commondir);

	strbuf_addf(&sb, "%s/worktrees", path);
	safe_create_dir(wt->repo, sb.buf, 1);
	strbuf_addf(&sb, "/%s", wt->id);
	safe_create_dir(wt->repo, sb.buf, 1);
	strbuf_reset(&sb);

	strbuf_addf(&sb, "this worktree stores references in %s/worktrees/%s",
		    path, wt->id);
	refs_create_refdir_stubs(wt->repo, wt_git_path, sb.buf);

	strbuf_release(&sb);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ add_worktree(ಧ್ರುವ ವರ್ಣ *path, ಧ್ರುವ ವರ್ಣ *refname,
			ಧ್ರುವ ಸಂರಚನಾ add_opts *opts)
{
	ಸಂರಚನಾ strbuf sb_git = STRBUF_INIT, sb_repo = STRBUF_INIT;
	ಸಂರಚನಾ strbuf sb = STRBUF_INIT;
	ಧ್ರುವ ವರ್ಣ *name;
	ವರ್ಣ *name_to_free = ರಿಕ್ತ;
	ಸಂರಚನಾ strvec child_env = STRVEC_INIT;
	unsigned ಪೂರ್ಣಾಙ್ಕ counter = 0;
	ಪೂರ್ಣಾಙ್ಕ len, ret;
	ಸಂರಚನಾ strbuf symref = STRBUF_INIT;
	ಸಂರಚನಾ commit *commit = ರಿಕ್ತ;
	ಪೂರ್ಣಾಙ್ಕ is_branch = 0;
	ಸಂರಚನಾ strbuf sb_name = STRBUF_INIT;
	ಸಂರಚನಾ worktree **worktrees, *wt = ರಿಕ್ತ;
	ಸಂರಚನಾ ref_store *wt_refs;
	ಸಂರಚನಾ repo_config_ಮೌಲ್ಯಗಳು *cfg = repo_config_ಮೌಲ್ಯಗಳು(the_repository);

	worktrees = get_worktrees(the_repository);
	check_candidate_path(path, opts->force, worktrees, "add");
	free_worktrees(worktrees);
	worktrees = ರಿಕ್ತ;

	/* is 'refname' a branch or commit? */
	ಯದಿ (!opts->detach && !check_branch_ref(the_repository, &symref, refname) &&
	    refs_ref_exists(get_main_ref_store(the_repository), symref.buf)) {
		is_branch = 1;
		ಯದಿ (!opts->force)
			die_if_checked_out(symref.buf, 0);
	}
	commit = lookup_commit_reference_by_name(refname);
	ಯದಿ (!commit && !opts->orphan)
		die(_("invalid reference: %s"), refname);

	name = name_to_free = worktree_basename_dup(path);
	ಯದಿ (!*name)
		die(_("invalid path '%s'"), path);
	sanitize_refname_component(name, &sb_name);
	ಯದಿ (!sb_name.len)
		BUG("How come '%s' becomes empty after sanitization?", name);
	name = sb_name.buf;
	repo_git_path_replace(the_repository, &sb_repo, "worktrees/%s", name);
	len = sb_repo.len;
	ಯದಿ (safe_create_leading_directories_const(the_repository, sb_repo.buf))
		die_errno(_("could not create leading directories of '%s'"),
			  sb_repo.buf);

	ಯಾವತ್ (mkdir(sb_repo.buf, 0777)) {
		counter++;
		ಯದಿ ((errno != EEXIST) || !counter /* overflow */)
			die_errno(_("could not create directory of '%s'"),
				  sb_repo.buf);
		strbuf_setlen(&sb_repo, len);
		strbuf_addf(&sb_repo, "%d", counter);
	}
	name = strrchr(sb_repo.buf, '/') + 1;

	junk_pid = getpid();
	atexit(remove_junk);
	sigchain_ತಳ್ಳು_common(remove_junk_on_signal);

	junk_git_dir = xstrdup(sb_repo.buf);
	is_junk = 1;

	/*
	 * lock the incomplete repo so prune won't delete it, unlock
	 * after the preparation is over.
	 */
	strbuf_addf(&sb, "%s/locked", sb_repo.buf);
	ಯದಿ (opts->keep_locked)
		write_file(sb.buf, "%s", opts->keep_locked);
	ಅನ್ಯಥಾ
		write_file(sb.buf, _("initializing"));

	strbuf_addf(&sb_git, "%s/.git", path);
	ಯದಿ (safe_create_leading_directories_const(the_repository, sb_git.buf))
		die_errno(_("could not create leading directories of '%s'"),
			  sb_git.buf);
	junk_work_ವೃಕ್ಷ = xstrdup(path);

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
	ಯದಿ (!wt) {
		ret = error(_("could not find created worktree '%s'"), name);
		ಗಚ್ಛ done;
	}
	setup_alternate_ref_dir(wt, sb_repo.buf);
	wt_refs = get_worktree_ref_store(wt);

	ret = ref_store_create_on_disk(wt_refs, REF_STORE_CREATE_ON_DISK_IS_WORKTREE, &sb);
	ಯದಿ (ret)
		ಗಚ್ಛ done;

	ಯದಿ (!is_branch && commit)
		ret = refs_update_ref(wt_refs, ರಿಕ್ತ, "HEAD", &commit->object.oid,
				      ರಿಕ್ತ, 0, UPDATE_REFS_ಸಂದೇಶ_ON_ERR);
	ಅನ್ಯಥಾ
		ret = refs_update_symref(wt_refs, "HEAD", symref.buf, ರಿಕ್ತ);
	ಯದಿ (ret)
		ಗಚ್ಛ done;

	/*
	 * If the current worktree has sparse-checkout enabled, then copy
	 * the sparse-checkout patterns from the current worktree.
	 */
	ಯದಿ (cfg->apply_sparse_checkout)
		copy_sparse_checkout(sb_repo.buf);

	/*
	 * If we are using worktree config, then copy all current config
	 * values from the current worktree into the new one, that way the
	 * new worktree behaves the same as this one.
	 */
	ಯದಿ (the_repository->repository_format_worktree_config)
		copy_filtered_worktree_config(sb_repo.buf);

	strvec_pushf(&child_env, "%s=%s", GIT_DIR_ENVIRONMENT, sb_git.buf);
	strvec_pushf(&child_env, "%s=%s", GIT_WORK_ವೃಕ್ಷ_ENVIRONMENT, path);

	ಯದಿ (opts->orphan &&
	    (ret = make_worktree_orphan(refname, opts, &child_env)))
		ಗಚ್ಛ done;

	ಯದಿ (opts->checkout &&
	    (ret = checkout_worktree(opts, &child_env)))
		ಗಚ್ಛ done;

	is_junk = 0;
	FREE_AND_NULL(junk_work_ವೃಕ್ಷ);
	FREE_AND_NULL(junk_git_dir);

done:
	ಯದಿ (ret || !opts->keep_locked) {
		strbuf_reset(&sb);
		strbuf_addf(&sb, "%s/locked", sb_repo.buf);
		unlink_or_warn(sb.buf);
	}

	/*
	 * Hook failure does not warrant worktree deletion, so run hook after
	 * is_junk is cleared, but do return appropriate code when hook fails.
	 */
	ಯದಿ (!ret && opts->checkout && !opts->orphan) {
		ಸಂರಚನಾ run_hooks_opt opt = RUN_HOOKS_OPT_INIT_FORCE_SERIAL;

		strvec_pushl(&opt.env, "GIT_DIR", "GIT_WORK_TREE", ರಿಕ್ತ);
		strvec_pushl(&opt.args,
			     oid_to_hex(null_oid(the_hash_algo)),
			     oid_to_hex(&commit->object.oid),
			     "1",
			     ರಿಕ್ತ);
		opt.dir = path;

		ret = run_hooks_opt(the_repository, "post-checkout", &opt);
	}

	strvec_clear(&child_env);
	strbuf_release(&sb);
	strbuf_release(&symref);
	strbuf_release(&sb_repo);
	strbuf_release(&sb_git);
	strbuf_release(&sb_name);
	free_worktree(wt);
	ಮುಕ್ತ(name_to_free);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಶೂನ್ಯ print_preparing_worktree_ಸಾಲು(ಪೂರ್ಣಾಙ್ಕ detach,
					  ಧ್ರುವ ವರ್ಣ *branch,
					  ಧ್ರುವ ವರ್ಣ *new_branch,
					  ಪೂರ್ಣಾಙ್ಕ force_new_branch)
{
	ಯದಿ (force_new_branch) {
		ಸಂರಚನಾ commit *commit = lookup_commit_reference_by_name(new_branch);
		ಯದಿ (!commit)
			fprintf_ln(stderr, _("Preparing worktree (new branch '%s')"), new_branch);
		ಅನ್ಯಥಾ
			fprintf_ln(stderr, _("Preparing worktree (resetting branch '%s'; was at %s)"),
				  new_branch,
				  repo_find_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV));
	} ಅನ್ಯಥಾ ಯದಿ (new_branch) {
		fprintf_ln(stderr, _("Preparing worktree (new branch '%s')"), new_branch);
	} ಅನ್ಯಥಾ {
		ಸಂರಚನಾ strbuf s = STRBUF_INIT;
		ಯದಿ (!detach && !check_branch_ref(the_repository, &s, branch) &&
		    refs_ref_exists(get_main_ref_store(the_repository), s.buf))
			fprintf_ln(stderr, _("Preparing worktree (checking out '%s')"),
				  branch);
		ಅನ್ಯಥಾ {
			ಸಂರಚನಾ commit *commit = lookup_commit_reference_by_name(branch);
			ಯದಿ (!commit)
				BUG("unreachable: invalid reference: %s", branch);
			fprintf_ln(stderr, _("Preparing worktree (detached HEAD %s)"),
				  repo_find_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV));
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
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ first_valid_ref(ಧ್ರುವ ಸಂರಚನಾ reference *ref UNUSED, ಶೂನ್ಯ *cb_data UNUSED)
{
	ಪ್ರತ್ಯಾಗಮ 1;
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
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ can_use_local_refs(ಧ್ರುವ ಸಂರಚನಾ add_opts *opts)
{
	ಯದಿ (refs_head_ref(get_main_ref_store(the_repository), first_valid_ref, ರಿಕ್ತ)) {
		ಪ್ರತ್ಯಾಗಮ 1;
	} ಅನ್ಯಥಾ ಯದಿ (refs_for_each_branch_ref(get_main_ref_store(the_repository), first_valid_ref, ರಿಕ್ತ)) {
		ಯದಿ (!opts->quiet)
			warning(_("HEAD points to an invalid (or orphaned) reference.\n"));
		ಪ್ರತ್ಯಾಗಮ 1;
	}
	ಪ್ರತ್ಯಾಗಮ 0;
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
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ can_use_remote_refs(ಧ್ರುವ ಸಂರಚನಾ add_opts *opts)
{
	ಯದಿ (!guess_remote) {
		ಪ್ರತ್ಯಾಗಮ 0;
	} ಅನ್ಯಥಾ ಯದಿ (refs_for_each_remote_ref(get_main_ref_store(the_repository), first_valid_ref, ರಿಕ್ತ)) {
		ಪ್ರತ್ಯಾಗಮ 1;
	} ಅನ್ಯಥಾ ಯದಿ (!opts->force && remote_get(ರಿಕ್ತ)) {
		die(_("No local or remote refs exist despite at least one remote\n"
		      "present, stopping; use 'add -f' to override or fetch a remote first"));
	}
	ಪ್ರತ್ಯಾಗಮ 0;
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
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ dwim_orphan(ಧ್ರುವ ಸಂರಚನಾ add_opts *opts, ಪೂರ್ಣಾಙ್ಕ opt_track, ಪೂರ್ಣಾಙ್ಕ remote)
{
	ಯದಿ (can_use_local_refs(opts)) {
		ಪ್ರತ್ಯಾಗಮ 0;
	} ಅನ್ಯಥಾ ಯದಿ (remote && can_use_remote_refs(opts)) {
		ಪ್ರತ್ಯಾಗಮ 0;
	} ಅನ್ಯಥಾ ಯದಿ (!opts->quiet) {
		fprintf_ln(stderr, WORKTREE_ADD_DWIM_ORPHAN_INFER_TEXT);
	}

	ಯದಿ (opt_track) {
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--track");
	} ಅನ್ಯಥಾ ಯದಿ (!opts->checkout) {
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--no-checkout");
	}
	ಪ್ರತ್ಯಾಗಮ 1;
}

ಸ್ಥಿರ ಶೂನ್ಯ advise_disambiguating_remotes(ಧ್ರುವ ವರ್ಣ *path, ಧ್ರುವ ವರ್ಣ *branch,
					  ಧ್ರುವ ಸಂರಚನಾ string_list *matched_remote_names)
{
	ಸಂರಚನಾ string_list_item *item;

	advise(_("Branch name '%s' appears in multiple remotes:"), branch);
	for_each_string_list_item(item, matched_remote_names) {
		advise(_("  %s"), item->string);
	}
	advise(_("If you meant to create a worktree from a remote tracking branch on\n"
		 "<remote>, you can do so by:\n"
		 "\n"
		 "    git worktree add -b %s %s <remote>/%s\n"
		 "\n"
		 "If you'd like to always prefer some remote, e.g. 'origin',\n"
		 "consider setting checkout.defaultRemote=origin in your config."),
	       branch, path, branch);
}

ಸ್ಥಿರ ವರ್ಣ *dwim_branch(ಧ್ರುವ ಸಂರಚನಾ add_opts *opts, ಧ್ರುವ ವರ್ಣ *path, ವರ್ಣ **new_branch)
{
	ಪೂರ್ಣಾಙ್ಕ branch_exists;
	ವರ್ಣ *branchname = worktree_basename_dup(path);
	ಸಂರಚನಾ strbuf ref = STRBUF_INIT;

	branch_exists = !check_branch_ref(the_repository, &ref, branchname) &&
			refs_ref_exists(get_main_ref_store(the_repository),
					ref.buf);
	strbuf_release(&ref);
	ಯದಿ (branch_exists)
		ಪ್ರತ್ಯಾಗಮ branchname;

	*new_branch = branchname;
	ಯದಿ (guess_remote) {
		ಸಂರಚನಾ object_id oid;
		ವರ್ಣ *remote;
		ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_matches = 0;
		ಸಂರಚನಾ string_list matched_remote_names = STRING_LIST_INIT_DUP;

		remote = unique_tracking_name(*new_branch, &oid, &ಅಂಕೆ_matches,
					      &matched_remote_names);
		ಯದಿ (!remote && ಅಂಕೆ_matches > 1) {
			ಯದಿ (!opts->quiet &&
			    advice_enabled(ADVICE_CHECKOUT_AMBIGUOUS_REMOTE_BRANCH_NAME))
				advise_disambiguating_remotes(path, *new_branch,
							      &matched_remote_names);
			die(_("'%s' matched multiple (%d) remote tracking branches"),
			    *new_branch, ಅಂಕೆ_matches);
		}
		string_list_clear(&matched_remote_names, 0);
		ಪ್ರತ್ಯಾಗಮ remote;
	}
	ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ add(ಪೂರ್ಣಾಙ್ಕ ac, ಧ್ರುವ ವರ್ಣ **av, ಧ್ರುವ ವರ್ಣ *prefix,
	       ಸಂರಚನಾ repository *repo UNUSED)
{
	ಸಂರಚನಾ add_opts opts;
	ಧ್ರುವ ವರ್ಣ *new_branch_force = ರಿಕ್ತ;
	ವರ್ಣ *path;
	ಧ್ರುವ ವರ್ಣ *branch;
	ವರ್ಣ *branch_to_free = ರಿಕ್ತ;
	ವರ್ಣ *new_branch_to_free = ರಿಕ್ತ;
	ಧ್ರುವ ವರ್ಣ *new_branch = ರಿಕ್ತ;
	ವರ್ಣ *opt_track = ರಿಕ್ತ;
	ಧ್ರುವ ವರ್ಣ *lock_reason = ರಿಕ್ತ;
	ಪೂರ್ಣಾಙ್ಕ keep_locked = 0;
	ಪೂರ್ಣಾಙ್ಕ used_new_branch_options;
	ಸಂರಚನಾ option options[] = {
		OPT__FORCE(&opts.force,
			   N_("checkout <branch> even if already checked out in other worktree"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_STRING('b', ರಿಕ್ತ, &new_branch, N_("branch"),
			   N_("create a new branch")),
		OPT_STRING('B', ರಿಕ್ತ, &new_branch_force, N_("branch"),
			   N_("create or reset a branch")),
		OPT_BOOL(0, "orphan", &opts.orphan, N_("create unborn branch")),
		OPT_BOOL('d', "detach", &opts.detach, N_("detach HEAD at named commit")),
		OPT_BOOL(0, "checkout", &opts.checkout, N_("populate the new working tree")),
		OPT_BOOL(0, "lock", &keep_locked, N_("keep the new working tree locked")),
		OPT_STRING(0, "reason", &lock_reason, N_("string"),
			   N_("reason for locking")),
		OPT__QUIET(&opts.quiet, N_("suppress progress reporting")),
		OPT_PASSTHRU(0, "track", &opt_track, ರಿಕ್ತ,
			     N_("set up tracking mode (see git-branch(1))"),
			     PARSE_OPT_NOARG | PARSE_OPT_OPTARG),
		OPT_BOOL(0, "guess-remote", &guess_remote,
			 N_("try to match the new branch name with a remote-tracking branch")),
		OPT_BOOL(0, "relative-paths", &opts.relative_paths,
			 N_("use relative paths for worktrees")),
		OPT_END()
	};
	ಪೂರ್ಣಾಙ್ಕ ret;

	memset(&opts, 0, ಪರಿಮಾಣ(opts));
	opts.checkout = 1;
	opts.relative_paths = use_relative_paths;
	ac = parse_options(ac, av, prefix, options, git_worktree_add_usage, 0);
	ಯದಿ (!!opts.detach + !!new_branch + !!new_branch_force > 1)
		die(_("options '%s', '%s', and '%s' cannot be used together"), "-b", "-B", "--detach");
	ಯದಿ (opts.detach && opts.orphan)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--detach");
	ಯದಿ (opts.orphan && opt_track)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--track");
	ಯದಿ (opts.orphan && !opts.checkout)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--no-checkout");
	ಯದಿ (opts.orphan && ac == 2)
		die(_("option '%s' and commit-ish cannot be used together"),
		    "--orphan");
	ಯದಿ (lock_reason && !keep_locked)
		die(_("the option '%s' requires '%s'"), "--reason", "--lock");
	ಯದಿ (lock_reason)
		opts.keep_locked = lock_reason;
	ಅನ್ಯಥಾ ಯದಿ (keep_locked)
		opts.keep_locked = _("added with --lock");

	ಯದಿ (ac < 1 || ac > 2)
		usage_with_options(git_worktree_add_usage, options);

	path = prefix_filename(prefix, av[0]);
	branch = ac < 2 ? "HEAD" : av[1];
	used_new_branch_options = new_branch || new_branch_force;

	ಯದಿ (!strcmp(branch, "-"))
		branch = "@{-1}";

	ಯದಿ (new_branch_force) {
		ಸಂರಚನಾ strbuf symref = STRBUF_INIT;

		new_branch = new_branch_force;

		ಯದಿ (!opts.force &&
		    !check_branch_ref(the_repository, &symref, new_branch) &&
		    refs_ref_exists(get_main_ref_store(the_repository), symref.buf))
			die_if_checked_out(symref.buf, 0);
		strbuf_release(&symref);
	}

	ಯದಿ (opts.orphan && !new_branch) {
		new_branch = new_branch_to_free = worktree_basename_dup(path);
	} ಅನ್ಯಥಾ ಯದಿ (opts.orphan) {
		; /* no-op */
	} ಅನ್ಯಥಾ ಯದಿ (opts.detach) {
		/* Check HEAD */
		ಯದಿ (!strcmp(branch, "HEAD"))
			can_use_local_refs(&opts);
	} ಅನ್ಯಥಾ ಯದಿ (ac < 2 && new_branch) {
		/* DWIM: Infer --orphan when repo has no refs. */
		opts.orphan = dwim_orphan(&opts, !!opt_track, 0);
	} ಅನ್ಯಥಾ ಯದಿ (ac < 2) {
		/* DWIM: Guess branch name from path. */
		ವರ್ಣ *s = dwim_branch(&opts, path, &new_branch_to_free);
		ಯದಿ (s)
			branch = branch_to_free = s;
		new_branch = new_branch_to_free;

		/* DWIM: Infer --orphan when repo has no refs. */
		opts.orphan = (!s) && dwim_orphan(&opts, !!opt_track, 1);
	} ಅನ್ಯಥಾ ಯದಿ (ac == 2 && new_branch) {
		ಯದಿ (!strcmp(branch, "HEAD"))
			can_use_local_refs(&opts);
	} ಅನ್ಯಥಾ ಯದಿ (ac == 2) {
		ಸಂರಚನಾ commit *commit;

		commit = lookup_commit_reference_by_name(branch);
		ಯದಿ (!commit) {
			ಸಂರಚನಾ object_id oid;
			ವರ್ಣ *remote;
			ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_matches = 0;
			ಸಂರಚನಾ string_list matched_remote_names = STRING_LIST_INIT_DUP;

			remote = unique_tracking_name(branch, &oid, &ಅಂಕೆ_matches,
						      &matched_remote_names);
			ಯದಿ (remote) {
				new_branch = branch;
				branch = new_branch_to_free = remote;
			} ಅನ್ಯಥಾ ಯದಿ (ಅಂಕೆ_matches > 1) {
				ಯದಿ (!opts.quiet &&
				    advice_enabled(ADVICE_CHECKOUT_AMBIGUOUS_REMOTE_BRANCH_NAME))
					advise_disambiguating_remotes(path, branch,
								      &matched_remote_names);
				die(_("'%s' matched multiple (%d) remote tracking branches"),
				    branch, ಅಂಕೆ_matches);
			}
			string_list_clear(&matched_remote_names, 0);
		}

		ಯದಿ (!strcmp(branch, "HEAD"))
			can_use_local_refs(&opts);

	}

	ಯದಿ (!opts.orphan && !lookup_commit_reference_by_name(branch)) {
		ಪೂರ್ಣಾಙ್ಕ attempt_hint = !opts.quiet && (ac < 2);
		ಯದಿ (attempt_hint && used_new_branch_options) {
			advise_if_enabled(ADVICE_WORKTREE_ADD_ORPHAN,
				WORKTREE_ADD_ORPHAN_WITH_DASH_B_HINT_TEXT,
				new_branch, path);
		} ಅನ್ಯಥಾ ಯದಿ (attempt_hint) {
			advise_if_enabled(ADVICE_WORKTREE_ADD_ORPHAN,
				WORKTREE_ADD_ORPHAN_NO_DASH_B_HINT_TEXT, path);
		}
		die(_("invalid reference: %s"), branch);
	}

	ಯದಿ (!opts.quiet)
		print_preparing_worktree_ಸಾಲು(opts.detach, branch, new_branch, !!new_branch_force);

	ಯದಿ (opts.orphan) {
		branch = new_branch;
	} ಅನ್ಯಥಾ ಯದಿ (new_branch) {
		ಸಂರಚನಾ child_process cp = CHILD_PROCESS_INIT;
		cp.git_cmd = 1;
		strvec_ತಳ್ಳು(&cp.args, "branch");
		ಯದಿ (new_branch_force)
			strvec_ತಳ್ಳು(&cp.args, "--force");
		ಯದಿ (opts.quiet)
			strvec_ತಳ್ಳು(&cp.args, "--quiet");
		strvec_ತಳ್ಳು(&cp.args, new_branch);
		strvec_ತಳ್ಳು(&cp.args, branch);
		ಯದಿ (opt_track)
			strvec_ತಳ್ಳು(&cp.args, opt_track);
		ಯದಿ (run_command(&cp)) {
			ret = -1;
			ಗಚ್ಛ cleanup;
		}
		branch = new_branch;
	} ಅನ್ಯಥಾ ಯದಿ (opt_track) {
		die(_("--[no-]track can only be used if a new branch is created"));
	}

	ret = add_worktree(path, branch, &opts);
cleanup:
	ಮುಕ್ತ(path);
	ಮುಕ್ತ(opt_track);
	ಮುಕ್ತ(branch_to_free);
	ಮುಕ್ತ(new_branch_to_free);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಶೂನ್ಯ show_worktree_porcelain(ಸಂರಚನಾ worktree *wt, ಪೂರ್ಣಾಙ್ಕ ಸಾಲು_terminator)
{
	ಧ್ರುವ ವರ್ಣ *reason;

	printf("worktree %s%c", wt->path, ಸಾಲು_terminator);
	ಯದಿ (wt->is_bare)
		printf("bare%c", ಸಾಲು_terminator);
	ಅನ್ಯಥಾ {
		printf("HEAD %s%c", oid_to_hex(&wt->head_oid), ಸಾಲು_terminator);
		ಯದಿ (wt->is_detached)
			printf("detached%c", ಸಾಲು_terminator);
		ಅನ್ಯಥಾ ಯದಿ (wt->head_ref)
			printf("branch %s%c", wt->head_ref, ಸಾಲು_terminator);
	}

	reason = worktree_lock_reason(wt);
	ಯದಿ (reason) {
		fputs("locked", stdout);
		ಯದಿ (*reason) {
			fputc(' ', stdout);
			write_name_quoted(reason, stdout, ಸಾಲು_terminator);
		} ಅನ್ಯಥಾ {
			fputc(ಸಾಲು_terminator, stdout);
		}
	}

	reason = worktree_prune_reason(wt, expire);
	ಯದಿ (reason)
		printf("prunable %s%c", reason, ಸಾಲು_terminator);

	fputc(ಸಾಲು_terminator, stdout);
}

ಸಂರಚನಾ worktree_display {
	ವರ್ಣ *path;
	ಪೂರ್ಣಾಙ್ಕ width;
};

ಸ್ಥಿರ ಶೂನ್ಯ show_worktree(ಸಂರಚನಾ worktree *wt, ಸಂರಚನಾ worktree_display *display,
			  ಪೂರ್ಣಾಙ್ಕ path_maxwidth, ಪೂರ್ಣಾಙ್ಕ abbrev_len)
{
	ಸಂರಚನಾ strbuf sb = STRBUF_INIT;
	ಧ್ರುವ ವರ್ಣ *reason;

	strbuf_addf(&sb, "%s%*s", display->path, 1 + path_maxwidth - display->width, "");
	ಯದಿ (wt->is_bare)
		strbuf_addstr(&sb, "(bare)");
	ಅನ್ಯಥಾ {
		strbuf_addf(&sb, "%-*s ", abbrev_len,
				repo_find_unique_abbrev(the_repository, &wt->head_oid, DEFAULT_ABBREV));
		ಯದಿ (wt->is_detached)
			strbuf_addstr(&sb, "(detached HEAD)");
		ಅನ್ಯಥಾ ಯದಿ (wt->head_ref) {
			ವರ್ಣ *ref = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
								 wt->head_ref,
								 0);
			strbuf_addf(&sb, "[%s]", ref);
			ಮುಕ್ತ(ref);
		} ಅನ್ಯಥಾ
			strbuf_addstr(&sb, "(error)");
	}

	reason = worktree_lock_reason(wt);
	ಯದಿ (verbose && reason && *reason)
		strbuf_addf(&sb, "\n\tlocked: %s", reason);
	ಅನ್ಯಥಾ ಯದಿ (reason)
		strbuf_addstr(&sb, " locked");

	reason = worktree_prune_reason(wt, expire);
	ಯದಿ (verbose && reason)
		strbuf_addf(&sb, "\n\tprunable: %s", reason);
	ಅನ್ಯಥಾ ಯದಿ (reason)
		strbuf_addstr(&sb, " prunable");

	printf("%s\n", sb.buf);
	strbuf_release(&sb);
}

ಸ್ಥಿರ ಶೂನ್ಯ measure_widths(ಸಂರಚನಾ worktree **wt, ಪೂರ್ಣಾಙ್ಕ *abbrev,
			   ಸಂರಚನಾ worktree_display **d, ಪೂರ್ಣಾಙ್ಕ *maxwidth)
{
	ಪೂರ್ಣಾಙ್ಕ i, display_alloc = 0;
	ಸಂರಚನಾ worktree_display *display = ರಿಕ್ತ;
	ಸಂರಚನಾ strbuf buf = STRBUF_INIT;

	ಪ್ರತಿ (i = 0; wt[i]; i++) {
		ಪೂರ್ಣಾಙ್ಕ sha1_len;
		ALLOC_GROW(display, i + 1, display_alloc);
		quote_path(wt[i]->path, ರಿಕ್ತ, &buf, 0);
		display[i].width = utf8_strwidth(buf.buf);
		display[i].path = strbuf_detach(&buf, ರಿಕ್ತ);

		ಯದಿ (display[i].width > *maxwidth)
			*maxwidth = display[i].width;
		sha1_len = strlen(repo_find_unique_abbrev(the_repository, &wt[i]->head_oid, *abbrev));
		ಯದಿ (sha1_len > *abbrev)
			*abbrev = sha1_len;
	}
	*d = display;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ pathcmp(ಧ್ರುವ ಶೂನ್ಯ *a_, ಧ್ರುವ ಶೂನ್ಯ *b_)
{
	ಧ್ರುವ ಸಂರಚನಾ worktree *ಧ್ರುವ *a = a_;
	ಧ್ರುವ ಸಂರಚನಾ worktree *ಧ್ರುವ *b = b_;
	ಪ್ರತ್ಯಾಗಮ fspathcmp((*a)->path, (*b)->path);
}

ಸ್ಥಿರ ಶೂನ್ಯ pathsort(ಸಂರಚನಾ worktree **wt)
{
	ಪೂರ್ಣಾಙ್ಕ n = 0;
	ಸಂರಚನಾ worktree **p = wt;

	ಯಾವತ್ (*p++)
		n++;
	QSORT(wt, n, pathcmp);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ list(ಪೂರ್ಣಾಙ್ಕ ac, ಧ್ರುವ ವರ್ಣ **av, ಧ್ರುವ ವರ್ಣ *prefix,
		ಸಂರಚನಾ repository *repo UNUSED)
{
	ಪೂರ್ಣಾಙ್ಕ porcelain = 0;
	ಪೂರ್ಣಾಙ್ಕ ಸಾಲು_terminator = '\n';

	ಸಂರಚನಾ option options[] = {
		OPT_BOOL(0, "porcelain", &porcelain, N_("machine-readable output")),
		OPT__VERBOSE(&verbose, N_("show extended annotations and reasons, if available")),
		OPT_EXPIRY_DATE(0, "expire", &expire,
				N_("add 'prunable' annotation to missing worktrees older than <time>")),
		OPT_SET_INT('z', ರಿಕ್ತ, &ಸಾಲು_terminator,
			    N_("terminate records with a NUL character"), '\0'),
		OPT_END()
	};

	expire = TIME_MAX;
	ac = parse_options(ac, av, prefix, options, git_worktree_list_usage, 0);
	ಯದಿ (ac)
		usage_with_options(git_worktree_list_usage, options);
	ಅನ್ಯಥಾ ಯದಿ (verbose && porcelain)
		die(_("options '%s' and '%s' cannot be used together"), "--verbose", "--porcelain");
	ಅನ್ಯಥಾ ಯದಿ (!ಸಾಲು_terminator && !porcelain)
		die(_("the option '%s' requires '%s'"), "-z", "--porcelain");
	ಅನ್ಯಥಾ {
		ಸಂರಚನಾ worktree **worktrees = get_worktrees(the_repository);
		ಪೂರ್ಣಾಙ್ಕ path_maxwidth = 0, abbrev = DEFAULT_ABBREV, i;
		ಸಂರಚನಾ worktree_display *display = ರಿಕ್ತ;

		/* sort worktrees by path but keep main worktree at top */
		pathsort(worktrees + 1);

		ಯದಿ (!porcelain)
			measure_widths(worktrees, &abbrev,
				       &display, &path_maxwidth);

		ಪ್ರತಿ (i = 0; worktrees[i]; i++) {
			ಯದಿ (porcelain)
				show_worktree_porcelain(worktrees[i],
							ಸಾಲು_terminator);
			ಅನ್ಯಥಾ
				show_worktree(worktrees[i],
					      &display[i], path_maxwidth, abbrev);
		}
		ಪ್ರತಿ (i = 0; display && worktrees[i]; i++)
			ಮುಕ್ತ(display[i].path);
		ಮುಕ್ತ(display);
		free_worktrees(worktrees);
	}
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ lock_worktree(ಪೂರ್ಣಾಙ್ಕ ac, ಧ್ರುವ ವರ್ಣ **av, ಧ್ರುವ ವರ್ಣ *prefix,
			 ಸಂರಚನಾ repository *repo UNUSED)
{
	ಧ್ರುವ ವರ್ಣ *reason = "", *old_reason;
	ಸಂರಚನಾ option options[] = {
		OPT_STRING(0, "reason", &reason, N_("string"),
			   N_("reason for locking")),
		OPT_END()
	};
	ಸಂರಚನಾ worktree **worktrees, *wt;
	ವರ್ಣ *path;

	ac = parse_options(ac, av, prefix, options, git_worktree_lock_usage, 0);
	ಯದಿ (ac != 1)
		usage_with_options(git_worktree_lock_usage, options);

	worktrees = get_worktrees(the_repository);
	wt = find_worktree(worktrees, prefix, av[0]);
	ಯದಿ (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	ಯದಿ (is_main_worktree(wt))
		die(_("The main working tree cannot be locked or unlocked"));

	old_reason = worktree_lock_reason(wt);
	ಯದಿ (old_reason) {
		ಯದಿ (*old_reason)
			die(_("'%s' is already locked, reason: %s"),
			    av[0], old_reason);
		die(_("'%s' is already locked"), av[0]);
	}

	path = repo_common_path(the_repository, "worktrees/%s/locked", wt->id);
	write_file(path, "%s", reason);

	free_worktrees(worktrees);
	ಮುಕ್ತ(path);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ unlock_worktree(ಪೂರ್ಣಾಙ್ಕ ac, ಧ್ರುವ ವರ್ಣ **av, ಧ್ರುವ ವರ್ಣ *prefix,
			   ಸಂರಚನಾ repository *repo UNUSED)
{
	ಸಂರಚನಾ option options[] = {
		OPT_END()
	};
	ಸಂರಚನಾ worktree **worktrees, *wt;
	ವರ್ಣ *path;
	ಪೂರ್ಣಾಙ್ಕ ret;

	ac = parse_options(ac, av, prefix, options, git_worktree_unlock_usage, 0);
	ಯದಿ (ac != 1)
		usage_with_options(git_worktree_unlock_usage, options);

	worktrees = get_worktrees(the_repository);
	wt = find_worktree(worktrees, prefix, av[0]);
	ಯದಿ (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	ಯದಿ (is_main_worktree(wt))
		die(_("The main working tree cannot be locked or unlocked"));
	ಯದಿ (!worktree_lock_reason(wt))
		die(_("'%s' is not locked"), av[0]);

	path = repo_common_path(the_repository, "worktrees/%s/locked", wt->id);
	ret = unlink_or_warn(path);

	free_worktrees(worktrees);
	ಮುಕ್ತ(path);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಶೂನ್ಯ validate_no_submodules(ಧ್ರುವ ಸಂರಚನಾ worktree *wt)
{
	ಸಂರಚನಾ index_state istate = INDEX_STATE_INIT(the_repository);
	ಸಂರಚನಾ strbuf path = STRBUF_INIT;
	ಪೂರ್ಣಾಙ್ಕ i, found_submodules = 0;
	ವರ್ಣ *wt_gitdir;

	wt_gitdir = get_worktree_git_dir(wt);

	ಯದಿ (is_directory(worktree_git_path(wt, "modules"))) {
		/*
		 * There could be false positives, e.g. the "modules"
		 * directory exists but is empty. But it's a rare case and
		 * this simpler check is probably good enough for now.
		 */
		found_submodules = 1;
	} ಅನ್ಯಥಾ ಯದಿ (read_index_from(&istate, worktree_git_path(wt, "index"),
				   wt_gitdir) > 0) {
		ಪ್ರತಿ (i = 0; i < istate.cache_nr; i++) {
			ಸಂರಚನಾ cache_entry *ce = istate.cache[i];
			ಪೂರ್ಣಾಙ್ಕ err;

			ಯದಿ (!S_ISGITLINK(ce->ce_mode))
				ಅನುವರ್ತ;

			strbuf_reset(&path);
			strbuf_addf(&path, "%s/%s", wt->path, ce->name);
			ಯದಿ (!is_submodule_populated_gently(path.buf, &err))
				ಅನುವರ್ತ;

			found_submodules = 1;
			ಭಙ್ಗ;
		}
	}
	discard_index(&istate);
	strbuf_release(&path);
	ಮುಕ್ತ(wt_gitdir);

	ಯದಿ (found_submodules)
		die(_("working trees containing submodules cannot be moved or removed"));
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ move_worktree(ಪೂರ್ಣಾಙ್ಕ ac, ಧ್ರುವ ವರ್ಣ **av, ಧ್ರುವ ವರ್ಣ *prefix,
			 ಸಂರಚನಾ repository *repo UNUSED)
{
	ಪೂರ್ಣಾಙ್ಕ force = 0;
	ಸಂರಚನಾ option options[] = {
		OPT__FORCE(&force,
			 N_("force move even if worktree is dirty or locked"),
			 PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "relative-paths", &use_relative_paths,
			 N_("use relative paths for worktrees")),
		OPT_END()
	};
	ಸಂರಚನಾ worktree **worktrees, *wt;
	ಸಂರಚನಾ strbuf dst = STRBUF_INIT;
	ಸಂರಚನಾ strbuf errmsg = STRBUF_INIT;
	ಧ್ರುವ ವರ್ಣ *reason = ರಿಕ್ತ;
	ವರ್ಣ *path;

	ac = parse_options(ac, av, prefix, options, git_worktree_move_usage,
			   0);
	ಯದಿ (ac != 2)
		usage_with_options(git_worktree_move_usage, options);

	path = prefix_filename(prefix, av[1]);
	strbuf_addstr(&dst, path);
	ಮುಕ್ತ(path);

	worktrees = get_worktrees(the_repository);
	wt = find_worktree(worktrees, prefix, av[0]);
	ಯದಿ (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	ಯದಿ (is_main_worktree(wt))
		die(_("'%s' is a main working tree"), av[0]);
	ಯದಿ (is_directory(dst.buf)) {
		ಧ್ರುವ ವರ್ಣ *sep = find_last_dir_sep(wt->path);

		ಯದಿ (!sep)
			die(_("could not figure out destination name from '%s'"),
			    wt->path);
		strbuf_trim_trailing_dir_sep(&dst);
		strbuf_addstr(&dst, sep);
	}
	check_candidate_path(dst.buf, force, worktrees, "move");

	validate_no_submodules(wt);

	ಯದಿ (force < 2)
		reason = worktree_lock_reason(wt);
	ಯದಿ (reason) {
		ಯದಿ (*reason)
			die(_("cannot move a locked working tree, lock reason: %s\nuse 'move -f -f' to override or unlock first"),
			    reason);
		die(_("cannot move a locked working tree;\nuse 'move -f -f' to override or unlock first"));
	}
	ಯದಿ (validate_worktree(wt, &errmsg, 0))
		die(_("validation failed, cannot move working tree: %s"),
		    errmsg.buf);
	strbuf_release(&errmsg);

	ಯದಿ (rename(wt->path, dst.buf) == -1)
		die_errno(_("failed to move '%s' to '%s'"), wt->path, dst.buf);

	update_worktree_location(wt, dst.buf, use_relative_paths);

	strbuf_release(&dst);
	free_worktrees(worktrees);
	ಪ್ರತ್ಯಾಗಮ 0;
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
ಸ್ಥಿರ ಶೂನ್ಯ check_clean_worktree(ಸಂರಚನಾ worktree *wt,
				 ಧ್ರುವ ವರ್ಣ *original_path)
{
	ಸಂರಚನಾ child_process cp;
	ವರ್ಣ buf[1];
	ಪೂರ್ಣಾಙ್ಕ ret;

	/*
	 * Until we sort this out, all submodules are "dirty" and
	 * will abort this function.
	 */
	validate_no_submodules(wt);

	child_process_init(&cp);
	strvec_pushf(&cp.env, "%s=%s/.git",
		     GIT_DIR_ENVIRONMENT, wt->path);
	strvec_pushf(&cp.env, "%s=%s",
		     GIT_WORK_ವೃಕ್ಷ_ENVIRONMENT, wt->path);
	strvec_pushl(&cp.args, "status",
		     "--porcelain", "--ignore-submodules=none",
		     ರಿಕ್ತ);
	cp.git_cmd = 1;
	cp.dir = wt->path;
	cp.out = -1;
	ret = start_command(&cp);
	ಯದಿ (ret)
		die_errno(_("failed to run 'git status' on '%s'"),
			  original_path);
	ret = xread(cp.out, buf, ಪರಿಮಾಣ(buf));
	ಯದಿ (ret)
		die(_("'%s' contains modified or untracked files, use --force to delete it"),
		    original_path);
	close(cp.out);
	ret = finish_command(&cp);
	ಯದಿ (ret)
		die_errno(_("failed to run 'git status' on '%s', code %d"),
			  original_path, ret);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ delete_git_work_ವೃಕ್ಷ(ಸಂರಚನಾ worktree *wt)
{
	ಸಂರಚನಾ strbuf sb = STRBUF_INIT;
	ಪೂರ್ಣಾಙ್ಕ ret = 0;

	strbuf_addstr(&sb, wt->path);
	ಯದಿ (remove_dir_recursively(&sb, 0)) {
		error_errno(_("failed to delete '%s'"), sb.buf);
		ret = -1;
	}
	strbuf_release(&sb);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ remove_worktree(ಪೂರ್ಣಾಙ್ಕ ac, ಧ್ರುವ ವರ್ಣ **av, ಧ್ರುವ ವರ್ಣ *prefix,
			   ಸಂರಚನಾ repository *repo UNUSED)
{
	ಪೂರ್ಣಾಙ್ಕ force = 0;
	ಸಂರಚನಾ option options[] = {
		OPT__FORCE(&force,
			 N_("force removal even if worktree is dirty or locked"),
			 PARSE_OPT_NOCOMPLETE),
		OPT_END()
	};
	ಸಂರಚನಾ worktree **worktrees, *wt;
	ಸಂರಚನಾ strbuf errmsg = STRBUF_INIT;
	ಧ್ರುವ ವರ್ಣ *reason = ರಿಕ್ತ;
	ಪೂರ್ಣಾಙ್ಕ ret = 0;

	ac = parse_options(ac, av, prefix, options, git_worktree_remove_usage, 0);
	ಯದಿ (ac != 1)
		usage_with_options(git_worktree_remove_usage, options);

	worktrees = get_worktrees(the_repository);
	wt = find_worktree(worktrees, prefix, av[0]);
	ಯದಿ (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	ಯದಿ (is_main_worktree(wt))
		die(_("'%s' is a main working tree"), av[0]);
	ಯದಿ (force < 2)
		reason = worktree_lock_reason(wt);
	ಯದಿ (reason) {
		ಯದಿ (*reason)
			die(_("cannot remove a locked working tree, lock reason: %s\nuse 'remove -f -f' to override or unlock first"),
			    reason);
		die(_("cannot remove a locked working tree;\nuse 'remove -f -f' to override or unlock first"));
	}
	ಯದಿ (validate_worktree(wt, &errmsg, WT_VALIDATE_WORKTREE_MISSING_OK))
		die(_("validation failed, cannot remove working tree: %s"),
		    errmsg.buf);
	strbuf_release(&errmsg);

	ಯದಿ (file_exists(wt->path)) {
		ಯದಿ (!force)
			check_clean_worktree(wt, av[0]);

		ret |= delete_git_work_ವೃಕ್ಷ(wt);
	}
	/*
	 * continue on even if ret is non-zero, there's no going back
	 * from here.
	 */
	ret |= delete_git_dir(wt->id);
	delete_worktrees_dir_if_empty();

	free_worktrees(worktrees);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಶೂನ್ಯ report_repair(ಪೂರ್ಣಾಙ್ಕ iserr, ಧ್ರುವ ವರ್ಣ *path, ಧ್ರುವ ವರ್ಣ *ಸಂದೇಶ, ಶೂನ್ಯ *cb_data)
{
	ಯದಿ (!iserr) {
		fprintf_ln(stderr, _("repair: %s: %s"), ಸಂದೇಶ, path);
	} ಅನ್ಯಥಾ {
		ಪೂರ್ಣಾಙ್ಕ *exit_status = (ಪೂರ್ಣಾಙ್ಕ *)cb_data;
		fprintf_ln(stderr, _("error: %s: %s"), ಸಂದೇಶ, path);
		*exit_status = 1;
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ repair(ಪೂರ್ಣಾಙ್ಕ ac, ಧ್ರುವ ವರ್ಣ **av, ಧ್ರುವ ವರ್ಣ *prefix,
		  ಸಂರಚನಾ repository *repo UNUSED)
{
	ಧ್ರುವ ವರ್ಣ **p;
	ಧ್ರುವ ವರ್ಣ *self[] = { ".", ರಿಕ್ತ };
	ಸಂರಚನಾ option options[] = {
		OPT_BOOL(0, "relative-paths", &use_relative_paths,
			 N_("use relative paths for worktrees")),
		OPT_END()
	};
	ಪೂರ್ಣಾಙ್ಕ rc = 0;

	ac = parse_options(ac, av, prefix, options, git_worktree_repair_usage, 0);
	p = ac > 0 ? av : self;
	ಪ್ರತಿ (; *p; p++)
		repair_worktree_at_path(the_repository, *p, report_repair,
					&rc, use_relative_paths);
	repair_worktrees(the_repository, report_repair, &rc, use_relative_paths);
	ಪ್ರತ್ಯಾಗಮ rc;
}

ಪೂರ್ಣಾಙ್ಕ cmd_worktree(ಪೂರ್ಣಾಙ್ಕ ac,
		 ಧ್ರುವ ವರ್ಣ **av,
		 ಧ್ರುವ ವರ್ಣ *prefix,
		 ಸಂರಚನಾ repository *repo)
{
	parse_opt_subcommand_fn *fn = ರಿಕ್ತ;
	ಸಂರಚನಾ option options[] = {
		OPT_SUBCOMMAND("add", &fn, add),
		OPT_SUBCOMMAND("prune", &fn, prune),
		OPT_SUBCOMMAND("list", &fn, list),
		OPT_SUBCOMMAND("lock", &fn, lock_worktree),
		OPT_SUBCOMMAND("unlock", &fn, unlock_worktree),
		OPT_SUBCOMMAND("move", &fn, move_worktree),
		OPT_SUBCOMMAND("remove", &fn, remove_worktree),
		OPT_SUBCOMMAND("repair", &fn, repair),
		OPT_END()
	};

	repo_config(the_repository, git_worktree_config, ರಿಕ್ತ);

	ಯದಿ (!prefix)
		prefix = "";

	ac = parse_options(ac, av, prefix, options, git_worktree_usage, 0);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	ಪ್ರತ್ಯಾಗಮ fn(ac, av, prefix, repo);
}
