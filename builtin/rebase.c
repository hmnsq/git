/*
 * "git rebase" builtin command
 *
 * Copyright (c) 2018 Pratik Karki
 */

#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE
#ವ್ಯಾಖ್ಯಾನಿಸು DISABLE_SIGN_COMPARE_WARNINGS

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

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ * ಸ್ಥಿರಾಂಕ builtin_rebase_usage[] = {
	N_("git rebase [-i] [options] [--exec <cmd>] "
		"[--onto <newbase> | --keep-base] [<upstream> [<branch>]]"),
	N_("git rebase [-i] [options] [--exec <cmd>] [--onto <newbase>] "
		"--root [<branch>]"),
	"git rebase --continue | --abort | --skip | --edit-todo",
	ಖಾಲಿ
};

ಸ್ಥಿರ GIT_ಮಾರ್ಗ_FUNC(ಮಾರ್ಗ_squash_onto, "rebase-merge/squash-onto")
ಸ್ಥಿರ GIT_ಮಾರ್ಗ_FUNC(ಮಾರ್ಗ_interactive, "rebase-merge/interactive")
ಸ್ಥಿರ GIT_ಮಾರ್ಗ_FUNC(apply_dir, "rebase-apply")
ಸ್ಥಿರ GIT_ಮಾರ್ಗ_FUNC(ವಿಲೀನ_dir, "rebase-merge")

ಎಣಿಕೆ rebase_type {
	REBASE_UNSPECIFIED = -1,
	REBASE_APPLY,
	REBASE_ವಿಲೀನ
};

ಎಣಿಕೆ ಖಾಲಿಗಳು_type {
	ಖಾಲಿಗಳು_UNSPECIFIED = -1,
	ಖಾಲಿಗಳು_DROP,
	ಖಾಲಿಗಳು_KEEP,
	ಖಾಲಿಗಳು_STOP
};

ಎಣಿಕೆ action {
	ACTION_NONE = 0,
	ACTION_CONTINUE,
	ACTION_SKIP,
	ACTION_ABORT,
	ACTION_QUIT,
	ACTION_EDIT_TODO,
	ACTION_SHOW_CURRENT_PATCH
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *action_ಹೆಸರುಗಳು[] = {
	"undefined",
	"continue",
	"skip",
	"abort",
	"quit",
	"edit_todo",
	"show_current_patch"
};

ರಚನೆ rebase_options {
	ಎಣಿಕೆ rebase_type type;
	ಎಣಿಕೆ ಖಾಲಿಗಳು_type ಖಾಲಿಗಳು;
	ಅಕ್ಷರ *default_backend;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state_dir;
	ರಚನೆ commit *upstream;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *upstream_name;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *upstream_arg;
	ಅಕ್ಷರ *ತಲೆ_name;
	ರಚನೆ commit *orig_ತಲೆ;
	ರಚನೆ commit *onto;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *onto_name;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *revisions;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *switch_ಗಮ್ಯ;
	ಪೂರ್ಣಾಂಕ ಬೇರು, ಬೇರು_with_onto;
	ರಚನೆ object_id *squash_onto;
	ರಚನೆ commit *restrict_revision;
	ಪೂರ್ಣಾಂಕ dont_ಮುಕ್ತಾಯ_rebase;
	ಎಣಿಕೆ {
		REBASE_NO_QUIET = 1<<0,
		REBASE_VERBOSE = 1<<1,
		REBASE_DIFFSTAT = 1<<2,
		REBASE_FORCE = 1<<3,
		REBASE_INTERACTIVE_EXPLICIT = 1<<4,
	} flags;
	ರಚನೆ strvec git_am_opts;
	ಎಣಿಕೆ action action;
	ಅಕ್ಷರ *reflog_action;
	ಪೂರ್ಣಾಂಕ signoff;
	ರಚನೆ strvec trailer_ವಾದಗಳು;
	ಪೂರ್ಣಾಂಕ allow_rerere_autoupdate;
	ಪೂರ್ಣಾಂಕ keep_ಖಾಲಿಗಳು;
	ಪೂರ್ಣಾಂಕ autosquash;
	ಅಕ್ಷರ *gpg_sign_opt;
	ಪೂರ್ಣಾಂಕ autostash;
	ಪೂರ್ಣಾಂಕ committer_date_is_author_date;
	ಪೂರ್ಣಾಂಕ ignore_date;
	ರಚನೆ string_list exec;
	ಪೂರ್ಣಾಂಕ allow_ಖಾಲಿಗಳು_ಸಂದೇಶ;
	ಪೂರ್ಣಾಂಕ rebase_merges, rebase_cousins;
	ಅಕ್ಷರ *strategy;
	ರಚನೆ string_list strategy_opts;
	ರಚನೆ strbuf git_format_patch_opt;
	ಪೂರ್ಣಾಂಕ reschedule_failed_exec;
	ಪೂರ್ಣಾಂಕ reapply_cherry_picks;
	ಪೂರ್ಣಾಂಕ fork_ಬಿಂದು;
	ಪೂರ್ಣಾಂಕ update_refs;
	ಪೂರ್ಣಾಂಕ config_autosquash;
	ಪೂರ್ಣಾಂಕ config_rebase_merges;
	ಪೂರ್ಣಾಂಕ config_update_refs;
};

#ವ್ಯಾಖ್ಯಾನಿಸು REBASE_OPTIONS_INIT {			  	\
		.type = REBASE_UNSPECIFIED,	  	\
		.ಖಾಲಿಗಳು = ಖಾಲಿಗಳು_UNSPECIFIED,	  	\
		.keep_ಖಾಲಿಗಳು = 1,			\
		.default_backend = xstrdup("merge"),  	\
		.flags = REBASE_NO_QUIET, 		\
		.git_am_opts = STRVEC_INIT,		\
		.exec = STRING_LIST_INIT_NODUP,		\
		.trailer_ವಾದಗಳು = STRVEC_INIT,		\
		.git_format_patch_opt = STRBUF_INIT,	\
		.fork_ಬಿಂದು = -1,			\
		.reapply_cherry_picks = -1,             \
		.allow_ಖಾಲಿಗಳು_ಸಂದೇಶ = 1,               \
		.autosquash = -1,                       \
		.rebase_merges = -1,                    \
		.config_rebase_merges = -1,             \
		.update_refs = -1,                      \
		.config_update_refs = -1,               \
		.strategy_opts = STRING_LIST_INIT_NODUP,\
	}

ಸ್ಥಿರ ಶೂನ್ಯ rebase_options_release(ರಚನೆ rebase_options *opts)
{
	ಬಿಡುಗಡೆ(opts->default_backend);
	ಬಿಡುಗಡೆ(opts->reflog_action);
	ಬಿಡುಗಡೆ(opts->ತಲೆ_name);
	strvec_clear(&opts->git_am_opts);
	ಬಿಡುಗಡೆ(opts->gpg_sign_opt);
	string_list_clear(&opts->exec, 0);
	ಬಿಡುಗಡೆ(opts->strategy);
	string_list_clear(&opts->strategy_opts, 0);
	strbuf_release(&opts->git_format_patch_opt);
	strvec_clear(&opts->trailer_ವಾದಗಳು);
}

ಸ್ಥಿರ ರಚನೆ replay_opts get_replay_opts(ಸ್ಥಿರಾಂಕ ರಚನೆ rebase_options *opts)
{
	ರಚನೆ replay_opts replay = REPLAY_OPTS_INIT;

	replay.action = REPLAY_INTERACTIVE_REBASE;
	replay.strategy = ಖಾಲಿ;
	sequencer_init_config(&replay);

	replay.signoff = opts->signoff;

	strvec_pushv(&replay.trailer_ವಾದಗಳು, opts->trailer_ವಾದಗಳು.v);

	replay.allow_ff = !(opts->flags & REBASE_FORCE);
	ಆದರೆ (opts->allow_rerere_autoupdate)
		replay.allow_rerere_auto = opts->allow_rerere_autoupdate;
	replay.allow_ಖಾಲಿಗಳು = 1;
	replay.allow_ಖಾಲಿಗಳು_ಸಂದೇಶ = opts->allow_ಖಾಲಿಗಳು_ಸಂದೇಶ;
	replay.drop_redundant_commits = (opts->ಖಾಲಿಗಳು == ಖಾಲಿಗಳು_DROP);
	replay.keep_redundant_commits = (opts->ಖಾಲಿಗಳು == ಖಾಲಿಗಳು_KEEP);
	replay.quiet = !(opts->flags & REBASE_NO_QUIET);
	replay.verbose = opts->flags & REBASE_VERBOSE;
	replay.reschedule_failed_exec = opts->reschedule_failed_exec;
	replay.committer_date_is_author_date =
					opts->committer_date_is_author_date;
	replay.ignore_date = opts->ignore_date;
	ಬಿಡುಗಡೆ(replay.gpg_sign);
	replay.gpg_sign = xstrdup_or_null(opts->gpg_sign_opt);
	replay.reflog_action = xstrdup(opts->reflog_action);
	ಆದರೆ (opts->strategy)
		replay.strategy = xstrdup_or_null(opts->strategy);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!replay.strategy && replay.default_strategy) {
		replay.strategy = replay.default_strategy;
		replay.default_strategy = ಖಾಲಿ;
	}

	ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < opts->strategy_opts.nr; i++)
		strvec_ತಳ್ಳು(&replay.xopts, opts->strategy_opts.ವಸ್ತುಗಳು[i].string);

	ಆದರೆ (opts->squash_onto) {
		oidcpy(&replay.squash_onto, opts->squash_onto);
		replay.have_squash_onto = 1;
	}

	ಹಿಂತಿರುಗಿ replay;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ edit_todo_file(ಚಿಹ್ನೆರಹಿತ flags, ರಚನೆ replay_opts *opts)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *todo_file = rebase_ಮಾರ್ಗ_todo();
	ರಚನೆ todo_list todo_list = TODO_LIST_INIT,
		new_todo = TODO_LIST_INIT;
	ಪೂರ್ಣಾಂಕ res = 0;

	ಆದರೆ (strbuf_read_file(&todo_list.buf, todo_file, 0) < 0)
		ಹಿಂತಿರುಗಿ error_errno(_("could not read '%s'."), todo_file);

	strbuf_stripspace(&todo_list.buf, comment_ಸಾಲು_str);
	res = edit_todo_list(the_repository, opts, &todo_list, &new_todo,
			     ಖಾಲಿ, ಖಾಲಿ, flags);
	ಆದರೆ (!res && todo_list_write_ಗಮ್ಯ_file(the_repository, &new_todo, todo_file,
					    ಖಾಲಿ, ಖಾಲಿ, -1, flags & ~(TODO_LIST_SHORTEN_IDS)))
		res = error_errno(_("could not write '%s'"), todo_file);

	todo_list_release(&todo_list);
	todo_list_release(&new_todo);

	ಹಿಂತಿರುಗಿ res;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ get_revision_ranges(ರಚನೆ commit *upstream, ರಚನೆ commit *onto,
			       ರಚನೆ object_id *orig_ತಲೆ, ಅಕ್ಷರ **revisions,
			       ಅಕ್ಷರ **shortrevisions)
{
	ರಚನೆ commit *ತಳ_rev = upstream ? upstream : onto;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *shorthead;

	*revisions = xstrfmt("%s...%s", oid_ಗಮ್ಯ_hex(&ತಳ_rev->object.oid),
			     oid_ಗಮ್ಯ_hex(orig_ತಲೆ));

	shorthead = repo_ಕಂಡುಹಿಡಿ_unique_abbrev(the_repository, orig_ತಲೆ,
					    DEFAULT_ABBREV);

	ಆದರೆ (upstream) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *shortrev;

		shortrev = repo_ಕಂಡುಹಿಡಿ_unique_abbrev(the_repository,
						   &ತಳ_rev->object.oid,
						   DEFAULT_ABBREV);

		*shortrevisions = xstrfmt("%s..%s", shortrev, shorthead);
	} ಇಲ್ಲದಿದ್ದರೆ
		*shortrevisions = xstrdup(shorthead);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ init_basic_state(ರಚನೆ replay_opts *opts, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ತಲೆ_name,
			    ರಚನೆ commit *onto,
			    ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *orig_ತಲೆ)
{
	FILE *interactive;

	ಆದರೆ (!is_directory(ವಿಲೀನ_dir()) &&
	    ಸುರಕ್ಷಿತ_create_dir_ಒಳ_gitdir(the_repository, ವಿಲೀನ_dir()))
		ಹಿಂತಿರುಗಿ error_errno(_("could not create temporary %s"), ವಿಲೀನ_dir());

	refs_delete_reflog(get_main_ref_store(the_repository), "REBASE_HEAD");

	interactive = fopen(ಮಾರ್ಗ_interactive(), "w");
	ಆದರೆ (!interactive)
		ಹಿಂತಿರುಗಿ error_errno(_("could not mark as interactive"));
	fclose(interactive);

	ಹಿಂತಿರುಗಿ write_basic_state(opts, ತಲೆ_name, onto, orig_ತಲೆ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ do_interactive_rebase(ರಚನೆ rebase_options *opts, ಚಿಹ್ನೆರಹಿತ flags)
{
	ಪೂರ್ಣಾಂಕ ret = -1;
	ಅಕ್ಷರ *revisions = ಖಾಲಿ, *shortrevisions = ಖಾಲಿ;
	ರಚನೆ strvec make_script_ವಾದಗಳು = STRVEC_INIT;
	ರಚನೆ todo_list todo_list = TODO_LIST_INIT;
	ರಚನೆ replay_opts replay = get_replay_opts(opts);

	ಆದರೆ (get_revision_ranges(opts->upstream, opts->onto, &opts->orig_ತಲೆ->object.oid,
				&revisions, &shortrevisions))
		ಹೋಗು cleanup;

	strvec_pushl(&make_script_ವಾದಗಳು, "", revisions, ಖಾಲಿ);
	ಆದರೆ (opts->restrict_revision)
		strvec_pushf(&make_script_ವಾದಗಳು, "^%s",
			     oid_ಗಮ್ಯ_hex(&opts->restrict_revision->object.oid));

	ret = sequencer_make_script(the_repository, &todo_list.buf,
				    &make_script_ವಾದಗಳು, flags);
	ಆದರೆ (ret) {
		error(_("could not generate todo list"));
		ಹೋಗು cleanup;
	}

	ಆದರೆ (init_basic_state(&replay,
			     opts->ತಲೆ_name ? opts->ತಲೆ_name : "detached HEAD",
			     opts->onto, &opts->orig_ತಲೆ->object.oid))
		ಹೋಗು cleanup;

	ಆದರೆ (!opts->upstream && opts->squash_onto)
		write_file(ಮಾರ್ಗ_squash_onto(), "%s\n",
			   oid_ಗಮ್ಯ_hex(opts->squash_onto));

	discard_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ);
	ಆದರೆ (todo_list_parse_insn_ಬಫರ್(the_repository, &replay,
					todo_list.buf.buf, &todo_list))
		BUG("unusable todo list");

	ret = complete_action(the_repository, &replay, flags,
		shortrevisions, opts->onto_name, opts->onto,
		&opts->orig_ತಲೆ->object.oid, &opts->exec,
		opts->autosquash, opts->update_refs, &todo_list);

cleanup:
	replay_opts_release(&replay);
	ಬಿಡುಗಡೆ(revisions);
	ಬಿಡುಗಡೆ(shortrevisions);
	todo_list_release(&todo_list);
	strvec_clear(&make_script_ವಾದಗಳು);

	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಸತತ_sequencer_rebase(ರಚನೆ rebase_options *opts)
{
	ಚಿಹ್ನೆರಹಿತ flags = 0;
	ಪೂರ್ಣಾಂಕ abbreviate_commands = 0, ret = 0;

	repo_config_get_bool(the_repository, "rebase.abbreviatecommands", &abbreviate_commands);

	flags |= opts->keep_ಖಾಲಿಗಳು ? TODO_LIST_KEEP_ಖಾಲಿಗಳು : 0;
	flags |= abbreviate_commands ? TODO_LIST_ABBREVIATE_CMDS : 0;
	flags |= opts->rebase_merges ? TODO_LIST_REBASE_MERGES : 0;
	flags |= opts->rebase_cousins > 0 ? TODO_LIST_REBASE_COUSINS : 0;
	flags |= opts->ಬೇರು_with_onto ? TODO_LIST_ಬೇರು_WITH_ONTO : 0;
	flags |= opts->reapply_cherry_picks ? TODO_LIST_REAPPLY_CHERRY_PICKS : 0;
	flags |= opts->flags & REBASE_NO_QUIET ? TODO_LIST_WARN_SKIPPED_CHERRY_PICKS : 0;

	ಆಯ್ಕೆ (opts->action) {
	ಸ್ಥಿತಿ ACTION_NONE: {
		ಆದರೆ (!opts->onto && !opts->upstream)
			die(_("a base commit must be provided with --upstream or --onto"));

		ret = do_interactive_rebase(opts, flags);
		ನಿಲ್ಲಿಸು;
	}
	ಸ್ಥಿತಿ ACTION_SKIP: {
		ರಚನೆ string_list ವಿಲೀನ_rr = STRING_LIST_INIT_DUP;

		rerere_clear(the_repository, &ವಿಲೀನ_rr);
	}
		/* fallthrough */
	ಸ್ಥಿತಿ ACTION_CONTINUE: {
		ರಚನೆ replay_opts replay_opts = get_replay_opts(opts);

		ret = sequencer_continue(the_repository, &replay_opts);
		replay_opts_release(&replay_opts);
		ನಿಲ್ಲಿಸು;
	}
	ಸ್ಥಿತಿ ACTION_EDIT_TODO: {
		ರಚನೆ replay_opts replay_opts = get_replay_opts(opts);

		ret = edit_todo_file(flags, &replay_opts);
		replay_opts_release(&replay_opts);
		ನಿಲ್ಲಿಸು;
	}
	ಸ್ಥಿತಿ ACTION_SHOW_CURRENT_PATCH: {
		ರಚನೆ ಮಗು_process cmd = ಮಗು_PROCESS_INIT;

		cmd.git_cmd = 1;
		strvec_pushl(&cmd.ವಾದಗಳು, "show", "REBASE_HEAD", "--", ಖಾಲಿ);
		ret = ಸತತ_command(&cmd);

		ನಿಲ್ಲಿಸು;
	}
	ಪೂರ್ವನಿಯೋಜಿತ:
		BUG("invalid command '%d'", opts->action);
	}

	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ is_ವಿಲೀನ(ರಚನೆ rebase_options *opts)
{
	ಹಿಂತಿರುಗಿ opts->type == REBASE_ವಿಲೀನ;
}

ಸ್ಥಿರ ಶೂನ್ಯ imply_ವಿಲೀನ(ರಚನೆ rebase_options *opts, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *option)
{
	ಆಯ್ಕೆ (opts->type) {
	ಸ್ಥಿತಿ REBASE_APPLY:
		die(_("%s requires the merge backend"), option);
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ REBASE_ವಿಲೀನ:
		ನಿಲ್ಲಿಸು;
	ಪೂರ್ವನಿಯೋಜಿತ:
		opts->type = REBASE_ವಿಲೀನ; /* implied */
		ನಿಲ್ಲಿಸು;
	}
}

/* Returns the filename prefixed by the state_dir */
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state_dir_ಮಾರ್ಗ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *filename, ರಚನೆ rebase_options *opts)
{
	ಸ್ಥಿರ ರಚನೆ strbuf ಮಾರ್ಗ = STRBUF_INIT;
	ಸ್ಥಿರ ಗಾತ್ರ_t ಪೂರ್ವಪ್ರತ್ಯಯ_len;

	ಆದರೆ (!ಪೂರ್ವಪ್ರತ್ಯಯ_len) {
		strbuf_addf(&ಮಾರ್ಗ, "%s/", opts->state_dir);
		ಪೂರ್ವಪ್ರತ್ಯಯ_len = ಮಾರ್ಗ.len;
	}

	strbuf_setlen(&ಮಾರ್ಗ, ಪೂರ್ವಪ್ರತ್ಯಯ_len);
	strbuf_addstr(&ಮಾರ್ಗ, filename);
	ಹಿಂತಿರುಗಿ ಮಾರ್ಗ.buf;
}

/* Initialize the rebase options from the state directory. */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ read_basic_state(ರಚನೆ rebase_options *opts)
{
	ರಚನೆ strbuf ತಲೆ_name = STRBUF_INIT;
	ರಚನೆ strbuf buf = STRBUF_INIT;
	ರಚನೆ object_id oid;

	ಆದರೆ (!read_oneliner(&ತಲೆ_name, state_dir_ಮಾರ್ಗ("head-name", opts),
			   READ_ONELINER_WARN_MISSING) ||
	    !read_oneliner(&buf, state_dir_ಮಾರ್ಗ("onto", opts),
			   READ_ONELINER_WARN_MISSING))
		ಹಿಂತಿರುಗಿ -1;
	opts->ತಲೆ_name = starts_with(ತಲೆ_name.buf, "refs/") ?
		xstrdup(ತಲೆ_name.buf) : ಖಾಲಿ;
	strbuf_release(&ತಲೆ_name);
	ಆದರೆ (get_oid_hex(buf.buf, &oid) ||
	    !(opts->onto = lookup_commit_object(the_repository, &oid)))
		ಹಿಂತಿರುಗಿ error(_("invalid onto: '%s'"), buf.buf);

	/*
	 * We always write to orig-head, but interactive rebase used to write to
	 * head. Fall back to reading from head to cover for the case that the
	 * user upgraded git with an ongoing interactive rebase.
	 */
	strbuf_reset(&buf);
	ಆದರೆ (file_exists(state_dir_ಮಾರ್ಗ("orig-head", opts))) {
		ಆದರೆ (!read_oneliner(&buf, state_dir_ಮಾರ್ಗ("orig-head", opts),
				   READ_ONELINER_WARN_MISSING))
			ಹಿಂತಿರುಗಿ -1;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!read_oneliner(&buf, state_dir_ಮಾರ್ಗ("head", opts),
				  READ_ONELINER_WARN_MISSING))
		ಹಿಂತಿರುಗಿ -1;
	ಆದರೆ (get_oid_hex(buf.buf, &oid) ||
	    !(opts->orig_ತಲೆ = lookup_commit_object(the_repository, &oid)))
		ಹಿಂತಿರುಗಿ error(_("invalid orig-head: '%s'"), buf.buf);

	ಆದರೆ (file_exists(state_dir_ಮಾರ್ಗ("quiet", opts)))
		opts->flags &= ~REBASE_NO_QUIET;
	ಇಲ್ಲದಿದ್ದರೆ
		opts->flags |= REBASE_NO_QUIET;

	ಆದರೆ (file_exists(state_dir_ಮಾರ್ಗ("verbose", opts)))
		opts->flags |= REBASE_VERBOSE;

	ಆದರೆ (file_exists(state_dir_ಮಾರ್ಗ("signoff", opts))) {
		opts->signoff = 1;
		opts->flags |= REBASE_FORCE;
	}

	ಆದರೆ (file_exists(state_dir_ಮಾರ್ಗ("allow_rerere_autoupdate", opts))) {
		strbuf_reset(&buf);
		ಆದರೆ (!read_oneliner(&buf, state_dir_ಮಾರ್ಗ("allow_rerere_autoupdate", opts),
				   READ_ONELINER_WARN_MISSING))
			ಹಿಂತಿರುಗಿ -1;
		ಆದರೆ (!strcmp(buf.buf, "--rerere-autoupdate"))
			opts->allow_rerere_autoupdate = RERERE_AUTOUPDATE;
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(buf.buf, "--no-rerere-autoupdate"))
			opts->allow_rerere_autoupdate = RERERE_NOAUTOUPDATE;
		ಇಲ್ಲದಿದ್ದರೆ
			warning(_("ignoring invalid allow_rerere_autoupdate: "
				  "'%s'"), buf.buf);
	}

	ಆದರೆ (file_exists(state_dir_ಮಾರ್ಗ("gpg_sign_opt", opts))) {
		strbuf_reset(&buf);
		ಆದರೆ (!read_oneliner(&buf, state_dir_ಮಾರ್ಗ("gpg_sign_opt", opts),
				   READ_ONELINER_WARN_MISSING))
			ಹಿಂತಿರುಗಿ -1;
		ಬಿಡುಗಡೆ(opts->gpg_sign_opt);
		opts->gpg_sign_opt = xstrdup(buf.buf);
	}

	strbuf_release(&buf);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ rebase_write_basic_state(ರಚನೆ rebase_options *opts)
{
	write_file(state_dir_ಮಾರ್ಗ("head-name", opts), "%s",
		   opts->ತಲೆ_name ? opts->ತಲೆ_name : "detached HEAD");
	write_file(state_dir_ಮಾರ್ಗ("onto", opts), "%s",
		   opts->onto ? oid_ಗಮ್ಯ_hex(&opts->onto->object.oid) : "");
	write_file(state_dir_ಮಾರ್ಗ("orig-head", opts), "%s",
		   oid_ಗಮ್ಯ_hex(&opts->orig_ತಲೆ->object.oid));
	ಆದರೆ (!(opts->flags & REBASE_NO_QUIET))
		write_file(state_dir_ಮಾರ್ಗ("quiet", opts), "%s", "");
	ಆದರೆ (opts->flags & REBASE_VERBOSE)
		write_file(state_dir_ಮಾರ್ಗ("verbose", opts), "%s", "");
	ಆದರೆ (opts->allow_rerere_autoupdate > 0)
		write_file(state_dir_ಮಾರ್ಗ("allow_rerere_autoupdate", opts),
			   "-%s-rerere-autoupdate",
			   opts->allow_rerere_autoupdate == RERERE_AUTOUPDATE ?
				"" : "-no");
	ಆದರೆ (opts->gpg_sign_opt)
		write_file(state_dir_ಮಾರ್ಗ("gpg_sign_opt", opts), "%s",
			   opts->gpg_sign_opt);
	ಆದರೆ (opts->signoff)
		write_file(state_dir_ಮಾರ್ಗ("signoff", opts), "--signoff");

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cleanup_autostash(ರಚನೆ rebase_options *opts)
{
	ಪೂರ್ಣಾಂಕ ret;
	ರಚನೆ strbuf dir = STRBUF_INIT;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ = state_dir_ಮಾರ್ಗ("autostash", opts);

	ಆದರೆ (!file_exists(ಮಾರ್ಗ))
		ಹಿಂತಿರುಗಿ 0;
	ret = apply_autostash(ಮಾರ್ಗ);
	strbuf_addstr(&dir, opts->state_dir);
	ಆದರೆ (remove_dir_recursively(&dir, 0))
		ret = error_errno(_("could not remove '%s'"), opts->state_dir);
	strbuf_release(&dir);

	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಮುಕ್ತಾಯ_rebase(ರಚನೆ rebase_options *opts)
{
	ರಚನೆ strbuf dir = STRBUF_INIT;
	ಪೂರ್ಣಾಂಕ ret = 0;

	refs_delete_ref(get_main_ref_store(the_repository), ಖಾಲಿ,
			"REBASE_HEAD", ಖಾಲಿ, REF_NO_DEREF);
	refs_delete_ref(get_main_ref_store(the_repository), ಖಾಲಿ,
			"AUTO_MERGE", ಖಾಲಿ, REF_NO_DEREF);
	apply_autostash(state_dir_ಮಾರ್ಗ("autostash", opts));
	/*
	 * We ignore errors in 'git maintenance run --auto', since the
	 * user should see them.
	 */
	ಸತತ_auto_maintenance(the_repository,
			!(opts->flags & (REBASE_NO_QUIET|REBASE_VERBOSE)));

	ಆದರೆ (opts->type == REBASE_ವಿಲೀನ) {
		ರಚನೆ replay_opts replay = REPLAY_OPTS_INIT;

		replay.action = REPLAY_INTERACTIVE_REBASE;
		ret = sequencer_remove_state(&replay);
		replay_opts_release(&replay);
	} ಇಲ್ಲದಿದ್ದರೆ {
		strbuf_addstr(&dir, opts->state_dir);
		ಆದರೆ (remove_dir_recursively(&dir, 0))
			ret = error(_("could not remove '%s'"),
				    opts->state_dir);
		strbuf_release(&dir);
	}

	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ move_ಗಮ್ಯ_original_branch(ರಚನೆ rebase_options *opts)
{
	ರಚನೆ strbuf branch_reflog = STRBUF_INIT, ತಲೆ_reflog = STRBUF_INIT;
	ರಚನೆ reset_working_ವೃಕ್ಷ_options ropts = { 0 };
	ಪೂರ್ಣಾಂಕ ret;

	ಆದರೆ (!opts->ತಲೆ_name)
		ಹಿಂತಿರುಗಿ 0; /* nothing to move back to */

	ಆದರೆ (!opts->onto)
		BUG("move_to_original_branch without onto");

	strbuf_addf(&branch_reflog, "%s (finish): %s onto %s",
		    opts->reflog_action,
		    opts->ತಲೆ_name, oid_ಗಮ್ಯ_hex(&opts->onto->object.oid));
	strbuf_addf(&ತಲೆ_reflog, "%s (finish): returning to %s",
		    opts->reflog_action, opts->ತಲೆ_name);
	ropts.branch = opts->ತಲೆ_name;
	ropts.flags = RESET_WORKING_ವೃಕ್ಷ_REFS_ONLY |
		      RESET_WORKING_ವೃಕ್ಷ_UPDATE_ತಲೆ;
	ropts.branch_ಸಂದೇಶ = branch_reflog.buf;
	ropts.ತಲೆ_ಸಂದೇಶ = ತಲೆ_reflog.buf;
	ret = reset_working_ವೃಕ್ಷ(the_repository, &ropts);

	strbuf_release(&branch_reflog);
	strbuf_release(&ತಲೆ_reflog);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಸತತ_am(ರಚನೆ rebase_options *opts)
{
	ರಚನೆ ಮಗು_process am = ಮಗು_PROCESS_INIT;
	ರಚನೆ ಮಗು_process format_patch = ಮಗು_PROCESS_INIT;
	ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2;
	ಅಕ್ಷರ *rebased_patches;

	am.git_cmd = 1;
	strvec_ತಳ್ಳು(&am.ವಾದಗಳು, "am");
	strvec_pushf(&am.env, GIT_REFLOG_ACTION_ENVIRONMENT "=%s (pick)",
		     opts->reflog_action);
	ಆದರೆ (opts->action == ACTION_CONTINUE) {
		strvec_ತಳ್ಳು(&am.ವಾದಗಳು, "--resolved");
		strvec_pushf(&am.ವಾದಗಳು, "--resolvemsg=%s", rebase_resolvemsg);
		ಆದರೆ (opts->gpg_sign_opt)
			strvec_ತಳ್ಳು(&am.ವಾದಗಳು, opts->gpg_sign_opt);
		ಸ್ಥಿತಿ_2 = ಸತತ_command(&am);
		ಆದರೆ (ಸ್ಥಿತಿ_2)
			ಹಿಂತಿರುಗಿ ಸ್ಥಿತಿ_2;

		ಹಿಂತಿರುಗಿ move_ಗಮ್ಯ_original_branch(opts);
	}
	ಆದರೆ (opts->action == ACTION_SKIP) {
		strvec_ತಳ್ಳು(&am.ವಾದಗಳು, "--skip");
		strvec_pushf(&am.ವಾದಗಳು, "--resolvemsg=%s", rebase_resolvemsg);
		ಸ್ಥಿತಿ_2 = ಸತತ_command(&am);
		ಆದರೆ (ಸ್ಥಿತಿ_2)
			ಹಿಂತಿರುಗಿ ಸ್ಥಿತಿ_2;

		ಹಿಂತಿರುಗಿ move_ಗಮ್ಯ_original_branch(opts);
	}
	ಆದರೆ (opts->action == ACTION_SHOW_CURRENT_PATCH) {
		strvec_ತಳ್ಳು(&am.ವಾದಗಳು, "--show-current-patch");
		ಹಿಂತಿರುಗಿ ಸತತ_command(&am);
	}

	rebased_patches = repo_git_ಮಾರ್ಗ(the_repository, "rebased-patches");
	format_patch.ಹೊರಹೋಗು = open(rebased_patches,
				O_WRONLY | O_CREAT | O_TRUNC, 0666);
	ಆದರೆ (format_patch.ಹೊರಹೋಗು < 0) {
		ಸ್ಥಿತಿ_2 = error_errno(_("could not open '%s' for writing"),
				     rebased_patches);
		ಬಿಡುಗಡೆ(rebased_patches);
		ಮಗು_process_clear(&am);
		ಹಿಂತಿರುಗಿ ಸ್ಥಿತಿ_2;
	}

	format_patch.git_cmd = 1;
	strvec_pushl(&format_patch.ವಾದಗಳು, "format-patch", "-k", "--stdout",
		     "--full-index", "--cherry-pick", "--right-only",
		     "--default-prefix", "--no-renames",
		     "--no-cover-letter", "--pretty=mboxrd", "--topo-order",
		     "--no-base", ಖಾಲಿ);
	ಆದರೆ (opts->git_format_patch_opt.len)
		strvec_ವಿಭಜಿಸು(&format_patch.ವಾದಗಳು,
			     opts->git_format_patch_opt.buf);
	strvec_pushf(&format_patch.ವಾದಗಳು, "%s...%s",
		     oid_ಗಮ್ಯ_hex(opts->ಬೇರು ?
				/* this is now equivalent to !opts->upstream */
				&opts->onto->object.oid :
				&opts->upstream->object.oid),
		     oid_ಗಮ್ಯ_hex(&opts->orig_ತಲೆ->object.oid));
	ಆದರೆ (opts->restrict_revision)
		strvec_pushf(&format_patch.ವಾದಗಳು, "^%s",
			     oid_ಗಮ್ಯ_hex(&opts->restrict_revision->object.oid));

	ಸ್ಥಿತಿ_2 = ಸತತ_command(&format_patch);
	ಆದರೆ (ಸ್ಥಿತಿ_2) {
		ರಚನೆ reset_working_ವೃಕ್ಷ_options ropts = { 0 };
		unlink(rebased_patches);
		ಬಿಡುಗಡೆ(rebased_patches);
		ಮಗು_process_clear(&am);

		ropts.oid = &opts->orig_ತಲೆ->object.oid;
		ropts.branch = opts->ತಲೆ_name;
		ropts.default_reflog_action = opts->reflog_action;
		ropts.flags = RESET_WORKING_ವೃಕ್ಷ_UPDATE_ತಲೆ;
		reset_working_ವೃಕ್ಷ(the_repository, &ropts);
		error(_("\ngit encountered an error while preparing the "
			"patches to replay\n"
			"these revisions:\n"
			"\n    %s\n\n"
			"As a result, git cannot rebase them."),
		      opts->revisions);

		ಹಿಂತಿರುಗಿ ಸ್ಥಿತಿ_2;
	}

	am.ಒಳ = open(rebased_patches, O_RDONLY);
	ಆದರೆ (am.ಒಳ < 0) {
		ಸ್ಥಿತಿ_2 = error_errno(_("could not open '%s' for reading"),
				     rebased_patches);
		ಬಿಡುಗಡೆ(rebased_patches);
		ಮಗು_process_clear(&am);
		ಹಿಂತಿರುಗಿ ಸ್ಥಿತಿ_2;
	}

	strvec_pushv(&am.ವಾದಗಳು, opts->git_am_opts.v);
	strvec_ತಳ್ಳು(&am.ವಾದಗಳು, "--rebasing");
	strvec_pushf(&am.ವಾದಗಳು, "--resolvemsg=%s", rebase_resolvemsg);
	strvec_ತಳ್ಳು(&am.ವಾದಗಳು, "--patch-format=mboxrd");
	ಆದರೆ (opts->allow_rerere_autoupdate == RERERE_AUTOUPDATE)
		strvec_ತಳ್ಳು(&am.ವಾದಗಳು, "--rerere-autoupdate");
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (opts->allow_rerere_autoupdate == RERERE_NOAUTOUPDATE)
		strvec_ತಳ್ಳು(&am.ವಾದಗಳು, "--no-rerere-autoupdate");
	ಆದರೆ (opts->gpg_sign_opt)
		strvec_ತಳ್ಳು(&am.ವಾದಗಳು, opts->gpg_sign_opt);
	ಸ್ಥಿತಿ_2 = ಸತತ_command(&am);
	unlink(rebased_patches);
	ಬಿಡುಗಡೆ(rebased_patches);

	ಆದರೆ (!ಸ್ಥಿತಿ_2) {
		ಹಿಂತಿರುಗಿ move_ಗಮ್ಯ_original_branch(opts);
	}

	ಆದರೆ (is_directory(opts->state_dir))
		rebase_write_basic_state(opts);

	ಹಿಂತಿರುಗಿ ಸ್ಥಿತಿ_2;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಸತತ_specific_rebase(ರಚನೆ rebase_options *opts)
{
	ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2;

	ಆದರೆ (opts->type == REBASE_ವಿಲೀನ) {
		/* Run sequencer-based rebase */
		ಆದರೆ (!(opts->flags & REBASE_INTERACTIVE_EXPLICIT))
			setenv("GIT_SEQUENCE_EDITOR", ":", 1);
		ಆದರೆ (opts->gpg_sign_opt) {
			/* remove the leading "-S" */
			ಅಕ್ಷರ *tmp = xstrdup(opts->gpg_sign_opt + 2);
			ಬಿಡುಗಡೆ(opts->gpg_sign_opt);
			opts->gpg_sign_opt = tmp;
		}

		ಸ್ಥಿತಿ_2 = ಸತತ_sequencer_rebase(opts);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (opts->type == REBASE_APPLY)
		ಸ್ಥಿತಿ_2 = ಸತತ_am(opts);
	ಇಲ್ಲದಿದ್ದರೆ
		BUG("Unhandled rebase type %d", opts->type);

	ಆದರೆ (opts->dont_ಮುಕ್ತಾಯ_rebase)
		; /* do nothing */
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (opts->type == REBASE_ವಿಲೀನ) {
		ಪೂರ್ಣಾಂಕ quiet = !(opts->flags & (REBASE_NO_QUIET|REBASE_VERBOSE));

		/*
		 * The sequencer cleans up after itself. Its state directory
		 * is gone once it is done, and stays while it is stopped.
		 */
		ಆದರೆ (ಸ್ಥಿತಿ_2 == 0 && !is_directory(opts->state_dir))
			ಸತತ_auto_maintenance(the_repository, quiet);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಸ್ಥಿತಿ_2 == 0) {
		ಆದರೆ (!file_exists(state_dir_ಮಾರ್ಗ("stopped-sha", opts)))
			ಮುಕ್ತಾಯ_rebase(opts);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಸ್ಥಿತಿ_2 == 2) {
		ರಚನೆ strbuf dir = STRBUF_INIT;

		apply_autostash(state_dir_ಮಾರ್ಗ("autostash", opts));
		strbuf_addstr(&dir, opts->state_dir);
		remove_dir_recursively(&dir, 0);
		strbuf_release(&dir);
		die("Nothing to do");
	}

	ಹಿಂತಿರುಗಿ ಸ್ಥಿತಿ_2 ? -1 : 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_rebase_merges_ಮೌಲ್ಯ(ರಚನೆ rebase_options *options, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ)
{
	ಆದರೆ (!strcmp("no-rebase-cousins", ಮೌಲ್ಯ))
		options->rebase_cousins = 0;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp("rebase-cousins", ಮೌಲ್ಯ))
		options->rebase_cousins = 1;
	ಇಲ್ಲದಿದ್ದರೆ
		die(_("Unknown rebase-merges mode: %s"), ಮೌಲ್ಯ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ rebase_config(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *var, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ,
			 ಸ್ಥಿರಾಂಕ ರಚನೆ config_context *ctx, ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
	ರಚನೆ rebase_options *opts = ದತ್ತಾಂಶ;

	ಆದರೆ (!strcmp(var, "rebase.stat")) {
		ಆದರೆ (git_config_bool(var, ಮೌಲ್ಯ))
			opts->flags |= REBASE_DIFFSTAT;
		ಇಲ್ಲದಿದ್ದರೆ
			opts->flags &= ~REBASE_DIFFSTAT;
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (!strcmp(var, "rebase.autosquash")) {
		opts->config_autosquash = git_config_bool(var, ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (!strcmp(var, "commit.gpgsign")) {
		ಬಿಡುಗಡೆ(opts->gpg_sign_opt);
		opts->gpg_sign_opt = git_config_bool(var, ಮೌಲ್ಯ) ?
			xstrdup("-S") : ಖಾಲಿ;
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (!strcmp(var, "rebase.autostash")) {
		opts->autostash = git_config_bool(var, ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (!strcmp(var, "rebase.rebasemerges")) {
		opts->config_rebase_merges = git_parse_maybe_bool(ಮೌಲ್ಯ);
		ಆದರೆ (opts->config_rebase_merges < 0) {
			opts->config_rebase_merges = 1;
			parse_rebase_merges_ಮೌಲ್ಯ(opts, ಮೌಲ್ಯ);
		} ಇಲ್ಲದಿದ್ದರೆ {
			opts->rebase_cousins = 0;
		}
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (!strcmp(var, "rebase.updaterefs")) {
		opts->config_update_refs = git_config_bool(var, ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (!strcmp(var, "rebase.reschedulefailedexec")) {
		opts->reschedule_failed_exec = git_config_bool(var, ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (!strcmp(var, "rebase.forkpoint")) {
		opts->fork_ಬಿಂದು = git_config_bool(var, ಮೌಲ್ಯ) ? -1 : 0;
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (!strcmp(var, "rebase.backend")) {
		FREE_AND_NULL(opts->default_backend);
		ಹಿಂತಿರುಗಿ git_config_string(&opts->default_backend, var, ಮೌಲ್ಯ);
	}

	ಹಿಂತಿರುಗಿ git_default_config(var, ಮೌಲ್ಯ, ctx, ದತ್ತಾಂಶ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ checkout_ಮೇಲೆ_ಗಮ್ಯ_date(ರಚನೆ rebase_options *options)
{
	ರಚನೆ strbuf buf = STRBUF_INIT;
	ರಚನೆ reset_working_ವೃಕ್ಷ_options ropts = { 0 };
	ಪೂರ್ಣಾಂಕ ret = 0;

	strbuf_addf(&buf, "%s: checkout %s",
		    options->reflog_action, options->switch_ಗಮ್ಯ);
	ropts.oid = &options->orig_ತಲೆ->object.oid;
	ropts.branch = options->ತಲೆ_name;
	ropts.flags = RESET_WORKING_ವೃಕ್ಷ_ಸತತ_POST_CHECKOUT_HOOK |
		      RESET_WORKING_ವೃಕ್ಷ_UPDATE_ತಲೆ;
	ಆದರೆ (!ropts.branch)
		ropts.flags |=  RESET_WORKING_ವೃಕ್ಷ_DETACH;
	ropts.ತಲೆ_ಸಂದೇಶ = buf.buf;
	ಆದರೆ (reset_working_ವೃಕ್ಷ(the_repository, &ropts) < 0)
		ret = error(_("could not switch to %s"), options->switch_ಗಮ್ಯ);
	strbuf_release(&buf);

	ಹಿಂತಿರುಗಿ ret;
}

/*
 * Determines whether the commits in from..to are linear, i.e. contain
 * no merge commits. This function *expects* `from` to be an ancestor of
 * `to`.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ is_ರೇಖೀಯ_history(ರಚನೆ commit *from, ರಚನೆ commit *ಗಮ್ಯ)
{
	ಆಗ (ಗಮ್ಯ && ಗಮ್ಯ != from) {
		repo_parse_commit(the_repository, ಗಮ್ಯ);
		ಆದರೆ (!ಗಮ್ಯ->parents)
			ಹಿಂತಿರುಗಿ 1;
		ಆದರೆ (ಗಮ್ಯ->parents->ಮುಂದಿನ)
			ಹಿಂತಿರುಗಿ 0;
		ಗಮ್ಯ = ಗಮ್ಯ->parents->ಅಂಶ;
	}
	ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ can_fast_forward(ರಚನೆ commit *onto, ರಚನೆ commit *upstream,
			    ರಚನೆ commit *restrict_revision,
			    ರಚನೆ commit *ತಲೆ, ರಚನೆ object_id *branch_ತಳ)
{
	ರಚನೆ commit_list *ವಿಲೀನ_bases = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ res = 0;

	ಆದರೆ (is_null_oid(branch_ತಳ))
		ಹೋಗು ಮುಗಿದ; /* fill_branch_base() found multiple merge bases */

	ಆದರೆ (!oideq(branch_ತಳ, &onto->object.oid))
		ಹೋಗು ಮುಗಿದ;

	ಆದರೆ (restrict_revision && !oideq(&restrict_revision->object.oid, branch_ತಳ))
		ಹೋಗು ಮುಗಿದ;

	ಆದರೆ (!upstream)
		ಹೋಗು ಮುಗಿದ;

	ಆದರೆ (repo_get_ವಿಲೀನ_bases(the_repository, upstream, ತಲೆ, &ವಿಲೀನ_bases) < 0)
		ನಿರ್ಗಮಿಸು(128);
	ಆದರೆ (!ವಿಲೀನ_bases || ವಿಲೀನ_bases->ಮುಂದಿನ)
		ಹೋಗು ಮುಗಿದ;

	ಆದರೆ (!oideq(&onto->object.oid, &ವಿಲೀನ_bases->ಅಂಶ->object.oid))
		ಹೋಗು ಮುಗಿದ;

	res = 1;

ಮುಗಿದ:
	commit_list_free(ವಿಲೀನ_bases);
	ಹಿಂತಿರುಗಿ res && is_ರೇಖೀಯ_history(onto, ತಲೆ);
}

ಸ್ಥಿರ ಶೂನ್ಯ fill_branch_ತಳ(ರಚನೆ rebase_options *options,
			    ರಚನೆ object_id *branch_ತಳ)
{
	ರಚನೆ commit_list *ವಿಲೀನ_bases = ಖಾಲಿ;

	ಆದರೆ (repo_get_ವಿಲೀನ_bases(the_repository, options->onto,
				 options->orig_ತಲೆ, &ವಿಲೀನ_bases) < 0)
		ನಿರ್ಗಮಿಸು(128);
	ಆದರೆ (!ವಿಲೀನ_bases || ವಿಲೀನ_bases->ಮುಂದಿನ)
		oidcpy(branch_ತಳ, null_oid(the_hash_algo));
	ಇಲ್ಲದಿದ್ದರೆ
		oidcpy(branch_ತಳ, &ವಿಲೀನ_bases->ಅಂಶ->object.oid);

	commit_list_free(ವಿಲೀನ_bases);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ parse_opt_am(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ರಚನೆ rebase_options *opts = opt->ಮೌಲ್ಯ;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	ಆದರೆ (opts->type != REBASE_UNSPECIFIED && opts->type != REBASE_APPLY)
	    die(_("apply options and merge options cannot be used together"));

	opts->type = REBASE_APPLY;

	ಹಿಂತಿರುಗಿ 0;
}

/* -i followed by -m is still -i */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ parse_opt_ವಿಲೀನ(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ರಚನೆ rebase_options *opts = opt->ಮೌಲ್ಯ;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	ಆದರೆ (opts->type != REBASE_UNSPECIFIED && opts->type != REBASE_ವಿಲೀನ)
	    die(_("apply options and merge options cannot be used together"));

	opts->type = REBASE_ವಿಲೀನ;

	ಹಿಂತಿರುಗಿ 0;
}

/* -i followed by -r is still explicitly interactive, but -r alone is not */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ parse_opt_interactive(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg,
				 ಪೂರ್ಣಾಂಕ unset)
{
	ರಚನೆ rebase_options *opts = opt->ಮೌಲ್ಯ;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	ಆದರೆ (opts->type != REBASE_UNSPECIFIED && opts->type != REBASE_ವಿಲೀನ)
	    die(_("apply options and merge options cannot be used together"));

	opts->type = REBASE_ವಿಲೀನ;
	opts->flags |= REBASE_INTERACTIVE_EXPLICIT;

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಎಣಿಕೆ ಖಾಲಿಗಳು_type parse_ಖಾಲಿಗಳು_ಮೌಲ್ಯ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ)
{
	ಆದರೆ (!strcasecmp(ಮೌಲ್ಯ, "drop"))
		ಹಿಂತಿರುಗಿ ಖಾಲಿಗಳು_DROP;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcasecmp(ಮೌಲ್ಯ, "keep"))
		ಹಿಂತಿರುಗಿ ಖಾಲಿಗಳು_KEEP;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcasecmp(ಮೌಲ್ಯ, "stop"))
		ಹಿಂತಿರುಗಿ ಖಾಲಿಗಳು_STOP;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcasecmp(ಮೌಲ್ಯ, "ask")) {
		warning(_("--empty=ask is deprecated; use '--empty=stop' instead."));
		ಹಿಂತಿರುಗಿ ಖಾಲಿಗಳು_STOP;
	}

	die(_("unrecognized empty type '%s'; valid values are \"drop\", \"keep\", and \"stop\"."), ಮೌಲ್ಯ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ parse_opt_keep_ಖಾಲಿಗಳು(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg,
				ಪೂರ್ಣಾಂಕ unset)
{
	ರಚನೆ rebase_options *opts = opt->ಮೌಲ್ಯ;

	BUG_ON_OPT_ARG(arg);

	imply_ವಿಲೀನ(opts, unset ? "--no-keep-empty" : "--keep-empty");
	opts->keep_ಖಾಲಿಗಳು = !unset;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ parse_opt_ಖಾಲಿಗಳು(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ರಚನೆ rebase_options *options = opt->ಮೌಲ್ಯ;
	ಎಣಿಕೆ ಖಾಲಿಗಳು_type ಮೌಲ್ಯ = parse_ಖಾಲಿಗಳು_ಮೌಲ್ಯ(arg);

	BUG_ON_OPT_NEG(unset);

	options->ಖಾಲಿಗಳು = ಮೌಲ್ಯ;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ parse_opt_rebase_merges(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ರಚನೆ rebase_options *options = opt->ಮೌಲ್ಯ;

	options->rebase_merges = !unset;
	options->rebase_cousins = 0;

	ಆದರೆ (arg) {
		ಆದರೆ (!*arg) {
			warning(_("--rebase-merges with an empty string "
				  "argument is deprecated and will stop "
				  "working in a future version of Git. Use "
				  "--rebase-merges without an argument "
				  "instead, which does the same thing."));
			ಹಿಂತಿರುಗಿ 0;
		}
		parse_rebase_merges_ಮೌಲ್ಯ(options, arg);
	}

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ NORETURN error_on_missing_default_upstream(ಶೂನ್ಯ)
{
	ರಚನೆ branch *current_branch = branch_get(ಖಾಲಿ);

	ಬರೆ(_("%s\n"
		 "Please specify which branch you want to rebase against.\n"
		 "See git-rebase(1) for details.\n"
		 "\n"
		 "    git rebase '<branch>'\n"
		 "\n"),
		current_branch ? _("There is no tracking information for "
			"the current branch.") :
			_("You are not currently on a branch."));

	ಆದರೆ (current_branch) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *remote = current_branch->remote_name;

		ಆದರೆ (!remote)
			remote = _("<remote>");

		ಬರೆ(_("If you wish to set tracking information for this "
			 "branch you can do so with:\n"
			 "\n"
			 "    git branch --set-upstream-to=%s/<branch> %s\n"
			 "\n"),
		       remote, current_branch->name);
	}
	ನಿರ್ಗಮಿಸು(1);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ check_exec_cmd(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cmd)
{
	ಆದರೆ (strchr(cmd, '\n'))
		ಹಿಂತಿರುಗಿ error(_("exec commands cannot contain newlines"));

	/* Does the command consist purely of whitespace? */
	ಆದರೆ (!cmd[strspn(cmd, " \t\r\f\v")])
		ಹಿಂತಿರುಗಿ error(_("empty exec command"));

	ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ cmd_rebase(ಪೂರ್ಣಾಂಕ argc,
	       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
	       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
	       ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ rebase_options options = REBASE_OPTIONS_INIT;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *branch_name;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *strategy_opt = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ ret, flags, ಒಟ್ಟು_argc, ಒಳ_ಪ್ರಗತಿ = 0;
	ಪೂರ್ಣಾಂಕ keep_ತಳ = 0;
	ಪೂರ್ಣಾಂಕ ok_ಗಮ್ಯ_skip_pre_rebase = 0;
	ರಚನೆ strbuf ಸಂದೇಶ = STRBUF_INIT;
	ರಚನೆ strbuf revisions = STRBUF_INIT;
	ರಚನೆ strbuf buf = STRBUF_INIT;
	ರಚನೆ object_id branch_ತಳ;
	ಪೂರ್ಣಾಂಕ ignore_whitespace = 0;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *gpg_sign = ಖಾಲಿ;
	ರಚನೆ object_id squash_onto;
	ಅಕ್ಷರ *squash_onto_name = ಖಾಲಿ;
	ಅಕ್ಷರ *keep_ತಳ_onto_name = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ reschedule_failed_exec = -1;
	ಪೂರ್ಣಾಂಕ allow_preemptive_ff = 1;
	ಪೂರ್ಣಾಂಕ preserve_merges_selected = 0;
	ರಚನೆ reset_working_ವೃಕ್ಷ_options ropts = { 0 };
	ರಚನೆ option builtin_rebase_options[] = {
		OPT_STRING(0, "onto", &options.onto_name,
			   N_("revision"),
			   N_("rebase onto given branch instead of upstream")),
		OPT_BOOL(0, "keep-base", &keep_ತಳ,
			 N_("use the merge-base of upstream and branch as the current base")),
		OPT_BOOL(0, "no-verify", &ok_ಗಮ್ಯ_skip_pre_rebase,
			 N_("allow pre-rebase hook to run")),
		OPT_NEGBIT('q', "quiet", &options.flags,
			   N_("be quiet. implies --no-stat"),
			   REBASE_NO_QUIET | REBASE_VERBOSE | REBASE_DIFFSTAT),
		OPT_ಬಿಟ್('v', "verbose", &options.flags,
			N_("display a diffstat of what changed upstream"),
			REBASE_NO_QUIET | REBASE_VERBOSE | REBASE_DIFFSTAT),
		{
			.type = OPTION_NEGBIT,
			.short_name = 'n',
			.long_name = "no-stat",
			.ಮೌಲ್ಯ = &options.flags,
			.precision = ಗಾತ್ರ(options.flags),
			.help = N_("do not show diffstat of what changed upstream"),
			.flags = PARSE_OPT_NOARG,
			.defval = REBASE_DIFFSTAT,
		},
		OPT_STRVEC(0, "trailer", &options.trailer_ವಾದಗಳು, N_("trailer"),
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
		OPT_PASSTHRU_ARGV('C', ಖಾಲಿ, &options.git_am_opts, N_("n"),
				  N_("passed to 'git apply'"), 0),
		OPT_BOOL(0, "ignore-whitespace", &ignore_whitespace,
			 N_("ignore changes in whitespace")),
		OPT_PASSTHRU_ARGV(0, "whitespace", &options.git_am_opts,
				  N_("action"), N_("passed to 'git apply'"), 0),
		OPT_ಬಿಟ್('f', "force-rebase", &options.flags,
			N_("cherry-pick all commits, even if unchanged"),
			REBASE_FORCE),
		OPT_ಬಿಟ್(0, "no-ff", &options.flags,
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
		OPT_CALLBACK_F(0, "apply", &options, ಖಾಲಿ,
			N_("use apply strategies to rebase"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			parse_opt_am),
		OPT_CALLBACK_F('m', "merge", &options, ಖಾಲಿ,
			N_("use merging strategies to rebase"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			parse_opt_ವಿಲೀನ),
		OPT_CALLBACK_F('i', "interactive", &options, ಖಾಲಿ,
			N_("let the user edit the list of commits to rebase"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			parse_opt_interactive),
		OPT_ಗಣ_INT_F('p', "preserve-merges", &preserve_merges_selected,
			      N_("(REMOVED) was: try to recreate merges "
				 "instead of ignoring them"),
			      1, PARSE_OPT_HIDDEN),
		OPT_RERERE_AUTOUPDATE(&options.allow_rerere_autoupdate),
		OPT_CALLBACK_F(0, "empty", &options, "(drop|keep|stop)",
			       N_("how to handle commits that become empty"),
			       PARSE_OPT_NONEG, parse_opt_ಖಾಲಿಗಳು),
		OPT_CALLBACK_F('k', "keep-empty", &options, ಖಾಲಿ,
			N_("keep commits which start empty"),
			PARSE_OPT_NOARG | PARSE_OPT_HIDDEN,
			parse_opt_keep_ಖಾಲಿಗಳು),
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
			   &options.allow_ಖಾಲಿಗಳು_ಸಂದೇಶ,
			   N_("allow rebasing commits with empty messages"),
			   PARSE_OPT_HIDDEN),
		OPT_CALLBACK_F('r', "rebase-merges", &options, N_("mode"),
			N_("try to rebase merges instead of skipping them"),
			PARSE_OPT_OPTARG, parse_opt_rebase_merges),
		OPT_BOOL(0, "fork-point", &options.fork_ಬಿಂದು,
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
		OPT_ಕೊನೆ(),
	};
	ಪೂರ್ಣಾಂಕ i;

	show_usage_with_options_if_asked(argc, argv,
					 builtin_rebase_usage,
					 builtin_rebase_options);

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ WITH_BREAKING_CHANGES
	warn_on_auto_comment_char = true;
#ಆದರೆ_ಅಂತ್ಯ /* !WITH_BREAKING_CHANGES */
	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_ತುಂಬಿದ_ಸೂಚ್ಯಂಕ = 0;

	repo_config(the_repository, rebase_config, &options);
	/* options.gpg_sign_opt will be either "-S" or NULL */
	gpg_sign = options.gpg_sign_opt ? "" : ಖಾಲಿ;
	FREE_AND_NULL(options.gpg_sign_opt);

	strbuf_reset(&buf);
	strbuf_addf(&buf, "%s/applying", apply_dir());
	ಆದರೆ(file_exists(buf.buf))
		die(_("It looks like 'git am' is in progress. Cannot rebase."));

	ಆದರೆ (is_directory(apply_dir())) {
		options.type = REBASE_APPLY;
		options.state_dir = apply_dir();
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (is_directory(ವಿಲೀನ_dir())) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "%s/rewritten", ವಿಲೀನ_dir());
		ಆದರೆ (!(options.action == ACTION_ABORT) && is_directory(buf.buf)) {
			die(_("`rebase --preserve-merges` (-p) is no longer supported.\n"
			"Use `git rebase --abort` to terminate current rebase.\n"
			"Or downgrade to v2.33, or earlier, to complete the rebase."));
		} ಇಲ್ಲದಿದ್ದರೆ {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "%s/interactive", ವಿಲೀನ_dir());
			options.type = REBASE_ವಿಲೀನ;
			ಆದರೆ (file_exists(buf.buf))
				options.flags |= REBASE_INTERACTIVE_EXPLICIT;
		}
		options.state_dir = ವಿಲೀನ_dir();
	}

	ಆದರೆ (options.type != REBASE_UNSPECIFIED)
		ಒಳ_ಪ್ರಗತಿ = 1;

	ಒಟ್ಟು_argc = argc;
	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ,
			     builtin_rebase_options,
			     builtin_rebase_usage, 0);

	ಆದರೆ (options.trailer_ವಾದಗಳು.nr) {
		ಆದರೆ (validate_trailer_ವಾದಗಳು(&options.trailer_ವಾದಗಳು))
			die(ಖಾಲಿ);
		options.flags |= REBASE_FORCE;
	}

	ಆದರೆ (preserve_merges_selected)
		die(_("--preserve-merges was replaced by --rebase-merges\n"
			"Note: Your `pull.rebase` configuration may also be set to 'preserve',\n"
			"which is no longer supported; use 'merges' instead"));

	ಆದರೆ (options.action != ACTION_NONE && ಒಟ್ಟು_argc != 2) {
		usage_with_options(builtin_rebase_usage,
				   builtin_rebase_options);
	}

	ಆದರೆ (argc > 2)
		usage_with_options(builtin_rebase_usage,
				   builtin_rebase_options);

	ಆದರೆ (keep_ತಳ) {
		ಆದರೆ (options.onto_name)
			die(_("options '%s' and '%s' cannot be used together"), "--keep-base", "--onto");
		ಆದರೆ (options.ಬೇರು)
			die(_("options '%s' and '%s' cannot be used together"), "--keep-base", "--root");
		/*
		 * --keep-base defaults to --no-fork-point to keep the
		 * base the same.
		 */
		ಆದರೆ (options.fork_ಬಿಂದು < 0)
			options.fork_ಬಿಂದು = 0;
	}
	ಆದರೆ (options.ಬೇರು && options.fork_ಬಿಂದು > 0)
		die(_("options '%s' and '%s' cannot be used together"), "--root", "--fork-point");

	ಆದರೆ (options.action != ACTION_NONE && !ಒಳ_ಪ್ರಗತಿ)
		die(_("no rebase in progress"));

	ಆದರೆ (options.action == ACTION_EDIT_TODO && !is_ವಿಲೀನ(&options))
		die(_("The --edit-todo action can only be used during "
		      "interactive rebase."));

	ಆದರೆ (trace2_is_enabled()) {
		ಆದರೆ (is_ವಿಲೀನ(&options))
			trace2_cmd_ಬಹುಲಕ("interactive");
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (options.exec.nr)
			trace2_cmd_ಬಹುಲಕ("interactive-exec");
		ಇಲ್ಲದಿದ್ದರೆ
			trace2_cmd_ಬಹುಲಕ(action_ಹೆಸರುಗಳು[options.action]);
	}

	options.reflog_action = getenv(GIT_REFLOG_ACTION_ENVIRONMENT);
	options.reflog_action =
		xstrdup(options.reflog_action ? options.reflog_action : "rebase");

	ಆಯ್ಕೆ (options.action) {
	ಸ್ಥಿತಿ ACTION_CONTINUE: {
		ರಚನೆ object_id ತಲೆ;
		ರಚನೆ ಬೀಗ_file ಬೀಗ_file = ಬೀಗ_INIT;
		ಪೂರ್ಣಾಂಕ fd;

		/* Sanity check */
		ಆದರೆ (repo_get_oid(the_repository, "HEAD", &ತಲೆ))
			die(_("Cannot read HEAD"));

		fd = repo_hold_locked_ಸೂಚ್ಯಂಕ(the_repository, &ಬೀಗ_file, 0);
		ಆದರೆ (repo_read_ಸೂಚ್ಯಂಕ(the_repository) < 0)
			die(_("could not read index"));
		refresh_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ, REFRESH_QUIET, ಖಾಲಿ, ಖಾಲಿ,
			      ಖಾಲಿ);
		ಆದರೆ (0 <= fd)
			repo_update_ಸೂಚ್ಯಂಕ_if_able(the_repository, &ಬೀಗ_file);
		rollback_ಬೀಗ_file(&ಬೀಗ_file);

		ಆದರೆ (has_unstaged_changes(the_repository, 1)) {
			puts(_("You must edit all merge conflicts and then\n"
			       "mark them as resolved using git add"));
			ನಿರ್ಗಮಿಸು(1);
		}
		ಆದರೆ (read_basic_state(&options))
			ನಿರ್ಗಮಿಸು(1);
		ಹೋಗು ಸತತ_rebase;
	}
	ಸ್ಥಿತಿ ACTION_SKIP: {
		ರಚನೆ string_list ವಿಲೀನ_rr = STRING_LIST_INIT_DUP;

		rerere_clear(the_repository, &ವಿಲೀನ_rr);
		string_list_clear(&ವಿಲೀನ_rr, 1);
		ropts.flags = RESET_WORKING_ವೃಕ್ಷ_HARD |
			      RESET_WORKING_ವೃಕ್ಷ_UPDATE_ತಲೆ;
		ಆದರೆ (reset_working_ವೃಕ್ಷ(the_repository, &ropts) < 0)
			die(_("could not discard worktree changes"));
		remove_branch_state(the_repository, 0);
		ಆದರೆ (read_basic_state(&options))
			ನಿರ್ಗಮಿಸು(1);
		ಹೋಗು ಸತತ_rebase;
	}
	ಸ್ಥಿತಿ ACTION_ABORT: {
		ರಚನೆ string_list ವಿಲೀನ_rr = STRING_LIST_INIT_DUP;
		ರಚನೆ strbuf ತಲೆ_ಸಂದೇಶ = STRBUF_INIT;

		rerere_clear(the_repository, &ವಿಲೀನ_rr);
		string_list_clear(&ವಿಲೀನ_rr, 1);

		ಆದರೆ (read_basic_state(&options))
			ನಿರ್ಗಮಿಸು(1);

		strbuf_addf(&ತಲೆ_ಸಂದೇಶ, "%s (abort): returning to %s",
			    options.reflog_action,
			    options.ತಲೆ_name ? options.ತಲೆ_name
					      : oid_ಗಮ್ಯ_hex(&options.orig_ತಲೆ->object.oid));
		ropts.oid = &options.orig_ತಲೆ->object.oid;
		ropts.ತಲೆ_ಸಂದೇಶ = ತಲೆ_ಸಂದೇಶ.buf;
		ropts.branch = options.ತಲೆ_name;
		ropts.flags = RESET_WORKING_ವೃಕ್ಷ_HARD |
			      RESET_WORKING_ವೃಕ್ಷ_UPDATE_ತಲೆ;
		ಆದರೆ (reset_working_ವೃಕ್ಷ(the_repository, &ropts) < 0)
			die(_("could not move back to %s"),
			    oid_ಗಮ್ಯ_hex(&options.orig_ತಲೆ->object.oid));
		strbuf_release(&ತಲೆ_ಸಂದೇಶ);
		remove_branch_state(the_repository, 0);
		ret = ಮುಕ್ತಾಯ_rebase(&options);
		ಹೋಗು cleanup;
	}
	ಸ್ಥಿತಿ ACTION_QUIT: {
		save_autostash(state_dir_ಮಾರ್ಗ("autostash", &options));
		ಆದರೆ (options.type == REBASE_ವಿಲೀನ) {
			ರಚನೆ replay_opts replay = REPLAY_OPTS_INIT;

			replay.action = REPLAY_INTERACTIVE_REBASE;
			ret = sequencer_remove_state(&replay);
			replay_opts_release(&replay);
		} ಇಲ್ಲದಿದ್ದರೆ {
			strbuf_reset(&buf);
			strbuf_addstr(&buf, options.state_dir);
			ret = remove_dir_recursively(&buf, 0);
			ಆದರೆ (ret)
				error(_("could not remove '%s'"),
				       options.state_dir);
		}
		ಹೋಗು cleanup;
	}
	ಸ್ಥಿತಿ ACTION_EDIT_TODO:
		options.dont_ಮುಕ್ತಾಯ_rebase = 1;
		ಹೋಗು ಸತತ_rebase;
	ಸ್ಥಿತಿ ACTION_SHOW_CURRENT_PATCH:
		options.dont_ಮುಕ್ತಾಯ_rebase = 1;
		ಹೋಗು ಸತತ_rebase;
	ಸ್ಥಿತಿ ACTION_NONE:
		ನಿಲ್ಲಿಸು;
	ಪೂರ್ವನಿಯೋಜಿತ:
		BUG("action: %d", options.action);
	}

	/* Make sure no rebase is in progress */
	ಆದರೆ (ಒಳ_ಪ್ರಗತಿ) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೊನೆಯ_slash = strrchr(options.state_dir, '/');
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state_dir_ತಳ =
			ಕೊನೆಯ_slash ? ಕೊನೆಯ_slash + 1 : options.state_dir;
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cmd_ಜೀವಂತ_rebase =
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
		    state_dir_ತಳ, cmd_ಜೀವಂತ_rebase, buf.buf);
	}

	ಆದರೆ ((options.flags & REBASE_INTERACTIVE_EXPLICIT) ||
	    (options.action != ACTION_NONE) ||
	    (options.exec.nr > 0) ||
	    options.autosquash == 1) {
		allow_preemptive_ff = 0;
	}
	ಆದರೆ (options.committer_date_is_author_date || options.ignore_date)
		options.flags |= REBASE_FORCE;

	ಗಾಗಿ (i = 0; i < options.git_am_opts.nr; i++) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *option = options.git_am_opts.v[i], *p;
		ಆದರೆ (!strcmp(option, "--whitespace=fix") ||
		    !strcmp(option, "--whitespace=strip"))
			allow_preemptive_ff = 0;
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(option, "-C", &p)) {
			ಆಗ (*p)
				ಆದರೆ (!ಅಂಕಿಯೇ(*(p++)))
					die(_("switch `C' expects a "
					      "numerical value"));
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(option, "--whitespace=", &p)) {
			ಆದರೆ (*p && strcmp(p, "warn") && strcmp(p, "nowarn") &&
			    strcmp(p, "error") && strcmp(p, "error-all"))
				die("Invalid whitespace option: '%s'", p);
		}
	}

	ಗಾಗಿ (i = 0; i < options.exec.nr; i++)
		ಆದರೆ (check_exec_cmd(options.exec.ವಸ್ತುಗಳು[i].string))
			ನಿರ್ಗಮಿಸು(1);

	ಆದರೆ (!(options.flags & REBASE_NO_QUIET))
		strvec_ತಳ್ಳು(&options.git_am_opts, "-q");

	ಆದರೆ (options.ಖಾಲಿಗಳು != ಖಾಲಿಗಳು_UNSPECIFIED)
		imply_ವಿಲೀನ(&options, "--empty");

	ಆದರೆ (options.reapply_cherry_picks < 0)
		/*
		 * We default to --no-reapply-cherry-picks unless
		 * --keep-base is given; when --keep-base is given, we want
		 * to default to --reapply-cherry-picks.
		 */
		options.reapply_cherry_picks = keep_ತಳ;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!keep_ತಳ)
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
		imply_ವಿಲೀನ(&options, options.reapply_cherry_picks ?
					  "--reapply-cherry-picks" :
					  "--no-reapply-cherry-picks");

	ಆದರೆ (gpg_sign)
		options.gpg_sign_opt = xstrfmt("-S%s", gpg_sign);

	ಆದರೆ (options.exec.nr)
		imply_ವಿಲೀನ(&options, "--exec");

	ಆದರೆ (options.type == REBASE_APPLY) {
		ಆದರೆ (ignore_whitespace)
			strvec_ತಳ್ಳು(&options.git_am_opts,
				    "--ignore-whitespace");
		ಆದರೆ (options.committer_date_is_author_date)
			strvec_ತಳ್ಳು(&options.git_am_opts,
				    "--committer-date-is-author-date");
		ಆದರೆ (options.ignore_date)
			strvec_ತಳ್ಳು(&options.git_am_opts, "--ignore-date");
	} ಇಲ್ಲದಿದ್ದರೆ {
		/* REBASE_MERGE */
		ಆದರೆ (ignore_whitespace) {
			string_list_append(&options.strategy_opts,
					   "ignore-space-change");
		}
	}

	ಆದರೆ (strategy_opt)
		options.strategy = xstrdup(strategy_opt);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (options.strategy_opts.nr && !options.strategy)
		options.strategy = xstrdup("ort");
	ಆದರೆ (options.strategy)
		imply_ವಿಲೀನ(&options, "--strategy");

	ಆದರೆ (options.ಬೇರು && !options.onto_name)
		imply_ವಿಲೀನ(&options, "--root without --onto");

	ಆದರೆ (options.trailer_ವಾದಗಳು.nr)
		imply_ವಿಲೀನ(&options, "--trailer");

	ಆದರೆ (isatty(2) && options.flags & REBASE_NO_QUIET)
		strbuf_addstr(&options.git_format_patch_opt, " --progress");

	ಆದರೆ (options.git_am_opts.nr || options.type == REBASE_APPLY) {
		/* all am options except -q are compatible only with --apply */
		ಗಾಗಿ (i = options.git_am_opts.nr - 1; i >= 0; i--)
			ಆದರೆ (strcmp(options.git_am_opts.v[i], "-q"))
				ನಿಲ್ಲಿಸು;

		ಆದರೆ (i >= 0 || options.type == REBASE_APPLY) {
			ಆದರೆ (is_ವಿಲೀನ(&options))
				die(_("apply options and merge options "
					  "cannot be used together"));
			ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (options.rebase_merges == -1 && options.config_rebase_merges == 1)
				die(_("apply options are incompatible with rebase.rebaseMerges.  Consider adding --no-rebase-merges"));
			ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (options.update_refs == -1 && options.config_update_refs == 1)
				die(_("apply options are incompatible with rebase.updateRefs.  Consider adding --no-update-refs"));
			ಇಲ್ಲದಿದ್ದರೆ
				options.type = REBASE_APPLY;
		}
	}

	ಆದರೆ (options.update_refs == 1)
		imply_ವಿಲೀನ(&options, "--update-refs");
	options.update_refs = (options.update_refs >= 0) ? options.update_refs :
			     ((options.config_update_refs >= 0) ? options.config_update_refs : 0);

	ಆದರೆ (options.rebase_merges == 1)
		imply_ವಿಲೀನ(&options, "--rebase-merges");
	options.rebase_merges = (options.rebase_merges >= 0) ? options.rebase_merges :
				((options.config_rebase_merges >= 0) ? options.config_rebase_merges : 0);

	ಆದರೆ (options.autosquash == 1) {
		imply_ವಿಲೀನ(&options, "--autosquash");
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (options.autosquash == -1) {
		options.autosquash =
			options.config_autosquash &&
			(options.flags & REBASE_INTERACTIVE_EXPLICIT);
	}

	ಆದರೆ (options.type == REBASE_UNSPECIFIED) {
		ಆದರೆ (!strcmp(options.default_backend, "merge"))
			options.type = REBASE_ವಿಲೀನ;
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(options.default_backend, "apply"))
			options.type = REBASE_APPLY;
		ಇಲ್ಲದಿದ್ದರೆ
			die(_("Unknown rebase backend: %s"),
			    options.default_backend);
	}

	ಆಯ್ಕೆ (options.type) {
	ಸ್ಥಿತಿ REBASE_ವಿಲೀನ:
		options.state_dir = ವಿಲೀನ_dir();
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ REBASE_APPLY:
		options.state_dir = apply_dir();
		ನಿಲ್ಲಿಸು;
	ಪೂರ್ವನಿಯೋಜಿತ:
		BUG("options.type was just set above; should be unreachable.");
	}

	ಆದರೆ (options.ಖಾಲಿಗಳು == ಖಾಲಿಗಳು_UNSPECIFIED) {
		ಆದರೆ (options.flags & REBASE_INTERACTIVE_EXPLICIT)
			options.ಖಾಲಿಗಳು = ಖಾಲಿಗಳು_STOP;
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (options.exec.nr > 0)
			options.ಖಾಲಿಗಳು = ಖಾಲಿಗಳು_KEEP;
		ಇಲ್ಲದಿದ್ದರೆ
			options.ಖಾಲಿಗಳು = ಖಾಲಿಗಳು_DROP;
	}
	ಆದರೆ (reschedule_failed_exec > 0 && !is_ವಿಲೀನ(&options))
		die(_("--reschedule-failed-exec requires "
		      "--exec or --interactive"));
	ಆದರೆ (reschedule_failed_exec >= 0)
		options.reschedule_failed_exec = reschedule_failed_exec;

	ಆದರೆ (options.signoff) {
		strvec_ತಳ್ಳು(&options.git_am_opts, "--signoff");
		options.flags |= REBASE_FORCE;
	}

	ಆದರೆ (!options.ಬೇರು) {
		ಆದರೆ (argc < 1) {
			ರಚನೆ branch *branch;

			branch = branch_get(ಖಾಲಿ);
			options.upstream_name = branch_get_upstream(branch,
								    ಖಾಲಿ);
			ಆದರೆ (!options.upstream_name)
				error_on_missing_default_upstream();
			ಆದರೆ (options.fork_ಬಿಂದು < 0)
				options.fork_ಬಿಂದು = 1;
		} ಇಲ್ಲದಿದ್ದರೆ {
			options.upstream_name = argv[0];
			argc--;
			argv++;
			ಆದರೆ (!strcmp(options.upstream_name, "-"))
				options.upstream_name = "@{-1}";
		}
		options.upstream =
			lookup_commit_reference_by_name(options.upstream_name);
		ಆದರೆ (!options.upstream)
			die(_("invalid upstream '%s'"), options.upstream_name);
		options.upstream_arg = options.upstream_name;
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಆದರೆ (!options.onto_name) {
			ಆದರೆ (commit_ವೃಕ್ಷ("", 0, the_hash_algo->ಖಾಲಿಗಳು_ವೃಕ್ಷ, ಖಾಲಿ,
					&squash_onto, ಖಾಲಿ, ಖಾಲಿ) < 0)
				die(_("Could not create new root commit"));
			options.squash_onto = &squash_onto;
			options.onto_name = squash_onto_name =
				xstrdup(oid_ಗಮ್ಯ_hex(&squash_onto));
		} ಇಲ್ಲದಿದ್ದರೆ
			options.ಬೇರು_with_onto = 1;

		options.upstream_name = ಖಾಲಿ;
		options.upstream = ಖಾಲಿ;
		ಆದರೆ (argc > 1)
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
	ಆದರೆ (argc == 1) {
		/* Is it "rebase other branchname" or "rebase other commit"? */
		ರಚನೆ object_id branch_oid;
		branch_name = argv[0];
		options.switch_ಗಮ್ಯ = argv[0];

		/* Is it a local branch? */
		strbuf_reset(&buf);
		strbuf_addf(&buf, "refs/heads/%s", branch_name);
		ಆದರೆ (!refs_read_ref(get_main_ref_store(the_repository), buf.buf, &branch_oid)) {
			die_if_checked_ಹೊರಹೋಗು(buf.buf, 1);
			options.ತಲೆ_name = xstrdup(buf.buf);
			options.orig_ತಲೆ =
				lookup_commit_object(the_repository,
						     &branch_oid);
		/* If not is it a valid ref (branch or commit)? */
		} ಇಲ್ಲದಿದ್ದರೆ {
			options.orig_ತಲೆ =
				lookup_commit_reference_by_name(branch_name);
			options.ತಲೆ_name = ಖಾಲಿ;
		}
		ಆದರೆ (!options.orig_ತಲೆ)
			die(_("no such branch/commit '%s'"), branch_name);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (argc == 0) {
		/* Do not need to switch branches, we are already on it. */
		options.ತಲೆ_name =
			xstrdup_or_null(refs_resolve_ref_unsafe(get_main_ref_store(the_repository), "HEAD", 0, ಖಾಲಿ,
								&flags));
		ಆದರೆ (!options.ತಲೆ_name)
			die(_("No such ref: %s"), "HEAD");
		ಆದರೆ (flags & REF_ISSYMREF) {
			ಆದರೆ (!skip_ಪೂರ್ವಪ್ರತ್ಯಯ(options.ತಲೆ_name,
					 "refs/heads/", &branch_name))
				branch_name = options.ತಲೆ_name;

		} ಇಲ್ಲದಿದ್ದರೆ {
			FREE_AND_NULL(options.ತಲೆ_name);
			branch_name = "HEAD";
		}
		options.orig_ತಲೆ = lookup_commit_reference_by_name("HEAD");
		ಆದರೆ (!options.orig_ತಲೆ)
			die(_("Could not resolve HEAD to a commit"));
	} ಇಲ್ಲದಿದ್ದರೆ
		BUG("unexpected number of arguments left to parse");

	/* Make sure the branch to rebase onto is valid. */
	ಆದರೆ (keep_ತಳ) {
		strbuf_reset(&buf);
		strbuf_addstr(&buf, options.upstream_name);
		strbuf_addstr(&buf, "...");
		strbuf_addstr(&buf, branch_name);
		options.onto_name = keep_ತಳ_onto_name = xstrdup(buf.buf);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!options.onto_name)
		options.onto_name = options.upstream_name;
	ಆದರೆ (ಅಕ್ಷರಸರಣಿ_ಹುಡುಕು(options.onto_name, "...")) {
		ಆದರೆ (repo_get_oid_mb(the_repository, options.onto_name, &branch_ತಳ) < 0) {
			ಆದರೆ (keep_ತಳ)
				die(_("'%s': need exactly one merge base with branch"),
				    options.upstream_name);
			ಇಲ್ಲದಿದ್ದರೆ
				die(_("'%s': need exactly one merge base"),
				    options.onto_name);
		}
		options.onto = lookup_commit_or_die(&branch_ತಳ,
						    options.onto_name);
	} ಇಲ್ಲದಿದ್ದರೆ {
		options.onto =
			lookup_commit_reference_by_name(options.onto_name);
		ಆದರೆ (!options.onto)
			die(_("Does not point to a valid commit '%s'"),
				options.onto_name);
		fill_branch_ತಳ(&options, &branch_ತಳ);
	}

	ಆದರೆ (keep_ತಳ && options.reapply_cherry_picks)
		options.upstream = options.onto;

	ಆದರೆ (options.fork_ಬಿಂದು > 0)
		options.restrict_revision =
			get_fork_ಬಿಂದು(options.upstream_name, options.orig_ತಲೆ);

	ಆದರೆ (repo_read_ಸೂಚ್ಯಂಕ(the_repository) < 0)
		die(_("could not read index"));

	ಆದರೆ (options.autostash)
		create_autostash(the_repository,
				 state_dir_ಮಾರ್ಗ("autostash", &options));


	ಆದರೆ (require_clean_work_ವೃಕ್ಷ(the_repository, "rebase",
				    _("Please commit or stash them."), 1, 1)) {
		ret = -1;
		ಹೋಗು cleanup_autostash;
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
	ಆದರೆ (allow_preemptive_ff &&
	    can_fast_forward(options.onto, options.upstream, options.restrict_revision,
			     options.orig_ತಲೆ, &branch_ತಳ)) {
		ಪೂರ್ಣಾಂಕ flag;

		ಆದರೆ (!(options.flags & REBASE_FORCE)) {
			/* Lazily switch to the target branch if needed... */
			ಆದರೆ (options.switch_ಗಮ್ಯ) {
				ret = checkout_ಮೇಲೆ_ಗಮ್ಯ_date(&options);
				ಆದರೆ (ret)
					ಹೋಗು cleanup_autostash;
			}

			ಆದರೆ (!(options.flags & REBASE_NO_QUIET))
				; /* be quiet */
			ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(branch_name, "HEAD") &&
				 refs_resolve_ref_unsafe(get_main_ref_store(the_repository), "HEAD", 0, ಖಾಲಿ, &flag))
				puts(_("HEAD is up to date."));
			ಇಲ್ಲದಿದ್ದರೆ
				ಬರೆ(_("Current branch %s is up to date.\n"),
				       branch_name);
			ret = ಮುಕ್ತಾಯ_rebase(&options);
			ಹೋಗು cleanup;
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!(options.flags & REBASE_NO_QUIET))
			; /* be quiet */
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(branch_name, "HEAD") &&
			 refs_resolve_ref_unsafe(get_main_ref_store(the_repository), "HEAD", 0, ಖಾಲಿ, &flag))
			puts(_("HEAD is up to date, rebase forced."));
		ಇಲ್ಲದಿದ್ದರೆ
			ಬರೆ(_("Current branch %s is up to date, rebase "
				 "forced.\n"), branch_name);
	}

	/* If a hook exists, give it a chance to interrupt*/
	ಆದರೆ (!ok_ಗಮ್ಯ_skip_pre_rebase &&
	    ಸತತ_hooks_l(the_repository, "pre-rebase", options.upstream_arg,
			argc ? argv[0] : ಖಾಲಿ, ಖಾಲಿ)) {
		ret = error(_("The pre-rebase hook refused to rebase."));
		ಹೋಗು cleanup_autostash;
	}

	ಆದರೆ (options.flags & REBASE_DIFFSTAT) {
		ರಚನೆ diff_options opts;

		ಆದರೆ (options.flags & REBASE_VERBOSE) {
			ಆದರೆ (is_null_oid(&branch_ತಳ))
				ಬರೆ(_("Changes to %s:\n"),
				       oid_ಗಮ್ಯ_hex(&options.onto->object.oid));
			ಇಲ್ಲದಿದ್ದರೆ
				ಬರೆ(_("Changes from %s to %s:\n"),
				       oid_ಗಮ್ಯ_hex(&branch_ತಳ),
				       oid_ಗಮ್ಯ_hex(&options.onto->object.oid));
		}

		/* We want color (if set), but no pager */
		repo_diff_ಸಿದ್ಧತೆ(the_repository, &opts);
		init_diffstat_widths(&opts);
		opts.output_format |=
			DIFF_FORMAT_SUMMARY | DIFF_FORMAT_DIFFSTAT;
		opts.detect_rename = DIFF_DETECT_RENAME;
		diff_ಸಿದ್ಧತೆ_ಮುಗಿದ(&opts);
		diff_ವೃಕ್ಷ_oid(is_null_oid(&branch_ತಳ) ?
			      the_hash_algo->ಖಾಲಿಗಳು_ವೃಕ್ಷ : &branch_ತಳ,
			      &options.onto->object.oid, "", &opts);
		diffcore_std(&opts);
		diff_flush(&opts);
	}

	ಆದರೆ (is_ವಿಲೀನ(&options))
		ಹೋಗು ಸತತ_rebase;

	/* Detach HEAD and reset the tree */
	ಆದರೆ (options.flags & REBASE_NO_QUIET)
		ಬರೆ(_("First, rewinding head to replay your work on top of "
			 "it...\n"));

	strbuf_addf(&ಸಂದೇಶ, "%s (start): checkout %s",
		    options.reflog_action, options.onto_name);
	ropts.oid = &options.onto->object.oid;
	ropts.orig_ತಲೆ = &options.orig_ತಲೆ->object.oid;
	ropts.flags = RESET_WORKING_ವೃಕ್ಷ_DETACH |
		      RESET_WORKING_ವೃಕ್ಷ_UPDATE_ತಲೆ |
		      RESET_WORKING_ವೃಕ್ಷ_UPDATE_ORIG_ತಲೆ |
		      RESET_WORKING_ವೃಕ್ಷ_ಸತತ_POST_CHECKOUT_HOOK;
	ropts.ತಲೆ_ಸಂದೇಶ = ಸಂದೇಶ.buf;
	ropts.default_reflog_action = options.reflog_action;
	ಆದರೆ (reset_working_ವೃಕ್ಷ(the_repository, &ropts)) {
		ret = error(_("Could not detach HEAD"));
		ಹೋಗು cleanup_autostash;
	}

	/*
	 * If the onto is a proper descendant of the tip of the branch, then
	 * we just fast-forwarded.
	 */
	ಆದರೆ (oideq(&branch_ತಳ, &options.orig_ತಲೆ->object.oid)) {
		ಬರೆ(_("Fast-forwarded %s to %s.\n"),
			branch_name, options.onto_name);
		move_ಗಮ್ಯ_original_branch(&options);
		ret = ಮುಕ್ತಾಯ_rebase(&options);
		ಹೋಗು cleanup;
	}

	strbuf_addf(&revisions, "%s..%s",
		    options.ಬೇರು ? oid_ಗಮ್ಯ_hex(&options.onto->object.oid) :
		    (options.restrict_revision ?
		     oid_ಗಮ್ಯ_hex(&options.restrict_revision->object.oid) :
		     oid_ಗಮ್ಯ_hex(&options.upstream->object.oid)),
		    oid_ಗಮ್ಯ_hex(&options.orig_ತಲೆ->object.oid));

	options.revisions = revisions.buf;

ಸತತ_rebase:
	ret = ಸತತ_specific_rebase(&options);

cleanup:
	strbuf_release(&buf);
	strbuf_release(&ಸಂದೇಶ);
	strbuf_release(&revisions);
	rebase_options_release(&options);
	ಬಿಡುಗಡೆ(squash_onto_name);
	ಬಿಡುಗಡೆ(keep_ತಳ_onto_name);
	ಹಿಂತಿರುಗಿ !!ret;

cleanup_autostash:
	ret |= !!cleanup_autostash(&options);
	ಹೋಗು cleanup;
}
