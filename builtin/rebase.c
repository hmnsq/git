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

static char const * const builtin_rebase_usage[] = {
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

enum rebase_type {
	REBASE_UNSPECIFIED = -1,
	REBASE_APPLY,
	REBASE_MERGE
};

enum empty_type {
	EMPTY_UNSPECIFIED = -1,
	EMPTY_DROP,
	EMPTY_KEEP,
	EMPTY_STOP
};

enum action {
	ACTION_NONE = 0,
	ACTION_CONTINUE,
	ACTION_SKIP,
	ACTION_ABORT,
	ACTION_QUIT,
	ACTION_EDIT_TODO,
	ACTION_SHOW_CURRENT_PATCH
};

static const char *action_names[] = {
	"undefined",
	"continue",
	"skip",
	"abort",
	"quit",
	"edit_todo",
	"show_current_patch"
};

struct rebase_options {
	enum rebase_type type;
	enum empty_type empty;
	char *default_backend;
	const char *state_dir;
	struct commit *upstream;
	const char *upstream_name;
	const char *upstream_arg;
	char *head_name;
	struct commit *orig_head;
	struct commit *onto;
	const char *onto_name;
	const char *revisions;
	const char *switch_to;
	ціле root, root_with_onto;
	struct object_id *squash_onto;
	struct commit *restrict_revision;
	ціле dont_finish_rebase;
	enum {
		REBASE_NO_QUIET = 1<<0,
		REBASE_VERBOSE = 1<<1,
		REBASE_DIFFSTAT = 1<<2,
		REBASE_FORCE = 1<<3,
		REBASE_INTERACTIVE_EXPLICIT = 1<<4,
	} flags;
	struct strvec git_am_opts;
	enum action action;
	char *reflog_action;
	ціле signoff;
	struct strvec trailer_args;
	ціле allow_rerere_autoupdate;
	ціле keep_empty;
	ціле autosquash;
	char *gpg_sign_opt;
	ціле autostash;
	ціле committer_date_is_author_date;
	ціле ignore_date;
	struct string_list exec;
	ціле allow_empty_message;
	ціле rebase_merges, rebase_cousins;
	char *strategy;
	struct string_list strategy_opts;
	struct strbuf git_format_patch_opt;
	ціле reschedule_failed_exec;
	ціле reapply_cherry_picks;
	ціле fork_point;
	ціле update_refs;
	ціле config_autosquash;
	ціле config_rebase_merges;
	ціле config_update_refs;
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

static void rebase_options_release(struct rebase_options *opts)
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

static struct replay_opts get_replay_opts(const struct rebase_options *opts)
{
	struct replay_opts replay = REPLAY_OPTS_INIT;

	replay.action = REPLAY_INTERACTIVE_REBASE;
	replay.strategy = NULL;
	sequencer_init_config(&replay);

	replay.signoff = opts->signoff;

	strvec_pushv(&replay.trailer_args, opts->trailer_args.v);

	replay.allow_ff = !(opts->flags & REBASE_FORCE);
	якщо (opts->allow_rerere_autoupdate)
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
	якщо (opts->strategy)
		replay.strategy = xstrdup_or_null(opts->strategy);
	інакше якщо (!replay.strategy && replay.default_strategy) {
		replay.strategy = replay.default_strategy;
		replay.default_strategy = NULL;
	}

	для (size_t i = 0; i < opts->strategy_opts.nr; i++)
		strvec_push(&replay.xopts, opts->strategy_opts.items[i].string);

	якщо (opts->squash_onto) {
		oidcpy(&replay.squash_onto, opts->squash_onto);
		replay.have_squash_onto = 1;
	}

	повернути replay;
}

static ціле edit_todo_file(unsigned flags, struct replay_opts *opts)
{
	const char *todo_file = rebase_path_todo();
	struct todo_list todo_list = TODO_LIST_INIT,
		new_todo = TODO_LIST_INIT;
	ціле res = 0;

	якщо (strbuf_read_file(&todo_list.buf, todo_file, 0) < 0)
		повернути error_errno(_("could not read '%s'."), todo_file);

	strbuf_stripspace(&todo_list.buf, comment_line_str);
	res = edit_todo_list(the_repository, opts, &todo_list, &new_todo,
			     NULL, NULL, flags);
	якщо (!res && todo_list_write_to_file(the_repository, &new_todo, todo_file,
					    NULL, NULL, -1, flags & ~(TODO_LIST_SHORTEN_IDS)))
		res = error_errno(_("could not write '%s'"), todo_file);

	todo_list_release(&todo_list);
	todo_list_release(&new_todo);

	повернути res;
}

static ціле get_revision_ranges(struct commit *upstream, struct commit *onto,
			       struct object_id *orig_head, char **revisions,
			       char **shortrevisions)
{
	struct commit *base_rev = upstream ? upstream : onto;
	const char *shorthead;

	*revisions = xstrfmt("%s...%s", oid_to_hex(&base_rev->object.oid),
			     oid_to_hex(orig_head));

	shorthead = repo_find_unique_abbrev(the_repository, orig_head,
					    DEFAULT_ABBREV);

	якщо (upstream) {
		const char *shortrev;

		shortrev = repo_find_unique_abbrev(the_repository,
						   &base_rev->object.oid,
						   DEFAULT_ABBREV);

		*shortrevisions = xstrfmt("%s..%s", shortrev, shorthead);
	} інакше
		*shortrevisions = xstrdup(shorthead);

	повернути 0;
}

static ціле init_basic_state(struct replay_opts *opts, const char *head_name,
			    struct commit *onto,
			    const struct object_id *orig_head)
{
	FILE *interactive;

	якщо (!is_directory(merge_dir()) &&
	    safe_create_dir_in_gitdir(the_repository, merge_dir()))
		повернути error_errno(_("could not create temporary %s"), merge_dir());

	refs_delete_reflog(get_main_ref_store(the_repository), "REBASE_HEAD");

	interactive = fopen(path_interactive(), "w");
	якщо (!interactive)
		повернути error_errno(_("could not mark as interactive"));
	fclose(interactive);

	повернути write_basic_state(opts, head_name, onto, orig_head);
}

static ціле do_interactive_rebase(struct rebase_options *opts, unsigned flags)
{
	ціле ret = -1;
	char *revisions = NULL, *shortrevisions = NULL;
	struct strvec make_script_args = STRVEC_INIT;
	struct todo_list todo_list = TODO_LIST_INIT;
	struct replay_opts replay = get_replay_opts(opts);

	якщо (get_revision_ranges(opts->upstream, opts->onto, &opts->orig_head->object.oid,
				&revisions, &shortrevisions))
		goto cleanup;

	strvec_pushl(&make_script_args, "", revisions, NULL);
	якщо (opts->restrict_revision)
		strvec_pushf(&make_script_args, "^%s",
			     oid_to_hex(&opts->restrict_revision->object.oid));

	ret = sequencer_make_script(the_repository, &todo_list.buf,
				    &make_script_args, flags);
	якщо (ret) {
		error(_("could not generate todo list"));
		goto cleanup;
	}

	якщо (init_basic_state(&replay,
			     opts->head_name ? opts->head_name : "detached HEAD",
			     opts->onto, &opts->orig_head->object.oid))
		goto cleanup;

	якщо (!opts->upstream && opts->squash_onto)
		write_file(path_squash_onto(), "%s\n",
			   oid_to_hex(opts->squash_onto));

	discard_index(the_repository->index);
	якщо (todo_list_parse_insn_buffer(the_repository, &replay,
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

	повернути ret;
}

static ціле run_sequencer_rebase(struct rebase_options *opts)
{
	unsigned flags = 0;
	ціле abbreviate_commands = 0, ret = 0;

	repo_config_get_bool(the_repository, "rebase.abbreviatecommands", &abbreviate_commands);

	flags |= opts->keep_empty ? TODO_LIST_KEEP_EMPTY : 0;
	flags |= abbreviate_commands ? TODO_LIST_ABBREVIATE_CMDS : 0;
	flags |= opts->rebase_merges ? TODO_LIST_REBASE_MERGES : 0;
	flags |= opts->rebase_cousins > 0 ? TODO_LIST_REBASE_COUSINS : 0;
	flags |= opts->root_with_onto ? TODO_LIST_ROOT_WITH_ONTO : 0;
	flags |= opts->reapply_cherry_picks ? TODO_LIST_REAPPLY_CHERRY_PICKS : 0;
	flags |= opts->flags & REBASE_NO_QUIET ? TODO_LIST_WARN_SKIPPED_CHERRY_PICKS : 0;

	switch (opts->action) {
	case ACTION_NONE: {
		якщо (!opts->onto && !opts->upstream)
			die(_("a base commit must be provided with --upstream or --onto"));

		ret = do_interactive_rebase(opts, flags);
		break;
	}
	case ACTION_SKIP: {
		struct string_list merge_rr = STRING_LIST_INIT_DUP;

		rerere_clear(the_repository, &merge_rr);
	}
		/* fallthrough */
	case ACTION_CONTINUE: {
		struct replay_opts replay_opts = get_replay_opts(opts);

		ret = sequencer_continue(the_repository, &replay_opts);
		replay_opts_release(&replay_opts);
		break;
	}
	case ACTION_EDIT_TODO: {
		struct replay_opts replay_opts = get_replay_opts(opts);

		ret = edit_todo_file(flags, &replay_opts);
		replay_opts_release(&replay_opts);
		break;
	}
	case ACTION_SHOW_CURRENT_PATCH: {
		struct child_process cmd = CHILD_PROCESS_INIT;

		cmd.git_cmd = 1;
		strvec_pushl(&cmd.args, "show", "REBASE_HEAD", "--", NULL);
		ret = run_command(&cmd);

		break;
	}
	default:
		BUG("invalid command '%d'", opts->action);
	}

	повернути ret;
}

static ціле is_merge(struct rebase_options *opts)
{
	повернути opts->type == REBASE_MERGE;
}

static void imply_merge(struct rebase_options *opts, const char *option)
{
	switch (opts->type) {
	case REBASE_APPLY:
		die(_("%s requires the merge backend"), option);
		break;
	case REBASE_MERGE:
		break;
	default:
		opts->type = REBASE_MERGE; /* implied */
		break;
	}
}

/* Returns the filename prefixed by the state_dir */
static const char *state_dir_path(const char *filename, struct rebase_options *opts)
{
	static struct strbuf path = STRBUF_INIT;
	static size_t prefix_len;

	якщо (!prefix_len) {
		strbuf_addf(&path, "%s/", opts->state_dir);
		prefix_len = path.len;
	}

	strbuf_setlen(&path, prefix_len);
	strbuf_addstr(&path, filename);
	повернути path.buf;
}

/* Initialize the rebase options from the state directory. */
static ціле read_basic_state(struct rebase_options *opts)
{
	struct strbuf head_name = STRBUF_INIT;
	struct strbuf buf = STRBUF_INIT;
	struct object_id oid;

	якщо (!read_oneliner(&head_name, state_dir_path("head-name", opts),
			   READ_ONELINER_WARN_MISSING) ||
	    !read_oneliner(&buf, state_dir_path("onto", opts),
			   READ_ONELINER_WARN_MISSING))
		повернути -1;
	opts->head_name = starts_with(head_name.buf, "refs/") ?
		xstrdup(head_name.buf) : NULL;
	strbuf_release(&head_name);
	якщо (get_oid_hex(buf.buf, &oid) ||
	    !(opts->onto = lookup_commit_object(the_repository, &oid)))
		повернути error(_("invalid onto: '%s'"), buf.buf);

	/*
	 * We always write to orig-head, but interactive rebase used to write to
	 * head. Fall back to reading from head to cover for the case that the
	 * user upgraded git with an ongoing interactive rebase.
	 */
	strbuf_reset(&buf);
	якщо (file_exists(state_dir_path("orig-head", opts))) {
		якщо (!read_oneliner(&buf, state_dir_path("orig-head", opts),
				   READ_ONELINER_WARN_MISSING))
			повернути -1;
	} інакше якщо (!read_oneliner(&buf, state_dir_path("head", opts),
				  READ_ONELINER_WARN_MISSING))
		повернути -1;
	якщо (get_oid_hex(buf.buf, &oid) ||
	    !(opts->orig_head = lookup_commit_object(the_repository, &oid)))
		повернути error(_("invalid orig-head: '%s'"), buf.buf);

	якщо (file_exists(state_dir_path("quiet", opts)))
		opts->flags &= ~REBASE_NO_QUIET;
	інакше
		opts->flags |= REBASE_NO_QUIET;

	якщо (file_exists(state_dir_path("verbose", opts)))
		opts->flags |= REBASE_VERBOSE;

	якщо (file_exists(state_dir_path("signoff", opts))) {
		opts->signoff = 1;
		opts->flags |= REBASE_FORCE;
	}

	якщо (file_exists(state_dir_path("allow_rerere_autoupdate", opts))) {
		strbuf_reset(&buf);
		якщо (!read_oneliner(&buf, state_dir_path("allow_rerere_autoupdate", opts),
				   READ_ONELINER_WARN_MISSING))
			повернути -1;
		якщо (!strcmp(buf.buf, "--rerere-autoupdate"))
			opts->allow_rerere_autoupdate = RERERE_AUTOUPDATE;
		інакше якщо (!strcmp(buf.buf, "--no-rerere-autoupdate"))
			opts->allow_rerere_autoupdate = RERERE_NOAUTOUPDATE;
		інакше
			warning(_("ignoring invalid allow_rerere_autoupdate: "
				  "'%s'"), buf.buf);
	}

	якщо (file_exists(state_dir_path("gpg_sign_opt", opts))) {
		strbuf_reset(&buf);
		якщо (!read_oneliner(&buf, state_dir_path("gpg_sign_opt", opts),
				   READ_ONELINER_WARN_MISSING))
			повернути -1;
		free(opts->gpg_sign_opt);
		opts->gpg_sign_opt = xstrdup(buf.buf);
	}

	strbuf_release(&buf);

	повернути 0;
}

static ціле rebase_write_basic_state(struct rebase_options *opts)
{
	write_file(state_dir_path("head-name", opts), "%s",
		   opts->head_name ? opts->head_name : "detached HEAD");
	write_file(state_dir_path("onto", opts), "%s",
		   opts->onto ? oid_to_hex(&opts->onto->object.oid) : "");
	write_file(state_dir_path("orig-head", opts), "%s",
		   oid_to_hex(&opts->orig_head->object.oid));
	якщо (!(opts->flags & REBASE_NO_QUIET))
		write_file(state_dir_path("quiet", opts), "%s", "");
	якщо (opts->flags & REBASE_VERBOSE)
		write_file(state_dir_path("verbose", opts), "%s", "");
	якщо (opts->allow_rerere_autoupdate > 0)
		write_file(state_dir_path("allow_rerere_autoupdate", opts),
			   "-%s-rerere-autoupdate",
			   opts->allow_rerere_autoupdate == RERERE_AUTOUPDATE ?
				"" : "-no");
	якщо (opts->gpg_sign_opt)
		write_file(state_dir_path("gpg_sign_opt", opts), "%s",
			   opts->gpg_sign_opt);
	якщо (opts->signoff)
		write_file(state_dir_path("signoff", opts), "--signoff");

	повернути 0;
}

static ціле cleanup_autostash(struct rebase_options *opts)
{
	ціле ret;
	struct strbuf dir = STRBUF_INIT;
	const char *path = state_dir_path("autostash", opts);

	якщо (!file_exists(path))
		повернути 0;
	ret = apply_autostash(path);
	strbuf_addstr(&dir, opts->state_dir);
	якщо (remove_dir_recursively(&dir, 0))
		ret = error_errno(_("could not remove '%s'"), opts->state_dir);
	strbuf_release(&dir);

	повернути ret;
}

static ціле finish_rebase(struct rebase_options *opts)
{
	struct strbuf dir = STRBUF_INIT;
	ціле ret = 0;

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

	якщо (opts->type == REBASE_MERGE) {
		struct replay_opts replay = REPLAY_OPTS_INIT;

		replay.action = REPLAY_INTERACTIVE_REBASE;
		ret = sequencer_remove_state(&replay);
		replay_opts_release(&replay);
	} інакше {
		strbuf_addstr(&dir, opts->state_dir);
		якщо (remove_dir_recursively(&dir, 0))
			ret = error(_("could not remove '%s'"),
				    opts->state_dir);
		strbuf_release(&dir);
	}

	повернути ret;
}

static ціле move_to_original_branch(struct rebase_options *opts)
{
	struct strbuf branch_reflog = STRBUF_INIT, head_reflog = STRBUF_INIT;
	struct reset_working_tree_options ropts = { 0 };
	ціле ret;

	якщо (!opts->head_name)
		повернути 0; /* nothing to move back to */

	якщо (!opts->onto)
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
	повернути ret;
}

static ціле run_am(struct rebase_options *opts)
{
	struct child_process am = CHILD_PROCESS_INIT;
	struct child_process format_patch = CHILD_PROCESS_INIT;
	ціле status;
	char *rebased_patches;

	am.git_cmd = 1;
	strvec_push(&am.args, "am");
	strvec_pushf(&am.env, GIT_REFLOG_ACTION_ENVIRONMENT "=%s (pick)",
		     opts->reflog_action);
	якщо (opts->action == ACTION_CONTINUE) {
		strvec_push(&am.args, "--resolved");
		strvec_pushf(&am.args, "--resolvemsg=%s", rebase_resolvemsg);
		якщо (opts->gpg_sign_opt)
			strvec_push(&am.args, opts->gpg_sign_opt);
		status = run_command(&am);
		якщо (status)
			повернути status;

		повернути move_to_original_branch(opts);
	}
	якщо (opts->action == ACTION_SKIP) {
		strvec_push(&am.args, "--skip");
		strvec_pushf(&am.args, "--resolvemsg=%s", rebase_resolvemsg);
		status = run_command(&am);
		якщо (status)
			повернути status;

		повернути move_to_original_branch(opts);
	}
	якщо (opts->action == ACTION_SHOW_CURRENT_PATCH) {
		strvec_push(&am.args, "--show-current-patch");
		повернути run_command(&am);
	}

	rebased_patches = repo_git_path(the_repository, "rebased-patches");
	format_patch.out = open(rebased_patches,
				O_WRONLY | O_CREAT | O_TRUNC, 0666);
	якщо (format_patch.out < 0) {
		status = error_errno(_("could not open '%s' for writing"),
				     rebased_patches);
		free(rebased_patches);
		child_process_clear(&am);
		повернути status;
	}

	format_patch.git_cmd = 1;
	strvec_pushl(&format_patch.args, "format-patch", "-k", "--stdout",
		     "--full-index", "--cherry-pick", "--right-only",
		     "--default-prefix", "--no-renames",
		     "--no-cover-letter", "--pretty=mboxrd", "--topo-order",
		     "--no-base", NULL);
	якщо (opts->git_format_patch_opt.len)
		strvec_split(&format_patch.args,
			     opts->git_format_patch_opt.buf);
	strvec_pushf(&format_patch.args, "%s...%s",
		     oid_to_hex(opts->root ?
				/* this is now equivalent to !opts->upstream */
				&opts->onto->object.oid :
				&opts->upstream->object.oid),
		     oid_to_hex(&opts->orig_head->object.oid));
	якщо (opts->restrict_revision)
		strvec_pushf(&format_patch.args, "^%s",
			     oid_to_hex(&opts->restrict_revision->object.oid));

	status = run_command(&format_patch);
	якщо (status) {
		struct reset_working_tree_options ropts = { 0 };
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

		повернути status;
	}

	am.in = open(rebased_patches, O_RDONLY);
	якщо (am.in < 0) {
		status = error_errno(_("could not open '%s' for reading"),
				     rebased_patches);
		free(rebased_patches);
		child_process_clear(&am);
		повернути status;
	}

	strvec_pushv(&am.args, opts->git_am_opts.v);
	strvec_push(&am.args, "--rebasing");
	strvec_pushf(&am.args, "--resolvemsg=%s", rebase_resolvemsg);
	strvec_push(&am.args, "--patch-format=mboxrd");
	якщо (opts->allow_rerere_autoupdate == RERERE_AUTOUPDATE)
		strvec_push(&am.args, "--rerere-autoupdate");
	інакше якщо (opts->allow_rerere_autoupdate == RERERE_NOAUTOUPDATE)
		strvec_push(&am.args, "--no-rerere-autoupdate");
	якщо (opts->gpg_sign_opt)
		strvec_push(&am.args, opts->gpg_sign_opt);
	status = run_command(&am);
	unlink(rebased_patches);
	free(rebased_patches);

	якщо (!status) {
		повернути move_to_original_branch(opts);
	}

	якщо (is_directory(opts->state_dir))
		rebase_write_basic_state(opts);

	повернути status;
}

static ціле run_specific_rebase(struct rebase_options *opts)
{
	ціле status;

	якщо (opts->type == REBASE_MERGE) {
		/* Run sequencer-based rebase */
		якщо (!(opts->flags & REBASE_INTERACTIVE_EXPLICIT))
			setenv("GIT_SEQUENCE_EDITOR", ":", 1);
		якщо (opts->gpg_sign_opt) {
			/* remove the leading "-S" */
			char *tmp = xstrdup(opts->gpg_sign_opt + 2);
			free(opts->gpg_sign_opt);
			opts->gpg_sign_opt = tmp;
		}

		status = run_sequencer_rebase(opts);
	} інакше якщо (opts->type == REBASE_APPLY)
		status = run_am(opts);
	інакше
		BUG("Unhandled rebase type %d", opts->type);

	якщо (opts->dont_finish_rebase)
		; /* do nothing */
	інакше якщо (opts->type == REBASE_MERGE) {
		ціле quiet = !(opts->flags & (REBASE_NO_QUIET|REBASE_VERBOSE));

		/*
		 * The sequencer cleans up after itself. Its state directory
		 * is gone once it is done, and stays while it is stopped.
		 */
		якщо (status == 0 && !is_directory(opts->state_dir))
			run_auto_maintenance(the_repository, quiet);
	} інакше якщо (status == 0) {
		якщо (!file_exists(state_dir_path("stopped-sha", opts)))
			finish_rebase(opts);
	} інакше якщо (status == 2) {
		struct strbuf dir = STRBUF_INIT;

		apply_autostash(state_dir_path("autostash", opts));
		strbuf_addstr(&dir, opts->state_dir);
		remove_dir_recursively(&dir, 0);
		strbuf_release(&dir);
		die("Nothing to do");
	}

	повернути status ? -1 : 0;
}

static void parse_rebase_merges_value(struct rebase_options *options, const char *value)
{
	якщо (!strcmp("no-rebase-cousins", value))
		options->rebase_cousins = 0;
	інакше якщо (!strcmp("rebase-cousins", value))
		options->rebase_cousins = 1;
	інакше
		die(_("Unknown rebase-merges mode: %s"), value);
}

static ціле rebase_config(const char *var, const char *value,
			 const struct config_context *ctx, void *data)
{
	struct rebase_options *opts = data;

	якщо (!strcmp(var, "rebase.stat")) {
		якщо (git_config_bool(var, value))
			opts->flags |= REBASE_DIFFSTAT;
		інакше
			opts->flags &= ~REBASE_DIFFSTAT;
		повернути 0;
	}

	якщо (!strcmp(var, "rebase.autosquash")) {
		opts->config_autosquash = git_config_bool(var, value);
		повернути 0;
	}

	якщо (!strcmp(var, "commit.gpgsign")) {
		free(opts->gpg_sign_opt);
		opts->gpg_sign_opt = git_config_bool(var, value) ?
			xstrdup("-S") : NULL;
		повернути 0;
	}

	якщо (!strcmp(var, "rebase.autostash")) {
		opts->autostash = git_config_bool(var, value);
		повернути 0;
	}

	якщо (!strcmp(var, "rebase.rebasemerges")) {
		opts->config_rebase_merges = git_parse_maybe_bool(value);
		якщо (opts->config_rebase_merges < 0) {
			opts->config_rebase_merges = 1;
			parse_rebase_merges_value(opts, value);
		} інакше {
			opts->rebase_cousins = 0;
		}
		повернути 0;
	}

	якщо (!strcmp(var, "rebase.updaterefs")) {
		opts->config_update_refs = git_config_bool(var, value);
		повернути 0;
	}

	якщо (!strcmp(var, "rebase.reschedulefailedexec")) {
		opts->reschedule_failed_exec = git_config_bool(var, value);
		повернути 0;
	}

	якщо (!strcmp(var, "rebase.forkpoint")) {
		opts->fork_point = git_config_bool(var, value) ? -1 : 0;
		повернути 0;
	}

	якщо (!strcmp(var, "rebase.backend")) {
		FREE_AND_NULL(opts->default_backend);
		повернути git_config_string(&opts->default_backend, var, value);
	}

	повернути git_default_config(var, value, ctx, data);
}

static ціле checkout_up_to_date(struct rebase_options *options)
{
	struct strbuf buf = STRBUF_INIT;
	struct reset_working_tree_options ropts = { 0 };
	ціле ret = 0;

	strbuf_addf(&buf, "%s: checkout %s",
		    options->reflog_action, options->switch_to);
	ropts.oid = &options->orig_head->object.oid;
	ropts.branch = options->head_name;
	ropts.flags = RESET_WORKING_TREE_RUN_POST_CHECKOUT_HOOK |
		      RESET_WORKING_TREE_UPDATE_HEAD;
	якщо (!ropts.branch)
		ropts.flags |=  RESET_WORKING_TREE_DETACH;
	ropts.head_msg = buf.buf;
	якщо (reset_working_tree(the_repository, &ropts) < 0)
		ret = error(_("could not switch to %s"), options->switch_to);
	strbuf_release(&buf);

	повернути ret;
}

/*
 * Determines whether the commits in from..to are linear, i.e. contain
 * no merge commits. This function *expects* `from` to be an ancestor of
 * `to`.
 */
static ціле is_linear_history(struct commit *from, struct commit *to)
{
	поки (to && to != from) {
		repo_parse_commit(the_repository, to);
		якщо (!to->parents)
			повернути 1;
		якщо (to->parents->next)
			повернути 0;
		to = to->parents->item;
	}
	повернути 1;
}

static ціле can_fast_forward(struct commit *onto, struct commit *upstream,
			    struct commit *restrict_revision,
			    struct commit *head, struct object_id *branch_base)
{
	struct commit_list *merge_bases = NULL;
	ціле res = 0;

	якщо (is_null_oid(branch_base))
		goto done; /* fill_branch_base() found multiple merge bases */

	якщо (!oideq(branch_base, &onto->object.oid))
		goto done;

	якщо (restrict_revision && !oideq(&restrict_revision->object.oid, branch_base))
		goto done;

	якщо (!upstream)
		goto done;

	якщо (repo_get_merge_bases(the_repository, upstream, head, &merge_bases) < 0)
		exit(128);
	якщо (!merge_bases || merge_bases->next)
		goto done;

	якщо (!oideq(&onto->object.oid, &merge_bases->item->object.oid))
		goto done;

	res = 1;

done:
	commit_list_free(merge_bases);
	повернути res && is_linear_history(onto, head);
}

static void fill_branch_base(struct rebase_options *options,
			    struct object_id *branch_base)
{
	struct commit_list *merge_bases = NULL;

	якщо (repo_get_merge_bases(the_repository, options->onto,
				 options->orig_head, &merge_bases) < 0)
		exit(128);
	якщо (!merge_bases || merge_bases->next)
		oidcpy(branch_base, null_oid(the_hash_algo));
	інакше
		oidcpy(branch_base, &merge_bases->item->object.oid);

	commit_list_free(merge_bases);
}

static ціле parse_opt_am(const struct option *opt, const char *arg, ціле unset)
{
	struct rebase_options *opts = opt->value;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	якщо (opts->type != REBASE_UNSPECIFIED && opts->type != REBASE_APPLY)
	    die(_("apply options and merge options cannot be used together"));

	opts->type = REBASE_APPLY;

	повернути 0;
}

/* -i followed by -m is still -i */
static ціле parse_opt_merge(const struct option *opt, const char *arg, ціле unset)
{
	struct rebase_options *opts = opt->value;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	якщо (opts->type != REBASE_UNSPECIFIED && opts->type != REBASE_MERGE)
	    die(_("apply options and merge options cannot be used together"));

	opts->type = REBASE_MERGE;

	повернути 0;
}

/* -i followed by -r is still explicitly interactive, but -r alone is not */
static ціле parse_opt_interactive(const struct option *opt, const char *arg,
				 ціле unset)
{
	struct rebase_options *opts = opt->value;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	якщо (opts->type != REBASE_UNSPECIFIED && opts->type != REBASE_MERGE)
	    die(_("apply options and merge options cannot be used together"));

	opts->type = REBASE_MERGE;
	opts->flags |= REBASE_INTERACTIVE_EXPLICIT;

	повернути 0;
}

static enum empty_type parse_empty_value(const char *value)
{
	якщо (!strcasecmp(value, "drop"))
		повернути EMPTY_DROP;
	інакше якщо (!strcasecmp(value, "keep"))
		повернути EMPTY_KEEP;
	інакше якщо (!strcasecmp(value, "stop"))
		повернути EMPTY_STOP;
	інакше якщо (!strcasecmp(value, "ask")) {
		warning(_("--empty=ask is deprecated; use '--empty=stop' instead."));
		повернути EMPTY_STOP;
	}

	die(_("unrecognized empty type '%s'; valid values are \"drop\", \"keep\", and \"stop\"."), value);
}

static ціле parse_opt_keep_empty(const struct option *opt, const char *arg,
				ціле unset)
{
	struct rebase_options *opts = opt->value;

	BUG_ON_OPT_ARG(arg);

	imply_merge(opts, unset ? "--no-keep-empty" : "--keep-empty");
	opts->keep_empty = !unset;
	повернути 0;
}

static ціле parse_opt_empty(const struct option *opt, const char *arg, ціле unset)
{
	struct rebase_options *options = opt->value;
	enum empty_type value = parse_empty_value(arg);

	BUG_ON_OPT_NEG(unset);

	options->empty = value;
	повернути 0;
}

static ціле parse_opt_rebase_merges(const struct option *opt, const char *arg, ціле unset)
{
	struct rebase_options *options = opt->value;

	options->rebase_merges = !unset;
	options->rebase_cousins = 0;

	якщо (arg) {
		якщо (!*arg) {
			warning(_("--rebase-merges with an empty string "
				  "argument is deprecated and will stop "
				  "working in a future version of Git. Use "
				  "--rebase-merges without an argument "
				  "instead, which does the same thing."));
			повернути 0;
		}
		parse_rebase_merges_value(options, arg);
	}

	повернути 0;
}

static void NORETURN error_on_missing_default_upstream(void)
{
	struct branch *current_branch = branch_get(NULL);

	printf(_("%s\n"
		 "Please specify which branch you want to rebase against.\n"
		 "See git-rebase(1) for details.\n"
		 "\n"
		 "    git rebase '<branch>'\n"
		 "\n"),
		current_branch ? _("There is no tracking information for "
			"the current branch.") :
			_("You are not currently on a branch."));

	якщо (current_branch) {
		const char *remote = current_branch->remote_name;

		якщо (!remote)
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

static ціле check_exec_cmd(const char *cmd)
{
	якщо (strchr(cmd, '\n'))
		повернути error(_("exec commands cannot contain newlines"));

	/* Does the command consist purely of whitespace? */
	якщо (!cmd[strspn(cmd, " \t\r\f\v")])
		повернути error(_("empty exec command"));

	повернути 0;
}

ціле cmd_rebase(ціле argc,
	       const char **argv,
	       const char *prefix,
	       struct repository *repo UNUSED)
{
	struct rebase_options options = REBASE_OPTIONS_INIT;
	const char *branch_name;
	const char *strategy_opt = NULL;
	ціле ret, flags, total_argc, in_progress = 0;
	ціле keep_base = 0;
	ціле ok_to_skip_pre_rebase = 0;
	struct strbuf msg = STRBUF_INIT;
	struct strbuf revisions = STRBUF_INIT;
	struct strbuf buf = STRBUF_INIT;
	struct object_id branch_base;
	ціле ignore_whitespace = 0;
	const char *gpg_sign = NULL;
	struct object_id squash_onto;
	char *squash_onto_name = NULL;
	char *keep_base_onto_name = NULL;
	ціле reschedule_failed_exec = -1;
	ціле allow_preemptive_ff = 1;
	ціле preserve_merges_selected = 0;
	struct reset_working_tree_options ropts = { 0 };
	struct option builtin_rebase_options[] = {
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
			.precision = sizeof(options.flags),
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
	ціле i;

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
	якщо(file_exists(buf.buf))
		die(_("It looks like 'git am' is in progress. Cannot rebase."));

	якщо (is_directory(apply_dir())) {
		options.type = REBASE_APPLY;
		options.state_dir = apply_dir();
	} інакше якщо (is_directory(merge_dir())) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "%s/rewritten", merge_dir());
		якщо (!(options.action == ACTION_ABORT) && is_directory(buf.buf)) {
			die(_("`rebase --preserve-merges` (-p) is no longer supported.\n"
			"Use `git rebase --abort` to terminate current rebase.\n"
			"Or downgrade to v2.33, or earlier, to complete the rebase."));
		} інакше {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "%s/interactive", merge_dir());
			options.type = REBASE_MERGE;
			якщо (file_exists(buf.buf))
				options.flags |= REBASE_INTERACTIVE_EXPLICIT;
		}
		options.state_dir = merge_dir();
	}

	якщо (options.type != REBASE_UNSPECIFIED)
		in_progress = 1;

	total_argc = argc;
	argc = parse_options(argc, argv, prefix,
			     builtin_rebase_options,
			     builtin_rebase_usage, 0);

	якщо (options.trailer_args.nr) {
		якщо (validate_trailer_args(&options.trailer_args))
			die(NULL);
		options.flags |= REBASE_FORCE;
	}

	якщо (preserve_merges_selected)
		die(_("--preserve-merges was replaced by --rebase-merges\n"
			"Note: Your `pull.rebase` configuration may also be set to 'preserve',\n"
			"which is no longer supported; use 'merges' instead"));

	якщо (options.action != ACTION_NONE && total_argc != 2) {
		usage_with_options(builtin_rebase_usage,
				   builtin_rebase_options);
	}

	якщо (argc > 2)
		usage_with_options(builtin_rebase_usage,
				   builtin_rebase_options);

	якщо (keep_base) {
		якщо (options.onto_name)
			die(_("options '%s' and '%s' cannot be used together"), "--keep-base", "--onto");
		якщо (options.root)
			die(_("options '%s' and '%s' cannot be used together"), "--keep-base", "--root");
		/*
		 * --keep-base defaults to --no-fork-point to keep the
		 * base the same.
		 */
		якщо (options.fork_point < 0)
			options.fork_point = 0;
	}
	якщо (options.root && options.fork_point > 0)
		die(_("options '%s' and '%s' cannot be used together"), "--root", "--fork-point");

	якщо (options.action != ACTION_NONE && !in_progress)
		die(_("no rebase in progress"));

	якщо (options.action == ACTION_EDIT_TODO && !is_merge(&options))
		die(_("The --edit-todo action can only be used during "
		      "interactive rebase."));

	якщо (trace2_is_enabled()) {
		якщо (is_merge(&options))
			trace2_cmd_mode("interactive");
		інакше якщо (options.exec.nr)
			trace2_cmd_mode("interactive-exec");
		інакше
			trace2_cmd_mode(action_names[options.action]);
	}

	options.reflog_action = getenv(GIT_REFLOG_ACTION_ENVIRONMENT);
	options.reflog_action =
		xstrdup(options.reflog_action ? options.reflog_action : "rebase");

	switch (options.action) {
	case ACTION_CONTINUE: {
		struct object_id head;
		struct lock_file lock_file = LOCK_INIT;
		ціле fd;

		/* Sanity check */
		якщо (repo_get_oid(the_repository, "HEAD", &head))
			die(_("Cannot read HEAD"));

		fd = repo_hold_locked_index(the_repository, &lock_file, 0);
		якщо (repo_read_index(the_repository) < 0)
			die(_("could not read index"));
		refresh_index(the_repository->index, REFRESH_QUIET, NULL, NULL,
			      NULL);
		якщо (0 <= fd)
			repo_update_index_if_able(the_repository, &lock_file);
		rollback_lock_file(&lock_file);

		якщо (has_unstaged_changes(the_repository, 1)) {
			puts(_("You must edit all merge conflicts and then\n"
			       "mark them as resolved using git add"));
			exit(1);
		}
		якщо (read_basic_state(&options))
			exit(1);
		goto run_rebase;
	}
	case ACTION_SKIP: {
		struct string_list merge_rr = STRING_LIST_INIT_DUP;

		rerere_clear(the_repository, &merge_rr);
		string_list_clear(&merge_rr, 1);
		ropts.flags = RESET_WORKING_TREE_HARD |
			      RESET_WORKING_TREE_UPDATE_HEAD;
		якщо (reset_working_tree(the_repository, &ropts) < 0)
			die(_("could not discard worktree changes"));
		remove_branch_state(the_repository, 0);
		якщо (read_basic_state(&options))
			exit(1);
		goto run_rebase;
	}
	case ACTION_ABORT: {
		struct string_list merge_rr = STRING_LIST_INIT_DUP;
		struct strbuf head_msg = STRBUF_INIT;

		rerere_clear(the_repository, &merge_rr);
		string_list_clear(&merge_rr, 1);

		якщо (read_basic_state(&options))
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
		якщо (reset_working_tree(the_repository, &ropts) < 0)
			die(_("could not move back to %s"),
			    oid_to_hex(&options.orig_head->object.oid));
		strbuf_release(&head_msg);
		remove_branch_state(the_repository, 0);
		ret = finish_rebase(&options);
		goto cleanup;
	}
	case ACTION_QUIT: {
		save_autostash(state_dir_path("autostash", &options));
		якщо (options.type == REBASE_MERGE) {
			struct replay_opts replay = REPLAY_OPTS_INIT;

			replay.action = REPLAY_INTERACTIVE_REBASE;
			ret = sequencer_remove_state(&replay);
			replay_opts_release(&replay);
		} інакше {
			strbuf_reset(&buf);
			strbuf_addstr(&buf, options.state_dir);
			ret = remove_dir_recursively(&buf, 0);
			якщо (ret)
				error(_("could not remove '%s'"),
				       options.state_dir);
		}
		goto cleanup;
	}
	case ACTION_EDIT_TODO:
		options.dont_finish_rebase = 1;
		goto run_rebase;
	case ACTION_SHOW_CURRENT_PATCH:
		options.dont_finish_rebase = 1;
		goto run_rebase;
	case ACTION_NONE:
		break;
	default:
		BUG("action: %d", options.action);
	}

	/* Make sure no rebase is in progress */
	якщо (in_progress) {
		const char *last_slash = strrchr(options.state_dir, '/');
		const char *state_dir_base =
			last_slash ? last_slash + 1 : options.state_dir;
		const char *cmd_live_rebase =
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

	якщо ((options.flags & REBASE_INTERACTIVE_EXPLICIT) ||
	    (options.action != ACTION_NONE) ||
	    (options.exec.nr > 0) ||
	    options.autosquash == 1) {
		allow_preemptive_ff = 0;
	}
	якщо (options.committer_date_is_author_date || options.ignore_date)
		options.flags |= REBASE_FORCE;

	для (i = 0; i < options.git_am_opts.nr; i++) {
		const char *option = options.git_am_opts.v[i], *p;
		якщо (!strcmp(option, "--whitespace=fix") ||
		    !strcmp(option, "--whitespace=strip"))
			allow_preemptive_ff = 0;
		інакше якщо (skip_prefix(option, "-C", &p)) {
			поки (*p)
				якщо (!isdigit(*(p++)))
					die(_("switch `C' expects a "
					      "numerical value"));
		} інакше якщо (skip_prefix(option, "--whitespace=", &p)) {
			якщо (*p && strcmp(p, "warn") && strcmp(p, "nowarn") &&
			    strcmp(p, "error") && strcmp(p, "error-all"))
				die("Invalid whitespace option: '%s'", p);
		}
	}

	для (i = 0; i < options.exec.nr; i++)
		якщо (check_exec_cmd(options.exec.items[i].string))
			exit(1);

	якщо (!(options.flags & REBASE_NO_QUIET))
		strvec_push(&options.git_am_opts, "-q");

	якщо (options.empty != EMPTY_UNSPECIFIED)
		imply_merge(&options, "--empty");

	якщо (options.reapply_cherry_picks < 0)
		/*
		 * We default to --no-reapply-cherry-picks unless
		 * --keep-base is given; when --keep-base is given, we want
		 * to default to --reapply-cherry-picks.
		 */
		options.reapply_cherry_picks = keep_base;
	інакше якщо (!keep_base)
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

	якщо (gpg_sign)
		options.gpg_sign_opt = xstrfmt("-S%s", gpg_sign);

	якщо (options.exec.nr)
		imply_merge(&options, "--exec");

	якщо (options.type == REBASE_APPLY) {
		якщо (ignore_whitespace)
			strvec_push(&options.git_am_opts,
				    "--ignore-whitespace");
		якщо (options.committer_date_is_author_date)
			strvec_push(&options.git_am_opts,
				    "--committer-date-is-author-date");
		якщо (options.ignore_date)
			strvec_push(&options.git_am_opts, "--ignore-date");
	} інакше {
		/* REBASE_MERGE */
		якщо (ignore_whitespace) {
			string_list_append(&options.strategy_opts,
					   "ignore-space-change");
		}
	}

	якщо (strategy_opt)
		options.strategy = xstrdup(strategy_opt);
	інакше якщо (options.strategy_opts.nr && !options.strategy)
		options.strategy = xstrdup("ort");
	якщо (options.strategy)
		imply_merge(&options, "--strategy");

	якщо (options.root && !options.onto_name)
		imply_merge(&options, "--root without --onto");

	якщо (options.trailer_args.nr)
		imply_merge(&options, "--trailer");

	якщо (isatty(2) && options.flags & REBASE_NO_QUIET)
		strbuf_addstr(&options.git_format_patch_opt, " --progress");

	якщо (options.git_am_opts.nr || options.type == REBASE_APPLY) {
		/* all am options except -q are compatible only with --apply */
		для (i = options.git_am_opts.nr - 1; i >= 0; i--)
			якщо (strcmp(options.git_am_opts.v[i], "-q"))
				break;

		якщо (i >= 0 || options.type == REBASE_APPLY) {
			якщо (is_merge(&options))
				die(_("apply options and merge options "
					  "cannot be used together"));
			інакше якщо (options.rebase_merges == -1 && options.config_rebase_merges == 1)
				die(_("apply options are incompatible with rebase.rebaseMerges.  Consider adding --no-rebase-merges"));
			інакше якщо (options.update_refs == -1 && options.config_update_refs == 1)
				die(_("apply options are incompatible with rebase.updateRefs.  Consider adding --no-update-refs"));
			інакше
				options.type = REBASE_APPLY;
		}
	}

	якщо (options.update_refs == 1)
		imply_merge(&options, "--update-refs");
	options.update_refs = (options.update_refs >= 0) ? options.update_refs :
			     ((options.config_update_refs >= 0) ? options.config_update_refs : 0);

	якщо (options.rebase_merges == 1)
		imply_merge(&options, "--rebase-merges");
	options.rebase_merges = (options.rebase_merges >= 0) ? options.rebase_merges :
				((options.config_rebase_merges >= 0) ? options.config_rebase_merges : 0);

	якщо (options.autosquash == 1) {
		imply_merge(&options, "--autosquash");
	} інакше якщо (options.autosquash == -1) {
		options.autosquash =
			options.config_autosquash &&
			(options.flags & REBASE_INTERACTIVE_EXPLICIT);
	}

	якщо (options.type == REBASE_UNSPECIFIED) {
		якщо (!strcmp(options.default_backend, "merge"))
			options.type = REBASE_MERGE;
		інакше якщо (!strcmp(options.default_backend, "apply"))
			options.type = REBASE_APPLY;
		інакше
			die(_("Unknown rebase backend: %s"),
			    options.default_backend);
	}

	switch (options.type) {
	case REBASE_MERGE:
		options.state_dir = merge_dir();
		break;
	case REBASE_APPLY:
		options.state_dir = apply_dir();
		break;
	default:
		BUG("options.type was just set above; should be unreachable.");
	}

	якщо (options.empty == EMPTY_UNSPECIFIED) {
		якщо (options.flags & REBASE_INTERACTIVE_EXPLICIT)
			options.empty = EMPTY_STOP;
		інакше якщо (options.exec.nr > 0)
			options.empty = EMPTY_KEEP;
		інакше
			options.empty = EMPTY_DROP;
	}
	якщо (reschedule_failed_exec > 0 && !is_merge(&options))
		die(_("--reschedule-failed-exec requires "
		      "--exec or --interactive"));
	якщо (reschedule_failed_exec >= 0)
		options.reschedule_failed_exec = reschedule_failed_exec;

	якщо (options.signoff) {
		strvec_push(&options.git_am_opts, "--signoff");
		options.flags |= REBASE_FORCE;
	}

	якщо (!options.root) {
		якщо (argc < 1) {
			struct branch *branch;

			branch = branch_get(NULL);
			options.upstream_name = branch_get_upstream(branch,
								    NULL);
			якщо (!options.upstream_name)
				error_on_missing_default_upstream();
			якщо (options.fork_point < 0)
				options.fork_point = 1;
		} інакше {
			options.upstream_name = argv[0];
			argc--;
			argv++;
			якщо (!strcmp(options.upstream_name, "-"))
				options.upstream_name = "@{-1}";
		}
		options.upstream =
			lookup_commit_reference_by_name(options.upstream_name);
		якщо (!options.upstream)
			die(_("invalid upstream '%s'"), options.upstream_name);
		options.upstream_arg = options.upstream_name;
	} інакше {
		якщо (!options.onto_name) {
			якщо (commit_tree("", 0, the_hash_algo->empty_tree, NULL,
					&squash_onto, NULL, NULL) < 0)
				die(_("Could not create new root commit"));
			options.squash_onto = &squash_onto;
			options.onto_name = squash_onto_name =
				xstrdup(oid_to_hex(&squash_onto));
		} інакше
			options.root_with_onto = 1;

		options.upstream_name = NULL;
		options.upstream = NULL;
		якщо (argc > 1)
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
	якщо (argc == 1) {
		/* Is it "rebase other branchname" or "rebase other commit"? */
		struct object_id branch_oid;
		branch_name = argv[0];
		options.switch_to = argv[0];

		/* Is it a local branch? */
		strbuf_reset(&buf);
		strbuf_addf(&buf, "refs/heads/%s", branch_name);
		якщо (!refs_read_ref(get_main_ref_store(the_repository), buf.buf, &branch_oid)) {
			die_if_checked_out(buf.buf, 1);
			options.head_name = xstrdup(buf.buf);
			options.orig_head =
				lookup_commit_object(the_repository,
						     &branch_oid);
		/* If not is it a valid ref (branch or commit)? */
		} інакше {
			options.orig_head =
				lookup_commit_reference_by_name(branch_name);
			options.head_name = NULL;
		}
		якщо (!options.orig_head)
			die(_("no such branch/commit '%s'"), branch_name);
	} інакше якщо (argc == 0) {
		/* Do not need to switch branches, we are already on it. */
		options.head_name =
			xstrdup_or_null(refs_resolve_ref_unsafe(get_main_ref_store(the_repository), "HEAD", 0, NULL,
								&flags));
		якщо (!options.head_name)
			die(_("No such ref: %s"), "HEAD");
		якщо (flags & REF_ISSYMREF) {
			якщо (!skip_prefix(options.head_name,
					 "refs/heads/", &branch_name))
				branch_name = options.head_name;

		} інакше {
			FREE_AND_NULL(options.head_name);
			branch_name = "HEAD";
		}
		options.orig_head = lookup_commit_reference_by_name("HEAD");
		якщо (!options.orig_head)
			die(_("Could not resolve HEAD to a commit"));
	} інакше
		BUG("unexpected number of arguments left to parse");

	/* Make sure the branch to rebase onto is valid. */
	якщо (keep_base) {
		strbuf_reset(&buf);
		strbuf_addstr(&buf, options.upstream_name);
		strbuf_addstr(&buf, "...");
		strbuf_addstr(&buf, branch_name);
		options.onto_name = keep_base_onto_name = xstrdup(buf.buf);
	} інакше якщо (!options.onto_name)
		options.onto_name = options.upstream_name;
	якщо (strstr(options.onto_name, "...")) {
		якщо (repo_get_oid_mb(the_repository, options.onto_name, &branch_base) < 0) {
			якщо (keep_base)
				die(_("'%s': need exactly one merge base with branch"),
				    options.upstream_name);
			інакше
				die(_("'%s': need exactly one merge base"),
				    options.onto_name);
		}
		options.onto = lookup_commit_or_die(&branch_base,
						    options.onto_name);
	} інакше {
		options.onto =
			lookup_commit_reference_by_name(options.onto_name);
		якщо (!options.onto)
			die(_("Does not point to a valid commit '%s'"),
				options.onto_name);
		fill_branch_base(&options, &branch_base);
	}

	якщо (keep_base && options.reapply_cherry_picks)
		options.upstream = options.onto;

	якщо (options.fork_point > 0)
		options.restrict_revision =
			get_fork_point(options.upstream_name, options.orig_head);

	якщо (repo_read_index(the_repository) < 0)
		die(_("could not read index"));

	якщо (options.autostash)
		create_autostash(the_repository,
				 state_dir_path("autostash", &options));


	якщо (require_clean_work_tree(the_repository, "rebase",
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
	якщо (allow_preemptive_ff &&
	    can_fast_forward(options.onto, options.upstream, options.restrict_revision,
			     options.orig_head, &branch_base)) {
		ціле flag;

		якщо (!(options.flags & REBASE_FORCE)) {
			/* Lazily switch to the target branch if needed... */
			якщо (options.switch_to) {
				ret = checkout_up_to_date(&options);
				якщо (ret)
					goto cleanup_autostash;
			}

			якщо (!(options.flags & REBASE_NO_QUIET))
				; /* be quiet */
			інакше якщо (!strcmp(branch_name, "HEAD") &&
				 refs_resolve_ref_unsafe(get_main_ref_store(the_repository), "HEAD", 0, NULL, &flag))
				puts(_("HEAD is up to date."));
			інакше
				printf(_("Current branch %s is up to date.\n"),
				       branch_name);
			ret = finish_rebase(&options);
			goto cleanup;
		} інакше якщо (!(options.flags & REBASE_NO_QUIET))
			; /* be quiet */
		інакше якщо (!strcmp(branch_name, "HEAD") &&
			 refs_resolve_ref_unsafe(get_main_ref_store(the_repository), "HEAD", 0, NULL, &flag))
			puts(_("HEAD is up to date, rebase forced."));
		інакше
			printf(_("Current branch %s is up to date, rebase "
				 "forced.\n"), branch_name);
	}

	/* If a hook exists, give it a chance to interrupt*/
	якщо (!ok_to_skip_pre_rebase &&
	    run_hooks_l(the_repository, "pre-rebase", options.upstream_arg,
			argc ? argv[0] : NULL, NULL)) {
		ret = error(_("The pre-rebase hook refused to rebase."));
		goto cleanup_autostash;
	}

	якщо (options.flags & REBASE_DIFFSTAT) {
		struct diff_options opts;

		якщо (options.flags & REBASE_VERBOSE) {
			якщо (is_null_oid(&branch_base))
				printf(_("Changes to %s:\n"),
				       oid_to_hex(&options.onto->object.oid));
			інакше
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

	якщо (is_merge(&options))
		goto run_rebase;

	/* Detach HEAD and reset the tree */
	якщо (options.flags & REBASE_NO_QUIET)
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
	якщо (reset_working_tree(the_repository, &ropts)) {
		ret = error(_("Could not detach HEAD"));
		goto cleanup_autostash;
	}

	/*
	 * If the onto is a proper descendant of the tip of the branch, then
	 * we just fast-forwarded.
	 */
	якщо (oideq(&branch_base, &options.orig_head->object.oid)) {
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
	повернути !!ret;

cleanup_autostash:
	ret |= !!cleanup_autostash(&options);
	goto cleanup;
}
