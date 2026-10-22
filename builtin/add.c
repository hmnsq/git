/*
 * "git add" builtin command
 *
 * Copyright (C) 2006 Linus Torvalds
 */

#ضمن "builtin.h"
#ضمن "advice.h"
#ضمن "config.h"
#ضمن "environment.h"
#ضمن "lockfile.h"
#ضمن "editor.h"
#ضمن "dir.h"
#ضمن "gettext.h"
#ضمن "pathspec.h"
#ضمن "object-file.h"
#ضمن "odb.h"
#ضمن "odb/transaction.h"
#ضمن "parse-options.h"
#ضمن "path.h"
#ضمن "preload-index.h"
#ضمن "diff.h"
#ضمن "read-cache.h"
#ضمن "revision.h"
#ضمن "submodule.h"
#ضمن "add-interactive.h"
#ضمن "merge-ll.h"
#ضمن "apply.h"

ساكن ثابت حرف * ثابت builtin_جوڙيو_usage[] = {
	N_("git add [<options>] [--] <pathspec>..."),
	عدم
};
ساكن صحيح patch_interactive, جوڙيو_interactive, edit_interactive;
ساكن بنية interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
ساكن صحيح take_worktree_changes;
ساكن صحيح جوڙيو_renormalize;
ساكن صحيح جوڙيو_resolved;
ساكن صحيح pathspec_file_nul;
ساكن صحيح include_sparse;
ساكن ثابت حرف *pathspec_from_file;

ساكن صحيح chmod_pathspec(بنية repository *repo,
			  بنية pathspec *pathspec,
			  حرف flip,
			  صحيح show_only)
{
	صحيح ret = 0;

	لكل (سائيز_t i = 0; i < repo->اشاريو->cache_nr; i++) {
		بنية cache_داخلا *ce = repo->اشاريو->cache[i];
		صحيح err;

		إذا (!include_sparse &&
		    (ce_skip_worktree(ce) ||
		     !رستو_اندريون_sparse_checkout(ce->name, repo->اشاريو)))
			استمر;

		إذا (pathspec && !ce_رستو_ميلاپ(repo->اشاريو, ce, pathspec, عدم))
			استمر;

		إذا (!show_only)
			err = chmod_اشاريو_داخلا(repo->اشاريو, ce, flip);
		وإلا
			err = S_ISREG(ce->ce_گهڻائي) ? 0 : -1;

		إذا (err < 0)
			ret = error(_("cannot chmod %cx '%s'"), flip, ce->name);
	}

	أرجع ret;
}

ساكن صحيح renormalize_tracked_files(بنية repository *repo,
				     ثابت بنية pathspec *pathspec,
				     صحيح flags)
{
	صحيح retval = 0;

	لكل (سائيز_t i = 0; i < repo->اشاريو->cache_nr; i++) {
		بنية cache_داخلا *ce = repo->اشاريو->cache[i];

		إذا (!include_sparse &&
		    (ce_skip_worktree(ce) ||
		     !رستو_اندريون_sparse_checkout(ce->name, repo->اشاريو)))
			استمر;
		إذا (ce_stage(ce))
			استمر; /* do not touch unmerged paths */
		إذا (!S_ISREG(ce->ce_گهڻائي) && !S_ISLNK(ce->ce_گهڻائي))
			استمر; /* do not touch non blobs */
		إذا (pathspec && !ce_رستو_ميلاپ(repo->اشاريو, ce, pathspec, عدم))
			استمر;
		retval |= جوڙيو_file_منزل_اشاريو(repo->اشاريو, ce->name,
					    flags | جوڙيو_CACHE_RENORMALIZE);
	}

	أرجع retval;
}

ساكن حرف *prune_directory(بنية repository *repo,
			     بنية dir_struct *dir,
			     بنية pathspec *pathspec,
			     صحيح اڳياڙي)
{
	حرف *ڏٺل;
	صحيح i;
	بنية dir_داخلا **src, **dst;

	ڏٺل = xcalloc(pathspec->nr, 1);

	src = dst = dir->entries;
	i = dir->nr;
	طالما (--i >= 0) {
		بنية dir_داخلا *داخلا = *src++;
		إذا (dir_رستو_ميلاپ(repo->اشاريو, داخلا, pathspec, اڳياڙي, ڏٺل))
			*dst++ = داخلا;
	}
	dir->nr = dst - dir->entries;
	جوڙيو_pathspec_matches_against_اشاريو(pathspec, repo->اشاريو, ڏٺل,
					   PS_IGNORE_SKIP_WORKTREE);
	أرجع ڏٺل;
}

ساكن صحيح refresh(بنية repository *repo, صحيح verbose, ثابت بنية pathspec *pathspec)
{
	حرف *ڏٺل;
	صحيح i, ret = 0;
	حرف *skip_worktree_ڏٺل = عدم;
	بنية string_list only_ميلاپ_skip_worktree = STRING_LIST_INIT_NODUP;
	بنانشان صحيح flags = REFRESH_IGNORE_SKIP_WORKTREE |
		    (verbose ? REFRESH_اندريون_PORCELAIN : REFRESH_QUIET);

	ڏٺل = xcalloc(pathspec->nr, 1);
	refresh_اشاريو(repo->اشاريو, flags, pathspec, ڏٺل,
		      _("Unstaged changes after refreshing the index:"));
	لكل (i = 0; i < pathspec->nr; i++) {
		إذا (!ڏٺل[i]) {
			ثابت حرف *رستو = pathspec->شيون[i].original;

			إذا (matches_skip_worktree(pathspec, i, &skip_worktree_ڏٺل) ||
			    !رستو_اندريون_sparse_checkout(رستو, repo->اشاريو)) {
				string_list_append(&only_ميلاپ_skip_worktree,
						   pathspec->شيون[i].original);
			} وإلا {
				die(_("pathspec '%s' did not match any files"),
				    pathspec->شيون[i].original);
			}
		}
	}

	إذا (only_ميلاپ_skip_worktree.nr) {
		advise_on_updating_sparse_paths(&only_ميلاپ_skip_worktree);
		ret = 1;
	}

	حرر(ڏٺل);
	حرر(skip_worktree_ڏٺل);
	string_list_clear(&only_ميلاپ_skip_worktree, 0);
	أرجع ret;
}

صحيح interactive_جوڙيو(بنية repository *repo,
		    ثابت حرف **argv,
		    ثابت حرف *اڳياڙي,
		    صحيح patch, بنية interactive_options *interactive_opts)
{
	بنية pathspec pathspec;
	صحيح ret;

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_ڀريل |
		       PATHSPEC_SYMLINK_LEADING_رستو |
		       PATHSPEC_اڳياڙي_ORIGIN,
		       اڳياڙي, argv);

	إذا (patch)
		ret = !!لاڳيتو_جوڙيو_p(repo, جوڙيو_P_جوڙيو, interactive_opts, عدم, &pathspec, 0);
	وإلا
		ret = !!لاڳيتو_جوڙيو_i(repo, &pathspec, interactive_opts);

	clear_pathspec(&pathspec);
	أرجع ret;
}

ساكن صحيح edit_patch(بنية repository *repo,
		      صحيح argc,
		      ثابت حرف **argv,
		      ثابت حرف *اڳياڙي)
{
	حرف *file = repo_git_رستو(repo, "ADD_EDIT.patch");
	بنية apply_state state;
	ثابت حرف *apply_argv[2];
	بنية rev_info rev;
	صحيح ٻاهريون;
	بنية stat st;

	repo_config(repo, git_diff_basic_config, عدم);

	إذا (repo_read_اشاريو(repo) < 0)
		die(_("could not read the index"));

	repo_init_revisions(repo, &rev, اڳياڙي);
	rev.diffopt.context = 7;

	argc = تياري_revisions(argc, argv, &rev, عدم);
	rev.diffopt.output_format = DIFF_FORMAT_PATCH;
	rev.diffopt.use_color = GIT_COLOR_NEVER;
	rev.diffopt.flags.ignore_dirty_submodules = 1;
	ٻاهريون = xopen(file, O_CREAT | O_WRONLY | O_TRUNC, 0666);
	rev.diffopt.file = xfdopen(ٻاهريون, "w");
	rev.diffopt.close_file = 1;
	لاڳيتو_diff_files(&rev, 0);

	إذا (launch_editor(file, عدم, عدم))
		die(_("editing patch failed"));

	إذا (stat(file, &st))
		die_errno(_("could not stat '%s'"), file);
	إذا (!st.st_سائيز)
		die(_("empty patch. aborted"));

	apply_argv[0] = file;
	apply_argv[1] = عدم;
	إذا (init_apply_state(&state, repo, عدم))
		die(_("could not initialize apply state"));
	state.cached = 1;
	إذا (check_apply_state(&state, 0))
		die(_("could not check apply state"));
	إذا (apply_all_patches(&state, 1, apply_argv, APPLY_OPT_RECOUNT))
		die(_("could not apply '%s'"), file);
	clear_apply_state(&state);

	unlink(file);
	حرر(file);
	release_revisions(&rev);
	أرجع 0;
}

ساكن ثابت حرف ignore_error[] =
N_("The following paths are ignored by one of your .gitignore files:\n");

ساكن صحيح verbose, show_only, ignored_too, refresh_only;
ساكن صحيح ignore_جوڙيو_errors, intent_منزل_جوڙيو, ignore_missing;
ساكن صحيح warn_on_embedded_repo = 1;

#وصف_ڪريو ADDREMOVE_DEFAULT 1
ساكن صحيح addremove = ADDREMOVE_DEFAULT;
ساكن صحيح addremove_explicit = -1; /* unspecified */

ساكن حرف *chmod_arg;

ساكن صحيح ignore_removal_cb(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_ARG(arg);

	/* if we are told to ignore, we are not adding removals */
	*(صحيح *)opt->قدر = !unset ? 0 : 1;
	أرجع 0;
}

ساكن بنية option builtin_جوڙيو_options[] = {
	OPT_DRY_لاڳيتو(&show_only, N_("dry run")),
	OPT__VERBOSE(&verbose, N_("be verbose")),
	OPT_گروهه(""),
	OPT_BOOL('i', "interactive", &جوڙيو_interactive, N_("interactive picking")),
	OPT_BOOL('p', "patch", &patch_interactive, N_("select hunks interactively")),
	OPT_BOOL(0, "auto-advance", &interactive_opts.auto_advance,
		 N_("auto advance to the next file when selecting hunks interactively")),
	OPT_DIFF_UNIFIED(&interactive_opts.context),
	OPT_DIFF_INTERHUNK_CONTEXT(&interactive_opts.interhunkcontext),
	OPT_BOOL('e', "edit", &edit_interactive, N_("edit current diff and apply")),
	OPT__FORCE(&ignored_too, N_("allow adding otherwise ignored files"), 0),
	OPT_BOOL('u', "update", &take_worktree_changes, N_("update tracked files")),
	OPT_BOOL(0, "renormalize", &جوڙيو_renormalize, N_("renormalize EOL of tracked files (implies -u)")),
	OPT_BOOL(0, "resolved", &جوڙيو_resolved, N_("add conflict-resolved tracked files")),
	OPT_BOOL('N', "intent-to-add", &intent_منزل_جوڙيو, N_("record only the fact that the path will be added later")),
	OPT_BOOL('A', "all", &addremove_explicit, N_("add changes from all tracked and untracked files")),
	OPT_CALLBACK_F(0, "ignore-removal", &addremove_explicit,
	  عدم /* takes no arguments */,
	  N_("ignore paths removed in the working tree (same as --no-all)"),
	  PARSE_OPT_NOARG, ignore_removal_cb),
	OPT_BOOL( 0 , "refresh", &refresh_only, N_("don't add, only refresh the index")),
	OPT_BOOL( 0 , "ignore-errors", &ignore_جوڙيو_errors, N_("just skip files which cannot be added because of errors")),
	OPT_BOOL( 0 , "ignore-missing", &ignore_missing, N_("check if - even missing - files are ignored in dry run")),
	OPT_BOOL(0, "sparse", &include_sparse, N_("allow updating entries outside of the sparse-checkout cone")),
	OPT_STRING(0, "chmod", &chmod_arg, "(+|-)x",
		   N_("override the executable bit of the listed files")),
	OPT_HIDDEN_BOOL(0, "warn-embedded-repo", &warn_on_embedded_repo,
			N_("warn when adding an embedded repository")),
	OPT_PATHSPEC_FROM_FILE(&pathspec_from_file),
	OPT_PATHSPEC_FILE_NUL(&pathspec_file_nul),
	OPT_پڇاڙي(),
};

ساكن صحيح جوڙيو_config(ثابت حرف *var, ثابت حرف *قدر,
		      ثابت بنية config_context *ctx, فراغ *cb)
{
	إذا (!strcmp(var, "add.ignoreerrors") ||
	    !strcmp(var, "add.ignore-errors")) {
		ignore_جوڙيو_errors = git_config_bool(var, قدر);
		أرجع 0;
	}

	إذا (git_color_config(var, قدر, cb) < 0)
		أرجع -1;

	أرجع git_default_config(var, قدر, ctx, cb);
}

ساكن ثابت حرف embedded_advice[] = N_(
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

ساكن فراغ check_embedded_repo(ثابت حرف *رستو)
{
	بنية strbuf name = STRBUF_INIT;
	ساكن صحيح adviced_on_embedded_repo = 0;

	إذا (!warn_on_embedded_repo)
		أرجع;
	إذا (!پڇاڙيون_with(رستو, "/"))
		أرجع;

	/* Drop trailing slash for aesthetics */
	strbuf_addstr(&name, رستو);
	strbuf_strip_suffix(&name, "/");

	warning(_("adding embedded git repository: %s"), name.buf);
	إذا (!adviced_on_embedded_repo) {
		advise_if_enabled(ADVICE_جوڙيو_EMBEDDED_REPO,
				  embedded_advice, name.buf, name.buf);
		adviced_on_embedded_repo = 1;
	}

	strbuf_release(&name);
}

ساكن صحيح جوڙيو_files(بنية repository *repo, بنية dir_struct *dir, صحيح flags)
{
	صحيح i, exit_حالت = 0;
	بنية string_list matched_sparse_paths = STRING_LIST_INIT_NODUP;

	إذا (dir->ignored_nr) {
		fprintf(stderr, _(ignore_error));
		لكل (i = 0; i < dir->ignored_nr; i++)
			fprintf(stderr, "%s\n", dir->ignored[i]->name);
		advise_if_enabled(ADVICE_جوڙيو_IGNORED_FILE,
				  _("Use -f if you really want to add them."));
		exit_حالت = 1;
	}

	لكل (i = 0; i < dir->nr; i++) {
		إذا (!include_sparse &&
		    !رستو_اندريون_sparse_checkout(dir->entries[i]->name, repo->اشاريو)) {
			string_list_append(&matched_sparse_paths,
					   dir->entries[i]->name);
			استمر;
		}
		إذا (جوڙيو_file_منزل_اشاريو(repo->اشاريو, dir->entries[i]->name, flags)) {
			إذا (!ignore_جوڙيو_errors)
				die(_("adding files failed"));
			exit_حالت = 1;
		} وإلا {
			check_embedded_repo(dir->entries[i]->name);
		}
	}

	إذا (matched_sparse_paths.nr) {
		advise_on_updating_sparse_paths(&matched_sparse_paths);
		exit_حالت = 1;
	}

	string_list_clear(&matched_sparse_paths, 0);

	أرجع exit_حالت;
}

ساكن صحيح failed_منزل_جوڙيو(صحيح flags, ثابت حرف *رستو)
{
	إذا (!(flags & جوڙيو_CACHE_IGNORE_ERRORS))
		die(_("updating file '%s' failed"), رستو);
	أرجع 1;
}

ساكن صحيح جوڙيو_resolved_files(بنية repository *repo,
			      ثابت بنية pathspec *pathspec,
			      صحيح flags)
{
	بنية اشاريو_state *istate = repo->اشاريو;
	بنية string_list unmerged_paths = STRING_LIST_INIT_DUP;
	بنية string_list unresolved_paths = STRING_LIST_INIT_DUP;
	صحيح exit_حالت = 0;
	سائيز_t i;

	لكل (i = 0; i < istate->cache_nr; i++) {
		بنية cache_داخلا *ce = istate->cache[i];
		إذا (!ce_stage(ce))
			استمر;
		إذا (pathspec->nr && !ce_رستو_ميلاپ(istate, ce, pathspec, عدم))
			استمر;
		إذا (!unmerged_paths.nr ||
		    strcmp(unmerged_paths.شيون[unmerged_paths.nr - 1].string, ce->name))
			string_list_append(&unmerged_paths, ce->name);
	}

	إذا (!unmerged_paths.nr) {
		string_list_clear(&unmerged_paths, 0);
		أرجع 0;
	}

	لكل (i = 0; i < unmerged_paths.nr; i++) {
		ثابت حرف *رستو = unmerged_paths.شيون[i].string;
		بنية stat st;

		إذا (!lstat(رستو, &st) && S_ISREG(st.st_گهڻائي)) {
			إذا (has_conflict_markers(istate, رستو))
				string_list_append(&unresolved_paths, رستو);
		}
	}

	إذا (unresolved_paths.nr) {
		بنية strbuf sb = STRBUF_INIT;
		لكل (i = 0; i < unresolved_paths.nr; i++)
			strbuf_addf(&sb, "\t%s\n", unresolved_paths.شيون[i].string);
		die(_("the following paths still have conflict markers:\n%s"), sb.buf);
	}

	لكل (i = 0; i < unmerged_paths.nr; i++) {
		ثابت حرف *رستو = unmerged_paths.شيون[i].string;
		بنية stat st;

		إذا (lstat(رستو, &st)) {
			إذا (errno != ENOENT)
				die_errno(_("cannot lstat: '%s'"), رستو);
			إذا (remove_file_from_اشاريو_with_flags(istate, رستو, flags))
				exit_حالت = failed_منزل_جوڙيو(flags, رستو);
		} وإلا {
			إذا (جوڙيو_file_منزل_اشاريو(istate, رستو, flags))
				exit_حالت = failed_منزل_جوڙيو(flags, رستو);
		}
	}

	string_list_clear(&unmerged_paths, 0);
	string_list_clear(&unresolved_paths, 0);
	أرجع exit_حالت;
}

صحيح cmd_جوڙيو(صحيح argc,
	    ثابت حرف **argv,
	    ثابت حرف *اڳياڙي,
	    بنية repository *repo)
{
	صحيح exit_حالت = 0;
	بنية pathspec pathspec;
	بنية dir_struct dir = DIR_INIT;
	صحيح flags;
	صحيح جوڙيو_new_files;
	صحيح require_pathspec;
	حرف *ڏٺل = عدم;
	حرف *ps_matched = عدم;
	بنية تالو_file تالو_file = تالو_INIT;
	بنية odb_transaction *transaction = عدم;

	repo_config(repo, جوڙيو_config, عدم);

	argc = parse_options(argc, argv, اڳياڙي, builtin_جوڙيو_options,
			  builtin_جوڙيو_usage, PARSE_OPT_KEEP_ARGV0);

	prepare_repo_settings(repo);
	repo->settings.command_requires_ڀريل_اشاريو = 0;

	إذا (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	إذا (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	إذا (patch_interactive)
		جوڙيو_interactive = 1;
	إذا (جوڙيو_interactive) {
		إذا (show_only)
			die(_("options '%s' and '%s' cannot be used together"), "--dry-run", "--interactive/--patch");
		إذا (pathspec_from_file)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--interactive/--patch");
		ٻاهر_نڪرو(interactive_جوڙيو(repo, argv + 1, اڳياڙي, patch_interactive, &interactive_opts));
	} وإلا {
		إذا (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--interactive/--patch");
		إذا (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--interactive/--patch");
		إذا (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--interactive/--patch");
	}

	إذا (edit_interactive) {
		إذا (pathspec_from_file)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--edit");
		أرجع(edit_patch(repo, argc, argv, اڳياڙي));
	}
	argc--;
	argv++;

	إذا (0 <= addremove_explicit)
		addremove = addremove_explicit;
	وإلا إذا (take_worktree_changes && ADDREMOVE_DEFAULT)
		addremove = 0; /* "-u" was given but not "-A" */

	die_for_incompatible_opt3(take_worktree_changes, "-u/--update",
				  0 < addremove_explicit, "-A/--all",
				  جوڙيو_resolved, "--resolved");

	إذا (!show_only && ignore_missing)
		die(_("the option '%s' requires '%s'"), "--ignore-missing", "--dry-run");

	إذا (chmod_arg && ((chmod_arg[0] != '-' && chmod_arg[0] != '+') ||
			  chmod_arg[1] != 'x' || chmod_arg[2]))
		die(_("--chmod param '%s' must be either -x or +x"), chmod_arg);

	جوڙيو_new_files = !take_worktree_changes && !refresh_only &&
			!جوڙيو_renormalize && !جوڙيو_resolved;
	require_pathspec = !(take_worktree_changes ||
			     (0 < addremove_explicit) ||
			     جوڙيو_resolved);

	repo_hold_locked_اشاريو(repo, &تالو_file, تالو_DIE_ON_ERROR);

	/*
	 * Check the "pathspec '%s' did not match any files" block
	 * below before enabling new magic.
	 */
	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_ڀريل |
		       PATHSPEC_SYMLINK_LEADING_رستو,
		       اڳياڙي, argv);

	إذا (pathspec_from_file) {
		إذا (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_ڀريل |
				    PATHSPEC_SYMLINK_LEADING_رستو,
				    اڳياڙي, pathspec_from_file, pathspec_file_nul);
	} وإلا إذا (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	إذا (require_pathspec && pathspec.nr == 0) {
		fprintf(stderr, _("Nothing specified, nothing added.\n"));
		advise_if_enabled(ADVICE_جوڙيو_خالي_جايون_PATHSPEC,
				  _("Maybe you wanted to say 'git add .'?"));
		أرجع 0;
	}

	إذا (!take_worktree_changes && !جوڙيو_resolved &&
	    addremove_explicit < 0 && pathspec.nr)
		/* Turn "git add pathspec..." to "git add -A pathspec..." */
		addremove = 1;

	flags = ((verbose ? جوڙيو_CACHE_VERBOSE : 0) |
		 (show_only ? جوڙيو_CACHE_PRETEND : 0) |
		 (intent_منزل_جوڙيو ? جوڙيو_CACHE_INTENT : 0) |
		 (ignore_جوڙيو_errors ? جوڙيو_CACHE_IGNORE_ERRORS : 0) |
		 (!(addremove || take_worktree_changes)
		  ? جوڙيو_CACHE_IGNORE_REMOVAL : 0));

	إذا (repo_read_اشاريو_preload(repo, &pathspec, 0) < 0)
		die(_("index file corrupt"));

	die_اندريون_unpopulated_submodule(repo->اشاريو, اڳياڙي);
	die_رستو_inside_submodule(repo->اشاريو, &pathspec);

	إذا (جوڙيو_new_files) {
		صحيح baselen;

		/* Set up the default git porcelain excludes */
		إذا (!ignored_too) {
			dir.flags |= DIR_COLLECT_IGNORED;
			تياري_standard_excludes(&dir);
		}

		/* This picks up the paths that are not tracked */
		baselen = fill_directory(&dir, repo->اشاريو, &pathspec);
		إذا (pathspec.nr)
			ڏٺل = prune_directory(repo, &dir, &pathspec, baselen);
	}

	إذا (refresh_only) {
		exit_حالت |= refresh(repo, verbose, &pathspec);
		اذهب پڄاڻي;
	}

	إذا (pathspec.nr) {
		صحيح i;
		حرف *skip_worktree_ڏٺل = عدم;
		بنية string_list only_ميلاپ_skip_worktree = STRING_LIST_INIT_NODUP;

		إذا (!ڏٺل)
			ڏٺل = ڳوليو_pathspecs_matching_against_اشاريو(&pathspec,
					repo->اشاريو, PS_IGNORE_SKIP_WORKTREE);

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

		لكل (i = 0; i < pathspec.nr; i++) {
			ثابت حرف *رستو = pathspec.شيون[i].ميلاپ;

			إذا (pathspec.شيون[i].magic & PATHSPEC_EXCLUDE)
				استمر;
			إذا (ڏٺل[i])
				استمر;

			إذا (!include_sparse &&
			    matches_skip_worktree(&pathspec, i, &skip_worktree_ڏٺل)) {
				string_list_append(&only_ميلاپ_skip_worktree,
						   pathspec.شيون[i].original);
				استمر;
			}

			/* Don't complain at 'git add .' on empty repo */
			إذا (!رستو[0])
				استمر;

			إذا ((pathspec.شيون[i].magic & (PATHSPEC_GLOB | PATHSPEC_ICASE)) ||
			    !file_exists(رستو)) {
				إذا (ignore_missing) {
					صحيح dtype = DT_UNKNOWN;
					إذا (is_excluded(&dir, repo->اشاريو, رستو, &dtype))
						dir_جوڙيو_ignored(&dir, repo->اشاريو,
								رستو, pathspec.شيون[i].len);
				} وإلا
					die(_("pathspec '%s' did not match any files"),
					    pathspec.شيون[i].original);
			}
		}


		إذا (only_ميلاپ_skip_worktree.nr) {
			advise_on_updating_sparse_paths(&only_ميلاپ_skip_worktree);
			exit_حالت = 1;
		}

		حرر(ڏٺل);
		حرر(skip_worktree_ڏٺل);
		string_list_clear(&only_ميلاپ_skip_worktree, 0);
	}

	odb_transaction_begin_or_die(repo->objects, &transaction, 0);

	ps_matched = xcalloc(pathspec.nr, 1);
	إذا (جوڙيو_resolved)
		exit_حالت |= جوڙيو_resolved_files(repo, &pathspec, flags);
	وإلا إذا (جوڙيو_renormalize)
		exit_حالت |= renormalize_tracked_files(repo, &pathspec, flags);
	وإلا
		exit_حالت |= جوڙيو_files_منزل_cache(repo, اڳياڙي,
						  &pathspec, ps_matched,
						  include_sparse, flags, ignored_too);

	إذا (take_worktree_changes && !جوڙيو_renormalize && !ignore_جوڙيو_errors &&
	    report_رستو_error(ps_matched, &pathspec))
		ٻاهر_نڪرو(128);

	إذا (جوڙيو_new_files)
		exit_حالت |= جوڙيو_files(repo, &dir, flags);

	إذا (chmod_arg && pathspec.nr)
		exit_حالت |= chmod_pathspec(repo, &pathspec, chmod_arg[0], show_only);
	odb_transaction_commit_and_finalize_or_die(transaction);

پڄاڻي:
	إذا (write_locked_اشاريو(repo->اشاريو, &تالو_file,
			       COMMIT_تالو | SKIP_IF_UNCHANGED))
		die(_("unable to write new index file"));

	حرر(ps_matched);
	dir_clear(&dir);
	clear_pathspec(&pathspec);
	أرجع exit_حالت;
}
