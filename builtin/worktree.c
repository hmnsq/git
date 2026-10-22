#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "abspath.h"
#અન્તર્ભાવ "advice.h"
#અન્તર્ભાવ "checkout.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "copy.h"
#અન્તર્ભાવ "dir.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "hex.h"
#અન્તર્ભાવ "object-file.h"
#અન્તર્ભાવ "object-name.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "path.h"
#અન્તર્ભાવ "strvec.h"
#અન્તર્ભાવ "branch.h"
#અન્તર્ભાવ "read-cache-ll.h"
#અન્તર્ભાવ "refs.h"
#અન્તર્ભાવ "remote.h"
#અન્તર્ભાવ "run-command.h"
#અન્તર્ભાવ "hook.h"
#અન્તર્ભાવ "sigchain.h"
#અન્તર્ભાવ "submodule.h"
#અન્તર્ભાવ "utf8.h"
#અન્તર્ભાવ "worktree.h"
#અન્તર્ભાવ "quote.h"

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

સ્થિર અચલ અક્ષર * અચલ git_worktree_usage[] = {
	BUILTIN_WORKTREE_ADD_USAGE,
	BUILTIN_WORKTREE_LIST_USAGE,
	BUILTIN_WORKTREE_LOCK_USAGE,
	BUILTIN_WORKTREE_MOVE_USAGE,
	BUILTIN_WORKTREE_PRUNE_USAGE,
	BUILTIN_WORKTREE_REMOVE_USAGE,
	BUILTIN_WORKTREE_REPAIR_USAGE,
	BUILTIN_WORKTREE_UNLOCK_USAGE,
	ખાલી
};

સ્થિર અચલ અક્ષર * અચલ git_worktree_add_usage[] = {
	BUILTIN_WORKTREE_ADD_USAGE,
	ખાલી,
};

સ્થિર અચલ અક્ષર * અચલ git_worktree_list_usage[] = {
	BUILTIN_WORKTREE_LIST_USAGE,
	ખાલી
};

સ્થિર અચલ અક્ષર * અચલ git_worktree_lock_usage[] = {
	BUILTIN_WORKTREE_LOCK_USAGE,
	ખાલી
};

સ્થિર અચલ અક્ષર * અચલ git_worktree_move_usage[] = {
	BUILTIN_WORKTREE_MOVE_USAGE,
	ખાલી
};

સ્થિર અચલ અક્ષર * અચલ git_worktree_prune_usage[] = {
	BUILTIN_WORKTREE_PRUNE_USAGE,
	ખાલી
};

સ્થિર અચલ અક્ષર * અચલ git_worktree_remove_usage[] = {
	BUILTIN_WORKTREE_REMOVE_USAGE,
	ખાલી
};

સ્થિર અચલ અક્ષર * અચલ git_worktree_repair_usage[] = {
	BUILTIN_WORKTREE_REPAIR_USAGE,
	ખાલી
};

સ્થિર અચલ અક્ષર * અચલ git_worktree_unlock_usage[] = {
	BUILTIN_WORKTREE_UNLOCK_USAGE,
	ખાલી
};

સંરચના add_opts {
	પૂર્ણાંક force;
	પૂર્ણાંક detach;
	પૂર્ણાંક quiet;
	પૂર્ણાંક checkout;
	પૂર્ણાંક orphan;
	પૂર્ણાંક relative_paths;
	અચલ અક્ષર *keep_locked;
};

સ્થિર પૂર્ણાંક show_only;
સ્થિર પૂર્ણાંક verbose;
સ્થિર પૂર્ણાંક guess_remote;
સ્થિર પૂર્ણાંક use_relative_paths;
સ્થિર timestamp_t expire;

સ્થિર પૂર્ણાંક git_worktree_config(અચલ અક્ષર *var, અચલ અક્ષર *કિંમત,
			       અચલ સંરચના config_context *ctx, શૂન્ય *cb)
{
	જો (!strcmp(var, "worktree.guessremote")) {
		guess_remote = git_config_bool(var, કિંમત);
		પાછા 0;
	} નહીંતર જો (!strcmp(var, "worktree.userelativepaths")) {
		use_relative_paths = git_config_bool(var, કિંમત);
		પાછા 0;
	}

	પાછા git_default_config(var, કિંમત, ctx, cb);
}

સ્થિર પૂર્ણાંક delete_git_dir(અચલ અક્ષર *id)
{
	સંરચના strbuf sb = STRBUF_INIT;
	પૂર્ણાંક ret;

	repo_common_path_append(the_repository, &sb, "worktrees/%s", id);
	ret = remove_dir_recursively(&sb, 0);
	જો (ret < 0 && errno == ENOTDIR)
		ret = unlink(sb.buf);
	જો (ret)
		error_errno(_("failed to delete '%s'"), sb.buf);
	strbuf_release(&sb);
	પાછા ret;
}

સ્થિર શૂન્ય delete_worktrees_dir_if_empty(શૂન્ય)
{
	અક્ષર *path = repo_git_path(the_repository, "worktrees");
	rmdir(path); /* ignore failed removal */
	મુક્ત(path);
}

સ્થિર શૂન્ય prune_worktree(અચલ અક્ષર *id, અચલ અક્ષર *reason)
{
	જો (show_only || verbose)
		fprintf_ln(stderr, _("Removing %s/%s: %s"), "worktrees", id, reason);
	જો (!show_only)
		delete_git_dir(id);
}

સ્થિર પૂર્ણાંક prune_cmp(અચલ શૂન્ય *a, અચલ શૂન્ય *b)
{
	અચલ સંરચના string_list_item *x = a;
	અચલ સંરચના string_list_item *y = b;
	પૂર્ણાંક c;

	જો ((c = fspathcmp(x->string, y->string)))
	    પાછા c;
	/*
	 * paths same; prune_dupes() removes all but the first worktree entry
	 * having the same path, so sort main worktree ('util' is NULL) above
	 * linked worktrees ('util' not NULL) since main worktree can't be
	 * removed
	 */
	જો (!x->util)
		પાછા -1;
	જો (!y->util)
		પાછા 1;
	/* paths same; sort by .git/worktrees/<id> */
	પાછા strcmp(x->util, y->util);
}

સ્થિર શૂન્ય prune_dups(સંરચના string_list *l)
{
	પૂર્ણાંક i;

	QSORT(l->વસ્તુઓ, l->nr, prune_cmp);
	માટે (i = 1; i < l->nr; i++) {
		જો (!fspathcmp(l->વસ્તુઓ[i].string, l->વસ્તુઓ[i - 1].string))
			prune_worktree(l->વસ્તુઓ[i].util, "duplicate entry");
	}
}

સ્થિર શૂન્ય prune_worktrees(શૂન્ય)
{
	સંરચના strbuf reason = STRBUF_INIT;
	સંરચના strbuf main_path = STRBUF_INIT;
	સંરચના string_list kept = STRING_LIST_INIT_DUP;
	અક્ષર *path;
	DIR *dir;
	સંરચના dirent *d;

	path = repo_git_path(the_repository, "worktrees");
	dir = opendir(path);
	મુક્ત(path);
	જો (!dir)
		પાછા;
	જ્યારે ((d = readdir_skip_dot_and_dotdot(dir)) != ખાલી) {
		અક્ષર *path;
		strbuf_reset(&reason);
		જો (should_prune_worktree(the_repository, d->d_name, &reason, &path, expire))
			prune_worktree(d->d_name, reason.buf);
		નહીંતર જો (path)
			string_list_append_nodup(&kept, path)->util = xstrdup(d->d_name);
	}
	closedir(dir);

	strbuf_add_absolute_path(&main_path, repo_get_common_dir(the_repository));
	/* massage main worktree absolute path to match 'gitdir' content */
	strbuf_strip_suffix(&main_path, "/.");
	string_list_append_nodup(&kept, strbuf_detach(&main_path, ખાલી));
	prune_dups(&kept);
	string_list_clear(&kept, 1);

	જો (!show_only)
		delete_worktrees_dir_if_empty();
	strbuf_release(&reason);
}

સ્થિર પૂર્ણાંક prune(પૂર્ણાંક ac, અચલ અક્ષર **av, અચલ અક્ષર *prefix,
		 સંરચના repository *repo UNUSED)
{
	સંરચના option options[] = {
		OPT__DRY_RUN(&show_only, N_("do not remove, show only")),
		OPT__VERBOSE(&verbose, N_("report pruned working trees")),
		OPT_EXPIRY_DATE(0, "expire", &expire,
				N_("prune missing working trees older than <time>")),
		OPT_END()
	};

	expire = TIME_MAX;
	ac = parse_options(ac, av, prefix, options, git_worktree_prune_usage,
			   0);
	જો (ac)
		usage_with_options(git_worktree_prune_usage, options);
	prune_worktrees();
	પાછા 0;
}

સ્થિર અક્ષર *junk_work_વૃક્ષ;
સ્થિર અક્ષર *junk_git_dir;
સ્થિર પૂર્ણાંક is_junk;
સ્થિર pid_t junk_pid;

સ્થિર શૂન્ય remove_junk(શૂન્ય)
{
	સંરચના strbuf sb = STRBUF_INIT;
	જો (!is_junk || getpid() != junk_pid)
		પાછા;
	જો (junk_git_dir) {
		strbuf_addstr(&sb, junk_git_dir);
		remove_dir_recursively(&sb, 0);
		strbuf_reset(&sb);
	}
	જો (junk_work_વૃક્ષ) {
		strbuf_addstr(&sb, junk_work_વૃક્ષ);
		remove_dir_recursively(&sb, 0);
	}
	strbuf_release(&sb);
}

સ્થિર શૂન્ય remove_junk_on_signal(પૂર્ણાંક signo)
{
	remove_junk();
	sigchain_કાઢો(signo);
	ઉઠાવો(signo);
}

સ્થિર અક્ષર *worktree_basename_dup(અચલ અક્ષર *path)
{
	અચલ અક્ષર *name;
	પૂર્ણાંક len;

	len = strlen(path);
	જ્યારે (len && is_dir_sep(path[len - 1]))
		len--;

	name = path + len;
	જ્યારે (name > path && !is_dir_sep(name[-1]))
		name--;

	પાછા xmemdupz(name, path + len - name);
}

/* check that path is viable location for worktree */
સ્થિર શૂન્ય check_candidate_path(અચલ અક્ષર *path,
				 પૂર્ણાંક force,
				 સંરચના worktree **worktrees,
				 અચલ અક્ષર *cmd)
{
	સંરચના worktree *wt;
	પૂર્ણાંક locked;

	જો (file_exists(path) && !is_empty_dir(path))
		die(_("'%s' already exists"), path);

	wt = find_worktree_by_path(worktrees, path);
	જો (!wt)
		પાછા;

	locked = !!worktree_lock_reason(wt);
	જો ((!locked && force) || (locked && force > 1)) {
		જો (delete_git_dir(wt->id))
		    die(_("unusable worktree destination '%s'"), path);
		પાછા;
	}

	જો (locked)
		die(_("'%s' is a missing but locked worktree;\nuse '%s -f -f' to override, or 'unlock' and 'prune' or 'remove' to clear"), path, cmd);
	નહીંતર
		die(_("'%s' is a missing but already registered worktree;\nuse '%s -f' to override, or 'prune' or 'remove' to clear"), path, cmd);
}

સ્થિર શૂન્ય copy_sparse_checkout(અચલ અક્ષર *worktree_git_dir)
{
	અક્ષર *from_file = repo_git_path(the_repository, "info/sparse-checkout");
	અક્ષર *to_file = xstrfmt("%s/info/sparse-checkout", worktree_git_dir);

	જો (file_exists(from_file)) {
		જો (safe_create_leading_directories(the_repository, to_file) ||
			copy_file(the_repository, to_file, from_file, 0666))
			error(_("failed to copy '%s' to '%s'; sparse-checkout may not work correctly"),
				from_file, to_file);
	}

	મુક્ત(from_file);
	મુક્ત(to_file);
}

સ્થિર શૂન્ય copy_filtered_worktree_config(અચલ અક્ષર *worktree_git_dir)
{
	અક્ષર *from_file = repo_git_path(the_repository, "config.worktree");
	અક્ષર *to_file = xstrfmt("%s/config.worktree", worktree_git_dir);

	જો (file_exists(from_file)) {
		સંરચના config_set cs = { { 0 } };
		પૂર્ણાંક bare;

		જો (safe_create_leading_directories(the_repository, to_file) ||
			copy_file(the_repository, to_file, from_file, 0666)) {
			error(_("failed to copy worktree config from '%s' to '%s'"),
				from_file, to_file);
			જાઓ worktree_copy_cleanup;
		}

		git_configset_init(&cs);
		git_configset_add_file(&cs, from_file);

		જો (!git_configset_get_bool(&cs, "core.bare", &bare) &&
			bare &&
			repo_config_set_multivar_in_file_gently(the_repository,
				to_file, "core.bare", ખાલી, "true", ખાલી, 0))
			error(_("failed to unset '%s' in '%s'"),
				"core.bare", to_file);
		જો (!git_configset_get(&cs, "core.worktree") &&
			repo_config_set_in_file_gently(the_repository, to_file,
						       "core.worktree", ખાલી, ખાલી))
			error(_("failed to unset '%s' in '%s'"),
				"core.worktree", to_file);

		git_configset_clear(&cs);
	}

worktree_copy_cleanup:
	મુક્ત(from_file);
	મુક્ત(to_file);
}

સ્થિર પૂર્ણાંક checkout_worktree(અચલ સંરચના add_opts *opts,
			     સંરચના strvec *child_env)
{
	સંરચના child_process cp = CHILD_PROCESS_INIT;
	cp.git_cmd = 1;
	strvec_pushl(&cp.args, "reset", "--hard", "--no-recurse-submodules", ખાલી);
	જો (opts->quiet)
		strvec_ધકેલો(&cp.args, "--quiet");
	strvec_pushv(&cp.env, child_env->v);
	પાછા run_command(&cp);
}

સ્થિર પૂર્ણાંક make_worktree_orphan(અચલ અક્ષર * ref, અચલ સંરચના add_opts *opts,
				સંરચના strvec *child_env)
{
	સંરચના strbuf symref = STRBUF_INIT;
	સંરચના child_process cp = CHILD_PROCESS_INIT;

	validate_new_branchname(ref, &symref, 0);
	strvec_pushl(&cp.args, "symbolic-ref", "HEAD", symref.buf, ખાલી);
	જો (opts->quiet)
		strvec_ધકેલો(&cp.args, "--quiet");
	strvec_pushv(&cp.env, child_env->v);
	strbuf_release(&symref);
	cp.git_cmd = 1;
	પાછા run_command(&cp);
}

/*
 * References for worktrees are generally stored in '$GIT_DIR/worktrees/<wt_id>'.
 * But when using alternate reference directories, we want to store the worktree
 * references in '$ALTERNATE_REFERENCE_DIR/worktrees/<wt_id>'.
 *
 * Create the necessary folder structure to facilitate the same. But to ensure
 * that the former path is still considered a Git directory, add stubs.
 */
સ્થિર શૂન્ય setup_alternate_ref_dir(સંરચના worktree *wt, અચલ અક્ષર *wt_git_path)
{
	સંરચના strbuf sb = STRBUF_INIT;
	અક્ષર *path;

	path = wt->repo->ref_storage_payload;
	જો (!path)
		પાછા;

	જો (!is_absolute_path(path))
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

સ્થિર પૂર્ણાંક add_worktree(અચલ અક્ષર *path, અચલ અક્ષર *refname,
			અચલ સંરચના add_opts *opts)
{
	સંરચના strbuf sb_git = STRBUF_INIT, sb_repo = STRBUF_INIT;
	સંરચના strbuf sb = STRBUF_INIT;
	અચલ અક્ષર *name;
	અક્ષર *name_to_free = ખાલી;
	સંરચના strvec child_env = STRVEC_INIT;
	unsigned પૂર્ણાંક counter = 0;
	પૂર્ણાંક len, ret;
	સંરચના strbuf symref = STRBUF_INIT;
	સંરચના commit *commit = ખાલી;
	પૂર્ણાંક is_branch = 0;
	સંરચના strbuf sb_name = STRBUF_INIT;
	સંરચના worktree **worktrees, *wt = ખાલી;
	સંરચના ref_store *wt_refs;
	સંરચના repo_config_કિંમતો *cfg = repo_config_કિંમતો(the_repository);

	worktrees = get_worktrees(the_repository);
	check_candidate_path(path, opts->force, worktrees, "add");
	free_worktrees(worktrees);
	worktrees = ખાલી;

	/* is 'refname' a branch or commit? */
	જો (!opts->detach && !check_branch_ref(the_repository, &symref, refname) &&
	    refs_ref_exists(get_main_ref_store(the_repository), symref.buf)) {
		is_branch = 1;
		જો (!opts->force)
			die_if_checked_out(symref.buf, 0);
	}
	commit = lookup_commit_reference_by_name(refname);
	જો (!commit && !opts->orphan)
		die(_("invalid reference: %s"), refname);

	name = name_to_free = worktree_basename_dup(path);
	જો (!*name)
		die(_("invalid path '%s'"), path);
	sanitize_refname_component(name, &sb_name);
	જો (!sb_name.len)
		BUG("How come '%s' becomes empty after sanitization?", name);
	name = sb_name.buf;
	repo_git_path_replace(the_repository, &sb_repo, "worktrees/%s", name);
	len = sb_repo.len;
	જો (safe_create_leading_directories_const(the_repository, sb_repo.buf))
		die_errno(_("could not create leading directories of '%s'"),
			  sb_repo.buf);

	જ્યારે (mkdir(sb_repo.buf, 0777)) {
		counter++;
		જો ((errno != EEXIST) || !counter /* overflow */)
			die_errno(_("could not create directory of '%s'"),
				  sb_repo.buf);
		strbuf_setlen(&sb_repo, len);
		strbuf_addf(&sb_repo, "%d", counter);
	}
	name = strrchr(sb_repo.buf, '/') + 1;

	junk_pid = getpid();
	atexit(remove_junk);
	sigchain_ધકેલો_common(remove_junk_on_signal);

	junk_git_dir = xstrdup(sb_repo.buf);
	is_junk = 1;

	/*
	 * lock the incomplete repo so prune won't delete it, unlock
	 * after the preparation is over.
	 */
	strbuf_addf(&sb, "%s/locked", sb_repo.buf);
	જો (opts->keep_locked)
		write_file(sb.buf, "%s", opts->keep_locked);
	નહીંતર
		write_file(sb.buf, _("initializing"));

	strbuf_addf(&sb_git, "%s/.git", path);
	જો (safe_create_leading_directories_const(the_repository, sb_git.buf))
		die_errno(_("could not create leading directories of '%s'"),
			  sb_git.buf);
	junk_work_વૃક્ષ = xstrdup(path);

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
	જો (!wt) {
		ret = error(_("could not find created worktree '%s'"), name);
		જાઓ done;
	}
	setup_alternate_ref_dir(wt, sb_repo.buf);
	wt_refs = get_worktree_ref_store(wt);

	ret = ref_store_create_on_disk(wt_refs, REF_STORE_CREATE_ON_DISK_IS_WORKTREE, &sb);
	જો (ret)
		જાઓ done;

	જો (!is_branch && commit)
		ret = refs_update_ref(wt_refs, ખાલી, "HEAD", &commit->object.oid,
				      ખાલી, 0, UPDATE_REFS_સંદેશ_ON_ERR);
	નહીંતર
		ret = refs_update_symref(wt_refs, "HEAD", symref.buf, ખાલી);
	જો (ret)
		જાઓ done;

	/*
	 * If the current worktree has sparse-checkout enabled, then copy
	 * the sparse-checkout patterns from the current worktree.
	 */
	જો (cfg->apply_sparse_checkout)
		copy_sparse_checkout(sb_repo.buf);

	/*
	 * If we are using worktree config, then copy all current config
	 * values from the current worktree into the new one, that way the
	 * new worktree behaves the same as this one.
	 */
	જો (the_repository->repository_format_worktree_config)
		copy_filtered_worktree_config(sb_repo.buf);

	strvec_pushf(&child_env, "%s=%s", GIT_DIR_ENVIRONMENT, sb_git.buf);
	strvec_pushf(&child_env, "%s=%s", GIT_WORK_વૃક્ષ_ENVIRONMENT, path);

	જો (opts->orphan &&
	    (ret = make_worktree_orphan(refname, opts, &child_env)))
		જાઓ done;

	જો (opts->checkout &&
	    (ret = checkout_worktree(opts, &child_env)))
		જાઓ done;

	is_junk = 0;
	FREE_AND_NULL(junk_work_વૃક્ષ);
	FREE_AND_NULL(junk_git_dir);

done:
	જો (ret || !opts->keep_locked) {
		strbuf_reset(&sb);
		strbuf_addf(&sb, "%s/locked", sb_repo.buf);
		unlink_or_warn(sb.buf);
	}

	/*
	 * Hook failure does not warrant worktree deletion, so run hook after
	 * is_junk is cleared, but do return appropriate code when hook fails.
	 */
	જો (!ret && opts->checkout && !opts->orphan) {
		સંરચના run_hooks_opt opt = RUN_HOOKS_OPT_INIT_FORCE_SERIAL;

		strvec_pushl(&opt.env, "GIT_DIR", "GIT_WORK_TREE", ખાલી);
		strvec_pushl(&opt.args,
			     oid_to_hex(null_oid(the_hash_algo)),
			     oid_to_hex(&commit->object.oid),
			     "1",
			     ખાલી);
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
	મુક્ત(name_to_free);
	પાછા ret;
}

સ્થિર શૂન્ય print_preparing_worktree_લીટી(પૂર્ણાંક detach,
					  અચલ અક્ષર *branch,
					  અચલ અક્ષર *new_branch,
					  પૂર્ણાંક force_new_branch)
{
	જો (force_new_branch) {
		સંરચના commit *commit = lookup_commit_reference_by_name(new_branch);
		જો (!commit)
			fprintf_ln(stderr, _("Preparing worktree (new branch '%s')"), new_branch);
		નહીંતર
			fprintf_ln(stderr, _("Preparing worktree (resetting branch '%s'; was at %s)"),
				  new_branch,
				  repo_find_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV));
	} નહીંતર જો (new_branch) {
		fprintf_ln(stderr, _("Preparing worktree (new branch '%s')"), new_branch);
	} નહીંતર {
		સંરચના strbuf s = STRBUF_INIT;
		જો (!detach && !check_branch_ref(the_repository, &s, branch) &&
		    refs_ref_exists(get_main_ref_store(the_repository), s.buf))
			fprintf_ln(stderr, _("Preparing worktree (checking out '%s')"),
				  branch);
		નહીંતર {
			સંરચના commit *commit = lookup_commit_reference_by_name(branch);
			જો (!commit)
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
સ્થિર પૂર્ણાંક first_valid_ref(અચલ સંરચના reference *ref UNUSED, શૂન્ય *cb_data UNUSED)
{
	પાછા 1;
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
સ્થિર પૂર્ણાંક can_use_local_refs(અચલ સંરચના add_opts *opts)
{
	જો (refs_head_ref(get_main_ref_store(the_repository), first_valid_ref, ખાલી)) {
		પાછા 1;
	} નહીંતર જો (refs_for_each_branch_ref(get_main_ref_store(the_repository), first_valid_ref, ખાલી)) {
		જો (!opts->quiet)
			warning(_("HEAD points to an invalid (or orphaned) reference.\n"));
		પાછા 1;
	}
	પાછા 0;
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
સ્થિર પૂર્ણાંક can_use_remote_refs(અચલ સંરચના add_opts *opts)
{
	જો (!guess_remote) {
		પાછા 0;
	} નહીંતર જો (refs_for_each_remote_ref(get_main_ref_store(the_repository), first_valid_ref, ખાલી)) {
		પાછા 1;
	} નહીંતર જો (!opts->force && remote_get(ખાલી)) {
		die(_("No local or remote refs exist despite at least one remote\n"
		      "present, stopping; use 'add -f' to override or fetch a remote first"));
	}
	પાછા 0;
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
સ્થિર પૂર્ણાંક dwim_orphan(અચલ સંરચના add_opts *opts, પૂર્ણાંક opt_track, પૂર્ણાંક remote)
{
	જો (can_use_local_refs(opts)) {
		પાછા 0;
	} નહીંતર જો (remote && can_use_remote_refs(opts)) {
		પાછા 0;
	} નહીંતર જો (!opts->quiet) {
		fprintf_ln(stderr, WORKTREE_ADD_DWIM_ORPHAN_INFER_TEXT);
	}

	જો (opt_track) {
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--track");
	} નહીંતર જો (!opts->checkout) {
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--no-checkout");
	}
	પાછા 1;
}

સ્થિર શૂન્ય advise_disambiguating_remotes(અચલ અક્ષર *path, અચલ અક્ષર *branch,
					  અચલ સંરચના string_list *matched_remote_names)
{
	સંરચના string_list_item *item;

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

સ્થિર અક્ષર *dwim_branch(અચલ સંરચના add_opts *opts, અચલ અક્ષર *path, અક્ષર **new_branch)
{
	પૂર્ણાંક branch_exists;
	અક્ષર *branchname = worktree_basename_dup(path);
	સંરચના strbuf ref = STRBUF_INIT;

	branch_exists = !check_branch_ref(the_repository, &ref, branchname) &&
			refs_ref_exists(get_main_ref_store(the_repository),
					ref.buf);
	strbuf_release(&ref);
	જો (branch_exists)
		પાછા branchname;

	*new_branch = branchname;
	જો (guess_remote) {
		સંરચના object_id oid;
		અક્ષર *remote;
		પૂર્ણાંક અંક_matches = 0;
		સંરચના string_list matched_remote_names = STRING_LIST_INIT_DUP;

		remote = unique_tracking_name(*new_branch, &oid, &અંક_matches,
					      &matched_remote_names);
		જો (!remote && અંક_matches > 1) {
			જો (!opts->quiet &&
			    advice_enabled(ADVICE_CHECKOUT_AMBIGUOUS_REMOTE_BRANCH_NAME))
				advise_disambiguating_remotes(path, *new_branch,
							      &matched_remote_names);
			die(_("'%s' matched multiple (%d) remote tracking branches"),
			    *new_branch, અંક_matches);
		}
		string_list_clear(&matched_remote_names, 0);
		પાછા remote;
	}
	પાછા ખાલી;
}

સ્થિર પૂર્ણાંક add(પૂર્ણાંક ac, અચલ અક્ષર **av, અચલ અક્ષર *prefix,
	       સંરચના repository *repo UNUSED)
{
	સંરચના add_opts opts;
	અચલ અક્ષર *new_branch_force = ખાલી;
	અક્ષર *path;
	અચલ અક્ષર *branch;
	અક્ષર *branch_to_free = ખાલી;
	અક્ષર *new_branch_to_free = ખાલી;
	અચલ અક્ષર *new_branch = ખાલી;
	અક્ષર *opt_track = ખાલી;
	અચલ અક્ષર *lock_reason = ખાલી;
	પૂર્ણાંક keep_locked = 0;
	પૂર્ણાંક used_new_branch_options;
	સંરચના option options[] = {
		OPT__FORCE(&opts.force,
			   N_("checkout <branch> even if already checked out in other worktree"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_STRING('b', ખાલી, &new_branch, N_("branch"),
			   N_("create a new branch")),
		OPT_STRING('B', ખાલી, &new_branch_force, N_("branch"),
			   N_("create or reset a branch")),
		OPT_BOOL(0, "orphan", &opts.orphan, N_("create unborn branch")),
		OPT_BOOL('d', "detach", &opts.detach, N_("detach HEAD at named commit")),
		OPT_BOOL(0, "checkout", &opts.checkout, N_("populate the new working tree")),
		OPT_BOOL(0, "lock", &keep_locked, N_("keep the new working tree locked")),
		OPT_STRING(0, "reason", &lock_reason, N_("string"),
			   N_("reason for locking")),
		OPT__QUIET(&opts.quiet, N_("suppress progress reporting")),
		OPT_PASSTHRU(0, "track", &opt_track, ખાલી,
			     N_("set up tracking mode (see git-branch(1))"),
			     PARSE_OPT_NOARG | PARSE_OPT_OPTARG),
		OPT_BOOL(0, "guess-remote", &guess_remote,
			 N_("try to match the new branch name with a remote-tracking branch")),
		OPT_BOOL(0, "relative-paths", &opts.relative_paths,
			 N_("use relative paths for worktrees")),
		OPT_END()
	};
	પૂર્ણાંક ret;

	memset(&opts, 0, કદ(opts));
	opts.checkout = 1;
	opts.relative_paths = use_relative_paths;
	ac = parse_options(ac, av, prefix, options, git_worktree_add_usage, 0);
	જો (!!opts.detach + !!new_branch + !!new_branch_force > 1)
		die(_("options '%s', '%s', and '%s' cannot be used together"), "-b", "-B", "--detach");
	જો (opts.detach && opts.orphan)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--detach");
	જો (opts.orphan && opt_track)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--track");
	જો (opts.orphan && !opts.checkout)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--no-checkout");
	જો (opts.orphan && ac == 2)
		die(_("option '%s' and commit-ish cannot be used together"),
		    "--orphan");
	જો (lock_reason && !keep_locked)
		die(_("the option '%s' requires '%s'"), "--reason", "--lock");
	જો (lock_reason)
		opts.keep_locked = lock_reason;
	નહીંતર જો (keep_locked)
		opts.keep_locked = _("added with --lock");

	જો (ac < 1 || ac > 2)
		usage_with_options(git_worktree_add_usage, options);

	path = prefix_filename(prefix, av[0]);
	branch = ac < 2 ? "HEAD" : av[1];
	used_new_branch_options = new_branch || new_branch_force;

	જો (!strcmp(branch, "-"))
		branch = "@{-1}";

	જો (new_branch_force) {
		સંરચના strbuf symref = STRBUF_INIT;

		new_branch = new_branch_force;

		જો (!opts.force &&
		    !check_branch_ref(the_repository, &symref, new_branch) &&
		    refs_ref_exists(get_main_ref_store(the_repository), symref.buf))
			die_if_checked_out(symref.buf, 0);
		strbuf_release(&symref);
	}

	જો (opts.orphan && !new_branch) {
		new_branch = new_branch_to_free = worktree_basename_dup(path);
	} નહીંતર જો (opts.orphan) {
		; /* no-op */
	} નહીંતર જો (opts.detach) {
		/* Check HEAD */
		જો (!strcmp(branch, "HEAD"))
			can_use_local_refs(&opts);
	} નહીંતર જો (ac < 2 && new_branch) {
		/* DWIM: Infer --orphan when repo has no refs. */
		opts.orphan = dwim_orphan(&opts, !!opt_track, 0);
	} નહીંતર જો (ac < 2) {
		/* DWIM: Guess branch name from path. */
		અક્ષર *s = dwim_branch(&opts, path, &new_branch_to_free);
		જો (s)
			branch = branch_to_free = s;
		new_branch = new_branch_to_free;

		/* DWIM: Infer --orphan when repo has no refs. */
		opts.orphan = (!s) && dwim_orphan(&opts, !!opt_track, 1);
	} નહીંતર જો (ac == 2 && new_branch) {
		જો (!strcmp(branch, "HEAD"))
			can_use_local_refs(&opts);
	} નહીંતર જો (ac == 2) {
		સંરચના commit *commit;

		commit = lookup_commit_reference_by_name(branch);
		જો (!commit) {
			સંરચના object_id oid;
			અક્ષર *remote;
			પૂર્ણાંક અંક_matches = 0;
			સંરચના string_list matched_remote_names = STRING_LIST_INIT_DUP;

			remote = unique_tracking_name(branch, &oid, &અંક_matches,
						      &matched_remote_names);
			જો (remote) {
				new_branch = branch;
				branch = new_branch_to_free = remote;
			} નહીંતર જો (અંક_matches > 1) {
				જો (!opts.quiet &&
				    advice_enabled(ADVICE_CHECKOUT_AMBIGUOUS_REMOTE_BRANCH_NAME))
					advise_disambiguating_remotes(path, branch,
								      &matched_remote_names);
				die(_("'%s' matched multiple (%d) remote tracking branches"),
				    branch, અંક_matches);
			}
			string_list_clear(&matched_remote_names, 0);
		}

		જો (!strcmp(branch, "HEAD"))
			can_use_local_refs(&opts);

	}

	જો (!opts.orphan && !lookup_commit_reference_by_name(branch)) {
		પૂર્ણાંક attempt_hint = !opts.quiet && (ac < 2);
		જો (attempt_hint && used_new_branch_options) {
			advise_if_enabled(ADVICE_WORKTREE_ADD_ORPHAN,
				WORKTREE_ADD_ORPHAN_WITH_DASH_B_HINT_TEXT,
				new_branch, path);
		} નહીંતર જો (attempt_hint) {
			advise_if_enabled(ADVICE_WORKTREE_ADD_ORPHAN,
				WORKTREE_ADD_ORPHAN_NO_DASH_B_HINT_TEXT, path);
		}
		die(_("invalid reference: %s"), branch);
	}

	જો (!opts.quiet)
		print_preparing_worktree_લીટી(opts.detach, branch, new_branch, !!new_branch_force);

	જો (opts.orphan) {
		branch = new_branch;
	} નહીંતર જો (new_branch) {
		સંરચના child_process cp = CHILD_PROCESS_INIT;
		cp.git_cmd = 1;
		strvec_ધકેલો(&cp.args, "branch");
		જો (new_branch_force)
			strvec_ધકેલો(&cp.args, "--force");
		જો (opts.quiet)
			strvec_ધકેલો(&cp.args, "--quiet");
		strvec_ધકેલો(&cp.args, new_branch);
		strvec_ધકેલો(&cp.args, branch);
		જો (opt_track)
			strvec_ધકેલો(&cp.args, opt_track);
		જો (run_command(&cp)) {
			ret = -1;
			જાઓ cleanup;
		}
		branch = new_branch;
	} નહીંતર જો (opt_track) {
		die(_("--[no-]track can only be used if a new branch is created"));
	}

	ret = add_worktree(path, branch, &opts);
cleanup:
	મુક્ત(path);
	મુક્ત(opt_track);
	મુક્ત(branch_to_free);
	મુક્ત(new_branch_to_free);
	પાછા ret;
}

સ્થિર શૂન્ય show_worktree_porcelain(સંરચના worktree *wt, પૂર્ણાંક લીટી_terminator)
{
	અચલ અક્ષર *reason;

	printf("worktree %s%c", wt->path, લીટી_terminator);
	જો (wt->is_bare)
		printf("bare%c", લીટી_terminator);
	નહીંતર {
		printf("HEAD %s%c", oid_to_hex(&wt->head_oid), લીટી_terminator);
		જો (wt->is_detached)
			printf("detached%c", લીટી_terminator);
		નહીંતર જો (wt->head_ref)
			printf("branch %s%c", wt->head_ref, લીટી_terminator);
	}

	reason = worktree_lock_reason(wt);
	જો (reason) {
		fputs("locked", stdout);
		જો (*reason) {
			fputc(' ', stdout);
			write_name_quoted(reason, stdout, લીટી_terminator);
		} નહીંતર {
			fputc(લીટી_terminator, stdout);
		}
	}

	reason = worktree_prune_reason(wt, expire);
	જો (reason)
		printf("prunable %s%c", reason, લીટી_terminator);

	fputc(લીટી_terminator, stdout);
}

સંરચના worktree_display {
	અક્ષર *path;
	પૂર્ણાંક width;
};

સ્થિર શૂન્ય show_worktree(સંરચના worktree *wt, સંરચના worktree_display *display,
			  પૂર્ણાંક path_maxwidth, પૂર્ણાંક abbrev_len)
{
	સંરચના strbuf sb = STRBUF_INIT;
	અચલ અક્ષર *reason;

	strbuf_addf(&sb, "%s%*s", display->path, 1 + path_maxwidth - display->width, "");
	જો (wt->is_bare)
		strbuf_addstr(&sb, "(bare)");
	નહીંતર {
		strbuf_addf(&sb, "%-*s ", abbrev_len,
				repo_find_unique_abbrev(the_repository, &wt->head_oid, DEFAULT_ABBREV));
		જો (wt->is_detached)
			strbuf_addstr(&sb, "(detached HEAD)");
		નહીંતર જો (wt->head_ref) {
			અક્ષર *ref = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
								 wt->head_ref,
								 0);
			strbuf_addf(&sb, "[%s]", ref);
			મુક્ત(ref);
		} નહીંતર
			strbuf_addstr(&sb, "(error)");
	}

	reason = worktree_lock_reason(wt);
	જો (verbose && reason && *reason)
		strbuf_addf(&sb, "\n\tlocked: %s", reason);
	નહીંતર જો (reason)
		strbuf_addstr(&sb, " locked");

	reason = worktree_prune_reason(wt, expire);
	જો (verbose && reason)
		strbuf_addf(&sb, "\n\tprunable: %s", reason);
	નહીંતર જો (reason)
		strbuf_addstr(&sb, " prunable");

	printf("%s\n", sb.buf);
	strbuf_release(&sb);
}

સ્થિર શૂન્ય measure_widths(સંરચના worktree **wt, પૂર્ણાંક *abbrev,
			   સંરચના worktree_display **d, પૂર્ણાંક *maxwidth)
{
	પૂર્ણાંક i, display_alloc = 0;
	સંરચના worktree_display *display = ખાલી;
	સંરચના strbuf buf = STRBUF_INIT;

	માટે (i = 0; wt[i]; i++) {
		પૂર્ણાંક sha1_len;
		ALLOC_GROW(display, i + 1, display_alloc);
		quote_path(wt[i]->path, ખાલી, &buf, 0);
		display[i].width = utf8_strwidth(buf.buf);
		display[i].path = strbuf_detach(&buf, ખાલી);

		જો (display[i].width > *maxwidth)
			*maxwidth = display[i].width;
		sha1_len = strlen(repo_find_unique_abbrev(the_repository, &wt[i]->head_oid, *abbrev));
		જો (sha1_len > *abbrev)
			*abbrev = sha1_len;
	}
	*d = display;
}

સ્થિર પૂર્ણાંક pathcmp(અચલ શૂન્ય *a_, અચલ શૂન્ય *b_)
{
	અચલ સંરચના worktree *અચલ *a = a_;
	અચલ સંરચના worktree *અચલ *b = b_;
	પાછા fspathcmp((*a)->path, (*b)->path);
}

સ્થિર શૂન્ય pathsort(સંરચના worktree **wt)
{
	પૂર્ણાંક n = 0;
	સંરચના worktree **p = wt;

	જ્યારે (*p++)
		n++;
	QSORT(wt, n, pathcmp);
}

સ્થિર પૂર્ણાંક list(પૂર્ણાંક ac, અચલ અક્ષર **av, અચલ અક્ષર *prefix,
		સંરચના repository *repo UNUSED)
{
	પૂર્ણાંક porcelain = 0;
	પૂર્ણાંક લીટી_terminator = '\n';

	સંરચના option options[] = {
		OPT_BOOL(0, "porcelain", &porcelain, N_("machine-readable output")),
		OPT__VERBOSE(&verbose, N_("show extended annotations and reasons, if available")),
		OPT_EXPIRY_DATE(0, "expire", &expire,
				N_("add 'prunable' annotation to missing worktrees older than <time>")),
		OPT_SET_INT('z', ખાલી, &લીટી_terminator,
			    N_("terminate records with a NUL character"), '\0'),
		OPT_END()
	};

	expire = TIME_MAX;
	ac = parse_options(ac, av, prefix, options, git_worktree_list_usage, 0);
	જો (ac)
		usage_with_options(git_worktree_list_usage, options);
	નહીંતર જો (verbose && porcelain)
		die(_("options '%s' and '%s' cannot be used together"), "--verbose", "--porcelain");
	નહીંતર જો (!લીટી_terminator && !porcelain)
		die(_("the option '%s' requires '%s'"), "-z", "--porcelain");
	નહીંતર {
		સંરચના worktree **worktrees = get_worktrees(the_repository);
		પૂર્ણાંક path_maxwidth = 0, abbrev = DEFAULT_ABBREV, i;
		સંરચના worktree_display *display = ખાલી;

		/* sort worktrees by path but keep main worktree at top */
		pathsort(worktrees + 1);

		જો (!porcelain)
			measure_widths(worktrees, &abbrev,
				       &display, &path_maxwidth);

		માટે (i = 0; worktrees[i]; i++) {
			જો (porcelain)
				show_worktree_porcelain(worktrees[i],
							લીટી_terminator);
			નહીંતર
				show_worktree(worktrees[i],
					      &display[i], path_maxwidth, abbrev);
		}
		માટે (i = 0; display && worktrees[i]; i++)
			મુક્ત(display[i].path);
		મુક્ત(display);
		free_worktrees(worktrees);
	}
	પાછા 0;
}

સ્થિર પૂર્ણાંક lock_worktree(પૂર્ણાંક ac, અચલ અક્ષર **av, અચલ અક્ષર *prefix,
			 સંરચના repository *repo UNUSED)
{
	અચલ અક્ષર *reason = "", *old_reason;
	સંરચના option options[] = {
		OPT_STRING(0, "reason", &reason, N_("string"),
			   N_("reason for locking")),
		OPT_END()
	};
	સંરચના worktree **worktrees, *wt;
	અક્ષર *path;

	ac = parse_options(ac, av, prefix, options, git_worktree_lock_usage, 0);
	જો (ac != 1)
		usage_with_options(git_worktree_lock_usage, options);

	worktrees = get_worktrees(the_repository);
	wt = find_worktree(worktrees, prefix, av[0]);
	જો (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	જો (is_main_worktree(wt))
		die(_("The main working tree cannot be locked or unlocked"));

	old_reason = worktree_lock_reason(wt);
	જો (old_reason) {
		જો (*old_reason)
			die(_("'%s' is already locked, reason: %s"),
			    av[0], old_reason);
		die(_("'%s' is already locked"), av[0]);
	}

	path = repo_common_path(the_repository, "worktrees/%s/locked", wt->id);
	write_file(path, "%s", reason);

	free_worktrees(worktrees);
	મુક્ત(path);
	પાછા 0;
}

સ્થિર પૂર્ણાંક unlock_worktree(પૂર્ણાંક ac, અચલ અક્ષર **av, અચલ અક્ષર *prefix,
			   સંરચના repository *repo UNUSED)
{
	સંરચના option options[] = {
		OPT_END()
	};
	સંરચના worktree **worktrees, *wt;
	અક્ષર *path;
	પૂર્ણાંક ret;

	ac = parse_options(ac, av, prefix, options, git_worktree_unlock_usage, 0);
	જો (ac != 1)
		usage_with_options(git_worktree_unlock_usage, options);

	worktrees = get_worktrees(the_repository);
	wt = find_worktree(worktrees, prefix, av[0]);
	જો (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	જો (is_main_worktree(wt))
		die(_("The main working tree cannot be locked or unlocked"));
	જો (!worktree_lock_reason(wt))
		die(_("'%s' is not locked"), av[0]);

	path = repo_common_path(the_repository, "worktrees/%s/locked", wt->id);
	ret = unlink_or_warn(path);

	free_worktrees(worktrees);
	મુક્ત(path);
	પાછા ret;
}

સ્થિર શૂન્ય validate_no_submodules(અચલ સંરચના worktree *wt)
{
	સંરચના index_state istate = INDEX_STATE_INIT(the_repository);
	સંરચના strbuf path = STRBUF_INIT;
	પૂર્ણાંક i, found_submodules = 0;
	અક્ષર *wt_gitdir;

	wt_gitdir = get_worktree_git_dir(wt);

	જો (is_directory(worktree_git_path(wt, "modules"))) {
		/*
		 * There could be false positives, e.g. the "modules"
		 * directory exists but is empty. But it's a rare case and
		 * this simpler check is probably good enough for now.
		 */
		found_submodules = 1;
	} નહીંતર જો (read_index_from(&istate, worktree_git_path(wt, "index"),
				   wt_gitdir) > 0) {
		માટે (i = 0; i < istate.cache_nr; i++) {
			સંરચના cache_entry *ce = istate.cache[i];
			પૂર્ણાંક err;

			જો (!S_ISGITLINK(ce->ce_mode))
				ચાલુ;

			strbuf_reset(&path);
			strbuf_addf(&path, "%s/%s", wt->path, ce->name);
			જો (!is_submodule_populated_gently(path.buf, &err))
				ચાલુ;

			found_submodules = 1;
			તોડો;
		}
	}
	discard_index(&istate);
	strbuf_release(&path);
	મુક્ત(wt_gitdir);

	જો (found_submodules)
		die(_("working trees containing submodules cannot be moved or removed"));
}

સ્થિર પૂર્ણાંક move_worktree(પૂર્ણાંક ac, અચલ અક્ષર **av, અચલ અક્ષર *prefix,
			 સંરચના repository *repo UNUSED)
{
	પૂર્ણાંક force = 0;
	સંરચના option options[] = {
		OPT__FORCE(&force,
			 N_("force move even if worktree is dirty or locked"),
			 PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "relative-paths", &use_relative_paths,
			 N_("use relative paths for worktrees")),
		OPT_END()
	};
	સંરચના worktree **worktrees, *wt;
	સંરચના strbuf dst = STRBUF_INIT;
	સંરચના strbuf errmsg = STRBUF_INIT;
	અચલ અક્ષર *reason = ખાલી;
	અક્ષર *path;

	ac = parse_options(ac, av, prefix, options, git_worktree_move_usage,
			   0);
	જો (ac != 2)
		usage_with_options(git_worktree_move_usage, options);

	path = prefix_filename(prefix, av[1]);
	strbuf_addstr(&dst, path);
	મુક્ત(path);

	worktrees = get_worktrees(the_repository);
	wt = find_worktree(worktrees, prefix, av[0]);
	જો (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	જો (is_main_worktree(wt))
		die(_("'%s' is a main working tree"), av[0]);
	જો (is_directory(dst.buf)) {
		અચલ અક્ષર *sep = find_last_dir_sep(wt->path);

		જો (!sep)
			die(_("could not figure out destination name from '%s'"),
			    wt->path);
		strbuf_trim_trailing_dir_sep(&dst);
		strbuf_addstr(&dst, sep);
	}
	check_candidate_path(dst.buf, force, worktrees, "move");

	validate_no_submodules(wt);

	જો (force < 2)
		reason = worktree_lock_reason(wt);
	જો (reason) {
		જો (*reason)
			die(_("cannot move a locked working tree, lock reason: %s\nuse 'move -f -f' to override or unlock first"),
			    reason);
		die(_("cannot move a locked working tree;\nuse 'move -f -f' to override or unlock first"));
	}
	જો (validate_worktree(wt, &errmsg, 0))
		die(_("validation failed, cannot move working tree: %s"),
		    errmsg.buf);
	strbuf_release(&errmsg);

	જો (rename(wt->path, dst.buf) == -1)
		die_errno(_("failed to move '%s' to '%s'"), wt->path, dst.buf);

	update_worktree_location(wt, dst.buf, use_relative_paths);

	strbuf_release(&dst);
	free_worktrees(worktrees);
	પાછા 0;
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
સ્થિર શૂન્ય check_clean_worktree(સંરચના worktree *wt,
				 અચલ અક્ષર *original_path)
{
	સંરચના child_process cp;
	અક્ષર buf[1];
	પૂર્ણાંક ret;

	/*
	 * Until we sort this out, all submodules are "dirty" and
	 * will abort this function.
	 */
	validate_no_submodules(wt);

	child_process_init(&cp);
	strvec_pushf(&cp.env, "%s=%s/.git",
		     GIT_DIR_ENVIRONMENT, wt->path);
	strvec_pushf(&cp.env, "%s=%s",
		     GIT_WORK_વૃક્ષ_ENVIRONMENT, wt->path);
	strvec_pushl(&cp.args, "status",
		     "--porcelain", "--ignore-submodules=none",
		     ખાલી);
	cp.git_cmd = 1;
	cp.dir = wt->path;
	cp.out = -1;
	ret = start_command(&cp);
	જો (ret)
		die_errno(_("failed to run 'git status' on '%s'"),
			  original_path);
	ret = xread(cp.out, buf, કદ(buf));
	જો (ret)
		die(_("'%s' contains modified or untracked files, use --force to delete it"),
		    original_path);
	close(cp.out);
	ret = finish_command(&cp);
	જો (ret)
		die_errno(_("failed to run 'git status' on '%s', code %d"),
			  original_path, ret);
}

સ્થિર પૂર્ણાંક delete_git_work_વૃક્ષ(સંરચના worktree *wt)
{
	સંરચના strbuf sb = STRBUF_INIT;
	પૂર્ણાંક ret = 0;

	strbuf_addstr(&sb, wt->path);
	જો (remove_dir_recursively(&sb, 0)) {
		error_errno(_("failed to delete '%s'"), sb.buf);
		ret = -1;
	}
	strbuf_release(&sb);
	પાછા ret;
}

સ્થિર પૂર્ણાંક remove_worktree(પૂર્ણાંક ac, અચલ અક્ષર **av, અચલ અક્ષર *prefix,
			   સંરચના repository *repo UNUSED)
{
	પૂર્ણાંક force = 0;
	સંરચના option options[] = {
		OPT__FORCE(&force,
			 N_("force removal even if worktree is dirty or locked"),
			 PARSE_OPT_NOCOMPLETE),
		OPT_END()
	};
	સંરચના worktree **worktrees, *wt;
	સંરચના strbuf errmsg = STRBUF_INIT;
	અચલ અક્ષર *reason = ખાલી;
	પૂર્ણાંક ret = 0;

	ac = parse_options(ac, av, prefix, options, git_worktree_remove_usage, 0);
	જો (ac != 1)
		usage_with_options(git_worktree_remove_usage, options);

	worktrees = get_worktrees(the_repository);
	wt = find_worktree(worktrees, prefix, av[0]);
	જો (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	જો (is_main_worktree(wt))
		die(_("'%s' is a main working tree"), av[0]);
	જો (force < 2)
		reason = worktree_lock_reason(wt);
	જો (reason) {
		જો (*reason)
			die(_("cannot remove a locked working tree, lock reason: %s\nuse 'remove -f -f' to override or unlock first"),
			    reason);
		die(_("cannot remove a locked working tree;\nuse 'remove -f -f' to override or unlock first"));
	}
	જો (validate_worktree(wt, &errmsg, WT_VALIDATE_WORKTREE_MISSING_OK))
		die(_("validation failed, cannot remove working tree: %s"),
		    errmsg.buf);
	strbuf_release(&errmsg);

	જો (file_exists(wt->path)) {
		જો (!force)
			check_clean_worktree(wt, av[0]);

		ret |= delete_git_work_વૃક્ષ(wt);
	}
	/*
	 * continue on even if ret is non-zero, there's no going back
	 * from here.
	 */
	ret |= delete_git_dir(wt->id);
	delete_worktrees_dir_if_empty();

	free_worktrees(worktrees);
	પાછા ret;
}

સ્થિર શૂન્ય report_repair(પૂર્ણાંક iserr, અચલ અક્ષર *path, અચલ અક્ષર *સંદેશ, શૂન્ય *cb_data)
{
	જો (!iserr) {
		fprintf_ln(stderr, _("repair: %s: %s"), સંદેશ, path);
	} નહીંતર {
		પૂર્ણાંક *exit_status = (પૂર્ણાંક *)cb_data;
		fprintf_ln(stderr, _("error: %s: %s"), સંદેશ, path);
		*exit_status = 1;
	}
}

સ્થિર પૂર્ણાંક repair(પૂર્ણાંક ac, અચલ અક્ષર **av, અચલ અક્ષર *prefix,
		  સંરચના repository *repo UNUSED)
{
	અચલ અક્ષર **p;
	અચલ અક્ષર *self[] = { ".", ખાલી };
	સંરચના option options[] = {
		OPT_BOOL(0, "relative-paths", &use_relative_paths,
			 N_("use relative paths for worktrees")),
		OPT_END()
	};
	પૂર્ણાંક rc = 0;

	ac = parse_options(ac, av, prefix, options, git_worktree_repair_usage, 0);
	p = ac > 0 ? av : self;
	માટે (; *p; p++)
		repair_worktree_at_path(the_repository, *p, report_repair,
					&rc, use_relative_paths);
	repair_worktrees(the_repository, report_repair, &rc, use_relative_paths);
	પાછા rc;
}

પૂર્ણાંક cmd_worktree(પૂર્ણાંક ac,
		 અચલ અક્ષર **av,
		 અચલ અક્ષર *prefix,
		 સંરચના repository *repo)
{
	parse_opt_subcommand_fn *fn = ખાલી;
	સંરચના option options[] = {
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

	repo_config(the_repository, git_worktree_config, ખાલી);

	જો (!prefix)
		prefix = "";

	ac = parse_options(ac, av, prefix, options, git_worktree_usage, 0);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	પાછા fn(ac, av, prefix, repo);
}
