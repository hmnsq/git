/*
 * "git add" builtin command
 *
 * Copyright (C) 2006 Linus Torvalds
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "advice.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "lockfile.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "editor.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "dir.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "pathspec.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-file.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "odb.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "odb/transaction.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "path.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "preload-index.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "diff.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "read-cache.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "revision.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "submodule.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "add-interactive.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "merge-ll.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "apply.h"

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ builtin_ਜੋੜੋ_usage[] = {
	N_("git add [<options>] [--] <pathspec>..."),
	ਨਿਰਾ
};
ਸਥਿਰ ਪੂਰਨ patch_interactive, ਜੋੜੋ_interactive, edit_interactive;
ਸਥਿਰ ਬਣਤਰ interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
ਸਥਿਰ ਪੂਰਨ take_worktree_changes;
ਸਥਿਰ ਪੂਰਨ ਜੋੜੋ_renormalize;
ਸਥਿਰ ਪੂਰਨ ਜੋੜੋ_resolved;
ਸਥਿਰ ਪੂਰਨ pathspec_file_nul;
ਸਥਿਰ ਪੂਰਨ include_sparse;
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *pathspec_from_file;

ਸਥਿਰ ਪੂਰਨ chmod_pathspec(ਬਣਤਰ repository *repo,
			  ਬਣਤਰ pathspec *pathspec,
			  ਅੱਖਰ flip,
			  ਪੂਰਨ show_only)
{
	ਪੂਰਨ ret = 0;

	ਲਈ (ਮਾਪ_t i = 0; i < repo->ਸੂਚਕਾਂਕ->cache_nr; i++) {
		ਬਣਤਰ cache_ਇੰਦਰਾਜ *ce = repo->ਸੂਚਕਾਂਕ->cache[i];
		ਪੂਰਨ err;

		ਜੇ (!include_sparse &&
		    (ce_skip_worktree(ce) ||
		     !ਰਸਤਾ_ਵਿੱਚ_sparse_checkout(ce->name, repo->ਸੂਚਕਾਂਕ)))
			ਜਾਰੀ;

		ਜੇ (pathspec && !ce_ਰਸਤਾ_ਮੇਲ(repo->ਸੂਚਕਾਂਕ, ce, pathspec, ਨਿਰਾ))
			ਜਾਰੀ;

		ਜੇ (!show_only)
			err = chmod_ਸੂਚਕਾਂਕ_ਇੰਦਰਾਜ(repo->ਸੂਚਕਾਂਕ, ce, flip);
		ਨਹੀਂਤਾਂ
			err = S_ISREG(ce->ce_ਬਹੁਲਕ) ? 0 : -1;

		ਜੇ (err < 0)
			ret = error(_("cannot chmod %cx '%s'"), flip, ce->name);
	}

	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ renormalize_tracked_files(ਬਣਤਰ repository *repo,
				     ਅਚਲ ਬਣਤਰ pathspec *pathspec,
				     ਪੂਰਨ flags)
{
	ਪੂਰਨ retval = 0;

	ਲਈ (ਮਾਪ_t i = 0; i < repo->ਸੂਚਕਾਂਕ->cache_nr; i++) {
		ਬਣਤਰ cache_ਇੰਦਰਾਜ *ce = repo->ਸੂਚਕਾਂਕ->cache[i];

		ਜੇ (!include_sparse &&
		    (ce_skip_worktree(ce) ||
		     !ਰਸਤਾ_ਵਿੱਚ_sparse_checkout(ce->name, repo->ਸੂਚਕਾਂਕ)))
			ਜਾਰੀ;
		ਜੇ (ce_stage(ce))
			ਜਾਰੀ; /* do not touch unmerged paths */
		ਜੇ (!S_ISREG(ce->ce_ਬਹੁਲਕ) && !S_ISLNK(ce->ce_ਬਹੁਲਕ))
			ਜਾਰੀ; /* do not touch non blobs */
		ਜੇ (pathspec && !ce_ਰਸਤਾ_ਮੇਲ(repo->ਸੂਚਕਾਂਕ, ce, pathspec, ਨਿਰਾ))
			ਜਾਰੀ;
		retval |= ਜੋੜੋ_file_ਮੰਜ਼ਿਲ_ਸੂਚਕਾਂਕ(repo->ਸੂਚਕਾਂਕ, ce->name,
					    flags | ਜੋੜੋ_CACHE_RENORMALIZE);
	}

	ਵਾਪਸ retval;
}

ਸਥਿਰ ਅੱਖਰ *prune_directory(ਬਣਤਰ repository *repo,
			     ਬਣਤਰ dir_struct *dir,
			     ਬਣਤਰ pathspec *pathspec,
			     ਪੂਰਨ ਅਗੇਤਰ)
{
	ਅੱਖਰ *ਵੇਖਿਆ;
	ਪੂਰਨ i;
	ਬਣਤਰ dir_ਇੰਦਰਾਜ **src, **dst;

	ਵੇਖਿਆ = xcalloc(pathspec->nr, 1);

	src = dst = dir->entries;
	i = dir->nr;
	ਜਦੋਂ (--i >= 0) {
		ਬਣਤਰ dir_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ = *src++;
		ਜੇ (dir_ਰਸਤਾ_ਮੇਲ(repo->ਸੂਚਕਾਂਕ, ਇੰਦਰਾਜ, pathspec, ਅਗੇਤਰ, ਵੇਖਿਆ))
			*dst++ = ਇੰਦਰਾਜ;
	}
	dir->nr = dst - dir->entries;
	ਜੋੜੋ_pathspec_matches_against_ਸੂਚਕਾਂਕ(pathspec, repo->ਸੂਚਕਾਂਕ, ਵੇਖਿਆ,
					   PS_IGNORE_SKIP_WORKTREE);
	ਵਾਪਸ ਵੇਖਿਆ;
}

ਸਥਿਰ ਪੂਰਨ refresh(ਬਣਤਰ repository *repo, ਪੂਰਨ verbose, ਅਚਲ ਬਣਤਰ pathspec *pathspec)
{
	ਅੱਖਰ *ਵੇਖਿਆ;
	ਪੂਰਨ i, ret = 0;
	ਅੱਖਰ *skip_worktree_ਵੇਖਿਆ = ਨਿਰਾ;
	ਬਣਤਰ string_list only_ਮੇਲ_skip_worktree = STRING_LIST_INIT_NODUP;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags = REFRESH_IGNORE_SKIP_WORKTREE |
		    (verbose ? REFRESH_ਵਿੱਚ_PORCELAIN : REFRESH_QUIET);

	ਵੇਖਿਆ = xcalloc(pathspec->nr, 1);
	refresh_ਸੂਚਕਾਂਕ(repo->ਸੂਚਕਾਂਕ, flags, pathspec, ਵੇਖਿਆ,
		      _("Unstaged changes after refreshing the index:"));
	ਲਈ (i = 0; i < pathspec->nr; i++) {
		ਜੇ (!ਵੇਖਿਆ[i]) {
			ਅਚਲ ਅੱਖਰ *ਰਸਤਾ = pathspec->ਚੀਜ਼ਾਂ[i].original;

			ਜੇ (matches_skip_worktree(pathspec, i, &skip_worktree_ਵੇਖਿਆ) ||
			    !ਰਸਤਾ_ਵਿੱਚ_sparse_checkout(ਰਸਤਾ, repo->ਸੂਚਕਾਂਕ)) {
				string_list_append(&only_ਮੇਲ_skip_worktree,
						   pathspec->ਚੀਜ਼ਾਂ[i].original);
			} ਨਹੀਂਤਾਂ {
				die(_("pathspec '%s' did not match any files"),
				    pathspec->ਚੀਜ਼ਾਂ[i].original);
			}
		}
	}

	ਜੇ (only_ਮੇਲ_skip_worktree.nr) {
		advise_on_updating_sparse_paths(&only_ਮੇਲ_skip_worktree);
		ret = 1;
	}

	ਮੁਕਤ(ਵੇਖਿਆ);
	ਮੁਕਤ(skip_worktree_ਵੇਖਿਆ);
	string_list_clear(&only_ਮੇਲ_skip_worktree, 0);
	ਵਾਪਸ ret;
}

ਪੂਰਨ interactive_ਜੋੜੋ(ਬਣਤਰ repository *repo,
		    ਅਚਲ ਅੱਖਰ **argv,
		    ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		    ਪੂਰਨ patch, ਬਣਤਰ interactive_options *interactive_opts)
{
	ਬਣਤਰ pathspec pathspec;
	ਪੂਰਨ ret;

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_ਭਰਿਆ |
		       PATHSPEC_SYMLINK_LEADING_ਰਸਤਾ |
		       PATHSPEC_ਅਗੇਤਰ_ORIGIN,
		       ਅਗੇਤਰ, argv);

	ਜੇ (patch)
		ret = !!ਲੜੀਵਾਰ_ਜੋੜੋ_p(repo, ਜੋੜੋ_P_ਜੋੜੋ, interactive_opts, ਨਿਰਾ, &pathspec, 0);
	ਨਹੀਂਤਾਂ
		ret = !!ਲੜੀਵਾਰ_ਜੋੜੋ_i(repo, &pathspec, interactive_opts);

	clear_pathspec(&pathspec);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ edit_patch(ਬਣਤਰ repository *repo,
		      ਪੂਰਨ argc,
		      ਅਚਲ ਅੱਖਰ **argv,
		      ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ)
{
	ਅੱਖਰ *file = repo_git_ਰਸਤਾ(repo, "ADD_EDIT.patch");
	ਬਣਤਰ apply_state state;
	ਅਚਲ ਅੱਖਰ *apply_argv[2];
	ਬਣਤਰ rev_info rev;
	ਪੂਰਨ ਬਾਹਰਲਾ;
	ਬਣਤਰ stat st;

	repo_config(repo, git_diff_basic_config, ਨਿਰਾ);

	ਜੇ (repo_read_ਸੂਚਕਾਂਕ(repo) < 0)
		die(_("could not read the index"));

	repo_init_revisions(repo, &rev, ਅਗੇਤਰ);
	rev.diffopt.context = 7;

	argc = ਤਿਆਰੀ_revisions(argc, argv, &rev, ਨਿਰਾ);
	rev.diffopt.output_format = DIFF_FORMAT_PATCH;
	rev.diffopt.use_color = GIT_COLOR_NEVER;
	rev.diffopt.flags.ignore_dirty_submodules = 1;
	ਬਾਹਰਲਾ = xopen(file, O_CREAT | O_WRONLY | O_TRUNC, 0666);
	rev.diffopt.file = xfdopen(ਬਾਹਰਲਾ, "w");
	rev.diffopt.close_file = 1;
	ਲੜੀਵਾਰ_diff_files(&rev, 0);

	ਜੇ (launch_editor(file, ਨਿਰਾ, ਨਿਰਾ))
		die(_("editing patch failed"));

	ਜੇ (stat(file, &st))
		die_errno(_("could not stat '%s'"), file);
	ਜੇ (!st.st_ਮਾਪ)
		die(_("empty patch. aborted"));

	apply_argv[0] = file;
	apply_argv[1] = ਨਿਰਾ;
	ਜੇ (init_apply_state(&state, repo, ਨਿਰਾ))
		die(_("could not initialize apply state"));
	state.cached = 1;
	ਜੇ (check_apply_state(&state, 0))
		die(_("could not check apply state"));
	ਜੇ (apply_all_patches(&state, 1, apply_argv, APPLY_OPT_RECOUNT))
		die(_("could not apply '%s'"), file);
	clear_apply_state(&state);

	unlink(file);
	ਮੁਕਤ(file);
	release_revisions(&rev);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ ignore_error[] =
N_("The following paths are ignored by one of your .gitignore files:\n");

ਸਥਿਰ ਪੂਰਨ verbose, show_only, ignored_too, refresh_only;
ਸਥਿਰ ਪੂਰਨ ignore_ਜੋੜੋ_errors, intent_ਮੰਜ਼ਿਲ_ਜੋੜੋ, ignore_missing;
ਸਥਿਰ ਪੂਰਨ warn_on_embedded_repo = 1;

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ADDREMOVE_DEFAULT 1
ਸਥਿਰ ਪੂਰਨ addremove = ADDREMOVE_DEFAULT;
ਸਥਿਰ ਪੂਰਨ addremove_explicit = -1; /* unspecified */

ਸਥਿਰ ਅੱਖਰ *chmod_arg;

ਸਥਿਰ ਪੂਰਨ ignore_removal_cb(ਅਚਲ ਬਣਤਰ option *opt, ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	BUG_ON_OPT_ARG(arg);

	/* if we are told to ignore, we are not adding removals */
	*(ਪੂਰਨ *)opt->ਮੁੱਲ = !unset ? 0 : 1;
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਬਣਤਰ option builtin_ਜੋੜੋ_options[] = {
	OPT_DRY_ਲੜੀਵਾਰ(&show_only, N_("dry run")),
	OPT__VERBOSE(&verbose, N_("be verbose")),
	OPT_ਸਮੂਹ(""),
	OPT_BOOL('i', "interactive", &ਜੋੜੋ_interactive, N_("interactive picking")),
	OPT_BOOL('p', "patch", &patch_interactive, N_("select hunks interactively")),
	OPT_BOOL(0, "auto-advance", &interactive_opts.auto_advance,
		 N_("auto advance to the next file when selecting hunks interactively")),
	OPT_DIFF_UNIFIED(&interactive_opts.context),
	OPT_DIFF_INTERHUNK_CONTEXT(&interactive_opts.interhunkcontext),
	OPT_BOOL('e', "edit", &edit_interactive, N_("edit current diff and apply")),
	OPT__FORCE(&ignored_too, N_("allow adding otherwise ignored files"), 0),
	OPT_BOOL('u', "update", &take_worktree_changes, N_("update tracked files")),
	OPT_BOOL(0, "renormalize", &ਜੋੜੋ_renormalize, N_("renormalize EOL of tracked files (implies -u)")),
	OPT_BOOL(0, "resolved", &ਜੋੜੋ_resolved, N_("add conflict-resolved tracked files")),
	OPT_BOOL('N', "intent-to-add", &intent_ਮੰਜ਼ਿਲ_ਜੋੜੋ, N_("record only the fact that the path will be added later")),
	OPT_BOOL('A', "all", &addremove_explicit, N_("add changes from all tracked and untracked files")),
	OPT_CALLBACK_F(0, "ignore-removal", &addremove_explicit,
	  ਨਿਰਾ /* takes no arguments */,
	  N_("ignore paths removed in the working tree (same as --no-all)"),
	  PARSE_OPT_NOARG, ignore_removal_cb),
	OPT_BOOL( 0 , "refresh", &refresh_only, N_("don't add, only refresh the index")),
	OPT_BOOL( 0 , "ignore-errors", &ignore_ਜੋੜੋ_errors, N_("just skip files which cannot be added because of errors")),
	OPT_BOOL( 0 , "ignore-missing", &ignore_missing, N_("check if - even missing - files are ignored in dry run")),
	OPT_BOOL(0, "sparse", &include_sparse, N_("allow updating entries outside of the sparse-checkout cone")),
	OPT_STRING(0, "chmod", &chmod_arg, "(+|-)x",
		   N_("override the executable bit of the listed files")),
	OPT_HIDDEN_BOOL(0, "warn-embedded-repo", &warn_on_embedded_repo,
			N_("warn when adding an embedded repository")),
	OPT_PATHSPEC_FROM_FILE(&pathspec_from_file),
	OPT_PATHSPEC_FILE_NUL(&pathspec_file_nul),
	OPT_ਸਿਰਾ(),
};

ਸਥਿਰ ਪੂਰਨ ਜੋੜੋ_config(ਅਚਲ ਅੱਖਰ *var, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ,
		      ਅਚਲ ਬਣਤਰ config_context *ctx, ਖਾਲੀ *cb)
{
	ਜੇ (!strcmp(var, "add.ignoreerrors") ||
	    !strcmp(var, "add.ignore-errors")) {
		ignore_ਜੋੜੋ_errors = git_config_bool(var, ਮੁੱਲ);
		ਵਾਪਸ 0;
	}

	ਜੇ (git_color_config(var, ਮੁੱਲ, cb) < 0)
		ਵਾਪਸ -1;

	ਵਾਪਸ git_default_config(var, ਮੁੱਲ, ctx, cb);
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ embedded_advice[] = N_(
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

ਸਥਿਰ ਖਾਲੀ check_embedded_repo(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ)
{
	ਬਣਤਰ strbuf name = STRBUF_INIT;
	ਸਥਿਰ ਪੂਰਨ adviced_on_embedded_repo = 0;

	ਜੇ (!warn_on_embedded_repo)
		ਵਾਪਸ;
	ਜੇ (!ਸਿਰੇ_with(ਰਸਤਾ, "/"))
		ਵਾਪਸ;

	/* Drop trailing slash for aesthetics */
	strbuf_addstr(&name, ਰਸਤਾ);
	strbuf_strip_suffix(&name, "/");

	warning(_("adding embedded git repository: %s"), name.buf);
	ਜੇ (!adviced_on_embedded_repo) {
		advise_if_enabled(ADVICE_ਜੋੜੋ_EMBEDDED_REPO,
				  embedded_advice, name.buf, name.buf);
		adviced_on_embedded_repo = 1;
	}

	strbuf_release(&name);
}

ਸਥਿਰ ਪੂਰਨ ਜੋੜੋ_files(ਬਣਤਰ repository *repo, ਬਣਤਰ dir_struct *dir, ਪੂਰਨ flags)
{
	ਪੂਰਨ i, exit_ਸਥਿਤੀ = 0;
	ਬਣਤਰ string_list matched_sparse_paths = STRING_LIST_INIT_NODUP;

	ਜੇ (dir->ignored_nr) {
		fprintf(stderr, _(ignore_error));
		ਲਈ (i = 0; i < dir->ignored_nr; i++)
			fprintf(stderr, "%s\n", dir->ignored[i]->name);
		advise_if_enabled(ADVICE_ਜੋੜੋ_IGNORED_FILE,
				  _("Use -f if you really want to add them."));
		exit_ਸਥਿਤੀ = 1;
	}

	ਲਈ (i = 0; i < dir->nr; i++) {
		ਜੇ (!include_sparse &&
		    !ਰਸਤਾ_ਵਿੱਚ_sparse_checkout(dir->entries[i]->name, repo->ਸੂਚਕਾਂਕ)) {
			string_list_append(&matched_sparse_paths,
					   dir->entries[i]->name);
			ਜਾਰੀ;
		}
		ਜੇ (ਜੋੜੋ_file_ਮੰਜ਼ਿਲ_ਸੂਚਕਾਂਕ(repo->ਸੂਚਕਾਂਕ, dir->entries[i]->name, flags)) {
			ਜੇ (!ignore_ਜੋੜੋ_errors)
				die(_("adding files failed"));
			exit_ਸਥਿਤੀ = 1;
		} ਨਹੀਂਤਾਂ {
			check_embedded_repo(dir->entries[i]->name);
		}
	}

	ਜੇ (matched_sparse_paths.nr) {
		advise_on_updating_sparse_paths(&matched_sparse_paths);
		exit_ਸਥਿਤੀ = 1;
	}

	string_list_clear(&matched_sparse_paths, 0);

	ਵਾਪਸ exit_ਸਥਿਤੀ;
}

ਸਥਿਰ ਪੂਰਨ failed_ਮੰਜ਼ਿਲ_ਜੋੜੋ(ਪੂਰਨ flags, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ)
{
	ਜੇ (!(flags & ਜੋੜੋ_CACHE_IGNORE_ERRORS))
		die(_("updating file '%s' failed"), ਰਸਤਾ);
	ਵਾਪਸ 1;
}

ਸਥਿਰ ਪੂਰਨ ਜੋੜੋ_resolved_files(ਬਣਤਰ repository *repo,
			      ਅਚਲ ਬਣਤਰ pathspec *pathspec,
			      ਪੂਰਨ flags)
{
	ਬਣਤਰ ਸੂਚਕਾਂਕ_state *istate = repo->ਸੂਚਕਾਂਕ;
	ਬਣਤਰ string_list unmerged_paths = STRING_LIST_INIT_DUP;
	ਬਣਤਰ string_list unresolved_paths = STRING_LIST_INIT_DUP;
	ਪੂਰਨ exit_ਸਥਿਤੀ = 0;
	ਮਾਪ_t i;

	ਲਈ (i = 0; i < istate->cache_nr; i++) {
		ਬਣਤਰ cache_ਇੰਦਰਾਜ *ce = istate->cache[i];
		ਜੇ (!ce_stage(ce))
			ਜਾਰੀ;
		ਜੇ (pathspec->nr && !ce_ਰਸਤਾ_ਮੇਲ(istate, ce, pathspec, ਨਿਰਾ))
			ਜਾਰੀ;
		ਜੇ (!unmerged_paths.nr ||
		    strcmp(unmerged_paths.ਚੀਜ਼ਾਂ[unmerged_paths.nr - 1].string, ce->name))
			string_list_append(&unmerged_paths, ce->name);
	}

	ਜੇ (!unmerged_paths.nr) {
		string_list_clear(&unmerged_paths, 0);
		ਵਾਪਸ 0;
	}

	ਲਈ (i = 0; i < unmerged_paths.nr; i++) {
		ਅਚਲ ਅੱਖਰ *ਰਸਤਾ = unmerged_paths.ਚੀਜ਼ਾਂ[i].string;
		ਬਣਤਰ stat st;

		ਜੇ (!lstat(ਰਸਤਾ, &st) && S_ISREG(st.st_ਬਹੁਲਕ)) {
			ਜੇ (has_conflict_markers(istate, ਰਸਤਾ))
				string_list_append(&unresolved_paths, ਰਸਤਾ);
		}
	}

	ਜੇ (unresolved_paths.nr) {
		ਬਣਤਰ strbuf sb = STRBUF_INIT;
		ਲਈ (i = 0; i < unresolved_paths.nr; i++)
			strbuf_addf(&sb, "\t%s\n", unresolved_paths.ਚੀਜ਼ਾਂ[i].string);
		die(_("the following paths still have conflict markers:\n%s"), sb.buf);
	}

	ਲਈ (i = 0; i < unmerged_paths.nr; i++) {
		ਅਚਲ ਅੱਖਰ *ਰਸਤਾ = unmerged_paths.ਚੀਜ਼ਾਂ[i].string;
		ਬਣਤਰ stat st;

		ਜੇ (lstat(ਰਸਤਾ, &st)) {
			ਜੇ (errno != ENOENT)
				die_errno(_("cannot lstat: '%s'"), ਰਸਤਾ);
			ਜੇ (remove_file_from_ਸੂਚਕਾਂਕ_with_flags(istate, ਰਸਤਾ, flags))
				exit_ਸਥਿਤੀ = failed_ਮੰਜ਼ਿਲ_ਜੋੜੋ(flags, ਰਸਤਾ);
		} ਨਹੀਂਤਾਂ {
			ਜੇ (ਜੋੜੋ_file_ਮੰਜ਼ਿਲ_ਸੂਚਕਾਂਕ(istate, ਰਸਤਾ, flags))
				exit_ਸਥਿਤੀ = failed_ਮੰਜ਼ਿਲ_ਜੋੜੋ(flags, ਰਸਤਾ);
		}
	}

	string_list_clear(&unmerged_paths, 0);
	string_list_clear(&unresolved_paths, 0);
	ਵਾਪਸ exit_ਸਥਿਤੀ;
}

ਪੂਰਨ cmd_ਜੋੜੋ(ਪੂਰਨ argc,
	    ਅਚਲ ਅੱਖਰ **argv,
	    ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
	    ਬਣਤਰ repository *repo)
{
	ਪੂਰਨ exit_ਸਥਿਤੀ = 0;
	ਬਣਤਰ pathspec pathspec;
	ਬਣਤਰ dir_struct dir = DIR_INIT;
	ਪੂਰਨ flags;
	ਪੂਰਨ ਜੋੜੋ_new_files;
	ਪੂਰਨ require_pathspec;
	ਅੱਖਰ *ਵੇਖਿਆ = ਨਿਰਾ;
	ਅੱਖਰ *ps_matched = ਨਿਰਾ;
	ਬਣਤਰ ਤਾਲਾ_file ਤਾਲਾ_file = ਤਾਲਾ_INIT;
	ਬਣਤਰ odb_transaction *transaction = ਨਿਰਾ;

	repo_config(repo, ਜੋੜੋ_config, ਨਿਰਾ);

	argc = parse_options(argc, argv, ਅਗੇਤਰ, builtin_ਜੋੜੋ_options,
			  builtin_ਜੋੜੋ_usage, PARSE_OPT_KEEP_ARGV0);

	prepare_repo_settings(repo);
	repo->settings.command_requires_ਭਰਿਆ_ਸੂਚਕਾਂਕ = 0;

	ਜੇ (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	ਜੇ (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	ਜੇ (patch_interactive)
		ਜੋੜੋ_interactive = 1;
	ਜੇ (ਜੋੜੋ_interactive) {
		ਜੇ (show_only)
			die(_("options '%s' and '%s' cannot be used together"), "--dry-run", "--interactive/--patch");
		ਜੇ (pathspec_from_file)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--interactive/--patch");
		ਬਾਹਰ_ਨਿਕਲੋ(interactive_ਜੋੜੋ(repo, argv + 1, ਅਗੇਤਰ, patch_interactive, &interactive_opts));
	} ਨਹੀਂਤਾਂ {
		ਜੇ (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--interactive/--patch");
		ਜੇ (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--interactive/--patch");
		ਜੇ (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--interactive/--patch");
	}

	ਜੇ (edit_interactive) {
		ਜੇ (pathspec_from_file)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--edit");
		ਵਾਪਸ(edit_patch(repo, argc, argv, ਅਗੇਤਰ));
	}
	argc--;
	argv++;

	ਜੇ (0 <= addremove_explicit)
		addremove = addremove_explicit;
	ਨਹੀਂਤਾਂ ਜੇ (take_worktree_changes && ADDREMOVE_DEFAULT)
		addremove = 0; /* "-u" was given but not "-A" */

	die_for_incompatible_opt3(take_worktree_changes, "-u/--update",
				  0 < addremove_explicit, "-A/--all",
				  ਜੋੜੋ_resolved, "--resolved");

	ਜੇ (!show_only && ignore_missing)
		die(_("the option '%s' requires '%s'"), "--ignore-missing", "--dry-run");

	ਜੇ (chmod_arg && ((chmod_arg[0] != '-' && chmod_arg[0] != '+') ||
			  chmod_arg[1] != 'x' || chmod_arg[2]))
		die(_("--chmod param '%s' must be either -x or +x"), chmod_arg);

	ਜੋੜੋ_new_files = !take_worktree_changes && !refresh_only &&
			!ਜੋੜੋ_renormalize && !ਜੋੜੋ_resolved;
	require_pathspec = !(take_worktree_changes ||
			     (0 < addremove_explicit) ||
			     ਜੋੜੋ_resolved);

	repo_hold_locked_ਸੂਚਕਾਂਕ(repo, &ਤਾਲਾ_file, ਤਾਲਾ_DIE_ON_ERROR);

	/*
	 * Check the "pathspec '%s' did not match any files" block
	 * below before enabling new magic.
	 */
	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_ਭਰਿਆ |
		       PATHSPEC_SYMLINK_LEADING_ਰਸਤਾ,
		       ਅਗੇਤਰ, argv);

	ਜੇ (pathspec_from_file) {
		ਜੇ (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_ਭਰਿਆ |
				    PATHSPEC_SYMLINK_LEADING_ਰਸਤਾ,
				    ਅਗੇਤਰ, pathspec_from_file, pathspec_file_nul);
	} ਨਹੀਂਤਾਂ ਜੇ (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	ਜੇ (require_pathspec && pathspec.nr == 0) {
		fprintf(stderr, _("Nothing specified, nothing added.\n"));
		advise_if_enabled(ADVICE_ਜੋੜੋ_ਖਾਲੀ_ਥਾਵਾਂ_PATHSPEC,
				  _("Maybe you wanted to say 'git add .'?"));
		ਵਾਪਸ 0;
	}

	ਜੇ (!take_worktree_changes && !ਜੋੜੋ_resolved &&
	    addremove_explicit < 0 && pathspec.nr)
		/* Turn "git add pathspec..." to "git add -A pathspec..." */
		addremove = 1;

	flags = ((verbose ? ਜੋੜੋ_CACHE_VERBOSE : 0) |
		 (show_only ? ਜੋੜੋ_CACHE_PRETEND : 0) |
		 (intent_ਮੰਜ਼ਿਲ_ਜੋੜੋ ? ਜੋੜੋ_CACHE_INTENT : 0) |
		 (ignore_ਜੋੜੋ_errors ? ਜੋੜੋ_CACHE_IGNORE_ERRORS : 0) |
		 (!(addremove || take_worktree_changes)
		  ? ਜੋੜੋ_CACHE_IGNORE_REMOVAL : 0));

	ਜੇ (repo_read_ਸੂਚਕਾਂਕ_preload(repo, &pathspec, 0) < 0)
		die(_("index file corrupt"));

	die_ਵਿੱਚ_unpopulated_submodule(repo->ਸੂਚਕਾਂਕ, ਅਗੇਤਰ);
	die_ਰਸਤਾ_inside_submodule(repo->ਸੂਚਕਾਂਕ, &pathspec);

	ਜੇ (ਜੋੜੋ_new_files) {
		ਪੂਰਨ baselen;

		/* Set up the default git porcelain excludes */
		ਜੇ (!ignored_too) {
			dir.flags |= DIR_COLLECT_IGNORED;
			ਤਿਆਰੀ_standard_excludes(&dir);
		}

		/* This picks up the paths that are not tracked */
		baselen = fill_directory(&dir, repo->ਸੂਚਕਾਂਕ, &pathspec);
		ਜੇ (pathspec.nr)
			ਵੇਖਿਆ = prune_directory(repo, &dir, &pathspec, baselen);
	}

	ਜੇ (refresh_only) {
		exit_ਸਥਿਤੀ |= refresh(repo, verbose, &pathspec);
		ਜਾਓ ਅੰਤ;
	}

	ਜੇ (pathspec.nr) {
		ਪੂਰਨ i;
		ਅੱਖਰ *skip_worktree_ਵੇਖਿਆ = ਨਿਰਾ;
		ਬਣਤਰ string_list only_ਮੇਲ_skip_worktree = STRING_LIST_INIT_NODUP;

		ਜੇ (!ਵੇਖਿਆ)
			ਵੇਖਿਆ = ਲੱਭੋ_pathspecs_matching_against_ਸੂਚਕਾਂਕ(&pathspec,
					repo->ਸੂਚਕਾਂਕ, PS_IGNORE_SKIP_WORKTREE);

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

		ਲਈ (i = 0; i < pathspec.nr; i++) {
			ਅਚਲ ਅੱਖਰ *ਰਸਤਾ = pathspec.ਚੀਜ਼ਾਂ[i].ਮੇਲ;

			ਜੇ (pathspec.ਚੀਜ਼ਾਂ[i].magic & PATHSPEC_EXCLUDE)
				ਜਾਰੀ;
			ਜੇ (ਵੇਖਿਆ[i])
				ਜਾਰੀ;

			ਜੇ (!include_sparse &&
			    matches_skip_worktree(&pathspec, i, &skip_worktree_ਵੇਖਿਆ)) {
				string_list_append(&only_ਮੇਲ_skip_worktree,
						   pathspec.ਚੀਜ਼ਾਂ[i].original);
				ਜਾਰੀ;
			}

			/* Don't complain at 'git add .' on empty repo */
			ਜੇ (!ਰਸਤਾ[0])
				ਜਾਰੀ;

			ਜੇ ((pathspec.ਚੀਜ਼ਾਂ[i].magic & (PATHSPEC_GLOB | PATHSPEC_ICASE)) ||
			    !file_exists(ਰਸਤਾ)) {
				ਜੇ (ignore_missing) {
					ਪੂਰਨ dtype = DT_UNKNOWN;
					ਜੇ (is_excluded(&dir, repo->ਸੂਚਕਾਂਕ, ਰਸਤਾ, &dtype))
						dir_ਜੋੜੋ_ignored(&dir, repo->ਸੂਚਕਾਂਕ,
								ਰਸਤਾ, pathspec.ਚੀਜ਼ਾਂ[i].len);
				} ਨਹੀਂਤਾਂ
					die(_("pathspec '%s' did not match any files"),
					    pathspec.ਚੀਜ਼ਾਂ[i].original);
			}
		}


		ਜੇ (only_ਮੇਲ_skip_worktree.nr) {
			advise_on_updating_sparse_paths(&only_ਮੇਲ_skip_worktree);
			exit_ਸਥਿਤੀ = 1;
		}

		ਮੁਕਤ(ਵੇਖਿਆ);
		ਮੁਕਤ(skip_worktree_ਵੇਖਿਆ);
		string_list_clear(&only_ਮੇਲ_skip_worktree, 0);
	}

	odb_transaction_begin_or_die(repo->objects, &transaction, 0);

	ps_matched = xcalloc(pathspec.nr, 1);
	ਜੇ (ਜੋੜੋ_resolved)
		exit_ਸਥਿਤੀ |= ਜੋੜੋ_resolved_files(repo, &pathspec, flags);
	ਨਹੀਂਤਾਂ ਜੇ (ਜੋੜੋ_renormalize)
		exit_ਸਥਿਤੀ |= renormalize_tracked_files(repo, &pathspec, flags);
	ਨਹੀਂਤਾਂ
		exit_ਸਥਿਤੀ |= ਜੋੜੋ_files_ਮੰਜ਼ਿਲ_cache(repo, ਅਗੇਤਰ,
						  &pathspec, ps_matched,
						  include_sparse, flags, ignored_too);

	ਜੇ (take_worktree_changes && !ਜੋੜੋ_renormalize && !ignore_ਜੋੜੋ_errors &&
	    report_ਰਸਤਾ_error(ps_matched, &pathspec))
		ਬਾਹਰ_ਨਿਕਲੋ(128);

	ਜੇ (ਜੋੜੋ_new_files)
		exit_ਸਥਿਤੀ |= ਜੋੜੋ_files(repo, &dir, flags);

	ਜੇ (chmod_arg && pathspec.nr)
		exit_ਸਥਿਤੀ |= chmod_pathspec(repo, &pathspec, chmod_arg[0], show_only);
	odb_transaction_commit_and_finalize_or_die(transaction);

ਅੰਤ:
	ਜੇ (write_locked_ਸੂਚਕਾਂਕ(repo->ਸੂਚਕਾਂਕ, &ਤਾਲਾ_file,
			       COMMIT_ਤਾਲਾ | SKIP_IF_UNCHANGED))
		die(_("unable to write new index file"));

	ਮੁਕਤ(ps_matched);
	dir_clear(&dir);
	clear_pathspec(&pathspec);
	ਵਾਪਸ exit_ਸਥਿਤੀ;
}
