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

static const char * const builtin_add_usage[] = {
	N_("git add [<options>] [--] <pathspec>..."),
	NULL
};
static 整数 patch_interactive, add_interactive, edit_interactive;
static struct interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
static 整数 take_worktree_changes;
static 整数 add_renormalize;
static 整数 add_resolved;
static 整数 pathspec_file_nul;
static 整数 include_sparse;
static const char *pathspec_from_file;

static 整数 chmod_pathspec(struct repository *repo,
			  struct pathspec *pathspec,
			  char flip,
			  整数 show_only)
{
	整数 ret = 0;

	ために (size_t i = 0; i < repo->index->cache_nr; i++) {
		struct cache_entry *ce = repo->index->cache[i];
		整数 err;

		もし (!include_sparse &&
		    (ce_skip_worktree(ce) ||
		     !path_in_sparse_checkout(ce->name, repo->index)))
			continue;

		もし (pathspec && !ce_path_match(repo->index, ce, pathspec, NULL))
			continue;

		もし (!show_only)
			err = chmod_index_entry(repo->index, ce, flip);
		否则
			err = S_ISREG(ce->ce_mode) ? 0 : -1;

		もし (err < 0)
			ret = error(_("cannot chmod %cx '%s'"), flip, ce->name);
	}

	戻る ret;
}

static 整数 renormalize_tracked_files(struct repository *repo,
				     const struct pathspec *pathspec,
				     整数 flags)
{
	整数 retval = 0;

	ために (size_t i = 0; i < repo->index->cache_nr; i++) {
		struct cache_entry *ce = repo->index->cache[i];

		もし (!include_sparse &&
		    (ce_skip_worktree(ce) ||
		     !path_in_sparse_checkout(ce->name, repo->index)))
			continue;
		もし (ce_stage(ce))
			continue; /* do not touch unmerged paths */
		もし (!S_ISREG(ce->ce_mode) && !S_ISLNK(ce->ce_mode))
			continue; /* do not touch non blobs */
		もし (pathspec && !ce_path_match(repo->index, ce, pathspec, NULL))
			continue;
		retval |= add_file_to_index(repo->index, ce->name,
					    flags | ADD_CACHE_RENORMALIZE);
	}

	戻る retval;
}

static char *prune_directory(struct repository *repo,
			     struct dir_struct *dir,
			     struct pathspec *pathspec,
			     整数 prefix)
{
	char *seen;
	整数 i;
	struct dir_entry **src, **dst;

	seen = xcalloc(pathspec->nr, 1);

	src = dst = dir->entries;
	i = dir->nr;
	ながら (--i >= 0) {
		struct dir_entry *entry = *src++;
		もし (dir_path_match(repo->index, entry, pathspec, prefix, seen))
			*dst++ = entry;
	}
	dir->nr = dst - dir->entries;
	add_pathspec_matches_against_index(pathspec, repo->index, seen,
					   PS_IGNORE_SKIP_WORKTREE);
	戻る seen;
}

static 整数 refresh(struct repository *repo, 整数 verbose, const struct pathspec *pathspec)
{
	char *seen;
	整数 i, ret = 0;
	char *skip_worktree_seen = NULL;
	struct string_list only_match_skip_worktree = STRING_LIST_INIT_NODUP;
	unsigned 整数 flags = REFRESH_IGNORE_SKIP_WORKTREE |
		    (verbose ? REFRESH_IN_PORCELAIN : REFRESH_QUIET);

	seen = xcalloc(pathspec->nr, 1);
	refresh_index(repo->index, flags, pathspec, seen,
		      _("Unstaged changes after refreshing the index:"));
	ために (i = 0; i < pathspec->nr; i++) {
		もし (!seen[i]) {
			const char *path = pathspec->items[i].original;

			もし (matches_skip_worktree(pathspec, i, &skip_worktree_seen) ||
			    !path_in_sparse_checkout(path, repo->index)) {
				string_list_append(&only_match_skip_worktree,
						   pathspec->items[i].original);
			} 否则 {
				die(_("pathspec '%s' did not match any files"),
				    pathspec->items[i].original);
			}
		}
	}

	もし (only_match_skip_worktree.nr) {
		advise_on_updating_sparse_paths(&only_match_skip_worktree);
		ret = 1;
	}

	free(seen);
	free(skip_worktree_seen);
	string_list_clear(&only_match_skip_worktree, 0);
	戻る ret;
}

整数 interactive_add(struct repository *repo,
		    const char **argv,
		    const char *prefix,
		    整数 patch, struct interactive_options *interactive_opts)
{
	struct pathspec pathspec;
	整数 ret;

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_FULL |
		       PATHSPEC_SYMLINK_LEADING_PATH |
		       PATHSPEC_PREFIX_ORIGIN,
		       prefix, argv);

	もし (patch)
		ret = !!run_add_p(repo, ADD_P_ADD, interactive_opts, NULL, &pathspec, 0);
	否则
		ret = !!run_add_i(repo, &pathspec, interactive_opts);

	clear_pathspec(&pathspec);
	戻る ret;
}

static 整数 edit_patch(struct repository *repo,
		      整数 argc,
		      const char **argv,
		      const char *prefix)
{
	char *file = repo_git_path(repo, "ADD_EDIT.patch");
	struct apply_state state;
	const char *apply_argv[2];
	struct rev_info rev;
	整数 out;
	struct stat st;

	repo_config(repo, git_diff_basic_config, NULL);

	もし (repo_read_index(repo) < 0)
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

	もし (launch_editor(file, NULL, NULL))
		die(_("editing patch failed"));

	もし (stat(file, &st))
		die_errno(_("could not stat '%s'"), file);
	もし (!st.st_size)
		die(_("empty patch. aborted"));

	apply_argv[0] = file;
	apply_argv[1] = NULL;
	もし (init_apply_state(&state, repo, NULL))
		die(_("could not initialize apply state"));
	state.cached = 1;
	もし (check_apply_state(&state, 0))
		die(_("could not check apply state"));
	もし (apply_all_patches(&state, 1, apply_argv, APPLY_OPT_RECOUNT))
		die(_("could not apply '%s'"), file);
	clear_apply_state(&state);

	unlink(file);
	free(file);
	release_revisions(&rev);
	戻る 0;
}

static const char ignore_error[] =
N_("The following paths are ignored by one of your .gitignore files:\n");

static 整数 verbose, show_only, ignored_too, refresh_only;
static 整数 ignore_add_errors, intent_to_add, ignore_missing;
static 整数 warn_on_embedded_repo = 1;

#define ADDREMOVE_DEFAULT 1
static 整数 addremove = ADDREMOVE_DEFAULT;
static 整数 addremove_explicit = -1; /* unspecified */

static char *chmod_arg;

static 整数 ignore_removal_cb(const struct option *opt, const char *arg, 整数 unset)
{
	BUG_ON_OPT_ARG(arg);

	/* if we are told to ignore, we are not adding removals */
	*(整数 *)opt->value = !unset ? 0 : 1;
	戻る 0;
}

static struct option builtin_add_options[] = {
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

static 整数 add_config(const char *var, const char *value,
		      const struct config_context *ctx, void *cb)
{
	もし (!strcmp(var, "add.ignoreerrors") ||
	    !strcmp(var, "add.ignore-errors")) {
		ignore_add_errors = git_config_bool(var, value);
		戻る 0;
	}

	もし (git_color_config(var, value, cb) < 0)
		戻る -1;

	戻る git_default_config(var, value, ctx, cb);
}

static const char embedded_advice[] = N_(
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

static void check_embedded_repo(const char *path)
{
	struct strbuf name = STRBUF_INIT;
	static 整数 adviced_on_embedded_repo = 0;

	もし (!warn_on_embedded_repo)
		戻る;
	もし (!ends_with(path, "/"))
		戻る;

	/* Drop trailing slash for aesthetics */
	strbuf_addstr(&name, path);
	strbuf_strip_suffix(&name, "/");

	warning(_("adding embedded git repository: %s"), name.buf);
	もし (!adviced_on_embedded_repo) {
		advise_if_enabled(ADVICE_ADD_EMBEDDED_REPO,
				  embedded_advice, name.buf, name.buf);
		adviced_on_embedded_repo = 1;
	}

	strbuf_release(&name);
}

static 整数 add_files(struct repository *repo, struct dir_struct *dir, 整数 flags)
{
	整数 i, exit_status = 0;
	struct string_list matched_sparse_paths = STRING_LIST_INIT_NODUP;

	もし (dir->ignored_nr) {
		fprintf(stderr, _(ignore_error));
		ために (i = 0; i < dir->ignored_nr; i++)
			fprintf(stderr, "%s\n", dir->ignored[i]->name);
		advise_if_enabled(ADVICE_ADD_IGNORED_FILE,
				  _("Use -f if you really want to add them."));
		exit_status = 1;
	}

	ために (i = 0; i < dir->nr; i++) {
		もし (!include_sparse &&
		    !path_in_sparse_checkout(dir->entries[i]->name, repo->index)) {
			string_list_append(&matched_sparse_paths,
					   dir->entries[i]->name);
			continue;
		}
		もし (add_file_to_index(repo->index, dir->entries[i]->name, flags)) {
			もし (!ignore_add_errors)
				die(_("adding files failed"));
			exit_status = 1;
		} 否则 {
			check_embedded_repo(dir->entries[i]->name);
		}
	}

	もし (matched_sparse_paths.nr) {
		advise_on_updating_sparse_paths(&matched_sparse_paths);
		exit_status = 1;
	}

	string_list_clear(&matched_sparse_paths, 0);

	戻る exit_status;
}

static 整数 failed_to_add(整数 flags, const char *path)
{
	もし (!(flags & ADD_CACHE_IGNORE_ERRORS))
		die(_("updating file '%s' failed"), path);
	戻る 1;
}

static 整数 add_resolved_files(struct repository *repo,
			      const struct pathspec *pathspec,
			      整数 flags)
{
	struct index_state *istate = repo->index;
	struct string_list unmerged_paths = STRING_LIST_INIT_DUP;
	struct string_list unresolved_paths = STRING_LIST_INIT_DUP;
	整数 exit_status = 0;
	size_t i;

	ために (i = 0; i < istate->cache_nr; i++) {
		struct cache_entry *ce = istate->cache[i];
		もし (!ce_stage(ce))
			continue;
		もし (pathspec->nr && !ce_path_match(istate, ce, pathspec, NULL))
			continue;
		もし (!unmerged_paths.nr ||
		    strcmp(unmerged_paths.items[unmerged_paths.nr - 1].string, ce->name))
			string_list_append(&unmerged_paths, ce->name);
	}

	もし (!unmerged_paths.nr) {
		string_list_clear(&unmerged_paths, 0);
		戻る 0;
	}

	ために (i = 0; i < unmerged_paths.nr; i++) {
		const char *path = unmerged_paths.items[i].string;
		struct stat st;

		もし (!lstat(path, &st) && S_ISREG(st.st_mode)) {
			もし (has_conflict_markers(istate, path))
				string_list_append(&unresolved_paths, path);
		}
	}

	もし (unresolved_paths.nr) {
		struct strbuf sb = STRBUF_INIT;
		ために (i = 0; i < unresolved_paths.nr; i++)
			strbuf_addf(&sb, "\t%s\n", unresolved_paths.items[i].string);
		die(_("the following paths still have conflict markers:\n%s"), sb.buf);
	}

	ために (i = 0; i < unmerged_paths.nr; i++) {
		const char *path = unmerged_paths.items[i].string;
		struct stat st;

		もし (lstat(path, &st)) {
			もし (errno != ENOENT)
				die_errno(_("cannot lstat: '%s'"), path);
			もし (remove_file_from_index_with_flags(istate, path, flags))
				exit_status = failed_to_add(flags, path);
		} 否则 {
			もし (add_file_to_index(istate, path, flags))
				exit_status = failed_to_add(flags, path);
		}
	}

	string_list_clear(&unmerged_paths, 0);
	string_list_clear(&unresolved_paths, 0);
	戻る exit_status;
}

整数 cmd_add(整数 argc,
	    const char **argv,
	    const char *prefix,
	    struct repository *repo)
{
	整数 exit_status = 0;
	struct pathspec pathspec;
	struct dir_struct dir = DIR_INIT;
	整数 flags;
	整数 add_new_files;
	整数 require_pathspec;
	char *seen = NULL;
	char *ps_matched = NULL;
	struct lock_file lock_file = LOCK_INIT;
	struct odb_transaction *transaction = NULL;

	repo_config(repo, add_config, NULL);

	argc = parse_options(argc, argv, prefix, builtin_add_options,
			  builtin_add_usage, PARSE_OPT_KEEP_ARGV0);

	prepare_repo_settings(repo);
	repo->settings.command_requires_full_index = 0;

	もし (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	もし (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	もし (patch_interactive)
		add_interactive = 1;
	もし (add_interactive) {
		もし (show_only)
			die(_("options '%s' and '%s' cannot be used together"), "--dry-run", "--interactive/--patch");
		もし (pathspec_from_file)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--interactive/--patch");
		exit(interactive_add(repo, argv + 1, prefix, patch_interactive, &interactive_opts));
	} 否则 {
		もし (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--interactive/--patch");
		もし (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--interactive/--patch");
		もし (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--interactive/--patch");
	}

	もし (edit_interactive) {
		もし (pathspec_from_file)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--edit");
		戻る(edit_patch(repo, argc, argv, prefix));
	}
	argc--;
	argv++;

	もし (0 <= addremove_explicit)
		addremove = addremove_explicit;
	否则 もし (take_worktree_changes && ADDREMOVE_DEFAULT)
		addremove = 0; /* "-u" was given but not "-A" */

	die_for_incompatible_opt3(take_worktree_changes, "-u/--update",
				  0 < addremove_explicit, "-A/--all",
				  add_resolved, "--resolved");

	もし (!show_only && ignore_missing)
		die(_("the option '%s' requires '%s'"), "--ignore-missing", "--dry-run");

	もし (chmod_arg && ((chmod_arg[0] != '-' && chmod_arg[0] != '+') ||
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

	もし (pathspec_from_file) {
		もし (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_FULL |
				    PATHSPEC_SYMLINK_LEADING_PATH,
				    prefix, pathspec_from_file, pathspec_file_nul);
	} 否则 もし (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	もし (require_pathspec && pathspec.nr == 0) {
		fprintf(stderr, _("Nothing specified, nothing added.\n"));
		advise_if_enabled(ADVICE_ADD_EMPTY_PATHSPEC,
				  _("Maybe you wanted to say 'git add .'?"));
		戻る 0;
	}

	もし (!take_worktree_changes && !add_resolved &&
	    addremove_explicit < 0 && pathspec.nr)
		/* Turn "git add pathspec..." to "git add -A pathspec..." */
		addremove = 1;

	flags = ((verbose ? ADD_CACHE_VERBOSE : 0) |
		 (show_only ? ADD_CACHE_PRETEND : 0) |
		 (intent_to_add ? ADD_CACHE_INTENT : 0) |
		 (ignore_add_errors ? ADD_CACHE_IGNORE_ERRORS : 0) |
		 (!(addremove || take_worktree_changes)
		  ? ADD_CACHE_IGNORE_REMOVAL : 0));

	もし (repo_read_index_preload(repo, &pathspec, 0) < 0)
		die(_("index file corrupt"));

	die_in_unpopulated_submodule(repo->index, prefix);
	die_path_inside_submodule(repo->index, &pathspec);

	もし (add_new_files) {
		整数 baselen;

		/* Set up the default git porcelain excludes */
		もし (!ignored_too) {
			dir.flags |= DIR_COLLECT_IGNORED;
			setup_standard_excludes(&dir);
		}

		/* This picks up the paths that are not tracked */
		baselen = fill_directory(&dir, repo->index, &pathspec);
		もし (pathspec.nr)
			seen = prune_directory(repo, &dir, &pathspec, baselen);
	}

	もし (refresh_only) {
		exit_status |= refresh(repo, verbose, &pathspec);
		goto finish;
	}

	もし (pathspec.nr) {
		整数 i;
		char *skip_worktree_seen = NULL;
		struct string_list only_match_skip_worktree = STRING_LIST_INIT_NODUP;

		もし (!seen)
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

		ために (i = 0; i < pathspec.nr; i++) {
			const char *path = pathspec.items[i].match;

			もし (pathspec.items[i].magic & PATHSPEC_EXCLUDE)
				continue;
			もし (seen[i])
				continue;

			もし (!include_sparse &&
			    matches_skip_worktree(&pathspec, i, &skip_worktree_seen)) {
				string_list_append(&only_match_skip_worktree,
						   pathspec.items[i].original);
				continue;
			}

			/* Don't complain at 'git add .' on empty repo */
			もし (!path[0])
				continue;

			もし ((pathspec.items[i].magic & (PATHSPEC_GLOB | PATHSPEC_ICASE)) ||
			    !file_exists(path)) {
				もし (ignore_missing) {
					整数 dtype = DT_UNKNOWN;
					もし (is_excluded(&dir, repo->index, path, &dtype))
						dir_add_ignored(&dir, repo->index,
								path, pathspec.items[i].len);
				} 否则
					die(_("pathspec '%s' did not match any files"),
					    pathspec.items[i].original);
			}
		}


		もし (only_match_skip_worktree.nr) {
			advise_on_updating_sparse_paths(&only_match_skip_worktree);
			exit_status = 1;
		}

		free(seen);
		free(skip_worktree_seen);
		string_list_clear(&only_match_skip_worktree, 0);
	}

	odb_transaction_begin_or_die(repo->objects, &transaction, 0);

	ps_matched = xcalloc(pathspec.nr, 1);
	もし (add_resolved)
		exit_status |= add_resolved_files(repo, &pathspec, flags);
	否则 もし (add_renormalize)
		exit_status |= renormalize_tracked_files(repo, &pathspec, flags);
	否则
		exit_status |= add_files_to_cache(repo, prefix,
						  &pathspec, ps_matched,
						  include_sparse, flags, ignored_too);

	もし (take_worktree_changes && !add_renormalize && !ignore_add_errors &&
	    report_path_error(ps_matched, &pathspec))
		exit(128);

	もし (add_new_files)
		exit_status |= add_files(repo, &dir, flags);

	もし (chmod_arg && pathspec.nr)
		exit_status |= chmod_pathspec(repo, &pathspec, chmod_arg[0], show_only);
	odb_transaction_commit_and_finalize_or_die(transaction);

finish:
	もし (write_locked_index(repo->index, &lock_file,
			       COMMIT_LOCK | SKIP_IF_UNCHANGED))
		die(_("unable to write new index file"));

	free(ps_matched);
	dir_clear(&dir);
	clear_pathspec(&pathspec);
	戻る exit_status;
}
