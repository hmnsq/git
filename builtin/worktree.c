#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "abspath.h"
#அந்தர்பாவ "advice.h"
#அந்தர்பாவ "checkout.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "copy.h"
#அந்தர்பாவ "dir.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "hex.h"
#அந்தர்பாவ "object-file.h"
#அந்தர்பாவ "object-name.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "path.h"
#அந்தர்பாவ "strvec.h"
#அந்தர்பாவ "branch.h"
#அந்தர்பாவ "read-cache-ll.h"
#அந்தர்பாவ "refs.h"
#அந்தர்பாவ "remote.h"
#அந்தர்பாவ "run-command.h"
#அந்தர்பாவ "hook.h"
#அந்தர்பாவ "sigchain.h"
#அந்தர்பாவ "submodule.h"
#அந்தர்பாவ "utf8.h"
#அந்தர்பாவ "worktree.h"
#அந்தர்பாவ "quote.h"

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

நிலையான மாறிலி எழுத்து * மாறிலி git_worktree_usage[] = {
	BUILTIN_WORKTREE_ADD_USAGE,
	BUILTIN_WORKTREE_LIST_USAGE,
	BUILTIN_WORKTREE_LOCK_USAGE,
	BUILTIN_WORKTREE_MOVE_USAGE,
	BUILTIN_WORKTREE_PRUNE_USAGE,
	BUILTIN_WORKTREE_REMOVE_USAGE,
	BUILTIN_WORKTREE_REPAIR_USAGE,
	BUILTIN_WORKTREE_UNLOCK_USAGE,
	வெற்று
};

நிலையான மாறிலி எழுத்து * மாறிலி git_worktree_add_usage[] = {
	BUILTIN_WORKTREE_ADD_USAGE,
	வெற்று,
};

நிலையான மாறிலி எழுத்து * மாறிலி git_worktree_list_usage[] = {
	BUILTIN_WORKTREE_LIST_USAGE,
	வெற்று
};

நிலையான மாறிலி எழுத்து * மாறிலி git_worktree_lock_usage[] = {
	BUILTIN_WORKTREE_LOCK_USAGE,
	வெற்று
};

நிலையான மாறிலி எழுத்து * மாறிலி git_worktree_move_usage[] = {
	BUILTIN_WORKTREE_MOVE_USAGE,
	வெற்று
};

நிலையான மாறிலி எழுத்து * மாறிலி git_worktree_prune_usage[] = {
	BUILTIN_WORKTREE_PRUNE_USAGE,
	வெற்று
};

நிலையான மாறிலி எழுத்து * மாறிலி git_worktree_remove_usage[] = {
	BUILTIN_WORKTREE_REMOVE_USAGE,
	வெற்று
};

நிலையான மாறிலி எழுத்து * மாறிலி git_worktree_repair_usage[] = {
	BUILTIN_WORKTREE_REPAIR_USAGE,
	வெற்று
};

நிலையான மாறிலி எழுத்து * மாறிலி git_worktree_unlock_usage[] = {
	BUILTIN_WORKTREE_UNLOCK_USAGE,
	வெற்று
};

கட்டமைப்பு add_opts {
	முழு force;
	முழு detach;
	முழு quiet;
	முழு checkout;
	முழு orphan;
	முழு relative_paths;
	மாறிலி எழுத்து *keep_locked;
};

நிலையான முழு show_only;
நிலையான முழு verbose;
நிலையான முழு guess_remote;
நிலையான முழு use_relative_paths;
நிலையான timestamp_t expire;

நிலையான முழு git_worktree_config(மாறிலி எழுத்து *var, மாறிலி எழுத்து *மதிப்பு,
			       மாறிலி கட்டமைப்பு config_context *ctx, வெறுமை *cb)
{
	எனில் (!strcmp(var, "worktree.guessremote")) {
		guess_remote = git_config_bool(var, மதிப்பு);
		திரும்பு 0;
	} இல்லையெனில் எனில் (!strcmp(var, "worktree.userelativepaths")) {
		use_relative_paths = git_config_bool(var, மதிப்பு);
		திரும்பு 0;
	}

	திரும்பு git_default_config(var, மதிப்பு, ctx, cb);
}

நிலையான முழு delete_git_dir(மாறிலி எழுத்து *id)
{
	கட்டமைப்பு strbuf sb = STRBUF_INIT;
	முழு ret;

	repo_common_path_append(the_repository, &sb, "worktrees/%s", id);
	ret = remove_dir_recursively(&sb, 0);
	எனில் (ret < 0 && errno == ENOTDIR)
		ret = unlink(sb.buf);
	எனில் (ret)
		error_errno(_("failed to delete '%s'"), sb.buf);
	strbuf_release(&sb);
	திரும்பு ret;
}

நிலையான வெறுமை delete_worktrees_dir_if_empty(வெறுமை)
{
	எழுத்து *path = repo_git_path(the_repository, "worktrees");
	rmdir(path); /* ignore failed removal */
	விடுவி(path);
}

நிலையான வெறுமை prune_worktree(மாறிலி எழுத்து *id, மாறிலி எழுத்து *reason)
{
	எனில் (show_only || verbose)
		fprintf_ln(stderr, _("Removing %s/%s: %s"), "worktrees", id, reason);
	எனில் (!show_only)
		delete_git_dir(id);
}

நிலையான முழு prune_cmp(மாறிலி வெறுமை *a, மாறிலி வெறுமை *b)
{
	மாறிலி கட்டமைப்பு string_list_item *x = a;
	மாறிலி கட்டமைப்பு string_list_item *y = b;
	முழு c;

	எனில் ((c = fspathcmp(x->string, y->string)))
	    திரும்பு c;
	/*
	 * paths same; prune_dupes() removes all but the first worktree entry
	 * having the same path, so sort main worktree ('util' is NULL) above
	 * linked worktrees ('util' not NULL) since main worktree can't be
	 * removed
	 */
	எனில் (!x->util)
		திரும்பு -1;
	எனில் (!y->util)
		திரும்பு 1;
	/* paths same; sort by .git/worktrees/<id> */
	திரும்பு strcmp(x->util, y->util);
}

நிலையான வெறுமை prune_dups(கட்டமைப்பு string_list *l)
{
	முழு i;

	QSORT(l->பொருட்கள், l->nr, prune_cmp);
	க்கு (i = 1; i < l->nr; i++) {
		எனில் (!fspathcmp(l->பொருட்கள்[i].string, l->பொருட்கள்[i - 1].string))
			prune_worktree(l->பொருட்கள்[i].util, "duplicate entry");
	}
}

நிலையான வெறுமை prune_worktrees(வெறுமை)
{
	கட்டமைப்பு strbuf reason = STRBUF_INIT;
	கட்டமைப்பு strbuf main_path = STRBUF_INIT;
	கட்டமைப்பு string_list kept = STRING_LIST_INIT_DUP;
	எழுத்து *path;
	DIR *dir;
	கட்டமைப்பு dirent *d;

	path = repo_git_path(the_repository, "worktrees");
	dir = opendir(path);
	விடுவி(path);
	எனில் (!dir)
		திரும்பு;
	போது ((d = readdir_skip_dot_and_dotdot(dir)) != வெற்று) {
		எழுத்து *path;
		strbuf_reset(&reason);
		எனில் (should_prune_worktree(the_repository, d->d_name, &reason, &path, expire))
			prune_worktree(d->d_name, reason.buf);
		இல்லையெனில் எனில் (path)
			string_list_append_nodup(&kept, path)->util = xstrdup(d->d_name);
	}
	closedir(dir);

	strbuf_add_absolute_path(&main_path, repo_get_common_dir(the_repository));
	/* massage main worktree absolute path to match 'gitdir' content */
	strbuf_strip_suffix(&main_path, "/.");
	string_list_append_nodup(&kept, strbuf_detach(&main_path, வெற்று));
	prune_dups(&kept);
	string_list_clear(&kept, 1);

	எனில் (!show_only)
		delete_worktrees_dir_if_empty();
	strbuf_release(&reason);
}

நிலையான முழு prune(முழு ac, மாறிலி எழுத்து **av, மாறிலி எழுத்து *prefix,
		 கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு option options[] = {
		OPT__DRY_RUN(&show_only, N_("do not remove, show only")),
		OPT__VERBOSE(&verbose, N_("report pruned working trees")),
		OPT_EXPIRY_DATE(0, "expire", &expire,
				N_("prune missing working trees older than <time>")),
		OPT_END()
	};

	expire = TIME_MAX;
	ac = parse_options(ac, av, prefix, options, git_worktree_prune_usage,
			   0);
	எனில் (ac)
		usage_with_options(git_worktree_prune_usage, options);
	prune_worktrees();
	திரும்பு 0;
}

நிலையான எழுத்து *junk_work_மரம்;
நிலையான எழுத்து *junk_git_dir;
நிலையான முழு is_junk;
நிலையான pid_t junk_pid;

நிலையான வெறுமை remove_junk(வெறுமை)
{
	கட்டமைப்பு strbuf sb = STRBUF_INIT;
	எனில் (!is_junk || getpid() != junk_pid)
		திரும்பு;
	எனில் (junk_git_dir) {
		strbuf_addstr(&sb, junk_git_dir);
		remove_dir_recursively(&sb, 0);
		strbuf_reset(&sb);
	}
	எனில் (junk_work_மரம்) {
		strbuf_addstr(&sb, junk_work_மரம்);
		remove_dir_recursively(&sb, 0);
	}
	strbuf_release(&sb);
}

நிலையான வெறுமை remove_junk_on_signal(முழு signo)
{
	remove_junk();
	sigchain_எடு(signo);
	எழுப்பு(signo);
}

நிலையான எழுத்து *worktree_basename_dup(மாறிலி எழுத்து *path)
{
	மாறிலி எழுத்து *name;
	முழு len;

	len = strlen(path);
	போது (len && is_dir_sep(path[len - 1]))
		len--;

	name = path + len;
	போது (name > path && !is_dir_sep(name[-1]))
		name--;

	திரும்பு xmemdupz(name, path + len - name);
}

/* check that path is viable location for worktree */
நிலையான வெறுமை check_candidate_path(மாறிலி எழுத்து *path,
				 முழு force,
				 கட்டமைப்பு worktree **worktrees,
				 மாறிலி எழுத்து *cmd)
{
	கட்டமைப்பு worktree *wt;
	முழு locked;

	எனில் (file_exists(path) && !is_empty_dir(path))
		die(_("'%s' already exists"), path);

	wt = find_worktree_by_path(worktrees, path);
	எனில் (!wt)
		திரும்பு;

	locked = !!worktree_lock_reason(wt);
	எனில் ((!locked && force) || (locked && force > 1)) {
		எனில் (delete_git_dir(wt->id))
		    die(_("unusable worktree destination '%s'"), path);
		திரும்பு;
	}

	எனில் (locked)
		die(_("'%s' is a missing but locked worktree;\nuse '%s -f -f' to override, or 'unlock' and 'prune' or 'remove' to clear"), path, cmd);
	இல்லையெனில்
		die(_("'%s' is a missing but already registered worktree;\nuse '%s -f' to override, or 'prune' or 'remove' to clear"), path, cmd);
}

நிலையான வெறுமை copy_sparse_checkout(மாறிலி எழுத்து *worktree_git_dir)
{
	எழுத்து *from_file = repo_git_path(the_repository, "info/sparse-checkout");
	எழுத்து *to_file = xstrfmt("%s/info/sparse-checkout", worktree_git_dir);

	எனில் (file_exists(from_file)) {
		எனில் (safe_create_leading_directories(the_repository, to_file) ||
			copy_file(the_repository, to_file, from_file, 0666))
			error(_("failed to copy '%s' to '%s'; sparse-checkout may not work correctly"),
				from_file, to_file);
	}

	விடுவி(from_file);
	விடுவி(to_file);
}

நிலையான வெறுமை copy_filtered_worktree_config(மாறிலி எழுத்து *worktree_git_dir)
{
	எழுத்து *from_file = repo_git_path(the_repository, "config.worktree");
	எழுத்து *to_file = xstrfmt("%s/config.worktree", worktree_git_dir);

	எனில் (file_exists(from_file)) {
		கட்டமைப்பு config_set cs = { { 0 } };
		முழு bare;

		எனில் (safe_create_leading_directories(the_repository, to_file) ||
			copy_file(the_repository, to_file, from_file, 0666)) {
			error(_("failed to copy worktree config from '%s' to '%s'"),
				from_file, to_file);
			செல் worktree_copy_cleanup;
		}

		git_configset_init(&cs);
		git_configset_add_file(&cs, from_file);

		எனில் (!git_configset_get_bool(&cs, "core.bare", &bare) &&
			bare &&
			repo_config_set_multivar_in_file_gently(the_repository,
				to_file, "core.bare", வெற்று, "true", வெற்று, 0))
			error(_("failed to unset '%s' in '%s'"),
				"core.bare", to_file);
		எனில் (!git_configset_get(&cs, "core.worktree") &&
			repo_config_set_in_file_gently(the_repository, to_file,
						       "core.worktree", வெற்று, வெற்று))
			error(_("failed to unset '%s' in '%s'"),
				"core.worktree", to_file);

		git_configset_clear(&cs);
	}

worktree_copy_cleanup:
	விடுவி(from_file);
	விடுவி(to_file);
}

நிலையான முழு checkout_worktree(மாறிலி கட்டமைப்பு add_opts *opts,
			     கட்டமைப்பு strvec *child_env)
{
	கட்டமைப்பு child_process cp = CHILD_PROCESS_INIT;
	cp.git_cmd = 1;
	strvec_pushl(&cp.args, "reset", "--hard", "--no-recurse-submodules", வெற்று);
	எனில் (opts->quiet)
		strvec_தள்ளு(&cp.args, "--quiet");
	strvec_pushv(&cp.env, child_env->v);
	திரும்பு run_command(&cp);
}

நிலையான முழு make_worktree_orphan(மாறிலி எழுத்து * ref, மாறிலி கட்டமைப்பு add_opts *opts,
				கட்டமைப்பு strvec *child_env)
{
	கட்டமைப்பு strbuf symref = STRBUF_INIT;
	கட்டமைப்பு child_process cp = CHILD_PROCESS_INIT;

	validate_new_branchname(ref, &symref, 0);
	strvec_pushl(&cp.args, "symbolic-ref", "HEAD", symref.buf, வெற்று);
	எனில் (opts->quiet)
		strvec_தள்ளு(&cp.args, "--quiet");
	strvec_pushv(&cp.env, child_env->v);
	strbuf_release(&symref);
	cp.git_cmd = 1;
	திரும்பு run_command(&cp);
}

/*
 * References for worktrees are generally stored in '$GIT_DIR/worktrees/<wt_id>'.
 * But when using alternate reference directories, we want to store the worktree
 * references in '$ALTERNATE_REFERENCE_DIR/worktrees/<wt_id>'.
 *
 * Create the necessary folder structure to facilitate the same. But to ensure
 * that the former path is still considered a Git directory, add stubs.
 */
நிலையான வெறுமை setup_alternate_ref_dir(கட்டமைப்பு worktree *wt, மாறிலி எழுத்து *wt_git_path)
{
	கட்டமைப்பு strbuf sb = STRBUF_INIT;
	எழுத்து *path;

	path = wt->repo->ref_storage_payload;
	எனில் (!path)
		திரும்பு;

	எனில் (!is_absolute_path(path))
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

நிலையான முழு add_worktree(மாறிலி எழுத்து *path, மாறிலி எழுத்து *refname,
			மாறிலி கட்டமைப்பு add_opts *opts)
{
	கட்டமைப்பு strbuf sb_git = STRBUF_INIT, sb_repo = STRBUF_INIT;
	கட்டமைப்பு strbuf sb = STRBUF_INIT;
	மாறிலி எழுத்து *name;
	எழுத்து *name_to_free = வெற்று;
	கட்டமைப்பு strvec child_env = STRVEC_INIT;
	unsigned முழு counter = 0;
	முழு len, ret;
	கட்டமைப்பு strbuf symref = STRBUF_INIT;
	கட்டமைப்பு commit *commit = வெற்று;
	முழு is_branch = 0;
	கட்டமைப்பு strbuf sb_name = STRBUF_INIT;
	கட்டமைப்பு worktree **worktrees, *wt = வெற்று;
	கட்டமைப்பு ref_store *wt_refs;
	கட்டமைப்பு repo_config_மதிப்புகள் *cfg = repo_config_மதிப்புகள்(the_repository);

	worktrees = get_worktrees(the_repository);
	check_candidate_path(path, opts->force, worktrees, "add");
	free_worktrees(worktrees);
	worktrees = வெற்று;

	/* is 'refname' a branch or commit? */
	எனில் (!opts->detach && !check_branch_ref(the_repository, &symref, refname) &&
	    refs_ref_exists(get_main_ref_store(the_repository), symref.buf)) {
		is_branch = 1;
		எனில் (!opts->force)
			die_if_checked_out(symref.buf, 0);
	}
	commit = lookup_commit_reference_by_name(refname);
	எனில் (!commit && !opts->orphan)
		die(_("invalid reference: %s"), refname);

	name = name_to_free = worktree_basename_dup(path);
	எனில் (!*name)
		die(_("invalid path '%s'"), path);
	sanitize_refname_component(name, &sb_name);
	எனில் (!sb_name.len)
		BUG("How come '%s' becomes empty after sanitization?", name);
	name = sb_name.buf;
	repo_git_path_replace(the_repository, &sb_repo, "worktrees/%s", name);
	len = sb_repo.len;
	எனில் (safe_create_leading_directories_const(the_repository, sb_repo.buf))
		die_errno(_("could not create leading directories of '%s'"),
			  sb_repo.buf);

	போது (mkdir(sb_repo.buf, 0777)) {
		counter++;
		எனில் ((errno != EEXIST) || !counter /* overflow */)
			die_errno(_("could not create directory of '%s'"),
				  sb_repo.buf);
		strbuf_setlen(&sb_repo, len);
		strbuf_addf(&sb_repo, "%d", counter);
	}
	name = strrchr(sb_repo.buf, '/') + 1;

	junk_pid = getpid();
	atexit(remove_junk);
	sigchain_தள்ளு_common(remove_junk_on_signal);

	junk_git_dir = xstrdup(sb_repo.buf);
	is_junk = 1;

	/*
	 * lock the incomplete repo so prune won't delete it, unlock
	 * after the preparation is over.
	 */
	strbuf_addf(&sb, "%s/locked", sb_repo.buf);
	எனில் (opts->keep_locked)
		write_file(sb.buf, "%s", opts->keep_locked);
	இல்லையெனில்
		write_file(sb.buf, _("initializing"));

	strbuf_addf(&sb_git, "%s/.git", path);
	எனில் (safe_create_leading_directories_const(the_repository, sb_git.buf))
		die_errno(_("could not create leading directories of '%s'"),
			  sb_git.buf);
	junk_work_மரம் = xstrdup(path);

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
	எனில் (!wt) {
		ret = error(_("could not find created worktree '%s'"), name);
		செல் done;
	}
	setup_alternate_ref_dir(wt, sb_repo.buf);
	wt_refs = get_worktree_ref_store(wt);

	ret = ref_store_create_on_disk(wt_refs, REF_STORE_CREATE_ON_DISK_IS_WORKTREE, &sb);
	எனில் (ret)
		செல் done;

	எனில் (!is_branch && commit)
		ret = refs_update_ref(wt_refs, வெற்று, "HEAD", &commit->object.oid,
				      வெற்று, 0, UPDATE_REFS_செய்தி_ON_ERR);
	இல்லையெனில்
		ret = refs_update_symref(wt_refs, "HEAD", symref.buf, வெற்று);
	எனில் (ret)
		செல் done;

	/*
	 * If the current worktree has sparse-checkout enabled, then copy
	 * the sparse-checkout patterns from the current worktree.
	 */
	எனில் (cfg->apply_sparse_checkout)
		copy_sparse_checkout(sb_repo.buf);

	/*
	 * If we are using worktree config, then copy all current config
	 * values from the current worktree into the new one, that way the
	 * new worktree behaves the same as this one.
	 */
	எனில் (the_repository->repository_format_worktree_config)
		copy_filtered_worktree_config(sb_repo.buf);

	strvec_pushf(&child_env, "%s=%s", GIT_DIR_ENVIRONMENT, sb_git.buf);
	strvec_pushf(&child_env, "%s=%s", GIT_WORK_மரம்_ENVIRONMENT, path);

	எனில் (opts->orphan &&
	    (ret = make_worktree_orphan(refname, opts, &child_env)))
		செல் done;

	எனில் (opts->checkout &&
	    (ret = checkout_worktree(opts, &child_env)))
		செல் done;

	is_junk = 0;
	FREE_AND_NULL(junk_work_மரம்);
	FREE_AND_NULL(junk_git_dir);

done:
	எனில் (ret || !opts->keep_locked) {
		strbuf_reset(&sb);
		strbuf_addf(&sb, "%s/locked", sb_repo.buf);
		unlink_or_warn(sb.buf);
	}

	/*
	 * Hook failure does not warrant worktree deletion, so run hook after
	 * is_junk is cleared, but do return appropriate code when hook fails.
	 */
	எனில் (!ret && opts->checkout && !opts->orphan) {
		கட்டமைப்பு run_hooks_opt opt = RUN_HOOKS_OPT_INIT_FORCE_SERIAL;

		strvec_pushl(&opt.env, "GIT_DIR", "GIT_WORK_TREE", வெற்று);
		strvec_pushl(&opt.args,
			     oid_to_hex(null_oid(the_hash_algo)),
			     oid_to_hex(&commit->object.oid),
			     "1",
			     வெற்று);
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
	விடுவி(name_to_free);
	திரும்பு ret;
}

நிலையான வெறுமை print_preparing_worktree_வரி(முழு detach,
					  மாறிலி எழுத்து *branch,
					  மாறிலி எழுத்து *new_branch,
					  முழு force_new_branch)
{
	எனில் (force_new_branch) {
		கட்டமைப்பு commit *commit = lookup_commit_reference_by_name(new_branch);
		எனில் (!commit)
			fprintf_ln(stderr, _("Preparing worktree (new branch '%s')"), new_branch);
		இல்லையெனில்
			fprintf_ln(stderr, _("Preparing worktree (resetting branch '%s'; was at %s)"),
				  new_branch,
				  repo_find_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV));
	} இல்லையெனில் எனில் (new_branch) {
		fprintf_ln(stderr, _("Preparing worktree (new branch '%s')"), new_branch);
	} இல்லையெனில் {
		கட்டமைப்பு strbuf s = STRBUF_INIT;
		எனில் (!detach && !check_branch_ref(the_repository, &s, branch) &&
		    refs_ref_exists(get_main_ref_store(the_repository), s.buf))
			fprintf_ln(stderr, _("Preparing worktree (checking out '%s')"),
				  branch);
		இல்லையெனில் {
			கட்டமைப்பு commit *commit = lookup_commit_reference_by_name(branch);
			எனில் (!commit)
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
நிலையான முழு first_valid_ref(மாறிலி கட்டமைப்பு reference *ref UNUSED, வெறுமை *cb_data UNUSED)
{
	திரும்பு 1;
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
நிலையான முழு can_use_local_refs(மாறிலி கட்டமைப்பு add_opts *opts)
{
	எனில் (refs_head_ref(get_main_ref_store(the_repository), first_valid_ref, வெற்று)) {
		திரும்பு 1;
	} இல்லையெனில் எனில் (refs_for_each_branch_ref(get_main_ref_store(the_repository), first_valid_ref, வெற்று)) {
		எனில் (!opts->quiet)
			warning(_("HEAD points to an invalid (or orphaned) reference.\n"));
		திரும்பு 1;
	}
	திரும்பு 0;
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
நிலையான முழு can_use_remote_refs(மாறிலி கட்டமைப்பு add_opts *opts)
{
	எனில் (!guess_remote) {
		திரும்பு 0;
	} இல்லையெனில் எனில் (refs_for_each_remote_ref(get_main_ref_store(the_repository), first_valid_ref, வெற்று)) {
		திரும்பு 1;
	} இல்லையெனில் எனில் (!opts->force && remote_get(வெற்று)) {
		die(_("No local or remote refs exist despite at least one remote\n"
		      "present, stopping; use 'add -f' to override or fetch a remote first"));
	}
	திரும்பு 0;
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
நிலையான முழு dwim_orphan(மாறிலி கட்டமைப்பு add_opts *opts, முழு opt_track, முழு remote)
{
	எனில் (can_use_local_refs(opts)) {
		திரும்பு 0;
	} இல்லையெனில் எனில் (remote && can_use_remote_refs(opts)) {
		திரும்பு 0;
	} இல்லையெனில் எனில் (!opts->quiet) {
		fprintf_ln(stderr, WORKTREE_ADD_DWIM_ORPHAN_INFER_TEXT);
	}

	எனில் (opt_track) {
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--track");
	} இல்லையெனில் எனில் (!opts->checkout) {
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--no-checkout");
	}
	திரும்பு 1;
}

நிலையான வெறுமை advise_disambiguating_remotes(மாறிலி எழுத்து *path, மாறிலி எழுத்து *branch,
					  மாறிலி கட்டமைப்பு string_list *matched_remote_names)
{
	கட்டமைப்பு string_list_item *item;

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

நிலையான எழுத்து *dwim_branch(மாறிலி கட்டமைப்பு add_opts *opts, மாறிலி எழுத்து *path, எழுத்து **new_branch)
{
	முழு branch_exists;
	எழுத்து *branchname = worktree_basename_dup(path);
	கட்டமைப்பு strbuf ref = STRBUF_INIT;

	branch_exists = !check_branch_ref(the_repository, &ref, branchname) &&
			refs_ref_exists(get_main_ref_store(the_repository),
					ref.buf);
	strbuf_release(&ref);
	எனில் (branch_exists)
		திரும்பு branchname;

	*new_branch = branchname;
	எனில் (guess_remote) {
		கட்டமைப்பு object_id oid;
		எழுத்து *remote;
		முழு இலக்கம்_matches = 0;
		கட்டமைப்பு string_list matched_remote_names = STRING_LIST_INIT_DUP;

		remote = unique_tracking_name(*new_branch, &oid, &இலக்கம்_matches,
					      &matched_remote_names);
		எனில் (!remote && இலக்கம்_matches > 1) {
			எனில் (!opts->quiet &&
			    advice_enabled(ADVICE_CHECKOUT_AMBIGUOUS_REMOTE_BRANCH_NAME))
				advise_disambiguating_remotes(path, *new_branch,
							      &matched_remote_names);
			die(_("'%s' matched multiple (%d) remote tracking branches"),
			    *new_branch, இலக்கம்_matches);
		}
		string_list_clear(&matched_remote_names, 0);
		திரும்பு remote;
	}
	திரும்பு வெற்று;
}

நிலையான முழு add(முழு ac, மாறிலி எழுத்து **av, மாறிலி எழுத்து *prefix,
	       கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு add_opts opts;
	மாறிலி எழுத்து *new_branch_force = வெற்று;
	எழுத்து *path;
	மாறிலி எழுத்து *branch;
	எழுத்து *branch_to_free = வெற்று;
	எழுத்து *new_branch_to_free = வெற்று;
	மாறிலி எழுத்து *new_branch = வெற்று;
	எழுத்து *opt_track = வெற்று;
	மாறிலி எழுத்து *lock_reason = வெற்று;
	முழு keep_locked = 0;
	முழு used_new_branch_options;
	கட்டமைப்பு option options[] = {
		OPT__FORCE(&opts.force,
			   N_("checkout <branch> even if already checked out in other worktree"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_STRING('b', வெற்று, &new_branch, N_("branch"),
			   N_("create a new branch")),
		OPT_STRING('B', வெற்று, &new_branch_force, N_("branch"),
			   N_("create or reset a branch")),
		OPT_BOOL(0, "orphan", &opts.orphan, N_("create unborn branch")),
		OPT_BOOL('d', "detach", &opts.detach, N_("detach HEAD at named commit")),
		OPT_BOOL(0, "checkout", &opts.checkout, N_("populate the new working tree")),
		OPT_BOOL(0, "lock", &keep_locked, N_("keep the new working tree locked")),
		OPT_STRING(0, "reason", &lock_reason, N_("string"),
			   N_("reason for locking")),
		OPT__QUIET(&opts.quiet, N_("suppress progress reporting")),
		OPT_PASSTHRU(0, "track", &opt_track, வெற்று,
			     N_("set up tracking mode (see git-branch(1))"),
			     PARSE_OPT_NOARG | PARSE_OPT_OPTARG),
		OPT_BOOL(0, "guess-remote", &guess_remote,
			 N_("try to match the new branch name with a remote-tracking branch")),
		OPT_BOOL(0, "relative-paths", &opts.relative_paths,
			 N_("use relative paths for worktrees")),
		OPT_END()
	};
	முழு ret;

	memset(&opts, 0, அளவு(opts));
	opts.checkout = 1;
	opts.relative_paths = use_relative_paths;
	ac = parse_options(ac, av, prefix, options, git_worktree_add_usage, 0);
	எனில் (!!opts.detach + !!new_branch + !!new_branch_force > 1)
		die(_("options '%s', '%s', and '%s' cannot be used together"), "-b", "-B", "--detach");
	எனில் (opts.detach && opts.orphan)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--detach");
	எனில் (opts.orphan && opt_track)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--track");
	எனில் (opts.orphan && !opts.checkout)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--no-checkout");
	எனில் (opts.orphan && ac == 2)
		die(_("option '%s' and commit-ish cannot be used together"),
		    "--orphan");
	எனில் (lock_reason && !keep_locked)
		die(_("the option '%s' requires '%s'"), "--reason", "--lock");
	எனில் (lock_reason)
		opts.keep_locked = lock_reason;
	இல்லையெனில் எனில் (keep_locked)
		opts.keep_locked = _("added with --lock");

	எனில் (ac < 1 || ac > 2)
		usage_with_options(git_worktree_add_usage, options);

	path = prefix_filename(prefix, av[0]);
	branch = ac < 2 ? "HEAD" : av[1];
	used_new_branch_options = new_branch || new_branch_force;

	எனில் (!strcmp(branch, "-"))
		branch = "@{-1}";

	எனில் (new_branch_force) {
		கட்டமைப்பு strbuf symref = STRBUF_INIT;

		new_branch = new_branch_force;

		எனில் (!opts.force &&
		    !check_branch_ref(the_repository, &symref, new_branch) &&
		    refs_ref_exists(get_main_ref_store(the_repository), symref.buf))
			die_if_checked_out(symref.buf, 0);
		strbuf_release(&symref);
	}

	எனில் (opts.orphan && !new_branch) {
		new_branch = new_branch_to_free = worktree_basename_dup(path);
	} இல்லையெனில் எனில் (opts.orphan) {
		; /* no-op */
	} இல்லையெனில் எனில் (opts.detach) {
		/* Check HEAD */
		எனில் (!strcmp(branch, "HEAD"))
			can_use_local_refs(&opts);
	} இல்லையெனில் எனில் (ac < 2 && new_branch) {
		/* DWIM: Infer --orphan when repo has no refs. */
		opts.orphan = dwim_orphan(&opts, !!opt_track, 0);
	} இல்லையெனில் எனில் (ac < 2) {
		/* DWIM: Guess branch name from path. */
		எழுத்து *s = dwim_branch(&opts, path, &new_branch_to_free);
		எனில் (s)
			branch = branch_to_free = s;
		new_branch = new_branch_to_free;

		/* DWIM: Infer --orphan when repo has no refs. */
		opts.orphan = (!s) && dwim_orphan(&opts, !!opt_track, 1);
	} இல்லையெனில் எனில் (ac == 2 && new_branch) {
		எனில் (!strcmp(branch, "HEAD"))
			can_use_local_refs(&opts);
	} இல்லையெனில் எனில் (ac == 2) {
		கட்டமைப்பு commit *commit;

		commit = lookup_commit_reference_by_name(branch);
		எனில் (!commit) {
			கட்டமைப்பு object_id oid;
			எழுத்து *remote;
			முழு இலக்கம்_matches = 0;
			கட்டமைப்பு string_list matched_remote_names = STRING_LIST_INIT_DUP;

			remote = unique_tracking_name(branch, &oid, &இலக்கம்_matches,
						      &matched_remote_names);
			எனில் (remote) {
				new_branch = branch;
				branch = new_branch_to_free = remote;
			} இல்லையெனில் எனில் (இலக்கம்_matches > 1) {
				எனில் (!opts.quiet &&
				    advice_enabled(ADVICE_CHECKOUT_AMBIGUOUS_REMOTE_BRANCH_NAME))
					advise_disambiguating_remotes(path, branch,
								      &matched_remote_names);
				die(_("'%s' matched multiple (%d) remote tracking branches"),
				    branch, இலக்கம்_matches);
			}
			string_list_clear(&matched_remote_names, 0);
		}

		எனில் (!strcmp(branch, "HEAD"))
			can_use_local_refs(&opts);

	}

	எனில் (!opts.orphan && !lookup_commit_reference_by_name(branch)) {
		முழு attempt_hint = !opts.quiet && (ac < 2);
		எனில் (attempt_hint && used_new_branch_options) {
			advise_if_enabled(ADVICE_WORKTREE_ADD_ORPHAN,
				WORKTREE_ADD_ORPHAN_WITH_DASH_B_HINT_TEXT,
				new_branch, path);
		} இல்லையெனில் எனில் (attempt_hint) {
			advise_if_enabled(ADVICE_WORKTREE_ADD_ORPHAN,
				WORKTREE_ADD_ORPHAN_NO_DASH_B_HINT_TEXT, path);
		}
		die(_("invalid reference: %s"), branch);
	}

	எனில் (!opts.quiet)
		print_preparing_worktree_வரி(opts.detach, branch, new_branch, !!new_branch_force);

	எனில் (opts.orphan) {
		branch = new_branch;
	} இல்லையெனில் எனில் (new_branch) {
		கட்டமைப்பு child_process cp = CHILD_PROCESS_INIT;
		cp.git_cmd = 1;
		strvec_தள்ளு(&cp.args, "branch");
		எனில் (new_branch_force)
			strvec_தள்ளு(&cp.args, "--force");
		எனில் (opts.quiet)
			strvec_தள்ளு(&cp.args, "--quiet");
		strvec_தள்ளு(&cp.args, new_branch);
		strvec_தள்ளு(&cp.args, branch);
		எனில் (opt_track)
			strvec_தள்ளு(&cp.args, opt_track);
		எனில் (run_command(&cp)) {
			ret = -1;
			செல் cleanup;
		}
		branch = new_branch;
	} இல்லையெனில் எனில் (opt_track) {
		die(_("--[no-]track can only be used if a new branch is created"));
	}

	ret = add_worktree(path, branch, &opts);
cleanup:
	விடுவி(path);
	விடுவி(opt_track);
	விடுவி(branch_to_free);
	விடுவி(new_branch_to_free);
	திரும்பு ret;
}

நிலையான வெறுமை show_worktree_porcelain(கட்டமைப்பு worktree *wt, முழு வரி_terminator)
{
	மாறிலி எழுத்து *reason;

	printf("worktree %s%c", wt->path, வரி_terminator);
	எனில் (wt->is_bare)
		printf("bare%c", வரி_terminator);
	இல்லையெனில் {
		printf("HEAD %s%c", oid_to_hex(&wt->head_oid), வரி_terminator);
		எனில் (wt->is_detached)
			printf("detached%c", வரி_terminator);
		இல்லையெனில் எனில் (wt->head_ref)
			printf("branch %s%c", wt->head_ref, வரி_terminator);
	}

	reason = worktree_lock_reason(wt);
	எனில் (reason) {
		fputs("locked", stdout);
		எனில் (*reason) {
			fputc(' ', stdout);
			write_name_quoted(reason, stdout, வரி_terminator);
		} இல்லையெனில் {
			fputc(வரி_terminator, stdout);
		}
	}

	reason = worktree_prune_reason(wt, expire);
	எனில் (reason)
		printf("prunable %s%c", reason, வரி_terminator);

	fputc(வரி_terminator, stdout);
}

கட்டமைப்பு worktree_display {
	எழுத்து *path;
	முழு width;
};

நிலையான வெறுமை show_worktree(கட்டமைப்பு worktree *wt, கட்டமைப்பு worktree_display *display,
			  முழு path_maxwidth, முழு abbrev_len)
{
	கட்டமைப்பு strbuf sb = STRBUF_INIT;
	மாறிலி எழுத்து *reason;

	strbuf_addf(&sb, "%s%*s", display->path, 1 + path_maxwidth - display->width, "");
	எனில் (wt->is_bare)
		strbuf_addstr(&sb, "(bare)");
	இல்லையெனில் {
		strbuf_addf(&sb, "%-*s ", abbrev_len,
				repo_find_unique_abbrev(the_repository, &wt->head_oid, DEFAULT_ABBREV));
		எனில் (wt->is_detached)
			strbuf_addstr(&sb, "(detached HEAD)");
		இல்லையெனில் எனில் (wt->head_ref) {
			எழுத்து *ref = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
								 wt->head_ref,
								 0);
			strbuf_addf(&sb, "[%s]", ref);
			விடுவி(ref);
		} இல்லையெனில்
			strbuf_addstr(&sb, "(error)");
	}

	reason = worktree_lock_reason(wt);
	எனில் (verbose && reason && *reason)
		strbuf_addf(&sb, "\n\tlocked: %s", reason);
	இல்லையெனில் எனில் (reason)
		strbuf_addstr(&sb, " locked");

	reason = worktree_prune_reason(wt, expire);
	எனில் (verbose && reason)
		strbuf_addf(&sb, "\n\tprunable: %s", reason);
	இல்லையெனில் எனில் (reason)
		strbuf_addstr(&sb, " prunable");

	printf("%s\n", sb.buf);
	strbuf_release(&sb);
}

நிலையான வெறுமை measure_widths(கட்டமைப்பு worktree **wt, முழு *abbrev,
			   கட்டமைப்பு worktree_display **d, முழு *maxwidth)
{
	முழு i, display_alloc = 0;
	கட்டமைப்பு worktree_display *display = வெற்று;
	கட்டமைப்பு strbuf buf = STRBUF_INIT;

	க்கு (i = 0; wt[i]; i++) {
		முழு sha1_len;
		ALLOC_GROW(display, i + 1, display_alloc);
		quote_path(wt[i]->path, வெற்று, &buf, 0);
		display[i].width = utf8_strwidth(buf.buf);
		display[i].path = strbuf_detach(&buf, வெற்று);

		எனில் (display[i].width > *maxwidth)
			*maxwidth = display[i].width;
		sha1_len = strlen(repo_find_unique_abbrev(the_repository, &wt[i]->head_oid, *abbrev));
		எனில் (sha1_len > *abbrev)
			*abbrev = sha1_len;
	}
	*d = display;
}

நிலையான முழு pathcmp(மாறிலி வெறுமை *a_, மாறிலி வெறுமை *b_)
{
	மாறிலி கட்டமைப்பு worktree *மாறிலி *a = a_;
	மாறிலி கட்டமைப்பு worktree *மாறிலி *b = b_;
	திரும்பு fspathcmp((*a)->path, (*b)->path);
}

நிலையான வெறுமை pathsort(கட்டமைப்பு worktree **wt)
{
	முழு n = 0;
	கட்டமைப்பு worktree **p = wt;

	போது (*p++)
		n++;
	QSORT(wt, n, pathcmp);
}

நிலையான முழு list(முழு ac, மாறிலி எழுத்து **av, மாறிலி எழுத்து *prefix,
		கட்டமைப்பு repository *repo UNUSED)
{
	முழு porcelain = 0;
	முழு வரி_terminator = '\n';

	கட்டமைப்பு option options[] = {
		OPT_BOOL(0, "porcelain", &porcelain, N_("machine-readable output")),
		OPT__VERBOSE(&verbose, N_("show extended annotations and reasons, if available")),
		OPT_EXPIRY_DATE(0, "expire", &expire,
				N_("add 'prunable' annotation to missing worktrees older than <time>")),
		OPT_SET_INT('z', வெற்று, &வரி_terminator,
			    N_("terminate records with a NUL character"), '\0'),
		OPT_END()
	};

	expire = TIME_MAX;
	ac = parse_options(ac, av, prefix, options, git_worktree_list_usage, 0);
	எனில் (ac)
		usage_with_options(git_worktree_list_usage, options);
	இல்லையெனில் எனில் (verbose && porcelain)
		die(_("options '%s' and '%s' cannot be used together"), "--verbose", "--porcelain");
	இல்லையெனில் எனில் (!வரி_terminator && !porcelain)
		die(_("the option '%s' requires '%s'"), "-z", "--porcelain");
	இல்லையெனில் {
		கட்டமைப்பு worktree **worktrees = get_worktrees(the_repository);
		முழு path_maxwidth = 0, abbrev = DEFAULT_ABBREV, i;
		கட்டமைப்பு worktree_display *display = வெற்று;

		/* sort worktrees by path but keep main worktree at top */
		pathsort(worktrees + 1);

		எனில் (!porcelain)
			measure_widths(worktrees, &abbrev,
				       &display, &path_maxwidth);

		க்கு (i = 0; worktrees[i]; i++) {
			எனில் (porcelain)
				show_worktree_porcelain(worktrees[i],
							வரி_terminator);
			இல்லையெனில்
				show_worktree(worktrees[i],
					      &display[i], path_maxwidth, abbrev);
		}
		க்கு (i = 0; display && worktrees[i]; i++)
			விடுவி(display[i].path);
		விடுவி(display);
		free_worktrees(worktrees);
	}
	திரும்பு 0;
}

நிலையான முழு lock_worktree(முழு ac, மாறிலி எழுத்து **av, மாறிலி எழுத்து *prefix,
			 கட்டமைப்பு repository *repo UNUSED)
{
	மாறிலி எழுத்து *reason = "", *old_reason;
	கட்டமைப்பு option options[] = {
		OPT_STRING(0, "reason", &reason, N_("string"),
			   N_("reason for locking")),
		OPT_END()
	};
	கட்டமைப்பு worktree **worktrees, *wt;
	எழுத்து *path;

	ac = parse_options(ac, av, prefix, options, git_worktree_lock_usage, 0);
	எனில் (ac != 1)
		usage_with_options(git_worktree_lock_usage, options);

	worktrees = get_worktrees(the_repository);
	wt = find_worktree(worktrees, prefix, av[0]);
	எனில் (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	எனில் (is_main_worktree(wt))
		die(_("The main working tree cannot be locked or unlocked"));

	old_reason = worktree_lock_reason(wt);
	எனில் (old_reason) {
		எனில் (*old_reason)
			die(_("'%s' is already locked, reason: %s"),
			    av[0], old_reason);
		die(_("'%s' is already locked"), av[0]);
	}

	path = repo_common_path(the_repository, "worktrees/%s/locked", wt->id);
	write_file(path, "%s", reason);

	free_worktrees(worktrees);
	விடுவி(path);
	திரும்பு 0;
}

நிலையான முழு unlock_worktree(முழு ac, மாறிலி எழுத்து **av, மாறிலி எழுத்து *prefix,
			   கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு option options[] = {
		OPT_END()
	};
	கட்டமைப்பு worktree **worktrees, *wt;
	எழுத்து *path;
	முழு ret;

	ac = parse_options(ac, av, prefix, options, git_worktree_unlock_usage, 0);
	எனில் (ac != 1)
		usage_with_options(git_worktree_unlock_usage, options);

	worktrees = get_worktrees(the_repository);
	wt = find_worktree(worktrees, prefix, av[0]);
	எனில் (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	எனில் (is_main_worktree(wt))
		die(_("The main working tree cannot be locked or unlocked"));
	எனில் (!worktree_lock_reason(wt))
		die(_("'%s' is not locked"), av[0]);

	path = repo_common_path(the_repository, "worktrees/%s/locked", wt->id);
	ret = unlink_or_warn(path);

	free_worktrees(worktrees);
	விடுவி(path);
	திரும்பு ret;
}

நிலையான வெறுமை validate_no_submodules(மாறிலி கட்டமைப்பு worktree *wt)
{
	கட்டமைப்பு index_state istate = INDEX_STATE_INIT(the_repository);
	கட்டமைப்பு strbuf path = STRBUF_INIT;
	முழு i, found_submodules = 0;
	எழுத்து *wt_gitdir;

	wt_gitdir = get_worktree_git_dir(wt);

	எனில் (is_directory(worktree_git_path(wt, "modules"))) {
		/*
		 * There could be false positives, e.g. the "modules"
		 * directory exists but is empty. But it's a rare case and
		 * this simpler check is probably good enough for now.
		 */
		found_submodules = 1;
	} இல்லையெனில் எனில் (read_index_from(&istate, worktree_git_path(wt, "index"),
				   wt_gitdir) > 0) {
		க்கு (i = 0; i < istate.cache_nr; i++) {
			கட்டமைப்பு cache_entry *ce = istate.cache[i];
			முழு err;

			எனில் (!S_ISGITLINK(ce->ce_mode))
				தொடர்;

			strbuf_reset(&path);
			strbuf_addf(&path, "%s/%s", wt->path, ce->name);
			எனில் (!is_submodule_populated_gently(path.buf, &err))
				தொடர்;

			found_submodules = 1;
			நிறுத்து;
		}
	}
	discard_index(&istate);
	strbuf_release(&path);
	விடுவி(wt_gitdir);

	எனில் (found_submodules)
		die(_("working trees containing submodules cannot be moved or removed"));
}

நிலையான முழு move_worktree(முழு ac, மாறிலி எழுத்து **av, மாறிலி எழுத்து *prefix,
			 கட்டமைப்பு repository *repo UNUSED)
{
	முழு force = 0;
	கட்டமைப்பு option options[] = {
		OPT__FORCE(&force,
			 N_("force move even if worktree is dirty or locked"),
			 PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "relative-paths", &use_relative_paths,
			 N_("use relative paths for worktrees")),
		OPT_END()
	};
	கட்டமைப்பு worktree **worktrees, *wt;
	கட்டமைப்பு strbuf dst = STRBUF_INIT;
	கட்டமைப்பு strbuf errmsg = STRBUF_INIT;
	மாறிலி எழுத்து *reason = வெற்று;
	எழுத்து *path;

	ac = parse_options(ac, av, prefix, options, git_worktree_move_usage,
			   0);
	எனில் (ac != 2)
		usage_with_options(git_worktree_move_usage, options);

	path = prefix_filename(prefix, av[1]);
	strbuf_addstr(&dst, path);
	விடுவி(path);

	worktrees = get_worktrees(the_repository);
	wt = find_worktree(worktrees, prefix, av[0]);
	எனில் (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	எனில் (is_main_worktree(wt))
		die(_("'%s' is a main working tree"), av[0]);
	எனில் (is_directory(dst.buf)) {
		மாறிலி எழுத்து *sep = find_last_dir_sep(wt->path);

		எனில் (!sep)
			die(_("could not figure out destination name from '%s'"),
			    wt->path);
		strbuf_trim_trailing_dir_sep(&dst);
		strbuf_addstr(&dst, sep);
	}
	check_candidate_path(dst.buf, force, worktrees, "move");

	validate_no_submodules(wt);

	எனில் (force < 2)
		reason = worktree_lock_reason(wt);
	எனில் (reason) {
		எனில் (*reason)
			die(_("cannot move a locked working tree, lock reason: %s\nuse 'move -f -f' to override or unlock first"),
			    reason);
		die(_("cannot move a locked working tree;\nuse 'move -f -f' to override or unlock first"));
	}
	எனில் (validate_worktree(wt, &errmsg, 0))
		die(_("validation failed, cannot move working tree: %s"),
		    errmsg.buf);
	strbuf_release(&errmsg);

	எனில் (rename(wt->path, dst.buf) == -1)
		die_errno(_("failed to move '%s' to '%s'"), wt->path, dst.buf);

	update_worktree_location(wt, dst.buf, use_relative_paths);

	strbuf_release(&dst);
	free_worktrees(worktrees);
	திரும்பு 0;
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
நிலையான வெறுமை check_clean_worktree(கட்டமைப்பு worktree *wt,
				 மாறிலி எழுத்து *original_path)
{
	கட்டமைப்பு child_process cp;
	எழுத்து buf[1];
	முழு ret;

	/*
	 * Until we sort this out, all submodules are "dirty" and
	 * will abort this function.
	 */
	validate_no_submodules(wt);

	child_process_init(&cp);
	strvec_pushf(&cp.env, "%s=%s/.git",
		     GIT_DIR_ENVIRONMENT, wt->path);
	strvec_pushf(&cp.env, "%s=%s",
		     GIT_WORK_மரம்_ENVIRONMENT, wt->path);
	strvec_pushl(&cp.args, "status",
		     "--porcelain", "--ignore-submodules=none",
		     வெற்று);
	cp.git_cmd = 1;
	cp.dir = wt->path;
	cp.out = -1;
	ret = start_command(&cp);
	எனில் (ret)
		die_errno(_("failed to run 'git status' on '%s'"),
			  original_path);
	ret = xread(cp.out, buf, அளவு(buf));
	எனில் (ret)
		die(_("'%s' contains modified or untracked files, use --force to delete it"),
		    original_path);
	close(cp.out);
	ret = finish_command(&cp);
	எனில் (ret)
		die_errno(_("failed to run 'git status' on '%s', code %d"),
			  original_path, ret);
}

நிலையான முழு delete_git_work_மரம்(கட்டமைப்பு worktree *wt)
{
	கட்டமைப்பு strbuf sb = STRBUF_INIT;
	முழு ret = 0;

	strbuf_addstr(&sb, wt->path);
	எனில் (remove_dir_recursively(&sb, 0)) {
		error_errno(_("failed to delete '%s'"), sb.buf);
		ret = -1;
	}
	strbuf_release(&sb);
	திரும்பு ret;
}

நிலையான முழு remove_worktree(முழு ac, மாறிலி எழுத்து **av, மாறிலி எழுத்து *prefix,
			   கட்டமைப்பு repository *repo UNUSED)
{
	முழு force = 0;
	கட்டமைப்பு option options[] = {
		OPT__FORCE(&force,
			 N_("force removal even if worktree is dirty or locked"),
			 PARSE_OPT_NOCOMPLETE),
		OPT_END()
	};
	கட்டமைப்பு worktree **worktrees, *wt;
	கட்டமைப்பு strbuf errmsg = STRBUF_INIT;
	மாறிலி எழுத்து *reason = வெற்று;
	முழு ret = 0;

	ac = parse_options(ac, av, prefix, options, git_worktree_remove_usage, 0);
	எனில் (ac != 1)
		usage_with_options(git_worktree_remove_usage, options);

	worktrees = get_worktrees(the_repository);
	wt = find_worktree(worktrees, prefix, av[0]);
	எனில் (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	எனில் (is_main_worktree(wt))
		die(_("'%s' is a main working tree"), av[0]);
	எனில் (force < 2)
		reason = worktree_lock_reason(wt);
	எனில் (reason) {
		எனில் (*reason)
			die(_("cannot remove a locked working tree, lock reason: %s\nuse 'remove -f -f' to override or unlock first"),
			    reason);
		die(_("cannot remove a locked working tree;\nuse 'remove -f -f' to override or unlock first"));
	}
	எனில் (validate_worktree(wt, &errmsg, WT_VALIDATE_WORKTREE_MISSING_OK))
		die(_("validation failed, cannot remove working tree: %s"),
		    errmsg.buf);
	strbuf_release(&errmsg);

	எனில் (file_exists(wt->path)) {
		எனில் (!force)
			check_clean_worktree(wt, av[0]);

		ret |= delete_git_work_மரம்(wt);
	}
	/*
	 * continue on even if ret is non-zero, there's no going back
	 * from here.
	 */
	ret |= delete_git_dir(wt->id);
	delete_worktrees_dir_if_empty();

	free_worktrees(worktrees);
	திரும்பு ret;
}

நிலையான வெறுமை report_repair(முழு iserr, மாறிலி எழுத்து *path, மாறிலி எழுத்து *செய்தி, வெறுமை *cb_data)
{
	எனில் (!iserr) {
		fprintf_ln(stderr, _("repair: %s: %s"), செய்தி, path);
	} இல்லையெனில் {
		முழு *exit_status = (முழு *)cb_data;
		fprintf_ln(stderr, _("error: %s: %s"), செய்தி, path);
		*exit_status = 1;
	}
}

நிலையான முழு repair(முழு ac, மாறிலி எழுத்து **av, மாறிலி எழுத்து *prefix,
		  கட்டமைப்பு repository *repo UNUSED)
{
	மாறிலி எழுத்து **p;
	மாறிலி எழுத்து *self[] = { ".", வெற்று };
	கட்டமைப்பு option options[] = {
		OPT_BOOL(0, "relative-paths", &use_relative_paths,
			 N_("use relative paths for worktrees")),
		OPT_END()
	};
	முழு rc = 0;

	ac = parse_options(ac, av, prefix, options, git_worktree_repair_usage, 0);
	p = ac > 0 ? av : self;
	க்கு (; *p; p++)
		repair_worktree_at_path(the_repository, *p, report_repair,
					&rc, use_relative_paths);
	repair_worktrees(the_repository, report_repair, &rc, use_relative_paths);
	திரும்பு rc;
}

முழு cmd_worktree(முழு ac,
		 மாறிலி எழுத்து **av,
		 மாறிலி எழுத்து *prefix,
		 கட்டமைப்பு repository *repo)
{
	parse_opt_subcommand_fn *fn = வெற்று;
	கட்டமைப்பு option options[] = {
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

	repo_config(the_repository, git_worktree_config, வெற்று);

	எனில் (!prefix)
		prefix = "";

	ac = parse_options(ac, av, prefix, options, git_worktree_usage, 0);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	திரும்பு fn(ac, av, prefix, repo);
}
