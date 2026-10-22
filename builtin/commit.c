/*
 * Builtin "git commit"
 *
 * Copyright (c) 2007 Kristian Høgsberg <krh@redhat.com>
 * Based on git-commit.sh by Junio C Hamano and Linus Torvalds
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "advice.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "lockfile.h"
#ଅନ୍ତର୍ଭାଵ "cache-tree.h"
#ଅନ୍ତର୍ଭାଵ "color.h"
#ଅନ୍ତର୍ଭାଵ "dir.h"
#ଅନ୍ତର୍ଭାଵ "editor.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "diff.h"
#ଅନ୍ତର୍ଭାଵ "commit.h"
#ଅନ୍ତର୍ଭାଵ "add-interactive.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "revision.h"
#ଅନ୍ତର୍ଭାଵ "wt-status.h"
#ଅନ୍ତର୍ଭାଵ "run-command.h"
#ଅନ୍ତର୍ଭାଵ "strbuf.h"
#ଅନ୍ତର୍ଭାଵ "object-name.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "path.h"
#ଅନ୍ତର୍ଭାଵ "preload-index.h"
#ଅନ୍ତର୍ଭାଵ "read-cache.h"
#ଅନ୍ତର୍ଭାଵ "repository.h"
#ଅନ୍ତର୍ଭାଵ "string-list.h"
#ଅନ୍ତର୍ଭାଵ "rerere.h"
#ଅନ୍ତର୍ଭାଵ "unpack-trees.h"
#ଅନ୍ତର୍ଭାଵ "column.h"
#ଅନ୍ତର୍ଭାଵ "sequencer.h"
#ଅନ୍ତର୍ଭାଵ "sparse-index.h"
#ଅନ୍ତର୍ଭାଵ "mailmap.h"
#ଅନ୍ତର୍ଭାଵ "help.h"
#ଅନ୍ତର୍ଭାଵ "commit-reach.h"
#ଅନ୍ତର୍ଭାଵ "commit-graph.h"
#ଅନ୍ତର୍ଭାଵ "pretty.h"
#ଅନ୍ତର୍ଭାଵ "trailer.h"

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ builtin_commit_usage[] = {
	N_("git commit [-a | --interactive | --patch] [-s] [-v] [-u[<mode>]] [--amend]\n"
	   "           [--dry-run] [(-c | -C | --squash) <commit> | --fixup [(amend|reword):]<commit>]\n"
	   "           [-F <file> | -m <msg>] [--reset-author] [--allow-empty]\n"
	   "           [--allow-empty-message] [--no-verify] [-e] [--author=<author>]\n"
	   "           [--date=<date>] [--cleanup=<mode>] [--[no-]status]\n"
	   "           [-i | -o] [--pathspec-from-file=<file> [--pathspec-file-nul]]\n"
	   "           [(--trailer <token>[(=|:)<value>])...] [-S[<keyid>]]\n"
	   "           [--] [<pathspec>...]"),
	ଖାଲି
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ builtin_status_usage[] = {
	N_("git status [<options>] [--] [<pathspec>...]"),
	ଖାଲି
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର empty_amend_advice[] =
N_("You asked to amend the most recent commit, but doing so would make\n"
"it empty. You can repeat your command with --allow-empty, or you can\n"
"remove the commit entirely with \"git reset HEAD^\".\n");

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର empty_cherry_pick_advice[] =
N_("The previous cherry-pick is now empty, possibly due to conflict resolution.\n"
"If you wish to commit it anyway, use:\n"
"\n"
"    git commit --allow-empty\n"
"\n");

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର empty_rebase_pick_advice[] =
N_("Otherwise, please use 'git rebase --skip'\n");

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର empty_cherry_pick_advice_single[] =
N_("Otherwise, please use 'git cherry-pick --skip'\n");

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର empty_cherry_pick_advice_multi[] =
N_("and then use:\n"
"\n"
"    git cherry-pick --continue\n"
"\n"
"to resume cherry-picking the remaining commits.\n"
"If you wish to skip this commit, use:\n"
"\n"
"    git cherry-pick --skip\n"
"\n");

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *color_status_slots[] = {
	[WT_STATUS_HEADER]	  = "header",
	[WT_STATUS_UPDATED]	  = "updated",
	[WT_STATUS_CHANGED]	  = "changed",
	[WT_STATUS_UNTRACKED]	  = "untracked",
	[WT_STATUS_NOBRANCH]	  = "noBranch",
	[WT_STATUS_UNMERGED]	  = "unmerged",
	[WT_STATUS_LOCAL_BRANCH]  = "localBranch",
	[WT_STATUS_REMOTE_BRANCH] = "remoteBranch",
	[WT_STATUS_ONBRANCH]	  = "branch",
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *use_message_buffer;
ସ୍ଥିର ସଂରଚନା lock_file index_lock; /* real index */
ସ୍ଥିର ସଂରଚନା lock_file false_lock; /* used only for partial commits */
ସ୍ଥିର ଗଣନା {
	COMMIT_AS_IS = 1,
	COMMIT_NORMAL,
	COMMIT_PARTIAL
} commit_style;

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *force_author;
ସ୍ଥିର ଅକ୍ଷର *logfile;
ସ୍ଥିର ଅକ୍ଷର *template_file;
/*
 * The _message variables are commit names from which to take
 * the commit message and/or authorship.
 */
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *author_message, *author_message_buffer;
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *edit_message, *use_message;
ସ୍ଥିର ଅକ୍ଷର *fixup_message, *fixup_commit, *squash_message;
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *fixup_prefix;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା all, also, interactive, patch_interactive, only, amend, signoff;
ସ୍ଥିର ସଂରଚନା interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା edit_flag = -1; /* unspecified */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା quiet, verbose, no_verify, allow_empty, dry_run, renew_authorship;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା config_commit_verbose = -1; /* unspecified */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା no_post_rewrite, allow_empty_message, pathspec_file_nul;
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *untracked_files_arg, *force_date, *ignore_submodule_arg, *ignored_arg;
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *sign_commit, *pathspec_from_file;
ସ୍ଥିର ସଂରଚନା strvec trailer_args = STRVEC_INIT;

/*
 * The default commit message cleanup mode will remove the lines
 * beginning with # (shell comments) and leading and trailing
 * whitespaces (empty lines or containing only whitespaces)
 * if editor is used, and only the whitespaces if the message
 * is specified explicitly.
 */
ସ୍ଥିର ଗଣନା commit_ସନ୍ଦେଶ_cleanup_mode cleanup_mode;
ସ୍ଥିର ଅକ୍ଷର *cleanup_config;

ସ୍ଥିର ଗଣନା commit_whence whence;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା use_editor = 1, include_status = 1;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା have_option_m;
ସ୍ଥିର ସଂରଚନା strbuf message = STRBUF_INIT;

ସ୍ଥିର ଗଣନା wt_status_format status_format = STATUS_FORMAT_UNSPECIFIED;

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା opt_parse_porcelain(ଅଚଳ ସଂରଚନା option *opt, ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ଗଣନା wt_status_format *ମୂଲ୍ୟ = (ଗଣନା wt_status_format *)opt->ମୂଲ୍ୟ;
	ଯଦି (unset)
		*ମୂଲ୍ୟ = STATUS_FORMAT_NONE;
	ଅନ୍ୟଥା ଯଦି (!arg)
		*ମୂଲ୍ୟ = STATUS_FORMAT_PORCELAIN;
	ଅନ୍ୟଥା ଯଦି (!strcmp(arg, "v1") || !strcmp(arg, "1"))
		*ମୂଲ୍ୟ = STATUS_FORMAT_PORCELAIN;
	ଅନ୍ୟଥା ଯଦି (!strcmp(arg, "v2") || !strcmp(arg, "2"))
		*ମୂଲ୍ୟ = STATUS_FORMAT_PORCELAIN_V2;
	ଅନ୍ୟଥା
		die("unsupported porcelain version '%s'", arg);

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା opt_parse_m(ଅଚଳ ସଂରଚନା option *opt, ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ସଂରଚନା strbuf *buf = opt->ମୂଲ୍ୟ;
	ଯଦି (unset) {
		have_option_m = 0;
		strbuf_setlen(buf, 0);
	} ଅନ୍ୟଥା {
		have_option_m = 1;
		ଯଦି (buf->len)
			strbuf_addch(buf, '\n');
		strbuf_addstr(buf, arg);
		strbuf_complete_ଧାଡ଼ି(buf);
	}
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା opt_parse_rename_score(ଅଚଳ ସଂରଚନା option *opt, ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ଅଚଳ ଅକ୍ଷର **ମୂଲ୍ୟ = opt->ମୂଲ୍ୟ;

	BUG_ON_OPT_NEG(unset);

	ଯଦି (arg != ଖାଲି && *arg == '=')
		arg = arg + 1;

	*ମୂଲ୍ୟ = arg;
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ determine_whence(ସଂରଚନା wt_status *s)
{
	ଯଦି (file_exists(git_path_merge_head(the_repository)))
		whence = FROM_MERGE;
	ଅନ୍ୟଥା ଯଦି (!sequencer_determine_whence(the_repository, &whence))
		whence = FROM_COMMIT;
	ଯଦି (s)
		s->whence = whence;
}

ସ୍ଥିର ଶୂନ୍ୟ status_init_config(ସଂରଚନା wt_status *s, config_fn_t fn)
{
	wt_status_prepare(the_repository, s);
	init_diff_ui_defaults();
	repo_config(the_repository, fn, s);
	determine_whence(s);
	s->hints = advice_enabled(ADVICE_STATUS_HINTS); /* must come after repo_config() */
}

ସ୍ଥିର ଶୂନ୍ୟ rollback_index_files(ଶୂନ୍ୟ)
{
	ବାଛ (commit_style) {
	ସ୍ଥିତି COMMIT_AS_IS:
		ଭାଙ୍ଗ; /* nothing to do */
	ସ୍ଥିତି COMMIT_NORMAL:
		rollback_lock_file(&index_lock);
		ଭାଙ୍ଗ;
	ସ୍ଥିତି COMMIT_PARTIAL:
		rollback_lock_file(&index_lock);
		rollback_lock_file(&false_lock);
		ଭାଙ୍ଗ;
	}
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା commit_index_files(ଶୂନ୍ୟ)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା err = 0;

	ବାଛ (commit_style) {
	ସ୍ଥିତି COMMIT_AS_IS:
		ଭାଙ୍ଗ; /* nothing to do */
	ସ୍ଥିତି COMMIT_NORMAL:
		err = commit_lock_file(&index_lock);
		ଭାଙ୍ଗ;
	ସ୍ଥିତି COMMIT_PARTIAL:
		err = commit_lock_file(&index_lock);
		rollback_lock_file(&false_lock);
		ଭାଙ୍ଗ;
	}

	ଫେରନ୍ତୁ err;
}

/*
 * Take a union of paths in the index and the named tree (typically, "HEAD"),
 * and return the paths that match the given pattern in list.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା list_paths(ସଂରଚନା string_list *list, ଅଚଳ ଅକ୍ଷର *with_ବୃକ୍ଷ,
		      ଅଚଳ ସଂରଚନା pathspec *pattern)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, ret;
	ଅକ୍ଷର *m;

	ଯଦି (!pattern->nr)
		ଫେରନ୍ତୁ 0;

	m = xcalloc(1, pattern->nr);

	ଯଦି (with_ବୃକ୍ଷ) {
		ଅକ୍ଷର *max_prefix = common_prefix(pattern);
		overlay_ବୃକ୍ଷ_on_index(the_repository->index, with_ବୃକ୍ଷ, max_prefix);
		ମୁକ୍ତ(max_prefix);
	}

	/* TODO: audit for interaction with sparse-index. */
	ensure_full_index(the_repository->index);
	ପାଇଁ (i = 0; i < the_repository->index->cache_nr; i++) {
		ଅଚଳ ସଂରଚନା cache_entry *ce = the_repository->index->cache[i];
		ସଂରଚନା string_list_item *item;

		ଯଦି (ce->ce_flags & CE_UPDATE)
			ଜାରି;
		ଯଦି (!ce_path_match(the_repository->index, ce, pattern, m))
			ଜାରି;
		item = string_list_ଭର୍ତ୍ତି(list, ce->name);
		ଯଦି (ce_skip_worktree(ce))
			item->util = item; /* better a valid pointer than a fake one */
	}

	ret = report_path_error(m, pattern);
	ମୁକ୍ତ(m);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ଶୂନ୍ୟ add_remove_files(ସଂରଚନା string_list *list)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	ପାଇଁ (i = 0; i < list->nr; i++) {
		ସଂରଚନା stat st;
		ସଂରଚନା string_list_item *p = &(list->ବସ୍ତୁଗୁଡ଼ିକ[i]);

		/* p->util is skip-worktree */
		ଯଦି (p->util)
			ଜାରି;

		ଯଦି (!lstat(p->string, &st)) {
			ଯଦି (add_to_index(the_repository->index, p->string, &st, 0))
				die(_("updating files failed"));
		} ଅନ୍ୟଥା
			remove_file_from_index(the_repository->index, p->string);
	}
}

ସ୍ଥିର ଶୂନ୍ୟ create_base_index(ଅଚଳ ସଂରଚନା commit *current_head)
{
	ସଂରଚନା ବୃକ୍ଷ *ବୃକ୍ଷ;
	ସଂରଚନା unpack_trees_options opts;
	ସଂରଚନା ବୃକ୍ଷ_desc t;

	ଯଦି (!current_head) {
		discard_index(the_repository->index);
		ଫେରନ୍ତୁ;
	}

	memset(&opts, 0, ଆକାର(opts));
	opts.head_idx = 1;
	opts.index_only = 1;
	opts.merge = 1;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;

	opts.fn = oneway_merge;
	ବୃକ୍ଷ = repo_parse_ବୃକ୍ଷ_indirect(the_repository,
					&current_head->object.oid);
	ଯଦି (!ବୃକ୍ଷ)
		die(_("failed to unpack HEAD tree object"));
	ଯଦି (repo_parse_ବୃକ୍ଷ(the_repository, ବୃକ୍ଷ) < 0)
		exit(128);
	init_ବୃକ୍ଷ_desc(&t, &ବୃକ୍ଷ->object.oid, ବୃକ୍ଷ->buffer, ବୃକ୍ଷ->size);
	ଯଦି (unpack_trees(1, &t, &opts))
		exit(128); /* We've already reported the error, finish dying */
}

ସ୍ଥିର ଶୂନ୍ୟ refresh_cache_or_die(ପୂର୍ଣ୍ଣସଂଖ୍ୟା refresh_flags)
{
	/*
	 * refresh_flags contains REFRESH_QUIET, so the only errors
	 * are for unmerged entries.
	 */
	ଯଦି (refresh_index(the_repository->index, refresh_flags | REFRESH_IN_PORCELAIN, ଖାଲି, ଖାଲି, ଖାଲି))
		die_resolve_conflict("commit");
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *prepare_index(ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
				 ଅଚଳ ସଂରଚନା commit *current_head, ପୂର୍ଣ୍ଣସଂଖ୍ୟା is_status)
{
	ସଂରଚନା string_list partial = STRING_LIST_INIT_DUP;
	ସଂରଚନା pathspec pathspec;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା refresh_flags = REFRESH_QUIET;
	ଅଚଳ ଅକ୍ଷର *ret;
	ଅକ୍ଷର *path = ଖାଲି;

	ଯଦି (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	ଯଦି (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	ଯଦି (is_status)
		refresh_flags |= REFRESH_UNMERGED;
	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_FULL,
		       prefix, argv);

	ଯଦି (pathspec_from_file) {
		ଯଦି (interactive)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--interactive/--patch");

		ଯଦି (all)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "-a");

		ଯଦି (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_FULL,
				    prefix, pathspec_from_file, pathspec_file_nul);
	} ଅନ୍ୟଥା ଯଦି (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	ଯଦି (!pathspec.nr && (also || (only && !allow_empty &&
	    (!amend || (fixup_message && strcmp(fixup_prefix, "amend"))))))
		die(_("No paths with --include/--only does not make sense."));

	ଯଦି (repo_read_index_preload(the_repository, &pathspec, 0) < 0)
		die(_("index file corrupt"));

	ଯଦି (interactive) {
		ଅକ୍ଷର *old_index_env = ଖାଲି, *old_repo_index_file;
		repo_hold_locked_index(the_repository, &index_lock,
				       LOCK_DIE_ON_ERROR);

		refresh_cache_or_die(refresh_flags);

		ଯଦି (write_locked_index(the_repository->index, &index_lock, 0))
			die(_("unable to create temporary index"));

		old_repo_index_file = the_repository->index_file;
		the_repository->index_file =
			(ଅକ୍ଷର *)get_lock_file_path(&index_lock);
		old_index_env = xstrdup_or_null(getenv(INDEX_ENVIRONMENT));
		setenv(INDEX_ENVIRONMENT, the_repository->index_file, 1);

		ଯଦି (interactive_add(the_repository, argv, prefix, patch_interactive, &interactive_opts) != 0)
			die(_("interactive add failed"));

		the_repository->index_file = old_repo_index_file;
		ଯଦି (old_index_env && *old_index_env)
			setenv(INDEX_ENVIRONMENT, old_index_env, 1);
		ଅନ୍ୟଥା
			unsetenv(INDEX_ENVIRONMENT);
		FREE_AND_NULL(old_index_env);

		discard_index(the_repository->index);
		read_index_from(the_repository->index, get_lock_file_path(&index_lock),
				repo_get_git_dir(the_repository));
		ଯଦି (cache_ବୃକ୍ଷ_update(the_repository->index, WRITE_ବୃକ୍ଷ_SILENT) == 0) {
			ଯଦି (reopen_lock_file(&index_lock) < 0)
				die(_("unable to write index file"));
			ଯଦି (write_locked_index(the_repository->index, &index_lock, 0))
				die(_("unable to update temporary index"));
		} ଅନ୍ୟଥା
			warning(_("Failed to update main cache tree"));

		commit_style = COMMIT_NORMAL;
		ret = get_lock_file_path(&index_lock);
		ଯାଅ out;
	} ଅନ୍ୟଥା {
		ଯଦି (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--interactive/--patch");
		ଯଦି (interactive_opts.interhunkcontext != -1)
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
	ଯଦି (all || (also && pathspec.nr)) {
		ଅକ୍ଷର *ps_matched = xcalloc(pathspec.nr, 1);
		repo_hold_locked_index(the_repository, &index_lock,
				       LOCK_DIE_ON_ERROR);
		add_files_to_cache(the_repository, also ? prefix : ଖାଲି,
				   &pathspec, ps_matched, 0, 0, 0 );
		ଯଦି (!all && report_path_error(ps_matched, &pathspec))
			exit(128);

		refresh_cache_or_die(refresh_flags);
		cache_ବୃକ୍ଷ_update(the_repository->index, WRITE_ବୃକ୍ଷ_SILENT);
		ଯଦି (write_locked_index(the_repository->index, &index_lock, 0))
			die(_("unable to write new index file"));
		commit_style = COMMIT_NORMAL;
		ret = get_lock_file_path(&index_lock);
		ମୁକ୍ତ(ps_matched);
		ଯାଅ out;
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
	ଯଦି (!only && !pathspec.nr) {
		repo_hold_locked_index(the_repository, &index_lock,
				       LOCK_DIE_ON_ERROR);
		refresh_cache_or_die(refresh_flags);
		ଯଦି (the_repository->index->cache_changed
		    || !cache_ବୃକ୍ଷ_fully_valid(the_repository->index))
			cache_ବୃକ୍ଷ_update(the_repository->index, WRITE_ବୃକ୍ଷ_SILENT);
		ଯଦି (write_locked_index(the_repository->index, &index_lock,
				       COMMIT_LOCK | SKIP_IF_UNCHANGED))
			die(_("unable to write new index file"));
		commit_style = COMMIT_AS_IS;
		ret = repo_get_index_file(the_repository);
		ଯାଅ out;
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

	ଯଦି (whence != FROM_COMMIT) {
		ଯଦି (whence == FROM_MERGE)
			die(_("cannot do a partial commit during a merge."));
		ଅନ୍ୟଥା ଯଦି (is_from_cherry_pick(whence))
			die(_("cannot do a partial commit during a cherry-pick."));
		ଅନ୍ୟଥା ଯଦି (is_from_rebase(whence))
			die(_("cannot do a partial commit during a rebase."));
	}

	ଯଦି (list_paths(&partial, !current_head ? ଖାଲି : "HEAD", &pathspec))
		exit(1);

	discard_index(the_repository->index);
	ଯଦି (repo_read_index(the_repository) < 0)
		die(_("cannot read the index"));

	repo_hold_locked_index(the_repository, &index_lock, LOCK_DIE_ON_ERROR);
	add_remove_files(&partial);
	refresh_index(the_repository->index, REFRESH_QUIET, ଖାଲି, ଖାଲି, ଖାଲି);
	cache_ବୃକ୍ଷ_update(the_repository->index, WRITE_ବୃକ୍ଷ_SILENT);
	ଯଦି (write_locked_index(the_repository->index, &index_lock, 0))
		die(_("unable to write new index file"));

	path = repo_git_path(the_repository, "next-index-%"PRIuMAX,
			     (uintmax_t) getpid());
	hold_lock_file_for_update(&false_lock, path,
				  LOCK_DIE_ON_ERROR);

	create_base_index(current_head);
	add_remove_files(&partial);
	refresh_index(the_repository->index, REFRESH_QUIET, ଖାଲି, ଖାଲି, ଖାଲି);

	ଯଦି (write_locked_index(the_repository->index, &false_lock, 0))
		die(_("unable to write temporary index file"));

	discard_index(the_repository->index);
	ret = get_lock_file_path(&false_lock);
	read_index_from(the_repository->index, ret, repo_get_git_dir(the_repository));
out:
	string_list_clear(&partial, 0);
	clear_pathspec(&pathspec);
	ମୁକ୍ତ(path);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା run_status(FILE *fp, ଅଚଳ ଅକ୍ଷର *index_file, ଅଚଳ ଅକ୍ଷର *prefix, ପୂର୍ଣ୍ଣସଂଖ୍ୟା nowarn,
		      ସଂରଚନା wt_status *s)
{
	ସଂରଚନା object_id oid;

	ଯଦି (s->relative_paths)
		s->prefix = prefix;

	ଯଦି (amend) {
		s->amend = 1;
		s->reference = "HEAD^1";
	}
	s->verbose = verbose;
	s->index_file = index_file;
	s->fp = fp;
	s->nowarn = nowarn;
	s->is_initial = repo_get_oid(the_repository, s->reference, &oid) ? 1 : 0;
	ଯଦି (!s->is_initial)
		oidcpy(&s->oid_commit, &oid);
	s->status_format = status_format;
	s->ignore_submodule_arg = ignore_submodule_arg;

	wt_status_collect(s);
	wt_status_print(s);
	wt_status_collect_free_buffers(s);

	ଫେରନ୍ତୁ s->committable;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା is_a_merge(ଅଚଳ ସଂରଚନା commit *current_head)
{
	ଫେରନ୍ତୁ !!(current_head->parents && current_head->parents->next);
}

ସ୍ଥିର ଶୂନ୍ୟ assert_split_ident(ସଂରଚନା ident_split *id, ଅଚଳ ସଂରଚନା strbuf *buf)
{
	ଯଦି (split_ident_ଧାଡ଼ି(id, buf->buf, buf->len) || !id->date_begin)
		BUG("unable to parse our own ident: %s", buf->buf);
}

ସ୍ଥିର ଶୂନ୍ୟ export_one(ଅଚଳ ଅକ୍ଷର *var, ଅଚଳ ଅକ୍ଷର *s, ଅଚଳ ଅକ୍ଷର *e, ପୂର୍ଣ୍ଣସଂଖ୍ୟା hack)
{
	ସଂରଚନା strbuf buf = STRBUF_INIT;
	ଯଦି (hack)
		strbuf_addch(&buf, hack);
	strbuf_add(&buf, s, e - s);
	setenv(var, buf.buf, 1);
	strbuf_release(&buf);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା parse_force_date(ଅଚଳ ଅକ୍ଷର *in, ସଂରଚନା strbuf *out)
{
	strbuf_addch(out, '@');

	ଯଦି (parse_date(in, out) < 0) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା errors = 0;
		unsigned long t = approxidate_careful(in, &errors);
		ଯଦି (errors)
			ଫେରନ୍ତୁ -1;
		strbuf_addf(out, "%lu", t);
	}

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ set_ident_var(ଅକ୍ଷର **buf, ଅକ୍ଷର *val)
{
	ମୁକ୍ତ(*buf);
	*buf = val;
}

ସ୍ଥିର ଶୂନ୍ୟ determine_author_info(ସଂରଚନା strbuf *author_ident)
{
	ଅକ୍ଷର *name, *email, *date;
	ସଂରଚନା ident_split author;

	name = xstrdup_or_null(getenv("GIT_AUTHOR_NAME"));
	email = xstrdup_or_null(getenv("GIT_AUTHOR_EMAIL"));
	date = xstrdup_or_null(getenv("GIT_AUTHOR_DATE"));

	ଯଦି (author_message) {
		ସଂରଚନା ident_split ident;
		size_t len;
		ଅଚଳ ଅକ୍ଷର *a;

		a = find_commit_header(author_message_buffer, "author", &len);
		ଯଦି (!a)
			die(_("commit '%s' lacks author header"), author_message);
		ଯଦି (split_ident_ଧାଡ଼ି(&ident, a, len) < 0)
			die(_("commit '%s' has malformed author line"), author_message);

		set_ident_var(&name, xmemdupz(ident.name_begin, ident.name_end - ident.name_begin));
		set_ident_var(&email, xmemdupz(ident.mail_begin, ident.mail_end - ident.mail_begin));

		ଯଦି (ident.date_begin) {
			ସଂରଚନା strbuf date_buf = STRBUF_INIT;
			strbuf_addch(&date_buf, '@');
			strbuf_add(&date_buf, ident.date_begin, ident.date_end - ident.date_begin);
			strbuf_addch(&date_buf, ' ');
			strbuf_add(&date_buf, ident.tz_begin, ident.tz_end - ident.tz_begin);
			set_ident_var(&date, strbuf_detach(&date_buf, ଖାଲି));
		}
	}

	ଯଦି (force_author) {
		ସଂରଚନା ident_split ident;

		ଯଦି (split_ident_ଧାଡ଼ି(&ident, force_author, strlen(force_author)) < 0)
			die(_("malformed --author parameter"));
		set_ident_var(&name, xmemdupz(ident.name_begin, ident.name_end - ident.name_begin));
		set_ident_var(&email, xmemdupz(ident.mail_begin, ident.mail_end - ident.mail_begin));
	}

	ଯଦି (force_date) {
		ସଂରଚନା strbuf date_buf = STRBUF_INIT;
		ଯଦି (parse_force_date(force_date, &date_buf))
			die(_("invalid date format: %s"), force_date);
		set_ident_var(&date, strbuf_detach(&date_buf, ଖାଲି));
	}

	strbuf_addstr(author_ident, fmt_ident(name, email, WANT_AUTHOR_IDENT, date,
				IDENT_STRICT));
	assert_split_ident(&author, author_ident);
	export_one("GIT_AUTHOR_NAME", author.name_begin, author.name_end, 0);
	export_one("GIT_AUTHOR_EMAIL", author.mail_begin, author.mail_end, 0);
	export_one("GIT_AUTHOR_DATE", author.date_begin, author.tz_end, '@');
	ମୁକ୍ତ(name);
	ମୁକ୍ତ(email);
	ମୁକ୍ତ(date);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା author_date_is_interesting(ଶୂନ୍ୟ)
{
	ଫେରନ୍ତୁ author_message || force_date;
}

#ifndef WITH_BREAKING_CHANGES
ସ୍ଥିର ଶୂନ୍ୟ adjust_comment_ଧାଡ଼ି_char(ଅଚଳ ସଂରଚନା strbuf *sb)
{
	ଅକ୍ଷର candidates[] = "#;@!$%^&|:";
	ଅକ୍ଷର *candidate;
	ଅଚଳ ଅକ୍ଷର *p;
	size_t cutoff;

	/* Ignore comment chars in trailing comments (e.g., Conflicts:) */
	cutoff = sb->len - ignored_log_message_bytes(sb->buf, sb->len);

	ଯଦି (!memchr(sb->buf, candidates[0], sb->len)) {
		ମୁକ୍ତ(comment_ଧାଡ଼ି_str_to_free);
		comment_ଧାଡ଼ି_str = comment_ଧାଡ଼ି_str_to_free =
			xstrfmt("%c", candidates[0]);
		ଫେରନ୍ତୁ;
	}

	p = sb->buf;
	candidate = strchr(candidates, *p);
	ଯଦି (candidate)
		*candidate = ' ';
	ପାଇଁ (p = sb->buf; p + 1 < sb->buf + cutoff; p++) {
		ଯଦି ((p[0] == '\n' || p[0] == '\r') && p[1]) {
			candidate = strchr(candidates, p[1]);
			ଯଦି (candidate)
				*candidate = ' ';
		}
	}

	ପାଇଁ (p = candidates; *p == ' '; p++)
		;
	ଯଦି (!*p)
		die(_("unable to select a comment character that is not used\n"
		      "in the current commit message"));
	ମୁକ୍ତ(comment_ଧାଡ଼ି_str_to_free);
	comment_ଧାଡ଼ି_str = comment_ଧାଡ଼ି_str_to_free = xstrfmt("%c", *p);
}
#endif /* !WITH_BREAKING_CHANGES */

ସ୍ଥିର ଶୂନ୍ୟ prepare_amend_commit(ସଂରଚନା commit *commit, ସଂରଚନା strbuf *sb,
				ସଂରଚନା pretty_print_context *ctx)
{
	ଅଚଳ ଅକ୍ଷର *buffer, *subject, *fmt;

	buffer = repo_get_commit_buffer(the_repository, commit, ଖାଲି);
	find_commit_subject(buffer, &subject);
	/*
	 * If we amend the 'amend!' commit then we don't want to
	 * duplicate the subject line.
	 */
	fmt = starts_with(subject, "amend!") ? "%b" : "%B";
	repo_format_commit_message(the_repository, commit, fmt, sb, ctx);
	repo_unuse_commit_buffer(the_repository, commit, buffer);
}

ସ୍ଥିର ଶୂନ୍ୟ change_data_free(ଶୂନ୍ୟ *util, ଅଚଳ ଅକ୍ଷର *str UNUSED)
{
	ସଂରଚନା wt_status_change_data *d = util;
	ମୁକ୍ତ(d->rename_source);
	ମୁକ୍ତ(d);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା prepare_to_commit(ଅଚଳ ଅକ୍ଷର *index_file, ଅଚଳ ଅକ୍ଷର *prefix,
			     ସଂରଚନା commit *current_head,
			     ସଂରଚନା wt_status *s,
			     ସଂରଚନା strbuf *author_ident)
{
	ସଂରଚନା stat statbuf;
	ସଂରଚନା strbuf committer_ident = STRBUF_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା committable;
	ସଂରଚନା strbuf sb = STRBUF_INIT;
	ଅଚଳ ଅକ୍ଷର *hook_arg1 = ଖାଲି;
	ଅଚଳ ଅକ୍ଷର *hook_arg2 = ଖାଲି;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା clean_message_contents = (cleanup_mode != COMMIT_ସନ୍ଦେଶ_CLEANUP_NONE);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା old_display_comment_prefix;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା invoked_hook;

	/* This checks and barfs if author is badly specified */
	determine_author_info(author_ident);

	ଯଦି (!no_verify && run_commit_hook(use_editor, index_file, &invoked_hook,
					  "pre-commit", ଖାଲି))
		ଫେରନ୍ତୁ 0;

	ଯଦି (squash_message) {
		/*
		 * Insert the proper subject line before other commit
		 * message options add their content.
		 */
		ଯଦି (use_message && !strcmp(use_message, squash_message))
			strbuf_addstr(&sb, "squash! ");
		ଅନ୍ୟଥା {
			ସଂରଚନା pretty_print_context ctx = {0};
			ସଂରଚନା commit *c;
			c = lookup_commit_reference_by_name(squash_message);
			ଯଦି (!c)
				die(_("could not lookup commit '%s'"), squash_message);
			ctx.output_encoding = get_commit_output_encoding();
			repo_format_commit_message(the_repository, c,
						   "squash! %s\n\n", &sb,
						   &ctx);
		}
	}

	ଯଦି (have_option_m && !fixup_message) {
		strbuf_addbuf(&sb, &message);
		hook_arg1 = "message";
	} ଅନ୍ୟଥା ଯଦି (logfile && !strcmp(logfile, "-")) {
		ଯଦି (isatty(0))
			fprintf(stderr, _("(reading log message from standard input)\n"));
		ଯଦି (strbuf_read(&sb, 0, 0) < 0)
			die_errno(_("could not read log from standard input"));
		hook_arg1 = "message";
	} ଅନ୍ୟଥା ଯଦି (logfile) {
		ଯଦି (strbuf_read_file(&sb, logfile, 0) < 0)
			die_errno(_("could not read log file '%s'"),
				  logfile);
		hook_arg1 = "message";
	} ଅନ୍ୟଥା ଯଦି (use_message) {
		ଅଚଳ ଅକ୍ଷର *buffer;
		buffer = strstr(use_message_buffer, "\n\n");
		ଯଦି (buffer)
			strbuf_addstr(&sb, skip_blank_ଧାଡ଼ିଗୁଡ଼ିକ(buffer + 2));
		hook_arg1 = "commit";
		hook_arg2 = use_message;
	} ଅନ୍ୟଥା ଯଦି (fixup_message) {
		ସଂରଚନା pretty_print_context ctx = {0};
		ସଂରଚନା commit *commit;
		ଅକ୍ଷର *fmt;
		commit = lookup_commit_reference_by_name(fixup_commit);
		ଯଦି (!commit)
			die(_("could not lookup commit '%s'"), fixup_commit);
		ctx.output_encoding = get_commit_output_encoding();
		fmt = xstrfmt("%s! %%s\n\n", fixup_prefix);
		repo_format_commit_message(the_repository, commit, fmt, &sb,
					   &ctx);
		ମୁକ୍ତ(fmt);
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
		ଯଦି (have_option_m && !strcmp(fixup_prefix, "fixup"))
			strbuf_addbuf(&sb, &message);

		ଯଦି (!strcmp(fixup_prefix, "amend")) {
			ଯଦି (have_option_m)
				die(_("options '%s' and '%s:%s' cannot be used together"), "-m", "--fixup", fixup_message);
			prepare_amend_commit(commit, &sb, &ctx);
		}
	} ଅନ୍ୟଥା ଯଦି (!stat(git_path_merge_ସନ୍ଦେଶ(the_repository), &statbuf)) {
		size_t merge_ସନ୍ଦେଶ_start;

		/*
		 * prepend SQUASH_MSG here if it exists and a
		 * "merge --squash" was originally performed
		 */
		ଯଦି (!stat(git_path_squash_ସନ୍ଦେଶ(the_repository), &statbuf)) {
			ଯଦି (strbuf_read_file(&sb, git_path_squash_ସନ୍ଦେଶ(the_repository), 0) < 0)
				die_errno(_("could not read SQUASH_MSG"));
			hook_arg1 = "squash";
		} ଅନ୍ୟଥା
			hook_arg1 = "merge";

		merge_ସନ୍ଦେଶ_start = sb.len;
		ଯଦି (strbuf_read_file(&sb, git_path_merge_ସନ୍ଦେଶ(the_repository), 0) < 0)
			die_errno(_("could not read MERGE_MSG"));

		ଯଦି (cleanup_mode == COMMIT_ସନ୍ଦେଶ_CLEANUP_SCISSORS &&
		    wt_status_locate_end(sb.buf + merge_ସନ୍ଦେଶ_start,
					 sb.len - merge_ସନ୍ଦେଶ_start) <
				sb.len - merge_ସନ୍ଦେଶ_start)
			s->added_cut_ଧାଡ଼ି = 1;
	} ଅନ୍ୟଥା ଯଦି (!stat(git_path_squash_ସନ୍ଦେଶ(the_repository), &statbuf)) {
		ଯଦି (strbuf_read_file(&sb, git_path_squash_ସନ୍ଦେଶ(the_repository), 0) < 0)
			die_errno(_("could not read SQUASH_MSG"));
		hook_arg1 = "squash";
	} ଅନ୍ୟଥା ଯଦି (template_file) {
		ଯଦି (strbuf_read_file(&sb, template_file, 0) < 0)
			die_errno(_("could not read '%s'"), template_file);
		hook_arg1 = "template";
		clean_message_contents = 0;
	}

	/*
	 * The remaining cases don't modify the template message, but
	 * just set the argument(s) to the prepare-commit-msg hook.
	 */
	ଅନ୍ୟଥା ଯଦି (whence == FROM_MERGE)
		hook_arg1 = "merge";
	ଅନ୍ୟଥା ଯଦି (is_from_cherry_pick(whence) || whence == FROM_REBASE_PICK) {
		hook_arg1 = "commit";
		hook_arg2 = "CHERRY_PICK_HEAD";
	}

	ଯଦି (squash_message) {
		/*
		 * If squash_commit was used for the commit subject,
		 * then we're possibly hijacking other commit log options.
		 * Reset the hook args to tell the real story.
		 */
		hook_arg1 = "message";
		hook_arg2 = "";
	}

	s->fp = fopen_for_writing(git_path_commit_editmsg());
	ଯଦି (!s->fp)
		die_errno(_("could not open '%s'"), git_path_commit_editmsg());

	/* Ignore status.displayCommentPrefix: we do need comments in COMMIT_EDITMSG. */
	old_display_comment_prefix = s->display_comment_prefix;
	s->display_comment_prefix = 1;

	/*
	 * Most hints are counter-productive when the commit has
	 * already started.
	 */
	s->hints = 0;

	ଯଦି (clean_message_contents)
		strbuf_stripspace(&sb, ଖାଲି);

	ଯଦି (signoff)
		append_signoff(&sb, ignored_log_message_bytes(sb.buf, sb.len), 0);

	ଯଦି (fwrite(sb.buf, 1, sb.len, s->fp) < sb.len)
		die_errno(_("could not write commit template"));

#ifndef WITH_BREAKING_CHANGES
	ଯଦି (auto_comment_ଧାଡ଼ି_char)
		adjust_comment_ଧାଡ଼ି_char(&sb);
#endif /* !WITH_BREAKING_CHANGES */
	strbuf_release(&sb);

	/* This checks if committer ident is explicitly given */
	strbuf_addstr(&committer_ident, git_committer_info(IDENT_STRICT));
	ଯଦି (use_editor && include_status) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା ident_shown = 0;
		ଗଣନା git_colorbool saved_color_setting;
		ସଂରଚନା ident_split ci, ai;
		ଅଚଳ ଅକ୍ଷର *hint_cleanup_all = allow_empty_message ?
			_("Please enter the commit message for your changes."
			  " Lines starting\nwith '%s' will be ignored.\n") :
			_("Please enter the commit message for your changes."
			  " Lines starting\nwith '%s' will be ignored, and an empty"
			  " message aborts the commit.\n");
		ଅଚଳ ଅକ୍ଷର *hint_cleanup_space = allow_empty_message ?
			_("Please enter the commit message for your changes."
			  " Lines starting\n"
			  "with '%s' will be kept; you may remove them"
			  " yourself if you want to.\n") :
			_("Please enter the commit message for your changes."
			  " Lines starting\n"
			  "with '%s' will be kept; you may remove them"
			  " yourself if you want to.\n"
			  "An empty message aborts the commit.\n");
		ଯଦି (whence != FROM_COMMIT) {
			ଯଦି (cleanup_mode == COMMIT_ସନ୍ଦେଶ_CLEANUP_SCISSORS)
				wt_status_add_cut_ଧାଡ଼ି(s);
			status_printf_ln(
				s, GIT_COLOR_NORMAL,
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
		ଯଦି (cleanup_mode == COMMIT_ସନ୍ଦେଶ_CLEANUP_ALL)
			status_printf(s, GIT_COLOR_NORMAL, hint_cleanup_all, comment_ଧାଡ଼ି_str);
		ଅନ୍ୟଥା ଯଦି (cleanup_mode == COMMIT_ସନ୍ଦେଶ_CLEANUP_SCISSORS) {
			ଯଦି (whence == FROM_COMMIT)
				wt_status_add_cut_ଧାଡ଼ି(s);
		} ଅନ୍ୟଥା /* COMMIT_MSG_CLEANUP_SPACE, that is. */
			status_printf(s, GIT_COLOR_NORMAL, hint_cleanup_space, comment_ଧାଡ଼ି_str);

		/*
		 * These should never fail because they come from our own
		 * fmt_ident. They may fail the sane_ident test, but we know
		 * that the name and mail pointers will at least be valid,
		 * which is enough for our tests and printing here.
		 */
		assert_split_ident(&ai, author_ident);
		assert_split_ident(&ci, &committer_ident);

		ଯଦି (ident_cmp(&ai, &ci))
			status_printf_ln(s, GIT_COLOR_NORMAL,
				_("%s"
				"Author:    %.*s <%.*s>"),
				ident_shown++ ? "" : "\n",
				(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(ai.name_end - ai.name_begin), ai.name_begin,
				(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(ai.mail_end - ai.mail_begin), ai.mail_begin);

		ଯଦି (author_date_is_interesting())
			status_printf_ln(s, GIT_COLOR_NORMAL,
				_("%s"
				"Date:      %s"),
				ident_shown++ ? "" : "\n",
				show_ident_date(&ai, DATE_MODE(NORMAL)));

		ଯଦି (!committer_ident_sufficiently_given())
			status_printf_ln(s, GIT_COLOR_NORMAL,
				_("%s"
				"Committer: %.*s <%.*s>"),
				ident_shown++ ? "" : "\n",
				(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(ci.name_end - ci.name_begin), ci.name_begin,
				(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(ci.mail_end - ci.mail_begin), ci.mail_begin);

		status_printf_ln(s, GIT_COLOR_NORMAL, "%s", ""); /* Add new line for clarity */

		saved_color_setting = s->use_color;
		s->use_color = GIT_COLOR_NEVER;
		committable = run_status(s->fp, index_file, prefix, 1, s);
		s->use_color = saved_color_setting;
		string_list_clear_func(&s->change, change_data_free);
	} ଅନ୍ୟଥା {
		ସଂରଚନା object_id oid;
		ଅଚଳ ଅକ୍ଷର *parent = "HEAD";

		ଯଦି (!the_repository->index->initialized && repo_read_index(the_repository) < 0)
			die(_("Cannot read index"));

		ଯଦି (amend)
			parent = "HEAD^1";

		ଯଦି (repo_get_oid(the_repository, parent, &oid)) {
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, ita_nr = 0;

			/* TODO: audit for interaction with sparse-index. */
			ensure_full_index(the_repository->index);
			ପାଇଁ (i = 0; i < the_repository->index->cache_nr; i++)
				ଯଦି (ce_intent_to_add(the_repository->index->cache[i]))
					ita_nr++;
			committable = the_repository->index->cache_nr > ita_nr;
		} ଅନ୍ୟଥା {
			/*
			 * Unless the user did explicitly request a submodule
			 * ignore mode by passing a command line option we do
			 * not ignore any changed submodule SHA-1s when
			 * comparing index and parent, no matter what is
			 * configured. Otherwise we won't commit any
			 * submodules which were manually staged, which would
			 * be really confusing.
			 */
			ସଂରଚନା diff_flags flags = DIFF_FLAGS_INIT;
			flags.override_submodule_config = 1;
			ଯଦି (ignore_submodule_arg &&
			    !strcmp(ignore_submodule_arg, "all"))
				flags.ignore_submodules = 1;
			committable = index_differs_from(the_repository,
							 parent, &flags, 1);
		}
	}
	strbuf_release(&committer_ident);

	fclose(s->fp);

	ଯଦି (trailer_args.nr) {
		ଯଦି (amend_file_with_trailers(git_path_commit_editmsg(), &trailer_args))
			die(_("unable to pass trailers to --trailers"));
		strvec_clear(&trailer_args);
	}

	/*
	 * Reject an attempt to record a non-merge empty commit without
	 * explicit --allow-empty. In the cherry-pick case, it may be
	 * empty due to conflict resolution, which the user should okay.
	 */
	ଯଦି (!committable && whence != FROM_MERGE && !allow_empty &&
	    !(amend && is_a_merge(current_head))) {
		s->hints = advice_enabled(ADVICE_STATUS_HINTS);
		s->display_comment_prefix = old_display_comment_prefix;
		run_status(stdout, index_file, prefix, 0, s);
		ଯଦି (amend)
			fputs(_(empty_amend_advice), stderr);
		ଅନ୍ୟଥା ଯଦି (is_from_cherry_pick(whence) ||
			 whence == FROM_REBASE_PICK) {
			fputs(_(empty_cherry_pick_advice), stderr);
			ଯଦି (whence == FROM_CHERRY_PICK_SINGLE)
				fputs(_(empty_cherry_pick_advice_single), stderr);
			ଅନ୍ୟଥା ଯଦି (whence == FROM_CHERRY_PICK_MULTI)
				fputs(_(empty_cherry_pick_advice_multi), stderr);
			ଅନ୍ୟଥା
				fputs(_(empty_rebase_pick_advice), stderr);
		}
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (!no_verify && invoked_hook) {
		/*
		 * Re-read the index as the pre-commit-commit hook was invoked
		 * and could have updated it. We must do this before we invoke
		 * the editor and after we invoke run_status above.
		 */
		discard_index(the_repository->index);
	}
	read_index_from(the_repository->index, index_file, repo_get_git_dir(the_repository));

	ଯଦି (cache_ବୃକ୍ଷ_update(the_repository->index, 0)) {
		error(_("Error building trees"));
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (run_commit_hook(use_editor, index_file, ଖାଲି, "prepare-commit-msg",
			    git_path_commit_editmsg(), hook_arg1, hook_arg2, ଖାଲି))
		ଫେରନ୍ତୁ 0;

	ଯଦି (use_editor) {
		ସଂରଚନା strvec env = STRVEC_INIT;

		strvec_pushf(&env, "GIT_INDEX_FILE=%s", index_file);
		ଯଦି (launch_editor(git_path_commit_editmsg(), ଖାଲି, env.v)) {
			fprintf(stderr,
			_("Please supply the message using either -m or -F option.\n"));
			exit(1);
		}
		strvec_clear(&env);
	}

	ଯଦି (!no_verify &&
	    run_commit_hook(use_editor, index_file, ଖାଲି, "commit-msg",
			    git_path_commit_editmsg(), ଖାଲି)) {
		ଫେରନ୍ତୁ 0;
	}

	ଫେରନ୍ତୁ 1;
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *find_author_by_nickname(ଅଚଳ ଅକ୍ଷର *name)
{
	ସଂରଚନା rev_info revs;
	ସଂରଚନା commit *commit;
	ସଂରଚନା strbuf buf = STRBUF_INIT;
	ଅଚଳ ଅକ୍ଷର *av[20];
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ac = 0;

	repo_init_revisions(the_repository, &revs, ଖାଲି);
	strbuf_addf(&buf, "--author=%s", name);
	av[++ac] = "--all";
	av[++ac] = "-i";
	av[++ac] = buf.buf;
	av[++ac] = ଖାଲି;
	setup_revisions(ac, av, &revs, ଖାଲି);
	revs.mailmap = xmalloc(ଆକାର(ସଂରଚନା string_list));
	string_list_init_nodup(revs.mailmap);
	read_mailmap(the_repository, revs.mailmap);

	ଯଦି (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	commit = get_revision(&revs);
	ଯଦି (commit) {
		ସଂରଚନା pretty_print_context ctx = {0};
		ctx.date_mode.type = DATE_NORMAL;
		strbuf_release(&buf);
		repo_format_commit_message(the_repository, commit,
					   "%aN <%aE>", &buf, &ctx);
		release_revisions(&revs);
		ଫେରନ୍ତୁ strbuf_detach(&buf, ଖାଲି);
	}
	die(_("--author '%s' is not 'Name <email>' and matches no existing author"), name);
}

ସ୍ଥିର ଶୂନ୍ୟ handle_ignored_arg(ସଂରଚନା wt_status *s)
{
	ଯଦି (!ignored_arg)
		; /* default already initialized */
	ଅନ୍ୟଥା ଯଦି (!strcmp(ignored_arg, "traditional"))
		s->show_ignored_mode = SHOW_TRADITIONAL_IGNORED;
	ଅନ୍ୟଥା ଯଦି (!strcmp(ignored_arg, "no"))
		s->show_ignored_mode = SHOW_NO_IGNORED;
	ଅନ୍ୟଥା ଯଦି (!strcmp(ignored_arg, "matching"))
		s->show_ignored_mode = SHOW_MATCHING_IGNORED;
	ଅନ୍ୟଥା
		die(_("Invalid ignored mode '%s'"), ignored_arg);
}

ସ୍ଥିର ଗଣନା untracked_status_type parse_untracked_setting_name(ଅଚଳ ଅକ୍ଷର *u)
{
	/*
	 * Please update $__git_untracked_file_modes in
	 * git-completion.bash when you add new options
	 */
	ବାଛ (git_parse_maybe_bool(u)) {
	ସ୍ଥିତି 0:
		u = "no";
		ଭାଙ୍ଗ;
	ସ୍ଥିତି 1:
		u = "normal";
		ଭାଙ୍ଗ;
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
		ଭାଙ୍ଗ;
	}

	ଯଦି (!strcmp(u, "no"))
		ଫେରନ୍ତୁ SHOW_NO_UNTRACKED_FILES;
	ଅନ୍ୟଥା ଯଦି (!strcmp(u, "normal"))
		ଫେରନ୍ତୁ SHOW_NORMAL_UNTRACKED_FILES;
	ଅନ୍ୟଥା ଯଦି (!strcmp(u, "all"))
		ଫେରନ୍ତୁ SHOW_ALL_UNTRACKED_FILES;
	ଅନ୍ୟଥା
		ଫେରନ୍ତୁ SHOW_UNTRACKED_FILES_ERROR;
}

ସ୍ଥିର ଶୂନ୍ୟ handle_untracked_files_arg(ସଂରଚନା wt_status *s)
{
	ଗଣନା untracked_status_type u;

	ଯଦି (!untracked_files_arg)
		ଫେରନ୍ତୁ; /* default already initialized */

	u = parse_untracked_setting_name(untracked_files_arg);
	ଯଦି (u == SHOW_UNTRACKED_FILES_ERROR)
		die(_("Invalid untracked files mode '%s'"),
		    untracked_files_arg);
	s->show_untracked_files = u;
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *read_commit_message(ଅଚଳ ଅକ୍ଷର *name)
{
	ଅଚଳ ଅକ୍ଷର *out_enc;
	ସଂରଚନା commit *commit;

	commit = lookup_commit_reference_by_name(name);
	ଯଦି (!commit)
		die(_("could not lookup commit '%s'"), name);
	out_enc = get_commit_output_encoding();
	ଫେରନ୍ତୁ repo_logmsg_reencode(the_repository, commit, ଖାଲି, out_enc);
}

/*
 * Enumerate what needs to be propagated when --porcelain
 * is not in effect here.
 */
ସ୍ଥିର ସଂରଚନା status_deferred_config {
	ଗଣନା wt_status_format status_format;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_branch;
	ଗଣନା ahead_behind_flags ahead_behind;
} status_deferred_config = {
	STATUS_FORMAT_UNSPECIFIED,
	-1, /* unspecified */
	AHEAD_BEHIND_UNSPECIFIED,
};

ସ୍ଥିର ଶୂନ୍ୟ finalize_deferred_config(ସଂରଚନା wt_status *s)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା use_deferred_config = (status_format != STATUS_FORMAT_PORCELAIN &&
				   status_format != STATUS_FORMAT_PORCELAIN_V2 &&
				   !s->null_termination);

	ଯଦି (s->null_termination) {
		ଯଦି (status_format == STATUS_FORMAT_NONE ||
		    status_format == STATUS_FORMAT_UNSPECIFIED)
			status_format = STATUS_FORMAT_PORCELAIN;
		ଅନ୍ୟଥା ଯଦି (status_format == STATUS_FORMAT_LONG)
			die(_("options '%s' and '%s' cannot be used together"), "--long", "-z");
	}

	ଯଦି (use_deferred_config && status_format == STATUS_FORMAT_UNSPECIFIED)
		status_format = status_deferred_config.status_format;
	ଯଦି (status_format == STATUS_FORMAT_UNSPECIFIED)
		status_format = STATUS_FORMAT_NONE;

	ଯଦି (use_deferred_config && s->show_branch < 0)
		s->show_branch = status_deferred_config.show_branch;
	ଯଦି (s->show_branch < 0)
		s->show_branch = 0;

	/*
	 * If the user did not give a "--[no]-ahead-behind" command
	 * line argument *AND* we will print in a human-readable format
	 * (short, long etc.) then we inherit from the status.aheadbehind
	 * config setting.  In all other cases (and porcelain V[12] formats
	 * in particular), we inherit _FULL for backwards compatibility.
	 */
	ଯଦି (use_deferred_config &&
	    s->ahead_behind_flags == AHEAD_BEHIND_UNSPECIFIED)
		s->ahead_behind_flags = status_deferred_config.ahead_behind;

	ଯଦି (s->ahead_behind_flags == AHEAD_BEHIND_UNSPECIFIED)
		s->ahead_behind_flags = AHEAD_BEHIND_FULL;
}

ସ୍ଥିର ଶୂନ୍ୟ check_fixup_reword_options(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର *argv[]) {
	ଯଦି (whence != FROM_COMMIT) {
		ଯଦି (whence == FROM_MERGE)
			die(_("You are in the middle of a merge -- cannot reword."));
		ଅନ୍ୟଥା ଯଦି (is_from_cherry_pick(whence))
			die(_("You are in the middle of a cherry-pick -- cannot reword."));
	}
	ଯଦି (argc)
		die(_("reword option of '%s' and path '%s' cannot be used together"), "--fixup", *argv);
	ଯଦି (patch_interactive || interactive || all || also || only)
		die(_("reword option of '%s' and '%s' cannot be used together"),
			"--fixup", "--patch/--interactive/--all/--include/--only");
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା parse_and_validate_options(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର *argv[],
				      ଅଚଳ ସଂରଚନା option *options,
				      ଅଚଳ ଅକ୍ଷର * ଅଚଳ usage[],
				      ଅଚଳ ଅକ୍ଷର *prefix,
				      ସଂରଚନା commit *current_head,
				      ସଂରଚନା wt_status *s)
{
	argc = parse_options(argc, argv, prefix, options, usage, 0);
	finalize_deferred_config(s);

	ଯଦି (force_author && !strchr(force_author, '>'))
		force_author = find_author_by_nickname(force_author);

	ଯଦି (force_author && renew_authorship)
		die(_("options '%s' and '%s' cannot be used together"), "--reset-author", "--author");

	ଯଦି (logfile || have_option_m || use_message)
		use_editor = 0;

	/* Sanity check options */
	ଯଦି (amend && !current_head)
		die(_("You have nothing to amend."));
	ଯଦି (amend && whence != FROM_COMMIT) {
		ଯଦି (whence == FROM_MERGE)
			die(_("You are in the middle of a merge -- cannot amend."));
		ଅନ୍ୟଥା ଯଦି (is_from_cherry_pick(whence))
			die(_("You are in the middle of a cherry-pick -- cannot amend."));
		ଅନ୍ୟଥା ଯଦି (whence == FROM_REBASE_PICK)
			die(_("You are in the middle of a rebase -- cannot amend."));
	}
	ଯଦି (fixup_message && squash_message)
		die(_("options '%s' and '%s' cannot be used together"), "--squash", "--fixup");
	die_for_incompatible_opt4(!!use_message, "-C",
				  !!edit_message, "-c",
				  !!logfile, "-F",
				  !!fixup_message, "--fixup");
	die_for_incompatible_opt4(have_option_m, "-m",
				  !!edit_message, "-c",
				  !!use_message, "-C",
				  !!logfile, "-F");
	ଯଦି (use_message || edit_message || logfile ||fixup_message || have_option_m)
		FREE_AND_NULL(template_file);
	ଯଦି (edit_message)
		use_message = edit_message;
	ଯଦି (amend && !use_message && !fixup_message)
		use_message = "HEAD";
	ଯଦି (!use_message && !is_from_cherry_pick(whence) &&
	    !is_from_rebase(whence) && renew_authorship)
		die(_("--reset-author can be used only with -C, -c or --amend."));
	ଯଦି (use_message) {
		use_message_buffer = read_commit_message(use_message);
		ଯଦି (!renew_authorship) {
			author_message = use_message;
			author_message_buffer = use_message_buffer;
		}
	}
	ଯଦି ((is_from_cherry_pick(whence) || whence == FROM_REBASE_PICK) &&
	    !renew_authorship) {
		author_message = "CHERRY_PICK_HEAD";
		author_message_buffer = read_commit_message(author_message);
	}

	ଯଦି (patch_interactive)
		interactive = 1;

	die_for_incompatible_opt4(also, "-i/--include",
				  only, "-o/--only",
				  all, "-a/--all",
				  interactive, "--interactive/-p/--patch");
	ଯଦି (fixup_message) {
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
		ଅକ୍ଷର *p = fixup_message;
		ଯେତେବେଳେ (isalpha(*p))
			p++;
		ଯଦି (p > fixup_message && *p == ':') {
			*p = '\0';
			fixup_commit = p + 1;
			ଯଦି (!strcmp("amend", fixup_message) ||
			    !strcmp("reword", fixup_message)) {
				fixup_prefix = "amend";
				allow_empty = 1;
				ଯଦି (*fixup_message == 'r') {
					check_fixup_reword_options(argc, argv);
					only = 1;
				}
			} ଅନ୍ୟଥା {
				die(_("unknown option: --fixup=%s:%s"), fixup_message, fixup_commit);
			}
		} ଅନ୍ୟଥା {
			fixup_commit = fixup_message;
			fixup_prefix = "fixup";
			use_editor = 0;
		}
	}

	ଯଦି (0 <= edit_flag)
		use_editor = edit_flag;

	handle_untracked_files_arg(s);

	ଯଦି (all && argc > 0)
		die(_("paths '%s ...' with -a does not make sense"),
		    argv[0]);

	ଯଦି (status_format != STATUS_FORMAT_NONE)
		dry_run = 1;

	ଫେରନ୍ତୁ argc;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା dry_run_commit(ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
			  ଅଚଳ ସଂରଚନା commit *current_head, ସଂରଚନା wt_status *s)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା committable;
	ଅଚଳ ଅକ୍ଷର *index_file;

	index_file = prepare_index(argv, prefix, current_head, 1);
	committable = run_status(stdout, index_file, prefix, 0, s);
	rollback_index_files();

	ଫେରନ୍ତୁ committable ? 0 : 1;
}

define_list_config_array_extra(color_status_slots, {"added"});

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା parse_status_slot(ଅଚଳ ଅକ୍ଷର *slot)
{
	ଯଦି (!strcasecmp(slot, "added"))
		ଫେରନ୍ତୁ WT_STATUS_UPDATED;

	ଫେରନ୍ତୁ LOOKUP_CONFIG(color_status_slots, slot);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା git_status_config(ଅଚଳ ଅକ୍ଷର *k, ଅଚଳ ଅକ୍ଷର *v,
			     ଅଚଳ ସଂରଚନା config_context *ctx, ଶୂନ୍ୟ *cb)
{
	ସଂରଚନା wt_status *s = cb;
	ଅଚଳ ଅକ୍ଷର *slot_name;

	ଯଦି (starts_with(k, "column."))
		ଫେରନ୍ତୁ git_column_config(k, v, "status", &s->colopts);
	ଯଦି (!strcmp(k, "status.submodulesummary")) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା is_bool;
		s->submodule_summary = git_config_bool_or_int(k, v, ctx->kvi,
							      &is_bool);
		ଯଦି (is_bool && s->submodule_summary)
			s->submodule_summary = -1;
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (!strcmp(k, "status.short")) {
		ଯଦି (git_config_bool(k, v))
			status_deferred_config.status_format = STATUS_FORMAT_SHORT;
		ଅନ୍ୟଥା
			status_deferred_config.status_format = STATUS_FORMAT_NONE;
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (!strcmp(k, "status.branch")) {
		status_deferred_config.show_branch = git_config_bool(k, v);
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (!strcmp(k, "status.aheadbehind")) {
		status_deferred_config.ahead_behind = git_config_bool(k, v);
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (!strcmp(k, "status.showstash")) {
		s->show_stash = git_config_bool(k, v);
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (!strcmp(k, "status.color") || !strcmp(k, "color.status")) {
		s->use_color = git_config_colorbool(k, v);
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (!strcmp(k, "status.displaycommentprefix")) {
		s->display_comment_prefix = git_config_bool(k, v);
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (skip_prefix(k, "status.color.", &slot_name) ||
	    skip_prefix(k, "color.status.", &slot_name)) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା slot = parse_status_slot(slot_name);
		ଯଦି (slot < 0)
			ଫେରନ୍ତୁ 0;
		ଯଦି (!v)
			ଫେରନ୍ତୁ config_error_nonbool(k);
		ଫେରନ୍ତୁ color_parse(v, s->color_palette[slot]);
	}
	ଯଦି (!strcmp(k, "status.relativepaths")) {
		s->relative_paths = git_config_bool(k, v);
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (!strcmp(k, "status.showuntrackedfiles")) {
		ଗଣନା untracked_status_type u;

		u = parse_untracked_setting_name(v);
		ଯଦି (u == SHOW_UNTRACKED_FILES_ERROR)
			ଫେରନ୍ତୁ error(_("Invalid untracked files mode '%s'"), v);
		s->show_untracked_files = u;
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (!strcmp(k, "diff.renamelimit")) {
		ଯଦି (s->rename_limit == -1)
			s->rename_limit = git_config_int(k, v, ctx->kvi);
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (!strcmp(k, "status.renamelimit")) {
		s->rename_limit = git_config_int(k, v, ctx->kvi);
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (!strcmp(k, "diff.renames")) {
		ଯଦି (s->detect_rename == -1)
			s->detect_rename = git_config_rename(k, v);
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (!strcmp(k, "status.renames")) {
		s->detect_rename = git_config_rename(k, v);
		ଫେରନ୍ତୁ 0;
	}
	ଫେରନ୍ତୁ git_diff_ui_config(k, v, ctx, ଖାଲି);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_status(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
ଅଚଳ ଅକ୍ଷର **argv,
ଅଚଳ ଅକ୍ଷର *prefix,
ସଂରଚନା repository *repo UNUSED)
{
	ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା no_renames = -1;
	ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *rename_score_arg = (ଅଚଳ ଅକ୍ଷର *)-1;
	ସ୍ଥିର ସଂରଚନା wt_status s;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା progress_flag = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd;
	ସଂରଚନା object_id oid;
	ସ୍ଥିର ସଂରଚନା option builtin_status_options[] = {
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT_SET_INT('s', "short", &status_format,
			    N_("show status concisely"), STATUS_FORMAT_SHORT),
		OPT_BOOL('b', "branch", &s.show_branch,
			 N_("show branch information")),
		OPT_BOOL(0, "show-stash", &s.show_stash,
			 N_("show stash information")),
		OPT_BOOL(0, "ahead-behind", &s.ahead_behind_flags,
			 N_("compute full ahead/behind values")),
		OPT_CALLBACK_F(0, "porcelain", &status_format,
		  N_("version"), N_("machine-readable output"),
		  PARSE_OPT_OPTARG, opt_parse_porcelain),
		OPT_SET_INT(0, "long", &status_format,
			    N_("show status in long format (default)"),
			    STATUS_FORMAT_LONG),
		OPT_BOOL('z', "null", &s.null_termination,
			 N_("terminate entries with NUL")),
		{
			.type = OPTION_STRING,
			.short_name = 'u',
			.long_name = "untracked-files",
			.ମୂଲ୍ୟ = &untracked_files_arg,
			.argh = N_("mode"),
			.help = N_("show untracked files, optional modes: all, normal, no. (Default: all)"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t)"all",
		},
		{
			.type = OPTION_STRING,
			.long_name = "ignored",
			.ମୂଲ୍ୟ = &ignored_arg,
			.argh = N_("mode"),
			.help = N_("show ignored files, optional modes: traditional, matching, no. (Default: traditional)"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t)"traditional",
		},
		{
			.type = OPTION_STRING,
			.long_name = "ignore-submodules",
			.ମୂଲ୍ୟ = &ignore_submodule_arg,
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
		OPT_END(),
	};

	show_usage_with_options_if_asked(argc, argv,
					 builtin_status_usage, builtin_status_options);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	status_init_config(&s, git_status_config);
	argc = parse_options(argc, argv, prefix,
			     builtin_status_options,
			     builtin_status_usage, 0);
	finalize_colopts(&s.colopts, -1);
	finalize_deferred_config(&s);

	handle_untracked_files_arg(&s);
	handle_ignored_arg(&s);

	ଯଦି (s.show_ignored_mode == SHOW_MATCHING_IGNORED &&
	    s.show_untracked_files == SHOW_NO_UNTRACKED_FILES)
		die(_("Unsupported combination of ignored and untracked-files arguments"));

	parse_pathspec(&s.pathspec, 0,
		       PATHSPEC_PREFER_FULL,
		       prefix, argv);

	ଯଦି (status_format != STATUS_FORMAT_PORCELAIN &&
	    status_format != STATUS_FORMAT_PORCELAIN_V2)
		progress_flag = REFRESH_PROGRESS;
	repo_read_index(the_repository);
	refresh_index(the_repository->index,
		      REFRESH_QUIET|REFRESH_UNMERGED|progress_flag,
		      &s.pathspec, ଖାଲି, ଖାଲି);

	ଯଦି (use_optional_locks())
		fd = repo_hold_locked_index(the_repository, &index_lock, 0);
	ଅନ୍ୟଥା
		fd = -1;

	s.is_initial = repo_get_oid(the_repository, s.reference, &oid) ? 1 : 0;
	ଯଦି (!s.is_initial)
		oidcpy(&s.oid_commit, &oid);

	s.ignore_submodule_arg = ignore_submodule_arg;
	s.status_format = status_format;
	s.verbose = verbose;
	ଯଦି (no_renames != -1)
		s.detect_rename = !no_renames;
	ଯଦି ((intptr_t)rename_score_arg != -1) {
		ଯଦି (s.detect_rename < DIFF_DETECT_RENAME)
			s.detect_rename = DIFF_DETECT_RENAME;
		ଯଦି (rename_score_arg)
			s.rename_score = parse_rename_score(&rename_score_arg);
	}

	wt_status_collect(&s);

	ଯଦି (0 <= fd)
		repo_update_index_if_able(the_repository, &index_lock);

	ଯଦି (s.relative_paths)
		s.prefix = prefix;

	wt_status_print(&s);
	wt_status_collect_free_buffers(&s);

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା git_commit_config(ଅଚଳ ଅକ୍ଷର *k, ଅଚଳ ଅକ୍ଷର *v,
			     ଅଚଳ ସଂରଚନା config_context *ctx, ଶୂନ୍ୟ *cb)
{
	ସଂରଚନା wt_status *s = cb;

	ଯଦି (!strcmp(k, "commit.template"))
		ଫେରନ୍ତୁ git_config_pathname(&template_file, k, v);
	ଯଦି (!strcmp(k, "commit.status")) {
		include_status = git_config_bool(k, v);
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (!strcmp(k, "commit.cleanup")) {
		FREE_AND_NULL(cleanup_config);
		ଫେରନ୍ତୁ git_config_string(&cleanup_config, k, v);
	}
	ଯଦି (!strcmp(k, "commit.gpgsign")) {
		sign_commit = git_config_bool(k, v) ? "" : ଖାଲି;
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (!strcmp(k, "commit.verbose")) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା is_bool;
		config_commit_verbose = git_config_bool_or_int(k, v, ctx->kvi,
							       &is_bool);
		ଫେରନ୍ତୁ 0;
	}

	ଫେରନ୍ତୁ git_status_config(k, v, ctx, s);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_commit(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
	       ଅଚଳ ଅକ୍ଷର **argv,
	       ଅଚଳ ଅକ୍ଷର *prefix,
	       ସଂରଚନା repository *repo UNUSED)
{
	ସ୍ଥିର ସଂରଚନା wt_status s;
	ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *cleanup_arg = ଖାଲି;
	ସ୍ଥିର ସଂରଚନା option builtin_commit_options[] = {
		OPT__QUIET(&quiet, N_("suppress summary after successful commit")),
		OPT__VERBOSE(&verbose, N_("show diff in commit message template")),

		OPT_GROUP(N_("Commit message options")),
		OPT_FILENAME('F', "file", &logfile, N_("read message from file")),
		OPT_STRING(0, "author", &force_author, N_("author"), N_("override author for commit")),
		OPT_STRING(0, "date", &force_date, N_("date"), N_("override date for commit")),
		OPT_CALLBACK('m', "message", &message, N_("message"), N_("commit message"), opt_parse_m),
		OPT_STRING('c', "reedit-message", &edit_message, N_("commit"), N_("reuse and edit message from specified commit")),
		OPT_STRING('C', "reuse-message", &use_message, N_("commit"), N_("reuse message from specified commit")),
		/*
		 * TRANSLATORS: Leave "[(amend|reword):]" as-is,
		 * and only translate <commit>.
		 */
		OPT_STRING(0, "fixup", &fixup_message, N_("[(amend|reword):]commit"), N_("use autosquash formatted message to fixup or amend/reword specified commit")),
		OPT_STRING(0, "squash", &squash_message, N_("commit"), N_("use autosquash formatted message to squash specified commit")),
		OPT_BOOL(0, "reset-author", &renew_authorship, N_("the commit is authored by me now (used with -C/-c/--amend)")),
		OPT_STRVEC(0, "trailer", &trailer_args, N_("trailer"),
			   N_("add custom trailer(s)")),
		OPT_BOOL('s', "signoff", &signoff, N_("add a Signed-off-by trailer")),
		OPT_FILENAME('t', "template", &template_file, N_("use specified template file")),
		OPT_BOOL('e', "edit", &edit_flag, N_("force edit of commit")),
		OPT_CLEANUP(&cleanup_arg),
		OPT_BOOL(0, "status", &include_status, N_("include status in commit message template")),
		{
			.type = OPTION_STRING,
			.short_name = 'S',
			.long_name = "gpg-sign",
			.ମୂଲ୍ୟ = &sign_commit,
			.argh = N_("key-id"),
			.help = N_("GPG sign commit"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "",
		},
		/* end commit message options */

		OPT_GROUP(N_("Commit contents options")),
		OPT_BOOL('a', "all", &all, N_("commit all changed files")),
		OPT_BOOL('i', "include", &also, N_("add specified files to index for commit")),
		OPT_BOOL(0, "interactive", &interactive, N_("interactively add files")),
		OPT_BOOL('p', "patch", &patch_interactive, N_("interactively add changes")),
		OPT_DIFF_UNIFIED(&interactive_opts.context),
		OPT_DIFF_INTERHUNK_CONTEXT(&interactive_opts.interhunkcontext),
		OPT_BOOL('o', "only", &only, N_("commit only specified files")),
		OPT_BOOL('n', "no-verify", &no_verify, N_("bypass pre-commit and commit-msg hooks")),
		OPT_BOOL(0, "dry-run", &dry_run, N_("show what would be committed")),
		OPT_SET_INT(0, "short", &status_format, N_("show status concisely"),
			    STATUS_FORMAT_SHORT),
		OPT_BOOL(0, "branch", &s.show_branch, N_("show branch information")),
		OPT_BOOL(0, "ahead-behind", &s.ahead_behind_flags,
			 N_("compute full ahead/behind values")),
		OPT_SET_INT(0, "porcelain", &status_format,
			    N_("machine-readable output"), STATUS_FORMAT_PORCELAIN),
		OPT_SET_INT(0, "long", &status_format,
			    N_("show status in long format (default)"),
			    STATUS_FORMAT_LONG),
		OPT_BOOL('z', "null", &s.null_termination,
			 N_("terminate entries with NUL")),
		OPT_BOOL(0, "amend", &amend, N_("amend previous commit")),
		OPT_BOOL(0, "no-post-rewrite", &no_post_rewrite, N_("bypass post-rewrite hook")),
		{
			.type = OPTION_STRING,
			.short_name = 'u',
			.long_name = "untracked-files",
			.ମୂଲ୍ୟ = &untracked_files_arg,
			.argh = N_("mode"),
			.help = N_("show untracked files, optional modes: all, normal, no. (Default: all)"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t)"all",
		},
		OPT_PATHSPEC_FROM_FILE(&pathspec_from_file),
		OPT_PATHSPEC_FILE_NUL(&pathspec_file_nul),
		/* end commit contents options */

		OPT_HIDDEN_BOOL(0, "allow-empty", &allow_empty,
				N_("ok to record an empty change")),
		OPT_HIDDEN_BOOL(0, "allow-empty-message", &allow_empty_message,
				N_("ok to record a change with an empty message")),

		OPT_END()
	};

	ସଂରଚନା strbuf sb = STRBUF_INIT;
	ସଂରଚନା strbuf author_ident = STRBUF_INIT;
	ଅଚଳ ଅକ୍ଷର *index_file, *reflog_ସନ୍ଦେଶ;
	ସଂରଚନା object_id oid;
	ସଂରଚନା commit_list *parents = ଖାଲି;
	ସଂରଚନା stat statbuf;
	ସଂରଚନା commit *current_head = ଖାଲି;
	ସଂରଚନା commit_extra_header *extra = ଖାଲି;
	ସଂରଚନା strbuf err = STRBUF_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;

	show_usage_with_options_if_asked(argc, argv,
					 builtin_commit_usage, builtin_commit_options);

#ifndef WITH_BREAKING_CHANGES
	warn_on_auto_comment_char = true;
#endif /* !WITH_BREAKING_CHANGES */
	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	status_init_config(&s, git_commit_config);
	s.commit_template = 1;
	status_format = STATUS_FORMAT_NONE; /* Ignore status.short */
	s.colopts = 0;

	ଯଦି (repo_get_oid(the_repository, "HEAD", &oid))
		current_head = ଖାଲି;
	ଅନ୍ୟଥା {
		current_head = lookup_commit_or_die(&oid, "HEAD");
		ଯଦି (repo_parse_commit(the_repository, current_head))
			die(_("could not parse HEAD commit"));
	}
	verbose = -1; /* unspecified */
	argc = parse_and_validate_options(argc, argv, builtin_commit_options,
					  builtin_commit_usage,
					  prefix, current_head, &s);
	ଯଦି (trailer_args.nr)
		trailer_config_init();

	ଯଦି (verbose == -1)
		verbose = (config_commit_verbose < 0) ? 0 : config_commit_verbose;

	ଯଦି (cleanup_arg) {
		ମୁକ୍ତ(cleanup_config);
		cleanup_config = xstrdup(cleanup_arg);
	}
	cleanup_mode = get_cleanup_mode(cleanup_config, use_editor);

	ଯଦି (dry_run)
		ଫେରନ୍ତୁ dry_run_commit(argv, prefix, current_head, &s);
	index_file = prepare_index(argv, prefix, current_head, 0);

	/* Set up everything for writing the commit object.  This includes
	   running hooks, writing the trees, and interacting with the user.  */
	ଯଦି (!prepare_to_commit(index_file, prefix,
			       current_head, &s, &author_ident)) {
		ret = 1;
		rollback_index_files();
		ଯାଅ cleanup;
	}

	/* Determine parents */
	reflog_ସନ୍ଦେଶ = getenv("GIT_REFLOG_ACTION");
	ଯଦି (!current_head) {
		ଯଦି (!reflog_ସନ୍ଦେଶ)
			reflog_ସନ୍ଦେଶ = "commit (initial)";
	} ଅନ୍ୟଥା ଯଦି (amend) {
		ଯଦି (!reflog_ସନ୍ଦେଶ)
			reflog_ସନ୍ଦେଶ = "commit (amend)";
		parents = commit_list_copy(current_head->parents);
	} ଅନ୍ୟଥା ଯଦି (whence == FROM_MERGE) {
		ସଂରଚନା strbuf m = STRBUF_INIT;
		FILE *fp;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା allow_fast_forward = 1;
		ସଂରଚନା commit_list **pptr = &parents;

		ଯଦି (!reflog_ସନ୍ଦେଶ)
			reflog_ସନ୍ଦେଶ = "commit (merge)";
		pptr = commit_list_append(current_head, pptr);
		fp = xfopen(git_path_merge_head(the_repository), "r");
		ଯେତେବେଳେ (strbuf_getline_lf(&m, fp) != EOF) {
			ସଂରଚନା commit *parent;

			parent = get_merge_parent(m.buf);
			ଯଦି (!parent)
				die(_("Corrupt MERGE_HEAD file (%s)"), m.buf);
			pptr = commit_list_append(parent, pptr);
		}
		fclose(fp);
		strbuf_release(&m);
		ଯଦି (!stat(git_path_merge_mode(the_repository), &statbuf)) {
			ଯଦି (strbuf_read_file(&sb, git_path_merge_mode(the_repository), 0) < 0)
				die_errno(_("could not read MERGE_MODE"));
			ଯଦି (!strcmp(sb.buf, "no-ff"))
				allow_fast_forward = 0;
		}
		ଯଦି (allow_fast_forward)
			reduce_heads_replace(&parents);
	} ଅନ୍ୟଥା {
		ଯଦି (!reflog_ସନ୍ଦେଶ)
			reflog_ସନ୍ଦେଶ = is_from_cherry_pick(whence)
					? "commit (cherry-pick)"
					: is_from_rebase(whence)
					? "commit (rebase)"
					: "commit";
		commit_list_ଭର୍ତ୍ତି(current_head, &parents);
	}

	/* Finally, get the commit message */
	strbuf_reset(&sb);
	ଯଦି (strbuf_read_file(&sb, git_path_commit_editmsg(), 0) < 0) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା saved_errno = errno;
		rollback_index_files();
		die(_("could not read commit message: %s"), strerror(saved_errno));
	}

	cleanup_message(&sb, cleanup_mode, verbose);

	ଯଦି (message_is_empty(&sb, cleanup_mode) && !allow_empty_message) {
		rollback_index_files();
		fprintf(stderr, _("Aborting commit due to empty commit message.\n"));
		exit(1);
	}
	ଯଦି (template_untouched(&sb, template_file, cleanup_mode) && !allow_empty_message) {
		rollback_index_files();
		fprintf(stderr, _("Aborting commit; you did not edit the message.\n"));
		exit(1);
	}

	ଯଦି (fixup_message && starts_with(sb.buf, "amend! ") &&
	    !allow_empty_message) {
		ସଂରଚନା strbuf body = STRBUF_INIT;
		size_t len = commit_subject_length(sb.buf);
		strbuf_addstr(&body, sb.buf + len);
		ଯଦି (message_is_empty(&body, cleanup_mode)) {
			rollback_index_files();
			fprintf(stderr, _("Aborting commit due to empty commit message body.\n"));
			exit(1);
		}
		strbuf_release(&body);
	}

	ଯଦି (amend) {
		ଅଚଳ ଅକ୍ଷର *exclude_gpgsig[3] = { "gpgsig", "gpgsig-sha256", ଖାଲି };
		extra = read_commit_extra_headers(current_head, exclude_gpgsig);
	} ଅନ୍ୟଥା {
		ସଂରଚନା commit_extra_header **tail = &extra;
		append_merge_tag_headers(parents, &tail);
	}

	ଯଦି (commit_ବୃକ୍ଷ_extended(sb.buf, sb.len, &the_repository->index->cache_ବୃକ୍ଷ->oid,
				 parents, &oid, author_ident.buf, ଖାଲି,
				 sign_commit, extra)) {
		rollback_index_files();
		die(_("failed to write commit object"));
	}

	ଯଦି (update_head_with_reflog(current_head, &oid, reflog_ସନ୍ଦେଶ, &sb,
				    &err)) {
		rollback_index_files();
		die("%s", err.buf);
	}

	sequencer_post_commit_cleanup(the_repository, 0);
	unlink(git_path_merge_head(the_repository));
	unlink(git_path_merge_ସନ୍ଦେଶ(the_repository));
	unlink(git_path_merge_mode(the_repository));
	unlink(git_path_squash_ସନ୍ଦେଶ(the_repository));

	ଯଦି (commit_index_files())
		die(_("repository has been updated, but unable to write\n"
		      "new index file. Check that disk is not full and quota is\n"
		      "not exceeded, and then \"git restore --staged :/\" to recover."));

	git_test_write_commit_graph_or_die(the_repository);

	repo_rerere(the_repository, 0);
	run_auto_maintenance(the_repository, quiet);
	run_commit_hook(use_editor, repo_get_index_file(the_repository),
			ଖାଲି, "post-commit", ଖାଲି);
	ଯଦି (amend && !no_post_rewrite) {
		commit_post_rewrite(the_repository, current_head, &oid);
	}
	ଯଦି (!quiet) {
		unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags = 0;

		ଯଦି (!current_head)
			flags |= SUMMARY_INITIAL_COMMIT;
		ଯଦି (author_date_is_interesting())
			flags |= SUMMARY_SHOW_AUTHOR_DATE;
		print_commit_summary(the_repository, prefix,
				     &oid, flags);
	}

	apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
			    ଖାଲି, ଖାଲି, ଖାଲି, ଖାଲି);

cleanup:
	free_commit_extra_headers(extra);
	commit_list_free(parents);
	strbuf_release(&author_ident);
	strbuf_release(&err);
	strbuf_release(&sb);
	ମୁକ୍ତ(logfile);
	ମୁକ୍ତ(template_file);
	ଫେରନ୍ତୁ ret;
}
