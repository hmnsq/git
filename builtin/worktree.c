#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "abspath.h"
#ଅନ୍ତର୍ଭାଵ "advice.h"
#ଅନ୍ତର୍ଭାଵ "checkout.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "copy.h"
#ଅନ୍ତର୍ଭାଵ "dir.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "hex.h"
#ଅନ୍ତର୍ଭାଵ "object-file.h"
#ଅନ୍ତର୍ଭାଵ "object-name.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "path.h"
#ଅନ୍ତର୍ଭାଵ "strvec.h"
#ଅନ୍ତର୍ଭାଵ "branch.h"
#ଅନ୍ତର୍ଭାଵ "read-cache-ll.h"
#ଅନ୍ତର୍ଭାଵ "refs.h"
#ଅନ୍ତର୍ଭାଵ "remote.h"
#ଅନ୍ତର୍ଭାଵ "run-command.h"
#ଅନ୍ତର୍ଭାଵ "hook.h"
#ଅନ୍ତର୍ଭାଵ "sigchain.h"
#ଅନ୍ତର୍ଭାଵ "submodule.h"
#ଅନ୍ତର୍ଭାଵ "utf8.h"
#ଅନ୍ତର୍ଭାଵ "worktree.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"

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

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ git_worktree_usage[] = {
	BUILTIN_WORKTREE_ADD_USAGE,
	BUILTIN_WORKTREE_LIST_USAGE,
	BUILTIN_WORKTREE_LOCK_USAGE,
	BUILTIN_WORKTREE_MOVE_USAGE,
	BUILTIN_WORKTREE_PRUNE_USAGE,
	BUILTIN_WORKTREE_REMOVE_USAGE,
	BUILTIN_WORKTREE_REPAIR_USAGE,
	BUILTIN_WORKTREE_UNLOCK_USAGE,
	ଖାଲି
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ git_worktree_add_usage[] = {
	BUILTIN_WORKTREE_ADD_USAGE,
	ଖାଲି,
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ git_worktree_list_usage[] = {
	BUILTIN_WORKTREE_LIST_USAGE,
	ଖାଲି
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ git_worktree_lock_usage[] = {
	BUILTIN_WORKTREE_LOCK_USAGE,
	ଖାଲି
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ git_worktree_move_usage[] = {
	BUILTIN_WORKTREE_MOVE_USAGE,
	ଖାଲି
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ git_worktree_prune_usage[] = {
	BUILTIN_WORKTREE_PRUNE_USAGE,
	ଖାଲି
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ git_worktree_remove_usage[] = {
	BUILTIN_WORKTREE_REMOVE_USAGE,
	ଖାଲି
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ git_worktree_repair_usage[] = {
	BUILTIN_WORKTREE_REPAIR_USAGE,
	ଖାଲି
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ git_worktree_unlock_usage[] = {
	BUILTIN_WORKTREE_UNLOCK_USAGE,
	ଖାଲି
};

ସଂରଚନା add_opts {
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା force;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା detach;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା quiet;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା checkout;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା orphan;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା relative_paths;
	ଅଚଳ ଅକ୍ଷର *keep_locked;
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_only;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା verbose;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା guess_remote;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା use_relative_paths;
ସ୍ଥିର timestamp_t expire;

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା git_worktree_config(ଅଚଳ ଅକ୍ଷର *var, ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ,
			       ଅଚଳ ସଂରଚନା config_context *ctx, ଶୂନ୍ୟ *cb)
{
	ଯଦି (!strcmp(var, "worktree.guessremote")) {
		guess_remote = git_config_bool(var, ମୂଲ୍ୟ);
		ଫେରନ୍ତୁ 0;
	} ଅନ୍ୟଥା ଯଦି (!strcmp(var, "worktree.userelativepaths")) {
		use_relative_paths = git_config_bool(var, ମୂଲ୍ୟ);
		ଫେରନ୍ତୁ 0;
	}

	ଫେରନ୍ତୁ git_default_config(var, ମୂଲ୍ୟ, ctx, cb);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା delete_git_dir(ଅଚଳ ଅକ୍ଷର *id)
{
	ସଂରଚନା strbuf sb = STRBUF_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	repo_common_path_append(the_repository, &sb, "worktrees/%s", id);
	ret = remove_dir_recursively(&sb, 0);
	ଯଦି (ret < 0 && errno == ENOTDIR)
		ret = unlink(sb.buf);
	ଯଦି (ret)
		error_errno(_("failed to delete '%s'"), sb.buf);
	strbuf_release(&sb);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ଶୂନ୍ୟ delete_worktrees_dir_if_empty(ଶୂନ୍ୟ)
{
	ଅକ୍ଷର *path = repo_git_path(the_repository, "worktrees");
	rmdir(path); /* ignore failed removal */
	ମୁକ୍ତ(path);
}

ସ୍ଥିର ଶୂନ୍ୟ prune_worktree(ଅଚଳ ଅକ୍ଷର *id, ଅଚଳ ଅକ୍ଷର *reason)
{
	ଯଦି (show_only || verbose)
		fprintf_ln(stderr, _("Removing %s/%s: %s"), "worktrees", id, reason);
	ଯଦି (!show_only)
		delete_git_dir(id);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା prune_cmp(ଅଚଳ ଶୂନ୍ୟ *a, ଅଚଳ ଶୂନ୍ୟ *b)
{
	ଅଚଳ ସଂରଚନା string_list_item *x = a;
	ଅଚଳ ସଂରଚନା string_list_item *y = b;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା c;

	ଯଦି ((c = fspathcmp(x->string, y->string)))
	    ଫେରନ୍ତୁ c;
	/*
	 * paths same; prune_dupes() removes all but the first worktree entry
	 * having the same path, so sort main worktree ('util' is NULL) above
	 * linked worktrees ('util' not NULL) since main worktree can't be
	 * removed
	 */
	ଯଦି (!x->util)
		ଫେରନ୍ତୁ -1;
	ଯଦି (!y->util)
		ଫେରନ୍ତୁ 1;
	/* paths same; sort by .git/worktrees/<id> */
	ଫେରନ୍ତୁ strcmp(x->util, y->util);
}

ସ୍ଥିର ଶୂନ୍ୟ prune_dups(ସଂରଚନା string_list *l)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	QSORT(l->ବସ୍ତୁଗୁଡ଼ିକ, l->nr, prune_cmp);
	ପାଇଁ (i = 1; i < l->nr; i++) {
		ଯଦି (!fspathcmp(l->ବସ୍ତୁଗୁଡ଼ିକ[i].string, l->ବସ୍ତୁଗୁଡ଼ିକ[i - 1].string))
			prune_worktree(l->ବସ୍ତୁଗୁଡ଼ିକ[i].util, "duplicate entry");
	}
}

ସ୍ଥିର ଶୂନ୍ୟ prune_worktrees(ଶୂନ୍ୟ)
{
	ସଂରଚନା strbuf reason = STRBUF_INIT;
	ସଂରଚନା strbuf main_path = STRBUF_INIT;
	ସଂରଚନା string_list kept = STRING_LIST_INIT_DUP;
	ଅକ୍ଷର *path;
	DIR *dir;
	ସଂରଚନା dirent *d;

	path = repo_git_path(the_repository, "worktrees");
	dir = opendir(path);
	ମୁକ୍ତ(path);
	ଯଦି (!dir)
		ଫେରନ୍ତୁ;
	ଯେତେବେଳେ ((d = readdir_skip_dot_and_dotdot(dir)) != ଖାଲି) {
		ଅକ୍ଷର *path;
		strbuf_reset(&reason);
		ଯଦି (should_prune_worktree(the_repository, d->d_name, &reason, &path, expire))
			prune_worktree(d->d_name, reason.buf);
		ଅନ୍ୟଥା ଯଦି (path)
			string_list_append_nodup(&kept, path)->util = xstrdup(d->d_name);
	}
	closedir(dir);

	strbuf_add_absolute_path(&main_path, repo_get_common_dir(the_repository));
	/* massage main worktree absolute path to match 'gitdir' content */
	strbuf_strip_suffix(&main_path, "/.");
	string_list_append_nodup(&kept, strbuf_detach(&main_path, ଖାଲି));
	prune_dups(&kept);
	string_list_clear(&kept, 1);

	ଯଦି (!show_only)
		delete_worktrees_dir_if_empty();
	strbuf_release(&reason);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା prune(ପୂର୍ଣ୍ଣସଂଖ୍ୟା ac, ଅଚଳ ଅକ୍ଷର **av, ଅଚଳ ଅକ୍ଷର *prefix,
		 ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା option options[] = {
		OPT__DRY_RUN(&show_only, N_("do not remove, show only")),
		OPT__VERBOSE(&verbose, N_("report pruned working trees")),
		OPT_EXPIRY_DATE(0, "expire", &expire,
				N_("prune missing working trees older than <time>")),
		OPT_END()
	};

	expire = TIME_MAX;
	ac = parse_options(ac, av, prefix, options, git_worktree_prune_usage,
			   0);
	ଯଦି (ac)
		usage_with_options(git_worktree_prune_usage, options);
	prune_worktrees();
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଅକ୍ଷର *junk_work_ବୃକ୍ଷ;
ସ୍ଥିର ଅକ୍ଷର *junk_git_dir;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା is_junk;
ସ୍ଥିର pid_t junk_pid;

ସ୍ଥିର ଶୂନ୍ୟ remove_junk(ଶୂନ୍ୟ)
{
	ସଂରଚନା strbuf sb = STRBUF_INIT;
	ଯଦି (!is_junk || getpid() != junk_pid)
		ଫେରନ୍ତୁ;
	ଯଦି (junk_git_dir) {
		strbuf_addstr(&sb, junk_git_dir);
		remove_dir_recursively(&sb, 0);
		strbuf_reset(&sb);
	}
	ଯଦି (junk_work_ବୃକ୍ଷ) {
		strbuf_addstr(&sb, junk_work_ବୃକ୍ଷ);
		remove_dir_recursively(&sb, 0);
	}
	strbuf_release(&sb);
}

ସ୍ଥିର ଶୂନ୍ୟ remove_junk_on_signal(ପୂର୍ଣ୍ଣସଂଖ୍ୟା signo)
{
	remove_junk();
	sigchain_କାଢ଼(signo);
	ଉଠାଅ(signo);
}

ସ୍ଥିର ଅକ୍ଷର *worktree_basename_dup(ଅଚଳ ଅକ୍ଷର *path)
{
	ଅଚଳ ଅକ୍ଷର *name;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା len;

	len = strlen(path);
	ଯେତେବେଳେ (len && is_dir_sep(path[len - 1]))
		len--;

	name = path + len;
	ଯେତେବେଳେ (name > path && !is_dir_sep(name[-1]))
		name--;

	ଫେରନ୍ତୁ xmemdupz(name, path + len - name);
}

/* check that path is viable location for worktree */
ସ୍ଥିର ଶୂନ୍ୟ check_candidate_path(ଅଚଳ ଅକ୍ଷର *path,
				 ପୂର୍ଣ୍ଣସଂଖ୍ୟା force,
				 ସଂରଚନା worktree **worktrees,
				 ଅଚଳ ଅକ୍ଷର *cmd)
{
	ସଂରଚନା worktree *wt;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା locked;

	ଯଦି (file_exists(path) && !is_empty_dir(path))
		die(_("'%s' already exists"), path);

	wt = find_worktree_by_path(worktrees, path);
	ଯଦି (!wt)
		ଫେରନ୍ତୁ;

	locked = !!worktree_lock_reason(wt);
	ଯଦି ((!locked && force) || (locked && force > 1)) {
		ଯଦି (delete_git_dir(wt->id))
		    die(_("unusable worktree destination '%s'"), path);
		ଫେରନ୍ତୁ;
	}

	ଯଦି (locked)
		die(_("'%s' is a missing but locked worktree;\nuse '%s -f -f' to override, or 'unlock' and 'prune' or 'remove' to clear"), path, cmd);
	ଅନ୍ୟଥା
		die(_("'%s' is a missing but already registered worktree;\nuse '%s -f' to override, or 'prune' or 'remove' to clear"), path, cmd);
}

ସ୍ଥିର ଶୂନ୍ୟ copy_sparse_checkout(ଅଚଳ ଅକ୍ଷର *worktree_git_dir)
{
	ଅକ୍ଷର *from_file = repo_git_path(the_repository, "info/sparse-checkout");
	ଅକ୍ଷର *to_file = xstrfmt("%s/info/sparse-checkout", worktree_git_dir);

	ଯଦି (file_exists(from_file)) {
		ଯଦି (safe_create_leading_directories(the_repository, to_file) ||
			copy_file(the_repository, to_file, from_file, 0666))
			error(_("failed to copy '%s' to '%s'; sparse-checkout may not work correctly"),
				from_file, to_file);
	}

	ମୁକ୍ତ(from_file);
	ମୁକ୍ତ(to_file);
}

ସ୍ଥିର ଶୂନ୍ୟ copy_filtered_worktree_config(ଅଚଳ ଅକ୍ଷର *worktree_git_dir)
{
	ଅକ୍ଷର *from_file = repo_git_path(the_repository, "config.worktree");
	ଅକ୍ଷର *to_file = xstrfmt("%s/config.worktree", worktree_git_dir);

	ଯଦି (file_exists(from_file)) {
		ସଂରଚନା config_set cs = { { 0 } };
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା bare;

		ଯଦି (safe_create_leading_directories(the_repository, to_file) ||
			copy_file(the_repository, to_file, from_file, 0666)) {
			error(_("failed to copy worktree config from '%s' to '%s'"),
				from_file, to_file);
			ଯାଅ worktree_copy_cleanup;
		}

		git_configset_init(&cs);
		git_configset_add_file(&cs, from_file);

		ଯଦି (!git_configset_get_bool(&cs, "core.bare", &bare) &&
			bare &&
			repo_config_set_multivar_in_file_gently(the_repository,
				to_file, "core.bare", ଖାଲି, "true", ଖାଲି, 0))
			error(_("failed to unset '%s' in '%s'"),
				"core.bare", to_file);
		ଯଦି (!git_configset_get(&cs, "core.worktree") &&
			repo_config_set_in_file_gently(the_repository, to_file,
						       "core.worktree", ଖାଲି, ଖାଲି))
			error(_("failed to unset '%s' in '%s'"),
				"core.worktree", to_file);

		git_configset_clear(&cs);
	}

worktree_copy_cleanup:
	ମୁକ୍ତ(from_file);
	ମୁକ୍ତ(to_file);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା checkout_worktree(ଅଚଳ ସଂରଚନା add_opts *opts,
			     ସଂରଚନା strvec *child_env)
{
	ସଂରଚନା child_process cp = CHILD_PROCESS_INIT;
	cp.git_cmd = 1;
	strvec_pushl(&cp.args, "reset", "--hard", "--no-recurse-submodules", ଖାଲି);
	ଯଦି (opts->quiet)
		strvec_ଠେଲ(&cp.args, "--quiet");
	strvec_pushv(&cp.env, child_env->v);
	ଫେରନ୍ତୁ run_command(&cp);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା make_worktree_orphan(ଅଚଳ ଅକ୍ଷର * ref, ଅଚଳ ସଂରଚନା add_opts *opts,
				ସଂରଚନା strvec *child_env)
{
	ସଂରଚନା strbuf symref = STRBUF_INIT;
	ସଂରଚନା child_process cp = CHILD_PROCESS_INIT;

	validate_new_branchname(ref, &symref, 0);
	strvec_pushl(&cp.args, "symbolic-ref", "HEAD", symref.buf, ଖାଲି);
	ଯଦି (opts->quiet)
		strvec_ଠେଲ(&cp.args, "--quiet");
	strvec_pushv(&cp.env, child_env->v);
	strbuf_release(&symref);
	cp.git_cmd = 1;
	ଫେରନ୍ତୁ run_command(&cp);
}

/*
 * References for worktrees are generally stored in '$GIT_DIR/worktrees/<wt_id>'.
 * But when using alternate reference directories, we want to store the worktree
 * references in '$ALTERNATE_REFERENCE_DIR/worktrees/<wt_id>'.
 *
 * Create the necessary folder structure to facilitate the same. But to ensure
 * that the former path is still considered a Git directory, add stubs.
 */
ସ୍ଥିର ଶୂନ୍ୟ setup_alternate_ref_dir(ସଂରଚନା worktree *wt, ଅଚଳ ଅକ୍ଷର *wt_git_path)
{
	ସଂରଚନା strbuf sb = STRBUF_INIT;
	ଅକ୍ଷର *path;

	path = wt->repo->ref_storage_payload;
	ଯଦି (!path)
		ଫେରନ୍ତୁ;

	ଯଦି (!is_absolute_path(path))
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

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା add_worktree(ଅଚଳ ଅକ୍ଷର *path, ଅଚଳ ଅକ୍ଷର *refname,
			ଅଚଳ ସଂରଚନା add_opts *opts)
{
	ସଂରଚନା strbuf sb_git = STRBUF_INIT, sb_repo = STRBUF_INIT;
	ସଂରଚନା strbuf sb = STRBUF_INIT;
	ଅଚଳ ଅକ୍ଷର *name;
	ଅକ୍ଷର *name_to_free = ଖାଲି;
	ସଂରଚନା strvec child_env = STRVEC_INIT;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା counter = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ret;
	ସଂରଚନା strbuf symref = STRBUF_INIT;
	ସଂରଚନା commit *commit = ଖାଲି;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା is_branch = 0;
	ସଂରଚନା strbuf sb_name = STRBUF_INIT;
	ସଂରଚନା worktree **worktrees, *wt = ଖାଲି;
	ସଂରଚନା ref_store *wt_refs;
	ସଂରଚନା repo_config_ମୂଲ୍ୟଗୁଡ଼ିକ *cfg = repo_config_ମୂଲ୍ୟଗୁଡ଼ିକ(the_repository);

	worktrees = get_worktrees(the_repository);
	check_candidate_path(path, opts->force, worktrees, "add");
	free_worktrees(worktrees);
	worktrees = ଖାଲି;

	/* is 'refname' a branch or commit? */
	ଯଦି (!opts->detach && !check_branch_ref(the_repository, &symref, refname) &&
	    refs_ref_exists(get_main_ref_store(the_repository), symref.buf)) {
		is_branch = 1;
		ଯଦି (!opts->force)
			die_if_checked_out(symref.buf, 0);
	}
	commit = lookup_commit_reference_by_name(refname);
	ଯଦି (!commit && !opts->orphan)
		die(_("invalid reference: %s"), refname);

	name = name_to_free = worktree_basename_dup(path);
	ଯଦି (!*name)
		die(_("invalid path '%s'"), path);
	sanitize_refname_component(name, &sb_name);
	ଯଦି (!sb_name.len)
		BUG("How come '%s' becomes empty after sanitization?", name);
	name = sb_name.buf;
	repo_git_path_replace(the_repository, &sb_repo, "worktrees/%s", name);
	len = sb_repo.len;
	ଯଦି (safe_create_leading_directories_const(the_repository, sb_repo.buf))
		die_errno(_("could not create leading directories of '%s'"),
			  sb_repo.buf);

	ଯେତେବେଳେ (mkdir(sb_repo.buf, 0777)) {
		counter++;
		ଯଦି ((errno != EEXIST) || !counter /* overflow */)
			die_errno(_("could not create directory of '%s'"),
				  sb_repo.buf);
		strbuf_setlen(&sb_repo, len);
		strbuf_addf(&sb_repo, "%d", counter);
	}
	name = strrchr(sb_repo.buf, '/') + 1;

	junk_pid = getpid();
	atexit(remove_junk);
	sigchain_ଠେଲ_common(remove_junk_on_signal);

	junk_git_dir = xstrdup(sb_repo.buf);
	is_junk = 1;

	/*
	 * lock the incomplete repo so prune won't delete it, unlock
	 * after the preparation is over.
	 */
	strbuf_addf(&sb, "%s/locked", sb_repo.buf);
	ଯଦି (opts->keep_locked)
		write_file(sb.buf, "%s", opts->keep_locked);
	ଅନ୍ୟଥା
		write_file(sb.buf, _("initializing"));

	strbuf_addf(&sb_git, "%s/.git", path);
	ଯଦି (safe_create_leading_directories_const(the_repository, sb_git.buf))
		die_errno(_("could not create leading directories of '%s'"),
			  sb_git.buf);
	junk_work_ବୃକ୍ଷ = xstrdup(path);

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
	ଯଦି (!wt) {
		ret = error(_("could not find created worktree '%s'"), name);
		ଯାଅ done;
	}
	setup_alternate_ref_dir(wt, sb_repo.buf);
	wt_refs = get_worktree_ref_store(wt);

	ret = ref_store_create_on_disk(wt_refs, REF_STORE_CREATE_ON_DISK_IS_WORKTREE, &sb);
	ଯଦି (ret)
		ଯାଅ done;

	ଯଦି (!is_branch && commit)
		ret = refs_update_ref(wt_refs, ଖାଲି, "HEAD", &commit->object.oid,
				      ଖାଲି, 0, UPDATE_REFS_ସନ୍ଦେଶ_ON_ERR);
	ଅନ୍ୟଥା
		ret = refs_update_symref(wt_refs, "HEAD", symref.buf, ଖାଲି);
	ଯଦି (ret)
		ଯାଅ done;

	/*
	 * If the current worktree has sparse-checkout enabled, then copy
	 * the sparse-checkout patterns from the current worktree.
	 */
	ଯଦି (cfg->apply_sparse_checkout)
		copy_sparse_checkout(sb_repo.buf);

	/*
	 * If we are using worktree config, then copy all current config
	 * values from the current worktree into the new one, that way the
	 * new worktree behaves the same as this one.
	 */
	ଯଦି (the_repository->repository_format_worktree_config)
		copy_filtered_worktree_config(sb_repo.buf);

	strvec_pushf(&child_env, "%s=%s", GIT_DIR_ENVIRONMENT, sb_git.buf);
	strvec_pushf(&child_env, "%s=%s", GIT_WORK_ବୃକ୍ଷ_ENVIRONMENT, path);

	ଯଦି (opts->orphan &&
	    (ret = make_worktree_orphan(refname, opts, &child_env)))
		ଯାଅ done;

	ଯଦି (opts->checkout &&
	    (ret = checkout_worktree(opts, &child_env)))
		ଯାଅ done;

	is_junk = 0;
	FREE_AND_NULL(junk_work_ବୃକ୍ଷ);
	FREE_AND_NULL(junk_git_dir);

done:
	ଯଦି (ret || !opts->keep_locked) {
		strbuf_reset(&sb);
		strbuf_addf(&sb, "%s/locked", sb_repo.buf);
		unlink_or_warn(sb.buf);
	}

	/*
	 * Hook failure does not warrant worktree deletion, so run hook after
	 * is_junk is cleared, but do return appropriate code when hook fails.
	 */
	ଯଦି (!ret && opts->checkout && !opts->orphan) {
		ସଂରଚନା run_hooks_opt opt = RUN_HOOKS_OPT_INIT_FORCE_SERIAL;

		strvec_pushl(&opt.env, "GIT_DIR", "GIT_WORK_TREE", ଖାଲି);
		strvec_pushl(&opt.args,
			     oid_to_hex(null_oid(the_hash_algo)),
			     oid_to_hex(&commit->object.oid),
			     "1",
			     ଖାଲି);
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
	ମୁକ୍ତ(name_to_free);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ଶୂନ୍ୟ print_preparing_worktree_ଧାଡ଼ି(ପୂର୍ଣ୍ଣସଂଖ୍ୟା detach,
					  ଅଚଳ ଅକ୍ଷର *branch,
					  ଅଚଳ ଅକ୍ଷର *new_branch,
					  ପୂର୍ଣ୍ଣସଂଖ୍ୟା force_new_branch)
{
	ଯଦି (force_new_branch) {
		ସଂରଚନା commit *commit = lookup_commit_reference_by_name(new_branch);
		ଯଦି (!commit)
			fprintf_ln(stderr, _("Preparing worktree (new branch '%s')"), new_branch);
		ଅନ୍ୟଥା
			fprintf_ln(stderr, _("Preparing worktree (resetting branch '%s'; was at %s)"),
				  new_branch,
				  repo_find_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV));
	} ଅନ୍ୟଥା ଯଦି (new_branch) {
		fprintf_ln(stderr, _("Preparing worktree (new branch '%s')"), new_branch);
	} ଅନ୍ୟଥା {
		ସଂରଚନା strbuf s = STRBUF_INIT;
		ଯଦି (!detach && !check_branch_ref(the_repository, &s, branch) &&
		    refs_ref_exists(get_main_ref_store(the_repository), s.buf))
			fprintf_ln(stderr, _("Preparing worktree (checking out '%s')"),
				  branch);
		ଅନ୍ୟଥା {
			ସଂରଚନା commit *commit = lookup_commit_reference_by_name(branch);
			ଯଦି (!commit)
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
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା first_valid_ref(ଅଚଳ ସଂରଚନା reference *ref UNUSED, ଶୂନ୍ୟ *cb_data UNUSED)
{
	ଫେରନ୍ତୁ 1;
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
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା can_use_local_refs(ଅଚଳ ସଂରଚନା add_opts *opts)
{
	ଯଦି (refs_head_ref(get_main_ref_store(the_repository), first_valid_ref, ଖାଲି)) {
		ଫେରନ୍ତୁ 1;
	} ଅନ୍ୟଥା ଯଦି (refs_for_each_branch_ref(get_main_ref_store(the_repository), first_valid_ref, ଖାଲି)) {
		ଯଦି (!opts->quiet)
			warning(_("HEAD points to an invalid (or orphaned) reference.\n"));
		ଫେରନ୍ତୁ 1;
	}
	ଫେରନ୍ତୁ 0;
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
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା can_use_remote_refs(ଅଚଳ ସଂରଚନା add_opts *opts)
{
	ଯଦି (!guess_remote) {
		ଫେରନ୍ତୁ 0;
	} ଅନ୍ୟଥା ଯଦି (refs_for_each_remote_ref(get_main_ref_store(the_repository), first_valid_ref, ଖାଲି)) {
		ଫେରନ୍ତୁ 1;
	} ଅନ୍ୟଥା ଯଦି (!opts->force && remote_get(ଖାଲି)) {
		die(_("No local or remote refs exist despite at least one remote\n"
		      "present, stopping; use 'add -f' to override or fetch a remote first"));
	}
	ଫେରନ୍ତୁ 0;
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
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା dwim_orphan(ଅଚଳ ସଂରଚନା add_opts *opts, ପୂର୍ଣ୍ଣସଂଖ୍ୟା opt_track, ପୂର୍ଣ୍ଣସଂଖ୍ୟା remote)
{
	ଯଦି (can_use_local_refs(opts)) {
		ଫେରନ୍ତୁ 0;
	} ଅନ୍ୟଥା ଯଦି (remote && can_use_remote_refs(opts)) {
		ଫେରନ୍ତୁ 0;
	} ଅନ୍ୟଥା ଯଦି (!opts->quiet) {
		fprintf_ln(stderr, WORKTREE_ADD_DWIM_ORPHAN_INFER_TEXT);
	}

	ଯଦି (opt_track) {
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--track");
	} ଅନ୍ୟଥା ଯଦି (!opts->checkout) {
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--no-checkout");
	}
	ଫେରନ୍ତୁ 1;
}

ସ୍ଥିର ଶୂନ୍ୟ advise_disambiguating_remotes(ଅଚଳ ଅକ୍ଷର *path, ଅଚଳ ଅକ୍ଷର *branch,
					  ଅଚଳ ସଂରଚନା string_list *matched_remote_names)
{
	ସଂରଚନା string_list_item *item;

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

ସ୍ଥିର ଅକ୍ଷର *dwim_branch(ଅଚଳ ସଂରଚନା add_opts *opts, ଅଚଳ ଅକ୍ଷର *path, ଅକ୍ଷର **new_branch)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା branch_exists;
	ଅକ୍ଷର *branchname = worktree_basename_dup(path);
	ସଂରଚନା strbuf ref = STRBUF_INIT;

	branch_exists = !check_branch_ref(the_repository, &ref, branchname) &&
			refs_ref_exists(get_main_ref_store(the_repository),
					ref.buf);
	strbuf_release(&ref);
	ଯଦି (branch_exists)
		ଫେରନ୍ତୁ branchname;

	*new_branch = branchname;
	ଯଦି (guess_remote) {
		ସଂରଚନା object_id oid;
		ଅକ୍ଷର *remote;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_matches = 0;
		ସଂରଚନା string_list matched_remote_names = STRING_LIST_INIT_DUP;

		remote = unique_tracking_name(*new_branch, &oid, &ଅଙ୍କ_matches,
					      &matched_remote_names);
		ଯଦି (!remote && ଅଙ୍କ_matches > 1) {
			ଯଦି (!opts->quiet &&
			    advice_enabled(ADVICE_CHECKOUT_AMBIGUOUS_REMOTE_BRANCH_NAME))
				advise_disambiguating_remotes(path, *new_branch,
							      &matched_remote_names);
			die(_("'%s' matched multiple (%d) remote tracking branches"),
			    *new_branch, ଅଙ୍କ_matches);
		}
		string_list_clear(&matched_remote_names, 0);
		ଫେରନ୍ତୁ remote;
	}
	ଫେରନ୍ତୁ ଖାଲି;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା add(ପୂର୍ଣ୍ଣସଂଖ୍ୟା ac, ଅଚଳ ଅକ୍ଷର **av, ଅଚଳ ଅକ୍ଷର *prefix,
	       ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା add_opts opts;
	ଅଚଳ ଅକ୍ଷର *new_branch_force = ଖାଲି;
	ଅକ୍ଷର *path;
	ଅଚଳ ଅକ୍ଷର *branch;
	ଅକ୍ଷର *branch_to_free = ଖାଲି;
	ଅକ୍ଷର *new_branch_to_free = ଖାଲି;
	ଅଚଳ ଅକ୍ଷର *new_branch = ଖାଲି;
	ଅକ୍ଷର *opt_track = ଖାଲି;
	ଅଚଳ ଅକ୍ଷର *lock_reason = ଖାଲି;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା keep_locked = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା used_new_branch_options;
	ସଂରଚନା option options[] = {
		OPT__FORCE(&opts.force,
			   N_("checkout <branch> even if already checked out in other worktree"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_STRING('b', ଖାଲି, &new_branch, N_("branch"),
			   N_("create a new branch")),
		OPT_STRING('B', ଖାଲି, &new_branch_force, N_("branch"),
			   N_("create or reset a branch")),
		OPT_BOOL(0, "orphan", &opts.orphan, N_("create unborn branch")),
		OPT_BOOL('d', "detach", &opts.detach, N_("detach HEAD at named commit")),
		OPT_BOOL(0, "checkout", &opts.checkout, N_("populate the new working tree")),
		OPT_BOOL(0, "lock", &keep_locked, N_("keep the new working tree locked")),
		OPT_STRING(0, "reason", &lock_reason, N_("string"),
			   N_("reason for locking")),
		OPT__QUIET(&opts.quiet, N_("suppress progress reporting")),
		OPT_PASSTHRU(0, "track", &opt_track, ଖାଲି,
			     N_("set up tracking mode (see git-branch(1))"),
			     PARSE_OPT_NOARG | PARSE_OPT_OPTARG),
		OPT_BOOL(0, "guess-remote", &guess_remote,
			 N_("try to match the new branch name with a remote-tracking branch")),
		OPT_BOOL(0, "relative-paths", &opts.relative_paths,
			 N_("use relative paths for worktrees")),
		OPT_END()
	};
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	memset(&opts, 0, ଆକାର(opts));
	opts.checkout = 1;
	opts.relative_paths = use_relative_paths;
	ac = parse_options(ac, av, prefix, options, git_worktree_add_usage, 0);
	ଯଦି (!!opts.detach + !!new_branch + !!new_branch_force > 1)
		die(_("options '%s', '%s', and '%s' cannot be used together"), "-b", "-B", "--detach");
	ଯଦି (opts.detach && opts.orphan)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--detach");
	ଯଦି (opts.orphan && opt_track)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--track");
	ଯଦି (opts.orphan && !opts.checkout)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--orphan", "--no-checkout");
	ଯଦି (opts.orphan && ac == 2)
		die(_("option '%s' and commit-ish cannot be used together"),
		    "--orphan");
	ଯଦି (lock_reason && !keep_locked)
		die(_("the option '%s' requires '%s'"), "--reason", "--lock");
	ଯଦି (lock_reason)
		opts.keep_locked = lock_reason;
	ଅନ୍ୟଥା ଯଦି (keep_locked)
		opts.keep_locked = _("added with --lock");

	ଯଦି (ac < 1 || ac > 2)
		usage_with_options(git_worktree_add_usage, options);

	path = prefix_filename(prefix, av[0]);
	branch = ac < 2 ? "HEAD" : av[1];
	used_new_branch_options = new_branch || new_branch_force;

	ଯଦି (!strcmp(branch, "-"))
		branch = "@{-1}";

	ଯଦି (new_branch_force) {
		ସଂରଚନା strbuf symref = STRBUF_INIT;

		new_branch = new_branch_force;

		ଯଦି (!opts.force &&
		    !check_branch_ref(the_repository, &symref, new_branch) &&
		    refs_ref_exists(get_main_ref_store(the_repository), symref.buf))
			die_if_checked_out(symref.buf, 0);
		strbuf_release(&symref);
	}

	ଯଦି (opts.orphan && !new_branch) {
		new_branch = new_branch_to_free = worktree_basename_dup(path);
	} ଅନ୍ୟଥା ଯଦି (opts.orphan) {
		; /* no-op */
	} ଅନ୍ୟଥା ଯଦି (opts.detach) {
		/* Check HEAD */
		ଯଦି (!strcmp(branch, "HEAD"))
			can_use_local_refs(&opts);
	} ଅନ୍ୟଥା ଯଦି (ac < 2 && new_branch) {
		/* DWIM: Infer --orphan when repo has no refs. */
		opts.orphan = dwim_orphan(&opts, !!opt_track, 0);
	} ଅନ୍ୟଥା ଯଦି (ac < 2) {
		/* DWIM: Guess branch name from path. */
		ଅକ୍ଷର *s = dwim_branch(&opts, path, &new_branch_to_free);
		ଯଦି (s)
			branch = branch_to_free = s;
		new_branch = new_branch_to_free;

		/* DWIM: Infer --orphan when repo has no refs. */
		opts.orphan = (!s) && dwim_orphan(&opts, !!opt_track, 1);
	} ଅନ୍ୟଥା ଯଦି (ac == 2 && new_branch) {
		ଯଦି (!strcmp(branch, "HEAD"))
			can_use_local_refs(&opts);
	} ଅନ୍ୟଥା ଯଦି (ac == 2) {
		ସଂରଚନା commit *commit;

		commit = lookup_commit_reference_by_name(branch);
		ଯଦି (!commit) {
			ସଂରଚନା object_id oid;
			ଅକ୍ଷର *remote;
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_matches = 0;
			ସଂରଚନା string_list matched_remote_names = STRING_LIST_INIT_DUP;

			remote = unique_tracking_name(branch, &oid, &ଅଙ୍କ_matches,
						      &matched_remote_names);
			ଯଦି (remote) {
				new_branch = branch;
				branch = new_branch_to_free = remote;
			} ଅନ୍ୟଥା ଯଦି (ଅଙ୍କ_matches > 1) {
				ଯଦି (!opts.quiet &&
				    advice_enabled(ADVICE_CHECKOUT_AMBIGUOUS_REMOTE_BRANCH_NAME))
					advise_disambiguating_remotes(path, branch,
								      &matched_remote_names);
				die(_("'%s' matched multiple (%d) remote tracking branches"),
				    branch, ଅଙ୍କ_matches);
			}
			string_list_clear(&matched_remote_names, 0);
		}

		ଯଦି (!strcmp(branch, "HEAD"))
			can_use_local_refs(&opts);

	}

	ଯଦି (!opts.orphan && !lookup_commit_reference_by_name(branch)) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା attempt_hint = !opts.quiet && (ac < 2);
		ଯଦି (attempt_hint && used_new_branch_options) {
			advise_if_enabled(ADVICE_WORKTREE_ADD_ORPHAN,
				WORKTREE_ADD_ORPHAN_WITH_DASH_B_HINT_TEXT,
				new_branch, path);
		} ଅନ୍ୟଥା ଯଦି (attempt_hint) {
			advise_if_enabled(ADVICE_WORKTREE_ADD_ORPHAN,
				WORKTREE_ADD_ORPHAN_NO_DASH_B_HINT_TEXT, path);
		}
		die(_("invalid reference: %s"), branch);
	}

	ଯଦି (!opts.quiet)
		print_preparing_worktree_ଧାଡ଼ି(opts.detach, branch, new_branch, !!new_branch_force);

	ଯଦି (opts.orphan) {
		branch = new_branch;
	} ଅନ୍ୟଥା ଯଦି (new_branch) {
		ସଂରଚନା child_process cp = CHILD_PROCESS_INIT;
		cp.git_cmd = 1;
		strvec_ଠେଲ(&cp.args, "branch");
		ଯଦି (new_branch_force)
			strvec_ଠେଲ(&cp.args, "--force");
		ଯଦି (opts.quiet)
			strvec_ଠେଲ(&cp.args, "--quiet");
		strvec_ଠେଲ(&cp.args, new_branch);
		strvec_ଠେଲ(&cp.args, branch);
		ଯଦି (opt_track)
			strvec_ଠେଲ(&cp.args, opt_track);
		ଯଦି (run_command(&cp)) {
			ret = -1;
			ଯାଅ cleanup;
		}
		branch = new_branch;
	} ଅନ୍ୟଥା ଯଦି (opt_track) {
		die(_("--[no-]track can only be used if a new branch is created"));
	}

	ret = add_worktree(path, branch, &opts);
cleanup:
	ମୁକ୍ତ(path);
	ମୁକ୍ତ(opt_track);
	ମୁକ୍ତ(branch_to_free);
	ମୁକ୍ତ(new_branch_to_free);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ଶୂନ୍ୟ show_worktree_porcelain(ସଂରଚନା worktree *wt, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଧାଡ଼ି_terminator)
{
	ଅଚଳ ଅକ୍ଷର *reason;

	printf("worktree %s%c", wt->path, ଧାଡ଼ି_terminator);
	ଯଦି (wt->is_bare)
		printf("bare%c", ଧାଡ଼ି_terminator);
	ଅନ୍ୟଥା {
		printf("HEAD %s%c", oid_to_hex(&wt->head_oid), ଧାଡ଼ି_terminator);
		ଯଦି (wt->is_detached)
			printf("detached%c", ଧାଡ଼ି_terminator);
		ଅନ୍ୟଥା ଯଦି (wt->head_ref)
			printf("branch %s%c", wt->head_ref, ଧାଡ଼ି_terminator);
	}

	reason = worktree_lock_reason(wt);
	ଯଦି (reason) {
		fputs("locked", stdout);
		ଯଦି (*reason) {
			fputc(' ', stdout);
			write_name_quoted(reason, stdout, ଧାଡ଼ି_terminator);
		} ଅନ୍ୟଥା {
			fputc(ଧାଡ଼ି_terminator, stdout);
		}
	}

	reason = worktree_prune_reason(wt, expire);
	ଯଦି (reason)
		printf("prunable %s%c", reason, ଧାଡ଼ି_terminator);

	fputc(ଧାଡ଼ି_terminator, stdout);
}

ସଂରଚନା worktree_display {
	ଅକ୍ଷର *path;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା width;
};

ସ୍ଥିର ଶୂନ୍ୟ show_worktree(ସଂରଚନା worktree *wt, ସଂରଚନା worktree_display *display,
			  ପୂର୍ଣ୍ଣସଂଖ୍ୟା path_maxwidth, ପୂର୍ଣ୍ଣସଂଖ୍ୟା abbrev_len)
{
	ସଂରଚନା strbuf sb = STRBUF_INIT;
	ଅଚଳ ଅକ୍ଷର *reason;

	strbuf_addf(&sb, "%s%*s", display->path, 1 + path_maxwidth - display->width, "");
	ଯଦି (wt->is_bare)
		strbuf_addstr(&sb, "(bare)");
	ଅନ୍ୟଥା {
		strbuf_addf(&sb, "%-*s ", abbrev_len,
				repo_find_unique_abbrev(the_repository, &wt->head_oid, DEFAULT_ABBREV));
		ଯଦି (wt->is_detached)
			strbuf_addstr(&sb, "(detached HEAD)");
		ଅନ୍ୟଥା ଯଦି (wt->head_ref) {
			ଅକ୍ଷର *ref = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
								 wt->head_ref,
								 0);
			strbuf_addf(&sb, "[%s]", ref);
			ମୁକ୍ତ(ref);
		} ଅନ୍ୟଥା
			strbuf_addstr(&sb, "(error)");
	}

	reason = worktree_lock_reason(wt);
	ଯଦି (verbose && reason && *reason)
		strbuf_addf(&sb, "\n\tlocked: %s", reason);
	ଅନ୍ୟଥା ଯଦି (reason)
		strbuf_addstr(&sb, " locked");

	reason = worktree_prune_reason(wt, expire);
	ଯଦି (verbose && reason)
		strbuf_addf(&sb, "\n\tprunable: %s", reason);
	ଅନ୍ୟଥା ଯଦି (reason)
		strbuf_addstr(&sb, " prunable");

	printf("%s\n", sb.buf);
	strbuf_release(&sb);
}

ସ୍ଥିର ଶୂନ୍ୟ measure_widths(ସଂରଚନା worktree **wt, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *abbrev,
			   ସଂରଚନା worktree_display **d, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *maxwidth)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, display_alloc = 0;
	ସଂରଚନା worktree_display *display = ଖାଲି;
	ସଂରଚନା strbuf buf = STRBUF_INIT;

	ପାଇଁ (i = 0; wt[i]; i++) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା sha1_len;
		ALLOC_GROW(display, i + 1, display_alloc);
		quote_path(wt[i]->path, ଖାଲି, &buf, 0);
		display[i].width = utf8_strwidth(buf.buf);
		display[i].path = strbuf_detach(&buf, ଖାଲି);

		ଯଦି (display[i].width > *maxwidth)
			*maxwidth = display[i].width;
		sha1_len = strlen(repo_find_unique_abbrev(the_repository, &wt[i]->head_oid, *abbrev));
		ଯଦି (sha1_len > *abbrev)
			*abbrev = sha1_len;
	}
	*d = display;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା pathcmp(ଅଚଳ ଶୂନ୍ୟ *a_, ଅଚଳ ଶୂନ୍ୟ *b_)
{
	ଅଚଳ ସଂରଚନା worktree *ଅଚଳ *a = a_;
	ଅଚଳ ସଂରଚନା worktree *ଅଚଳ *b = b_;
	ଫେରନ୍ତୁ fspathcmp((*a)->path, (*b)->path);
}

ସ୍ଥିର ଶୂନ୍ୟ pathsort(ସଂରଚନା worktree **wt)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = 0;
	ସଂରଚନା worktree **p = wt;

	ଯେତେବେଳେ (*p++)
		n++;
	QSORT(wt, n, pathcmp);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା list(ପୂର୍ଣ୍ଣସଂଖ୍ୟା ac, ଅଚଳ ଅକ୍ଷର **av, ଅଚଳ ଅକ୍ଷର *prefix,
		ସଂରଚନା repository *repo UNUSED)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା porcelain = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଧାଡ଼ି_terminator = '\n';

	ସଂରଚନା option options[] = {
		OPT_BOOL(0, "porcelain", &porcelain, N_("machine-readable output")),
		OPT__VERBOSE(&verbose, N_("show extended annotations and reasons, if available")),
		OPT_EXPIRY_DATE(0, "expire", &expire,
				N_("add 'prunable' annotation to missing worktrees older than <time>")),
		OPT_SET_INT('z', ଖାଲି, &ଧାଡ଼ି_terminator,
			    N_("terminate records with a NUL character"), '\0'),
		OPT_END()
	};

	expire = TIME_MAX;
	ac = parse_options(ac, av, prefix, options, git_worktree_list_usage, 0);
	ଯଦି (ac)
		usage_with_options(git_worktree_list_usage, options);
	ଅନ୍ୟଥା ଯଦି (verbose && porcelain)
		die(_("options '%s' and '%s' cannot be used together"), "--verbose", "--porcelain");
	ଅନ୍ୟଥା ଯଦି (!ଧାଡ଼ି_terminator && !porcelain)
		die(_("the option '%s' requires '%s'"), "-z", "--porcelain");
	ଅନ୍ୟଥା {
		ସଂରଚନା worktree **worktrees = get_worktrees(the_repository);
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା path_maxwidth = 0, abbrev = DEFAULT_ABBREV, i;
		ସଂରଚନା worktree_display *display = ଖାଲି;

		/* sort worktrees by path but keep main worktree at top */
		pathsort(worktrees + 1);

		ଯଦି (!porcelain)
			measure_widths(worktrees, &abbrev,
				       &display, &path_maxwidth);

		ପାଇଁ (i = 0; worktrees[i]; i++) {
			ଯଦି (porcelain)
				show_worktree_porcelain(worktrees[i],
							ଧାଡ଼ି_terminator);
			ଅନ୍ୟଥା
				show_worktree(worktrees[i],
					      &display[i], path_maxwidth, abbrev);
		}
		ପାଇଁ (i = 0; display && worktrees[i]; i++)
			ମୁକ୍ତ(display[i].path);
		ମୁକ୍ତ(display);
		free_worktrees(worktrees);
	}
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା lock_worktree(ପୂର୍ଣ୍ଣସଂଖ୍ୟା ac, ଅଚଳ ଅକ୍ଷର **av, ଅଚଳ ଅକ୍ଷର *prefix,
			 ସଂରଚନା repository *repo UNUSED)
{
	ଅଚଳ ଅକ୍ଷର *reason = "", *old_reason;
	ସଂରଚନା option options[] = {
		OPT_STRING(0, "reason", &reason, N_("string"),
			   N_("reason for locking")),
		OPT_END()
	};
	ସଂରଚନା worktree **worktrees, *wt;
	ଅକ୍ଷର *path;

	ac = parse_options(ac, av, prefix, options, git_worktree_lock_usage, 0);
	ଯଦି (ac != 1)
		usage_with_options(git_worktree_lock_usage, options);

	worktrees = get_worktrees(the_repository);
	wt = find_worktree(worktrees, prefix, av[0]);
	ଯଦି (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	ଯଦି (is_main_worktree(wt))
		die(_("The main working tree cannot be locked or unlocked"));

	old_reason = worktree_lock_reason(wt);
	ଯଦି (old_reason) {
		ଯଦି (*old_reason)
			die(_("'%s' is already locked, reason: %s"),
			    av[0], old_reason);
		die(_("'%s' is already locked"), av[0]);
	}

	path = repo_common_path(the_repository, "worktrees/%s/locked", wt->id);
	write_file(path, "%s", reason);

	free_worktrees(worktrees);
	ମୁକ୍ତ(path);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା unlock_worktree(ପୂର୍ଣ୍ଣସଂଖ୍ୟା ac, ଅଚଳ ଅକ୍ଷର **av, ଅଚଳ ଅକ୍ଷର *prefix,
			   ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା option options[] = {
		OPT_END()
	};
	ସଂରଚନା worktree **worktrees, *wt;
	ଅକ୍ଷର *path;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	ac = parse_options(ac, av, prefix, options, git_worktree_unlock_usage, 0);
	ଯଦି (ac != 1)
		usage_with_options(git_worktree_unlock_usage, options);

	worktrees = get_worktrees(the_repository);
	wt = find_worktree(worktrees, prefix, av[0]);
	ଯଦି (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	ଯଦି (is_main_worktree(wt))
		die(_("The main working tree cannot be locked or unlocked"));
	ଯଦି (!worktree_lock_reason(wt))
		die(_("'%s' is not locked"), av[0]);

	path = repo_common_path(the_repository, "worktrees/%s/locked", wt->id);
	ret = unlink_or_warn(path);

	free_worktrees(worktrees);
	ମୁକ୍ତ(path);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ଶୂନ୍ୟ validate_no_submodules(ଅଚଳ ସଂରଚନା worktree *wt)
{
	ସଂରଚନା index_state istate = INDEX_STATE_INIT(the_repository);
	ସଂରଚନା strbuf path = STRBUF_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, found_submodules = 0;
	ଅକ୍ଷର *wt_gitdir;

	wt_gitdir = get_worktree_git_dir(wt);

	ଯଦି (is_directory(worktree_git_path(wt, "modules"))) {
		/*
		 * There could be false positives, e.g. the "modules"
		 * directory exists but is empty. But it's a rare case and
		 * this simpler check is probably good enough for now.
		 */
		found_submodules = 1;
	} ଅନ୍ୟଥା ଯଦି (read_index_from(&istate, worktree_git_path(wt, "index"),
				   wt_gitdir) > 0) {
		ପାଇଁ (i = 0; i < istate.cache_nr; i++) {
			ସଂରଚନା cache_entry *ce = istate.cache[i];
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା err;

			ଯଦି (!S_ISGITLINK(ce->ce_mode))
				ଜାରି;

			strbuf_reset(&path);
			strbuf_addf(&path, "%s/%s", wt->path, ce->name);
			ଯଦି (!is_submodule_populated_gently(path.buf, &err))
				ଜାରି;

			found_submodules = 1;
			ଭାଙ୍ଗ;
		}
	}
	discard_index(&istate);
	strbuf_release(&path);
	ମୁକ୍ତ(wt_gitdir);

	ଯଦି (found_submodules)
		die(_("working trees containing submodules cannot be moved or removed"));
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା move_worktree(ପୂର୍ଣ୍ଣସଂଖ୍ୟା ac, ଅଚଳ ଅକ୍ଷର **av, ଅଚଳ ଅକ୍ଷର *prefix,
			 ସଂରଚନା repository *repo UNUSED)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା force = 0;
	ସଂରଚନା option options[] = {
		OPT__FORCE(&force,
			 N_("force move even if worktree is dirty or locked"),
			 PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "relative-paths", &use_relative_paths,
			 N_("use relative paths for worktrees")),
		OPT_END()
	};
	ସଂରଚନା worktree **worktrees, *wt;
	ସଂରଚନା strbuf dst = STRBUF_INIT;
	ସଂରଚନା strbuf errmsg = STRBUF_INIT;
	ଅଚଳ ଅକ୍ଷର *reason = ଖାଲି;
	ଅକ୍ଷର *path;

	ac = parse_options(ac, av, prefix, options, git_worktree_move_usage,
			   0);
	ଯଦି (ac != 2)
		usage_with_options(git_worktree_move_usage, options);

	path = prefix_filename(prefix, av[1]);
	strbuf_addstr(&dst, path);
	ମୁକ୍ତ(path);

	worktrees = get_worktrees(the_repository);
	wt = find_worktree(worktrees, prefix, av[0]);
	ଯଦି (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	ଯଦି (is_main_worktree(wt))
		die(_("'%s' is a main working tree"), av[0]);
	ଯଦି (is_directory(dst.buf)) {
		ଅଚଳ ଅକ୍ଷର *sep = find_last_dir_sep(wt->path);

		ଯଦି (!sep)
			die(_("could not figure out destination name from '%s'"),
			    wt->path);
		strbuf_trim_trailing_dir_sep(&dst);
		strbuf_addstr(&dst, sep);
	}
	check_candidate_path(dst.buf, force, worktrees, "move");

	validate_no_submodules(wt);

	ଯଦି (force < 2)
		reason = worktree_lock_reason(wt);
	ଯଦି (reason) {
		ଯଦି (*reason)
			die(_("cannot move a locked working tree, lock reason: %s\nuse 'move -f -f' to override or unlock first"),
			    reason);
		die(_("cannot move a locked working tree;\nuse 'move -f -f' to override or unlock first"));
	}
	ଯଦି (validate_worktree(wt, &errmsg, 0))
		die(_("validation failed, cannot move working tree: %s"),
		    errmsg.buf);
	strbuf_release(&errmsg);

	ଯଦି (rename(wt->path, dst.buf) == -1)
		die_errno(_("failed to move '%s' to '%s'"), wt->path, dst.buf);

	update_worktree_location(wt, dst.buf, use_relative_paths);

	strbuf_release(&dst);
	free_worktrees(worktrees);
	ଫେରନ୍ତୁ 0;
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
ସ୍ଥିର ଶୂନ୍ୟ check_clean_worktree(ସଂରଚନା worktree *wt,
				 ଅଚଳ ଅକ୍ଷର *original_path)
{
	ସଂରଚନା child_process cp;
	ଅକ୍ଷର buf[1];
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	/*
	 * Until we sort this out, all submodules are "dirty" and
	 * will abort this function.
	 */
	validate_no_submodules(wt);

	child_process_init(&cp);
	strvec_pushf(&cp.env, "%s=%s/.git",
		     GIT_DIR_ENVIRONMENT, wt->path);
	strvec_pushf(&cp.env, "%s=%s",
		     GIT_WORK_ବୃକ୍ଷ_ENVIRONMENT, wt->path);
	strvec_pushl(&cp.args, "status",
		     "--porcelain", "--ignore-submodules=none",
		     ଖାଲି);
	cp.git_cmd = 1;
	cp.dir = wt->path;
	cp.out = -1;
	ret = start_command(&cp);
	ଯଦି (ret)
		die_errno(_("failed to run 'git status' on '%s'"),
			  original_path);
	ret = xread(cp.out, buf, ଆକାର(buf));
	ଯଦି (ret)
		die(_("'%s' contains modified or untracked files, use --force to delete it"),
		    original_path);
	close(cp.out);
	ret = finish_command(&cp);
	ଯଦି (ret)
		die_errno(_("failed to run 'git status' on '%s', code %d"),
			  original_path, ret);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା delete_git_work_ବୃକ୍ଷ(ସଂରଚନା worktree *wt)
{
	ସଂରଚନା strbuf sb = STRBUF_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;

	strbuf_addstr(&sb, wt->path);
	ଯଦି (remove_dir_recursively(&sb, 0)) {
		error_errno(_("failed to delete '%s'"), sb.buf);
		ret = -1;
	}
	strbuf_release(&sb);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା remove_worktree(ପୂର୍ଣ୍ଣସଂଖ୍ୟା ac, ଅଚଳ ଅକ୍ଷର **av, ଅଚଳ ଅକ୍ଷର *prefix,
			   ସଂରଚନା repository *repo UNUSED)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା force = 0;
	ସଂରଚନା option options[] = {
		OPT__FORCE(&force,
			 N_("force removal even if worktree is dirty or locked"),
			 PARSE_OPT_NOCOMPLETE),
		OPT_END()
	};
	ସଂରଚନା worktree **worktrees, *wt;
	ସଂରଚନା strbuf errmsg = STRBUF_INIT;
	ଅଚଳ ଅକ୍ଷର *reason = ଖାଲି;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;

	ac = parse_options(ac, av, prefix, options, git_worktree_remove_usage, 0);
	ଯଦି (ac != 1)
		usage_with_options(git_worktree_remove_usage, options);

	worktrees = get_worktrees(the_repository);
	wt = find_worktree(worktrees, prefix, av[0]);
	ଯଦି (!wt)
		die(_("'%s' is not a working tree"), av[0]);
	ଯଦି (is_main_worktree(wt))
		die(_("'%s' is a main working tree"), av[0]);
	ଯଦି (force < 2)
		reason = worktree_lock_reason(wt);
	ଯଦି (reason) {
		ଯଦି (*reason)
			die(_("cannot remove a locked working tree, lock reason: %s\nuse 'remove -f -f' to override or unlock first"),
			    reason);
		die(_("cannot remove a locked working tree;\nuse 'remove -f -f' to override or unlock first"));
	}
	ଯଦି (validate_worktree(wt, &errmsg, WT_VALIDATE_WORKTREE_MISSING_OK))
		die(_("validation failed, cannot remove working tree: %s"),
		    errmsg.buf);
	strbuf_release(&errmsg);

	ଯଦି (file_exists(wt->path)) {
		ଯଦି (!force)
			check_clean_worktree(wt, av[0]);

		ret |= delete_git_work_ବୃକ୍ଷ(wt);
	}
	/*
	 * continue on even if ret is non-zero, there's no going back
	 * from here.
	 */
	ret |= delete_git_dir(wt->id);
	delete_worktrees_dir_if_empty();

	free_worktrees(worktrees);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ଶୂନ୍ୟ report_repair(ପୂର୍ଣ୍ଣସଂଖ୍ୟା iserr, ଅଚଳ ଅକ୍ଷର *path, ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ, ଶୂନ୍ୟ *cb_data)
{
	ଯଦି (!iserr) {
		fprintf_ln(stderr, _("repair: %s: %s"), ସନ୍ଦେଶ, path);
	} ଅନ୍ୟଥା {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା *exit_status = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା *)cb_data;
		fprintf_ln(stderr, _("error: %s: %s"), ସନ୍ଦେଶ, path);
		*exit_status = 1;
	}
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା repair(ପୂର୍ଣ୍ଣସଂଖ୍ୟା ac, ଅଚଳ ଅକ୍ଷର **av, ଅଚଳ ଅକ୍ଷର *prefix,
		  ସଂରଚନା repository *repo UNUSED)
{
	ଅଚଳ ଅକ୍ଷର **p;
	ଅଚଳ ଅକ୍ଷର *self[] = { ".", ଖାଲି };
	ସଂରଚନା option options[] = {
		OPT_BOOL(0, "relative-paths", &use_relative_paths,
			 N_("use relative paths for worktrees")),
		OPT_END()
	};
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା rc = 0;

	ac = parse_options(ac, av, prefix, options, git_worktree_repair_usage, 0);
	p = ac > 0 ? av : self;
	ପାଇଁ (; *p; p++)
		repair_worktree_at_path(the_repository, *p, report_repair,
					&rc, use_relative_paths);
	repair_worktrees(the_repository, report_repair, &rc, use_relative_paths);
	ଫେରନ୍ତୁ rc;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_worktree(ପୂର୍ଣ୍ଣସଂଖ୍ୟା ac,
		 ଅଚଳ ଅକ୍ଷର **av,
		 ଅଚଳ ଅକ୍ଷର *prefix,
		 ସଂରଚନା repository *repo)
{
	parse_opt_subcommand_fn *fn = ଖାଲି;
	ସଂରଚନା option options[] = {
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

	repo_config(the_repository, git_worktree_config, ଖାଲି);

	ଯଦି (!prefix)
		prefix = "";

	ac = parse_options(ac, av, prefix, options, git_worktree_usage, 0);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	ଫେରନ୍ତୁ fn(ac, av, prefix, repo);
}
