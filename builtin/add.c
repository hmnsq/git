/*
 * "git add" builtin command
 *
 * Copyright (C) 2006 Linus Torvalds
 */

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "advice.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "lockfile.h"
#अन्तर्भाव "editor.h"
#अन्तर्भाव "dir.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "pathspec.h"
#अन्तर्भाव "object-file.h"
#अन्तर्भाव "odb.h"
#अन्तर्भाव "odb/transaction.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "path.h"
#अन्तर्भाव "preload-index.h"
#अन्तर्भाव "diff.h"
#अन्तर्भाव "read-cache.h"
#अन्तर्भाव "revision.h"
#अन्तर्भाव "submodule.h"
#अन्तर्भाव "add-interactive.h"
#अन्तर्भाव "merge-ll.h"
#अन्तर्भाव "apply.h"

स्थिर ध्रुव वर्ण * ध्रुव builtin_सङ्कलय_usage[] = {
	N_("git add [<options>] [--] <pathspec>..."),
	रिक्त
};
स्थिर पूर्णाङ्क patch_interactive, सङ्कलय_interactive, edit_interactive;
स्थिर संरचना interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
स्थिर पूर्णाङ्क take_worktree_changes;
स्थिर पूर्णाङ्क सङ्कलय_renormalize;
स्थिर पूर्णाङ्क सङ्कलय_resolved;
स्थिर पूर्णाङ्क pathspec_file_nul;
स्थिर पूर्णाङ्क include_sparse;
स्थिर ध्रुव वर्ण *pathspec_from_file;

स्थिर पूर्णाङ्क chmod_pathspec(संरचना repository *repo,
			  संरचना pathspec *pathspec,
			  वर्ण flip,
			  पूर्णाङ्क show_only)
{
	पूर्णाङ्क ret = 0;

	प्रति (आकार_t i = 0; i < repo->सूचकांक->cache_nr; i++) {
		संरचना cache_प्रविष्टिः *ce = repo->सूचकांक->cache[i];
		पूर्णाङ्क err;

		यदि (!include_sparse &&
		    (ce_skip_worktree(ce) ||
		     !मार्गः_अन्तर्_sparse_checkout(ce->name, repo->सूचकांक)))
			अनुवर्त;

		यदि (pathspec && !ce_मार्गः_मेलनम्(repo->सूचकांक, ce, pathspec, रिक्त))
			अनुवर्त;

		यदि (!show_only)
			err = chmod_सूचकांक_प्रविष्टिः(repo->सूचकांक, ce, flip);
		अन्यथा
			err = S_ISREG(ce->ce_बहुलकम्) ? 0 : -1;

		यदि (err < 0)
			ret = error(_("cannot chmod %cx '%s'"), flip, ce->name);
	}

	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क renormalize_tracked_files(संरचना repository *repo,
				     ध्रुव संरचना pathspec *pathspec,
				     पूर्णाङ्क flags)
{
	पूर्णाङ्क retval = 0;

	प्रति (आकार_t i = 0; i < repo->सूचकांक->cache_nr; i++) {
		संरचना cache_प्रविष्टिः *ce = repo->सूचकांक->cache[i];

		यदि (!include_sparse &&
		    (ce_skip_worktree(ce) ||
		     !मार्गः_अन्तर्_sparse_checkout(ce->name, repo->सूचकांक)))
			अनुवर्त;
		यदि (ce_stage(ce))
			अनुवर्त; /* do not touch unmerged paths */
		यदि (!S_ISREG(ce->ce_बहुलकम्) && !S_ISLNK(ce->ce_बहुलकम्))
			अनुवर्त; /* do not touch non blobs */
		यदि (pathspec && !ce_मार्गः_मेलनम्(repo->सूचकांक, ce, pathspec, रिक्त))
			अनुवर्त;
		retval |= सङ्कलय_file_गन्तव्यम्_सूचकांक(repo->सूचकांक, ce->name,
					    flags | सङ्कलय_CACHE_RENORMALIZE);
	}

	प्रत्यागम retval;
}

स्थिर वर्ण *prune_directory(संरचना repository *repo,
			     संरचना dir_struct *dir,
			     संरचना pathspec *pathspec,
			     पूर्णाङ्क उपसर्गः)
{
	वर्ण *आलोकितम्;
	पूर्णाङ्क i;
	संरचना dir_प्रविष्टिः **src, **dst;

	आलोकितम् = xcalloc(pathspec->nr, 1);

	src = dst = dir->entries;
	i = dir->nr;
	यावत् (--i >= 0) {
		संरचना dir_प्रविष्टिः *प्रविष्टिः = *src++;
		यदि (dir_मार्गः_मेलनम्(repo->सूचकांक, प्रविष्टिः, pathspec, उपसर्गः, आलोकितम्))
			*dst++ = प्रविष्टिः;
	}
	dir->nr = dst - dir->entries;
	सङ्कलय_pathspec_matches_against_सूचकांक(pathspec, repo->सूचकांक, आलोकितम्,
					   PS_IGNORE_SKIP_WORKTREE);
	प्रत्यागम आलोकितम्;
}

स्थिर पूर्णाङ्क refresh(संरचना repository *repo, पूर्णाङ्क verbose, ध्रुव संरचना pathspec *pathspec)
{
	वर्ण *आलोकितम्;
	पूर्णाङ्क i, ret = 0;
	वर्ण *skip_worktree_आलोकितम् = रिक्त;
	संरचना string_list only_मेलनम्_skip_worktree = STRING_LIST_INIT_NODUP;
	अचिह्नित पूर्णाङ्क flags = REFRESH_IGNORE_SKIP_WORKTREE |
		    (verbose ? REFRESH_अन्तर्_PORCELAIN : REFRESH_QUIET);

	आलोकितम् = xcalloc(pathspec->nr, 1);
	refresh_सूचकांक(repo->सूचकांक, flags, pathspec, आलोकितम्,
		      _("Unstaged changes after refreshing the index:"));
	प्रति (i = 0; i < pathspec->nr; i++) {
		यदि (!आलोकितम्[i]) {
			ध्रुव वर्ण *मार्गः = pathspec->वस्तूनि[i].original;

			यदि (matches_skip_worktree(pathspec, i, &skip_worktree_आलोकितम्) ||
			    !मार्गः_अन्तर्_sparse_checkout(मार्गः, repo->सूचकांक)) {
				string_list_append(&only_मेलनम्_skip_worktree,
						   pathspec->वस्तूनि[i].original);
			} अन्यथा {
				die(_("pathspec '%s' did not match any files"),
				    pathspec->वस्तूनि[i].original);
			}
		}
	}

	यदि (only_मेलनम्_skip_worktree.nr) {
		advise_on_updating_sparse_paths(&only_मेलनम्_skip_worktree);
		ret = 1;
	}

	मुक्त(आलोकितम्);
	मुक्त(skip_worktree_आलोकितम्);
	string_list_clear(&only_मेलनम्_skip_worktree, 0);
	प्रत्यागम ret;
}

पूर्णाङ्क interactive_सङ्कलय(संरचना repository *repo,
		    ध्रुव वर्ण **argv,
		    ध्रुव वर्ण *उपसर्गः,
		    पूर्णाङ्क patch, संरचना interactive_options *interactive_opts)
{
	संरचना pathspec pathspec;
	पूर्णाङ्क ret;

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_पूर्णम् |
		       PATHSPEC_SYMLINK_LEADING_मार्गः |
		       PATHSPEC_उपसर्गः_ORIGIN,
		       उपसर्गः, argv);

	यदि (patch)
		ret = !!सततम्_सङ्कलय_p(repo, सङ्कलय_P_सङ्कलय, interactive_opts, रिक्त, &pathspec, 0);
	अन्यथा
		ret = !!सततम्_सङ्कलय_i(repo, &pathspec, interactive_opts);

	clear_pathspec(&pathspec);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क edit_patch(संरचना repository *repo,
		      पूर्णाङ्क argc,
		      ध्रुव वर्ण **argv,
		      ध्रुव वर्ण *उपसर्गः)
{
	वर्ण *file = repo_git_मार्गः(repo, "ADD_EDIT.patch");
	संरचना apply_state state;
	ध्रुव वर्ण *apply_argv[2];
	संरचना rev_info rev;
	पूर्णाङ्क बहिर्गमनम्;
	संरचना stat st;

	repo_config(repo, git_diff_basic_config, रिक्त);

	यदि (repo_read_सूचकांक(repo) < 0)
		die(_("could not read the index"));

	repo_init_revisions(repo, &rev, उपसर्गः);
	rev.diffopt.context = 7;

	argc = सज्जता_revisions(argc, argv, &rev, रिक्त);
	rev.diffopt.output_format = DIFF_FORMAT_PATCH;
	rev.diffopt.use_color = GIT_COLOR_NEVER;
	rev.diffopt.flags.ignore_dirty_submodules = 1;
	बहिर्गमनम् = xopen(file, O_CREAT | O_WRONLY | O_TRUNC, 0666);
	rev.diffopt.file = xfdopen(बहिर्गमनम्, "w");
	rev.diffopt.close_file = 1;
	सततम्_diff_files(&rev, 0);

	यदि (launch_editor(file, रिक्त, रिक्त))
		die(_("editing patch failed"));

	यदि (stat(file, &st))
		die_errno(_("could not stat '%s'"), file);
	यदि (!st.st_आकार)
		die(_("empty patch. aborted"));

	apply_argv[0] = file;
	apply_argv[1] = रिक्त;
	यदि (init_apply_state(&state, repo, रिक्त))
		die(_("could not initialize apply state"));
	state.cached = 1;
	यदि (check_apply_state(&state, 0))
		die(_("could not check apply state"));
	यदि (apply_all_patches(&state, 1, apply_argv, APPLY_OPT_RECOUNT))
		die(_("could not apply '%s'"), file);
	clear_apply_state(&state);

	unlink(file);
	मुक्त(file);
	release_revisions(&rev);
	प्रत्यागम 0;
}

स्थिर ध्रुव वर्ण ignore_error[] =
N_("The following paths are ignored by one of your .gitignore files:\n");

स्थिर पूर्णाङ्क verbose, show_only, ignored_too, refresh_only;
स्थिर पूर्णाङ्क ignore_सङ्कलय_errors, intent_गन्तव्यम्_सङ्कलय, ignore_missing;
स्थिर पूर्णाङ्क warn_on_embedded_repo = 1;

#परिभाषय ADDREMOVE_DEFAULT 1
स्थिर पूर्णाङ्क addremove = ADDREMOVE_DEFAULT;
स्थिर पूर्णाङ्क addremove_explicit = -1; /* unspecified */

स्थिर वर्ण *chmod_arg;

स्थिर पूर्णाङ्क ignore_removal_cb(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	BUG_ON_OPT_ARG(arg);

	/* if we are told to ignore, we are not adding removals */
	*(पूर्णाङ्क *)opt->मानम् = !unset ? 0 : 1;
	प्रत्यागम 0;
}

स्थिर संरचना option builtin_सङ्कलय_options[] = {
	OPT_DRY_सततम्(&show_only, N_("dry run")),
	OPT__VERBOSE(&verbose, N_("be verbose")),
	OPT_गणः(""),
	OPT_BOOL('i', "interactive", &सङ्कलय_interactive, N_("interactive picking")),
	OPT_BOOL('p', "patch", &patch_interactive, N_("select hunks interactively")),
	OPT_BOOL(0, "auto-advance", &interactive_opts.auto_advance,
		 N_("auto advance to the next file when selecting hunks interactively")),
	OPT_DIFF_UNIFIED(&interactive_opts.context),
	OPT_DIFF_INTERHUNK_CONTEXT(&interactive_opts.interhunkcontext),
	OPT_BOOL('e', "edit", &edit_interactive, N_("edit current diff and apply")),
	OPT__FORCE(&ignored_too, N_("allow adding otherwise ignored files"), 0),
	OPT_BOOL('u', "update", &take_worktree_changes, N_("update tracked files")),
	OPT_BOOL(0, "renormalize", &सङ्कलय_renormalize, N_("renormalize EOL of tracked files (implies -u)")),
	OPT_BOOL(0, "resolved", &सङ्कलय_resolved, N_("add conflict-resolved tracked files")),
	OPT_BOOL('N', "intent-to-add", &intent_गन्तव्यम्_सङ्कलय, N_("record only the fact that the path will be added later")),
	OPT_BOOL('A', "all", &addremove_explicit, N_("add changes from all tracked and untracked files")),
	OPT_CALLBACK_F(0, "ignore-removal", &addremove_explicit,
	  रिक्त /* takes no arguments */,
	  N_("ignore paths removed in the working tree (same as --no-all)"),
	  PARSE_OPT_NOARG, ignore_removal_cb),
	OPT_BOOL( 0 , "refresh", &refresh_only, N_("don't add, only refresh the index")),
	OPT_BOOL( 0 , "ignore-errors", &ignore_सङ्कलय_errors, N_("just skip files which cannot be added because of errors")),
	OPT_BOOL( 0 , "ignore-missing", &ignore_missing, N_("check if - even missing - files are ignored in dry run")),
	OPT_BOOL(0, "sparse", &include_sparse, N_("allow updating entries outside of the sparse-checkout cone")),
	OPT_STRING(0, "chmod", &chmod_arg, "(+|-)x",
		   N_("override the executable bit of the listed files")),
	OPT_HIDDEN_BOOL(0, "warn-embedded-repo", &warn_on_embedded_repo,
			N_("warn when adding an embedded repository")),
	OPT_PATHSPEC_FROM_FILE(&pathspec_from_file),
	OPT_PATHSPEC_FILE_NUL(&pathspec_file_nul),
	OPT_अन्तः(),
};

स्थिर पूर्णाङ्क सङ्कलय_config(ध्रुव वर्ण *var, ध्रुव वर्ण *मानम्,
		      ध्रुव संरचना config_context *ctx, शून्य *cb)
{
	यदि (!वाक्यं_तुलय(var, "add.ignoreerrors") ||
	    !वाक्यं_तुलय(var, "add.ignore-errors")) {
		ignore_सङ्कलय_errors = git_config_bool(var, मानम्);
		प्रत्यागम 0;
	}

	यदि (git_color_config(var, मानम्, cb) < 0)
		प्रत्यागम -1;

	प्रत्यागम git_default_config(var, मानम्, ctx, cb);
}

स्थिर ध्रुव वर्ण embedded_advice[] = N_(
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

स्थिर शून्य check_embedded_repo(ध्रुव वर्ण *मार्गः)
{
	संरचना strbuf name = STRBUF_INIT;
	स्थिर पूर्णाङ्क adviced_on_embedded_repo = 0;

	यदि (!warn_on_embedded_repo)
		प्रत्यागम;
	यदि (!अन्ताः_with(मार्गः, "/"))
		प्रत्यागम;

	/* Drop trailing slash for aesthetics */
	strbuf_addstr(&name, मार्गः);
	strbuf_strip_suffix(&name, "/");

	warning(_("adding embedded git repository: %s"), name.buf);
	यदि (!adviced_on_embedded_repo) {
		advise_if_enabled(ADVICE_सङ्कलय_EMBEDDED_REPO,
				  embedded_advice, name.buf, name.buf);
		adviced_on_embedded_repo = 1;
	}

	strbuf_release(&name);
}

स्थिर पूर्णाङ्क सङ्कलय_files(संरचना repository *repo, संरचना dir_struct *dir, पूर्णाङ्क flags)
{
	पूर्णाङ्क i, exit_स्थितिः = 0;
	संरचना string_list matched_sparse_paths = STRING_LIST_INIT_NODUP;

	यदि (dir->ignored_nr) {
		fprintf(stderr, _(ignore_error));
		प्रति (i = 0; i < dir->ignored_nr; i++)
			fprintf(stderr, "%s\n", dir->ignored[i]->name);
		advise_if_enabled(ADVICE_सङ्कलय_IGNORED_FILE,
				  _("Use -f if you really want to add them."));
		exit_स्थितिः = 1;
	}

	प्रति (i = 0; i < dir->nr; i++) {
		यदि (!include_sparse &&
		    !मार्गः_अन्तर्_sparse_checkout(dir->entries[i]->name, repo->सूचकांक)) {
			string_list_append(&matched_sparse_paths,
					   dir->entries[i]->name);
			अनुवर्त;
		}
		यदि (सङ्कलय_file_गन्तव्यम्_सूचकांक(repo->सूचकांक, dir->entries[i]->name, flags)) {
			यदि (!ignore_सङ्कलय_errors)
				die(_("adding files failed"));
			exit_स्थितिः = 1;
		} अन्यथा {
			check_embedded_repo(dir->entries[i]->name);
		}
	}

	यदि (matched_sparse_paths.nr) {
		advise_on_updating_sparse_paths(&matched_sparse_paths);
		exit_स्थितिः = 1;
	}

	string_list_clear(&matched_sparse_paths, 0);

	प्रत्यागम exit_स्थितिः;
}

स्थिर पूर्णाङ्क failed_गन्तव्यम्_सङ्कलय(पूर्णाङ्क flags, ध्रुव वर्ण *मार्गः)
{
	यदि (!(flags & सङ्कलय_CACHE_IGNORE_ERRORS))
		die(_("updating file '%s' failed"), मार्गः);
	प्रत्यागम 1;
}

स्थिर पूर्णाङ्क सङ्कलय_resolved_files(संरचना repository *repo,
			      ध्रुव संरचना pathspec *pathspec,
			      पूर्णाङ्क flags)
{
	संरचना सूचकांक_state *istate = repo->सूचकांक;
	संरचना string_list unmerged_paths = STRING_LIST_INIT_DUP;
	संरचना string_list unresolved_paths = STRING_LIST_INIT_DUP;
	पूर्णाङ्क exit_स्थितिः = 0;
	आकार_t i;

	प्रति (i = 0; i < istate->cache_nr; i++) {
		संरचना cache_प्रविष्टिः *ce = istate->cache[i];
		यदि (!ce_stage(ce))
			अनुवर्त;
		यदि (pathspec->nr && !ce_मार्गः_मेलनम्(istate, ce, pathspec, रिक्त))
			अनुवर्त;
		यदि (!unmerged_paths.nr ||
		    वाक्यं_तुलय(unmerged_paths.वस्तूनि[unmerged_paths.nr - 1].string, ce->name))
			string_list_append(&unmerged_paths, ce->name);
	}

	यदि (!unmerged_paths.nr) {
		string_list_clear(&unmerged_paths, 0);
		प्रत्यागम 0;
	}

	प्रति (i = 0; i < unmerged_paths.nr; i++) {
		ध्रुव वर्ण *मार्गः = unmerged_paths.वस्तूनि[i].string;
		संरचना stat st;

		यदि (!lstat(मार्गः, &st) && S_ISREG(st.st_बहुलकम्)) {
			यदि (has_conflict_markers(istate, मार्गः))
				string_list_append(&unresolved_paths, मार्गः);
		}
	}

	यदि (unresolved_paths.nr) {
		संरचना strbuf sb = STRBUF_INIT;
		प्रति (i = 0; i < unresolved_paths.nr; i++)
			strbuf_addf(&sb, "\t%s\n", unresolved_paths.वस्तूनि[i].string);
		die(_("the following paths still have conflict markers:\n%s"), sb.buf);
	}

	प्रति (i = 0; i < unmerged_paths.nr; i++) {
		ध्रुव वर्ण *मार्गः = unmerged_paths.वस्तूनि[i].string;
		संरचना stat st;

		यदि (lstat(मार्गः, &st)) {
			यदि (errno != ENOENT)
				die_errno(_("cannot lstat: '%s'"), मार्गः);
			यदि (remove_file_from_सूचकांक_with_flags(istate, मार्गः, flags))
				exit_स्थितिः = failed_गन्तव्यम्_सङ्कलय(flags, मार्गः);
		} अन्यथा {
			यदि (सङ्कलय_file_गन्तव्यम्_सूचकांक(istate, मार्गः, flags))
				exit_स्थितिः = failed_गन्तव्यम्_सङ्कलय(flags, मार्गः);
		}
	}

	string_list_clear(&unmerged_paths, 0);
	string_list_clear(&unresolved_paths, 0);
	प्रत्यागम exit_स्थितिः;
}

पूर्णाङ्क cmd_सङ्कलय(पूर्णाङ्क argc,
	    ध्रुव वर्ण **argv,
	    ध्रुव वर्ण *उपसर्गः,
	    संरचना repository *repo)
{
	पूर्णाङ्क exit_स्थितिः = 0;
	संरचना pathspec pathspec;
	संरचना dir_struct dir = DIR_INIT;
	पूर्णाङ्क flags;
	पूर्णाङ्क सङ्कलय_new_files;
	पूर्णाङ्क require_pathspec;
	वर्ण *आलोकितम् = रिक्त;
	वर्ण *ps_matched = रिक्त;
	संरचना तालकम्_file तालकम्_file = तालकम्_INIT;
	संरचना odb_transaction *transaction = रिक्त;

	repo_config(repo, सङ्कलय_config, रिक्त);

	argc = parse_options(argc, argv, उपसर्गः, builtin_सङ्कलय_options,
			  builtin_सङ्कलय_usage, PARSE_OPT_KEEP_ARGV0);

	prepare_repo_settings(repo);
	repo->settings.command_requires_पूर्णम्_सूचकांक = 0;

	यदि (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	यदि (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	यदि (patch_interactive)
		सङ्कलय_interactive = 1;
	यदि (सङ्कलय_interactive) {
		यदि (show_only)
			die(_("options '%s' and '%s' cannot be used together"), "--dry-run", "--interactive/--patch");
		यदि (pathspec_from_file)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--interactive/--patch");
		निर्गच्छ(interactive_सङ्कलय(repo, argv + 1, उपसर्गः, patch_interactive, &interactive_opts));
	} अन्यथा {
		यदि (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--interactive/--patch");
		यदि (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--interactive/--patch");
		यदि (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--interactive/--patch");
	}

	यदि (edit_interactive) {
		यदि (pathspec_from_file)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--edit");
		प्रत्यागम(edit_patch(repo, argc, argv, उपसर्गः));
	}
	argc--;
	argv++;

	यदि (0 <= addremove_explicit)
		addremove = addremove_explicit;
	अन्यथा यदि (take_worktree_changes && ADDREMOVE_DEFAULT)
		addremove = 0; /* "-u" was given but not "-A" */

	die_for_incompatible_opt3(take_worktree_changes, "-u/--update",
				  0 < addremove_explicit, "-A/--all",
				  सङ्कलय_resolved, "--resolved");

	यदि (!show_only && ignore_missing)
		die(_("the option '%s' requires '%s'"), "--ignore-missing", "--dry-run");

	यदि (chmod_arg && ((chmod_arg[0] != '-' && chmod_arg[0] != '+') ||
			  chmod_arg[1] != 'x' || chmod_arg[2]))
		die(_("--chmod param '%s' must be either -x or +x"), chmod_arg);

	सङ्कलय_new_files = !take_worktree_changes && !refresh_only &&
			!सङ्कलय_renormalize && !सङ्कलय_resolved;
	require_pathspec = !(take_worktree_changes ||
			     (0 < addremove_explicit) ||
			     सङ्कलय_resolved);

	repo_hold_locked_सूचकांक(repo, &तालकम्_file, तालकम्_DIE_ON_ERROR);

	/*
	 * Check the "pathspec '%s' did not match any files" block
	 * below before enabling new magic.
	 */
	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_पूर्णम् |
		       PATHSPEC_SYMLINK_LEADING_मार्गः,
		       उपसर्गः, argv);

	यदि (pathspec_from_file) {
		यदि (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_पूर्णम् |
				    PATHSPEC_SYMLINK_LEADING_मार्गः,
				    उपसर्गः, pathspec_from_file, pathspec_file_nul);
	} अन्यथा यदि (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	यदि (require_pathspec && pathspec.nr == 0) {
		fprintf(stderr, _("Nothing specified, nothing added.\n"));
		advise_if_enabled(ADVICE_सङ्कलय_रिक्तानि_PATHSPEC,
				  _("Maybe you wanted to say 'git add .'?"));
		प्रत्यागम 0;
	}

	यदि (!take_worktree_changes && !सङ्कलय_resolved &&
	    addremove_explicit < 0 && pathspec.nr)
		/* Turn "git add pathspec..." to "git add -A pathspec..." */
		addremove = 1;

	flags = ((verbose ? सङ्कलय_CACHE_VERBOSE : 0) |
		 (show_only ? सङ्कलय_CACHE_PRETEND : 0) |
		 (intent_गन्तव्यम्_सङ्कलय ? सङ्कलय_CACHE_INTENT : 0) |
		 (ignore_सङ्कलय_errors ? सङ्कलय_CACHE_IGNORE_ERRORS : 0) |
		 (!(addremove || take_worktree_changes)
		  ? सङ्कलय_CACHE_IGNORE_REMOVAL : 0));

	यदि (repo_read_सूचकांक_preload(repo, &pathspec, 0) < 0)
		die(_("index file corrupt"));

	die_अन्तर्_unpopulated_submodule(repo->सूचकांक, उपसर्गः);
	die_मार्गः_inside_submodule(repo->सूचकांक, &pathspec);

	यदि (सङ्कलय_new_files) {
		पूर्णाङ्क baselen;

		/* Set up the default git porcelain excludes */
		यदि (!ignored_too) {
			dir.flags |= DIR_COLLECT_IGNORED;
			सज्जता_standard_excludes(&dir);
		}

		/* This picks up the paths that are not tracked */
		baselen = fill_directory(&dir, repo->सूचकांक, &pathspec);
		यदि (pathspec.nr)
			आलोकितम् = prune_directory(repo, &dir, &pathspec, baselen);
	}

	यदि (refresh_only) {
		exit_स्थितिः |= refresh(repo, verbose, &pathspec);
		गच्छ finish;
	}

	यदि (pathspec.nr) {
		पूर्णाङ्क i;
		वर्ण *skip_worktree_आलोकितम् = रिक्त;
		संरचना string_list only_मेलनम्_skip_worktree = STRING_LIST_INIT_NODUP;

		यदि (!आलोकितम्)
			आलोकितम् = find_pathspecs_matching_against_सूचकांक(&pathspec,
					repo->सूचकांक, PS_IGNORE_SKIP_WORKTREE);

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

		प्रति (i = 0; i < pathspec.nr; i++) {
			ध्रुव वर्ण *मार्गः = pathspec.वस्तूनि[i].मेलनम्;

			यदि (pathspec.वस्तूनि[i].magic & PATHSPEC_EXCLUDE)
				अनुवर्त;
			यदि (आलोकितम्[i])
				अनुवर्त;

			यदि (!include_sparse &&
			    matches_skip_worktree(&pathspec, i, &skip_worktree_आलोकितम्)) {
				string_list_append(&only_मेलनम्_skip_worktree,
						   pathspec.वस्तूनि[i].original);
				अनुवर्त;
			}

			/* Don't complain at 'git add .' on empty repo */
			यदि (!मार्गः[0])
				अनुवर्त;

			यदि ((pathspec.वस्तूनि[i].magic & (PATHSPEC_GLOB | PATHSPEC_ICASE)) ||
			    !file_exists(मार्गः)) {
				यदि (ignore_missing) {
					पूर्णाङ्क dtype = DT_UNKNOWN;
					यदि (is_excluded(&dir, repo->सूचकांक, मार्गः, &dtype))
						dir_सङ्कलय_ignored(&dir, repo->सूचकांक,
								मार्गः, pathspec.वस्तूनि[i].len);
				} अन्यथा
					die(_("pathspec '%s' did not match any files"),
					    pathspec.वस्तूनि[i].original);
			}
		}


		यदि (only_मेलनम्_skip_worktree.nr) {
			advise_on_updating_sparse_paths(&only_मेलनम्_skip_worktree);
			exit_स्थितिः = 1;
		}

		मुक्त(आलोकितम्);
		मुक्त(skip_worktree_आलोकितम्);
		string_list_clear(&only_मेलनम्_skip_worktree, 0);
	}

	odb_transaction_begin_or_die(repo->objects, &transaction, 0);

	ps_matched = xcalloc(pathspec.nr, 1);
	यदि (सङ्कलय_resolved)
		exit_स्थितिः |= सङ्कलय_resolved_files(repo, &pathspec, flags);
	अन्यथा यदि (सङ्कलय_renormalize)
		exit_स्थितिः |= renormalize_tracked_files(repo, &pathspec, flags);
	अन्यथा
		exit_स्थितिः |= सङ्कलय_files_गन्तव्यम्_cache(repo, उपसर्गः,
						  &pathspec, ps_matched,
						  include_sparse, flags, ignored_too);

	यदि (take_worktree_changes && !सङ्कलय_renormalize && !ignore_सङ्कलय_errors &&
	    report_मार्गः_error(ps_matched, &pathspec))
		निर्गच्छ(128);

	यदि (सङ्कलय_new_files)
		exit_स्थितिः |= सङ्कलय_files(repo, &dir, flags);

	यदि (chmod_arg && pathspec.nr)
		exit_स्थितिः |= chmod_pathspec(repo, &pathspec, chmod_arg[0], show_only);
	odb_transaction_commit_and_finalize_or_die(transaction);

finish:
	यदि (write_locked_सूचकांक(repo->सूचकांक, &तालकम्_file,
			       COMMIT_तालकम् | SKIP_IF_UNCHANGED))
		die(_("unable to write new index file"));

	मुक्त(ps_matched);
	dir_clear(&dir);
	clear_pathspec(&pathspec);
	प्रत्यागम exit_स्थितिः;
}
