#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "abspath.h"
#অন্তর্ভাব "advice.h"
#অন্তর্ভাব "checkout.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "copy.h"
#অন্তর্ভাব "dir.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "hex.h"
#অন্তর্ভাব "object-file.h"
#অন্তর্ভাব "object-name.h"
#অন্তর্ভাব "parse-options.h"
#অন্তর্ভাব "path.h"
#অন্তর্ভাব "strvec.h"
#অন্তর্ভাব "branch.h"
#অন্তর্ভাব "read-cache-ll.h"
#অন্তর্ভাব "refs.h"
#অন্তর্ভাব "remote.h"
#অন্তর্ভাব "run-command.h"
#অন্তর্ভাব "hook.h"
#অন্তর্ভাব "sigchain.h"
#অন্তর্ভাব "submodule.h"
#অন্তর্ভাব "utf8.h"
#অন্তর্ভাব "worktree.h"
#অন্তর্ভাব "quote.h"

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

স্থির ধ্রুবক অক্ষর * ধ্রুবক git_worktree_usage[] = {
	BUILTIN_WORKTREE_ADD_USAGE,
	BUILTIN_WORKTREE_LIST_USAGE,
	BUILTIN_WORKTREE_LOCK_USAGE,
	BUILTIN_WORKTREE_MOVE_USAGE,
	BUILTIN_WORKTREE_PRUNE_USAGE,
	BUILTIN_WORKTREE_REMOVE_USAGE,
	BUILTIN_WORKTREE_REPAIR_USAGE,
	BUILTIN_WORKTREE_UNLOCK_USAGE,
	ফাঁকা
};

স্থির ধ্রুবক অক্ষর * ধ্রুবক git_worktree_add_usage[] = {
	BUILTIN_WORKTREE_ADD_USAGE,
	ফাঁকা,
};

স্থির ধ্রুবক অক্ষর * ধ্রুবক git_worktree_list_usage[] = {
	BUILTIN_WORKTREE_LIST_USAGE,
	ফাঁকা
};

স্থির ধ্রুবক অক্ষর * ধ্রুবক git_worktree_lock_usage[] = {
	BUILTIN_WORKTREE_LOCK_USAGE,
	ফাঁকা
};

স্থির ধ্রুবক অক্ষর * ধ্রুবক git_worktree_move_usage[] = {
	BUILTIN_WORKTREE_MOVE_USAGE,
	ফাঁকা
};

স্থির ধ্রুবক অক্ষর * ধ্রুবক git_worktree_prune_usage[] = {
	BUILTIN_WORKTREE_PRUNE_USAGE,
	ফাঁকা
};

স্থির ধ্রুবক অক্ষর * ধ্রুবক git_worktree_remove_usage[] = {
	BUILTIN_WORKTREE_REMOVE_USAGE,
	ফাঁকা
};

স্থির ধ্রুবক অক্ষর * ধ্রুবক git_worktree_repair_usage[] = {
	BUILTIN_WORKTREE_REPAIR_USAGE,
	ফাঁকা
};

স্থির ধ্রুবক অক্ষর * ধ্রুবক git_worktree_unlock_usage[] = {
	BUILTIN_WORKTREE_UNLOCK_USAGE,
	ফাঁকা
};

কাঠামো add_opts {
	পূর্ণসংখ্যা force;
	পূর্ণসংখ্যা detach;
	পূর্ণসংখ্যা quiet;
	পূর্ণসংখ্যা checkout;
	পূর্ণসংখ্যা orphan;
	পূর্ণসংখ্যা relative_paths;
	ধ্রুবক অক্ষর *keep_locked;
};

স্থির পূর্ণসংখ্যা show_only;
স্থির পূর্ণসংখ্যা verbose;
স্থির পূর্ণসংখ্যা guess_remote;
স্থির পূর্ণসংখ্যা use_relative_paths;
স্থির timestamp_t expire;

স্থির পূর্ণসংখ্যা git_worktree_config(ধ্রুবক অক্ষর *var, ধ্রুবক অক্ষর *মান,
			       ধ্রুবক কাঠামো config_context *ctx, শূন্য *cb)
{
	যদি (!strcmp(var, "worktree.guessremote")) {
		guess_remote = git_config_bool(var, মান);
		ফেরত 0;
	} অন্যথা যদি (!strcmp(var, "worktree.userelativepaths")) {
		use_relative_paths = git_config_bool(var, মান);
		ফেরত 0;
	}

	ফেরত git_default_config(var, মান, ctx, cb);
}

স্থির পূর্ণসংখ্যা delete_git_dir(ধ্রুবক অক্ষর *id)
{
	কাঠামো strbuf sb = STRBUF_INIT;
	পূর্ণসংখ্যা ret;

	repo_common_path_append(the_repository, &sb, "worktrees/%s", id);
	ret = remove_dir_recursively(&sb, 0);
	যদি (ret < 0 && errno == ENOTDIR)
		ret = unlink(sb.buf);
	যদি (ret)
		error_errno(_("failed to delete '%s'"), sb.buf);
	strbuf_release(&sb);
	ফেরত ret;
}

স্থির শূন্য delete_worktrees_dir_if_empty(শূন্য)
{
	অক্ষর *path = repo_git_path(the_repository, "worktrees");
	rmdir(path); /* ignore failed removal */
	মুক্ত(path);
}

স্থির শূন্য prune_worktree(ধ্রুবক অক্ষর *id, ধ্রুবক অক্ষর *reason)
{
	যদি (show_only || verbose)
		fprintf_ln(stderr, _("Removing %s/%s: %s"), "worktrees", id, reason);
	যদি (!show_only)
		delete_git_dir(id);
}

স্থির পূর্ণসংখ্যা prune_cmp(ধ্রুবক শূন্য *a, ধ্রুবক শূন্য *b)
{
	ধ্রুবক কাঠামো string_list_item *x = a;
	ধ্রুবক কাঠামো string_list_item *y = b;
	পূর্ণসংখ্যা c;

	যদি ((c = fspathcmp(x->string, y->string)))
	    ফেরত c;
	/*
	 * paths same; prune_dupes() removes all but the first worktree entry
	 * having the same path, so sort main worktree ('util' is NULL) above
	 * linked worktrees ('util' not NULL) since main worktree can't be
	 * removed
	 */
	যদি (!x->util)
		ফেরত -1;
	যদি (!y->util)
		ফেরত 1;
	/* paths same; sort by .git/worktrees/<id> */
	ফেরত strcmp(x->util, y->util);
}

স্থির শূন্য prune_dups(কাঠামো string_list *l)
{
	পূর্ণসংখ্যা i;

	QSORT(l->বস্তুসমূহ, l->nr, prune_cmp);
	জন্য (i = 1; i < l->nr; i++) {
		যদি (!fspathcmp(l->বস্তুসমূহ[i].string, l->বস্তুসমূহ[i - 1].string))
			prune_worktree(l->বস্তুসমূহ[i].util, "duplicate entry");
	}
}

স্থির শূন্য prune_worktrees(শূন্য)
{
	কাঠামো strbuf reason = STRBUF_INIT;
	কাঠামো strbuf main_path = STRBUF_INIT;
	কাঠামো string_list kept = STRING_LIST_INIT_DUP;
	অক্ষর *path;
	DIR *dir;
	কাঠামো dirent *d;

	path = repo_git_path(the_repository, "worktrees");
	dir = opendir(path);
	মুক্ত(path);
	যদি (!dir)
		ফেরত;
	যখন ((d = readdir_skip_dot_and_dotdot(dir)) != ফাঁকা) {
		অক্ষর *path;
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
	string_list_append_nodup(&kept, strbuf_detach(&main_path, ফাঁকা));
	prune_dups(&kept);
	string_list_clear(&kept, 1);

	যদি (!show_only)
		delete_worktrees_dir_if_empty();
	strbuf_release(&reason);
}

স্থির পূর্ণসংখ্যা prune(পূর্ণসংখ্যা ac, ধ্রুবক অক্ষর **av, ধ্রুবক অক্ষর *prefix,
		 কাঠামো repository *repo UNUSED)
{
	কাঠামো option options[] = {
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
	ফেরত 0;
}

স্থির অক্ষর *junk_work_বৃক্ষ;
স্থির অক্ষর *junk_git_dir;
স্থির পূর্ণসংখ্যা is_junk;
স্থির pid_t junk_pid;

স্থির শূন্য remove_junk(শূন্য)
{
	কাঠামো strbuf sb = STRBUF_INIT;
	যদি (!is_junk || getpid() != junk_pid)
		ফেরত;
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

স্থির শূন্য remove_junk_on_signal(পূর্ণসংখ্যা signo)
{
	remove_junk();
	sigchain_বের_করো(signo);
	তোলো(signo);
}

স্থির অক্ষর *worktree_basename_dup(ধ্রুবক অক্ষর *path)
{
	ধ্রুবক অক্ষর *name;
	পূর্ণসংখ্যা len;

	len = strlen(path);
	যখন (len && is_dir_sep(path[len - 1]))
		len--;

	name = path + len;
	যখন (name > path && !is_dir_sep(name[-1]))
		name--;

	ফেরত xmemdupz(name, path + len - name);
}

/* check that path is viable location for worktree */
স্থির শূন্য check_candidate_path(ধ্রুবক অক্ষর *path,
				 পূর্ণসংখ্যা force,
				 কাঠামো worktree **worktrees,
				 ধ্রুবক অক্ষর *cmd)
{
	কাঠামো worktree *wt;
	পূর্ণসংখ্যা locked;

	যদি (file_exists(path) && !is_empty_dir(path))
		die(_("'%s' already exists"), path);

	wt = find_worktree_by_path(worktrees, path);
	যদি (!wt)
		ফেরত;

	locked = !!worktree_lock_reason(wt);
	যদি ((!locked && force) || (locked && force > 1)) {
		যদি (delete_git_dir(wt->id))
		    die(_("unusable worktree destination '%s'"), path);
		ফেরত;
	}

	যদি (locked)
		die(_("'%s' is a missing but locked worktree;\nuse '%s -f -f' to override, or 'unlock' and 'prune' or 'remove' to clear"), path, cmd);
	অন্যথা
		die(_("'%s' is a missing but already registered worktree;\nuse '%s -f' to override, or 'prune' or 'remove' to clear"), path, cmd);
}

স্থির শূন্য copy_sparse_checkout(ধ্রুবক অক্ষর *worktree_git_dir)
{
	অক্ষর *from_file = repo_git_path(the_repository, "info/sparse-checkout");
	অক্ষর *to_file = xstrfmt("%s/info/sparse-checkout", worktree_git_dir);

	যদি (file_exists(from_file)) {
		যদি (safe_create_leading_directories(the_repository, to_file) ||
			copy_file(the_repository, to_file, from_file, 0666))
			error(_("failed to copy '%s' to '%s'; sparse-checkout may not work correctly"),
				from_file, to_file);
	}

	মুক্ত(from_file);
	মুক্ত(to_file);
}

স্থির শূন্য copy_filtered_worktree_config(ধ্রুবক অক্ষর *worktree_git_dir)
{
	অক্ষর *from_file = repo_git_path(the_repository, "config.worktree");
	অক্ষর *to_file = xstrfmt("%s/config.worktree", worktree_git_dir);

	যদি (file_exists(from_file)) {
		কাঠামো config_set cs = { { 0 } };
		পূর্ণসংখ্যা bare;

		যদি (safe_create_leading_directories(the_repository, to_file) ||
			copy_file(the_repository, to_file, from_file, 0666)) {
			error(_("failed to copy worktree config from '%s' to '%s'"),
				from_file, to_file);
			যাও worktree_copy_cleanup;
		}

		git_configset_init(&cs);
		git_configset_add_file(&cs, from_file);

		যদি (!git_configset_get_bool(&cs, "core.bare", &bare) &&
			bare &&
			repo_config_set_multivar_in_file_gently(the_repository,
				to_file, "core.bare", ফাঁকা, "true", ফাঁকা, 0))
			error(_("failed to unset '%s' in '%s'"),
				"core.bare", to_file);
		যদি (!git_configset_get(&cs, "core.worktree") &&
			repo_config_set_in_file_gently(the_repository, to_file,
						       "core.worktree", ফাঁকা, ফাঁকা))
			error(_("failed to unset '%s' in '%s'"),
				"core.worktree", to_file);

		git_configset_clear(&cs);
	}

worktree_copy_cleanup:
	মুক্ত(from_file);
	মুক্ত(to_file);
}

স্থির পূর্ণসংখ্যা checkout_worktree(ধ্রুবক কাঠামো add_opts *opts,
			     কাঠামো strvec *child_env)
{
	কাঠামো child_process cp = CHILD_PROCESS_INIT;
	cp.git_cmd = 1;
	strvec_pushl(&cp.args, "reset", "--hard", "--no-recurse-submodules", ফাঁকা);
	যদি (opts->quiet)
		strvec_ঠেলো(&cp.args, "--quiet");
	strvec_pushv(&cp.env, child_env->v);
	ফেরত run_command(&cp);
}

স্থির পূর্ণসংখ্যা make_worktree_orphan(ধ্রুবক অক্ষর * ref, ধ্রুবক কাঠামো add_opts *opts,
				কাঠামো strvec *child_env)
{
	কাঠামো strbuf symref = STRBUF_INIT;
	কাঠামো child_process cp = CHILD_PROCESS_INIT;

	validate_new_branchname(ref, &symref, 0);
	strvec_pushl(&cp.args, "symbolic-ref", "HEAD", symref.buf, ফাঁকা);
	যদি (opts->quiet)
		strvec_ঠেলো(&cp.args, "--quiet");
	strvec_pushv(&cp.env, child_env->v);
	strbuf_release(&symref);
	cp.git_cmd = 1;
	ফেরত run_command(&cp);
}

/*
 * References for worktrees are generally stored in '$GIT_DIR/worktrees/<wt_id>'.
 * But when using alternate reference directories, we want to store the worktree
 * references in '$ALTERNATE_REFERENCE_DIR/worktrees/<wt_id>'.
 *
 * Create the necessary folder structure to facilitate the same. But to ensure
 * that the former path is still considered a Git directory, add stubs.
 */
স্থির শূন্য setup_alternate_ref_dir(কাঠামো worktree *wt, ধ্রুবক অক্ষর *wt_git_path)
{
	কাঠামো strbuf sb = STRBUF_INIT;
	অক্ষর *path;

	path = wt->repo->ref_storage_payload;
	যদি (!path)
		ফেরত;

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

স্থির পূর্ণসংখ্যা add_worktree(ধ্রুবক অক্ষর *path, ধ্রুবক অক্ষর *refname,
			ধ্রুবক কাঠামো add_opts *opts)
{
	কাঠামো strbuf sb_git = STRBUF_INIT, sb_repo = STRBUF_INIT;
	কাঠামো strbuf sb = STRBUF_INIT;
	ধ্রুবক অক্ষর *name;
	অক্ষর *name_to_free = ফাঁকা;
	কাঠামো strvec child_env = STRVEC_INIT;
	unsigned পূর্ণসংখ্যা গণক = 0;
	পূর্ণসংখ্যা len, ret;
	কাঠামো strbuf symref = STRBUF_INIT;
	কাঠামো commit *commit = ফাঁকা;
	পূর্ণসংখ্যা is_branch = 0;
	কাঠামো strbuf sb_name = STRBUF_INIT;
	কাঠামো worktree **worktrees, *wt = ফাঁকা;
	কাঠামো ref_store *wt_refs;
	কাঠামো repo_config_মানসমূহ *cfg = repo_config_মানসমূহ(the_repository);

	worktrees = get_worktrees(the_repository);
	check_candidate_path(path, opts->force, worktrees, "add");
	free_worktrees(worktrees);
	worktrees = ফাঁকা;

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

	যখন (mkdir(sb_repo.buf, 0777)) {
		গণক++;
		যদি ((errno != EEXIST) || !গণক /* overflow */)
			die_errno(_("could not create directory of '%s'"),
				  sb_repo.buf);
		strbuf_setlen(&sb_repo, len);
		strbuf_addf(&sb_repo, "%d", গণক);
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
		যাও done;
	}
	setup_alternate_ref_dir(wt, sb_repo.buf);
	wt_refs = get_worktree_ref_store(wt);

	ret = ref_store_create_on_disk(wt_refs, REF_STORE_CREATE_ON_DISK_IS_WORKTREE, &sb);
	যদি (ret)
		যাও done;

	যদি (!is_branch && commit)
		ret = refs_update_ref(wt_refs, ফাঁকা, "HEAD", &commit->object.oid,
				      ফাঁকা, 0, UPDATE_REFS_বার্তা_ON_ERR);
	অন্যথা
		ret = refs_update_symref(wt_refs, "HEAD", symref.buf, ফাঁকা);
	যদি (ret)
		যাও done;

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
		যাও done;

	যদি (opts->checkout &&
	    (ret = checkout_worktree(opts, &child_env)))
		যাও done;

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
		কাঠামো run_hooks_opt opt = RUN_HOOKS_OPT_INIT_FORCE_SERIAL;

		strvec_pushl(&opt.env, "GIT_DIR", "GIT_WORK_TREE", ফাঁকা);
		strvec_pushl(&opt.args,
			     oid_to_hex(null_oid(the_hash_algo)),
			     oid_to_hex(&commit->object.oid),
			     "1",
			     ফাঁকা);
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
	ফেরত ret;
}

স্থির শূন্য print_preparing_worktree_সারি(পূর্ণসংখ্যা detach,
					  ধ্রুবক অক্ষর *branch,
					  ধ্রুবক অক্ষর *new_branch,
					  পূর্ণসংখ্যা force_new_branch)
{
	যদি (force_new_branch) {
		কাঠামো commit *commit = lookup_commit_reference_by_name(new_branch);
		যদি (!commit)
			fprintf_ln(stderr, _("Preparing worktree (new branch '%s')"), new_branch);
		অন্যথা
			fprintf_ln(stderr, _("Preparing worktree (resetting branch '%s'; was at %s)"),
				  new_branch,
				  repo_find_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV));
	} অন্যথা যদি (new_branch) {
		fprintf_ln(stderr, _("Preparing worktree (new branch '%s')"), new_branch);
	} অন্যথা {
		কাঠামো strbuf s = STRBUF_INIT;
		যদি (!detach && !check_branch_ref(the_repository, &s, branch) &&
		    refs_ref_exists(get_main_ref_store(the_repository), s.buf))
			fprintf_ln(stderr, _("Preparing worktree (checking out '%s')"),
				  branch);
		অন্যথা {
			কাঠামো commit *commit = lookup_commit_reference_by_name(branch);
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
স্থির পূর্ণসংখ্যা first_valid_ref(ধ্রুবক কাঠামো reference *ref UNUSED, শূন্য *cb_তথ্য UNUSED)
{
	ফেরত 1;
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
স্থির পূর্ণসংখ্যা can_use_local_refs(ধ্রুবক কাঠামো add_opts *opts)
{
	যদি (refs_শীর্ষ_ref(get_main_ref_store(the_repository), first_valid_ref, ফাঁকা)) {
		ফেরত 1;
	} অন্যথা যদি (refs_for_each_branch_ref(get_main_ref_store(the_repository), first_valid_ref, ফাঁকা)) {
		যদি (!opts->quiet)
			warning(_("HEAD points to an invalid (or orphaned) reference.\n"));
		ফেরত 1;
	}
	ফেরত 0;
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
স্থির পূর্ণসংখ্যা can_use_remote_refs(ধ্রুবক কাঠামো add_opts *opts)
{
	যদি (!guess_remote) {
		ফেরত 0;
	} অন্যথা যদি (refs_for_each_remote_ref(get_main_ref_store(the_repository), first_valid_ref, ফাঁকা)) {
		ফেরত 1;
	} অন্যথা যদি (!opts->force && remote_get(ফাঁকা)) {
		die(_("No local or remote refs exist despite at least one remote\n"
		      "present, stopping; use 'add -f' to override or fetch a remote first"));
	}
	ফেরত 0;
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
স্থির পূর্ণসংখ্যা dwim_orphan(ধ্রুবক কাঠামো add_opts *opts, পূর্ণসংখ্যা opt_track, পূর্ণসংখ্যা remote)
{
	যদি (can_use_local_refs(opts)) {
		ফেরত 0;
	} অন্যথা যদি (remote && can_use_remote_refs(opts)) {
		ফেরত 0;
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
	ফেরত 1;
}

স্থির শূন্য advise_disambiguating_remotes(ধ্রুবক অক্ষর *path, ধ্রুবক অক্ষর *branch,
					  ধ্রুবক কাঠামো string_list *matched_remote_names)
{
	কাঠামো string_list_item *item;

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

স্থির অক্ষর *dwim_branch(ধ্রুবক কাঠামো add_opts *opts, ধ্রুবক অক্ষর *path, অক্ষর **new_branch)
{
	পূর্ণসংখ্যা branch_exists;
	অক্ষর *branchname = worktree_basename_dup(path);
	কাঠামো strbuf ref = STRBUF_INIT;

	branch_exists = !check_branch_ref(the_repository, &ref, branchname) &&
			refs_ref_exists(get_main_ref_store(the_repository),
					ref.buf);
	strbuf_release(&ref);
	যদি (branch_exists)
		ফেরত branchname;

	*new_branch = branchname;
	যদি (guess_remote) {
		কাঠামো object_id oid;
		অক্ষর *remote;
		পূর্ণসংখ্যা অঙ্ক_matches = 0;
		কাঠামো string_list matched_remote_names = STRING_LIST_INIT_DUP;

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
		ফেরত remote;
	}
	ফেরত ফাঁকা;
}

স্থির পূর্ণসংখ্যা add(পূর্ণসংখ্যা ac, ধ্রুবক অক্ষর **av, ধ্রুবক অক্ষর *prefix,
	       কাঠামো repository *repo UNUSED)
{
	কাঠামো add_opts opts;
	ধ্রুবক অক্ষর *new_branch_force = ফাঁকা;
	অক্ষর *path;
	ধ্রুবক অক্ষর *branch;
	অক্ষর *branch_to_free = ফাঁকা;
	অক্ষর *new_branch_to_free = ফাঁকা;
	ধ্রুবক অক্ষর *new_branch = ফাঁকা;
	অক্ষর *opt_track = ফাঁকা;
	ধ্রুবক অক্ষর *lock_reason = ফাঁকা;
	পূর্ণসংখ্যা keep_locked = 0;
	পূর্ণসংখ্যা used_new_branch_options;
	কাঠামো option options[] = {
		OPT__FORCE(&opts.force,
			   N_("checkout <branch> even if already checked out in other worktree"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_STRING('b', ফাঁকা, &new_branch, N_("branch"),
			   N_("create a new branch")),
		OPT_STRING('B', ফাঁকা, &new_branch_force, N_("branch"),
			   N_("create or reset a branch")),
		OPT_BOOL(0, "orphan", &opts.orphan, N_("create unborn branch")),
		OPT_BOOL('d', "detach", &opts.detach, N_("detach HEAD at named commit")),
		OPT_BOOL(0, "checkout", &opts.checkout, N_("populate the new working tree")),
		OPT_BOOL(0, "lock", &keep_locked, N_("keep the new working tree locked")),
		OPT_STRING(0, "reason", &lock_reason, N_("string"),
			   N_("reason for locking")),
		OPT__QUIET(&opts.quiet, N_("suppress progress reporting")),
		OPT_PASSTHRU(0, "track", &opt_track, ফাঁকা,
			     N_("set up tracking mode (see git-branch(1))"),
			     PARSE_OPT_NOARG | PARSE_OPT_OPTARG),
		OPT_BOOL(0, "guess-remote", &guess_remote,
			 N_("try to match the new branch name with a remote-tracking branch")),
		OPT_BOOL(0, "relative-paths", &opts.relative_paths,
			 N_("use relative paths for worktrees")),
		OPT_END()
	};
	পূর্ণসংখ্যা ret;

	memset(&opts, 0, আকার(opts));
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
		কাঠামো strbuf symref = STRBUF_INIT;

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
		অক্ষর *s = dwim_branch(&opts, path, &new_branch_to_free);
		যদি (s)
			branch = branch_to_free = s;
		new_branch = new_branch_to_free;

		/* DWIM: Infer --orphan when repo has no refs. */
		opts.orphan = (!s) && dwim_orphan(&opts, !!opt_track, 1);
	} অন্যথা যদি (ac == 2 && new_branch) {
		যদি (!strcmp(branch, "HEAD"))
			can_use_local_refs(&opts);
	} অন্যথা যদি (ac == 2) {
		কাঠামো commit *commit;

		commit = lookup_commit_reference_by_name(branch);
		যদি (!commit) {
			কাঠামো object_id oid;
			অক্ষর *remote;
			পূর্ণসংখ্যা অঙ্ক_matches = 0;
			কাঠামো string_list matched_remote_names = STRING_LIST_INIT_DUP;

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
		পূর্ণসংখ্যা attempt_hint = !opts.quiet && (ac < 2);
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
		print_preparing_worktree_সারি(opts.detach, branch, new_branch, !!new_branch_force);

	যদি (opts.orphan) {
		branch = new_branch;
	} অন্যথা যদি (new_branch) {
		কাঠামো child_process cp = CHILD_PROCESS_INIT;
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
			যাও cleanup;
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
	ফেরত ret;
}

স্থির শূন্য show_worktree_porcelain(কাঠামো worktree *wt, পূর্ণসংখ্যা সারি_terminator)
{
	ধ্রুবক অক্ষর *reason;

	printf("worktree %s%c", wt->path, সারি_terminator);
	যদি (wt->is_bare)
		printf("bare%c", সারি_terminator);
	অন্যথা {
		printf("HEAD %s%c", oid_to_hex(&wt->শীর্ষ_oid), সারি_terminator);
		যদি (wt->is_detached)
			printf("detached%c", সারি_terminator);
		অন্যথা যদি (wt->শীর্ষ_ref)
			printf("branch %s%c", wt->শীর্ষ_ref, সারি_terminator);
	}

	reason = worktree_lock_reason(wt);
	যদি (reason) {
		fputs("locked", stdout);
		যদি (*reason) {
			fputc(' ', stdout);
			write_name_quoted(reason, stdout, সারি_terminator);
		} অন্যথা {
			fputc(সারি_terminator, stdout);
		}
	}

	reason = worktree_prune_reason(wt, expire);
	যদি (reason)
		printf("prunable %s%c", reason, সারি_terminator);

	fputc(সারি_terminator, stdout);
}

কাঠামো worktree_display {
	অক্ষর *path;
	পূর্ণসংখ্যা width;
};

স্থির শূন্য show_worktree(কাঠামো worktree *wt, কাঠামো worktree_display *display,
			  পূর্ণসংখ্যা path_maxwidth, পূর্ণসংখ্যা abbrev_len)
{
	কাঠামো strbuf sb = STRBUF_INIT;
	ধ্রুবক অক্ষর *reason;

	strbuf_addf(&sb, "%s%*s", display->path, 1 + path_maxwidth - display->width, "");
	যদি (wt->is_bare)
		strbuf_addstr(&sb, "(bare)");
	অন্যথা {
		strbuf_addf(&sb, "%-*s ", abbrev_len,
				repo_find_unique_abbrev(the_repository, &wt->শীর্ষ_oid, DEFAULT_ABBREV));
		যদি (wt->is_detached)
			strbuf_addstr(&sb, "(detached HEAD)");
		অন্যথা যদি (wt->শীর্ষ_ref) {
			অক্ষর *ref = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
								 wt->শীর্ষ_ref,
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

স্থির শূন্য measure_widths(কাঠামো worktree **wt, পূর্ণসংখ্যা *abbrev,
			   কাঠামো worktree_display **d, পূর্ণসংখ্যা *maxwidth)
{
	পূর্ণসংখ্যা i, display_alloc = 0;
	কাঠামো worktree_display *display = ফাঁকা;
	কাঠামো strbuf buf = STRBUF_INIT;

	জন্য (i = 0; wt[i]; i++) {
		পূর্ণসংখ্যা sha1_len;
		ALLOC_GROW(display, i + 1, display_alloc);
		quote_path(wt[i]->path, ফাঁকা, &buf, 0);
		display[i].width = utf8_strwidth(buf.buf);
		display[i].path = strbuf_detach(&buf, ফাঁকা);

		যদি (display[i].width > *maxwidth)
			*maxwidth = display[i].width;
		sha1_len = strlen(repo_find_unique_abbrev(the_repository, &wt[i]->শীর্ষ_oid, *abbrev));
		যদি (sha1_len > *abbrev)
			*abbrev = sha1_len;
	}
	*d = display;
}

স্থির পূর্ণসংখ্যা pathcmp(ধ্রুবক শূন্য *a_, ধ্রুবক শূন্য *b_)
{
	ধ্রুবক কাঠামো worktree *ধ্রুবক *a = a_;
	ধ্রুবক কাঠামো worktree *ধ্রুবক *b = b_;
	ফেরত fspathcmp((*a)->path, (*b)->path);
}

স্থির শূন্য pathsort(কাঠামো worktree **wt)
{
	পূর্ণসংখ্যা n = 0;
	কাঠামো worktree **p = wt;

	যখন (*p++)
		n++;
	QSORT(wt, n, pathcmp);
}

স্থির পূর্ণসংখ্যা list(পূর্ণসংখ্যা ac, ধ্রুবক অক্ষর **av, ধ্রুবক অক্ষর *prefix,
		কাঠামো repository *repo UNUSED)
{
	পূর্ণসংখ্যা porcelain = 0;
	পূর্ণসংখ্যা সারি_terminator = '\n';

	কাঠামো option options[] = {
		OPT_BOOL(0, "porcelain", &porcelain, N_("machine-readable output")),
		OPT__VERBOSE(&verbose, N_("show extended annotations and reasons, if available")),
		OPT_EXPIRY_DATE(0, "expire", &expire,
				N_("add 'prunable' annotation to missing worktrees older than <time>")),
		OPT_SET_INT('z', ফাঁকা, &সারি_terminator,
			    N_("terminate records with a NUL character"), '\0'),
		OPT_END()
	};

	expire = TIME_MAX;
	ac = parse_options(ac, av, prefix, options, git_worktree_list_usage, 0);
	যদি (ac)
		usage_with_options(git_worktree_list_usage, options);
	অন্যথা যদি (verbose && porcelain)
		die(_("options '%s' and '%s' cannot be used together"), "--verbose", "--porcelain");
	অন্যথা যদি (!সারি_terminator && !porcelain)
		die(_("the option '%s' requires '%s'"), "-z", "--porcelain");
	অন্যথা {
		কাঠামো worktree **worktrees = get_worktrees(the_repository);
		পূর্ণসংখ্যা path_maxwidth = 0, abbrev = DEFAULT_ABBREV, i;
		কাঠামো worktree_display *display = ফাঁকা;

		/* sort worktrees by path but keep main worktree at top */
		pathsort(worktrees + 1);

		যদি (!porcelain)
			measure_widths(worktrees, &abbrev,
				       &display, &path_maxwidth);

		জন্য (i = 0; worktrees[i]; i++) {
			যদি (porcelain)
				show_worktree_porcelain(worktrees[i],
							সারি_terminator);
			অন্যথা
				show_worktree(worktrees[i],
					      &display[i], path_maxwidth, abbrev);
		}
		জন্য (i = 0; display && worktrees[i]; i++)
			মুক্ত(display[i].path);
		মুক্ত(display);
		free_worktrees(worktrees);
	}
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা lock_worktree(পূর্ণসংখ্যা ac, ধ্রুবক অক্ষর **av, ধ্রুবক অক্ষর *prefix,
			 কাঠামো repository *repo UNUSED)
{
	ধ্রুবক অক্ষর *reason = "", *old_reason;
	কাঠামো option options[] = {
		OPT_STRING(0, "reason", &reason, N_("string"),
			   N_("reason for locking")),
		OPT_END()
	};
	কাঠামো worktree **worktrees, *wt;
	অক্ষর *path;

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
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা unlock_worktree(পূর্ণসংখ্যা ac, ধ্রুবক অক্ষর **av, ধ্রুবক অক্ষর *prefix,
			   কাঠামো repository *repo UNUSED)
{
	কাঠামো option options[] = {
		OPT_END()
	};
	কাঠামো worktree **worktrees, *wt;
	অক্ষর *path;
	পূর্ণসংখ্যা ret;

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
	ফেরত ret;
}

স্থির শূন্য validate_no_submodules(ধ্রুবক কাঠামো worktree *wt)
{
	কাঠামো সূচক_state istate = সূচক_STATE_INIT(the_repository);
	কাঠামো strbuf path = STRBUF_INIT;
	পূর্ণসংখ্যা i, found_submodules = 0;
	অক্ষর *wt_gitdir;

	wt_gitdir = get_worktree_git_dir(wt);

	যদি (is_directory(worktree_git_path(wt, "modules"))) {
		/*
		 * There could be false positives, e.g. the "modules"
		 * directory exists but is empty. But it's a rare case and
		 * this simpler check is probably good enough for now.
		 */
		found_submodules = 1;
	} অন্যথা যদি (read_সূচক_from(&istate, worktree_git_path(wt, "index"),
				   wt_gitdir) > 0) {
		জন্য (i = 0; i < istate.cache_nr; i++) {
			কাঠামো cache_entry *ce = istate.cache[i];
			পূর্ণসংখ্যা err;

			যদি (!S_ISGITLINK(ce->ce_mode))
				চালু;

			strbuf_reset(&path);
			strbuf_addf(&path, "%s/%s", wt->path, ce->name);
			যদি (!is_submodule_populated_gently(path.buf, &err))
				চালু;

			found_submodules = 1;
			বিরতি;
		}
	}
	discard_সূচক(&istate);
	strbuf_release(&path);
	মুক্ত(wt_gitdir);

	যদি (found_submodules)
		die(_("working trees containing submodules cannot be moved or removed"));
}

স্থির পূর্ণসংখ্যা move_worktree(পূর্ণসংখ্যা ac, ধ্রুবক অক্ষর **av, ধ্রুবক অক্ষর *prefix,
			 কাঠামো repository *repo UNUSED)
{
	পূর্ণসংখ্যা force = 0;
	কাঠামো option options[] = {
		OPT__FORCE(&force,
			 N_("force move even if worktree is dirty or locked"),
			 PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "relative-paths", &use_relative_paths,
			 N_("use relative paths for worktrees")),
		OPT_END()
	};
	কাঠামো worktree **worktrees, *wt;
	কাঠামো strbuf dst = STRBUF_INIT;
	কাঠামো strbuf errmsg = STRBUF_INIT;
	ধ্রুবক অক্ষর *reason = ফাঁকা;
	অক্ষর *path;

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
		ধ্রুবক অক্ষর *sep = find_last_dir_sep(wt->path);

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
	ফেরত 0;
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
স্থির শূন্য check_clean_worktree(কাঠামো worktree *wt,
				 ধ্রুবক অক্ষর *original_path)
{
	কাঠামো child_process cp;
	অক্ষর buf[1];
	পূর্ণসংখ্যা ret;

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
		     ফাঁকা);
	cp.git_cmd = 1;
	cp.dir = wt->path;
	cp.out = -1;
	ret = start_command(&cp);
	যদি (ret)
		die_errno(_("failed to run 'git status' on '%s'"),
			  original_path);
	ret = xread(cp.out, buf, আকার(buf));
	যদি (ret)
		die(_("'%s' contains modified or untracked files, use --force to delete it"),
		    original_path);
	close(cp.out);
	ret = finish_command(&cp);
	যদি (ret)
		die_errno(_("failed to run 'git status' on '%s', code %d"),
			  original_path, ret);
}

স্থির পূর্ণসংখ্যা delete_git_work_বৃক্ষ(কাঠামো worktree *wt)
{
	কাঠামো strbuf sb = STRBUF_INIT;
	পূর্ণসংখ্যা ret = 0;

	strbuf_addstr(&sb, wt->path);
	যদি (remove_dir_recursively(&sb, 0)) {
		error_errno(_("failed to delete '%s'"), sb.buf);
		ret = -1;
	}
	strbuf_release(&sb);
	ফেরত ret;
}

স্থির পূর্ণসংখ্যা remove_worktree(পূর্ণসংখ্যা ac, ধ্রুবক অক্ষর **av, ধ্রুবক অক্ষর *prefix,
			   কাঠামো repository *repo UNUSED)
{
	পূর্ণসংখ্যা force = 0;
	কাঠামো option options[] = {
		OPT__FORCE(&force,
			 N_("force removal even if worktree is dirty or locked"),
			 PARSE_OPT_NOCOMPLETE),
		OPT_END()
	};
	কাঠামো worktree **worktrees, *wt;
	কাঠামো strbuf errmsg = STRBUF_INIT;
	ধ্রুবক অক্ষর *reason = ফাঁকা;
	পূর্ণসংখ্যা ret = 0;

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
	ফেরত ret;
}

স্থির শূন্য report_repair(পূর্ণসংখ্যা iserr, ধ্রুবক অক্ষর *path, ধ্রুবক অক্ষর *বার্তা, শূন্য *cb_তথ্য)
{
	যদি (!iserr) {
		fprintf_ln(stderr, _("repair: %s: %s"), বার্তা, path);
	} অন্যথা {
		পূর্ণসংখ্যা *exit_status = (পূর্ণসংখ্যা *)cb_তথ্য;
		fprintf_ln(stderr, _("error: %s: %s"), বার্তা, path);
		*exit_status = 1;
	}
}

স্থির পূর্ণসংখ্যা repair(পূর্ণসংখ্যা ac, ধ্রুবক অক্ষর **av, ধ্রুবক অক্ষর *prefix,
		  কাঠামো repository *repo UNUSED)
{
	ধ্রুবক অক্ষর **p;
	ধ্রুবক অক্ষর *self[] = { ".", ফাঁকা };
	কাঠামো option options[] = {
		OPT_BOOL(0, "relative-paths", &use_relative_paths,
			 N_("use relative paths for worktrees")),
		OPT_END()
	};
	পূর্ণসংখ্যা rc = 0;

	ac = parse_options(ac, av, prefix, options, git_worktree_repair_usage, 0);
	p = ac > 0 ? av : self;
	জন্য (; *p; p++)
		repair_worktree_at_path(the_repository, *p, report_repair,
					&rc, use_relative_paths);
	repair_worktrees(the_repository, report_repair, &rc, use_relative_paths);
	ফেরত rc;
}

পূর্ণসংখ্যা cmd_worktree(পূর্ণসংখ্যা ac,
		 ধ্রুবক অক্ষর **av,
		 ধ্রুবক অক্ষর *prefix,
		 কাঠামো repository *repo)
{
	parse_opt_subcommand_fn *fn = ফাঁকা;
	কাঠামো option options[] = {
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

	repo_config(the_repository, git_worktree_config, ফাঁকা);

	যদি (!prefix)
		prefix = "";

	ac = parse_options(ac, av, prefix, options, git_worktree_usage, 0);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_সূচক = 0;

	ফেরত fn(ac, av, prefix, repo);
}
