/*
 * "git add" builtin command
 *
 * Copyright (C) 2006 Linus Torvalds
 */

#شامل "builtin.h"
#شامل "advice.h"
#شامل "config.h"
#شامل "environment.h"
#شامل "lockfile.h"
#شامل "editor.h"
#شامل "dir.h"
#شامل "gettext.h"
#شامل "pathspec.h"
#شامل "object-file.h"
#شامل "odb.h"
#شامل "odb/transaction.h"
#شامل "parse-options.h"
#شامل "path.h"
#شامل "preload-index.h"
#شامل "diff.h"
#شامل "read-cache.h"
#شامل "revision.h"
#شامل "submodule.h"
#شامل "add-interactive.h"
#شامل "merge-ll.h"
#شامل "apply.h"

ساکن ثابت حرف * ثابت builtin_جوڑو_usage[] = {
	N_("git add [<options>] [--] <pathspec>..."),
	تہی
};
ساکن صحیح patch_interactive, جوڑو_interactive, edit_interactive;
ساکن ساخت interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
ساکن صحیح take_worktree_changes;
ساکن صحیح جوڑو_renormalize;
ساکن صحیح جوڑو_resolved;
ساکن صحیح pathspec_file_nul;
ساکن صحیح include_sparse;
ساکن ثابت حرف *pathspec_from_file;

ساکن صحیح chmod_pathspec(ساخت repository *repo,
			  ساخت pathspec *pathspec,
			  حرف flip,
			  صحیح show_only)
{
	صحیح ret = 0;

	برائے (سائز_t i = 0; i < repo->اشاریہ->cache_nr; i++) {
		ساخت cache_اندراج *ce = repo->اشاریہ->cache[i];
		صحیح err;

		اگر (!include_sparse &&
		    (ce_skip_worktree(ce) ||
		     !راستہ_اندر_sparse_checkout(ce->name, repo->اشاریہ)))
			جاری;

		اگر (pathspec && !ce_راستہ_مطابقت(repo->اشاریہ, ce, pathspec, تہی))
			جاری;

		اگر (!show_only)
			err = chmod_اشاریہ_اندراج(repo->اشاریہ, ce, flip);
		ورنہ
			err = S_ISREG(ce->ce_منوال) ? 0 : -1;

		اگر (err < 0)
			ret = error(_("cannot chmod %cx '%s'"), flip, ce->name);
	}

	واپس ret;
}

ساکن صحیح renormalize_tracked_files(ساخت repository *repo,
				     ثابت ساخت pathspec *pathspec,
				     صحیح flags)
{
	صحیح retval = 0;

	برائے (سائز_t i = 0; i < repo->اشاریہ->cache_nr; i++) {
		ساخت cache_اندراج *ce = repo->اشاریہ->cache[i];

		اگر (!include_sparse &&
		    (ce_skip_worktree(ce) ||
		     !راستہ_اندر_sparse_checkout(ce->name, repo->اشاریہ)))
			جاری;
		اگر (ce_stage(ce))
			جاری; /* do not touch unmerged paths */
		اگر (!S_ISREG(ce->ce_منوال) && !S_ISLNK(ce->ce_منوال))
			جاری; /* do not touch non blobs */
		اگر (pathspec && !ce_راستہ_مطابقت(repo->اشاریہ, ce, pathspec, تہی))
			جاری;
		retval |= جوڑو_file_مقصد_اشاریہ(repo->اشاریہ, ce->name,
					    flags | جوڑو_CACHE_RENORMALIZE);
	}

	واپس retval;
}

ساکن حرف *prune_directory(ساخت repository *repo,
			     ساخت dir_struct *dir,
			     ساخت pathspec *pathspec,
			     صحیح سابقہ)
{
	حرف *نظر_آیا;
	صحیح i;
	ساخت dir_اندراج **src, **dst;

	نظر_آیا = xcalloc(pathspec->nr, 1);

	src = dst = dir->entries;
	i = dir->nr;
	جب (--i >= 0) {
		ساخت dir_اندراج *اندراج_2 = *src++;
		اگر (dir_راستہ_مطابقت(repo->اشاریہ, اندراج_2, pathspec, سابقہ, نظر_آیا))
			*dst++ = اندراج_2;
	}
	dir->nr = dst - dir->entries;
	جوڑو_pathspec_matches_against_اشاریہ(pathspec, repo->اشاریہ, نظر_آیا,
					   PS_IGNORE_SKIP_WORKTREE);
	واپس نظر_آیا;
}

ساکن صحیح refresh(ساخت repository *repo, صحیح verbose, ثابت ساخت pathspec *pathspec)
{
	حرف *نظر_آیا;
	صحیح i, ret = 0;
	حرف *skip_worktree_نظر_آیا = تہی;
	ساخت string_list only_مطابقت_skip_worktree = STRING_LIST_INIT_NODUP;
	غیرعلامتی صحیح flags = REFRESH_IGNORE_SKIP_WORKTREE |
		    (verbose ? REFRESH_اندر_PORCELAIN : REFRESH_QUIET);

	نظر_آیا = xcalloc(pathspec->nr, 1);
	refresh_اشاریہ(repo->اشاریہ, flags, pathspec, نظر_آیا,
		      _("Unstaged changes after refreshing the index:"));
	برائے (i = 0; i < pathspec->nr; i++) {
		اگر (!نظر_آیا[i]) {
			ثابت حرف *راستہ = pathspec->اشیاء[i].original;

			اگر (matches_skip_worktree(pathspec, i, &skip_worktree_نظر_آیا) ||
			    !راستہ_اندر_sparse_checkout(راستہ, repo->اشاریہ)) {
				string_list_append(&only_مطابقت_skip_worktree,
						   pathspec->اشیاء[i].original);
			} ورنہ {
				die(_("pathspec '%s' did not match any files"),
				    pathspec->اشیاء[i].original);
			}
		}
	}

	اگر (only_مطابقت_skip_worktree.nr) {
		advise_on_updating_sparse_paths(&only_مطابقت_skip_worktree);
		ret = 1;
	}

	آزاد(نظر_آیا);
	آزاد(skip_worktree_نظر_آیا);
	string_list_clear(&only_مطابقت_skip_worktree, 0);
	واپس ret;
}

صحیح interactive_جوڑو(ساخت repository *repo,
		    ثابت حرف **argv,
		    ثابت حرف *سابقہ,
		    صحیح patch, ساخت interactive_options *interactive_opts)
{
	ساخت pathspec pathspec;
	صحیح ret;

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_بھرا |
		       PATHSPEC_SYMLINK_LEADING_راستہ |
		       PATHSPEC_سابقہ_ORIGIN,
		       سابقہ, argv);

	اگر (patch)
		ret = !!تسلسل_جوڑو_p(repo, جوڑو_P_جوڑو, interactive_opts, تہی, &pathspec, 0);
	ورنہ
		ret = !!تسلسل_جوڑو_i(repo, &pathspec, interactive_opts);

	clear_pathspec(&pathspec);
	واپس ret;
}

ساکن صحیح edit_patch(ساخت repository *repo,
		      صحیح argc,
		      ثابت حرف **argv,
		      ثابت حرف *سابقہ)
{
	حرف *file = repo_git_راستہ(repo, "ADD_EDIT.patch");
	ساخت apply_state state;
	ثابت حرف *apply_argv[2];
	ساخت rev_info rev;
	صحیح برآمد;
	ساخت stat st;

	repo_config(repo, git_diff_basic_config, تہی);

	اگر (repo_read_اشاریہ(repo) < 0)
		die(_("could not read the index"));

	repo_init_revisions(repo, &rev, سابقہ);
	rev.diffopt.context = 7;

	argc = تیاری_revisions(argc, argv, &rev, تہی);
	rev.diffopt.output_format = DIFF_FORMAT_PATCH;
	rev.diffopt.use_color = GIT_COLOR_NEVER;
	rev.diffopt.flags.ignore_dirty_submodules = 1;
	برآمد = xopen(file, O_CREAT | O_WRONLY | O_TRUNC, 0666);
	rev.diffopt.file = xfdopen(برآمد, "w");
	rev.diffopt.close_file = 1;
	تسلسل_diff_files(&rev, 0);

	اگر (launch_editor(file, تہی, تہی))
		die(_("editing patch failed"));

	اگر (stat(file, &st))
		die_errno(_("could not stat '%s'"), file);
	اگر (!st.st_سائز)
		die(_("empty patch. aborted"));

	apply_argv[0] = file;
	apply_argv[1] = تہی;
	اگر (init_apply_state(&state, repo, تہی))
		die(_("could not initialize apply state"));
	state.cached = 1;
	اگر (check_apply_state(&state, 0))
		die(_("could not check apply state"));
	اگر (apply_all_patches(&state, 1, apply_argv, APPLY_OPT_RECOUNT))
		die(_("could not apply '%s'"), file);
	clear_apply_state(&state);

	unlink(file);
	آزاد(file);
	release_revisions(&rev);
	واپس 0;
}

ساکن ثابت حرف ignore_error[] =
N_("The following paths are ignored by one of your .gitignore files:\n");

ساکن صحیح verbose, show_only, ignored_too, refresh_only;
ساکن صحیح ignore_جوڑو_errors, intent_مقصد_جوڑو, ignore_missing;
ساکن صحیح warn_on_embedded_repo = 1;

#تعریف_کرو ADDREMOVE_DEFAULT 1
ساکن صحیح addremove = ADDREMOVE_DEFAULT;
ساکن صحیح addremove_explicit = -1; /* unspecified */

ساکن حرف *chmod_arg;

ساکن صحیح ignore_removal_cb(ثابت ساخت option *opt, ثابت حرف *arg, صحیح unset)
{
	BUG_ON_OPT_ARG(arg);

	/* if we are told to ignore, we are not adding removals */
	*(صحیح *)opt->قدر = !unset ? 0 : 1;
	واپس 0;
}

ساکن ساخت option builtin_جوڑو_options[] = {
	OPT_DRY_تسلسل(&show_only, N_("dry run")),
	OPT__VERBOSE(&verbose, N_("be verbose")),
	OPT_گروہ(""),
	OPT_BOOL('i', "interactive", &جوڑو_interactive, N_("interactive picking")),
	OPT_BOOL('p', "patch", &patch_interactive, N_("select hunks interactively")),
	OPT_BOOL(0, "auto-advance", &interactive_opts.auto_advance,
		 N_("auto advance to the next file when selecting hunks interactively")),
	OPT_DIFF_UNIFIED(&interactive_opts.context),
	OPT_DIFF_INTERHUNK_CONTEXT(&interactive_opts.interhunkcontext),
	OPT_BOOL('e', "edit", &edit_interactive, N_("edit current diff and apply")),
	OPT__FORCE(&ignored_too, N_("allow adding otherwise ignored files"), 0),
	OPT_BOOL('u', "update", &take_worktree_changes, N_("update tracked files")),
	OPT_BOOL(0, "renormalize", &جوڑو_renormalize, N_("renormalize EOL of tracked files (implies -u)")),
	OPT_BOOL(0, "resolved", &جوڑو_resolved, N_("add conflict-resolved tracked files")),
	OPT_BOOL('N', "intent-to-add", &intent_مقصد_جوڑو, N_("record only the fact that the path will be added later")),
	OPT_BOOL('A', "all", &addremove_explicit, N_("add changes from all tracked and untracked files")),
	OPT_CALLBACK_F(0, "ignore-removal", &addremove_explicit,
	  تہی /* takes no arguments */,
	  N_("ignore paths removed in the working tree (same as --no-all)"),
	  PARSE_OPT_NOARG, ignore_removal_cb),
	OPT_BOOL( 0 , "refresh", &refresh_only, N_("don't add, only refresh the index")),
	OPT_BOOL( 0 , "ignore-errors", &ignore_جوڑو_errors, N_("just skip files which cannot be added because of errors")),
	OPT_BOOL( 0 , "ignore-missing", &ignore_missing, N_("check if - even missing - files are ignored in dry run")),
	OPT_BOOL(0, "sparse", &include_sparse, N_("allow updating entries outside of the sparse-checkout cone")),
	OPT_STRING(0, "chmod", &chmod_arg, "(+|-)x",
		   N_("override the executable bit of the listed files")),
	OPT_HIDDEN_BOOL(0, "warn-embedded-repo", &warn_on_embedded_repo,
			N_("warn when adding an embedded repository")),
	OPT_PATHSPEC_FROM_FILE(&pathspec_from_file),
	OPT_PATHSPEC_FILE_NUL(&pathspec_file_nul),
	OPT_انتہا(),
};

ساکن صحیح جوڑو_config(ثابت حرف *var, ثابت حرف *قدر,
		      ثابت ساخت config_context *ctx, خالی *cb)
{
	اگر (!strcmp(var, "add.ignoreerrors") ||
	    !strcmp(var, "add.ignore-errors")) {
		ignore_جوڑو_errors = git_config_bool(var, قدر);
		واپس 0;
	}

	اگر (git_color_config(var, قدر, cb) < 0)
		واپس -1;

	واپس git_default_config(var, قدر, ctx, cb);
}

ساکن ثابت حرف embedded_advice[] = N_(
"You've added another git repository inside your current repository.\n"
"Clones of the outer repository will not contain the contents of\n"
"the embedded repository and will not know how to obtain it.\n"
"If you meant to add a submodule, use:\n"
"\n"
"	git submodule add <url> %s\n"
"\n"
"If you added this path by mistake, you can remove it from the\n"
"index with:\n"
"\n"
"	git rm --cached %s\n"
"\n"
"See \"git help submodule\" for more information."
);

ساکن خالی check_embedded_repo(ثابت حرف *راستہ)
{
	ساخت strbuf name = STRBUF_INIT;
	ساکن صحیح adviced_on_embedded_repo = 0;

	اگر (!warn_on_embedded_repo)
		واپس;
	اگر (!سرے_with(راستہ, "/"))
		واپس;

	/* Drop trailing slash for aesthetics */
	strbuf_addstr(&name, راستہ);
	strbuf_strip_suffix(&name, "/");

	warning(_("adding embedded git repository: %s"), name.buf);
	اگر (!adviced_on_embedded_repo) {
		advise_if_enabled(ADVICE_جوڑو_EMBEDDED_REPO,
				  embedded_advice, name.buf, name.buf);
		adviced_on_embedded_repo = 1;
	}

	strbuf_release(&name);
}

ساکن صحیح جوڑو_files(ساخت repository *repo, ساخت dir_struct *dir, صحیح flags)
{
	صحیح i, exit_حالت = 0;
	ساخت string_list matched_sparse_paths = STRING_LIST_INIT_NODUP;

	اگر (dir->ignored_nr) {
		fprintf(stderr, _(ignore_error));
		برائے (i = 0; i < dir->ignored_nr; i++)
			fprintf(stderr, "%s\n", dir->ignored[i]->name);
		advise_if_enabled(ADVICE_جوڑو_IGNORED_FILE,
				  _("Use -f if you really want to add them."));
		exit_حالت = 1;
	}

	برائے (i = 0; i < dir->nr; i++) {
		اگر (!include_sparse &&
		    !راستہ_اندر_sparse_checkout(dir->entries[i]->name, repo->اشاریہ)) {
			string_list_append(&matched_sparse_paths,
					   dir->entries[i]->name);
			جاری;
		}
		اگر (جوڑو_file_مقصد_اشاریہ(repo->اشاریہ, dir->entries[i]->name, flags)) {
			اگر (!ignore_جوڑو_errors)
				die(_("adding files failed"));
			exit_حالت = 1;
		} ورنہ {
			check_embedded_repo(dir->entries[i]->name);
		}
	}

	اگر (matched_sparse_paths.nr) {
		advise_on_updating_sparse_paths(&matched_sparse_paths);
		exit_حالت = 1;
	}

	string_list_clear(&matched_sparse_paths, 0);

	واپس exit_حالت;
}

ساکن صحیح failed_مقصد_جوڑو(صحیح flags, ثابت حرف *راستہ)
{
	اگر (!(flags & جوڑو_CACHE_IGNORE_ERRORS))
		die(_("updating file '%s' failed"), راستہ);
	واپس 1;
}

ساکن صحیح جوڑو_resolved_files(ساخت repository *repo,
			      ثابت ساخت pathspec *pathspec,
			      صحیح flags)
{
	ساخت اشاریہ_state *istate = repo->اشاریہ;
	ساخت string_list unmerged_paths = STRING_LIST_INIT_DUP;
	ساخت string_list unresolved_paths = STRING_LIST_INIT_DUP;
	صحیح exit_حالت = 0;
	سائز_t i;

	برائے (i = 0; i < istate->cache_nr; i++) {
		ساخت cache_اندراج *ce = istate->cache[i];
		اگر (!ce_stage(ce))
			جاری;
		اگر (pathspec->nr && !ce_راستہ_مطابقت(istate, ce, pathspec, تہی))
			جاری;
		اگر (!unmerged_paths.nr ||
		    strcmp(unmerged_paths.اشیاء[unmerged_paths.nr - 1].string, ce->name))
			string_list_append(&unmerged_paths, ce->name);
	}

	اگر (!unmerged_paths.nr) {
		string_list_clear(&unmerged_paths, 0);
		واپس 0;
	}

	برائے (i = 0; i < unmerged_paths.nr; i++) {
		ثابت حرف *راستہ = unmerged_paths.اشیاء[i].string;
		ساخت stat st;

		اگر (!lstat(راستہ, &st) && S_ISREG(st.st_منوال)) {
			اگر (has_conflict_markers(istate, راستہ))
				string_list_append(&unresolved_paths, راستہ);
		}
	}

	اگر (unresolved_paths.nr) {
		ساخت strbuf sb = STRBUF_INIT;
		برائے (i = 0; i < unresolved_paths.nr; i++)
			strbuf_addf(&sb, "\t%s\n", unresolved_paths.اشیاء[i].string);
		die(_("the following paths still have conflict markers:\n%s"), sb.buf);
	}

	برائے (i = 0; i < unmerged_paths.nr; i++) {
		ثابت حرف *راستہ = unmerged_paths.اشیاء[i].string;
		ساخت stat st;

		اگر (lstat(راستہ, &st)) {
			اگر (errno != ENOENT)
				die_errno(_("cannot lstat: '%s'"), راستہ);
			اگر (remove_file_from_اشاریہ_with_flags(istate, راستہ, flags))
				exit_حالت = failed_مقصد_جوڑو(flags, راستہ);
		} ورنہ {
			اگر (جوڑو_file_مقصد_اشاریہ(istate, راستہ, flags))
				exit_حالت = failed_مقصد_جوڑو(flags, راستہ);
		}
	}

	string_list_clear(&unmerged_paths, 0);
	string_list_clear(&unresolved_paths, 0);
	واپس exit_حالت;
}

صحیح cmd_جوڑو(صحیح argc,
	    ثابت حرف **argv,
	    ثابت حرف *سابقہ,
	    ساخت repository *repo)
{
	صحیح exit_حالت = 0;
	ساخت pathspec pathspec;
	ساخت dir_struct dir = DIR_INIT;
	صحیح flags;
	صحیح جوڑو_new_files;
	صحیح require_pathspec;
	حرف *نظر_آیا = تہی;
	حرف *ps_matched = تہی;
	ساخت تالا_file تالا_file = تالا_INIT;
	ساخت odb_transaction *transaction = تہی;

	repo_config(repo, جوڑو_config, تہی);

	argc = parse_options(argc, argv, سابقہ, builtin_جوڑو_options,
			  builtin_جوڑو_usage, PARSE_OPT_KEEP_ARGV0);

	prepare_repo_settings(repo);
	repo->settings.command_requires_بھرا_اشاریہ = 0;

	اگر (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	اگر (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	اگر (patch_interactive)
		جوڑو_interactive = 1;
	اگر (جوڑو_interactive) {
		اگر (show_only)
			die(_("options '%s' and '%s' cannot be used together"), "--dry-run", "--interactive/--patch");
		اگر (pathspec_from_file)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--interactive/--patch");
		باہر_نکلو(interactive_جوڑو(repo, argv + 1, سابقہ, patch_interactive, &interactive_opts));
	} ورنہ {
		اگر (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--interactive/--patch");
		اگر (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--interactive/--patch");
		اگر (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--interactive/--patch");
	}

	اگر (edit_interactive) {
		اگر (pathspec_from_file)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--edit");
		واپس(edit_patch(repo, argc, argv, سابقہ));
	}
	argc--;
	argv++;

	اگر (0 <= addremove_explicit)
		addremove = addremove_explicit;
	ورنہ اگر (take_worktree_changes && ADDREMOVE_DEFAULT)
		addremove = 0; /* "-u" was given but not "-A" */

	die_for_incompatible_opt3(take_worktree_changes, "-u/--update",
				  0 < addremove_explicit, "-A/--all",
				  جوڑو_resolved, "--resolved");

	اگر (!show_only && ignore_missing)
		die(_("the option '%s' requires '%s'"), "--ignore-missing", "--dry-run");

	اگر (chmod_arg && ((chmod_arg[0] != '-' && chmod_arg[0] != '+') ||
			  chmod_arg[1] != 'x' || chmod_arg[2]))
		die(_("--chmod param '%s' must be either -x or +x"), chmod_arg);

	جوڑو_new_files = !take_worktree_changes && !refresh_only &&
			!جوڑو_renormalize && !جوڑو_resolved;
	require_pathspec = !(take_worktree_changes ||
			     (0 < addremove_explicit) ||
			     جوڑو_resolved);

	repo_hold_locked_اشاریہ(repo, &تالا_file, تالا_DIE_ON_ERROR);

	/*
	 * Check the "pathspec '%s' did not match any files" block
	 * below before enabling new magic.
	 */
	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_بھرا |
		       PATHSPEC_SYMLINK_LEADING_راستہ,
		       سابقہ, argv);

	اگر (pathspec_from_file) {
		اگر (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_بھرا |
				    PATHSPEC_SYMLINK_LEADING_راستہ,
				    سابقہ, pathspec_from_file, pathspec_file_nul);
	} ورنہ اگر (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	اگر (require_pathspec && pathspec.nr == 0) {
		fprintf(stderr, _("Nothing specified, nothing added.\n"));
		advise_if_enabled(ADVICE_جوڑو_خالی_خانے_PATHSPEC,
				  _("Maybe you wanted to say 'git add .'?"));
		واپس 0;
	}

	اگر (!take_worktree_changes && !جوڑو_resolved &&
	    addremove_explicit < 0 && pathspec.nr)
		/* Turn "git add pathspec..." to "git add -A pathspec..." */
		addremove = 1;

	flags = ((verbose ? جوڑو_CACHE_VERBOSE : 0) |
		 (show_only ? جوڑو_CACHE_PRETEND : 0) |
		 (intent_مقصد_جوڑو ? جوڑو_CACHE_INTENT : 0) |
		 (ignore_جوڑو_errors ? جوڑو_CACHE_IGNORE_ERRORS : 0) |
		 (!(addremove || take_worktree_changes)
		  ? جوڑو_CACHE_IGNORE_REMOVAL : 0));

	اگر (repo_read_اشاریہ_preload(repo, &pathspec, 0) < 0)
		die(_("index file corrupt"));

	die_اندر_unpopulated_submodule(repo->اشاریہ, سابقہ);
	die_راستہ_inside_submodule(repo->اشاریہ, &pathspec);

	اگر (جوڑو_new_files) {
		صحیح baselen;

		/* Set up the default git porcelain excludes */
		اگر (!ignored_too) {
			dir.flags |= DIR_COLLECT_IGNORED;
			تیاری_standard_excludes(&dir);
		}

		/* This picks up the paths that are not tracked */
		baselen = fill_directory(&dir, repo->اشاریہ, &pathspec);
		اگر (pathspec.nr)
			نظر_آیا = prune_directory(repo, &dir, &pathspec, baselen);
	}

	اگر (refresh_only) {
		exit_حالت |= refresh(repo, verbose, &pathspec);
		جاؤ اختتام;
	}

	اگر (pathspec.nr) {
		صحیح i;
		حرف *skip_worktree_نظر_آیا = تہی;
		ساخت string_list only_مطابقت_skip_worktree = STRING_LIST_INIT_NODUP;

		اگر (!نظر_آیا)
			نظر_آیا = ڈھونڈو_pathspecs_matching_against_اشاریہ(&pathspec,
					repo->اشاریہ, PS_IGNORE_SKIP_WORKTREE);

		/*
		 * file_exists() assumes exact match
		 */
		GUARD_PATHSPEC(&pathspec,
			       PATHSPEC_FROMTOP |
			       PATHSPEC_LITERAL |
			       PATHSPEC_GLOB |
			       PATHSPEC_ICASE |
			       PATHSPEC_EXCLUDE |
			       PATHSPEC_ATTR);

		برائے (i = 0; i < pathspec.nr; i++) {
			ثابت حرف *راستہ = pathspec.اشیاء[i].مطابقت;

			اگر (pathspec.اشیاء[i].magic & PATHSPEC_EXCLUDE)
				جاری;
			اگر (نظر_آیا[i])
				جاری;

			اگر (!include_sparse &&
			    matches_skip_worktree(&pathspec, i, &skip_worktree_نظر_آیا)) {
				string_list_append(&only_مطابقت_skip_worktree,
						   pathspec.اشیاء[i].original);
				جاری;
			}

			/* Don't complain at 'git add .' on empty repo */
			اگر (!راستہ[0])
				جاری;

			اگر ((pathspec.اشیاء[i].magic & (PATHSPEC_GLOB | PATHSPEC_ICASE)) ||
			    !file_exists(راستہ)) {
				اگر (ignore_missing) {
					صحیح dtype = DT_UNKNOWN;
					اگر (is_excluded(&dir, repo->اشاریہ, راستہ, &dtype))
						dir_جوڑو_ignored(&dir, repo->اشاریہ,
								راستہ, pathspec.اشیاء[i].len);
				} ورنہ
					die(_("pathspec '%s' did not match any files"),
					    pathspec.اشیاء[i].original);
			}
		}


		اگر (only_مطابقت_skip_worktree.nr) {
			advise_on_updating_sparse_paths(&only_مطابقت_skip_worktree);
			exit_حالت = 1;
		}

		آزاد(نظر_آیا);
		آزاد(skip_worktree_نظر_آیا);
		string_list_clear(&only_مطابقت_skip_worktree, 0);
	}

	odb_transaction_begin_or_die(repo->objects, &transaction, 0);

	ps_matched = xcalloc(pathspec.nr, 1);
	اگر (جوڑو_resolved)
		exit_حالت |= جوڑو_resolved_files(repo, &pathspec, flags);
	ورنہ اگر (جوڑو_renormalize)
		exit_حالت |= renormalize_tracked_files(repo, &pathspec, flags);
	ورنہ
		exit_حالت |= جوڑو_files_مقصد_cache(repo, سابقہ,
						  &pathspec, ps_matched,
						  include_sparse, flags, ignored_too);

	اگر (take_worktree_changes && !جوڑو_renormalize && !ignore_جوڑو_errors &&
	    report_راستہ_error(ps_matched, &pathspec))
		باہر_نکلو(128);

	اگر (جوڑو_new_files)
		exit_حالت |= جوڑو_files(repo, &dir, flags);

	اگر (chmod_arg && pathspec.nr)
		exit_حالت |= chmod_pathspec(repo, &pathspec, chmod_arg[0], show_only);
	odb_transaction_commit_and_finalize_or_die(transaction);

اختتام:
	اگر (write_locked_اشاریہ(repo->اشاریہ, &تالا_file,
			       COMMIT_تالا | SKIP_IF_UNCHANGED))
		die(_("unable to write new index file"));

	آزاد(ps_matched);
	dir_clear(&dir);
	clear_pathspec(&pathspec);
	واپس exit_حالت;
}
