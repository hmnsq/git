/*
 * Builtin "git commit"
 *
 * Copyright (c) 2007 Kristian Høgsberg <krh@redhat.com>
 * Based on git-commit.sh by Junio C Hamano and Linus Torvalds
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ضمن "builtin.h"
#ضمن "advice.h"
#ضمن "config.h"
#ضمن "lockfile.h"
#ضمن "cache-tree.h"
#ضمن "color.h"
#ضمن "dir.h"
#ضمن "editor.h"
#ضمن "environment.h"
#ضمن "diff.h"
#ضمن "commit.h"
#ضمن "add-interactive.h"
#ضمن "gettext.h"
#ضمن "revision.h"
#ضمن "wt-status.h"
#ضمن "run-command.h"
#ضمن "strbuf.h"
#ضمن "object-name.h"
#ضمن "parse-options.h"
#ضمن "path.h"
#ضمن "preload-index.h"
#ضمن "read-cache.h"
#ضمن "repository.h"
#ضمن "string-list.h"
#ضمن "rerere.h"
#ضمن "unpack-trees.h"
#ضمن "column.h"
#ضمن "sequencer.h"
#ضمن "sparse-index.h"
#ضمن "mailmap.h"
#ضمن "help.h"
#ضمن "commit-reach.h"
#ضمن "commit-graph.h"
#ضمن "pretty.h"
#ضمن "trailer.h"

ساكن ثابت حرف * ثابت builtin_commit_usage[] = {
	N_("git commit [-a | --interactive | --patch] [-s] [-v] [-u[<mode>]] [--amend]\n"
	   "           [--dry-run] [(-c | -C | --squash) <commit> | --fixup [(amend|reword):]<commit>]\n"
	   "           [-F <file> | -m <msg>] [--reset-author] [--allow-empty]\n"
	   "           [--allow-empty-message] [--no-verify] [-e] [--author=<author>]\n"
	   "           [--date=<date>] [--cleanup=<mode>] [--[no-]status]\n"
	   "           [-i | -o] [--pathspec-from-file=<file> [--pathspec-file-nul]]\n"
	   "           [(--trailer <token>[(=|:)<value>])...] [-S[<keyid>]]\n"
	   "           [--] [<pathspec>...]"),
	عدم
};

ساكن ثابت حرف * ثابت builtin_status_usage[] = {
	N_("git status [<options>] [--] [<pathspec>...]"),
	عدم
};

ساكن ثابت حرف empty_amend_advice[] =
N_("You asked to amend the most recent commit, but doing so would make\n"
"it empty. You can repeat your command with --allow-empty, or you can\n"
"remove the commit entirely with \"git reset HEAD^\".\n");

ساكن ثابت حرف empty_cherry_pick_advice[] =
N_("The previous cherry-pick is now empty, possibly due to conflict resolution.\n"
"If you wish to commit it anyway, use:\n"
"\n"
"    git commit --allow-empty\n"
"\n");

ساكن ثابت حرف empty_rebase_pick_advice[] =
N_("Otherwise, please use 'git rebase --skip'\n");

ساكن ثابت حرف empty_cherry_pick_advice_single[] =
N_("Otherwise, please use 'git cherry-pick --skip'\n");

ساكن ثابت حرف empty_cherry_pick_advice_multi[] =
N_("and then use:\n"
"\n"
"    git cherry-pick --continue\n"
"\n"
"to resume cherry-picking the remaining commits.\n"
"If you wish to skip this commit, use:\n"
"\n"
"    git cherry-pick --skip\n"
"\n");

ساكن ثابت حرف *color_status_slots[] = {
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

ساكن ثابت حرف *use_message_buffer;
ساكن بنية lock_file index_lock; /* real index */
ساكن بنية lock_file false_lock; /* used only for partial commits */
ساكن تعداد {
	COMMIT_AS_IS = 1,
	COMMIT_NORMAL,
	COMMIT_PARTIAL
} commit_style;

ساكن ثابت حرف *force_author;
ساكن حرف *logfile;
ساكن حرف *template_file;
/*
 * The _message variables are commit names from which to take
 * the commit message and/or authorship.
 */
ساكن ثابت حرف *author_message, *author_message_buffer;
ساكن ثابت حرف *edit_message, *use_message;
ساكن حرف *fixup_message, *fixup_commit, *squash_message;
ساكن ثابت حرف *fixup_prefix;
ساكن صحيح all, also, interactive, patch_interactive, only, amend, signoff;
ساكن بنية interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
ساكن صحيح edit_flag = -1; /* unspecified */
ساكن صحيح quiet, verbose, no_verify, allow_empty, dry_run, renew_authorship;
ساكن صحيح config_commit_verbose = -1; /* unspecified */
ساكن صحيح no_post_rewrite, allow_empty_message, pathspec_file_nul;
ساكن ثابت حرف *untracked_files_arg, *force_date, *ignore_submodule_arg, *ignored_arg;
ساكن ثابت حرف *sign_commit, *pathspec_from_file;
ساكن بنية strvec trailer_args = STRVEC_INIT;

/*
 * The default commit message cleanup mode will remove the lines
 * beginning with # (shell comments) and leading and trailing
 * whitespaces (empty lines or containing only whitespaces)
 * if editor is used, and only the whitespaces if the message
 * is specified explicitly.
 */
ساكن تعداد commit_msg_cleanup_mode cleanup_mode;
ساكن حرف *cleanup_config;

ساكن تعداد commit_whence whence;
ساكن صحيح use_editor = 1, include_status = 1;
ساكن صحيح have_option_m;
ساكن بنية strbuf message = STRBUF_INIT;

ساكن تعداد wt_status_format status_format = STATUS_FORMAT_UNSPECIFIED;

ساكن صحيح opt_parse_porcelain(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	تعداد wt_status_format *value = (تعداد wt_status_format *)opt->value;
	𐤀𐤌 (unset)
		*value = STATUS_FORMAT_NONE;
	وإلا 𐤀𐤌 (!arg)
		*value = STATUS_FORMAT_PORCELAIN;
	وإلا 𐤀𐤌 (!strcmp(arg, "v1") || !strcmp(arg, "1"))
		*value = STATUS_FORMAT_PORCELAIN;
	وإلا 𐤀𐤌 (!strcmp(arg, "v2") || !strcmp(arg, "2"))
		*value = STATUS_FORMAT_PORCELAIN_V2;
	وإلا
		die("unsupported porcelain version '%s'", arg);

	أرجع 0;
}

ساكن صحيح opt_parse_m(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	بنية strbuf *buf = opt->value;
	𐤀𐤌 (unset) {
		have_option_m = 0;
		strbuf_setlen(buf, 0);
	} وإلا {
		have_option_m = 1;
		𐤀𐤌 (buf->len)
			strbuf_addch(buf, '\n');
		strbuf_addstr(buf, arg);
		strbuf_complete_line(buf);
	}
	أرجع 0;
}

ساكن صحيح opt_parse_rename_score(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	ثابت حرف **value = opt->value;

	BUG_ON_OPT_NEG(unset);

	𐤀𐤌 (arg != عدم && *arg == '=')
		arg = arg + 1;

	*value = arg;
	أرجع 0;
}

ساكن فراغ determine_whence(بنية wt_status *s)
{
	𐤀𐤌 (file_exists(git_path_merge_head(the_repository)))
		whence = FROM_MERGE;
	وإلا 𐤀𐤌 (!sequencer_determine_whence(the_repository, &whence))
		whence = FROM_COMMIT;
	𐤀𐤌 (s)
		s->whence = whence;
}

ساكن فراغ status_init_config(بنية wt_status *s, config_fn_t fn)
{
	wt_status_prepare(the_repository, s);
	init_diff_ui_defaults();
	repo_config(the_repository, fn, s);
	determine_whence(s);
	s->hints = advice_enabled(ADVICE_STATUS_HINTS); /* must come after repo_config() */
}

ساكن فراغ rollback_index_files(فراغ)
{
	بدل (commit_style) {
	حالة COMMIT_AS_IS:
		اكسر; /* nothing to do */
	حالة COMMIT_NORMAL:
		rollback_lock_file(&index_lock);
		اكسر;
	حالة COMMIT_PARTIAL:
		rollback_lock_file(&index_lock);
		rollback_lock_file(&false_lock);
		اكسر;
	}
}

ساكن صحيح commit_index_files(فراغ)
{
	صحيح err = 0;

	بدل (commit_style) {
	حالة COMMIT_AS_IS:
		اكسر; /* nothing to do */
	حالة COMMIT_NORMAL:
		err = commit_lock_file(&index_lock);
		اكسر;
	حالة COMMIT_PARTIAL:
		err = commit_lock_file(&index_lock);
		rollback_lock_file(&false_lock);
		اكسر;
	}

	أرجع err;
}

/*
 * Take a union of paths in the index and the named tree (typically, "HEAD"),
 * and return the paths that match the given pattern in list.
 */
ساكن صحيح list_paths(بنية string_list *list, ثابت حرف *with_tree,
		      ثابت بنية pathspec *pattern)
{
	صحيح i, ret;
	حرف *m;

	𐤀𐤌 (!pattern->nr)
		أرجع 0;

	m = xcalloc(1, pattern->nr);

	𐤀𐤌 (with_tree) {
		حرف *max_prefix = common_prefix(pattern);
		overlay_tree_on_index(the_repository->index, with_tree, max_prefix);
		حرر(max_prefix);
	}

	/* TODO: audit for interaction with sparse-index. */
	ensure_full_index(the_repository->index);
	لكل (i = 0; i < the_repository->index->cache_nr; i++) {
		ثابت بنية cache_entry *ce = the_repository->index->cache[i];
		بنية string_list_item *item;

		𐤀𐤌 (ce->ce_flags & CE_UPDATE)
			استمر;
		𐤀𐤌 (!ce_path_match(the_repository->index, ce, pattern, m))
			استمر;
		item = string_list_insert(list, ce->name);
		𐤀𐤌 (ce_skip_worktree(ce))
			item->util = item; /* better a valid pointer than a fake one */
	}

	ret = report_path_error(m, pattern);
	حرر(m);
	أرجع ret;
}

ساكن فراغ add_remove_files(بنية string_list *list)
{
	صحيح i;
	لكل (i = 0; i < list->nr; i++) {
		بنية stat st;
		بنية string_list_item *p = &(list->items[i]);

		/* p->util is skip-worktree */
		𐤀𐤌 (p->util)
			استمر;

		𐤀𐤌 (!lstat(p->string, &st)) {
			𐤀𐤌 (add_to_index(the_repository->index, p->string, &st, 0))
				die(_("updating files failed"));
		} وإلا
			remove_file_from_index(the_repository->index, p->string);
	}
}

ساكن فراغ create_base_index(ثابت بنية commit *current_head)
{
	بنية tree *tree;
	بنية unpack_trees_options opts;
	بنية tree_desc t;

	𐤀𐤌 (!current_head) {
		discard_index(the_repository->index);
		أرجع;
	}

	memset(&opts, 0, حجم(opts));
	opts.head_idx = 1;
	opts.index_only = 1;
	opts.merge = 1;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;

	opts.fn = oneway_merge;
	tree = repo_parse_tree_indirect(the_repository,
					&current_head->object.oid);
	𐤀𐤌 (!tree)
		die(_("failed to unpack HEAD tree object"));
	𐤀𐤌 (repo_parse_tree(the_repository, tree) < 0)
		exit(128);
	init_tree_desc(&t, &tree->object.oid, tree->buffer, tree->size);
	𐤀𐤌 (unpack_trees(1, &t, &opts))
		exit(128); /* We've already reported the error, finish dying */
}

ساكن فراغ refresh_cache_or_die(صحيح refresh_flags)
{
	/*
	 * refresh_flags contains REFRESH_QUIET, so the only errors
	 * are for unmerged entries.
	 */
	𐤀𐤌 (refresh_index(the_repository->index, refresh_flags | REFRESH_IN_PORCELAIN, عدم, عدم, عدم))
		die_resolve_conflict("commit");
}

ساكن ثابت حرف *prepare_index(ثابت حرف **argv, ثابت حرف *prefix,
				 ثابت بنية commit *current_head, صحيح is_status)
{
	بنية string_list partial = STRING_LIST_INIT_DUP;
	بنية pathspec pathspec;
	صحيح refresh_flags = REFRESH_QUIET;
	ثابت حرف *ret;
	حرف *path = عدم;

	𐤀𐤌 (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	𐤀𐤌 (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	𐤀𐤌 (is_status)
		refresh_flags |= REFRESH_UNMERGED;
	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_FULL,
		       prefix, argv);

	𐤀𐤌 (pathspec_from_file) {
		𐤀𐤌 (interactive)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--interactive/--patch");

		𐤀𐤌 (all)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "-a");

		𐤀𐤌 (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_FULL,
				    prefix, pathspec_from_file, pathspec_file_nul);
	} وإلا 𐤀𐤌 (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	𐤀𐤌 (!pathspec.nr && (also || (only && !allow_empty &&
	    (!amend || (fixup_message && strcmp(fixup_prefix, "amend"))))))
		die(_("No paths with --include/--only does not make sense."));

	𐤀𐤌 (repo_read_index_preload(the_repository, &pathspec, 0) < 0)
		die(_("index file corrupt"));

	𐤀𐤌 (interactive) {
		حرف *old_index_env = عدم, *old_repo_index_file;
		repo_hold_locked_index(the_repository, &index_lock,
				       LOCK_DIE_ON_ERROR);

		refresh_cache_or_die(refresh_flags);

		𐤀𐤌 (write_locked_index(the_repository->index, &index_lock, 0))
			die(_("unable to create temporary index"));

		old_repo_index_file = the_repository->index_file;
		the_repository->index_file =
			(حرف *)get_lock_file_path(&index_lock);
		old_index_env = xstrdup_or_null(getenv(INDEX_ENVIRONMENT));
		setenv(INDEX_ENVIRONMENT, the_repository->index_file, 1);

		𐤀𐤌 (interactive_add(the_repository, argv, prefix, patch_interactive, &interactive_opts) != 0)
			die(_("interactive add failed"));

		the_repository->index_file = old_repo_index_file;
		𐤀𐤌 (old_index_env && *old_index_env)
			setenv(INDEX_ENVIRONMENT, old_index_env, 1);
		وإلا
			unsetenv(INDEX_ENVIRONMENT);
		FREE_AND_NULL(old_index_env);

		discard_index(the_repository->index);
		read_index_from(the_repository->index, get_lock_file_path(&index_lock),
				repo_get_git_dir(the_repository));
		𐤀𐤌 (cache_tree_update(the_repository->index, WRITE_TREE_SILENT) == 0) {
			𐤀𐤌 (reopen_lock_file(&index_lock) < 0)
				die(_("unable to write index file"));
			𐤀𐤌 (write_locked_index(the_repository->index, &index_lock, 0))
				die(_("unable to update temporary index"));
		} وإلا
			warning(_("Failed to update main cache tree"));

		commit_style = COMMIT_NORMAL;
		ret = get_lock_file_path(&index_lock);
		اذهب out;
	} وإلا {
		𐤀𐤌 (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--interactive/--patch");
		𐤀𐤌 (interactive_opts.interhunkcontext != -1)
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
	𐤀𐤌 (all || (also && pathspec.nr)) {
		حرف *ps_matched = xcalloc(pathspec.nr, 1);
		repo_hold_locked_index(the_repository, &index_lock,
				       LOCK_DIE_ON_ERROR);
		add_files_to_cache(the_repository, also ? prefix : عدم,
				   &pathspec, ps_matched, 0, 0, 0 );
		𐤀𐤌 (!all && report_path_error(ps_matched, &pathspec))
			exit(128);

		refresh_cache_or_die(refresh_flags);
		cache_tree_update(the_repository->index, WRITE_TREE_SILENT);
		𐤀𐤌 (write_locked_index(the_repository->index, &index_lock, 0))
			die(_("unable to write new index file"));
		commit_style = COMMIT_NORMAL;
		ret = get_lock_file_path(&index_lock);
		حرر(ps_matched);
		اذهب out;
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
	𐤀𐤌 (!only && !pathspec.nr) {
		repo_hold_locked_index(the_repository, &index_lock,
				       LOCK_DIE_ON_ERROR);
		refresh_cache_or_die(refresh_flags);
		𐤀𐤌 (the_repository->index->cache_changed
		    || !cache_tree_fully_valid(the_repository->index))
			cache_tree_update(the_repository->index, WRITE_TREE_SILENT);
		𐤀𐤌 (write_locked_index(the_repository->index, &index_lock,
				       COMMIT_LOCK | SKIP_IF_UNCHANGED))
			die(_("unable to write new index file"));
		commit_style = COMMIT_AS_IS;
		ret = repo_get_index_file(the_repository);
		اذهب out;
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

	𐤀𐤌 (whence != FROM_COMMIT) {
		𐤀𐤌 (whence == FROM_MERGE)
			die(_("cannot do a partial commit during a merge."));
		وإلا 𐤀𐤌 (is_from_cherry_pick(whence))
			die(_("cannot do a partial commit during a cherry-pick."));
		وإلا 𐤀𐤌 (is_from_rebase(whence))
			die(_("cannot do a partial commit during a rebase."));
	}

	𐤀𐤌 (list_paths(&partial, !current_head ? عدم : "HEAD", &pathspec))
		exit(1);

	discard_index(the_repository->index);
	𐤀𐤌 (repo_read_index(the_repository) < 0)
		die(_("cannot read the index"));

	repo_hold_locked_index(the_repository, &index_lock, LOCK_DIE_ON_ERROR);
	add_remove_files(&partial);
	refresh_index(the_repository->index, REFRESH_QUIET, عدم, عدم, عدم);
	cache_tree_update(the_repository->index, WRITE_TREE_SILENT);
	𐤀𐤌 (write_locked_index(the_repository->index, &index_lock, 0))
		die(_("unable to write new index file"));

	path = repo_git_path(the_repository, "next-index-%"PRIuMAX,
			     (uintmax_t) getpid());
	hold_lock_file_for_update(&false_lock, path,
				  LOCK_DIE_ON_ERROR);

	create_base_index(current_head);
	add_remove_files(&partial);
	refresh_index(the_repository->index, REFRESH_QUIET, عدم, عدم, عدم);

	𐤀𐤌 (write_locked_index(the_repository->index, &false_lock, 0))
		die(_("unable to write temporary index file"));

	discard_index(the_repository->index);
	ret = get_lock_file_path(&false_lock);
	read_index_from(the_repository->index, ret, repo_get_git_dir(the_repository));
out:
	string_list_clear(&partial, 0);
	clear_pathspec(&pathspec);
	حرر(path);
	أرجع ret;
}

ساكن صحيح run_status(FILE *fp, ثابت حرف *index_file, ثابت حرف *prefix, صحيح nowarn,
		      بنية wt_status *s)
{
	بنية object_id oid;

	𐤀𐤌 (s->relative_paths)
		s->prefix = prefix;

	𐤀𐤌 (amend) {
		s->amend = 1;
		s->reference = "HEAD^1";
	}
	s->verbose = verbose;
	s->index_file = index_file;
	s->fp = fp;
	s->nowarn = nowarn;
	s->is_initial = repo_get_oid(the_repository, s->reference, &oid) ? 1 : 0;
	𐤀𐤌 (!s->is_initial)
		oidcpy(&s->oid_commit, &oid);
	s->status_format = status_format;
	s->ignore_submodule_arg = ignore_submodule_arg;

	wt_status_collect(s);
	wt_status_print(s);
	wt_status_collect_free_buffers(s);

	أرجع s->committable;
}

ساكن صحيح is_a_merge(ثابت بنية commit *current_head)
{
	أرجع !!(current_head->parents && current_head->parents->next);
}

ساكن فراغ assert_split_ident(بنية ident_split *id, ثابت بنية strbuf *buf)
{
	𐤀𐤌 (split_ident_line(id, buf->buf, buf->len) || !id->date_begin)
		BUG("unable to parse our own ident: %s", buf->buf);
}

ساكن فراغ export_one(ثابت حرف *var, ثابت حرف *s, ثابت حرف *e, صحيح hack)
{
	بنية strbuf buf = STRBUF_INIT;
	𐤀𐤌 (hack)
		strbuf_addch(&buf, hack);
	strbuf_add(&buf, s, e - s);
	setenv(var, buf.buf, 1);
	strbuf_release(&buf);
}

ساكن صحيح parse_force_date(ثابت حرف *in, بنية strbuf *out)
{
	strbuf_addch(out, '@');

	𐤀𐤌 (parse_date(in, out) < 0) {
		صحيح errors = 0;
		unsigned long t = approxidate_careful(in, &errors);
		𐤀𐤌 (errors)
			أرجع -1;
		strbuf_addf(out, "%lu", t);
	}

	أرجع 0;
}

ساكن فراغ set_ident_var(حرف **buf, حرف *val)
{
	حرر(*buf);
	*buf = val;
}

ساكن فراغ determine_author_info(بنية strbuf *author_ident)
{
	حرف *name, *email, *date;
	بنية ident_split author;

	name = xstrdup_or_null(getenv("GIT_AUTHOR_NAME"));
	email = xstrdup_or_null(getenv("GIT_AUTHOR_EMAIL"));
	date = xstrdup_or_null(getenv("GIT_AUTHOR_DATE"));

	𐤀𐤌 (author_message) {
		بنية ident_split ident;
		size_t len;
		ثابت حرف *a;

		a = find_commit_header(author_message_buffer, "author", &len);
		𐤀𐤌 (!a)
			die(_("commit '%s' lacks author header"), author_message);
		𐤀𐤌 (split_ident_line(&ident, a, len) < 0)
			die(_("commit '%s' has malformed author line"), author_message);

		set_ident_var(&name, xmemdupz(ident.name_begin, ident.name_end - ident.name_begin));
		set_ident_var(&email, xmemdupz(ident.mail_begin, ident.mail_end - ident.mail_begin));

		𐤀𐤌 (ident.date_begin) {
			بنية strbuf date_buf = STRBUF_INIT;
			strbuf_addch(&date_buf, '@');
			strbuf_add(&date_buf, ident.date_begin, ident.date_end - ident.date_begin);
			strbuf_addch(&date_buf, ' ');
			strbuf_add(&date_buf, ident.tz_begin, ident.tz_end - ident.tz_begin);
			set_ident_var(&date, strbuf_detach(&date_buf, عدم));
		}
	}

	𐤀𐤌 (force_author) {
		بنية ident_split ident;

		𐤀𐤌 (split_ident_line(&ident, force_author, strlen(force_author)) < 0)
			die(_("malformed --author parameter"));
		set_ident_var(&name, xmemdupz(ident.name_begin, ident.name_end - ident.name_begin));
		set_ident_var(&email, xmemdupz(ident.mail_begin, ident.mail_end - ident.mail_begin));
	}

	𐤀𐤌 (force_date) {
		بنية strbuf date_buf = STRBUF_INIT;
		𐤀𐤌 (parse_force_date(force_date, &date_buf))
			die(_("invalid date format: %s"), force_date);
		set_ident_var(&date, strbuf_detach(&date_buf, عدم));
	}

	strbuf_addstr(author_ident, fmt_ident(name, email, WANT_AUTHOR_IDENT, date,
				IDENT_STRICT));
	assert_split_ident(&author, author_ident);
	export_one("GIT_AUTHOR_NAME", author.name_begin, author.name_end, 0);
	export_one("GIT_AUTHOR_EMAIL", author.mail_begin, author.mail_end, 0);
	export_one("GIT_AUTHOR_DATE", author.date_begin, author.tz_end, '@');
	حرر(name);
	حرر(email);
	حرر(date);
}

ساكن صحيح author_date_is_interesting(فراغ)
{
	أرجع author_message || force_date;
}

#ifndef WITH_BREAKING_CHANGES
ساكن فراغ adjust_comment_line_char(ثابت بنية strbuf *sb)
{
	حرف candidates[] = "#;@!$%^&|:";
	حرف *candidate;
	ثابت حرف *p;
	size_t cutoff;

	/* Ignore comment chars in trailing comments (e.g., Conflicts:) */
	cutoff = sb->len - ignored_log_message_bytes(sb->buf, sb->len);

	𐤀𐤌 (!memchr(sb->buf, candidates[0], sb->len)) {
		حرر(comment_line_str_to_free);
		comment_line_str = comment_line_str_to_free =
			xstrfmt("%c", candidates[0]);
		أرجع;
	}

	p = sb->buf;
	candidate = strchr(candidates, *p);
	𐤀𐤌 (candidate)
		*candidate = ' ';
	لكل (p = sb->buf; p + 1 < sb->buf + cutoff; p++) {
		𐤀𐤌 ((p[0] == '\n' || p[0] == '\r') && p[1]) {
			candidate = strchr(candidates, p[1]);
			𐤀𐤌 (candidate)
				*candidate = ' ';
		}
	}

	لكل (p = candidates; *p == ' '; p++)
		;
	𐤀𐤌 (!*p)
		die(_("unable to select a comment character that is not used\n"
		      "in the current commit message"));
	حرر(comment_line_str_to_free);
	comment_line_str = comment_line_str_to_free = xstrfmt("%c", *p);
}
#endif /* !WITH_BREAKING_CHANGES */

ساكن فراغ prepare_amend_commit(بنية commit *commit, بنية strbuf *sb,
				بنية pretty_print_context *ctx)
{
	ثابت حرف *buffer, *subject, *fmt;

	buffer = repo_get_commit_buffer(the_repository, commit, عدم);
	find_commit_subject(buffer, &subject);
	/*
	 * If we amend the 'amend!' commit then we don't want to
	 * duplicate the subject line.
	 */
	fmt = starts_with(subject, "amend!") ? "%b" : "%B";
	repo_format_commit_message(the_repository, commit, fmt, sb, ctx);
	repo_unuse_commit_buffer(the_repository, commit, buffer);
}

ساكن فراغ change_data_free(فراغ *util, ثابت حرف *str UNUSED)
{
	بنية wt_status_change_data *d = util;
	حرر(d->rename_source);
	حرر(d);
}

ساكن صحيح prepare_to_commit(ثابت حرف *index_file, ثابت حرف *prefix,
			     بنية commit *current_head,
			     بنية wt_status *s,
			     بنية strbuf *author_ident)
{
	بنية stat statbuf;
	بنية strbuf committer_ident = STRBUF_INIT;
	صحيح committable;
	بنية strbuf sb = STRBUF_INIT;
	ثابت حرف *hook_arg1 = عدم;
	ثابت حرف *hook_arg2 = عدم;
	صحيح clean_message_contents = (cleanup_mode != COMMIT_MSG_CLEANUP_NONE);
	صحيح old_display_comment_prefix;
	صحيح invoked_hook;

	/* This checks and barfs if author is badly specified */
	determine_author_info(author_ident);

	𐤀𐤌 (!no_verify && run_commit_hook(use_editor, index_file, &invoked_hook,
					  "pre-commit", عدم))
		أرجع 0;

	𐤀𐤌 (squash_message) {
		/*
		 * Insert the proper subject line before other commit
		 * message options add their content.
		 */
		𐤀𐤌 (use_message && !strcmp(use_message, squash_message))
			strbuf_addstr(&sb, "squash! ");
		وإلا {
			بنية pretty_print_context ctx = {0};
			بنية commit *c;
			c = lookup_commit_reference_by_name(squash_message);
			𐤀𐤌 (!c)
				die(_("could not lookup commit '%s'"), squash_message);
			ctx.output_encoding = get_commit_output_encoding();
			repo_format_commit_message(the_repository, c,
						   "squash! %s\n\n", &sb,
						   &ctx);
		}
	}

	𐤀𐤌 (have_option_m && !fixup_message) {
		strbuf_addbuf(&sb, &message);
		hook_arg1 = "message";
	} وإلا 𐤀𐤌 (logfile && !strcmp(logfile, "-")) {
		𐤀𐤌 (isatty(0))
			fprintf(stderr, _("(reading log message from standard input)\n"));
		𐤀𐤌 (strbuf_read(&sb, 0, 0) < 0)
			die_errno(_("could not read log from standard input"));
		hook_arg1 = "message";
	} وإلا 𐤀𐤌 (logfile) {
		𐤀𐤌 (strbuf_read_file(&sb, logfile, 0) < 0)
			die_errno(_("could not read log file '%s'"),
				  logfile);
		hook_arg1 = "message";
	} وإلا 𐤀𐤌 (use_message) {
		ثابت حرف *buffer;
		buffer = strstr(use_message_buffer, "\n\n");
		𐤀𐤌 (buffer)
			strbuf_addstr(&sb, skip_blank_lines(buffer + 2));
		hook_arg1 = "commit";
		hook_arg2 = use_message;
	} وإلا 𐤀𐤌 (fixup_message) {
		بنية pretty_print_context ctx = {0};
		بنية commit *commit;
		حرف *fmt;
		commit = lookup_commit_reference_by_name(fixup_commit);
		𐤀𐤌 (!commit)
			die(_("could not lookup commit '%s'"), fixup_commit);
		ctx.output_encoding = get_commit_output_encoding();
		fmt = xstrfmt("%s! %%s\n\n", fixup_prefix);
		repo_format_commit_message(the_repository, commit, fmt, &sb,
					   &ctx);
		حرر(fmt);
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
		𐤀𐤌 (have_option_m && !strcmp(fixup_prefix, "fixup"))
			strbuf_addbuf(&sb, &message);

		𐤀𐤌 (!strcmp(fixup_prefix, "amend")) {
			𐤀𐤌 (have_option_m)
				die(_("options '%s' and '%s:%s' cannot be used together"), "-m", "--fixup", fixup_message);
			prepare_amend_commit(commit, &sb, &ctx);
		}
	} وإلا 𐤀𐤌 (!stat(git_path_merge_msg(the_repository), &statbuf)) {
		size_t merge_msg_start;

		/*
		 * prepend SQUASH_MSG here if it exists and a
		 * "merge --squash" was originally performed
		 */
		𐤀𐤌 (!stat(git_path_squash_msg(the_repository), &statbuf)) {
			𐤀𐤌 (strbuf_read_file(&sb, git_path_squash_msg(the_repository), 0) < 0)
				die_errno(_("could not read SQUASH_MSG"));
			hook_arg1 = "squash";
		} وإلا
			hook_arg1 = "merge";

		merge_msg_start = sb.len;
		𐤀𐤌 (strbuf_read_file(&sb, git_path_merge_msg(the_repository), 0) < 0)
			die_errno(_("could not read MERGE_MSG"));

		𐤀𐤌 (cleanup_mode == COMMIT_MSG_CLEANUP_SCISSORS &&
		    wt_status_locate_end(sb.buf + merge_msg_start,
					 sb.len - merge_msg_start) <
				sb.len - merge_msg_start)
			s->added_cut_line = 1;
	} وإلا 𐤀𐤌 (!stat(git_path_squash_msg(the_repository), &statbuf)) {
		𐤀𐤌 (strbuf_read_file(&sb, git_path_squash_msg(the_repository), 0) < 0)
			die_errno(_("could not read SQUASH_MSG"));
		hook_arg1 = "squash";
	} وإلا 𐤀𐤌 (template_file) {
		𐤀𐤌 (strbuf_read_file(&sb, template_file, 0) < 0)
			die_errno(_("could not read '%s'"), template_file);
		hook_arg1 = "template";
		clean_message_contents = 0;
	}

	/*
	 * The remaining cases don't modify the template message, but
	 * just set the argument(s) to the prepare-commit-msg hook.
	 */
	وإلا 𐤀𐤌 (whence == FROM_MERGE)
		hook_arg1 = "merge";
	وإلا 𐤀𐤌 (is_from_cherry_pick(whence) || whence == FROM_REBASE_PICK) {
		hook_arg1 = "commit";
		hook_arg2 = "CHERRY_PICK_HEAD";
	}

	𐤀𐤌 (squash_message) {
		/*
		 * If squash_commit was used for the commit subject,
		 * then we're possibly hijacking other commit log options.
		 * Reset the hook args to tell the real story.
		 */
		hook_arg1 = "message";
		hook_arg2 = "";
	}

	s->fp = fopen_for_writing(git_path_commit_editmsg());
	𐤀𐤌 (!s->fp)
		die_errno(_("could not open '%s'"), git_path_commit_editmsg());

	/* Ignore status.displayCommentPrefix: we do need comments in COMMIT_EDITMSG. */
	old_display_comment_prefix = s->display_comment_prefix;
	s->display_comment_prefix = 1;

	/*
	 * Most hints are counter-productive when the commit has
	 * already started.
	 */
	s->hints = 0;

	𐤀𐤌 (clean_message_contents)
		strbuf_stripspace(&sb, عدم);

	𐤀𐤌 (signoff)
		append_signoff(&sb, ignored_log_message_bytes(sb.buf, sb.len), 0);

	𐤀𐤌 (fwrite(sb.buf, 1, sb.len, s->fp) < sb.len)
		die_errno(_("could not write commit template"));

#ifndef WITH_BREAKING_CHANGES
	𐤀𐤌 (auto_comment_line_char)
		adjust_comment_line_char(&sb);
#endif /* !WITH_BREAKING_CHANGES */
	strbuf_release(&sb);

	/* This checks if committer ident is explicitly given */
	strbuf_addstr(&committer_ident, git_committer_info(IDENT_STRICT));
	𐤀𐤌 (use_editor && include_status) {
		صحيح ident_shown = 0;
		تعداد git_colorbool saved_color_setting;
		بنية ident_split ci, ai;
		ثابت حرف *hint_cleanup_all = allow_empty_message ?
			_("Please enter the commit message for your changes."
			  " Lines starting\nwith '%s' will be ignored.\n") :
			_("Please enter the commit message for your changes."
			  " Lines starting\nwith '%s' will be ignored, and an empty"
			  " message aborts the commit.\n");
		ثابت حرف *hint_cleanup_space = allow_empty_message ?
			_("Please enter the commit message for your changes."
			  " Lines starting\n"
			  "with '%s' will be kept; you may remove them"
			  " yourself if you want to.\n") :
			_("Please enter the commit message for your changes."
			  " Lines starting\n"
			  "with '%s' will be kept; you may remove them"
			  " yourself if you want to.\n"
			  "An empty message aborts the commit.\n");
		𐤀𐤌 (whence != FROM_COMMIT) {
			𐤀𐤌 (cleanup_mode == COMMIT_MSG_CLEANUP_SCISSORS)
				wt_status_add_cut_line(s);
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
		𐤀𐤌 (cleanup_mode == COMMIT_MSG_CLEANUP_ALL)
			status_printf(s, GIT_COLOR_NORMAL, hint_cleanup_all, comment_line_str);
		وإلا 𐤀𐤌 (cleanup_mode == COMMIT_MSG_CLEANUP_SCISSORS) {
			𐤀𐤌 (whence == FROM_COMMIT)
				wt_status_add_cut_line(s);
		} وإلا /* COMMIT_MSG_CLEANUP_SPACE, that is. */
			status_printf(s, GIT_COLOR_NORMAL, hint_cleanup_space, comment_line_str);

		/*
		 * These should never fail because they come from our own
		 * fmt_ident. They may fail the sane_ident test, but we know
		 * that the name and mail pointers will at least be valid,
		 * which is enough for our tests and printing here.
		 */
		assert_split_ident(&ai, author_ident);
		assert_split_ident(&ci, &committer_ident);

		𐤀𐤌 (ident_cmp(&ai, &ci))
			status_printf_ln(s, GIT_COLOR_NORMAL,
				_("%s"
				"Author:    %.*s <%.*s>"),
				ident_shown++ ? "" : "\n",
				(صحيح)(ai.name_end - ai.name_begin), ai.name_begin,
				(صحيح)(ai.mail_end - ai.mail_begin), ai.mail_begin);

		𐤀𐤌 (author_date_is_interesting())
			status_printf_ln(s, GIT_COLOR_NORMAL,
				_("%s"
				"Date:      %s"),
				ident_shown++ ? "" : "\n",
				show_ident_date(&ai, DATE_MODE(NORMAL)));

		𐤀𐤌 (!committer_ident_sufficiently_given())
			status_printf_ln(s, GIT_COLOR_NORMAL,
				_("%s"
				"Committer: %.*s <%.*s>"),
				ident_shown++ ? "" : "\n",
				(صحيح)(ci.name_end - ci.name_begin), ci.name_begin,
				(صحيح)(ci.mail_end - ci.mail_begin), ci.mail_begin);

		status_printf_ln(s, GIT_COLOR_NORMAL, "%s", ""); /* Add new line for clarity */

		saved_color_setting = s->use_color;
		s->use_color = GIT_COLOR_NEVER;
		committable = run_status(s->fp, index_file, prefix, 1, s);
		s->use_color = saved_color_setting;
		string_list_clear_func(&s->change, change_data_free);
	} وإلا {
		بنية object_id oid;
		ثابت حرف *parent = "HEAD";

		𐤀𐤌 (!the_repository->index->initialized && repo_read_index(the_repository) < 0)
			die(_("Cannot read index"));

		𐤀𐤌 (amend)
			parent = "HEAD^1";

		𐤀𐤌 (repo_get_oid(the_repository, parent, &oid)) {
			صحيح i, ita_nr = 0;

			/* TODO: audit for interaction with sparse-index. */
			ensure_full_index(the_repository->index);
			لكل (i = 0; i < the_repository->index->cache_nr; i++)
				𐤀𐤌 (ce_intent_to_add(the_repository->index->cache[i]))
					ita_nr++;
			committable = the_repository->index->cache_nr > ita_nr;
		} وإلا {
			/*
			 * Unless the user did explicitly request a submodule
			 * ignore mode by passing a command line option we do
			 * not ignore any changed submodule SHA-1s when
			 * comparing index and parent, no matter what is
			 * configured. Otherwise we won't commit any
			 * submodules which were manually staged, which would
			 * be really confusing.
			 */
			بنية diff_flags flags = DIFF_FLAGS_INIT;
			flags.override_submodule_config = 1;
			𐤀𐤌 (ignore_submodule_arg &&
			    !strcmp(ignore_submodule_arg, "all"))
				flags.ignore_submodules = 1;
			committable = index_differs_from(the_repository,
							 parent, &flags, 1);
		}
	}
	strbuf_release(&committer_ident);

	fclose(s->fp);

	𐤀𐤌 (trailer_args.nr) {
		𐤀𐤌 (amend_file_with_trailers(git_path_commit_editmsg(), &trailer_args))
			die(_("unable to pass trailers to --trailers"));
		strvec_clear(&trailer_args);
	}

	/*
	 * Reject an attempt to record a non-merge empty commit without
	 * explicit --allow-empty. In the cherry-pick case, it may be
	 * empty due to conflict resolution, which the user should okay.
	 */
	𐤀𐤌 (!committable && whence != FROM_MERGE && !allow_empty &&
	    !(amend && is_a_merge(current_head))) {
		s->hints = advice_enabled(ADVICE_STATUS_HINTS);
		s->display_comment_prefix = old_display_comment_prefix;
		run_status(stdout, index_file, prefix, 0, s);
		𐤀𐤌 (amend)
			fputs(_(empty_amend_advice), stderr);
		وإلا 𐤀𐤌 (is_from_cherry_pick(whence) ||
			 whence == FROM_REBASE_PICK) {
			fputs(_(empty_cherry_pick_advice), stderr);
			𐤀𐤌 (whence == FROM_CHERRY_PICK_SINGLE)
				fputs(_(empty_cherry_pick_advice_single), stderr);
			وإلا 𐤀𐤌 (whence == FROM_CHERRY_PICK_MULTI)
				fputs(_(empty_cherry_pick_advice_multi), stderr);
			وإلا
				fputs(_(empty_rebase_pick_advice), stderr);
		}
		أرجع 0;
	}

	𐤀𐤌 (!no_verify && invoked_hook) {
		/*
		 * Re-read the index as the pre-commit-commit hook was invoked
		 * and could have updated it. We must do this before we invoke
		 * the editor and after we invoke run_status above.
		 */
		discard_index(the_repository->index);
	}
	read_index_from(the_repository->index, index_file, repo_get_git_dir(the_repository));

	𐤀𐤌 (cache_tree_update(the_repository->index, 0)) {
		error(_("Error building trees"));
		أرجع 0;
	}

	𐤀𐤌 (run_commit_hook(use_editor, index_file, عدم, "prepare-commit-msg",
			    git_path_commit_editmsg(), hook_arg1, hook_arg2, عدم))
		أرجع 0;

	𐤀𐤌 (use_editor) {
		بنية strvec env = STRVEC_INIT;

		strvec_pushf(&env, "GIT_INDEX_FILE=%s", index_file);
		𐤀𐤌 (launch_editor(git_path_commit_editmsg(), عدم, env.v)) {
			fprintf(stderr,
			_("Please supply the message using either -m or -F option.\n"));
			exit(1);
		}
		strvec_clear(&env);
	}

	𐤀𐤌 (!no_verify &&
	    run_commit_hook(use_editor, index_file, عدم, "commit-msg",
			    git_path_commit_editmsg(), عدم)) {
		أرجع 0;
	}

	أرجع 1;
}

ساكن ثابت حرف *find_author_by_nickname(ثابت حرف *name)
{
	بنية rev_info revs;
	بنية commit *commit;
	بنية strbuf buf = STRBUF_INIT;
	ثابت حرف *av[20];
	صحيح ac = 0;

	repo_init_revisions(the_repository, &revs, عدم);
	strbuf_addf(&buf, "--author=%s", name);
	av[++ac] = "--all";
	av[++ac] = "-i";
	av[++ac] = buf.buf;
	av[++ac] = عدم;
	setup_revisions(ac, av, &revs, عدم);
	revs.mailmap = xmalloc(حجم(بنية string_list));
	string_list_init_nodup(revs.mailmap);
	read_mailmap(the_repository, revs.mailmap);

	𐤀𐤌 (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	commit = get_revision(&revs);
	𐤀𐤌 (commit) {
		بنية pretty_print_context ctx = {0};
		ctx.date_mode.type = DATE_NORMAL;
		strbuf_release(&buf);
		repo_format_commit_message(the_repository, commit,
					   "%aN <%aE>", &buf, &ctx);
		release_revisions(&revs);
		أرجع strbuf_detach(&buf, عدم);
	}
	die(_("--author '%s' is not 'Name <email>' and matches no existing author"), name);
}

ساكن فراغ handle_ignored_arg(بنية wt_status *s)
{
	𐤀𐤌 (!ignored_arg)
		; /* default already initialized */
	وإلا 𐤀𐤌 (!strcmp(ignored_arg, "traditional"))
		s->show_ignored_mode = SHOW_TRADITIONAL_IGNORED;
	وإلا 𐤀𐤌 (!strcmp(ignored_arg, "no"))
		s->show_ignored_mode = SHOW_NO_IGNORED;
	وإلا 𐤀𐤌 (!strcmp(ignored_arg, "matching"))
		s->show_ignored_mode = SHOW_MATCHING_IGNORED;
	وإلا
		die(_("Invalid ignored mode '%s'"), ignored_arg);
}

ساكن تعداد untracked_status_type parse_untracked_setting_name(ثابت حرف *u)
{
	/*
	 * Please update $__git_untracked_file_modes in
	 * git-completion.bash when you add new options
	 */
	بدل (git_parse_maybe_bool(u)) {
	حالة 0:
		u = "no";
		اكسر;
	حالة 1:
		u = "normal";
		اكسر;
	افتراضي:
		اكسر;
	}

	𐤀𐤌 (!strcmp(u, "no"))
		أرجع SHOW_NO_UNTRACKED_FILES;
	وإلا 𐤀𐤌 (!strcmp(u, "normal"))
		أرجع SHOW_NORMAL_UNTRACKED_FILES;
	وإلا 𐤀𐤌 (!strcmp(u, "all"))
		أرجع SHOW_ALL_UNTRACKED_FILES;
	وإلا
		أرجع SHOW_UNTRACKED_FILES_ERROR;
}

ساكن فراغ handle_untracked_files_arg(بنية wt_status *s)
{
	تعداد untracked_status_type u;

	𐤀𐤌 (!untracked_files_arg)
		أرجع; /* default already initialized */

	u = parse_untracked_setting_name(untracked_files_arg);
	𐤀𐤌 (u == SHOW_UNTRACKED_FILES_ERROR)
		die(_("Invalid untracked files mode '%s'"),
		    untracked_files_arg);
	s->show_untracked_files = u;
}

ساكن ثابت حرف *read_commit_message(ثابت حرف *name)
{
	ثابت حرف *out_enc;
	بنية commit *commit;

	commit = lookup_commit_reference_by_name(name);
	𐤀𐤌 (!commit)
		die(_("could not lookup commit '%s'"), name);
	out_enc = get_commit_output_encoding();
	أرجع repo_logmsg_reencode(the_repository, commit, عدم, out_enc);
}

/*
 * Enumerate what needs to be propagated when --porcelain
 * is not in effect here.
 */
ساكن بنية status_deferred_config {
	تعداد wt_status_format status_format;
	صحيح show_branch;
	تعداد ahead_behind_flags ahead_behind;
} status_deferred_config = {
	STATUS_FORMAT_UNSPECIFIED,
	-1, /* unspecified */
	AHEAD_BEHIND_UNSPECIFIED,
};

ساكن فراغ finalize_deferred_config(بنية wt_status *s)
{
	صحيح use_deferred_config = (status_format != STATUS_FORMAT_PORCELAIN &&
				   status_format != STATUS_FORMAT_PORCELAIN_V2 &&
				   !s->null_termination);

	𐤀𐤌 (s->null_termination) {
		𐤀𐤌 (status_format == STATUS_FORMAT_NONE ||
		    status_format == STATUS_FORMAT_UNSPECIFIED)
			status_format = STATUS_FORMAT_PORCELAIN;
		وإلا 𐤀𐤌 (status_format == STATUS_FORMAT_LONG)
			die(_("options '%s' and '%s' cannot be used together"), "--long", "-z");
	}

	𐤀𐤌 (use_deferred_config && status_format == STATUS_FORMAT_UNSPECIFIED)
		status_format = status_deferred_config.status_format;
	𐤀𐤌 (status_format == STATUS_FORMAT_UNSPECIFIED)
		status_format = STATUS_FORMAT_NONE;

	𐤀𐤌 (use_deferred_config && s->show_branch < 0)
		s->show_branch = status_deferred_config.show_branch;
	𐤀𐤌 (s->show_branch < 0)
		s->show_branch = 0;

	/*
	 * If the user did not give a "--[no]-ahead-behind" command
	 * line argument *AND* we will print in a human-readable format
	 * (short, long etc.) then we inherit from the status.aheadbehind
	 * config setting.  In all other cases (and porcelain V[12] formats
	 * in particular), we inherit _FULL for backwards compatibility.
	 */
	𐤀𐤌 (use_deferred_config &&
	    s->ahead_behind_flags == AHEAD_BEHIND_UNSPECIFIED)
		s->ahead_behind_flags = status_deferred_config.ahead_behind;

	𐤀𐤌 (s->ahead_behind_flags == AHEAD_BEHIND_UNSPECIFIED)
		s->ahead_behind_flags = AHEAD_BEHIND_FULL;
}

ساكن فراغ check_fixup_reword_options(صحيح argc, ثابت حرف *argv[]) {
	𐤀𐤌 (whence != FROM_COMMIT) {
		𐤀𐤌 (whence == FROM_MERGE)
			die(_("You are in the middle of a merge -- cannot reword."));
		وإلا 𐤀𐤌 (is_from_cherry_pick(whence))
			die(_("You are in the middle of a cherry-pick -- cannot reword."));
	}
	𐤀𐤌 (argc)
		die(_("reword option of '%s' and path '%s' cannot be used together"), "--fixup", *argv);
	𐤀𐤌 (patch_interactive || interactive || all || also || only)
		die(_("reword option of '%s' and '%s' cannot be used together"),
			"--fixup", "--patch/--interactive/--all/--include/--only");
}

ساكن صحيح parse_and_validate_options(صحيح argc, ثابت حرف *argv[],
				      ثابت بنية option *options,
				      ثابت حرف * ثابت usage[],
				      ثابت حرف *prefix,
				      بنية commit *current_head,
				      بنية wt_status *s)
{
	argc = parse_options(argc, argv, prefix, options, usage, 0);
	finalize_deferred_config(s);

	𐤀𐤌 (force_author && !strchr(force_author, '>'))
		force_author = find_author_by_nickname(force_author);

	𐤀𐤌 (force_author && renew_authorship)
		die(_("options '%s' and '%s' cannot be used together"), "--reset-author", "--author");

	𐤀𐤌 (logfile || have_option_m || use_message)
		use_editor = 0;

	/* Sanity check options */
	𐤀𐤌 (amend && !current_head)
		die(_("You have nothing to amend."));
	𐤀𐤌 (amend && whence != FROM_COMMIT) {
		𐤀𐤌 (whence == FROM_MERGE)
			die(_("You are in the middle of a merge -- cannot amend."));
		وإلا 𐤀𐤌 (is_from_cherry_pick(whence))
			die(_("You are in the middle of a cherry-pick -- cannot amend."));
		وإلا 𐤀𐤌 (whence == FROM_REBASE_PICK)
			die(_("You are in the middle of a rebase -- cannot amend."));
	}
	𐤀𐤌 (fixup_message && squash_message)
		die(_("options '%s' and '%s' cannot be used together"), "--squash", "--fixup");
	die_for_incompatible_opt4(!!use_message, "-C",
				  !!edit_message, "-c",
				  !!logfile, "-F",
				  !!fixup_message, "--fixup");
	die_for_incompatible_opt4(have_option_m, "-m",
				  !!edit_message, "-c",
				  !!use_message, "-C",
				  !!logfile, "-F");
	𐤀𐤌 (use_message || edit_message || logfile ||fixup_message || have_option_m)
		FREE_AND_NULL(template_file);
	𐤀𐤌 (edit_message)
		use_message = edit_message;
	𐤀𐤌 (amend && !use_message && !fixup_message)
		use_message = "HEAD";
	𐤀𐤌 (!use_message && !is_from_cherry_pick(whence) &&
	    !is_from_rebase(whence) && renew_authorship)
		die(_("--reset-author can be used only with -C, -c or --amend."));
	𐤀𐤌 (use_message) {
		use_message_buffer = read_commit_message(use_message);
		𐤀𐤌 (!renew_authorship) {
			author_message = use_message;
			author_message_buffer = use_message_buffer;
		}
	}
	𐤀𐤌 ((is_from_cherry_pick(whence) || whence == FROM_REBASE_PICK) &&
	    !renew_authorship) {
		author_message = "CHERRY_PICK_HEAD";
		author_message_buffer = read_commit_message(author_message);
	}

	𐤀𐤌 (patch_interactive)
		interactive = 1;

	die_for_incompatible_opt4(also, "-i/--include",
				  only, "-o/--only",
				  all, "-a/--all",
				  interactive, "--interactive/-p/--patch");
	𐤀𐤌 (fixup_message) {
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
		حرف *p = fixup_message;
		طالما (isalpha(*p))
			p++;
		𐤀𐤌 (p > fixup_message && *p == ':') {
			*p = '\0';
			fixup_commit = p + 1;
			𐤀𐤌 (!strcmp("amend", fixup_message) ||
			    !strcmp("reword", fixup_message)) {
				fixup_prefix = "amend";
				allow_empty = 1;
				𐤀𐤌 (*fixup_message == 'r') {
					check_fixup_reword_options(argc, argv);
					only = 1;
				}
			} وإلا {
				die(_("unknown option: --fixup=%s:%s"), fixup_message, fixup_commit);
			}
		} وإلا {
			fixup_commit = fixup_message;
			fixup_prefix = "fixup";
			use_editor = 0;
		}
	}

	𐤀𐤌 (0 <= edit_flag)
		use_editor = edit_flag;

	handle_untracked_files_arg(s);

	𐤀𐤌 (all && argc > 0)
		die(_("paths '%s ...' with -a does not make sense"),
		    argv[0]);

	𐤀𐤌 (status_format != STATUS_FORMAT_NONE)
		dry_run = 1;

	أرجع argc;
}

ساكن صحيح dry_run_commit(ثابت حرف **argv, ثابت حرف *prefix,
			  ثابت بنية commit *current_head, بنية wt_status *s)
{
	صحيح committable;
	ثابت حرف *index_file;

	index_file = prepare_index(argv, prefix, current_head, 1);
	committable = run_status(stdout, index_file, prefix, 0, s);
	rollback_index_files();

	أرجع committable ? 0 : 1;
}

define_list_config_array_extra(color_status_slots, {"added"});

ساكن صحيح parse_status_slot(ثابت حرف *slot)
{
	𐤀𐤌 (!strcasecmp(slot, "added"))
		أرجع WT_STATUS_UPDATED;

	أرجع LOOKUP_CONFIG(color_status_slots, slot);
}

ساكن صحيح git_status_config(ثابت حرف *k, ثابت حرف *v,
			     ثابت بنية config_context *ctx, فراغ *cb)
{
	بنية wt_status *s = cb;
	ثابت حرف *slot_name;

	𐤀𐤌 (starts_with(k, "column."))
		أرجع git_column_config(k, v, "status", &s->colopts);
	𐤀𐤌 (!strcmp(k, "status.submodulesummary")) {
		صحيح is_bool;
		s->submodule_summary = git_config_bool_or_int(k, v, ctx->kvi,
							      &is_bool);
		𐤀𐤌 (is_bool && s->submodule_summary)
			s->submodule_summary = -1;
		أرجع 0;
	}
	𐤀𐤌 (!strcmp(k, "status.short")) {
		𐤀𐤌 (git_config_bool(k, v))
			status_deferred_config.status_format = STATUS_FORMAT_SHORT;
		وإلا
			status_deferred_config.status_format = STATUS_FORMAT_NONE;
		أرجع 0;
	}
	𐤀𐤌 (!strcmp(k, "status.branch")) {
		status_deferred_config.show_branch = git_config_bool(k, v);
		أرجع 0;
	}
	𐤀𐤌 (!strcmp(k, "status.aheadbehind")) {
		status_deferred_config.ahead_behind = git_config_bool(k, v);
		أرجع 0;
	}
	𐤀𐤌 (!strcmp(k, "status.showstash")) {
		s->show_stash = git_config_bool(k, v);
		أرجع 0;
	}
	𐤀𐤌 (!strcmp(k, "status.color") || !strcmp(k, "color.status")) {
		s->use_color = git_config_colorbool(k, v);
		أرجع 0;
	}
	𐤀𐤌 (!strcmp(k, "status.displaycommentprefix")) {
		s->display_comment_prefix = git_config_bool(k, v);
		أرجع 0;
	}
	𐤀𐤌 (skip_prefix(k, "status.color.", &slot_name) ||
	    skip_prefix(k, "color.status.", &slot_name)) {
		صحيح slot = parse_status_slot(slot_name);
		𐤀𐤌 (slot < 0)
			أرجع 0;
		𐤀𐤌 (!v)
			أرجع config_error_nonbool(k);
		أرجع color_parse(v, s->color_palette[slot]);
	}
	𐤀𐤌 (!strcmp(k, "status.relativepaths")) {
		s->relative_paths = git_config_bool(k, v);
		أرجع 0;
	}
	𐤀𐤌 (!strcmp(k, "status.showuntrackedfiles")) {
		تعداد untracked_status_type u;

		u = parse_untracked_setting_name(v);
		𐤀𐤌 (u == SHOW_UNTRACKED_FILES_ERROR)
			أرجع error(_("Invalid untracked files mode '%s'"), v);
		s->show_untracked_files = u;
		أرجع 0;
	}
	𐤀𐤌 (!strcmp(k, "diff.renamelimit")) {
		𐤀𐤌 (s->rename_limit == -1)
			s->rename_limit = git_config_int(k, v, ctx->kvi);
		أرجع 0;
	}
	𐤀𐤌 (!strcmp(k, "status.renamelimit")) {
		s->rename_limit = git_config_int(k, v, ctx->kvi);
		أرجع 0;
	}
	𐤀𐤌 (!strcmp(k, "diff.renames")) {
		𐤀𐤌 (s->detect_rename == -1)
			s->detect_rename = git_config_rename(k, v);
		أرجع 0;
	}
	𐤀𐤌 (!strcmp(k, "status.renames")) {
		s->detect_rename = git_config_rename(k, v);
		أرجع 0;
	}
	أرجع git_diff_ui_config(k, v, ctx, عدم);
}

صحيح cmd_status(صحيح argc,
ثابت حرف **argv,
ثابت حرف *prefix,
بنية repository *repo UNUSED)
{
	ساكن صحيح no_renames = -1;
	ساكن ثابت حرف *rename_score_arg = (ثابت حرف *)-1;
	ساكن بنية wt_status s;
	unsigned صحيح progress_flag = 0;
	صحيح fd;
	بنية object_id oid;
	ساكن بنية option builtin_status_options[] = {
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
			.value = &untracked_files_arg,
			.argh = N_("mode"),
			.help = N_("show untracked files, optional modes: all, normal, no. (Default: all)"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t)"all",
		},
		{
			.type = OPTION_STRING,
			.long_name = "ignored",
			.value = &ignored_arg,
			.argh = N_("mode"),
			.help = N_("show ignored files, optional modes: traditional, matching, no. (Default: traditional)"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t)"traditional",
		},
		{
			.type = OPTION_STRING,
			.long_name = "ignore-submodules",
			.value = &ignore_submodule_arg,
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

	𐤀𐤌 (s.show_ignored_mode == SHOW_MATCHING_IGNORED &&
	    s.show_untracked_files == SHOW_NO_UNTRACKED_FILES)
		die(_("Unsupported combination of ignored and untracked-files arguments"));

	parse_pathspec(&s.pathspec, 0,
		       PATHSPEC_PREFER_FULL,
		       prefix, argv);

	𐤀𐤌 (status_format != STATUS_FORMAT_PORCELAIN &&
	    status_format != STATUS_FORMAT_PORCELAIN_V2)
		progress_flag = REFRESH_PROGRESS;
	repo_read_index(the_repository);
	refresh_index(the_repository->index,
		      REFRESH_QUIET|REFRESH_UNMERGED|progress_flag,
		      &s.pathspec, عدم, عدم);

	𐤀𐤌 (use_optional_locks())
		fd = repo_hold_locked_index(the_repository, &index_lock, 0);
	وإلا
		fd = -1;

	s.is_initial = repo_get_oid(the_repository, s.reference, &oid) ? 1 : 0;
	𐤀𐤌 (!s.is_initial)
		oidcpy(&s.oid_commit, &oid);

	s.ignore_submodule_arg = ignore_submodule_arg;
	s.status_format = status_format;
	s.verbose = verbose;
	𐤀𐤌 (no_renames != -1)
		s.detect_rename = !no_renames;
	𐤀𐤌 ((intptr_t)rename_score_arg != -1) {
		𐤀𐤌 (s.detect_rename < DIFF_DETECT_RENAME)
			s.detect_rename = DIFF_DETECT_RENAME;
		𐤀𐤌 (rename_score_arg)
			s.rename_score = parse_rename_score(&rename_score_arg);
	}

	wt_status_collect(&s);

	𐤀𐤌 (0 <= fd)
		repo_update_index_if_able(the_repository, &index_lock);

	𐤀𐤌 (s.relative_paths)
		s.prefix = prefix;

	wt_status_print(&s);
	wt_status_collect_free_buffers(&s);

	أرجع 0;
}

ساكن صحيح git_commit_config(ثابت حرف *k, ثابت حرف *v,
			     ثابت بنية config_context *ctx, فراغ *cb)
{
	بنية wt_status *s = cb;

	𐤀𐤌 (!strcmp(k, "commit.template"))
		أرجع git_config_pathname(&template_file, k, v);
	𐤀𐤌 (!strcmp(k, "commit.status")) {
		include_status = git_config_bool(k, v);
		أرجع 0;
	}
	𐤀𐤌 (!strcmp(k, "commit.cleanup")) {
		FREE_AND_NULL(cleanup_config);
		أرجع git_config_string(&cleanup_config, k, v);
	}
	𐤀𐤌 (!strcmp(k, "commit.gpgsign")) {
		sign_commit = git_config_bool(k, v) ? "" : عدم;
		أرجع 0;
	}
	𐤀𐤌 (!strcmp(k, "commit.verbose")) {
		صحيح is_bool;
		config_commit_verbose = git_config_bool_or_int(k, v, ctx->kvi,
							       &is_bool);
		أرجع 0;
	}

	أرجع git_status_config(k, v, ctx, s);
}

صحيح cmd_commit(صحيح argc,
	       ثابت حرف **argv,
	       ثابت حرف *prefix,
	       بنية repository *repo UNUSED)
{
	ساكن بنية wt_status s;
	ساكن ثابت حرف *cleanup_arg = عدم;
	ساكن بنية option builtin_commit_options[] = {
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
			.value = &sign_commit,
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
			.value = &untracked_files_arg,
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

	بنية strbuf sb = STRBUF_INIT;
	بنية strbuf author_ident = STRBUF_INIT;
	ثابت حرف *index_file, *reflog_msg;
	بنية object_id oid;
	بنية commit_list *parents = عدم;
	بنية stat statbuf;
	بنية commit *current_head = عدم;
	بنية commit_extra_header *extra = عدم;
	بنية strbuf err = STRBUF_INIT;
	صحيح ret = 0;

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

	𐤀𐤌 (repo_get_oid(the_repository, "HEAD", &oid))
		current_head = عدم;
	وإلا {
		current_head = lookup_commit_or_die(&oid, "HEAD");
		𐤀𐤌 (repo_parse_commit(the_repository, current_head))
			die(_("could not parse HEAD commit"));
	}
	verbose = -1; /* unspecified */
	argc = parse_and_validate_options(argc, argv, builtin_commit_options,
					  builtin_commit_usage,
					  prefix, current_head, &s);
	𐤀𐤌 (trailer_args.nr)
		trailer_config_init();

	𐤀𐤌 (verbose == -1)
		verbose = (config_commit_verbose < 0) ? 0 : config_commit_verbose;

	𐤀𐤌 (cleanup_arg) {
		حرر(cleanup_config);
		cleanup_config = xstrdup(cleanup_arg);
	}
	cleanup_mode = get_cleanup_mode(cleanup_config, use_editor);

	𐤀𐤌 (dry_run)
		أرجع dry_run_commit(argv, prefix, current_head, &s);
	index_file = prepare_index(argv, prefix, current_head, 0);

	/* Set up everything for writing the commit object.  This includes
	   running hooks, writing the trees, and interacting with the user.  */
	𐤀𐤌 (!prepare_to_commit(index_file, prefix,
			       current_head, &s, &author_ident)) {
		ret = 1;
		rollback_index_files();
		اذهب cleanup;
	}

	/* Determine parents */
	reflog_msg = getenv("GIT_REFLOG_ACTION");
	𐤀𐤌 (!current_head) {
		𐤀𐤌 (!reflog_msg)
			reflog_msg = "commit (initial)";
	} وإلا 𐤀𐤌 (amend) {
		𐤀𐤌 (!reflog_msg)
			reflog_msg = "commit (amend)";
		parents = commit_list_copy(current_head->parents);
	} وإلا 𐤀𐤌 (whence == FROM_MERGE) {
		بنية strbuf m = STRBUF_INIT;
		FILE *fp;
		صحيح allow_fast_forward = 1;
		بنية commit_list **pptr = &parents;

		𐤀𐤌 (!reflog_msg)
			reflog_msg = "commit (merge)";
		pptr = commit_list_append(current_head, pptr);
		fp = xfopen(git_path_merge_head(the_repository), "r");
		طالما (strbuf_getline_lf(&m, fp) != EOF) {
			بنية commit *parent;

			parent = get_merge_parent(m.buf);
			𐤀𐤌 (!parent)
				die(_("Corrupt MERGE_HEAD file (%s)"), m.buf);
			pptr = commit_list_append(parent, pptr);
		}
		fclose(fp);
		strbuf_release(&m);
		𐤀𐤌 (!stat(git_path_merge_mode(the_repository), &statbuf)) {
			𐤀𐤌 (strbuf_read_file(&sb, git_path_merge_mode(the_repository), 0) < 0)
				die_errno(_("could not read MERGE_MODE"));
			𐤀𐤌 (!strcmp(sb.buf, "no-ff"))
				allow_fast_forward = 0;
		}
		𐤀𐤌 (allow_fast_forward)
			reduce_heads_replace(&parents);
	} وإلا {
		𐤀𐤌 (!reflog_msg)
			reflog_msg = is_from_cherry_pick(whence)
					? "commit (cherry-pick)"
					: is_from_rebase(whence)
					? "commit (rebase)"
					: "commit";
		commit_list_insert(current_head, &parents);
	}

	/* Finally, get the commit message */
	strbuf_reset(&sb);
	𐤀𐤌 (strbuf_read_file(&sb, git_path_commit_editmsg(), 0) < 0) {
		صحيح saved_errno = errno;
		rollback_index_files();
		die(_("could not read commit message: %s"), strerror(saved_errno));
	}

	cleanup_message(&sb, cleanup_mode, verbose);

	𐤀𐤌 (message_is_empty(&sb, cleanup_mode) && !allow_empty_message) {
		rollback_index_files();
		fprintf(stderr, _("Aborting commit due to empty commit message.\n"));
		exit(1);
	}
	𐤀𐤌 (template_untouched(&sb, template_file, cleanup_mode) && !allow_empty_message) {
		rollback_index_files();
		fprintf(stderr, _("Aborting commit; you did not edit the message.\n"));
		exit(1);
	}

	𐤀𐤌 (fixup_message && starts_with(sb.buf, "amend! ") &&
	    !allow_empty_message) {
		بنية strbuf body = STRBUF_INIT;
		size_t len = commit_subject_length(sb.buf);
		strbuf_addstr(&body, sb.buf + len);
		𐤀𐤌 (message_is_empty(&body, cleanup_mode)) {
			rollback_index_files();
			fprintf(stderr, _("Aborting commit due to empty commit message body.\n"));
			exit(1);
		}
		strbuf_release(&body);
	}

	𐤀𐤌 (amend) {
		ثابت حرف *exclude_gpgsig[3] = { "gpgsig", "gpgsig-sha256", عدم };
		extra = read_commit_extra_headers(current_head, exclude_gpgsig);
	} وإلا {
		بنية commit_extra_header **tail = &extra;
		append_merge_tag_headers(parents, &tail);
	}

	𐤀𐤌 (commit_tree_extended(sb.buf, sb.len, &the_repository->index->cache_tree->oid,
				 parents, &oid, author_ident.buf, عدم,
				 sign_commit, extra)) {
		rollback_index_files();
		die(_("failed to write commit object"));
	}

	𐤀𐤌 (update_head_with_reflog(current_head, &oid, reflog_msg, &sb,
				    &err)) {
		rollback_index_files();
		die("%s", err.buf);
	}

	sequencer_post_commit_cleanup(the_repository, 0);
	unlink(git_path_merge_head(the_repository));
	unlink(git_path_merge_msg(the_repository));
	unlink(git_path_merge_mode(the_repository));
	unlink(git_path_squash_msg(the_repository));

	𐤀𐤌 (commit_index_files())
		die(_("repository has been updated, but unable to write\n"
		      "new index file. Check that disk is not full and quota is\n"
		      "not exceeded, and then \"git restore --staged :/\" to recover."));

	git_test_write_commit_graph_or_die(the_repository);

	repo_rerere(the_repository, 0);
	run_auto_maintenance(the_repository, quiet);
	run_commit_hook(use_editor, repo_get_index_file(the_repository),
			عدم, "post-commit", عدم);
	𐤀𐤌 (amend && !no_post_rewrite) {
		commit_post_rewrite(the_repository, current_head, &oid);
	}
	𐤀𐤌 (!quiet) {
		unsigned صحيح flags = 0;

		𐤀𐤌 (!current_head)
			flags |= SUMMARY_INITIAL_COMMIT;
		𐤀𐤌 (author_date_is_interesting())
			flags |= SUMMARY_SHOW_AUTHOR_DATE;
		print_commit_summary(the_repository, prefix,
				     &oid, flags);
	}

	apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
			    عدم, عدم, عدم, عدم);

cleanup:
	free_commit_extra_headers(extra);
	commit_list_free(parents);
	strbuf_release(&author_ident);
	strbuf_release(&err);
	strbuf_release(&sb);
	حرر(logfile);
	حرر(template_file);
	أرجع ret;
}
