/*
 * "git rebase" builtin command
 *
 * Copyright (c) 2018 Pratik Karki
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DISABLE_SIGN_COMPARE_WARNINGS

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "abspath.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hex.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "run-command.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "strvec.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "dir.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "refs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "unpack-trees.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "lockfile.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-file.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-name.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "path.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "commit.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "diff.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "wt-status.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "revision.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "commit-reach.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "rerere.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "branch.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "sequencer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "rebase-interactive.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "reset.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "trace2.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hook.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "trailer.h"

ਸਥਿਰ ਅੱਖਰ ਅਚਲ * ਅਚਲ builtin_rebase_usage[] = {
	N_("git rebase [-i] [options] [--exec <cmd>] "
		"[--onto <newbase> | --keep-base] [<upstream> [<branch>]]"),
	N_("git rebase [-i] [options] [--exec <cmd>] [--onto <newbase>] "
		"--root [<branch>]"),
	"git rebase --continue | --abort | --skip | --edit-todo",
	ਨਿਰਾ
};

ਸਥਿਰ GIT_ਰਸਤਾ_FUNC(ਰਸਤਾ_squash_onto, "rebase-merge/squash-onto")
ਸਥਿਰ GIT_ਰਸਤਾ_FUNC(ਰਸਤਾ_interactive, "rebase-merge/interactive")
ਸਥਿਰ GIT_ਰਸਤਾ_FUNC(apply_dir, "rebase-apply")
ਸਥਿਰ GIT_ਰਸਤਾ_FUNC(ਮਿਲਾਓ_dir, "rebase-merge")

ਗਿਣਤੀ rebase_type {
	REBASE_UNSPECIFIED = -1,
	REBASE_APPLY,
	REBASE_ਮਿਲਾਓ
};

ਗਿਣਤੀ ਖਾਲੀ_ਥਾਵਾਂ_type {
	ਖਾਲੀ_ਥਾਵਾਂ_UNSPECIFIED = -1,
	ਖਾਲੀ_ਥਾਵਾਂ_DROP,
	ਖਾਲੀ_ਥਾਵਾਂ_KEEP,
	ਖਾਲੀ_ਥਾਵਾਂ_STOP
};

ਗਿਣਤੀ action {
	ACTION_NONE = 0,
	ACTION_CONTINUE,
	ACTION_SKIP,
	ACTION_ABORT,
	ACTION_QUIT,
	ACTION_EDIT_TODO,
	ACTION_SHOW_CURRENT_PATCH
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *action_ਨਾਂ[] = {
	"undefined",
	"continue",
	"skip",
	"abort",
	"quit",
	"edit_todo",
	"show_current_patch"
};

ਬਣਤਰ rebase_options {
	ਗਿਣਤੀ rebase_type type;
	ਗਿਣਤੀ ਖਾਲੀ_ਥਾਵਾਂ_type ਖਾਲੀ_ਥਾਵਾਂ;
	ਅੱਖਰ *default_backend;
	ਅਚਲ ਅੱਖਰ *state_dir;
	ਬਣਤਰ commit *upstream;
	ਅਚਲ ਅੱਖਰ *upstream_name;
	ਅਚਲ ਅੱਖਰ *upstream_arg;
	ਅੱਖਰ *ਸਿਰ_name;
	ਬਣਤਰ commit *orig_ਸਿਰ;
	ਬਣਤਰ commit *onto;
	ਅਚਲ ਅੱਖਰ *onto_name;
	ਅਚਲ ਅੱਖਰ *revisions;
	ਅਚਲ ਅੱਖਰ *switch_ਮੰਜ਼ਿਲ;
	ਪੂਰਨ ਜੜ੍ਹ, ਜੜ੍ਹ_with_onto;
	ਬਣਤਰ object_id *squash_onto;
	ਬਣਤਰ commit *restrict_revision;
	ਪੂਰਨ dont_ਅੰਤ_rebase;
	ਗਿਣਤੀ {
		REBASE_NO_QUIET = 1<<0,
		REBASE_VERBOSE = 1<<1,
		REBASE_DIFFSTAT = 1<<2,
		REBASE_FORCE = 1<<3,
		REBASE_INTERACTIVE_EXPLICIT = 1<<4,
	} flags;
	ਬਣਤਰ strvec git_am_opts;
	ਗਿਣਤੀ action action;
	ਅੱਖਰ *reflog_action;
	ਪੂਰਨ signoff;
	ਬਣਤਰ strvec trailer_ਦਲੀਲਾਂ;
	ਪੂਰਨ allow_rerere_autoupdate;
	ਪੂਰਨ keep_ਖਾਲੀ_ਥਾਵਾਂ;
	ਪੂਰਨ autosquash;
	ਅੱਖਰ *gpg_sign_opt;
	ਪੂਰਨ autostash;
	ਪੂਰਨ committer_date_is_author_date;
	ਪੂਰਨ ignore_date;
	ਬਣਤਰ string_list exec;
	ਪੂਰਨ allow_ਖਾਲੀ_ਥਾਵਾਂ_ਸੁਨੇਹਾ;
	ਪੂਰਨ rebase_merges, rebase_cousins;
	ਅੱਖਰ *strategy;
	ਬਣਤਰ string_list strategy_opts;
	ਬਣਤਰ strbuf git_format_patch_opt;
	ਪੂਰਨ reschedule_failed_exec;
	ਪੂਰਨ reapply_cherry_picks;
	ਪੂਰਨ fork_ਬਿੰਦੂ;
	ਪੂਰਨ update_refs;
	ਪੂਰਨ config_autosquash;
	ਪੂਰਨ config_rebase_merges;
	ਪੂਰਨ config_update_refs;
};

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ REBASE_OPTIONS_INIT {			  	\
		.type = REBASE_UNSPECIFIED,	  	\
		.ਖਾਲੀ_ਥਾਵਾਂ = ਖਾਲੀ_ਥਾਵਾਂ_UNSPECIFIED,	  	\
		.keep_ਖਾਲੀ_ਥਾਵਾਂ = 1,			\
		.default_backend = xstrdup("merge"),  	\
		.flags = REBASE_NO_QUIET, 		\
		.git_am_opts = STRVEC_INIT,		\
		.exec = STRING_LIST_INIT_NODUP,		\
		.trailer_ਦਲੀਲਾਂ = STRVEC_INIT,		\
		.git_format_patch_opt = STRBUF_INIT,	\
		.fork_ਬਿੰਦੂ = -1,			\
		.reapply_cherry_picks = -1,             \
		.allow_ਖਾਲੀ_ਥਾਵਾਂ_ਸੁਨੇਹਾ = 1,               \
		.autosquash = -1,                       \
		.rebase_merges = -1,                    \
		.config_rebase_merges = -1,             \
		.update_refs = -1,                      \
		.config_update_refs = -1,               \
		.strategy_opts = STRING_LIST_INIT_NODUP,\
	}

ਸਥਿਰ ਖਾਲੀ rebase_options_release(ਬਣਤਰ rebase_options *opts)
{
	ਮੁਕਤ(opts->default_backend);
	ਮੁਕਤ(opts->reflog_action);
	ਮੁਕਤ(opts->ਸਿਰ_name);
	strvec_clear(&opts->git_am_opts);
	ਮੁਕਤ(opts->gpg_sign_opt);
	string_list_clear(&opts->exec, 0);
	ਮੁਕਤ(opts->strategy);
	string_list_clear(&opts->strategy_opts, 0);
	strbuf_release(&opts->git_format_patch_opt);
	strvec_clear(&opts->trailer_ਦਲੀਲਾਂ);
}

ਸਥਿਰ ਬਣਤਰ replay_opts get_replay_opts(ਅਚਲ ਬਣਤਰ rebase_options *opts)
{
	ਬਣਤਰ replay_opts replay = REPLAY_OPTS_INIT;

	replay.action = REPLAY_INTERACTIVE_REBASE;
	replay.strategy = ਨਿਰਾ;
	sequencer_init_config(&replay);

	replay.signoff = opts->signoff;

	strvec_pushv(&replay.trailer_ਦਲੀਲਾਂ, opts->trailer_ਦਲੀਲਾਂ.v);

	replay.allow_ff = !(opts->flags & REBASE_FORCE);
	ਜੇ (opts->allow_rerere_autoupdate)
		replay.allow_rerere_auto = opts->allow_rerere_autoupdate;
	replay.allow_ਖਾਲੀ_ਥਾਵਾਂ = 1;
	replay.allow_ਖਾਲੀ_ਥਾਵਾਂ_ਸੁਨੇਹਾ = opts->allow_ਖਾਲੀ_ਥਾਵਾਂ_ਸੁਨੇਹਾ;
	replay.drop_redundant_commits = (opts->ਖਾਲੀ_ਥਾਵਾਂ == ਖਾਲੀ_ਥਾਵਾਂ_DROP);
	replay.keep_redundant_commits = (opts->ਖਾਲੀ_ਥਾਵਾਂ == ਖਾਲੀ_ਥਾਵਾਂ_KEEP);
	replay.quiet = !(opts->flags & REBASE_NO_QUIET);
	replay.verbose = opts->flags & REBASE_VERBOSE;
	replay.reschedule_failed_exec = opts->reschedule_failed_exec;
	replay.committer_date_is_author_date =
					opts->committer_date_is_author_date;
	replay.ignore_date = opts->ignore_date;
	ਮੁਕਤ(replay.gpg_sign);
	replay.gpg_sign = xstrdup_or_null(opts->gpg_sign_opt);
	replay.reflog_action = xstrdup(opts->reflog_action);
	ਜੇ (opts->strategy)
		replay.strategy = xstrdup_or_null(opts->strategy);
	ਨਹੀਂਤਾਂ ਜੇ (!replay.strategy && replay.default_strategy) {
		replay.strategy = replay.default_strategy;
		replay.default_strategy = ਨਿਰਾ;
	}

	ਲਈ (ਮਾਪ_t i = 0; i < opts->strategy_opts.nr; i++)
		strvec_ਧੱਕੋ(&replay.xopts, opts->strategy_opts.ਚੀਜ਼ਾਂ[i].string);

	ਜੇ (opts->squash_onto) {
		oidcpy(&replay.squash_onto, opts->squash_onto);
		replay.have_squash_onto = 1;
	}

	ਵਾਪਸ replay;
}

ਸਥਿਰ ਪੂਰਨ edit_todo_file(ਬਿਨਾਨਿਸ਼ਾਨੀ flags, ਬਣਤਰ replay_opts *opts)
{
	ਅਚਲ ਅੱਖਰ *todo_file = rebase_ਰਸਤਾ_todo();
	ਬਣਤਰ todo_list todo_list = TODO_LIST_INIT,
		new_todo = TODO_LIST_INIT;
	ਪੂਰਨ res = 0;

	ਜੇ (strbuf_read_file(&todo_list.buf, todo_file, 0) < 0)
		ਵਾਪਸ error_errno(_("could not read '%s'."), todo_file);

	strbuf_stripspace(&todo_list.buf, comment_ਸਤਰ_str);
	res = edit_todo_list(the_repository, opts, &todo_list, &new_todo,
			     ਨਿਰਾ, ਨਿਰਾ, flags);
	ਜੇ (!res && todo_list_write_ਮੰਜ਼ਿਲ_file(the_repository, &new_todo, todo_file,
					    ਨਿਰਾ, ਨਿਰਾ, -1, flags & ~(TODO_LIST_SHORTEN_IDS)))
		res = error_errno(_("could not write '%s'"), todo_file);

	todo_list_release(&todo_list);
	todo_list_release(&new_todo);

	ਵਾਪਸ res;
}

ਸਥਿਰ ਪੂਰਨ get_revision_ranges(ਬਣਤਰ commit *upstream, ਬਣਤਰ commit *onto,
			       ਬਣਤਰ object_id *orig_ਸਿਰ, ਅੱਖਰ **revisions,
			       ਅੱਖਰ **shortrevisions)
{
	ਬਣਤਰ commit *ਅਧਾਰ_rev = upstream ? upstream : onto;
	ਅਚਲ ਅੱਖਰ *shorthead;

	*revisions = xstrfmt("%s...%s", oid_ਮੰਜ਼ਿਲ_hex(&ਅਧਾਰ_rev->object.oid),
			     oid_ਮੰਜ਼ਿਲ_hex(orig_ਸਿਰ));

	shorthead = repo_ਲੱਭੋ_unique_abbrev(the_repository, orig_ਸਿਰ,
					    DEFAULT_ABBREV);

	ਜੇ (upstream) {
		ਅਚਲ ਅੱਖਰ *shortrev;

		shortrev = repo_ਲੱਭੋ_unique_abbrev(the_repository,
						   &ਅਧਾਰ_rev->object.oid,
						   DEFAULT_ABBREV);

		*shortrevisions = xstrfmt("%s..%s", shortrev, shorthead);
	} ਨਹੀਂਤਾਂ
		*shortrevisions = xstrdup(shorthead);

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ init_basic_state(ਬਣਤਰ replay_opts *opts, ਅਚਲ ਅੱਖਰ *ਸਿਰ_name,
			    ਬਣਤਰ commit *onto,
			    ਅਚਲ ਬਣਤਰ object_id *orig_ਸਿਰ)
{
	FILE *interactive;

	ਜੇ (!is_directory(ਮਿਲਾਓ_dir()) &&
	    ਸੁਰੱਖਿਅਤ_create_dir_ਵਿੱਚ_gitdir(the_repository, ਮਿਲਾਓ_dir()))
		ਵਾਪਸ error_errno(_("could not create temporary %s"), ਮਿਲਾਓ_dir());

	refs_delete_reflog(get_main_ref_store(the_repository), "REBASE_HEAD");

	interactive = fopen(ਰਸਤਾ_interactive(), "w");
	ਜੇ (!interactive)
		ਵਾਪਸ error_errno(_("could not mark as interactive"));
	fclose(interactive);

	ਵਾਪਸ write_basic_state(opts, ਸਿਰ_name, onto, orig_ਸਿਰ);
}

ਸਥਿਰ ਪੂਰਨ do_interactive_rebase(ਬਣਤਰ rebase_options *opts, ਬਿਨਾਨਿਸ਼ਾਨੀ flags)
{
	ਪੂਰਨ ret = -1;
	ਅੱਖਰ *revisions = ਨਿਰਾ, *shortrevisions = ਨਿਰਾ;
	ਬਣਤਰ strvec make_script_ਦਲੀਲਾਂ = STRVEC_INIT;
	ਬਣਤਰ todo_list todo_list = TODO_LIST_INIT;
	ਬਣਤਰ replay_opts replay = get_replay_opts(opts);

	ਜੇ (get_revision_ranges(opts->upstream, opts->onto, &opts->orig_ਸਿਰ->object.oid,
				&revisions, &shortrevisions))
		ਜਾਓ cleanup;

	strvec_pushl(&make_script_ਦਲੀਲਾਂ, "", revisions, ਨਿਰਾ);
	ਜੇ (opts->restrict_revision)
		strvec_pushf(&make_script_ਦਲੀਲਾਂ, "^%s",
			     oid_ਮੰਜ਼ਿਲ_hex(&opts->restrict_revision->object.oid));

	ret = sequencer_make_script(the_repository, &todo_list.buf,
				    &make_script_ਦਲੀਲਾਂ, flags);
	ਜੇ (ret) {
		error(_("could not generate todo list"));
		ਜਾਓ cleanup;
	}

	ਜੇ (init_basic_state(&replay,
			     opts->ਸਿਰ_name ? opts->ਸਿਰ_name : "detached HEAD",
			     opts->onto, &opts->orig_ਸਿਰ->object.oid))
		ਜਾਓ cleanup;

	ਜੇ (!opts->upstream && opts->squash_onto)
		write_file(ਰਸਤਾ_squash_onto(), "%s\n",
			   oid_ਮੰਜ਼ਿਲ_hex(opts->squash_onto));

	discard_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ);
	ਜੇ (todo_list_parse_insn_ਬਫਰ(the_repository, &replay,
					todo_list.buf.buf, &todo_list))
		BUG("unusable todo list");

	ret = complete_action(the_repository, &replay, flags,
		shortrevisions, opts->onto_name, opts->onto,
		&opts->orig_ਸਿਰ->object.oid, &opts->exec,
		opts->autosquash, opts->update_refs, &todo_list);

cleanup:
	replay_opts_release(&replay);
	ਮੁਕਤ(revisions);
	ਮੁਕਤ(shortrevisions);
	todo_list_release(&todo_list);
	strvec_clear(&make_script_ਦਲੀਲਾਂ);

	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ ਲੜੀਵਾਰ_sequencer_rebase(ਬਣਤਰ rebase_options *opts)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ flags = 0;
	ਪੂਰਨ abbreviate_commands = 0, ret = 0;

	repo_config_get_bool(the_repository, "rebase.abbreviatecommands", &abbreviate_commands);

	flags |= opts->keep_ਖਾਲੀ_ਥਾਵਾਂ ? TODO_LIST_KEEP_ਖਾਲੀ_ਥਾਵਾਂ : 0;
	flags |= abbreviate_commands ? TODO_LIST_ABBREVIATE_CMDS : 0;
	flags |= opts->rebase_merges ? TODO_LIST_REBASE_MERGES : 0;
	flags |= opts->rebase_cousins > 0 ? TODO_LIST_REBASE_COUSINS : 0;
	flags |= opts->ਜੜ੍ਹ_with_onto ? TODO_LIST_ਜੜ੍ਹ_WITH_ONTO : 0;
	flags |= opts->reapply_cherry_picks ? TODO_LIST_REAPPLY_CHERRY_PICKS : 0;
	flags |= opts->flags & REBASE_NO_QUIET ? TODO_LIST_WARN_SKIPPED_CHERRY_PICKS : 0;

	ਚੋਣ (opts->action) {
	ਹਾਲਤ ACTION_NONE: {
		ਜੇ (!opts->onto && !opts->upstream)
			die(_("a base commit must be provided with --upstream or --onto"));

		ret = do_interactive_rebase(opts, flags);
		ਰੋਕੋ;
	}
	ਹਾਲਤ ACTION_SKIP: {
		ਬਣਤਰ string_list ਮਿਲਾਓ_rr = STRING_LIST_INIT_DUP;

		rerere_clear(the_repository, &ਮਿਲਾਓ_rr);
	}
		/* fallthrough */
	ਹਾਲਤ ACTION_CONTINUE: {
		ਬਣਤਰ replay_opts replay_opts = get_replay_opts(opts);

		ret = sequencer_continue(the_repository, &replay_opts);
		replay_opts_release(&replay_opts);
		ਰੋਕੋ;
	}
	ਹਾਲਤ ACTION_EDIT_TODO: {
		ਬਣਤਰ replay_opts replay_opts = get_replay_opts(opts);

		ret = edit_todo_file(flags, &replay_opts);
		replay_opts_release(&replay_opts);
		ਰੋਕੋ;
	}
	ਹਾਲਤ ACTION_SHOW_CURRENT_PATCH: {
		ਬਣਤਰ ਬੱਚਾ_process cmd = ਬੱਚਾ_PROCESS_INIT;

		cmd.git_cmd = 1;
		strvec_pushl(&cmd.ਦਲੀਲਾਂ, "show", "REBASE_HEAD", "--", ਨਿਰਾ);
		ret = ਲੜੀਵਾਰ_command(&cmd);

		ਰੋਕੋ;
	}
	ਮੂਲ:
		BUG("invalid command '%d'", opts->action);
	}

	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ is_ਮਿਲਾਓ(ਬਣਤਰ rebase_options *opts)
{
	ਵਾਪਸ opts->type == REBASE_ਮਿਲਾਓ;
}

ਸਥਿਰ ਖਾਲੀ imply_ਮਿਲਾਓ(ਬਣਤਰ rebase_options *opts, ਅਚਲ ਅੱਖਰ *option)
{
	ਚੋਣ (opts->type) {
	ਹਾਲਤ REBASE_APPLY:
		die(_("%s requires the merge backend"), option);
		ਰੋਕੋ;
	ਹਾਲਤ REBASE_ਮਿਲਾਓ:
		ਰੋਕੋ;
	ਮੂਲ:
		opts->type = REBASE_ਮਿਲਾਓ; /* implied */
		ਰੋਕੋ;
	}
}

/* Returns the filename prefixed by the state_dir */
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *state_dir_ਰਸਤਾ(ਅਚਲ ਅੱਖਰ *filename, ਬਣਤਰ rebase_options *opts)
{
	ਸਥਿਰ ਬਣਤਰ strbuf ਰਸਤਾ = STRBUF_INIT;
	ਸਥਿਰ ਮਾਪ_t ਅਗੇਤਰ_len;

	ਜੇ (!ਅਗੇਤਰ_len) {
		strbuf_addf(&ਰਸਤਾ, "%s/", opts->state_dir);
		ਅਗੇਤਰ_len = ਰਸਤਾ.len;
	}

	strbuf_setlen(&ਰਸਤਾ, ਅਗੇਤਰ_len);
	strbuf_addstr(&ਰਸਤਾ, filename);
	ਵਾਪਸ ਰਸਤਾ.buf;
}

/* Initialize the rebase options from the state directory. */
ਸਥਿਰ ਪੂਰਨ read_basic_state(ਬਣਤਰ rebase_options *opts)
{
	ਬਣਤਰ strbuf ਸਿਰ_name = STRBUF_INIT;
	ਬਣਤਰ strbuf buf = STRBUF_INIT;
	ਬਣਤਰ object_id oid;

	ਜੇ (!read_oneliner(&ਸਿਰ_name, state_dir_ਰਸਤਾ("head-name", opts),
			   READ_ONELINER_WARN_MISSING) ||
	    !read_oneliner(&buf, state_dir_ਰਸਤਾ("onto", opts),
			   READ_ONELINER_WARN_MISSING))
		ਵਾਪਸ -1;
	opts->ਸਿਰ_name = starts_with(ਸਿਰ_name.buf, "refs/") ?
		xstrdup(ਸਿਰ_name.buf) : ਨਿਰਾ;
	strbuf_release(&ਸਿਰ_name);
	ਜੇ (get_oid_hex(buf.buf, &oid) ||
	    !(opts->onto = lookup_commit_object(the_repository, &oid)))
		ਵਾਪਸ error(_("invalid onto: '%s'"), buf.buf);

	/*
	 * We always write to orig-head, but interactive rebase used to write to
	 * head. Fall back to reading from head to cover for the case that the
	 * user upgraded git with an ongoing interactive rebase.
	 */
	strbuf_reset(&buf);
	ਜੇ (file_exists(state_dir_ਰਸਤਾ("orig-head", opts))) {
		ਜੇ (!read_oneliner(&buf, state_dir_ਰਸਤਾ("orig-head", opts),
				   READ_ONELINER_WARN_MISSING))
			ਵਾਪਸ -1;
	} ਨਹੀਂਤਾਂ ਜੇ (!read_oneliner(&buf, state_dir_ਰਸਤਾ("head", opts),
				  READ_ONELINER_WARN_MISSING))
		ਵਾਪਸ -1;
	ਜੇ (get_oid_hex(buf.buf, &oid) ||
	    !(opts->orig_ਸਿਰ = lookup_commit_object(the_repository, &oid)))
		ਵਾਪਸ error(_("invalid orig-head: '%s'"), buf.buf);

	ਜੇ (file_exists(state_dir_ਰਸਤਾ("quiet", opts)))
		opts->flags &= ~REBASE_NO_QUIET;
	ਨਹੀਂਤਾਂ
		opts->flags |= REBASE_NO_QUIET;

	ਜੇ (file_exists(state_dir_ਰਸਤਾ("verbose", opts)))
		opts->flags |= REBASE_VERBOSE;

	ਜੇ (file_exists(state_dir_ਰਸਤਾ("signoff", opts))) {
		opts->signoff = 1;
		opts->flags |= REBASE_FORCE;
	}

	ਜੇ (file_exists(state_dir_ਰਸਤਾ("allow_rerere_autoupdate", opts))) {
		strbuf_reset(&buf);
		ਜੇ (!read_oneliner(&buf, state_dir_ਰਸਤਾ("allow_rerere_autoupdate", opts),
				   READ_ONELINER_WARN_MISSING))
			ਵਾਪਸ -1;
		ਜੇ (!strcmp(buf.buf, "--rerere-autoupdate"))
			opts->allow_rerere_autoupdate = RERERE_AUTOUPDATE;
		ਨਹੀਂਤਾਂ ਜੇ (!strcmp(buf.buf, "--no-rerere-autoupdate"))
			opts->allow_rerere_autoupdate = RERERE_NOAUTOUPDATE;
		ਨਹੀਂਤਾਂ
			warning(_("ignoring invalid allow_rerere_autoupdate: "
				  "'%s'"), buf.buf);
	}

	ਜੇ (file_exists(state_dir_ਰਸਤਾ("gpg_sign_opt", opts))) {
		strbuf_reset(&buf);
		ਜੇ (!read_oneliner(&buf, state_dir_ਰਸਤਾ("gpg_sign_opt", opts),
				   READ_ONELINER_WARN_MISSING))
			ਵਾਪਸ -1;
		ਮੁਕਤ(opts->gpg_sign_opt);
		opts->gpg_sign_opt = xstrdup(buf.buf);
	}

	strbuf_release(&buf);

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ rebase_write_basic_state(ਬਣਤਰ rebase_options *opts)
{
	write_file(state_dir_ਰਸਤਾ("head-name", opts), "%s",
		   opts->ਸਿਰ_name ? opts->ਸਿਰ_name : "detached HEAD");
	write_file(state_dir_ਰਸਤਾ("onto", opts), "%s",
		   opts->onto ? oid_ਮੰਜ਼ਿਲ_hex(&opts->onto->object.oid) : "");
	write_file(state_dir_ਰਸਤਾ("orig-head", opts), "%s",
		   oid_ਮੰਜ਼ਿਲ_hex(&opts->orig_ਸਿਰ->object.oid));
	ਜੇ (!(opts->flags & REBASE_NO_QUIET))
		write_file(state_dir_ਰਸਤਾ("quiet", opts), "%s", "");
	ਜੇ (opts->flags & REBASE_VERBOSE)
		write_file(state_dir_ਰਸਤਾ("verbose", opts), "%s", "");
	ਜੇ (opts->allow_rerere_autoupdate > 0)
		write_file(state_dir_ਰਸਤਾ("allow_rerere_autoupdate", opts),
			   "-%s-rerere-autoupdate",
			   opts->allow_rerere_autoupdate == RERERE_AUTOUPDATE ?
				"" : "-no");
	ਜੇ (opts->gpg_sign_opt)
		write_file(state_dir_ਰਸਤਾ("gpg_sign_opt", opts), "%s",
			   opts->gpg_sign_opt);
	ਜੇ (opts->signoff)
		write_file(state_dir_ਰਸਤਾ("signoff", opts), "--signoff");

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ cleanup_autostash(ਬਣਤਰ rebase_options *opts)
{
	ਪੂਰਨ ret;
	ਬਣਤਰ strbuf dir = STRBUF_INIT;
	ਅਚਲ ਅੱਖਰ *ਰਸਤਾ = state_dir_ਰਸਤਾ("autostash", opts);

	ਜੇ (!file_exists(ਰਸਤਾ))
		ਵਾਪਸ 0;
	ret = apply_autostash(ਰਸਤਾ);
	strbuf_addstr(&dir, opts->state_dir);
	ਜੇ (remove_dir_recursively(&dir, 0))
		ret = error_errno(_("could not remove '%s'"), opts->state_dir);
	strbuf_release(&dir);

	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ ਅੰਤ_rebase(ਬਣਤਰ rebase_options *opts)
{
	ਬਣਤਰ strbuf dir = STRBUF_INIT;
	ਪੂਰਨ ret = 0;

	refs_delete_ref(get_main_ref_store(the_repository), ਨਿਰਾ,
			"REBASE_HEAD", ਨਿਰਾ, REF_NO_DEREF);
	refs_delete_ref(get_main_ref_store(the_repository), ਨਿਰਾ,
			"AUTO_MERGE", ਨਿਰਾ, REF_NO_DEREF);
	apply_autostash(state_dir_ਰਸਤਾ("autostash", opts));
	/*
	 * We ignore errors in 'git maintenance run --auto', since the
	 * user should see them.
	 */
	ਲੜੀਵਾਰ_auto_maintenance(the_repository,
			!(opts->flags & (REBASE_NO_QUIET|REBASE_VERBOSE)));

	ਜੇ (opts->type == REBASE_ਮਿਲਾਓ) {
		ਬਣਤਰ replay_opts replay = REPLAY_OPTS_INIT;

		replay.action = REPLAY_INTERACTIVE_REBASE;
		ret = sequencer_remove_state(&replay);
		replay_opts_release(&replay);
	} ਨਹੀਂਤਾਂ {
		strbuf_addstr(&dir, opts->state_dir);
		ਜੇ (remove_dir_recursively(&dir, 0))
			ret = error(_("could not remove '%s'"),
				    opts->state_dir);
		strbuf_release(&dir);
	}

	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ move_ਮੰਜ਼ਿਲ_original_branch(ਬਣਤਰ rebase_options *opts)
{
	ਬਣਤਰ strbuf branch_reflog = STRBUF_INIT, ਸਿਰ_reflog = STRBUF_INIT;
	ਬਣਤਰ reset_working_ਰੁੱਖ_options ropts = { 0 };
	ਪੂਰਨ ret;

	ਜੇ (!opts->ਸਿਰ_name)
		ਵਾਪਸ 0; /* nothing to move back to */

	ਜੇ (!opts->onto)
		BUG("move_to_original_branch without onto");

	strbuf_addf(&branch_reflog, "%s (finish): %s onto %s",
		    opts->reflog_action,
		    opts->ਸਿਰ_name, oid_ਮੰਜ਼ਿਲ_hex(&opts->onto->object.oid));
	strbuf_addf(&ਸਿਰ_reflog, "%s (finish): returning to %s",
		    opts->reflog_action, opts->ਸਿਰ_name);
	ropts.branch = opts->ਸਿਰ_name;
	ropts.flags = RESET_WORKING_ਰੁੱਖ_REFS_ONLY |
		      RESET_WORKING_ਰੁੱਖ_UPDATE_ਸਿਰ;
	ropts.branch_ਸੁਨੇਹਾ = branch_reflog.buf;
	ropts.ਸਿਰ_ਸੁਨੇਹਾ = ਸਿਰ_reflog.buf;
	ret = reset_working_ਰੁੱਖ(the_repository, &ropts);

	strbuf_release(&branch_reflog);
	strbuf_release(&ਸਿਰ_reflog);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ ਲੜੀਵਾਰ_am(ਬਣਤਰ rebase_options *opts)
{
	ਬਣਤਰ ਬੱਚਾ_process am = ਬੱਚਾ_PROCESS_INIT;
	ਬਣਤਰ ਬੱਚਾ_process format_patch = ਬੱਚਾ_PROCESS_INIT;
	ਪੂਰਨ ਸਥਿਤੀ;
	ਅੱਖਰ *rebased_patches;

	am.git_cmd = 1;
	strvec_ਧੱਕੋ(&am.ਦਲੀਲਾਂ, "am");
	strvec_pushf(&am.env, GIT_REFLOG_ACTION_ENVIRONMENT "=%s (pick)",
		     opts->reflog_action);
	ਜੇ (opts->action == ACTION_CONTINUE) {
		strvec_ਧੱਕੋ(&am.ਦਲੀਲਾਂ, "--resolved");
		strvec_pushf(&am.ਦਲੀਲਾਂ, "--resolvemsg=%s", rebase_resolvemsg);
		ਜੇ (opts->gpg_sign_opt)
			strvec_ਧੱਕੋ(&am.ਦਲੀਲਾਂ, opts->gpg_sign_opt);
		ਸਥਿਤੀ = ਲੜੀਵਾਰ_command(&am);
		ਜੇ (ਸਥਿਤੀ)
			ਵਾਪਸ ਸਥਿਤੀ;

		ਵਾਪਸ move_ਮੰਜ਼ਿਲ_original_branch(opts);
	}
	ਜੇ (opts->action == ACTION_SKIP) {
		strvec_ਧੱਕੋ(&am.ਦਲੀਲਾਂ, "--skip");
		strvec_pushf(&am.ਦਲੀਲਾਂ, "--resolvemsg=%s", rebase_resolvemsg);
		ਸਥਿਤੀ = ਲੜੀਵਾਰ_command(&am);
		ਜੇ (ਸਥਿਤੀ)
			ਵਾਪਸ ਸਥਿਤੀ;

		ਵਾਪਸ move_ਮੰਜ਼ਿਲ_original_branch(opts);
	}
	ਜੇ (opts->action == ACTION_SHOW_CURRENT_PATCH) {
		strvec_ਧੱਕੋ(&am.ਦਲੀਲਾਂ, "--show-current-patch");
		ਵਾਪਸ ਲੜੀਵਾਰ_command(&am);
	}

	rebased_patches = repo_git_ਰਸਤਾ(the_repository, "rebased-patches");
	format_patch.ਬਾਹਰਲਾ = open(rebased_patches,
				O_WRONLY | O_CREAT | O_TRUNC, 0666);
	ਜੇ (format_patch.ਬਾਹਰਲਾ < 0) {
		ਸਥਿਤੀ = error_errno(_("could not open '%s' for writing"),
				     rebased_patches);
		ਮੁਕਤ(rebased_patches);
		ਬੱਚਾ_process_clear(&am);
		ਵਾਪਸ ਸਥਿਤੀ;
	}

	format_patch.git_cmd = 1;
	strvec_pushl(&format_patch.ਦਲੀਲਾਂ, "format-patch", "-k", "--stdout",
		     "--full-index", "--cherry-pick", "--right-only",
		     "--default-prefix", "--no-renames",
		     "--no-cover-letter", "--pretty=mboxrd", "--topo-order",
		     "--no-base", ਨਿਰਾ);
	ਜੇ (opts->git_format_patch_opt.len)
		strvec_ਵੰਡੋ(&format_patch.ਦਲੀਲਾਂ,
			     opts->git_format_patch_opt.buf);
	strvec_pushf(&format_patch.ਦਲੀਲਾਂ, "%s...%s",
		     oid_ਮੰਜ਼ਿਲ_hex(opts->ਜੜ੍ਹ ?
				/* this is now equivalent to !opts->upstream */
				&opts->onto->object.oid :
				&opts->upstream->object.oid),
		     oid_ਮੰਜ਼ਿਲ_hex(&opts->orig_ਸਿਰ->object.oid));
	ਜੇ (opts->restrict_revision)
		strvec_pushf(&format_patch.ਦਲੀਲਾਂ, "^%s",
			     oid_ਮੰਜ਼ਿਲ_hex(&opts->restrict_revision->object.oid));

	ਸਥਿਤੀ = ਲੜੀਵਾਰ_command(&format_patch);
	ਜੇ (ਸਥਿਤੀ) {
		ਬਣਤਰ reset_working_ਰੁੱਖ_options ropts = { 0 };
		unlink(rebased_patches);
		ਮੁਕਤ(rebased_patches);
		ਬੱਚਾ_process_clear(&am);

		ropts.oid = &opts->orig_ਸਿਰ->object.oid;
		ropts.branch = opts->ਸਿਰ_name;
		ropts.default_reflog_action = opts->reflog_action;
		ropts.flags = RESET_WORKING_ਰੁੱਖ_UPDATE_ਸਿਰ;
		reset_working_ਰੁੱਖ(the_repository, &ropts);
		error(_("\ngit encountered an error while preparing the "
			"patches to replay\n"
			"these revisions:\n"
			"\n    %s\n\n"
			"As a result, git cannot rebase them."),
		      opts->revisions);

		ਵਾਪਸ ਸਥਿਤੀ;
	}

	am.ਵਿੱਚ = open(rebased_patches, O_RDONLY);
	ਜੇ (am.ਵਿੱਚ < 0) {
		ਸਥਿਤੀ = error_errno(_("could not open '%s' for reading"),
				     rebased_patches);
		ਮੁਕਤ(rebased_patches);
		ਬੱਚਾ_process_clear(&am);
		ਵਾਪਸ ਸਥਿਤੀ;
	}

	strvec_pushv(&am.ਦਲੀਲਾਂ, opts->git_am_opts.v);
	strvec_ਧੱਕੋ(&am.ਦਲੀਲਾਂ, "--rebasing");
	strvec_pushf(&am.ਦਲੀਲਾਂ, "--resolvemsg=%s", rebase_resolvemsg);
	strvec_ਧੱਕੋ(&am.ਦਲੀਲਾਂ, "--patch-format=mboxrd");
	ਜੇ (opts->allow_rerere_autoupdate == RERERE_AUTOUPDATE)
		strvec_ਧੱਕੋ(&am.ਦਲੀਲਾਂ, "--rerere-autoupdate");
	ਨਹੀਂਤਾਂ ਜੇ (opts->allow_rerere_autoupdate == RERERE_NOAUTOUPDATE)
		strvec_ਧੱਕੋ(&am.ਦਲੀਲਾਂ, "--no-rerere-autoupdate");
	ਜੇ (opts->gpg_sign_opt)
		strvec_ਧੱਕੋ(&am.ਦਲੀਲਾਂ, opts->gpg_sign_opt);
	ਸਥਿਤੀ = ਲੜੀਵਾਰ_command(&am);
	unlink(rebased_patches);
	ਮੁਕਤ(rebased_patches);

	ਜੇ (!ਸਥਿਤੀ) {
		ਵਾਪਸ move_ਮੰਜ਼ਿਲ_original_branch(opts);
	}

	ਜੇ (is_directory(opts->state_dir))
		rebase_write_basic_state(opts);

	ਵਾਪਸ ਸਥਿਤੀ;
}

ਸਥਿਰ ਪੂਰਨ ਲੜੀਵਾਰ_specific_rebase(ਬਣਤਰ rebase_options *opts)
{
	ਪੂਰਨ ਸਥਿਤੀ;

	ਜੇ (opts->type == REBASE_ਮਿਲਾਓ) {
		/* Run sequencer-based rebase */
		ਜੇ (!(opts->flags & REBASE_INTERACTIVE_EXPLICIT))
			setenv("GIT_SEQUENCE_EDITOR", ":", 1);
		ਜੇ (opts->gpg_sign_opt) {
			/* remove the leading "-S" */
			ਅੱਖਰ *tmp = xstrdup(opts->gpg_sign_opt + 2);
			ਮੁਕਤ(opts->gpg_sign_opt);
			opts->gpg_sign_opt = tmp;
		}

		ਸਥਿਤੀ = ਲੜੀਵਾਰ_sequencer_rebase(opts);
	} ਨਹੀਂਤਾਂ ਜੇ (opts->type == REBASE_APPLY)
		ਸਥਿਤੀ = ਲੜੀਵਾਰ_am(opts);
	ਨਹੀਂਤਾਂ
		BUG("Unhandled rebase type %d", opts->type);

	ਜੇ (opts->dont_ਅੰਤ_rebase)
		; /* do nothing */
	ਨਹੀਂਤਾਂ ਜੇ (opts->type == REBASE_ਮਿਲਾਓ) {
		ਪੂਰਨ quiet = !(opts->flags & (REBASE_NO_QUIET|REBASE_VERBOSE));

		/*
		 * The sequencer cleans up after itself. Its state directory
		 * is gone once it is done, and stays while it is stopped.
		 */
		ਜੇ (ਸਥਿਤੀ == 0 && !is_directory(opts->state_dir))
			ਲੜੀਵਾਰ_auto_maintenance(the_repository, quiet);
	} ਨਹੀਂਤਾਂ ਜੇ (ਸਥਿਤੀ == 0) {
		ਜੇ (!file_exists(state_dir_ਰਸਤਾ("stopped-sha", opts)))
			ਅੰਤ_rebase(opts);
	} ਨਹੀਂਤਾਂ ਜੇ (ਸਥਿਤੀ == 2) {
		ਬਣਤਰ strbuf dir = STRBUF_INIT;

		apply_autostash(state_dir_ਰਸਤਾ("autostash", opts));
		strbuf_addstr(&dir, opts->state_dir);
		remove_dir_recursively(&dir, 0);
		strbuf_release(&dir);
		die("Nothing to do");
	}

	ਵਾਪਸ ਸਥਿਤੀ ? -1 : 0;
}

ਸਥਿਰ ਖਾਲੀ parse_rebase_merges_ਮੁੱਲ(ਬਣਤਰ rebase_options *options, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ)
{
	ਜੇ (!strcmp("no-rebase-cousins", ਮੁੱਲ))
		options->rebase_cousins = 0;
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp("rebase-cousins", ਮੁੱਲ))
		options->rebase_cousins = 1;
	ਨਹੀਂਤਾਂ
		die(_("Unknown rebase-merges mode: %s"), ਮੁੱਲ);
}

ਸਥਿਰ ਪੂਰਨ rebase_config(ਅਚਲ ਅੱਖਰ *var, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ,
			 ਅਚਲ ਬਣਤਰ config_context *ctx, ਖਾਲੀ *ਅੰਕੜੇ)
{
	ਬਣਤਰ rebase_options *opts = ਅੰਕੜੇ;

	ਜੇ (!strcmp(var, "rebase.stat")) {
		ਜੇ (git_config_bool(var, ਮੁੱਲ))
			opts->flags |= REBASE_DIFFSTAT;
		ਨਹੀਂਤਾਂ
			opts->flags &= ~REBASE_DIFFSTAT;
		ਵਾਪਸ 0;
	}

	ਜੇ (!strcmp(var, "rebase.autosquash")) {
		opts->config_autosquash = git_config_bool(var, ਮੁੱਲ);
		ਵਾਪਸ 0;
	}

	ਜੇ (!strcmp(var, "commit.gpgsign")) {
		ਮੁਕਤ(opts->gpg_sign_opt);
		opts->gpg_sign_opt = git_config_bool(var, ਮੁੱਲ) ?
			xstrdup("-S") : ਨਿਰਾ;
		ਵਾਪਸ 0;
	}

	ਜੇ (!strcmp(var, "rebase.autostash")) {
		opts->autostash = git_config_bool(var, ਮੁੱਲ);
		ਵਾਪਸ 0;
	}

	ਜੇ (!strcmp(var, "rebase.rebasemerges")) {
		opts->config_rebase_merges = git_parse_maybe_bool(ਮੁੱਲ);
		ਜੇ (opts->config_rebase_merges < 0) {
			opts->config_rebase_merges = 1;
			parse_rebase_merges_ਮੁੱਲ(opts, ਮੁੱਲ);
		} ਨਹੀਂਤਾਂ {
			opts->rebase_cousins = 0;
		}
		ਵਾਪਸ 0;
	}

	ਜੇ (!strcmp(var, "rebase.updaterefs")) {
		opts->config_update_refs = git_config_bool(var, ਮੁੱਲ);
		ਵਾਪਸ 0;
	}

	ਜੇ (!strcmp(var, "rebase.reschedulefailedexec")) {
		opts->reschedule_failed_exec = git_config_bool(var, ਮੁੱਲ);
		ਵਾਪਸ 0;
	}

	ਜੇ (!strcmp(var, "rebase.forkpoint")) {
		opts->fork_ਬਿੰਦੂ = git_config_bool(var, ਮੁੱਲ) ? -1 : 0;
		ਵਾਪਸ 0;
	}

	ਜੇ (!strcmp(var, "rebase.backend")) {
		FREE_AND_NULL(opts->default_backend);
		ਵਾਪਸ git_config_string(&opts->default_backend, var, ਮੁੱਲ);
	}

	ਵਾਪਸ git_default_config(var, ਮੁੱਲ, ctx, ਅੰਕੜੇ);
}

ਸਥਿਰ ਪੂਰਨ checkout_ਉੱਪਰ_ਮੰਜ਼ਿਲ_date(ਬਣਤਰ rebase_options *options)
{
	ਬਣਤਰ strbuf buf = STRBUF_INIT;
	ਬਣਤਰ reset_working_ਰੁੱਖ_options ropts = { 0 };
	ਪੂਰਨ ret = 0;

	strbuf_addf(&buf, "%s: checkout %s",
		    options->reflog_action, options->switch_ਮੰਜ਼ਿਲ);
	ropts.oid = &options->orig_ਸਿਰ->object.oid;
	ropts.branch = options->ਸਿਰ_name;
	ropts.flags = RESET_WORKING_ਰੁੱਖ_ਲੜੀਵਾਰ_POST_CHECKOUT_HOOK |
		      RESET_WORKING_ਰੁੱਖ_UPDATE_ਸਿਰ;
	ਜੇ (!ropts.branch)
		ropts.flags |=  RESET_WORKING_ਰੁੱਖ_DETACH;
	ropts.ਸਿਰ_ਸੁਨੇਹਾ = buf.buf;
	ਜੇ (reset_working_ਰੁੱਖ(the_repository, &ropts) < 0)
		ret = error(_("could not switch to %s"), options->switch_ਮੰਜ਼ਿਲ);
	strbuf_release(&buf);

	ਵਾਪਸ ret;
}

/*
 * Determines whether the commits in from..to are linear, i.e. contain
 * no merge commits. This function *expects* `from` to be an ancestor of
 * `to`.
 */
ਸਥਿਰ ਪੂਰਨ is_ਰੇਖਿਕ_history(ਬਣਤਰ commit *from, ਬਣਤਰ commit *ਮੰਜ਼ਿਲ)
{
	ਜਦੋਂ (ਮੰਜ਼ਿਲ && ਮੰਜ਼ਿਲ != from) {
		repo_parse_commit(the_repository, ਮੰਜ਼ਿਲ);
		ਜੇ (!ਮੰਜ਼ਿਲ->parents)
			ਵਾਪਸ 1;
		ਜੇ (ਮੰਜ਼ਿਲ->parents->ਅਗਲਾ)
			ਵਾਪਸ 0;
		ਮੰਜ਼ਿਲ = ਮੰਜ਼ਿਲ->parents->ਵਸਤੂ;
	}
	ਵਾਪਸ 1;
}

ਸਥਿਰ ਪੂਰਨ can_fast_forward(ਬਣਤਰ commit *onto, ਬਣਤਰ commit *upstream,
			    ਬਣਤਰ commit *restrict_revision,
			    ਬਣਤਰ commit *ਸਿਰ, ਬਣਤਰ object_id *branch_ਅਧਾਰ)
{
	ਬਣਤਰ commit_list *ਮਿਲਾਓ_bases = ਨਿਰਾ;
	ਪੂਰਨ res = 0;

	ਜੇ (is_null_oid(branch_ਅਧਾਰ))
		ਜਾਓ ਪੂਰਾ; /* fill_branch_base() found multiple merge bases */

	ਜੇ (!oideq(branch_ਅਧਾਰ, &onto->object.oid))
		ਜਾਓ ਪੂਰਾ;

	ਜੇ (restrict_revision && !oideq(&restrict_revision->object.oid, branch_ਅਧਾਰ))
		ਜਾਓ ਪੂਰਾ;

	ਜੇ (!upstream)
		ਜਾਓ ਪੂਰਾ;

	ਜੇ (repo_get_ਮਿਲਾਓ_bases(the_repository, upstream, ਸਿਰ, &ਮਿਲਾਓ_bases) < 0)
		ਬਾਹਰ_ਨਿਕਲੋ(128);
	ਜੇ (!ਮਿਲਾਓ_bases || ਮਿਲਾਓ_bases->ਅਗਲਾ)
		ਜਾਓ ਪੂਰਾ;

	ਜੇ (!oideq(&onto->object.oid, &ਮਿਲਾਓ_bases->ਵਸਤੂ->object.oid))
		ਜਾਓ ਪੂਰਾ;

	res = 1;

ਪੂਰਾ:
	commit_list_free(ਮਿਲਾਓ_bases);
	ਵਾਪਸ res && is_ਰੇਖਿਕ_history(onto, ਸਿਰ);
}

ਸਥਿਰ ਖਾਲੀ fill_branch_ਅਧਾਰ(ਬਣਤਰ rebase_options *options,
			    ਬਣਤਰ object_id *branch_ਅਧਾਰ)
{
	ਬਣਤਰ commit_list *ਮਿਲਾਓ_bases = ਨਿਰਾ;

	ਜੇ (repo_get_ਮਿਲਾਓ_bases(the_repository, options->onto,
				 options->orig_ਸਿਰ, &ਮਿਲਾਓ_bases) < 0)
		ਬਾਹਰ_ਨਿਕਲੋ(128);
	ਜੇ (!ਮਿਲਾਓ_bases || ਮਿਲਾਓ_bases->ਅਗਲਾ)
		oidcpy(branch_ਅਧਾਰ, null_oid(the_hash_algo));
	ਨਹੀਂਤਾਂ
		oidcpy(branch_ਅਧਾਰ, &ਮਿਲਾਓ_bases->ਵਸਤੂ->object.oid);

	commit_list_free(ਮਿਲਾਓ_bases);
}

ਸਥਿਰ ਪੂਰਨ parse_opt_am(ਅਚਲ ਬਣਤਰ option *opt, ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਬਣਤਰ rebase_options *opts = opt->ਮੁੱਲ;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	ਜੇ (opts->type != REBASE_UNSPECIFIED && opts->type != REBASE_APPLY)
	    die(_("apply options and merge options cannot be used together"));

	opts->type = REBASE_APPLY;

	ਵਾਪਸ 0;
}

/* -i followed by -m is still -i */
ਸਥਿਰ ਪੂਰਨ parse_opt_ਮਿਲਾਓ(ਅਚਲ ਬਣਤਰ option *opt, ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਬਣਤਰ rebase_options *opts = opt->ਮੁੱਲ;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	ਜੇ (opts->type != REBASE_UNSPECIFIED && opts->type != REBASE_ਮਿਲਾਓ)
	    die(_("apply options and merge options cannot be used together"));

	opts->type = REBASE_ਮਿਲਾਓ;

	ਵਾਪਸ 0;
}

/* -i followed by -r is still explicitly interactive, but -r alone is not */
ਸਥਿਰ ਪੂਰਨ parse_opt_interactive(ਅਚਲ ਬਣਤਰ option *opt, ਅਚਲ ਅੱਖਰ *arg,
				 ਪੂਰਨ unset)
{
	ਬਣਤਰ rebase_options *opts = opt->ਮੁੱਲ;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	ਜੇ (opts->type != REBASE_UNSPECIFIED && opts->type != REBASE_ਮਿਲਾਓ)
	    die(_("apply options and merge options cannot be used together"));

	opts->type = REBASE_ਮਿਲਾਓ;
	opts->flags |= REBASE_INTERACTIVE_EXPLICIT;

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਗਿਣਤੀ ਖਾਲੀ_ਥਾਵਾਂ_type parse_ਖਾਲੀ_ਥਾਵਾਂ_ਮੁੱਲ(ਅਚਲ ਅੱਖਰ *ਮੁੱਲ)
{
	ਜੇ (!strcasecmp(ਮੁੱਲ, "drop"))
		ਵਾਪਸ ਖਾਲੀ_ਥਾਵਾਂ_DROP;
	ਨਹੀਂਤਾਂ ਜੇ (!strcasecmp(ਮੁੱਲ, "keep"))
		ਵਾਪਸ ਖਾਲੀ_ਥਾਵਾਂ_KEEP;
	ਨਹੀਂਤਾਂ ਜੇ (!strcasecmp(ਮੁੱਲ, "stop"))
		ਵਾਪਸ ਖਾਲੀ_ਥਾਵਾਂ_STOP;
	ਨਹੀਂਤਾਂ ਜੇ (!strcasecmp(ਮੁੱਲ, "ask")) {
		warning(_("--empty=ask is deprecated; use '--empty=stop' instead."));
		ਵਾਪਸ ਖਾਲੀ_ਥਾਵਾਂ_STOP;
	}

	die(_("unrecognized empty type '%s'; valid values are \"drop\", \"keep\", and \"stop\"."), ਮੁੱਲ);
}

ਸਥਿਰ ਪੂਰਨ parse_opt_keep_ਖਾਲੀ_ਥਾਵਾਂ(ਅਚਲ ਬਣਤਰ option *opt, ਅਚਲ ਅੱਖਰ *arg,
				ਪੂਰਨ unset)
{
	ਬਣਤਰ rebase_options *opts = opt->ਮੁੱਲ;

	BUG_ON_OPT_ARG(arg);

	imply_ਮਿਲਾਓ(opts, unset ? "--no-keep-empty" : "--keep-empty");
	opts->keep_ਖਾਲੀ_ਥਾਵਾਂ = !unset;
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ parse_opt_ਖਾਲੀ_ਥਾਵਾਂ(ਅਚਲ ਬਣਤਰ option *opt, ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਬਣਤਰ rebase_options *options = opt->ਮੁੱਲ;
	ਗਿਣਤੀ ਖਾਲੀ_ਥਾਵਾਂ_type ਮੁੱਲ = parse_ਖਾਲੀ_ਥਾਵਾਂ_ਮੁੱਲ(arg);

	BUG_ON_OPT_NEG(unset);

	options->ਖਾਲੀ_ਥਾਵਾਂ = ਮੁੱਲ;
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ parse_opt_rebase_merges(ਅਚਲ ਬਣਤਰ option *opt, ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਬਣਤਰ rebase_options *options = opt->ਮੁੱਲ;

	options->rebase_merges = !unset;
	options->rebase_cousins = 0;

	ਜੇ (arg) {
		ਜੇ (!*arg) {
			warning(_("--rebase-merges with an empty string "
				  "argument is deprecated and will stop "
				  "working in a future version of Git. Use "
				  "--rebase-merges without an argument "
				  "instead, which does the same thing."));
			ਵਾਪਸ 0;
		}
		parse_rebase_merges_ਮੁੱਲ(options, arg);
	}

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ NORETURN error_on_missing_default_upstream(ਖਾਲੀ)
{
	ਬਣਤਰ branch *current_branch = branch_get(ਨਿਰਾ);

	ਲਿਖੋ(_("%s\n"
		 "Please specify which branch you want to rebase against.\n"
		 "See git-rebase(1) for details.\n"
		 "\n"
		 "    git rebase '<branch>'\n"
		 "\n"),
		current_branch ? _("There is no tracking information for "
			"the current branch.") :
			_("You are not currently on a branch."));

	ਜੇ (current_branch) {
		ਅਚਲ ਅੱਖਰ *remote = current_branch->remote_name;

		ਜੇ (!remote)
			remote = _("<remote>");

		ਲਿਖੋ(_("If you wish to set tracking information for this "
			 "branch you can do so with:\n"
			 "\n"
			 "    git branch --set-upstream-to=%s/<branch> %s\n"
			 "\n"),
		       remote, current_branch->name);
	}
	ਬਾਹਰ_ਨਿਕਲੋ(1);
}

ਸਥਿਰ ਪੂਰਨ check_exec_cmd(ਅਚਲ ਅੱਖਰ *cmd)
{
	ਜੇ (strchr(cmd, '\n'))
		ਵਾਪਸ error(_("exec commands cannot contain newlines"));

	/* Does the command consist purely of whitespace? */
	ਜੇ (!cmd[strspn(cmd, " \t\r\f\v")])
		ਵਾਪਸ error(_("empty exec command"));

	ਵਾਪਸ 0;
}

ਪੂਰਨ cmd_rebase(ਪੂਰਨ argc,
	       ਅਚਲ ਅੱਖਰ **argv,
	       ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
	       ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ rebase_options options = REBASE_OPTIONS_INIT;
	ਅਚਲ ਅੱਖਰ *branch_name;
	ਅਚਲ ਅੱਖਰ *strategy_opt = ਨਿਰਾ;
	ਪੂਰਨ ret, flags, ਕੁੱਲ_argc, ਵਿੱਚ_ਤਰੱਕੀ = 0;
	ਪੂਰਨ keep_ਅਧਾਰ = 0;
	ਪੂਰਨ ok_ਮੰਜ਼ਿਲ_skip_pre_rebase = 0;
	ਬਣਤਰ strbuf ਸੁਨੇਹਾ = STRBUF_INIT;
	ਬਣਤਰ strbuf revisions = STRBUF_INIT;
	ਬਣਤਰ strbuf buf = STRBUF_INIT;
	ਬਣਤਰ object_id branch_ਅਧਾਰ;
	ਪੂਰਨ ignore_whitespace = 0;
	ਅਚਲ ਅੱਖਰ *gpg_sign = ਨਿਰਾ;
	ਬਣਤਰ object_id squash_onto;
	ਅੱਖਰ *squash_onto_name = ਨਿਰਾ;
	ਅੱਖਰ *keep_ਅਧਾਰ_onto_name = ਨਿਰਾ;
	ਪੂਰਨ reschedule_failed_exec = -1;
	ਪੂਰਨ allow_preemptive_ff = 1;
	ਪੂਰਨ preserve_merges_selected = 0;
	ਬਣਤਰ reset_working_ਰੁੱਖ_options ropts = { 0 };
	ਬਣਤਰ option builtin_rebase_options[] = {
		OPT_STRING(0, "onto", &options.onto_name,
			   N_("revision"),
			   N_("rebase onto given branch instead of upstream")),
		OPT_BOOL(0, "keep-base", &keep_ਅਧਾਰ,
			 N_("use the merge-base of upstream and branch as the current base")),
		OPT_BOOL(0, "no-verify", &ok_ਮੰਜ਼ਿਲ_skip_pre_rebase,
			 N_("allow pre-rebase hook to run")),
		OPT_NEGBIT('q', "quiet", &options.flags,
			   N_("be quiet. implies --no-stat"),
			   REBASE_NO_QUIET | REBASE_VERBOSE | REBASE_DIFFSTAT),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ('v', "verbose", &options.flags,
			N_("display a diffstat of what changed upstream"),
			REBASE_NO_QUIET | REBASE_VERBOSE | REBASE_DIFFSTAT),
		{
			.type = OPTION_NEGBIT,
			.short_name = 'n',
			.long_name = "no-stat",
			.ਮੁੱਲ = &options.flags,
			.precision = ਆਕਾਰ(options.flags),
			.help = N_("do not show diffstat of what changed upstream"),
			.flags = PARSE_OPT_NOARG,
			.defval = REBASE_DIFFSTAT,
		},
		OPT_STRVEC(0, "trailer", &options.trailer_ਦਲੀਲਾਂ, N_("trailer"),
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
		OPT_PASSTHRU_ARGV('C', ਨਿਰਾ, &options.git_am_opts, N_("n"),
				  N_("passed to 'git apply'"), 0),
		OPT_BOOL(0, "ignore-whitespace", &ignore_whitespace,
			 N_("ignore changes in whitespace")),
		OPT_PASSTHRU_ARGV(0, "whitespace", &options.git_am_opts,
				  N_("action"), N_("passed to 'git apply'"), 0),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ('f', "force-rebase", &options.flags,
			N_("cherry-pick all commits, even if unchanged"),
			REBASE_FORCE),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ(0, "no-ff", &options.flags,
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
		OPT_CALLBACK_F(0, "apply", &options, ਨਿਰਾ,
			N_("use apply strategies to rebase"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			parse_opt_am),
		OPT_CALLBACK_F('m', "merge", &options, ਨਿਰਾ,
			N_("use merging strategies to rebase"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			parse_opt_ਮਿਲਾਓ),
		OPT_CALLBACK_F('i', "interactive", &options, ਨਿਰਾ,
			N_("let the user edit the list of commits to rebase"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			parse_opt_interactive),
		OPT_ਸੈੱਟ_INT_F('p', "preserve-merges", &preserve_merges_selected,
			      N_("(REMOVED) was: try to recreate merges "
				 "instead of ignoring them"),
			      1, PARSE_OPT_HIDDEN),
		OPT_RERERE_AUTOUPDATE(&options.allow_rerere_autoupdate),
		OPT_CALLBACK_F(0, "empty", &options, "(drop|keep|stop)",
			       N_("how to handle commits that become empty"),
			       PARSE_OPT_NONEG, parse_opt_ਖਾਲੀ_ਥਾਵਾਂ),
		OPT_CALLBACK_F('k', "keep-empty", &options, ਨਿਰਾ,
			N_("keep commits which start empty"),
			PARSE_OPT_NOARG | PARSE_OPT_HIDDEN,
			parse_opt_keep_ਖਾਲੀ_ਥਾਵਾਂ),
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
			.ਮੁੱਲ = &gpg_sign,
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
			   &options.allow_ਖਾਲੀ_ਥਾਵਾਂ_ਸੁਨੇਹਾ,
			   N_("allow rebasing commits with empty messages"),
			   PARSE_OPT_HIDDEN),
		OPT_CALLBACK_F('r', "rebase-merges", &options, N_("mode"),
			N_("try to rebase merges instead of skipping them"),
			PARSE_OPT_OPTARG, parse_opt_rebase_merges),
		OPT_BOOL(0, "fork-point", &options.fork_ਬਿੰਦੂ,
			 N_("use 'merge-base --fork-point' to refine upstream")),
		OPT_STRING('s', "strategy", &strategy_opt,
			   N_("strategy"), N_("use the given merge strategy")),
		OPT_STRING_LIST('X', "strategy-option", &options.strategy_opts,
				N_("option"),
				N_("pass the argument through to the merge "
				   "strategy")),
		OPT_BOOL(0, "root", &options.ਜੜ੍ਹ,
			 N_("rebase all reachable commits up to the root(s)")),
		OPT_BOOL(0, "reschedule-failed-exec",
			 &reschedule_failed_exec,
			 N_("automatically re-schedule any `exec` that fails")),
		OPT_BOOL(0, "reapply-cherry-picks", &options.reapply_cherry_picks,
			 N_("apply all changes, even those already present upstream")),
		OPT_ਸਿਰਾ(),
	};
	ਪੂਰਨ i;

	show_usage_with_options_if_asked(argc, argv,
					 builtin_rebase_usage,
					 builtin_rebase_options);

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ WITH_BREAKING_CHANGES
	warn_on_auto_comment_char = true;
#ਜੇ_ਅੰਤ /* !WITH_BREAKING_CHANGES */
	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_ਭਰਿਆ_ਸੂਚਕਾਂਕ = 0;

	repo_config(the_repository, rebase_config, &options);
	/* options.gpg_sign_opt will be either "-S" or NULL */
	gpg_sign = options.gpg_sign_opt ? "" : ਨਿਰਾ;
	FREE_AND_NULL(options.gpg_sign_opt);

	strbuf_reset(&buf);
	strbuf_addf(&buf, "%s/applying", apply_dir());
	ਜੇ(file_exists(buf.buf))
		die(_("It looks like 'git am' is in progress. Cannot rebase."));

	ਜੇ (is_directory(apply_dir())) {
		options.type = REBASE_APPLY;
		options.state_dir = apply_dir();
	} ਨਹੀਂਤਾਂ ਜੇ (is_directory(ਮਿਲਾਓ_dir())) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "%s/rewritten", ਮਿਲਾਓ_dir());
		ਜੇ (!(options.action == ACTION_ABORT) && is_directory(buf.buf)) {
			die(_("`rebase --preserve-merges` (-p) is no longer supported.\n"
			"Use `git rebase --abort` to terminate current rebase.\n"
			"Or downgrade to v2.33, or earlier, to complete the rebase."));
		} ਨਹੀਂਤਾਂ {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "%s/interactive", ਮਿਲਾਓ_dir());
			options.type = REBASE_ਮਿਲਾਓ;
			ਜੇ (file_exists(buf.buf))
				options.flags |= REBASE_INTERACTIVE_EXPLICIT;
		}
		options.state_dir = ਮਿਲਾਓ_dir();
	}

	ਜੇ (options.type != REBASE_UNSPECIFIED)
		ਵਿੱਚ_ਤਰੱਕੀ = 1;

	ਕੁੱਲ_argc = argc;
	argc = parse_options(argc, argv, ਅਗੇਤਰ,
			     builtin_rebase_options,
			     builtin_rebase_usage, 0);

	ਜੇ (options.trailer_ਦਲੀਲਾਂ.nr) {
		ਜੇ (validate_trailer_ਦਲੀਲਾਂ(&options.trailer_ਦਲੀਲਾਂ))
			die(ਨਿਰਾ);
		options.flags |= REBASE_FORCE;
	}

	ਜੇ (preserve_merges_selected)
		die(_("--preserve-merges was replaced by --rebase-merges\n"
			"Note: Your `pull.rebase` configuration may also be set to 'preserve',\n"
			"which is no longer supported; use 'merges' instead"));

	ਜੇ (options.action != ACTION_NONE && ਕੁੱਲ_argc != 2) {
		usage_with_options(builtin_rebase_usage,
				   builtin_rebase_options);
	}

	ਜੇ (argc > 2)
		usage_with_options(builtin_rebase_usage,
				   builtin_rebase_options);

	ਜੇ (keep_ਅਧਾਰ) {
		ਜੇ (options.onto_name)
			die(_("options '%s' and '%s' cannot be used together"), "--keep-base", "--onto");
		ਜੇ (options.ਜੜ੍ਹ)
			die(_("options '%s' and '%s' cannot be used together"), "--keep-base", "--root");
		/*
		 * --keep-base defaults to --no-fork-point to keep the
		 * base the same.
		 */
		ਜੇ (options.fork_ਬਿੰਦੂ < 0)
			options.fork_ਬਿੰਦੂ = 0;
	}
	ਜੇ (options.ਜੜ੍ਹ && options.fork_ਬਿੰਦੂ > 0)
		die(_("options '%s' and '%s' cannot be used together"), "--root", "--fork-point");

	ਜੇ (options.action != ACTION_NONE && !ਵਿੱਚ_ਤਰੱਕੀ)
		die(_("no rebase in progress"));

	ਜੇ (options.action == ACTION_EDIT_TODO && !is_ਮਿਲਾਓ(&options))
		die(_("The --edit-todo action can only be used during "
		      "interactive rebase."));

	ਜੇ (trace2_is_enabled()) {
		ਜੇ (is_ਮਿਲਾਓ(&options))
			trace2_cmd_ਬਹੁਲਕ("interactive");
		ਨਹੀਂਤਾਂ ਜੇ (options.exec.nr)
			trace2_cmd_ਬਹੁਲਕ("interactive-exec");
		ਨਹੀਂਤਾਂ
			trace2_cmd_ਬਹੁਲਕ(action_ਨਾਂ[options.action]);
	}

	options.reflog_action = getenv(GIT_REFLOG_ACTION_ENVIRONMENT);
	options.reflog_action =
		xstrdup(options.reflog_action ? options.reflog_action : "rebase");

	ਚੋਣ (options.action) {
	ਹਾਲਤ ACTION_CONTINUE: {
		ਬਣਤਰ object_id ਸਿਰ;
		ਬਣਤਰ ਤਾਲਾ_file ਤਾਲਾ_file = ਤਾਲਾ_INIT;
		ਪੂਰਨ fd;

		/* Sanity check */
		ਜੇ (repo_get_oid(the_repository, "HEAD", &ਸਿਰ))
			die(_("Cannot read HEAD"));

		fd = repo_hold_locked_ਸੂਚਕਾਂਕ(the_repository, &ਤਾਲਾ_file, 0);
		ਜੇ (repo_read_ਸੂਚਕਾਂਕ(the_repository) < 0)
			die(_("could not read index"));
		refresh_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ, REFRESH_QUIET, ਨਿਰਾ, ਨਿਰਾ,
			      ਨਿਰਾ);
		ਜੇ (0 <= fd)
			repo_update_ਸੂਚਕਾਂਕ_if_able(the_repository, &ਤਾਲਾ_file);
		rollback_ਤਾਲਾ_file(&ਤਾਲਾ_file);

		ਜੇ (has_unstaged_changes(the_repository, 1)) {
			puts(_("You must edit all merge conflicts and then\n"
			       "mark them as resolved using git add"));
			ਬਾਹਰ_ਨਿਕਲੋ(1);
		}
		ਜੇ (read_basic_state(&options))
			ਬਾਹਰ_ਨਿਕਲੋ(1);
		ਜਾਓ ਲੜੀਵਾਰ_rebase;
	}
	ਹਾਲਤ ACTION_SKIP: {
		ਬਣਤਰ string_list ਮਿਲਾਓ_rr = STRING_LIST_INIT_DUP;

		rerere_clear(the_repository, &ਮਿਲਾਓ_rr);
		string_list_clear(&ਮਿਲਾਓ_rr, 1);
		ropts.flags = RESET_WORKING_ਰੁੱਖ_HARD |
			      RESET_WORKING_ਰੁੱਖ_UPDATE_ਸਿਰ;
		ਜੇ (reset_working_ਰੁੱਖ(the_repository, &ropts) < 0)
			die(_("could not discard worktree changes"));
		remove_branch_state(the_repository, 0);
		ਜੇ (read_basic_state(&options))
			ਬਾਹਰ_ਨਿਕਲੋ(1);
		ਜਾਓ ਲੜੀਵਾਰ_rebase;
	}
	ਹਾਲਤ ACTION_ABORT: {
		ਬਣਤਰ string_list ਮਿਲਾਓ_rr = STRING_LIST_INIT_DUP;
		ਬਣਤਰ strbuf ਸਿਰ_ਸੁਨੇਹਾ = STRBUF_INIT;

		rerere_clear(the_repository, &ਮਿਲਾਓ_rr);
		string_list_clear(&ਮਿਲਾਓ_rr, 1);

		ਜੇ (read_basic_state(&options))
			ਬਾਹਰ_ਨਿਕਲੋ(1);

		strbuf_addf(&ਸਿਰ_ਸੁਨੇਹਾ, "%s (abort): returning to %s",
			    options.reflog_action,
			    options.ਸਿਰ_name ? options.ਸਿਰ_name
					      : oid_ਮੰਜ਼ਿਲ_hex(&options.orig_ਸਿਰ->object.oid));
		ropts.oid = &options.orig_ਸਿਰ->object.oid;
		ropts.ਸਿਰ_ਸੁਨੇਹਾ = ਸਿਰ_ਸੁਨੇਹਾ.buf;
		ropts.branch = options.ਸਿਰ_name;
		ropts.flags = RESET_WORKING_ਰੁੱਖ_HARD |
			      RESET_WORKING_ਰੁੱਖ_UPDATE_ਸਿਰ;
		ਜੇ (reset_working_ਰੁੱਖ(the_repository, &ropts) < 0)
			die(_("could not move back to %s"),
			    oid_ਮੰਜ਼ਿਲ_hex(&options.orig_ਸਿਰ->object.oid));
		strbuf_release(&ਸਿਰ_ਸੁਨੇਹਾ);
		remove_branch_state(the_repository, 0);
		ret = ਅੰਤ_rebase(&options);
		ਜਾਓ cleanup;
	}
	ਹਾਲਤ ACTION_QUIT: {
		save_autostash(state_dir_ਰਸਤਾ("autostash", &options));
		ਜੇ (options.type == REBASE_ਮਿਲਾਓ) {
			ਬਣਤਰ replay_opts replay = REPLAY_OPTS_INIT;

			replay.action = REPLAY_INTERACTIVE_REBASE;
			ret = sequencer_remove_state(&replay);
			replay_opts_release(&replay);
		} ਨਹੀਂਤਾਂ {
			strbuf_reset(&buf);
			strbuf_addstr(&buf, options.state_dir);
			ret = remove_dir_recursively(&buf, 0);
			ਜੇ (ret)
				error(_("could not remove '%s'"),
				       options.state_dir);
		}
		ਜਾਓ cleanup;
	}
	ਹਾਲਤ ACTION_EDIT_TODO:
		options.dont_ਅੰਤ_rebase = 1;
		ਜਾਓ ਲੜੀਵਾਰ_rebase;
	ਹਾਲਤ ACTION_SHOW_CURRENT_PATCH:
		options.dont_ਅੰਤ_rebase = 1;
		ਜਾਓ ਲੜੀਵਾਰ_rebase;
	ਹਾਲਤ ACTION_NONE:
		ਰੋਕੋ;
	ਮੂਲ:
		BUG("action: %d", options.action);
	}

	/* Make sure no rebase is in progress */
	ਜੇ (ਵਿੱਚ_ਤਰੱਕੀ) {
		ਅਚਲ ਅੱਖਰ *ਆਖਰੀ_slash = strrchr(options.state_dir, '/');
		ਅਚਲ ਅੱਖਰ *state_dir_ਅਧਾਰ =
			ਆਖਰੀ_slash ? ਆਖਰੀ_slash + 1 : options.state_dir;
		ਅਚਲ ਅੱਖਰ *cmd_ਜੀਵਤ_rebase =
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
		    state_dir_ਅਧਾਰ, cmd_ਜੀਵਤ_rebase, buf.buf);
	}

	ਜੇ ((options.flags & REBASE_INTERACTIVE_EXPLICIT) ||
	    (options.action != ACTION_NONE) ||
	    (options.exec.nr > 0) ||
	    options.autosquash == 1) {
		allow_preemptive_ff = 0;
	}
	ਜੇ (options.committer_date_is_author_date || options.ignore_date)
		options.flags |= REBASE_FORCE;

	ਲਈ (i = 0; i < options.git_am_opts.nr; i++) {
		ਅਚਲ ਅੱਖਰ *option = options.git_am_opts.v[i], *p;
		ਜੇ (!strcmp(option, "--whitespace=fix") ||
		    !strcmp(option, "--whitespace=strip"))
			allow_preemptive_ff = 0;
		ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(option, "-C", &p)) {
			ਜਦੋਂ (*p)
				ਜੇ (!ਅੰਕ_ਹੈ(*(p++)))
					die(_("switch `C' expects a "
					      "numerical value"));
		} ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(option, "--whitespace=", &p)) {
			ਜੇ (*p && strcmp(p, "warn") && strcmp(p, "nowarn") &&
			    strcmp(p, "error") && strcmp(p, "error-all"))
				die("Invalid whitespace option: '%s'", p);
		}
	}

	ਲਈ (i = 0; i < options.exec.nr; i++)
		ਜੇ (check_exec_cmd(options.exec.ਚੀਜ਼ਾਂ[i].string))
			ਬਾਹਰ_ਨਿਕਲੋ(1);

	ਜੇ (!(options.flags & REBASE_NO_QUIET))
		strvec_ਧੱਕੋ(&options.git_am_opts, "-q");

	ਜੇ (options.ਖਾਲੀ_ਥਾਵਾਂ != ਖਾਲੀ_ਥਾਵਾਂ_UNSPECIFIED)
		imply_ਮਿਲਾਓ(&options, "--empty");

	ਜੇ (options.reapply_cherry_picks < 0)
		/*
		 * We default to --no-reapply-cherry-picks unless
		 * --keep-base is given; when --keep-base is given, we want
		 * to default to --reapply-cherry-picks.
		 */
		options.reapply_cherry_picks = keep_ਅਧਾਰ;
	ਨਹੀਂਤਾਂ ਜੇ (!keep_ਅਧਾਰ)
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
		imply_ਮਿਲਾਓ(&options, options.reapply_cherry_picks ?
					  "--reapply-cherry-picks" :
					  "--no-reapply-cherry-picks");

	ਜੇ (gpg_sign)
		options.gpg_sign_opt = xstrfmt("-S%s", gpg_sign);

	ਜੇ (options.exec.nr)
		imply_ਮਿਲਾਓ(&options, "--exec");

	ਜੇ (options.type == REBASE_APPLY) {
		ਜੇ (ignore_whitespace)
			strvec_ਧੱਕੋ(&options.git_am_opts,
				    "--ignore-whitespace");
		ਜੇ (options.committer_date_is_author_date)
			strvec_ਧੱਕੋ(&options.git_am_opts,
				    "--committer-date-is-author-date");
		ਜੇ (options.ignore_date)
			strvec_ਧੱਕੋ(&options.git_am_opts, "--ignore-date");
	} ਨਹੀਂਤਾਂ {
		/* REBASE_MERGE */
		ਜੇ (ignore_whitespace) {
			string_list_append(&options.strategy_opts,
					   "ignore-space-change");
		}
	}

	ਜੇ (strategy_opt)
		options.strategy = xstrdup(strategy_opt);
	ਨਹੀਂਤਾਂ ਜੇ (options.strategy_opts.nr && !options.strategy)
		options.strategy = xstrdup("ort");
	ਜੇ (options.strategy)
		imply_ਮਿਲਾਓ(&options, "--strategy");

	ਜੇ (options.ਜੜ੍ਹ && !options.onto_name)
		imply_ਮਿਲਾਓ(&options, "--root without --onto");

	ਜੇ (options.trailer_ਦਲੀਲਾਂ.nr)
		imply_ਮਿਲਾਓ(&options, "--trailer");

	ਜੇ (isatty(2) && options.flags & REBASE_NO_QUIET)
		strbuf_addstr(&options.git_format_patch_opt, " --progress");

	ਜੇ (options.git_am_opts.nr || options.type == REBASE_APPLY) {
		/* all am options except -q are compatible only with --apply */
		ਲਈ (i = options.git_am_opts.nr - 1; i >= 0; i--)
			ਜੇ (strcmp(options.git_am_opts.v[i], "-q"))
				ਰੋਕੋ;

		ਜੇ (i >= 0 || options.type == REBASE_APPLY) {
			ਜੇ (is_ਮਿਲਾਓ(&options))
				die(_("apply options and merge options "
					  "cannot be used together"));
			ਨਹੀਂਤਾਂ ਜੇ (options.rebase_merges == -1 && options.config_rebase_merges == 1)
				die(_("apply options are incompatible with rebase.rebaseMerges.  Consider adding --no-rebase-merges"));
			ਨਹੀਂਤਾਂ ਜੇ (options.update_refs == -1 && options.config_update_refs == 1)
				die(_("apply options are incompatible with rebase.updateRefs.  Consider adding --no-update-refs"));
			ਨਹੀਂਤਾਂ
				options.type = REBASE_APPLY;
		}
	}

	ਜੇ (options.update_refs == 1)
		imply_ਮਿਲਾਓ(&options, "--update-refs");
	options.update_refs = (options.update_refs >= 0) ? options.update_refs :
			     ((options.config_update_refs >= 0) ? options.config_update_refs : 0);

	ਜੇ (options.rebase_merges == 1)
		imply_ਮਿਲਾਓ(&options, "--rebase-merges");
	options.rebase_merges = (options.rebase_merges >= 0) ? options.rebase_merges :
				((options.config_rebase_merges >= 0) ? options.config_rebase_merges : 0);

	ਜੇ (options.autosquash == 1) {
		imply_ਮਿਲਾਓ(&options, "--autosquash");
	} ਨਹੀਂਤਾਂ ਜੇ (options.autosquash == -1) {
		options.autosquash =
			options.config_autosquash &&
			(options.flags & REBASE_INTERACTIVE_EXPLICIT);
	}

	ਜੇ (options.type == REBASE_UNSPECIFIED) {
		ਜੇ (!strcmp(options.default_backend, "merge"))
			options.type = REBASE_ਮਿਲਾਓ;
		ਨਹੀਂਤਾਂ ਜੇ (!strcmp(options.default_backend, "apply"))
			options.type = REBASE_APPLY;
		ਨਹੀਂਤਾਂ
			die(_("Unknown rebase backend: %s"),
			    options.default_backend);
	}

	ਚੋਣ (options.type) {
	ਹਾਲਤ REBASE_ਮਿਲਾਓ:
		options.state_dir = ਮਿਲਾਓ_dir();
		ਰੋਕੋ;
	ਹਾਲਤ REBASE_APPLY:
		options.state_dir = apply_dir();
		ਰੋਕੋ;
	ਮੂਲ:
		BUG("options.type was just set above; should be unreachable.");
	}

	ਜੇ (options.ਖਾਲੀ_ਥਾਵਾਂ == ਖਾਲੀ_ਥਾਵਾਂ_UNSPECIFIED) {
		ਜੇ (options.flags & REBASE_INTERACTIVE_EXPLICIT)
			options.ਖਾਲੀ_ਥਾਵਾਂ = ਖਾਲੀ_ਥਾਵਾਂ_STOP;
		ਨਹੀਂਤਾਂ ਜੇ (options.exec.nr > 0)
			options.ਖਾਲੀ_ਥਾਵਾਂ = ਖਾਲੀ_ਥਾਵਾਂ_KEEP;
		ਨਹੀਂਤਾਂ
			options.ਖਾਲੀ_ਥਾਵਾਂ = ਖਾਲੀ_ਥਾਵਾਂ_DROP;
	}
	ਜੇ (reschedule_failed_exec > 0 && !is_ਮਿਲਾਓ(&options))
		die(_("--reschedule-failed-exec requires "
		      "--exec or --interactive"));
	ਜੇ (reschedule_failed_exec >= 0)
		options.reschedule_failed_exec = reschedule_failed_exec;

	ਜੇ (options.signoff) {
		strvec_ਧੱਕੋ(&options.git_am_opts, "--signoff");
		options.flags |= REBASE_FORCE;
	}

	ਜੇ (!options.ਜੜ੍ਹ) {
		ਜੇ (argc < 1) {
			ਬਣਤਰ branch *branch;

			branch = branch_get(ਨਿਰਾ);
			options.upstream_name = branch_get_upstream(branch,
								    ਨਿਰਾ);
			ਜੇ (!options.upstream_name)
				error_on_missing_default_upstream();
			ਜੇ (options.fork_ਬਿੰਦੂ < 0)
				options.fork_ਬਿੰਦੂ = 1;
		} ਨਹੀਂਤਾਂ {
			options.upstream_name = argv[0];
			argc--;
			argv++;
			ਜੇ (!strcmp(options.upstream_name, "-"))
				options.upstream_name = "@{-1}";
		}
		options.upstream =
			lookup_commit_reference_by_name(options.upstream_name);
		ਜੇ (!options.upstream)
			die(_("invalid upstream '%s'"), options.upstream_name);
		options.upstream_arg = options.upstream_name;
	} ਨਹੀਂਤਾਂ {
		ਜੇ (!options.onto_name) {
			ਜੇ (commit_ਰੁੱਖ("", 0, the_hash_algo->ਖਾਲੀ_ਥਾਵਾਂ_ਰੁੱਖ, ਨਿਰਾ,
					&squash_onto, ਨਿਰਾ, ਨਿਰਾ) < 0)
				die(_("Could not create new root commit"));
			options.squash_onto = &squash_onto;
			options.onto_name = squash_onto_name =
				xstrdup(oid_ਮੰਜ਼ਿਲ_hex(&squash_onto));
		} ਨਹੀਂਤਾਂ
			options.ਜੜ੍ਹ_with_onto = 1;

		options.upstream_name = ਨਿਰਾ;
		options.upstream = ਨਿਰਾ;
		ਜੇ (argc > 1)
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
	ਜੇ (argc == 1) {
		/* Is it "rebase other branchname" or "rebase other commit"? */
		ਬਣਤਰ object_id branch_oid;
		branch_name = argv[0];
		options.switch_ਮੰਜ਼ਿਲ = argv[0];

		/* Is it a local branch? */
		strbuf_reset(&buf);
		strbuf_addf(&buf, "refs/heads/%s", branch_name);
		ਜੇ (!refs_read_ref(get_main_ref_store(the_repository), buf.buf, &branch_oid)) {
			die_if_checked_ਬਾਹਰਲਾ(buf.buf, 1);
			options.ਸਿਰ_name = xstrdup(buf.buf);
			options.orig_ਸਿਰ =
				lookup_commit_object(the_repository,
						     &branch_oid);
		/* If not is it a valid ref (branch or commit)? */
		} ਨਹੀਂਤਾਂ {
			options.orig_ਸਿਰ =
				lookup_commit_reference_by_name(branch_name);
			options.ਸਿਰ_name = ਨਿਰਾ;
		}
		ਜੇ (!options.orig_ਸਿਰ)
			die(_("no such branch/commit '%s'"), branch_name);
	} ਨਹੀਂਤਾਂ ਜੇ (argc == 0) {
		/* Do not need to switch branches, we are already on it. */
		options.ਸਿਰ_name =
			xstrdup_or_null(refs_resolve_ref_unsafe(get_main_ref_store(the_repository), "HEAD", 0, ਨਿਰਾ,
								&flags));
		ਜੇ (!options.ਸਿਰ_name)
			die(_("No such ref: %s"), "HEAD");
		ਜੇ (flags & REF_ISSYMREF) {
			ਜੇ (!skip_ਅਗੇਤਰ(options.ਸਿਰ_name,
					 "refs/heads/", &branch_name))
				branch_name = options.ਸਿਰ_name;

		} ਨਹੀਂਤਾਂ {
			FREE_AND_NULL(options.ਸਿਰ_name);
			branch_name = "HEAD";
		}
		options.orig_ਸਿਰ = lookup_commit_reference_by_name("HEAD");
		ਜੇ (!options.orig_ਸਿਰ)
			die(_("Could not resolve HEAD to a commit"));
	} ਨਹੀਂਤਾਂ
		BUG("unexpected number of arguments left to parse");

	/* Make sure the branch to rebase onto is valid. */
	ਜੇ (keep_ਅਧਾਰ) {
		strbuf_reset(&buf);
		strbuf_addstr(&buf, options.upstream_name);
		strbuf_addstr(&buf, "...");
		strbuf_addstr(&buf, branch_name);
		options.onto_name = keep_ਅਧਾਰ_onto_name = xstrdup(buf.buf);
	} ਨਹੀਂਤਾਂ ਜੇ (!options.onto_name)
		options.onto_name = options.upstream_name;
	ਜੇ (ਲੜੀ_ਖੋਜ(options.onto_name, "...")) {
		ਜੇ (repo_get_oid_mb(the_repository, options.onto_name, &branch_ਅਧਾਰ) < 0) {
			ਜੇ (keep_ਅਧਾਰ)
				die(_("'%s': need exactly one merge base with branch"),
				    options.upstream_name);
			ਨਹੀਂਤਾਂ
				die(_("'%s': need exactly one merge base"),
				    options.onto_name);
		}
		options.onto = lookup_commit_or_die(&branch_ਅਧਾਰ,
						    options.onto_name);
	} ਨਹੀਂਤਾਂ {
		options.onto =
			lookup_commit_reference_by_name(options.onto_name);
		ਜੇ (!options.onto)
			die(_("Does not point to a valid commit '%s'"),
				options.onto_name);
		fill_branch_ਅਧਾਰ(&options, &branch_ਅਧਾਰ);
	}

	ਜੇ (keep_ਅਧਾਰ && options.reapply_cherry_picks)
		options.upstream = options.onto;

	ਜੇ (options.fork_ਬਿੰਦੂ > 0)
		options.restrict_revision =
			get_fork_ਬਿੰਦੂ(options.upstream_name, options.orig_ਸਿਰ);

	ਜੇ (repo_read_ਸੂਚਕਾਂਕ(the_repository) < 0)
		die(_("could not read index"));

	ਜੇ (options.autostash)
		create_autostash(the_repository,
				 state_dir_ਰਸਤਾ("autostash", &options));


	ਜੇ (require_clean_work_ਰੁੱਖ(the_repository, "rebase",
				    _("Please commit or stash them."), 1, 1)) {
		ret = -1;
		ਜਾਓ cleanup_autostash;
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
	ਜੇ (allow_preemptive_ff &&
	    can_fast_forward(options.onto, options.upstream, options.restrict_revision,
			     options.orig_ਸਿਰ, &branch_ਅਧਾਰ)) {
		ਪੂਰਨ flag;

		ਜੇ (!(options.flags & REBASE_FORCE)) {
			/* Lazily switch to the target branch if needed... */
			ਜੇ (options.switch_ਮੰਜ਼ਿਲ) {
				ret = checkout_ਉੱਪਰ_ਮੰਜ਼ਿਲ_date(&options);
				ਜੇ (ret)
					ਜਾਓ cleanup_autostash;
			}

			ਜੇ (!(options.flags & REBASE_NO_QUIET))
				; /* be quiet */
			ਨਹੀਂਤਾਂ ਜੇ (!strcmp(branch_name, "HEAD") &&
				 refs_resolve_ref_unsafe(get_main_ref_store(the_repository), "HEAD", 0, ਨਿਰਾ, &flag))
				puts(_("HEAD is up to date."));
			ਨਹੀਂਤਾਂ
				ਲਿਖੋ(_("Current branch %s is up to date.\n"),
				       branch_name);
			ret = ਅੰਤ_rebase(&options);
			ਜਾਓ cleanup;
		} ਨਹੀਂਤਾਂ ਜੇ (!(options.flags & REBASE_NO_QUIET))
			; /* be quiet */
		ਨਹੀਂਤਾਂ ਜੇ (!strcmp(branch_name, "HEAD") &&
			 refs_resolve_ref_unsafe(get_main_ref_store(the_repository), "HEAD", 0, ਨਿਰਾ, &flag))
			puts(_("HEAD is up to date, rebase forced."));
		ਨਹੀਂਤਾਂ
			ਲਿਖੋ(_("Current branch %s is up to date, rebase "
				 "forced.\n"), branch_name);
	}

	/* If a hook exists, give it a chance to interrupt*/
	ਜੇ (!ok_ਮੰਜ਼ਿਲ_skip_pre_rebase &&
	    ਲੜੀਵਾਰ_hooks_l(the_repository, "pre-rebase", options.upstream_arg,
			argc ? argv[0] : ਨਿਰਾ, ਨਿਰਾ)) {
		ret = error(_("The pre-rebase hook refused to rebase."));
		ਜਾਓ cleanup_autostash;
	}

	ਜੇ (options.flags & REBASE_DIFFSTAT) {
		ਬਣਤਰ diff_options opts;

		ਜੇ (options.flags & REBASE_VERBOSE) {
			ਜੇ (is_null_oid(&branch_ਅਧਾਰ))
				ਲਿਖੋ(_("Changes to %s:\n"),
				       oid_ਮੰਜ਼ਿਲ_hex(&options.onto->object.oid));
			ਨਹੀਂਤਾਂ
				ਲਿਖੋ(_("Changes from %s to %s:\n"),
				       oid_ਮੰਜ਼ਿਲ_hex(&branch_ਅਧਾਰ),
				       oid_ਮੰਜ਼ਿਲ_hex(&options.onto->object.oid));
		}

		/* We want color (if set), but no pager */
		repo_diff_ਤਿਆਰੀ(the_repository, &opts);
		init_diffstat_widths(&opts);
		opts.output_format |=
			DIFF_FORMAT_SUMMARY | DIFF_FORMAT_DIFFSTAT;
		opts.detect_rename = DIFF_DETECT_RENAME;
		diff_ਤਿਆਰੀ_ਪੂਰਾ(&opts);
		diff_ਰੁੱਖ_oid(is_null_oid(&branch_ਅਧਾਰ) ?
			      the_hash_algo->ਖਾਲੀ_ਥਾਵਾਂ_ਰੁੱਖ : &branch_ਅਧਾਰ,
			      &options.onto->object.oid, "", &opts);
		diffcore_std(&opts);
		diff_flush(&opts);
	}

	ਜੇ (is_ਮਿਲਾਓ(&options))
		ਜਾਓ ਲੜੀਵਾਰ_rebase;

	/* Detach HEAD and reset the tree */
	ਜੇ (options.flags & REBASE_NO_QUIET)
		ਲਿਖੋ(_("First, rewinding head to replay your work on top of "
			 "it...\n"));

	strbuf_addf(&ਸੁਨੇਹਾ, "%s (start): checkout %s",
		    options.reflog_action, options.onto_name);
	ropts.oid = &options.onto->object.oid;
	ropts.orig_ਸਿਰ = &options.orig_ਸਿਰ->object.oid;
	ropts.flags = RESET_WORKING_ਰੁੱਖ_DETACH |
		      RESET_WORKING_ਰੁੱਖ_UPDATE_ਸਿਰ |
		      RESET_WORKING_ਰੁੱਖ_UPDATE_ORIG_ਸਿਰ |
		      RESET_WORKING_ਰੁੱਖ_ਲੜੀਵਾਰ_POST_CHECKOUT_HOOK;
	ropts.ਸਿਰ_ਸੁਨੇਹਾ = ਸੁਨੇਹਾ.buf;
	ropts.default_reflog_action = options.reflog_action;
	ਜੇ (reset_working_ਰੁੱਖ(the_repository, &ropts)) {
		ret = error(_("Could not detach HEAD"));
		ਜਾਓ cleanup_autostash;
	}

	/*
	 * If the onto is a proper descendant of the tip of the branch, then
	 * we just fast-forwarded.
	 */
	ਜੇ (oideq(&branch_ਅਧਾਰ, &options.orig_ਸਿਰ->object.oid)) {
		ਲਿਖੋ(_("Fast-forwarded %s to %s.\n"),
			branch_name, options.onto_name);
		move_ਮੰਜ਼ਿਲ_original_branch(&options);
		ret = ਅੰਤ_rebase(&options);
		ਜਾਓ cleanup;
	}

	strbuf_addf(&revisions, "%s..%s",
		    options.ਜੜ੍ਹ ? oid_ਮੰਜ਼ਿਲ_hex(&options.onto->object.oid) :
		    (options.restrict_revision ?
		     oid_ਮੰਜ਼ਿਲ_hex(&options.restrict_revision->object.oid) :
		     oid_ਮੰਜ਼ਿਲ_hex(&options.upstream->object.oid)),
		    oid_ਮੰਜ਼ਿਲ_hex(&options.orig_ਸਿਰ->object.oid));

	options.revisions = revisions.buf;

ਲੜੀਵਾਰ_rebase:
	ret = ਲੜੀਵਾਰ_specific_rebase(&options);

cleanup:
	strbuf_release(&buf);
	strbuf_release(&ਸੁਨੇਹਾ);
	strbuf_release(&revisions);
	rebase_options_release(&options);
	ਮੁਕਤ(squash_onto_name);
	ਮੁਕਤ(keep_ਅਧਾਰ_onto_name);
	ਵਾਪਸ !!ret;

cleanup_autostash:
	ret |= !!cleanup_autostash(&options);
	ਜਾਓ cleanup;
}
