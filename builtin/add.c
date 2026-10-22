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

ایستا ثابت نویسه * ثابت builtin_add_usage[] = {
	N_("git add [<options>] [--] <pathspec>..."),
	پوچ
};
ایستا صحیح patch_interactive, add_interactive, edit_interactive;
ایستا ساختار interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
ایستا صحیح take_worktree_changes;
ایستا صحیح add_renormalize;
ایستا صحیح add_resolved;
ایستا صحیح pathspec_file_nul;
ایستا صحیح include_sparse;
ایستا ثابت نویسه *pathspec_from_file;

ایستا صحیح chmod_pathspec(ساختار repository *repo,
			  ساختار pathspec *pathspec,
			  نویسه flip,
			  صحیح show_only)
{
	صحیح ret = 0;

	برای (size_t i = 0; i < repo->index->cache_nr; i++) {
		ساختار cache_entry *ce = repo->index->cache[i];
		صحیح err;

		اگر (!include_sparse &&
		    (ce_skip_worktree(ce) ||
		     !path_in_sparse_checkout(ce->name, repo->index)))
			ادامه;

		اگر (pathspec && !ce_path_match(repo->index, ce, pathspec, پوچ))
			ادامه;

		اگر (!show_only)
			err = chmod_index_entry(repo->index, ce, flip);
		وگرنه
			err = S_ISREG(ce->ce_mode) ? 0 : -1;

		اگر (err < 0)
			ret = error(_("cannot chmod %cx '%s'"), flip, ce->name);
	}

	بازگشت ret;
}

ایستا صحیح renormalize_tracked_files(ساختار repository *repo,
				     ثابت ساختار pathspec *pathspec,
				     صحیح flags)
{
	صحیح retval = 0;

	برای (size_t i = 0; i < repo->index->cache_nr; i++) {
		ساختار cache_entry *ce = repo->index->cache[i];

		اگر (!include_sparse &&
		    (ce_skip_worktree(ce) ||
		     !path_in_sparse_checkout(ce->name, repo->index)))
			ادامه;
		اگر (ce_stage(ce))
			ادامه; /* do not touch unmerged paths */
		اگر (!S_ISREG(ce->ce_mode) && !S_ISLNK(ce->ce_mode))
			ادامه; /* do not touch non blobs */
		اگر (pathspec && !ce_path_match(repo->index, ce, pathspec, پوچ))
			ادامه;
		retval |= add_file_to_index(repo->index, ce->name,
					    flags | ADD_CACHE_RENORMALIZE);
	}

	بازگشت retval;
}

ایستا نویسه *prune_directory(ساختار repository *repo,
			     ساختار dir_struct *dir,
			     ساختار pathspec *pathspec,
			     صحیح prefix)
{
	نویسه *seen;
	صحیح i;
	ساختار dir_entry **src, **dst;

	seen = xcalloc(pathspec->nr, 1);

	src = dst = dir->entries;
	i = dir->nr;
	وقتی (--i >= 0) {
		ساختار dir_entry *entry = *src++;
		اگر (dir_path_match(repo->index, entry, pathspec, prefix, seen))
			*dst++ = entry;
	}
	dir->nr = dst - dir->entries;
	add_pathspec_matches_against_index(pathspec, repo->index, seen,
					   PS_IGNORE_SKIP_WORKTREE);
	بازگشت seen;
}

ایستا صحیح refresh(ساختار repository *repo, صحیح verbose, ثابت ساختار pathspec *pathspec)
{
	نویسه *seen;
	صحیح i, ret = 0;
	نویسه *skip_worktree_seen = پوچ;
	ساختار string_list only_match_skip_worktree = STRING_LIST_INIT_NODUP;
	unsigned صحیح flags = REFRESH_IGNORE_SKIP_WORKTREE |
		    (verbose ? REFRESH_IN_PORCELAIN : REFRESH_QUIET);

	seen = xcalloc(pathspec->nr, 1);
	refresh_index(repo->index, flags, pathspec, seen,
		      _("Unstaged changes after refreshing the index:"));
	برای (i = 0; i < pathspec->nr; i++) {
		اگر (!seen[i]) {
			ثابت نویسه *path = pathspec->اقلام[i].original;

			اگر (matches_skip_worktree(pathspec, i, &skip_worktree_seen) ||
			    !path_in_sparse_checkout(path, repo->index)) {
				string_list_append(&only_match_skip_worktree,
						   pathspec->اقلام[i].original);
			} وگرنه {
				die(_("pathspec '%s' did not match any files"),
				    pathspec->اقلام[i].original);
			}
		}
	}

	اگر (only_match_skip_worktree.nr) {
		advise_on_updating_sparse_paths(&only_match_skip_worktree);
		ret = 1;
	}

	آزاد(seen);
	آزاد(skip_worktree_seen);
	string_list_clear(&only_match_skip_worktree, 0);
	بازگشت ret;
}

صحیح interactive_add(ساختار repository *repo,
		    ثابت نویسه **argv,
		    ثابت نویسه *prefix,
		    صحیح patch, ساختار interactive_options *interactive_opts)
{
	ساختار pathspec pathspec;
	صحیح ret;

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_FULL |
		       PATHSPEC_SYMLINK_LEADING_PATH |
		       PATHSPEC_PREFIX_ORIGIN,
		       prefix, argv);

	اگر (patch)
		ret = !!run_add_p(repo, ADD_P_ADD, interactive_opts, پوچ, &pathspec, 0);
	وگرنه
		ret = !!run_add_i(repo, &pathspec, interactive_opts);

	clear_pathspec(&pathspec);
	بازگشت ret;
}

ایستا صحیح edit_patch(ساختار repository *repo,
		      صحیح argc,
		      ثابت نویسه **argv,
		      ثابت نویسه *prefix)
{
	نویسه *file = repo_git_path(repo, "ADD_EDIT.patch");
	ساختار apply_state state;
	ثابت نویسه *apply_argv[2];
	ساختار rev_info rev;
	صحیح out;
	ساختار stat st;

	repo_config(repo, git_diff_basic_config, پوچ);

	اگر (repo_read_index(repo) < 0)
		die(_("could not read the index"));

	repo_init_revisions(repo, &rev, prefix);
	rev.diffopt.context = 7;

	argc = setup_revisions(argc, argv, &rev, پوچ);
	rev.diffopt.output_format = DIFF_FORMAT_PATCH;
	rev.diffopt.use_color = GIT_COLOR_NEVER;
	rev.diffopt.flags.ignore_dirty_submodules = 1;
	out = xopen(file, O_CREAT | O_WRONLY | O_TRUNC, 0666);
	rev.diffopt.file = xfdopen(out, "w");
	rev.diffopt.close_file = 1;
	run_diff_files(&rev, 0);

	اگر (launch_editor(file, پوچ, پوچ))
		die(_("editing patch failed"));

	اگر (stat(file, &st))
		die_errno(_("could not stat '%s'"), file);
	اگر (!st.st_size)
		die(_("empty patch. aborted"));

	apply_argv[0] = file;
	apply_argv[1] = پوچ;
	اگر (init_apply_state(&state, repo, پوچ))
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
	بازگشت 0;
}

ایستا ثابت نویسه ignore_error[] =
N_("The following paths are ignored by one of your .gitignore files:\n");

ایستا صحیح verbose, show_only, ignored_too, refresh_only;
ایستا صحیح ignore_add_errors, intent_to_add, ignore_missing;
ایستا صحیح warn_on_embedded_repo = 1;

#define ADDREMOVE_DEFAULT 1
ایستا صحیح addremove = ADDREMOVE_DEFAULT;
ایستا صحیح addremove_explicit = -1; /* unspecified */

ایستا نویسه *chmod_arg;

ایستا صحیح ignore_removal_cb(ثابت ساختار option *opt, ثابت نویسه *arg, صحیح unset)
{
	BUG_ON_OPT_ARG(arg);

	/* if we are told to ignore, we are not adding removals */
	*(صحیح *)opt->مقدار = !unset ? 0 : 1;
	بازگشت 0;
}

ایستا ساختار option builtin_add_options[] = {
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
	  پوچ /* takes no arguments */,
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

ایستا صحیح add_config(ثابت نویسه *var, ثابت نویسه *مقدار,
		      ثابت ساختار config_context *ctx, تهی *cb)
{
	اگر (!strcmp(var, "add.ignoreerrors") ||
	    !strcmp(var, "add.ignore-errors")) {
		ignore_add_errors = git_config_bool(var, مقدار);
		بازگشت 0;
	}

	اگر (git_color_config(var, مقدار, cb) < 0)
		بازگشت -1;

	بازگشت git_default_config(var, مقدار, ctx, cb);
}

ایستا ثابت نویسه embedded_advice[] = N_(
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

ایستا تهی check_embedded_repo(ثابت نویسه *path)
{
	ساختار strbuf name = STRBUF_INIT;
	ایستا صحیح adviced_on_embedded_repo = 0;

	اگر (!warn_on_embedded_repo)
		بازگشت;
	اگر (!ends_with(path, "/"))
		بازگشت;

	/* Drop trailing slash for aesthetics */
	strbuf_addstr(&name, path);
	strbuf_strip_suffix(&name, "/");

	warning(_("adding embedded git repository: %s"), name.buf);
	اگر (!adviced_on_embedded_repo) {
		advise_if_enabled(ADVICE_ADD_EMBEDDED_REPO,
				  embedded_advice, name.buf, name.buf);
		adviced_on_embedded_repo = 1;
	}

	strbuf_release(&name);
}

ایستا صحیح add_files(ساختار repository *repo, ساختار dir_struct *dir, صحیح flags)
{
	صحیح i, exit_status = 0;
	ساختار string_list matched_sparse_paths = STRING_LIST_INIT_NODUP;

	اگر (dir->ignored_nr) {
		fprintf(stderr, _(ignore_error));
		برای (i = 0; i < dir->ignored_nr; i++)
			fprintf(stderr, "%s\n", dir->ignored[i]->name);
		advise_if_enabled(ADVICE_ADD_IGNORED_FILE,
				  _("Use -f if you really want to add them."));
		exit_status = 1;
	}

	برای (i = 0; i < dir->nr; i++) {
		اگر (!include_sparse &&
		    !path_in_sparse_checkout(dir->entries[i]->name, repo->index)) {
			string_list_append(&matched_sparse_paths,
					   dir->entries[i]->name);
			ادامه;
		}
		اگر (add_file_to_index(repo->index, dir->entries[i]->name, flags)) {
			اگر (!ignore_add_errors)
				die(_("adding files failed"));
			exit_status = 1;
		} وگرنه {
			check_embedded_repo(dir->entries[i]->name);
		}
	}

	اگر (matched_sparse_paths.nr) {
		advise_on_updating_sparse_paths(&matched_sparse_paths);
		exit_status = 1;
	}

	string_list_clear(&matched_sparse_paths, 0);

	بازگشت exit_status;
}

ایستا صحیح failed_to_add(صحیح flags, ثابت نویسه *path)
{
	اگر (!(flags & ADD_CACHE_IGNORE_ERRORS))
		die(_("updating file '%s' failed"), path);
	بازگشت 1;
}

ایستا صحیح add_resolved_files(ساختار repository *repo,
			      ثابت ساختار pathspec *pathspec,
			      صحیح flags)
{
	ساختار index_state *istate = repo->index;
	ساختار string_list unmerged_paths = STRING_LIST_INIT_DUP;
	ساختار string_list unresolved_paths = STRING_LIST_INIT_DUP;
	صحیح exit_status = 0;
	size_t i;

	برای (i = 0; i < istate->cache_nr; i++) {
		ساختار cache_entry *ce = istate->cache[i];
		اگر (!ce_stage(ce))
			ادامه;
		اگر (pathspec->nr && !ce_path_match(istate, ce, pathspec, پوچ))
			ادامه;
		اگر (!unmerged_paths.nr ||
		    strcmp(unmerged_paths.اقلام[unmerged_paths.nr - 1].string, ce->name))
			string_list_append(&unmerged_paths, ce->name);
	}

	اگر (!unmerged_paths.nr) {
		string_list_clear(&unmerged_paths, 0);
		بازگشت 0;
	}

	برای (i = 0; i < unmerged_paths.nr; i++) {
		ثابت نویسه *path = unmerged_paths.اقلام[i].string;
		ساختار stat st;

		اگر (!lstat(path, &st) && S_ISREG(st.st_mode)) {
			اگر (has_conflict_markers(istate, path))
				string_list_append(&unresolved_paths, path);
		}
	}

	اگر (unresolved_paths.nr) {
		ساختار strbuf sb = STRBUF_INIT;
		برای (i = 0; i < unresolved_paths.nr; i++)
			strbuf_addf(&sb, "\t%s\n", unresolved_paths.اقلام[i].string);
		die(_("the following paths still have conflict markers:\n%s"), sb.buf);
	}

	برای (i = 0; i < unmerged_paths.nr; i++) {
		ثابت نویسه *path = unmerged_paths.اقلام[i].string;
		ساختار stat st;

		اگر (lstat(path, &st)) {
			اگر (errno != ENOENT)
				die_errno(_("cannot lstat: '%s'"), path);
			اگر (remove_file_from_index_with_flags(istate, path, flags))
				exit_status = failed_to_add(flags, path);
		} وگرنه {
			اگر (add_file_to_index(istate, path, flags))
				exit_status = failed_to_add(flags, path);
		}
	}

	string_list_clear(&unmerged_paths, 0);
	string_list_clear(&unresolved_paths, 0);
	بازگشت exit_status;
}

صحیح cmd_add(صحیح argc,
	    ثابت نویسه **argv,
	    ثابت نویسه *prefix,
	    ساختار repository *repo)
{
	صحیح exit_status = 0;
	ساختار pathspec pathspec;
	ساختار dir_struct dir = DIR_INIT;
	صحیح flags;
	صحیح add_new_files;
	صحیح require_pathspec;
	نویسه *seen = پوچ;
	نویسه *ps_matched = پوچ;
	ساختار lock_file lock_file = LOCK_INIT;
	ساختار odb_transaction *transaction = پوچ;

	repo_config(repo, add_config, پوچ);

	argc = parse_options(argc, argv, prefix, builtin_add_options,
			  builtin_add_usage, PARSE_OPT_KEEP_ARGV0);

	prepare_repo_settings(repo);
	repo->settings.command_requires_full_index = 0;

	اگر (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	اگر (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	اگر (patch_interactive)
		add_interactive = 1;
	اگر (add_interactive) {
		اگر (show_only)
			die(_("options '%s' and '%s' cannot be used together"), "--dry-run", "--interactive/--patch");
		اگر (pathspec_from_file)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--interactive/--patch");
		exit(interactive_add(repo, argv + 1, prefix, patch_interactive, &interactive_opts));
	} وگرنه {
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
		بازگشت(edit_patch(repo, argc, argv, prefix));
	}
	argc--;
	argv++;

	اگر (0 <= addremove_explicit)
		addremove = addremove_explicit;
	وگرنه اگر (take_worktree_changes && ADDREMOVE_DEFAULT)
		addremove = 0; /* "-u" was given but not "-A" */

	die_for_incompatible_opt3(take_worktree_changes, "-u/--update",
				  0 < addremove_explicit, "-A/--all",
				  add_resolved, "--resolved");

	اگر (!show_only && ignore_missing)
		die(_("the option '%s' requires '%s'"), "--ignore-missing", "--dry-run");

	اگر (chmod_arg && ((chmod_arg[0] != '-' && chmod_arg[0] != '+') ||
			  chmod_arg[1] != 'x' || chmod_arg[2]))
		die(_("--chmod param '%s' must be either -x or +x"), chmod_arg);

	add_new_files = !take_worktree_changes && !refresh_only &&
			!add_renormalize && !add_resolved;
	require_pathspec = !(take_worktree_changes ||
			     (0 < addremove_explicit) ||
			     add_resolved);

	repo_hold_locked_index(repo, &lock_file, LOCK_DIE_ON_ERROR);

	/*
	 * Check the "pathspec '%s' did not match any files" block
	 * below before enabling new magic.
	 */
	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_FULL |
		       PATHSPEC_SYMLINK_LEADING_PATH,
		       prefix, argv);

	اگر (pathspec_from_file) {
		اگر (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_FULL |
				    PATHSPEC_SYMLINK_LEADING_PATH,
				    prefix, pathspec_from_file, pathspec_file_nul);
	} وگرنه اگر (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	اگر (require_pathspec && pathspec.nr == 0) {
		fprintf(stderr, _("Nothing specified, nothing added.\n"));
		advise_if_enabled(ADVICE_ADD_EMPTY_PATHSPEC,
				  _("Maybe you wanted to say 'git add .'?"));
		بازگشت 0;
	}

	اگر (!take_worktree_changes && !add_resolved &&
	    addremove_explicit < 0 && pathspec.nr)
		/* Turn "git add pathspec..." to "git add -A pathspec..." */
		addremove = 1;

	flags = ((verbose ? ADD_CACHE_VERBOSE : 0) |
		 (show_only ? ADD_CACHE_PRETEND : 0) |
		 (intent_to_add ? ADD_CACHE_INTENT : 0) |
		 (ignore_add_errors ? ADD_CACHE_IGNORE_ERRORS : 0) |
		 (!(addremove || take_worktree_changes)
		  ? ADD_CACHE_IGNORE_REMOVAL : 0));

	اگر (repo_read_index_preload(repo, &pathspec, 0) < 0)
		die(_("index file corrupt"));

	die_in_unpopulated_submodule(repo->index, prefix);
	die_path_inside_submodule(repo->index, &pathspec);

	اگر (add_new_files) {
		صحیح baselen;

		/* Set up the default git porcelain excludes */
		اگر (!ignored_too) {
			dir.flags |= DIR_COLLECT_IGNORED;
			setup_standard_excludes(&dir);
		}

		/* This picks up the paths that are not tracked */
		baselen = fill_directory(&dir, repo->index, &pathspec);
		اگر (pathspec.nr)
			seen = prune_directory(repo, &dir, &pathspec, baselen);
	}

	اگر (refresh_only) {
		exit_status |= refresh(repo, verbose, &pathspec);
		برو finish;
	}

	اگر (pathspec.nr) {
		صحیح i;
		نویسه *skip_worktree_seen = پوچ;
		ساختار string_list only_match_skip_worktree = STRING_LIST_INIT_NODUP;

		اگر (!seen)
			seen = find_pathspecs_matching_against_index(&pathspec,
					repo->index, PS_IGNORE_SKIP_WORKTREE);

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

		برای (i = 0; i < pathspec.nr; i++) {
			ثابت نویسه *path = pathspec.اقلام[i].match;

			اگر (pathspec.اقلام[i].magic & PATHSPEC_EXCLUDE)
				ادامه;
			اگر (seen[i])
				ادامه;

			اگر (!include_sparse &&
			    matches_skip_worktree(&pathspec, i, &skip_worktree_seen)) {
				string_list_append(&only_match_skip_worktree,
						   pathspec.اقلام[i].original);
				ادامه;
			}

			/* Don't complain at 'git add .' on empty repo */
			اگر (!path[0])
				ادامه;

			اگر ((pathspec.اقلام[i].magic & (PATHSPEC_GLOB | PATHSPEC_ICASE)) ||
			    !file_exists(path)) {
				اگر (ignore_missing) {
					صحیح dtype = DT_UNKNOWN;
					اگر (is_excluded(&dir, repo->index, path, &dtype))
						dir_add_ignored(&dir, repo->index,
								path, pathspec.اقلام[i].len);
				} وگرنه
					die(_("pathspec '%s' did not match any files"),
					    pathspec.اقلام[i].original);
			}
		}


		اگر (only_match_skip_worktree.nr) {
			advise_on_updating_sparse_paths(&only_match_skip_worktree);
			exit_status = 1;
		}

		آزاد(seen);
		آزاد(skip_worktree_seen);
		string_list_clear(&only_match_skip_worktree, 0);
	}

	odb_transaction_begin_or_die(repo->objects, &transaction, 0);

	ps_matched = xcalloc(pathspec.nr, 1);
	اگر (add_resolved)
		exit_status |= add_resolved_files(repo, &pathspec, flags);
	وگرنه اگر (add_renormalize)
		exit_status |= renormalize_tracked_files(repo, &pathspec, flags);
	وگرنه
		exit_status |= add_files_to_cache(repo, prefix,
						  &pathspec, ps_matched,
						  include_sparse, flags, ignored_too);

	اگر (take_worktree_changes && !add_renormalize && !ignore_add_errors &&
	    report_path_error(ps_matched, &pathspec))
		exit(128);

	اگر (add_new_files)
		exit_status |= add_files(repo, &dir, flags);

	اگر (chmod_arg && pathspec.nr)
		exit_status |= chmod_pathspec(repo, &pathspec, chmod_arg[0], show_only);
	odb_transaction_commit_and_finalize_or_die(transaction);

finish:
	اگر (write_locked_index(repo->index, &lock_file,
			       COMMIT_LOCK | SKIP_IF_UNCHANGED))
		die(_("unable to write new index file"));

	آزاد(ps_matched);
	dir_clear(&dir);
	clear_pathspec(&pathspec);
	بازگشت exit_status;
}
