/*
 * "git add" builtin command
 *
 * Copyright (C) 2006 Linus Torvalds
 */

#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "advice.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "lockfile.h"
#ଅନ୍ତର୍ଭାଵ "editor.h"
#ଅନ୍ତର୍ଭାଵ "dir.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "pathspec.h"
#ଅନ୍ତର୍ଭାଵ "object-file.h"
#ଅନ୍ତର୍ଭାଵ "odb.h"
#ଅନ୍ତର୍ଭାଵ "odb/transaction.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "path.h"
#ଅନ୍ତର୍ଭାଵ "preload-index.h"
#ଅନ୍ତର୍ଭାଵ "diff.h"
#ଅନ୍ତର୍ଭାଵ "read-cache.h"
#ଅନ୍ତର୍ଭାଵ "revision.h"
#ଅନ୍ତର୍ଭାଵ "submodule.h"
#ଅନ୍ତର୍ଭାଵ "add-interactive.h"
#ଅନ୍ତର୍ଭାଵ "merge-ll.h"
#ଅନ୍ତର୍ଭାଵ "apply.h"

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ builtin_add_usage[] = {
	N_("git add [<options>] [--] <pathspec>..."),
	ଖାଲି
};
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା patch_interactive, add_interactive, edit_interactive;
ସ୍ଥିର ସଂରଚନା interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା take_worktree_changes;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା add_renormalize;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା add_resolved;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା pathspec_file_nul;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା include_sparse;
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *pathspec_from_file;

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା chmod_pathspec(ସଂରଚନା repository *repo,
			  ସଂରଚନା pathspec *pathspec,
			  ଅକ୍ଷର flip,
			  ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_only)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;

	ପାଇଁ (size_t i = 0; i < repo->index->cache_nr; i++) {
		ସଂରଚନା cache_entry *ce = repo->index->cache[i];
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା err;

		ଯଦି (!include_sparse &&
		    (ce_skip_worktree(ce) ||
		     !path_in_sparse_checkout(ce->name, repo->index)))
			ଜାରି;

		ଯଦି (pathspec && !ce_path_match(repo->index, ce, pathspec, ଖାଲି))
			ଜାରି;

		ଯଦି (!show_only)
			err = chmod_index_entry(repo->index, ce, flip);
		ଅନ୍ୟଥା
			err = S_ISREG(ce->ce_mode) ? 0 : -1;

		ଯଦି (err < 0)
			ret = error(_("cannot chmod %cx '%s'"), flip, ce->name);
	}

	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା renormalize_tracked_files(ସଂରଚନା repository *repo,
				     ଅଚଳ ସଂରଚନା pathspec *pathspec,
				     ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା retval = 0;

	ପାଇଁ (size_t i = 0; i < repo->index->cache_nr; i++) {
		ସଂରଚନା cache_entry *ce = repo->index->cache[i];

		ଯଦି (!include_sparse &&
		    (ce_skip_worktree(ce) ||
		     !path_in_sparse_checkout(ce->name, repo->index)))
			ଜାରି;
		ଯଦି (ce_stage(ce))
			ଜାରି; /* do not touch unmerged paths */
		ଯଦି (!S_ISREG(ce->ce_mode) && !S_ISLNK(ce->ce_mode))
			ଜାରି; /* do not touch non blobs */
		ଯଦି (pathspec && !ce_path_match(repo->index, ce, pathspec, ଖାଲି))
			ଜାରି;
		retval |= add_file_to_index(repo->index, ce->name,
					    flags | ADD_CACHE_RENORMALIZE);
	}

	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର ଅକ୍ଷର *prune_directory(ସଂରଚନା repository *repo,
			     ସଂରଚନା dir_struct *dir,
			     ସଂରଚନା pathspec *pathspec,
			     ପୂର୍ଣ୍ଣସଂଖ୍ୟା prefix)
{
	ଅକ୍ଷର *seen;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	ସଂରଚନା dir_entry **src, **dst;

	seen = xcalloc(pathspec->nr, 1);

	src = dst = dir->entries;
	i = dir->nr;
	ଯେତେବେଳେ (--i >= 0) {
		ସଂରଚନା dir_entry *entry = *src++;
		ଯଦି (dir_path_match(repo->index, entry, pathspec, prefix, seen))
			*dst++ = entry;
	}
	dir->nr = dst - dir->entries;
	add_pathspec_matches_against_index(pathspec, repo->index, seen,
					   PS_IGNORE_SKIP_WORKTREE);
	ଫେରନ୍ତୁ seen;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା refresh(ସଂରଚନା repository *repo, ପୂର୍ଣ୍ଣସଂଖ୍ୟା verbose, ଅଚଳ ସଂରଚନା pathspec *pathspec)
{
	ଅକ୍ଷର *seen;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, ret = 0;
	ଅକ୍ଷର *skip_worktree_seen = ଖାଲି;
	ସଂରଚନା string_list only_match_skip_worktree = STRING_LIST_INIT_NODUP;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags = REFRESH_IGNORE_SKIP_WORKTREE |
		    (verbose ? REFRESH_IN_PORCELAIN : REFRESH_QUIET);

	seen = xcalloc(pathspec->nr, 1);
	refresh_index(repo->index, flags, pathspec, seen,
		      _("Unstaged changes after refreshing the index:"));
	ପାଇଁ (i = 0; i < pathspec->nr; i++) {
		ଯଦି (!seen[i]) {
			ଅଚଳ ଅକ୍ଷର *path = pathspec->ବସ୍ତୁଗୁଡ଼ିକ[i].original;

			ଯଦି (matches_skip_worktree(pathspec, i, &skip_worktree_seen) ||
			    !path_in_sparse_checkout(path, repo->index)) {
				string_list_append(&only_match_skip_worktree,
						   pathspec->ବସ୍ତୁଗୁଡ଼ିକ[i].original);
			} ଅନ୍ୟଥା {
				die(_("pathspec '%s' did not match any files"),
				    pathspec->ବସ୍ତୁଗୁଡ଼ିକ[i].original);
			}
		}
	}

	ଯଦି (only_match_skip_worktree.nr) {
		advise_on_updating_sparse_paths(&only_match_skip_worktree);
		ret = 1;
	}

	ମୁକ୍ତ(seen);
	ମୁକ୍ତ(skip_worktree_seen);
	string_list_clear(&only_match_skip_worktree, 0);
	ଫେରନ୍ତୁ ret;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା interactive_add(ସଂରଚନା repository *repo,
		    ଅଚଳ ଅକ୍ଷର **argv,
		    ଅଚଳ ଅକ୍ଷର *prefix,
		    ପୂର୍ଣ୍ଣସଂଖ୍ୟା patch, ସଂରଚନା interactive_options *interactive_opts)
{
	ସଂରଚନା pathspec pathspec;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_FULL |
		       PATHSPEC_SYMLINK_LEADING_PATH |
		       PATHSPEC_PREFIX_ORIGIN,
		       prefix, argv);

	ଯଦି (patch)
		ret = !!run_add_p(repo, ADD_P_ADD, interactive_opts, ଖାଲି, &pathspec, 0);
	ଅନ୍ୟଥା
		ret = !!run_add_i(repo, &pathspec, interactive_opts);

	clear_pathspec(&pathspec);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା edit_patch(ସଂରଚନା repository *repo,
		      ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
		      ଅଚଳ ଅକ୍ଷର **argv,
		      ଅଚଳ ଅକ୍ଷର *prefix)
{
	ଅକ୍ଷର *file = repo_git_path(repo, "ADD_EDIT.patch");
	ସଂରଚନା apply_state state;
	ଅଚଳ ଅକ୍ଷର *apply_argv[2];
	ସଂରଚନା rev_info rev;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା out;
	ସଂରଚନା stat st;

	repo_config(repo, git_diff_basic_config, ଖାଲି);

	ଯଦି (repo_read_index(repo) < 0)
		die(_("could not read the index"));

	repo_init_revisions(repo, &rev, prefix);
	rev.diffopt.context = 7;

	argc = setup_revisions(argc, argv, &rev, ଖାଲି);
	rev.diffopt.output_format = DIFF_FORMAT_PATCH;
	rev.diffopt.use_color = GIT_COLOR_NEVER;
	rev.diffopt.flags.ignore_dirty_submodules = 1;
	out = xopen(file, O_CREAT | O_WRONLY | O_TRUNC, 0666);
	rev.diffopt.file = xfdopen(out, "w");
	rev.diffopt.close_file = 1;
	run_diff_files(&rev, 0);

	ଯଦି (launch_editor(file, ଖାଲି, ଖାଲି))
		die(_("editing patch failed"));

	ଯଦି (stat(file, &st))
		die_errno(_("could not stat '%s'"), file);
	ଯଦି (!st.st_size)
		die(_("empty patch. aborted"));

	apply_argv[0] = file;
	apply_argv[1] = ଖାଲି;
	ଯଦି (init_apply_state(&state, repo, ଖାଲି))
		die(_("could not initialize apply state"));
	state.cached = 1;
	ଯଦି (check_apply_state(&state, 0))
		die(_("could not check apply state"));
	ଯଦି (apply_all_patches(&state, 1, apply_argv, APPLY_OPT_RECOUNT))
		die(_("could not apply '%s'"), file);
	clear_apply_state(&state);

	unlink(file);
	ମୁକ୍ତ(file);
	release_revisions(&rev);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର ignore_error[] =
N_("The following paths are ignored by one of your .gitignore files:\n");

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା verbose, show_only, ignored_too, refresh_only;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା ignore_add_errors, intent_to_add, ignore_missing;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା warn_on_embedded_repo = 1;

#define ADDREMOVE_DEFAULT 1
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା addremove = ADDREMOVE_DEFAULT;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା addremove_explicit = -1; /* unspecified */

ସ୍ଥିର ଅକ୍ଷର *chmod_arg;

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା ignore_removal_cb(ଅଚଳ ସଂରଚନା option *opt, ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	BUG_ON_OPT_ARG(arg);

	/* if we are told to ignore, we are not adding removals */
	*(ପୂର୍ଣ୍ଣସଂଖ୍ୟା *)opt->ମୂଲ୍ୟ = !unset ? 0 : 1;
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ସଂରଚନା option builtin_add_options[] = {
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
	  ଖାଲି /* takes no arguments */,
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

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା add_config(ଅଚଳ ଅକ୍ଷର *var, ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ,
		      ଅଚଳ ସଂରଚନା config_context *ctx, ଶୂନ୍ୟ *cb)
{
	ଯଦି (!strcmp(var, "add.ignoreerrors") ||
	    !strcmp(var, "add.ignore-errors")) {
		ignore_add_errors = git_config_bool(var, ମୂଲ୍ୟ);
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (git_color_config(var, ମୂଲ୍ୟ, cb) < 0)
		ଫେରନ୍ତୁ -1;

	ଫେରନ୍ତୁ git_default_config(var, ମୂଲ୍ୟ, ctx, cb);
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର embedded_advice[] = N_(
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

ସ୍ଥିର ଶୂନ୍ୟ check_embedded_repo(ଅଚଳ ଅକ୍ଷର *path)
{
	ସଂରଚନା strbuf name = STRBUF_INIT;
	ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା adviced_on_embedded_repo = 0;

	ଯଦି (!warn_on_embedded_repo)
		ଫେରନ୍ତୁ;
	ଯଦି (!ends_with(path, "/"))
		ଫେରନ୍ତୁ;

	/* Drop trailing slash for aesthetics */
	strbuf_addstr(&name, path);
	strbuf_strip_suffix(&name, "/");

	warning(_("adding embedded git repository: %s"), name.buf);
	ଯଦି (!adviced_on_embedded_repo) {
		advise_if_enabled(ADVICE_ADD_EMBEDDED_REPO,
				  embedded_advice, name.buf, name.buf);
		adviced_on_embedded_repo = 1;
	}

	strbuf_release(&name);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା add_files(ସଂରଚନା repository *repo, ସଂରଚନା dir_struct *dir, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, exit_status = 0;
	ସଂରଚନା string_list matched_sparse_paths = STRING_LIST_INIT_NODUP;

	ଯଦି (dir->ignored_nr) {
		fprintf(stderr, _(ignore_error));
		ପାଇଁ (i = 0; i < dir->ignored_nr; i++)
			fprintf(stderr, "%s\n", dir->ignored[i]->name);
		advise_if_enabled(ADVICE_ADD_IGNORED_FILE,
				  _("Use -f if you really want to add them."));
		exit_status = 1;
	}

	ପାଇଁ (i = 0; i < dir->nr; i++) {
		ଯଦି (!include_sparse &&
		    !path_in_sparse_checkout(dir->entries[i]->name, repo->index)) {
			string_list_append(&matched_sparse_paths,
					   dir->entries[i]->name);
			ଜାରି;
		}
		ଯଦି (add_file_to_index(repo->index, dir->entries[i]->name, flags)) {
			ଯଦି (!ignore_add_errors)
				die(_("adding files failed"));
			exit_status = 1;
		} ଅନ୍ୟଥା {
			check_embedded_repo(dir->entries[i]->name);
		}
	}

	ଯଦି (matched_sparse_paths.nr) {
		advise_on_updating_sparse_paths(&matched_sparse_paths);
		exit_status = 1;
	}

	string_list_clear(&matched_sparse_paths, 0);

	ଫେରନ୍ତୁ exit_status;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା failed_to_add(ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, ଅଚଳ ଅକ୍ଷର *path)
{
	ଯଦି (!(flags & ADD_CACHE_IGNORE_ERRORS))
		die(_("updating file '%s' failed"), path);
	ଫେରନ୍ତୁ 1;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା add_resolved_files(ସଂରଚନା repository *repo,
			      ଅଚଳ ସଂରଚନା pathspec *pathspec,
			      ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	ସଂରଚନା index_state *istate = repo->index;
	ସଂରଚନା string_list unmerged_paths = STRING_LIST_INIT_DUP;
	ସଂରଚନା string_list unresolved_paths = STRING_LIST_INIT_DUP;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା exit_status = 0;
	size_t i;

	ପାଇଁ (i = 0; i < istate->cache_nr; i++) {
		ସଂରଚନା cache_entry *ce = istate->cache[i];
		ଯଦି (!ce_stage(ce))
			ଜାରି;
		ଯଦି (pathspec->nr && !ce_path_match(istate, ce, pathspec, ଖାଲି))
			ଜାରି;
		ଯଦି (!unmerged_paths.nr ||
		    strcmp(unmerged_paths.ବସ୍ତୁଗୁଡ଼ିକ[unmerged_paths.nr - 1].string, ce->name))
			string_list_append(&unmerged_paths, ce->name);
	}

	ଯଦି (!unmerged_paths.nr) {
		string_list_clear(&unmerged_paths, 0);
		ଫେରନ୍ତୁ 0;
	}

	ପାଇଁ (i = 0; i < unmerged_paths.nr; i++) {
		ଅଚଳ ଅକ୍ଷର *path = unmerged_paths.ବସ୍ତୁଗୁଡ଼ିକ[i].string;
		ସଂରଚନା stat st;

		ଯଦି (!lstat(path, &st) && S_ISREG(st.st_mode)) {
			ଯଦି (has_conflict_markers(istate, path))
				string_list_append(&unresolved_paths, path);
		}
	}

	ଯଦି (unresolved_paths.nr) {
		ସଂରଚନା strbuf sb = STRBUF_INIT;
		ପାଇଁ (i = 0; i < unresolved_paths.nr; i++)
			strbuf_addf(&sb, "\t%s\n", unresolved_paths.ବସ୍ତୁଗୁଡ଼ିକ[i].string);
		die(_("the following paths still have conflict markers:\n%s"), sb.buf);
	}

	ପାଇଁ (i = 0; i < unmerged_paths.nr; i++) {
		ଅଚଳ ଅକ୍ଷର *path = unmerged_paths.ବସ୍ତୁଗୁଡ଼ିକ[i].string;
		ସଂରଚନା stat st;

		ଯଦି (lstat(path, &st)) {
			ଯଦି (errno != ENOENT)
				die_errno(_("cannot lstat: '%s'"), path);
			ଯଦି (remove_file_from_index_with_flags(istate, path, flags))
				exit_status = failed_to_add(flags, path);
		} ଅନ୍ୟଥା {
			ଯଦି (add_file_to_index(istate, path, flags))
				exit_status = failed_to_add(flags, path);
		}
	}

	string_list_clear(&unmerged_paths, 0);
	string_list_clear(&unresolved_paths, 0);
	ଫେରନ୍ତୁ exit_status;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_add(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
	    ଅଚଳ ଅକ୍ଷର **argv,
	    ଅଚଳ ଅକ୍ଷର *prefix,
	    ସଂରଚନା repository *repo)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା exit_status = 0;
	ସଂରଚନା pathspec pathspec;
	ସଂରଚନା dir_struct dir = DIR_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା add_new_files;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା require_pathspec;
	ଅକ୍ଷର *seen = ଖାଲି;
	ଅକ୍ଷର *ps_matched = ଖାଲି;
	ସଂରଚନା lock_file lock_file = LOCK_INIT;
	ସଂରଚନା odb_transaction *transaction = ଖାଲି;

	repo_config(repo, add_config, ଖାଲି);

	argc = parse_options(argc, argv, prefix, builtin_add_options,
			  builtin_add_usage, PARSE_OPT_KEEP_ARGV0);

	prepare_repo_settings(repo);
	repo->settings.command_requires_full_index = 0;

	ଯଦି (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	ଯଦି (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	ଯଦି (patch_interactive)
		add_interactive = 1;
	ଯଦି (add_interactive) {
		ଯଦି (show_only)
			die(_("options '%s' and '%s' cannot be used together"), "--dry-run", "--interactive/--patch");
		ଯଦି (pathspec_from_file)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--interactive/--patch");
		exit(interactive_add(repo, argv + 1, prefix, patch_interactive, &interactive_opts));
	} ଅନ୍ୟଥା {
		ଯଦି (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--interactive/--patch");
		ଯଦି (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--interactive/--patch");
		ଯଦି (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--interactive/--patch");
	}

	ଯଦି (edit_interactive) {
		ଯଦି (pathspec_from_file)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--edit");
		ଫେରନ୍ତୁ(edit_patch(repo, argc, argv, prefix));
	}
	argc--;
	argv++;

	ଯଦି (0 <= addremove_explicit)
		addremove = addremove_explicit;
	ଅନ୍ୟଥା ଯଦି (take_worktree_changes && ADDREMOVE_DEFAULT)
		addremove = 0; /* "-u" was given but not "-A" */

	die_for_incompatible_opt3(take_worktree_changes, "-u/--update",
				  0 < addremove_explicit, "-A/--all",
				  add_resolved, "--resolved");

	ଯଦି (!show_only && ignore_missing)
		die(_("the option '%s' requires '%s'"), "--ignore-missing", "--dry-run");

	ଯଦି (chmod_arg && ((chmod_arg[0] != '-' && chmod_arg[0] != '+') ||
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

	ଯଦି (pathspec_from_file) {
		ଯଦି (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_FULL |
				    PATHSPEC_SYMLINK_LEADING_PATH,
				    prefix, pathspec_from_file, pathspec_file_nul);
	} ଅନ୍ୟଥା ଯଦି (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	ଯଦି (require_pathspec && pathspec.nr == 0) {
		fprintf(stderr, _("Nothing specified, nothing added.\n"));
		advise_if_enabled(ADVICE_ADD_EMPTY_PATHSPEC,
				  _("Maybe you wanted to say 'git add .'?"));
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (!take_worktree_changes && !add_resolved &&
	    addremove_explicit < 0 && pathspec.nr)
		/* Turn "git add pathspec..." to "git add -A pathspec..." */
		addremove = 1;

	flags = ((verbose ? ADD_CACHE_VERBOSE : 0) |
		 (show_only ? ADD_CACHE_PRETEND : 0) |
		 (intent_to_add ? ADD_CACHE_INTENT : 0) |
		 (ignore_add_errors ? ADD_CACHE_IGNORE_ERRORS : 0) |
		 (!(addremove || take_worktree_changes)
		  ? ADD_CACHE_IGNORE_REMOVAL : 0));

	ଯଦି (repo_read_index_preload(repo, &pathspec, 0) < 0)
		die(_("index file corrupt"));

	die_in_unpopulated_submodule(repo->index, prefix);
	die_path_inside_submodule(repo->index, &pathspec);

	ଯଦି (add_new_files) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା baselen;

		/* Set up the default git porcelain excludes */
		ଯଦି (!ignored_too) {
			dir.flags |= DIR_COLLECT_IGNORED;
			setup_standard_excludes(&dir);
		}

		/* This picks up the paths that are not tracked */
		baselen = fill_directory(&dir, repo->index, &pathspec);
		ଯଦି (pathspec.nr)
			seen = prune_directory(repo, &dir, &pathspec, baselen);
	}

	ଯଦି (refresh_only) {
		exit_status |= refresh(repo, verbose, &pathspec);
		ଯାଅ finish;
	}

	ଯଦି (pathspec.nr) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
		ଅକ୍ଷର *skip_worktree_seen = ଖାଲି;
		ସଂରଚନା string_list only_match_skip_worktree = STRING_LIST_INIT_NODUP;

		ଯଦି (!seen)
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

		ପାଇଁ (i = 0; i < pathspec.nr; i++) {
			ଅଚଳ ଅକ୍ଷର *path = pathspec.ବସ୍ତୁଗୁଡ଼ିକ[i].match;

			ଯଦି (pathspec.ବସ୍ତୁଗୁଡ଼ିକ[i].magic & PATHSPEC_EXCLUDE)
				ଜାରି;
			ଯଦି (seen[i])
				ଜାରି;

			ଯଦି (!include_sparse &&
			    matches_skip_worktree(&pathspec, i, &skip_worktree_seen)) {
				string_list_append(&only_match_skip_worktree,
						   pathspec.ବସ୍ତୁଗୁଡ଼ିକ[i].original);
				ଜାରି;
			}

			/* Don't complain at 'git add .' on empty repo */
			ଯଦି (!path[0])
				ଜାରି;

			ଯଦି ((pathspec.ବସ୍ତୁଗୁଡ଼ିକ[i].magic & (PATHSPEC_GLOB | PATHSPEC_ICASE)) ||
			    !file_exists(path)) {
				ଯଦି (ignore_missing) {
					ପୂର୍ଣ୍ଣସଂଖ୍ୟା dtype = DT_UNKNOWN;
					ଯଦି (is_excluded(&dir, repo->index, path, &dtype))
						dir_add_ignored(&dir, repo->index,
								path, pathspec.ବସ୍ତୁଗୁଡ଼ିକ[i].len);
				} ଅନ୍ୟଥା
					die(_("pathspec '%s' did not match any files"),
					    pathspec.ବସ୍ତୁଗୁଡ଼ିକ[i].original);
			}
		}


		ଯଦି (only_match_skip_worktree.nr) {
			advise_on_updating_sparse_paths(&only_match_skip_worktree);
			exit_status = 1;
		}

		ମୁକ୍ତ(seen);
		ମୁକ୍ତ(skip_worktree_seen);
		string_list_clear(&only_match_skip_worktree, 0);
	}

	odb_transaction_begin_or_die(repo->objects, &transaction, 0);

	ps_matched = xcalloc(pathspec.nr, 1);
	ଯଦି (add_resolved)
		exit_status |= add_resolved_files(repo, &pathspec, flags);
	ଅନ୍ୟଥା ଯଦି (add_renormalize)
		exit_status |= renormalize_tracked_files(repo, &pathspec, flags);
	ଅନ୍ୟଥା
		exit_status |= add_files_to_cache(repo, prefix,
						  &pathspec, ps_matched,
						  include_sparse, flags, ignored_too);

	ଯଦି (take_worktree_changes && !add_renormalize && !ignore_add_errors &&
	    report_path_error(ps_matched, &pathspec))
		exit(128);

	ଯଦି (add_new_files)
		exit_status |= add_files(repo, &dir, flags);

	ଯଦି (chmod_arg && pathspec.nr)
		exit_status |= chmod_pathspec(repo, &pathspec, chmod_arg[0], show_only);
	odb_transaction_commit_and_finalize_or_die(transaction);

finish:
	ଯଦି (write_locked_index(repo->index, &lock_file,
			       COMMIT_LOCK | SKIP_IF_UNCHANGED))
		die(_("unable to write new index file"));

	ମୁକ୍ତ(ps_matched);
	dir_clear(&dir);
	clear_pathspec(&pathspec);
	ଫେରନ୍ତୁ exit_status;
}
