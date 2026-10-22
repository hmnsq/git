/*
 * "git rebase" builtin command
 *
 * Copyright (c) 2018 Pratik Karki
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#அந்தர்பாவ "builtin.h"

#அந்தர்பாவ "abspath.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "hex.h"
#அந்தர்பாவ "run-command.h"
#அந்தர்பாவ "strvec.h"
#அந்தர்பாவ "dir.h"
#அந்தர்பாவ "refs.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "unpack-trees.h"
#அந்தர்பாவ "lockfile.h"
#அந்தர்பாவ "object-file.h"
#அந்தர்பாவ "object-name.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "path.h"
#அந்தர்பாவ "commit.h"
#அந்தர்பாவ "diff.h"
#அந்தர்பாவ "wt-status.h"
#அந்தர்பாவ "revision.h"
#அந்தர்பாவ "commit-reach.h"
#அந்தர்பாவ "rerere.h"
#அந்தர்பாவ "branch.h"
#அந்தர்பாவ "sequencer.h"
#அந்தர்பாவ "rebase-interactive.h"
#அந்தர்பாவ "reset.h"
#அந்தர்பாவ "trace2.h"
#அந்தர்பாவ "hook.h"
#அந்தர்பாவ "trailer.h"

நிலையான எழுத்து மாறிலி * மாறிலி builtin_rebase_usage[] = {
	N_("git rebase [-i] [options] [--exec <cmd>] "
		"[--onto <newbase> | --keep-base] [<upstream> [<branch>]]"),
	N_("git rebase [-i] [options] [--exec <cmd>] [--onto <newbase>] "
		"--root [<branch>]"),
	"git rebase --continue | --abort | --skip | --edit-todo",
	வெற்று
};

நிலையான GIT_PATH_FUNC(path_squash_onto, "rebase-merge/squash-onto")
நிலையான GIT_PATH_FUNC(path_interactive, "rebase-merge/interactive")
நிலையான GIT_PATH_FUNC(apply_dir, "rebase-apply")
நிலையான GIT_PATH_FUNC(merge_dir, "rebase-merge")

எண்ணிக்கை rebase_type {
	REBASE_UNSPECIFIED = -1,
	REBASE_APPLY,
	REBASE_MERGE
};

எண்ணிக்கை empty_type {
	EMPTY_UNSPECIFIED = -1,
	EMPTY_DROP,
	EMPTY_KEEP,
	EMPTY_STOP
};

எண்ணிக்கை action {
	ACTION_NONE = 0,
	ACTION_CONTINUE,
	ACTION_SKIP,
	ACTION_ABORT,
	ACTION_QUIT,
	ACTION_EDIT_TODO,
	ACTION_SHOW_CURRENT_PATCH
};

நிலையான மாறிலி எழுத்து *action_names[] = {
	"undefined",
	"continue",
	"skip",
	"abort",
	"quit",
	"edit_todo",
	"show_current_patch"
};

கட்டமைப்பு rebase_options {
	எண்ணிக்கை rebase_type type;
	எண்ணிக்கை empty_type empty;
	எழுத்து *default_backend;
	மாறிலி எழுத்து *state_dir;
	கட்டமைப்பு commit *upstream;
	மாறிலி எழுத்து *upstream_name;
	மாறிலி எழுத்து *upstream_arg;
	எழுத்து *head_name;
	கட்டமைப்பு commit *orig_head;
	கட்டமைப்பு commit *onto;
	மாறிலி எழுத்து *onto_name;
	மாறிலி எழுத்து *revisions;
	மாறிலி எழுத்து *switch_to;
	முழு வேர், வேர்_with_onto;
	கட்டமைப்பு object_id *squash_onto;
	கட்டமைப்பு commit *restrict_revision;
	முழு dont_finish_rebase;
	எண்ணிக்கை {
		REBASE_NO_QUIET = 1<<0,
		REBASE_VERBOSE = 1<<1,
		REBASE_DIFFSTAT = 1<<2,
		REBASE_FORCE = 1<<3,
		REBASE_INTERACTIVE_EXPLICIT = 1<<4,
	} flags;
	கட்டமைப்பு strvec git_am_opts;
	எண்ணிக்கை action action;
	எழுத்து *reflog_action;
	முழு signoff;
	கட்டமைப்பு strvec trailer_args;
	முழு allow_rerere_autoupdate;
	முழு keep_empty;
	முழு autosquash;
	எழுத்து *gpg_sign_opt;
	முழு autostash;
	முழு committer_date_is_author_date;
	முழு ignore_date;
	கட்டமைப்பு string_list exec;
	முழு allow_empty_message;
	முழு rebase_merges, rebase_cousins;
	எழுத்து *strategy;
	கட்டமைப்பு string_list strategy_opts;
	கட்டமைப்பு strbuf git_format_patch_opt;
	முழு reschedule_failed_exec;
	முழு reapply_cherry_picks;
	முழு fork_point;
	முழு update_refs;
	முழு config_autosquash;
	முழு config_rebase_merges;
	முழு config_update_refs;
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

நிலையான வெறுமை rebase_options_release(கட்டமைப்பு rebase_options *opts)
{
	விடுவி(opts->default_backend);
	விடுவி(opts->reflog_action);
	விடுவி(opts->head_name);
	strvec_clear(&opts->git_am_opts);
	விடுவி(opts->gpg_sign_opt);
	string_list_clear(&opts->exec, 0);
	விடுவி(opts->strategy);
	string_list_clear(&opts->strategy_opts, 0);
	strbuf_release(&opts->git_format_patch_opt);
	strvec_clear(&opts->trailer_args);
}

நிலையான கட்டமைப்பு replay_opts get_replay_opts(மாறிலி கட்டமைப்பு rebase_options *opts)
{
	கட்டமைப்பு replay_opts replay = REPLAY_OPTS_INIT;

	replay.action = REPLAY_INTERACTIVE_REBASE;
	replay.strategy = வெற்று;
	sequencer_init_config(&replay);

	replay.signoff = opts->signoff;

	strvec_pushv(&replay.trailer_args, opts->trailer_args.v);

	replay.allow_ff = !(opts->flags & REBASE_FORCE);
	எனில் (opts->allow_rerere_autoupdate)
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
	விடுவி(replay.gpg_sign);
	replay.gpg_sign = xstrdup_or_null(opts->gpg_sign_opt);
	replay.reflog_action = xstrdup(opts->reflog_action);
	எனில் (opts->strategy)
		replay.strategy = xstrdup_or_null(opts->strategy);
	இல்லையெனில் எனில் (!replay.strategy && replay.default_strategy) {
		replay.strategy = replay.default_strategy;
		replay.default_strategy = வெற்று;
	}

	க்கு (size_t i = 0; i < opts->strategy_opts.nr; i++)
		strvec_தள்ளு(&replay.xopts, opts->strategy_opts.பொருட்கள்[i].string);

	எனில் (opts->squash_onto) {
		oidcpy(&replay.squash_onto, opts->squash_onto);
		replay.have_squash_onto = 1;
	}

	திரும்பு replay;
}

நிலையான முழு edit_todo_file(unsigned flags, கட்டமைப்பு replay_opts *opts)
{
	மாறிலி எழுத்து *todo_file = rebase_path_todo();
	கட்டமைப்பு todo_list todo_list = TODO_LIST_INIT,
		new_todo = TODO_LIST_INIT;
	முழு res = 0;

	எனில் (strbuf_read_file(&todo_list.buf, todo_file, 0) < 0)
		திரும்பு error_errno(_("could not read '%s'."), todo_file);

	strbuf_stripspace(&todo_list.buf, comment_வரி_str);
	res = edit_todo_list(the_repository, opts, &todo_list, &new_todo,
			     வெற்று, வெற்று, flags);
	எனில் (!res && todo_list_write_to_file(the_repository, &new_todo, todo_file,
					    வெற்று, வெற்று, -1, flags & ~(TODO_LIST_SHORTEN_IDS)))
		res = error_errno(_("could not write '%s'"), todo_file);

	todo_list_release(&todo_list);
	todo_list_release(&new_todo);

	திரும்பு res;
}

நிலையான முழு get_revision_ranges(கட்டமைப்பு commit *upstream, கட்டமைப்பு commit *onto,
			       கட்டமைப்பு object_id *orig_head, எழுத்து **revisions,
			       எழுத்து **shortrevisions)
{
	கட்டமைப்பு commit *base_rev = upstream ? upstream : onto;
	மாறிலி எழுத்து *shorthead;

	*revisions = xstrfmt("%s...%s", oid_to_hex(&base_rev->object.oid),
			     oid_to_hex(orig_head));

	shorthead = repo_find_unique_abbrev(the_repository, orig_head,
					    DEFAULT_ABBREV);

	எனில் (upstream) {
		மாறிலி எழுத்து *shortrev;

		shortrev = repo_find_unique_abbrev(the_repository,
						   &base_rev->object.oid,
						   DEFAULT_ABBREV);

		*shortrevisions = xstrfmt("%s..%s", shortrev, shorthead);
	} இல்லையெனில்
		*shortrevisions = xstrdup(shorthead);

	திரும்பு 0;
}

நிலையான முழு init_basic_state(கட்டமைப்பு replay_opts *opts, மாறிலி எழுத்து *head_name,
			    கட்டமைப்பு commit *onto,
			    மாறிலி கட்டமைப்பு object_id *orig_head)
{
	FILE *interactive;

	எனில் (!is_directory(merge_dir()) &&
	    safe_create_dir_in_gitdir(the_repository, merge_dir()))
		திரும்பு error_errno(_("could not create temporary %s"), merge_dir());

	refs_delete_reflog(get_main_ref_store(the_repository), "REBASE_HEAD");

	interactive = fopen(path_interactive(), "w");
	எனில் (!interactive)
		திரும்பு error_errno(_("could not mark as interactive"));
	fclose(interactive);

	திரும்பு write_basic_state(opts, head_name, onto, orig_head);
}

நிலையான முழு do_interactive_rebase(கட்டமைப்பு rebase_options *opts, unsigned flags)
{
	முழு ret = -1;
	எழுத்து *revisions = வெற்று, *shortrevisions = வெற்று;
	கட்டமைப்பு strvec make_script_args = STRVEC_INIT;
	கட்டமைப்பு todo_list todo_list = TODO_LIST_INIT;
	கட்டமைப்பு replay_opts replay = get_replay_opts(opts);

	எனில் (get_revision_ranges(opts->upstream, opts->onto, &opts->orig_head->object.oid,
				&revisions, &shortrevisions))
		செல் cleanup;

	strvec_pushl(&make_script_args, "", revisions, வெற்று);
	எனில் (opts->restrict_revision)
		strvec_pushf(&make_script_args, "^%s",
			     oid_to_hex(&opts->restrict_revision->object.oid));

	ret = sequencer_make_script(the_repository, &todo_list.buf,
				    &make_script_args, flags);
	எனில் (ret) {
		error(_("could not generate todo list"));
		செல் cleanup;
	}

	எனில் (init_basic_state(&replay,
			     opts->head_name ? opts->head_name : "detached HEAD",
			     opts->onto, &opts->orig_head->object.oid))
		செல் cleanup;

	எனில் (!opts->upstream && opts->squash_onto)
		write_file(path_squash_onto(), "%s\n",
			   oid_to_hex(opts->squash_onto));

	discard_index(the_repository->index);
	எனில் (todo_list_parse_insn_buffer(the_repository, &replay,
					todo_list.buf.buf, &todo_list))
		BUG("unusable todo list");

	ret = complete_action(the_repository, &replay, flags,
		shortrevisions, opts->onto_name, opts->onto,
		&opts->orig_head->object.oid, &opts->exec,
		opts->autosquash, opts->update_refs, &todo_list);

cleanup:
	replay_opts_release(&replay);
	விடுவி(revisions);
	விடுவி(shortrevisions);
	todo_list_release(&todo_list);
	strvec_clear(&make_script_args);

	திரும்பு ret;
}

நிலையான முழு run_sequencer_rebase(கட்டமைப்பு rebase_options *opts)
{
	unsigned flags = 0;
	முழு abbreviate_commands = 0, ret = 0;

	repo_config_get_bool(the_repository, "rebase.abbreviatecommands", &abbreviate_commands);

	flags |= opts->keep_empty ? TODO_LIST_KEEP_EMPTY : 0;
	flags |= abbreviate_commands ? TODO_LIST_ABBREVIATE_CMDS : 0;
	flags |= opts->rebase_merges ? TODO_LIST_REBASE_MERGES : 0;
	flags |= opts->rebase_cousins > 0 ? TODO_LIST_REBASE_COUSINS : 0;
	flags |= opts->வேர்_with_onto ? TODO_LIST_வேர்_WITH_ONTO : 0;
	flags |= opts->reapply_cherry_picks ? TODO_LIST_REAPPLY_CHERRY_PICKS : 0;
	flags |= opts->flags & REBASE_NO_QUIET ? TODO_LIST_WARN_SKIPPED_CHERRY_PICKS : 0;

	தேர்வு (opts->action) {
	நிலை ACTION_NONE: {
		எனில் (!opts->onto && !opts->upstream)
			die(_("a base commit must be provided with --upstream or --onto"));

		ret = do_interactive_rebase(opts, flags);
		நிறுத்து;
	}
	நிலை ACTION_SKIP: {
		கட்டமைப்பு string_list merge_rr = STRING_LIST_INIT_DUP;

		rerere_clear(the_repository, &merge_rr);
	}
		/* fallthrough */
	நிலை ACTION_CONTINUE: {
		கட்டமைப்பு replay_opts replay_opts = get_replay_opts(opts);

		ret = sequencer_continue(the_repository, &replay_opts);
		replay_opts_release(&replay_opts);
		நிறுத்து;
	}
	நிலை ACTION_EDIT_TODO: {
		கட்டமைப்பு replay_opts replay_opts = get_replay_opts(opts);

		ret = edit_todo_file(flags, &replay_opts);
		replay_opts_release(&replay_opts);
		நிறுத்து;
	}
	நிலை ACTION_SHOW_CURRENT_PATCH: {
		கட்டமைப்பு child_process cmd = CHILD_PROCESS_INIT;

		cmd.git_cmd = 1;
		strvec_pushl(&cmd.args, "show", "REBASE_HEAD", "--", வெற்று);
		ret = run_command(&cmd);

		நிறுத்து;
	}
	இயல்பு:
		BUG("invalid command '%d'", opts->action);
	}

	திரும்பு ret;
}

நிலையான முழு is_merge(கட்டமைப்பு rebase_options *opts)
{
	திரும்பு opts->type == REBASE_MERGE;
}

நிலையான வெறுமை imply_merge(கட்டமைப்பு rebase_options *opts, மாறிலி எழுத்து *option)
{
	தேர்வு (opts->type) {
	நிலை REBASE_APPLY:
		die(_("%s requires the merge backend"), option);
		நிறுத்து;
	நிலை REBASE_MERGE:
		நிறுத்து;
	இயல்பு:
		opts->type = REBASE_MERGE; /* implied */
		நிறுத்து;
	}
}

/* Returns the filename prefixed by the state_dir */
நிலையான மாறிலி எழுத்து *state_dir_path(மாறிலி எழுத்து *filename, கட்டமைப்பு rebase_options *opts)
{
	நிலையான கட்டமைப்பு strbuf path = STRBUF_INIT;
	நிலையான size_t prefix_len;

	எனில் (!prefix_len) {
		strbuf_addf(&path, "%s/", opts->state_dir);
		prefix_len = path.len;
	}

	strbuf_setlen(&path, prefix_len);
	strbuf_addstr(&path, filename);
	திரும்பு path.buf;
}

/* Initialize the rebase options from the state directory. */
நிலையான முழு read_basic_state(கட்டமைப்பு rebase_options *opts)
{
	கட்டமைப்பு strbuf head_name = STRBUF_INIT;
	கட்டமைப்பு strbuf buf = STRBUF_INIT;
	கட்டமைப்பு object_id oid;

	எனில் (!read_oneliner(&head_name, state_dir_path("head-name", opts),
			   READ_ONELINER_WARN_MISSING) ||
	    !read_oneliner(&buf, state_dir_path("onto", opts),
			   READ_ONELINER_WARN_MISSING))
		திரும்பு -1;
	opts->head_name = starts_with(head_name.buf, "refs/") ?
		xstrdup(head_name.buf) : வெற்று;
	strbuf_release(&head_name);
	எனில் (get_oid_hex(buf.buf, &oid) ||
	    !(opts->onto = lookup_commit_object(the_repository, &oid)))
		திரும்பு error(_("invalid onto: '%s'"), buf.buf);

	/*
	 * We always write to orig-head, but interactive rebase used to write to
	 * head. Fall back to reading from head to cover for the case that the
	 * user upgraded git with an ongoing interactive rebase.
	 */
	strbuf_reset(&buf);
	எனில் (file_exists(state_dir_path("orig-head", opts))) {
		எனில் (!read_oneliner(&buf, state_dir_path("orig-head", opts),
				   READ_ONELINER_WARN_MISSING))
			திரும்பு -1;
	} இல்லையெனில் எனில் (!read_oneliner(&buf, state_dir_path("head", opts),
				  READ_ONELINER_WARN_MISSING))
		திரும்பு -1;
	எனில் (get_oid_hex(buf.buf, &oid) ||
	    !(opts->orig_head = lookup_commit_object(the_repository, &oid)))
		திரும்பு error(_("invalid orig-head: '%s'"), buf.buf);

	எனில் (file_exists(state_dir_path("quiet", opts)))
		opts->flags &= ~REBASE_NO_QUIET;
	இல்லையெனில்
		opts->flags |= REBASE_NO_QUIET;

	எனில் (file_exists(state_dir_path("verbose", opts)))
		opts->flags |= REBASE_VERBOSE;

	எனில் (file_exists(state_dir_path("signoff", opts))) {
		opts->signoff = 1;
		opts->flags |= REBASE_FORCE;
	}

	எனில் (file_exists(state_dir_path("allow_rerere_autoupdate", opts))) {
		strbuf_reset(&buf);
		எனில் (!read_oneliner(&buf, state_dir_path("allow_rerere_autoupdate", opts),
				   READ_ONELINER_WARN_MISSING))
			திரும்பு -1;
		எனில் (!strcmp(buf.buf, "--rerere-autoupdate"))
			opts->allow_rerere_autoupdate = RERERE_AUTOUPDATE;
		இல்லையெனில் எனில் (!strcmp(buf.buf, "--no-rerere-autoupdate"))
			opts->allow_rerere_autoupdate = RERERE_NOAUTOUPDATE;
		இல்லையெனில்
			warning(_("ignoring invalid allow_rerere_autoupdate: "
				  "'%s'"), buf.buf);
	}

	எனில் (file_exists(state_dir_path("gpg_sign_opt", opts))) {
		strbuf_reset(&buf);
		எனில் (!read_oneliner(&buf, state_dir_path("gpg_sign_opt", opts),
				   READ_ONELINER_WARN_MISSING))
			திரும்பு -1;
		விடுவி(opts->gpg_sign_opt);
		opts->gpg_sign_opt = xstrdup(buf.buf);
	}

	strbuf_release(&buf);

	திரும்பு 0;
}

நிலையான முழு rebase_write_basic_state(கட்டமைப்பு rebase_options *opts)
{
	write_file(state_dir_path("head-name", opts), "%s",
		   opts->head_name ? opts->head_name : "detached HEAD");
	write_file(state_dir_path("onto", opts), "%s",
		   opts->onto ? oid_to_hex(&opts->onto->object.oid) : "");
	write_file(state_dir_path("orig-head", opts), "%s",
		   oid_to_hex(&opts->orig_head->object.oid));
	எனில் (!(opts->flags & REBASE_NO_QUIET))
		write_file(state_dir_path("quiet", opts), "%s", "");
	எனில் (opts->flags & REBASE_VERBOSE)
		write_file(state_dir_path("verbose", opts), "%s", "");
	எனில் (opts->allow_rerere_autoupdate > 0)
		write_file(state_dir_path("allow_rerere_autoupdate", opts),
			   "-%s-rerere-autoupdate",
			   opts->allow_rerere_autoupdate == RERERE_AUTOUPDATE ?
				"" : "-no");
	எனில் (opts->gpg_sign_opt)
		write_file(state_dir_path("gpg_sign_opt", opts), "%s",
			   opts->gpg_sign_opt);
	எனில் (opts->signoff)
		write_file(state_dir_path("signoff", opts), "--signoff");

	திரும்பு 0;
}

நிலையான முழு cleanup_autostash(கட்டமைப்பு rebase_options *opts)
{
	முழு ret;
	கட்டமைப்பு strbuf dir = STRBUF_INIT;
	மாறிலி எழுத்து *path = state_dir_path("autostash", opts);

	எனில் (!file_exists(path))
		திரும்பு 0;
	ret = apply_autostash(path);
	strbuf_addstr(&dir, opts->state_dir);
	எனில் (remove_dir_recursively(&dir, 0))
		ret = error_errno(_("could not remove '%s'"), opts->state_dir);
	strbuf_release(&dir);

	திரும்பு ret;
}

நிலையான முழு finish_rebase(கட்டமைப்பு rebase_options *opts)
{
	கட்டமைப்பு strbuf dir = STRBUF_INIT;
	முழு ret = 0;

	refs_delete_ref(get_main_ref_store(the_repository), வெற்று,
			"REBASE_HEAD", வெற்று, REF_NO_DEREF);
	refs_delete_ref(get_main_ref_store(the_repository), வெற்று,
			"AUTO_MERGE", வெற்று, REF_NO_DEREF);
	apply_autostash(state_dir_path("autostash", opts));
	/*
	 * We ignore errors in 'git maintenance run --auto', since the
	 * user should see them.
	 */
	run_auto_maintenance(the_repository,
			!(opts->flags & (REBASE_NO_QUIET|REBASE_VERBOSE)));

	எனில் (opts->type == REBASE_MERGE) {
		கட்டமைப்பு replay_opts replay = REPLAY_OPTS_INIT;

		replay.action = REPLAY_INTERACTIVE_REBASE;
		ret = sequencer_remove_state(&replay);
		replay_opts_release(&replay);
	} இல்லையெனில் {
		strbuf_addstr(&dir, opts->state_dir);
		எனில் (remove_dir_recursively(&dir, 0))
			ret = error(_("could not remove '%s'"),
				    opts->state_dir);
		strbuf_release(&dir);
	}

	திரும்பு ret;
}

நிலையான முழு move_to_original_branch(கட்டமைப்பு rebase_options *opts)
{
	கட்டமைப்பு strbuf branch_reflog = STRBUF_INIT, head_reflog = STRBUF_INIT;
	கட்டமைப்பு reset_working_மரம்_options ropts = { 0 };
	முழு ret;

	எனில் (!opts->head_name)
		திரும்பு 0; /* nothing to move back to */

	எனில் (!opts->onto)
		BUG("move_to_original_branch without onto");

	strbuf_addf(&branch_reflog, "%s (finish): %s onto %s",
		    opts->reflog_action,
		    opts->head_name, oid_to_hex(&opts->onto->object.oid));
	strbuf_addf(&head_reflog, "%s (finish): returning to %s",
		    opts->reflog_action, opts->head_name);
	ropts.branch = opts->head_name;
	ropts.flags = RESET_WORKING_மரம்_REFS_ONLY |
		      RESET_WORKING_மரம்_UPDATE_HEAD;
	ropts.branch_செய்தி = branch_reflog.buf;
	ropts.head_செய்தி = head_reflog.buf;
	ret = reset_working_மரம்(the_repository, &ropts);

	strbuf_release(&branch_reflog);
	strbuf_release(&head_reflog);
	திரும்பு ret;
}

நிலையான முழு run_am(கட்டமைப்பு rebase_options *opts)
{
	கட்டமைப்பு child_process am = CHILD_PROCESS_INIT;
	கட்டமைப்பு child_process format_patch = CHILD_PROCESS_INIT;
	முழு status;
	எழுத்து *rebased_patches;

	am.git_cmd = 1;
	strvec_தள்ளு(&am.args, "am");
	strvec_pushf(&am.env, GIT_REFLOG_ACTION_ENVIRONMENT "=%s (pick)",
		     opts->reflog_action);
	எனில் (opts->action == ACTION_CONTINUE) {
		strvec_தள்ளு(&am.args, "--resolved");
		strvec_pushf(&am.args, "--resolvemsg=%s", rebase_resolvemsg);
		எனில் (opts->gpg_sign_opt)
			strvec_தள்ளு(&am.args, opts->gpg_sign_opt);
		status = run_command(&am);
		எனில் (status)
			திரும்பு status;

		திரும்பு move_to_original_branch(opts);
	}
	எனில் (opts->action == ACTION_SKIP) {
		strvec_தள்ளு(&am.args, "--skip");
		strvec_pushf(&am.args, "--resolvemsg=%s", rebase_resolvemsg);
		status = run_command(&am);
		எனில் (status)
			திரும்பு status;

		திரும்பு move_to_original_branch(opts);
	}
	எனில் (opts->action == ACTION_SHOW_CURRENT_PATCH) {
		strvec_தள்ளு(&am.args, "--show-current-patch");
		திரும்பு run_command(&am);
	}

	rebased_patches = repo_git_path(the_repository, "rebased-patches");
	format_patch.out = open(rebased_patches,
				O_WRONLY | O_CREAT | O_TRUNC, 0666);
	எனில் (format_patch.out < 0) {
		status = error_errno(_("could not open '%s' for writing"),
				     rebased_patches);
		விடுவி(rebased_patches);
		child_process_clear(&am);
		திரும்பு status;
	}

	format_patch.git_cmd = 1;
	strvec_pushl(&format_patch.args, "format-patch", "-k", "--stdout",
		     "--full-index", "--cherry-pick", "--right-only",
		     "--default-prefix", "--no-renames",
		     "--no-cover-letter", "--pretty=mboxrd", "--topo-order",
		     "--no-base", வெற்று);
	எனில் (opts->git_format_patch_opt.len)
		strvec_split(&format_patch.args,
			     opts->git_format_patch_opt.buf);
	strvec_pushf(&format_patch.args, "%s...%s",
		     oid_to_hex(opts->வேர் ?
				/* this is now equivalent to !opts->upstream */
				&opts->onto->object.oid :
				&opts->upstream->object.oid),
		     oid_to_hex(&opts->orig_head->object.oid));
	எனில் (opts->restrict_revision)
		strvec_pushf(&format_patch.args, "^%s",
			     oid_to_hex(&opts->restrict_revision->object.oid));

	status = run_command(&format_patch);
	எனில் (status) {
		கட்டமைப்பு reset_working_மரம்_options ropts = { 0 };
		unlink(rebased_patches);
		விடுவி(rebased_patches);
		child_process_clear(&am);

		ropts.oid = &opts->orig_head->object.oid;
		ropts.branch = opts->head_name;
		ropts.default_reflog_action = opts->reflog_action;
		ropts.flags = RESET_WORKING_மரம்_UPDATE_HEAD;
		reset_working_மரம்(the_repository, &ropts);
		error(_("\ngit encountered an error while preparing the "
			"patches to replay\n"
			"these revisions:\n"
			"\n    %s\n\n"
			"As a result, git cannot rebase them."),
		      opts->revisions);

		திரும்பு status;
	}

	am.in = open(rebased_patches, O_RDONLY);
	எனில் (am.in < 0) {
		status = error_errno(_("could not open '%s' for reading"),
				     rebased_patches);
		விடுவி(rebased_patches);
		child_process_clear(&am);
		திரும்பு status;
	}

	strvec_pushv(&am.args, opts->git_am_opts.v);
	strvec_தள்ளு(&am.args, "--rebasing");
	strvec_pushf(&am.args, "--resolvemsg=%s", rebase_resolvemsg);
	strvec_தள்ளு(&am.args, "--patch-format=mboxrd");
	எனில் (opts->allow_rerere_autoupdate == RERERE_AUTOUPDATE)
		strvec_தள்ளு(&am.args, "--rerere-autoupdate");
	இல்லையெனில் எனில் (opts->allow_rerere_autoupdate == RERERE_NOAUTOUPDATE)
		strvec_தள்ளு(&am.args, "--no-rerere-autoupdate");
	எனில் (opts->gpg_sign_opt)
		strvec_தள்ளு(&am.args, opts->gpg_sign_opt);
	status = run_command(&am);
	unlink(rebased_patches);
	விடுவி(rebased_patches);

	எனில் (!status) {
		திரும்பு move_to_original_branch(opts);
	}

	எனில் (is_directory(opts->state_dir))
		rebase_write_basic_state(opts);

	திரும்பு status;
}

நிலையான முழு run_specific_rebase(கட்டமைப்பு rebase_options *opts)
{
	முழு status;

	எனில் (opts->type == REBASE_MERGE) {
		/* Run sequencer-based rebase */
		எனில் (!(opts->flags & REBASE_INTERACTIVE_EXPLICIT))
			setenv("GIT_SEQUENCE_EDITOR", ":", 1);
		எனில் (opts->gpg_sign_opt) {
			/* remove the leading "-S" */
			எழுத்து *tmp = xstrdup(opts->gpg_sign_opt + 2);
			விடுவி(opts->gpg_sign_opt);
			opts->gpg_sign_opt = tmp;
		}

		status = run_sequencer_rebase(opts);
	} இல்லையெனில் எனில் (opts->type == REBASE_APPLY)
		status = run_am(opts);
	இல்லையெனில்
		BUG("Unhandled rebase type %d", opts->type);

	எனில் (opts->dont_finish_rebase)
		; /* do nothing */
	இல்லையெனில் எனில் (opts->type == REBASE_MERGE) {
		முழு quiet = !(opts->flags & (REBASE_NO_QUIET|REBASE_VERBOSE));

		/*
		 * The sequencer cleans up after itself. Its state directory
		 * is gone once it is done, and stays while it is stopped.
		 */
		எனில் (status == 0 && !is_directory(opts->state_dir))
			run_auto_maintenance(the_repository, quiet);
	} இல்லையெனில் எனில் (status == 0) {
		எனில் (!file_exists(state_dir_path("stopped-sha", opts)))
			finish_rebase(opts);
	} இல்லையெனில் எனில் (status == 2) {
		கட்டமைப்பு strbuf dir = STRBUF_INIT;

		apply_autostash(state_dir_path("autostash", opts));
		strbuf_addstr(&dir, opts->state_dir);
		remove_dir_recursively(&dir, 0);
		strbuf_release(&dir);
		die("Nothing to do");
	}

	திரும்பு status ? -1 : 0;
}

நிலையான வெறுமை parse_rebase_merges_மதிப்பு(கட்டமைப்பு rebase_options *options, மாறிலி எழுத்து *மதிப்பு)
{
	எனில் (!strcmp("no-rebase-cousins", மதிப்பு))
		options->rebase_cousins = 0;
	இல்லையெனில் எனில் (!strcmp("rebase-cousins", மதிப்பு))
		options->rebase_cousins = 1;
	இல்லையெனில்
		die(_("Unknown rebase-merges mode: %s"), மதிப்பு);
}

நிலையான முழு rebase_config(மாறிலி எழுத்து *var, மாறிலி எழுத்து *மதிப்பு,
			 மாறிலி கட்டமைப்பு config_context *ctx, வெறுமை *data)
{
	கட்டமைப்பு rebase_options *opts = data;

	எனில் (!strcmp(var, "rebase.stat")) {
		எனில் (git_config_bool(var, மதிப்பு))
			opts->flags |= REBASE_DIFFSTAT;
		இல்லையெனில்
			opts->flags &= ~REBASE_DIFFSTAT;
		திரும்பு 0;
	}

	எனில் (!strcmp(var, "rebase.autosquash")) {
		opts->config_autosquash = git_config_bool(var, மதிப்பு);
		திரும்பு 0;
	}

	எனில் (!strcmp(var, "commit.gpgsign")) {
		விடுவி(opts->gpg_sign_opt);
		opts->gpg_sign_opt = git_config_bool(var, மதிப்பு) ?
			xstrdup("-S") : வெற்று;
		திரும்பு 0;
	}

	எனில் (!strcmp(var, "rebase.autostash")) {
		opts->autostash = git_config_bool(var, மதிப்பு);
		திரும்பு 0;
	}

	எனில் (!strcmp(var, "rebase.rebasemerges")) {
		opts->config_rebase_merges = git_parse_maybe_bool(மதிப்பு);
		எனில் (opts->config_rebase_merges < 0) {
			opts->config_rebase_merges = 1;
			parse_rebase_merges_மதிப்பு(opts, மதிப்பு);
		} இல்லையெனில் {
			opts->rebase_cousins = 0;
		}
		திரும்பு 0;
	}

	எனில் (!strcmp(var, "rebase.updaterefs")) {
		opts->config_update_refs = git_config_bool(var, மதிப்பு);
		திரும்பு 0;
	}

	எனில் (!strcmp(var, "rebase.reschedulefailedexec")) {
		opts->reschedule_failed_exec = git_config_bool(var, மதிப்பு);
		திரும்பு 0;
	}

	எனில் (!strcmp(var, "rebase.forkpoint")) {
		opts->fork_point = git_config_bool(var, மதிப்பு) ? -1 : 0;
		திரும்பு 0;
	}

	எனில் (!strcmp(var, "rebase.backend")) {
		FREE_AND_NULL(opts->default_backend);
		திரும்பு git_config_string(&opts->default_backend, var, மதிப்பு);
	}

	திரும்பு git_default_config(var, மதிப்பு, ctx, data);
}

நிலையான முழு checkout_up_to_date(கட்டமைப்பு rebase_options *options)
{
	கட்டமைப்பு strbuf buf = STRBUF_INIT;
	கட்டமைப்பு reset_working_மரம்_options ropts = { 0 };
	முழு ret = 0;

	strbuf_addf(&buf, "%s: checkout %s",
		    options->reflog_action, options->switch_to);
	ropts.oid = &options->orig_head->object.oid;
	ropts.branch = options->head_name;
	ropts.flags = RESET_WORKING_மரம்_RUN_POST_CHECKOUT_HOOK |
		      RESET_WORKING_மரம்_UPDATE_HEAD;
	எனில் (!ropts.branch)
		ropts.flags |=  RESET_WORKING_மரம்_DETACH;
	ropts.head_செய்தி = buf.buf;
	எனில் (reset_working_மரம்(the_repository, &ropts) < 0)
		ret = error(_("could not switch to %s"), options->switch_to);
	strbuf_release(&buf);

	திரும்பு ret;
}

/*
 * Determines whether the commits in from..to are linear, i.e. contain
 * no merge commits. This function *expects* `from` to be an ancestor of
 * `to`.
 */
நிலையான முழு is_linear_history(கட்டமைப்பு commit *from, கட்டமைப்பு commit *to)
{
	போது (to && to != from) {
		repo_parse_commit(the_repository, to);
		எனில் (!to->parents)
			திரும்பு 1;
		எனில் (to->parents->next)
			திரும்பு 0;
		to = to->parents->item;
	}
	திரும்பு 1;
}

நிலையான முழு can_fast_forward(கட்டமைப்பு commit *onto, கட்டமைப்பு commit *upstream,
			    கட்டமைப்பு commit *restrict_revision,
			    கட்டமைப்பு commit *head, கட்டமைப்பு object_id *branch_base)
{
	கட்டமைப்பு commit_list *merge_bases = வெற்று;
	முழு res = 0;

	எனில் (is_null_oid(branch_base))
		செல் done; /* fill_branch_base() found multiple merge bases */

	எனில் (!oideq(branch_base, &onto->object.oid))
		செல் done;

	எனில் (restrict_revision && !oideq(&restrict_revision->object.oid, branch_base))
		செல் done;

	எனில் (!upstream)
		செல் done;

	எனில் (repo_get_merge_bases(the_repository, upstream, head, &merge_bases) < 0)
		exit(128);
	எனில் (!merge_bases || merge_bases->next)
		செல் done;

	எனில் (!oideq(&onto->object.oid, &merge_bases->item->object.oid))
		செல் done;

	res = 1;

done:
	commit_list_free(merge_bases);
	திரும்பு res && is_linear_history(onto, head);
}

நிலையான வெறுமை fill_branch_base(கட்டமைப்பு rebase_options *options,
			    கட்டமைப்பு object_id *branch_base)
{
	கட்டமைப்பு commit_list *merge_bases = வெற்று;

	எனில் (repo_get_merge_bases(the_repository, options->onto,
				 options->orig_head, &merge_bases) < 0)
		exit(128);
	எனில் (!merge_bases || merge_bases->next)
		oidcpy(branch_base, null_oid(the_hash_algo));
	இல்லையெனில்
		oidcpy(branch_base, &merge_bases->item->object.oid);

	commit_list_free(merge_bases);
}

நிலையான முழு parse_opt_am(மாறிலி கட்டமைப்பு option *opt, மாறிலி எழுத்து *arg, முழு unset)
{
	கட்டமைப்பு rebase_options *opts = opt->மதிப்பு;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	எனில் (opts->type != REBASE_UNSPECIFIED && opts->type != REBASE_APPLY)
	    die(_("apply options and merge options cannot be used together"));

	opts->type = REBASE_APPLY;

	திரும்பு 0;
}

/* -i followed by -m is still -i */
நிலையான முழு parse_opt_merge(மாறிலி கட்டமைப்பு option *opt, மாறிலி எழுத்து *arg, முழு unset)
{
	கட்டமைப்பு rebase_options *opts = opt->மதிப்பு;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	எனில் (opts->type != REBASE_UNSPECIFIED && opts->type != REBASE_MERGE)
	    die(_("apply options and merge options cannot be used together"));

	opts->type = REBASE_MERGE;

	திரும்பு 0;
}

/* -i followed by -r is still explicitly interactive, but -r alone is not */
நிலையான முழு parse_opt_interactive(மாறிலி கட்டமைப்பு option *opt, மாறிலி எழுத்து *arg,
				 முழு unset)
{
	கட்டமைப்பு rebase_options *opts = opt->மதிப்பு;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	எனில் (opts->type != REBASE_UNSPECIFIED && opts->type != REBASE_MERGE)
	    die(_("apply options and merge options cannot be used together"));

	opts->type = REBASE_MERGE;
	opts->flags |= REBASE_INTERACTIVE_EXPLICIT;

	திரும்பு 0;
}

நிலையான எண்ணிக்கை empty_type parse_empty_மதிப்பு(மாறிலி எழுத்து *மதிப்பு)
{
	எனில் (!strcasecmp(மதிப்பு, "drop"))
		திரும்பு EMPTY_DROP;
	இல்லையெனில் எனில் (!strcasecmp(மதிப்பு, "keep"))
		திரும்பு EMPTY_KEEP;
	இல்லையெனில் எனில் (!strcasecmp(மதிப்பு, "stop"))
		திரும்பு EMPTY_STOP;
	இல்லையெனில் எனில் (!strcasecmp(மதிப்பு, "ask")) {
		warning(_("--empty=ask is deprecated; use '--empty=stop' instead."));
		திரும்பு EMPTY_STOP;
	}

	die(_("unrecognized empty type '%s'; valid values are \"drop\", \"keep\", and \"stop\"."), மதிப்பு);
}

நிலையான முழு parse_opt_keep_empty(மாறிலி கட்டமைப்பு option *opt, மாறிலி எழுத்து *arg,
				முழு unset)
{
	கட்டமைப்பு rebase_options *opts = opt->மதிப்பு;

	BUG_ON_OPT_ARG(arg);

	imply_merge(opts, unset ? "--no-keep-empty" : "--keep-empty");
	opts->keep_empty = !unset;
	திரும்பு 0;
}

நிலையான முழு parse_opt_empty(மாறிலி கட்டமைப்பு option *opt, மாறிலி எழுத்து *arg, முழு unset)
{
	கட்டமைப்பு rebase_options *options = opt->மதிப்பு;
	எண்ணிக்கை empty_type மதிப்பு = parse_empty_மதிப்பு(arg);

	BUG_ON_OPT_NEG(unset);

	options->empty = மதிப்பு;
	திரும்பு 0;
}

நிலையான முழு parse_opt_rebase_merges(மாறிலி கட்டமைப்பு option *opt, மாறிலி எழுத்து *arg, முழு unset)
{
	கட்டமைப்பு rebase_options *options = opt->மதிப்பு;

	options->rebase_merges = !unset;
	options->rebase_cousins = 0;

	எனில் (arg) {
		எனில் (!*arg) {
			warning(_("--rebase-merges with an empty string "
				  "argument is deprecated and will stop "
				  "working in a future version of Git. Use "
				  "--rebase-merges without an argument "
				  "instead, which does the same thing."));
			திரும்பு 0;
		}
		parse_rebase_merges_மதிப்பு(options, arg);
	}

	திரும்பு 0;
}

நிலையான வெறுமை NORETURN error_on_missing_default_upstream(வெறுமை)
{
	கட்டமைப்பு branch *current_branch = branch_get(வெற்று);

	printf(_("%s\n"
		 "Please specify which branch you want to rebase against.\n"
		 "See git-rebase(1) for details.\n"
		 "\n"
		 "    git rebase '<branch>'\n"
		 "\n"),
		current_branch ? _("There is no tracking information for "
			"the current branch.") :
			_("You are not currently on a branch."));

	எனில் (current_branch) {
		மாறிலி எழுத்து *remote = current_branch->remote_name;

		எனில் (!remote)
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

நிலையான முழு check_exec_cmd(மாறிலி எழுத்து *cmd)
{
	எனில் (strchr(cmd, '\n'))
		திரும்பு error(_("exec commands cannot contain newlines"));

	/* Does the command consist purely of whitespace? */
	எனில் (!cmd[strspn(cmd, " \t\r\f\v")])
		திரும்பு error(_("empty exec command"));

	திரும்பு 0;
}

முழு cmd_rebase(முழு argc,
	       மாறிலி எழுத்து **argv,
	       மாறிலி எழுத்து *prefix,
	       கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு rebase_options options = REBASE_OPTIONS_INIT;
	மாறிலி எழுத்து *branch_name;
	மாறிலி எழுத்து *strategy_opt = வெற்று;
	முழு ret, flags, total_argc, in_progress = 0;
	முழு keep_base = 0;
	முழு ok_to_skip_pre_rebase = 0;
	கட்டமைப்பு strbuf செய்தி = STRBUF_INIT;
	கட்டமைப்பு strbuf revisions = STRBUF_INIT;
	கட்டமைப்பு strbuf buf = STRBUF_INIT;
	கட்டமைப்பு object_id branch_base;
	முழு ignore_whitespace = 0;
	மாறிலி எழுத்து *gpg_sign = வெற்று;
	கட்டமைப்பு object_id squash_onto;
	எழுத்து *squash_onto_name = வெற்று;
	எழுத்து *keep_base_onto_name = வெற்று;
	முழு reschedule_failed_exec = -1;
	முழு allow_preemptive_ff = 1;
	முழு preserve_merges_selected = 0;
	கட்டமைப்பு reset_working_மரம்_options ropts = { 0 };
	கட்டமைப்பு option builtin_rebase_options[] = {
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
			.மதிப்பு = &options.flags,
			.precision = அளவு(options.flags),
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
		OPT_PASSTHRU_ARGV('C', வெற்று, &options.git_am_opts, N_("n"),
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
		OPT_CALLBACK_F(0, "apply", &options, வெற்று,
			N_("use apply strategies to rebase"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			parse_opt_am),
		OPT_CALLBACK_F('m', "merge", &options, வெற்று,
			N_("use merging strategies to rebase"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			parse_opt_merge),
		OPT_CALLBACK_F('i', "interactive", &options, வெற்று,
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
		OPT_CALLBACK_F('k', "keep-empty", &options, வெற்று,
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
			.மதிப்பு = &gpg_sign,
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
		OPT_BOOL(0, "root", &options.வேர்,
			 N_("rebase all reachable commits up to the root(s)")),
		OPT_BOOL(0, "reschedule-failed-exec",
			 &reschedule_failed_exec,
			 N_("automatically re-schedule any `exec` that fails")),
		OPT_BOOL(0, "reapply-cherry-picks", &options.reapply_cherry_picks,
			 N_("apply all changes, even those already present upstream")),
		OPT_END(),
	};
	முழு i;

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
	gpg_sign = options.gpg_sign_opt ? "" : வெற்று;
	FREE_AND_NULL(options.gpg_sign_opt);

	strbuf_reset(&buf);
	strbuf_addf(&buf, "%s/applying", apply_dir());
	எனில்(file_exists(buf.buf))
		die(_("It looks like 'git am' is in progress. Cannot rebase."));

	எனில் (is_directory(apply_dir())) {
		options.type = REBASE_APPLY;
		options.state_dir = apply_dir();
	} இல்லையெனில் எனில் (is_directory(merge_dir())) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "%s/rewritten", merge_dir());
		எனில் (!(options.action == ACTION_ABORT) && is_directory(buf.buf)) {
			die(_("`rebase --preserve-merges` (-p) is no longer supported.\n"
			"Use `git rebase --abort` to terminate current rebase.\n"
			"Or downgrade to v2.33, or earlier, to complete the rebase."));
		} இல்லையெனில் {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "%s/interactive", merge_dir());
			options.type = REBASE_MERGE;
			எனில் (file_exists(buf.buf))
				options.flags |= REBASE_INTERACTIVE_EXPLICIT;
		}
		options.state_dir = merge_dir();
	}

	எனில் (options.type != REBASE_UNSPECIFIED)
		in_progress = 1;

	total_argc = argc;
	argc = parse_options(argc, argv, prefix,
			     builtin_rebase_options,
			     builtin_rebase_usage, 0);

	எனில் (options.trailer_args.nr) {
		எனில் (validate_trailer_args(&options.trailer_args))
			die(வெற்று);
		options.flags |= REBASE_FORCE;
	}

	எனில் (preserve_merges_selected)
		die(_("--preserve-merges was replaced by --rebase-merges\n"
			"Note: Your `pull.rebase` configuration may also be set to 'preserve',\n"
			"which is no longer supported; use 'merges' instead"));

	எனில் (options.action != ACTION_NONE && total_argc != 2) {
		usage_with_options(builtin_rebase_usage,
				   builtin_rebase_options);
	}

	எனில் (argc > 2)
		usage_with_options(builtin_rebase_usage,
				   builtin_rebase_options);

	எனில் (keep_base) {
		எனில் (options.onto_name)
			die(_("options '%s' and '%s' cannot be used together"), "--keep-base", "--onto");
		எனில் (options.வேர்)
			die(_("options '%s' and '%s' cannot be used together"), "--keep-base", "--root");
		/*
		 * --keep-base defaults to --no-fork-point to keep the
		 * base the same.
		 */
		எனில் (options.fork_point < 0)
			options.fork_point = 0;
	}
	எனில் (options.வேர் && options.fork_point > 0)
		die(_("options '%s' and '%s' cannot be used together"), "--root", "--fork-point");

	எனில் (options.action != ACTION_NONE && !in_progress)
		die(_("no rebase in progress"));

	எனில் (options.action == ACTION_EDIT_TODO && !is_merge(&options))
		die(_("The --edit-todo action can only be used during "
		      "interactive rebase."));

	எனில் (trace2_is_enabled()) {
		எனில் (is_merge(&options))
			trace2_cmd_mode("interactive");
		இல்லையெனில் எனில் (options.exec.nr)
			trace2_cmd_mode("interactive-exec");
		இல்லையெனில்
			trace2_cmd_mode(action_names[options.action]);
	}

	options.reflog_action = getenv(GIT_REFLOG_ACTION_ENVIRONMENT);
	options.reflog_action =
		xstrdup(options.reflog_action ? options.reflog_action : "rebase");

	தேர்வு (options.action) {
	நிலை ACTION_CONTINUE: {
		கட்டமைப்பு object_id head;
		கட்டமைப்பு lock_file lock_file = LOCK_INIT;
		முழு fd;

		/* Sanity check */
		எனில் (repo_get_oid(the_repository, "HEAD", &head))
			die(_("Cannot read HEAD"));

		fd = repo_hold_locked_index(the_repository, &lock_file, 0);
		எனில் (repo_read_index(the_repository) < 0)
			die(_("could not read index"));
		refresh_index(the_repository->index, REFRESH_QUIET, வெற்று, வெற்று,
			      வெற்று);
		எனில் (0 <= fd)
			repo_update_index_if_able(the_repository, &lock_file);
		rollback_lock_file(&lock_file);

		எனில் (has_unstaged_changes(the_repository, 1)) {
			puts(_("You must edit all merge conflicts and then\n"
			       "mark them as resolved using git add"));
			exit(1);
		}
		எனில் (read_basic_state(&options))
			exit(1);
		செல் run_rebase;
	}
	நிலை ACTION_SKIP: {
		கட்டமைப்பு string_list merge_rr = STRING_LIST_INIT_DUP;

		rerere_clear(the_repository, &merge_rr);
		string_list_clear(&merge_rr, 1);
		ropts.flags = RESET_WORKING_மரம்_HARD |
			      RESET_WORKING_மரம்_UPDATE_HEAD;
		எனில் (reset_working_மரம்(the_repository, &ropts) < 0)
			die(_("could not discard worktree changes"));
		remove_branch_state(the_repository, 0);
		எனில் (read_basic_state(&options))
			exit(1);
		செல் run_rebase;
	}
	நிலை ACTION_ABORT: {
		கட்டமைப்பு string_list merge_rr = STRING_LIST_INIT_DUP;
		கட்டமைப்பு strbuf head_செய்தி = STRBUF_INIT;

		rerere_clear(the_repository, &merge_rr);
		string_list_clear(&merge_rr, 1);

		எனில் (read_basic_state(&options))
			exit(1);

		strbuf_addf(&head_செய்தி, "%s (abort): returning to %s",
			    options.reflog_action,
			    options.head_name ? options.head_name
					      : oid_to_hex(&options.orig_head->object.oid));
		ropts.oid = &options.orig_head->object.oid;
		ropts.head_செய்தி = head_செய்தி.buf;
		ropts.branch = options.head_name;
		ropts.flags = RESET_WORKING_மரம்_HARD |
			      RESET_WORKING_மரம்_UPDATE_HEAD;
		எனில் (reset_working_மரம்(the_repository, &ropts) < 0)
			die(_("could not move back to %s"),
			    oid_to_hex(&options.orig_head->object.oid));
		strbuf_release(&head_செய்தி);
		remove_branch_state(the_repository, 0);
		ret = finish_rebase(&options);
		செல் cleanup;
	}
	நிலை ACTION_QUIT: {
		save_autostash(state_dir_path("autostash", &options));
		எனில் (options.type == REBASE_MERGE) {
			கட்டமைப்பு replay_opts replay = REPLAY_OPTS_INIT;

			replay.action = REPLAY_INTERACTIVE_REBASE;
			ret = sequencer_remove_state(&replay);
			replay_opts_release(&replay);
		} இல்லையெனில் {
			strbuf_reset(&buf);
			strbuf_addstr(&buf, options.state_dir);
			ret = remove_dir_recursively(&buf, 0);
			எனில் (ret)
				error(_("could not remove '%s'"),
				       options.state_dir);
		}
		செல் cleanup;
	}
	நிலை ACTION_EDIT_TODO:
		options.dont_finish_rebase = 1;
		செல் run_rebase;
	நிலை ACTION_SHOW_CURRENT_PATCH:
		options.dont_finish_rebase = 1;
		செல் run_rebase;
	நிலை ACTION_NONE:
		நிறுத்து;
	இயல்பு:
		BUG("action: %d", options.action);
	}

	/* Make sure no rebase is in progress */
	எனில் (in_progress) {
		மாறிலி எழுத்து *last_slash = strrchr(options.state_dir, '/');
		மாறிலி எழுத்து *state_dir_base =
			last_slash ? last_slash + 1 : options.state_dir;
		மாறிலி எழுத்து *cmd_live_rebase =
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

	எனில் ((options.flags & REBASE_INTERACTIVE_EXPLICIT) ||
	    (options.action != ACTION_NONE) ||
	    (options.exec.nr > 0) ||
	    options.autosquash == 1) {
		allow_preemptive_ff = 0;
	}
	எனில் (options.committer_date_is_author_date || options.ignore_date)
		options.flags |= REBASE_FORCE;

	க்கு (i = 0; i < options.git_am_opts.nr; i++) {
		மாறிலி எழுத்து *option = options.git_am_opts.v[i], *p;
		எனில் (!strcmp(option, "--whitespace=fix") ||
		    !strcmp(option, "--whitespace=strip"))
			allow_preemptive_ff = 0;
		இல்லையெனில் எனில் (skip_prefix(option, "-C", &p)) {
			போது (*p)
				எனில் (!isdigit(*(p++)))
					die(_("switch `C' expects a "
					      "numerical value"));
		} இல்லையெனில் எனில் (skip_prefix(option, "--whitespace=", &p)) {
			எனில் (*p && strcmp(p, "warn") && strcmp(p, "nowarn") &&
			    strcmp(p, "error") && strcmp(p, "error-all"))
				die("Invalid whitespace option: '%s'", p);
		}
	}

	க்கு (i = 0; i < options.exec.nr; i++)
		எனில் (check_exec_cmd(options.exec.பொருட்கள்[i].string))
			exit(1);

	எனில் (!(options.flags & REBASE_NO_QUIET))
		strvec_தள்ளு(&options.git_am_opts, "-q");

	எனில் (options.empty != EMPTY_UNSPECIFIED)
		imply_merge(&options, "--empty");

	எனில் (options.reapply_cherry_picks < 0)
		/*
		 * We default to --no-reapply-cherry-picks unless
		 * --keep-base is given; when --keep-base is given, we want
		 * to default to --reapply-cherry-picks.
		 */
		options.reapply_cherry_picks = keep_base;
	இல்லையெனில் எனில் (!keep_base)
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

	எனில் (gpg_sign)
		options.gpg_sign_opt = xstrfmt("-S%s", gpg_sign);

	எனில் (options.exec.nr)
		imply_merge(&options, "--exec");

	எனில் (options.type == REBASE_APPLY) {
		எனில் (ignore_whitespace)
			strvec_தள்ளு(&options.git_am_opts,
				    "--ignore-whitespace");
		எனில் (options.committer_date_is_author_date)
			strvec_தள்ளு(&options.git_am_opts,
				    "--committer-date-is-author-date");
		எனில் (options.ignore_date)
			strvec_தள்ளு(&options.git_am_opts, "--ignore-date");
	} இல்லையெனில் {
		/* REBASE_MERGE */
		எனில் (ignore_whitespace) {
			string_list_append(&options.strategy_opts,
					   "ignore-space-change");
		}
	}

	எனில் (strategy_opt)
		options.strategy = xstrdup(strategy_opt);
	இல்லையெனில் எனில் (options.strategy_opts.nr && !options.strategy)
		options.strategy = xstrdup("ort");
	எனில் (options.strategy)
		imply_merge(&options, "--strategy");

	எனில் (options.வேர் && !options.onto_name)
		imply_merge(&options, "--root without --onto");

	எனில் (options.trailer_args.nr)
		imply_merge(&options, "--trailer");

	எனில் (isatty(2) && options.flags & REBASE_NO_QUIET)
		strbuf_addstr(&options.git_format_patch_opt, " --progress");

	எனில் (options.git_am_opts.nr || options.type == REBASE_APPLY) {
		/* all am options except -q are compatible only with --apply */
		க்கு (i = options.git_am_opts.nr - 1; i >= 0; i--)
			எனில் (strcmp(options.git_am_opts.v[i], "-q"))
				நிறுத்து;

		எனில் (i >= 0 || options.type == REBASE_APPLY) {
			எனில் (is_merge(&options))
				die(_("apply options and merge options "
					  "cannot be used together"));
			இல்லையெனில் எனில் (options.rebase_merges == -1 && options.config_rebase_merges == 1)
				die(_("apply options are incompatible with rebase.rebaseMerges.  Consider adding --no-rebase-merges"));
			இல்லையெனில் எனில் (options.update_refs == -1 && options.config_update_refs == 1)
				die(_("apply options are incompatible with rebase.updateRefs.  Consider adding --no-update-refs"));
			இல்லையெனில்
				options.type = REBASE_APPLY;
		}
	}

	எனில் (options.update_refs == 1)
		imply_merge(&options, "--update-refs");
	options.update_refs = (options.update_refs >= 0) ? options.update_refs :
			     ((options.config_update_refs >= 0) ? options.config_update_refs : 0);

	எனில் (options.rebase_merges == 1)
		imply_merge(&options, "--rebase-merges");
	options.rebase_merges = (options.rebase_merges >= 0) ? options.rebase_merges :
				((options.config_rebase_merges >= 0) ? options.config_rebase_merges : 0);

	எனில் (options.autosquash == 1) {
		imply_merge(&options, "--autosquash");
	} இல்லையெனில் எனில் (options.autosquash == -1) {
		options.autosquash =
			options.config_autosquash &&
			(options.flags & REBASE_INTERACTIVE_EXPLICIT);
	}

	எனில் (options.type == REBASE_UNSPECIFIED) {
		எனில் (!strcmp(options.default_backend, "merge"))
			options.type = REBASE_MERGE;
		இல்லையெனில் எனில் (!strcmp(options.default_backend, "apply"))
			options.type = REBASE_APPLY;
		இல்லையெனில்
			die(_("Unknown rebase backend: %s"),
			    options.default_backend);
	}

	தேர்வு (options.type) {
	நிலை REBASE_MERGE:
		options.state_dir = merge_dir();
		நிறுத்து;
	நிலை REBASE_APPLY:
		options.state_dir = apply_dir();
		நிறுத்து;
	இயல்பு:
		BUG("options.type was just set above; should be unreachable.");
	}

	எனில் (options.empty == EMPTY_UNSPECIFIED) {
		எனில் (options.flags & REBASE_INTERACTIVE_EXPLICIT)
			options.empty = EMPTY_STOP;
		இல்லையெனில் எனில் (options.exec.nr > 0)
			options.empty = EMPTY_KEEP;
		இல்லையெனில்
			options.empty = EMPTY_DROP;
	}
	எனில் (reschedule_failed_exec > 0 && !is_merge(&options))
		die(_("--reschedule-failed-exec requires "
		      "--exec or --interactive"));
	எனில் (reschedule_failed_exec >= 0)
		options.reschedule_failed_exec = reschedule_failed_exec;

	எனில் (options.signoff) {
		strvec_தள்ளு(&options.git_am_opts, "--signoff");
		options.flags |= REBASE_FORCE;
	}

	எனில் (!options.வேர்) {
		எனில் (argc < 1) {
			கட்டமைப்பு branch *branch;

			branch = branch_get(வெற்று);
			options.upstream_name = branch_get_upstream(branch,
								    வெற்று);
			எனில் (!options.upstream_name)
				error_on_missing_default_upstream();
			எனில் (options.fork_point < 0)
				options.fork_point = 1;
		} இல்லையெனில் {
			options.upstream_name = argv[0];
			argc--;
			argv++;
			எனில் (!strcmp(options.upstream_name, "-"))
				options.upstream_name = "@{-1}";
		}
		options.upstream =
			lookup_commit_reference_by_name(options.upstream_name);
		எனில் (!options.upstream)
			die(_("invalid upstream '%s'"), options.upstream_name);
		options.upstream_arg = options.upstream_name;
	} இல்லையெனில் {
		எனில் (!options.onto_name) {
			எனில் (commit_மரம்("", 0, the_hash_algo->empty_மரம், வெற்று,
					&squash_onto, வெற்று, வெற்று) < 0)
				die(_("Could not create new root commit"));
			options.squash_onto = &squash_onto;
			options.onto_name = squash_onto_name =
				xstrdup(oid_to_hex(&squash_onto));
		} இல்லையெனில்
			options.வேர்_with_onto = 1;

		options.upstream_name = வெற்று;
		options.upstream = வெற்று;
		எனில் (argc > 1)
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
	எனில் (argc == 1) {
		/* Is it "rebase other branchname" or "rebase other commit"? */
		கட்டமைப்பு object_id branch_oid;
		branch_name = argv[0];
		options.switch_to = argv[0];

		/* Is it a local branch? */
		strbuf_reset(&buf);
		strbuf_addf(&buf, "refs/heads/%s", branch_name);
		எனில் (!refs_read_ref(get_main_ref_store(the_repository), buf.buf, &branch_oid)) {
			die_if_checked_out(buf.buf, 1);
			options.head_name = xstrdup(buf.buf);
			options.orig_head =
				lookup_commit_object(the_repository,
						     &branch_oid);
		/* If not is it a valid ref (branch or commit)? */
		} இல்லையெனில் {
			options.orig_head =
				lookup_commit_reference_by_name(branch_name);
			options.head_name = வெற்று;
		}
		எனில் (!options.orig_head)
			die(_("no such branch/commit '%s'"), branch_name);
	} இல்லையெனில் எனில் (argc == 0) {
		/* Do not need to switch branches, we are already on it. */
		options.head_name =
			xstrdup_or_null(refs_resolve_ref_unsafe(get_main_ref_store(the_repository), "HEAD", 0, வெற்று,
								&flags));
		எனில் (!options.head_name)
			die(_("No such ref: %s"), "HEAD");
		எனில் (flags & REF_ISSYMREF) {
			எனில் (!skip_prefix(options.head_name,
					 "refs/heads/", &branch_name))
				branch_name = options.head_name;

		} இல்லையெனில் {
			FREE_AND_NULL(options.head_name);
			branch_name = "HEAD";
		}
		options.orig_head = lookup_commit_reference_by_name("HEAD");
		எனில் (!options.orig_head)
			die(_("Could not resolve HEAD to a commit"));
	} இல்லையெனில்
		BUG("unexpected number of arguments left to parse");

	/* Make sure the branch to rebase onto is valid. */
	எனில் (keep_base) {
		strbuf_reset(&buf);
		strbuf_addstr(&buf, options.upstream_name);
		strbuf_addstr(&buf, "...");
		strbuf_addstr(&buf, branch_name);
		options.onto_name = keep_base_onto_name = xstrdup(buf.buf);
	} இல்லையெனில் எனில் (!options.onto_name)
		options.onto_name = options.upstream_name;
	எனில் (strstr(options.onto_name, "...")) {
		எனில் (repo_get_oid_mb(the_repository, options.onto_name, &branch_base) < 0) {
			எனில் (keep_base)
				die(_("'%s': need exactly one merge base with branch"),
				    options.upstream_name);
			இல்லையெனில்
				die(_("'%s': need exactly one merge base"),
				    options.onto_name);
		}
		options.onto = lookup_commit_or_die(&branch_base,
						    options.onto_name);
	} இல்லையெனில் {
		options.onto =
			lookup_commit_reference_by_name(options.onto_name);
		எனில் (!options.onto)
			die(_("Does not point to a valid commit '%s'"),
				options.onto_name);
		fill_branch_base(&options, &branch_base);
	}

	எனில் (keep_base && options.reapply_cherry_picks)
		options.upstream = options.onto;

	எனில் (options.fork_point > 0)
		options.restrict_revision =
			get_fork_point(options.upstream_name, options.orig_head);

	எனில் (repo_read_index(the_repository) < 0)
		die(_("could not read index"));

	எனில் (options.autostash)
		create_autostash(the_repository,
				 state_dir_path("autostash", &options));


	எனில் (require_clean_work_மரம்(the_repository, "rebase",
				    _("Please commit or stash them."), 1, 1)) {
		ret = -1;
		செல் cleanup_autostash;
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
	எனில் (allow_preemptive_ff &&
	    can_fast_forward(options.onto, options.upstream, options.restrict_revision,
			     options.orig_head, &branch_base)) {
		முழு flag;

		எனில் (!(options.flags & REBASE_FORCE)) {
			/* Lazily switch to the target branch if needed... */
			எனில் (options.switch_to) {
				ret = checkout_up_to_date(&options);
				எனில் (ret)
					செல் cleanup_autostash;
			}

			எனில் (!(options.flags & REBASE_NO_QUIET))
				; /* be quiet */
			இல்லையெனில் எனில் (!strcmp(branch_name, "HEAD") &&
				 refs_resolve_ref_unsafe(get_main_ref_store(the_repository), "HEAD", 0, வெற்று, &flag))
				puts(_("HEAD is up to date."));
			இல்லையெனில்
				printf(_("Current branch %s is up to date.\n"),
				       branch_name);
			ret = finish_rebase(&options);
			செல் cleanup;
		} இல்லையெனில் எனில் (!(options.flags & REBASE_NO_QUIET))
			; /* be quiet */
		இல்லையெனில் எனில் (!strcmp(branch_name, "HEAD") &&
			 refs_resolve_ref_unsafe(get_main_ref_store(the_repository), "HEAD", 0, வெற்று, &flag))
			puts(_("HEAD is up to date, rebase forced."));
		இல்லையெனில்
			printf(_("Current branch %s is up to date, rebase "
				 "forced.\n"), branch_name);
	}

	/* If a hook exists, give it a chance to interrupt*/
	எனில் (!ok_to_skip_pre_rebase &&
	    run_hooks_l(the_repository, "pre-rebase", options.upstream_arg,
			argc ? argv[0] : வெற்று, வெற்று)) {
		ret = error(_("The pre-rebase hook refused to rebase."));
		செல் cleanup_autostash;
	}

	எனில் (options.flags & REBASE_DIFFSTAT) {
		கட்டமைப்பு diff_options opts;

		எனில் (options.flags & REBASE_VERBOSE) {
			எனில் (is_null_oid(&branch_base))
				printf(_("Changes to %s:\n"),
				       oid_to_hex(&options.onto->object.oid));
			இல்லையெனில்
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
		diff_மரம்_oid(is_null_oid(&branch_base) ?
			      the_hash_algo->empty_மரம் : &branch_base,
			      &options.onto->object.oid, "", &opts);
		diffcore_std(&opts);
		diff_flush(&opts);
	}

	எனில் (is_merge(&options))
		செல் run_rebase;

	/* Detach HEAD and reset the tree */
	எனில் (options.flags & REBASE_NO_QUIET)
		printf(_("First, rewinding head to replay your work on top of "
			 "it...\n"));

	strbuf_addf(&செய்தி, "%s (start): checkout %s",
		    options.reflog_action, options.onto_name);
	ropts.oid = &options.onto->object.oid;
	ropts.orig_head = &options.orig_head->object.oid;
	ropts.flags = RESET_WORKING_மரம்_DETACH |
		      RESET_WORKING_மரம்_UPDATE_HEAD |
		      RESET_WORKING_மரம்_UPDATE_ORIG_HEAD |
		      RESET_WORKING_மரம்_RUN_POST_CHECKOUT_HOOK;
	ropts.head_செய்தி = செய்தி.buf;
	ropts.default_reflog_action = options.reflog_action;
	எனில் (reset_working_மரம்(the_repository, &ropts)) {
		ret = error(_("Could not detach HEAD"));
		செல் cleanup_autostash;
	}

	/*
	 * If the onto is a proper descendant of the tip of the branch, then
	 * we just fast-forwarded.
	 */
	எனில் (oideq(&branch_base, &options.orig_head->object.oid)) {
		printf(_("Fast-forwarded %s to %s.\n"),
			branch_name, options.onto_name);
		move_to_original_branch(&options);
		ret = finish_rebase(&options);
		செல் cleanup;
	}

	strbuf_addf(&revisions, "%s..%s",
		    options.வேர் ? oid_to_hex(&options.onto->object.oid) :
		    (options.restrict_revision ?
		     oid_to_hex(&options.restrict_revision->object.oid) :
		     oid_to_hex(&options.upstream->object.oid)),
		    oid_to_hex(&options.orig_head->object.oid));

	options.revisions = revisions.buf;

run_rebase:
	ret = run_specific_rebase(&options);

cleanup:
	strbuf_release(&buf);
	strbuf_release(&செய்தி);
	strbuf_release(&revisions);
	rebase_options_release(&options);
	விடுவி(squash_onto_name);
	விடுவி(keep_base_onto_name);
	திரும்பு !!ret;

cleanup_autostash:
	ret |= !!cleanup_autostash(&options);
	செல் cleanup;
}
