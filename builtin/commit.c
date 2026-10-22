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
static số_nguyên all, also, interactive, patch_interactive, only, amend, signoff;
static struct interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
static số_nguyên edit_flag = -1; /* unspecified */
static số_nguyên quiet, verbose, no_verify, allow_empty, dry_run, renew_authorship;
static số_nguyên config_commit_verbose = -1; /* unspecified */
static số_nguyên no_post_rewrite, allow_empty_message, pathspec_file_nul;
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
static số_nguyên use_editor = 1, include_status = 1;
static số_nguyên have_option_m;
static struct strbuf message = STRBUF_INIT;

static enum wt_status_format status_format = STATUS_FORMAT_UNSPECIFIED;

static số_nguyên opt_parse_porcelain(const struct option *opt, const char *arg, số_nguyên unset)
{
	enum wt_status_format *value = (enum wt_status_format *)opt->value;
	nếu (unset)
		*value = STATUS_FORMAT_NONE;
	không_thì nếu (!arg)
		*value = STATUS_FORMAT_PORCELAIN;
	không_thì nếu (!strcmp(arg, "v1") || !strcmp(arg, "1"))
		*value = STATUS_FORMAT_PORCELAIN;
	không_thì nếu (!strcmp(arg, "v2") || !strcmp(arg, "2"))
		*value = STATUS_FORMAT_PORCELAIN_V2;
	không_thì
		die("unsupported porcelain version '%s'", arg);

	trả_về 0;
}

static số_nguyên opt_parse_m(const struct option *opt, const char *arg, số_nguyên unset)
{
	struct strbuf *buf = opt->value;
	nếu (unset) {
		have_option_m = 0;
		strbuf_setlen(buf, 0);
	} không_thì {
		have_option_m = 1;
		nếu (buf->len)
			strbuf_addch(buf, '\n');
		strbuf_addstr(buf, arg);
		strbuf_complete_line(buf);
	}
	trả_về 0;
}

static số_nguyên opt_parse_rename_score(const struct option *opt, const char *arg, số_nguyên unset)
{
	const char **value = opt->value;

	BUG_ON_OPT_NEG(unset);

	nếu (arg != NULL && *arg == '=')
		arg = arg + 1;

	*value = arg;
	trả_về 0;
}

static void determine_whence(struct wt_status *s)
{
	nếu (file_exists(git_path_merge_head(the_repository)))
		whence = FROM_MERGE;
	không_thì nếu (!sequencer_determine_whence(the_repository, &whence))
		whence = FROM_COMMIT;
	nếu (s)
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

static số_nguyên commit_index_files(void)
{
	số_nguyên err = 0;

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

	trả_về err;
}

/*
 * Take a union of paths in the index and the named tree (typically, "HEAD"),
 * and return the paths that match the given pattern in list.
 */
static số_nguyên list_paths(struct string_list *list, const char *with_tree,
		      const struct pathspec *pattern)
{
	số_nguyên i, ret;
	char *m;

	nếu (!pattern->nr)
		trả_về 0;

	m = xcalloc(1, pattern->nr);

	nếu (with_tree) {
		char *max_prefix = common_prefix(pattern);
		overlay_tree_on_index(the_repository->index, with_tree, max_prefix);
		free(max_prefix);
	}

	/* TODO: audit for interaction with sparse-index. */
	ensure_full_index(the_repository->index);
	cho (i = 0; i < the_repository->index->cache_nr; i++) {
		const struct cache_entry *ce = the_repository->index->cache[i];
		struct string_list_item *item;

		nếu (ce->ce_flags & CE_UPDATE)
			continue;
		nếu (!ce_path_match(the_repository->index, ce, pattern, m))
			continue;
		item = string_list_insert(list, ce->name);
		nếu (ce_skip_worktree(ce))
			item->util = item; /* better a valid pointer than a fake one */
	}

	ret = report_path_error(m, pattern);
	free(m);
	trả_về ret;
}

static void add_remove_files(struct string_list *list)
{
	số_nguyên i;
	cho (i = 0; i < list->nr; i++) {
		struct stat st;
		struct string_list_item *p = &(list->items[i]);

		/* p->util is skip-worktree */
		nếu (p->util)
			continue;

		nếu (!lstat(p->string, &st)) {
			nếu (add_to_index(the_repository->index, p->string, &st, 0))
				die(_("updating files failed"));
		} không_thì
			remove_file_from_index(the_repository->index, p->string);
	}
}

static void create_base_index(const struct commit *current_head)
{
	struct tree *tree;
	struct unpack_trees_options opts;
	struct tree_desc t;

	nếu (!current_head) {
		discard_index(the_repository->index);
		trả_về;
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
	nếu (!tree)
		die(_("failed to unpack HEAD tree object"));
	nếu (repo_parse_tree(the_repository, tree) < 0)
		exit(128);
	init_tree_desc(&t, &tree->object.oid, tree->buffer, tree->size);
	nếu (unpack_trees(1, &t, &opts))
		exit(128); /* We've already reported the error, finish dying */
}

static void refresh_cache_or_die(số_nguyên refresh_flags)
{
	/*
	 * refresh_flags contains REFRESH_QUIET, so the only errors
	 * are for unmerged entries.
	 */
	nếu (refresh_index(the_repository->index, refresh_flags | REFRESH_IN_PORCELAIN, NULL, NULL, NULL))
		die_resolve_conflict("commit");
}

static const char *prepare_index(const char **argv, const char *prefix,
				 const struct commit *current_head, số_nguyên is_status)
{
	struct string_list partial = STRING_LIST_INIT_DUP;
	struct pathspec pathspec;
	số_nguyên refresh_flags = REFRESH_QUIET;
	const char *ret;
	char *path = NULL;

	nếu (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	nếu (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	nếu (is_status)
		refresh_flags |= REFRESH_UNMERGED;
	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_FULL,
		       prefix, argv);

	nếu (pathspec_from_file) {
		nếu (interactive)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--interactive/--patch");

		nếu (all)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "-a");

		nếu (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_FULL,
				    prefix, pathspec_from_file, pathspec_file_nul);
	} không_thì nếu (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	nếu (!pathspec.nr && (also || (only && !allow_empty &&
	    (!amend || (fixup_message && strcmp(fixup_prefix, "amend"))))))
		die(_("No paths with --include/--only does not make sense."));

	nếu (repo_read_index_preload(the_repository, &pathspec, 0) < 0)
		die(_("index file corrupt"));

	nếu (interactive) {
		char *old_index_env = NULL, *old_repo_index_file;
		repo_hold_locked_index(the_repository, &index_lock,
				       LOCK_DIE_ON_ERROR);

		refresh_cache_or_die(refresh_flags);

		nếu (write_locked_index(the_repository->index, &index_lock, 0))
			die(_("unable to create temporary index"));

		old_repo_index_file = the_repository->index_file;
		the_repository->index_file =
			(char *)get_lock_file_path(&index_lock);
		old_index_env = xstrdup_or_null(getenv(INDEX_ENVIRONMENT));
		setenv(INDEX_ENVIRONMENT, the_repository->index_file, 1);

		nếu (interactive_add(the_repository, argv, prefix, patch_interactive, &interactive_opts) != 0)
			die(_("interactive add failed"));

		the_repository->index_file = old_repo_index_file;
		nếu (old_index_env && *old_index_env)
			setenv(INDEX_ENVIRONMENT, old_index_env, 1);
		không_thì
			unsetenv(INDEX_ENVIRONMENT);
		FREE_AND_NULL(old_index_env);

		discard_index(the_repository->index);
		read_index_from(the_repository->index, get_lock_file_path(&index_lock),
				repo_get_git_dir(the_repository));
		nếu (cache_tree_update(the_repository->index, WRITE_TREE_SILENT) == 0) {
			nếu (reopen_lock_file(&index_lock) < 0)
				die(_("unable to write index file"));
			nếu (write_locked_index(the_repository->index, &index_lock, 0))
				die(_("unable to update temporary index"));
		} không_thì
			warning(_("Failed to update main cache tree"));

		commit_style = COMMIT_NORMAL;
		ret = get_lock_file_path(&index_lock);
		goto out;
	} không_thì {
		nếu (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--interactive/--patch");
		nếu (interactive_opts.interhunkcontext != -1)
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
	nếu (all || (also && pathspec.nr)) {
		char *ps_matched = xcalloc(pathspec.nr, 1);
		repo_hold_locked_index(the_repository, &index_lock,
				       LOCK_DIE_ON_ERROR);
		add_files_to_cache(the_repository, also ? prefix : NULL,
				   &pathspec, ps_matched, 0, 0, 0 );
		nếu (!all && report_path_error(ps_matched, &pathspec))
			exit(128);

		refresh_cache_or_die(refresh_flags);
		cache_tree_update(the_repository->index, WRITE_TREE_SILENT);
		nếu (write_locked_index(the_repository->index, &index_lock, 0))
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
	nếu (!only && !pathspec.nr) {
		repo_hold_locked_index(the_repository, &index_lock,
				       LOCK_DIE_ON_ERROR);
		refresh_cache_or_die(refresh_flags);
		nếu (the_repository->index->cache_changed
		    || !cache_tree_fully_valid(the_repository->index))
			cache_tree_update(the_repository->index, WRITE_TREE_SILENT);
		nếu (write_locked_index(the_repository->index, &index_lock,
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

	nếu (whence != FROM_COMMIT) {
		nếu (whence == FROM_MERGE)
			die(_("cannot do a partial commit during a merge."));
		không_thì nếu (is_from_cherry_pick(whence))
			die(_("cannot do a partial commit during a cherry-pick."));
		không_thì nếu (is_from_rebase(whence))
			die(_("cannot do a partial commit during a rebase."));
	}

	nếu (list_paths(&partial, !current_head ? NULL : "HEAD", &pathspec))
		exit(1);

	discard_index(the_repository->index);
	nếu (repo_read_index(the_repository) < 0)
		die(_("cannot read the index"));

	repo_hold_locked_index(the_repository, &index_lock, LOCK_DIE_ON_ERROR);
	add_remove_files(&partial);
	refresh_index(the_repository->index, REFRESH_QUIET, NULL, NULL, NULL);
	cache_tree_update(the_repository->index, WRITE_TREE_SILENT);
	nếu (write_locked_index(the_repository->index, &index_lock, 0))
		die(_("unable to write new index file"));

	path = repo_git_path(the_repository, "next-index-%"PRIuMAX,
			     (uintmax_t) getpid());
	hold_lock_file_for_update(&false_lock, path,
				  LOCK_DIE_ON_ERROR);

	create_base_index(current_head);
	add_remove_files(&partial);
	refresh_index(the_repository->index, REFRESH_QUIET, NULL, NULL, NULL);

	nếu (write_locked_index(the_repository->index, &false_lock, 0))
		die(_("unable to write temporary index file"));

	discard_index(the_repository->index);
	ret = get_lock_file_path(&false_lock);
	read_index_from(the_repository->index, ret, repo_get_git_dir(the_repository));
out:
	string_list_clear(&partial, 0);
	clear_pathspec(&pathspec);
	free(path);
	trả_về ret;
}

static số_nguyên run_status(FILE *fp, const char *index_file, const char *prefix, số_nguyên nowarn,
		      struct wt_status *s)
{
	struct object_id oid;

	nếu (s->relative_paths)
		s->prefix = prefix;

	nếu (amend) {
		s->amend = 1;
		s->reference = "HEAD^1";
	}
	s->verbose = verbose;
	s->index_file = index_file;
	s->fp = fp;
	s->nowarn = nowarn;
	s->is_initial = repo_get_oid(the_repository, s->reference, &oid) ? 1 : 0;
	nếu (!s->is_initial)
		oidcpy(&s->oid_commit, &oid);
	s->status_format = status_format;
	s->ignore_submodule_arg = ignore_submodule_arg;

	wt_status_collect(s);
	wt_status_print(s);
	wt_status_collect_free_buffers(s);

	trả_về s->committable;
}

static số_nguyên is_a_merge(const struct commit *current_head)
{
	trả_về !!(current_head->parents && current_head->parents->next);
}

static void assert_split_ident(struct ident_split *id, const struct strbuf *buf)
{
	nếu (split_ident_line(id, buf->buf, buf->len) || !id->date_begin)
		BUG("unable to parse our own ident: %s", buf->buf);
}

static void export_one(const char *var, const char *s, const char *e, số_nguyên hack)
{
	struct strbuf buf = STRBUF_INIT;
	nếu (hack)
		strbuf_addch(&buf, hack);
	strbuf_add(&buf, s, e - s);
	setenv(var, buf.buf, 1);
	strbuf_release(&buf);
}

static số_nguyên parse_force_date(const char *in, struct strbuf *out)
{
	strbuf_addch(out, '@');

	nếu (parse_date(in, out) < 0) {
		số_nguyên errors = 0;
		unsigned long t = approxidate_careful(in, &errors);
		nếu (errors)
			trả_về -1;
		strbuf_addf(out, "%lu", t);
	}

	trả_về 0;
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

	nếu (author_message) {
		struct ident_split ident;
		size_t len;
		const char *a;

		a = find_commit_header(author_message_buffer, "author", &len);
		nếu (!a)
			die(_("commit '%s' lacks author header"), author_message);
		nếu (split_ident_line(&ident, a, len) < 0)
			die(_("commit '%s' has malformed author line"), author_message);

		set_ident_var(&name, xmemdupz(ident.name_begin, ident.name_end - ident.name_begin));
		set_ident_var(&email, xmemdupz(ident.mail_begin, ident.mail_end - ident.mail_begin));

		nếu (ident.date_begin) {
			struct strbuf date_buf = STRBUF_INIT;
			strbuf_addch(&date_buf, '@');
			strbuf_add(&date_buf, ident.date_begin, ident.date_end - ident.date_begin);
			strbuf_addch(&date_buf, ' ');
			strbuf_add(&date_buf, ident.tz_begin, ident.tz_end - ident.tz_begin);
			set_ident_var(&date, strbuf_detach(&date_buf, NULL));
		}
	}

	nếu (force_author) {
		struct ident_split ident;

		nếu (split_ident_line(&ident, force_author, strlen(force_author)) < 0)
			die(_("malformed --author parameter"));
		set_ident_var(&name, xmemdupz(ident.name_begin, ident.name_end - ident.name_begin));
		set_ident_var(&email, xmemdupz(ident.mail_begin, ident.mail_end - ident.mail_begin));
	}

	nếu (force_date) {
		struct strbuf date_buf = STRBUF_INIT;
		nếu (parse_force_date(force_date, &date_buf))
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

static số_nguyên author_date_is_interesting(void)
{
	trả_về author_message || force_date;
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

	nếu (!memchr(sb->buf, candidates[0], sb->len)) {
		free(comment_line_str_to_free);
		comment_line_str = comment_line_str_to_free =
			xstrfmt("%c", candidates[0]);
		trả_về;
	}

	p = sb->buf;
	candidate = strchr(candidates, *p);
	nếu (candidate)
		*candidate = ' ';
	cho (p = sb->buf; p + 1 < sb->buf + cutoff; p++) {
		nếu ((p[0] == '\n' || p[0] == '\r') && p[1]) {
			candidate = strchr(candidates, p[1]);
			nếu (candidate)
				*candidate = ' ';
		}
	}

	cho (p = candidates; *p == ' '; p++)
		;
	nếu (!*p)
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

static số_nguyên prepare_to_commit(const char *index_file, const char *prefix,
			     struct commit *current_head,
			     struct wt_status *s,
			     struct strbuf *author_ident)
{
	struct stat statbuf;
	struct strbuf committer_ident = STRBUF_INIT;
	số_nguyên committable;
	struct strbuf sb = STRBUF_INIT;
	const char *hook_arg1 = NULL;
	const char *hook_arg2 = NULL;
	số_nguyên clean_message_contents = (cleanup_mode != COMMIT_MSG_CLEANUP_NONE);
	số_nguyên old_display_comment_prefix;
	số_nguyên invoked_hook;

	/* This checks and barfs if author is badly specified */
	determine_author_info(author_ident);

	nếu (!no_verify && run_commit_hook(use_editor, index_file, &invoked_hook,
					  "pre-commit", NULL))
		trả_về 0;

	nếu (squash_message) {
		/*
		 * Insert the proper subject line before other commit
		 * message options add their content.
		 */
		nếu (use_message && !strcmp(use_message, squash_message))
			strbuf_addstr(&sb, "squash! ");
		không_thì {
			struct pretty_print_context ctx = {0};
			struct commit *c;
			c = lookup_commit_reference_by_name(squash_message);
			nếu (!c)
				die(_("could not lookup commit '%s'"), squash_message);
			ctx.output_encoding = get_commit_output_encoding();
			repo_format_commit_message(the_repository, c,
						   "squash! %s\n\n", &sb,
						   &ctx);
		}
	}

	nếu (have_option_m && !fixup_message) {
		strbuf_addbuf(&sb, &message);
		hook_arg1 = "message";
	} không_thì nếu (logfile && !strcmp(logfile, "-")) {
		nếu (isatty(0))
			fprintf(stderr, _("(reading log message from standard input)\n"));
		nếu (strbuf_read(&sb, 0, 0) < 0)
			die_errno(_("could not read log from standard input"));
		hook_arg1 = "message";
	} không_thì nếu (logfile) {
		nếu (strbuf_read_file(&sb, logfile, 0) < 0)
			die_errno(_("could not read log file '%s'"),
				  logfile);
		hook_arg1 = "message";
	} không_thì nếu (use_message) {
		const char *buffer;
		buffer = strstr(use_message_buffer, "\n\n");
		nếu (buffer)
			strbuf_addstr(&sb, skip_blank_lines(buffer + 2));
		hook_arg1 = "commit";
		hook_arg2 = use_message;
	} không_thì nếu (fixup_message) {
		struct pretty_print_context ctx = {0};
		struct commit *commit;
		char *fmt;
		commit = lookup_commit_reference_by_name(fixup_commit);
		nếu (!commit)
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
		nếu (have_option_m && !strcmp(fixup_prefix, "fixup"))
			strbuf_addbuf(&sb, &message);

		nếu (!strcmp(fixup_prefix, "amend")) {
			nếu (have_option_m)
				die(_("options '%s' and '%s:%s' cannot be used together"), "-m", "--fixup", fixup_message);
			prepare_amend_commit(commit, &sb, &ctx);
		}
	} không_thì nếu (!stat(git_path_merge_msg(the_repository), &statbuf)) {
		size_t merge_msg_start;

		/*
		 * prepend SQUASH_MSG here if it exists and a
		 * "merge --squash" was originally performed
		 */
		nếu (!stat(git_path_squash_msg(the_repository), &statbuf)) {
			nếu (strbuf_read_file(&sb, git_path_squash_msg(the_repository), 0) < 0)
				die_errno(_("could not read SQUASH_MSG"));
			hook_arg1 = "squash";
		} không_thì
			hook_arg1 = "merge";

		merge_msg_start = sb.len;
		nếu (strbuf_read_file(&sb, git_path_merge_msg(the_repository), 0) < 0)
			die_errno(_("could not read MERGE_MSG"));

		nếu (cleanup_mode == COMMIT_MSG_CLEANUP_SCISSORS &&
		    wt_status_locate_end(sb.buf + merge_msg_start,
					 sb.len - merge_msg_start) <
				sb.len - merge_msg_start)
			s->added_cut_line = 1;
	} không_thì nếu (!stat(git_path_squash_msg(the_repository), &statbuf)) {
		nếu (strbuf_read_file(&sb, git_path_squash_msg(the_repository), 0) < 0)
			die_errno(_("could not read SQUASH_MSG"));
		hook_arg1 = "squash";
	} không_thì nếu (template_file) {
		nếu (strbuf_read_file(&sb, template_file, 0) < 0)
			die_errno(_("could not read '%s'"), template_file);
		hook_arg1 = "template";
		clean_message_contents = 0;
	}

	/*
	 * The remaining cases don't modify the template message, but
	 * just set the argument(s) to the prepare-commit-msg hook.
	 */
	không_thì nếu (whence == FROM_MERGE)
		hook_arg1 = "merge";
	không_thì nếu (is_from_cherry_pick(whence) || whence == FROM_REBASE_PICK) {
		hook_arg1 = "commit";
		hook_arg2 = "CHERRY_PICK_HEAD";
	}

	nếu (squash_message) {
		/*
		 * If squash_commit was used for the commit subject,
		 * then we're possibly hijacking other commit log options.
		 * Reset the hook args to tell the real story.
		 */
		hook_arg1 = "message";
		hook_arg2 = "";
	}

	s->fp = fopen_for_writing(git_path_commit_editmsg());
	nếu (!s->fp)
		die_errno(_("could not open '%s'"), git_path_commit_editmsg());

	/* Ignore status.displayCommentPrefix: we do need comments in COMMIT_EDITMSG. */
	old_display_comment_prefix = s->display_comment_prefix;
	s->display_comment_prefix = 1;

	/*
	 * Most hints are counter-productive when the commit has
	 * already started.
	 */
	s->hints = 0;

	nếu (clean_message_contents)
		strbuf_stripspace(&sb, NULL);

	nếu (signoff)
		append_signoff(&sb, ignored_log_message_bytes(sb.buf, sb.len), 0);

	nếu (fwrite(sb.buf, 1, sb.len, s->fp) < sb.len)
		die_errno(_("could not write commit template"));

#ifndef WITH_BREAKING_CHANGES
	nếu (auto_comment_line_char)
		adjust_comment_line_char(&sb);
#endif /* !WITH_BREAKING_CHANGES */
	strbuf_release(&sb);

	/* This checks if committer ident is explicitly given */
	strbuf_addstr(&committer_ident, git_committer_info(IDENT_STRICT));
	nếu (use_editor && include_status) {
		số_nguyên ident_shown = 0;
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
		nếu (whence != FROM_COMMIT) {
			nếu (cleanup_mode == COMMIT_MSG_CLEANUP_SCISSORS)
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
		nếu (cleanup_mode == COMMIT_MSG_CLEANUP_ALL)
			status_printf(s, GIT_COLOR_NORMAL, hint_cleanup_all, comment_line_str);
		không_thì nếu (cleanup_mode == COMMIT_MSG_CLEANUP_SCISSORS) {
			nếu (whence == FROM_COMMIT)
				wt_status_add_cut_line(s);
		} không_thì /* COMMIT_MSG_CLEANUP_SPACE, that is. */
			status_printf(s, GIT_COLOR_NORMAL, hint_cleanup_space, comment_line_str);

		/*
		 * These should never fail because they come from our own
		 * fmt_ident. They may fail the sane_ident test, but we know
		 * that the name and mail pointers will at least be valid,
		 * which is enough for our tests and printing here.
		 */
		assert_split_ident(&ai, author_ident);
		assert_split_ident(&ci, &committer_ident);

		nếu (ident_cmp(&ai, &ci))
			status_printf_ln(s, GIT_COLOR_NORMAL,
				_("%s"
				"Author:    %.*s <%.*s>"),
				ident_shown++ ? "" : "\n",
				(số_nguyên)(ai.name_end - ai.name_begin), ai.name_begin,
				(số_nguyên)(ai.mail_end - ai.mail_begin), ai.mail_begin);

		nếu (author_date_is_interesting())
			status_printf_ln(s, GIT_COLOR_NORMAL,
				_("%s"
				"Date:      %s"),
				ident_shown++ ? "" : "\n",
				show_ident_date(&ai, DATE_MODE(NORMAL)));

		nếu (!committer_ident_sufficiently_given())
			status_printf_ln(s, GIT_COLOR_NORMAL,
				_("%s"
				"Committer: %.*s <%.*s>"),
				ident_shown++ ? "" : "\n",
				(số_nguyên)(ci.name_end - ci.name_begin), ci.name_begin,
				(số_nguyên)(ci.mail_end - ci.mail_begin), ci.mail_begin);

		status_printf_ln(s, GIT_COLOR_NORMAL, "%s", ""); /* Add new line for clarity */

		saved_color_setting = s->use_color;
		s->use_color = GIT_COLOR_NEVER;
		committable = run_status(s->fp, index_file, prefix, 1, s);
		s->use_color = saved_color_setting;
		string_list_clear_func(&s->change, change_data_free);
	} không_thì {
		struct object_id oid;
		const char *parent = "HEAD";

		nếu (!the_repository->index->initialized && repo_read_index(the_repository) < 0)
			die(_("Cannot read index"));

		nếu (amend)
			parent = "HEAD^1";

		nếu (repo_get_oid(the_repository, parent, &oid)) {
			số_nguyên i, ita_nr = 0;

			/* TODO: audit for interaction with sparse-index. */
			ensure_full_index(the_repository->index);
			cho (i = 0; i < the_repository->index->cache_nr; i++)
				nếu (ce_intent_to_add(the_repository->index->cache[i]))
					ita_nr++;
			committable = the_repository->index->cache_nr > ita_nr;
		} không_thì {
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
			nếu (ignore_submodule_arg &&
			    !strcmp(ignore_submodule_arg, "all"))
				flags.ignore_submodules = 1;
			committable = index_differs_from(the_repository,
							 parent, &flags, 1);
		}
	}
	strbuf_release(&committer_ident);

	fclose(s->fp);

	nếu (trailer_args.nr) {
		nếu (amend_file_with_trailers(git_path_commit_editmsg(), &trailer_args))
			die(_("unable to pass trailers to --trailers"));
		strvec_clear(&trailer_args);
	}

	/*
	 * Reject an attempt to record a non-merge empty commit without
	 * explicit --allow-empty. In the cherry-pick case, it may be
	 * empty due to conflict resolution, which the user should okay.
	 */
	nếu (!committable && whence != FROM_MERGE && !allow_empty &&
	    !(amend && is_a_merge(current_head))) {
		s->hints = advice_enabled(ADVICE_STATUS_HINTS);
		s->display_comment_prefix = old_display_comment_prefix;
		run_status(stdout, index_file, prefix, 0, s);
		nếu (amend)
			fputs(_(empty_amend_advice), stderr);
		không_thì nếu (is_from_cherry_pick(whence) ||
			 whence == FROM_REBASE_PICK) {
			fputs(_(empty_cherry_pick_advice), stderr);
			nếu (whence == FROM_CHERRY_PICK_SINGLE)
				fputs(_(empty_cherry_pick_advice_single), stderr);
			không_thì nếu (whence == FROM_CHERRY_PICK_MULTI)
				fputs(_(empty_cherry_pick_advice_multi), stderr);
			không_thì
				fputs(_(empty_rebase_pick_advice), stderr);
		}
		trả_về 0;
	}

	nếu (!no_verify && invoked_hook) {
		/*
		 * Re-read the index as the pre-commit-commit hook was invoked
		 * and could have updated it. We must do this before we invoke
		 * the editor and after we invoke run_status above.
		 */
		discard_index(the_repository->index);
	}
	read_index_from(the_repository->index, index_file, repo_get_git_dir(the_repository));

	nếu (cache_tree_update(the_repository->index, 0)) {
		error(_("Error building trees"));
		trả_về 0;
	}

	nếu (run_commit_hook(use_editor, index_file, NULL, "prepare-commit-msg",
			    git_path_commit_editmsg(), hook_arg1, hook_arg2, NULL))
		trả_về 0;

	nếu (use_editor) {
		struct strvec env = STRVEC_INIT;

		strvec_pushf(&env, "GIT_INDEX_FILE=%s", index_file);
		nếu (launch_editor(git_path_commit_editmsg(), NULL, env.v)) {
			fprintf(stderr,
			_("Please supply the message using either -m or -F option.\n"));
			exit(1);
		}
		strvec_clear(&env);
	}

	nếu (!no_verify &&
	    run_commit_hook(use_editor, index_file, NULL, "commit-msg",
			    git_path_commit_editmsg(), NULL)) {
		trả_về 0;
	}

	trả_về 1;
}

static const char *find_author_by_nickname(const char *name)
{
	struct rev_info revs;
	struct commit *commit;
	struct strbuf buf = STRBUF_INIT;
	const char *av[20];
	số_nguyên ac = 0;

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

	nếu (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	commit = get_revision(&revs);
	nếu (commit) {
		struct pretty_print_context ctx = {0};
		ctx.date_mode.type = DATE_NORMAL;
		strbuf_release(&buf);
		repo_format_commit_message(the_repository, commit,
					   "%aN <%aE>", &buf, &ctx);
		release_revisions(&revs);
		trả_về strbuf_detach(&buf, NULL);
	}
	die(_("--author '%s' is not 'Name <email>' and matches no existing author"), name);
}

static void handle_ignored_arg(struct wt_status *s)
{
	nếu (!ignored_arg)
		; /* default already initialized */
	không_thì nếu (!strcmp(ignored_arg, "traditional"))
		s->show_ignored_mode = SHOW_TRADITIONAL_IGNORED;
	không_thì nếu (!strcmp(ignored_arg, "no"))
		s->show_ignored_mode = SHOW_NO_IGNORED;
	không_thì nếu (!strcmp(ignored_arg, "matching"))
		s->show_ignored_mode = SHOW_MATCHING_IGNORED;
	không_thì
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

	nếu (!strcmp(u, "no"))
		trả_về SHOW_NO_UNTRACKED_FILES;
	không_thì nếu (!strcmp(u, "normal"))
		trả_về SHOW_NORMAL_UNTRACKED_FILES;
	không_thì nếu (!strcmp(u, "all"))
		trả_về SHOW_ALL_UNTRACKED_FILES;
	không_thì
		trả_về SHOW_UNTRACKED_FILES_ERROR;
}

static void handle_untracked_files_arg(struct wt_status *s)
{
	enum untracked_status_type u;

	nếu (!untracked_files_arg)
		trả_về; /* default already initialized */

	u = parse_untracked_setting_name(untracked_files_arg);
	nếu (u == SHOW_UNTRACKED_FILES_ERROR)
		die(_("Invalid untracked files mode '%s'"),
		    untracked_files_arg);
	s->show_untracked_files = u;
}

static const char *read_commit_message(const char *name)
{
	const char *out_enc;
	struct commit *commit;

	commit = lookup_commit_reference_by_name(name);
	nếu (!commit)
		die(_("could not lookup commit '%s'"), name);
	out_enc = get_commit_output_encoding();
	trả_về repo_logmsg_reencode(the_repository, commit, NULL, out_enc);
}

/*
 * Enumerate what needs to be propagated when --porcelain
 * is not in effect here.
 */
static struct status_deferred_config {
	enum wt_status_format status_format;
	số_nguyên show_branch;
	enum ahead_behind_flags ahead_behind;
} status_deferred_config = {
	STATUS_FORMAT_UNSPECIFIED,
	-1, /* unspecified */
	AHEAD_BEHIND_UNSPECIFIED,
};

static void finalize_deferred_config(struct wt_status *s)
{
	số_nguyên use_deferred_config = (status_format != STATUS_FORMAT_PORCELAIN &&
				   status_format != STATUS_FORMAT_PORCELAIN_V2 &&
				   !s->null_termination);

	nếu (s->null_termination) {
		nếu (status_format == STATUS_FORMAT_NONE ||
		    status_format == STATUS_FORMAT_UNSPECIFIED)
			status_format = STATUS_FORMAT_PORCELAIN;
		không_thì nếu (status_format == STATUS_FORMAT_LONG)
			die(_("options '%s' and '%s' cannot be used together"), "--long", "-z");
	}

	nếu (use_deferred_config && status_format == STATUS_FORMAT_UNSPECIFIED)
		status_format = status_deferred_config.status_format;
	nếu (status_format == STATUS_FORMAT_UNSPECIFIED)
		status_format = STATUS_FORMAT_NONE;

	nếu (use_deferred_config && s->show_branch < 0)
		s->show_branch = status_deferred_config.show_branch;
	nếu (s->show_branch < 0)
		s->show_branch = 0;

	/*
	 * If the user did not give a "--[no]-ahead-behind" command
	 * line argument *AND* we will print in a human-readable format
	 * (short, long etc.) then we inherit from the status.aheadbehind
	 * config setting.  In all other cases (and porcelain V[12] formats
	 * in particular), we inherit _FULL for backwards compatibility.
	 */
	nếu (use_deferred_config &&
	    s->ahead_behind_flags == AHEAD_BEHIND_UNSPECIFIED)
		s->ahead_behind_flags = status_deferred_config.ahead_behind;

	nếu (s->ahead_behind_flags == AHEAD_BEHIND_UNSPECIFIED)
		s->ahead_behind_flags = AHEAD_BEHIND_FULL;
}

static void check_fixup_reword_options(số_nguyên argc, const char *argv[]) {
	nếu (whence != FROM_COMMIT) {
		nếu (whence == FROM_MERGE)
			die(_("You are in the middle of a merge -- cannot reword."));
		không_thì nếu (is_from_cherry_pick(whence))
			die(_("You are in the middle of a cherry-pick -- cannot reword."));
	}
	nếu (argc)
		die(_("reword option of '%s' and path '%s' cannot be used together"), "--fixup", *argv);
	nếu (patch_interactive || interactive || all || also || only)
		die(_("reword option of '%s' and '%s' cannot be used together"),
			"--fixup", "--patch/--interactive/--all/--include/--only");
}

static số_nguyên parse_and_validate_options(số_nguyên argc, const char *argv[],
				      const struct option *options,
				      const char * const usage[],
				      const char *prefix,
				      struct commit *current_head,
				      struct wt_status *s)
{
	argc = parse_options(argc, argv, prefix, options, usage, 0);
	finalize_deferred_config(s);

	nếu (force_author && !strchr(force_author, '>'))
		force_author = find_author_by_nickname(force_author);

	nếu (force_author && renew_authorship)
		die(_("options '%s' and '%s' cannot be used together"), "--reset-author", "--author");

	nếu (logfile || have_option_m || use_message)
		use_editor = 0;

	/* Sanity check options */
	nếu (amend && !current_head)
		die(_("You have nothing to amend."));
	nếu (amend && whence != FROM_COMMIT) {
		nếu (whence == FROM_MERGE)
			die(_("You are in the middle of a merge -- cannot amend."));
		không_thì nếu (is_from_cherry_pick(whence))
			die(_("You are in the middle of a cherry-pick -- cannot amend."));
		không_thì nếu (whence == FROM_REBASE_PICK)
			die(_("You are in the middle of a rebase -- cannot amend."));
	}
	nếu (fixup_message && squash_message)
		die(_("options '%s' and '%s' cannot be used together"), "--squash", "--fixup");
	die_for_incompatible_opt4(!!use_message, "-C",
				  !!edit_message, "-c",
				  !!logfile, "-F",
				  !!fixup_message, "--fixup");
	die_for_incompatible_opt4(have_option_m, "-m",
				  !!edit_message, "-c",
				  !!use_message, "-C",
				  !!logfile, "-F");
	nếu (use_message || edit_message || logfile ||fixup_message || have_option_m)
		FREE_AND_NULL(template_file);
	nếu (edit_message)
		use_message = edit_message;
	nếu (amend && !use_message && !fixup_message)
		use_message = "HEAD";
	nếu (!use_message && !is_from_cherry_pick(whence) &&
	    !is_from_rebase(whence) && renew_authorship)
		die(_("--reset-author can be used only with -C, -c or --amend."));
	nếu (use_message) {
		use_message_buffer = read_commit_message(use_message);
		nếu (!renew_authorship) {
			author_message = use_message;
			author_message_buffer = use_message_buffer;
		}
	}
	nếu ((is_from_cherry_pick(whence) || whence == FROM_REBASE_PICK) &&
	    !renew_authorship) {
		author_message = "CHERRY_PICK_HEAD";
		author_message_buffer = read_commit_message(author_message);
	}

	nếu (patch_interactive)
		interactive = 1;

	die_for_incompatible_opt4(also, "-i/--include",
				  only, "-o/--only",
				  all, "-a/--all",
				  interactive, "--interactive/-p/--patch");
	nếu (fixup_message) {
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
		trong_khi (isalpha(*p))
			p++;
		nếu (p > fixup_message && *p == ':') {
			*p = '\0';
			fixup_commit = p + 1;
			nếu (!strcmp("amend", fixup_message) ||
			    !strcmp("reword", fixup_message)) {
				fixup_prefix = "amend";
				allow_empty = 1;
				nếu (*fixup_message == 'r') {
					check_fixup_reword_options(argc, argv);
					only = 1;
				}
			} không_thì {
				die(_("unknown option: --fixup=%s:%s"), fixup_message, fixup_commit);
			}
		} không_thì {
			fixup_commit = fixup_message;
			fixup_prefix = "fixup";
			use_editor = 0;
		}
	}

	nếu (0 <= edit_flag)
		use_editor = edit_flag;

	handle_untracked_files_arg(s);

	nếu (all && argc > 0)
		die(_("paths '%s ...' with -a does not make sense"),
		    argv[0]);

	nếu (status_format != STATUS_FORMAT_NONE)
		dry_run = 1;

	trả_về argc;
}

static số_nguyên dry_run_commit(const char **argv, const char *prefix,
			  const struct commit *current_head, struct wt_status *s)
{
	số_nguyên committable;
	const char *index_file;

	index_file = prepare_index(argv, prefix, current_head, 1);
	committable = run_status(stdout, index_file, prefix, 0, s);
	rollback_index_files();

	trả_về committable ? 0 : 1;
}

define_list_config_array_extra(color_status_slots, {"added"});

static số_nguyên parse_status_slot(const char *slot)
{
	nếu (!strcasecmp(slot, "added"))
		trả_về WT_STATUS_UPDATED;

	trả_về LOOKUP_CONFIG(color_status_slots, slot);
}

static số_nguyên git_status_config(const char *k, const char *v,
			     const struct config_context *ctx, void *cb)
{
	struct wt_status *s = cb;
	const char *slot_name;

	nếu (starts_with(k, "column."))
		trả_về git_column_config(k, v, "status", &s->colopts);
	nếu (!strcmp(k, "status.submodulesummary")) {
		số_nguyên is_bool;
		s->submodule_summary = git_config_bool_or_int(k, v, ctx->kvi,
							      &is_bool);
		nếu (is_bool && s->submodule_summary)
			s->submodule_summary = -1;
		trả_về 0;
	}
	nếu (!strcmp(k, "status.short")) {
		nếu (git_config_bool(k, v))
			status_deferred_config.status_format = STATUS_FORMAT_SHORT;
		không_thì
			status_deferred_config.status_format = STATUS_FORMAT_NONE;
		trả_về 0;
	}
	nếu (!strcmp(k, "status.branch")) {
		status_deferred_config.show_branch = git_config_bool(k, v);
		trả_về 0;
	}
	nếu (!strcmp(k, "status.aheadbehind")) {
		status_deferred_config.ahead_behind = git_config_bool(k, v);
		trả_về 0;
	}
	nếu (!strcmp(k, "status.showstash")) {
		s->show_stash = git_config_bool(k, v);
		trả_về 0;
	}
	nếu (!strcmp(k, "status.color") || !strcmp(k, "color.status")) {
		s->use_color = git_config_colorbool(k, v);
		trả_về 0;
	}
	nếu (!strcmp(k, "status.displaycommentprefix")) {
		s->display_comment_prefix = git_config_bool(k, v);
		trả_về 0;
	}
	nếu (skip_prefix(k, "status.color.", &slot_name) ||
	    skip_prefix(k, "color.status.", &slot_name)) {
		số_nguyên slot = parse_status_slot(slot_name);
		nếu (slot < 0)
			trả_về 0;
		nếu (!v)
			trả_về config_error_nonbool(k);
		trả_về color_parse(v, s->color_palette[slot]);
	}
	nếu (!strcmp(k, "status.relativepaths")) {
		s->relative_paths = git_config_bool(k, v);
		trả_về 0;
	}
	nếu (!strcmp(k, "status.showuntrackedfiles")) {
		enum untracked_status_type u;

		u = parse_untracked_setting_name(v);
		nếu (u == SHOW_UNTRACKED_FILES_ERROR)
			trả_về error(_("Invalid untracked files mode '%s'"), v);
		s->show_untracked_files = u;
		trả_về 0;
	}
	nếu (!strcmp(k, "diff.renamelimit")) {
		nếu (s->rename_limit == -1)
			s->rename_limit = git_config_int(k, v, ctx->kvi);
		trả_về 0;
	}
	nếu (!strcmp(k, "status.renamelimit")) {
		s->rename_limit = git_config_int(k, v, ctx->kvi);
		trả_về 0;
	}
	nếu (!strcmp(k, "diff.renames")) {
		nếu (s->detect_rename == -1)
			s->detect_rename = git_config_rename(k, v);
		trả_về 0;
	}
	nếu (!strcmp(k, "status.renames")) {
		s->detect_rename = git_config_rename(k, v);
		trả_về 0;
	}
	trả_về git_diff_ui_config(k, v, ctx, NULL);
}

số_nguyên cmd_status(số_nguyên argc,
const char **argv,
const char *prefix,
struct repository *repo UNUSED)
{
	static số_nguyên no_renames = -1;
	static const char *rename_score_arg = (const char *)-1;
	static struct wt_status s;
	unsigned số_nguyên progress_flag = 0;
	số_nguyên fd;
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

	nếu (s.show_ignored_mode == SHOW_MATCHING_IGNORED &&
	    s.show_untracked_files == SHOW_NO_UNTRACKED_FILES)
		die(_("Unsupported combination of ignored and untracked-files arguments"));

	parse_pathspec(&s.pathspec, 0,
		       PATHSPEC_PREFER_FULL,
		       prefix, argv);

	nếu (status_format != STATUS_FORMAT_PORCELAIN &&
	    status_format != STATUS_FORMAT_PORCELAIN_V2)
		progress_flag = REFRESH_PROGRESS;
	repo_read_index(the_repository);
	refresh_index(the_repository->index,
		      REFRESH_QUIET|REFRESH_UNMERGED|progress_flag,
		      &s.pathspec, NULL, NULL);

	nếu (use_optional_locks())
		fd = repo_hold_locked_index(the_repository, &index_lock, 0);
	không_thì
		fd = -1;

	s.is_initial = repo_get_oid(the_repository, s.reference, &oid) ? 1 : 0;
	nếu (!s.is_initial)
		oidcpy(&s.oid_commit, &oid);

	s.ignore_submodule_arg = ignore_submodule_arg;
	s.status_format = status_format;
	s.verbose = verbose;
	nếu (no_renames != -1)
		s.detect_rename = !no_renames;
	nếu ((intptr_t)rename_score_arg != -1) {
		nếu (s.detect_rename < DIFF_DETECT_RENAME)
			s.detect_rename = DIFF_DETECT_RENAME;
		nếu (rename_score_arg)
			s.rename_score = parse_rename_score(&rename_score_arg);
	}

	wt_status_collect(&s);

	nếu (0 <= fd)
		repo_update_index_if_able(the_repository, &index_lock);

	nếu (s.relative_paths)
		s.prefix = prefix;

	wt_status_print(&s);
	wt_status_collect_free_buffers(&s);

	trả_về 0;
}

static số_nguyên git_commit_config(const char *k, const char *v,
			     const struct config_context *ctx, void *cb)
{
	struct wt_status *s = cb;

	nếu (!strcmp(k, "commit.template"))
		trả_về git_config_pathname(&template_file, k, v);
	nếu (!strcmp(k, "commit.status")) {
		include_status = git_config_bool(k, v);
		trả_về 0;
	}
	nếu (!strcmp(k, "commit.cleanup")) {
		FREE_AND_NULL(cleanup_config);
		trả_về git_config_string(&cleanup_config, k, v);
	}
	nếu (!strcmp(k, "commit.gpgsign")) {
		sign_commit = git_config_bool(k, v) ? "" : NULL;
		trả_về 0;
	}
	nếu (!strcmp(k, "commit.verbose")) {
		số_nguyên is_bool;
		config_commit_verbose = git_config_bool_or_int(k, v, ctx->kvi,
							       &is_bool);
		trả_về 0;
	}

	trả_về git_status_config(k, v, ctx, s);
}

số_nguyên cmd_commit(số_nguyên argc,
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
	số_nguyên ret = 0;

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

	nếu (repo_get_oid(the_repository, "HEAD", &oid))
		current_head = NULL;
	không_thì {
		current_head = lookup_commit_or_die(&oid, "HEAD");
		nếu (repo_parse_commit(the_repository, current_head))
			die(_("could not parse HEAD commit"));
	}
	verbose = -1; /* unspecified */
	argc = parse_and_validate_options(argc, argv, builtin_commit_options,
					  builtin_commit_usage,
					  prefix, current_head, &s);
	nếu (trailer_args.nr)
		trailer_config_init();

	nếu (verbose == -1)
		verbose = (config_commit_verbose < 0) ? 0 : config_commit_verbose;

	nếu (cleanup_arg) {
		free(cleanup_config);
		cleanup_config = xstrdup(cleanup_arg);
	}
	cleanup_mode = get_cleanup_mode(cleanup_config, use_editor);

	nếu (dry_run)
		trả_về dry_run_commit(argv, prefix, current_head, &s);
	index_file = prepare_index(argv, prefix, current_head, 0);

	/* Set up everything for writing the commit object.  This includes
	   running hooks, writing the trees, and interacting with the user.  */
	nếu (!prepare_to_commit(index_file, prefix,
			       current_head, &s, &author_ident)) {
		ret = 1;
		rollback_index_files();
		goto cleanup;
	}

	/* Determine parents */
	reflog_msg = getenv("GIT_REFLOG_ACTION");
	nếu (!current_head) {
		nếu (!reflog_msg)
			reflog_msg = "commit (initial)";
	} không_thì nếu (amend) {
		nếu (!reflog_msg)
			reflog_msg = "commit (amend)";
		parents = commit_list_copy(current_head->parents);
	} không_thì nếu (whence == FROM_MERGE) {
		struct strbuf m = STRBUF_INIT;
		FILE *fp;
		số_nguyên allow_fast_forward = 1;
		struct commit_list **pptr = &parents;

		nếu (!reflog_msg)
			reflog_msg = "commit (merge)";
		pptr = commit_list_append(current_head, pptr);
		fp = xfopen(git_path_merge_head(the_repository), "r");
		trong_khi (strbuf_getline_lf(&m, fp) != EOF) {
			struct commit *parent;

			parent = get_merge_parent(m.buf);
			nếu (!parent)
				die(_("Corrupt MERGE_HEAD file (%s)"), m.buf);
			pptr = commit_list_append(parent, pptr);
		}
		fclose(fp);
		strbuf_release(&m);
		nếu (!stat(git_path_merge_mode(the_repository), &statbuf)) {
			nếu (strbuf_read_file(&sb, git_path_merge_mode(the_repository), 0) < 0)
				die_errno(_("could not read MERGE_MODE"));
			nếu (!strcmp(sb.buf, "no-ff"))
				allow_fast_forward = 0;
		}
		nếu (allow_fast_forward)
			reduce_heads_replace(&parents);
	} không_thì {
		nếu (!reflog_msg)
			reflog_msg = is_from_cherry_pick(whence)
					? "commit (cherry-pick)"
					: is_from_rebase(whence)
					? "commit (rebase)"
					: "commit";
		commit_list_insert(current_head, &parents);
	}

	/* Finally, get the commit message */
	strbuf_reset(&sb);
	nếu (strbuf_read_file(&sb, git_path_commit_editmsg(), 0) < 0) {
		số_nguyên saved_errno = errno;
		rollback_index_files();
		die(_("could not read commit message: %s"), strerror(saved_errno));
	}

	cleanup_message(&sb, cleanup_mode, verbose);

	nếu (message_is_empty(&sb, cleanup_mode) && !allow_empty_message) {
		rollback_index_files();
		fprintf(stderr, _("Aborting commit due to empty commit message.\n"));
		exit(1);
	}
	nếu (template_untouched(&sb, template_file, cleanup_mode) && !allow_empty_message) {
		rollback_index_files();
		fprintf(stderr, _("Aborting commit; you did not edit the message.\n"));
		exit(1);
	}

	nếu (fixup_message && starts_with(sb.buf, "amend! ") &&
	    !allow_empty_message) {
		struct strbuf body = STRBUF_INIT;
		size_t len = commit_subject_length(sb.buf);
		strbuf_addstr(&body, sb.buf + len);
		nếu (message_is_empty(&body, cleanup_mode)) {
			rollback_index_files();
			fprintf(stderr, _("Aborting commit due to empty commit message body.\n"));
			exit(1);
		}
		strbuf_release(&body);
	}

	nếu (amend) {
		const char *exclude_gpgsig[3] = { "gpgsig", "gpgsig-sha256", NULL };
		extra = read_commit_extra_headers(current_head, exclude_gpgsig);
	} không_thì {
		struct commit_extra_header **tail = &extra;
		append_merge_tag_headers(parents, &tail);
	}

	nếu (commit_tree_extended(sb.buf, sb.len, &the_repository->index->cache_tree->oid,
				 parents, &oid, author_ident.buf, NULL,
				 sign_commit, extra)) {
		rollback_index_files();
		die(_("failed to write commit object"));
	}

	nếu (update_head_with_reflog(current_head, &oid, reflog_msg, &sb,
				    &err)) {
		rollback_index_files();
		die("%s", err.buf);
	}

	sequencer_post_commit_cleanup(the_repository, 0);
	unlink(git_path_merge_head(the_repository));
	unlink(git_path_merge_msg(the_repository));
	unlink(git_path_merge_mode(the_repository));
	unlink(git_path_squash_msg(the_repository));

	nếu (commit_index_files())
		die(_("repository has been updated, but unable to write\n"
		      "new index file. Check that disk is not full and quota is\n"
		      "not exceeded, and then \"git restore --staged :/\" to recover."));

	git_test_write_commit_graph_or_die(the_repository);

	repo_rerere(the_repository, 0);
	run_auto_maintenance(the_repository, quiet);
	run_commit_hook(use_editor, repo_get_index_file(the_repository),
			NULL, "post-commit", NULL);
	nếu (amend && !no_post_rewrite) {
		commit_post_rewrite(the_repository, current_head, &oid);
	}
	nếu (!quiet) {
		unsigned số_nguyên flags = 0;

		nếu (!current_head)
			flags |= SUMMARY_INITIAL_COMMIT;
		nếu (author_date_is_interesting())
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
	trả_về ret;
}
