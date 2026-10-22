/*
 * "git rebase" builtin command
 *
 * Copyright (c) 2018 Pratik Karki
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#અન્તર્ભાવ "builtin.h"

#અન્તર્ભાવ "abspath.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "hex.h"
#અન્તર્ભાવ "run-command.h"
#અન્તર્ભાવ "strvec.h"
#અન્તર્ભાવ "dir.h"
#અન્તર્ભાવ "refs.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "unpack-trees.h"
#અન્તર્ભાવ "lockfile.h"
#અન્તર્ભાવ "object-file.h"
#અન્તર્ભાવ "object-name.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "path.h"
#અન્તર્ભાવ "commit.h"
#અન્તર્ભાવ "diff.h"
#અન્તર્ભાવ "wt-status.h"
#અન્તર્ભાવ "revision.h"
#અન્તર્ભાવ "commit-reach.h"
#અન્તર્ભાવ "rerere.h"
#અન્તર્ભાવ "branch.h"
#અન્તર્ભાવ "sequencer.h"
#અન્તર્ભાવ "rebase-interactive.h"
#અન્તર્ભાવ "reset.h"
#અન્તર્ભાવ "trace2.h"
#અન્તર્ભાવ "hook.h"
#અન્તર્ભાવ "trailer.h"

સ્થિર અક્ષર અચલ * અચલ builtin_rebase_usage[] = {
	N_("git rebase [-i] [options] [--exec <cmd>] "
		"[--onto <newbase> | --keep-base] [<upstream> [<branch>]]"),
	N_("git rebase [-i] [options] [--exec <cmd>] [--onto <newbase>] "
		"--root [<branch>]"),
	"git rebase --continue | --abort | --skip | --edit-todo",
	ખાલી
};

સ્થિર GIT_PATH_FUNC(path_squash_onto, "rebase-merge/squash-onto")
સ્થિર GIT_PATH_FUNC(path_interactive, "rebase-merge/interactive")
સ્થિર GIT_PATH_FUNC(apply_dir, "rebase-apply")
સ્થિર GIT_PATH_FUNC(merge_dir, "rebase-merge")

ગણના rebase_type {
	REBASE_UNSPECIFIED = -1,
	REBASE_APPLY,
	REBASE_MERGE
};

ગણના empty_type {
	EMPTY_UNSPECIFIED = -1,
	EMPTY_DROP,
	EMPTY_KEEP,
	EMPTY_STOP
};

ગણના action {
	ACTION_NONE = 0,
	ACTION_CONTINUE,
	ACTION_SKIP,
	ACTION_ABORT,
	ACTION_QUIT,
	ACTION_EDIT_TODO,
	ACTION_SHOW_CURRENT_PATCH
};

સ્થિર અચલ અક્ષર *action_names[] = {
	"undefined",
	"continue",
	"skip",
	"abort",
	"quit",
	"edit_todo",
	"show_current_patch"
};

સંરચના rebase_options {
	ગણના rebase_type type;
	ગણના empty_type empty;
	અક્ષર *default_backend;
	અચલ અક્ષર *state_dir;
	સંરચના commit *upstream;
	અચલ અક્ષર *upstream_name;
	અચલ અક્ષર *upstream_arg;
	અક્ષર *head_name;
	સંરચના commit *orig_head;
	સંરચના commit *onto;
	અચલ અક્ષર *onto_name;
	અચલ અક્ષર *revisions;
	અચલ અક્ષર *switch_to;
	પૂર્ણાંક મૂળ, મૂળ_with_onto;
	સંરચના object_id *squash_onto;
	સંરચના commit *restrict_revision;
	પૂર્ણાંક dont_finish_rebase;
	ગણના {
		REBASE_NO_QUIET = 1<<0,
		REBASE_VERBOSE = 1<<1,
		REBASE_DIFFSTAT = 1<<2,
		REBASE_FORCE = 1<<3,
		REBASE_INTERACTIVE_EXPLICIT = 1<<4,
	} flags;
	સંરચના strvec git_am_opts;
	ગણના action action;
	અક્ષર *reflog_action;
	પૂર્ણાંક signoff;
	સંરચના strvec trailer_args;
	પૂર્ણાંક allow_rerere_autoupdate;
	પૂર્ણાંક keep_empty;
	પૂર્ણાંક autosquash;
	અક્ષર *gpg_sign_opt;
	પૂર્ણાંક autostash;
	પૂર્ણાંક committer_date_is_author_date;
	પૂર્ણાંક ignore_date;
	સંરચના string_list exec;
	પૂર્ણાંક allow_empty_message;
	પૂર્ણાંક rebase_merges, rebase_cousins;
	અક્ષર *strategy;
	સંરચના string_list strategy_opts;
	સંરચના strbuf git_format_patch_opt;
	પૂર્ણાંક reschedule_failed_exec;
	પૂર્ણાંક reapply_cherry_picks;
	પૂર્ણાંક fork_point;
	પૂર્ણાંક update_refs;
	પૂર્ણાંક config_autosquash;
	પૂર્ણાંક config_rebase_merges;
	પૂર્ણાંક config_update_refs;
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

સ્થિર શૂન્ય rebase_options_release(સંરચના rebase_options *opts)
{
	મુક્ત(opts->default_backend);
	મુક્ત(opts->reflog_action);
	મુક્ત(opts->head_name);
	strvec_clear(&opts->git_am_opts);
	મુક્ત(opts->gpg_sign_opt);
	string_list_clear(&opts->exec, 0);
	મુક્ત(opts->strategy);
	string_list_clear(&opts->strategy_opts, 0);
	strbuf_release(&opts->git_format_patch_opt);
	strvec_clear(&opts->trailer_args);
}

સ્થિર સંરચના replay_opts get_replay_opts(અચલ સંરચના rebase_options *opts)
{
	સંરચના replay_opts replay = REPLAY_OPTS_INIT;

	replay.action = REPLAY_INTERACTIVE_REBASE;
	replay.strategy = ખાલી;
	sequencer_init_config(&replay);

	replay.signoff = opts->signoff;

	strvec_pushv(&replay.trailer_args, opts->trailer_args.v);

	replay.allow_ff = !(opts->flags & REBASE_FORCE);
	જો (opts->allow_rerere_autoupdate)
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
	મુક્ત(replay.gpg_sign);
	replay.gpg_sign = xstrdup_or_null(opts->gpg_sign_opt);
	replay.reflog_action = xstrdup(opts->reflog_action);
	જો (opts->strategy)
		replay.strategy = xstrdup_or_null(opts->strategy);
	નહીંતર જો (!replay.strategy && replay.default_strategy) {
		replay.strategy = replay.default_strategy;
		replay.default_strategy = ખાલી;
	}

	માટે (size_t i = 0; i < opts->strategy_opts.nr; i++)
		strvec_ધકેલો(&replay.xopts, opts->strategy_opts.વસ્તુઓ[i].string);

	જો (opts->squash_onto) {
		oidcpy(&replay.squash_onto, opts->squash_onto);
		replay.have_squash_onto = 1;
	}

	પાછા replay;
}

સ્થિર પૂર્ણાંક edit_todo_file(unsigned flags, સંરચના replay_opts *opts)
{
	અચલ અક્ષર *todo_file = rebase_path_todo();
	સંરચના todo_list todo_list = TODO_LIST_INIT,
		new_todo = TODO_LIST_INIT;
	પૂર્ણાંક res = 0;

	જો (strbuf_read_file(&todo_list.buf, todo_file, 0) < 0)
		પાછા error_errno(_("could not read '%s'."), todo_file);

	strbuf_stripspace(&todo_list.buf, comment_લીટી_str);
	res = edit_todo_list(the_repository, opts, &todo_list, &new_todo,
			     ખાલી, ખાલી, flags);
	જો (!res && todo_list_write_to_file(the_repository, &new_todo, todo_file,
					    ખાલી, ખાલી, -1, flags & ~(TODO_LIST_SHORTEN_IDS)))
		res = error_errno(_("could not write '%s'"), todo_file);

	todo_list_release(&todo_list);
	todo_list_release(&new_todo);

	પાછા res;
}

સ્થિર પૂર્ણાંક get_revision_ranges(સંરચના commit *upstream, સંરચના commit *onto,
			       સંરચના object_id *orig_head, અક્ષર **revisions,
			       અક્ષર **shortrevisions)
{
	સંરચના commit *base_rev = upstream ? upstream : onto;
	અચલ અક્ષર *shorthead;

	*revisions = xstrfmt("%s...%s", oid_to_hex(&base_rev->object.oid),
			     oid_to_hex(orig_head));

	shorthead = repo_find_unique_abbrev(the_repository, orig_head,
					    DEFAULT_ABBREV);

	જો (upstream) {
		અચલ અક્ષર *shortrev;

		shortrev = repo_find_unique_abbrev(the_repository,
						   &base_rev->object.oid,
						   DEFAULT_ABBREV);

		*shortrevisions = xstrfmt("%s..%s", shortrev, shorthead);
	} નહીંતર
		*shortrevisions = xstrdup(shorthead);

	પાછા 0;
}

સ્થિર પૂર્ણાંક init_basic_state(સંરચના replay_opts *opts, અચલ અક્ષર *head_name,
			    સંરચના commit *onto,
			    અચલ સંરચના object_id *orig_head)
{
	FILE *interactive;

	જો (!is_directory(merge_dir()) &&
	    safe_create_dir_in_gitdir(the_repository, merge_dir()))
		પાછા error_errno(_("could not create temporary %s"), merge_dir());

	refs_delete_reflog(get_main_ref_store(the_repository), "REBASE_HEAD");

	interactive = fopen(path_interactive(), "w");
	જો (!interactive)
		પાછા error_errno(_("could not mark as interactive"));
	fclose(interactive);

	પાછા write_basic_state(opts, head_name, onto, orig_head);
}

સ્થિર પૂર્ણાંક do_interactive_rebase(સંરચના rebase_options *opts, unsigned flags)
{
	પૂર્ણાંક ret = -1;
	અક્ષર *revisions = ખાલી, *shortrevisions = ખાલી;
	સંરચના strvec make_script_args = STRVEC_INIT;
	સંરચના todo_list todo_list = TODO_LIST_INIT;
	સંરચના replay_opts replay = get_replay_opts(opts);

	જો (get_revision_ranges(opts->upstream, opts->onto, &opts->orig_head->object.oid,
				&revisions, &shortrevisions))
		જાઓ cleanup;

	strvec_pushl(&make_script_args, "", revisions, ખાલી);
	જો (opts->restrict_revision)
		strvec_pushf(&make_script_args, "^%s",
			     oid_to_hex(&opts->restrict_revision->object.oid));

	ret = sequencer_make_script(the_repository, &todo_list.buf,
				    &make_script_args, flags);
	જો (ret) {
		error(_("could not generate todo list"));
		જાઓ cleanup;
	}

	જો (init_basic_state(&replay,
			     opts->head_name ? opts->head_name : "detached HEAD",
			     opts->onto, &opts->orig_head->object.oid))
		જાઓ cleanup;

	જો (!opts->upstream && opts->squash_onto)
		write_file(path_squash_onto(), "%s\n",
			   oid_to_hex(opts->squash_onto));

	discard_index(the_repository->index);
	જો (todo_list_parse_insn_buffer(the_repository, &replay,
					todo_list.buf.buf, &todo_list))
		BUG("unusable todo list");

	ret = complete_action(the_repository, &replay, flags,
		shortrevisions, opts->onto_name, opts->onto,
		&opts->orig_head->object.oid, &opts->exec,
		opts->autosquash, opts->update_refs, &todo_list);

cleanup:
	replay_opts_release(&replay);
	મુક્ત(revisions);
	મુક્ત(shortrevisions);
	todo_list_release(&todo_list);
	strvec_clear(&make_script_args);

	પાછા ret;
}

સ્થિર પૂર્ણાંક run_sequencer_rebase(સંરચના rebase_options *opts)
{
	unsigned flags = 0;
	પૂર્ણાંક abbreviate_commands = 0, ret = 0;

	repo_config_get_bool(the_repository, "rebase.abbreviatecommands", &abbreviate_commands);

	flags |= opts->keep_empty ? TODO_LIST_KEEP_EMPTY : 0;
	flags |= abbreviate_commands ? TODO_LIST_ABBREVIATE_CMDS : 0;
	flags |= opts->rebase_merges ? TODO_LIST_REBASE_MERGES : 0;
	flags |= opts->rebase_cousins > 0 ? TODO_LIST_REBASE_COUSINS : 0;
	flags |= opts->મૂળ_with_onto ? TODO_LIST_મૂળ_WITH_ONTO : 0;
	flags |= opts->reapply_cherry_picks ? TODO_LIST_REAPPLY_CHERRY_PICKS : 0;
	flags |= opts->flags & REBASE_NO_QUIET ? TODO_LIST_WARN_SKIPPED_CHERRY_PICKS : 0;

	પસંદગી (opts->action) {
	સ્થિતિ ACTION_NONE: {
		જો (!opts->onto && !opts->upstream)
			die(_("a base commit must be provided with --upstream or --onto"));

		ret = do_interactive_rebase(opts, flags);
		તોડો;
	}
	સ્થિતિ ACTION_SKIP: {
		સંરચના string_list merge_rr = STRING_LIST_INIT_DUP;

		rerere_clear(the_repository, &merge_rr);
	}
		/* fallthrough */
	સ્થિતિ ACTION_CONTINUE: {
		સંરચના replay_opts replay_opts = get_replay_opts(opts);

		ret = sequencer_continue(the_repository, &replay_opts);
		replay_opts_release(&replay_opts);
		તોડો;
	}
	સ્થિતિ ACTION_EDIT_TODO: {
		સંરચના replay_opts replay_opts = get_replay_opts(opts);

		ret = edit_todo_file(flags, &replay_opts);
		replay_opts_release(&replay_opts);
		તોડો;
	}
	સ્થિતિ ACTION_SHOW_CURRENT_PATCH: {
		સંરચના child_process cmd = CHILD_PROCESS_INIT;

		cmd.git_cmd = 1;
		strvec_pushl(&cmd.args, "show", "REBASE_HEAD", "--", ખાલી);
		ret = run_command(&cmd);

		તોડો;
	}
	મૂળભૂત:
		BUG("invalid command '%d'", opts->action);
	}

	પાછા ret;
}

સ્થિર પૂર્ણાંક is_merge(સંરચના rebase_options *opts)
{
	પાછા opts->type == REBASE_MERGE;
}

સ્થિર શૂન્ય imply_merge(સંરચના rebase_options *opts, અચલ અક્ષર *option)
{
	પસંદગી (opts->type) {
	સ્થિતિ REBASE_APPLY:
		die(_("%s requires the merge backend"), option);
		તોડો;
	સ્થિતિ REBASE_MERGE:
		તોડો;
	મૂળભૂત:
		opts->type = REBASE_MERGE; /* implied */
		તોડો;
	}
}

/* Returns the filename prefixed by the state_dir */
સ્થિર અચલ અક્ષર *state_dir_path(અચલ અક્ષર *filename, સંરચના rebase_options *opts)
{
	સ્થિર સંરચના strbuf path = STRBUF_INIT;
	સ્થિર size_t prefix_len;

	જો (!prefix_len) {
		strbuf_addf(&path, "%s/", opts->state_dir);
		prefix_len = path.len;
	}

	strbuf_setlen(&path, prefix_len);
	strbuf_addstr(&path, filename);
	પાછા path.buf;
}

/* Initialize the rebase options from the state directory. */
સ્થિર પૂર્ણાંક read_basic_state(સંરચના rebase_options *opts)
{
	સંરચના strbuf head_name = STRBUF_INIT;
	સંરચના strbuf buf = STRBUF_INIT;
	સંરચના object_id oid;

	જો (!read_oneliner(&head_name, state_dir_path("head-name", opts),
			   READ_ONELINER_WARN_MISSING) ||
	    !read_oneliner(&buf, state_dir_path("onto", opts),
			   READ_ONELINER_WARN_MISSING))
		પાછા -1;
	opts->head_name = starts_with(head_name.buf, "refs/") ?
		xstrdup(head_name.buf) : ખાલી;
	strbuf_release(&head_name);
	જો (get_oid_hex(buf.buf, &oid) ||
	    !(opts->onto = lookup_commit_object(the_repository, &oid)))
		પાછા error(_("invalid onto: '%s'"), buf.buf);

	/*
	 * We always write to orig-head, but interactive rebase used to write to
	 * head. Fall back to reading from head to cover for the case that the
	 * user upgraded git with an ongoing interactive rebase.
	 */
	strbuf_reset(&buf);
	જો (file_exists(state_dir_path("orig-head", opts))) {
		જો (!read_oneliner(&buf, state_dir_path("orig-head", opts),
				   READ_ONELINER_WARN_MISSING))
			પાછા -1;
	} નહીંતર જો (!read_oneliner(&buf, state_dir_path("head", opts),
				  READ_ONELINER_WARN_MISSING))
		પાછા -1;
	જો (get_oid_hex(buf.buf, &oid) ||
	    !(opts->orig_head = lookup_commit_object(the_repository, &oid)))
		પાછા error(_("invalid orig-head: '%s'"), buf.buf);

	જો (file_exists(state_dir_path("quiet", opts)))
		opts->flags &= ~REBASE_NO_QUIET;
	નહીંતર
		opts->flags |= REBASE_NO_QUIET;

	જો (file_exists(state_dir_path("verbose", opts)))
		opts->flags |= REBASE_VERBOSE;

	જો (file_exists(state_dir_path("signoff", opts))) {
		opts->signoff = 1;
		opts->flags |= REBASE_FORCE;
	}

	જો (file_exists(state_dir_path("allow_rerere_autoupdate", opts))) {
		strbuf_reset(&buf);
		જો (!read_oneliner(&buf, state_dir_path("allow_rerere_autoupdate", opts),
				   READ_ONELINER_WARN_MISSING))
			પાછા -1;
		જો (!strcmp(buf.buf, "--rerere-autoupdate"))
			opts->allow_rerere_autoupdate = RERERE_AUTOUPDATE;
		નહીંતર જો (!strcmp(buf.buf, "--no-rerere-autoupdate"))
			opts->allow_rerere_autoupdate = RERERE_NOAUTOUPDATE;
		નહીંતર
			warning(_("ignoring invalid allow_rerere_autoupdate: "
				  "'%s'"), buf.buf);
	}

	જો (file_exists(state_dir_path("gpg_sign_opt", opts))) {
		strbuf_reset(&buf);
		જો (!read_oneliner(&buf, state_dir_path("gpg_sign_opt", opts),
				   READ_ONELINER_WARN_MISSING))
			પાછા -1;
		મુક્ત(opts->gpg_sign_opt);
		opts->gpg_sign_opt = xstrdup(buf.buf);
	}

	strbuf_release(&buf);

	પાછા 0;
}

સ્થિર પૂર્ણાંક rebase_write_basic_state(સંરચના rebase_options *opts)
{
	write_file(state_dir_path("head-name", opts), "%s",
		   opts->head_name ? opts->head_name : "detached HEAD");
	write_file(state_dir_path("onto", opts), "%s",
		   opts->onto ? oid_to_hex(&opts->onto->object.oid) : "");
	write_file(state_dir_path("orig-head", opts), "%s",
		   oid_to_hex(&opts->orig_head->object.oid));
	જો (!(opts->flags & REBASE_NO_QUIET))
		write_file(state_dir_path("quiet", opts), "%s", "");
	જો (opts->flags & REBASE_VERBOSE)
		write_file(state_dir_path("verbose", opts), "%s", "");
	જો (opts->allow_rerere_autoupdate > 0)
		write_file(state_dir_path("allow_rerere_autoupdate", opts),
			   "-%s-rerere-autoupdate",
			   opts->allow_rerere_autoupdate == RERERE_AUTOUPDATE ?
				"" : "-no");
	જો (opts->gpg_sign_opt)
		write_file(state_dir_path("gpg_sign_opt", opts), "%s",
			   opts->gpg_sign_opt);
	જો (opts->signoff)
		write_file(state_dir_path("signoff", opts), "--signoff");

	પાછા 0;
}

સ્થિર પૂર્ણાંક cleanup_autostash(સંરચના rebase_options *opts)
{
	પૂર્ણાંક ret;
	સંરચના strbuf dir = STRBUF_INIT;
	અચલ અક્ષર *path = state_dir_path("autostash", opts);

	જો (!file_exists(path))
		પાછા 0;
	ret = apply_autostash(path);
	strbuf_addstr(&dir, opts->state_dir);
	જો (remove_dir_recursively(&dir, 0))
		ret = error_errno(_("could not remove '%s'"), opts->state_dir);
	strbuf_release(&dir);

	પાછા ret;
}

સ્થિર પૂર્ણાંક finish_rebase(સંરચના rebase_options *opts)
{
	સંરચના strbuf dir = STRBUF_INIT;
	પૂર્ણાંક ret = 0;

	refs_delete_ref(get_main_ref_store(the_repository), ખાલી,
			"REBASE_HEAD", ખાલી, REF_NO_DEREF);
	refs_delete_ref(get_main_ref_store(the_repository), ખાલી,
			"AUTO_MERGE", ખાલી, REF_NO_DEREF);
	apply_autostash(state_dir_path("autostash", opts));
	/*
	 * We ignore errors in 'git maintenance run --auto', since the
	 * user should see them.
	 */
	run_auto_maintenance(the_repository,
			!(opts->flags & (REBASE_NO_QUIET|REBASE_VERBOSE)));

	જો (opts->type == REBASE_MERGE) {
		સંરચના replay_opts replay = REPLAY_OPTS_INIT;

		replay.action = REPLAY_INTERACTIVE_REBASE;
		ret = sequencer_remove_state(&replay);
		replay_opts_release(&replay);
	} નહીંતર {
		strbuf_addstr(&dir, opts->state_dir);
		જો (remove_dir_recursively(&dir, 0))
			ret = error(_("could not remove '%s'"),
				    opts->state_dir);
		strbuf_release(&dir);
	}

	પાછા ret;
}

સ્થિર પૂર્ણાંક move_to_original_branch(સંરચના rebase_options *opts)
{
	સંરચના strbuf branch_reflog = STRBUF_INIT, head_reflog = STRBUF_INIT;
	સંરચના reset_working_વૃક્ષ_options ropts = { 0 };
	પૂર્ણાંક ret;

	જો (!opts->head_name)
		પાછા 0; /* nothing to move back to */

	જો (!opts->onto)
		BUG("move_to_original_branch without onto");

	strbuf_addf(&branch_reflog, "%s (finish): %s onto %s",
		    opts->reflog_action,
		    opts->head_name, oid_to_hex(&opts->onto->object.oid));
	strbuf_addf(&head_reflog, "%s (finish): returning to %s",
		    opts->reflog_action, opts->head_name);
	ropts.branch = opts->head_name;
	ropts.flags = RESET_WORKING_વૃક્ષ_REFS_ONLY |
		      RESET_WORKING_વૃક્ષ_UPDATE_HEAD;
	ropts.branch_સંદેશ = branch_reflog.buf;
	ropts.head_સંદેશ = head_reflog.buf;
	ret = reset_working_વૃક્ષ(the_repository, &ropts);

	strbuf_release(&branch_reflog);
	strbuf_release(&head_reflog);
	પાછા ret;
}

સ્થિર પૂર્ણાંક run_am(સંરચના rebase_options *opts)
{
	સંરચના child_process am = CHILD_PROCESS_INIT;
	સંરચના child_process format_patch = CHILD_PROCESS_INIT;
	પૂર્ણાંક status;
	અક્ષર *rebased_patches;

	am.git_cmd = 1;
	strvec_ધકેલો(&am.args, "am");
	strvec_pushf(&am.env, GIT_REFLOG_ACTION_ENVIRONMENT "=%s (pick)",
		     opts->reflog_action);
	જો (opts->action == ACTION_CONTINUE) {
		strvec_ધકેલો(&am.args, "--resolved");
		strvec_pushf(&am.args, "--resolvemsg=%s", rebase_resolvemsg);
		જો (opts->gpg_sign_opt)
			strvec_ધકેલો(&am.args, opts->gpg_sign_opt);
		status = run_command(&am);
		જો (status)
			પાછા status;

		પાછા move_to_original_branch(opts);
	}
	જો (opts->action == ACTION_SKIP) {
		strvec_ધકેલો(&am.args, "--skip");
		strvec_pushf(&am.args, "--resolvemsg=%s", rebase_resolvemsg);
		status = run_command(&am);
		જો (status)
			પાછા status;

		પાછા move_to_original_branch(opts);
	}
	જો (opts->action == ACTION_SHOW_CURRENT_PATCH) {
		strvec_ધકેલો(&am.args, "--show-current-patch");
		પાછા run_command(&am);
	}

	rebased_patches = repo_git_path(the_repository, "rebased-patches");
	format_patch.out = open(rebased_patches,
				O_WRONLY | O_CREAT | O_TRUNC, 0666);
	જો (format_patch.out < 0) {
		status = error_errno(_("could not open '%s' for writing"),
				     rebased_patches);
		મુક્ત(rebased_patches);
		child_process_clear(&am);
		પાછા status;
	}

	format_patch.git_cmd = 1;
	strvec_pushl(&format_patch.args, "format-patch", "-k", "--stdout",
		     "--full-index", "--cherry-pick", "--right-only",
		     "--default-prefix", "--no-renames",
		     "--no-cover-letter", "--pretty=mboxrd", "--topo-order",
		     "--no-base", ખાલી);
	જો (opts->git_format_patch_opt.len)
		strvec_split(&format_patch.args,
			     opts->git_format_patch_opt.buf);
	strvec_pushf(&format_patch.args, "%s...%s",
		     oid_to_hex(opts->મૂળ ?
				/* this is now equivalent to !opts->upstream */
				&opts->onto->object.oid :
				&opts->upstream->object.oid),
		     oid_to_hex(&opts->orig_head->object.oid));
	જો (opts->restrict_revision)
		strvec_pushf(&format_patch.args, "^%s",
			     oid_to_hex(&opts->restrict_revision->object.oid));

	status = run_command(&format_patch);
	જો (status) {
		સંરચના reset_working_વૃક્ષ_options ropts = { 0 };
		unlink(rebased_patches);
		મુક્ત(rebased_patches);
		child_process_clear(&am);

		ropts.oid = &opts->orig_head->object.oid;
		ropts.branch = opts->head_name;
		ropts.default_reflog_action = opts->reflog_action;
		ropts.flags = RESET_WORKING_વૃક્ષ_UPDATE_HEAD;
		reset_working_વૃક્ષ(the_repository, &ropts);
		error(_("\ngit encountered an error while preparing the "
			"patches to replay\n"
			"these revisions:\n"
			"\n    %s\n\n"
			"As a result, git cannot rebase them."),
		      opts->revisions);

		પાછા status;
	}

	am.in = open(rebased_patches, O_RDONLY);
	જો (am.in < 0) {
		status = error_errno(_("could not open '%s' for reading"),
				     rebased_patches);
		મુક્ત(rebased_patches);
		child_process_clear(&am);
		પાછા status;
	}

	strvec_pushv(&am.args, opts->git_am_opts.v);
	strvec_ધકેલો(&am.args, "--rebasing");
	strvec_pushf(&am.args, "--resolvemsg=%s", rebase_resolvemsg);
	strvec_ધકેલો(&am.args, "--patch-format=mboxrd");
	જો (opts->allow_rerere_autoupdate == RERERE_AUTOUPDATE)
		strvec_ધકેલો(&am.args, "--rerere-autoupdate");
	નહીંતર જો (opts->allow_rerere_autoupdate == RERERE_NOAUTOUPDATE)
		strvec_ધકેલો(&am.args, "--no-rerere-autoupdate");
	જો (opts->gpg_sign_opt)
		strvec_ધકેલો(&am.args, opts->gpg_sign_opt);
	status = run_command(&am);
	unlink(rebased_patches);
	મુક્ત(rebased_patches);

	જો (!status) {
		પાછા move_to_original_branch(opts);
	}

	જો (is_directory(opts->state_dir))
		rebase_write_basic_state(opts);

	પાછા status;
}

સ્થિર પૂર્ણાંક run_specific_rebase(સંરચના rebase_options *opts)
{
	પૂર્ણાંક status;

	જો (opts->type == REBASE_MERGE) {
		/* Run sequencer-based rebase */
		જો (!(opts->flags & REBASE_INTERACTIVE_EXPLICIT))
			setenv("GIT_SEQUENCE_EDITOR", ":", 1);
		જો (opts->gpg_sign_opt) {
			/* remove the leading "-S" */
			અક્ષર *tmp = xstrdup(opts->gpg_sign_opt + 2);
			મુક્ત(opts->gpg_sign_opt);
			opts->gpg_sign_opt = tmp;
		}

		status = run_sequencer_rebase(opts);
	} નહીંતર જો (opts->type == REBASE_APPLY)
		status = run_am(opts);
	નહીંતર
		BUG("Unhandled rebase type %d", opts->type);

	જો (opts->dont_finish_rebase)
		; /* do nothing */
	નહીંતર જો (opts->type == REBASE_MERGE) {
		પૂર્ણાંક quiet = !(opts->flags & (REBASE_NO_QUIET|REBASE_VERBOSE));

		/*
		 * The sequencer cleans up after itself. Its state directory
		 * is gone once it is done, and stays while it is stopped.
		 */
		જો (status == 0 && !is_directory(opts->state_dir))
			run_auto_maintenance(the_repository, quiet);
	} નહીંતર જો (status == 0) {
		જો (!file_exists(state_dir_path("stopped-sha", opts)))
			finish_rebase(opts);
	} નહીંતર જો (status == 2) {
		સંરચના strbuf dir = STRBUF_INIT;

		apply_autostash(state_dir_path("autostash", opts));
		strbuf_addstr(&dir, opts->state_dir);
		remove_dir_recursively(&dir, 0);
		strbuf_release(&dir);
		die("Nothing to do");
	}

	પાછા status ? -1 : 0;
}

સ્થિર શૂન્ય parse_rebase_merges_કિંમત(સંરચના rebase_options *options, અચલ અક્ષર *કિંમત)
{
	જો (!strcmp("no-rebase-cousins", કિંમત))
		options->rebase_cousins = 0;
	નહીંતર જો (!strcmp("rebase-cousins", કિંમત))
		options->rebase_cousins = 1;
	નહીંતર
		die(_("Unknown rebase-merges mode: %s"), કિંમત);
}

સ્થિર પૂર્ણાંક rebase_config(અચલ અક્ષર *var, અચલ અક્ષર *કિંમત,
			 અચલ સંરચના config_context *ctx, શૂન્ય *data)
{
	સંરચના rebase_options *opts = data;

	જો (!strcmp(var, "rebase.stat")) {
		જો (git_config_bool(var, કિંમત))
			opts->flags |= REBASE_DIFFSTAT;
		નહીંતર
			opts->flags &= ~REBASE_DIFFSTAT;
		પાછા 0;
	}

	જો (!strcmp(var, "rebase.autosquash")) {
		opts->config_autosquash = git_config_bool(var, કિંમત);
		પાછા 0;
	}

	જો (!strcmp(var, "commit.gpgsign")) {
		મુક્ત(opts->gpg_sign_opt);
		opts->gpg_sign_opt = git_config_bool(var, કિંમત) ?
			xstrdup("-S") : ખાલી;
		પાછા 0;
	}

	જો (!strcmp(var, "rebase.autostash")) {
		opts->autostash = git_config_bool(var, કિંમત);
		પાછા 0;
	}

	જો (!strcmp(var, "rebase.rebasemerges")) {
		opts->config_rebase_merges = git_parse_maybe_bool(કિંમત);
		જો (opts->config_rebase_merges < 0) {
			opts->config_rebase_merges = 1;
			parse_rebase_merges_કિંમત(opts, કિંમત);
		} નહીંતર {
			opts->rebase_cousins = 0;
		}
		પાછા 0;
	}

	જો (!strcmp(var, "rebase.updaterefs")) {
		opts->config_update_refs = git_config_bool(var, કિંમત);
		પાછા 0;
	}

	જો (!strcmp(var, "rebase.reschedulefailedexec")) {
		opts->reschedule_failed_exec = git_config_bool(var, કિંમત);
		પાછા 0;
	}

	જો (!strcmp(var, "rebase.forkpoint")) {
		opts->fork_point = git_config_bool(var, કિંમત) ? -1 : 0;
		પાછા 0;
	}

	જો (!strcmp(var, "rebase.backend")) {
		FREE_AND_NULL(opts->default_backend);
		પાછા git_config_string(&opts->default_backend, var, કિંમત);
	}

	પાછા git_default_config(var, કિંમત, ctx, data);
}

સ્થિર પૂર્ણાંક checkout_up_to_date(સંરચના rebase_options *options)
{
	સંરચના strbuf buf = STRBUF_INIT;
	સંરચના reset_working_વૃક્ષ_options ropts = { 0 };
	પૂર્ણાંક ret = 0;

	strbuf_addf(&buf, "%s: checkout %s",
		    options->reflog_action, options->switch_to);
	ropts.oid = &options->orig_head->object.oid;
	ropts.branch = options->head_name;
	ropts.flags = RESET_WORKING_વૃક્ષ_RUN_POST_CHECKOUT_HOOK |
		      RESET_WORKING_વૃક્ષ_UPDATE_HEAD;
	જો (!ropts.branch)
		ropts.flags |=  RESET_WORKING_વૃક્ષ_DETACH;
	ropts.head_સંદેશ = buf.buf;
	જો (reset_working_વૃક્ષ(the_repository, &ropts) < 0)
		ret = error(_("could not switch to %s"), options->switch_to);
	strbuf_release(&buf);

	પાછા ret;
}

/*
 * Determines whether the commits in from..to are linear, i.e. contain
 * no merge commits. This function *expects* `from` to be an ancestor of
 * `to`.
 */
સ્થિર પૂર્ણાંક is_linear_history(સંરચના commit *from, સંરચના commit *to)
{
	જ્યારે (to && to != from) {
		repo_parse_commit(the_repository, to);
		જો (!to->parents)
			પાછા 1;
		જો (to->parents->next)
			પાછા 0;
		to = to->parents->item;
	}
	પાછા 1;
}

સ્થિર પૂર્ણાંક can_fast_forward(સંરચના commit *onto, સંરચના commit *upstream,
			    સંરચના commit *restrict_revision,
			    સંરચના commit *head, સંરચના object_id *branch_base)
{
	સંરચના commit_list *merge_bases = ખાલી;
	પૂર્ણાંક res = 0;

	જો (is_null_oid(branch_base))
		જાઓ done; /* fill_branch_base() found multiple merge bases */

	જો (!oideq(branch_base, &onto->object.oid))
		જાઓ done;

	જો (restrict_revision && !oideq(&restrict_revision->object.oid, branch_base))
		જાઓ done;

	જો (!upstream)
		જાઓ done;

	જો (repo_get_merge_bases(the_repository, upstream, head, &merge_bases) < 0)
		exit(128);
	જો (!merge_bases || merge_bases->next)
		જાઓ done;

	જો (!oideq(&onto->object.oid, &merge_bases->item->object.oid))
		જાઓ done;

	res = 1;

done:
	commit_list_free(merge_bases);
	પાછા res && is_linear_history(onto, head);
}

સ્થિર શૂન્ય fill_branch_base(સંરચના rebase_options *options,
			    સંરચના object_id *branch_base)
{
	સંરચના commit_list *merge_bases = ખાલી;

	જો (repo_get_merge_bases(the_repository, options->onto,
				 options->orig_head, &merge_bases) < 0)
		exit(128);
	જો (!merge_bases || merge_bases->next)
		oidcpy(branch_base, null_oid(the_hash_algo));
	નહીંતર
		oidcpy(branch_base, &merge_bases->item->object.oid);

	commit_list_free(merge_bases);
}

સ્થિર પૂર્ણાંક parse_opt_am(અચલ સંરચના option *opt, અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	સંરચના rebase_options *opts = opt->કિંમત;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	જો (opts->type != REBASE_UNSPECIFIED && opts->type != REBASE_APPLY)
	    die(_("apply options and merge options cannot be used together"));

	opts->type = REBASE_APPLY;

	પાછા 0;
}

/* -i followed by -m is still -i */
સ્થિર પૂર્ણાંક parse_opt_merge(અચલ સંરચના option *opt, અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	સંરચના rebase_options *opts = opt->કિંમત;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	જો (opts->type != REBASE_UNSPECIFIED && opts->type != REBASE_MERGE)
	    die(_("apply options and merge options cannot be used together"));

	opts->type = REBASE_MERGE;

	પાછા 0;
}

/* -i followed by -r is still explicitly interactive, but -r alone is not */
સ્થિર પૂર્ણાંક parse_opt_interactive(અચલ સંરચના option *opt, અચલ અક્ષર *arg,
				 પૂર્ણાંક unset)
{
	સંરચના rebase_options *opts = opt->કિંમત;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	જો (opts->type != REBASE_UNSPECIFIED && opts->type != REBASE_MERGE)
	    die(_("apply options and merge options cannot be used together"));

	opts->type = REBASE_MERGE;
	opts->flags |= REBASE_INTERACTIVE_EXPLICIT;

	પાછા 0;
}

સ્થિર ગણના empty_type parse_empty_કિંમત(અચલ અક્ષર *કિંમત)
{
	જો (!strcasecmp(કિંમત, "drop"))
		પાછા EMPTY_DROP;
	નહીંતર જો (!strcasecmp(કિંમત, "keep"))
		પાછા EMPTY_KEEP;
	નહીંતર જો (!strcasecmp(કિંમત, "stop"))
		પાછા EMPTY_STOP;
	નહીંતર જો (!strcasecmp(કિંમત, "ask")) {
		warning(_("--empty=ask is deprecated; use '--empty=stop' instead."));
		પાછા EMPTY_STOP;
	}

	die(_("unrecognized empty type '%s'; valid values are \"drop\", \"keep\", and \"stop\"."), કિંમત);
}

સ્થિર પૂર્ણાંક parse_opt_keep_empty(અચલ સંરચના option *opt, અચલ અક્ષર *arg,
				પૂર્ણાંક unset)
{
	સંરચના rebase_options *opts = opt->કિંમત;

	BUG_ON_OPT_ARG(arg);

	imply_merge(opts, unset ? "--no-keep-empty" : "--keep-empty");
	opts->keep_empty = !unset;
	પાછા 0;
}

સ્થિર પૂર્ણાંક parse_opt_empty(અચલ સંરચના option *opt, અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	સંરચના rebase_options *options = opt->કિંમત;
	ગણના empty_type કિંમત = parse_empty_કિંમત(arg);

	BUG_ON_OPT_NEG(unset);

	options->empty = કિંમત;
	પાછા 0;
}

સ્થિર પૂર્ણાંક parse_opt_rebase_merges(અચલ સંરચના option *opt, અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	સંરચના rebase_options *options = opt->કિંમત;

	options->rebase_merges = !unset;
	options->rebase_cousins = 0;

	જો (arg) {
		જો (!*arg) {
			warning(_("--rebase-merges with an empty string "
				  "argument is deprecated and will stop "
				  "working in a future version of Git. Use "
				  "--rebase-merges without an argument "
				  "instead, which does the same thing."));
			પાછા 0;
		}
		parse_rebase_merges_કિંમત(options, arg);
	}

	પાછા 0;
}

સ્થિર શૂન્ય NORETURN error_on_missing_default_upstream(શૂન્ય)
{
	સંરચના branch *current_branch = branch_get(ખાલી);

	printf(_("%s\n"
		 "Please specify which branch you want to rebase against.\n"
		 "See git-rebase(1) for details.\n"
		 "\n"
		 "    git rebase '<branch>'\n"
		 "\n"),
		current_branch ? _("There is no tracking information for "
			"the current branch.") :
			_("You are not currently on a branch."));

	જો (current_branch) {
		અચલ અક્ષર *remote = current_branch->remote_name;

		જો (!remote)
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

સ્થિર પૂર્ણાંક check_exec_cmd(અચલ અક્ષર *cmd)
{
	જો (strchr(cmd, '\n'))
		પાછા error(_("exec commands cannot contain newlines"));

	/* Does the command consist purely of whitespace? */
	જો (!cmd[strspn(cmd, " \t\r\f\v")])
		પાછા error(_("empty exec command"));

	પાછા 0;
}

પૂર્ણાંક cmd_rebase(પૂર્ણાંક argc,
	       અચલ અક્ષર **argv,
	       અચલ અક્ષર *prefix,
	       સંરચના repository *repo UNUSED)
{
	સંરચના rebase_options options = REBASE_OPTIONS_INIT;
	અચલ અક્ષર *branch_name;
	અચલ અક્ષર *strategy_opt = ખાલી;
	પૂર્ણાંક ret, flags, total_argc, in_progress = 0;
	પૂર્ણાંક keep_base = 0;
	પૂર્ણાંક ok_to_skip_pre_rebase = 0;
	સંરચના strbuf સંદેશ = STRBUF_INIT;
	સંરચના strbuf revisions = STRBUF_INIT;
	સંરચના strbuf buf = STRBUF_INIT;
	સંરચના object_id branch_base;
	પૂર્ણાંક ignore_whitespace = 0;
	અચલ અક્ષર *gpg_sign = ખાલી;
	સંરચના object_id squash_onto;
	અક્ષર *squash_onto_name = ખાલી;
	અક્ષર *keep_base_onto_name = ખાલી;
	પૂર્ણાંક reschedule_failed_exec = -1;
	પૂર્ણાંક allow_preemptive_ff = 1;
	પૂર્ણાંક preserve_merges_selected = 0;
	સંરચના reset_working_વૃક્ષ_options ropts = { 0 };
	સંરચના option builtin_rebase_options[] = {
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
			.કિંમત = &options.flags,
			.precision = કદ(options.flags),
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
		OPT_PASSTHRU_ARGV('C', ખાલી, &options.git_am_opts, N_("n"),
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
		OPT_CALLBACK_F(0, "apply", &options, ખાલી,
			N_("use apply strategies to rebase"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			parse_opt_am),
		OPT_CALLBACK_F('m', "merge", &options, ખાલી,
			N_("use merging strategies to rebase"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			parse_opt_merge),
		OPT_CALLBACK_F('i', "interactive", &options, ખાલી,
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
		OPT_CALLBACK_F('k', "keep-empty", &options, ખાલી,
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
			.કિંમત = &gpg_sign,
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
		OPT_BOOL(0, "root", &options.મૂળ,
			 N_("rebase all reachable commits up to the root(s)")),
		OPT_BOOL(0, "reschedule-failed-exec",
			 &reschedule_failed_exec,
			 N_("automatically re-schedule any `exec` that fails")),
		OPT_BOOL(0, "reapply-cherry-picks", &options.reapply_cherry_picks,
			 N_("apply all changes, even those already present upstream")),
		OPT_END(),
	};
	પૂર્ણાંક i;

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
	gpg_sign = options.gpg_sign_opt ? "" : ખાલી;
	FREE_AND_NULL(options.gpg_sign_opt);

	strbuf_reset(&buf);
	strbuf_addf(&buf, "%s/applying", apply_dir());
	જો(file_exists(buf.buf))
		die(_("It looks like 'git am' is in progress. Cannot rebase."));

	જો (is_directory(apply_dir())) {
		options.type = REBASE_APPLY;
		options.state_dir = apply_dir();
	} નહીંતર જો (is_directory(merge_dir())) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "%s/rewritten", merge_dir());
		જો (!(options.action == ACTION_ABORT) && is_directory(buf.buf)) {
			die(_("`rebase --preserve-merges` (-p) is no longer supported.\n"
			"Use `git rebase --abort` to terminate current rebase.\n"
			"Or downgrade to v2.33, or earlier, to complete the rebase."));
		} નહીંતર {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "%s/interactive", merge_dir());
			options.type = REBASE_MERGE;
			જો (file_exists(buf.buf))
				options.flags |= REBASE_INTERACTIVE_EXPLICIT;
		}
		options.state_dir = merge_dir();
	}

	જો (options.type != REBASE_UNSPECIFIED)
		in_progress = 1;

	total_argc = argc;
	argc = parse_options(argc, argv, prefix,
			     builtin_rebase_options,
			     builtin_rebase_usage, 0);

	જો (options.trailer_args.nr) {
		જો (validate_trailer_args(&options.trailer_args))
			die(ખાલી);
		options.flags |= REBASE_FORCE;
	}

	જો (preserve_merges_selected)
		die(_("--preserve-merges was replaced by --rebase-merges\n"
			"Note: Your `pull.rebase` configuration may also be set to 'preserve',\n"
			"which is no longer supported; use 'merges' instead"));

	જો (options.action != ACTION_NONE && total_argc != 2) {
		usage_with_options(builtin_rebase_usage,
				   builtin_rebase_options);
	}

	જો (argc > 2)
		usage_with_options(builtin_rebase_usage,
				   builtin_rebase_options);

	જો (keep_base) {
		જો (options.onto_name)
			die(_("options '%s' and '%s' cannot be used together"), "--keep-base", "--onto");
		જો (options.મૂળ)
			die(_("options '%s' and '%s' cannot be used together"), "--keep-base", "--root");
		/*
		 * --keep-base defaults to --no-fork-point to keep the
		 * base the same.
		 */
		જો (options.fork_point < 0)
			options.fork_point = 0;
	}
	જો (options.મૂળ && options.fork_point > 0)
		die(_("options '%s' and '%s' cannot be used together"), "--root", "--fork-point");

	જો (options.action != ACTION_NONE && !in_progress)
		die(_("no rebase in progress"));

	જો (options.action == ACTION_EDIT_TODO && !is_merge(&options))
		die(_("The --edit-todo action can only be used during "
		      "interactive rebase."));

	જો (trace2_is_enabled()) {
		જો (is_merge(&options))
			trace2_cmd_mode("interactive");
		નહીંતર જો (options.exec.nr)
			trace2_cmd_mode("interactive-exec");
		નહીંતર
			trace2_cmd_mode(action_names[options.action]);
	}

	options.reflog_action = getenv(GIT_REFLOG_ACTION_ENVIRONMENT);
	options.reflog_action =
		xstrdup(options.reflog_action ? options.reflog_action : "rebase");

	પસંદગી (options.action) {
	સ્થિતિ ACTION_CONTINUE: {
		સંરચના object_id head;
		સંરચના lock_file lock_file = LOCK_INIT;
		પૂર્ણાંક fd;

		/* Sanity check */
		જો (repo_get_oid(the_repository, "HEAD", &head))
			die(_("Cannot read HEAD"));

		fd = repo_hold_locked_index(the_repository, &lock_file, 0);
		જો (repo_read_index(the_repository) < 0)
			die(_("could not read index"));
		refresh_index(the_repository->index, REFRESH_QUIET, ખાલી, ખાલી,
			      ખાલી);
		જો (0 <= fd)
			repo_update_index_if_able(the_repository, &lock_file);
		rollback_lock_file(&lock_file);

		જો (has_unstaged_changes(the_repository, 1)) {
			puts(_("You must edit all merge conflicts and then\n"
			       "mark them as resolved using git add"));
			exit(1);
		}
		જો (read_basic_state(&options))
			exit(1);
		જાઓ run_rebase;
	}
	સ્થિતિ ACTION_SKIP: {
		સંરચના string_list merge_rr = STRING_LIST_INIT_DUP;

		rerere_clear(the_repository, &merge_rr);
		string_list_clear(&merge_rr, 1);
		ropts.flags = RESET_WORKING_વૃક્ષ_HARD |
			      RESET_WORKING_વૃક્ષ_UPDATE_HEAD;
		જો (reset_working_વૃક્ષ(the_repository, &ropts) < 0)
			die(_("could not discard worktree changes"));
		remove_branch_state(the_repository, 0);
		જો (read_basic_state(&options))
			exit(1);
		જાઓ run_rebase;
	}
	સ્થિતિ ACTION_ABORT: {
		સંરચના string_list merge_rr = STRING_LIST_INIT_DUP;
		સંરચના strbuf head_સંદેશ = STRBUF_INIT;

		rerere_clear(the_repository, &merge_rr);
		string_list_clear(&merge_rr, 1);

		જો (read_basic_state(&options))
			exit(1);

		strbuf_addf(&head_સંદેશ, "%s (abort): returning to %s",
			    options.reflog_action,
			    options.head_name ? options.head_name
					      : oid_to_hex(&options.orig_head->object.oid));
		ropts.oid = &options.orig_head->object.oid;
		ropts.head_સંદેશ = head_સંદેશ.buf;
		ropts.branch = options.head_name;
		ropts.flags = RESET_WORKING_વૃક્ષ_HARD |
			      RESET_WORKING_વૃક્ષ_UPDATE_HEAD;
		જો (reset_working_વૃક્ષ(the_repository, &ropts) < 0)
			die(_("could not move back to %s"),
			    oid_to_hex(&options.orig_head->object.oid));
		strbuf_release(&head_સંદેશ);
		remove_branch_state(the_repository, 0);
		ret = finish_rebase(&options);
		જાઓ cleanup;
	}
	સ્થિતિ ACTION_QUIT: {
		save_autostash(state_dir_path("autostash", &options));
		જો (options.type == REBASE_MERGE) {
			સંરચના replay_opts replay = REPLAY_OPTS_INIT;

			replay.action = REPLAY_INTERACTIVE_REBASE;
			ret = sequencer_remove_state(&replay);
			replay_opts_release(&replay);
		} નહીંતર {
			strbuf_reset(&buf);
			strbuf_addstr(&buf, options.state_dir);
			ret = remove_dir_recursively(&buf, 0);
			જો (ret)
				error(_("could not remove '%s'"),
				       options.state_dir);
		}
		જાઓ cleanup;
	}
	સ્થિતિ ACTION_EDIT_TODO:
		options.dont_finish_rebase = 1;
		જાઓ run_rebase;
	સ્થિતિ ACTION_SHOW_CURRENT_PATCH:
		options.dont_finish_rebase = 1;
		જાઓ run_rebase;
	સ્થિતિ ACTION_NONE:
		તોડો;
	મૂળભૂત:
		BUG("action: %d", options.action);
	}

	/* Make sure no rebase is in progress */
	જો (in_progress) {
		અચલ અક્ષર *last_slash = strrchr(options.state_dir, '/');
		અચલ અક્ષર *state_dir_base =
			last_slash ? last_slash + 1 : options.state_dir;
		અચલ અક્ષર *cmd_live_rebase =
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

	જો ((options.flags & REBASE_INTERACTIVE_EXPLICIT) ||
	    (options.action != ACTION_NONE) ||
	    (options.exec.nr > 0) ||
	    options.autosquash == 1) {
		allow_preemptive_ff = 0;
	}
	જો (options.committer_date_is_author_date || options.ignore_date)
		options.flags |= REBASE_FORCE;

	માટે (i = 0; i < options.git_am_opts.nr; i++) {
		અચલ અક્ષર *option = options.git_am_opts.v[i], *p;
		જો (!strcmp(option, "--whitespace=fix") ||
		    !strcmp(option, "--whitespace=strip"))
			allow_preemptive_ff = 0;
		નહીંતર જો (skip_prefix(option, "-C", &p)) {
			જ્યારે (*p)
				જો (!isdigit(*(p++)))
					die(_("switch `C' expects a "
					      "numerical value"));
		} નહીંતર જો (skip_prefix(option, "--whitespace=", &p)) {
			જો (*p && strcmp(p, "warn") && strcmp(p, "nowarn") &&
			    strcmp(p, "error") && strcmp(p, "error-all"))
				die("Invalid whitespace option: '%s'", p);
		}
	}

	માટે (i = 0; i < options.exec.nr; i++)
		જો (check_exec_cmd(options.exec.વસ્તુઓ[i].string))
			exit(1);

	જો (!(options.flags & REBASE_NO_QUIET))
		strvec_ધકેલો(&options.git_am_opts, "-q");

	જો (options.empty != EMPTY_UNSPECIFIED)
		imply_merge(&options, "--empty");

	જો (options.reapply_cherry_picks < 0)
		/*
		 * We default to --no-reapply-cherry-picks unless
		 * --keep-base is given; when --keep-base is given, we want
		 * to default to --reapply-cherry-picks.
		 */
		options.reapply_cherry_picks = keep_base;
	નહીંતર જો (!keep_base)
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

	જો (gpg_sign)
		options.gpg_sign_opt = xstrfmt("-S%s", gpg_sign);

	જો (options.exec.nr)
		imply_merge(&options, "--exec");

	જો (options.type == REBASE_APPLY) {
		જો (ignore_whitespace)
			strvec_ધકેલો(&options.git_am_opts,
				    "--ignore-whitespace");
		જો (options.committer_date_is_author_date)
			strvec_ધકેલો(&options.git_am_opts,
				    "--committer-date-is-author-date");
		જો (options.ignore_date)
			strvec_ધકેલો(&options.git_am_opts, "--ignore-date");
	} નહીંતર {
		/* REBASE_MERGE */
		જો (ignore_whitespace) {
			string_list_append(&options.strategy_opts,
					   "ignore-space-change");
		}
	}

	જો (strategy_opt)
		options.strategy = xstrdup(strategy_opt);
	નહીંતર જો (options.strategy_opts.nr && !options.strategy)
		options.strategy = xstrdup("ort");
	જો (options.strategy)
		imply_merge(&options, "--strategy");

	જો (options.મૂળ && !options.onto_name)
		imply_merge(&options, "--root without --onto");

	જો (options.trailer_args.nr)
		imply_merge(&options, "--trailer");

	જો (isatty(2) && options.flags & REBASE_NO_QUIET)
		strbuf_addstr(&options.git_format_patch_opt, " --progress");

	જો (options.git_am_opts.nr || options.type == REBASE_APPLY) {
		/* all am options except -q are compatible only with --apply */
		માટે (i = options.git_am_opts.nr - 1; i >= 0; i--)
			જો (strcmp(options.git_am_opts.v[i], "-q"))
				તોડો;

		જો (i >= 0 || options.type == REBASE_APPLY) {
			જો (is_merge(&options))
				die(_("apply options and merge options "
					  "cannot be used together"));
			નહીંતર જો (options.rebase_merges == -1 && options.config_rebase_merges == 1)
				die(_("apply options are incompatible with rebase.rebaseMerges.  Consider adding --no-rebase-merges"));
			નહીંતર જો (options.update_refs == -1 && options.config_update_refs == 1)
				die(_("apply options are incompatible with rebase.updateRefs.  Consider adding --no-update-refs"));
			નહીંતર
				options.type = REBASE_APPLY;
		}
	}

	જો (options.update_refs == 1)
		imply_merge(&options, "--update-refs");
	options.update_refs = (options.update_refs >= 0) ? options.update_refs :
			     ((options.config_update_refs >= 0) ? options.config_update_refs : 0);

	જો (options.rebase_merges == 1)
		imply_merge(&options, "--rebase-merges");
	options.rebase_merges = (options.rebase_merges >= 0) ? options.rebase_merges :
				((options.config_rebase_merges >= 0) ? options.config_rebase_merges : 0);

	જો (options.autosquash == 1) {
		imply_merge(&options, "--autosquash");
	} નહીંતર જો (options.autosquash == -1) {
		options.autosquash =
			options.config_autosquash &&
			(options.flags & REBASE_INTERACTIVE_EXPLICIT);
	}

	જો (options.type == REBASE_UNSPECIFIED) {
		જો (!strcmp(options.default_backend, "merge"))
			options.type = REBASE_MERGE;
		નહીંતર જો (!strcmp(options.default_backend, "apply"))
			options.type = REBASE_APPLY;
		નહીંતર
			die(_("Unknown rebase backend: %s"),
			    options.default_backend);
	}

	પસંદગી (options.type) {
	સ્થિતિ REBASE_MERGE:
		options.state_dir = merge_dir();
		તોડો;
	સ્થિતિ REBASE_APPLY:
		options.state_dir = apply_dir();
		તોડો;
	મૂળભૂત:
		BUG("options.type was just set above; should be unreachable.");
	}

	જો (options.empty == EMPTY_UNSPECIFIED) {
		જો (options.flags & REBASE_INTERACTIVE_EXPLICIT)
			options.empty = EMPTY_STOP;
		નહીંતર જો (options.exec.nr > 0)
			options.empty = EMPTY_KEEP;
		નહીંતર
			options.empty = EMPTY_DROP;
	}
	જો (reschedule_failed_exec > 0 && !is_merge(&options))
		die(_("--reschedule-failed-exec requires "
		      "--exec or --interactive"));
	જો (reschedule_failed_exec >= 0)
		options.reschedule_failed_exec = reschedule_failed_exec;

	જો (options.signoff) {
		strvec_ધકેલો(&options.git_am_opts, "--signoff");
		options.flags |= REBASE_FORCE;
	}

	જો (!options.મૂળ) {
		જો (argc < 1) {
			સંરચના branch *branch;

			branch = branch_get(ખાલી);
			options.upstream_name = branch_get_upstream(branch,
								    ખાલી);
			જો (!options.upstream_name)
				error_on_missing_default_upstream();
			જો (options.fork_point < 0)
				options.fork_point = 1;
		} નહીંતર {
			options.upstream_name = argv[0];
			argc--;
			argv++;
			જો (!strcmp(options.upstream_name, "-"))
				options.upstream_name = "@{-1}";
		}
		options.upstream =
			lookup_commit_reference_by_name(options.upstream_name);
		જો (!options.upstream)
			die(_("invalid upstream '%s'"), options.upstream_name);
		options.upstream_arg = options.upstream_name;
	} નહીંતર {
		જો (!options.onto_name) {
			જો (commit_વૃક્ષ("", 0, the_hash_algo->empty_વૃક્ષ, ખાલી,
					&squash_onto, ખાલી, ખાલી) < 0)
				die(_("Could not create new root commit"));
			options.squash_onto = &squash_onto;
			options.onto_name = squash_onto_name =
				xstrdup(oid_to_hex(&squash_onto));
		} નહીંતર
			options.મૂળ_with_onto = 1;

		options.upstream_name = ખાલી;
		options.upstream = ખાલી;
		જો (argc > 1)
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
	જો (argc == 1) {
		/* Is it "rebase other branchname" or "rebase other commit"? */
		સંરચના object_id branch_oid;
		branch_name = argv[0];
		options.switch_to = argv[0];

		/* Is it a local branch? */
		strbuf_reset(&buf);
		strbuf_addf(&buf, "refs/heads/%s", branch_name);
		જો (!refs_read_ref(get_main_ref_store(the_repository), buf.buf, &branch_oid)) {
			die_if_checked_out(buf.buf, 1);
			options.head_name = xstrdup(buf.buf);
			options.orig_head =
				lookup_commit_object(the_repository,
						     &branch_oid);
		/* If not is it a valid ref (branch or commit)? */
		} નહીંતર {
			options.orig_head =
				lookup_commit_reference_by_name(branch_name);
			options.head_name = ખાલી;
		}
		જો (!options.orig_head)
			die(_("no such branch/commit '%s'"), branch_name);
	} નહીંતર જો (argc == 0) {
		/* Do not need to switch branches, we are already on it. */
		options.head_name =
			xstrdup_or_null(refs_resolve_ref_unsafe(get_main_ref_store(the_repository), "HEAD", 0, ખાલી,
								&flags));
		જો (!options.head_name)
			die(_("No such ref: %s"), "HEAD");
		જો (flags & REF_ISSYMREF) {
			જો (!skip_prefix(options.head_name,
					 "refs/heads/", &branch_name))
				branch_name = options.head_name;

		} નહીંતર {
			FREE_AND_NULL(options.head_name);
			branch_name = "HEAD";
		}
		options.orig_head = lookup_commit_reference_by_name("HEAD");
		જો (!options.orig_head)
			die(_("Could not resolve HEAD to a commit"));
	} નહીંતર
		BUG("unexpected number of arguments left to parse");

	/* Make sure the branch to rebase onto is valid. */
	જો (keep_base) {
		strbuf_reset(&buf);
		strbuf_addstr(&buf, options.upstream_name);
		strbuf_addstr(&buf, "...");
		strbuf_addstr(&buf, branch_name);
		options.onto_name = keep_base_onto_name = xstrdup(buf.buf);
	} નહીંતર જો (!options.onto_name)
		options.onto_name = options.upstream_name;
	જો (strstr(options.onto_name, "...")) {
		જો (repo_get_oid_mb(the_repository, options.onto_name, &branch_base) < 0) {
			જો (keep_base)
				die(_("'%s': need exactly one merge base with branch"),
				    options.upstream_name);
			નહીંતર
				die(_("'%s': need exactly one merge base"),
				    options.onto_name);
		}
		options.onto = lookup_commit_or_die(&branch_base,
						    options.onto_name);
	} નહીંતર {
		options.onto =
			lookup_commit_reference_by_name(options.onto_name);
		જો (!options.onto)
			die(_("Does not point to a valid commit '%s'"),
				options.onto_name);
		fill_branch_base(&options, &branch_base);
	}

	જો (keep_base && options.reapply_cherry_picks)
		options.upstream = options.onto;

	જો (options.fork_point > 0)
		options.restrict_revision =
			get_fork_point(options.upstream_name, options.orig_head);

	જો (repo_read_index(the_repository) < 0)
		die(_("could not read index"));

	જો (options.autostash)
		create_autostash(the_repository,
				 state_dir_path("autostash", &options));


	જો (require_clean_work_વૃક્ષ(the_repository, "rebase",
				    _("Please commit or stash them."), 1, 1)) {
		ret = -1;
		જાઓ cleanup_autostash;
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
	જો (allow_preemptive_ff &&
	    can_fast_forward(options.onto, options.upstream, options.restrict_revision,
			     options.orig_head, &branch_base)) {
		પૂર્ણાંક flag;

		જો (!(options.flags & REBASE_FORCE)) {
			/* Lazily switch to the target branch if needed... */
			જો (options.switch_to) {
				ret = checkout_up_to_date(&options);
				જો (ret)
					જાઓ cleanup_autostash;
			}

			જો (!(options.flags & REBASE_NO_QUIET))
				; /* be quiet */
			નહીંતર જો (!strcmp(branch_name, "HEAD") &&
				 refs_resolve_ref_unsafe(get_main_ref_store(the_repository), "HEAD", 0, ખાલી, &flag))
				puts(_("HEAD is up to date."));
			નહીંતર
				printf(_("Current branch %s is up to date.\n"),
				       branch_name);
			ret = finish_rebase(&options);
			જાઓ cleanup;
		} નહીંતર જો (!(options.flags & REBASE_NO_QUIET))
			; /* be quiet */
		નહીંતર જો (!strcmp(branch_name, "HEAD") &&
			 refs_resolve_ref_unsafe(get_main_ref_store(the_repository), "HEAD", 0, ખાલી, &flag))
			puts(_("HEAD is up to date, rebase forced."));
		નહીંતર
			printf(_("Current branch %s is up to date, rebase "
				 "forced.\n"), branch_name);
	}

	/* If a hook exists, give it a chance to interrupt*/
	જો (!ok_to_skip_pre_rebase &&
	    run_hooks_l(the_repository, "pre-rebase", options.upstream_arg,
			argc ? argv[0] : ખાલી, ખાલી)) {
		ret = error(_("The pre-rebase hook refused to rebase."));
		જાઓ cleanup_autostash;
	}

	જો (options.flags & REBASE_DIFFSTAT) {
		સંરચના diff_options opts;

		જો (options.flags & REBASE_VERBOSE) {
			જો (is_null_oid(&branch_base))
				printf(_("Changes to %s:\n"),
				       oid_to_hex(&options.onto->object.oid));
			નહીંતર
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
		diff_વૃક્ષ_oid(is_null_oid(&branch_base) ?
			      the_hash_algo->empty_વૃક્ષ : &branch_base,
			      &options.onto->object.oid, "", &opts);
		diffcore_std(&opts);
		diff_flush(&opts);
	}

	જો (is_merge(&options))
		જાઓ run_rebase;

	/* Detach HEAD and reset the tree */
	જો (options.flags & REBASE_NO_QUIET)
		printf(_("First, rewinding head to replay your work on top of "
			 "it...\n"));

	strbuf_addf(&સંદેશ, "%s (start): checkout %s",
		    options.reflog_action, options.onto_name);
	ropts.oid = &options.onto->object.oid;
	ropts.orig_head = &options.orig_head->object.oid;
	ropts.flags = RESET_WORKING_વૃક્ષ_DETACH |
		      RESET_WORKING_વૃક્ષ_UPDATE_HEAD |
		      RESET_WORKING_વૃક્ષ_UPDATE_ORIG_HEAD |
		      RESET_WORKING_વૃક્ષ_RUN_POST_CHECKOUT_HOOK;
	ropts.head_સંદેશ = સંદેશ.buf;
	ropts.default_reflog_action = options.reflog_action;
	જો (reset_working_વૃક્ષ(the_repository, &ropts)) {
		ret = error(_("Could not detach HEAD"));
		જાઓ cleanup_autostash;
	}

	/*
	 * If the onto is a proper descendant of the tip of the branch, then
	 * we just fast-forwarded.
	 */
	જો (oideq(&branch_base, &options.orig_head->object.oid)) {
		printf(_("Fast-forwarded %s to %s.\n"),
			branch_name, options.onto_name);
		move_to_original_branch(&options);
		ret = finish_rebase(&options);
		જાઓ cleanup;
	}

	strbuf_addf(&revisions, "%s..%s",
		    options.મૂળ ? oid_to_hex(&options.onto->object.oid) :
		    (options.restrict_revision ?
		     oid_to_hex(&options.restrict_revision->object.oid) :
		     oid_to_hex(&options.upstream->object.oid)),
		    oid_to_hex(&options.orig_head->object.oid));

	options.revisions = revisions.buf;

run_rebase:
	ret = run_specific_rebase(&options);

cleanup:
	strbuf_release(&buf);
	strbuf_release(&સંદેશ);
	strbuf_release(&revisions);
	rebase_options_release(&options);
	મુક્ત(squash_onto_name);
	મુક્ત(keep_base_onto_name);
	પાછા !!ret;

cleanup_autostash:
	ret |= !!cleanup_autostash(&options);
	જાઓ cleanup;
}
