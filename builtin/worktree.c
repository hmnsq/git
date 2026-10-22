#परिभाषय USE_THE_REPOSITORY_VARIABLE
#परिभाषय DISABLE_SIGN_COMPARE_WARNINGS

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

#परिभाषय BUILTIN_WORKTREE_सङ्कलय_USAGE \
	N_("git worktree add [-f] [--detach] [--checkout] [--lock [--reason <string>]]\n" \
	   "                 [--orphan] [(-b | -B) <new-branch>] <path> [<commit-ish>]")

#परिभाषय BUILTIN_WORKTREE_LIST_USAGE \
	N_("git worktree list [-v | --porcelain [-z]]")
#परिभाषय BUILTIN_WORKTREE_तालकम्_USAGE \
	N_("git worktree lock [--reason <string>] <worktree>")
#परिभाषय BUILTIN_WORKTREE_MOVE_USAGE \
	N_("git worktree move <worktree> <new-path>")
#परिभाषय BUILTIN_WORKTREE_PRUNE_USAGE \
	N_("git worktree prune [-n] [-v] [--expire <expire>]")
#परिभाषय BUILTIN_WORKTREE_REMOVE_USAGE \
	N_("git worktree remove [-f] <worktree>")
#परिभाषय BUILTIN_WORKTREE_REPAIR_USAGE \
	N_("git worktree repair [<path>...]")
#परिभाषय BUILTIN_WORKTREE_UNLOCK_USAGE \
	N_("git worktree unlock <worktree>")

#परिभाषय WORKTREE_सङ्कलय_DWIM_ORPHAN_INFER_TEXT \
	_("No possible source branch, inferring '--orphan'")

#परिभाषय WORKTREE_सङ्कलय_ORPHAN_WITH_DASH_B_HINT_TEXT \
	_("If you meant to create a worktree containing a new unborn branch\n" \
	"(branch with no commits) for this repository, you can do so\n" \
	"using the --orphan flag:\n" \
	"\n" \
	"    git worktree add --orphan -b %s %s\n")

#परिभाषय WORKTREE_सङ्कलय_ORPHAN_NO_DASH_B_HINT_TEXT \
	_("If you meant to create a worktree containing a new unborn branch\n" \
	"(branch with no commits) for this repository, you can do so\n" \
	"using the --orphan flag:\n" \
	"\n" \
	"    git worktree add --orphan %s\n")

स्थिर ध्रुव वर्ण * ध्रुव git_worktree_usage[] = {
	BUILTIN_WORKTREE_सङ्कलय_USAGE,
	BUILTIN_WORKTREE_LIST_USAGE,
	BUILTIN_WORKTREE_तालकम्_USAGE,
	BUILTIN_WORKTREE_MOVE_USAGE,
	BUILTIN_WORKTREE_PRUNE_USAGE,
	BUILTIN_WORKTREE_REMOVE_USAGE,
	BUILTIN_WORKTREE_REPAIR_USAGE,
	BUILTIN_WORKTREE_UNLOCK_USAGE,
	रिक्त
};

स्थिर ध्रुव वर्ण * ध्रुव git_worktree_सङ्कलय_usage[] = {
	BUILTIN_WORKTREE_सङ्कलय_USAGE,
	रिक्त,
};

स्थिर ध्रुव वर्ण * ध्रुव git_worktree_list_usage[] = {
	BUILTIN_WORKTREE_LIST_USAGE,
	रिक्त
};

स्थिर ध्रुव वर्ण * ध्रुव git_worktree_तालकम्_usage[] = {
	BUILTIN_WORKTREE_तालकम्_USAGE,
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

संरचना सङ्कलय_opts {
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
	यदि (!वाक्यं_तुलय(var, "worktree.guessremote")) {
		guess_remote = git_config_bool(var, मानम्);
		प्रत्यागम 0;
	} अन्यथा यदि (!वाक्यं_तुलय(var, "worktree.userelativepaths")) {
		use_relative_paths = git_config_bool(var, मानम्);
		प्रत्यागम 0;
	}

	प्रत्यागम git_default_config(var, मानम्, ctx, cb);
}

स्थिर पूर्णाङ्क delete_git_dir(ध्रुव वर्ण *id)
{
	संरचना strbuf sb = STRBUF_INIT;
	पूर्णाङ्क ret;

	repo_common_मार्गः_append(the_repository, &sb, "worktrees/%s", id);
	ret = remove_dir_recursively(&sb, 0);
	यदि (ret < 0 && errno == ENOTDIR)
		ret = unlink(sb.buf);
	यदि (ret)
		error_errno(_("failed to delete '%s'"), sb.buf);
	strbuf_release(&sb);
	प्रत्यागम ret;
}

स्थिर शून्य delete_worktrees_dir_if_रिक्तानि(शून्य)
{
	वर्ण *मार्गः = repo_git_मार्गः(the_repository, "worktrees");
	rmdir(मार्गः); /* ignore failed removal */
	मुक्त(मार्गः);
}

स्थिर शून्य prune_worktree(ध्रुव वर्ण *id, ध्रुव वर्ण *reason)
{
	यदि (show_only || verbose)
		fprintf_ln(stderr, _("Removing %s/%s: %s"), "worktrees", id, reason);
	यदि (!show_only)
		delete_git_dir(id);
}

स्थिर पूर्णाङ्क prune_cmp(ध्रुव शून्य *a, ध्रुव शून्य *b)
{
	ध्रुव संरचना string_list_item *x = a;
	ध्रुव संरचना string_list_item *y = b;
	पूर्णाङ्क c;

	यदि ((c = fspathcmp(x->string, y->string)))
	    प्रत्यागम c;
	/*
	 * paths same; prune_dupes() removes all but the first worktree entry
	 * having the same path, so sort main worktree ('util' is NULL) above
	 * linked worktrees ('util' not NULL) since main worktree can't be
	 * removed
	 */
	यदि (!x->util)
		प्रत्यागम -1;
	यदि (!y->util)
		प्रत्यागम 1;
	/* paths same; sort by .git/worktrees/<id> */
	प्रत्यागम वाक्यं_तुलय(x->util, y->util);
}

स्थिर शून्य prune_dups(संरचना string_list *l)
{
	पूर्णाङ्क i;

	QSORT(l->वस्तूनि, l->nr, prune_cmp);
	प्रति (i = 1; i < l->nr; i++) {
		यदि (!fspathcmp(l->वस्तूनि[i].string, l->वस्तूनि[i - 1].string))
			prune_worktree(l->वस्तूनि[i].util, "duplicate entry");
	}
}

स्थिर शून्य prune_worktrees(शून्य)
{
	संरचना strbuf reason = STRBUF_INIT;
	संरचना strbuf main_मार्गः = STRBUF_INIT;
	संरचना string_list kept = STRING_LIST_INIT_DUP;
	वर्ण *मार्गः;
	DIR *dir;
	संरचना dirent *d;

	मार्गः = repo_git_मार्गः(the_repository, "worktrees");
	dir = opendir(मार्गः);
	मुक्त(मार्गः);
	यदि (!dir)
		प्रत्यागम;
	यावत् ((d = readdir_skip_dot_and_dotdot(dir)) != रिक्त) {
		वर्ण *मार्गः;
		strbuf_reset(&reason);
		यदि (should_prune_worktree(the_repository, d->d_name, &reason, &मार्गः, expire))
			prune_worktree(d->d_name, reason.buf);
		अन्यथा यदि (मार्गः)
			string_list_append_nodup(&kept, मार्गः)->util = xstrdup(d->d_name);
	}
	closedir(dir);

	strbuf_सङ्कलय_absolute_मार्गः(&main_मार्गः, repo_get_common_dir(the_repository));
	/* massage main worktree absolute path to match 'gitdir' content */
	strbuf_strip_suffix(&main_मार्गः, "/.");
	string_list_append_nodup(&kept, strbuf_detach(&main_मार्गः, रिक्त));
	prune_dups(&kept);
	string_list_clear(&kept, 1);

	यदि (!show_only)
		delete_worktrees_dir_if_रिक्तानि();
	strbuf_release(&reason);
}

स्थिर पूर्णाङ्क prune(पूर्णाङ्क ac, ध्रुव वर्ण **av, ध्रुव वर्ण *उपसर्गः,
		 संरचना repository *repo अप्रयुक्तम्)
{
	संरचना option options[] = {
		OPT_DRY_सततम्(&show_only, N_("do not remove, show only")),
		OPT__VERBOSE(&verbose, N_("report pruned working trees")),
		OPT_EXPIRY_DATE(0, "expire", &expire,
				N_("prune missing working trees older than <time>")),
		OPT_अन्तः()
	};

	expire = TIME_MAX;
	ac = parse_options(ac, av, उपसर्गः, options, git_worktree_prune_usage,
			   0);
	यदि (ac)
		usage_with_options(git_worktree_prune_usage, options);
	prune_worktrees();
	प्रत्यागम 0;
}

स्थिर वर्ण *junk_work_वृक्षः;
स्थिर वर्ण *junk_git_dir;
स्थिर पूर्णाङ्क is_junk;
स्थिर pid_t junk_pid;

स्थिर शून्य remove_junk(शून्य)
{
	संरचना strbuf sb = STRBUF_INIT;
	यदि (!is_junk || getpid() != junk_pid)
		प्रत्यागम;
	यदि (junk_git_dir) {
		strbuf_addstr(&sb, junk_git_dir);
		remove_dir_recursively(&sb, 0);
		strbuf_reset(&sb);
	}
	यदि (junk_work_वृक्षः) {
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

स्थिर वर्ण *worktree_basename_dup(ध्रुव वर्ण *मार्गः)
{
	ध्रुव वर्ण *name;
	पूर्णाङ्क len;

	len = वाक्यदैर्घ्यम्(मार्गः);
	यावत् (len && is_dir_sep(मार्गः[len - 1]))
		len--;

	name = मार्गः + len;
	यावत् (name > मार्गः && !is_dir_sep(name[-1]))
		name--;

	प्रत्यागम xmemdupz(name, मार्गः + len - name);
}

/* check that path is viable location for worktree */
स्थिर शून्य check_candidate_मार्गः(ध्रुव वर्ण *मार्गः,
				 पूर्णाङ्क force,
				 संरचना worktree **worktrees,
				 ध्रुव वर्ण *cmd)
{
	संरचना worktree *wt;
	पूर्णाङ्क locked;

	यदि (file_exists(मार्गः) && !is_रिक्तानि_dir(मार्गः))
		die(_("'%s' already exists"), मार्गः);

	wt = find_worktree_by_मार्गः(worktrees, मार्गः);
	यदि (!wt)
		प्रत्यागम;

	locked = !!worktree_तालकम्_reason(wt);
	यदि ((!locked && force) || (locked && force > 1)) {
		यदि (delete_git_dir(wt->id))
		    die(_("unusable worktree destination '%s'"), मार्गः);
		प्रत्यागम;
	}

	यदि (locked)
		die(_("'%s' is a missing but locked worktree;\nuse '%s -f -f' to override, or 'unlock' and 'prune' or 'remove' to clear"), मार्गः, cmd);
	अन्यथा
		die(_("'%s' is a missing but already registered worktree;\nuse '%s -f' to override, or 'prune' or 'remove' to clear"), मार्गः, cmd);
}

स्थिर शून्य copy_sparse_checkout(ध्रुव वर्ण *worktree_git_dir)
{
	वर्ण *from_file = repo_git_मार्गः(the_repository, "info/sparse-checkout");
	वर्ण *गन्तव्यम्_file = xstrfmt("%s/info/sparse-checkout", worktree_git_dir);

	यदि (file_exists(from_file)) {
		यदि (safe_create_leading_directories(the_repository, गन्तव्यम्_file) ||
			copy_file(the_repository, गन्तव्यम्_file, from_file, 0666))
			error(_("failed to copy '%s' to '%s'; sparse-checkout may not work correctly"),
				from_file, गन्तव्यम्_file);
	}

	मुक्त(from_file);
	मुक्त(गन्तव्यम्_file);
}

स्थिर शून्य copy_filtered_worktree_config(ध्रुव वर्ण *worktree_git_dir)
{
	वर्ण *from_file = repo_git_मार्गः(the_repository, "config.worktree");
	वर्ण *गन्तव्यम्_file = xstrfmt("%s/config.worktree", worktree_git_dir);

	यदि (file_exists(from_file)) {
		संरचना config_समुच्चयः cs = { { 0 } };
		पूर्णाङ्क bare;

		यदि (safe_create_leading_directories(the_repository, गन्तव्यम्_file) ||
			copy_file(the_repository, गन्तव्यम्_file, from_file, 0666)) {
			error(_("failed to copy worktree config from '%s' to '%s'"),
				from_file, गन्तव्यम्_file);
			गच्छ worktree_copy_cleanup;
		}

		git_configset_init(&cs);
		git_configset_सङ्कलय_file(&cs, from_file);

		यदि (!git_configset_get_bool(&cs, "core.bare", &bare) &&
			bare &&
			repo_config_समुच्चयः_multivar_अन्तर्_file_gently(the_repository,
				गन्तव्यम्_file, "core.bare", रिक्त, "true", रिक्त, 0))
			error(_("failed to unset '%s' in '%s'"),
				"core.bare", गन्तव्यम्_file);
		यदि (!git_configset_get(&cs, "core.worktree") &&
			repo_config_समुच्चयः_अन्तर्_file_gently(the_repository, गन्तव्यम्_file,
						       "core.worktree", रिक्त, रिक्त))
			error(_("failed to unset '%s' in '%s'"),
				"core.worktree", गन्तव्यम्_file);

		git_configset_clear(&cs);
	}

worktree_copy_cleanup:
	मुक्त(from_file);
	मुक्त(गन्तव्यम्_file);
}

स्थिर पूर्णाङ्क checkout_worktree(ध्रुव संरचना सङ्कलय_opts *opts,
			     संरचना strvec *child_env)
{
	संरचना child_process cp = CHILD_PROCESS_INIT;
	cp.git_cmd = 1;
	strvec_pushl(&cp.तर्काः, "reset", "--hard", "--no-recurse-submodules", रिक्त);
	यदि (opts->quiet)
		strvec_प्रक्षिप(&cp.तर्काः, "--quiet");
	strvec_pushv(&cp.env, child_env->v);
	प्रत्यागम सततम्_command(&cp);
}

स्थिर पूर्णाङ्क make_worktree_orphan(ध्रुव वर्ण * ref, ध्रुव संरचना सङ्कलय_opts *opts,
				संरचना strvec *child_env)
{
	संरचना strbuf symref = STRBUF_INIT;
	संरचना child_process cp = CHILD_PROCESS_INIT;

	validate_new_branchname(ref, &symref, 0);
	strvec_pushl(&cp.तर्काः, "symbolic-ref", "HEAD", symref.buf, रिक्त);
	यदि (opts->quiet)
		strvec_प्रक्षिप(&cp.तर्काः, "--quiet");
	strvec_pushv(&cp.env, child_env->v);
	strbuf_release(&symref);
	cp.git_cmd = 1;
	प्रत्यागम सततम्_command(&cp);
}

/*
 * References for worktrees are generally stored in '$GIT_DIR/worktrees/<wt_id>'.
 * But when using alternate reference directories, we want to store the worktree
 * references in '$ALTERNATE_REFERENCE_DIR/worktrees/<wt_id>'.
 *
 * Create the necessary folder structure to facilitate the same. But to ensure
 * that the former path is still considered a Git directory, add stubs.
 */
स्थिर शून्य सज्जता_alternate_ref_dir(संरचना worktree *wt, ध्रुव वर्ण *wt_git_मार्गः)
{
	संरचना strbuf sb = STRBUF_INIT;
	वर्ण *मार्गः;

	मार्गः = wt->repo->ref_storage_payload;
	यदि (!मार्गः)
		प्रत्यागम;

	यदि (!is_absolute_मार्गः(मार्गः))
		strbuf_addf(&sb, "%s/", wt->repo->commondir);

	strbuf_addf(&sb, "%s/worktrees", मार्गः);
	safe_create_dir(wt->repo, sb.buf, 1);
	strbuf_addf(&sb, "/%s", wt->id);
	safe_create_dir(wt->repo, sb.buf, 1);
	strbuf_reset(&sb);

	strbuf_addf(&sb, "this worktree stores references in %s/worktrees/%s",
		    मार्गः, wt->id);
	refs_create_refdir_stubs(wt->repo, wt_git_मार्गः, sb.buf);

	strbuf_release(&sb);
}

स्थिर पूर्णाङ्क सङ्कलय_worktree(ध्रुव वर्ण *मार्गः, ध्रुव वर्ण *refname,
			ध्रुव संरचना सङ्कलय_opts *opts)
{
	संरचना strbuf sb_git = STRBUF_INIT, sb_repo = STRBUF_INIT;
	संरचना strbuf sb = STRBUF_INIT;
	ध्रुव वर्ण *name;
	वर्ण *name_गन्तव्यम्_free = रिक्त;
	संरचना strvec child_env = STRVEC_INIT;
	अचिह्नित पूर्णाङ्क गणक = 0;
	पूर्णाङ्क len, ret;
	संरचना strbuf symref = STRBUF_INIT;
	संरचना commit *commit = रिक्त;
	पूर्णाङ्क is_branch = 0;
	संरचना strbuf sb_name = STRBUF_INIT;
	संरचना worktree **worktrees, *wt = रिक्त;
	संरचना ref_store *wt_refs;
	संरचना repo_config_मानानि *cfg = repo_config_मानानि(the_repository);

	worktrees = get_worktrees(the_repository);
	check_candidate_मार्गः(मार्गः, opts->force, worktrees, "add");
	free_worktrees(worktrees);
	worktrees = रिक्त;

	/* is 'refname' a branch or commit? */
	यदि (!opts->detach && !check_branch_ref(the_repository, &symref, refname) &&
	    refs_ref_exists(get_main_ref_store(the_repository), symref.buf)) {
		is_branch = 1;
		यदि (!opts->force)
			die_if_checked_बहिर्गमनम्(symref.buf, 0);
	}
	commit = lookup_commit_reference_by_name(refname);
	यदि (!commit && !opts->orphan)
		die(_("invalid reference: %s"), refname);

	name = name_गन्तव्यम्_free = worktree_basename_dup(मार्गः);
	यदि (!*name)
		die(_("invalid path '%s'"), मार्गः);
	sanitize_refname_component(name, &sb_name);
	यदि (!sb_name.len)
		BUG("How come '%s' becomes empty after sanitization?", name);
	name = sb_name.buf;
	repo_git_मार्गः_replace(the_repository, &sb_repo, "worktrees/%s", name);
	len = sb_repo.len;
	यदि (safe_create_leading_directories_const(the_repository, sb_repo.buf))
		die_errno(_("could not create leading directories of '%s'"),
			  sb_repo.buf);

	यावत् (mkdir(sb_repo.buf, 0777)) {
		गणक++;
		यदि ((errno != EEXIST) || !गणक /*अतिप्रवाहः*/)
			die_errno(_("could not create directory of '%s'"),
				  sb_repo.buf);
		strbuf_setlen(&sb_repo, len);
		strbuf_addf(&sb_repo, "%d", गणक);
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
	यदि (opts->keep_locked)
		write_file(sb.buf, "%s", opts->keep_locked);
	अन्यथा
		write_file(sb.buf, _("initializing"));

	strbuf_addf(&sb_git, "%s/.git", मार्गः);
	यदि (safe_create_leading_directories_const(the_repository, sb_git.buf))
		die_errno(_("could not create leading directories of '%s'"),
			  sb_git.buf);
	junk_work_वृक्षः = xstrdup(मार्गः);

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
	यदि (!wt) {
		ret = error(_("could not find created worktree '%s'"), name);
		गच्छ समाप्तम्;
	}
	सज्जता_alternate_ref_dir(wt, sb_repo.buf);
	wt_refs = get_worktree_ref_store(wt);

	ret = ref_store_create_on_disk(wt_refs, REF_STORE_CREATE_ON_DISK_IS_WORKTREE, &sb);
	यदि (ret)
		गच्छ समाप्तम्;

	यदि (!is_branch && commit)
		ret = refs_update_ref(wt_refs, रिक्त, "HEAD", &commit->object.oid,
				      रिक्त, 0, UPDATE_REFS_सन्देशः_ON_ERR);
	अन्यथा
		ret = refs_update_symref(wt_refs, "HEAD", symref.buf, रिक्त);
	यदि (ret)
		गच्छ समाप्तम्;

	/*
	 * If the current worktree has sparse-checkout enabled, then copy
	 * the sparse-checkout patterns from the current worktree.
	 */
	यदि (cfg->apply_sparse_checkout)
		copy_sparse_checkout(sb_repo.buf);

	/*
	 * If we are using worktree config, then copy all current config
	 * values from the current worktree into the new one, that way the
	 * new worktree behaves the same as this one.
	 */
	यदि (the_repository->repository_format_worktree_config)
		copy_filtered_worktree_config(sb_repo.buf);

	strvec_pushf(&child_env, "%s=%s", GIT_DIR_ENVIRONMENT, sb_git.buf);
	strvec_pushf(&child_env, "%s=%s", GIT_WORK_वृक्षः_ENVIRONMENT, मार्गः);

	यदि (opts->orphan &&
	    (ret = make_worktree_orphan(refname, opts, &child_env)))
		गच्छ समाप्तम्;

	यदि (opts->checkout &&
	    (ret = checkout_worktree(opts, &child_env)))
		गच्छ समाप्तम्;

	is_junk = 0;
	FREE_AND_NULL(junk_work_वृक्षः);
	FREE_AND_NULL(junk_git_dir);

समाप्तम्:
	यदि (ret || !opts->keep_locked) {
		strbuf_reset(&sb);
		strbuf_addf(&sb, "%s/locked", sb_repo.buf);
		unlink_or_warn(sb.buf);
	}

	/*
	 * Hook failure does not warrant worktree deletion, so run hook after
	 * is_junk is cleared, but do return appropriate code when hook fails.
	 */
	यदि (!ret && opts->checkout && !opts->orphan) {
		संरचना सततम्_hooks_opt opt = सततम्_HOOKS_OPT_INIT_FORCE_SERIAL;

		strvec_pushl(&opt.env, "GIT_DIR", "GIT_WORK_TREE", रिक्त);
		strvec_pushl(&opt.तर्काः,
			     oid_गन्तव्यम्_hex(null_oid(the_hash_algo)),
			     oid_गन्तव्यम्_hex(&commit->object.oid),
			     "1",
			     रिक्त);
		opt.dir = मार्गः;

		ret = सततम्_hooks_opt(the_repository, "post-checkout", &opt);
	}

	strvec_clear(&child_env);
	strbuf_release(&sb);
	strbuf_release(&symref);
	strbuf_release(&sb_repo);
	strbuf_release(&sb_git);
	strbuf_release(&sb_name);
	free_worktree(wt);
	मुक्त(name_गन्तव्यम्_free);
	प्रत्यागम ret;
}

स्थिर शून्य print_preparing_worktree_पङ्क्तिः(पूर्णाङ्क detach,
					  ध्रुव वर्ण *branch,
					  ध्रुव वर्ण *new_branch,
					  पूर्णाङ्क force_new_branch)
{
	यदि (force_new_branch) {
		संरचना commit *commit = lookup_commit_reference_by_name(new_branch);
		यदि (!commit)
			fprintf_ln(stderr, _("Preparing worktree (new branch '%s')"), new_branch);
		अन्यथा
			fprintf_ln(stderr, _("Preparing worktree (resetting branch '%s'; was at %s)"),
				  new_branch,
				  repo_find_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV));
	} अन्यथा यदि (new_branch) {
		fprintf_ln(stderr, _("Preparing worktree (new branch '%s')"), new_branch);
	} अन्यथा {
		संरचना strbuf s = STRBUF_INIT;
		यदि (!detach && !check_branch_ref(the_repository, &s, branch) &&
		    refs_ref_exists(get_main_ref_store(the_repository), s.buf))
			fprintf_ln(stderr, _("Preparing worktree (checking out '%s')"),
				  branch);
		अन्यथा {
			संरचना commit *commit = lookup_commit_reference_by_name(branch);
			यदि (!commit)
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
स्थिर पूर्णाङ्क first_valid_ref(ध्रुव संरचना reference *ref अप्रयुक्तम्, शून्य *cb_दत्त अप्रयुक्तम्)
{
	प्रत्यागम 1;
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
स्थिर पूर्णाङ्क can_use_local_refs(ध्रुव संरचना सङ्कलय_opts *opts)
{
	यदि (refs_शीर्ष_ref(get_main_ref_store(the_repository), first_valid_ref, रिक्त)) {
		प्रत्यागम 1;
	} अन्यथा यदि (refs_for_each_branch_ref(get_main_ref_store(the_repository), first_valid_ref, रिक्त)) {
		यदि (!opts->quiet)
			warning(_("HEAD points to an invalid (or orphaned) reference.\n"));
		प्रत्यागम 1;
	}
	प्रत्यागम 0;
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
स्थिर पूर्णाङ्क can_use_remote_refs(ध्रुव संरचना सङ्कलय_opts *opts)
{
	यदि (!guess_remote) {
		प्रत्यागम 0;
	} अन्यथा यदि (refs_for_each_remote_ref(get_main_ref_store(the_repository), first_valid_ref, रिक्त)) {
		प्रत्यागम 1;
	} अन्यथा यदि (!opts->force && remote_get(रिक्त)) {
		die(_("No local or remote refs exist despite at least one remote\n"
		      "present, stopping; use 'add -f' to override or fetch a remote first"));
	}
	प्रत्यागम 0;
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
स्थिर पूर्णाङ्क dwim_orphan(ध्रुव संरचना सङ्कलय_opts *opts, पूर्णाङ्क opt_track, पूर्णाङ्क remote)
{
	यदि (can_use_local_refs(opts)) {
		प्रत्यागम 0;
	} अन्यथा यदि (remote && can_use_remote_refs(opts)) {
		प्रत्यागम 0;
	} अन्यथा यदि (!opts->quiet) {
		fprintf_ln(stderr, WORKTREE_सङ्कलय_DWIM_ORPHAN_INFER_TEXT);
	}

	यदि (opt_track) {
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--track");
	} अन्यथा यदि (!opts->checkout) {
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--no-checkout");
	}
	प्रत्यागम 1;
}

स्थिर शून्य advise_disambiguating_remotes(ध्रुव वर्ण *मार्गः, ध्रुव वर्ण *branch,
					  ध्रुव संरचना string_list *matched_remote_नामानि)
{
	संरचना string_list_item *item;

	advise(_("Branch name '%s' appears in multiple remotes:"), branch);
	for_each_string_list_item(item, matched_remote_नामानि) {
		advise(_("  %s"), item->string);
	}
	advise(_("If you meant to create a worktree from a remote tracking branch on\n"
		 "<remote>, you can do so by:\n"
		 "\n"
		 "    git worktree add -b %s %s <remote>/%s\n"
		 "\n"
		 "If you'd like to always prefer some remote, e.g. 'origin',\n"
		 "consider setting checkout.defaultRemote=origin in your config."),
	       branch, मार्गः, branch);
}

स्थिर वर्ण *dwim_branch(ध्रुव संरचना सङ्कलय_opts *opts, ध्रुव वर्ण *मार्गः, वर्ण **new_branch)
{
	पूर्णाङ्क branch_exists;
	वर्ण *branchname = worktree_basename_dup(मार्गः);
	संरचना strbuf ref = STRBUF_INIT;

	branch_exists = !check_branch_ref(the_repository, &ref, branchname) &&
			refs_ref_exists(get_main_ref_store(the_repository),
					ref.buf);
	strbuf_release(&ref);
	यदि (branch_exists)
		प्रत्यागम branchname;

	*new_branch = branchname;
	यदि (guess_remote) {
		संरचना object_id oid;
		वर्ण *remote;
		पूर्णाङ्क अङ्कः_matches = 0;
		संरचना string_list matched_remote_नामानि = STRING_LIST_INIT_DUP;

		remote = unique_tracking_name(*new_branch, &oid, &अङ्कः_matches,
					      &matched_remote_नामानि);
		यदि (!remote && अङ्कः_matches > 1) {
			यदि (!opts->quiet &&
			    advice_enabled(ADVICE_CHECKOUT_AMBIGUOUS_REMOTE_BRANCH_NAME))
				advise_disambiguating_remotes(मार्गः, *new_branch,
							      &matched_remote_नामानि);
			die(_("'%s' matched multiple (%d) remote tracking branches"),
			    *new_branch, अङ्कः_matches);
		}
		string_list_clear(&matched_remote_नामानि, 0);
		प्रत्यागम remote;
	}
	प्रत्यागम रिक्त;
}

स्थिर पूर्णाङ्क सङ्कलय(पूर्णाङ्क ac, ध्रुव वर्ण **av, ध्रुव वर्ण *उपसर्गः,
	       संरचना repository *repo अप्रयुक्तम्)
{
	संरचना सङ्कलय_opts opts;
	ध्रुव वर्ण *new_branch_force = रिक्त;
	वर्ण *मार्गः;
	ध्रुव वर्ण *branch;
	वर्ण *branch_गन्तव्यम्_free = रिक्त;
	वर्ण *new_branch_गन्तव्यम्_free = रिक्त;
	ध्रुव वर्ण *new_branch = रिक्त;
	वर्ण *opt_track = रिक्त;
	ध्रुव वर्ण *तालकम्_reason = रिक्त;
	पूर्णाङ्क keep_locked = 0;
	पूर्णाङ्क प्रयुक्तम्_new_branch_options;
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
		OPT_STRING(0, "reason", &तालकम्_reason, N_("string"),
			   N_("reason for locking")),
		OPT__QUIET(&opts.quiet, N_("suppress progress reporting")),
		OPT_PASSTHRU(0, "track", &opt_track, रिक्त,
			     N_("set up tracking mode (see git-branch(1))"),
			     PARSE_OPT_NOARG | PARSE_OPT_OPTARG),
		OPT_BOOL(0, "guess-remote", &guess_remote,
			 N_("try to match the new branch name with a remote-tracking branch")),
		OPT_BOOL(0, "relative-paths", &opts.relative_paths,
			 N_("use relative paths for worktrees")),
		OPT_अन्तः()
	};
	पूर्णाङ्क ret;

	स्मृतिं_पूरय(&opts, 0, परिमाण(opts));
	opts.checkout = 1;
	opts.relative_paths = use_relative_paths;
	ac = parse_options(ac, av, उपसर्गः, options, git_worktree_सङ्कलय_usage, 0);
	यदि (!!opts.detach + !!new_branch + !!new_branch_force > 1)
		die(_("options '%s', '%s', and '%s' cannot be used together"), "-b", "-B", "--detach");
	यदि (opts.detach && opts.orphan)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--detach");
	यदि (opts.orphan && opt_track)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--track");
	यदि (opts.orphan && !opts.checkout)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--no-checkout");
	यदि (opts.orphan && ac == 2)
		die(_("option '%s' and commit-ish cannot be used together"),
		    "--orphan");
	यदि (तालकम्_reason && !keep_locked)
		die(_("the option '%s' requires '%s'"), "--reason", "--lock");
	यदि (तालकम्_reason)
		opts.keep_locked = तालकम्_reason;
	अन्यथा यदि (keep_locked)
		opts.keep_locked = _("added with --lock");

	यदि (ac < 1 || ac > 2)
		usage_with_options(git_worktree_सङ्कलय_usage, options);

	मार्गः = उपसर्गः_filename(उपसर्गः, av[0]);
	branch = ac < 2 ? "HEAD" : av[1];
	प्रयुक्तम्_new_branch_options = new_branch || new_branch_force;

	यदि (!वाक्यं_तुलय(branch, "-"))
		branch = "@{-1}";

	यदि (new_branch_force) {
		संरचना strbuf symref = STRBUF_INIT;

		new_branch = new_branch_force;

		यदि (!opts.force &&
		    !check_branch_ref(the_repository, &symref, new_branch) &&
		    refs_ref_exists(get_main_ref_store(the_repository), symref.buf))
			die_if_checked_बहिर्गमनम्(symref.buf, 0);
		strbuf_release(&symref);
	}

	यदि (opts.orphan && !new_branch) {
		new_branch = new_branch_गन्तव्यम्_free = worktree_basename_dup(मार्गः);
	} अन्यथा यदि (opts.orphan) {
		; /* no-op */
	} अन्यथा यदि (opts.detach) {
		/* Check HEAD */
		यदि (!वाक्यं_तुलय(branch, "HEAD"))
			can_use_local_refs(&opts);
	} अन्यथा यदि (ac < 2 && new_branch) {
		/* DWIM: Infer --orphan when repo has no refs. */
		opts.orphan = dwim_orphan(&opts, !!opt_track, 0);
	} अन्यथा यदि (ac < 2) {
		/* DWIM: Guess branch name from path. */
		वर्ण *s = dwim_branch(&opts, मार्गः, &new_branch_गन्तव्यम्_free);
		यदि (s)
			branch = branch_गन्तव्यम्_free = s;
		new_branch = new_branch_गन्तव्यम्_free;

		/* DWIM: Infer --orphan when repo has no refs. */
		opts.orphan = (!s) && dwim_orphan(&opts, !!opt_track, 1);
	} अन्यथा यदि (ac == 2 && new_branch) {
		यदि (!वाक्यं_तुलय(branch, "HEAD"))
			can_use_local_refs(&opts);
	} अन्यथा यदि (ac == 2) {
		संरचना commit *commit;

		commit = lookup_commit_reference_by_name(branch);
		यदि (!commit) {
			संरचना object_id oid;
			वर्ण *remote;
			पूर्णाङ्क अङ्कः_matches = 0;
			संरचना string_list matched_remote_नामानि = STRING_LIST_INIT_DUP;

			remote = unique_tracking_name(branch, &oid, &अङ्कः_matches,
						      &matched_remote_नामानि);
			यदि (remote) {
				new_branch = branch;
				branch = new_branch_गन्तव्यम्_free = remote;
			} अन्यथा यदि (अङ्कः_matches > 1) {
				यदि (!opts.quiet &&
				    advice_enabled(ADVICE_CHECKOUT_AMBIGUOUS_REMOTE_BRANCH_NAME))
					advise_disambiguating_remotes(मार्गः, branch,
								      &matched_remote_नामानि);
				die(_("'%s' matched multiple (%d) remote tracking branches"),
				    branch, अङ्कः_matches);
			}
			string_list_clear(&matched_remote_नामानि, 0);
		}

		यदि (!वाक्यं_तुलय(branch, "HEAD"))
			can_use_local_refs(&opts);

	}

	यदि (!opts.orphan && !lookup_commit_reference_by_name(branch)) {
		पूर्णाङ्क attempt_hint = !opts.quiet && (ac < 2);
		यदि (attempt_hint && प्रयुक्तम्_new_branch_options) {
			advise_if_enabled(ADVICE_WORKTREE_सङ्कलय_ORPHAN,
				WORKTREE_सङ्कलय_ORPHAN_WITH_DASH_B_HINT_TEXT,
				new_branch, मार्गः);
		} अन्यथा यदि (attempt_hint) {
			advise_if_enabled(ADVICE_WORKTREE_सङ्कलय_ORPHAN,
				WORKTREE_सङ्कलय_ORPHAN_NO_DASH_B_HINT_TEXT, मार्गः);
		}
		die(_("invalid reference: %s"), branch);
	}

	यदि (!opts.quiet)
		print_preparing_worktree_पङ्क्तिः(opts.detach, branch, new_branch, !!new_branch_force);

	यदि (opts.orphan) {
		branch = new_branch;
	} अन्यथा यदि (new_branch) {
		संरचना child_process cp = CHILD_PROCESS_INIT;
		cp.git_cmd = 1;
		strvec_प्रक्षिप(&cp.तर्काः, "branch");
		यदि (new_branch_force)
			strvec_प्रक्षिप(&cp.तर्काः, "--force");
		यदि (opts.quiet)
			strvec_प्रक्षिप(&cp.तर्काः, "--quiet");
		strvec_प्रक्षिप(&cp.तर्काः, new_branch);
		strvec_प्रक्षिप(&cp.तर्काः, branch);
		यदि (opt_track)
			strvec_प्रक्षिप(&cp.तर्काः, opt_track);
		यदि (सततम्_command(&cp)) {
			ret = -1;
			गच्छ cleanup;
		}
		branch = new_branch;
	} अन्यथा यदि (opt_track) {
		die(_("--[no-]track can only be used if a new branch is created"));
	}

	ret = सङ्कलय_worktree(मार्गः, branch, &opts);
cleanup:
	मुक्त(मार्गः);
	मुक्त(opt_track);
	मुक्त(branch_गन्तव्यम्_free);
	मुक्त(new_branch_गन्तव्यम्_free);
	प्रत्यागम ret;
}

स्थिर शून्य show_worktree_porcelain(संरचना worktree *wt, पूर्णाङ्क पङ्क्तिः_terminator)
{
	ध्रुव वर्ण *reason;

	प्रारूप_लिख("worktree %s%c", wt->मार्गः, पङ्क्तिः_terminator);
	यदि (wt->is_bare)
		प्रारूप_लिख("bare%c", पङ्क्तिः_terminator);
	अन्यथा {
		प्रारूप_लिख("HEAD %s%c", oid_गन्तव्यम्_hex(&wt->शीर्ष_oid), पङ्क्तिः_terminator);
		यदि (wt->is_detached)
			प्रारूप_लिख("detached%c", पङ्क्तिः_terminator);
		अन्यथा यदि (wt->शीर्ष_ref)
			प्रारूप_लिख("branch %s%c", wt->शीर्ष_ref, पङ्क्तिः_terminator);
	}

	reason = worktree_तालकम्_reason(wt);
	यदि (reason) {
		fputs("locked", stdout);
		यदि (*reason) {
			fputc(' ', stdout);
			write_name_quoted(reason, stdout, पङ्क्तिः_terminator);
		} अन्यथा {
			fputc(पङ्क्तिः_terminator, stdout);
		}
	}

	reason = worktree_prune_reason(wt, expire);
	यदि (reason)
		प्रारूप_लिख("prunable %s%c", reason, पङ्क्तिः_terminator);

	fputc(पङ्क्तिः_terminator, stdout);
}

संरचना worktree_display {
	वर्ण *मार्गः;
	पूर्णाङ्क विस्तारः;
};

स्थिर शून्य show_worktree(संरचना worktree *wt, संरचना worktree_display *display,
			  पूर्णाङ्क मार्गः_maxwidth, पूर्णाङ्क abbrev_len)
{
	संरचना strbuf sb = STRBUF_INIT;
	ध्रुव वर्ण *reason;

	strbuf_addf(&sb, "%s%*s", display->मार्गः, 1 + मार्गः_maxwidth - display->विस्तारः, "");
	यदि (wt->is_bare)
		strbuf_addstr(&sb, "(bare)");
	अन्यथा {
		strbuf_addf(&sb, "%-*s ", abbrev_len,
				repo_find_unique_abbrev(the_repository, &wt->शीर्ष_oid, DEFAULT_ABBREV));
		यदि (wt->is_detached)
			strbuf_addstr(&sb, "(detached HEAD)");
		अन्यथा यदि (wt->शीर्ष_ref) {
			वर्ण *ref = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
								 wt->शीर्ष_ref,
								 0);
			strbuf_addf(&sb, "[%s]", ref);
			मुक्त(ref);
		} अन्यथा
			strbuf_addstr(&sb, "(error)");
	}

	reason = worktree_तालकम्_reason(wt);
	यदि (verbose && reason && *reason)
		strbuf_addf(&sb, "\n\tlocked: %s", reason);
	अन्यथा यदि (reason)
		strbuf_addstr(&sb, " locked");

	reason = worktree_prune_reason(wt, expire);
	यदि (verbose && reason)
		strbuf_addf(&sb, "\n\tprunable: %s", reason);
	अन्यथा यदि (reason)
		strbuf_addstr(&sb, " prunable");

	प्रारूप_लिख("%s\n", sb.buf);
	strbuf_release(&sb);
}

स्थिर शून्य measure_widths(संरचना worktree **wt, पूर्णाङ्क *abbrev,
			   संरचना worktree_display **d, पूर्णाङ्क *maxwidth)
{
	पूर्णाङ्क i, display_alloc = 0;
	संरचना worktree_display *display = रिक्त;
	संरचना strbuf buf = STRBUF_INIT;

	प्रति (i = 0; wt[i]; i++) {
		पूर्णाङ्क sha1_len;
		ALLOC_GROW(display, i + 1, display_alloc);
		quote_मार्गः(wt[i]->मार्गः, रिक्त, &buf, 0);
		display[i].विस्तारः = utf8_strwidth(buf.buf);
		display[i].मार्गः = strbuf_detach(&buf, रिक्त);

		यदि (display[i].विस्तारः > *maxwidth)
			*maxwidth = display[i].विस्तारः;
		sha1_len = वाक्यदैर्घ्यम्(repo_find_unique_abbrev(the_repository, &wt[i]->शीर्ष_oid, *abbrev));
		यदि (sha1_len > *abbrev)
			*abbrev = sha1_len;
	}
	*d = display;
}

स्थिर पूर्णाङ्क pathcmp(ध्रुव शून्य *a_, ध्रुव शून्य *b_)
{
	ध्रुव संरचना worktree *ध्रुव *a = a_;
	ध्रुव संरचना worktree *ध्रुव *b = b_;
	प्रत्यागम fspathcmp((*a)->मार्गः, (*b)->मार्गः);
}

स्थिर शून्य pathsort(संरचना worktree **wt)
{
	पूर्णाङ्क n = 0;
	संरचना worktree **p = wt;

	यावत् (*p++)
		n++;
	QSORT(wt, n, pathcmp);
}

स्थिर पूर्णाङ्क list(पूर्णाङ्क ac, ध्रुव वर्ण **av, ध्रुव वर्ण *उपसर्गः,
		संरचना repository *repo अप्रयुक्तम्)
{
	पूर्णाङ्क porcelain = 0;
	पूर्णाङ्क पङ्क्तिः_terminator = '\n';

	संरचना option options[] = {
		OPT_BOOL(0, "porcelain", &porcelain, N_("machine-readable output")),
		OPT__VERBOSE(&verbose, N_("show extended annotations and reasons, if available")),
		OPT_EXPIRY_DATE(0, "expire", &expire,
				N_("add 'prunable' annotation to missing worktrees older than <time>")),
		OPT_समुच्चयः_INT('z', रिक्त, &पङ्क्तिः_terminator,
			    N_("terminate records with a NUL character"), '\0'),
		OPT_अन्तः()
	};

	expire = TIME_MAX;
	ac = parse_options(ac, av, उपसर्गः, options, git_worktree_list_usage, 0);
	यदि (ac)
		usage_with_options(git_worktree_list_usage, options);
	अन्यथा यदि (verbose && porcelain)
		die(_("options '%s' and '%s' cannot be used together"), "--verbose", "--porcelain");
	अन्यथा यदि (!पङ्क्तिः_terminator && !porcelain)
		die(_("the option '%s' requires '%s'"), "-z", "--porcelain");
	अन्यथा {
		संरचना worktree **worktrees = get_worktrees(the_repository);
		पूर्णाङ्क मार्गः_maxwidth = 0, abbrev = DEFAULT_ABBREV, i;
		संरचना worktree_display *display = रिक्त;

		/* sort worktrees by path but keep main worktree at top */
		pathsort(worktrees + 1);

		यदि (!porcelain)
			measure_widths(worktrees, &abbrev,
				       &display, &मार्गः_maxwidth);

		प्रति (i = 0; worktrees[i]; i++) {
			यदि (porcelain)
				show_worktree_porcelain(worktrees[i],
							पङ्क्तिः_terminator);
			अन्यथा
				show_worktree(worktrees[i],
					      &display[i], मार्गः_maxwidth, abbrev);
		}
		प्रति (i = 0; display && worktrees[i]; i++)
			मुक्त(display[i].मार्गः);
		मुक्त(display);
		free_worktrees(worktrees);
	}
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क तालकम्_worktree(पूर्णाङ्क ac, ध्रुव वर्ण **av, ध्रुव वर्ण *उपसर्गः,
			 संरचना repository *repo अप्रयुक्तम्)
{
	ध्रुव वर्ण *reason = "", *old_reason;
	संरचना option options[] = {
		OPT_STRING(0, "reason", &reason, N_("string"),
			   N_("reason for locking")),
		OPT_अन्तः()
	};
	संरचना worktree **worktrees, *wt;
	वर्ण *मार्गः;

	ac = parse_options(ac, av, उपसर्गः, options, git_worktree_तालकम्_usage, 0);
	यदि (ac != 1)
		usage_with_options(git_worktree_तालकम्_usage, options);

	worktrees = get_worktrees(the_repository);
	wt = find_worktree(worktrees, उपसर्गः, av[0]);
	यदि (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	यदि (is_main_worktree(wt))
		die(_("The main working tree cannot be locked or unlocked"));

	old_reason = worktree_तालकम्_reason(wt);
	यदि (old_reason) {
		यदि (*old_reason)
			die(_("'%s' is already locked, reason: %s"),
			    av[0], old_reason);
		die(_("'%s' is already locked"), av[0]);
	}

	मार्गः = repo_common_मार्गः(the_repository, "worktrees/%s/locked", wt->id);
	write_file(मार्गः, "%s", reason);

	free_worktrees(worktrees);
	मुक्त(मार्गः);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क unlock_worktree(पूर्णाङ्क ac, ध्रुव वर्ण **av, ध्रुव वर्ण *उपसर्गः,
			   संरचना repository *repo अप्रयुक्तम्)
{
	संरचना option options[] = {
		OPT_अन्तः()
	};
	संरचना worktree **worktrees, *wt;
	वर्ण *मार्गः;
	पूर्णाङ्क ret;

	ac = parse_options(ac, av, उपसर्गः, options, git_worktree_unlock_usage, 0);
	यदि (ac != 1)
		usage_with_options(git_worktree_unlock_usage, options);

	worktrees = get_worktrees(the_repository);
	wt = find_worktree(worktrees, उपसर्गः, av[0]);
	यदि (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	यदि (is_main_worktree(wt))
		die(_("The main working tree cannot be locked or unlocked"));
	यदि (!worktree_तालकम्_reason(wt))
		die(_("'%s' is not locked"), av[0]);

	मार्गः = repo_common_मार्गः(the_repository, "worktrees/%s/locked", wt->id);
	ret = unlink_or_warn(मार्गः);

	free_worktrees(worktrees);
	मुक्त(मार्गः);
	प्रत्यागम ret;
}

स्थिर शून्य validate_no_submodules(ध्रुव संरचना worktree *wt)
{
	संरचना सूचकांक_state istate = सूचकांक_STATE_INIT(the_repository);
	संरचना strbuf मार्गः = STRBUF_INIT;
	पूर्णाङ्क i, प्राप्तम्_submodules = 0;
	वर्ण *wt_gitdir;

	wt_gitdir = get_worktree_git_dir(wt);

	यदि (is_directory(worktree_git_मार्गः(wt, "modules"))) {
		/*
		 * There could be false positives, e.g. the "modules"
		 * directory exists but is empty. But it's a rare case and
		 * this simpler check is probably good enough for now.
		 */
		प्राप्तम्_submodules = 1;
	} अन्यथा यदि (read_सूचकांक_from(&istate, worktree_git_मार्गः(wt, "index"),
				   wt_gitdir) > 0) {
		प्रति (i = 0; i < istate.cache_nr; i++) {
			संरचना cache_प्रविष्टिः *ce = istate.cache[i];
			पूर्णाङ्क err;

			यदि (!S_ISGITLINK(ce->ce_बहुलकम्))
				अनुवर्त;

			strbuf_reset(&मार्गः);
			strbuf_addf(&मार्गः, "%s/%s", wt->मार्गः, ce->name);
			यदि (!is_submodule_populated_gently(मार्गः.buf, &err))
				अनुवर्त;

			प्राप्तम्_submodules = 1;
			भङ्ग;
		}
	}
	discard_सूचकांक(&istate);
	strbuf_release(&मार्गः);
	मुक्त(wt_gitdir);

	यदि (प्राप्तम्_submodules)
		die(_("working trees containing submodules cannot be moved or removed"));
}

स्थिर पूर्णाङ्क move_worktree(पूर्णाङ्क ac, ध्रुव वर्ण **av, ध्रुव वर्ण *उपसर्गः,
			 संरचना repository *repo अप्रयुक्तम्)
{
	पूर्णाङ्क force = 0;
	संरचना option options[] = {
		OPT__FORCE(&force,
			 N_("force move even if worktree is dirty or locked"),
			 PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "relative-paths", &use_relative_paths,
			 N_("use relative paths for worktrees")),
		OPT_अन्तः()
	};
	संरचना worktree **worktrees, *wt;
	संरचना strbuf dst = STRBUF_INIT;
	संरचना strbuf errmsg = STRBUF_INIT;
	ध्रुव वर्ण *reason = रिक्त;
	वर्ण *मार्गः;

	ac = parse_options(ac, av, उपसर्गः, options, git_worktree_move_usage,
			   0);
	यदि (ac != 2)
		usage_with_options(git_worktree_move_usage, options);

	मार्गः = उपसर्गः_filename(उपसर्गः, av[1]);
	strbuf_addstr(&dst, मार्गः);
	मुक्त(मार्गः);

	worktrees = get_worktrees(the_repository);
	wt = find_worktree(worktrees, उपसर्गः, av[0]);
	यदि (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	यदि (is_main_worktree(wt))
		die(_("'%s' is a main working tree"), av[0]);
	यदि (is_directory(dst.buf)) {
		ध्रुव वर्ण *sep = find_चरमः_dir_sep(wt->मार्गः);

		यदि (!sep)
			die(_("could not figure out destination name from '%s'"),
			    wt->मार्गः);
		strbuf_trim_trailing_dir_sep(&dst);
		strbuf_addstr(&dst, sep);
	}
	check_candidate_मार्गः(dst.buf, force, worktrees, "move");

	validate_no_submodules(wt);

	यदि (force < 2)
		reason = worktree_तालकम्_reason(wt);
	यदि (reason) {
		यदि (*reason)
			die(_("cannot move a locked working tree, lock reason: %s\nuse 'move -f -f' to override or unlock first"),
			    reason);
		die(_("cannot move a locked working tree;\nuse 'move -f -f' to override or unlock first"));
	}
	यदि (validate_worktree(wt, &errmsg, 0))
		die(_("validation failed, cannot move working tree: %s"),
		    errmsg.buf);
	strbuf_release(&errmsg);

	यदि (rename(wt->मार्गः, dst.buf) == -1)
		die_errno(_("failed to move '%s' to '%s'"), wt->मार्गः, dst.buf);

	update_worktree_location(wt, dst.buf, use_relative_paths);

	strbuf_release(&dst);
	free_worktrees(worktrees);
	प्रत्यागम 0;
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
				 ध्रुव वर्ण *original_मार्गः)
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
		     GIT_DIR_ENVIRONMENT, wt->मार्गः);
	strvec_pushf(&cp.env, "%s=%s",
		     GIT_WORK_वृक्षः_ENVIRONMENT, wt->मार्गः);
	strvec_pushl(&cp.तर्काः, "status",
		     "--porcelain", "--ignore-submodules=none",
		     रिक्त);
	cp.git_cmd = 1;
	cp.dir = wt->मार्गः;
	cp.बहिर्गमनम् = -1;
	ret = start_command(&cp);
	यदि (ret)
		die_errno(_("failed to run 'git status' on '%s'"),
			  original_मार्गः);
	ret = xread(cp.बहिर्गमनम्, buf, परिमाण(buf));
	यदि (ret)
		die(_("'%s' contains modified or untracked files, use --force to delete it"),
		    original_मार्गः);
	close(cp.बहिर्गमनम्);
	ret = finish_command(&cp);
	यदि (ret)
		die_errno(_("failed to run 'git status' on '%s', code %d"),
			  original_मार्गः, ret);
}

स्थिर पूर्णाङ्क delete_git_work_वृक्षः(संरचना worktree *wt)
{
	संरचना strbuf sb = STRBUF_INIT;
	पूर्णाङ्क ret = 0;

	strbuf_addstr(&sb, wt->मार्गः);
	यदि (remove_dir_recursively(&sb, 0)) {
		error_errno(_("failed to delete '%s'"), sb.buf);
		ret = -1;
	}
	strbuf_release(&sb);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क remove_worktree(पूर्णाङ्क ac, ध्रुव वर्ण **av, ध्रुव वर्ण *उपसर्गः,
			   संरचना repository *repo अप्रयुक्तम्)
{
	पूर्णाङ्क force = 0;
	संरचना option options[] = {
		OPT__FORCE(&force,
			 N_("force removal even if worktree is dirty or locked"),
			 PARSE_OPT_NOCOMPLETE),
		OPT_अन्तः()
	};
	संरचना worktree **worktrees, *wt;
	संरचना strbuf errmsg = STRBUF_INIT;
	ध्रुव वर्ण *reason = रिक्त;
	पूर्णाङ्क ret = 0;

	ac = parse_options(ac, av, उपसर्गः, options, git_worktree_remove_usage, 0);
	यदि (ac != 1)
		usage_with_options(git_worktree_remove_usage, options);

	worktrees = get_worktrees(the_repository);
	wt = find_worktree(worktrees, उपसर्गः, av[0]);
	यदि (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	यदि (is_main_worktree(wt))
		die(_("'%s' is a main working tree"), av[0]);
	यदि (force < 2)
		reason = worktree_तालकम्_reason(wt);
	यदि (reason) {
		यदि (*reason)
			die(_("cannot remove a locked working tree, lock reason: %s\nuse 'remove -f -f' to override or unlock first"),
			    reason);
		die(_("cannot remove a locked working tree;\nuse 'remove -f -f' to override or unlock first"));
	}
	यदि (validate_worktree(wt, &errmsg, WT_VALIDATE_WORKTREE_MISSING_OK))
		die(_("validation failed, cannot remove working tree: %s"),
		    errmsg.buf);
	strbuf_release(&errmsg);

	यदि (file_exists(wt->मार्गः)) {
		यदि (!force)
			check_clean_worktree(wt, av[0]);

		ret |= delete_git_work_वृक्षः(wt);
	}
	/*
	 * continue on even if ret is non-zero, there's no going back
	 * from here.
	 */
	ret |= delete_git_dir(wt->id);
	delete_worktrees_dir_if_रिक्तानि();

	free_worktrees(worktrees);
	प्रत्यागम ret;
}

स्थिर शून्य report_repair(पूर्णाङ्क iserr, ध्रुव वर्ण *मार्गः, ध्रुव वर्ण *सन्देशः, शून्य *cb_दत्त)
{
	यदि (!iserr) {
		fprintf_ln(stderr, _("repair: %s: %s"), सन्देशः, मार्गः);
	} अन्यथा {
		पूर्णाङ्क *exit_स्थितिः = (पूर्णाङ्क *)cb_दत्त;
		fprintf_ln(stderr, _("error: %s: %s"), सन्देशः, मार्गः);
		*exit_स्थितिः = 1;
	}
}

स्थिर पूर्णाङ्क repair(पूर्णाङ्क ac, ध्रुव वर्ण **av, ध्रुव वर्ण *उपसर्गः,
		  संरचना repository *repo अप्रयुक्तम्)
{
	ध्रुव वर्ण **p;
	ध्रुव वर्ण *self[] = { ".", रिक्त };
	संरचना option options[] = {
		OPT_BOOL(0, "relative-paths", &use_relative_paths,
			 N_("use relative paths for worktrees")),
		OPT_अन्तः()
	};
	पूर्णाङ्क rc = 0;

	ac = parse_options(ac, av, उपसर्गः, options, git_worktree_repair_usage, 0);
	p = ac > 0 ? av : self;
	प्रति (; *p; p++)
		repair_worktree_at_मार्गः(the_repository, *p, report_repair,
					&rc, use_relative_paths);
	repair_worktrees(the_repository, report_repair, &rc, use_relative_paths);
	प्रत्यागम rc;
}

पूर्णाङ्क cmd_worktree(पूर्णाङ्क ac,
		 ध्रुव वर्ण **av,
		 ध्रुव वर्ण *उपसर्गः,
		 संरचना repository *repo)
{
	parse_opt_subcommand_fn *fn = रिक्त;
	संरचना option options[] = {
		OPT_SUBCOMMAND("add", &fn, सङ्कलय),
		OPT_SUBCOMMAND("prune", &fn, prune),
		OPT_SUBCOMMAND("list", &fn, list),
		OPT_SUBCOMMAND("lock", &fn, तालकम्_worktree),
		OPT_SUBCOMMAND("unlock", &fn, unlock_worktree),
		OPT_SUBCOMMAND("move", &fn, move_worktree),
		OPT_SUBCOMMAND("remove", &fn, remove_worktree),
		OPT_SUBCOMMAND("repair", &fn, repair),
		OPT_अन्तः()
	};

	repo_config(the_repository, git_worktree_config, रिक्त);

	यदि (!उपसर्गः)
		उपसर्गः = "";

	ac = parse_options(ac, av, उपसर्गः, options, git_worktree_usage, 0);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_पूर्णम्_सूचकांक = 0;

	प्रत्यागम fn(ac, av, उपसर्गः, repo);
}
