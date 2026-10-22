/*
 * "git rebase" builtin command
 *
 * Copyright (c) 2018 Pratik Karki
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#അന്തര്ഭാവ "builtin.h"

#അന്തര്ഭാവ "abspath.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "hex.h"
#അന്തര്ഭാവ "run-command.h"
#അന്തര്ഭാവ "strvec.h"
#അന്തര്ഭാവ "dir.h"
#അന്തര്ഭാവ "refs.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "unpack-trees.h"
#അന്തര്ഭാവ "lockfile.h"
#അന്തര്ഭാവ "object-file.h"
#അന്തര്ഭാവ "object-name.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "path.h"
#അന്തര്ഭാവ "commit.h"
#അന്തര്ഭാവ "diff.h"
#അന്തര്ഭാവ "wt-status.h"
#അന്തര്ഭാവ "revision.h"
#അന്തര്ഭാവ "commit-reach.h"
#അന്തര്ഭാവ "rerere.h"
#അന്തര്ഭാവ "branch.h"
#അന്തര്ഭാവ "sequencer.h"
#അന്തര്ഭാവ "rebase-interactive.h"
#അന്തര്ഭാവ "reset.h"
#അന്തര്ഭാവ "trace2.h"
#അന്തര്ഭാവ "hook.h"
#അന്തര്ഭാവ "trailer.h"

സ്ഥിതം അക്ഷരം സ്ഥിരം * സ്ഥിരം builtin_rebase_usage[] = {
	N_("git rebase [-i] [options] [--exec <cmd>] "
		"[--onto <newbase> | --keep-base] [<upstream> [<branch>]]"),
	N_("git rebase [-i] [options] [--exec <cmd>] [--onto <newbase>] "
		"--root [<branch>]"),
	"git rebase --continue | --abort | --skip | --edit-todo",
	ശൂന്യ
};

സ്ഥിതം GIT_PATH_FUNC(path_squash_onto, "rebase-merge/squash-onto")
സ്ഥിതം GIT_PATH_FUNC(path_interactive, "rebase-merge/interactive")
സ്ഥിതം GIT_PATH_FUNC(apply_dir, "rebase-apply")
സ്ഥിതം GIT_PATH_FUNC(merge_dir, "rebase-merge")

എണ്ണൽ rebase_type {
	REBASE_UNSPECIFIED = -1,
	REBASE_APPLY,
	REBASE_MERGE
};

എണ്ണൽ empty_type {
	EMPTY_UNSPECIFIED = -1,
	EMPTY_DROP,
	EMPTY_KEEP,
	EMPTY_STOP
};

എണ്ണൽ action {
	ACTION_NONE = 0,
	ACTION_CONTINUE,
	ACTION_SKIP,
	ACTION_ABORT,
	ACTION_QUIT,
	ACTION_EDIT_TODO,
	ACTION_SHOW_CURRENT_PATCH
};

സ്ഥിതം സ്ഥിരം അക്ഷരം *action_names[] = {
	"undefined",
	"continue",
	"skip",
	"abort",
	"quit",
	"edit_todo",
	"show_current_patch"
};

ഘടന rebase_options {
	എണ്ണൽ rebase_type type;
	എണ്ണൽ empty_type empty;
	അക്ഷരം *default_backend;
	സ്ഥിരം അക്ഷരം *state_dir;
	ഘടന commit *upstream;
	സ്ഥിരം അക്ഷരം *upstream_name;
	സ്ഥിരം അക്ഷരം *upstream_arg;
	അക്ഷരം *head_name;
	ഘടന commit *orig_head;
	ഘടന commit *onto;
	സ്ഥിരം അക്ഷരം *onto_name;
	സ്ഥിരം അക്ഷരം *revisions;
	സ്ഥിരം അക്ഷരം *switch_to;
	പൂർണ്ണസംഖ്യ വേര്, വേര്_with_onto;
	ഘടന object_id *squash_onto;
	ഘടന commit *restrict_revision;
	പൂർണ്ണസംഖ്യ dont_finish_rebase;
	എണ്ണൽ {
		REBASE_NO_QUIET = 1<<0,
		REBASE_VERBOSE = 1<<1,
		REBASE_DIFFSTAT = 1<<2,
		REBASE_FORCE = 1<<3,
		REBASE_INTERACTIVE_EXPLICIT = 1<<4,
	} flags;
	ഘടന strvec git_am_opts;
	എണ്ണൽ action action;
	അക്ഷരം *reflog_action;
	പൂർണ്ണസംഖ്യ signoff;
	ഘടന strvec trailer_args;
	പൂർണ്ണസംഖ്യ allow_rerere_autoupdate;
	പൂർണ്ണസംഖ്യ keep_empty;
	പൂർണ്ണസംഖ്യ autosquash;
	അക്ഷരം *gpg_sign_opt;
	പൂർണ്ണസംഖ്യ autostash;
	പൂർണ്ണസംഖ്യ committer_date_is_author_date;
	പൂർണ്ണസംഖ്യ ignore_date;
	ഘടന string_list exec;
	പൂർണ്ണസംഖ്യ allow_empty_message;
	പൂർണ്ണസംഖ്യ rebase_merges, rebase_cousins;
	അക്ഷരം *strategy;
	ഘടന string_list strategy_opts;
	ഘടന strbuf git_format_patch_opt;
	പൂർണ്ണസംഖ്യ reschedule_failed_exec;
	പൂർണ്ണസംഖ്യ reapply_cherry_picks;
	പൂർണ്ണസംഖ്യ fork_point;
	പൂർണ്ണസംഖ്യ update_refs;
	പൂർണ്ണസംഖ്യ config_autosquash;
	പൂർണ്ണസംഖ്യ config_rebase_merges;
	പൂർണ്ണസംഖ്യ config_update_refs;
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

സ്ഥിതം ശൂന്യം rebase_options_release(ഘടന rebase_options *opts)
{
	സ്വതന്ത്രം(opts->default_backend);
	സ്വതന്ത്രം(opts->reflog_action);
	സ്വതന്ത്രം(opts->head_name);
	strvec_clear(&opts->git_am_opts);
	സ്വതന്ത്രം(opts->gpg_sign_opt);
	string_list_clear(&opts->exec, 0);
	സ്വതന്ത്രം(opts->strategy);
	string_list_clear(&opts->strategy_opts, 0);
	strbuf_release(&opts->git_format_patch_opt);
	strvec_clear(&opts->trailer_args);
}

സ്ഥിതം ഘടന replay_opts get_replay_opts(സ്ഥിരം ഘടന rebase_options *opts)
{
	ഘടന replay_opts replay = REPLAY_OPTS_INIT;

	replay.action = REPLAY_INTERACTIVE_REBASE;
	replay.strategy = ശൂന്യ;
	sequencer_init_config(&replay);

	replay.signoff = opts->signoff;

	strvec_pushv(&replay.trailer_args, opts->trailer_args.v);

	replay.allow_ff = !(opts->flags & REBASE_FORCE);
	എങ്കിൽ (opts->allow_rerere_autoupdate)
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
	സ്വതന്ത്രം(replay.gpg_sign);
	replay.gpg_sign = xstrdup_or_null(opts->gpg_sign_opt);
	replay.reflog_action = xstrdup(opts->reflog_action);
	എങ്കിൽ (opts->strategy)
		replay.strategy = xstrdup_or_null(opts->strategy);
	അല്ലെങ്കിൽ എങ്കിൽ (!replay.strategy && replay.default_strategy) {
		replay.strategy = replay.default_strategy;
		replay.default_strategy = ശൂന്യ;
	}

	വേണ്ടി (size_t i = 0; i < opts->strategy_opts.nr; i++)
		strvec_തള്ളുക(&replay.xopts, opts->strategy_opts.ഇനങ്ങൾ[i].string);

	എങ്കിൽ (opts->squash_onto) {
		oidcpy(&replay.squash_onto, opts->squash_onto);
		replay.have_squash_onto = 1;
	}

	തിരികെ replay;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ edit_todo_file(unsigned flags, ഘടന replay_opts *opts)
{
	സ്ഥിരം അക്ഷരം *todo_file = rebase_path_todo();
	ഘടന todo_list todo_list = TODO_LIST_INIT,
		new_todo = TODO_LIST_INIT;
	പൂർണ്ണസംഖ്യ res = 0;

	എങ്കിൽ (strbuf_read_file(&todo_list.buf, todo_file, 0) < 0)
		തിരികെ error_errno(_("could not read '%s'."), todo_file);

	strbuf_stripspace(&todo_list.buf, comment_വരി_str);
	res = edit_todo_list(the_repository, opts, &todo_list, &new_todo,
			     ശൂന്യ, ശൂന്യ, flags);
	എങ്കിൽ (!res && todo_list_write_to_file(the_repository, &new_todo, todo_file,
					    ശൂന്യ, ശൂന്യ, -1, flags & ~(TODO_LIST_SHORTEN_IDS)))
		res = error_errno(_("could not write '%s'"), todo_file);

	todo_list_release(&todo_list);
	todo_list_release(&new_todo);

	തിരികെ res;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ get_revision_ranges(ഘടന commit *upstream, ഘടന commit *onto,
			       ഘടന object_id *orig_head, അക്ഷരം **revisions,
			       അക്ഷരം **shortrevisions)
{
	ഘടന commit *base_rev = upstream ? upstream : onto;
	സ്ഥിരം അക്ഷരം *shorthead;

	*revisions = xstrfmt("%s...%s", oid_to_hex(&base_rev->object.oid),
			     oid_to_hex(orig_head));

	shorthead = repo_find_unique_abbrev(the_repository, orig_head,
					    DEFAULT_ABBREV);

	എങ്കിൽ (upstream) {
		സ്ഥിരം അക്ഷരം *shortrev;

		shortrev = repo_find_unique_abbrev(the_repository,
						   &base_rev->object.oid,
						   DEFAULT_ABBREV);

		*shortrevisions = xstrfmt("%s..%s", shortrev, shorthead);
	} അല്ലെങ്കിൽ
		*shortrevisions = xstrdup(shorthead);

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ init_basic_state(ഘടന replay_opts *opts, സ്ഥിരം അക്ഷരം *head_name,
			    ഘടന commit *onto,
			    സ്ഥിരം ഘടന object_id *orig_head)
{
	FILE *interactive;

	എങ്കിൽ (!is_directory(merge_dir()) &&
	    safe_create_dir_in_gitdir(the_repository, merge_dir()))
		തിരികെ error_errno(_("could not create temporary %s"), merge_dir());

	refs_delete_reflog(get_main_ref_store(the_repository), "REBASE_HEAD");

	interactive = fopen(path_interactive(), "w");
	എങ്കിൽ (!interactive)
		തിരികെ error_errno(_("could not mark as interactive"));
	fclose(interactive);

	തിരികെ write_basic_state(opts, head_name, onto, orig_head);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ do_interactive_rebase(ഘടന rebase_options *opts, unsigned flags)
{
	പൂർണ്ണസംഖ്യ ret = -1;
	അക്ഷരം *revisions = ശൂന്യ, *shortrevisions = ശൂന്യ;
	ഘടന strvec make_script_args = STRVEC_INIT;
	ഘടന todo_list todo_list = TODO_LIST_INIT;
	ഘടന replay_opts replay = get_replay_opts(opts);

	എങ്കിൽ (get_revision_ranges(opts->upstream, opts->onto, &opts->orig_head->object.oid,
				&revisions, &shortrevisions))
		പോകുക cleanup;

	strvec_pushl(&make_script_args, "", revisions, ശൂന്യ);
	എങ്കിൽ (opts->restrict_revision)
		strvec_pushf(&make_script_args, "^%s",
			     oid_to_hex(&opts->restrict_revision->object.oid));

	ret = sequencer_make_script(the_repository, &todo_list.buf,
				    &make_script_args, flags);
	എങ്കിൽ (ret) {
		error(_("could not generate todo list"));
		പോകുക cleanup;
	}

	എങ്കിൽ (init_basic_state(&replay,
			     opts->head_name ? opts->head_name : "detached HEAD",
			     opts->onto, &opts->orig_head->object.oid))
		പോകുക cleanup;

	എങ്കിൽ (!opts->upstream && opts->squash_onto)
		write_file(path_squash_onto(), "%s\n",
			   oid_to_hex(opts->squash_onto));

	discard_index(the_repository->index);
	എങ്കിൽ (todo_list_parse_insn_buffer(the_repository, &replay,
					todo_list.buf.buf, &todo_list))
		BUG("unusable todo list");

	ret = complete_action(the_repository, &replay, flags,
		shortrevisions, opts->onto_name, opts->onto,
		&opts->orig_head->object.oid, &opts->exec,
		opts->autosquash, opts->update_refs, &todo_list);

cleanup:
	replay_opts_release(&replay);
	സ്വതന്ത്രം(revisions);
	സ്വതന്ത്രം(shortrevisions);
	todo_list_release(&todo_list);
	strvec_clear(&make_script_args);

	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ run_sequencer_rebase(ഘടന rebase_options *opts)
{
	unsigned flags = 0;
	പൂർണ്ണസംഖ്യ abbreviate_commands = 0, ret = 0;

	repo_config_get_bool(the_repository, "rebase.abbreviatecommands", &abbreviate_commands);

	flags |= opts->keep_empty ? TODO_LIST_KEEP_EMPTY : 0;
	flags |= abbreviate_commands ? TODO_LIST_ABBREVIATE_CMDS : 0;
	flags |= opts->rebase_merges ? TODO_LIST_REBASE_MERGES : 0;
	flags |= opts->rebase_cousins > 0 ? TODO_LIST_REBASE_COUSINS : 0;
	flags |= opts->വേര്_with_onto ? TODO_LIST_വേര്_WITH_ONTO : 0;
	flags |= opts->reapply_cherry_picks ? TODO_LIST_REAPPLY_CHERRY_PICKS : 0;
	flags |= opts->flags & REBASE_NO_QUIET ? TODO_LIST_WARN_SKIPPED_CHERRY_PICKS : 0;

	തിരഞ്ഞെടുപ്പ് (opts->action) {
	അവസ്ഥ ACTION_NONE: {
		എങ്കിൽ (!opts->onto && !opts->upstream)
			die(_("a base commit must be provided with --upstream or --onto"));

		ret = do_interactive_rebase(opts, flags);
		നിർത്തുക;
	}
	അവസ്ഥ ACTION_SKIP: {
		ഘടന string_list merge_rr = STRING_LIST_INIT_DUP;

		rerere_clear(the_repository, &merge_rr);
	}
		/* fallthrough */
	അവസ്ഥ ACTION_CONTINUE: {
		ഘടന replay_opts replay_opts = get_replay_opts(opts);

		ret = sequencer_continue(the_repository, &replay_opts);
		replay_opts_release(&replay_opts);
		നിർത്തുക;
	}
	അവസ്ഥ ACTION_EDIT_TODO: {
		ഘടന replay_opts replay_opts = get_replay_opts(opts);

		ret = edit_todo_file(flags, &replay_opts);
		replay_opts_release(&replay_opts);
		നിർത്തുക;
	}
	അവസ്ഥ ACTION_SHOW_CURRENT_PATCH: {
		ഘടന child_process cmd = CHILD_PROCESS_INIT;

		cmd.git_cmd = 1;
		strvec_pushl(&cmd.args, "show", "REBASE_HEAD", "--", ശൂന്യ);
		ret = run_command(&cmd);

		നിർത്തുക;
	}
	സ്ഥിരസ്ഥിതി:
		BUG("invalid command '%d'", opts->action);
	}

	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ is_merge(ഘടന rebase_options *opts)
{
	തിരികെ opts->type == REBASE_MERGE;
}

സ്ഥിതം ശൂന്യം imply_merge(ഘടന rebase_options *opts, സ്ഥിരം അക്ഷരം *option)
{
	തിരഞ്ഞെടുപ്പ് (opts->type) {
	അവസ്ഥ REBASE_APPLY:
		die(_("%s requires the merge backend"), option);
		നിർത്തുക;
	അവസ്ഥ REBASE_MERGE:
		നിർത്തുക;
	സ്ഥിരസ്ഥിതി:
		opts->type = REBASE_MERGE; /* implied */
		നിർത്തുക;
	}
}

/* Returns the filename prefixed by the state_dir */
സ്ഥിതം സ്ഥിരം അക്ഷരം *state_dir_path(സ്ഥിരം അക്ഷരം *filename, ഘടന rebase_options *opts)
{
	സ്ഥിതം ഘടന strbuf path = STRBUF_INIT;
	സ്ഥിതം size_t prefix_len;

	എങ്കിൽ (!prefix_len) {
		strbuf_addf(&path, "%s/", opts->state_dir);
		prefix_len = path.len;
	}

	strbuf_setlen(&path, prefix_len);
	strbuf_addstr(&path, filename);
	തിരികെ path.buf;
}

/* Initialize the rebase options from the state directory. */
സ്ഥിതം പൂർണ്ണസംഖ്യ read_basic_state(ഘടന rebase_options *opts)
{
	ഘടന strbuf head_name = STRBUF_INIT;
	ഘടന strbuf buf = STRBUF_INIT;
	ഘടന object_id oid;

	എങ്കിൽ (!read_oneliner(&head_name, state_dir_path("head-name", opts),
			   READ_ONELINER_WARN_MISSING) ||
	    !read_oneliner(&buf, state_dir_path("onto", opts),
			   READ_ONELINER_WARN_MISSING))
		തിരികെ -1;
	opts->head_name = starts_with(head_name.buf, "refs/") ?
		xstrdup(head_name.buf) : ശൂന്യ;
	strbuf_release(&head_name);
	എങ്കിൽ (get_oid_hex(buf.buf, &oid) ||
	    !(opts->onto = lookup_commit_object(the_repository, &oid)))
		തിരികെ error(_("invalid onto: '%s'"), buf.buf);

	/*
	 * We always write to orig-head, but interactive rebase used to write to
	 * head. Fall back to reading from head to cover for the case that the
	 * user upgraded git with an ongoing interactive rebase.
	 */
	strbuf_reset(&buf);
	എങ്കിൽ (file_exists(state_dir_path("orig-head", opts))) {
		എങ്കിൽ (!read_oneliner(&buf, state_dir_path("orig-head", opts),
				   READ_ONELINER_WARN_MISSING))
			തിരികെ -1;
	} അല്ലെങ്കിൽ എങ്കിൽ (!read_oneliner(&buf, state_dir_path("head", opts),
				  READ_ONELINER_WARN_MISSING))
		തിരികെ -1;
	എങ്കിൽ (get_oid_hex(buf.buf, &oid) ||
	    !(opts->orig_head = lookup_commit_object(the_repository, &oid)))
		തിരികെ error(_("invalid orig-head: '%s'"), buf.buf);

	എങ്കിൽ (file_exists(state_dir_path("quiet", opts)))
		opts->flags &= ~REBASE_NO_QUIET;
	അല്ലെങ്കിൽ
		opts->flags |= REBASE_NO_QUIET;

	എങ്കിൽ (file_exists(state_dir_path("verbose", opts)))
		opts->flags |= REBASE_VERBOSE;

	എങ്കിൽ (file_exists(state_dir_path("signoff", opts))) {
		opts->signoff = 1;
		opts->flags |= REBASE_FORCE;
	}

	എങ്കിൽ (file_exists(state_dir_path("allow_rerere_autoupdate", opts))) {
		strbuf_reset(&buf);
		എങ്കിൽ (!read_oneliner(&buf, state_dir_path("allow_rerere_autoupdate", opts),
				   READ_ONELINER_WARN_MISSING))
			തിരികെ -1;
		എങ്കിൽ (!strcmp(buf.buf, "--rerere-autoupdate"))
			opts->allow_rerere_autoupdate = RERERE_AUTOUPDATE;
		അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(buf.buf, "--no-rerere-autoupdate"))
			opts->allow_rerere_autoupdate = RERERE_NOAUTOUPDATE;
		അല്ലെങ്കിൽ
			warning(_("ignoring invalid allow_rerere_autoupdate: "
				  "'%s'"), buf.buf);
	}

	എങ്കിൽ (file_exists(state_dir_path("gpg_sign_opt", opts))) {
		strbuf_reset(&buf);
		എങ്കിൽ (!read_oneliner(&buf, state_dir_path("gpg_sign_opt", opts),
				   READ_ONELINER_WARN_MISSING))
			തിരികെ -1;
		സ്വതന്ത്രം(opts->gpg_sign_opt);
		opts->gpg_sign_opt = xstrdup(buf.buf);
	}

	strbuf_release(&buf);

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ rebase_write_basic_state(ഘടന rebase_options *opts)
{
	write_file(state_dir_path("head-name", opts), "%s",
		   opts->head_name ? opts->head_name : "detached HEAD");
	write_file(state_dir_path("onto", opts), "%s",
		   opts->onto ? oid_to_hex(&opts->onto->object.oid) : "");
	write_file(state_dir_path("orig-head", opts), "%s",
		   oid_to_hex(&opts->orig_head->object.oid));
	എങ്കിൽ (!(opts->flags & REBASE_NO_QUIET))
		write_file(state_dir_path("quiet", opts), "%s", "");
	എങ്കിൽ (opts->flags & REBASE_VERBOSE)
		write_file(state_dir_path("verbose", opts), "%s", "");
	എങ്കിൽ (opts->allow_rerere_autoupdate > 0)
		write_file(state_dir_path("allow_rerere_autoupdate", opts),
			   "-%s-rerere-autoupdate",
			   opts->allow_rerere_autoupdate == RERERE_AUTOUPDATE ?
				"" : "-no");
	എങ്കിൽ (opts->gpg_sign_opt)
		write_file(state_dir_path("gpg_sign_opt", opts), "%s",
			   opts->gpg_sign_opt);
	എങ്കിൽ (opts->signoff)
		write_file(state_dir_path("signoff", opts), "--signoff");

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cleanup_autostash(ഘടന rebase_options *opts)
{
	പൂർണ്ണസംഖ്യ ret;
	ഘടന strbuf dir = STRBUF_INIT;
	സ്ഥിരം അക്ഷരം *path = state_dir_path("autostash", opts);

	എങ്കിൽ (!file_exists(path))
		തിരികെ 0;
	ret = apply_autostash(path);
	strbuf_addstr(&dir, opts->state_dir);
	എങ്കിൽ (remove_dir_recursively(&dir, 0))
		ret = error_errno(_("could not remove '%s'"), opts->state_dir);
	strbuf_release(&dir);

	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ finish_rebase(ഘടന rebase_options *opts)
{
	ഘടന strbuf dir = STRBUF_INIT;
	പൂർണ്ണസംഖ്യ ret = 0;

	refs_delete_ref(get_main_ref_store(the_repository), ശൂന്യ,
			"REBASE_HEAD", ശൂന്യ, REF_NO_DEREF);
	refs_delete_ref(get_main_ref_store(the_repository), ശൂന്യ,
			"AUTO_MERGE", ശൂന്യ, REF_NO_DEREF);
	apply_autostash(state_dir_path("autostash", opts));
	/*
	 * We ignore errors in 'git maintenance run --auto', since the
	 * user should see them.
	 */
	run_auto_maintenance(the_repository,
			!(opts->flags & (REBASE_NO_QUIET|REBASE_VERBOSE)));

	എങ്കിൽ (opts->type == REBASE_MERGE) {
		ഘടന replay_opts replay = REPLAY_OPTS_INIT;

		replay.action = REPLAY_INTERACTIVE_REBASE;
		ret = sequencer_remove_state(&replay);
		replay_opts_release(&replay);
	} അല്ലെങ്കിൽ {
		strbuf_addstr(&dir, opts->state_dir);
		എങ്കിൽ (remove_dir_recursively(&dir, 0))
			ret = error(_("could not remove '%s'"),
				    opts->state_dir);
		strbuf_release(&dir);
	}

	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ move_to_original_branch(ഘടന rebase_options *opts)
{
	ഘടന strbuf branch_reflog = STRBUF_INIT, head_reflog = STRBUF_INIT;
	ഘടന reset_working_വൃക്ഷം_options ropts = { 0 };
	പൂർണ്ണസംഖ്യ ret;

	എങ്കിൽ (!opts->head_name)
		തിരികെ 0; /* nothing to move back to */

	എങ്കിൽ (!opts->onto)
		BUG("move_to_original_branch without onto");

	strbuf_addf(&branch_reflog, "%s (finish): %s onto %s",
		    opts->reflog_action,
		    opts->head_name, oid_to_hex(&opts->onto->object.oid));
	strbuf_addf(&head_reflog, "%s (finish): returning to %s",
		    opts->reflog_action, opts->head_name);
	ropts.branch = opts->head_name;
	ropts.flags = RESET_WORKING_വൃക്ഷം_REFS_ONLY |
		      RESET_WORKING_വൃക്ഷം_UPDATE_HEAD;
	ropts.branch_സന്ദേശം = branch_reflog.buf;
	ropts.head_സന്ദേശം = head_reflog.buf;
	ret = reset_working_വൃക്ഷം(the_repository, &ropts);

	strbuf_release(&branch_reflog);
	strbuf_release(&head_reflog);
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ run_am(ഘടന rebase_options *opts)
{
	ഘടന child_process am = CHILD_PROCESS_INIT;
	ഘടന child_process format_patch = CHILD_PROCESS_INIT;
	പൂർണ്ണസംഖ്യ status;
	അക്ഷരം *rebased_patches;

	am.git_cmd = 1;
	strvec_തള്ളുക(&am.args, "am");
	strvec_pushf(&am.env, GIT_REFLOG_ACTION_ENVIRONMENT "=%s (pick)",
		     opts->reflog_action);
	എങ്കിൽ (opts->action == ACTION_CONTINUE) {
		strvec_തള്ളുക(&am.args, "--resolved");
		strvec_pushf(&am.args, "--resolvemsg=%s", rebase_resolvemsg);
		എങ്കിൽ (opts->gpg_sign_opt)
			strvec_തള്ളുക(&am.args, opts->gpg_sign_opt);
		status = run_command(&am);
		എങ്കിൽ (status)
			തിരികെ status;

		തിരികെ move_to_original_branch(opts);
	}
	എങ്കിൽ (opts->action == ACTION_SKIP) {
		strvec_തള്ളുക(&am.args, "--skip");
		strvec_pushf(&am.args, "--resolvemsg=%s", rebase_resolvemsg);
		status = run_command(&am);
		എങ്കിൽ (status)
			തിരികെ status;

		തിരികെ move_to_original_branch(opts);
	}
	എങ്കിൽ (opts->action == ACTION_SHOW_CURRENT_PATCH) {
		strvec_തള്ളുക(&am.args, "--show-current-patch");
		തിരികെ run_command(&am);
	}

	rebased_patches = repo_git_path(the_repository, "rebased-patches");
	format_patch.out = open(rebased_patches,
				O_WRONLY | O_CREAT | O_TRUNC, 0666);
	എങ്കിൽ (format_patch.out < 0) {
		status = error_errno(_("could not open '%s' for writing"),
				     rebased_patches);
		സ്വതന്ത്രം(rebased_patches);
		child_process_clear(&am);
		തിരികെ status;
	}

	format_patch.git_cmd = 1;
	strvec_pushl(&format_patch.args, "format-patch", "-k", "--stdout",
		     "--full-index", "--cherry-pick", "--right-only",
		     "--default-prefix", "--no-renames",
		     "--no-cover-letter", "--pretty=mboxrd", "--topo-order",
		     "--no-base", ശൂന്യ);
	എങ്കിൽ (opts->git_format_patch_opt.len)
		strvec_split(&format_patch.args,
			     opts->git_format_patch_opt.buf);
	strvec_pushf(&format_patch.args, "%s...%s",
		     oid_to_hex(opts->വേര് ?
				/* this is now equivalent to !opts->upstream */
				&opts->onto->object.oid :
				&opts->upstream->object.oid),
		     oid_to_hex(&opts->orig_head->object.oid));
	എങ്കിൽ (opts->restrict_revision)
		strvec_pushf(&format_patch.args, "^%s",
			     oid_to_hex(&opts->restrict_revision->object.oid));

	status = run_command(&format_patch);
	എങ്കിൽ (status) {
		ഘടന reset_working_വൃക്ഷം_options ropts = { 0 };
		unlink(rebased_patches);
		സ്വതന്ത്രം(rebased_patches);
		child_process_clear(&am);

		ropts.oid = &opts->orig_head->object.oid;
		ropts.branch = opts->head_name;
		ropts.default_reflog_action = opts->reflog_action;
		ropts.flags = RESET_WORKING_വൃക്ഷം_UPDATE_HEAD;
		reset_working_വൃക്ഷം(the_repository, &ropts);
		error(_("\ngit encountered an error while preparing the "
			"patches to replay\n"
			"these revisions:\n"
			"\n    %s\n\n"
			"As a result, git cannot rebase them."),
		      opts->revisions);

		തിരികെ status;
	}

	am.in = open(rebased_patches, O_RDONLY);
	എങ്കിൽ (am.in < 0) {
		status = error_errno(_("could not open '%s' for reading"),
				     rebased_patches);
		സ്വതന്ത്രം(rebased_patches);
		child_process_clear(&am);
		തിരികെ status;
	}

	strvec_pushv(&am.args, opts->git_am_opts.v);
	strvec_തള്ളുക(&am.args, "--rebasing");
	strvec_pushf(&am.args, "--resolvemsg=%s", rebase_resolvemsg);
	strvec_തള്ളുക(&am.args, "--patch-format=mboxrd");
	എങ്കിൽ (opts->allow_rerere_autoupdate == RERERE_AUTOUPDATE)
		strvec_തള്ളുക(&am.args, "--rerere-autoupdate");
	അല്ലെങ്കിൽ എങ്കിൽ (opts->allow_rerere_autoupdate == RERERE_NOAUTOUPDATE)
		strvec_തള്ളുക(&am.args, "--no-rerere-autoupdate");
	എങ്കിൽ (opts->gpg_sign_opt)
		strvec_തള്ളുക(&am.args, opts->gpg_sign_opt);
	status = run_command(&am);
	unlink(rebased_patches);
	സ്വതന്ത്രം(rebased_patches);

	എങ്കിൽ (!status) {
		തിരികെ move_to_original_branch(opts);
	}

	എങ്കിൽ (is_directory(opts->state_dir))
		rebase_write_basic_state(opts);

	തിരികെ status;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ run_specific_rebase(ഘടന rebase_options *opts)
{
	പൂർണ്ണസംഖ്യ status;

	എങ്കിൽ (opts->type == REBASE_MERGE) {
		/* Run sequencer-based rebase */
		എങ്കിൽ (!(opts->flags & REBASE_INTERACTIVE_EXPLICIT))
			setenv("GIT_SEQUENCE_EDITOR", ":", 1);
		എങ്കിൽ (opts->gpg_sign_opt) {
			/* remove the leading "-S" */
			അക്ഷരം *tmp = xstrdup(opts->gpg_sign_opt + 2);
			സ്വതന്ത്രം(opts->gpg_sign_opt);
			opts->gpg_sign_opt = tmp;
		}

		status = run_sequencer_rebase(opts);
	} അല്ലെങ്കിൽ എങ്കിൽ (opts->type == REBASE_APPLY)
		status = run_am(opts);
	അല്ലെങ്കിൽ
		BUG("Unhandled rebase type %d", opts->type);

	എങ്കിൽ (opts->dont_finish_rebase)
		; /* do nothing */
	അല്ലെങ്കിൽ എങ്കിൽ (opts->type == REBASE_MERGE) {
		പൂർണ്ണസംഖ്യ quiet = !(opts->flags & (REBASE_NO_QUIET|REBASE_VERBOSE));

		/*
		 * The sequencer cleans up after itself. Its state directory
		 * is gone once it is done, and stays while it is stopped.
		 */
		എങ്കിൽ (status == 0 && !is_directory(opts->state_dir))
			run_auto_maintenance(the_repository, quiet);
	} അല്ലെങ്കിൽ എങ്കിൽ (status == 0) {
		എങ്കിൽ (!file_exists(state_dir_path("stopped-sha", opts)))
			finish_rebase(opts);
	} അല്ലെങ്കിൽ എങ്കിൽ (status == 2) {
		ഘടന strbuf dir = STRBUF_INIT;

		apply_autostash(state_dir_path("autostash", opts));
		strbuf_addstr(&dir, opts->state_dir);
		remove_dir_recursively(&dir, 0);
		strbuf_release(&dir);
		die("Nothing to do");
	}

	തിരികെ status ? -1 : 0;
}

സ്ഥിതം ശൂന്യം parse_rebase_merges_മൂല്യം(ഘടന rebase_options *options, സ്ഥിരം അക്ഷരം *മൂല്യം)
{
	എങ്കിൽ (!strcmp("no-rebase-cousins", മൂല്യം))
		options->rebase_cousins = 0;
	അല്ലെങ്കിൽ എങ്കിൽ (!strcmp("rebase-cousins", മൂല്യം))
		options->rebase_cousins = 1;
	അല്ലെങ്കിൽ
		die(_("Unknown rebase-merges mode: %s"), മൂല്യം);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ rebase_config(സ്ഥിരം അക്ഷരം *var, സ്ഥിരം അക്ഷരം *മൂല്യം,
			 സ്ഥിരം ഘടന config_context *ctx, ശൂന്യം *data)
{
	ഘടന rebase_options *opts = data;

	എങ്കിൽ (!strcmp(var, "rebase.stat")) {
		എങ്കിൽ (git_config_bool(var, മൂല്യം))
			opts->flags |= REBASE_DIFFSTAT;
		അല്ലെങ്കിൽ
			opts->flags &= ~REBASE_DIFFSTAT;
		തിരികെ 0;
	}

	എങ്കിൽ (!strcmp(var, "rebase.autosquash")) {
		opts->config_autosquash = git_config_bool(var, മൂല്യം);
		തിരികെ 0;
	}

	എങ്കിൽ (!strcmp(var, "commit.gpgsign")) {
		സ്വതന്ത്രം(opts->gpg_sign_opt);
		opts->gpg_sign_opt = git_config_bool(var, മൂല്യം) ?
			xstrdup("-S") : ശൂന്യ;
		തിരികെ 0;
	}

	എങ്കിൽ (!strcmp(var, "rebase.autostash")) {
		opts->autostash = git_config_bool(var, മൂല്യം);
		തിരികെ 0;
	}

	എങ്കിൽ (!strcmp(var, "rebase.rebasemerges")) {
		opts->config_rebase_merges = git_parse_maybe_bool(മൂല്യം);
		എങ്കിൽ (opts->config_rebase_merges < 0) {
			opts->config_rebase_merges = 1;
			parse_rebase_merges_മൂല്യം(opts, മൂല്യം);
		} അല്ലെങ്കിൽ {
			opts->rebase_cousins = 0;
		}
		തിരികെ 0;
	}

	എങ്കിൽ (!strcmp(var, "rebase.updaterefs")) {
		opts->config_update_refs = git_config_bool(var, മൂല്യം);
		തിരികെ 0;
	}

	എങ്കിൽ (!strcmp(var, "rebase.reschedulefailedexec")) {
		opts->reschedule_failed_exec = git_config_bool(var, മൂല്യം);
		തിരികെ 0;
	}

	എങ്കിൽ (!strcmp(var, "rebase.forkpoint")) {
		opts->fork_point = git_config_bool(var, മൂല്യം) ? -1 : 0;
		തിരികെ 0;
	}

	എങ്കിൽ (!strcmp(var, "rebase.backend")) {
		FREE_AND_NULL(opts->default_backend);
		തിരികെ git_config_string(&opts->default_backend, var, മൂല്യം);
	}

	തിരികെ git_default_config(var, മൂല്യം, ctx, data);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ checkout_up_to_date(ഘടന rebase_options *options)
{
	ഘടന strbuf buf = STRBUF_INIT;
	ഘടന reset_working_വൃക്ഷം_options ropts = { 0 };
	പൂർണ്ണസംഖ്യ ret = 0;

	strbuf_addf(&buf, "%s: checkout %s",
		    options->reflog_action, options->switch_to);
	ropts.oid = &options->orig_head->object.oid;
	ropts.branch = options->head_name;
	ropts.flags = RESET_WORKING_വൃക്ഷം_RUN_POST_CHECKOUT_HOOK |
		      RESET_WORKING_വൃക്ഷം_UPDATE_HEAD;
	എങ്കിൽ (!ropts.branch)
		ropts.flags |=  RESET_WORKING_വൃക്ഷം_DETACH;
	ropts.head_സന്ദേശം = buf.buf;
	എങ്കിൽ (reset_working_വൃക്ഷം(the_repository, &ropts) < 0)
		ret = error(_("could not switch to %s"), options->switch_to);
	strbuf_release(&buf);

	തിരികെ ret;
}

/*
 * Determines whether the commits in from..to are linear, i.e. contain
 * no merge commits. This function *expects* `from` to be an ancestor of
 * `to`.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ is_linear_history(ഘടന commit *from, ഘടന commit *to)
{
	ആകുമ്പോൾ (to && to != from) {
		repo_parse_commit(the_repository, to);
		എങ്കിൽ (!to->parents)
			തിരികെ 1;
		എങ്കിൽ (to->parents->next)
			തിരികെ 0;
		to = to->parents->item;
	}
	തിരികെ 1;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ can_fast_forward(ഘടന commit *onto, ഘടന commit *upstream,
			    ഘടന commit *restrict_revision,
			    ഘടന commit *head, ഘടന object_id *branch_base)
{
	ഘടന commit_list *merge_bases = ശൂന്യ;
	പൂർണ്ണസംഖ്യ res = 0;

	എങ്കിൽ (is_null_oid(branch_base))
		പോകുക done; /* fill_branch_base() found multiple merge bases */

	എങ്കിൽ (!oideq(branch_base, &onto->object.oid))
		പോകുക done;

	എങ്കിൽ (restrict_revision && !oideq(&restrict_revision->object.oid, branch_base))
		പോകുക done;

	എങ്കിൽ (!upstream)
		പോകുക done;

	എങ്കിൽ (repo_get_merge_bases(the_repository, upstream, head, &merge_bases) < 0)
		exit(128);
	എങ്കിൽ (!merge_bases || merge_bases->next)
		പോകുക done;

	എങ്കിൽ (!oideq(&onto->object.oid, &merge_bases->item->object.oid))
		പോകുക done;

	res = 1;

done:
	commit_list_free(merge_bases);
	തിരികെ res && is_linear_history(onto, head);
}

സ്ഥിതം ശൂന്യം fill_branch_base(ഘടന rebase_options *options,
			    ഘടന object_id *branch_base)
{
	ഘടന commit_list *merge_bases = ശൂന്യ;

	എങ്കിൽ (repo_get_merge_bases(the_repository, options->onto,
				 options->orig_head, &merge_bases) < 0)
		exit(128);
	എങ്കിൽ (!merge_bases || merge_bases->next)
		oidcpy(branch_base, null_oid(the_hash_algo));
	അല്ലെങ്കിൽ
		oidcpy(branch_base, &merge_bases->item->object.oid);

	commit_list_free(merge_bases);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ parse_opt_am(സ്ഥിരം ഘടന option *opt, സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	ഘടന rebase_options *opts = opt->മൂല്യം;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	എങ്കിൽ (opts->type != REBASE_UNSPECIFIED && opts->type != REBASE_APPLY)
	    die(_("apply options and merge options cannot be used together"));

	opts->type = REBASE_APPLY;

	തിരികെ 0;
}

/* -i followed by -m is still -i */
സ്ഥിതം പൂർണ്ണസംഖ്യ parse_opt_merge(സ്ഥിരം ഘടന option *opt, സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	ഘടന rebase_options *opts = opt->മൂല്യം;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	എങ്കിൽ (opts->type != REBASE_UNSPECIFIED && opts->type != REBASE_MERGE)
	    die(_("apply options and merge options cannot be used together"));

	opts->type = REBASE_MERGE;

	തിരികെ 0;
}

/* -i followed by -r is still explicitly interactive, but -r alone is not */
സ്ഥിതം പൂർണ്ണസംഖ്യ parse_opt_interactive(സ്ഥിരം ഘടന option *opt, സ്ഥിരം അക്ഷരം *arg,
				 പൂർണ്ണസംഖ്യ unset)
{
	ഘടന rebase_options *opts = opt->മൂല്യം;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	എങ്കിൽ (opts->type != REBASE_UNSPECIFIED && opts->type != REBASE_MERGE)
	    die(_("apply options and merge options cannot be used together"));

	opts->type = REBASE_MERGE;
	opts->flags |= REBASE_INTERACTIVE_EXPLICIT;

	തിരികെ 0;
}

സ്ഥിതം എണ്ണൽ empty_type parse_empty_മൂല്യം(സ്ഥിരം അക്ഷരം *മൂല്യം)
{
	എങ്കിൽ (!strcasecmp(മൂല്യം, "drop"))
		തിരികെ EMPTY_DROP;
	അല്ലെങ്കിൽ എങ്കിൽ (!strcasecmp(മൂല്യം, "keep"))
		തിരികെ EMPTY_KEEP;
	അല്ലെങ്കിൽ എങ്കിൽ (!strcasecmp(മൂല്യം, "stop"))
		തിരികെ EMPTY_STOP;
	അല്ലെങ്കിൽ എങ്കിൽ (!strcasecmp(മൂല്യം, "ask")) {
		warning(_("--empty=ask is deprecated; use '--empty=stop' instead."));
		തിരികെ EMPTY_STOP;
	}

	die(_("unrecognized empty type '%s'; valid values are \"drop\", \"keep\", and \"stop\"."), മൂല്യം);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ parse_opt_keep_empty(സ്ഥിരം ഘടന option *opt, സ്ഥിരം അക്ഷരം *arg,
				പൂർണ്ണസംഖ്യ unset)
{
	ഘടന rebase_options *opts = opt->മൂല്യം;

	BUG_ON_OPT_ARG(arg);

	imply_merge(opts, unset ? "--no-keep-empty" : "--keep-empty");
	opts->keep_empty = !unset;
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ parse_opt_empty(സ്ഥിരം ഘടന option *opt, സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	ഘടന rebase_options *options = opt->മൂല്യം;
	എണ്ണൽ empty_type മൂല്യം = parse_empty_മൂല്യം(arg);

	BUG_ON_OPT_NEG(unset);

	options->empty = മൂല്യം;
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ parse_opt_rebase_merges(സ്ഥിരം ഘടന option *opt, സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	ഘടന rebase_options *options = opt->മൂല്യം;

	options->rebase_merges = !unset;
	options->rebase_cousins = 0;

	എങ്കിൽ (arg) {
		എങ്കിൽ (!*arg) {
			warning(_("--rebase-merges with an empty string "
				  "argument is deprecated and will stop "
				  "working in a future version of Git. Use "
				  "--rebase-merges without an argument "
				  "instead, which does the same thing."));
			തിരികെ 0;
		}
		parse_rebase_merges_മൂല്യം(options, arg);
	}

	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം NORETURN error_on_missing_default_upstream(ശൂന്യം)
{
	ഘടന branch *current_branch = branch_get(ശൂന്യ);

	printf(_("%s\n"
		 "Please specify which branch you want to rebase against.\n"
		 "See git-rebase(1) for details.\n"
		 "\n"
		 "    git rebase '<branch>'\n"
		 "\n"),
		current_branch ? _("There is no tracking information for "
			"the current branch.") :
			_("You are not currently on a branch."));

	എങ്കിൽ (current_branch) {
		സ്ഥിരം അക്ഷരം *remote = current_branch->remote_name;

		എങ്കിൽ (!remote)
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

സ്ഥിതം പൂർണ്ണസംഖ്യ check_exec_cmd(സ്ഥിരം അക്ഷരം *cmd)
{
	എങ്കിൽ (strchr(cmd, '\n'))
		തിരികെ error(_("exec commands cannot contain newlines"));

	/* Does the command consist purely of whitespace? */
	എങ്കിൽ (!cmd[strspn(cmd, " \t\r\f\v")])
		തിരികെ error(_("empty exec command"));

	തിരികെ 0;
}

പൂർണ്ണസംഖ്യ cmd_rebase(പൂർണ്ണസംഖ്യ argc,
	       സ്ഥിരം അക്ഷരം **argv,
	       സ്ഥിരം അക്ഷരം *prefix,
	       ഘടന repository *repo UNUSED)
{
	ഘടന rebase_options options = REBASE_OPTIONS_INIT;
	സ്ഥിരം അക്ഷരം *branch_name;
	സ്ഥിരം അക്ഷരം *strategy_opt = ശൂന്യ;
	പൂർണ്ണസംഖ്യ ret, flags, total_argc, in_progress = 0;
	പൂർണ്ണസംഖ്യ keep_base = 0;
	പൂർണ്ണസംഖ്യ ok_to_skip_pre_rebase = 0;
	ഘടന strbuf സന്ദേശം = STRBUF_INIT;
	ഘടന strbuf revisions = STRBUF_INIT;
	ഘടന strbuf buf = STRBUF_INIT;
	ഘടന object_id branch_base;
	പൂർണ്ണസംഖ്യ ignore_whitespace = 0;
	സ്ഥിരം അക്ഷരം *gpg_sign = ശൂന്യ;
	ഘടന object_id squash_onto;
	അക്ഷരം *squash_onto_name = ശൂന്യ;
	അക്ഷരം *keep_base_onto_name = ശൂന്യ;
	പൂർണ്ണസംഖ്യ reschedule_failed_exec = -1;
	പൂർണ്ണസംഖ്യ allow_preemptive_ff = 1;
	പൂർണ്ണസംഖ്യ preserve_merges_selected = 0;
	ഘടന reset_working_വൃക്ഷം_options ropts = { 0 };
	ഘടന option builtin_rebase_options[] = {
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
			.മൂല്യം = &options.flags,
			.precision = വലുപ്പം(options.flags),
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
		OPT_PASSTHRU_ARGV('C', ശൂന്യ, &options.git_am_opts, N_("n"),
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
		OPT_CALLBACK_F(0, "apply", &options, ശൂന്യ,
			N_("use apply strategies to rebase"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			parse_opt_am),
		OPT_CALLBACK_F('m', "merge", &options, ശൂന്യ,
			N_("use merging strategies to rebase"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			parse_opt_merge),
		OPT_CALLBACK_F('i', "interactive", &options, ശൂന്യ,
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
		OPT_CALLBACK_F('k', "keep-empty", &options, ശൂന്യ,
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
			.മൂല്യം = &gpg_sign,
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
		OPT_BOOL(0, "root", &options.വേര്,
			 N_("rebase all reachable commits up to the root(s)")),
		OPT_BOOL(0, "reschedule-failed-exec",
			 &reschedule_failed_exec,
			 N_("automatically re-schedule any `exec` that fails")),
		OPT_BOOL(0, "reapply-cherry-picks", &options.reapply_cherry_picks,
			 N_("apply all changes, even those already present upstream")),
		OPT_END(),
	};
	പൂർണ്ണസംഖ്യ i;

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
	gpg_sign = options.gpg_sign_opt ? "" : ശൂന്യ;
	FREE_AND_NULL(options.gpg_sign_opt);

	strbuf_reset(&buf);
	strbuf_addf(&buf, "%s/applying", apply_dir());
	എങ്കിൽ(file_exists(buf.buf))
		die(_("It looks like 'git am' is in progress. Cannot rebase."));

	എങ്കിൽ (is_directory(apply_dir())) {
		options.type = REBASE_APPLY;
		options.state_dir = apply_dir();
	} അല്ലെങ്കിൽ എങ്കിൽ (is_directory(merge_dir())) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "%s/rewritten", merge_dir());
		എങ്കിൽ (!(options.action == ACTION_ABORT) && is_directory(buf.buf)) {
			die(_("`rebase --preserve-merges` (-p) is no longer supported.\n"
			"Use `git rebase --abort` to terminate current rebase.\n"
			"Or downgrade to v2.33, or earlier, to complete the rebase."));
		} അല്ലെങ്കിൽ {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "%s/interactive", merge_dir());
			options.type = REBASE_MERGE;
			എങ്കിൽ (file_exists(buf.buf))
				options.flags |= REBASE_INTERACTIVE_EXPLICIT;
		}
		options.state_dir = merge_dir();
	}

	എങ്കിൽ (options.type != REBASE_UNSPECIFIED)
		in_progress = 1;

	total_argc = argc;
	argc = parse_options(argc, argv, prefix,
			     builtin_rebase_options,
			     builtin_rebase_usage, 0);

	എങ്കിൽ (options.trailer_args.nr) {
		എങ്കിൽ (validate_trailer_args(&options.trailer_args))
			die(ശൂന്യ);
		options.flags |= REBASE_FORCE;
	}

	എങ്കിൽ (preserve_merges_selected)
		die(_("--preserve-merges was replaced by --rebase-merges\n"
			"Note: Your `pull.rebase` configuration may also be set to 'preserve',\n"
			"which is no longer supported; use 'merges' instead"));

	എങ്കിൽ (options.action != ACTION_NONE && total_argc != 2) {
		usage_with_options(builtin_rebase_usage,
				   builtin_rebase_options);
	}

	എങ്കിൽ (argc > 2)
		usage_with_options(builtin_rebase_usage,
				   builtin_rebase_options);

	എങ്കിൽ (keep_base) {
		എങ്കിൽ (options.onto_name)
			die(_("options '%s' and '%s' cannot be used together"), "--keep-base", "--onto");
		എങ്കിൽ (options.വേര്)
			die(_("options '%s' and '%s' cannot be used together"), "--keep-base", "--root");
		/*
		 * --keep-base defaults to --no-fork-point to keep the
		 * base the same.
		 */
		എങ്കിൽ (options.fork_point < 0)
			options.fork_point = 0;
	}
	എങ്കിൽ (options.വേര് && options.fork_point > 0)
		die(_("options '%s' and '%s' cannot be used together"), "--root", "--fork-point");

	എങ്കിൽ (options.action != ACTION_NONE && !in_progress)
		die(_("no rebase in progress"));

	എങ്കിൽ (options.action == ACTION_EDIT_TODO && !is_merge(&options))
		die(_("The --edit-todo action can only be used during "
		      "interactive rebase."));

	എങ്കിൽ (trace2_is_enabled()) {
		എങ്കിൽ (is_merge(&options))
			trace2_cmd_mode("interactive");
		അല്ലെങ്കിൽ എങ്കിൽ (options.exec.nr)
			trace2_cmd_mode("interactive-exec");
		അല്ലെങ്കിൽ
			trace2_cmd_mode(action_names[options.action]);
	}

	options.reflog_action = getenv(GIT_REFLOG_ACTION_ENVIRONMENT);
	options.reflog_action =
		xstrdup(options.reflog_action ? options.reflog_action : "rebase");

	തിരഞ്ഞെടുപ്പ് (options.action) {
	അവസ്ഥ ACTION_CONTINUE: {
		ഘടന object_id head;
		ഘടന lock_file lock_file = LOCK_INIT;
		പൂർണ്ണസംഖ്യ fd;

		/* Sanity check */
		എങ്കിൽ (repo_get_oid(the_repository, "HEAD", &head))
			die(_("Cannot read HEAD"));

		fd = repo_hold_locked_index(the_repository, &lock_file, 0);
		എങ്കിൽ (repo_read_index(the_repository) < 0)
			die(_("could not read index"));
		refresh_index(the_repository->index, REFRESH_QUIET, ശൂന്യ, ശൂന്യ,
			      ശൂന്യ);
		എങ്കിൽ (0 <= fd)
			repo_update_index_if_able(the_repository, &lock_file);
		rollback_lock_file(&lock_file);

		എങ്കിൽ (has_unstaged_changes(the_repository, 1)) {
			puts(_("You must edit all merge conflicts and then\n"
			       "mark them as resolved using git add"));
			exit(1);
		}
		എങ്കിൽ (read_basic_state(&options))
			exit(1);
		പോകുക run_rebase;
	}
	അവസ്ഥ ACTION_SKIP: {
		ഘടന string_list merge_rr = STRING_LIST_INIT_DUP;

		rerere_clear(the_repository, &merge_rr);
		string_list_clear(&merge_rr, 1);
		ropts.flags = RESET_WORKING_വൃക്ഷം_HARD |
			      RESET_WORKING_വൃക്ഷം_UPDATE_HEAD;
		എങ്കിൽ (reset_working_വൃക്ഷം(the_repository, &ropts) < 0)
			die(_("could not discard worktree changes"));
		remove_branch_state(the_repository, 0);
		എങ്കിൽ (read_basic_state(&options))
			exit(1);
		പോകുക run_rebase;
	}
	അവസ്ഥ ACTION_ABORT: {
		ഘടന string_list merge_rr = STRING_LIST_INIT_DUP;
		ഘടന strbuf head_സന്ദേശം = STRBUF_INIT;

		rerere_clear(the_repository, &merge_rr);
		string_list_clear(&merge_rr, 1);

		എങ്കിൽ (read_basic_state(&options))
			exit(1);

		strbuf_addf(&head_സന്ദേശം, "%s (abort): returning to %s",
			    options.reflog_action,
			    options.head_name ? options.head_name
					      : oid_to_hex(&options.orig_head->object.oid));
		ropts.oid = &options.orig_head->object.oid;
		ropts.head_സന്ദേശം = head_സന്ദേശം.buf;
		ropts.branch = options.head_name;
		ropts.flags = RESET_WORKING_വൃക്ഷം_HARD |
			      RESET_WORKING_വൃക്ഷം_UPDATE_HEAD;
		എങ്കിൽ (reset_working_വൃക്ഷം(the_repository, &ropts) < 0)
			die(_("could not move back to %s"),
			    oid_to_hex(&options.orig_head->object.oid));
		strbuf_release(&head_സന്ദേശം);
		remove_branch_state(the_repository, 0);
		ret = finish_rebase(&options);
		പോകുക cleanup;
	}
	അവസ്ഥ ACTION_QUIT: {
		save_autostash(state_dir_path("autostash", &options));
		എങ്കിൽ (options.type == REBASE_MERGE) {
			ഘടന replay_opts replay = REPLAY_OPTS_INIT;

			replay.action = REPLAY_INTERACTIVE_REBASE;
			ret = sequencer_remove_state(&replay);
			replay_opts_release(&replay);
		} അല്ലെങ്കിൽ {
			strbuf_reset(&buf);
			strbuf_addstr(&buf, options.state_dir);
			ret = remove_dir_recursively(&buf, 0);
			എങ്കിൽ (ret)
				error(_("could not remove '%s'"),
				       options.state_dir);
		}
		പോകുക cleanup;
	}
	അവസ്ഥ ACTION_EDIT_TODO:
		options.dont_finish_rebase = 1;
		പോകുക run_rebase;
	അവസ്ഥ ACTION_SHOW_CURRENT_PATCH:
		options.dont_finish_rebase = 1;
		പോകുക run_rebase;
	അവസ്ഥ ACTION_NONE:
		നിർത്തുക;
	സ്ഥിരസ്ഥിതി:
		BUG("action: %d", options.action);
	}

	/* Make sure no rebase is in progress */
	എങ്കിൽ (in_progress) {
		സ്ഥിരം അക്ഷരം *last_slash = strrchr(options.state_dir, '/');
		സ്ഥിരം അക്ഷരം *state_dir_base =
			last_slash ? last_slash + 1 : options.state_dir;
		സ്ഥിരം അക്ഷരം *cmd_live_rebase =
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

	എങ്കിൽ ((options.flags & REBASE_INTERACTIVE_EXPLICIT) ||
	    (options.action != ACTION_NONE) ||
	    (options.exec.nr > 0) ||
	    options.autosquash == 1) {
		allow_preemptive_ff = 0;
	}
	എങ്കിൽ (options.committer_date_is_author_date || options.ignore_date)
		options.flags |= REBASE_FORCE;

	വേണ്ടി (i = 0; i < options.git_am_opts.nr; i++) {
		സ്ഥിരം അക്ഷരം *option = options.git_am_opts.v[i], *p;
		എങ്കിൽ (!strcmp(option, "--whitespace=fix") ||
		    !strcmp(option, "--whitespace=strip"))
			allow_preemptive_ff = 0;
		അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(option, "-C", &p)) {
			ആകുമ്പോൾ (*p)
				എങ്കിൽ (!isdigit(*(p++)))
					die(_("switch `C' expects a "
					      "numerical value"));
		} അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(option, "--whitespace=", &p)) {
			എങ്കിൽ (*p && strcmp(p, "warn") && strcmp(p, "nowarn") &&
			    strcmp(p, "error") && strcmp(p, "error-all"))
				die("Invalid whitespace option: '%s'", p);
		}
	}

	വേണ്ടി (i = 0; i < options.exec.nr; i++)
		എങ്കിൽ (check_exec_cmd(options.exec.ഇനങ്ങൾ[i].string))
			exit(1);

	എങ്കിൽ (!(options.flags & REBASE_NO_QUIET))
		strvec_തള്ളുക(&options.git_am_opts, "-q");

	എങ്കിൽ (options.empty != EMPTY_UNSPECIFIED)
		imply_merge(&options, "--empty");

	എങ്കിൽ (options.reapply_cherry_picks < 0)
		/*
		 * We default to --no-reapply-cherry-picks unless
		 * --keep-base is given; when --keep-base is given, we want
		 * to default to --reapply-cherry-picks.
		 */
		options.reapply_cherry_picks = keep_base;
	അല്ലെങ്കിൽ എങ്കിൽ (!keep_base)
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

	എങ്കിൽ (gpg_sign)
		options.gpg_sign_opt = xstrfmt("-S%s", gpg_sign);

	എങ്കിൽ (options.exec.nr)
		imply_merge(&options, "--exec");

	എങ്കിൽ (options.type == REBASE_APPLY) {
		എങ്കിൽ (ignore_whitespace)
			strvec_തള്ളുക(&options.git_am_opts,
				    "--ignore-whitespace");
		എങ്കിൽ (options.committer_date_is_author_date)
			strvec_തള്ളുക(&options.git_am_opts,
				    "--committer-date-is-author-date");
		എങ്കിൽ (options.ignore_date)
			strvec_തള്ളുക(&options.git_am_opts, "--ignore-date");
	} അല്ലെങ്കിൽ {
		/* REBASE_MERGE */
		എങ്കിൽ (ignore_whitespace) {
			string_list_append(&options.strategy_opts,
					   "ignore-space-change");
		}
	}

	എങ്കിൽ (strategy_opt)
		options.strategy = xstrdup(strategy_opt);
	അല്ലെങ്കിൽ എങ്കിൽ (options.strategy_opts.nr && !options.strategy)
		options.strategy = xstrdup("ort");
	എങ്കിൽ (options.strategy)
		imply_merge(&options, "--strategy");

	എങ്കിൽ (options.വേര് && !options.onto_name)
		imply_merge(&options, "--root without --onto");

	എങ്കിൽ (options.trailer_args.nr)
		imply_merge(&options, "--trailer");

	എങ്കിൽ (isatty(2) && options.flags & REBASE_NO_QUIET)
		strbuf_addstr(&options.git_format_patch_opt, " --progress");

	എങ്കിൽ (options.git_am_opts.nr || options.type == REBASE_APPLY) {
		/* all am options except -q are compatible only with --apply */
		വേണ്ടി (i = options.git_am_opts.nr - 1; i >= 0; i--)
			എങ്കിൽ (strcmp(options.git_am_opts.v[i], "-q"))
				നിർത്തുക;

		എങ്കിൽ (i >= 0 || options.type == REBASE_APPLY) {
			എങ്കിൽ (is_merge(&options))
				die(_("apply options and merge options "
					  "cannot be used together"));
			അല്ലെങ്കിൽ എങ്കിൽ (options.rebase_merges == -1 && options.config_rebase_merges == 1)
				die(_("apply options are incompatible with rebase.rebaseMerges.  Consider adding --no-rebase-merges"));
			അല്ലെങ്കിൽ എങ്കിൽ (options.update_refs == -1 && options.config_update_refs == 1)
				die(_("apply options are incompatible with rebase.updateRefs.  Consider adding --no-update-refs"));
			അല്ലെങ്കിൽ
				options.type = REBASE_APPLY;
		}
	}

	എങ്കിൽ (options.update_refs == 1)
		imply_merge(&options, "--update-refs");
	options.update_refs = (options.update_refs >= 0) ? options.update_refs :
			     ((options.config_update_refs >= 0) ? options.config_update_refs : 0);

	എങ്കിൽ (options.rebase_merges == 1)
		imply_merge(&options, "--rebase-merges");
	options.rebase_merges = (options.rebase_merges >= 0) ? options.rebase_merges :
				((options.config_rebase_merges >= 0) ? options.config_rebase_merges : 0);

	എങ്കിൽ (options.autosquash == 1) {
		imply_merge(&options, "--autosquash");
	} അല്ലെങ്കിൽ എങ്കിൽ (options.autosquash == -1) {
		options.autosquash =
			options.config_autosquash &&
			(options.flags & REBASE_INTERACTIVE_EXPLICIT);
	}

	എങ്കിൽ (options.type == REBASE_UNSPECIFIED) {
		എങ്കിൽ (!strcmp(options.default_backend, "merge"))
			options.type = REBASE_MERGE;
		അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(options.default_backend, "apply"))
			options.type = REBASE_APPLY;
		അല്ലെങ്കിൽ
			die(_("Unknown rebase backend: %s"),
			    options.default_backend);
	}

	തിരഞ്ഞെടുപ്പ് (options.type) {
	അവസ്ഥ REBASE_MERGE:
		options.state_dir = merge_dir();
		നിർത്തുക;
	അവസ്ഥ REBASE_APPLY:
		options.state_dir = apply_dir();
		നിർത്തുക;
	സ്ഥിരസ്ഥിതി:
		BUG("options.type was just set above; should be unreachable.");
	}

	എങ്കിൽ (options.empty == EMPTY_UNSPECIFIED) {
		എങ്കിൽ (options.flags & REBASE_INTERACTIVE_EXPLICIT)
			options.empty = EMPTY_STOP;
		അല്ലെങ്കിൽ എങ്കിൽ (options.exec.nr > 0)
			options.empty = EMPTY_KEEP;
		അല്ലെങ്കിൽ
			options.empty = EMPTY_DROP;
	}
	എങ്കിൽ (reschedule_failed_exec > 0 && !is_merge(&options))
		die(_("--reschedule-failed-exec requires "
		      "--exec or --interactive"));
	എങ്കിൽ (reschedule_failed_exec >= 0)
		options.reschedule_failed_exec = reschedule_failed_exec;

	എങ്കിൽ (options.signoff) {
		strvec_തള്ളുക(&options.git_am_opts, "--signoff");
		options.flags |= REBASE_FORCE;
	}

	എങ്കിൽ (!options.വേര്) {
		എങ്കിൽ (argc < 1) {
			ഘടന branch *branch;

			branch = branch_get(ശൂന്യ);
			options.upstream_name = branch_get_upstream(branch,
								    ശൂന്യ);
			എങ്കിൽ (!options.upstream_name)
				error_on_missing_default_upstream();
			എങ്കിൽ (options.fork_point < 0)
				options.fork_point = 1;
		} അല്ലെങ്കിൽ {
			options.upstream_name = argv[0];
			argc--;
			argv++;
			എങ്കിൽ (!strcmp(options.upstream_name, "-"))
				options.upstream_name = "@{-1}";
		}
		options.upstream =
			lookup_commit_reference_by_name(options.upstream_name);
		എങ്കിൽ (!options.upstream)
			die(_("invalid upstream '%s'"), options.upstream_name);
		options.upstream_arg = options.upstream_name;
	} അല്ലെങ്കിൽ {
		എങ്കിൽ (!options.onto_name) {
			എങ്കിൽ (commit_വൃക്ഷം("", 0, the_hash_algo->empty_വൃക്ഷം, ശൂന്യ,
					&squash_onto, ശൂന്യ, ശൂന്യ) < 0)
				die(_("Could not create new root commit"));
			options.squash_onto = &squash_onto;
			options.onto_name = squash_onto_name =
				xstrdup(oid_to_hex(&squash_onto));
		} അല്ലെങ്കിൽ
			options.വേര്_with_onto = 1;

		options.upstream_name = ശൂന്യ;
		options.upstream = ശൂന്യ;
		എങ്കിൽ (argc > 1)
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
	എങ്കിൽ (argc == 1) {
		/* Is it "rebase other branchname" or "rebase other commit"? */
		ഘടന object_id branch_oid;
		branch_name = argv[0];
		options.switch_to = argv[0];

		/* Is it a local branch? */
		strbuf_reset(&buf);
		strbuf_addf(&buf, "refs/heads/%s", branch_name);
		എങ്കിൽ (!refs_read_ref(get_main_ref_store(the_repository), buf.buf, &branch_oid)) {
			die_if_checked_out(buf.buf, 1);
			options.head_name = xstrdup(buf.buf);
			options.orig_head =
				lookup_commit_object(the_repository,
						     &branch_oid);
		/* If not is it a valid ref (branch or commit)? */
		} അല്ലെങ്കിൽ {
			options.orig_head =
				lookup_commit_reference_by_name(branch_name);
			options.head_name = ശൂന്യ;
		}
		എങ്കിൽ (!options.orig_head)
			die(_("no such branch/commit '%s'"), branch_name);
	} അല്ലെങ്കിൽ എങ്കിൽ (argc == 0) {
		/* Do not need to switch branches, we are already on it. */
		options.head_name =
			xstrdup_or_null(refs_resolve_ref_unsafe(get_main_ref_store(the_repository), "HEAD", 0, ശൂന്യ,
								&flags));
		എങ്കിൽ (!options.head_name)
			die(_("No such ref: %s"), "HEAD");
		എങ്കിൽ (flags & REF_ISSYMREF) {
			എങ്കിൽ (!skip_prefix(options.head_name,
					 "refs/heads/", &branch_name))
				branch_name = options.head_name;

		} അല്ലെങ്കിൽ {
			FREE_AND_NULL(options.head_name);
			branch_name = "HEAD";
		}
		options.orig_head = lookup_commit_reference_by_name("HEAD");
		എങ്കിൽ (!options.orig_head)
			die(_("Could not resolve HEAD to a commit"));
	} അല്ലെങ്കിൽ
		BUG("unexpected number of arguments left to parse");

	/* Make sure the branch to rebase onto is valid. */
	എങ്കിൽ (keep_base) {
		strbuf_reset(&buf);
		strbuf_addstr(&buf, options.upstream_name);
		strbuf_addstr(&buf, "...");
		strbuf_addstr(&buf, branch_name);
		options.onto_name = keep_base_onto_name = xstrdup(buf.buf);
	} അല്ലെങ്കിൽ എങ്കിൽ (!options.onto_name)
		options.onto_name = options.upstream_name;
	എങ്കിൽ (strstr(options.onto_name, "...")) {
		എങ്കിൽ (repo_get_oid_mb(the_repository, options.onto_name, &branch_base) < 0) {
			എങ്കിൽ (keep_base)
				die(_("'%s': need exactly one merge base with branch"),
				    options.upstream_name);
			അല്ലെങ്കിൽ
				die(_("'%s': need exactly one merge base"),
				    options.onto_name);
		}
		options.onto = lookup_commit_or_die(&branch_base,
						    options.onto_name);
	} അല്ലെങ്കിൽ {
		options.onto =
			lookup_commit_reference_by_name(options.onto_name);
		എങ്കിൽ (!options.onto)
			die(_("Does not point to a valid commit '%s'"),
				options.onto_name);
		fill_branch_base(&options, &branch_base);
	}

	എങ്കിൽ (keep_base && options.reapply_cherry_picks)
		options.upstream = options.onto;

	എങ്കിൽ (options.fork_point > 0)
		options.restrict_revision =
			get_fork_point(options.upstream_name, options.orig_head);

	എങ്കിൽ (repo_read_index(the_repository) < 0)
		die(_("could not read index"));

	എങ്കിൽ (options.autostash)
		create_autostash(the_repository,
				 state_dir_path("autostash", &options));


	എങ്കിൽ (require_clean_work_വൃക്ഷം(the_repository, "rebase",
				    _("Please commit or stash them."), 1, 1)) {
		ret = -1;
		പോകുക cleanup_autostash;
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
	എങ്കിൽ (allow_preemptive_ff &&
	    can_fast_forward(options.onto, options.upstream, options.restrict_revision,
			     options.orig_head, &branch_base)) {
		പൂർണ്ണസംഖ്യ flag;

		എങ്കിൽ (!(options.flags & REBASE_FORCE)) {
			/* Lazily switch to the target branch if needed... */
			എങ്കിൽ (options.switch_to) {
				ret = checkout_up_to_date(&options);
				എങ്കിൽ (ret)
					പോകുക cleanup_autostash;
			}

			എങ്കിൽ (!(options.flags & REBASE_NO_QUIET))
				; /* be quiet */
			അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(branch_name, "HEAD") &&
				 refs_resolve_ref_unsafe(get_main_ref_store(the_repository), "HEAD", 0, ശൂന്യ, &flag))
				puts(_("HEAD is up to date."));
			അല്ലെങ്കിൽ
				printf(_("Current branch %s is up to date.\n"),
				       branch_name);
			ret = finish_rebase(&options);
			പോകുക cleanup;
		} അല്ലെങ്കിൽ എങ്കിൽ (!(options.flags & REBASE_NO_QUIET))
			; /* be quiet */
		അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(branch_name, "HEAD") &&
			 refs_resolve_ref_unsafe(get_main_ref_store(the_repository), "HEAD", 0, ശൂന്യ, &flag))
			puts(_("HEAD is up to date, rebase forced."));
		അല്ലെങ്കിൽ
			printf(_("Current branch %s is up to date, rebase "
				 "forced.\n"), branch_name);
	}

	/* If a hook exists, give it a chance to interrupt*/
	എങ്കിൽ (!ok_to_skip_pre_rebase &&
	    run_hooks_l(the_repository, "pre-rebase", options.upstream_arg,
			argc ? argv[0] : ശൂന്യ, ശൂന്യ)) {
		ret = error(_("The pre-rebase hook refused to rebase."));
		പോകുക cleanup_autostash;
	}

	എങ്കിൽ (options.flags & REBASE_DIFFSTAT) {
		ഘടന diff_options opts;

		എങ്കിൽ (options.flags & REBASE_VERBOSE) {
			എങ്കിൽ (is_null_oid(&branch_base))
				printf(_("Changes to %s:\n"),
				       oid_to_hex(&options.onto->object.oid));
			അല്ലെങ്കിൽ
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
		diff_വൃക്ഷം_oid(is_null_oid(&branch_base) ?
			      the_hash_algo->empty_വൃക്ഷം : &branch_base,
			      &options.onto->object.oid, "", &opts);
		diffcore_std(&opts);
		diff_flush(&opts);
	}

	എങ്കിൽ (is_merge(&options))
		പോകുക run_rebase;

	/* Detach HEAD and reset the tree */
	എങ്കിൽ (options.flags & REBASE_NO_QUIET)
		printf(_("First, rewinding head to replay your work on top of "
			 "it...\n"));

	strbuf_addf(&സന്ദേശം, "%s (start): checkout %s",
		    options.reflog_action, options.onto_name);
	ropts.oid = &options.onto->object.oid;
	ropts.orig_head = &options.orig_head->object.oid;
	ropts.flags = RESET_WORKING_വൃക്ഷം_DETACH |
		      RESET_WORKING_വൃക്ഷം_UPDATE_HEAD |
		      RESET_WORKING_വൃക്ഷം_UPDATE_ORIG_HEAD |
		      RESET_WORKING_വൃക്ഷം_RUN_POST_CHECKOUT_HOOK;
	ropts.head_സന്ദേശം = സന്ദേശം.buf;
	ropts.default_reflog_action = options.reflog_action;
	എങ്കിൽ (reset_working_വൃക്ഷം(the_repository, &ropts)) {
		ret = error(_("Could not detach HEAD"));
		പോകുക cleanup_autostash;
	}

	/*
	 * If the onto is a proper descendant of the tip of the branch, then
	 * we just fast-forwarded.
	 */
	എങ്കിൽ (oideq(&branch_base, &options.orig_head->object.oid)) {
		printf(_("Fast-forwarded %s to %s.\n"),
			branch_name, options.onto_name);
		move_to_original_branch(&options);
		ret = finish_rebase(&options);
		പോകുക cleanup;
	}

	strbuf_addf(&revisions, "%s..%s",
		    options.വേര് ? oid_to_hex(&options.onto->object.oid) :
		    (options.restrict_revision ?
		     oid_to_hex(&options.restrict_revision->object.oid) :
		     oid_to_hex(&options.upstream->object.oid)),
		    oid_to_hex(&options.orig_head->object.oid));

	options.revisions = revisions.buf;

run_rebase:
	ret = run_specific_rebase(&options);

cleanup:
	strbuf_release(&buf);
	strbuf_release(&സന്ദേശം);
	strbuf_release(&revisions);
	rebase_options_release(&options);
	സ്വതന്ത്രം(squash_onto_name);
	സ്വതന്ത്രം(keep_base_onto_name);
	തിരികെ !!ret;

cleanup_autostash:
	ret |= !!cleanup_autostash(&options);
	പോകുക cleanup;
}
