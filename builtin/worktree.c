#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "abspath.h"
#അന്തര്ഭാവ "advice.h"
#അന്തര്ഭാവ "checkout.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "copy.h"
#അന്തര്ഭാവ "dir.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "hex.h"
#അന്തര്ഭാവ "object-file.h"
#അന്തര്ഭാവ "object-name.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "path.h"
#അന്തര്ഭാവ "strvec.h"
#അന്തര്ഭാവ "branch.h"
#അന്തര്ഭാവ "read-cache-ll.h"
#അന്തര്ഭാവ "refs.h"
#അന്തര്ഭാവ "remote.h"
#അന്തര്ഭാവ "run-command.h"
#അന്തര്ഭാവ "hook.h"
#അന്തര്ഭാവ "sigchain.h"
#അന്തര്ഭാവ "submodule.h"
#അന്തര്ഭാവ "utf8.h"
#അന്തര്ഭാവ "worktree.h"
#അന്തര്ഭാവ "quote.h"

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

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം git_worktree_usage[] = {
	BUILTIN_WORKTREE_ADD_USAGE,
	BUILTIN_WORKTREE_LIST_USAGE,
	BUILTIN_WORKTREE_LOCK_USAGE,
	BUILTIN_WORKTREE_MOVE_USAGE,
	BUILTIN_WORKTREE_PRUNE_USAGE,
	BUILTIN_WORKTREE_REMOVE_USAGE,
	BUILTIN_WORKTREE_REPAIR_USAGE,
	BUILTIN_WORKTREE_UNLOCK_USAGE,
	ശൂന്യ
};

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം git_worktree_add_usage[] = {
	BUILTIN_WORKTREE_ADD_USAGE,
	ശൂന്യ,
};

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം git_worktree_list_usage[] = {
	BUILTIN_WORKTREE_LIST_USAGE,
	ശൂന്യ
};

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം git_worktree_lock_usage[] = {
	BUILTIN_WORKTREE_LOCK_USAGE,
	ശൂന്യ
};

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം git_worktree_move_usage[] = {
	BUILTIN_WORKTREE_MOVE_USAGE,
	ശൂന്യ
};

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം git_worktree_prune_usage[] = {
	BUILTIN_WORKTREE_PRUNE_USAGE,
	ശൂന്യ
};

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം git_worktree_remove_usage[] = {
	BUILTIN_WORKTREE_REMOVE_USAGE,
	ശൂന്യ
};

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം git_worktree_repair_usage[] = {
	BUILTIN_WORKTREE_REPAIR_USAGE,
	ശൂന്യ
};

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം git_worktree_unlock_usage[] = {
	BUILTIN_WORKTREE_UNLOCK_USAGE,
	ശൂന്യ
};

ഘടന add_opts {
	പൂർണ്ണസംഖ്യ force;
	പൂർണ്ണസംഖ്യ detach;
	പൂർണ്ണസംഖ്യ quiet;
	പൂർണ്ണസംഖ്യ checkout;
	പൂർണ്ണസംഖ്യ orphan;
	പൂർണ്ണസംഖ്യ relative_paths;
	സ്ഥിരം അക്ഷരം *keep_locked;
};

സ്ഥിതം പൂർണ്ണസംഖ്യ show_only;
സ്ഥിതം പൂർണ്ണസംഖ്യ verbose;
സ്ഥിതം പൂർണ്ണസംഖ്യ guess_remote;
സ്ഥിതം പൂർണ്ണസംഖ്യ use_relative_paths;
സ്ഥിതം timestamp_t expire;

സ്ഥിതം പൂർണ്ണസംഖ്യ git_worktree_config(സ്ഥിരം അക്ഷരം *var, സ്ഥിരം അക്ഷരം *മൂല്യം,
			       സ്ഥിരം ഘടന config_context *ctx, ശൂന്യം *cb)
{
	എങ്കിൽ (!strcmp(var, "worktree.guessremote")) {
		guess_remote = git_config_bool(var, മൂല്യം);
		തിരികെ 0;
	} അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(var, "worktree.userelativepaths")) {
		use_relative_paths = git_config_bool(var, മൂല്യം);
		തിരികെ 0;
	}

	തിരികെ git_default_config(var, മൂല്യം, ctx, cb);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ delete_git_dir(സ്ഥിരം അക്ഷരം *id)
{
	ഘടന strbuf sb = STRBUF_INIT;
	പൂർണ്ണസംഖ്യ ret;

	repo_common_path_append(the_repository, &sb, "worktrees/%s", id);
	ret = remove_dir_recursively(&sb, 0);
	എങ്കിൽ (ret < 0 && errno == ENOTDIR)
		ret = unlink(sb.buf);
	എങ്കിൽ (ret)
		error_errno(_("failed to delete '%s'"), sb.buf);
	strbuf_release(&sb);
	തിരികെ ret;
}

സ്ഥിതം ശൂന്യം delete_worktrees_dir_if_empty(ശൂന്യം)
{
	അക്ഷരം *path = repo_git_path(the_repository, "worktrees");
	rmdir(path); /* ignore failed removal */
	സ്വതന്ത്രം(path);
}

സ്ഥിതം ശൂന്യം prune_worktree(സ്ഥിരം അക്ഷരം *id, സ്ഥിരം അക്ഷരം *reason)
{
	എങ്കിൽ (show_only || verbose)
		fprintf_ln(stderr, _("Removing %s/%s: %s"), "worktrees", id, reason);
	എങ്കിൽ (!show_only)
		delete_git_dir(id);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ prune_cmp(സ്ഥിരം ശൂന്യം *a, സ്ഥിരം ശൂന്യം *b)
{
	സ്ഥിരം ഘടന string_list_item *x = a;
	സ്ഥിരം ഘടന string_list_item *y = b;
	പൂർണ്ണസംഖ്യ c;

	എങ്കിൽ ((c = fspathcmp(x->string, y->string)))
	    തിരികെ c;
	/*
	 * paths same; prune_dupes() removes all but the first worktree entry
	 * having the same path, so sort main worktree ('util' is NULL) above
	 * linked worktrees ('util' not NULL) since main worktree can't be
	 * removed
	 */
	എങ്കിൽ (!x->util)
		തിരികെ -1;
	എങ്കിൽ (!y->util)
		തിരികെ 1;
	/* paths same; sort by .git/worktrees/<id> */
	തിരികെ strcmp(x->util, y->util);
}

സ്ഥിതം ശൂന്യം prune_dups(ഘടന string_list *l)
{
	പൂർണ്ണസംഖ്യ i;

	QSORT(l->ഇനങ്ങൾ, l->nr, prune_cmp);
	വേണ്ടി (i = 1; i < l->nr; i++) {
		എങ്കിൽ (!fspathcmp(l->ഇനങ്ങൾ[i].string, l->ഇനങ്ങൾ[i - 1].string))
			prune_worktree(l->ഇനങ്ങൾ[i].util, "duplicate entry");
	}
}

സ്ഥിതം ശൂന്യം prune_worktrees(ശൂന്യം)
{
	ഘടന strbuf reason = STRBUF_INIT;
	ഘടന strbuf main_path = STRBUF_INIT;
	ഘടന string_list kept = STRING_LIST_INIT_DUP;
	അക്ഷരം *path;
	DIR *dir;
	ഘടന dirent *d;

	path = repo_git_path(the_repository, "worktrees");
	dir = opendir(path);
	സ്വതന്ത്രം(path);
	എങ്കിൽ (!dir)
		തിരികെ;
	ആകുമ്പോൾ ((d = readdir_skip_dot_and_dotdot(dir)) != ശൂന്യ) {
		അക്ഷരം *path;
		strbuf_reset(&reason);
		എങ്കിൽ (should_prune_worktree(the_repository, d->d_name, &reason, &path, expire))
			prune_worktree(d->d_name, reason.buf);
		അല്ലെങ്കിൽ എങ്കിൽ (path)
			string_list_append_nodup(&kept, path)->util = xstrdup(d->d_name);
	}
	closedir(dir);

	strbuf_add_absolute_path(&main_path, repo_get_common_dir(the_repository));
	/* massage main worktree absolute path to match 'gitdir' content */
	strbuf_strip_suffix(&main_path, "/.");
	string_list_append_nodup(&kept, strbuf_detach(&main_path, ശൂന്യ));
	prune_dups(&kept);
	string_list_clear(&kept, 1);

	എങ്കിൽ (!show_only)
		delete_worktrees_dir_if_empty();
	strbuf_release(&reason);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ prune(പൂർണ്ണസംഖ്യ ac, സ്ഥിരം അക്ഷരം **av, സ്ഥിരം അക്ഷരം *prefix,
		 ഘടന repository *repo UNUSED)
{
	ഘടന option options[] = {
		OPT__DRY_RUN(&show_only, N_("do not remove, show only")),
		OPT__VERBOSE(&verbose, N_("report pruned working trees")),
		OPT_EXPIRY_DATE(0, "expire", &expire,
				N_("prune missing working trees older than <time>")),
		OPT_END()
	};

	expire = TIME_MAX;
	ac = parse_options(ac, av, prefix, options, git_worktree_prune_usage,
			   0);
	എങ്കിൽ (ac)
		usage_with_options(git_worktree_prune_usage, options);
	prune_worktrees();
	തിരികെ 0;
}

സ്ഥിതം അക്ഷരം *junk_work_വൃക്ഷം;
സ്ഥിതം അക്ഷരം *junk_git_dir;
സ്ഥിതം പൂർണ്ണസംഖ്യ is_junk;
സ്ഥിതം pid_t junk_pid;

സ്ഥിതം ശൂന്യം remove_junk(ശൂന്യം)
{
	ഘടന strbuf sb = STRBUF_INIT;
	എങ്കിൽ (!is_junk || getpid() != junk_pid)
		തിരികെ;
	എങ്കിൽ (junk_git_dir) {
		strbuf_addstr(&sb, junk_git_dir);
		remove_dir_recursively(&sb, 0);
		strbuf_reset(&sb);
	}
	എങ്കിൽ (junk_work_വൃക്ഷം) {
		strbuf_addstr(&sb, junk_work_വൃക്ഷം);
		remove_dir_recursively(&sb, 0);
	}
	strbuf_release(&sb);
}

സ്ഥിതം ശൂന്യം remove_junk_on_signal(പൂർണ്ണസംഖ്യ signo)
{
	remove_junk();
	sigchain_എടുക്കുക(signo);
	ഉയർത്തുക(signo);
}

സ്ഥിതം അക്ഷരം *worktree_basename_dup(സ്ഥിരം അക്ഷരം *path)
{
	സ്ഥിരം അക്ഷരം *name;
	പൂർണ്ണസംഖ്യ len;

	len = strlen(path);
	ആകുമ്പോൾ (len && is_dir_sep(path[len - 1]))
		len--;

	name = path + len;
	ആകുമ്പോൾ (name > path && !is_dir_sep(name[-1]))
		name--;

	തിരികെ xmemdupz(name, path + len - name);
}

/* check that path is viable location for worktree */
സ്ഥിതം ശൂന്യം check_candidate_path(സ്ഥിരം അക്ഷരം *path,
				 പൂർണ്ണസംഖ്യ force,
				 ഘടന worktree **worktrees,
				 സ്ഥിരം അക്ഷരം *cmd)
{
	ഘടന worktree *wt;
	പൂർണ്ണസംഖ്യ locked;

	എങ്കിൽ (file_exists(path) && !is_empty_dir(path))
		die(_("'%s' already exists"), path);

	wt = find_worktree_by_path(worktrees, path);
	എങ്കിൽ (!wt)
		തിരികെ;

	locked = !!worktree_lock_reason(wt);
	എങ്കിൽ ((!locked && force) || (locked && force > 1)) {
		എങ്കിൽ (delete_git_dir(wt->id))
		    die(_("unusable worktree destination '%s'"), path);
		തിരികെ;
	}

	എങ്കിൽ (locked)
		die(_("'%s' is a missing but locked worktree;\nuse '%s -f -f' to override, or 'unlock' and 'prune' or 'remove' to clear"), path, cmd);
	അല്ലെങ്കിൽ
		die(_("'%s' is a missing but already registered worktree;\nuse '%s -f' to override, or 'prune' or 'remove' to clear"), path, cmd);
}

സ്ഥിതം ശൂന്യം copy_sparse_checkout(സ്ഥിരം അക്ഷരം *worktree_git_dir)
{
	അക്ഷരം *from_file = repo_git_path(the_repository, "info/sparse-checkout");
	അക്ഷരം *to_file = xstrfmt("%s/info/sparse-checkout", worktree_git_dir);

	എങ്കിൽ (file_exists(from_file)) {
		എങ്കിൽ (safe_create_leading_directories(the_repository, to_file) ||
			copy_file(the_repository, to_file, from_file, 0666))
			error(_("failed to copy '%s' to '%s'; sparse-checkout may not work correctly"),
				from_file, to_file);
	}

	സ്വതന്ത്രം(from_file);
	സ്വതന്ത്രം(to_file);
}

സ്ഥിതം ശൂന്യം copy_filtered_worktree_config(സ്ഥിരം അക്ഷരം *worktree_git_dir)
{
	അക്ഷരം *from_file = repo_git_path(the_repository, "config.worktree");
	അക്ഷരം *to_file = xstrfmt("%s/config.worktree", worktree_git_dir);

	എങ്കിൽ (file_exists(from_file)) {
		ഘടന config_set cs = { { 0 } };
		പൂർണ്ണസംഖ്യ bare;

		എങ്കിൽ (safe_create_leading_directories(the_repository, to_file) ||
			copy_file(the_repository, to_file, from_file, 0666)) {
			error(_("failed to copy worktree config from '%s' to '%s'"),
				from_file, to_file);
			പോകുക worktree_copy_cleanup;
		}

		git_configset_init(&cs);
		git_configset_add_file(&cs, from_file);

		എങ്കിൽ (!git_configset_get_bool(&cs, "core.bare", &bare) &&
			bare &&
			repo_config_set_multivar_in_file_gently(the_repository,
				to_file, "core.bare", ശൂന്യ, "true", ശൂന്യ, 0))
			error(_("failed to unset '%s' in '%s'"),
				"core.bare", to_file);
		എങ്കിൽ (!git_configset_get(&cs, "core.worktree") &&
			repo_config_set_in_file_gently(the_repository, to_file,
						       "core.worktree", ശൂന്യ, ശൂന്യ))
			error(_("failed to unset '%s' in '%s'"),
				"core.worktree", to_file);

		git_configset_clear(&cs);
	}

worktree_copy_cleanup:
	സ്വതന്ത്രം(from_file);
	സ്വതന്ത്രം(to_file);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ checkout_worktree(സ്ഥിരം ഘടന add_opts *opts,
			     ഘടന strvec *child_env)
{
	ഘടന child_process cp = CHILD_PROCESS_INIT;
	cp.git_cmd = 1;
	strvec_pushl(&cp.args, "reset", "--hard", "--no-recurse-submodules", ശൂന്യ);
	എങ്കിൽ (opts->quiet)
		strvec_തള്ളുക(&cp.args, "--quiet");
	strvec_pushv(&cp.env, child_env->v);
	തിരികെ run_command(&cp);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ make_worktree_orphan(സ്ഥിരം അക്ഷരം * ref, സ്ഥിരം ഘടന add_opts *opts,
				ഘടന strvec *child_env)
{
	ഘടന strbuf symref = STRBUF_INIT;
	ഘടന child_process cp = CHILD_PROCESS_INIT;

	validate_new_branchname(ref, &symref, 0);
	strvec_pushl(&cp.args, "symbolic-ref", "HEAD", symref.buf, ശൂന്യ);
	എങ്കിൽ (opts->quiet)
		strvec_തള്ളുക(&cp.args, "--quiet");
	strvec_pushv(&cp.env, child_env->v);
	strbuf_release(&symref);
	cp.git_cmd = 1;
	തിരികെ run_command(&cp);
}

/*
 * References for worktrees are generally stored in '$GIT_DIR/worktrees/<wt_id>'.
 * But when using alternate reference directories, we want to store the worktree
 * references in '$ALTERNATE_REFERENCE_DIR/worktrees/<wt_id>'.
 *
 * Create the necessary folder structure to facilitate the same. But to ensure
 * that the former path is still considered a Git directory, add stubs.
 */
സ്ഥിതം ശൂന്യം setup_alternate_ref_dir(ഘടന worktree *wt, സ്ഥിരം അക്ഷരം *wt_git_path)
{
	ഘടന strbuf sb = STRBUF_INIT;
	അക്ഷരം *path;

	path = wt->repo->ref_storage_payload;
	എങ്കിൽ (!path)
		തിരികെ;

	എങ്കിൽ (!is_absolute_path(path))
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

സ്ഥിതം പൂർണ്ണസംഖ്യ add_worktree(സ്ഥിരം അക്ഷരം *path, സ്ഥിരം അക്ഷരം *refname,
			സ്ഥിരം ഘടന add_opts *opts)
{
	ഘടന strbuf sb_git = STRBUF_INIT, sb_repo = STRBUF_INIT;
	ഘടന strbuf sb = STRBUF_INIT;
	സ്ഥിരം അക്ഷരം *name;
	അക്ഷരം *name_to_free = ശൂന്യ;
	ഘടന strvec child_env = STRVEC_INIT;
	unsigned പൂർണ്ണസംഖ്യ counter = 0;
	പൂർണ്ണസംഖ്യ len, ret;
	ഘടന strbuf symref = STRBUF_INIT;
	ഘടന commit *commit = ശൂന്യ;
	പൂർണ്ണസംഖ്യ is_branch = 0;
	ഘടന strbuf sb_name = STRBUF_INIT;
	ഘടന worktree **worktrees, *wt = ശൂന്യ;
	ഘടന ref_store *wt_refs;
	ഘടന repo_config_മൂല്യങ്ങൾ *cfg = repo_config_മൂല്യങ്ങൾ(the_repository);

	worktrees = get_worktrees(the_repository);
	check_candidate_path(path, opts->force, worktrees, "add");
	free_worktrees(worktrees);
	worktrees = ശൂന്യ;

	/* is 'refname' a branch or commit? */
	എങ്കിൽ (!opts->detach && !check_branch_ref(the_repository, &symref, refname) &&
	    refs_ref_exists(get_main_ref_store(the_repository), symref.buf)) {
		is_branch = 1;
		എങ്കിൽ (!opts->force)
			die_if_checked_out(symref.buf, 0);
	}
	commit = lookup_commit_reference_by_name(refname);
	എങ്കിൽ (!commit && !opts->orphan)
		die(_("invalid reference: %s"), refname);

	name = name_to_free = worktree_basename_dup(path);
	എങ്കിൽ (!*name)
		die(_("invalid path '%s'"), path);
	sanitize_refname_component(name, &sb_name);
	എങ്കിൽ (!sb_name.len)
		BUG("How come '%s' becomes empty after sanitization?", name);
	name = sb_name.buf;
	repo_git_path_replace(the_repository, &sb_repo, "worktrees/%s", name);
	len = sb_repo.len;
	എങ്കിൽ (safe_create_leading_directories_const(the_repository, sb_repo.buf))
		die_errno(_("could not create leading directories of '%s'"),
			  sb_repo.buf);

	ആകുമ്പോൾ (mkdir(sb_repo.buf, 0777)) {
		counter++;
		എങ്കിൽ ((errno != EEXIST) || !counter /* overflow */)
			die_errno(_("could not create directory of '%s'"),
				  sb_repo.buf);
		strbuf_setlen(&sb_repo, len);
		strbuf_addf(&sb_repo, "%d", counter);
	}
	name = strrchr(sb_repo.buf, '/') + 1;

	junk_pid = getpid();
	atexit(remove_junk);
	sigchain_തള്ളുക_common(remove_junk_on_signal);

	junk_git_dir = xstrdup(sb_repo.buf);
	is_junk = 1;

	/*
	 * lock the incomplete repo so prune won't delete it, unlock
	 * after the preparation is over.
	 */
	strbuf_addf(&sb, "%s/locked", sb_repo.buf);
	എങ്കിൽ (opts->keep_locked)
		write_file(sb.buf, "%s", opts->keep_locked);
	അല്ലെങ്കിൽ
		write_file(sb.buf, _("initializing"));

	strbuf_addf(&sb_git, "%s/.git", path);
	എങ്കിൽ (safe_create_leading_directories_const(the_repository, sb_git.buf))
		die_errno(_("could not create leading directories of '%s'"),
			  sb_git.buf);
	junk_work_വൃക്ഷം = xstrdup(path);

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
	എങ്കിൽ (!wt) {
		ret = error(_("could not find created worktree '%s'"), name);
		പോകുക done;
	}
	setup_alternate_ref_dir(wt, sb_repo.buf);
	wt_refs = get_worktree_ref_store(wt);

	ret = ref_store_create_on_disk(wt_refs, REF_STORE_CREATE_ON_DISK_IS_WORKTREE, &sb);
	എങ്കിൽ (ret)
		പോകുക done;

	എങ്കിൽ (!is_branch && commit)
		ret = refs_update_ref(wt_refs, ശൂന്യ, "HEAD", &commit->object.oid,
				      ശൂന്യ, 0, UPDATE_REFS_സന്ദേശം_ON_ERR);
	അല്ലെങ്കിൽ
		ret = refs_update_symref(wt_refs, "HEAD", symref.buf, ശൂന്യ);
	എങ്കിൽ (ret)
		പോകുക done;

	/*
	 * If the current worktree has sparse-checkout enabled, then copy
	 * the sparse-checkout patterns from the current worktree.
	 */
	എങ്കിൽ (cfg->apply_sparse_checkout)
		copy_sparse_checkout(sb_repo.buf);

	/*
	 * If we are using worktree config, then copy all current config
	 * values from the current worktree into the new one, that way the
	 * new worktree behaves the same as this one.
	 */
	എങ്കിൽ (the_repository->repository_format_worktree_config)
		copy_filtered_worktree_config(sb_repo.buf);

	strvec_pushf(&child_env, "%s=%s", GIT_DIR_ENVIRONMENT, sb_git.buf);
	strvec_pushf(&child_env, "%s=%s", GIT_WORK_വൃക്ഷം_ENVIRONMENT, path);

	എങ്കിൽ (opts->orphan &&
	    (ret = make_worktree_orphan(refname, opts, &child_env)))
		പോകുക done;

	എങ്കിൽ (opts->checkout &&
	    (ret = checkout_worktree(opts, &child_env)))
		പോകുക done;

	is_junk = 0;
	FREE_AND_NULL(junk_work_വൃക്ഷം);
	FREE_AND_NULL(junk_git_dir);

done:
	എങ്കിൽ (ret || !opts->keep_locked) {
		strbuf_reset(&sb);
		strbuf_addf(&sb, "%s/locked", sb_repo.buf);
		unlink_or_warn(sb.buf);
	}

	/*
	 * Hook failure does not warrant worktree deletion, so run hook after
	 * is_junk is cleared, but do return appropriate code when hook fails.
	 */
	എങ്കിൽ (!ret && opts->checkout && !opts->orphan) {
		ഘടന run_hooks_opt opt = RUN_HOOKS_OPT_INIT_FORCE_SERIAL;

		strvec_pushl(&opt.env, "GIT_DIR", "GIT_WORK_TREE", ശൂന്യ);
		strvec_pushl(&opt.args,
			     oid_to_hex(null_oid(the_hash_algo)),
			     oid_to_hex(&commit->object.oid),
			     "1",
			     ശൂന്യ);
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
	സ്വതന്ത്രം(name_to_free);
	തിരികെ ret;
}

സ്ഥിതം ശൂന്യം print_preparing_worktree_വരി(പൂർണ്ണസംഖ്യ detach,
					  സ്ഥിരം അക്ഷരം *branch,
					  സ്ഥിരം അക്ഷരം *new_branch,
					  പൂർണ്ണസംഖ്യ force_new_branch)
{
	എങ്കിൽ (force_new_branch) {
		ഘടന commit *commit = lookup_commit_reference_by_name(new_branch);
		എങ്കിൽ (!commit)
			fprintf_ln(stderr, _("Preparing worktree (new branch '%s')"), new_branch);
		അല്ലെങ്കിൽ
			fprintf_ln(stderr, _("Preparing worktree (resetting branch '%s'; was at %s)"),
				  new_branch,
				  repo_find_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV));
	} അല്ലെങ്കിൽ എങ്കിൽ (new_branch) {
		fprintf_ln(stderr, _("Preparing worktree (new branch '%s')"), new_branch);
	} അല്ലെങ്കിൽ {
		ഘടന strbuf s = STRBUF_INIT;
		എങ്കിൽ (!detach && !check_branch_ref(the_repository, &s, branch) &&
		    refs_ref_exists(get_main_ref_store(the_repository), s.buf))
			fprintf_ln(stderr, _("Preparing worktree (checking out '%s')"),
				  branch);
		അല്ലെങ്കിൽ {
			ഘടന commit *commit = lookup_commit_reference_by_name(branch);
			എങ്കിൽ (!commit)
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
സ്ഥിതം പൂർണ്ണസംഖ്യ first_valid_ref(സ്ഥിരം ഘടന reference *ref UNUSED, ശൂന്യം *cb_data UNUSED)
{
	തിരികെ 1;
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
സ്ഥിതം പൂർണ്ണസംഖ്യ can_use_local_refs(സ്ഥിരം ഘടന add_opts *opts)
{
	എങ്കിൽ (refs_head_ref(get_main_ref_store(the_repository), first_valid_ref, ശൂന്യ)) {
		തിരികെ 1;
	} അല്ലെങ്കിൽ എങ്കിൽ (refs_for_each_branch_ref(get_main_ref_store(the_repository), first_valid_ref, ശൂന്യ)) {
		എങ്കിൽ (!opts->quiet)
			warning(_("HEAD points to an invalid (or orphaned) reference.\n"));
		തിരികെ 1;
	}
	തിരികെ 0;
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
സ്ഥിതം പൂർണ്ണസംഖ്യ can_use_remote_refs(സ്ഥിരം ഘടന add_opts *opts)
{
	എങ്കിൽ (!guess_remote) {
		തിരികെ 0;
	} അല്ലെങ്കിൽ എങ്കിൽ (refs_for_each_remote_ref(get_main_ref_store(the_repository), first_valid_ref, ശൂന്യ)) {
		തിരികെ 1;
	} അല്ലെങ്കിൽ എങ്കിൽ (!opts->force && remote_get(ശൂന്യ)) {
		die(_("No local or remote refs exist despite at least one remote\n"
		      "present, stopping; use 'add -f' to override or fetch a remote first"));
	}
	തിരികെ 0;
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
സ്ഥിതം പൂർണ്ണസംഖ്യ dwim_orphan(സ്ഥിരം ഘടന add_opts *opts, പൂർണ്ണസംഖ്യ opt_track, പൂർണ്ണസംഖ്യ remote)
{
	എങ്കിൽ (can_use_local_refs(opts)) {
		തിരികെ 0;
	} അല്ലെങ്കിൽ എങ്കിൽ (remote && can_use_remote_refs(opts)) {
		തിരികെ 0;
	} അല്ലെങ്കിൽ എങ്കിൽ (!opts->quiet) {
		fprintf_ln(stderr, WORKTREE_ADD_DWIM_ORPHAN_INFER_TEXT);
	}

	എങ്കിൽ (opt_track) {
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--track");
	} അല്ലെങ്കിൽ എങ്കിൽ (!opts->checkout) {
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--no-checkout");
	}
	തിരികെ 1;
}

സ്ഥിതം ശൂന്യം advise_disambiguating_remotes(സ്ഥിരം അക്ഷരം *path, സ്ഥിരം അക്ഷരം *branch,
					  സ്ഥിരം ഘടന string_list *matched_remote_names)
{
	ഘടന string_list_item *item;

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

സ്ഥിതം അക്ഷരം *dwim_branch(സ്ഥിരം ഘടന add_opts *opts, സ്ഥിരം അക്ഷരം *path, അക്ഷരം **new_branch)
{
	പൂർണ്ണസംഖ്യ branch_exists;
	അക്ഷരം *branchname = worktree_basename_dup(path);
	ഘടന strbuf ref = STRBUF_INIT;

	branch_exists = !check_branch_ref(the_repository, &ref, branchname) &&
			refs_ref_exists(get_main_ref_store(the_repository),
					ref.buf);
	strbuf_release(&ref);
	എങ്കിൽ (branch_exists)
		തിരികെ branchname;

	*new_branch = branchname;
	എങ്കിൽ (guess_remote) {
		ഘടന object_id oid;
		അക്ഷരം *remote;
		പൂർണ്ണസംഖ്യ അക്കം_matches = 0;
		ഘടന string_list matched_remote_names = STRING_LIST_INIT_DUP;

		remote = unique_tracking_name(*new_branch, &oid, &അക്കം_matches,
					      &matched_remote_names);
		എങ്കിൽ (!remote && അക്കം_matches > 1) {
			എങ്കിൽ (!opts->quiet &&
			    advice_enabled(ADVICE_CHECKOUT_AMBIGUOUS_REMOTE_BRANCH_NAME))
				advise_disambiguating_remotes(path, *new_branch,
							      &matched_remote_names);
			die(_("'%s' matched multiple (%d) remote tracking branches"),
			    *new_branch, അക്കം_matches);
		}
		string_list_clear(&matched_remote_names, 0);
		തിരികെ remote;
	}
	തിരികെ ശൂന്യ;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ add(പൂർണ്ണസംഖ്യ ac, സ്ഥിരം അക്ഷരം **av, സ്ഥിരം അക്ഷരം *prefix,
	       ഘടന repository *repo UNUSED)
{
	ഘടന add_opts opts;
	സ്ഥിരം അക്ഷരം *new_branch_force = ശൂന്യ;
	അക്ഷരം *path;
	സ്ഥിരം അക്ഷരം *branch;
	അക്ഷരം *branch_to_free = ശൂന്യ;
	അക്ഷരം *new_branch_to_free = ശൂന്യ;
	സ്ഥിരം അക്ഷരം *new_branch = ശൂന്യ;
	അക്ഷരം *opt_track = ശൂന്യ;
	സ്ഥിരം അക്ഷരം *lock_reason = ശൂന്യ;
	പൂർണ്ണസംഖ്യ keep_locked = 0;
	പൂർണ്ണസംഖ്യ used_new_branch_options;
	ഘടന option options[] = {
		OPT__FORCE(&opts.force,
			   N_("checkout <branch> even if already checked out in other worktree"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_STRING('b', ശൂന്യ, &new_branch, N_("branch"),
			   N_("create a new branch")),
		OPT_STRING('B', ശൂന്യ, &new_branch_force, N_("branch"),
			   N_("create or reset a branch")),
		OPT_BOOL(0, "orphan", &opts.orphan, N_("create unborn branch")),
		OPT_BOOL('d', "detach", &opts.detach, N_("detach HEAD at named commit")),
		OPT_BOOL(0, "checkout", &opts.checkout, N_("populate the new working tree")),
		OPT_BOOL(0, "lock", &keep_locked, N_("keep the new working tree locked")),
		OPT_STRING(0, "reason", &lock_reason, N_("string"),
			   N_("reason for locking")),
		OPT__QUIET(&opts.quiet, N_("suppress progress reporting")),
		OPT_PASSTHRU(0, "track", &opt_track, ശൂന്യ,
			     N_("set up tracking mode (see git-branch(1))"),
			     PARSE_OPT_NOARG | PARSE_OPT_OPTARG),
		OPT_BOOL(0, "guess-remote", &guess_remote,
			 N_("try to match the new branch name with a remote-tracking branch")),
		OPT_BOOL(0, "relative-paths", &opts.relative_paths,
			 N_("use relative paths for worktrees")),
		OPT_END()
	};
	പൂർണ്ണസംഖ്യ ret;

	memset(&opts, 0, വലുപ്പം(opts));
	opts.checkout = 1;
	opts.relative_paths = use_relative_paths;
	ac = parse_options(ac, av, prefix, options, git_worktree_add_usage, 0);
	എങ്കിൽ (!!opts.detach + !!new_branch + !!new_branch_force > 1)
		die(_("options '%s', '%s', and '%s' cannot be used together"), "-b", "-B", "--detach");
	എങ്കിൽ (opts.detach && opts.orphan)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--detach");
	എങ്കിൽ (opts.orphan && opt_track)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--track");
	എങ്കിൽ (opts.orphan && !opts.checkout)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--no-checkout");
	എങ്കിൽ (opts.orphan && ac == 2)
		die(_("option '%s' and commit-ish cannot be used together"),
		    "--orphan");
	എങ്കിൽ (lock_reason && !keep_locked)
		die(_("the option '%s' requires '%s'"), "--reason", "--lock");
	എങ്കിൽ (lock_reason)
		opts.keep_locked = lock_reason;
	അല്ലെങ്കിൽ എങ്കിൽ (keep_locked)
		opts.keep_locked = _("added with --lock");

	എങ്കിൽ (ac < 1 || ac > 2)
		usage_with_options(git_worktree_add_usage, options);

	path = prefix_filename(prefix, av[0]);
	branch = ac < 2 ? "HEAD" : av[1];
	used_new_branch_options = new_branch || new_branch_force;

	എങ്കിൽ (!strcmp(branch, "-"))
		branch = "@{-1}";

	എങ്കിൽ (new_branch_force) {
		ഘടന strbuf symref = STRBUF_INIT;

		new_branch = new_branch_force;

		എങ്കിൽ (!opts.force &&
		    !check_branch_ref(the_repository, &symref, new_branch) &&
		    refs_ref_exists(get_main_ref_store(the_repository), symref.buf))
			die_if_checked_out(symref.buf, 0);
		strbuf_release(&symref);
	}

	എങ്കിൽ (opts.orphan && !new_branch) {
		new_branch = new_branch_to_free = worktree_basename_dup(path);
	} അല്ലെങ്കിൽ എങ്കിൽ (opts.orphan) {
		; /* no-op */
	} അല്ലെങ്കിൽ എങ്കിൽ (opts.detach) {
		/* Check HEAD */
		എങ്കിൽ (!strcmp(branch, "HEAD"))
			can_use_local_refs(&opts);
	} അല്ലെങ്കിൽ എങ്കിൽ (ac < 2 && new_branch) {
		/* DWIM: Infer --orphan when repo has no refs. */
		opts.orphan = dwim_orphan(&opts, !!opt_track, 0);
	} അല്ലെങ്കിൽ എങ്കിൽ (ac < 2) {
		/* DWIM: Guess branch name from path. */
		അക്ഷരം *s = dwim_branch(&opts, path, &new_branch_to_free);
		എങ്കിൽ (s)
			branch = branch_to_free = s;
		new_branch = new_branch_to_free;

		/* DWIM: Infer --orphan when repo has no refs. */
		opts.orphan = (!s) && dwim_orphan(&opts, !!opt_track, 1);
	} അല്ലെങ്കിൽ എങ്കിൽ (ac == 2 && new_branch) {
		എങ്കിൽ (!strcmp(branch, "HEAD"))
			can_use_local_refs(&opts);
	} അല്ലെങ്കിൽ എങ്കിൽ (ac == 2) {
		ഘടന commit *commit;

		commit = lookup_commit_reference_by_name(branch);
		എങ്കിൽ (!commit) {
			ഘടന object_id oid;
			അക്ഷരം *remote;
			പൂർണ്ണസംഖ്യ അക്കം_matches = 0;
			ഘടന string_list matched_remote_names = STRING_LIST_INIT_DUP;

			remote = unique_tracking_name(branch, &oid, &അക്കം_matches,
						      &matched_remote_names);
			എങ്കിൽ (remote) {
				new_branch = branch;
				branch = new_branch_to_free = remote;
			} അല്ലെങ്കിൽ എങ്കിൽ (അക്കം_matches > 1) {
				എങ്കിൽ (!opts.quiet &&
				    advice_enabled(ADVICE_CHECKOUT_AMBIGUOUS_REMOTE_BRANCH_NAME))
					advise_disambiguating_remotes(path, branch,
								      &matched_remote_names);
				die(_("'%s' matched multiple (%d) remote tracking branches"),
				    branch, അക്കം_matches);
			}
			string_list_clear(&matched_remote_names, 0);
		}

		എങ്കിൽ (!strcmp(branch, "HEAD"))
			can_use_local_refs(&opts);

	}

	എങ്കിൽ (!opts.orphan && !lookup_commit_reference_by_name(branch)) {
		പൂർണ്ണസംഖ്യ attempt_hint = !opts.quiet && (ac < 2);
		എങ്കിൽ (attempt_hint && used_new_branch_options) {
			advise_if_enabled(ADVICE_WORKTREE_ADD_ORPHAN,
				WORKTREE_ADD_ORPHAN_WITH_DASH_B_HINT_TEXT,
				new_branch, path);
		} അല്ലെങ്കിൽ എങ്കിൽ (attempt_hint) {
			advise_if_enabled(ADVICE_WORKTREE_ADD_ORPHAN,
				WORKTREE_ADD_ORPHAN_NO_DASH_B_HINT_TEXT, path);
		}
		die(_("invalid reference: %s"), branch);
	}

	എങ്കിൽ (!opts.quiet)
		print_preparing_worktree_വരി(opts.detach, branch, new_branch, !!new_branch_force);

	എങ്കിൽ (opts.orphan) {
		branch = new_branch;
	} അല്ലെങ്കിൽ എങ്കിൽ (new_branch) {
		ഘടന child_process cp = CHILD_PROCESS_INIT;
		cp.git_cmd = 1;
		strvec_തള്ളുക(&cp.args, "branch");
		എങ്കിൽ (new_branch_force)
			strvec_തള്ളുക(&cp.args, "--force");
		എങ്കിൽ (opts.quiet)
			strvec_തള്ളുക(&cp.args, "--quiet");
		strvec_തള്ളുക(&cp.args, new_branch);
		strvec_തള്ളുക(&cp.args, branch);
		എങ്കിൽ (opt_track)
			strvec_തള്ളുക(&cp.args, opt_track);
		എങ്കിൽ (run_command(&cp)) {
			ret = -1;
			പോകുക cleanup;
		}
		branch = new_branch;
	} അല്ലെങ്കിൽ എങ്കിൽ (opt_track) {
		die(_("--[no-]track can only be used if a new branch is created"));
	}

	ret = add_worktree(path, branch, &opts);
cleanup:
	സ്വതന്ത്രം(path);
	സ്വതന്ത്രം(opt_track);
	സ്വതന്ത്രം(branch_to_free);
	സ്വതന്ത്രം(new_branch_to_free);
	തിരികെ ret;
}

സ്ഥിതം ശൂന്യം show_worktree_porcelain(ഘടന worktree *wt, പൂർണ്ണസംഖ്യ വരി_terminator)
{
	സ്ഥിരം അക്ഷരം *reason;

	printf("worktree %s%c", wt->path, വരി_terminator);
	എങ്കിൽ (wt->is_bare)
		printf("bare%c", വരി_terminator);
	അല്ലെങ്കിൽ {
		printf("HEAD %s%c", oid_to_hex(&wt->head_oid), വരി_terminator);
		എങ്കിൽ (wt->is_detached)
			printf("detached%c", വരി_terminator);
		അല്ലെങ്കിൽ എങ്കിൽ (wt->head_ref)
			printf("branch %s%c", wt->head_ref, വരി_terminator);
	}

	reason = worktree_lock_reason(wt);
	എങ്കിൽ (reason) {
		fputs("locked", stdout);
		എങ്കിൽ (*reason) {
			fputc(' ', stdout);
			write_name_quoted(reason, stdout, വരി_terminator);
		} അല്ലെങ്കിൽ {
			fputc(വരി_terminator, stdout);
		}
	}

	reason = worktree_prune_reason(wt, expire);
	എങ്കിൽ (reason)
		printf("prunable %s%c", reason, വരി_terminator);

	fputc(വരി_terminator, stdout);
}

ഘടന worktree_display {
	അക്ഷരം *path;
	പൂർണ്ണസംഖ്യ width;
};

സ്ഥിതം ശൂന്യം show_worktree(ഘടന worktree *wt, ഘടന worktree_display *display,
			  പൂർണ്ണസംഖ്യ path_maxwidth, പൂർണ്ണസംഖ്യ abbrev_len)
{
	ഘടന strbuf sb = STRBUF_INIT;
	സ്ഥിരം അക്ഷരം *reason;

	strbuf_addf(&sb, "%s%*s", display->path, 1 + path_maxwidth - display->width, "");
	എങ്കിൽ (wt->is_bare)
		strbuf_addstr(&sb, "(bare)");
	അല്ലെങ്കിൽ {
		strbuf_addf(&sb, "%-*s ", abbrev_len,
				repo_find_unique_abbrev(the_repository, &wt->head_oid, DEFAULT_ABBREV));
		എങ്കിൽ (wt->is_detached)
			strbuf_addstr(&sb, "(detached HEAD)");
		അല്ലെങ്കിൽ എങ്കിൽ (wt->head_ref) {
			അക്ഷരം *ref = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
								 wt->head_ref,
								 0);
			strbuf_addf(&sb, "[%s]", ref);
			സ്വതന്ത്രം(ref);
		} അല്ലെങ്കിൽ
			strbuf_addstr(&sb, "(error)");
	}

	reason = worktree_lock_reason(wt);
	എങ്കിൽ (verbose && reason && *reason)
		strbuf_addf(&sb, "\n\tlocked: %s", reason);
	അല്ലെങ്കിൽ എങ്കിൽ (reason)
		strbuf_addstr(&sb, " locked");

	reason = worktree_prune_reason(wt, expire);
	എങ്കിൽ (verbose && reason)
		strbuf_addf(&sb, "\n\tprunable: %s", reason);
	അല്ലെങ്കിൽ എങ്കിൽ (reason)
		strbuf_addstr(&sb, " prunable");

	printf("%s\n", sb.buf);
	strbuf_release(&sb);
}

സ്ഥിതം ശൂന്യം measure_widths(ഘടന worktree **wt, പൂർണ്ണസംഖ്യ *abbrev,
			   ഘടന worktree_display **d, പൂർണ്ണസംഖ്യ *maxwidth)
{
	പൂർണ്ണസംഖ്യ i, display_alloc = 0;
	ഘടന worktree_display *display = ശൂന്യ;
	ഘടന strbuf buf = STRBUF_INIT;

	വേണ്ടി (i = 0; wt[i]; i++) {
		പൂർണ്ണസംഖ്യ sha1_len;
		ALLOC_GROW(display, i + 1, display_alloc);
		quote_path(wt[i]->path, ശൂന്യ, &buf, 0);
		display[i].width = utf8_strwidth(buf.buf);
		display[i].path = strbuf_detach(&buf, ശൂന്യ);

		എങ്കിൽ (display[i].width > *maxwidth)
			*maxwidth = display[i].width;
		sha1_len = strlen(repo_find_unique_abbrev(the_repository, &wt[i]->head_oid, *abbrev));
		എങ്കിൽ (sha1_len > *abbrev)
			*abbrev = sha1_len;
	}
	*d = display;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ pathcmp(സ്ഥിരം ശൂന്യം *a_, സ്ഥിരം ശൂന്യം *b_)
{
	സ്ഥിരം ഘടന worktree *സ്ഥിരം *a = a_;
	സ്ഥിരം ഘടന worktree *സ്ഥിരം *b = b_;
	തിരികെ fspathcmp((*a)->path, (*b)->path);
}

സ്ഥിതം ശൂന്യം pathsort(ഘടന worktree **wt)
{
	പൂർണ്ണസംഖ്യ n = 0;
	ഘടന worktree **p = wt;

	ആകുമ്പോൾ (*p++)
		n++;
	QSORT(wt, n, pathcmp);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ list(പൂർണ്ണസംഖ്യ ac, സ്ഥിരം അക്ഷരം **av, സ്ഥിരം അക്ഷരം *prefix,
		ഘടന repository *repo UNUSED)
{
	പൂർണ്ണസംഖ്യ porcelain = 0;
	പൂർണ്ണസംഖ്യ വരി_terminator = '\n';

	ഘടന option options[] = {
		OPT_BOOL(0, "porcelain", &porcelain, N_("machine-readable output")),
		OPT__VERBOSE(&verbose, N_("show extended annotations and reasons, if available")),
		OPT_EXPIRY_DATE(0, "expire", &expire,
				N_("add 'prunable' annotation to missing worktrees older than <time>")),
		OPT_SET_INT('z', ശൂന്യ, &വരി_terminator,
			    N_("terminate records with a NUL character"), '\0'),
		OPT_END()
	};

	expire = TIME_MAX;
	ac = parse_options(ac, av, prefix, options, git_worktree_list_usage, 0);
	എങ്കിൽ (ac)
		usage_with_options(git_worktree_list_usage, options);
	അല്ലെങ്കിൽ എങ്കിൽ (verbose && porcelain)
		die(_("options '%s' and '%s' cannot be used together"), "--verbose", "--porcelain");
	അല്ലെങ്കിൽ എങ്കിൽ (!വരി_terminator && !porcelain)
		die(_("the option '%s' requires '%s'"), "-z", "--porcelain");
	അല്ലെങ്കിൽ {
		ഘടന worktree **worktrees = get_worktrees(the_repository);
		പൂർണ്ണസംഖ്യ path_maxwidth = 0, abbrev = DEFAULT_ABBREV, i;
		ഘടന worktree_display *display = ശൂന്യ;

		/* sort worktrees by path but keep main worktree at top */
		pathsort(worktrees + 1);

		എങ്കിൽ (!porcelain)
			measure_widths(worktrees, &abbrev,
				       &display, &path_maxwidth);

		വേണ്ടി (i = 0; worktrees[i]; i++) {
			എങ്കിൽ (porcelain)
				show_worktree_porcelain(worktrees[i],
							വരി_terminator);
			അല്ലെങ്കിൽ
				show_worktree(worktrees[i],
					      &display[i], path_maxwidth, abbrev);
		}
		വേണ്ടി (i = 0; display && worktrees[i]; i++)
			സ്വതന്ത്രം(display[i].path);
		സ്വതന്ത്രം(display);
		free_worktrees(worktrees);
	}
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ lock_worktree(പൂർണ്ണസംഖ്യ ac, സ്ഥിരം അക്ഷരം **av, സ്ഥിരം അക്ഷരം *prefix,
			 ഘടന repository *repo UNUSED)
{
	സ്ഥിരം അക്ഷരം *reason = "", *old_reason;
	ഘടന option options[] = {
		OPT_STRING(0, "reason", &reason, N_("string"),
			   N_("reason for locking")),
		OPT_END()
	};
	ഘടന worktree **worktrees, *wt;
	അക്ഷരം *path;

	ac = parse_options(ac, av, prefix, options, git_worktree_lock_usage, 0);
	എങ്കിൽ (ac != 1)
		usage_with_options(git_worktree_lock_usage, options);

	worktrees = get_worktrees(the_repository);
	wt = find_worktree(worktrees, prefix, av[0]);
	എങ്കിൽ (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	എങ്കിൽ (is_main_worktree(wt))
		die(_("The main working tree cannot be locked or unlocked"));

	old_reason = worktree_lock_reason(wt);
	എങ്കിൽ (old_reason) {
		എങ്കിൽ (*old_reason)
			die(_("'%s' is already locked, reason: %s"),
			    av[0], old_reason);
		die(_("'%s' is already locked"), av[0]);
	}

	path = repo_common_path(the_repository, "worktrees/%s/locked", wt->id);
	write_file(path, "%s", reason);

	free_worktrees(worktrees);
	സ്വതന്ത്രം(path);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ unlock_worktree(പൂർണ്ണസംഖ്യ ac, സ്ഥിരം അക്ഷരം **av, സ്ഥിരം അക്ഷരം *prefix,
			   ഘടന repository *repo UNUSED)
{
	ഘടന option options[] = {
		OPT_END()
	};
	ഘടന worktree **worktrees, *wt;
	അക്ഷരം *path;
	പൂർണ്ണസംഖ്യ ret;

	ac = parse_options(ac, av, prefix, options, git_worktree_unlock_usage, 0);
	എങ്കിൽ (ac != 1)
		usage_with_options(git_worktree_unlock_usage, options);

	worktrees = get_worktrees(the_repository);
	wt = find_worktree(worktrees, prefix, av[0]);
	എങ്കിൽ (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	എങ്കിൽ (is_main_worktree(wt))
		die(_("The main working tree cannot be locked or unlocked"));
	എങ്കിൽ (!worktree_lock_reason(wt))
		die(_("'%s' is not locked"), av[0]);

	path = repo_common_path(the_repository, "worktrees/%s/locked", wt->id);
	ret = unlink_or_warn(path);

	free_worktrees(worktrees);
	സ്വതന്ത്രം(path);
	തിരികെ ret;
}

സ്ഥിതം ശൂന്യം validate_no_submodules(സ്ഥിരം ഘടന worktree *wt)
{
	ഘടന index_state istate = INDEX_STATE_INIT(the_repository);
	ഘടന strbuf path = STRBUF_INIT;
	പൂർണ്ണസംഖ്യ i, found_submodules = 0;
	അക്ഷരം *wt_gitdir;

	wt_gitdir = get_worktree_git_dir(wt);

	എങ്കിൽ (is_directory(worktree_git_path(wt, "modules"))) {
		/*
		 * There could be false positives, e.g. the "modules"
		 * directory exists but is empty. But it's a rare case and
		 * this simpler check is probably good enough for now.
		 */
		found_submodules = 1;
	} അല്ലെങ്കിൽ എങ്കിൽ (read_index_from(&istate, worktree_git_path(wt, "index"),
				   wt_gitdir) > 0) {
		വേണ്ടി (i = 0; i < istate.cache_nr; i++) {
			ഘടന cache_entry *ce = istate.cache[i];
			പൂർണ്ണസംഖ്യ err;

			എങ്കിൽ (!S_ISGITLINK(ce->ce_mode))
				തുടരുക;

			strbuf_reset(&path);
			strbuf_addf(&path, "%s/%s", wt->path, ce->name);
			എങ്കിൽ (!is_submodule_populated_gently(path.buf, &err))
				തുടരുക;

			found_submodules = 1;
			നിർത്തുക;
		}
	}
	discard_index(&istate);
	strbuf_release(&path);
	സ്വതന്ത്രം(wt_gitdir);

	എങ്കിൽ (found_submodules)
		die(_("working trees containing submodules cannot be moved or removed"));
}

സ്ഥിതം പൂർണ്ണസംഖ്യ move_worktree(പൂർണ്ണസംഖ്യ ac, സ്ഥിരം അക്ഷരം **av, സ്ഥിരം അക്ഷരം *prefix,
			 ഘടന repository *repo UNUSED)
{
	പൂർണ്ണസംഖ്യ force = 0;
	ഘടന option options[] = {
		OPT__FORCE(&force,
			 N_("force move even if worktree is dirty or locked"),
			 PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "relative-paths", &use_relative_paths,
			 N_("use relative paths for worktrees")),
		OPT_END()
	};
	ഘടന worktree **worktrees, *wt;
	ഘടന strbuf dst = STRBUF_INIT;
	ഘടന strbuf errmsg = STRBUF_INIT;
	സ്ഥിരം അക്ഷരം *reason = ശൂന്യ;
	അക്ഷരം *path;

	ac = parse_options(ac, av, prefix, options, git_worktree_move_usage,
			   0);
	എങ്കിൽ (ac != 2)
		usage_with_options(git_worktree_move_usage, options);

	path = prefix_filename(prefix, av[1]);
	strbuf_addstr(&dst, path);
	സ്വതന്ത്രം(path);

	worktrees = get_worktrees(the_repository);
	wt = find_worktree(worktrees, prefix, av[0]);
	എങ്കിൽ (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	എങ്കിൽ (is_main_worktree(wt))
		die(_("'%s' is a main working tree"), av[0]);
	എങ്കിൽ (is_directory(dst.buf)) {
		സ്ഥിരം അക്ഷരം *sep = find_last_dir_sep(wt->path);

		എങ്കിൽ (!sep)
			die(_("could not figure out destination name from '%s'"),
			    wt->path);
		strbuf_trim_trailing_dir_sep(&dst);
		strbuf_addstr(&dst, sep);
	}
	check_candidate_path(dst.buf, force, worktrees, "move");

	validate_no_submodules(wt);

	എങ്കിൽ (force < 2)
		reason = worktree_lock_reason(wt);
	എങ്കിൽ (reason) {
		എങ്കിൽ (*reason)
			die(_("cannot move a locked working tree, lock reason: %s\nuse 'move -f -f' to override or unlock first"),
			    reason);
		die(_("cannot move a locked working tree;\nuse 'move -f -f' to override or unlock first"));
	}
	എങ്കിൽ (validate_worktree(wt, &errmsg, 0))
		die(_("validation failed, cannot move working tree: %s"),
		    errmsg.buf);
	strbuf_release(&errmsg);

	എങ്കിൽ (rename(wt->path, dst.buf) == -1)
		die_errno(_("failed to move '%s' to '%s'"), wt->path, dst.buf);

	update_worktree_location(wt, dst.buf, use_relative_paths);

	strbuf_release(&dst);
	free_worktrees(worktrees);
	തിരികെ 0;
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
സ്ഥിതം ശൂന്യം check_clean_worktree(ഘടന worktree *wt,
				 സ്ഥിരം അക്ഷരം *original_path)
{
	ഘടന child_process cp;
	അക്ഷരം buf[1];
	പൂർണ്ണസംഖ്യ ret;

	/*
	 * Until we sort this out, all submodules are "dirty" and
	 * will abort this function.
	 */
	validate_no_submodules(wt);

	child_process_init(&cp);
	strvec_pushf(&cp.env, "%s=%s/.git",
		     GIT_DIR_ENVIRONMENT, wt->path);
	strvec_pushf(&cp.env, "%s=%s",
		     GIT_WORK_വൃക്ഷം_ENVIRONMENT, wt->path);
	strvec_pushl(&cp.args, "status",
		     "--porcelain", "--ignore-submodules=none",
		     ശൂന്യ);
	cp.git_cmd = 1;
	cp.dir = wt->path;
	cp.out = -1;
	ret = start_command(&cp);
	എങ്കിൽ (ret)
		die_errno(_("failed to run 'git status' on '%s'"),
			  original_path);
	ret = xread(cp.out, buf, വലുപ്പം(buf));
	എങ്കിൽ (ret)
		die(_("'%s' contains modified or untracked files, use --force to delete it"),
		    original_path);
	close(cp.out);
	ret = finish_command(&cp);
	എങ്കിൽ (ret)
		die_errno(_("failed to run 'git status' on '%s', code %d"),
			  original_path, ret);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ delete_git_work_വൃക്ഷം(ഘടന worktree *wt)
{
	ഘടന strbuf sb = STRBUF_INIT;
	പൂർണ്ണസംഖ്യ ret = 0;

	strbuf_addstr(&sb, wt->path);
	എങ്കിൽ (remove_dir_recursively(&sb, 0)) {
		error_errno(_("failed to delete '%s'"), sb.buf);
		ret = -1;
	}
	strbuf_release(&sb);
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ remove_worktree(പൂർണ്ണസംഖ്യ ac, സ്ഥിരം അക്ഷരം **av, സ്ഥിരം അക്ഷരം *prefix,
			   ഘടന repository *repo UNUSED)
{
	പൂർണ്ണസംഖ്യ force = 0;
	ഘടന option options[] = {
		OPT__FORCE(&force,
			 N_("force removal even if worktree is dirty or locked"),
			 PARSE_OPT_NOCOMPLETE),
		OPT_END()
	};
	ഘടന worktree **worktrees, *wt;
	ഘടന strbuf errmsg = STRBUF_INIT;
	സ്ഥിരം അക്ഷരം *reason = ശൂന്യ;
	പൂർണ്ണസംഖ്യ ret = 0;

	ac = parse_options(ac, av, prefix, options, git_worktree_remove_usage, 0);
	എങ്കിൽ (ac != 1)
		usage_with_options(git_worktree_remove_usage, options);

	worktrees = get_worktrees(the_repository);
	wt = find_worktree(worktrees, prefix, av[0]);
	എങ്കിൽ (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	എങ്കിൽ (is_main_worktree(wt))
		die(_("'%s' is a main working tree"), av[0]);
	എങ്കിൽ (force < 2)
		reason = worktree_lock_reason(wt);
	എങ്കിൽ (reason) {
		എങ്കിൽ (*reason)
			die(_("cannot remove a locked working tree, lock reason: %s\nuse 'remove -f -f' to override or unlock first"),
			    reason);
		die(_("cannot remove a locked working tree;\nuse 'remove -f -f' to override or unlock first"));
	}
	എങ്കിൽ (validate_worktree(wt, &errmsg, WT_VALIDATE_WORKTREE_MISSING_OK))
		die(_("validation failed, cannot remove working tree: %s"),
		    errmsg.buf);
	strbuf_release(&errmsg);

	എങ്കിൽ (file_exists(wt->path)) {
		എങ്കിൽ (!force)
			check_clean_worktree(wt, av[0]);

		ret |= delete_git_work_വൃക്ഷം(wt);
	}
	/*
	 * continue on even if ret is non-zero, there's no going back
	 * from here.
	 */
	ret |= delete_git_dir(wt->id);
	delete_worktrees_dir_if_empty();

	free_worktrees(worktrees);
	തിരികെ ret;
}

സ്ഥിതം ശൂന്യം report_repair(പൂർണ്ണസംഖ്യ iserr, സ്ഥിരം അക്ഷരം *path, സ്ഥിരം അക്ഷരം *സന്ദേശം, ശൂന്യം *cb_data)
{
	എങ്കിൽ (!iserr) {
		fprintf_ln(stderr, _("repair: %s: %s"), സന്ദേശം, path);
	} അല്ലെങ്കിൽ {
		പൂർണ്ണസംഖ്യ *exit_status = (പൂർണ്ണസംഖ്യ *)cb_data;
		fprintf_ln(stderr, _("error: %s: %s"), സന്ദേശം, path);
		*exit_status = 1;
	}
}

സ്ഥിതം പൂർണ്ണസംഖ്യ repair(പൂർണ്ണസംഖ്യ ac, സ്ഥിരം അക്ഷരം **av, സ്ഥിരം അക്ഷരം *prefix,
		  ഘടന repository *repo UNUSED)
{
	സ്ഥിരം അക്ഷരം **p;
	സ്ഥിരം അക്ഷരം *self[] = { ".", ശൂന്യ };
	ഘടന option options[] = {
		OPT_BOOL(0, "relative-paths", &use_relative_paths,
			 N_("use relative paths for worktrees")),
		OPT_END()
	};
	പൂർണ്ണസംഖ്യ rc = 0;

	ac = parse_options(ac, av, prefix, options, git_worktree_repair_usage, 0);
	p = ac > 0 ? av : self;
	വേണ്ടി (; *p; p++)
		repair_worktree_at_path(the_repository, *p, report_repair,
					&rc, use_relative_paths);
	repair_worktrees(the_repository, report_repair, &rc, use_relative_paths);
	തിരികെ rc;
}

പൂർണ്ണസംഖ്യ cmd_worktree(പൂർണ്ണസംഖ്യ ac,
		 സ്ഥിരം അക്ഷരം **av,
		 സ്ഥിരം അക്ഷരം *prefix,
		 ഘടന repository *repo)
{
	parse_opt_subcommand_fn *fn = ശൂന്യ;
	ഘടന option options[] = {
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

	repo_config(the_repository, git_worktree_config, ശൂന്യ);

	എങ്കിൽ (!prefix)
		prefix = "";

	ac = parse_options(ac, av, prefix, options, git_worktree_usage, 0);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	തിരികെ fn(ac, av, prefix, repo);
}
