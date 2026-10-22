#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE
#ವ್ಯಾಖ್ಯಾನಿಸು DISABLE_SIGN_COMPARE_WARNINGS

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

#ವ್ಯಾಖ್ಯಾನಿಸು BUILTIN_WORKTREE_ಕೂಡಿಸು_USAGE \
	N_("git worktree add [-f] [--detach] [--checkout] [--lock [--reason <string>]]\n" \
	   "                 [--orphan] [(-b | -B) <new-branch>] <path> [<commit-ish>]")

#ವ್ಯಾಖ್ಯಾನಿಸು BUILTIN_WORKTREE_LIST_USAGE \
	N_("git worktree list [-v | --porcelain [-z]]")
#ವ್ಯಾಖ್ಯಾನಿಸು BUILTIN_WORKTREE_ಬೀಗ_USAGE \
	N_("git worktree lock [--reason <string>] <worktree>")
#ವ್ಯಾಖ್ಯಾನಿಸು BUILTIN_WORKTREE_MOVE_USAGE \
	N_("git worktree move <worktree> <new-path>")
#ವ್ಯಾಖ್ಯಾನಿಸು BUILTIN_WORKTREE_PRUNE_USAGE \
	N_("git worktree prune [-n] [-v] [--expire <expire>]")
#ವ್ಯಾಖ್ಯಾನಿಸು BUILTIN_WORKTREE_REMOVE_USAGE \
	N_("git worktree remove [-f] <worktree>")
#ವ್ಯಾಖ್ಯಾನಿಸು BUILTIN_WORKTREE_REPAIR_USAGE \
	N_("git worktree repair [<path>...]")
#ವ್ಯಾಖ್ಯಾನಿಸು BUILTIN_WORKTREE_UNLOCK_USAGE \
	N_("git worktree unlock <worktree>")

#ವ್ಯಾಖ್ಯಾನಿಸು WORKTREE_ಕೂಡಿಸು_DWIM_ORPHAN_INFER_ಪಠ್ಯ \
	_("No possible source branch, inferring '--orphan'")

#ವ್ಯಾಖ್ಯಾನಿಸು WORKTREE_ಕೂಡಿಸು_ORPHAN_WITH_DASH_B_HINT_ಪಠ್ಯ \
	_("If you meant to create a worktree containing a new unborn branch\n" \
	"(branch with no commits) for this repository, you can do so\n" \
	"using the --orphan flag:\n" \
	"\n" \
	"    git worktree add --orphan -b %s %s\n")

#ವ್ಯಾಖ್ಯಾನಿಸು WORKTREE_ಕೂಡಿಸು_ORPHAN_NO_DASH_B_HINT_ಪಠ್ಯ \
	_("If you meant to create a worktree containing a new unborn branch\n" \
	"(branch with no commits) for this repository, you can do so\n" \
	"using the --orphan flag:\n" \
	"\n" \
	"    git worktree add --orphan %s\n")

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ git_worktree_usage[] = {
	BUILTIN_WORKTREE_ಕೂಡಿಸು_USAGE,
	BUILTIN_WORKTREE_LIST_USAGE,
	BUILTIN_WORKTREE_ಬೀಗ_USAGE,
	BUILTIN_WORKTREE_MOVE_USAGE,
	BUILTIN_WORKTREE_PRUNE_USAGE,
	BUILTIN_WORKTREE_REMOVE_USAGE,
	BUILTIN_WORKTREE_REPAIR_USAGE,
	BUILTIN_WORKTREE_UNLOCK_USAGE,
	ಖಾಲಿ
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ git_worktree_ಕೂಡಿಸು_usage[] = {
	BUILTIN_WORKTREE_ಕೂಡಿಸು_USAGE,
	ಖಾಲಿ,
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ git_worktree_list_usage[] = {
	BUILTIN_WORKTREE_LIST_USAGE,
	ಖಾಲಿ
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ git_worktree_ಬೀಗ_usage[] = {
	BUILTIN_WORKTREE_ಬೀಗ_USAGE,
	ಖಾಲಿ
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ git_worktree_move_usage[] = {
	BUILTIN_WORKTREE_MOVE_USAGE,
	ಖಾಲಿ
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ git_worktree_prune_usage[] = {
	BUILTIN_WORKTREE_PRUNE_USAGE,
	ಖಾಲಿ
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ git_worktree_remove_usage[] = {
	BUILTIN_WORKTREE_REMOVE_USAGE,
	ಖಾಲಿ
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ git_worktree_repair_usage[] = {
	BUILTIN_WORKTREE_REPAIR_USAGE,
	ಖಾಲಿ
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ git_worktree_unlock_usage[] = {
	BUILTIN_WORKTREE_UNLOCK_USAGE,
	ಖಾಲಿ
};

ರಚನೆ ಕೂಡಿಸು_opts {
	ಪೂರ್ಣಾಂಕ force;
	ಪೂರ್ಣಾಂಕ detach;
	ಪೂರ್ಣಾಂಕ quiet;
	ಪೂರ್ಣಾಂಕ checkout;
	ಪೂರ್ಣಾಂಕ orphan;
	ಪೂರ್ಣಾಂಕ relative_paths;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *keep_locked;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_only;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ verbose;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ guess_remote;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ use_relative_paths;
ಸ್ಥಿರ timestamp_t expire;

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ git_worktree_config(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *var, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ,
			       ಸ್ಥಿರಾಂಕ ರಚನೆ config_context *ctx, ಶೂನ್ಯ *cb)
{
	ಆದರೆ (!strcmp(var, "worktree.guessremote")) {
		guess_remote = git_config_bool(var, ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(var, "worktree.userelativepaths")) {
		use_relative_paths = git_config_bool(var, ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಹಿಂತಿರುಗಿ git_default_config(var, ಮೌಲ್ಯ, ctx, cb);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ delete_git_dir(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id)
{
	ರಚನೆ strbuf sb = STRBUF_INIT;
	ಪೂರ್ಣಾಂಕ ret;

	repo_common_ಮಾರ್ಗ_append(the_repository, &sb, "worktrees/%s", id);
	ret = remove_dir_recursively(&sb, 0);
	ಆದರೆ (ret < 0 && errno == ENOTDIR)
		ret = unlink(sb.buf);
	ಆದರೆ (ret)
		error_errno(_("failed to delete '%s'"), sb.buf);
	strbuf_release(&sb);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಶೂನ್ಯ delete_worktrees_dir_if_ಖಾಲಿಗಳು(ಶೂನ್ಯ)
{
	ಅಕ್ಷರ *ಮಾರ್ಗ = repo_git_ಮಾರ್ಗ(the_repository, "worktrees");
	rmdir(ಮಾರ್ಗ); /* ignore failed removal */
	ಬಿಡುಗಡೆ(ಮಾರ್ಗ);
}

ಸ್ಥಿರ ಶೂನ್ಯ prune_worktree(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *reason)
{
	ಆದರೆ (show_only || verbose)
		fprintf_ln(stderr, _("Removing %s/%s: %s"), "worktrees", id, reason);
	ಆದರೆ (!show_only)
		delete_git_dir(id);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ prune_cmp(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *a, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *b)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ string_list_ಅಂಶ *x = a;
	ಸ್ಥಿರಾಂಕ ರಚನೆ string_list_ಅಂಶ *y = b;
	ಪೂರ್ಣಾಂಕ c;

	ಆದರೆ ((c = fspathcmp(x->string, y->string)))
	    ಹಿಂತಿರುಗಿ c;
	/*
	 * paths same; prune_dupes() removes all but the first worktree entry
	 * having the same path, so sort main worktree ('util' is NULL) above
	 * linked worktrees ('util' not NULL) since main worktree can't be
	 * removed
	 */
	ಆದರೆ (!x->util)
		ಹಿಂತಿರುಗಿ -1;
	ಆದರೆ (!y->util)
		ಹಿಂತಿರುಗಿ 1;
	/* paths same; sort by .git/worktrees/<id> */
	ಹಿಂತಿರುಗಿ strcmp(x->util, y->util);
}

ಸ್ಥಿರ ಶೂನ್ಯ prune_dups(ರಚನೆ string_list *l)
{
	ಪೂರ್ಣಾಂಕ i;

	QSORT(l->ವಸ್ತುಗಳು, l->nr, prune_cmp);
	ಗಾಗಿ (i = 1; i < l->nr; i++) {
		ಆದರೆ (!fspathcmp(l->ವಸ್ತುಗಳು[i].string, l->ವಸ್ತುಗಳು[i - 1].string))
			prune_worktree(l->ವಸ್ತುಗಳು[i].util, "duplicate entry");
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ prune_worktrees(ಶೂನ್ಯ)
{
	ರಚನೆ strbuf reason = STRBUF_INIT;
	ರಚನೆ strbuf main_ಮಾರ್ಗ = STRBUF_INIT;
	ರಚನೆ string_list kept = STRING_LIST_INIT_DUP;
	ಅಕ್ಷರ *ಮಾರ್ಗ;
	DIR *dir;
	ರಚನೆ dirent *d;

	ಮಾರ್ಗ = repo_git_ಮಾರ್ಗ(the_repository, "worktrees");
	dir = opendir(ಮಾರ್ಗ);
	ಬಿಡುಗಡೆ(ಮಾರ್ಗ);
	ಆದರೆ (!dir)
		ಹಿಂತಿರುಗಿ;
	ಆಗ ((d = readdir_skip_dot_and_dotdot(dir)) != ಖಾಲಿ) {
		ಅಕ್ಷರ *ಮಾರ್ಗ;
		strbuf_reset(&reason);
		ಆದರೆ (should_prune_worktree(the_repository, d->d_name, &reason, &ಮಾರ್ಗ, expire))
			prune_worktree(d->d_name, reason.buf);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಮಾರ್ಗ)
			string_list_append_nodup(&kept, ಮಾರ್ಗ)->util = xstrdup(d->d_name);
	}
	closedir(dir);

	strbuf_ಕೂಡಿಸು_absolute_ಮಾರ್ಗ(&main_ಮಾರ್ಗ, repo_get_common_dir(the_repository));
	/* massage main worktree absolute path to match 'gitdir' content */
	strbuf_strip_suffix(&main_ಮಾರ್ಗ, "/.");
	string_list_append_nodup(&kept, strbuf_detach(&main_ಮಾರ್ಗ, ಖಾಲಿ));
	prune_dups(&kept);
	string_list_clear(&kept, 1);

	ಆದರೆ (!show_only)
		delete_worktrees_dir_if_ಖಾಲಿಗಳು();
	strbuf_release(&reason);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ prune(ಪೂರ್ಣಾಂಕ ac, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **av, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		 ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ option options[] = {
		OPT_DRY_ಸತತ(&show_only, N_("do not remove, show only")),
		OPT__VERBOSE(&verbose, N_("report pruned working trees")),
		OPT_EXPIRY_DATE(0, "expire", &expire,
				N_("prune missing working trees older than <time>")),
		OPT_ಕೊನೆ()
	};

	expire = ಸಮಯ_MAX;
	ac = parse_options(ac, av, ಪೂರ್ವಪ್ರತ್ಯಯ, options, git_worktree_prune_usage,
			   0);
	ಆದರೆ (ac)
		usage_with_options(git_worktree_prune_usage, options);
	prune_worktrees();
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಅಕ್ಷರ *junk_work_ವೃಕ್ಷ;
ಸ್ಥಿರ ಅಕ್ಷರ *junk_git_dir;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ is_junk;
ಸ್ಥಿರ pid_t junk_pid;

ಸ್ಥಿರ ಶೂನ್ಯ remove_junk(ಶೂನ್ಯ)
{
	ರಚನೆ strbuf sb = STRBUF_INIT;
	ಆದರೆ (!is_junk || getpid() != junk_pid)
		ಹಿಂತಿರುಗಿ;
	ಆದರೆ (junk_git_dir) {
		strbuf_addstr(&sb, junk_git_dir);
		remove_dir_recursively(&sb, 0);
		strbuf_reset(&sb);
	}
	ಆದರೆ (junk_work_ವೃಕ್ಷ) {
		strbuf_addstr(&sb, junk_work_ವೃಕ್ಷ);
		remove_dir_recursively(&sb, 0);
	}
	strbuf_release(&sb);
}

ಸ್ಥಿರ ಶೂನ್ಯ remove_junk_on_signal(ಪೂರ್ಣಾಂಕ signo)
{
	remove_junk();
	sigchain_ತೆಗೆ(signo);
	ಎತ್ತು(signo);
}

ಸ್ಥಿರ ಅಕ್ಷರ *worktree_basename_dup(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name;
	ಪೂರ್ಣಾಂಕ len;

	len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ಮಾರ್ಗ);
	ಆಗ (len && is_dir_sep(ಮಾರ್ಗ[len - 1]))
		len--;

	name = ಮಾರ್ಗ + len;
	ಆಗ (name > ಮಾರ್ಗ && !is_dir_sep(name[-1]))
		name--;

	ಹಿಂತಿರುಗಿ xmemdupz(name, ಮಾರ್ಗ + len - name);
}

/* check that path is viable location for worktree */
ಸ್ಥಿರ ಶೂನ್ಯ check_candidate_ಮಾರ್ಗ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ,
				 ಪೂರ್ಣಾಂಕ force,
				 ರಚನೆ worktree **worktrees,
				 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cmd)
{
	ರಚನೆ worktree *wt;
	ಪೂರ್ಣಾಂಕ locked;

	ಆದರೆ (file_exists(ಮಾರ್ಗ) && !is_ಖಾಲಿಗಳು_dir(ಮಾರ್ಗ))
		die(_("'%s' already exists"), ಮಾರ್ಗ);

	wt = ಕಂಡುಹಿಡಿ_worktree_by_ಮಾರ್ಗ(worktrees, ಮಾರ್ಗ);
	ಆದರೆ (!wt)
		ಹಿಂತಿರುಗಿ;

	locked = !!worktree_ಬೀಗ_reason(wt);
	ಆದರೆ ((!locked && force) || (locked && force > 1)) {
		ಆದರೆ (delete_git_dir(wt->id))
		    die(_("unusable worktree destination '%s'"), ಮಾರ್ಗ);
		ಹಿಂತಿರುಗಿ;
	}

	ಆದರೆ (locked)
		die(_("'%s' is a missing but locked worktree;\nuse '%s -f -f' to override, or 'unlock' and 'prune' or 'remove' to clear"), ಮಾರ್ಗ, cmd);
	ಇಲ್ಲದಿದ್ದರೆ
		die(_("'%s' is a missing but already registered worktree;\nuse '%s -f' to override, or 'prune' or 'remove' to clear"), ಮಾರ್ಗ, cmd);
}

ಸ್ಥಿರ ಶೂನ್ಯ copy_sparse_checkout(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *worktree_git_dir)
{
	ಅಕ್ಷರ *from_file = repo_git_ಮಾರ್ಗ(the_repository, "info/sparse-checkout");
	ಅಕ್ಷರ *ಗಮ್ಯ_file = xstrfmt("%s/info/sparse-checkout", worktree_git_dir);

	ಆದರೆ (file_exists(from_file)) {
		ಆದರೆ (ಸುರಕ್ಷಿತ_create_leading_directories(the_repository, ಗಮ್ಯ_file) ||
			copy_file(the_repository, ಗಮ್ಯ_file, from_file, 0666))
			error(_("failed to copy '%s' to '%s'; sparse-checkout may not work correctly"),
				from_file, ಗಮ್ಯ_file);
	}

	ಬಿಡುಗಡೆ(from_file);
	ಬಿಡುಗಡೆ(ಗಮ್ಯ_file);
}

ಸ್ಥಿರ ಶೂನ್ಯ copy_filtered_worktree_config(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *worktree_git_dir)
{
	ಅಕ್ಷರ *from_file = repo_git_ಮಾರ್ಗ(the_repository, "config.worktree");
	ಅಕ್ಷರ *ಗಮ್ಯ_file = xstrfmt("%s/config.worktree", worktree_git_dir);

	ಆದರೆ (file_exists(from_file)) {
		ರಚನೆ config_ಗಣ cs = { { 0 } };
		ಪೂರ್ಣಾಂಕ bare;

		ಆದರೆ (ಸುರಕ್ಷಿತ_create_leading_directories(the_repository, ಗಮ್ಯ_file) ||
			copy_file(the_repository, ಗಮ್ಯ_file, from_file, 0666)) {
			error(_("failed to copy worktree config from '%s' to '%s'"),
				from_file, ಗಮ್ಯ_file);
			ಹೋಗು worktree_copy_cleanup;
		}

		git_configset_init(&cs);
		git_configset_ಕೂಡಿಸು_file(&cs, from_file);

		ಆದರೆ (!git_configset_get_bool(&cs, "core.bare", &bare) &&
			bare &&
			repo_config_ಗಣ_multivar_ಒಳ_file_gently(the_repository,
				ಗಮ್ಯ_file, "core.bare", ಖಾಲಿ, "true", ಖಾಲಿ, 0))
			error(_("failed to unset '%s' in '%s'"),
				"core.bare", ಗಮ್ಯ_file);
		ಆದರೆ (!git_configset_get(&cs, "core.worktree") &&
			repo_config_ಗಣ_ಒಳ_file_gently(the_repository, ಗಮ್ಯ_file,
						       "core.worktree", ಖಾಲಿ, ಖಾಲಿ))
			error(_("failed to unset '%s' in '%s'"),
				"core.worktree", ಗಮ್ಯ_file);

		git_configset_clear(&cs);
	}

worktree_copy_cleanup:
	ಬಿಡುಗಡೆ(from_file);
	ಬಿಡುಗಡೆ(ಗಮ್ಯ_file);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ checkout_worktree(ಸ್ಥಿರಾಂಕ ರಚನೆ ಕೂಡಿಸು_opts *opts,
			     ರಚನೆ strvec *ಮಗು_env)
{
	ರಚನೆ ಮಗು_process cp = ಮಗು_PROCESS_INIT;
	cp.git_cmd = 1;
	strvec_pushl(&cp.ವಾದಗಳು, "reset", "--hard", "--no-recurse-submodules", ಖಾಲಿ);
	ಆದರೆ (opts->quiet)
		strvec_ತಳ್ಳು(&cp.ವಾದಗಳು, "--quiet");
	strvec_pushv(&cp.env, ಮಗು_env->v);
	ಹಿಂತಿರುಗಿ ಸತತ_command(&cp);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ make_worktree_orphan(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ref, ಸ್ಥಿರಾಂಕ ರಚನೆ ಕೂಡಿಸು_opts *opts,
				ರಚನೆ strvec *ಮಗು_env)
{
	ರಚನೆ strbuf symref = STRBUF_INIT;
	ರಚನೆ ಮಗು_process cp = ಮಗು_PROCESS_INIT;

	validate_new_branchname(ref, &symref, 0);
	strvec_pushl(&cp.ವಾದಗಳು, "symbolic-ref", "HEAD", symref.buf, ಖಾಲಿ);
	ಆದರೆ (opts->quiet)
		strvec_ತಳ್ಳು(&cp.ವಾದಗಳು, "--quiet");
	strvec_pushv(&cp.env, ಮಗು_env->v);
	strbuf_release(&symref);
	cp.git_cmd = 1;
	ಹಿಂತಿರುಗಿ ಸತತ_command(&cp);
}

/*
 * References for worktrees are generally stored in '$GIT_DIR/worktrees/<wt_id>'.
 * But when using alternate reference directories, we want to store the worktree
 * references in '$ALTERNATE_REFERENCE_DIR/worktrees/<wt_id>'.
 *
 * Create the necessary folder structure to facilitate the same. But to ensure
 * that the former path is still considered a Git directory, add stubs.
 */
ಸ್ಥಿರ ಶೂನ್ಯ ಸಿದ್ಧತೆ_alternate_ref_dir(ರಚನೆ worktree *wt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *wt_git_ಮಾರ್ಗ)
{
	ರಚನೆ strbuf sb = STRBUF_INIT;
	ಅಕ್ಷರ *ಮಾರ್ಗ;

	ಮಾರ್ಗ = wt->repo->ref_storage_payload;
	ಆದರೆ (!ಮಾರ್ಗ)
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (!is_absolute_ಮಾರ್ಗ(ಮಾರ್ಗ))
		strbuf_addf(&sb, "%s/", wt->repo->commondir);

	strbuf_addf(&sb, "%s/worktrees", ಮಾರ್ಗ);
	ಸುರಕ್ಷಿತ_create_dir(wt->repo, sb.buf, 1);
	strbuf_addf(&sb, "/%s", wt->id);
	ಸುರಕ್ಷಿತ_create_dir(wt->repo, sb.buf, 1);
	strbuf_reset(&sb);

	strbuf_addf(&sb, "this worktree stores references in %s/worktrees/%s",
		    ಮಾರ್ಗ, wt->id);
	refs_create_refdir_stubs(wt->repo, wt_git_ಮಾರ್ಗ, sb.buf);

	strbuf_release(&sb);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಕೂಡಿಸು_worktree(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *refname,
			ಸ್ಥಿರಾಂಕ ರಚನೆ ಕೂಡಿಸು_opts *opts)
{
	ರಚನೆ strbuf sb_git = STRBUF_INIT, sb_repo = STRBUF_INIT;
	ರಚನೆ strbuf sb = STRBUF_INIT;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name;
	ಅಕ್ಷರ *name_ಗಮ್ಯ_free = ಖಾಲಿ;
	ರಚನೆ strvec ಮಗು_env = STRVEC_INIT;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ counter = 0;
	ಪೂರ್ಣಾಂಕ len, ret;
	ರಚನೆ strbuf symref = STRBUF_INIT;
	ರಚನೆ commit *commit = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ is_branch = 0;
	ರಚನೆ strbuf sb_name = STRBUF_INIT;
	ರಚನೆ worktree **worktrees, *wt = ಖಾಲಿ;
	ರಚನೆ ref_store *wt_refs;
	ರಚನೆ repo_config_ಮೌಲ್ಯಗಳು *cfg = repo_config_ಮೌಲ್ಯಗಳು(the_repository);

	worktrees = get_worktrees(the_repository);
	check_candidate_ಮಾರ್ಗ(ಮಾರ್ಗ, opts->force, worktrees, "add");
	free_worktrees(worktrees);
	worktrees = ಖಾಲಿ;

	/* is 'refname' a branch or commit? */
	ಆದರೆ (!opts->detach && !check_branch_ref(the_repository, &symref, refname) &&
	    refs_ref_exists(get_main_ref_store(the_repository), symref.buf)) {
		is_branch = 1;
		ಆದರೆ (!opts->force)
			die_if_checked_ಹೊರಹೋಗು(symref.buf, 0);
	}
	commit = lookup_commit_reference_by_name(refname);
	ಆದರೆ (!commit && !opts->orphan)
		die(_("invalid reference: %s"), refname);

	name = name_ಗಮ್ಯ_free = worktree_basename_dup(ಮಾರ್ಗ);
	ಆದರೆ (!*name)
		die(_("invalid path '%s'"), ಮಾರ್ಗ);
	sanitize_refname_component(name, &sb_name);
	ಆದರೆ (!sb_name.len)
		BUG("How come '%s' becomes empty after sanitization?", name);
	name = sb_name.buf;
	repo_git_ಮಾರ್ಗ_replace(the_repository, &sb_repo, "worktrees/%s", name);
	len = sb_repo.len;
	ಆದರೆ (ಸುರಕ್ಷಿತ_create_leading_directories_const(the_repository, sb_repo.buf))
		die_errno(_("could not create leading directories of '%s'"),
			  sb_repo.buf);

	ಆಗ (mkdir(sb_repo.buf, 0777)) {
		counter++;
		ಆದರೆ ((errno != EEXIST) || !counter /*ಅತಿಪ್ರವಾಹ*/)
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
	ಆದರೆ (opts->keep_locked)
		write_file(sb.buf, "%s", opts->keep_locked);
	ಇಲ್ಲದಿದ್ದರೆ
		write_file(sb.buf, _("initializing"));

	strbuf_addf(&sb_git, "%s/.git", ಮಾರ್ಗ);
	ಆದರೆ (ಸುರಕ್ಷಿತ_create_leading_directories_const(the_repository, sb_git.buf))
		die_errno(_("could not create leading directories of '%s'"),
			  sb_git.buf);
	junk_work_ವೃಕ್ಷ = xstrdup(ಮಾರ್ಗ);

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
	ಆದರೆ (!wt) {
		ret = error(_("could not find created worktree '%s'"), name);
		ಹೋಗು ಮುಗಿದ;
	}
	ಸಿದ್ಧತೆ_alternate_ref_dir(wt, sb_repo.buf);
	wt_refs = get_worktree_ref_store(wt);

	ret = ref_store_create_on_disk(wt_refs, REF_STORE_CREATE_ON_DISK_IS_WORKTREE, &sb);
	ಆದರೆ (ret)
		ಹೋಗು ಮುಗಿದ;

	ಆದರೆ (!is_branch && commit)
		ret = refs_update_ref(wt_refs, ಖಾಲಿ, "HEAD", &commit->object.oid,
				      ಖಾಲಿ, 0, UPDATE_REFS_ಸಂದೇಶ_ON_ERR);
	ಇಲ್ಲದಿದ್ದರೆ
		ret = refs_update_symref(wt_refs, "HEAD", symref.buf, ಖಾಲಿ);
	ಆದರೆ (ret)
		ಹೋಗು ಮುಗಿದ;

	/*
	 * If the current worktree has sparse-checkout enabled, then copy
	 * the sparse-checkout patterns from the current worktree.
	 */
	ಆದರೆ (cfg->apply_sparse_checkout)
		copy_sparse_checkout(sb_repo.buf);

	/*
	 * If we are using worktree config, then copy all current config
	 * values from the current worktree into the new one, that way the
	 * new worktree behaves the same as this one.
	 */
	ಆದರೆ (the_repository->repository_format_worktree_config)
		copy_filtered_worktree_config(sb_repo.buf);

	strvec_pushf(&ಮಗು_env, "%s=%s", GIT_DIR_ENVIRONMENT, sb_git.buf);
	strvec_pushf(&ಮಗು_env, "%s=%s", GIT_WORK_ವೃಕ್ಷ_ENVIRONMENT, ಮಾರ್ಗ);

	ಆದರೆ (opts->orphan &&
	    (ret = make_worktree_orphan(refname, opts, &ಮಗು_env)))
		ಹೋಗು ಮುಗಿದ;

	ಆದರೆ (opts->checkout &&
	    (ret = checkout_worktree(opts, &ಮಗು_env)))
		ಹೋಗು ಮುಗಿದ;

	is_junk = 0;
	FREE_AND_NULL(junk_work_ವೃಕ್ಷ);
	FREE_AND_NULL(junk_git_dir);

ಮುಗಿದ:
	ಆದರೆ (ret || !opts->keep_locked) {
		strbuf_reset(&sb);
		strbuf_addf(&sb, "%s/locked", sb_repo.buf);
		unlink_or_warn(sb.buf);
	}

	/*
	 * Hook failure does not warrant worktree deletion, so run hook after
	 * is_junk is cleared, but do return appropriate code when hook fails.
	 */
	ಆದರೆ (!ret && opts->checkout && !opts->orphan) {
		ರಚನೆ ಸತತ_hooks_opt opt = ಸತತ_HOOKS_OPT_INIT_FORCE_SERIAL;

		strvec_pushl(&opt.env, "GIT_DIR", "GIT_WORK_TREE", ಖಾಲಿ);
		strvec_pushl(&opt.ವಾದಗಳು,
			     oid_ಗಮ್ಯ_hex(null_oid(the_hash_algo)),
			     oid_ಗಮ್ಯ_hex(&commit->object.oid),
			     "1",
			     ಖಾಲಿ);
		opt.dir = ಮಾರ್ಗ;

		ret = ಸತತ_hooks_opt(the_repository, "post-checkout", &opt);
	}

	strvec_clear(&ಮಗು_env);
	strbuf_release(&sb);
	strbuf_release(&symref);
	strbuf_release(&sb_repo);
	strbuf_release(&sb_git);
	strbuf_release(&sb_name);
	free_worktree(wt);
	ಬಿಡುಗಡೆ(name_ಗಮ್ಯ_free);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಶೂನ್ಯ print_preparing_worktree_ಸಾಲು(ಪೂರ್ಣಾಂಕ detach,
					  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *branch,
					  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *new_branch,
					  ಪೂರ್ಣಾಂಕ force_new_branch)
{
	ಆದರೆ (force_new_branch) {
		ರಚನೆ commit *commit = lookup_commit_reference_by_name(new_branch);
		ಆದರೆ (!commit)
			fprintf_ln(stderr, _("Preparing worktree (new branch '%s')"), new_branch);
		ಇಲ್ಲದಿದ್ದರೆ
			fprintf_ln(stderr, _("Preparing worktree (resetting branch '%s'; was at %s)"),
				  new_branch,
				  repo_ಕಂಡುಹಿಡಿ_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV));
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (new_branch) {
		fprintf_ln(stderr, _("Preparing worktree (new branch '%s')"), new_branch);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ರಚನೆ strbuf s = STRBUF_INIT;
		ಆದರೆ (!detach && !check_branch_ref(the_repository, &s, branch) &&
		    refs_ref_exists(get_main_ref_store(the_repository), s.buf))
			fprintf_ln(stderr, _("Preparing worktree (checking out '%s')"),
				  branch);
		ಇಲ್ಲದಿದ್ದರೆ {
			ರಚನೆ commit *commit = lookup_commit_reference_by_name(branch);
			ಆದರೆ (!commit)
				BUG("unreachable: invalid reference: %s", branch);
			fprintf_ln(stderr, _("Preparing worktree (detached HEAD %s)"),
				  repo_ಕಂಡುಹಿಡಿ_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV));
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
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಮೊದಲ_valid_ref(ಸ್ಥಿರಾಂಕ ರಚನೆ reference *ref ಬಳಸದ, ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ ಬಳಸದ)
{
	ಹಿಂತಿರುಗಿ 1;
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
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ can_use_local_refs(ಸ್ಥಿರಾಂಕ ರಚನೆ ಕೂಡಿಸು_opts *opts)
{
	ಆದರೆ (refs_ತಲೆ_ref(get_main_ref_store(the_repository), ಮೊದಲ_valid_ref, ಖಾಲಿ)) {
		ಹಿಂತಿರುಗಿ 1;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (refs_for_each_branch_ref(get_main_ref_store(the_repository), ಮೊದಲ_valid_ref, ಖಾಲಿ)) {
		ಆದರೆ (!opts->quiet)
			warning(_("HEAD points to an invalid (or orphaned) reference.\n"));
		ಹಿಂತಿರುಗಿ 1;
	}
	ಹಿಂತಿರುಗಿ 0;
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
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ can_use_remote_refs(ಸ್ಥಿರಾಂಕ ರಚನೆ ಕೂಡಿಸು_opts *opts)
{
	ಆದರೆ (!guess_remote) {
		ಹಿಂತಿರುಗಿ 0;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (refs_for_each_remote_ref(get_main_ref_store(the_repository), ಮೊದಲ_valid_ref, ಖಾಲಿ)) {
		ಹಿಂತಿರುಗಿ 1;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!opts->force && remote_get(ಖಾಲಿ)) {
		die(_("No local or remote refs exist despite at least one remote\n"
		      "present, stopping; use 'add -f' to override or fetch a remote first"));
	}
	ಹಿಂತಿರುಗಿ 0;
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
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ dwim_orphan(ಸ್ಥಿರಾಂಕ ರಚನೆ ಕೂಡಿಸು_opts *opts, ಪೂರ್ಣಾಂಕ opt_track, ಪೂರ್ಣಾಂಕ remote)
{
	ಆದರೆ (can_use_local_refs(opts)) {
		ಹಿಂತಿರುಗಿ 0;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (remote && can_use_remote_refs(opts)) {
		ಹಿಂತಿರುಗಿ 0;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!opts->quiet) {
		fprintf_ln(stderr, WORKTREE_ಕೂಡಿಸು_DWIM_ORPHAN_INFER_ಪಠ್ಯ);
	}

	ಆದರೆ (opt_track) {
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--track");
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!opts->checkout) {
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--no-checkout");
	}
	ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ ಶೂನ್ಯ advise_disambiguating_remotes(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *branch,
					  ಸ್ಥಿರಾಂಕ ರಚನೆ string_list *matched_remote_ಹೆಸರುಗಳು)
{
	ರಚನೆ string_list_ಅಂಶ *ಅಂಶ;

	advise(_("Branch name '%s' appears in multiple remotes:"), branch);
	for_each_string_list_ಅಂಶ(ಅಂಶ, matched_remote_ಹೆಸರುಗಳು) {
		advise(_("  %s"), ಅಂಶ->string);
	}
	advise(_("If you meant to create a worktree from a remote tracking branch on\n"
		 "<remote>, you can do so by:\n"
		 "\n"
		 "    git worktree add -b %s %s <remote>/%s\n"
		 "\n"
		 "If you'd like to always prefer some remote, e.g. 'origin',\n"
		 "consider setting checkout.defaultRemote=origin in your config."),
	       branch, ಮಾರ್ಗ, branch);
}

ಸ್ಥಿರ ಅಕ್ಷರ *dwim_branch(ಸ್ಥಿರಾಂಕ ರಚನೆ ಕೂಡಿಸು_opts *opts, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಅಕ್ಷರ **new_branch)
{
	ಪೂರ್ಣಾಂಕ branch_exists;
	ಅಕ್ಷರ *branchname = worktree_basename_dup(ಮಾರ್ಗ);
	ರಚನೆ strbuf ref = STRBUF_INIT;

	branch_exists = !check_branch_ref(the_repository, &ref, branchname) &&
			refs_ref_exists(get_main_ref_store(the_repository),
					ref.buf);
	strbuf_release(&ref);
	ಆದರೆ (branch_exists)
		ಹಿಂತಿರುಗಿ branchname;

	*new_branch = branchname;
	ಆದರೆ (guess_remote) {
		ರಚನೆ object_id oid;
		ಅಕ್ಷರ *remote;
		ಪೂರ್ಣಾಂಕ ಅಂಕೆ_matches = 0;
		ರಚನೆ string_list matched_remote_ಹೆಸರುಗಳು = STRING_LIST_INIT_DUP;

		remote = unique_tracking_name(*new_branch, &oid, &ಅಂಕೆ_matches,
					      &matched_remote_ಹೆಸರುಗಳು);
		ಆದರೆ (!remote && ಅಂಕೆ_matches > 1) {
			ಆದರೆ (!opts->quiet &&
			    advice_enabled(ADVICE_CHECKOUT_AMBIGUOUS_REMOTE_BRANCH_NAME))
				advise_disambiguating_remotes(ಮಾರ್ಗ, *new_branch,
							      &matched_remote_ಹೆಸರುಗಳು);
			die(_("'%s' matched multiple (%d) remote tracking branches"),
			    *new_branch, ಅಂಕೆ_matches);
		}
		string_list_clear(&matched_remote_ಹೆಸರುಗಳು, 0);
		ಹಿಂತಿರುಗಿ remote;
	}
	ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಕೂಡಿಸು(ಪೂರ್ಣಾಂಕ ac, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **av, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
	       ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ ಕೂಡಿಸು_opts opts;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *new_branch_force = ಖಾಲಿ;
	ಅಕ್ಷರ *ಮಾರ್ಗ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *branch;
	ಅಕ್ಷರ *branch_ಗಮ್ಯ_free = ಖಾಲಿ;
	ಅಕ್ಷರ *new_branch_ಗಮ್ಯ_free = ಖಾಲಿ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *new_branch = ಖಾಲಿ;
	ಅಕ್ಷರ *opt_track = ಖಾಲಿ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಬೀಗ_reason = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ keep_locked = 0;
	ಪೂರ್ಣಾಂಕ ಬಳಸಿದ_new_branch_options;
	ರಚನೆ option options[] = {
		OPT__FORCE(&opts.force,
			   N_("checkout <branch> even if already checked out in other worktree"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_STRING('b', ಖಾಲಿ, &new_branch, N_("branch"),
			   N_("create a new branch")),
		OPT_STRING('B', ಖಾಲಿ, &new_branch_force, N_("branch"),
			   N_("create or reset a branch")),
		OPT_BOOL(0, "orphan", &opts.orphan, N_("create unborn branch")),
		OPT_BOOL('d', "detach", &opts.detach, N_("detach HEAD at named commit")),
		OPT_BOOL(0, "checkout", &opts.checkout, N_("populate the new working tree")),
		OPT_BOOL(0, "lock", &keep_locked, N_("keep the new working tree locked")),
		OPT_STRING(0, "reason", &ಬೀಗ_reason, N_("string"),
			   N_("reason for locking")),
		OPT__QUIET(&opts.quiet, N_("suppress progress reporting")),
		OPT_PASSTHRU(0, "track", &opt_track, ಖಾಲಿ,
			     N_("set up tracking mode (see git-branch(1))"),
			     PARSE_OPT_NOARG | PARSE_OPT_OPTARG),
		OPT_BOOL(0, "guess-remote", &guess_remote,
			 N_("try to match the new branch name with a remote-tracking branch")),
		OPT_BOOL(0, "relative-paths", &opts.relative_paths,
			 N_("use relative paths for worktrees")),
		OPT_ಕೊನೆ()
	};
	ಪೂರ್ಣಾಂಕ ret;

	memset(&opts, 0, ಗಾತ್ರ(opts));
	opts.checkout = 1;
	opts.relative_paths = use_relative_paths;
	ac = parse_options(ac, av, ಪೂರ್ವಪ್ರತ್ಯಯ, options, git_worktree_ಕೂಡಿಸು_usage, 0);
	ಆದರೆ (!!opts.detach + !!new_branch + !!new_branch_force > 1)
		die(_("options '%s', '%s', and '%s' cannot be used together"), "-b", "-B", "--detach");
	ಆದರೆ (opts.detach && opts.orphan)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--detach");
	ಆದರೆ (opts.orphan && opt_track)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--track");
	ಆದರೆ (opts.orphan && !opts.checkout)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--no-checkout");
	ಆದರೆ (opts.orphan && ac == 2)
		die(_("option '%s' and commit-ish cannot be used together"),
		    "--orphan");
	ಆದರೆ (ಬೀಗ_reason && !keep_locked)
		die(_("the option '%s' requires '%s'"), "--reason", "--lock");
	ಆದರೆ (ಬೀಗ_reason)
		opts.keep_locked = ಬೀಗ_reason;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (keep_locked)
		opts.keep_locked = _("added with --lock");

	ಆದರೆ (ac < 1 || ac > 2)
		usage_with_options(git_worktree_ಕೂಡಿಸು_usage, options);

	ಮಾರ್ಗ = ಪೂರ್ವಪ್ರತ್ಯಯ_filename(ಪೂರ್ವಪ್ರತ್ಯಯ, av[0]);
	branch = ac < 2 ? "HEAD" : av[1];
	ಬಳಸಿದ_new_branch_options = new_branch || new_branch_force;

	ಆದರೆ (!strcmp(branch, "-"))
		branch = "@{-1}";

	ಆದರೆ (new_branch_force) {
		ರಚನೆ strbuf symref = STRBUF_INIT;

		new_branch = new_branch_force;

		ಆದರೆ (!opts.force &&
		    !check_branch_ref(the_repository, &symref, new_branch) &&
		    refs_ref_exists(get_main_ref_store(the_repository), symref.buf))
			die_if_checked_ಹೊರಹೋಗು(symref.buf, 0);
		strbuf_release(&symref);
	}

	ಆದರೆ (opts.orphan && !new_branch) {
		new_branch = new_branch_ಗಮ್ಯ_free = worktree_basename_dup(ಮಾರ್ಗ);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (opts.orphan) {
		; /* no-op */
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (opts.detach) {
		/* Check HEAD */
		ಆದರೆ (!strcmp(branch, "HEAD"))
			can_use_local_refs(&opts);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ac < 2 && new_branch) {
		/* DWIM: Infer --orphan when repo has no refs. */
		opts.orphan = dwim_orphan(&opts, !!opt_track, 0);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ac < 2) {
		/* DWIM: Guess branch name from path. */
		ಅಕ್ಷರ *s = dwim_branch(&opts, ಮಾರ್ಗ, &new_branch_ಗಮ್ಯ_free);
		ಆದರೆ (s)
			branch = branch_ಗಮ್ಯ_free = s;
		new_branch = new_branch_ಗಮ್ಯ_free;

		/* DWIM: Infer --orphan when repo has no refs. */
		opts.orphan = (!s) && dwim_orphan(&opts, !!opt_track, 1);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ac == 2 && new_branch) {
		ಆದರೆ (!strcmp(branch, "HEAD"))
			can_use_local_refs(&opts);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ac == 2) {
		ರಚನೆ commit *commit;

		commit = lookup_commit_reference_by_name(branch);
		ಆದರೆ (!commit) {
			ರಚನೆ object_id oid;
			ಅಕ್ಷರ *remote;
			ಪೂರ್ಣಾಂಕ ಅಂಕೆ_matches = 0;
			ರಚನೆ string_list matched_remote_ಹೆಸರುಗಳು = STRING_LIST_INIT_DUP;

			remote = unique_tracking_name(branch, &oid, &ಅಂಕೆ_matches,
						      &matched_remote_ಹೆಸರುಗಳು);
			ಆದರೆ (remote) {
				new_branch = branch;
				branch = new_branch_ಗಮ್ಯ_free = remote;
			} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಅಂಕೆ_matches > 1) {
				ಆದರೆ (!opts.quiet &&
				    advice_enabled(ADVICE_CHECKOUT_AMBIGUOUS_REMOTE_BRANCH_NAME))
					advise_disambiguating_remotes(ಮಾರ್ಗ, branch,
								      &matched_remote_ಹೆಸರುಗಳು);
				die(_("'%s' matched multiple (%d) remote tracking branches"),
				    branch, ಅಂಕೆ_matches);
			}
			string_list_clear(&matched_remote_ಹೆಸರುಗಳು, 0);
		}

		ಆದರೆ (!strcmp(branch, "HEAD"))
			can_use_local_refs(&opts);

	}

	ಆದರೆ (!opts.orphan && !lookup_commit_reference_by_name(branch)) {
		ಪೂರ್ಣಾಂಕ attempt_hint = !opts.quiet && (ac < 2);
		ಆದರೆ (attempt_hint && ಬಳಸಿದ_new_branch_options) {
			advise_if_enabled(ADVICE_WORKTREE_ಕೂಡಿಸು_ORPHAN,
				WORKTREE_ಕೂಡಿಸು_ORPHAN_WITH_DASH_B_HINT_ಪಠ್ಯ,
				new_branch, ಮಾರ್ಗ);
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (attempt_hint) {
			advise_if_enabled(ADVICE_WORKTREE_ಕೂಡಿಸು_ORPHAN,
				WORKTREE_ಕೂಡಿಸು_ORPHAN_NO_DASH_B_HINT_ಪಠ್ಯ, ಮಾರ್ಗ);
		}
		die(_("invalid reference: %s"), branch);
	}

	ಆದರೆ (!opts.quiet)
		print_preparing_worktree_ಸಾಲು(opts.detach, branch, new_branch, !!new_branch_force);

	ಆದರೆ (opts.orphan) {
		branch = new_branch;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (new_branch) {
		ರಚನೆ ಮಗು_process cp = ಮಗು_PROCESS_INIT;
		cp.git_cmd = 1;
		strvec_ತಳ್ಳು(&cp.ವಾದಗಳು, "branch");
		ಆದರೆ (new_branch_force)
			strvec_ತಳ್ಳು(&cp.ವಾದಗಳು, "--force");
		ಆದರೆ (opts.quiet)
			strvec_ತಳ್ಳು(&cp.ವಾದಗಳು, "--quiet");
		strvec_ತಳ್ಳು(&cp.ವಾದಗಳು, new_branch);
		strvec_ತಳ್ಳು(&cp.ವಾದಗಳು, branch);
		ಆದರೆ (opt_track)
			strvec_ತಳ್ಳು(&cp.ವಾದಗಳು, opt_track);
		ಆದರೆ (ಸತತ_command(&cp)) {
			ret = -1;
			ಹೋಗು cleanup;
		}
		branch = new_branch;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (opt_track) {
		die(_("--[no-]track can only be used if a new branch is created"));
	}

	ret = ಕೂಡಿಸು_worktree(ಮಾರ್ಗ, branch, &opts);
cleanup:
	ಬಿಡುಗಡೆ(ಮಾರ್ಗ);
	ಬಿಡುಗಡೆ(opt_track);
	ಬಿಡುಗಡೆ(branch_ಗಮ್ಯ_free);
	ಬಿಡುಗಡೆ(new_branch_ಗಮ್ಯ_free);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಶೂನ್ಯ show_worktree_porcelain(ರಚನೆ worktree *wt, ಪೂರ್ಣಾಂಕ ಸಾಲು_terminator)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *reason;

	ಬರೆ("worktree %s%c", wt->ಮಾರ್ಗ, ಸಾಲು_terminator);
	ಆದರೆ (wt->is_bare)
		ಬರೆ("bare%c", ಸಾಲು_terminator);
	ಇಲ್ಲದಿದ್ದರೆ {
		ಬರೆ("HEAD %s%c", oid_ಗಮ್ಯ_hex(&wt->ತಲೆ_oid), ಸಾಲು_terminator);
		ಆದರೆ (wt->is_detached)
			ಬರೆ("detached%c", ಸಾಲು_terminator);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (wt->ತಲೆ_ref)
			ಬರೆ("branch %s%c", wt->ತಲೆ_ref, ಸಾಲು_terminator);
	}

	reason = worktree_ಬೀಗ_reason(wt);
	ಆದರೆ (reason) {
		fputs("locked", stdout);
		ಆದರೆ (*reason) {
			fputc(' ', stdout);
			write_name_quoted(reason, stdout, ಸಾಲು_terminator);
		} ಇಲ್ಲದಿದ್ದರೆ {
			fputc(ಸಾಲು_terminator, stdout);
		}
	}

	reason = worktree_prune_reason(wt, expire);
	ಆದರೆ (reason)
		ಬರೆ("prunable %s%c", reason, ಸಾಲು_terminator);

	fputc(ಸಾಲು_terminator, stdout);
}

ರಚನೆ worktree_display {
	ಅಕ್ಷರ *ಮಾರ್ಗ;
	ಪೂರ್ಣಾಂಕ ಅಗಲ;
};

ಸ್ಥಿರ ಶೂನ್ಯ show_worktree(ರಚನೆ worktree *wt, ರಚನೆ worktree_display *display,
			  ಪೂರ್ಣಾಂಕ ಮಾರ್ಗ_maxwidth, ಪೂರ್ಣಾಂಕ abbrev_len)
{
	ರಚನೆ strbuf sb = STRBUF_INIT;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *reason;

	strbuf_addf(&sb, "%s%*s", display->ಮಾರ್ಗ, 1 + ಮಾರ್ಗ_maxwidth - display->ಅಗಲ, "");
	ಆದರೆ (wt->is_bare)
		strbuf_addstr(&sb, "(bare)");
	ಇಲ್ಲದಿದ್ದರೆ {
		strbuf_addf(&sb, "%-*s ", abbrev_len,
				repo_ಕಂಡುಹಿಡಿ_unique_abbrev(the_repository, &wt->ತಲೆ_oid, DEFAULT_ABBREV));
		ಆದರೆ (wt->is_detached)
			strbuf_addstr(&sb, "(detached HEAD)");
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (wt->ತಲೆ_ref) {
			ಅಕ್ಷರ *ref = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
								 wt->ತಲೆ_ref,
								 0);
			strbuf_addf(&sb, "[%s]", ref);
			ಬಿಡುಗಡೆ(ref);
		} ಇಲ್ಲದಿದ್ದರೆ
			strbuf_addstr(&sb, "(error)");
	}

	reason = worktree_ಬೀಗ_reason(wt);
	ಆದರೆ (verbose && reason && *reason)
		strbuf_addf(&sb, "\n\tlocked: %s", reason);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (reason)
		strbuf_addstr(&sb, " locked");

	reason = worktree_prune_reason(wt, expire);
	ಆದರೆ (verbose && reason)
		strbuf_addf(&sb, "\n\tprunable: %s", reason);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (reason)
		strbuf_addstr(&sb, " prunable");

	ಬರೆ("%s\n", sb.buf);
	strbuf_release(&sb);
}

ಸ್ಥಿರ ಶೂನ್ಯ measure_widths(ರಚನೆ worktree **wt, ಪೂರ್ಣಾಂಕ *abbrev,
			   ರಚನೆ worktree_display **d, ಪೂರ್ಣಾಂಕ *maxwidth)
{
	ಪೂರ್ಣಾಂಕ i, display_alloc = 0;
	ರಚನೆ worktree_display *display = ಖಾಲಿ;
	ರಚನೆ strbuf buf = STRBUF_INIT;

	ಗಾಗಿ (i = 0; wt[i]; i++) {
		ಪೂರ್ಣಾಂಕ sha1_len;
		ALLOC_GROW(display, i + 1, display_alloc);
		quote_ಮಾರ್ಗ(wt[i]->ಮಾರ್ಗ, ಖಾಲಿ, &buf, 0);
		display[i].ಅಗಲ = utf8_strwidth(buf.buf);
		display[i].ಮಾರ್ಗ = strbuf_detach(&buf, ಖಾಲಿ);

		ಆದರೆ (display[i].ಅಗಲ > *maxwidth)
			*maxwidth = display[i].ಅಗಲ;
		sha1_len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(repo_ಕಂಡುಹಿಡಿ_unique_abbrev(the_repository, &wt[i]->ತಲೆ_oid, *abbrev));
		ಆದರೆ (sha1_len > *abbrev)
			*abbrev = sha1_len;
	}
	*d = display;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ pathcmp(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *a_, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *b_)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ worktree *ಸ್ಥಿರಾಂಕ *a = a_;
	ಸ್ಥಿರಾಂಕ ರಚನೆ worktree *ಸ್ಥಿರಾಂಕ *b = b_;
	ಹಿಂತಿರುಗಿ fspathcmp((*a)->ಮಾರ್ಗ, (*b)->ಮಾರ್ಗ);
}

ಸ್ಥಿರ ಶೂನ್ಯ pathsort(ರಚನೆ worktree **wt)
{
	ಪೂರ್ಣಾಂಕ n = 0;
	ರಚನೆ worktree **p = wt;

	ಆಗ (*p++)
		n++;
	QSORT(wt, n, pathcmp);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ list(ಪೂರ್ಣಾಂಕ ac, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **av, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		ರಚನೆ repository *repo ಬಳಸದ)
{
	ಪೂರ್ಣಾಂಕ porcelain = 0;
	ಪೂರ್ಣಾಂಕ ಸಾಲು_terminator = '\n';

	ರಚನೆ option options[] = {
		OPT_BOOL(0, "porcelain", &porcelain, N_("machine-readable output")),
		OPT__VERBOSE(&verbose, N_("show extended annotations and reasons, if available")),
		OPT_EXPIRY_DATE(0, "expire", &expire,
				N_("add 'prunable' annotation to missing worktrees older than <time>")),
		OPT_ಗಣ_INT('z', ಖಾಲಿ, &ಸಾಲು_terminator,
			    N_("terminate records with a NUL character"), '\0'),
		OPT_ಕೊನೆ()
	};

	expire = ಸಮಯ_MAX;
	ac = parse_options(ac, av, ಪೂರ್ವಪ್ರತ್ಯಯ, options, git_worktree_list_usage, 0);
	ಆದರೆ (ac)
		usage_with_options(git_worktree_list_usage, options);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (verbose && porcelain)
		die(_("options '%s' and '%s' cannot be used together"), "--verbose", "--porcelain");
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!ಸಾಲು_terminator && !porcelain)
		die(_("the option '%s' requires '%s'"), "-z", "--porcelain");
	ಇಲ್ಲದಿದ್ದರೆ {
		ರಚನೆ worktree **worktrees = get_worktrees(the_repository);
		ಪೂರ್ಣಾಂಕ ಮಾರ್ಗ_maxwidth = 0, abbrev = DEFAULT_ABBREV, i;
		ರಚನೆ worktree_display *display = ಖಾಲಿ;

		/* sort worktrees by path but keep main worktree at top */
		pathsort(worktrees + 1);

		ಆದರೆ (!porcelain)
			measure_widths(worktrees, &abbrev,
				       &display, &ಮಾರ್ಗ_maxwidth);

		ಗಾಗಿ (i = 0; worktrees[i]; i++) {
			ಆದರೆ (porcelain)
				show_worktree_porcelain(worktrees[i],
							ಸಾಲು_terminator);
			ಇಲ್ಲದಿದ್ದರೆ
				show_worktree(worktrees[i],
					      &display[i], ಮಾರ್ಗ_maxwidth, abbrev);
		}
		ಗಾಗಿ (i = 0; display && worktrees[i]; i++)
			ಬಿಡುಗಡೆ(display[i].ಮಾರ್ಗ);
		ಬಿಡುಗಡೆ(display);
		free_worktrees(worktrees);
	}
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಬೀಗ_worktree(ಪೂರ್ಣಾಂಕ ac, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **av, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			 ರಚನೆ repository *repo ಬಳಸದ)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *reason = "", *old_reason;
	ರಚನೆ option options[] = {
		OPT_STRING(0, "reason", &reason, N_("string"),
			   N_("reason for locking")),
		OPT_ಕೊನೆ()
	};
	ರಚನೆ worktree **worktrees, *wt;
	ಅಕ್ಷರ *ಮಾರ್ಗ;

	ac = parse_options(ac, av, ಪೂರ್ವಪ್ರತ್ಯಯ, options, git_worktree_ಬೀಗ_usage, 0);
	ಆದರೆ (ac != 1)
		usage_with_options(git_worktree_ಬೀಗ_usage, options);

	worktrees = get_worktrees(the_repository);
	wt = ಕಂಡುಹಿಡಿ_worktree(worktrees, ಪೂರ್ವಪ್ರತ್ಯಯ, av[0]);
	ಆದರೆ (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	ಆದರೆ (is_main_worktree(wt))
		die(_("The main working tree cannot be locked or unlocked"));

	old_reason = worktree_ಬೀಗ_reason(wt);
	ಆದರೆ (old_reason) {
		ಆದರೆ (*old_reason)
			die(_("'%s' is already locked, reason: %s"),
			    av[0], old_reason);
		die(_("'%s' is already locked"), av[0]);
	}

	ಮಾರ್ಗ = repo_common_ಮಾರ್ಗ(the_repository, "worktrees/%s/locked", wt->id);
	write_file(ಮಾರ್ಗ, "%s", reason);

	free_worktrees(worktrees);
	ಬಿಡುಗಡೆ(ಮಾರ್ಗ);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ unlock_worktree(ಪೂರ್ಣಾಂಕ ac, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **av, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			   ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ option options[] = {
		OPT_ಕೊನೆ()
	};
	ರಚನೆ worktree **worktrees, *wt;
	ಅಕ್ಷರ *ಮಾರ್ಗ;
	ಪೂರ್ಣಾಂಕ ret;

	ac = parse_options(ac, av, ಪೂರ್ವಪ್ರತ್ಯಯ, options, git_worktree_unlock_usage, 0);
	ಆದರೆ (ac != 1)
		usage_with_options(git_worktree_unlock_usage, options);

	worktrees = get_worktrees(the_repository);
	wt = ಕಂಡುಹಿಡಿ_worktree(worktrees, ಪೂರ್ವಪ್ರತ್ಯಯ, av[0]);
	ಆದರೆ (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	ಆದರೆ (is_main_worktree(wt))
		die(_("The main working tree cannot be locked or unlocked"));
	ಆದರೆ (!worktree_ಬೀಗ_reason(wt))
		die(_("'%s' is not locked"), av[0]);

	ಮಾರ್ಗ = repo_common_ಮಾರ್ಗ(the_repository, "worktrees/%s/locked", wt->id);
	ret = unlink_or_warn(ಮಾರ್ಗ);

	free_worktrees(worktrees);
	ಬಿಡುಗಡೆ(ಮಾರ್ಗ);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಶೂನ್ಯ validate_no_submodules(ಸ್ಥಿರಾಂಕ ರಚನೆ worktree *wt)
{
	ರಚನೆ ಸೂಚ್ಯಂಕ_state istate = ಸೂಚ್ಯಂಕ_STATE_INIT(the_repository);
	ರಚನೆ strbuf ಮಾರ್ಗ = STRBUF_INIT;
	ಪೂರ್ಣಾಂಕ i, ಸಿಕ್ಕಿತು_submodules = 0;
	ಅಕ್ಷರ *wt_gitdir;

	wt_gitdir = get_worktree_git_dir(wt);

	ಆದರೆ (is_directory(worktree_git_ಮಾರ್ಗ(wt, "modules"))) {
		/*
		 * There could be false positives, e.g. the "modules"
		 * directory exists but is empty. But it's a rare case and
		 * this simpler check is probably good enough for now.
		 */
		ಸಿಕ್ಕಿತು_submodules = 1;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (read_ಸೂಚ್ಯಂಕ_from(&istate, worktree_git_ಮಾರ್ಗ(wt, "index"),
				   wt_gitdir) > 0) {
		ಗಾಗಿ (i = 0; i < istate.cache_nr; i++) {
			ರಚನೆ cache_ನಮೂದು *ce = istate.cache[i];
			ಪೂರ್ಣಾಂಕ err;

			ಆದರೆ (!S_ISGITLINK(ce->ce_ಬಹುಲಕ))
				ಮುಂದುವರಿ;

			strbuf_reset(&ಮಾರ್ಗ);
			strbuf_addf(&ಮಾರ್ಗ, "%s/%s", wt->ಮಾರ್ಗ, ce->name);
			ಆದರೆ (!is_submodule_populated_gently(ಮಾರ್ಗ.buf, &err))
				ಮುಂದುವರಿ;

			ಸಿಕ್ಕಿತು_submodules = 1;
			ನಿಲ್ಲಿಸು;
		}
	}
	discard_ಸೂಚ್ಯಂಕ(&istate);
	strbuf_release(&ಮಾರ್ಗ);
	ಬಿಡುಗಡೆ(wt_gitdir);

	ಆದರೆ (ಸಿಕ್ಕಿತು_submodules)
		die(_("working trees containing submodules cannot be moved or removed"));
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ move_worktree(ಪೂರ್ಣಾಂಕ ac, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **av, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			 ರಚನೆ repository *repo ಬಳಸದ)
{
	ಪೂರ್ಣಾಂಕ force = 0;
	ರಚನೆ option options[] = {
		OPT__FORCE(&force,
			 N_("force move even if worktree is dirty or locked"),
			 PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "relative-paths", &use_relative_paths,
			 N_("use relative paths for worktrees")),
		OPT_ಕೊನೆ()
	};
	ರಚನೆ worktree **worktrees, *wt;
	ರಚನೆ strbuf dst = STRBUF_INIT;
	ರಚನೆ strbuf errmsg = STRBUF_INIT;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *reason = ಖಾಲಿ;
	ಅಕ್ಷರ *ಮಾರ್ಗ;

	ac = parse_options(ac, av, ಪೂರ್ವಪ್ರತ್ಯಯ, options, git_worktree_move_usage,
			   0);
	ಆದರೆ (ac != 2)
		usage_with_options(git_worktree_move_usage, options);

	ಮಾರ್ಗ = ಪೂರ್ವಪ್ರತ್ಯಯ_filename(ಪೂರ್ವಪ್ರತ್ಯಯ, av[1]);
	strbuf_addstr(&dst, ಮಾರ್ಗ);
	ಬಿಡುಗಡೆ(ಮಾರ್ಗ);

	worktrees = get_worktrees(the_repository);
	wt = ಕಂಡುಹಿಡಿ_worktree(worktrees, ಪೂರ್ವಪ್ರತ್ಯಯ, av[0]);
	ಆದರೆ (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	ಆದರೆ (is_main_worktree(wt))
		die(_("'%s' is a main working tree"), av[0]);
	ಆದರೆ (is_directory(dst.buf)) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *sep = ಕಂಡುಹಿಡಿ_ಕೊನೆಯ_dir_sep(wt->ಮಾರ್ಗ);

		ಆದರೆ (!sep)
			die(_("could not figure out destination name from '%s'"),
			    wt->ಮಾರ್ಗ);
		strbuf_trim_trailing_dir_sep(&dst);
		strbuf_addstr(&dst, sep);
	}
	check_candidate_ಮಾರ್ಗ(dst.buf, force, worktrees, "move");

	validate_no_submodules(wt);

	ಆದರೆ (force < 2)
		reason = worktree_ಬೀಗ_reason(wt);
	ಆದರೆ (reason) {
		ಆದರೆ (*reason)
			die(_("cannot move a locked working tree, lock reason: %s\nuse 'move -f -f' to override or unlock first"),
			    reason);
		die(_("cannot move a locked working tree;\nuse 'move -f -f' to override or unlock first"));
	}
	ಆದರೆ (validate_worktree(wt, &errmsg, 0))
		die(_("validation failed, cannot move working tree: %s"),
		    errmsg.buf);
	strbuf_release(&errmsg);

	ಆದರೆ (rename(wt->ಮಾರ್ಗ, dst.buf) == -1)
		die_errno(_("failed to move '%s' to '%s'"), wt->ಮಾರ್ಗ, dst.buf);

	update_worktree_location(wt, dst.buf, use_relative_paths);

	strbuf_release(&dst);
	free_worktrees(worktrees);
	ಹಿಂತಿರುಗಿ 0;
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
ಸ್ಥಿರ ಶೂನ್ಯ check_clean_worktree(ರಚನೆ worktree *wt,
				 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *original_ಮಾರ್ಗ)
{
	ರಚನೆ ಮಗು_process cp;
	ಅಕ್ಷರ buf[1];
	ಪೂರ್ಣಾಂಕ ret;

	/*
	 * Until we sort this out, all submodules are "dirty" and
	 * will abort this function.
	 */
	validate_no_submodules(wt);

	ಮಗು_process_init(&cp);
	strvec_pushf(&cp.env, "%s=%s/.git",
		     GIT_DIR_ENVIRONMENT, wt->ಮಾರ್ಗ);
	strvec_pushf(&cp.env, "%s=%s",
		     GIT_WORK_ವೃಕ್ಷ_ENVIRONMENT, wt->ಮಾರ್ಗ);
	strvec_pushl(&cp.ವಾದಗಳು, "status",
		     "--porcelain", "--ignore-submodules=none",
		     ಖಾಲಿ);
	cp.git_cmd = 1;
	cp.dir = wt->ಮಾರ್ಗ;
	cp.ಹೊರಹೋಗು = -1;
	ret = ಆರಂಭ_command(&cp);
	ಆದರೆ (ret)
		die_errno(_("failed to run 'git status' on '%s'"),
			  original_ಮಾರ್ಗ);
	ret = xread(cp.ಹೊರಹೋಗು, buf, ಗಾತ್ರ(buf));
	ಆದರೆ (ret)
		die(_("'%s' contains modified or untracked files, use --force to delete it"),
		    original_ಮಾರ್ಗ);
	close(cp.ಹೊರಹೋಗು);
	ret = ಮುಕ್ತಾಯ_command(&cp);
	ಆದರೆ (ret)
		die_errno(_("failed to run 'git status' on '%s', code %d"),
			  original_ಮಾರ್ಗ, ret);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ delete_git_work_ವೃಕ್ಷ(ರಚನೆ worktree *wt)
{
	ರಚನೆ strbuf sb = STRBUF_INIT;
	ಪೂರ್ಣಾಂಕ ret = 0;

	strbuf_addstr(&sb, wt->ಮಾರ್ಗ);
	ಆದರೆ (remove_dir_recursively(&sb, 0)) {
		error_errno(_("failed to delete '%s'"), sb.buf);
		ret = -1;
	}
	strbuf_release(&sb);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ remove_worktree(ಪೂರ್ಣಾಂಕ ac, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **av, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			   ರಚನೆ repository *repo ಬಳಸದ)
{
	ಪೂರ್ಣಾಂಕ force = 0;
	ರಚನೆ option options[] = {
		OPT__FORCE(&force,
			 N_("force removal even if worktree is dirty or locked"),
			 PARSE_OPT_NOCOMPLETE),
		OPT_ಕೊನೆ()
	};
	ರಚನೆ worktree **worktrees, *wt;
	ರಚನೆ strbuf errmsg = STRBUF_INIT;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *reason = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ ret = 0;

	ac = parse_options(ac, av, ಪೂರ್ವಪ್ರತ್ಯಯ, options, git_worktree_remove_usage, 0);
	ಆದರೆ (ac != 1)
		usage_with_options(git_worktree_remove_usage, options);

	worktrees = get_worktrees(the_repository);
	wt = ಕಂಡುಹಿಡಿ_worktree(worktrees, ಪೂರ್ವಪ್ರತ್ಯಯ, av[0]);
	ಆದರೆ (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	ಆದರೆ (is_main_worktree(wt))
		die(_("'%s' is a main working tree"), av[0]);
	ಆದರೆ (force < 2)
		reason = worktree_ಬೀಗ_reason(wt);
	ಆದರೆ (reason) {
		ಆದರೆ (*reason)
			die(_("cannot remove a locked working tree, lock reason: %s\nuse 'remove -f -f' to override or unlock first"),
			    reason);
		die(_("cannot remove a locked working tree;\nuse 'remove -f -f' to override or unlock first"));
	}
	ಆದರೆ (validate_worktree(wt, &errmsg, WT_VALIDATE_WORKTREE_MISSING_OK))
		die(_("validation failed, cannot remove working tree: %s"),
		    errmsg.buf);
	strbuf_release(&errmsg);

	ಆದರೆ (file_exists(wt->ಮಾರ್ಗ)) {
		ಆದರೆ (!force)
			check_clean_worktree(wt, av[0]);

		ret |= delete_git_work_ವೃಕ್ಷ(wt);
	}
	/*
	 * continue on even if ret is non-zero, there's no going back
	 * from here.
	 */
	ret |= delete_git_dir(wt->id);
	delete_worktrees_dir_if_ಖಾಲಿಗಳು();

	free_worktrees(worktrees);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಶೂನ್ಯ report_repair(ಪೂರ್ಣಾಂಕ iserr, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ, ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ)
{
	ಆದರೆ (!iserr) {
		fprintf_ln(stderr, _("repair: %s: %s"), ಸಂದೇಶ, ಮಾರ್ಗ);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಪೂರ್ಣಾಂಕ *exit_ಸ್ಥಿತಿ = (ಪೂರ್ಣಾಂಕ *)cb_ದತ್ತಾಂಶ;
		fprintf_ln(stderr, _("error: %s: %s"), ಸಂದೇಶ, ಮಾರ್ಗ);
		*exit_ಸ್ಥಿತಿ = 1;
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ repair(ಪೂರ್ಣಾಂಕ ac, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **av, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		  ರಚನೆ repository *repo ಬಳಸದ)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **p;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *self[] = { ".", ಖಾಲಿ };
	ರಚನೆ option options[] = {
		OPT_BOOL(0, "relative-paths", &use_relative_paths,
			 N_("use relative paths for worktrees")),
		OPT_ಕೊನೆ()
	};
	ಪೂರ್ಣಾಂಕ rc = 0;

	ac = parse_options(ac, av, ಪೂರ್ವಪ್ರತ್ಯಯ, options, git_worktree_repair_usage, 0);
	p = ac > 0 ? av : self;
	ಗಾಗಿ (; *p; p++)
		repair_worktree_at_ಮಾರ್ಗ(the_repository, *p, report_repair,
					&rc, use_relative_paths);
	repair_worktrees(the_repository, report_repair, &rc, use_relative_paths);
	ಹಿಂತಿರುಗಿ rc;
}

ಪೂರ್ಣಾಂಕ cmd_worktree(ಪೂರ್ಣಾಂಕ ac,
		 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **av,
		 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		 ರಚನೆ repository *repo)
{
	parse_opt_subcommand_fn *fn = ಖಾಲಿ;
	ರಚನೆ option options[] = {
		OPT_SUBCOMMAND("add", &fn, ಕೂಡಿಸು),
		OPT_SUBCOMMAND("prune", &fn, prune),
		OPT_SUBCOMMAND("list", &fn, list),
		OPT_SUBCOMMAND("lock", &fn, ಬೀಗ_worktree),
		OPT_SUBCOMMAND("unlock", &fn, unlock_worktree),
		OPT_SUBCOMMAND("move", &fn, move_worktree),
		OPT_SUBCOMMAND("remove", &fn, remove_worktree),
		OPT_SUBCOMMAND("repair", &fn, repair),
		OPT_ಕೊನೆ()
	};

	repo_config(the_repository, git_worktree_config, ಖಾಲಿ);

	ಆದರೆ (!ಪೂರ್ವಪ್ರತ್ಯಯ)
		ಪೂರ್ವಪ್ರತ್ಯಯ = "";

	ac = parse_options(ac, av, ಪೂರ್ವಪ್ರತ್ಯಯ, options, git_worktree_usage, 0);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_ತುಂಬಿದ_ಸೂಚ್ಯಂಕ = 0;

	ಹಿಂತಿರುಗಿ fn(ac, av, ಪೂರ್ವಪ್ರತ್ಯಯ, repo);
}
