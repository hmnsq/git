/*
 * Builtin "git commit"
 *
 * Copyright (c) 2007 Kristian Høgsberg <krh@redhat.com>
 * Based on git-commit.sh by Junio C Hamano and Linus Torvalds
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#include "builtin.h"
#include "advice.h"
#include "config.h"
#include "lockfile.h"
#include "cache-tree.h"
#include "color.h"
#include "dir.h"
#include "editor.h"
#include "environment.h"
#include "diff.h"
#include "commit.h"
#include "add-interactive.h"
#include "gettext.h"
#include "revision.h"
#include "wt-status.h"
#include "run-command.h"
#include "strbuf.h"
#include "object-name.h"
#include "parse-options.h"
#include "path.h"
#include "preload-index.h"
#include "read-cache.h"
#include "repository.h"
#include "string-list.h"
#include "rerere.h"
#include "unpack-trees.h"
#include "column.h"
#include "sequencer.h"
#include "sparse-index.h"
#include "mailmap.h"
#include "help.h"
#include "commit-reach.h"
#include "commit-graph.h"
#include "pretty.h"
#include "trailer.h"

static const char * const builtin_commit_usage[] = {
	N_("git commit [-a | --interactive | --patch] [-s] [-v] [-u[<mode>]] [--amend]\n"
	   "           [--dry-run] [(-c | -C | --squash) <commit> | --fixup [(amend|reword):]<commit>]\n"
	   "           [-F <file> | -m <msg>] [--reset-author] [--allow-empty]\n"
	   "           [--allow-empty-message] [--no-verify] [-e] [--author=<author>]\n"
	   "           [--date=<date>] [--cleanup=<mode>] [--[no-]status]\n"
	   "           [-i | -o] [--pathspec-from-file=<file> [--pathspec-file-nul]]\n"
	   "           [(--trailer <token>[(=|:)<value>])...] [-S[<keyid>]]\n"
	   "           [--] [<pathspec>...]"),
	NULL
};

static const char * const builtin_status_usage[] = {
	N_("git status [<options>] [--] [<pathspec>...]"),
	NULL
};

static const char empty_amend_advice[] =
N_("You asked to amend the most recent commit, but doing so would make\n"
"it empty. You can repeat your command with --allow-empty, or you can\n"
"remove the commit entirely with \"git reset HEAD^\".\n");

static const char empty_cherry_pick_advice[] =
N_("The previous cherry-pick is now empty, possibly due to conflict resolution.\n"
"If you wish to commit it anyway, use:\n"
"\n"
"    git commit --allow-empty\n"
"\n");

static const char empty_rebase_pick_advice[] =
N_("Otherwise, please use 'git rebase --skip'\n");

static const char empty_cherry_pick_advice_single[] =
N_("Otherwise, please use 'git cherry-pick --skip'\n");

static const char empty_cherry_pick_advice_multi[] =
N_("and then use:\n"
"\n"
"    git cherry-pick --continue\n"
"\n"
"to resume cherry-picking the remaining commits.\n"
"If you wish to skip this commit, use:\n"
"\n"
"    git cherry-pick --skip\n"
"\n");

static const char *color_status_slots[] = {
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

static const char *use_message_buffer;
static struct lock_file index_lock; /* real index */
static struct lock_file false_lock; /* used only for partial commits */
static enum {
	COMMIT_AS_IS = 1,
	COMMIT_NORMAL,
	COMMIT_PARTIAL
} commit_style;

static const char *force_author;
static char *logfile;
static char *template_file;
/*
 * The _message variables are commit names from which to take
 * the commit message and/or authorship.
 */
static const char *author_message, *author_message_buffer;
static const char *edit_message, *use_message;
static char *fixup_message, *fixup_commit, *squash_message;
static const char *fixup_prefix;
static 整数 all, also, interactive, patch_interactive, only, amend, signoff;
static struct interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
static 整数 edit_flag = -1; /* unspecified */
static 整数 quiet, verbose, no_verify, allow_empty, dry_run, renew_authorship;
static 整数 config_commit_verbose = -1; /* unspecified */
static 整数 no_post_rewrite, allow_empty_message, pathspec_file_nul;
static const char *untracked_files_arg, *force_date, *ignore_submodule_arg, *ignored_arg;
static const char *sign_commit, *pathspec_from_file;
static struct strvec trailer_args = STRVEC_INIT;

/*
 * The default commit message cleanup mode will remove the lines
 * beginning with # (shell comments) and leading and trailing
 * whitespaces (empty lines or containing only whitespaces)
 * if editor is used, and only the whitespaces if the message
 * is specified explicitly.
 */
static enum commit_msg_cleanup_mode cleanup_mode;
static char *cleanup_config;

static enum commit_whence whence;
static 整数 use_editor = 1, include_status = 1;
static 整数 have_option_m;
static struct strbuf message = STRBUF_INIT;

static enum wt_status_format status_format = STATUS_FORMAT_UNSPECIFIED;

static 整数 opt_parse_porcelain(const struct option *opt, const char *arg, 整数 unset)
{
	enum wt_status_format *value = (enum wt_status_format *)opt->value;
	如果 (unset)
		*value = STATUS_FORMAT_NONE;
	否则 如果 (!arg)
		*value = STATUS_FORMAT_PORCELAIN;
	否则 如果 (!strcmp(arg, "v1") || !strcmp(arg, "1"))
		*value = STATUS_FORMAT_PORCELAIN;
	否则 如果 (!strcmp(arg, "v2") || !strcmp(arg, "2"))
		*value = STATUS_FORMAT_PORCELAIN_V2;
	否则
		die("unsupported porcelain version '%s'", arg);

	返回 0;
}

static 整数 opt_parse_m(const struct option *opt, const char *arg, 整数 unset)
{
	struct strbuf *buf = opt->value;
	如果 (unset) {
		have_option_m = 0;
		strbuf_setlen(buf, 0);
	} 否则 {
		have_option_m = 1;
		如果 (buf->len)
			strbuf_addch(buf, '\n');
		strbuf_addstr(buf, arg);
		strbuf_complete_line(buf);
	}
	返回 0;
}

static 整数 opt_parse_rename_score(const struct option *opt, const char *arg, 整数 unset)
{
	const char **value = opt->value;

	BUG_ON_OPT_NEG(unset);

	如果 (arg != NULL && *arg == '=')
		arg = arg + 1;

	*value = arg;
	返回 0;
}

static void determine_whence(struct wt_status *s)
{
	如果 (file_exists(git_path_merge_head(the_repository)))
		whence = FROM_MERGE;
	否则 如果 (!sequencer_determine_whence(the_repository, &whence))
		whence = FROM_COMMIT;
	如果 (s)
		s->whence = whence;
}

static void status_init_config(struct wt_status *s, config_fn_t fn)
{
	wt_status_prepare(the_repository, s);
	init_diff_ui_defaults();
	repo_config(the_repository, fn, s);
	determine_whence(s);
	s->hints = advice_enabled(ADVICE_STATUS_HINTS); /* must come after repo_config() */
}

static void rollback_index_files(void)
{
	switch (commit_style) {
	case COMMIT_AS_IS:
		break; /* nothing to do */
	case COMMIT_NORMAL:
		rollback_lock_file(&index_lock);
		break;
	case COMMIT_PARTIAL:
		rollback_lock_file(&index_lock);
		rollback_lock_file(&false_lock);
		break;
	}
}

static 整数 commit_index_files(void)
{
	整数 err = 0;

	switch (commit_style) {
	case COMMIT_AS_IS:
		break; /* nothing to do */
	case COMMIT_NORMAL:
		err = commit_lock_file(&index_lock);
		break;
	case COMMIT_PARTIAL:
		err = commit_lock_file(&index_lock);
		rollback_lock_file(&false_lock);
		break;
	}

	返回 err;
}

/*
 * Take a union of paths in the index and the named tree (typically, "HEAD"),
 * and return the paths that match the given pattern in list.
 */
static 整数 list_paths(struct string_list *list, const char *with_tree,
		      const struct pathspec *pattern)
{
	整数 i, ret;
	char *m;

	如果 (!pattern->nr)
		返回 0;

	m = xcalloc(1, pattern->nr);

	如果 (with_tree) {
		char *max_prefix = common_prefix(pattern);
		overlay_tree_on_index(the_repository->index, with_tree, max_prefix);
		free(max_prefix);
	}

	/* TODO: audit for interaction with sparse-index. */
	ensure_full_index(the_repository->index);
	为 (i = 0; i < the_repository->index->cache_nr; i++) {
		const struct cache_entry *ce = the_repository->index->cache[i];
		struct string_list_item *item;

		如果 (ce->ce_flags & CE_UPDATE)
			continue;
		如果 (!ce_path_match(the_repository->index, ce, pattern, m))
			continue;
		item = string_list_insert(list, ce->name);
		如果 (ce_skip_worktree(ce))
			item->util = item; /* better a valid pointer than a fake one */
	}

	ret = report_path_error(m, pattern);
	free(m);
	返回 ret;
}

static void add_remove_files(struct string_list *list)
{
	整数 i;
	为 (i = 0; i < list->nr; i++) {
		struct stat st;
		struct string_list_item *p = &(list->items[i]);

		/* p->util is skip-worktree */
		如果 (p->util)
			continue;

		如果 (!lstat(p->string, &st)) {
			如果 (add_to_index(the_repository->index, p->string, &st, 0))
				die(_("updating files failed"));
		} 否则
			remove_file_from_index(the_repository->index, p->string);
	}
}

static void create_base_index(const struct commit *current_head)
{
	struct tree *tree;
	struct unpack_trees_options opts;
	struct tree_desc t;

	如果 (!current_head) {
		discard_index(the_repository->index);
		返回;
	}

	memset(&opts, 0, sizeof(opts));
	opts.head_idx = 1;
	opts.index_only = 1;
	opts.merge = 1;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;

	opts.fn = oneway_merge;
	tree = repo_parse_tree_indirect(the_repository,
					&current_head->object.oid);
	如果 (!tree)
		die(_("failed to unpack HEAD tree object"));
	如果 (repo_parse_tree(the_repository, tree) < 0)
		exit(128);
	init_tree_desc(&t, &tree->object.oid, tree->buffer, tree->size);
	如果 (unpack_trees(1, &t, &opts))
		exit(128); /* We've already reported the error, finish dying */
}

static void refresh_cache_or_die(整数 refresh_flags)
{
	/*
	 * refresh_flags contains REFRESH_QUIET, so the only errors
	 * are for unmerged entries.
	 */
	如果 (refresh_index(the_repository->index, refresh_flags | REFRESH_IN_PORCELAIN, NULL, NULL, NULL))
		die_resolve_conflict("commit");
}

static const char *prepare_index(const char **argv, const char *prefix,
				 const struct commit *current_head, 整数 is_status)
{
	struct string_list partial = STRING_LIST_INIT_DUP;
	struct pathspec pathspec;
	整数 refresh_flags = REFRESH_QUIET;
	const char *ret;
	char *path = NULL;

	如果 (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	如果 (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	如果 (is_status)
		refresh_flags |= REFRESH_UNMERGED;
	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_FULL,
		       prefix, argv);

	如果 (pathspec_from_file) {
		如果 (interactive)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--interactive/--patch");

		如果 (all)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "-a");

		如果 (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_FULL,
				    prefix, pathspec_from_file, pathspec_file_nul);
	} 否则 如果 (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	如果 (!pathspec.nr && (also || (only && !allow_empty &&
	    (!amend || (fixup_message && strcmp(fixup_prefix, "amend"))))))
		die(_("No paths with --include/--only does not make sense."));

	如果 (repo_read_index_preload(the_repository, &pathspec, 0) < 0)
		die(_("index file corrupt"));

	如果 (interactive) {
		char *old_index_env = NULL, *old_repo_index_file;
		repo_hold_locked_index(the_repository, &index_lock,
				       LOCK_DIE_ON_ERROR);

		refresh_cache_or_die(refresh_flags);

		如果 (write_locked_index(the_repository->index, &index_lock, 0))
			die(_("unable to create temporary index"));

		old_repo_index_file = the_repository->index_file;
		the_repository->index_file =
			(char *)get_lock_file_path(&index_lock);
		old_index_env = xstrdup_or_null(getenv(INDEX_ENVIRONMENT));
		setenv(INDEX_ENVIRONMENT, the_repository->index_file, 1);

		如果 (interactive_add(the_repository, argv, prefix, patch_interactive, &interactive_opts) != 0)
			die(_("interactive add failed"));

		the_repository->index_file = old_repo_index_file;
		如果 (old_index_env && *old_index_env)
			setenv(INDEX_ENVIRONMENT, old_index_env, 1);
		否则
			unsetenv(INDEX_ENVIRONMENT);
		FREE_AND_NULL(old_index_env);

		discard_index(the_repository->index);
		read_index_from(the_repository->index, get_lock_file_path(&index_lock),
				repo_get_git_dir(the_repository));
		如果 (cache_tree_update(the_repository->index, WRITE_TREE_SILENT) == 0) {
			如果 (reopen_lock_file(&index_lock) < 0)
				die(_("unable to write index file"));
			如果 (write_locked_index(the_repository->index, &index_lock, 0))
				die(_("unable to update temporary index"));
		} 否则
			warning(_("Failed to update main cache tree"));

		commit_style = COMMIT_NORMAL;
		ret = get_lock_file_path(&index_lock);
		goto out;
	} 否则 {
		如果 (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--interactive/--patch");
		如果 (interactive_opts.interhunkcontext != -1)
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
	如果 (all || (also && pathspec.nr)) {
		char *ps_matched = xcalloc(pathspec.nr, 1);
		repo_hold_locked_index(the_repository, &index_lock,
				       LOCK_DIE_ON_ERROR);
		add_files_to_cache(the_repository, also ? prefix : NULL,
				   &pathspec, ps_matched, 0, 0, 0 );
		如果 (!all && report_path_error(ps_matched, &pathspec))
			exit(128);

		refresh_cache_or_die(refresh_flags);
		cache_tree_update(the_repository->index, WRITE_TREE_SILENT);
		如果 (write_locked_index(the_repository->index, &index_lock, 0))
			die(_("unable to write new index file"));
		commit_style = COMMIT_NORMAL;
		ret = get_lock_file_path(&index_lock);
		free(ps_matched);
		goto out;
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
	如果 (!only && !pathspec.nr) {
		repo_hold_locked_index(the_repository, &index_lock,
				       LOCK_DIE_ON_ERROR);
		refresh_cache_or_die(refresh_flags);
		如果 (the_repository->index->cache_changed
		    || !cache_tree_fully_valid(the_repository->index))
			cache_tree_update(the_repository->index, WRITE_TREE_SILENT);
		如果 (write_locked_index(the_repository->index, &index_lock,
				       COMMIT_LOCK | SKIP_IF_UNCHANGED))
			die(_("unable to write new index file"));
		commit_style = COMMIT_AS_IS;
		ret = repo_get_index_file(the_repository);
		goto out;
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

	如果 (whence != FROM_COMMIT) {
		如果 (whence == FROM_MERGE)
			die(_("cannot do a partial commit during a merge."));
		否则 如果 (is_from_cherry_pick(whence))
			die(_("cannot do a partial commit during a cherry-pick."));
		否则 如果 (is_from_rebase(whence))
			die(_("cannot do a partial commit during a rebase."));
	}

	如果 (list_paths(&partial, !current_head ? NULL : "HEAD", &pathspec))
		exit(1);

	discard_index(the_repository->index);
	如果 (repo_read_index(the_repository) < 0)
		die(_("cannot read the index"));

	repo_hold_locked_index(the_repository, &index_lock, LOCK_DIE_ON_ERROR);
	add_remove_files(&partial);
	refresh_index(the_repository->index, REFRESH_QUIET, NULL, NULL, NULL);
	cache_tree_update(the_repository->index, WRITE_TREE_SILENT);
	如果 (write_locked_index(the_repository->index, &index_lock, 0))
		die(_("unable to write new index file"));

	path = repo_git_path(the_repository, "next-index-%"PRIuMAX,
			     (uintmax_t) getpid());
	hold_lock_file_for_update(&false_lock, path,
				  LOCK_DIE_ON_ERROR);

	create_base_index(current_head);
	add_remove_files(&partial);
	refresh_index(the_repository->index, REFRESH_QUIET, NULL, NULL, NULL);

	如果 (write_locked_index(the_repository->index, &false_lock, 0))
		die(_("unable to write temporary index file"));

	discard_index(the_repository->index);
	ret = get_lock_file_path(&false_lock);
	read_index_from(the_repository->index, ret, repo_get_git_dir(the_repository));
out:
	string_list_clear(&partial, 0);
	clear_pathspec(&pathspec);
	free(path);
	返回 ret;
}

static 整数 run_status(FILE *fp, const char *index_file, const char *prefix, 整数 nowarn,
		      struct wt_status *s)
{
	struct object_id oid;

	如果 (s->relative_paths)
		s->prefix = prefix;

	如果 (amend) {
		s->amend = 1;
		s->reference = "HEAD^1";
	}
	s->verbose = verbose;
	s->index_file = index_file;
	s->fp = fp;
	s->nowarn = nowarn;
	s->is_initial = repo_get_oid(the_repository, s->reference, &oid) ? 1 : 0;
	如果 (!s->is_initial)
		oidcpy(&s->oid_commit, &oid);
	s->status_format = status_format;
	s->ignore_submodule_arg = ignore_submodule_arg;

	wt_status_collect(s);
	wt_status_print(s);
	wt_status_collect_free_buffers(s);

	返回 s->committable;
}

static 整数 is_a_merge(const struct commit *current_head)
{
	返回 !!(current_head->parents && current_head->parents->next);
}

static void assert_split_ident(struct ident_split *id, const struct strbuf *buf)
{
	如果 (split_ident_line(id, buf->buf, buf->len) || !id->date_begin)
		BUG("unable to parse our own ident: %s", buf->buf);
}

static void export_one(const char *var, const char *s, const char *e, 整数 hack)
{
	struct strbuf buf = STRBUF_INIT;
	如果 (hack)
		strbuf_addch(&buf, hack);
	strbuf_add(&buf, s, e - s);
	setenv(var, buf.buf, 1);
	strbuf_release(&buf);
}

static 整数 parse_force_date(const char *in, struct strbuf *out)
{
	strbuf_addch(out, '@');

	如果 (parse_date(in, out) < 0) {
		整数 errors = 0;
		unsigned long t = approxidate_careful(in, &errors);
		如果 (errors)
			返回 -1;
		strbuf_addf(out, "%lu", t);
	}

	返回 0;
}

static void set_ident_var(char **buf, char *val)
{
	free(*buf);
	*buf = val;
}

static void determine_author_info(struct strbuf *author_ident)
{
	char *name, *email, *date;
	struct ident_split author;

	name = xstrdup_or_null(getenv("GIT_AUTHOR_NAME"));
	email = xstrdup_or_null(getenv("GIT_AUTHOR_EMAIL"));
	date = xstrdup_or_null(getenv("GIT_AUTHOR_DATE"));

	如果 (author_message) {
		struct ident_split ident;
		size_t len;
		const char *a;

		a = find_commit_header(author_message_buffer, "author", &len);
		如果 (!a)
			die(_("commit '%s' lacks author header"), author_message);
		如果 (split_ident_line(&ident, a, len) < 0)
			die(_("commit '%s' has malformed author line"), author_message);

		set_ident_var(&name, xmemdupz(ident.name_begin, ident.name_end - ident.name_begin));
		set_ident_var(&email, xmemdupz(ident.mail_begin, ident.mail_end - ident.mail_begin));

		如果 (ident.date_begin) {
			struct strbuf date_buf = STRBUF_INIT;
			strbuf_addch(&date_buf, '@');
			strbuf_add(&date_buf, ident.date_begin, ident.date_end - ident.date_begin);
			strbuf_addch(&date_buf, ' ');
			strbuf_add(&date_buf, ident.tz_begin, ident.tz_end - ident.tz_begin);
			set_ident_var(&date, strbuf_detach(&date_buf, NULL));
		}
	}

	如果 (force_author) {
		struct ident_split ident;

		如果 (split_ident_line(&ident, force_author, strlen(force_author)) < 0)
			die(_("malformed --author parameter"));
		set_ident_var(&name, xmemdupz(ident.name_begin, ident.name_end - ident.name_begin));
		set_ident_var(&email, xmemdupz(ident.mail_begin, ident.mail_end - ident.mail_begin));
	}

	如果 (force_date) {
		struct strbuf date_buf = STRBUF_INIT;
		如果 (parse_force_date(force_date, &date_buf))
			die(_("invalid date format: %s"), force_date);
		set_ident_var(&date, strbuf_detach(&date_buf, NULL));
	}

	strbuf_addstr(author_ident, fmt_ident(name, email, WANT_AUTHOR_IDENT, date,
				IDENT_STRICT));
	assert_split_ident(&author, author_ident);
	export_one("GIT_AUTHOR_NAME", author.name_begin, author.name_end, 0);
	export_one("GIT_AUTHOR_EMAIL", author.mail_begin, author.mail_end, 0);
	export_one("GIT_AUTHOR_DATE", author.date_begin, author.tz_end, '@');
	free(name);
	free(email);
	free(date);
}

static 整数 author_date_is_interesting(void)
{
	返回 author_message || force_date;
}

#ifndef WITH_BREAKING_CHANGES
static void adjust_comment_line_char(const struct strbuf *sb)
{
	char candidates[] = "#;@!$%^&|:";
	char *candidate;
	const char *p;
	size_t cutoff;

	/* Ignore comment chars in trailing comments (e.g., Conflicts:) */
	cutoff = sb->len - ignored_log_message_bytes(sb->buf, sb->len);

	如果 (!memchr(sb->buf, candidates[0], sb->len)) {
		free(comment_line_str_to_free);
		comment_line_str = comment_line_str_to_free =
			xstrfmt("%c", candidates[0]);
		返回;
	}

	p = sb->buf;
	candidate = strchr(candidates, *p);
	如果 (candidate)
		*candidate = ' ';
	为 (p = sb->buf; p + 1 < sb->buf + cutoff; p++) {
		如果 ((p[0] == '\n' || p[0] == '\r') && p[1]) {
			candidate = strchr(candidates, p[1]);
			如果 (candidate)
				*candidate = ' ';
		}
	}

	为 (p = candidates; *p == ' '; p++)
		;
	如果 (!*p)
		die(_("unable to select a comment character that is not used\n"
		      "in the current commit message"));
	free(comment_line_str_to_free);
	comment_line_str = comment_line_str_to_free = xstrfmt("%c", *p);
}
#endif /* !WITH_BREAKING_CHANGES */

static void prepare_amend_commit(struct commit *commit, struct strbuf *sb,
				struct pretty_print_context *ctx)
{
	const char *buffer, *subject, *fmt;

	buffer = repo_get_commit_buffer(the_repository, commit, NULL);
	find_commit_subject(buffer, &subject);
	/*
	 * If we amend the 'amend!' commit then we don't want to
	 * duplicate the subject line.
	 */
	fmt = starts_with(subject, "amend!") ? "%b" : "%B";
	repo_format_commit_message(the_repository, commit, fmt, sb, ctx);
	repo_unuse_commit_buffer(the_repository, commit, buffer);
}

static void change_data_free(void *util, const char *str UNUSED)
{
	struct wt_status_change_data *d = util;
	free(d->rename_source);
	free(d);
}

static 整数 prepare_to_commit(const char *index_file, const char *prefix,
			     struct commit *current_head,
			     struct wt_status *s,
			     struct strbuf *author_ident)
{
	struct stat statbuf;
	struct strbuf committer_ident = STRBUF_INIT;
	整数 committable;
	struct strbuf sb = STRBUF_INIT;
	const char *hook_arg1 = NULL;
	const char *hook_arg2 = NULL;
	整数 clean_message_contents = (cleanup_mode != COMMIT_MSG_CLEANUP_NONE);
	整数 old_display_comment_prefix;
	整数 invoked_hook;

	/* This checks and barfs if author is badly specified */
	determine_author_info(author_ident);

	如果 (!no_verify && run_commit_hook(use_editor, index_file, &invoked_hook,
					  "pre-commit", NULL))
		返回 0;

	如果 (squash_message) {
		/*
		 * Insert the proper subject line before other commit
		 * message options add their content.
		 */
		如果 (use_message && !strcmp(use_message, squash_message))
			strbuf_addstr(&sb, "squash! ");
		否则 {
			struct pretty_print_context ctx = {0};
			struct commit *c;
			c = lookup_commit_reference_by_name(squash_message);
			如果 (!c)
				die(_("could not lookup commit '%s'"), squash_message);
			ctx.output_encoding = get_commit_output_encoding();
			repo_format_commit_message(the_repository, c,
						   "squash! %s\n\n", &sb,
						   &ctx);
		}
	}

	如果 (have_option_m && !fixup_message) {
		strbuf_addbuf(&sb, &message);
		hook_arg1 = "message";
	} 否则 如果 (logfile && !strcmp(logfile, "-")) {
		如果 (isatty(0))
			fprintf(stderr, _("(reading log message from standard input)\n"));
		如果 (strbuf_read(&sb, 0, 0) < 0)
			die_errno(_("could not read log from standard input"));
		hook_arg1 = "message";
	} 否则 如果 (logfile) {
		如果 (strbuf_read_file(&sb, logfile, 0) < 0)
			die_errno(_("could not read log file '%s'"),
				  logfile);
		hook_arg1 = "message";
	} 否则 如果 (use_message) {
		const char *buffer;
		buffer = strstr(use_message_buffer, "\n\n");
		如果 (buffer)
			strbuf_addstr(&sb, skip_blank_lines(buffer + 2));
		hook_arg1 = "commit";
		hook_arg2 = use_message;
	} 否则 如果 (fixup_message) {
		struct pretty_print_context ctx = {0};
		struct commit *commit;
		char *fmt;
		commit = lookup_commit_reference_by_name(fixup_commit);
		如果 (!commit)
			die(_("could not lookup commit '%s'"), fixup_commit);
		ctx.output_encoding = get_commit_output_encoding();
		fmt = xstrfmt("%s! %%s\n\n", fixup_prefix);
		repo_format_commit_message(the_repository, commit, fmt, &sb,
					   &ctx);
		free(fmt);
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
		如果 (have_option_m && !strcmp(fixup_prefix, "fixup"))
			strbuf_addbuf(&sb, &message);

		如果 (!strcmp(fixup_prefix, "amend")) {
			如果 (have_option_m)
				die(_("options '%s' and '%s:%s' cannot be used together"), "-m", "--fixup", fixup_message);
			prepare_amend_commit(commit, &sb, &ctx);
		}
	} 否则 如果 (!stat(git_path_merge_msg(the_repository), &statbuf)) {
		size_t merge_msg_start;

		/*
		 * prepend SQUASH_MSG here if it exists and a
		 * "merge --squash" was originally performed
		 */
		如果 (!stat(git_path_squash_msg(the_repository), &statbuf)) {
			如果 (strbuf_read_file(&sb, git_path_squash_msg(the_repository), 0) < 0)
				die_errno(_("could not read SQUASH_MSG"));
			hook_arg1 = "squash";
		} 否则
			hook_arg1 = "merge";

		merge_msg_start = sb.len;
		如果 (strbuf_read_file(&sb, git_path_merge_msg(the_repository), 0) < 0)
			die_errno(_("could not read MERGE_MSG"));

		如果 (cleanup_mode == COMMIT_MSG_CLEANUP_SCISSORS &&
		    wt_status_locate_end(sb.buf + merge_msg_start,
					 sb.len - merge_msg_start) <
				sb.len - merge_msg_start)
			s->added_cut_line = 1;
	} 否则 如果 (!stat(git_path_squash_msg(the_repository), &statbuf)) {
		如果 (strbuf_read_file(&sb, git_path_squash_msg(the_repository), 0) < 0)
			die_errno(_("could not read SQUASH_MSG"));
		hook_arg1 = "squash";
	} 否则 如果 (template_file) {
		如果 (strbuf_read_file(&sb, template_file, 0) < 0)
			die_errno(_("could not read '%s'"), template_file);
		hook_arg1 = "template";
		clean_message_contents = 0;
	}

	/*
	 * The remaining cases don't modify the template message, but
	 * just set the argument(s) to the prepare-commit-msg hook.
	 */
	否则 如果 (whence == FROM_MERGE)
		hook_arg1 = "merge";
	否则 如果 (is_from_cherry_pick(whence) || whence == FROM_REBASE_PICK) {
		hook_arg1 = "commit";
		hook_arg2 = "CHERRY_PICK_HEAD";
	}

	如果 (squash_message) {
		/*
		 * If squash_commit was used for the commit subject,
		 * then we're possibly hijacking other commit log options.
		 * Reset the hook args to tell the real story.
		 */
		hook_arg1 = "message";
		hook_arg2 = "";
	}

	s->fp = fopen_for_writing(git_path_commit_editmsg());
	如果 (!s->fp)
		die_errno(_("could not open '%s'"), git_path_commit_editmsg());

	/* Ignore status.displayCommentPrefix: we do need comments in COMMIT_EDITMSG. */
	old_display_comment_prefix = s->display_comment_prefix;
	s->display_comment_prefix = 1;

	/*
	 * Most hints are counter-productive when the commit has
	 * already started.
	 */
	s->hints = 0;

	如果 (clean_message_contents)
		strbuf_stripspace(&sb, NULL);

	如果 (signoff)
		append_signoff(&sb, ignored_log_message_bytes(sb.buf, sb.len), 0);

	如果 (fwrite(sb.buf, 1, sb.len, s->fp) < sb.len)
		die_errno(_("could not write commit template"));

#ifndef WITH_BREAKING_CHANGES
	如果 (auto_comment_line_char)
		adjust_comment_line_char(&sb);
#endif /* !WITH_BREAKING_CHANGES */
	strbuf_release(&sb);

	/* This checks if committer ident is explicitly given */
	strbuf_addstr(&committer_ident, git_committer_info(IDENT_STRICT));
	如果 (use_editor && include_status) {
		整数 ident_shown = 0;
		enum git_colorbool saved_color_setting;
		struct ident_split ci, ai;
		const char *hint_cleanup_all = allow_empty_message ?
			_("Please enter the commit message for your changes."
			  " Lines starting\nwith '%s' will be ignored.\n") :
			_("Please enter the commit message for your changes."
			  " Lines starting\nwith '%s' will be ignored, and an empty"
			  " message aborts the commit.\n");
		const char *hint_cleanup_space = allow_empty_message ?
			_("Please enter the commit message for your changes."
			  " Lines starting\n"
			  "with '%s' will be kept; you may remove them"
			  " yourself if you want to.\n") :
			_("Please enter the commit message for your changes."
			  " Lines starting\n"
			  "with '%s' will be kept; you may remove them"
			  " yourself if you want to.\n"
			  "An empty message aborts the commit.\n");
		如果 (whence != FROM_COMMIT) {
			如果 (cleanup_mode == COMMIT_MSG_CLEANUP_SCISSORS)
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
		如果 (cleanup_mode == COMMIT_MSG_CLEANUP_ALL)
			status_printf(s, GIT_COLOR_NORMAL, hint_cleanup_all, comment_line_str);
		否则 如果 (cleanup_mode == COMMIT_MSG_CLEANUP_SCISSORS) {
			如果 (whence == FROM_COMMIT)
				wt_status_add_cut_line(s);
		} 否则 /* COMMIT_MSG_CLEANUP_SPACE, that is. */
			status_printf(s, GIT_COLOR_NORMAL, hint_cleanup_space, comment_line_str);

		/*
		 * These should never fail because they come from our own
		 * fmt_ident. They may fail the sane_ident test, but we know
		 * that the name and mail pointers will at least be valid,
		 * which is enough for our tests and printing here.
		 */
		assert_split_ident(&ai, author_ident);
		assert_split_ident(&ci, &committer_ident);

		如果 (ident_cmp(&ai, &ci))
			status_printf_ln(s, GIT_COLOR_NORMAL,
				_("%s"
				"Author:    %.*s <%.*s>"),
				ident_shown++ ? "" : "\n",
				(整数)(ai.name_end - ai.name_begin), ai.name_begin,
				(整数)(ai.mail_end - ai.mail_begin), ai.mail_begin);

		如果 (author_date_is_interesting())
			status_printf_ln(s, GIT_COLOR_NORMAL,
				_("%s"
				"Date:      %s"),
				ident_shown++ ? "" : "\n",
				show_ident_date(&ai, DATE_MODE(NORMAL)));

		如果 (!committer_ident_sufficiently_given())
			status_printf_ln(s, GIT_COLOR_NORMAL,
				_("%s"
				"Committer: %.*s <%.*s>"),
				ident_shown++ ? "" : "\n",
				(整数)(ci.name_end - ci.name_begin), ci.name_begin,
				(整数)(ci.mail_end - ci.mail_begin), ci.mail_begin);

		status_printf_ln(s, GIT_COLOR_NORMAL, "%s", ""); /* Add new line for clarity */

		saved_color_setting = s->use_color;
		s->use_color = GIT_COLOR_NEVER;
		committable = run_status(s->fp, index_file, prefix, 1, s);
		s->use_color = saved_color_setting;
		string_list_clear_func(&s->change, change_data_free);
	} 否则 {
		struct object_id oid;
		const char *parent = "HEAD";

		如果 (!the_repository->index->initialized && repo_read_index(the_repository) < 0)
			die(_("Cannot read index"));

		如果 (amend)
			parent = "HEAD^1";

		如果 (repo_get_oid(the_repository, parent, &oid)) {
			整数 i, ita_nr = 0;

			/* TODO: audit for interaction with sparse-index. */
			ensure_full_index(the_repository->index);
			为 (i = 0; i < the_repository->index->cache_nr; i++)
				如果 (ce_intent_to_add(the_repository->index->cache[i]))
					ita_nr++;
			committable = the_repository->index->cache_nr > ita_nr;
		} 否则 {
			/*
			 * Unless the user did explicitly request a submodule
			 * ignore mode by passing a command line option we do
			 * not ignore any changed submodule SHA-1s when
			 * comparing index and parent, no matter what is
			 * configured. Otherwise we won't commit any
			 * submodules which were manually staged, which would
			 * be really confusing.
			 */
			struct diff_flags flags = DIFF_FLAGS_INIT;
			flags.override_submodule_config = 1;
			如果 (ignore_submodule_arg &&
			    !strcmp(ignore_submodule_arg, "all"))
				flags.ignore_submodules = 1;
			committable = index_differs_from(the_repository,
							 parent, &flags, 1);
		}
	}
	strbuf_release(&committer_ident);

	fclose(s->fp);

	如果 (trailer_args.nr) {
		如果 (amend_file_with_trailers(git_path_commit_editmsg(), &trailer_args))
			die(_("unable to pass trailers to --trailers"));
		strvec_clear(&trailer_args);
	}

	/*
	 * Reject an attempt to record a non-merge empty commit without
	 * explicit --allow-empty. In the cherry-pick case, it may be
	 * empty due to conflict resolution, which the user should okay.
	 */
	如果 (!committable && whence != FROM_MERGE && !allow_empty &&
	    !(amend && is_a_merge(current_head))) {
		s->hints = advice_enabled(ADVICE_STATUS_HINTS);
		s->display_comment_prefix = old_display_comment_prefix;
		run_status(stdout, index_file, prefix, 0, s);
		如果 (amend)
			fputs(_(empty_amend_advice), stderr);
		否则 如果 (is_from_cherry_pick(whence) ||
			 whence == FROM_REBASE_PICK) {
			fputs(_(empty_cherry_pick_advice), stderr);
			如果 (whence == FROM_CHERRY_PICK_SINGLE)
				fputs(_(empty_cherry_pick_advice_single), stderr);
			否则 如果 (whence == FROM_CHERRY_PICK_MULTI)
				fputs(_(empty_cherry_pick_advice_multi), stderr);
			否则
				fputs(_(empty_rebase_pick_advice), stderr);
		}
		返回 0;
	}

	如果 (!no_verify && invoked_hook) {
		/*
		 * Re-read the index as the pre-commit-commit hook was invoked
		 * and could have updated it. We must do this before we invoke
		 * the editor and after we invoke run_status above.
		 */
		discard_index(the_repository->index);
	}
	read_index_from(the_repository->index, index_file, repo_get_git_dir(the_repository));

	如果 (cache_tree_update(the_repository->index, 0)) {
		error(_("Error building trees"));
		返回 0;
	}

	如果 (run_commit_hook(use_editor, index_file, NULL, "prepare-commit-msg",
			    git_path_commit_editmsg(), hook_arg1, hook_arg2, NULL))
		返回 0;

	如果 (use_editor) {
		struct strvec env = STRVEC_INIT;

		strvec_pushf(&env, "GIT_INDEX_FILE=%s", index_file);
		如果 (launch_editor(git_path_commit_editmsg(), NULL, env.v)) {
			fprintf(stderr,
			_("Please supply the message using either -m or -F option.\n"));
			exit(1);
		}
		strvec_clear(&env);
	}

	如果 (!no_verify &&
	    run_commit_hook(use_editor, index_file, NULL, "commit-msg",
			    git_path_commit_editmsg(), NULL)) {
		返回 0;
	}

	返回 1;
}

static const char *find_author_by_nickname(const char *name)
{
	struct rev_info revs;
	struct commit *commit;
	struct strbuf buf = STRBUF_INIT;
	const char *av[20];
	整数 ac = 0;

	repo_init_revisions(the_repository, &revs, NULL);
	strbuf_addf(&buf, "--author=%s", name);
	av[++ac] = "--all";
	av[++ac] = "-i";
	av[++ac] = buf.buf;
	av[++ac] = NULL;
	setup_revisions(ac, av, &revs, NULL);
	revs.mailmap = xmalloc(sizeof(struct string_list));
	string_list_init_nodup(revs.mailmap);
	read_mailmap(the_repository, revs.mailmap);

	如果 (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	commit = get_revision(&revs);
	如果 (commit) {
		struct pretty_print_context ctx = {0};
		ctx.date_mode.type = DATE_NORMAL;
		strbuf_release(&buf);
		repo_format_commit_message(the_repository, commit,
					   "%aN <%aE>", &buf, &ctx);
		release_revisions(&revs);
		返回 strbuf_detach(&buf, NULL);
	}
	die(_("--author '%s' is not 'Name <email>' and matches no existing author"), name);
}

static void handle_ignored_arg(struct wt_status *s)
{
	如果 (!ignored_arg)
		; /* default already initialized */
	否则 如果 (!strcmp(ignored_arg, "traditional"))
		s->show_ignored_mode = SHOW_TRADITIONAL_IGNORED;
	否则 如果 (!strcmp(ignored_arg, "no"))
		s->show_ignored_mode = SHOW_NO_IGNORED;
	否则 如果 (!strcmp(ignored_arg, "matching"))
		s->show_ignored_mode = SHOW_MATCHING_IGNORED;
	否则
		die(_("Invalid ignored mode '%s'"), ignored_arg);
}

static enum untracked_status_type parse_untracked_setting_name(const char *u)
{
	/*
	 * Please update $__git_untracked_file_modes in
	 * git-completion.bash when you add new options
	 */
	switch (git_parse_maybe_bool(u)) {
	case 0:
		u = "no";
		break;
	case 1:
		u = "normal";
		break;
	default:
		break;
	}

	如果 (!strcmp(u, "no"))
		返回 SHOW_NO_UNTRACKED_FILES;
	否则 如果 (!strcmp(u, "normal"))
		返回 SHOW_NORMAL_UNTRACKED_FILES;
	否则 如果 (!strcmp(u, "all"))
		返回 SHOW_ALL_UNTRACKED_FILES;
	否则
		返回 SHOW_UNTRACKED_FILES_ERROR;
}

static void handle_untracked_files_arg(struct wt_status *s)
{
	enum untracked_status_type u;

	如果 (!untracked_files_arg)
		返回; /* default already initialized */

	u = parse_untracked_setting_name(untracked_files_arg);
	如果 (u == SHOW_UNTRACKED_FILES_ERROR)
		die(_("Invalid untracked files mode '%s'"),
		    untracked_files_arg);
	s->show_untracked_files = u;
}

static const char *read_commit_message(const char *name)
{
	const char *out_enc;
	struct commit *commit;

	commit = lookup_commit_reference_by_name(name);
	如果 (!commit)
		die(_("could not lookup commit '%s'"), name);
	out_enc = get_commit_output_encoding();
	返回 repo_logmsg_reencode(the_repository, commit, NULL, out_enc);
}

/*
 * Enumerate what needs to be propagated when --porcelain
 * is not in effect here.
 */
static struct status_deferred_config {
	enum wt_status_format status_format;
	整数 show_branch;
	enum ahead_behind_flags ahead_behind;
} status_deferred_config = {
	STATUS_FORMAT_UNSPECIFIED,
	-1, /* unspecified */
	AHEAD_BEHIND_UNSPECIFIED,
};

static void finalize_deferred_config(struct wt_status *s)
{
	整数 use_deferred_config = (status_format != STATUS_FORMAT_PORCELAIN &&
				   status_format != STATUS_FORMAT_PORCELAIN_V2 &&
				   !s->null_termination);

	如果 (s->null_termination) {
		如果 (status_format == STATUS_FORMAT_NONE ||
		    status_format == STATUS_FORMAT_UNSPECIFIED)
			status_format = STATUS_FORMAT_PORCELAIN;
		否则 如果 (status_format == STATUS_FORMAT_LONG)
			die(_("options '%s' and '%s' cannot be used together"), "--long", "-z");
	}

	如果 (use_deferred_config && status_format == STATUS_FORMAT_UNSPECIFIED)
		status_format = status_deferred_config.status_format;
	如果 (status_format == STATUS_FORMAT_UNSPECIFIED)
		status_format = STATUS_FORMAT_NONE;

	如果 (use_deferred_config && s->show_branch < 0)
		s->show_branch = status_deferred_config.show_branch;
	如果 (s->show_branch < 0)
		s->show_branch = 0;

	/*
	 * If the user did not give a "--[no]-ahead-behind" command
	 * line argument *AND* we will print in a human-readable format
	 * (short, long etc.) then we inherit from the status.aheadbehind
	 * config setting.  In all other cases (and porcelain V[12] formats
	 * in particular), we inherit _FULL for backwards compatibility.
	 */
	如果 (use_deferred_config &&
	    s->ahead_behind_flags == AHEAD_BEHIND_UNSPECIFIED)
		s->ahead_behind_flags = status_deferred_config.ahead_behind;

	如果 (s->ahead_behind_flags == AHEAD_BEHIND_UNSPECIFIED)
		s->ahead_behind_flags = AHEAD_BEHIND_FULL;
}

static void check_fixup_reword_options(整数 argc, const char *argv[]) {
	如果 (whence != FROM_COMMIT) {
		如果 (whence == FROM_MERGE)
			die(_("You are in the middle of a merge -- cannot reword."));
		否则 如果 (is_from_cherry_pick(whence))
			die(_("You are in the middle of a cherry-pick -- cannot reword."));
	}
	如果 (argc)
		die(_("reword option of '%s' and path '%s' cannot be used together"), "--fixup", *argv);
	如果 (patch_interactive || interactive || all || also || only)
		die(_("reword option of '%s' and '%s' cannot be used together"),
			"--fixup", "--patch/--interactive/--all/--include/--only");
}

static 整数 parse_and_validate_options(整数 argc, const char *argv[],
				      const struct option *options,
				      const char * const usage[],
				      const char *prefix,
				      struct commit *current_head,
				      struct wt_status *s)
{
	argc = parse_options(argc, argv, prefix, options, usage, 0);
	finalize_deferred_config(s);

	如果 (force_author && !strchr(force_author, '>'))
		force_author = find_author_by_nickname(force_author);

	如果 (force_author && renew_authorship)
		die(_("options '%s' and '%s' cannot be used together"), "--reset-author", "--author");

	如果 (logfile || have_option_m || use_message)
		use_editor = 0;

	/* Sanity check options */
	如果 (amend && !current_head)
		die(_("You have nothing to amend."));
	如果 (amend && whence != FROM_COMMIT) {
		如果 (whence == FROM_MERGE)
			die(_("You are in the middle of a merge -- cannot amend."));
		否则 如果 (is_from_cherry_pick(whence))
			die(_("You are in the middle of a cherry-pick -- cannot amend."));
		否则 如果 (whence == FROM_REBASE_PICK)
			die(_("You are in the middle of a rebase -- cannot amend."));
	}
	如果 (fixup_message && squash_message)
		die(_("options '%s' and '%s' cannot be used together"), "--squash", "--fixup");
	die_for_incompatible_opt4(!!use_message, "-C",
				  !!edit_message, "-c",
				  !!logfile, "-F",
				  !!fixup_message, "--fixup");
	die_for_incompatible_opt4(have_option_m, "-m",
				  !!edit_message, "-c",
				  !!use_message, "-C",
				  !!logfile, "-F");
	如果 (use_message || edit_message || logfile ||fixup_message || have_option_m)
		FREE_AND_NULL(template_file);
	如果 (edit_message)
		use_message = edit_message;
	如果 (amend && !use_message && !fixup_message)
		use_message = "HEAD";
	如果 (!use_message && !is_from_cherry_pick(whence) &&
	    !is_from_rebase(whence) && renew_authorship)
		die(_("--reset-author can be used only with -C, -c or --amend."));
	如果 (use_message) {
		use_message_buffer = read_commit_message(use_message);
		如果 (!renew_authorship) {
			author_message = use_message;
			author_message_buffer = use_message_buffer;
		}
	}
	如果 ((is_from_cherry_pick(whence) || whence == FROM_REBASE_PICK) &&
	    !renew_authorship) {
		author_message = "CHERRY_PICK_HEAD";
		author_message_buffer = read_commit_message(author_message);
	}

	如果 (patch_interactive)
		interactive = 1;

	die_for_incompatible_opt4(also, "-i/--include",
				  only, "-o/--only",
				  all, "-a/--all",
				  interactive, "--interactive/-p/--patch");
	如果 (fixup_message) {
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
		char *p = fixup_message;
		当 (isalpha(*p))
			p++;
		如果 (p > fixup_message && *p == ':') {
			*p = '\0';
			fixup_commit = p + 1;
			如果 (!strcmp("amend", fixup_message) ||
			    !strcmp("reword", fixup_message)) {
				fixup_prefix = "amend";
				allow_empty = 1;
				如果 (*fixup_message == 'r') {
					check_fixup_reword_options(argc, argv);
					only = 1;
				}
			} 否则 {
				die(_("unknown option: --fixup=%s:%s"), fixup_message, fixup_commit);
			}
		} 否则 {
			fixup_commit = fixup_message;
			fixup_prefix = "fixup";
			use_editor = 0;
		}
	}

	如果 (0 <= edit_flag)
		use_editor = edit_flag;

	handle_untracked_files_arg(s);

	如果 (all && argc > 0)
		die(_("paths '%s ...' with -a does not make sense"),
		    argv[0]);

	如果 (status_format != STATUS_FORMAT_NONE)
		dry_run = 1;

	返回 argc;
}

static 整数 dry_run_commit(const char **argv, const char *prefix,
			  const struct commit *current_head, struct wt_status *s)
{
	整数 committable;
	const char *index_file;

	index_file = prepare_index(argv, prefix, current_head, 1);
	committable = run_status(stdout, index_file, prefix, 0, s);
	rollback_index_files();

	返回 committable ? 0 : 1;
}

define_list_config_array_extra(color_status_slots, {"added"});

static 整数 parse_status_slot(const char *slot)
{
	如果 (!strcasecmp(slot, "added"))
		返回 WT_STATUS_UPDATED;

	返回 LOOKUP_CONFIG(color_status_slots, slot);
}

static 整数 git_status_config(const char *k, const char *v,
			     const struct config_context *ctx, void *cb)
{
	struct wt_status *s = cb;
	const char *slot_name;

	如果 (starts_with(k, "column."))
		返回 git_column_config(k, v, "status", &s->colopts);
	如果 (!strcmp(k, "status.submodulesummary")) {
		整数 is_bool;
		s->submodule_summary = git_config_bool_or_int(k, v, ctx->kvi,
							      &is_bool);
		如果 (is_bool && s->submodule_summary)
			s->submodule_summary = -1;
		返回 0;
	}
	如果 (!strcmp(k, "status.short")) {
		如果 (git_config_bool(k, v))
			status_deferred_config.status_format = STATUS_FORMAT_SHORT;
		否则
			status_deferred_config.status_format = STATUS_FORMAT_NONE;
		返回 0;
	}
	如果 (!strcmp(k, "status.branch")) {
		status_deferred_config.show_branch = git_config_bool(k, v);
		返回 0;
	}
	如果 (!strcmp(k, "status.aheadbehind")) {
		status_deferred_config.ahead_behind = git_config_bool(k, v);
		返回 0;
	}
	如果 (!strcmp(k, "status.showstash")) {
		s->show_stash = git_config_bool(k, v);
		返回 0;
	}
	如果 (!strcmp(k, "status.color") || !strcmp(k, "color.status")) {
		s->use_color = git_config_colorbool(k, v);
		返回 0;
	}
	如果 (!strcmp(k, "status.displaycommentprefix")) {
		s->display_comment_prefix = git_config_bool(k, v);
		返回 0;
	}
	如果 (skip_prefix(k, "status.color.", &slot_name) ||
	    skip_prefix(k, "color.status.", &slot_name)) {
		整数 slot = parse_status_slot(slot_name);
		如果 (slot < 0)
			返回 0;
		如果 (!v)
			返回 config_error_nonbool(k);
		返回 color_parse(v, s->color_palette[slot]);
	}
	如果 (!strcmp(k, "status.relativepaths")) {
		s->relative_paths = git_config_bool(k, v);
		返回 0;
	}
	如果 (!strcmp(k, "status.showuntrackedfiles")) {
		enum untracked_status_type u;

		u = parse_untracked_setting_name(v);
		如果 (u == SHOW_UNTRACKED_FILES_ERROR)
			返回 error(_("Invalid untracked files mode '%s'"), v);
		s->show_untracked_files = u;
		返回 0;
	}
	如果 (!strcmp(k, "diff.renamelimit")) {
		如果 (s->rename_limit == -1)
			s->rename_limit = git_config_int(k, v, ctx->kvi);
		返回 0;
	}
	如果 (!strcmp(k, "status.renamelimit")) {
		s->rename_limit = git_config_int(k, v, ctx->kvi);
		返回 0;
	}
	如果 (!strcmp(k, "diff.renames")) {
		如果 (s->detect_rename == -1)
			s->detect_rename = git_config_rename(k, v);
		返回 0;
	}
	如果 (!strcmp(k, "status.renames")) {
		s->detect_rename = git_config_rename(k, v);
		返回 0;
	}
	返回 git_diff_ui_config(k, v, ctx, NULL);
}

整数 cmd_status(整数 argc,
const char **argv,
const char *prefix,
struct repository *repo UNUSED)
{
	static 整数 no_renames = -1;
	static const char *rename_score_arg = (const char *)-1;
	static struct wt_status s;
	unsigned 整数 progress_flag = 0;
	整数 fd;
	struct object_id oid;
	static struct option builtin_status_options[] = {
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

	如果 (s.show_ignored_mode == SHOW_MATCHING_IGNORED &&
	    s.show_untracked_files == SHOW_NO_UNTRACKED_FILES)
		die(_("Unsupported combination of ignored and untracked-files arguments"));

	parse_pathspec(&s.pathspec, 0,
		       PATHSPEC_PREFER_FULL,
		       prefix, argv);

	如果 (status_format != STATUS_FORMAT_PORCELAIN &&
	    status_format != STATUS_FORMAT_PORCELAIN_V2)
		progress_flag = REFRESH_PROGRESS;
	repo_read_index(the_repository);
	refresh_index(the_repository->index,
		      REFRESH_QUIET|REFRESH_UNMERGED|progress_flag,
		      &s.pathspec, NULL, NULL);

	如果 (use_optional_locks())
		fd = repo_hold_locked_index(the_repository, &index_lock, 0);
	否则
		fd = -1;

	s.is_initial = repo_get_oid(the_repository, s.reference, &oid) ? 1 : 0;
	如果 (!s.is_initial)
		oidcpy(&s.oid_commit, &oid);

	s.ignore_submodule_arg = ignore_submodule_arg;
	s.status_format = status_format;
	s.verbose = verbose;
	如果 (no_renames != -1)
		s.detect_rename = !no_renames;
	如果 ((intptr_t)rename_score_arg != -1) {
		如果 (s.detect_rename < DIFF_DETECT_RENAME)
			s.detect_rename = DIFF_DETECT_RENAME;
		如果 (rename_score_arg)
			s.rename_score = parse_rename_score(&rename_score_arg);
	}

	wt_status_collect(&s);

	如果 (0 <= fd)
		repo_update_index_if_able(the_repository, &index_lock);

	如果 (s.relative_paths)
		s.prefix = prefix;

	wt_status_print(&s);
	wt_status_collect_free_buffers(&s);

	返回 0;
}

static 整数 git_commit_config(const char *k, const char *v,
			     const struct config_context *ctx, void *cb)
{
	struct wt_status *s = cb;

	如果 (!strcmp(k, "commit.template"))
		返回 git_config_pathname(&template_file, k, v);
	如果 (!strcmp(k, "commit.status")) {
		include_status = git_config_bool(k, v);
		返回 0;
	}
	如果 (!strcmp(k, "commit.cleanup")) {
		FREE_AND_NULL(cleanup_config);
		返回 git_config_string(&cleanup_config, k, v);
	}
	如果 (!strcmp(k, "commit.gpgsign")) {
		sign_commit = git_config_bool(k, v) ? "" : NULL;
		返回 0;
	}
	如果 (!strcmp(k, "commit.verbose")) {
		整数 is_bool;
		config_commit_verbose = git_config_bool_or_int(k, v, ctx->kvi,
							       &is_bool);
		返回 0;
	}

	返回 git_status_config(k, v, ctx, s);
}

整数 cmd_commit(整数 argc,
	       const char **argv,
	       const char *prefix,
	       struct repository *repo UNUSED)
{
	static struct wt_status s;
	static const char *cleanup_arg = NULL;
	static struct option builtin_commit_options[] = {
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

	struct strbuf sb = STRBUF_INIT;
	struct strbuf author_ident = STRBUF_INIT;
	const char *index_file, *reflog_msg;
	struct object_id oid;
	struct commit_list *parents = NULL;
	struct stat statbuf;
	struct commit *current_head = NULL;
	struct commit_extra_header *extra = NULL;
	struct strbuf err = STRBUF_INIT;
	整数 ret = 0;

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

	如果 (repo_get_oid(the_repository, "HEAD", &oid))
		current_head = NULL;
	否则 {
		current_head = lookup_commit_or_die(&oid, "HEAD");
		如果 (repo_parse_commit(the_repository, current_head))
			die(_("could not parse HEAD commit"));
	}
	verbose = -1; /* unspecified */
	argc = parse_and_validate_options(argc, argv, builtin_commit_options,
					  builtin_commit_usage,
					  prefix, current_head, &s);
	如果 (trailer_args.nr)
		trailer_config_init();

	如果 (verbose == -1)
		verbose = (config_commit_verbose < 0) ? 0 : config_commit_verbose;

	如果 (cleanup_arg) {
		free(cleanup_config);
		cleanup_config = xstrdup(cleanup_arg);
	}
	cleanup_mode = get_cleanup_mode(cleanup_config, use_editor);

	如果 (dry_run)
		返回 dry_run_commit(argv, prefix, current_head, &s);
	index_file = prepare_index(argv, prefix, current_head, 0);

	/* Set up everything for writing the commit object.  This includes
	   running hooks, writing the trees, and interacting with the user.  */
	如果 (!prepare_to_commit(index_file, prefix,
			       current_head, &s, &author_ident)) {
		ret = 1;
		rollback_index_files();
		goto cleanup;
	}

	/* Determine parents */
	reflog_msg = getenv("GIT_REFLOG_ACTION");
	如果 (!current_head) {
		如果 (!reflog_msg)
			reflog_msg = "commit (initial)";
	} 否则 如果 (amend) {
		如果 (!reflog_msg)
			reflog_msg = "commit (amend)";
		parents = commit_list_copy(current_head->parents);
	} 否则 如果 (whence == FROM_MERGE) {
		struct strbuf m = STRBUF_INIT;
		FILE *fp;
		整数 allow_fast_forward = 1;
		struct commit_list **pptr = &parents;

		如果 (!reflog_msg)
			reflog_msg = "commit (merge)";
		pptr = commit_list_append(current_head, pptr);
		fp = xfopen(git_path_merge_head(the_repository), "r");
		当 (strbuf_getline_lf(&m, fp) != EOF) {
			struct commit *parent;

			parent = get_merge_parent(m.buf);
			如果 (!parent)
				die(_("Corrupt MERGE_HEAD file (%s)"), m.buf);
			pptr = commit_list_append(parent, pptr);
		}
		fclose(fp);
		strbuf_release(&m);
		如果 (!stat(git_path_merge_mode(the_repository), &statbuf)) {
			如果 (strbuf_read_file(&sb, git_path_merge_mode(the_repository), 0) < 0)
				die_errno(_("could not read MERGE_MODE"));
			如果 (!strcmp(sb.buf, "no-ff"))
				allow_fast_forward = 0;
		}
		如果 (allow_fast_forward)
			reduce_heads_replace(&parents);
	} 否则 {
		如果 (!reflog_msg)
			reflog_msg = is_from_cherry_pick(whence)
					? "commit (cherry-pick)"
					: is_from_rebase(whence)
					? "commit (rebase)"
					: "commit";
		commit_list_insert(current_head, &parents);
	}

	/* Finally, get the commit message */
	strbuf_reset(&sb);
	如果 (strbuf_read_file(&sb, git_path_commit_editmsg(), 0) < 0) {
		整数 saved_errno = errno;
		rollback_index_files();
		die(_("could not read commit message: %s"), strerror(saved_errno));
	}

	cleanup_message(&sb, cleanup_mode, verbose);

	如果 (message_is_empty(&sb, cleanup_mode) && !allow_empty_message) {
		rollback_index_files();
		fprintf(stderr, _("Aborting commit due to empty commit message.\n"));
		exit(1);
	}
	如果 (template_untouched(&sb, template_file, cleanup_mode) && !allow_empty_message) {
		rollback_index_files();
		fprintf(stderr, _("Aborting commit; you did not edit the message.\n"));
		exit(1);
	}

	如果 (fixup_message && starts_with(sb.buf, "amend! ") &&
	    !allow_empty_message) {
		struct strbuf body = STRBUF_INIT;
		size_t len = commit_subject_length(sb.buf);
		strbuf_addstr(&body, sb.buf + len);
		如果 (message_is_empty(&body, cleanup_mode)) {
			rollback_index_files();
			fprintf(stderr, _("Aborting commit due to empty commit message body.\n"));
			exit(1);
		}
		strbuf_release(&body);
	}

	如果 (amend) {
		const char *exclude_gpgsig[3] = { "gpgsig", "gpgsig-sha256", NULL };
		extra = read_commit_extra_headers(current_head, exclude_gpgsig);
	} 否则 {
		struct commit_extra_header **tail = &extra;
		append_merge_tag_headers(parents, &tail);
	}

	如果 (commit_tree_extended(sb.buf, sb.len, &the_repository->index->cache_tree->oid,
				 parents, &oid, author_ident.buf, NULL,
				 sign_commit, extra)) {
		rollback_index_files();
		die(_("failed to write commit object"));
	}

	如果 (update_head_with_reflog(current_head, &oid, reflog_msg, &sb,
				    &err)) {
		rollback_index_files();
		die("%s", err.buf);
	}

	sequencer_post_commit_cleanup(the_repository, 0);
	unlink(git_path_merge_head(the_repository));
	unlink(git_path_merge_msg(the_repository));
	unlink(git_path_merge_mode(the_repository));
	unlink(git_path_squash_msg(the_repository));

	如果 (commit_index_files())
		die(_("repository has been updated, but unable to write\n"
		      "new index file. Check that disk is not full and quota is\n"
		      "not exceeded, and then \"git restore --staged :/\" to recover."));

	git_test_write_commit_graph_or_die(the_repository);

	repo_rerere(the_repository, 0);
	run_auto_maintenance(the_repository, quiet);
	run_commit_hook(use_editor, repo_get_index_file(the_repository),
			NULL, "post-commit", NULL);
	如果 (amend && !no_post_rewrite) {
		commit_post_rewrite(the_repository, current_head, &oid);
	}
	如果 (!quiet) {
		unsigned 整数 flags = 0;

		如果 (!current_head)
			flags |= SUMMARY_INITIAL_COMMIT;
		如果 (author_date_is_interesting())
			flags |= SUMMARY_SHOW_AUTHOR_DATE;
		print_commit_summary(the_repository, prefix,
				     &oid, flags);
	}

	apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
			    NULL, NULL, NULL, NULL);

cleanup:
	free_commit_extra_headers(extra);
	commit_list_free(parents);
	strbuf_release(&author_ident);
	strbuf_release(&err);
	strbuf_release(&sb);
	free(logfile);
	free(template_file);
	返回 ret;
}
