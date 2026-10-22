#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ضمن "builtin.h"
#ضمن "abspath.h"
#ضمن "advice.h"
#ضمن "checkout.h"
#ضمن "config.h"
#ضمن "copy.h"
#ضمن "dir.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "hex.h"
#ضمن "object-file.h"
#ضمن "object-name.h"
#ضمن "parse-options.h"
#ضمن "path.h"
#ضمن "strvec.h"
#ضمن "branch.h"
#ضمن "read-cache-ll.h"
#ضمن "refs.h"
#ضمن "remote.h"
#ضمن "run-command.h"
#ضمن "hook.h"
#ضمن "sigchain.h"
#ضمن "submodule.h"
#ضمن "utf8.h"
#ضمن "worktree.h"
#ضمن "quote.h"

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

ساكن ثابت حرف * ثابت git_worktree_usage[] = {
	BUILTIN_WORKTREE_ADD_USAGE,
	BUILTIN_WORKTREE_LIST_USAGE,
	BUILTIN_WORKTREE_LOCK_USAGE,
	BUILTIN_WORKTREE_MOVE_USAGE,
	BUILTIN_WORKTREE_PRUNE_USAGE,
	BUILTIN_WORKTREE_REMOVE_USAGE,
	BUILTIN_WORKTREE_REPAIR_USAGE,
	BUILTIN_WORKTREE_UNLOCK_USAGE,
	عدم
};

ساكن ثابت حرف * ثابت git_worktree_add_usage[] = {
	BUILTIN_WORKTREE_ADD_USAGE,
	عدم,
};

ساكن ثابت حرف * ثابت git_worktree_list_usage[] = {
	BUILTIN_WORKTREE_LIST_USAGE,
	عدم
};

ساكن ثابت حرف * ثابت git_worktree_lock_usage[] = {
	BUILTIN_WORKTREE_LOCK_USAGE,
	عدم
};

ساكن ثابت حرف * ثابت git_worktree_move_usage[] = {
	BUILTIN_WORKTREE_MOVE_USAGE,
	عدم
};

ساكن ثابت حرف * ثابت git_worktree_prune_usage[] = {
	BUILTIN_WORKTREE_PRUNE_USAGE,
	عدم
};

ساكن ثابت حرف * ثابت git_worktree_remove_usage[] = {
	BUILTIN_WORKTREE_REMOVE_USAGE,
	عدم
};

ساكن ثابت حرف * ثابت git_worktree_repair_usage[] = {
	BUILTIN_WORKTREE_REPAIR_USAGE,
	عدم
};

ساكن ثابت حرف * ثابت git_worktree_unlock_usage[] = {
	BUILTIN_WORKTREE_UNLOCK_USAGE,
	عدم
};

بنية add_opts {
	صحيح force;
	صحيح detach;
	صحيح quiet;
	صحيح checkout;
	صحيح orphan;
	صحيح relative_paths;
	ثابت حرف *keep_locked;
};

ساكن صحيح show_only;
ساكن صحيح verbose;
ساكن صحيح guess_remote;
ساكن صحيح use_relative_paths;
ساكن timestamp_t expire;

ساكن صحيح git_worktree_config(ثابت حرف *var, ثابت حرف *value,
			       ثابت بنية config_context *ctx, فراغ *cb)
{
	ܐܢ (!strcmp(var, "worktree.guessremote")) {
		guess_remote = git_config_bool(var, value);
		ܦܢܐ 0;
	} ܐܚܪܢܐ ܐܢ (!strcmp(var, "worktree.userelativepaths")) {
		use_relative_paths = git_config_bool(var, value);
		ܦܢܐ 0;
	}

	ܦܢܐ git_default_config(var, value, ctx, cb);
}

ساكن صحيح delete_git_dir(ثابت حرف *id)
{
	بنية strbuf sb = STRBUF_INIT;
	صحيح ret;

	repo_common_path_append(the_repository, &sb, "worktrees/%s", id);
	ret = remove_dir_recursively(&sb, 0);
	ܐܢ (ret < 0 && errno == ENOTDIR)
		ret = unlink(sb.buf);
	ܐܢ (ret)
		error_errno(_("failed to delete '%s'"), sb.buf);
	strbuf_release(&sb);
	ܦܢܐ ret;
}

ساكن فراغ delete_worktrees_dir_if_empty(فراغ)
{
	حرف *path = repo_git_path(the_repository, "worktrees");
	rmdir(path); /* ignore failed removal */
	حرر(path);
}

ساكن فراغ prune_worktree(ثابت حرف *id, ثابت حرف *reason)
{
	ܐܢ (show_only || verbose)
		fprintf_ln(stderr, _("Removing %s/%s: %s"), "worktrees", id, reason);
	ܐܢ (!show_only)
		delete_git_dir(id);
}

ساكن صحيح prune_cmp(ثابت فراغ *a, ثابت فراغ *b)
{
	ثابت بنية string_list_item *x = a;
	ثابت بنية string_list_item *y = b;
	صحيح c;

	ܐܢ ((c = fspathcmp(x->string, y->string)))
	    ܦܢܐ c;
	/*
	 * paths same; prune_dupes() removes all but the first worktree entry
	 * having the same path, so sort main worktree ('util' is NULL) above
	 * linked worktrees ('util' not NULL) since main worktree can't be
	 * removed
	 */
	ܐܢ (!x->util)
		ܦܢܐ -1;
	ܐܢ (!y->util)
		ܦܢܐ 1;
	/* paths same; sort by .git/worktrees/<id> */
	ܦܢܐ strcmp(x->util, y->util);
}

ساكن فراغ prune_dups(بنية string_list *l)
{
	صحيح i;

	QSORT(l->items, l->nr, prune_cmp);
	لكل (i = 1; i < l->nr; i++) {
		ܐܢ (!fspathcmp(l->items[i].string, l->items[i - 1].string))
			prune_worktree(l->items[i].util, "duplicate entry");
	}
}

ساكن فراغ prune_worktrees(فراغ)
{
	بنية strbuf reason = STRBUF_INIT;
	بنية strbuf main_path = STRBUF_INIT;
	بنية string_list kept = STRING_LIST_INIT_DUP;
	حرف *path;
	DIR *dir;
	بنية dirent *d;

	path = repo_git_path(the_repository, "worktrees");
	dir = opendir(path);
	حرر(path);
	ܐܢ (!dir)
		ܦܢܐ;
	ܟܡܐ ((d = readdir_skip_dot_and_dotdot(dir)) != عدم) {
		حرف *path;
		strbuf_reset(&reason);
		ܐܢ (should_prune_worktree(the_repository, d->d_name, &reason, &path, expire))
			prune_worktree(d->d_name, reason.buf);
		ܐܚܪܢܐ ܐܢ (path)
			string_list_append_nodup(&kept, path)->util = xstrdup(d->d_name);
	}
	closedir(dir);

	strbuf_add_absolute_path(&main_path, repo_get_common_dir(the_repository));
	/* massage main worktree absolute path to match 'gitdir' content */
	strbuf_strip_suffix(&main_path, "/.");
	string_list_append_nodup(&kept, strbuf_detach(&main_path, عدم));
	prune_dups(&kept);
	string_list_clear(&kept, 1);

	ܐܢ (!show_only)
		delete_worktrees_dir_if_empty();
	strbuf_release(&reason);
}

ساكن صحيح prune(صحيح ac, ثابت حرف **av, ثابت حرف *prefix,
		 بنية repository *repo UNUSED)
{
	بنية option options[] = {
		OPT__DRY_RUN(&show_only, N_("do not remove, show only")),
		OPT__VERBOSE(&verbose, N_("report pruned working trees")),
		OPT_EXPIRY_DATE(0, "expire", &expire,
				N_("prune missing working trees older than <time>")),
		OPT_END()
	};

	expire = TIME_MAX;
	ac = parse_options(ac, av, prefix, options, git_worktree_prune_usage,
			   0);
	ܐܢ (ac)
		usage_with_options(git_worktree_prune_usage, options);
	prune_worktrees();
	ܦܢܐ 0;
}

ساكن حرف *junk_work_tree;
ساكن حرف *junk_git_dir;
ساكن صحيح is_junk;
ساكن pid_t junk_pid;

ساكن فراغ remove_junk(فراغ)
{
	بنية strbuf sb = STRBUF_INIT;
	ܐܢ (!is_junk || getpid() != junk_pid)
		ܦܢܐ;
	ܐܢ (junk_git_dir) {
		strbuf_addstr(&sb, junk_git_dir);
		remove_dir_recursively(&sb, 0);
		strbuf_reset(&sb);
	}
	ܐܢ (junk_work_tree) {
		strbuf_addstr(&sb, junk_work_tree);
		remove_dir_recursively(&sb, 0);
	}
	strbuf_release(&sb);
}

ساكن فراغ remove_junk_on_signal(صحيح signo)
{
	remove_junk();
	sigchain_pop(signo);
	ارفع(signo);
}

ساكن حرف *worktree_basename_dup(ثابت حرف *path)
{
	ثابت حرف *name;
	صحيح len;

	len = strlen(path);
	ܟܡܐ (len && is_dir_sep(path[len - 1]))
		len--;

	name = path + len;
	ܟܡܐ (name > path && !is_dir_sep(name[-1]))
		name--;

	ܦܢܐ xmemdupz(name, path + len - name);
}

/* check that path is viable location for worktree */
ساكن فراغ check_candidate_path(ثابت حرف *path,
				 صحيح force,
				 بنية worktree **worktrees,
				 ثابت حرف *cmd)
{
	بنية worktree *wt;
	صحيح locked;

	ܐܢ (file_exists(path) && !is_empty_dir(path))
		die(_("'%s' already exists"), path);

	wt = find_worktree_by_path(worktrees, path);
	ܐܢ (!wt)
		ܦܢܐ;

	locked = !!worktree_lock_reason(wt);
	ܐܢ ((!locked && force) || (locked && force > 1)) {
		ܐܢ (delete_git_dir(wt->id))
		    die(_("unusable worktree destination '%s'"), path);
		ܦܢܐ;
	}

	ܐܢ (locked)
		die(_("'%s' is a missing but locked worktree;\nuse '%s -f -f' to override, or 'unlock' and 'prune' or 'remove' to clear"), path, cmd);
	ܐܚܪܢܐ
		die(_("'%s' is a missing but already registered worktree;\nuse '%s -f' to override, or 'prune' or 'remove' to clear"), path, cmd);
}

ساكن فراغ copy_sparse_checkout(ثابت حرف *worktree_git_dir)
{
	حرف *from_file = repo_git_path(the_repository, "info/sparse-checkout");
	حرف *to_file = xstrfmt("%s/info/sparse-checkout", worktree_git_dir);

	ܐܢ (file_exists(from_file)) {
		ܐܢ (safe_create_leading_directories(the_repository, to_file) ||
			copy_file(the_repository, to_file, from_file, 0666))
			error(_("failed to copy '%s' to '%s'; sparse-checkout may not work correctly"),
				from_file, to_file);
	}

	حرر(from_file);
	حرر(to_file);
}

ساكن فراغ copy_filtered_worktree_config(ثابت حرف *worktree_git_dir)
{
	حرف *from_file = repo_git_path(the_repository, "config.worktree");
	حرف *to_file = xstrfmt("%s/config.worktree", worktree_git_dir);

	ܐܢ (file_exists(from_file)) {
		بنية config_set cs = { { 0 } };
		صحيح bare;

		ܐܢ (safe_create_leading_directories(the_repository, to_file) ||
			copy_file(the_repository, to_file, from_file, 0666)) {
			error(_("failed to copy worktree config from '%s' to '%s'"),
				from_file, to_file);
			اذهب worktree_copy_cleanup;
		}

		git_configset_init(&cs);
		git_configset_add_file(&cs, from_file);

		ܐܢ (!git_configset_get_bool(&cs, "core.bare", &bare) &&
			bare &&
			repo_config_set_multivar_in_file_gently(the_repository,
				to_file, "core.bare", عدم, "true", عدم, 0))
			error(_("failed to unset '%s' in '%s'"),
				"core.bare", to_file);
		ܐܢ (!git_configset_get(&cs, "core.worktree") &&
			repo_config_set_in_file_gently(the_repository, to_file,
						       "core.worktree", عدم, عدم))
			error(_("failed to unset '%s' in '%s'"),
				"core.worktree", to_file);

		git_configset_clear(&cs);
	}

worktree_copy_cleanup:
	حرر(from_file);
	حرر(to_file);
}

ساكن صحيح checkout_worktree(ثابت بنية add_opts *opts,
			     بنية strvec *child_env)
{
	بنية child_process cp = CHILD_PROCESS_INIT;
	cp.git_cmd = 1;
	strvec_pushl(&cp.args, "reset", "--hard", "--no-recurse-submodules", عدم);
	ܐܢ (opts->quiet)
		strvec_push(&cp.args, "--quiet");
	strvec_pushv(&cp.env, child_env->v);
	ܦܢܐ run_command(&cp);
}

ساكن صحيح make_worktree_orphan(ثابت حرف * ref, ثابت بنية add_opts *opts,
				بنية strvec *child_env)
{
	بنية strbuf symref = STRBUF_INIT;
	بنية child_process cp = CHILD_PROCESS_INIT;

	validate_new_branchname(ref, &symref, 0);
	strvec_pushl(&cp.args, "symbolic-ref", "HEAD", symref.buf, عدم);
	ܐܢ (opts->quiet)
		strvec_push(&cp.args, "--quiet");
	strvec_pushv(&cp.env, child_env->v);
	strbuf_release(&symref);
	cp.git_cmd = 1;
	ܦܢܐ run_command(&cp);
}

/*
 * References for worktrees are generally stored in '$GIT_DIR/worktrees/<wt_id>'.
 * But when using alternate reference directories, we want to store the worktree
 * references in '$ALTERNATE_REFERENCE_DIR/worktrees/<wt_id>'.
 *
 * Create the necessary folder structure to facilitate the same. But to ensure
 * that the former path is still considered a Git directory, add stubs.
 */
ساكن فراغ setup_alternate_ref_dir(بنية worktree *wt, ثابت حرف *wt_git_path)
{
	بنية strbuf sb = STRBUF_INIT;
	حرف *path;

	path = wt->repo->ref_storage_payload;
	ܐܢ (!path)
		ܦܢܐ;

	ܐܢ (!is_absolute_path(path))
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

ساكن صحيح add_worktree(ثابت حرف *path, ثابت حرف *refname,
			ثابت بنية add_opts *opts)
{
	بنية strbuf sb_git = STRBUF_INIT, sb_repo = STRBUF_INIT;
	بنية strbuf sb = STRBUF_INIT;
	ثابت حرف *name;
	حرف *name_to_free = عدم;
	بنية strvec child_env = STRVEC_INIT;
	unsigned صحيح counter = 0;
	صحيح len, ret;
	بنية strbuf symref = STRBUF_INIT;
	بنية commit *commit = عدم;
	صحيح is_branch = 0;
	بنية strbuf sb_name = STRBUF_INIT;
	بنية worktree **worktrees, *wt = عدم;
	بنية ref_store *wt_refs;
	بنية repo_config_values *cfg = repo_config_values(the_repository);

	worktrees = get_worktrees(the_repository);
	check_candidate_path(path, opts->force, worktrees, "add");
	free_worktrees(worktrees);
	worktrees = عدم;

	/* is 'refname' a branch or commit? */
	ܐܢ (!opts->detach && !check_branch_ref(the_repository, &symref, refname) &&
	    refs_ref_exists(get_main_ref_store(the_repository), symref.buf)) {
		is_branch = 1;
		ܐܢ (!opts->force)
			die_if_checked_out(symref.buf, 0);
	}
	commit = lookup_commit_reference_by_name(refname);
	ܐܢ (!commit && !opts->orphan)
		die(_("invalid reference: %s"), refname);

	name = name_to_free = worktree_basename_dup(path);
	ܐܢ (!*name)
		die(_("invalid path '%s'"), path);
	sanitize_refname_component(name, &sb_name);
	ܐܢ (!sb_name.len)
		BUG("How come '%s' becomes empty after sanitization?", name);
	name = sb_name.buf;
	repo_git_path_replace(the_repository, &sb_repo, "worktrees/%s", name);
	len = sb_repo.len;
	ܐܢ (safe_create_leading_directories_const(the_repository, sb_repo.buf))
		die_errno(_("could not create leading directories of '%s'"),
			  sb_repo.buf);

	ܟܡܐ (mkdir(sb_repo.buf, 0777)) {
		counter++;
		ܐܢ ((errno != EEXIST) || !counter /* overflow */)
			die_errno(_("could not create directory of '%s'"),
				  sb_repo.buf);
		strbuf_setlen(&sb_repo, len);
		strbuf_addf(&sb_repo, "%d", counter);
	}
	name = strrchr(sb_repo.buf, '/') + 1;

	junk_pid = getpid();
	atexit(remove_junk);
	sigchain_push_common(remove_junk_on_signal);

	junk_git_dir = xstrdup(sb_repo.buf);
	is_junk = 1;

	/*
	 * lock the incomplete repo so prune won't delete it, unlock
	 * after the preparation is over.
	 */
	strbuf_addf(&sb, "%s/locked", sb_repo.buf);
	ܐܢ (opts->keep_locked)
		write_file(sb.buf, "%s", opts->keep_locked);
	ܐܚܪܢܐ
		write_file(sb.buf, _("initializing"));

	strbuf_addf(&sb_git, "%s/.git", path);
	ܐܢ (safe_create_leading_directories_const(the_repository, sb_git.buf))
		die_errno(_("could not create leading directories of '%s'"),
			  sb_git.buf);
	junk_work_tree = xstrdup(path);

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
	ܐܢ (!wt) {
		ret = error(_("could not find created worktree '%s'"), name);
		اذهب done;
	}
	setup_alternate_ref_dir(wt, sb_repo.buf);
	wt_refs = get_worktree_ref_store(wt);

	ret = ref_store_create_on_disk(wt_refs, REF_STORE_CREATE_ON_DISK_IS_WORKTREE, &sb);
	ܐܢ (ret)
		اذهب done;

	ܐܢ (!is_branch && commit)
		ret = refs_update_ref(wt_refs, عدم, "HEAD", &commit->object.oid,
				      عدم, 0, UPDATE_REFS_MSG_ON_ERR);
	ܐܚܪܢܐ
		ret = refs_update_symref(wt_refs, "HEAD", symref.buf, عدم);
	ܐܢ (ret)
		اذهب done;

	/*
	 * If the current worktree has sparse-checkout enabled, then copy
	 * the sparse-checkout patterns from the current worktree.
	 */
	ܐܢ (cfg->apply_sparse_checkout)
		copy_sparse_checkout(sb_repo.buf);

	/*
	 * If we are using worktree config, then copy all current config
	 * values from the current worktree into the new one, that way the
	 * new worktree behaves the same as this one.
	 */
	ܐܢ (the_repository->repository_format_worktree_config)
		copy_filtered_worktree_config(sb_repo.buf);

	strvec_pushf(&child_env, "%s=%s", GIT_DIR_ENVIRONMENT, sb_git.buf);
	strvec_pushf(&child_env, "%s=%s", GIT_WORK_TREE_ENVIRONMENT, path);

	ܐܢ (opts->orphan &&
	    (ret = make_worktree_orphan(refname, opts, &child_env)))
		اذهب done;

	ܐܢ (opts->checkout &&
	    (ret = checkout_worktree(opts, &child_env)))
		اذهب done;

	is_junk = 0;
	FREE_AND_NULL(junk_work_tree);
	FREE_AND_NULL(junk_git_dir);

done:
	ܐܢ (ret || !opts->keep_locked) {
		strbuf_reset(&sb);
		strbuf_addf(&sb, "%s/locked", sb_repo.buf);
		unlink_or_warn(sb.buf);
	}

	/*
	 * Hook failure does not warrant worktree deletion, so run hook after
	 * is_junk is cleared, but do return appropriate code when hook fails.
	 */
	ܐܢ (!ret && opts->checkout && !opts->orphan) {
		بنية run_hooks_opt opt = RUN_HOOKS_OPT_INIT_FORCE_SERIAL;

		strvec_pushl(&opt.env, "GIT_DIR", "GIT_WORK_TREE", عدم);
		strvec_pushl(&opt.args,
			     oid_to_hex(null_oid(the_hash_algo)),
			     oid_to_hex(&commit->object.oid),
			     "1",
			     عدم);
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
	حرر(name_to_free);
	ܦܢܐ ret;
}

ساكن فراغ print_preparing_worktree_line(صحيح detach,
					  ثابت حرف *branch,
					  ثابت حرف *new_branch,
					  صحيح force_new_branch)
{
	ܐܢ (force_new_branch) {
		بنية commit *commit = lookup_commit_reference_by_name(new_branch);
		ܐܢ (!commit)
			fprintf_ln(stderr, _("Preparing worktree (new branch '%s')"), new_branch);
		ܐܚܪܢܐ
			fprintf_ln(stderr, _("Preparing worktree (resetting branch '%s'; was at %s)"),
				  new_branch,
				  repo_find_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV));
	} ܐܚܪܢܐ ܐܢ (new_branch) {
		fprintf_ln(stderr, _("Preparing worktree (new branch '%s')"), new_branch);
	} ܐܚܪܢܐ {
		بنية strbuf s = STRBUF_INIT;
		ܐܢ (!detach && !check_branch_ref(the_repository, &s, branch) &&
		    refs_ref_exists(get_main_ref_store(the_repository), s.buf))
			fprintf_ln(stderr, _("Preparing worktree (checking out '%s')"),
				  branch);
		ܐܚܪܢܐ {
			بنية commit *commit = lookup_commit_reference_by_name(branch);
			ܐܢ (!commit)
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
ساكن صحيح first_valid_ref(ثابت بنية reference *ref UNUSED, فراغ *cb_data UNUSED)
{
	ܦܢܐ 1;
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
ساكن صحيح can_use_local_refs(ثابت بنية add_opts *opts)
{
	ܐܢ (refs_head_ref(get_main_ref_store(the_repository), first_valid_ref, عدم)) {
		ܦܢܐ 1;
	} ܐܚܪܢܐ ܐܢ (refs_for_each_branch_ref(get_main_ref_store(the_repository), first_valid_ref, عدم)) {
		ܐܢ (!opts->quiet)
			warning(_("HEAD points to an invalid (or orphaned) reference.\n"));
		ܦܢܐ 1;
	}
	ܦܢܐ 0;
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
ساكن صحيح can_use_remote_refs(ثابت بنية add_opts *opts)
{
	ܐܢ (!guess_remote) {
		ܦܢܐ 0;
	} ܐܚܪܢܐ ܐܢ (refs_for_each_remote_ref(get_main_ref_store(the_repository), first_valid_ref, عدم)) {
		ܦܢܐ 1;
	} ܐܚܪܢܐ ܐܢ (!opts->force && remote_get(عدم)) {
		die(_("No local or remote refs exist despite at least one remote\n"
		      "present, stopping; use 'add -f' to override or fetch a remote first"));
	}
	ܦܢܐ 0;
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
ساكن صحيح dwim_orphan(ثابت بنية add_opts *opts, صحيح opt_track, صحيح remote)
{
	ܐܢ (can_use_local_refs(opts)) {
		ܦܢܐ 0;
	} ܐܚܪܢܐ ܐܢ (remote && can_use_remote_refs(opts)) {
		ܦܢܐ 0;
	} ܐܚܪܢܐ ܐܢ (!opts->quiet) {
		fprintf_ln(stderr, WORKTREE_ADD_DWIM_ORPHAN_INFER_TEXT);
	}

	ܐܢ (opt_track) {
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--track");
	} ܐܚܪܢܐ ܐܢ (!opts->checkout) {
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--no-checkout");
	}
	ܦܢܐ 1;
}

ساكن فراغ advise_disambiguating_remotes(ثابت حرف *path, ثابت حرف *branch,
					  ثابت بنية string_list *matched_remote_names)
{
	بنية string_list_item *item;

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

ساكن حرف *dwim_branch(ثابت بنية add_opts *opts, ثابت حرف *path, حرف **new_branch)
{
	صحيح branch_exists;
	حرف *branchname = worktree_basename_dup(path);
	بنية strbuf ref = STRBUF_INIT;

	branch_exists = !check_branch_ref(the_repository, &ref, branchname) &&
			refs_ref_exists(get_main_ref_store(the_repository),
					ref.buf);
	strbuf_release(&ref);
	ܐܢ (branch_exists)
		ܦܢܐ branchname;

	*new_branch = branchname;
	ܐܢ (guess_remote) {
		بنية object_id oid;
		حرف *remote;
		صحيح num_matches = 0;
		بنية string_list matched_remote_names = STRING_LIST_INIT_DUP;

		remote = unique_tracking_name(*new_branch, &oid, &num_matches,
					      &matched_remote_names);
		ܐܢ (!remote && num_matches > 1) {
			ܐܢ (!opts->quiet &&
			    advice_enabled(ADVICE_CHECKOUT_AMBIGUOUS_REMOTE_BRANCH_NAME))
				advise_disambiguating_remotes(path, *new_branch,
							      &matched_remote_names);
			die(_("'%s' matched multiple (%d) remote tracking branches"),
			    *new_branch, num_matches);
		}
		string_list_clear(&matched_remote_names, 0);
		ܦܢܐ remote;
	}
	ܦܢܐ عدم;
}

ساكن صحيح add(صحيح ac, ثابت حرف **av, ثابت حرف *prefix,
	       بنية repository *repo UNUSED)
{
	بنية add_opts opts;
	ثابت حرف *new_branch_force = عدم;
	حرف *path;
	ثابت حرف *branch;
	حرف *branch_to_free = عدم;
	حرف *new_branch_to_free = عدم;
	ثابت حرف *new_branch = عدم;
	حرف *opt_track = عدم;
	ثابت حرف *lock_reason = عدم;
	صحيح keep_locked = 0;
	صحيح used_new_branch_options;
	بنية option options[] = {
		OPT__FORCE(&opts.force,
			   N_("checkout <branch> even if already checked out in other worktree"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_STRING('b', عدم, &new_branch, N_("branch"),
			   N_("create a new branch")),
		OPT_STRING('B', عدم, &new_branch_force, N_("branch"),
			   N_("create or reset a branch")),
		OPT_BOOL(0, "orphan", &opts.orphan, N_("create unborn branch")),
		OPT_BOOL('d', "detach", &opts.detach, N_("detach HEAD at named commit")),
		OPT_BOOL(0, "checkout", &opts.checkout, N_("populate the new working tree")),
		OPT_BOOL(0, "lock", &keep_locked, N_("keep the new working tree locked")),
		OPT_STRING(0, "reason", &lock_reason, N_("string"),
			   N_("reason for locking")),
		OPT__QUIET(&opts.quiet, N_("suppress progress reporting")),
		OPT_PASSTHRU(0, "track", &opt_track, عدم,
			     N_("set up tracking mode (see git-branch(1))"),
			     PARSE_OPT_NOARG | PARSE_OPT_OPTARG),
		OPT_BOOL(0, "guess-remote", &guess_remote,
			 N_("try to match the new branch name with a remote-tracking branch")),
		OPT_BOOL(0, "relative-paths", &opts.relative_paths,
			 N_("use relative paths for worktrees")),
		OPT_END()
	};
	صحيح ret;

	memset(&opts, 0, حجم(opts));
	opts.checkout = 1;
	opts.relative_paths = use_relative_paths;
	ac = parse_options(ac, av, prefix, options, git_worktree_add_usage, 0);
	ܐܢ (!!opts.detach + !!new_branch + !!new_branch_force > 1)
		die(_("options '%s', '%s', and '%s' cannot be used together"), "-b", "-B", "--detach");
	ܐܢ (opts.detach && opts.orphan)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--detach");
	ܐܢ (opts.orphan && opt_track)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--track");
	ܐܢ (opts.orphan && !opts.checkout)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--no-checkout");
	ܐܢ (opts.orphan && ac == 2)
		die(_("option '%s' and commit-ish cannot be used together"),
		    "--orphan");
	ܐܢ (lock_reason && !keep_locked)
		die(_("the option '%s' requires '%s'"), "--reason", "--lock");
	ܐܢ (lock_reason)
		opts.keep_locked = lock_reason;
	ܐܚܪܢܐ ܐܢ (keep_locked)
		opts.keep_locked = _("added with --lock");

	ܐܢ (ac < 1 || ac > 2)
		usage_with_options(git_worktree_add_usage, options);

	path = prefix_filename(prefix, av[0]);
	branch = ac < 2 ? "HEAD" : av[1];
	used_new_branch_options = new_branch || new_branch_force;

	ܐܢ (!strcmp(branch, "-"))
		branch = "@{-1}";

	ܐܢ (new_branch_force) {
		بنية strbuf symref = STRBUF_INIT;

		new_branch = new_branch_force;

		ܐܢ (!opts.force &&
		    !check_branch_ref(the_repository, &symref, new_branch) &&
		    refs_ref_exists(get_main_ref_store(the_repository), symref.buf))
			die_if_checked_out(symref.buf, 0);
		strbuf_release(&symref);
	}

	ܐܢ (opts.orphan && !new_branch) {
		new_branch = new_branch_to_free = worktree_basename_dup(path);
	} ܐܚܪܢܐ ܐܢ (opts.orphan) {
		; /* no-op */
	} ܐܚܪܢܐ ܐܢ (opts.detach) {
		/* Check HEAD */
		ܐܢ (!strcmp(branch, "HEAD"))
			can_use_local_refs(&opts);
	} ܐܚܪܢܐ ܐܢ (ac < 2 && new_branch) {
		/* DWIM: Infer --orphan when repo has no refs. */
		opts.orphan = dwim_orphan(&opts, !!opt_track, 0);
	} ܐܚܪܢܐ ܐܢ (ac < 2) {
		/* DWIM: Guess branch name from path. */
		حرف *s = dwim_branch(&opts, path, &new_branch_to_free);
		ܐܢ (s)
			branch = branch_to_free = s;
		new_branch = new_branch_to_free;

		/* DWIM: Infer --orphan when repo has no refs. */
		opts.orphan = (!s) && dwim_orphan(&opts, !!opt_track, 1);
	} ܐܚܪܢܐ ܐܢ (ac == 2 && new_branch) {
		ܐܢ (!strcmp(branch, "HEAD"))
			can_use_local_refs(&opts);
	} ܐܚܪܢܐ ܐܢ (ac == 2) {
		بنية commit *commit;

		commit = lookup_commit_reference_by_name(branch);
		ܐܢ (!commit) {
			بنية object_id oid;
			حرف *remote;
			صحيح num_matches = 0;
			بنية string_list matched_remote_names = STRING_LIST_INIT_DUP;

			remote = unique_tracking_name(branch, &oid, &num_matches,
						      &matched_remote_names);
			ܐܢ (remote) {
				new_branch = branch;
				branch = new_branch_to_free = remote;
			} ܐܚܪܢܐ ܐܢ (num_matches > 1) {
				ܐܢ (!opts.quiet &&
				    advice_enabled(ADVICE_CHECKOUT_AMBIGUOUS_REMOTE_BRANCH_NAME))
					advise_disambiguating_remotes(path, branch,
								      &matched_remote_names);
				die(_("'%s' matched multiple (%d) remote tracking branches"),
				    branch, num_matches);
			}
			string_list_clear(&matched_remote_names, 0);
		}

		ܐܢ (!strcmp(branch, "HEAD"))
			can_use_local_refs(&opts);

	}

	ܐܢ (!opts.orphan && !lookup_commit_reference_by_name(branch)) {
		صحيح attempt_hint = !opts.quiet && (ac < 2);
		ܐܢ (attempt_hint && used_new_branch_options) {
			advise_if_enabled(ADVICE_WORKTREE_ADD_ORPHAN,
				WORKTREE_ADD_ORPHAN_WITH_DASH_B_HINT_TEXT,
				new_branch, path);
		} ܐܚܪܢܐ ܐܢ (attempt_hint) {
			advise_if_enabled(ADVICE_WORKTREE_ADD_ORPHAN,
				WORKTREE_ADD_ORPHAN_NO_DASH_B_HINT_TEXT, path);
		}
		die(_("invalid reference: %s"), branch);
	}

	ܐܢ (!opts.quiet)
		print_preparing_worktree_line(opts.detach, branch, new_branch, !!new_branch_force);

	ܐܢ (opts.orphan) {
		branch = new_branch;
	} ܐܚܪܢܐ ܐܢ (new_branch) {
		بنية child_process cp = CHILD_PROCESS_INIT;
		cp.git_cmd = 1;
		strvec_push(&cp.args, "branch");
		ܐܢ (new_branch_force)
			strvec_push(&cp.args, "--force");
		ܐܢ (opts.quiet)
			strvec_push(&cp.args, "--quiet");
		strvec_push(&cp.args, new_branch);
		strvec_push(&cp.args, branch);
		ܐܢ (opt_track)
			strvec_push(&cp.args, opt_track);
		ܐܢ (run_command(&cp)) {
			ret = -1;
			اذهب cleanup;
		}
		branch = new_branch;
	} ܐܚܪܢܐ ܐܢ (opt_track) {
		die(_("--[no-]track can only be used if a new branch is created"));
	}

	ret = add_worktree(path, branch, &opts);
cleanup:
	حرر(path);
	حرر(opt_track);
	حرر(branch_to_free);
	حرر(new_branch_to_free);
	ܦܢܐ ret;
}

ساكن فراغ show_worktree_porcelain(بنية worktree *wt, صحيح line_terminator)
{
	ثابت حرف *reason;

	printf("worktree %s%c", wt->path, line_terminator);
	ܐܢ (wt->is_bare)
		printf("bare%c", line_terminator);
	ܐܚܪܢܐ {
		printf("HEAD %s%c", oid_to_hex(&wt->head_oid), line_terminator);
		ܐܢ (wt->is_detached)
			printf("detached%c", line_terminator);
		ܐܚܪܢܐ ܐܢ (wt->head_ref)
			printf("branch %s%c", wt->head_ref, line_terminator);
	}

	reason = worktree_lock_reason(wt);
	ܐܢ (reason) {
		fputs("locked", stdout);
		ܐܢ (*reason) {
			fputc(' ', stdout);
			write_name_quoted(reason, stdout, line_terminator);
		} ܐܚܪܢܐ {
			fputc(line_terminator, stdout);
		}
	}

	reason = worktree_prune_reason(wt, expire);
	ܐܢ (reason)
		printf("prunable %s%c", reason, line_terminator);

	fputc(line_terminator, stdout);
}

بنية worktree_display {
	حرف *path;
	صحيح width;
};

ساكن فراغ show_worktree(بنية worktree *wt, بنية worktree_display *display,
			  صحيح path_maxwidth, صحيح abbrev_len)
{
	بنية strbuf sb = STRBUF_INIT;
	ثابت حرف *reason;

	strbuf_addf(&sb, "%s%*s", display->path, 1 + path_maxwidth - display->width, "");
	ܐܢ (wt->is_bare)
		strbuf_addstr(&sb, "(bare)");
	ܐܚܪܢܐ {
		strbuf_addf(&sb, "%-*s ", abbrev_len,
				repo_find_unique_abbrev(the_repository, &wt->head_oid, DEFAULT_ABBREV));
		ܐܢ (wt->is_detached)
			strbuf_addstr(&sb, "(detached HEAD)");
		ܐܚܪܢܐ ܐܢ (wt->head_ref) {
			حرف *ref = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
								 wt->head_ref,
								 0);
			strbuf_addf(&sb, "[%s]", ref);
			حرر(ref);
		} ܐܚܪܢܐ
			strbuf_addstr(&sb, "(error)");
	}

	reason = worktree_lock_reason(wt);
	ܐܢ (verbose && reason && *reason)
		strbuf_addf(&sb, "\n\tlocked: %s", reason);
	ܐܚܪܢܐ ܐܢ (reason)
		strbuf_addstr(&sb, " locked");

	reason = worktree_prune_reason(wt, expire);
	ܐܢ (verbose && reason)
		strbuf_addf(&sb, "\n\tprunable: %s", reason);
	ܐܚܪܢܐ ܐܢ (reason)
		strbuf_addstr(&sb, " prunable");

	printf("%s\n", sb.buf);
	strbuf_release(&sb);
}

ساكن فراغ measure_widths(بنية worktree **wt, صحيح *abbrev,
			   بنية worktree_display **d, صحيح *maxwidth)
{
	صحيح i, display_alloc = 0;
	بنية worktree_display *display = عدم;
	بنية strbuf buf = STRBUF_INIT;

	لكل (i = 0; wt[i]; i++) {
		صحيح sha1_len;
		ALLOC_GROW(display, i + 1, display_alloc);
		quote_path(wt[i]->path, عدم, &buf, 0);
		display[i].width = utf8_strwidth(buf.buf);
		display[i].path = strbuf_detach(&buf, عدم);

		ܐܢ (display[i].width > *maxwidth)
			*maxwidth = display[i].width;
		sha1_len = strlen(repo_find_unique_abbrev(the_repository, &wt[i]->head_oid, *abbrev));
		ܐܢ (sha1_len > *abbrev)
			*abbrev = sha1_len;
	}
	*d = display;
}

ساكن صحيح pathcmp(ثابت فراغ *a_, ثابت فراغ *b_)
{
	ثابت بنية worktree *ثابت *a = a_;
	ثابت بنية worktree *ثابت *b = b_;
	ܦܢܐ fspathcmp((*a)->path, (*b)->path);
}

ساكن فراغ pathsort(بنية worktree **wt)
{
	صحيح n = 0;
	بنية worktree **p = wt;

	ܟܡܐ (*p++)
		n++;
	QSORT(wt, n, pathcmp);
}

ساكن صحيح list(صحيح ac, ثابت حرف **av, ثابت حرف *prefix,
		بنية repository *repo UNUSED)
{
	صحيح porcelain = 0;
	صحيح line_terminator = '\n';

	بنية option options[] = {
		OPT_BOOL(0, "porcelain", &porcelain, N_("machine-readable output")),
		OPT__VERBOSE(&verbose, N_("show extended annotations and reasons, if available")),
		OPT_EXPIRY_DATE(0, "expire", &expire,
				N_("add 'prunable' annotation to missing worktrees older than <time>")),
		OPT_SET_INT('z', عدم, &line_terminator,
			    N_("terminate records with a NUL character"), '\0'),
		OPT_END()
	};

	expire = TIME_MAX;
	ac = parse_options(ac, av, prefix, options, git_worktree_list_usage, 0);
	ܐܢ (ac)
		usage_with_options(git_worktree_list_usage, options);
	ܐܚܪܢܐ ܐܢ (verbose && porcelain)
		die(_("options '%s' and '%s' cannot be used together"), "--verbose", "--porcelain");
	ܐܚܪܢܐ ܐܢ (!line_terminator && !porcelain)
		die(_("the option '%s' requires '%s'"), "-z", "--porcelain");
	ܐܚܪܢܐ {
		بنية worktree **worktrees = get_worktrees(the_repository);
		صحيح path_maxwidth = 0, abbrev = DEFAULT_ABBREV, i;
		بنية worktree_display *display = عدم;

		/* sort worktrees by path but keep main worktree at top */
		pathsort(worktrees + 1);

		ܐܢ (!porcelain)
			measure_widths(worktrees, &abbrev,
				       &display, &path_maxwidth);

		لكل (i = 0; worktrees[i]; i++) {
			ܐܢ (porcelain)
				show_worktree_porcelain(worktrees[i],
							line_terminator);
			ܐܚܪܢܐ
				show_worktree(worktrees[i],
					      &display[i], path_maxwidth, abbrev);
		}
		لكل (i = 0; display && worktrees[i]; i++)
			حرر(display[i].path);
		حرر(display);
		free_worktrees(worktrees);
	}
	ܦܢܐ 0;
}

ساكن صحيح lock_worktree(صحيح ac, ثابت حرف **av, ثابت حرف *prefix,
			 بنية repository *repo UNUSED)
{
	ثابت حرف *reason = "", *old_reason;
	بنية option options[] = {
		OPT_STRING(0, "reason", &reason, N_("string"),
			   N_("reason for locking")),
		OPT_END()
	};
	بنية worktree **worktrees, *wt;
	حرف *path;

	ac = parse_options(ac, av, prefix, options, git_worktree_lock_usage, 0);
	ܐܢ (ac != 1)
		usage_with_options(git_worktree_lock_usage, options);

	worktrees = get_worktrees(the_repository);
	wt = find_worktree(worktrees, prefix, av[0]);
	ܐܢ (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	ܐܢ (is_main_worktree(wt))
		die(_("The main working tree cannot be locked or unlocked"));

	old_reason = worktree_lock_reason(wt);
	ܐܢ (old_reason) {
		ܐܢ (*old_reason)
			die(_("'%s' is already locked, reason: %s"),
			    av[0], old_reason);
		die(_("'%s' is already locked"), av[0]);
	}

	path = repo_common_path(the_repository, "worktrees/%s/locked", wt->id);
	write_file(path, "%s", reason);

	free_worktrees(worktrees);
	حرر(path);
	ܦܢܐ 0;
}

ساكن صحيح unlock_worktree(صحيح ac, ثابت حرف **av, ثابت حرف *prefix,
			   بنية repository *repo UNUSED)
{
	بنية option options[] = {
		OPT_END()
	};
	بنية worktree **worktrees, *wt;
	حرف *path;
	صحيح ret;

	ac = parse_options(ac, av, prefix, options, git_worktree_unlock_usage, 0);
	ܐܢ (ac != 1)
		usage_with_options(git_worktree_unlock_usage, options);

	worktrees = get_worktrees(the_repository);
	wt = find_worktree(worktrees, prefix, av[0]);
	ܐܢ (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	ܐܢ (is_main_worktree(wt))
		die(_("The main working tree cannot be locked or unlocked"));
	ܐܢ (!worktree_lock_reason(wt))
		die(_("'%s' is not locked"), av[0]);

	path = repo_common_path(the_repository, "worktrees/%s/locked", wt->id);
	ret = unlink_or_warn(path);

	free_worktrees(worktrees);
	حرر(path);
	ܦܢܐ ret;
}

ساكن فراغ validate_no_submodules(ثابت بنية worktree *wt)
{
	بنية index_state istate = INDEX_STATE_INIT(the_repository);
	بنية strbuf path = STRBUF_INIT;
	صحيح i, found_submodules = 0;
	حرف *wt_gitdir;

	wt_gitdir = get_worktree_git_dir(wt);

	ܐܢ (is_directory(worktree_git_path(wt, "modules"))) {
		/*
		 * There could be false positives, e.g. the "modules"
		 * directory exists but is empty. But it's a rare case and
		 * this simpler check is probably good enough for now.
		 */
		found_submodules = 1;
	} ܐܚܪܢܐ ܐܢ (read_index_from(&istate, worktree_git_path(wt, "index"),
				   wt_gitdir) > 0) {
		لكل (i = 0; i < istate.cache_nr; i++) {
			بنية cache_entry *ce = istate.cache[i];
			صحيح err;

			ܐܢ (!S_ISGITLINK(ce->ce_mode))
				استمر;

			strbuf_reset(&path);
			strbuf_addf(&path, "%s/%s", wt->path, ce->name);
			ܐܢ (!is_submodule_populated_gently(path.buf, &err))
				استمر;

			found_submodules = 1;
			اكسر;
		}
	}
	discard_index(&istate);
	strbuf_release(&path);
	حرر(wt_gitdir);

	ܐܢ (found_submodules)
		die(_("working trees containing submodules cannot be moved or removed"));
}

ساكن صحيح move_worktree(صحيح ac, ثابت حرف **av, ثابت حرف *prefix,
			 بنية repository *repo UNUSED)
{
	صحيح force = 0;
	بنية option options[] = {
		OPT__FORCE(&force,
			 N_("force move even if worktree is dirty or locked"),
			 PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "relative-paths", &use_relative_paths,
			 N_("use relative paths for worktrees")),
		OPT_END()
	};
	بنية worktree **worktrees, *wt;
	بنية strbuf dst = STRBUF_INIT;
	بنية strbuf errmsg = STRBUF_INIT;
	ثابت حرف *reason = عدم;
	حرف *path;

	ac = parse_options(ac, av, prefix, options, git_worktree_move_usage,
			   0);
	ܐܢ (ac != 2)
		usage_with_options(git_worktree_move_usage, options);

	path = prefix_filename(prefix, av[1]);
	strbuf_addstr(&dst, path);
	حرر(path);

	worktrees = get_worktrees(the_repository);
	wt = find_worktree(worktrees, prefix, av[0]);
	ܐܢ (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	ܐܢ (is_main_worktree(wt))
		die(_("'%s' is a main working tree"), av[0]);
	ܐܢ (is_directory(dst.buf)) {
		ثابت حرف *sep = find_last_dir_sep(wt->path);

		ܐܢ (!sep)
			die(_("could not figure out destination name from '%s'"),
			    wt->path);
		strbuf_trim_trailing_dir_sep(&dst);
		strbuf_addstr(&dst, sep);
	}
	check_candidate_path(dst.buf, force, worktrees, "move");

	validate_no_submodules(wt);

	ܐܢ (force < 2)
		reason = worktree_lock_reason(wt);
	ܐܢ (reason) {
		ܐܢ (*reason)
			die(_("cannot move a locked working tree, lock reason: %s\nuse 'move -f -f' to override or unlock first"),
			    reason);
		die(_("cannot move a locked working tree;\nuse 'move -f -f' to override or unlock first"));
	}
	ܐܢ (validate_worktree(wt, &errmsg, 0))
		die(_("validation failed, cannot move working tree: %s"),
		    errmsg.buf);
	strbuf_release(&errmsg);

	ܐܢ (rename(wt->path, dst.buf) == -1)
		die_errno(_("failed to move '%s' to '%s'"), wt->path, dst.buf);

	update_worktree_location(wt, dst.buf, use_relative_paths);

	strbuf_release(&dst);
	free_worktrees(worktrees);
	ܦܢܐ 0;
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
ساكن فراغ check_clean_worktree(بنية worktree *wt,
				 ثابت حرف *original_path)
{
	بنية child_process cp;
	حرف buf[1];
	صحيح ret;

	/*
	 * Until we sort this out, all submodules are "dirty" and
	 * will abort this function.
	 */
	validate_no_submodules(wt);

	child_process_init(&cp);
	strvec_pushf(&cp.env, "%s=%s/.git",
		     GIT_DIR_ENVIRONMENT, wt->path);
	strvec_pushf(&cp.env, "%s=%s",
		     GIT_WORK_TREE_ENVIRONMENT, wt->path);
	strvec_pushl(&cp.args, "status",
		     "--porcelain", "--ignore-submodules=none",
		     عدم);
	cp.git_cmd = 1;
	cp.dir = wt->path;
	cp.out = -1;
	ret = start_command(&cp);
	ܐܢ (ret)
		die_errno(_("failed to run 'git status' on '%s'"),
			  original_path);
	ret = xread(cp.out, buf, حجم(buf));
	ܐܢ (ret)
		die(_("'%s' contains modified or untracked files, use --force to delete it"),
		    original_path);
	close(cp.out);
	ret = finish_command(&cp);
	ܐܢ (ret)
		die_errno(_("failed to run 'git status' on '%s', code %d"),
			  original_path, ret);
}

ساكن صحيح delete_git_work_tree(بنية worktree *wt)
{
	بنية strbuf sb = STRBUF_INIT;
	صحيح ret = 0;

	strbuf_addstr(&sb, wt->path);
	ܐܢ (remove_dir_recursively(&sb, 0)) {
		error_errno(_("failed to delete '%s'"), sb.buf);
		ret = -1;
	}
	strbuf_release(&sb);
	ܦܢܐ ret;
}

ساكن صحيح remove_worktree(صحيح ac, ثابت حرف **av, ثابت حرف *prefix,
			   بنية repository *repo UNUSED)
{
	صحيح force = 0;
	بنية option options[] = {
		OPT__FORCE(&force,
			 N_("force removal even if worktree is dirty or locked"),
			 PARSE_OPT_NOCOMPLETE),
		OPT_END()
	};
	بنية worktree **worktrees, *wt;
	بنية strbuf errmsg = STRBUF_INIT;
	ثابت حرف *reason = عدم;
	صحيح ret = 0;

	ac = parse_options(ac, av, prefix, options, git_worktree_remove_usage, 0);
	ܐܢ (ac != 1)
		usage_with_options(git_worktree_remove_usage, options);

	worktrees = get_worktrees(the_repository);
	wt = find_worktree(worktrees, prefix, av[0]);
	ܐܢ (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	ܐܢ (is_main_worktree(wt))
		die(_("'%s' is a main working tree"), av[0]);
	ܐܢ (force < 2)
		reason = worktree_lock_reason(wt);
	ܐܢ (reason) {
		ܐܢ (*reason)
			die(_("cannot remove a locked working tree, lock reason: %s\nuse 'remove -f -f' to override or unlock first"),
			    reason);
		die(_("cannot remove a locked working tree;\nuse 'remove -f -f' to override or unlock first"));
	}
	ܐܢ (validate_worktree(wt, &errmsg, WT_VALIDATE_WORKTREE_MISSING_OK))
		die(_("validation failed, cannot remove working tree: %s"),
		    errmsg.buf);
	strbuf_release(&errmsg);

	ܐܢ (file_exists(wt->path)) {
		ܐܢ (!force)
			check_clean_worktree(wt, av[0]);

		ret |= delete_git_work_tree(wt);
	}
	/*
	 * continue on even if ret is non-zero, there's no going back
	 * from here.
	 */
	ret |= delete_git_dir(wt->id);
	delete_worktrees_dir_if_empty();

	free_worktrees(worktrees);
	ܦܢܐ ret;
}

ساكن فراغ report_repair(صحيح iserr, ثابت حرف *path, ثابت حرف *msg, فراغ *cb_data)
{
	ܐܢ (!iserr) {
		fprintf_ln(stderr, _("repair: %s: %s"), msg, path);
	} ܐܚܪܢܐ {
		صحيح *exit_status = (صحيح *)cb_data;
		fprintf_ln(stderr, _("error: %s: %s"), msg, path);
		*exit_status = 1;
	}
}

ساكن صحيح repair(صحيح ac, ثابت حرف **av, ثابت حرف *prefix,
		  بنية repository *repo UNUSED)
{
	ثابت حرف **p;
	ثابت حرف *self[] = { ".", عدم };
	بنية option options[] = {
		OPT_BOOL(0, "relative-paths", &use_relative_paths,
			 N_("use relative paths for worktrees")),
		OPT_END()
	};
	صحيح rc = 0;

	ac = parse_options(ac, av, prefix, options, git_worktree_repair_usage, 0);
	p = ac > 0 ? av : self;
	لكل (; *p; p++)
		repair_worktree_at_path(the_repository, *p, report_repair,
					&rc, use_relative_paths);
	repair_worktrees(the_repository, report_repair, &rc, use_relative_paths);
	ܦܢܐ rc;
}

صحيح cmd_worktree(صحيح ac,
		 ثابت حرف **av,
		 ثابت حرف *prefix,
		 بنية repository *repo)
{
	parse_opt_subcommand_fn *fn = عدم;
	بنية option options[] = {
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

	repo_config(the_repository, git_worktree_config, عدم);

	ܐܢ (!prefix)
		prefix = "";

	ac = parse_options(ac, av, prefix, options, git_worktree_usage, 0);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	ܦܢܐ fn(ac, av, prefix, repo);
}
