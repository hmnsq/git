/*
 * Builtin "git commit"
 *
 * Copyright (c) 2007 Kristian Høgsberg <krh@redhat.com>
 * Based on git-commit.sh by Junio C Hamano and Linus Torvalds
 */

#परिभाषय USE_THE_REPOSITORY_VARIABLE
#परिभाषय DISABLE_SIGN_COMPARE_WARNINGS

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "advice.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "lockfile.h"
#अन्तर्भाव "cache-tree.h"
#अन्तर्भाव "color.h"
#अन्तर्भाव "dir.h"
#अन्तर्भाव "editor.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "diff.h"
#अन्तर्भाव "commit.h"
#अन्तर्भाव "add-interactive.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "revision.h"
#अन्तर्भाव "wt-status.h"
#अन्तर्भाव "run-command.h"
#अन्तर्भाव "strbuf.h"
#अन्तर्भाव "object-name.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "path.h"
#अन्तर्भाव "preload-index.h"
#अन्तर्भाव "read-cache.h"
#अन्तर्भाव "repository.h"
#अन्तर्भाव "string-list.h"
#अन्तर्भाव "rerere.h"
#अन्तर्भाव "unpack-trees.h"
#अन्तर्भाव "column.h"
#अन्तर्भाव "sequencer.h"
#अन्तर्भाव "sparse-index.h"
#अन्तर्भाव "mailmap.h"
#अन्तर्भाव "help.h"
#अन्तर्भाव "commit-reach.h"
#अन्तर्भाव "commit-graph.h"
#अन्तर्भाव "pretty.h"
#अन्तर्भाव "trailer.h"

स्थिर ध्रुव वर्ण * ध्रुव builtin_commit_usage[] = {
	N_("git commit [-a | --interactive | --patch] [-s] [-v] [-u[<mode>]] [--amend]\n"
	   "           [--dry-run] [(-c | -C | --squash) <commit> | --fixup [(amend|reword):]<commit>]\n"
	   "           [-F <file> | -m <msg>] [--reset-author] [--allow-empty]\n"
	   "           [--allow-empty-message] [--no-verify] [-e] [--author=<author>]\n"
	   "           [--date=<date>] [--cleanup=<mode>] [--[no-]status]\n"
	   "           [-i | -o] [--pathspec-from-file=<file> [--pathspec-file-nul]]\n"
	   "           [(--trailer <token>[(=|:)<value>])...] [-S[<keyid>]]\n"
	   "           [--] [<pathspec>...]"),
	रिक्त
};

स्थिर ध्रुव वर्ण * ध्रुव builtin_स्थितिः_usage[] = {
	N_("git status [<options>] [--] [<pathspec>...]"),
	रिक्त
};

स्थिर ध्रुव वर्ण रिक्तानि_amend_advice[] =
N_("You asked to amend the most recent commit, but doing so would make\n"
"it empty. You can repeat your command with --allow-empty, or you can\n"
"remove the commit entirely with \"git reset HEAD^\".\n");

स्थिर ध्रुव वर्ण रिक्तानि_cherry_pick_advice[] =
N_("The previous cherry-pick is now empty, possibly due to conflict resolution.\n"
"If you wish to commit it anyway, use:\n"
"\n"
"    git commit --allow-empty\n"
"\n");

स्थिर ध्रुव वर्ण रिक्तानि_rebase_pick_advice[] =
N_("Otherwise, please use 'git rebase --skip'\n");

स्थिर ध्रुव वर्ण रिक्तानि_cherry_pick_advice_single[] =
N_("Otherwise, please use 'git cherry-pick --skip'\n");

स्थिर ध्रुव वर्ण रिक्तानि_cherry_pick_advice_multi[] =
N_("and then use:\n"
"\n"
"    git cherry-pick --continue\n"
"\n"
"to resume cherry-picking the remaining commits.\n"
"If you wish to skip this commit, use:\n"
"\n"
"    git cherry-pick --skip\n"
"\n");

स्थिर ध्रुव वर्ण *color_स्थितिः_slots[] = {
	[WT_स्थितिः_शीर्षकम्]	  = "header",
	[WT_स्थितिः_UPDATED]	  = "updated",
	[WT_स्थितिः_परिवर्तितम्]	  = "changed",
	[WT_स्थितिः_UNTRACKED]	  = "untracked",
	[WT_स्थितिः_NOBRANCH]	  = "noBranch",
	[WT_स्थितिः_UNMERGED]	  = "unmerged",
	[WT_स्थितिः_LOCAL_BRANCH]  = "localBranch",
	[WT_स्थितिः_REMOTE_BRANCH] = "remoteBranch",
	[WT_स्थितिः_ONBRANCH]	  = "branch",
};

स्थिर ध्रुव वर्ण *use_सन्देशः_बफर;
स्थिर संरचना तालकम्_file सूचकांक_तालकम्; /* real index */
स्थिर संरचना तालकम्_file false_तालकम्; /* used only for partial commits */
स्थिर गणना {
	COMMIT_AS_IS = 1,
	COMMIT_प्रसामान्यम्,
	COMMIT_PARTIAL
} commit_style;

स्थिर ध्रुव वर्ण *force_author;
स्थिर वर्ण *logfile;
स्थिर वर्ण *template_file;
/*
 * The _message variables are commit names from which to take
 * the commit message and/or authorship.
 */
स्थिर ध्रुव वर्ण *author_सन्देशः, *author_सन्देशः_बफर;
स्थिर ध्रुव वर्ण *edit_सन्देशः, *use_सन्देशः;
स्थिर वर्ण *fixup_सन्देशः, *fixup_commit, *squash_सन्देशः;
स्थिर ध्रुव वर्ण *fixup_उपसर्गः;
स्थिर पूर्णाङ्क all, also, interactive, patch_interactive, only, amend, signoff;
स्थिर संरचना interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
स्थिर पूर्णाङ्क edit_flag = -1; /* unspecified */
स्थिर पूर्णाङ्क quiet, verbose, no_verify, allow_रिक्तानि, dry_सततम्, renew_authorship;
स्थिर पूर्णाङ्क config_commit_verbose = -1; /* unspecified */
स्थिर पूर्णाङ्क no_post_rewrite, allow_रिक्तानि_सन्देशः, pathspec_file_nul;
स्थिर ध्रुव वर्ण *untracked_files_arg, *force_date, *ignore_submodule_arg, *ignored_arg;
स्थिर ध्रुव वर्ण *sign_commit, *pathspec_from_file;
स्थिर संरचना strvec trailer_तर्काः = STRVEC_INIT;

/*
 * The default commit message cleanup mode will remove the lines
 * beginning with # (shell comments) and leading and trailing
 * whitespaces (empty lines or containing only whitespaces)
 * if editor is used, and only the whitespaces if the message
 * is specified explicitly.
 */
स्थिर गणना commit_सन्देशः_cleanup_बहुलकम् cleanup_बहुलकम्;
स्थिर वर्ण *cleanup_config;

स्थिर गणना commit_whence whence;
स्थिर पूर्णाङ्क use_editor = 1, include_स्थितिः = 1;
स्थिर पूर्णाङ्क have_option_m;
स्थिर संरचना strbuf सन्देशः_2 = STRBUF_INIT;

स्थिर गणना wt_स्थितिः_format स्थितिः_format = स्थितिः_FORMAT_UNSPECIFIED;

स्थिर पूर्णाङ्क opt_parse_porcelain(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	गणना wt_स्थितिः_format *मानम् = (गणना wt_स्थितिः_format *)opt->मानम्;
	यदि (unset)
		*मानम् = स्थितिः_FORMAT_NONE;
	अन्यथा यदि (!arg)
		*मानम् = स्थितिः_FORMAT_PORCELAIN;
	अन्यथा यदि (!वाक्यं_तुलय(arg, "v1") || !वाक्यं_तुलय(arg, "1"))
		*मानम् = स्थितिः_FORMAT_PORCELAIN;
	अन्यथा यदि (!वाक्यं_तुलय(arg, "v2") || !वाक्यं_तुलय(arg, "2"))
		*मानम् = स्थितिः_FORMAT_PORCELAIN_V2;
	अन्यथा
		die("unsupported porcelain version '%s'", arg);

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क opt_parse_m(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना strbuf *buf = opt->मानम्;
	यदि (unset) {
		have_option_m = 0;
		strbuf_setlen(buf, 0);
	} अन्यथा {
		have_option_m = 1;
		यदि (buf->len)
			strbuf_addch(buf, '\n');
		strbuf_addstr(buf, arg);
		strbuf_complete_पङ्क्तिः(buf);
	}
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क opt_parse_rename_score(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	ध्रुव वर्ण **मानम् = opt->मानम्;

	BUG_ON_OPT_NEG(unset);

	यदि (arg != रिक्त && *arg == '=')
		arg = arg + 1;

	*मानम् = arg;
	प्रत्यागम 0;
}

स्थिर शून्य determine_whence(संरचना wt_स्थितिः *s)
{
	यदि (file_exists(git_मार्गः_merge_शीर्ष(the_repository)))
		whence = FROM_MERGE;
	अन्यथा यदि (!sequencer_determine_whence(the_repository, &whence))
		whence = FROM_COMMIT;
	यदि (s)
		s->whence = whence;
}

स्थिर शून्य स्थितिः_init_config(संरचना wt_स्थितिः *s, config_fn_t fn)
{
	wt_स्थितिः_prepare(the_repository, s);
	init_diff_ui_defaults();
	repo_config(the_repository, fn, s);
	determine_whence(s);
	s->hints = advice_enabled(ADVICE_स्थितिः_HINTS); /* must come after repo_config() */
}

स्थिर शून्य rollback_सूचकांक_files(शून्य)
{
	विकल्प (commit_style) {
	पक्ष COMMIT_AS_IS:
		भङ्ग; /* nothing to do */
	पक्ष COMMIT_प्रसामान्यम्:
		rollback_तालकम्_file(&सूचकांक_तालकम्);
		भङ्ग;
	पक्ष COMMIT_PARTIAL:
		rollback_तालकम्_file(&सूचकांक_तालकम्);
		rollback_तालकम्_file(&false_तालकम्);
		भङ्ग;
	}
}

स्थिर पूर्णाङ्क commit_सूचकांक_files(शून्य)
{
	पूर्णाङ्क err = 0;

	विकल्प (commit_style) {
	पक्ष COMMIT_AS_IS:
		भङ्ग; /* nothing to do */
	पक्ष COMMIT_प्रसामान्यम्:
		err = commit_तालकम्_file(&सूचकांक_तालकम्);
		भङ्ग;
	पक्ष COMMIT_PARTIAL:
		err = commit_तालकम्_file(&सूचकांक_तालकम्);
		rollback_तालकम्_file(&false_तालकम्);
		भङ्ग;
	}

	प्रत्यागम err;
}

/*
 * Take a union of paths in the index and the named tree (typically, "HEAD"),
 * and return the paths that match the given pattern in list.
 */
स्थिर पूर्णाङ्क list_paths(संरचना string_list *list, ध्रुव वर्ण *with_वृक्षः,
		      ध्रुव संरचना pathspec *आदर्शः)
{
	पूर्णाङ्क i, ret;
	वर्ण *m;

	यदि (!आदर्शः->nr)
		प्रत्यागम 0;

	m = xcalloc(1, आदर्शः->nr);

	यदि (with_वृक्षः) {
		वर्ण *max_उपसर्गः = common_उपसर्गः(आदर्शः);
		overlay_वृक्षः_on_सूचकांक(the_repository->सूचकांक, with_वृक्षः, max_उपसर्गः);
		मुक्त(max_उपसर्गः);
	}

	/* TODO: audit for interaction with sparse-index. */
	ensure_पूर्णम्_सूचकांक(the_repository->सूचकांक);
	प्रति (i = 0; i < the_repository->सूचकांक->cache_nr; i++) {
		ध्रुव संरचना cache_प्रविष्टिः *ce = the_repository->सूचकांक->cache[i];
		संरचना string_list_item *item;

		यदि (ce->ce_flags & CE_UPDATE)
			अनुवर्त;
		यदि (!ce_मार्गः_मेलनम्(the_repository->सूचकांक, ce, आदर्शः, m))
			अनुवर्त;
		item = string_list_निवेशय(list, ce->name);
		यदि (ce_skip_worktree(ce))
			item->util = item; /* better a valid pointer than a fake one */
	}

	ret = report_मार्गः_error(m, आदर्शः);
	मुक्त(m);
	प्रत्यागम ret;
}

स्थिर शून्य सङ्कलय_remove_files(संरचना string_list *list)
{
	पूर्णाङ्क i;
	प्रति (i = 0; i < list->nr; i++) {
		संरचना stat st;
		संरचना string_list_item *p = &(list->वस्तूनि[i]);

		/* p->util is skip-worktree */
		यदि (p->util)
			अनुवर्त;

		यदि (!lstat(p->string, &st)) {
			यदि (सङ्कलय_गन्तव्यम्_सूचकांक(the_repository->सूचकांक, p->string, &st, 0))
				die(_("updating files failed"));
		} अन्यथा
			remove_file_from_सूचकांक(the_repository->सूचकांक, p->string);
	}
}

स्थिर शून्य create_आधारः_सूचकांक(ध्रुव संरचना commit *current_शीर्ष)
{
	संरचना वृक्षः *वृक्षः;
	संरचना unpack_trees_options opts;
	संरचना वृक्षः_desc t;

	यदि (!current_शीर्ष) {
		discard_सूचकांक(the_repository->सूचकांक);
		प्रत्यागम;
	}

	स्मृतिं_पूरय(&opts, 0, परिमाण(opts));
	opts.शीर्ष_idx = 1;
	opts.सूचकांक_only = 1;
	opts.merge = 1;
	opts.src_सूचकांक = the_repository->सूचकांक;
	opts.dst_सूचकांक = the_repository->सूचकांक;

	opts.fn = oneway_merge;
	वृक्षः = repo_parse_वृक्षः_परोक्षम्(the_repository,
					&current_शीर्ष->object.oid);
	यदि (!वृक्षः)
		die(_("failed to unpack HEAD tree object"));
	यदि (repo_parse_वृक्षः(the_repository, वृक्षः) < 0)
		निर्गच्छ(128);
	init_वृक्षः_desc(&t, &वृक्षः->object.oid, वृक्षः->बफर, वृक्षः->आकार);
	यदि (unpack_trees(1, &t, &opts))
		निर्गच्छ(128); /* We've already reported the error, finish dying */
}

स्थिर शून्य refresh_cache_or_die(पूर्णाङ्क refresh_flags)
{
	/*
	 * refresh_flags contains REFRESH_QUIET, so the only errors
	 * are for unmerged entries.
	 */
	यदि (refresh_सूचकांक(the_repository->सूचकांक, refresh_flags | REFRESH_अन्तर्_PORCELAIN, रिक्त, रिक्त, रिक्त))
		die_resolve_conflict("commit");
}

स्थिर ध्रुव वर्ण *prepare_सूचकांक(ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
				 ध्रुव संरचना commit *current_शीर्ष, पूर्णाङ्क is_स्थितिः)
{
	संरचना string_list partial = STRING_LIST_INIT_DUP;
	संरचना pathspec pathspec;
	पूर्णाङ्क refresh_flags = REFRESH_QUIET;
	ध्रुव वर्ण *ret;
	वर्ण *मार्गः = रिक्त;

	यदि (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	यदि (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	यदि (is_स्थितिः)
		refresh_flags |= REFRESH_UNMERGED;
	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_पूर्णम्,
		       उपसर्गः, argv);

	यदि (pathspec_from_file) {
		यदि (interactive)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--interactive/--patch");

		यदि (all)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "-a");

		यदि (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_पूर्णम्,
				    उपसर्गः, pathspec_from_file, pathspec_file_nul);
	} अन्यथा यदि (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	यदि (!pathspec.nr && (also || (only && !allow_रिक्तानि &&
	    (!amend || (fixup_सन्देशः && वाक्यं_तुलय(fixup_उपसर्गः, "amend"))))))
		die(_("No paths with --include/--only does not make sense."));

	यदि (repo_read_सूचकांक_preload(the_repository, &pathspec, 0) < 0)
		die(_("index file corrupt"));

	यदि (interactive) {
		वर्ण *old_सूचकांक_env = रिक्त, *old_repo_सूचकांक_file;
		repo_hold_locked_सूचकांक(the_repository, &सूचकांक_तालकम्,
				       तालकम्_DIE_ON_ERROR);

		refresh_cache_or_die(refresh_flags);

		यदि (write_locked_सूचकांक(the_repository->सूचकांक, &सूचकांक_तालकम्, 0))
			die(_("unable to create temporary index"));

		old_repo_सूचकांक_file = the_repository->सूचकांक_file;
		the_repository->सूचकांक_file =
			(वर्ण *)get_तालकम्_file_मार्गः(&सूचकांक_तालकम्);
		old_सूचकांक_env = xstrdup_or_null(getenv(सूचकांक_ENVIRONMENT));
		setenv(सूचकांक_ENVIRONMENT, the_repository->सूचकांक_file, 1);

		यदि (interactive_सङ्कलय(the_repository, argv, उपसर्गः, patch_interactive, &interactive_opts) != 0)
			die(_("interactive add failed"));

		the_repository->सूचकांक_file = old_repo_सूचकांक_file;
		यदि (old_सूचकांक_env && *old_सूचकांक_env)
			setenv(सूचकांक_ENVIRONMENT, old_सूचकांक_env, 1);
		अन्यथा
			unsetenv(सूचकांक_ENVIRONMENT);
		FREE_AND_NULL(old_सूचकांक_env);

		discard_सूचकांक(the_repository->सूचकांक);
		read_सूचकांक_from(the_repository->सूचकांक, get_तालकम्_file_मार्गः(&सूचकांक_तालकम्),
				repo_get_git_dir(the_repository));
		यदि (cache_वृक्षः_update(the_repository->सूचकांक, WRITE_वृक्षः_SILENT) == 0) {
			यदि (reopen_तालकम्_file(&सूचकांक_तालकम्) < 0)
				die(_("unable to write index file"));
			यदि (write_locked_सूचकांक(the_repository->सूचकांक, &सूचकांक_तालकम्, 0))
				die(_("unable to update temporary index"));
		} अन्यथा
			warning(_("Failed to update main cache tree"));

		commit_style = COMMIT_प्रसामान्यम्;
		ret = get_तालकम्_file_मार्गः(&सूचकांक_तालकम्);
		गच्छ बहिर्गमनम्;
	} अन्यथा {
		यदि (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--interactive/--patch");
		यदि (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--interactive/--patch");
	}

	/*
	 * Non partial, non as-is commit.
	 *
	 * (1) get the real index;
	 * (2) update the_index as necessary;
	 * (3) write the_index out to the real index (still locked);
	 * (4) return the name of the locked index file.
	 *
	 * The caller should run hooks on the locked real index, and
	 * (A) if all goes well, commit the real index;
	 * (B) on failure, rollback the real index.
	 */
	यदि (all || (also && pathspec.nr)) {
		वर्ण *ps_matched = xcalloc(pathspec.nr, 1);
		repo_hold_locked_सूचकांक(the_repository, &सूचकांक_तालकम्,
				       तालकम्_DIE_ON_ERROR);
		सङ्कलय_files_गन्तव्यम्_cache(the_repository, also ? उपसर्गः : रिक्त,
				   &pathspec, ps_matched, 0, 0, 0 );
		यदि (!all && report_मार्गः_error(ps_matched, &pathspec))
			निर्गच्छ(128);

		refresh_cache_or_die(refresh_flags);
		cache_वृक्षः_update(the_repository->सूचकांक, WRITE_वृक्षः_SILENT);
		यदि (write_locked_सूचकांक(the_repository->सूचकांक, &सूचकांक_तालकम्, 0))
			die(_("unable to write new index file"));
		commit_style = COMMIT_प्रसामान्यम्;
		ret = get_तालकम्_file_मार्गः(&सूचकांक_तालकम्);
		मुक्त(ps_matched);
		गच्छ बहिर्गमनम्;
	}

	/*
	 * As-is commit.
	 *
	 * (1) return the name of the real index file.
	 *
	 * The caller should run hooks on the real index,
	 * and create commit from the_index.
	 * We still need to refresh the index here.
	 */
	यदि (!only && !pathspec.nr) {
		repo_hold_locked_सूचकांक(the_repository, &सूचकांक_तालकम्,
				       तालकम्_DIE_ON_ERROR);
		refresh_cache_or_die(refresh_flags);
		यदि (the_repository->सूचकांक->cache_परिवर्तितम्
		    || !cache_वृक्षः_fully_valid(the_repository->सूचकांक))
			cache_वृक्षः_update(the_repository->सूचकांक, WRITE_वृक्षः_SILENT);
		यदि (write_locked_सूचकांक(the_repository->सूचकांक, &सूचकांक_तालकम्,
				       COMMIT_तालकम् | SKIP_IF_UNCHANGED))
			die(_("unable to write new index file"));
		commit_style = COMMIT_AS_IS;
		ret = repo_get_सूचकांक_file(the_repository);
		गच्छ बहिर्गमनम्;
	}

	/*
	 * A partial commit.
	 *
	 * (0) find the set of affected paths;
	 * (1) get lock on the real index file;
	 * (2) update the_index with the given paths;
	 * (3) write the_index out to the real index (still locked);
	 * (4) get lock on the false index file;
	 * (5) reset the_index from HEAD;
	 * (6) update the_index the same way as (2);
	 * (7) write the_index out to the false index file;
	 * (8) return the name of the false index file (still locked);
	 *
	 * The caller should run hooks on the locked false index, and
	 * create commit from it.  Then
	 * (A) if all goes well, commit the real index;
	 * (B) on failure, rollback the real index;
	 * In either case, rollback the false index.
	 */
	commit_style = COMMIT_PARTIAL;

	यदि (whence != FROM_COMMIT) {
		यदि (whence == FROM_MERGE)
			die(_("cannot do a partial commit during a merge."));
		अन्यथा यदि (is_from_cherry_pick(whence))
			die(_("cannot do a partial commit during a cherry-pick."));
		अन्यथा यदि (is_from_rebase(whence))
			die(_("cannot do a partial commit during a rebase."));
	}

	यदि (list_paths(&partial, !current_शीर्ष ? रिक्त : "HEAD", &pathspec))
		निर्गच्छ(1);

	discard_सूचकांक(the_repository->सूचकांक);
	यदि (repo_read_सूचकांक(the_repository) < 0)
		die(_("cannot read the index"));

	repo_hold_locked_सूचकांक(the_repository, &सूचकांक_तालकम्, तालकम्_DIE_ON_ERROR);
	सङ्कलय_remove_files(&partial);
	refresh_सूचकांक(the_repository->सूचकांक, REFRESH_QUIET, रिक्त, रिक्त, रिक्त);
	cache_वृक्षः_update(the_repository->सूचकांक, WRITE_वृक्षः_SILENT);
	यदि (write_locked_सूचकांक(the_repository->सूचकांक, &सूचकांक_तालकम्, 0))
		die(_("unable to write new index file"));

	मार्गः = repo_git_मार्गः(the_repository, "next-index-%"PRIuMAX,
			     (uintmax_t) getpid());
	hold_तालकम्_file_for_update(&false_तालकम्, मार्गः,
				  तालकम्_DIE_ON_ERROR);

	create_आधारः_सूचकांक(current_शीर्ष);
	सङ्कलय_remove_files(&partial);
	refresh_सूचकांक(the_repository->सूचकांक, REFRESH_QUIET, रिक्त, रिक्त, रिक्त);

	यदि (write_locked_सूचकांक(the_repository->सूचकांक, &false_तालकम्, 0))
		die(_("unable to write temporary index file"));

	discard_सूचकांक(the_repository->सूचकांक);
	ret = get_तालकम्_file_मार्गः(&false_तालकम्);
	read_सूचकांक_from(the_repository->सूचकांक, ret, repo_get_git_dir(the_repository));
बहिर्गमनम्:
	string_list_clear(&partial, 0);
	clear_pathspec(&pathspec);
	मुक्त(मार्गः);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क सततम्_स्थितिः(FILE *fp, ध्रुव वर्ण *सूचकांक_file, ध्रुव वर्ण *उपसर्गः, पूर्णाङ्क nowarn,
		      संरचना wt_स्थितिः *s)
{
	संरचना object_id oid;

	यदि (s->relative_paths)
		s->उपसर्गः = उपसर्गः;

	यदि (amend) {
		s->amend = 1;
		s->reference = "HEAD^1";
	}
	s->verbose = verbose;
	s->सूचकांक_file = सूचकांक_file;
	s->fp = fp;
	s->nowarn = nowarn;
	s->is_initial = repo_get_oid(the_repository, s->reference, &oid) ? 1 : 0;
	यदि (!s->is_initial)
		oidcpy(&s->oid_commit, &oid);
	s->स्थितिः_format = स्थितिः_format;
	s->ignore_submodule_arg = ignore_submodule_arg;

	wt_स्थितिः_collect(s);
	wt_स्थितिः_print(s);
	wt_स्थितिः_collect_free_buffers(s);

	प्रत्यागम s->committable;
}

स्थिर पूर्णाङ्क is_a_merge(ध्रुव संरचना commit *current_शीर्ष)
{
	प्रत्यागम !!(current_शीर्ष->parents && current_शीर्ष->parents->अग्र);
}

स्थिर शून्य assert_विभज_ident(संरचना ident_विभज *id, ध्रुव संरचना strbuf *buf)
{
	यदि (विभज_ident_पङ्क्तिः(id, buf->buf, buf->len) || !id->date_begin)
		BUG("unable to parse our own ident: %s", buf->buf);
}

स्थिर शून्य export_one(ध्रुव वर्ण *var, ध्रुव वर्ण *s, ध्रुव वर्ण *e, पूर्णाङ्क hack)
{
	संरचना strbuf buf = STRBUF_INIT;
	यदि (hack)
		strbuf_addch(&buf, hack);
	strbuf_सङ्कलय(&buf, s, e - s);
	setenv(var, buf.buf, 1);
	strbuf_release(&buf);
}

स्थिर पूर्णाङ्क parse_force_date(ध्रुव वर्ण *अन्तर्, संरचना strbuf *बहिर्गमनम्)
{
	strbuf_addch(बहिर्गमनम्, '@');

	यदि (parse_date(अन्तर्, बहिर्गमनम्) < 0) {
		पूर्णाङ्क errors = 0;
		अचिह्नित दीर्घ t = approxidate_careful(अन्तर्, &errors);
		यदि (errors)
			प्रत्यागम -1;
		strbuf_addf(बहिर्गमनम्, "%lu", t);
	}

	प्रत्यागम 0;
}

स्थिर शून्य समुच्चयः_ident_var(वर्ण **buf, वर्ण *val)
{
	मुक्त(*buf);
	*buf = val;
}

स्थिर शून्य determine_author_info(संरचना strbuf *author_ident)
{
	वर्ण *name, *email, *date;
	संरचना ident_विभज author;

	name = xstrdup_or_null(getenv("GIT_AUTHOR_NAME"));
	email = xstrdup_or_null(getenv("GIT_AUTHOR_EMAIL"));
	date = xstrdup_or_null(getenv("GIT_AUTHOR_DATE"));

	यदि (author_सन्देशः) {
		संरचना ident_विभज ident;
		आकार_t len;
		ध्रुव वर्ण *a;

		a = find_commit_शीर्षकम्(author_सन्देशः_बफर, "author", &len);
		यदि (!a)
			die(_("commit '%s' lacks author header"), author_सन्देशः);
		यदि (विभज_ident_पङ्क्तिः(&ident, a, len) < 0)
			die(_("commit '%s' has malformed author line"), author_सन्देशः);

		समुच्चयः_ident_var(&name, xmemdupz(ident.name_begin, ident.name_अन्तः - ident.name_begin));
		समुच्चयः_ident_var(&email, xmemdupz(ident.mail_begin, ident.mail_अन्तः - ident.mail_begin));

		यदि (ident.date_begin) {
			संरचना strbuf date_buf = STRBUF_INIT;
			strbuf_addch(&date_buf, '@');
			strbuf_सङ्कलय(&date_buf, ident.date_begin, ident.date_अन्तः - ident.date_begin);
			strbuf_addch(&date_buf, ' ');
			strbuf_सङ्कलय(&date_buf, ident.tz_begin, ident.tz_अन्तः - ident.tz_begin);
			समुच्चयः_ident_var(&date, strbuf_detach(&date_buf, रिक्त));
		}
	}

	यदि (force_author) {
		संरचना ident_विभज ident;

		यदि (विभज_ident_पङ्क्तिः(&ident, force_author, वाक्यदैर्घ्यम्(force_author)) < 0)
			die(_("malformed --author parameter"));
		समुच्चयः_ident_var(&name, xmemdupz(ident.name_begin, ident.name_अन्तः - ident.name_begin));
		समुच्चयः_ident_var(&email, xmemdupz(ident.mail_begin, ident.mail_अन्तः - ident.mail_begin));
	}

	यदि (force_date) {
		संरचना strbuf date_buf = STRBUF_INIT;
		यदि (parse_force_date(force_date, &date_buf))
			die(_("invalid date format: %s"), force_date);
		समुच्चयः_ident_var(&date, strbuf_detach(&date_buf, रिक्त));
	}

	strbuf_addstr(author_ident, fmt_ident(name, email, WANT_AUTHOR_IDENT, date,
				IDENT_STRICT));
	assert_विभज_ident(&author, author_ident);
	export_one("GIT_AUTHOR_NAME", author.name_begin, author.name_अन्तः, 0);
	export_one("GIT_AUTHOR_EMAIL", author.mail_begin, author.mail_अन्तः, 0);
	export_one("GIT_AUTHOR_DATE", author.date_begin, author.tz_अन्तः, '@');
	मुक्त(name);
	मुक्त(email);
	मुक्त(date);
}

स्थिर पूर्णाङ्क author_date_is_interesting(शून्य)
{
	प्रत्यागम author_सन्देशः || force_date;
}

#यदि_अपरिभाषितम् WITH_BREAKING_CHANGES
स्थिर शून्य adjust_comment_पङ्क्तिः_char(ध्रुव संरचना strbuf *sb)
{
	वर्ण candidates[] = "#;@!$%^&|:";
	वर्ण *candidate;
	ध्रुव वर्ण *p;
	आकार_t cutoff;

	/* Ignore comment chars in trailing comments (e.g., Conflicts:) */
	cutoff = sb->len - ignored_log_सन्देशः_अष्टकानि(sb->buf, sb->len);

	यदि (!memchr(sb->buf, candidates[0], sb->len)) {
		मुक्त(comment_पङ्क्तिः_str_गन्तव्यम्_free);
		comment_पङ्क्तिः_str = comment_पङ्क्तिः_str_गन्तव्यम्_free =
			xstrfmt("%c", candidates[0]);
		प्रत्यागम;
	}

	p = sb->buf;
	candidate = strchr(candidates, *p);
	यदि (candidate)
		*candidate = ' ';
	प्रति (p = sb->buf; p + 1 < sb->buf + cutoff; p++) {
		यदि ((p[0] == '\n' || p[0] == '\r') && p[1]) {
			candidate = strchr(candidates, p[1]);
			यदि (candidate)
				*candidate = ' ';
		}
	}

	प्रति (p = candidates; *p == ' '; p++)
		;
	यदि (!*p)
		die(_("unable to select a comment character that is not used\n"
		      "in the current commit message"));
	मुक्त(comment_पङ्क्तिः_str_गन्तव्यम्_free);
	comment_पङ्क्तिः_str = comment_पङ्क्तिः_str_गन्तव्यम्_free = xstrfmt("%c", *p);
}
#यदि_अन्तः /* !WITH_BREAKING_CHANGES */

स्थिर शून्य prepare_amend_commit(संरचना commit *commit, संरचना strbuf *sb,
				संरचना pretty_print_context *ctx)
{
	ध्रुव वर्ण *बफर, *subject, *fmt;

	बफर = repo_get_commit_बफर(the_repository, commit, रिक्त);
	find_commit_subject(बफर, &subject);
	/*
	 * If we amend the 'amend!' commit then we don't want to
	 * duplicate the subject line.
	 */
	fmt = starts_with(subject, "amend!") ? "%b" : "%B";
	repo_format_commit_सन्देशः(the_repository, commit, fmt, sb, ctx);
	repo_unuse_commit_बफर(the_repository, commit, बफर);
}

स्थिर शून्य change_दत्त_free(शून्य *util, ध्रुव वर्ण *str अप्रयुक्तम्)
{
	संरचना wt_स्थितिः_change_दत्त *d = util;
	मुक्त(d->rename_source);
	मुक्त(d);
}

स्थिर पूर्णाङ्क prepare_गन्तव्यम्_commit(ध्रुव वर्ण *सूचकांक_file, ध्रुव वर्ण *उपसर्गः,
			     संरचना commit *current_शीर्ष,
			     संरचना wt_स्थितिः *s,
			     संरचना strbuf *author_ident)
{
	संरचना stat statbuf;
	संरचना strbuf committer_ident = STRBUF_INIT;
	पूर्णाङ्क committable;
	संरचना strbuf sb = STRBUF_INIT;
	ध्रुव वर्ण *hook_arg1 = रिक्त;
	ध्रुव वर्ण *hook_arg2 = रिक्त;
	पूर्णाङ्क clean_सन्देशः_contents = (cleanup_बहुलकम् != COMMIT_सन्देशः_CLEANUP_NONE);
	पूर्णाङ्क old_display_comment_उपसर्गः;
	पूर्णाङ्क invoked_hook;

	/* This checks and barfs if author is badly specified */
	determine_author_info(author_ident);

	यदि (!no_verify && सततम्_commit_hook(use_editor, सूचकांक_file, &invoked_hook,
					  "pre-commit", रिक्त))
		प्रत्यागम 0;

	यदि (squash_सन्देशः) {
		/*
		 * Insert the proper subject line before other commit
		 * message options add their content.
		 */
		यदि (use_सन्देशः && !वाक्यं_तुलय(use_सन्देशः, squash_सन्देशः))
			strbuf_addstr(&sb, "squash! ");
		अन्यथा {
			संरचना pretty_print_context ctx = {0};
			संरचना commit *c;
			c = lookup_commit_reference_by_name(squash_सन्देशः);
			यदि (!c)
				die(_("could not lookup commit '%s'"), squash_सन्देशः);
			ctx.output_encoding = get_commit_output_encoding();
			repo_format_commit_सन्देशः(the_repository, c,
						   "squash! %s\n\n", &sb,
						   &ctx);
		}
	}

	यदि (have_option_m && !fixup_सन्देशः) {
		strbuf_addbuf(&sb, &सन्देशः_2);
		hook_arg1 = "message";
	} अन्यथा यदि (logfile && !वाक्यं_तुलय(logfile, "-")) {
		यदि (isatty(0))
			fprintf(stderr, _("(reading log message from standard input)\n"));
		यदि (strbuf_read(&sb, 0, 0) < 0)
			die_errno(_("could not read log from standard input"));
		hook_arg1 = "message";
	} अन्यथा यदि (logfile) {
		यदि (strbuf_read_file(&sb, logfile, 0) < 0)
			die_errno(_("could not read log file '%s'"),
				  logfile);
		hook_arg1 = "message";
	} अन्यथा यदि (use_सन्देशः) {
		ध्रुव वर्ण *बफर;
		बफर = वाक्ये_अन्विष(use_सन्देशः_बफर, "\n\n");
		यदि (बफर)
			strbuf_addstr(&sb, skip_blank_पङ्क्तयः(बफर + 2));
		hook_arg1 = "commit";
		hook_arg2 = use_सन्देशः;
	} अन्यथा यदि (fixup_सन्देशः) {
		संरचना pretty_print_context ctx = {0};
		संरचना commit *commit;
		वर्ण *fmt;
		commit = lookup_commit_reference_by_name(fixup_commit);
		यदि (!commit)
			die(_("could not lookup commit '%s'"), fixup_commit);
		ctx.output_encoding = get_commit_output_encoding();
		fmt = xstrfmt("%s! %%s\n\n", fixup_उपसर्गः);
		repo_format_commit_सन्देशः(the_repository, commit, fmt, &sb,
					   &ctx);
		मुक्त(fmt);
		hook_arg1 = "message";

		/*
		 * Only `-m` commit message option is checked here, as
		 * it supports `--fixup` to append the commit message.
		 *
		 * The other commit message options `-c`/`-C`/`-F` are
		 * incompatible with all the forms of `--fixup` and
		 * have already errored out while parsing the `git commit`
		 * options.
		 */
		यदि (have_option_m && !वाक्यं_तुलय(fixup_उपसर्गः, "fixup"))
			strbuf_addbuf(&sb, &सन्देशः_2);

		यदि (!वाक्यं_तुलय(fixup_उपसर्गः, "amend")) {
			यदि (have_option_m)
				die(_("options '%s' and '%s:%s' cannot be used together"), "-m", "--fixup", fixup_सन्देशः);
			prepare_amend_commit(commit, &sb, &ctx);
		}
	} अन्यथा यदि (!stat(git_मार्गः_merge_सन्देशः(the_repository), &statbuf)) {
		आकार_t merge_सन्देशः_start;

		/*
		 * prepend SQUASH_MSG here if it exists and a
		 * "merge --squash" was originally performed
		 */
		यदि (!stat(git_मार्गः_squash_सन्देशः(the_repository), &statbuf)) {
			यदि (strbuf_read_file(&sb, git_मार्गः_squash_सन्देशः(the_repository), 0) < 0)
				die_errno(_("could not read SQUASH_MSG"));
			hook_arg1 = "squash";
		} अन्यथा
			hook_arg1 = "merge";

		merge_सन्देशः_start = sb.len;
		यदि (strbuf_read_file(&sb, git_मार्गः_merge_सन्देशः(the_repository), 0) < 0)
			die_errno(_("could not read MERGE_MSG"));

		यदि (cleanup_बहुलकम् == COMMIT_सन्देशः_CLEANUP_SCISSORS &&
		    wt_स्थितिः_locate_अन्तः(sb.buf + merge_सन्देशः_start,
					 sb.len - merge_सन्देशः_start) <
				sb.len - merge_सन्देशः_start)
			s->added_cut_पङ्क्तिः = 1;
	} अन्यथा यदि (!stat(git_मार्गः_squash_सन्देशः(the_repository), &statbuf)) {
		यदि (strbuf_read_file(&sb, git_मार्गः_squash_सन्देशः(the_repository), 0) < 0)
			die_errno(_("could not read SQUASH_MSG"));
		hook_arg1 = "squash";
	} अन्यथा यदि (template_file) {
		यदि (strbuf_read_file(&sb, template_file, 0) < 0)
			die_errno(_("could not read '%s'"), template_file);
		hook_arg1 = "template";
		clean_सन्देशः_contents = 0;
	}

	/*
	 * The remaining cases don't modify the template message, but
	 * just set the argument(s) to the prepare-commit-msg hook.
	 */
	अन्यथा यदि (whence == FROM_MERGE)
		hook_arg1 = "merge";
	अन्यथा यदि (is_from_cherry_pick(whence) || whence == FROM_REBASE_PICK) {
		hook_arg1 = "commit";
		hook_arg2 = "CHERRY_PICK_HEAD";
	}

	यदि (squash_सन्देशः) {
		/*
		 * If squash_commit was used for the commit subject,
		 * then we're possibly hijacking other commit log options.
		 * Reset the hook args to tell the real story.
		 */
		hook_arg1 = "message";
		hook_arg2 = "";
	}

	s->fp = fopen_for_writing(git_मार्गः_commit_editmsg());
	यदि (!s->fp)
		die_errno(_("could not open '%s'"), git_मार्गः_commit_editmsg());

	/* Ignore status.displayCommentPrefix: we do need comments in COMMIT_EDITMSG. */
	old_display_comment_उपसर्गः = s->display_comment_उपसर्गः;
	s->display_comment_उपसर्गः = 1;

	/*
	 * Most hints are counter-productive when the commit has
	 * already started.
	 */
	s->hints = 0;

	यदि (clean_सन्देशः_contents)
		strbuf_stripspace(&sb, रिक्त);

	यदि (signoff)
		append_signoff(&sb, ignored_log_सन्देशः_अष्टकानि(sb.buf, sb.len), 0);

	यदि (fwrite(sb.buf, 1, sb.len, s->fp) < sb.len)
		die_errno(_("could not write commit template"));

#यदि_अपरिभाषितम् WITH_BREAKING_CHANGES
	यदि (auto_comment_पङ्क्तिः_char)
		adjust_comment_पङ्क्तिः_char(&sb);
#यदि_अन्तः /* !WITH_BREAKING_CHANGES */
	strbuf_release(&sb);

	/* This checks if committer ident is explicitly given */
	strbuf_addstr(&committer_ident, git_committer_info(IDENT_STRICT));
	यदि (use_editor && include_स्थितिः) {
		पूर्णाङ्क ident_shown = 0;
		गणना git_colorbool saved_color_setting;
		संरचना ident_विभज ci, ai;
		ध्रुव वर्ण *hint_cleanup_all = allow_रिक्तानि_सन्देशः ?
			_("Please enter the commit message for your changes."
			  " Lines starting\nwith '%s' will be ignored.\n") :
			_("Please enter the commit message for your changes."
			  " Lines starting\nwith '%s' will be ignored, and an empty"
			  " message aborts the commit.\n");
		ध्रुव वर्ण *hint_cleanup_space = allow_रिक्तानि_सन्देशः ?
			_("Please enter the commit message for your changes."
			  " Lines starting\n"
			  "with '%s' will be kept; you may remove them"
			  " yourself if you want to.\n") :
			_("Please enter the commit message for your changes."
			  " Lines starting\n"
			  "with '%s' will be kept; you may remove them"
			  " yourself if you want to.\n"
			  "An empty message aborts the commit.\n");
		यदि (whence != FROM_COMMIT) {
			यदि (cleanup_बहुलकम् == COMMIT_सन्देशः_CLEANUP_SCISSORS)
				wt_स्थितिः_सङ्कलय_cut_पङ्क्तिः(s);
			स्थितिः_printf_ln(
				s, GIT_COLOR_प्रसामान्यम्,
				whence == FROM_MERGE ?
					      _("\n"
					  "It looks like you may be committing a merge.\n"
					  "If this is not correct, please run\n"
					  "	git update-ref -d MERGE_HEAD\n"
					  "and try again.\n") :
					      _("\n"
					  "It looks like you may be committing a cherry-pick.\n"
					  "If this is not correct, please run\n"
					  "	git update-ref -d CHERRY_PICK_HEAD\n"
					  "and try again.\n"));
		}

		fprintf(s->fp, "\n");
		यदि (cleanup_बहुलकम् == COMMIT_सन्देशः_CLEANUP_ALL)
			स्थितिः_printf(s, GIT_COLOR_प्रसामान्यम्, hint_cleanup_all, comment_पङ्क्तिः_str);
		अन्यथा यदि (cleanup_बहुलकम् == COMMIT_सन्देशः_CLEANUP_SCISSORS) {
			यदि (whence == FROM_COMMIT)
				wt_स्थितिः_सङ्कलय_cut_पङ्क्तिः(s);
		} अन्यथा /* COMMIT_MSG_CLEANUP_SPACE, that is. */
			स्थितिः_printf(s, GIT_COLOR_प्रसामान्यम्, hint_cleanup_space, comment_पङ्क्तिः_str);

		/*
		 * These should never fail because they come from our own
		 * fmt_ident. They may fail the sane_ident test, but we know
		 * that the name and mail pointers will at least be valid,
		 * which is enough for our tests and printing here.
		 */
		assert_विभज_ident(&ai, author_ident);
		assert_विभज_ident(&ci, &committer_ident);

		यदि (ident_cmp(&ai, &ci))
			स्थितिः_printf_ln(s, GIT_COLOR_प्रसामान्यम्,
				_("%s"
				"Author:    %.*s <%.*s>"),
				ident_shown++ ? "" : "\n",
				(पूर्णाङ्क)(ai.name_अन्तः - ai.name_begin), ai.name_begin,
				(पूर्णाङ्क)(ai.mail_अन्तः - ai.mail_begin), ai.mail_begin);

		यदि (author_date_is_interesting())
			स्थितिः_printf_ln(s, GIT_COLOR_प्रसामान्यम्,
				_("%s"
				"Date:      %s"),
				ident_shown++ ? "" : "\n",
				show_ident_date(&ai, DATE_बहुलकम्(प्रसामान्यम्)));

		यदि (!committer_ident_sufficiently_given())
			स्थितिः_printf_ln(s, GIT_COLOR_प्रसामान्यम्,
				_("%s"
				"Committer: %.*s <%.*s>"),
				ident_shown++ ? "" : "\n",
				(पूर्णाङ्क)(ci.name_अन्तः - ci.name_begin), ci.name_begin,
				(पूर्णाङ्क)(ci.mail_अन्तः - ci.mail_begin), ci.mail_begin);

		स्थितिः_printf_ln(s, GIT_COLOR_प्रसामान्यम्, "%s", ""); /* Add new line for clarity */

		saved_color_setting = s->use_color;
		s->use_color = GIT_COLOR_NEVER;
		committable = सततम्_स्थितिः(s->fp, सूचकांक_file, उपसर्गः, 1, s);
		s->use_color = saved_color_setting;
		string_list_clear_func(&s->change, change_दत्त_free);
	} अन्यथा {
		संरचना object_id oid;
		ध्रुव वर्ण *parent = "HEAD";

		यदि (!the_repository->सूचकांक->initialized && repo_read_सूचकांक(the_repository) < 0)
			die(_("Cannot read index"));

		यदि (amend)
			parent = "HEAD^1";

		यदि (repo_get_oid(the_repository, parent, &oid)) {
			पूर्णाङ्क i, ita_nr = 0;

			/* TODO: audit for interaction with sparse-index. */
			ensure_पूर्णम्_सूचकांक(the_repository->सूचकांक);
			प्रति (i = 0; i < the_repository->सूचकांक->cache_nr; i++)
				यदि (ce_intent_गन्तव्यम्_सङ्कलय(the_repository->सूचकांक->cache[i]))
					ita_nr++;
			committable = the_repository->सूचकांक->cache_nr > ita_nr;
		} अन्यथा {
			/*
			 * Unless the user did explicitly request a submodule
			 * ignore mode by passing a command line option we do
			 * not ignore any changed submodule SHA-1s when
			 * comparing index and parent, no matter what is
			 * configured. Otherwise we won't commit any
			 * submodules which were manually staged, which would
			 * be really confusing.
			 */
			संरचना diff_flags flags = DIFF_FLAGS_INIT;
			flags.override_submodule_config = 1;
			यदि (ignore_submodule_arg &&
			    !वाक्यं_तुलय(ignore_submodule_arg, "all"))
				flags.ignore_submodules = 1;
			committable = सूचकांक_differs_from(the_repository,
							 parent, &flags, 1);
		}
	}
	strbuf_release(&committer_ident);

	fclose(s->fp);

	यदि (trailer_तर्काः.nr) {
		यदि (amend_file_with_trailers(git_मार्गः_commit_editmsg(), &trailer_तर्काः))
			die(_("unable to pass trailers to --trailers"));
		strvec_clear(&trailer_तर्काः);
	}

	/*
	 * Reject an attempt to record a non-merge empty commit without
	 * explicit --allow-empty. In the cherry-pick case, it may be
	 * empty due to conflict resolution, which the user should okay.
	 */
	यदि (!committable && whence != FROM_MERGE && !allow_रिक्तानि &&
	    !(amend && is_a_merge(current_शीर्ष))) {
		s->hints = advice_enabled(ADVICE_स्थितिः_HINTS);
		s->display_comment_उपसर्गः = old_display_comment_उपसर्गः;
		सततम्_स्थितिः(stdout, सूचकांक_file, उपसर्गः, 0, s);
		यदि (amend)
			fputs(_(रिक्तानि_amend_advice), stderr);
		अन्यथा यदि (is_from_cherry_pick(whence) ||
			 whence == FROM_REBASE_PICK) {
			fputs(_(रिक्तानि_cherry_pick_advice), stderr);
			यदि (whence == FROM_CHERRY_PICK_SINGLE)
				fputs(_(रिक्तानि_cherry_pick_advice_single), stderr);
			अन्यथा यदि (whence == FROM_CHERRY_PICK_MULTI)
				fputs(_(रिक्तानि_cherry_pick_advice_multi), stderr);
			अन्यथा
				fputs(_(रिक्तानि_rebase_pick_advice), stderr);
		}
		प्रत्यागम 0;
	}

	यदि (!no_verify && invoked_hook) {
		/*
		 * Re-read the index as the pre-commit-commit hook was invoked
		 * and could have updated it. We must do this before we invoke
		 * the editor and after we invoke run_status above.
		 */
		discard_सूचकांक(the_repository->सूचकांक);
	}
	read_सूचकांक_from(the_repository->सूचकांक, सूचकांक_file, repo_get_git_dir(the_repository));

	यदि (cache_वृक्षः_update(the_repository->सूचकांक, 0)) {
		error(_("Error building trees"));
		प्रत्यागम 0;
	}

	यदि (सततम्_commit_hook(use_editor, सूचकांक_file, रिक्त, "prepare-commit-msg",
			    git_मार्गः_commit_editmsg(), hook_arg1, hook_arg2, रिक्त))
		प्रत्यागम 0;

	यदि (use_editor) {
		संरचना strvec env = STRVEC_INIT;

		strvec_pushf(&env, "GIT_INDEX_FILE=%s", सूचकांक_file);
		यदि (launch_editor(git_मार्गः_commit_editmsg(), रिक्त, env.v)) {
			fprintf(stderr,
			_("Please supply the message using either -m or -F option.\n"));
			निर्गच्छ(1);
		}
		strvec_clear(&env);
	}

	यदि (!no_verify &&
	    सततम्_commit_hook(use_editor, सूचकांक_file, रिक्त, "commit-msg",
			    git_मार्गः_commit_editmsg(), रिक्त)) {
		प्रत्यागम 0;
	}

	प्रत्यागम 1;
}

स्थिर ध्रुव वर्ण *find_author_by_nickname(ध्रुव वर्ण *name)
{
	संरचना rev_info revs;
	संरचना commit *commit;
	संरचना strbuf buf = STRBUF_INIT;
	ध्रुव वर्ण *av[20];
	पूर्णाङ्क ac = 0;

	repo_init_revisions(the_repository, &revs, रिक्त);
	strbuf_addf(&buf, "--author=%s", name);
	av[++ac] = "--all";
	av[++ac] = "-i";
	av[++ac] = buf.buf;
	av[++ac] = रिक्त;
	सज्जता_revisions(ac, av, &revs, रिक्त);
	revs.mailmap = xmalloc(परिमाण(संरचना string_list));
	string_list_init_nodup(revs.mailmap);
	read_mailmap(the_repository, revs.mailmap);

	यदि (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	commit = get_revision(&revs);
	यदि (commit) {
		संरचना pretty_print_context ctx = {0};
		ctx.date_बहुलकम्.type = DATE_प्रसामान्यम्;
		strbuf_release(&buf);
		repo_format_commit_सन्देशः(the_repository, commit,
					   "%aN <%aE>", &buf, &ctx);
		release_revisions(&revs);
		प्रत्यागम strbuf_detach(&buf, रिक्त);
	}
	die(_("--author '%s' is not 'Name <email>' and matches no existing author"), name);
}

स्थिर शून्य handle_ignored_arg(संरचना wt_स्थितिः *s)
{
	यदि (!ignored_arg)
		; /* default already initialized */
	अन्यथा यदि (!वाक्यं_तुलय(ignored_arg, "traditional"))
		s->show_ignored_बहुलकम् = SHOW_TRADITIONAL_IGNORED;
	अन्यथा यदि (!वाक्यं_तुलय(ignored_arg, "no"))
		s->show_ignored_बहुलकम् = SHOW_NO_IGNORED;
	अन्यथा यदि (!वाक्यं_तुलय(ignored_arg, "matching"))
		s->show_ignored_बहुलकम् = SHOW_MATCHING_IGNORED;
	अन्यथा
		die(_("Invalid ignored mode '%s'"), ignored_arg);
}

स्थिर गणना untracked_स्थितिः_type parse_untracked_setting_name(ध्रुव वर्ण *u)
{
	/*
	 * Please update $__git_untracked_file_modes in
	 * git-completion.bash when you add new options
	 */
	विकल्प (git_parse_maybe_bool(u)) {
	पक्ष 0:
		u = "no";
		भङ्ग;
	पक्ष 1:
		u = "normal";
		भङ्ग;
	स्वभाव:
		भङ्ग;
	}

	यदि (!वाक्यं_तुलय(u, "no"))
		प्रत्यागम SHOW_NO_UNTRACKED_FILES;
	अन्यथा यदि (!वाक्यं_तुलय(u, "normal"))
		प्रत्यागम SHOW_प्रसामान्यम्_UNTRACKED_FILES;
	अन्यथा यदि (!वाक्यं_तुलय(u, "all"))
		प्रत्यागम SHOW_ALL_UNTRACKED_FILES;
	अन्यथा
		प्रत्यागम SHOW_UNTRACKED_FILES_ERROR;
}

स्थिर शून्य handle_untracked_files_arg(संरचना wt_स्थितिः *s)
{
	गणना untracked_स्थितिः_type u;

	यदि (!untracked_files_arg)
		प्रत्यागम; /* default already initialized */

	u = parse_untracked_setting_name(untracked_files_arg);
	यदि (u == SHOW_UNTRACKED_FILES_ERROR)
		die(_("Invalid untracked files mode '%s'"),
		    untracked_files_arg);
	s->show_untracked_files = u;
}

स्थिर ध्रुव वर्ण *read_commit_सन्देशः_2(ध्रुव वर्ण *name)
{
	ध्रुव वर्ण *बहिर्गमनम्_enc;
	संरचना commit *commit;

	commit = lookup_commit_reference_by_name(name);
	यदि (!commit)
		die(_("could not lookup commit '%s'"), name);
	बहिर्गमनम्_enc = get_commit_output_encoding();
	प्रत्यागम repo_logmsg_reencode(the_repository, commit, रिक्त, बहिर्गमनम्_enc);
}

/*
 * Enumerate what needs to be propagated when --porcelain
 * is not in effect here.
 */
स्थिर संरचना स्थितिः_deferred_config {
	गणना wt_स्थितिः_format स्थितिः_format;
	पूर्णाङ्क show_branch;
	गणना ahead_behind_flags ahead_behind;
} स्थितिः_deferred_config = {
	स्थितिः_FORMAT_UNSPECIFIED,
	-1, /* unspecified */
	AHEAD_BEHIND_UNSPECIFIED,
};

स्थिर शून्य finalize_deferred_config(संरचना wt_स्थितिः *s)
{
	पूर्णाङ्क use_deferred_config = (स्थितिः_format != स्थितिः_FORMAT_PORCELAIN &&
				   स्थितिः_format != स्थितिः_FORMAT_PORCELAIN_V2 &&
				   !s->null_termination);

	यदि (s->null_termination) {
		यदि (स्थितिः_format == स्थितिः_FORMAT_NONE ||
		    स्थितिः_format == स्थितिः_FORMAT_UNSPECIFIED)
			स्थितिः_format = स्थितिः_FORMAT_PORCELAIN;
		अन्यथा यदि (स्थितिः_format == स्थितिः_FORMAT_LONG)
			die(_("options '%s' and '%s' cannot be used together"), "--long", "-z");
	}

	यदि (use_deferred_config && स्थितिः_format == स्थितिः_FORMAT_UNSPECIFIED)
		स्थितिः_format = स्थितिः_deferred_config.स्थितिः_format;
	यदि (स्थितिः_format == स्थितिः_FORMAT_UNSPECIFIED)
		स्थितिः_format = स्थितिः_FORMAT_NONE;

	यदि (use_deferred_config && s->show_branch < 0)
		s->show_branch = स्थितिः_deferred_config.show_branch;
	यदि (s->show_branch < 0)
		s->show_branch = 0;

	/*
	 * If the user did not give a "--[no]-ahead-behind" command
	 * line argument *AND* we will print in a human-readable format
	 * (short, long etc.) then we inherit from the status.aheadbehind
	 * config setting.  In all other cases (and porcelain V[12] formats
	 * in particular), we inherit _FULL for backwards compatibility.
	 */
	यदि (use_deferred_config &&
	    s->ahead_behind_flags == AHEAD_BEHIND_UNSPECIFIED)
		s->ahead_behind_flags = स्थितिः_deferred_config.ahead_behind;

	यदि (s->ahead_behind_flags == AHEAD_BEHIND_UNSPECIFIED)
		s->ahead_behind_flags = AHEAD_BEHIND_पूर्णम्;
}

स्थिर शून्य check_fixup_reword_options(पूर्णाङ्क argc, ध्रुव वर्ण *argv[]) {
	यदि (whence != FROM_COMMIT) {
		यदि (whence == FROM_MERGE)
			die(_("You are in the middle of a merge -- cannot reword."));
		अन्यथा यदि (is_from_cherry_pick(whence))
			die(_("You are in the middle of a cherry-pick -- cannot reword."));
	}
	यदि (argc)
		die(_("reword option of '%s' and path '%s' cannot be used together"), "--fixup", *argv);
	यदि (patch_interactive || interactive || all || also || only)
		die(_("reword option of '%s' and '%s' cannot be used together"),
			"--fixup", "--patch/--interactive/--all/--include/--only");
}

स्थिर पूर्णाङ्क parse_and_validate_options(पूर्णाङ्क argc, ध्रुव वर्ण *argv[],
				      ध्रुव संरचना option *options,
				      ध्रुव वर्ण * ध्रुव usage[],
				      ध्रुव वर्ण *उपसर्गः,
				      संरचना commit *current_शीर्ष,
				      संरचना wt_स्थितिः *s)
{
	argc = parse_options(argc, argv, उपसर्गः, options, usage, 0);
	finalize_deferred_config(s);

	यदि (force_author && !strchr(force_author, '>'))
		force_author = find_author_by_nickname(force_author);

	यदि (force_author && renew_authorship)
		die(_("options '%s' and '%s' cannot be used together"), "--reset-author", "--author");

	यदि (logfile || have_option_m || use_सन्देशः)
		use_editor = 0;

	/* Sanity check options */
	यदि (amend && !current_शीर्ष)
		die(_("You have nothing to amend."));
	यदि (amend && whence != FROM_COMMIT) {
		यदि (whence == FROM_MERGE)
			die(_("You are in the middle of a merge -- cannot amend."));
		अन्यथा यदि (is_from_cherry_pick(whence))
			die(_("You are in the middle of a cherry-pick -- cannot amend."));
		अन्यथा यदि (whence == FROM_REBASE_PICK)
			die(_("You are in the middle of a rebase -- cannot amend."));
	}
	यदि (fixup_सन्देशः && squash_सन्देशः)
		die(_("options '%s' and '%s' cannot be used together"), "--squash", "--fixup");
	die_for_incompatible_opt4(!!use_सन्देशः, "-C",
				  !!edit_सन्देशः, "-c",
				  !!logfile, "-F",
				  !!fixup_सन्देशः, "--fixup");
	die_for_incompatible_opt4(have_option_m, "-m",
				  !!edit_सन्देशः, "-c",
				  !!use_सन्देशः, "-C",
				  !!logfile, "-F");
	यदि (use_सन्देशः || edit_सन्देशः || logfile ||fixup_सन्देशः || have_option_m)
		FREE_AND_NULL(template_file);
	यदि (edit_सन्देशः)
		use_सन्देशः = edit_सन्देशः;
	यदि (amend && !use_सन्देशः && !fixup_सन्देशः)
		use_सन्देशः = "HEAD";
	यदि (!use_सन्देशः && !is_from_cherry_pick(whence) &&
	    !is_from_rebase(whence) && renew_authorship)
		die(_("--reset-author can be used only with -C, -c or --amend."));
	यदि (use_सन्देशः) {
		use_सन्देशः_बफर = read_commit_सन्देशः_2(use_सन्देशः);
		यदि (!renew_authorship) {
			author_सन्देशः = use_सन्देशः;
			author_सन्देशः_बफर = use_सन्देशः_बफर;
		}
	}
	यदि ((is_from_cherry_pick(whence) || whence == FROM_REBASE_PICK) &&
	    !renew_authorship) {
		author_सन्देशः = "CHERRY_PICK_HEAD";
		author_सन्देशः_बफर = read_commit_सन्देशः_2(author_सन्देशः);
	}

	यदि (patch_interactive)
		interactive = 1;

	die_for_incompatible_opt4(also, "-i/--include",
				  only, "-o/--only",
				  all, "-a/--all",
				  interactive, "--interactive/-p/--patch");
	यदि (fixup_सन्देशः) {
		/*
		 * We limit --fixup's suboptions to only alpha characters.
		 * If the first character after a run of alpha is colon,
		 * then the part before the colon may be a known suboption
		 * name like `amend` or `reword`, or a misspelt suboption
		 * name. In either case, we treat it as
		 * --fixup=<suboption>:<arg>.
		 *
		 * Otherwise, we are dealing with --fixup=<commit>.
		 */
		वर्ण *p = fixup_सन्देशः;
		यावत् (अक्षरम्_वा(*p))
			p++;
		यदि (p > fixup_सन्देशः && *p == ':') {
			*p = '\0';
			fixup_commit = p + 1;
			यदि (!वाक्यं_तुलय("amend", fixup_सन्देशः) ||
			    !वाक्यं_तुलय("reword", fixup_सन्देशः)) {
				fixup_उपसर्गः = "amend";
				allow_रिक्तानि = 1;
				यदि (*fixup_सन्देशः == 'r') {
					check_fixup_reword_options(argc, argv);
					only = 1;
				}
			} अन्यथा {
				die(_("unknown option: --fixup=%s:%s"), fixup_सन्देशः, fixup_commit);
			}
		} अन्यथा {
			fixup_commit = fixup_सन्देशः;
			fixup_उपसर्गः = "fixup";
			use_editor = 0;
		}
	}

	यदि (0 <= edit_flag)
		use_editor = edit_flag;

	handle_untracked_files_arg(s);

	यदि (all && argc > 0)
		die(_("paths '%s ...' with -a does not make sense"),
		    argv[0]);

	यदि (स्थितिः_format != स्थितिः_FORMAT_NONE)
		dry_सततम् = 1;

	प्रत्यागम argc;
}

स्थिर पूर्णाङ्क dry_सततम्_commit(ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
			  ध्रुव संरचना commit *current_शीर्ष, संरचना wt_स्थितिः *s)
{
	पूर्णाङ्क committable;
	ध्रुव वर्ण *सूचकांक_file;

	सूचकांक_file = prepare_सूचकांक(argv, उपसर्गः, current_शीर्ष, 1);
	committable = सततम्_स्थितिः(stdout, सूचकांक_file, उपसर्गः, 0, s);
	rollback_सूचकांक_files();

	प्रत्यागम committable ? 0 : 1;
}

define_list_config_array_extra(color_स्थितिः_slots, {"added"});

स्थिर पूर्णाङ्क parse_स्थितिः_slot(ध्रुव वर्ण *slot)
{
	यदि (!strcasecmp(slot, "added"))
		प्रत्यागम WT_स्थितिः_UPDATED;

	प्रत्यागम LOOKUP_CONFIG(color_स्थितिः_slots, slot);
}

स्थिर पूर्णाङ्क git_स्थितिः_config(ध्रुव वर्ण *k, ध्रुव वर्ण *v,
			     ध्रुव संरचना config_context *ctx, शून्य *cb)
{
	संरचना wt_स्थितिः *s = cb;
	ध्रुव वर्ण *slot_name;

	यदि (starts_with(k, "column."))
		प्रत्यागम git_column_config(k, v, "status", &s->colopts);
	यदि (!वाक्यं_तुलय(k, "status.submodulesummary")) {
		पूर्णाङ्क is_bool;
		s->submodule_summary = git_config_bool_or_int(k, v, ctx->kvi,
							      &is_bool);
		यदि (is_bool && s->submodule_summary)
			s->submodule_summary = -1;
		प्रत्यागम 0;
	}
	यदि (!वाक्यं_तुलय(k, "status.short")) {
		यदि (git_config_bool(k, v))
			स्थितिः_deferred_config.स्थितिः_format = स्थितिः_FORMAT_SHORT;
		अन्यथा
			स्थितिः_deferred_config.स्थितिः_format = स्थितिः_FORMAT_NONE;
		प्रत्यागम 0;
	}
	यदि (!वाक्यं_तुलय(k, "status.branch")) {
		स्थितिः_deferred_config.show_branch = git_config_bool(k, v);
		प्रत्यागम 0;
	}
	यदि (!वाक्यं_तुलय(k, "status.aheadbehind")) {
		स्थितिः_deferred_config.ahead_behind = git_config_bool(k, v);
		प्रत्यागम 0;
	}
	यदि (!वाक्यं_तुलय(k, "status.showstash")) {
		s->show_stash = git_config_bool(k, v);
		प्रत्यागम 0;
	}
	यदि (!वाक्यं_तुलय(k, "status.color") || !वाक्यं_तुलय(k, "color.status")) {
		s->use_color = git_config_colorbool(k, v);
		प्रत्यागम 0;
	}
	यदि (!वाक्यं_तुलय(k, "status.displaycommentprefix")) {
		s->display_comment_उपसर्गः = git_config_bool(k, v);
		प्रत्यागम 0;
	}
	यदि (skip_उपसर्गः(k, "status.color.", &slot_name) ||
	    skip_उपसर्गः(k, "color.status.", &slot_name)) {
		पूर्णाङ्क slot = parse_स्थितिः_slot(slot_name);
		यदि (slot < 0)
			प्रत्यागम 0;
		यदि (!v)
			प्रत्यागम config_error_nonbool(k);
		प्रत्यागम color_parse(v, s->color_palette[slot]);
	}
	यदि (!वाक्यं_तुलय(k, "status.relativepaths")) {
		s->relative_paths = git_config_bool(k, v);
		प्रत्यागम 0;
	}
	यदि (!वाक्यं_तुलय(k, "status.showuntrackedfiles")) {
		गणना untracked_स्थितिः_type u;

		u = parse_untracked_setting_name(v);
		यदि (u == SHOW_UNTRACKED_FILES_ERROR)
			प्रत्यागम error(_("Invalid untracked files mode '%s'"), v);
		s->show_untracked_files = u;
		प्रत्यागम 0;
	}
	यदि (!वाक्यं_तुलय(k, "diff.renamelimit")) {
		यदि (s->rename_सीमा == -1)
			s->rename_सीमा = git_config_int(k, v, ctx->kvi);
		प्रत्यागम 0;
	}
	यदि (!वाक्यं_तुलय(k, "status.renamelimit")) {
		s->rename_सीमा = git_config_int(k, v, ctx->kvi);
		प्रत्यागम 0;
	}
	यदि (!वाक्यं_तुलय(k, "diff.renames")) {
		यदि (s->detect_rename == -1)
			s->detect_rename = git_config_rename(k, v);
		प्रत्यागम 0;
	}
	यदि (!वाक्यं_तुलय(k, "status.renames")) {
		s->detect_rename = git_config_rename(k, v);
		प्रत्यागम 0;
	}
	प्रत्यागम git_diff_ui_config(k, v, ctx, रिक्त);
}

पूर्णाङ्क cmd_स्थितिः(पूर्णाङ्क argc,
ध्रुव वर्ण **argv,
ध्रुव वर्ण *उपसर्गः,
संरचना repository *repo अप्रयुक्तम्)
{
	स्थिर पूर्णाङ्क no_renames = -1;
	स्थिर ध्रुव वर्ण *rename_score_arg = (ध्रुव वर्ण *)-1;
	स्थिर संरचना wt_स्थितिः s;
	अचिह्नित पूर्णाङ्क प्रगतिः_flag = 0;
	पूर्णाङ्क fd;
	संरचना object_id oid;
	स्थिर संरचना option builtin_स्थितिः_options[] = {
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT_समुच्चयः_INT('s', "short", &स्थितिः_format,
			    N_("show status concisely"), स्थितिः_FORMAT_SHORT),
		OPT_BOOL('b', "branch", &s.show_branch,
			 N_("show branch information")),
		OPT_BOOL(0, "show-stash", &s.show_stash,
			 N_("show stash information")),
		OPT_BOOL(0, "ahead-behind", &s.ahead_behind_flags,
			 N_("compute full ahead/behind values")),
		OPT_CALLBACK_F(0, "porcelain", &स्थितिः_format,
		  N_("version"), N_("machine-readable output"),
		  PARSE_OPT_OPTARG, opt_parse_porcelain),
		OPT_समुच्चयः_INT(0, "long", &स्थितिः_format,
			    N_("show status in long format (default)"),
			    स्थितिः_FORMAT_LONG),
		OPT_BOOL('z', "null", &s.null_termination,
			 N_("terminate entries with NUL")),
		{
			.type = OPTION_STRING,
			.short_name = 'u',
			.long_name = "untracked-files",
			.मानम् = &untracked_files_arg,
			.argh = N_("mode"),
			.help = N_("show untracked files, optional modes: all, normal, no. (Default: all)"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t)"all",
		},
		{
			.type = OPTION_STRING,
			.long_name = "ignored",
			.मानम् = &ignored_arg,
			.argh = N_("mode"),
			.help = N_("show ignored files, optional modes: traditional, matching, no. (Default: traditional)"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t)"traditional",
		},
		{
			.type = OPTION_STRING,
			.long_name = "ignore-submodules",
			.मानम् = &ignore_submodule_arg,
			.argh = N_("when"),
			.help = N_("ignore changes to submodules, optional when: all, dirty, untracked. (Default: all)"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t)"all",
		},
		OPT_COLUMN(0, "column", &s.colopts, N_("list untracked files in columns")),
		OPT_BOOL(0, "no-renames", &no_renames, N_("do not detect renames")),
		OPT_CALLBACK_F('M', "find-renames", &rename_score_arg,
		  N_("n"), N_("detect renames, optionally set similarity index"),
		  PARSE_OPT_OPTARG | PARSE_OPT_NONEG, opt_parse_rename_score),
		OPT_अन्तः(),
	};

	show_usage_with_options_if_asked(argc, argv,
					 builtin_स्थितिः_usage, builtin_स्थितिः_options);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_पूर्णम्_सूचकांक = 0;

	स्थितिः_init_config(&s, git_स्थितिः_config);
	argc = parse_options(argc, argv, उपसर्गः,
			     builtin_स्थितिः_options,
			     builtin_स्थितिः_usage, 0);
	finalize_colopts(&s.colopts, -1);
	finalize_deferred_config(&s);

	handle_untracked_files_arg(&s);
	handle_ignored_arg(&s);

	यदि (s.show_ignored_बहुलकम् == SHOW_MATCHING_IGNORED &&
	    s.show_untracked_files == SHOW_NO_UNTRACKED_FILES)
		die(_("Unsupported combination of ignored and untracked-files arguments"));

	parse_pathspec(&s.pathspec, 0,
		       PATHSPEC_PREFER_पूर्णम्,
		       उपसर्गः, argv);

	यदि (स्थितिः_format != स्थितिः_FORMAT_PORCELAIN &&
	    स्थितिः_format != स्थितिः_FORMAT_PORCELAIN_V2)
		प्रगतिः_flag = REFRESH_प्रगतिः;
	repo_read_सूचकांक(the_repository);
	refresh_सूचकांक(the_repository->सूचकांक,
		      REFRESH_QUIET|REFRESH_UNMERGED|प्रगतिः_flag,
		      &s.pathspec, रिक्त, रिक्त);

	यदि (use_optional_locks())
		fd = repo_hold_locked_सूचकांक(the_repository, &सूचकांक_तालकम्, 0);
	अन्यथा
		fd = -1;

	s.is_initial = repo_get_oid(the_repository, s.reference, &oid) ? 1 : 0;
	यदि (!s.is_initial)
		oidcpy(&s.oid_commit, &oid);

	s.ignore_submodule_arg = ignore_submodule_arg;
	s.स्थितिः_format = स्थितिः_format;
	s.verbose = verbose;
	यदि (no_renames != -1)
		s.detect_rename = !no_renames;
	यदि ((intptr_t)rename_score_arg != -1) {
		यदि (s.detect_rename < DIFF_DETECT_RENAME)
			s.detect_rename = DIFF_DETECT_RENAME;
		यदि (rename_score_arg)
			s.rename_score = parse_rename_score(&rename_score_arg);
	}

	wt_स्थितिः_collect(&s);

	यदि (0 <= fd)
		repo_update_सूचकांक_if_able(the_repository, &सूचकांक_तालकम्);

	यदि (s.relative_paths)
		s.उपसर्गः = उपसर्गः;

	wt_स्थितिः_print(&s);
	wt_स्थितिः_collect_free_buffers(&s);

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क git_commit_config(ध्रुव वर्ण *k, ध्रुव वर्ण *v,
			     ध्रुव संरचना config_context *ctx, शून्य *cb)
{
	संरचना wt_स्थितिः *s = cb;

	यदि (!वाक्यं_तुलय(k, "commit.template"))
		प्रत्यागम git_config_pathname(&template_file, k, v);
	यदि (!वाक्यं_तुलय(k, "commit.status")) {
		include_स्थितिः = git_config_bool(k, v);
		प्रत्यागम 0;
	}
	यदि (!वाक्यं_तुलय(k, "commit.cleanup")) {
		FREE_AND_NULL(cleanup_config);
		प्रत्यागम git_config_string(&cleanup_config, k, v);
	}
	यदि (!वाक्यं_तुलय(k, "commit.gpgsign")) {
		sign_commit = git_config_bool(k, v) ? "" : रिक्त;
		प्रत्यागम 0;
	}
	यदि (!वाक्यं_तुलय(k, "commit.verbose")) {
		पूर्णाङ्क is_bool;
		config_commit_verbose = git_config_bool_or_int(k, v, ctx->kvi,
							       &is_bool);
		प्रत्यागम 0;
	}

	प्रत्यागम git_स्थितिः_config(k, v, ctx, s);
}

पूर्णाङ्क cmd_commit(पूर्णाङ्क argc,
	       ध्रुव वर्ण **argv,
	       ध्रुव वर्ण *उपसर्गः,
	       संरचना repository *repo अप्रयुक्तम्)
{
	स्थिर संरचना wt_स्थितिः s;
	स्थिर ध्रुव वर्ण *cleanup_arg = रिक्त;
	स्थिर संरचना option builtin_commit_options[] = {
		OPT__QUIET(&quiet, N_("suppress summary after successful commit")),
		OPT__VERBOSE(&verbose, N_("show diff in commit message template")),

		OPT_गणः(N_("Commit message options")),
		OPT_FILENAME('F', "file", &logfile, N_("read message from file")),
		OPT_STRING(0, "author", &force_author, N_("author"), N_("override author for commit")),
		OPT_STRING(0, "date", &force_date, N_("date"), N_("override date for commit")),
		OPT_CALLBACK('m', "message", &सन्देशः_2, N_("message"), N_("commit message"), opt_parse_m),
		OPT_STRING('c', "reedit-message", &edit_सन्देशः, N_("commit"), N_("reuse and edit message from specified commit")),
		OPT_STRING('C', "reuse-message", &use_सन्देशः, N_("commit"), N_("reuse message from specified commit")),
		/*
		 * TRANSLATORS: Leave "[(amend|reword):]" as-is,
		 * and only translate <commit>.
		 */
		OPT_STRING(0, "fixup", &fixup_सन्देशः, N_("[(amend|reword):]commit"), N_("use autosquash formatted message to fixup or amend/reword specified commit")),
		OPT_STRING(0, "squash", &squash_सन्देशः, N_("commit"), N_("use autosquash formatted message to squash specified commit")),
		OPT_BOOL(0, "reset-author", &renew_authorship, N_("the commit is authored by me now (used with -C/-c/--amend)")),
		OPT_STRVEC(0, "trailer", &trailer_तर्काः, N_("trailer"),
			   N_("add custom trailer(s)")),
		OPT_BOOL('s', "signoff", &signoff, N_("add a Signed-off-by trailer")),
		OPT_FILENAME('t', "template", &template_file, N_("use specified template file")),
		OPT_BOOL('e', "edit", &edit_flag, N_("force edit of commit")),
		OPT_CLEANUP(&cleanup_arg),
		OPT_BOOL(0, "status", &include_स्थितिः, N_("include status in commit message template")),
		{
			.type = OPTION_STRING,
			.short_name = 'S',
			.long_name = "gpg-sign",
			.मानम् = &sign_commit,
			.argh = N_("key-id"),
			.help = N_("GPG sign commit"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "",
		},
		/* end commit message options */

		OPT_गणः(N_("Commit contents options")),
		OPT_BOOL('a', "all", &all, N_("commit all changed files")),
		OPT_BOOL('i', "include", &also, N_("add specified files to index for commit")),
		OPT_BOOL(0, "interactive", &interactive, N_("interactively add files")),
		OPT_BOOL('p', "patch", &patch_interactive, N_("interactively add changes")),
		OPT_DIFF_UNIFIED(&interactive_opts.context),
		OPT_DIFF_INTERHUNK_CONTEXT(&interactive_opts.interhunkcontext),
		OPT_BOOL('o', "only", &only, N_("commit only specified files")),
		OPT_BOOL('n', "no-verify", &no_verify, N_("bypass pre-commit and commit-msg hooks")),
		OPT_BOOL(0, "dry-run", &dry_सततम्, N_("show what would be committed")),
		OPT_समुच्चयः_INT(0, "short", &स्थितिः_format, N_("show status concisely"),
			    स्थितिः_FORMAT_SHORT),
		OPT_BOOL(0, "branch", &s.show_branch, N_("show branch information")),
		OPT_BOOL(0, "ahead-behind", &s.ahead_behind_flags,
			 N_("compute full ahead/behind values")),
		OPT_समुच्चयः_INT(0, "porcelain", &स्थितिः_format,
			    N_("machine-readable output"), स्थितिः_FORMAT_PORCELAIN),
		OPT_समुच्चयः_INT(0, "long", &स्थितिः_format,
			    N_("show status in long format (default)"),
			    स्थितिः_FORMAT_LONG),
		OPT_BOOL('z', "null", &s.null_termination,
			 N_("terminate entries with NUL")),
		OPT_BOOL(0, "amend", &amend, N_("amend previous commit")),
		OPT_BOOL(0, "no-post-rewrite", &no_post_rewrite, N_("bypass post-rewrite hook")),
		{
			.type = OPTION_STRING,
			.short_name = 'u',
			.long_name = "untracked-files",
			.मानम् = &untracked_files_arg,
			.argh = N_("mode"),
			.help = N_("show untracked files, optional modes: all, normal, no. (Default: all)"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t)"all",
		},
		OPT_PATHSPEC_FROM_FILE(&pathspec_from_file),
		OPT_PATHSPEC_FILE_NUL(&pathspec_file_nul),
		/* end commit contents options */

		OPT_HIDDEN_BOOL(0, "allow-empty", &allow_रिक्तानि,
				N_("ok to record an empty change")),
		OPT_HIDDEN_BOOL(0, "allow-empty-message", &allow_रिक्तानि_सन्देशः,
				N_("ok to record a change with an empty message")),

		OPT_अन्तः()
	};

	संरचना strbuf sb = STRBUF_INIT;
	संरचना strbuf author_ident = STRBUF_INIT;
	ध्रुव वर्ण *सूचकांक_file, *reflog_सन्देशः;
	संरचना object_id oid;
	संरचना commit_list *parents = रिक्त;
	संरचना stat statbuf;
	संरचना commit *current_शीर्ष = रिक्त;
	संरचना commit_extra_शीर्षकम् *extra = रिक्त;
	संरचना strbuf err = STRBUF_INIT;
	पूर्णाङ्क ret = 0;

	show_usage_with_options_if_asked(argc, argv,
					 builtin_commit_usage, builtin_commit_options);

#यदि_अपरिभाषितम् WITH_BREAKING_CHANGES
	warn_on_auto_comment_char = true;
#यदि_अन्तः /* !WITH_BREAKING_CHANGES */
	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_पूर्णम्_सूचकांक = 0;

	स्थितिः_init_config(&s, git_commit_config);
	s.commit_template = 1;
	स्थितिः_format = स्थितिः_FORMAT_NONE; /* Ignore status.short */
	s.colopts = 0;

	यदि (repo_get_oid(the_repository, "HEAD", &oid))
		current_शीर्ष = रिक्त;
	अन्यथा {
		current_शीर्ष = lookup_commit_or_die(&oid, "HEAD");
		यदि (repo_parse_commit(the_repository, current_शीर्ष))
			die(_("could not parse HEAD commit"));
	}
	verbose = -1; /* unspecified */
	argc = parse_and_validate_options(argc, argv, builtin_commit_options,
					  builtin_commit_usage,
					  उपसर्गः, current_शीर्ष, &s);
	यदि (trailer_तर्काः.nr)
		trailer_config_init();

	यदि (verbose == -1)
		verbose = (config_commit_verbose < 0) ? 0 : config_commit_verbose;

	यदि (cleanup_arg) {
		मुक्त(cleanup_config);
		cleanup_config = xstrdup(cleanup_arg);
	}
	cleanup_बहुलकम् = get_cleanup_बहुलकम्(cleanup_config, use_editor);

	यदि (dry_सततम्)
		प्रत्यागम dry_सततम्_commit(argv, उपसर्गः, current_शीर्ष, &s);
	सूचकांक_file = prepare_सूचकांक(argv, उपसर्गः, current_शीर्ष, 0);

	/* Set up everything for writing the commit object.  This includes
	   running hooks, writing the trees, and interacting with the user.  */
	यदि (!prepare_गन्तव्यम्_commit(सूचकांक_file, उपसर्गः,
			       current_शीर्ष, &s, &author_ident)) {
		ret = 1;
		rollback_सूचकांक_files();
		गच्छ cleanup;
	}

	/* Determine parents */
	reflog_सन्देशः = getenv("GIT_REFLOG_ACTION");
	यदि (!current_शीर्ष) {
		यदि (!reflog_सन्देशः)
			reflog_सन्देशः = "commit (initial)";
	} अन्यथा यदि (amend) {
		यदि (!reflog_सन्देशः)
			reflog_सन्देशः = "commit (amend)";
		parents = commit_list_copy(current_शीर्ष->parents);
	} अन्यथा यदि (whence == FROM_MERGE) {
		संरचना strbuf m = STRBUF_INIT;
		FILE *fp;
		पूर्णाङ्क allow_fast_forward = 1;
		संरचना commit_list **pptr = &parents;

		यदि (!reflog_सन्देशः)
			reflog_सन्देशः = "commit (merge)";
		pptr = commit_list_append(current_शीर्ष, pptr);
		fp = xfopen(git_मार्गः_merge_शीर्ष(the_repository), "r");
		यावत् (strbuf_getline_lf(&m, fp) != EOF) {
			संरचना commit *parent;

			parent = get_merge_parent(m.buf);
			यदि (!parent)
				die(_("Corrupt MERGE_HEAD file (%s)"), m.buf);
			pptr = commit_list_append(parent, pptr);
		}
		fclose(fp);
		strbuf_release(&m);
		यदि (!stat(git_मार्गः_merge_बहुलकम्(the_repository), &statbuf)) {
			यदि (strbuf_read_file(&sb, git_मार्गः_merge_बहुलकम्(the_repository), 0) < 0)
				die_errno(_("could not read MERGE_MODE"));
			यदि (!वाक्यं_तुलय(sb.buf, "no-ff"))
				allow_fast_forward = 0;
		}
		यदि (allow_fast_forward)
			reduce_heads_replace(&parents);
	} अन्यथा {
		यदि (!reflog_सन्देशः)
			reflog_सन्देशः = is_from_cherry_pick(whence)
					? "commit (cherry-pick)"
					: is_from_rebase(whence)
					? "commit (rebase)"
					: "commit";
		commit_list_निवेशय(current_शीर्ष, &parents);
	}

	/* Finally, get the commit message */
	strbuf_reset(&sb);
	यदि (strbuf_read_file(&sb, git_मार्गः_commit_editmsg(), 0) < 0) {
		पूर्णाङ्क saved_errno = errno;
		rollback_सूचकांक_files();
		die(_("could not read commit message: %s"), strerror(saved_errno));
	}

	cleanup_सन्देशः(&sb, cleanup_बहुलकम्, verbose);

	यदि (सन्देशः_is_रिक्तानि(&sb, cleanup_बहुलकम्) && !allow_रिक्तानि_सन्देशः) {
		rollback_सूचकांक_files();
		fprintf(stderr, _("Aborting commit due to empty commit message.\n"));
		निर्गच्छ(1);
	}
	यदि (template_untouched(&sb, template_file, cleanup_बहुलकम्) && !allow_रिक्तानि_सन्देशः) {
		rollback_सूचकांक_files();
		fprintf(stderr, _("Aborting commit; you did not edit the message.\n"));
		निर्गच्छ(1);
	}

	यदि (fixup_सन्देशः && starts_with(sb.buf, "amend! ") &&
	    !allow_रिक्तानि_सन्देशः) {
		संरचना strbuf body = STRBUF_INIT;
		आकार_t len = commit_subject_दीर्घता(sb.buf);
		strbuf_addstr(&body, sb.buf + len);
		यदि (सन्देशः_is_रिक्तानि(&body, cleanup_बहुलकम्)) {
			rollback_सूचकांक_files();
			fprintf(stderr, _("Aborting commit due to empty commit message body.\n"));
			निर्गच्छ(1);
		}
		strbuf_release(&body);
	}

	यदि (amend) {
		ध्रुव वर्ण *exclude_gpgsig[3] = { "gpgsig", "gpgsig-sha256", रिक्त };
		extra = read_commit_extra_शीर्षकाणि(current_शीर्ष, exclude_gpgsig);
	} अन्यथा {
		संरचना commit_extra_शीर्षकम् **पुच्छ = &extra;
		append_merge_चिह्नकम्_शीर्षकाणि(parents, &पुच्छ);
	}

	यदि (commit_वृक्षः_extended(sb.buf, sb.len, &the_repository->सूचकांक->cache_वृक्षः->oid,
				 parents, &oid, author_ident.buf, रिक्त,
				 sign_commit, extra)) {
		rollback_सूचकांक_files();
		die(_("failed to write commit object"));
	}

	यदि (update_शीर्ष_with_reflog(current_शीर्ष, &oid, reflog_सन्देशः, &sb,
				    &err)) {
		rollback_सूचकांक_files();
		die("%s", err.buf);
	}

	sequencer_post_commit_cleanup(the_repository, 0);
	unlink(git_मार्गः_merge_शीर्ष(the_repository));
	unlink(git_मार्गः_merge_सन्देशः(the_repository));
	unlink(git_मार्गः_merge_बहुलकम्(the_repository));
	unlink(git_मार्गः_squash_सन्देशः(the_repository));

	यदि (commit_सूचकांक_files())
		die(_("repository has been updated, but unable to write\n"
		      "new index file. Check that disk is not full and quota is\n"
		      "not exceeded, and then \"git restore --staged :/\" to recover."));

	git_test_write_commit_graph_or_die(the_repository);

	repo_rerere(the_repository, 0);
	सततम्_auto_maintenance(the_repository, quiet);
	सततम्_commit_hook(use_editor, repo_get_सूचकांक_file(the_repository),
			रिक्त, "post-commit", रिक्त);
	यदि (amend && !no_post_rewrite) {
		commit_post_rewrite(the_repository, current_शीर्ष, &oid);
	}
	यदि (!quiet) {
		अचिह्नित पूर्णाङ्क flags = 0;

		यदि (!current_शीर्ष)
			flags |= SUMMARY_INITIAL_COMMIT;
		यदि (author_date_is_interesting())
			flags |= SUMMARY_SHOW_AUTHOR_DATE;
		print_commit_summary(the_repository, उपसर्गः,
				     &oid, flags);
	}

	apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
			    रिक्त, रिक्त, रिक्त, रिक्त);

cleanup:
	free_commit_extra_शीर्षकाणि(extra);
	commit_list_free(parents);
	strbuf_release(&author_ident);
	strbuf_release(&err);
	strbuf_release(&sb);
	मुक्त(logfile);
	मुक्त(template_file);
	प्रत्यागम ret;
}
