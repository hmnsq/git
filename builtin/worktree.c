#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DISABLE_SIGN_COMPARE_WARNINGS

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "abspath.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "advice.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "checkout.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "copy.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "dir.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hex.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-file.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-name.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "path.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "strvec.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "branch.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "read-cache-ll.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "refs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "remote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "run-command.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hook.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "sigchain.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "submodule.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utf8.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "worktree.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BUILTIN_WORKTREE_ਜੋੜੋ_USAGE \
	N_("git worktree add [-f] [--detach] [--checkout] [--lock [--reason <string>]]\n" \
	   "                 [--orphan] [(-b | -B) <new-branch>] <path> [<commit-ish>]")

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BUILTIN_WORKTREE_LIST_USAGE \
	N_("git worktree list [-v | --porcelain [-z]]")
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BUILTIN_WORKTREE_ਤਾਲਾ_USAGE \
	N_("git worktree lock [--reason <string>] <worktree>")
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BUILTIN_WORKTREE_MOVE_USAGE \
	N_("git worktree move <worktree> <new-path>")
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BUILTIN_WORKTREE_PRUNE_USAGE \
	N_("git worktree prune [-n] [-v] [--expire <expire>]")
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BUILTIN_WORKTREE_REMOVE_USAGE \
	N_("git worktree remove [-f] <worktree>")
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BUILTIN_WORKTREE_REPAIR_USAGE \
	N_("git worktree repair [<path>...]")
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BUILTIN_WORKTREE_UNLOCK_USAGE \
	N_("git worktree unlock <worktree>")

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ WORKTREE_ਜੋੜੋ_DWIM_ORPHAN_INFER_ਪਾਠ \
	_("No possible source branch, inferring '--orphan'")

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ WORKTREE_ਜੋੜੋ_ORPHAN_WITH_DASH_B_HINT_ਪਾਠ \
	_("If you meant to create a worktree containing a new unborn branch\n" \
	"(branch with no commits) for this repository, you can do so\n" \
	"using the --orphan flag:\n" \
	"\n" \
	"    git worktree add --orphan -b %s %s\n")

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ WORKTREE_ਜੋੜੋ_ORPHAN_NO_DASH_B_HINT_ਪਾਠ \
	_("If you meant to create a worktree containing a new unborn branch\n" \
	"(branch with no commits) for this repository, you can do so\n" \
	"using the --orphan flag:\n" \
	"\n" \
	"    git worktree add --orphan %s\n")

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ git_worktree_usage[] = {
	BUILTIN_WORKTREE_ਜੋੜੋ_USAGE,
	BUILTIN_WORKTREE_LIST_USAGE,
	BUILTIN_WORKTREE_ਤਾਲਾ_USAGE,
	BUILTIN_WORKTREE_MOVE_USAGE,
	BUILTIN_WORKTREE_PRUNE_USAGE,
	BUILTIN_WORKTREE_REMOVE_USAGE,
	BUILTIN_WORKTREE_REPAIR_USAGE,
	BUILTIN_WORKTREE_UNLOCK_USAGE,
	ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ git_worktree_ਜੋੜੋ_usage[] = {
	BUILTIN_WORKTREE_ਜੋੜੋ_USAGE,
	ਨਿਰਾ,
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ git_worktree_list_usage[] = {
	BUILTIN_WORKTREE_LIST_USAGE,
	ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ git_worktree_ਤਾਲਾ_usage[] = {
	BUILTIN_WORKTREE_ਤਾਲਾ_USAGE,
	ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ git_worktree_move_usage[] = {
	BUILTIN_WORKTREE_MOVE_USAGE,
	ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ git_worktree_prune_usage[] = {
	BUILTIN_WORKTREE_PRUNE_USAGE,
	ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ git_worktree_remove_usage[] = {
	BUILTIN_WORKTREE_REMOVE_USAGE,
	ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ git_worktree_repair_usage[] = {
	BUILTIN_WORKTREE_REPAIR_USAGE,
	ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ git_worktree_unlock_usage[] = {
	BUILTIN_WORKTREE_UNLOCK_USAGE,
	ਨਿਰਾ
};

ਬਣਤਰ ਜੋੜੋ_opts {
	ਪੂਰਨ force;
	ਪੂਰਨ detach;
	ਪੂਰਨ quiet;
	ਪੂਰਨ checkout;
	ਪੂਰਨ orphan;
	ਪੂਰਨ relative_paths;
	ਅਚਲ ਅੱਖਰ *keep_locked;
};

ਸਥਿਰ ਪੂਰਨ show_only;
ਸਥਿਰ ਪੂਰਨ verbose;
ਸਥਿਰ ਪੂਰਨ guess_remote;
ਸਥਿਰ ਪੂਰਨ use_relative_paths;
ਸਥਿਰ timestamp_t expire;

ਸਥਿਰ ਪੂਰਨ git_worktree_config(ਅਚਲ ਅੱਖਰ *var, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ,
			       ਅਚਲ ਬਣਤਰ config_context *ctx, ਖਾਲੀ *cb)
{
	ਜੇ (!strcmp(var, "worktree.guessremote")) {
		guess_remote = git_config_bool(var, ਮੁੱਲ);
		ਵਾਪਸ 0;
	} ਨਹੀਂਤਾਂ ਜੇ (!strcmp(var, "worktree.userelativepaths")) {
		use_relative_paths = git_config_bool(var, ਮੁੱਲ);
		ਵਾਪਸ 0;
	}

	ਵਾਪਸ git_default_config(var, ਮੁੱਲ, ctx, cb);
}

ਸਥਿਰ ਪੂਰਨ delete_git_dir(ਅਚਲ ਅੱਖਰ *id)
{
	ਬਣਤਰ strbuf sb = STRBUF_INIT;
	ਪੂਰਨ ret;

	repo_common_ਰਸਤਾ_append(the_repository, &sb, "worktrees/%s", id);
	ret = remove_dir_recursively(&sb, 0);
	ਜੇ (ret < 0 && errno == ENOTDIR)
		ret = unlink(sb.buf);
	ਜੇ (ret)
		error_errno(_("failed to delete '%s'"), sb.buf);
	strbuf_release(&sb);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਖਾਲੀ delete_worktrees_dir_if_ਖਾਲੀ_ਥਾਵਾਂ(ਖਾਲੀ)
{
	ਅੱਖਰ *ਰਸਤਾ = repo_git_ਰਸਤਾ(the_repository, "worktrees");
	rmdir(ਰਸਤਾ); /* ignore failed removal */
	ਮੁਕਤ(ਰਸਤਾ);
}

ਸਥਿਰ ਖਾਲੀ prune_worktree(ਅਚਲ ਅੱਖਰ *id, ਅਚਲ ਅੱਖਰ *reason)
{
	ਜੇ (show_only || verbose)
		fprintf_ln(stderr, _("Removing %s/%s: %s"), "worktrees", id, reason);
	ਜੇ (!show_only)
		delete_git_dir(id);
}

ਸਥਿਰ ਪੂਰਨ prune_cmp(ਅਚਲ ਖਾਲੀ *a, ਅਚਲ ਖਾਲੀ *b)
{
	ਅਚਲ ਬਣਤਰ string_list_ਵਸਤੂ *x = a;
	ਅਚਲ ਬਣਤਰ string_list_ਵਸਤੂ *y = b;
	ਪੂਰਨ c;

	ਜੇ ((c = fspathcmp(x->string, y->string)))
	    ਵਾਪਸ c;
	/*
	 * paths same; prune_dupes() removes all but the first worktree entry
	 * having the same path, so sort main worktree ('util' is NULL) above
	 * linked worktrees ('util' not NULL) since main worktree can't be
	 * removed
	 */
	ਜੇ (!x->util)
		ਵਾਪਸ -1;
	ਜੇ (!y->util)
		ਵਾਪਸ 1;
	/* paths same; sort by .git/worktrees/<id> */
	ਵਾਪਸ strcmp(x->util, y->util);
}

ਸਥਿਰ ਖਾਲੀ prune_dups(ਬਣਤਰ string_list *l)
{
	ਪੂਰਨ i;

	QSORT(l->ਚੀਜ਼ਾਂ, l->nr, prune_cmp);
	ਲਈ (i = 1; i < l->nr; i++) {
		ਜੇ (!fspathcmp(l->ਚੀਜ਼ਾਂ[i].string, l->ਚੀਜ਼ਾਂ[i - 1].string))
			prune_worktree(l->ਚੀਜ਼ਾਂ[i].util, "duplicate entry");
	}
}

ਸਥਿਰ ਖਾਲੀ prune_worktrees(ਖਾਲੀ)
{
	ਬਣਤਰ strbuf reason = STRBUF_INIT;
	ਬਣਤਰ strbuf main_ਰਸਤਾ = STRBUF_INIT;
	ਬਣਤਰ string_list kept = STRING_LIST_INIT_DUP;
	ਅੱਖਰ *ਰਸਤਾ;
	DIR *dir;
	ਬਣਤਰ dirent *d;

	ਰਸਤਾ = repo_git_ਰਸਤਾ(the_repository, "worktrees");
	dir = opendir(ਰਸਤਾ);
	ਮੁਕਤ(ਰਸਤਾ);
	ਜੇ (!dir)
		ਵਾਪਸ;
	ਜਦੋਂ ((d = readdir_skip_dot_and_dotdot(dir)) != ਨਿਰਾ) {
		ਅੱਖਰ *ਰਸਤਾ;
		strbuf_reset(&reason);
		ਜੇ (should_prune_worktree(the_repository, d->d_name, &reason, &ਰਸਤਾ, expire))
			prune_worktree(d->d_name, reason.buf);
		ਨਹੀਂਤਾਂ ਜੇ (ਰਸਤਾ)
			string_list_append_nodup(&kept, ਰਸਤਾ)->util = xstrdup(d->d_name);
	}
	closedir(dir);

	strbuf_ਜੋੜੋ_absolute_ਰਸਤਾ(&main_ਰਸਤਾ, repo_get_common_dir(the_repository));
	/* massage main worktree absolute path to match 'gitdir' content */
	strbuf_strip_suffix(&main_ਰਸਤਾ, "/.");
	string_list_append_nodup(&kept, strbuf_detach(&main_ਰਸਤਾ, ਨਿਰਾ));
	prune_dups(&kept);
	string_list_clear(&kept, 1);

	ਜੇ (!show_only)
		delete_worktrees_dir_if_ਖਾਲੀ_ਥਾਵਾਂ();
	strbuf_release(&reason);
}

ਸਥਿਰ ਪੂਰਨ prune(ਪੂਰਨ ac, ਅਚਲ ਅੱਖਰ **av, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		 ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ option options[] = {
		OPT_DRY_ਲੜੀਵਾਰ(&show_only, N_("do not remove, show only")),
		OPT__VERBOSE(&verbose, N_("report pruned working trees")),
		OPT_EXPIRY_DATE(0, "expire", &expire,
				N_("prune missing working trees older than <time>")),
		OPT_ਸਿਰਾ()
	};

	expire = ਸਮਾਂ_MAX;
	ac = parse_options(ac, av, ਅਗੇਤਰ, options, git_worktree_prune_usage,
			   0);
	ਜੇ (ac)
		usage_with_options(git_worktree_prune_usage, options);
	prune_worktrees();
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਅੱਖਰ *junk_work_ਰੁੱਖ;
ਸਥਿਰ ਅੱਖਰ *junk_git_dir;
ਸਥਿਰ ਪੂਰਨ is_junk;
ਸਥਿਰ pid_t junk_pid;

ਸਥਿਰ ਖਾਲੀ remove_junk(ਖਾਲੀ)
{
	ਬਣਤਰ strbuf sb = STRBUF_INIT;
	ਜੇ (!is_junk || getpid() != junk_pid)
		ਵਾਪਸ;
	ਜੇ (junk_git_dir) {
		strbuf_addstr(&sb, junk_git_dir);
		remove_dir_recursively(&sb, 0);
		strbuf_reset(&sb);
	}
	ਜੇ (junk_work_ਰੁੱਖ) {
		strbuf_addstr(&sb, junk_work_ਰੁੱਖ);
		remove_dir_recursively(&sb, 0);
	}
	strbuf_release(&sb);
}

ਸਥਿਰ ਖਾਲੀ remove_junk_on_signal(ਪੂਰਨ signo)
{
	remove_junk();
	sigchain_ਕੱਢੋ(signo);
	ਉਠਾਓ(signo);
}

ਸਥਿਰ ਅੱਖਰ *worktree_basename_dup(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ)
{
	ਅਚਲ ਅੱਖਰ *name;
	ਪੂਰਨ len;

	len = ਲੜੀ_ਲੰਬਾਈ(ਰਸਤਾ);
	ਜਦੋਂ (len && is_dir_sep(ਰਸਤਾ[len - 1]))
		len--;

	name = ਰਸਤਾ + len;
	ਜਦੋਂ (name > ਰਸਤਾ && !is_dir_sep(name[-1]))
		name--;

	ਵਾਪਸ xmemdupz(name, ਰਸਤਾ + len - name);
}

/* check that path is viable location for worktree */
ਸਥਿਰ ਖਾਲੀ check_candidate_ਰਸਤਾ(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ,
				 ਪੂਰਨ force,
				 ਬਣਤਰ worktree **worktrees,
				 ਅਚਲ ਅੱਖਰ *cmd)
{
	ਬਣਤਰ worktree *wt;
	ਪੂਰਨ locked;

	ਜੇ (file_exists(ਰਸਤਾ) && !is_ਖਾਲੀ_ਥਾਵਾਂ_dir(ਰਸਤਾ))
		die(_("'%s' already exists"), ਰਸਤਾ);

	wt = ਲੱਭੋ_worktree_by_ਰਸਤਾ(worktrees, ਰਸਤਾ);
	ਜੇ (!wt)
		ਵਾਪਸ;

	locked = !!worktree_ਤਾਲਾ_reason(wt);
	ਜੇ ((!locked && force) || (locked && force > 1)) {
		ਜੇ (delete_git_dir(wt->id))
		    die(_("unusable worktree destination '%s'"), ਰਸਤਾ);
		ਵਾਪਸ;
	}

	ਜੇ (locked)
		die(_("'%s' is a missing but locked worktree;\nuse '%s -f -f' to override, or 'unlock' and 'prune' or 'remove' to clear"), ਰਸਤਾ, cmd);
	ਨਹੀਂਤਾਂ
		die(_("'%s' is a missing but already registered worktree;\nuse '%s -f' to override, or 'prune' or 'remove' to clear"), ਰਸਤਾ, cmd);
}

ਸਥਿਰ ਖਾਲੀ copy_sparse_checkout(ਅਚਲ ਅੱਖਰ *worktree_git_dir)
{
	ਅੱਖਰ *from_file = repo_git_ਰਸਤਾ(the_repository, "info/sparse-checkout");
	ਅੱਖਰ *ਮੰਜ਼ਿਲ_file = xstrfmt("%s/info/sparse-checkout", worktree_git_dir);

	ਜੇ (file_exists(from_file)) {
		ਜੇ (ਸੁਰੱਖਿਅਤ_create_leading_directories(the_repository, ਮੰਜ਼ਿਲ_file) ||
			copy_file(the_repository, ਮੰਜ਼ਿਲ_file, from_file, 0666))
			error(_("failed to copy '%s' to '%s'; sparse-checkout may not work correctly"),
				from_file, ਮੰਜ਼ਿਲ_file);
	}

	ਮੁਕਤ(from_file);
	ਮੁਕਤ(ਮੰਜ਼ਿਲ_file);
}

ਸਥਿਰ ਖਾਲੀ copy_filtered_worktree_config(ਅਚਲ ਅੱਖਰ *worktree_git_dir)
{
	ਅੱਖਰ *from_file = repo_git_ਰਸਤਾ(the_repository, "config.worktree");
	ਅੱਖਰ *ਮੰਜ਼ਿਲ_file = xstrfmt("%s/config.worktree", worktree_git_dir);

	ਜੇ (file_exists(from_file)) {
		ਬਣਤਰ config_ਸੈੱਟ cs = { { 0 } };
		ਪੂਰਨ bare;

		ਜੇ (ਸੁਰੱਖਿਅਤ_create_leading_directories(the_repository, ਮੰਜ਼ਿਲ_file) ||
			copy_file(the_repository, ਮੰਜ਼ਿਲ_file, from_file, 0666)) {
			error(_("failed to copy worktree config from '%s' to '%s'"),
				from_file, ਮੰਜ਼ਿਲ_file);
			ਜਾਓ worktree_copy_cleanup;
		}

		git_configset_init(&cs);
		git_configset_ਜੋੜੋ_file(&cs, from_file);

		ਜੇ (!git_configset_get_bool(&cs, "core.bare", &bare) &&
			bare &&
			repo_config_ਸੈੱਟ_multivar_ਵਿੱਚ_file_gently(the_repository,
				ਮੰਜ਼ਿਲ_file, "core.bare", ਨਿਰਾ, "true", ਨਿਰਾ, 0))
			error(_("failed to unset '%s' in '%s'"),
				"core.bare", ਮੰਜ਼ਿਲ_file);
		ਜੇ (!git_configset_get(&cs, "core.worktree") &&
			repo_config_ਸੈੱਟ_ਵਿੱਚ_file_gently(the_repository, ਮੰਜ਼ਿਲ_file,
						       "core.worktree", ਨਿਰਾ, ਨਿਰਾ))
			error(_("failed to unset '%s' in '%s'"),
				"core.worktree", ਮੰਜ਼ਿਲ_file);

		git_configset_clear(&cs);
	}

worktree_copy_cleanup:
	ਮੁਕਤ(from_file);
	ਮੁਕਤ(ਮੰਜ਼ਿਲ_file);
}

ਸਥਿਰ ਪੂਰਨ checkout_worktree(ਅਚਲ ਬਣਤਰ ਜੋੜੋ_opts *opts,
			     ਬਣਤਰ strvec *ਬੱਚਾ_env)
{
	ਬਣਤਰ ਬੱਚਾ_process cp = ਬੱਚਾ_PROCESS_INIT;
	cp.git_cmd = 1;
	strvec_pushl(&cp.ਦਲੀਲਾਂ, "reset", "--hard", "--no-recurse-submodules", ਨਿਰਾ);
	ਜੇ (opts->quiet)
		strvec_ਧੱਕੋ(&cp.ਦਲੀਲਾਂ, "--quiet");
	strvec_pushv(&cp.env, ਬੱਚਾ_env->v);
	ਵਾਪਸ ਲੜੀਵਾਰ_command(&cp);
}

ਸਥਿਰ ਪੂਰਨ make_worktree_orphan(ਅਚਲ ਅੱਖਰ * ref, ਅਚਲ ਬਣਤਰ ਜੋੜੋ_opts *opts,
				ਬਣਤਰ strvec *ਬੱਚਾ_env)
{
	ਬਣਤਰ strbuf symref = STRBUF_INIT;
	ਬਣਤਰ ਬੱਚਾ_process cp = ਬੱਚਾ_PROCESS_INIT;

	validate_new_branchname(ref, &symref, 0);
	strvec_pushl(&cp.ਦਲੀਲਾਂ, "symbolic-ref", "HEAD", symref.buf, ਨਿਰਾ);
	ਜੇ (opts->quiet)
		strvec_ਧੱਕੋ(&cp.ਦਲੀਲਾਂ, "--quiet");
	strvec_pushv(&cp.env, ਬੱਚਾ_env->v);
	strbuf_release(&symref);
	cp.git_cmd = 1;
	ਵਾਪਸ ਲੜੀਵਾਰ_command(&cp);
}

/*
 * References for worktrees are generally stored in '$GIT_DIR/worktrees/<wt_id>'.
 * But when using alternate reference directories, we want to store the worktree
 * references in '$ALTERNATE_REFERENCE_DIR/worktrees/<wt_id>'.
 *
 * Create the necessary folder structure to facilitate the same. But to ensure
 * that the former path is still considered a Git directory, add stubs.
 */
ਸਥਿਰ ਖਾਲੀ ਤਿਆਰੀ_alternate_ref_dir(ਬਣਤਰ worktree *wt, ਅਚਲ ਅੱਖਰ *wt_git_ਰਸਤਾ)
{
	ਬਣਤਰ strbuf sb = STRBUF_INIT;
	ਅੱਖਰ *ਰਸਤਾ;

	ਰਸਤਾ = wt->repo->ref_storage_payload;
	ਜੇ (!ਰਸਤਾ)
		ਵਾਪਸ;

	ਜੇ (!is_absolute_ਰਸਤਾ(ਰਸਤਾ))
		strbuf_addf(&sb, "%s/", wt->repo->commondir);

	strbuf_addf(&sb, "%s/worktrees", ਰਸਤਾ);
	ਸੁਰੱਖਿਅਤ_create_dir(wt->repo, sb.buf, 1);
	strbuf_addf(&sb, "/%s", wt->id);
	ਸੁਰੱਖਿਅਤ_create_dir(wt->repo, sb.buf, 1);
	strbuf_reset(&sb);

	strbuf_addf(&sb, "this worktree stores references in %s/worktrees/%s",
		    ਰਸਤਾ, wt->id);
	refs_create_refdir_stubs(wt->repo, wt_git_ਰਸਤਾ, sb.buf);

	strbuf_release(&sb);
}

ਸਥਿਰ ਪੂਰਨ ਜੋੜੋ_worktree(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਅਚਲ ਅੱਖਰ *refname,
			ਅਚਲ ਬਣਤਰ ਜੋੜੋ_opts *opts)
{
	ਬਣਤਰ strbuf sb_git = STRBUF_INIT, sb_repo = STRBUF_INIT;
	ਬਣਤਰ strbuf sb = STRBUF_INIT;
	ਅਚਲ ਅੱਖਰ *name;
	ਅੱਖਰ *name_ਮੰਜ਼ਿਲ_free = ਨਿਰਾ;
	ਬਣਤਰ strvec ਬੱਚਾ_env = STRVEC_INIT;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ counter = 0;
	ਪੂਰਨ len, ret;
	ਬਣਤਰ strbuf symref = STRBUF_INIT;
	ਬਣਤਰ commit *commit = ਨਿਰਾ;
	ਪੂਰਨ is_branch = 0;
	ਬਣਤਰ strbuf sb_name = STRBUF_INIT;
	ਬਣਤਰ worktree **worktrees, *wt = ਨਿਰਾ;
	ਬਣਤਰ ref_store *wt_refs;
	ਬਣਤਰ repo_config_ਮੁੱਲਾਂ *cfg = repo_config_ਮੁੱਲਾਂ(the_repository);

	worktrees = get_worktrees(the_repository);
	check_candidate_ਰਸਤਾ(ਰਸਤਾ, opts->force, worktrees, "add");
	free_worktrees(worktrees);
	worktrees = ਨਿਰਾ;

	/* is 'refname' a branch or commit? */
	ਜੇ (!opts->detach && !check_branch_ref(the_repository, &symref, refname) &&
	    refs_ref_exists(get_main_ref_store(the_repository), symref.buf)) {
		is_branch = 1;
		ਜੇ (!opts->force)
			die_if_checked_ਬਾਹਰਲਾ(symref.buf, 0);
	}
	commit = lookup_commit_reference_by_name(refname);
	ਜੇ (!commit && !opts->orphan)
		die(_("invalid reference: %s"), refname);

	name = name_ਮੰਜ਼ਿਲ_free = worktree_basename_dup(ਰਸਤਾ);
	ਜੇ (!*name)
		die(_("invalid path '%s'"), ਰਸਤਾ);
	sanitize_refname_component(name, &sb_name);
	ਜੇ (!sb_name.len)
		BUG("How come '%s' becomes empty after sanitization?", name);
	name = sb_name.buf;
	repo_git_ਰਸਤਾ_replace(the_repository, &sb_repo, "worktrees/%s", name);
	len = sb_repo.len;
	ਜੇ (ਸੁਰੱਖਿਅਤ_create_leading_directories_const(the_repository, sb_repo.buf))
		die_errno(_("could not create leading directories of '%s'"),
			  sb_repo.buf);

	ਜਦੋਂ (mkdir(sb_repo.buf, 0777)) {
		counter++;
		ਜੇ ((errno != EEXIST) || !counter /*ਉਛਾਲ*/)
			die_errno(_("could not create directory of '%s'"),
				  sb_repo.buf);
		strbuf_setlen(&sb_repo, len);
		strbuf_addf(&sb_repo, "%d", counter);
	}
	name = strrchr(sb_repo.buf, '/') + 1;

	junk_pid = getpid();
	atexit(remove_junk);
	sigchain_ਧੱਕੋ_common(remove_junk_on_signal);

	junk_git_dir = xstrdup(sb_repo.buf);
	is_junk = 1;

	/*
	 * lock the incomplete repo so prune won't delete it, unlock
	 * after the preparation is over.
	 */
	strbuf_addf(&sb, "%s/locked", sb_repo.buf);
	ਜੇ (opts->keep_locked)
		write_file(sb.buf, "%s", opts->keep_locked);
	ਨਹੀਂਤਾਂ
		write_file(sb.buf, _("initializing"));

	strbuf_addf(&sb_git, "%s/.git", ਰਸਤਾ);
	ਜੇ (ਸੁਰੱਖਿਅਤ_create_leading_directories_const(the_repository, sb_git.buf))
		die_errno(_("could not create leading directories of '%s'"),
			  sb_git.buf);
	junk_work_ਰੁੱਖ = xstrdup(ਰਸਤਾ);

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
	ਜੇ (!wt) {
		ret = error(_("could not find created worktree '%s'"), name);
		ਜਾਓ ਪੂਰਾ;
	}
	ਤਿਆਰੀ_alternate_ref_dir(wt, sb_repo.buf);
	wt_refs = get_worktree_ref_store(wt);

	ret = ref_store_create_on_disk(wt_refs, REF_STORE_CREATE_ON_DISK_IS_WORKTREE, &sb);
	ਜੇ (ret)
		ਜਾਓ ਪੂਰਾ;

	ਜੇ (!is_branch && commit)
		ret = refs_update_ref(wt_refs, ਨਿਰਾ, "HEAD", &commit->object.oid,
				      ਨਿਰਾ, 0, UPDATE_REFS_ਸੁਨੇਹਾ_ON_ERR);
	ਨਹੀਂਤਾਂ
		ret = refs_update_symref(wt_refs, "HEAD", symref.buf, ਨਿਰਾ);
	ਜੇ (ret)
		ਜਾਓ ਪੂਰਾ;

	/*
	 * If the current worktree has sparse-checkout enabled, then copy
	 * the sparse-checkout patterns from the current worktree.
	 */
	ਜੇ (cfg->apply_sparse_checkout)
		copy_sparse_checkout(sb_repo.buf);

	/*
	 * If we are using worktree config, then copy all current config
	 * values from the current worktree into the new one, that way the
	 * new worktree behaves the same as this one.
	 */
	ਜੇ (the_repository->repository_format_worktree_config)
		copy_filtered_worktree_config(sb_repo.buf);

	strvec_pushf(&ਬੱਚਾ_env, "%s=%s", GIT_DIR_ENVIRONMENT, sb_git.buf);
	strvec_pushf(&ਬੱਚਾ_env, "%s=%s", GIT_WORK_ਰੁੱਖ_ENVIRONMENT, ਰਸਤਾ);

	ਜੇ (opts->orphan &&
	    (ret = make_worktree_orphan(refname, opts, &ਬੱਚਾ_env)))
		ਜਾਓ ਪੂਰਾ;

	ਜੇ (opts->checkout &&
	    (ret = checkout_worktree(opts, &ਬੱਚਾ_env)))
		ਜਾਓ ਪੂਰਾ;

	is_junk = 0;
	FREE_AND_NULL(junk_work_ਰੁੱਖ);
	FREE_AND_NULL(junk_git_dir);

ਪੂਰਾ:
	ਜੇ (ret || !opts->keep_locked) {
		strbuf_reset(&sb);
		strbuf_addf(&sb, "%s/locked", sb_repo.buf);
		unlink_or_warn(sb.buf);
	}

	/*
	 * Hook failure does not warrant worktree deletion, so run hook after
	 * is_junk is cleared, but do return appropriate code when hook fails.
	 */
	ਜੇ (!ret && opts->checkout && !opts->orphan) {
		ਬਣਤਰ ਲੜੀਵਾਰ_hooks_opt opt = ਲੜੀਵਾਰ_HOOKS_OPT_INIT_FORCE_SERIAL;

		strvec_pushl(&opt.env, "GIT_DIR", "GIT_WORK_TREE", ਨਿਰਾ);
		strvec_pushl(&opt.ਦਲੀਲਾਂ,
			     oid_ਮੰਜ਼ਿਲ_hex(null_oid(the_hash_algo)),
			     oid_ਮੰਜ਼ਿਲ_hex(&commit->object.oid),
			     "1",
			     ਨਿਰਾ);
		opt.dir = ਰਸਤਾ;

		ret = ਲੜੀਵਾਰ_hooks_opt(the_repository, "post-checkout", &opt);
	}

	strvec_clear(&ਬੱਚਾ_env);
	strbuf_release(&sb);
	strbuf_release(&symref);
	strbuf_release(&sb_repo);
	strbuf_release(&sb_git);
	strbuf_release(&sb_name);
	free_worktree(wt);
	ਮੁਕਤ(name_ਮੰਜ਼ਿਲ_free);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਖਾਲੀ print_preparing_worktree_ਸਤਰ(ਪੂਰਨ detach,
					  ਅਚਲ ਅੱਖਰ *branch,
					  ਅਚਲ ਅੱਖਰ *new_branch,
					  ਪੂਰਨ force_new_branch)
{
	ਜੇ (force_new_branch) {
		ਬਣਤਰ commit *commit = lookup_commit_reference_by_name(new_branch);
		ਜੇ (!commit)
			fprintf_ln(stderr, _("Preparing worktree (new branch '%s')"), new_branch);
		ਨਹੀਂਤਾਂ
			fprintf_ln(stderr, _("Preparing worktree (resetting branch '%s'; was at %s)"),
				  new_branch,
				  repo_ਲੱਭੋ_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV));
	} ਨਹੀਂਤਾਂ ਜੇ (new_branch) {
		fprintf_ln(stderr, _("Preparing worktree (new branch '%s')"), new_branch);
	} ਨਹੀਂਤਾਂ {
		ਬਣਤਰ strbuf s = STRBUF_INIT;
		ਜੇ (!detach && !check_branch_ref(the_repository, &s, branch) &&
		    refs_ref_exists(get_main_ref_store(the_repository), s.buf))
			fprintf_ln(stderr, _("Preparing worktree (checking out '%s')"),
				  branch);
		ਨਹੀਂਤਾਂ {
			ਬਣਤਰ commit *commit = lookup_commit_reference_by_name(branch);
			ਜੇ (!commit)
				BUG("unreachable: invalid reference: %s", branch);
			fprintf_ln(stderr, _("Preparing worktree (detached HEAD %s)"),
				  repo_ਲੱਭੋ_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV));
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
ਸਥਿਰ ਪੂਰਨ ਪਹਿਲਾ_valid_ref(ਅਚਲ ਬਣਤਰ reference *ref ਅਣਵਰਤਿਆ, ਖਾਲੀ *cb_ਅੰਕੜੇ ਅਣਵਰਤਿਆ)
{
	ਵਾਪਸ 1;
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
ਸਥਿਰ ਪੂਰਨ can_use_local_refs(ਅਚਲ ਬਣਤਰ ਜੋੜੋ_opts *opts)
{
	ਜੇ (refs_ਸਿਰ_ref(get_main_ref_store(the_repository), ਪਹਿਲਾ_valid_ref, ਨਿਰਾ)) {
		ਵਾਪਸ 1;
	} ਨਹੀਂਤਾਂ ਜੇ (refs_for_each_branch_ref(get_main_ref_store(the_repository), ਪਹਿਲਾ_valid_ref, ਨਿਰਾ)) {
		ਜੇ (!opts->quiet)
			warning(_("HEAD points to an invalid (or orphaned) reference.\n"));
		ਵਾਪਸ 1;
	}
	ਵਾਪਸ 0;
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
ਸਥਿਰ ਪੂਰਨ can_use_remote_refs(ਅਚਲ ਬਣਤਰ ਜੋੜੋ_opts *opts)
{
	ਜੇ (!guess_remote) {
		ਵਾਪਸ 0;
	} ਨਹੀਂਤਾਂ ਜੇ (refs_for_each_remote_ref(get_main_ref_store(the_repository), ਪਹਿਲਾ_valid_ref, ਨਿਰਾ)) {
		ਵਾਪਸ 1;
	} ਨਹੀਂਤਾਂ ਜੇ (!opts->force && remote_get(ਨਿਰਾ)) {
		die(_("No local or remote refs exist despite at least one remote\n"
		      "present, stopping; use 'add -f' to override or fetch a remote first"));
	}
	ਵਾਪਸ 0;
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
ਸਥਿਰ ਪੂਰਨ dwim_orphan(ਅਚਲ ਬਣਤਰ ਜੋੜੋ_opts *opts, ਪੂਰਨ opt_track, ਪੂਰਨ remote)
{
	ਜੇ (can_use_local_refs(opts)) {
		ਵਾਪਸ 0;
	} ਨਹੀਂਤਾਂ ਜੇ (remote && can_use_remote_refs(opts)) {
		ਵਾਪਸ 0;
	} ਨਹੀਂਤਾਂ ਜੇ (!opts->quiet) {
		fprintf_ln(stderr, WORKTREE_ਜੋੜੋ_DWIM_ORPHAN_INFER_ਪਾਠ);
	}

	ਜੇ (opt_track) {
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--track");
	} ਨਹੀਂਤਾਂ ਜੇ (!opts->checkout) {
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--no-checkout");
	}
	ਵਾਪਸ 1;
}

ਸਥਿਰ ਖਾਲੀ advise_disambiguating_remotes(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਅਚਲ ਅੱਖਰ *branch,
					  ਅਚਲ ਬਣਤਰ string_list *matched_remote_ਨਾਂ)
{
	ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ;

	advise(_("Branch name '%s' appears in multiple remotes:"), branch);
	for_each_string_list_ਵਸਤੂ(ਵਸਤੂ, matched_remote_ਨਾਂ) {
		advise(_("  %s"), ਵਸਤੂ->string);
	}
	advise(_("If you meant to create a worktree from a remote tracking branch on\n"
		 "<remote>, you can do so by:\n"
		 "\n"
		 "    git worktree add -b %s %s <remote>/%s\n"
		 "\n"
		 "If you'd like to always prefer some remote, e.g. 'origin',\n"
		 "consider setting checkout.defaultRemote=origin in your config."),
	       branch, ਰਸਤਾ, branch);
}

ਸਥਿਰ ਅੱਖਰ *dwim_branch(ਅਚਲ ਬਣਤਰ ਜੋੜੋ_opts *opts, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਅੱਖਰ **new_branch)
{
	ਪੂਰਨ branch_exists;
	ਅੱਖਰ *branchname = worktree_basename_dup(ਰਸਤਾ);
	ਬਣਤਰ strbuf ref = STRBUF_INIT;

	branch_exists = !check_branch_ref(the_repository, &ref, branchname) &&
			refs_ref_exists(get_main_ref_store(the_repository),
					ref.buf);
	strbuf_release(&ref);
	ਜੇ (branch_exists)
		ਵਾਪਸ branchname;

	*new_branch = branchname;
	ਜੇ (guess_remote) {
		ਬਣਤਰ object_id oid;
		ਅੱਖਰ *remote;
		ਪੂਰਨ ਅੰਕ_matches = 0;
		ਬਣਤਰ string_list matched_remote_ਨਾਂ = STRING_LIST_INIT_DUP;

		remote = unique_tracking_name(*new_branch, &oid, &ਅੰਕ_matches,
					      &matched_remote_ਨਾਂ);
		ਜੇ (!remote && ਅੰਕ_matches > 1) {
			ਜੇ (!opts->quiet &&
			    advice_enabled(ADVICE_CHECKOUT_AMBIGUOUS_REMOTE_BRANCH_NAME))
				advise_disambiguating_remotes(ਰਸਤਾ, *new_branch,
							      &matched_remote_ਨਾਂ);
			die(_("'%s' matched multiple (%d) remote tracking branches"),
			    *new_branch, ਅੰਕ_matches);
		}
		string_list_clear(&matched_remote_ਨਾਂ, 0);
		ਵਾਪਸ remote;
	}
	ਵਾਪਸ ਨਿਰਾ;
}

ਸਥਿਰ ਪੂਰਨ ਜੋੜੋ(ਪੂਰਨ ac, ਅਚਲ ਅੱਖਰ **av, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
	       ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ ਜੋੜੋ_opts opts;
	ਅਚਲ ਅੱਖਰ *new_branch_force = ਨਿਰਾ;
	ਅੱਖਰ *ਰਸਤਾ;
	ਅਚਲ ਅੱਖਰ *branch;
	ਅੱਖਰ *branch_ਮੰਜ਼ਿਲ_free = ਨਿਰਾ;
	ਅੱਖਰ *new_branch_ਮੰਜ਼ਿਲ_free = ਨਿਰਾ;
	ਅਚਲ ਅੱਖਰ *new_branch = ਨਿਰਾ;
	ਅੱਖਰ *opt_track = ਨਿਰਾ;
	ਅਚਲ ਅੱਖਰ *ਤਾਲਾ_reason = ਨਿਰਾ;
	ਪੂਰਨ keep_locked = 0;
	ਪੂਰਨ ਵਰਤਿਆ_new_branch_options;
	ਬਣਤਰ option options[] = {
		OPT__FORCE(&opts.force,
			   N_("checkout <branch> even if already checked out in other worktree"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_STRING('b', ਨਿਰਾ, &new_branch, N_("branch"),
			   N_("create a new branch")),
		OPT_STRING('B', ਨਿਰਾ, &new_branch_force, N_("branch"),
			   N_("create or reset a branch")),
		OPT_BOOL(0, "orphan", &opts.orphan, N_("create unborn branch")),
		OPT_BOOL('d', "detach", &opts.detach, N_("detach HEAD at named commit")),
		OPT_BOOL(0, "checkout", &opts.checkout, N_("populate the new working tree")),
		OPT_BOOL(0, "lock", &keep_locked, N_("keep the new working tree locked")),
		OPT_STRING(0, "reason", &ਤਾਲਾ_reason, N_("string"),
			   N_("reason for locking")),
		OPT__QUIET(&opts.quiet, N_("suppress progress reporting")),
		OPT_PASSTHRU(0, "track", &opt_track, ਨਿਰਾ,
			     N_("set up tracking mode (see git-branch(1))"),
			     PARSE_OPT_NOARG | PARSE_OPT_OPTARG),
		OPT_BOOL(0, "guess-remote", &guess_remote,
			 N_("try to match the new branch name with a remote-tracking branch")),
		OPT_BOOL(0, "relative-paths", &opts.relative_paths,
			 N_("use relative paths for worktrees")),
		OPT_ਸਿਰਾ()
	};
	ਪੂਰਨ ret;

	memset(&opts, 0, ਆਕਾਰ(opts));
	opts.checkout = 1;
	opts.relative_paths = use_relative_paths;
	ac = parse_options(ac, av, ਅਗੇਤਰ, options, git_worktree_ਜੋੜੋ_usage, 0);
	ਜੇ (!!opts.detach + !!new_branch + !!new_branch_force > 1)
		die(_("options '%s', '%s', and '%s' cannot be used together"), "-b", "-B", "--detach");
	ਜੇ (opts.detach && opts.orphan)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--detach");
	ਜੇ (opts.orphan && opt_track)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--track");
	ਜੇ (opts.orphan && !opts.checkout)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--no-checkout");
	ਜੇ (opts.orphan && ac == 2)
		die(_("option '%s' and commit-ish cannot be used together"),
		    "--orphan");
	ਜੇ (ਤਾਲਾ_reason && !keep_locked)
		die(_("the option '%s' requires '%s'"), "--reason", "--lock");
	ਜੇ (ਤਾਲਾ_reason)
		opts.keep_locked = ਤਾਲਾ_reason;
	ਨਹੀਂਤਾਂ ਜੇ (keep_locked)
		opts.keep_locked = _("added with --lock");

	ਜੇ (ac < 1 || ac > 2)
		usage_with_options(git_worktree_ਜੋੜੋ_usage, options);

	ਰਸਤਾ = ਅਗੇਤਰ_filename(ਅਗੇਤਰ, av[0]);
	branch = ac < 2 ? "HEAD" : av[1];
	ਵਰਤਿਆ_new_branch_options = new_branch || new_branch_force;

	ਜੇ (!strcmp(branch, "-"))
		branch = "@{-1}";

	ਜੇ (new_branch_force) {
		ਬਣਤਰ strbuf symref = STRBUF_INIT;

		new_branch = new_branch_force;

		ਜੇ (!opts.force &&
		    !check_branch_ref(the_repository, &symref, new_branch) &&
		    refs_ref_exists(get_main_ref_store(the_repository), symref.buf))
			die_if_checked_ਬਾਹਰਲਾ(symref.buf, 0);
		strbuf_release(&symref);
	}

	ਜੇ (opts.orphan && !new_branch) {
		new_branch = new_branch_ਮੰਜ਼ਿਲ_free = worktree_basename_dup(ਰਸਤਾ);
	} ਨਹੀਂਤਾਂ ਜੇ (opts.orphan) {
		; /* no-op */
	} ਨਹੀਂਤਾਂ ਜੇ (opts.detach) {
		/* Check HEAD */
		ਜੇ (!strcmp(branch, "HEAD"))
			can_use_local_refs(&opts);
	} ਨਹੀਂਤਾਂ ਜੇ (ac < 2 && new_branch) {
		/* DWIM: Infer --orphan when repo has no refs. */
		opts.orphan = dwim_orphan(&opts, !!opt_track, 0);
	} ਨਹੀਂਤਾਂ ਜੇ (ac < 2) {
		/* DWIM: Guess branch name from path. */
		ਅੱਖਰ *s = dwim_branch(&opts, ਰਸਤਾ, &new_branch_ਮੰਜ਼ਿਲ_free);
		ਜੇ (s)
			branch = branch_ਮੰਜ਼ਿਲ_free = s;
		new_branch = new_branch_ਮੰਜ਼ਿਲ_free;

		/* DWIM: Infer --orphan when repo has no refs. */
		opts.orphan = (!s) && dwim_orphan(&opts, !!opt_track, 1);
	} ਨਹੀਂਤਾਂ ਜੇ (ac == 2 && new_branch) {
		ਜੇ (!strcmp(branch, "HEAD"))
			can_use_local_refs(&opts);
	} ਨਹੀਂਤਾਂ ਜੇ (ac == 2) {
		ਬਣਤਰ commit *commit;

		commit = lookup_commit_reference_by_name(branch);
		ਜੇ (!commit) {
			ਬਣਤਰ object_id oid;
			ਅੱਖਰ *remote;
			ਪੂਰਨ ਅੰਕ_matches = 0;
			ਬਣਤਰ string_list matched_remote_ਨਾਂ = STRING_LIST_INIT_DUP;

			remote = unique_tracking_name(branch, &oid, &ਅੰਕ_matches,
						      &matched_remote_ਨਾਂ);
			ਜੇ (remote) {
				new_branch = branch;
				branch = new_branch_ਮੰਜ਼ਿਲ_free = remote;
			} ਨਹੀਂਤਾਂ ਜੇ (ਅੰਕ_matches > 1) {
				ਜੇ (!opts.quiet &&
				    advice_enabled(ADVICE_CHECKOUT_AMBIGUOUS_REMOTE_BRANCH_NAME))
					advise_disambiguating_remotes(ਰਸਤਾ, branch,
								      &matched_remote_ਨਾਂ);
				die(_("'%s' matched multiple (%d) remote tracking branches"),
				    branch, ਅੰਕ_matches);
			}
			string_list_clear(&matched_remote_ਨਾਂ, 0);
		}

		ਜੇ (!strcmp(branch, "HEAD"))
			can_use_local_refs(&opts);

	}

	ਜੇ (!opts.orphan && !lookup_commit_reference_by_name(branch)) {
		ਪੂਰਨ attempt_hint = !opts.quiet && (ac < 2);
		ਜੇ (attempt_hint && ਵਰਤਿਆ_new_branch_options) {
			advise_if_enabled(ADVICE_WORKTREE_ਜੋੜੋ_ORPHAN,
				WORKTREE_ਜੋੜੋ_ORPHAN_WITH_DASH_B_HINT_ਪਾਠ,
				new_branch, ਰਸਤਾ);
		} ਨਹੀਂਤਾਂ ਜੇ (attempt_hint) {
			advise_if_enabled(ADVICE_WORKTREE_ਜੋੜੋ_ORPHAN,
				WORKTREE_ਜੋੜੋ_ORPHAN_NO_DASH_B_HINT_ਪਾਠ, ਰਸਤਾ);
		}
		die(_("invalid reference: %s"), branch);
	}

	ਜੇ (!opts.quiet)
		print_preparing_worktree_ਸਤਰ(opts.detach, branch, new_branch, !!new_branch_force);

	ਜੇ (opts.orphan) {
		branch = new_branch;
	} ਨਹੀਂਤਾਂ ਜੇ (new_branch) {
		ਬਣਤਰ ਬੱਚਾ_process cp = ਬੱਚਾ_PROCESS_INIT;
		cp.git_cmd = 1;
		strvec_ਧੱਕੋ(&cp.ਦਲੀਲਾਂ, "branch");
		ਜੇ (new_branch_force)
			strvec_ਧੱਕੋ(&cp.ਦਲੀਲਾਂ, "--force");
		ਜੇ (opts.quiet)
			strvec_ਧੱਕੋ(&cp.ਦਲੀਲਾਂ, "--quiet");
		strvec_ਧੱਕੋ(&cp.ਦਲੀਲਾਂ, new_branch);
		strvec_ਧੱਕੋ(&cp.ਦਲੀਲਾਂ, branch);
		ਜੇ (opt_track)
			strvec_ਧੱਕੋ(&cp.ਦਲੀਲਾਂ, opt_track);
		ਜੇ (ਲੜੀਵਾਰ_command(&cp)) {
			ret = -1;
			ਜਾਓ cleanup;
		}
		branch = new_branch;
	} ਨਹੀਂਤਾਂ ਜੇ (opt_track) {
		die(_("--[no-]track can only be used if a new branch is created"));
	}

	ret = ਜੋੜੋ_worktree(ਰਸਤਾ, branch, &opts);
cleanup:
	ਮੁਕਤ(ਰਸਤਾ);
	ਮੁਕਤ(opt_track);
	ਮੁਕਤ(branch_ਮੰਜ਼ਿਲ_free);
	ਮੁਕਤ(new_branch_ਮੰਜ਼ਿਲ_free);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਖਾਲੀ show_worktree_porcelain(ਬਣਤਰ worktree *wt, ਪੂਰਨ ਸਤਰ_terminator)
{
	ਅਚਲ ਅੱਖਰ *reason;

	ਲਿਖੋ("worktree %s%c", wt->ਰਸਤਾ, ਸਤਰ_terminator);
	ਜੇ (wt->is_bare)
		ਲਿਖੋ("bare%c", ਸਤਰ_terminator);
	ਨਹੀਂਤਾਂ {
		ਲਿਖੋ("HEAD %s%c", oid_ਮੰਜ਼ਿਲ_hex(&wt->ਸਿਰ_oid), ਸਤਰ_terminator);
		ਜੇ (wt->is_detached)
			ਲਿਖੋ("detached%c", ਸਤਰ_terminator);
		ਨਹੀਂਤਾਂ ਜੇ (wt->ਸਿਰ_ref)
			ਲਿਖੋ("branch %s%c", wt->ਸਿਰ_ref, ਸਤਰ_terminator);
	}

	reason = worktree_ਤਾਲਾ_reason(wt);
	ਜੇ (reason) {
		fputs("locked", stdout);
		ਜੇ (*reason) {
			fputc(' ', stdout);
			write_name_quoted(reason, stdout, ਸਤਰ_terminator);
		} ਨਹੀਂਤਾਂ {
			fputc(ਸਤਰ_terminator, stdout);
		}
	}

	reason = worktree_prune_reason(wt, expire);
	ਜੇ (reason)
		ਲਿਖੋ("prunable %s%c", reason, ਸਤਰ_terminator);

	fputc(ਸਤਰ_terminator, stdout);
}

ਬਣਤਰ worktree_display {
	ਅੱਖਰ *ਰਸਤਾ;
	ਪੂਰਨ ਚੌੜਾਈ;
};

ਸਥਿਰ ਖਾਲੀ show_worktree(ਬਣਤਰ worktree *wt, ਬਣਤਰ worktree_display *display,
			  ਪੂਰਨ ਰਸਤਾ_maxwidth, ਪੂਰਨ abbrev_len)
{
	ਬਣਤਰ strbuf sb = STRBUF_INIT;
	ਅਚਲ ਅੱਖਰ *reason;

	strbuf_addf(&sb, "%s%*s", display->ਰਸਤਾ, 1 + ਰਸਤਾ_maxwidth - display->ਚੌੜਾਈ, "");
	ਜੇ (wt->is_bare)
		strbuf_addstr(&sb, "(bare)");
	ਨਹੀਂਤਾਂ {
		strbuf_addf(&sb, "%-*s ", abbrev_len,
				repo_ਲੱਭੋ_unique_abbrev(the_repository, &wt->ਸਿਰ_oid, DEFAULT_ABBREV));
		ਜੇ (wt->is_detached)
			strbuf_addstr(&sb, "(detached HEAD)");
		ਨਹੀਂਤਾਂ ਜੇ (wt->ਸਿਰ_ref) {
			ਅੱਖਰ *ref = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
								 wt->ਸਿਰ_ref,
								 0);
			strbuf_addf(&sb, "[%s]", ref);
			ਮੁਕਤ(ref);
		} ਨਹੀਂਤਾਂ
			strbuf_addstr(&sb, "(error)");
	}

	reason = worktree_ਤਾਲਾ_reason(wt);
	ਜੇ (verbose && reason && *reason)
		strbuf_addf(&sb, "\n\tlocked: %s", reason);
	ਨਹੀਂਤਾਂ ਜੇ (reason)
		strbuf_addstr(&sb, " locked");

	reason = worktree_prune_reason(wt, expire);
	ਜੇ (verbose && reason)
		strbuf_addf(&sb, "\n\tprunable: %s", reason);
	ਨਹੀਂਤਾਂ ਜੇ (reason)
		strbuf_addstr(&sb, " prunable");

	ਲਿਖੋ("%s\n", sb.buf);
	strbuf_release(&sb);
}

ਸਥਿਰ ਖਾਲੀ measure_widths(ਬਣਤਰ worktree **wt, ਪੂਰਨ *abbrev,
			   ਬਣਤਰ worktree_display **d, ਪੂਰਨ *maxwidth)
{
	ਪੂਰਨ i, display_alloc = 0;
	ਬਣਤਰ worktree_display *display = ਨਿਰਾ;
	ਬਣਤਰ strbuf buf = STRBUF_INIT;

	ਲਈ (i = 0; wt[i]; i++) {
		ਪੂਰਨ sha1_len;
		ALLOC_GROW(display, i + 1, display_alloc);
		quote_ਰਸਤਾ(wt[i]->ਰਸਤਾ, ਨਿਰਾ, &buf, 0);
		display[i].ਚੌੜਾਈ = utf8_strwidth(buf.buf);
		display[i].ਰਸਤਾ = strbuf_detach(&buf, ਨਿਰਾ);

		ਜੇ (display[i].ਚੌੜਾਈ > *maxwidth)
			*maxwidth = display[i].ਚੌੜਾਈ;
		sha1_len = ਲੜੀ_ਲੰਬਾਈ(repo_ਲੱਭੋ_unique_abbrev(the_repository, &wt[i]->ਸਿਰ_oid, *abbrev));
		ਜੇ (sha1_len > *abbrev)
			*abbrev = sha1_len;
	}
	*d = display;
}

ਸਥਿਰ ਪੂਰਨ pathcmp(ਅਚਲ ਖਾਲੀ *a_, ਅਚਲ ਖਾਲੀ *b_)
{
	ਅਚਲ ਬਣਤਰ worktree *ਅਚਲ *a = a_;
	ਅਚਲ ਬਣਤਰ worktree *ਅਚਲ *b = b_;
	ਵਾਪਸ fspathcmp((*a)->ਰਸਤਾ, (*b)->ਰਸਤਾ);
}

ਸਥਿਰ ਖਾਲੀ pathsort(ਬਣਤਰ worktree **wt)
{
	ਪੂਰਨ n = 0;
	ਬਣਤਰ worktree **p = wt;

	ਜਦੋਂ (*p++)
		n++;
	QSORT(wt, n, pathcmp);
}

ਸਥਿਰ ਪੂਰਨ list(ਪੂਰਨ ac, ਅਚਲ ਅੱਖਰ **av, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ porcelain = 0;
	ਪੂਰਨ ਸਤਰ_terminator = '\n';

	ਬਣਤਰ option options[] = {
		OPT_BOOL(0, "porcelain", &porcelain, N_("machine-readable output")),
		OPT__VERBOSE(&verbose, N_("show extended annotations and reasons, if available")),
		OPT_EXPIRY_DATE(0, "expire", &expire,
				N_("add 'prunable' annotation to missing worktrees older than <time>")),
		OPT_ਸੈੱਟ_INT('z', ਨਿਰਾ, &ਸਤਰ_terminator,
			    N_("terminate records with a NUL character"), '\0'),
		OPT_ਸਿਰਾ()
	};

	expire = ਸਮਾਂ_MAX;
	ac = parse_options(ac, av, ਅਗੇਤਰ, options, git_worktree_list_usage, 0);
	ਜੇ (ac)
		usage_with_options(git_worktree_list_usage, options);
	ਨਹੀਂਤਾਂ ਜੇ (verbose && porcelain)
		die(_("options '%s' and '%s' cannot be used together"), "--verbose", "--porcelain");
	ਨਹੀਂਤਾਂ ਜੇ (!ਸਤਰ_terminator && !porcelain)
		die(_("the option '%s' requires '%s'"), "-z", "--porcelain");
	ਨਹੀਂਤਾਂ {
		ਬਣਤਰ worktree **worktrees = get_worktrees(the_repository);
		ਪੂਰਨ ਰਸਤਾ_maxwidth = 0, abbrev = DEFAULT_ABBREV, i;
		ਬਣਤਰ worktree_display *display = ਨਿਰਾ;

		/* sort worktrees by path but keep main worktree at top */
		pathsort(worktrees + 1);

		ਜੇ (!porcelain)
			measure_widths(worktrees, &abbrev,
				       &display, &ਰਸਤਾ_maxwidth);

		ਲਈ (i = 0; worktrees[i]; i++) {
			ਜੇ (porcelain)
				show_worktree_porcelain(worktrees[i],
							ਸਤਰ_terminator);
			ਨਹੀਂਤਾਂ
				show_worktree(worktrees[i],
					      &display[i], ਰਸਤਾ_maxwidth, abbrev);
		}
		ਲਈ (i = 0; display && worktrees[i]; i++)
			ਮੁਕਤ(display[i].ਰਸਤਾ);
		ਮੁਕਤ(display);
		free_worktrees(worktrees);
	}
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ ਤਾਲਾ_worktree(ਪੂਰਨ ac, ਅਚਲ ਅੱਖਰ **av, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			 ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਅਚਲ ਅੱਖਰ *reason = "", *old_reason;
	ਬਣਤਰ option options[] = {
		OPT_STRING(0, "reason", &reason, N_("string"),
			   N_("reason for locking")),
		OPT_ਸਿਰਾ()
	};
	ਬਣਤਰ worktree **worktrees, *wt;
	ਅੱਖਰ *ਰਸਤਾ;

	ac = parse_options(ac, av, ਅਗੇਤਰ, options, git_worktree_ਤਾਲਾ_usage, 0);
	ਜੇ (ac != 1)
		usage_with_options(git_worktree_ਤਾਲਾ_usage, options);

	worktrees = get_worktrees(the_repository);
	wt = ਲੱਭੋ_worktree(worktrees, ਅਗੇਤਰ, av[0]);
	ਜੇ (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	ਜੇ (is_main_worktree(wt))
		die(_("The main working tree cannot be locked or unlocked"));

	old_reason = worktree_ਤਾਲਾ_reason(wt);
	ਜੇ (old_reason) {
		ਜੇ (*old_reason)
			die(_("'%s' is already locked, reason: %s"),
			    av[0], old_reason);
		die(_("'%s' is already locked"), av[0]);
	}

	ਰਸਤਾ = repo_common_ਰਸਤਾ(the_repository, "worktrees/%s/locked", wt->id);
	write_file(ਰਸਤਾ, "%s", reason);

	free_worktrees(worktrees);
	ਮੁਕਤ(ਰਸਤਾ);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ unlock_worktree(ਪੂਰਨ ac, ਅਚਲ ਅੱਖਰ **av, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			   ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ option options[] = {
		OPT_ਸਿਰਾ()
	};
	ਬਣਤਰ worktree **worktrees, *wt;
	ਅੱਖਰ *ਰਸਤਾ;
	ਪੂਰਨ ret;

	ac = parse_options(ac, av, ਅਗੇਤਰ, options, git_worktree_unlock_usage, 0);
	ਜੇ (ac != 1)
		usage_with_options(git_worktree_unlock_usage, options);

	worktrees = get_worktrees(the_repository);
	wt = ਲੱਭੋ_worktree(worktrees, ਅਗੇਤਰ, av[0]);
	ਜੇ (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	ਜੇ (is_main_worktree(wt))
		die(_("The main working tree cannot be locked or unlocked"));
	ਜੇ (!worktree_ਤਾਲਾ_reason(wt))
		die(_("'%s' is not locked"), av[0]);

	ਰਸਤਾ = repo_common_ਰਸਤਾ(the_repository, "worktrees/%s/locked", wt->id);
	ret = unlink_or_warn(ਰਸਤਾ);

	free_worktrees(worktrees);
	ਮੁਕਤ(ਰਸਤਾ);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਖਾਲੀ validate_no_submodules(ਅਚਲ ਬਣਤਰ worktree *wt)
{
	ਬਣਤਰ ਸੂਚਕਾਂਕ_state istate = ਸੂਚਕਾਂਕ_STATE_INIT(the_repository);
	ਬਣਤਰ strbuf ਰਸਤਾ = STRBUF_INIT;
	ਪੂਰਨ i, ਮਿਲਿਆ_submodules = 0;
	ਅੱਖਰ *wt_gitdir;

	wt_gitdir = get_worktree_git_dir(wt);

	ਜੇ (is_directory(worktree_git_ਰਸਤਾ(wt, "modules"))) {
		/*
		 * There could be false positives, e.g. the "modules"
		 * directory exists but is empty. But it's a rare case and
		 * this simpler check is probably good enough for now.
		 */
		ਮਿਲਿਆ_submodules = 1;
	} ਨਹੀਂਤਾਂ ਜੇ (read_ਸੂਚਕਾਂਕ_from(&istate, worktree_git_ਰਸਤਾ(wt, "index"),
				   wt_gitdir) > 0) {
		ਲਈ (i = 0; i < istate.cache_nr; i++) {
			ਬਣਤਰ cache_ਇੰਦਰਾਜ *ce = istate.cache[i];
			ਪੂਰਨ err;

			ਜੇ (!S_ISGITLINK(ce->ce_ਬਹੁਲਕ))
				ਜਾਰੀ;

			strbuf_reset(&ਰਸਤਾ);
			strbuf_addf(&ਰਸਤਾ, "%s/%s", wt->ਰਸਤਾ, ce->name);
			ਜੇ (!is_submodule_populated_gently(ਰਸਤਾ.buf, &err))
				ਜਾਰੀ;

			ਮਿਲਿਆ_submodules = 1;
			ਰੋਕੋ;
		}
	}
	discard_ਸੂਚਕਾਂਕ(&istate);
	strbuf_release(&ਰਸਤਾ);
	ਮੁਕਤ(wt_gitdir);

	ਜੇ (ਮਿਲਿਆ_submodules)
		die(_("working trees containing submodules cannot be moved or removed"));
}

ਸਥਿਰ ਪੂਰਨ move_worktree(ਪੂਰਨ ac, ਅਚਲ ਅੱਖਰ **av, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			 ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ force = 0;
	ਬਣਤਰ option options[] = {
		OPT__FORCE(&force,
			 N_("force move even if worktree is dirty or locked"),
			 PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "relative-paths", &use_relative_paths,
			 N_("use relative paths for worktrees")),
		OPT_ਸਿਰਾ()
	};
	ਬਣਤਰ worktree **worktrees, *wt;
	ਬਣਤਰ strbuf dst = STRBUF_INIT;
	ਬਣਤਰ strbuf errmsg = STRBUF_INIT;
	ਅਚਲ ਅੱਖਰ *reason = ਨਿਰਾ;
	ਅੱਖਰ *ਰਸਤਾ;

	ac = parse_options(ac, av, ਅਗੇਤਰ, options, git_worktree_move_usage,
			   0);
	ਜੇ (ac != 2)
		usage_with_options(git_worktree_move_usage, options);

	ਰਸਤਾ = ਅਗੇਤਰ_filename(ਅਗੇਤਰ, av[1]);
	strbuf_addstr(&dst, ਰਸਤਾ);
	ਮੁਕਤ(ਰਸਤਾ);

	worktrees = get_worktrees(the_repository);
	wt = ਲੱਭੋ_worktree(worktrees, ਅਗੇਤਰ, av[0]);
	ਜੇ (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	ਜੇ (is_main_worktree(wt))
		die(_("'%s' is a main working tree"), av[0]);
	ਜੇ (is_directory(dst.buf)) {
		ਅਚਲ ਅੱਖਰ *sep = ਲੱਭੋ_ਆਖਰੀ_dir_sep(wt->ਰਸਤਾ);

		ਜੇ (!sep)
			die(_("could not figure out destination name from '%s'"),
			    wt->ਰਸਤਾ);
		strbuf_trim_trailing_dir_sep(&dst);
		strbuf_addstr(&dst, sep);
	}
	check_candidate_ਰਸਤਾ(dst.buf, force, worktrees, "move");

	validate_no_submodules(wt);

	ਜੇ (force < 2)
		reason = worktree_ਤਾਲਾ_reason(wt);
	ਜੇ (reason) {
		ਜੇ (*reason)
			die(_("cannot move a locked working tree, lock reason: %s\nuse 'move -f -f' to override or unlock first"),
			    reason);
		die(_("cannot move a locked working tree;\nuse 'move -f -f' to override or unlock first"));
	}
	ਜੇ (validate_worktree(wt, &errmsg, 0))
		die(_("validation failed, cannot move working tree: %s"),
		    errmsg.buf);
	strbuf_release(&errmsg);

	ਜੇ (rename(wt->ਰਸਤਾ, dst.buf) == -1)
		die_errno(_("failed to move '%s' to '%s'"), wt->ਰਸਤਾ, dst.buf);

	update_worktree_location(wt, dst.buf, use_relative_paths);

	strbuf_release(&dst);
	free_worktrees(worktrees);
	ਵਾਪਸ 0;
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
ਸਥਿਰ ਖਾਲੀ check_clean_worktree(ਬਣਤਰ worktree *wt,
				 ਅਚਲ ਅੱਖਰ *original_ਰਸਤਾ)
{
	ਬਣਤਰ ਬੱਚਾ_process cp;
	ਅੱਖਰ buf[1];
	ਪੂਰਨ ret;

	/*
	 * Until we sort this out, all submodules are "dirty" and
	 * will abort this function.
	 */
	validate_no_submodules(wt);

	ਬੱਚਾ_process_init(&cp);
	strvec_pushf(&cp.env, "%s=%s/.git",
		     GIT_DIR_ENVIRONMENT, wt->ਰਸਤਾ);
	strvec_pushf(&cp.env, "%s=%s",
		     GIT_WORK_ਰੁੱਖ_ENVIRONMENT, wt->ਰਸਤਾ);
	strvec_pushl(&cp.ਦਲੀਲਾਂ, "status",
		     "--porcelain", "--ignore-submodules=none",
		     ਨਿਰਾ);
	cp.git_cmd = 1;
	cp.dir = wt->ਰਸਤਾ;
	cp.ਬਾਹਰਲਾ = -1;
	ret = ਸ਼ੁਰੂ_command(&cp);
	ਜੇ (ret)
		die_errno(_("failed to run 'git status' on '%s'"),
			  original_ਰਸਤਾ);
	ret = xread(cp.ਬਾਹਰਲਾ, buf, ਆਕਾਰ(buf));
	ਜੇ (ret)
		die(_("'%s' contains modified or untracked files, use --force to delete it"),
		    original_ਰਸਤਾ);
	close(cp.ਬਾਹਰਲਾ);
	ret = ਅੰਤ_command(&cp);
	ਜੇ (ret)
		die_errno(_("failed to run 'git status' on '%s', code %d"),
			  original_ਰਸਤਾ, ret);
}

ਸਥਿਰ ਪੂਰਨ delete_git_work_ਰੁੱਖ(ਬਣਤਰ worktree *wt)
{
	ਬਣਤਰ strbuf sb = STRBUF_INIT;
	ਪੂਰਨ ret = 0;

	strbuf_addstr(&sb, wt->ਰਸਤਾ);
	ਜੇ (remove_dir_recursively(&sb, 0)) {
		error_errno(_("failed to delete '%s'"), sb.buf);
		ret = -1;
	}
	strbuf_release(&sb);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ remove_worktree(ਪੂਰਨ ac, ਅਚਲ ਅੱਖਰ **av, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			   ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ force = 0;
	ਬਣਤਰ option options[] = {
		OPT__FORCE(&force,
			 N_("force removal even if worktree is dirty or locked"),
			 PARSE_OPT_NOCOMPLETE),
		OPT_ਸਿਰਾ()
	};
	ਬਣਤਰ worktree **worktrees, *wt;
	ਬਣਤਰ strbuf errmsg = STRBUF_INIT;
	ਅਚਲ ਅੱਖਰ *reason = ਨਿਰਾ;
	ਪੂਰਨ ret = 0;

	ac = parse_options(ac, av, ਅਗੇਤਰ, options, git_worktree_remove_usage, 0);
	ਜੇ (ac != 1)
		usage_with_options(git_worktree_remove_usage, options);

	worktrees = get_worktrees(the_repository);
	wt = ਲੱਭੋ_worktree(worktrees, ਅਗੇਤਰ, av[0]);
	ਜੇ (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	ਜੇ (is_main_worktree(wt))
		die(_("'%s' is a main working tree"), av[0]);
	ਜੇ (force < 2)
		reason = worktree_ਤਾਲਾ_reason(wt);
	ਜੇ (reason) {
		ਜੇ (*reason)
			die(_("cannot remove a locked working tree, lock reason: %s\nuse 'remove -f -f' to override or unlock first"),
			    reason);
		die(_("cannot remove a locked working tree;\nuse 'remove -f -f' to override or unlock first"));
	}
	ਜੇ (validate_worktree(wt, &errmsg, WT_VALIDATE_WORKTREE_MISSING_OK))
		die(_("validation failed, cannot remove working tree: %s"),
		    errmsg.buf);
	strbuf_release(&errmsg);

	ਜੇ (file_exists(wt->ਰਸਤਾ)) {
		ਜੇ (!force)
			check_clean_worktree(wt, av[0]);

		ret |= delete_git_work_ਰੁੱਖ(wt);
	}
	/*
	 * continue on even if ret is non-zero, there's no going back
	 * from here.
	 */
	ret |= delete_git_dir(wt->id);
	delete_worktrees_dir_if_ਖਾਲੀ_ਥਾਵਾਂ();

	free_worktrees(worktrees);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਖਾਲੀ report_repair(ਪੂਰਨ iserr, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ, ਖਾਲੀ *cb_ਅੰਕੜੇ)
{
	ਜੇ (!iserr) {
		fprintf_ln(stderr, _("repair: %s: %s"), ਸੁਨੇਹਾ, ਰਸਤਾ);
	} ਨਹੀਂਤਾਂ {
		ਪੂਰਨ *exit_ਸਥਿਤੀ = (ਪੂਰਨ *)cb_ਅੰਕੜੇ;
		fprintf_ln(stderr, _("error: %s: %s"), ਸੁਨੇਹਾ, ਰਸਤਾ);
		*exit_ਸਥਿਤੀ = 1;
	}
}

ਸਥਿਰ ਪੂਰਨ repair(ਪੂਰਨ ac, ਅਚਲ ਅੱਖਰ **av, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		  ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਅਚਲ ਅੱਖਰ **p;
	ਅਚਲ ਅੱਖਰ *self[] = { ".", ਨਿਰਾ };
	ਬਣਤਰ option options[] = {
		OPT_BOOL(0, "relative-paths", &use_relative_paths,
			 N_("use relative paths for worktrees")),
		OPT_ਸਿਰਾ()
	};
	ਪੂਰਨ rc = 0;

	ac = parse_options(ac, av, ਅਗੇਤਰ, options, git_worktree_repair_usage, 0);
	p = ac > 0 ? av : self;
	ਲਈ (; *p; p++)
		repair_worktree_at_ਰਸਤਾ(the_repository, *p, report_repair,
					&rc, use_relative_paths);
	repair_worktrees(the_repository, report_repair, &rc, use_relative_paths);
	ਵਾਪਸ rc;
}

ਪੂਰਨ cmd_worktree(ਪੂਰਨ ac,
		 ਅਚਲ ਅੱਖਰ **av,
		 ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		 ਬਣਤਰ repository *repo)
{
	parse_opt_subcommand_fn *fn = ਨਿਰਾ;
	ਬਣਤਰ option options[] = {
		OPT_SUBCOMMAND("add", &fn, ਜੋੜੋ),
		OPT_SUBCOMMAND("prune", &fn, prune),
		OPT_SUBCOMMAND("list", &fn, list),
		OPT_SUBCOMMAND("lock", &fn, ਤਾਲਾ_worktree),
		OPT_SUBCOMMAND("unlock", &fn, unlock_worktree),
		OPT_SUBCOMMAND("move", &fn, move_worktree),
		OPT_SUBCOMMAND("remove", &fn, remove_worktree),
		OPT_SUBCOMMAND("repair", &fn, repair),
		OPT_ਸਿਰਾ()
	};

	repo_config(the_repository, git_worktree_config, ਨਿਰਾ);

	ਜੇ (!ਅਗੇਤਰ)
		ਅਗੇਤਰ = "";

	ac = parse_options(ac, av, ਅਗੇਤਰ, options, git_worktree_usage, 0);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_ਭਰਿਆ_ਸੂਚਕਾਂਕ = 0;

	ਵਾਪਸ fn(ac, av, ਅਗੇਤਰ, repo);
}
