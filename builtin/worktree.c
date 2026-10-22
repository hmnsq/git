#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#تعریف_کرو DISABLE_SIGN_COMPARE_WARNINGS

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

#تعریف_کرو BUILTIN_WORKTREE_جوڑو_USAGE \
	N_("git worktree add [-f] [--detach] [--checkout] [--lock [--reason <string>]]\n" \
	   "                 [--orphan] [(-b | -B) <new-branch>] <path> [<commit-ish>]")

#تعریف_کرو BUILTIN_WORKTREE_LIST_USAGE \
	N_("git worktree list [-v | --porcelain [-z]]")
#تعریف_کرو BUILTIN_WORKTREE_تالا_USAGE \
	N_("git worktree lock [--reason <string>] <worktree>")
#تعریف_کرو BUILTIN_WORKTREE_MOVE_USAGE \
	N_("git worktree move <worktree> <new-path>")
#تعریف_کرو BUILTIN_WORKTREE_PRUNE_USAGE \
	N_("git worktree prune [-n] [-v] [--expire <expire>]")
#تعریف_کرو BUILTIN_WORKTREE_REMOVE_USAGE \
	N_("git worktree remove [-f] <worktree>")
#تعریف_کرو BUILTIN_WORKTREE_REPAIR_USAGE \
	N_("git worktree repair [<path>...]")
#تعریف_کرو BUILTIN_WORKTREE_UNLOCK_USAGE \
	N_("git worktree unlock <worktree>")

#تعریف_کرو WORKTREE_جوڑو_DWIM_ORPHAN_INFER_متن \
	_("No possible source branch, inferring '--orphan'")

#تعریف_کرو WORKTREE_جوڑو_ORPHAN_WITH_DASH_B_HINT_متن \
	_("If you meant to create a worktree containing a new unborn branch\n" \
	"(branch with no commits) for this repository, you can do so\n" \
	"using the --orphan flag:\n" \
	"\n" \
	"    git worktree add --orphan -b %s %s\n")

#تعریف_کرو WORKTREE_جوڑو_ORPHAN_NO_DASH_B_HINT_متن \
	_("If you meant to create a worktree containing a new unborn branch\n" \
	"(branch with no commits) for this repository, you can do so\n" \
	"using the --orphan flag:\n" \
	"\n" \
	"    git worktree add --orphan %s\n")

ساکن ثابت حرف * ثابت git_worktree_usage[] = {
	BUILTIN_WORKTREE_جوڑو_USAGE,
	BUILTIN_WORKTREE_LIST_USAGE,
	BUILTIN_WORKTREE_تالا_USAGE,
	BUILTIN_WORKTREE_MOVE_USAGE,
	BUILTIN_WORKTREE_PRUNE_USAGE,
	BUILTIN_WORKTREE_REMOVE_USAGE,
	BUILTIN_WORKTREE_REPAIR_USAGE,
	BUILTIN_WORKTREE_UNLOCK_USAGE,
	تہی
};

ساکن ثابت حرف * ثابت git_worktree_جوڑو_usage[] = {
	BUILTIN_WORKTREE_جوڑو_USAGE,
	تہی,
};

ساکن ثابت حرف * ثابت git_worktree_list_usage[] = {
	BUILTIN_WORKTREE_LIST_USAGE,
	تہی
};

ساکن ثابت حرف * ثابت git_worktree_تالا_usage[] = {
	BUILTIN_WORKTREE_تالا_USAGE,
	تہی
};

ساکن ثابت حرف * ثابت git_worktree_move_usage[] = {
	BUILTIN_WORKTREE_MOVE_USAGE,
	تہی
};

ساکن ثابت حرف * ثابت git_worktree_prune_usage[] = {
	BUILTIN_WORKTREE_PRUNE_USAGE,
	تہی
};

ساکن ثابت حرف * ثابت git_worktree_remove_usage[] = {
	BUILTIN_WORKTREE_REMOVE_USAGE,
	تہی
};

ساکن ثابت حرف * ثابت git_worktree_repair_usage[] = {
	BUILTIN_WORKTREE_REPAIR_USAGE,
	تہی
};

ساکن ثابت حرف * ثابت git_worktree_unlock_usage[] = {
	BUILTIN_WORKTREE_UNLOCK_USAGE,
	تہی
};

ساخت جوڑو_opts {
	صحیح force;
	صحیح detach;
	صحیح quiet;
	صحیح checkout;
	صحیح orphan;
	صحیح relative_paths;
	ثابت حرف *keep_locked;
};

ساکن صحیح show_only;
ساکن صحیح verbose;
ساکن صحیح guess_remote;
ساکن صحیح use_relative_paths;
ساکن timestamp_t expire;

ساکن صحیح git_worktree_config(ثابت حرف *var, ثابت حرف *قدر,
			       ثابت ساخت config_context *ctx, خالی *cb)
{
	اگر (!strcmp(var, "worktree.guessremote")) {
		guess_remote = git_config_bool(var, قدر);
		واپس 0;
	} ورنہ اگر (!strcmp(var, "worktree.userelativepaths")) {
		use_relative_paths = git_config_bool(var, قدر);
		واپس 0;
	}

	واپس git_default_config(var, قدر, ctx, cb);
}

ساکن صحیح delete_git_dir(ثابت حرف *id)
{
	ساخت strbuf sb = STRBUF_INIT;
	صحیح ret;

	repo_common_راستہ_append(the_repository, &sb, "worktrees/%s", id);
	ret = remove_dir_recursively(&sb, 0);
	اگر (ret < 0 && errno == ENOTDIR)
		ret = unlink(sb.buf);
	اگر (ret)
		error_errno(_("failed to delete '%s'"), sb.buf);
	strbuf_release(&sb);
	واپس ret;
}

ساکن خالی delete_worktrees_dir_if_خالی_خانے(خالی)
{
	حرف *راستہ = repo_git_راستہ(the_repository, "worktrees");
	rmdir(راستہ); /* ignore failed removal */
	آزاد(راستہ);
}

ساکن خالی prune_worktree(ثابت حرف *id, ثابت حرف *reason)
{
	اگر (show_only || verbose)
		fprintf_ln(stderr, _("Removing %s/%s: %s"), "worktrees", id, reason);
	اگر (!show_only)
		delete_git_dir(id);
}

ساکن صحیح prune_cmp(ثابت خالی *a, ثابت خالی *b)
{
	ثابت ساخت string_list_شے *x = a;
	ثابت ساخت string_list_شے *y = b;
	صحیح c;

	اگر ((c = fspathcmp(x->string, y->string)))
	    واپس c;
	/*
	 * paths same; prune_dupes() removes all but the first worktree entry
	 * having the same path, so sort main worktree ('util' is NULL) above
	 * linked worktrees ('util' not NULL) since main worktree can't be
	 * removed
	 */
	اگر (!x->util)
		واپس -1;
	اگر (!y->util)
		واپس 1;
	/* paths same; sort by .git/worktrees/<id> */
	واپس strcmp(x->util, y->util);
}

ساکن خالی prune_dups(ساخت string_list *l)
{
	صحیح i;

	QSORT(l->اشیاء, l->nr, prune_cmp);
	برائے (i = 1; i < l->nr; i++) {
		اگر (!fspathcmp(l->اشیاء[i].string, l->اشیاء[i - 1].string))
			prune_worktree(l->اشیاء[i].util, "duplicate entry");
	}
}

ساکن خالی prune_worktrees(خالی)
{
	ساخت strbuf reason = STRBUF_INIT;
	ساخت strbuf main_راستہ = STRBUF_INIT;
	ساخت string_list kept = STRING_LIST_INIT_DUP;
	حرف *راستہ;
	DIR *dir;
	ساخت dirent *d;

	راستہ = repo_git_راستہ(the_repository, "worktrees");
	dir = opendir(راستہ);
	آزاد(راستہ);
	اگر (!dir)
		واپس;
	جب ((d = readdir_skip_dot_and_dotdot(dir)) != تہی) {
		حرف *راستہ;
		strbuf_reset(&reason);
		اگر (should_prune_worktree(the_repository, d->d_name, &reason, &راستہ, expire))
			prune_worktree(d->d_name, reason.buf);
		ورنہ اگر (راستہ)
			string_list_append_nodup(&kept, راستہ)->util = xstrdup(d->d_name);
	}
	closedir(dir);

	strbuf_جوڑو_absolute_راستہ(&main_راستہ, repo_get_common_dir(the_repository));
	/* massage main worktree absolute path to match 'gitdir' content */
	strbuf_strip_suffix(&main_راستہ, "/.");
	string_list_append_nodup(&kept, strbuf_detach(&main_راستہ, تہی));
	prune_dups(&kept);
	string_list_clear(&kept, 1);

	اگر (!show_only)
		delete_worktrees_dir_if_خالی_خانے();
	strbuf_release(&reason);
}

ساکن صحیح prune(صحیح ac, ثابت حرف **av, ثابت حرف *سابقہ,
		 ساخت repository *repo غیر_مستعمل)
{
	ساخت option options[] = {
		OPT_DRY_تسلسل(&show_only, N_("do not remove, show only")),
		OPT__VERBOSE(&verbose, N_("report pruned working trees")),
		OPT_EXPIRY_DATE(0, "expire", &expire,
				N_("prune missing working trees older than <time>")),
		OPT_انتہا()
	};

	expire = وقت_MAX;
	ac = parse_options(ac, av, سابقہ, options, git_worktree_prune_usage,
			   0);
	اگر (ac)
		usage_with_options(git_worktree_prune_usage, options);
	prune_worktrees();
	واپس 0;
}

ساکن حرف *junk_work_درخت;
ساکن حرف *junk_git_dir;
ساکن صحیح is_junk;
ساکن pid_t junk_pid;

ساکن خالی remove_junk(خالی)
{
	ساخت strbuf sb = STRBUF_INIT;
	اگر (!is_junk || getpid() != junk_pid)
		واپس;
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

ساکن خالی remove_junk_on_signal(صحیح signo)
{
	remove_junk();
	sigchain_نکالو(signo);
	اٹھاؤ(signo);
}

ساکن حرف *worktree_basename_dup(ثابت حرف *راستہ)
{
	ثابت حرف *name;
	صحیح len;

	len = لڑی_لمبائی(راستہ);
	جب (len && is_dir_sep(راستہ[len - 1]))
		len--;

	name = راستہ + len;
	جب (name > راستہ && !is_dir_sep(name[-1]))
		name--;

	واپس xmemdupz(name, راستہ + len - name);
}

/* check that path is viable location for worktree */
ساکن خالی check_candidate_راستہ(ثابت حرف *راستہ,
				 صحیح force,
				 ساخت worktree **worktrees,
				 ثابت حرف *cmd)
{
	ساخت worktree *wt;
	صحیح locked;

	اگر (file_exists(راستہ) && !is_خالی_خانے_dir(راستہ))
		die(_("'%s' already exists"), راستہ);

	wt = ڈھونڈو_worktree_by_راستہ(worktrees, راستہ);
	اگر (!wt)
		واپس;

	locked = !!worktree_تالا_reason(wt);
	اگر ((!locked && force) || (locked && force > 1)) {
		اگر (delete_git_dir(wt->id))
		    die(_("unusable worktree destination '%s'"), راستہ);
		واپس;
	}

	اگر (locked)
		die(_("'%s' is a missing but locked worktree;\nuse '%s -f -f' to override, or 'unlock' and 'prune' or 'remove' to clear"), راستہ, cmd);
	ورنہ
		die(_("'%s' is a missing but already registered worktree;\nuse '%s -f' to override, or 'prune' or 'remove' to clear"), راستہ, cmd);
}

ساکن خالی copy_sparse_checkout(ثابت حرف *worktree_git_dir)
{
	حرف *from_file = repo_git_راستہ(the_repository, "info/sparse-checkout");
	حرف *مقصد_file = xstrfmt("%s/info/sparse-checkout", worktree_git_dir);

	اگر (file_exists(from_file)) {
		اگر (محفوظ_create_leading_directories(the_repository, مقصد_file) ||
			copy_file(the_repository, مقصد_file, from_file, 0666))
			error(_("failed to copy '%s' to '%s'; sparse-checkout may not work correctly"),
				from_file, مقصد_file);
	}

	آزاد(from_file);
	آزاد(مقصد_file);
}

ساکن خالی copy_filtered_worktree_config(ثابت حرف *worktree_git_dir)
{
	حرف *from_file = repo_git_راستہ(the_repository, "config.worktree");
	حرف *مقصد_file = xstrfmt("%s/config.worktree", worktree_git_dir);

	اگر (file_exists(from_file)) {
		ساخت config_سیٹ cs = { { 0 } };
		صحیح bare;

		اگر (محفوظ_create_leading_directories(the_repository, مقصد_file) ||
			copy_file(the_repository, مقصد_file, from_file, 0666)) {
			error(_("failed to copy worktree config from '%s' to '%s'"),
				from_file, مقصد_file);
			جاؤ worktree_copy_cleanup;
		}

		git_configset_init(&cs);
		git_configset_جوڑو_file(&cs, from_file);

		اگر (!git_configset_get_bool(&cs, "core.bare", &bare) &&
			bare &&
			repo_config_سیٹ_multivar_اندر_file_gently(the_repository,
				مقصد_file, "core.bare", تہی, "true", تہی, 0))
			error(_("failed to unset '%s' in '%s'"),
				"core.bare", مقصد_file);
		اگر (!git_configset_get(&cs, "core.worktree") &&
			repo_config_سیٹ_اندر_file_gently(the_repository, مقصد_file,
						       "core.worktree", تہی, تہی))
			error(_("failed to unset '%s' in '%s'"),
				"core.worktree", مقصد_file);

		git_configset_clear(&cs);
	}

worktree_copy_cleanup:
	آزاد(from_file);
	آزاد(مقصد_file);
}

ساکن صحیح checkout_worktree(ثابت ساخت جوڑو_opts *opts,
			     ساخت strvec *بچہ_env)
{
	ساخت بچہ_process cp = بچہ_PROCESS_INIT;
	cp.git_cmd = 1;
	strvec_pushl(&cp.دلائل, "reset", "--hard", "--no-recurse-submodules", تہی);
	اگر (opts->quiet)
		strvec_ڈالو(&cp.دلائل, "--quiet");
	strvec_pushv(&cp.env, بچہ_env->v);
	واپس تسلسل_command(&cp);
}

ساکن صحیح make_worktree_orphan(ثابت حرف * ref, ثابت ساخت جوڑو_opts *opts,
				ساخت strvec *بچہ_env)
{
	ساخت strbuf symref = STRBUF_INIT;
	ساخت بچہ_process cp = بچہ_PROCESS_INIT;

	validate_new_branchname(ref, &symref, 0);
	strvec_pushl(&cp.دلائل, "symbolic-ref", "HEAD", symref.buf, تہی);
	اگر (opts->quiet)
		strvec_ڈالو(&cp.دلائل, "--quiet");
	strvec_pushv(&cp.env, بچہ_env->v);
	strbuf_release(&symref);
	cp.git_cmd = 1;
	واپس تسلسل_command(&cp);
}

/*
 * References for worktrees are generally stored in '$GIT_DIR/worktrees/<wt_id>'.
 * But when using alternate reference directories, we want to store the worktree
 * references in '$ALTERNATE_REFERENCE_DIR/worktrees/<wt_id>'.
 *
 * Create the necessary folder structure to facilitate the same. But to ensure
 * that the former path is still considered a Git directory, add stubs.
 */
ساکن خالی تیاری_alternate_ref_dir(ساخت worktree *wt, ثابت حرف *wt_git_راستہ)
{
	ساخت strbuf sb = STRBUF_INIT;
	حرف *راستہ;

	راستہ = wt->repo->ref_storage_payload;
	اگر (!راستہ)
		واپس;

	اگر (!is_absolute_راستہ(راستہ))
		strbuf_addf(&sb, "%s/", wt->repo->commondir);

	strbuf_addf(&sb, "%s/worktrees", راستہ);
	محفوظ_create_dir(wt->repo, sb.buf, 1);
	strbuf_addf(&sb, "/%s", wt->id);
	محفوظ_create_dir(wt->repo, sb.buf, 1);
	strbuf_reset(&sb);

	strbuf_addf(&sb, "this worktree stores references in %s/worktrees/%s",
		    راستہ, wt->id);
	refs_create_refdir_stubs(wt->repo, wt_git_راستہ, sb.buf);

	strbuf_release(&sb);
}

ساکن صحیح جوڑو_worktree(ثابت حرف *راستہ, ثابت حرف *refname,
			ثابت ساخت جوڑو_opts *opts)
{
	ساخت strbuf sb_git = STRBUF_INIT, sb_repo = STRBUF_INIT;
	ساخت strbuf sb = STRBUF_INIT;
	ثابت حرف *name;
	حرف *name_مقصد_free = تہی;
	ساخت strvec بچہ_env = STRVEC_INIT;
	غیرعلامتی صحیح شمار_2 = 0;
	صحیح len, ret;
	ساخت strbuf symref = STRBUF_INIT;
	ساخت commit *commit = تہی;
	صحیح is_branch = 0;
	ساخت strbuf sb_name = STRBUF_INIT;
	ساخت worktree **worktrees, *wt = تہی;
	ساخت ref_store *wt_refs;
	ساخت repo_config_اقدار *cfg = repo_config_اقدار(the_repository);

	worktrees = get_worktrees(the_repository);
	check_candidate_راستہ(راستہ, opts->force, worktrees, "add");
	free_worktrees(worktrees);
	worktrees = تہی;

	/* is 'refname' a branch or commit? */
	اگر (!opts->detach && !check_branch_ref(the_repository, &symref, refname) &&
	    refs_ref_exists(get_main_ref_store(the_repository), symref.buf)) {
		is_branch = 1;
		اگر (!opts->force)
			die_if_checked_برآمد(symref.buf, 0);
	}
	commit = lookup_commit_reference_by_name(refname);
	اگر (!commit && !opts->orphan)
		die(_("invalid reference: %s"), refname);

	name = name_مقصد_free = worktree_basename_dup(راستہ);
	اگر (!*name)
		die(_("invalid path '%s'"), راستہ);
	sanitize_refname_component(name, &sb_name);
	اگر (!sb_name.len)
		BUG("How come '%s' becomes empty after sanitization?", name);
	name = sb_name.buf;
	repo_git_راستہ_replace(the_repository, &sb_repo, "worktrees/%s", name);
	len = sb_repo.len;
	اگر (محفوظ_create_leading_directories_const(the_repository, sb_repo.buf))
		die_errno(_("could not create leading directories of '%s'"),
			  sb_repo.buf);

	جب (mkdir(sb_repo.buf, 0777)) {
		شمار_2++;
		اگر ((errno != EEXIST) || !شمار_2 /*حد_سے_تجاوز*/)
			die_errno(_("could not create directory of '%s'"),
				  sb_repo.buf);
		strbuf_setlen(&sb_repo, len);
		strbuf_addf(&sb_repo, "%d", شمار_2);
	}
	name = strrchr(sb_repo.buf, '/') + 1;

	junk_pid = getpid();
	atexit(remove_junk);
	sigchain_ڈالو_common(remove_junk_on_signal);

	junk_git_dir = xstrdup(sb_repo.buf);
	is_junk = 1;

	/*
	 * lock the incomplete repo so prune won't delete it, unlock
	 * after the preparation is over.
	 */
	strbuf_addf(&sb, "%s/locked", sb_repo.buf);
	اگر (opts->keep_locked)
		write_file(sb.buf, "%s", opts->keep_locked);
	ورنہ
		write_file(sb.buf, _("initializing"));

	strbuf_addf(&sb_git, "%s/.git", راستہ);
	اگر (محفوظ_create_leading_directories_const(the_repository, sb_git.buf))
		die_errno(_("could not create leading directories of '%s'"),
			  sb_git.buf);
	junk_work_درخت = xstrdup(راستہ);

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
		جاؤ مکمل;
	}
	تیاری_alternate_ref_dir(wt, sb_repo.buf);
	wt_refs = get_worktree_ref_store(wt);

	ret = ref_store_create_on_disk(wt_refs, REF_STORE_CREATE_ON_DISK_IS_WORKTREE, &sb);
	اگر (ret)
		جاؤ مکمل;

	اگر (!is_branch && commit)
		ret = refs_update_ref(wt_refs, تہی, "HEAD", &commit->object.oid,
				      تہی, 0, UPDATE_REFS_پیغام_ON_ERR);
	ورنہ
		ret = refs_update_symref(wt_refs, "HEAD", symref.buf, تہی);
	اگر (ret)
		جاؤ مکمل;

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

	strvec_pushf(&بچہ_env, "%s=%s", GIT_DIR_ENVIRONMENT, sb_git.buf);
	strvec_pushf(&بچہ_env, "%s=%s", GIT_WORK_درخت_ENVIRONMENT, راستہ);

	اگر (opts->orphan &&
	    (ret = make_worktree_orphan(refname, opts, &بچہ_env)))
		جاؤ مکمل;

	اگر (opts->checkout &&
	    (ret = checkout_worktree(opts, &بچہ_env)))
		جاؤ مکمل;

	is_junk = 0;
	FREE_AND_NULL(junk_work_درخت);
	FREE_AND_NULL(junk_git_dir);

مکمل:
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
		ساخت تسلسل_hooks_opt opt = تسلسل_HOOKS_OPT_INIT_FORCE_SERIAL;

		strvec_pushl(&opt.env, "GIT_DIR", "GIT_WORK_TREE", تہی);
		strvec_pushl(&opt.دلائل,
			     oid_مقصد_hex(null_oid(the_hash_algo)),
			     oid_مقصد_hex(&commit->object.oid),
			     "1",
			     تہی);
		opt.dir = راستہ;

		ret = تسلسل_hooks_opt(the_repository, "post-checkout", &opt);
	}

	strvec_clear(&بچہ_env);
	strbuf_release(&sb);
	strbuf_release(&symref);
	strbuf_release(&sb_repo);
	strbuf_release(&sb_git);
	strbuf_release(&sb_name);
	free_worktree(wt);
	آزاد(name_مقصد_free);
	واپس ret;
}

ساکن خالی print_preparing_worktree_سطر(صحیح detach,
					  ثابت حرف *branch,
					  ثابت حرف *new_branch,
					  صحیح force_new_branch)
{
	اگر (force_new_branch) {
		ساخت commit *commit = lookup_commit_reference_by_name(new_branch);
		اگر (!commit)
			fprintf_ln(stderr, _("Preparing worktree (new branch '%s')"), new_branch);
		ورنہ
			fprintf_ln(stderr, _("Preparing worktree (resetting branch '%s'; was at %s)"),
				  new_branch,
				  repo_ڈھونڈو_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV));
	} ورنہ اگر (new_branch) {
		fprintf_ln(stderr, _("Preparing worktree (new branch '%s')"), new_branch);
	} ورنہ {
		ساخت strbuf s = STRBUF_INIT;
		اگر (!detach && !check_branch_ref(the_repository, &s, branch) &&
		    refs_ref_exists(get_main_ref_store(the_repository), s.buf))
			fprintf_ln(stderr, _("Preparing worktree (checking out '%s')"),
				  branch);
		ورنہ {
			ساخت commit *commit = lookup_commit_reference_by_name(branch);
			اگر (!commit)
				BUG("unreachable: invalid reference: %s", branch);
			fprintf_ln(stderr, _("Preparing worktree (detached HEAD %s)"),
				  repo_ڈھونڈو_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV));
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
ساکن صحیح پہلا_valid_ref(ثابت ساخت reference *ref غیر_مستعمل, خالی *cb_ڈیٹا غیر_مستعمل)
{
	واپس 1;
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
ساکن صحیح can_use_local_refs(ثابت ساخت جوڑو_opts *opts)
{
	اگر (refs_سر_ref(get_main_ref_store(the_repository), پہلا_valid_ref, تہی)) {
		واپس 1;
	} ورنہ اگر (refs_for_each_branch_ref(get_main_ref_store(the_repository), پہلا_valid_ref, تہی)) {
		اگر (!opts->quiet)
			warning(_("HEAD points to an invalid (or orphaned) reference.\n"));
		واپس 1;
	}
	واپس 0;
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
ساکن صحیح can_use_remote_refs(ثابت ساخت جوڑو_opts *opts)
{
	اگر (!guess_remote) {
		واپس 0;
	} ورنہ اگر (refs_for_each_remote_ref(get_main_ref_store(the_repository), پہلا_valid_ref, تہی)) {
		واپس 1;
	} ورنہ اگر (!opts->force && remote_get(تہی)) {
		die(_("No local or remote refs exist despite at least one remote\n"
		      "present, stopping; use 'add -f' to override or fetch a remote first"));
	}
	واپس 0;
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
ساکن صحیح dwim_orphan(ثابت ساخت جوڑو_opts *opts, صحیح opt_track, صحیح remote)
{
	اگر (can_use_local_refs(opts)) {
		واپس 0;
	} ورنہ اگر (remote && can_use_remote_refs(opts)) {
		واپس 0;
	} ورنہ اگر (!opts->quiet) {
		fprintf_ln(stderr, WORKTREE_جوڑو_DWIM_ORPHAN_INFER_متن);
	}

	اگر (opt_track) {
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--track");
	} ورنہ اگر (!opts->checkout) {
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--no-checkout");
	}
	واپس 1;
}

ساکن خالی advise_disambiguating_remotes(ثابت حرف *راستہ, ثابت حرف *branch,
					  ثابت ساخت string_list *matched_remote_نام)
{
	ساخت string_list_شے *شے;

	advise(_("Branch name '%s' appears in multiple remotes:"), branch);
	for_each_string_list_شے(شے, matched_remote_نام) {
		advise(_("  %s"), شے->string);
	}
	advise(_("If you meant to create a worktree from a remote tracking branch on\n"
		 "<remote>, you can do so by:\n"
		 "\n"
		 "    git worktree add -b %s %s <remote>/%s\n"
		 "\n"
		 "If you'd like to always prefer some remote, e.g. 'origin',\n"
		 "consider setting checkout.defaultRemote=origin in your config."),
	       branch, راستہ, branch);
}

ساکن حرف *dwim_branch(ثابت ساخت جوڑو_opts *opts, ثابت حرف *راستہ, حرف **new_branch)
{
	صحیح branch_exists;
	حرف *branchname = worktree_basename_dup(راستہ);
	ساخت strbuf ref = STRBUF_INIT;

	branch_exists = !check_branch_ref(the_repository, &ref, branchname) &&
			refs_ref_exists(get_main_ref_store(the_repository),
					ref.buf);
	strbuf_release(&ref);
	اگر (branch_exists)
		واپس branchname;

	*new_branch = branchname;
	اگر (guess_remote) {
		ساخت object_id oid;
		حرف *remote;
		صحیح عدد_matches = 0;
		ساخت string_list matched_remote_نام = STRING_LIST_INIT_DUP;

		remote = unique_tracking_name(*new_branch, &oid, &عدد_matches,
					      &matched_remote_نام);
		اگر (!remote && عدد_matches > 1) {
			اگر (!opts->quiet &&
			    advice_enabled(ADVICE_CHECKOUT_AMBIGUOUS_REMOTE_BRANCH_NAME))
				advise_disambiguating_remotes(راستہ, *new_branch,
							      &matched_remote_نام);
			die(_("'%s' matched multiple (%d) remote tracking branches"),
			    *new_branch, عدد_matches);
		}
		string_list_clear(&matched_remote_نام, 0);
		واپس remote;
	}
	واپس تہی;
}

ساکن صحیح جوڑو(صحیح ac, ثابت حرف **av, ثابت حرف *سابقہ,
	       ساخت repository *repo غیر_مستعمل)
{
	ساخت جوڑو_opts opts;
	ثابت حرف *new_branch_force = تہی;
	حرف *راستہ;
	ثابت حرف *branch;
	حرف *branch_مقصد_free = تہی;
	حرف *new_branch_مقصد_free = تہی;
	ثابت حرف *new_branch = تہی;
	حرف *opt_track = تہی;
	ثابت حرف *تالا_reason = تہی;
	صحیح keep_locked = 0;
	صحیح مستعمل_new_branch_options;
	ساخت option options[] = {
		OPT__FORCE(&opts.force,
			   N_("checkout <branch> even if already checked out in other worktree"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_STRING('b', تہی, &new_branch, N_("branch"),
			   N_("create a new branch")),
		OPT_STRING('B', تہی, &new_branch_force, N_("branch"),
			   N_("create or reset a branch")),
		OPT_BOOL(0, "orphan", &opts.orphan, N_("create unborn branch")),
		OPT_BOOL('d', "detach", &opts.detach, N_("detach HEAD at named commit")),
		OPT_BOOL(0, "checkout", &opts.checkout, N_("populate the new working tree")),
		OPT_BOOL(0, "lock", &keep_locked, N_("keep the new working tree locked")),
		OPT_STRING(0, "reason", &تالا_reason, N_("string"),
			   N_("reason for locking")),
		OPT__QUIET(&opts.quiet, N_("suppress progress reporting")),
		OPT_PASSTHRU(0, "track", &opt_track, تہی,
			     N_("set up tracking mode (see git-branch(1))"),
			     PARSE_OPT_NOARG | PARSE_OPT_OPTARG),
		OPT_BOOL(0, "guess-remote", &guess_remote,
			 N_("try to match the new branch name with a remote-tracking branch")),
		OPT_BOOL(0, "relative-paths", &opts.relative_paths,
			 N_("use relative paths for worktrees")),
		OPT_انتہا()
	};
	صحیح ret;

	memset(&opts, 0, حجم(opts));
	opts.checkout = 1;
	opts.relative_paths = use_relative_paths;
	ac = parse_options(ac, av, سابقہ, options, git_worktree_جوڑو_usage, 0);
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
	اگر (تالا_reason && !keep_locked)
		die(_("the option '%s' requires '%s'"), "--reason", "--lock");
	اگر (تالا_reason)
		opts.keep_locked = تالا_reason;
	ورنہ اگر (keep_locked)
		opts.keep_locked = _("added with --lock");

	اگر (ac < 1 || ac > 2)
		usage_with_options(git_worktree_جوڑو_usage, options);

	راستہ = سابقہ_filename(سابقہ, av[0]);
	branch = ac < 2 ? "HEAD" : av[1];
	مستعمل_new_branch_options = new_branch || new_branch_force;

	اگر (!strcmp(branch, "-"))
		branch = "@{-1}";

	اگر (new_branch_force) {
		ساخت strbuf symref = STRBUF_INIT;

		new_branch = new_branch_force;

		اگر (!opts.force &&
		    !check_branch_ref(the_repository, &symref, new_branch) &&
		    refs_ref_exists(get_main_ref_store(the_repository), symref.buf))
			die_if_checked_برآمد(symref.buf, 0);
		strbuf_release(&symref);
	}

	اگر (opts.orphan && !new_branch) {
		new_branch = new_branch_مقصد_free = worktree_basename_dup(راستہ);
	} ورنہ اگر (opts.orphan) {
		; /* no-op */
	} ورنہ اگر (opts.detach) {
		/* Check HEAD */
		اگر (!strcmp(branch, "HEAD"))
			can_use_local_refs(&opts);
	} ورنہ اگر (ac < 2 && new_branch) {
		/* DWIM: Infer --orphan when repo has no refs. */
		opts.orphan = dwim_orphan(&opts, !!opt_track, 0);
	} ورنہ اگر (ac < 2) {
		/* DWIM: Guess branch name from path. */
		حرف *s = dwim_branch(&opts, راستہ, &new_branch_مقصد_free);
		اگر (s)
			branch = branch_مقصد_free = s;
		new_branch = new_branch_مقصد_free;

		/* DWIM: Infer --orphan when repo has no refs. */
		opts.orphan = (!s) && dwim_orphan(&opts, !!opt_track, 1);
	} ورنہ اگر (ac == 2 && new_branch) {
		اگر (!strcmp(branch, "HEAD"))
			can_use_local_refs(&opts);
	} ورنہ اگر (ac == 2) {
		ساخت commit *commit;

		commit = lookup_commit_reference_by_name(branch);
		اگر (!commit) {
			ساخت object_id oid;
			حرف *remote;
			صحیح عدد_matches = 0;
			ساخت string_list matched_remote_نام = STRING_LIST_INIT_DUP;

			remote = unique_tracking_name(branch, &oid, &عدد_matches,
						      &matched_remote_نام);
			اگر (remote) {
				new_branch = branch;
				branch = new_branch_مقصد_free = remote;
			} ورنہ اگر (عدد_matches > 1) {
				اگر (!opts.quiet &&
				    advice_enabled(ADVICE_CHECKOUT_AMBIGUOUS_REMOTE_BRANCH_NAME))
					advise_disambiguating_remotes(راستہ, branch,
								      &matched_remote_نام);
				die(_("'%s' matched multiple (%d) remote tracking branches"),
				    branch, عدد_matches);
			}
			string_list_clear(&matched_remote_نام, 0);
		}

		اگر (!strcmp(branch, "HEAD"))
			can_use_local_refs(&opts);

	}

	اگر (!opts.orphan && !lookup_commit_reference_by_name(branch)) {
		صحیح attempt_hint = !opts.quiet && (ac < 2);
		اگر (attempt_hint && مستعمل_new_branch_options) {
			advise_if_enabled(ADVICE_WORKTREE_جوڑو_ORPHAN,
				WORKTREE_جوڑو_ORPHAN_WITH_DASH_B_HINT_متن,
				new_branch, راستہ);
		} ورنہ اگر (attempt_hint) {
			advise_if_enabled(ADVICE_WORKTREE_جوڑو_ORPHAN,
				WORKTREE_جوڑو_ORPHAN_NO_DASH_B_HINT_متن, راستہ);
		}
		die(_("invalid reference: %s"), branch);
	}

	اگر (!opts.quiet)
		print_preparing_worktree_سطر(opts.detach, branch, new_branch, !!new_branch_force);

	اگر (opts.orphan) {
		branch = new_branch;
	} ورنہ اگر (new_branch) {
		ساخت بچہ_process cp = بچہ_PROCESS_INIT;
		cp.git_cmd = 1;
		strvec_ڈالو(&cp.دلائل, "branch");
		اگر (new_branch_force)
			strvec_ڈالو(&cp.دلائل, "--force");
		اگر (opts.quiet)
			strvec_ڈالو(&cp.دلائل, "--quiet");
		strvec_ڈالو(&cp.دلائل, new_branch);
		strvec_ڈالو(&cp.دلائل, branch);
		اگر (opt_track)
			strvec_ڈالو(&cp.دلائل, opt_track);
		اگر (تسلسل_command(&cp)) {
			ret = -1;
			جاؤ cleanup;
		}
		branch = new_branch;
	} ورنہ اگر (opt_track) {
		die(_("--[no-]track can only be used if a new branch is created"));
	}

	ret = جوڑو_worktree(راستہ, branch, &opts);
cleanup:
	آزاد(راستہ);
	آزاد(opt_track);
	آزاد(branch_مقصد_free);
	آزاد(new_branch_مقصد_free);
	واپس ret;
}

ساکن خالی show_worktree_porcelain(ساخت worktree *wt, صحیح سطر_terminator)
{
	ثابت حرف *reason;

	لکھو("worktree %s%c", wt->راستہ, سطر_terminator);
	اگر (wt->is_bare)
		لکھو("bare%c", سطر_terminator);
	ورنہ {
		لکھو("HEAD %s%c", oid_مقصد_hex(&wt->سر_oid), سطر_terminator);
		اگر (wt->is_detached)
			لکھو("detached%c", سطر_terminator);
		ورنہ اگر (wt->سر_ref)
			لکھو("branch %s%c", wt->سر_ref, سطر_terminator);
	}

	reason = worktree_تالا_reason(wt);
	اگر (reason) {
		fputs("locked", stdout);
		اگر (*reason) {
			fputc(' ', stdout);
			write_name_quoted(reason, stdout, سطر_terminator);
		} ورنہ {
			fputc(سطر_terminator, stdout);
		}
	}

	reason = worktree_prune_reason(wt, expire);
	اگر (reason)
		لکھو("prunable %s%c", reason, سطر_terminator);

	fputc(سطر_terminator, stdout);
}

ساخت worktree_display {
	حرف *راستہ;
	صحیح چوڑائی;
};

ساکن خالی show_worktree(ساخت worktree *wt, ساخت worktree_display *display,
			  صحیح راستہ_maxwidth, صحیح abbrev_len)
{
	ساخت strbuf sb = STRBUF_INIT;
	ثابت حرف *reason;

	strbuf_addf(&sb, "%s%*s", display->راستہ, 1 + راستہ_maxwidth - display->چوڑائی, "");
	اگر (wt->is_bare)
		strbuf_addstr(&sb, "(bare)");
	ورنہ {
		strbuf_addf(&sb, "%-*s ", abbrev_len,
				repo_ڈھونڈو_unique_abbrev(the_repository, &wt->سر_oid, DEFAULT_ABBREV));
		اگر (wt->is_detached)
			strbuf_addstr(&sb, "(detached HEAD)");
		ورنہ اگر (wt->سر_ref) {
			حرف *ref = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
								 wt->سر_ref,
								 0);
			strbuf_addf(&sb, "[%s]", ref);
			آزاد(ref);
		} ورنہ
			strbuf_addstr(&sb, "(error)");
	}

	reason = worktree_تالا_reason(wt);
	اگر (verbose && reason && *reason)
		strbuf_addf(&sb, "\n\tlocked: %s", reason);
	ورنہ اگر (reason)
		strbuf_addstr(&sb, " locked");

	reason = worktree_prune_reason(wt, expire);
	اگر (verbose && reason)
		strbuf_addf(&sb, "\n\tprunable: %s", reason);
	ورنہ اگر (reason)
		strbuf_addstr(&sb, " prunable");

	لکھو("%s\n", sb.buf);
	strbuf_release(&sb);
}

ساکن خالی measure_widths(ساخت worktree **wt, صحیح *abbrev,
			   ساخت worktree_display **d, صحیح *maxwidth)
{
	صحیح i, display_alloc = 0;
	ساخت worktree_display *display = تہی;
	ساخت strbuf buf = STRBUF_INIT;

	برائے (i = 0; wt[i]; i++) {
		صحیح sha1_len;
		ALLOC_GROW(display, i + 1, display_alloc);
		quote_راستہ(wt[i]->راستہ, تہی, &buf, 0);
		display[i].چوڑائی = utf8_strwidth(buf.buf);
		display[i].راستہ = strbuf_detach(&buf, تہی);

		اگر (display[i].چوڑائی > *maxwidth)
			*maxwidth = display[i].چوڑائی;
		sha1_len = لڑی_لمبائی(repo_ڈھونڈو_unique_abbrev(the_repository, &wt[i]->سر_oid, *abbrev));
		اگر (sha1_len > *abbrev)
			*abbrev = sha1_len;
	}
	*d = display;
}

ساکن صحیح pathcmp(ثابت خالی *a_, ثابت خالی *b_)
{
	ثابت ساخت worktree *ثابت *a = a_;
	ثابت ساخت worktree *ثابت *b = b_;
	واپس fspathcmp((*a)->راستہ, (*b)->راستہ);
}

ساکن خالی pathsort(ساخت worktree **wt)
{
	صحیح n = 0;
	ساخت worktree **p = wt;

	جب (*p++)
		n++;
	QSORT(wt, n, pathcmp);
}

ساکن صحیح list(صحیح ac, ثابت حرف **av, ثابت حرف *سابقہ,
		ساخت repository *repo غیر_مستعمل)
{
	صحیح porcelain = 0;
	صحیح سطر_terminator = '\n';

	ساخت option options[] = {
		OPT_BOOL(0, "porcelain", &porcelain, N_("machine-readable output")),
		OPT__VERBOSE(&verbose, N_("show extended annotations and reasons, if available")),
		OPT_EXPIRY_DATE(0, "expire", &expire,
				N_("add 'prunable' annotation to missing worktrees older than <time>")),
		OPT_سیٹ_INT('z', تہی, &سطر_terminator,
			    N_("terminate records with a NUL character"), '\0'),
		OPT_انتہا()
	};

	expire = وقت_MAX;
	ac = parse_options(ac, av, سابقہ, options, git_worktree_list_usage, 0);
	اگر (ac)
		usage_with_options(git_worktree_list_usage, options);
	ورنہ اگر (verbose && porcelain)
		die(_("options '%s' and '%s' cannot be used together"), "--verbose", "--porcelain");
	ورنہ اگر (!سطر_terminator && !porcelain)
		die(_("the option '%s' requires '%s'"), "-z", "--porcelain");
	ورنہ {
		ساخت worktree **worktrees = get_worktrees(the_repository);
		صحیح راستہ_maxwidth = 0, abbrev = DEFAULT_ABBREV, i;
		ساخت worktree_display *display = تہی;

		/* sort worktrees by path but keep main worktree at top */
		pathsort(worktrees + 1);

		اگر (!porcelain)
			measure_widths(worktrees, &abbrev,
				       &display, &راستہ_maxwidth);

		برائے (i = 0; worktrees[i]; i++) {
			اگر (porcelain)
				show_worktree_porcelain(worktrees[i],
							سطر_terminator);
			ورنہ
				show_worktree(worktrees[i],
					      &display[i], راستہ_maxwidth, abbrev);
		}
		برائے (i = 0; display && worktrees[i]; i++)
			آزاد(display[i].راستہ);
		آزاد(display);
		free_worktrees(worktrees);
	}
	واپس 0;
}

ساکن صحیح تالا_worktree(صحیح ac, ثابت حرف **av, ثابت حرف *سابقہ,
			 ساخت repository *repo غیر_مستعمل)
{
	ثابت حرف *reason = "", *old_reason;
	ساخت option options[] = {
		OPT_STRING(0, "reason", &reason, N_("string"),
			   N_("reason for locking")),
		OPT_انتہا()
	};
	ساخت worktree **worktrees, *wt;
	حرف *راستہ;

	ac = parse_options(ac, av, سابقہ, options, git_worktree_تالا_usage, 0);
	اگر (ac != 1)
		usage_with_options(git_worktree_تالا_usage, options);

	worktrees = get_worktrees(the_repository);
	wt = ڈھونڈو_worktree(worktrees, سابقہ, av[0]);
	اگر (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	اگر (is_main_worktree(wt))
		die(_("The main working tree cannot be locked or unlocked"));

	old_reason = worktree_تالا_reason(wt);
	اگر (old_reason) {
		اگر (*old_reason)
			die(_("'%s' is already locked, reason: %s"),
			    av[0], old_reason);
		die(_("'%s' is already locked"), av[0]);
	}

	راستہ = repo_common_راستہ(the_repository, "worktrees/%s/locked", wt->id);
	write_file(راستہ, "%s", reason);

	free_worktrees(worktrees);
	آزاد(راستہ);
	واپس 0;
}

ساکن صحیح unlock_worktree(صحیح ac, ثابت حرف **av, ثابت حرف *سابقہ,
			   ساخت repository *repo غیر_مستعمل)
{
	ساخت option options[] = {
		OPT_انتہا()
	};
	ساخت worktree **worktrees, *wt;
	حرف *راستہ;
	صحیح ret;

	ac = parse_options(ac, av, سابقہ, options, git_worktree_unlock_usage, 0);
	اگر (ac != 1)
		usage_with_options(git_worktree_unlock_usage, options);

	worktrees = get_worktrees(the_repository);
	wt = ڈھونڈو_worktree(worktrees, سابقہ, av[0]);
	اگر (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	اگر (is_main_worktree(wt))
		die(_("The main working tree cannot be locked or unlocked"));
	اگر (!worktree_تالا_reason(wt))
		die(_("'%s' is not locked"), av[0]);

	راستہ = repo_common_راستہ(the_repository, "worktrees/%s/locked", wt->id);
	ret = unlink_or_warn(راستہ);

	free_worktrees(worktrees);
	آزاد(راستہ);
	واپس ret;
}

ساکن خالی validate_no_submodules(ثابت ساخت worktree *wt)
{
	ساخت اشاریہ_state istate = اشاریہ_STATE_INIT(the_repository);
	ساخت strbuf راستہ = STRBUF_INIT;
	صحیح i, ملا_submodules = 0;
	حرف *wt_gitdir;

	wt_gitdir = get_worktree_git_dir(wt);

	اگر (is_directory(worktree_git_راستہ(wt, "modules"))) {
		/*
		 * There could be false positives, e.g. the "modules"
		 * directory exists but is empty. But it's a rare case and
		 * this simpler check is probably good enough for now.
		 */
		ملا_submodules = 1;
	} ورنہ اگر (read_اشاریہ_from(&istate, worktree_git_راستہ(wt, "index"),
				   wt_gitdir) > 0) {
		برائے (i = 0; i < istate.cache_nr; i++) {
			ساخت cache_اندراج *ce = istate.cache[i];
			صحیح err;

			اگر (!S_ISGITLINK(ce->ce_منوال))
				جاری;

			strbuf_reset(&راستہ);
			strbuf_addf(&راستہ, "%s/%s", wt->راستہ, ce->name);
			اگر (!is_submodule_populated_gently(راستہ.buf, &err))
				جاری;

			ملا_submodules = 1;
			توڑو;
		}
	}
	discard_اشاریہ(&istate);
	strbuf_release(&راستہ);
	آزاد(wt_gitdir);

	اگر (ملا_submodules)
		die(_("working trees containing submodules cannot be moved or removed"));
}

ساکن صحیح move_worktree(صحیح ac, ثابت حرف **av, ثابت حرف *سابقہ,
			 ساخت repository *repo غیر_مستعمل)
{
	صحیح force = 0;
	ساخت option options[] = {
		OPT__FORCE(&force,
			 N_("force move even if worktree is dirty or locked"),
			 PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "relative-paths", &use_relative_paths,
			 N_("use relative paths for worktrees")),
		OPT_انتہا()
	};
	ساخت worktree **worktrees, *wt;
	ساخت strbuf dst = STRBUF_INIT;
	ساخت strbuf errmsg = STRBUF_INIT;
	ثابت حرف *reason = تہی;
	حرف *راستہ;

	ac = parse_options(ac, av, سابقہ, options, git_worktree_move_usage,
			   0);
	اگر (ac != 2)
		usage_with_options(git_worktree_move_usage, options);

	راستہ = سابقہ_filename(سابقہ, av[1]);
	strbuf_addstr(&dst, راستہ);
	آزاد(راستہ);

	worktrees = get_worktrees(the_repository);
	wt = ڈھونڈو_worktree(worktrees, سابقہ, av[0]);
	اگر (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	اگر (is_main_worktree(wt))
		die(_("'%s' is a main working tree"), av[0]);
	اگر (is_directory(dst.buf)) {
		ثابت حرف *sep = ڈھونڈو_آخری_dir_sep(wt->راستہ);

		اگر (!sep)
			die(_("could not figure out destination name from '%s'"),
			    wt->راستہ);
		strbuf_trim_trailing_dir_sep(&dst);
		strbuf_addstr(&dst, sep);
	}
	check_candidate_راستہ(dst.buf, force, worktrees, "move");

	validate_no_submodules(wt);

	اگر (force < 2)
		reason = worktree_تالا_reason(wt);
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

	اگر (rename(wt->راستہ, dst.buf) == -1)
		die_errno(_("failed to move '%s' to '%s'"), wt->راستہ, dst.buf);

	update_worktree_location(wt, dst.buf, use_relative_paths);

	strbuf_release(&dst);
	free_worktrees(worktrees);
	واپس 0;
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
ساکن خالی check_clean_worktree(ساخت worktree *wt,
				 ثابت حرف *original_راستہ)
{
	ساخت بچہ_process cp;
	حرف buf[1];
	صحیح ret;

	/*
	 * Until we sort this out, all submodules are "dirty" and
	 * will abort this function.
	 */
	validate_no_submodules(wt);

	بچہ_process_init(&cp);
	strvec_pushf(&cp.env, "%s=%s/.git",
		     GIT_DIR_ENVIRONMENT, wt->راستہ);
	strvec_pushf(&cp.env, "%s=%s",
		     GIT_WORK_درخت_ENVIRONMENT, wt->راستہ);
	strvec_pushl(&cp.دلائل, "status",
		     "--porcelain", "--ignore-submodules=none",
		     تہی);
	cp.git_cmd = 1;
	cp.dir = wt->راستہ;
	cp.برآمد = -1;
	ret = آغاز_command(&cp);
	اگر (ret)
		die_errno(_("failed to run 'git status' on '%s'"),
			  original_راستہ);
	ret = xread(cp.برآمد, buf, حجم(buf));
	اگر (ret)
		die(_("'%s' contains modified or untracked files, use --force to delete it"),
		    original_راستہ);
	close(cp.برآمد);
	ret = اختتام_command(&cp);
	اگر (ret)
		die_errno(_("failed to run 'git status' on '%s', code %d"),
			  original_راستہ, ret);
}

ساکن صحیح delete_git_work_درخت(ساخت worktree *wt)
{
	ساخت strbuf sb = STRBUF_INIT;
	صحیح ret = 0;

	strbuf_addstr(&sb, wt->راستہ);
	اگر (remove_dir_recursively(&sb, 0)) {
		error_errno(_("failed to delete '%s'"), sb.buf);
		ret = -1;
	}
	strbuf_release(&sb);
	واپس ret;
}

ساکن صحیح remove_worktree(صحیح ac, ثابت حرف **av, ثابت حرف *سابقہ,
			   ساخت repository *repo غیر_مستعمل)
{
	صحیح force = 0;
	ساخت option options[] = {
		OPT__FORCE(&force,
			 N_("force removal even if worktree is dirty or locked"),
			 PARSE_OPT_NOCOMPLETE),
		OPT_انتہا()
	};
	ساخت worktree **worktrees, *wt;
	ساخت strbuf errmsg = STRBUF_INIT;
	ثابت حرف *reason = تہی;
	صحیح ret = 0;

	ac = parse_options(ac, av, سابقہ, options, git_worktree_remove_usage, 0);
	اگر (ac != 1)
		usage_with_options(git_worktree_remove_usage, options);

	worktrees = get_worktrees(the_repository);
	wt = ڈھونڈو_worktree(worktrees, سابقہ, av[0]);
	اگر (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	اگر (is_main_worktree(wt))
		die(_("'%s' is a main working tree"), av[0]);
	اگر (force < 2)
		reason = worktree_تالا_reason(wt);
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

	اگر (file_exists(wt->راستہ)) {
		اگر (!force)
			check_clean_worktree(wt, av[0]);

		ret |= delete_git_work_درخت(wt);
	}
	/*
	 * continue on even if ret is non-zero, there's no going back
	 * from here.
	 */
	ret |= delete_git_dir(wt->id);
	delete_worktrees_dir_if_خالی_خانے();

	free_worktrees(worktrees);
	واپس ret;
}

ساکن خالی report_repair(صحیح iserr, ثابت حرف *راستہ, ثابت حرف *پیغام, خالی *cb_ڈیٹا)
{
	اگر (!iserr) {
		fprintf_ln(stderr, _("repair: %s: %s"), پیغام, راستہ);
	} ورنہ {
		صحیح *exit_حالت = (صحیح *)cb_ڈیٹا;
		fprintf_ln(stderr, _("error: %s: %s"), پیغام, راستہ);
		*exit_حالت = 1;
	}
}

ساکن صحیح repair(صحیح ac, ثابت حرف **av, ثابت حرف *سابقہ,
		  ساخت repository *repo غیر_مستعمل)
{
	ثابت حرف **p;
	ثابت حرف *self[] = { ".", تہی };
	ساخت option options[] = {
		OPT_BOOL(0, "relative-paths", &use_relative_paths,
			 N_("use relative paths for worktrees")),
		OPT_انتہا()
	};
	صحیح rc = 0;

	ac = parse_options(ac, av, سابقہ, options, git_worktree_repair_usage, 0);
	p = ac > 0 ? av : self;
	برائے (; *p; p++)
		repair_worktree_at_راستہ(the_repository, *p, report_repair,
					&rc, use_relative_paths);
	repair_worktrees(the_repository, report_repair, &rc, use_relative_paths);
	واپس rc;
}

صحیح cmd_worktree(صحیح ac,
		 ثابت حرف **av,
		 ثابت حرف *سابقہ,
		 ساخت repository *repo)
{
	parse_opt_subcommand_fn *fn = تہی;
	ساخت option options[] = {
		OPT_SUBCOMMAND("add", &fn, جوڑو),
		OPT_SUBCOMMAND("prune", &fn, prune),
		OPT_SUBCOMMAND("list", &fn, list),
		OPT_SUBCOMMAND("lock", &fn, تالا_worktree),
		OPT_SUBCOMMAND("unlock", &fn, unlock_worktree),
		OPT_SUBCOMMAND("move", &fn, move_worktree),
		OPT_SUBCOMMAND("remove", &fn, remove_worktree),
		OPT_SUBCOMMAND("repair", &fn, repair),
		OPT_انتہا()
	};

	repo_config(the_repository, git_worktree_config, تہی);

	اگر (!سابقہ)
		سابقہ = "";

	ac = parse_options(ac, av, سابقہ, options, git_worktree_usage, 0);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_بھرا_اشاریہ = 0;

	واپس fn(ac, av, سابقہ, repo);
}
