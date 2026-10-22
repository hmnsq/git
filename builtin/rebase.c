/*
 * "git rebase" builtin command
 *
 * Copyright (c) 2018 Pratik Karki
 */

#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#تعریف_کرو DISABLE_SIGN_COMPARE_WARNINGS

#ضمن "builtin.h"

#ضمن "abspath.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "hex.h"
#ضمن "run-command.h"
#ضمن "strvec.h"
#ضمن "dir.h"
#ضمن "refs.h"
#ضمن "config.h"
#ضمن "unpack-trees.h"
#ضمن "lockfile.h"
#ضمن "object-file.h"
#ضمن "object-name.h"
#ضمن "parse-options.h"
#ضمن "path.h"
#ضمن "commit.h"
#ضمن "diff.h"
#ضمن "wt-status.h"
#ضمن "revision.h"
#ضمن "commit-reach.h"
#ضمن "rerere.h"
#ضمن "branch.h"
#ضمن "sequencer.h"
#ضمن "rebase-interactive.h"
#ضمن "reset.h"
#ضمن "trace2.h"
#ضمن "hook.h"
#ضمن "trailer.h"

ساكن حرف ثابت * ثابت builtin_rebase_usage[] = {
	N_("git rebase [-i] [options] [--exec <cmd>] "
		"[--onto <newbase> | --keep-base] [<upstream> [<branch>]]"),
	N_("git rebase [-i] [options] [--exec <cmd>] [--onto <newbase>] "
		"--root [<branch>]"),
	"git rebase --continue | --abort | --skip | --edit-todo",
	عدم
};

ساكن GIT_رستہ_FUNC(رستہ_squash_onto, "rebase-merge/squash-onto")
ساكن GIT_رستہ_FUNC(رستہ_interactive, "rebase-merge/interactive")
ساكن GIT_رستہ_FUNC(apply_dir, "rebase-apply")
ساكن GIT_رستہ_FUNC(ملاؤ_dir, "rebase-merge")

تعداد rebase_type {
	REBASE_UNSPECIFIED = -1,
	REBASE_APPLY,
	REBASE_ملاؤ
};

تعداد خالی_تھاواں_type {
	خالی_تھاواں_UNSPECIFIED = -1,
	خالی_تھاواں_DROP,
	خالی_تھاواں_KEEP,
	خالی_تھاواں_STOP
};

تعداد action {
	ACTION_NONE = 0,
	ACTION_CONTINUE,
	ACTION_SKIP,
	ACTION_ABORT,
	ACTION_QUIT,
	ACTION_EDIT_TODO,
	ACTION_SHOW_CURRENT_PATCH
};

ساكن ثابت حرف *action_ناں[] = {
	"undefined",
	"continue",
	"skip",
	"abort",
	"quit",
	"edit_todo",
	"show_current_patch"
};

بنية rebase_options {
	تعداد rebase_type type;
	تعداد خالی_تھاواں_type خالی_تھاواں;
	حرف *default_backend;
	ثابت حرف *state_dir;
	بنية commit *upstream;
	ثابت حرف *upstream_name;
	ثابت حرف *upstream_arg;
	حرف *سر_name;
	بنية commit *orig_سر;
	بنية commit *onto;
	ثابت حرف *onto_name;
	ثابت حرف *revisions;
	ثابت حرف *switch_منزل;
	صحيح جڑ, جڑ_with_onto;
	بنية object_id *squash_onto;
	بنية commit *restrict_revision;
	صحيح dont_انت_rebase;
	تعداد {
		REBASE_NO_QUIET = 1<<0,
		REBASE_VERBOSE = 1<<1,
		REBASE_DIFFSTAT = 1<<2,
		REBASE_FORCE = 1<<3,
		REBASE_INTERACTIVE_EXPLICIT = 1<<4,
	} flags;
	بنية strvec git_am_opts;
	تعداد action action;
	حرف *reflog_action;
	صحيح signoff;
	بنية strvec trailer_دلیلاں;
	صحيح allow_rerere_autoupdate;
	صحيح keep_خالی_تھاواں;
	صحيح autosquash;
	حرف *gpg_sign_opt;
	صحيح autostash;
	صحيح committer_date_is_author_date;
	صحيح ignore_date;
	بنية string_list exec;
	صحيح allow_خالی_تھاواں_سنیہا;
	صحيح rebase_merges, rebase_cousins;
	حرف *strategy;
	بنية string_list strategy_opts;
	بنية strbuf git_format_patch_opt;
	صحيح reschedule_failed_exec;
	صحيح reapply_cherry_picks;
	صحيح fork_نقطہ;
	صحيح update_refs;
	صحيح config_autosquash;
	صحيح config_rebase_merges;
	صحيح config_update_refs;
};

#تعریف_کرو REBASE_OPTIONS_INIT {			  	\
		.type = REBASE_UNSPECIFIED,	  	\
		.خالی_تھاواں = خالی_تھاواں_UNSPECIFIED,	  	\
		.keep_خالی_تھاواں = 1,			\
		.default_backend = xstrdup("merge"),  	\
		.flags = REBASE_NO_QUIET, 		\
		.git_am_opts = STRVEC_INIT,		\
		.exec = STRING_LIST_INIT_NODUP,		\
		.trailer_دلیلاں = STRVEC_INIT,		\
		.git_format_patch_opt = STRBUF_INIT,	\
		.fork_نقطہ = -1,			\
		.reapply_cherry_picks = -1,             \
		.allow_خالی_تھاواں_سنیہا = 1,               \
		.autosquash = -1,                       \
		.rebase_merges = -1,                    \
		.config_rebase_merges = -1,             \
		.update_refs = -1,                      \
		.config_update_refs = -1,               \
		.strategy_opts = STRING_LIST_INIT_NODUP,\
	}

ساكن فراغ rebase_options_release(بنية rebase_options *opts)
{
	حرر(opts->default_backend);
	حرر(opts->reflog_action);
	حرر(opts->سر_name);
	strvec_clear(&opts->git_am_opts);
	حرر(opts->gpg_sign_opt);
	string_list_clear(&opts->exec, 0);
	حرر(opts->strategy);
	string_list_clear(&opts->strategy_opts, 0);
	strbuf_release(&opts->git_format_patch_opt);
	strvec_clear(&opts->trailer_دلیلاں);
}

ساكن بنية replay_opts get_replay_opts(ثابت بنية rebase_options *opts)
{
	بنية replay_opts replay = REPLAY_OPTS_INIT;

	replay.action = REPLAY_INTERACTIVE_REBASE;
	replay.strategy = عدم;
	sequencer_init_config(&replay);

	replay.signoff = opts->signoff;

	strvec_pushv(&replay.trailer_دلیلاں, opts->trailer_دلیلاں.v);

	replay.allow_ff = !(opts->flags & REBASE_FORCE);
	إذا (opts->allow_rerere_autoupdate)
		replay.allow_rerere_auto = opts->allow_rerere_autoupdate;
	replay.allow_خالی_تھاواں = 1;
	replay.allow_خالی_تھاواں_سنیہا = opts->allow_خالی_تھاواں_سنیہا;
	replay.drop_redundant_commits = (opts->خالی_تھاواں == خالی_تھاواں_DROP);
	replay.keep_redundant_commits = (opts->خالی_تھاواں == خالی_تھاواں_KEEP);
	replay.quiet = !(opts->flags & REBASE_NO_QUIET);
	replay.verbose = opts->flags & REBASE_VERBOSE;
	replay.reschedule_failed_exec = opts->reschedule_failed_exec;
	replay.committer_date_is_author_date =
					opts->committer_date_is_author_date;
	replay.ignore_date = opts->ignore_date;
	حرر(replay.gpg_sign);
	replay.gpg_sign = xstrdup_or_null(opts->gpg_sign_opt);
	replay.reflog_action = xstrdup(opts->reflog_action);
	إذا (opts->strategy)
		replay.strategy = xstrdup_or_null(opts->strategy);
	وإلا إذا (!replay.strategy && replay.default_strategy) {
		replay.strategy = replay.default_strategy;
		replay.default_strategy = عدم;
	}

	لكل (ناپ_t i = 0; i < opts->strategy_opts.nr; i++)
		strvec_ڈالو(&replay.xopts, opts->strategy_opts.اشیاء[i].string);

	إذا (opts->squash_onto) {
		oidcpy(&replay.squash_onto, opts->squash_onto);
		replay.have_squash_onto = 1;
	}

	أرجع replay;
}

ساكن صحيح edit_todo_file(بنانشانی flags, بنية replay_opts *opts)
{
	ثابت حرف *todo_file = rebase_رستہ_todo();
	بنية todo_list todo_list = TODO_LIST_INIT,
		new_todo = TODO_LIST_INIT;
	صحيح res = 0;

	إذا (strbuf_read_file(&todo_list.buf, todo_file, 0) < 0)
		أرجع error_errno(_("could not read '%s'."), todo_file);

	strbuf_stripspace(&todo_list.buf, comment_سطر_str);
	res = edit_todo_list(the_repository, opts, &todo_list, &new_todo,
			     عدم, عدم, flags);
	إذا (!res && todo_list_write_منزل_file(the_repository, &new_todo, todo_file,
					    عدم, عدم, -1, flags & ~(TODO_LIST_SHORTEN_IDS)))
		res = error_errno(_("could not write '%s'"), todo_file);

	todo_list_release(&todo_list);
	todo_list_release(&new_todo);

	أرجع res;
}

ساكن صحيح get_revision_ranges(بنية commit *upstream, بنية commit *onto,
			       بنية object_id *orig_سر, حرف **revisions,
			       حرف **shortrevisions)
{
	بنية commit *ادھار_rev = upstream ? upstream : onto;
	ثابت حرف *shorthead;

	*revisions = xstrfmt("%s...%s", oid_منزل_hex(&ادھار_rev->object.oid),
			     oid_منزل_hex(orig_سر));

	shorthead = repo_لبھو_unique_abbrev(the_repository, orig_سر,
					    DEFAULT_ABBREV);

	إذا (upstream) {
		ثابت حرف *shortrev;

		shortrev = repo_لبھو_unique_abbrev(the_repository,
						   &ادھار_rev->object.oid,
						   DEFAULT_ABBREV);

		*shortrevisions = xstrfmt("%s..%s", shortrev, shorthead);
	} وإلا
		*shortrevisions = xstrdup(shorthead);

	أرجع 0;
}

ساكن صحيح init_basic_state(بنية replay_opts *opts, ثابت حرف *سر_name,
			    بنية commit *onto,
			    ثابت بنية object_id *orig_سر)
{
	FILE *interactive;

	إذا (!is_directory(ملاؤ_dir()) &&
	    محفوظ_create_dir_وچ_gitdir(the_repository, ملاؤ_dir()))
		أرجع error_errno(_("could not create temporary %s"), ملاؤ_dir());

	refs_delete_reflog(get_main_ref_store(the_repository), "REBASE_HEAD");

	interactive = fopen(رستہ_interactive(), "w");
	إذا (!interactive)
		أرجع error_errno(_("could not mark as interactive"));
	fclose(interactive);

	أرجع write_basic_state(opts, سر_name, onto, orig_سر);
}

ساكن صحيح do_interactive_rebase(بنية rebase_options *opts, بنانشانی flags)
{
	صحيح ret = -1;
	حرف *revisions = عدم, *shortrevisions = عدم;
	بنية strvec make_script_دلیلاں = STRVEC_INIT;
	بنية todo_list todo_list = TODO_LIST_INIT;
	بنية replay_opts replay = get_replay_opts(opts);

	إذا (get_revision_ranges(opts->upstream, opts->onto, &opts->orig_سر->object.oid,
				&revisions, &shortrevisions))
		اذهب cleanup;

	strvec_pushl(&make_script_دلیلاں, "", revisions, عدم);
	إذا (opts->restrict_revision)
		strvec_pushf(&make_script_دلیلاں, "^%s",
			     oid_منزل_hex(&opts->restrict_revision->object.oid));

	ret = sequencer_make_script(the_repository, &todo_list.buf,
				    &make_script_دلیلاں, flags);
	إذا (ret) {
		error(_("could not generate todo list"));
		اذهب cleanup;
	}

	إذا (init_basic_state(&replay,
			     opts->سر_name ? opts->سر_name : "detached HEAD",
			     opts->onto, &opts->orig_سر->object.oid))
		اذهب cleanup;

	إذا (!opts->upstream && opts->squash_onto)
		write_file(رستہ_squash_onto(), "%s\n",
			   oid_منزل_hex(opts->squash_onto));

	discard_اشاریہ(the_repository->اشاریہ);
	إذا (todo_list_parse_insn_بفر(the_repository, &replay,
					todo_list.buf.buf, &todo_list))
		BUG("unusable todo list");

	ret = complete_action(the_repository, &replay, flags,
		shortrevisions, opts->onto_name, opts->onto,
		&opts->orig_سر->object.oid, &opts->exec,
		opts->autosquash, opts->update_refs, &todo_list);

cleanup:
	replay_opts_release(&replay);
	حرر(revisions);
	حرر(shortrevisions);
	todo_list_release(&todo_list);
	strvec_clear(&make_script_دلیلاں);

	أرجع ret;
}

ساكن صحيح لگاتار_sequencer_rebase(بنية rebase_options *opts)
{
	بنانشانی flags = 0;
	صحيح abbreviate_commands = 0, ret = 0;

	repo_config_get_bool(the_repository, "rebase.abbreviatecommands", &abbreviate_commands);

	flags |= opts->keep_خالی_تھاواں ? TODO_LIST_KEEP_خالی_تھاواں : 0;
	flags |= abbreviate_commands ? TODO_LIST_ABBREVIATE_CMDS : 0;
	flags |= opts->rebase_merges ? TODO_LIST_REBASE_MERGES : 0;
	flags |= opts->rebase_cousins > 0 ? TODO_LIST_REBASE_COUSINS : 0;
	flags |= opts->جڑ_with_onto ? TODO_LIST_جڑ_WITH_ONTO : 0;
	flags |= opts->reapply_cherry_picks ? TODO_LIST_REAPPLY_CHERRY_PICKS : 0;
	flags |= opts->flags & REBASE_NO_QUIET ? TODO_LIST_WARN_SKIPPED_CHERRY_PICKS : 0;

	بدل (opts->action) {
	حالة ACTION_NONE: {
		إذا (!opts->onto && !opts->upstream)
			die(_("a base commit must be provided with --upstream or --onto"));

		ret = do_interactive_rebase(opts, flags);
		اكسر;
	}
	حالة ACTION_SKIP: {
		بنية string_list ملاؤ_rr = STRING_LIST_INIT_DUP;

		rerere_clear(the_repository, &ملاؤ_rr);
	}
		/* fallthrough */
	حالة ACTION_CONTINUE: {
		بنية replay_opts replay_opts = get_replay_opts(opts);

		ret = sequencer_continue(the_repository, &replay_opts);
		replay_opts_release(&replay_opts);
		اكسر;
	}
	حالة ACTION_EDIT_TODO: {
		بنية replay_opts replay_opts = get_replay_opts(opts);

		ret = edit_todo_file(flags, &replay_opts);
		replay_opts_release(&replay_opts);
		اكسر;
	}
	حالة ACTION_SHOW_CURRENT_PATCH: {
		بنية بچہ_process cmd = بچہ_PROCESS_INIT;

		cmd.git_cmd = 1;
		strvec_pushl(&cmd.دلیلاں, "show", "REBASE_HEAD", "--", عدم);
		ret = لگاتار_command(&cmd);

		اكسر;
	}
	افتراضي:
		BUG("invalid command '%d'", opts->action);
	}

	أرجع ret;
}

ساكن صحيح is_ملاؤ(بنية rebase_options *opts)
{
	أرجع opts->type == REBASE_ملاؤ;
}

ساكن فراغ imply_ملاؤ(بنية rebase_options *opts, ثابت حرف *option)
{
	بدل (opts->type) {
	حالة REBASE_APPLY:
		die(_("%s requires the merge backend"), option);
		اكسر;
	حالة REBASE_ملاؤ:
		اكسر;
	افتراضي:
		opts->type = REBASE_ملاؤ; /* implied */
		اكسر;
	}
}

/* Returns the filename prefixed by the state_dir */
ساكن ثابت حرف *state_dir_رستہ(ثابت حرف *filename, بنية rebase_options *opts)
{
	ساكن بنية strbuf رستہ = STRBUF_INIT;
	ساكن ناپ_t اگیتر_len;

	إذا (!اگیتر_len) {
		strbuf_addf(&رستہ, "%s/", opts->state_dir);
		اگیتر_len = رستہ.len;
	}

	strbuf_setlen(&رستہ, اگیتر_len);
	strbuf_addstr(&رستہ, filename);
	أرجع رستہ.buf;
}

/* Initialize the rebase options from the state directory. */
ساكن صحيح read_basic_state(بنية rebase_options *opts)
{
	بنية strbuf سر_name = STRBUF_INIT;
	بنية strbuf buf = STRBUF_INIT;
	بنية object_id oid;

	إذا (!read_oneliner(&سر_name, state_dir_رستہ("head-name", opts),
			   READ_ONELINER_WARN_MISSING) ||
	    !read_oneliner(&buf, state_dir_رستہ("onto", opts),
			   READ_ONELINER_WARN_MISSING))
		أرجع -1;
	opts->سر_name = starts_with(سر_name.buf, "refs/") ?
		xstrdup(سر_name.buf) : عدم;
	strbuf_release(&سر_name);
	إذا (get_oid_hex(buf.buf, &oid) ||
	    !(opts->onto = lookup_commit_object(the_repository, &oid)))
		أرجع error(_("invalid onto: '%s'"), buf.buf);

	/*
	 * We always write to orig-head, but interactive rebase used to write to
	 * head. Fall back to reading from head to cover for the case that the
	 * user upgraded git with an ongoing interactive rebase.
	 */
	strbuf_reset(&buf);
	إذا (file_exists(state_dir_رستہ("orig-head", opts))) {
		إذا (!read_oneliner(&buf, state_dir_رستہ("orig-head", opts),
				   READ_ONELINER_WARN_MISSING))
			أرجع -1;
	} وإلا إذا (!read_oneliner(&buf, state_dir_رستہ("head", opts),
				  READ_ONELINER_WARN_MISSING))
		أرجع -1;
	إذا (get_oid_hex(buf.buf, &oid) ||
	    !(opts->orig_سر = lookup_commit_object(the_repository, &oid)))
		أرجع error(_("invalid orig-head: '%s'"), buf.buf);

	إذا (file_exists(state_dir_رستہ("quiet", opts)))
		opts->flags &= ~REBASE_NO_QUIET;
	وإلا
		opts->flags |= REBASE_NO_QUIET;

	إذا (file_exists(state_dir_رستہ("verbose", opts)))
		opts->flags |= REBASE_VERBOSE;

	إذا (file_exists(state_dir_رستہ("signoff", opts))) {
		opts->signoff = 1;
		opts->flags |= REBASE_FORCE;
	}

	إذا (file_exists(state_dir_رستہ("allow_rerere_autoupdate", opts))) {
		strbuf_reset(&buf);
		إذا (!read_oneliner(&buf, state_dir_رستہ("allow_rerere_autoupdate", opts),
				   READ_ONELINER_WARN_MISSING))
			أرجع -1;
		إذا (!strcmp(buf.buf, "--rerere-autoupdate"))
			opts->allow_rerere_autoupdate = RERERE_AUTOUPDATE;
		وإلا إذا (!strcmp(buf.buf, "--no-rerere-autoupdate"))
			opts->allow_rerere_autoupdate = RERERE_NOAUTOUPDATE;
		وإلا
			warning(_("ignoring invalid allow_rerere_autoupdate: "
				  "'%s'"), buf.buf);
	}

	إذا (file_exists(state_dir_رستہ("gpg_sign_opt", opts))) {
		strbuf_reset(&buf);
		إذا (!read_oneliner(&buf, state_dir_رستہ("gpg_sign_opt", opts),
				   READ_ONELINER_WARN_MISSING))
			أرجع -1;
		حرر(opts->gpg_sign_opt);
		opts->gpg_sign_opt = xstrdup(buf.buf);
	}

	strbuf_release(&buf);

	أرجع 0;
}

ساكن صحيح rebase_write_basic_state(بنية rebase_options *opts)
{
	write_file(state_dir_رستہ("head-name", opts), "%s",
		   opts->سر_name ? opts->سر_name : "detached HEAD");
	write_file(state_dir_رستہ("onto", opts), "%s",
		   opts->onto ? oid_منزل_hex(&opts->onto->object.oid) : "");
	write_file(state_dir_رستہ("orig-head", opts), "%s",
		   oid_منزل_hex(&opts->orig_سر->object.oid));
	إذا (!(opts->flags & REBASE_NO_QUIET))
		write_file(state_dir_رستہ("quiet", opts), "%s", "");
	إذا (opts->flags & REBASE_VERBOSE)
		write_file(state_dir_رستہ("verbose", opts), "%s", "");
	إذا (opts->allow_rerere_autoupdate > 0)
		write_file(state_dir_رستہ("allow_rerere_autoupdate", opts),
			   "-%s-rerere-autoupdate",
			   opts->allow_rerere_autoupdate == RERERE_AUTOUPDATE ?
				"" : "-no");
	إذا (opts->gpg_sign_opt)
		write_file(state_dir_رستہ("gpg_sign_opt", opts), "%s",
			   opts->gpg_sign_opt);
	إذا (opts->signoff)
		write_file(state_dir_رستہ("signoff", opts), "--signoff");

	أرجع 0;
}

ساكن صحيح cleanup_autostash(بنية rebase_options *opts)
{
	صحيح ret;
	بنية strbuf dir = STRBUF_INIT;
	ثابت حرف *رستہ = state_dir_رستہ("autostash", opts);

	إذا (!file_exists(رستہ))
		أرجع 0;
	ret = apply_autostash(رستہ);
	strbuf_addstr(&dir, opts->state_dir);
	إذا (remove_dir_recursively(&dir, 0))
		ret = error_errno(_("could not remove '%s'"), opts->state_dir);
	strbuf_release(&dir);

	أرجع ret;
}

ساكن صحيح انت_rebase(بنية rebase_options *opts)
{
	بنية strbuf dir = STRBUF_INIT;
	صحيح ret = 0;

	refs_delete_ref(get_main_ref_store(the_repository), عدم,
			"REBASE_HEAD", عدم, REF_NO_DEREF);
	refs_delete_ref(get_main_ref_store(the_repository), عدم,
			"AUTO_MERGE", عدم, REF_NO_DEREF);
	apply_autostash(state_dir_رستہ("autostash", opts));
	/*
	 * We ignore errors in 'git maintenance run --auto', since the
	 * user should see them.
	 */
	لگاتار_auto_maintenance(the_repository,
			!(opts->flags & (REBASE_NO_QUIET|REBASE_VERBOSE)));

	إذا (opts->type == REBASE_ملاؤ) {
		بنية replay_opts replay = REPLAY_OPTS_INIT;

		replay.action = REPLAY_INTERACTIVE_REBASE;
		ret = sequencer_remove_state(&replay);
		replay_opts_release(&replay);
	} وإلا {
		strbuf_addstr(&dir, opts->state_dir);
		إذا (remove_dir_recursively(&dir, 0))
			ret = error(_("could not remove '%s'"),
				    opts->state_dir);
		strbuf_release(&dir);
	}

	أرجع ret;
}

ساكن صحيح move_منزل_original_branch(بنية rebase_options *opts)
{
	بنية strbuf branch_reflog = STRBUF_INIT, سر_reflog = STRBUF_INIT;
	بنية reset_working_درخت_options ropts = { 0 };
	صحيح ret;

	إذا (!opts->سر_name)
		أرجع 0; /* nothing to move back to */

	إذا (!opts->onto)
		BUG("move_to_original_branch without onto");

	strbuf_addf(&branch_reflog, "%s (finish): %s onto %s",
		    opts->reflog_action,
		    opts->سر_name, oid_منزل_hex(&opts->onto->object.oid));
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
	أرجع ret;
}

ساكن صحيح لگاتار_am(بنية rebase_options *opts)
{
	بنية بچہ_process am = بچہ_PROCESS_INIT;
	بنية بچہ_process format_patch = بچہ_PROCESS_INIT;
	صحيح حالت;
	حرف *rebased_patches;

	am.git_cmd = 1;
	strvec_ڈالو(&am.دلیلاں, "am");
	strvec_pushf(&am.env, GIT_REFLOG_ACTION_ENVIRONMENT "=%s (pick)",
		     opts->reflog_action);
	إذا (opts->action == ACTION_CONTINUE) {
		strvec_ڈالو(&am.دلیلاں, "--resolved");
		strvec_pushf(&am.دلیلاں, "--resolvemsg=%s", rebase_resolvemsg);
		إذا (opts->gpg_sign_opt)
			strvec_ڈالو(&am.دلیلاں, opts->gpg_sign_opt);
		حالت = لگاتار_command(&am);
		إذا (حالت)
			أرجع حالت;

		أرجع move_منزل_original_branch(opts);
	}
	إذا (opts->action == ACTION_SKIP) {
		strvec_ڈالو(&am.دلیلاں, "--skip");
		strvec_pushf(&am.دلیلاں, "--resolvemsg=%s", rebase_resolvemsg);
		حالت = لگاتار_command(&am);
		إذا (حالت)
			أرجع حالت;

		أرجع move_منزل_original_branch(opts);
	}
	إذا (opts->action == ACTION_SHOW_CURRENT_PATCH) {
		strvec_ڈالو(&am.دلیلاں, "--show-current-patch");
		أرجع لگاتار_command(&am);
	}

	rebased_patches = repo_git_رستہ(the_repository, "rebased-patches");
	format_patch.باہرلا = open(rebased_patches,
				O_WRONLY | O_CREAT | O_TRUNC, 0666);
	إذا (format_patch.باہرلا < 0) {
		حالت = error_errno(_("could not open '%s' for writing"),
				     rebased_patches);
		حرر(rebased_patches);
		بچہ_process_clear(&am);
		أرجع حالت;
	}

	format_patch.git_cmd = 1;
	strvec_pushl(&format_patch.دلیلاں, "format-patch", "-k", "--stdout",
		     "--full-index", "--cherry-pick", "--right-only",
		     "--default-prefix", "--no-renames",
		     "--no-cover-letter", "--pretty=mboxrd", "--topo-order",
		     "--no-base", عدم);
	إذا (opts->git_format_patch_opt.len)
		strvec_ونڈو(&format_patch.دلیلاں,
			     opts->git_format_patch_opt.buf);
	strvec_pushf(&format_patch.دلیلاں, "%s...%s",
		     oid_منزل_hex(opts->جڑ ?
				/* this is now equivalent to !opts->upstream */
				&opts->onto->object.oid :
				&opts->upstream->object.oid),
		     oid_منزل_hex(&opts->orig_سر->object.oid));
	إذا (opts->restrict_revision)
		strvec_pushf(&format_patch.دلیلاں, "^%s",
			     oid_منزل_hex(&opts->restrict_revision->object.oid));

	حالت = لگاتار_command(&format_patch);
	إذا (حالت) {
		بنية reset_working_درخت_options ropts = { 0 };
		unlink(rebased_patches);
		حرر(rebased_patches);
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

		أرجع حالت;
	}

	am.وچ = open(rebased_patches, O_RDONLY);
	إذا (am.وچ < 0) {
		حالت = error_errno(_("could not open '%s' for reading"),
				     rebased_patches);
		حرر(rebased_patches);
		بچہ_process_clear(&am);
		أرجع حالت;
	}

	strvec_pushv(&am.دلیلاں, opts->git_am_opts.v);
	strvec_ڈالو(&am.دلیلاں, "--rebasing");
	strvec_pushf(&am.دلیلاں, "--resolvemsg=%s", rebase_resolvemsg);
	strvec_ڈالو(&am.دلیلاں, "--patch-format=mboxrd");
	إذا (opts->allow_rerere_autoupdate == RERERE_AUTOUPDATE)
		strvec_ڈالو(&am.دلیلاں, "--rerere-autoupdate");
	وإلا إذا (opts->allow_rerere_autoupdate == RERERE_NOAUTOUPDATE)
		strvec_ڈالو(&am.دلیلاں, "--no-rerere-autoupdate");
	إذا (opts->gpg_sign_opt)
		strvec_ڈالو(&am.دلیلاں, opts->gpg_sign_opt);
	حالت = لگاتار_command(&am);
	unlink(rebased_patches);
	حرر(rebased_patches);

	إذا (!حالت) {
		أرجع move_منزل_original_branch(opts);
	}

	إذا (is_directory(opts->state_dir))
		rebase_write_basic_state(opts);

	أرجع حالت;
}

ساكن صحيح لگاتار_specific_rebase(بنية rebase_options *opts)
{
	صحيح حالت;

	إذا (opts->type == REBASE_ملاؤ) {
		/* Run sequencer-based rebase */
		إذا (!(opts->flags & REBASE_INTERACTIVE_EXPLICIT))
			setenv("GIT_SEQUENCE_EDITOR", ":", 1);
		إذا (opts->gpg_sign_opt) {
			/* remove the leading "-S" */
			حرف *tmp = xstrdup(opts->gpg_sign_opt + 2);
			حرر(opts->gpg_sign_opt);
			opts->gpg_sign_opt = tmp;
		}

		حالت = لگاتار_sequencer_rebase(opts);
	} وإلا إذا (opts->type == REBASE_APPLY)
		حالت = لگاتار_am(opts);
	وإلا
		BUG("Unhandled rebase type %d", opts->type);

	إذا (opts->dont_انت_rebase)
		; /* do nothing */
	وإلا إذا (opts->type == REBASE_ملاؤ) {
		صحيح quiet = !(opts->flags & (REBASE_NO_QUIET|REBASE_VERBOSE));

		/*
		 * The sequencer cleans up after itself. Its state directory
		 * is gone once it is done, and stays while it is stopped.
		 */
		إذا (حالت == 0 && !is_directory(opts->state_dir))
			لگاتار_auto_maintenance(the_repository, quiet);
	} وإلا إذا (حالت == 0) {
		إذا (!file_exists(state_dir_رستہ("stopped-sha", opts)))
			انت_rebase(opts);
	} وإلا إذا (حالت == 2) {
		بنية strbuf dir = STRBUF_INIT;

		apply_autostash(state_dir_رستہ("autostash", opts));
		strbuf_addstr(&dir, opts->state_dir);
		remove_dir_recursively(&dir, 0);
		strbuf_release(&dir);
		die("Nothing to do");
	}

	أرجع حالت ? -1 : 0;
}

ساكن فراغ parse_rebase_merges_قدر(بنية rebase_options *options, ثابت حرف *قدر)
{
	إذا (!strcmp("no-rebase-cousins", قدر))
		options->rebase_cousins = 0;
	وإلا إذا (!strcmp("rebase-cousins", قدر))
		options->rebase_cousins = 1;
	وإلا
		die(_("Unknown rebase-merges mode: %s"), قدر);
}

ساكن صحيح rebase_config(ثابت حرف *var, ثابت حرف *قدر,
			 ثابت بنية config_context *ctx, فراغ *انکڑے)
{
	بنية rebase_options *opts = انکڑے;

	إذا (!strcmp(var, "rebase.stat")) {
		إذا (git_config_bool(var, قدر))
			opts->flags |= REBASE_DIFFSTAT;
		وإلا
			opts->flags &= ~REBASE_DIFFSTAT;
		أرجع 0;
	}

	إذا (!strcmp(var, "rebase.autosquash")) {
		opts->config_autosquash = git_config_bool(var, قدر);
		أرجع 0;
	}

	إذا (!strcmp(var, "commit.gpgsign")) {
		حرر(opts->gpg_sign_opt);
		opts->gpg_sign_opt = git_config_bool(var, قدر) ?
			xstrdup("-S") : عدم;
		أرجع 0;
	}

	إذا (!strcmp(var, "rebase.autostash")) {
		opts->autostash = git_config_bool(var, قدر);
		أرجع 0;
	}

	إذا (!strcmp(var, "rebase.rebasemerges")) {
		opts->config_rebase_merges = git_parse_maybe_bool(قدر);
		إذا (opts->config_rebase_merges < 0) {
			opts->config_rebase_merges = 1;
			parse_rebase_merges_قدر(opts, قدر);
		} وإلا {
			opts->rebase_cousins = 0;
		}
		أرجع 0;
	}

	إذا (!strcmp(var, "rebase.updaterefs")) {
		opts->config_update_refs = git_config_bool(var, قدر);
		أرجع 0;
	}

	إذا (!strcmp(var, "rebase.reschedulefailedexec")) {
		opts->reschedule_failed_exec = git_config_bool(var, قدر);
		أرجع 0;
	}

	إذا (!strcmp(var, "rebase.forkpoint")) {
		opts->fork_نقطہ = git_config_bool(var, قدر) ? -1 : 0;
		أرجع 0;
	}

	إذا (!strcmp(var, "rebase.backend")) {
		FREE_AND_NULL(opts->default_backend);
		أرجع git_config_string(&opts->default_backend, var, قدر);
	}

	أرجع git_default_config(var, قدر, ctx, انکڑے);
}

ساكن صحيح checkout_اتے_منزل_date(بنية rebase_options *options)
{
	بنية strbuf buf = STRBUF_INIT;
	بنية reset_working_درخت_options ropts = { 0 };
	صحيح ret = 0;

	strbuf_addf(&buf, "%s: checkout %s",
		    options->reflog_action, options->switch_منزل);
	ropts.oid = &options->orig_سر->object.oid;
	ropts.branch = options->سر_name;
	ropts.flags = RESET_WORKING_درخت_لگاتار_POST_CHECKOUT_HOOK |
		      RESET_WORKING_درخت_UPDATE_سر;
	إذا (!ropts.branch)
		ropts.flags |=  RESET_WORKING_درخت_DETACH;
	ropts.سر_پیغام = buf.buf;
	إذا (reset_working_درخت(the_repository, &ropts) < 0)
		ret = error(_("could not switch to %s"), options->switch_منزل);
	strbuf_release(&buf);

	أرجع ret;
}

/*
 * Determines whether the commits in from..to are linear, i.e. contain
 * no merge commits. This function *expects* `from` to be an ancestor of
 * `to`.
 */
ساكن صحيح is_خطی_history(بنية commit *from, بنية commit *منزل)
{
	طالما (منزل && منزل != from) {
		repo_parse_commit(the_repository, منزل);
		إذا (!منزل->parents)
			أرجع 1;
		إذا (منزل->parents->اگلا)
			أرجع 0;
		منزل = منزل->parents->شے;
	}
	أرجع 1;
}

ساكن صحيح can_fast_forward(بنية commit *onto, بنية commit *upstream,
			    بنية commit *restrict_revision,
			    بنية commit *سر, بنية object_id *branch_ادھار)
{
	بنية commit_list *ملاؤ_bases = عدم;
	صحيح res = 0;

	إذا (is_null_oid(branch_ادھار))
		اذهب پورا; /* fill_branch_base() found multiple merge bases */

	إذا (!oideq(branch_ادھار, &onto->object.oid))
		اذهب پورا;

	إذا (restrict_revision && !oideq(&restrict_revision->object.oid, branch_ادھار))
		اذهب پورا;

	إذا (!upstream)
		اذهب پورا;

	إذا (repo_get_ملاؤ_bases(the_repository, upstream, سر, &ملاؤ_bases) < 0)
		باہر_نکلو(128);
	إذا (!ملاؤ_bases || ملاؤ_bases->اگلا)
		اذهب پورا;

	إذا (!oideq(&onto->object.oid, &ملاؤ_bases->شے->object.oid))
		اذهب پورا;

	res = 1;

پورا:
	commit_list_free(ملاؤ_bases);
	أرجع res && is_خطی_history(onto, سر);
}

ساكن فراغ fill_branch_ادھار(بنية rebase_options *options,
			    بنية object_id *branch_ادھار)
{
	بنية commit_list *ملاؤ_bases = عدم;

	إذا (repo_get_ملاؤ_bases(the_repository, options->onto,
				 options->orig_سر, &ملاؤ_bases) < 0)
		باہر_نکلو(128);
	إذا (!ملاؤ_bases || ملاؤ_bases->اگلا)
		oidcpy(branch_ادھار, null_oid(the_hash_algo));
	وإلا
		oidcpy(branch_ادھار, &ملاؤ_bases->شے->object.oid);

	commit_list_free(ملاؤ_bases);
}

ساكن صحيح parse_opt_am(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	بنية rebase_options *opts = opt->قدر;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	إذا (opts->type != REBASE_UNSPECIFIED && opts->type != REBASE_APPLY)
	    die(_("apply options and merge options cannot be used together"));

	opts->type = REBASE_APPLY;

	أرجع 0;
}

/* -i followed by -m is still -i */
ساكن صحيح parse_opt_ملاؤ(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	بنية rebase_options *opts = opt->قدر;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	إذا (opts->type != REBASE_UNSPECIFIED && opts->type != REBASE_ملاؤ)
	    die(_("apply options and merge options cannot be used together"));

	opts->type = REBASE_ملاؤ;

	أرجع 0;
}

/* -i followed by -r is still explicitly interactive, but -r alone is not */
ساكن صحيح parse_opt_interactive(ثابت بنية option *opt, ثابت حرف *arg,
				 صحيح unset)
{
	بنية rebase_options *opts = opt->قدر;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	إذا (opts->type != REBASE_UNSPECIFIED && opts->type != REBASE_ملاؤ)
	    die(_("apply options and merge options cannot be used together"));

	opts->type = REBASE_ملاؤ;
	opts->flags |= REBASE_INTERACTIVE_EXPLICIT;

	أرجع 0;
}

ساكن تعداد خالی_تھاواں_type parse_خالی_تھاواں_قدر(ثابت حرف *قدر)
{
	إذا (!strcasecmp(قدر, "drop"))
		أرجع خالی_تھاواں_DROP;
	وإلا إذا (!strcasecmp(قدر, "keep"))
		أرجع خالی_تھاواں_KEEP;
	وإلا إذا (!strcasecmp(قدر, "stop"))
		أرجع خالی_تھاواں_STOP;
	وإلا إذا (!strcasecmp(قدر, "ask")) {
		warning(_("--empty=ask is deprecated; use '--empty=stop' instead."));
		أرجع خالی_تھاواں_STOP;
	}

	die(_("unrecognized empty type '%s'; valid values are \"drop\", \"keep\", and \"stop\"."), قدر);
}

ساكن صحيح parse_opt_keep_خالی_تھاواں(ثابت بنية option *opt, ثابت حرف *arg,
				صحيح unset)
{
	بنية rebase_options *opts = opt->قدر;

	BUG_ON_OPT_ARG(arg);

	imply_ملاؤ(opts, unset ? "--no-keep-empty" : "--keep-empty");
	opts->keep_خالی_تھاواں = !unset;
	أرجع 0;
}

ساكن صحيح parse_opt_خالی_تھاواں(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	بنية rebase_options *options = opt->قدر;
	تعداد خالی_تھاواں_type قدر = parse_خالی_تھاواں_قدر(arg);

	BUG_ON_OPT_NEG(unset);

	options->خالی_تھاواں = قدر;
	أرجع 0;
}

ساكن صحيح parse_opt_rebase_merges(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	بنية rebase_options *options = opt->قدر;

	options->rebase_merges = !unset;
	options->rebase_cousins = 0;

	إذا (arg) {
		إذا (!*arg) {
			warning(_("--rebase-merges with an empty string "
				  "argument is deprecated and will stop "
				  "working in a future version of Git. Use "
				  "--rebase-merges without an argument "
				  "instead, which does the same thing."));
			أرجع 0;
		}
		parse_rebase_merges_قدر(options, arg);
	}

	أرجع 0;
}

ساكن فراغ NORETURN error_on_missing_default_upstream(فراغ)
{
	بنية branch *current_branch = branch_get(عدم);

	لکھو(_("%s\n"
		 "Please specify which branch you want to rebase against.\n"
		 "See git-rebase(1) for details.\n"
		 "\n"
		 "    git rebase '<branch>'\n"
		 "\n"),
		current_branch ? _("There is no tracking information for "
			"the current branch.") :
			_("You are not currently on a branch."));

	إذا (current_branch) {
		ثابت حرف *remote = current_branch->remote_name;

		إذا (!remote)
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

ساكن صحيح check_exec_cmd(ثابت حرف *cmd)
{
	إذا (strchr(cmd, '\n'))
		أرجع error(_("exec commands cannot contain newlines"));

	/* Does the command consist purely of whitespace? */
	إذا (!cmd[strspn(cmd, " \t\r\f\v")])
		أرجع error(_("empty exec command"));

	أرجع 0;
}

صحيح cmd_rebase(صحيح argc,
	       ثابت حرف **argv,
	       ثابت حرف *اگیتر,
	       بنية repository *repo بن_ورتیا)
{
	بنية rebase_options options = REBASE_OPTIONS_INIT;
	ثابت حرف *branch_name;
	ثابت حرف *strategy_opt = عدم;
	صحيح ret, flags, کل_argc, وچ_ترقی = 0;
	صحيح keep_ادھار = 0;
	صحيح ok_منزل_skip_pre_rebase = 0;
	بنية strbuf پیغام = STRBUF_INIT;
	بنية strbuf revisions = STRBUF_INIT;
	بنية strbuf buf = STRBUF_INIT;
	بنية object_id branch_ادھار;
	صحيح ignore_whitespace = 0;
	ثابت حرف *gpg_sign = عدم;
	بنية object_id squash_onto;
	حرف *squash_onto_name = عدم;
	حرف *keep_ادھار_onto_name = عدم;
	صحيح reschedule_failed_exec = -1;
	صحيح allow_preemptive_ff = 1;
	صحيح preserve_merges_selected = 0;
	بنية reset_working_درخت_options ropts = { 0 };
	بنية option builtin_rebase_options[] = {
		OPT_STRING(0, "onto", &options.onto_name,
			   N_("revision"),
			   N_("rebase onto given branch instead of upstream")),
		OPT_BOOL(0, "keep-base", &keep_ادھار,
			 N_("use the merge-base of upstream and branch as the current base")),
		OPT_BOOL(0, "no-verify", &ok_منزل_skip_pre_rebase,
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
		OPT_STRVEC(0, "trailer", &options.trailer_دلیلاں, N_("trailer"),
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
		OPT_PASSTHRU_ARGV('C', عدم, &options.git_am_opts, N_("n"),
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
		OPT_CALLBACK_F(0, "apply", &options, عدم,
			N_("use apply strategies to rebase"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			parse_opt_am),
		OPT_CALLBACK_F('m', "merge", &options, عدم,
			N_("use merging strategies to rebase"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			parse_opt_ملاؤ),
		OPT_CALLBACK_F('i', "interactive", &options, عدم,
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
			       PARSE_OPT_NONEG, parse_opt_خالی_تھاواں),
		OPT_CALLBACK_F('k', "keep-empty", &options, عدم,
			N_("keep commits which start empty"),
			PARSE_OPT_NOARG | PARSE_OPT_HIDDEN,
			parse_opt_keep_خالی_تھاواں),
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
			   &options.allow_خالی_تھاواں_سنیہا,
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
		OPT_سرا(),
	};
	صحيح i;

	show_usage_with_options_if_asked(argc, argv,
					 builtin_rebase_usage,
					 builtin_rebase_options);

#جے_بنا_تعریف WITH_BREAKING_CHANGES
	warn_on_auto_comment_char = true;
#جے_انت /* !WITH_BREAKING_CHANGES */
	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_بھریا_اشاریہ = 0;

	repo_config(the_repository, rebase_config, &options);
	/* options.gpg_sign_opt will be either "-S" or NULL */
	gpg_sign = options.gpg_sign_opt ? "" : عدم;
	FREE_AND_NULL(options.gpg_sign_opt);

	strbuf_reset(&buf);
	strbuf_addf(&buf, "%s/applying", apply_dir());
	إذا(file_exists(buf.buf))
		die(_("It looks like 'git am' is in progress. Cannot rebase."));

	إذا (is_directory(apply_dir())) {
		options.type = REBASE_APPLY;
		options.state_dir = apply_dir();
	} وإلا إذا (is_directory(ملاؤ_dir())) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "%s/rewritten", ملاؤ_dir());
		إذا (!(options.action == ACTION_ABORT) && is_directory(buf.buf)) {
			die(_("`rebase --preserve-merges` (-p) is no longer supported.\n"
			"Use `git rebase --abort` to terminate current rebase.\n"
			"Or downgrade to v2.33, or earlier, to complete the rebase."));
		} وإلا {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "%s/interactive", ملاؤ_dir());
			options.type = REBASE_ملاؤ;
			إذا (file_exists(buf.buf))
				options.flags |= REBASE_INTERACTIVE_EXPLICIT;
		}
		options.state_dir = ملاؤ_dir();
	}

	إذا (options.type != REBASE_UNSPECIFIED)
		وچ_ترقی = 1;

	کل_argc = argc;
	argc = parse_options(argc, argv, اگیتر,
			     builtin_rebase_options,
			     builtin_rebase_usage, 0);

	إذا (options.trailer_دلیلاں.nr) {
		إذا (validate_trailer_دلیلاں(&options.trailer_دلیلاں))
			die(عدم);
		options.flags |= REBASE_FORCE;
	}

	إذا (preserve_merges_selected)
		die(_("--preserve-merges was replaced by --rebase-merges\n"
			"Note: Your `pull.rebase` configuration may also be set to 'preserve',\n"
			"which is no longer supported; use 'merges' instead"));

	إذا (options.action != ACTION_NONE && کل_argc != 2) {
		usage_with_options(builtin_rebase_usage,
				   builtin_rebase_options);
	}

	إذا (argc > 2)
		usage_with_options(builtin_rebase_usage,
				   builtin_rebase_options);

	إذا (keep_ادھار) {
		إذا (options.onto_name)
			die(_("options '%s' and '%s' cannot be used together"), "--keep-base", "--onto");
		إذا (options.جڑ)
			die(_("options '%s' and '%s' cannot be used together"), "--keep-base", "--root");
		/*
		 * --keep-base defaults to --no-fork-point to keep the
		 * base the same.
		 */
		إذا (options.fork_نقطہ < 0)
			options.fork_نقطہ = 0;
	}
	إذا (options.جڑ && options.fork_نقطہ > 0)
		die(_("options '%s' and '%s' cannot be used together"), "--root", "--fork-point");

	إذا (options.action != ACTION_NONE && !وچ_ترقی)
		die(_("no rebase in progress"));

	إذا (options.action == ACTION_EDIT_TODO && !is_ملاؤ(&options))
		die(_("The --edit-todo action can only be used during "
		      "interactive rebase."));

	إذا (trace2_is_enabled()) {
		إذا (is_ملاؤ(&options))
			trace2_cmd_منوال("interactive");
		وإلا إذا (options.exec.nr)
			trace2_cmd_منوال("interactive-exec");
		وإلا
			trace2_cmd_منوال(action_ناں[options.action]);
	}

	options.reflog_action = getenv(GIT_REFLOG_ACTION_ENVIRONMENT);
	options.reflog_action =
		xstrdup(options.reflog_action ? options.reflog_action : "rebase");

	بدل (options.action) {
	حالة ACTION_CONTINUE: {
		بنية object_id سر;
		بنية جندرا_file جندرا_file = جندرا_INIT;
		صحيح fd;

		/* Sanity check */
		إذا (repo_get_oid(the_repository, "HEAD", &سر))
			die(_("Cannot read HEAD"));

		fd = repo_hold_locked_اشاریہ(the_repository, &جندرا_file, 0);
		إذا (repo_read_اشاریہ(the_repository) < 0)
			die(_("could not read index"));
		refresh_اشاریہ(the_repository->اشاریہ, REFRESH_QUIET, عدم, عدم,
			      عدم);
		إذا (0 <= fd)
			repo_update_اشاریہ_if_able(the_repository, &جندرا_file);
		rollback_جندرا_file(&جندرا_file);

		إذا (has_unstaged_changes(the_repository, 1)) {
			puts(_("You must edit all merge conflicts and then\n"
			       "mark them as resolved using git add"));
			باہر_نکلو(1);
		}
		إذا (read_basic_state(&options))
			باہر_نکلو(1);
		اذهب لگاتار_rebase;
	}
	حالة ACTION_SKIP: {
		بنية string_list ملاؤ_rr = STRING_LIST_INIT_DUP;

		rerere_clear(the_repository, &ملاؤ_rr);
		string_list_clear(&ملاؤ_rr, 1);
		ropts.flags = RESET_WORKING_درخت_HARD |
			      RESET_WORKING_درخت_UPDATE_سر;
		إذا (reset_working_درخت(the_repository, &ropts) < 0)
			die(_("could not discard worktree changes"));
		remove_branch_state(the_repository, 0);
		إذا (read_basic_state(&options))
			باہر_نکلو(1);
		اذهب لگاتار_rebase;
	}
	حالة ACTION_ABORT: {
		بنية string_list ملاؤ_rr = STRING_LIST_INIT_DUP;
		بنية strbuf سر_پیغام = STRBUF_INIT;

		rerere_clear(the_repository, &ملاؤ_rr);
		string_list_clear(&ملاؤ_rr, 1);

		إذا (read_basic_state(&options))
			باہر_نکلو(1);

		strbuf_addf(&سر_پیغام, "%s (abort): returning to %s",
			    options.reflog_action,
			    options.سر_name ? options.سر_name
					      : oid_منزل_hex(&options.orig_سر->object.oid));
		ropts.oid = &options.orig_سر->object.oid;
		ropts.سر_پیغام = سر_پیغام.buf;
		ropts.branch = options.سر_name;
		ropts.flags = RESET_WORKING_درخت_HARD |
			      RESET_WORKING_درخت_UPDATE_سر;
		إذا (reset_working_درخت(the_repository, &ropts) < 0)
			die(_("could not move back to %s"),
			    oid_منزل_hex(&options.orig_سر->object.oid));
		strbuf_release(&سر_پیغام);
		remove_branch_state(the_repository, 0);
		ret = انت_rebase(&options);
		اذهب cleanup;
	}
	حالة ACTION_QUIT: {
		save_autostash(state_dir_رستہ("autostash", &options));
		إذا (options.type == REBASE_ملاؤ) {
			بنية replay_opts replay = REPLAY_OPTS_INIT;

			replay.action = REPLAY_INTERACTIVE_REBASE;
			ret = sequencer_remove_state(&replay);
			replay_opts_release(&replay);
		} وإلا {
			strbuf_reset(&buf);
			strbuf_addstr(&buf, options.state_dir);
			ret = remove_dir_recursively(&buf, 0);
			إذا (ret)
				error(_("could not remove '%s'"),
				       options.state_dir);
		}
		اذهب cleanup;
	}
	حالة ACTION_EDIT_TODO:
		options.dont_انت_rebase = 1;
		اذهب لگاتار_rebase;
	حالة ACTION_SHOW_CURRENT_PATCH:
		options.dont_انت_rebase = 1;
		اذهب لگاتار_rebase;
	حالة ACTION_NONE:
		اكسر;
	افتراضي:
		BUG("action: %d", options.action);
	}

	/* Make sure no rebase is in progress */
	إذا (وچ_ترقی) {
		ثابت حرف *آخری_slash = strrchr(options.state_dir, '/');
		ثابت حرف *state_dir_ادھار =
			آخری_slash ? آخری_slash + 1 : options.state_dir;
		ثابت حرف *cmd_جیوندا_rebase =
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
		    state_dir_ادھار, cmd_جیوندا_rebase, buf.buf);
	}

	إذا ((options.flags & REBASE_INTERACTIVE_EXPLICIT) ||
	    (options.action != ACTION_NONE) ||
	    (options.exec.nr > 0) ||
	    options.autosquash == 1) {
		allow_preemptive_ff = 0;
	}
	إذا (options.committer_date_is_author_date || options.ignore_date)
		options.flags |= REBASE_FORCE;

	لكل (i = 0; i < options.git_am_opts.nr; i++) {
		ثابت حرف *option = options.git_am_opts.v[i], *p;
		إذا (!strcmp(option, "--whitespace=fix") ||
		    !strcmp(option, "--whitespace=strip"))
			allow_preemptive_ff = 0;
		وإلا إذا (skip_اگیتر(option, "-C", &p)) {
			طالما (*p)
				إذا (!ہندسہ_اے(*(p++)))
					die(_("switch `C' expects a "
					      "numerical value"));
		} وإلا إذا (skip_اگیتر(option, "--whitespace=", &p)) {
			إذا (*p && strcmp(p, "warn") && strcmp(p, "nowarn") &&
			    strcmp(p, "error") && strcmp(p, "error-all"))
				die("Invalid whitespace option: '%s'", p);
		}
	}

	لكل (i = 0; i < options.exec.nr; i++)
		إذا (check_exec_cmd(options.exec.اشیاء[i].string))
			باہر_نکلو(1);

	إذا (!(options.flags & REBASE_NO_QUIET))
		strvec_ڈالو(&options.git_am_opts, "-q");

	إذا (options.خالی_تھاواں != خالی_تھاواں_UNSPECIFIED)
		imply_ملاؤ(&options, "--empty");

	إذا (options.reapply_cherry_picks < 0)
		/*
		 * We default to --no-reapply-cherry-picks unless
		 * --keep-base is given; when --keep-base is given, we want
		 * to default to --reapply-cherry-picks.
		 */
		options.reapply_cherry_picks = keep_ادھار;
	وإلا إذا (!keep_ادھار)
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

	إذا (gpg_sign)
		options.gpg_sign_opt = xstrfmt("-S%s", gpg_sign);

	إذا (options.exec.nr)
		imply_ملاؤ(&options, "--exec");

	إذا (options.type == REBASE_APPLY) {
		إذا (ignore_whitespace)
			strvec_ڈالو(&options.git_am_opts,
				    "--ignore-whitespace");
		إذا (options.committer_date_is_author_date)
			strvec_ڈالو(&options.git_am_opts,
				    "--committer-date-is-author-date");
		إذا (options.ignore_date)
			strvec_ڈالو(&options.git_am_opts, "--ignore-date");
	} وإلا {
		/* REBASE_MERGE */
		إذا (ignore_whitespace) {
			string_list_append(&options.strategy_opts,
					   "ignore-space-change");
		}
	}

	إذا (strategy_opt)
		options.strategy = xstrdup(strategy_opt);
	وإلا إذا (options.strategy_opts.nr && !options.strategy)
		options.strategy = xstrdup("ort");
	إذا (options.strategy)
		imply_ملاؤ(&options, "--strategy");

	إذا (options.جڑ && !options.onto_name)
		imply_ملاؤ(&options, "--root without --onto");

	إذا (options.trailer_دلیلاں.nr)
		imply_ملاؤ(&options, "--trailer");

	إذا (isatty(2) && options.flags & REBASE_NO_QUIET)
		strbuf_addstr(&options.git_format_patch_opt, " --progress");

	إذا (options.git_am_opts.nr || options.type == REBASE_APPLY) {
		/* all am options except -q are compatible only with --apply */
		لكل (i = options.git_am_opts.nr - 1; i >= 0; i--)
			إذا (strcmp(options.git_am_opts.v[i], "-q"))
				اكسر;

		إذا (i >= 0 || options.type == REBASE_APPLY) {
			إذا (is_ملاؤ(&options))
				die(_("apply options and merge options "
					  "cannot be used together"));
			وإلا إذا (options.rebase_merges == -1 && options.config_rebase_merges == 1)
				die(_("apply options are incompatible with rebase.rebaseMerges.  Consider adding --no-rebase-merges"));
			وإلا إذا (options.update_refs == -1 && options.config_update_refs == 1)
				die(_("apply options are incompatible with rebase.updateRefs.  Consider adding --no-update-refs"));
			وإلا
				options.type = REBASE_APPLY;
		}
	}

	إذا (options.update_refs == 1)
		imply_ملاؤ(&options, "--update-refs");
	options.update_refs = (options.update_refs >= 0) ? options.update_refs :
			     ((options.config_update_refs >= 0) ? options.config_update_refs : 0);

	إذا (options.rebase_merges == 1)
		imply_ملاؤ(&options, "--rebase-merges");
	options.rebase_merges = (options.rebase_merges >= 0) ? options.rebase_merges :
				((options.config_rebase_merges >= 0) ? options.config_rebase_merges : 0);

	إذا (options.autosquash == 1) {
		imply_ملاؤ(&options, "--autosquash");
	} وإلا إذا (options.autosquash == -1) {
		options.autosquash =
			options.config_autosquash &&
			(options.flags & REBASE_INTERACTIVE_EXPLICIT);
	}

	إذا (options.type == REBASE_UNSPECIFIED) {
		إذا (!strcmp(options.default_backend, "merge"))
			options.type = REBASE_ملاؤ;
		وإلا إذا (!strcmp(options.default_backend, "apply"))
			options.type = REBASE_APPLY;
		وإلا
			die(_("Unknown rebase backend: %s"),
			    options.default_backend);
	}

	بدل (options.type) {
	حالة REBASE_ملاؤ:
		options.state_dir = ملاؤ_dir();
		اكسر;
	حالة REBASE_APPLY:
		options.state_dir = apply_dir();
		اكسر;
	افتراضي:
		BUG("options.type was just set above; should be unreachable.");
	}

	إذا (options.خالی_تھاواں == خالی_تھاواں_UNSPECIFIED) {
		إذا (options.flags & REBASE_INTERACTIVE_EXPLICIT)
			options.خالی_تھاواں = خالی_تھاواں_STOP;
		وإلا إذا (options.exec.nr > 0)
			options.خالی_تھاواں = خالی_تھاواں_KEEP;
		وإلا
			options.خالی_تھاواں = خالی_تھاواں_DROP;
	}
	إذا (reschedule_failed_exec > 0 && !is_ملاؤ(&options))
		die(_("--reschedule-failed-exec requires "
		      "--exec or --interactive"));
	إذا (reschedule_failed_exec >= 0)
		options.reschedule_failed_exec = reschedule_failed_exec;

	إذا (options.signoff) {
		strvec_ڈالو(&options.git_am_opts, "--signoff");
		options.flags |= REBASE_FORCE;
	}

	إذا (!options.جڑ) {
		إذا (argc < 1) {
			بنية branch *branch;

			branch = branch_get(عدم);
			options.upstream_name = branch_get_upstream(branch,
								    عدم);
			إذا (!options.upstream_name)
				error_on_missing_default_upstream();
			إذا (options.fork_نقطہ < 0)
				options.fork_نقطہ = 1;
		} وإلا {
			options.upstream_name = argv[0];
			argc--;
			argv++;
			إذا (!strcmp(options.upstream_name, "-"))
				options.upstream_name = "@{-1}";
		}
		options.upstream =
			lookup_commit_reference_by_name(options.upstream_name);
		إذا (!options.upstream)
			die(_("invalid upstream '%s'"), options.upstream_name);
		options.upstream_arg = options.upstream_name;
	} وإلا {
		إذا (!options.onto_name) {
			إذا (commit_درخت("", 0, the_hash_algo->خالی_تھاواں_درخت, عدم,
					&squash_onto, عدم, عدم) < 0)
				die(_("Could not create new root commit"));
			options.squash_onto = &squash_onto;
			options.onto_name = squash_onto_name =
				xstrdup(oid_منزل_hex(&squash_onto));
		} وإلا
			options.جڑ_with_onto = 1;

		options.upstream_name = عدم;
		options.upstream = عدم;
		إذا (argc > 1)
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
	إذا (argc == 1) {
		/* Is it "rebase other branchname" or "rebase other commit"? */
		بنية object_id branch_oid;
		branch_name = argv[0];
		options.switch_منزل = argv[0];

		/* Is it a local branch? */
		strbuf_reset(&buf);
		strbuf_addf(&buf, "refs/heads/%s", branch_name);
		إذا (!refs_read_ref(get_main_ref_store(the_repository), buf.buf, &branch_oid)) {
			die_if_checked_باہرلا(buf.buf, 1);
			options.سر_name = xstrdup(buf.buf);
			options.orig_سر =
				lookup_commit_object(the_repository,
						     &branch_oid);
		/* If not is it a valid ref (branch or commit)? */
		} وإلا {
			options.orig_سر =
				lookup_commit_reference_by_name(branch_name);
			options.سر_name = عدم;
		}
		إذا (!options.orig_سر)
			die(_("no such branch/commit '%s'"), branch_name);
	} وإلا إذا (argc == 0) {
		/* Do not need to switch branches, we are already on it. */
		options.سر_name =
			xstrdup_or_null(refs_resolve_ref_unsafe(get_main_ref_store(the_repository), "HEAD", 0, عدم,
								&flags));
		إذا (!options.سر_name)
			die(_("No such ref: %s"), "HEAD");
		إذا (flags & REF_ISSYMREF) {
			إذا (!skip_اگیتر(options.سر_name,
					 "refs/heads/", &branch_name))
				branch_name = options.سر_name;

		} وإلا {
			FREE_AND_NULL(options.سر_name);
			branch_name = "HEAD";
		}
		options.orig_سر = lookup_commit_reference_by_name("HEAD");
		إذا (!options.orig_سر)
			die(_("Could not resolve HEAD to a commit"));
	} وإلا
		BUG("unexpected number of arguments left to parse");

	/* Make sure the branch to rebase onto is valid. */
	إذا (keep_ادھار) {
		strbuf_reset(&buf);
		strbuf_addstr(&buf, options.upstream_name);
		strbuf_addstr(&buf, "...");
		strbuf_addstr(&buf, branch_name);
		options.onto_name = keep_ادھار_onto_name = xstrdup(buf.buf);
	} وإلا إذا (!options.onto_name)
		options.onto_name = options.upstream_name;
	إذا (لڑی_کھوج(options.onto_name, "...")) {
		إذا (repo_get_oid_mb(the_repository, options.onto_name, &branch_ادھار) < 0) {
			إذا (keep_ادھار)
				die(_("'%s': need exactly one merge base with branch"),
				    options.upstream_name);
			وإلا
				die(_("'%s': need exactly one merge base"),
				    options.onto_name);
		}
		options.onto = lookup_commit_or_die(&branch_ادھار,
						    options.onto_name);
	} وإلا {
		options.onto =
			lookup_commit_reference_by_name(options.onto_name);
		إذا (!options.onto)
			die(_("Does not point to a valid commit '%s'"),
				options.onto_name);
		fill_branch_ادھار(&options, &branch_ادھار);
	}

	إذا (keep_ادھار && options.reapply_cherry_picks)
		options.upstream = options.onto;

	إذا (options.fork_نقطہ > 0)
		options.restrict_revision =
			get_fork_نقطہ(options.upstream_name, options.orig_سر);

	إذا (repo_read_اشاریہ(the_repository) < 0)
		die(_("could not read index"));

	إذا (options.autostash)
		create_autostash(the_repository,
				 state_dir_رستہ("autostash", &options));


	إذا (require_clean_work_درخت(the_repository, "rebase",
				    _("Please commit or stash them."), 1, 1)) {
		ret = -1;
		اذهب cleanup_autostash;
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
	إذا (allow_preemptive_ff &&
	    can_fast_forward(options.onto, options.upstream, options.restrict_revision,
			     options.orig_سر, &branch_ادھار)) {
		صحيح flag;

		إذا (!(options.flags & REBASE_FORCE)) {
			/* Lazily switch to the target branch if needed... */
			إذا (options.switch_منزل) {
				ret = checkout_اتے_منزل_date(&options);
				إذا (ret)
					اذهب cleanup_autostash;
			}

			إذا (!(options.flags & REBASE_NO_QUIET))
				; /* be quiet */
			وإلا إذا (!strcmp(branch_name, "HEAD") &&
				 refs_resolve_ref_unsafe(get_main_ref_store(the_repository), "HEAD", 0, عدم, &flag))
				puts(_("HEAD is up to date."));
			وإلا
				لکھو(_("Current branch %s is up to date.\n"),
				       branch_name);
			ret = انت_rebase(&options);
			اذهب cleanup;
		} وإلا إذا (!(options.flags & REBASE_NO_QUIET))
			; /* be quiet */
		وإلا إذا (!strcmp(branch_name, "HEAD") &&
			 refs_resolve_ref_unsafe(get_main_ref_store(the_repository), "HEAD", 0, عدم, &flag))
			puts(_("HEAD is up to date, rebase forced."));
		وإلا
			لکھو(_("Current branch %s is up to date, rebase "
				 "forced.\n"), branch_name);
	}

	/* If a hook exists, give it a chance to interrupt*/
	إذا (!ok_منزل_skip_pre_rebase &&
	    لگاتار_hooks_l(the_repository, "pre-rebase", options.upstream_arg,
			argc ? argv[0] : عدم, عدم)) {
		ret = error(_("The pre-rebase hook refused to rebase."));
		اذهب cleanup_autostash;
	}

	إذا (options.flags & REBASE_DIFFSTAT) {
		بنية diff_options opts;

		إذا (options.flags & REBASE_VERBOSE) {
			إذا (is_null_oid(&branch_ادھار))
				لکھو(_("Changes to %s:\n"),
				       oid_منزل_hex(&options.onto->object.oid));
			وإلا
				لکھو(_("Changes from %s to %s:\n"),
				       oid_منزل_hex(&branch_ادھار),
				       oid_منزل_hex(&options.onto->object.oid));
		}

		/* We want color (if set), but no pager */
		repo_diff_تیاری(the_repository, &opts);
		init_diffstat_widths(&opts);
		opts.output_format |=
			DIFF_FORMAT_SUMMARY | DIFF_FORMAT_DIFFSTAT;
		opts.detect_rename = DIFF_DETECT_RENAME;
		diff_تیاری_پورا(&opts);
		diff_درخت_oid(is_null_oid(&branch_ادھار) ?
			      the_hash_algo->خالی_تھاواں_درخت : &branch_ادھار,
			      &options.onto->object.oid, "", &opts);
		diffcore_std(&opts);
		diff_flush(&opts);
	}

	إذا (is_ملاؤ(&options))
		اذهب لگاتار_rebase;

	/* Detach HEAD and reset the tree */
	إذا (options.flags & REBASE_NO_QUIET)
		لکھو(_("First, rewinding head to replay your work on top of "
			 "it...\n"));

	strbuf_addf(&پیغام, "%s (start): checkout %s",
		    options.reflog_action, options.onto_name);
	ropts.oid = &options.onto->object.oid;
	ropts.orig_سر = &options.orig_سر->object.oid;
	ropts.flags = RESET_WORKING_درخت_DETACH |
		      RESET_WORKING_درخت_UPDATE_سر |
		      RESET_WORKING_درخت_UPDATE_ORIG_سر |
		      RESET_WORKING_درخت_لگاتار_POST_CHECKOUT_HOOK;
	ropts.سر_پیغام = پیغام.buf;
	ropts.default_reflog_action = options.reflog_action;
	إذا (reset_working_درخت(the_repository, &ropts)) {
		ret = error(_("Could not detach HEAD"));
		اذهب cleanup_autostash;
	}

	/*
	 * If the onto is a proper descendant of the tip of the branch, then
	 * we just fast-forwarded.
	 */
	إذا (oideq(&branch_ادھار, &options.orig_سر->object.oid)) {
		لکھو(_("Fast-forwarded %s to %s.\n"),
			branch_name, options.onto_name);
		move_منزل_original_branch(&options);
		ret = انت_rebase(&options);
		اذهب cleanup;
	}

	strbuf_addf(&revisions, "%s..%s",
		    options.جڑ ? oid_منزل_hex(&options.onto->object.oid) :
		    (options.restrict_revision ?
		     oid_منزل_hex(&options.restrict_revision->object.oid) :
		     oid_منزل_hex(&options.upstream->object.oid)),
		    oid_منزل_hex(&options.orig_سر->object.oid));

	options.revisions = revisions.buf;

لگاتار_rebase:
	ret = لگاتار_specific_rebase(&options);

cleanup:
	strbuf_release(&buf);
	strbuf_release(&پیغام);
	strbuf_release(&revisions);
	rebase_options_release(&options);
	حرر(squash_onto_name);
	حرر(keep_ادھار_onto_name);
	أرجع !!ret;

cleanup_autostash:
	ret |= !!cleanup_autostash(&options);
	اذهب cleanup;
}
