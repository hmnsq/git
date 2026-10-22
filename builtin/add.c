/*
 * "git add" builtin command
 *
 * Copyright (C) 2006 Linus Torvalds
 */

#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "advice.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "lockfile.h"
#অন্তর্ভাব "editor.h"
#অন্তর্ভাব "dir.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "pathspec.h"
#অন্তর্ভাব "object-file.h"
#অন্তর্ভাব "odb.h"
#অন্তর্ভাব "odb/transaction.h"
#অন্তর্ভাব "parse-options.h"
#অন্তর্ভাব "path.h"
#অন্তর্ভাব "preload-index.h"
#অন্তর্ভাব "diff.h"
#অন্তর্ভাব "read-cache.h"
#অন্তর্ভাব "revision.h"
#অন্তর্ভাব "submodule.h"
#অন্তর্ভাব "add-interactive.h"
#অন্তর্ভাব "merge-ll.h"
#অন্তর্ভাব "apply.h"

স্থির ধ্রুবক অক্ষর * ধ্রুবক builtin_add_usage[] = {
	N_("git add [<options>] [--] <pathspec>..."),
	ফাঁকা
};
স্থির পূর্ণসংখ্যা patch_interactive, add_interactive, edit_interactive;
স্থির কাঠামো interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
স্থির পূর্ণসংখ্যা take_worktree_changes;
স্থির পূর্ণসংখ্যা add_renormalize;
স্থির পূর্ণসংখ্যা add_resolved;
স্থির পূর্ণসংখ্যা pathspec_file_nul;
স্থির পূর্ণসংখ্যা include_sparse;
স্থির ধ্রুবক অক্ষর *pathspec_from_file;

স্থির পূর্ণসংখ্যা chmod_pathspec(কাঠামো repository *repo,
			  কাঠামো pathspec *pathspec,
			  অক্ষর flip,
			  পূর্ণসংখ্যা show_only)
{
	পূর্ণসংখ্যা ret = 0;

	জন্য (আকার_t i = 0; i < repo->সূচক->cache_nr; i++) {
		কাঠামো cache_entry *ce = repo->সূচক->cache[i];
		পূর্ণসংখ্যা err;

		যদি (!include_sparse &&
		    (ce_skip_worktree(ce) ||
		     !path_in_sparse_checkout(ce->name, repo->সূচক)))
			চালু;

		যদি (pathspec && !ce_path_match(repo->সূচক, ce, pathspec, ফাঁকা))
			চালু;

		যদি (!show_only)
			err = chmod_সূচক_entry(repo->সূচক, ce, flip);
		অন্যথা
			err = S_ISREG(ce->ce_mode) ? 0 : -1;

		যদি (err < 0)
			ret = error(_("cannot chmod %cx '%s'"), flip, ce->name);
	}

	ফেরত ret;
}

স্থির পূর্ণসংখ্যা renormalize_tracked_files(কাঠামো repository *repo,
				     ধ্রুবক কাঠামো pathspec *pathspec,
				     পূর্ণসংখ্যা flags)
{
	পূর্ণসংখ্যা retval = 0;

	জন্য (আকার_t i = 0; i < repo->সূচক->cache_nr; i++) {
		কাঠামো cache_entry *ce = repo->সূচক->cache[i];

		যদি (!include_sparse &&
		    (ce_skip_worktree(ce) ||
		     !path_in_sparse_checkout(ce->name, repo->সূচক)))
			চালু;
		যদি (ce_stage(ce))
			চালু; /* do not touch unmerged paths */
		যদি (!S_ISREG(ce->ce_mode) && !S_ISLNK(ce->ce_mode))
			চালু; /* do not touch non blobs */
		যদি (pathspec && !ce_path_match(repo->সূচক, ce, pathspec, ফাঁকা))
			চালু;
		retval |= add_file_to_সূচক(repo->সূচক, ce->name,
					    flags | ADD_CACHE_RENORMALIZE);
	}

	ফেরত retval;
}

স্থির অক্ষর *prune_directory(কাঠামো repository *repo,
			     কাঠামো dir_struct *dir,
			     কাঠামো pathspec *pathspec,
			     পূর্ণসংখ্যা prefix)
{
	অক্ষর *seen;
	পূর্ণসংখ্যা i;
	কাঠামো dir_entry **src, **dst;

	seen = xcalloc(pathspec->nr, 1);

	src = dst = dir->entries;
	i = dir->nr;
	যখন (--i >= 0) {
		কাঠামো dir_entry *entry = *src++;
		যদি (dir_path_match(repo->সূচক, entry, pathspec, prefix, seen))
			*dst++ = entry;
	}
	dir->nr = dst - dir->entries;
	add_pathspec_matches_against_সূচক(pathspec, repo->সূচক, seen,
					   PS_IGNORE_SKIP_WORKTREE);
	ফেরত seen;
}

স্থির পূর্ণসংখ্যা refresh(কাঠামো repository *repo, পূর্ণসংখ্যা verbose, ধ্রুবক কাঠামো pathspec *pathspec)
{
	অক্ষর *seen;
	পূর্ণসংখ্যা i, ret = 0;
	অক্ষর *skip_worktree_seen = ফাঁকা;
	কাঠামো string_list only_match_skip_worktree = STRING_LIST_INIT_NODUP;
	unsigned পূর্ণসংখ্যা flags = REFRESH_IGNORE_SKIP_WORKTREE |
		    (verbose ? REFRESH_IN_PORCELAIN : REFRESH_QUIET);

	seen = xcalloc(pathspec->nr, 1);
	refresh_সূচক(repo->সূচক, flags, pathspec, seen,
		      _("Unstaged changes after refreshing the index:"));
	জন্য (i = 0; i < pathspec->nr; i++) {
		যদি (!seen[i]) {
			ধ্রুবক অক্ষর *path = pathspec->বস্তুসমূহ[i].original;

			যদি (matches_skip_worktree(pathspec, i, &skip_worktree_seen) ||
			    !path_in_sparse_checkout(path, repo->সূচক)) {
				string_list_append(&only_match_skip_worktree,
						   pathspec->বস্তুসমূহ[i].original);
			} অন্যথা {
				die(_("pathspec '%s' did not match any files"),
				    pathspec->বস্তুসমূহ[i].original);
			}
		}
	}

	যদি (only_match_skip_worktree.nr) {
		advise_on_updating_sparse_paths(&only_match_skip_worktree);
		ret = 1;
	}

	মুক্ত(seen);
	মুক্ত(skip_worktree_seen);
	string_list_clear(&only_match_skip_worktree, 0);
	ফেরত ret;
}

পূর্ণসংখ্যা interactive_add(কাঠামো repository *repo,
		    ধ্রুবক অক্ষর **argv,
		    ধ্রুবক অক্ষর *prefix,
		    পূর্ণসংখ্যা patch, কাঠামো interactive_options *interactive_opts)
{
	কাঠামো pathspec pathspec;
	পূর্ণসংখ্যা ret;

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_FULL |
		       PATHSPEC_SYMLINK_LEADING_PATH |
		       PATHSPEC_PREFIX_ORIGIN,
		       prefix, argv);

	যদি (patch)
		ret = !!run_add_p(repo, ADD_P_ADD, interactive_opts, ফাঁকা, &pathspec, 0);
	অন্যথা
		ret = !!run_add_i(repo, &pathspec, interactive_opts);

	clear_pathspec(&pathspec);
	ফেরত ret;
}

স্থির পূর্ণসংখ্যা edit_patch(কাঠামো repository *repo,
		      পূর্ণসংখ্যা argc,
		      ধ্রুবক অক্ষর **argv,
		      ধ্রুবক অক্ষর *prefix)
{
	অক্ষর *file = repo_git_path(repo, "ADD_EDIT.patch");
	কাঠামো apply_state state;
	ধ্রুবক অক্ষর *apply_argv[2];
	কাঠামো rev_info rev;
	পূর্ণসংখ্যা out;
	কাঠামো stat st;

	repo_config(repo, git_diff_basic_config, ফাঁকা);

	যদি (repo_read_সূচক(repo) < 0)
		die(_("could not read the index"));

	repo_init_revisions(repo, &rev, prefix);
	rev.diffopt.context = 7;

	argc = setup_revisions(argc, argv, &rev, ফাঁকা);
	rev.diffopt.output_format = DIFF_FORMAT_PATCH;
	rev.diffopt.use_color = GIT_COLOR_NEVER;
	rev.diffopt.flags.ignore_dirty_submodules = 1;
	out = xopen(file, O_CREAT | O_WRONLY | O_TRUNC, 0666);
	rev.diffopt.file = xfdopen(out, "w");
	rev.diffopt.close_file = 1;
	run_diff_files(&rev, 0);

	যদি (launch_editor(file, ফাঁকা, ফাঁকা))
		die(_("editing patch failed"));

	যদি (stat(file, &st))
		die_errno(_("could not stat '%s'"), file);
	যদি (!st.st_আকার)
		die(_("empty patch. aborted"));

	apply_argv[0] = file;
	apply_argv[1] = ফাঁকা;
	যদি (init_apply_state(&state, repo, ফাঁকা))
		die(_("could not initialize apply state"));
	state.cached = 1;
	যদি (check_apply_state(&state, 0))
		die(_("could not check apply state"));
	যদি (apply_all_patches(&state, 1, apply_argv, APPLY_OPT_RECOUNT))
		die(_("could not apply '%s'"), file);
	clear_apply_state(&state);

	unlink(file);
	মুক্ত(file);
	release_revisions(&rev);
	ফেরত 0;
}

স্থির ধ্রুবক অক্ষর ignore_error[] =
N_("The following paths are ignored by one of your .gitignore files:\n");

স্থির পূর্ণসংখ্যা verbose, show_only, ignored_too, refresh_only;
স্থির পূর্ণসংখ্যা ignore_add_errors, intent_to_add, ignore_missing;
স্থির পূর্ণসংখ্যা warn_on_embedded_repo = 1;

#define ADDREMOVE_DEFAULT 1
স্থির পূর্ণসংখ্যা addremove = ADDREMOVE_DEFAULT;
স্থির পূর্ণসংখ্যা addremove_explicit = -1; /* unspecified */

স্থির অক্ষর *chmod_arg;

স্থির পূর্ণসংখ্যা ignore_removal_cb(ধ্রুবক কাঠামো option *opt, ধ্রুবক অক্ষর *arg, পূর্ণসংখ্যা unset)
{
	BUG_ON_OPT_ARG(arg);

	/* if we are told to ignore, we are not adding removals */
	*(পূর্ণসংখ্যা *)opt->মান = !unset ? 0 : 1;
	ফেরত 0;
}

স্থির কাঠামো option builtin_add_options[] = {
	OPT__DRY_RUN(&show_only, N_("dry run")),
	OPT__VERBOSE(&verbose, N_("be verbose")),
	OPT_GROUP(""),
	OPT_BOOL('i', "interactive", &add_interactive, N_("interactive picking")),
	OPT_BOOL('p', "patch", &patch_interactive, N_("select hunks interactively")),
	OPT_BOOL(0, "auto-advance", &interactive_opts.auto_advance,
		 N_("auto advance to the next file when selecting hunks interactively")),
	OPT_DIFF_UNIFIED(&interactive_opts.context),
	OPT_DIFF_INTERHUNK_CONTEXT(&interactive_opts.interhunkcontext),
	OPT_BOOL('e', "edit", &edit_interactive, N_("edit current diff and apply")),
	OPT__FORCE(&ignored_too, N_("allow adding otherwise ignored files"), 0),
	OPT_BOOL('u', "update", &take_worktree_changes, N_("update tracked files")),
	OPT_BOOL(0, "renormalize", &add_renormalize, N_("renormalize EOL of tracked files (implies -u)")),
	OPT_BOOL(0, "resolved", &add_resolved, N_("add conflict-resolved tracked files")),
	OPT_BOOL('N', "intent-to-add", &intent_to_add, N_("record only the fact that the path will be added later")),
	OPT_BOOL('A', "all", &addremove_explicit, N_("add changes from all tracked and untracked files")),
	OPT_CALLBACK_F(0, "ignore-removal", &addremove_explicit,
	  ফাঁকা /* takes no arguments */,
	  N_("ignore paths removed in the working tree (same as --no-all)"),
	  PARSE_OPT_NOARG, ignore_removal_cb),
	OPT_BOOL( 0 , "refresh", &refresh_only, N_("don't add, only refresh the index")),
	OPT_BOOL( 0 , "ignore-errors", &ignore_add_errors, N_("just skip files which cannot be added because of errors")),
	OPT_BOOL( 0 , "ignore-missing", &ignore_missing, N_("check if - even missing - files are ignored in dry run")),
	OPT_BOOL(0, "sparse", &include_sparse, N_("allow updating entries outside of the sparse-checkout cone")),
	OPT_STRING(0, "chmod", &chmod_arg, "(+|-)x",
		   N_("override the executable bit of the listed files")),
	OPT_HIDDEN_BOOL(0, "warn-embedded-repo", &warn_on_embedded_repo,
			N_("warn when adding an embedded repository")),
	OPT_PATHSPEC_FROM_FILE(&pathspec_from_file),
	OPT_PATHSPEC_FILE_NUL(&pathspec_file_nul),
	OPT_END(),
};

স্থির পূর্ণসংখ্যা add_config(ধ্রুবক অক্ষর *var, ধ্রুবক অক্ষর *মান,
		      ধ্রুবক কাঠামো config_context *ctx, শূন্য *cb)
{
	যদি (!strcmp(var, "add.ignoreerrors") ||
	    !strcmp(var, "add.ignore-errors")) {
		ignore_add_errors = git_config_bool(var, মান);
		ফেরত 0;
	}

	যদি (git_color_config(var, মান, cb) < 0)
		ফেরত -1;

	ফেরত git_default_config(var, মান, ctx, cb);
}

স্থির ধ্রুবক অক্ষর embedded_advice[] = N_(
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

স্থির শূন্য check_embedded_repo(ধ্রুবক অক্ষর *path)
{
	কাঠামো strbuf name = STRBUF_INIT;
	স্থির পূর্ণসংখ্যা adviced_on_embedded_repo = 0;

	যদি (!warn_on_embedded_repo)
		ফেরত;
	যদি (!ends_with(path, "/"))
		ফেরত;

	/* Drop trailing slash for aesthetics */
	strbuf_addstr(&name, path);
	strbuf_strip_suffix(&name, "/");

	warning(_("adding embedded git repository: %s"), name.buf);
	যদি (!adviced_on_embedded_repo) {
		advise_if_enabled(ADVICE_ADD_EMBEDDED_REPO,
				  embedded_advice, name.buf, name.buf);
		adviced_on_embedded_repo = 1;
	}

	strbuf_release(&name);
}

স্থির পূর্ণসংখ্যা add_files(কাঠামো repository *repo, কাঠামো dir_struct *dir, পূর্ণসংখ্যা flags)
{
	পূর্ণসংখ্যা i, exit_status = 0;
	কাঠামো string_list matched_sparse_paths = STRING_LIST_INIT_NODUP;

	যদি (dir->ignored_nr) {
		fprintf(stderr, _(ignore_error));
		জন্য (i = 0; i < dir->ignored_nr; i++)
			fprintf(stderr, "%s\n", dir->ignored[i]->name);
		advise_if_enabled(ADVICE_ADD_IGNORED_FILE,
				  _("Use -f if you really want to add them."));
		exit_status = 1;
	}

	জন্য (i = 0; i < dir->nr; i++) {
		যদি (!include_sparse &&
		    !path_in_sparse_checkout(dir->entries[i]->name, repo->সূচক)) {
			string_list_append(&matched_sparse_paths,
					   dir->entries[i]->name);
			চালু;
		}
		যদি (add_file_to_সূচক(repo->সূচক, dir->entries[i]->name, flags)) {
			যদি (!ignore_add_errors)
				die(_("adding files failed"));
			exit_status = 1;
		} অন্যথা {
			check_embedded_repo(dir->entries[i]->name);
		}
	}

	যদি (matched_sparse_paths.nr) {
		advise_on_updating_sparse_paths(&matched_sparse_paths);
		exit_status = 1;
	}

	string_list_clear(&matched_sparse_paths, 0);

	ফেরত exit_status;
}

স্থির পূর্ণসংখ্যা failed_to_add(পূর্ণসংখ্যা flags, ধ্রুবক অক্ষর *path)
{
	যদি (!(flags & ADD_CACHE_IGNORE_ERRORS))
		die(_("updating file '%s' failed"), path);
	ফেরত 1;
}

স্থির পূর্ণসংখ্যা add_resolved_files(কাঠামো repository *repo,
			      ধ্রুবক কাঠামো pathspec *pathspec,
			      পূর্ণসংখ্যা flags)
{
	কাঠামো সূচক_state *istate = repo->সূচক;
	কাঠামো string_list unmerged_paths = STRING_LIST_INIT_DUP;
	কাঠামো string_list unresolved_paths = STRING_LIST_INIT_DUP;
	পূর্ণসংখ্যা exit_status = 0;
	আকার_t i;

	জন্য (i = 0; i < istate->cache_nr; i++) {
		কাঠামো cache_entry *ce = istate->cache[i];
		যদি (!ce_stage(ce))
			চালু;
		যদি (pathspec->nr && !ce_path_match(istate, ce, pathspec, ফাঁকা))
			চালু;
		যদি (!unmerged_paths.nr ||
		    strcmp(unmerged_paths.বস্তুসমূহ[unmerged_paths.nr - 1].string, ce->name))
			string_list_append(&unmerged_paths, ce->name);
	}

	যদি (!unmerged_paths.nr) {
		string_list_clear(&unmerged_paths, 0);
		ফেরত 0;
	}

	জন্য (i = 0; i < unmerged_paths.nr; i++) {
		ধ্রুবক অক্ষর *path = unmerged_paths.বস্তুসমূহ[i].string;
		কাঠামো stat st;

		যদি (!lstat(path, &st) && S_ISREG(st.st_mode)) {
			যদি (has_conflict_markers(istate, path))
				string_list_append(&unresolved_paths, path);
		}
	}

	যদি (unresolved_paths.nr) {
		কাঠামো strbuf sb = STRBUF_INIT;
		জন্য (i = 0; i < unresolved_paths.nr; i++)
			strbuf_addf(&sb, "\t%s\n", unresolved_paths.বস্তুসমূহ[i].string);
		die(_("the following paths still have conflict markers:\n%s"), sb.buf);
	}

	জন্য (i = 0; i < unmerged_paths.nr; i++) {
		ধ্রুবক অক্ষর *path = unmerged_paths.বস্তুসমূহ[i].string;
		কাঠামো stat st;

		যদি (lstat(path, &st)) {
			যদি (errno != ENOENT)
				die_errno(_("cannot lstat: '%s'"), path);
			যদি (remove_file_from_সূচক_with_flags(istate, path, flags))
				exit_status = failed_to_add(flags, path);
		} অন্যথা {
			যদি (add_file_to_সূচক(istate, path, flags))
				exit_status = failed_to_add(flags, path);
		}
	}

	string_list_clear(&unmerged_paths, 0);
	string_list_clear(&unresolved_paths, 0);
	ফেরত exit_status;
}

পূর্ণসংখ্যা cmd_add(পূর্ণসংখ্যা argc,
	    ধ্রুবক অক্ষর **argv,
	    ধ্রুবক অক্ষর *prefix,
	    কাঠামো repository *repo)
{
	পূর্ণসংখ্যা exit_status = 0;
	কাঠামো pathspec pathspec;
	কাঠামো dir_struct dir = DIR_INIT;
	পূর্ণসংখ্যা flags;
	পূর্ণসংখ্যা add_new_files;
	পূর্ণসংখ্যা require_pathspec;
	অক্ষর *seen = ফাঁকা;
	অক্ষর *ps_matched = ফাঁকা;
	কাঠামো lock_file lock_file = LOCK_INIT;
	কাঠামো odb_transaction *transaction = ফাঁকা;

	repo_config(repo, add_config, ফাঁকা);

	argc = parse_options(argc, argv, prefix, builtin_add_options,
			  builtin_add_usage, PARSE_OPT_KEEP_ARGV0);

	prepare_repo_settings(repo);
	repo->settings.command_requires_full_সূচক = 0;

	যদি (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	যদি (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	যদি (patch_interactive)
		add_interactive = 1;
	যদি (add_interactive) {
		যদি (show_only)
			die(_("options '%s' and '%s' cannot be used together"), "--dry-run", "--interactive/--patch");
		যদি (pathspec_from_file)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--interactive/--patch");
		exit(interactive_add(repo, argv + 1, prefix, patch_interactive, &interactive_opts));
	} অন্যথা {
		যদি (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--interactive/--patch");
		যদি (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--interactive/--patch");
		যদি (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--interactive/--patch");
	}

	যদি (edit_interactive) {
		যদি (pathspec_from_file)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--edit");
		ফেরত(edit_patch(repo, argc, argv, prefix));
	}
	argc--;
	argv++;

	যদি (0 <= addremove_explicit)
		addremove = addremove_explicit;
	অন্যথা যদি (take_worktree_changes && ADDREMOVE_DEFAULT)
		addremove = 0; /* "-u" was given but not "-A" */

	die_for_incompatible_opt3(take_worktree_changes, "-u/--update",
				  0 < addremove_explicit, "-A/--all",
				  add_resolved, "--resolved");

	যদি (!show_only && ignore_missing)
		die(_("the option '%s' requires '%s'"), "--ignore-missing", "--dry-run");

	যদি (chmod_arg && ((chmod_arg[0] != '-' && chmod_arg[0] != '+') ||
			  chmod_arg[1] != 'x' || chmod_arg[2]))
		die(_("--chmod param '%s' must be either -x or +x"), chmod_arg);

	add_new_files = !take_worktree_changes && !refresh_only &&
			!add_renormalize && !add_resolved;
	require_pathspec = !(take_worktree_changes ||
			     (0 < addremove_explicit) ||
			     add_resolved);

	repo_hold_locked_সূচক(repo, &lock_file, LOCK_DIE_ON_ERROR);

	/*
	 * Check the "pathspec '%s' did not match any files" block
	 * below before enabling new magic.
	 */
	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_FULL |
		       PATHSPEC_SYMLINK_LEADING_PATH,
		       prefix, argv);

	যদি (pathspec_from_file) {
		যদি (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_FULL |
				    PATHSPEC_SYMLINK_LEADING_PATH,
				    prefix, pathspec_from_file, pathspec_file_nul);
	} অন্যথা যদি (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	যদি (require_pathspec && pathspec.nr == 0) {
		fprintf(stderr, _("Nothing specified, nothing added.\n"));
		advise_if_enabled(ADVICE_ADD_EMPTY_PATHSPEC,
				  _("Maybe you wanted to say 'git add .'?"));
		ফেরত 0;
	}

	যদি (!take_worktree_changes && !add_resolved &&
	    addremove_explicit < 0 && pathspec.nr)
		/* Turn "git add pathspec..." to "git add -A pathspec..." */
		addremove = 1;

	flags = ((verbose ? ADD_CACHE_VERBOSE : 0) |
		 (show_only ? ADD_CACHE_PRETEND : 0) |
		 (intent_to_add ? ADD_CACHE_INTENT : 0) |
		 (ignore_add_errors ? ADD_CACHE_IGNORE_ERRORS : 0) |
		 (!(addremove || take_worktree_changes)
		  ? ADD_CACHE_IGNORE_REMOVAL : 0));

	যদি (repo_read_সূচক_preload(repo, &pathspec, 0) < 0)
		die(_("index file corrupt"));

	die_in_unpopulated_submodule(repo->সূচক, prefix);
	die_path_inside_submodule(repo->সূচক, &pathspec);

	যদি (add_new_files) {
		পূর্ণসংখ্যা baselen;

		/* Set up the default git porcelain excludes */
		যদি (!ignored_too) {
			dir.flags |= DIR_COLLECT_IGNORED;
			setup_standard_excludes(&dir);
		}

		/* This picks up the paths that are not tracked */
		baselen = fill_directory(&dir, repo->সূচক, &pathspec);
		যদি (pathspec.nr)
			seen = prune_directory(repo, &dir, &pathspec, baselen);
	}

	যদি (refresh_only) {
		exit_status |= refresh(repo, verbose, &pathspec);
		যাও finish;
	}

	যদি (pathspec.nr) {
		পূর্ণসংখ্যা i;
		অক্ষর *skip_worktree_seen = ফাঁকা;
		কাঠামো string_list only_match_skip_worktree = STRING_LIST_INIT_NODUP;

		যদি (!seen)
			seen = find_pathspecs_matching_against_সূচক(&pathspec,
					repo->সূচক, PS_IGNORE_SKIP_WORKTREE);

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

		জন্য (i = 0; i < pathspec.nr; i++) {
			ধ্রুবক অক্ষর *path = pathspec.বস্তুসমূহ[i].match;

			যদি (pathspec.বস্তুসমূহ[i].magic & PATHSPEC_EXCLUDE)
				চালু;
			যদি (seen[i])
				চালু;

			যদি (!include_sparse &&
			    matches_skip_worktree(&pathspec, i, &skip_worktree_seen)) {
				string_list_append(&only_match_skip_worktree,
						   pathspec.বস্তুসমূহ[i].original);
				চালু;
			}

			/* Don't complain at 'git add .' on empty repo */
			যদি (!path[0])
				চালু;

			যদি ((pathspec.বস্তুসমূহ[i].magic & (PATHSPEC_GLOB | PATHSPEC_ICASE)) ||
			    !file_exists(path)) {
				যদি (ignore_missing) {
					পূর্ণসংখ্যা dtype = DT_UNKNOWN;
					যদি (is_excluded(&dir, repo->সূচক, path, &dtype))
						dir_add_ignored(&dir, repo->সূচক,
								path, pathspec.বস্তুসমূহ[i].len);
				} অন্যথা
					die(_("pathspec '%s' did not match any files"),
					    pathspec.বস্তুসমূহ[i].original);
			}
		}


		যদি (only_match_skip_worktree.nr) {
			advise_on_updating_sparse_paths(&only_match_skip_worktree);
			exit_status = 1;
		}

		মুক্ত(seen);
		মুক্ত(skip_worktree_seen);
		string_list_clear(&only_match_skip_worktree, 0);
	}

	odb_transaction_begin_or_die(repo->objects, &transaction, 0);

	ps_matched = xcalloc(pathspec.nr, 1);
	যদি (add_resolved)
		exit_status |= add_resolved_files(repo, &pathspec, flags);
	অন্যথা যদি (add_renormalize)
		exit_status |= renormalize_tracked_files(repo, &pathspec, flags);
	অন্যথা
		exit_status |= add_files_to_cache(repo, prefix,
						  &pathspec, ps_matched,
						  include_sparse, flags, ignored_too);

	যদি (take_worktree_changes && !add_renormalize && !ignore_add_errors &&
	    report_path_error(ps_matched, &pathspec))
		exit(128);

	যদি (add_new_files)
		exit_status |= add_files(repo, &dir, flags);

	যদি (chmod_arg && pathspec.nr)
		exit_status |= chmod_pathspec(repo, &pathspec, chmod_arg[0], show_only);
	odb_transaction_commit_and_finalize_or_die(transaction);

finish:
	যদি (write_locked_সূচক(repo->সূচক, &lock_file,
			       COMMIT_LOCK | SKIP_IF_UNCHANGED))
		die(_("unable to write new index file"));

	মুক্ত(ps_matched);
	dir_clear(&dir);
	clear_pathspec(&pathspec);
	ফেরত exit_status;
}
