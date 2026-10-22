/*
 * "git add" builtin command
 *
 * Copyright (C) 2006 Linus Torvalds
 */

#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "advice.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "lockfile.h"
#અન્તર્ભાવ "editor.h"
#અન્તર્ભાવ "dir.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "pathspec.h"
#અન્તર્ભાવ "object-file.h"
#અન્તર્ભાવ "odb.h"
#અન્તર્ભાવ "odb/transaction.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "path.h"
#અન્તર્ભાવ "preload-index.h"
#અન્તર્ભાવ "diff.h"
#અન્તર્ભાવ "read-cache.h"
#અન્તર્ભાવ "revision.h"
#અન્તર્ભાવ "submodule.h"
#અન્તર્ભાવ "add-interactive.h"
#અન્તર્ભાવ "merge-ll.h"
#અન્તર્ભાવ "apply.h"

સ્થિર અચલ અક્ષર * અચલ builtin_add_usage[] = {
	N_("git add [<options>] [--] <pathspec>..."),
	ખાલી
};
સ્થિર પૂર્ણાંક patch_interactive, add_interactive, edit_interactive;
સ્થિર સંરચના interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
સ્થિર પૂર્ણાંક take_worktree_changes;
સ્થિર પૂર્ણાંક add_renormalize;
સ્થિર પૂર્ણાંક add_resolved;
સ્થિર પૂર્ણાંક pathspec_file_nul;
સ્થિર પૂર્ણાંક include_sparse;
સ્થિર અચલ અક્ષર *pathspec_from_file;

સ્થિર પૂર્ણાંક chmod_pathspec(સંરચના repository *repo,
			  સંરચના pathspec *pathspec,
			  અક્ષર flip,
			  પૂર્ણાંક show_only)
{
	પૂર્ણાંક ret = 0;

	માટે (size_t i = 0; i < repo->index->cache_nr; i++) {
		સંરચના cache_entry *ce = repo->index->cache[i];
		પૂર્ણાંક err;

		જો (!include_sparse &&
		    (ce_skip_worktree(ce) ||
		     !path_in_sparse_checkout(ce->name, repo->index)))
			ચાલુ;

		જો (pathspec && !ce_path_match(repo->index, ce, pathspec, ખાલી))
			ચાલુ;

		જો (!show_only)
			err = chmod_index_entry(repo->index, ce, flip);
		નહીંતર
			err = S_ISREG(ce->ce_mode) ? 0 : -1;

		જો (err < 0)
			ret = error(_("cannot chmod %cx '%s'"), flip, ce->name);
	}

	પાછા ret;
}

સ્થિર પૂર્ણાંક renormalize_tracked_files(સંરચના repository *repo,
				     અચલ સંરચના pathspec *pathspec,
				     પૂર્ણાંક flags)
{
	પૂર્ણાંક retval = 0;

	માટે (size_t i = 0; i < repo->index->cache_nr; i++) {
		સંરચના cache_entry *ce = repo->index->cache[i];

		જો (!include_sparse &&
		    (ce_skip_worktree(ce) ||
		     !path_in_sparse_checkout(ce->name, repo->index)))
			ચાલુ;
		જો (ce_stage(ce))
			ચાલુ; /* do not touch unmerged paths */
		જો (!S_ISREG(ce->ce_mode) && !S_ISLNK(ce->ce_mode))
			ચાલુ; /* do not touch non blobs */
		જો (pathspec && !ce_path_match(repo->index, ce, pathspec, ખાલી))
			ચાલુ;
		retval |= add_file_to_index(repo->index, ce->name,
					    flags | ADD_CACHE_RENORMALIZE);
	}

	પાછા retval;
}

સ્થિર અક્ષર *prune_directory(સંરચના repository *repo,
			     સંરચના dir_struct *dir,
			     સંરચના pathspec *pathspec,
			     પૂર્ણાંક prefix)
{
	અક્ષર *seen;
	પૂર્ણાંક i;
	સંરચના dir_entry **src, **dst;

	seen = xcalloc(pathspec->nr, 1);

	src = dst = dir->entries;
	i = dir->nr;
	જ્યારે (--i >= 0) {
		સંરચના dir_entry *entry = *src++;
		જો (dir_path_match(repo->index, entry, pathspec, prefix, seen))
			*dst++ = entry;
	}
	dir->nr = dst - dir->entries;
	add_pathspec_matches_against_index(pathspec, repo->index, seen,
					   PS_IGNORE_SKIP_WORKTREE);
	પાછા seen;
}

સ્થિર પૂર્ણાંક refresh(સંરચના repository *repo, પૂર્ણાંક verbose, અચલ સંરચના pathspec *pathspec)
{
	અક્ષર *seen;
	પૂર્ણાંક i, ret = 0;
	અક્ષર *skip_worktree_seen = ખાલી;
	સંરચના string_list only_match_skip_worktree = STRING_LIST_INIT_NODUP;
	unsigned પૂર્ણાંક flags = REFRESH_IGNORE_SKIP_WORKTREE |
		    (verbose ? REFRESH_IN_PORCELAIN : REFRESH_QUIET);

	seen = xcalloc(pathspec->nr, 1);
	refresh_index(repo->index, flags, pathspec, seen,
		      _("Unstaged changes after refreshing the index:"));
	માટે (i = 0; i < pathspec->nr; i++) {
		જો (!seen[i]) {
			અચલ અક્ષર *path = pathspec->વસ્તુઓ[i].original;

			જો (matches_skip_worktree(pathspec, i, &skip_worktree_seen) ||
			    !path_in_sparse_checkout(path, repo->index)) {
				string_list_append(&only_match_skip_worktree,
						   pathspec->વસ્તુઓ[i].original);
			} નહીંતર {
				die(_("pathspec '%s' did not match any files"),
				    pathspec->વસ્તુઓ[i].original);
			}
		}
	}

	જો (only_match_skip_worktree.nr) {
		advise_on_updating_sparse_paths(&only_match_skip_worktree);
		ret = 1;
	}

	મુક્ત(seen);
	મુક્ત(skip_worktree_seen);
	string_list_clear(&only_match_skip_worktree, 0);
	પાછા ret;
}

પૂર્ણાંક interactive_add(સંરચના repository *repo,
		    અચલ અક્ષર **argv,
		    અચલ અક્ષર *prefix,
		    પૂર્ણાંક patch, સંરચના interactive_options *interactive_opts)
{
	સંરચના pathspec pathspec;
	પૂર્ણાંક ret;

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_FULL |
		       PATHSPEC_SYMLINK_LEADING_PATH |
		       PATHSPEC_PREFIX_ORIGIN,
		       prefix, argv);

	જો (patch)
		ret = !!run_add_p(repo, ADD_P_ADD, interactive_opts, ખાલી, &pathspec, 0);
	નહીંતર
		ret = !!run_add_i(repo, &pathspec, interactive_opts);

	clear_pathspec(&pathspec);
	પાછા ret;
}

સ્થિર પૂર્ણાંક edit_patch(સંરચના repository *repo,
		      પૂર્ણાંક argc,
		      અચલ અક્ષર **argv,
		      અચલ અક્ષર *prefix)
{
	અક્ષર *file = repo_git_path(repo, "ADD_EDIT.patch");
	સંરચના apply_state state;
	અચલ અક્ષર *apply_argv[2];
	સંરચના rev_info rev;
	પૂર્ણાંક out;
	સંરચના stat st;

	repo_config(repo, git_diff_basic_config, ખાલી);

	જો (repo_read_index(repo) < 0)
		die(_("could not read the index"));

	repo_init_revisions(repo, &rev, prefix);
	rev.diffopt.context = 7;

	argc = setup_revisions(argc, argv, &rev, ખાલી);
	rev.diffopt.output_format = DIFF_FORMAT_PATCH;
	rev.diffopt.use_color = GIT_COLOR_NEVER;
	rev.diffopt.flags.ignore_dirty_submodules = 1;
	out = xopen(file, O_CREAT | O_WRONLY | O_TRUNC, 0666);
	rev.diffopt.file = xfdopen(out, "w");
	rev.diffopt.close_file = 1;
	run_diff_files(&rev, 0);

	જો (launch_editor(file, ખાલી, ખાલી))
		die(_("editing patch failed"));

	જો (stat(file, &st))
		die_errno(_("could not stat '%s'"), file);
	જો (!st.st_size)
		die(_("empty patch. aborted"));

	apply_argv[0] = file;
	apply_argv[1] = ખાલી;
	જો (init_apply_state(&state, repo, ખાલી))
		die(_("could not initialize apply state"));
	state.cached = 1;
	જો (check_apply_state(&state, 0))
		die(_("could not check apply state"));
	જો (apply_all_patches(&state, 1, apply_argv, APPLY_OPT_RECOUNT))
		die(_("could not apply '%s'"), file);
	clear_apply_state(&state);

	unlink(file);
	મુક્ત(file);
	release_revisions(&rev);
	પાછા 0;
}

સ્થિર અચલ અક્ષર ignore_error[] =
N_("The following paths are ignored by one of your .gitignore files:\n");

સ્થિર પૂર્ણાંક verbose, show_only, ignored_too, refresh_only;
સ્થિર પૂર્ણાંક ignore_add_errors, intent_to_add, ignore_missing;
સ્થિર પૂર્ણાંક warn_on_embedded_repo = 1;

#define ADDREMOVE_DEFAULT 1
સ્થિર પૂર્ણાંક addremove = ADDREMOVE_DEFAULT;
સ્થિર પૂર્ણાંક addremove_explicit = -1; /* unspecified */

સ્થિર અક્ષર *chmod_arg;

સ્થિર પૂર્ણાંક ignore_removal_cb(અચલ સંરચના option *opt, અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	BUG_ON_OPT_ARG(arg);

	/* if we are told to ignore, we are not adding removals */
	*(પૂર્ણાંક *)opt->કિંમત = !unset ? 0 : 1;
	પાછા 0;
}

સ્થિર સંરચના option builtin_add_options[] = {
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
	  ખાલી /* takes no arguments */,
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

સ્થિર પૂર્ણાંક add_config(અચલ અક્ષર *var, અચલ અક્ષર *કિંમત,
		      અચલ સંરચના config_context *ctx, શૂન્ય *cb)
{
	જો (!strcmp(var, "add.ignoreerrors") ||
	    !strcmp(var, "add.ignore-errors")) {
		ignore_add_errors = git_config_bool(var, કિંમત);
		પાછા 0;
	}

	જો (git_color_config(var, કિંમત, cb) < 0)
		પાછા -1;

	પાછા git_default_config(var, કિંમત, ctx, cb);
}

સ્થિર અચલ અક્ષર embedded_advice[] = N_(
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

સ્થિર શૂન્ય check_embedded_repo(અચલ અક્ષર *path)
{
	સંરચના strbuf name = STRBUF_INIT;
	સ્થિર પૂર્ણાંક adviced_on_embedded_repo = 0;

	જો (!warn_on_embedded_repo)
		પાછા;
	જો (!ends_with(path, "/"))
		પાછા;

	/* Drop trailing slash for aesthetics */
	strbuf_addstr(&name, path);
	strbuf_strip_suffix(&name, "/");

	warning(_("adding embedded git repository: %s"), name.buf);
	જો (!adviced_on_embedded_repo) {
		advise_if_enabled(ADVICE_ADD_EMBEDDED_REPO,
				  embedded_advice, name.buf, name.buf);
		adviced_on_embedded_repo = 1;
	}

	strbuf_release(&name);
}

સ્થિર પૂર્ણાંક add_files(સંરચના repository *repo, સંરચના dir_struct *dir, પૂર્ણાંક flags)
{
	પૂર્ણાંક i, exit_status = 0;
	સંરચના string_list matched_sparse_paths = STRING_LIST_INIT_NODUP;

	જો (dir->ignored_nr) {
		fprintf(stderr, _(ignore_error));
		માટે (i = 0; i < dir->ignored_nr; i++)
			fprintf(stderr, "%s\n", dir->ignored[i]->name);
		advise_if_enabled(ADVICE_ADD_IGNORED_FILE,
				  _("Use -f if you really want to add them."));
		exit_status = 1;
	}

	માટે (i = 0; i < dir->nr; i++) {
		જો (!include_sparse &&
		    !path_in_sparse_checkout(dir->entries[i]->name, repo->index)) {
			string_list_append(&matched_sparse_paths,
					   dir->entries[i]->name);
			ચાલુ;
		}
		જો (add_file_to_index(repo->index, dir->entries[i]->name, flags)) {
			જો (!ignore_add_errors)
				die(_("adding files failed"));
			exit_status = 1;
		} નહીંતર {
			check_embedded_repo(dir->entries[i]->name);
		}
	}

	જો (matched_sparse_paths.nr) {
		advise_on_updating_sparse_paths(&matched_sparse_paths);
		exit_status = 1;
	}

	string_list_clear(&matched_sparse_paths, 0);

	પાછા exit_status;
}

સ્થિર પૂર્ણાંક failed_to_add(પૂર્ણાંક flags, અચલ અક્ષર *path)
{
	જો (!(flags & ADD_CACHE_IGNORE_ERRORS))
		die(_("updating file '%s' failed"), path);
	પાછા 1;
}

સ્થિર પૂર્ણાંક add_resolved_files(સંરચના repository *repo,
			      અચલ સંરચના pathspec *pathspec,
			      પૂર્ણાંક flags)
{
	સંરચના index_state *istate = repo->index;
	સંરચના string_list unmerged_paths = STRING_LIST_INIT_DUP;
	સંરચના string_list unresolved_paths = STRING_LIST_INIT_DUP;
	પૂર્ણાંક exit_status = 0;
	size_t i;

	માટે (i = 0; i < istate->cache_nr; i++) {
		સંરચના cache_entry *ce = istate->cache[i];
		જો (!ce_stage(ce))
			ચાલુ;
		જો (pathspec->nr && !ce_path_match(istate, ce, pathspec, ખાલી))
			ચાલુ;
		જો (!unmerged_paths.nr ||
		    strcmp(unmerged_paths.વસ્તુઓ[unmerged_paths.nr - 1].string, ce->name))
			string_list_append(&unmerged_paths, ce->name);
	}

	જો (!unmerged_paths.nr) {
		string_list_clear(&unmerged_paths, 0);
		પાછા 0;
	}

	માટે (i = 0; i < unmerged_paths.nr; i++) {
		અચલ અક્ષર *path = unmerged_paths.વસ્તુઓ[i].string;
		સંરચના stat st;

		જો (!lstat(path, &st) && S_ISREG(st.st_mode)) {
			જો (has_conflict_markers(istate, path))
				string_list_append(&unresolved_paths, path);
		}
	}

	જો (unresolved_paths.nr) {
		સંરચના strbuf sb = STRBUF_INIT;
		માટે (i = 0; i < unresolved_paths.nr; i++)
			strbuf_addf(&sb, "\t%s\n", unresolved_paths.વસ્તુઓ[i].string);
		die(_("the following paths still have conflict markers:\n%s"), sb.buf);
	}

	માટે (i = 0; i < unmerged_paths.nr; i++) {
		અચલ અક્ષર *path = unmerged_paths.વસ્તુઓ[i].string;
		સંરચના stat st;

		જો (lstat(path, &st)) {
			જો (errno != ENOENT)
				die_errno(_("cannot lstat: '%s'"), path);
			જો (remove_file_from_index_with_flags(istate, path, flags))
				exit_status = failed_to_add(flags, path);
		} નહીંતર {
			જો (add_file_to_index(istate, path, flags))
				exit_status = failed_to_add(flags, path);
		}
	}

	string_list_clear(&unmerged_paths, 0);
	string_list_clear(&unresolved_paths, 0);
	પાછા exit_status;
}

પૂર્ણાંક cmd_add(પૂર્ણાંક argc,
	    અચલ અક્ષર **argv,
	    અચલ અક્ષર *prefix,
	    સંરચના repository *repo)
{
	પૂર્ણાંક exit_status = 0;
	સંરચના pathspec pathspec;
	સંરચના dir_struct dir = DIR_INIT;
	પૂર્ણાંક flags;
	પૂર્ણાંક add_new_files;
	પૂર્ણાંક require_pathspec;
	અક્ષર *seen = ખાલી;
	અક્ષર *ps_matched = ખાલી;
	સંરચના lock_file lock_file = LOCK_INIT;
	સંરચના odb_transaction *transaction = ખાલી;

	repo_config(repo, add_config, ખાલી);

	argc = parse_options(argc, argv, prefix, builtin_add_options,
			  builtin_add_usage, PARSE_OPT_KEEP_ARGV0);

	prepare_repo_settings(repo);
	repo->settings.command_requires_full_index = 0;

	જો (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	જો (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	જો (patch_interactive)
		add_interactive = 1;
	જો (add_interactive) {
		જો (show_only)
			die(_("options '%s' and '%s' cannot be used together"), "--dry-run", "--interactive/--patch");
		જો (pathspec_from_file)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--interactive/--patch");
		exit(interactive_add(repo, argv + 1, prefix, patch_interactive, &interactive_opts));
	} નહીંતર {
		જો (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--interactive/--patch");
		જો (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--interactive/--patch");
		જો (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--interactive/--patch");
	}

	જો (edit_interactive) {
		જો (pathspec_from_file)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--edit");
		પાછા(edit_patch(repo, argc, argv, prefix));
	}
	argc--;
	argv++;

	જો (0 <= addremove_explicit)
		addremove = addremove_explicit;
	નહીંતર જો (take_worktree_changes && ADDREMOVE_DEFAULT)
		addremove = 0; /* "-u" was given but not "-A" */

	die_for_incompatible_opt3(take_worktree_changes, "-u/--update",
				  0 < addremove_explicit, "-A/--all",
				  add_resolved, "--resolved");

	જો (!show_only && ignore_missing)
		die(_("the option '%s' requires '%s'"), "--ignore-missing", "--dry-run");

	જો (chmod_arg && ((chmod_arg[0] != '-' && chmod_arg[0] != '+') ||
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

	જો (pathspec_from_file) {
		જો (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_FULL |
				    PATHSPEC_SYMLINK_LEADING_PATH,
				    prefix, pathspec_from_file, pathspec_file_nul);
	} નહીંતર જો (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	જો (require_pathspec && pathspec.nr == 0) {
		fprintf(stderr, _("Nothing specified, nothing added.\n"));
		advise_if_enabled(ADVICE_ADD_EMPTY_PATHSPEC,
				  _("Maybe you wanted to say 'git add .'?"));
		પાછા 0;
	}

	જો (!take_worktree_changes && !add_resolved &&
	    addremove_explicit < 0 && pathspec.nr)
		/* Turn "git add pathspec..." to "git add -A pathspec..." */
		addremove = 1;

	flags = ((verbose ? ADD_CACHE_VERBOSE : 0) |
		 (show_only ? ADD_CACHE_PRETEND : 0) |
		 (intent_to_add ? ADD_CACHE_INTENT : 0) |
		 (ignore_add_errors ? ADD_CACHE_IGNORE_ERRORS : 0) |
		 (!(addremove || take_worktree_changes)
		  ? ADD_CACHE_IGNORE_REMOVAL : 0));

	જો (repo_read_index_preload(repo, &pathspec, 0) < 0)
		die(_("index file corrupt"));

	die_in_unpopulated_submodule(repo->index, prefix);
	die_path_inside_submodule(repo->index, &pathspec);

	જો (add_new_files) {
		પૂર્ણાંક baselen;

		/* Set up the default git porcelain excludes */
		જો (!ignored_too) {
			dir.flags |= DIR_COLLECT_IGNORED;
			setup_standard_excludes(&dir);
		}

		/* This picks up the paths that are not tracked */
		baselen = fill_directory(&dir, repo->index, &pathspec);
		જો (pathspec.nr)
			seen = prune_directory(repo, &dir, &pathspec, baselen);
	}

	જો (refresh_only) {
		exit_status |= refresh(repo, verbose, &pathspec);
		જાઓ finish;
	}

	જો (pathspec.nr) {
		પૂર્ણાંક i;
		અક્ષર *skip_worktree_seen = ખાલી;
		સંરચના string_list only_match_skip_worktree = STRING_LIST_INIT_NODUP;

		જો (!seen)
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

		માટે (i = 0; i < pathspec.nr; i++) {
			અચલ અક્ષર *path = pathspec.વસ્તુઓ[i].match;

			જો (pathspec.વસ્તુઓ[i].magic & PATHSPEC_EXCLUDE)
				ચાલુ;
			જો (seen[i])
				ચાલુ;

			જો (!include_sparse &&
			    matches_skip_worktree(&pathspec, i, &skip_worktree_seen)) {
				string_list_append(&only_match_skip_worktree,
						   pathspec.વસ્તુઓ[i].original);
				ચાલુ;
			}

			/* Don't complain at 'git add .' on empty repo */
			જો (!path[0])
				ચાલુ;

			જો ((pathspec.વસ્તુઓ[i].magic & (PATHSPEC_GLOB | PATHSPEC_ICASE)) ||
			    !file_exists(path)) {
				જો (ignore_missing) {
					પૂર્ણાંક dtype = DT_UNKNOWN;
					જો (is_excluded(&dir, repo->index, path, &dtype))
						dir_add_ignored(&dir, repo->index,
								path, pathspec.વસ્તુઓ[i].len);
				} નહીંતર
					die(_("pathspec '%s' did not match any files"),
					    pathspec.વસ્તુઓ[i].original);
			}
		}


		જો (only_match_skip_worktree.nr) {
			advise_on_updating_sparse_paths(&only_match_skip_worktree);
			exit_status = 1;
		}

		મુક્ત(seen);
		મુક્ત(skip_worktree_seen);
		string_list_clear(&only_match_skip_worktree, 0);
	}

	odb_transaction_begin_or_die(repo->objects, &transaction, 0);

	ps_matched = xcalloc(pathspec.nr, 1);
	જો (add_resolved)
		exit_status |= add_resolved_files(repo, &pathspec, flags);
	નહીંતર જો (add_renormalize)
		exit_status |= renormalize_tracked_files(repo, &pathspec, flags);
	નહીંતર
		exit_status |= add_files_to_cache(repo, prefix,
						  &pathspec, ps_matched,
						  include_sparse, flags, ignored_too);

	જો (take_worktree_changes && !add_renormalize && !ignore_add_errors &&
	    report_path_error(ps_matched, &pathspec))
		exit(128);

	જો (add_new_files)
		exit_status |= add_files(repo, &dir, flags);

	જો (chmod_arg && pathspec.nr)
		exit_status |= chmod_pathspec(repo, &pathspec, chmod_arg[0], show_only);
	odb_transaction_commit_and_finalize_or_die(transaction);

finish:
	જો (write_locked_index(repo->index, &lock_file,
			       COMMIT_LOCK | SKIP_IF_UNCHANGED))
		die(_("unable to write new index file"));

	મુક્ત(ps_matched);
	dir_clear(&dir);
	clear_pathspec(&pathspec);
	પાછા exit_status;
}
