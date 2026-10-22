/*
 * "git rebase" builtin command
 *
 * Copyright (c) 2018 Pratik Karki
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ಅನ್ತರ್ಭಾವ "builtin.h"

#ಅನ್ತರ್ಭಾವ "abspath.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "run-command.h"
#ಅನ್ತರ್ಭಾವ "strvec.h"
#ಅನ್ತರ್ಭಾವ "dir.h"
#ಅನ್ತರ್ಭಾವ "refs.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "unpack-trees.h"
#ಅನ್ತರ್ಭಾವ "lockfile.h"
#ಅನ್ತರ್ಭಾವ "object-file.h"
#ಅನ್ತರ್ಭಾವ "object-name.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "path.h"
#ಅನ್ತರ್ಭಾವ "commit.h"
#ಅನ್ತರ್ಭಾವ "diff.h"
#ಅನ್ತರ್ಭಾವ "wt-status.h"
#ಅನ್ತರ್ಭಾವ "revision.h"
#ಅನ್ತರ್ಭಾವ "commit-reach.h"
#ಅನ್ತರ್ಭಾವ "rerere.h"
#ಅನ್ತರ್ಭಾವ "branch.h"
#ಅನ್ತರ್ಭಾವ "sequencer.h"
#ಅನ್ತರ್ಭಾವ "rebase-interactive.h"
#ಅನ್ತರ್ಭಾವ "reset.h"
#ಅನ್ತರ್ಭಾವ "trace2.h"
#ಅನ್ತರ್ಭಾವ "hook.h"
#ಅನ್ತರ್ಭಾವ "trailer.h"

ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ * ಧ್ರುವ builtin_rebase_usage[] = {
	N_("git rebase [-i] [options] [--exec <cmd>] "
		"[--onto <newbase> | --keep-base] [<upstream> [<branch>]]"),
	N_("git rebase [-i] [options] [--exec <cmd>] [--onto <newbase>] "
		"--root [<branch>]"),
	"git rebase --continue | --abort | --skip | --edit-todo",
	ರಿಕ್ತ
};

ಸ್ಥಿರ GIT_PATH_FUNC(path_squash_onto, "rebase-merge/squash-onto")
ಸ್ಥಿರ GIT_PATH_FUNC(path_interactive, "rebase-merge/interactive")
ಸ್ಥಿರ GIT_PATH_FUNC(apply_dir, "rebase-apply")
ಸ್ಥಿರ GIT_PATH_FUNC(merge_dir, "rebase-merge")

ಗಣನಾ rebase_type {
	REBASE_UNSPECIFIED = -1,
	REBASE_APPLY,
	REBASE_MERGE
};

ಗಣನಾ empty_type {
	EMPTY_UNSPECIFIED = -1,
	EMPTY_DROP,
	EMPTY_KEEP,
	EMPTY_STOP
};

ಗಣನಾ action {
	ACTION_NONE = 0,
	ACTION_CONTINUE,
	ACTION_SKIP,
	ACTION_ABORT,
	ACTION_QUIT,
	ACTION_EDIT_TODO,
	ACTION_SHOW_CURRENT_PATCH
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *action_names[] = {
	"undefined",
	"continue",
	"skip",
	"abort",
	"quit",
	"edit_todo",
	"show_current_patch"
};

ಸಂರಚನಾ rebase_options {
	ಗಣನಾ rebase_type type;
	ಗಣನಾ empty_type empty;
	ವರ್ಣ *default_backend;
	ಧ್ರುವ ವರ್ಣ *state_dir;
	ಸಂರಚನಾ commit *upstream;
	ಧ್ರುವ ವರ್ಣ *upstream_name;
	ಧ್ರುವ ವರ್ಣ *upstream_arg;
	ವರ್ಣ *head_name;
	ಸಂರಚನಾ commit *orig_head;
	ಸಂರಚನಾ commit *onto;
	ಧ್ರುವ ವರ್ಣ *onto_name;
	ಧ್ರುವ ವರ್ಣ *revisions;
	ಧ್ರುವ ವರ್ಣ *switch_to;
	ಪೂರ್ಣಾಙ್ಕ ಬೇರು, ಬೇರು_with_onto;
	ಸಂರಚನಾ object_id *squash_onto;
	ಸಂರಚನಾ commit *restrict_revision;
	ಪೂರ್ಣಾಙ್ಕ dont_finish_rebase;
	ಗಣನಾ {
		REBASE_NO_QUIET = 1<<0,
		REBASE_VERBOSE = 1<<1,
		REBASE_DIFFSTAT = 1<<2,
		REBASE_FORCE = 1<<3,
		REBASE_INTERACTIVE_EXPLICIT = 1<<4,
	} flags;
	ಸಂರಚನಾ strvec git_am_opts;
	ಗಣನಾ action action;
	ವರ್ಣ *reflog_action;
	ಪೂರ್ಣಾಙ್ಕ signoff;
	ಸಂರಚನಾ strvec trailer_args;
	ಪೂರ್ಣಾಙ್ಕ allow_rerere_autoupdate;
	ಪೂರ್ಣಾಙ್ಕ keep_empty;
	ಪೂರ್ಣಾಙ್ಕ autosquash;
	ವರ್ಣ *gpg_sign_opt;
	ಪೂರ್ಣಾಙ್ಕ autostash;
	ಪೂರ್ಣಾಙ್ಕ committer_date_is_author_date;
	ಪೂರ್ಣಾಙ್ಕ ignore_date;
	ಸಂರಚನಾ string_list exec;
	ಪೂರ್ಣಾಙ್ಕ allow_empty_message;
	ಪೂರ್ಣಾಙ್ಕ rebase_merges, rebase_cousins;
	ವರ್ಣ *strategy;
	ಸಂರಚನಾ string_list strategy_opts;
	ಸಂರಚನಾ strbuf git_format_patch_opt;
	ಪೂರ್ಣಾಙ್ಕ reschedule_failed_exec;
	ಪೂರ್ಣಾಙ್ಕ reapply_cherry_picks;
	ಪೂರ್ಣಾಙ್ಕ fork_point;
	ಪೂರ್ಣಾಙ್ಕ update_refs;
	ಪೂರ್ಣಾಙ್ಕ config_autosquash;
	ಪೂರ್ಣಾಙ್ಕ config_rebase_merges;
	ಪೂರ್ಣಾಙ್ಕ config_update_refs;
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

ಸ್ಥಿರ ಶೂನ್ಯ rebase_options_release(ಸಂರಚನಾ rebase_options *opts)
{
	ಮುಕ್ತ(opts->default_backend);
	ಮುಕ್ತ(opts->reflog_action);
	ಮುಕ್ತ(opts->head_name);
	strvec_clear(&opts->git_am_opts);
	ಮುಕ್ತ(opts->gpg_sign_opt);
	string_list_clear(&opts->exec, 0);
	ಮುಕ್ತ(opts->strategy);
	string_list_clear(&opts->strategy_opts, 0);
	strbuf_release(&opts->git_format_patch_opt);
	strvec_clear(&opts->trailer_args);
}

ಸ್ಥಿರ ಸಂರಚನಾ replay_opts get_replay_opts(ಧ್ರುವ ಸಂರಚನಾ rebase_options *opts)
{
	ಸಂರಚನಾ replay_opts replay = REPLAY_OPTS_INIT;

	replay.action = REPLAY_INTERACTIVE_REBASE;
	replay.strategy = ರಿಕ್ತ;
	sequencer_init_config(&replay);

	replay.signoff = opts->signoff;

	strvec_pushv(&replay.trailer_args, opts->trailer_args.v);

	replay.allow_ff = !(opts->flags & REBASE_FORCE);
	ಯದಿ (opts->allow_rerere_autoupdate)
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
	ಮುಕ್ತ(replay.gpg_sign);
	replay.gpg_sign = xstrdup_or_null(opts->gpg_sign_opt);
	replay.reflog_action = xstrdup(opts->reflog_action);
	ಯದಿ (opts->strategy)
		replay.strategy = xstrdup_or_null(opts->strategy);
	ಅನ್ಯಥಾ ಯದಿ (!replay.strategy && replay.default_strategy) {
		replay.strategy = replay.default_strategy;
		replay.default_strategy = ರಿಕ್ತ;
	}

	ಪ್ರತಿ (size_t i = 0; i < opts->strategy_opts.nr; i++)
		strvec_ತಳ್ಳು(&replay.xopts, opts->strategy_opts.ವಸ್ತುಗಳು[i].string);

	ಯದಿ (opts->squash_onto) {
		oidcpy(&replay.squash_onto, opts->squash_onto);
		replay.have_squash_onto = 1;
	}

	ಪ್ರತ್ಯಾಗಮ replay;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ edit_todo_file(unsigned flags, ಸಂರಚನಾ replay_opts *opts)
{
	ಧ್ರುವ ವರ್ಣ *todo_file = rebase_path_todo();
	ಸಂರಚನಾ todo_list todo_list = TODO_LIST_INIT,
		new_todo = TODO_LIST_INIT;
	ಪೂರ್ಣಾಙ್ಕ res = 0;

	ಯದಿ (strbuf_read_file(&todo_list.buf, todo_file, 0) < 0)
		ಪ್ರತ್ಯಾಗಮ error_errno(_("could not read '%s'."), todo_file);

	strbuf_stripspace(&todo_list.buf, comment_ಸಾಲು_str);
	res = edit_todo_list(the_repository, opts, &todo_list, &new_todo,
			     ರಿಕ್ತ, ರಿಕ್ತ, flags);
	ಯದಿ (!res && todo_list_write_to_file(the_repository, &new_todo, todo_file,
					    ರಿಕ್ತ, ರಿಕ್ತ, -1, flags & ~(TODO_LIST_SHORTEN_IDS)))
		res = error_errno(_("could not write '%s'"), todo_file);

	todo_list_release(&todo_list);
	todo_list_release(&new_todo);

	ಪ್ರತ್ಯಾಗಮ res;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ get_revision_ranges(ಸಂರಚನಾ commit *upstream, ಸಂರಚನಾ commit *onto,
			       ಸಂರಚನಾ object_id *orig_head, ವರ್ಣ **revisions,
			       ವರ್ಣ **shortrevisions)
{
	ಸಂರಚನಾ commit *base_rev = upstream ? upstream : onto;
	ಧ್ರುವ ವರ್ಣ *shorthead;

	*revisions = xstrfmt("%s...%s", oid_to_hex(&base_rev->object.oid),
			     oid_to_hex(orig_head));

	shorthead = repo_find_unique_abbrev(the_repository, orig_head,
					    DEFAULT_ABBREV);

	ಯದಿ (upstream) {
		ಧ್ರುವ ವರ್ಣ *shortrev;

		shortrev = repo_find_unique_abbrev(the_repository,
						   &base_rev->object.oid,
						   DEFAULT_ABBREV);

		*shortrevisions = xstrfmt("%s..%s", shortrev, shorthead);
	} ಅನ್ಯಥಾ
		*shortrevisions = xstrdup(shorthead);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ init_basic_state(ಸಂರಚನಾ replay_opts *opts, ಧ್ರುವ ವರ್ಣ *head_name,
			    ಸಂರಚನಾ commit *onto,
			    ಧ್ರುವ ಸಂರಚನಾ object_id *orig_head)
{
	FILE *interactive;

	ಯದಿ (!is_directory(merge_dir()) &&
	    safe_create_dir_in_gitdir(the_repository, merge_dir()))
		ಪ್ರತ್ಯಾಗಮ error_errno(_("could not create temporary %s"), merge_dir());

	refs_delete_reflog(get_main_ref_store(the_repository), "REBASE_HEAD");

	interactive = fopen(path_interactive(), "w");
	ಯದಿ (!interactive)
		ಪ್ರತ್ಯಾಗಮ error_errno(_("could not mark as interactive"));
	fclose(interactive);

	ಪ್ರತ್ಯಾಗಮ write_basic_state(opts, head_name, onto, orig_head);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ do_interactive_rebase(ಸಂರಚನಾ rebase_options *opts, unsigned flags)
{
	ಪೂರ್ಣಾಙ್ಕ ret = -1;
	ವರ್ಣ *revisions = ರಿಕ್ತ, *shortrevisions = ರಿಕ್ತ;
	ಸಂರಚನಾ strvec make_script_args = STRVEC_INIT;
	ಸಂರಚನಾ todo_list todo_list = TODO_LIST_INIT;
	ಸಂರಚನಾ replay_opts replay = get_replay_opts(opts);

	ಯದಿ (get_revision_ranges(opts->upstream, opts->onto, &opts->orig_head->object.oid,
				&revisions, &shortrevisions))
		ಗಚ್ಛ cleanup;

	strvec_pushl(&make_script_args, "", revisions, ರಿಕ್ತ);
	ಯದಿ (opts->restrict_revision)
		strvec_pushf(&make_script_args, "^%s",
			     oid_to_hex(&opts->restrict_revision->object.oid));

	ret = sequencer_make_script(the_repository, &todo_list.buf,
				    &make_script_args, flags);
	ಯದಿ (ret) {
		error(_("could not generate todo list"));
		ಗಚ್ಛ cleanup;
	}

	ಯದಿ (init_basic_state(&replay,
			     opts->head_name ? opts->head_name : "detached HEAD",
			     opts->onto, &opts->orig_head->object.oid))
		ಗಚ್ಛ cleanup;

	ಯದಿ (!opts->upstream && opts->squash_onto)
		write_file(path_squash_onto(), "%s\n",
			   oid_to_hex(opts->squash_onto));

	discard_index(the_repository->index);
	ಯದಿ (todo_list_parse_insn_buffer(the_repository, &replay,
					todo_list.buf.buf, &todo_list))
		BUG("unusable todo list");

	ret = complete_action(the_repository, &replay, flags,
		shortrevisions, opts->onto_name, opts->onto,
		&opts->orig_head->object.oid, &opts->exec,
		opts->autosquash, opts->update_refs, &todo_list);

cleanup:
	replay_opts_release(&replay);
	ಮುಕ್ತ(revisions);
	ಮುಕ್ತ(shortrevisions);
	todo_list_release(&todo_list);
	strvec_clear(&make_script_args);

	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ run_sequencer_rebase(ಸಂರಚನಾ rebase_options *opts)
{
	unsigned flags = 0;
	ಪೂರ್ಣಾಙ್ಕ abbreviate_commands = 0, ret = 0;

	repo_config_get_bool(the_repository, "rebase.abbreviatecommands", &abbreviate_commands);

	flags |= opts->keep_empty ? TODO_LIST_KEEP_EMPTY : 0;
	flags |= abbreviate_commands ? TODO_LIST_ABBREVIATE_CMDS : 0;
	flags |= opts->rebase_merges ? TODO_LIST_REBASE_MERGES : 0;
	flags |= opts->rebase_cousins > 0 ? TODO_LIST_REBASE_COUSINS : 0;
	flags |= opts->ಬೇರು_with_onto ? TODO_LIST_ಬೇರು_WITH_ONTO : 0;
	flags |= opts->reapply_cherry_picks ? TODO_LIST_REAPPLY_CHERRY_PICKS : 0;
	flags |= opts->flags & REBASE_NO_QUIET ? TODO_LIST_WARN_SKIPPED_CHERRY_PICKS : 0;

	ವಿಕಲ್ಪ (opts->action) {
	ಪಕ್ಷ ACTION_NONE: {
		ಯದಿ (!opts->onto && !opts->upstream)
			die(_("a base commit must be provided with --upstream or --onto"));

		ret = do_interactive_rebase(opts, flags);
		ಭಙ್ಗ;
	}
	ಪಕ್ಷ ACTION_SKIP: {
		ಸಂರಚನಾ string_list merge_rr = STRING_LIST_INIT_DUP;

		rerere_clear(the_repository, &merge_rr);
	}
		/* fallthrough */
	ಪಕ್ಷ ACTION_CONTINUE: {
		ಸಂರಚನಾ replay_opts replay_opts = get_replay_opts(opts);

		ret = sequencer_continue(the_repository, &replay_opts);
		replay_opts_release(&replay_opts);
		ಭಙ್ಗ;
	}
	ಪಕ್ಷ ACTION_EDIT_TODO: {
		ಸಂರಚನಾ replay_opts replay_opts = get_replay_opts(opts);

		ret = edit_todo_file(flags, &replay_opts);
		replay_opts_release(&replay_opts);
		ಭಙ್ಗ;
	}
	ಪಕ್ಷ ACTION_SHOW_CURRENT_PATCH: {
		ಸಂರಚನಾ child_process cmd = CHILD_PROCESS_INIT;

		cmd.git_cmd = 1;
		strvec_pushl(&cmd.args, "show", "REBASE_HEAD", "--", ರಿಕ್ತ);
		ret = run_command(&cmd);

		ಭಙ್ಗ;
	}
	ಸ್ವಭಾವ:
		BUG("invalid command '%d'", opts->action);
	}

	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ is_merge(ಸಂರಚನಾ rebase_options *opts)
{
	ಪ್ರತ್ಯಾಗಮ opts->type == REBASE_MERGE;
}

ಸ್ಥಿರ ಶೂನ್ಯ imply_merge(ಸಂರಚನಾ rebase_options *opts, ಧ್ರುವ ವರ್ಣ *option)
{
	ವಿಕಲ್ಪ (opts->type) {
	ಪಕ್ಷ REBASE_APPLY:
		die(_("%s requires the merge backend"), option);
		ಭಙ್ಗ;
	ಪಕ್ಷ REBASE_MERGE:
		ಭಙ್ಗ;
	ಸ್ವಭಾವ:
		opts->type = REBASE_MERGE; /* implied */
		ಭಙ್ಗ;
	}
}

/* Returns the filename prefixed by the state_dir */
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *state_dir_path(ಧ್ರುವ ವರ್ಣ *filename, ಸಂರಚನಾ rebase_options *opts)
{
	ಸ್ಥಿರ ಸಂರಚನಾ strbuf path = STRBUF_INIT;
	ಸ್ಥಿರ size_t prefix_len;

	ಯದಿ (!prefix_len) {
		strbuf_addf(&path, "%s/", opts->state_dir);
		prefix_len = path.len;
	}

	strbuf_setlen(&path, prefix_len);
	strbuf_addstr(&path, filename);
	ಪ್ರತ್ಯಾಗಮ path.buf;
}

/* Initialize the rebase options from the state directory. */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ read_basic_state(ಸಂರಚನಾ rebase_options *opts)
{
	ಸಂರಚನಾ strbuf head_name = STRBUF_INIT;
	ಸಂರಚನಾ strbuf buf = STRBUF_INIT;
	ಸಂರಚನಾ object_id oid;

	ಯದಿ (!read_oneliner(&head_name, state_dir_path("head-name", opts),
			   READ_ONELINER_WARN_MISSING) ||
	    !read_oneliner(&buf, state_dir_path("onto", opts),
			   READ_ONELINER_WARN_MISSING))
		ಪ್ರತ್ಯಾಗಮ -1;
	opts->head_name = starts_with(head_name.buf, "refs/") ?
		xstrdup(head_name.buf) : ರಿಕ್ತ;
	strbuf_release(&head_name);
	ಯದಿ (get_oid_hex(buf.buf, &oid) ||
	    !(opts->onto = lookup_commit_object(the_repository, &oid)))
		ಪ್ರತ್ಯಾಗಮ error(_("invalid onto: '%s'"), buf.buf);

	/*
	 * We always write to orig-head, but interactive rebase used to write to
	 * head. Fall back to reading from head to cover for the case that the
	 * user upgraded git with an ongoing interactive rebase.
	 */
	strbuf_reset(&buf);
	ಯದಿ (file_exists(state_dir_path("orig-head", opts))) {
		ಯದಿ (!read_oneliner(&buf, state_dir_path("orig-head", opts),
				   READ_ONELINER_WARN_MISSING))
			ಪ್ರತ್ಯಾಗಮ -1;
	} ಅನ್ಯಥಾ ಯದಿ (!read_oneliner(&buf, state_dir_path("head", opts),
				  READ_ONELINER_WARN_MISSING))
		ಪ್ರತ್ಯಾಗಮ -1;
	ಯದಿ (get_oid_hex(buf.buf, &oid) ||
	    !(opts->orig_head = lookup_commit_object(the_repository, &oid)))
		ಪ್ರತ್ಯಾಗಮ error(_("invalid orig-head: '%s'"), buf.buf);

	ಯದಿ (file_exists(state_dir_path("quiet", opts)))
		opts->flags &= ~REBASE_NO_QUIET;
	ಅನ್ಯಥಾ
		opts->flags |= REBASE_NO_QUIET;

	ಯದಿ (file_exists(state_dir_path("verbose", opts)))
		opts->flags |= REBASE_VERBOSE;

	ಯದಿ (file_exists(state_dir_path("signoff", opts))) {
		opts->signoff = 1;
		opts->flags |= REBASE_FORCE;
	}

	ಯದಿ (file_exists(state_dir_path("allow_rerere_autoupdate", opts))) {
		strbuf_reset(&buf);
		ಯದಿ (!read_oneliner(&buf, state_dir_path("allow_rerere_autoupdate", opts),
				   READ_ONELINER_WARN_MISSING))
			ಪ್ರತ್ಯಾಗಮ -1;
		ಯದಿ (!strcmp(buf.buf, "--rerere-autoupdate"))
			opts->allow_rerere_autoupdate = RERERE_AUTOUPDATE;
		ಅನ್ಯಥಾ ಯದಿ (!strcmp(buf.buf, "--no-rerere-autoupdate"))
			opts->allow_rerere_autoupdate = RERERE_NOAUTOUPDATE;
		ಅನ್ಯಥಾ
			warning(_("ignoring invalid allow_rerere_autoupdate: "
				  "'%s'"), buf.buf);
	}

	ಯದಿ (file_exists(state_dir_path("gpg_sign_opt", opts))) {
		strbuf_reset(&buf);
		ಯದಿ (!read_oneliner(&buf, state_dir_path("gpg_sign_opt", opts),
				   READ_ONELINER_WARN_MISSING))
			ಪ್ರತ್ಯಾಗಮ -1;
		ಮುಕ್ತ(opts->gpg_sign_opt);
		opts->gpg_sign_opt = xstrdup(buf.buf);
	}

	strbuf_release(&buf);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ rebase_write_basic_state(ಸಂರಚನಾ rebase_options *opts)
{
	write_file(state_dir_path("head-name", opts), "%s",
		   opts->head_name ? opts->head_name : "detached HEAD");
	write_file(state_dir_path("onto", opts), "%s",
		   opts->onto ? oid_to_hex(&opts->onto->object.oid) : "");
	write_file(state_dir_path("orig-head", opts), "%s",
		   oid_to_hex(&opts->orig_head->object.oid));
	ಯದಿ (!(opts->flags & REBASE_NO_QUIET))
		write_file(state_dir_path("quiet", opts), "%s", "");
	ಯದಿ (opts->flags & REBASE_VERBOSE)
		write_file(state_dir_path("verbose", opts), "%s", "");
	ಯದಿ (opts->allow_rerere_autoupdate > 0)
		write_file(state_dir_path("allow_rerere_autoupdate", opts),
			   "-%s-rerere-autoupdate",
			   opts->allow_rerere_autoupdate == RERERE_AUTOUPDATE ?
				"" : "-no");
	ಯದಿ (opts->gpg_sign_opt)
		write_file(state_dir_path("gpg_sign_opt", opts), "%s",
			   opts->gpg_sign_opt);
	ಯದಿ (opts->signoff)
		write_file(state_dir_path("signoff", opts), "--signoff");

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cleanup_autostash(ಸಂರಚನಾ rebase_options *opts)
{
	ಪೂರ್ಣಾಙ್ಕ ret;
	ಸಂರಚನಾ strbuf dir = STRBUF_INIT;
	ಧ್ರುವ ವರ್ಣ *path = state_dir_path("autostash", opts);

	ಯದಿ (!file_exists(path))
		ಪ್ರತ್ಯಾಗಮ 0;
	ret = apply_autostash(path);
	strbuf_addstr(&dir, opts->state_dir);
	ಯದಿ (remove_dir_recursively(&dir, 0))
		ret = error_errno(_("could not remove '%s'"), opts->state_dir);
	strbuf_release(&dir);

	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ finish_rebase(ಸಂರಚನಾ rebase_options *opts)
{
	ಸಂರಚನಾ strbuf dir = STRBUF_INIT;
	ಪೂರ್ಣಾಙ್ಕ ret = 0;

	refs_delete_ref(get_main_ref_store(the_repository), ರಿಕ್ತ,
			"REBASE_HEAD", ರಿಕ್ತ, REF_NO_DEREF);
	refs_delete_ref(get_main_ref_store(the_repository), ರಿಕ್ತ,
			"AUTO_MERGE", ರಿಕ್ತ, REF_NO_DEREF);
	apply_autostash(state_dir_path("autostash", opts));
	/*
	 * We ignore errors in 'git maintenance run --auto', since the
	 * user should see them.
	 */
	run_auto_maintenance(the_repository,
			!(opts->flags & (REBASE_NO_QUIET|REBASE_VERBOSE)));

	ಯದಿ (opts->type == REBASE_MERGE) {
		ಸಂರಚನಾ replay_opts replay = REPLAY_OPTS_INIT;

		replay.action = REPLAY_INTERACTIVE_REBASE;
		ret = sequencer_remove_state(&replay);
		replay_opts_release(&replay);
	} ಅನ್ಯಥಾ {
		strbuf_addstr(&dir, opts->state_dir);
		ಯದಿ (remove_dir_recursively(&dir, 0))
			ret = error(_("could not remove '%s'"),
				    opts->state_dir);
		strbuf_release(&dir);
	}

	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ move_to_original_branch(ಸಂರಚನಾ rebase_options *opts)
{
	ಸಂರಚನಾ strbuf branch_reflog = STRBUF_INIT, head_reflog = STRBUF_INIT;
	ಸಂರಚನಾ reset_working_ವೃಕ್ಷ_options ropts = { 0 };
	ಪೂರ್ಣಾಙ್ಕ ret;

	ಯದಿ (!opts->head_name)
		ಪ್ರತ್ಯಾಗಮ 0; /* nothing to move back to */

	ಯದಿ (!opts->onto)
		BUG("move_to_original_branch without onto");

	strbuf_addf(&branch_reflog, "%s (finish): %s onto %s",
		    opts->reflog_action,
		    opts->head_name, oid_to_hex(&opts->onto->object.oid));
	strbuf_addf(&head_reflog, "%s (finish): returning to %s",
		    opts->reflog_action, opts->head_name);
	ropts.branch = opts->head_name;
	ropts.flags = RESET_WORKING_ವೃಕ್ಷ_REFS_ONLY |
		      RESET_WORKING_ವೃಕ್ಷ_UPDATE_HEAD;
	ropts.branch_ಸಂದೇಶ = branch_reflog.buf;
	ropts.head_ಸಂದೇಶ = head_reflog.buf;
	ret = reset_working_ವೃಕ್ಷ(the_repository, &ropts);

	strbuf_release(&branch_reflog);
	strbuf_release(&head_reflog);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ run_am(ಸಂರಚನಾ rebase_options *opts)
{
	ಸಂರಚನಾ child_process am = CHILD_PROCESS_INIT;
	ಸಂರಚನಾ child_process format_patch = CHILD_PROCESS_INIT;
	ಪೂರ್ಣಾಙ್ಕ status;
	ವರ್ಣ *rebased_patches;

	am.git_cmd = 1;
	strvec_ತಳ್ಳು(&am.args, "am");
	strvec_pushf(&am.env, GIT_REFLOG_ACTION_ENVIRONMENT "=%s (pick)",
		     opts->reflog_action);
	ಯದಿ (opts->action == ACTION_CONTINUE) {
		strvec_ತಳ್ಳು(&am.args, "--resolved");
		strvec_pushf(&am.args, "--resolvemsg=%s", rebase_resolvemsg);
		ಯದಿ (opts->gpg_sign_opt)
			strvec_ತಳ್ಳು(&am.args, opts->gpg_sign_opt);
		status = run_command(&am);
		ಯದಿ (status)
			ಪ್ರತ್ಯಾಗಮ status;

		ಪ್ರತ್ಯಾಗಮ move_to_original_branch(opts);
	}
	ಯದಿ (opts->action == ACTION_SKIP) {
		strvec_ತಳ್ಳು(&am.args, "--skip");
		strvec_pushf(&am.args, "--resolvemsg=%s", rebase_resolvemsg);
		status = run_command(&am);
		ಯದಿ (status)
			ಪ್ರತ್ಯಾಗಮ status;

		ಪ್ರತ್ಯಾಗಮ move_to_original_branch(opts);
	}
	ಯದಿ (opts->action == ACTION_SHOW_CURRENT_PATCH) {
		strvec_ತಳ್ಳು(&am.args, "--show-current-patch");
		ಪ್ರತ್ಯಾಗಮ run_command(&am);
	}

	rebased_patches = repo_git_path(the_repository, "rebased-patches");
	format_patch.out = open(rebased_patches,
				O_WRONLY | O_CREAT | O_TRUNC, 0666);
	ಯದಿ (format_patch.out < 0) {
		status = error_errno(_("could not open '%s' for writing"),
				     rebased_patches);
		ಮುಕ್ತ(rebased_patches);
		child_process_clear(&am);
		ಪ್ರತ್ಯಾಗಮ status;
	}

	format_patch.git_cmd = 1;
	strvec_pushl(&format_patch.args, "format-patch", "-k", "--stdout",
		     "--full-index", "--cherry-pick", "--right-only",
		     "--default-prefix", "--no-renames",
		     "--no-cover-letter", "--pretty=mboxrd", "--topo-order",
		     "--no-base", ರಿಕ್ತ);
	ಯದಿ (opts->git_format_patch_opt.len)
		strvec_split(&format_patch.args,
			     opts->git_format_patch_opt.buf);
	strvec_pushf(&format_patch.args, "%s...%s",
		     oid_to_hex(opts->ಬೇರು ?
				/* this is now equivalent to !opts->upstream */
				&opts->onto->object.oid :
				&opts->upstream->object.oid),
		     oid_to_hex(&opts->orig_head->object.oid));
	ಯದಿ (opts->restrict_revision)
		strvec_pushf(&format_patch.args, "^%s",
			     oid_to_hex(&opts->restrict_revision->object.oid));

	status = run_command(&format_patch);
	ಯದಿ (status) {
		ಸಂರಚನಾ reset_working_ವೃಕ್ಷ_options ropts = { 0 };
		unlink(rebased_patches);
		ಮುಕ್ತ(rebased_patches);
		child_process_clear(&am);

		ropts.oid = &opts->orig_head->object.oid;
		ropts.branch = opts->head_name;
		ropts.default_reflog_action = opts->reflog_action;
		ropts.flags = RESET_WORKING_ವೃಕ್ಷ_UPDATE_HEAD;
		reset_working_ವೃಕ್ಷ(the_repository, &ropts);
		error(_("\ngit encountered an error while preparing the "
			"patches to replay\n"
			"these revisions:\n"
			"\n    %s\n\n"
			"As a result, git cannot rebase them."),
		      opts->revisions);

		ಪ್ರತ್ಯಾಗಮ status;
	}

	am.in = open(rebased_patches, O_RDONLY);
	ಯದಿ (am.in < 0) {
		status = error_errno(_("could not open '%s' for reading"),
				     rebased_patches);
		ಮುಕ್ತ(rebased_patches);
		child_process_clear(&am);
		ಪ್ರತ್ಯಾಗಮ status;
	}

	strvec_pushv(&am.args, opts->git_am_opts.v);
	strvec_ತಳ್ಳು(&am.args, "--rebasing");
	strvec_pushf(&am.args, "--resolvemsg=%s", rebase_resolvemsg);
	strvec_ತಳ್ಳು(&am.args, "--patch-format=mboxrd");
	ಯದಿ (opts->allow_rerere_autoupdate == RERERE_AUTOUPDATE)
		strvec_ತಳ್ಳು(&am.args, "--rerere-autoupdate");
	ಅನ್ಯಥಾ ಯದಿ (opts->allow_rerere_autoupdate == RERERE_NOAUTOUPDATE)
		strvec_ತಳ್ಳು(&am.args, "--no-rerere-autoupdate");
	ಯದಿ (opts->gpg_sign_opt)
		strvec_ತಳ್ಳು(&am.args, opts->gpg_sign_opt);
	status = run_command(&am);
	unlink(rebased_patches);
	ಮುಕ್ತ(rebased_patches);

	ಯದಿ (!status) {
		ಪ್ರತ್ಯಾಗಮ move_to_original_branch(opts);
	}

	ಯದಿ (is_directory(opts->state_dir))
		rebase_write_basic_state(opts);

	ಪ್ರತ್ಯಾಗಮ status;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ run_specific_rebase(ಸಂರಚನಾ rebase_options *opts)
{
	ಪೂರ್ಣಾಙ್ಕ status;

	ಯದಿ (opts->type == REBASE_MERGE) {
		/* Run sequencer-based rebase */
		ಯದಿ (!(opts->flags & REBASE_INTERACTIVE_EXPLICIT))
			setenv("GIT_SEQUENCE_EDITOR", ":", 1);
		ಯದಿ (opts->gpg_sign_opt) {
			/* remove the leading "-S" */
			ವರ್ಣ *tmp = xstrdup(opts->gpg_sign_opt + 2);
			ಮುಕ್ತ(opts->gpg_sign_opt);
			opts->gpg_sign_opt = tmp;
		}

		status = run_sequencer_rebase(opts);
	} ಅನ್ಯಥಾ ಯದಿ (opts->type == REBASE_APPLY)
		status = run_am(opts);
	ಅನ್ಯಥಾ
		BUG("Unhandled rebase type %d", opts->type);

	ಯದಿ (opts->dont_finish_rebase)
		; /* do nothing */
	ಅನ್ಯಥಾ ಯದಿ (opts->type == REBASE_MERGE) {
		ಪೂರ್ಣಾಙ್ಕ quiet = !(opts->flags & (REBASE_NO_QUIET|REBASE_VERBOSE));

		/*
		 * The sequencer cleans up after itself. Its state directory
		 * is gone once it is done, and stays while it is stopped.
		 */
		ಯದಿ (status == 0 && !is_directory(opts->state_dir))
			run_auto_maintenance(the_repository, quiet);
	} ಅನ್ಯಥಾ ಯದಿ (status == 0) {
		ಯದಿ (!file_exists(state_dir_path("stopped-sha", opts)))
			finish_rebase(opts);
	} ಅನ್ಯಥಾ ಯದಿ (status == 2) {
		ಸಂರಚನಾ strbuf dir = STRBUF_INIT;

		apply_autostash(state_dir_path("autostash", opts));
		strbuf_addstr(&dir, opts->state_dir);
		remove_dir_recursively(&dir, 0);
		strbuf_release(&dir);
		die("Nothing to do");
	}

	ಪ್ರತ್ಯಾಗಮ status ? -1 : 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_rebase_merges_ಮೌಲ್ಯ(ಸಂರಚನಾ rebase_options *options, ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ)
{
	ಯದಿ (!strcmp("no-rebase-cousins", ಮೌಲ್ಯ))
		options->rebase_cousins = 0;
	ಅನ್ಯಥಾ ಯದಿ (!strcmp("rebase-cousins", ಮೌಲ್ಯ))
		options->rebase_cousins = 1;
	ಅನ್ಯಥಾ
		die(_("Unknown rebase-merges mode: %s"), ಮೌಲ್ಯ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ rebase_config(ಧ್ರುವ ವರ್ಣ *var, ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ,
			 ಧ್ರುವ ಸಂರಚನಾ config_context *ctx, ಶೂನ್ಯ *data)
{
	ಸಂರಚನಾ rebase_options *opts = data;

	ಯದಿ (!strcmp(var, "rebase.stat")) {
		ಯದಿ (git_config_bool(var, ಮೌಲ್ಯ))
			opts->flags |= REBASE_DIFFSTAT;
		ಅನ್ಯಥಾ
			opts->flags &= ~REBASE_DIFFSTAT;
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (!strcmp(var, "rebase.autosquash")) {
		opts->config_autosquash = git_config_bool(var, ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (!strcmp(var, "commit.gpgsign")) {
		ಮುಕ್ತ(opts->gpg_sign_opt);
		opts->gpg_sign_opt = git_config_bool(var, ಮೌಲ್ಯ) ?
			xstrdup("-S") : ರಿಕ್ತ;
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (!strcmp(var, "rebase.autostash")) {
		opts->autostash = git_config_bool(var, ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (!strcmp(var, "rebase.rebasemerges")) {
		opts->config_rebase_merges = git_parse_maybe_bool(ಮೌಲ್ಯ);
		ಯದಿ (opts->config_rebase_merges < 0) {
			opts->config_rebase_merges = 1;
			parse_rebase_merges_ಮೌಲ್ಯ(opts, ಮೌಲ್ಯ);
		} ಅನ್ಯಥಾ {
			opts->rebase_cousins = 0;
		}
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (!strcmp(var, "rebase.updaterefs")) {
		opts->config_update_refs = git_config_bool(var, ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (!strcmp(var, "rebase.reschedulefailedexec")) {
		opts->reschedule_failed_exec = git_config_bool(var, ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (!strcmp(var, "rebase.forkpoint")) {
		opts->fork_point = git_config_bool(var, ಮೌಲ್ಯ) ? -1 : 0;
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (!strcmp(var, "rebase.backend")) {
		FREE_AND_NULL(opts->default_backend);
		ಪ್ರತ್ಯಾಗಮ git_config_string(&opts->default_backend, var, ಮೌಲ್ಯ);
	}

	ಪ್ರತ್ಯಾಗಮ git_default_config(var, ಮೌಲ್ಯ, ctx, data);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ checkout_up_to_date(ಸಂರಚನಾ rebase_options *options)
{
	ಸಂರಚನಾ strbuf buf = STRBUF_INIT;
	ಸಂರಚನಾ reset_working_ವೃಕ್ಷ_options ropts = { 0 };
	ಪೂರ್ಣಾಙ್ಕ ret = 0;

	strbuf_addf(&buf, "%s: checkout %s",
		    options->reflog_action, options->switch_to);
	ropts.oid = &options->orig_head->object.oid;
	ropts.branch = options->head_name;
	ropts.flags = RESET_WORKING_ವೃಕ್ಷ_RUN_POST_CHECKOUT_HOOK |
		      RESET_WORKING_ವೃಕ್ಷ_UPDATE_HEAD;
	ಯದಿ (!ropts.branch)
		ropts.flags |=  RESET_WORKING_ವೃಕ್ಷ_DETACH;
	ropts.head_ಸಂದೇಶ = buf.buf;
	ಯದಿ (reset_working_ವೃಕ್ಷ(the_repository, &ropts) < 0)
		ret = error(_("could not switch to %s"), options->switch_to);
	strbuf_release(&buf);

	ಪ್ರತ್ಯಾಗಮ ret;
}

/*
 * Determines whether the commits in from..to are linear, i.e. contain
 * no merge commits. This function *expects* `from` to be an ancestor of
 * `to`.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ is_linear_history(ಸಂರಚನಾ commit *from, ಸಂರಚನಾ commit *to)
{
	ಯಾವತ್ (to && to != from) {
		repo_parse_commit(the_repository, to);
		ಯದಿ (!to->parents)
			ಪ್ರತ್ಯಾಗಮ 1;
		ಯದಿ (to->parents->next)
			ಪ್ರತ್ಯಾಗಮ 0;
		to = to->parents->item;
	}
	ಪ್ರತ್ಯಾಗಮ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ can_fast_forward(ಸಂರಚನಾ commit *onto, ಸಂರಚನಾ commit *upstream,
			    ಸಂರಚನಾ commit *restrict_revision,
			    ಸಂರಚನಾ commit *head, ಸಂರಚನಾ object_id *branch_base)
{
	ಸಂರಚನಾ commit_list *merge_bases = ರಿಕ್ತ;
	ಪೂರ್ಣಾಙ್ಕ res = 0;

	ಯದಿ (is_null_oid(branch_base))
		ಗಚ್ಛ done; /* fill_branch_base() found multiple merge bases */

	ಯದಿ (!oideq(branch_base, &onto->object.oid))
		ಗಚ್ಛ done;

	ಯದಿ (restrict_revision && !oideq(&restrict_revision->object.oid, branch_base))
		ಗಚ್ಛ done;

	ಯದಿ (!upstream)
		ಗಚ್ಛ done;

	ಯದಿ (repo_get_merge_bases(the_repository, upstream, head, &merge_bases) < 0)
		exit(128);
	ಯದಿ (!merge_bases || merge_bases->next)
		ಗಚ್ಛ done;

	ಯದಿ (!oideq(&onto->object.oid, &merge_bases->item->object.oid))
		ಗಚ್ಛ done;

	res = 1;

done:
	commit_list_free(merge_bases);
	ಪ್ರತ್ಯಾಗಮ res && is_linear_history(onto, head);
}

ಸ್ಥಿರ ಶೂನ್ಯ fill_branch_base(ಸಂರಚನಾ rebase_options *options,
			    ಸಂರಚನಾ object_id *branch_base)
{
	ಸಂರಚನಾ commit_list *merge_bases = ರಿಕ್ತ;

	ಯದಿ (repo_get_merge_bases(the_repository, options->onto,
				 options->orig_head, &merge_bases) < 0)
		exit(128);
	ಯದಿ (!merge_bases || merge_bases->next)
		oidcpy(branch_base, null_oid(the_hash_algo));
	ಅನ್ಯಥಾ
		oidcpy(branch_base, &merge_bases->item->object.oid);

	commit_list_free(merge_bases);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ parse_opt_am(ಧ್ರುವ ಸಂರಚನಾ option *opt, ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಸಂರಚನಾ rebase_options *opts = opt->ಮೌಲ್ಯ;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	ಯದಿ (opts->type != REBASE_UNSPECIFIED && opts->type != REBASE_APPLY)
	    die(_("apply options and merge options cannot be used together"));

	opts->type = REBASE_APPLY;

	ಪ್ರತ್ಯಾಗಮ 0;
}

/* -i followed by -m is still -i */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ parse_opt_merge(ಧ್ರುವ ಸಂರಚನಾ option *opt, ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಸಂರಚನಾ rebase_options *opts = opt->ಮೌಲ್ಯ;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	ಯದಿ (opts->type != REBASE_UNSPECIFIED && opts->type != REBASE_MERGE)
	    die(_("apply options and merge options cannot be used together"));

	opts->type = REBASE_MERGE;

	ಪ್ರತ್ಯಾಗಮ 0;
}

/* -i followed by -r is still explicitly interactive, but -r alone is not */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ parse_opt_interactive(ಧ್ರುವ ಸಂರಚನಾ option *opt, ಧ್ರುವ ವರ್ಣ *arg,
				 ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಸಂರಚನಾ rebase_options *opts = opt->ಮೌಲ್ಯ;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	ಯದಿ (opts->type != REBASE_UNSPECIFIED && opts->type != REBASE_MERGE)
	    die(_("apply options and merge options cannot be used together"));

	opts->type = REBASE_MERGE;
	opts->flags |= REBASE_INTERACTIVE_EXPLICIT;

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಗಣನಾ empty_type parse_empty_ಮೌಲ್ಯ(ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ)
{
	ಯದಿ (!strcasecmp(ಮೌಲ್ಯ, "drop"))
		ಪ್ರತ್ಯಾಗಮ EMPTY_DROP;
	ಅನ್ಯಥಾ ಯದಿ (!strcasecmp(ಮೌಲ್ಯ, "keep"))
		ಪ್ರತ್ಯಾಗಮ EMPTY_KEEP;
	ಅನ್ಯಥಾ ಯದಿ (!strcasecmp(ಮೌಲ್ಯ, "stop"))
		ಪ್ರತ್ಯಾಗಮ EMPTY_STOP;
	ಅನ್ಯಥಾ ಯದಿ (!strcasecmp(ಮೌಲ್ಯ, "ask")) {
		warning(_("--empty=ask is deprecated; use '--empty=stop' instead."));
		ಪ್ರತ್ಯಾಗಮ EMPTY_STOP;
	}

	die(_("unrecognized empty type '%s'; valid values are \"drop\", \"keep\", and \"stop\"."), ಮೌಲ್ಯ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ parse_opt_keep_empty(ಧ್ರುವ ಸಂರಚನಾ option *opt, ಧ್ರುವ ವರ್ಣ *arg,
				ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಸಂರಚನಾ rebase_options *opts = opt->ಮೌಲ್ಯ;

	BUG_ON_OPT_ARG(arg);

	imply_merge(opts, unset ? "--no-keep-empty" : "--keep-empty");
	opts->keep_empty = !unset;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ parse_opt_empty(ಧ್ರುವ ಸಂರಚನಾ option *opt, ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಸಂರಚನಾ rebase_options *options = opt->ಮೌಲ್ಯ;
	ಗಣನಾ empty_type ಮೌಲ್ಯ = parse_empty_ಮೌಲ್ಯ(arg);

	BUG_ON_OPT_NEG(unset);

	options->empty = ಮೌಲ್ಯ;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ parse_opt_rebase_merges(ಧ್ರುವ ಸಂರಚನಾ option *opt, ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಸಂರಚನಾ rebase_options *options = opt->ಮೌಲ್ಯ;

	options->rebase_merges = !unset;
	options->rebase_cousins = 0;

	ಯದಿ (arg) {
		ಯದಿ (!*arg) {
			warning(_("--rebase-merges with an empty string "
				  "argument is deprecated and will stop "
				  "working in a future version of Git. Use "
				  "--rebase-merges without an argument "
				  "instead, which does the same thing."));
			ಪ್ರತ್ಯಾಗಮ 0;
		}
		parse_rebase_merges_ಮೌಲ್ಯ(options, arg);
	}

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ NORETURN error_on_missing_default_upstream(ಶೂನ್ಯ)
{
	ಸಂರಚನಾ branch *current_branch = branch_get(ರಿಕ್ತ);

	printf(_("%s\n"
		 "Please specify which branch you want to rebase against.\n"
		 "See git-rebase(1) for details.\n"
		 "\n"
		 "    git rebase '<branch>'\n"
		 "\n"),
		current_branch ? _("There is no tracking information for "
			"the current branch.") :
			_("You are not currently on a branch."));

	ಯದಿ (current_branch) {
		ಧ್ರುವ ವರ್ಣ *remote = current_branch->remote_name;

		ಯದಿ (!remote)
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

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ check_exec_cmd(ಧ್ರುವ ವರ್ಣ *cmd)
{
	ಯದಿ (strchr(cmd, '\n'))
		ಪ್ರತ್ಯಾಗಮ error(_("exec commands cannot contain newlines"));

	/* Does the command consist purely of whitespace? */
	ಯದಿ (!cmd[strspn(cmd, " \t\r\f\v")])
		ಪ್ರತ್ಯಾಗಮ error(_("empty exec command"));

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ cmd_rebase(ಪೂರ್ಣಾಙ್ಕ argc,
	       ಧ್ರುವ ವರ್ಣ **argv,
	       ಧ್ರುವ ವರ್ಣ *prefix,
	       ಸಂರಚನಾ repository *repo UNUSED)
{
	ಸಂರಚನಾ rebase_options options = REBASE_OPTIONS_INIT;
	ಧ್ರುವ ವರ್ಣ *branch_name;
	ಧ್ರುವ ವರ್ಣ *strategy_opt = ರಿಕ್ತ;
	ಪೂರ್ಣಾಙ್ಕ ret, flags, total_argc, in_progress = 0;
	ಪೂರ್ಣಾಙ್ಕ keep_base = 0;
	ಪೂರ್ಣಾಙ್ಕ ok_to_skip_pre_rebase = 0;
	ಸಂರಚನಾ strbuf ಸಂದೇಶ = STRBUF_INIT;
	ಸಂರಚನಾ strbuf revisions = STRBUF_INIT;
	ಸಂರಚನಾ strbuf buf = STRBUF_INIT;
	ಸಂರಚನಾ object_id branch_base;
	ಪೂರ್ಣಾಙ್ಕ ignore_whitespace = 0;
	ಧ್ರುವ ವರ್ಣ *gpg_sign = ರಿಕ್ತ;
	ಸಂರಚನಾ object_id squash_onto;
	ವರ್ಣ *squash_onto_name = ರಿಕ್ತ;
	ವರ್ಣ *keep_base_onto_name = ರಿಕ್ತ;
	ಪೂರ್ಣಾಙ್ಕ reschedule_failed_exec = -1;
	ಪೂರ್ಣಾಙ್ಕ allow_preemptive_ff = 1;
	ಪೂರ್ಣಾಙ್ಕ preserve_merges_selected = 0;
	ಸಂರಚನಾ reset_working_ವೃಕ್ಷ_options ropts = { 0 };
	ಸಂರಚನಾ option builtin_rebase_options[] = {
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
			.ಮೌಲ್ಯ = &options.flags,
			.precision = ಪರಿಮಾಣ(options.flags),
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
		OPT_PASSTHRU_ARGV('C', ರಿಕ್ತ, &options.git_am_opts, N_("n"),
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
		OPT_CALLBACK_F(0, "apply", &options, ರಿಕ್ತ,
			N_("use apply strategies to rebase"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			parse_opt_am),
		OPT_CALLBACK_F('m', "merge", &options, ರಿಕ್ತ,
			N_("use merging strategies to rebase"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			parse_opt_merge),
		OPT_CALLBACK_F('i', "interactive", &options, ರಿಕ್ತ,
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
		OPT_CALLBACK_F('k', "keep-empty", &options, ರಿಕ್ತ,
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
			.ಮೌಲ್ಯ = &gpg_sign,
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
		OPT_BOOL(0, "root", &options.ಬೇರು,
			 N_("rebase all reachable commits up to the root(s)")),
		OPT_BOOL(0, "reschedule-failed-exec",
			 &reschedule_failed_exec,
			 N_("automatically re-schedule any `exec` that fails")),
		OPT_BOOL(0, "reapply-cherry-picks", &options.reapply_cherry_picks,
			 N_("apply all changes, even those already present upstream")),
		OPT_END(),
	};
	ಪೂರ್ಣಾಙ್ಕ i;

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
	gpg_sign = options.gpg_sign_opt ? "" : ರಿಕ್ತ;
	FREE_AND_NULL(options.gpg_sign_opt);

	strbuf_reset(&buf);
	strbuf_addf(&buf, "%s/applying", apply_dir());
	ಯದಿ(file_exists(buf.buf))
		die(_("It looks like 'git am' is in progress. Cannot rebase."));

	ಯದಿ (is_directory(apply_dir())) {
		options.type = REBASE_APPLY;
		options.state_dir = apply_dir();
	} ಅನ್ಯಥಾ ಯದಿ (is_directory(merge_dir())) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "%s/rewritten", merge_dir());
		ಯದಿ (!(options.action == ACTION_ABORT) && is_directory(buf.buf)) {
			die(_("`rebase --preserve-merges` (-p) is no longer supported.\n"
			"Use `git rebase --abort` to terminate current rebase.\n"
			"Or downgrade to v2.33, or earlier, to complete the rebase."));
		} ಅನ್ಯಥಾ {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "%s/interactive", merge_dir());
			options.type = REBASE_MERGE;
			ಯದಿ (file_exists(buf.buf))
				options.flags |= REBASE_INTERACTIVE_EXPLICIT;
		}
		options.state_dir = merge_dir();
	}

	ಯದಿ (options.type != REBASE_UNSPECIFIED)
		in_progress = 1;

	total_argc = argc;
	argc = parse_options(argc, argv, prefix,
			     builtin_rebase_options,
			     builtin_rebase_usage, 0);

	ಯದಿ (options.trailer_args.nr) {
		ಯದಿ (validate_trailer_args(&options.trailer_args))
			die(ರಿಕ್ತ);
		options.flags |= REBASE_FORCE;
	}

	ಯದಿ (preserve_merges_selected)
		die(_("--preserve-merges was replaced by --rebase-merges\n"
			"Note: Your `pull.rebase` configuration may also be set to 'preserve',\n"
			"which is no longer supported; use 'merges' instead"));

	ಯದಿ (options.action != ACTION_NONE && total_argc != 2) {
		usage_with_options(builtin_rebase_usage,
				   builtin_rebase_options);
	}

	ಯದಿ (argc > 2)
		usage_with_options(builtin_rebase_usage,
				   builtin_rebase_options);

	ಯದಿ (keep_base) {
		ಯದಿ (options.onto_name)
			die(_("options '%s' and '%s' cannot be used together"), "--keep-base", "--onto");
		ಯದಿ (options.ಬೇರು)
			die(_("options '%s' and '%s' cannot be used together"), "--keep-base", "--root");
		/*
		 * --keep-base defaults to --no-fork-point to keep the
		 * base the same.
		 */
		ಯದಿ (options.fork_point < 0)
			options.fork_point = 0;
	}
	ಯದಿ (options.ಬೇರು && options.fork_point > 0)
		die(_("options '%s' and '%s' cannot be used together"), "--root", "--fork-point");

	ಯದಿ (options.action != ACTION_NONE && !in_progress)
		die(_("no rebase in progress"));

	ಯದಿ (options.action == ACTION_EDIT_TODO && !is_merge(&options))
		die(_("The --edit-todo action can only be used during "
		      "interactive rebase."));

	ಯದಿ (trace2_is_enabled()) {
		ಯದಿ (is_merge(&options))
			trace2_cmd_mode("interactive");
		ಅನ್ಯಥಾ ಯದಿ (options.exec.nr)
			trace2_cmd_mode("interactive-exec");
		ಅನ್ಯಥಾ
			trace2_cmd_mode(action_names[options.action]);
	}

	options.reflog_action = getenv(GIT_REFLOG_ACTION_ENVIRONMENT);
	options.reflog_action =
		xstrdup(options.reflog_action ? options.reflog_action : "rebase");

	ವಿಕಲ್ಪ (options.action) {
	ಪಕ್ಷ ACTION_CONTINUE: {
		ಸಂರಚನಾ object_id head;
		ಸಂರಚನಾ lock_file lock_file = LOCK_INIT;
		ಪೂರ್ಣಾಙ್ಕ fd;

		/* Sanity check */
		ಯದಿ (repo_get_oid(the_repository, "HEAD", &head))
			die(_("Cannot read HEAD"));

		fd = repo_hold_locked_index(the_repository, &lock_file, 0);
		ಯದಿ (repo_read_index(the_repository) < 0)
			die(_("could not read index"));
		refresh_index(the_repository->index, REFRESH_QUIET, ರಿಕ್ತ, ರಿಕ್ತ,
			      ರಿಕ್ತ);
		ಯದಿ (0 <= fd)
			repo_update_index_if_able(the_repository, &lock_file);
		rollback_lock_file(&lock_file);

		ಯದಿ (has_unstaged_changes(the_repository, 1)) {
			puts(_("You must edit all merge conflicts and then\n"
			       "mark them as resolved using git add"));
			exit(1);
		}
		ಯದಿ (read_basic_state(&options))
			exit(1);
		ಗಚ್ಛ run_rebase;
	}
	ಪಕ್ಷ ACTION_SKIP: {
		ಸಂರಚನಾ string_list merge_rr = STRING_LIST_INIT_DUP;

		rerere_clear(the_repository, &merge_rr);
		string_list_clear(&merge_rr, 1);
		ropts.flags = RESET_WORKING_ವೃಕ್ಷ_HARD |
			      RESET_WORKING_ವೃಕ್ಷ_UPDATE_HEAD;
		ಯದಿ (reset_working_ವೃಕ್ಷ(the_repository, &ropts) < 0)
			die(_("could not discard worktree changes"));
		remove_branch_state(the_repository, 0);
		ಯದಿ (read_basic_state(&options))
			exit(1);
		ಗಚ್ಛ run_rebase;
	}
	ಪಕ್ಷ ACTION_ABORT: {
		ಸಂರಚನಾ string_list merge_rr = STRING_LIST_INIT_DUP;
		ಸಂರಚನಾ strbuf head_ಸಂದೇಶ = STRBUF_INIT;

		rerere_clear(the_repository, &merge_rr);
		string_list_clear(&merge_rr, 1);

		ಯದಿ (read_basic_state(&options))
			exit(1);

		strbuf_addf(&head_ಸಂದೇಶ, "%s (abort): returning to %s",
			    options.reflog_action,
			    options.head_name ? options.head_name
					      : oid_to_hex(&options.orig_head->object.oid));
		ropts.oid = &options.orig_head->object.oid;
		ropts.head_ಸಂದೇಶ = head_ಸಂದೇಶ.buf;
		ropts.branch = options.head_name;
		ropts.flags = RESET_WORKING_ವೃಕ್ಷ_HARD |
			      RESET_WORKING_ವೃಕ್ಷ_UPDATE_HEAD;
		ಯದಿ (reset_working_ವೃಕ್ಷ(the_repository, &ropts) < 0)
			die(_("could not move back to %s"),
			    oid_to_hex(&options.orig_head->object.oid));
		strbuf_release(&head_ಸಂದೇಶ);
		remove_branch_state(the_repository, 0);
		ret = finish_rebase(&options);
		ಗಚ್ಛ cleanup;
	}
	ಪಕ್ಷ ACTION_QUIT: {
		save_autostash(state_dir_path("autostash", &options));
		ಯದಿ (options.type == REBASE_MERGE) {
			ಸಂರಚನಾ replay_opts replay = REPLAY_OPTS_INIT;

			replay.action = REPLAY_INTERACTIVE_REBASE;
			ret = sequencer_remove_state(&replay);
			replay_opts_release(&replay);
		} ಅನ್ಯಥಾ {
			strbuf_reset(&buf);
			strbuf_addstr(&buf, options.state_dir);
			ret = remove_dir_recursively(&buf, 0);
			ಯದಿ (ret)
				error(_("could not remove '%s'"),
				       options.state_dir);
		}
		ಗಚ್ಛ cleanup;
	}
	ಪಕ್ಷ ACTION_EDIT_TODO:
		options.dont_finish_rebase = 1;
		ಗಚ್ಛ run_rebase;
	ಪಕ್ಷ ACTION_SHOW_CURRENT_PATCH:
		options.dont_finish_rebase = 1;
		ಗಚ್ಛ run_rebase;
	ಪಕ್ಷ ACTION_NONE:
		ಭಙ್ಗ;
	ಸ್ವಭಾವ:
		BUG("action: %d", options.action);
	}

	/* Make sure no rebase is in progress */
	ಯದಿ (in_progress) {
		ಧ್ರುವ ವರ್ಣ *last_slash = strrchr(options.state_dir, '/');
		ಧ್ರುವ ವರ್ಣ *state_dir_base =
			last_slash ? last_slash + 1 : options.state_dir;
		ಧ್ರುವ ವರ್ಣ *cmd_live_rebase =
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

	ಯದಿ ((options.flags & REBASE_INTERACTIVE_EXPLICIT) ||
	    (options.action != ACTION_NONE) ||
	    (options.exec.nr > 0) ||
	    options.autosquash == 1) {
		allow_preemptive_ff = 0;
	}
	ಯದಿ (options.committer_date_is_author_date || options.ignore_date)
		options.flags |= REBASE_FORCE;

	ಪ್ರತಿ (i = 0; i < options.git_am_opts.nr; i++) {
		ಧ್ರುವ ವರ್ಣ *option = options.git_am_opts.v[i], *p;
		ಯದಿ (!strcmp(option, "--whitespace=fix") ||
		    !strcmp(option, "--whitespace=strip"))
			allow_preemptive_ff = 0;
		ಅನ್ಯಥಾ ಯದಿ (skip_prefix(option, "-C", &p)) {
			ಯಾವತ್ (*p)
				ಯದಿ (!isdigit(*(p++)))
					die(_("switch `C' expects a "
					      "numerical value"));
		} ಅನ್ಯಥಾ ಯದಿ (skip_prefix(option, "--whitespace=", &p)) {
			ಯದಿ (*p && strcmp(p, "warn") && strcmp(p, "nowarn") &&
			    strcmp(p, "error") && strcmp(p, "error-all"))
				die("Invalid whitespace option: '%s'", p);
		}
	}

	ಪ್ರತಿ (i = 0; i < options.exec.nr; i++)
		ಯದಿ (check_exec_cmd(options.exec.ವಸ್ತುಗಳು[i].string))
			exit(1);

	ಯದಿ (!(options.flags & REBASE_NO_QUIET))
		strvec_ತಳ್ಳು(&options.git_am_opts, "-q");

	ಯದಿ (options.empty != EMPTY_UNSPECIFIED)
		imply_merge(&options, "--empty");

	ಯದಿ (options.reapply_cherry_picks < 0)
		/*
		 * We default to --no-reapply-cherry-picks unless
		 * --keep-base is given; when --keep-base is given, we want
		 * to default to --reapply-cherry-picks.
		 */
		options.reapply_cherry_picks = keep_base;
	ಅನ್ಯಥಾ ಯದಿ (!keep_base)
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

	ಯದಿ (gpg_sign)
		options.gpg_sign_opt = xstrfmt("-S%s", gpg_sign);

	ಯದಿ (options.exec.nr)
		imply_merge(&options, "--exec");

	ಯದಿ (options.type == REBASE_APPLY) {
		ಯದಿ (ignore_whitespace)
			strvec_ತಳ್ಳು(&options.git_am_opts,
				    "--ignore-whitespace");
		ಯದಿ (options.committer_date_is_author_date)
			strvec_ತಳ್ಳು(&options.git_am_opts,
				    "--committer-date-is-author-date");
		ಯದಿ (options.ignore_date)
			strvec_ತಳ್ಳು(&options.git_am_opts, "--ignore-date");
	} ಅನ್ಯಥಾ {
		/* REBASE_MERGE */
		ಯದಿ (ignore_whitespace) {
			string_list_append(&options.strategy_opts,
					   "ignore-space-change");
		}
	}

	ಯದಿ (strategy_opt)
		options.strategy = xstrdup(strategy_opt);
	ಅನ್ಯಥಾ ಯದಿ (options.strategy_opts.nr && !options.strategy)
		options.strategy = xstrdup("ort");
	ಯದಿ (options.strategy)
		imply_merge(&options, "--strategy");

	ಯದಿ (options.ಬೇರು && !options.onto_name)
		imply_merge(&options, "--root without --onto");

	ಯದಿ (options.trailer_args.nr)
		imply_merge(&options, "--trailer");

	ಯದಿ (isatty(2) && options.flags & REBASE_NO_QUIET)
		strbuf_addstr(&options.git_format_patch_opt, " --progress");

	ಯದಿ (options.git_am_opts.nr || options.type == REBASE_APPLY) {
		/* all am options except -q are compatible only with --apply */
		ಪ್ರತಿ (i = options.git_am_opts.nr - 1; i >= 0; i--)
			ಯದಿ (strcmp(options.git_am_opts.v[i], "-q"))
				ಭಙ್ಗ;

		ಯದಿ (i >= 0 || options.type == REBASE_APPLY) {
			ಯದಿ (is_merge(&options))
				die(_("apply options and merge options "
					  "cannot be used together"));
			ಅನ್ಯಥಾ ಯದಿ (options.rebase_merges == -1 && options.config_rebase_merges == 1)
				die(_("apply options are incompatible with rebase.rebaseMerges.  Consider adding --no-rebase-merges"));
			ಅನ್ಯಥಾ ಯದಿ (options.update_refs == -1 && options.config_update_refs == 1)
				die(_("apply options are incompatible with rebase.updateRefs.  Consider adding --no-update-refs"));
			ಅನ್ಯಥಾ
				options.type = REBASE_APPLY;
		}
	}

	ಯದಿ (options.update_refs == 1)
		imply_merge(&options, "--update-refs");
	options.update_refs = (options.update_refs >= 0) ? options.update_refs :
			     ((options.config_update_refs >= 0) ? options.config_update_refs : 0);

	ಯದಿ (options.rebase_merges == 1)
		imply_merge(&options, "--rebase-merges");
	options.rebase_merges = (options.rebase_merges >= 0) ? options.rebase_merges :
				((options.config_rebase_merges >= 0) ? options.config_rebase_merges : 0);

	ಯದಿ (options.autosquash == 1) {
		imply_merge(&options, "--autosquash");
	} ಅನ್ಯಥಾ ಯದಿ (options.autosquash == -1) {
		options.autosquash =
			options.config_autosquash &&
			(options.flags & REBASE_INTERACTIVE_EXPLICIT);
	}

	ಯದಿ (options.type == REBASE_UNSPECIFIED) {
		ಯದಿ (!strcmp(options.default_backend, "merge"))
			options.type = REBASE_MERGE;
		ಅನ್ಯಥಾ ಯದಿ (!strcmp(options.default_backend, "apply"))
			options.type = REBASE_APPLY;
		ಅನ್ಯಥಾ
			die(_("Unknown rebase backend: %s"),
			    options.default_backend);
	}

	ವಿಕಲ್ಪ (options.type) {
	ಪಕ್ಷ REBASE_MERGE:
		options.state_dir = merge_dir();
		ಭಙ್ಗ;
	ಪಕ್ಷ REBASE_APPLY:
		options.state_dir = apply_dir();
		ಭಙ್ಗ;
	ಸ್ವಭಾವ:
		BUG("options.type was just set above; should be unreachable.");
	}

	ಯದಿ (options.empty == EMPTY_UNSPECIFIED) {
		ಯದಿ (options.flags & REBASE_INTERACTIVE_EXPLICIT)
			options.empty = EMPTY_STOP;
		ಅನ್ಯಥಾ ಯದಿ (options.exec.nr > 0)
			options.empty = EMPTY_KEEP;
		ಅನ್ಯಥಾ
			options.empty = EMPTY_DROP;
	}
	ಯದಿ (reschedule_failed_exec > 0 && !is_merge(&options))
		die(_("--reschedule-failed-exec requires "
		      "--exec or --interactive"));
	ಯದಿ (reschedule_failed_exec >= 0)
		options.reschedule_failed_exec = reschedule_failed_exec;

	ಯದಿ (options.signoff) {
		strvec_ತಳ್ಳು(&options.git_am_opts, "--signoff");
		options.flags |= REBASE_FORCE;
	}

	ಯದಿ (!options.ಬೇರು) {
		ಯದಿ (argc < 1) {
			ಸಂರಚನಾ branch *branch;

			branch = branch_get(ರಿಕ್ತ);
			options.upstream_name = branch_get_upstream(branch,
								    ರಿಕ್ತ);
			ಯದಿ (!options.upstream_name)
				error_on_missing_default_upstream();
			ಯದಿ (options.fork_point < 0)
				options.fork_point = 1;
		} ಅನ್ಯಥಾ {
			options.upstream_name = argv[0];
			argc--;
			argv++;
			ಯದಿ (!strcmp(options.upstream_name, "-"))
				options.upstream_name = "@{-1}";
		}
		options.upstream =
			lookup_commit_reference_by_name(options.upstream_name);
		ಯದಿ (!options.upstream)
			die(_("invalid upstream '%s'"), options.upstream_name);
		options.upstream_arg = options.upstream_name;
	} ಅನ್ಯಥಾ {
		ಯದಿ (!options.onto_name) {
			ಯದಿ (commit_ವೃಕ್ಷ("", 0, the_hash_algo->empty_ವೃಕ್ಷ, ರಿಕ್ತ,
					&squash_onto, ರಿಕ್ತ, ರಿಕ್ತ) < 0)
				die(_("Could not create new root commit"));
			options.squash_onto = &squash_onto;
			options.onto_name = squash_onto_name =
				xstrdup(oid_to_hex(&squash_onto));
		} ಅನ್ಯಥಾ
			options.ಬೇರು_with_onto = 1;

		options.upstream_name = ರಿಕ್ತ;
		options.upstream = ರಿಕ್ತ;
		ಯದಿ (argc > 1)
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
	ಯದಿ (argc == 1) {
		/* Is it "rebase other branchname" or "rebase other commit"? */
		ಸಂರಚನಾ object_id branch_oid;
		branch_name = argv[0];
		options.switch_to = argv[0];

		/* Is it a local branch? */
		strbuf_reset(&buf);
		strbuf_addf(&buf, "refs/heads/%s", branch_name);
		ಯದಿ (!refs_read_ref(get_main_ref_store(the_repository), buf.buf, &branch_oid)) {
			die_if_checked_out(buf.buf, 1);
			options.head_name = xstrdup(buf.buf);
			options.orig_head =
				lookup_commit_object(the_repository,
						     &branch_oid);
		/* If not is it a valid ref (branch or commit)? */
		} ಅನ್ಯಥಾ {
			options.orig_head =
				lookup_commit_reference_by_name(branch_name);
			options.head_name = ರಿಕ್ತ;
		}
		ಯದಿ (!options.orig_head)
			die(_("no such branch/commit '%s'"), branch_name);
	} ಅನ್ಯಥಾ ಯದಿ (argc == 0) {
		/* Do not need to switch branches, we are already on it. */
		options.head_name =
			xstrdup_or_null(refs_resolve_ref_unsafe(get_main_ref_store(the_repository), "HEAD", 0, ರಿಕ್ತ,
								&flags));
		ಯದಿ (!options.head_name)
			die(_("No such ref: %s"), "HEAD");
		ಯದಿ (flags & REF_ISSYMREF) {
			ಯದಿ (!skip_prefix(options.head_name,
					 "refs/heads/", &branch_name))
				branch_name = options.head_name;

		} ಅನ್ಯಥಾ {
			FREE_AND_NULL(options.head_name);
			branch_name = "HEAD";
		}
		options.orig_head = lookup_commit_reference_by_name("HEAD");
		ಯದಿ (!options.orig_head)
			die(_("Could not resolve HEAD to a commit"));
	} ಅನ್ಯಥಾ
		BUG("unexpected number of arguments left to parse");

	/* Make sure the branch to rebase onto is valid. */
	ಯದಿ (keep_base) {
		strbuf_reset(&buf);
		strbuf_addstr(&buf, options.upstream_name);
		strbuf_addstr(&buf, "...");
		strbuf_addstr(&buf, branch_name);
		options.onto_name = keep_base_onto_name = xstrdup(buf.buf);
	} ಅನ್ಯಥಾ ಯದಿ (!options.onto_name)
		options.onto_name = options.upstream_name;
	ಯದಿ (strstr(options.onto_name, "...")) {
		ಯದಿ (repo_get_oid_mb(the_repository, options.onto_name, &branch_base) < 0) {
			ಯದಿ (keep_base)
				die(_("'%s': need exactly one merge base with branch"),
				    options.upstream_name);
			ಅನ್ಯಥಾ
				die(_("'%s': need exactly one merge base"),
				    options.onto_name);
		}
		options.onto = lookup_commit_or_die(&branch_base,
						    options.onto_name);
	} ಅನ್ಯಥಾ {
		options.onto =
			lookup_commit_reference_by_name(options.onto_name);
		ಯದಿ (!options.onto)
			die(_("Does not point to a valid commit '%s'"),
				options.onto_name);
		fill_branch_base(&options, &branch_base);
	}

	ಯದಿ (keep_base && options.reapply_cherry_picks)
		options.upstream = options.onto;

	ಯದಿ (options.fork_point > 0)
		options.restrict_revision =
			get_fork_point(options.upstream_name, options.orig_head);

	ಯದಿ (repo_read_index(the_repository) < 0)
		die(_("could not read index"));

	ಯದಿ (options.autostash)
		create_autostash(the_repository,
				 state_dir_path("autostash", &options));


	ಯದಿ (require_clean_work_ವೃಕ್ಷ(the_repository, "rebase",
				    _("Please commit or stash them."), 1, 1)) {
		ret = -1;
		ಗಚ್ಛ cleanup_autostash;
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
	ಯದಿ (allow_preemptive_ff &&
	    can_fast_forward(options.onto, options.upstream, options.restrict_revision,
			     options.orig_head, &branch_base)) {
		ಪೂರ್ಣಾಙ್ಕ flag;

		ಯದಿ (!(options.flags & REBASE_FORCE)) {
			/* Lazily switch to the target branch if needed... */
			ಯದಿ (options.switch_to) {
				ret = checkout_up_to_date(&options);
				ಯದಿ (ret)
					ಗಚ್ಛ cleanup_autostash;
			}

			ಯದಿ (!(options.flags & REBASE_NO_QUIET))
				; /* be quiet */
			ಅನ್ಯಥಾ ಯದಿ (!strcmp(branch_name, "HEAD") &&
				 refs_resolve_ref_unsafe(get_main_ref_store(the_repository), "HEAD", 0, ರಿಕ್ತ, &flag))
				puts(_("HEAD is up to date."));
			ಅನ್ಯಥಾ
				printf(_("Current branch %s is up to date.\n"),
				       branch_name);
			ret = finish_rebase(&options);
			ಗಚ್ಛ cleanup;
		} ಅನ್ಯಥಾ ಯದಿ (!(options.flags & REBASE_NO_QUIET))
			; /* be quiet */
		ಅನ್ಯಥಾ ಯದಿ (!strcmp(branch_name, "HEAD") &&
			 refs_resolve_ref_unsafe(get_main_ref_store(the_repository), "HEAD", 0, ರಿಕ್ತ, &flag))
			puts(_("HEAD is up to date, rebase forced."));
		ಅನ್ಯಥಾ
			printf(_("Current branch %s is up to date, rebase "
				 "forced.\n"), branch_name);
	}

	/* If a hook exists, give it a chance to interrupt*/
	ಯದಿ (!ok_to_skip_pre_rebase &&
	    run_hooks_l(the_repository, "pre-rebase", options.upstream_arg,
			argc ? argv[0] : ರಿಕ್ತ, ರಿಕ್ತ)) {
		ret = error(_("The pre-rebase hook refused to rebase."));
		ಗಚ್ಛ cleanup_autostash;
	}

	ಯದಿ (options.flags & REBASE_DIFFSTAT) {
		ಸಂರಚನಾ diff_options opts;

		ಯದಿ (options.flags & REBASE_VERBOSE) {
			ಯದಿ (is_null_oid(&branch_base))
				printf(_("Changes to %s:\n"),
				       oid_to_hex(&options.onto->object.oid));
			ಅನ್ಯಥಾ
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
		diff_ವೃಕ್ಷ_oid(is_null_oid(&branch_base) ?
			      the_hash_algo->empty_ವೃಕ್ಷ : &branch_base,
			      &options.onto->object.oid, "", &opts);
		diffcore_std(&opts);
		diff_flush(&opts);
	}

	ಯದಿ (is_merge(&options))
		ಗಚ್ಛ run_rebase;

	/* Detach HEAD and reset the tree */
	ಯದಿ (options.flags & REBASE_NO_QUIET)
		printf(_("First, rewinding head to replay your work on top of "
			 "it...\n"));

	strbuf_addf(&ಸಂದೇಶ, "%s (start): checkout %s",
		    options.reflog_action, options.onto_name);
	ropts.oid = &options.onto->object.oid;
	ropts.orig_head = &options.orig_head->object.oid;
	ropts.flags = RESET_WORKING_ವೃಕ್ಷ_DETACH |
		      RESET_WORKING_ವೃಕ್ಷ_UPDATE_HEAD |
		      RESET_WORKING_ವೃಕ್ಷ_UPDATE_ORIG_HEAD |
		      RESET_WORKING_ವೃಕ್ಷ_RUN_POST_CHECKOUT_HOOK;
	ropts.head_ಸಂದೇಶ = ಸಂದೇಶ.buf;
	ropts.default_reflog_action = options.reflog_action;
	ಯದಿ (reset_working_ವೃಕ್ಷ(the_repository, &ropts)) {
		ret = error(_("Could not detach HEAD"));
		ಗಚ್ಛ cleanup_autostash;
	}

	/*
	 * If the onto is a proper descendant of the tip of the branch, then
	 * we just fast-forwarded.
	 */
	ಯದಿ (oideq(&branch_base, &options.orig_head->object.oid)) {
		printf(_("Fast-forwarded %s to %s.\n"),
			branch_name, options.onto_name);
		move_to_original_branch(&options);
		ret = finish_rebase(&options);
		ಗಚ್ಛ cleanup;
	}

	strbuf_addf(&revisions, "%s..%s",
		    options.ಬೇರು ? oid_to_hex(&options.onto->object.oid) :
		    (options.restrict_revision ?
		     oid_to_hex(&options.restrict_revision->object.oid) :
		     oid_to_hex(&options.upstream->object.oid)),
		    oid_to_hex(&options.orig_head->object.oid));

	options.revisions = revisions.buf;

run_rebase:
	ret = run_specific_rebase(&options);

cleanup:
	strbuf_release(&buf);
	strbuf_release(&ಸಂದೇಶ);
	strbuf_release(&revisions);
	rebase_options_release(&options);
	ಮುಕ್ತ(squash_onto_name);
	ಮುಕ್ತ(keep_base_onto_name);
	ಪ್ರತ್ಯಾಗಮ !!ret;

cleanup_autostash:
	ret |= !!cleanup_autostash(&options);
	ಗಚ್ಛ cleanup;
}
