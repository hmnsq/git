/*
 * Builtin "git commit"
 *
 * Copyright (c) 2007 Kristian Høgsberg <krh@redhat.com>
 * Based on git-commit.sh by Junio C Hamano and Linus Torvalds
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#شامل "builtin.h"
#شامل "advice.h"
#شامل "config.h"
#شامل "lockfile.h"
#شامل "cache-tree.h"
#شامل "color.h"
#شامل "dir.h"
#شامل "editor.h"
#شامل "environment.h"
#شامل "diff.h"
#شامل "commit.h"
#شامل "add-interactive.h"
#شامل "gettext.h"
#شامل "revision.h"
#شامل "wt-status.h"
#شامل "run-command.h"
#شامل "strbuf.h"
#شامل "object-name.h"
#شامل "parse-options.h"
#شامل "path.h"
#شامل "preload-index.h"
#شامل "read-cache.h"
#شامل "repository.h"
#شامل "string-list.h"
#شامل "rerere.h"
#شامل "unpack-trees.h"
#شامل "column.h"
#شامل "sequencer.h"
#شامل "sparse-index.h"
#شامل "mailmap.h"
#شامل "help.h"
#شامل "commit-reach.h"
#شامل "commit-graph.h"
#شامل "pretty.h"
#شامل "trailer.h"

ایستا ثابت نویسه * ثابت builtin_commit_usage[] = {
	N_("git commit [-a | --interactive | --patch] [-s] [-v] [-u[<mode>]] [--amend]\n"
	   "           [--dry-run] [(-c | -C | --squash) <commit> | --fixup [(amend|reword):]<commit>]\n"
	   "           [-F <file> | -m <msg>] [--reset-author] [--allow-empty]\n"
	   "           [--allow-empty-message] [--no-verify] [-e] [--author=<author>]\n"
	   "           [--date=<date>] [--cleanup=<mode>] [--[no-]status]\n"
	   "           [-i | -o] [--pathspec-from-file=<file> [--pathspec-file-nul]]\n"
	   "           [(--trailer <token>[(=|:)<value>])...] [-S[<keyid>]]\n"
	   "           [--] [<pathspec>...]"),
	پوچ
};

ایستا ثابت نویسه * ثابت builtin_status_usage[] = {
	N_("git status [<options>] [--] [<pathspec>...]"),
	پوچ
};

ایستا ثابت نویسه empty_amend_advice[] =
N_("You asked to amend the most recent commit, but doing so would make\n"
"it empty. You can repeat your command with --allow-empty, or you can\n"
"remove the commit entirely with \"git reset HEAD^\".\n");

ایستا ثابت نویسه empty_cherry_pick_advice[] =
N_("The previous cherry-pick is now empty, possibly due to conflict resolution.\n"
"If you wish to commit it anyway, use:\n"
"\n"
"    git commit --allow-empty\n"
"\n");

ایستا ثابت نویسه empty_rebase_pick_advice[] =
N_("Otherwise, please use 'git rebase --skip'\n");

ایستا ثابت نویسه empty_cherry_pick_advice_single[] =
N_("Otherwise, please use 'git cherry-pick --skip'\n");

ایستا ثابت نویسه empty_cherry_pick_advice_multi[] =
N_("and then use:\n"
"\n"
"    git cherry-pick --continue\n"
"\n"
"to resume cherry-picking the remaining commits.\n"
"If you wish to skip this commit, use:\n"
"\n"
"    git cherry-pick --skip\n"
"\n");

ایستا ثابت نویسه *color_status_slots[] = {
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

ایستا ثابت نویسه *use_message_buffer;
ایستا ساختار lock_file index_lock; /* real index */
ایستا ساختار lock_file false_lock; /* used only for partial commits */
ایستا شمارش {
	COMMIT_AS_IS = 1,
	COMMIT_NORMAL,
	COMMIT_PARTIAL
} commit_style;

ایستا ثابت نویسه *force_author;
ایستا نویسه *logfile;
ایستا نویسه *template_file;
/*
 * The _message variables are commit names from which to take
 * the commit message and/or authorship.
 */
ایستا ثابت نویسه *author_message, *author_message_buffer;
ایستا ثابت نویسه *edit_message, *use_message;
ایستا نویسه *fixup_message, *fixup_commit, *squash_message;
ایستا ثابت نویسه *fixup_prefix;
ایستا صحیح all, also, interactive, patch_interactive, only, amend, signoff;
ایستا ساختار interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
ایستا صحیح edit_flag = -1; /* unspecified */
ایستا صحیح quiet, verbose, no_verify, allow_empty, dry_run, renew_authorship;
ایستا صحیح config_commit_verbose = -1; /* unspecified */
ایستا صحیح no_post_rewrite, allow_empty_message, pathspec_file_nul;
ایستا ثابت نویسه *untracked_files_arg, *force_date, *ignore_submodule_arg, *ignored_arg;
ایستا ثابت نویسه *sign_commit, *pathspec_from_file;
ایستا ساختار strvec trailer_args = STRVEC_INIT;

/*
 * The default commit message cleanup mode will remove the lines
 * beginning with # (shell comments) and leading and trailing
 * whitespaces (empty lines or containing only whitespaces)
 * if editor is used, and only the whitespaces if the message
 * is specified explicitly.
 */
ایستا شمارش commit_پیام_cleanup_mode cleanup_mode;
ایستا نویسه *cleanup_config;

ایستا شمارش commit_whence whence;
ایستا صحیح use_editor = 1, include_status = 1;
ایستا صحیح have_option_m;
ایستا ساختار strbuf message = STRBUF_INIT;

ایستا شمارش wt_status_format status_format = STATUS_FORMAT_UNSPECIFIED;

ایستا صحیح opt_parse_porcelain(ثابت ساختار option *opt, ثابت نویسه *arg, صحیح unset)
{
	شمارش wt_status_format *مقدار = (شمارش wt_status_format *)opt->مقدار;
	اگر (unset)
		*مقدار = STATUS_FORMAT_NONE;
	وگرنه اگر (!arg)
		*مقدار = STATUS_FORMAT_PORCELAIN;
	وگرنه اگر (!strcmp(arg, "v1") || !strcmp(arg, "1"))
		*مقدار = STATUS_FORMAT_PORCELAIN;
	وگرنه اگر (!strcmp(arg, "v2") || !strcmp(arg, "2"))
		*مقدار = STATUS_FORMAT_PORCELAIN_V2;
	وگرنه
		die("unsupported porcelain version '%s'", arg);

	بازگشت 0;
}

ایستا صحیح opt_parse_m(ثابت ساختار option *opt, ثابت نویسه *arg, صحیح unset)
{
	ساختار strbuf *buf = opt->مقدار;
	اگر (unset) {
		have_option_m = 0;
		strbuf_setlen(buf, 0);
	} وگرنه {
		have_option_m = 1;
		اگر (buf->len)
			strbuf_addch(buf, '\n');
		strbuf_addstr(buf, arg);
		strbuf_complete_سطر(buf);
	}
	بازگشت 0;
}

ایستا صحیح opt_parse_rename_score(ثابت ساختار option *opt, ثابت نویسه *arg, صحیح unset)
{
	ثابت نویسه **مقدار = opt->مقدار;

	BUG_ON_OPT_NEG(unset);

	اگر (arg != پوچ && *arg == '=')
		arg = arg + 1;

	*مقدار = arg;
	بازگشت 0;
}

ایستا تهی determine_whence(ساختار wt_status *s)
{
	اگر (file_exists(git_path_merge_head(the_repository)))
		whence = FROM_MERGE;
	وگرنه اگر (!sequencer_determine_whence(the_repository, &whence))
		whence = FROM_COMMIT;
	اگر (s)
		s->whence = whence;
}

ایستا تهی status_init_config(ساختار wt_status *s, config_fn_t fn)
{
	wt_status_prepare(the_repository, s);
	init_diff_ui_defaults();
	repo_config(the_repository, fn, s);
	determine_whence(s);
	s->hints = advice_enabled(ADVICE_STATUS_HINTS); /* must come after repo_config() */
}

ایستا تهی rollback_index_files(تهی)
{
	انتخاب (commit_style) {
	حالت COMMIT_AS_IS:
		بشکن; /* nothing to do */
	حالت COMMIT_NORMAL:
		rollback_lock_file(&index_lock);
		بشکن;
	حالت COMMIT_PARTIAL:
		rollback_lock_file(&index_lock);
		rollback_lock_file(&false_lock);
		بشکن;
	}
}

ایستا صحیح commit_index_files(تهی)
{
	صحیح err = 0;

	انتخاب (commit_style) {
	حالت COMMIT_AS_IS:
		بشکن; /* nothing to do */
	حالت COMMIT_NORMAL:
		err = commit_lock_file(&index_lock);
		بشکن;
	حالت COMMIT_PARTIAL:
		err = commit_lock_file(&index_lock);
		rollback_lock_file(&false_lock);
		بشکن;
	}

	بازگشت err;
}

/*
 * Take a union of paths in the index and the named tree (typically, "HEAD"),
 * and return the paths that match the given pattern in list.
 */
ایستا صحیح list_paths(ساختار string_list *list, ثابت نویسه *with_درخت,
		      ثابت ساختار pathspec *pattern)
{
	صحیح i, ret;
	نویسه *m;

	اگر (!pattern->nr)
		بازگشت 0;

	m = xcalloc(1, pattern->nr);

	اگر (with_درخت) {
		نویسه *max_prefix = common_prefix(pattern);
		overlay_درخت_on_index(the_repository->index, with_درخت, max_prefix);
		آزاد(max_prefix);
	}

	/* TODO: audit for interaction with sparse-index. */
	ensure_full_index(the_repository->index);
	برای (i = 0; i < the_repository->index->cache_nr; i++) {
		ثابت ساختار cache_entry *ce = the_repository->index->cache[i];
		ساختار string_list_item *item;

		اگر (ce->ce_flags & CE_UPDATE)
			ادامه;
		اگر (!ce_path_match(the_repository->index, ce, pattern, m))
			ادامه;
		item = string_list_درج(list, ce->name);
		اگر (ce_skip_worktree(ce))
			item->util = item; /* better a valid pointer than a fake one */
	}

	ret = report_path_error(m, pattern);
	آزاد(m);
	بازگشت ret;
}

ایستا تهی add_remove_files(ساختار string_list *list)
{
	صحیح i;
	برای (i = 0; i < list->nr; i++) {
		ساختار stat st;
		ساختار string_list_item *p = &(list->اقلام[i]);

		/* p->util is skip-worktree */
		اگر (p->util)
			ادامه;

		اگر (!lstat(p->string, &st)) {
			اگر (add_to_index(the_repository->index, p->string, &st, 0))
				die(_("updating files failed"));
		} وگرنه
			remove_file_from_index(the_repository->index, p->string);
	}
}

ایستا تهی create_base_index(ثابت ساختار commit *current_head)
{
	ساختار درخت *درخت;
	ساختار unpack_trees_options opts;
	ساختار درخت_desc t;

	اگر (!current_head) {
		discard_index(the_repository->index);
		بازگشت;
	}

	memset(&opts, 0, اندازه(opts));
	opts.head_idx = 1;
	opts.index_only = 1;
	opts.merge = 1;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;

	opts.fn = oneway_merge;
	درخت = repo_parse_درخت_indirect(the_repository,
					&current_head->object.oid);
	اگر (!درخت)
		die(_("failed to unpack HEAD tree object"));
	اگر (repo_parse_درخت(the_repository, درخت) < 0)
		exit(128);
	init_درخت_desc(&t, &درخت->object.oid, درخت->buffer, درخت->size);
	اگر (unpack_trees(1, &t, &opts))
		exit(128); /* We've already reported the error, finish dying */
}

ایستا تهی refresh_cache_or_die(صحیح refresh_flags)
{
	/*
	 * refresh_flags contains REFRESH_QUIET, so the only errors
	 * are for unmerged entries.
	 */
	اگر (refresh_index(the_repository->index, refresh_flags | REFRESH_IN_PORCELAIN, پوچ, پوچ, پوچ))
		die_resolve_conflict("commit");
}

ایستا ثابت نویسه *prepare_index(ثابت نویسه **argv, ثابت نویسه *prefix,
				 ثابت ساختار commit *current_head, صحیح is_status)
{
	ساختار string_list partial = STRING_LIST_INIT_DUP;
	ساختار pathspec pathspec;
	صحیح refresh_flags = REFRESH_QUIET;
	ثابت نویسه *ret;
	نویسه *path = پوچ;

	اگر (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	اگر (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	اگر (is_status)
		refresh_flags |= REFRESH_UNMERGED;
	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_FULL,
		       prefix, argv);

	اگر (pathspec_from_file) {
		اگر (interactive)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--interactive/--patch");

		اگر (all)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "-a");

		اگر (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_FULL,
				    prefix, pathspec_from_file, pathspec_file_nul);
	} وگرنه اگر (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	اگر (!pathspec.nr && (also || (only && !allow_empty &&
	    (!amend || (fixup_message && strcmp(fixup_prefix, "amend"))))))
		die(_("No paths with --include/--only does not make sense."));

	اگر (repo_read_index_preload(the_repository, &pathspec, 0) < 0)
		die(_("index file corrupt"));

	اگر (interactive) {
		نویسه *old_index_env = پوچ, *old_repo_index_file;
		repo_hold_locked_index(the_repository, &index_lock,
				       LOCK_DIE_ON_ERROR);

		refresh_cache_or_die(refresh_flags);

		اگر (write_locked_index(the_repository->index, &index_lock, 0))
			die(_("unable to create temporary index"));

		old_repo_index_file = the_repository->index_file;
		the_repository->index_file =
			(نویسه *)get_lock_file_path(&index_lock);
		old_index_env = xstrdup_or_null(getenv(INDEX_ENVIRONMENT));
		setenv(INDEX_ENVIRONMENT, the_repository->index_file, 1);

		اگر (interactive_add(the_repository, argv, prefix, patch_interactive, &interactive_opts) != 0)
			die(_("interactive add failed"));

		the_repository->index_file = old_repo_index_file;
		اگر (old_index_env && *old_index_env)
			setenv(INDEX_ENVIRONMENT, old_index_env, 1);
		وگرنه
			unsetenv(INDEX_ENVIRONMENT);
		FREE_AND_NULL(old_index_env);

		discard_index(the_repository->index);
		read_index_from(the_repository->index, get_lock_file_path(&index_lock),
				repo_get_git_dir(the_repository));
		اگر (cache_درخت_update(the_repository->index, WRITE_درخت_SILENT) == 0) {
			اگر (reopen_lock_file(&index_lock) < 0)
				die(_("unable to write index file"));
			اگر (write_locked_index(the_repository->index, &index_lock, 0))
				die(_("unable to update temporary index"));
		} وگرنه
			warning(_("Failed to update main cache tree"));

		commit_style = COMMIT_NORMAL;
		ret = get_lock_file_path(&index_lock);
		برو out;
	} وگرنه {
		اگر (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--interactive/--patch");
		اگر (interactive_opts.interhunkcontext != -1)
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
	اگر (all || (also && pathspec.nr)) {
		نویسه *ps_matched = xcalloc(pathspec.nr, 1);
		repo_hold_locked_index(the_repository, &index_lock,
				       LOCK_DIE_ON_ERROR);
		add_files_to_cache(the_repository, also ? prefix : پوچ,
				   &pathspec, ps_matched, 0, 0, 0 );
		اگر (!all && report_path_error(ps_matched, &pathspec))
			exit(128);

		refresh_cache_or_die(refresh_flags);
		cache_درخت_update(the_repository->index, WRITE_درخت_SILENT);
		اگر (write_locked_index(the_repository->index, &index_lock, 0))
			die(_("unable to write new index file"));
		commit_style = COMMIT_NORMAL;
		ret = get_lock_file_path(&index_lock);
		آزاد(ps_matched);
		برو out;
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
	اگر (!only && !pathspec.nr) {
		repo_hold_locked_index(the_repository, &index_lock,
				       LOCK_DIE_ON_ERROR);
		refresh_cache_or_die(refresh_flags);
		اگر (the_repository->index->cache_changed
		    || !cache_درخت_fully_valid(the_repository->index))
			cache_درخت_update(the_repository->index, WRITE_درخت_SILENT);
		اگر (write_locked_index(the_repository->index, &index_lock,
				       COMMIT_LOCK | SKIP_IF_UNCHANGED))
			die(_("unable to write new index file"));
		commit_style = COMMIT_AS_IS;
		ret = repo_get_index_file(the_repository);
		برو out;
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

	اگر (whence != FROM_COMMIT) {
		اگر (whence == FROM_MERGE)
			die(_("cannot do a partial commit during a merge."));
		وگرنه اگر (is_from_cherry_pick(whence))
			die(_("cannot do a partial commit during a cherry-pick."));
		وگرنه اگر (is_from_rebase(whence))
			die(_("cannot do a partial commit during a rebase."));
	}

	اگر (list_paths(&partial, !current_head ? پوچ : "HEAD", &pathspec))
		exit(1);

	discard_index(the_repository->index);
	اگر (repo_read_index(the_repository) < 0)
		die(_("cannot read the index"));

	repo_hold_locked_index(the_repository, &index_lock, LOCK_DIE_ON_ERROR);
	add_remove_files(&partial);
	refresh_index(the_repository->index, REFRESH_QUIET, پوچ, پوچ, پوچ);
	cache_درخت_update(the_repository->index, WRITE_درخت_SILENT);
	اگر (write_locked_index(the_repository->index, &index_lock, 0))
		die(_("unable to write new index file"));

	path = repo_git_path(the_repository, "next-index-%"PRIuMAX,
			     (uintmax_t) getpid());
	hold_lock_file_for_update(&false_lock, path,
				  LOCK_DIE_ON_ERROR);

	create_base_index(current_head);
	add_remove_files(&partial);
	refresh_index(the_repository->index, REFRESH_QUIET, پوچ, پوچ, پوچ);

	اگر (write_locked_index(the_repository->index, &false_lock, 0))
		die(_("unable to write temporary index file"));

	discard_index(the_repository->index);
	ret = get_lock_file_path(&false_lock);
	read_index_from(the_repository->index, ret, repo_get_git_dir(the_repository));
out:
	string_list_clear(&partial, 0);
	clear_pathspec(&pathspec);
	آزاد(path);
	بازگشت ret;
}

ایستا صحیح run_status(FILE *fp, ثابت نویسه *index_file, ثابت نویسه *prefix, صحیح nowarn,
		      ساختار wt_status *s)
{
	ساختار object_id oid;

	اگر (s->relative_paths)
		s->prefix = prefix;

	اگر (amend) {
		s->amend = 1;
		s->reference = "HEAD^1";
	}
	s->verbose = verbose;
	s->index_file = index_file;
	s->fp = fp;
	s->nowarn = nowarn;
	s->is_initial = repo_get_oid(the_repository, s->reference, &oid) ? 1 : 0;
	اگر (!s->is_initial)
		oidcpy(&s->oid_commit, &oid);
	s->status_format = status_format;
	s->ignore_submodule_arg = ignore_submodule_arg;

	wt_status_collect(s);
	wt_status_print(s);
	wt_status_collect_free_buffers(s);

	بازگشت s->committable;
}

ایستا صحیح is_a_merge(ثابت ساختار commit *current_head)
{
	بازگشت !!(current_head->parents && current_head->parents->next);
}

ایستا تهی assert_split_ident(ساختار ident_split *id, ثابت ساختار strbuf *buf)
{
	اگر (split_ident_سطر(id, buf->buf, buf->len) || !id->date_begin)
		BUG("unable to parse our own ident: %s", buf->buf);
}

ایستا تهی export_one(ثابت نویسه *var, ثابت نویسه *s, ثابت نویسه *e, صحیح hack)
{
	ساختار strbuf buf = STRBUF_INIT;
	اگر (hack)
		strbuf_addch(&buf, hack);
	strbuf_add(&buf, s, e - s);
	setenv(var, buf.buf, 1);
	strbuf_release(&buf);
}

ایستا صحیح parse_force_date(ثابت نویسه *in, ساختار strbuf *out)
{
	strbuf_addch(out, '@');

	اگر (parse_date(in, out) < 0) {
		صحیح errors = 0;
		unsigned long t = approxidate_careful(in, &errors);
		اگر (errors)
			بازگشت -1;
		strbuf_addf(out, "%lu", t);
	}

	بازگشت 0;
}

ایستا تهی set_ident_var(نویسه **buf, نویسه *val)
{
	آزاد(*buf);
	*buf = val;
}

ایستا تهی determine_author_info(ساختار strbuf *author_ident)
{
	نویسه *name, *email, *date;
	ساختار ident_split author;

	name = xstrdup_or_null(getenv("GIT_AUTHOR_NAME"));
	email = xstrdup_or_null(getenv("GIT_AUTHOR_EMAIL"));
	date = xstrdup_or_null(getenv("GIT_AUTHOR_DATE"));

	اگر (author_message) {
		ساختار ident_split ident;
		size_t len;
		ثابت نویسه *a;

		a = find_commit_header(author_message_buffer, "author", &len);
		اگر (!a)
			die(_("commit '%s' lacks author header"), author_message);
		اگر (split_ident_سطر(&ident, a, len) < 0)
			die(_("commit '%s' has malformed author line"), author_message);

		set_ident_var(&name, xmemdupz(ident.name_begin, ident.name_end - ident.name_begin));
		set_ident_var(&email, xmemdupz(ident.mail_begin, ident.mail_end - ident.mail_begin));

		اگر (ident.date_begin) {
			ساختار strbuf date_buf = STRBUF_INIT;
			strbuf_addch(&date_buf, '@');
			strbuf_add(&date_buf, ident.date_begin, ident.date_end - ident.date_begin);
			strbuf_addch(&date_buf, ' ');
			strbuf_add(&date_buf, ident.tz_begin, ident.tz_end - ident.tz_begin);
			set_ident_var(&date, strbuf_detach(&date_buf, پوچ));
		}
	}

	اگر (force_author) {
		ساختار ident_split ident;

		اگر (split_ident_سطر(&ident, force_author, strlen(force_author)) < 0)
			die(_("malformed --author parameter"));
		set_ident_var(&name, xmemdupz(ident.name_begin, ident.name_end - ident.name_begin));
		set_ident_var(&email, xmemdupz(ident.mail_begin, ident.mail_end - ident.mail_begin));
	}

	اگر (force_date) {
		ساختار strbuf date_buf = STRBUF_INIT;
		اگر (parse_force_date(force_date, &date_buf))
			die(_("invalid date format: %s"), force_date);
		set_ident_var(&date, strbuf_detach(&date_buf, پوچ));
	}

	strbuf_addstr(author_ident, fmt_ident(name, email, WANT_AUTHOR_IDENT, date,
				IDENT_STRICT));
	assert_split_ident(&author, author_ident);
	export_one("GIT_AUTHOR_NAME", author.name_begin, author.name_end, 0);
	export_one("GIT_AUTHOR_EMAIL", author.mail_begin, author.mail_end, 0);
	export_one("GIT_AUTHOR_DATE", author.date_begin, author.tz_end, '@');
	آزاد(name);
	آزاد(email);
	آزاد(date);
}

ایستا صحیح author_date_is_interesting(تهی)
{
	بازگشت author_message || force_date;
}

#ifndef WITH_BREAKING_CHANGES
ایستا تهی adjust_comment_سطر_char(ثابت ساختار strbuf *sb)
{
	نویسه candidates[] = "#;@!$%^&|:";
	نویسه *candidate;
	ثابت نویسه *p;
	size_t cutoff;

	/* Ignore comment chars in trailing comments (e.g., Conflicts:) */
	cutoff = sb->len - ignored_log_message_bytes(sb->buf, sb->len);

	اگر (!memchr(sb->buf, candidates[0], sb->len)) {
		آزاد(comment_سطر_str_to_free);
		comment_سطر_str = comment_سطر_str_to_free =
			xstrfmt("%c", candidates[0]);
		بازگشت;
	}

	p = sb->buf;
	candidate = strchr(candidates, *p);
	اگر (candidate)
		*candidate = ' ';
	برای (p = sb->buf; p + 1 < sb->buf + cutoff; p++) {
		اگر ((p[0] == '\n' || p[0] == '\r') && p[1]) {
			candidate = strchr(candidates, p[1]);
			اگر (candidate)
				*candidate = ' ';
		}
	}

	برای (p = candidates; *p == ' '; p++)
		;
	اگر (!*p)
		die(_("unable to select a comment character that is not used\n"
		      "in the current commit message"));
	آزاد(comment_سطر_str_to_free);
	comment_سطر_str = comment_سطر_str_to_free = xstrfmt("%c", *p);
}
#endif /* !WITH_BREAKING_CHANGES */

ایستا تهی prepare_amend_commit(ساختار commit *commit, ساختار strbuf *sb,
				ساختار pretty_print_context *ctx)
{
	ثابت نویسه *buffer, *subject, *fmt;

	buffer = repo_get_commit_buffer(the_repository, commit, پوچ);
	find_commit_subject(buffer, &subject);
	/*
	 * If we amend the 'amend!' commit then we don't want to
	 * duplicate the subject line.
	 */
	fmt = starts_with(subject, "amend!") ? "%b" : "%B";
	repo_format_commit_message(the_repository, commit, fmt, sb, ctx);
	repo_unuse_commit_buffer(the_repository, commit, buffer);
}

ایستا تهی change_data_free(تهی *util, ثابت نویسه *str UNUSED)
{
	ساختار wt_status_change_data *d = util;
	آزاد(d->rename_source);
	آزاد(d);
}

ایستا صحیح prepare_to_commit(ثابت نویسه *index_file, ثابت نویسه *prefix,
			     ساختار commit *current_head,
			     ساختار wt_status *s,
			     ساختار strbuf *author_ident)
{
	ساختار stat statbuf;
	ساختار strbuf committer_ident = STRBUF_INIT;
	صحیح committable;
	ساختار strbuf sb = STRBUF_INIT;
	ثابت نویسه *hook_arg1 = پوچ;
	ثابت نویسه *hook_arg2 = پوچ;
	صحیح clean_message_contents = (cleanup_mode != COMMIT_پیام_CLEANUP_NONE);
	صحیح old_display_comment_prefix;
	صحیح invoked_hook;

	/* This checks and barfs if author is badly specified */
	determine_author_info(author_ident);

	اگر (!no_verify && run_commit_hook(use_editor, index_file, &invoked_hook,
					  "pre-commit", پوچ))
		بازگشت 0;

	اگر (squash_message) {
		/*
		 * Insert the proper subject line before other commit
		 * message options add their content.
		 */
		اگر (use_message && !strcmp(use_message, squash_message))
			strbuf_addstr(&sb, "squash! ");
		وگرنه {
			ساختار pretty_print_context ctx = {0};
			ساختار commit *c;
			c = lookup_commit_reference_by_name(squash_message);
			اگر (!c)
				die(_("could not lookup commit '%s'"), squash_message);
			ctx.output_encoding = get_commit_output_encoding();
			repo_format_commit_message(the_repository, c,
						   "squash! %s\n\n", &sb,
						   &ctx);
		}
	}

	اگر (have_option_m && !fixup_message) {
		strbuf_addbuf(&sb, &message);
		hook_arg1 = "message";
	} وگرنه اگر (logfile && !strcmp(logfile, "-")) {
		اگر (isatty(0))
			fprintf(stderr, _("(reading log message from standard input)\n"));
		اگر (strbuf_read(&sb, 0, 0) < 0)
			die_errno(_("could not read log from standard input"));
		hook_arg1 = "message";
	} وگرنه اگر (logfile) {
		اگر (strbuf_read_file(&sb, logfile, 0) < 0)
			die_errno(_("could not read log file '%s'"),
				  logfile);
		hook_arg1 = "message";
	} وگرنه اگر (use_message) {
		ثابت نویسه *buffer;
		buffer = strstr(use_message_buffer, "\n\n");
		اگر (buffer)
			strbuf_addstr(&sb, skip_blank_سطرها(buffer + 2));
		hook_arg1 = "commit";
		hook_arg2 = use_message;
	} وگرنه اگر (fixup_message) {
		ساختار pretty_print_context ctx = {0};
		ساختار commit *commit;
		نویسه *fmt;
		commit = lookup_commit_reference_by_name(fixup_commit);
		اگر (!commit)
			die(_("could not lookup commit '%s'"), fixup_commit);
		ctx.output_encoding = get_commit_output_encoding();
		fmt = xstrfmt("%s! %%s\n\n", fixup_prefix);
		repo_format_commit_message(the_repository, commit, fmt, &sb,
					   &ctx);
		آزاد(fmt);
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
		اگر (have_option_m && !strcmp(fixup_prefix, "fixup"))
			strbuf_addbuf(&sb, &message);

		اگر (!strcmp(fixup_prefix, "amend")) {
			اگر (have_option_m)
				die(_("options '%s' and '%s:%s' cannot be used together"), "-m", "--fixup", fixup_message);
			prepare_amend_commit(commit, &sb, &ctx);
		}
	} وگرنه اگر (!stat(git_path_merge_پیام(the_repository), &statbuf)) {
		size_t merge_پیام_start;

		/*
		 * prepend SQUASH_MSG here if it exists and a
		 * "merge --squash" was originally performed
		 */
		اگر (!stat(git_path_squash_پیام(the_repository), &statbuf)) {
			اگر (strbuf_read_file(&sb, git_path_squash_پیام(the_repository), 0) < 0)
				die_errno(_("could not read SQUASH_MSG"));
			hook_arg1 = "squash";
		} وگرنه
			hook_arg1 = "merge";

		merge_پیام_start = sb.len;
		اگر (strbuf_read_file(&sb, git_path_merge_پیام(the_repository), 0) < 0)
			die_errno(_("could not read MERGE_MSG"));

		اگر (cleanup_mode == COMMIT_پیام_CLEANUP_SCISSORS &&
		    wt_status_locate_end(sb.buf + merge_پیام_start,
					 sb.len - merge_پیام_start) <
				sb.len - merge_پیام_start)
			s->added_cut_سطر = 1;
	} وگرنه اگر (!stat(git_path_squash_پیام(the_repository), &statbuf)) {
		اگر (strbuf_read_file(&sb, git_path_squash_پیام(the_repository), 0) < 0)
			die_errno(_("could not read SQUASH_MSG"));
		hook_arg1 = "squash";
	} وگرنه اگر (template_file) {
		اگر (strbuf_read_file(&sb, template_file, 0) < 0)
			die_errno(_("could not read '%s'"), template_file);
		hook_arg1 = "template";
		clean_message_contents = 0;
	}

	/*
	 * The remaining cases don't modify the template message, but
	 * just set the argument(s) to the prepare-commit-msg hook.
	 */
	وگرنه اگر (whence == FROM_MERGE)
		hook_arg1 = "merge";
	وگرنه اگر (is_from_cherry_pick(whence) || whence == FROM_REBASE_PICK) {
		hook_arg1 = "commit";
		hook_arg2 = "CHERRY_PICK_HEAD";
	}

	اگر (squash_message) {
		/*
		 * If squash_commit was used for the commit subject,
		 * then we're possibly hijacking other commit log options.
		 * Reset the hook args to tell the real story.
		 */
		hook_arg1 = "message";
		hook_arg2 = "";
	}

	s->fp = fopen_for_writing(git_path_commit_editmsg());
	اگر (!s->fp)
		die_errno(_("could not open '%s'"), git_path_commit_editmsg());

	/* Ignore status.displayCommentPrefix: we do need comments in COMMIT_EDITMSG. */
	old_display_comment_prefix = s->display_comment_prefix;
	s->display_comment_prefix = 1;

	/*
	 * Most hints are counter-productive when the commit has
	 * already started.
	 */
	s->hints = 0;

	اگر (clean_message_contents)
		strbuf_stripspace(&sb, پوچ);

	اگر (signoff)
		append_signoff(&sb, ignored_log_message_bytes(sb.buf, sb.len), 0);

	اگر (fwrite(sb.buf, 1, sb.len, s->fp) < sb.len)
		die_errno(_("could not write commit template"));

#ifndef WITH_BREAKING_CHANGES
	اگر (auto_comment_سطر_char)
		adjust_comment_سطر_char(&sb);
#endif /* !WITH_BREAKING_CHANGES */
	strbuf_release(&sb);

	/* This checks if committer ident is explicitly given */
	strbuf_addstr(&committer_ident, git_committer_info(IDENT_STRICT));
	اگر (use_editor && include_status) {
		صحیح ident_shown = 0;
		شمارش git_colorbool saved_color_setting;
		ساختار ident_split ci, ai;
		ثابت نویسه *hint_cleanup_all = allow_empty_message ?
			_("Please enter the commit message for your changes."
			  " Lines starting\nwith '%s' will be ignored.\n") :
			_("Please enter the commit message for your changes."
			  " Lines starting\nwith '%s' will be ignored, and an empty"
			  " message aborts the commit.\n");
		ثابت نویسه *hint_cleanup_space = allow_empty_message ?
			_("Please enter the commit message for your changes."
			  " Lines starting\n"
			  "with '%s' will be kept; you may remove them"
			  " yourself if you want to.\n") :
			_("Please enter the commit message for your changes."
			  " Lines starting\n"
			  "with '%s' will be kept; you may remove them"
			  " yourself if you want to.\n"
			  "An empty message aborts the commit.\n");
		اگر (whence != FROM_COMMIT) {
			اگر (cleanup_mode == COMMIT_پیام_CLEANUP_SCISSORS)
				wt_status_add_cut_سطر(s);
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
		اگر (cleanup_mode == COMMIT_پیام_CLEANUP_ALL)
			status_printf(s, GIT_COLOR_NORMAL, hint_cleanup_all, comment_سطر_str);
		وگرنه اگر (cleanup_mode == COMMIT_پیام_CLEANUP_SCISSORS) {
			اگر (whence == FROM_COMMIT)
				wt_status_add_cut_سطر(s);
		} وگرنه /* COMMIT_MSG_CLEANUP_SPACE, that is. */
			status_printf(s, GIT_COLOR_NORMAL, hint_cleanup_space, comment_سطر_str);

		/*
		 * These should never fail because they come from our own
		 * fmt_ident. They may fail the sane_ident test, but we know
		 * that the name and mail pointers will at least be valid,
		 * which is enough for our tests and printing here.
		 */
		assert_split_ident(&ai, author_ident);
		assert_split_ident(&ci, &committer_ident);

		اگر (ident_cmp(&ai, &ci))
			status_printf_ln(s, GIT_COLOR_NORMAL,
				_("%s"
				"Author:    %.*s <%.*s>"),
				ident_shown++ ? "" : "\n",
				(صحیح)(ai.name_end - ai.name_begin), ai.name_begin,
				(صحیح)(ai.mail_end - ai.mail_begin), ai.mail_begin);

		اگر (author_date_is_interesting())
			status_printf_ln(s, GIT_COLOR_NORMAL,
				_("%s"
				"Date:      %s"),
				ident_shown++ ? "" : "\n",
				show_ident_date(&ai, DATE_MODE(NORMAL)));

		اگر (!committer_ident_sufficiently_given())
			status_printf_ln(s, GIT_COLOR_NORMAL,
				_("%s"
				"Committer: %.*s <%.*s>"),
				ident_shown++ ? "" : "\n",
				(صحیح)(ci.name_end - ci.name_begin), ci.name_begin,
				(صحیح)(ci.mail_end - ci.mail_begin), ci.mail_begin);

		status_printf_ln(s, GIT_COLOR_NORMAL, "%s", ""); /* Add new line for clarity */

		saved_color_setting = s->use_color;
		s->use_color = GIT_COLOR_NEVER;
		committable = run_status(s->fp, index_file, prefix, 1, s);
		s->use_color = saved_color_setting;
		string_list_clear_func(&s->change, change_data_free);
	} وگرنه {
		ساختار object_id oid;
		ثابت نویسه *parent = "HEAD";

		اگر (!the_repository->index->initialized && repo_read_index(the_repository) < 0)
			die(_("Cannot read index"));

		اگر (amend)
			parent = "HEAD^1";

		اگر (repo_get_oid(the_repository, parent, &oid)) {
			صحیح i, ita_nr = 0;

			/* TODO: audit for interaction with sparse-index. */
			ensure_full_index(the_repository->index);
			برای (i = 0; i < the_repository->index->cache_nr; i++)
				اگر (ce_intent_to_add(the_repository->index->cache[i]))
					ita_nr++;
			committable = the_repository->index->cache_nr > ita_nr;
		} وگرنه {
			/*
			 * Unless the user did explicitly request a submodule
			 * ignore mode by passing a command line option we do
			 * not ignore any changed submodule SHA-1s when
			 * comparing index and parent, no matter what is
			 * configured. Otherwise we won't commit any
			 * submodules which were manually staged, which would
			 * be really confusing.
			 */
			ساختار diff_flags flags = DIFF_FLAGS_INIT;
			flags.override_submodule_config = 1;
			اگر (ignore_submodule_arg &&
			    !strcmp(ignore_submodule_arg, "all"))
				flags.ignore_submodules = 1;
			committable = index_differs_from(the_repository,
							 parent, &flags, 1);
		}
	}
	strbuf_release(&committer_ident);

	fclose(s->fp);

	اگر (trailer_args.nr) {
		اگر (amend_file_with_trailers(git_path_commit_editmsg(), &trailer_args))
			die(_("unable to pass trailers to --trailers"));
		strvec_clear(&trailer_args);
	}

	/*
	 * Reject an attempt to record a non-merge empty commit without
	 * explicit --allow-empty. In the cherry-pick case, it may be
	 * empty due to conflict resolution, which the user should okay.
	 */
	اگر (!committable && whence != FROM_MERGE && !allow_empty &&
	    !(amend && is_a_merge(current_head))) {
		s->hints = advice_enabled(ADVICE_STATUS_HINTS);
		s->display_comment_prefix = old_display_comment_prefix;
		run_status(stdout, index_file, prefix, 0, s);
		اگر (amend)
			fputs(_(empty_amend_advice), stderr);
		وگرنه اگر (is_from_cherry_pick(whence) ||
			 whence == FROM_REBASE_PICK) {
			fputs(_(empty_cherry_pick_advice), stderr);
			اگر (whence == FROM_CHERRY_PICK_SINGLE)
				fputs(_(empty_cherry_pick_advice_single), stderr);
			وگرنه اگر (whence == FROM_CHERRY_PICK_MULTI)
				fputs(_(empty_cherry_pick_advice_multi), stderr);
			وگرنه
				fputs(_(empty_rebase_pick_advice), stderr);
		}
		بازگشت 0;
	}

	اگر (!no_verify && invoked_hook) {
		/*
		 * Re-read the index as the pre-commit-commit hook was invoked
		 * and could have updated it. We must do this before we invoke
		 * the editor and after we invoke run_status above.
		 */
		discard_index(the_repository->index);
	}
	read_index_from(the_repository->index, index_file, repo_get_git_dir(the_repository));

	اگر (cache_درخت_update(the_repository->index, 0)) {
		error(_("Error building trees"));
		بازگشت 0;
	}

	اگر (run_commit_hook(use_editor, index_file, پوچ, "prepare-commit-msg",
			    git_path_commit_editmsg(), hook_arg1, hook_arg2, پوچ))
		بازگشت 0;

	اگر (use_editor) {
		ساختار strvec env = STRVEC_INIT;

		strvec_pushf(&env, "GIT_INDEX_FILE=%s", index_file);
		اگر (launch_editor(git_path_commit_editmsg(), پوچ, env.v)) {
			fprintf(stderr,
			_("Please supply the message using either -m or -F option.\n"));
			exit(1);
		}
		strvec_clear(&env);
	}

	اگر (!no_verify &&
	    run_commit_hook(use_editor, index_file, پوچ, "commit-msg",
			    git_path_commit_editmsg(), پوچ)) {
		بازگشت 0;
	}

	بازگشت 1;
}

ایستا ثابت نویسه *find_author_by_nickname(ثابت نویسه *name)
{
	ساختار rev_info revs;
	ساختار commit *commit;
	ساختار strbuf buf = STRBUF_INIT;
	ثابت نویسه *av[20];
	صحیح ac = 0;

	repo_init_revisions(the_repository, &revs, پوچ);
	strbuf_addf(&buf, "--author=%s", name);
	av[++ac] = "--all";
	av[++ac] = "-i";
	av[++ac] = buf.buf;
	av[++ac] = پوچ;
	setup_revisions(ac, av, &revs, پوچ);
	revs.mailmap = xmalloc(اندازه(ساختار string_list));
	string_list_init_nodup(revs.mailmap);
	read_mailmap(the_repository, revs.mailmap);

	اگر (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	commit = get_revision(&revs);
	اگر (commit) {
		ساختار pretty_print_context ctx = {0};
		ctx.date_mode.type = DATE_NORMAL;
		strbuf_release(&buf);
		repo_format_commit_message(the_repository, commit,
					   "%aN <%aE>", &buf, &ctx);
		release_revisions(&revs);
		بازگشت strbuf_detach(&buf, پوچ);
	}
	die(_("--author '%s' is not 'Name <email>' and matches no existing author"), name);
}

ایستا تهی handle_ignored_arg(ساختار wt_status *s)
{
	اگر (!ignored_arg)
		; /* default already initialized */
	وگرنه اگر (!strcmp(ignored_arg, "traditional"))
		s->show_ignored_mode = SHOW_TRADITIONAL_IGNORED;
	وگرنه اگر (!strcmp(ignored_arg, "no"))
		s->show_ignored_mode = SHOW_NO_IGNORED;
	وگرنه اگر (!strcmp(ignored_arg, "matching"))
		s->show_ignored_mode = SHOW_MATCHING_IGNORED;
	وگرنه
		die(_("Invalid ignored mode '%s'"), ignored_arg);
}

ایستا شمارش untracked_status_type parse_untracked_setting_name(ثابت نویسه *u)
{
	/*
	 * Please update $__git_untracked_file_modes in
	 * git-completion.bash when you add new options
	 */
	انتخاب (git_parse_maybe_bool(u)) {
	حالت 0:
		u = "no";
		بشکن;
	حالت 1:
		u = "normal";
		بشکن;
	پیشفرض:
		بشکن;
	}

	اگر (!strcmp(u, "no"))
		بازگشت SHOW_NO_UNTRACKED_FILES;
	وگرنه اگر (!strcmp(u, "normal"))
		بازگشت SHOW_NORMAL_UNTRACKED_FILES;
	وگرنه اگر (!strcmp(u, "all"))
		بازگشت SHOW_ALL_UNTRACKED_FILES;
	وگرنه
		بازگشت SHOW_UNTRACKED_FILES_ERROR;
}

ایستا تهی handle_untracked_files_arg(ساختار wt_status *s)
{
	شمارش untracked_status_type u;

	اگر (!untracked_files_arg)
		بازگشت; /* default already initialized */

	u = parse_untracked_setting_name(untracked_files_arg);
	اگر (u == SHOW_UNTRACKED_FILES_ERROR)
		die(_("Invalid untracked files mode '%s'"),
		    untracked_files_arg);
	s->show_untracked_files = u;
}

ایستا ثابت نویسه *read_commit_message(ثابت نویسه *name)
{
	ثابت نویسه *out_enc;
	ساختار commit *commit;

	commit = lookup_commit_reference_by_name(name);
	اگر (!commit)
		die(_("could not lookup commit '%s'"), name);
	out_enc = get_commit_output_encoding();
	بازگشت repo_logmsg_reencode(the_repository, commit, پوچ, out_enc);
}

/*
 * Enumerate what needs to be propagated when --porcelain
 * is not in effect here.
 */
ایستا ساختار status_deferred_config {
	شمارش wt_status_format status_format;
	صحیح show_branch;
	شمارش ahead_behind_flags ahead_behind;
} status_deferred_config = {
	STATUS_FORMAT_UNSPECIFIED,
	-1, /* unspecified */
	AHEAD_BEHIND_UNSPECIFIED,
};

ایستا تهی finalize_deferred_config(ساختار wt_status *s)
{
	صحیح use_deferred_config = (status_format != STATUS_FORMAT_PORCELAIN &&
				   status_format != STATUS_FORMAT_PORCELAIN_V2 &&
				   !s->null_termination);

	اگر (s->null_termination) {
		اگر (status_format == STATUS_FORMAT_NONE ||
		    status_format == STATUS_FORMAT_UNSPECIFIED)
			status_format = STATUS_FORMAT_PORCELAIN;
		وگرنه اگر (status_format == STATUS_FORMAT_LONG)
			die(_("options '%s' and '%s' cannot be used together"), "--long", "-z");
	}

	اگر (use_deferred_config && status_format == STATUS_FORMAT_UNSPECIFIED)
		status_format = status_deferred_config.status_format;
	اگر (status_format == STATUS_FORMAT_UNSPECIFIED)
		status_format = STATUS_FORMAT_NONE;

	اگر (use_deferred_config && s->show_branch < 0)
		s->show_branch = status_deferred_config.show_branch;
	اگر (s->show_branch < 0)
		s->show_branch = 0;

	/*
	 * If the user did not give a "--[no]-ahead-behind" command
	 * line argument *AND* we will print in a human-readable format
	 * (short, long etc.) then we inherit from the status.aheadbehind
	 * config setting.  In all other cases (and porcelain V[12] formats
	 * in particular), we inherit _FULL for backwards compatibility.
	 */
	اگر (use_deferred_config &&
	    s->ahead_behind_flags == AHEAD_BEHIND_UNSPECIFIED)
		s->ahead_behind_flags = status_deferred_config.ahead_behind;

	اگر (s->ahead_behind_flags == AHEAD_BEHIND_UNSPECIFIED)
		s->ahead_behind_flags = AHEAD_BEHIND_FULL;
}

ایستا تهی check_fixup_reword_options(صحیح argc, ثابت نویسه *argv[]) {
	اگر (whence != FROM_COMMIT) {
		اگر (whence == FROM_MERGE)
			die(_("You are in the middle of a merge -- cannot reword."));
		وگرنه اگر (is_from_cherry_pick(whence))
			die(_("You are in the middle of a cherry-pick -- cannot reword."));
	}
	اگر (argc)
		die(_("reword option of '%s' and path '%s' cannot be used together"), "--fixup", *argv);
	اگر (patch_interactive || interactive || all || also || only)
		die(_("reword option of '%s' and '%s' cannot be used together"),
			"--fixup", "--patch/--interactive/--all/--include/--only");
}

ایستا صحیح parse_and_validate_options(صحیح argc, ثابت نویسه *argv[],
				      ثابت ساختار option *options,
				      ثابت نویسه * ثابت usage[],
				      ثابت نویسه *prefix,
				      ساختار commit *current_head,
				      ساختار wt_status *s)
{
	argc = parse_options(argc, argv, prefix, options, usage, 0);
	finalize_deferred_config(s);

	اگر (force_author && !strchr(force_author, '>'))
		force_author = find_author_by_nickname(force_author);

	اگر (force_author && renew_authorship)
		die(_("options '%s' and '%s' cannot be used together"), "--reset-author", "--author");

	اگر (logfile || have_option_m || use_message)
		use_editor = 0;

	/* Sanity check options */
	اگر (amend && !current_head)
		die(_("You have nothing to amend."));
	اگر (amend && whence != FROM_COMMIT) {
		اگر (whence == FROM_MERGE)
			die(_("You are in the middle of a merge -- cannot amend."));
		وگرنه اگر (is_from_cherry_pick(whence))
			die(_("You are in the middle of a cherry-pick -- cannot amend."));
		وگرنه اگر (whence == FROM_REBASE_PICK)
			die(_("You are in the middle of a rebase -- cannot amend."));
	}
	اگر (fixup_message && squash_message)
		die(_("options '%s' and '%s' cannot be used together"), "--squash", "--fixup");
	die_for_incompatible_opt4(!!use_message, "-C",
				  !!edit_message, "-c",
				  !!logfile, "-F",
				  !!fixup_message, "--fixup");
	die_for_incompatible_opt4(have_option_m, "-m",
				  !!edit_message, "-c",
				  !!use_message, "-C",
				  !!logfile, "-F");
	اگر (use_message || edit_message || logfile ||fixup_message || have_option_m)
		FREE_AND_NULL(template_file);
	اگر (edit_message)
		use_message = edit_message;
	اگر (amend && !use_message && !fixup_message)
		use_message = "HEAD";
	اگر (!use_message && !is_from_cherry_pick(whence) &&
	    !is_from_rebase(whence) && renew_authorship)
		die(_("--reset-author can be used only with -C, -c or --amend."));
	اگر (use_message) {
		use_message_buffer = read_commit_message(use_message);
		اگر (!renew_authorship) {
			author_message = use_message;
			author_message_buffer = use_message_buffer;
		}
	}
	اگر ((is_from_cherry_pick(whence) || whence == FROM_REBASE_PICK) &&
	    !renew_authorship) {
		author_message = "CHERRY_PICK_HEAD";
		author_message_buffer = read_commit_message(author_message);
	}

	اگر (patch_interactive)
		interactive = 1;

	die_for_incompatible_opt4(also, "-i/--include",
				  only, "-o/--only",
				  all, "-a/--all",
				  interactive, "--interactive/-p/--patch");
	اگر (fixup_message) {
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
		نویسه *p = fixup_message;
		وقتی (isalpha(*p))
			p++;
		اگر (p > fixup_message && *p == ':') {
			*p = '\0';
			fixup_commit = p + 1;
			اگر (!strcmp("amend", fixup_message) ||
			    !strcmp("reword", fixup_message)) {
				fixup_prefix = "amend";
				allow_empty = 1;
				اگر (*fixup_message == 'r') {
					check_fixup_reword_options(argc, argv);
					only = 1;
				}
			} وگرنه {
				die(_("unknown option: --fixup=%s:%s"), fixup_message, fixup_commit);
			}
		} وگرنه {
			fixup_commit = fixup_message;
			fixup_prefix = "fixup";
			use_editor = 0;
		}
	}

	اگر (0 <= edit_flag)
		use_editor = edit_flag;

	handle_untracked_files_arg(s);

	اگر (all && argc > 0)
		die(_("paths '%s ...' with -a does not make sense"),
		    argv[0]);

	اگر (status_format != STATUS_FORMAT_NONE)
		dry_run = 1;

	بازگشت argc;
}

ایستا صحیح dry_run_commit(ثابت نویسه **argv, ثابت نویسه *prefix,
			  ثابت ساختار commit *current_head, ساختار wt_status *s)
{
	صحیح committable;
	ثابت نویسه *index_file;

	index_file = prepare_index(argv, prefix, current_head, 1);
	committable = run_status(stdout, index_file, prefix, 0, s);
	rollback_index_files();

	بازگشت committable ? 0 : 1;
}

define_list_config_array_extra(color_status_slots, {"added"});

ایستا صحیح parse_status_slot(ثابت نویسه *slot)
{
	اگر (!strcasecmp(slot, "added"))
		بازگشت WT_STATUS_UPDATED;

	بازگشت LOOKUP_CONFIG(color_status_slots, slot);
}

ایستا صحیح git_status_config(ثابت نویسه *k, ثابت نویسه *v,
			     ثابت ساختار config_context *ctx, تهی *cb)
{
	ساختار wt_status *s = cb;
	ثابت نویسه *slot_name;

	اگر (starts_with(k, "column."))
		بازگشت git_column_config(k, v, "status", &s->colopts);
	اگر (!strcmp(k, "status.submodulesummary")) {
		صحیح is_bool;
		s->submodule_summary = git_config_bool_or_int(k, v, ctx->kvi,
							      &is_bool);
		اگر (is_bool && s->submodule_summary)
			s->submodule_summary = -1;
		بازگشت 0;
	}
	اگر (!strcmp(k, "status.short")) {
		اگر (git_config_bool(k, v))
			status_deferred_config.status_format = STATUS_FORMAT_SHORT;
		وگرنه
			status_deferred_config.status_format = STATUS_FORMAT_NONE;
		بازگشت 0;
	}
	اگر (!strcmp(k, "status.branch")) {
		status_deferred_config.show_branch = git_config_bool(k, v);
		بازگشت 0;
	}
	اگر (!strcmp(k, "status.aheadbehind")) {
		status_deferred_config.ahead_behind = git_config_bool(k, v);
		بازگشت 0;
	}
	اگر (!strcmp(k, "status.showstash")) {
		s->show_stash = git_config_bool(k, v);
		بازگشت 0;
	}
	اگر (!strcmp(k, "status.color") || !strcmp(k, "color.status")) {
		s->use_color = git_config_colorbool(k, v);
		بازگشت 0;
	}
	اگر (!strcmp(k, "status.displaycommentprefix")) {
		s->display_comment_prefix = git_config_bool(k, v);
		بازگشت 0;
	}
	اگر (skip_prefix(k, "status.color.", &slot_name) ||
	    skip_prefix(k, "color.status.", &slot_name)) {
		صحیح slot = parse_status_slot(slot_name);
		اگر (slot < 0)
			بازگشت 0;
		اگر (!v)
			بازگشت config_error_nonbool(k);
		بازگشت color_parse(v, s->color_palette[slot]);
	}
	اگر (!strcmp(k, "status.relativepaths")) {
		s->relative_paths = git_config_bool(k, v);
		بازگشت 0;
	}
	اگر (!strcmp(k, "status.showuntrackedfiles")) {
		شمارش untracked_status_type u;

		u = parse_untracked_setting_name(v);
		اگر (u == SHOW_UNTRACKED_FILES_ERROR)
			بازگشت error(_("Invalid untracked files mode '%s'"), v);
		s->show_untracked_files = u;
		بازگشت 0;
	}
	اگر (!strcmp(k, "diff.renamelimit")) {
		اگر (s->rename_limit == -1)
			s->rename_limit = git_config_int(k, v, ctx->kvi);
		بازگشت 0;
	}
	اگر (!strcmp(k, "status.renamelimit")) {
		s->rename_limit = git_config_int(k, v, ctx->kvi);
		بازگشت 0;
	}
	اگر (!strcmp(k, "diff.renames")) {
		اگر (s->detect_rename == -1)
			s->detect_rename = git_config_rename(k, v);
		بازگشت 0;
	}
	اگر (!strcmp(k, "status.renames")) {
		s->detect_rename = git_config_rename(k, v);
		بازگشت 0;
	}
	بازگشت git_diff_ui_config(k, v, ctx, پوچ);
}

صحیح cmd_status(صحیح argc,
ثابت نویسه **argv,
ثابت نویسه *prefix,
ساختار repository *repo UNUSED)
{
	ایستا صحیح no_renames = -1;
	ایستا ثابت نویسه *rename_score_arg = (ثابت نویسه *)-1;
	ایستا ساختار wt_status s;
	unsigned صحیح progress_flag = 0;
	صحیح fd;
	ساختار object_id oid;
	ایستا ساختار option builtin_status_options[] = {
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
			.مقدار = &untracked_files_arg,
			.argh = N_("mode"),
			.help = N_("show untracked files, optional modes: all, normal, no. (Default: all)"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t)"all",
		},
		{
			.type = OPTION_STRING,
			.long_name = "ignored",
			.مقدار = &ignored_arg,
			.argh = N_("mode"),
			.help = N_("show ignored files, optional modes: traditional, matching, no. (Default: traditional)"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t)"traditional",
		},
		{
			.type = OPTION_STRING,
			.long_name = "ignore-submodules",
			.مقدار = &ignore_submodule_arg,
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

	اگر (s.show_ignored_mode == SHOW_MATCHING_IGNORED &&
	    s.show_untracked_files == SHOW_NO_UNTRACKED_FILES)
		die(_("Unsupported combination of ignored and untracked-files arguments"));

	parse_pathspec(&s.pathspec, 0,
		       PATHSPEC_PREFER_FULL,
		       prefix, argv);

	اگر (status_format != STATUS_FORMAT_PORCELAIN &&
	    status_format != STATUS_FORMAT_PORCELAIN_V2)
		progress_flag = REFRESH_PROGRESS;
	repo_read_index(the_repository);
	refresh_index(the_repository->index,
		      REFRESH_QUIET|REFRESH_UNMERGED|progress_flag,
		      &s.pathspec, پوچ, پوچ);

	اگر (use_optional_locks())
		fd = repo_hold_locked_index(the_repository, &index_lock, 0);
	وگرنه
		fd = -1;

	s.is_initial = repo_get_oid(the_repository, s.reference, &oid) ? 1 : 0;
	اگر (!s.is_initial)
		oidcpy(&s.oid_commit, &oid);

	s.ignore_submodule_arg = ignore_submodule_arg;
	s.status_format = status_format;
	s.verbose = verbose;
	اگر (no_renames != -1)
		s.detect_rename = !no_renames;
	اگر ((intptr_t)rename_score_arg != -1) {
		اگر (s.detect_rename < DIFF_DETECT_RENAME)
			s.detect_rename = DIFF_DETECT_RENAME;
		اگر (rename_score_arg)
			s.rename_score = parse_rename_score(&rename_score_arg);
	}

	wt_status_collect(&s);

	اگر (0 <= fd)
		repo_update_index_if_able(the_repository, &index_lock);

	اگر (s.relative_paths)
		s.prefix = prefix;

	wt_status_print(&s);
	wt_status_collect_free_buffers(&s);

	بازگشت 0;
}

ایستا صحیح git_commit_config(ثابت نویسه *k, ثابت نویسه *v,
			     ثابت ساختار config_context *ctx, تهی *cb)
{
	ساختار wt_status *s = cb;

	اگر (!strcmp(k, "commit.template"))
		بازگشت git_config_pathname(&template_file, k, v);
	اگر (!strcmp(k, "commit.status")) {
		include_status = git_config_bool(k, v);
		بازگشت 0;
	}
	اگر (!strcmp(k, "commit.cleanup")) {
		FREE_AND_NULL(cleanup_config);
		بازگشت git_config_string(&cleanup_config, k, v);
	}
	اگر (!strcmp(k, "commit.gpgsign")) {
		sign_commit = git_config_bool(k, v) ? "" : پوچ;
		بازگشت 0;
	}
	اگر (!strcmp(k, "commit.verbose")) {
		صحیح is_bool;
		config_commit_verbose = git_config_bool_or_int(k, v, ctx->kvi,
							       &is_bool);
		بازگشت 0;
	}

	بازگشت git_status_config(k, v, ctx, s);
}

صحیح cmd_commit(صحیح argc,
	       ثابت نویسه **argv,
	       ثابت نویسه *prefix,
	       ساختار repository *repo UNUSED)
{
	ایستا ساختار wt_status s;
	ایستا ثابت نویسه *cleanup_arg = پوچ;
	ایستا ساختار option builtin_commit_options[] = {
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
			.مقدار = &sign_commit,
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
			.مقدار = &untracked_files_arg,
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

	ساختار strbuf sb = STRBUF_INIT;
	ساختار strbuf author_ident = STRBUF_INIT;
	ثابت نویسه *index_file, *reflog_پیام;
	ساختار object_id oid;
	ساختار commit_list *parents = پوچ;
	ساختار stat statbuf;
	ساختار commit *current_head = پوچ;
	ساختار commit_extra_header *extra = پوچ;
	ساختار strbuf err = STRBUF_INIT;
	صحیح ret = 0;

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

	اگر (repo_get_oid(the_repository, "HEAD", &oid))
		current_head = پوچ;
	وگرنه {
		current_head = lookup_commit_or_die(&oid, "HEAD");
		اگر (repo_parse_commit(the_repository, current_head))
			die(_("could not parse HEAD commit"));
	}
	verbose = -1; /* unspecified */
	argc = parse_and_validate_options(argc, argv, builtin_commit_options,
					  builtin_commit_usage,
					  prefix, current_head, &s);
	اگر (trailer_args.nr)
		trailer_config_init();

	اگر (verbose == -1)
		verbose = (config_commit_verbose < 0) ? 0 : config_commit_verbose;

	اگر (cleanup_arg) {
		آزاد(cleanup_config);
		cleanup_config = xstrdup(cleanup_arg);
	}
	cleanup_mode = get_cleanup_mode(cleanup_config, use_editor);

	اگر (dry_run)
		بازگشت dry_run_commit(argv, prefix, current_head, &s);
	index_file = prepare_index(argv, prefix, current_head, 0);

	/* Set up everything for writing the commit object.  This includes
	   running hooks, writing the trees, and interacting with the user.  */
	اگر (!prepare_to_commit(index_file, prefix,
			       current_head, &s, &author_ident)) {
		ret = 1;
		rollback_index_files();
		برو cleanup;
	}

	/* Determine parents */
	reflog_پیام = getenv("GIT_REFLOG_ACTION");
	اگر (!current_head) {
		اگر (!reflog_پیام)
			reflog_پیام = "commit (initial)";
	} وگرنه اگر (amend) {
		اگر (!reflog_پیام)
			reflog_پیام = "commit (amend)";
		parents = commit_list_copy(current_head->parents);
	} وگرنه اگر (whence == FROM_MERGE) {
		ساختار strbuf m = STRBUF_INIT;
		FILE *fp;
		صحیح allow_fast_forward = 1;
		ساختار commit_list **pptr = &parents;

		اگر (!reflog_پیام)
			reflog_پیام = "commit (merge)";
		pptr = commit_list_append(current_head, pptr);
		fp = xfopen(git_path_merge_head(the_repository), "r");
		وقتی (strbuf_getline_lf(&m, fp) != EOF) {
			ساختار commit *parent;

			parent = get_merge_parent(m.buf);
			اگر (!parent)
				die(_("Corrupt MERGE_HEAD file (%s)"), m.buf);
			pptr = commit_list_append(parent, pptr);
		}
		fclose(fp);
		strbuf_release(&m);
		اگر (!stat(git_path_merge_mode(the_repository), &statbuf)) {
			اگر (strbuf_read_file(&sb, git_path_merge_mode(the_repository), 0) < 0)
				die_errno(_("could not read MERGE_MODE"));
			اگر (!strcmp(sb.buf, "no-ff"))
				allow_fast_forward = 0;
		}
		اگر (allow_fast_forward)
			reduce_heads_replace(&parents);
	} وگرنه {
		اگر (!reflog_پیام)
			reflog_پیام = is_from_cherry_pick(whence)
					? "commit (cherry-pick)"
					: is_from_rebase(whence)
					? "commit (rebase)"
					: "commit";
		commit_list_درج(current_head, &parents);
	}

	/* Finally, get the commit message */
	strbuf_reset(&sb);
	اگر (strbuf_read_file(&sb, git_path_commit_editmsg(), 0) < 0) {
		صحیح saved_errno = errno;
		rollback_index_files();
		die(_("could not read commit message: %s"), strerror(saved_errno));
	}

	cleanup_message(&sb, cleanup_mode, verbose);

	اگر (message_is_empty(&sb, cleanup_mode) && !allow_empty_message) {
		rollback_index_files();
		fprintf(stderr, _("Aborting commit due to empty commit message.\n"));
		exit(1);
	}
	اگر (template_untouched(&sb, template_file, cleanup_mode) && !allow_empty_message) {
		rollback_index_files();
		fprintf(stderr, _("Aborting commit; you did not edit the message.\n"));
		exit(1);
	}

	اگر (fixup_message && starts_with(sb.buf, "amend! ") &&
	    !allow_empty_message) {
		ساختار strbuf body = STRBUF_INIT;
		size_t len = commit_subject_length(sb.buf);
		strbuf_addstr(&body, sb.buf + len);
		اگر (message_is_empty(&body, cleanup_mode)) {
			rollback_index_files();
			fprintf(stderr, _("Aborting commit due to empty commit message body.\n"));
			exit(1);
		}
		strbuf_release(&body);
	}

	اگر (amend) {
		ثابت نویسه *exclude_gpgsig[3] = { "gpgsig", "gpgsig-sha256", پوچ };
		extra = read_commit_extra_headers(current_head, exclude_gpgsig);
	} وگرنه {
		ساختار commit_extra_header **tail = &extra;
		append_merge_tag_headers(parents, &tail);
	}

	اگر (commit_درخت_extended(sb.buf, sb.len, &the_repository->index->cache_درخت->oid,
				 parents, &oid, author_ident.buf, پوچ,
				 sign_commit, extra)) {
		rollback_index_files();
		die(_("failed to write commit object"));
	}

	اگر (update_head_with_reflog(current_head, &oid, reflog_پیام, &sb,
				    &err)) {
		rollback_index_files();
		die("%s", err.buf);
	}

	sequencer_post_commit_cleanup(the_repository, 0);
	unlink(git_path_merge_head(the_repository));
	unlink(git_path_merge_پیام(the_repository));
	unlink(git_path_merge_mode(the_repository));
	unlink(git_path_squash_پیام(the_repository));

	اگر (commit_index_files())
		die(_("repository has been updated, but unable to write\n"
		      "new index file. Check that disk is not full and quota is\n"
		      "not exceeded, and then \"git restore --staged :/\" to recover."));

	git_test_write_commit_graph_or_die(the_repository);

	repo_rerere(the_repository, 0);
	run_auto_maintenance(the_repository, quiet);
	run_commit_hook(use_editor, repo_get_index_file(the_repository),
			پوچ, "post-commit", پوچ);
	اگر (amend && !no_post_rewrite) {
		commit_post_rewrite(the_repository, current_head, &oid);
	}
	اگر (!quiet) {
		unsigned صحیح flags = 0;

		اگر (!current_head)
			flags |= SUMMARY_INITIAL_COMMIT;
		اگر (author_date_is_interesting())
			flags |= SUMMARY_SHOW_AUTHOR_DATE;
		print_commit_summary(the_repository, prefix,
				     &oid, flags);
	}

	apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
			    پوچ, پوچ, پوچ, پوچ);

cleanup:
	free_commit_extra_headers(extra);
	commit_list_free(parents);
	strbuf_release(&author_ident);
	strbuf_release(&err);
	strbuf_release(&sb);
	آزاد(logfile);
	آزاد(template_file);
	بازگشت ret;
}
