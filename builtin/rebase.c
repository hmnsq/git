/*
 * "git rebase" builtin command
 *
 * Copyright (c) 2018 Pratik Karki
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#شامل "builtin.h"

#شامل "abspath.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "hex.h"
#شامل "run-command.h"
#شامل "strvec.h"
#شامل "dir.h"
#شامل "refs.h"
#شامل "config.h"
#شامل "unpack-trees.h"
#شامل "lockfile.h"
#شامل "object-file.h"
#شامل "object-name.h"
#شامل "parse-options.h"
#شامل "path.h"
#شامل "commit.h"
#شامل "diff.h"
#شامل "wt-status.h"
#شامل "revision.h"
#شامل "commit-reach.h"
#شامل "rerere.h"
#شامل "branch.h"
#شامل "sequencer.h"
#شامل "rebase-interactive.h"
#شامل "reset.h"
#شامل "trace2.h"
#شامل "hook.h"
#شامل "trailer.h"

ایستا نویسه ثابت * ثابت builtin_rebase_usage[] = {
	N_("git rebase [-i] [options] [--exec <cmd>] "
		"[--onto <newbase> | --keep-base] [<upstream> [<branch>]]"),
	N_("git rebase [-i] [options] [--exec <cmd>] [--onto <newbase>] "
		"--root [<branch>]"),
	"git rebase --continue | --abort | --skip | --edit-todo",
	پوچ
};

ایستا GIT_PATH_FUNC(path_squash_onto, "rebase-merge/squash-onto")
ایستا GIT_PATH_FUNC(path_interactive, "rebase-merge/interactive")
ایستا GIT_PATH_FUNC(apply_dir, "rebase-apply")
ایستا GIT_PATH_FUNC(merge_dir, "rebase-merge")

شمارش rebase_type {
	REBASE_UNSPECIFIED = -1,
	REBASE_APPLY,
	REBASE_MERGE
};

شمارش empty_type {
	EMPTY_UNSPECIFIED = -1,
	EMPTY_DROP,
	EMPTY_KEEP,
	EMPTY_STOP
};

شمارش action {
	ACTION_NONE = 0,
	ACTION_CONTINUE,
	ACTION_SKIP,
	ACTION_ABORT,
	ACTION_QUIT,
	ACTION_EDIT_TODO,
	ACTION_SHOW_CURRENT_PATCH
};

ایستا ثابت نویسه *action_names[] = {
	"undefined",
	"continue",
	"skip",
	"abort",
	"quit",
	"edit_todo",
	"show_current_patch"
};

ساختار rebase_options {
	شمارش rebase_type type;
	شمارش empty_type empty;
	نویسه *default_backend;
	ثابت نویسه *state_dir;
	ساختار commit *upstream;
	ثابت نویسه *upstream_name;
	ثابت نویسه *upstream_arg;
	نویسه *head_name;
	ساختار commit *orig_head;
	ساختار commit *onto;
	ثابت نویسه *onto_name;
	ثابت نویسه *revisions;
	ثابت نویسه *switch_to;
	صحیح ریشه, ریشه_with_onto;
	ساختار object_id *squash_onto;
	ساختار commit *restrict_revision;
	صحیح dont_finish_rebase;
	شمارش {
		REBASE_NO_QUIET = 1<<0,
		REBASE_VERBOSE = 1<<1,
		REBASE_DIFFSTAT = 1<<2,
		REBASE_FORCE = 1<<3,
		REBASE_INTERACTIVE_EXPLICIT = 1<<4,
	} flags;
	ساختار strvec git_am_opts;
	شمارش action action;
	نویسه *reflog_action;
	صحیح signoff;
	ساختار strvec trailer_args;
	صحیح allow_rerere_autoupdate;
	صحیح keep_empty;
	صحیح autosquash;
	نویسه *gpg_sign_opt;
	صحیح autostash;
	صحیح committer_date_is_author_date;
	صحیح ignore_date;
	ساختار string_list exec;
	صحیح allow_empty_message;
	صحیح rebase_merges, rebase_cousins;
	نویسه *strategy;
	ساختار string_list strategy_opts;
	ساختار strbuf git_format_patch_opt;
	صحیح reschedule_failed_exec;
	صحیح reapply_cherry_picks;
	صحیح fork_point;
	صحیح update_refs;
	صحیح config_autosquash;
	صحیح config_rebase_merges;
	صحیح config_update_refs;
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

ایستا تهی rebase_options_release(ساختار rebase_options *opts)
{
	آزاد(opts->default_backend);
	آزاد(opts->reflog_action);
	آزاد(opts->head_name);
	strvec_clear(&opts->git_am_opts);
	آزاد(opts->gpg_sign_opt);
	string_list_clear(&opts->exec, 0);
	آزاد(opts->strategy);
	string_list_clear(&opts->strategy_opts, 0);
	strbuf_release(&opts->git_format_patch_opt);
	strvec_clear(&opts->trailer_args);
}

ایستا ساختار replay_opts get_replay_opts(ثابت ساختار rebase_options *opts)
{
	ساختار replay_opts replay = REPLAY_OPTS_INIT;

	replay.action = REPLAY_INTERACTIVE_REBASE;
	replay.strategy = پوچ;
	sequencer_init_config(&replay);

	replay.signoff = opts->signoff;

	strvec_pushv(&replay.trailer_args, opts->trailer_args.v);

	replay.allow_ff = !(opts->flags & REBASE_FORCE);
	اگر (opts->allow_rerere_autoupdate)
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
	آزاد(replay.gpg_sign);
	replay.gpg_sign = xstrdup_or_null(opts->gpg_sign_opt);
	replay.reflog_action = xstrdup(opts->reflog_action);
	اگر (opts->strategy)
		replay.strategy = xstrdup_or_null(opts->strategy);
	وگرنه اگر (!replay.strategy && replay.default_strategy) {
		replay.strategy = replay.default_strategy;
		replay.default_strategy = پوچ;
	}

	برای (size_t i = 0; i < opts->strategy_opts.nr; i++)
		strvec_افزودن(&replay.xopts, opts->strategy_opts.اقلام[i].string);

	اگر (opts->squash_onto) {
		oidcpy(&replay.squash_onto, opts->squash_onto);
		replay.have_squash_onto = 1;
	}

	بازگشت replay;
}

ایستا صحیح edit_todo_file(unsigned flags, ساختار replay_opts *opts)
{
	ثابت نویسه *todo_file = rebase_path_todo();
	ساختار todo_list todo_list = TODO_LIST_INIT,
		new_todo = TODO_LIST_INIT;
	صحیح res = 0;

	اگر (strbuf_read_file(&todo_list.buf, todo_file, 0) < 0)
		بازگشت error_errno(_("could not read '%s'."), todo_file);

	strbuf_stripspace(&todo_list.buf, comment_سطر_str);
	res = edit_todo_list(the_repository, opts, &todo_list, &new_todo,
			     پوچ, پوچ, flags);
	اگر (!res && todo_list_write_to_file(the_repository, &new_todo, todo_file,
					    پوچ, پوچ, -1, flags & ~(TODO_LIST_SHORTEN_IDS)))
		res = error_errno(_("could not write '%s'"), todo_file);

	todo_list_release(&todo_list);
	todo_list_release(&new_todo);

	بازگشت res;
}

ایستا صحیح get_revision_ranges(ساختار commit *upstream, ساختار commit *onto,
			       ساختار object_id *orig_head, نویسه **revisions,
			       نویسه **shortrevisions)
{
	ساختار commit *base_rev = upstream ? upstream : onto;
	ثابت نویسه *shorthead;

	*revisions = xstrfmt("%s...%s", oid_to_hex(&base_rev->object.oid),
			     oid_to_hex(orig_head));

	shorthead = repo_find_unique_abbrev(the_repository, orig_head,
					    DEFAULT_ABBREV);

	اگر (upstream) {
		ثابت نویسه *shortrev;

		shortrev = repo_find_unique_abbrev(the_repository,
						   &base_rev->object.oid,
						   DEFAULT_ABBREV);

		*shortrevisions = xstrfmt("%s..%s", shortrev, shorthead);
	} وگرنه
		*shortrevisions = xstrdup(shorthead);

	بازگشت 0;
}

ایستا صحیح init_basic_state(ساختار replay_opts *opts, ثابت نویسه *head_name,
			    ساختار commit *onto,
			    ثابت ساختار object_id *orig_head)
{
	FILE *interactive;

	اگر (!is_directory(merge_dir()) &&
	    safe_create_dir_in_gitdir(the_repository, merge_dir()))
		بازگشت error_errno(_("could not create temporary %s"), merge_dir());

	refs_delete_reflog(get_main_ref_store(the_repository), "REBASE_HEAD");

	interactive = fopen(path_interactive(), "w");
	اگر (!interactive)
		بازگشت error_errno(_("could not mark as interactive"));
	fclose(interactive);

	بازگشت write_basic_state(opts, head_name, onto, orig_head);
}

ایستا صحیح do_interactive_rebase(ساختار rebase_options *opts, unsigned flags)
{
	صحیح ret = -1;
	نویسه *revisions = پوچ, *shortrevisions = پوچ;
	ساختار strvec make_script_args = STRVEC_INIT;
	ساختار todo_list todo_list = TODO_LIST_INIT;
	ساختار replay_opts replay = get_replay_opts(opts);

	اگر (get_revision_ranges(opts->upstream, opts->onto, &opts->orig_head->object.oid,
				&revisions, &shortrevisions))
		برو cleanup;

	strvec_pushl(&make_script_args, "", revisions, پوچ);
	اگر (opts->restrict_revision)
		strvec_pushf(&make_script_args, "^%s",
			     oid_to_hex(&opts->restrict_revision->object.oid));

	ret = sequencer_make_script(the_repository, &todo_list.buf,
				    &make_script_args, flags);
	اگر (ret) {
		error(_("could not generate todo list"));
		برو cleanup;
	}

	اگر (init_basic_state(&replay,
			     opts->head_name ? opts->head_name : "detached HEAD",
			     opts->onto, &opts->orig_head->object.oid))
		برو cleanup;

	اگر (!opts->upstream && opts->squash_onto)
		write_file(path_squash_onto(), "%s\n",
			   oid_to_hex(opts->squash_onto));

	discard_index(the_repository->index);
	اگر (todo_list_parse_insn_buffer(the_repository, &replay,
					todo_list.buf.buf, &todo_list))
		BUG("unusable todo list");

	ret = complete_action(the_repository, &replay, flags,
		shortrevisions, opts->onto_name, opts->onto,
		&opts->orig_head->object.oid, &opts->exec,
		opts->autosquash, opts->update_refs, &todo_list);

cleanup:
	replay_opts_release(&replay);
	آزاد(revisions);
	آزاد(shortrevisions);
	todo_list_release(&todo_list);
	strvec_clear(&make_script_args);

	بازگشت ret;
}

ایستا صحیح run_sequencer_rebase(ساختار rebase_options *opts)
{
	unsigned flags = 0;
	صحیح abbreviate_commands = 0, ret = 0;

	repo_config_get_bool(the_repository, "rebase.abbreviatecommands", &abbreviate_commands);

	flags |= opts->keep_empty ? TODO_LIST_KEEP_EMPTY : 0;
	flags |= abbreviate_commands ? TODO_LIST_ABBREVIATE_CMDS : 0;
	flags |= opts->rebase_merges ? TODO_LIST_REBASE_MERGES : 0;
	flags |= opts->rebase_cousins > 0 ? TODO_LIST_REBASE_COUSINS : 0;
	flags |= opts->ریشه_with_onto ? TODO_LIST_ریشه_WITH_ONTO : 0;
	flags |= opts->reapply_cherry_picks ? TODO_LIST_REAPPLY_CHERRY_PICKS : 0;
	flags |= opts->flags & REBASE_NO_QUIET ? TODO_LIST_WARN_SKIPPED_CHERRY_PICKS : 0;

	انتخاب (opts->action) {
	حالت ACTION_NONE: {
		اگر (!opts->onto && !opts->upstream)
			die(_("a base commit must be provided with --upstream or --onto"));

		ret = do_interactive_rebase(opts, flags);
		بشکن;
	}
	حالت ACTION_SKIP: {
		ساختار string_list merge_rr = STRING_LIST_INIT_DUP;

		rerere_clear(the_repository, &merge_rr);
	}
		/* fallthrough */
	حالت ACTION_CONTINUE: {
		ساختار replay_opts replay_opts = get_replay_opts(opts);

		ret = sequencer_continue(the_repository, &replay_opts);
		replay_opts_release(&replay_opts);
		بشکن;
	}
	حالت ACTION_EDIT_TODO: {
		ساختار replay_opts replay_opts = get_replay_opts(opts);

		ret = edit_todo_file(flags, &replay_opts);
		replay_opts_release(&replay_opts);
		بشکن;
	}
	حالت ACTION_SHOW_CURRENT_PATCH: {
		ساختار child_process cmd = CHILD_PROCESS_INIT;

		cmd.git_cmd = 1;
		strvec_pushl(&cmd.args, "show", "REBASE_HEAD", "--", پوچ);
		ret = run_command(&cmd);

		بشکن;
	}
	پیشفرض:
		BUG("invalid command '%d'", opts->action);
	}

	بازگشت ret;
}

ایستا صحیح is_merge(ساختار rebase_options *opts)
{
	بازگشت opts->type == REBASE_MERGE;
}

ایستا تهی imply_merge(ساختار rebase_options *opts, ثابت نویسه *option)
{
	انتخاب (opts->type) {
	حالت REBASE_APPLY:
		die(_("%s requires the merge backend"), option);
		بشکن;
	حالت REBASE_MERGE:
		بشکن;
	پیشفرض:
		opts->type = REBASE_MERGE; /* implied */
		بشکن;
	}
}

/* Returns the filename prefixed by the state_dir */
ایستا ثابت نویسه *state_dir_path(ثابت نویسه *filename, ساختار rebase_options *opts)
{
	ایستا ساختار strbuf path = STRBUF_INIT;
	ایستا size_t prefix_len;

	اگر (!prefix_len) {
		strbuf_addf(&path, "%s/", opts->state_dir);
		prefix_len = path.len;
	}

	strbuf_setlen(&path, prefix_len);
	strbuf_addstr(&path, filename);
	بازگشت path.buf;
}

/* Initialize the rebase options from the state directory. */
ایستا صحیح read_basic_state(ساختار rebase_options *opts)
{
	ساختار strbuf head_name = STRBUF_INIT;
	ساختار strbuf buf = STRBUF_INIT;
	ساختار object_id oid;

	اگر (!read_oneliner(&head_name, state_dir_path("head-name", opts),
			   READ_ONELINER_WARN_MISSING) ||
	    !read_oneliner(&buf, state_dir_path("onto", opts),
			   READ_ONELINER_WARN_MISSING))
		بازگشت -1;
	opts->head_name = starts_with(head_name.buf, "refs/") ?
		xstrdup(head_name.buf) : پوچ;
	strbuf_release(&head_name);
	اگر (get_oid_hex(buf.buf, &oid) ||
	    !(opts->onto = lookup_commit_object(the_repository, &oid)))
		بازگشت error(_("invalid onto: '%s'"), buf.buf);

	/*
	 * We always write to orig-head, but interactive rebase used to write to
	 * head. Fall back to reading from head to cover for the case that the
	 * user upgraded git with an ongoing interactive rebase.
	 */
	strbuf_reset(&buf);
	اگر (file_exists(state_dir_path("orig-head", opts))) {
		اگر (!read_oneliner(&buf, state_dir_path("orig-head", opts),
				   READ_ONELINER_WARN_MISSING))
			بازگشت -1;
	} وگرنه اگر (!read_oneliner(&buf, state_dir_path("head", opts),
				  READ_ONELINER_WARN_MISSING))
		بازگشت -1;
	اگر (get_oid_hex(buf.buf, &oid) ||
	    !(opts->orig_head = lookup_commit_object(the_repository, &oid)))
		بازگشت error(_("invalid orig-head: '%s'"), buf.buf);

	اگر (file_exists(state_dir_path("quiet", opts)))
		opts->flags &= ~REBASE_NO_QUIET;
	وگرنه
		opts->flags |= REBASE_NO_QUIET;

	اگر (file_exists(state_dir_path("verbose", opts)))
		opts->flags |= REBASE_VERBOSE;

	اگر (file_exists(state_dir_path("signoff", opts))) {
		opts->signoff = 1;
		opts->flags |= REBASE_FORCE;
	}

	اگر (file_exists(state_dir_path("allow_rerere_autoupdate", opts))) {
		strbuf_reset(&buf);
		اگر (!read_oneliner(&buf, state_dir_path("allow_rerere_autoupdate", opts),
				   READ_ONELINER_WARN_MISSING))
			بازگشت -1;
		اگر (!strcmp(buf.buf, "--rerere-autoupdate"))
			opts->allow_rerere_autoupdate = RERERE_AUTOUPDATE;
		وگرنه اگر (!strcmp(buf.buf, "--no-rerere-autoupdate"))
			opts->allow_rerere_autoupdate = RERERE_NOAUTOUPDATE;
		وگرنه
			warning(_("ignoring invalid allow_rerere_autoupdate: "
				  "'%s'"), buf.buf);
	}

	اگر (file_exists(state_dir_path("gpg_sign_opt", opts))) {
		strbuf_reset(&buf);
		اگر (!read_oneliner(&buf, state_dir_path("gpg_sign_opt", opts),
				   READ_ONELINER_WARN_MISSING))
			بازگشت -1;
		آزاد(opts->gpg_sign_opt);
		opts->gpg_sign_opt = xstrdup(buf.buf);
	}

	strbuf_release(&buf);

	بازگشت 0;
}

ایستا صحیح rebase_write_basic_state(ساختار rebase_options *opts)
{
	write_file(state_dir_path("head-name", opts), "%s",
		   opts->head_name ? opts->head_name : "detached HEAD");
	write_file(state_dir_path("onto", opts), "%s",
		   opts->onto ? oid_to_hex(&opts->onto->object.oid) : "");
	write_file(state_dir_path("orig-head", opts), "%s",
		   oid_to_hex(&opts->orig_head->object.oid));
	اگر (!(opts->flags & REBASE_NO_QUIET))
		write_file(state_dir_path("quiet", opts), "%s", "");
	اگر (opts->flags & REBASE_VERBOSE)
		write_file(state_dir_path("verbose", opts), "%s", "");
	اگر (opts->allow_rerere_autoupdate > 0)
		write_file(state_dir_path("allow_rerere_autoupdate", opts),
			   "-%s-rerere-autoupdate",
			   opts->allow_rerere_autoupdate == RERERE_AUTOUPDATE ?
				"" : "-no");
	اگر (opts->gpg_sign_opt)
		write_file(state_dir_path("gpg_sign_opt", opts), "%s",
			   opts->gpg_sign_opt);
	اگر (opts->signoff)
		write_file(state_dir_path("signoff", opts), "--signoff");

	بازگشت 0;
}

ایستا صحیح cleanup_autostash(ساختار rebase_options *opts)
{
	صحیح ret;
	ساختار strbuf dir = STRBUF_INIT;
	ثابت نویسه *path = state_dir_path("autostash", opts);

	اگر (!file_exists(path))
		بازگشت 0;
	ret = apply_autostash(path);
	strbuf_addstr(&dir, opts->state_dir);
	اگر (remove_dir_recursively(&dir, 0))
		ret = error_errno(_("could not remove '%s'"), opts->state_dir);
	strbuf_release(&dir);

	بازگشت ret;
}

ایستا صحیح finish_rebase(ساختار rebase_options *opts)
{
	ساختار strbuf dir = STRBUF_INIT;
	صحیح ret = 0;

	refs_delete_ref(get_main_ref_store(the_repository), پوچ,
			"REBASE_HEAD", پوچ, REF_NO_DEREF);
	refs_delete_ref(get_main_ref_store(the_repository), پوچ,
			"AUTO_MERGE", پوچ, REF_NO_DEREF);
	apply_autostash(state_dir_path("autostash", opts));
	/*
	 * We ignore errors in 'git maintenance run --auto', since the
	 * user should see them.
	 */
	run_auto_maintenance(the_repository,
			!(opts->flags & (REBASE_NO_QUIET|REBASE_VERBOSE)));

	اگر (opts->type == REBASE_MERGE) {
		ساختار replay_opts replay = REPLAY_OPTS_INIT;

		replay.action = REPLAY_INTERACTIVE_REBASE;
		ret = sequencer_remove_state(&replay);
		replay_opts_release(&replay);
	} وگرنه {
		strbuf_addstr(&dir, opts->state_dir);
		اگر (remove_dir_recursively(&dir, 0))
			ret = error(_("could not remove '%s'"),
				    opts->state_dir);
		strbuf_release(&dir);
	}

	بازگشت ret;
}

ایستا صحیح move_to_original_branch(ساختار rebase_options *opts)
{
	ساختار strbuf branch_reflog = STRBUF_INIT, head_reflog = STRBUF_INIT;
	ساختار reset_working_درخت_options ropts = { 0 };
	صحیح ret;

	اگر (!opts->head_name)
		بازگشت 0; /* nothing to move back to */

	اگر (!opts->onto)
		BUG("move_to_original_branch without onto");

	strbuf_addf(&branch_reflog, "%s (finish): %s onto %s",
		    opts->reflog_action,
		    opts->head_name, oid_to_hex(&opts->onto->object.oid));
	strbuf_addf(&head_reflog, "%s (finish): returning to %s",
		    opts->reflog_action, opts->head_name);
	ropts.branch = opts->head_name;
	ropts.flags = RESET_WORKING_درخت_REFS_ONLY |
		      RESET_WORKING_درخت_UPDATE_HEAD;
	ropts.branch_پیام = branch_reflog.buf;
	ropts.head_پیام = head_reflog.buf;
	ret = reset_working_درخت(the_repository, &ropts);

	strbuf_release(&branch_reflog);
	strbuf_release(&head_reflog);
	بازگشت ret;
}

ایستا صحیح run_am(ساختار rebase_options *opts)
{
	ساختار child_process am = CHILD_PROCESS_INIT;
	ساختار child_process format_patch = CHILD_PROCESS_INIT;
	صحیح status;
	نویسه *rebased_patches;

	am.git_cmd = 1;
	strvec_افزودن(&am.args, "am");
	strvec_pushf(&am.env, GIT_REFLOG_ACTION_ENVIRONMENT "=%s (pick)",
		     opts->reflog_action);
	اگر (opts->action == ACTION_CONTINUE) {
		strvec_افزودن(&am.args, "--resolved");
		strvec_pushf(&am.args, "--resolvemsg=%s", rebase_resolvemsg);
		اگر (opts->gpg_sign_opt)
			strvec_افزودن(&am.args, opts->gpg_sign_opt);
		status = run_command(&am);
		اگر (status)
			بازگشت status;

		بازگشت move_to_original_branch(opts);
	}
	اگر (opts->action == ACTION_SKIP) {
		strvec_افزودن(&am.args, "--skip");
		strvec_pushf(&am.args, "--resolvemsg=%s", rebase_resolvemsg);
		status = run_command(&am);
		اگر (status)
			بازگشت status;

		بازگشت move_to_original_branch(opts);
	}
	اگر (opts->action == ACTION_SHOW_CURRENT_PATCH) {
		strvec_افزودن(&am.args, "--show-current-patch");
		بازگشت run_command(&am);
	}

	rebased_patches = repo_git_path(the_repository, "rebased-patches");
	format_patch.out = open(rebased_patches,
				O_WRONLY | O_CREAT | O_TRUNC, 0666);
	اگر (format_patch.out < 0) {
		status = error_errno(_("could not open '%s' for writing"),
				     rebased_patches);
		آزاد(rebased_patches);
		child_process_clear(&am);
		بازگشت status;
	}

	format_patch.git_cmd = 1;
	strvec_pushl(&format_patch.args, "format-patch", "-k", "--stdout",
		     "--full-index", "--cherry-pick", "--right-only",
		     "--default-prefix", "--no-renames",
		     "--no-cover-letter", "--pretty=mboxrd", "--topo-order",
		     "--no-base", پوچ);
	اگر (opts->git_format_patch_opt.len)
		strvec_split(&format_patch.args,
			     opts->git_format_patch_opt.buf);
	strvec_pushf(&format_patch.args, "%s...%s",
		     oid_to_hex(opts->ریشه ?
				/* this is now equivalent to !opts->upstream */
				&opts->onto->object.oid :
				&opts->upstream->object.oid),
		     oid_to_hex(&opts->orig_head->object.oid));
	اگر (opts->restrict_revision)
		strvec_pushf(&format_patch.args, "^%s",
			     oid_to_hex(&opts->restrict_revision->object.oid));

	status = run_command(&format_patch);
	اگر (status) {
		ساختار reset_working_درخت_options ropts = { 0 };
		unlink(rebased_patches);
		آزاد(rebased_patches);
		child_process_clear(&am);

		ropts.oid = &opts->orig_head->object.oid;
		ropts.branch = opts->head_name;
		ropts.default_reflog_action = opts->reflog_action;
		ropts.flags = RESET_WORKING_درخت_UPDATE_HEAD;
		reset_working_درخت(the_repository, &ropts);
		error(_("\ngit encountered an error while preparing the "
			"patches to replay\n"
			"these revisions:\n"
			"\n    %s\n\n"
			"As a result, git cannot rebase them."),
		      opts->revisions);

		بازگشت status;
	}

	am.in = open(rebased_patches, O_RDONLY);
	اگر (am.in < 0) {
		status = error_errno(_("could not open '%s' for reading"),
				     rebased_patches);
		آزاد(rebased_patches);
		child_process_clear(&am);
		بازگشت status;
	}

	strvec_pushv(&am.args, opts->git_am_opts.v);
	strvec_افزودن(&am.args, "--rebasing");
	strvec_pushf(&am.args, "--resolvemsg=%s", rebase_resolvemsg);
	strvec_افزودن(&am.args, "--patch-format=mboxrd");
	اگر (opts->allow_rerere_autoupdate == RERERE_AUTOUPDATE)
		strvec_افزودن(&am.args, "--rerere-autoupdate");
	وگرنه اگر (opts->allow_rerere_autoupdate == RERERE_NOAUTOUPDATE)
		strvec_افزودن(&am.args, "--no-rerere-autoupdate");
	اگر (opts->gpg_sign_opt)
		strvec_افزودن(&am.args, opts->gpg_sign_opt);
	status = run_command(&am);
	unlink(rebased_patches);
	آزاد(rebased_patches);

	اگر (!status) {
		بازگشت move_to_original_branch(opts);
	}

	اگر (is_directory(opts->state_dir))
		rebase_write_basic_state(opts);

	بازگشت status;
}

ایستا صحیح run_specific_rebase(ساختار rebase_options *opts)
{
	صحیح status;

	اگر (opts->type == REBASE_MERGE) {
		/* Run sequencer-based rebase */
		اگر (!(opts->flags & REBASE_INTERACTIVE_EXPLICIT))
			setenv("GIT_SEQUENCE_EDITOR", ":", 1);
		اگر (opts->gpg_sign_opt) {
			/* remove the leading "-S" */
			نویسه *tmp = xstrdup(opts->gpg_sign_opt + 2);
			آزاد(opts->gpg_sign_opt);
			opts->gpg_sign_opt = tmp;
		}

		status = run_sequencer_rebase(opts);
	} وگرنه اگر (opts->type == REBASE_APPLY)
		status = run_am(opts);
	وگرنه
		BUG("Unhandled rebase type %d", opts->type);

	اگر (opts->dont_finish_rebase)
		; /* do nothing */
	وگرنه اگر (opts->type == REBASE_MERGE) {
		صحیح quiet = !(opts->flags & (REBASE_NO_QUIET|REBASE_VERBOSE));

		/*
		 * The sequencer cleans up after itself. Its state directory
		 * is gone once it is done, and stays while it is stopped.
		 */
		اگر (status == 0 && !is_directory(opts->state_dir))
			run_auto_maintenance(the_repository, quiet);
	} وگرنه اگر (status == 0) {
		اگر (!file_exists(state_dir_path("stopped-sha", opts)))
			finish_rebase(opts);
	} وگرنه اگر (status == 2) {
		ساختار strbuf dir = STRBUF_INIT;

		apply_autostash(state_dir_path("autostash", opts));
		strbuf_addstr(&dir, opts->state_dir);
		remove_dir_recursively(&dir, 0);
		strbuf_release(&dir);
		die("Nothing to do");
	}

	بازگشت status ? -1 : 0;
}

ایستا تهی parse_rebase_merges_مقدار(ساختار rebase_options *options, ثابت نویسه *مقدار)
{
	اگر (!strcmp("no-rebase-cousins", مقدار))
		options->rebase_cousins = 0;
	وگرنه اگر (!strcmp("rebase-cousins", مقدار))
		options->rebase_cousins = 1;
	وگرنه
		die(_("Unknown rebase-merges mode: %s"), مقدار);
}

ایستا صحیح rebase_config(ثابت نویسه *var, ثابت نویسه *مقدار,
			 ثابت ساختار config_context *ctx, تهی *data)
{
	ساختار rebase_options *opts = data;

	اگر (!strcmp(var, "rebase.stat")) {
		اگر (git_config_bool(var, مقدار))
			opts->flags |= REBASE_DIFFSTAT;
		وگرنه
			opts->flags &= ~REBASE_DIFFSTAT;
		بازگشت 0;
	}

	اگر (!strcmp(var, "rebase.autosquash")) {
		opts->config_autosquash = git_config_bool(var, مقدار);
		بازگشت 0;
	}

	اگر (!strcmp(var, "commit.gpgsign")) {
		آزاد(opts->gpg_sign_opt);
		opts->gpg_sign_opt = git_config_bool(var, مقدار) ?
			xstrdup("-S") : پوچ;
		بازگشت 0;
	}

	اگر (!strcmp(var, "rebase.autostash")) {
		opts->autostash = git_config_bool(var, مقدار);
		بازگشت 0;
	}

	اگر (!strcmp(var, "rebase.rebasemerges")) {
		opts->config_rebase_merges = git_parse_maybe_bool(مقدار);
		اگر (opts->config_rebase_merges < 0) {
			opts->config_rebase_merges = 1;
			parse_rebase_merges_مقدار(opts, مقدار);
		} وگرنه {
			opts->rebase_cousins = 0;
		}
		بازگشت 0;
	}

	اگر (!strcmp(var, "rebase.updaterefs")) {
		opts->config_update_refs = git_config_bool(var, مقدار);
		بازگشت 0;
	}

	اگر (!strcmp(var, "rebase.reschedulefailedexec")) {
		opts->reschedule_failed_exec = git_config_bool(var, مقدار);
		بازگشت 0;
	}

	اگر (!strcmp(var, "rebase.forkpoint")) {
		opts->fork_point = git_config_bool(var, مقدار) ? -1 : 0;
		بازگشت 0;
	}

	اگر (!strcmp(var, "rebase.backend")) {
		FREE_AND_NULL(opts->default_backend);
		بازگشت git_config_string(&opts->default_backend, var, مقدار);
	}

	بازگشت git_default_config(var, مقدار, ctx, data);
}

ایستا صحیح checkout_up_to_date(ساختار rebase_options *options)
{
	ساختار strbuf buf = STRBUF_INIT;
	ساختار reset_working_درخت_options ropts = { 0 };
	صحیح ret = 0;

	strbuf_addf(&buf, "%s: checkout %s",
		    options->reflog_action, options->switch_to);
	ropts.oid = &options->orig_head->object.oid;
	ropts.branch = options->head_name;
	ropts.flags = RESET_WORKING_درخت_RUN_POST_CHECKOUT_HOOK |
		      RESET_WORKING_درخت_UPDATE_HEAD;
	اگر (!ropts.branch)
		ropts.flags |=  RESET_WORKING_درخت_DETACH;
	ropts.head_پیام = buf.buf;
	اگر (reset_working_درخت(the_repository, &ropts) < 0)
		ret = error(_("could not switch to %s"), options->switch_to);
	strbuf_release(&buf);

	بازگشت ret;
}

/*
 * Determines whether the commits in from..to are linear, i.e. contain
 * no merge commits. This function *expects* `from` to be an ancestor of
 * `to`.
 */
ایستا صحیح is_linear_history(ساختار commit *from, ساختار commit *to)
{
	وقتی (to && to != from) {
		repo_parse_commit(the_repository, to);
		اگر (!to->parents)
			بازگشت 1;
		اگر (to->parents->next)
			بازگشت 0;
		to = to->parents->item;
	}
	بازگشت 1;
}

ایستا صحیح can_fast_forward(ساختار commit *onto, ساختار commit *upstream,
			    ساختار commit *restrict_revision,
			    ساختار commit *head, ساختار object_id *branch_base)
{
	ساختار commit_list *merge_bases = پوچ;
	صحیح res = 0;

	اگر (is_null_oid(branch_base))
		برو done; /* fill_branch_base() found multiple merge bases */

	اگر (!oideq(branch_base, &onto->object.oid))
		برو done;

	اگر (restrict_revision && !oideq(&restrict_revision->object.oid, branch_base))
		برو done;

	اگر (!upstream)
		برو done;

	اگر (repo_get_merge_bases(the_repository, upstream, head, &merge_bases) < 0)
		exit(128);
	اگر (!merge_bases || merge_bases->next)
		برو done;

	اگر (!oideq(&onto->object.oid, &merge_bases->item->object.oid))
		برو done;

	res = 1;

done:
	commit_list_free(merge_bases);
	بازگشت res && is_linear_history(onto, head);
}

ایستا تهی fill_branch_base(ساختار rebase_options *options,
			    ساختار object_id *branch_base)
{
	ساختار commit_list *merge_bases = پوچ;

	اگر (repo_get_merge_bases(the_repository, options->onto,
				 options->orig_head, &merge_bases) < 0)
		exit(128);
	اگر (!merge_bases || merge_bases->next)
		oidcpy(branch_base, null_oid(the_hash_algo));
	وگرنه
		oidcpy(branch_base, &merge_bases->item->object.oid);

	commit_list_free(merge_bases);
}

ایستا صحیح parse_opt_am(ثابت ساختار option *opt, ثابت نویسه *arg, صحیح unset)
{
	ساختار rebase_options *opts = opt->مقدار;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	اگر (opts->type != REBASE_UNSPECIFIED && opts->type != REBASE_APPLY)
	    die(_("apply options and merge options cannot be used together"));

	opts->type = REBASE_APPLY;

	بازگشت 0;
}

/* -i followed by -m is still -i */
ایستا صحیح parse_opt_merge(ثابت ساختار option *opt, ثابت نویسه *arg, صحیح unset)
{
	ساختار rebase_options *opts = opt->مقدار;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	اگر (opts->type != REBASE_UNSPECIFIED && opts->type != REBASE_MERGE)
	    die(_("apply options and merge options cannot be used together"));

	opts->type = REBASE_MERGE;

	بازگشت 0;
}

/* -i followed by -r is still explicitly interactive, but -r alone is not */
ایستا صحیح parse_opt_interactive(ثابت ساختار option *opt, ثابت نویسه *arg,
				 صحیح unset)
{
	ساختار rebase_options *opts = opt->مقدار;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	اگر (opts->type != REBASE_UNSPECIFIED && opts->type != REBASE_MERGE)
	    die(_("apply options and merge options cannot be used together"));

	opts->type = REBASE_MERGE;
	opts->flags |= REBASE_INTERACTIVE_EXPLICIT;

	بازگشت 0;
}

ایستا شمارش empty_type parse_empty_مقدار(ثابت نویسه *مقدار)
{
	اگر (!strcasecmp(مقدار, "drop"))
		بازگشت EMPTY_DROP;
	وگرنه اگر (!strcasecmp(مقدار, "keep"))
		بازگشت EMPTY_KEEP;
	وگرنه اگر (!strcasecmp(مقدار, "stop"))
		بازگشت EMPTY_STOP;
	وگرنه اگر (!strcasecmp(مقدار, "ask")) {
		warning(_("--empty=ask is deprecated; use '--empty=stop' instead."));
		بازگشت EMPTY_STOP;
	}

	die(_("unrecognized empty type '%s'; valid values are \"drop\", \"keep\", and \"stop\"."), مقدار);
}

ایستا صحیح parse_opt_keep_empty(ثابت ساختار option *opt, ثابت نویسه *arg,
				صحیح unset)
{
	ساختار rebase_options *opts = opt->مقدار;

	BUG_ON_OPT_ARG(arg);

	imply_merge(opts, unset ? "--no-keep-empty" : "--keep-empty");
	opts->keep_empty = !unset;
	بازگشت 0;
}

ایستا صحیح parse_opt_empty(ثابت ساختار option *opt, ثابت نویسه *arg, صحیح unset)
{
	ساختار rebase_options *options = opt->مقدار;
	شمارش empty_type مقدار = parse_empty_مقدار(arg);

	BUG_ON_OPT_NEG(unset);

	options->empty = مقدار;
	بازگشت 0;
}

ایستا صحیح parse_opt_rebase_merges(ثابت ساختار option *opt, ثابت نویسه *arg, صحیح unset)
{
	ساختار rebase_options *options = opt->مقدار;

	options->rebase_merges = !unset;
	options->rebase_cousins = 0;

	اگر (arg) {
		اگر (!*arg) {
			warning(_("--rebase-merges with an empty string "
				  "argument is deprecated and will stop "
				  "working in a future version of Git. Use "
				  "--rebase-merges without an argument "
				  "instead, which does the same thing."));
			بازگشت 0;
		}
		parse_rebase_merges_مقدار(options, arg);
	}

	بازگشت 0;
}

ایستا تهی NORETURN error_on_missing_default_upstream(تهی)
{
	ساختار branch *current_branch = branch_get(پوچ);

	printf(_("%s\n"
		 "Please specify which branch you want to rebase against.\n"
		 "See git-rebase(1) for details.\n"
		 "\n"
		 "    git rebase '<branch>'\n"
		 "\n"),
		current_branch ? _("There is no tracking information for "
			"the current branch.") :
			_("You are not currently on a branch."));

	اگر (current_branch) {
		ثابت نویسه *remote = current_branch->remote_name;

		اگر (!remote)
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

ایستا صحیح check_exec_cmd(ثابت نویسه *cmd)
{
	اگر (strchr(cmd, '\n'))
		بازگشت error(_("exec commands cannot contain newlines"));

	/* Does the command consist purely of whitespace? */
	اگر (!cmd[strspn(cmd, " \t\r\f\v")])
		بازگشت error(_("empty exec command"));

	بازگشت 0;
}

صحیح cmd_rebase(صحیح argc,
	       ثابت نویسه **argv,
	       ثابت نویسه *prefix,
	       ساختار repository *repo UNUSED)
{
	ساختار rebase_options options = REBASE_OPTIONS_INIT;
	ثابت نویسه *branch_name;
	ثابت نویسه *strategy_opt = پوچ;
	صحیح ret, flags, total_argc, in_progress = 0;
	صحیح keep_base = 0;
	صحیح ok_to_skip_pre_rebase = 0;
	ساختار strbuf پیام = STRBUF_INIT;
	ساختار strbuf revisions = STRBUF_INIT;
	ساختار strbuf buf = STRBUF_INIT;
	ساختار object_id branch_base;
	صحیح ignore_whitespace = 0;
	ثابت نویسه *gpg_sign = پوچ;
	ساختار object_id squash_onto;
	نویسه *squash_onto_name = پوچ;
	نویسه *keep_base_onto_name = پوچ;
	صحیح reschedule_failed_exec = -1;
	صحیح allow_preemptive_ff = 1;
	صحیح preserve_merges_selected = 0;
	ساختار reset_working_درخت_options ropts = { 0 };
	ساختار option builtin_rebase_options[] = {
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
			.مقدار = &options.flags,
			.precision = اندازه(options.flags),
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
		OPT_PASSTHRU_ARGV('C', پوچ, &options.git_am_opts, N_("n"),
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
		OPT_CALLBACK_F(0, "apply", &options, پوچ,
			N_("use apply strategies to rebase"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			parse_opt_am),
		OPT_CALLBACK_F('m', "merge", &options, پوچ,
			N_("use merging strategies to rebase"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			parse_opt_merge),
		OPT_CALLBACK_F('i', "interactive", &options, پوچ,
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
		OPT_CALLBACK_F('k', "keep-empty", &options, پوچ,
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
			.مقدار = &gpg_sign,
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
		OPT_BOOL(0, "root", &options.ریشه,
			 N_("rebase all reachable commits up to the root(s)")),
		OPT_BOOL(0, "reschedule-failed-exec",
			 &reschedule_failed_exec,
			 N_("automatically re-schedule any `exec` that fails")),
		OPT_BOOL(0, "reapply-cherry-picks", &options.reapply_cherry_picks,
			 N_("apply all changes, even those already present upstream")),
		OPT_END(),
	};
	صحیح i;

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
	gpg_sign = options.gpg_sign_opt ? "" : پوچ;
	FREE_AND_NULL(options.gpg_sign_opt);

	strbuf_reset(&buf);
	strbuf_addf(&buf, "%s/applying", apply_dir());
	اگر(file_exists(buf.buf))
		die(_("It looks like 'git am' is in progress. Cannot rebase."));

	اگر (is_directory(apply_dir())) {
		options.type = REBASE_APPLY;
		options.state_dir = apply_dir();
	} وگرنه اگر (is_directory(merge_dir())) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "%s/rewritten", merge_dir());
		اگر (!(options.action == ACTION_ABORT) && is_directory(buf.buf)) {
			die(_("`rebase --preserve-merges` (-p) is no longer supported.\n"
			"Use `git rebase --abort` to terminate current rebase.\n"
			"Or downgrade to v2.33, or earlier, to complete the rebase."));
		} وگرنه {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "%s/interactive", merge_dir());
			options.type = REBASE_MERGE;
			اگر (file_exists(buf.buf))
				options.flags |= REBASE_INTERACTIVE_EXPLICIT;
		}
		options.state_dir = merge_dir();
	}

	اگر (options.type != REBASE_UNSPECIFIED)
		in_progress = 1;

	total_argc = argc;
	argc = parse_options(argc, argv, prefix,
			     builtin_rebase_options,
			     builtin_rebase_usage, 0);

	اگر (options.trailer_args.nr) {
		اگر (validate_trailer_args(&options.trailer_args))
			die(پوچ);
		options.flags |= REBASE_FORCE;
	}

	اگر (preserve_merges_selected)
		die(_("--preserve-merges was replaced by --rebase-merges\n"
			"Note: Your `pull.rebase` configuration may also be set to 'preserve',\n"
			"which is no longer supported; use 'merges' instead"));

	اگر (options.action != ACTION_NONE && total_argc != 2) {
		usage_with_options(builtin_rebase_usage,
				   builtin_rebase_options);
	}

	اگر (argc > 2)
		usage_with_options(builtin_rebase_usage,
				   builtin_rebase_options);

	اگر (keep_base) {
		اگر (options.onto_name)
			die(_("options '%s' and '%s' cannot be used together"), "--keep-base", "--onto");
		اگر (options.ریشه)
			die(_("options '%s' and '%s' cannot be used together"), "--keep-base", "--root");
		/*
		 * --keep-base defaults to --no-fork-point to keep the
		 * base the same.
		 */
		اگر (options.fork_point < 0)
			options.fork_point = 0;
	}
	اگر (options.ریشه && options.fork_point > 0)
		die(_("options '%s' and '%s' cannot be used together"), "--root", "--fork-point");

	اگر (options.action != ACTION_NONE && !in_progress)
		die(_("no rebase in progress"));

	اگر (options.action == ACTION_EDIT_TODO && !is_merge(&options))
		die(_("The --edit-todo action can only be used during "
		      "interactive rebase."));

	اگر (trace2_is_enabled()) {
		اگر (is_merge(&options))
			trace2_cmd_mode("interactive");
		وگرنه اگر (options.exec.nr)
			trace2_cmd_mode("interactive-exec");
		وگرنه
			trace2_cmd_mode(action_names[options.action]);
	}

	options.reflog_action = getenv(GIT_REFLOG_ACTION_ENVIRONMENT);
	options.reflog_action =
		xstrdup(options.reflog_action ? options.reflog_action : "rebase");

	انتخاب (options.action) {
	حالت ACTION_CONTINUE: {
		ساختار object_id head;
		ساختار lock_file lock_file = LOCK_INIT;
		صحیح fd;

		/* Sanity check */
		اگر (repo_get_oid(the_repository, "HEAD", &head))
			die(_("Cannot read HEAD"));

		fd = repo_hold_locked_index(the_repository, &lock_file, 0);
		اگر (repo_read_index(the_repository) < 0)
			die(_("could not read index"));
		refresh_index(the_repository->index, REFRESH_QUIET, پوچ, پوچ,
			      پوچ);
		اگر (0 <= fd)
			repo_update_index_if_able(the_repository, &lock_file);
		rollback_lock_file(&lock_file);

		اگر (has_unstaged_changes(the_repository, 1)) {
			puts(_("You must edit all merge conflicts and then\n"
			       "mark them as resolved using git add"));
			exit(1);
		}
		اگر (read_basic_state(&options))
			exit(1);
		برو run_rebase;
	}
	حالت ACTION_SKIP: {
		ساختار string_list merge_rr = STRING_LIST_INIT_DUP;

		rerere_clear(the_repository, &merge_rr);
		string_list_clear(&merge_rr, 1);
		ropts.flags = RESET_WORKING_درخت_HARD |
			      RESET_WORKING_درخت_UPDATE_HEAD;
		اگر (reset_working_درخت(the_repository, &ropts) < 0)
			die(_("could not discard worktree changes"));
		remove_branch_state(the_repository, 0);
		اگر (read_basic_state(&options))
			exit(1);
		برو run_rebase;
	}
	حالت ACTION_ABORT: {
		ساختار string_list merge_rr = STRING_LIST_INIT_DUP;
		ساختار strbuf head_پیام = STRBUF_INIT;

		rerere_clear(the_repository, &merge_rr);
		string_list_clear(&merge_rr, 1);

		اگر (read_basic_state(&options))
			exit(1);

		strbuf_addf(&head_پیام, "%s (abort): returning to %s",
			    options.reflog_action,
			    options.head_name ? options.head_name
					      : oid_to_hex(&options.orig_head->object.oid));
		ropts.oid = &options.orig_head->object.oid;
		ropts.head_پیام = head_پیام.buf;
		ropts.branch = options.head_name;
		ropts.flags = RESET_WORKING_درخت_HARD |
			      RESET_WORKING_درخت_UPDATE_HEAD;
		اگر (reset_working_درخت(the_repository, &ropts) < 0)
			die(_("could not move back to %s"),
			    oid_to_hex(&options.orig_head->object.oid));
		strbuf_release(&head_پیام);
		remove_branch_state(the_repository, 0);
		ret = finish_rebase(&options);
		برو cleanup;
	}
	حالت ACTION_QUIT: {
		save_autostash(state_dir_path("autostash", &options));
		اگر (options.type == REBASE_MERGE) {
			ساختار replay_opts replay = REPLAY_OPTS_INIT;

			replay.action = REPLAY_INTERACTIVE_REBASE;
			ret = sequencer_remove_state(&replay);
			replay_opts_release(&replay);
		} وگرنه {
			strbuf_reset(&buf);
			strbuf_addstr(&buf, options.state_dir);
			ret = remove_dir_recursively(&buf, 0);
			اگر (ret)
				error(_("could not remove '%s'"),
				       options.state_dir);
		}
		برو cleanup;
	}
	حالت ACTION_EDIT_TODO:
		options.dont_finish_rebase = 1;
		برو run_rebase;
	حالت ACTION_SHOW_CURRENT_PATCH:
		options.dont_finish_rebase = 1;
		برو run_rebase;
	حالت ACTION_NONE:
		بشکن;
	پیشفرض:
		BUG("action: %d", options.action);
	}

	/* Make sure no rebase is in progress */
	اگر (in_progress) {
		ثابت نویسه *last_slash = strrchr(options.state_dir, '/');
		ثابت نویسه *state_dir_base =
			last_slash ? last_slash + 1 : options.state_dir;
		ثابت نویسه *cmd_live_rebase =
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

	اگر ((options.flags & REBASE_INTERACTIVE_EXPLICIT) ||
	    (options.action != ACTION_NONE) ||
	    (options.exec.nr > 0) ||
	    options.autosquash == 1) {
		allow_preemptive_ff = 0;
	}
	اگر (options.committer_date_is_author_date || options.ignore_date)
		options.flags |= REBASE_FORCE;

	برای (i = 0; i < options.git_am_opts.nr; i++) {
		ثابت نویسه *option = options.git_am_opts.v[i], *p;
		اگر (!strcmp(option, "--whitespace=fix") ||
		    !strcmp(option, "--whitespace=strip"))
			allow_preemptive_ff = 0;
		وگرنه اگر (skip_prefix(option, "-C", &p)) {
			وقتی (*p)
				اگر (!isdigit(*(p++)))
					die(_("switch `C' expects a "
					      "numerical value"));
		} وگرنه اگر (skip_prefix(option, "--whitespace=", &p)) {
			اگر (*p && strcmp(p, "warn") && strcmp(p, "nowarn") &&
			    strcmp(p, "error") && strcmp(p, "error-all"))
				die("Invalid whitespace option: '%s'", p);
		}
	}

	برای (i = 0; i < options.exec.nr; i++)
		اگر (check_exec_cmd(options.exec.اقلام[i].string))
			exit(1);

	اگر (!(options.flags & REBASE_NO_QUIET))
		strvec_افزودن(&options.git_am_opts, "-q");

	اگر (options.empty != EMPTY_UNSPECIFIED)
		imply_merge(&options, "--empty");

	اگر (options.reapply_cherry_picks < 0)
		/*
		 * We default to --no-reapply-cherry-picks unless
		 * --keep-base is given; when --keep-base is given, we want
		 * to default to --reapply-cherry-picks.
		 */
		options.reapply_cherry_picks = keep_base;
	وگرنه اگر (!keep_base)
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

	اگر (gpg_sign)
		options.gpg_sign_opt = xstrfmt("-S%s", gpg_sign);

	اگر (options.exec.nr)
		imply_merge(&options, "--exec");

	اگر (options.type == REBASE_APPLY) {
		اگر (ignore_whitespace)
			strvec_افزودن(&options.git_am_opts,
				    "--ignore-whitespace");
		اگر (options.committer_date_is_author_date)
			strvec_افزودن(&options.git_am_opts,
				    "--committer-date-is-author-date");
		اگر (options.ignore_date)
			strvec_افزودن(&options.git_am_opts, "--ignore-date");
	} وگرنه {
		/* REBASE_MERGE */
		اگر (ignore_whitespace) {
			string_list_append(&options.strategy_opts,
					   "ignore-space-change");
		}
	}

	اگر (strategy_opt)
		options.strategy = xstrdup(strategy_opt);
	وگرنه اگر (options.strategy_opts.nr && !options.strategy)
		options.strategy = xstrdup("ort");
	اگر (options.strategy)
		imply_merge(&options, "--strategy");

	اگر (options.ریشه && !options.onto_name)
		imply_merge(&options, "--root without --onto");

	اگر (options.trailer_args.nr)
		imply_merge(&options, "--trailer");

	اگر (isatty(2) && options.flags & REBASE_NO_QUIET)
		strbuf_addstr(&options.git_format_patch_opt, " --progress");

	اگر (options.git_am_opts.nr || options.type == REBASE_APPLY) {
		/* all am options except -q are compatible only with --apply */
		برای (i = options.git_am_opts.nr - 1; i >= 0; i--)
			اگر (strcmp(options.git_am_opts.v[i], "-q"))
				بشکن;

		اگر (i >= 0 || options.type == REBASE_APPLY) {
			اگر (is_merge(&options))
				die(_("apply options and merge options "
					  "cannot be used together"));
			وگرنه اگر (options.rebase_merges == -1 && options.config_rebase_merges == 1)
				die(_("apply options are incompatible with rebase.rebaseMerges.  Consider adding --no-rebase-merges"));
			وگرنه اگر (options.update_refs == -1 && options.config_update_refs == 1)
				die(_("apply options are incompatible with rebase.updateRefs.  Consider adding --no-update-refs"));
			وگرنه
				options.type = REBASE_APPLY;
		}
	}

	اگر (options.update_refs == 1)
		imply_merge(&options, "--update-refs");
	options.update_refs = (options.update_refs >= 0) ? options.update_refs :
			     ((options.config_update_refs >= 0) ? options.config_update_refs : 0);

	اگر (options.rebase_merges == 1)
		imply_merge(&options, "--rebase-merges");
	options.rebase_merges = (options.rebase_merges >= 0) ? options.rebase_merges :
				((options.config_rebase_merges >= 0) ? options.config_rebase_merges : 0);

	اگر (options.autosquash == 1) {
		imply_merge(&options, "--autosquash");
	} وگرنه اگر (options.autosquash == -1) {
		options.autosquash =
			options.config_autosquash &&
			(options.flags & REBASE_INTERACTIVE_EXPLICIT);
	}

	اگر (options.type == REBASE_UNSPECIFIED) {
		اگر (!strcmp(options.default_backend, "merge"))
			options.type = REBASE_MERGE;
		وگرنه اگر (!strcmp(options.default_backend, "apply"))
			options.type = REBASE_APPLY;
		وگرنه
			die(_("Unknown rebase backend: %s"),
			    options.default_backend);
	}

	انتخاب (options.type) {
	حالت REBASE_MERGE:
		options.state_dir = merge_dir();
		بشکن;
	حالت REBASE_APPLY:
		options.state_dir = apply_dir();
		بشکن;
	پیشفرض:
		BUG("options.type was just set above; should be unreachable.");
	}

	اگر (options.empty == EMPTY_UNSPECIFIED) {
		اگر (options.flags & REBASE_INTERACTIVE_EXPLICIT)
			options.empty = EMPTY_STOP;
		وگرنه اگر (options.exec.nr > 0)
			options.empty = EMPTY_KEEP;
		وگرنه
			options.empty = EMPTY_DROP;
	}
	اگر (reschedule_failed_exec > 0 && !is_merge(&options))
		die(_("--reschedule-failed-exec requires "
		      "--exec or --interactive"));
	اگر (reschedule_failed_exec >= 0)
		options.reschedule_failed_exec = reschedule_failed_exec;

	اگر (options.signoff) {
		strvec_افزودن(&options.git_am_opts, "--signoff");
		options.flags |= REBASE_FORCE;
	}

	اگر (!options.ریشه) {
		اگر (argc < 1) {
			ساختار branch *branch;

			branch = branch_get(پوچ);
			options.upstream_name = branch_get_upstream(branch,
								    پوچ);
			اگر (!options.upstream_name)
				error_on_missing_default_upstream();
			اگر (options.fork_point < 0)
				options.fork_point = 1;
		} وگرنه {
			options.upstream_name = argv[0];
			argc--;
			argv++;
			اگر (!strcmp(options.upstream_name, "-"))
				options.upstream_name = "@{-1}";
		}
		options.upstream =
			lookup_commit_reference_by_name(options.upstream_name);
		اگر (!options.upstream)
			die(_("invalid upstream '%s'"), options.upstream_name);
		options.upstream_arg = options.upstream_name;
	} وگرنه {
		اگر (!options.onto_name) {
			اگر (commit_درخت("", 0, the_hash_algo->empty_درخت, پوچ,
					&squash_onto, پوچ, پوچ) < 0)
				die(_("Could not create new root commit"));
			options.squash_onto = &squash_onto;
			options.onto_name = squash_onto_name =
				xstrdup(oid_to_hex(&squash_onto));
		} وگرنه
			options.ریشه_with_onto = 1;

		options.upstream_name = پوچ;
		options.upstream = پوچ;
		اگر (argc > 1)
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
	اگر (argc == 1) {
		/* Is it "rebase other branchname" or "rebase other commit"? */
		ساختار object_id branch_oid;
		branch_name = argv[0];
		options.switch_to = argv[0];

		/* Is it a local branch? */
		strbuf_reset(&buf);
		strbuf_addf(&buf, "refs/heads/%s", branch_name);
		اگر (!refs_read_ref(get_main_ref_store(the_repository), buf.buf, &branch_oid)) {
			die_if_checked_out(buf.buf, 1);
			options.head_name = xstrdup(buf.buf);
			options.orig_head =
				lookup_commit_object(the_repository,
						     &branch_oid);
		/* If not is it a valid ref (branch or commit)? */
		} وگرنه {
			options.orig_head =
				lookup_commit_reference_by_name(branch_name);
			options.head_name = پوچ;
		}
		اگر (!options.orig_head)
			die(_("no such branch/commit '%s'"), branch_name);
	} وگرنه اگر (argc == 0) {
		/* Do not need to switch branches, we are already on it. */
		options.head_name =
			xstrdup_or_null(refs_resolve_ref_unsafe(get_main_ref_store(the_repository), "HEAD", 0, پوچ,
								&flags));
		اگر (!options.head_name)
			die(_("No such ref: %s"), "HEAD");
		اگر (flags & REF_ISSYMREF) {
			اگر (!skip_prefix(options.head_name,
					 "refs/heads/", &branch_name))
				branch_name = options.head_name;

		} وگرنه {
			FREE_AND_NULL(options.head_name);
			branch_name = "HEAD";
		}
		options.orig_head = lookup_commit_reference_by_name("HEAD");
		اگر (!options.orig_head)
			die(_("Could not resolve HEAD to a commit"));
	} وگرنه
		BUG("unexpected number of arguments left to parse");

	/* Make sure the branch to rebase onto is valid. */
	اگر (keep_base) {
		strbuf_reset(&buf);
		strbuf_addstr(&buf, options.upstream_name);
		strbuf_addstr(&buf, "...");
		strbuf_addstr(&buf, branch_name);
		options.onto_name = keep_base_onto_name = xstrdup(buf.buf);
	} وگرنه اگر (!options.onto_name)
		options.onto_name = options.upstream_name;
	اگر (strstr(options.onto_name, "...")) {
		اگر (repo_get_oid_mb(the_repository, options.onto_name, &branch_base) < 0) {
			اگر (keep_base)
				die(_("'%s': need exactly one merge base with branch"),
				    options.upstream_name);
			وگرنه
				die(_("'%s': need exactly one merge base"),
				    options.onto_name);
		}
		options.onto = lookup_commit_or_die(&branch_base,
						    options.onto_name);
	} وگرنه {
		options.onto =
			lookup_commit_reference_by_name(options.onto_name);
		اگر (!options.onto)
			die(_("Does not point to a valid commit '%s'"),
				options.onto_name);
		fill_branch_base(&options, &branch_base);
	}

	اگر (keep_base && options.reapply_cherry_picks)
		options.upstream = options.onto;

	اگر (options.fork_point > 0)
		options.restrict_revision =
			get_fork_point(options.upstream_name, options.orig_head);

	اگر (repo_read_index(the_repository) < 0)
		die(_("could not read index"));

	اگر (options.autostash)
		create_autostash(the_repository,
				 state_dir_path("autostash", &options));


	اگر (require_clean_work_درخت(the_repository, "rebase",
				    _("Please commit or stash them."), 1, 1)) {
		ret = -1;
		برو cleanup_autostash;
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
	اگر (allow_preemptive_ff &&
	    can_fast_forward(options.onto, options.upstream, options.restrict_revision,
			     options.orig_head, &branch_base)) {
		صحیح flag;

		اگر (!(options.flags & REBASE_FORCE)) {
			/* Lazily switch to the target branch if needed... */
			اگر (options.switch_to) {
				ret = checkout_up_to_date(&options);
				اگر (ret)
					برو cleanup_autostash;
			}

			اگر (!(options.flags & REBASE_NO_QUIET))
				; /* be quiet */
			وگرنه اگر (!strcmp(branch_name, "HEAD") &&
				 refs_resolve_ref_unsafe(get_main_ref_store(the_repository), "HEAD", 0, پوچ, &flag))
				puts(_("HEAD is up to date."));
			وگرنه
				printf(_("Current branch %s is up to date.\n"),
				       branch_name);
			ret = finish_rebase(&options);
			برو cleanup;
		} وگرنه اگر (!(options.flags & REBASE_NO_QUIET))
			; /* be quiet */
		وگرنه اگر (!strcmp(branch_name, "HEAD") &&
			 refs_resolve_ref_unsafe(get_main_ref_store(the_repository), "HEAD", 0, پوچ, &flag))
			puts(_("HEAD is up to date, rebase forced."));
		وگرنه
			printf(_("Current branch %s is up to date, rebase "
				 "forced.\n"), branch_name);
	}

	/* If a hook exists, give it a chance to interrupt*/
	اگر (!ok_to_skip_pre_rebase &&
	    run_hooks_l(the_repository, "pre-rebase", options.upstream_arg,
			argc ? argv[0] : پوچ, پوچ)) {
		ret = error(_("The pre-rebase hook refused to rebase."));
		برو cleanup_autostash;
	}

	اگر (options.flags & REBASE_DIFFSTAT) {
		ساختار diff_options opts;

		اگر (options.flags & REBASE_VERBOSE) {
			اگر (is_null_oid(&branch_base))
				printf(_("Changes to %s:\n"),
				       oid_to_hex(&options.onto->object.oid));
			وگرنه
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
		diff_درخت_oid(is_null_oid(&branch_base) ?
			      the_hash_algo->empty_درخت : &branch_base,
			      &options.onto->object.oid, "", &opts);
		diffcore_std(&opts);
		diff_flush(&opts);
	}

	اگر (is_merge(&options))
		برو run_rebase;

	/* Detach HEAD and reset the tree */
	اگر (options.flags & REBASE_NO_QUIET)
		printf(_("First, rewinding head to replay your work on top of "
			 "it...\n"));

	strbuf_addf(&پیام, "%s (start): checkout %s",
		    options.reflog_action, options.onto_name);
	ropts.oid = &options.onto->object.oid;
	ropts.orig_head = &options.orig_head->object.oid;
	ropts.flags = RESET_WORKING_درخت_DETACH |
		      RESET_WORKING_درخت_UPDATE_HEAD |
		      RESET_WORKING_درخت_UPDATE_ORIG_HEAD |
		      RESET_WORKING_درخت_RUN_POST_CHECKOUT_HOOK;
	ropts.head_پیام = پیام.buf;
	ropts.default_reflog_action = options.reflog_action;
	اگر (reset_working_درخت(the_repository, &ropts)) {
		ret = error(_("Could not detach HEAD"));
		برو cleanup_autostash;
	}

	/*
	 * If the onto is a proper descendant of the tip of the branch, then
	 * we just fast-forwarded.
	 */
	اگر (oideq(&branch_base, &options.orig_head->object.oid)) {
		printf(_("Fast-forwarded %s to %s.\n"),
			branch_name, options.onto_name);
		move_to_original_branch(&options);
		ret = finish_rebase(&options);
		برو cleanup;
	}

	strbuf_addf(&revisions, "%s..%s",
		    options.ریشه ? oid_to_hex(&options.onto->object.oid) :
		    (options.restrict_revision ?
		     oid_to_hex(&options.restrict_revision->object.oid) :
		     oid_to_hex(&options.upstream->object.oid)),
		    oid_to_hex(&options.orig_head->object.oid));

	options.revisions = revisions.buf;

run_rebase:
	ret = run_specific_rebase(&options);

cleanup:
	strbuf_release(&buf);
	strbuf_release(&پیام);
	strbuf_release(&revisions);
	rebase_options_release(&options);
	آزاد(squash_onto_name);
	آزاد(keep_base_onto_name);
	بازگشت !!ret;

cleanup_autostash:
	ret |= !!cleanup_autostash(&options);
	برو cleanup;
}
