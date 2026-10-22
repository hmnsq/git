/*
 * "git rebase" builtin command
 *
 * Copyright (c) 2018 Pratik Karki
 */

#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#تعریف_کرو DISABLE_SIGN_COMPARE_WARNINGS

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

ساکن حرف ثابت * ثابت builtin_rebase_usage[] = {
	N_("git rebase [-i] [options] [--exec <cmd>] "
		"[--onto <newbase> | --keep-base] [<upstream> [<branch>]]"),
	N_("git rebase [-i] [options] [--exec <cmd>] [--onto <newbase>] "
		"--root [<branch>]"),
	"git rebase --continue | --abort | --skip | --edit-todo",
	تہی
};

ساکن GIT_راستہ_FUNC(راستہ_squash_onto, "rebase-merge/squash-onto")
ساکن GIT_راستہ_FUNC(راستہ_interactive, "rebase-merge/interactive")
ساکن GIT_راستہ_FUNC(apply_dir, "rebase-apply")
ساکن GIT_راستہ_FUNC(ملاؤ_dir, "rebase-merge")

شمار rebase_type {
	REBASE_UNSPECIFIED = -1,
	REBASE_APPLY,
	REBASE_ملاؤ
};

شمار خالی_خانے_type {
	خالی_خانے_UNSPECIFIED = -1,
	خالی_خانے_DROP,
	خالی_خانے_KEEP,
	خالی_خانے_STOP
};

شمار action {
	ACTION_NONE = 0,
	ACTION_CONTINUE,
	ACTION_SKIP,
	ACTION_ABORT,
	ACTION_QUIT,
	ACTION_EDIT_TODO,
	ACTION_SHOW_CURRENT_PATCH
};

ساکن ثابت حرف *action_نام[] = {
	"undefined",
	"continue",
	"skip",
	"abort",
	"quit",
	"edit_todo",
	"show_current_patch"
};

ساخت rebase_options {
	شمار rebase_type type;
	شمار خالی_خانے_type خالی_خانے;
	حرف *default_backend;
	ثابت حرف *state_dir;
	ساخت commit *upstream;
	ثابت حرف *upstream_name;
	ثابت حرف *upstream_arg;
	حرف *سر_name;
	ساخت commit *orig_سر;
	ساخت commit *onto;
	ثابت حرف *onto_name;
	ثابت حرف *revisions;
	ثابت حرف *switch_مقصد;
	صحیح جڑ, جڑ_with_onto;
	ساخت object_id *squash_onto;
	ساخت commit *restrict_revision;
	صحیح dont_اختتام_rebase;
	شمار {
		REBASE_NO_QUIET = 1<<0,
		REBASE_VERBOSE = 1<<1,
		REBASE_DIFFSTAT = 1<<2,
		REBASE_FORCE = 1<<3,
		REBASE_INTERACTIVE_EXPLICIT = 1<<4,
	} flags;
	ساخت strvec git_am_opts;
	شمار action action;
	حرف *reflog_action;
	صحیح signoff;
	ساخت strvec trailer_دلائل;
	صحیح allow_rerere_autoupdate;
	صحیح keep_خالی_خانے;
	صحیح autosquash;
	حرف *gpg_sign_opt;
	صحیح autostash;
	صحیح committer_date_is_author_date;
	صحیح ignore_date;
	ساخت string_list exec;
	صحیح allow_خالی_خانے_پیغام;
	صحیح rebase_merges, rebase_cousins;
	حرف *strategy;
	ساخت string_list strategy_opts;
	ساخت strbuf git_format_patch_opt;
	صحیح reschedule_failed_exec;
	صحیح reapply_cherry_picks;
	صحیح fork_نقطہ;
	صحیح update_refs;
	صحیح config_autosquash;
	صحیح config_rebase_merges;
	صحیح config_update_refs;
};

#تعریف_کرو REBASE_OPTIONS_INIT {			  	\
		.type = REBASE_UNSPECIFIED,	  	\
		.خالی_خانے = خالی_خانے_UNSPECIFIED,	  	\
		.keep_خالی_خانے = 1,			\
		.default_backend = xstrdup("merge"),  	\
		.flags = REBASE_NO_QUIET, 		\
		.git_am_opts = STRVEC_INIT,		\
		.exec = STRING_LIST_INIT_NODUP,		\
		.trailer_دلائل = STRVEC_INIT,		\
		.git_format_patch_opt = STRBUF_INIT,	\
		.fork_نقطہ = -1,			\
		.reapply_cherry_picks = -1,             \
		.allow_خالی_خانے_پیغام = 1,               \
		.autosquash = -1,                       \
		.rebase_merges = -1,                    \
		.config_rebase_merges = -1,             \
		.update_refs = -1,                      \
		.config_update_refs = -1,               \
		.strategy_opts = STRING_LIST_INIT_NODUP,\
	}

ساکن خالی rebase_options_release(ساخت rebase_options *opts)
{
	آزاد(opts->default_backend);
	آزاد(opts->reflog_action);
	آزاد(opts->سر_name);
	strvec_clear(&opts->git_am_opts);
	آزاد(opts->gpg_sign_opt);
	string_list_clear(&opts->exec, 0);
	آزاد(opts->strategy);
	string_list_clear(&opts->strategy_opts, 0);
	strbuf_release(&opts->git_format_patch_opt);
	strvec_clear(&opts->trailer_دلائل);
}

ساکن ساخت replay_opts get_replay_opts(ثابت ساخت rebase_options *opts)
{
	ساخت replay_opts replay = REPLAY_OPTS_INIT;

	replay.action = REPLAY_INTERACTIVE_REBASE;
	replay.strategy = تہی;
	sequencer_init_config(&replay);

	replay.signoff = opts->signoff;

	strvec_pushv(&replay.trailer_دلائل, opts->trailer_دلائل.v);

	replay.allow_ff = !(opts->flags & REBASE_FORCE);
	اگر (opts->allow_rerere_autoupdate)
		replay.allow_rerere_auto = opts->allow_rerere_autoupdate;
	replay.allow_خالی_خانے = 1;
	replay.allow_خالی_خانے_پیغام = opts->allow_خالی_خانے_پیغام;
	replay.drop_redundant_commits = (opts->خالی_خانے == خالی_خانے_DROP);
	replay.keep_redundant_commits = (opts->خالی_خانے == خالی_خانے_KEEP);
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
	ورنہ اگر (!replay.strategy && replay.default_strategy) {
		replay.strategy = replay.default_strategy;
		replay.default_strategy = تہی;
	}

	برائے (سائز_t i = 0; i < opts->strategy_opts.nr; i++)
		strvec_ڈالو(&replay.xopts, opts->strategy_opts.اشیاء[i].string);

	اگر (opts->squash_onto) {
		oidcpy(&replay.squash_onto, opts->squash_onto);
		replay.have_squash_onto = 1;
	}

	واپس replay;
}

ساکن صحیح edit_todo_file(غیرعلامتی flags, ساخت replay_opts *opts)
{
	ثابت حرف *todo_file = rebase_راستہ_todo();
	ساخت todo_list todo_list = TODO_LIST_INIT,
		new_todo = TODO_LIST_INIT;
	صحیح res = 0;

	اگر (strbuf_read_file(&todo_list.buf, todo_file, 0) < 0)
		واپس error_errno(_("could not read '%s'."), todo_file);

	strbuf_stripspace(&todo_list.buf, comment_سطر_str);
	res = edit_todo_list(the_repository, opts, &todo_list, &new_todo,
			     تہی, تہی, flags);
	اگر (!res && todo_list_write_مقصد_file(the_repository, &new_todo, todo_file,
					    تہی, تہی, -1, flags & ~(TODO_LIST_SHORTEN_IDS)))
		res = error_errno(_("could not write '%s'"), todo_file);

	todo_list_release(&todo_list);
	todo_list_release(&new_todo);

	واپس res;
}

ساکن صحیح get_revision_ranges(ساخت commit *upstream, ساخت commit *onto,
			       ساخت object_id *orig_سر, حرف **revisions,
			       حرف **shortrevisions)
{
	ساخت commit *بنیاد_rev = upstream ? upstream : onto;
	ثابت حرف *shorthead;

	*revisions = xstrfmt("%s...%s", oid_مقصد_hex(&بنیاد_rev->object.oid),
			     oid_مقصد_hex(orig_سر));

	shorthead = repo_ڈھونڈو_unique_abbrev(the_repository, orig_سر,
					    DEFAULT_ABBREV);

	اگر (upstream) {
		ثابت حرف *shortrev;

		shortrev = repo_ڈھونڈو_unique_abbrev(the_repository,
						   &بنیاد_rev->object.oid,
						   DEFAULT_ABBREV);

		*shortrevisions = xstrfmt("%s..%s", shortrev, shorthead);
	} ورنہ
		*shortrevisions = xstrdup(shorthead);

	واپس 0;
}

ساکن صحیح init_basic_state(ساخت replay_opts *opts, ثابت حرف *سر_name,
			    ساخت commit *onto,
			    ثابت ساخت object_id *orig_سر)
{
	FILE *interactive;

	اگر (!is_directory(ملاؤ_dir()) &&
	    محفوظ_create_dir_اندر_gitdir(the_repository, ملاؤ_dir()))
		واپس error_errno(_("could not create temporary %s"), ملاؤ_dir());

	refs_delete_reflog(get_main_ref_store(the_repository), "REBASE_HEAD");

	interactive = fopen(راستہ_interactive(), "w");
	اگر (!interactive)
		واپس error_errno(_("could not mark as interactive"));
	fclose(interactive);

	واپس write_basic_state(opts, سر_name, onto, orig_سر);
}

ساکن صحیح do_interactive_rebase(ساخت rebase_options *opts, غیرعلامتی flags)
{
	صحیح ret = -1;
	حرف *revisions = تہی, *shortrevisions = تہی;
	ساخت strvec make_script_دلائل = STRVEC_INIT;
	ساخت todo_list todo_list = TODO_LIST_INIT;
	ساخت replay_opts replay = get_replay_opts(opts);

	اگر (get_revision_ranges(opts->upstream, opts->onto, &opts->orig_سر->object.oid,
				&revisions, &shortrevisions))
		جاؤ cleanup;

	strvec_pushl(&make_script_دلائل, "", revisions, تہی);
	اگر (opts->restrict_revision)
		strvec_pushf(&make_script_دلائل, "^%s",
			     oid_مقصد_hex(&opts->restrict_revision->object.oid));

	ret = sequencer_make_script(the_repository, &todo_list.buf,
				    &make_script_دلائل, flags);
	اگر (ret) {
		error(_("could not generate todo list"));
		جاؤ cleanup;
	}

	اگر (init_basic_state(&replay,
			     opts->سر_name ? opts->سر_name : "detached HEAD",
			     opts->onto, &opts->orig_سر->object.oid))
		جاؤ cleanup;

	اگر (!opts->upstream && opts->squash_onto)
		write_file(راستہ_squash_onto(), "%s\n",
			   oid_مقصد_hex(opts->squash_onto));

	discard_اشاریہ(the_repository->اشاریہ);
	اگر (todo_list_parse_insn_بفر(the_repository, &replay,
					todo_list.buf.buf, &todo_list))
		BUG("unusable todo list");

	ret = complete_action(the_repository, &replay, flags,
		shortrevisions, opts->onto_name, opts->onto,
		&opts->orig_سر->object.oid, &opts->exec,
		opts->autosquash, opts->update_refs, &todo_list);

cleanup:
	replay_opts_release(&replay);
	آزاد(revisions);
	آزاد(shortrevisions);
	todo_list_release(&todo_list);
	strvec_clear(&make_script_دلائل);

	واپس ret;
}

ساکن صحیح تسلسل_sequencer_rebase(ساخت rebase_options *opts)
{
	غیرعلامتی flags = 0;
	صحیح abbreviate_commands = 0, ret = 0;

	repo_config_get_bool(the_repository, "rebase.abbreviatecommands", &abbreviate_commands);

	flags |= opts->keep_خالی_خانے ? TODO_LIST_KEEP_خالی_خانے : 0;
	flags |= abbreviate_commands ? TODO_LIST_ABBREVIATE_CMDS : 0;
	flags |= opts->rebase_merges ? TODO_LIST_REBASE_MERGES : 0;
	flags |= opts->rebase_cousins > 0 ? TODO_LIST_REBASE_COUSINS : 0;
	flags |= opts->جڑ_with_onto ? TODO_LIST_جڑ_WITH_ONTO : 0;
	flags |= opts->reapply_cherry_picks ? TODO_LIST_REAPPLY_CHERRY_PICKS : 0;
	flags |= opts->flags & REBASE_NO_QUIET ? TODO_LIST_WARN_SKIPPED_CHERRY_PICKS : 0;

	انتخاب (opts->action) {
	حالت ACTION_NONE: {
		اگر (!opts->onto && !opts->upstream)
			die(_("a base commit must be provided with --upstream or --onto"));

		ret = do_interactive_rebase(opts, flags);
		توڑو;
	}
	حالت ACTION_SKIP: {
		ساخت string_list ملاؤ_rr = STRING_LIST_INIT_DUP;

		rerere_clear(the_repository, &ملاؤ_rr);
	}
		/* fallthrough */
	حالت ACTION_CONTINUE: {
		ساخت replay_opts replay_opts = get_replay_opts(opts);

		ret = sequencer_continue(the_repository, &replay_opts);
		replay_opts_release(&replay_opts);
		توڑو;
	}
	حالت ACTION_EDIT_TODO: {
		ساخت replay_opts replay_opts = get_replay_opts(opts);

		ret = edit_todo_file(flags, &replay_opts);
		replay_opts_release(&replay_opts);
		توڑو;
	}
	حالت ACTION_SHOW_CURRENT_PATCH: {
		ساخت بچہ_process cmd = بچہ_PROCESS_INIT;

		cmd.git_cmd = 1;
		strvec_pushl(&cmd.دلائل, "show", "REBASE_HEAD", "--", تہی);
		ret = تسلسل_command(&cmd);

		توڑو;
	}
	طےشدہ:
		BUG("invalid command '%d'", opts->action);
	}

	واپس ret;
}

ساکن صحیح is_ملاؤ(ساخت rebase_options *opts)
{
	واپس opts->type == REBASE_ملاؤ;
}

ساکن خالی imply_ملاؤ(ساخت rebase_options *opts, ثابت حرف *option)
{
	انتخاب (opts->type) {
	حالت REBASE_APPLY:
		die(_("%s requires the merge backend"), option);
		توڑو;
	حالت REBASE_ملاؤ:
		توڑو;
	طےشدہ:
		opts->type = REBASE_ملاؤ; /* implied */
		توڑو;
	}
}

/* Returns the filename prefixed by the state_dir */
ساکن ثابت حرف *state_dir_راستہ(ثابت حرف *filename, ساخت rebase_options *opts)
{
	ساکن ساخت strbuf راستہ = STRBUF_INIT;
	ساکن سائز_t سابقہ_len;

	اگر (!سابقہ_len) {
		strbuf_addf(&راستہ, "%s/", opts->state_dir);
		سابقہ_len = راستہ.len;
	}

	strbuf_setlen(&راستہ, سابقہ_len);
	strbuf_addstr(&راستہ, filename);
	واپس راستہ.buf;
}

/* Initialize the rebase options from the state directory. */
ساکن صحیح read_basic_state(ساخت rebase_options *opts)
{
	ساخت strbuf سر_name = STRBUF_INIT;
	ساخت strbuf buf = STRBUF_INIT;
	ساخت object_id oid;

	اگر (!read_oneliner(&سر_name, state_dir_راستہ("head-name", opts),
			   READ_ONELINER_WARN_MISSING) ||
	    !read_oneliner(&buf, state_dir_راستہ("onto", opts),
			   READ_ONELINER_WARN_MISSING))
		واپس -1;
	opts->سر_name = starts_with(سر_name.buf, "refs/") ?
		xstrdup(سر_name.buf) : تہی;
	strbuf_release(&سر_name);
	اگر (get_oid_hex(buf.buf, &oid) ||
	    !(opts->onto = lookup_commit_object(the_repository, &oid)))
		واپس error(_("invalid onto: '%s'"), buf.buf);

	/*
	 * We always write to orig-head, but interactive rebase used to write to
	 * head. Fall back to reading from head to cover for the case that the
	 * user upgraded git with an ongoing interactive rebase.
	 */
	strbuf_reset(&buf);
	اگر (file_exists(state_dir_راستہ("orig-head", opts))) {
		اگر (!read_oneliner(&buf, state_dir_راستہ("orig-head", opts),
				   READ_ONELINER_WARN_MISSING))
			واپس -1;
	} ورنہ اگر (!read_oneliner(&buf, state_dir_راستہ("head", opts),
				  READ_ONELINER_WARN_MISSING))
		واپس -1;
	اگر (get_oid_hex(buf.buf, &oid) ||
	    !(opts->orig_سر = lookup_commit_object(the_repository, &oid)))
		واپس error(_("invalid orig-head: '%s'"), buf.buf);

	اگر (file_exists(state_dir_راستہ("quiet", opts)))
		opts->flags &= ~REBASE_NO_QUIET;
	ورنہ
		opts->flags |= REBASE_NO_QUIET;

	اگر (file_exists(state_dir_راستہ("verbose", opts)))
		opts->flags |= REBASE_VERBOSE;

	اگر (file_exists(state_dir_راستہ("signoff", opts))) {
		opts->signoff = 1;
		opts->flags |= REBASE_FORCE;
	}

	اگر (file_exists(state_dir_راستہ("allow_rerere_autoupdate", opts))) {
		strbuf_reset(&buf);
		اگر (!read_oneliner(&buf, state_dir_راستہ("allow_rerere_autoupdate", opts),
				   READ_ONELINER_WARN_MISSING))
			واپس -1;
		اگر (!strcmp(buf.buf, "--rerere-autoupdate"))
			opts->allow_rerere_autoupdate = RERERE_AUTOUPDATE;
		ورنہ اگر (!strcmp(buf.buf, "--no-rerere-autoupdate"))
			opts->allow_rerere_autoupdate = RERERE_NOAUTOUPDATE;
		ورنہ
			warning(_("ignoring invalid allow_rerere_autoupdate: "
				  "'%s'"), buf.buf);
	}

	اگر (file_exists(state_dir_راستہ("gpg_sign_opt", opts))) {
		strbuf_reset(&buf);
		اگر (!read_oneliner(&buf, state_dir_راستہ("gpg_sign_opt", opts),
				   READ_ONELINER_WARN_MISSING))
			واپس -1;
		آزاد(opts->gpg_sign_opt);
		opts->gpg_sign_opt = xstrdup(buf.buf);
	}

	strbuf_release(&buf);

	واپس 0;
}

ساکن صحیح rebase_write_basic_state(ساخت rebase_options *opts)
{
	write_file(state_dir_راستہ("head-name", opts), "%s",
		   opts->سر_name ? opts->سر_name : "detached HEAD");
	write_file(state_dir_راستہ("onto", opts), "%s",
		   opts->onto ? oid_مقصد_hex(&opts->onto->object.oid) : "");
	write_file(state_dir_راستہ("orig-head", opts), "%s",
		   oid_مقصد_hex(&opts->orig_سر->object.oid));
	اگر (!(opts->flags & REBASE_NO_QUIET))
		write_file(state_dir_راستہ("quiet", opts), "%s", "");
	اگر (opts->flags & REBASE_VERBOSE)
		write_file(state_dir_راستہ("verbose", opts), "%s", "");
	اگر (opts->allow_rerere_autoupdate > 0)
		write_file(state_dir_راستہ("allow_rerere_autoupdate", opts),
			   "-%s-rerere-autoupdate",
			   opts->allow_rerere_autoupdate == RERERE_AUTOUPDATE ?
				"" : "-no");
	اگر (opts->gpg_sign_opt)
		write_file(state_dir_راستہ("gpg_sign_opt", opts), "%s",
			   opts->gpg_sign_opt);
	اگر (opts->signoff)
		write_file(state_dir_راستہ("signoff", opts), "--signoff");

	واپس 0;
}

ساکن صحیح cleanup_autostash(ساخت rebase_options *opts)
{
	صحیح ret;
	ساخت strbuf dir = STRBUF_INIT;
	ثابت حرف *راستہ = state_dir_راستہ("autostash", opts);

	اگر (!file_exists(راستہ))
		واپس 0;
	ret = apply_autostash(راستہ);
	strbuf_addstr(&dir, opts->state_dir);
	اگر (remove_dir_recursively(&dir, 0))
		ret = error_errno(_("could not remove '%s'"), opts->state_dir);
	strbuf_release(&dir);

	واپس ret;
}

ساکن صحیح اختتام_rebase(ساخت rebase_options *opts)
{
	ساخت strbuf dir = STRBUF_INIT;
	صحیح ret = 0;

	refs_delete_ref(get_main_ref_store(the_repository), تہی,
			"REBASE_HEAD", تہی, REF_NO_DEREF);
	refs_delete_ref(get_main_ref_store(the_repository), تہی,
			"AUTO_MERGE", تہی, REF_NO_DEREF);
	apply_autostash(state_dir_راستہ("autostash", opts));
	/*
	 * We ignore errors in 'git maintenance run --auto', since the
	 * user should see them.
	 */
	تسلسل_auto_maintenance(the_repository,
			!(opts->flags & (REBASE_NO_QUIET|REBASE_VERBOSE)));

	اگر (opts->type == REBASE_ملاؤ) {
		ساخت replay_opts replay = REPLAY_OPTS_INIT;

		replay.action = REPLAY_INTERACTIVE_REBASE;
		ret = sequencer_remove_state(&replay);
		replay_opts_release(&replay);
	} ورنہ {
		strbuf_addstr(&dir, opts->state_dir);
		اگر (remove_dir_recursively(&dir, 0))
			ret = error(_("could not remove '%s'"),
				    opts->state_dir);
		strbuf_release(&dir);
	}

	واپس ret;
}

ساکن صحیح move_مقصد_original_branch(ساخت rebase_options *opts)
{
	ساخت strbuf branch_reflog = STRBUF_INIT, سر_reflog = STRBUF_INIT;
	ساخت reset_working_درخت_options ropts = { 0 };
	صحیح ret;

	اگر (!opts->سر_name)
		واپس 0; /* nothing to move back to */

	اگر (!opts->onto)
		BUG("move_to_original_branch without onto");

	strbuf_addf(&branch_reflog, "%s (finish): %s onto %s",
		    opts->reflog_action,
		    opts->سر_name, oid_مقصد_hex(&opts->onto->object.oid));
	strbuf_addf(&سر_reflog, "%s (finish): returning to %s",
		    opts->reflog_action, opts->سر_name);
	ropts.branch = opts->سر_name;
	ropts.flags = RESET_WORKING_درخت_REFS_ONLY |
		      RESET_WORKING_درخت_UPDATE_سر;
	ropts.branch_پیغام = branch_reflog.buf;
	ropts.سر_پیغام = سر_reflog.buf;
	ret = reset_working_درخت(the_repository, &ropts);

	strbuf_release(&branch_reflog);
	strbuf_release(&سر_reflog);
	واپس ret;
}

ساکن صحیح تسلسل_am(ساخت rebase_options *opts)
{
	ساخت بچہ_process am = بچہ_PROCESS_INIT;
	ساخت بچہ_process format_patch = بچہ_PROCESS_INIT;
	صحیح حالت_2;
	حرف *rebased_patches;

	am.git_cmd = 1;
	strvec_ڈالو(&am.دلائل, "am");
	strvec_pushf(&am.env, GIT_REFLOG_ACTION_ENVIRONMENT "=%s (pick)",
		     opts->reflog_action);
	اگر (opts->action == ACTION_CONTINUE) {
		strvec_ڈالو(&am.دلائل, "--resolved");
		strvec_pushf(&am.دلائل, "--resolvemsg=%s", rebase_resolvemsg);
		اگر (opts->gpg_sign_opt)
			strvec_ڈالو(&am.دلائل, opts->gpg_sign_opt);
		حالت_2 = تسلسل_command(&am);
		اگر (حالت_2)
			واپس حالت_2;

		واپس move_مقصد_original_branch(opts);
	}
	اگر (opts->action == ACTION_SKIP) {
		strvec_ڈالو(&am.دلائل, "--skip");
		strvec_pushf(&am.دلائل, "--resolvemsg=%s", rebase_resolvemsg);
		حالت_2 = تسلسل_command(&am);
		اگر (حالت_2)
			واپس حالت_2;

		واپس move_مقصد_original_branch(opts);
	}
	اگر (opts->action == ACTION_SHOW_CURRENT_PATCH) {
		strvec_ڈالو(&am.دلائل, "--show-current-patch");
		واپس تسلسل_command(&am);
	}

	rebased_patches = repo_git_راستہ(the_repository, "rebased-patches");
	format_patch.برآمد = open(rebased_patches,
				O_WRONLY | O_CREAT | O_TRUNC, 0666);
	اگر (format_patch.برآمد < 0) {
		حالت_2 = error_errno(_("could not open '%s' for writing"),
				     rebased_patches);
		آزاد(rebased_patches);
		بچہ_process_clear(&am);
		واپس حالت_2;
	}

	format_patch.git_cmd = 1;
	strvec_pushl(&format_patch.دلائل, "format-patch", "-k", "--stdout",
		     "--full-index", "--cherry-pick", "--right-only",
		     "--default-prefix", "--no-renames",
		     "--no-cover-letter", "--pretty=mboxrd", "--topo-order",
		     "--no-base", تہی);
	اگر (opts->git_format_patch_opt.len)
		strvec_بانٹو(&format_patch.دلائل,
			     opts->git_format_patch_opt.buf);
	strvec_pushf(&format_patch.دلائل, "%s...%s",
		     oid_مقصد_hex(opts->جڑ ?
				/* this is now equivalent to !opts->upstream */
				&opts->onto->object.oid :
				&opts->upstream->object.oid),
		     oid_مقصد_hex(&opts->orig_سر->object.oid));
	اگر (opts->restrict_revision)
		strvec_pushf(&format_patch.دلائل, "^%s",
			     oid_مقصد_hex(&opts->restrict_revision->object.oid));

	حالت_2 = تسلسل_command(&format_patch);
	اگر (حالت_2) {
		ساخت reset_working_درخت_options ropts = { 0 };
		unlink(rebased_patches);
		آزاد(rebased_patches);
		بچہ_process_clear(&am);

		ropts.oid = &opts->orig_سر->object.oid;
		ropts.branch = opts->سر_name;
		ropts.default_reflog_action = opts->reflog_action;
		ropts.flags = RESET_WORKING_درخت_UPDATE_سر;
		reset_working_درخت(the_repository, &ropts);
		error(_("\ngit encountered an error while preparing the "
			"patches to replay\n"
			"these revisions:\n"
			"\n    %s\n\n"
			"As a result, git cannot rebase them."),
		      opts->revisions);

		واپس حالت_2;
	}

	am.اندر = open(rebased_patches, O_RDONLY);
	اگر (am.اندر < 0) {
		حالت_2 = error_errno(_("could not open '%s' for reading"),
				     rebased_patches);
		آزاد(rebased_patches);
		بچہ_process_clear(&am);
		واپس حالت_2;
	}

	strvec_pushv(&am.دلائل, opts->git_am_opts.v);
	strvec_ڈالو(&am.دلائل, "--rebasing");
	strvec_pushf(&am.دلائل, "--resolvemsg=%s", rebase_resolvemsg);
	strvec_ڈالو(&am.دلائل, "--patch-format=mboxrd");
	اگر (opts->allow_rerere_autoupdate == RERERE_AUTOUPDATE)
		strvec_ڈالو(&am.دلائل, "--rerere-autoupdate");
	ورنہ اگر (opts->allow_rerere_autoupdate == RERERE_NOAUTOUPDATE)
		strvec_ڈالو(&am.دلائل, "--no-rerere-autoupdate");
	اگر (opts->gpg_sign_opt)
		strvec_ڈالو(&am.دلائل, opts->gpg_sign_opt);
	حالت_2 = تسلسل_command(&am);
	unlink(rebased_patches);
	آزاد(rebased_patches);

	اگر (!حالت_2) {
		واپس move_مقصد_original_branch(opts);
	}

	اگر (is_directory(opts->state_dir))
		rebase_write_basic_state(opts);

	واپس حالت_2;
}

ساکن صحیح تسلسل_specific_rebase(ساخت rebase_options *opts)
{
	صحیح حالت_2;

	اگر (opts->type == REBASE_ملاؤ) {
		/* Run sequencer-based rebase */
		اگر (!(opts->flags & REBASE_INTERACTIVE_EXPLICIT))
			setenv("GIT_SEQUENCE_EDITOR", ":", 1);
		اگر (opts->gpg_sign_opt) {
			/* remove the leading "-S" */
			حرف *tmp = xstrdup(opts->gpg_sign_opt + 2);
			آزاد(opts->gpg_sign_opt);
			opts->gpg_sign_opt = tmp;
		}

		حالت_2 = تسلسل_sequencer_rebase(opts);
	} ورنہ اگر (opts->type == REBASE_APPLY)
		حالت_2 = تسلسل_am(opts);
	ورنہ
		BUG("Unhandled rebase type %d", opts->type);

	اگر (opts->dont_اختتام_rebase)
		; /* do nothing */
	ورنہ اگر (opts->type == REBASE_ملاؤ) {
		صحیح quiet = !(opts->flags & (REBASE_NO_QUIET|REBASE_VERBOSE));

		/*
		 * The sequencer cleans up after itself. Its state directory
		 * is gone once it is done, and stays while it is stopped.
		 */
		اگر (حالت_2 == 0 && !is_directory(opts->state_dir))
			تسلسل_auto_maintenance(the_repository, quiet);
	} ورنہ اگر (حالت_2 == 0) {
		اگر (!file_exists(state_dir_راستہ("stopped-sha", opts)))
			اختتام_rebase(opts);
	} ورنہ اگر (حالت_2 == 2) {
		ساخت strbuf dir = STRBUF_INIT;

		apply_autostash(state_dir_راستہ("autostash", opts));
		strbuf_addstr(&dir, opts->state_dir);
		remove_dir_recursively(&dir, 0);
		strbuf_release(&dir);
		die("Nothing to do");
	}

	واپس حالت_2 ? -1 : 0;
}

ساکن خالی parse_rebase_merges_قدر(ساخت rebase_options *options, ثابت حرف *قدر)
{
	اگر (!strcmp("no-rebase-cousins", قدر))
		options->rebase_cousins = 0;
	ورنہ اگر (!strcmp("rebase-cousins", قدر))
		options->rebase_cousins = 1;
	ورنہ
		die(_("Unknown rebase-merges mode: %s"), قدر);
}

ساکن صحیح rebase_config(ثابت حرف *var, ثابت حرف *قدر,
			 ثابت ساخت config_context *ctx, خالی *ڈیٹا)
{
	ساخت rebase_options *opts = ڈیٹا;

	اگر (!strcmp(var, "rebase.stat")) {
		اگر (git_config_bool(var, قدر))
			opts->flags |= REBASE_DIFFSTAT;
		ورنہ
			opts->flags &= ~REBASE_DIFFSTAT;
		واپس 0;
	}

	اگر (!strcmp(var, "rebase.autosquash")) {
		opts->config_autosquash = git_config_bool(var, قدر);
		واپس 0;
	}

	اگر (!strcmp(var, "commit.gpgsign")) {
		آزاد(opts->gpg_sign_opt);
		opts->gpg_sign_opt = git_config_bool(var, قدر) ?
			xstrdup("-S") : تہی;
		واپس 0;
	}

	اگر (!strcmp(var, "rebase.autostash")) {
		opts->autostash = git_config_bool(var, قدر);
		واپس 0;
	}

	اگر (!strcmp(var, "rebase.rebasemerges")) {
		opts->config_rebase_merges = git_parse_maybe_bool(قدر);
		اگر (opts->config_rebase_merges < 0) {
			opts->config_rebase_merges = 1;
			parse_rebase_merges_قدر(opts, قدر);
		} ورنہ {
			opts->rebase_cousins = 0;
		}
		واپس 0;
	}

	اگر (!strcmp(var, "rebase.updaterefs")) {
		opts->config_update_refs = git_config_bool(var, قدر);
		واپس 0;
	}

	اگر (!strcmp(var, "rebase.reschedulefailedexec")) {
		opts->reschedule_failed_exec = git_config_bool(var, قدر);
		واپس 0;
	}

	اگر (!strcmp(var, "rebase.forkpoint")) {
		opts->fork_نقطہ = git_config_bool(var, قدر) ? -1 : 0;
		واپس 0;
	}

	اگر (!strcmp(var, "rebase.backend")) {
		FREE_AND_NULL(opts->default_backend);
		واپس git_config_string(&opts->default_backend, var, قدر);
	}

	واپس git_default_config(var, قدر, ctx, ڈیٹا);
}

ساکن صحیح checkout_اوپر_مقصد_date(ساخت rebase_options *options)
{
	ساخت strbuf buf = STRBUF_INIT;
	ساخت reset_working_درخت_options ropts = { 0 };
	صحیح ret = 0;

	strbuf_addf(&buf, "%s: checkout %s",
		    options->reflog_action, options->switch_مقصد);
	ropts.oid = &options->orig_سر->object.oid;
	ropts.branch = options->سر_name;
	ropts.flags = RESET_WORKING_درخت_تسلسل_POST_CHECKOUT_HOOK |
		      RESET_WORKING_درخت_UPDATE_سر;
	اگر (!ropts.branch)
		ropts.flags |=  RESET_WORKING_درخت_DETACH;
	ropts.سر_پیغام = buf.buf;
	اگر (reset_working_درخت(the_repository, &ropts) < 0)
		ret = error(_("could not switch to %s"), options->switch_مقصد);
	strbuf_release(&buf);

	واپس ret;
}

/*
 * Determines whether the commits in from..to are linear, i.e. contain
 * no merge commits. This function *expects* `from` to be an ancestor of
 * `to`.
 */
ساکن صحیح is_خطی_history(ساخت commit *from, ساخت commit *مقصد)
{
	جب (مقصد && مقصد != from) {
		repo_parse_commit(the_repository, مقصد);
		اگر (!مقصد->parents)
			واپس 1;
		اگر (مقصد->parents->اگلا)
			واپس 0;
		مقصد = مقصد->parents->شے;
	}
	واپس 1;
}

ساکن صحیح can_fast_forward(ساخت commit *onto, ساخت commit *upstream,
			    ساخت commit *restrict_revision,
			    ساخت commit *سر, ساخت object_id *branch_بنیاد)
{
	ساخت commit_list *ملاؤ_bases = تہی;
	صحیح res = 0;

	اگر (is_null_oid(branch_بنیاد))
		جاؤ مکمل; /* fill_branch_base() found multiple merge bases */

	اگر (!oideq(branch_بنیاد, &onto->object.oid))
		جاؤ مکمل;

	اگر (restrict_revision && !oideq(&restrict_revision->object.oid, branch_بنیاد))
		جاؤ مکمل;

	اگر (!upstream)
		جاؤ مکمل;

	اگر (repo_get_ملاؤ_bases(the_repository, upstream, سر, &ملاؤ_bases) < 0)
		باہر_نکلو(128);
	اگر (!ملاؤ_bases || ملاؤ_bases->اگلا)
		جاؤ مکمل;

	اگر (!oideq(&onto->object.oid, &ملاؤ_bases->شے->object.oid))
		جاؤ مکمل;

	res = 1;

مکمل:
	commit_list_free(ملاؤ_bases);
	واپس res && is_خطی_history(onto, سر);
}

ساکن خالی fill_branch_بنیاد(ساخت rebase_options *options,
			    ساخت object_id *branch_بنیاد)
{
	ساخت commit_list *ملاؤ_bases = تہی;

	اگر (repo_get_ملاؤ_bases(the_repository, options->onto,
				 options->orig_سر, &ملاؤ_bases) < 0)
		باہر_نکلو(128);
	اگر (!ملاؤ_bases || ملاؤ_bases->اگلا)
		oidcpy(branch_بنیاد, null_oid(the_hash_algo));
	ورنہ
		oidcpy(branch_بنیاد, &ملاؤ_bases->شے->object.oid);

	commit_list_free(ملاؤ_bases);
}

ساکن صحیح parse_opt_am(ثابت ساخت option *opt, ثابت حرف *arg, صحیح unset)
{
	ساخت rebase_options *opts = opt->قدر;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	اگر (opts->type != REBASE_UNSPECIFIED && opts->type != REBASE_APPLY)
	    die(_("apply options and merge options cannot be used together"));

	opts->type = REBASE_APPLY;

	واپس 0;
}

/* -i followed by -m is still -i */
ساکن صحیح parse_opt_ملاؤ(ثابت ساخت option *opt, ثابت حرف *arg, صحیح unset)
{
	ساخت rebase_options *opts = opt->قدر;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	اگر (opts->type != REBASE_UNSPECIFIED && opts->type != REBASE_ملاؤ)
	    die(_("apply options and merge options cannot be used together"));

	opts->type = REBASE_ملاؤ;

	واپس 0;
}

/* -i followed by -r is still explicitly interactive, but -r alone is not */
ساکن صحیح parse_opt_interactive(ثابت ساخت option *opt, ثابت حرف *arg,
				 صحیح unset)
{
	ساخت rebase_options *opts = opt->قدر;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	اگر (opts->type != REBASE_UNSPECIFIED && opts->type != REBASE_ملاؤ)
	    die(_("apply options and merge options cannot be used together"));

	opts->type = REBASE_ملاؤ;
	opts->flags |= REBASE_INTERACTIVE_EXPLICIT;

	واپس 0;
}

ساکن شمار خالی_خانے_type parse_خالی_خانے_قدر(ثابت حرف *قدر)
{
	اگر (!strcasecmp(قدر, "drop"))
		واپس خالی_خانے_DROP;
	ورنہ اگر (!strcasecmp(قدر, "keep"))
		واپس خالی_خانے_KEEP;
	ورنہ اگر (!strcasecmp(قدر, "stop"))
		واپس خالی_خانے_STOP;
	ورنہ اگر (!strcasecmp(قدر, "ask")) {
		warning(_("--empty=ask is deprecated; use '--empty=stop' instead."));
		واپس خالی_خانے_STOP;
	}

	die(_("unrecognized empty type '%s'; valid values are \"drop\", \"keep\", and \"stop\"."), قدر);
}

ساکن صحیح parse_opt_keep_خالی_خانے(ثابت ساخت option *opt, ثابت حرف *arg,
				صحیح unset)
{
	ساخت rebase_options *opts = opt->قدر;

	BUG_ON_OPT_ARG(arg);

	imply_ملاؤ(opts, unset ? "--no-keep-empty" : "--keep-empty");
	opts->keep_خالی_خانے = !unset;
	واپس 0;
}

ساکن صحیح parse_opt_خالی_خانے(ثابت ساخت option *opt, ثابت حرف *arg, صحیح unset)
{
	ساخت rebase_options *options = opt->قدر;
	شمار خالی_خانے_type قدر = parse_خالی_خانے_قدر(arg);

	BUG_ON_OPT_NEG(unset);

	options->خالی_خانے = قدر;
	واپس 0;
}

ساکن صحیح parse_opt_rebase_merges(ثابت ساخت option *opt, ثابت حرف *arg, صحیح unset)
{
	ساخت rebase_options *options = opt->قدر;

	options->rebase_merges = !unset;
	options->rebase_cousins = 0;

	اگر (arg) {
		اگر (!*arg) {
			warning(_("--rebase-merges with an empty string "
				  "argument is deprecated and will stop "
				  "working in a future version of Git. Use "
				  "--rebase-merges without an argument "
				  "instead, which does the same thing."));
			واپس 0;
		}
		parse_rebase_merges_قدر(options, arg);
	}

	واپس 0;
}

ساکن خالی NORETURN error_on_missing_default_upstream(خالی)
{
	ساخت branch *current_branch = branch_get(تہی);

	لکھو(_("%s\n"
		 "Please specify which branch you want to rebase against.\n"
		 "See git-rebase(1) for details.\n"
		 "\n"
		 "    git rebase '<branch>'\n"
		 "\n"),
		current_branch ? _("There is no tracking information for "
			"the current branch.") :
			_("You are not currently on a branch."));

	اگر (current_branch) {
		ثابت حرف *remote = current_branch->remote_name;

		اگر (!remote)
			remote = _("<remote>");

		لکھو(_("If you wish to set tracking information for this "
			 "branch you can do so with:\n"
			 "\n"
			 "    git branch --set-upstream-to=%s/<branch> %s\n"
			 "\n"),
		       remote, current_branch->name);
	}
	باہر_نکلو(1);
}

ساکن صحیح check_exec_cmd(ثابت حرف *cmd)
{
	اگر (strchr(cmd, '\n'))
		واپس error(_("exec commands cannot contain newlines"));

	/* Does the command consist purely of whitespace? */
	اگر (!cmd[strspn(cmd, " \t\r\f\v")])
		واپس error(_("empty exec command"));

	واپس 0;
}

صحیح cmd_rebase(صحیح argc,
	       ثابت حرف **argv,
	       ثابت حرف *سابقہ,
	       ساخت repository *repo غیر_مستعمل)
{
	ساخت rebase_options options = REBASE_OPTIONS_INIT;
	ثابت حرف *branch_name;
	ثابت حرف *strategy_opt = تہی;
	صحیح ret, flags, کل_argc, اندر_پیش_رفت = 0;
	صحیح keep_بنیاد = 0;
	صحیح ok_مقصد_skip_pre_rebase = 0;
	ساخت strbuf پیغام = STRBUF_INIT;
	ساخت strbuf revisions = STRBUF_INIT;
	ساخت strbuf buf = STRBUF_INIT;
	ساخت object_id branch_بنیاد;
	صحیح ignore_whitespace = 0;
	ثابت حرف *gpg_sign = تہی;
	ساخت object_id squash_onto;
	حرف *squash_onto_name = تہی;
	حرف *keep_بنیاد_onto_name = تہی;
	صحیح reschedule_failed_exec = -1;
	صحیح allow_preemptive_ff = 1;
	صحیح preserve_merges_selected = 0;
	ساخت reset_working_درخت_options ropts = { 0 };
	ساخت option builtin_rebase_options[] = {
		OPT_STRING(0, "onto", &options.onto_name,
			   N_("revision"),
			   N_("rebase onto given branch instead of upstream")),
		OPT_BOOL(0, "keep-base", &keep_بنیاد,
			 N_("use the merge-base of upstream and branch as the current base")),
		OPT_BOOL(0, "no-verify", &ok_مقصد_skip_pre_rebase,
			 N_("allow pre-rebase hook to run")),
		OPT_NEGBIT('q', "quiet", &options.flags,
			   N_("be quiet. implies --no-stat"),
			   REBASE_NO_QUIET | REBASE_VERBOSE | REBASE_DIFFSTAT),
		OPT_ثنائی_ہندسہ('v', "verbose", &options.flags,
			N_("display a diffstat of what changed upstream"),
			REBASE_NO_QUIET | REBASE_VERBOSE | REBASE_DIFFSTAT),
		{
			.type = OPTION_NEGBIT,
			.short_name = 'n',
			.long_name = "no-stat",
			.قدر = &options.flags,
			.precision = حجم(options.flags),
			.help = N_("do not show diffstat of what changed upstream"),
			.flags = PARSE_OPT_NOARG,
			.defval = REBASE_DIFFSTAT,
		},
		OPT_STRVEC(0, "trailer", &options.trailer_دلائل, N_("trailer"),
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
		OPT_PASSTHRU_ARGV('C', تہی, &options.git_am_opts, N_("n"),
				  N_("passed to 'git apply'"), 0),
		OPT_BOOL(0, "ignore-whitespace", &ignore_whitespace,
			 N_("ignore changes in whitespace")),
		OPT_PASSTHRU_ARGV(0, "whitespace", &options.git_am_opts,
				  N_("action"), N_("passed to 'git apply'"), 0),
		OPT_ثنائی_ہندسہ('f', "force-rebase", &options.flags,
			N_("cherry-pick all commits, even if unchanged"),
			REBASE_FORCE),
		OPT_ثنائی_ہندسہ(0, "no-ff", &options.flags,
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
		OPT_CALLBACK_F(0, "apply", &options, تہی,
			N_("use apply strategies to rebase"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			parse_opt_am),
		OPT_CALLBACK_F('m', "merge", &options, تہی,
			N_("use merging strategies to rebase"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			parse_opt_ملاؤ),
		OPT_CALLBACK_F('i', "interactive", &options, تہی,
			N_("let the user edit the list of commits to rebase"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			parse_opt_interactive),
		OPT_سیٹ_INT_F('p', "preserve-merges", &preserve_merges_selected,
			      N_("(REMOVED) was: try to recreate merges "
				 "instead of ignoring them"),
			      1, PARSE_OPT_HIDDEN),
		OPT_RERERE_AUTOUPDATE(&options.allow_rerere_autoupdate),
		OPT_CALLBACK_F(0, "empty", &options, "(drop|keep|stop)",
			       N_("how to handle commits that become empty"),
			       PARSE_OPT_NONEG, parse_opt_خالی_خانے),
		OPT_CALLBACK_F('k', "keep-empty", &options, تہی,
			N_("keep commits which start empty"),
			PARSE_OPT_NOARG | PARSE_OPT_HIDDEN,
			parse_opt_keep_خالی_خانے),
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
			.قدر = &gpg_sign,
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
			   &options.allow_خالی_خانے_پیغام,
			   N_("allow rebasing commits with empty messages"),
			   PARSE_OPT_HIDDEN),
		OPT_CALLBACK_F('r', "rebase-merges", &options, N_("mode"),
			N_("try to rebase merges instead of skipping them"),
			PARSE_OPT_OPTARG, parse_opt_rebase_merges),
		OPT_BOOL(0, "fork-point", &options.fork_نقطہ,
			 N_("use 'merge-base --fork-point' to refine upstream")),
		OPT_STRING('s', "strategy", &strategy_opt,
			   N_("strategy"), N_("use the given merge strategy")),
		OPT_STRING_LIST('X', "strategy-option", &options.strategy_opts,
				N_("option"),
				N_("pass the argument through to the merge "
				   "strategy")),
		OPT_BOOL(0, "root", &options.جڑ,
			 N_("rebase all reachable commits up to the root(s)")),
		OPT_BOOL(0, "reschedule-failed-exec",
			 &reschedule_failed_exec,
			 N_("automatically re-schedule any `exec` that fails")),
		OPT_BOOL(0, "reapply-cherry-picks", &options.reapply_cherry_picks,
			 N_("apply all changes, even those already present upstream")),
		OPT_انتہا(),
	};
	صحیح i;

	show_usage_with_options_if_asked(argc, argv,
					 builtin_rebase_usage,
					 builtin_rebase_options);

#اگر_غیر_تعریف_شدہ WITH_BREAKING_CHANGES
	warn_on_auto_comment_char = true;
#اگر_ختم /* !WITH_BREAKING_CHANGES */
	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_بھرا_اشاریہ = 0;

	repo_config(the_repository, rebase_config, &options);
	/* options.gpg_sign_opt will be either "-S" or NULL */
	gpg_sign = options.gpg_sign_opt ? "" : تہی;
	FREE_AND_NULL(options.gpg_sign_opt);

	strbuf_reset(&buf);
	strbuf_addf(&buf, "%s/applying", apply_dir());
	اگر(file_exists(buf.buf))
		die(_("It looks like 'git am' is in progress. Cannot rebase."));

	اگر (is_directory(apply_dir())) {
		options.type = REBASE_APPLY;
		options.state_dir = apply_dir();
	} ورنہ اگر (is_directory(ملاؤ_dir())) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "%s/rewritten", ملاؤ_dir());
		اگر (!(options.action == ACTION_ABORT) && is_directory(buf.buf)) {
			die(_("`rebase --preserve-merges` (-p) is no longer supported.\n"
			"Use `git rebase --abort` to terminate current rebase.\n"
			"Or downgrade to v2.33, or earlier, to complete the rebase."));
		} ورنہ {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "%s/interactive", ملاؤ_dir());
			options.type = REBASE_ملاؤ;
			اگر (file_exists(buf.buf))
				options.flags |= REBASE_INTERACTIVE_EXPLICIT;
		}
		options.state_dir = ملاؤ_dir();
	}

	اگر (options.type != REBASE_UNSPECIFIED)
		اندر_پیش_رفت = 1;

	کل_argc = argc;
	argc = parse_options(argc, argv, سابقہ,
			     builtin_rebase_options,
			     builtin_rebase_usage, 0);

	اگر (options.trailer_دلائل.nr) {
		اگر (validate_trailer_دلائل(&options.trailer_دلائل))
			die(تہی);
		options.flags |= REBASE_FORCE;
	}

	اگر (preserve_merges_selected)
		die(_("--preserve-merges was replaced by --rebase-merges\n"
			"Note: Your `pull.rebase` configuration may also be set to 'preserve',\n"
			"which is no longer supported; use 'merges' instead"));

	اگر (options.action != ACTION_NONE && کل_argc != 2) {
		usage_with_options(builtin_rebase_usage,
				   builtin_rebase_options);
	}

	اگر (argc > 2)
		usage_with_options(builtin_rebase_usage,
				   builtin_rebase_options);

	اگر (keep_بنیاد) {
		اگر (options.onto_name)
			die(_("options '%s' and '%s' cannot be used together"), "--keep-base", "--onto");
		اگر (options.جڑ)
			die(_("options '%s' and '%s' cannot be used together"), "--keep-base", "--root");
		/*
		 * --keep-base defaults to --no-fork-point to keep the
		 * base the same.
		 */
		اگر (options.fork_نقطہ < 0)
			options.fork_نقطہ = 0;
	}
	اگر (options.جڑ && options.fork_نقطہ > 0)
		die(_("options '%s' and '%s' cannot be used together"), "--root", "--fork-point");

	اگر (options.action != ACTION_NONE && !اندر_پیش_رفت)
		die(_("no rebase in progress"));

	اگر (options.action == ACTION_EDIT_TODO && !is_ملاؤ(&options))
		die(_("The --edit-todo action can only be used during "
		      "interactive rebase."));

	اگر (trace2_is_enabled()) {
		اگر (is_ملاؤ(&options))
			trace2_cmd_منوال("interactive");
		ورنہ اگر (options.exec.nr)
			trace2_cmd_منوال("interactive-exec");
		ورنہ
			trace2_cmd_منوال(action_نام[options.action]);
	}

	options.reflog_action = getenv(GIT_REFLOG_ACTION_ENVIRONMENT);
	options.reflog_action =
		xstrdup(options.reflog_action ? options.reflog_action : "rebase");

	انتخاب (options.action) {
	حالت ACTION_CONTINUE: {
		ساخت object_id سر;
		ساخت تالا_file تالا_file = تالا_INIT;
		صحیح fd;

		/* Sanity check */
		اگر (repo_get_oid(the_repository, "HEAD", &سر))
			die(_("Cannot read HEAD"));

		fd = repo_hold_locked_اشاریہ(the_repository, &تالا_file, 0);
		اگر (repo_read_اشاریہ(the_repository) < 0)
			die(_("could not read index"));
		refresh_اشاریہ(the_repository->اشاریہ, REFRESH_QUIET, تہی, تہی,
			      تہی);
		اگر (0 <= fd)
			repo_update_اشاریہ_if_able(the_repository, &تالا_file);
		rollback_تالا_file(&تالا_file);

		اگر (has_unstaged_changes(the_repository, 1)) {
			puts(_("You must edit all merge conflicts and then\n"
			       "mark them as resolved using git add"));
			باہر_نکلو(1);
		}
		اگر (read_basic_state(&options))
			باہر_نکلو(1);
		جاؤ تسلسل_rebase;
	}
	حالت ACTION_SKIP: {
		ساخت string_list ملاؤ_rr = STRING_LIST_INIT_DUP;

		rerere_clear(the_repository, &ملاؤ_rr);
		string_list_clear(&ملاؤ_rr, 1);
		ropts.flags = RESET_WORKING_درخت_HARD |
			      RESET_WORKING_درخت_UPDATE_سر;
		اگر (reset_working_درخت(the_repository, &ropts) < 0)
			die(_("could not discard worktree changes"));
		remove_branch_state(the_repository, 0);
		اگر (read_basic_state(&options))
			باہر_نکلو(1);
		جاؤ تسلسل_rebase;
	}
	حالت ACTION_ABORT: {
		ساخت string_list ملاؤ_rr = STRING_LIST_INIT_DUP;
		ساخت strbuf سر_پیغام = STRBUF_INIT;

		rerere_clear(the_repository, &ملاؤ_rr);
		string_list_clear(&ملاؤ_rr, 1);

		اگر (read_basic_state(&options))
			باہر_نکلو(1);

		strbuf_addf(&سر_پیغام, "%s (abort): returning to %s",
			    options.reflog_action,
			    options.سر_name ? options.سر_name
					      : oid_مقصد_hex(&options.orig_سر->object.oid));
		ropts.oid = &options.orig_سر->object.oid;
		ropts.سر_پیغام = سر_پیغام.buf;
		ropts.branch = options.سر_name;
		ropts.flags = RESET_WORKING_درخت_HARD |
			      RESET_WORKING_درخت_UPDATE_سر;
		اگر (reset_working_درخت(the_repository, &ropts) < 0)
			die(_("could not move back to %s"),
			    oid_مقصد_hex(&options.orig_سر->object.oid));
		strbuf_release(&سر_پیغام);
		remove_branch_state(the_repository, 0);
		ret = اختتام_rebase(&options);
		جاؤ cleanup;
	}
	حالت ACTION_QUIT: {
		save_autostash(state_dir_راستہ("autostash", &options));
		اگر (options.type == REBASE_ملاؤ) {
			ساخت replay_opts replay = REPLAY_OPTS_INIT;

			replay.action = REPLAY_INTERACTIVE_REBASE;
			ret = sequencer_remove_state(&replay);
			replay_opts_release(&replay);
		} ورنہ {
			strbuf_reset(&buf);
			strbuf_addstr(&buf, options.state_dir);
			ret = remove_dir_recursively(&buf, 0);
			اگر (ret)
				error(_("could not remove '%s'"),
				       options.state_dir);
		}
		جاؤ cleanup;
	}
	حالت ACTION_EDIT_TODO:
		options.dont_اختتام_rebase = 1;
		جاؤ تسلسل_rebase;
	حالت ACTION_SHOW_CURRENT_PATCH:
		options.dont_اختتام_rebase = 1;
		جاؤ تسلسل_rebase;
	حالت ACTION_NONE:
		توڑو;
	طےشدہ:
		BUG("action: %d", options.action);
	}

	/* Make sure no rebase is in progress */
	اگر (اندر_پیش_رفت) {
		ثابت حرف *آخری_slash = strrchr(options.state_dir, '/');
		ثابت حرف *state_dir_بنیاد =
			آخری_slash ? آخری_slash + 1 : options.state_dir;
		ثابت حرف *cmd_زندہ_rebase =
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
		    state_dir_بنیاد, cmd_زندہ_rebase, buf.buf);
	}

	اگر ((options.flags & REBASE_INTERACTIVE_EXPLICIT) ||
	    (options.action != ACTION_NONE) ||
	    (options.exec.nr > 0) ||
	    options.autosquash == 1) {
		allow_preemptive_ff = 0;
	}
	اگر (options.committer_date_is_author_date || options.ignore_date)
		options.flags |= REBASE_FORCE;

	برائے (i = 0; i < options.git_am_opts.nr; i++) {
		ثابت حرف *option = options.git_am_opts.v[i], *p;
		اگر (!strcmp(option, "--whitespace=fix") ||
		    !strcmp(option, "--whitespace=strip"))
			allow_preemptive_ff = 0;
		ورنہ اگر (skip_سابقہ(option, "-C", &p)) {
			جب (*p)
				اگر (!ہندسہ_ہے(*(p++)))
					die(_("switch `C' expects a "
					      "numerical value"));
		} ورنہ اگر (skip_سابقہ(option, "--whitespace=", &p)) {
			اگر (*p && strcmp(p, "warn") && strcmp(p, "nowarn") &&
			    strcmp(p, "error") && strcmp(p, "error-all"))
				die("Invalid whitespace option: '%s'", p);
		}
	}

	برائے (i = 0; i < options.exec.nr; i++)
		اگر (check_exec_cmd(options.exec.اشیاء[i].string))
			باہر_نکلو(1);

	اگر (!(options.flags & REBASE_NO_QUIET))
		strvec_ڈالو(&options.git_am_opts, "-q");

	اگر (options.خالی_خانے != خالی_خانے_UNSPECIFIED)
		imply_ملاؤ(&options, "--empty");

	اگر (options.reapply_cherry_picks < 0)
		/*
		 * We default to --no-reapply-cherry-picks unless
		 * --keep-base is given; when --keep-base is given, we want
		 * to default to --reapply-cherry-picks.
		 */
		options.reapply_cherry_picks = keep_بنیاد;
	ورنہ اگر (!keep_بنیاد)
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
		imply_ملاؤ(&options, options.reapply_cherry_picks ?
					  "--reapply-cherry-picks" :
					  "--no-reapply-cherry-picks");

	اگر (gpg_sign)
		options.gpg_sign_opt = xstrfmt("-S%s", gpg_sign);

	اگر (options.exec.nr)
		imply_ملاؤ(&options, "--exec");

	اگر (options.type == REBASE_APPLY) {
		اگر (ignore_whitespace)
			strvec_ڈالو(&options.git_am_opts,
				    "--ignore-whitespace");
		اگر (options.committer_date_is_author_date)
			strvec_ڈالو(&options.git_am_opts,
				    "--committer-date-is-author-date");
		اگر (options.ignore_date)
			strvec_ڈالو(&options.git_am_opts, "--ignore-date");
	} ورنہ {
		/* REBASE_MERGE */
		اگر (ignore_whitespace) {
			string_list_append(&options.strategy_opts,
					   "ignore-space-change");
		}
	}

	اگر (strategy_opt)
		options.strategy = xstrdup(strategy_opt);
	ورنہ اگر (options.strategy_opts.nr && !options.strategy)
		options.strategy = xstrdup("ort");
	اگر (options.strategy)
		imply_ملاؤ(&options, "--strategy");

	اگر (options.جڑ && !options.onto_name)
		imply_ملاؤ(&options, "--root without --onto");

	اگر (options.trailer_دلائل.nr)
		imply_ملاؤ(&options, "--trailer");

	اگر (isatty(2) && options.flags & REBASE_NO_QUIET)
		strbuf_addstr(&options.git_format_patch_opt, " --progress");

	اگر (options.git_am_opts.nr || options.type == REBASE_APPLY) {
		/* all am options except -q are compatible only with --apply */
		برائے (i = options.git_am_opts.nr - 1; i >= 0; i--)
			اگر (strcmp(options.git_am_opts.v[i], "-q"))
				توڑو;

		اگر (i >= 0 || options.type == REBASE_APPLY) {
			اگر (is_ملاؤ(&options))
				die(_("apply options and merge options "
					  "cannot be used together"));
			ورنہ اگر (options.rebase_merges == -1 && options.config_rebase_merges == 1)
				die(_("apply options are incompatible with rebase.rebaseMerges.  Consider adding --no-rebase-merges"));
			ورنہ اگر (options.update_refs == -1 && options.config_update_refs == 1)
				die(_("apply options are incompatible with rebase.updateRefs.  Consider adding --no-update-refs"));
			ورنہ
				options.type = REBASE_APPLY;
		}
	}

	اگر (options.update_refs == 1)
		imply_ملاؤ(&options, "--update-refs");
	options.update_refs = (options.update_refs >= 0) ? options.update_refs :
			     ((options.config_update_refs >= 0) ? options.config_update_refs : 0);

	اگر (options.rebase_merges == 1)
		imply_ملاؤ(&options, "--rebase-merges");
	options.rebase_merges = (options.rebase_merges >= 0) ? options.rebase_merges :
				((options.config_rebase_merges >= 0) ? options.config_rebase_merges : 0);

	اگر (options.autosquash == 1) {
		imply_ملاؤ(&options, "--autosquash");
	} ورنہ اگر (options.autosquash == -1) {
		options.autosquash =
			options.config_autosquash &&
			(options.flags & REBASE_INTERACTIVE_EXPLICIT);
	}

	اگر (options.type == REBASE_UNSPECIFIED) {
		اگر (!strcmp(options.default_backend, "merge"))
			options.type = REBASE_ملاؤ;
		ورنہ اگر (!strcmp(options.default_backend, "apply"))
			options.type = REBASE_APPLY;
		ورنہ
			die(_("Unknown rebase backend: %s"),
			    options.default_backend);
	}

	انتخاب (options.type) {
	حالت REBASE_ملاؤ:
		options.state_dir = ملاؤ_dir();
		توڑو;
	حالت REBASE_APPLY:
		options.state_dir = apply_dir();
		توڑو;
	طےشدہ:
		BUG("options.type was just set above; should be unreachable.");
	}

	اگر (options.خالی_خانے == خالی_خانے_UNSPECIFIED) {
		اگر (options.flags & REBASE_INTERACTIVE_EXPLICIT)
			options.خالی_خانے = خالی_خانے_STOP;
		ورنہ اگر (options.exec.nr > 0)
			options.خالی_خانے = خالی_خانے_KEEP;
		ورنہ
			options.خالی_خانے = خالی_خانے_DROP;
	}
	اگر (reschedule_failed_exec > 0 && !is_ملاؤ(&options))
		die(_("--reschedule-failed-exec requires "
		      "--exec or --interactive"));
	اگر (reschedule_failed_exec >= 0)
		options.reschedule_failed_exec = reschedule_failed_exec;

	اگر (options.signoff) {
		strvec_ڈالو(&options.git_am_opts, "--signoff");
		options.flags |= REBASE_FORCE;
	}

	اگر (!options.جڑ) {
		اگر (argc < 1) {
			ساخت branch *branch;

			branch = branch_get(تہی);
			options.upstream_name = branch_get_upstream(branch,
								    تہی);
			اگر (!options.upstream_name)
				error_on_missing_default_upstream();
			اگر (options.fork_نقطہ < 0)
				options.fork_نقطہ = 1;
		} ورنہ {
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
	} ورنہ {
		اگر (!options.onto_name) {
			اگر (commit_درخت("", 0, the_hash_algo->خالی_خانے_درخت, تہی,
					&squash_onto, تہی, تہی) < 0)
				die(_("Could not create new root commit"));
			options.squash_onto = &squash_onto;
			options.onto_name = squash_onto_name =
				xstrdup(oid_مقصد_hex(&squash_onto));
		} ورنہ
			options.جڑ_with_onto = 1;

		options.upstream_name = تہی;
		options.upstream = تہی;
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
		ساخت object_id branch_oid;
		branch_name = argv[0];
		options.switch_مقصد = argv[0];

		/* Is it a local branch? */
		strbuf_reset(&buf);
		strbuf_addf(&buf, "refs/heads/%s", branch_name);
		اگر (!refs_read_ref(get_main_ref_store(the_repository), buf.buf, &branch_oid)) {
			die_if_checked_برآمد(buf.buf, 1);
			options.سر_name = xstrdup(buf.buf);
			options.orig_سر =
				lookup_commit_object(the_repository,
						     &branch_oid);
		/* If not is it a valid ref (branch or commit)? */
		} ورنہ {
			options.orig_سر =
				lookup_commit_reference_by_name(branch_name);
			options.سر_name = تہی;
		}
		اگر (!options.orig_سر)
			die(_("no such branch/commit '%s'"), branch_name);
	} ورنہ اگر (argc == 0) {
		/* Do not need to switch branches, we are already on it. */
		options.سر_name =
			xstrdup_or_null(refs_resolve_ref_unsafe(get_main_ref_store(the_repository), "HEAD", 0, تہی,
								&flags));
		اگر (!options.سر_name)
			die(_("No such ref: %s"), "HEAD");
		اگر (flags & REF_ISSYMREF) {
			اگر (!skip_سابقہ(options.سر_name,
					 "refs/heads/", &branch_name))
				branch_name = options.سر_name;

		} ورنہ {
			FREE_AND_NULL(options.سر_name);
			branch_name = "HEAD";
		}
		options.orig_سر = lookup_commit_reference_by_name("HEAD");
		اگر (!options.orig_سر)
			die(_("Could not resolve HEAD to a commit"));
	} ورنہ
		BUG("unexpected number of arguments left to parse");

	/* Make sure the branch to rebase onto is valid. */
	اگر (keep_بنیاد) {
		strbuf_reset(&buf);
		strbuf_addstr(&buf, options.upstream_name);
		strbuf_addstr(&buf, "...");
		strbuf_addstr(&buf, branch_name);
		options.onto_name = keep_بنیاد_onto_name = xstrdup(buf.buf);
	} ورنہ اگر (!options.onto_name)
		options.onto_name = options.upstream_name;
	اگر (لڑی_تلاش(options.onto_name, "...")) {
		اگر (repo_get_oid_mb(the_repository, options.onto_name, &branch_بنیاد) < 0) {
			اگر (keep_بنیاد)
				die(_("'%s': need exactly one merge base with branch"),
				    options.upstream_name);
			ورنہ
				die(_("'%s': need exactly one merge base"),
				    options.onto_name);
		}
		options.onto = lookup_commit_or_die(&branch_بنیاد,
						    options.onto_name);
	} ورنہ {
		options.onto =
			lookup_commit_reference_by_name(options.onto_name);
		اگر (!options.onto)
			die(_("Does not point to a valid commit '%s'"),
				options.onto_name);
		fill_branch_بنیاد(&options, &branch_بنیاد);
	}

	اگر (keep_بنیاد && options.reapply_cherry_picks)
		options.upstream = options.onto;

	اگر (options.fork_نقطہ > 0)
		options.restrict_revision =
			get_fork_نقطہ(options.upstream_name, options.orig_سر);

	اگر (repo_read_اشاریہ(the_repository) < 0)
		die(_("could not read index"));

	اگر (options.autostash)
		create_autostash(the_repository,
				 state_dir_راستہ("autostash", &options));


	اگر (require_clean_work_درخت(the_repository, "rebase",
				    _("Please commit or stash them."), 1, 1)) {
		ret = -1;
		جاؤ cleanup_autostash;
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
			     options.orig_سر, &branch_بنیاد)) {
		صحیح flag;

		اگر (!(options.flags & REBASE_FORCE)) {
			/* Lazily switch to the target branch if needed... */
			اگر (options.switch_مقصد) {
				ret = checkout_اوپر_مقصد_date(&options);
				اگر (ret)
					جاؤ cleanup_autostash;
			}

			اگر (!(options.flags & REBASE_NO_QUIET))
				; /* be quiet */
			ورنہ اگر (!strcmp(branch_name, "HEAD") &&
				 refs_resolve_ref_unsafe(get_main_ref_store(the_repository), "HEAD", 0, تہی, &flag))
				puts(_("HEAD is up to date."));
			ورنہ
				لکھو(_("Current branch %s is up to date.\n"),
				       branch_name);
			ret = اختتام_rebase(&options);
			جاؤ cleanup;
		} ورنہ اگر (!(options.flags & REBASE_NO_QUIET))
			; /* be quiet */
		ورنہ اگر (!strcmp(branch_name, "HEAD") &&
			 refs_resolve_ref_unsafe(get_main_ref_store(the_repository), "HEAD", 0, تہی, &flag))
			puts(_("HEAD is up to date, rebase forced."));
		ورنہ
			لکھو(_("Current branch %s is up to date, rebase "
				 "forced.\n"), branch_name);
	}

	/* If a hook exists, give it a chance to interrupt*/
	اگر (!ok_مقصد_skip_pre_rebase &&
	    تسلسل_hooks_l(the_repository, "pre-rebase", options.upstream_arg,
			argc ? argv[0] : تہی, تہی)) {
		ret = error(_("The pre-rebase hook refused to rebase."));
		جاؤ cleanup_autostash;
	}

	اگر (options.flags & REBASE_DIFFSTAT) {
		ساخت diff_options opts;

		اگر (options.flags & REBASE_VERBOSE) {
			اگر (is_null_oid(&branch_بنیاد))
				لکھو(_("Changes to %s:\n"),
				       oid_مقصد_hex(&options.onto->object.oid));
			ورنہ
				لکھو(_("Changes from %s to %s:\n"),
				       oid_مقصد_hex(&branch_بنیاد),
				       oid_مقصد_hex(&options.onto->object.oid));
		}

		/* We want color (if set), but no pager */
		repo_diff_تیاری(the_repository, &opts);
		init_diffstat_widths(&opts);
		opts.output_format |=
			DIFF_FORMAT_SUMMARY | DIFF_FORMAT_DIFFSTAT;
		opts.detect_rename = DIFF_DETECT_RENAME;
		diff_تیاری_مکمل(&opts);
		diff_درخت_oid(is_null_oid(&branch_بنیاد) ?
			      the_hash_algo->خالی_خانے_درخت : &branch_بنیاد,
			      &options.onto->object.oid, "", &opts);
		diffcore_std(&opts);
		diff_flush(&opts);
	}

	اگر (is_ملاؤ(&options))
		جاؤ تسلسل_rebase;

	/* Detach HEAD and reset the tree */
	اگر (options.flags & REBASE_NO_QUIET)
		لکھو(_("First, rewinding head to replay your work on top of "
			 "it...\n"));

	strbuf_addf(&پیغام, "%s (start): checkout %s",
		    options.reflog_action, options.onto_name);
	ropts.oid = &options.onto->object.oid;
	ropts.orig_سر = &options.orig_سر->object.oid;
	ropts.flags = RESET_WORKING_درخت_DETACH |
		      RESET_WORKING_درخت_UPDATE_سر |
		      RESET_WORKING_درخت_UPDATE_ORIG_سر |
		      RESET_WORKING_درخت_تسلسل_POST_CHECKOUT_HOOK;
	ropts.سر_پیغام = پیغام.buf;
	ropts.default_reflog_action = options.reflog_action;
	اگر (reset_working_درخت(the_repository, &ropts)) {
		ret = error(_("Could not detach HEAD"));
		جاؤ cleanup_autostash;
	}

	/*
	 * If the onto is a proper descendant of the tip of the branch, then
	 * we just fast-forwarded.
	 */
	اگر (oideq(&branch_بنیاد, &options.orig_سر->object.oid)) {
		لکھو(_("Fast-forwarded %s to %s.\n"),
			branch_name, options.onto_name);
		move_مقصد_original_branch(&options);
		ret = اختتام_rebase(&options);
		جاؤ cleanup;
	}

	strbuf_addf(&revisions, "%s..%s",
		    options.جڑ ? oid_مقصد_hex(&options.onto->object.oid) :
		    (options.restrict_revision ?
		     oid_مقصد_hex(&options.restrict_revision->object.oid) :
		     oid_مقصد_hex(&options.upstream->object.oid)),
		    oid_مقصد_hex(&options.orig_سر->object.oid));

	options.revisions = revisions.buf;

تسلسل_rebase:
	ret = تسلسل_specific_rebase(&options);

cleanup:
	strbuf_release(&buf);
	strbuf_release(&پیغام);
	strbuf_release(&revisions);
	rebase_options_release(&options);
	آزاد(squash_onto_name);
	آزاد(keep_بنیاد_onto_name);
	واپس !!ret;

cleanup_autostash:
	ret |= !!cleanup_autostash(&options);
	جاؤ cleanup;
}
