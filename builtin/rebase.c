/*
 * "git rebase" builtin command
 *
 * Copyright (c) 2018 Pratik Karki
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#अन्तर्भाव "builtin.h"

#अन्तर्भाव "abspath.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "run-command.h"
#अन्तर्भाव "strvec.h"
#अन्तर्भाव "dir.h"
#अन्तर्भाव "refs.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "unpack-trees.h"
#अन्तर्भाव "lockfile.h"
#अन्तर्भाव "object-file.h"
#अन्तर्भाव "object-name.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "path.h"
#अन्तर्भाव "commit.h"
#अन्तर्भाव "diff.h"
#अन्तर्भाव "wt-status.h"
#अन्तर्भाव "revision.h"
#अन्तर्भाव "commit-reach.h"
#अन्तर्भाव "rerere.h"
#अन्तर्भाव "branch.h"
#अन्तर्भाव "sequencer.h"
#अन्तर्भाव "rebase-interactive.h"
#अन्तर्भाव "reset.h"
#अन्तर्भाव "trace2.h"
#अन्तर्भाव "hook.h"
#अन्तर्भाव "trailer.h"

स्थिर वर्ण ध्रुव * ध्रुव builtin_rebase_usage[] = {
	N_("git rebase [-i] [options] [--exec <cmd>] "
		"[--onto <newbase> | --keep-base] [<upstream> [<branch>]]"),
	N_("git rebase [-i] [options] [--exec <cmd>] [--onto <newbase>] "
		"--root [<branch>]"),
	"git rebase --continue | --abort | --skip | --edit-todo",
	रिक्त
};

स्थिर GIT_PATH_FUNC(path_squash_onto, "rebase-merge/squash-onto")
स्थिर GIT_PATH_FUNC(path_interactive, "rebase-merge/interactive")
स्थिर GIT_PATH_FUNC(apply_dir, "rebase-apply")
स्थिर GIT_PATH_FUNC(merge_dir, "rebase-merge")

गणना rebase_type {
	REBASE_UNSPECIFIED = -1,
	REBASE_APPLY,
	REBASE_MERGE
};

गणना empty_type {
	EMPTY_UNSPECIFIED = -1,
	EMPTY_DROP,
	EMPTY_KEEP,
	EMPTY_STOP
};

गणना action {
	ACTION_NONE = 0,
	ACTION_CONTINUE,
	ACTION_SKIP,
	ACTION_ABORT,
	ACTION_QUIT,
	ACTION_EDIT_TODO,
	ACTION_SHOW_CURRENT_PATCH
};

स्थिर ध्रुव वर्ण *action_names[] = {
	"undefined",
	"continue",
	"skip",
	"abort",
	"quit",
	"edit_todo",
	"show_current_patch"
};

संरचना rebase_options {
	गणना rebase_type type;
	गणना empty_type empty;
	वर्ण *default_backend;
	ध्रुव वर्ण *state_dir;
	संरचना commit *upstream;
	ध्रुव वर्ण *upstream_name;
	ध्रुव वर्ण *upstream_arg;
	वर्ण *head_name;
	संरचना commit *orig_head;
	संरचना commit *onto;
	ध्रुव वर्ण *onto_name;
	ध्रुव वर्ण *revisions;
	ध्रुव वर्ण *switch_to;
	पूर्णाङ्क मूलम्, मूलम्_with_onto;
	संरचना object_id *squash_onto;
	संरचना commit *restrict_revision;
	पूर्णाङ्क dont_finish_rebase;
	गणना {
		REBASE_NO_QUIET = 1<<0,
		REBASE_VERBOSE = 1<<1,
		REBASE_DIFFSTAT = 1<<2,
		REBASE_FORCE = 1<<3,
		REBASE_INTERACTIVE_EXPLICIT = 1<<4,
	} flags;
	संरचना strvec git_am_opts;
	गणना action action;
	वर्ण *reflog_action;
	पूर्णाङ्क signoff;
	संरचना strvec trailer_args;
	पूर्णाङ्क allow_rerere_autoupdate;
	पूर्णाङ्क keep_empty;
	पूर्णाङ्क autosquash;
	वर्ण *gpg_sign_opt;
	पूर्णाङ्क autostash;
	पूर्णाङ्क committer_date_is_author_date;
	पूर्णाङ्क ignore_date;
	संरचना string_list exec;
	पूर्णाङ्क allow_empty_message;
	पूर्णाङ्क rebase_merges, rebase_cousins;
	वर्ण *strategy;
	संरचना string_list strategy_opts;
	संरचना strbuf git_format_patch_opt;
	पूर्णाङ्क reschedule_failed_exec;
	पूर्णाङ्क reapply_cherry_picks;
	पूर्णाङ्क fork_point;
	पूर्णाङ्क update_refs;
	पूर्णाङ्क config_autosquash;
	पूर्णाङ्क config_rebase_merges;
	पूर्णाङ्क config_update_refs;
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

स्थिर शून्य rebase_options_release(संरचना rebase_options *opts)
{
	मुक्त(opts->default_backend);
	मुक्त(opts->reflog_action);
	मुक्त(opts->head_name);
	strvec_clear(&opts->git_am_opts);
	मुक्त(opts->gpg_sign_opt);
	string_list_clear(&opts->exec, 0);
	मुक्त(opts->strategy);
	string_list_clear(&opts->strategy_opts, 0);
	strbuf_release(&opts->git_format_patch_opt);
	strvec_clear(&opts->trailer_args);
}

स्थिर संरचना replay_opts get_replay_opts(ध्रुव संरचना rebase_options *opts)
{
	संरचना replay_opts replay = REPLAY_OPTS_INIT;

	replay.action = REPLAY_INTERACTIVE_REBASE;
	replay.strategy = रिक्त;
	sequencer_init_config(&replay);

	replay.signoff = opts->signoff;

	strvec_pushv(&replay.trailer_args, opts->trailer_args.v);

	replay.allow_ff = !(opts->flags & REBASE_FORCE);
	सचे (opts->allow_rerere_autoupdate)
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
	मुक्त(replay.gpg_sign);
	replay.gpg_sign = xstrdup_or_null(opts->gpg_sign_opt);
	replay.reflog_action = xstrdup(opts->reflog_action);
	सचे (opts->strategy)
		replay.strategy = xstrdup_or_null(opts->strategy);
	अञ्ञथा सचे (!replay.strategy && replay.default_strategy) {
		replay.strategy = replay.default_strategy;
		replay.default_strategy = रिक्त;
	}

	अत्थाय (size_t i = 0; i < opts->strategy_opts.nr; i++)
		strvec_प्रक्षिप(&replay.xopts, opts->strategy_opts.वस्तूनि[i].string);

	सचे (opts->squash_onto) {
		oidcpy(&replay.squash_onto, opts->squash_onto);
		replay.have_squash_onto = 1;
	}

	पटिनिवत्त replay;
}

स्थिर पूर्णाङ्क edit_todo_file(unsigned flags, संरचना replay_opts *opts)
{
	ध्रुव वर्ण *todo_file = rebase_path_todo();
	संरचना todo_list todo_list = TODO_LIST_INIT,
		new_todo = TODO_LIST_INIT;
	पूर्णाङ्क res = 0;

	सचे (strbuf_read_file(&todo_list.buf, todo_file, 0) < 0)
		पटिनिवत्त error_errno(_("could not read '%s'."), todo_file);

	strbuf_stripspace(&todo_list.buf, comment_पङ्क्तिः_str);
	res = edit_todo_list(the_repository, opts, &todo_list, &new_todo,
			     रिक्त, रिक्त, flags);
	सचे (!res && todo_list_write_to_file(the_repository, &new_todo, todo_file,
					    रिक्त, रिक्त, -1, flags & ~(TODO_LIST_SHORTEN_IDS)))
		res = error_errno(_("could not write '%s'"), todo_file);

	todo_list_release(&todo_list);
	todo_list_release(&new_todo);

	पटिनिवत्त res;
}

स्थिर पूर्णाङ्क get_revision_ranges(संरचना commit *upstream, संरचना commit *onto,
			       संरचना object_id *orig_head, वर्ण **revisions,
			       वर्ण **shortrevisions)
{
	संरचना commit *base_rev = upstream ? upstream : onto;
	ध्रुव वर्ण *shorthead;

	*revisions = xstrfmt("%s...%s", oid_to_hex(&base_rev->object.oid),
			     oid_to_hex(orig_head));

	shorthead = repo_find_unique_abbrev(the_repository, orig_head,
					    DEFAULT_ABBREV);

	सचे (upstream) {
		ध्रुव वर्ण *shortrev;

		shortrev = repo_find_unique_abbrev(the_repository,
						   &base_rev->object.oid,
						   DEFAULT_ABBREV);

		*shortrevisions = xstrfmt("%s..%s", shortrev, shorthead);
	} अञ्ञथा
		*shortrevisions = xstrdup(shorthead);

	पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क init_basic_state(संरचना replay_opts *opts, ध्रुव वर्ण *head_name,
			    संरचना commit *onto,
			    ध्रुव संरचना object_id *orig_head)
{
	FILE *interactive;

	सचे (!is_directory(merge_dir()) &&
	    safe_create_dir_in_gitdir(the_repository, merge_dir()))
		पटिनिवत्त error_errno(_("could not create temporary %s"), merge_dir());

	refs_delete_reflog(get_main_ref_store(the_repository), "REBASE_HEAD");

	interactive = fopen(path_interactive(), "w");
	सचे (!interactive)
		पटिनिवत्त error_errno(_("could not mark as interactive"));
	fclose(interactive);

	पटिनिवत्त write_basic_state(opts, head_name, onto, orig_head);
}

स्थिर पूर्णाङ्क do_interactive_rebase(संरचना rebase_options *opts, unsigned flags)
{
	पूर्णाङ्क ret = -1;
	वर्ण *revisions = रिक्त, *shortrevisions = रिक्त;
	संरचना strvec make_script_args = STRVEC_INIT;
	संरचना todo_list todo_list = TODO_LIST_INIT;
	संरचना replay_opts replay = get_replay_opts(opts);

	सचे (get_revision_ranges(opts->upstream, opts->onto, &opts->orig_head->object.oid,
				&revisions, &shortrevisions))
		गच्छ cleanup;

	strvec_pushl(&make_script_args, "", revisions, रिक्त);
	सचे (opts->restrict_revision)
		strvec_pushf(&make_script_args, "^%s",
			     oid_to_hex(&opts->restrict_revision->object.oid));

	ret = sequencer_make_script(the_repository, &todo_list.buf,
				    &make_script_args, flags);
	सचे (ret) {
		error(_("could not generate todo list"));
		गच्छ cleanup;
	}

	सचे (init_basic_state(&replay,
			     opts->head_name ? opts->head_name : "detached HEAD",
			     opts->onto, &opts->orig_head->object.oid))
		गच्छ cleanup;

	सचे (!opts->upstream && opts->squash_onto)
		write_file(path_squash_onto(), "%s\n",
			   oid_to_hex(opts->squash_onto));

	discard_index(the_repository->index);
	सचे (todo_list_parse_insn_buffer(the_repository, &replay,
					todo_list.buf.buf, &todo_list))
		BUG("unusable todo list");

	ret = complete_action(the_repository, &replay, flags,
		shortrevisions, opts->onto_name, opts->onto,
		&opts->orig_head->object.oid, &opts->exec,
		opts->autosquash, opts->update_refs, &todo_list);

cleanup:
	replay_opts_release(&replay);
	मुक्त(revisions);
	मुक्त(shortrevisions);
	todo_list_release(&todo_list);
	strvec_clear(&make_script_args);

	पटिनिवत्त ret;
}

स्थिर पूर्णाङ्क run_sequencer_rebase(संरचना rebase_options *opts)
{
	unsigned flags = 0;
	पूर्णाङ्क abbreviate_commands = 0, ret = 0;

	repo_config_get_bool(the_repository, "rebase.abbreviatecommands", &abbreviate_commands);

	flags |= opts->keep_empty ? TODO_LIST_KEEP_EMPTY : 0;
	flags |= abbreviate_commands ? TODO_LIST_ABBREVIATE_CMDS : 0;
	flags |= opts->rebase_merges ? TODO_LIST_REBASE_MERGES : 0;
	flags |= opts->rebase_cousins > 0 ? TODO_LIST_REBASE_COUSINS : 0;
	flags |= opts->मूलम्_with_onto ? TODO_LIST_मूलम्_WITH_ONTO : 0;
	flags |= opts->reapply_cherry_picks ? TODO_LIST_REAPPLY_CHERRY_PICKS : 0;
	flags |= opts->flags & REBASE_NO_QUIET ? TODO_LIST_WARN_SKIPPED_CHERRY_PICKS : 0;

	विकल्प (opts->action) {
	पक्ष ACTION_NONE: {
		सचे (!opts->onto && !opts->upstream)
			die(_("a base commit must be provided with --upstream or --onto"));

		ret = do_interactive_rebase(opts, flags);
		भङ्ग;
	}
	पक्ष ACTION_SKIP: {
		संरचना string_list merge_rr = STRING_LIST_INIT_DUP;

		rerere_clear(the_repository, &merge_rr);
	}
		/* fallthrough */
	पक्ष ACTION_CONTINUE: {
		संरचना replay_opts replay_opts = get_replay_opts(opts);

		ret = sequencer_continue(the_repository, &replay_opts);
		replay_opts_release(&replay_opts);
		भङ्ग;
	}
	पक्ष ACTION_EDIT_TODO: {
		संरचना replay_opts replay_opts = get_replay_opts(opts);

		ret = edit_todo_file(flags, &replay_opts);
		replay_opts_release(&replay_opts);
		भङ्ग;
	}
	पक्ष ACTION_SHOW_CURRENT_PATCH: {
		संरचना child_process cmd = CHILD_PROCESS_INIT;

		cmd.git_cmd = 1;
		strvec_pushl(&cmd.args, "show", "REBASE_HEAD", "--", रिक्त);
		ret = run_command(&cmd);

		भङ्ग;
	}
	स्वभाव:
		BUG("invalid command '%d'", opts->action);
	}

	पटिनिवत्त ret;
}

स्थिर पूर्णाङ्क is_merge(संरचना rebase_options *opts)
{
	पटिनिवत्त opts->type == REBASE_MERGE;
}

स्थिर शून्य imply_merge(संरचना rebase_options *opts, ध्रुव वर्ण *option)
{
	विकल्प (opts->type) {
	पक्ष REBASE_APPLY:
		die(_("%s requires the merge backend"), option);
		भङ्ग;
	पक्ष REBASE_MERGE:
		भङ्ग;
	स्वभाव:
		opts->type = REBASE_MERGE; /* implied */
		भङ्ग;
	}
}

/* Returns the filename prefixed by the state_dir */
स्थिर ध्रुव वर्ण *state_dir_path(ध्रुव वर्ण *filename, संरचना rebase_options *opts)
{
	स्थिर संरचना strbuf path = STRBUF_INIT;
	स्थिर size_t prefix_len;

	सचे (!prefix_len) {
		strbuf_addf(&path, "%s/", opts->state_dir);
		prefix_len = path.len;
	}

	strbuf_setlen(&path, prefix_len);
	strbuf_addstr(&path, filename);
	पटिनिवत्त path.buf;
}

/* Initialize the rebase options from the state directory. */
स्थिर पूर्णाङ्क read_basic_state(संरचना rebase_options *opts)
{
	संरचना strbuf head_name = STRBUF_INIT;
	संरचना strbuf buf = STRBUF_INIT;
	संरचना object_id oid;

	सचे (!read_oneliner(&head_name, state_dir_path("head-name", opts),
			   READ_ONELINER_WARN_MISSING) ||
	    !read_oneliner(&buf, state_dir_path("onto", opts),
			   READ_ONELINER_WARN_MISSING))
		पटिनिवत्त -1;
	opts->head_name = starts_with(head_name.buf, "refs/") ?
		xstrdup(head_name.buf) : रिक्त;
	strbuf_release(&head_name);
	सचे (get_oid_hex(buf.buf, &oid) ||
	    !(opts->onto = lookup_commit_object(the_repository, &oid)))
		पटिनिवत्त error(_("invalid onto: '%s'"), buf.buf);

	/*
	 * We always write to orig-head, but interactive rebase used to write to
	 * head. Fall back to reading from head to cover for the case that the
	 * user upgraded git with an ongoing interactive rebase.
	 */
	strbuf_reset(&buf);
	सचे (file_exists(state_dir_path("orig-head", opts))) {
		सचे (!read_oneliner(&buf, state_dir_path("orig-head", opts),
				   READ_ONELINER_WARN_MISSING))
			पटिनिवत्त -1;
	} अञ्ञथा सचे (!read_oneliner(&buf, state_dir_path("head", opts),
				  READ_ONELINER_WARN_MISSING))
		पटिनिवत्त -1;
	सचे (get_oid_hex(buf.buf, &oid) ||
	    !(opts->orig_head = lookup_commit_object(the_repository, &oid)))
		पटिनिवत्त error(_("invalid orig-head: '%s'"), buf.buf);

	सचे (file_exists(state_dir_path("quiet", opts)))
		opts->flags &= ~REBASE_NO_QUIET;
	अञ्ञथा
		opts->flags |= REBASE_NO_QUIET;

	सचे (file_exists(state_dir_path("verbose", opts)))
		opts->flags |= REBASE_VERBOSE;

	सचे (file_exists(state_dir_path("signoff", opts))) {
		opts->signoff = 1;
		opts->flags |= REBASE_FORCE;
	}

	सचे (file_exists(state_dir_path("allow_rerere_autoupdate", opts))) {
		strbuf_reset(&buf);
		सचे (!read_oneliner(&buf, state_dir_path("allow_rerere_autoupdate", opts),
				   READ_ONELINER_WARN_MISSING))
			पटिनिवत्त -1;
		सचे (!strcmp(buf.buf, "--rerere-autoupdate"))
			opts->allow_rerere_autoupdate = RERERE_AUTOUPDATE;
		अञ्ञथा सचे (!strcmp(buf.buf, "--no-rerere-autoupdate"))
			opts->allow_rerere_autoupdate = RERERE_NOAUTOUPDATE;
		अञ्ञथा
			warning(_("ignoring invalid allow_rerere_autoupdate: "
				  "'%s'"), buf.buf);
	}

	सचे (file_exists(state_dir_path("gpg_sign_opt", opts))) {
		strbuf_reset(&buf);
		सचे (!read_oneliner(&buf, state_dir_path("gpg_sign_opt", opts),
				   READ_ONELINER_WARN_MISSING))
			पटिनिवत्त -1;
		मुक्त(opts->gpg_sign_opt);
		opts->gpg_sign_opt = xstrdup(buf.buf);
	}

	strbuf_release(&buf);

	पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क rebase_write_basic_state(संरचना rebase_options *opts)
{
	write_file(state_dir_path("head-name", opts), "%s",
		   opts->head_name ? opts->head_name : "detached HEAD");
	write_file(state_dir_path("onto", opts), "%s",
		   opts->onto ? oid_to_hex(&opts->onto->object.oid) : "");
	write_file(state_dir_path("orig-head", opts), "%s",
		   oid_to_hex(&opts->orig_head->object.oid));
	सचे (!(opts->flags & REBASE_NO_QUIET))
		write_file(state_dir_path("quiet", opts), "%s", "");
	सचे (opts->flags & REBASE_VERBOSE)
		write_file(state_dir_path("verbose", opts), "%s", "");
	सचे (opts->allow_rerere_autoupdate > 0)
		write_file(state_dir_path("allow_rerere_autoupdate", opts),
			   "-%s-rerere-autoupdate",
			   opts->allow_rerere_autoupdate == RERERE_AUTOUPDATE ?
				"" : "-no");
	सचे (opts->gpg_sign_opt)
		write_file(state_dir_path("gpg_sign_opt", opts), "%s",
			   opts->gpg_sign_opt);
	सचे (opts->signoff)
		write_file(state_dir_path("signoff", opts), "--signoff");

	पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क cleanup_autostash(संरचना rebase_options *opts)
{
	पूर्णाङ्क ret;
	संरचना strbuf dir = STRBUF_INIT;
	ध्रुव वर्ण *path = state_dir_path("autostash", opts);

	सचे (!file_exists(path))
		पटिनिवत्त 0;
	ret = apply_autostash(path);
	strbuf_addstr(&dir, opts->state_dir);
	सचे (remove_dir_recursively(&dir, 0))
		ret = error_errno(_("could not remove '%s'"), opts->state_dir);
	strbuf_release(&dir);

	पटिनिवत्त ret;
}

स्थिर पूर्णाङ्क finish_rebase(संरचना rebase_options *opts)
{
	संरचना strbuf dir = STRBUF_INIT;
	पूर्णाङ्क ret = 0;

	refs_delete_ref(get_main_ref_store(the_repository), रिक्त,
			"REBASE_HEAD", रिक्त, REF_NO_DEREF);
	refs_delete_ref(get_main_ref_store(the_repository), रिक्त,
			"AUTO_MERGE", रिक्त, REF_NO_DEREF);
	apply_autostash(state_dir_path("autostash", opts));
	/*
	 * We ignore errors in 'git maintenance run --auto', since the
	 * user should see them.
	 */
	run_auto_maintenance(the_repository,
			!(opts->flags & (REBASE_NO_QUIET|REBASE_VERBOSE)));

	सचे (opts->type == REBASE_MERGE) {
		संरचना replay_opts replay = REPLAY_OPTS_INIT;

		replay.action = REPLAY_INTERACTIVE_REBASE;
		ret = sequencer_remove_state(&replay);
		replay_opts_release(&replay);
	} अञ्ञथा {
		strbuf_addstr(&dir, opts->state_dir);
		सचे (remove_dir_recursively(&dir, 0))
			ret = error(_("could not remove '%s'"),
				    opts->state_dir);
		strbuf_release(&dir);
	}

	पटिनिवत्त ret;
}

स्थिर पूर्णाङ्क move_to_original_branch(संरचना rebase_options *opts)
{
	संरचना strbuf branch_reflog = STRBUF_INIT, head_reflog = STRBUF_INIT;
	संरचना reset_working_वृक्षः_options ropts = { 0 };
	पूर्णाङ्क ret;

	सचे (!opts->head_name)
		पटिनिवत्त 0; /* nothing to move back to */

	सचे (!opts->onto)
		BUG("move_to_original_branch without onto");

	strbuf_addf(&branch_reflog, "%s (finish): %s onto %s",
		    opts->reflog_action,
		    opts->head_name, oid_to_hex(&opts->onto->object.oid));
	strbuf_addf(&head_reflog, "%s (finish): returning to %s",
		    opts->reflog_action, opts->head_name);
	ropts.branch = opts->head_name;
	ropts.flags = RESET_WORKING_वृक्षः_REFS_ONLY |
		      RESET_WORKING_वृक्षः_UPDATE_HEAD;
	ropts.branch_सन्देशः = branch_reflog.buf;
	ropts.head_सन्देशः = head_reflog.buf;
	ret = reset_working_वृक्षः(the_repository, &ropts);

	strbuf_release(&branch_reflog);
	strbuf_release(&head_reflog);
	पटिनिवत्त ret;
}

स्थिर पूर्णाङ्क run_am(संरचना rebase_options *opts)
{
	संरचना child_process am = CHILD_PROCESS_INIT;
	संरचना child_process format_patch = CHILD_PROCESS_INIT;
	पूर्णाङ्क status;
	वर्ण *rebased_patches;

	am.git_cmd = 1;
	strvec_प्रक्षिप(&am.args, "am");
	strvec_pushf(&am.env, GIT_REFLOG_ACTION_ENVIRONMENT "=%s (pick)",
		     opts->reflog_action);
	सचे (opts->action == ACTION_CONTINUE) {
		strvec_प्रक्षिप(&am.args, "--resolved");
		strvec_pushf(&am.args, "--resolvemsg=%s", rebase_resolvemsg);
		सचे (opts->gpg_sign_opt)
			strvec_प्रक्षिप(&am.args, opts->gpg_sign_opt);
		status = run_command(&am);
		सचे (status)
			पटिनिवत्त status;

		पटिनिवत्त move_to_original_branch(opts);
	}
	सचे (opts->action == ACTION_SKIP) {
		strvec_प्रक्षिप(&am.args, "--skip");
		strvec_pushf(&am.args, "--resolvemsg=%s", rebase_resolvemsg);
		status = run_command(&am);
		सचे (status)
			पटिनिवत्त status;

		पटिनिवत्त move_to_original_branch(opts);
	}
	सचे (opts->action == ACTION_SHOW_CURRENT_PATCH) {
		strvec_प्रक्षिप(&am.args, "--show-current-patch");
		पटिनिवत्त run_command(&am);
	}

	rebased_patches = repo_git_path(the_repository, "rebased-patches");
	format_patch.out = open(rebased_patches,
				O_WRONLY | O_CREAT | O_TRUNC, 0666);
	सचे (format_patch.out < 0) {
		status = error_errno(_("could not open '%s' for writing"),
				     rebased_patches);
		मुक्त(rebased_patches);
		child_process_clear(&am);
		पटिनिवत्त status;
	}

	format_patch.git_cmd = 1;
	strvec_pushl(&format_patch.args, "format-patch", "-k", "--stdout",
		     "--full-index", "--cherry-pick", "--right-only",
		     "--default-prefix", "--no-renames",
		     "--no-cover-letter", "--pretty=mboxrd", "--topo-order",
		     "--no-base", रिक्त);
	सचे (opts->git_format_patch_opt.len)
		strvec_split(&format_patch.args,
			     opts->git_format_patch_opt.buf);
	strvec_pushf(&format_patch.args, "%s...%s",
		     oid_to_hex(opts->मूलम् ?
				/* this is now equivalent to !opts->upstream */
				&opts->onto->object.oid :
				&opts->upstream->object.oid),
		     oid_to_hex(&opts->orig_head->object.oid));
	सचे (opts->restrict_revision)
		strvec_pushf(&format_patch.args, "^%s",
			     oid_to_hex(&opts->restrict_revision->object.oid));

	status = run_command(&format_patch);
	सचे (status) {
		संरचना reset_working_वृक्षः_options ropts = { 0 };
		unlink(rebased_patches);
		मुक्त(rebased_patches);
		child_process_clear(&am);

		ropts.oid = &opts->orig_head->object.oid;
		ropts.branch = opts->head_name;
		ropts.default_reflog_action = opts->reflog_action;
		ropts.flags = RESET_WORKING_वृक्षः_UPDATE_HEAD;
		reset_working_वृक्षः(the_repository, &ropts);
		error(_("\ngit encountered an error while preparing the "
			"patches to replay\n"
			"these revisions:\n"
			"\n    %s\n\n"
			"As a result, git cannot rebase them."),
		      opts->revisions);

		पटिनिवत्त status;
	}

	am.in = open(rebased_patches, O_RDONLY);
	सचे (am.in < 0) {
		status = error_errno(_("could not open '%s' for reading"),
				     rebased_patches);
		मुक्त(rebased_patches);
		child_process_clear(&am);
		पटिनिवत्त status;
	}

	strvec_pushv(&am.args, opts->git_am_opts.v);
	strvec_प्रक्षिप(&am.args, "--rebasing");
	strvec_pushf(&am.args, "--resolvemsg=%s", rebase_resolvemsg);
	strvec_प्रक्षिप(&am.args, "--patch-format=mboxrd");
	सचे (opts->allow_rerere_autoupdate == RERERE_AUTOUPDATE)
		strvec_प्रक्षिप(&am.args, "--rerere-autoupdate");
	अञ्ञथा सचे (opts->allow_rerere_autoupdate == RERERE_NOAUTOUPDATE)
		strvec_प्रक्षिप(&am.args, "--no-rerere-autoupdate");
	सचे (opts->gpg_sign_opt)
		strvec_प्रक्षिप(&am.args, opts->gpg_sign_opt);
	status = run_command(&am);
	unlink(rebased_patches);
	मुक्त(rebased_patches);

	सचे (!status) {
		पटिनिवत्त move_to_original_branch(opts);
	}

	सचे (is_directory(opts->state_dir))
		rebase_write_basic_state(opts);

	पटिनिवत्त status;
}

स्थिर पूर्णाङ्क run_specific_rebase(संरचना rebase_options *opts)
{
	पूर्णाङ्क status;

	सचे (opts->type == REBASE_MERGE) {
		/* Run sequencer-based rebase */
		सचे (!(opts->flags & REBASE_INTERACTIVE_EXPLICIT))
			setenv("GIT_SEQUENCE_EDITOR", ":", 1);
		सचे (opts->gpg_sign_opt) {
			/* remove the leading "-S" */
			वर्ण *tmp = xstrdup(opts->gpg_sign_opt + 2);
			मुक्त(opts->gpg_sign_opt);
			opts->gpg_sign_opt = tmp;
		}

		status = run_sequencer_rebase(opts);
	} अञ्ञथा सचे (opts->type == REBASE_APPLY)
		status = run_am(opts);
	अञ्ञथा
		BUG("Unhandled rebase type %d", opts->type);

	सचे (opts->dont_finish_rebase)
		; /* do nothing */
	अञ्ञथा सचे (opts->type == REBASE_MERGE) {
		पूर्णाङ्क quiet = !(opts->flags & (REBASE_NO_QUIET|REBASE_VERBOSE));

		/*
		 * The sequencer cleans up after itself. Its state directory
		 * is gone once it is done, and stays while it is stopped.
		 */
		सचे (status == 0 && !is_directory(opts->state_dir))
			run_auto_maintenance(the_repository, quiet);
	} अञ्ञथा सचे (status == 0) {
		सचे (!file_exists(state_dir_path("stopped-sha", opts)))
			finish_rebase(opts);
	} अञ्ञथा सचे (status == 2) {
		संरचना strbuf dir = STRBUF_INIT;

		apply_autostash(state_dir_path("autostash", opts));
		strbuf_addstr(&dir, opts->state_dir);
		remove_dir_recursively(&dir, 0);
		strbuf_release(&dir);
		die("Nothing to do");
	}

	पटिनिवत्त status ? -1 : 0;
}

स्थिर शून्य parse_rebase_merges_मानम्(संरचना rebase_options *options, ध्रुव वर्ण *मानम्)
{
	सचे (!strcmp("no-rebase-cousins", मानम्))
		options->rebase_cousins = 0;
	अञ्ञथा सचे (!strcmp("rebase-cousins", मानम्))
		options->rebase_cousins = 1;
	अञ्ञथा
		die(_("Unknown rebase-merges mode: %s"), मानम्);
}

स्थिर पूर्णाङ्क rebase_config(ध्रुव वर्ण *var, ध्रुव वर्ण *मानम्,
			 ध्रुव संरचना config_context *ctx, शून्य *data)
{
	संरचना rebase_options *opts = data;

	सचे (!strcmp(var, "rebase.stat")) {
		सचे (git_config_bool(var, मानम्))
			opts->flags |= REBASE_DIFFSTAT;
		अञ्ञथा
			opts->flags &= ~REBASE_DIFFSTAT;
		पटिनिवत्त 0;
	}

	सचे (!strcmp(var, "rebase.autosquash")) {
		opts->config_autosquash = git_config_bool(var, मानम्);
		पटिनिवत्त 0;
	}

	सचे (!strcmp(var, "commit.gpgsign")) {
		मुक्त(opts->gpg_sign_opt);
		opts->gpg_sign_opt = git_config_bool(var, मानम्) ?
			xstrdup("-S") : रिक्त;
		पटिनिवत्त 0;
	}

	सचे (!strcmp(var, "rebase.autostash")) {
		opts->autostash = git_config_bool(var, मानम्);
		पटिनिवत्त 0;
	}

	सचे (!strcmp(var, "rebase.rebasemerges")) {
		opts->config_rebase_merges = git_parse_maybe_bool(मानम्);
		सचे (opts->config_rebase_merges < 0) {
			opts->config_rebase_merges = 1;
			parse_rebase_merges_मानम्(opts, मानम्);
		} अञ्ञथा {
			opts->rebase_cousins = 0;
		}
		पटिनिवत्त 0;
	}

	सचे (!strcmp(var, "rebase.updaterefs")) {
		opts->config_update_refs = git_config_bool(var, मानम्);
		पटिनिवत्त 0;
	}

	सचे (!strcmp(var, "rebase.reschedulefailedexec")) {
		opts->reschedule_failed_exec = git_config_bool(var, मानम्);
		पटिनिवत्त 0;
	}

	सचे (!strcmp(var, "rebase.forkpoint")) {
		opts->fork_point = git_config_bool(var, मानम्) ? -1 : 0;
		पटिनिवत्त 0;
	}

	सचे (!strcmp(var, "rebase.backend")) {
		FREE_AND_NULL(opts->default_backend);
		पटिनिवत्त git_config_string(&opts->default_backend, var, मानम्);
	}

	पटिनिवत्त git_default_config(var, मानम्, ctx, data);
}

स्थिर पूर्णाङ्क checkout_up_to_date(संरचना rebase_options *options)
{
	संरचना strbuf buf = STRBUF_INIT;
	संरचना reset_working_वृक्षः_options ropts = { 0 };
	पूर्णाङ्क ret = 0;

	strbuf_addf(&buf, "%s: checkout %s",
		    options->reflog_action, options->switch_to);
	ropts.oid = &options->orig_head->object.oid;
	ropts.branch = options->head_name;
	ropts.flags = RESET_WORKING_वृक्षः_RUN_POST_CHECKOUT_HOOK |
		      RESET_WORKING_वृक्षः_UPDATE_HEAD;
	सचे (!ropts.branch)
		ropts.flags |=  RESET_WORKING_वृक्षः_DETACH;
	ropts.head_सन्देशः = buf.buf;
	सचे (reset_working_वृक्षः(the_repository, &ropts) < 0)
		ret = error(_("could not switch to %s"), options->switch_to);
	strbuf_release(&buf);

	पटिनिवत्त ret;
}

/*
 * Determines whether the commits in from..to are linear, i.e. contain
 * no merge commits. This function *expects* `from` to be an ancestor of
 * `to`.
 */
स्थिर पूर्णाङ्क is_linear_history(संरचना commit *from, संरचना commit *to)
{
	याव (to && to != from) {
		repo_parse_commit(the_repository, to);
		सचे (!to->parents)
			पटिनिवत्त 1;
		सचे (to->parents->next)
			पटिनिवत्त 0;
		to = to->parents->item;
	}
	पटिनिवत्त 1;
}

स्थिर पूर्णाङ्क can_fast_forward(संरचना commit *onto, संरचना commit *upstream,
			    संरचना commit *restrict_revision,
			    संरचना commit *head, संरचना object_id *branch_base)
{
	संरचना commit_list *merge_bases = रिक्त;
	पूर्णाङ्क res = 0;

	सचे (is_null_oid(branch_base))
		गच्छ done; /* fill_branch_base() found multiple merge bases */

	सचे (!oideq(branch_base, &onto->object.oid))
		गच्छ done;

	सचे (restrict_revision && !oideq(&restrict_revision->object.oid, branch_base))
		गच्छ done;

	सचे (!upstream)
		गच्छ done;

	सचे (repo_get_merge_bases(the_repository, upstream, head, &merge_bases) < 0)
		exit(128);
	सचे (!merge_bases || merge_bases->next)
		गच्छ done;

	सचे (!oideq(&onto->object.oid, &merge_bases->item->object.oid))
		गच्छ done;

	res = 1;

done:
	commit_list_free(merge_bases);
	पटिनिवत्त res && is_linear_history(onto, head);
}

स्थिर शून्य fill_branch_base(संरचना rebase_options *options,
			    संरचना object_id *branch_base)
{
	संरचना commit_list *merge_bases = रिक्त;

	सचे (repo_get_merge_bases(the_repository, options->onto,
				 options->orig_head, &merge_bases) < 0)
		exit(128);
	सचे (!merge_bases || merge_bases->next)
		oidcpy(branch_base, null_oid(the_hash_algo));
	अञ्ञथा
		oidcpy(branch_base, &merge_bases->item->object.oid);

	commit_list_free(merge_bases);
}

स्थिर पूर्णाङ्क parse_opt_am(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना rebase_options *opts = opt->मानम्;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	सचे (opts->type != REBASE_UNSPECIFIED && opts->type != REBASE_APPLY)
	    die(_("apply options and merge options cannot be used together"));

	opts->type = REBASE_APPLY;

	पटिनिवत्त 0;
}

/* -i followed by -m is still -i */
स्थिर पूर्णाङ्क parse_opt_merge(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना rebase_options *opts = opt->मानम्;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	सचे (opts->type != REBASE_UNSPECIFIED && opts->type != REBASE_MERGE)
	    die(_("apply options and merge options cannot be used together"));

	opts->type = REBASE_MERGE;

	पटिनिवत्त 0;
}

/* -i followed by -r is still explicitly interactive, but -r alone is not */
स्थिर पूर्णाङ्क parse_opt_interactive(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg,
				 पूर्णाङ्क unset)
{
	संरचना rebase_options *opts = opt->मानम्;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	सचे (opts->type != REBASE_UNSPECIFIED && opts->type != REBASE_MERGE)
	    die(_("apply options and merge options cannot be used together"));

	opts->type = REBASE_MERGE;
	opts->flags |= REBASE_INTERACTIVE_EXPLICIT;

	पटिनिवत्त 0;
}

स्थिर गणना empty_type parse_empty_मानम्(ध्रुव वर्ण *मानम्)
{
	सचे (!strcasecmp(मानम्, "drop"))
		पटिनिवत्त EMPTY_DROP;
	अञ्ञथा सचे (!strcasecmp(मानम्, "keep"))
		पटिनिवत्त EMPTY_KEEP;
	अञ्ञथा सचे (!strcasecmp(मानम्, "stop"))
		पटिनिवत्त EMPTY_STOP;
	अञ्ञथा सचे (!strcasecmp(मानम्, "ask")) {
		warning(_("--empty=ask is deprecated; use '--empty=stop' instead."));
		पटिनिवत्त EMPTY_STOP;
	}

	die(_("unrecognized empty type '%s'; valid values are \"drop\", \"keep\", and \"stop\"."), मानम्);
}

स्थिर पूर्णाङ्क parse_opt_keep_empty(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg,
				पूर्णाङ्क unset)
{
	संरचना rebase_options *opts = opt->मानम्;

	BUG_ON_OPT_ARG(arg);

	imply_merge(opts, unset ? "--no-keep-empty" : "--keep-empty");
	opts->keep_empty = !unset;
	पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क parse_opt_empty(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना rebase_options *options = opt->मानम्;
	गणना empty_type मानम् = parse_empty_मानम्(arg);

	BUG_ON_OPT_NEG(unset);

	options->empty = मानम्;
	पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क parse_opt_rebase_merges(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना rebase_options *options = opt->मानम्;

	options->rebase_merges = !unset;
	options->rebase_cousins = 0;

	सचे (arg) {
		सचे (!*arg) {
			warning(_("--rebase-merges with an empty string "
				  "argument is deprecated and will stop "
				  "working in a future version of Git. Use "
				  "--rebase-merges without an argument "
				  "instead, which does the same thing."));
			पटिनिवत्त 0;
		}
		parse_rebase_merges_मानम्(options, arg);
	}

	पटिनिवत्त 0;
}

स्थिर शून्य NORETURN error_on_missing_default_upstream(शून्य)
{
	संरचना branch *current_branch = branch_get(रिक्त);

	printf(_("%s\n"
		 "Please specify which branch you want to rebase against.\n"
		 "See git-rebase(1) for details.\n"
		 "\n"
		 "    git rebase '<branch>'\n"
		 "\n"),
		current_branch ? _("There is no tracking information for "
			"the current branch.") :
			_("You are not currently on a branch."));

	सचे (current_branch) {
		ध्रुव वर्ण *remote = current_branch->remote_name;

		सचे (!remote)
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

स्थिर पूर्णाङ्क check_exec_cmd(ध्रुव वर्ण *cmd)
{
	सचे (strchr(cmd, '\n'))
		पटिनिवत्त error(_("exec commands cannot contain newlines"));

	/* Does the command consist purely of whitespace? */
	सचे (!cmd[strspn(cmd, " \t\r\f\v")])
		पटिनिवत्त error(_("empty exec command"));

	पटिनिवत्त 0;
}

पूर्णाङ्क cmd_rebase(पूर्णाङ्क argc,
	       ध्रुव वर्ण **argv,
	       ध्रुव वर्ण *prefix,
	       संरचना repository *repo UNUSED)
{
	संरचना rebase_options options = REBASE_OPTIONS_INIT;
	ध्रुव वर्ण *branch_name;
	ध्रुव वर्ण *strategy_opt = रिक्त;
	पूर्णाङ्क ret, flags, total_argc, in_progress = 0;
	पूर्णाङ्क keep_base = 0;
	पूर्णाङ्क ok_to_skip_pre_rebase = 0;
	संरचना strbuf सन्देशः = STRBUF_INIT;
	संरचना strbuf revisions = STRBUF_INIT;
	संरचना strbuf buf = STRBUF_INIT;
	संरचना object_id branch_base;
	पूर्णाङ्क ignore_whitespace = 0;
	ध्रुव वर्ण *gpg_sign = रिक्त;
	संरचना object_id squash_onto;
	वर्ण *squash_onto_name = रिक्त;
	वर्ण *keep_base_onto_name = रिक्त;
	पूर्णाङ्क reschedule_failed_exec = -1;
	पूर्णाङ्क allow_preemptive_ff = 1;
	पूर्णाङ्क preserve_merges_selected = 0;
	संरचना reset_working_वृक्षः_options ropts = { 0 };
	संरचना option builtin_rebase_options[] = {
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
			.मानम् = &options.flags,
			.precision = परिमाण(options.flags),
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
		OPT_PASSTHRU_ARGV('C', रिक्त, &options.git_am_opts, N_("n"),
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
		OPT_CALLBACK_F(0, "apply", &options, रिक्त,
			N_("use apply strategies to rebase"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			parse_opt_am),
		OPT_CALLBACK_F('m', "merge", &options, रिक्त,
			N_("use merging strategies to rebase"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			parse_opt_merge),
		OPT_CALLBACK_F('i', "interactive", &options, रिक्त,
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
		OPT_CALLBACK_F('k', "keep-empty", &options, रिक्त,
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
			.मानम् = &gpg_sign,
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
		OPT_BOOL(0, "root", &options.मूलम्,
			 N_("rebase all reachable commits up to the root(s)")),
		OPT_BOOL(0, "reschedule-failed-exec",
			 &reschedule_failed_exec,
			 N_("automatically re-schedule any `exec` that fails")),
		OPT_BOOL(0, "reapply-cherry-picks", &options.reapply_cherry_picks,
			 N_("apply all changes, even those already present upstream")),
		OPT_END(),
	};
	पूर्णाङ्क i;

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
	gpg_sign = options.gpg_sign_opt ? "" : रिक्त;
	FREE_AND_NULL(options.gpg_sign_opt);

	strbuf_reset(&buf);
	strbuf_addf(&buf, "%s/applying", apply_dir());
	सचे(file_exists(buf.buf))
		die(_("It looks like 'git am' is in progress. Cannot rebase."));

	सचे (is_directory(apply_dir())) {
		options.type = REBASE_APPLY;
		options.state_dir = apply_dir();
	} अञ्ञथा सचे (is_directory(merge_dir())) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "%s/rewritten", merge_dir());
		सचे (!(options.action == ACTION_ABORT) && is_directory(buf.buf)) {
			die(_("`rebase --preserve-merges` (-p) is no longer supported.\n"
			"Use `git rebase --abort` to terminate current rebase.\n"
			"Or downgrade to v2.33, or earlier, to complete the rebase."));
		} अञ्ञथा {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "%s/interactive", merge_dir());
			options.type = REBASE_MERGE;
			सचे (file_exists(buf.buf))
				options.flags |= REBASE_INTERACTIVE_EXPLICIT;
		}
		options.state_dir = merge_dir();
	}

	सचे (options.type != REBASE_UNSPECIFIED)
		in_progress = 1;

	total_argc = argc;
	argc = parse_options(argc, argv, prefix,
			     builtin_rebase_options,
			     builtin_rebase_usage, 0);

	सचे (options.trailer_args.nr) {
		सचे (validate_trailer_args(&options.trailer_args))
			die(रिक्त);
		options.flags |= REBASE_FORCE;
	}

	सचे (preserve_merges_selected)
		die(_("--preserve-merges was replaced by --rebase-merges\n"
			"Note: Your `pull.rebase` configuration may also be set to 'preserve',\n"
			"which is no longer supported; use 'merges' instead"));

	सचे (options.action != ACTION_NONE && total_argc != 2) {
		usage_with_options(builtin_rebase_usage,
				   builtin_rebase_options);
	}

	सचे (argc > 2)
		usage_with_options(builtin_rebase_usage,
				   builtin_rebase_options);

	सचे (keep_base) {
		सचे (options.onto_name)
			die(_("options '%s' and '%s' cannot be used together"), "--keep-base", "--onto");
		सचे (options.मूलम्)
			die(_("options '%s' and '%s' cannot be used together"), "--keep-base", "--root");
		/*
		 * --keep-base defaults to --no-fork-point to keep the
		 * base the same.
		 */
		सचे (options.fork_point < 0)
			options.fork_point = 0;
	}
	सचे (options.मूलम् && options.fork_point > 0)
		die(_("options '%s' and '%s' cannot be used together"), "--root", "--fork-point");

	सचे (options.action != ACTION_NONE && !in_progress)
		die(_("no rebase in progress"));

	सचे (options.action == ACTION_EDIT_TODO && !is_merge(&options))
		die(_("The --edit-todo action can only be used during "
		      "interactive rebase."));

	सचे (trace2_is_enabled()) {
		सचे (is_merge(&options))
			trace2_cmd_mode("interactive");
		अञ्ञथा सचे (options.exec.nr)
			trace2_cmd_mode("interactive-exec");
		अञ्ञथा
			trace2_cmd_mode(action_names[options.action]);
	}

	options.reflog_action = getenv(GIT_REFLOG_ACTION_ENVIRONMENT);
	options.reflog_action =
		xstrdup(options.reflog_action ? options.reflog_action : "rebase");

	विकल्प (options.action) {
	पक्ष ACTION_CONTINUE: {
		संरचना object_id head;
		संरचना lock_file lock_file = LOCK_INIT;
		पूर्णाङ्क fd;

		/* Sanity check */
		सचे (repo_get_oid(the_repository, "HEAD", &head))
			die(_("Cannot read HEAD"));

		fd = repo_hold_locked_index(the_repository, &lock_file, 0);
		सचे (repo_read_index(the_repository) < 0)
			die(_("could not read index"));
		refresh_index(the_repository->index, REFRESH_QUIET, रिक्त, रिक्त,
			      रिक्त);
		सचे (0 <= fd)
			repo_update_index_if_able(the_repository, &lock_file);
		rollback_lock_file(&lock_file);

		सचे (has_unstaged_changes(the_repository, 1)) {
			puts(_("You must edit all merge conflicts and then\n"
			       "mark them as resolved using git add"));
			exit(1);
		}
		सचे (read_basic_state(&options))
			exit(1);
		गच्छ run_rebase;
	}
	पक्ष ACTION_SKIP: {
		संरचना string_list merge_rr = STRING_LIST_INIT_DUP;

		rerere_clear(the_repository, &merge_rr);
		string_list_clear(&merge_rr, 1);
		ropts.flags = RESET_WORKING_वृक्षः_HARD |
			      RESET_WORKING_वृक्षः_UPDATE_HEAD;
		सचे (reset_working_वृक्षः(the_repository, &ropts) < 0)
			die(_("could not discard worktree changes"));
		remove_branch_state(the_repository, 0);
		सचे (read_basic_state(&options))
			exit(1);
		गच्छ run_rebase;
	}
	पक्ष ACTION_ABORT: {
		संरचना string_list merge_rr = STRING_LIST_INIT_DUP;
		संरचना strbuf head_सन्देशः = STRBUF_INIT;

		rerere_clear(the_repository, &merge_rr);
		string_list_clear(&merge_rr, 1);

		सचे (read_basic_state(&options))
			exit(1);

		strbuf_addf(&head_सन्देशः, "%s (abort): returning to %s",
			    options.reflog_action,
			    options.head_name ? options.head_name
					      : oid_to_hex(&options.orig_head->object.oid));
		ropts.oid = &options.orig_head->object.oid;
		ropts.head_सन्देशः = head_सन्देशः.buf;
		ropts.branch = options.head_name;
		ropts.flags = RESET_WORKING_वृक्षः_HARD |
			      RESET_WORKING_वृक्षः_UPDATE_HEAD;
		सचे (reset_working_वृक्षः(the_repository, &ropts) < 0)
			die(_("could not move back to %s"),
			    oid_to_hex(&options.orig_head->object.oid));
		strbuf_release(&head_सन्देशः);
		remove_branch_state(the_repository, 0);
		ret = finish_rebase(&options);
		गच्छ cleanup;
	}
	पक्ष ACTION_QUIT: {
		save_autostash(state_dir_path("autostash", &options));
		सचे (options.type == REBASE_MERGE) {
			संरचना replay_opts replay = REPLAY_OPTS_INIT;

			replay.action = REPLAY_INTERACTIVE_REBASE;
			ret = sequencer_remove_state(&replay);
			replay_opts_release(&replay);
		} अञ्ञथा {
			strbuf_reset(&buf);
			strbuf_addstr(&buf, options.state_dir);
			ret = remove_dir_recursively(&buf, 0);
			सचे (ret)
				error(_("could not remove '%s'"),
				       options.state_dir);
		}
		गच्छ cleanup;
	}
	पक्ष ACTION_EDIT_TODO:
		options.dont_finish_rebase = 1;
		गच्छ run_rebase;
	पक्ष ACTION_SHOW_CURRENT_PATCH:
		options.dont_finish_rebase = 1;
		गच्छ run_rebase;
	पक्ष ACTION_NONE:
		भङ्ग;
	स्वभाव:
		BUG("action: %d", options.action);
	}

	/* Make sure no rebase is in progress */
	सचे (in_progress) {
		ध्रुव वर्ण *last_slash = strrchr(options.state_dir, '/');
		ध्रुव वर्ण *state_dir_base =
			last_slash ? last_slash + 1 : options.state_dir;
		ध्रुव वर्ण *cmd_live_rebase =
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

	सचे ((options.flags & REBASE_INTERACTIVE_EXPLICIT) ||
	    (options.action != ACTION_NONE) ||
	    (options.exec.nr > 0) ||
	    options.autosquash == 1) {
		allow_preemptive_ff = 0;
	}
	सचे (options.committer_date_is_author_date || options.ignore_date)
		options.flags |= REBASE_FORCE;

	अत्थाय (i = 0; i < options.git_am_opts.nr; i++) {
		ध्रुव वर्ण *option = options.git_am_opts.v[i], *p;
		सचे (!strcmp(option, "--whitespace=fix") ||
		    !strcmp(option, "--whitespace=strip"))
			allow_preemptive_ff = 0;
		अञ्ञथा सचे (skip_prefix(option, "-C", &p)) {
			याव (*p)
				सचे (!isdigit(*(p++)))
					die(_("switch `C' expects a "
					      "numerical value"));
		} अञ्ञथा सचे (skip_prefix(option, "--whitespace=", &p)) {
			सचे (*p && strcmp(p, "warn") && strcmp(p, "nowarn") &&
			    strcmp(p, "error") && strcmp(p, "error-all"))
				die("Invalid whitespace option: '%s'", p);
		}
	}

	अत्थाय (i = 0; i < options.exec.nr; i++)
		सचे (check_exec_cmd(options.exec.वस्तूनि[i].string))
			exit(1);

	सचे (!(options.flags & REBASE_NO_QUIET))
		strvec_प्रक्षिप(&options.git_am_opts, "-q");

	सचे (options.empty != EMPTY_UNSPECIFIED)
		imply_merge(&options, "--empty");

	सचे (options.reapply_cherry_picks < 0)
		/*
		 * We default to --no-reapply-cherry-picks unless
		 * --keep-base is given; when --keep-base is given, we want
		 * to default to --reapply-cherry-picks.
		 */
		options.reapply_cherry_picks = keep_base;
	अञ्ञथा सचे (!keep_base)
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

	सचे (gpg_sign)
		options.gpg_sign_opt = xstrfmt("-S%s", gpg_sign);

	सचे (options.exec.nr)
		imply_merge(&options, "--exec");

	सचे (options.type == REBASE_APPLY) {
		सचे (ignore_whitespace)
			strvec_प्रक्षिप(&options.git_am_opts,
				    "--ignore-whitespace");
		सचे (options.committer_date_is_author_date)
			strvec_प्रक्षिप(&options.git_am_opts,
				    "--committer-date-is-author-date");
		सचे (options.ignore_date)
			strvec_प्रक्षिप(&options.git_am_opts, "--ignore-date");
	} अञ्ञथा {
		/* REBASE_MERGE */
		सचे (ignore_whitespace) {
			string_list_append(&options.strategy_opts,
					   "ignore-space-change");
		}
	}

	सचे (strategy_opt)
		options.strategy = xstrdup(strategy_opt);
	अञ्ञथा सचे (options.strategy_opts.nr && !options.strategy)
		options.strategy = xstrdup("ort");
	सचे (options.strategy)
		imply_merge(&options, "--strategy");

	सचे (options.मूलम् && !options.onto_name)
		imply_merge(&options, "--root without --onto");

	सचे (options.trailer_args.nr)
		imply_merge(&options, "--trailer");

	सचे (isatty(2) && options.flags & REBASE_NO_QUIET)
		strbuf_addstr(&options.git_format_patch_opt, " --progress");

	सचे (options.git_am_opts.nr || options.type == REBASE_APPLY) {
		/* all am options except -q are compatible only with --apply */
		अत्थाय (i = options.git_am_opts.nr - 1; i >= 0; i--)
			सचे (strcmp(options.git_am_opts.v[i], "-q"))
				भङ्ग;

		सचे (i >= 0 || options.type == REBASE_APPLY) {
			सचे (is_merge(&options))
				die(_("apply options and merge options "
					  "cannot be used together"));
			अञ्ञथा सचे (options.rebase_merges == -1 && options.config_rebase_merges == 1)
				die(_("apply options are incompatible with rebase.rebaseMerges.  Consider adding --no-rebase-merges"));
			अञ्ञथा सचे (options.update_refs == -1 && options.config_update_refs == 1)
				die(_("apply options are incompatible with rebase.updateRefs.  Consider adding --no-update-refs"));
			अञ्ञथा
				options.type = REBASE_APPLY;
		}
	}

	सचे (options.update_refs == 1)
		imply_merge(&options, "--update-refs");
	options.update_refs = (options.update_refs >= 0) ? options.update_refs :
			     ((options.config_update_refs >= 0) ? options.config_update_refs : 0);

	सचे (options.rebase_merges == 1)
		imply_merge(&options, "--rebase-merges");
	options.rebase_merges = (options.rebase_merges >= 0) ? options.rebase_merges :
				((options.config_rebase_merges >= 0) ? options.config_rebase_merges : 0);

	सचे (options.autosquash == 1) {
		imply_merge(&options, "--autosquash");
	} अञ्ञथा सचे (options.autosquash == -1) {
		options.autosquash =
			options.config_autosquash &&
			(options.flags & REBASE_INTERACTIVE_EXPLICIT);
	}

	सचे (options.type == REBASE_UNSPECIFIED) {
		सचे (!strcmp(options.default_backend, "merge"))
			options.type = REBASE_MERGE;
		अञ्ञथा सचे (!strcmp(options.default_backend, "apply"))
			options.type = REBASE_APPLY;
		अञ्ञथा
			die(_("Unknown rebase backend: %s"),
			    options.default_backend);
	}

	विकल्प (options.type) {
	पक्ष REBASE_MERGE:
		options.state_dir = merge_dir();
		भङ्ग;
	पक्ष REBASE_APPLY:
		options.state_dir = apply_dir();
		भङ्ग;
	स्वभाव:
		BUG("options.type was just set above; should be unreachable.");
	}

	सचे (options.empty == EMPTY_UNSPECIFIED) {
		सचे (options.flags & REBASE_INTERACTIVE_EXPLICIT)
			options.empty = EMPTY_STOP;
		अञ्ञथा सचे (options.exec.nr > 0)
			options.empty = EMPTY_KEEP;
		अञ्ञथा
			options.empty = EMPTY_DROP;
	}
	सचे (reschedule_failed_exec > 0 && !is_merge(&options))
		die(_("--reschedule-failed-exec requires "
		      "--exec or --interactive"));
	सचे (reschedule_failed_exec >= 0)
		options.reschedule_failed_exec = reschedule_failed_exec;

	सचे (options.signoff) {
		strvec_प्रक्षिप(&options.git_am_opts, "--signoff");
		options.flags |= REBASE_FORCE;
	}

	सचे (!options.मूलम्) {
		सचे (argc < 1) {
			संरचना branch *branch;

			branch = branch_get(रिक्त);
			options.upstream_name = branch_get_upstream(branch,
								    रिक्त);
			सचे (!options.upstream_name)
				error_on_missing_default_upstream();
			सचे (options.fork_point < 0)
				options.fork_point = 1;
		} अञ्ञथा {
			options.upstream_name = argv[0];
			argc--;
			argv++;
			सचे (!strcmp(options.upstream_name, "-"))
				options.upstream_name = "@{-1}";
		}
		options.upstream =
			lookup_commit_reference_by_name(options.upstream_name);
		सचे (!options.upstream)
			die(_("invalid upstream '%s'"), options.upstream_name);
		options.upstream_arg = options.upstream_name;
	} अञ्ञथा {
		सचे (!options.onto_name) {
			सचे (commit_वृक्षः("", 0, the_hash_algo->empty_वृक्षः, रिक्त,
					&squash_onto, रिक्त, रिक्त) < 0)
				die(_("Could not create new root commit"));
			options.squash_onto = &squash_onto;
			options.onto_name = squash_onto_name =
				xstrdup(oid_to_hex(&squash_onto));
		} अञ्ञथा
			options.मूलम्_with_onto = 1;

		options.upstream_name = रिक्त;
		options.upstream = रिक्त;
		सचे (argc > 1)
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
	सचे (argc == 1) {
		/* Is it "rebase other branchname" or "rebase other commit"? */
		संरचना object_id branch_oid;
		branch_name = argv[0];
		options.switch_to = argv[0];

		/* Is it a local branch? */
		strbuf_reset(&buf);
		strbuf_addf(&buf, "refs/heads/%s", branch_name);
		सचे (!refs_read_ref(get_main_ref_store(the_repository), buf.buf, &branch_oid)) {
			die_if_checked_out(buf.buf, 1);
			options.head_name = xstrdup(buf.buf);
			options.orig_head =
				lookup_commit_object(the_repository,
						     &branch_oid);
		/* If not is it a valid ref (branch or commit)? */
		} अञ्ञथा {
			options.orig_head =
				lookup_commit_reference_by_name(branch_name);
			options.head_name = रिक्त;
		}
		सचे (!options.orig_head)
			die(_("no such branch/commit '%s'"), branch_name);
	} अञ्ञथा सचे (argc == 0) {
		/* Do not need to switch branches, we are already on it. */
		options.head_name =
			xstrdup_or_null(refs_resolve_ref_unsafe(get_main_ref_store(the_repository), "HEAD", 0, रिक्त,
								&flags));
		सचे (!options.head_name)
			die(_("No such ref: %s"), "HEAD");
		सचे (flags & REF_ISSYMREF) {
			सचे (!skip_prefix(options.head_name,
					 "refs/heads/", &branch_name))
				branch_name = options.head_name;

		} अञ्ञथा {
			FREE_AND_NULL(options.head_name);
			branch_name = "HEAD";
		}
		options.orig_head = lookup_commit_reference_by_name("HEAD");
		सचे (!options.orig_head)
			die(_("Could not resolve HEAD to a commit"));
	} अञ्ञथा
		BUG("unexpected number of arguments left to parse");

	/* Make sure the branch to rebase onto is valid. */
	सचे (keep_base) {
		strbuf_reset(&buf);
		strbuf_addstr(&buf, options.upstream_name);
		strbuf_addstr(&buf, "...");
		strbuf_addstr(&buf, branch_name);
		options.onto_name = keep_base_onto_name = xstrdup(buf.buf);
	} अञ्ञथा सचे (!options.onto_name)
		options.onto_name = options.upstream_name;
	सचे (strstr(options.onto_name, "...")) {
		सचे (repo_get_oid_mb(the_repository, options.onto_name, &branch_base) < 0) {
			सचे (keep_base)
				die(_("'%s': need exactly one merge base with branch"),
				    options.upstream_name);
			अञ्ञथा
				die(_("'%s': need exactly one merge base"),
				    options.onto_name);
		}
		options.onto = lookup_commit_or_die(&branch_base,
						    options.onto_name);
	} अञ्ञथा {
		options.onto =
			lookup_commit_reference_by_name(options.onto_name);
		सचे (!options.onto)
			die(_("Does not point to a valid commit '%s'"),
				options.onto_name);
		fill_branch_base(&options, &branch_base);
	}

	सचे (keep_base && options.reapply_cherry_picks)
		options.upstream = options.onto;

	सचे (options.fork_point > 0)
		options.restrict_revision =
			get_fork_point(options.upstream_name, options.orig_head);

	सचे (repo_read_index(the_repository) < 0)
		die(_("could not read index"));

	सचे (options.autostash)
		create_autostash(the_repository,
				 state_dir_path("autostash", &options));


	सचे (require_clean_work_वृक्षः(the_repository, "rebase",
				    _("Please commit or stash them."), 1, 1)) {
		ret = -1;
		गच्छ cleanup_autostash;
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
	सचे (allow_preemptive_ff &&
	    can_fast_forward(options.onto, options.upstream, options.restrict_revision,
			     options.orig_head, &branch_base)) {
		पूर्णाङ्क flag;

		सचे (!(options.flags & REBASE_FORCE)) {
			/* Lazily switch to the target branch if needed... */
			सचे (options.switch_to) {
				ret = checkout_up_to_date(&options);
				सचे (ret)
					गच्छ cleanup_autostash;
			}

			सचे (!(options.flags & REBASE_NO_QUIET))
				; /* be quiet */
			अञ्ञथा सचे (!strcmp(branch_name, "HEAD") &&
				 refs_resolve_ref_unsafe(get_main_ref_store(the_repository), "HEAD", 0, रिक्त, &flag))
				puts(_("HEAD is up to date."));
			अञ्ञथा
				printf(_("Current branch %s is up to date.\n"),
				       branch_name);
			ret = finish_rebase(&options);
			गच्छ cleanup;
		} अञ्ञथा सचे (!(options.flags & REBASE_NO_QUIET))
			; /* be quiet */
		अञ्ञथा सचे (!strcmp(branch_name, "HEAD") &&
			 refs_resolve_ref_unsafe(get_main_ref_store(the_repository), "HEAD", 0, रिक्त, &flag))
			puts(_("HEAD is up to date, rebase forced."));
		अञ्ञथा
			printf(_("Current branch %s is up to date, rebase "
				 "forced.\n"), branch_name);
	}

	/* If a hook exists, give it a chance to interrupt*/
	सचे (!ok_to_skip_pre_rebase &&
	    run_hooks_l(the_repository, "pre-rebase", options.upstream_arg,
			argc ? argv[0] : रिक्त, रिक्त)) {
		ret = error(_("The pre-rebase hook refused to rebase."));
		गच्छ cleanup_autostash;
	}

	सचे (options.flags & REBASE_DIFFSTAT) {
		संरचना diff_options opts;

		सचे (options.flags & REBASE_VERBOSE) {
			सचे (is_null_oid(&branch_base))
				printf(_("Changes to %s:\n"),
				       oid_to_hex(&options.onto->object.oid));
			अञ्ञथा
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
		diff_वृक्षः_oid(is_null_oid(&branch_base) ?
			      the_hash_algo->empty_वृक्षः : &branch_base,
			      &options.onto->object.oid, "", &opts);
		diffcore_std(&opts);
		diff_flush(&opts);
	}

	सचे (is_merge(&options))
		गच्छ run_rebase;

	/* Detach HEAD and reset the tree */
	सचे (options.flags & REBASE_NO_QUIET)
		printf(_("First, rewinding head to replay your work on top of "
			 "it...\n"));

	strbuf_addf(&सन्देशः, "%s (start): checkout %s",
		    options.reflog_action, options.onto_name);
	ropts.oid = &options.onto->object.oid;
	ropts.orig_head = &options.orig_head->object.oid;
	ropts.flags = RESET_WORKING_वृक्षः_DETACH |
		      RESET_WORKING_वृक्षः_UPDATE_HEAD |
		      RESET_WORKING_वृक्षः_UPDATE_ORIG_HEAD |
		      RESET_WORKING_वृक्षः_RUN_POST_CHECKOUT_HOOK;
	ropts.head_सन्देशः = सन्देशः.buf;
	ropts.default_reflog_action = options.reflog_action;
	सचे (reset_working_वृक्षः(the_repository, &ropts)) {
		ret = error(_("Could not detach HEAD"));
		गच्छ cleanup_autostash;
	}

	/*
	 * If the onto is a proper descendant of the tip of the branch, then
	 * we just fast-forwarded.
	 */
	सचे (oideq(&branch_base, &options.orig_head->object.oid)) {
		printf(_("Fast-forwarded %s to %s.\n"),
			branch_name, options.onto_name);
		move_to_original_branch(&options);
		ret = finish_rebase(&options);
		गच्छ cleanup;
	}

	strbuf_addf(&revisions, "%s..%s",
		    options.मूलम् ? oid_to_hex(&options.onto->object.oid) :
		    (options.restrict_revision ?
		     oid_to_hex(&options.restrict_revision->object.oid) :
		     oid_to_hex(&options.upstream->object.oid)),
		    oid_to_hex(&options.orig_head->object.oid));

	options.revisions = revisions.buf;

run_rebase:
	ret = run_specific_rebase(&options);

cleanup:
	strbuf_release(&buf);
	strbuf_release(&सन्देशः);
	strbuf_release(&revisions);
	rebase_options_release(&options);
	मुक्त(squash_onto_name);
	मुक्त(keep_base_onto_name);
	पटिनिवत्त !!ret;

cleanup_autostash:
	ret |= !!cleanup_autostash(&options);
	गच्छ cleanup;
}
