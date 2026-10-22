#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#অন্তর্ভাৱ "builtin.h"
#অন্তর্ভাৱ "abspath.h"
#অন্তর্ভাৱ "advice.h"
#অন্তর্ভাৱ "checkout.h"
#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ "copy.h"
#অন্তর্ভাৱ "dir.h"
#অন্তর্ভাৱ "environment.h"
#অন্তর্ভাৱ "gettext.h"
#অন্তর্ভাৱ "hex.h"
#অন্তর্ভাৱ "object-file.h"
#অন্তর্ভাৱ "object-name.h"
#অন্তর্ভাৱ "parse-options.h"
#অন্তর্ভাৱ "path.h"
#অন্তর্ভাৱ "strvec.h"
#অন্তর্ভাৱ "branch.h"
#অন্তর্ভাৱ "read-cache-ll.h"
#অন্তর্ভাৱ "refs.h"
#অন্তর্ভাৱ "remote.h"
#অন্তর্ভাৱ "run-command.h"
#অন্তর্ভাৱ "hook.h"
#অন্তর্ভাৱ "sigchain.h"
#অন্তর্ভাৱ "submodule.h"
#অন্তর্ভাৱ "utf8.h"
#অন্তর্ভাৱ "worktree.h"
#অন্তর্ভাৱ "quote.h"

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

স্থির ধ্রুৱ ৱর্ণ * ধ্রুৱ git_worktree_usage[] = {
	BUILTIN_WORKTREE_ADD_USAGE,
	BUILTIN_WORKTREE_LIST_USAGE,
	BUILTIN_WORKTREE_LOCK_USAGE,
	BUILTIN_WORKTREE_MOVE_USAGE,
	BUILTIN_WORKTREE_PRUNE_USAGE,
	BUILTIN_WORKTREE_REMOVE_USAGE,
	BUILTIN_WORKTREE_REPAIR_USAGE,
	BUILTIN_WORKTREE_UNLOCK_USAGE,
	রিক্ত
};

স্থির ধ্রুৱ ৱর্ণ * ধ্রুৱ git_worktree_add_usage[] = {
	BUILTIN_WORKTREE_ADD_USAGE,
	রিক্ত,
};

স্থির ধ্রুৱ ৱর্ণ * ধ্রুৱ git_worktree_list_usage[] = {
	BUILTIN_WORKTREE_LIST_USAGE,
	রিক্ত
};

স্থির ধ্রুৱ ৱর্ণ * ধ্রুৱ git_worktree_lock_usage[] = {
	BUILTIN_WORKTREE_LOCK_USAGE,
	রিক্ত
};

স্থির ধ্রুৱ ৱর্ণ * ধ্রুৱ git_worktree_move_usage[] = {
	BUILTIN_WORKTREE_MOVE_USAGE,
	রিক্ত
};

স্থির ধ্রুৱ ৱর্ণ * ধ্রুৱ git_worktree_prune_usage[] = {
	BUILTIN_WORKTREE_PRUNE_USAGE,
	রিক্ত
};

স্থির ধ্রুৱ ৱর্ণ * ধ্রুৱ git_worktree_remove_usage[] = {
	BUILTIN_WORKTREE_REMOVE_USAGE,
	রিক্ত
};

স্থির ধ্রুৱ ৱর্ণ * ধ্রুৱ git_worktree_repair_usage[] = {
	BUILTIN_WORKTREE_REPAIR_USAGE,
	রিক্ত
};

স্থির ধ্রুৱ ৱর্ণ * ধ্রুৱ git_worktree_unlock_usage[] = {
	BUILTIN_WORKTREE_UNLOCK_USAGE,
	রিক্ত
};

সংরচনা add_opts {
	পূর্ণাঙ্ক force;
	পূর্ণাঙ্ক detach;
	পূর্ণাঙ্ক quiet;
	পূর্ণাঙ্ক checkout;
	পূর্ণাঙ্ক orphan;
	পূর্ণাঙ্ক relative_paths;
	ধ্রুৱ ৱর্ণ *keep_locked;
};

স্থির পূর্ণাঙ্ক show_only;
স্থির পূর্ণাঙ্ক verbose;
স্থির পূর্ণাঙ্ক guess_remote;
স্থির পূর্ণাঙ্ক use_relative_paths;
স্থির timestamp_t expire;

স্থির পূর্ণাঙ্ক git_worktree_config(ধ্রুৱ ৱর্ণ *var, ধ্রুৱ ৱর্ণ *মান,
			       ধ্রুৱ সংরচনা config_context *ctx, শূন্য *cb)
{
	যদি (!strcmp(var, "worktree.guessremote")) {
		guess_remote = git_config_bool(var, মান);
		ঘূৰাই 0;
	} অন্যথা যদি (!strcmp(var, "worktree.userelativepaths")) {
		use_relative_paths = git_config_bool(var, মান);
		ঘূৰাই 0;
	}

	ঘূৰাই git_default_config(var, মান, ctx, cb);
}

স্থির পূর্ণাঙ্ক delete_git_dir(ধ্রুৱ ৱর্ণ *id)
{
	সংরচনা strbuf sb = STRBUF_INIT;
	পূর্ণাঙ্ক ret;

	repo_common_path_append(the_repository, &sb, "worktrees/%s", id);
	ret = remove_dir_recursively(&sb, 0);
	যদি (ret < 0 && errno == ENOTDIR)
		ret = unlink(sb.buf);
	যদি (ret)
		error_errno(_("failed to delete '%s'"), sb.buf);
	strbuf_release(&sb);
	ঘূৰাই ret;
}

স্থির শূন্য delete_worktrees_dir_if_empty(শূন্য)
{
	ৱর্ণ *path = repo_git_path(the_repository, "worktrees");
	rmdir(path); /* ignore failed removal */
	মুক্ত(path);
}

স্থির শূন্য prune_worktree(ধ্রুৱ ৱর্ণ *id, ধ্রুৱ ৱর্ণ *reason)
{
	যদি (show_only || verbose)
		fprintf_ln(stderr, _("Removing %s/%s: %s"), "worktrees", id, reason);
	যদি (!show_only)
		delete_git_dir(id);
}

স্থির পূর্ণাঙ্ক prune_cmp(ধ্রুৱ শূন্য *a, ধ্রুৱ শূন্য *b)
{
	ধ্রুৱ সংরচনা string_list_item *x = a;
	ধ্রুৱ সংরচনা string_list_item *y = b;
	পূর্ণাঙ্ক c;

	যদি ((c = fspathcmp(x->string, y->string)))
	    ঘূৰাই c;
	/*
	 * paths same; prune_dupes() removes all but the first worktree entry
	 * having the same path, so sort main worktree ('util' is NULL) above
	 * linked worktrees ('util' not NULL) since main worktree can't be
	 * removed
	 */
	যদি (!x->util)
		ঘূৰাই -1;
	যদি (!y->util)
		ঘূৰাই 1;
	/* paths same; sort by .git/worktrees/<id> */
	ঘূৰাই strcmp(x->util, y->util);
}

স্থির শূন্য prune_dups(সংরচনা string_list *l)
{
	পূর্ণাঙ্ক i;

	QSORT(l->বস্তুসমূহ, l->nr, prune_cmp);
	বাবে (i = 1; i < l->nr; i++) {
		যদি (!fspathcmp(l->বস্তুসমূহ[i].string, l->বস্তুসমূহ[i - 1].string))
			prune_worktree(l->বস্তুসমূহ[i].util, "duplicate entry");
	}
}

স্থির শূন্য prune_worktrees(শূন্য)
{
	সংরচনা strbuf reason = STRBUF_INIT;
	সংরচনা strbuf main_path = STRBUF_INIT;
	সংরচনা string_list kept = STRING_LIST_INIT_DUP;
	ৱর্ণ *path;
	DIR *dir;
	সংরচনা dirent *d;

	path = repo_git_path(the_repository, "worktrees");
	dir = opendir(path);
	মুক্ত(path);
	যদি (!dir)
		ঘূৰাই;
	যেতিয়ালৈ ((d = readdir_skip_dot_and_dotdot(dir)) != রিক্ত) {
		ৱর্ণ *path;
		strbuf_reset(&reason);
		যদি (should_prune_worktree(the_repository, d->d_name, &reason, &path, expire))
			prune_worktree(d->d_name, reason.buf);
		অন্যথা যদি (path)
			string_list_append_nodup(&kept, path)->util = xstrdup(d->d_name);
	}
	closedir(dir);

	strbuf_add_absolute_path(&main_path, repo_get_common_dir(the_repository));
	/* massage main worktree absolute path to match 'gitdir' content */
	strbuf_strip_suffix(&main_path, "/.");
	string_list_append_nodup(&kept, strbuf_detach(&main_path, রিক্ত));
	prune_dups(&kept);
	string_list_clear(&kept, 1);

	যদি (!show_only)
		delete_worktrees_dir_if_empty();
	strbuf_release(&reason);
}

স্থির পূর্ণাঙ্ক prune(পূর্ণাঙ্ক ac, ধ্রুৱ ৱর্ণ **av, ধ্রুৱ ৱর্ণ *prefix,
		 সংরচনা repository *repo UNUSED)
{
	সংরচনা option options[] = {
		OPT__DRY_RUN(&show_only, N_("do not remove, show only")),
		OPT__VERBOSE(&verbose, N_("report pruned working trees")),
		OPT_EXPIRY_DATE(0, "expire", &expire,
				N_("prune missing working trees older than <time>")),
		OPT_END()
	};

	expire = TIME_MAX;
	ac = parse_options(ac, av, prefix, options, git_worktree_prune_usage,
			   0);
	যদি (ac)
		usage_with_options(git_worktree_prune_usage, options);
	prune_worktrees();
	ঘূৰাই 0;
}

স্থির ৱর্ণ *junk_work_বৃক্ষ;
স্থির ৱর্ণ *junk_git_dir;
স্থির পূর্ণাঙ্ক is_junk;
স্থির pid_t junk_pid;

স্থির শূন্য remove_junk(শূন্য)
{
	সংরচনা strbuf sb = STRBUF_INIT;
	যদি (!is_junk || getpid() != junk_pid)
		ঘূৰাই;
	যদি (junk_git_dir) {
		strbuf_addstr(&sb, junk_git_dir);
		remove_dir_recursively(&sb, 0);
		strbuf_reset(&sb);
	}
	যদি (junk_work_বৃক্ষ) {
		strbuf_addstr(&sb, junk_work_বৃক্ষ);
		remove_dir_recursively(&sb, 0);
	}
	strbuf_release(&sb);
}

স্থির শূন্য remove_junk_on_signal(পূর্ণাঙ্ক signo)
{
	remove_junk();
	sigchain_বেৰ_কৰো(signo);
	উত্থাপয(signo);
}

স্থির ৱর্ণ *worktree_basename_dup(ধ্রুৱ ৱর্ণ *path)
{
	ধ্রুৱ ৱর্ণ *name;
	পূর্ণাঙ্ক len;

	len = strlen(path);
	যেতিয়ালৈ (len && is_dir_sep(path[len - 1]))
		len--;

	name = path + len;
	যেতিয়ালৈ (name > path && !is_dir_sep(name[-1]))
		name--;

	ঘূৰাই xmemdupz(name, path + len - name);
}

/* check that path is viable location for worktree */
স্থির শূন্য check_candidate_path(ধ্রুৱ ৱর্ণ *path,
				 পূর্ণাঙ্ক force,
				 সংরচনা worktree **worktrees,
				 ধ্রুৱ ৱর্ণ *cmd)
{
	সংরচনা worktree *wt;
	পূর্ণাঙ্ক locked;

	যদি (file_exists(path) && !is_empty_dir(path))
		die(_("'%s' already exists"), path);

	wt = find_worktree_by_path(worktrees, path);
	যদি (!wt)
		ঘূৰাই;

	locked = !!worktree_lock_reason(wt);
	যদি ((!locked && force) || (locked && force > 1)) {
		যদি (delete_git_dir(wt->id))
		    die(_("unusable worktree destination '%s'"), path);
		ঘূৰাই;
	}

	যদি (locked)
		die(_("'%s' is a missing but locked worktree;\nuse '%s -f -f' to override, or 'unlock' and 'prune' or 'remove' to clear"), path, cmd);
	অন্যথা
		die(_("'%s' is a missing but already registered worktree;\nuse '%s -f' to override, or 'prune' or 'remove' to clear"), path, cmd);
}

স্থির শূন্য copy_sparse_checkout(ধ্রুৱ ৱর্ণ *worktree_git_dir)
{
	ৱর্ণ *from_file = repo_git_path(the_repository, "info/sparse-checkout");
	ৱর্ণ *to_file = xstrfmt("%s/info/sparse-checkout", worktree_git_dir);

	যদি (file_exists(from_file)) {
		যদি (safe_create_leading_directories(the_repository, to_file) ||
			copy_file(the_repository, to_file, from_file, 0666))
			error(_("failed to copy '%s' to '%s'; sparse-checkout may not work correctly"),
				from_file, to_file);
	}

	মুক্ত(from_file);
	মুক্ত(to_file);
}

স্থির শূন্য copy_filtered_worktree_config(ধ্রুৱ ৱর্ণ *worktree_git_dir)
{
	ৱর্ণ *from_file = repo_git_path(the_repository, "config.worktree");
	ৱর্ণ *to_file = xstrfmt("%s/config.worktree", worktree_git_dir);

	যদি (file_exists(from_file)) {
		সংরচনা config_set cs = { { 0 } };
		পূর্ণাঙ্ক bare;

		যদি (safe_create_leading_directories(the_repository, to_file) ||
			copy_file(the_repository, to_file, from_file, 0666)) {
			error(_("failed to copy worktree config from '%s' to '%s'"),
				from_file, to_file);
			গচ্ছ worktree_copy_cleanup;
		}

		git_configset_init(&cs);
		git_configset_add_file(&cs, from_file);

		যদি (!git_configset_get_bool(&cs, "core.bare", &bare) &&
			bare &&
			repo_config_set_multivar_in_file_gently(the_repository,
				to_file, "core.bare", রিক্ত, "true", রিক্ত, 0))
			error(_("failed to unset '%s' in '%s'"),
				"core.bare", to_file);
		যদি (!git_configset_get(&cs, "core.worktree") &&
			repo_config_set_in_file_gently(the_repository, to_file,
						       "core.worktree", রিক্ত, রিক্ত))
			error(_("failed to unset '%s' in '%s'"),
				"core.worktree", to_file);

		git_configset_clear(&cs);
	}

worktree_copy_cleanup:
	মুক্ত(from_file);
	মুক্ত(to_file);
}

স্থির পূর্ণাঙ্ক checkout_worktree(ধ্রুৱ সংরচনা add_opts *opts,
			     সংরচনা strvec *child_env)
{
	সংরচনা child_process cp = CHILD_PROCESS_INIT;
	cp.git_cmd = 1;
	strvec_pushl(&cp.args, "reset", "--hard", "--no-recurse-submodules", রিক্ত);
	যদি (opts->quiet)
		strvec_ঠেলো(&cp.args, "--quiet");
	strvec_pushv(&cp.env, child_env->v);
	ঘূৰাই run_command(&cp);
}

স্থির পূর্ণাঙ্ক make_worktree_orphan(ধ্রুৱ ৱর্ণ * ref, ধ্রুৱ সংরচনা add_opts *opts,
				সংরচনা strvec *child_env)
{
	সংরচনা strbuf symref = STRBUF_INIT;
	সংরচনা child_process cp = CHILD_PROCESS_INIT;

	validate_new_branchname(ref, &symref, 0);
	strvec_pushl(&cp.args, "symbolic-ref", "HEAD", symref.buf, রিক্ত);
	যদি (opts->quiet)
		strvec_ঠেলো(&cp.args, "--quiet");
	strvec_pushv(&cp.env, child_env->v);
	strbuf_release(&symref);
	cp.git_cmd = 1;
	ঘূৰাই run_command(&cp);
}

/*
 * References for worktrees are generally stored in '$GIT_DIR/worktrees/<wt_id>'.
 * But when using alternate reference directories, we want to store the worktree
 * references in '$ALTERNATE_REFERENCE_DIR/worktrees/<wt_id>'.
 *
 * Create the necessary folder structure to facilitate the same. But to ensure
 * that the former path is still considered a Git directory, add stubs.
 */
স্থির শূন্য setup_alternate_ref_dir(সংরচনা worktree *wt, ধ্রুৱ ৱর্ণ *wt_git_path)
{
	সংরচনা strbuf sb = STRBUF_INIT;
	ৱর্ণ *path;

	path = wt->repo->ref_storage_payload;
	যদি (!path)
		ঘূৰাই;

	যদি (!is_absolute_path(path))
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

স্থির পূর্ণাঙ্ক add_worktree(ধ্রুৱ ৱর্ণ *path, ধ্রুৱ ৱর্ণ *refname,
			ধ্রুৱ সংরচনা add_opts *opts)
{
	সংরচনা strbuf sb_git = STRBUF_INIT, sb_repo = STRBUF_INIT;
	সংরচনা strbuf sb = STRBUF_INIT;
	ধ্রুৱ ৱর্ণ *name;
	ৱর্ণ *name_to_free = রিক্ত;
	সংরচনা strvec child_env = STRVEC_INIT;
	unsigned পূর্ণাঙ্ক counter = 0;
	পূর্ণাঙ্ক len, ret;
	সংরচনা strbuf symref = STRBUF_INIT;
	সংরচনা commit *commit = রিক্ত;
	পূর্ণাঙ্ক is_branch = 0;
	সংরচনা strbuf sb_name = STRBUF_INIT;
	সংরচনা worktree **worktrees, *wt = রিক্ত;
	সংরচনা ref_store *wt_refs;
	সংরচনা repo_config_মানসমূহ *cfg = repo_config_মানসমূহ(the_repository);

	worktrees = get_worktrees(the_repository);
	check_candidate_path(path, opts->force, worktrees, "add");
	free_worktrees(worktrees);
	worktrees = রিক্ত;

	/* is 'refname' a branch or commit? */
	যদি (!opts->detach && !check_branch_ref(the_repository, &symref, refname) &&
	    refs_ref_exists(get_main_ref_store(the_repository), symref.buf)) {
		is_branch = 1;
		যদি (!opts->force)
			die_if_checked_out(symref.buf, 0);
	}
	commit = lookup_commit_reference_by_name(refname);
	যদি (!commit && !opts->orphan)
		die(_("invalid reference: %s"), refname);

	name = name_to_free = worktree_basename_dup(path);
	যদি (!*name)
		die(_("invalid path '%s'"), path);
	sanitize_refname_component(name, &sb_name);
	যদি (!sb_name.len)
		BUG("How come '%s' becomes empty after sanitization?", name);
	name = sb_name.buf;
	repo_git_path_replace(the_repository, &sb_repo, "worktrees/%s", name);
	len = sb_repo.len;
	যদি (safe_create_leading_directories_const(the_repository, sb_repo.buf))
		die_errno(_("could not create leading directories of '%s'"),
			  sb_repo.buf);

	যেতিয়ালৈ (mkdir(sb_repo.buf, 0777)) {
		counter++;
		যদি ((errno != EEXIST) || !counter /* overflow */)
			die_errno(_("could not create directory of '%s'"),
				  sb_repo.buf);
		strbuf_setlen(&sb_repo, len);
		strbuf_addf(&sb_repo, "%d", counter);
	}
	name = strrchr(sb_repo.buf, '/') + 1;

	junk_pid = getpid();
	atexit(remove_junk);
	sigchain_ঠেলো_common(remove_junk_on_signal);

	junk_git_dir = xstrdup(sb_repo.buf);
	is_junk = 1;

	/*
	 * lock the incomplete repo so prune won't delete it, unlock
	 * after the preparation is over.
	 */
	strbuf_addf(&sb, "%s/locked", sb_repo.buf);
	যদি (opts->keep_locked)
		write_file(sb.buf, "%s", opts->keep_locked);
	অন্যথা
		write_file(sb.buf, _("initializing"));

	strbuf_addf(&sb_git, "%s/.git", path);
	যদি (safe_create_leading_directories_const(the_repository, sb_git.buf))
		die_errno(_("could not create leading directories of '%s'"),
			  sb_git.buf);
	junk_work_বৃক্ষ = xstrdup(path);

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
	যদি (!wt) {
		ret = error(_("could not find created worktree '%s'"), name);
		গচ্ছ done;
	}
	setup_alternate_ref_dir(wt, sb_repo.buf);
	wt_refs = get_worktree_ref_store(wt);

	ret = ref_store_create_on_disk(wt_refs, REF_STORE_CREATE_ON_DISK_IS_WORKTREE, &sb);
	যদি (ret)
		গচ্ছ done;

	যদি (!is_branch && commit)
		ret = refs_update_ref(wt_refs, রিক্ত, "HEAD", &commit->object.oid,
				      রিক্ত, 0, UPDATE_REFS_বাৰ্তা_ON_ERR);
	অন্যথা
		ret = refs_update_symref(wt_refs, "HEAD", symref.buf, রিক্ত);
	যদি (ret)
		গচ্ছ done;

	/*
	 * If the current worktree has sparse-checkout enabled, then copy
	 * the sparse-checkout patterns from the current worktree.
	 */
	যদি (cfg->apply_sparse_checkout)
		copy_sparse_checkout(sb_repo.buf);

	/*
	 * If we are using worktree config, then copy all current config
	 * values from the current worktree into the new one, that way the
	 * new worktree behaves the same as this one.
	 */
	যদি (the_repository->repository_format_worktree_config)
		copy_filtered_worktree_config(sb_repo.buf);

	strvec_pushf(&child_env, "%s=%s", GIT_DIR_ENVIRONMENT, sb_git.buf);
	strvec_pushf(&child_env, "%s=%s", GIT_WORK_বৃক্ষ_ENVIRONMENT, path);

	যদি (opts->orphan &&
	    (ret = make_worktree_orphan(refname, opts, &child_env)))
		গচ্ছ done;

	যদি (opts->checkout &&
	    (ret = checkout_worktree(opts, &child_env)))
		গচ্ছ done;

	is_junk = 0;
	FREE_AND_NULL(junk_work_বৃক্ষ);
	FREE_AND_NULL(junk_git_dir);

done:
	যদি (ret || !opts->keep_locked) {
		strbuf_reset(&sb);
		strbuf_addf(&sb, "%s/locked", sb_repo.buf);
		unlink_or_warn(sb.buf);
	}

	/*
	 * Hook failure does not warrant worktree deletion, so run hook after
	 * is_junk is cleared, but do return appropriate code when hook fails.
	 */
	যদি (!ret && opts->checkout && !opts->orphan) {
		সংরচনা run_hooks_opt opt = RUN_HOOKS_OPT_INIT_FORCE_SERIAL;

		strvec_pushl(&opt.env, "GIT_DIR", "GIT_WORK_TREE", রিক্ত);
		strvec_pushl(&opt.args,
			     oid_to_hex(null_oid(the_hash_algo)),
			     oid_to_hex(&commit->object.oid),
			     "1",
			     রিক্ত);
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
	মুক্ত(name_to_free);
	ঘূৰাই ret;
}

স্থির শূন্য print_preparing_worktree_সাৰি(পূর্ণাঙ্ক detach,
					  ধ্রুৱ ৱর্ণ *branch,
					  ধ্রুৱ ৱর্ণ *new_branch,
					  পূর্ণাঙ্ক force_new_branch)
{
	যদি (force_new_branch) {
		সংরচনা commit *commit = lookup_commit_reference_by_name(new_branch);
		যদি (!commit)
			fprintf_ln(stderr, _("Preparing worktree (new branch '%s')"), new_branch);
		অন্যথা
			fprintf_ln(stderr, _("Preparing worktree (resetting branch '%s'; was at %s)"),
				  new_branch,
				  repo_find_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV));
	} অন্যথা যদি (new_branch) {
		fprintf_ln(stderr, _("Preparing worktree (new branch '%s')"), new_branch);
	} অন্যথা {
		সংরচনা strbuf s = STRBUF_INIT;
		যদি (!detach && !check_branch_ref(the_repository, &s, branch) &&
		    refs_ref_exists(get_main_ref_store(the_repository), s.buf))
			fprintf_ln(stderr, _("Preparing worktree (checking out '%s')"),
				  branch);
		অন্যথা {
			সংরচনা commit *commit = lookup_commit_reference_by_name(branch);
			যদি (!commit)
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
স্থির পূর্ণাঙ্ক first_valid_ref(ধ্রুৱ সংরচনা reference *ref UNUSED, শূন্য *cb_data UNUSED)
{
	ঘূৰাই 1;
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
স্থির পূর্ণাঙ্ক can_use_local_refs(ধ্রুৱ সংরচনা add_opts *opts)
{
	যদি (refs_head_ref(get_main_ref_store(the_repository), first_valid_ref, রিক্ত)) {
		ঘূৰাই 1;
	} অন্যথা যদি (refs_for_each_branch_ref(get_main_ref_store(the_repository), first_valid_ref, রিক্ত)) {
		যদি (!opts->quiet)
			warning(_("HEAD points to an invalid (or orphaned) reference.\n"));
		ঘূৰাই 1;
	}
	ঘূৰাই 0;
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
স্থির পূর্ণাঙ্ক can_use_remote_refs(ধ্রুৱ সংরচনা add_opts *opts)
{
	যদি (!guess_remote) {
		ঘূৰাই 0;
	} অন্যথা যদি (refs_for_each_remote_ref(get_main_ref_store(the_repository), first_valid_ref, রিক্ত)) {
		ঘূৰাই 1;
	} অন্যথা যদি (!opts->force && remote_get(রিক্ত)) {
		die(_("No local or remote refs exist despite at least one remote\n"
		      "present, stopping; use 'add -f' to override or fetch a remote first"));
	}
	ঘূৰাই 0;
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
স্থির পূর্ণাঙ্ক dwim_orphan(ধ্রুৱ সংরচনা add_opts *opts, পূর্ণাঙ্ক opt_track, পূর্ণাঙ্ক remote)
{
	যদি (can_use_local_refs(opts)) {
		ঘূৰাই 0;
	} অন্যথা যদি (remote && can_use_remote_refs(opts)) {
		ঘূৰাই 0;
	} অন্যথা যদি (!opts->quiet) {
		fprintf_ln(stderr, WORKTREE_ADD_DWIM_ORPHAN_INFER_TEXT);
	}

	যদি (opt_track) {
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--track");
	} অন্যথা যদি (!opts->checkout) {
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--no-checkout");
	}
	ঘূৰাই 1;
}

স্থির শূন্য advise_disambiguating_remotes(ধ্রুৱ ৱর্ণ *path, ধ্রুৱ ৱর্ণ *branch,
					  ধ্রুৱ সংরচনা string_list *matched_remote_names)
{
	সংরচনা string_list_item *item;

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

স্থির ৱর্ণ *dwim_branch(ধ্রুৱ সংরচনা add_opts *opts, ধ্রুৱ ৱর্ণ *path, ৱর্ণ **new_branch)
{
	পূর্ণাঙ্ক branch_exists;
	ৱর্ণ *branchname = worktree_basename_dup(path);
	সংরচনা strbuf ref = STRBUF_INIT;

	branch_exists = !check_branch_ref(the_repository, &ref, branchname) &&
			refs_ref_exists(get_main_ref_store(the_repository),
					ref.buf);
	strbuf_release(&ref);
	যদি (branch_exists)
		ঘূৰাই branchname;

	*new_branch = branchname;
	যদি (guess_remote) {
		সংরচনা object_id oid;
		ৱর্ণ *remote;
		পূর্ণাঙ্ক অঙ্ক_matches = 0;
		সংরচনা string_list matched_remote_names = STRING_LIST_INIT_DUP;

		remote = unique_tracking_name(*new_branch, &oid, &অঙ্ক_matches,
					      &matched_remote_names);
		যদি (!remote && অঙ্ক_matches > 1) {
			যদি (!opts->quiet &&
			    advice_enabled(ADVICE_CHECKOUT_AMBIGUOUS_REMOTE_BRANCH_NAME))
				advise_disambiguating_remotes(path, *new_branch,
							      &matched_remote_names);
			die(_("'%s' matched multiple (%d) remote tracking branches"),
			    *new_branch, অঙ্ক_matches);
		}
		string_list_clear(&matched_remote_names, 0);
		ঘূৰাই remote;
	}
	ঘূৰাই রিক্ত;
}

স্থির পূর্ণাঙ্ক add(পূর্ণাঙ্ক ac, ধ্রুৱ ৱর্ণ **av, ধ্রুৱ ৱর্ণ *prefix,
	       সংরচনা repository *repo UNUSED)
{
	সংরচনা add_opts opts;
	ধ্রুৱ ৱর্ণ *new_branch_force = রিক্ত;
	ৱর্ণ *path;
	ধ্রুৱ ৱর্ণ *branch;
	ৱর্ণ *branch_to_free = রিক্ত;
	ৱর্ণ *new_branch_to_free = রিক্ত;
	ধ্রুৱ ৱর্ণ *new_branch = রিক্ত;
	ৱর্ণ *opt_track = রিক্ত;
	ধ্রুৱ ৱর্ণ *lock_reason = রিক্ত;
	পূর্ণাঙ্ক keep_locked = 0;
	পূর্ণাঙ্ক used_new_branch_options;
	সংরচনা option options[] = {
		OPT__FORCE(&opts.force,
			   N_("checkout <branch> even if already checked out in other worktree"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_STRING('b', রিক্ত, &new_branch, N_("branch"),
			   N_("create a new branch")),
		OPT_STRING('B', রিক্ত, &new_branch_force, N_("branch"),
			   N_("create or reset a branch")),
		OPT_BOOL(0, "orphan", &opts.orphan, N_("create unborn branch")),
		OPT_BOOL('d', "detach", &opts.detach, N_("detach HEAD at named commit")),
		OPT_BOOL(0, "checkout", &opts.checkout, N_("populate the new working tree")),
		OPT_BOOL(0, "lock", &keep_locked, N_("keep the new working tree locked")),
		OPT_STRING(0, "reason", &lock_reason, N_("string"),
			   N_("reason for locking")),
		OPT__QUIET(&opts.quiet, N_("suppress progress reporting")),
		OPT_PASSTHRU(0, "track", &opt_track, রিক্ত,
			     N_("set up tracking mode (see git-branch(1))"),
			     PARSE_OPT_NOARG | PARSE_OPT_OPTARG),
		OPT_BOOL(0, "guess-remote", &guess_remote,
			 N_("try to match the new branch name with a remote-tracking branch")),
		OPT_BOOL(0, "relative-paths", &opts.relative_paths,
			 N_("use relative paths for worktrees")),
		OPT_END()
	};
	পূর্ণাঙ্ক ret;

	memset(&opts, 0, পরিমাণ(opts));
	opts.checkout = 1;
	opts.relative_paths = use_relative_paths;
	ac = parse_options(ac, av, prefix, options, git_worktree_add_usage, 0);
	যদি (!!opts.detach + !!new_branch + !!new_branch_force > 1)
		die(_("options '%s', '%s', and '%s' cannot be used together"), "-b", "-B", "--detach");
	যদি (opts.detach && opts.orphan)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--detach");
	যদি (opts.orphan && opt_track)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--track");
	যদি (opts.orphan && !opts.checkout)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--no-checkout");
	যদি (opts.orphan && ac == 2)
		die(_("option '%s' and commit-ish cannot be used together"),
		    "--orphan");
	যদি (lock_reason && !keep_locked)
		die(_("the option '%s' requires '%s'"), "--reason", "--lock");
	যদি (lock_reason)
		opts.keep_locked = lock_reason;
	অন্যথা যদি (keep_locked)
		opts.keep_locked = _("added with --lock");

	যদি (ac < 1 || ac > 2)
		usage_with_options(git_worktree_add_usage, options);

	path = prefix_filename(prefix, av[0]);
	branch = ac < 2 ? "HEAD" : av[1];
	used_new_branch_options = new_branch || new_branch_force;

	যদি (!strcmp(branch, "-"))
		branch = "@{-1}";

	যদি (new_branch_force) {
		সংরচনা strbuf symref = STRBUF_INIT;

		new_branch = new_branch_force;

		যদি (!opts.force &&
		    !check_branch_ref(the_repository, &symref, new_branch) &&
		    refs_ref_exists(get_main_ref_store(the_repository), symref.buf))
			die_if_checked_out(symref.buf, 0);
		strbuf_release(&symref);
	}

	যদি (opts.orphan && !new_branch) {
		new_branch = new_branch_to_free = worktree_basename_dup(path);
	} অন্যথা যদি (opts.orphan) {
		; /* no-op */
	} অন্যথা যদি (opts.detach) {
		/* Check HEAD */
		যদি (!strcmp(branch, "HEAD"))
			can_use_local_refs(&opts);
	} অন্যথা যদি (ac < 2 && new_branch) {
		/* DWIM: Infer --orphan when repo has no refs. */
		opts.orphan = dwim_orphan(&opts, !!opt_track, 0);
	} অন্যথা যদি (ac < 2) {
		/* DWIM: Guess branch name from path. */
		ৱর্ণ *s = dwim_branch(&opts, path, &new_branch_to_free);
		যদি (s)
			branch = branch_to_free = s;
		new_branch = new_branch_to_free;

		/* DWIM: Infer --orphan when repo has no refs. */
		opts.orphan = (!s) && dwim_orphan(&opts, !!opt_track, 1);
	} অন্যথা যদি (ac == 2 && new_branch) {
		যদি (!strcmp(branch, "HEAD"))
			can_use_local_refs(&opts);
	} অন্যথা যদি (ac == 2) {
		সংরচনা commit *commit;

		commit = lookup_commit_reference_by_name(branch);
		যদি (!commit) {
			সংরচনা object_id oid;
			ৱর্ণ *remote;
			পূর্ণাঙ্ক অঙ্ক_matches = 0;
			সংরচনা string_list matched_remote_names = STRING_LIST_INIT_DUP;

			remote = unique_tracking_name(branch, &oid, &অঙ্ক_matches,
						      &matched_remote_names);
			যদি (remote) {
				new_branch = branch;
				branch = new_branch_to_free = remote;
			} অন্যথা যদি (অঙ্ক_matches > 1) {
				যদি (!opts.quiet &&
				    advice_enabled(ADVICE_CHECKOUT_AMBIGUOUS_REMOTE_BRANCH_NAME))
					advise_disambiguating_remotes(path, branch,
								      &matched_remote_names);
				die(_("'%s' matched multiple (%d) remote tracking branches"),
				    branch, অঙ্ক_matches);
			}
			string_list_clear(&matched_remote_names, 0);
		}

		যদি (!strcmp(branch, "HEAD"))
			can_use_local_refs(&opts);

	}

	যদি (!opts.orphan && !lookup_commit_reference_by_name(branch)) {
		পূর্ণাঙ্ক attempt_hint = !opts.quiet && (ac < 2);
		যদি (attempt_hint && used_new_branch_options) {
			advise_if_enabled(ADVICE_WORKTREE_ADD_ORPHAN,
				WORKTREE_ADD_ORPHAN_WITH_DASH_B_HINT_TEXT,
				new_branch, path);
		} অন্যথা যদি (attempt_hint) {
			advise_if_enabled(ADVICE_WORKTREE_ADD_ORPHAN,
				WORKTREE_ADD_ORPHAN_NO_DASH_B_HINT_TEXT, path);
		}
		die(_("invalid reference: %s"), branch);
	}

	যদি (!opts.quiet)
		print_preparing_worktree_সাৰি(opts.detach, branch, new_branch, !!new_branch_force);

	যদি (opts.orphan) {
		branch = new_branch;
	} অন্যথা যদি (new_branch) {
		সংরচনা child_process cp = CHILD_PROCESS_INIT;
		cp.git_cmd = 1;
		strvec_ঠেলো(&cp.args, "branch");
		যদি (new_branch_force)
			strvec_ঠেলো(&cp.args, "--force");
		যদি (opts.quiet)
			strvec_ঠেলো(&cp.args, "--quiet");
		strvec_ঠেলো(&cp.args, new_branch);
		strvec_ঠেলো(&cp.args, branch);
		যদি (opt_track)
			strvec_ঠেলো(&cp.args, opt_track);
		যদি (run_command(&cp)) {
			ret = -1;
			গচ্ছ cleanup;
		}
		branch = new_branch;
	} অন্যথা যদি (opt_track) {
		die(_("--[no-]track can only be used if a new branch is created"));
	}

	ret = add_worktree(path, branch, &opts);
cleanup:
	মুক্ত(path);
	মুক্ত(opt_track);
	মুক্ত(branch_to_free);
	মুক্ত(new_branch_to_free);
	ঘূৰাই ret;
}

স্থির শূন্য show_worktree_porcelain(সংরচনা worktree *wt, পূর্ণাঙ্ক সাৰি_terminator)
{
	ধ্রুৱ ৱর্ণ *reason;

	printf("worktree %s%c", wt->path, সাৰি_terminator);
	যদি (wt->is_bare)
		printf("bare%c", সাৰি_terminator);
	অন্যথা {
		printf("HEAD %s%c", oid_to_hex(&wt->head_oid), সাৰি_terminator);
		যদি (wt->is_detached)
			printf("detached%c", সাৰি_terminator);
		অন্যথা যদি (wt->head_ref)
			printf("branch %s%c", wt->head_ref, সাৰি_terminator);
	}

	reason = worktree_lock_reason(wt);
	যদি (reason) {
		fputs("locked", stdout);
		যদি (*reason) {
			fputc(' ', stdout);
			write_name_quoted(reason, stdout, সাৰি_terminator);
		} অন্যথা {
			fputc(সাৰি_terminator, stdout);
		}
	}

	reason = worktree_prune_reason(wt, expire);
	যদি (reason)
		printf("prunable %s%c", reason, সাৰি_terminator);

	fputc(সাৰি_terminator, stdout);
}

সংরচনা worktree_display {
	ৱর্ণ *path;
	পূর্ণাঙ্ক width;
};

স্থির শূন্য show_worktree(সংরচনা worktree *wt, সংরচনা worktree_display *display,
			  পূর্ণাঙ্ক path_maxwidth, পূর্ণাঙ্ক abbrev_len)
{
	সংরচনা strbuf sb = STRBUF_INIT;
	ধ্রুৱ ৱর্ণ *reason;

	strbuf_addf(&sb, "%s%*s", display->path, 1 + path_maxwidth - display->width, "");
	যদি (wt->is_bare)
		strbuf_addstr(&sb, "(bare)");
	অন্যথা {
		strbuf_addf(&sb, "%-*s ", abbrev_len,
				repo_find_unique_abbrev(the_repository, &wt->head_oid, DEFAULT_ABBREV));
		যদি (wt->is_detached)
			strbuf_addstr(&sb, "(detached HEAD)");
		অন্যথা যদি (wt->head_ref) {
			ৱর্ণ *ref = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
								 wt->head_ref,
								 0);
			strbuf_addf(&sb, "[%s]", ref);
			মুক্ত(ref);
		} অন্যথা
			strbuf_addstr(&sb, "(error)");
	}

	reason = worktree_lock_reason(wt);
	যদি (verbose && reason && *reason)
		strbuf_addf(&sb, "\n\tlocked: %s", reason);
	অন্যথা যদি (reason)
		strbuf_addstr(&sb, " locked");

	reason = worktree_prune_reason(wt, expire);
	যদি (verbose && reason)
		strbuf_addf(&sb, "\n\tprunable: %s", reason);
	অন্যথা যদি (reason)
		strbuf_addstr(&sb, " prunable");

	printf("%s\n", sb.buf);
	strbuf_release(&sb);
}

স্থির শূন্য measure_widths(সংরচনা worktree **wt, পূর্ণাঙ্ক *abbrev,
			   সংরচনা worktree_display **d, পূর্ণাঙ্ক *maxwidth)
{
	পূর্ণাঙ্ক i, display_alloc = 0;
	সংরচনা worktree_display *display = রিক্ত;
	সংরচনা strbuf buf = STRBUF_INIT;

	বাবে (i = 0; wt[i]; i++) {
		পূর্ণাঙ্ক sha1_len;
		ALLOC_GROW(display, i + 1, display_alloc);
		quote_path(wt[i]->path, রিক্ত, &buf, 0);
		display[i].width = utf8_strwidth(buf.buf);
		display[i].path = strbuf_detach(&buf, রিক্ত);

		যদি (display[i].width > *maxwidth)
			*maxwidth = display[i].width;
		sha1_len = strlen(repo_find_unique_abbrev(the_repository, &wt[i]->head_oid, *abbrev));
		যদি (sha1_len > *abbrev)
			*abbrev = sha1_len;
	}
	*d = display;
}

স্থির পূর্ণাঙ্ক pathcmp(ধ্রুৱ শূন্য *a_, ধ্রুৱ শূন্য *b_)
{
	ধ্রুৱ সংরচনা worktree *ধ্রুৱ *a = a_;
	ধ্রুৱ সংরচনা worktree *ধ্রুৱ *b = b_;
	ঘূৰাই fspathcmp((*a)->path, (*b)->path);
}

স্থির শূন্য pathsort(সংরচনা worktree **wt)
{
	পূর্ণাঙ্ক n = 0;
	সংরচনা worktree **p = wt;

	যেতিয়ালৈ (*p++)
		n++;
	QSORT(wt, n, pathcmp);
}

স্থির পূর্ণাঙ্ক list(পূর্ণাঙ্ক ac, ধ্রুৱ ৱর্ণ **av, ধ্রুৱ ৱর্ণ *prefix,
		সংরচনা repository *repo UNUSED)
{
	পূর্ণাঙ্ক porcelain = 0;
	পূর্ণাঙ্ক সাৰি_terminator = '\n';

	সংরচনা option options[] = {
		OPT_BOOL(0, "porcelain", &porcelain, N_("machine-readable output")),
		OPT__VERBOSE(&verbose, N_("show extended annotations and reasons, if available")),
		OPT_EXPIRY_DATE(0, "expire", &expire,
				N_("add 'prunable' annotation to missing worktrees older than <time>")),
		OPT_SET_INT('z', রিক্ত, &সাৰি_terminator,
			    N_("terminate records with a NUL character"), '\0'),
		OPT_END()
	};

	expire = TIME_MAX;
	ac = parse_options(ac, av, prefix, options, git_worktree_list_usage, 0);
	যদি (ac)
		usage_with_options(git_worktree_list_usage, options);
	অন্যথা যদি (verbose && porcelain)
		die(_("options '%s' and '%s' cannot be used together"), "--verbose", "--porcelain");
	অন্যথা যদি (!সাৰি_terminator && !porcelain)
		die(_("the option '%s' requires '%s'"), "-z", "--porcelain");
	অন্যথা {
		সংরচনা worktree **worktrees = get_worktrees(the_repository);
		পূর্ণাঙ্ক path_maxwidth = 0, abbrev = DEFAULT_ABBREV, i;
		সংরচনা worktree_display *display = রিক্ত;

		/* sort worktrees by path but keep main worktree at top */
		pathsort(worktrees + 1);

		যদি (!porcelain)
			measure_widths(worktrees, &abbrev,
				       &display, &path_maxwidth);

		বাবে (i = 0; worktrees[i]; i++) {
			যদি (porcelain)
				show_worktree_porcelain(worktrees[i],
							সাৰি_terminator);
			অন্যথা
				show_worktree(worktrees[i],
					      &display[i], path_maxwidth, abbrev);
		}
		বাবে (i = 0; display && worktrees[i]; i++)
			মুক্ত(display[i].path);
		মুক্ত(display);
		free_worktrees(worktrees);
	}
	ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক lock_worktree(পূর্ণাঙ্ক ac, ধ্রুৱ ৱর্ণ **av, ধ্রুৱ ৱর্ণ *prefix,
			 সংরচনা repository *repo UNUSED)
{
	ধ্রুৱ ৱর্ণ *reason = "", *old_reason;
	সংরচনা option options[] = {
		OPT_STRING(0, "reason", &reason, N_("string"),
			   N_("reason for locking")),
		OPT_END()
	};
	সংরচনা worktree **worktrees, *wt;
	ৱর্ণ *path;

	ac = parse_options(ac, av, prefix, options, git_worktree_lock_usage, 0);
	যদি (ac != 1)
		usage_with_options(git_worktree_lock_usage, options);

	worktrees = get_worktrees(the_repository);
	wt = find_worktree(worktrees, prefix, av[0]);
	যদি (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	যদি (is_main_worktree(wt))
		die(_("The main working tree cannot be locked or unlocked"));

	old_reason = worktree_lock_reason(wt);
	যদি (old_reason) {
		যদি (*old_reason)
			die(_("'%s' is already locked, reason: %s"),
			    av[0], old_reason);
		die(_("'%s' is already locked"), av[0]);
	}

	path = repo_common_path(the_repository, "worktrees/%s/locked", wt->id);
	write_file(path, "%s", reason);

	free_worktrees(worktrees);
	মুক্ত(path);
	ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক unlock_worktree(পূর্ণাঙ্ক ac, ধ্রুৱ ৱর্ণ **av, ধ্রুৱ ৱর্ণ *prefix,
			   সংরচনা repository *repo UNUSED)
{
	সংরচনা option options[] = {
		OPT_END()
	};
	সংরচনা worktree **worktrees, *wt;
	ৱর্ণ *path;
	পূর্ণাঙ্ক ret;

	ac = parse_options(ac, av, prefix, options, git_worktree_unlock_usage, 0);
	যদি (ac != 1)
		usage_with_options(git_worktree_unlock_usage, options);

	worktrees = get_worktrees(the_repository);
	wt = find_worktree(worktrees, prefix, av[0]);
	যদি (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	যদি (is_main_worktree(wt))
		die(_("The main working tree cannot be locked or unlocked"));
	যদি (!worktree_lock_reason(wt))
		die(_("'%s' is not locked"), av[0]);

	path = repo_common_path(the_repository, "worktrees/%s/locked", wt->id);
	ret = unlink_or_warn(path);

	free_worktrees(worktrees);
	মুক্ত(path);
	ঘূৰাই ret;
}

স্থির শূন্য validate_no_submodules(ধ্রুৱ সংরচনা worktree *wt)
{
	সংরচনা index_state istate = INDEX_STATE_INIT(the_repository);
	সংরচনা strbuf path = STRBUF_INIT;
	পূর্ণাঙ্ক i, found_submodules = 0;
	ৱর্ণ *wt_gitdir;

	wt_gitdir = get_worktree_git_dir(wt);

	যদি (is_directory(worktree_git_path(wt, "modules"))) {
		/*
		 * There could be false positives, e.g. the "modules"
		 * directory exists but is empty. But it's a rare case and
		 * this simpler check is probably good enough for now.
		 */
		found_submodules = 1;
	} অন্যথা যদি (read_index_from(&istate, worktree_git_path(wt, "index"),
				   wt_gitdir) > 0) {
		বাবে (i = 0; i < istate.cache_nr; i++) {
			সংরচনা cache_entry *ce = istate.cache[i];
			পূর্ণাঙ্ক err;

			যদি (!S_ISGITLINK(ce->ce_mode))
				অনুৱর্ত;

			strbuf_reset(&path);
			strbuf_addf(&path, "%s/%s", wt->path, ce->name);
			যদি (!is_submodule_populated_gently(path.buf, &err))
				অনুৱর্ত;

			found_submodules = 1;
			ভঙ্গ;
		}
	}
	discard_index(&istate);
	strbuf_release(&path);
	মুক্ত(wt_gitdir);

	যদি (found_submodules)
		die(_("working trees containing submodules cannot be moved or removed"));
}

স্থির পূর্ণাঙ্ক move_worktree(পূর্ণাঙ্ক ac, ধ্রুৱ ৱর্ণ **av, ধ্রুৱ ৱর্ণ *prefix,
			 সংরচনা repository *repo UNUSED)
{
	পূর্ণাঙ্ক force = 0;
	সংরচনা option options[] = {
		OPT__FORCE(&force,
			 N_("force move even if worktree is dirty or locked"),
			 PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "relative-paths", &use_relative_paths,
			 N_("use relative paths for worktrees")),
		OPT_END()
	};
	সংরচনা worktree **worktrees, *wt;
	সংরচনা strbuf dst = STRBUF_INIT;
	সংরচনা strbuf errmsg = STRBUF_INIT;
	ধ্রুৱ ৱর্ণ *reason = রিক্ত;
	ৱর্ণ *path;

	ac = parse_options(ac, av, prefix, options, git_worktree_move_usage,
			   0);
	যদি (ac != 2)
		usage_with_options(git_worktree_move_usage, options);

	path = prefix_filename(prefix, av[1]);
	strbuf_addstr(&dst, path);
	মুক্ত(path);

	worktrees = get_worktrees(the_repository);
	wt = find_worktree(worktrees, prefix, av[0]);
	যদি (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	যদি (is_main_worktree(wt))
		die(_("'%s' is a main working tree"), av[0]);
	যদি (is_directory(dst.buf)) {
		ধ্রুৱ ৱর্ণ *sep = find_last_dir_sep(wt->path);

		যদি (!sep)
			die(_("could not figure out destination name from '%s'"),
			    wt->path);
		strbuf_trim_trailing_dir_sep(&dst);
		strbuf_addstr(&dst, sep);
	}
	check_candidate_path(dst.buf, force, worktrees, "move");

	validate_no_submodules(wt);

	যদি (force < 2)
		reason = worktree_lock_reason(wt);
	যদি (reason) {
		যদি (*reason)
			die(_("cannot move a locked working tree, lock reason: %s\nuse 'move -f -f' to override or unlock first"),
			    reason);
		die(_("cannot move a locked working tree;\nuse 'move -f -f' to override or unlock first"));
	}
	যদি (validate_worktree(wt, &errmsg, 0))
		die(_("validation failed, cannot move working tree: %s"),
		    errmsg.buf);
	strbuf_release(&errmsg);

	যদি (rename(wt->path, dst.buf) == -1)
		die_errno(_("failed to move '%s' to '%s'"), wt->path, dst.buf);

	update_worktree_location(wt, dst.buf, use_relative_paths);

	strbuf_release(&dst);
	free_worktrees(worktrees);
	ঘূৰাই 0;
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
স্থির শূন্য check_clean_worktree(সংরচনা worktree *wt,
				 ধ্রুৱ ৱর্ণ *original_path)
{
	সংরচনা child_process cp;
	ৱর্ণ buf[1];
	পূর্ণাঙ্ক ret;

	/*
	 * Until we sort this out, all submodules are "dirty" and
	 * will abort this function.
	 */
	validate_no_submodules(wt);

	child_process_init(&cp);
	strvec_pushf(&cp.env, "%s=%s/.git",
		     GIT_DIR_ENVIRONMENT, wt->path);
	strvec_pushf(&cp.env, "%s=%s",
		     GIT_WORK_বৃক্ষ_ENVIRONMENT, wt->path);
	strvec_pushl(&cp.args, "status",
		     "--porcelain", "--ignore-submodules=none",
		     রিক্ত);
	cp.git_cmd = 1;
	cp.dir = wt->path;
	cp.out = -1;
	ret = start_command(&cp);
	যদি (ret)
		die_errno(_("failed to run 'git status' on '%s'"),
			  original_path);
	ret = xread(cp.out, buf, পরিমাণ(buf));
	যদি (ret)
		die(_("'%s' contains modified or untracked files, use --force to delete it"),
		    original_path);
	close(cp.out);
	ret = finish_command(&cp);
	যদি (ret)
		die_errno(_("failed to run 'git status' on '%s', code %d"),
			  original_path, ret);
}

স্থির পূর্ণাঙ্ক delete_git_work_বৃক্ষ(সংরচনা worktree *wt)
{
	সংরচনা strbuf sb = STRBUF_INIT;
	পূর্ণাঙ্ক ret = 0;

	strbuf_addstr(&sb, wt->path);
	যদি (remove_dir_recursively(&sb, 0)) {
		error_errno(_("failed to delete '%s'"), sb.buf);
		ret = -1;
	}
	strbuf_release(&sb);
	ঘূৰাই ret;
}

স্থির পূর্ণাঙ্ক remove_worktree(পূর্ণাঙ্ক ac, ধ্রুৱ ৱর্ণ **av, ধ্রুৱ ৱর্ণ *prefix,
			   সংরচনা repository *repo UNUSED)
{
	পূর্ণাঙ্ক force = 0;
	সংরচনা option options[] = {
		OPT__FORCE(&force,
			 N_("force removal even if worktree is dirty or locked"),
			 PARSE_OPT_NOCOMPLETE),
		OPT_END()
	};
	সংরচনা worktree **worktrees, *wt;
	সংরচনা strbuf errmsg = STRBUF_INIT;
	ধ্রুৱ ৱর্ণ *reason = রিক্ত;
	পূর্ণাঙ্ক ret = 0;

	ac = parse_options(ac, av, prefix, options, git_worktree_remove_usage, 0);
	যদি (ac != 1)
		usage_with_options(git_worktree_remove_usage, options);

	worktrees = get_worktrees(the_repository);
	wt = find_worktree(worktrees, prefix, av[0]);
	যদি (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	যদি (is_main_worktree(wt))
		die(_("'%s' is a main working tree"), av[0]);
	যদি (force < 2)
		reason = worktree_lock_reason(wt);
	যদি (reason) {
		যদি (*reason)
			die(_("cannot remove a locked working tree, lock reason: %s\nuse 'remove -f -f' to override or unlock first"),
			    reason);
		die(_("cannot remove a locked working tree;\nuse 'remove -f -f' to override or unlock first"));
	}
	যদি (validate_worktree(wt, &errmsg, WT_VALIDATE_WORKTREE_MISSING_OK))
		die(_("validation failed, cannot remove working tree: %s"),
		    errmsg.buf);
	strbuf_release(&errmsg);

	যদি (file_exists(wt->path)) {
		যদি (!force)
			check_clean_worktree(wt, av[0]);

		ret |= delete_git_work_বৃক্ষ(wt);
	}
	/*
	 * continue on even if ret is non-zero, there's no going back
	 * from here.
	 */
	ret |= delete_git_dir(wt->id);
	delete_worktrees_dir_if_empty();

	free_worktrees(worktrees);
	ঘূৰাই ret;
}

স্থির শূন্য report_repair(পূর্ণাঙ্ক iserr, ধ্রুৱ ৱর্ণ *path, ধ্রুৱ ৱর্ণ *বাৰ্তা, শূন্য *cb_data)
{
	যদি (!iserr) {
		fprintf_ln(stderr, _("repair: %s: %s"), বাৰ্তা, path);
	} অন্যথা {
		পূর্ণাঙ্ক *exit_status = (পূর্ণাঙ্ক *)cb_data;
		fprintf_ln(stderr, _("error: %s: %s"), বাৰ্তা, path);
		*exit_status = 1;
	}
}

স্থির পূর্ণাঙ্ক repair(পূর্ণাঙ্ক ac, ধ্রুৱ ৱর্ণ **av, ধ্রুৱ ৱর্ণ *prefix,
		  সংরচনা repository *repo UNUSED)
{
	ধ্রুৱ ৱর্ণ **p;
	ধ্রুৱ ৱর্ণ *self[] = { ".", রিক্ত };
	সংরচনা option options[] = {
		OPT_BOOL(0, "relative-paths", &use_relative_paths,
			 N_("use relative paths for worktrees")),
		OPT_END()
	};
	পূর্ণাঙ্ক rc = 0;

	ac = parse_options(ac, av, prefix, options, git_worktree_repair_usage, 0);
	p = ac > 0 ? av : self;
	বাবে (; *p; p++)
		repair_worktree_at_path(the_repository, *p, report_repair,
					&rc, use_relative_paths);
	repair_worktrees(the_repository, report_repair, &rc, use_relative_paths);
	ঘূৰাই rc;
}

পূর্ণাঙ্ক cmd_worktree(পূর্ণাঙ্ক ac,
		 ধ্রুৱ ৱর্ণ **av,
		 ধ্রুৱ ৱর্ণ *prefix,
		 সংরচনা repository *repo)
{
	parse_opt_subcommand_fn *fn = রিক্ত;
	সংরচনা option options[] = {
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

	repo_config(the_repository, git_worktree_config, রিক্ত);

	যদি (!prefix)
		prefix = "";

	ac = parse_options(ac, av, prefix, options, git_worktree_usage, 0);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	ঘূৰাই fn(ac, av, prefix, repo);
}
