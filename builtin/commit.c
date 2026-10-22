/*
 * Builtin "git commit"
 *
 * Copyright (c) 2007 Kristian Høgsberg <krh@redhat.com>
 * Based on git-commit.sh by Junio C Hamano and Linus Torvalds
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "advice.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "lockfile.h"
#அந்தர்பாவ "cache-tree.h"
#அந்தர்பாவ "color.h"
#அந்தர்பாவ "dir.h"
#அந்தர்பாவ "editor.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "diff.h"
#அந்தர்பாவ "commit.h"
#அந்தர்பாவ "add-interactive.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "revision.h"
#அந்தர்பாவ "wt-status.h"
#அந்தர்பாவ "run-command.h"
#அந்தர்பாவ "strbuf.h"
#அந்தர்பாவ "object-name.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "path.h"
#அந்தர்பாவ "preload-index.h"
#அந்தர்பாவ "read-cache.h"
#அந்தர்பாவ "repository.h"
#அந்தர்பாவ "string-list.h"
#அந்தர்பாவ "rerere.h"
#அந்தர்பாவ "unpack-trees.h"
#அந்தர்பாவ "column.h"
#அந்தர்பாவ "sequencer.h"
#அந்தர்பாவ "sparse-index.h"
#அந்தர்பாவ "mailmap.h"
#அந்தர்பாவ "help.h"
#அந்தர்பாவ "commit-reach.h"
#அந்தர்பாவ "commit-graph.h"
#அந்தர்பாவ "pretty.h"
#அந்தர்பாவ "trailer.h"

நிலையான மாறிலி எழுத்து * மாறிலி builtin_commit_usage[] = {
	N_("git commit [-a | --interactive | --patch] [-s] [-v] [-u[<mode>]] [--amend]\n"
	   "           [--dry-run] [(-c | -C | --squash) <commit> | --fixup [(amend|reword):]<commit>]\n"
	   "           [-F <file> | -m <msg>] [--reset-author] [--allow-empty]\n"
	   "           [--allow-empty-message] [--no-verify] [-e] [--author=<author>]\n"
	   "           [--date=<date>] [--cleanup=<mode>] [--[no-]status]\n"
	   "           [-i | -o] [--pathspec-from-file=<file> [--pathspec-file-nul]]\n"
	   "           [(--trailer <token>[(=|:)<value>])...] [-S[<keyid>]]\n"
	   "           [--] [<pathspec>...]"),
	வெற்று
};

நிலையான மாறிலி எழுத்து * மாறிலி builtin_status_usage[] = {
	N_("git status [<options>] [--] [<pathspec>...]"),
	வெற்று
};

நிலையான மாறிலி எழுத்து empty_amend_advice[] =
N_("You asked to amend the most recent commit, but doing so would make\n"
"it empty. You can repeat your command with --allow-empty, or you can\n"
"remove the commit entirely with \"git reset HEAD^\".\n");

நிலையான மாறிலி எழுத்து empty_cherry_pick_advice[] =
N_("The previous cherry-pick is now empty, possibly due to conflict resolution.\n"
"If you wish to commit it anyway, use:\n"
"\n"
"    git commit --allow-empty\n"
"\n");

நிலையான மாறிலி எழுத்து empty_rebase_pick_advice[] =
N_("Otherwise, please use 'git rebase --skip'\n");

நிலையான மாறிலி எழுத்து empty_cherry_pick_advice_single[] =
N_("Otherwise, please use 'git cherry-pick --skip'\n");

நிலையான மாறிலி எழுத்து empty_cherry_pick_advice_multi[] =
N_("and then use:\n"
"\n"
"    git cherry-pick --continue\n"
"\n"
"to resume cherry-picking the remaining commits.\n"
"If you wish to skip this commit, use:\n"
"\n"
"    git cherry-pick --skip\n"
"\n");

நிலையான மாறிலி எழுத்து *color_status_slots[] = {
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

நிலையான மாறிலி எழுத்து *use_message_buffer;
நிலையான கட்டமைப்பு lock_file index_lock; /* real index */
நிலையான கட்டமைப்பு lock_file false_lock; /* used only for partial commits */
நிலையான எண்ணிக்கை {
	COMMIT_AS_IS = 1,
	COMMIT_NORMAL,
	COMMIT_PARTIAL
} commit_style;

நிலையான மாறிலி எழுத்து *force_author;
நிலையான எழுத்து *logfile;
நிலையான எழுத்து *template_file;
/*
 * The _message variables are commit names from which to take
 * the commit message and/or authorship.
 */
நிலையான மாறிலி எழுத்து *author_message, *author_message_buffer;
நிலையான மாறிலி எழுத்து *edit_message, *use_message;
நிலையான எழுத்து *fixup_message, *fixup_commit, *squash_message;
நிலையான மாறிலி எழுத்து *fixup_prefix;
நிலையான முழு all, also, interactive, patch_interactive, only, amend, signoff;
நிலையான கட்டமைப்பு interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
நிலையான முழு edit_flag = -1; /* unspecified */
நிலையான முழு quiet, verbose, no_verify, allow_empty, dry_run, renew_authorship;
நிலையான முழு config_commit_verbose = -1; /* unspecified */
நிலையான முழு no_post_rewrite, allow_empty_message, pathspec_file_nul;
நிலையான மாறிலி எழுத்து *untracked_files_arg, *force_date, *ignore_submodule_arg, *ignored_arg;
நிலையான மாறிலி எழுத்து *sign_commit, *pathspec_from_file;
நிலையான கட்டமைப்பு strvec trailer_args = STRVEC_INIT;

/*
 * The default commit message cleanup mode will remove the lines
 * beginning with # (shell comments) and leading and trailing
 * whitespaces (empty lines or containing only whitespaces)
 * if editor is used, and only the whitespaces if the message
 * is specified explicitly.
 */
நிலையான எண்ணிக்கை commit_செய்தி_cleanup_mode cleanup_mode;
நிலையான எழுத்து *cleanup_config;

நிலையான எண்ணிக்கை commit_whence whence;
நிலையான முழு use_editor = 1, include_status = 1;
நிலையான முழு have_option_m;
நிலையான கட்டமைப்பு strbuf message = STRBUF_INIT;

நிலையான எண்ணிக்கை wt_status_format status_format = STATUS_FORMAT_UNSPECIFIED;

நிலையான முழு opt_parse_porcelain(மாறிலி கட்டமைப்பு option *opt, மாறிலி எழுத்து *arg, முழு unset)
{
	எண்ணிக்கை wt_status_format *மதிப்பு = (எண்ணிக்கை wt_status_format *)opt->மதிப்பு;
	எனில் (unset)
		*மதிப்பு = STATUS_FORMAT_NONE;
	இல்லையெனில் எனில் (!arg)
		*மதிப்பு = STATUS_FORMAT_PORCELAIN;
	இல்லையெனில் எனில் (!strcmp(arg, "v1") || !strcmp(arg, "1"))
		*மதிப்பு = STATUS_FORMAT_PORCELAIN;
	இல்லையெனில் எனில் (!strcmp(arg, "v2") || !strcmp(arg, "2"))
		*மதிப்பு = STATUS_FORMAT_PORCELAIN_V2;
	இல்லையெனில்
		die("unsupported porcelain version '%s'", arg);

	திரும்பு 0;
}

நிலையான முழு opt_parse_m(மாறிலி கட்டமைப்பு option *opt, மாறிலி எழுத்து *arg, முழு unset)
{
	கட்டமைப்பு strbuf *buf = opt->மதிப்பு;
	எனில் (unset) {
		have_option_m = 0;
		strbuf_setlen(buf, 0);
	} இல்லையெனில் {
		have_option_m = 1;
		எனில் (buf->len)
			strbuf_addch(buf, '\n');
		strbuf_addstr(buf, arg);
		strbuf_complete_வரி(buf);
	}
	திரும்பு 0;
}

நிலையான முழு opt_parse_rename_score(மாறிலி கட்டமைப்பு option *opt, மாறிலி எழுத்து *arg, முழு unset)
{
	மாறிலி எழுத்து **மதிப்பு = opt->மதிப்பு;

	BUG_ON_OPT_NEG(unset);

	எனில் (arg != வெற்று && *arg == '=')
		arg = arg + 1;

	*மதிப்பு = arg;
	திரும்பு 0;
}

நிலையான வெறுமை determine_whence(கட்டமைப்பு wt_status *s)
{
	எனில் (file_exists(git_path_merge_head(the_repository)))
		whence = FROM_MERGE;
	இல்லையெனில் எனில் (!sequencer_determine_whence(the_repository, &whence))
		whence = FROM_COMMIT;
	எனில் (s)
		s->whence = whence;
}

நிலையான வெறுமை status_init_config(கட்டமைப்பு wt_status *s, config_fn_t fn)
{
	wt_status_prepare(the_repository, s);
	init_diff_ui_defaults();
	repo_config(the_repository, fn, s);
	determine_whence(s);
	s->hints = advice_enabled(ADVICE_STATUS_HINTS); /* must come after repo_config() */
}

நிலையான வெறுமை rollback_index_files(வெறுமை)
{
	தேர்வு (commit_style) {
	நிலை COMMIT_AS_IS:
		நிறுத்து; /* nothing to do */
	நிலை COMMIT_NORMAL:
		rollback_lock_file(&index_lock);
		நிறுத்து;
	நிலை COMMIT_PARTIAL:
		rollback_lock_file(&index_lock);
		rollback_lock_file(&false_lock);
		நிறுத்து;
	}
}

நிலையான முழு commit_index_files(வெறுமை)
{
	முழு err = 0;

	தேர்வு (commit_style) {
	நிலை COMMIT_AS_IS:
		நிறுத்து; /* nothing to do */
	நிலை COMMIT_NORMAL:
		err = commit_lock_file(&index_lock);
		நிறுத்து;
	நிலை COMMIT_PARTIAL:
		err = commit_lock_file(&index_lock);
		rollback_lock_file(&false_lock);
		நிறுத்து;
	}

	திரும்பு err;
}

/*
 * Take a union of paths in the index and the named tree (typically, "HEAD"),
 * and return the paths that match the given pattern in list.
 */
நிலையான முழு list_paths(கட்டமைப்பு string_list *list, மாறிலி எழுத்து *with_மரம்,
		      மாறிலி கட்டமைப்பு pathspec *pattern)
{
	முழு i, ret;
	எழுத்து *m;

	எனில் (!pattern->nr)
		திரும்பு 0;

	m = xcalloc(1, pattern->nr);

	எனில் (with_மரம்) {
		எழுத்து *max_prefix = common_prefix(pattern);
		overlay_மரம்_on_index(the_repository->index, with_மரம், max_prefix);
		விடுவி(max_prefix);
	}

	/* TODO: audit for interaction with sparse-index. */
	ensure_full_index(the_repository->index);
	க்கு (i = 0; i < the_repository->index->cache_nr; i++) {
		மாறிலி கட்டமைப்பு cache_entry *ce = the_repository->index->cache[i];
		கட்டமைப்பு string_list_item *item;

		எனில் (ce->ce_flags & CE_UPDATE)
			தொடர்;
		எனில் (!ce_path_match(the_repository->index, ce, pattern, m))
			தொடர்;
		item = string_list_செருகு(list, ce->name);
		எனில் (ce_skip_worktree(ce))
			item->util = item; /* better a valid pointer than a fake one */
	}

	ret = report_path_error(m, pattern);
	விடுவி(m);
	திரும்பு ret;
}

நிலையான வெறுமை add_remove_files(கட்டமைப்பு string_list *list)
{
	முழு i;
	க்கு (i = 0; i < list->nr; i++) {
		கட்டமைப்பு stat st;
		கட்டமைப்பு string_list_item *p = &(list->பொருட்கள்[i]);

		/* p->util is skip-worktree */
		எனில் (p->util)
			தொடர்;

		எனில் (!lstat(p->string, &st)) {
			எனில் (add_to_index(the_repository->index, p->string, &st, 0))
				die(_("updating files failed"));
		} இல்லையெனில்
			remove_file_from_index(the_repository->index, p->string);
	}
}

நிலையான வெறுமை create_base_index(மாறிலி கட்டமைப்பு commit *current_head)
{
	கட்டமைப்பு மரம் *மரம்;
	கட்டமைப்பு unpack_trees_options opts;
	கட்டமைப்பு மரம்_desc t;

	எனில் (!current_head) {
		discard_index(the_repository->index);
		திரும்பு;
	}

	memset(&opts, 0, அளவு(opts));
	opts.head_idx = 1;
	opts.index_only = 1;
	opts.merge = 1;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;

	opts.fn = oneway_merge;
	மரம் = repo_parse_மரம்_indirect(the_repository,
					&current_head->object.oid);
	எனில் (!மரம்)
		die(_("failed to unpack HEAD tree object"));
	எனில் (repo_parse_மரம்(the_repository, மரம்) < 0)
		exit(128);
	init_மரம்_desc(&t, &மரம்->object.oid, மரம்->buffer, மரம்->size);
	எனில் (unpack_trees(1, &t, &opts))
		exit(128); /* We've already reported the error, finish dying */
}

நிலையான வெறுமை refresh_cache_or_die(முழு refresh_flags)
{
	/*
	 * refresh_flags contains REFRESH_QUIET, so the only errors
	 * are for unmerged entries.
	 */
	எனில் (refresh_index(the_repository->index, refresh_flags | REFRESH_IN_PORCELAIN, வெற்று, வெற்று, வெற்று))
		die_resolve_conflict("commit");
}

நிலையான மாறிலி எழுத்து *prepare_index(மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
				 மாறிலி கட்டமைப்பு commit *current_head, முழு is_status)
{
	கட்டமைப்பு string_list partial = STRING_LIST_INIT_DUP;
	கட்டமைப்பு pathspec pathspec;
	முழு refresh_flags = REFRESH_QUIET;
	மாறிலி எழுத்து *ret;
	எழுத்து *path = வெற்று;

	எனில் (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	எனில் (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	எனில் (is_status)
		refresh_flags |= REFRESH_UNMERGED;
	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_FULL,
		       prefix, argv);

	எனில் (pathspec_from_file) {
		எனில் (interactive)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--interactive/--patch");

		எனில் (all)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "-a");

		எனில் (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_FULL,
				    prefix, pathspec_from_file, pathspec_file_nul);
	} இல்லையெனில் எனில் (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	எனில் (!pathspec.nr && (also || (only && !allow_empty &&
	    (!amend || (fixup_message && strcmp(fixup_prefix, "amend"))))))
		die(_("No paths with --include/--only does not make sense."));

	எனில் (repo_read_index_preload(the_repository, &pathspec, 0) < 0)
		die(_("index file corrupt"));

	எனில் (interactive) {
		எழுத்து *old_index_env = வெற்று, *old_repo_index_file;
		repo_hold_locked_index(the_repository, &index_lock,
				       LOCK_DIE_ON_ERROR);

		refresh_cache_or_die(refresh_flags);

		எனில் (write_locked_index(the_repository->index, &index_lock, 0))
			die(_("unable to create temporary index"));

		old_repo_index_file = the_repository->index_file;
		the_repository->index_file =
			(எழுத்து *)get_lock_file_path(&index_lock);
		old_index_env = xstrdup_or_null(getenv(INDEX_ENVIRONMENT));
		setenv(INDEX_ENVIRONMENT, the_repository->index_file, 1);

		எனில் (interactive_add(the_repository, argv, prefix, patch_interactive, &interactive_opts) != 0)
			die(_("interactive add failed"));

		the_repository->index_file = old_repo_index_file;
		எனில் (old_index_env && *old_index_env)
			setenv(INDEX_ENVIRONMENT, old_index_env, 1);
		இல்லையெனில்
			unsetenv(INDEX_ENVIRONMENT);
		FREE_AND_NULL(old_index_env);

		discard_index(the_repository->index);
		read_index_from(the_repository->index, get_lock_file_path(&index_lock),
				repo_get_git_dir(the_repository));
		எனில் (cache_மரம்_update(the_repository->index, WRITE_மரம்_SILENT) == 0) {
			எனில் (reopen_lock_file(&index_lock) < 0)
				die(_("unable to write index file"));
			எனில் (write_locked_index(the_repository->index, &index_lock, 0))
				die(_("unable to update temporary index"));
		} இல்லையெனில்
			warning(_("Failed to update main cache tree"));

		commit_style = COMMIT_NORMAL;
		ret = get_lock_file_path(&index_lock);
		செல் out;
	} இல்லையெனில் {
		எனில் (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--interactive/--patch");
		எனில் (interactive_opts.interhunkcontext != -1)
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
	எனில் (all || (also && pathspec.nr)) {
		எழுத்து *ps_matched = xcalloc(pathspec.nr, 1);
		repo_hold_locked_index(the_repository, &index_lock,
				       LOCK_DIE_ON_ERROR);
		add_files_to_cache(the_repository, also ? prefix : வெற்று,
				   &pathspec, ps_matched, 0, 0, 0 );
		எனில் (!all && report_path_error(ps_matched, &pathspec))
			exit(128);

		refresh_cache_or_die(refresh_flags);
		cache_மரம்_update(the_repository->index, WRITE_மரம்_SILENT);
		எனில் (write_locked_index(the_repository->index, &index_lock, 0))
			die(_("unable to write new index file"));
		commit_style = COMMIT_NORMAL;
		ret = get_lock_file_path(&index_lock);
		விடுவி(ps_matched);
		செல் out;
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
	எனில் (!only && !pathspec.nr) {
		repo_hold_locked_index(the_repository, &index_lock,
				       LOCK_DIE_ON_ERROR);
		refresh_cache_or_die(refresh_flags);
		எனில் (the_repository->index->cache_changed
		    || !cache_மரம்_fully_valid(the_repository->index))
			cache_மரம்_update(the_repository->index, WRITE_மரம்_SILENT);
		எனில் (write_locked_index(the_repository->index, &index_lock,
				       COMMIT_LOCK | SKIP_IF_UNCHANGED))
			die(_("unable to write new index file"));
		commit_style = COMMIT_AS_IS;
		ret = repo_get_index_file(the_repository);
		செல் out;
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

	எனில் (whence != FROM_COMMIT) {
		எனில் (whence == FROM_MERGE)
			die(_("cannot do a partial commit during a merge."));
		இல்லையெனில் எனில் (is_from_cherry_pick(whence))
			die(_("cannot do a partial commit during a cherry-pick."));
		இல்லையெனில் எனில் (is_from_rebase(whence))
			die(_("cannot do a partial commit during a rebase."));
	}

	எனில் (list_paths(&partial, !current_head ? வெற்று : "HEAD", &pathspec))
		exit(1);

	discard_index(the_repository->index);
	எனில் (repo_read_index(the_repository) < 0)
		die(_("cannot read the index"));

	repo_hold_locked_index(the_repository, &index_lock, LOCK_DIE_ON_ERROR);
	add_remove_files(&partial);
	refresh_index(the_repository->index, REFRESH_QUIET, வெற்று, வெற்று, வெற்று);
	cache_மரம்_update(the_repository->index, WRITE_மரம்_SILENT);
	எனில் (write_locked_index(the_repository->index, &index_lock, 0))
		die(_("unable to write new index file"));

	path = repo_git_path(the_repository, "next-index-%"PRIuMAX,
			     (uintmax_t) getpid());
	hold_lock_file_for_update(&false_lock, path,
				  LOCK_DIE_ON_ERROR);

	create_base_index(current_head);
	add_remove_files(&partial);
	refresh_index(the_repository->index, REFRESH_QUIET, வெற்று, வெற்று, வெற்று);

	எனில் (write_locked_index(the_repository->index, &false_lock, 0))
		die(_("unable to write temporary index file"));

	discard_index(the_repository->index);
	ret = get_lock_file_path(&false_lock);
	read_index_from(the_repository->index, ret, repo_get_git_dir(the_repository));
out:
	string_list_clear(&partial, 0);
	clear_pathspec(&pathspec);
	விடுவி(path);
	திரும்பு ret;
}

நிலையான முழு run_status(FILE *fp, மாறிலி எழுத்து *index_file, மாறிலி எழுத்து *prefix, முழு nowarn,
		      கட்டமைப்பு wt_status *s)
{
	கட்டமைப்பு object_id oid;

	எனில் (s->relative_paths)
		s->prefix = prefix;

	எனில் (amend) {
		s->amend = 1;
		s->reference = "HEAD^1";
	}
	s->verbose = verbose;
	s->index_file = index_file;
	s->fp = fp;
	s->nowarn = nowarn;
	s->is_initial = repo_get_oid(the_repository, s->reference, &oid) ? 1 : 0;
	எனில் (!s->is_initial)
		oidcpy(&s->oid_commit, &oid);
	s->status_format = status_format;
	s->ignore_submodule_arg = ignore_submodule_arg;

	wt_status_collect(s);
	wt_status_print(s);
	wt_status_collect_free_buffers(s);

	திரும்பு s->committable;
}

நிலையான முழு is_a_merge(மாறிலி கட்டமைப்பு commit *current_head)
{
	திரும்பு !!(current_head->parents && current_head->parents->next);
}

நிலையான வெறுமை assert_split_ident(கட்டமைப்பு ident_split *id, மாறிலி கட்டமைப்பு strbuf *buf)
{
	எனில் (split_ident_வரி(id, buf->buf, buf->len) || !id->date_begin)
		BUG("unable to parse our own ident: %s", buf->buf);
}

நிலையான வெறுமை export_one(மாறிலி எழுத்து *var, மாறிலி எழுத்து *s, மாறிலி எழுத்து *e, முழு hack)
{
	கட்டமைப்பு strbuf buf = STRBUF_INIT;
	எனில் (hack)
		strbuf_addch(&buf, hack);
	strbuf_add(&buf, s, e - s);
	setenv(var, buf.buf, 1);
	strbuf_release(&buf);
}

நிலையான முழு parse_force_date(மாறிலி எழுத்து *in, கட்டமைப்பு strbuf *out)
{
	strbuf_addch(out, '@');

	எனில் (parse_date(in, out) < 0) {
		முழு errors = 0;
		unsigned long t = approxidate_careful(in, &errors);
		எனில் (errors)
			திரும்பு -1;
		strbuf_addf(out, "%lu", t);
	}

	திரும்பு 0;
}

நிலையான வெறுமை set_ident_var(எழுத்து **buf, எழுத்து *val)
{
	விடுவி(*buf);
	*buf = val;
}

நிலையான வெறுமை determine_author_info(கட்டமைப்பு strbuf *author_ident)
{
	எழுத்து *name, *email, *date;
	கட்டமைப்பு ident_split author;

	name = xstrdup_or_null(getenv("GIT_AUTHOR_NAME"));
	email = xstrdup_or_null(getenv("GIT_AUTHOR_EMAIL"));
	date = xstrdup_or_null(getenv("GIT_AUTHOR_DATE"));

	எனில் (author_message) {
		கட்டமைப்பு ident_split ident;
		size_t len;
		மாறிலி எழுத்து *a;

		a = find_commit_header(author_message_buffer, "author", &len);
		எனில் (!a)
			die(_("commit '%s' lacks author header"), author_message);
		எனில் (split_ident_வரி(&ident, a, len) < 0)
			die(_("commit '%s' has malformed author line"), author_message);

		set_ident_var(&name, xmemdupz(ident.name_begin, ident.name_end - ident.name_begin));
		set_ident_var(&email, xmemdupz(ident.mail_begin, ident.mail_end - ident.mail_begin));

		எனில் (ident.date_begin) {
			கட்டமைப்பு strbuf date_buf = STRBUF_INIT;
			strbuf_addch(&date_buf, '@');
			strbuf_add(&date_buf, ident.date_begin, ident.date_end - ident.date_begin);
			strbuf_addch(&date_buf, ' ');
			strbuf_add(&date_buf, ident.tz_begin, ident.tz_end - ident.tz_begin);
			set_ident_var(&date, strbuf_detach(&date_buf, வெற்று));
		}
	}

	எனில் (force_author) {
		கட்டமைப்பு ident_split ident;

		எனில் (split_ident_வரி(&ident, force_author, strlen(force_author)) < 0)
			die(_("malformed --author parameter"));
		set_ident_var(&name, xmemdupz(ident.name_begin, ident.name_end - ident.name_begin));
		set_ident_var(&email, xmemdupz(ident.mail_begin, ident.mail_end - ident.mail_begin));
	}

	எனில் (force_date) {
		கட்டமைப்பு strbuf date_buf = STRBUF_INIT;
		எனில் (parse_force_date(force_date, &date_buf))
			die(_("invalid date format: %s"), force_date);
		set_ident_var(&date, strbuf_detach(&date_buf, வெற்று));
	}

	strbuf_addstr(author_ident, fmt_ident(name, email, WANT_AUTHOR_IDENT, date,
				IDENT_STRICT));
	assert_split_ident(&author, author_ident);
	export_one("GIT_AUTHOR_NAME", author.name_begin, author.name_end, 0);
	export_one("GIT_AUTHOR_EMAIL", author.mail_begin, author.mail_end, 0);
	export_one("GIT_AUTHOR_DATE", author.date_begin, author.tz_end, '@');
	விடுவி(name);
	விடுவி(email);
	விடுவி(date);
}

நிலையான முழு author_date_is_interesting(வெறுமை)
{
	திரும்பு author_message || force_date;
}

#ifndef WITH_BREAKING_CHANGES
நிலையான வெறுமை adjust_comment_வரி_char(மாறிலி கட்டமைப்பு strbuf *sb)
{
	எழுத்து candidates[] = "#;@!$%^&|:";
	எழுத்து *candidate;
	மாறிலி எழுத்து *p;
	size_t cutoff;

	/* Ignore comment chars in trailing comments (e.g., Conflicts:) */
	cutoff = sb->len - ignored_log_message_bytes(sb->buf, sb->len);

	எனில் (!memchr(sb->buf, candidates[0], sb->len)) {
		விடுவி(comment_வரி_str_to_free);
		comment_வரி_str = comment_வரி_str_to_free =
			xstrfmt("%c", candidates[0]);
		திரும்பு;
	}

	p = sb->buf;
	candidate = strchr(candidates, *p);
	எனில் (candidate)
		*candidate = ' ';
	க்கு (p = sb->buf; p + 1 < sb->buf + cutoff; p++) {
		எனில் ((p[0] == '\n' || p[0] == '\r') && p[1]) {
			candidate = strchr(candidates, p[1]);
			எனில் (candidate)
				*candidate = ' ';
		}
	}

	க்கு (p = candidates; *p == ' '; p++)
		;
	எனில் (!*p)
		die(_("unable to select a comment character that is not used\n"
		      "in the current commit message"));
	விடுவி(comment_வரி_str_to_free);
	comment_வரி_str = comment_வரி_str_to_free = xstrfmt("%c", *p);
}
#endif /* !WITH_BREAKING_CHANGES */

நிலையான வெறுமை prepare_amend_commit(கட்டமைப்பு commit *commit, கட்டமைப்பு strbuf *sb,
				கட்டமைப்பு pretty_print_context *ctx)
{
	மாறிலி எழுத்து *buffer, *subject, *fmt;

	buffer = repo_get_commit_buffer(the_repository, commit, வெற்று);
	find_commit_subject(buffer, &subject);
	/*
	 * If we amend the 'amend!' commit then we don't want to
	 * duplicate the subject line.
	 */
	fmt = starts_with(subject, "amend!") ? "%b" : "%B";
	repo_format_commit_message(the_repository, commit, fmt, sb, ctx);
	repo_unuse_commit_buffer(the_repository, commit, buffer);
}

நிலையான வெறுமை change_data_free(வெறுமை *util, மாறிலி எழுத்து *str UNUSED)
{
	கட்டமைப்பு wt_status_change_data *d = util;
	விடுவி(d->rename_source);
	விடுவி(d);
}

நிலையான முழு prepare_to_commit(மாறிலி எழுத்து *index_file, மாறிலி எழுத்து *prefix,
			     கட்டமைப்பு commit *current_head,
			     கட்டமைப்பு wt_status *s,
			     கட்டமைப்பு strbuf *author_ident)
{
	கட்டமைப்பு stat statbuf;
	கட்டமைப்பு strbuf committer_ident = STRBUF_INIT;
	முழு committable;
	கட்டமைப்பு strbuf sb = STRBUF_INIT;
	மாறிலி எழுத்து *hook_arg1 = வெற்று;
	மாறிலி எழுத்து *hook_arg2 = வெற்று;
	முழு clean_message_contents = (cleanup_mode != COMMIT_செய்தி_CLEANUP_NONE);
	முழு old_display_comment_prefix;
	முழு invoked_hook;

	/* This checks and barfs if author is badly specified */
	determine_author_info(author_ident);

	எனில் (!no_verify && run_commit_hook(use_editor, index_file, &invoked_hook,
					  "pre-commit", வெற்று))
		திரும்பு 0;

	எனில் (squash_message) {
		/*
		 * Insert the proper subject line before other commit
		 * message options add their content.
		 */
		எனில் (use_message && !strcmp(use_message, squash_message))
			strbuf_addstr(&sb, "squash! ");
		இல்லையெனில் {
			கட்டமைப்பு pretty_print_context ctx = {0};
			கட்டமைப்பு commit *c;
			c = lookup_commit_reference_by_name(squash_message);
			எனில் (!c)
				die(_("could not lookup commit '%s'"), squash_message);
			ctx.output_encoding = get_commit_output_encoding();
			repo_format_commit_message(the_repository, c,
						   "squash! %s\n\n", &sb,
						   &ctx);
		}
	}

	எனில் (have_option_m && !fixup_message) {
		strbuf_addbuf(&sb, &message);
		hook_arg1 = "message";
	} இல்லையெனில் எனில் (logfile && !strcmp(logfile, "-")) {
		எனில் (isatty(0))
			fprintf(stderr, _("(reading log message from standard input)\n"));
		எனில் (strbuf_read(&sb, 0, 0) < 0)
			die_errno(_("could not read log from standard input"));
		hook_arg1 = "message";
	} இல்லையெனில் எனில் (logfile) {
		எனில் (strbuf_read_file(&sb, logfile, 0) < 0)
			die_errno(_("could not read log file '%s'"),
				  logfile);
		hook_arg1 = "message";
	} இல்லையெனில் எனில் (use_message) {
		மாறிலி எழுத்து *buffer;
		buffer = strstr(use_message_buffer, "\n\n");
		எனில் (buffer)
			strbuf_addstr(&sb, skip_blank_வரிகள்(buffer + 2));
		hook_arg1 = "commit";
		hook_arg2 = use_message;
	} இல்லையெனில் எனில் (fixup_message) {
		கட்டமைப்பு pretty_print_context ctx = {0};
		கட்டமைப்பு commit *commit;
		எழுத்து *fmt;
		commit = lookup_commit_reference_by_name(fixup_commit);
		எனில் (!commit)
			die(_("could not lookup commit '%s'"), fixup_commit);
		ctx.output_encoding = get_commit_output_encoding();
		fmt = xstrfmt("%s! %%s\n\n", fixup_prefix);
		repo_format_commit_message(the_repository, commit, fmt, &sb,
					   &ctx);
		விடுவி(fmt);
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
		எனில் (have_option_m && !strcmp(fixup_prefix, "fixup"))
			strbuf_addbuf(&sb, &message);

		எனில் (!strcmp(fixup_prefix, "amend")) {
			எனில் (have_option_m)
				die(_("options '%s' and '%s:%s' cannot be used together"), "-m", "--fixup", fixup_message);
			prepare_amend_commit(commit, &sb, &ctx);
		}
	} இல்லையெனில் எனில் (!stat(git_path_merge_செய்தி(the_repository), &statbuf)) {
		size_t merge_செய்தி_start;

		/*
		 * prepend SQUASH_MSG here if it exists and a
		 * "merge --squash" was originally performed
		 */
		எனில் (!stat(git_path_squash_செய்தி(the_repository), &statbuf)) {
			எனில் (strbuf_read_file(&sb, git_path_squash_செய்தி(the_repository), 0) < 0)
				die_errno(_("could not read SQUASH_MSG"));
			hook_arg1 = "squash";
		} இல்லையெனில்
			hook_arg1 = "merge";

		merge_செய்தி_start = sb.len;
		எனில் (strbuf_read_file(&sb, git_path_merge_செய்தி(the_repository), 0) < 0)
			die_errno(_("could not read MERGE_MSG"));

		எனில் (cleanup_mode == COMMIT_செய்தி_CLEANUP_SCISSORS &&
		    wt_status_locate_end(sb.buf + merge_செய்தி_start,
					 sb.len - merge_செய்தி_start) <
				sb.len - merge_செய்தி_start)
			s->added_cut_வரி = 1;
	} இல்லையெனில் எனில் (!stat(git_path_squash_செய்தி(the_repository), &statbuf)) {
		எனில் (strbuf_read_file(&sb, git_path_squash_செய்தி(the_repository), 0) < 0)
			die_errno(_("could not read SQUASH_MSG"));
		hook_arg1 = "squash";
	} இல்லையெனில் எனில் (template_file) {
		எனில் (strbuf_read_file(&sb, template_file, 0) < 0)
			die_errno(_("could not read '%s'"), template_file);
		hook_arg1 = "template";
		clean_message_contents = 0;
	}

	/*
	 * The remaining cases don't modify the template message, but
	 * just set the argument(s) to the prepare-commit-msg hook.
	 */
	இல்லையெனில் எனில் (whence == FROM_MERGE)
		hook_arg1 = "merge";
	இல்லையெனில் எனில் (is_from_cherry_pick(whence) || whence == FROM_REBASE_PICK) {
		hook_arg1 = "commit";
		hook_arg2 = "CHERRY_PICK_HEAD";
	}

	எனில் (squash_message) {
		/*
		 * If squash_commit was used for the commit subject,
		 * then we're possibly hijacking other commit log options.
		 * Reset the hook args to tell the real story.
		 */
		hook_arg1 = "message";
		hook_arg2 = "";
	}

	s->fp = fopen_for_writing(git_path_commit_editmsg());
	எனில் (!s->fp)
		die_errno(_("could not open '%s'"), git_path_commit_editmsg());

	/* Ignore status.displayCommentPrefix: we do need comments in COMMIT_EDITMSG. */
	old_display_comment_prefix = s->display_comment_prefix;
	s->display_comment_prefix = 1;

	/*
	 * Most hints are counter-productive when the commit has
	 * already started.
	 */
	s->hints = 0;

	எனில் (clean_message_contents)
		strbuf_stripspace(&sb, வெற்று);

	எனில் (signoff)
		append_signoff(&sb, ignored_log_message_bytes(sb.buf, sb.len), 0);

	எனில் (fwrite(sb.buf, 1, sb.len, s->fp) < sb.len)
		die_errno(_("could not write commit template"));

#ifndef WITH_BREAKING_CHANGES
	எனில் (auto_comment_வரி_char)
		adjust_comment_வரி_char(&sb);
#endif /* !WITH_BREAKING_CHANGES */
	strbuf_release(&sb);

	/* This checks if committer ident is explicitly given */
	strbuf_addstr(&committer_ident, git_committer_info(IDENT_STRICT));
	எனில் (use_editor && include_status) {
		முழு ident_shown = 0;
		எண்ணிக்கை git_colorbool saved_color_setting;
		கட்டமைப்பு ident_split ci, ai;
		மாறிலி எழுத்து *hint_cleanup_all = allow_empty_message ?
			_("Please enter the commit message for your changes."
			  " Lines starting\nwith '%s' will be ignored.\n") :
			_("Please enter the commit message for your changes."
			  " Lines starting\nwith '%s' will be ignored, and an empty"
			  " message aborts the commit.\n");
		மாறிலி எழுத்து *hint_cleanup_space = allow_empty_message ?
			_("Please enter the commit message for your changes."
			  " Lines starting\n"
			  "with '%s' will be kept; you may remove them"
			  " yourself if you want to.\n") :
			_("Please enter the commit message for your changes."
			  " Lines starting\n"
			  "with '%s' will be kept; you may remove them"
			  " yourself if you want to.\n"
			  "An empty message aborts the commit.\n");
		எனில் (whence != FROM_COMMIT) {
			எனில் (cleanup_mode == COMMIT_செய்தி_CLEANUP_SCISSORS)
				wt_status_add_cut_வரி(s);
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
		எனில் (cleanup_mode == COMMIT_செய்தி_CLEANUP_ALL)
			status_printf(s, GIT_COLOR_NORMAL, hint_cleanup_all, comment_வரி_str);
		இல்லையெனில் எனில் (cleanup_mode == COMMIT_செய்தி_CLEANUP_SCISSORS) {
			எனில் (whence == FROM_COMMIT)
				wt_status_add_cut_வரி(s);
		} இல்லையெனில் /* COMMIT_MSG_CLEANUP_SPACE, that is. */
			status_printf(s, GIT_COLOR_NORMAL, hint_cleanup_space, comment_வரி_str);

		/*
		 * These should never fail because they come from our own
		 * fmt_ident. They may fail the sane_ident test, but we know
		 * that the name and mail pointers will at least be valid,
		 * which is enough for our tests and printing here.
		 */
		assert_split_ident(&ai, author_ident);
		assert_split_ident(&ci, &committer_ident);

		எனில் (ident_cmp(&ai, &ci))
			status_printf_ln(s, GIT_COLOR_NORMAL,
				_("%s"
				"Author:    %.*s <%.*s>"),
				ident_shown++ ? "" : "\n",
				(முழு)(ai.name_end - ai.name_begin), ai.name_begin,
				(முழு)(ai.mail_end - ai.mail_begin), ai.mail_begin);

		எனில் (author_date_is_interesting())
			status_printf_ln(s, GIT_COLOR_NORMAL,
				_("%s"
				"Date:      %s"),
				ident_shown++ ? "" : "\n",
				show_ident_date(&ai, DATE_MODE(NORMAL)));

		எனில் (!committer_ident_sufficiently_given())
			status_printf_ln(s, GIT_COLOR_NORMAL,
				_("%s"
				"Committer: %.*s <%.*s>"),
				ident_shown++ ? "" : "\n",
				(முழு)(ci.name_end - ci.name_begin), ci.name_begin,
				(முழு)(ci.mail_end - ci.mail_begin), ci.mail_begin);

		status_printf_ln(s, GIT_COLOR_NORMAL, "%s", ""); /* Add new line for clarity */

		saved_color_setting = s->use_color;
		s->use_color = GIT_COLOR_NEVER;
		committable = run_status(s->fp, index_file, prefix, 1, s);
		s->use_color = saved_color_setting;
		string_list_clear_func(&s->change, change_data_free);
	} இல்லையெனில் {
		கட்டமைப்பு object_id oid;
		மாறிலி எழுத்து *parent = "HEAD";

		எனில் (!the_repository->index->initialized && repo_read_index(the_repository) < 0)
			die(_("Cannot read index"));

		எனில் (amend)
			parent = "HEAD^1";

		எனில் (repo_get_oid(the_repository, parent, &oid)) {
			முழு i, ita_nr = 0;

			/* TODO: audit for interaction with sparse-index. */
			ensure_full_index(the_repository->index);
			க்கு (i = 0; i < the_repository->index->cache_nr; i++)
				எனில் (ce_intent_to_add(the_repository->index->cache[i]))
					ita_nr++;
			committable = the_repository->index->cache_nr > ita_nr;
		} இல்லையெனில் {
			/*
			 * Unless the user did explicitly request a submodule
			 * ignore mode by passing a command line option we do
			 * not ignore any changed submodule SHA-1s when
			 * comparing index and parent, no matter what is
			 * configured. Otherwise we won't commit any
			 * submodules which were manually staged, which would
			 * be really confusing.
			 */
			கட்டமைப்பு diff_flags flags = DIFF_FLAGS_INIT;
			flags.override_submodule_config = 1;
			எனில் (ignore_submodule_arg &&
			    !strcmp(ignore_submodule_arg, "all"))
				flags.ignore_submodules = 1;
			committable = index_differs_from(the_repository,
							 parent, &flags, 1);
		}
	}
	strbuf_release(&committer_ident);

	fclose(s->fp);

	எனில் (trailer_args.nr) {
		எனில் (amend_file_with_trailers(git_path_commit_editmsg(), &trailer_args))
			die(_("unable to pass trailers to --trailers"));
		strvec_clear(&trailer_args);
	}

	/*
	 * Reject an attempt to record a non-merge empty commit without
	 * explicit --allow-empty. In the cherry-pick case, it may be
	 * empty due to conflict resolution, which the user should okay.
	 */
	எனில் (!committable && whence != FROM_MERGE && !allow_empty &&
	    !(amend && is_a_merge(current_head))) {
		s->hints = advice_enabled(ADVICE_STATUS_HINTS);
		s->display_comment_prefix = old_display_comment_prefix;
		run_status(stdout, index_file, prefix, 0, s);
		எனில் (amend)
			fputs(_(empty_amend_advice), stderr);
		இல்லையெனில் எனில் (is_from_cherry_pick(whence) ||
			 whence == FROM_REBASE_PICK) {
			fputs(_(empty_cherry_pick_advice), stderr);
			எனில் (whence == FROM_CHERRY_PICK_SINGLE)
				fputs(_(empty_cherry_pick_advice_single), stderr);
			இல்லையெனில் எனில் (whence == FROM_CHERRY_PICK_MULTI)
				fputs(_(empty_cherry_pick_advice_multi), stderr);
			இல்லையெனில்
				fputs(_(empty_rebase_pick_advice), stderr);
		}
		திரும்பு 0;
	}

	எனில் (!no_verify && invoked_hook) {
		/*
		 * Re-read the index as the pre-commit-commit hook was invoked
		 * and could have updated it. We must do this before we invoke
		 * the editor and after we invoke run_status above.
		 */
		discard_index(the_repository->index);
	}
	read_index_from(the_repository->index, index_file, repo_get_git_dir(the_repository));

	எனில் (cache_மரம்_update(the_repository->index, 0)) {
		error(_("Error building trees"));
		திரும்பு 0;
	}

	எனில் (run_commit_hook(use_editor, index_file, வெற்று, "prepare-commit-msg",
			    git_path_commit_editmsg(), hook_arg1, hook_arg2, வெற்று))
		திரும்பு 0;

	எனில் (use_editor) {
		கட்டமைப்பு strvec env = STRVEC_INIT;

		strvec_pushf(&env, "GIT_INDEX_FILE=%s", index_file);
		எனில் (launch_editor(git_path_commit_editmsg(), வெற்று, env.v)) {
			fprintf(stderr,
			_("Please supply the message using either -m or -F option.\n"));
			exit(1);
		}
		strvec_clear(&env);
	}

	எனில் (!no_verify &&
	    run_commit_hook(use_editor, index_file, வெற்று, "commit-msg",
			    git_path_commit_editmsg(), வெற்று)) {
		திரும்பு 0;
	}

	திரும்பு 1;
}

நிலையான மாறிலி எழுத்து *find_author_by_nickname(மாறிலி எழுத்து *name)
{
	கட்டமைப்பு rev_info revs;
	கட்டமைப்பு commit *commit;
	கட்டமைப்பு strbuf buf = STRBUF_INIT;
	மாறிலி எழுத்து *av[20];
	முழு ac = 0;

	repo_init_revisions(the_repository, &revs, வெற்று);
	strbuf_addf(&buf, "--author=%s", name);
	av[++ac] = "--all";
	av[++ac] = "-i";
	av[++ac] = buf.buf;
	av[++ac] = வெற்று;
	setup_revisions(ac, av, &revs, வெற்று);
	revs.mailmap = xmalloc(அளவு(கட்டமைப்பு string_list));
	string_list_init_nodup(revs.mailmap);
	read_mailmap(the_repository, revs.mailmap);

	எனில் (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	commit = get_revision(&revs);
	எனில் (commit) {
		கட்டமைப்பு pretty_print_context ctx = {0};
		ctx.date_mode.type = DATE_NORMAL;
		strbuf_release(&buf);
		repo_format_commit_message(the_repository, commit,
					   "%aN <%aE>", &buf, &ctx);
		release_revisions(&revs);
		திரும்பு strbuf_detach(&buf, வெற்று);
	}
	die(_("--author '%s' is not 'Name <email>' and matches no existing author"), name);
}

நிலையான வெறுமை handle_ignored_arg(கட்டமைப்பு wt_status *s)
{
	எனில் (!ignored_arg)
		; /* default already initialized */
	இல்லையெனில் எனில் (!strcmp(ignored_arg, "traditional"))
		s->show_ignored_mode = SHOW_TRADITIONAL_IGNORED;
	இல்லையெனில் எனில் (!strcmp(ignored_arg, "no"))
		s->show_ignored_mode = SHOW_NO_IGNORED;
	இல்லையெனில் எனில் (!strcmp(ignored_arg, "matching"))
		s->show_ignored_mode = SHOW_MATCHING_IGNORED;
	இல்லையெனில்
		die(_("Invalid ignored mode '%s'"), ignored_arg);
}

நிலையான எண்ணிக்கை untracked_status_type parse_untracked_setting_name(மாறிலி எழுத்து *u)
{
	/*
	 * Please update $__git_untracked_file_modes in
	 * git-completion.bash when you add new options
	 */
	தேர்வு (git_parse_maybe_bool(u)) {
	நிலை 0:
		u = "no";
		நிறுத்து;
	நிலை 1:
		u = "normal";
		நிறுத்து;
	இயல்பு:
		நிறுத்து;
	}

	எனில் (!strcmp(u, "no"))
		திரும்பு SHOW_NO_UNTRACKED_FILES;
	இல்லையெனில் எனில் (!strcmp(u, "normal"))
		திரும்பு SHOW_NORMAL_UNTRACKED_FILES;
	இல்லையெனில் எனில் (!strcmp(u, "all"))
		திரும்பு SHOW_ALL_UNTRACKED_FILES;
	இல்லையெனில்
		திரும்பு SHOW_UNTRACKED_FILES_ERROR;
}

நிலையான வெறுமை handle_untracked_files_arg(கட்டமைப்பு wt_status *s)
{
	எண்ணிக்கை untracked_status_type u;

	எனில் (!untracked_files_arg)
		திரும்பு; /* default already initialized */

	u = parse_untracked_setting_name(untracked_files_arg);
	எனில் (u == SHOW_UNTRACKED_FILES_ERROR)
		die(_("Invalid untracked files mode '%s'"),
		    untracked_files_arg);
	s->show_untracked_files = u;
}

நிலையான மாறிலி எழுத்து *read_commit_message(மாறிலி எழுத்து *name)
{
	மாறிலி எழுத்து *out_enc;
	கட்டமைப்பு commit *commit;

	commit = lookup_commit_reference_by_name(name);
	எனில் (!commit)
		die(_("could not lookup commit '%s'"), name);
	out_enc = get_commit_output_encoding();
	திரும்பு repo_logmsg_reencode(the_repository, commit, வெற்று, out_enc);
}

/*
 * Enumerate what needs to be propagated when --porcelain
 * is not in effect here.
 */
நிலையான கட்டமைப்பு status_deferred_config {
	எண்ணிக்கை wt_status_format status_format;
	முழு show_branch;
	எண்ணிக்கை ahead_behind_flags ahead_behind;
} status_deferred_config = {
	STATUS_FORMAT_UNSPECIFIED,
	-1, /* unspecified */
	AHEAD_BEHIND_UNSPECIFIED,
};

நிலையான வெறுமை finalize_deferred_config(கட்டமைப்பு wt_status *s)
{
	முழு use_deferred_config = (status_format != STATUS_FORMAT_PORCELAIN &&
				   status_format != STATUS_FORMAT_PORCELAIN_V2 &&
				   !s->null_termination);

	எனில் (s->null_termination) {
		எனில் (status_format == STATUS_FORMAT_NONE ||
		    status_format == STATUS_FORMAT_UNSPECIFIED)
			status_format = STATUS_FORMAT_PORCELAIN;
		இல்லையெனில் எனில் (status_format == STATUS_FORMAT_LONG)
			die(_("options '%s' and '%s' cannot be used together"), "--long", "-z");
	}

	எனில் (use_deferred_config && status_format == STATUS_FORMAT_UNSPECIFIED)
		status_format = status_deferred_config.status_format;
	எனில் (status_format == STATUS_FORMAT_UNSPECIFIED)
		status_format = STATUS_FORMAT_NONE;

	எனில் (use_deferred_config && s->show_branch < 0)
		s->show_branch = status_deferred_config.show_branch;
	எனில் (s->show_branch < 0)
		s->show_branch = 0;

	/*
	 * If the user did not give a "--[no]-ahead-behind" command
	 * line argument *AND* we will print in a human-readable format
	 * (short, long etc.) then we inherit from the status.aheadbehind
	 * config setting.  In all other cases (and porcelain V[12] formats
	 * in particular), we inherit _FULL for backwards compatibility.
	 */
	எனில் (use_deferred_config &&
	    s->ahead_behind_flags == AHEAD_BEHIND_UNSPECIFIED)
		s->ahead_behind_flags = status_deferred_config.ahead_behind;

	எனில் (s->ahead_behind_flags == AHEAD_BEHIND_UNSPECIFIED)
		s->ahead_behind_flags = AHEAD_BEHIND_FULL;
}

நிலையான வெறுமை check_fixup_reword_options(முழு argc, மாறிலி எழுத்து *argv[]) {
	எனில் (whence != FROM_COMMIT) {
		எனில் (whence == FROM_MERGE)
			die(_("You are in the middle of a merge -- cannot reword."));
		இல்லையெனில் எனில் (is_from_cherry_pick(whence))
			die(_("You are in the middle of a cherry-pick -- cannot reword."));
	}
	எனில் (argc)
		die(_("reword option of '%s' and path '%s' cannot be used together"), "--fixup", *argv);
	எனில் (patch_interactive || interactive || all || also || only)
		die(_("reword option of '%s' and '%s' cannot be used together"),
			"--fixup", "--patch/--interactive/--all/--include/--only");
}

நிலையான முழு parse_and_validate_options(முழு argc, மாறிலி எழுத்து *argv[],
				      மாறிலி கட்டமைப்பு option *options,
				      மாறிலி எழுத்து * மாறிலி usage[],
				      மாறிலி எழுத்து *prefix,
				      கட்டமைப்பு commit *current_head,
				      கட்டமைப்பு wt_status *s)
{
	argc = parse_options(argc, argv, prefix, options, usage, 0);
	finalize_deferred_config(s);

	எனில் (force_author && !strchr(force_author, '>'))
		force_author = find_author_by_nickname(force_author);

	எனில் (force_author && renew_authorship)
		die(_("options '%s' and '%s' cannot be used together"), "--reset-author", "--author");

	எனில் (logfile || have_option_m || use_message)
		use_editor = 0;

	/* Sanity check options */
	எனில் (amend && !current_head)
		die(_("You have nothing to amend."));
	எனில் (amend && whence != FROM_COMMIT) {
		எனில் (whence == FROM_MERGE)
			die(_("You are in the middle of a merge -- cannot amend."));
		இல்லையெனில் எனில் (is_from_cherry_pick(whence))
			die(_("You are in the middle of a cherry-pick -- cannot amend."));
		இல்லையெனில் எனில் (whence == FROM_REBASE_PICK)
			die(_("You are in the middle of a rebase -- cannot amend."));
	}
	எனில் (fixup_message && squash_message)
		die(_("options '%s' and '%s' cannot be used together"), "--squash", "--fixup");
	die_for_incompatible_opt4(!!use_message, "-C",
				  !!edit_message, "-c",
				  !!logfile, "-F",
				  !!fixup_message, "--fixup");
	die_for_incompatible_opt4(have_option_m, "-m",
				  !!edit_message, "-c",
				  !!use_message, "-C",
				  !!logfile, "-F");
	எனில் (use_message || edit_message || logfile ||fixup_message || have_option_m)
		FREE_AND_NULL(template_file);
	எனில் (edit_message)
		use_message = edit_message;
	எனில் (amend && !use_message && !fixup_message)
		use_message = "HEAD";
	எனில் (!use_message && !is_from_cherry_pick(whence) &&
	    !is_from_rebase(whence) && renew_authorship)
		die(_("--reset-author can be used only with -C, -c or --amend."));
	எனில் (use_message) {
		use_message_buffer = read_commit_message(use_message);
		எனில் (!renew_authorship) {
			author_message = use_message;
			author_message_buffer = use_message_buffer;
		}
	}
	எனில் ((is_from_cherry_pick(whence) || whence == FROM_REBASE_PICK) &&
	    !renew_authorship) {
		author_message = "CHERRY_PICK_HEAD";
		author_message_buffer = read_commit_message(author_message);
	}

	எனில் (patch_interactive)
		interactive = 1;

	die_for_incompatible_opt4(also, "-i/--include",
				  only, "-o/--only",
				  all, "-a/--all",
				  interactive, "--interactive/-p/--patch");
	எனில் (fixup_message) {
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
		எழுத்து *p = fixup_message;
		போது (isalpha(*p))
			p++;
		எனில் (p > fixup_message && *p == ':') {
			*p = '\0';
			fixup_commit = p + 1;
			எனில் (!strcmp("amend", fixup_message) ||
			    !strcmp("reword", fixup_message)) {
				fixup_prefix = "amend";
				allow_empty = 1;
				எனில் (*fixup_message == 'r') {
					check_fixup_reword_options(argc, argv);
					only = 1;
				}
			} இல்லையெனில் {
				die(_("unknown option: --fixup=%s:%s"), fixup_message, fixup_commit);
			}
		} இல்லையெனில் {
			fixup_commit = fixup_message;
			fixup_prefix = "fixup";
			use_editor = 0;
		}
	}

	எனில் (0 <= edit_flag)
		use_editor = edit_flag;

	handle_untracked_files_arg(s);

	எனில் (all && argc > 0)
		die(_("paths '%s ...' with -a does not make sense"),
		    argv[0]);

	எனில் (status_format != STATUS_FORMAT_NONE)
		dry_run = 1;

	திரும்பு argc;
}

நிலையான முழு dry_run_commit(மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
			  மாறிலி கட்டமைப்பு commit *current_head, கட்டமைப்பு wt_status *s)
{
	முழு committable;
	மாறிலி எழுத்து *index_file;

	index_file = prepare_index(argv, prefix, current_head, 1);
	committable = run_status(stdout, index_file, prefix, 0, s);
	rollback_index_files();

	திரும்பு committable ? 0 : 1;
}

define_list_config_array_extra(color_status_slots, {"added"});

நிலையான முழு parse_status_slot(மாறிலி எழுத்து *slot)
{
	எனில் (!strcasecmp(slot, "added"))
		திரும்பு WT_STATUS_UPDATED;

	திரும்பு LOOKUP_CONFIG(color_status_slots, slot);
}

நிலையான முழு git_status_config(மாறிலி எழுத்து *k, மாறிலி எழுத்து *v,
			     மாறிலி கட்டமைப்பு config_context *ctx, வெறுமை *cb)
{
	கட்டமைப்பு wt_status *s = cb;
	மாறிலி எழுத்து *slot_name;

	எனில் (starts_with(k, "column."))
		திரும்பு git_column_config(k, v, "status", &s->colopts);
	எனில் (!strcmp(k, "status.submodulesummary")) {
		முழு is_bool;
		s->submodule_summary = git_config_bool_or_int(k, v, ctx->kvi,
							      &is_bool);
		எனில் (is_bool && s->submodule_summary)
			s->submodule_summary = -1;
		திரும்பு 0;
	}
	எனில் (!strcmp(k, "status.short")) {
		எனில் (git_config_bool(k, v))
			status_deferred_config.status_format = STATUS_FORMAT_SHORT;
		இல்லையெனில்
			status_deferred_config.status_format = STATUS_FORMAT_NONE;
		திரும்பு 0;
	}
	எனில் (!strcmp(k, "status.branch")) {
		status_deferred_config.show_branch = git_config_bool(k, v);
		திரும்பு 0;
	}
	எனில் (!strcmp(k, "status.aheadbehind")) {
		status_deferred_config.ahead_behind = git_config_bool(k, v);
		திரும்பு 0;
	}
	எனில் (!strcmp(k, "status.showstash")) {
		s->show_stash = git_config_bool(k, v);
		திரும்பு 0;
	}
	எனில் (!strcmp(k, "status.color") || !strcmp(k, "color.status")) {
		s->use_color = git_config_colorbool(k, v);
		திரும்பு 0;
	}
	எனில் (!strcmp(k, "status.displaycommentprefix")) {
		s->display_comment_prefix = git_config_bool(k, v);
		திரும்பு 0;
	}
	எனில் (skip_prefix(k, "status.color.", &slot_name) ||
	    skip_prefix(k, "color.status.", &slot_name)) {
		முழு slot = parse_status_slot(slot_name);
		எனில் (slot < 0)
			திரும்பு 0;
		எனில் (!v)
			திரும்பு config_error_nonbool(k);
		திரும்பு color_parse(v, s->color_palette[slot]);
	}
	எனில் (!strcmp(k, "status.relativepaths")) {
		s->relative_paths = git_config_bool(k, v);
		திரும்பு 0;
	}
	எனில் (!strcmp(k, "status.showuntrackedfiles")) {
		எண்ணிக்கை untracked_status_type u;

		u = parse_untracked_setting_name(v);
		எனில் (u == SHOW_UNTRACKED_FILES_ERROR)
			திரும்பு error(_("Invalid untracked files mode '%s'"), v);
		s->show_untracked_files = u;
		திரும்பு 0;
	}
	எனில் (!strcmp(k, "diff.renamelimit")) {
		எனில் (s->rename_limit == -1)
			s->rename_limit = git_config_int(k, v, ctx->kvi);
		திரும்பு 0;
	}
	எனில் (!strcmp(k, "status.renamelimit")) {
		s->rename_limit = git_config_int(k, v, ctx->kvi);
		திரும்பு 0;
	}
	எனில் (!strcmp(k, "diff.renames")) {
		எனில் (s->detect_rename == -1)
			s->detect_rename = git_config_rename(k, v);
		திரும்பு 0;
	}
	எனில் (!strcmp(k, "status.renames")) {
		s->detect_rename = git_config_rename(k, v);
		திரும்பு 0;
	}
	திரும்பு git_diff_ui_config(k, v, ctx, வெற்று);
}

முழு cmd_status(முழு argc,
மாறிலி எழுத்து **argv,
மாறிலி எழுத்து *prefix,
கட்டமைப்பு repository *repo UNUSED)
{
	நிலையான முழு no_renames = -1;
	நிலையான மாறிலி எழுத்து *rename_score_arg = (மாறிலி எழுத்து *)-1;
	நிலையான கட்டமைப்பு wt_status s;
	unsigned முழு progress_flag = 0;
	முழு fd;
	கட்டமைப்பு object_id oid;
	நிலையான கட்டமைப்பு option builtin_status_options[] = {
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
			.மதிப்பு = &untracked_files_arg,
			.argh = N_("mode"),
			.help = N_("show untracked files, optional modes: all, normal, no. (Default: all)"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t)"all",
		},
		{
			.type = OPTION_STRING,
			.long_name = "ignored",
			.மதிப்பு = &ignored_arg,
			.argh = N_("mode"),
			.help = N_("show ignored files, optional modes: traditional, matching, no. (Default: traditional)"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t)"traditional",
		},
		{
			.type = OPTION_STRING,
			.long_name = "ignore-submodules",
			.மதிப்பு = &ignore_submodule_arg,
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

	எனில் (s.show_ignored_mode == SHOW_MATCHING_IGNORED &&
	    s.show_untracked_files == SHOW_NO_UNTRACKED_FILES)
		die(_("Unsupported combination of ignored and untracked-files arguments"));

	parse_pathspec(&s.pathspec, 0,
		       PATHSPEC_PREFER_FULL,
		       prefix, argv);

	எனில் (status_format != STATUS_FORMAT_PORCELAIN &&
	    status_format != STATUS_FORMAT_PORCELAIN_V2)
		progress_flag = REFRESH_PROGRESS;
	repo_read_index(the_repository);
	refresh_index(the_repository->index,
		      REFRESH_QUIET|REFRESH_UNMERGED|progress_flag,
		      &s.pathspec, வெற்று, வெற்று);

	எனில் (use_optional_locks())
		fd = repo_hold_locked_index(the_repository, &index_lock, 0);
	இல்லையெனில்
		fd = -1;

	s.is_initial = repo_get_oid(the_repository, s.reference, &oid) ? 1 : 0;
	எனில் (!s.is_initial)
		oidcpy(&s.oid_commit, &oid);

	s.ignore_submodule_arg = ignore_submodule_arg;
	s.status_format = status_format;
	s.verbose = verbose;
	எனில் (no_renames != -1)
		s.detect_rename = !no_renames;
	எனில் ((intptr_t)rename_score_arg != -1) {
		எனில் (s.detect_rename < DIFF_DETECT_RENAME)
			s.detect_rename = DIFF_DETECT_RENAME;
		எனில் (rename_score_arg)
			s.rename_score = parse_rename_score(&rename_score_arg);
	}

	wt_status_collect(&s);

	எனில் (0 <= fd)
		repo_update_index_if_able(the_repository, &index_lock);

	எனில் (s.relative_paths)
		s.prefix = prefix;

	wt_status_print(&s);
	wt_status_collect_free_buffers(&s);

	திரும்பு 0;
}

நிலையான முழு git_commit_config(மாறிலி எழுத்து *k, மாறிலி எழுத்து *v,
			     மாறிலி கட்டமைப்பு config_context *ctx, வெறுமை *cb)
{
	கட்டமைப்பு wt_status *s = cb;

	எனில் (!strcmp(k, "commit.template"))
		திரும்பு git_config_pathname(&template_file, k, v);
	எனில் (!strcmp(k, "commit.status")) {
		include_status = git_config_bool(k, v);
		திரும்பு 0;
	}
	எனில் (!strcmp(k, "commit.cleanup")) {
		FREE_AND_NULL(cleanup_config);
		திரும்பு git_config_string(&cleanup_config, k, v);
	}
	எனில் (!strcmp(k, "commit.gpgsign")) {
		sign_commit = git_config_bool(k, v) ? "" : வெற்று;
		திரும்பு 0;
	}
	எனில் (!strcmp(k, "commit.verbose")) {
		முழு is_bool;
		config_commit_verbose = git_config_bool_or_int(k, v, ctx->kvi,
							       &is_bool);
		திரும்பு 0;
	}

	திரும்பு git_status_config(k, v, ctx, s);
}

முழு cmd_commit(முழு argc,
	       மாறிலி எழுத்து **argv,
	       மாறிலி எழுத்து *prefix,
	       கட்டமைப்பு repository *repo UNUSED)
{
	நிலையான கட்டமைப்பு wt_status s;
	நிலையான மாறிலி எழுத்து *cleanup_arg = வெற்று;
	நிலையான கட்டமைப்பு option builtin_commit_options[] = {
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
			.மதிப்பு = &sign_commit,
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
			.மதிப்பு = &untracked_files_arg,
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

	கட்டமைப்பு strbuf sb = STRBUF_INIT;
	கட்டமைப்பு strbuf author_ident = STRBUF_INIT;
	மாறிலி எழுத்து *index_file, *reflog_செய்தி;
	கட்டமைப்பு object_id oid;
	கட்டமைப்பு commit_list *parents = வெற்று;
	கட்டமைப்பு stat statbuf;
	கட்டமைப்பு commit *current_head = வெற்று;
	கட்டமைப்பு commit_extra_header *extra = வெற்று;
	கட்டமைப்பு strbuf err = STRBUF_INIT;
	முழு ret = 0;

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

	எனில் (repo_get_oid(the_repository, "HEAD", &oid))
		current_head = வெற்று;
	இல்லையெனில் {
		current_head = lookup_commit_or_die(&oid, "HEAD");
		எனில் (repo_parse_commit(the_repository, current_head))
			die(_("could not parse HEAD commit"));
	}
	verbose = -1; /* unspecified */
	argc = parse_and_validate_options(argc, argv, builtin_commit_options,
					  builtin_commit_usage,
					  prefix, current_head, &s);
	எனில் (trailer_args.nr)
		trailer_config_init();

	எனில் (verbose == -1)
		verbose = (config_commit_verbose < 0) ? 0 : config_commit_verbose;

	எனில் (cleanup_arg) {
		விடுவி(cleanup_config);
		cleanup_config = xstrdup(cleanup_arg);
	}
	cleanup_mode = get_cleanup_mode(cleanup_config, use_editor);

	எனில் (dry_run)
		திரும்பு dry_run_commit(argv, prefix, current_head, &s);
	index_file = prepare_index(argv, prefix, current_head, 0);

	/* Set up everything for writing the commit object.  This includes
	   running hooks, writing the trees, and interacting with the user.  */
	எனில் (!prepare_to_commit(index_file, prefix,
			       current_head, &s, &author_ident)) {
		ret = 1;
		rollback_index_files();
		செல் cleanup;
	}

	/* Determine parents */
	reflog_செய்தி = getenv("GIT_REFLOG_ACTION");
	எனில் (!current_head) {
		எனில் (!reflog_செய்தி)
			reflog_செய்தி = "commit (initial)";
	} இல்லையெனில் எனில் (amend) {
		எனில் (!reflog_செய்தி)
			reflog_செய்தி = "commit (amend)";
		parents = commit_list_copy(current_head->parents);
	} இல்லையெனில் எனில் (whence == FROM_MERGE) {
		கட்டமைப்பு strbuf m = STRBUF_INIT;
		FILE *fp;
		முழு allow_fast_forward = 1;
		கட்டமைப்பு commit_list **pptr = &parents;

		எனில் (!reflog_செய்தி)
			reflog_செய்தி = "commit (merge)";
		pptr = commit_list_append(current_head, pptr);
		fp = xfopen(git_path_merge_head(the_repository), "r");
		போது (strbuf_getline_lf(&m, fp) != EOF) {
			கட்டமைப்பு commit *parent;

			parent = get_merge_parent(m.buf);
			எனில் (!parent)
				die(_("Corrupt MERGE_HEAD file (%s)"), m.buf);
			pptr = commit_list_append(parent, pptr);
		}
		fclose(fp);
		strbuf_release(&m);
		எனில் (!stat(git_path_merge_mode(the_repository), &statbuf)) {
			எனில் (strbuf_read_file(&sb, git_path_merge_mode(the_repository), 0) < 0)
				die_errno(_("could not read MERGE_MODE"));
			எனில் (!strcmp(sb.buf, "no-ff"))
				allow_fast_forward = 0;
		}
		எனில் (allow_fast_forward)
			reduce_heads_replace(&parents);
	} இல்லையெனில் {
		எனில் (!reflog_செய்தி)
			reflog_செய்தி = is_from_cherry_pick(whence)
					? "commit (cherry-pick)"
					: is_from_rebase(whence)
					? "commit (rebase)"
					: "commit";
		commit_list_செருகு(current_head, &parents);
	}

	/* Finally, get the commit message */
	strbuf_reset(&sb);
	எனில் (strbuf_read_file(&sb, git_path_commit_editmsg(), 0) < 0) {
		முழு saved_errno = errno;
		rollback_index_files();
		die(_("could not read commit message: %s"), strerror(saved_errno));
	}

	cleanup_message(&sb, cleanup_mode, verbose);

	எனில் (message_is_empty(&sb, cleanup_mode) && !allow_empty_message) {
		rollback_index_files();
		fprintf(stderr, _("Aborting commit due to empty commit message.\n"));
		exit(1);
	}
	எனில் (template_untouched(&sb, template_file, cleanup_mode) && !allow_empty_message) {
		rollback_index_files();
		fprintf(stderr, _("Aborting commit; you did not edit the message.\n"));
		exit(1);
	}

	எனில் (fixup_message && starts_with(sb.buf, "amend! ") &&
	    !allow_empty_message) {
		கட்டமைப்பு strbuf body = STRBUF_INIT;
		size_t len = commit_subject_length(sb.buf);
		strbuf_addstr(&body, sb.buf + len);
		எனில் (message_is_empty(&body, cleanup_mode)) {
			rollback_index_files();
			fprintf(stderr, _("Aborting commit due to empty commit message body.\n"));
			exit(1);
		}
		strbuf_release(&body);
	}

	எனில் (amend) {
		மாறிலி எழுத்து *exclude_gpgsig[3] = { "gpgsig", "gpgsig-sha256", வெற்று };
		extra = read_commit_extra_headers(current_head, exclude_gpgsig);
	} இல்லையெனில் {
		கட்டமைப்பு commit_extra_header **tail = &extra;
		append_merge_tag_headers(parents, &tail);
	}

	எனில் (commit_மரம்_extended(sb.buf, sb.len, &the_repository->index->cache_மரம்->oid,
				 parents, &oid, author_ident.buf, வெற்று,
				 sign_commit, extra)) {
		rollback_index_files();
		die(_("failed to write commit object"));
	}

	எனில் (update_head_with_reflog(current_head, &oid, reflog_செய்தி, &sb,
				    &err)) {
		rollback_index_files();
		die("%s", err.buf);
	}

	sequencer_post_commit_cleanup(the_repository, 0);
	unlink(git_path_merge_head(the_repository));
	unlink(git_path_merge_செய்தி(the_repository));
	unlink(git_path_merge_mode(the_repository));
	unlink(git_path_squash_செய்தி(the_repository));

	எனில் (commit_index_files())
		die(_("repository has been updated, but unable to write\n"
		      "new index file. Check that disk is not full and quota is\n"
		      "not exceeded, and then \"git restore --staged :/\" to recover."));

	git_test_write_commit_graph_or_die(the_repository);

	repo_rerere(the_repository, 0);
	run_auto_maintenance(the_repository, quiet);
	run_commit_hook(use_editor, repo_get_index_file(the_repository),
			வெற்று, "post-commit", வெற்று);
	எனில் (amend && !no_post_rewrite) {
		commit_post_rewrite(the_repository, current_head, &oid);
	}
	எனில் (!quiet) {
		unsigned முழு flags = 0;

		எனில் (!current_head)
			flags |= SUMMARY_INITIAL_COMMIT;
		எனில் (author_date_is_interesting())
			flags |= SUMMARY_SHOW_AUTHOR_DATE;
		print_commit_summary(the_repository, prefix,
				     &oid, flags);
	}

	apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
			    வெற்று, வெற்று, வெற்று, வெற்று);

cleanup:
	free_commit_extra_headers(extra);
	commit_list_free(parents);
	strbuf_release(&author_ident);
	strbuf_release(&err);
	strbuf_release(&sb);
	விடுவி(logfile);
	விடுவி(template_file);
	திரும்பு ret;
}
