#घोषणा USE_THE_REPOSITORY_VARIABLE
#घोषणा DISABLE_SIGN_COMPARE_WARNINGS

#समावेश "builtin.h"
#समावेश "abspath.h"
#समावेश "advice.h"
#समावेश "checkout.h"
#समावेश "config.h"
#समावेश "copy.h"
#समावेश "dir.h"
#समावेश "environment.h"
#समावेश "gettext.h"
#समावेश "hex.h"
#समावेश "object-file.h"
#समावेश "object-name.h"
#समावेश "parse-options.h"
#समावेश "path.h"
#समावेश "strvec.h"
#समावेश "branch.h"
#समावेश "read-cache-ll.h"
#समावेश "refs.h"
#समावेश "remote.h"
#समावेश "run-command.h"
#समावेश "hook.h"
#समावेश "sigchain.h"
#समावेश "submodule.h"
#समावेश "utf8.h"
#समावेश "worktree.h"
#समावेश "quote.h"

#घोषणा BUILTIN_WORKTREE_ADD_USAGE \
	N_("git worktree add [-f] [--detach] [--checkout] [--lock [--reason <string>]]\n" \
	   "                 [--orphan] [(-b | -B) <new-branch>] <path> [<commit-ish>]")

#घोषणा BUILTIN_WORKTREE_LIST_USAGE \
	N_("git worktree list [-v | --porcelain [-z]]")
#घोषणा BUILTIN_WORKTREE_LOCK_USAGE \
	N_("git worktree lock [--reason <string>] <worktree>")
#घोषणा BUILTIN_WORKTREE_MOVE_USAGE \
	N_("git worktree move <worktree> <new-path>")
#घोषणा BUILTIN_WORKTREE_PRUNE_USAGE \
	N_("git worktree prune [-n] [-v] [--expire <expire>]")
#घोषणा BUILTIN_WORKTREE_REMOVE_USAGE \
	N_("git worktree remove [-f] <worktree>")
#घोषणा BUILTIN_WORKTREE_REPAIR_USAGE \
	N_("git worktree repair [<path>...]")
#घोषणा BUILTIN_WORKTREE_UNLOCK_USAGE \
	N_("git worktree unlock <worktree>")

#घोषणा WORKTREE_ADD_DWIM_ORPHAN_INFER_TEXT \
	_("No possible source branch, inferring '--orphan'")

#घोषणा WORKTREE_ADD_ORPHAN_WITH_DASH_B_HINT_TEXT \
	_("If you meant to create a worktree containing a new unborn branch\n" \
	"(branch with no commits) for this repository, you can do so\n" \
	"using the --orphan flag:\n" \
	"\n" \
	"    git worktree add --orphan -b %s %s\n")

#घोषणा WORKTREE_ADD_ORPHAN_NO_DASH_B_HINT_TEXT \
	_("If you meant to create a worktree containing a new unborn branch\n" \
	"(branch with no commits) for this repository, you can do so\n" \
	"using the --orphan flag:\n" \
	"\n" \
	"    git worktree add --orphan %s\n")

अटल स्थिर अक्षर * स्थिर git_worktree_usage[] = {
	BUILTIN_WORKTREE_ADD_USAGE,
	BUILTIN_WORKTREE_LIST_USAGE,
	BUILTIN_WORKTREE_LOCK_USAGE,
	BUILTIN_WORKTREE_MOVE_USAGE,
	BUILTIN_WORKTREE_PRUNE_USAGE,
	BUILTIN_WORKTREE_REMOVE_USAGE,
	BUILTIN_WORKTREE_REPAIR_USAGE,
	BUILTIN_WORKTREE_UNLOCK_USAGE,
	शून्य
};

अटल स्थिर अक्षर * स्थिर git_worktree_add_usage[] = {
	BUILTIN_WORKTREE_ADD_USAGE,
	शून्य,
};

अटल स्थिर अक्षर * स्थिर git_worktree_list_usage[] = {
	BUILTIN_WORKTREE_LIST_USAGE,
	शून्य
};

अटल स्थिर अक्षर * स्थिर git_worktree_lock_usage[] = {
	BUILTIN_WORKTREE_LOCK_USAGE,
	शून्य
};

अटल स्थिर अक्षर * स्थिर git_worktree_move_usage[] = {
	BUILTIN_WORKTREE_MOVE_USAGE,
	शून्य
};

अटल स्थिर अक्षर * स्थिर git_worktree_prune_usage[] = {
	BUILTIN_WORKTREE_PRUNE_USAGE,
	शून्य
};

अटल स्थिर अक्षर * स्थिर git_worktree_remove_usage[] = {
	BUILTIN_WORKTREE_REMOVE_USAGE,
	शून्य
};

अटल स्थिर अक्षर * स्थिर git_worktree_repair_usage[] = {
	BUILTIN_WORKTREE_REPAIR_USAGE,
	शून्य
};

अटल स्थिर अक्षर * स्थिर git_worktree_unlock_usage[] = {
	BUILTIN_WORKTREE_UNLOCK_USAGE,
	शून्य
};

काष्ठा add_opts {
	पूर्णांक force;
	पूर्णांक detach;
	पूर्णांक quiet;
	पूर्णांक checkout;
	पूर्णांक orphan;
	पूर्णांक relative_paths;
	स्थिर अक्षर *keep_locked;
};

अटल पूर्णांक show_only;
अटल पूर्णांक verbose;
अटल पूर्णांक guess_remote;
अटल पूर्णांक use_relative_paths;
अटल timestamp_t expire;

अटल पूर्णांक git_worktree_config(स्थिर अक्षर *var, स्थिर अक्षर *मूल्य,
			       स्थिर काष्ठा config_context *ctx, व्योम *cb)
{
	अगर (!म_भेद(var, "worktree.guessremote")) {
		guess_remote = git_config_bool(var, मूल्य);
		वापस 0;
	} अन्यथा अगर (!म_भेद(var, "worktree.userelativepaths")) {
		use_relative_paths = git_config_bool(var, मूल्य);
		वापस 0;
	}

	वापस git_default_config(var, मूल्य, ctx, cb);
}

अटल पूर्णांक delete_git_dir(स्थिर अक्षर *id)
{
	काष्ठा strbuf sb = STRBUF_INIT;
	पूर्णांक ret;

	repo_common_path_append(the_repository, &sb, "worktrees/%s", id);
	ret = remove_dir_recursively(&sb, 0);
	अगर (ret < 0 && त्रुटि_सं == ENOTDIR)
		ret = unlink(sb.buf);
	अगर (ret)
		error_errno(_("failed to delete '%s'"), sb.buf);
	strbuf_release(&sb);
	वापस ret;
}

अटल व्योम delete_worktrees_dir_if_empty(व्योम)
{
	अक्षर *path = repo_git_path(the_repository, "worktrees");
	सूची_हटाओ(path); /* ignore failed removal */
	मुक्त(path);
}

अटल व्योम prune_worktree(स्थिर अक्षर *id, स्थिर अक्षर *reason)
{
	अगर (show_only || verbose)
		fprintf_ln(मानक_त्रुटि, _("Removing %s/%s: %s"), "worktrees", id, reason);
	अगर (!show_only)
		delete_git_dir(id);
}

अटल पूर्णांक prune_cmp(स्थिर व्योम *a, स्थिर व्योम *b)
{
	स्थिर काष्ठा string_list_item *x = a;
	स्थिर काष्ठा string_list_item *y = b;
	पूर्णांक c;

	अगर ((c = fspathcmp(x->string, y->string)))
	    वापस c;
	/*
	 * paths same; prune_dupes() removes all but the first worktree entry
	 * having the same path, so sort main worktree ('util' is NULL) above
	 * linked worktrees ('util' not NULL) since main worktree can't be
	 * removed
	 */
	अगर (!x->util)
		वापस -1;
	अगर (!y->util)
		वापस 1;
	/* paths same; sort by .git/worktrees/<id> */
	वापस म_भेद(x->util, y->util);
}

अटल व्योम prune_dups(काष्ठा string_list *l)
{
	पूर्णांक i;

	QSORT(l->वस्तुएँ, l->nr, prune_cmp);
	क्रम (i = 1; i < l->nr; i++) {
		अगर (!fspathcmp(l->वस्तुएँ[i].string, l->वस्तुएँ[i - 1].string))
			prune_worktree(l->वस्तुएँ[i].util, "duplicate entry");
	}
}

अटल व्योम prune_worktrees(व्योम)
{
	काष्ठा strbuf reason = STRBUF_INIT;
	काष्ठा strbuf main_path = STRBUF_INIT;
	काष्ठा string_list kept = STRING_LIST_INIT_DUP;
	अक्षर *path;
	सूची *dir;
	काष्ठा dirent *d;

	path = repo_git_path(the_repository, "worktrees");
	dir = सूची_खोलो(path);
	मुक्त(path);
	अगर (!dir)
		वापस;
	जबतक ((d = readdir_skip_dot_and_dotdot(dir)) != शून्य) {
		अक्षर *path;
		strbuf_reset(&reason);
		अगर (should_prune_worktree(the_repository, d->d_name, &reason, &path, expire))
			prune_worktree(d->d_name, reason.buf);
		अन्यथा अगर (path)
			string_list_append_nodup(&kept, path)->util = xstrdup(d->d_name);
	}
	बंद_सूची(dir);

	strbuf_add_absolute_path(&main_path, repo_get_common_dir(the_repository));
	/* massage main worktree absolute path to match 'gitdir' content */
	strbuf_strip_suffix(&main_path, "/.");
	string_list_append_nodup(&kept, strbuf_detach(&main_path, शून्य));
	prune_dups(&kept);
	string_list_clear(&kept, 1);

	अगर (!show_only)
		delete_worktrees_dir_if_empty();
	strbuf_release(&reason);
}

अटल पूर्णांक prune(पूर्णांक ac, स्थिर अक्षर **av, स्थिर अक्षर *prefix,
		 काष्ठा repository *repo UNUSED)
{
	काष्ठा option options[] = {
		OPT__DRY_RUN(&show_only, N_("do not remove, show only")),
		OPT__VERBOSE(&verbose, N_("report pruned working trees")),
		OPT_EXPIRY_DATE(0, "expire", &expire,
				N_("prune missing working trees older than <time>")),
		OPT_END()
	};

	expire = TIME_MAX;
	ac = parse_options(ac, av, prefix, options, git_worktree_prune_usage,
			   0);
	अगर (ac)
		usage_with_options(git_worktree_prune_usage, options);
	prune_worktrees();
	वापस 0;
}

अटल अक्षर *junk_work_वृक्ष;
अटल अक्षर *junk_git_dir;
अटल पूर्णांक is_junk;
अटल pid_t junk_pid;

अटल व्योम remove_junk(व्योम)
{
	काष्ठा strbuf sb = STRBUF_INIT;
	अगर (!is_junk || getpid() != junk_pid)
		वापस;
	अगर (junk_git_dir) {
		strbuf_addstr(&sb, junk_git_dir);
		remove_dir_recursively(&sb, 0);
		strbuf_reset(&sb);
	}
	अगर (junk_work_वृक्ष) {
		strbuf_addstr(&sb, junk_work_वृक्ष);
		remove_dir_recursively(&sb, 0);
	}
	strbuf_release(&sb);
}

अटल व्योम remove_junk_on_signal(पूर्णांक signo)
{
	remove_junk();
	sigchain_निकालो(signo);
	उठाओ(signo);
}

अटल अक्षर *worktree_basename_dup(स्थिर अक्षर *path)
{
	स्थिर अक्षर *name;
	पूर्णांक len;

	len = म_माप(path);
	जबतक (len && is_dir_sep(path[len - 1]))
		len--;

	name = path + len;
	जबतक (name > path && !is_dir_sep(name[-1]))
		name--;

	वापस xmemdupz(name, path + len - name);
}

/* check that path is viable location for worktree */
अटल व्योम check_candidate_path(स्थिर अक्षर *path,
				 पूर्णांक force,
				 काष्ठा worktree **worktrees,
				 स्थिर अक्षर *cmd)
{
	काष्ठा worktree *wt;
	पूर्णांक locked;

	अगर (file_exists(path) && !is_empty_dir(path))
		die(_("'%s' already exists"), path);

	wt = find_worktree_by_path(worktrees, path);
	अगर (!wt)
		वापस;

	locked = !!worktree_lock_reason(wt);
	अगर ((!locked && force) || (locked && force > 1)) {
		अगर (delete_git_dir(wt->id))
		    die(_("unusable worktree destination '%s'"), path);
		वापस;
	}

	अगर (locked)
		die(_("'%s' is a missing but locked worktree;\nuse '%s -f -f' to override, or 'unlock' and 'prune' or 'remove' to clear"), path, cmd);
	अन्यथा
		die(_("'%s' is a missing but already registered worktree;\nuse '%s -f' to override, or 'prune' or 'remove' to clear"), path, cmd);
}

अटल व्योम copy_sparse_checkout(स्थिर अक्षर *worktree_git_dir)
{
	अक्षर *from_file = repo_git_path(the_repository, "info/sparse-checkout");
	अक्षर *to_file = xstrfmt("%s/info/sparse-checkout", worktree_git_dir);

	अगर (file_exists(from_file)) {
		अगर (safe_create_leading_directories(the_repository, to_file) ||
			copy_file(the_repository, to_file, from_file, 0666))
			error(_("failed to copy '%s' to '%s'; sparse-checkout may not work correctly"),
				from_file, to_file);
	}

	मुक्त(from_file);
	मुक्त(to_file);
}

अटल व्योम copy_filtered_worktree_config(स्थिर अक्षर *worktree_git_dir)
{
	अक्षर *from_file = repo_git_path(the_repository, "config.worktree");
	अक्षर *to_file = xstrfmt("%s/config.worktree", worktree_git_dir);

	अगर (file_exists(from_file)) {
		काष्ठा config_set cs = { { 0 } };
		पूर्णांक bare;

		अगर (safe_create_leading_directories(the_repository, to_file) ||
			copy_file(the_repository, to_file, from_file, 0666)) {
			error(_("failed to copy worktree config from '%s' to '%s'"),
				from_file, to_file);
			जाओ worktree_copy_cleanup;
		}

		git_configset_init(&cs);
		git_configset_add_file(&cs, from_file);

		अगर (!git_configset_get_bool(&cs, "core.bare", &bare) &&
			bare &&
			repo_config_set_multivar_in_file_gently(the_repository,
				to_file, "core.bare", शून्य, "true", शून्य, 0))
			error(_("failed to unset '%s' in '%s'"),
				"core.bare", to_file);
		अगर (!git_configset_get(&cs, "core.worktree") &&
			repo_config_set_in_file_gently(the_repository, to_file,
						       "core.worktree", शून्य, शून्य))
			error(_("failed to unset '%s' in '%s'"),
				"core.worktree", to_file);

		git_configset_clear(&cs);
	}

worktree_copy_cleanup:
	मुक्त(from_file);
	मुक्त(to_file);
}

अटल पूर्णांक checkout_worktree(स्थिर काष्ठा add_opts *opts,
			     काष्ठा strvec *child_env)
{
	काष्ठा child_process cp = CHILD_PROCESS_INIT;
	cp.git_cmd = 1;
	strvec_pushl(&cp.args, "reset", "--hard", "--no-recurse-submodules", शून्य);
	अगर (opts->quiet)
		strvec_धकेलो(&cp.args, "--quiet");
	strvec_pushv(&cp.env, child_env->v);
	वापस run_command(&cp);
}

अटल पूर्णांक make_worktree_orphan(स्थिर अक्षर * ref, स्थिर काष्ठा add_opts *opts,
				काष्ठा strvec *child_env)
{
	काष्ठा strbuf symref = STRBUF_INIT;
	काष्ठा child_process cp = CHILD_PROCESS_INIT;

	validate_new_branchname(ref, &symref, 0);
	strvec_pushl(&cp.args, "symbolic-ref", "HEAD", symref.buf, शून्य);
	अगर (opts->quiet)
		strvec_धकेलो(&cp.args, "--quiet");
	strvec_pushv(&cp.env, child_env->v);
	strbuf_release(&symref);
	cp.git_cmd = 1;
	वापस run_command(&cp);
}

/*
 * References for worktrees are generally stored in '$GIT_DIR/worktrees/<wt_id>'.
 * But when using alternate reference directories, we want to store the worktree
 * references in '$ALTERNATE_REFERENCE_DIR/worktrees/<wt_id>'.
 *
 * Create the necessary folder structure to facilitate the same. But to ensure
 * that the former path is still considered a Git directory, add stubs.
 */
अटल व्योम setup_alternate_ref_dir(काष्ठा worktree *wt, स्थिर अक्षर *wt_git_path)
{
	काष्ठा strbuf sb = STRBUF_INIT;
	अक्षर *path;

	path = wt->repo->ref_storage_payload;
	अगर (!path)
		वापस;

	अगर (!is_absolute_path(path))
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

अटल पूर्णांक add_worktree(स्थिर अक्षर *path, स्थिर अक्षर *refname,
			स्थिर काष्ठा add_opts *opts)
{
	काष्ठा strbuf sb_git = STRBUF_INIT, sb_repo = STRBUF_INIT;
	काष्ठा strbuf sb = STRBUF_INIT;
	स्थिर अक्षर *name;
	अक्षर *name_to_free = शून्य;
	काष्ठा strvec child_env = STRVEC_INIT;
	अचिन्हित पूर्णांक गणक = 0;
	पूर्णांक len, ret;
	काष्ठा strbuf symref = STRBUF_INIT;
	काष्ठा commit *commit = शून्य;
	पूर्णांक is_branch = 0;
	काष्ठा strbuf sb_name = STRBUF_INIT;
	काष्ठा worktree **worktrees, *wt = शून्य;
	काष्ठा ref_store *wt_refs;
	काष्ठा repo_config_मूल्यावली *cfg = repo_config_मूल्यावली(the_repository);

	worktrees = get_worktrees(the_repository);
	check_candidate_path(path, opts->force, worktrees, "add");
	free_worktrees(worktrees);
	worktrees = शून्य;

	/* is 'refname' a branch or commit? */
	अगर (!opts->detach && !check_branch_ref(the_repository, &symref, refname) &&
	    refs_ref_exists(get_main_ref_store(the_repository), symref.buf)) {
		is_branch = 1;
		अगर (!opts->force)
			die_if_checked_out(symref.buf, 0);
	}
	commit = lookup_commit_reference_by_name(refname);
	अगर (!commit && !opts->orphan)
		die(_("invalid reference: %s"), refname);

	name = name_to_free = worktree_basename_dup(path);
	अगर (!*name)
		die(_("invalid path '%s'"), path);
	sanitize_refname_component(name, &sb_name);
	अगर (!sb_name.len)
		BUG("How come '%s' becomes empty after sanitization?", name);
	name = sb_name.buf;
	repo_git_path_replace(the_repository, &sb_repo, "worktrees/%s", name);
	len = sb_repo.len;
	अगर (safe_create_leading_directories_const(the_repository, sb_repo.buf))
		die_errno(_("could not create leading directories of '%s'"),
			  sb_repo.buf);

	जबतक (सूची_गढ़ो(sb_repo.buf, 0777)) {
		गणक++;
		अगर ((त्रुटि_सं != EEXIST) || !गणक /* overflow */)
			die_errno(_("could not create directory of '%s'"),
				  sb_repo.buf);
		strbuf_setlen(&sb_repo, len);
		strbuf_addf(&sb_repo, "%d", गणक);
	}
	name = म_खोजप(sb_repo.buf, '/') + 1;

	junk_pid = getpid();
	निकास_पर(remove_junk);
	sigchain_धकेलो_common(remove_junk_on_signal);

	junk_git_dir = xstrdup(sb_repo.buf);
	is_junk = 1;

	/*
	 * lock the incomplete repo so prune won't delete it, unlock
	 * after the preparation is over.
	 */
	strbuf_addf(&sb, "%s/locked", sb_repo.buf);
	अगर (opts->keep_locked)
		write_file(sb.buf, "%s", opts->keep_locked);
	अन्यथा
		write_file(sb.buf, _("initializing"));

	strbuf_addf(&sb_git, "%s/.git", path);
	अगर (safe_create_leading_directories_const(the_repository, sb_git.buf))
		die_errno(_("could not create leading directories of '%s'"),
			  sb_git.buf);
	junk_work_वृक्ष = xstrdup(path);

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
	अगर (!wt) {
		ret = error(_("could not find created worktree '%s'"), name);
		जाओ done;
	}
	setup_alternate_ref_dir(wt, sb_repo.buf);
	wt_refs = get_worktree_ref_store(wt);

	ret = ref_store_create_on_disk(wt_refs, REF_STORE_CREATE_ON_DISK_IS_WORKTREE, &sb);
	अगर (ret)
		जाओ done;

	अगर (!is_branch && commit)
		ret = refs_update_ref(wt_refs, शून्य, "HEAD", &commit->object.oid,
				      शून्य, 0, UPDATE_REFS_संदेश_ON_ERR);
	अन्यथा
		ret = refs_update_symref(wt_refs, "HEAD", symref.buf, शून्य);
	अगर (ret)
		जाओ done;

	/*
	 * If the current worktree has sparse-checkout enabled, then copy
	 * the sparse-checkout patterns from the current worktree.
	 */
	अगर (cfg->apply_sparse_checkout)
		copy_sparse_checkout(sb_repo.buf);

	/*
	 * If we are using worktree config, then copy all current config
	 * values from the current worktree into the new one, that way the
	 * new worktree behaves the same as this one.
	 */
	अगर (the_repository->repository_format_worktree_config)
		copy_filtered_worktree_config(sb_repo.buf);

	strvec_pushf(&child_env, "%s=%s", GIT_DIR_ENVIRONMENT, sb_git.buf);
	strvec_pushf(&child_env, "%s=%s", GIT_WORK_वृक्ष_ENVIRONMENT, path);

	अगर (opts->orphan &&
	    (ret = make_worktree_orphan(refname, opts, &child_env)))
		जाओ done;

	अगर (opts->checkout &&
	    (ret = checkout_worktree(opts, &child_env)))
		जाओ done;

	is_junk = 0;
	FREE_AND_NULL(junk_work_वृक्ष);
	FREE_AND_NULL(junk_git_dir);

done:
	अगर (ret || !opts->keep_locked) {
		strbuf_reset(&sb);
		strbuf_addf(&sb, "%s/locked", sb_repo.buf);
		unlink_or_warn(sb.buf);
	}

	/*
	 * Hook failure does not warrant worktree deletion, so run hook after
	 * is_junk is cleared, but do return appropriate code when hook fails.
	 */
	अगर (!ret && opts->checkout && !opts->orphan) {
		काष्ठा run_hooks_opt opt = RUN_HOOKS_OPT_INIT_FORCE_SERIAL;

		strvec_pushl(&opt.env, "GIT_DIR", "GIT_WORK_TREE", शून्य);
		strvec_pushl(&opt.args,
			     oid_to_hex(null_oid(the_hash_algo)),
			     oid_to_hex(&commit->object.oid),
			     "1",
			     शून्य);
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
	मुक्त(name_to_free);
	वापस ret;
}

अटल व्योम print_preparing_worktree_पंक्ति(पूर्णांक detach,
					  स्थिर अक्षर *branch,
					  स्थिर अक्षर *new_branch,
					  पूर्णांक force_new_branch)
{
	अगर (force_new_branch) {
		काष्ठा commit *commit = lookup_commit_reference_by_name(new_branch);
		अगर (!commit)
			fprintf_ln(मानक_त्रुटि, _("Preparing worktree (new branch '%s')"), new_branch);
		अन्यथा
			fprintf_ln(मानक_त्रुटि, _("Preparing worktree (resetting branch '%s'; was at %s)"),
				  new_branch,
				  repo_find_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV));
	} अन्यथा अगर (new_branch) {
		fprintf_ln(मानक_त्रुटि, _("Preparing worktree (new branch '%s')"), new_branch);
	} अन्यथा {
		काष्ठा strbuf s = STRBUF_INIT;
		अगर (!detach && !check_branch_ref(the_repository, &s, branch) &&
		    refs_ref_exists(get_main_ref_store(the_repository), s.buf))
			fprintf_ln(मानक_त्रुटि, _("Preparing worktree (checking out '%s')"),
				  branch);
		अन्यथा {
			काष्ठा commit *commit = lookup_commit_reference_by_name(branch);
			अगर (!commit)
				BUG("unreachable: invalid reference: %s", branch);
			fprintf_ln(मानक_त्रुटि, _("Preparing worktree (detached HEAD %s)"),
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
अटल पूर्णांक first_valid_ref(स्थिर काष्ठा reference *ref UNUSED, व्योम *cb_तथ्य UNUSED)
{
	वापस 1;
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
अटल पूर्णांक can_use_local_refs(स्थिर काष्ठा add_opts *opts)
{
	अगर (refs_शीर्ष_ref(get_main_ref_store(the_repository), first_valid_ref, शून्य)) {
		वापस 1;
	} अन्यथा अगर (refs_for_each_branch_ref(get_main_ref_store(the_repository), first_valid_ref, शून्य)) {
		अगर (!opts->quiet)
			warning(_("HEAD points to an invalid (or orphaned) reference.\n"));
		वापस 1;
	}
	वापस 0;
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
अटल पूर्णांक can_use_remote_refs(स्थिर काष्ठा add_opts *opts)
{
	अगर (!guess_remote) {
		वापस 0;
	} अन्यथा अगर (refs_for_each_remote_ref(get_main_ref_store(the_repository), first_valid_ref, शून्य)) {
		वापस 1;
	} अन्यथा अगर (!opts->force && remote_get(शून्य)) {
		die(_("No local or remote refs exist despite at least one remote\n"
		      "present, stopping; use 'add -f' to override or fetch a remote first"));
	}
	वापस 0;
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
अटल पूर्णांक dwim_orphan(स्थिर काष्ठा add_opts *opts, पूर्णांक opt_track, पूर्णांक remote)
{
	अगर (can_use_local_refs(opts)) {
		वापस 0;
	} अन्यथा अगर (remote && can_use_remote_refs(opts)) {
		वापस 0;
	} अन्यथा अगर (!opts->quiet) {
		fprintf_ln(मानक_त्रुटि, WORKTREE_ADD_DWIM_ORPHAN_INFER_TEXT);
	}

	अगर (opt_track) {
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--track");
	} अन्यथा अगर (!opts->checkout) {
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--no-checkout");
	}
	वापस 1;
}

अटल व्योम advise_disambiguating_remotes(स्थिर अक्षर *path, स्थिर अक्षर *branch,
					  स्थिर काष्ठा string_list *matched_remote_names)
{
	काष्ठा string_list_item *item;

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

अटल अक्षर *dwim_branch(स्थिर काष्ठा add_opts *opts, स्थिर अक्षर *path, अक्षर **new_branch)
{
	पूर्णांक branch_exists;
	अक्षर *branchname = worktree_basename_dup(path);
	काष्ठा strbuf ref = STRBUF_INIT;

	branch_exists = !check_branch_ref(the_repository, &ref, branchname) &&
			refs_ref_exists(get_main_ref_store(the_repository),
					ref.buf);
	strbuf_release(&ref);
	अगर (branch_exists)
		वापस branchname;

	*new_branch = branchname;
	अगर (guess_remote) {
		काष्ठा object_id oid;
		अक्षर *remote;
		पूर्णांक अंक_matches = 0;
		काष्ठा string_list matched_remote_names = STRING_LIST_INIT_DUP;

		remote = unique_tracking_name(*new_branch, &oid, &अंक_matches,
					      &matched_remote_names);
		अगर (!remote && अंक_matches > 1) {
			अगर (!opts->quiet &&
			    advice_enabled(ADVICE_CHECKOUT_AMBIGUOUS_REMOTE_BRANCH_NAME))
				advise_disambiguating_remotes(path, *new_branch,
							      &matched_remote_names);
			die(_("'%s' matched multiple (%d) remote tracking branches"),
			    *new_branch, अंक_matches);
		}
		string_list_clear(&matched_remote_names, 0);
		वापस remote;
	}
	वापस शून्य;
}

अटल पूर्णांक add(पूर्णांक ac, स्थिर अक्षर **av, स्थिर अक्षर *prefix,
	       काष्ठा repository *repo UNUSED)
{
	काष्ठा add_opts opts;
	स्थिर अक्षर *new_branch_force = शून्य;
	अक्षर *path;
	स्थिर अक्षर *branch;
	अक्षर *branch_to_free = शून्य;
	अक्षर *new_branch_to_free = शून्य;
	स्थिर अक्षर *new_branch = शून्य;
	अक्षर *opt_track = शून्य;
	स्थिर अक्षर *lock_reason = शून्य;
	पूर्णांक keep_locked = 0;
	पूर्णांक used_new_branch_options;
	काष्ठा option options[] = {
		OPT__FORCE(&opts.force,
			   N_("checkout <branch> even if already checked out in other worktree"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_STRING('b', शून्य, &new_branch, N_("branch"),
			   N_("create a new branch")),
		OPT_STRING('B', शून्य, &new_branch_force, N_("branch"),
			   N_("create or reset a branch")),
		OPT_BOOL(0, "orphan", &opts.orphan, N_("create unborn branch")),
		OPT_BOOL('d', "detach", &opts.detach, N_("detach HEAD at named commit")),
		OPT_BOOL(0, "checkout", &opts.checkout, N_("populate the new working tree")),
		OPT_BOOL(0, "lock", &keep_locked, N_("keep the new working tree locked")),
		OPT_STRING(0, "reason", &lock_reason, N_("string"),
			   N_("reason for locking")),
		OPT__QUIET(&opts.quiet, N_("suppress progress reporting")),
		OPT_PASSTHRU(0, "track", &opt_track, शून्य,
			     N_("set up tracking mode (see git-branch(1))"),
			     PARSE_OPT_NOARG | PARSE_OPT_OPTARG),
		OPT_BOOL(0, "guess-remote", &guess_remote,
			 N_("try to match the new branch name with a remote-tracking branch")),
		OPT_BOOL(0, "relative-paths", &opts.relative_paths,
			 N_("use relative paths for worktrees")),
		OPT_END()
	};
	पूर्णांक ret;

	स_रखो(&opts, 0, माप(opts));
	opts.checkout = 1;
	opts.relative_paths = use_relative_paths;
	ac = parse_options(ac, av, prefix, options, git_worktree_add_usage, 0);
	अगर (!!opts.detach + !!new_branch + !!new_branch_force > 1)
		die(_("options '%s', '%s', and '%s' cannot be used together"), "-b", "-B", "--detach");
	अगर (opts.detach && opts.orphan)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--detach");
	अगर (opts.orphan && opt_track)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--track");
	अगर (opts.orphan && !opts.checkout)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--no-checkout");
	अगर (opts.orphan && ac == 2)
		die(_("option '%s' and commit-ish cannot be used together"),
		    "--orphan");
	अगर (lock_reason && !keep_locked)
		die(_("the option '%s' requires '%s'"), "--reason", "--lock");
	अगर (lock_reason)
		opts.keep_locked = lock_reason;
	अन्यथा अगर (keep_locked)
		opts.keep_locked = _("added with --lock");

	अगर (ac < 1 || ac > 2)
		usage_with_options(git_worktree_add_usage, options);

	path = prefix_filename(prefix, av[0]);
	branch = ac < 2 ? "HEAD" : av[1];
	used_new_branch_options = new_branch || new_branch_force;

	अगर (!म_भेद(branch, "-"))
		branch = "@{-1}";

	अगर (new_branch_force) {
		काष्ठा strbuf symref = STRBUF_INIT;

		new_branch = new_branch_force;

		अगर (!opts.force &&
		    !check_branch_ref(the_repository, &symref, new_branch) &&
		    refs_ref_exists(get_main_ref_store(the_repository), symref.buf))
			die_if_checked_out(symref.buf, 0);
		strbuf_release(&symref);
	}

	अगर (opts.orphan && !new_branch) {
		new_branch = new_branch_to_free = worktree_basename_dup(path);
	} अन्यथा अगर (opts.orphan) {
		; /* no-op */
	} अन्यथा अगर (opts.detach) {
		/* Check HEAD */
		अगर (!म_भेद(branch, "HEAD"))
			can_use_local_refs(&opts);
	} अन्यथा अगर (ac < 2 && new_branch) {
		/* DWIM: Infer --orphan when repo has no refs. */
		opts.orphan = dwim_orphan(&opts, !!opt_track, 0);
	} अन्यथा अगर (ac < 2) {
		/* DWIM: Guess branch name from path. */
		अक्षर *s = dwim_branch(&opts, path, &new_branch_to_free);
		अगर (s)
			branch = branch_to_free = s;
		new_branch = new_branch_to_free;

		/* DWIM: Infer --orphan when repo has no refs. */
		opts.orphan = (!s) && dwim_orphan(&opts, !!opt_track, 1);
	} अन्यथा अगर (ac == 2 && new_branch) {
		अगर (!म_भेद(branch, "HEAD"))
			can_use_local_refs(&opts);
	} अन्यथा अगर (ac == 2) {
		काष्ठा commit *commit;

		commit = lookup_commit_reference_by_name(branch);
		अगर (!commit) {
			काष्ठा object_id oid;
			अक्षर *remote;
			पूर्णांक अंक_matches = 0;
			काष्ठा string_list matched_remote_names = STRING_LIST_INIT_DUP;

			remote = unique_tracking_name(branch, &oid, &अंक_matches,
						      &matched_remote_names);
			अगर (remote) {
				new_branch = branch;
				branch = new_branch_to_free = remote;
			} अन्यथा अगर (अंक_matches > 1) {
				अगर (!opts.quiet &&
				    advice_enabled(ADVICE_CHECKOUT_AMBIGUOUS_REMOTE_BRANCH_NAME))
					advise_disambiguating_remotes(path, branch,
								      &matched_remote_names);
				die(_("'%s' matched multiple (%d) remote tracking branches"),
				    branch, अंक_matches);
			}
			string_list_clear(&matched_remote_names, 0);
		}

		अगर (!म_भेद(branch, "HEAD"))
			can_use_local_refs(&opts);

	}

	अगर (!opts.orphan && !lookup_commit_reference_by_name(branch)) {
		पूर्णांक attempt_hint = !opts.quiet && (ac < 2);
		अगर (attempt_hint && used_new_branch_options) {
			advise_if_enabled(ADVICE_WORKTREE_ADD_ORPHAN,
				WORKTREE_ADD_ORPHAN_WITH_DASH_B_HINT_TEXT,
				new_branch, path);
		} अन्यथा अगर (attempt_hint) {
			advise_if_enabled(ADVICE_WORKTREE_ADD_ORPHAN,
				WORKTREE_ADD_ORPHAN_NO_DASH_B_HINT_TEXT, path);
		}
		die(_("invalid reference: %s"), branch);
	}

	अगर (!opts.quiet)
		print_preparing_worktree_पंक्ति(opts.detach, branch, new_branch, !!new_branch_force);

	अगर (opts.orphan) {
		branch = new_branch;
	} अन्यथा अगर (new_branch) {
		काष्ठा child_process cp = CHILD_PROCESS_INIT;
		cp.git_cmd = 1;
		strvec_धकेलो(&cp.args, "branch");
		अगर (new_branch_force)
			strvec_धकेलो(&cp.args, "--force");
		अगर (opts.quiet)
			strvec_धकेलो(&cp.args, "--quiet");
		strvec_धकेलो(&cp.args, new_branch);
		strvec_धकेलो(&cp.args, branch);
		अगर (opt_track)
			strvec_धकेलो(&cp.args, opt_track);
		अगर (run_command(&cp)) {
			ret = -1;
			जाओ cleanup;
		}
		branch = new_branch;
	} अन्यथा अगर (opt_track) {
		die(_("--[no-]track can only be used if a new branch is created"));
	}

	ret = add_worktree(path, branch, &opts);
cleanup:
	मुक्त(path);
	मुक्त(opt_track);
	मुक्त(branch_to_free);
	मुक्त(new_branch_to_free);
	वापस ret;
}

अटल व्योम show_worktree_porcelain(काष्ठा worktree *wt, पूर्णांक पंक्ति_terminator)
{
	स्थिर अक्षर *reason;

	म_लिखो("worktree %s%c", wt->path, पंक्ति_terminator);
	अगर (wt->is_bare)
		म_लिखो("bare%c", पंक्ति_terminator);
	अन्यथा {
		म_लिखो("HEAD %s%c", oid_to_hex(&wt->शीर्ष_oid), पंक्ति_terminator);
		अगर (wt->is_detached)
			म_लिखो("detached%c", पंक्ति_terminator);
		अन्यथा अगर (wt->शीर्ष_ref)
			म_लिखो("branch %s%c", wt->शीर्ष_ref, पंक्ति_terminator);
	}

	reason = worktree_lock_reason(wt);
	अगर (reason) {
		ख_माला_दो("locked", मानक_निकास);
		अगर (*reason) {
			ख_अक्षर_दो(' ', मानक_निकास);
			write_name_quoted(reason, मानक_निकास, पंक्ति_terminator);
		} अन्यथा {
			ख_अक्षर_दो(पंक्ति_terminator, मानक_निकास);
		}
	}

	reason = worktree_prune_reason(wt, expire);
	अगर (reason)
		म_लिखो("prunable %s%c", reason, पंक्ति_terminator);

	ख_अक्षर_दो(पंक्ति_terminator, मानक_निकास);
}

काष्ठा worktree_display {
	अक्षर *path;
	पूर्णांक width;
};

अटल व्योम show_worktree(काष्ठा worktree *wt, काष्ठा worktree_display *display,
			  पूर्णांक path_maxwidth, पूर्णांक abbrev_len)
{
	काष्ठा strbuf sb = STRBUF_INIT;
	स्थिर अक्षर *reason;

	strbuf_addf(&sb, "%s%*s", display->path, 1 + path_maxwidth - display->width, "");
	अगर (wt->is_bare)
		strbuf_addstr(&sb, "(bare)");
	अन्यथा {
		strbuf_addf(&sb, "%-*s ", abbrev_len,
				repo_find_unique_abbrev(the_repository, &wt->शीर्ष_oid, DEFAULT_ABBREV));
		अगर (wt->is_detached)
			strbuf_addstr(&sb, "(detached HEAD)");
		अन्यथा अगर (wt->शीर्ष_ref) {
			अक्षर *ref = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
								 wt->शीर्ष_ref,
								 0);
			strbuf_addf(&sb, "[%s]", ref);
			मुक्त(ref);
		} अन्यथा
			strbuf_addstr(&sb, "(error)");
	}

	reason = worktree_lock_reason(wt);
	अगर (verbose && reason && *reason)
		strbuf_addf(&sb, "\n\tlocked: %s", reason);
	अन्यथा अगर (reason)
		strbuf_addstr(&sb, " locked");

	reason = worktree_prune_reason(wt, expire);
	अगर (verbose && reason)
		strbuf_addf(&sb, "\n\tprunable: %s", reason);
	अन्यथा अगर (reason)
		strbuf_addstr(&sb, " prunable");

	म_लिखो("%s\n", sb.buf);
	strbuf_release(&sb);
}

अटल व्योम measure_widths(काष्ठा worktree **wt, पूर्णांक *abbrev,
			   काष्ठा worktree_display **d, पूर्णांक *maxwidth)
{
	पूर्णांक i, display_alloc = 0;
	काष्ठा worktree_display *display = शून्य;
	काष्ठा strbuf buf = STRBUF_INIT;

	क्रम (i = 0; wt[i]; i++) {
		पूर्णांक sha1_len;
		ALLOC_GROW(display, i + 1, display_alloc);
		quote_path(wt[i]->path, शून्य, &buf, 0);
		display[i].width = utf8_strwidth(buf.buf);
		display[i].path = strbuf_detach(&buf, शून्य);

		अगर (display[i].width > *maxwidth)
			*maxwidth = display[i].width;
		sha1_len = म_माप(repo_find_unique_abbrev(the_repository, &wt[i]->शीर्ष_oid, *abbrev));
		अगर (sha1_len > *abbrev)
			*abbrev = sha1_len;
	}
	*d = display;
}

अटल पूर्णांक pathcmp(स्थिर व्योम *a_, स्थिर व्योम *b_)
{
	स्थिर काष्ठा worktree *स्थिर *a = a_;
	स्थिर काष्ठा worktree *स्थिर *b = b_;
	वापस fspathcmp((*a)->path, (*b)->path);
}

अटल व्योम pathsort(काष्ठा worktree **wt)
{
	पूर्णांक n = 0;
	काष्ठा worktree **p = wt;

	जबतक (*p++)
		n++;
	QSORT(wt, n, pathcmp);
}

अटल पूर्णांक list(पूर्णांक ac, स्थिर अक्षर **av, स्थिर अक्षर *prefix,
		काष्ठा repository *repo UNUSED)
{
	पूर्णांक porcelain = 0;
	पूर्णांक पंक्ति_terminator = '\n';

	काष्ठा option options[] = {
		OPT_BOOL(0, "porcelain", &porcelain, N_("machine-readable output")),
		OPT__VERBOSE(&verbose, N_("show extended annotations and reasons, if available")),
		OPT_EXPIRY_DATE(0, "expire", &expire,
				N_("add 'prunable' annotation to missing worktrees older than <time>")),
		OPT_SET_INT('z', शून्य, &पंक्ति_terminator,
			    N_("terminate records with a NUL character"), '\0'),
		OPT_END()
	};

	expire = TIME_MAX;
	ac = parse_options(ac, av, prefix, options, git_worktree_list_usage, 0);
	अगर (ac)
		usage_with_options(git_worktree_list_usage, options);
	अन्यथा अगर (verbose && porcelain)
		die(_("options '%s' and '%s' cannot be used together"), "--verbose", "--porcelain");
	अन्यथा अगर (!पंक्ति_terminator && !porcelain)
		die(_("the option '%s' requires '%s'"), "-z", "--porcelain");
	अन्यथा {
		काष्ठा worktree **worktrees = get_worktrees(the_repository);
		पूर्णांक path_maxwidth = 0, abbrev = DEFAULT_ABBREV, i;
		काष्ठा worktree_display *display = शून्य;

		/* sort worktrees by path but keep main worktree at top */
		pathsort(worktrees + 1);

		अगर (!porcelain)
			measure_widths(worktrees, &abbrev,
				       &display, &path_maxwidth);

		क्रम (i = 0; worktrees[i]; i++) {
			अगर (porcelain)
				show_worktree_porcelain(worktrees[i],
							पंक्ति_terminator);
			अन्यथा
				show_worktree(worktrees[i],
					      &display[i], path_maxwidth, abbrev);
		}
		क्रम (i = 0; display && worktrees[i]; i++)
			मुक्त(display[i].path);
		मुक्त(display);
		free_worktrees(worktrees);
	}
	वापस 0;
}

अटल पूर्णांक lock_worktree(पूर्णांक ac, स्थिर अक्षर **av, स्थिर अक्षर *prefix,
			 काष्ठा repository *repo UNUSED)
{
	स्थिर अक्षर *reason = "", *old_reason;
	काष्ठा option options[] = {
		OPT_STRING(0, "reason", &reason, N_("string"),
			   N_("reason for locking")),
		OPT_END()
	};
	काष्ठा worktree **worktrees, *wt;
	अक्षर *path;

	ac = parse_options(ac, av, prefix, options, git_worktree_lock_usage, 0);
	अगर (ac != 1)
		usage_with_options(git_worktree_lock_usage, options);

	worktrees = get_worktrees(the_repository);
	wt = find_worktree(worktrees, prefix, av[0]);
	अगर (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	अगर (is_main_worktree(wt))
		die(_("The main working tree cannot be locked or unlocked"));

	old_reason = worktree_lock_reason(wt);
	अगर (old_reason) {
		अगर (*old_reason)
			die(_("'%s' is already locked, reason: %s"),
			    av[0], old_reason);
		die(_("'%s' is already locked"), av[0]);
	}

	path = repo_common_path(the_repository, "worktrees/%s/locked", wt->id);
	write_file(path, "%s", reason);

	free_worktrees(worktrees);
	मुक्त(path);
	वापस 0;
}

अटल पूर्णांक unlock_worktree(पूर्णांक ac, स्थिर अक्षर **av, स्थिर अक्षर *prefix,
			   काष्ठा repository *repo UNUSED)
{
	काष्ठा option options[] = {
		OPT_END()
	};
	काष्ठा worktree **worktrees, *wt;
	अक्षर *path;
	पूर्णांक ret;

	ac = parse_options(ac, av, prefix, options, git_worktree_unlock_usage, 0);
	अगर (ac != 1)
		usage_with_options(git_worktree_unlock_usage, options);

	worktrees = get_worktrees(the_repository);
	wt = find_worktree(worktrees, prefix, av[0]);
	अगर (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	अगर (is_main_worktree(wt))
		die(_("The main working tree cannot be locked or unlocked"));
	अगर (!worktree_lock_reason(wt))
		die(_("'%s' is not locked"), av[0]);

	path = repo_common_path(the_repository, "worktrees/%s/locked", wt->id);
	ret = unlink_or_warn(path);

	free_worktrees(worktrees);
	मुक्त(path);
	वापस ret;
}

अटल व्योम validate_no_submodules(स्थिर काष्ठा worktree *wt)
{
	काष्ठा सूचकांक_state istate = सूचकांक_STATE_INIT(the_repository);
	काष्ठा strbuf path = STRBUF_INIT;
	पूर्णांक i, found_submodules = 0;
	अक्षर *wt_gitdir;

	wt_gitdir = get_worktree_git_dir(wt);

	अगर (is_directory(worktree_git_path(wt, "modules"))) {
		/*
		 * There could be false positives, e.g. the "modules"
		 * directory exists but is empty. But it's a rare case and
		 * this simpler check is probably good enough for now.
		 */
		found_submodules = 1;
	} अन्यथा अगर (read_सूचकांक_from(&istate, worktree_git_path(wt, "index"),
				   wt_gitdir) > 0) {
		क्रम (i = 0; i < istate.cache_nr; i++) {
			काष्ठा cache_entry *ce = istate.cache[i];
			पूर्णांक err;

			अगर (!S_ISGITLINK(ce->ce_mode))
				जारी;

			strbuf_reset(&path);
			strbuf_addf(&path, "%s/%s", wt->path, ce->name);
			अगर (!is_submodule_populated_gently(path.buf, &err))
				जारी;

			found_submodules = 1;
			अवरोध;
		}
	}
	discard_सूचकांक(&istate);
	strbuf_release(&path);
	मुक्त(wt_gitdir);

	अगर (found_submodules)
		die(_("working trees containing submodules cannot be moved or removed"));
}

अटल पूर्णांक move_worktree(पूर्णांक ac, स्थिर अक्षर **av, स्थिर अक्षर *prefix,
			 काष्ठा repository *repo UNUSED)
{
	पूर्णांक force = 0;
	काष्ठा option options[] = {
		OPT__FORCE(&force,
			 N_("force move even if worktree is dirty or locked"),
			 PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "relative-paths", &use_relative_paths,
			 N_("use relative paths for worktrees")),
		OPT_END()
	};
	काष्ठा worktree **worktrees, *wt;
	काष्ठा strbuf dst = STRBUF_INIT;
	काष्ठा strbuf errmsg = STRBUF_INIT;
	स्थिर अक्षर *reason = शून्य;
	अक्षर *path;

	ac = parse_options(ac, av, prefix, options, git_worktree_move_usage,
			   0);
	अगर (ac != 2)
		usage_with_options(git_worktree_move_usage, options);

	path = prefix_filename(prefix, av[1]);
	strbuf_addstr(&dst, path);
	मुक्त(path);

	worktrees = get_worktrees(the_repository);
	wt = find_worktree(worktrees, prefix, av[0]);
	अगर (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	अगर (is_main_worktree(wt))
		die(_("'%s' is a main working tree"), av[0]);
	अगर (is_directory(dst.buf)) {
		स्थिर अक्षर *sep = find_last_dir_sep(wt->path);

		अगर (!sep)
			die(_("could not figure out destination name from '%s'"),
			    wt->path);
		strbuf_trim_trailing_dir_sep(&dst);
		strbuf_addstr(&dst, sep);
	}
	check_candidate_path(dst.buf, force, worktrees, "move");

	validate_no_submodules(wt);

	अगर (force < 2)
		reason = worktree_lock_reason(wt);
	अगर (reason) {
		अगर (*reason)
			die(_("cannot move a locked working tree, lock reason: %s\nuse 'move -f -f' to override or unlock first"),
			    reason);
		die(_("cannot move a locked working tree;\nuse 'move -f -f' to override or unlock first"));
	}
	अगर (validate_worktree(wt, &errmsg, 0))
		die(_("validation failed, cannot move working tree: %s"),
		    errmsg.buf);
	strbuf_release(&errmsg);

	अगर (नाम(wt->path, dst.buf) == -1)
		die_errno(_("failed to move '%s' to '%s'"), wt->path, dst.buf);

	update_worktree_location(wt, dst.buf, use_relative_paths);

	strbuf_release(&dst);
	free_worktrees(worktrees);
	वापस 0;
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
अटल व्योम check_clean_worktree(काष्ठा worktree *wt,
				 स्थिर अक्षर *original_path)
{
	काष्ठा child_process cp;
	अक्षर buf[1];
	पूर्णांक ret;

	/*
	 * Until we sort this out, all submodules are "dirty" and
	 * will abort this function.
	 */
	validate_no_submodules(wt);

	child_process_init(&cp);
	strvec_pushf(&cp.env, "%s=%s/.git",
		     GIT_DIR_ENVIRONMENT, wt->path);
	strvec_pushf(&cp.env, "%s=%s",
		     GIT_WORK_वृक्ष_ENVIRONMENT, wt->path);
	strvec_pushl(&cp.args, "status",
		     "--porcelain", "--ignore-submodules=none",
		     शून्य);
	cp.git_cmd = 1;
	cp.dir = wt->path;
	cp.out = -1;
	ret = start_command(&cp);
	अगर (ret)
		die_errno(_("failed to run 'git status' on '%s'"),
			  original_path);
	ret = xread(cp.out, buf, माप(buf));
	अगर (ret)
		die(_("'%s' contains modified or untracked files, use --force to delete it"),
		    original_path);
	बंद(cp.out);
	ret = finish_command(&cp);
	अगर (ret)
		die_errno(_("failed to run 'git status' on '%s', code %d"),
			  original_path, ret);
}

अटल पूर्णांक delete_git_work_वृक्ष(काष्ठा worktree *wt)
{
	काष्ठा strbuf sb = STRBUF_INIT;
	पूर्णांक ret = 0;

	strbuf_addstr(&sb, wt->path);
	अगर (remove_dir_recursively(&sb, 0)) {
		error_errno(_("failed to delete '%s'"), sb.buf);
		ret = -1;
	}
	strbuf_release(&sb);
	वापस ret;
}

अटल पूर्णांक remove_worktree(पूर्णांक ac, स्थिर अक्षर **av, स्थिर अक्षर *prefix,
			   काष्ठा repository *repo UNUSED)
{
	पूर्णांक force = 0;
	काष्ठा option options[] = {
		OPT__FORCE(&force,
			 N_("force removal even if worktree is dirty or locked"),
			 PARSE_OPT_NOCOMPLETE),
		OPT_END()
	};
	काष्ठा worktree **worktrees, *wt;
	काष्ठा strbuf errmsg = STRBUF_INIT;
	स्थिर अक्षर *reason = शून्य;
	पूर्णांक ret = 0;

	ac = parse_options(ac, av, prefix, options, git_worktree_remove_usage, 0);
	अगर (ac != 1)
		usage_with_options(git_worktree_remove_usage, options);

	worktrees = get_worktrees(the_repository);
	wt = find_worktree(worktrees, prefix, av[0]);
	अगर (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	अगर (is_main_worktree(wt))
		die(_("'%s' is a main working tree"), av[0]);
	अगर (force < 2)
		reason = worktree_lock_reason(wt);
	अगर (reason) {
		अगर (*reason)
			die(_("cannot remove a locked working tree, lock reason: %s\nuse 'remove -f -f' to override or unlock first"),
			    reason);
		die(_("cannot remove a locked working tree;\nuse 'remove -f -f' to override or unlock first"));
	}
	अगर (validate_worktree(wt, &errmsg, WT_VALIDATE_WORKTREE_MISSING_OK))
		die(_("validation failed, cannot remove working tree: %s"),
		    errmsg.buf);
	strbuf_release(&errmsg);

	अगर (file_exists(wt->path)) {
		अगर (!force)
			check_clean_worktree(wt, av[0]);

		ret |= delete_git_work_वृक्ष(wt);
	}
	/*
	 * continue on even if ret is non-zero, there's no going back
	 * from here.
	 */
	ret |= delete_git_dir(wt->id);
	delete_worktrees_dir_if_empty();

	free_worktrees(worktrees);
	वापस ret;
}

अटल व्योम report_repair(पूर्णांक iserr, स्थिर अक्षर *path, स्थिर अक्षर *संदेश, व्योम *cb_तथ्य)
{
	अगर (!iserr) {
		fprintf_ln(मानक_त्रुटि, _("repair: %s: %s"), संदेश, path);
	} अन्यथा {
		पूर्णांक *exit_status = (पूर्णांक *)cb_तथ्य;
		fprintf_ln(मानक_त्रुटि, _("error: %s: %s"), संदेश, path);
		*exit_status = 1;
	}
}

अटल पूर्णांक repair(पूर्णांक ac, स्थिर अक्षर **av, स्थिर अक्षर *prefix,
		  काष्ठा repository *repo UNUSED)
{
	स्थिर अक्षर **p;
	स्थिर अक्षर *self[] = { ".", शून्य };
	काष्ठा option options[] = {
		OPT_BOOL(0, "relative-paths", &use_relative_paths,
			 N_("use relative paths for worktrees")),
		OPT_END()
	};
	पूर्णांक rc = 0;

	ac = parse_options(ac, av, prefix, options, git_worktree_repair_usage, 0);
	p = ac > 0 ? av : self;
	क्रम (; *p; p++)
		repair_worktree_at_path(the_repository, *p, report_repair,
					&rc, use_relative_paths);
	repair_worktrees(the_repository, report_repair, &rc, use_relative_paths);
	वापस rc;
}

पूर्णांक cmd_worktree(पूर्णांक ac,
		 स्थिर अक्षर **av,
		 स्थिर अक्षर *prefix,
		 काष्ठा repository *repo)
{
	parse_opt_subcommand_fn *fn = शून्य;
	काष्ठा option options[] = {
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

	repo_config(the_repository, git_worktree_config, शून्य);

	अगर (!prefix)
		prefix = "";

	ac = parse_options(ac, av, prefix, options, git_worktree_usage, 0);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_सूचकांक = 0;

	वापस fn(ac, av, prefix, repo);
}
