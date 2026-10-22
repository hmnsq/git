/*
 * Builtin "git commit"
 *
 * Copyright (c) 2007 Kristian Høgsberg <krh@redhat.com>
 * Based on git-commit.sh by Junio C Hamano and Linus Torvalds
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "advice.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "lockfile.h"
#અન્તર્ભાવ "cache-tree.h"
#અન્તર્ભાવ "color.h"
#અન્તર્ભાવ "dir.h"
#અન્તર્ભાવ "editor.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "diff.h"
#અન્તર્ભાવ "commit.h"
#અન્તર્ભાવ "add-interactive.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "revision.h"
#અન્તર્ભાવ "wt-status.h"
#અન્તર્ભાવ "run-command.h"
#અન્તર્ભાવ "strbuf.h"
#અન્તર્ભાવ "object-name.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "path.h"
#અન્તર્ભાવ "preload-index.h"
#અન્તર્ભાવ "read-cache.h"
#અન્તર્ભાવ "repository.h"
#અન્તર્ભાવ "string-list.h"
#અન્તર્ભાવ "rerere.h"
#અન્તર્ભાવ "unpack-trees.h"
#અન્તર્ભાવ "column.h"
#અન્તર્ભાવ "sequencer.h"
#અન્તર્ભાવ "sparse-index.h"
#અન્તર્ભાવ "mailmap.h"
#અન્તર્ભાવ "help.h"
#અન્તર્ભાવ "commit-reach.h"
#અન્તર્ભાવ "commit-graph.h"
#અન્તર્ભાવ "pretty.h"
#અન્તર્ભાવ "trailer.h"

સ્થિર અચલ અક્ષર * અચલ builtin_commit_usage[] = {
	N_("git commit [-a | --interactive | --patch] [-s] [-v] [-u[<mode>]] [--amend]\n"
	   "           [--dry-run] [(-c | -C | --squash) <commit> | --fixup [(amend|reword):]<commit>]\n"
	   "           [-F <file> | -m <msg>] [--reset-author] [--allow-empty]\n"
	   "           [--allow-empty-message] [--no-verify] [-e] [--author=<author>]\n"
	   "           [--date=<date>] [--cleanup=<mode>] [--[no-]status]\n"
	   "           [-i | -o] [--pathspec-from-file=<file> [--pathspec-file-nul]]\n"
	   "           [(--trailer <token>[(=|:)<value>])...] [-S[<keyid>]]\n"
	   "           [--] [<pathspec>...]"),
	ખાલી
};

સ્થિર અચલ અક્ષર * અચલ builtin_status_usage[] = {
	N_("git status [<options>] [--] [<pathspec>...]"),
	ખાલી
};

સ્થિર અચલ અક્ષર empty_amend_advice[] =
N_("You asked to amend the most recent commit, but doing so would make\n"
"it empty. You can repeat your command with --allow-empty, or you can\n"
"remove the commit entirely with \"git reset HEAD^\".\n");

સ્થિર અચલ અક્ષર empty_cherry_pick_advice[] =
N_("The previous cherry-pick is now empty, possibly due to conflict resolution.\n"
"If you wish to commit it anyway, use:\n"
"\n"
"    git commit --allow-empty\n"
"\n");

સ્થિર અચલ અક્ષર empty_rebase_pick_advice[] =
N_("Otherwise, please use 'git rebase --skip'\n");

સ્થિર અચલ અક્ષર empty_cherry_pick_advice_single[] =
N_("Otherwise, please use 'git cherry-pick --skip'\n");

સ્થિર અચલ અક્ષર empty_cherry_pick_advice_multi[] =
N_("and then use:\n"
"\n"
"    git cherry-pick --continue\n"
"\n"
"to resume cherry-picking the remaining commits.\n"
"If you wish to skip this commit, use:\n"
"\n"
"    git cherry-pick --skip\n"
"\n");

સ્થિર અચલ અક્ષર *color_status_slots[] = {
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

સ્થિર અચલ અક્ષર *use_message_buffer;
સ્થિર સંરચના lock_file index_lock; /* real index */
સ્થિર સંરચના lock_file false_lock; /* used only for partial commits */
સ્થિર ગણના {
	COMMIT_AS_IS = 1,
	COMMIT_NORMAL,
	COMMIT_PARTIAL
} commit_style;

સ્થિર અચલ અક્ષર *force_author;
સ્થિર અક્ષર *logfile;
સ્થિર અક્ષર *template_file;
/*
 * The _message variables are commit names from which to take
 * the commit message and/or authorship.
 */
સ્થિર અચલ અક્ષર *author_message, *author_message_buffer;
સ્થિર અચલ અક્ષર *edit_message, *use_message;
સ્થિર અક્ષર *fixup_message, *fixup_commit, *squash_message;
સ્થિર અચલ અક્ષર *fixup_prefix;
સ્થિર પૂર્ણાંક all, also, interactive, patch_interactive, only, amend, signoff;
સ્થિર સંરચના interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
સ્થિર પૂર્ણાંક edit_flag = -1; /* unspecified */
સ્થિર પૂર્ણાંક quiet, verbose, no_verify, allow_empty, dry_run, renew_authorship;
સ્થિર પૂર્ણાંક config_commit_verbose = -1; /* unspecified */
સ્થિર પૂર્ણાંક no_post_rewrite, allow_empty_message, pathspec_file_nul;
સ્થિર અચલ અક્ષર *untracked_files_arg, *force_date, *ignore_submodule_arg, *ignored_arg;
સ્થિર અચલ અક્ષર *sign_commit, *pathspec_from_file;
સ્થિર સંરચના strvec trailer_args = STRVEC_INIT;

/*
 * The default commit message cleanup mode will remove the lines
 * beginning with # (shell comments) and leading and trailing
 * whitespaces (empty lines or containing only whitespaces)
 * if editor is used, and only the whitespaces if the message
 * is specified explicitly.
 */
સ્થિર ગણના commit_સંદેશ_cleanup_mode cleanup_mode;
સ્થિર અક્ષર *cleanup_config;

સ્થિર ગણના commit_whence whence;
સ્થિર પૂર્ણાંક use_editor = 1, include_status = 1;
સ્થિર પૂર્ણાંક have_option_m;
સ્થિર સંરચના strbuf message = STRBUF_INIT;

સ્થિર ગણના wt_status_format status_format = STATUS_FORMAT_UNSPECIFIED;

સ્થિર પૂર્ણાંક opt_parse_porcelain(અચલ સંરચના option *opt, અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	ગણના wt_status_format *કિંમત = (ગણના wt_status_format *)opt->કિંમત;
	જો (unset)
		*કિંમત = STATUS_FORMAT_NONE;
	નહીંતર જો (!arg)
		*કિંમત = STATUS_FORMAT_PORCELAIN;
	નહીંતર જો (!strcmp(arg, "v1") || !strcmp(arg, "1"))
		*કિંમત = STATUS_FORMAT_PORCELAIN;
	નહીંતર જો (!strcmp(arg, "v2") || !strcmp(arg, "2"))
		*કિંમત = STATUS_FORMAT_PORCELAIN_V2;
	નહીંતર
		die("unsupported porcelain version '%s'", arg);

	પાછા 0;
}

સ્થિર પૂર્ણાંક opt_parse_m(અચલ સંરચના option *opt, અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	સંરચના strbuf *buf = opt->કિંમત;
	જો (unset) {
		have_option_m = 0;
		strbuf_setlen(buf, 0);
	} નહીંતર {
		have_option_m = 1;
		જો (buf->len)
			strbuf_addch(buf, '\n');
		strbuf_addstr(buf, arg);
		strbuf_complete_લીટી(buf);
	}
	પાછા 0;
}

સ્થિર પૂર્ણાંક opt_parse_rename_score(અચલ સંરચના option *opt, અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	અચલ અક્ષર **કિંમત = opt->કિંમત;

	BUG_ON_OPT_NEG(unset);

	જો (arg != ખાલી && *arg == '=')
		arg = arg + 1;

	*કિંમત = arg;
	પાછા 0;
}

સ્થિર શૂન્ય determine_whence(સંરચના wt_status *s)
{
	જો (file_exists(git_path_merge_head(the_repository)))
		whence = FROM_MERGE;
	નહીંતર જો (!sequencer_determine_whence(the_repository, &whence))
		whence = FROM_COMMIT;
	જો (s)
		s->whence = whence;
}

સ્થિર શૂન્ય status_init_config(સંરચના wt_status *s, config_fn_t fn)
{
	wt_status_prepare(the_repository, s);
	init_diff_ui_defaults();
	repo_config(the_repository, fn, s);
	determine_whence(s);
	s->hints = advice_enabled(ADVICE_STATUS_HINTS); /* must come after repo_config() */
}

સ્થિર શૂન્ય rollback_index_files(શૂન્ય)
{
	પસંદગી (commit_style) {
	સ્થિતિ COMMIT_AS_IS:
		તોડો; /* nothing to do */
	સ્થિતિ COMMIT_NORMAL:
		rollback_lock_file(&index_lock);
		તોડો;
	સ્થિતિ COMMIT_PARTIAL:
		rollback_lock_file(&index_lock);
		rollback_lock_file(&false_lock);
		તોડો;
	}
}

સ્થિર પૂર્ણાંક commit_index_files(શૂન્ય)
{
	પૂર્ણાંક err = 0;

	પસંદગી (commit_style) {
	સ્થિતિ COMMIT_AS_IS:
		તોડો; /* nothing to do */
	સ્થિતિ COMMIT_NORMAL:
		err = commit_lock_file(&index_lock);
		તોડો;
	સ્થિતિ COMMIT_PARTIAL:
		err = commit_lock_file(&index_lock);
		rollback_lock_file(&false_lock);
		તોડો;
	}

	પાછા err;
}

/*
 * Take a union of paths in the index and the named tree (typically, "HEAD"),
 * and return the paths that match the given pattern in list.
 */
સ્થિર પૂર્ણાંક list_paths(સંરચના string_list *list, અચલ અક્ષર *with_વૃક્ષ,
		      અચલ સંરચના pathspec *pattern)
{
	પૂર્ણાંક i, ret;
	અક્ષર *m;

	જો (!pattern->nr)
		પાછા 0;

	m = xcalloc(1, pattern->nr);

	જો (with_વૃક્ષ) {
		અક્ષર *max_prefix = common_prefix(pattern);
		overlay_વૃક્ષ_on_index(the_repository->index, with_વૃક્ષ, max_prefix);
		મુક્ત(max_prefix);
	}

	/* TODO: audit for interaction with sparse-index. */
	ensure_full_index(the_repository->index);
	માટે (i = 0; i < the_repository->index->cache_nr; i++) {
		અચલ સંરચના cache_entry *ce = the_repository->index->cache[i];
		સંરચના string_list_item *item;

		જો (ce->ce_flags & CE_UPDATE)
			ચાલુ;
		જો (!ce_path_match(the_repository->index, ce, pattern, m))
			ચાલુ;
		item = string_list_ઉમેરો(list, ce->name);
		જો (ce_skip_worktree(ce))
			item->util = item; /* better a valid pointer than a fake one */
	}

	ret = report_path_error(m, pattern);
	મુક્ત(m);
	પાછા ret;
}

સ્થિર શૂન્ય add_remove_files(સંરચના string_list *list)
{
	પૂર્ણાંક i;
	માટે (i = 0; i < list->nr; i++) {
		સંરચના stat st;
		સંરચના string_list_item *p = &(list->વસ્તુઓ[i]);

		/* p->util is skip-worktree */
		જો (p->util)
			ચાલુ;

		જો (!lstat(p->string, &st)) {
			જો (add_to_index(the_repository->index, p->string, &st, 0))
				die(_("updating files failed"));
		} નહીંતર
			remove_file_from_index(the_repository->index, p->string);
	}
}

સ્થિર શૂન્ય create_base_index(અચલ સંરચના commit *current_head)
{
	સંરચના વૃક્ષ *વૃક્ષ;
	સંરચના unpack_trees_options opts;
	સંરચના વૃક્ષ_desc t;

	જો (!current_head) {
		discard_index(the_repository->index);
		પાછા;
	}

	memset(&opts, 0, કદ(opts));
	opts.head_idx = 1;
	opts.index_only = 1;
	opts.merge = 1;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;

	opts.fn = oneway_merge;
	વૃક્ષ = repo_parse_વૃક્ષ_indirect(the_repository,
					&current_head->object.oid);
	જો (!વૃક્ષ)
		die(_("failed to unpack HEAD tree object"));
	જો (repo_parse_વૃક્ષ(the_repository, વૃક્ષ) < 0)
		exit(128);
	init_વૃક્ષ_desc(&t, &વૃક્ષ->object.oid, વૃક્ષ->buffer, વૃક્ષ->size);
	જો (unpack_trees(1, &t, &opts))
		exit(128); /* We've already reported the error, finish dying */
}

સ્થિર શૂન્ય refresh_cache_or_die(પૂર્ણાંક refresh_flags)
{
	/*
	 * refresh_flags contains REFRESH_QUIET, so the only errors
	 * are for unmerged entries.
	 */
	જો (refresh_index(the_repository->index, refresh_flags | REFRESH_IN_PORCELAIN, ખાલી, ખાલી, ખાલી))
		die_resolve_conflict("commit");
}

સ્થિર અચલ અક્ષર *prepare_index(અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
				 અચલ સંરચના commit *current_head, પૂર્ણાંક is_status)
{
	સંરચના string_list partial = STRING_LIST_INIT_DUP;
	સંરચના pathspec pathspec;
	પૂર્ણાંક refresh_flags = REFRESH_QUIET;
	અચલ અક્ષર *ret;
	અક્ષર *path = ખાલી;

	જો (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	જો (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	જો (is_status)
		refresh_flags |= REFRESH_UNMERGED;
	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_FULL,
		       prefix, argv);

	જો (pathspec_from_file) {
		જો (interactive)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--interactive/--patch");

		જો (all)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "-a");

		જો (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_FULL,
				    prefix, pathspec_from_file, pathspec_file_nul);
	} નહીંતર જો (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	જો (!pathspec.nr && (also || (only && !allow_empty &&
	    (!amend || (fixup_message && strcmp(fixup_prefix, "amend"))))))
		die(_("No paths with --include/--only does not make sense."));

	જો (repo_read_index_preload(the_repository, &pathspec, 0) < 0)
		die(_("index file corrupt"));

	જો (interactive) {
		અક્ષર *old_index_env = ખાલી, *old_repo_index_file;
		repo_hold_locked_index(the_repository, &index_lock,
				       LOCK_DIE_ON_ERROR);

		refresh_cache_or_die(refresh_flags);

		જો (write_locked_index(the_repository->index, &index_lock, 0))
			die(_("unable to create temporary index"));

		old_repo_index_file = the_repository->index_file;
		the_repository->index_file =
			(અક્ષર *)get_lock_file_path(&index_lock);
		old_index_env = xstrdup_or_null(getenv(INDEX_ENVIRONMENT));
		setenv(INDEX_ENVIRONMENT, the_repository->index_file, 1);

		જો (interactive_add(the_repository, argv, prefix, patch_interactive, &interactive_opts) != 0)
			die(_("interactive add failed"));

		the_repository->index_file = old_repo_index_file;
		જો (old_index_env && *old_index_env)
			setenv(INDEX_ENVIRONMENT, old_index_env, 1);
		નહીંતર
			unsetenv(INDEX_ENVIRONMENT);
		FREE_AND_NULL(old_index_env);

		discard_index(the_repository->index);
		read_index_from(the_repository->index, get_lock_file_path(&index_lock),
				repo_get_git_dir(the_repository));
		જો (cache_વૃક્ષ_update(the_repository->index, WRITE_વૃક્ષ_SILENT) == 0) {
			જો (reopen_lock_file(&index_lock) < 0)
				die(_("unable to write index file"));
			જો (write_locked_index(the_repository->index, &index_lock, 0))
				die(_("unable to update temporary index"));
		} નહીંતર
			warning(_("Failed to update main cache tree"));

		commit_style = COMMIT_NORMAL;
		ret = get_lock_file_path(&index_lock);
		જાઓ out;
	} નહીંતર {
		જો (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--interactive/--patch");
		જો (interactive_opts.interhunkcontext != -1)
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
	જો (all || (also && pathspec.nr)) {
		અક્ષર *ps_matched = xcalloc(pathspec.nr, 1);
		repo_hold_locked_index(the_repository, &index_lock,
				       LOCK_DIE_ON_ERROR);
		add_files_to_cache(the_repository, also ? prefix : ખાલી,
				   &pathspec, ps_matched, 0, 0, 0 );
		જો (!all && report_path_error(ps_matched, &pathspec))
			exit(128);

		refresh_cache_or_die(refresh_flags);
		cache_વૃક્ષ_update(the_repository->index, WRITE_વૃક્ષ_SILENT);
		જો (write_locked_index(the_repository->index, &index_lock, 0))
			die(_("unable to write new index file"));
		commit_style = COMMIT_NORMAL;
		ret = get_lock_file_path(&index_lock);
		મુક્ત(ps_matched);
		જાઓ out;
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
	જો (!only && !pathspec.nr) {
		repo_hold_locked_index(the_repository, &index_lock,
				       LOCK_DIE_ON_ERROR);
		refresh_cache_or_die(refresh_flags);
		જો (the_repository->index->cache_changed
		    || !cache_વૃક્ષ_fully_valid(the_repository->index))
			cache_વૃક્ષ_update(the_repository->index, WRITE_વૃક્ષ_SILENT);
		જો (write_locked_index(the_repository->index, &index_lock,
				       COMMIT_LOCK | SKIP_IF_UNCHANGED))
			die(_("unable to write new index file"));
		commit_style = COMMIT_AS_IS;
		ret = repo_get_index_file(the_repository);
		જાઓ out;
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

	જો (whence != FROM_COMMIT) {
		જો (whence == FROM_MERGE)
			die(_("cannot do a partial commit during a merge."));
		નહીંતર જો (is_from_cherry_pick(whence))
			die(_("cannot do a partial commit during a cherry-pick."));
		નહીંતર જો (is_from_rebase(whence))
			die(_("cannot do a partial commit during a rebase."));
	}

	જો (list_paths(&partial, !current_head ? ખાલી : "HEAD", &pathspec))
		exit(1);

	discard_index(the_repository->index);
	જો (repo_read_index(the_repository) < 0)
		die(_("cannot read the index"));

	repo_hold_locked_index(the_repository, &index_lock, LOCK_DIE_ON_ERROR);
	add_remove_files(&partial);
	refresh_index(the_repository->index, REFRESH_QUIET, ખાલી, ખાલી, ખાલી);
	cache_વૃક્ષ_update(the_repository->index, WRITE_વૃક્ષ_SILENT);
	જો (write_locked_index(the_repository->index, &index_lock, 0))
		die(_("unable to write new index file"));

	path = repo_git_path(the_repository, "next-index-%"PRIuMAX,
			     (uintmax_t) getpid());
	hold_lock_file_for_update(&false_lock, path,
				  LOCK_DIE_ON_ERROR);

	create_base_index(current_head);
	add_remove_files(&partial);
	refresh_index(the_repository->index, REFRESH_QUIET, ખાલી, ખાલી, ખાલી);

	જો (write_locked_index(the_repository->index, &false_lock, 0))
		die(_("unable to write temporary index file"));

	discard_index(the_repository->index);
	ret = get_lock_file_path(&false_lock);
	read_index_from(the_repository->index, ret, repo_get_git_dir(the_repository));
out:
	string_list_clear(&partial, 0);
	clear_pathspec(&pathspec);
	મુક્ત(path);
	પાછા ret;
}

સ્થિર પૂર્ણાંક run_status(FILE *fp, અચલ અક્ષર *index_file, અચલ અક્ષર *prefix, પૂર્ણાંક nowarn,
		      સંરચના wt_status *s)
{
	સંરચના object_id oid;

	જો (s->relative_paths)
		s->prefix = prefix;

	જો (amend) {
		s->amend = 1;
		s->reference = "HEAD^1";
	}
	s->verbose = verbose;
	s->index_file = index_file;
	s->fp = fp;
	s->nowarn = nowarn;
	s->is_initial = repo_get_oid(the_repository, s->reference, &oid) ? 1 : 0;
	જો (!s->is_initial)
		oidcpy(&s->oid_commit, &oid);
	s->status_format = status_format;
	s->ignore_submodule_arg = ignore_submodule_arg;

	wt_status_collect(s);
	wt_status_print(s);
	wt_status_collect_free_buffers(s);

	પાછા s->committable;
}

સ્થિર પૂર્ણાંક is_a_merge(અચલ સંરચના commit *current_head)
{
	પાછા !!(current_head->parents && current_head->parents->next);
}

સ્થિર શૂન્ય assert_split_ident(સંરચના ident_split *id, અચલ સંરચના strbuf *buf)
{
	જો (split_ident_લીટી(id, buf->buf, buf->len) || !id->date_begin)
		BUG("unable to parse our own ident: %s", buf->buf);
}

સ્થિર શૂન્ય export_one(અચલ અક્ષર *var, અચલ અક્ષર *s, અચલ અક્ષર *e, પૂર્ણાંક hack)
{
	સંરચના strbuf buf = STRBUF_INIT;
	જો (hack)
		strbuf_addch(&buf, hack);
	strbuf_add(&buf, s, e - s);
	setenv(var, buf.buf, 1);
	strbuf_release(&buf);
}

સ્થિર પૂર્ણાંક parse_force_date(અચલ અક્ષર *in, સંરચના strbuf *out)
{
	strbuf_addch(out, '@');

	જો (parse_date(in, out) < 0) {
		પૂર્ણાંક errors = 0;
		unsigned long t = approxidate_careful(in, &errors);
		જો (errors)
			પાછા -1;
		strbuf_addf(out, "%lu", t);
	}

	પાછા 0;
}

સ્થિર શૂન્ય set_ident_var(અક્ષર **buf, અક્ષર *val)
{
	મુક્ત(*buf);
	*buf = val;
}

સ્થિર શૂન્ય determine_author_info(સંરચના strbuf *author_ident)
{
	અક્ષર *name, *email, *date;
	સંરચના ident_split author;

	name = xstrdup_or_null(getenv("GIT_AUTHOR_NAME"));
	email = xstrdup_or_null(getenv("GIT_AUTHOR_EMAIL"));
	date = xstrdup_or_null(getenv("GIT_AUTHOR_DATE"));

	જો (author_message) {
		સંરચના ident_split ident;
		size_t len;
		અચલ અક્ષર *a;

		a = find_commit_header(author_message_buffer, "author", &len);
		જો (!a)
			die(_("commit '%s' lacks author header"), author_message);
		જો (split_ident_લીટી(&ident, a, len) < 0)
			die(_("commit '%s' has malformed author line"), author_message);

		set_ident_var(&name, xmemdupz(ident.name_begin, ident.name_end - ident.name_begin));
		set_ident_var(&email, xmemdupz(ident.mail_begin, ident.mail_end - ident.mail_begin));

		જો (ident.date_begin) {
			સંરચના strbuf date_buf = STRBUF_INIT;
			strbuf_addch(&date_buf, '@');
			strbuf_add(&date_buf, ident.date_begin, ident.date_end - ident.date_begin);
			strbuf_addch(&date_buf, ' ');
			strbuf_add(&date_buf, ident.tz_begin, ident.tz_end - ident.tz_begin);
			set_ident_var(&date, strbuf_detach(&date_buf, ખાલી));
		}
	}

	જો (force_author) {
		સંરચના ident_split ident;

		જો (split_ident_લીટી(&ident, force_author, strlen(force_author)) < 0)
			die(_("malformed --author parameter"));
		set_ident_var(&name, xmemdupz(ident.name_begin, ident.name_end - ident.name_begin));
		set_ident_var(&email, xmemdupz(ident.mail_begin, ident.mail_end - ident.mail_begin));
	}

	જો (force_date) {
		સંરચના strbuf date_buf = STRBUF_INIT;
		જો (parse_force_date(force_date, &date_buf))
			die(_("invalid date format: %s"), force_date);
		set_ident_var(&date, strbuf_detach(&date_buf, ખાલી));
	}

	strbuf_addstr(author_ident, fmt_ident(name, email, WANT_AUTHOR_IDENT, date,
				IDENT_STRICT));
	assert_split_ident(&author, author_ident);
	export_one("GIT_AUTHOR_NAME", author.name_begin, author.name_end, 0);
	export_one("GIT_AUTHOR_EMAIL", author.mail_begin, author.mail_end, 0);
	export_one("GIT_AUTHOR_DATE", author.date_begin, author.tz_end, '@');
	મુક્ત(name);
	મુક્ત(email);
	મુક્ત(date);
}

સ્થિર પૂર્ણાંક author_date_is_interesting(શૂન્ય)
{
	પાછા author_message || force_date;
}

#ifndef WITH_BREAKING_CHANGES
સ્થિર શૂન્ય adjust_comment_લીટી_char(અચલ સંરચના strbuf *sb)
{
	અક્ષર candidates[] = "#;@!$%^&|:";
	અક્ષર *candidate;
	અચલ અક્ષર *p;
	size_t cutoff;

	/* Ignore comment chars in trailing comments (e.g., Conflicts:) */
	cutoff = sb->len - ignored_log_message_bytes(sb->buf, sb->len);

	જો (!memchr(sb->buf, candidates[0], sb->len)) {
		મુક્ત(comment_લીટી_str_to_free);
		comment_લીટી_str = comment_લીટી_str_to_free =
			xstrfmt("%c", candidates[0]);
		પાછા;
	}

	p = sb->buf;
	candidate = strchr(candidates, *p);
	જો (candidate)
		*candidate = ' ';
	માટે (p = sb->buf; p + 1 < sb->buf + cutoff; p++) {
		જો ((p[0] == '\n' || p[0] == '\r') && p[1]) {
			candidate = strchr(candidates, p[1]);
			જો (candidate)
				*candidate = ' ';
		}
	}

	માટે (p = candidates; *p == ' '; p++)
		;
	જો (!*p)
		die(_("unable to select a comment character that is not used\n"
		      "in the current commit message"));
	મુક્ત(comment_લીટી_str_to_free);
	comment_લીટી_str = comment_લીટી_str_to_free = xstrfmt("%c", *p);
}
#endif /* !WITH_BREAKING_CHANGES */

સ્થિર શૂન્ય prepare_amend_commit(સંરચના commit *commit, સંરચના strbuf *sb,
				સંરચના pretty_print_context *ctx)
{
	અચલ અક્ષર *buffer, *subject, *fmt;

	buffer = repo_get_commit_buffer(the_repository, commit, ખાલી);
	find_commit_subject(buffer, &subject);
	/*
	 * If we amend the 'amend!' commit then we don't want to
	 * duplicate the subject line.
	 */
	fmt = starts_with(subject, "amend!") ? "%b" : "%B";
	repo_format_commit_message(the_repository, commit, fmt, sb, ctx);
	repo_unuse_commit_buffer(the_repository, commit, buffer);
}

સ્થિર શૂન્ય change_data_free(શૂન્ય *util, અચલ અક્ષર *str UNUSED)
{
	સંરચના wt_status_change_data *d = util;
	મુક્ત(d->rename_source);
	મુક્ત(d);
}

સ્થિર પૂર્ણાંક prepare_to_commit(અચલ અક્ષર *index_file, અચલ અક્ષર *prefix,
			     સંરચના commit *current_head,
			     સંરચના wt_status *s,
			     સંરચના strbuf *author_ident)
{
	સંરચના stat statbuf;
	સંરચના strbuf committer_ident = STRBUF_INIT;
	પૂર્ણાંક committable;
	સંરચના strbuf sb = STRBUF_INIT;
	અચલ અક્ષર *hook_arg1 = ખાલી;
	અચલ અક્ષર *hook_arg2 = ખાલી;
	પૂર્ણાંક clean_message_contents = (cleanup_mode != COMMIT_સંદેશ_CLEANUP_NONE);
	પૂર્ણાંક old_display_comment_prefix;
	પૂર્ણાંક invoked_hook;

	/* This checks and barfs if author is badly specified */
	determine_author_info(author_ident);

	જો (!no_verify && run_commit_hook(use_editor, index_file, &invoked_hook,
					  "pre-commit", ખાલી))
		પાછા 0;

	જો (squash_message) {
		/*
		 * Insert the proper subject line before other commit
		 * message options add their content.
		 */
		જો (use_message && !strcmp(use_message, squash_message))
			strbuf_addstr(&sb, "squash! ");
		નહીંતર {
			સંરચના pretty_print_context ctx = {0};
			સંરચના commit *c;
			c = lookup_commit_reference_by_name(squash_message);
			જો (!c)
				die(_("could not lookup commit '%s'"), squash_message);
			ctx.output_encoding = get_commit_output_encoding();
			repo_format_commit_message(the_repository, c,
						   "squash! %s\n\n", &sb,
						   &ctx);
		}
	}

	જો (have_option_m && !fixup_message) {
		strbuf_addbuf(&sb, &message);
		hook_arg1 = "message";
	} નહીંતર જો (logfile && !strcmp(logfile, "-")) {
		જો (isatty(0))
			fprintf(stderr, _("(reading log message from standard input)\n"));
		જો (strbuf_read(&sb, 0, 0) < 0)
			die_errno(_("could not read log from standard input"));
		hook_arg1 = "message";
	} નહીંતર જો (logfile) {
		જો (strbuf_read_file(&sb, logfile, 0) < 0)
			die_errno(_("could not read log file '%s'"),
				  logfile);
		hook_arg1 = "message";
	} નહીંતર જો (use_message) {
		અચલ અક્ષર *buffer;
		buffer = strstr(use_message_buffer, "\n\n");
		જો (buffer)
			strbuf_addstr(&sb, skip_blank_લીટીઓ(buffer + 2));
		hook_arg1 = "commit";
		hook_arg2 = use_message;
	} નહીંતર જો (fixup_message) {
		સંરચના pretty_print_context ctx = {0};
		સંરચના commit *commit;
		અક્ષર *fmt;
		commit = lookup_commit_reference_by_name(fixup_commit);
		જો (!commit)
			die(_("could not lookup commit '%s'"), fixup_commit);
		ctx.output_encoding = get_commit_output_encoding();
		fmt = xstrfmt("%s! %%s\n\n", fixup_prefix);
		repo_format_commit_message(the_repository, commit, fmt, &sb,
					   &ctx);
		મુક્ત(fmt);
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
		જો (have_option_m && !strcmp(fixup_prefix, "fixup"))
			strbuf_addbuf(&sb, &message);

		જો (!strcmp(fixup_prefix, "amend")) {
			જો (have_option_m)
				die(_("options '%s' and '%s:%s' cannot be used together"), "-m", "--fixup", fixup_message);
			prepare_amend_commit(commit, &sb, &ctx);
		}
	} નહીંતર જો (!stat(git_path_merge_સંદેશ(the_repository), &statbuf)) {
		size_t merge_સંદેશ_start;

		/*
		 * prepend SQUASH_MSG here if it exists and a
		 * "merge --squash" was originally performed
		 */
		જો (!stat(git_path_squash_સંદેશ(the_repository), &statbuf)) {
			જો (strbuf_read_file(&sb, git_path_squash_સંદેશ(the_repository), 0) < 0)
				die_errno(_("could not read SQUASH_MSG"));
			hook_arg1 = "squash";
		} નહીંતર
			hook_arg1 = "merge";

		merge_સંદેશ_start = sb.len;
		જો (strbuf_read_file(&sb, git_path_merge_સંદેશ(the_repository), 0) < 0)
			die_errno(_("could not read MERGE_MSG"));

		જો (cleanup_mode == COMMIT_સંદેશ_CLEANUP_SCISSORS &&
		    wt_status_locate_end(sb.buf + merge_સંદેશ_start,
					 sb.len - merge_સંદેશ_start) <
				sb.len - merge_સંદેશ_start)
			s->added_cut_લીટી = 1;
	} નહીંતર જો (!stat(git_path_squash_સંદેશ(the_repository), &statbuf)) {
		જો (strbuf_read_file(&sb, git_path_squash_સંદેશ(the_repository), 0) < 0)
			die_errno(_("could not read SQUASH_MSG"));
		hook_arg1 = "squash";
	} નહીંતર જો (template_file) {
		જો (strbuf_read_file(&sb, template_file, 0) < 0)
			die_errno(_("could not read '%s'"), template_file);
		hook_arg1 = "template";
		clean_message_contents = 0;
	}

	/*
	 * The remaining cases don't modify the template message, but
	 * just set the argument(s) to the prepare-commit-msg hook.
	 */
	નહીંતર જો (whence == FROM_MERGE)
		hook_arg1 = "merge";
	નહીંતર જો (is_from_cherry_pick(whence) || whence == FROM_REBASE_PICK) {
		hook_arg1 = "commit";
		hook_arg2 = "CHERRY_PICK_HEAD";
	}

	જો (squash_message) {
		/*
		 * If squash_commit was used for the commit subject,
		 * then we're possibly hijacking other commit log options.
		 * Reset the hook args to tell the real story.
		 */
		hook_arg1 = "message";
		hook_arg2 = "";
	}

	s->fp = fopen_for_writing(git_path_commit_editmsg());
	જો (!s->fp)
		die_errno(_("could not open '%s'"), git_path_commit_editmsg());

	/* Ignore status.displayCommentPrefix: we do need comments in COMMIT_EDITMSG. */
	old_display_comment_prefix = s->display_comment_prefix;
	s->display_comment_prefix = 1;

	/*
	 * Most hints are counter-productive when the commit has
	 * already started.
	 */
	s->hints = 0;

	જો (clean_message_contents)
		strbuf_stripspace(&sb, ખાલી);

	જો (signoff)
		append_signoff(&sb, ignored_log_message_bytes(sb.buf, sb.len), 0);

	જો (fwrite(sb.buf, 1, sb.len, s->fp) < sb.len)
		die_errno(_("could not write commit template"));

#ifndef WITH_BREAKING_CHANGES
	જો (auto_comment_લીટી_char)
		adjust_comment_લીટી_char(&sb);
#endif /* !WITH_BREAKING_CHANGES */
	strbuf_release(&sb);

	/* This checks if committer ident is explicitly given */
	strbuf_addstr(&committer_ident, git_committer_info(IDENT_STRICT));
	જો (use_editor && include_status) {
		પૂર્ણાંક ident_shown = 0;
		ગણના git_colorbool saved_color_setting;
		સંરચના ident_split ci, ai;
		અચલ અક્ષર *hint_cleanup_all = allow_empty_message ?
			_("Please enter the commit message for your changes."
			  " Lines starting\nwith '%s' will be ignored.\n") :
			_("Please enter the commit message for your changes."
			  " Lines starting\nwith '%s' will be ignored, and an empty"
			  " message aborts the commit.\n");
		અચલ અક્ષર *hint_cleanup_space = allow_empty_message ?
			_("Please enter the commit message for your changes."
			  " Lines starting\n"
			  "with '%s' will be kept; you may remove them"
			  " yourself if you want to.\n") :
			_("Please enter the commit message for your changes."
			  " Lines starting\n"
			  "with '%s' will be kept; you may remove them"
			  " yourself if you want to.\n"
			  "An empty message aborts the commit.\n");
		જો (whence != FROM_COMMIT) {
			જો (cleanup_mode == COMMIT_સંદેશ_CLEANUP_SCISSORS)
				wt_status_add_cut_લીટી(s);
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
		જો (cleanup_mode == COMMIT_સંદેશ_CLEANUP_ALL)
			status_printf(s, GIT_COLOR_NORMAL, hint_cleanup_all, comment_લીટી_str);
		નહીંતર જો (cleanup_mode == COMMIT_સંદેશ_CLEANUP_SCISSORS) {
			જો (whence == FROM_COMMIT)
				wt_status_add_cut_લીટી(s);
		} નહીંતર /* COMMIT_MSG_CLEANUP_SPACE, that is. */
			status_printf(s, GIT_COLOR_NORMAL, hint_cleanup_space, comment_લીટી_str);

		/*
		 * These should never fail because they come from our own
		 * fmt_ident. They may fail the sane_ident test, but we know
		 * that the name and mail pointers will at least be valid,
		 * which is enough for our tests and printing here.
		 */
		assert_split_ident(&ai, author_ident);
		assert_split_ident(&ci, &committer_ident);

		જો (ident_cmp(&ai, &ci))
			status_printf_ln(s, GIT_COLOR_NORMAL,
				_("%s"
				"Author:    %.*s <%.*s>"),
				ident_shown++ ? "" : "\n",
				(પૂર્ણાંક)(ai.name_end - ai.name_begin), ai.name_begin,
				(પૂર્ણાંક)(ai.mail_end - ai.mail_begin), ai.mail_begin);

		જો (author_date_is_interesting())
			status_printf_ln(s, GIT_COLOR_NORMAL,
				_("%s"
				"Date:      %s"),
				ident_shown++ ? "" : "\n",
				show_ident_date(&ai, DATE_MODE(NORMAL)));

		જો (!committer_ident_sufficiently_given())
			status_printf_ln(s, GIT_COLOR_NORMAL,
				_("%s"
				"Committer: %.*s <%.*s>"),
				ident_shown++ ? "" : "\n",
				(પૂર્ણાંક)(ci.name_end - ci.name_begin), ci.name_begin,
				(પૂર્ણાંક)(ci.mail_end - ci.mail_begin), ci.mail_begin);

		status_printf_ln(s, GIT_COLOR_NORMAL, "%s", ""); /* Add new line for clarity */

		saved_color_setting = s->use_color;
		s->use_color = GIT_COLOR_NEVER;
		committable = run_status(s->fp, index_file, prefix, 1, s);
		s->use_color = saved_color_setting;
		string_list_clear_func(&s->change, change_data_free);
	} નહીંતર {
		સંરચના object_id oid;
		અચલ અક્ષર *parent = "HEAD";

		જો (!the_repository->index->initialized && repo_read_index(the_repository) < 0)
			die(_("Cannot read index"));

		જો (amend)
			parent = "HEAD^1";

		જો (repo_get_oid(the_repository, parent, &oid)) {
			પૂર્ણાંક i, ita_nr = 0;

			/* TODO: audit for interaction with sparse-index. */
			ensure_full_index(the_repository->index);
			માટે (i = 0; i < the_repository->index->cache_nr; i++)
				જો (ce_intent_to_add(the_repository->index->cache[i]))
					ita_nr++;
			committable = the_repository->index->cache_nr > ita_nr;
		} નહીંતર {
			/*
			 * Unless the user did explicitly request a submodule
			 * ignore mode by passing a command line option we do
			 * not ignore any changed submodule SHA-1s when
			 * comparing index and parent, no matter what is
			 * configured. Otherwise we won't commit any
			 * submodules which were manually staged, which would
			 * be really confusing.
			 */
			સંરચના diff_flags flags = DIFF_FLAGS_INIT;
			flags.override_submodule_config = 1;
			જો (ignore_submodule_arg &&
			    !strcmp(ignore_submodule_arg, "all"))
				flags.ignore_submodules = 1;
			committable = index_differs_from(the_repository,
							 parent, &flags, 1);
		}
	}
	strbuf_release(&committer_ident);

	fclose(s->fp);

	જો (trailer_args.nr) {
		જો (amend_file_with_trailers(git_path_commit_editmsg(), &trailer_args))
			die(_("unable to pass trailers to --trailers"));
		strvec_clear(&trailer_args);
	}

	/*
	 * Reject an attempt to record a non-merge empty commit without
	 * explicit --allow-empty. In the cherry-pick case, it may be
	 * empty due to conflict resolution, which the user should okay.
	 */
	જો (!committable && whence != FROM_MERGE && !allow_empty &&
	    !(amend && is_a_merge(current_head))) {
		s->hints = advice_enabled(ADVICE_STATUS_HINTS);
		s->display_comment_prefix = old_display_comment_prefix;
		run_status(stdout, index_file, prefix, 0, s);
		જો (amend)
			fputs(_(empty_amend_advice), stderr);
		નહીંતર જો (is_from_cherry_pick(whence) ||
			 whence == FROM_REBASE_PICK) {
			fputs(_(empty_cherry_pick_advice), stderr);
			જો (whence == FROM_CHERRY_PICK_SINGLE)
				fputs(_(empty_cherry_pick_advice_single), stderr);
			નહીંતર જો (whence == FROM_CHERRY_PICK_MULTI)
				fputs(_(empty_cherry_pick_advice_multi), stderr);
			નહીંતર
				fputs(_(empty_rebase_pick_advice), stderr);
		}
		પાછા 0;
	}

	જો (!no_verify && invoked_hook) {
		/*
		 * Re-read the index as the pre-commit-commit hook was invoked
		 * and could have updated it. We must do this before we invoke
		 * the editor and after we invoke run_status above.
		 */
		discard_index(the_repository->index);
	}
	read_index_from(the_repository->index, index_file, repo_get_git_dir(the_repository));

	જો (cache_વૃક્ષ_update(the_repository->index, 0)) {
		error(_("Error building trees"));
		પાછા 0;
	}

	જો (run_commit_hook(use_editor, index_file, ખાલી, "prepare-commit-msg",
			    git_path_commit_editmsg(), hook_arg1, hook_arg2, ખાલી))
		પાછા 0;

	જો (use_editor) {
		સંરચના strvec env = STRVEC_INIT;

		strvec_pushf(&env, "GIT_INDEX_FILE=%s", index_file);
		જો (launch_editor(git_path_commit_editmsg(), ખાલી, env.v)) {
			fprintf(stderr,
			_("Please supply the message using either -m or -F option.\n"));
			exit(1);
		}
		strvec_clear(&env);
	}

	જો (!no_verify &&
	    run_commit_hook(use_editor, index_file, ખાલી, "commit-msg",
			    git_path_commit_editmsg(), ખાલી)) {
		પાછા 0;
	}

	પાછા 1;
}

સ્થિર અચલ અક્ષર *find_author_by_nickname(અચલ અક્ષર *name)
{
	સંરચના rev_info revs;
	સંરચના commit *commit;
	સંરચના strbuf buf = STRBUF_INIT;
	અચલ અક્ષર *av[20];
	પૂર્ણાંક ac = 0;

	repo_init_revisions(the_repository, &revs, ખાલી);
	strbuf_addf(&buf, "--author=%s", name);
	av[++ac] = "--all";
	av[++ac] = "-i";
	av[++ac] = buf.buf;
	av[++ac] = ખાલી;
	setup_revisions(ac, av, &revs, ખાલી);
	revs.mailmap = xmalloc(કદ(સંરચના string_list));
	string_list_init_nodup(revs.mailmap);
	read_mailmap(the_repository, revs.mailmap);

	જો (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	commit = get_revision(&revs);
	જો (commit) {
		સંરચના pretty_print_context ctx = {0};
		ctx.date_mode.type = DATE_NORMAL;
		strbuf_release(&buf);
		repo_format_commit_message(the_repository, commit,
					   "%aN <%aE>", &buf, &ctx);
		release_revisions(&revs);
		પાછા strbuf_detach(&buf, ખાલી);
	}
	die(_("--author '%s' is not 'Name <email>' and matches no existing author"), name);
}

સ્થિર શૂન્ય handle_ignored_arg(સંરચના wt_status *s)
{
	જો (!ignored_arg)
		; /* default already initialized */
	નહીંતર જો (!strcmp(ignored_arg, "traditional"))
		s->show_ignored_mode = SHOW_TRADITIONAL_IGNORED;
	નહીંતર જો (!strcmp(ignored_arg, "no"))
		s->show_ignored_mode = SHOW_NO_IGNORED;
	નહીંતર જો (!strcmp(ignored_arg, "matching"))
		s->show_ignored_mode = SHOW_MATCHING_IGNORED;
	નહીંતર
		die(_("Invalid ignored mode '%s'"), ignored_arg);
}

સ્થિર ગણના untracked_status_type parse_untracked_setting_name(અચલ અક્ષર *u)
{
	/*
	 * Please update $__git_untracked_file_modes in
	 * git-completion.bash when you add new options
	 */
	પસંદગી (git_parse_maybe_bool(u)) {
	સ્થિતિ 0:
		u = "no";
		તોડો;
	સ્થિતિ 1:
		u = "normal";
		તોડો;
	મૂળભૂત:
		તોડો;
	}

	જો (!strcmp(u, "no"))
		પાછા SHOW_NO_UNTRACKED_FILES;
	નહીંતર જો (!strcmp(u, "normal"))
		પાછા SHOW_NORMAL_UNTRACKED_FILES;
	નહીંતર જો (!strcmp(u, "all"))
		પાછા SHOW_ALL_UNTRACKED_FILES;
	નહીંતર
		પાછા SHOW_UNTRACKED_FILES_ERROR;
}

સ્થિર શૂન્ય handle_untracked_files_arg(સંરચના wt_status *s)
{
	ગણના untracked_status_type u;

	જો (!untracked_files_arg)
		પાછા; /* default already initialized */

	u = parse_untracked_setting_name(untracked_files_arg);
	જો (u == SHOW_UNTRACKED_FILES_ERROR)
		die(_("Invalid untracked files mode '%s'"),
		    untracked_files_arg);
	s->show_untracked_files = u;
}

સ્થિર અચલ અક્ષર *read_commit_message(અચલ અક્ષર *name)
{
	અચલ અક્ષર *out_enc;
	સંરચના commit *commit;

	commit = lookup_commit_reference_by_name(name);
	જો (!commit)
		die(_("could not lookup commit '%s'"), name);
	out_enc = get_commit_output_encoding();
	પાછા repo_logmsg_reencode(the_repository, commit, ખાલી, out_enc);
}

/*
 * Enumerate what needs to be propagated when --porcelain
 * is not in effect here.
 */
સ્થિર સંરચના status_deferred_config {
	ગણના wt_status_format status_format;
	પૂર્ણાંક show_branch;
	ગણના ahead_behind_flags ahead_behind;
} status_deferred_config = {
	STATUS_FORMAT_UNSPECIFIED,
	-1, /* unspecified */
	AHEAD_BEHIND_UNSPECIFIED,
};

સ્થિર શૂન્ય finalize_deferred_config(સંરચના wt_status *s)
{
	પૂર્ણાંક use_deferred_config = (status_format != STATUS_FORMAT_PORCELAIN &&
				   status_format != STATUS_FORMAT_PORCELAIN_V2 &&
				   !s->null_termination);

	જો (s->null_termination) {
		જો (status_format == STATUS_FORMAT_NONE ||
		    status_format == STATUS_FORMAT_UNSPECIFIED)
			status_format = STATUS_FORMAT_PORCELAIN;
		નહીંતર જો (status_format == STATUS_FORMAT_LONG)
			die(_("options '%s' and '%s' cannot be used together"), "--long", "-z");
	}

	જો (use_deferred_config && status_format == STATUS_FORMAT_UNSPECIFIED)
		status_format = status_deferred_config.status_format;
	જો (status_format == STATUS_FORMAT_UNSPECIFIED)
		status_format = STATUS_FORMAT_NONE;

	જો (use_deferred_config && s->show_branch < 0)
		s->show_branch = status_deferred_config.show_branch;
	જો (s->show_branch < 0)
		s->show_branch = 0;

	/*
	 * If the user did not give a "--[no]-ahead-behind" command
	 * line argument *AND* we will print in a human-readable format
	 * (short, long etc.) then we inherit from the status.aheadbehind
	 * config setting.  In all other cases (and porcelain V[12] formats
	 * in particular), we inherit _FULL for backwards compatibility.
	 */
	જો (use_deferred_config &&
	    s->ahead_behind_flags == AHEAD_BEHIND_UNSPECIFIED)
		s->ahead_behind_flags = status_deferred_config.ahead_behind;

	જો (s->ahead_behind_flags == AHEAD_BEHIND_UNSPECIFIED)
		s->ahead_behind_flags = AHEAD_BEHIND_FULL;
}

સ્થિર શૂન્ય check_fixup_reword_options(પૂર્ણાંક argc, અચલ અક્ષર *argv[]) {
	જો (whence != FROM_COMMIT) {
		જો (whence == FROM_MERGE)
			die(_("You are in the middle of a merge -- cannot reword."));
		નહીંતર જો (is_from_cherry_pick(whence))
			die(_("You are in the middle of a cherry-pick -- cannot reword."));
	}
	જો (argc)
		die(_("reword option of '%s' and path '%s' cannot be used together"), "--fixup", *argv);
	જો (patch_interactive || interactive || all || also || only)
		die(_("reword option of '%s' and '%s' cannot be used together"),
			"--fixup", "--patch/--interactive/--all/--include/--only");
}

સ્થિર પૂર્ણાંક parse_and_validate_options(પૂર્ણાંક argc, અચલ અક્ષર *argv[],
				      અચલ સંરચના option *options,
				      અચલ અક્ષર * અચલ usage[],
				      અચલ અક્ષર *prefix,
				      સંરચના commit *current_head,
				      સંરચના wt_status *s)
{
	argc = parse_options(argc, argv, prefix, options, usage, 0);
	finalize_deferred_config(s);

	જો (force_author && !strchr(force_author, '>'))
		force_author = find_author_by_nickname(force_author);

	જો (force_author && renew_authorship)
		die(_("options '%s' and '%s' cannot be used together"), "--reset-author", "--author");

	જો (logfile || have_option_m || use_message)
		use_editor = 0;

	/* Sanity check options */
	જો (amend && !current_head)
		die(_("You have nothing to amend."));
	જો (amend && whence != FROM_COMMIT) {
		જો (whence == FROM_MERGE)
			die(_("You are in the middle of a merge -- cannot amend."));
		નહીંતર જો (is_from_cherry_pick(whence))
			die(_("You are in the middle of a cherry-pick -- cannot amend."));
		નહીંતર જો (whence == FROM_REBASE_PICK)
			die(_("You are in the middle of a rebase -- cannot amend."));
	}
	જો (fixup_message && squash_message)
		die(_("options '%s' and '%s' cannot be used together"), "--squash", "--fixup");
	die_for_incompatible_opt4(!!use_message, "-C",
				  !!edit_message, "-c",
				  !!logfile, "-F",
				  !!fixup_message, "--fixup");
	die_for_incompatible_opt4(have_option_m, "-m",
				  !!edit_message, "-c",
				  !!use_message, "-C",
				  !!logfile, "-F");
	જો (use_message || edit_message || logfile ||fixup_message || have_option_m)
		FREE_AND_NULL(template_file);
	જો (edit_message)
		use_message = edit_message;
	જો (amend && !use_message && !fixup_message)
		use_message = "HEAD";
	જો (!use_message && !is_from_cherry_pick(whence) &&
	    !is_from_rebase(whence) && renew_authorship)
		die(_("--reset-author can be used only with -C, -c or --amend."));
	જો (use_message) {
		use_message_buffer = read_commit_message(use_message);
		જો (!renew_authorship) {
			author_message = use_message;
			author_message_buffer = use_message_buffer;
		}
	}
	જો ((is_from_cherry_pick(whence) || whence == FROM_REBASE_PICK) &&
	    !renew_authorship) {
		author_message = "CHERRY_PICK_HEAD";
		author_message_buffer = read_commit_message(author_message);
	}

	જો (patch_interactive)
		interactive = 1;

	die_for_incompatible_opt4(also, "-i/--include",
				  only, "-o/--only",
				  all, "-a/--all",
				  interactive, "--interactive/-p/--patch");
	જો (fixup_message) {
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
		અક્ષર *p = fixup_message;
		જ્યારે (isalpha(*p))
			p++;
		જો (p > fixup_message && *p == ':') {
			*p = '\0';
			fixup_commit = p + 1;
			જો (!strcmp("amend", fixup_message) ||
			    !strcmp("reword", fixup_message)) {
				fixup_prefix = "amend";
				allow_empty = 1;
				જો (*fixup_message == 'r') {
					check_fixup_reword_options(argc, argv);
					only = 1;
				}
			} નહીંતર {
				die(_("unknown option: --fixup=%s:%s"), fixup_message, fixup_commit);
			}
		} નહીંતર {
			fixup_commit = fixup_message;
			fixup_prefix = "fixup";
			use_editor = 0;
		}
	}

	જો (0 <= edit_flag)
		use_editor = edit_flag;

	handle_untracked_files_arg(s);

	જો (all && argc > 0)
		die(_("paths '%s ...' with -a does not make sense"),
		    argv[0]);

	જો (status_format != STATUS_FORMAT_NONE)
		dry_run = 1;

	પાછા argc;
}

સ્થિર પૂર્ણાંક dry_run_commit(અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
			  અચલ સંરચના commit *current_head, સંરચના wt_status *s)
{
	પૂર્ણાંક committable;
	અચલ અક્ષર *index_file;

	index_file = prepare_index(argv, prefix, current_head, 1);
	committable = run_status(stdout, index_file, prefix, 0, s);
	rollback_index_files();

	પાછા committable ? 0 : 1;
}

define_list_config_array_extra(color_status_slots, {"added"});

સ્થિર પૂર્ણાંક parse_status_slot(અચલ અક્ષર *slot)
{
	જો (!strcasecmp(slot, "added"))
		પાછા WT_STATUS_UPDATED;

	પાછા LOOKUP_CONFIG(color_status_slots, slot);
}

સ્થિર પૂર્ણાંક git_status_config(અચલ અક્ષર *k, અચલ અક્ષર *v,
			     અચલ સંરચના config_context *ctx, શૂન્ય *cb)
{
	સંરચના wt_status *s = cb;
	અચલ અક્ષર *slot_name;

	જો (starts_with(k, "column."))
		પાછા git_column_config(k, v, "status", &s->colopts);
	જો (!strcmp(k, "status.submodulesummary")) {
		પૂર્ણાંક is_bool;
		s->submodule_summary = git_config_bool_or_int(k, v, ctx->kvi,
							      &is_bool);
		જો (is_bool && s->submodule_summary)
			s->submodule_summary = -1;
		પાછા 0;
	}
	જો (!strcmp(k, "status.short")) {
		જો (git_config_bool(k, v))
			status_deferred_config.status_format = STATUS_FORMAT_SHORT;
		નહીંતર
			status_deferred_config.status_format = STATUS_FORMAT_NONE;
		પાછા 0;
	}
	જો (!strcmp(k, "status.branch")) {
		status_deferred_config.show_branch = git_config_bool(k, v);
		પાછા 0;
	}
	જો (!strcmp(k, "status.aheadbehind")) {
		status_deferred_config.ahead_behind = git_config_bool(k, v);
		પાછા 0;
	}
	જો (!strcmp(k, "status.showstash")) {
		s->show_stash = git_config_bool(k, v);
		પાછા 0;
	}
	જો (!strcmp(k, "status.color") || !strcmp(k, "color.status")) {
		s->use_color = git_config_colorbool(k, v);
		પાછા 0;
	}
	જો (!strcmp(k, "status.displaycommentprefix")) {
		s->display_comment_prefix = git_config_bool(k, v);
		પાછા 0;
	}
	જો (skip_prefix(k, "status.color.", &slot_name) ||
	    skip_prefix(k, "color.status.", &slot_name)) {
		પૂર્ણાંક slot = parse_status_slot(slot_name);
		જો (slot < 0)
			પાછા 0;
		જો (!v)
			પાછા config_error_nonbool(k);
		પાછા color_parse(v, s->color_palette[slot]);
	}
	જો (!strcmp(k, "status.relativepaths")) {
		s->relative_paths = git_config_bool(k, v);
		પાછા 0;
	}
	જો (!strcmp(k, "status.showuntrackedfiles")) {
		ગણના untracked_status_type u;

		u = parse_untracked_setting_name(v);
		જો (u == SHOW_UNTRACKED_FILES_ERROR)
			પાછા error(_("Invalid untracked files mode '%s'"), v);
		s->show_untracked_files = u;
		પાછા 0;
	}
	જો (!strcmp(k, "diff.renamelimit")) {
		જો (s->rename_limit == -1)
			s->rename_limit = git_config_int(k, v, ctx->kvi);
		પાછા 0;
	}
	જો (!strcmp(k, "status.renamelimit")) {
		s->rename_limit = git_config_int(k, v, ctx->kvi);
		પાછા 0;
	}
	જો (!strcmp(k, "diff.renames")) {
		જો (s->detect_rename == -1)
			s->detect_rename = git_config_rename(k, v);
		પાછા 0;
	}
	જો (!strcmp(k, "status.renames")) {
		s->detect_rename = git_config_rename(k, v);
		પાછા 0;
	}
	પાછા git_diff_ui_config(k, v, ctx, ખાલી);
}

પૂર્ણાંક cmd_status(પૂર્ણાંક argc,
અચલ અક્ષર **argv,
અચલ અક્ષર *prefix,
સંરચના repository *repo UNUSED)
{
	સ્થિર પૂર્ણાંક no_renames = -1;
	સ્થિર અચલ અક્ષર *rename_score_arg = (અચલ અક્ષર *)-1;
	સ્થિર સંરચના wt_status s;
	unsigned પૂર્ણાંક progress_flag = 0;
	પૂર્ણાંક fd;
	સંરચના object_id oid;
	સ્થિર સંરચના option builtin_status_options[] = {
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
			.કિંમત = &untracked_files_arg,
			.argh = N_("mode"),
			.help = N_("show untracked files, optional modes: all, normal, no. (Default: all)"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t)"all",
		},
		{
			.type = OPTION_STRING,
			.long_name = "ignored",
			.કિંમત = &ignored_arg,
			.argh = N_("mode"),
			.help = N_("show ignored files, optional modes: traditional, matching, no. (Default: traditional)"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t)"traditional",
		},
		{
			.type = OPTION_STRING,
			.long_name = "ignore-submodules",
			.કિંમત = &ignore_submodule_arg,
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

	જો (s.show_ignored_mode == SHOW_MATCHING_IGNORED &&
	    s.show_untracked_files == SHOW_NO_UNTRACKED_FILES)
		die(_("Unsupported combination of ignored and untracked-files arguments"));

	parse_pathspec(&s.pathspec, 0,
		       PATHSPEC_PREFER_FULL,
		       prefix, argv);

	જો (status_format != STATUS_FORMAT_PORCELAIN &&
	    status_format != STATUS_FORMAT_PORCELAIN_V2)
		progress_flag = REFRESH_PROGRESS;
	repo_read_index(the_repository);
	refresh_index(the_repository->index,
		      REFRESH_QUIET|REFRESH_UNMERGED|progress_flag,
		      &s.pathspec, ખાલી, ખાલી);

	જો (use_optional_locks())
		fd = repo_hold_locked_index(the_repository, &index_lock, 0);
	નહીંતર
		fd = -1;

	s.is_initial = repo_get_oid(the_repository, s.reference, &oid) ? 1 : 0;
	જો (!s.is_initial)
		oidcpy(&s.oid_commit, &oid);

	s.ignore_submodule_arg = ignore_submodule_arg;
	s.status_format = status_format;
	s.verbose = verbose;
	જો (no_renames != -1)
		s.detect_rename = !no_renames;
	જો ((intptr_t)rename_score_arg != -1) {
		જો (s.detect_rename < DIFF_DETECT_RENAME)
			s.detect_rename = DIFF_DETECT_RENAME;
		જો (rename_score_arg)
			s.rename_score = parse_rename_score(&rename_score_arg);
	}

	wt_status_collect(&s);

	જો (0 <= fd)
		repo_update_index_if_able(the_repository, &index_lock);

	જો (s.relative_paths)
		s.prefix = prefix;

	wt_status_print(&s);
	wt_status_collect_free_buffers(&s);

	પાછા 0;
}

સ્થિર પૂર્ણાંક git_commit_config(અચલ અક્ષર *k, અચલ અક્ષર *v,
			     અચલ સંરચના config_context *ctx, શૂન્ય *cb)
{
	સંરચના wt_status *s = cb;

	જો (!strcmp(k, "commit.template"))
		પાછા git_config_pathname(&template_file, k, v);
	જો (!strcmp(k, "commit.status")) {
		include_status = git_config_bool(k, v);
		પાછા 0;
	}
	જો (!strcmp(k, "commit.cleanup")) {
		FREE_AND_NULL(cleanup_config);
		પાછા git_config_string(&cleanup_config, k, v);
	}
	જો (!strcmp(k, "commit.gpgsign")) {
		sign_commit = git_config_bool(k, v) ? "" : ખાલી;
		પાછા 0;
	}
	જો (!strcmp(k, "commit.verbose")) {
		પૂર્ણાંક is_bool;
		config_commit_verbose = git_config_bool_or_int(k, v, ctx->kvi,
							       &is_bool);
		પાછા 0;
	}

	પાછા git_status_config(k, v, ctx, s);
}

પૂર્ણાંક cmd_commit(પૂર્ણાંક argc,
	       અચલ અક્ષર **argv,
	       અચલ અક્ષર *prefix,
	       સંરચના repository *repo UNUSED)
{
	સ્થિર સંરચના wt_status s;
	સ્થિર અચલ અક્ષર *cleanup_arg = ખાલી;
	સ્થિર સંરચના option builtin_commit_options[] = {
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
			.કિંમત = &sign_commit,
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
			.કિંમત = &untracked_files_arg,
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

	સંરચના strbuf sb = STRBUF_INIT;
	સંરચના strbuf author_ident = STRBUF_INIT;
	અચલ અક્ષર *index_file, *reflog_સંદેશ;
	સંરચના object_id oid;
	સંરચના commit_list *parents = ખાલી;
	સંરચના stat statbuf;
	સંરચના commit *current_head = ખાલી;
	સંરચના commit_extra_header *extra = ખાલી;
	સંરચના strbuf err = STRBUF_INIT;
	પૂર્ણાંક ret = 0;

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

	જો (repo_get_oid(the_repository, "HEAD", &oid))
		current_head = ખાલી;
	નહીંતર {
		current_head = lookup_commit_or_die(&oid, "HEAD");
		જો (repo_parse_commit(the_repository, current_head))
			die(_("could not parse HEAD commit"));
	}
	verbose = -1; /* unspecified */
	argc = parse_and_validate_options(argc, argv, builtin_commit_options,
					  builtin_commit_usage,
					  prefix, current_head, &s);
	જો (trailer_args.nr)
		trailer_config_init();

	જો (verbose == -1)
		verbose = (config_commit_verbose < 0) ? 0 : config_commit_verbose;

	જો (cleanup_arg) {
		મુક્ત(cleanup_config);
		cleanup_config = xstrdup(cleanup_arg);
	}
	cleanup_mode = get_cleanup_mode(cleanup_config, use_editor);

	જો (dry_run)
		પાછા dry_run_commit(argv, prefix, current_head, &s);
	index_file = prepare_index(argv, prefix, current_head, 0);

	/* Set up everything for writing the commit object.  This includes
	   running hooks, writing the trees, and interacting with the user.  */
	જો (!prepare_to_commit(index_file, prefix,
			       current_head, &s, &author_ident)) {
		ret = 1;
		rollback_index_files();
		જાઓ cleanup;
	}

	/* Determine parents */
	reflog_સંદેશ = getenv("GIT_REFLOG_ACTION");
	જો (!current_head) {
		જો (!reflog_સંદેશ)
			reflog_સંદેશ = "commit (initial)";
	} નહીંતર જો (amend) {
		જો (!reflog_સંદેશ)
			reflog_સંદેશ = "commit (amend)";
		parents = commit_list_copy(current_head->parents);
	} નહીંતર જો (whence == FROM_MERGE) {
		સંરચના strbuf m = STRBUF_INIT;
		FILE *fp;
		પૂર્ણાંક allow_fast_forward = 1;
		સંરચના commit_list **pptr = &parents;

		જો (!reflog_સંદેશ)
			reflog_સંદેશ = "commit (merge)";
		pptr = commit_list_append(current_head, pptr);
		fp = xfopen(git_path_merge_head(the_repository), "r");
		જ્યારે (strbuf_getline_lf(&m, fp) != EOF) {
			સંરચના commit *parent;

			parent = get_merge_parent(m.buf);
			જો (!parent)
				die(_("Corrupt MERGE_HEAD file (%s)"), m.buf);
			pptr = commit_list_append(parent, pptr);
		}
		fclose(fp);
		strbuf_release(&m);
		જો (!stat(git_path_merge_mode(the_repository), &statbuf)) {
			જો (strbuf_read_file(&sb, git_path_merge_mode(the_repository), 0) < 0)
				die_errno(_("could not read MERGE_MODE"));
			જો (!strcmp(sb.buf, "no-ff"))
				allow_fast_forward = 0;
		}
		જો (allow_fast_forward)
			reduce_heads_replace(&parents);
	} નહીંતર {
		જો (!reflog_સંદેશ)
			reflog_સંદેશ = is_from_cherry_pick(whence)
					? "commit (cherry-pick)"
					: is_from_rebase(whence)
					? "commit (rebase)"
					: "commit";
		commit_list_ઉમેરો(current_head, &parents);
	}

	/* Finally, get the commit message */
	strbuf_reset(&sb);
	જો (strbuf_read_file(&sb, git_path_commit_editmsg(), 0) < 0) {
		પૂર્ણાંક saved_errno = errno;
		rollback_index_files();
		die(_("could not read commit message: %s"), strerror(saved_errno));
	}

	cleanup_message(&sb, cleanup_mode, verbose);

	જો (message_is_empty(&sb, cleanup_mode) && !allow_empty_message) {
		rollback_index_files();
		fprintf(stderr, _("Aborting commit due to empty commit message.\n"));
		exit(1);
	}
	જો (template_untouched(&sb, template_file, cleanup_mode) && !allow_empty_message) {
		rollback_index_files();
		fprintf(stderr, _("Aborting commit; you did not edit the message.\n"));
		exit(1);
	}

	જો (fixup_message && starts_with(sb.buf, "amend! ") &&
	    !allow_empty_message) {
		સંરચના strbuf body = STRBUF_INIT;
		size_t len = commit_subject_length(sb.buf);
		strbuf_addstr(&body, sb.buf + len);
		જો (message_is_empty(&body, cleanup_mode)) {
			rollback_index_files();
			fprintf(stderr, _("Aborting commit due to empty commit message body.\n"));
			exit(1);
		}
		strbuf_release(&body);
	}

	જો (amend) {
		અચલ અક્ષર *exclude_gpgsig[3] = { "gpgsig", "gpgsig-sha256", ખાલી };
		extra = read_commit_extra_headers(current_head, exclude_gpgsig);
	} નહીંતર {
		સંરચના commit_extra_header **tail = &extra;
		append_merge_tag_headers(parents, &tail);
	}

	જો (commit_વૃક્ષ_extended(sb.buf, sb.len, &the_repository->index->cache_વૃક્ષ->oid,
				 parents, &oid, author_ident.buf, ખાલી,
				 sign_commit, extra)) {
		rollback_index_files();
		die(_("failed to write commit object"));
	}

	જો (update_head_with_reflog(current_head, &oid, reflog_સંદેશ, &sb,
				    &err)) {
		rollback_index_files();
		die("%s", err.buf);
	}

	sequencer_post_commit_cleanup(the_repository, 0);
	unlink(git_path_merge_head(the_repository));
	unlink(git_path_merge_સંદેશ(the_repository));
	unlink(git_path_merge_mode(the_repository));
	unlink(git_path_squash_સંદેશ(the_repository));

	જો (commit_index_files())
		die(_("repository has been updated, but unable to write\n"
		      "new index file. Check that disk is not full and quota is\n"
		      "not exceeded, and then \"git restore --staged :/\" to recover."));

	git_test_write_commit_graph_or_die(the_repository);

	repo_rerere(the_repository, 0);
	run_auto_maintenance(the_repository, quiet);
	run_commit_hook(use_editor, repo_get_index_file(the_repository),
			ખાલી, "post-commit", ખાલી);
	જો (amend && !no_post_rewrite) {
		commit_post_rewrite(the_repository, current_head, &oid);
	}
	જો (!quiet) {
		unsigned પૂર્ણાંક flags = 0;

		જો (!current_head)
			flags |= SUMMARY_INITIAL_COMMIT;
		જો (author_date_is_interesting())
			flags |= SUMMARY_SHOW_AUTHOR_DATE;
		print_commit_summary(the_repository, prefix,
				     &oid, flags);
	}

	apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
			    ખાલી, ખાલી, ખાલી, ખાલી);

cleanup:
	free_commit_extra_headers(extra);
	commit_list_free(parents);
	strbuf_release(&author_ident);
	strbuf_release(&err);
	strbuf_release(&sb);
	મુક્ત(logfile);
	મુક્ત(template_file);
	પાછા ret;
}
