/*
 * "git rebase" builtin command
 *
 * Copyright (c) 2018 Pratik Karki
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#অন্তর্ভাব "builtin.h"

#অন্তর্ভাব "abspath.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "hex.h"
#অন্তর্ভাব "run-command.h"
#অন্তর্ভাব "strvec.h"
#অন্তর্ভাব "dir.h"
#অন্তর্ভাব "refs.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "unpack-trees.h"
#অন্তর্ভাব "lockfile.h"
#অন্তর্ভাব "object-file.h"
#অন্তর্ভাব "object-name.h"
#অন্তর্ভাব "parse-options.h"
#অন্তর্ভাব "path.h"
#অন্তর্ভাব "commit.h"
#অন্তর্ভাব "diff.h"
#অন্তর্ভাব "wt-status.h"
#অন্তর্ভাব "revision.h"
#অন্তর্ভাব "commit-reach.h"
#অন্তর্ভাব "rerere.h"
#অন্তর্ভাব "branch.h"
#অন্তর্ভাব "sequencer.h"
#অন্তর্ভাব "rebase-interactive.h"
#অন্তর্ভাব "reset.h"
#অন্তর্ভাব "trace2.h"
#অন্তর্ভাব "hook.h"
#অন্তর্ভাব "trailer.h"

স্থির অক্ষর ধ্রুবক * ধ্রুবক builtin_rebase_usage[] = {
	N_("git rebase [-i] [options] [--exec <cmd>] "
		"[--onto <newbase> | --keep-base] [<upstream> [<branch>]]"),
	N_("git rebase [-i] [options] [--exec <cmd>] [--onto <newbase>] "
		"--root [<branch>]"),
	"git rebase --continue | --abort | --skip | --edit-todo",
	ফাঁকা
};

স্থির GIT_PATH_FUNC(path_squash_onto, "rebase-merge/squash-onto")
স্থির GIT_PATH_FUNC(path_interactive, "rebase-merge/interactive")
স্থির GIT_PATH_FUNC(apply_dir, "rebase-apply")
স্থির GIT_PATH_FUNC(merge_dir, "rebase-merge")

গণনা rebase_type {
	REBASE_UNSPECIFIED = -1,
	REBASE_APPLY,
	REBASE_MERGE
};

গণনা empty_type {
	EMPTY_UNSPECIFIED = -1,
	EMPTY_DROP,
	EMPTY_KEEP,
	EMPTY_STOP
};

গণনা action {
	ACTION_NONE = 0,
	ACTION_CONTINUE,
	ACTION_SKIP,
	ACTION_ABORT,
	ACTION_QUIT,
	ACTION_EDIT_TODO,
	ACTION_SHOW_CURRENT_PATCH
};

স্থির ধ্রুবক অক্ষর *action_names[] = {
	"undefined",
	"continue",
	"skip",
	"abort",
	"quit",
	"edit_todo",
	"show_current_patch"
};

কাঠামো rebase_options {
	গণনা rebase_type type;
	গণনা empty_type empty;
	অক্ষর *default_backend;
	ধ্রুবক অক্ষর *state_dir;
	কাঠামো commit *upstream;
	ধ্রুবক অক্ষর *upstream_name;
	ধ্রুবক অক্ষর *upstream_arg;
	অক্ষর *শীর্ষ_name;
	কাঠামো commit *orig_শীর্ষ;
	কাঠামো commit *onto;
	ধ্রুবক অক্ষর *onto_name;
	ধ্রুবক অক্ষর *revisions;
	ধ্রুবক অক্ষর *switch_to;
	পূর্ণসংখ্যা মূল, মূল_with_onto;
	কাঠামো object_id *squash_onto;
	কাঠামো commit *restrict_revision;
	পূর্ণসংখ্যা dont_finish_rebase;
	গণনা {
		REBASE_NO_QUIET = 1<<0,
		REBASE_VERBOSE = 1<<1,
		REBASE_DIFFSTAT = 1<<2,
		REBASE_FORCE = 1<<3,
		REBASE_INTERACTIVE_EXPLICIT = 1<<4,
	} flags;
	কাঠামো strvec git_am_opts;
	গণনা action action;
	অক্ষর *reflog_action;
	পূর্ণসংখ্যা signoff;
	কাঠামো strvec trailer_args;
	পূর্ণসংখ্যা allow_rerere_autoupdate;
	পূর্ণসংখ্যা keep_empty;
	পূর্ণসংখ্যা autosquash;
	অক্ষর *gpg_sign_opt;
	পূর্ণসংখ্যা autostash;
	পূর্ণসংখ্যা committer_date_is_author_date;
	পূর্ণসংখ্যা ignore_date;
	কাঠামো string_list exec;
	পূর্ণসংখ্যা allow_empty_message;
	পূর্ণসংখ্যা rebase_merges, rebase_cousins;
	অক্ষর *strategy;
	কাঠামো string_list strategy_opts;
	কাঠামো strbuf git_format_patch_opt;
	পূর্ণসংখ্যা reschedule_failed_exec;
	পূর্ণসংখ্যা reapply_cherry_picks;
	পূর্ণসংখ্যা fork_point;
	পূর্ণসংখ্যা update_refs;
	পূর্ণসংখ্যা config_autosquash;
	পূর্ণসংখ্যা config_rebase_merges;
	পূর্ণসংখ্যা config_update_refs;
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

স্থির শূন্য rebase_options_release(কাঠামো rebase_options *opts)
{
	মুক্ত(opts->default_backend);
	মুক্ত(opts->reflog_action);
	মুক্ত(opts->শীর্ষ_name);
	strvec_clear(&opts->git_am_opts);
	মুক্ত(opts->gpg_sign_opt);
	string_list_clear(&opts->exec, 0);
	মুক্ত(opts->strategy);
	string_list_clear(&opts->strategy_opts, 0);
	strbuf_release(&opts->git_format_patch_opt);
	strvec_clear(&opts->trailer_args);
}

স্থির কাঠামো replay_opts get_replay_opts(ধ্রুবক কাঠামো rebase_options *opts)
{
	কাঠামো replay_opts replay = REPLAY_OPTS_INIT;

	replay.action = REPLAY_INTERACTIVE_REBASE;
	replay.strategy = ফাঁকা;
	sequencer_init_config(&replay);

	replay.signoff = opts->signoff;

	strvec_pushv(&replay.trailer_args, opts->trailer_args.v);

	replay.allow_ff = !(opts->flags & REBASE_FORCE);
	যদি (opts->allow_rerere_autoupdate)
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
	মুক্ত(replay.gpg_sign);
	replay.gpg_sign = xstrdup_or_null(opts->gpg_sign_opt);
	replay.reflog_action = xstrdup(opts->reflog_action);
	যদি (opts->strategy)
		replay.strategy = xstrdup_or_null(opts->strategy);
	অন্যথা যদি (!replay.strategy && replay.default_strategy) {
		replay.strategy = replay.default_strategy;
		replay.default_strategy = ফাঁকা;
	}

	জন্য (আকার_t i = 0; i < opts->strategy_opts.nr; i++)
		strvec_ঠেলো(&replay.xopts, opts->strategy_opts.বস্তুসমূহ[i].string);

	যদি (opts->squash_onto) {
		oidcpy(&replay.squash_onto, opts->squash_onto);
		replay.have_squash_onto = 1;
	}

	ফেরত replay;
}

স্থির পূর্ণসংখ্যা edit_todo_file(unsigned flags, কাঠামো replay_opts *opts)
{
	ধ্রুবক অক্ষর *todo_file = rebase_path_todo();
	কাঠামো todo_list todo_list = TODO_LIST_INIT,
		new_todo = TODO_LIST_INIT;
	পূর্ণসংখ্যা res = 0;

	যদি (strbuf_read_file(&todo_list.buf, todo_file, 0) < 0)
		ফেরত error_errno(_("could not read '%s'."), todo_file);

	strbuf_stripspace(&todo_list.buf, comment_সারি_str);
	res = edit_todo_list(the_repository, opts, &todo_list, &new_todo,
			     ফাঁকা, ফাঁকা, flags);
	যদি (!res && todo_list_write_to_file(the_repository, &new_todo, todo_file,
					    ফাঁকা, ফাঁকা, -1, flags & ~(TODO_LIST_SHORTEN_IDS)))
		res = error_errno(_("could not write '%s'"), todo_file);

	todo_list_release(&todo_list);
	todo_list_release(&new_todo);

	ফেরত res;
}

স্থির পূর্ণসংখ্যা get_revision_ranges(কাঠামো commit *upstream, কাঠামো commit *onto,
			       কাঠামো object_id *orig_শীর্ষ, অক্ষর **revisions,
			       অক্ষর **shortrevisions)
{
	কাঠামো commit *base_rev = upstream ? upstream : onto;
	ধ্রুবক অক্ষর *shorthead;

	*revisions = xstrfmt("%s...%s", oid_to_hex(&base_rev->object.oid),
			     oid_to_hex(orig_শীর্ষ));

	shorthead = repo_find_unique_abbrev(the_repository, orig_শীর্ষ,
					    DEFAULT_ABBREV);

	যদি (upstream) {
		ধ্রুবক অক্ষর *shortrev;

		shortrev = repo_find_unique_abbrev(the_repository,
						   &base_rev->object.oid,
						   DEFAULT_ABBREV);

		*shortrevisions = xstrfmt("%s..%s", shortrev, shorthead);
	} অন্যথা
		*shortrevisions = xstrdup(shorthead);

	ফেরত 0;
}

স্থির পূর্ণসংখ্যা init_basic_state(কাঠামো replay_opts *opts, ধ্রুবক অক্ষর *শীর্ষ_name,
			    কাঠামো commit *onto,
			    ধ্রুবক কাঠামো object_id *orig_শীর্ষ)
{
	FILE *interactive;

	যদি (!is_directory(merge_dir()) &&
	    safe_create_dir_in_gitdir(the_repository, merge_dir()))
		ফেরত error_errno(_("could not create temporary %s"), merge_dir());

	refs_delete_reflog(get_main_ref_store(the_repository), "REBASE_HEAD");

	interactive = fopen(path_interactive(), "w");
	যদি (!interactive)
		ফেরত error_errno(_("could not mark as interactive"));
	fclose(interactive);

	ফেরত write_basic_state(opts, শীর্ষ_name, onto, orig_শীর্ষ);
}

স্থির পূর্ণসংখ্যা do_interactive_rebase(কাঠামো rebase_options *opts, unsigned flags)
{
	পূর্ণসংখ্যা ret = -1;
	অক্ষর *revisions = ফাঁকা, *shortrevisions = ফাঁকা;
	কাঠামো strvec make_script_args = STRVEC_INIT;
	কাঠামো todo_list todo_list = TODO_LIST_INIT;
	কাঠামো replay_opts replay = get_replay_opts(opts);

	যদি (get_revision_ranges(opts->upstream, opts->onto, &opts->orig_শীর্ষ->object.oid,
				&revisions, &shortrevisions))
		যাও cleanup;

	strvec_pushl(&make_script_args, "", revisions, ফাঁকা);
	যদি (opts->restrict_revision)
		strvec_pushf(&make_script_args, "^%s",
			     oid_to_hex(&opts->restrict_revision->object.oid));

	ret = sequencer_make_script(the_repository, &todo_list.buf,
				    &make_script_args, flags);
	যদি (ret) {
		error(_("could not generate todo list"));
		যাও cleanup;
	}

	যদি (init_basic_state(&replay,
			     opts->শীর্ষ_name ? opts->শীর্ষ_name : "detached HEAD",
			     opts->onto, &opts->orig_শীর্ষ->object.oid))
		যাও cleanup;

	যদি (!opts->upstream && opts->squash_onto)
		write_file(path_squash_onto(), "%s\n",
			   oid_to_hex(opts->squash_onto));

	discard_সূচক(the_repository->সূচক);
	যদি (todo_list_parse_insn_বাফার(the_repository, &replay,
					todo_list.buf.buf, &todo_list))
		BUG("unusable todo list");

	ret = complete_action(the_repository, &replay, flags,
		shortrevisions, opts->onto_name, opts->onto,
		&opts->orig_শীর্ষ->object.oid, &opts->exec,
		opts->autosquash, opts->update_refs, &todo_list);

cleanup:
	replay_opts_release(&replay);
	মুক্ত(revisions);
	মুক্ত(shortrevisions);
	todo_list_release(&todo_list);
	strvec_clear(&make_script_args);

	ফেরত ret;
}

স্থির পূর্ণসংখ্যা run_sequencer_rebase(কাঠামো rebase_options *opts)
{
	unsigned flags = 0;
	পূর্ণসংখ্যা abbreviate_commands = 0, ret = 0;

	repo_config_get_bool(the_repository, "rebase.abbreviatecommands", &abbreviate_commands);

	flags |= opts->keep_empty ? TODO_LIST_KEEP_EMPTY : 0;
	flags |= abbreviate_commands ? TODO_LIST_ABBREVIATE_CMDS : 0;
	flags |= opts->rebase_merges ? TODO_LIST_REBASE_MERGES : 0;
	flags |= opts->rebase_cousins > 0 ? TODO_LIST_REBASE_COUSINS : 0;
	flags |= opts->মূল_with_onto ? TODO_LIST_মূল_WITH_ONTO : 0;
	flags |= opts->reapply_cherry_picks ? TODO_LIST_REAPPLY_CHERRY_PICKS : 0;
	flags |= opts->flags & REBASE_NO_QUIET ? TODO_LIST_WARN_SKIPPED_CHERRY_PICKS : 0;

	নির্বাচন (opts->action) {
	ক্ষেত্র ACTION_NONE: {
		যদি (!opts->onto && !opts->upstream)
			die(_("a base commit must be provided with --upstream or --onto"));

		ret = do_interactive_rebase(opts, flags);
		বিরতি;
	}
	ক্ষেত্র ACTION_SKIP: {
		কাঠামো string_list merge_rr = STRING_LIST_INIT_DUP;

		rerere_clear(the_repository, &merge_rr);
	}
		/* fallthrough */
	ক্ষেত্র ACTION_CONTINUE: {
		কাঠামো replay_opts replay_opts = get_replay_opts(opts);

		ret = sequencer_continue(the_repository, &replay_opts);
		replay_opts_release(&replay_opts);
		বিরতি;
	}
	ক্ষেত্র ACTION_EDIT_TODO: {
		কাঠামো replay_opts replay_opts = get_replay_opts(opts);

		ret = edit_todo_file(flags, &replay_opts);
		replay_opts_release(&replay_opts);
		বিরতি;
	}
	ক্ষেত্র ACTION_SHOW_CURRENT_PATCH: {
		কাঠামো child_process cmd = CHILD_PROCESS_INIT;

		cmd.git_cmd = 1;
		strvec_pushl(&cmd.args, "show", "REBASE_HEAD", "--", ফাঁকা);
		ret = run_command(&cmd);

		বিরতি;
	}
	পূর্বনির্ধারিত:
		BUG("invalid command '%d'", opts->action);
	}

	ফেরত ret;
}

স্থির পূর্ণসংখ্যা is_merge(কাঠামো rebase_options *opts)
{
	ফেরত opts->type == REBASE_MERGE;
}

স্থির শূন্য imply_merge(কাঠামো rebase_options *opts, ধ্রুবক অক্ষর *option)
{
	নির্বাচন (opts->type) {
	ক্ষেত্র REBASE_APPLY:
		die(_("%s requires the merge backend"), option);
		বিরতি;
	ক্ষেত্র REBASE_MERGE:
		বিরতি;
	পূর্বনির্ধারিত:
		opts->type = REBASE_MERGE; /* implied */
		বিরতি;
	}
}

/* Returns the filename prefixed by the state_dir */
স্থির ধ্রুবক অক্ষর *state_dir_path(ধ্রুবক অক্ষর *filename, কাঠামো rebase_options *opts)
{
	স্থির কাঠামো strbuf path = STRBUF_INIT;
	স্থির আকার_t prefix_len;

	যদি (!prefix_len) {
		strbuf_addf(&path, "%s/", opts->state_dir);
		prefix_len = path.len;
	}

	strbuf_setlen(&path, prefix_len);
	strbuf_addstr(&path, filename);
	ফেরত path.buf;
}

/* Initialize the rebase options from the state directory. */
স্থির পূর্ণসংখ্যা read_basic_state(কাঠামো rebase_options *opts)
{
	কাঠামো strbuf শীর্ষ_name = STRBUF_INIT;
	কাঠামো strbuf buf = STRBUF_INIT;
	কাঠামো object_id oid;

	যদি (!read_oneliner(&শীর্ষ_name, state_dir_path("head-name", opts),
			   READ_ONELINER_WARN_MISSING) ||
	    !read_oneliner(&buf, state_dir_path("onto", opts),
			   READ_ONELINER_WARN_MISSING))
		ফেরত -1;
	opts->শীর্ষ_name = starts_with(শীর্ষ_name.buf, "refs/") ?
		xstrdup(শীর্ষ_name.buf) : ফাঁকা;
	strbuf_release(&শীর্ষ_name);
	যদি (get_oid_hex(buf.buf, &oid) ||
	    !(opts->onto = lookup_commit_object(the_repository, &oid)))
		ফেরত error(_("invalid onto: '%s'"), buf.buf);

	/*
	 * We always write to orig-head, but interactive rebase used to write to
	 * head. Fall back to reading from head to cover for the case that the
	 * user upgraded git with an ongoing interactive rebase.
	 */
	strbuf_reset(&buf);
	যদি (file_exists(state_dir_path("orig-head", opts))) {
		যদি (!read_oneliner(&buf, state_dir_path("orig-head", opts),
				   READ_ONELINER_WARN_MISSING))
			ফেরত -1;
	} অন্যথা যদি (!read_oneliner(&buf, state_dir_path("head", opts),
				  READ_ONELINER_WARN_MISSING))
		ফেরত -1;
	যদি (get_oid_hex(buf.buf, &oid) ||
	    !(opts->orig_শীর্ষ = lookup_commit_object(the_repository, &oid)))
		ফেরত error(_("invalid orig-head: '%s'"), buf.buf);

	যদি (file_exists(state_dir_path("quiet", opts)))
		opts->flags &= ~REBASE_NO_QUIET;
	অন্যথা
		opts->flags |= REBASE_NO_QUIET;

	যদি (file_exists(state_dir_path("verbose", opts)))
		opts->flags |= REBASE_VERBOSE;

	যদি (file_exists(state_dir_path("signoff", opts))) {
		opts->signoff = 1;
		opts->flags |= REBASE_FORCE;
	}

	যদি (file_exists(state_dir_path("allow_rerere_autoupdate", opts))) {
		strbuf_reset(&buf);
		যদি (!read_oneliner(&buf, state_dir_path("allow_rerere_autoupdate", opts),
				   READ_ONELINER_WARN_MISSING))
			ফেরত -1;
		যদি (!strcmp(buf.buf, "--rerere-autoupdate"))
			opts->allow_rerere_autoupdate = RERERE_AUTOUPDATE;
		অন্যথা যদি (!strcmp(buf.buf, "--no-rerere-autoupdate"))
			opts->allow_rerere_autoupdate = RERERE_NOAUTOUPDATE;
		অন্যথা
			warning(_("ignoring invalid allow_rerere_autoupdate: "
				  "'%s'"), buf.buf);
	}

	যদি (file_exists(state_dir_path("gpg_sign_opt", opts))) {
		strbuf_reset(&buf);
		যদি (!read_oneliner(&buf, state_dir_path("gpg_sign_opt", opts),
				   READ_ONELINER_WARN_MISSING))
			ফেরত -1;
		মুক্ত(opts->gpg_sign_opt);
		opts->gpg_sign_opt = xstrdup(buf.buf);
	}

	strbuf_release(&buf);

	ফেরত 0;
}

স্থির পূর্ণসংখ্যা rebase_write_basic_state(কাঠামো rebase_options *opts)
{
	write_file(state_dir_path("head-name", opts), "%s",
		   opts->শীর্ষ_name ? opts->শীর্ষ_name : "detached HEAD");
	write_file(state_dir_path("onto", opts), "%s",
		   opts->onto ? oid_to_hex(&opts->onto->object.oid) : "");
	write_file(state_dir_path("orig-head", opts), "%s",
		   oid_to_hex(&opts->orig_শীর্ষ->object.oid));
	যদি (!(opts->flags & REBASE_NO_QUIET))
		write_file(state_dir_path("quiet", opts), "%s", "");
	যদি (opts->flags & REBASE_VERBOSE)
		write_file(state_dir_path("verbose", opts), "%s", "");
	যদি (opts->allow_rerere_autoupdate > 0)
		write_file(state_dir_path("allow_rerere_autoupdate", opts),
			   "-%s-rerere-autoupdate",
			   opts->allow_rerere_autoupdate == RERERE_AUTOUPDATE ?
				"" : "-no");
	যদি (opts->gpg_sign_opt)
		write_file(state_dir_path("gpg_sign_opt", opts), "%s",
			   opts->gpg_sign_opt);
	যদি (opts->signoff)
		write_file(state_dir_path("signoff", opts), "--signoff");

	ফেরত 0;
}

স্থির পূর্ণসংখ্যা cleanup_autostash(কাঠামো rebase_options *opts)
{
	পূর্ণসংখ্যা ret;
	কাঠামো strbuf dir = STRBUF_INIT;
	ধ্রুবক অক্ষর *path = state_dir_path("autostash", opts);

	যদি (!file_exists(path))
		ফেরত 0;
	ret = apply_autostash(path);
	strbuf_addstr(&dir, opts->state_dir);
	যদি (remove_dir_recursively(&dir, 0))
		ret = error_errno(_("could not remove '%s'"), opts->state_dir);
	strbuf_release(&dir);

	ফেরত ret;
}

স্থির পূর্ণসংখ্যা finish_rebase(কাঠামো rebase_options *opts)
{
	কাঠামো strbuf dir = STRBUF_INIT;
	পূর্ণসংখ্যা ret = 0;

	refs_delete_ref(get_main_ref_store(the_repository), ফাঁকা,
			"REBASE_HEAD", ফাঁকা, REF_NO_DEREF);
	refs_delete_ref(get_main_ref_store(the_repository), ফাঁকা,
			"AUTO_MERGE", ফাঁকা, REF_NO_DEREF);
	apply_autostash(state_dir_path("autostash", opts));
	/*
	 * We ignore errors in 'git maintenance run --auto', since the
	 * user should see them.
	 */
	run_auto_maintenance(the_repository,
			!(opts->flags & (REBASE_NO_QUIET|REBASE_VERBOSE)));

	যদি (opts->type == REBASE_MERGE) {
		কাঠামো replay_opts replay = REPLAY_OPTS_INIT;

		replay.action = REPLAY_INTERACTIVE_REBASE;
		ret = sequencer_remove_state(&replay);
		replay_opts_release(&replay);
	} অন্যথা {
		strbuf_addstr(&dir, opts->state_dir);
		যদি (remove_dir_recursively(&dir, 0))
			ret = error(_("could not remove '%s'"),
				    opts->state_dir);
		strbuf_release(&dir);
	}

	ফেরত ret;
}

স্থির পূর্ণসংখ্যা move_to_original_branch(কাঠামো rebase_options *opts)
{
	কাঠামো strbuf branch_reflog = STRBUF_INIT, শীর্ষ_reflog = STRBUF_INIT;
	কাঠামো reset_working_বৃক্ষ_options ropts = { 0 };
	পূর্ণসংখ্যা ret;

	যদি (!opts->শীর্ষ_name)
		ফেরত 0; /* nothing to move back to */

	যদি (!opts->onto)
		BUG("move_to_original_branch without onto");

	strbuf_addf(&branch_reflog, "%s (finish): %s onto %s",
		    opts->reflog_action,
		    opts->শীর্ষ_name, oid_to_hex(&opts->onto->object.oid));
	strbuf_addf(&শীর্ষ_reflog, "%s (finish): returning to %s",
		    opts->reflog_action, opts->শীর্ষ_name);
	ropts.branch = opts->শীর্ষ_name;
	ropts.flags = RESET_WORKING_বৃক্ষ_REFS_ONLY |
		      RESET_WORKING_বৃক্ষ_UPDATE_শীর্ষ;
	ropts.branch_বার্তা = branch_reflog.buf;
	ropts.শীর্ষ_বার্তা = শীর্ষ_reflog.buf;
	ret = reset_working_বৃক্ষ(the_repository, &ropts);

	strbuf_release(&branch_reflog);
	strbuf_release(&শীর্ষ_reflog);
	ফেরত ret;
}

স্থির পূর্ণসংখ্যা run_am(কাঠামো rebase_options *opts)
{
	কাঠামো child_process am = CHILD_PROCESS_INIT;
	কাঠামো child_process format_patch = CHILD_PROCESS_INIT;
	পূর্ণসংখ্যা status;
	অক্ষর *rebased_patches;

	am.git_cmd = 1;
	strvec_ঠেলো(&am.args, "am");
	strvec_pushf(&am.env, GIT_REFLOG_ACTION_ENVIRONMENT "=%s (pick)",
		     opts->reflog_action);
	যদি (opts->action == ACTION_CONTINUE) {
		strvec_ঠেলো(&am.args, "--resolved");
		strvec_pushf(&am.args, "--resolvemsg=%s", rebase_resolvemsg);
		যদি (opts->gpg_sign_opt)
			strvec_ঠেলো(&am.args, opts->gpg_sign_opt);
		status = run_command(&am);
		যদি (status)
			ফেরত status;

		ফেরত move_to_original_branch(opts);
	}
	যদি (opts->action == ACTION_SKIP) {
		strvec_ঠেলো(&am.args, "--skip");
		strvec_pushf(&am.args, "--resolvemsg=%s", rebase_resolvemsg);
		status = run_command(&am);
		যদি (status)
			ফেরত status;

		ফেরত move_to_original_branch(opts);
	}
	যদি (opts->action == ACTION_SHOW_CURRENT_PATCH) {
		strvec_ঠেলো(&am.args, "--show-current-patch");
		ফেরত run_command(&am);
	}

	rebased_patches = repo_git_path(the_repository, "rebased-patches");
	format_patch.out = open(rebased_patches,
				O_WRONLY | O_CREAT | O_TRUNC, 0666);
	যদি (format_patch.out < 0) {
		status = error_errno(_("could not open '%s' for writing"),
				     rebased_patches);
		মুক্ত(rebased_patches);
		child_process_clear(&am);
		ফেরত status;
	}

	format_patch.git_cmd = 1;
	strvec_pushl(&format_patch.args, "format-patch", "-k", "--stdout",
		     "--full-index", "--cherry-pick", "--right-only",
		     "--default-prefix", "--no-renames",
		     "--no-cover-letter", "--pretty=mboxrd", "--topo-order",
		     "--no-base", ফাঁকা);
	যদি (opts->git_format_patch_opt.len)
		strvec_split(&format_patch.args,
			     opts->git_format_patch_opt.buf);
	strvec_pushf(&format_patch.args, "%s...%s",
		     oid_to_hex(opts->মূল ?
				/* this is now equivalent to !opts->upstream */
				&opts->onto->object.oid :
				&opts->upstream->object.oid),
		     oid_to_hex(&opts->orig_শীর্ষ->object.oid));
	যদি (opts->restrict_revision)
		strvec_pushf(&format_patch.args, "^%s",
			     oid_to_hex(&opts->restrict_revision->object.oid));

	status = run_command(&format_patch);
	যদি (status) {
		কাঠামো reset_working_বৃক্ষ_options ropts = { 0 };
		unlink(rebased_patches);
		মুক্ত(rebased_patches);
		child_process_clear(&am);

		ropts.oid = &opts->orig_শীর্ষ->object.oid;
		ropts.branch = opts->শীর্ষ_name;
		ropts.default_reflog_action = opts->reflog_action;
		ropts.flags = RESET_WORKING_বৃক্ষ_UPDATE_শীর্ষ;
		reset_working_বৃক্ষ(the_repository, &ropts);
		error(_("\ngit encountered an error while preparing the "
			"patches to replay\n"
			"these revisions:\n"
			"\n    %s\n\n"
			"As a result, git cannot rebase them."),
		      opts->revisions);

		ফেরত status;
	}

	am.in = open(rebased_patches, O_RDONLY);
	যদি (am.in < 0) {
		status = error_errno(_("could not open '%s' for reading"),
				     rebased_patches);
		মুক্ত(rebased_patches);
		child_process_clear(&am);
		ফেরত status;
	}

	strvec_pushv(&am.args, opts->git_am_opts.v);
	strvec_ঠেলো(&am.args, "--rebasing");
	strvec_pushf(&am.args, "--resolvemsg=%s", rebase_resolvemsg);
	strvec_ঠেলো(&am.args, "--patch-format=mboxrd");
	যদি (opts->allow_rerere_autoupdate == RERERE_AUTOUPDATE)
		strvec_ঠেলো(&am.args, "--rerere-autoupdate");
	অন্যথা যদি (opts->allow_rerere_autoupdate == RERERE_NOAUTOUPDATE)
		strvec_ঠেলো(&am.args, "--no-rerere-autoupdate");
	যদি (opts->gpg_sign_opt)
		strvec_ঠেলো(&am.args, opts->gpg_sign_opt);
	status = run_command(&am);
	unlink(rebased_patches);
	মুক্ত(rebased_patches);

	যদি (!status) {
		ফেরত move_to_original_branch(opts);
	}

	যদি (is_directory(opts->state_dir))
		rebase_write_basic_state(opts);

	ফেরত status;
}

স্থির পূর্ণসংখ্যা run_specific_rebase(কাঠামো rebase_options *opts)
{
	পূর্ণসংখ্যা status;

	যদি (opts->type == REBASE_MERGE) {
		/* Run sequencer-based rebase */
		যদি (!(opts->flags & REBASE_INTERACTIVE_EXPLICIT))
			setenv("GIT_SEQUENCE_EDITOR", ":", 1);
		যদি (opts->gpg_sign_opt) {
			/* remove the leading "-S" */
			অক্ষর *tmp = xstrdup(opts->gpg_sign_opt + 2);
			মুক্ত(opts->gpg_sign_opt);
			opts->gpg_sign_opt = tmp;
		}

		status = run_sequencer_rebase(opts);
	} অন্যথা যদি (opts->type == REBASE_APPLY)
		status = run_am(opts);
	অন্যথা
		BUG("Unhandled rebase type %d", opts->type);

	যদি (opts->dont_finish_rebase)
		; /* do nothing */
	অন্যথা যদি (opts->type == REBASE_MERGE) {
		পূর্ণসংখ্যা quiet = !(opts->flags & (REBASE_NO_QUIET|REBASE_VERBOSE));

		/*
		 * The sequencer cleans up after itself. Its state directory
		 * is gone once it is done, and stays while it is stopped.
		 */
		যদি (status == 0 && !is_directory(opts->state_dir))
			run_auto_maintenance(the_repository, quiet);
	} অন্যথা যদি (status == 0) {
		যদি (!file_exists(state_dir_path("stopped-sha", opts)))
			finish_rebase(opts);
	} অন্যথা যদি (status == 2) {
		কাঠামো strbuf dir = STRBUF_INIT;

		apply_autostash(state_dir_path("autostash", opts));
		strbuf_addstr(&dir, opts->state_dir);
		remove_dir_recursively(&dir, 0);
		strbuf_release(&dir);
		die("Nothing to do");
	}

	ফেরত status ? -1 : 0;
}

স্থির শূন্য parse_rebase_merges_মান(কাঠামো rebase_options *options, ধ্রুবক অক্ষর *মান)
{
	যদি (!strcmp("no-rebase-cousins", মান))
		options->rebase_cousins = 0;
	অন্যথা যদি (!strcmp("rebase-cousins", মান))
		options->rebase_cousins = 1;
	অন্যথা
		die(_("Unknown rebase-merges mode: %s"), মান);
}

স্থির পূর্ণসংখ্যা rebase_config(ধ্রুবক অক্ষর *var, ধ্রুবক অক্ষর *মান,
			 ধ্রুবক কাঠামো config_context *ctx, শূন্য *তথ্য)
{
	কাঠামো rebase_options *opts = তথ্য;

	যদি (!strcmp(var, "rebase.stat")) {
		যদি (git_config_bool(var, মান))
			opts->flags |= REBASE_DIFFSTAT;
		অন্যথা
			opts->flags &= ~REBASE_DIFFSTAT;
		ফেরত 0;
	}

	যদি (!strcmp(var, "rebase.autosquash")) {
		opts->config_autosquash = git_config_bool(var, মান);
		ফেরত 0;
	}

	যদি (!strcmp(var, "commit.gpgsign")) {
		মুক্ত(opts->gpg_sign_opt);
		opts->gpg_sign_opt = git_config_bool(var, মান) ?
			xstrdup("-S") : ফাঁকা;
		ফেরত 0;
	}

	যদি (!strcmp(var, "rebase.autostash")) {
		opts->autostash = git_config_bool(var, মান);
		ফেরত 0;
	}

	যদি (!strcmp(var, "rebase.rebasemerges")) {
		opts->config_rebase_merges = git_parse_maybe_bool(মান);
		যদি (opts->config_rebase_merges < 0) {
			opts->config_rebase_merges = 1;
			parse_rebase_merges_মান(opts, মান);
		} অন্যথা {
			opts->rebase_cousins = 0;
		}
		ফেরত 0;
	}

	যদি (!strcmp(var, "rebase.updaterefs")) {
		opts->config_update_refs = git_config_bool(var, মান);
		ফেরত 0;
	}

	যদি (!strcmp(var, "rebase.reschedulefailedexec")) {
		opts->reschedule_failed_exec = git_config_bool(var, মান);
		ফেরত 0;
	}

	যদি (!strcmp(var, "rebase.forkpoint")) {
		opts->fork_point = git_config_bool(var, মান) ? -1 : 0;
		ফেরত 0;
	}

	যদি (!strcmp(var, "rebase.backend")) {
		FREE_AND_NULL(opts->default_backend);
		ফেরত git_config_string(&opts->default_backend, var, মান);
	}

	ফেরত git_default_config(var, মান, ctx, তথ্য);
}

স্থির পূর্ণসংখ্যা checkout_up_to_date(কাঠামো rebase_options *options)
{
	কাঠামো strbuf buf = STRBUF_INIT;
	কাঠামো reset_working_বৃক্ষ_options ropts = { 0 };
	পূর্ণসংখ্যা ret = 0;

	strbuf_addf(&buf, "%s: checkout %s",
		    options->reflog_action, options->switch_to);
	ropts.oid = &options->orig_শীর্ষ->object.oid;
	ropts.branch = options->শীর্ষ_name;
	ropts.flags = RESET_WORKING_বৃক্ষ_RUN_POST_CHECKOUT_HOOK |
		      RESET_WORKING_বৃক্ষ_UPDATE_শীর্ষ;
	যদি (!ropts.branch)
		ropts.flags |=  RESET_WORKING_বৃক্ষ_DETACH;
	ropts.শীর্ষ_বার্তা = buf.buf;
	যদি (reset_working_বৃক্ষ(the_repository, &ropts) < 0)
		ret = error(_("could not switch to %s"), options->switch_to);
	strbuf_release(&buf);

	ফেরত ret;
}

/*
 * Determines whether the commits in from..to are linear, i.e. contain
 * no merge commits. This function *expects* `from` to be an ancestor of
 * `to`.
 */
স্থির পূর্ণসংখ্যা is_linear_history(কাঠামো commit *from, কাঠামো commit *to)
{
	যখন (to && to != from) {
		repo_parse_commit(the_repository, to);
		যদি (!to->parents)
			ফেরত 1;
		যদি (to->parents->পরবর্তী)
			ফেরত 0;
		to = to->parents->item;
	}
	ফেরত 1;
}

স্থির পূর্ণসংখ্যা can_fast_forward(কাঠামো commit *onto, কাঠামো commit *upstream,
			    কাঠামো commit *restrict_revision,
			    কাঠামো commit *শীর্ষ, কাঠামো object_id *branch_base)
{
	কাঠামো commit_list *merge_bases = ফাঁকা;
	পূর্ণসংখ্যা res = 0;

	যদি (is_null_oid(branch_base))
		যাও done; /* fill_branch_base() found multiple merge bases */

	যদি (!oideq(branch_base, &onto->object.oid))
		যাও done;

	যদি (restrict_revision && !oideq(&restrict_revision->object.oid, branch_base))
		যাও done;

	যদি (!upstream)
		যাও done;

	যদি (repo_get_merge_bases(the_repository, upstream, শীর্ষ, &merge_bases) < 0)
		exit(128);
	যদি (!merge_bases || merge_bases->পরবর্তী)
		যাও done;

	যদি (!oideq(&onto->object.oid, &merge_bases->item->object.oid))
		যাও done;

	res = 1;

done:
	commit_list_free(merge_bases);
	ফেরত res && is_linear_history(onto, শীর্ষ);
}

স্থির শূন্য fill_branch_base(কাঠামো rebase_options *options,
			    কাঠামো object_id *branch_base)
{
	কাঠামো commit_list *merge_bases = ফাঁকা;

	যদি (repo_get_merge_bases(the_repository, options->onto,
				 options->orig_শীর্ষ, &merge_bases) < 0)
		exit(128);
	যদি (!merge_bases || merge_bases->পরবর্তী)
		oidcpy(branch_base, null_oid(the_hash_algo));
	অন্যথা
		oidcpy(branch_base, &merge_bases->item->object.oid);

	commit_list_free(merge_bases);
}

স্থির পূর্ণসংখ্যা parse_opt_am(ধ্রুবক কাঠামো option *opt, ধ্রুবক অক্ষর *arg, পূর্ণসংখ্যা unset)
{
	কাঠামো rebase_options *opts = opt->মান;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	যদি (opts->type != REBASE_UNSPECIFIED && opts->type != REBASE_APPLY)
	    die(_("apply options and merge options cannot be used together"));

	opts->type = REBASE_APPLY;

	ফেরত 0;
}

/* -i followed by -m is still -i */
স্থির পূর্ণসংখ্যা parse_opt_merge(ধ্রুবক কাঠামো option *opt, ধ্রুবক অক্ষর *arg, পূর্ণসংখ্যা unset)
{
	কাঠামো rebase_options *opts = opt->মান;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	যদি (opts->type != REBASE_UNSPECIFIED && opts->type != REBASE_MERGE)
	    die(_("apply options and merge options cannot be used together"));

	opts->type = REBASE_MERGE;

	ফেরত 0;
}

/* -i followed by -r is still explicitly interactive, but -r alone is not */
স্থির পূর্ণসংখ্যা parse_opt_interactive(ধ্রুবক কাঠামো option *opt, ধ্রুবক অক্ষর *arg,
				 পূর্ণসংখ্যা unset)
{
	কাঠামো rebase_options *opts = opt->মান;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	যদি (opts->type != REBASE_UNSPECIFIED && opts->type != REBASE_MERGE)
	    die(_("apply options and merge options cannot be used together"));

	opts->type = REBASE_MERGE;
	opts->flags |= REBASE_INTERACTIVE_EXPLICIT;

	ফেরত 0;
}

স্থির গণনা empty_type parse_empty_মান(ধ্রুবক অক্ষর *মান)
{
	যদি (!strcasecmp(মান, "drop"))
		ফেরত EMPTY_DROP;
	অন্যথা যদি (!strcasecmp(মান, "keep"))
		ফেরত EMPTY_KEEP;
	অন্যথা যদি (!strcasecmp(মান, "stop"))
		ফেরত EMPTY_STOP;
	অন্যথা যদি (!strcasecmp(মান, "ask")) {
		warning(_("--empty=ask is deprecated; use '--empty=stop' instead."));
		ফেরত EMPTY_STOP;
	}

	die(_("unrecognized empty type '%s'; valid values are \"drop\", \"keep\", and \"stop\"."), মান);
}

স্থির পূর্ণসংখ্যা parse_opt_keep_empty(ধ্রুবক কাঠামো option *opt, ধ্রুবক অক্ষর *arg,
				পূর্ণসংখ্যা unset)
{
	কাঠামো rebase_options *opts = opt->মান;

	BUG_ON_OPT_ARG(arg);

	imply_merge(opts, unset ? "--no-keep-empty" : "--keep-empty");
	opts->keep_empty = !unset;
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা parse_opt_empty(ধ্রুবক কাঠামো option *opt, ধ্রুবক অক্ষর *arg, পূর্ণসংখ্যা unset)
{
	কাঠামো rebase_options *options = opt->মান;
	গণনা empty_type মান = parse_empty_মান(arg);

	BUG_ON_OPT_NEG(unset);

	options->empty = মান;
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা parse_opt_rebase_merges(ধ্রুবক কাঠামো option *opt, ধ্রুবক অক্ষর *arg, পূর্ণসংখ্যা unset)
{
	কাঠামো rebase_options *options = opt->মান;

	options->rebase_merges = !unset;
	options->rebase_cousins = 0;

	যদি (arg) {
		যদি (!*arg) {
			warning(_("--rebase-merges with an empty string "
				  "argument is deprecated and will stop "
				  "working in a future version of Git. Use "
				  "--rebase-merges without an argument "
				  "instead, which does the same thing."));
			ফেরত 0;
		}
		parse_rebase_merges_মান(options, arg);
	}

	ফেরত 0;
}

স্থির শূন্য NORETURN error_on_missing_default_upstream(শূন্য)
{
	কাঠামো branch *current_branch = branch_get(ফাঁকা);

	printf(_("%s\n"
		 "Please specify which branch you want to rebase against.\n"
		 "See git-rebase(1) for details.\n"
		 "\n"
		 "    git rebase '<branch>'\n"
		 "\n"),
		current_branch ? _("There is no tracking information for "
			"the current branch.") :
			_("You are not currently on a branch."));

	যদি (current_branch) {
		ধ্রুবক অক্ষর *remote = current_branch->remote_name;

		যদি (!remote)
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

স্থির পূর্ণসংখ্যা check_exec_cmd(ধ্রুবক অক্ষর *cmd)
{
	যদি (strchr(cmd, '\n'))
		ফেরত error(_("exec commands cannot contain newlines"));

	/* Does the command consist purely of whitespace? */
	যদি (!cmd[strspn(cmd, " \t\r\f\v")])
		ফেরত error(_("empty exec command"));

	ফেরত 0;
}

পূর্ণসংখ্যা cmd_rebase(পূর্ণসংখ্যা argc,
	       ধ্রুবক অক্ষর **argv,
	       ধ্রুবক অক্ষর *prefix,
	       কাঠামো repository *repo UNUSED)
{
	কাঠামো rebase_options options = REBASE_OPTIONS_INIT;
	ধ্রুবক অক্ষর *branch_name;
	ধ্রুবক অক্ষর *strategy_opt = ফাঁকা;
	পূর্ণসংখ্যা ret, flags, যোগ_argc, in_progress = 0;
	পূর্ণসংখ্যা keep_base = 0;
	পূর্ণসংখ্যা ok_to_skip_pre_rebase = 0;
	কাঠামো strbuf বার্তা = STRBUF_INIT;
	কাঠামো strbuf revisions = STRBUF_INIT;
	কাঠামো strbuf buf = STRBUF_INIT;
	কাঠামো object_id branch_base;
	পূর্ণসংখ্যা ignore_whitespace = 0;
	ধ্রুবক অক্ষর *gpg_sign = ফাঁকা;
	কাঠামো object_id squash_onto;
	অক্ষর *squash_onto_name = ফাঁকা;
	অক্ষর *keep_base_onto_name = ফাঁকা;
	পূর্ণসংখ্যা reschedule_failed_exec = -1;
	পূর্ণসংখ্যা allow_preemptive_ff = 1;
	পূর্ণসংখ্যা preserve_merges_selected = 0;
	কাঠামো reset_working_বৃক্ষ_options ropts = { 0 };
	কাঠামো option builtin_rebase_options[] = {
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
			.মান = &options.flags,
			.precision = আকার(options.flags),
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
		OPT_PASSTHRU_ARGV('C', ফাঁকা, &options.git_am_opts, N_("n"),
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
		OPT_CALLBACK_F(0, "apply", &options, ফাঁকা,
			N_("use apply strategies to rebase"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			parse_opt_am),
		OPT_CALLBACK_F('m', "merge", &options, ফাঁকা,
			N_("use merging strategies to rebase"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			parse_opt_merge),
		OPT_CALLBACK_F('i', "interactive", &options, ফাঁকা,
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
		OPT_CALLBACK_F('k', "keep-empty", &options, ফাঁকা,
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
			.মান = &gpg_sign,
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
		OPT_BOOL(0, "root", &options.মূল,
			 N_("rebase all reachable commits up to the root(s)")),
		OPT_BOOL(0, "reschedule-failed-exec",
			 &reschedule_failed_exec,
			 N_("automatically re-schedule any `exec` that fails")),
		OPT_BOOL(0, "reapply-cherry-picks", &options.reapply_cherry_picks,
			 N_("apply all changes, even those already present upstream")),
		OPT_END(),
	};
	পূর্ণসংখ্যা i;

	show_usage_with_options_if_asked(argc, argv,
					 builtin_rebase_usage,
					 builtin_rebase_options);

#ifndef WITH_BREAKING_CHANGES
	warn_on_auto_comment_char = true;
#endif /* !WITH_BREAKING_CHANGES */
	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_সূচক = 0;

	repo_config(the_repository, rebase_config, &options);
	/* options.gpg_sign_opt will be either "-S" or NULL */
	gpg_sign = options.gpg_sign_opt ? "" : ফাঁকা;
	FREE_AND_NULL(options.gpg_sign_opt);

	strbuf_reset(&buf);
	strbuf_addf(&buf, "%s/applying", apply_dir());
	যদি(file_exists(buf.buf))
		die(_("It looks like 'git am' is in progress. Cannot rebase."));

	যদি (is_directory(apply_dir())) {
		options.type = REBASE_APPLY;
		options.state_dir = apply_dir();
	} অন্যথা যদি (is_directory(merge_dir())) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "%s/rewritten", merge_dir());
		যদি (!(options.action == ACTION_ABORT) && is_directory(buf.buf)) {
			die(_("`rebase --preserve-merges` (-p) is no longer supported.\n"
			"Use `git rebase --abort` to terminate current rebase.\n"
			"Or downgrade to v2.33, or earlier, to complete the rebase."));
		} অন্যথা {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "%s/interactive", merge_dir());
			options.type = REBASE_MERGE;
			যদি (file_exists(buf.buf))
				options.flags |= REBASE_INTERACTIVE_EXPLICIT;
		}
		options.state_dir = merge_dir();
	}

	যদি (options.type != REBASE_UNSPECIFIED)
		in_progress = 1;

	যোগ_argc = argc;
	argc = parse_options(argc, argv, prefix,
			     builtin_rebase_options,
			     builtin_rebase_usage, 0);

	যদি (options.trailer_args.nr) {
		যদি (validate_trailer_args(&options.trailer_args))
			die(ফাঁকা);
		options.flags |= REBASE_FORCE;
	}

	যদি (preserve_merges_selected)
		die(_("--preserve-merges was replaced by --rebase-merges\n"
			"Note: Your `pull.rebase` configuration may also be set to 'preserve',\n"
			"which is no longer supported; use 'merges' instead"));

	যদি (options.action != ACTION_NONE && যোগ_argc != 2) {
		usage_with_options(builtin_rebase_usage,
				   builtin_rebase_options);
	}

	যদি (argc > 2)
		usage_with_options(builtin_rebase_usage,
				   builtin_rebase_options);

	যদি (keep_base) {
		যদি (options.onto_name)
			die(_("options '%s' and '%s' cannot be used together"), "--keep-base", "--onto");
		যদি (options.মূল)
			die(_("options '%s' and '%s' cannot be used together"), "--keep-base", "--root");
		/*
		 * --keep-base defaults to --no-fork-point to keep the
		 * base the same.
		 */
		যদি (options.fork_point < 0)
			options.fork_point = 0;
	}
	যদি (options.মূল && options.fork_point > 0)
		die(_("options '%s' and '%s' cannot be used together"), "--root", "--fork-point");

	যদি (options.action != ACTION_NONE && !in_progress)
		die(_("no rebase in progress"));

	যদি (options.action == ACTION_EDIT_TODO && !is_merge(&options))
		die(_("The --edit-todo action can only be used during "
		      "interactive rebase."));

	যদি (trace2_is_enabled()) {
		যদি (is_merge(&options))
			trace2_cmd_mode("interactive");
		অন্যথা যদি (options.exec.nr)
			trace2_cmd_mode("interactive-exec");
		অন্যথা
			trace2_cmd_mode(action_names[options.action]);
	}

	options.reflog_action = getenv(GIT_REFLOG_ACTION_ENVIRONMENT);
	options.reflog_action =
		xstrdup(options.reflog_action ? options.reflog_action : "rebase");

	নির্বাচন (options.action) {
	ক্ষেত্র ACTION_CONTINUE: {
		কাঠামো object_id শীর্ষ;
		কাঠামো lock_file lock_file = LOCK_INIT;
		পূর্ণসংখ্যা fd;

		/* Sanity check */
		যদি (repo_get_oid(the_repository, "HEAD", &শীর্ষ))
			die(_("Cannot read HEAD"));

		fd = repo_hold_locked_সূচক(the_repository, &lock_file, 0);
		যদি (repo_read_সূচক(the_repository) < 0)
			die(_("could not read index"));
		refresh_সূচক(the_repository->সূচক, REFRESH_QUIET, ফাঁকা, ফাঁকা,
			      ফাঁকা);
		যদি (0 <= fd)
			repo_update_সূচক_if_able(the_repository, &lock_file);
		rollback_lock_file(&lock_file);

		যদি (has_unstaged_changes(the_repository, 1)) {
			puts(_("You must edit all merge conflicts and then\n"
			       "mark them as resolved using git add"));
			exit(1);
		}
		যদি (read_basic_state(&options))
			exit(1);
		যাও run_rebase;
	}
	ক্ষেত্র ACTION_SKIP: {
		কাঠামো string_list merge_rr = STRING_LIST_INIT_DUP;

		rerere_clear(the_repository, &merge_rr);
		string_list_clear(&merge_rr, 1);
		ropts.flags = RESET_WORKING_বৃক্ষ_HARD |
			      RESET_WORKING_বৃক্ষ_UPDATE_শীর্ষ;
		যদি (reset_working_বৃক্ষ(the_repository, &ropts) < 0)
			die(_("could not discard worktree changes"));
		remove_branch_state(the_repository, 0);
		যদি (read_basic_state(&options))
			exit(1);
		যাও run_rebase;
	}
	ক্ষেত্র ACTION_ABORT: {
		কাঠামো string_list merge_rr = STRING_LIST_INIT_DUP;
		কাঠামো strbuf শীর্ষ_বার্তা = STRBUF_INIT;

		rerere_clear(the_repository, &merge_rr);
		string_list_clear(&merge_rr, 1);

		যদি (read_basic_state(&options))
			exit(1);

		strbuf_addf(&শীর্ষ_বার্তা, "%s (abort): returning to %s",
			    options.reflog_action,
			    options.শীর্ষ_name ? options.শীর্ষ_name
					      : oid_to_hex(&options.orig_শীর্ষ->object.oid));
		ropts.oid = &options.orig_শীর্ষ->object.oid;
		ropts.শীর্ষ_বার্তা = শীর্ষ_বার্তা.buf;
		ropts.branch = options.শীর্ষ_name;
		ropts.flags = RESET_WORKING_বৃক্ষ_HARD |
			      RESET_WORKING_বৃক্ষ_UPDATE_শীর্ষ;
		যদি (reset_working_বৃক্ষ(the_repository, &ropts) < 0)
			die(_("could not move back to %s"),
			    oid_to_hex(&options.orig_শীর্ষ->object.oid));
		strbuf_release(&শীর্ষ_বার্তা);
		remove_branch_state(the_repository, 0);
		ret = finish_rebase(&options);
		যাও cleanup;
	}
	ক্ষেত্র ACTION_QUIT: {
		save_autostash(state_dir_path("autostash", &options));
		যদি (options.type == REBASE_MERGE) {
			কাঠামো replay_opts replay = REPLAY_OPTS_INIT;

			replay.action = REPLAY_INTERACTIVE_REBASE;
			ret = sequencer_remove_state(&replay);
			replay_opts_release(&replay);
		} অন্যথা {
			strbuf_reset(&buf);
			strbuf_addstr(&buf, options.state_dir);
			ret = remove_dir_recursively(&buf, 0);
			যদি (ret)
				error(_("could not remove '%s'"),
				       options.state_dir);
		}
		যাও cleanup;
	}
	ক্ষেত্র ACTION_EDIT_TODO:
		options.dont_finish_rebase = 1;
		যাও run_rebase;
	ক্ষেত্র ACTION_SHOW_CURRENT_PATCH:
		options.dont_finish_rebase = 1;
		যাও run_rebase;
	ক্ষেত্র ACTION_NONE:
		বিরতি;
	পূর্বনির্ধারিত:
		BUG("action: %d", options.action);
	}

	/* Make sure no rebase is in progress */
	যদি (in_progress) {
		ধ্রুবক অক্ষর *last_slash = strrchr(options.state_dir, '/');
		ধ্রুবক অক্ষর *state_dir_base =
			last_slash ? last_slash + 1 : options.state_dir;
		ধ্রুবক অক্ষর *cmd_live_rebase =
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

	যদি ((options.flags & REBASE_INTERACTIVE_EXPLICIT) ||
	    (options.action != ACTION_NONE) ||
	    (options.exec.nr > 0) ||
	    options.autosquash == 1) {
		allow_preemptive_ff = 0;
	}
	যদি (options.committer_date_is_author_date || options.ignore_date)
		options.flags |= REBASE_FORCE;

	জন্য (i = 0; i < options.git_am_opts.nr; i++) {
		ধ্রুবক অক্ষর *option = options.git_am_opts.v[i], *p;
		যদি (!strcmp(option, "--whitespace=fix") ||
		    !strcmp(option, "--whitespace=strip"))
			allow_preemptive_ff = 0;
		অন্যথা যদি (skip_prefix(option, "-C", &p)) {
			যখন (*p)
				যদি (!isdigit(*(p++)))
					die(_("switch `C' expects a "
					      "numerical value"));
		} অন্যথা যদি (skip_prefix(option, "--whitespace=", &p)) {
			যদি (*p && strcmp(p, "warn") && strcmp(p, "nowarn") &&
			    strcmp(p, "error") && strcmp(p, "error-all"))
				die("Invalid whitespace option: '%s'", p);
		}
	}

	জন্য (i = 0; i < options.exec.nr; i++)
		যদি (check_exec_cmd(options.exec.বস্তুসমূহ[i].string))
			exit(1);

	যদি (!(options.flags & REBASE_NO_QUIET))
		strvec_ঠেলো(&options.git_am_opts, "-q");

	যদি (options.empty != EMPTY_UNSPECIFIED)
		imply_merge(&options, "--empty");

	যদি (options.reapply_cherry_picks < 0)
		/*
		 * We default to --no-reapply-cherry-picks unless
		 * --keep-base is given; when --keep-base is given, we want
		 * to default to --reapply-cherry-picks.
		 */
		options.reapply_cherry_picks = keep_base;
	অন্যথা যদি (!keep_base)
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

	যদি (gpg_sign)
		options.gpg_sign_opt = xstrfmt("-S%s", gpg_sign);

	যদি (options.exec.nr)
		imply_merge(&options, "--exec");

	যদি (options.type == REBASE_APPLY) {
		যদি (ignore_whitespace)
			strvec_ঠেলো(&options.git_am_opts,
				    "--ignore-whitespace");
		যদি (options.committer_date_is_author_date)
			strvec_ঠেলো(&options.git_am_opts,
				    "--committer-date-is-author-date");
		যদি (options.ignore_date)
			strvec_ঠেলো(&options.git_am_opts, "--ignore-date");
	} অন্যথা {
		/* REBASE_MERGE */
		যদি (ignore_whitespace) {
			string_list_append(&options.strategy_opts,
					   "ignore-space-change");
		}
	}

	যদি (strategy_opt)
		options.strategy = xstrdup(strategy_opt);
	অন্যথা যদি (options.strategy_opts.nr && !options.strategy)
		options.strategy = xstrdup("ort");
	যদি (options.strategy)
		imply_merge(&options, "--strategy");

	যদি (options.মূল && !options.onto_name)
		imply_merge(&options, "--root without --onto");

	যদি (options.trailer_args.nr)
		imply_merge(&options, "--trailer");

	যদি (isatty(2) && options.flags & REBASE_NO_QUIET)
		strbuf_addstr(&options.git_format_patch_opt, " --progress");

	যদি (options.git_am_opts.nr || options.type == REBASE_APPLY) {
		/* all am options except -q are compatible only with --apply */
		জন্য (i = options.git_am_opts.nr - 1; i >= 0; i--)
			যদি (strcmp(options.git_am_opts.v[i], "-q"))
				বিরতি;

		যদি (i >= 0 || options.type == REBASE_APPLY) {
			যদি (is_merge(&options))
				die(_("apply options and merge options "
					  "cannot be used together"));
			অন্যথা যদি (options.rebase_merges == -1 && options.config_rebase_merges == 1)
				die(_("apply options are incompatible with rebase.rebaseMerges.  Consider adding --no-rebase-merges"));
			অন্যথা যদি (options.update_refs == -1 && options.config_update_refs == 1)
				die(_("apply options are incompatible with rebase.updateRefs.  Consider adding --no-update-refs"));
			অন্যথা
				options.type = REBASE_APPLY;
		}
	}

	যদি (options.update_refs == 1)
		imply_merge(&options, "--update-refs");
	options.update_refs = (options.update_refs >= 0) ? options.update_refs :
			     ((options.config_update_refs >= 0) ? options.config_update_refs : 0);

	যদি (options.rebase_merges == 1)
		imply_merge(&options, "--rebase-merges");
	options.rebase_merges = (options.rebase_merges >= 0) ? options.rebase_merges :
				((options.config_rebase_merges >= 0) ? options.config_rebase_merges : 0);

	যদি (options.autosquash == 1) {
		imply_merge(&options, "--autosquash");
	} অন্যথা যদি (options.autosquash == -1) {
		options.autosquash =
			options.config_autosquash &&
			(options.flags & REBASE_INTERACTIVE_EXPLICIT);
	}

	যদি (options.type == REBASE_UNSPECIFIED) {
		যদি (!strcmp(options.default_backend, "merge"))
			options.type = REBASE_MERGE;
		অন্যথা যদি (!strcmp(options.default_backend, "apply"))
			options.type = REBASE_APPLY;
		অন্যথা
			die(_("Unknown rebase backend: %s"),
			    options.default_backend);
	}

	নির্বাচন (options.type) {
	ক্ষেত্র REBASE_MERGE:
		options.state_dir = merge_dir();
		বিরতি;
	ক্ষেত্র REBASE_APPLY:
		options.state_dir = apply_dir();
		বিরতি;
	পূর্বনির্ধারিত:
		BUG("options.type was just set above; should be unreachable.");
	}

	যদি (options.empty == EMPTY_UNSPECIFIED) {
		যদি (options.flags & REBASE_INTERACTIVE_EXPLICIT)
			options.empty = EMPTY_STOP;
		অন্যথা যদি (options.exec.nr > 0)
			options.empty = EMPTY_KEEP;
		অন্যথা
			options.empty = EMPTY_DROP;
	}
	যদি (reschedule_failed_exec > 0 && !is_merge(&options))
		die(_("--reschedule-failed-exec requires "
		      "--exec or --interactive"));
	যদি (reschedule_failed_exec >= 0)
		options.reschedule_failed_exec = reschedule_failed_exec;

	যদি (options.signoff) {
		strvec_ঠেলো(&options.git_am_opts, "--signoff");
		options.flags |= REBASE_FORCE;
	}

	যদি (!options.মূল) {
		যদি (argc < 1) {
			কাঠামো branch *branch;

			branch = branch_get(ফাঁকা);
			options.upstream_name = branch_get_upstream(branch,
								    ফাঁকা);
			যদি (!options.upstream_name)
				error_on_missing_default_upstream();
			যদি (options.fork_point < 0)
				options.fork_point = 1;
		} অন্যথা {
			options.upstream_name = argv[0];
			argc--;
			argv++;
			যদি (!strcmp(options.upstream_name, "-"))
				options.upstream_name = "@{-1}";
		}
		options.upstream =
			lookup_commit_reference_by_name(options.upstream_name);
		যদি (!options.upstream)
			die(_("invalid upstream '%s'"), options.upstream_name);
		options.upstream_arg = options.upstream_name;
	} অন্যথা {
		যদি (!options.onto_name) {
			যদি (commit_বৃক্ষ("", 0, the_hash_algo->empty_বৃক্ষ, ফাঁকা,
					&squash_onto, ফাঁকা, ফাঁকা) < 0)
				die(_("Could not create new root commit"));
			options.squash_onto = &squash_onto;
			options.onto_name = squash_onto_name =
				xstrdup(oid_to_hex(&squash_onto));
		} অন্যথা
			options.মূল_with_onto = 1;

		options.upstream_name = ফাঁকা;
		options.upstream = ফাঁকা;
		যদি (argc > 1)
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
	যদি (argc == 1) {
		/* Is it "rebase other branchname" or "rebase other commit"? */
		কাঠামো object_id branch_oid;
		branch_name = argv[0];
		options.switch_to = argv[0];

		/* Is it a local branch? */
		strbuf_reset(&buf);
		strbuf_addf(&buf, "refs/heads/%s", branch_name);
		যদি (!refs_read_ref(get_main_ref_store(the_repository), buf.buf, &branch_oid)) {
			die_if_checked_out(buf.buf, 1);
			options.শীর্ষ_name = xstrdup(buf.buf);
			options.orig_শীর্ষ =
				lookup_commit_object(the_repository,
						     &branch_oid);
		/* If not is it a valid ref (branch or commit)? */
		} অন্যথা {
			options.orig_শীর্ষ =
				lookup_commit_reference_by_name(branch_name);
			options.শীর্ষ_name = ফাঁকা;
		}
		যদি (!options.orig_শীর্ষ)
			die(_("no such branch/commit '%s'"), branch_name);
	} অন্যথা যদি (argc == 0) {
		/* Do not need to switch branches, we are already on it. */
		options.শীর্ষ_name =
			xstrdup_or_null(refs_resolve_ref_unsafe(get_main_ref_store(the_repository), "HEAD", 0, ফাঁকা,
								&flags));
		যদি (!options.শীর্ষ_name)
			die(_("No such ref: %s"), "HEAD");
		যদি (flags & REF_ISSYMREF) {
			যদি (!skip_prefix(options.শীর্ষ_name,
					 "refs/heads/", &branch_name))
				branch_name = options.শীর্ষ_name;

		} অন্যথা {
			FREE_AND_NULL(options.শীর্ষ_name);
			branch_name = "HEAD";
		}
		options.orig_শীর্ষ = lookup_commit_reference_by_name("HEAD");
		যদি (!options.orig_শীর্ষ)
			die(_("Could not resolve HEAD to a commit"));
	} অন্যথা
		BUG("unexpected number of arguments left to parse");

	/* Make sure the branch to rebase onto is valid. */
	যদি (keep_base) {
		strbuf_reset(&buf);
		strbuf_addstr(&buf, options.upstream_name);
		strbuf_addstr(&buf, "...");
		strbuf_addstr(&buf, branch_name);
		options.onto_name = keep_base_onto_name = xstrdup(buf.buf);
	} অন্যথা যদি (!options.onto_name)
		options.onto_name = options.upstream_name;
	যদি (strstr(options.onto_name, "...")) {
		যদি (repo_get_oid_mb(the_repository, options.onto_name, &branch_base) < 0) {
			যদি (keep_base)
				die(_("'%s': need exactly one merge base with branch"),
				    options.upstream_name);
			অন্যথা
				die(_("'%s': need exactly one merge base"),
				    options.onto_name);
		}
		options.onto = lookup_commit_or_die(&branch_base,
						    options.onto_name);
	} অন্যথা {
		options.onto =
			lookup_commit_reference_by_name(options.onto_name);
		যদি (!options.onto)
			die(_("Does not point to a valid commit '%s'"),
				options.onto_name);
		fill_branch_base(&options, &branch_base);
	}

	যদি (keep_base && options.reapply_cherry_picks)
		options.upstream = options.onto;

	যদি (options.fork_point > 0)
		options.restrict_revision =
			get_fork_point(options.upstream_name, options.orig_শীর্ষ);

	যদি (repo_read_সূচক(the_repository) < 0)
		die(_("could not read index"));

	যদি (options.autostash)
		create_autostash(the_repository,
				 state_dir_path("autostash", &options));


	যদি (require_clean_work_বৃক্ষ(the_repository, "rebase",
				    _("Please commit or stash them."), 1, 1)) {
		ret = -1;
		যাও cleanup_autostash;
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
	যদি (allow_preemptive_ff &&
	    can_fast_forward(options.onto, options.upstream, options.restrict_revision,
			     options.orig_শীর্ষ, &branch_base)) {
		পূর্ণসংখ্যা flag;

		যদি (!(options.flags & REBASE_FORCE)) {
			/* Lazily switch to the target branch if needed... */
			যদি (options.switch_to) {
				ret = checkout_up_to_date(&options);
				যদি (ret)
					যাও cleanup_autostash;
			}

			যদি (!(options.flags & REBASE_NO_QUIET))
				; /* be quiet */
			অন্যথা যদি (!strcmp(branch_name, "HEAD") &&
				 refs_resolve_ref_unsafe(get_main_ref_store(the_repository), "HEAD", 0, ফাঁকা, &flag))
				puts(_("HEAD is up to date."));
			অন্যথা
				printf(_("Current branch %s is up to date.\n"),
				       branch_name);
			ret = finish_rebase(&options);
			যাও cleanup;
		} অন্যথা যদি (!(options.flags & REBASE_NO_QUIET))
			; /* be quiet */
		অন্যথা যদি (!strcmp(branch_name, "HEAD") &&
			 refs_resolve_ref_unsafe(get_main_ref_store(the_repository), "HEAD", 0, ফাঁকা, &flag))
			puts(_("HEAD is up to date, rebase forced."));
		অন্যথা
			printf(_("Current branch %s is up to date, rebase "
				 "forced.\n"), branch_name);
	}

	/* If a hook exists, give it a chance to interrupt*/
	যদি (!ok_to_skip_pre_rebase &&
	    run_hooks_l(the_repository, "pre-rebase", options.upstream_arg,
			argc ? argv[0] : ফাঁকা, ফাঁকা)) {
		ret = error(_("The pre-rebase hook refused to rebase."));
		যাও cleanup_autostash;
	}

	যদি (options.flags & REBASE_DIFFSTAT) {
		কাঠামো diff_options opts;

		যদি (options.flags & REBASE_VERBOSE) {
			যদি (is_null_oid(&branch_base))
				printf(_("Changes to %s:\n"),
				       oid_to_hex(&options.onto->object.oid));
			অন্যথা
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
		diff_বৃক্ষ_oid(is_null_oid(&branch_base) ?
			      the_hash_algo->empty_বৃক্ষ : &branch_base,
			      &options.onto->object.oid, "", &opts);
		diffcore_std(&opts);
		diff_flush(&opts);
	}

	যদি (is_merge(&options))
		যাও run_rebase;

	/* Detach HEAD and reset the tree */
	যদি (options.flags & REBASE_NO_QUIET)
		printf(_("First, rewinding head to replay your work on top of "
			 "it...\n"));

	strbuf_addf(&বার্তা, "%s (start): checkout %s",
		    options.reflog_action, options.onto_name);
	ropts.oid = &options.onto->object.oid;
	ropts.orig_শীর্ষ = &options.orig_শীর্ষ->object.oid;
	ropts.flags = RESET_WORKING_বৃক্ষ_DETACH |
		      RESET_WORKING_বৃক্ষ_UPDATE_শীর্ষ |
		      RESET_WORKING_বৃক্ষ_UPDATE_ORIG_শীর্ষ |
		      RESET_WORKING_বৃক্ষ_RUN_POST_CHECKOUT_HOOK;
	ropts.শীর্ষ_বার্তা = বার্তা.buf;
	ropts.default_reflog_action = options.reflog_action;
	যদি (reset_working_বৃক্ষ(the_repository, &ropts)) {
		ret = error(_("Could not detach HEAD"));
		যাও cleanup_autostash;
	}

	/*
	 * If the onto is a proper descendant of the tip of the branch, then
	 * we just fast-forwarded.
	 */
	যদি (oideq(&branch_base, &options.orig_শীর্ষ->object.oid)) {
		printf(_("Fast-forwarded %s to %s.\n"),
			branch_name, options.onto_name);
		move_to_original_branch(&options);
		ret = finish_rebase(&options);
		যাও cleanup;
	}

	strbuf_addf(&revisions, "%s..%s",
		    options.মূল ? oid_to_hex(&options.onto->object.oid) :
		    (options.restrict_revision ?
		     oid_to_hex(&options.restrict_revision->object.oid) :
		     oid_to_hex(&options.upstream->object.oid)),
		    oid_to_hex(&options.orig_শীর্ষ->object.oid));

	options.revisions = revisions.buf;

run_rebase:
	ret = run_specific_rebase(&options);

cleanup:
	strbuf_release(&buf);
	strbuf_release(&বার্তা);
	strbuf_release(&revisions);
	rebase_options_release(&options);
	মুক্ত(squash_onto_name);
	মুক্ত(keep_base_onto_name);
	ফেরত !!ret;

cleanup_autostash:
	ret |= !!cleanup_autostash(&options);
	যাও cleanup;
}
