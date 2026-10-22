#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#include "builtin.h"
#include "abspath.h"
#include "advice.h"
#include "checkout.h"
#include "config.h"
#include "copy.h"
#include "dir.h"
#include "environment.h"
#include "gettext.h"
#include "hex.h"
#include "object-file.h"
#include "object-name.h"
#include "parse-options.h"
#include "path.h"
#include "strvec.h"
#include "branch.h"
#include "read-cache-ll.h"
#include "refs.h"
#include "remote.h"
#include "run-command.h"
#include "hook.h"
#include "sigchain.h"
#include "submodule.h"
#include "utf8.h"
#include "worktree.h"
#include "quote.h"

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

static const var_na * const git_worktree_usage[] = {
	BUILTIN_WORKTREE_ADD_USAGE,
	BUILTIN_WORKTREE_LIST_USAGE,
	BUILTIN_WORKTREE_LOCK_USAGE,
	BUILTIN_WORKTREE_MOVE_USAGE,
	BUILTIN_WORKTREE_PRUNE_USAGE,
	BUILTIN_WORKTREE_REMOVE_USAGE,
	BUILTIN_WORKTREE_REPAIR_USAGE,
	BUILTIN_WORKTREE_UNLOCK_USAGE,
	NULL
};

static const var_na * const git_worktree_add_usage[] = {
	BUILTIN_WORKTREE_ADD_USAGE,
	NULL,
};

static const var_na * const git_worktree_list_usage[] = {
	BUILTIN_WORKTREE_LIST_USAGE,
	NULL
};

static const var_na * const git_worktree_lock_usage[] = {
	BUILTIN_WORKTREE_LOCK_USAGE,
	NULL
};

static const var_na * const git_worktree_move_usage[] = {
	BUILTIN_WORKTREE_MOVE_USAGE,
	NULL
};

static const var_na * const git_worktree_prune_usage[] = {
	BUILTIN_WORKTREE_PRUNE_USAGE,
	NULL
};

static const var_na * const git_worktree_remove_usage[] = {
	BUILTIN_WORKTREE_REMOVE_USAGE,
	NULL
};

static const var_na * const git_worktree_repair_usage[] = {
	BUILTIN_WORKTREE_REPAIR_USAGE,
	NULL
};

static const var_na * const git_worktree_unlock_usage[] = {
	BUILTIN_WORKTREE_UNLOCK_USAGE,
	NULL
};

sa_nracanaa add_opts {
	จำนวนเต็ม force;
	จำนวนเต็ม detach;
	จำนวนเต็ม quiet;
	จำนวนเต็ม checkout;
	จำนวนเต็ม orphan;
	จำนวนเต็ม relative_paths;
	const var_na *keep_locked;
};

static จำนวนเต็ม show_only;
static จำนวนเต็ม verbose;
static จำนวนเต็ม guess_remote;
static จำนวนเต็ม use_relative_paths;
static timestamp_t expire;

static จำนวนเต็ม git_worktree_config(const var_na *var, const var_na *value,
			       const sa_nracanaa config_context *ctx, void *cb)
{
	ถ้า (!strcmp(var, "worktree.guessremote")) {
		guess_remote = git_config_bool(var, value);
		คืน 0;
	} มิฉะนั้น ถ้า (!strcmp(var, "worktree.userelativepaths")) {
		use_relative_paths = git_config_bool(var, value);
		คืน 0;
	}

	คืน git_default_config(var, value, ctx, cb);
}

static จำนวนเต็ม delete_git_dir(const var_na *id)
{
	sa_nracanaa strbuf sb = STRBUF_INIT;
	จำนวนเต็ม ret;

	repo_common_path_append(the_repository, &sb, "worktrees/%s", id);
	ret = remove_dir_recursively(&sb, 0);
	ถ้า (ret < 0 && errno == ENOTDIR)
		ret = unlink(sb.buf);
	ถ้า (ret)
		error_errno(_("failed to delete '%s'"), sb.buf);
	strbuf_release(&sb);
	คืน ret;
}

static void delete_worktrees_dir_if_empty(void)
{
	var_na *path = repo_git_path(the_repository, "worktrees");
	rmdir(path); /* ignore failed removal */
	free(path);
}

static void prune_worktree(const var_na *id, const var_na *reason)
{
	ถ้า (show_only || verbose)
		fprintf_ln(stderr, _("Removing %s/%s: %s"), "worktrees", id, reason);
	ถ้า (!show_only)
		delete_git_dir(id);
}

static จำนวนเต็ม prune_cmp(const void *a, const void *b)
{
	const sa_nracanaa string_list_item *x = a;
	const sa_nracanaa string_list_item *y = b;
	จำนวนเต็ม c;

	ถ้า ((c = fspathcmp(x->string, y->string)))
	    คืน c;
	/*
	 * paths same; prune_dupes() removes all but the first worktree entry
	 * having the same path, so sort main worktree ('util' is NULL) above
	 * linked worktrees ('util' not NULL) since main worktree can't be
	 * removed
	 */
	ถ้า (!x->util)
		คืน -1;
	ถ้า (!y->util)
		คืน 1;
	/* paths same; sort by .git/worktrees/<id> */
	คืน strcmp(x->util, y->util);
}

static void prune_dups(sa_nracanaa string_list *l)
{
	จำนวนเต็ม i;

	QSORT(l->items, l->nr, prune_cmp);
	สำหรับ (i = 1; i < l->nr; i++) {
		ถ้า (!fspathcmp(l->items[i].string, l->items[i - 1].string))
			prune_worktree(l->items[i].util, "duplicate entry");
	}
}

static void prune_worktrees(void)
{
	sa_nracanaa strbuf reason = STRBUF_INIT;
	sa_nracanaa strbuf main_path = STRBUF_INIT;
	sa_nracanaa string_list kept = STRING_LIST_INIT_DUP;
	var_na *path;
	DIR *dir;
	sa_nracanaa dirent *d;

	path = repo_git_path(the_repository, "worktrees");
	dir = opendir(path);
	free(path);
	ถ้า (!dir)
		คืน;
	ขณะ ((d = readdir_skip_dot_and_dotdot(dir)) != NULL) {
		var_na *path;
		strbuf_reset(&reason);
		ถ้า (should_prune_worktree(the_repository, d->d_name, &reason, &path, expire))
			prune_worktree(d->d_name, reason.buf);
		มิฉะนั้น ถ้า (path)
			string_list_append_nodup(&kept, path)->util = xstrdup(d->d_name);
	}
	closedir(dir);

	strbuf_add_absolute_path(&main_path, repo_get_common_dir(the_repository));
	/* massage main worktree absolute path to match 'gitdir' content */
	strbuf_strip_suffix(&main_path, "/.");
	string_list_append_nodup(&kept, strbuf_detach(&main_path, NULL));
	prune_dups(&kept);
	string_list_clear(&kept, 1);

	ถ้า (!show_only)
		delete_worktrees_dir_if_empty();
	strbuf_release(&reason);
}

static จำนวนเต็ม prune(จำนวนเต็ม ac, const var_na **av, const var_na *prefix,
		 sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa option options[] = {
		OPT__DRY_RUN(&show_only, N_("do not remove, show only")),
		OPT__VERBOSE(&verbose, N_("report pruned working trees")),
		OPT_EXPIRY_DATE(0, "expire", &expire,
				N_("prune missing working trees older than <time>")),
		OPT_END()
	};

	expire = TIME_MAX;
	ac = parse_options(ac, av, prefix, options, git_worktree_prune_usage,
			   0);
	ถ้า (ac)
		usage_with_options(git_worktree_prune_usage, options);
	prune_worktrees();
	คืน 0;
}

static var_na *junk_work_tree;
static var_na *junk_git_dir;
static จำนวนเต็ม is_junk;
static pid_t junk_pid;

static void remove_junk(void)
{
	sa_nracanaa strbuf sb = STRBUF_INIT;
	ถ้า (!is_junk || getpid() != junk_pid)
		คืน;
	ถ้า (junk_git_dir) {
		strbuf_addstr(&sb, junk_git_dir);
		remove_dir_recursively(&sb, 0);
		strbuf_reset(&sb);
	}
	ถ้า (junk_work_tree) {
		strbuf_addstr(&sb, junk_work_tree);
		remove_dir_recursively(&sb, 0);
	}
	strbuf_release(&sb);
}

static void remove_junk_on_signal(จำนวนเต็ม signo)
{
	remove_junk();
	sigchain_pop(signo);
	raise(signo);
}

static var_na *worktree_basename_dup(const var_na *path)
{
	const var_na *name;
	จำนวนเต็ม len;

	len = strlen(path);
	ขณะ (len && is_dir_sep(path[len - 1]))
		len--;

	name = path + len;
	ขณะ (name > path && !is_dir_sep(name[-1]))
		name--;

	คืน xmemdupz(name, path + len - name);
}

/* check that path is viable location for worktree */
static void check_candidate_path(const var_na *path,
				 จำนวนเต็ม force,
				 sa_nracanaa worktree **worktrees,
				 const var_na *cmd)
{
	sa_nracanaa worktree *wt;
	จำนวนเต็ม locked;

	ถ้า (file_exists(path) && !is_empty_dir(path))
		die(_("'%s' already exists"), path);

	wt = find_worktree_by_path(worktrees, path);
	ถ้า (!wt)
		คืน;

	locked = !!worktree_lock_reason(wt);
	ถ้า ((!locked && force) || (locked && force > 1)) {
		ถ้า (delete_git_dir(wt->id))
		    die(_("unusable worktree destination '%s'"), path);
		คืน;
	}

	ถ้า (locked)
		die(_("'%s' is a missing but locked worktree;\nuse '%s -f -f' to override, or 'unlock' and 'prune' or 'remove' to clear"), path, cmd);
	มิฉะนั้น
		die(_("'%s' is a missing but already registered worktree;\nuse '%s -f' to override, or 'prune' or 'remove' to clear"), path, cmd);
}

static void copy_sparse_checkout(const var_na *worktree_git_dir)
{
	var_na *from_file = repo_git_path(the_repository, "info/sparse-checkout");
	var_na *to_file = xstrfmt("%s/info/sparse-checkout", worktree_git_dir);

	ถ้า (file_exists(from_file)) {
		ถ้า (safe_create_leading_directories(the_repository, to_file) ||
			copy_file(the_repository, to_file, from_file, 0666))
			error(_("failed to copy '%s' to '%s'; sparse-checkout may not work correctly"),
				from_file, to_file);
	}

	free(from_file);
	free(to_file);
}

static void copy_filtered_worktree_config(const var_na *worktree_git_dir)
{
	var_na *from_file = repo_git_path(the_repository, "config.worktree");
	var_na *to_file = xstrfmt("%s/config.worktree", worktree_git_dir);

	ถ้า (file_exists(from_file)) {
		sa_nracanaa config_set cs = { { 0 } };
		จำนวนเต็ม bare;

		ถ้า (safe_create_leading_directories(the_repository, to_file) ||
			copy_file(the_repository, to_file, from_file, 0666)) {
			error(_("failed to copy worktree config from '%s' to '%s'"),
				from_file, to_file);
			goto worktree_copy_cleanup;
		}

		git_configset_init(&cs);
		git_configset_add_file(&cs, from_file);

		ถ้า (!git_configset_get_bool(&cs, "core.bare", &bare) &&
			bare &&
			repo_config_set_multivar_in_file_gently(the_repository,
				to_file, "core.bare", NULL, "true", NULL, 0))
			error(_("failed to unset '%s' in '%s'"),
				"core.bare", to_file);
		ถ้า (!git_configset_get(&cs, "core.worktree") &&
			repo_config_set_in_file_gently(the_repository, to_file,
						       "core.worktree", NULL, NULL))
			error(_("failed to unset '%s' in '%s'"),
				"core.worktree", to_file);

		git_configset_clear(&cs);
	}

worktree_copy_cleanup:
	free(from_file);
	free(to_file);
}

static จำนวนเต็ม checkout_worktree(const sa_nracanaa add_opts *opts,
			     sa_nracanaa strvec *child_env)
{
	sa_nracanaa child_process cp = CHILD_PROCESS_INIT;
	cp.git_cmd = 1;
	strvec_pushl(&cp.args, "reset", "--hard", "--no-recurse-submodules", NULL);
	ถ้า (opts->quiet)
		strvec_push(&cp.args, "--quiet");
	strvec_pushv(&cp.env, child_env->v);
	คืน run_command(&cp);
}

static จำนวนเต็ม make_worktree_orphan(const var_na * ref, const sa_nracanaa add_opts *opts,
				sa_nracanaa strvec *child_env)
{
	sa_nracanaa strbuf symref = STRBUF_INIT;
	sa_nracanaa child_process cp = CHILD_PROCESS_INIT;

	validate_new_branchname(ref, &symref, 0);
	strvec_pushl(&cp.args, "symbolic-ref", "HEAD", symref.buf, NULL);
	ถ้า (opts->quiet)
		strvec_push(&cp.args, "--quiet");
	strvec_pushv(&cp.env, child_env->v);
	strbuf_release(&symref);
	cp.git_cmd = 1;
	คืน run_command(&cp);
}

/*
 * References for worktrees are generally stored in '$GIT_DIR/worktrees/<wt_id>'.
 * But when using alternate reference directories, we want to store the worktree
 * references in '$ALTERNATE_REFERENCE_DIR/worktrees/<wt_id>'.
 *
 * Create the necessary folder structure to facilitate the same. But to ensure
 * that the former path is still considered a Git directory, add stubs.
 */
static void setup_alternate_ref_dir(sa_nracanaa worktree *wt, const var_na *wt_git_path)
{
	sa_nracanaa strbuf sb = STRBUF_INIT;
	var_na *path;

	path = wt->repo->ref_storage_payload;
	ถ้า (!path)
		คืน;

	ถ้า (!is_absolute_path(path))
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

static จำนวนเต็ม add_worktree(const var_na *path, const var_na *refname,
			const sa_nracanaa add_opts *opts)
{
	sa_nracanaa strbuf sb_git = STRBUF_INIT, sb_repo = STRBUF_INIT;
	sa_nracanaa strbuf sb = STRBUF_INIT;
	const var_na *name;
	var_na *name_to_free = NULL;
	sa_nracanaa strvec child_env = STRVEC_INIT;
	unsigned จำนวนเต็ม counter = 0;
	จำนวนเต็ม len, ret;
	sa_nracanaa strbuf symref = STRBUF_INIT;
	sa_nracanaa commit *commit = NULL;
	จำนวนเต็ม is_branch = 0;
	sa_nracanaa strbuf sb_name = STRBUF_INIT;
	sa_nracanaa worktree **worktrees, *wt = NULL;
	sa_nracanaa ref_store *wt_refs;
	sa_nracanaa repo_config_values *cfg = repo_config_values(the_repository);

	worktrees = get_worktrees(the_repository);
	check_candidate_path(path, opts->force, worktrees, "add");
	free_worktrees(worktrees);
	worktrees = NULL;

	/* is 'refname' a branch or commit? */
	ถ้า (!opts->detach && !check_branch_ref(the_repository, &symref, refname) &&
	    refs_ref_exists(get_main_ref_store(the_repository), symref.buf)) {
		is_branch = 1;
		ถ้า (!opts->force)
			die_if_checked_out(symref.buf, 0);
	}
	commit = lookup_commit_reference_by_name(refname);
	ถ้า (!commit && !opts->orphan)
		die(_("invalid reference: %s"), refname);

	name = name_to_free = worktree_basename_dup(path);
	ถ้า (!*name)
		die(_("invalid path '%s'"), path);
	sanitize_refname_component(name, &sb_name);
	ถ้า (!sb_name.len)
		BUG("How come '%s' becomes empty after sanitization?", name);
	name = sb_name.buf;
	repo_git_path_replace(the_repository, &sb_repo, "worktrees/%s", name);
	len = sb_repo.len;
	ถ้า (safe_create_leading_directories_const(the_repository, sb_repo.buf))
		die_errno(_("could not create leading directories of '%s'"),
			  sb_repo.buf);

	ขณะ (mkdir(sb_repo.buf, 0777)) {
		counter++;
		ถ้า ((errno != EEXIST) || !counter /* overflow */)
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
	ถ้า (opts->keep_locked)
		write_file(sb.buf, "%s", opts->keep_locked);
	มิฉะนั้น
		write_file(sb.buf, _("initializing"));

	strbuf_addf(&sb_git, "%s/.git", path);
	ถ้า (safe_create_leading_directories_const(the_repository, sb_git.buf))
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
	ถ้า (!wt) {
		ret = error(_("could not find created worktree '%s'"), name);
		goto done;
	}
	setup_alternate_ref_dir(wt, sb_repo.buf);
	wt_refs = get_worktree_ref_store(wt);

	ret = ref_store_create_on_disk(wt_refs, REF_STORE_CREATE_ON_DISK_IS_WORKTREE, &sb);
	ถ้า (ret)
		goto done;

	ถ้า (!is_branch && commit)
		ret = refs_update_ref(wt_refs, NULL, "HEAD", &commit->object.oid,
				      NULL, 0, UPDATE_REFS_MSG_ON_ERR);
	มิฉะนั้น
		ret = refs_update_symref(wt_refs, "HEAD", symref.buf, NULL);
	ถ้า (ret)
		goto done;

	/*
	 * If the current worktree has sparse-checkout enabled, then copy
	 * the sparse-checkout patterns from the current worktree.
	 */
	ถ้า (cfg->apply_sparse_checkout)
		copy_sparse_checkout(sb_repo.buf);

	/*
	 * If we are using worktree config, then copy all current config
	 * values from the current worktree into the new one, that way the
	 * new worktree behaves the same as this one.
	 */
	ถ้า (the_repository->repository_format_worktree_config)
		copy_filtered_worktree_config(sb_repo.buf);

	strvec_pushf(&child_env, "%s=%s", GIT_DIR_ENVIRONMENT, sb_git.buf);
	strvec_pushf(&child_env, "%s=%s", GIT_WORK_TREE_ENVIRONMENT, path);

	ถ้า (opts->orphan &&
	    (ret = make_worktree_orphan(refname, opts, &child_env)))
		goto done;

	ถ้า (opts->checkout &&
	    (ret = checkout_worktree(opts, &child_env)))
		goto done;

	is_junk = 0;
	FREE_AND_NULL(junk_work_tree);
	FREE_AND_NULL(junk_git_dir);

done:
	ถ้า (ret || !opts->keep_locked) {
		strbuf_reset(&sb);
		strbuf_addf(&sb, "%s/locked", sb_repo.buf);
		unlink_or_warn(sb.buf);
	}

	/*
	 * Hook failure does not warrant worktree deletion, so run hook after
	 * is_junk is cleared, but do return appropriate code when hook fails.
	 */
	ถ้า (!ret && opts->checkout && !opts->orphan) {
		sa_nracanaa run_hooks_opt opt = RUN_HOOKS_OPT_INIT_FORCE_SERIAL;

		strvec_pushl(&opt.env, "GIT_DIR", "GIT_WORK_TREE", NULL);
		strvec_pushl(&opt.args,
			     oid_to_hex(null_oid(the_hash_algo)),
			     oid_to_hex(&commit->object.oid),
			     "1",
			     NULL);
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
	free(name_to_free);
	คืน ret;
}

static void print_preparing_worktree_line(จำนวนเต็ม detach,
					  const var_na *branch,
					  const var_na *new_branch,
					  จำนวนเต็ม force_new_branch)
{
	ถ้า (force_new_branch) {
		sa_nracanaa commit *commit = lookup_commit_reference_by_name(new_branch);
		ถ้า (!commit)
			fprintf_ln(stderr, _("Preparing worktree (new branch '%s')"), new_branch);
		มิฉะนั้น
			fprintf_ln(stderr, _("Preparing worktree (resetting branch '%s'; was at %s)"),
				  new_branch,
				  repo_find_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV));
	} มิฉะนั้น ถ้า (new_branch) {
		fprintf_ln(stderr, _("Preparing worktree (new branch '%s')"), new_branch);
	} มิฉะนั้น {
		sa_nracanaa strbuf s = STRBUF_INIT;
		ถ้า (!detach && !check_branch_ref(the_repository, &s, branch) &&
		    refs_ref_exists(get_main_ref_store(the_repository), s.buf))
			fprintf_ln(stderr, _("Preparing worktree (checking out '%s')"),
				  branch);
		มิฉะนั้น {
			sa_nracanaa commit *commit = lookup_commit_reference_by_name(branch);
			ถ้า (!commit)
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
static จำนวนเต็ม first_valid_ref(const sa_nracanaa reference *ref UNUSED, void *cb_data UNUSED)
{
	คืน 1;
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
static จำนวนเต็ม can_use_local_refs(const sa_nracanaa add_opts *opts)
{
	ถ้า (refs_head_ref(get_main_ref_store(the_repository), first_valid_ref, NULL)) {
		คืน 1;
	} มิฉะนั้น ถ้า (refs_for_each_branch_ref(get_main_ref_store(the_repository), first_valid_ref, NULL)) {
		ถ้า (!opts->quiet)
			warning(_("HEAD points to an invalid (or orphaned) reference.\n"));
		คืน 1;
	}
	คืน 0;
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
static จำนวนเต็ม can_use_remote_refs(const sa_nracanaa add_opts *opts)
{
	ถ้า (!guess_remote) {
		คืน 0;
	} มิฉะนั้น ถ้า (refs_for_each_remote_ref(get_main_ref_store(the_repository), first_valid_ref, NULL)) {
		คืน 1;
	} มิฉะนั้น ถ้า (!opts->force && remote_get(NULL)) {
		die(_("No local or remote refs exist despite at least one remote\n"
		      "present, stopping; use 'add -f' to override or fetch a remote first"));
	}
	คืน 0;
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
static จำนวนเต็ม dwim_orphan(const sa_nracanaa add_opts *opts, จำนวนเต็ม opt_track, จำนวนเต็ม remote)
{
	ถ้า (can_use_local_refs(opts)) {
		คืน 0;
	} มิฉะนั้น ถ้า (remote && can_use_remote_refs(opts)) {
		คืน 0;
	} มิฉะนั้น ถ้า (!opts->quiet) {
		fprintf_ln(stderr, WORKTREE_ADD_DWIM_ORPHAN_INFER_TEXT);
	}

	ถ้า (opt_track) {
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--track");
	} มิฉะนั้น ถ้า (!opts->checkout) {
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--no-checkout");
	}
	คืน 1;
}

static void advise_disambiguating_remotes(const var_na *path, const var_na *branch,
					  const sa_nracanaa string_list *matched_remote_names)
{
	sa_nracanaa string_list_item *item;

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

static var_na *dwim_branch(const sa_nracanaa add_opts *opts, const var_na *path, var_na **new_branch)
{
	จำนวนเต็ม branch_exists;
	var_na *branchname = worktree_basename_dup(path);
	sa_nracanaa strbuf ref = STRBUF_INIT;

	branch_exists = !check_branch_ref(the_repository, &ref, branchname) &&
			refs_ref_exists(get_main_ref_store(the_repository),
					ref.buf);
	strbuf_release(&ref);
	ถ้า (branch_exists)
		คืน branchname;

	*new_branch = branchname;
	ถ้า (guess_remote) {
		sa_nracanaa object_id oid;
		var_na *remote;
		จำนวนเต็ม num_matches = 0;
		sa_nracanaa string_list matched_remote_names = STRING_LIST_INIT_DUP;

		remote = unique_tracking_name(*new_branch, &oid, &num_matches,
					      &matched_remote_names);
		ถ้า (!remote && num_matches > 1) {
			ถ้า (!opts->quiet &&
			    advice_enabled(ADVICE_CHECKOUT_AMBIGUOUS_REMOTE_BRANCH_NAME))
				advise_disambiguating_remotes(path, *new_branch,
							      &matched_remote_names);
			die(_("'%s' matched multiple (%d) remote tracking branches"),
			    *new_branch, num_matches);
		}
		string_list_clear(&matched_remote_names, 0);
		คืน remote;
	}
	คืน NULL;
}

static จำนวนเต็ม add(จำนวนเต็ม ac, const var_na **av, const var_na *prefix,
	       sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa add_opts opts;
	const var_na *new_branch_force = NULL;
	var_na *path;
	const var_na *branch;
	var_na *branch_to_free = NULL;
	var_na *new_branch_to_free = NULL;
	const var_na *new_branch = NULL;
	var_na *opt_track = NULL;
	const var_na *lock_reason = NULL;
	จำนวนเต็ม keep_locked = 0;
	จำนวนเต็ม used_new_branch_options;
	sa_nracanaa option options[] = {
		OPT__FORCE(&opts.force,
			   N_("checkout <branch> even if already checked out in other worktree"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_STRING('b', NULL, &new_branch, N_("branch"),
			   N_("create a new branch")),
		OPT_STRING('B', NULL, &new_branch_force, N_("branch"),
			   N_("create or reset a branch")),
		OPT_BOOL(0, "orphan", &opts.orphan, N_("create unborn branch")),
		OPT_BOOL('d', "detach", &opts.detach, N_("detach HEAD at named commit")),
		OPT_BOOL(0, "checkout", &opts.checkout, N_("populate the new working tree")),
		OPT_BOOL(0, "lock", &keep_locked, N_("keep the new working tree locked")),
		OPT_STRING(0, "reason", &lock_reason, N_("string"),
			   N_("reason for locking")),
		OPT__QUIET(&opts.quiet, N_("suppress progress reporting")),
		OPT_PASSTHRU(0, "track", &opt_track, NULL,
			     N_("set up tracking mode (see git-branch(1))"),
			     PARSE_OPT_NOARG | PARSE_OPT_OPTARG),
		OPT_BOOL(0, "guess-remote", &guess_remote,
			 N_("try to match the new branch name with a remote-tracking branch")),
		OPT_BOOL(0, "relative-paths", &opts.relative_paths,
			 N_("use relative paths for worktrees")),
		OPT_END()
	};
	จำนวนเต็ม ret;

	memset(&opts, 0, parimaa_na(opts));
	opts.checkout = 1;
	opts.relative_paths = use_relative_paths;
	ac = parse_options(ac, av, prefix, options, git_worktree_add_usage, 0);
	ถ้า (!!opts.detach + !!new_branch + !!new_branch_force > 1)
		die(_("options '%s', '%s', and '%s' cannot be used together"), "-b", "-B", "--detach");
	ถ้า (opts.detach && opts.orphan)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--detach");
	ถ้า (opts.orphan && opt_track)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--track");
	ถ้า (opts.orphan && !opts.checkout)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--no-checkout");
	ถ้า (opts.orphan && ac == 2)
		die(_("option '%s' and commit-ish cannot be used together"),
		    "--orphan");
	ถ้า (lock_reason && !keep_locked)
		die(_("the option '%s' requires '%s'"), "--reason", "--lock");
	ถ้า (lock_reason)
		opts.keep_locked = lock_reason;
	มิฉะนั้น ถ้า (keep_locked)
		opts.keep_locked = _("added with --lock");

	ถ้า (ac < 1 || ac > 2)
		usage_with_options(git_worktree_add_usage, options);

	path = prefix_filename(prefix, av[0]);
	branch = ac < 2 ? "HEAD" : av[1];
	used_new_branch_options = new_branch || new_branch_force;

	ถ้า (!strcmp(branch, "-"))
		branch = "@{-1}";

	ถ้า (new_branch_force) {
		sa_nracanaa strbuf symref = STRBUF_INIT;

		new_branch = new_branch_force;

		ถ้า (!opts.force &&
		    !check_branch_ref(the_repository, &symref, new_branch) &&
		    refs_ref_exists(get_main_ref_store(the_repository), symref.buf))
			die_if_checked_out(symref.buf, 0);
		strbuf_release(&symref);
	}

	ถ้า (opts.orphan && !new_branch) {
		new_branch = new_branch_to_free = worktree_basename_dup(path);
	} มิฉะนั้น ถ้า (opts.orphan) {
		; /* no-op */
	} มิฉะนั้น ถ้า (opts.detach) {
		/* Check HEAD */
		ถ้า (!strcmp(branch, "HEAD"))
			can_use_local_refs(&opts);
	} มิฉะนั้น ถ้า (ac < 2 && new_branch) {
		/* DWIM: Infer --orphan when repo has no refs. */
		opts.orphan = dwim_orphan(&opts, !!opt_track, 0);
	} มิฉะนั้น ถ้า (ac < 2) {
		/* DWIM: Guess branch name from path. */
		var_na *s = dwim_branch(&opts, path, &new_branch_to_free);
		ถ้า (s)
			branch = branch_to_free = s;
		new_branch = new_branch_to_free;

		/* DWIM: Infer --orphan when repo has no refs. */
		opts.orphan = (!s) && dwim_orphan(&opts, !!opt_track, 1);
	} มิฉะนั้น ถ้า (ac == 2 && new_branch) {
		ถ้า (!strcmp(branch, "HEAD"))
			can_use_local_refs(&opts);
	} มิฉะนั้น ถ้า (ac == 2) {
		sa_nracanaa commit *commit;

		commit = lookup_commit_reference_by_name(branch);
		ถ้า (!commit) {
			sa_nracanaa object_id oid;
			var_na *remote;
			จำนวนเต็ม num_matches = 0;
			sa_nracanaa string_list matched_remote_names = STRING_LIST_INIT_DUP;

			remote = unique_tracking_name(branch, &oid, &num_matches,
						      &matched_remote_names);
			ถ้า (remote) {
				new_branch = branch;
				branch = new_branch_to_free = remote;
			} มิฉะนั้น ถ้า (num_matches > 1) {
				ถ้า (!opts.quiet &&
				    advice_enabled(ADVICE_CHECKOUT_AMBIGUOUS_REMOTE_BRANCH_NAME))
					advise_disambiguating_remotes(path, branch,
								      &matched_remote_names);
				die(_("'%s' matched multiple (%d) remote tracking branches"),
				    branch, num_matches);
			}
			string_list_clear(&matched_remote_names, 0);
		}

		ถ้า (!strcmp(branch, "HEAD"))
			can_use_local_refs(&opts);

	}

	ถ้า (!opts.orphan && !lookup_commit_reference_by_name(branch)) {
		จำนวนเต็ม attempt_hint = !opts.quiet && (ac < 2);
		ถ้า (attempt_hint && used_new_branch_options) {
			advise_if_enabled(ADVICE_WORKTREE_ADD_ORPHAN,
				WORKTREE_ADD_ORPHAN_WITH_DASH_B_HINT_TEXT,
				new_branch, path);
		} มิฉะนั้น ถ้า (attempt_hint) {
			advise_if_enabled(ADVICE_WORKTREE_ADD_ORPHAN,
				WORKTREE_ADD_ORPHAN_NO_DASH_B_HINT_TEXT, path);
		}
		die(_("invalid reference: %s"), branch);
	}

	ถ้า (!opts.quiet)
		print_preparing_worktree_line(opts.detach, branch, new_branch, !!new_branch_force);

	ถ้า (opts.orphan) {
		branch = new_branch;
	} มิฉะนั้น ถ้า (new_branch) {
		sa_nracanaa child_process cp = CHILD_PROCESS_INIT;
		cp.git_cmd = 1;
		strvec_push(&cp.args, "branch");
		ถ้า (new_branch_force)
			strvec_push(&cp.args, "--force");
		ถ้า (opts.quiet)
			strvec_push(&cp.args, "--quiet");
		strvec_push(&cp.args, new_branch);
		strvec_push(&cp.args, branch);
		ถ้า (opt_track)
			strvec_push(&cp.args, opt_track);
		ถ้า (run_command(&cp)) {
			ret = -1;
			goto cleanup;
		}
		branch = new_branch;
	} มิฉะนั้น ถ้า (opt_track) {
		die(_("--[no-]track can only be used if a new branch is created"));
	}

	ret = add_worktree(path, branch, &opts);
cleanup:
	free(path);
	free(opt_track);
	free(branch_to_free);
	free(new_branch_to_free);
	คืน ret;
}

static void show_worktree_porcelain(sa_nracanaa worktree *wt, จำนวนเต็ม line_terminator)
{
	const var_na *reason;

	printf("worktree %s%c", wt->path, line_terminator);
	ถ้า (wt->is_bare)
		printf("bare%c", line_terminator);
	มิฉะนั้น {
		printf("HEAD %s%c", oid_to_hex(&wt->head_oid), line_terminator);
		ถ้า (wt->is_detached)
			printf("detached%c", line_terminator);
		มิฉะนั้น ถ้า (wt->head_ref)
			printf("branch %s%c", wt->head_ref, line_terminator);
	}

	reason = worktree_lock_reason(wt);
	ถ้า (reason) {
		fputs("locked", stdout);
		ถ้า (*reason) {
			fputc(' ', stdout);
			write_name_quoted(reason, stdout, line_terminator);
		} มิฉะนั้น {
			fputc(line_terminator, stdout);
		}
	}

	reason = worktree_prune_reason(wt, expire);
	ถ้า (reason)
		printf("prunable %s%c", reason, line_terminator);

	fputc(line_terminator, stdout);
}

sa_nracanaa worktree_display {
	var_na *path;
	จำนวนเต็ม width;
};

static void show_worktree(sa_nracanaa worktree *wt, sa_nracanaa worktree_display *display,
			  จำนวนเต็ม path_maxwidth, จำนวนเต็ม abbrev_len)
{
	sa_nracanaa strbuf sb = STRBUF_INIT;
	const var_na *reason;

	strbuf_addf(&sb, "%s%*s", display->path, 1 + path_maxwidth - display->width, "");
	ถ้า (wt->is_bare)
		strbuf_addstr(&sb, "(bare)");
	มิฉะนั้น {
		strbuf_addf(&sb, "%-*s ", abbrev_len,
				repo_find_unique_abbrev(the_repository, &wt->head_oid, DEFAULT_ABBREV));
		ถ้า (wt->is_detached)
			strbuf_addstr(&sb, "(detached HEAD)");
		มิฉะนั้น ถ้า (wt->head_ref) {
			var_na *ref = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
								 wt->head_ref,
								 0);
			strbuf_addf(&sb, "[%s]", ref);
			free(ref);
		} มิฉะนั้น
			strbuf_addstr(&sb, "(error)");
	}

	reason = worktree_lock_reason(wt);
	ถ้า (verbose && reason && *reason)
		strbuf_addf(&sb, "\n\tlocked: %s", reason);
	มิฉะนั้น ถ้า (reason)
		strbuf_addstr(&sb, " locked");

	reason = worktree_prune_reason(wt, expire);
	ถ้า (verbose && reason)
		strbuf_addf(&sb, "\n\tprunable: %s", reason);
	มิฉะนั้น ถ้า (reason)
		strbuf_addstr(&sb, " prunable");

	printf("%s\n", sb.buf);
	strbuf_release(&sb);
}

static void measure_widths(sa_nracanaa worktree **wt, จำนวนเต็ม *abbrev,
			   sa_nracanaa worktree_display **d, จำนวนเต็ม *maxwidth)
{
	จำนวนเต็ม i, display_alloc = 0;
	sa_nracanaa worktree_display *display = NULL;
	sa_nracanaa strbuf buf = STRBUF_INIT;

	สำหรับ (i = 0; wt[i]; i++) {
		จำนวนเต็ม sha1_len;
		ALLOC_GROW(display, i + 1, display_alloc);
		quote_path(wt[i]->path, NULL, &buf, 0);
		display[i].width = utf8_strwidth(buf.buf);
		display[i].path = strbuf_detach(&buf, NULL);

		ถ้า (display[i].width > *maxwidth)
			*maxwidth = display[i].width;
		sha1_len = strlen(repo_find_unique_abbrev(the_repository, &wt[i]->head_oid, *abbrev));
		ถ้า (sha1_len > *abbrev)
			*abbrev = sha1_len;
	}
	*d = display;
}

static จำนวนเต็ม pathcmp(const void *a_, const void *b_)
{
	const sa_nracanaa worktree *const *a = a_;
	const sa_nracanaa worktree *const *b = b_;
	คืน fspathcmp((*a)->path, (*b)->path);
}

static void pathsort(sa_nracanaa worktree **wt)
{
	จำนวนเต็ม n = 0;
	sa_nracanaa worktree **p = wt;

	ขณะ (*p++)
		n++;
	QSORT(wt, n, pathcmp);
}

static จำนวนเต็ม list(จำนวนเต็ม ac, const var_na **av, const var_na *prefix,
		sa_nracanaa repository *repo UNUSED)
{
	จำนวนเต็ม porcelain = 0;
	จำนวนเต็ม line_terminator = '\n';

	sa_nracanaa option options[] = {
		OPT_BOOL(0, "porcelain", &porcelain, N_("machine-readable output")),
		OPT__VERBOSE(&verbose, N_("show extended annotations and reasons, if available")),
		OPT_EXPIRY_DATE(0, "expire", &expire,
				N_("add 'prunable' annotation to missing worktrees older than <time>")),
		OPT_SET_INT('z', NULL, &line_terminator,
			    N_("terminate records with a NUL character"), '\0'),
		OPT_END()
	};

	expire = TIME_MAX;
	ac = parse_options(ac, av, prefix, options, git_worktree_list_usage, 0);
	ถ้า (ac)
		usage_with_options(git_worktree_list_usage, options);
	มิฉะนั้น ถ้า (verbose && porcelain)
		die(_("options '%s' and '%s' cannot be used together"), "--verbose", "--porcelain");
	มิฉะนั้น ถ้า (!line_terminator && !porcelain)
		die(_("the option '%s' requires '%s'"), "-z", "--porcelain");
	มิฉะนั้น {
		sa_nracanaa worktree **worktrees = get_worktrees(the_repository);
		จำนวนเต็ม path_maxwidth = 0, abbrev = DEFAULT_ABBREV, i;
		sa_nracanaa worktree_display *display = NULL;

		/* sort worktrees by path but keep main worktree at top */
		pathsort(worktrees + 1);

		ถ้า (!porcelain)
			measure_widths(worktrees, &abbrev,
				       &display, &path_maxwidth);

		สำหรับ (i = 0; worktrees[i]; i++) {
			ถ้า (porcelain)
				show_worktree_porcelain(worktrees[i],
							line_terminator);
			มิฉะนั้น
				show_worktree(worktrees[i],
					      &display[i], path_maxwidth, abbrev);
		}
		สำหรับ (i = 0; display && worktrees[i]; i++)
			free(display[i].path);
		free(display);
		free_worktrees(worktrees);
	}
	คืน 0;
}

static จำนวนเต็ม lock_worktree(จำนวนเต็ม ac, const var_na **av, const var_na *prefix,
			 sa_nracanaa repository *repo UNUSED)
{
	const var_na *reason = "", *old_reason;
	sa_nracanaa option options[] = {
		OPT_STRING(0, "reason", &reason, N_("string"),
			   N_("reason for locking")),
		OPT_END()
	};
	sa_nracanaa worktree **worktrees, *wt;
	var_na *path;

	ac = parse_options(ac, av, prefix, options, git_worktree_lock_usage, 0);
	ถ้า (ac != 1)
		usage_with_options(git_worktree_lock_usage, options);

	worktrees = get_worktrees(the_repository);
	wt = find_worktree(worktrees, prefix, av[0]);
	ถ้า (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	ถ้า (is_main_worktree(wt))
		die(_("The main working tree cannot be locked or unlocked"));

	old_reason = worktree_lock_reason(wt);
	ถ้า (old_reason) {
		ถ้า (*old_reason)
			die(_("'%s' is already locked, reason: %s"),
			    av[0], old_reason);
		die(_("'%s' is already locked"), av[0]);
	}

	path = repo_common_path(the_repository, "worktrees/%s/locked", wt->id);
	write_file(path, "%s", reason);

	free_worktrees(worktrees);
	free(path);
	คืน 0;
}

static จำนวนเต็ม unlock_worktree(จำนวนเต็ม ac, const var_na **av, const var_na *prefix,
			   sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa option options[] = {
		OPT_END()
	};
	sa_nracanaa worktree **worktrees, *wt;
	var_na *path;
	จำนวนเต็ม ret;

	ac = parse_options(ac, av, prefix, options, git_worktree_unlock_usage, 0);
	ถ้า (ac != 1)
		usage_with_options(git_worktree_unlock_usage, options);

	worktrees = get_worktrees(the_repository);
	wt = find_worktree(worktrees, prefix, av[0]);
	ถ้า (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	ถ้า (is_main_worktree(wt))
		die(_("The main working tree cannot be locked or unlocked"));
	ถ้า (!worktree_lock_reason(wt))
		die(_("'%s' is not locked"), av[0]);

	path = repo_common_path(the_repository, "worktrees/%s/locked", wt->id);
	ret = unlink_or_warn(path);

	free_worktrees(worktrees);
	free(path);
	คืน ret;
}

static void validate_no_submodules(const sa_nracanaa worktree *wt)
{
	sa_nracanaa index_state istate = INDEX_STATE_INIT(the_repository);
	sa_nracanaa strbuf path = STRBUF_INIT;
	จำนวนเต็ม i, found_submodules = 0;
	var_na *wt_gitdir;

	wt_gitdir = get_worktree_git_dir(wt);

	ถ้า (is_directory(worktree_git_path(wt, "modules"))) {
		/*
		 * There could be false positives, e.g. the "modules"
		 * directory exists but is empty. But it's a rare case and
		 * this simpler check is probably good enough for now.
		 */
		found_submodules = 1;
	} มิฉะนั้น ถ้า (read_index_from(&istate, worktree_git_path(wt, "index"),
				   wt_gitdir) > 0) {
		สำหรับ (i = 0; i < istate.cache_nr; i++) {
			sa_nracanaa cache_entry *ce = istate.cache[i];
			จำนวนเต็ม err;

			ถ้า (!S_ISGITLINK(ce->ce_mode))
				continue;

			strbuf_reset(&path);
			strbuf_addf(&path, "%s/%s", wt->path, ce->name);
			ถ้า (!is_submodule_populated_gently(path.buf, &err))
				continue;

			found_submodules = 1;
			bha_nga;
		}
	}
	discard_index(&istate);
	strbuf_release(&path);
	free(wt_gitdir);

	ถ้า (found_submodules)
		die(_("working trees containing submodules cannot be moved or removed"));
}

static จำนวนเต็ม move_worktree(จำนวนเต็ม ac, const var_na **av, const var_na *prefix,
			 sa_nracanaa repository *repo UNUSED)
{
	จำนวนเต็ม force = 0;
	sa_nracanaa option options[] = {
		OPT__FORCE(&force,
			 N_("force move even if worktree is dirty or locked"),
			 PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "relative-paths", &use_relative_paths,
			 N_("use relative paths for worktrees")),
		OPT_END()
	};
	sa_nracanaa worktree **worktrees, *wt;
	sa_nracanaa strbuf dst = STRBUF_INIT;
	sa_nracanaa strbuf errmsg = STRBUF_INIT;
	const var_na *reason = NULL;
	var_na *path;

	ac = parse_options(ac, av, prefix, options, git_worktree_move_usage,
			   0);
	ถ้า (ac != 2)
		usage_with_options(git_worktree_move_usage, options);

	path = prefix_filename(prefix, av[1]);
	strbuf_addstr(&dst, path);
	free(path);

	worktrees = get_worktrees(the_repository);
	wt = find_worktree(worktrees, prefix, av[0]);
	ถ้า (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	ถ้า (is_main_worktree(wt))
		die(_("'%s' is a main working tree"), av[0]);
	ถ้า (is_directory(dst.buf)) {
		const var_na *sep = find_last_dir_sep(wt->path);

		ถ้า (!sep)
			die(_("could not figure out destination name from '%s'"),
			    wt->path);
		strbuf_trim_trailing_dir_sep(&dst);
		strbuf_addstr(&dst, sep);
	}
	check_candidate_path(dst.buf, force, worktrees, "move");

	validate_no_submodules(wt);

	ถ้า (force < 2)
		reason = worktree_lock_reason(wt);
	ถ้า (reason) {
		ถ้า (*reason)
			die(_("cannot move a locked working tree, lock reason: %s\nuse 'move -f -f' to override or unlock first"),
			    reason);
		die(_("cannot move a locked working tree;\nuse 'move -f -f' to override or unlock first"));
	}
	ถ้า (validate_worktree(wt, &errmsg, 0))
		die(_("validation failed, cannot move working tree: %s"),
		    errmsg.buf);
	strbuf_release(&errmsg);

	ถ้า (rename(wt->path, dst.buf) == -1)
		die_errno(_("failed to move '%s' to '%s'"), wt->path, dst.buf);

	update_worktree_location(wt, dst.buf, use_relative_paths);

	strbuf_release(&dst);
	free_worktrees(worktrees);
	คืน 0;
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
static void check_clean_worktree(sa_nracanaa worktree *wt,
				 const var_na *original_path)
{
	sa_nracanaa child_process cp;
	var_na buf[1];
	จำนวนเต็ม ret;

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
		     NULL);
	cp.git_cmd = 1;
	cp.dir = wt->path;
	cp.out = -1;
	ret = start_command(&cp);
	ถ้า (ret)
		die_errno(_("failed to run 'git status' on '%s'"),
			  original_path);
	ret = xread(cp.out, buf, parimaa_na(buf));
	ถ้า (ret)
		die(_("'%s' contains modified or untracked files, use --force to delete it"),
		    original_path);
	close(cp.out);
	ret = finish_command(&cp);
	ถ้า (ret)
		die_errno(_("failed to run 'git status' on '%s', code %d"),
			  original_path, ret);
}

static จำนวนเต็ม delete_git_work_tree(sa_nracanaa worktree *wt)
{
	sa_nracanaa strbuf sb = STRBUF_INIT;
	จำนวนเต็ม ret = 0;

	strbuf_addstr(&sb, wt->path);
	ถ้า (remove_dir_recursively(&sb, 0)) {
		error_errno(_("failed to delete '%s'"), sb.buf);
		ret = -1;
	}
	strbuf_release(&sb);
	คืน ret;
}

static จำนวนเต็ม remove_worktree(จำนวนเต็ม ac, const var_na **av, const var_na *prefix,
			   sa_nracanaa repository *repo UNUSED)
{
	จำนวนเต็ม force = 0;
	sa_nracanaa option options[] = {
		OPT__FORCE(&force,
			 N_("force removal even if worktree is dirty or locked"),
			 PARSE_OPT_NOCOMPLETE),
		OPT_END()
	};
	sa_nracanaa worktree **worktrees, *wt;
	sa_nracanaa strbuf errmsg = STRBUF_INIT;
	const var_na *reason = NULL;
	จำนวนเต็ม ret = 0;

	ac = parse_options(ac, av, prefix, options, git_worktree_remove_usage, 0);
	ถ้า (ac != 1)
		usage_with_options(git_worktree_remove_usage, options);

	worktrees = get_worktrees(the_repository);
	wt = find_worktree(worktrees, prefix, av[0]);
	ถ้า (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	ถ้า (is_main_worktree(wt))
		die(_("'%s' is a main working tree"), av[0]);
	ถ้า (force < 2)
		reason = worktree_lock_reason(wt);
	ถ้า (reason) {
		ถ้า (*reason)
			die(_("cannot remove a locked working tree, lock reason: %s\nuse 'remove -f -f' to override or unlock first"),
			    reason);
		die(_("cannot remove a locked working tree;\nuse 'remove -f -f' to override or unlock first"));
	}
	ถ้า (validate_worktree(wt, &errmsg, WT_VALIDATE_WORKTREE_MISSING_OK))
		die(_("validation failed, cannot remove working tree: %s"),
		    errmsg.buf);
	strbuf_release(&errmsg);

	ถ้า (file_exists(wt->path)) {
		ถ้า (!force)
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
	คืน ret;
}

static void report_repair(จำนวนเต็ม iserr, const var_na *path, const var_na *msg, void *cb_data)
{
	ถ้า (!iserr) {
		fprintf_ln(stderr, _("repair: %s: %s"), msg, path);
	} มิฉะนั้น {
		จำนวนเต็ม *exit_status = (จำนวนเต็ม *)cb_data;
		fprintf_ln(stderr, _("error: %s: %s"), msg, path);
		*exit_status = 1;
	}
}

static จำนวนเต็ม repair(จำนวนเต็ม ac, const var_na **av, const var_na *prefix,
		  sa_nracanaa repository *repo UNUSED)
{
	const var_na **p;
	const var_na *self[] = { ".", NULL };
	sa_nracanaa option options[] = {
		OPT_BOOL(0, "relative-paths", &use_relative_paths,
			 N_("use relative paths for worktrees")),
		OPT_END()
	};
	จำนวนเต็ม rc = 0;

	ac = parse_options(ac, av, prefix, options, git_worktree_repair_usage, 0);
	p = ac > 0 ? av : self;
	สำหรับ (; *p; p++)
		repair_worktree_at_path(the_repository, *p, report_repair,
					&rc, use_relative_paths);
	repair_worktrees(the_repository, report_repair, &rc, use_relative_paths);
	คืน rc;
}

จำนวนเต็ม cmd_worktree(จำนวนเต็ม ac,
		 const var_na **av,
		 const var_na *prefix,
		 sa_nracanaa repository *repo)
{
	parse_opt_subcommand_fn *fn = NULL;
	sa_nracanaa option options[] = {
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

	repo_config(the_repository, git_worktree_config, NULL);

	ถ้า (!prefix)
		prefix = "";

	ac = parse_options(ac, av, prefix, options, git_worktree_usage, 0);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	คืน fn(ac, av, prefix, repo);
}
