#تعریف_کن USE_THE_REPOSITORY_VARIABLE
#تعریف_کن DISABLE_SIGN_COMPARE_WARNINGS

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

#تعریف_کن BUILTIN_WORKTREE_جمع_کن_USAGE \
	N_("git worktree add [-f] [--detach] [--checkout] [--lock [--reason <string>]]\n" \
	   "                 [--orphan] [(-b | -B) <new-branch>] <path> [<commit-ish>]")

#تعریف_کن BUILTIN_WORKTREE_LIST_USAGE \
	N_("git worktree list [-v | --porcelain [-z]]")
#تعریف_کن BUILTIN_WORKTREE_قفل_USAGE \
	N_("git worktree lock [--reason <string>] <worktree>")
#تعریف_کن BUILTIN_WORKTREE_MOVE_USAGE \
	N_("git worktree move <worktree> <new-path>")
#تعریف_کن BUILTIN_WORKTREE_PRUNE_USAGE \
	N_("git worktree prune [-n] [-v] [--expire <expire>]")
#تعریف_کن BUILTIN_WORKTREE_REMOVE_USAGE \
	N_("git worktree remove [-f] <worktree>")
#تعریف_کن BUILTIN_WORKTREE_REPAIR_USAGE \
	N_("git worktree repair [<path>...]")
#تعریف_کن BUILTIN_WORKTREE_UNLOCK_USAGE \
	N_("git worktree unlock <worktree>")

#تعریف_کن WORKTREE_جمع_کن_DWIM_ORPHAN_INFER_متن \
	_("No possible source branch, inferring '--orphan'")

#تعریف_کن WORKTREE_جمع_کن_ORPHAN_WITH_DASH_B_HINT_متن \
	_("If you meant to create a worktree containing a new unborn branch\n" \
	"(branch with no commits) for this repository, you can do so\n" \
	"using the --orphan flag:\n" \
	"\n" \
	"    git worktree add --orphan -b %s %s\n")

#تعریف_کن WORKTREE_جمع_کن_ORPHAN_NO_DASH_B_HINT_متن \
	_("If you meant to create a worktree containing a new unborn branch\n" \
	"(branch with no commits) for this repository, you can do so\n" \
	"using the --orphan flag:\n" \
	"\n" \
	"    git worktree add --orphan %s\n")

ساكن ثابت حرف * ثابت git_worktree_usage[] = {
	BUILTIN_WORKTREE_جمع_کن_USAGE,
	BUILTIN_WORKTREE_LIST_USAGE,
	BUILTIN_WORKTREE_قفل_USAGE,
	BUILTIN_WORKTREE_MOVE_USAGE,
	BUILTIN_WORKTREE_PRUNE_USAGE,
	BUILTIN_WORKTREE_REMOVE_USAGE,
	BUILTIN_WORKTREE_REPAIR_USAGE,
	BUILTIN_WORKTREE_UNLOCK_USAGE,
	عدم
};

ساكن ثابت حرف * ثابت git_worktree_جمع_کن_usage[] = {
	BUILTIN_WORKTREE_جمع_کن_USAGE,
	عدم,
};

ساكن ثابت حرف * ثابت git_worktree_list_usage[] = {
	BUILTIN_WORKTREE_LIST_USAGE,
	عدم
};

ساكن ثابت حرف * ثابت git_worktree_قفل_usage[] = {
	BUILTIN_WORKTREE_قفل_USAGE,
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

بنية جمع_کن_opts {
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

ساكن صحيح git_worktree_config(ثابت حرف *var, ثابت حرف *مقدار,
			       ثابت بنية config_context *ctx, فراغ *cb)
{
	إذا (!strcmp(var, "worktree.guessremote")) {
		guess_remote = git_config_bool(var, مقدار);
		أرجع 0;
	} وإلا إذا (!strcmp(var, "worktree.userelativepaths")) {
		use_relative_paths = git_config_bool(var, مقدار);
		أرجع 0;
	}

	أرجع git_default_config(var, مقدار, ctx, cb);
}

ساكن صحيح delete_git_dir(ثابت حرف *id)
{
	بنية strbuf sb = STRBUF_INIT;
	صحيح ret;

	repo_common_مسیر_append(the_repository, &sb, "worktrees/%s", id);
	ret = remove_dir_recursively(&sb, 0);
	إذا (ret < 0 && errno == ENOTDIR)
		ret = unlink(sb.buf);
	إذا (ret)
		error_errno(_("failed to delete '%s'"), sb.buf);
	strbuf_release(&sb);
	أرجع ret;
}

ساكن فراغ delete_worktrees_dir_if_خالی(فراغ)
{
	حرف *مسیر = repo_git_مسیر(the_repository, "worktrees");
	rmdir(مسیر); /* ignore failed removal */
	حرر(مسیر);
}

ساكن فراغ prune_worktree(ثابت حرف *id, ثابت حرف *reason)
{
	إذا (show_only || verbose)
		fprintf_ln(stderr, _("Removing %s/%s: %s"), "worktrees", id, reason);
	إذا (!show_only)
		delete_git_dir(id);
}

ساكن صحيح prune_cmp(ثابت فراغ *a, ثابت فراغ *b)
{
	ثابت بنية string_list_قلم *x = a;
	ثابت بنية string_list_قلم *y = b;
	صحيح c;

	إذا ((c = fspathcmp(x->string, y->string)))
	    أرجع c;
	/*
	 * paths same; prune_dupes() removes all but the first worktree entry
	 * having the same path, so sort main worktree ('util' is NULL) above
	 * linked worktrees ('util' not NULL) since main worktree can't be
	 * removed
	 */
	إذا (!x->util)
		أرجع -1;
	إذا (!y->util)
		أرجع 1;
	/* paths same; sort by .git/worktrees/<id> */
	أرجع strcmp(x->util, y->util);
}

ساكن فراغ prune_dups(بنية string_list *l)
{
	صحيح i;

	QSORT(l->اقلام, l->nr, prune_cmp);
	لكل (i = 1; i < l->nr; i++) {
		إذا (!fspathcmp(l->اقلام[i].string, l->اقلام[i - 1].string))
			prune_worktree(l->اقلام[i].util, "duplicate entry");
	}
}

ساكن فراغ prune_worktrees(فراغ)
{
	بنية strbuf reason = STRBUF_INIT;
	بنية strbuf main_مسیر = STRBUF_INIT;
	بنية string_list kept = STRING_LIST_INIT_DUP;
	حرف *مسیر;
	DIR *dir;
	بنية dirent *d;

	مسیر = repo_git_مسیر(the_repository, "worktrees");
	dir = opendir(مسیر);
	حرر(مسیر);
	إذا (!dir)
		أرجع;
	طالما ((d = readdir_skip_dot_and_dotdot(dir)) != عدم) {
		حرف *مسیر;
		strbuf_reset(&reason);
		إذا (should_prune_worktree(the_repository, d->d_name, &reason, &مسیر, expire))
			prune_worktree(d->d_name, reason.buf);
		وإلا إذا (مسیر)
			string_list_append_nodup(&kept, مسیر)->util = xstrdup(d->d_name);
	}
	closedir(dir);

	strbuf_جمع_کن_absolute_مسیر(&main_مسیر, repo_get_common_dir(the_repository));
	/* massage main worktree absolute path to match 'gitdir' content */
	strbuf_strip_suffix(&main_مسیر, "/.");
	string_list_append_nodup(&kept, strbuf_detach(&main_مسیر, عدم));
	prune_dups(&kept);
	string_list_clear(&kept, 1);

	إذا (!show_only)
		delete_worktrees_dir_if_خالی();
	strbuf_release(&reason);
}

ساكن صحيح prune(صحيح ac, ثابت حرف **av, ثابت حرف *پیشوند,
		 بنية repository *repo استفادهنشده)
{
	بنية option options[] = {
		OPT_DRY_پیاپی(&show_only, N_("do not remove, show only")),
		OPT__VERBOSE(&verbose, N_("report pruned working trees")),
		OPT_EXPIRY_DATE(0, "expire", &expire,
				N_("prune missing working trees older than <time>")),
		OPT_انتها()
	};

	expire = زمان_MAX;
	ac = parse_options(ac, av, پیشوند, options, git_worktree_prune_usage,
			   0);
	إذا (ac)
		usage_with_options(git_worktree_prune_usage, options);
	prune_worktrees();
	أرجع 0;
}

ساكن حرف *junk_work_درخت;
ساكن حرف *junk_git_dir;
ساكن صحيح is_junk;
ساكن pid_t junk_pid;

ساكن فراغ remove_junk(فراغ)
{
	بنية strbuf sb = STRBUF_INIT;
	إذا (!is_junk || getpid() != junk_pid)
		أرجع;
	إذا (junk_git_dir) {
		strbuf_addstr(&sb, junk_git_dir);
		remove_dir_recursively(&sb, 0);
		strbuf_reset(&sb);
	}
	إذا (junk_work_درخت) {
		strbuf_addstr(&sb, junk_work_درخت);
		remove_dir_recursively(&sb, 0);
	}
	strbuf_release(&sb);
}

ساكن فراغ remove_junk_on_signal(صحيح signo)
{
	remove_junk();
	sigchain_برداشت(signo);
	ارفع(signo);
}

ساكن حرف *worktree_basename_dup(ثابت حرف *مسیر)
{
	ثابت حرف *name;
	صحيح len;

	len = طول_رشته(مسیر);
	طالما (len && is_dir_sep(مسیر[len - 1]))
		len--;

	name = مسیر + len;
	طالما (name > مسیر && !is_dir_sep(name[-1]))
		name--;

	أرجع xmemdupz(name, مسیر + len - name);
}

/* check that path is viable location for worktree */
ساكن فراغ check_candidate_مسیر(ثابت حرف *مسیر,
				 صحيح force,
				 بنية worktree **worktrees,
				 ثابت حرف *cmd)
{
	بنية worktree *wt;
	صحيح locked;

	إذا (file_exists(مسیر) && !is_خالی_dir(مسیر))
		die(_("'%s' already exists"), مسیر);

	wt = پیدا_کن_worktree_by_مسیر(worktrees, مسیر);
	إذا (!wt)
		أرجع;

	locked = !!worktree_قفل_reason(wt);
	إذا ((!locked && force) || (locked && force > 1)) {
		إذا (delete_git_dir(wt->id))
		    die(_("unusable worktree destination '%s'"), مسیر);
		أرجع;
	}

	إذا (locked)
		die(_("'%s' is a missing but locked worktree;\nuse '%s -f -f' to override, or 'unlock' and 'prune' or 'remove' to clear"), مسیر, cmd);
	وإلا
		die(_("'%s' is a missing but already registered worktree;\nuse '%s -f' to override, or 'prune' or 'remove' to clear"), مسیر, cmd);
}

ساكن فراغ copy_sparse_checkout(ثابت حرف *worktree_git_dir)
{
	حرف *from_file = repo_git_مسیر(the_repository, "info/sparse-checkout");
	حرف *مقصد_file = xstrfmt("%s/info/sparse-checkout", worktree_git_dir);

	إذا (file_exists(from_file)) {
		إذا (امن_create_leading_directories(the_repository, مقصد_file) ||
			copy_file(the_repository, مقصد_file, from_file, 0666))
			error(_("failed to copy '%s' to '%s'; sparse-checkout may not work correctly"),
				from_file, مقصد_file);
	}

	حرر(from_file);
	حرر(مقصد_file);
}

ساكن فراغ copy_filtered_worktree_config(ثابت حرف *worktree_git_dir)
{
	حرف *from_file = repo_git_مسیر(the_repository, "config.worktree");
	حرف *مقصد_file = xstrfmt("%s/config.worktree", worktree_git_dir);

	إذا (file_exists(from_file)) {
		بنية config_مجموعه cs = { { 0 } };
		صحيح bare;

		إذا (امن_create_leading_directories(the_repository, مقصد_file) ||
			copy_file(the_repository, مقصد_file, from_file, 0666)) {
			error(_("failed to copy worktree config from '%s' to '%s'"),
				from_file, مقصد_file);
			اذهب worktree_copy_cleanup;
		}

		git_configset_init(&cs);
		git_configset_جمع_کن_file(&cs, from_file);

		إذا (!git_configset_get_bool(&cs, "core.bare", &bare) &&
			bare &&
			repo_config_مجموعه_multivar_درون_file_gently(the_repository,
				مقصد_file, "core.bare", عدم, "true", عدم, 0))
			error(_("failed to unset '%s' in '%s'"),
				"core.bare", مقصد_file);
		إذا (!git_configset_get(&cs, "core.worktree") &&
			repo_config_مجموعه_درون_file_gently(the_repository, مقصد_file,
						       "core.worktree", عدم, عدم))
			error(_("failed to unset '%s' in '%s'"),
				"core.worktree", مقصد_file);

		git_configset_clear(&cs);
	}

worktree_copy_cleanup:
	حرر(from_file);
	حرر(مقصد_file);
}

ساكن صحيح checkout_worktree(ثابت بنية جمع_کن_opts *opts,
			     بنية strvec *فرزند_env)
{
	بنية فرزند_process cp = فرزند_PROCESS_INIT;
	cp.git_cmd = 1;
	strvec_pushl(&cp.آرگومانها, "reset", "--hard", "--no-recurse-submodules", عدم);
	إذا (opts->quiet)
		strvec_افزودن(&cp.آرگومانها, "--quiet");
	strvec_pushv(&cp.env, فرزند_env->v);
	أرجع پیاپی_command(&cp);
}

ساكن صحيح make_worktree_orphan(ثابت حرف * ref, ثابت بنية جمع_کن_opts *opts,
				بنية strvec *فرزند_env)
{
	بنية strbuf symref = STRBUF_INIT;
	بنية فرزند_process cp = فرزند_PROCESS_INIT;

	validate_new_branchname(ref, &symref, 0);
	strvec_pushl(&cp.آرگومانها, "symbolic-ref", "HEAD", symref.buf, عدم);
	إذا (opts->quiet)
		strvec_افزودن(&cp.آرگومانها, "--quiet");
	strvec_pushv(&cp.env, فرزند_env->v);
	strbuf_release(&symref);
	cp.git_cmd = 1;
	أرجع پیاپی_command(&cp);
}

/*
 * References for worktrees are generally stored in '$GIT_DIR/worktrees/<wt_id>'.
 * But when using alternate reference directories, we want to store the worktree
 * references in '$ALTERNATE_REFERENCE_DIR/worktrees/<wt_id>'.
 *
 * Create the necessary folder structure to facilitate the same. But to ensure
 * that the former path is still considered a Git directory, add stubs.
 */
ساكن فراغ راهاندازی_alternate_ref_dir(بنية worktree *wt, ثابت حرف *wt_git_مسیر)
{
	بنية strbuf sb = STRBUF_INIT;
	حرف *مسیر;

	مسیر = wt->repo->ref_storage_payload;
	إذا (!مسیر)
		أرجع;

	إذا (!is_absolute_مسیر(مسیر))
		strbuf_addf(&sb, "%s/", wt->repo->commondir);

	strbuf_addf(&sb, "%s/worktrees", مسیر);
	امن_create_dir(wt->repo, sb.buf, 1);
	strbuf_addf(&sb, "/%s", wt->id);
	امن_create_dir(wt->repo, sb.buf, 1);
	strbuf_reset(&sb);

	strbuf_addf(&sb, "this worktree stores references in %s/worktrees/%s",
		    مسیر, wt->id);
	refs_create_refdir_stubs(wt->repo, wt_git_مسیر, sb.buf);

	strbuf_release(&sb);
}

ساكن صحيح جمع_کن_worktree(ثابت حرف *مسیر, ثابت حرف *refname,
			ثابت بنية جمع_کن_opts *opts)
{
	بنية strbuf sb_git = STRBUF_INIT, sb_repo = STRBUF_INIT;
	بنية strbuf sb = STRBUF_INIT;
	ثابت حرف *name;
	حرف *name_مقصد_free = عدم;
	بنية strvec فرزند_env = STRVEC_INIT;
	بیعلامت صحيح counter = 0;
	صحيح len, ret;
	بنية strbuf symref = STRBUF_INIT;
	بنية commit *commit = عدم;
	صحيح is_branch = 0;
	بنية strbuf sb_name = STRBUF_INIT;
	بنية worktree **worktrees, *wt = عدم;
	بنية ref_store *wt_refs;
	بنية repo_config_مقادیر *cfg = repo_config_مقادیر(the_repository);

	worktrees = get_worktrees(the_repository);
	check_candidate_مسیر(مسیر, opts->force, worktrees, "add");
	free_worktrees(worktrees);
	worktrees = عدم;

	/* is 'refname' a branch or commit? */
	إذا (!opts->detach && !check_branch_ref(the_repository, &symref, refname) &&
	    refs_ref_exists(get_main_ref_store(the_repository), symref.buf)) {
		is_branch = 1;
		إذا (!opts->force)
			die_if_checked_بیرون(symref.buf, 0);
	}
	commit = lookup_commit_reference_by_name(refname);
	إذا (!commit && !opts->orphan)
		die(_("invalid reference: %s"), refname);

	name = name_مقصد_free = worktree_basename_dup(مسیر);
	إذا (!*name)
		die(_("invalid path '%s'"), مسیر);
	sanitize_refname_component(name, &sb_name);
	إذا (!sb_name.len)
		BUG("How come '%s' becomes empty after sanitization?", name);
	name = sb_name.buf;
	repo_git_مسیر_replace(the_repository, &sb_repo, "worktrees/%s", name);
	len = sb_repo.len;
	إذا (امن_create_leading_directories_const(the_repository, sb_repo.buf))
		die_errno(_("could not create leading directories of '%s'"),
			  sb_repo.buf);

	طالما (mkdir(sb_repo.buf, 0777)) {
		counter++;
		إذا ((errno != EEXIST) || !counter /*سرریز*/)
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
	إذا (opts->keep_locked)
		write_file(sb.buf, "%s", opts->keep_locked);
	وإلا
		write_file(sb.buf, _("initializing"));

	strbuf_addf(&sb_git, "%s/.git", مسیر);
	إذا (امن_create_leading_directories_const(the_repository, sb_git.buf))
		die_errno(_("could not create leading directories of '%s'"),
			  sb_git.buf);
	junk_work_درخت = xstrdup(مسیر);

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
	إذا (!wt) {
		ret = error(_("could not find created worktree '%s'"), name);
		اذهب انجامشده;
	}
	راهاندازی_alternate_ref_dir(wt, sb_repo.buf);
	wt_refs = get_worktree_ref_store(wt);

	ret = ref_store_create_on_disk(wt_refs, REF_STORE_CREATE_ON_DISK_IS_WORKTREE, &sb);
	إذا (ret)
		اذهب انجامشده;

	إذا (!is_branch && commit)
		ret = refs_update_ref(wt_refs, عدم, "HEAD", &commit->object.oid,
				      عدم, 0, UPDATE_REFS_پیام_ON_ERR);
	وإلا
		ret = refs_update_symref(wt_refs, "HEAD", symref.buf, عدم);
	إذا (ret)
		اذهب انجامشده;

	/*
	 * If the current worktree has sparse-checkout enabled, then copy
	 * the sparse-checkout patterns from the current worktree.
	 */
	إذا (cfg->apply_sparse_checkout)
		copy_sparse_checkout(sb_repo.buf);

	/*
	 * If we are using worktree config, then copy all current config
	 * values from the current worktree into the new one, that way the
	 * new worktree behaves the same as this one.
	 */
	إذا (the_repository->repository_format_worktree_config)
		copy_filtered_worktree_config(sb_repo.buf);

	strvec_pushf(&فرزند_env, "%s=%s", GIT_DIR_ENVIRONMENT, sb_git.buf);
	strvec_pushf(&فرزند_env, "%s=%s", GIT_WORK_درخت_ENVIRONMENT, مسیر);

	إذا (opts->orphan &&
	    (ret = make_worktree_orphan(refname, opts, &فرزند_env)))
		اذهب انجامشده;

	إذا (opts->checkout &&
	    (ret = checkout_worktree(opts, &فرزند_env)))
		اذهب انجامشده;

	is_junk = 0;
	FREE_AND_NULL(junk_work_درخت);
	FREE_AND_NULL(junk_git_dir);

انجامشده:
	إذا (ret || !opts->keep_locked) {
		strbuf_reset(&sb);
		strbuf_addf(&sb, "%s/locked", sb_repo.buf);
		unlink_or_warn(sb.buf);
	}

	/*
	 * Hook failure does not warrant worktree deletion, so run hook after
	 * is_junk is cleared, but do return appropriate code when hook fails.
	 */
	إذا (!ret && opts->checkout && !opts->orphan) {
		بنية پیاپی_hooks_opt opt = پیاپی_HOOKS_OPT_INIT_FORCE_SERIAL;

		strvec_pushl(&opt.env, "GIT_DIR", "GIT_WORK_TREE", عدم);
		strvec_pushl(&opt.آرگومانها,
			     oid_مقصد_hex(null_oid(the_hash_algo)),
			     oid_مقصد_hex(&commit->object.oid),
			     "1",
			     عدم);
		opt.dir = مسیر;

		ret = پیاپی_hooks_opt(the_repository, "post-checkout", &opt);
	}

	strvec_clear(&فرزند_env);
	strbuf_release(&sb);
	strbuf_release(&symref);
	strbuf_release(&sb_repo);
	strbuf_release(&sb_git);
	strbuf_release(&sb_name);
	free_worktree(wt);
	حرر(name_مقصد_free);
	أرجع ret;
}

ساكن فراغ print_preparing_worktree_سطر(صحيح detach,
					  ثابت حرف *branch,
					  ثابت حرف *new_branch,
					  صحيح force_new_branch)
{
	إذا (force_new_branch) {
		بنية commit *commit = lookup_commit_reference_by_name(new_branch);
		إذا (!commit)
			fprintf_ln(stderr, _("Preparing worktree (new branch '%s')"), new_branch);
		وإلا
			fprintf_ln(stderr, _("Preparing worktree (resetting branch '%s'; was at %s)"),
				  new_branch,
				  repo_پیدا_کن_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV));
	} وإلا إذا (new_branch) {
		fprintf_ln(stderr, _("Preparing worktree (new branch '%s')"), new_branch);
	} وإلا {
		بنية strbuf s = STRBUF_INIT;
		إذا (!detach && !check_branch_ref(the_repository, &s, branch) &&
		    refs_ref_exists(get_main_ref_store(the_repository), s.buf))
			fprintf_ln(stderr, _("Preparing worktree (checking out '%s')"),
				  branch);
		وإلا {
			بنية commit *commit = lookup_commit_reference_by_name(branch);
			إذا (!commit)
				BUG("unreachable: invalid reference: %s", branch);
			fprintf_ln(stderr, _("Preparing worktree (detached HEAD %s)"),
				  repo_پیدا_کن_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV));
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
ساكن صحيح اولی_valid_ref(ثابت بنية reference *ref استفادهنشده, فراغ *cb_داده استفادهنشده)
{
	أرجع 1;
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
ساكن صحيح can_use_local_refs(ثابت بنية جمع_کن_opts *opts)
{
	إذا (refs_سر_ref(get_main_ref_store(the_repository), اولی_valid_ref, عدم)) {
		أرجع 1;
	} وإلا إذا (refs_for_each_branch_ref(get_main_ref_store(the_repository), اولی_valid_ref, عدم)) {
		إذا (!opts->quiet)
			warning(_("HEAD points to an invalid (or orphaned) reference.\n"));
		أرجع 1;
	}
	أرجع 0;
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
ساكن صحيح can_use_remote_refs(ثابت بنية جمع_کن_opts *opts)
{
	إذا (!guess_remote) {
		أرجع 0;
	} وإلا إذا (refs_for_each_remote_ref(get_main_ref_store(the_repository), اولی_valid_ref, عدم)) {
		أرجع 1;
	} وإلا إذا (!opts->force && remote_get(عدم)) {
		die(_("No local or remote refs exist despite at least one remote\n"
		      "present, stopping; use 'add -f' to override or fetch a remote first"));
	}
	أرجع 0;
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
ساكن صحيح dwim_orphan(ثابت بنية جمع_کن_opts *opts, صحيح opt_track, صحيح remote)
{
	إذا (can_use_local_refs(opts)) {
		أرجع 0;
	} وإلا إذا (remote && can_use_remote_refs(opts)) {
		أرجع 0;
	} وإلا إذا (!opts->quiet) {
		fprintf_ln(stderr, WORKTREE_جمع_کن_DWIM_ORPHAN_INFER_متن);
	}

	إذا (opt_track) {
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--track");
	} وإلا إذا (!opts->checkout) {
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--no-checkout");
	}
	أرجع 1;
}

ساكن فراغ advise_disambiguating_remotes(ثابت حرف *مسیر, ثابت حرف *branch,
					  ثابت بنية string_list *matched_remote_نامها)
{
	بنية string_list_قلم *قلم;

	advise(_("Branch name '%s' appears in multiple remotes:"), branch);
	for_each_string_list_قلم(قلم, matched_remote_نامها) {
		advise(_("  %s"), قلم->string);
	}
	advise(_("If you meant to create a worktree from a remote tracking branch on\n"
		 "<remote>, you can do so by:\n"
		 "\n"
		 "    git worktree add -b %s %s <remote>/%s\n"
		 "\n"
		 "If you'd like to always prefer some remote, e.g. 'origin',\n"
		 "consider setting checkout.defaultRemote=origin in your config."),
	       branch, مسیر, branch);
}

ساكن حرف *dwim_branch(ثابت بنية جمع_کن_opts *opts, ثابت حرف *مسیر, حرف **new_branch)
{
	صحيح branch_exists;
	حرف *branchname = worktree_basename_dup(مسیر);
	بنية strbuf ref = STRBUF_INIT;

	branch_exists = !check_branch_ref(the_repository, &ref, branchname) &&
			refs_ref_exists(get_main_ref_store(the_repository),
					ref.buf);
	strbuf_release(&ref);
	إذا (branch_exists)
		أرجع branchname;

	*new_branch = branchname;
	إذا (guess_remote) {
		بنية object_id oid;
		حرف *remote;
		صحيح رقم_matches = 0;
		بنية string_list matched_remote_نامها = STRING_LIST_INIT_DUP;

		remote = unique_tracking_name(*new_branch, &oid, &رقم_matches,
					      &matched_remote_نامها);
		إذا (!remote && رقم_matches > 1) {
			إذا (!opts->quiet &&
			    advice_enabled(ADVICE_CHECKOUT_AMBIGUOUS_REMOTE_BRANCH_NAME))
				advise_disambiguating_remotes(مسیر, *new_branch,
							      &matched_remote_نامها);
			die(_("'%s' matched multiple (%d) remote tracking branches"),
			    *new_branch, رقم_matches);
		}
		string_list_clear(&matched_remote_نامها, 0);
		أرجع remote;
	}
	أرجع عدم;
}

ساكن صحيح جمع_کن(صحيح ac, ثابت حرف **av, ثابت حرف *پیشوند,
	       بنية repository *repo استفادهنشده)
{
	بنية جمع_کن_opts opts;
	ثابت حرف *new_branch_force = عدم;
	حرف *مسیر;
	ثابت حرف *branch;
	حرف *branch_مقصد_free = عدم;
	حرف *new_branch_مقصد_free = عدم;
	ثابت حرف *new_branch = عدم;
	حرف *opt_track = عدم;
	ثابت حرف *قفل_reason = عدم;
	صحيح keep_locked = 0;
	صحيح استفاده_شده_new_branch_options;
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
		OPT_STRING(0, "reason", &قفل_reason, N_("string"),
			   N_("reason for locking")),
		OPT__QUIET(&opts.quiet, N_("suppress progress reporting")),
		OPT_PASSTHRU(0, "track", &opt_track, عدم,
			     N_("set up tracking mode (see git-branch(1))"),
			     PARSE_OPT_NOARG | PARSE_OPT_OPTARG),
		OPT_BOOL(0, "guess-remote", &guess_remote,
			 N_("try to match the new branch name with a remote-tracking branch")),
		OPT_BOOL(0, "relative-paths", &opts.relative_paths,
			 N_("use relative paths for worktrees")),
		OPT_انتها()
	};
	صحيح ret;

	memset(&opts, 0, حجم(opts));
	opts.checkout = 1;
	opts.relative_paths = use_relative_paths;
	ac = parse_options(ac, av, پیشوند, options, git_worktree_جمع_کن_usage, 0);
	إذا (!!opts.detach + !!new_branch + !!new_branch_force > 1)
		die(_("options '%s', '%s', and '%s' cannot be used together"), "-b", "-B", "--detach");
	إذا (opts.detach && opts.orphan)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--detach");
	إذا (opts.orphan && opt_track)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--track");
	إذا (opts.orphan && !opts.checkout)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--no-checkout");
	إذا (opts.orphan && ac == 2)
		die(_("option '%s' and commit-ish cannot be used together"),
		    "--orphan");
	إذا (قفل_reason && !keep_locked)
		die(_("the option '%s' requires '%s'"), "--reason", "--lock");
	إذا (قفل_reason)
		opts.keep_locked = قفل_reason;
	وإلا إذا (keep_locked)
		opts.keep_locked = _("added with --lock");

	إذا (ac < 1 || ac > 2)
		usage_with_options(git_worktree_جمع_کن_usage, options);

	مسیر = پیشوند_filename(پیشوند, av[0]);
	branch = ac < 2 ? "HEAD" : av[1];
	استفاده_شده_new_branch_options = new_branch || new_branch_force;

	إذا (!strcmp(branch, "-"))
		branch = "@{-1}";

	إذا (new_branch_force) {
		بنية strbuf symref = STRBUF_INIT;

		new_branch = new_branch_force;

		إذا (!opts.force &&
		    !check_branch_ref(the_repository, &symref, new_branch) &&
		    refs_ref_exists(get_main_ref_store(the_repository), symref.buf))
			die_if_checked_بیرون(symref.buf, 0);
		strbuf_release(&symref);
	}

	إذا (opts.orphan && !new_branch) {
		new_branch = new_branch_مقصد_free = worktree_basename_dup(مسیر);
	} وإلا إذا (opts.orphan) {
		; /* no-op */
	} وإلا إذا (opts.detach) {
		/* Check HEAD */
		إذا (!strcmp(branch, "HEAD"))
			can_use_local_refs(&opts);
	} وإلا إذا (ac < 2 && new_branch) {
		/* DWIM: Infer --orphan when repo has no refs. */
		opts.orphan = dwim_orphan(&opts, !!opt_track, 0);
	} وإلا إذا (ac < 2) {
		/* DWIM: Guess branch name from path. */
		حرف *s = dwim_branch(&opts, مسیر, &new_branch_مقصد_free);
		إذا (s)
			branch = branch_مقصد_free = s;
		new_branch = new_branch_مقصد_free;

		/* DWIM: Infer --orphan when repo has no refs. */
		opts.orphan = (!s) && dwim_orphan(&opts, !!opt_track, 1);
	} وإلا إذا (ac == 2 && new_branch) {
		إذا (!strcmp(branch, "HEAD"))
			can_use_local_refs(&opts);
	} وإلا إذا (ac == 2) {
		بنية commit *commit;

		commit = lookup_commit_reference_by_name(branch);
		إذا (!commit) {
			بنية object_id oid;
			حرف *remote;
			صحيح رقم_matches = 0;
			بنية string_list matched_remote_نامها = STRING_LIST_INIT_DUP;

			remote = unique_tracking_name(branch, &oid, &رقم_matches,
						      &matched_remote_نامها);
			إذا (remote) {
				new_branch = branch;
				branch = new_branch_مقصد_free = remote;
			} وإلا إذا (رقم_matches > 1) {
				إذا (!opts.quiet &&
				    advice_enabled(ADVICE_CHECKOUT_AMBIGUOUS_REMOTE_BRANCH_NAME))
					advise_disambiguating_remotes(مسیر, branch,
								      &matched_remote_نامها);
				die(_("'%s' matched multiple (%d) remote tracking branches"),
				    branch, رقم_matches);
			}
			string_list_clear(&matched_remote_نامها, 0);
		}

		إذا (!strcmp(branch, "HEAD"))
			can_use_local_refs(&opts);

	}

	إذا (!opts.orphan && !lookup_commit_reference_by_name(branch)) {
		صحيح attempt_hint = !opts.quiet && (ac < 2);
		إذا (attempt_hint && استفاده_شده_new_branch_options) {
			advise_if_enabled(ADVICE_WORKTREE_جمع_کن_ORPHAN,
				WORKTREE_جمع_کن_ORPHAN_WITH_DASH_B_HINT_متن,
				new_branch, مسیر);
		} وإلا إذا (attempt_hint) {
			advise_if_enabled(ADVICE_WORKTREE_جمع_کن_ORPHAN,
				WORKTREE_جمع_کن_ORPHAN_NO_DASH_B_HINT_متن, مسیر);
		}
		die(_("invalid reference: %s"), branch);
	}

	إذا (!opts.quiet)
		print_preparing_worktree_سطر(opts.detach, branch, new_branch, !!new_branch_force);

	إذا (opts.orphan) {
		branch = new_branch;
	} وإلا إذا (new_branch) {
		بنية فرزند_process cp = فرزند_PROCESS_INIT;
		cp.git_cmd = 1;
		strvec_افزودن(&cp.آرگومانها, "branch");
		إذا (new_branch_force)
			strvec_افزودن(&cp.آرگومانها, "--force");
		إذا (opts.quiet)
			strvec_افزودن(&cp.آرگومانها, "--quiet");
		strvec_افزودن(&cp.آرگومانها, new_branch);
		strvec_افزودن(&cp.آرگومانها, branch);
		إذا (opt_track)
			strvec_افزودن(&cp.آرگومانها, opt_track);
		إذا (پیاپی_command(&cp)) {
			ret = -1;
			اذهب cleanup;
		}
		branch = new_branch;
	} وإلا إذا (opt_track) {
		die(_("--[no-]track can only be used if a new branch is created"));
	}

	ret = جمع_کن_worktree(مسیر, branch, &opts);
cleanup:
	حرر(مسیر);
	حرر(opt_track);
	حرر(branch_مقصد_free);
	حرر(new_branch_مقصد_free);
	أرجع ret;
}

ساكن فراغ show_worktree_porcelain(بنية worktree *wt, صحيح سطر_terminator)
{
	ثابت حرف *reason;

	چاپ_کن("worktree %s%c", wt->مسیر, سطر_terminator);
	إذا (wt->is_bare)
		چاپ_کن("bare%c", سطر_terminator);
	وإلا {
		چاپ_کن("HEAD %s%c", oid_مقصد_hex(&wt->سر_oid), سطر_terminator);
		إذا (wt->is_detached)
			چاپ_کن("detached%c", سطر_terminator);
		وإلا إذا (wt->سر_ref)
			چاپ_کن("branch %s%c", wt->سر_ref, سطر_terminator);
	}

	reason = worktree_قفل_reason(wt);
	إذا (reason) {
		fputs("locked", stdout);
		إذا (*reason) {
			fputc(' ', stdout);
			write_name_quoted(reason, stdout, سطر_terminator);
		} وإلا {
			fputc(سطر_terminator, stdout);
		}
	}

	reason = worktree_prune_reason(wt, expire);
	إذا (reason)
		چاپ_کن("prunable %s%c", reason, سطر_terminator);

	fputc(سطر_terminator, stdout);
}

بنية worktree_display {
	حرف *مسیر;
	صحيح عرض;
};

ساكن فراغ show_worktree(بنية worktree *wt, بنية worktree_display *display,
			  صحيح مسیر_maxwidth, صحيح abbrev_len)
{
	بنية strbuf sb = STRBUF_INIT;
	ثابت حرف *reason;

	strbuf_addf(&sb, "%s%*s", display->مسیر, 1 + مسیر_maxwidth - display->عرض, "");
	إذا (wt->is_bare)
		strbuf_addstr(&sb, "(bare)");
	وإلا {
		strbuf_addf(&sb, "%-*s ", abbrev_len,
				repo_پیدا_کن_unique_abbrev(the_repository, &wt->سر_oid, DEFAULT_ABBREV));
		إذا (wt->is_detached)
			strbuf_addstr(&sb, "(detached HEAD)");
		وإلا إذا (wt->سر_ref) {
			حرف *ref = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
								 wt->سر_ref,
								 0);
			strbuf_addf(&sb, "[%s]", ref);
			حرر(ref);
		} وإلا
			strbuf_addstr(&sb, "(error)");
	}

	reason = worktree_قفل_reason(wt);
	إذا (verbose && reason && *reason)
		strbuf_addf(&sb, "\n\tlocked: %s", reason);
	وإلا إذا (reason)
		strbuf_addstr(&sb, " locked");

	reason = worktree_prune_reason(wt, expire);
	إذا (verbose && reason)
		strbuf_addf(&sb, "\n\tprunable: %s", reason);
	وإلا إذا (reason)
		strbuf_addstr(&sb, " prunable");

	چاپ_کن("%s\n", sb.buf);
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
		quote_مسیر(wt[i]->مسیر, عدم, &buf, 0);
		display[i].عرض = utf8_strwidth(buf.buf);
		display[i].مسیر = strbuf_detach(&buf, عدم);

		إذا (display[i].عرض > *maxwidth)
			*maxwidth = display[i].عرض;
		sha1_len = طول_رشته(repo_پیدا_کن_unique_abbrev(the_repository, &wt[i]->سر_oid, *abbrev));
		إذا (sha1_len > *abbrev)
			*abbrev = sha1_len;
	}
	*d = display;
}

ساكن صحيح pathcmp(ثابت فراغ *a_, ثابت فراغ *b_)
{
	ثابت بنية worktree *ثابت *a = a_;
	ثابت بنية worktree *ثابت *b = b_;
	أرجع fspathcmp((*a)->مسیر, (*b)->مسیر);
}

ساكن فراغ pathsort(بنية worktree **wt)
{
	صحيح n = 0;
	بنية worktree **p = wt;

	طالما (*p++)
		n++;
	QSORT(wt, n, pathcmp);
}

ساكن صحيح list(صحيح ac, ثابت حرف **av, ثابت حرف *پیشوند,
		بنية repository *repo استفادهنشده)
{
	صحيح porcelain = 0;
	صحيح سطر_terminator = '\n';

	بنية option options[] = {
		OPT_BOOL(0, "porcelain", &porcelain, N_("machine-readable output")),
		OPT__VERBOSE(&verbose, N_("show extended annotations and reasons, if available")),
		OPT_EXPIRY_DATE(0, "expire", &expire,
				N_("add 'prunable' annotation to missing worktrees older than <time>")),
		OPT_مجموعه_INT('z', عدم, &سطر_terminator,
			    N_("terminate records with a NUL character"), '\0'),
		OPT_انتها()
	};

	expire = زمان_MAX;
	ac = parse_options(ac, av, پیشوند, options, git_worktree_list_usage, 0);
	إذا (ac)
		usage_with_options(git_worktree_list_usage, options);
	وإلا إذا (verbose && porcelain)
		die(_("options '%s' and '%s' cannot be used together"), "--verbose", "--porcelain");
	وإلا إذا (!سطر_terminator && !porcelain)
		die(_("the option '%s' requires '%s'"), "-z", "--porcelain");
	وإلا {
		بنية worktree **worktrees = get_worktrees(the_repository);
		صحيح مسیر_maxwidth = 0, abbrev = DEFAULT_ABBREV, i;
		بنية worktree_display *display = عدم;

		/* sort worktrees by path but keep main worktree at top */
		pathsort(worktrees + 1);

		إذا (!porcelain)
			measure_widths(worktrees, &abbrev,
				       &display, &مسیر_maxwidth);

		لكل (i = 0; worktrees[i]; i++) {
			إذا (porcelain)
				show_worktree_porcelain(worktrees[i],
							سطر_terminator);
			وإلا
				show_worktree(worktrees[i],
					      &display[i], مسیر_maxwidth, abbrev);
		}
		لكل (i = 0; display && worktrees[i]; i++)
			حرر(display[i].مسیر);
		حرر(display);
		free_worktrees(worktrees);
	}
	أرجع 0;
}

ساكن صحيح قفل_worktree(صحيح ac, ثابت حرف **av, ثابت حرف *پیشوند,
			 بنية repository *repo استفادهنشده)
{
	ثابت حرف *reason = "", *old_reason;
	بنية option options[] = {
		OPT_STRING(0, "reason", &reason, N_("string"),
			   N_("reason for locking")),
		OPT_انتها()
	};
	بنية worktree **worktrees, *wt;
	حرف *مسیر;

	ac = parse_options(ac, av, پیشوند, options, git_worktree_قفل_usage, 0);
	إذا (ac != 1)
		usage_with_options(git_worktree_قفل_usage, options);

	worktrees = get_worktrees(the_repository);
	wt = پیدا_کن_worktree(worktrees, پیشوند, av[0]);
	إذا (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	إذا (is_main_worktree(wt))
		die(_("The main working tree cannot be locked or unlocked"));

	old_reason = worktree_قفل_reason(wt);
	إذا (old_reason) {
		إذا (*old_reason)
			die(_("'%s' is already locked, reason: %s"),
			    av[0], old_reason);
		die(_("'%s' is already locked"), av[0]);
	}

	مسیر = repo_common_مسیر(the_repository, "worktrees/%s/locked", wt->id);
	write_file(مسیر, "%s", reason);

	free_worktrees(worktrees);
	حرر(مسیر);
	أرجع 0;
}

ساكن صحيح unlock_worktree(صحيح ac, ثابت حرف **av, ثابت حرف *پیشوند,
			   بنية repository *repo استفادهنشده)
{
	بنية option options[] = {
		OPT_انتها()
	};
	بنية worktree **worktrees, *wt;
	حرف *مسیر;
	صحيح ret;

	ac = parse_options(ac, av, پیشوند, options, git_worktree_unlock_usage, 0);
	إذا (ac != 1)
		usage_with_options(git_worktree_unlock_usage, options);

	worktrees = get_worktrees(the_repository);
	wt = پیدا_کن_worktree(worktrees, پیشوند, av[0]);
	إذا (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	إذا (is_main_worktree(wt))
		die(_("The main working tree cannot be locked or unlocked"));
	إذا (!worktree_قفل_reason(wt))
		die(_("'%s' is not locked"), av[0]);

	مسیر = repo_common_مسیر(the_repository, "worktrees/%s/locked", wt->id);
	ret = unlink_or_warn(مسیر);

	free_worktrees(worktrees);
	حرر(مسیر);
	أرجع ret;
}

ساكن فراغ validate_no_submodules(ثابت بنية worktree *wt)
{
	بنية شاخص_state istate = شاخص_STATE_INIT(the_repository);
	بنية strbuf مسیر = STRBUF_INIT;
	صحيح i, یافت_submodules = 0;
	حرف *wt_gitdir;

	wt_gitdir = get_worktree_git_dir(wt);

	إذا (is_directory(worktree_git_مسیر(wt, "modules"))) {
		/*
		 * There could be false positives, e.g. the "modules"
		 * directory exists but is empty. But it's a rare case and
		 * this simpler check is probably good enough for now.
		 */
		یافت_submodules = 1;
	} وإلا إذا (read_شاخص_from(&istate, worktree_git_مسیر(wt, "index"),
				   wt_gitdir) > 0) {
		لكل (i = 0; i < istate.cache_nr; i++) {
			بنية cache_مدخل *ce = istate.cache[i];
			صحيح err;

			إذا (!S_ISGITLINK(ce->ce_نما))
				استمر;

			strbuf_reset(&مسیر);
			strbuf_addf(&مسیر, "%s/%s", wt->مسیر, ce->name);
			إذا (!is_submodule_populated_gently(مسیر.buf, &err))
				استمر;

			یافت_submodules = 1;
			اكسر;
		}
	}
	discard_شاخص(&istate);
	strbuf_release(&مسیر);
	حرر(wt_gitdir);

	إذا (یافت_submodules)
		die(_("working trees containing submodules cannot be moved or removed"));
}

ساكن صحيح move_worktree(صحيح ac, ثابت حرف **av, ثابت حرف *پیشوند,
			 بنية repository *repo استفادهنشده)
{
	صحيح force = 0;
	بنية option options[] = {
		OPT__FORCE(&force,
			 N_("force move even if worktree is dirty or locked"),
			 PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "relative-paths", &use_relative_paths,
			 N_("use relative paths for worktrees")),
		OPT_انتها()
	};
	بنية worktree **worktrees, *wt;
	بنية strbuf dst = STRBUF_INIT;
	بنية strbuf errmsg = STRBUF_INIT;
	ثابت حرف *reason = عدم;
	حرف *مسیر;

	ac = parse_options(ac, av, پیشوند, options, git_worktree_move_usage,
			   0);
	إذا (ac != 2)
		usage_with_options(git_worktree_move_usage, options);

	مسیر = پیشوند_filename(پیشوند, av[1]);
	strbuf_addstr(&dst, مسیر);
	حرر(مسیر);

	worktrees = get_worktrees(the_repository);
	wt = پیدا_کن_worktree(worktrees, پیشوند, av[0]);
	إذا (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	إذا (is_main_worktree(wt))
		die(_("'%s' is a main working tree"), av[0]);
	إذا (is_directory(dst.buf)) {
		ثابت حرف *sep = پیدا_کن_آخرین_dir_sep(wt->مسیر);

		إذا (!sep)
			die(_("could not figure out destination name from '%s'"),
			    wt->مسیر);
		strbuf_trim_trailing_dir_sep(&dst);
		strbuf_addstr(&dst, sep);
	}
	check_candidate_مسیر(dst.buf, force, worktrees, "move");

	validate_no_submodules(wt);

	إذا (force < 2)
		reason = worktree_قفل_reason(wt);
	إذا (reason) {
		إذا (*reason)
			die(_("cannot move a locked working tree, lock reason: %s\nuse 'move -f -f' to override or unlock first"),
			    reason);
		die(_("cannot move a locked working tree;\nuse 'move -f -f' to override or unlock first"));
	}
	إذا (validate_worktree(wt, &errmsg, 0))
		die(_("validation failed, cannot move working tree: %s"),
		    errmsg.buf);
	strbuf_release(&errmsg);

	إذا (rename(wt->مسیر, dst.buf) == -1)
		die_errno(_("failed to move '%s' to '%s'"), wt->مسیر, dst.buf);

	update_worktree_location(wt, dst.buf, use_relative_paths);

	strbuf_release(&dst);
	free_worktrees(worktrees);
	أرجع 0;
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
				 ثابت حرف *original_مسیر)
{
	بنية فرزند_process cp;
	حرف buf[1];
	صحيح ret;

	/*
	 * Until we sort this out, all submodules are "dirty" and
	 * will abort this function.
	 */
	validate_no_submodules(wt);

	فرزند_process_init(&cp);
	strvec_pushf(&cp.env, "%s=%s/.git",
		     GIT_DIR_ENVIRONMENT, wt->مسیر);
	strvec_pushf(&cp.env, "%s=%s",
		     GIT_WORK_درخت_ENVIRONMENT, wt->مسیر);
	strvec_pushl(&cp.آرگومانها, "status",
		     "--porcelain", "--ignore-submodules=none",
		     عدم);
	cp.git_cmd = 1;
	cp.dir = wt->مسیر;
	cp.بیرون = -1;
	ret = شروع_command(&cp);
	إذا (ret)
		die_errno(_("failed to run 'git status' on '%s'"),
			  original_مسیر);
	ret = xread(cp.بیرون, buf, حجم(buf));
	إذا (ret)
		die(_("'%s' contains modified or untracked files, use --force to delete it"),
		    original_مسیر);
	close(cp.بیرون);
	ret = پایان_command(&cp);
	إذا (ret)
		die_errno(_("failed to run 'git status' on '%s', code %d"),
			  original_مسیر, ret);
}

ساكن صحيح delete_git_work_درخت(بنية worktree *wt)
{
	بنية strbuf sb = STRBUF_INIT;
	صحيح ret = 0;

	strbuf_addstr(&sb, wt->مسیر);
	إذا (remove_dir_recursively(&sb, 0)) {
		error_errno(_("failed to delete '%s'"), sb.buf);
		ret = -1;
	}
	strbuf_release(&sb);
	أرجع ret;
}

ساكن صحيح remove_worktree(صحيح ac, ثابت حرف **av, ثابت حرف *پیشوند,
			   بنية repository *repo استفادهنشده)
{
	صحيح force = 0;
	بنية option options[] = {
		OPT__FORCE(&force,
			 N_("force removal even if worktree is dirty or locked"),
			 PARSE_OPT_NOCOMPLETE),
		OPT_انتها()
	};
	بنية worktree **worktrees, *wt;
	بنية strbuf errmsg = STRBUF_INIT;
	ثابت حرف *reason = عدم;
	صحيح ret = 0;

	ac = parse_options(ac, av, پیشوند, options, git_worktree_remove_usage, 0);
	إذا (ac != 1)
		usage_with_options(git_worktree_remove_usage, options);

	worktrees = get_worktrees(the_repository);
	wt = پیدا_کن_worktree(worktrees, پیشوند, av[0]);
	إذا (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	إذا (is_main_worktree(wt))
		die(_("'%s' is a main working tree"), av[0]);
	إذا (force < 2)
		reason = worktree_قفل_reason(wt);
	إذا (reason) {
		إذا (*reason)
			die(_("cannot remove a locked working tree, lock reason: %s\nuse 'remove -f -f' to override or unlock first"),
			    reason);
		die(_("cannot remove a locked working tree;\nuse 'remove -f -f' to override or unlock first"));
	}
	إذا (validate_worktree(wt, &errmsg, WT_VALIDATE_WORKTREE_MISSING_OK))
		die(_("validation failed, cannot remove working tree: %s"),
		    errmsg.buf);
	strbuf_release(&errmsg);

	إذا (file_exists(wt->مسیر)) {
		إذا (!force)
			check_clean_worktree(wt, av[0]);

		ret |= delete_git_work_درخت(wt);
	}
	/*
	 * continue on even if ret is non-zero, there's no going back
	 * from here.
	 */
	ret |= delete_git_dir(wt->id);
	delete_worktrees_dir_if_خالی();

	free_worktrees(worktrees);
	أرجع ret;
}

ساكن فراغ report_repair(صحيح iserr, ثابت حرف *مسیر, ثابت حرف *پیام, فراغ *cb_داده)
{
	إذا (!iserr) {
		fprintf_ln(stderr, _("repair: %s: %s"), پیام, مسیر);
	} وإلا {
		صحيح *exit_وضعیت = (صحيح *)cb_داده;
		fprintf_ln(stderr, _("error: %s: %s"), پیام, مسیر);
		*exit_وضعیت = 1;
	}
}

ساكن صحيح repair(صحيح ac, ثابت حرف **av, ثابت حرف *پیشوند,
		  بنية repository *repo استفادهنشده)
{
	ثابت حرف **p;
	ثابت حرف *self[] = { ".", عدم };
	بنية option options[] = {
		OPT_BOOL(0, "relative-paths", &use_relative_paths,
			 N_("use relative paths for worktrees")),
		OPT_انتها()
	};
	صحيح rc = 0;

	ac = parse_options(ac, av, پیشوند, options, git_worktree_repair_usage, 0);
	p = ac > 0 ? av : self;
	لكل (; *p; p++)
		repair_worktree_at_مسیر(the_repository, *p, report_repair,
					&rc, use_relative_paths);
	repair_worktrees(the_repository, report_repair, &rc, use_relative_paths);
	أرجع rc;
}

صحيح cmd_worktree(صحيح ac,
		 ثابت حرف **av,
		 ثابت حرف *پیشوند,
		 بنية repository *repo)
{
	parse_opt_subcommand_fn *fn = عدم;
	بنية option options[] = {
		OPT_SUBCOMMAND("add", &fn, جمع_کن),
		OPT_SUBCOMMAND("prune", &fn, prune),
		OPT_SUBCOMMAND("list", &fn, list),
		OPT_SUBCOMMAND("lock", &fn, قفل_worktree),
		OPT_SUBCOMMAND("unlock", &fn, unlock_worktree),
		OPT_SUBCOMMAND("move", &fn, move_worktree),
		OPT_SUBCOMMAND("remove", &fn, remove_worktree),
		OPT_SUBCOMMAND("repair", &fn, repair),
		OPT_انتها()
	};

	repo_config(the_repository, git_worktree_config, عدم);

	إذا (!پیشوند)
		پیشوند = "";

	ac = parse_options(ac, av, پیشوند, options, git_worktree_usage, 0);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_پر_شاخص = 0;

	أرجع fn(ac, av, پیشوند, repo);
}
