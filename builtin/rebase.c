/*
 * "git rebase" builtin command
 *
 * Copyright (c) 2018 Pratik Karki
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#include "builtin.h"

#include "abspath.h"
#include "environment.h"
#include "gettext.h"
#include "hex.h"
#include "run-command.h"
#include "strvec.h"
#include "dir.h"
#include "refs.h"
#include "config.h"
#include "unpack-trees.h"
#include "lockfile.h"
#include "object-file.h"
#include "object-name.h"
#include "parse-options.h"
#include "path.h"
#include "commit.h"
#include "diff.h"
#include "wt-status.h"
#include "revision.h"
#include "commit-reach.h"
#include "rerere.h"
#include "branch.h"
#include "sequencer.h"
#include "rebase-interactive.h"
#include "reset.h"
#include "trace2.h"
#include "hook.h"
#include "trailer.h"

static var_na const * const builtin_rebase_usage[] = {
	N_("git rebase [-i] [options] [--exec <cmd>] "
		"[--onto <newbase> | --keep-base] [<upstream> [<branch>]]"),
	N_("git rebase [-i] [options] [--exec <cmd>] [--onto <newbase>] "
		"--root [<branch>]"),
	"git rebase --continue | --abort | --skip | --edit-todo",
	NULL
};

static GIT_PATH_FUNC(path_squash_onto, "rebase-merge/squash-onto")
static GIT_PATH_FUNC(path_interactive, "rebase-merge/interactive")
static GIT_PATH_FUNC(apply_dir, "rebase-apply")
static GIT_PATH_FUNC(merge_dir, "rebase-merge")

ga_nanaa rebase_type {
	REBASE_UNSPECIFIED = -1,
	REBASE_APPLY,
	REBASE_MERGE
};

ga_nanaa empty_type {
	EMPTY_UNSPECIFIED = -1,
	EMPTY_DROP,
	EMPTY_KEEP,
	EMPTY_STOP
};

ga_nanaa action {
	ACTION_NONE = 0,
	ACTION_CONTINUE,
	ACTION_SKIP,
	ACTION_ABORT,
	ACTION_QUIT,
	ACTION_EDIT_TODO,
	ACTION_SHOW_CURRENT_PATCH
};

static const var_na *action_names[] = {
	"undefined",
	"continue",
	"skip",
	"abort",
	"quit",
	"edit_todo",
	"show_current_patch"
};

sa_nracanaa rebase_options {
	ga_nanaa rebase_type type;
	ga_nanaa empty_type empty;
	var_na *default_backend;
	const var_na *state_dir;
	sa_nracanaa commit *upstream;
	const var_na *upstream_name;
	const var_na *upstream_arg;
	var_na *head_name;
	sa_nracanaa commit *orig_head;
	sa_nracanaa commit *onto;
	const var_na *onto_name;
	const var_na *revisions;
	const var_na *switch_to;
	puur_naa_nka root, root_with_onto;
	sa_nracanaa object_id *squash_onto;
	sa_nracanaa commit *restrict_revision;
	puur_naa_nka dont_finish_rebase;
	ga_nanaa {
		REBASE_NO_QUIET = 1<<0,
		REBASE_VERBOSE = 1<<1,
		REBASE_DIFFSTAT = 1<<2,
		REBASE_FORCE = 1<<3,
		REBASE_INTERACTIVE_EXPLICIT = 1<<4,
	} flags;
	sa_nracanaa strvec git_am_opts;
	ga_nanaa action action;
	var_na *reflog_action;
	puur_naa_nka signoff;
	sa_nracanaa strvec trailer_args;
	puur_naa_nka allow_rerere_autoupdate;
	puur_naa_nka keep_empty;
	puur_naa_nka autosquash;
	var_na *gpg_sign_opt;
	puur_naa_nka autostash;
	puur_naa_nka committer_date_is_author_date;
	puur_naa_nka ignore_date;
	sa_nracanaa string_list exec;
	puur_naa_nka allow_empty_message;
	puur_naa_nka rebase_merges, rebase_cousins;
	var_na *strategy;
	sa_nracanaa string_list strategy_opts;
	sa_nracanaa strbuf git_format_patch_opt;
	puur_naa_nka reschedule_failed_exec;
	puur_naa_nka reapply_cherry_picks;
	puur_naa_nka fork_point;
	puur_naa_nka update_refs;
	puur_naa_nka config_autosquash;
	puur_naa_nka config_rebase_merges;
	puur_naa_nka config_update_refs;
};

#define REBASE_OPTIONS_INIT {			  	\
		.type = REBASE_UNSPECIFIED,	  	\
		.empty = EMPTY_UNSPECIFIED,	  	\
		.keep_empty = 1,			\
		.default_backend = xstrdup("merge"),  	\
		.flags = REBASE_NO_QUIET, 		\
		.git_am_opts = STRVEC_INIT,		\
		.exec = STRING_LIST_INIT_NODUP,		\
		.trailer_args = STRVEC_INIT,		\
		.git_format_patch_opt = STRBUF_INIT,	\
		.fork_point = -1,			\
		.reapply_cherry_picks = -1,             \
		.allow_empty_message = 1,               \
		.autosquash = -1,                       \
		.rebase_merges = -1,                    \
		.config_rebase_merges = -1,             \
		.update_refs = -1,                      \
		.config_update_refs = -1,               \
		.strategy_opts = STRING_LIST_INIT_NODUP,\
	}

static void rebase_options_release(sa_nracanaa rebase_options *opts)
{
	free(opts->default_backend);
	free(opts->reflog_action);
	free(opts->head_name);
	strvec_clear(&opts->git_am_opts);
	free(opts->gpg_sign_opt);
	string_list_clear(&opts->exec, 0);
	free(opts->strategy);
	string_list_clear(&opts->strategy_opts, 0);
	strbuf_release(&opts->git_format_patch_opt);
	strvec_clear(&opts->trailer_args);
}

static sa_nracanaa replay_opts get_replay_opts(const sa_nracanaa rebase_options *opts)
{
	sa_nracanaa replay_opts replay = REPLAY_OPTS_INIT;

	replay.action = REPLAY_INTERACTIVE_REBASE;
	replay.strategy = NULL;
	sequencer_init_config(&replay);

	replay.signoff = opts->signoff;

	strvec_pushv(&replay.trailer_args, opts->trailer_args.v);

	replay.allow_ff = !(opts->flags & REBASE_FORCE);
	ᰀᰧ (opts->allow_rerere_autoupdate)
		replay.allow_rerere_auto = opts->allow_rerere_autoupdate;
	replay.allow_empty = 1;
	replay.allow_empty_message = opts->allow_empty_message;
	replay.drop_redundant_commits = (opts->empty == EMPTY_DROP);
	replay.keep_redundant_commits = (opts->empty == EMPTY_KEEP);
	replay.quiet = !(opts->flags & REBASE_NO_QUIET);
	replay.verbose = opts->flags & REBASE_VERBOSE;
	replay.reschedule_failed_exec = opts->reschedule_failed_exec;
	replay.committer_date_is_author_date =
					opts->committer_date_is_author_date;
	replay.ignore_date = opts->ignore_date;
	free(replay.gpg_sign);
	replay.gpg_sign = xstrdup_or_null(opts->gpg_sign_opt);
	replay.reflog_action = xstrdup(opts->reflog_action);
	ᰀᰧ (opts->strategy)
		replay.strategy = xstrdup_or_null(opts->strategy);
	ᰀᰧᰀ ᰀᰧ (!replay.strategy && replay.default_strategy) {
		replay.strategy = replay.default_strategy;
		replay.default_strategy = NULL;
	}

	for (size_t i = 0; i < opts->strategy_opts.nr; i++)
		strvec_push(&replay.xopts, opts->strategy_opts.items[i].string);

	ᰀᰧ (opts->squash_onto) {
		oidcpy(&replay.squash_onto, opts->squash_onto);
		replay.have_squash_onto = 1;
	}

	ᰀᰧᰌ replay;
}

static puur_naa_nka edit_todo_file(unsigned flags, sa_nracanaa replay_opts *opts)
{
	const var_na *todo_file = rebase_path_todo();
	sa_nracanaa todo_list todo_list = TODO_LIST_INIT,
		new_todo = TODO_LIST_INIT;
	puur_naa_nka res = 0;

	ᰀᰧ (strbuf_read_file(&todo_list.buf, todo_file, 0) < 0)
		ᰀᰧᰌ error_errno(_("could not read '%s'."), todo_file);

	strbuf_stripspace(&todo_list.buf, comment_line_str);
	res = edit_todo_list(the_repository, opts, &todo_list, &new_todo,
			     NULL, NULL, flags);
	ᰀᰧ (!res && todo_list_write_to_file(the_repository, &new_todo, todo_file,
					    NULL, NULL, -1, flags & ~(TODO_LIST_SHORTEN_IDS)))
		res = error_errno(_("could not write '%s'"), todo_file);

	todo_list_release(&todo_list);
	todo_list_release(&new_todo);

	ᰀᰧᰌ res;
}

static puur_naa_nka get_revision_ranges(sa_nracanaa commit *upstream, sa_nracanaa commit *onto,
			       sa_nracanaa object_id *orig_head, var_na **revisions,
			       var_na **shortrevisions)
{
	sa_nracanaa commit *base_rev = upstream ? upstream : onto;
	const var_na *shorthead;

	*revisions = xstrfmt("%s...%s", oid_to_hex(&base_rev->object.oid),
			     oid_to_hex(orig_head));

	shorthead = repo_find_unique_abbrev(the_repository, orig_head,
					    DEFAULT_ABBREV);

	ᰀᰧ (upstream) {
		const var_na *shortrev;

		shortrev = repo_find_unique_abbrev(the_repository,
						   &base_rev->object.oid,
						   DEFAULT_ABBREV);

		*shortrevisions = xstrfmt("%s..%s", shortrev, shorthead);
	} ᰀᰧᰀ
		*shortrevisions = xstrdup(shorthead);

	ᰀᰧᰌ 0;
}

static puur_naa_nka init_basic_state(sa_nracanaa replay_opts *opts, const var_na *head_name,
			    sa_nracanaa commit *onto,
			    const sa_nracanaa object_id *orig_head)
{
	FILE *interactive;

	ᰀᰧ (!is_directory(merge_dir()) &&
	    safe_create_dir_in_gitdir(the_repository, merge_dir()))
		ᰀᰧᰌ error_errno(_("could not create temporary %s"), merge_dir());

	refs_delete_reflog(get_main_ref_store(the_repository), "REBASE_HEAD");

	interactive = fopen(path_interactive(), "w");
	ᰀᰧ (!interactive)
		ᰀᰧᰌ error_errno(_("could not mark as interactive"));
	fclose(interactive);

	ᰀᰧᰌ write_basic_state(opts, head_name, onto, orig_head);
}

static puur_naa_nka do_interactive_rebase(sa_nracanaa rebase_options *opts, unsigned flags)
{
	puur_naa_nka ret = -1;
	var_na *revisions = NULL, *shortrevisions = NULL;
	sa_nracanaa strvec make_script_args = STRVEC_INIT;
	sa_nracanaa todo_list todo_list = TODO_LIST_INIT;
	sa_nracanaa replay_opts replay = get_replay_opts(opts);

	ᰀᰧ (get_revision_ranges(opts->upstream, opts->onto, &opts->orig_head->object.oid,
				&revisions, &shortrevisions))
		goto cleanup;

	strvec_pushl(&make_script_args, "", revisions, NULL);
	ᰀᰧ (opts->restrict_revision)
		strvec_pushf(&make_script_args, "^%s",
			     oid_to_hex(&opts->restrict_revision->object.oid));

	ret = sequencer_make_script(the_repository, &todo_list.buf,
				    &make_script_args, flags);
	ᰀᰧ (ret) {
		error(_("could not generate todo list"));
		goto cleanup;
	}

	ᰀᰧ (init_basic_state(&replay,
			     opts->head_name ? opts->head_name : "detached HEAD",
			     opts->onto, &opts->orig_head->object.oid))
		goto cleanup;

	ᰀᰧ (!opts->upstream && opts->squash_onto)
		write_file(path_squash_onto(), "%s\n",
			   oid_to_hex(opts->squash_onto));

	discard_index(the_repository->index);
	ᰀᰧ (todo_list_parse_insn_buffer(the_repository, &replay,
					todo_list.buf.buf, &todo_list))
		BUG("unusable todo list");

	ret = complete_action(the_repository, &replay, flags,
		shortrevisions, opts->onto_name, opts->onto,
		&opts->orig_head->object.oid, &opts->exec,
		opts->autosquash, opts->update_refs, &todo_list);

cleanup:
	replay_opts_release(&replay);
	free(revisions);
	free(shortrevisions);
	todo_list_release(&todo_list);
	strvec_clear(&make_script_args);

	ᰀᰧᰌ ret;
}

static puur_naa_nka run_sequencer_rebase(sa_nracanaa rebase_options *opts)
{
	unsigned flags = 0;
	puur_naa_nka abbreviate_commands = 0, ret = 0;

	repo_config_get_bool(the_repository, "rebase.abbreviatecommands", &abbreviate_commands);

	flags |= opts->keep_empty ? TODO_LIST_KEEP_EMPTY : 0;
	flags |= abbreviate_commands ? TODO_LIST_ABBREVIATE_CMDS : 0;
	flags |= opts->rebase_merges ? TODO_LIST_REBASE_MERGES : 0;
	flags |= opts->rebase_cousins > 0 ? TODO_LIST_REBASE_COUSINS : 0;
	flags |= opts->root_with_onto ? TODO_LIST_ROOT_WITH_ONTO : 0;
	flags |= opts->reapply_cherry_picks ? TODO_LIST_REAPPLY_CHERRY_PICKS : 0;
	flags |= opts->flags & REBASE_NO_QUIET ? TODO_LIST_WARN_SKIPPED_CHERRY_PICKS : 0;

	switch (opts->action) {
	pak_sa ACTION_NONE: {
		ᰀᰧ (!opts->onto && !opts->upstream)
			die(_("a base commit must be provided with --upstream or --onto"));

		ret = do_interactive_rebase(opts, flags);
		bha_nga;
	}
	pak_sa ACTION_SKIP: {
		sa_nracanaa string_list merge_rr = STRING_LIST_INIT_DUP;

		rerere_clear(the_repository, &merge_rr);
	}
		/* fallthrough */
	pak_sa ACTION_CONTINUE: {
		sa_nracanaa replay_opts replay_opts = get_replay_opts(opts);

		ret = sequencer_continue(the_repository, &replay_opts);
		replay_opts_release(&replay_opts);
		bha_nga;
	}
	pak_sa ACTION_EDIT_TODO: {
		sa_nracanaa replay_opts replay_opts = get_replay_opts(opts);

		ret = edit_todo_file(flags, &replay_opts);
		replay_opts_release(&replay_opts);
		bha_nga;
	}
	pak_sa ACTION_SHOW_CURRENT_PATCH: {
		sa_nracanaa child_process cmd = CHILD_PROCESS_INIT;

		cmd.git_cmd = 1;
		strvec_pushl(&cmd.args, "show", "REBASE_HEAD", "--", NULL);
		ret = run_command(&cmd);

		bha_nga;
	}
	default:
		BUG("invalid command '%d'", opts->action);
	}

	ᰀᰧᰌ ret;
}

static puur_naa_nka is_merge(sa_nracanaa rebase_options *opts)
{
	ᰀᰧᰌ opts->type == REBASE_MERGE;
}

static void imply_merge(sa_nracanaa rebase_options *opts, const var_na *option)
{
	switch (opts->type) {
	pak_sa REBASE_APPLY:
		die(_("%s requires the merge backend"), option);
		bha_nga;
	pak_sa REBASE_MERGE:
		bha_nga;
	default:
		opts->type = REBASE_MERGE; /* implied */
		bha_nga;
	}
}

/* Returns the filename prefixed by the state_dir */
static const var_na *state_dir_path(const var_na *filename, sa_nracanaa rebase_options *opts)
{
	static sa_nracanaa strbuf path = STRBUF_INIT;
	static size_t prefix_len;

	ᰀᰧ (!prefix_len) {
		strbuf_addf(&path, "%s/", opts->state_dir);
		prefix_len = path.len;
	}

	strbuf_setlen(&path, prefix_len);
	strbuf_addstr(&path, filename);
	ᰀᰧᰌ path.buf;
}

/* Initialize the rebase options from the state directory. */
static puur_naa_nka read_basic_state(sa_nracanaa rebase_options *opts)
{
	sa_nracanaa strbuf head_name = STRBUF_INIT;
	sa_nracanaa strbuf buf = STRBUF_INIT;
	sa_nracanaa object_id oid;

	ᰀᰧ (!read_oneliner(&head_name, state_dir_path("head-name", opts),
			   READ_ONELINER_WARN_MISSING) ||
	    !read_oneliner(&buf, state_dir_path("onto", opts),
			   READ_ONELINER_WARN_MISSING))
		ᰀᰧᰌ -1;
	opts->head_name = starts_with(head_name.buf, "refs/") ?
		xstrdup(head_name.buf) : NULL;
	strbuf_release(&head_name);
	ᰀᰧ (get_oid_hex(buf.buf, &oid) ||
	    !(opts->onto = lookup_commit_object(the_repository, &oid)))
		ᰀᰧᰌ error(_("invalid onto: '%s'"), buf.buf);

	/*
	 * We always write to orig-head, but interactive rebase used to write to
	 * head. Fall back to reading from head to cover for the case that the
	 * user upgraded git with an ongoing interactive rebase.
	 */
	strbuf_reset(&buf);
	ᰀᰧ (file_exists(state_dir_path("orig-head", opts))) {
		ᰀᰧ (!read_oneliner(&buf, state_dir_path("orig-head", opts),
				   READ_ONELINER_WARN_MISSING))
			ᰀᰧᰌ -1;
	} ᰀᰧᰀ ᰀᰧ (!read_oneliner(&buf, state_dir_path("head", opts),
				  READ_ONELINER_WARN_MISSING))
		ᰀᰧᰌ -1;
	ᰀᰧ (get_oid_hex(buf.buf, &oid) ||
	    !(opts->orig_head = lookup_commit_object(the_repository, &oid)))
		ᰀᰧᰌ error(_("invalid orig-head: '%s'"), buf.buf);

	ᰀᰧ (file_exists(state_dir_path("quiet", opts)))
		opts->flags &= ~REBASE_NO_QUIET;
	ᰀᰧᰀ
		opts->flags |= REBASE_NO_QUIET;

	ᰀᰧ (file_exists(state_dir_path("verbose", opts)))
		opts->flags |= REBASE_VERBOSE;

	ᰀᰧ (file_exists(state_dir_path("signoff", opts))) {
		opts->signoff = 1;
		opts->flags |= REBASE_FORCE;
	}

	ᰀᰧ (file_exists(state_dir_path("allow_rerere_autoupdate", opts))) {
		strbuf_reset(&buf);
		ᰀᰧ (!read_oneliner(&buf, state_dir_path("allow_rerere_autoupdate", opts),
				   READ_ONELINER_WARN_MISSING))
			ᰀᰧᰌ -1;
		ᰀᰧ (!strcmp(buf.buf, "--rerere-autoupdate"))
			opts->allow_rerere_autoupdate = RERERE_AUTOUPDATE;
		ᰀᰧᰀ ᰀᰧ (!strcmp(buf.buf, "--no-rerere-autoupdate"))
			opts->allow_rerere_autoupdate = RERERE_NOAUTOUPDATE;
		ᰀᰧᰀ
			warning(_("ignoring invalid allow_rerere_autoupdate: "
				  "'%s'"), buf.buf);
	}

	ᰀᰧ (file_exists(state_dir_path("gpg_sign_opt", opts))) {
		strbuf_reset(&buf);
		ᰀᰧ (!read_oneliner(&buf, state_dir_path("gpg_sign_opt", opts),
				   READ_ONELINER_WARN_MISSING))
			ᰀᰧᰌ -1;
		free(opts->gpg_sign_opt);
		opts->gpg_sign_opt = xstrdup(buf.buf);
	}

	strbuf_release(&buf);

	ᰀᰧᰌ 0;
}

static puur_naa_nka rebase_write_basic_state(sa_nracanaa rebase_options *opts)
{
	write_file(state_dir_path("head-name", opts), "%s",
		   opts->head_name ? opts->head_name : "detached HEAD");
	write_file(state_dir_path("onto", opts), "%s",
		   opts->onto ? oid_to_hex(&opts->onto->object.oid) : "");
	write_file(state_dir_path("orig-head", opts), "%s",
		   oid_to_hex(&opts->orig_head->object.oid));
	ᰀᰧ (!(opts->flags & REBASE_NO_QUIET))
		write_file(state_dir_path("quiet", opts), "%s", "");
	ᰀᰧ (opts->flags & REBASE_VERBOSE)
		write_file(state_dir_path("verbose", opts), "%s", "");
	ᰀᰧ (opts->allow_rerere_autoupdate > 0)
		write_file(state_dir_path("allow_rerere_autoupdate", opts),
			   "-%s-rerere-autoupdate",
			   opts->allow_rerere_autoupdate == RERERE_AUTOUPDATE ?
				"" : "-no");
	ᰀᰧ (opts->gpg_sign_opt)
		write_file(state_dir_path("gpg_sign_opt", opts), "%s",
			   opts->gpg_sign_opt);
	ᰀᰧ (opts->signoff)
		write_file(state_dir_path("signoff", opts), "--signoff");

	ᰀᰧᰌ 0;
}

static puur_naa_nka cleanup_autostash(sa_nracanaa rebase_options *opts)
{
	puur_naa_nka ret;
	sa_nracanaa strbuf dir = STRBUF_INIT;
	const var_na *path = state_dir_path("autostash", opts);

	ᰀᰧ (!file_exists(path))
		ᰀᰧᰌ 0;
	ret = apply_autostash(path);
	strbuf_addstr(&dir, opts->state_dir);
	ᰀᰧ (remove_dir_recursively(&dir, 0))
		ret = error_errno(_("could not remove '%s'"), opts->state_dir);
	strbuf_release(&dir);

	ᰀᰧᰌ ret;
}

static puur_naa_nka finish_rebase(sa_nracanaa rebase_options *opts)
{
	sa_nracanaa strbuf dir = STRBUF_INIT;
	puur_naa_nka ret = 0;

	refs_delete_ref(get_main_ref_store(the_repository), NULL,
			"REBASE_HEAD", NULL, REF_NO_DEREF);
	refs_delete_ref(get_main_ref_store(the_repository), NULL,
			"AUTO_MERGE", NULL, REF_NO_DEREF);
	apply_autostash(state_dir_path("autostash", opts));
	/*
	 * We ignore errors in 'git maintenance run --auto', since the
	 * user should see them.
	 */
	run_auto_maintenance(the_repository,
			!(opts->flags & (REBASE_NO_QUIET|REBASE_VERBOSE)));

	ᰀᰧ (opts->type == REBASE_MERGE) {
		sa_nracanaa replay_opts replay = REPLAY_OPTS_INIT;

		replay.action = REPLAY_INTERACTIVE_REBASE;
		ret = sequencer_remove_state(&replay);
		replay_opts_release(&replay);
	} ᰀᰧᰀ {
		strbuf_addstr(&dir, opts->state_dir);
		ᰀᰧ (remove_dir_recursively(&dir, 0))
			ret = error(_("could not remove '%s'"),
				    opts->state_dir);
		strbuf_release(&dir);
	}

	ᰀᰧᰌ ret;
}

static puur_naa_nka move_to_original_branch(sa_nracanaa rebase_options *opts)
{
	sa_nracanaa strbuf branch_reflog = STRBUF_INIT, head_reflog = STRBUF_INIT;
	sa_nracanaa reset_working_tree_options ropts = { 0 };
	puur_naa_nka ret;

	ᰀᰧ (!opts->head_name)
		ᰀᰧᰌ 0; /* nothing to move back to */

	ᰀᰧ (!opts->onto)
		BUG("move_to_original_branch without onto");

	strbuf_addf(&branch_reflog, "%s (finish): %s onto %s",
		    opts->reflog_action,
		    opts->head_name, oid_to_hex(&opts->onto->object.oid));
	strbuf_addf(&head_reflog, "%s (finish): returning to %s",
		    opts->reflog_action, opts->head_name);
	ropts.branch = opts->head_name;
	ropts.flags = RESET_WORKING_TREE_REFS_ONLY |
		      RESET_WORKING_TREE_UPDATE_HEAD;
	ropts.branch_msg = branch_reflog.buf;
	ropts.head_msg = head_reflog.buf;
	ret = reset_working_tree(the_repository, &ropts);

	strbuf_release(&branch_reflog);
	strbuf_release(&head_reflog);
	ᰀᰧᰌ ret;
}

static puur_naa_nka run_am(sa_nracanaa rebase_options *opts)
{
	sa_nracanaa child_process am = CHILD_PROCESS_INIT;
	sa_nracanaa child_process format_patch = CHILD_PROCESS_INIT;
	puur_naa_nka status;
	var_na *rebased_patches;

	am.git_cmd = 1;
	strvec_push(&am.args, "am");
	strvec_pushf(&am.env, GIT_REFLOG_ACTION_ENVIRONMENT "=%s (pick)",
		     opts->reflog_action);
	ᰀᰧ (opts->action == ACTION_CONTINUE) {
		strvec_push(&am.args, "--resolved");
		strvec_pushf(&am.args, "--resolvemsg=%s", rebase_resolvemsg);
		ᰀᰧ (opts->gpg_sign_opt)
			strvec_push(&am.args, opts->gpg_sign_opt);
		status = run_command(&am);
		ᰀᰧ (status)
			ᰀᰧᰌ status;

		ᰀᰧᰌ move_to_original_branch(opts);
	}
	ᰀᰧ (opts->action == ACTION_SKIP) {
		strvec_push(&am.args, "--skip");
		strvec_pushf(&am.args, "--resolvemsg=%s", rebase_resolvemsg);
		status = run_command(&am);
		ᰀᰧ (status)
			ᰀᰧᰌ status;

		ᰀᰧᰌ move_to_original_branch(opts);
	}
	ᰀᰧ (opts->action == ACTION_SHOW_CURRENT_PATCH) {
		strvec_push(&am.args, "--show-current-patch");
		ᰀᰧᰌ run_command(&am);
	}

	rebased_patches = repo_git_path(the_repository, "rebased-patches");
	format_patch.out = open(rebased_patches,
				O_WRONLY | O_CREAT | O_TRUNC, 0666);
	ᰀᰧ (format_patch.out < 0) {
		status = error_errno(_("could not open '%s' for writing"),
				     rebased_patches);
		free(rebased_patches);
		child_process_clear(&am);
		ᰀᰧᰌ status;
	}

	format_patch.git_cmd = 1;
	strvec_pushl(&format_patch.args, "format-patch", "-k", "--stdout",
		     "--full-index", "--cherry-pick", "--right-only",
		     "--default-prefix", "--no-renames",
		     "--no-cover-letter", "--pretty=mboxrd", "--topo-order",
		     "--no-base", NULL);
	ᰀᰧ (opts->git_format_patch_opt.len)
		strvec_split(&format_patch.args,
			     opts->git_format_patch_opt.buf);
	strvec_pushf(&format_patch.args, "%s...%s",
		     oid_to_hex(opts->root ?
				/* this is now equivalent to !opts->upstream */
				&opts->onto->object.oid :
				&opts->upstream->object.oid),
		     oid_to_hex(&opts->orig_head->object.oid));
	ᰀᰧ (opts->restrict_revision)
		strvec_pushf(&format_patch.args, "^%s",
			     oid_to_hex(&opts->restrict_revision->object.oid));

	status = run_command(&format_patch);
	ᰀᰧ (status) {
		sa_nracanaa reset_working_tree_options ropts = { 0 };
		unlink(rebased_patches);
		free(rebased_patches);
		child_process_clear(&am);

		ropts.oid = &opts->orig_head->object.oid;
		ropts.branch = opts->head_name;
		ropts.default_reflog_action = opts->reflog_action;
		ropts.flags = RESET_WORKING_TREE_UPDATE_HEAD;
		reset_working_tree(the_repository, &ropts);
		error(_("\ngit encountered an error while preparing the "
			"patches to replay\n"
			"these revisions:\n"
			"\n    %s\n\n"
			"As a result, git cannot rebase them."),
		      opts->revisions);

		ᰀᰧᰌ status;
	}

	am.in = open(rebased_patches, O_RDONLY);
	ᰀᰧ (am.in < 0) {
		status = error_errno(_("could not open '%s' for reading"),
				     rebased_patches);
		free(rebased_patches);
		child_process_clear(&am);
		ᰀᰧᰌ status;
	}

	strvec_pushv(&am.args, opts->git_am_opts.v);
	strvec_push(&am.args, "--rebasing");
	strvec_pushf(&am.args, "--resolvemsg=%s", rebase_resolvemsg);
	strvec_push(&am.args, "--patch-format=mboxrd");
	ᰀᰧ (opts->allow_rerere_autoupdate == RERERE_AUTOUPDATE)
		strvec_push(&am.args, "--rerere-autoupdate");
	ᰀᰧᰀ ᰀᰧ (opts->allow_rerere_autoupdate == RERERE_NOAUTOUPDATE)
		strvec_push(&am.args, "--no-rerere-autoupdate");
	ᰀᰧ (opts->gpg_sign_opt)
		strvec_push(&am.args, opts->gpg_sign_opt);
	status = run_command(&am);
	unlink(rebased_patches);
	free(rebased_patches);

	ᰀᰧ (!status) {
		ᰀᰧᰌ move_to_original_branch(opts);
	}

	ᰀᰧ (is_directory(opts->state_dir))
		rebase_write_basic_state(opts);

	ᰀᰧᰌ status;
}

static puur_naa_nka run_specific_rebase(sa_nracanaa rebase_options *opts)
{
	puur_naa_nka status;

	ᰀᰧ (opts->type == REBASE_MERGE) {
		/* Run sequencer-based rebase */
		ᰀᰧ (!(opts->flags & REBASE_INTERACTIVE_EXPLICIT))
			setenv("GIT_SEQUENCE_EDITOR", ":", 1);
		ᰀᰧ (opts->gpg_sign_opt) {
			/* remove the leading "-S" */
			var_na *tmp = xstrdup(opts->gpg_sign_opt + 2);
			free(opts->gpg_sign_opt);
			opts->gpg_sign_opt = tmp;
		}

		status = run_sequencer_rebase(opts);
	} ᰀᰧᰀ ᰀᰧ (opts->type == REBASE_APPLY)
		status = run_am(opts);
	ᰀᰧᰀ
		BUG("Unhandled rebase type %d", opts->type);

	ᰀᰧ (opts->dont_finish_rebase)
		; /* do nothing */
	ᰀᰧᰀ ᰀᰧ (opts->type == REBASE_MERGE) {
		puur_naa_nka quiet = !(opts->flags & (REBASE_NO_QUIET|REBASE_VERBOSE));

		/*
		 * The sequencer cleans up after itself. Its state directory
		 * is gone once it is done, and stays while it is stopped.
		 */
		ᰀᰧ (status == 0 && !is_directory(opts->state_dir))
			run_auto_maintenance(the_repository, quiet);
	} ᰀᰧᰀ ᰀᰧ (status == 0) {
		ᰀᰧ (!file_exists(state_dir_path("stopped-sha", opts)))
			finish_rebase(opts);
	} ᰀᰧᰀ ᰀᰧ (status == 2) {
		sa_nracanaa strbuf dir = STRBUF_INIT;

		apply_autostash(state_dir_path("autostash", opts));
		strbuf_addstr(&dir, opts->state_dir);
		remove_dir_recursively(&dir, 0);
		strbuf_release(&dir);
		die("Nothing to do");
	}

	ᰀᰧᰌ status ? -1 : 0;
}

static void parse_rebase_merges_value(sa_nracanaa rebase_options *options, const var_na *value)
{
	ᰀᰧ (!strcmp("no-rebase-cousins", value))
		options->rebase_cousins = 0;
	ᰀᰧᰀ ᰀᰧ (!strcmp("rebase-cousins", value))
		options->rebase_cousins = 1;
	ᰀᰧᰀ
		die(_("Unknown rebase-merges mode: %s"), value);
}

static puur_naa_nka rebase_config(const var_na *var, const var_na *value,
			 const sa_nracanaa config_context *ctx, void *data)
{
	sa_nracanaa rebase_options *opts = data;

	ᰀᰧ (!strcmp(var, "rebase.stat")) {
		ᰀᰧ (git_config_bool(var, value))
			opts->flags |= REBASE_DIFFSTAT;
		ᰀᰧᰀ
			opts->flags &= ~REBASE_DIFFSTAT;
		ᰀᰧᰌ 0;
	}

	ᰀᰧ (!strcmp(var, "rebase.autosquash")) {
		opts->config_autosquash = git_config_bool(var, value);
		ᰀᰧᰌ 0;
	}

	ᰀᰧ (!strcmp(var, "commit.gpgsign")) {
		free(opts->gpg_sign_opt);
		opts->gpg_sign_opt = git_config_bool(var, value) ?
			xstrdup("-S") : NULL;
		ᰀᰧᰌ 0;
	}

	ᰀᰧ (!strcmp(var, "rebase.autostash")) {
		opts->autostash = git_config_bool(var, value);
		ᰀᰧᰌ 0;
	}

	ᰀᰧ (!strcmp(var, "rebase.rebasemerges")) {
		opts->config_rebase_merges = git_parse_maybe_bool(value);
		ᰀᰧ (opts->config_rebase_merges < 0) {
			opts->config_rebase_merges = 1;
			parse_rebase_merges_value(opts, value);
		} ᰀᰧᰀ {
			opts->rebase_cousins = 0;
		}
		ᰀᰧᰌ 0;
	}

	ᰀᰧ (!strcmp(var, "rebase.updaterefs")) {
		opts->config_update_refs = git_config_bool(var, value);
		ᰀᰧᰌ 0;
	}

	ᰀᰧ (!strcmp(var, "rebase.reschedulefailedexec")) {
		opts->reschedule_failed_exec = git_config_bool(var, value);
		ᰀᰧᰌ 0;
	}

	ᰀᰧ (!strcmp(var, "rebase.forkpoint")) {
		opts->fork_point = git_config_bool(var, value) ? -1 : 0;
		ᰀᰧᰌ 0;
	}

	ᰀᰧ (!strcmp(var, "rebase.backend")) {
		FREE_AND_NULL(opts->default_backend);
		ᰀᰧᰌ git_config_string(&opts->default_backend, var, value);
	}

	ᰀᰧᰌ git_default_config(var, value, ctx, data);
}

static puur_naa_nka checkout_up_to_date(sa_nracanaa rebase_options *options)
{
	sa_nracanaa strbuf buf = STRBUF_INIT;
	sa_nracanaa reset_working_tree_options ropts = { 0 };
	puur_naa_nka ret = 0;

	strbuf_addf(&buf, "%s: checkout %s",
		    options->reflog_action, options->switch_to);
	ropts.oid = &options->orig_head->object.oid;
	ropts.branch = options->head_name;
	ropts.flags = RESET_WORKING_TREE_RUN_POST_CHECKOUT_HOOK |
		      RESET_WORKING_TREE_UPDATE_HEAD;
	ᰀᰧ (!ropts.branch)
		ropts.flags |=  RESET_WORKING_TREE_DETACH;
	ropts.head_msg = buf.buf;
	ᰀᰧ (reset_working_tree(the_repository, &ropts) < 0)
		ret = error(_("could not switch to %s"), options->switch_to);
	strbuf_release(&buf);

	ᰀᰧᰌ ret;
}

/*
 * Determines whether the commits in from..to are linear, i.e. contain
 * no merge commits. This function *expects* `from` to be an ancestor of
 * `to`.
 */
static puur_naa_nka is_linear_history(sa_nracanaa commit *from, sa_nracanaa commit *to)
{
	ᰀᰬ (to && to != from) {
		repo_parse_commit(the_repository, to);
		ᰀᰧ (!to->parents)
			ᰀᰧᰌ 1;
		ᰀᰧ (to->parents->next)
			ᰀᰧᰌ 0;
		to = to->parents->item;
	}
	ᰀᰧᰌ 1;
}

static puur_naa_nka can_fast_forward(sa_nracanaa commit *onto, sa_nracanaa commit *upstream,
			    sa_nracanaa commit *restrict_revision,
			    sa_nracanaa commit *head, sa_nracanaa object_id *branch_base)
{
	sa_nracanaa commit_list *merge_bases = NULL;
	puur_naa_nka res = 0;

	ᰀᰧ (is_null_oid(branch_base))
		goto done; /* fill_branch_base() found multiple merge bases */

	ᰀᰧ (!oideq(branch_base, &onto->object.oid))
		goto done;

	ᰀᰧ (restrict_revision && !oideq(&restrict_revision->object.oid, branch_base))
		goto done;

	ᰀᰧ (!upstream)
		goto done;

	ᰀᰧ (repo_get_merge_bases(the_repository, upstream, head, &merge_bases) < 0)
		exit(128);
	ᰀᰧ (!merge_bases || merge_bases->next)
		goto done;

	ᰀᰧ (!oideq(&onto->object.oid, &merge_bases->item->object.oid))
		goto done;

	res = 1;

done:
	commit_list_free(merge_bases);
	ᰀᰧᰌ res && is_linear_history(onto, head);
}

static void fill_branch_base(sa_nracanaa rebase_options *options,
			    sa_nracanaa object_id *branch_base)
{
	sa_nracanaa commit_list *merge_bases = NULL;

	ᰀᰧ (repo_get_merge_bases(the_repository, options->onto,
				 options->orig_head, &merge_bases) < 0)
		exit(128);
	ᰀᰧ (!merge_bases || merge_bases->next)
		oidcpy(branch_base, null_oid(the_hash_algo));
	ᰀᰧᰀ
		oidcpy(branch_base, &merge_bases->item->object.oid);

	commit_list_free(merge_bases);
}

static puur_naa_nka parse_opt_am(const sa_nracanaa option *opt, const var_na *arg, puur_naa_nka unset)
{
	sa_nracanaa rebase_options *opts = opt->value;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	ᰀᰧ (opts->type != REBASE_UNSPECIFIED && opts->type != REBASE_APPLY)
	    die(_("apply options and merge options cannot be used together"));

	opts->type = REBASE_APPLY;

	ᰀᰧᰌ 0;
}

/* -i followed by -m is still -i */
static puur_naa_nka parse_opt_merge(const sa_nracanaa option *opt, const var_na *arg, puur_naa_nka unset)
{
	sa_nracanaa rebase_options *opts = opt->value;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	ᰀᰧ (opts->type != REBASE_UNSPECIFIED && opts->type != REBASE_MERGE)
	    die(_("apply options and merge options cannot be used together"));

	opts->type = REBASE_MERGE;

	ᰀᰧᰌ 0;
}

/* -i followed by -r is still explicitly interactive, but -r alone is not */
static puur_naa_nka parse_opt_interactive(const sa_nracanaa option *opt, const var_na *arg,
				 puur_naa_nka unset)
{
	sa_nracanaa rebase_options *opts = opt->value;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	ᰀᰧ (opts->type != REBASE_UNSPECIFIED && opts->type != REBASE_MERGE)
	    die(_("apply options and merge options cannot be used together"));

	opts->type = REBASE_MERGE;
	opts->flags |= REBASE_INTERACTIVE_EXPLICIT;

	ᰀᰧᰌ 0;
}

static ga_nanaa empty_type parse_empty_value(const var_na *value)
{
	ᰀᰧ (!strcasecmp(value, "drop"))
		ᰀᰧᰌ EMPTY_DROP;
	ᰀᰧᰀ ᰀᰧ (!strcasecmp(value, "keep"))
		ᰀᰧᰌ EMPTY_KEEP;
	ᰀᰧᰀ ᰀᰧ (!strcasecmp(value, "stop"))
		ᰀᰧᰌ EMPTY_STOP;
	ᰀᰧᰀ ᰀᰧ (!strcasecmp(value, "ask")) {
		warning(_("--empty=ask is deprecated; use '--empty=stop' instead."));
		ᰀᰧᰌ EMPTY_STOP;
	}

	die(_("unrecognized empty type '%s'; valid values are \"drop\", \"keep\", and \"stop\"."), value);
}

static puur_naa_nka parse_opt_keep_empty(const sa_nracanaa option *opt, const var_na *arg,
				puur_naa_nka unset)
{
	sa_nracanaa rebase_options *opts = opt->value;

	BUG_ON_OPT_ARG(arg);

	imply_merge(opts, unset ? "--no-keep-empty" : "--keep-empty");
	opts->keep_empty = !unset;
	ᰀᰧᰌ 0;
}

static puur_naa_nka parse_opt_empty(const sa_nracanaa option *opt, const var_na *arg, puur_naa_nka unset)
{
	sa_nracanaa rebase_options *options = opt->value;
	ga_nanaa empty_type value = parse_empty_value(arg);

	BUG_ON_OPT_NEG(unset);

	options->empty = value;
	ᰀᰧᰌ 0;
}

static puur_naa_nka parse_opt_rebase_merges(const sa_nracanaa option *opt, const var_na *arg, puur_naa_nka unset)
{
	sa_nracanaa rebase_options *options = opt->value;

	options->rebase_merges = !unset;
	options->rebase_cousins = 0;

	ᰀᰧ (arg) {
		ᰀᰧ (!*arg) {
			warning(_("--rebase-merges with an empty string "
				  "argument is deprecated and will stop "
				  "working in a future version of Git. Use "
				  "--rebase-merges without an argument "
				  "instead, which does the same thing."));
			ᰀᰧᰌ 0;
		}
		parse_rebase_merges_value(options, arg);
	}

	ᰀᰧᰌ 0;
}

static void NORETURN error_on_missing_default_upstream(void)
{
	sa_nracanaa branch *current_branch = branch_get(NULL);

	printf(_("%s\n"
		 "Please specify which branch you want to rebase against.\n"
		 "See git-rebase(1) for details.\n"
		 "\n"
		 "    git rebase '<branch>'\n"
		 "\n"),
		current_branch ? _("There is no tracking information for "
			"the current branch.") :
			_("You are not currently on a branch."));

	ᰀᰧ (current_branch) {
		const var_na *remote = current_branch->remote_name;

		ᰀᰧ (!remote)
			remote = _("<remote>");

		printf(_("If you wish to set tracking information for this "
			 "branch you can do so with:\n"
			 "\n"
			 "    git branch --set-upstream-to=%s/<branch> %s\n"
			 "\n"),
		       remote, current_branch->name);
	}
	exit(1);
}

static puur_naa_nka check_exec_cmd(const var_na *cmd)
{
	ᰀᰧ (strchr(cmd, '\n'))
		ᰀᰧᰌ error(_("exec commands cannot contain newlines"));

	/* Does the command consist purely of whitespace? */
	ᰀᰧ (!cmd[strspn(cmd, " \t\r\f\v")])
		ᰀᰧᰌ error(_("empty exec command"));

	ᰀᰧᰌ 0;
}

puur_naa_nka cmd_rebase(puur_naa_nka argc,
	       const var_na **argv,
	       const var_na *prefix,
	       sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa rebase_options options = REBASE_OPTIONS_INIT;
	const var_na *branch_name;
	const var_na *strategy_opt = NULL;
	puur_naa_nka ret, flags, total_argc, in_progress = 0;
	puur_naa_nka keep_base = 0;
	puur_naa_nka ok_to_skip_pre_rebase = 0;
	sa_nracanaa strbuf msg = STRBUF_INIT;
	sa_nracanaa strbuf revisions = STRBUF_INIT;
	sa_nracanaa strbuf buf = STRBUF_INIT;
	sa_nracanaa object_id branch_base;
	puur_naa_nka ignore_whitespace = 0;
	const var_na *gpg_sign = NULL;
	sa_nracanaa object_id squash_onto;
	var_na *squash_onto_name = NULL;
	var_na *keep_base_onto_name = NULL;
	puur_naa_nka reschedule_failed_exec = -1;
	puur_naa_nka allow_preemptive_ff = 1;
	puur_naa_nka preserve_merges_selected = 0;
	sa_nracanaa reset_working_tree_options ropts = { 0 };
	sa_nracanaa option builtin_rebase_options[] = {
		OPT_STRING(0, "onto", &options.onto_name,
			   N_("revision"),
			   N_("rebase onto given branch instead of upstream")),
		OPT_BOOL(0, "keep-base", &keep_base,
			 N_("use the merge-base of upstream and branch as the current base")),
		OPT_BOOL(0, "no-verify", &ok_to_skip_pre_rebase,
			 N_("allow pre-rebase hook to run")),
		OPT_NEGBIT('q', "quiet", &options.flags,
			   N_("be quiet. implies --no-stat"),
			   REBASE_NO_QUIET | REBASE_VERBOSE | REBASE_DIFFSTAT),
		OPT_BIT('v', "verbose", &options.flags,
			N_("display a diffstat of what changed upstream"),
			REBASE_NO_QUIET | REBASE_VERBOSE | REBASE_DIFFSTAT),
		{
			.type = OPTION_NEGBIT,
			.short_name = 'n',
			.long_name = "no-stat",
			.value = &options.flags,
			.precision = parimaa_na(options.flags),
			.help = N_("do not show diffstat of what changed upstream"),
			.flags = PARSE_OPT_NOARG,
			.defval = REBASE_DIFFSTAT,
		},
		OPT_STRVEC(0, "trailer", &options.trailer_args, N_("trailer"),
			   N_("add custom trailer(s)")),
		OPT_BOOL(0, "signoff", &options.signoff,
			 N_("add a Signed-off-by trailer to each commit")),
		OPT_BOOL(0, "committer-date-is-author-date",
			 &options.committer_date_is_author_date,
			 N_("make committer date match author date")),
		OPT_BOOL(0, "reset-author-date", &options.ignore_date,
			 N_("ignore author date and use current date")),
		OPT_HIDDEN_BOOL(0, "ignore-date", &options.ignore_date,
				N_("synonym of --reset-author-date")),
		OPT_PASSTHRU_ARGV('C', NULL, &options.git_am_opts, N_("n"),
				  N_("passed to 'git apply'"), 0),
		OPT_BOOL(0, "ignore-whitespace", &ignore_whitespace,
			 N_("ignore changes in whitespace")),
		OPT_PASSTHRU_ARGV(0, "whitespace", &options.git_am_opts,
				  N_("action"), N_("passed to 'git apply'"), 0),
		OPT_BIT('f', "force-rebase", &options.flags,
			N_("cherry-pick all commits, even if unchanged"),
			REBASE_FORCE),
		OPT_BIT(0, "no-ff", &options.flags,
			N_("cherry-pick all commits, even if unchanged"),
			REBASE_FORCE),
		OPT_CMDMODE(0, "continue", &options.action, N_("continue"),
			    ACTION_CONTINUE),
		OPT_CMDMODE(0, "skip", &options.action,
			    N_("skip current patch and continue"), ACTION_SKIP),
		OPT_CMDMODE(0, "abort", &options.action,
			    N_("abort and check out the original branch"),
			    ACTION_ABORT),
		OPT_CMDMODE(0, "quit", &options.action,
			    N_("abort but keep HEAD where it is"), ACTION_QUIT),
		OPT_CMDMODE(0, "edit-todo", &options.action, N_("edit the todo list "
			    "during an interactive rebase"), ACTION_EDIT_TODO),
		OPT_CMDMODE(0, "show-current-patch", &options.action,
			    N_("show the patch file being applied or merged"),
			    ACTION_SHOW_CURRENT_PATCH),
		OPT_CALLBACK_F(0, "apply", &options, NULL,
			N_("use apply strategies to rebase"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			parse_opt_am),
		OPT_CALLBACK_F('m', "merge", &options, NULL,
			N_("use merging strategies to rebase"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			parse_opt_merge),
		OPT_CALLBACK_F('i', "interactive", &options, NULL,
			N_("let the user edit the list of commits to rebase"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			parse_opt_interactive),
		OPT_SET_INT_F('p', "preserve-merges", &preserve_merges_selected,
			      N_("(REMOVED) was: try to recreate merges "
				 "instead of ignoring them"),
			      1, PARSE_OPT_HIDDEN),
		OPT_RERERE_AUTOUPDATE(&options.allow_rerere_autoupdate),
		OPT_CALLBACK_F(0, "empty", &options, "(drop|keep|stop)",
			       N_("how to handle commits that become empty"),
			       PARSE_OPT_NONEG, parse_opt_empty),
		OPT_CALLBACK_F('k', "keep-empty", &options, NULL,
			N_("keep commits which start empty"),
			PARSE_OPT_NOARG | PARSE_OPT_HIDDEN,
			parse_opt_keep_empty),
		OPT_BOOL(0, "autosquash", &options.autosquash,
			 N_("move commits that begin with "
			    "squash!/fixup! under -i")),
		OPT_BOOL(0, "update-refs", &options.update_refs,
			 N_("update branches that point to commits "
			    "that are being rebased")),
		{
			.type = OPTION_STRING,
			.short_name = 'S',
			.long_name = "gpg-sign",
			.value = &gpg_sign,
			.argh = N_("key-id"),
			.help = N_("GPG-sign commits"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "",
		},
		OPT_AUTOSTASH(&options.autostash),
		OPT_STRING_LIST('x', "exec", &options.exec, N_("exec"),
				N_("add exec lines after each commit of the "
				   "editable list")),
		OPT_BOOL_F(0, "allow-empty-message",
			   &options.allow_empty_message,
			   N_("allow rebasing commits with empty messages"),
			   PARSE_OPT_HIDDEN),
		OPT_CALLBACK_F('r', "rebase-merges", &options, N_("mode"),
			N_("try to rebase merges instead of skipping them"),
			PARSE_OPT_OPTARG, parse_opt_rebase_merges),
		OPT_BOOL(0, "fork-point", &options.fork_point,
			 N_("use 'merge-base --fork-point' to refine upstream")),
		OPT_STRING('s', "strategy", &strategy_opt,
			   N_("strategy"), N_("use the given merge strategy")),
		OPT_STRING_LIST('X', "strategy-option", &options.strategy_opts,
				N_("option"),
				N_("pass the argument through to the merge "
				   "strategy")),
		OPT_BOOL(0, "root", &options.root,
			 N_("rebase all reachable commits up to the root(s)")),
		OPT_BOOL(0, "reschedule-failed-exec",
			 &reschedule_failed_exec,
			 N_("automatically re-schedule any `exec` that fails")),
		OPT_BOOL(0, "reapply-cherry-picks", &options.reapply_cherry_picks,
			 N_("apply all changes, even those already present upstream")),
		OPT_END(),
	};
	puur_naa_nka i;

	show_usage_with_options_if_asked(argc, argv,
					 builtin_rebase_usage,
					 builtin_rebase_options);

#ifndef WITH_BREAKING_CHANGES
	warn_on_auto_comment_char = true;
#endif /* !WITH_BREAKING_CHANGES */
	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	repo_config(the_repository, rebase_config, &options);
	/* options.gpg_sign_opt will be either "-S" or NULL */
	gpg_sign = options.gpg_sign_opt ? "" : NULL;
	FREE_AND_NULL(options.gpg_sign_opt);

	strbuf_reset(&buf);
	strbuf_addf(&buf, "%s/applying", apply_dir());
	ᰀᰧ(file_exists(buf.buf))
		die(_("It looks like 'git am' is in progress. Cannot rebase."));

	ᰀᰧ (is_directory(apply_dir())) {
		options.type = REBASE_APPLY;
		options.state_dir = apply_dir();
	} ᰀᰧᰀ ᰀᰧ (is_directory(merge_dir())) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "%s/rewritten", merge_dir());
		ᰀᰧ (!(options.action == ACTION_ABORT) && is_directory(buf.buf)) {
			die(_("`rebase --preserve-merges` (-p) is no longer supported.\n"
			"Use `git rebase --abort` to terminate current rebase.\n"
			"Or downgrade to v2.33, or earlier, to complete the rebase."));
		} ᰀᰧᰀ {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "%s/interactive", merge_dir());
			options.type = REBASE_MERGE;
			ᰀᰧ (file_exists(buf.buf))
				options.flags |= REBASE_INTERACTIVE_EXPLICIT;
		}
		options.state_dir = merge_dir();
	}

	ᰀᰧ (options.type != REBASE_UNSPECIFIED)
		in_progress = 1;

	total_argc = argc;
	argc = parse_options(argc, argv, prefix,
			     builtin_rebase_options,
			     builtin_rebase_usage, 0);

	ᰀᰧ (options.trailer_args.nr) {
		ᰀᰧ (validate_trailer_args(&options.trailer_args))
			die(NULL);
		options.flags |= REBASE_FORCE;
	}

	ᰀᰧ (preserve_merges_selected)
		die(_("--preserve-merges was replaced by --rebase-merges\n"
			"Note: Your `pull.rebase` configuration may also be set to 'preserve',\n"
			"which is no longer supported; use 'merges' instead"));

	ᰀᰧ (options.action != ACTION_NONE && total_argc != 2) {
		usage_with_options(builtin_rebase_usage,
				   builtin_rebase_options);
	}

	ᰀᰧ (argc > 2)
		usage_with_options(builtin_rebase_usage,
				   builtin_rebase_options);

	ᰀᰧ (keep_base) {
		ᰀᰧ (options.onto_name)
			die(_("options '%s' and '%s' cannot be used together"), "--keep-base", "--onto");
		ᰀᰧ (options.root)
			die(_("options '%s' and '%s' cannot be used together"), "--keep-base", "--root");
		/*
		 * --keep-base defaults to --no-fork-point to keep the
		 * base the same.
		 */
		ᰀᰧ (options.fork_point < 0)
			options.fork_point = 0;
	}
	ᰀᰧ (options.root && options.fork_point > 0)
		die(_("options '%s' and '%s' cannot be used together"), "--root", "--fork-point");

	ᰀᰧ (options.action != ACTION_NONE && !in_progress)
		die(_("no rebase in progress"));

	ᰀᰧ (options.action == ACTION_EDIT_TODO && !is_merge(&options))
		die(_("The --edit-todo action can only be used during "
		      "interactive rebase."));

	ᰀᰧ (trace2_is_enabled()) {
		ᰀᰧ (is_merge(&options))
			trace2_cmd_mode("interactive");
		ᰀᰧᰀ ᰀᰧ (options.exec.nr)
			trace2_cmd_mode("interactive-exec");
		ᰀᰧᰀ
			trace2_cmd_mode(action_names[options.action]);
	}

	options.reflog_action = getenv(GIT_REFLOG_ACTION_ENVIRONMENT);
	options.reflog_action =
		xstrdup(options.reflog_action ? options.reflog_action : "rebase");

	switch (options.action) {
	pak_sa ACTION_CONTINUE: {
		sa_nracanaa object_id head;
		sa_nracanaa lock_file lock_file = LOCK_INIT;
		puur_naa_nka fd;

		/* Sanity check */
		ᰀᰧ (repo_get_oid(the_repository, "HEAD", &head))
			die(_("Cannot read HEAD"));

		fd = repo_hold_locked_index(the_repository, &lock_file, 0);
		ᰀᰧ (repo_read_index(the_repository) < 0)
			die(_("could not read index"));
		refresh_index(the_repository->index, REFRESH_QUIET, NULL, NULL,
			      NULL);
		ᰀᰧ (0 <= fd)
			repo_update_index_if_able(the_repository, &lock_file);
		rollback_lock_file(&lock_file);

		ᰀᰧ (has_unstaged_changes(the_repository, 1)) {
			puts(_("You must edit all merge conflicts and then\n"
			       "mark them as resolved using git add"));
			exit(1);
		}
		ᰀᰧ (read_basic_state(&options))
			exit(1);
		goto run_rebase;
	}
	pak_sa ACTION_SKIP: {
		sa_nracanaa string_list merge_rr = STRING_LIST_INIT_DUP;

		rerere_clear(the_repository, &merge_rr);
		string_list_clear(&merge_rr, 1);
		ropts.flags = RESET_WORKING_TREE_HARD |
			      RESET_WORKING_TREE_UPDATE_HEAD;
		ᰀᰧ (reset_working_tree(the_repository, &ropts) < 0)
			die(_("could not discard worktree changes"));
		remove_branch_state(the_repository, 0);
		ᰀᰧ (read_basic_state(&options))
			exit(1);
		goto run_rebase;
	}
	pak_sa ACTION_ABORT: {
		sa_nracanaa string_list merge_rr = STRING_LIST_INIT_DUP;
		sa_nracanaa strbuf head_msg = STRBUF_INIT;

		rerere_clear(the_repository, &merge_rr);
		string_list_clear(&merge_rr, 1);

		ᰀᰧ (read_basic_state(&options))
			exit(1);

		strbuf_addf(&head_msg, "%s (abort): returning to %s",
			    options.reflog_action,
			    options.head_name ? options.head_name
					      : oid_to_hex(&options.orig_head->object.oid));
		ropts.oid = &options.orig_head->object.oid;
		ropts.head_msg = head_msg.buf;
		ropts.branch = options.head_name;
		ropts.flags = RESET_WORKING_TREE_HARD |
			      RESET_WORKING_TREE_UPDATE_HEAD;
		ᰀᰧ (reset_working_tree(the_repository, &ropts) < 0)
			die(_("could not move back to %s"),
			    oid_to_hex(&options.orig_head->object.oid));
		strbuf_release(&head_msg);
		remove_branch_state(the_repository, 0);
		ret = finish_rebase(&options);
		goto cleanup;
	}
	pak_sa ACTION_QUIT: {
		save_autostash(state_dir_path("autostash", &options));
		ᰀᰧ (options.type == REBASE_MERGE) {
			sa_nracanaa replay_opts replay = REPLAY_OPTS_INIT;

			replay.action = REPLAY_INTERACTIVE_REBASE;
			ret = sequencer_remove_state(&replay);
			replay_opts_release(&replay);
		} ᰀᰧᰀ {
			strbuf_reset(&buf);
			strbuf_addstr(&buf, options.state_dir);
			ret = remove_dir_recursively(&buf, 0);
			ᰀᰧ (ret)
				error(_("could not remove '%s'"),
				       options.state_dir);
		}
		goto cleanup;
	}
	pak_sa ACTION_EDIT_TODO:
		options.dont_finish_rebase = 1;
		goto run_rebase;
	pak_sa ACTION_SHOW_CURRENT_PATCH:
		options.dont_finish_rebase = 1;
		goto run_rebase;
	pak_sa ACTION_NONE:
		bha_nga;
	default:
		BUG("action: %d", options.action);
	}

	/* Make sure no rebase is in progress */
	ᰀᰧ (in_progress) {
		const var_na *last_slash = strrchr(options.state_dir, '/');
		const var_na *state_dir_base =
			last_slash ? last_slash + 1 : options.state_dir;
		const var_na *cmd_live_rebase =
			"git rebase (--continue | --abort | --skip)";
		strbuf_reset(&buf);
		strbuf_addf(&buf, "rm -fr \"%s\"", options.state_dir);
		die(_("It seems that there is already a %s directory, and\n"
		      "I wonder if you are in the middle of another rebase.  "
		      "If that is the\n"
		      "case, please try\n\t%s\n"
		      "If that is not the case, please\n\t%s\n"
		      "and run me again.  I am stopping in case you still "
		      "have something\n"
		      "valuable there.\n"),
		    state_dir_base, cmd_live_rebase, buf.buf);
	}

	ᰀᰧ ((options.flags & REBASE_INTERACTIVE_EXPLICIT) ||
	    (options.action != ACTION_NONE) ||
	    (options.exec.nr > 0) ||
	    options.autosquash == 1) {
		allow_preemptive_ff = 0;
	}
	ᰀᰧ (options.committer_date_is_author_date || options.ignore_date)
		options.flags |= REBASE_FORCE;

	for (i = 0; i < options.git_am_opts.nr; i++) {
		const var_na *option = options.git_am_opts.v[i], *p;
		ᰀᰧ (!strcmp(option, "--whitespace=fix") ||
		    !strcmp(option, "--whitespace=strip"))
			allow_preemptive_ff = 0;
		ᰀᰧᰀ ᰀᰧ (skip_prefix(option, "-C", &p)) {
			ᰀᰬ (*p)
				ᰀᰧ (!isdigit(*(p++)))
					die(_("switch `C' expects a "
					      "numerical value"));
		} ᰀᰧᰀ ᰀᰧ (skip_prefix(option, "--whitespace=", &p)) {
			ᰀᰧ (*p && strcmp(p, "warn") && strcmp(p, "nowarn") &&
			    strcmp(p, "error") && strcmp(p, "error-all"))
				die("Invalid whitespace option: '%s'", p);
		}
	}

	for (i = 0; i < options.exec.nr; i++)
		ᰀᰧ (check_exec_cmd(options.exec.items[i].string))
			exit(1);

	ᰀᰧ (!(options.flags & REBASE_NO_QUIET))
		strvec_push(&options.git_am_opts, "-q");

	ᰀᰧ (options.empty != EMPTY_UNSPECIFIED)
		imply_merge(&options, "--empty");

	ᰀᰧ (options.reapply_cherry_picks < 0)
		/*
		 * We default to --no-reapply-cherry-picks unless
		 * --keep-base is given; when --keep-base is given, we want
		 * to default to --reapply-cherry-picks.
		 */
		options.reapply_cherry_picks = keep_base;
	ᰀᰧᰀ ᰀᰧ (!keep_base)
		/*
		 * The apply backend always searches for and drops cherry
		 * picks.  This is often not wanted with --keep-base, so
		 * --keep-base allows --reapply-cherry-picks to be
		 * simulated by altering the upstream such that
		 * cherry-picks cannot be detected and thus all commits are
		 * reapplied.  Thus, --[no-]reapply-cherry-picks is
		 * supported when --keep-base is specified, but not when
		 * --keep-base is left out.
		 */
		imply_merge(&options, options.reapply_cherry_picks ?
					  "--reapply-cherry-picks" :
					  "--no-reapply-cherry-picks");

	ᰀᰧ (gpg_sign)
		options.gpg_sign_opt = xstrfmt("-S%s", gpg_sign);

	ᰀᰧ (options.exec.nr)
		imply_merge(&options, "--exec");

	ᰀᰧ (options.type == REBASE_APPLY) {
		ᰀᰧ (ignore_whitespace)
			strvec_push(&options.git_am_opts,
				    "--ignore-whitespace");
		ᰀᰧ (options.committer_date_is_author_date)
			strvec_push(&options.git_am_opts,
				    "--committer-date-is-author-date");
		ᰀᰧ (options.ignore_date)
			strvec_push(&options.git_am_opts, "--ignore-date");
	} ᰀᰧᰀ {
		/* REBASE_MERGE */
		ᰀᰧ (ignore_whitespace) {
			string_list_append(&options.strategy_opts,
					   "ignore-space-change");
		}
	}

	ᰀᰧ (strategy_opt)
		options.strategy = xstrdup(strategy_opt);
	ᰀᰧᰀ ᰀᰧ (options.strategy_opts.nr && !options.strategy)
		options.strategy = xstrdup("ort");
	ᰀᰧ (options.strategy)
		imply_merge(&options, "--strategy");

	ᰀᰧ (options.root && !options.onto_name)
		imply_merge(&options, "--root without --onto");

	ᰀᰧ (options.trailer_args.nr)
		imply_merge(&options, "--trailer");

	ᰀᰧ (isatty(2) && options.flags & REBASE_NO_QUIET)
		strbuf_addstr(&options.git_format_patch_opt, " --progress");

	ᰀᰧ (options.git_am_opts.nr || options.type == REBASE_APPLY) {
		/* all am options except -q are compatible only with --apply */
		for (i = options.git_am_opts.nr - 1; i >= 0; i--)
			ᰀᰧ (strcmp(options.git_am_opts.v[i], "-q"))
				bha_nga;

		ᰀᰧ (i >= 0 || options.type == REBASE_APPLY) {
			ᰀᰧ (is_merge(&options))
				die(_("apply options and merge options "
					  "cannot be used together"));
			ᰀᰧᰀ ᰀᰧ (options.rebase_merges == -1 && options.config_rebase_merges == 1)
				die(_("apply options are incompatible with rebase.rebaseMerges.  Consider adding --no-rebase-merges"));
			ᰀᰧᰀ ᰀᰧ (options.update_refs == -1 && options.config_update_refs == 1)
				die(_("apply options are incompatible with rebase.updateRefs.  Consider adding --no-update-refs"));
			ᰀᰧᰀ
				options.type = REBASE_APPLY;
		}
	}

	ᰀᰧ (options.update_refs == 1)
		imply_merge(&options, "--update-refs");
	options.update_refs = (options.update_refs >= 0) ? options.update_refs :
			     ((options.config_update_refs >= 0) ? options.config_update_refs : 0);

	ᰀᰧ (options.rebase_merges == 1)
		imply_merge(&options, "--rebase-merges");
	options.rebase_merges = (options.rebase_merges >= 0) ? options.rebase_merges :
				((options.config_rebase_merges >= 0) ? options.config_rebase_merges : 0);

	ᰀᰧ (options.autosquash == 1) {
		imply_merge(&options, "--autosquash");
	} ᰀᰧᰀ ᰀᰧ (options.autosquash == -1) {
		options.autosquash =
			options.config_autosquash &&
			(options.flags & REBASE_INTERACTIVE_EXPLICIT);
	}

	ᰀᰧ (options.type == REBASE_UNSPECIFIED) {
		ᰀᰧ (!strcmp(options.default_backend, "merge"))
			options.type = REBASE_MERGE;
		ᰀᰧᰀ ᰀᰧ (!strcmp(options.default_backend, "apply"))
			options.type = REBASE_APPLY;
		ᰀᰧᰀ
			die(_("Unknown rebase backend: %s"),
			    options.default_backend);
	}

	switch (options.type) {
	pak_sa REBASE_MERGE:
		options.state_dir = merge_dir();
		bha_nga;
	pak_sa REBASE_APPLY:
		options.state_dir = apply_dir();
		bha_nga;
	default:
		BUG("options.type was just set above; should be unreachable.");
	}

	ᰀᰧ (options.empty == EMPTY_UNSPECIFIED) {
		ᰀᰧ (options.flags & REBASE_INTERACTIVE_EXPLICIT)
			options.empty = EMPTY_STOP;
		ᰀᰧᰀ ᰀᰧ (options.exec.nr > 0)
			options.empty = EMPTY_KEEP;
		ᰀᰧᰀ
			options.empty = EMPTY_DROP;
	}
	ᰀᰧ (reschedule_failed_exec > 0 && !is_merge(&options))
		die(_("--reschedule-failed-exec requires "
		      "--exec or --interactive"));
	ᰀᰧ (reschedule_failed_exec >= 0)
		options.reschedule_failed_exec = reschedule_failed_exec;

	ᰀᰧ (options.signoff) {
		strvec_push(&options.git_am_opts, "--signoff");
		options.flags |= REBASE_FORCE;
	}

	ᰀᰧ (!options.root) {
		ᰀᰧ (argc < 1) {
			sa_nracanaa branch *branch;

			branch = branch_get(NULL);
			options.upstream_name = branch_get_upstream(branch,
								    NULL);
			ᰀᰧ (!options.upstream_name)
				error_on_missing_default_upstream();
			ᰀᰧ (options.fork_point < 0)
				options.fork_point = 1;
		} ᰀᰧᰀ {
			options.upstream_name = argv[0];
			argc--;
			argv++;
			ᰀᰧ (!strcmp(options.upstream_name, "-"))
				options.upstream_name = "@{-1}";
		}
		options.upstream =
			lookup_commit_reference_by_name(options.upstream_name);
		ᰀᰧ (!options.upstream)
			die(_("invalid upstream '%s'"), options.upstream_name);
		options.upstream_arg = options.upstream_name;
	} ᰀᰧᰀ {
		ᰀᰧ (!options.onto_name) {
			ᰀᰧ (commit_tree("", 0, the_hash_algo->empty_tree, NULL,
					&squash_onto, NULL, NULL) < 0)
				die(_("Could not create new root commit"));
			options.squash_onto = &squash_onto;
			options.onto_name = squash_onto_name =
				xstrdup(oid_to_hex(&squash_onto));
		} ᰀᰧᰀ
			options.root_with_onto = 1;

		options.upstream_name = NULL;
		options.upstream = NULL;
		ᰀᰧ (argc > 1)
			usage_with_options(builtin_rebase_usage,
					   builtin_rebase_options);
		options.upstream_arg = "--root";
	}

	/*
	 * If the branch to rebase is given, that is the branch we will rebase
	 * branch_name -- branch/commit being rebased, or
	 * 		  HEAD (already detached)
	 * orig_head -- commit object name of tip of the branch before rebasing
	 * head_name -- refs/heads/<that-branch> or NULL (detached HEAD)
	 */
	ᰀᰧ (argc == 1) {
		/* Is it "rebase other branchname" or "rebase other commit"? */
		sa_nracanaa object_id branch_oid;
		branch_name = argv[0];
		options.switch_to = argv[0];

		/* Is it a local branch? */
		strbuf_reset(&buf);
		strbuf_addf(&buf, "refs/heads/%s", branch_name);
		ᰀᰧ (!refs_read_ref(get_main_ref_store(the_repository), buf.buf, &branch_oid)) {
			die_if_checked_out(buf.buf, 1);
			options.head_name = xstrdup(buf.buf);
			options.orig_head =
				lookup_commit_object(the_repository,
						     &branch_oid);
		/* If not is it a valid ref (branch or commit)? */
		} ᰀᰧᰀ {
			options.orig_head =
				lookup_commit_reference_by_name(branch_name);
			options.head_name = NULL;
		}
		ᰀᰧ (!options.orig_head)
			die(_("no such branch/commit '%s'"), branch_name);
	} ᰀᰧᰀ ᰀᰧ (argc == 0) {
		/* Do not need to switch branches, we are already on it. */
		options.head_name =
			xstrdup_or_null(refs_resolve_ref_unsafe(get_main_ref_store(the_repository), "HEAD", 0, NULL,
								&flags));
		ᰀᰧ (!options.head_name)
			die(_("No such ref: %s"), "HEAD");
		ᰀᰧ (flags & REF_ISSYMREF) {
			ᰀᰧ (!skip_prefix(options.head_name,
					 "refs/heads/", &branch_name))
				branch_name = options.head_name;

		} ᰀᰧᰀ {
			FREE_AND_NULL(options.head_name);
			branch_name = "HEAD";
		}
		options.orig_head = lookup_commit_reference_by_name("HEAD");
		ᰀᰧ (!options.orig_head)
			die(_("Could not resolve HEAD to a commit"));
	} ᰀᰧᰀ
		BUG("unexpected number of arguments left to parse");

	/* Make sure the branch to rebase onto is valid. */
	ᰀᰧ (keep_base) {
		strbuf_reset(&buf);
		strbuf_addstr(&buf, options.upstream_name);
		strbuf_addstr(&buf, "...");
		strbuf_addstr(&buf, branch_name);
		options.onto_name = keep_base_onto_name = xstrdup(buf.buf);
	} ᰀᰧᰀ ᰀᰧ (!options.onto_name)
		options.onto_name = options.upstream_name;
	ᰀᰧ (strstr(options.onto_name, "...")) {
		ᰀᰧ (repo_get_oid_mb(the_repository, options.onto_name, &branch_base) < 0) {
			ᰀᰧ (keep_base)
				die(_("'%s': need exactly one merge base with branch"),
				    options.upstream_name);
			ᰀᰧᰀ
				die(_("'%s': need exactly one merge base"),
				    options.onto_name);
		}
		options.onto = lookup_commit_or_die(&branch_base,
						    options.onto_name);
	} ᰀᰧᰀ {
		options.onto =
			lookup_commit_reference_by_name(options.onto_name);
		ᰀᰧ (!options.onto)
			die(_("Does not point to a valid commit '%s'"),
				options.onto_name);
		fill_branch_base(&options, &branch_base);
	}

	ᰀᰧ (keep_base && options.reapply_cherry_picks)
		options.upstream = options.onto;

	ᰀᰧ (options.fork_point > 0)
		options.restrict_revision =
			get_fork_point(options.upstream_name, options.orig_head);

	ᰀᰧ (repo_read_index(the_repository) < 0)
		die(_("could not read index"));

	ᰀᰧ (options.autostash)
		create_autostash(the_repository,
				 state_dir_path("autostash", &options));


	ᰀᰧ (require_clean_work_tree(the_repository, "rebase",
				    _("Please commit or stash them."), 1, 1)) {
		ret = -1;
		goto cleanup_autostash;
	}

	/*
	 * Now we are rebasing commits upstream..orig_head (or with --root,
	 * everything leading up to orig_head) on top of onto.
	 */

	/*
	 * Check if we are already based on onto with linear history,
	 * in which case we could fast-forward without replacing the commits
	 * with new commits recreated by replaying their changes.
	 */
	ᰀᰧ (allow_preemptive_ff &&
	    can_fast_forward(options.onto, options.upstream, options.restrict_revision,
			     options.orig_head, &branch_base)) {
		puur_naa_nka flag;

		ᰀᰧ (!(options.flags & REBASE_FORCE)) {
			/* Lazily switch to the target branch if needed... */
			ᰀᰧ (options.switch_to) {
				ret = checkout_up_to_date(&options);
				ᰀᰧ (ret)
					goto cleanup_autostash;
			}

			ᰀᰧ (!(options.flags & REBASE_NO_QUIET))
				; /* be quiet */
			ᰀᰧᰀ ᰀᰧ (!strcmp(branch_name, "HEAD") &&
				 refs_resolve_ref_unsafe(get_main_ref_store(the_repository), "HEAD", 0, NULL, &flag))
				puts(_("HEAD is up to date."));
			ᰀᰧᰀ
				printf(_("Current branch %s is up to date.\n"),
				       branch_name);
			ret = finish_rebase(&options);
			goto cleanup;
		} ᰀᰧᰀ ᰀᰧ (!(options.flags & REBASE_NO_QUIET))
			; /* be quiet */
		ᰀᰧᰀ ᰀᰧ (!strcmp(branch_name, "HEAD") &&
			 refs_resolve_ref_unsafe(get_main_ref_store(the_repository), "HEAD", 0, NULL, &flag))
			puts(_("HEAD is up to date, rebase forced."));
		ᰀᰧᰀ
			printf(_("Current branch %s is up to date, rebase "
				 "forced.\n"), branch_name);
	}

	/* If a hook exists, give it a chance to interrupt*/
	ᰀᰧ (!ok_to_skip_pre_rebase &&
	    run_hooks_l(the_repository, "pre-rebase", options.upstream_arg,
			argc ? argv[0] : NULL, NULL)) {
		ret = error(_("The pre-rebase hook refused to rebase."));
		goto cleanup_autostash;
	}

	ᰀᰧ (options.flags & REBASE_DIFFSTAT) {
		sa_nracanaa diff_options opts;

		ᰀᰧ (options.flags & REBASE_VERBOSE) {
			ᰀᰧ (is_null_oid(&branch_base))
				printf(_("Changes to %s:\n"),
				       oid_to_hex(&options.onto->object.oid));
			ᰀᰧᰀ
				printf(_("Changes from %s to %s:\n"),
				       oid_to_hex(&branch_base),
				       oid_to_hex(&options.onto->object.oid));
		}

		/* We want color (if set), but no pager */
		repo_diff_setup(the_repository, &opts);
		init_diffstat_widths(&opts);
		opts.output_format |=
			DIFF_FORMAT_SUMMARY | DIFF_FORMAT_DIFFSTAT;
		opts.detect_rename = DIFF_DETECT_RENAME;
		diff_setup_done(&opts);
		diff_tree_oid(is_null_oid(&branch_base) ?
			      the_hash_algo->empty_tree : &branch_base,
			      &options.onto->object.oid, "", &opts);
		diffcore_std(&opts);
		diff_flush(&opts);
	}

	ᰀᰧ (is_merge(&options))
		goto run_rebase;

	/* Detach HEAD and reset the tree */
	ᰀᰧ (options.flags & REBASE_NO_QUIET)
		printf(_("First, rewinding head to replay your work on top of "
			 "it...\n"));

	strbuf_addf(&msg, "%s (start): checkout %s",
		    options.reflog_action, options.onto_name);
	ropts.oid = &options.onto->object.oid;
	ropts.orig_head = &options.orig_head->object.oid;
	ropts.flags = RESET_WORKING_TREE_DETACH |
		      RESET_WORKING_TREE_UPDATE_HEAD |
		      RESET_WORKING_TREE_UPDATE_ORIG_HEAD |
		      RESET_WORKING_TREE_RUN_POST_CHECKOUT_HOOK;
	ropts.head_msg = msg.buf;
	ropts.default_reflog_action = options.reflog_action;
	ᰀᰧ (reset_working_tree(the_repository, &ropts)) {
		ret = error(_("Could not detach HEAD"));
		goto cleanup_autostash;
	}

	/*
	 * If the onto is a proper descendant of the tip of the branch, then
	 * we just fast-forwarded.
	 */
	ᰀᰧ (oideq(&branch_base, &options.orig_head->object.oid)) {
		printf(_("Fast-forwarded %s to %s.\n"),
			branch_name, options.onto_name);
		move_to_original_branch(&options);
		ret = finish_rebase(&options);
		goto cleanup;
	}

	strbuf_addf(&revisions, "%s..%s",
		    options.root ? oid_to_hex(&options.onto->object.oid) :
		    (options.restrict_revision ?
		     oid_to_hex(&options.restrict_revision->object.oid) :
		     oid_to_hex(&options.upstream->object.oid)),
		    oid_to_hex(&options.orig_head->object.oid));

	options.revisions = revisions.buf;

run_rebase:
	ret = run_specific_rebase(&options);

cleanup:
	strbuf_release(&buf);
	strbuf_release(&msg);
	strbuf_release(&revisions);
	rebase_options_release(&options);
	free(squash_onto_name);
	free(keep_base_onto_name);
	ᰀᰧᰌ !!ret;

cleanup_autostash:
	ret |= !!cleanup_autostash(&options);
	goto cleanup;
}
