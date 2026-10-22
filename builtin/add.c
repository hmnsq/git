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

ساكن ثابت حرف * ثابت builtin_جمع_کن_usage[] = {
	N_("git add [<options>] [--] <pathspec>..."),
	عدم
};
ساكن صحيح patch_interactive, جمع_کن_interactive, edit_interactive;
ساكن بنية interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
ساكن صحيح take_worktree_changes;
ساكن صحيح جمع_کن_renormalize;
ساكن صحيح جمع_کن_resolved;
ساكن صحيح pathspec_file_nul;
ساكن صحيح include_sparse;
ساكن ثابت حرف *pathspec_from_file;

ساكن صحيح chmod_pathspec(بنية repository *repo,
			  بنية pathspec *pathspec,
			  حرف flip,
			  صحيح show_only)
{
	صحيح ret = 0;

	لكل (اندازه_t i = 0; i < repo->شاخص->cache_nr; i++) {
		بنية cache_مدخل *ce = repo->شاخص->cache[i];
		صحيح err;

		إذا (!include_sparse &&
		    (ce_skip_worktree(ce) ||
		     !مسیر_درون_sparse_checkout(ce->name, repo->شاخص)))
			استمر;

		إذا (pathspec && !ce_مسیر_تطابق(repo->شاخص, ce, pathspec, عدم))
			استمر;

		إذا (!show_only)
			err = chmod_شاخص_مدخل(repo->شاخص, ce, flip);
		وإلا
			err = S_ISREG(ce->ce_نما) ? 0 : -1;

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

	لكل (اندازه_t i = 0; i < repo->شاخص->cache_nr; i++) {
		بنية cache_مدخل *ce = repo->شاخص->cache[i];

		إذا (!include_sparse &&
		    (ce_skip_worktree(ce) ||
		     !مسیر_درون_sparse_checkout(ce->name, repo->شاخص)))
			استمر;
		إذا (ce_stage(ce))
			استمر; /* do not touch unmerged paths */
		إذا (!S_ISREG(ce->ce_نما) && !S_ISLNK(ce->ce_نما))
			استمر; /* do not touch non blobs */
		إذا (pathspec && !ce_مسیر_تطابق(repo->شاخص, ce, pathspec, عدم))
			استمر;
		retval |= جمع_کن_file_مقصد_شاخص(repo->شاخص, ce->name,
					    flags | جمع_کن_CACHE_RENORMALIZE);
	}

	أرجع retval;
}

ساكن حرف *prune_directory(بنية repository *repo,
			     بنية dir_struct *dir,
			     بنية pathspec *pathspec,
			     صحيح پیشوند)
{
	حرف *دیده;
	صحيح i;
	بنية dir_مدخل **src, **dst;

	دیده = xcalloc(pathspec->nr, 1);

	src = dst = dir->entries;
	i = dir->nr;
	طالما (--i >= 0) {
		بنية dir_مدخل *مدخل = *src++;
		إذا (dir_مسیر_تطابق(repo->شاخص, مدخل, pathspec, پیشوند, دیده))
			*dst++ = مدخل;
	}
	dir->nr = dst - dir->entries;
	جمع_کن_pathspec_matches_against_شاخص(pathspec, repo->شاخص, دیده,
					   PS_IGNORE_SKIP_WORKTREE);
	أرجع دیده;
}

ساكن صحيح refresh(بنية repository *repo, صحيح verbose, ثابت بنية pathspec *pathspec)
{
	حرف *دیده;
	صحيح i, ret = 0;
	حرف *skip_worktree_دیده = عدم;
	بنية string_list only_تطابق_skip_worktree = STRING_LIST_INIT_NODUP;
	بیعلامت صحيح flags = REFRESH_IGNORE_SKIP_WORKTREE |
		    (verbose ? REFRESH_درون_PORCELAIN : REFRESH_QUIET);

	دیده = xcalloc(pathspec->nr, 1);
	refresh_شاخص(repo->شاخص, flags, pathspec, دیده,
		      _("Unstaged changes after refreshing the index:"));
	لكل (i = 0; i < pathspec->nr; i++) {
		إذا (!دیده[i]) {
			ثابت حرف *مسیر = pathspec->اقلام[i].original;

			إذا (matches_skip_worktree(pathspec, i, &skip_worktree_دیده) ||
			    !مسیر_درون_sparse_checkout(مسیر, repo->شاخص)) {
				string_list_append(&only_تطابق_skip_worktree,
						   pathspec->اقلام[i].original);
			} وإلا {
				die(_("pathspec '%s' did not match any files"),
				    pathspec->اقلام[i].original);
			}
		}
	}

	إذا (only_تطابق_skip_worktree.nr) {
		advise_on_updating_sparse_paths(&only_تطابق_skip_worktree);
		ret = 1;
	}

	حرر(دیده);
	حرر(skip_worktree_دیده);
	string_list_clear(&only_تطابق_skip_worktree, 0);
	أرجع ret;
}

صحيح interactive_جمع_کن(بنية repository *repo,
		    ثابت حرف **argv,
		    ثابت حرف *پیشوند,
		    صحيح patch, بنية interactive_options *interactive_opts)
{
	بنية pathspec pathspec;
	صحيح ret;

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_پر |
		       PATHSPEC_SYMLINK_LEADING_مسیر |
		       PATHSPEC_پیشوند_ORIGIN,
		       پیشوند, argv);

	إذا (patch)
		ret = !!پیاپی_جمع_کن_p(repo, جمع_کن_P_جمع_کن, interactive_opts, عدم, &pathspec, 0);
	وإلا
		ret = !!پیاپی_جمع_کن_i(repo, &pathspec, interactive_opts);

	clear_pathspec(&pathspec);
	أرجع ret;
}

ساكن صحيح edit_patch(بنية repository *repo,
		      صحيح argc,
		      ثابت حرف **argv,
		      ثابت حرف *پیشوند)
{
	حرف *file = repo_git_مسیر(repo, "ADD_EDIT.patch");
	بنية apply_state state;
	ثابت حرف *apply_argv[2];
	بنية rev_info rev;
	صحيح بیرون;
	بنية stat st;

	repo_config(repo, git_diff_basic_config, عدم);

	إذا (repo_read_شاخص(repo) < 0)
		die(_("could not read the index"));

	repo_init_revisions(repo, &rev, پیشوند);
	rev.diffopt.context = 7;

	argc = راهاندازی_revisions(argc, argv, &rev, عدم);
	rev.diffopt.output_format = DIFF_FORMAT_PATCH;
	rev.diffopt.use_color = GIT_COLOR_NEVER;
	rev.diffopt.flags.ignore_dirty_submodules = 1;
	بیرون = xopen(file, O_CREAT | O_WRONLY | O_TRUNC, 0666);
	rev.diffopt.file = xfdopen(بیرون, "w");
	rev.diffopt.close_file = 1;
	پیاپی_diff_files(&rev, 0);

	إذا (launch_editor(file, عدم, عدم))
		die(_("editing patch failed"));

	إذا (stat(file, &st))
		die_errno(_("could not stat '%s'"), file);
	إذا (!st.st_اندازه)
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
ساكن صحيح ignore_جمع_کن_errors, intent_مقصد_جمع_کن, ignore_missing;
ساكن صحيح warn_on_embedded_repo = 1;

#تعریف_کن ADDREMOVE_DEFAULT 1
ساكن صحيح addremove = ADDREMOVE_DEFAULT;
ساكن صحيح addremove_explicit = -1; /* unspecified */

ساكن حرف *chmod_arg;

ساكن صحيح ignore_removal_cb(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_ARG(arg);

	/* if we are told to ignore, we are not adding removals */
	*(صحيح *)opt->مقدار = !unset ? 0 : 1;
	أرجع 0;
}

ساكن بنية option builtin_جمع_کن_options[] = {
	OPT_DRY_پیاپی(&show_only, N_("dry run")),
	OPT__VERBOSE(&verbose, N_("be verbose")),
	OPT_گروه(""),
	OPT_BOOL('i', "interactive", &جمع_کن_interactive, N_("interactive picking")),
	OPT_BOOL('p', "patch", &patch_interactive, N_("select hunks interactively")),
	OPT_BOOL(0, "auto-advance", &interactive_opts.auto_advance,
		 N_("auto advance to the next file when selecting hunks interactively")),
	OPT_DIFF_UNIFIED(&interactive_opts.context),
	OPT_DIFF_INTERHUNK_CONTEXT(&interactive_opts.interhunkcontext),
	OPT_BOOL('e', "edit", &edit_interactive, N_("edit current diff and apply")),
	OPT__FORCE(&ignored_too, N_("allow adding otherwise ignored files"), 0),
	OPT_BOOL('u', "update", &take_worktree_changes, N_("update tracked files")),
	OPT_BOOL(0, "renormalize", &جمع_کن_renormalize, N_("renormalize EOL of tracked files (implies -u)")),
	OPT_BOOL(0, "resolved", &جمع_کن_resolved, N_("add conflict-resolved tracked files")),
	OPT_BOOL('N', "intent-to-add", &intent_مقصد_جمع_کن, N_("record only the fact that the path will be added later")),
	OPT_BOOL('A', "all", &addremove_explicit, N_("add changes from all tracked and untracked files")),
	OPT_CALLBACK_F(0, "ignore-removal", &addremove_explicit,
	  عدم /* takes no arguments */,
	  N_("ignore paths removed in the working tree (same as --no-all)"),
	  PARSE_OPT_NOARG, ignore_removal_cb),
	OPT_BOOL( 0 , "refresh", &refresh_only, N_("don't add, only refresh the index")),
	OPT_BOOL( 0 , "ignore-errors", &ignore_جمع_کن_errors, N_("just skip files which cannot be added because of errors")),
	OPT_BOOL( 0 , "ignore-missing", &ignore_missing, N_("check if - even missing - files are ignored in dry run")),
	OPT_BOOL(0, "sparse", &include_sparse, N_("allow updating entries outside of the sparse-checkout cone")),
	OPT_STRING(0, "chmod", &chmod_arg, "(+|-)x",
		   N_("override the executable bit of the listed files")),
	OPT_HIDDEN_BOOL(0, "warn-embedded-repo", &warn_on_embedded_repo,
			N_("warn when adding an embedded repository")),
	OPT_PATHSPEC_FROM_FILE(&pathspec_from_file),
	OPT_PATHSPEC_FILE_NUL(&pathspec_file_nul),
	OPT_انتها(),
};

ساكن صحيح جمع_کن_config(ثابت حرف *var, ثابت حرف *مقدار,
		      ثابت بنية config_context *ctx, فراغ *cb)
{
	إذا (!strcmp(var, "add.ignoreerrors") ||
	    !strcmp(var, "add.ignore-errors")) {
		ignore_جمع_کن_errors = git_config_bool(var, مقدار);
		أرجع 0;
	}

	إذا (git_color_config(var, مقدار, cb) < 0)
		أرجع -1;

	أرجع git_default_config(var, مقدار, ctx, cb);
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

ساكن فراغ check_embedded_repo(ثابت حرف *مسیر)
{
	بنية strbuf name = STRBUF_INIT;
	ساكن صحيح adviced_on_embedded_repo = 0;

	إذا (!warn_on_embedded_repo)
		أرجع;
	إذا (!دوسر_with(مسیر, "/"))
		أرجع;

	/* Drop trailing slash for aesthetics */
	strbuf_addstr(&name, مسیر);
	strbuf_strip_suffix(&name, "/");

	warning(_("adding embedded git repository: %s"), name.buf);
	إذا (!adviced_on_embedded_repo) {
		advise_if_enabled(ADVICE_جمع_کن_EMBEDDED_REPO,
				  embedded_advice, name.buf, name.buf);
		adviced_on_embedded_repo = 1;
	}

	strbuf_release(&name);
}

ساكن صحيح جمع_کن_files(بنية repository *repo, بنية dir_struct *dir, صحيح flags)
{
	صحيح i, exit_وضعیت = 0;
	بنية string_list matched_sparse_paths = STRING_LIST_INIT_NODUP;

	إذا (dir->ignored_nr) {
		fprintf(stderr, _(ignore_error));
		لكل (i = 0; i < dir->ignored_nr; i++)
			fprintf(stderr, "%s\n", dir->ignored[i]->name);
		advise_if_enabled(ADVICE_جمع_کن_IGNORED_FILE,
				  _("Use -f if you really want to add them."));
		exit_وضعیت = 1;
	}

	لكل (i = 0; i < dir->nr; i++) {
		إذا (!include_sparse &&
		    !مسیر_درون_sparse_checkout(dir->entries[i]->name, repo->شاخص)) {
			string_list_append(&matched_sparse_paths,
					   dir->entries[i]->name);
			استمر;
		}
		إذا (جمع_کن_file_مقصد_شاخص(repo->شاخص, dir->entries[i]->name, flags)) {
			إذا (!ignore_جمع_کن_errors)
				die(_("adding files failed"));
			exit_وضعیت = 1;
		} وإلا {
			check_embedded_repo(dir->entries[i]->name);
		}
	}

	إذا (matched_sparse_paths.nr) {
		advise_on_updating_sparse_paths(&matched_sparse_paths);
		exit_وضعیت = 1;
	}

	string_list_clear(&matched_sparse_paths, 0);

	أرجع exit_وضعیت;
}

ساكن صحيح failed_مقصد_جمع_کن(صحيح flags, ثابت حرف *مسیر)
{
	إذا (!(flags & جمع_کن_CACHE_IGNORE_ERRORS))
		die(_("updating file '%s' failed"), مسیر);
	أرجع 1;
}

ساكن صحيح جمع_کن_resolved_files(بنية repository *repo,
			      ثابت بنية pathspec *pathspec,
			      صحيح flags)
{
	بنية شاخص_state *istate = repo->شاخص;
	بنية string_list unmerged_paths = STRING_LIST_INIT_DUP;
	بنية string_list unresolved_paths = STRING_LIST_INIT_DUP;
	صحيح exit_وضعیت = 0;
	اندازه_t i;

	لكل (i = 0; i < istate->cache_nr; i++) {
		بنية cache_مدخل *ce = istate->cache[i];
		إذا (!ce_stage(ce))
			استمر;
		إذا (pathspec->nr && !ce_مسیر_تطابق(istate, ce, pathspec, عدم))
			استمر;
		إذا (!unmerged_paths.nr ||
		    strcmp(unmerged_paths.اقلام[unmerged_paths.nr - 1].string, ce->name))
			string_list_append(&unmerged_paths, ce->name);
	}

	إذا (!unmerged_paths.nr) {
		string_list_clear(&unmerged_paths, 0);
		أرجع 0;
	}

	لكل (i = 0; i < unmerged_paths.nr; i++) {
		ثابت حرف *مسیر = unmerged_paths.اقلام[i].string;
		بنية stat st;

		إذا (!lstat(مسیر, &st) && S_ISREG(st.st_نما)) {
			إذا (has_conflict_markers(istate, مسیر))
				string_list_append(&unresolved_paths, مسیر);
		}
	}

	إذا (unresolved_paths.nr) {
		بنية strbuf sb = STRBUF_INIT;
		لكل (i = 0; i < unresolved_paths.nr; i++)
			strbuf_addf(&sb, "\t%s\n", unresolved_paths.اقلام[i].string);
		die(_("the following paths still have conflict markers:\n%s"), sb.buf);
	}

	لكل (i = 0; i < unmerged_paths.nr; i++) {
		ثابت حرف *مسیر = unmerged_paths.اقلام[i].string;
		بنية stat st;

		إذا (lstat(مسیر, &st)) {
			إذا (errno != ENOENT)
				die_errno(_("cannot lstat: '%s'"), مسیر);
			إذا (remove_file_from_شاخص_with_flags(istate, مسیر, flags))
				exit_وضعیت = failed_مقصد_جمع_کن(flags, مسیر);
		} وإلا {
			إذا (جمع_کن_file_مقصد_شاخص(istate, مسیر, flags))
				exit_وضعیت = failed_مقصد_جمع_کن(flags, مسیر);
		}
	}

	string_list_clear(&unmerged_paths, 0);
	string_list_clear(&unresolved_paths, 0);
	أرجع exit_وضعیت;
}

صحيح cmd_جمع_کن(صحيح argc,
	    ثابت حرف **argv,
	    ثابت حرف *پیشوند,
	    بنية repository *repo)
{
	صحيح exit_وضعیت = 0;
	بنية pathspec pathspec;
	بنية dir_struct dir = DIR_INIT;
	صحيح flags;
	صحيح جمع_کن_new_files;
	صحيح require_pathspec;
	حرف *دیده = عدم;
	حرف *ps_matched = عدم;
	بنية قفل_file قفل_file = قفل_INIT;
	بنية odb_transaction *transaction = عدم;

	repo_config(repo, جمع_کن_config, عدم);

	argc = parse_options(argc, argv, پیشوند, builtin_جمع_کن_options,
			  builtin_جمع_کن_usage, PARSE_OPT_KEEP_ARGV0);

	prepare_repo_settings(repo);
	repo->settings.command_requires_پر_شاخص = 0;

	إذا (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	إذا (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	إذا (patch_interactive)
		جمع_کن_interactive = 1;
	إذا (جمع_کن_interactive) {
		إذا (show_only)
			die(_("options '%s' and '%s' cannot be used together"), "--dry-run", "--interactive/--patch");
		إذا (pathspec_from_file)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--interactive/--patch");
		خروج(interactive_جمع_کن(repo, argv + 1, پیشوند, patch_interactive, &interactive_opts));
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
		أرجع(edit_patch(repo, argc, argv, پیشوند));
	}
	argc--;
	argv++;

	إذا (0 <= addremove_explicit)
		addremove = addremove_explicit;
	وإلا إذا (take_worktree_changes && ADDREMOVE_DEFAULT)
		addremove = 0; /* "-u" was given but not "-A" */

	die_for_incompatible_opt3(take_worktree_changes, "-u/--update",
				  0 < addremove_explicit, "-A/--all",
				  جمع_کن_resolved, "--resolved");

	إذا (!show_only && ignore_missing)
		die(_("the option '%s' requires '%s'"), "--ignore-missing", "--dry-run");

	إذا (chmod_arg && ((chmod_arg[0] != '-' && chmod_arg[0] != '+') ||
			  chmod_arg[1] != 'x' || chmod_arg[2]))
		die(_("--chmod param '%s' must be either -x or +x"), chmod_arg);

	جمع_کن_new_files = !take_worktree_changes && !refresh_only &&
			!جمع_کن_renormalize && !جمع_کن_resolved;
	require_pathspec = !(take_worktree_changes ||
			     (0 < addremove_explicit) ||
			     جمع_کن_resolved);

	repo_hold_locked_شاخص(repo, &قفل_file, قفل_DIE_ON_ERROR);

	/*
	 * Check the "pathspec '%s' did not match any files" block
	 * below before enabling new magic.
	 */
	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_پر |
		       PATHSPEC_SYMLINK_LEADING_مسیر,
		       پیشوند, argv);

	إذا (pathspec_from_file) {
		إذا (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_پر |
				    PATHSPEC_SYMLINK_LEADING_مسیر,
				    پیشوند, pathspec_from_file, pathspec_file_nul);
	} وإلا إذا (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	إذا (require_pathspec && pathspec.nr == 0) {
		fprintf(stderr, _("Nothing specified, nothing added.\n"));
		advise_if_enabled(ADVICE_جمع_کن_خالی_PATHSPEC,
				  _("Maybe you wanted to say 'git add .'?"));
		أرجع 0;
	}

	إذا (!take_worktree_changes && !جمع_کن_resolved &&
	    addremove_explicit < 0 && pathspec.nr)
		/* Turn "git add pathspec..." to "git add -A pathspec..." */
		addremove = 1;

	flags = ((verbose ? جمع_کن_CACHE_VERBOSE : 0) |
		 (show_only ? جمع_کن_CACHE_PRETEND : 0) |
		 (intent_مقصد_جمع_کن ? جمع_کن_CACHE_INTENT : 0) |
		 (ignore_جمع_کن_errors ? جمع_کن_CACHE_IGNORE_ERRORS : 0) |
		 (!(addremove || take_worktree_changes)
		  ? جمع_کن_CACHE_IGNORE_REMOVAL : 0));

	إذا (repo_read_شاخص_preload(repo, &pathspec, 0) < 0)
		die(_("index file corrupt"));

	die_درون_unpopulated_submodule(repo->شاخص, پیشوند);
	die_مسیر_inside_submodule(repo->شاخص, &pathspec);

	إذا (جمع_کن_new_files) {
		صحيح baselen;

		/* Set up the default git porcelain excludes */
		إذا (!ignored_too) {
			dir.flags |= DIR_COLLECT_IGNORED;
			راهاندازی_standard_excludes(&dir);
		}

		/* This picks up the paths that are not tracked */
		baselen = fill_directory(&dir, repo->شاخص, &pathspec);
		إذا (pathspec.nr)
			دیده = prune_directory(repo, &dir, &pathspec, baselen);
	}

	إذا (refresh_only) {
		exit_وضعیت |= refresh(repo, verbose, &pathspec);
		اذهب پایان;
	}

	إذا (pathspec.nr) {
		صحيح i;
		حرف *skip_worktree_دیده = عدم;
		بنية string_list only_تطابق_skip_worktree = STRING_LIST_INIT_NODUP;

		إذا (!دیده)
			دیده = پیدا_کن_pathspecs_matching_against_شاخص(&pathspec,
					repo->شاخص, PS_IGNORE_SKIP_WORKTREE);

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
			ثابت حرف *مسیر = pathspec.اقلام[i].تطابق;

			إذا (pathspec.اقلام[i].magic & PATHSPEC_EXCLUDE)
				استمر;
			إذا (دیده[i])
				استمر;

			إذا (!include_sparse &&
			    matches_skip_worktree(&pathspec, i, &skip_worktree_دیده)) {
				string_list_append(&only_تطابق_skip_worktree,
						   pathspec.اقلام[i].original);
				استمر;
			}

			/* Don't complain at 'git add .' on empty repo */
			إذا (!مسیر[0])
				استمر;

			إذا ((pathspec.اقلام[i].magic & (PATHSPEC_GLOB | PATHSPEC_ICASE)) ||
			    !file_exists(مسیر)) {
				إذا (ignore_missing) {
					صحيح dtype = DT_UNKNOWN;
					إذا (is_excluded(&dir, repo->شاخص, مسیر, &dtype))
						dir_جمع_کن_ignored(&dir, repo->شاخص,
								مسیر, pathspec.اقلام[i].len);
				} وإلا
					die(_("pathspec '%s' did not match any files"),
					    pathspec.اقلام[i].original);
			}
		}


		إذا (only_تطابق_skip_worktree.nr) {
			advise_on_updating_sparse_paths(&only_تطابق_skip_worktree);
			exit_وضعیت = 1;
		}

		حرر(دیده);
		حرر(skip_worktree_دیده);
		string_list_clear(&only_تطابق_skip_worktree, 0);
	}

	odb_transaction_begin_or_die(repo->objects, &transaction, 0);

	ps_matched = xcalloc(pathspec.nr, 1);
	إذا (جمع_کن_resolved)
		exit_وضعیت |= جمع_کن_resolved_files(repo, &pathspec, flags);
	وإلا إذا (جمع_کن_renormalize)
		exit_وضعیت |= renormalize_tracked_files(repo, &pathspec, flags);
	وإلا
		exit_وضعیت |= جمع_کن_files_مقصد_cache(repo, پیشوند,
						  &pathspec, ps_matched,
						  include_sparse, flags, ignored_too);

	إذا (take_worktree_changes && !جمع_کن_renormalize && !ignore_جمع_کن_errors &&
	    report_مسیر_error(ps_matched, &pathspec))
		خروج(128);

	إذا (جمع_کن_new_files)
		exit_وضعیت |= جمع_کن_files(repo, &dir, flags);

	إذا (chmod_arg && pathspec.nr)
		exit_وضعیت |= chmod_pathspec(repo, &pathspec, chmod_arg[0], show_only);
	odb_transaction_commit_and_finalize_or_die(transaction);

پایان:
	إذا (write_locked_شاخص(repo->شاخص, &قفل_file,
			       COMMIT_قفل | SKIP_IF_UNCHANGED))
		die(_("unable to write new index file"));

	حرر(ps_matched);
	dir_clear(&dir);
	clear_pathspec(&pathspec);
	أرجع exit_وضعیت;
}
