/*
 * "git add" builtin command
 *
 * Copyright (C) 2006 Linus Torvalds
 */

#include "builtin.h"
#include "advice.h"
#include "config.h"
#include "environment.h"
#include "lockfile.h"
#include "editor.h"
#include "dir.h"
#include "gettext.h"
#include "pathspec.h"
#include "object-file.h"
#include "odb.h"
#include "odb/transaction.h"
#include "parse-options.h"
#include "path.h"
#include "preload-index.h"
#include "diff.h"
#include "read-cache.h"
#include "revision.h"
#include "submodule.h"
#include "add-interactive.h"
#include "merge-ll.h"
#include "apply.h"

static const var_na * const builtin_add_usage[] = {
	N_("git add [<options>] [--] <pathspec>..."),
	NULL
};
static ចំនួនគត់ patch_interactive, add_interactive, edit_interactive;
static sa_nracanaa interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
static ចំនួនគត់ take_worktree_changes;
static ចំនួនគត់ add_renormalize;
static ចំនួនគត់ add_resolved;
static ចំនួនគត់ pathspec_file_nul;
static ចំនួនគត់ include_sparse;
static const var_na *pathspec_from_file;

static ចំនួនគត់ chmod_pathspec(sa_nracanaa repository *repo,
			  sa_nracanaa pathspec *pathspec,
			  var_na flip,
			  ចំនួនគត់ show_only)
{
	ចំនួនគត់ ret = 0;

	សម្រាប់ (size_t i = 0; i < repo->index->cache_nr; i++) {
		sa_nracanaa cache_entry *ce = repo->index->cache[i];
		ចំនួនគត់ err;

		ប្រសិនបើ (!include_sparse &&
		    (ce_skip_worktree(ce) ||
		     !path_in_sparse_checkout(ce->name, repo->index)))
			continue;

		ប្រសិនបើ (pathspec && !ce_path_match(repo->index, ce, pathspec, NULL))
			continue;

		ប្រសិនបើ (!show_only)
			err = chmod_index_entry(repo->index, ce, flip);
		បើមិនដូច្នេះ
			err = S_ISREG(ce->ce_mode) ? 0 : -1;

		ប្រសិនបើ (err < 0)
			ret = error(_("cannot chmod %cx '%s'"), flip, ce->name);
	}

	ត្រឡប់ ret;
}

static ចំនួនគត់ renormalize_tracked_files(sa_nracanaa repository *repo,
				     const sa_nracanaa pathspec *pathspec,
				     ចំនួនគត់ flags)
{
	ចំនួនគត់ retval = 0;

	សម្រាប់ (size_t i = 0; i < repo->index->cache_nr; i++) {
		sa_nracanaa cache_entry *ce = repo->index->cache[i];

		ប្រសិនបើ (!include_sparse &&
		    (ce_skip_worktree(ce) ||
		     !path_in_sparse_checkout(ce->name, repo->index)))
			continue;
		ប្រសិនបើ (ce_stage(ce))
			continue; /* do not touch unmerged paths */
		ប្រសិនបើ (!S_ISREG(ce->ce_mode) && !S_ISLNK(ce->ce_mode))
			continue; /* do not touch non blobs */
		ប្រសិនបើ (pathspec && !ce_path_match(repo->index, ce, pathspec, NULL))
			continue;
		retval |= add_file_to_index(repo->index, ce->name,
					    flags | ADD_CACHE_RENORMALIZE);
	}

	ត្រឡប់ retval;
}

static var_na *prune_directory(sa_nracanaa repository *repo,
			     sa_nracanaa dir_struct *dir,
			     sa_nracanaa pathspec *pathspec,
			     ចំនួនគត់ prefix)
{
	var_na *seen;
	ចំនួនគត់ i;
	sa_nracanaa dir_entry **src, **dst;

	seen = xcalloc(pathspec->nr, 1);

	src = dst = dir->entries;
	i = dir->nr;
	ខណៈពេល (--i >= 0) {
		sa_nracanaa dir_entry *entry = *src++;
		ប្រសិនបើ (dir_path_match(repo->index, entry, pathspec, prefix, seen))
			*dst++ = entry;
	}
	dir->nr = dst - dir->entries;
	add_pathspec_matches_against_index(pathspec, repo->index, seen,
					   PS_IGNORE_SKIP_WORKTREE);
	ត្រឡប់ seen;
}

static ចំនួនគត់ refresh(sa_nracanaa repository *repo, ចំនួនគត់ verbose, const sa_nracanaa pathspec *pathspec)
{
	var_na *seen;
	ចំនួនគត់ i, ret = 0;
	var_na *skip_worktree_seen = NULL;
	sa_nracanaa string_list only_match_skip_worktree = STRING_LIST_INIT_NODUP;
	unsigned ចំនួនគត់ flags = REFRESH_IGNORE_SKIP_WORKTREE |
		    (verbose ? REFRESH_IN_PORCELAIN : REFRESH_QUIET);

	seen = xcalloc(pathspec->nr, 1);
	refresh_index(repo->index, flags, pathspec, seen,
		      _("Unstaged changes after refreshing the index:"));
	សម្រាប់ (i = 0; i < pathspec->nr; i++) {
		ប្រសិនបើ (!seen[i]) {
			const var_na *path = pathspec->items[i].original;

			ប្រសិនបើ (matches_skip_worktree(pathspec, i, &skip_worktree_seen) ||
			    !path_in_sparse_checkout(path, repo->index)) {
				string_list_append(&only_match_skip_worktree,
						   pathspec->items[i].original);
			} បើមិនដូច្នេះ {
				die(_("pathspec '%s' did not match any files"),
				    pathspec->items[i].original);
			}
		}
	}

	ប្រសិនបើ (only_match_skip_worktree.nr) {
		advise_on_updating_sparse_paths(&only_match_skip_worktree);
		ret = 1;
	}

	free(seen);
	free(skip_worktree_seen);
	string_list_clear(&only_match_skip_worktree, 0);
	ត្រឡប់ ret;
}

ចំនួនគត់ interactive_add(sa_nracanaa repository *repo,
		    const var_na **argv,
		    const var_na *prefix,
		    ចំនួនគត់ patch, sa_nracanaa interactive_options *interactive_opts)
{
	sa_nracanaa pathspec pathspec;
	ចំនួនគត់ ret;

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_FULL |
		       PATHSPEC_SYMLINK_LEADING_PATH |
		       PATHSPEC_PREFIX_ORIGIN,
		       prefix, argv);

	ប្រសិនបើ (patch)
		ret = !!run_add_p(repo, ADD_P_ADD, interactive_opts, NULL, &pathspec, 0);
	បើមិនដូច្នេះ
		ret = !!run_add_i(repo, &pathspec, interactive_opts);

	clear_pathspec(&pathspec);
	ត្រឡប់ ret;
}

static ចំនួនគត់ edit_patch(sa_nracanaa repository *repo,
		      ចំនួនគត់ argc,
		      const var_na **argv,
		      const var_na *prefix)
{
	var_na *file = repo_git_path(repo, "ADD_EDIT.patch");
	sa_nracanaa apply_state state;
	const var_na *apply_argv[2];
	sa_nracanaa rev_info rev;
	ចំនួនគត់ out;
	sa_nracanaa stat st;

	repo_config(repo, git_diff_basic_config, NULL);

	ប្រសិនបើ (repo_read_index(repo) < 0)
		die(_("could not read the index"));

	repo_init_revisions(repo, &rev, prefix);
	rev.diffopt.context = 7;

	argc = setup_revisions(argc, argv, &rev, NULL);
	rev.diffopt.output_format = DIFF_FORMAT_PATCH;
	rev.diffopt.use_color = GIT_COLOR_NEVER;
	rev.diffopt.flags.ignore_dirty_submodules = 1;
	out = xopen(file, O_CREAT | O_WRONLY | O_TRUNC, 0666);
	rev.diffopt.file = xfdopen(out, "w");
	rev.diffopt.close_file = 1;
	run_diff_files(&rev, 0);

	ប្រសិនបើ (launch_editor(file, NULL, NULL))
		die(_("editing patch failed"));

	ប្រសិនបើ (stat(file, &st))
		die_errno(_("could not stat '%s'"), file);
	ប្រសិនបើ (!st.st_size)
		die(_("empty patch. aborted"));

	apply_argv[0] = file;
	apply_argv[1] = NULL;
	ប្រសិនបើ (init_apply_state(&state, repo, NULL))
		die(_("could not initialize apply state"));
	state.cached = 1;
	ប្រសិនបើ (check_apply_state(&state, 0))
		die(_("could not check apply state"));
	ប្រសិនបើ (apply_all_patches(&state, 1, apply_argv, APPLY_OPT_RECOUNT))
		die(_("could not apply '%s'"), file);
	clear_apply_state(&state);

	unlink(file);
	free(file);
	release_revisions(&rev);
	ត្រឡប់ 0;
}

static const var_na ignore_error[] =
N_("The following paths are ignored by one of your .gitignore files:\n");

static ចំនួនគត់ verbose, show_only, ignored_too, refresh_only;
static ចំនួនគត់ ignore_add_errors, intent_to_add, ignore_missing;
static ចំនួនគត់ warn_on_embedded_repo = 1;

#define ADDREMOVE_DEFAULT 1
static ចំនួនគត់ addremove = ADDREMOVE_DEFAULT;
static ចំនួនគត់ addremove_explicit = -1; /* unspecified */

static var_na *chmod_arg;

static ចំនួនគត់ ignore_removal_cb(const sa_nracanaa option *opt, const var_na *arg, ចំនួនគត់ unset)
{
	BUG_ON_OPT_ARG(arg);

	/* if we are told to ignore, we are not adding removals */
	*(ចំនួនគត់ *)opt->value = !unset ? 0 : 1;
	ត្រឡប់ 0;
}

static sa_nracanaa option builtin_add_options[] = {
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
	  NULL /* takes no arguments */,
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

static ចំនួនគត់ add_config(const var_na *var, const var_na *value,
		      const sa_nracanaa config_context *ctx, void *cb)
{
	ប្រសិនបើ (!strcmp(var, "add.ignoreerrors") ||
	    !strcmp(var, "add.ignore-errors")) {
		ignore_add_errors = git_config_bool(var, value);
		ត្រឡប់ 0;
	}

	ប្រសិនបើ (git_color_config(var, value, cb) < 0)
		ត្រឡប់ -1;

	ត្រឡប់ git_default_config(var, value, ctx, cb);
}

static const var_na embedded_advice[] = N_(
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

static void check_embedded_repo(const var_na *path)
{
	sa_nracanaa strbuf name = STRBUF_INIT;
	static ចំនួនគត់ adviced_on_embedded_repo = 0;

	ប្រសិនបើ (!warn_on_embedded_repo)
		ត្រឡប់;
	ប្រសិនបើ (!ends_with(path, "/"))
		ត្រឡប់;

	/* Drop trailing slash for aesthetics */
	strbuf_addstr(&name, path);
	strbuf_strip_suffix(&name, "/");

	warning(_("adding embedded git repository: %s"), name.buf);
	ប្រសិនបើ (!adviced_on_embedded_repo) {
		advise_if_enabled(ADVICE_ADD_EMBEDDED_REPO,
				  embedded_advice, name.buf, name.buf);
		adviced_on_embedded_repo = 1;
	}

	strbuf_release(&name);
}

static ចំនួនគត់ add_files(sa_nracanaa repository *repo, sa_nracanaa dir_struct *dir, ចំនួនគត់ flags)
{
	ចំនួនគត់ i, exit_status = 0;
	sa_nracanaa string_list matched_sparse_paths = STRING_LIST_INIT_NODUP;

	ប្រសិនបើ (dir->ignored_nr) {
		fprintf(stderr, _(ignore_error));
		សម្រាប់ (i = 0; i < dir->ignored_nr; i++)
			fprintf(stderr, "%s\n", dir->ignored[i]->name);
		advise_if_enabled(ADVICE_ADD_IGNORED_FILE,
				  _("Use -f if you really want to add them."));
		exit_status = 1;
	}

	សម្រាប់ (i = 0; i < dir->nr; i++) {
		ប្រសិនបើ (!include_sparse &&
		    !path_in_sparse_checkout(dir->entries[i]->name, repo->index)) {
			string_list_append(&matched_sparse_paths,
					   dir->entries[i]->name);
			continue;
		}
		ប្រសិនបើ (add_file_to_index(repo->index, dir->entries[i]->name, flags)) {
			ប្រសិនបើ (!ignore_add_errors)
				die(_("adding files failed"));
			exit_status = 1;
		} បើមិនដូច្នេះ {
			check_embedded_repo(dir->entries[i]->name);
		}
	}

	ប្រសិនបើ (matched_sparse_paths.nr) {
		advise_on_updating_sparse_paths(&matched_sparse_paths);
		exit_status = 1;
	}

	string_list_clear(&matched_sparse_paths, 0);

	ត្រឡប់ exit_status;
}

static ចំនួនគត់ failed_to_add(ចំនួនគត់ flags, const var_na *path)
{
	ប្រសិនបើ (!(flags & ADD_CACHE_IGNORE_ERRORS))
		die(_("updating file '%s' failed"), path);
	ត្រឡប់ 1;
}

static ចំនួនគត់ add_resolved_files(sa_nracanaa repository *repo,
			      const sa_nracanaa pathspec *pathspec,
			      ចំនួនគត់ flags)
{
	sa_nracanaa index_state *istate = repo->index;
	sa_nracanaa string_list unmerged_paths = STRING_LIST_INIT_DUP;
	sa_nracanaa string_list unresolved_paths = STRING_LIST_INIT_DUP;
	ចំនួនគត់ exit_status = 0;
	size_t i;

	សម្រាប់ (i = 0; i < istate->cache_nr; i++) {
		sa_nracanaa cache_entry *ce = istate->cache[i];
		ប្រសិនបើ (!ce_stage(ce))
			continue;
		ប្រសិនបើ (pathspec->nr && !ce_path_match(istate, ce, pathspec, NULL))
			continue;
		ប្រសិនបើ (!unmerged_paths.nr ||
		    strcmp(unmerged_paths.items[unmerged_paths.nr - 1].string, ce->name))
			string_list_append(&unmerged_paths, ce->name);
	}

	ប្រសិនបើ (!unmerged_paths.nr) {
		string_list_clear(&unmerged_paths, 0);
		ត្រឡប់ 0;
	}

	សម្រាប់ (i = 0; i < unmerged_paths.nr; i++) {
		const var_na *path = unmerged_paths.items[i].string;
		sa_nracanaa stat st;

		ប្រសិនបើ (!lstat(path, &st) && S_ISREG(st.st_mode)) {
			ប្រសិនបើ (has_conflict_markers(istate, path))
				string_list_append(&unresolved_paths, path);
		}
	}

	ប្រសិនបើ (unresolved_paths.nr) {
		sa_nracanaa strbuf sb = STRBUF_INIT;
		សម្រាប់ (i = 0; i < unresolved_paths.nr; i++)
			strbuf_addf(&sb, "\t%s\n", unresolved_paths.items[i].string);
		die(_("the following paths still have conflict markers:\n%s"), sb.buf);
	}

	សម្រាប់ (i = 0; i < unmerged_paths.nr; i++) {
		const var_na *path = unmerged_paths.items[i].string;
		sa_nracanaa stat st;

		ប្រសិនបើ (lstat(path, &st)) {
			ប្រសិនបើ (errno != ENOENT)
				die_errno(_("cannot lstat: '%s'"), path);
			ប្រសិនបើ (remove_file_from_index_with_flags(istate, path, flags))
				exit_status = failed_to_add(flags, path);
		} បើមិនដូច្នេះ {
			ប្រសិនបើ (add_file_to_index(istate, path, flags))
				exit_status = failed_to_add(flags, path);
		}
	}

	string_list_clear(&unmerged_paths, 0);
	string_list_clear(&unresolved_paths, 0);
	ត្រឡប់ exit_status;
}

ចំនួនគត់ cmd_add(ចំនួនគត់ argc,
	    const var_na **argv,
	    const var_na *prefix,
	    sa_nracanaa repository *repo)
{
	ចំនួនគត់ exit_status = 0;
	sa_nracanaa pathspec pathspec;
	sa_nracanaa dir_struct dir = DIR_INIT;
	ចំនួនគត់ flags;
	ចំនួនគត់ add_new_files;
	ចំនួនគត់ require_pathspec;
	var_na *seen = NULL;
	var_na *ps_matched = NULL;
	sa_nracanaa lock_file lock_file = LOCK_INIT;
	sa_nracanaa odb_transaction *transaction = NULL;

	repo_config(repo, add_config, NULL);

	argc = parse_options(argc, argv, prefix, builtin_add_options,
			  builtin_add_usage, PARSE_OPT_KEEP_ARGV0);

	prepare_repo_settings(repo);
	repo->settings.command_requires_full_index = 0;

	ប្រសិនបើ (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	ប្រសិនបើ (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	ប្រសិនបើ (patch_interactive)
		add_interactive = 1;
	ប្រសិនបើ (add_interactive) {
		ប្រសិនបើ (show_only)
			die(_("options '%s' and '%s' cannot be used together"), "--dry-run", "--interactive/--patch");
		ប្រសិនបើ (pathspec_from_file)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--interactive/--patch");
		exit(interactive_add(repo, argv + 1, prefix, patch_interactive, &interactive_opts));
	} បើមិនដូច្នេះ {
		ប្រសិនបើ (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--interactive/--patch");
		ប្រសិនបើ (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--interactive/--patch");
		ប្រសិនបើ (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--interactive/--patch");
	}

	ប្រសិនបើ (edit_interactive) {
		ប្រសិនបើ (pathspec_from_file)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--edit");
		ត្រឡប់(edit_patch(repo, argc, argv, prefix));
	}
	argc--;
	argv++;

	ប្រសិនបើ (0 <= addremove_explicit)
		addremove = addremove_explicit;
	បើមិនដូច្នេះ ប្រសិនបើ (take_worktree_changes && ADDREMOVE_DEFAULT)
		addremove = 0; /* "-u" was given but not "-A" */

	die_for_incompatible_opt3(take_worktree_changes, "-u/--update",
				  0 < addremove_explicit, "-A/--all",
				  add_resolved, "--resolved");

	ប្រសិនបើ (!show_only && ignore_missing)
		die(_("the option '%s' requires '%s'"), "--ignore-missing", "--dry-run");

	ប្រសិនបើ (chmod_arg && ((chmod_arg[0] != '-' && chmod_arg[0] != '+') ||
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

	ប្រសិនបើ (pathspec_from_file) {
		ប្រសិនបើ (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_FULL |
				    PATHSPEC_SYMLINK_LEADING_PATH,
				    prefix, pathspec_from_file, pathspec_file_nul);
	} បើមិនដូច្នេះ ប្រសិនបើ (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	ប្រសិនបើ (require_pathspec && pathspec.nr == 0) {
		fprintf(stderr, _("Nothing specified, nothing added.\n"));
		advise_if_enabled(ADVICE_ADD_EMPTY_PATHSPEC,
				  _("Maybe you wanted to say 'git add .'?"));
		ត្រឡប់ 0;
	}

	ប្រសិនបើ (!take_worktree_changes && !add_resolved &&
	    addremove_explicit < 0 && pathspec.nr)
		/* Turn "git add pathspec..." to "git add -A pathspec..." */
		addremove = 1;

	flags = ((verbose ? ADD_CACHE_VERBOSE : 0) |
		 (show_only ? ADD_CACHE_PRETEND : 0) |
		 (intent_to_add ? ADD_CACHE_INTENT : 0) |
		 (ignore_add_errors ? ADD_CACHE_IGNORE_ERRORS : 0) |
		 (!(addremove || take_worktree_changes)
		  ? ADD_CACHE_IGNORE_REMOVAL : 0));

	ប្រសិនបើ (repo_read_index_preload(repo, &pathspec, 0) < 0)
		die(_("index file corrupt"));

	die_in_unpopulated_submodule(repo->index, prefix);
	die_path_inside_submodule(repo->index, &pathspec);

	ប្រសិនបើ (add_new_files) {
		ចំនួនគត់ baselen;

		/* Set up the default git porcelain excludes */
		ប្រសិនបើ (!ignored_too) {
			dir.flags |= DIR_COLLECT_IGNORED;
			setup_standard_excludes(&dir);
		}

		/* This picks up the paths that are not tracked */
		baselen = fill_directory(&dir, repo->index, &pathspec);
		ប្រសិនបើ (pathspec.nr)
			seen = prune_directory(repo, &dir, &pathspec, baselen);
	}

	ប្រសិនបើ (refresh_only) {
		exit_status |= refresh(repo, verbose, &pathspec);
		goto finish;
	}

	ប្រសិនបើ (pathspec.nr) {
		ចំនួនគត់ i;
		var_na *skip_worktree_seen = NULL;
		sa_nracanaa string_list only_match_skip_worktree = STRING_LIST_INIT_NODUP;

		ប្រសិនបើ (!seen)
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

		សម្រាប់ (i = 0; i < pathspec.nr; i++) {
			const var_na *path = pathspec.items[i].match;

			ប្រសិនបើ (pathspec.items[i].magic & PATHSPEC_EXCLUDE)
				continue;
			ប្រសិនបើ (seen[i])
				continue;

			ប្រសិនបើ (!include_sparse &&
			    matches_skip_worktree(&pathspec, i, &skip_worktree_seen)) {
				string_list_append(&only_match_skip_worktree,
						   pathspec.items[i].original);
				continue;
			}

			/* Don't complain at 'git add .' on empty repo */
			ប្រសិនបើ (!path[0])
				continue;

			ប្រសិនបើ ((pathspec.items[i].magic & (PATHSPEC_GLOB | PATHSPEC_ICASE)) ||
			    !file_exists(path)) {
				ប្រសិនបើ (ignore_missing) {
					ចំនួនគត់ dtype = DT_UNKNOWN;
					ប្រសិនបើ (is_excluded(&dir, repo->index, path, &dtype))
						dir_add_ignored(&dir, repo->index,
								path, pathspec.items[i].len);
				} បើមិនដូច្នេះ
					die(_("pathspec '%s' did not match any files"),
					    pathspec.items[i].original);
			}
		}


		ប្រសិនបើ (only_match_skip_worktree.nr) {
			advise_on_updating_sparse_paths(&only_match_skip_worktree);
			exit_status = 1;
		}

		free(seen);
		free(skip_worktree_seen);
		string_list_clear(&only_match_skip_worktree, 0);
	}

	odb_transaction_begin_or_die(repo->objects, &transaction, 0);

	ps_matched = xcalloc(pathspec.nr, 1);
	ប្រសិនបើ (add_resolved)
		exit_status |= add_resolved_files(repo, &pathspec, flags);
	បើមិនដូច្នេះ ប្រសិនបើ (add_renormalize)
		exit_status |= renormalize_tracked_files(repo, &pathspec, flags);
	បើមិនដូច្នេះ
		exit_status |= add_files_to_cache(repo, prefix,
						  &pathspec, ps_matched,
						  include_sparse, flags, ignored_too);

	ប្រសិនបើ (take_worktree_changes && !add_renormalize && !ignore_add_errors &&
	    report_path_error(ps_matched, &pathspec))
		exit(128);

	ប្រសិនបើ (add_new_files)
		exit_status |= add_files(repo, &dir, flags);

	ប្រសិនបើ (chmod_arg && pathspec.nr)
		exit_status |= chmod_pathspec(repo, &pathspec, chmod_arg[0], show_only);
	odb_transaction_commit_and_finalize_or_die(transaction);

finish:
	ប្រសិនបើ (write_locked_index(repo->index, &lock_file,
			       COMMIT_LOCK | SKIP_IF_UNCHANGED))
		die(_("unable to write new index file"));

	free(ps_matched);
	dir_clear(&dir);
	clear_pathspec(&pathspec);
	ត្រឡប់ exit_status;
}
