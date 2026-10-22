#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "abspath.h"
#अन्तर्भाव "advice.h"
#अन्तर्भाव "checkout.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "copy.h"
#अन्तर्भाव "dir.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "object-file.h"
#अन्तर्भाव "object-name.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "path.h"
#अन्तर्भाव "strvec.h"
#अन्तर्भाव "branch.h"
#अन्तर्भाव "read-cache-ll.h"
#अन्तर्भाव "refs.h"
#अन्तर्भाव "remote.h"
#अन्तर्भाव "run-command.h"
#अन्तर्भाव "hook.h"
#अन्तर्भाव "sigchain.h"
#अन्तर्भाव "submodule.h"
#अन्तर्भाव "utf8.h"
#अन्तर्भाव "worktree.h"
#अन्तर्भाव "quote.h"

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

स्थिर ध्रुव वर्ण * ध्रुव git_worktree_usage[] = {
	BUILTIN_WORKTREE_ADD_USAGE,
	BUILTIN_WORKTREE_LIST_USAGE,
	BUILTIN_WORKTREE_LOCK_USAGE,
	BUILTIN_WORKTREE_MOVE_USAGE,
	BUILTIN_WORKTREE_PRUNE_USAGE,
	BUILTIN_WORKTREE_REMOVE_USAGE,
	BUILTIN_WORKTREE_REPAIR_USAGE,
	BUILTIN_WORKTREE_UNLOCK_USAGE,
	रिक्त
};

स्थिर ध्रुव वर्ण * ध्रुव git_worktree_add_usage[] = {
	BUILTIN_WORKTREE_ADD_USAGE,
	रिक्त,
};

स्थिर ध्रुव वर्ण * ध्रुव git_worktree_list_usage[] = {
	BUILTIN_WORKTREE_LIST_USAGE,
	रिक्त
};

स्थिर ध्रुव वर्ण * ध्रुव git_worktree_lock_usage[] = {
	BUILTIN_WORKTREE_LOCK_USAGE,
	रिक्त
};

स्थिर ध्रुव वर्ण * ध्रुव git_worktree_move_usage[] = {
	BUILTIN_WORKTREE_MOVE_USAGE,
	रिक्त
};

स्थिर ध्रुव वर्ण * ध्रुव git_worktree_prune_usage[] = {
	BUILTIN_WORKTREE_PRUNE_USAGE,
	रिक्त
};

स्थिर ध्रुव वर्ण * ध्रुव git_worktree_remove_usage[] = {
	BUILTIN_WORKTREE_REMOVE_USAGE,
	रिक्त
};

स्थिर ध्रुव वर्ण * ध्रुव git_worktree_repair_usage[] = {
	BUILTIN_WORKTREE_REPAIR_USAGE,
	रिक्त
};

स्थिर ध्रुव वर्ण * ध्रुव git_worktree_unlock_usage[] = {
	BUILTIN_WORKTREE_UNLOCK_USAGE,
	रिक्त
};

संरचना add_opts {
	पूर्णाङ्क force;
	पूर्णाङ्क detach;
	पूर्णाङ्क quiet;
	पूर्णाङ्क checkout;
	पूर्णाङ्क orphan;
	पूर्णाङ्क relative_paths;
	ध्रुव वर्ण *keep_locked;
};

स्थिर पूर्णाङ्क show_only;
स्थिर पूर्णाङ्क verbose;
स्थिर पूर्णाङ्क guess_remote;
स्थिर पूर्णाङ्क use_relative_paths;
स्थिर timestamp_t expire;

स्थिर पूर्णाङ्क git_worktree_config(ध्रुव वर्ण *var, ध्रुव वर्ण *मानम्,
			       ध्रुव संरचना config_context *ctx, शून्य *cb)
{
	सचे (!strcmp(var, "worktree.guessremote")) {
		guess_remote = git_config_bool(var, मानम्);
		पटिनिवत्त 0;
	} अञ्ञथा सचे (!strcmp(var, "worktree.userelativepaths")) {
		use_relative_paths = git_config_bool(var, मानम्);
		पटिनिवत्त 0;
	}

	पटिनिवत्त git_default_config(var, मानम्, ctx, cb);
}

स्थिर पूर्णाङ्क delete_git_dir(ध्रुव वर्ण *id)
{
	संरचना strbuf sb = STRBUF_INIT;
	पूर्णाङ्क ret;

	repo_common_path_append(the_repository, &sb, "worktrees/%s", id);
	ret = remove_dir_recursively(&sb, 0);
	सचे (ret < 0 && errno == ENOTDIR)
		ret = unlink(sb.buf);
	सचे (ret)
		error_errno(_("failed to delete '%s'"), sb.buf);
	strbuf_release(&sb);
	पटिनिवत्त ret;
}

स्थिर शून्य delete_worktrees_dir_if_empty(शून्य)
{
	वर्ण *path = repo_git_path(the_repository, "worktrees");
	rmdir(path); /* ignore failed removal */
	मुक्त(path);
}

स्थिर शून्य prune_worktree(ध्रुव वर्ण *id, ध्रुव वर्ण *reason)
{
	सचे (show_only || verbose)
		fprintf_ln(stderr, _("Removing %s/%s: %s"), "worktrees", id, reason);
	सचे (!show_only)
		delete_git_dir(id);
}

स्थिर पूर्णाङ्क prune_cmp(ध्रुव शून्य *a, ध्रुव शून्य *b)
{
	ध्रुव संरचना string_list_item *x = a;
	ध्रुव संरचना string_list_item *y = b;
	पूर्णाङ्क c;

	सचे ((c = fspathcmp(x->string, y->string)))
	    पटिनिवत्त c;
	/*
	 * paths same; prune_dupes() removes all but the first worktree entry
	 * having the same path, so sort main worktree ('util' is NULL) above
	 * linked worktrees ('util' not NULL) since main worktree can't be
	 * removed
	 */
	सचे (!x->util)
		पटिनिवत्त -1;
	सचे (!y->util)
		पटिनिवत्त 1;
	/* paths same; sort by .git/worktrees/<id> */
	पटिनिवत्त strcmp(x->util, y->util);
}

स्थिर शून्य prune_dups(संरचना string_list *l)
{
	पूर्णाङ्क i;

	QSORT(l->वस्तूनि, l->nr, prune_cmp);
	अत्थाय (i = 1; i < l->nr; i++) {
		सचे (!fspathcmp(l->वस्तूनि[i].string, l->वस्तूनि[i - 1].string))
			prune_worktree(l->वस्तूनि[i].util, "duplicate entry");
	}
}

स्थिर शून्य prune_worktrees(शून्य)
{
	संरचना strbuf reason = STRBUF_INIT;
	संरचना strbuf main_path = STRBUF_INIT;
	संरचना string_list kept = STRING_LIST_INIT_DUP;
	वर्ण *path;
	DIR *dir;
	संरचना dirent *d;

	path = repo_git_path(the_repository, "worktrees");
	dir = opendir(path);
	मुक्त(path);
	सचे (!dir)
		पटिनिवत्त;
	याव ((d = readdir_skip_dot_and_dotdot(dir)) != रिक्त) {
		वर्ण *path;
		strbuf_reset(&reason);
		सचे (should_prune_worktree(the_repository, d->d_name, &reason, &path, expire))
			prune_worktree(d->d_name, reason.buf);
		अञ्ञथा सचे (path)
			string_list_append_nodup(&kept, path)->util = xstrdup(d->d_name);
	}
	closedir(dir);

	strbuf_add_absolute_path(&main_path, repo_get_common_dir(the_repository));
	/* massage main worktree absolute path to match 'gitdir' content */
	strbuf_strip_suffix(&main_path, "/.");
	string_list_append_nodup(&kept, strbuf_detach(&main_path, रिक्त));
	prune_dups(&kept);
	string_list_clear(&kept, 1);

	सचे (!show_only)
		delete_worktrees_dir_if_empty();
	strbuf_release(&reason);
}

स्थिर पूर्णाङ्क prune(पूर्णाङ्क ac, ध्रुव वर्ण **av, ध्रुव वर्ण *prefix,
		 संरचना repository *repo UNUSED)
{
	संरचना option options[] = {
		OPT__DRY_RUN(&show_only, N_("do not remove, show only")),
		OPT__VERBOSE(&verbose, N_("report pruned working trees")),
		OPT_EXPIRY_DATE(0, "expire", &expire,
				N_("prune missing working trees older than <time>")),
		OPT_END()
	};

	expire = TIME_MAX;
	ac = parse_options(ac, av, prefix, options, git_worktree_prune_usage,
			   0);
	सचे (ac)
		usage_with_options(git_worktree_prune_usage, options);
	prune_worktrees();
	पटिनिवत्त 0;
}

स्थिर वर्ण *junk_work_वृक्षः;
स्थिर वर्ण *junk_git_dir;
स्थिर पूर्णाङ्क is_junk;
स्थिर pid_t junk_pid;

स्थिर शून्य remove_junk(शून्य)
{
	संरचना strbuf sb = STRBUF_INIT;
	सचे (!is_junk || getpid() != junk_pid)
		पटिनिवत्त;
	सचे (junk_git_dir) {
		strbuf_addstr(&sb, junk_git_dir);
		remove_dir_recursively(&sb, 0);
		strbuf_reset(&sb);
	}
	सचे (junk_work_वृक्षः) {
		strbuf_addstr(&sb, junk_work_वृक्षः);
		remove_dir_recursively(&sb, 0);
	}
	strbuf_release(&sb);
}

स्थिर शून्य remove_junk_on_signal(पूर्णाङ्क signo)
{
	remove_junk();
	sigchain_निष्कासय(signo);
	उत्थापय(signo);
}

स्थिर वर्ण *worktree_basename_dup(ध्रुव वर्ण *path)
{
	ध्रुव वर्ण *name;
	पूर्णाङ्क len;

	len = strlen(path);
	याव (len && is_dir_sep(path[len - 1]))
		len--;

	name = path + len;
	याव (name > path && !is_dir_sep(name[-1]))
		name--;

	पटिनिवत्त xmemdupz(name, path + len - name);
}

/* check that path is viable location for worktree */
स्थिर शून्य check_candidate_path(ध्रुव वर्ण *path,
				 पूर्णाङ्क force,
				 संरचना worktree **worktrees,
				 ध्रुव वर्ण *cmd)
{
	संरचना worktree *wt;
	पूर्णाङ्क locked;

	सचे (file_exists(path) && !is_empty_dir(path))
		die(_("'%s' already exists"), path);

	wt = find_worktree_by_path(worktrees, path);
	सचे (!wt)
		पटिनिवत्त;

	locked = !!worktree_lock_reason(wt);
	सचे ((!locked && force) || (locked && force > 1)) {
		सचे (delete_git_dir(wt->id))
		    die(_("unusable worktree destination '%s'"), path);
		पटिनिवत्त;
	}

	सचे (locked)
		die(_("'%s' is a missing but locked worktree;\nuse '%s -f -f' to override, or 'unlock' and 'prune' or 'remove' to clear"), path, cmd);
	अञ्ञथा
		die(_("'%s' is a missing but already registered worktree;\nuse '%s -f' to override, or 'prune' or 'remove' to clear"), path, cmd);
}

स्थिर शून्य copy_sparse_checkout(ध्रुव वर्ण *worktree_git_dir)
{
	वर्ण *from_file = repo_git_path(the_repository, "info/sparse-checkout");
	वर्ण *to_file = xstrfmt("%s/info/sparse-checkout", worktree_git_dir);

	सचे (file_exists(from_file)) {
		सचे (safe_create_leading_directories(the_repository, to_file) ||
			copy_file(the_repository, to_file, from_file, 0666))
			error(_("failed to copy '%s' to '%s'; sparse-checkout may not work correctly"),
				from_file, to_file);
	}

	मुक्त(from_file);
	मुक्त(to_file);
}

स्थिर शून्य copy_filtered_worktree_config(ध्रुव वर्ण *worktree_git_dir)
{
	वर्ण *from_file = repo_git_path(the_repository, "config.worktree");
	वर्ण *to_file = xstrfmt("%s/config.worktree", worktree_git_dir);

	सचे (file_exists(from_file)) {
		संरचना config_set cs = { { 0 } };
		पूर्णाङ्क bare;

		सचे (safe_create_leading_directories(the_repository, to_file) ||
			copy_file(the_repository, to_file, from_file, 0666)) {
			error(_("failed to copy worktree config from '%s' to '%s'"),
				from_file, to_file);
			गच्छ worktree_copy_cleanup;
		}

		git_configset_init(&cs);
		git_configset_add_file(&cs, from_file);

		सचे (!git_configset_get_bool(&cs, "core.bare", &bare) &&
			bare &&
			repo_config_set_multivar_in_file_gently(the_repository,
				to_file, "core.bare", रिक्त, "true", रिक्त, 0))
			error(_("failed to unset '%s' in '%s'"),
				"core.bare", to_file);
		सचे (!git_configset_get(&cs, "core.worktree") &&
			repo_config_set_in_file_gently(the_repository, to_file,
						       "core.worktree", रिक्त, रिक्त))
			error(_("failed to unset '%s' in '%s'"),
				"core.worktree", to_file);

		git_configset_clear(&cs);
	}

worktree_copy_cleanup:
	मुक्त(from_file);
	मुक्त(to_file);
}

स्थिर पूर्णाङ्क checkout_worktree(ध्रुव संरचना add_opts *opts,
			     संरचना strvec *child_env)
{
	संरचना child_process cp = CHILD_PROCESS_INIT;
	cp.git_cmd = 1;
	strvec_pushl(&cp.args, "reset", "--hard", "--no-recurse-submodules", रिक्त);
	सचे (opts->quiet)
		strvec_प्रक्षिप(&cp.args, "--quiet");
	strvec_pushv(&cp.env, child_env->v);
	पटिनिवत्त run_command(&cp);
}

स्थिर पूर्णाङ्क make_worktree_orphan(ध्रुव वर्ण * ref, ध्रुव संरचना add_opts *opts,
				संरचना strvec *child_env)
{
	संरचना strbuf symref = STRBUF_INIT;
	संरचना child_process cp = CHILD_PROCESS_INIT;

	validate_new_branchname(ref, &symref, 0);
	strvec_pushl(&cp.args, "symbolic-ref", "HEAD", symref.buf, रिक्त);
	सचे (opts->quiet)
		strvec_प्रक्षिप(&cp.args, "--quiet");
	strvec_pushv(&cp.env, child_env->v);
	strbuf_release(&symref);
	cp.git_cmd = 1;
	पटिनिवत्त run_command(&cp);
}

/*
 * References for worktrees are generally stored in '$GIT_DIR/worktrees/<wt_id>'.
 * But when using alternate reference directories, we want to store the worktree
 * references in '$ALTERNATE_REFERENCE_DIR/worktrees/<wt_id>'.
 *
 * Create the necessary folder structure to facilitate the same. But to ensure
 * that the former path is still considered a Git directory, add stubs.
 */
स्थिर शून्य setup_alternate_ref_dir(संरचना worktree *wt, ध्रुव वर्ण *wt_git_path)
{
	संरचना strbuf sb = STRBUF_INIT;
	वर्ण *path;

	path = wt->repo->ref_storage_payload;
	सचे (!path)
		पटिनिवत्त;

	सचे (!is_absolute_path(path))
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

स्थिर पूर्णाङ्क add_worktree(ध्रुव वर्ण *path, ध्रुव वर्ण *refname,
			ध्रुव संरचना add_opts *opts)
{
	संरचना strbuf sb_git = STRBUF_INIT, sb_repo = STRBUF_INIT;
	संरचना strbuf sb = STRBUF_INIT;
	ध्रुव वर्ण *name;
	वर्ण *name_to_free = रिक्त;
	संरचना strvec child_env = STRVEC_INIT;
	unsigned पूर्णाङ्क counter = 0;
	पूर्णाङ्क len, ret;
	संरचना strbuf symref = STRBUF_INIT;
	संरचना commit *commit = रिक्त;
	पूर्णाङ्क is_branch = 0;
	संरचना strbuf sb_name = STRBUF_INIT;
	संरचना worktree **worktrees, *wt = रिक्त;
	संरचना ref_store *wt_refs;
	संरचना repo_config_मानानि *cfg = repo_config_मानानि(the_repository);

	worktrees = get_worktrees(the_repository);
	check_candidate_path(path, opts->force, worktrees, "add");
	free_worktrees(worktrees);
	worktrees = रिक्त;

	/* is 'refname' a branch or commit? */
	सचे (!opts->detach && !check_branch_ref(the_repository, &symref, refname) &&
	    refs_ref_exists(get_main_ref_store(the_repository), symref.buf)) {
		is_branch = 1;
		सचे (!opts->force)
			die_if_checked_out(symref.buf, 0);
	}
	commit = lookup_commit_reference_by_name(refname);
	सचे (!commit && !opts->orphan)
		die(_("invalid reference: %s"), refname);

	name = name_to_free = worktree_basename_dup(path);
	सचे (!*name)
		die(_("invalid path '%s'"), path);
	sanitize_refname_component(name, &sb_name);
	सचे (!sb_name.len)
		BUG("How come '%s' becomes empty after sanitization?", name);
	name = sb_name.buf;
	repo_git_path_replace(the_repository, &sb_repo, "worktrees/%s", name);
	len = sb_repo.len;
	सचे (safe_create_leading_directories_const(the_repository, sb_repo.buf))
		die_errno(_("could not create leading directories of '%s'"),
			  sb_repo.buf);

	याव (mkdir(sb_repo.buf, 0777)) {
		counter++;
		सचे ((errno != EEXIST) || !counter /* overflow */)
			die_errno(_("could not create directory of '%s'"),
				  sb_repo.buf);
		strbuf_setlen(&sb_repo, len);
		strbuf_addf(&sb_repo, "%d", counter);
	}
	name = strrchr(sb_repo.buf, '/') + 1;

	junk_pid = getpid();
	atexit(remove_junk);
	sigchain_प्रक्षिप_common(remove_junk_on_signal);

	junk_git_dir = xstrdup(sb_repo.buf);
	is_junk = 1;

	/*
	 * lock the incomplete repo so prune won't delete it, unlock
	 * after the preparation is over.
	 */
	strbuf_addf(&sb, "%s/locked", sb_repo.buf);
	सचे (opts->keep_locked)
		write_file(sb.buf, "%s", opts->keep_locked);
	अञ्ञथा
		write_file(sb.buf, _("initializing"));

	strbuf_addf(&sb_git, "%s/.git", path);
	सचे (safe_create_leading_directories_const(the_repository, sb_git.buf))
		die_errno(_("could not create leading directories of '%s'"),
			  sb_git.buf);
	junk_work_वृक्षः = xstrdup(path);

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
	सचे (!wt) {
		ret = error(_("could not find created worktree '%s'"), name);
		गच्छ done;
	}
	setup_alternate_ref_dir(wt, sb_repo.buf);
	wt_refs = get_worktree_ref_store(wt);

	ret = ref_store_create_on_disk(wt_refs, REF_STORE_CREATE_ON_DISK_IS_WORKTREE, &sb);
	सचे (ret)
		गच्छ done;

	सचे (!is_branch && commit)
		ret = refs_update_ref(wt_refs, रिक्त, "HEAD", &commit->object.oid,
				      रिक्त, 0, UPDATE_REFS_सन्देशः_ON_ERR);
	अञ्ञथा
		ret = refs_update_symref(wt_refs, "HEAD", symref.buf, रिक्त);
	सचे (ret)
		गच्छ done;

	/*
	 * If the current worktree has sparse-checkout enabled, then copy
	 * the sparse-checkout patterns from the current worktree.
	 */
	सचे (cfg->apply_sparse_checkout)
		copy_sparse_checkout(sb_repo.buf);

	/*
	 * If we are using worktree config, then copy all current config
	 * values from the current worktree into the new one, that way the
	 * new worktree behaves the same as this one.
	 */
	सचे (the_repository->repository_format_worktree_config)
		copy_filtered_worktree_config(sb_repo.buf);

	strvec_pushf(&child_env, "%s=%s", GIT_DIR_ENVIRONMENT, sb_git.buf);
	strvec_pushf(&child_env, "%s=%s", GIT_WORK_वृक्षः_ENVIRONMENT, path);

	सचे (opts->orphan &&
	    (ret = make_worktree_orphan(refname, opts, &child_env)))
		गच्छ done;

	सचे (opts->checkout &&
	    (ret = checkout_worktree(opts, &child_env)))
		गच्छ done;

	is_junk = 0;
	FREE_AND_NULL(junk_work_वृक्षः);
	FREE_AND_NULL(junk_git_dir);

done:
	सचे (ret || !opts->keep_locked) {
		strbuf_reset(&sb);
		strbuf_addf(&sb, "%s/locked", sb_repo.buf);
		unlink_or_warn(sb.buf);
	}

	/*
	 * Hook failure does not warrant worktree deletion, so run hook after
	 * is_junk is cleared, but do return appropriate code when hook fails.
	 */
	सचे (!ret && opts->checkout && !opts->orphan) {
		संरचना run_hooks_opt opt = RUN_HOOKS_OPT_INIT_FORCE_SERIAL;

		strvec_pushl(&opt.env, "GIT_DIR", "GIT_WORK_TREE", रिक्त);
		strvec_pushl(&opt.args,
			     oid_to_hex(null_oid(the_hash_algo)),
			     oid_to_hex(&commit->object.oid),
			     "1",
			     रिक्त);
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
	पटिनिवत्त ret;
}

स्थिर शून्य print_preparing_worktree_पङ्क्तिः(पूर्णाङ्क detach,
					  ध्रुव वर्ण *branch,
					  ध्रुव वर्ण *new_branch,
					  पूर्णाङ्क force_new_branch)
{
	सचे (force_new_branch) {
		संरचना commit *commit = lookup_commit_reference_by_name(new_branch);
		सचे (!commit)
			fprintf_ln(stderr, _("Preparing worktree (new branch '%s')"), new_branch);
		अञ्ञथा
			fprintf_ln(stderr, _("Preparing worktree (resetting branch '%s'; was at %s)"),
				  new_branch,
				  repo_find_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV));
	} अञ्ञथा सचे (new_branch) {
		fprintf_ln(stderr, _("Preparing worktree (new branch '%s')"), new_branch);
	} अञ्ञथा {
		संरचना strbuf s = STRBUF_INIT;
		सचे (!detach && !check_branch_ref(the_repository, &s, branch) &&
		    refs_ref_exists(get_main_ref_store(the_repository), s.buf))
			fprintf_ln(stderr, _("Preparing worktree (checking out '%s')"),
				  branch);
		अञ्ञथा {
			संरचना commit *commit = lookup_commit_reference_by_name(branch);
			सचे (!commit)
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
स्थिर पूर्णाङ्क first_valid_ref(ध्रुव संरचना reference *ref UNUSED, शून्य *cb_data UNUSED)
{
	पटिनिवत्त 1;
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
स्थिर पूर्णाङ्क can_use_local_refs(ध्रुव संरचना add_opts *opts)
{
	सचे (refs_head_ref(get_main_ref_store(the_repository), first_valid_ref, रिक्त)) {
		पटिनिवत्त 1;
	} अञ्ञथा सचे (refs_for_each_branch_ref(get_main_ref_store(the_repository), first_valid_ref, रिक्त)) {
		सचे (!opts->quiet)
			warning(_("HEAD points to an invalid (or orphaned) reference.\n"));
		पटिनिवत्त 1;
	}
	पटिनिवत्त 0;
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
स्थिर पूर्णाङ्क can_use_remote_refs(ध्रुव संरचना add_opts *opts)
{
	सचे (!guess_remote) {
		पटिनिवत्त 0;
	} अञ्ञथा सचे (refs_for_each_remote_ref(get_main_ref_store(the_repository), first_valid_ref, रिक्त)) {
		पटिनिवत्त 1;
	} अञ्ञथा सचे (!opts->force && remote_get(रिक्त)) {
		die(_("No local or remote refs exist despite at least one remote\n"
		      "present, stopping; use 'add -f' to override or fetch a remote first"));
	}
	पटिनिवत्त 0;
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
स्थिर पूर्णाङ्क dwim_orphan(ध्रुव संरचना add_opts *opts, पूर्णाङ्क opt_track, पूर्णाङ्क remote)
{
	सचे (can_use_local_refs(opts)) {
		पटिनिवत्त 0;
	} अञ्ञथा सचे (remote && can_use_remote_refs(opts)) {
		पटिनिवत्त 0;
	} अञ्ञथा सचे (!opts->quiet) {
		fprintf_ln(stderr, WORKTREE_ADD_DWIM_ORPHAN_INFER_TEXT);
	}

	सचे (opt_track) {
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--track");
	} अञ्ञथा सचे (!opts->checkout) {
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--no-checkout");
	}
	पटिनिवत्त 1;
}

स्थिर शून्य advise_disambiguating_remotes(ध्रुव वर्ण *path, ध्रुव वर्ण *branch,
					  ध्रुव संरचना string_list *matched_remote_names)
{
	संरचना string_list_item *item;

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

स्थिर वर्ण *dwim_branch(ध्रुव संरचना add_opts *opts, ध्रुव वर्ण *path, वर्ण **new_branch)
{
	पूर्णाङ्क branch_exists;
	वर्ण *branchname = worktree_basename_dup(path);
	संरचना strbuf ref = STRBUF_INIT;

	branch_exists = !check_branch_ref(the_repository, &ref, branchname) &&
			refs_ref_exists(get_main_ref_store(the_repository),
					ref.buf);
	strbuf_release(&ref);
	सचे (branch_exists)
		पटिनिवत्त branchname;

	*new_branch = branchname;
	सचे (guess_remote) {
		संरचना object_id oid;
		वर्ण *remote;
		पूर्णाङ्क अङ्कः_matches = 0;
		संरचना string_list matched_remote_names = STRING_LIST_INIT_DUP;

		remote = unique_tracking_name(*new_branch, &oid, &अङ्कः_matches,
					      &matched_remote_names);
		सचे (!remote && अङ्कः_matches > 1) {
			सचे (!opts->quiet &&
			    advice_enabled(ADVICE_CHECKOUT_AMBIGUOUS_REMOTE_BRANCH_NAME))
				advise_disambiguating_remotes(path, *new_branch,
							      &matched_remote_names);
			die(_("'%s' matched multiple (%d) remote tracking branches"),
			    *new_branch, अङ्कः_matches);
		}
		string_list_clear(&matched_remote_names, 0);
		पटिनिवत्त remote;
	}
	पटिनिवत्त रिक्त;
}

स्थिर पूर्णाङ्क add(पूर्णाङ्क ac, ध्रुव वर्ण **av, ध्रुव वर्ण *prefix,
	       संरचना repository *repo UNUSED)
{
	संरचना add_opts opts;
	ध्रुव वर्ण *new_branch_force = रिक्त;
	वर्ण *path;
	ध्रुव वर्ण *branch;
	वर्ण *branch_to_free = रिक्त;
	वर्ण *new_branch_to_free = रिक्त;
	ध्रुव वर्ण *new_branch = रिक्त;
	वर्ण *opt_track = रिक्त;
	ध्रुव वर्ण *lock_reason = रिक्त;
	पूर्णाङ्क keep_locked = 0;
	पूर्णाङ्क used_new_branch_options;
	संरचना option options[] = {
		OPT__FORCE(&opts.force,
			   N_("checkout <branch> even if already checked out in other worktree"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_STRING('b', रिक्त, &new_branch, N_("branch"),
			   N_("create a new branch")),
		OPT_STRING('B', रिक्त, &new_branch_force, N_("branch"),
			   N_("create or reset a branch")),
		OPT_BOOL(0, "orphan", &opts.orphan, N_("create unborn branch")),
		OPT_BOOL('d', "detach", &opts.detach, N_("detach HEAD at named commit")),
		OPT_BOOL(0, "checkout", &opts.checkout, N_("populate the new working tree")),
		OPT_BOOL(0, "lock", &keep_locked, N_("keep the new working tree locked")),
		OPT_STRING(0, "reason", &lock_reason, N_("string"),
			   N_("reason for locking")),
		OPT__QUIET(&opts.quiet, N_("suppress progress reporting")),
		OPT_PASSTHRU(0, "track", &opt_track, रिक्त,
			     N_("set up tracking mode (see git-branch(1))"),
			     PARSE_OPT_NOARG | PARSE_OPT_OPTARG),
		OPT_BOOL(0, "guess-remote", &guess_remote,
			 N_("try to match the new branch name with a remote-tracking branch")),
		OPT_BOOL(0, "relative-paths", &opts.relative_paths,
			 N_("use relative paths for worktrees")),
		OPT_END()
	};
	पूर्णाङ्क ret;

	memset(&opts, 0, परिमाण(opts));
	opts.checkout = 1;
	opts.relative_paths = use_relative_paths;
	ac = parse_options(ac, av, prefix, options, git_worktree_add_usage, 0);
	सचे (!!opts.detach + !!new_branch + !!new_branch_force > 1)
		die(_("options '%s', '%s', and '%s' cannot be used together"), "-b", "-B", "--detach");
	सचे (opts.detach && opts.orphan)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--detach");
	सचे (opts.orphan && opt_track)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--track");
	सचे (opts.orphan && !opts.checkout)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--no-checkout");
	सचे (opts.orphan && ac == 2)
		die(_("option '%s' and commit-ish cannot be used together"),
		    "--orphan");
	सचे (lock_reason && !keep_locked)
		die(_("the option '%s' requires '%s'"), "--reason", "--lock");
	सचे (lock_reason)
		opts.keep_locked = lock_reason;
	अञ्ञथा सचे (keep_locked)
		opts.keep_locked = _("added with --lock");

	सचे (ac < 1 || ac > 2)
		usage_with_options(git_worktree_add_usage, options);

	path = prefix_filename(prefix, av[0]);
	branch = ac < 2 ? "HEAD" : av[1];
	used_new_branch_options = new_branch || new_branch_force;

	सचे (!strcmp(branch, "-"))
		branch = "@{-1}";

	सचे (new_branch_force) {
		संरचना strbuf symref = STRBUF_INIT;

		new_branch = new_branch_force;

		सचे (!opts.force &&
		    !check_branch_ref(the_repository, &symref, new_branch) &&
		    refs_ref_exists(get_main_ref_store(the_repository), symref.buf))
			die_if_checked_out(symref.buf, 0);
		strbuf_release(&symref);
	}

	सचे (opts.orphan && !new_branch) {
		new_branch = new_branch_to_free = worktree_basename_dup(path);
	} अञ्ञथा सचे (opts.orphan) {
		; /* no-op */
	} अञ्ञथा सचे (opts.detach) {
		/* Check HEAD */
		सचे (!strcmp(branch, "HEAD"))
			can_use_local_refs(&opts);
	} अञ्ञथा सचे (ac < 2 && new_branch) {
		/* DWIM: Infer --orphan when repo has no refs. */
		opts.orphan = dwim_orphan(&opts, !!opt_track, 0);
	} अञ्ञथा सचे (ac < 2) {
		/* DWIM: Guess branch name from path. */
		वर्ण *s = dwim_branch(&opts, path, &new_branch_to_free);
		सचे (s)
			branch = branch_to_free = s;
		new_branch = new_branch_to_free;

		/* DWIM: Infer --orphan when repo has no refs. */
		opts.orphan = (!s) && dwim_orphan(&opts, !!opt_track, 1);
	} अञ्ञथा सचे (ac == 2 && new_branch) {
		सचे (!strcmp(branch, "HEAD"))
			can_use_local_refs(&opts);
	} अञ्ञथा सचे (ac == 2) {
		संरचना commit *commit;

		commit = lookup_commit_reference_by_name(branch);
		सचे (!commit) {
			संरचना object_id oid;
			वर्ण *remote;
			पूर्णाङ्क अङ्कः_matches = 0;
			संरचना string_list matched_remote_names = STRING_LIST_INIT_DUP;

			remote = unique_tracking_name(branch, &oid, &अङ्कः_matches,
						      &matched_remote_names);
			सचे (remote) {
				new_branch = branch;
				branch = new_branch_to_free = remote;
			} अञ्ञथा सचे (अङ्कः_matches > 1) {
				सचे (!opts.quiet &&
				    advice_enabled(ADVICE_CHECKOUT_AMBIGUOUS_REMOTE_BRANCH_NAME))
					advise_disambiguating_remotes(path, branch,
								      &matched_remote_names);
				die(_("'%s' matched multiple (%d) remote tracking branches"),
				    branch, अङ्कः_matches);
			}
			string_list_clear(&matched_remote_names, 0);
		}

		सचे (!strcmp(branch, "HEAD"))
			can_use_local_refs(&opts);

	}

	सचे (!opts.orphan && !lookup_commit_reference_by_name(branch)) {
		पूर्णाङ्क attempt_hint = !opts.quiet && (ac < 2);
		सचे (attempt_hint && used_new_branch_options) {
			advise_if_enabled(ADVICE_WORKTREE_ADD_ORPHAN,
				WORKTREE_ADD_ORPHAN_WITH_DASH_B_HINT_TEXT,
				new_branch, path);
		} अञ्ञथा सचे (attempt_hint) {
			advise_if_enabled(ADVICE_WORKTREE_ADD_ORPHAN,
				WORKTREE_ADD_ORPHAN_NO_DASH_B_HINT_TEXT, path);
		}
		die(_("invalid reference: %s"), branch);
	}

	सचे (!opts.quiet)
		print_preparing_worktree_पङ्क्तिः(opts.detach, branch, new_branch, !!new_branch_force);

	सचे (opts.orphan) {
		branch = new_branch;
	} अञ्ञथा सचे (new_branch) {
		संरचना child_process cp = CHILD_PROCESS_INIT;
		cp.git_cmd = 1;
		strvec_प्रक्षिप(&cp.args, "branch");
		सचे (new_branch_force)
			strvec_प्रक्षिप(&cp.args, "--force");
		सचे (opts.quiet)
			strvec_प्रक्षिप(&cp.args, "--quiet");
		strvec_प्रक्षिप(&cp.args, new_branch);
		strvec_प्रक्षिप(&cp.args, branch);
		सचे (opt_track)
			strvec_प्रक्षिप(&cp.args, opt_track);
		सचे (run_command(&cp)) {
			ret = -1;
			गच्छ cleanup;
		}
		branch = new_branch;
	} अञ्ञथा सचे (opt_track) {
		die(_("--[no-]track can only be used if a new branch is created"));
	}

	ret = add_worktree(path, branch, &opts);
cleanup:
	मुक्त(path);
	मुक्त(opt_track);
	मुक्त(branch_to_free);
	मुक्त(new_branch_to_free);
	पटिनिवत्त ret;
}

स्थिर शून्य show_worktree_porcelain(संरचना worktree *wt, पूर्णाङ्क पङ्क्तिः_terminator)
{
	ध्रुव वर्ण *reason;

	printf("worktree %s%c", wt->path, पङ्क्तिः_terminator);
	सचे (wt->is_bare)
		printf("bare%c", पङ्क्तिः_terminator);
	अञ्ञथा {
		printf("HEAD %s%c", oid_to_hex(&wt->head_oid), पङ्क्तिः_terminator);
		सचे (wt->is_detached)
			printf("detached%c", पङ्क्तिः_terminator);
		अञ्ञथा सचे (wt->head_ref)
			printf("branch %s%c", wt->head_ref, पङ्क्तिः_terminator);
	}

	reason = worktree_lock_reason(wt);
	सचे (reason) {
		fputs("locked", stdout);
		सचे (*reason) {
			fputc(' ', stdout);
			write_name_quoted(reason, stdout, पङ्क्तिः_terminator);
		} अञ्ञथा {
			fputc(पङ्क्तिः_terminator, stdout);
		}
	}

	reason = worktree_prune_reason(wt, expire);
	सचे (reason)
		printf("prunable %s%c", reason, पङ्क्तिः_terminator);

	fputc(पङ्क्तिः_terminator, stdout);
}

संरचना worktree_display {
	वर्ण *path;
	पूर्णाङ्क width;
};

स्थिर शून्य show_worktree(संरचना worktree *wt, संरचना worktree_display *display,
			  पूर्णाङ्क path_maxwidth, पूर्णाङ्क abbrev_len)
{
	संरचना strbuf sb = STRBUF_INIT;
	ध्रुव वर्ण *reason;

	strbuf_addf(&sb, "%s%*s", display->path, 1 + path_maxwidth - display->width, "");
	सचे (wt->is_bare)
		strbuf_addstr(&sb, "(bare)");
	अञ्ञथा {
		strbuf_addf(&sb, "%-*s ", abbrev_len,
				repo_find_unique_abbrev(the_repository, &wt->head_oid, DEFAULT_ABBREV));
		सचे (wt->is_detached)
			strbuf_addstr(&sb, "(detached HEAD)");
		अञ्ञथा सचे (wt->head_ref) {
			वर्ण *ref = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
								 wt->head_ref,
								 0);
			strbuf_addf(&sb, "[%s]", ref);
			मुक्त(ref);
		} अञ्ञथा
			strbuf_addstr(&sb, "(error)");
	}

	reason = worktree_lock_reason(wt);
	सचे (verbose && reason && *reason)
		strbuf_addf(&sb, "\n\tlocked: %s", reason);
	अञ्ञथा सचे (reason)
		strbuf_addstr(&sb, " locked");

	reason = worktree_prune_reason(wt, expire);
	सचे (verbose && reason)
		strbuf_addf(&sb, "\n\tprunable: %s", reason);
	अञ्ञथा सचे (reason)
		strbuf_addstr(&sb, " prunable");

	printf("%s\n", sb.buf);
	strbuf_release(&sb);
}

स्थिर शून्य measure_widths(संरचना worktree **wt, पूर्णाङ्क *abbrev,
			   संरचना worktree_display **d, पूर्णाङ्क *maxwidth)
{
	पूर्णाङ्क i, display_alloc = 0;
	संरचना worktree_display *display = रिक्त;
	संरचना strbuf buf = STRBUF_INIT;

	अत्थाय (i = 0; wt[i]; i++) {
		पूर्णाङ्क sha1_len;
		ALLOC_GROW(display, i + 1, display_alloc);
		quote_path(wt[i]->path, रिक्त, &buf, 0);
		display[i].width = utf8_strwidth(buf.buf);
		display[i].path = strbuf_detach(&buf, रिक्त);

		सचे (display[i].width > *maxwidth)
			*maxwidth = display[i].width;
		sha1_len = strlen(repo_find_unique_abbrev(the_repository, &wt[i]->head_oid, *abbrev));
		सचे (sha1_len > *abbrev)
			*abbrev = sha1_len;
	}
	*d = display;
}

स्थिर पूर्णाङ्क pathcmp(ध्रुव शून्य *a_, ध्रुव शून्य *b_)
{
	ध्रुव संरचना worktree *ध्रुव *a = a_;
	ध्रुव संरचना worktree *ध्रुव *b = b_;
	पटिनिवत्त fspathcmp((*a)->path, (*b)->path);
}

स्थिर शून्य pathsort(संरचना worktree **wt)
{
	पूर्णाङ्क n = 0;
	संरचना worktree **p = wt;

	याव (*p++)
		n++;
	QSORT(wt, n, pathcmp);
}

स्थिर पूर्णाङ्क list(पूर्णाङ्क ac, ध्रुव वर्ण **av, ध्रुव वर्ण *prefix,
		संरचना repository *repo UNUSED)
{
	पूर्णाङ्क porcelain = 0;
	पूर्णाङ्क पङ्क्तिः_terminator = '\n';

	संरचना option options[] = {
		OPT_BOOL(0, "porcelain", &porcelain, N_("machine-readable output")),
		OPT__VERBOSE(&verbose, N_("show extended annotations and reasons, if available")),
		OPT_EXPIRY_DATE(0, "expire", &expire,
				N_("add 'prunable' annotation to missing worktrees older than <time>")),
		OPT_SET_INT('z', रिक्त, &पङ्क्तिः_terminator,
			    N_("terminate records with a NUL character"), '\0'),
		OPT_END()
	};

	expire = TIME_MAX;
	ac = parse_options(ac, av, prefix, options, git_worktree_list_usage, 0);
	सचे (ac)
		usage_with_options(git_worktree_list_usage, options);
	अञ्ञथा सचे (verbose && porcelain)
		die(_("options '%s' and '%s' cannot be used together"), "--verbose", "--porcelain");
	अञ्ञथा सचे (!पङ्क्तिः_terminator && !porcelain)
		die(_("the option '%s' requires '%s'"), "-z", "--porcelain");
	अञ्ञथा {
		संरचना worktree **worktrees = get_worktrees(the_repository);
		पूर्णाङ्क path_maxwidth = 0, abbrev = DEFAULT_ABBREV, i;
		संरचना worktree_display *display = रिक्त;

		/* sort worktrees by path but keep main worktree at top */
		pathsort(worktrees + 1);

		सचे (!porcelain)
			measure_widths(worktrees, &abbrev,
				       &display, &path_maxwidth);

		अत्थाय (i = 0; worktrees[i]; i++) {
			सचे (porcelain)
				show_worktree_porcelain(worktrees[i],
							पङ्क्तिः_terminator);
			अञ्ञथा
				show_worktree(worktrees[i],
					      &display[i], path_maxwidth, abbrev);
		}
		अत्थाय (i = 0; display && worktrees[i]; i++)
			मुक्त(display[i].path);
		मुक्त(display);
		free_worktrees(worktrees);
	}
	पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क lock_worktree(पूर्णाङ्क ac, ध्रुव वर्ण **av, ध्रुव वर्ण *prefix,
			 संरचना repository *repo UNUSED)
{
	ध्रुव वर्ण *reason = "", *old_reason;
	संरचना option options[] = {
		OPT_STRING(0, "reason", &reason, N_("string"),
			   N_("reason for locking")),
		OPT_END()
	};
	संरचना worktree **worktrees, *wt;
	वर्ण *path;

	ac = parse_options(ac, av, prefix, options, git_worktree_lock_usage, 0);
	सचे (ac != 1)
		usage_with_options(git_worktree_lock_usage, options);

	worktrees = get_worktrees(the_repository);
	wt = find_worktree(worktrees, prefix, av[0]);
	सचे (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	सचे (is_main_worktree(wt))
		die(_("The main working tree cannot be locked or unlocked"));

	old_reason = worktree_lock_reason(wt);
	सचे (old_reason) {
		सचे (*old_reason)
			die(_("'%s' is already locked, reason: %s"),
			    av[0], old_reason);
		die(_("'%s' is already locked"), av[0]);
	}

	path = repo_common_path(the_repository, "worktrees/%s/locked", wt->id);
	write_file(path, "%s", reason);

	free_worktrees(worktrees);
	मुक्त(path);
	पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क unlock_worktree(पूर्णाङ्क ac, ध्रुव वर्ण **av, ध्रुव वर्ण *prefix,
			   संरचना repository *repo UNUSED)
{
	संरचना option options[] = {
		OPT_END()
	};
	संरचना worktree **worktrees, *wt;
	वर्ण *path;
	पूर्णाङ्क ret;

	ac = parse_options(ac, av, prefix, options, git_worktree_unlock_usage, 0);
	सचे (ac != 1)
		usage_with_options(git_worktree_unlock_usage, options);

	worktrees = get_worktrees(the_repository);
	wt = find_worktree(worktrees, prefix, av[0]);
	सचे (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	सचे (is_main_worktree(wt))
		die(_("The main working tree cannot be locked or unlocked"));
	सचे (!worktree_lock_reason(wt))
		die(_("'%s' is not locked"), av[0]);

	path = repo_common_path(the_repository, "worktrees/%s/locked", wt->id);
	ret = unlink_or_warn(path);

	free_worktrees(worktrees);
	मुक्त(path);
	पटिनिवत्त ret;
}

स्थिर शून्य validate_no_submodules(ध्रुव संरचना worktree *wt)
{
	संरचना index_state istate = INDEX_STATE_INIT(the_repository);
	संरचना strbuf path = STRBUF_INIT;
	पूर्णाङ्क i, found_submodules = 0;
	वर्ण *wt_gitdir;

	wt_gitdir = get_worktree_git_dir(wt);

	सचे (is_directory(worktree_git_path(wt, "modules"))) {
		/*
		 * There could be false positives, e.g. the "modules"
		 * directory exists but is empty. But it's a rare case and
		 * this simpler check is probably good enough for now.
		 */
		found_submodules = 1;
	} अञ्ञथा सचे (read_index_from(&istate, worktree_git_path(wt, "index"),
				   wt_gitdir) > 0) {
		अत्थाय (i = 0; i < istate.cache_nr; i++) {
			संरचना cache_entry *ce = istate.cache[i];
			पूर्णाङ्क err;

			सचे (!S_ISGITLINK(ce->ce_mode))
				अनुवर्त;

			strbuf_reset(&path);
			strbuf_addf(&path, "%s/%s", wt->path, ce->name);
			सचे (!is_submodule_populated_gently(path.buf, &err))
				अनुवर्त;

			found_submodules = 1;
			भङ्ग;
		}
	}
	discard_index(&istate);
	strbuf_release(&path);
	मुक्त(wt_gitdir);

	सचे (found_submodules)
		die(_("working trees containing submodules cannot be moved or removed"));
}

स्थिर पूर्णाङ्क move_worktree(पूर्णाङ्क ac, ध्रुव वर्ण **av, ध्रुव वर्ण *prefix,
			 संरचना repository *repo UNUSED)
{
	पूर्णाङ्क force = 0;
	संरचना option options[] = {
		OPT__FORCE(&force,
			 N_("force move even if worktree is dirty or locked"),
			 PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "relative-paths", &use_relative_paths,
			 N_("use relative paths for worktrees")),
		OPT_END()
	};
	संरचना worktree **worktrees, *wt;
	संरचना strbuf dst = STRBUF_INIT;
	संरचना strbuf errmsg = STRBUF_INIT;
	ध्रुव वर्ण *reason = रिक्त;
	वर्ण *path;

	ac = parse_options(ac, av, prefix, options, git_worktree_move_usage,
			   0);
	सचे (ac != 2)
		usage_with_options(git_worktree_move_usage, options);

	path = prefix_filename(prefix, av[1]);
	strbuf_addstr(&dst, path);
	मुक्त(path);

	worktrees = get_worktrees(the_repository);
	wt = find_worktree(worktrees, prefix, av[0]);
	सचे (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	सचे (is_main_worktree(wt))
		die(_("'%s' is a main working tree"), av[0]);
	सचे (is_directory(dst.buf)) {
		ध्रुव वर्ण *sep = find_last_dir_sep(wt->path);

		सचे (!sep)
			die(_("could not figure out destination name from '%s'"),
			    wt->path);
		strbuf_trim_trailing_dir_sep(&dst);
		strbuf_addstr(&dst, sep);
	}
	check_candidate_path(dst.buf, force, worktrees, "move");

	validate_no_submodules(wt);

	सचे (force < 2)
		reason = worktree_lock_reason(wt);
	सचे (reason) {
		सचे (*reason)
			die(_("cannot move a locked working tree, lock reason: %s\nuse 'move -f -f' to override or unlock first"),
			    reason);
		die(_("cannot move a locked working tree;\nuse 'move -f -f' to override or unlock first"));
	}
	सचे (validate_worktree(wt, &errmsg, 0))
		die(_("validation failed, cannot move working tree: %s"),
		    errmsg.buf);
	strbuf_release(&errmsg);

	सचे (rename(wt->path, dst.buf) == -1)
		die_errno(_("failed to move '%s' to '%s'"), wt->path, dst.buf);

	update_worktree_location(wt, dst.buf, use_relative_paths);

	strbuf_release(&dst);
	free_worktrees(worktrees);
	पटिनिवत्त 0;
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
स्थिर शून्य check_clean_worktree(संरचना worktree *wt,
				 ध्रुव वर्ण *original_path)
{
	संरचना child_process cp;
	वर्ण buf[1];
	पूर्णाङ्क ret;

	/*
	 * Until we sort this out, all submodules are "dirty" and
	 * will abort this function.
	 */
	validate_no_submodules(wt);

	child_process_init(&cp);
	strvec_pushf(&cp.env, "%s=%s/.git",
		     GIT_DIR_ENVIRONMENT, wt->path);
	strvec_pushf(&cp.env, "%s=%s",
		     GIT_WORK_वृक्षः_ENVIRONMENT, wt->path);
	strvec_pushl(&cp.args, "status",
		     "--porcelain", "--ignore-submodules=none",
		     रिक्त);
	cp.git_cmd = 1;
	cp.dir = wt->path;
	cp.out = -1;
	ret = start_command(&cp);
	सचे (ret)
		die_errno(_("failed to run 'git status' on '%s'"),
			  original_path);
	ret = xread(cp.out, buf, परिमाण(buf));
	सचे (ret)
		die(_("'%s' contains modified or untracked files, use --force to delete it"),
		    original_path);
	close(cp.out);
	ret = finish_command(&cp);
	सचे (ret)
		die_errno(_("failed to run 'git status' on '%s', code %d"),
			  original_path, ret);
}

स्थिर पूर्णाङ्क delete_git_work_वृक्षः(संरचना worktree *wt)
{
	संरचना strbuf sb = STRBUF_INIT;
	पूर्णाङ्क ret = 0;

	strbuf_addstr(&sb, wt->path);
	सचे (remove_dir_recursively(&sb, 0)) {
		error_errno(_("failed to delete '%s'"), sb.buf);
		ret = -1;
	}
	strbuf_release(&sb);
	पटिनिवत्त ret;
}

स्थिर पूर्णाङ्क remove_worktree(पूर्णाङ्क ac, ध्रुव वर्ण **av, ध्रुव वर्ण *prefix,
			   संरचना repository *repo UNUSED)
{
	पूर्णाङ्क force = 0;
	संरचना option options[] = {
		OPT__FORCE(&force,
			 N_("force removal even if worktree is dirty or locked"),
			 PARSE_OPT_NOCOMPLETE),
		OPT_END()
	};
	संरचना worktree **worktrees, *wt;
	संरचना strbuf errmsg = STRBUF_INIT;
	ध्रुव वर्ण *reason = रिक्त;
	पूर्णाङ्क ret = 0;

	ac = parse_options(ac, av, prefix, options, git_worktree_remove_usage, 0);
	सचे (ac != 1)
		usage_with_options(git_worktree_remove_usage, options);

	worktrees = get_worktrees(the_repository);
	wt = find_worktree(worktrees, prefix, av[0]);
	सचे (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	सचे (is_main_worktree(wt))
		die(_("'%s' is a main working tree"), av[0]);
	सचे (force < 2)
		reason = worktree_lock_reason(wt);
	सचे (reason) {
		सचे (*reason)
			die(_("cannot remove a locked working tree, lock reason: %s\nuse 'remove -f -f' to override or unlock first"),
			    reason);
		die(_("cannot remove a locked working tree;\nuse 'remove -f -f' to override or unlock first"));
	}
	सचे (validate_worktree(wt, &errmsg, WT_VALIDATE_WORKTREE_MISSING_OK))
		die(_("validation failed, cannot remove working tree: %s"),
		    errmsg.buf);
	strbuf_release(&errmsg);

	सचे (file_exists(wt->path)) {
		सचे (!force)
			check_clean_worktree(wt, av[0]);

		ret |= delete_git_work_वृक्षः(wt);
	}
	/*
	 * continue on even if ret is non-zero, there's no going back
	 * from here.
	 */
	ret |= delete_git_dir(wt->id);
	delete_worktrees_dir_if_empty();

	free_worktrees(worktrees);
	पटिनिवत्त ret;
}

स्थिर शून्य report_repair(पूर्णाङ्क iserr, ध्रुव वर्ण *path, ध्रुव वर्ण *सन्देशः, शून्य *cb_data)
{
	सचे (!iserr) {
		fprintf_ln(stderr, _("repair: %s: %s"), सन्देशः, path);
	} अञ्ञथा {
		पूर्णाङ्क *exit_status = (पूर्णाङ्क *)cb_data;
		fprintf_ln(stderr, _("error: %s: %s"), सन्देशः, path);
		*exit_status = 1;
	}
}

स्थिर पूर्णाङ्क repair(पूर्णाङ्क ac, ध्रुव वर्ण **av, ध्रुव वर्ण *prefix,
		  संरचना repository *repo UNUSED)
{
	ध्रुव वर्ण **p;
	ध्रुव वर्ण *self[] = { ".", रिक्त };
	संरचना option options[] = {
		OPT_BOOL(0, "relative-paths", &use_relative_paths,
			 N_("use relative paths for worktrees")),
		OPT_END()
	};
	पूर्णाङ्क rc = 0;

	ac = parse_options(ac, av, prefix, options, git_worktree_repair_usage, 0);
	p = ac > 0 ? av : self;
	अत्थाय (; *p; p++)
		repair_worktree_at_path(the_repository, *p, report_repair,
					&rc, use_relative_paths);
	repair_worktrees(the_repository, report_repair, &rc, use_relative_paths);
	पटिनिवत्त rc;
}

पूर्णाङ्क cmd_worktree(पूर्णाङ्क ac,
		 ध्रुव वर्ण **av,
		 ध्रुव वर्ण *prefix,
		 संरचना repository *repo)
{
	parse_opt_subcommand_fn *fn = रिक्त;
	संरचना option options[] = {
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

	repo_config(the_repository, git_worktree_config, रिक्त);

	सचे (!prefix)
		prefix = "";

	ac = parse_options(ac, av, prefix, options, git_worktree_usage, 0);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	पटिनिवत्त fn(ac, av, prefix, repo);
}
