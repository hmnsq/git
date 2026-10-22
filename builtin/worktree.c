#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#شامل "builtin.h"
#شامل "abspath.h"
#شامل "advice.h"
#شامل "checkout.h"
#شامل "config.h"
#شامل "copy.h"
#شامل "dir.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "hex.h"
#شامل "object-file.h"
#شامل "object-name.h"
#شامل "parse-options.h"
#شامل "path.h"
#شامل "strvec.h"
#شامل "branch.h"
#شامل "read-cache-ll.h"
#شامل "refs.h"
#شامل "remote.h"
#شامل "run-command.h"
#شامل "hook.h"
#شامل "sigchain.h"
#شامل "submodule.h"
#شامل "utf8.h"
#شامل "worktree.h"
#شامل "quote.h"

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

ایستا ثابت نویسه * ثابت git_worktree_usage[] = {
	BUILTIN_WORKTREE_ADD_USAGE,
	BUILTIN_WORKTREE_LIST_USAGE,
	BUILTIN_WORKTREE_LOCK_USAGE,
	BUILTIN_WORKTREE_MOVE_USAGE,
	BUILTIN_WORKTREE_PRUNE_USAGE,
	BUILTIN_WORKTREE_REMOVE_USAGE,
	BUILTIN_WORKTREE_REPAIR_USAGE,
	BUILTIN_WORKTREE_UNLOCK_USAGE,
	پوچ
};

ایستا ثابت نویسه * ثابت git_worktree_add_usage[] = {
	BUILTIN_WORKTREE_ADD_USAGE,
	پوچ,
};

ایستا ثابت نویسه * ثابت git_worktree_list_usage[] = {
	BUILTIN_WORKTREE_LIST_USAGE,
	پوچ
};

ایستا ثابت نویسه * ثابت git_worktree_lock_usage[] = {
	BUILTIN_WORKTREE_LOCK_USAGE,
	پوچ
};

ایستا ثابت نویسه * ثابت git_worktree_move_usage[] = {
	BUILTIN_WORKTREE_MOVE_USAGE,
	پوچ
};

ایستا ثابت نویسه * ثابت git_worktree_prune_usage[] = {
	BUILTIN_WORKTREE_PRUNE_USAGE,
	پوچ
};

ایستا ثابت نویسه * ثابت git_worktree_remove_usage[] = {
	BUILTIN_WORKTREE_REMOVE_USAGE,
	پوچ
};

ایستا ثابت نویسه * ثابت git_worktree_repair_usage[] = {
	BUILTIN_WORKTREE_REPAIR_USAGE,
	پوچ
};

ایستا ثابت نویسه * ثابت git_worktree_unlock_usage[] = {
	BUILTIN_WORKTREE_UNLOCK_USAGE,
	پوچ
};

ساختار add_opts {
	صحیح force;
	صحیح detach;
	صحیح quiet;
	صحیح checkout;
	صحیح orphan;
	صحیح relative_paths;
	ثابت نویسه *keep_locked;
};

ایستا صحیح show_only;
ایستا صحیح verbose;
ایستا صحیح guess_remote;
ایستا صحیح use_relative_paths;
ایستا timestamp_t expire;

ایستا صحیح git_worktree_config(ثابت نویسه *var, ثابت نویسه *مقدار,
			       ثابت ساختار config_context *ctx, تهی *cb)
{
	اگر (!strcmp(var, "worktree.guessremote")) {
		guess_remote = git_config_bool(var, مقدار);
		بازگشت 0;
	} وگرنه اگر (!strcmp(var, "worktree.userelativepaths")) {
		use_relative_paths = git_config_bool(var, مقدار);
		بازگشت 0;
	}

	بازگشت git_default_config(var, مقدار, ctx, cb);
}

ایستا صحیح delete_git_dir(ثابت نویسه *id)
{
	ساختار strbuf sb = STRBUF_INIT;
	صحیح ret;

	repo_common_path_append(the_repository, &sb, "worktrees/%s", id);
	ret = remove_dir_recursively(&sb, 0);
	اگر (ret < 0 && errno == ENOTDIR)
		ret = unlink(sb.buf);
	اگر (ret)
		error_errno(_("failed to delete '%s'"), sb.buf);
	strbuf_release(&sb);
	بازگشت ret;
}

ایستا تهی delete_worktrees_dir_if_empty(تهی)
{
	نویسه *path = repo_git_path(the_repository, "worktrees");
	rmdir(path); /* ignore failed removal */
	آزاد(path);
}

ایستا تهی prune_worktree(ثابت نویسه *id, ثابت نویسه *reason)
{
	اگر (show_only || verbose)
		fprintf_ln(stderr, _("Removing %s/%s: %s"), "worktrees", id, reason);
	اگر (!show_only)
		delete_git_dir(id);
}

ایستا صحیح prune_cmp(ثابت تهی *a, ثابت تهی *b)
{
	ثابت ساختار string_list_item *x = a;
	ثابت ساختار string_list_item *y = b;
	صحیح c;

	اگر ((c = fspathcmp(x->string, y->string)))
	    بازگشت c;
	/*
	 * paths same; prune_dupes() removes all but the first worktree entry
	 * having the same path, so sort main worktree ('util' is NULL) above
	 * linked worktrees ('util' not NULL) since main worktree can't be
	 * removed
	 */
	اگر (!x->util)
		بازگشت -1;
	اگر (!y->util)
		بازگشت 1;
	/* paths same; sort by .git/worktrees/<id> */
	بازگشت strcmp(x->util, y->util);
}

ایستا تهی prune_dups(ساختار string_list *l)
{
	صحیح i;

	QSORT(l->اقلام, l->nr, prune_cmp);
	برای (i = 1; i < l->nr; i++) {
		اگر (!fspathcmp(l->اقلام[i].string, l->اقلام[i - 1].string))
			prune_worktree(l->اقلام[i].util, "duplicate entry");
	}
}

ایستا تهی prune_worktrees(تهی)
{
	ساختار strbuf reason = STRBUF_INIT;
	ساختار strbuf main_path = STRBUF_INIT;
	ساختار string_list kept = STRING_LIST_INIT_DUP;
	نویسه *path;
	DIR *dir;
	ساختار dirent *d;

	path = repo_git_path(the_repository, "worktrees");
	dir = opendir(path);
	آزاد(path);
	اگر (!dir)
		بازگشت;
	وقتی ((d = readdir_skip_dot_and_dotdot(dir)) != پوچ) {
		نویسه *path;
		strbuf_reset(&reason);
		اگر (should_prune_worktree(the_repository, d->d_name, &reason, &path, expire))
			prune_worktree(d->d_name, reason.buf);
		وگرنه اگر (path)
			string_list_append_nodup(&kept, path)->util = xstrdup(d->d_name);
	}
	closedir(dir);

	strbuf_add_absolute_path(&main_path, repo_get_common_dir(the_repository));
	/* massage main worktree absolute path to match 'gitdir' content */
	strbuf_strip_suffix(&main_path, "/.");
	string_list_append_nodup(&kept, strbuf_detach(&main_path, پوچ));
	prune_dups(&kept);
	string_list_clear(&kept, 1);

	اگر (!show_only)
		delete_worktrees_dir_if_empty();
	strbuf_release(&reason);
}

ایستا صحیح prune(صحیح ac, ثابت نویسه **av, ثابت نویسه *prefix,
		 ساختار repository *repo UNUSED)
{
	ساختار option options[] = {
		OPT__DRY_RUN(&show_only, N_("do not remove, show only")),
		OPT__VERBOSE(&verbose, N_("report pruned working trees")),
		OPT_EXPIRY_DATE(0, "expire", &expire,
				N_("prune missing working trees older than <time>")),
		OPT_END()
	};

	expire = TIME_MAX;
	ac = parse_options(ac, av, prefix, options, git_worktree_prune_usage,
			   0);
	اگر (ac)
		usage_with_options(git_worktree_prune_usage, options);
	prune_worktrees();
	بازگشت 0;
}

ایستا نویسه *junk_work_درخت;
ایستا نویسه *junk_git_dir;
ایستا صحیح is_junk;
ایستا pid_t junk_pid;

ایستا تهی remove_junk(تهی)
{
	ساختار strbuf sb = STRBUF_INIT;
	اگر (!is_junk || getpid() != junk_pid)
		بازگشت;
	اگر (junk_git_dir) {
		strbuf_addstr(&sb, junk_git_dir);
		remove_dir_recursively(&sb, 0);
		strbuf_reset(&sb);
	}
	اگر (junk_work_درخت) {
		strbuf_addstr(&sb, junk_work_درخت);
		remove_dir_recursively(&sb, 0);
	}
	strbuf_release(&sb);
}

ایستا تهی remove_junk_on_signal(صحیح signo)
{
	remove_junk();
	sigchain_برداشت(signo);
	بالاببر(signo);
}

ایستا نویسه *worktree_basename_dup(ثابت نویسه *path)
{
	ثابت نویسه *name;
	صحیح len;

	len = strlen(path);
	وقتی (len && is_dir_sep(path[len - 1]))
		len--;

	name = path + len;
	وقتی (name > path && !is_dir_sep(name[-1]))
		name--;

	بازگشت xmemdupz(name, path + len - name);
}

/* check that path is viable location for worktree */
ایستا تهی check_candidate_path(ثابت نویسه *path,
				 صحیح force,
				 ساختار worktree **worktrees,
				 ثابت نویسه *cmd)
{
	ساختار worktree *wt;
	صحیح locked;

	اگر (file_exists(path) && !is_empty_dir(path))
		die(_("'%s' already exists"), path);

	wt = find_worktree_by_path(worktrees, path);
	اگر (!wt)
		بازگشت;

	locked = !!worktree_lock_reason(wt);
	اگر ((!locked && force) || (locked && force > 1)) {
		اگر (delete_git_dir(wt->id))
		    die(_("unusable worktree destination '%s'"), path);
		بازگشت;
	}

	اگر (locked)
		die(_("'%s' is a missing but locked worktree;\nuse '%s -f -f' to override, or 'unlock' and 'prune' or 'remove' to clear"), path, cmd);
	وگرنه
		die(_("'%s' is a missing but already registered worktree;\nuse '%s -f' to override, or 'prune' or 'remove' to clear"), path, cmd);
}

ایستا تهی copy_sparse_checkout(ثابت نویسه *worktree_git_dir)
{
	نویسه *from_file = repo_git_path(the_repository, "info/sparse-checkout");
	نویسه *to_file = xstrfmt("%s/info/sparse-checkout", worktree_git_dir);

	اگر (file_exists(from_file)) {
		اگر (safe_create_leading_directories(the_repository, to_file) ||
			copy_file(the_repository, to_file, from_file, 0666))
			error(_("failed to copy '%s' to '%s'; sparse-checkout may not work correctly"),
				from_file, to_file);
	}

	آزاد(from_file);
	آزاد(to_file);
}

ایستا تهی copy_filtered_worktree_config(ثابت نویسه *worktree_git_dir)
{
	نویسه *from_file = repo_git_path(the_repository, "config.worktree");
	نویسه *to_file = xstrfmt("%s/config.worktree", worktree_git_dir);

	اگر (file_exists(from_file)) {
		ساختار config_set cs = { { 0 } };
		صحیح bare;

		اگر (safe_create_leading_directories(the_repository, to_file) ||
			copy_file(the_repository, to_file, from_file, 0666)) {
			error(_("failed to copy worktree config from '%s' to '%s'"),
				from_file, to_file);
			برو worktree_copy_cleanup;
		}

		git_configset_init(&cs);
		git_configset_add_file(&cs, from_file);

		اگر (!git_configset_get_bool(&cs, "core.bare", &bare) &&
			bare &&
			repo_config_set_multivar_in_file_gently(the_repository,
				to_file, "core.bare", پوچ, "true", پوچ, 0))
			error(_("failed to unset '%s' in '%s'"),
				"core.bare", to_file);
		اگر (!git_configset_get(&cs, "core.worktree") &&
			repo_config_set_in_file_gently(the_repository, to_file,
						       "core.worktree", پوچ, پوچ))
			error(_("failed to unset '%s' in '%s'"),
				"core.worktree", to_file);

		git_configset_clear(&cs);
	}

worktree_copy_cleanup:
	آزاد(from_file);
	آزاد(to_file);
}

ایستا صحیح checkout_worktree(ثابت ساختار add_opts *opts,
			     ساختار strvec *child_env)
{
	ساختار child_process cp = CHILD_PROCESS_INIT;
	cp.git_cmd = 1;
	strvec_pushl(&cp.args, "reset", "--hard", "--no-recurse-submodules", پوچ);
	اگر (opts->quiet)
		strvec_افزودن(&cp.args, "--quiet");
	strvec_pushv(&cp.env, child_env->v);
	بازگشت run_command(&cp);
}

ایستا صحیح make_worktree_orphan(ثابت نویسه * ref, ثابت ساختار add_opts *opts,
				ساختار strvec *child_env)
{
	ساختار strbuf symref = STRBUF_INIT;
	ساختار child_process cp = CHILD_PROCESS_INIT;

	validate_new_branchname(ref, &symref, 0);
	strvec_pushl(&cp.args, "symbolic-ref", "HEAD", symref.buf, پوچ);
	اگر (opts->quiet)
		strvec_افزودن(&cp.args, "--quiet");
	strvec_pushv(&cp.env, child_env->v);
	strbuf_release(&symref);
	cp.git_cmd = 1;
	بازگشت run_command(&cp);
}

/*
 * References for worktrees are generally stored in '$GIT_DIR/worktrees/<wt_id>'.
 * But when using alternate reference directories, we want to store the worktree
 * references in '$ALTERNATE_REFERENCE_DIR/worktrees/<wt_id>'.
 *
 * Create the necessary folder structure to facilitate the same. But to ensure
 * that the former path is still considered a Git directory, add stubs.
 */
ایستا تهی setup_alternate_ref_dir(ساختار worktree *wt, ثابت نویسه *wt_git_path)
{
	ساختار strbuf sb = STRBUF_INIT;
	نویسه *path;

	path = wt->repo->ref_storage_payload;
	اگر (!path)
		بازگشت;

	اگر (!is_absolute_path(path))
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

ایستا صحیح add_worktree(ثابت نویسه *path, ثابت نویسه *refname,
			ثابت ساختار add_opts *opts)
{
	ساختار strbuf sb_git = STRBUF_INIT, sb_repo = STRBUF_INIT;
	ساختار strbuf sb = STRBUF_INIT;
	ثابت نویسه *name;
	نویسه *name_to_free = پوچ;
	ساختار strvec child_env = STRVEC_INIT;
	unsigned صحیح counter = 0;
	صحیح len, ret;
	ساختار strbuf symref = STRBUF_INIT;
	ساختار commit *commit = پوچ;
	صحیح is_branch = 0;
	ساختار strbuf sb_name = STRBUF_INIT;
	ساختار worktree **worktrees, *wt = پوچ;
	ساختار ref_store *wt_refs;
	ساختار repo_config_مقادیر *cfg = repo_config_مقادیر(the_repository);

	worktrees = get_worktrees(the_repository);
	check_candidate_path(path, opts->force, worktrees, "add");
	free_worktrees(worktrees);
	worktrees = پوچ;

	/* is 'refname' a branch or commit? */
	اگر (!opts->detach && !check_branch_ref(the_repository, &symref, refname) &&
	    refs_ref_exists(get_main_ref_store(the_repository), symref.buf)) {
		is_branch = 1;
		اگر (!opts->force)
			die_if_checked_out(symref.buf, 0);
	}
	commit = lookup_commit_reference_by_name(refname);
	اگر (!commit && !opts->orphan)
		die(_("invalid reference: %s"), refname);

	name = name_to_free = worktree_basename_dup(path);
	اگر (!*name)
		die(_("invalid path '%s'"), path);
	sanitize_refname_component(name, &sb_name);
	اگر (!sb_name.len)
		BUG("How come '%s' becomes empty after sanitization?", name);
	name = sb_name.buf;
	repo_git_path_replace(the_repository, &sb_repo, "worktrees/%s", name);
	len = sb_repo.len;
	اگر (safe_create_leading_directories_const(the_repository, sb_repo.buf))
		die_errno(_("could not create leading directories of '%s'"),
			  sb_repo.buf);

	وقتی (mkdir(sb_repo.buf, 0777)) {
		counter++;
		اگر ((errno != EEXIST) || !counter /* overflow */)
			die_errno(_("could not create directory of '%s'"),
				  sb_repo.buf);
		strbuf_setlen(&sb_repo, len);
		strbuf_addf(&sb_repo, "%d", counter);
	}
	name = strrchr(sb_repo.buf, '/') + 1;

	junk_pid = getpid();
	atexit(remove_junk);
	sigchain_افزودن_common(remove_junk_on_signal);

	junk_git_dir = xstrdup(sb_repo.buf);
	is_junk = 1;

	/*
	 * lock the incomplete repo so prune won't delete it, unlock
	 * after the preparation is over.
	 */
	strbuf_addf(&sb, "%s/locked", sb_repo.buf);
	اگر (opts->keep_locked)
		write_file(sb.buf, "%s", opts->keep_locked);
	وگرنه
		write_file(sb.buf, _("initializing"));

	strbuf_addf(&sb_git, "%s/.git", path);
	اگر (safe_create_leading_directories_const(the_repository, sb_git.buf))
		die_errno(_("could not create leading directories of '%s'"),
			  sb_git.buf);
	junk_work_درخت = xstrdup(path);

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
	اگر (!wt) {
		ret = error(_("could not find created worktree '%s'"), name);
		برو done;
	}
	setup_alternate_ref_dir(wt, sb_repo.buf);
	wt_refs = get_worktree_ref_store(wt);

	ret = ref_store_create_on_disk(wt_refs, REF_STORE_CREATE_ON_DISK_IS_WORKTREE, &sb);
	اگر (ret)
		برو done;

	اگر (!is_branch && commit)
		ret = refs_update_ref(wt_refs, پوچ, "HEAD", &commit->object.oid,
				      پوچ, 0, UPDATE_REFS_پیام_ON_ERR);
	وگرنه
		ret = refs_update_symref(wt_refs, "HEAD", symref.buf, پوچ);
	اگر (ret)
		برو done;

	/*
	 * If the current worktree has sparse-checkout enabled, then copy
	 * the sparse-checkout patterns from the current worktree.
	 */
	اگر (cfg->apply_sparse_checkout)
		copy_sparse_checkout(sb_repo.buf);

	/*
	 * If we are using worktree config, then copy all current config
	 * values from the current worktree into the new one, that way the
	 * new worktree behaves the same as this one.
	 */
	اگر (the_repository->repository_format_worktree_config)
		copy_filtered_worktree_config(sb_repo.buf);

	strvec_pushf(&child_env, "%s=%s", GIT_DIR_ENVIRONMENT, sb_git.buf);
	strvec_pushf(&child_env, "%s=%s", GIT_WORK_درخت_ENVIRONMENT, path);

	اگر (opts->orphan &&
	    (ret = make_worktree_orphan(refname, opts, &child_env)))
		برو done;

	اگر (opts->checkout &&
	    (ret = checkout_worktree(opts, &child_env)))
		برو done;

	is_junk = 0;
	FREE_AND_NULL(junk_work_درخت);
	FREE_AND_NULL(junk_git_dir);

done:
	اگر (ret || !opts->keep_locked) {
		strbuf_reset(&sb);
		strbuf_addf(&sb, "%s/locked", sb_repo.buf);
		unlink_or_warn(sb.buf);
	}

	/*
	 * Hook failure does not warrant worktree deletion, so run hook after
	 * is_junk is cleared, but do return appropriate code when hook fails.
	 */
	اگر (!ret && opts->checkout && !opts->orphan) {
		ساختار run_hooks_opt opt = RUN_HOOKS_OPT_INIT_FORCE_SERIAL;

		strvec_pushl(&opt.env, "GIT_DIR", "GIT_WORK_TREE", پوچ);
		strvec_pushl(&opt.args,
			     oid_to_hex(null_oid(the_hash_algo)),
			     oid_to_hex(&commit->object.oid),
			     "1",
			     پوچ);
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
	آزاد(name_to_free);
	بازگشت ret;
}

ایستا تهی print_preparing_worktree_سطر(صحیح detach,
					  ثابت نویسه *branch,
					  ثابت نویسه *new_branch,
					  صحیح force_new_branch)
{
	اگر (force_new_branch) {
		ساختار commit *commit = lookup_commit_reference_by_name(new_branch);
		اگر (!commit)
			fprintf_ln(stderr, _("Preparing worktree (new branch '%s')"), new_branch);
		وگرنه
			fprintf_ln(stderr, _("Preparing worktree (resetting branch '%s'; was at %s)"),
				  new_branch,
				  repo_find_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV));
	} وگرنه اگر (new_branch) {
		fprintf_ln(stderr, _("Preparing worktree (new branch '%s')"), new_branch);
	} وگرنه {
		ساختار strbuf s = STRBUF_INIT;
		اگر (!detach && !check_branch_ref(the_repository, &s, branch) &&
		    refs_ref_exists(get_main_ref_store(the_repository), s.buf))
			fprintf_ln(stderr, _("Preparing worktree (checking out '%s')"),
				  branch);
		وگرنه {
			ساختار commit *commit = lookup_commit_reference_by_name(branch);
			اگر (!commit)
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
ایستا صحیح first_valid_ref(ثابت ساختار reference *ref UNUSED, تهی *cb_data UNUSED)
{
	بازگشت 1;
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
ایستا صحیح can_use_local_refs(ثابت ساختار add_opts *opts)
{
	اگر (refs_head_ref(get_main_ref_store(the_repository), first_valid_ref, پوچ)) {
		بازگشت 1;
	} وگرنه اگر (refs_for_each_branch_ref(get_main_ref_store(the_repository), first_valid_ref, پوچ)) {
		اگر (!opts->quiet)
			warning(_("HEAD points to an invalid (or orphaned) reference.\n"));
		بازگشت 1;
	}
	بازگشت 0;
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
ایستا صحیح can_use_remote_refs(ثابت ساختار add_opts *opts)
{
	اگر (!guess_remote) {
		بازگشت 0;
	} وگرنه اگر (refs_for_each_remote_ref(get_main_ref_store(the_repository), first_valid_ref, پوچ)) {
		بازگشت 1;
	} وگرنه اگر (!opts->force && remote_get(پوچ)) {
		die(_("No local or remote refs exist despite at least one remote\n"
		      "present, stopping; use 'add -f' to override or fetch a remote first"));
	}
	بازگشت 0;
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
ایستا صحیح dwim_orphan(ثابت ساختار add_opts *opts, صحیح opt_track, صحیح remote)
{
	اگر (can_use_local_refs(opts)) {
		بازگشت 0;
	} وگرنه اگر (remote && can_use_remote_refs(opts)) {
		بازگشت 0;
	} وگرنه اگر (!opts->quiet) {
		fprintf_ln(stderr, WORKTREE_ADD_DWIM_ORPHAN_INFER_TEXT);
	}

	اگر (opt_track) {
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--track");
	} وگرنه اگر (!opts->checkout) {
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--no-checkout");
	}
	بازگشت 1;
}

ایستا تهی advise_disambiguating_remotes(ثابت نویسه *path, ثابت نویسه *branch,
					  ثابت ساختار string_list *matched_remote_names)
{
	ساختار string_list_item *item;

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

ایستا نویسه *dwim_branch(ثابت ساختار add_opts *opts, ثابت نویسه *path, نویسه **new_branch)
{
	صحیح branch_exists;
	نویسه *branchname = worktree_basename_dup(path);
	ساختار strbuf ref = STRBUF_INIT;

	branch_exists = !check_branch_ref(the_repository, &ref, branchname) &&
			refs_ref_exists(get_main_ref_store(the_repository),
					ref.buf);
	strbuf_release(&ref);
	اگر (branch_exists)
		بازگشت branchname;

	*new_branch = branchname;
	اگر (guess_remote) {
		ساختار object_id oid;
		نویسه *remote;
		صحیح رقم_matches = 0;
		ساختار string_list matched_remote_names = STRING_LIST_INIT_DUP;

		remote = unique_tracking_name(*new_branch, &oid, &رقم_matches,
					      &matched_remote_names);
		اگر (!remote && رقم_matches > 1) {
			اگر (!opts->quiet &&
			    advice_enabled(ADVICE_CHECKOUT_AMBIGUOUS_REMOTE_BRANCH_NAME))
				advise_disambiguating_remotes(path, *new_branch,
							      &matched_remote_names);
			die(_("'%s' matched multiple (%d) remote tracking branches"),
			    *new_branch, رقم_matches);
		}
		string_list_clear(&matched_remote_names, 0);
		بازگشت remote;
	}
	بازگشت پوچ;
}

ایستا صحیح add(صحیح ac, ثابت نویسه **av, ثابت نویسه *prefix,
	       ساختار repository *repo UNUSED)
{
	ساختار add_opts opts;
	ثابت نویسه *new_branch_force = پوچ;
	نویسه *path;
	ثابت نویسه *branch;
	نویسه *branch_to_free = پوچ;
	نویسه *new_branch_to_free = پوچ;
	ثابت نویسه *new_branch = پوچ;
	نویسه *opt_track = پوچ;
	ثابت نویسه *lock_reason = پوچ;
	صحیح keep_locked = 0;
	صحیح used_new_branch_options;
	ساختار option options[] = {
		OPT__FORCE(&opts.force,
			   N_("checkout <branch> even if already checked out in other worktree"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_STRING('b', پوچ, &new_branch, N_("branch"),
			   N_("create a new branch")),
		OPT_STRING('B', پوچ, &new_branch_force, N_("branch"),
			   N_("create or reset a branch")),
		OPT_BOOL(0, "orphan", &opts.orphan, N_("create unborn branch")),
		OPT_BOOL('d', "detach", &opts.detach, N_("detach HEAD at named commit")),
		OPT_BOOL(0, "checkout", &opts.checkout, N_("populate the new working tree")),
		OPT_BOOL(0, "lock", &keep_locked, N_("keep the new working tree locked")),
		OPT_STRING(0, "reason", &lock_reason, N_("string"),
			   N_("reason for locking")),
		OPT__QUIET(&opts.quiet, N_("suppress progress reporting")),
		OPT_PASSTHRU(0, "track", &opt_track, پوچ,
			     N_("set up tracking mode (see git-branch(1))"),
			     PARSE_OPT_NOARG | PARSE_OPT_OPTARG),
		OPT_BOOL(0, "guess-remote", &guess_remote,
			 N_("try to match the new branch name with a remote-tracking branch")),
		OPT_BOOL(0, "relative-paths", &opts.relative_paths,
			 N_("use relative paths for worktrees")),
		OPT_END()
	};
	صحیح ret;

	memset(&opts, 0, اندازه(opts));
	opts.checkout = 1;
	opts.relative_paths = use_relative_paths;
	ac = parse_options(ac, av, prefix, options, git_worktree_add_usage, 0);
	اگر (!!opts.detach + !!new_branch + !!new_branch_force > 1)
		die(_("options '%s', '%s', and '%s' cannot be used together"), "-b", "-B", "--detach");
	اگر (opts.detach && opts.orphan)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--detach");
	اگر (opts.orphan && opt_track)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--track");
	اگر (opts.orphan && !opts.checkout)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--no-checkout");
	اگر (opts.orphan && ac == 2)
		die(_("option '%s' and commit-ish cannot be used together"),
		    "--orphan");
	اگر (lock_reason && !keep_locked)
		die(_("the option '%s' requires '%s'"), "--reason", "--lock");
	اگر (lock_reason)
		opts.keep_locked = lock_reason;
	وگرنه اگر (keep_locked)
		opts.keep_locked = _("added with --lock");

	اگر (ac < 1 || ac > 2)
		usage_with_options(git_worktree_add_usage, options);

	path = prefix_filename(prefix, av[0]);
	branch = ac < 2 ? "HEAD" : av[1];
	used_new_branch_options = new_branch || new_branch_force;

	اگر (!strcmp(branch, "-"))
		branch = "@{-1}";

	اگر (new_branch_force) {
		ساختار strbuf symref = STRBUF_INIT;

		new_branch = new_branch_force;

		اگر (!opts.force &&
		    !check_branch_ref(the_repository, &symref, new_branch) &&
		    refs_ref_exists(get_main_ref_store(the_repository), symref.buf))
			die_if_checked_out(symref.buf, 0);
		strbuf_release(&symref);
	}

	اگر (opts.orphan && !new_branch) {
		new_branch = new_branch_to_free = worktree_basename_dup(path);
	} وگرنه اگر (opts.orphan) {
		; /* no-op */
	} وگرنه اگر (opts.detach) {
		/* Check HEAD */
		اگر (!strcmp(branch, "HEAD"))
			can_use_local_refs(&opts);
	} وگرنه اگر (ac < 2 && new_branch) {
		/* DWIM: Infer --orphan when repo has no refs. */
		opts.orphan = dwim_orphan(&opts, !!opt_track, 0);
	} وگرنه اگر (ac < 2) {
		/* DWIM: Guess branch name from path. */
		نویسه *s = dwim_branch(&opts, path, &new_branch_to_free);
		اگر (s)
			branch = branch_to_free = s;
		new_branch = new_branch_to_free;

		/* DWIM: Infer --orphan when repo has no refs. */
		opts.orphan = (!s) && dwim_orphan(&opts, !!opt_track, 1);
	} وگرنه اگر (ac == 2 && new_branch) {
		اگر (!strcmp(branch, "HEAD"))
			can_use_local_refs(&opts);
	} وگرنه اگر (ac == 2) {
		ساختار commit *commit;

		commit = lookup_commit_reference_by_name(branch);
		اگر (!commit) {
			ساختار object_id oid;
			نویسه *remote;
			صحیح رقم_matches = 0;
			ساختار string_list matched_remote_names = STRING_LIST_INIT_DUP;

			remote = unique_tracking_name(branch, &oid, &رقم_matches,
						      &matched_remote_names);
			اگر (remote) {
				new_branch = branch;
				branch = new_branch_to_free = remote;
			} وگرنه اگر (رقم_matches > 1) {
				اگر (!opts.quiet &&
				    advice_enabled(ADVICE_CHECKOUT_AMBIGUOUS_REMOTE_BRANCH_NAME))
					advise_disambiguating_remotes(path, branch,
								      &matched_remote_names);
				die(_("'%s' matched multiple (%d) remote tracking branches"),
				    branch, رقم_matches);
			}
			string_list_clear(&matched_remote_names, 0);
		}

		اگر (!strcmp(branch, "HEAD"))
			can_use_local_refs(&opts);

	}

	اگر (!opts.orphan && !lookup_commit_reference_by_name(branch)) {
		صحیح attempt_hint = !opts.quiet && (ac < 2);
		اگر (attempt_hint && used_new_branch_options) {
			advise_if_enabled(ADVICE_WORKTREE_ADD_ORPHAN,
				WORKTREE_ADD_ORPHAN_WITH_DASH_B_HINT_TEXT,
				new_branch, path);
		} وگرنه اگر (attempt_hint) {
			advise_if_enabled(ADVICE_WORKTREE_ADD_ORPHAN,
				WORKTREE_ADD_ORPHAN_NO_DASH_B_HINT_TEXT, path);
		}
		die(_("invalid reference: %s"), branch);
	}

	اگر (!opts.quiet)
		print_preparing_worktree_سطر(opts.detach, branch, new_branch, !!new_branch_force);

	اگر (opts.orphan) {
		branch = new_branch;
	} وگرنه اگر (new_branch) {
		ساختار child_process cp = CHILD_PROCESS_INIT;
		cp.git_cmd = 1;
		strvec_افزودن(&cp.args, "branch");
		اگر (new_branch_force)
			strvec_افزودن(&cp.args, "--force");
		اگر (opts.quiet)
			strvec_افزودن(&cp.args, "--quiet");
		strvec_افزودن(&cp.args, new_branch);
		strvec_افزودن(&cp.args, branch);
		اگر (opt_track)
			strvec_افزودن(&cp.args, opt_track);
		اگر (run_command(&cp)) {
			ret = -1;
			برو cleanup;
		}
		branch = new_branch;
	} وگرنه اگر (opt_track) {
		die(_("--[no-]track can only be used if a new branch is created"));
	}

	ret = add_worktree(path, branch, &opts);
cleanup:
	آزاد(path);
	آزاد(opt_track);
	آزاد(branch_to_free);
	آزاد(new_branch_to_free);
	بازگشت ret;
}

ایستا تهی show_worktree_porcelain(ساختار worktree *wt, صحیح سطر_terminator)
{
	ثابت نویسه *reason;

	printf("worktree %s%c", wt->path, سطر_terminator);
	اگر (wt->is_bare)
		printf("bare%c", سطر_terminator);
	وگرنه {
		printf("HEAD %s%c", oid_to_hex(&wt->head_oid), سطر_terminator);
		اگر (wt->is_detached)
			printf("detached%c", سطر_terminator);
		وگرنه اگر (wt->head_ref)
			printf("branch %s%c", wt->head_ref, سطر_terminator);
	}

	reason = worktree_lock_reason(wt);
	اگر (reason) {
		fputs("locked", stdout);
		اگر (*reason) {
			fputc(' ', stdout);
			write_name_quoted(reason, stdout, سطر_terminator);
		} وگرنه {
			fputc(سطر_terminator, stdout);
		}
	}

	reason = worktree_prune_reason(wt, expire);
	اگر (reason)
		printf("prunable %s%c", reason, سطر_terminator);

	fputc(سطر_terminator, stdout);
}

ساختار worktree_display {
	نویسه *path;
	صحیح width;
};

ایستا تهی show_worktree(ساختار worktree *wt, ساختار worktree_display *display,
			  صحیح path_maxwidth, صحیح abbrev_len)
{
	ساختار strbuf sb = STRBUF_INIT;
	ثابت نویسه *reason;

	strbuf_addf(&sb, "%s%*s", display->path, 1 + path_maxwidth - display->width, "");
	اگر (wt->is_bare)
		strbuf_addstr(&sb, "(bare)");
	وگرنه {
		strbuf_addf(&sb, "%-*s ", abbrev_len,
				repo_find_unique_abbrev(the_repository, &wt->head_oid, DEFAULT_ABBREV));
		اگر (wt->is_detached)
			strbuf_addstr(&sb, "(detached HEAD)");
		وگرنه اگر (wt->head_ref) {
			نویسه *ref = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
								 wt->head_ref,
								 0);
			strbuf_addf(&sb, "[%s]", ref);
			آزاد(ref);
		} وگرنه
			strbuf_addstr(&sb, "(error)");
	}

	reason = worktree_lock_reason(wt);
	اگر (verbose && reason && *reason)
		strbuf_addf(&sb, "\n\tlocked: %s", reason);
	وگرنه اگر (reason)
		strbuf_addstr(&sb, " locked");

	reason = worktree_prune_reason(wt, expire);
	اگر (verbose && reason)
		strbuf_addf(&sb, "\n\tprunable: %s", reason);
	وگرنه اگر (reason)
		strbuf_addstr(&sb, " prunable");

	printf("%s\n", sb.buf);
	strbuf_release(&sb);
}

ایستا تهی measure_widths(ساختار worktree **wt, صحیح *abbrev,
			   ساختار worktree_display **d, صحیح *maxwidth)
{
	صحیح i, display_alloc = 0;
	ساختار worktree_display *display = پوچ;
	ساختار strbuf buf = STRBUF_INIT;

	برای (i = 0; wt[i]; i++) {
		صحیح sha1_len;
		ALLOC_GROW(display, i + 1, display_alloc);
		quote_path(wt[i]->path, پوچ, &buf, 0);
		display[i].width = utf8_strwidth(buf.buf);
		display[i].path = strbuf_detach(&buf, پوچ);

		اگر (display[i].width > *maxwidth)
			*maxwidth = display[i].width;
		sha1_len = strlen(repo_find_unique_abbrev(the_repository, &wt[i]->head_oid, *abbrev));
		اگر (sha1_len > *abbrev)
			*abbrev = sha1_len;
	}
	*d = display;
}

ایستا صحیح pathcmp(ثابت تهی *a_, ثابت تهی *b_)
{
	ثابت ساختار worktree *ثابت *a = a_;
	ثابت ساختار worktree *ثابت *b = b_;
	بازگشت fspathcmp((*a)->path, (*b)->path);
}

ایستا تهی pathsort(ساختار worktree **wt)
{
	صحیح n = 0;
	ساختار worktree **p = wt;

	وقتی (*p++)
		n++;
	QSORT(wt, n, pathcmp);
}

ایستا صحیح list(صحیح ac, ثابت نویسه **av, ثابت نویسه *prefix,
		ساختار repository *repo UNUSED)
{
	صحیح porcelain = 0;
	صحیح سطر_terminator = '\n';

	ساختار option options[] = {
		OPT_BOOL(0, "porcelain", &porcelain, N_("machine-readable output")),
		OPT__VERBOSE(&verbose, N_("show extended annotations and reasons, if available")),
		OPT_EXPIRY_DATE(0, "expire", &expire,
				N_("add 'prunable' annotation to missing worktrees older than <time>")),
		OPT_SET_INT('z', پوچ, &سطر_terminator,
			    N_("terminate records with a NUL character"), '\0'),
		OPT_END()
	};

	expire = TIME_MAX;
	ac = parse_options(ac, av, prefix, options, git_worktree_list_usage, 0);
	اگر (ac)
		usage_with_options(git_worktree_list_usage, options);
	وگرنه اگر (verbose && porcelain)
		die(_("options '%s' and '%s' cannot be used together"), "--verbose", "--porcelain");
	وگرنه اگر (!سطر_terminator && !porcelain)
		die(_("the option '%s' requires '%s'"), "-z", "--porcelain");
	وگرنه {
		ساختار worktree **worktrees = get_worktrees(the_repository);
		صحیح path_maxwidth = 0, abbrev = DEFAULT_ABBREV, i;
		ساختار worktree_display *display = پوچ;

		/* sort worktrees by path but keep main worktree at top */
		pathsort(worktrees + 1);

		اگر (!porcelain)
			measure_widths(worktrees, &abbrev,
				       &display, &path_maxwidth);

		برای (i = 0; worktrees[i]; i++) {
			اگر (porcelain)
				show_worktree_porcelain(worktrees[i],
							سطر_terminator);
			وگرنه
				show_worktree(worktrees[i],
					      &display[i], path_maxwidth, abbrev);
		}
		برای (i = 0; display && worktrees[i]; i++)
			آزاد(display[i].path);
		آزاد(display);
		free_worktrees(worktrees);
	}
	بازگشت 0;
}

ایستا صحیح lock_worktree(صحیح ac, ثابت نویسه **av, ثابت نویسه *prefix,
			 ساختار repository *repo UNUSED)
{
	ثابت نویسه *reason = "", *old_reason;
	ساختار option options[] = {
		OPT_STRING(0, "reason", &reason, N_("string"),
			   N_("reason for locking")),
		OPT_END()
	};
	ساختار worktree **worktrees, *wt;
	نویسه *path;

	ac = parse_options(ac, av, prefix, options, git_worktree_lock_usage, 0);
	اگر (ac != 1)
		usage_with_options(git_worktree_lock_usage, options);

	worktrees = get_worktrees(the_repository);
	wt = find_worktree(worktrees, prefix, av[0]);
	اگر (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	اگر (is_main_worktree(wt))
		die(_("The main working tree cannot be locked or unlocked"));

	old_reason = worktree_lock_reason(wt);
	اگر (old_reason) {
		اگر (*old_reason)
			die(_("'%s' is already locked, reason: %s"),
			    av[0], old_reason);
		die(_("'%s' is already locked"), av[0]);
	}

	path = repo_common_path(the_repository, "worktrees/%s/locked", wt->id);
	write_file(path, "%s", reason);

	free_worktrees(worktrees);
	آزاد(path);
	بازگشت 0;
}

ایستا صحیح unlock_worktree(صحیح ac, ثابت نویسه **av, ثابت نویسه *prefix,
			   ساختار repository *repo UNUSED)
{
	ساختار option options[] = {
		OPT_END()
	};
	ساختار worktree **worktrees, *wt;
	نویسه *path;
	صحیح ret;

	ac = parse_options(ac, av, prefix, options, git_worktree_unlock_usage, 0);
	اگر (ac != 1)
		usage_with_options(git_worktree_unlock_usage, options);

	worktrees = get_worktrees(the_repository);
	wt = find_worktree(worktrees, prefix, av[0]);
	اگر (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	اگر (is_main_worktree(wt))
		die(_("The main working tree cannot be locked or unlocked"));
	اگر (!worktree_lock_reason(wt))
		die(_("'%s' is not locked"), av[0]);

	path = repo_common_path(the_repository, "worktrees/%s/locked", wt->id);
	ret = unlink_or_warn(path);

	free_worktrees(worktrees);
	آزاد(path);
	بازگشت ret;
}

ایستا تهی validate_no_submodules(ثابت ساختار worktree *wt)
{
	ساختار index_state istate = INDEX_STATE_INIT(the_repository);
	ساختار strbuf path = STRBUF_INIT;
	صحیح i, found_submodules = 0;
	نویسه *wt_gitdir;

	wt_gitdir = get_worktree_git_dir(wt);

	اگر (is_directory(worktree_git_path(wt, "modules"))) {
		/*
		 * There could be false positives, e.g. the "modules"
		 * directory exists but is empty. But it's a rare case and
		 * this simpler check is probably good enough for now.
		 */
		found_submodules = 1;
	} وگرنه اگر (read_index_from(&istate, worktree_git_path(wt, "index"),
				   wt_gitdir) > 0) {
		برای (i = 0; i < istate.cache_nr; i++) {
			ساختار cache_entry *ce = istate.cache[i];
			صحیح err;

			اگر (!S_ISGITLINK(ce->ce_mode))
				ادامه;

			strbuf_reset(&path);
			strbuf_addf(&path, "%s/%s", wt->path, ce->name);
			اگر (!is_submodule_populated_gently(path.buf, &err))
				ادامه;

			found_submodules = 1;
			بشکن;
		}
	}
	discard_index(&istate);
	strbuf_release(&path);
	آزاد(wt_gitdir);

	اگر (found_submodules)
		die(_("working trees containing submodules cannot be moved or removed"));
}

ایستا صحیح move_worktree(صحیح ac, ثابت نویسه **av, ثابت نویسه *prefix,
			 ساختار repository *repo UNUSED)
{
	صحیح force = 0;
	ساختار option options[] = {
		OPT__FORCE(&force,
			 N_("force move even if worktree is dirty or locked"),
			 PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "relative-paths", &use_relative_paths,
			 N_("use relative paths for worktrees")),
		OPT_END()
	};
	ساختار worktree **worktrees, *wt;
	ساختار strbuf dst = STRBUF_INIT;
	ساختار strbuf errmsg = STRBUF_INIT;
	ثابت نویسه *reason = پوچ;
	نویسه *path;

	ac = parse_options(ac, av, prefix, options, git_worktree_move_usage,
			   0);
	اگر (ac != 2)
		usage_with_options(git_worktree_move_usage, options);

	path = prefix_filename(prefix, av[1]);
	strbuf_addstr(&dst, path);
	آزاد(path);

	worktrees = get_worktrees(the_repository);
	wt = find_worktree(worktrees, prefix, av[0]);
	اگر (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	اگر (is_main_worktree(wt))
		die(_("'%s' is a main working tree"), av[0]);
	اگر (is_directory(dst.buf)) {
		ثابت نویسه *sep = find_last_dir_sep(wt->path);

		اگر (!sep)
			die(_("could not figure out destination name from '%s'"),
			    wt->path);
		strbuf_trim_trailing_dir_sep(&dst);
		strbuf_addstr(&dst, sep);
	}
	check_candidate_path(dst.buf, force, worktrees, "move");

	validate_no_submodules(wt);

	اگر (force < 2)
		reason = worktree_lock_reason(wt);
	اگر (reason) {
		اگر (*reason)
			die(_("cannot move a locked working tree, lock reason: %s\nuse 'move -f -f' to override or unlock first"),
			    reason);
		die(_("cannot move a locked working tree;\nuse 'move -f -f' to override or unlock first"));
	}
	اگر (validate_worktree(wt, &errmsg, 0))
		die(_("validation failed, cannot move working tree: %s"),
		    errmsg.buf);
	strbuf_release(&errmsg);

	اگر (rename(wt->path, dst.buf) == -1)
		die_errno(_("failed to move '%s' to '%s'"), wt->path, dst.buf);

	update_worktree_location(wt, dst.buf, use_relative_paths);

	strbuf_release(&dst);
	free_worktrees(worktrees);
	بازگشت 0;
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
ایستا تهی check_clean_worktree(ساختار worktree *wt,
				 ثابت نویسه *original_path)
{
	ساختار child_process cp;
	نویسه buf[1];
	صحیح ret;

	/*
	 * Until we sort this out, all submodules are "dirty" and
	 * will abort this function.
	 */
	validate_no_submodules(wt);

	child_process_init(&cp);
	strvec_pushf(&cp.env, "%s=%s/.git",
		     GIT_DIR_ENVIRONMENT, wt->path);
	strvec_pushf(&cp.env, "%s=%s",
		     GIT_WORK_درخت_ENVIRONMENT, wt->path);
	strvec_pushl(&cp.args, "status",
		     "--porcelain", "--ignore-submodules=none",
		     پوچ);
	cp.git_cmd = 1;
	cp.dir = wt->path;
	cp.out = -1;
	ret = start_command(&cp);
	اگر (ret)
		die_errno(_("failed to run 'git status' on '%s'"),
			  original_path);
	ret = xread(cp.out, buf, اندازه(buf));
	اگر (ret)
		die(_("'%s' contains modified or untracked files, use --force to delete it"),
		    original_path);
	close(cp.out);
	ret = finish_command(&cp);
	اگر (ret)
		die_errno(_("failed to run 'git status' on '%s', code %d"),
			  original_path, ret);
}

ایستا صحیح delete_git_work_درخت(ساختار worktree *wt)
{
	ساختار strbuf sb = STRBUF_INIT;
	صحیح ret = 0;

	strbuf_addstr(&sb, wt->path);
	اگر (remove_dir_recursively(&sb, 0)) {
		error_errno(_("failed to delete '%s'"), sb.buf);
		ret = -1;
	}
	strbuf_release(&sb);
	بازگشت ret;
}

ایستا صحیح remove_worktree(صحیح ac, ثابت نویسه **av, ثابت نویسه *prefix,
			   ساختار repository *repo UNUSED)
{
	صحیح force = 0;
	ساختار option options[] = {
		OPT__FORCE(&force,
			 N_("force removal even if worktree is dirty or locked"),
			 PARSE_OPT_NOCOMPLETE),
		OPT_END()
	};
	ساختار worktree **worktrees, *wt;
	ساختار strbuf errmsg = STRBUF_INIT;
	ثابت نویسه *reason = پوچ;
	صحیح ret = 0;

	ac = parse_options(ac, av, prefix, options, git_worktree_remove_usage, 0);
	اگر (ac != 1)
		usage_with_options(git_worktree_remove_usage, options);

	worktrees = get_worktrees(the_repository);
	wt = find_worktree(worktrees, prefix, av[0]);
	اگر (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	اگر (is_main_worktree(wt))
		die(_("'%s' is a main working tree"), av[0]);
	اگر (force < 2)
		reason = worktree_lock_reason(wt);
	اگر (reason) {
		اگر (*reason)
			die(_("cannot remove a locked working tree, lock reason: %s\nuse 'remove -f -f' to override or unlock first"),
			    reason);
		die(_("cannot remove a locked working tree;\nuse 'remove -f -f' to override or unlock first"));
	}
	اگر (validate_worktree(wt, &errmsg, WT_VALIDATE_WORKTREE_MISSING_OK))
		die(_("validation failed, cannot remove working tree: %s"),
		    errmsg.buf);
	strbuf_release(&errmsg);

	اگر (file_exists(wt->path)) {
		اگر (!force)
			check_clean_worktree(wt, av[0]);

		ret |= delete_git_work_درخت(wt);
	}
	/*
	 * continue on even if ret is non-zero, there's no going back
	 * from here.
	 */
	ret |= delete_git_dir(wt->id);
	delete_worktrees_dir_if_empty();

	free_worktrees(worktrees);
	بازگشت ret;
}

ایستا تهی report_repair(صحیح iserr, ثابت نویسه *path, ثابت نویسه *پیام, تهی *cb_data)
{
	اگر (!iserr) {
		fprintf_ln(stderr, _("repair: %s: %s"), پیام, path);
	} وگرنه {
		صحیح *exit_status = (صحیح *)cb_data;
		fprintf_ln(stderr, _("error: %s: %s"), پیام, path);
		*exit_status = 1;
	}
}

ایستا صحیح repair(صحیح ac, ثابت نویسه **av, ثابت نویسه *prefix,
		  ساختار repository *repo UNUSED)
{
	ثابت نویسه **p;
	ثابت نویسه *self[] = { ".", پوچ };
	ساختار option options[] = {
		OPT_BOOL(0, "relative-paths", &use_relative_paths,
			 N_("use relative paths for worktrees")),
		OPT_END()
	};
	صحیح rc = 0;

	ac = parse_options(ac, av, prefix, options, git_worktree_repair_usage, 0);
	p = ac > 0 ? av : self;
	برای (; *p; p++)
		repair_worktree_at_path(the_repository, *p, report_repair,
					&rc, use_relative_paths);
	repair_worktrees(the_repository, report_repair, &rc, use_relative_paths);
	بازگشت rc;
}

صحیح cmd_worktree(صحیح ac,
		 ثابت نویسه **av,
		 ثابت نویسه *prefix,
		 ساختار repository *repo)
{
	parse_opt_subcommand_fn *fn = پوچ;
	ساختار option options[] = {
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

	repo_config(the_repository, git_worktree_config, پوچ);

	اگر (!prefix)
		prefix = "";

	ac = parse_options(ac, av, prefix, options, git_worktree_usage, 0);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	بازگشت fn(ac, av, prefix, repo);
}
