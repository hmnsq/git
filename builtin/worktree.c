#وصف_ڪريو USE_THE_REPOSITORY_VARIABLE
#وصف_ڪريو DISABLE_SIGN_COMPARE_WARNINGS

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

#وصف_ڪريو BUILTIN_WORKTREE_جوڙيو_USAGE \
	N_("git worktree add [-f] [--detach] [--checkout] [--lock [--reason <string>]]\n" \
	   "                 [--orphan] [(-b | -B) <new-branch>] <path> [<commit-ish>]")

#وصف_ڪريو BUILTIN_WORKTREE_LIST_USAGE \
	N_("git worktree list [-v | --porcelain [-z]]")
#وصف_ڪريو BUILTIN_WORKTREE_تالو_USAGE \
	N_("git worktree lock [--reason <string>] <worktree>")
#وصف_ڪريو BUILTIN_WORKTREE_MOVE_USAGE \
	N_("git worktree move <worktree> <new-path>")
#وصف_ڪريو BUILTIN_WORKTREE_PRUNE_USAGE \
	N_("git worktree prune [-n] [-v] [--expire <expire>]")
#وصف_ڪريو BUILTIN_WORKTREE_REMOVE_USAGE \
	N_("git worktree remove [-f] <worktree>")
#وصف_ڪريو BUILTIN_WORKTREE_REPAIR_USAGE \
	N_("git worktree repair [<path>...]")
#وصف_ڪريو BUILTIN_WORKTREE_UNLOCK_USAGE \
	N_("git worktree unlock <worktree>")

#وصف_ڪريو WORKTREE_جوڙيو_DWIM_ORPHAN_INFER_متن \
	_("No possible source branch, inferring '--orphan'")

#وصف_ڪريو WORKTREE_جوڙيو_ORPHAN_WITH_DASH_B_HINT_متن \
	_("If you meant to create a worktree containing a new unborn branch\n" \
	"(branch with no commits) for this repository, you can do so\n" \
	"using the --orphan flag:\n" \
	"\n" \
	"    git worktree add --orphan -b %s %s\n")

#وصف_ڪريو WORKTREE_جوڙيو_ORPHAN_NO_DASH_B_HINT_متن \
	_("If you meant to create a worktree containing a new unborn branch\n" \
	"(branch with no commits) for this repository, you can do so\n" \
	"using the --orphan flag:\n" \
	"\n" \
	"    git worktree add --orphan %s\n")

ساكن ثابت حرف * ثابت git_worktree_usage[] = {
	BUILTIN_WORKTREE_جوڙيو_USAGE,
	BUILTIN_WORKTREE_LIST_USAGE,
	BUILTIN_WORKTREE_تالو_USAGE,
	BUILTIN_WORKTREE_MOVE_USAGE,
	BUILTIN_WORKTREE_PRUNE_USAGE,
	BUILTIN_WORKTREE_REMOVE_USAGE,
	BUILTIN_WORKTREE_REPAIR_USAGE,
	BUILTIN_WORKTREE_UNLOCK_USAGE,
	عدم
};

ساكن ثابت حرف * ثابت git_worktree_جوڙيو_usage[] = {
	BUILTIN_WORKTREE_جوڙيو_USAGE,
	عدم,
};

ساكن ثابت حرف * ثابت git_worktree_list_usage[] = {
	BUILTIN_WORKTREE_LIST_USAGE,
	عدم
};

ساكن ثابت حرف * ثابت git_worktree_تالو_usage[] = {
	BUILTIN_WORKTREE_تالو_USAGE,
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

بنية جوڙيو_opts {
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

ساكن صحيح git_worktree_config(ثابت حرف *var, ثابت حرف *قدر,
			       ثابت بنية config_context *ctx, فراغ *cb)
{
	إذا (!strcmp(var, "worktree.guessremote")) {
		guess_remote = git_config_bool(var, قدر);
		أرجع 0;
	} وإلا إذا (!strcmp(var, "worktree.userelativepaths")) {
		use_relative_paths = git_config_bool(var, قدر);
		أرجع 0;
	}

	أرجع git_default_config(var, قدر, ctx, cb);
}

ساكن صحيح delete_git_dir(ثابت حرف *id)
{
	بنية strbuf sb = STRBUF_INIT;
	صحيح ret;

	repo_common_رستو_append(the_repository, &sb, "worktrees/%s", id);
	ret = remove_dir_recursively(&sb, 0);
	إذا (ret < 0 && errno == ENOTDIR)
		ret = unlink(sb.buf);
	إذا (ret)
		error_errno(_("failed to delete '%s'"), sb.buf);
	strbuf_release(&sb);
	أرجع ret;
}

ساكن فراغ delete_worktrees_dir_if_خالي_جايون(فراغ)
{
	حرف *رستو = repo_git_رستو(the_repository, "worktrees");
	rmdir(رستو); /* ignore failed removal */
	حرر(رستو);
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
	ثابت بنية string_list_شي *x = a;
	ثابت بنية string_list_شي *y = b;
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

	QSORT(l->شيون, l->nr, prune_cmp);
	لكل (i = 1; i < l->nr; i++) {
		إذا (!fspathcmp(l->شيون[i].string, l->شيون[i - 1].string))
			prune_worktree(l->شيون[i].util, "duplicate entry");
	}
}

ساكن فراغ prune_worktrees(فراغ)
{
	بنية strbuf reason = STRBUF_INIT;
	بنية strbuf main_رستو = STRBUF_INIT;
	بنية string_list kept = STRING_LIST_INIT_DUP;
	حرف *رستو;
	DIR *dir;
	بنية dirent *d;

	رستو = repo_git_رستو(the_repository, "worktrees");
	dir = opendir(رستو);
	حرر(رستو);
	إذا (!dir)
		أرجع;
	طالما ((d = readdir_skip_dot_and_dotdot(dir)) != عدم) {
		حرف *رستو;
		strbuf_reset(&reason);
		إذا (should_prune_worktree(the_repository, d->d_name, &reason, &رستو, expire))
			prune_worktree(d->d_name, reason.buf);
		وإلا إذا (رستو)
			string_list_append_nodup(&kept, رستو)->util = xstrdup(d->d_name);
	}
	closedir(dir);

	strbuf_جوڙيو_absolute_رستو(&main_رستو, repo_get_common_dir(the_repository));
	/* massage main worktree absolute path to match 'gitdir' content */
	strbuf_strip_suffix(&main_رستو, "/.");
	string_list_append_nodup(&kept, strbuf_detach(&main_رستو, عدم));
	prune_dups(&kept);
	string_list_clear(&kept, 1);

	إذا (!show_only)
		delete_worktrees_dir_if_خالي_جايون();
	strbuf_release(&reason);
}

ساكن صحيح prune(صحيح ac, ثابت حرف **av, ثابت حرف *اڳياڙي,
		 بنية repository *repo اڻ_استعمال_ٿيل)
{
	بنية option options[] = {
		OPT_DRY_لاڳيتو(&show_only, N_("do not remove, show only")),
		OPT__VERBOSE(&verbose, N_("report pruned working trees")),
		OPT_EXPIRY_DATE(0, "expire", &expire,
				N_("prune missing working trees older than <time>")),
		OPT_پڇاڙي()
	};

	expire = وقت_MAX;
	ac = parse_options(ac, av, اڳياڙي, options, git_worktree_prune_usage,
			   0);
	إذا (ac)
		usage_with_options(git_worktree_prune_usage, options);
	prune_worktrees();
	أرجع 0;
}

ساكن حرف *junk_work_وڻ;
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
	إذا (junk_work_وڻ) {
		strbuf_addstr(&sb, junk_work_وڻ);
		remove_dir_recursively(&sb, 0);
	}
	strbuf_release(&sb);
}

ساكن فراغ remove_junk_on_signal(صحيح signo)
{
	remove_junk();
	sigchain_ڪڍو(signo);
	ارفع(signo);
}

ساكن حرف *worktree_basename_dup(ثابت حرف *رستو)
{
	ثابت حرف *name;
	صحيح len;

	len = لڙي_ڊيگهه(رستو);
	طالما (len && is_dir_sep(رستو[len - 1]))
		len--;

	name = رستو + len;
	طالما (name > رستو && !is_dir_sep(name[-1]))
		name--;

	أرجع xmemdupz(name, رستو + len - name);
}

/* check that path is viable location for worktree */
ساكن فراغ check_candidate_رستو(ثابت حرف *رستو,
				 صحيح force,
				 بنية worktree **worktrees,
				 ثابت حرف *cmd)
{
	بنية worktree *wt;
	صحيح locked;

	إذا (file_exists(رستو) && !is_خالي_جايون_dir(رستو))
		die(_("'%s' already exists"), رستو);

	wt = ڳوليو_worktree_by_رستو(worktrees, رستو);
	إذا (!wt)
		أرجع;

	locked = !!worktree_تالو_reason(wt);
	إذا ((!locked && force) || (locked && force > 1)) {
		إذا (delete_git_dir(wt->id))
		    die(_("unusable worktree destination '%s'"), رستو);
		أرجع;
	}

	إذا (locked)
		die(_("'%s' is a missing but locked worktree;\nuse '%s -f -f' to override, or 'unlock' and 'prune' or 'remove' to clear"), رستو, cmd);
	وإلا
		die(_("'%s' is a missing but already registered worktree;\nuse '%s -f' to override, or 'prune' or 'remove' to clear"), رستو, cmd);
}

ساكن فراغ copy_sparse_checkout(ثابت حرف *worktree_git_dir)
{
	حرف *from_file = repo_git_رستو(the_repository, "info/sparse-checkout");
	حرف *منزل_file = xstrfmt("%s/info/sparse-checkout", worktree_git_dir);

	إذا (file_exists(from_file)) {
		إذا (محفوظ_create_leading_directories(the_repository, منزل_file) ||
			copy_file(the_repository, منزل_file, from_file, 0666))
			error(_("failed to copy '%s' to '%s'; sparse-checkout may not work correctly"),
				from_file, منزل_file);
	}

	حرر(from_file);
	حرر(منزل_file);
}

ساكن فراغ copy_filtered_worktree_config(ثابت حرف *worktree_git_dir)
{
	حرف *from_file = repo_git_رستو(the_repository, "config.worktree");
	حرف *منزل_file = xstrfmt("%s/config.worktree", worktree_git_dir);

	إذا (file_exists(from_file)) {
		بنية config_سيٽ cs = { { 0 } };
		صحيح bare;

		إذا (محفوظ_create_leading_directories(the_repository, منزل_file) ||
			copy_file(the_repository, منزل_file, from_file, 0666)) {
			error(_("failed to copy worktree config from '%s' to '%s'"),
				from_file, منزل_file);
			اذهب worktree_copy_cleanup;
		}

		git_configset_init(&cs);
		git_configset_جوڙيو_file(&cs, from_file);

		إذا (!git_configset_get_bool(&cs, "core.bare", &bare) &&
			bare &&
			repo_config_سيٽ_multivar_اندريون_file_gently(the_repository,
				منزل_file, "core.bare", عدم, "true", عدم, 0))
			error(_("failed to unset '%s' in '%s'"),
				"core.bare", منزل_file);
		إذا (!git_configset_get(&cs, "core.worktree") &&
			repo_config_سيٽ_اندريون_file_gently(the_repository, منزل_file,
						       "core.worktree", عدم, عدم))
			error(_("failed to unset '%s' in '%s'"),
				"core.worktree", منزل_file);

		git_configset_clear(&cs);
	}

worktree_copy_cleanup:
	حرر(from_file);
	حرر(منزل_file);
}

ساكن صحيح checkout_worktree(ثابت بنية جوڙيو_opts *opts,
			     بنية strvec *ٻار_env)
{
	بنية ٻار_process cp = ٻار_PROCESS_INIT;
	cp.git_cmd = 1;
	strvec_pushl(&cp.دليلون, "reset", "--hard", "--no-recurse-submodules", عدم);
	إذا (opts->quiet)
		strvec_ڌڪو(&cp.دليلون, "--quiet");
	strvec_pushv(&cp.env, ٻار_env->v);
	أرجع لاڳيتو_command(&cp);
}

ساكن صحيح make_worktree_orphan(ثابت حرف * ref, ثابت بنية جوڙيو_opts *opts,
				بنية strvec *ٻار_env)
{
	بنية strbuf symref = STRBUF_INIT;
	بنية ٻار_process cp = ٻار_PROCESS_INIT;

	validate_new_branchname(ref, &symref, 0);
	strvec_pushl(&cp.دليلون, "symbolic-ref", "HEAD", symref.buf, عدم);
	إذا (opts->quiet)
		strvec_ڌڪو(&cp.دليلون, "--quiet");
	strvec_pushv(&cp.env, ٻار_env->v);
	strbuf_release(&symref);
	cp.git_cmd = 1;
	أرجع لاڳيتو_command(&cp);
}

/*
 * References for worktrees are generally stored in '$GIT_DIR/worktrees/<wt_id>'.
 * But when using alternate reference directories, we want to store the worktree
 * references in '$ALTERNATE_REFERENCE_DIR/worktrees/<wt_id>'.
 *
 * Create the necessary folder structure to facilitate the same. But to ensure
 * that the former path is still considered a Git directory, add stubs.
 */
ساكن فراغ تياري_alternate_ref_dir(بنية worktree *wt, ثابت حرف *wt_git_رستو)
{
	بنية strbuf sb = STRBUF_INIT;
	حرف *رستو;

	رستو = wt->repo->ref_storage_payload;
	إذا (!رستو)
		أرجع;

	إذا (!is_absolute_رستو(رستو))
		strbuf_addf(&sb, "%s/", wt->repo->commondir);

	strbuf_addf(&sb, "%s/worktrees", رستو);
	محفوظ_create_dir(wt->repo, sb.buf, 1);
	strbuf_addf(&sb, "/%s", wt->id);
	محفوظ_create_dir(wt->repo, sb.buf, 1);
	strbuf_reset(&sb);

	strbuf_addf(&sb, "this worktree stores references in %s/worktrees/%s",
		    رستو, wt->id);
	refs_create_refdir_stubs(wt->repo, wt_git_رستو, sb.buf);

	strbuf_release(&sb);
}

ساكن صحيح جوڙيو_worktree(ثابت حرف *رستو, ثابت حرف *refname,
			ثابت بنية جوڙيو_opts *opts)
{
	بنية strbuf sb_git = STRBUF_INIT, sb_repo = STRBUF_INIT;
	بنية strbuf sb = STRBUF_INIT;
	ثابت حرف *name;
	حرف *name_منزل_free = عدم;
	بنية strvec ٻار_env = STRVEC_INIT;
	بنانشان صحيح counter = 0;
	صحيح len, ret;
	بنية strbuf symref = STRBUF_INIT;
	بنية commit *commit = عدم;
	صحيح is_branch = 0;
	بنية strbuf sb_name = STRBUF_INIT;
	بنية worktree **worktrees, *wt = عدم;
	بنية ref_store *wt_refs;
	بنية repo_config_قدرون *cfg = repo_config_قدرون(the_repository);

	worktrees = get_worktrees(the_repository);
	check_candidate_رستو(رستو, opts->force, worktrees, "add");
	free_worktrees(worktrees);
	worktrees = عدم;

	/* is 'refname' a branch or commit? */
	إذا (!opts->detach && !check_branch_ref(the_repository, &symref, refname) &&
	    refs_ref_exists(get_main_ref_store(the_repository), symref.buf)) {
		is_branch = 1;
		إذا (!opts->force)
			die_if_checked_ٻاهريون(symref.buf, 0);
	}
	commit = lookup_commit_reference_by_name(refname);
	إذا (!commit && !opts->orphan)
		die(_("invalid reference: %s"), refname);

	name = name_منزل_free = worktree_basename_dup(رستو);
	إذا (!*name)
		die(_("invalid path '%s'"), رستو);
	sanitize_refname_component(name, &sb_name);
	إذا (!sb_name.len)
		BUG("How come '%s' becomes empty after sanitization?", name);
	name = sb_name.buf;
	repo_git_رستو_replace(the_repository, &sb_repo, "worktrees/%s", name);
	len = sb_repo.len;
	إذا (محفوظ_create_leading_directories_const(the_repository, sb_repo.buf))
		die_errno(_("could not create leading directories of '%s'"),
			  sb_repo.buf);

	طالما (mkdir(sb_repo.buf, 0777)) {
		counter++;
		إذا ((errno != EEXIST) || !counter /*حد_کان_مٿي*/)
			die_errno(_("could not create directory of '%s'"),
				  sb_repo.buf);
		strbuf_setlen(&sb_repo, len);
		strbuf_addf(&sb_repo, "%d", counter);
	}
	name = strrchr(sb_repo.buf, '/') + 1;

	junk_pid = getpid();
	atexit(remove_junk);
	sigchain_ڌڪو_common(remove_junk_on_signal);

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

	strbuf_addf(&sb_git, "%s/.git", رستو);
	إذا (محفوظ_create_leading_directories_const(the_repository, sb_git.buf))
		die_errno(_("could not create leading directories of '%s'"),
			  sb_git.buf);
	junk_work_وڻ = xstrdup(رستو);

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
		اذهب پورو;
	}
	تياري_alternate_ref_dir(wt, sb_repo.buf);
	wt_refs = get_worktree_ref_store(wt);

	ret = ref_store_create_on_disk(wt_refs, REF_STORE_CREATE_ON_DISK_IS_WORKTREE, &sb);
	إذا (ret)
		اذهب پورو;

	إذا (!is_branch && commit)
		ret = refs_update_ref(wt_refs, عدم, "HEAD", &commit->object.oid,
				      عدم, 0, UPDATE_REFS_نياپو_ON_ERR);
	وإلا
		ret = refs_update_symref(wt_refs, "HEAD", symref.buf, عدم);
	إذا (ret)
		اذهب پورو;

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

	strvec_pushf(&ٻار_env, "%s=%s", GIT_DIR_ENVIRONMENT, sb_git.buf);
	strvec_pushf(&ٻار_env, "%s=%s", GIT_WORK_وڻ_ENVIRONMENT, رستو);

	إذا (opts->orphan &&
	    (ret = make_worktree_orphan(refname, opts, &ٻار_env)))
		اذهب پورو;

	إذا (opts->checkout &&
	    (ret = checkout_worktree(opts, &ٻار_env)))
		اذهب پورو;

	is_junk = 0;
	FREE_AND_NULL(junk_work_وڻ);
	FREE_AND_NULL(junk_git_dir);

پورو:
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
		بنية لاڳيتو_hooks_opt opt = لاڳيتو_HOOKS_OPT_INIT_FORCE_SERIAL;

		strvec_pushl(&opt.env, "GIT_DIR", "GIT_WORK_TREE", عدم);
		strvec_pushl(&opt.دليلون,
			     oid_منزل_hex(null_oid(the_hash_algo)),
			     oid_منزل_hex(&commit->object.oid),
			     "1",
			     عدم);
		opt.dir = رستو;

		ret = لاڳيتو_hooks_opt(the_repository, "post-checkout", &opt);
	}

	strvec_clear(&ٻار_env);
	strbuf_release(&sb);
	strbuf_release(&symref);
	strbuf_release(&sb_repo);
	strbuf_release(&sb_git);
	strbuf_release(&sb_name);
	free_worktree(wt);
	حرر(name_منزل_free);
	أرجع ret;
}

ساكن فراغ print_preparing_worktree_سٽ(صحيح detach,
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
				  repo_ڳوليو_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV));
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
				  repo_ڳوليو_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV));
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
ساكن صحيح پهريون_valid_ref(ثابت بنية reference *ref اڻ_استعمال_ٿيل, فراغ *cb_معلومات اڻ_استعمال_ٿيل)
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
ساكن صحيح can_use_local_refs(ثابت بنية جوڙيو_opts *opts)
{
	إذا (refs_سر_ref(get_main_ref_store(the_repository), پهريون_valid_ref, عدم)) {
		أرجع 1;
	} وإلا إذا (refs_for_each_branch_ref(get_main_ref_store(the_repository), پهريون_valid_ref, عدم)) {
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
ساكن صحيح can_use_remote_refs(ثابت بنية جوڙيو_opts *opts)
{
	إذا (!guess_remote) {
		أرجع 0;
	} وإلا إذا (refs_for_each_remote_ref(get_main_ref_store(the_repository), پهريون_valid_ref, عدم)) {
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
ساكن صحيح dwim_orphan(ثابت بنية جوڙيو_opts *opts, صحيح opt_track, صحيح remote)
{
	إذا (can_use_local_refs(opts)) {
		أرجع 0;
	} وإلا إذا (remote && can_use_remote_refs(opts)) {
		أرجع 0;
	} وإلا إذا (!opts->quiet) {
		fprintf_ln(stderr, WORKTREE_جوڙيو_DWIM_ORPHAN_INFER_متن);
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

ساكن فراغ advise_disambiguating_remotes(ثابت حرف *رستو, ثابت حرف *branch,
					  ثابت بنية string_list *matched_remote_نالا)
{
	بنية string_list_شي *شي;

	advise(_("Branch name '%s' appears in multiple remotes:"), branch);
	for_each_string_list_شي(شي, matched_remote_نالا) {
		advise(_("  %s"), شي->string);
	}
	advise(_("If you meant to create a worktree from a remote tracking branch on\n"
		 "<remote>, you can do so by:\n"
		 "\n"
		 "    git worktree add -b %s %s <remote>/%s\n"
		 "\n"
		 "If you'd like to always prefer some remote, e.g. 'origin',\n"
		 "consider setting checkout.defaultRemote=origin in your config."),
	       branch, رستو, branch);
}

ساكن حرف *dwim_branch(ثابت بنية جوڙيو_opts *opts, ثابت حرف *رستو, حرف **new_branch)
{
	صحيح branch_exists;
	حرف *branchname = worktree_basename_dup(رستو);
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
		صحيح انگ_matches = 0;
		بنية string_list matched_remote_نالا = STRING_LIST_INIT_DUP;

		remote = unique_tracking_name(*new_branch, &oid, &انگ_matches,
					      &matched_remote_نالا);
		إذا (!remote && انگ_matches > 1) {
			إذا (!opts->quiet &&
			    advice_enabled(ADVICE_CHECKOUT_AMBIGUOUS_REMOTE_BRANCH_NAME))
				advise_disambiguating_remotes(رستو, *new_branch,
							      &matched_remote_نالا);
			die(_("'%s' matched multiple (%d) remote tracking branches"),
			    *new_branch, انگ_matches);
		}
		string_list_clear(&matched_remote_نالا, 0);
		أرجع remote;
	}
	أرجع عدم;
}

ساكن صحيح جوڙيو(صحيح ac, ثابت حرف **av, ثابت حرف *اڳياڙي,
	       بنية repository *repo اڻ_استعمال_ٿيل)
{
	بنية جوڙيو_opts opts;
	ثابت حرف *new_branch_force = عدم;
	حرف *رستو;
	ثابت حرف *branch;
	حرف *branch_منزل_free = عدم;
	حرف *new_branch_منزل_free = عدم;
	ثابت حرف *new_branch = عدم;
	حرف *opt_track = عدم;
	ثابت حرف *تالو_reason = عدم;
	صحيح keep_locked = 0;
	صحيح استعمال_ٿيل_new_branch_options;
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
		OPT_STRING(0, "reason", &تالو_reason, N_("string"),
			   N_("reason for locking")),
		OPT__QUIET(&opts.quiet, N_("suppress progress reporting")),
		OPT_PASSTHRU(0, "track", &opt_track, عدم,
			     N_("set up tracking mode (see git-branch(1))"),
			     PARSE_OPT_NOARG | PARSE_OPT_OPTARG),
		OPT_BOOL(0, "guess-remote", &guess_remote,
			 N_("try to match the new branch name with a remote-tracking branch")),
		OPT_BOOL(0, "relative-paths", &opts.relative_paths,
			 N_("use relative paths for worktrees")),
		OPT_پڇاڙي()
	};
	صحيح ret;

	memset(&opts, 0, حجم(opts));
	opts.checkout = 1;
	opts.relative_paths = use_relative_paths;
	ac = parse_options(ac, av, اڳياڙي, options, git_worktree_جوڙيو_usage, 0);
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
	إذا (تالو_reason && !keep_locked)
		die(_("the option '%s' requires '%s'"), "--reason", "--lock");
	إذا (تالو_reason)
		opts.keep_locked = تالو_reason;
	وإلا إذا (keep_locked)
		opts.keep_locked = _("added with --lock");

	إذا (ac < 1 || ac > 2)
		usage_with_options(git_worktree_جوڙيو_usage, options);

	رستو = اڳياڙي_filename(اڳياڙي, av[0]);
	branch = ac < 2 ? "HEAD" : av[1];
	استعمال_ٿيل_new_branch_options = new_branch || new_branch_force;

	إذا (!strcmp(branch, "-"))
		branch = "@{-1}";

	إذا (new_branch_force) {
		بنية strbuf symref = STRBUF_INIT;

		new_branch = new_branch_force;

		إذا (!opts.force &&
		    !check_branch_ref(the_repository, &symref, new_branch) &&
		    refs_ref_exists(get_main_ref_store(the_repository), symref.buf))
			die_if_checked_ٻاهريون(symref.buf, 0);
		strbuf_release(&symref);
	}

	إذا (opts.orphan && !new_branch) {
		new_branch = new_branch_منزل_free = worktree_basename_dup(رستو);
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
		حرف *s = dwim_branch(&opts, رستو, &new_branch_منزل_free);
		إذا (s)
			branch = branch_منزل_free = s;
		new_branch = new_branch_منزل_free;

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
			صحيح انگ_matches = 0;
			بنية string_list matched_remote_نالا = STRING_LIST_INIT_DUP;

			remote = unique_tracking_name(branch, &oid, &انگ_matches,
						      &matched_remote_نالا);
			إذا (remote) {
				new_branch = branch;
				branch = new_branch_منزل_free = remote;
			} وإلا إذا (انگ_matches > 1) {
				إذا (!opts.quiet &&
				    advice_enabled(ADVICE_CHECKOUT_AMBIGUOUS_REMOTE_BRANCH_NAME))
					advise_disambiguating_remotes(رستو, branch,
								      &matched_remote_نالا);
				die(_("'%s' matched multiple (%d) remote tracking branches"),
				    branch, انگ_matches);
			}
			string_list_clear(&matched_remote_نالا, 0);
		}

		إذا (!strcmp(branch, "HEAD"))
			can_use_local_refs(&opts);

	}

	إذا (!opts.orphan && !lookup_commit_reference_by_name(branch)) {
		صحيح attempt_hint = !opts.quiet && (ac < 2);
		إذا (attempt_hint && استعمال_ٿيل_new_branch_options) {
			advise_if_enabled(ADVICE_WORKTREE_جوڙيو_ORPHAN,
				WORKTREE_جوڙيو_ORPHAN_WITH_DASH_B_HINT_متن,
				new_branch, رستو);
		} وإلا إذا (attempt_hint) {
			advise_if_enabled(ADVICE_WORKTREE_جوڙيو_ORPHAN,
				WORKTREE_جوڙيو_ORPHAN_NO_DASH_B_HINT_متن, رستو);
		}
		die(_("invalid reference: %s"), branch);
	}

	إذا (!opts.quiet)
		print_preparing_worktree_سٽ(opts.detach, branch, new_branch, !!new_branch_force);

	إذا (opts.orphan) {
		branch = new_branch;
	} وإلا إذا (new_branch) {
		بنية ٻار_process cp = ٻار_PROCESS_INIT;
		cp.git_cmd = 1;
		strvec_ڌڪو(&cp.دليلون, "branch");
		إذا (new_branch_force)
			strvec_ڌڪو(&cp.دليلون, "--force");
		إذا (opts.quiet)
			strvec_ڌڪو(&cp.دليلون, "--quiet");
		strvec_ڌڪو(&cp.دليلون, new_branch);
		strvec_ڌڪو(&cp.دليلون, branch);
		إذا (opt_track)
			strvec_ڌڪو(&cp.دليلون, opt_track);
		إذا (لاڳيتو_command(&cp)) {
			ret = -1;
			اذهب cleanup;
		}
		branch = new_branch;
	} وإلا إذا (opt_track) {
		die(_("--[no-]track can only be used if a new branch is created"));
	}

	ret = جوڙيو_worktree(رستو, branch, &opts);
cleanup:
	حرر(رستو);
	حرر(opt_track);
	حرر(branch_منزل_free);
	حرر(new_branch_منزل_free);
	أرجع ret;
}

ساكن فراغ show_worktree_porcelain(بنية worktree *wt, صحيح سٽ_terminator)
{
	ثابت حرف *reason;

	لکو("worktree %s%c", wt->رستو, سٽ_terminator);
	إذا (wt->is_bare)
		لکو("bare%c", سٽ_terminator);
	وإلا {
		لکو("HEAD %s%c", oid_منزل_hex(&wt->سر_oid), سٽ_terminator);
		إذا (wt->is_detached)
			لکو("detached%c", سٽ_terminator);
		وإلا إذا (wt->سر_ref)
			لکو("branch %s%c", wt->سر_ref, سٽ_terminator);
	}

	reason = worktree_تالو_reason(wt);
	إذا (reason) {
		fputs("locked", stdout);
		إذا (*reason) {
			fputc(' ', stdout);
			write_name_quoted(reason, stdout, سٽ_terminator);
		} وإلا {
			fputc(سٽ_terminator, stdout);
		}
	}

	reason = worktree_prune_reason(wt, expire);
	إذا (reason)
		لکو("prunable %s%c", reason, سٽ_terminator);

	fputc(سٽ_terminator, stdout);
}

بنية worktree_display {
	حرف *رستو;
	صحيح ويڪر;
};

ساكن فراغ show_worktree(بنية worktree *wt, بنية worktree_display *display,
			  صحيح رستو_maxwidth, صحيح abbrev_len)
{
	بنية strbuf sb = STRBUF_INIT;
	ثابت حرف *reason;

	strbuf_addf(&sb, "%s%*s", display->رستو, 1 + رستو_maxwidth - display->ويڪر, "");
	إذا (wt->is_bare)
		strbuf_addstr(&sb, "(bare)");
	وإلا {
		strbuf_addf(&sb, "%-*s ", abbrev_len,
				repo_ڳوليو_unique_abbrev(the_repository, &wt->سر_oid, DEFAULT_ABBREV));
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

	reason = worktree_تالو_reason(wt);
	إذا (verbose && reason && *reason)
		strbuf_addf(&sb, "\n\tlocked: %s", reason);
	وإلا إذا (reason)
		strbuf_addstr(&sb, " locked");

	reason = worktree_prune_reason(wt, expire);
	إذا (verbose && reason)
		strbuf_addf(&sb, "\n\tprunable: %s", reason);
	وإلا إذا (reason)
		strbuf_addstr(&sb, " prunable");

	لکو("%s\n", sb.buf);
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
		quote_رستو(wt[i]->رستو, عدم, &buf, 0);
		display[i].ويڪر = utf8_strwidth(buf.buf);
		display[i].رستو = strbuf_detach(&buf, عدم);

		إذا (display[i].ويڪر > *maxwidth)
			*maxwidth = display[i].ويڪر;
		sha1_len = لڙي_ڊيگهه(repo_ڳوليو_unique_abbrev(the_repository, &wt[i]->سر_oid, *abbrev));
		إذا (sha1_len > *abbrev)
			*abbrev = sha1_len;
	}
	*d = display;
}

ساكن صحيح pathcmp(ثابت فراغ *a_, ثابت فراغ *b_)
{
	ثابت بنية worktree *ثابت *a = a_;
	ثابت بنية worktree *ثابت *b = b_;
	أرجع fspathcmp((*a)->رستو, (*b)->رستو);
}

ساكن فراغ pathsort(بنية worktree **wt)
{
	صحيح n = 0;
	بنية worktree **p = wt;

	طالما (*p++)
		n++;
	QSORT(wt, n, pathcmp);
}

ساكن صحيح list(صحيح ac, ثابت حرف **av, ثابت حرف *اڳياڙي,
		بنية repository *repo اڻ_استعمال_ٿيل)
{
	صحيح porcelain = 0;
	صحيح سٽ_terminator = '\n';

	بنية option options[] = {
		OPT_BOOL(0, "porcelain", &porcelain, N_("machine-readable output")),
		OPT__VERBOSE(&verbose, N_("show extended annotations and reasons, if available")),
		OPT_EXPIRY_DATE(0, "expire", &expire,
				N_("add 'prunable' annotation to missing worktrees older than <time>")),
		OPT_سيٽ_INT('z', عدم, &سٽ_terminator,
			    N_("terminate records with a NUL character"), '\0'),
		OPT_پڇاڙي()
	};

	expire = وقت_MAX;
	ac = parse_options(ac, av, اڳياڙي, options, git_worktree_list_usage, 0);
	إذا (ac)
		usage_with_options(git_worktree_list_usage, options);
	وإلا إذا (verbose && porcelain)
		die(_("options '%s' and '%s' cannot be used together"), "--verbose", "--porcelain");
	وإلا إذا (!سٽ_terminator && !porcelain)
		die(_("the option '%s' requires '%s'"), "-z", "--porcelain");
	وإلا {
		بنية worktree **worktrees = get_worktrees(the_repository);
		صحيح رستو_maxwidth = 0, abbrev = DEFAULT_ABBREV, i;
		بنية worktree_display *display = عدم;

		/* sort worktrees by path but keep main worktree at top */
		pathsort(worktrees + 1);

		إذا (!porcelain)
			measure_widths(worktrees, &abbrev,
				       &display, &رستو_maxwidth);

		لكل (i = 0; worktrees[i]; i++) {
			إذا (porcelain)
				show_worktree_porcelain(worktrees[i],
							سٽ_terminator);
			وإلا
				show_worktree(worktrees[i],
					      &display[i], رستو_maxwidth, abbrev);
		}
		لكل (i = 0; display && worktrees[i]; i++)
			حرر(display[i].رستو);
		حرر(display);
		free_worktrees(worktrees);
	}
	أرجع 0;
}

ساكن صحيح تالو_worktree(صحيح ac, ثابت حرف **av, ثابت حرف *اڳياڙي,
			 بنية repository *repo اڻ_استعمال_ٿيل)
{
	ثابت حرف *reason = "", *old_reason;
	بنية option options[] = {
		OPT_STRING(0, "reason", &reason, N_("string"),
			   N_("reason for locking")),
		OPT_پڇاڙي()
	};
	بنية worktree **worktrees, *wt;
	حرف *رستو;

	ac = parse_options(ac, av, اڳياڙي, options, git_worktree_تالو_usage, 0);
	إذا (ac != 1)
		usage_with_options(git_worktree_تالو_usage, options);

	worktrees = get_worktrees(the_repository);
	wt = ڳوليو_worktree(worktrees, اڳياڙي, av[0]);
	إذا (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	إذا (is_main_worktree(wt))
		die(_("The main working tree cannot be locked or unlocked"));

	old_reason = worktree_تالو_reason(wt);
	إذا (old_reason) {
		إذا (*old_reason)
			die(_("'%s' is already locked, reason: %s"),
			    av[0], old_reason);
		die(_("'%s' is already locked"), av[0]);
	}

	رستو = repo_common_رستو(the_repository, "worktrees/%s/locked", wt->id);
	write_file(رستو, "%s", reason);

	free_worktrees(worktrees);
	حرر(رستو);
	أرجع 0;
}

ساكن صحيح unlock_worktree(صحيح ac, ثابت حرف **av, ثابت حرف *اڳياڙي,
			   بنية repository *repo اڻ_استعمال_ٿيل)
{
	بنية option options[] = {
		OPT_پڇاڙي()
	};
	بنية worktree **worktrees, *wt;
	حرف *رستو;
	صحيح ret;

	ac = parse_options(ac, av, اڳياڙي, options, git_worktree_unlock_usage, 0);
	إذا (ac != 1)
		usage_with_options(git_worktree_unlock_usage, options);

	worktrees = get_worktrees(the_repository);
	wt = ڳوليو_worktree(worktrees, اڳياڙي, av[0]);
	إذا (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	إذا (is_main_worktree(wt))
		die(_("The main working tree cannot be locked or unlocked"));
	إذا (!worktree_تالو_reason(wt))
		die(_("'%s' is not locked"), av[0]);

	رستو = repo_common_رستو(the_repository, "worktrees/%s/locked", wt->id);
	ret = unlink_or_warn(رستو);

	free_worktrees(worktrees);
	حرر(رستو);
	أرجع ret;
}

ساكن فراغ validate_no_submodules(ثابت بنية worktree *wt)
{
	بنية اشاريو_state istate = اشاريو_STATE_INIT(the_repository);
	بنية strbuf رستو = STRBUF_INIT;
	صحيح i, مليو_submodules = 0;
	حرف *wt_gitdir;

	wt_gitdir = get_worktree_git_dir(wt);

	إذا (is_directory(worktree_git_رستو(wt, "modules"))) {
		/*
		 * There could be false positives, e.g. the "modules"
		 * directory exists but is empty. But it's a rare case and
		 * this simpler check is probably good enough for now.
		 */
		مليو_submodules = 1;
	} وإلا إذا (read_اشاريو_from(&istate, worktree_git_رستو(wt, "index"),
				   wt_gitdir) > 0) {
		لكل (i = 0; i < istate.cache_nr; i++) {
			بنية cache_داخلا *ce = istate.cache[i];
			صحيح err;

			إذا (!S_ISGITLINK(ce->ce_گهڻائي))
				استمر;

			strbuf_reset(&رستو);
			strbuf_addf(&رستو, "%s/%s", wt->رستو, ce->name);
			إذا (!is_submodule_populated_gently(رستو.buf, &err))
				استمر;

			مليو_submodules = 1;
			اكسر;
		}
	}
	discard_اشاريو(&istate);
	strbuf_release(&رستو);
	حرر(wt_gitdir);

	إذا (مليو_submodules)
		die(_("working trees containing submodules cannot be moved or removed"));
}

ساكن صحيح move_worktree(صحيح ac, ثابت حرف **av, ثابت حرف *اڳياڙي,
			 بنية repository *repo اڻ_استعمال_ٿيل)
{
	صحيح force = 0;
	بنية option options[] = {
		OPT__FORCE(&force,
			 N_("force move even if worktree is dirty or locked"),
			 PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "relative-paths", &use_relative_paths,
			 N_("use relative paths for worktrees")),
		OPT_پڇاڙي()
	};
	بنية worktree **worktrees, *wt;
	بنية strbuf dst = STRBUF_INIT;
	بنية strbuf errmsg = STRBUF_INIT;
	ثابت حرف *reason = عدم;
	حرف *رستو;

	ac = parse_options(ac, av, اڳياڙي, options, git_worktree_move_usage,
			   0);
	إذا (ac != 2)
		usage_with_options(git_worktree_move_usage, options);

	رستو = اڳياڙي_filename(اڳياڙي, av[1]);
	strbuf_addstr(&dst, رستو);
	حرر(رستو);

	worktrees = get_worktrees(the_repository);
	wt = ڳوليو_worktree(worktrees, اڳياڙي, av[0]);
	إذا (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	إذا (is_main_worktree(wt))
		die(_("'%s' is a main working tree"), av[0]);
	إذا (is_directory(dst.buf)) {
		ثابت حرف *sep = ڳوليو_آخري_dir_sep(wt->رستو);

		إذا (!sep)
			die(_("could not figure out destination name from '%s'"),
			    wt->رستو);
		strbuf_trim_trailing_dir_sep(&dst);
		strbuf_addstr(&dst, sep);
	}
	check_candidate_رستو(dst.buf, force, worktrees, "move");

	validate_no_submodules(wt);

	إذا (force < 2)
		reason = worktree_تالو_reason(wt);
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

	إذا (rename(wt->رستو, dst.buf) == -1)
		die_errno(_("failed to move '%s' to '%s'"), wt->رستو, dst.buf);

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
				 ثابت حرف *original_رستو)
{
	بنية ٻار_process cp;
	حرف buf[1];
	صحيح ret;

	/*
	 * Until we sort this out, all submodules are "dirty" and
	 * will abort this function.
	 */
	validate_no_submodules(wt);

	ٻار_process_init(&cp);
	strvec_pushf(&cp.env, "%s=%s/.git",
		     GIT_DIR_ENVIRONMENT, wt->رستو);
	strvec_pushf(&cp.env, "%s=%s",
		     GIT_WORK_وڻ_ENVIRONMENT, wt->رستو);
	strvec_pushl(&cp.دليلون, "status",
		     "--porcelain", "--ignore-submodules=none",
		     عدم);
	cp.git_cmd = 1;
	cp.dir = wt->رستو;
	cp.ٻاهريون = -1;
	ret = شروعات_command(&cp);
	إذا (ret)
		die_errno(_("failed to run 'git status' on '%s'"),
			  original_رستو);
	ret = xread(cp.ٻاهريون, buf, حجم(buf));
	إذا (ret)
		die(_("'%s' contains modified or untracked files, use --force to delete it"),
		    original_رستو);
	close(cp.ٻاهريون);
	ret = پڄاڻي_command(&cp);
	إذا (ret)
		die_errno(_("failed to run 'git status' on '%s', code %d"),
			  original_رستو, ret);
}

ساكن صحيح delete_git_work_وڻ(بنية worktree *wt)
{
	بنية strbuf sb = STRBUF_INIT;
	صحيح ret = 0;

	strbuf_addstr(&sb, wt->رستو);
	إذا (remove_dir_recursively(&sb, 0)) {
		error_errno(_("failed to delete '%s'"), sb.buf);
		ret = -1;
	}
	strbuf_release(&sb);
	أرجع ret;
}

ساكن صحيح remove_worktree(صحيح ac, ثابت حرف **av, ثابت حرف *اڳياڙي,
			   بنية repository *repo اڻ_استعمال_ٿيل)
{
	صحيح force = 0;
	بنية option options[] = {
		OPT__FORCE(&force,
			 N_("force removal even if worktree is dirty or locked"),
			 PARSE_OPT_NOCOMPLETE),
		OPT_پڇاڙي()
	};
	بنية worktree **worktrees, *wt;
	بنية strbuf errmsg = STRBUF_INIT;
	ثابت حرف *reason = عدم;
	صحيح ret = 0;

	ac = parse_options(ac, av, اڳياڙي, options, git_worktree_remove_usage, 0);
	إذا (ac != 1)
		usage_with_options(git_worktree_remove_usage, options);

	worktrees = get_worktrees(the_repository);
	wt = ڳوليو_worktree(worktrees, اڳياڙي, av[0]);
	إذا (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	إذا (is_main_worktree(wt))
		die(_("'%s' is a main working tree"), av[0]);
	إذا (force < 2)
		reason = worktree_تالو_reason(wt);
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

	إذا (file_exists(wt->رستو)) {
		إذا (!force)
			check_clean_worktree(wt, av[0]);

		ret |= delete_git_work_وڻ(wt);
	}
	/*
	 * continue on even if ret is non-zero, there's no going back
	 * from here.
	 */
	ret |= delete_git_dir(wt->id);
	delete_worktrees_dir_if_خالي_جايون();

	free_worktrees(worktrees);
	أرجع ret;
}

ساكن فراغ report_repair(صحيح iserr, ثابت حرف *رستو, ثابت حرف *نياپو, فراغ *cb_معلومات)
{
	إذا (!iserr) {
		fprintf_ln(stderr, _("repair: %s: %s"), نياپو, رستو);
	} وإلا {
		صحيح *exit_حالت = (صحيح *)cb_معلومات;
		fprintf_ln(stderr, _("error: %s: %s"), نياپو, رستو);
		*exit_حالت = 1;
	}
}

ساكن صحيح repair(صحيح ac, ثابت حرف **av, ثابت حرف *اڳياڙي,
		  بنية repository *repo اڻ_استعمال_ٿيل)
{
	ثابت حرف **p;
	ثابت حرف *self[] = { ".", عدم };
	بنية option options[] = {
		OPT_BOOL(0, "relative-paths", &use_relative_paths,
			 N_("use relative paths for worktrees")),
		OPT_پڇاڙي()
	};
	صحيح rc = 0;

	ac = parse_options(ac, av, اڳياڙي, options, git_worktree_repair_usage, 0);
	p = ac > 0 ? av : self;
	لكل (; *p; p++)
		repair_worktree_at_رستو(the_repository, *p, report_repair,
					&rc, use_relative_paths);
	repair_worktrees(the_repository, report_repair, &rc, use_relative_paths);
	أرجع rc;
}

صحيح cmd_worktree(صحيح ac,
		 ثابت حرف **av,
		 ثابت حرف *اڳياڙي,
		 بنية repository *repo)
{
	parse_opt_subcommand_fn *fn = عدم;
	بنية option options[] = {
		OPT_SUBCOMMAND("add", &fn, جوڙيو),
		OPT_SUBCOMMAND("prune", &fn, prune),
		OPT_SUBCOMMAND("list", &fn, list),
		OPT_SUBCOMMAND("lock", &fn, تالو_worktree),
		OPT_SUBCOMMAND("unlock", &fn, unlock_worktree),
		OPT_SUBCOMMAND("move", &fn, move_worktree),
		OPT_SUBCOMMAND("remove", &fn, remove_worktree),
		OPT_SUBCOMMAND("repair", &fn, repair),
		OPT_پڇاڙي()
	};

	repo_config(the_repository, git_worktree_config, عدم);

	إذا (!اڳياڙي)
		اڳياڙي = "";

	ac = parse_options(ac, av, اڳياڙي, options, git_worktree_usage, 0);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_ڀريل_اشاريو = 0;

	أرجع fn(ac, av, اڳياڙي, repo);
}
