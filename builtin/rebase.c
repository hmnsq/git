/*
 * "git rebase" builtin command
 *
 * Copyright (c) 2018 Pratik Karki
 */

#నిర్వచించు USE_THE_REPOSITORY_VARIABLE
#నిర్వచించు DISABLE_SIGN_COMPARE_WARNINGS

#అన్తర్భావ "builtin.h"

#అన్తర్భావ "abspath.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "hex.h"
#అన్తర్భావ "run-command.h"
#అన్తర్భావ "strvec.h"
#అన్తర్భావ "dir.h"
#అన్తర్భావ "refs.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "unpack-trees.h"
#అన్తర్భావ "lockfile.h"
#అన్తర్భావ "object-file.h"
#అన్తర్భావ "object-name.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "path.h"
#అన్తర్భావ "commit.h"
#అన్తర్భావ "diff.h"
#అన్తర్భావ "wt-status.h"
#అన్తర్భావ "revision.h"
#అన్తర్భావ "commit-reach.h"
#అన్తర్భావ "rerere.h"
#అన్తర్భావ "branch.h"
#అన్తర్భావ "sequencer.h"
#అన్తర్భావ "rebase-interactive.h"
#అన్తర్భావ "reset.h"
#అన్తర్భావ "trace2.h"
#అన్తర్భావ "hook.h"
#అన్తర్భావ "trailer.h"

స్థిర అక్షరం స్థిరరాశి * స్థిరరాశి builtin_rebase_usage[] = {
	N_("git rebase [-i] [options] [--exec <cmd>] "
		"[--onto <newbase> | --keep-base] [<upstream> [<branch>]]"),
	N_("git rebase [-i] [options] [--exec <cmd>] [--onto <newbase>] "
		"--root [<branch>]"),
	"git rebase --continue | --abort | --skip | --edit-todo",
	ఖాళీ
};

స్థిర GIT_మార్గం_FUNC(మార్గం_squash_onto, "rebase-merge/squash-onto")
స్థిర GIT_మార్గం_FUNC(మార్గం_interactive, "rebase-merge/interactive")
స్థిర GIT_మార్గం_FUNC(apply_dir, "rebase-apply")
స్థిర GIT_మార్గం_FUNC(విలీనం_dir, "rebase-merge")

గణన rebase_type {
	REBASE_UNSPECIFIED = -1,
	REBASE_APPLY,
	REBASE_విలీనం
};

గణన ఖాళీలు_type {
	ఖాళీలు_UNSPECIFIED = -1,
	ఖాళీలు_DROP,
	ఖాళీలు_KEEP,
	ఖాళీలు_STOP
};

గణన action {
	ACTION_NONE = 0,
	ACTION_CONTINUE,
	ACTION_SKIP,
	ACTION_ABORT,
	ACTION_QUIT,
	ACTION_EDIT_TODO,
	ACTION_SHOW_CURRENT_PATCH
};

స్థిర స్థిరరాశి అక్షరం *action_పేర్లు[] = {
	"undefined",
	"continue",
	"skip",
	"abort",
	"quit",
	"edit_todo",
	"show_current_patch"
};

నిర్మాణం rebase_options {
	గణన rebase_type type;
	గణన ఖాళీలు_type ఖాళీలు;
	అక్షరం *default_backend;
	స్థిరరాశి అక్షరం *state_dir;
	నిర్మాణం commit *upstream;
	స్థిరరాశి అక్షరం *upstream_name;
	స్థిరరాశి అక్షరం *upstream_arg;
	అక్షరం *తల_name;
	నిర్మాణం commit *orig_తల;
	నిర్మాణం commit *onto;
	స్థిరరాశి అక్షరం *onto_name;
	స్థిరరాశి అక్షరం *revisions;
	స్థిరరాశి అక్షరం *switch_గమ్యం;
	పూర్ణాంకం మూలం, మూలం_with_onto;
	నిర్మాణం object_id *squash_onto;
	నిర్మాణం commit *restrict_revision;
	పూర్ణాంకం dont_ముగింపు_rebase;
	గణన {
		REBASE_NO_QUIET = 1<<0,
		REBASE_VERBOSE = 1<<1,
		REBASE_DIFFSTAT = 1<<2,
		REBASE_FORCE = 1<<3,
		REBASE_INTERACTIVE_EXPLICIT = 1<<4,
	} flags;
	నిర్మాణం strvec git_am_opts;
	గణన action action;
	అక్షరం *reflog_action;
	పూర్ణాంకం signoff;
	నిర్మాణం strvec trailer_వాదనలు;
	పూర్ణాంకం allow_rerere_autoupdate;
	పూర్ణాంకం keep_ఖాళీలు;
	పూర్ణాంకం autosquash;
	అక్షరం *gpg_sign_opt;
	పూర్ణాంకం autostash;
	పూర్ణాంకం committer_date_is_author_date;
	పూర్ణాంకం ignore_date;
	నిర్మాణం string_list exec;
	పూర్ణాంకం allow_ఖాళీలు_సందేశం;
	పూర్ణాంకం rebase_merges, rebase_cousins;
	అక్షరం *strategy;
	నిర్మాణం string_list strategy_opts;
	నిర్మాణం strbuf git_format_patch_opt;
	పూర్ణాంకం reschedule_failed_exec;
	పూర్ణాంకం reapply_cherry_picks;
	పూర్ణాంకం fork_బిందువు;
	పూర్ణాంకం update_refs;
	పూర్ణాంకం config_autosquash;
	పూర్ణాంకం config_rebase_merges;
	పూర్ణాంకం config_update_refs;
};

#నిర్వచించు REBASE_OPTIONS_INIT {			  	\
		.type = REBASE_UNSPECIFIED,	  	\
		.ఖాళీలు = ఖాళీలు_UNSPECIFIED,	  	\
		.keep_ఖాళీలు = 1,			\
		.default_backend = xstrdup("merge"),  	\
		.flags = REBASE_NO_QUIET, 		\
		.git_am_opts = STRVEC_INIT,		\
		.exec = STRING_LIST_INIT_NODUP,		\
		.trailer_వాదనలు = STRVEC_INIT,		\
		.git_format_patch_opt = STRBUF_INIT,	\
		.fork_బిందువు = -1,			\
		.reapply_cherry_picks = -1,             \
		.allow_ఖాళీలు_సందేశం = 1,               \
		.autosquash = -1,                       \
		.rebase_merges = -1,                    \
		.config_rebase_merges = -1,             \
		.update_refs = -1,                      \
		.config_update_refs = -1,               \
		.strategy_opts = STRING_LIST_INIT_NODUP,\
	}

స్థిర శూన్య rebase_options_release(నిర్మాణం rebase_options *opts)
{
	విడుదల(opts->default_backend);
	విడుదల(opts->reflog_action);
	విడుదల(opts->తల_name);
	strvec_clear(&opts->git_am_opts);
	విడుదల(opts->gpg_sign_opt);
	string_list_clear(&opts->exec, 0);
	విడుదల(opts->strategy);
	string_list_clear(&opts->strategy_opts, 0);
	strbuf_release(&opts->git_format_patch_opt);
	strvec_clear(&opts->trailer_వాదనలు);
}

స్థిర నిర్మాణం replay_opts get_replay_opts(స్థిరరాశి నిర్మాణం rebase_options *opts)
{
	నిర్మాణం replay_opts replay = REPLAY_OPTS_INIT;

	replay.action = REPLAY_INTERACTIVE_REBASE;
	replay.strategy = ఖాళీ;
	sequencer_init_config(&replay);

	replay.signoff = opts->signoff;

	strvec_pushv(&replay.trailer_వాదనలు, opts->trailer_వాదనలు.v);

	replay.allow_ff = !(opts->flags & REBASE_FORCE);
	ఒకవేళ (opts->allow_rerere_autoupdate)
		replay.allow_rerere_auto = opts->allow_rerere_autoupdate;
	replay.allow_ఖాళీలు = 1;
	replay.allow_ఖాళీలు_సందేశం = opts->allow_ఖాళీలు_సందేశం;
	replay.drop_redundant_commits = (opts->ఖాళీలు == ఖాళీలు_DROP);
	replay.keep_redundant_commits = (opts->ఖాళీలు == ఖాళీలు_KEEP);
	replay.quiet = !(opts->flags & REBASE_NO_QUIET);
	replay.verbose = opts->flags & REBASE_VERBOSE;
	replay.reschedule_failed_exec = opts->reschedule_failed_exec;
	replay.committer_date_is_author_date =
					opts->committer_date_is_author_date;
	replay.ignore_date = opts->ignore_date;
	విడుదల(replay.gpg_sign);
	replay.gpg_sign = xstrdup_or_null(opts->gpg_sign_opt);
	replay.reflog_action = xstrdup(opts->reflog_action);
	ఒకవేళ (opts->strategy)
		replay.strategy = xstrdup_or_null(opts->strategy);
	లేకపోతే ఒకవేళ (!replay.strategy && replay.default_strategy) {
		replay.strategy = replay.default_strategy;
		replay.default_strategy = ఖాళీ;
	}

	కొరకు (కొలత_t i = 0; i < opts->strategy_opts.nr; i++)
		strvec_నెట్టు(&replay.xopts, opts->strategy_opts.వస్తువులు[i].string);

	ఒకవేళ (opts->squash_onto) {
		oidcpy(&replay.squash_onto, opts->squash_onto);
		replay.have_squash_onto = 1;
	}

	తిరిగి replay;
}

స్థిర పూర్ణాంకం edit_todo_file(గుర్తులేని flags, నిర్మాణం replay_opts *opts)
{
	స్థిరరాశి అక్షరం *todo_file = rebase_మార్గం_todo();
	నిర్మాణం todo_list todo_list = TODO_LIST_INIT,
		new_todo = TODO_LIST_INIT;
	పూర్ణాంకం res = 0;

	ఒకవేళ (strbuf_read_file(&todo_list.buf, todo_file, 0) < 0)
		తిరిగి error_errno(_("could not read '%s'."), todo_file);

	strbuf_stripspace(&todo_list.buf, comment_పంక్తి_str);
	res = edit_todo_list(the_repository, opts, &todo_list, &new_todo,
			     ఖాళీ, ఖాళీ, flags);
	ఒకవేళ (!res && todo_list_write_గమ్యం_file(the_repository, &new_todo, todo_file,
					    ఖాళీ, ఖాళీ, -1, flags & ~(TODO_LIST_SHORTEN_IDS)))
		res = error_errno(_("could not write '%s'"), todo_file);

	todo_list_release(&todo_list);
	todo_list_release(&new_todo);

	తిరిగి res;
}

స్థిర పూర్ణాంకం get_revision_ranges(నిర్మాణం commit *upstream, నిర్మాణం commit *onto,
			       నిర్మాణం object_id *orig_తల, అక్షరం **revisions,
			       అక్షరం **shortrevisions)
{
	నిర్మాణం commit *పునాది_rev = upstream ? upstream : onto;
	స్థిరరాశి అక్షరం *shorthead;

	*revisions = xstrfmt("%s...%s", oid_గమ్యం_hex(&పునాది_rev->object.oid),
			     oid_గమ్యం_hex(orig_తల));

	shorthead = repo_కనుగొను_unique_abbrev(the_repository, orig_తల,
					    DEFAULT_ABBREV);

	ఒకవేళ (upstream) {
		స్థిరరాశి అక్షరం *shortrev;

		shortrev = repo_కనుగొను_unique_abbrev(the_repository,
						   &పునాది_rev->object.oid,
						   DEFAULT_ABBREV);

		*shortrevisions = xstrfmt("%s..%s", shortrev, shorthead);
	} లేకపోతే
		*shortrevisions = xstrdup(shorthead);

	తిరిగి 0;
}

స్థిర పూర్ణాంకం init_basic_state(నిర్మాణం replay_opts *opts, స్థిరరాశి అక్షరం *తల_name,
			    నిర్మాణం commit *onto,
			    స్థిరరాశి నిర్మాణం object_id *orig_తల)
{
	FILE *interactive;

	ఒకవేళ (!is_directory(విలీనం_dir()) &&
	    సురక్షిత_create_dir_లోపల_gitdir(the_repository, విలీనం_dir()))
		తిరిగి error_errno(_("could not create temporary %s"), విలీనం_dir());

	refs_delete_reflog(get_main_ref_store(the_repository), "REBASE_HEAD");

	interactive = fopen(మార్గం_interactive(), "w");
	ఒకవేళ (!interactive)
		తిరిగి error_errno(_("could not mark as interactive"));
	fclose(interactive);

	తిరిగి write_basic_state(opts, తల_name, onto, orig_తల);
}

స్థిర పూర్ణాంకం do_interactive_rebase(నిర్మాణం rebase_options *opts, గుర్తులేని flags)
{
	పూర్ణాంకం ret = -1;
	అక్షరం *revisions = ఖాళీ, *shortrevisions = ఖాళీ;
	నిర్మాణం strvec make_script_వాదనలు = STRVEC_INIT;
	నిర్మాణం todo_list todo_list = TODO_LIST_INIT;
	నిర్మాణం replay_opts replay = get_replay_opts(opts);

	ఒకవేళ (get_revision_ranges(opts->upstream, opts->onto, &opts->orig_తల->object.oid,
				&revisions, &shortrevisions))
		వెళ్ళు cleanup;

	strvec_pushl(&make_script_వాదనలు, "", revisions, ఖాళీ);
	ఒకవేళ (opts->restrict_revision)
		strvec_pushf(&make_script_వాదనలు, "^%s",
			     oid_గమ్యం_hex(&opts->restrict_revision->object.oid));

	ret = sequencer_make_script(the_repository, &todo_list.buf,
				    &make_script_వాదనలు, flags);
	ఒకవేళ (ret) {
		error(_("could not generate todo list"));
		వెళ్ళు cleanup;
	}

	ఒకవేళ (init_basic_state(&replay,
			     opts->తల_name ? opts->తల_name : "detached HEAD",
			     opts->onto, &opts->orig_తల->object.oid))
		వెళ్ళు cleanup;

	ఒకవేళ (!opts->upstream && opts->squash_onto)
		write_file(మార్గం_squash_onto(), "%s\n",
			   oid_గమ్యం_hex(opts->squash_onto));

	discard_సూచీ(the_repository->సూచీ);
	ఒకవేళ (todo_list_parse_insn_బఫరు(the_repository, &replay,
					todo_list.buf.buf, &todo_list))
		BUG("unusable todo list");

	ret = complete_action(the_repository, &replay, flags,
		shortrevisions, opts->onto_name, opts->onto,
		&opts->orig_తల->object.oid, &opts->exec,
		opts->autosquash, opts->update_refs, &todo_list);

cleanup:
	replay_opts_release(&replay);
	విడుదల(revisions);
	విడుదల(shortrevisions);
	todo_list_release(&todo_list);
	strvec_clear(&make_script_వాదనలు);

	తిరిగి ret;
}

స్థిర పూర్ణాంకం నిరంతరం_sequencer_rebase(నిర్మాణం rebase_options *opts)
{
	గుర్తులేని flags = 0;
	పూర్ణాంకం abbreviate_commands = 0, ret = 0;

	repo_config_get_bool(the_repository, "rebase.abbreviatecommands", &abbreviate_commands);

	flags |= opts->keep_ఖాళీలు ? TODO_LIST_KEEP_ఖాళీలు : 0;
	flags |= abbreviate_commands ? TODO_LIST_ABBREVIATE_CMDS : 0;
	flags |= opts->rebase_merges ? TODO_LIST_REBASE_MERGES : 0;
	flags |= opts->rebase_cousins > 0 ? TODO_LIST_REBASE_COUSINS : 0;
	flags |= opts->మూలం_with_onto ? TODO_LIST_మూలం_WITH_ONTO : 0;
	flags |= opts->reapply_cherry_picks ? TODO_LIST_REAPPLY_CHERRY_PICKS : 0;
	flags |= opts->flags & REBASE_NO_QUIET ? TODO_LIST_WARN_SKIPPED_CHERRY_PICKS : 0;

	ఎంపిక (opts->action) {
	స్థితి ACTION_NONE: {
		ఒకవేళ (!opts->onto && !opts->upstream)
			die(_("a base commit must be provided with --upstream or --onto"));

		ret = do_interactive_rebase(opts, flags);
		ఆపు;
	}
	స్థితి ACTION_SKIP: {
		నిర్మాణం string_list విలీనం_rr = STRING_LIST_INIT_DUP;

		rerere_clear(the_repository, &విలీనం_rr);
	}
		/* fallthrough */
	స్థితి ACTION_CONTINUE: {
		నిర్మాణం replay_opts replay_opts = get_replay_opts(opts);

		ret = sequencer_continue(the_repository, &replay_opts);
		replay_opts_release(&replay_opts);
		ఆపు;
	}
	స్థితి ACTION_EDIT_TODO: {
		నిర్మాణం replay_opts replay_opts = get_replay_opts(opts);

		ret = edit_todo_file(flags, &replay_opts);
		replay_opts_release(&replay_opts);
		ఆపు;
	}
	స్థితి ACTION_SHOW_CURRENT_PATCH: {
		నిర్మాణం బిడ్డ_process cmd = బిడ్డ_PROCESS_INIT;

		cmd.git_cmd = 1;
		strvec_pushl(&cmd.వాదనలు, "show", "REBASE_HEAD", "--", ఖాళీ);
		ret = నిరంతరం_command(&cmd);

		ఆపు;
	}
	అప్రమేయ:
		BUG("invalid command '%d'", opts->action);
	}

	తిరిగి ret;
}

స్థిర పూర్ణాంకం is_విలీనం(నిర్మాణం rebase_options *opts)
{
	తిరిగి opts->type == REBASE_విలీనం;
}

స్థిర శూన్య imply_విలీనం(నిర్మాణం rebase_options *opts, స్థిరరాశి అక్షరం *option)
{
	ఎంపిక (opts->type) {
	స్థితి REBASE_APPLY:
		die(_("%s requires the merge backend"), option);
		ఆపు;
	స్థితి REBASE_విలీనం:
		ఆపు;
	అప్రమేయ:
		opts->type = REBASE_విలీనం; /* implied */
		ఆపు;
	}
}

/* Returns the filename prefixed by the state_dir */
స్థిర స్థిరరాశి అక్షరం *state_dir_మార్గం(స్థిరరాశి అక్షరం *filename, నిర్మాణం rebase_options *opts)
{
	స్థిర నిర్మాణం strbuf మార్గం = STRBUF_INIT;
	స్థిర కొలత_t ఉపసర్గ_len;

	ఒకవేళ (!ఉపసర్గ_len) {
		strbuf_addf(&మార్గం, "%s/", opts->state_dir);
		ఉపసర్గ_len = మార్గం.len;
	}

	strbuf_setlen(&మార్గం, ఉపసర్గ_len);
	strbuf_addstr(&మార్గం, filename);
	తిరిగి మార్గం.buf;
}

/* Initialize the rebase options from the state directory. */
స్థిర పూర్ణాంకం read_basic_state(నిర్మాణం rebase_options *opts)
{
	నిర్మాణం strbuf తల_name = STRBUF_INIT;
	నిర్మాణం strbuf buf = STRBUF_INIT;
	నిర్మాణం object_id oid;

	ఒకవేళ (!read_oneliner(&తల_name, state_dir_మార్గం("head-name", opts),
			   READ_ONELINER_WARN_MISSING) ||
	    !read_oneliner(&buf, state_dir_మార్గం("onto", opts),
			   READ_ONELINER_WARN_MISSING))
		తిరిగి -1;
	opts->తల_name = starts_with(తల_name.buf, "refs/") ?
		xstrdup(తల_name.buf) : ఖాళీ;
	strbuf_release(&తల_name);
	ఒకవేళ (get_oid_hex(buf.buf, &oid) ||
	    !(opts->onto = lookup_commit_object(the_repository, &oid)))
		తిరిగి error(_("invalid onto: '%s'"), buf.buf);

	/*
	 * We always write to orig-head, but interactive rebase used to write to
	 * head. Fall back to reading from head to cover for the case that the
	 * user upgraded git with an ongoing interactive rebase.
	 */
	strbuf_reset(&buf);
	ఒకవేళ (file_exists(state_dir_మార్గం("orig-head", opts))) {
		ఒకవేళ (!read_oneliner(&buf, state_dir_మార్గం("orig-head", opts),
				   READ_ONELINER_WARN_MISSING))
			తిరిగి -1;
	} లేకపోతే ఒకవేళ (!read_oneliner(&buf, state_dir_మార్గం("head", opts),
				  READ_ONELINER_WARN_MISSING))
		తిరిగి -1;
	ఒకవేళ (get_oid_hex(buf.buf, &oid) ||
	    !(opts->orig_తల = lookup_commit_object(the_repository, &oid)))
		తిరిగి error(_("invalid orig-head: '%s'"), buf.buf);

	ఒకవేళ (file_exists(state_dir_మార్గం("quiet", opts)))
		opts->flags &= ~REBASE_NO_QUIET;
	లేకపోతే
		opts->flags |= REBASE_NO_QUIET;

	ఒకవేళ (file_exists(state_dir_మార్గం("verbose", opts)))
		opts->flags |= REBASE_VERBOSE;

	ఒకవేళ (file_exists(state_dir_మార్గం("signoff", opts))) {
		opts->signoff = 1;
		opts->flags |= REBASE_FORCE;
	}

	ఒకవేళ (file_exists(state_dir_మార్గం("allow_rerere_autoupdate", opts))) {
		strbuf_reset(&buf);
		ఒకవేళ (!read_oneliner(&buf, state_dir_మార్గం("allow_rerere_autoupdate", opts),
				   READ_ONELINER_WARN_MISSING))
			తిరిగి -1;
		ఒకవేళ (!strcmp(buf.buf, "--rerere-autoupdate"))
			opts->allow_rerere_autoupdate = RERERE_AUTOUPDATE;
		లేకపోతే ఒకవేళ (!strcmp(buf.buf, "--no-rerere-autoupdate"))
			opts->allow_rerere_autoupdate = RERERE_NOAUTOUPDATE;
		లేకపోతే
			warning(_("ignoring invalid allow_rerere_autoupdate: "
				  "'%s'"), buf.buf);
	}

	ఒకవేళ (file_exists(state_dir_మార్గం("gpg_sign_opt", opts))) {
		strbuf_reset(&buf);
		ఒకవేళ (!read_oneliner(&buf, state_dir_మార్గం("gpg_sign_opt", opts),
				   READ_ONELINER_WARN_MISSING))
			తిరిగి -1;
		విడుదల(opts->gpg_sign_opt);
		opts->gpg_sign_opt = xstrdup(buf.buf);
	}

	strbuf_release(&buf);

	తిరిగి 0;
}

స్థిర పూర్ణాంకం rebase_write_basic_state(నిర్మాణం rebase_options *opts)
{
	write_file(state_dir_మార్గం("head-name", opts), "%s",
		   opts->తల_name ? opts->తల_name : "detached HEAD");
	write_file(state_dir_మార్గం("onto", opts), "%s",
		   opts->onto ? oid_గమ్యం_hex(&opts->onto->object.oid) : "");
	write_file(state_dir_మార్గం("orig-head", opts), "%s",
		   oid_గమ్యం_hex(&opts->orig_తల->object.oid));
	ఒకవేళ (!(opts->flags & REBASE_NO_QUIET))
		write_file(state_dir_మార్గం("quiet", opts), "%s", "");
	ఒకవేళ (opts->flags & REBASE_VERBOSE)
		write_file(state_dir_మార్గం("verbose", opts), "%s", "");
	ఒకవేళ (opts->allow_rerere_autoupdate > 0)
		write_file(state_dir_మార్గం("allow_rerere_autoupdate", opts),
			   "-%s-rerere-autoupdate",
			   opts->allow_rerere_autoupdate == RERERE_AUTOUPDATE ?
				"" : "-no");
	ఒకవేళ (opts->gpg_sign_opt)
		write_file(state_dir_మార్గం("gpg_sign_opt", opts), "%s",
			   opts->gpg_sign_opt);
	ఒకవేళ (opts->signoff)
		write_file(state_dir_మార్గం("signoff", opts), "--signoff");

	తిరిగి 0;
}

స్థిర పూర్ణాంకం cleanup_autostash(నిర్మాణం rebase_options *opts)
{
	పూర్ణాంకం ret;
	నిర్మాణం strbuf dir = STRBUF_INIT;
	స్థిరరాశి అక్షరం *మార్గం = state_dir_మార్గం("autostash", opts);

	ఒకవేళ (!file_exists(మార్గం))
		తిరిగి 0;
	ret = apply_autostash(మార్గం);
	strbuf_addstr(&dir, opts->state_dir);
	ఒకవేళ (remove_dir_recursively(&dir, 0))
		ret = error_errno(_("could not remove '%s'"), opts->state_dir);
	strbuf_release(&dir);

	తిరిగి ret;
}

స్థిర పూర్ణాంకం ముగింపు_rebase(నిర్మాణం rebase_options *opts)
{
	నిర్మాణం strbuf dir = STRBUF_INIT;
	పూర్ణాంకం ret = 0;

	refs_delete_ref(get_main_ref_store(the_repository), ఖాళీ,
			"REBASE_HEAD", ఖాళీ, REF_NO_DEREF);
	refs_delete_ref(get_main_ref_store(the_repository), ఖాళీ,
			"AUTO_MERGE", ఖాళీ, REF_NO_DEREF);
	apply_autostash(state_dir_మార్గం("autostash", opts));
	/*
	 * We ignore errors in 'git maintenance run --auto', since the
	 * user should see them.
	 */
	నిరంతరం_auto_maintenance(the_repository,
			!(opts->flags & (REBASE_NO_QUIET|REBASE_VERBOSE)));

	ఒకవేళ (opts->type == REBASE_విలీనం) {
		నిర్మాణం replay_opts replay = REPLAY_OPTS_INIT;

		replay.action = REPLAY_INTERACTIVE_REBASE;
		ret = sequencer_remove_state(&replay);
		replay_opts_release(&replay);
	} లేకపోతే {
		strbuf_addstr(&dir, opts->state_dir);
		ఒకవేళ (remove_dir_recursively(&dir, 0))
			ret = error(_("could not remove '%s'"),
				    opts->state_dir);
		strbuf_release(&dir);
	}

	తిరిగి ret;
}

స్థిర పూర్ణాంకం move_గమ్యం_original_branch(నిర్మాణం rebase_options *opts)
{
	నిర్మాణం strbuf branch_reflog = STRBUF_INIT, తల_reflog = STRBUF_INIT;
	నిర్మాణం reset_working_వృక్షం_options ropts = { 0 };
	పూర్ణాంకం ret;

	ఒకవేళ (!opts->తల_name)
		తిరిగి 0; /* nothing to move back to */

	ఒకవేళ (!opts->onto)
		BUG("move_to_original_branch without onto");

	strbuf_addf(&branch_reflog, "%s (finish): %s onto %s",
		    opts->reflog_action,
		    opts->తల_name, oid_గమ్యం_hex(&opts->onto->object.oid));
	strbuf_addf(&తల_reflog, "%s (finish): returning to %s",
		    opts->reflog_action, opts->తల_name);
	ropts.branch = opts->తల_name;
	ropts.flags = RESET_WORKING_వృక్షం_REFS_ONLY |
		      RESET_WORKING_వృక్షం_UPDATE_తల;
	ropts.branch_సందేశం = branch_reflog.buf;
	ropts.తల_సందేశం = తల_reflog.buf;
	ret = reset_working_వృక్షం(the_repository, &ropts);

	strbuf_release(&branch_reflog);
	strbuf_release(&తల_reflog);
	తిరిగి ret;
}

స్థిర పూర్ణాంకం నిరంతరం_am(నిర్మాణం rebase_options *opts)
{
	నిర్మాణం బిడ్డ_process am = బిడ్డ_PROCESS_INIT;
	నిర్మాణం బిడ్డ_process format_patch = బిడ్డ_PROCESS_INIT;
	పూర్ణాంకం స్థితి_2;
	అక్షరం *rebased_patches;

	am.git_cmd = 1;
	strvec_నెట్టు(&am.వాదనలు, "am");
	strvec_pushf(&am.env, GIT_REFLOG_ACTION_ENVIRONMENT "=%s (pick)",
		     opts->reflog_action);
	ఒకవేళ (opts->action == ACTION_CONTINUE) {
		strvec_నెట్టు(&am.వాదనలు, "--resolved");
		strvec_pushf(&am.వాదనలు, "--resolvemsg=%s", rebase_resolvemsg);
		ఒకవేళ (opts->gpg_sign_opt)
			strvec_నెట్టు(&am.వాదనలు, opts->gpg_sign_opt);
		స్థితి_2 = నిరంతరం_command(&am);
		ఒకవేళ (స్థితి_2)
			తిరిగి స్థితి_2;

		తిరిగి move_గమ్యం_original_branch(opts);
	}
	ఒకవేళ (opts->action == ACTION_SKIP) {
		strvec_నెట్టు(&am.వాదనలు, "--skip");
		strvec_pushf(&am.వాదనలు, "--resolvemsg=%s", rebase_resolvemsg);
		స్థితి_2 = నిరంతరం_command(&am);
		ఒకవేళ (స్థితి_2)
			తిరిగి స్థితి_2;

		తిరిగి move_గమ్యం_original_branch(opts);
	}
	ఒకవేళ (opts->action == ACTION_SHOW_CURRENT_PATCH) {
		strvec_నెట్టు(&am.వాదనలు, "--show-current-patch");
		తిరిగి నిరంతరం_command(&am);
	}

	rebased_patches = repo_git_మార్గం(the_repository, "rebased-patches");
	format_patch.బహిర్గతం = open(rebased_patches,
				O_WRONLY | O_CREAT | O_TRUNC, 0666);
	ఒకవేళ (format_patch.బహిర్గతం < 0) {
		స్థితి_2 = error_errno(_("could not open '%s' for writing"),
				     rebased_patches);
		విడుదల(rebased_patches);
		బిడ్డ_process_clear(&am);
		తిరిగి స్థితి_2;
	}

	format_patch.git_cmd = 1;
	strvec_pushl(&format_patch.వాదనలు, "format-patch", "-k", "--stdout",
		     "--full-index", "--cherry-pick", "--right-only",
		     "--default-prefix", "--no-renames",
		     "--no-cover-letter", "--pretty=mboxrd", "--topo-order",
		     "--no-base", ఖాళీ);
	ఒకవేళ (opts->git_format_patch_opt.len)
		strvec_విభజించు(&format_patch.వాదనలు,
			     opts->git_format_patch_opt.buf);
	strvec_pushf(&format_patch.వాదనలు, "%s...%s",
		     oid_గమ్యం_hex(opts->మూలం ?
				/* this is now equivalent to !opts->upstream */
				&opts->onto->object.oid :
				&opts->upstream->object.oid),
		     oid_గమ్యం_hex(&opts->orig_తల->object.oid));
	ఒకవేళ (opts->restrict_revision)
		strvec_pushf(&format_patch.వాదనలు, "^%s",
			     oid_గమ్యం_hex(&opts->restrict_revision->object.oid));

	స్థితి_2 = నిరంతరం_command(&format_patch);
	ఒకవేళ (స్థితి_2) {
		నిర్మాణం reset_working_వృక్షం_options ropts = { 0 };
		unlink(rebased_patches);
		విడుదల(rebased_patches);
		బిడ్డ_process_clear(&am);

		ropts.oid = &opts->orig_తల->object.oid;
		ropts.branch = opts->తల_name;
		ropts.default_reflog_action = opts->reflog_action;
		ropts.flags = RESET_WORKING_వృక్షం_UPDATE_తల;
		reset_working_వృక్షం(the_repository, &ropts);
		error(_("\ngit encountered an error while preparing the "
			"patches to replay\n"
			"these revisions:\n"
			"\n    %s\n\n"
			"As a result, git cannot rebase them."),
		      opts->revisions);

		తిరిగి స్థితి_2;
	}

	am.లోపల = open(rebased_patches, O_RDONLY);
	ఒకవేళ (am.లోపల < 0) {
		స్థితి_2 = error_errno(_("could not open '%s' for reading"),
				     rebased_patches);
		విడుదల(rebased_patches);
		బిడ్డ_process_clear(&am);
		తిరిగి స్థితి_2;
	}

	strvec_pushv(&am.వాదనలు, opts->git_am_opts.v);
	strvec_నెట్టు(&am.వాదనలు, "--rebasing");
	strvec_pushf(&am.వాదనలు, "--resolvemsg=%s", rebase_resolvemsg);
	strvec_నెట్టు(&am.వాదనలు, "--patch-format=mboxrd");
	ఒకవేళ (opts->allow_rerere_autoupdate == RERERE_AUTOUPDATE)
		strvec_నెట్టు(&am.వాదనలు, "--rerere-autoupdate");
	లేకపోతే ఒకవేళ (opts->allow_rerere_autoupdate == RERERE_NOAUTOUPDATE)
		strvec_నెట్టు(&am.వాదనలు, "--no-rerere-autoupdate");
	ఒకవేళ (opts->gpg_sign_opt)
		strvec_నెట్టు(&am.వాదనలు, opts->gpg_sign_opt);
	స్థితి_2 = నిరంతరం_command(&am);
	unlink(rebased_patches);
	విడుదల(rebased_patches);

	ఒకవేళ (!స్థితి_2) {
		తిరిగి move_గమ్యం_original_branch(opts);
	}

	ఒకవేళ (is_directory(opts->state_dir))
		rebase_write_basic_state(opts);

	తిరిగి స్థితి_2;
}

స్థిర పూర్ణాంకం నిరంతరం_specific_rebase(నిర్మాణం rebase_options *opts)
{
	పూర్ణాంకం స్థితి_2;

	ఒకవేళ (opts->type == REBASE_విలీనం) {
		/* Run sequencer-based rebase */
		ఒకవేళ (!(opts->flags & REBASE_INTERACTIVE_EXPLICIT))
			setenv("GIT_SEQUENCE_EDITOR", ":", 1);
		ఒకవేళ (opts->gpg_sign_opt) {
			/* remove the leading "-S" */
			అక్షరం *tmp = xstrdup(opts->gpg_sign_opt + 2);
			విడుదల(opts->gpg_sign_opt);
			opts->gpg_sign_opt = tmp;
		}

		స్థితి_2 = నిరంతరం_sequencer_rebase(opts);
	} లేకపోతే ఒకవేళ (opts->type == REBASE_APPLY)
		స్థితి_2 = నిరంతరం_am(opts);
	లేకపోతే
		BUG("Unhandled rebase type %d", opts->type);

	ఒకవేళ (opts->dont_ముగింపు_rebase)
		; /* do nothing */
	లేకపోతే ఒకవేళ (opts->type == REBASE_విలీనం) {
		పూర్ణాంకం quiet = !(opts->flags & (REBASE_NO_QUIET|REBASE_VERBOSE));

		/*
		 * The sequencer cleans up after itself. Its state directory
		 * is gone once it is done, and stays while it is stopped.
		 */
		ఒకవేళ (స్థితి_2 == 0 && !is_directory(opts->state_dir))
			నిరంతరం_auto_maintenance(the_repository, quiet);
	} లేకపోతే ఒకవేళ (స్థితి_2 == 0) {
		ఒకవేళ (!file_exists(state_dir_మార్గం("stopped-sha", opts)))
			ముగింపు_rebase(opts);
	} లేకపోతే ఒకవేళ (స్థితి_2 == 2) {
		నిర్మాణం strbuf dir = STRBUF_INIT;

		apply_autostash(state_dir_మార్గం("autostash", opts));
		strbuf_addstr(&dir, opts->state_dir);
		remove_dir_recursively(&dir, 0);
		strbuf_release(&dir);
		die("Nothing to do");
	}

	తిరిగి స్థితి_2 ? -1 : 0;
}

స్థిర శూన్య parse_rebase_merges_విలువ(నిర్మాణం rebase_options *options, స్థిరరాశి అక్షరం *విలువ)
{
	ఒకవేళ (!strcmp("no-rebase-cousins", విలువ))
		options->rebase_cousins = 0;
	లేకపోతే ఒకవేళ (!strcmp("rebase-cousins", విలువ))
		options->rebase_cousins = 1;
	లేకపోతే
		die(_("Unknown rebase-merges mode: %s"), విలువ);
}

స్థిర పూర్ణాంకం rebase_config(స్థిరరాశి అక్షరం *var, స్థిరరాశి అక్షరం *విలువ,
			 స్థిరరాశి నిర్మాణం config_context *ctx, శూన్య *దత్తాంశం)
{
	నిర్మాణం rebase_options *opts = దత్తాంశం;

	ఒకవేళ (!strcmp(var, "rebase.stat")) {
		ఒకవేళ (git_config_bool(var, విలువ))
			opts->flags |= REBASE_DIFFSTAT;
		లేకపోతే
			opts->flags &= ~REBASE_DIFFSTAT;
		తిరిగి 0;
	}

	ఒకవేళ (!strcmp(var, "rebase.autosquash")) {
		opts->config_autosquash = git_config_bool(var, విలువ);
		తిరిగి 0;
	}

	ఒకవేళ (!strcmp(var, "commit.gpgsign")) {
		విడుదల(opts->gpg_sign_opt);
		opts->gpg_sign_opt = git_config_bool(var, విలువ) ?
			xstrdup("-S") : ఖాళీ;
		తిరిగి 0;
	}

	ఒకవేళ (!strcmp(var, "rebase.autostash")) {
		opts->autostash = git_config_bool(var, విలువ);
		తిరిగి 0;
	}

	ఒకవేళ (!strcmp(var, "rebase.rebasemerges")) {
		opts->config_rebase_merges = git_parse_maybe_bool(విలువ);
		ఒకవేళ (opts->config_rebase_merges < 0) {
			opts->config_rebase_merges = 1;
			parse_rebase_merges_విలువ(opts, విలువ);
		} లేకపోతే {
			opts->rebase_cousins = 0;
		}
		తిరిగి 0;
	}

	ఒకవేళ (!strcmp(var, "rebase.updaterefs")) {
		opts->config_update_refs = git_config_bool(var, విలువ);
		తిరిగి 0;
	}

	ఒకవేళ (!strcmp(var, "rebase.reschedulefailedexec")) {
		opts->reschedule_failed_exec = git_config_bool(var, విలువ);
		తిరిగి 0;
	}

	ఒకవేళ (!strcmp(var, "rebase.forkpoint")) {
		opts->fork_బిందువు = git_config_bool(var, విలువ) ? -1 : 0;
		తిరిగి 0;
	}

	ఒకవేళ (!strcmp(var, "rebase.backend")) {
		FREE_AND_NULL(opts->default_backend);
		తిరిగి git_config_string(&opts->default_backend, var, విలువ);
	}

	తిరిగి git_default_config(var, విలువ, ctx, దత్తాంశం);
}

స్థిర పూర్ణాంకం checkout_పైకి_గమ్యం_date(నిర్మాణం rebase_options *options)
{
	నిర్మాణం strbuf buf = STRBUF_INIT;
	నిర్మాణం reset_working_వృక్షం_options ropts = { 0 };
	పూర్ణాంకం ret = 0;

	strbuf_addf(&buf, "%s: checkout %s",
		    options->reflog_action, options->switch_గమ్యం);
	ropts.oid = &options->orig_తల->object.oid;
	ropts.branch = options->తల_name;
	ropts.flags = RESET_WORKING_వృక్షం_నిరంతరం_POST_CHECKOUT_HOOK |
		      RESET_WORKING_వృక్షం_UPDATE_తల;
	ఒకవేళ (!ropts.branch)
		ropts.flags |=  RESET_WORKING_వృక్షం_DETACH;
	ropts.తల_సందేశం = buf.buf;
	ఒకవేళ (reset_working_వృక్షం(the_repository, &ropts) < 0)
		ret = error(_("could not switch to %s"), options->switch_గమ్యం);
	strbuf_release(&buf);

	తిరిగి ret;
}

/*
 * Determines whether the commits in from..to are linear, i.e. contain
 * no merge commits. This function *expects* `from` to be an ancestor of
 * `to`.
 */
స్థిర పూర్ణాంకం is_రేఖీయ_history(నిర్మాణం commit *from, నిర్మాణం commit *గమ్యం)
{
	అయితే (గమ్యం && గమ్యం != from) {
		repo_parse_commit(the_repository, గమ్యం);
		ఒకవేళ (!గమ్యం->parents)
			తిరిగి 1;
		ఒకవేళ (గమ్యం->parents->తదుపరి)
			తిరిగి 0;
		గమ్యం = గమ్యం->parents->అంశం;
	}
	తిరిగి 1;
}

స్థిర పూర్ణాంకం can_fast_forward(నిర్మాణం commit *onto, నిర్మాణం commit *upstream,
			    నిర్మాణం commit *restrict_revision,
			    నిర్మాణం commit *తల, నిర్మాణం object_id *branch_పునాది)
{
	నిర్మాణం commit_list *విలీనం_bases = ఖాళీ;
	పూర్ణాంకం res = 0;

	ఒకవేళ (is_null_oid(branch_పునాది))
		వెళ్ళు పూర్తి; /* fill_branch_base() found multiple merge bases */

	ఒకవేళ (!oideq(branch_పునాది, &onto->object.oid))
		వెళ్ళు పూర్తి;

	ఒకవేళ (restrict_revision && !oideq(&restrict_revision->object.oid, branch_పునాది))
		వెళ్ళు పూర్తి;

	ఒకవేళ (!upstream)
		వెళ్ళు పూర్తి;

	ఒకవేళ (repo_get_విలీనం_bases(the_repository, upstream, తల, &విలీనం_bases) < 0)
		నిష్క్రమించు(128);
	ఒకవేళ (!విలీనం_bases || విలీనం_bases->తదుపరి)
		వెళ్ళు పూర్తి;

	ఒకవేళ (!oideq(&onto->object.oid, &విలీనం_bases->అంశం->object.oid))
		వెళ్ళు పూర్తి;

	res = 1;

పూర్తి:
	commit_list_free(విలీనం_bases);
	తిరిగి res && is_రేఖీయ_history(onto, తల);
}

స్థిర శూన్య fill_branch_పునాది(నిర్మాణం rebase_options *options,
			    నిర్మాణం object_id *branch_పునాది)
{
	నిర్మాణం commit_list *విలీనం_bases = ఖాళీ;

	ఒకవేళ (repo_get_విలీనం_bases(the_repository, options->onto,
				 options->orig_తల, &విలీనం_bases) < 0)
		నిష్క్రమించు(128);
	ఒకవేళ (!విలీనం_bases || విలీనం_bases->తదుపరి)
		oidcpy(branch_పునాది, null_oid(the_hash_algo));
	లేకపోతే
		oidcpy(branch_పునాది, &విలీనం_bases->అంశం->object.oid);

	commit_list_free(విలీనం_bases);
}

స్థిర పూర్ణాంకం parse_opt_am(స్థిరరాశి నిర్మాణం option *opt, స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	నిర్మాణం rebase_options *opts = opt->విలువ;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	ఒకవేళ (opts->type != REBASE_UNSPECIFIED && opts->type != REBASE_APPLY)
	    die(_("apply options and merge options cannot be used together"));

	opts->type = REBASE_APPLY;

	తిరిగి 0;
}

/* -i followed by -m is still -i */
స్థిర పూర్ణాంకం parse_opt_విలీనం(స్థిరరాశి నిర్మాణం option *opt, స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	నిర్మాణం rebase_options *opts = opt->విలువ;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	ఒకవేళ (opts->type != REBASE_UNSPECIFIED && opts->type != REBASE_విలీనం)
	    die(_("apply options and merge options cannot be used together"));

	opts->type = REBASE_విలీనం;

	తిరిగి 0;
}

/* -i followed by -r is still explicitly interactive, but -r alone is not */
స్థిర పూర్ణాంకం parse_opt_interactive(స్థిరరాశి నిర్మాణం option *opt, స్థిరరాశి అక్షరం *arg,
				 పూర్ణాంకం unset)
{
	నిర్మాణం rebase_options *opts = opt->విలువ;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	ఒకవేళ (opts->type != REBASE_UNSPECIFIED && opts->type != REBASE_విలీనం)
	    die(_("apply options and merge options cannot be used together"));

	opts->type = REBASE_విలీనం;
	opts->flags |= REBASE_INTERACTIVE_EXPLICIT;

	తిరిగి 0;
}

స్థిర గణన ఖాళీలు_type parse_ఖాళీలు_విలువ(స్థిరరాశి అక్షరం *విలువ)
{
	ఒకవేళ (!strcasecmp(విలువ, "drop"))
		తిరిగి ఖాళీలు_DROP;
	లేకపోతే ఒకవేళ (!strcasecmp(విలువ, "keep"))
		తిరిగి ఖాళీలు_KEEP;
	లేకపోతే ఒకవేళ (!strcasecmp(విలువ, "stop"))
		తిరిగి ఖాళీలు_STOP;
	లేకపోతే ఒకవేళ (!strcasecmp(విలువ, "ask")) {
		warning(_("--empty=ask is deprecated; use '--empty=stop' instead."));
		తిరిగి ఖాళీలు_STOP;
	}

	die(_("unrecognized empty type '%s'; valid values are \"drop\", \"keep\", and \"stop\"."), విలువ);
}

స్థిర పూర్ణాంకం parse_opt_keep_ఖాళీలు(స్థిరరాశి నిర్మాణం option *opt, స్థిరరాశి అక్షరం *arg,
				పూర్ణాంకం unset)
{
	నిర్మాణం rebase_options *opts = opt->విలువ;

	BUG_ON_OPT_ARG(arg);

	imply_విలీనం(opts, unset ? "--no-keep-empty" : "--keep-empty");
	opts->keep_ఖాళీలు = !unset;
	తిరిగి 0;
}

స్థిర పూర్ణాంకం parse_opt_ఖాళీలు(స్థిరరాశి నిర్మాణం option *opt, స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	నిర్మాణం rebase_options *options = opt->విలువ;
	గణన ఖాళీలు_type విలువ = parse_ఖాళీలు_విలువ(arg);

	BUG_ON_OPT_NEG(unset);

	options->ఖాళీలు = విలువ;
	తిరిగి 0;
}

స్థిర పూర్ణాంకం parse_opt_rebase_merges(స్థిరరాశి నిర్మాణం option *opt, స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	నిర్మాణం rebase_options *options = opt->విలువ;

	options->rebase_merges = !unset;
	options->rebase_cousins = 0;

	ఒకవేళ (arg) {
		ఒకవేళ (!*arg) {
			warning(_("--rebase-merges with an empty string "
				  "argument is deprecated and will stop "
				  "working in a future version of Git. Use "
				  "--rebase-merges without an argument "
				  "instead, which does the same thing."));
			తిరిగి 0;
		}
		parse_rebase_merges_విలువ(options, arg);
	}

	తిరిగి 0;
}

స్థిర శూన్య NORETURN error_on_missing_default_upstream(శూన్య)
{
	నిర్మాణం branch *current_branch = branch_get(ఖాళీ);

	రాయి(_("%s\n"
		 "Please specify which branch you want to rebase against.\n"
		 "See git-rebase(1) for details.\n"
		 "\n"
		 "    git rebase '<branch>'\n"
		 "\n"),
		current_branch ? _("There is no tracking information for "
			"the current branch.") :
			_("You are not currently on a branch."));

	ఒకవేళ (current_branch) {
		స్థిరరాశి అక్షరం *remote = current_branch->remote_name;

		ఒకవేళ (!remote)
			remote = _("<remote>");

		రాయి(_("If you wish to set tracking information for this "
			 "branch you can do so with:\n"
			 "\n"
			 "    git branch --set-upstream-to=%s/<branch> %s\n"
			 "\n"),
		       remote, current_branch->name);
	}
	నిష్క్రమించు(1);
}

స్థిర పూర్ణాంకం check_exec_cmd(స్థిరరాశి అక్షరం *cmd)
{
	ఒకవేళ (strchr(cmd, '\n'))
		తిరిగి error(_("exec commands cannot contain newlines"));

	/* Does the command consist purely of whitespace? */
	ఒకవేళ (!cmd[strspn(cmd, " \t\r\f\v")])
		తిరిగి error(_("empty exec command"));

	తిరిగి 0;
}

పూర్ణాంకం cmd_rebase(పూర్ణాంకం argc,
	       స్థిరరాశి అక్షరం **argv,
	       స్థిరరాశి అక్షరం *ఉపసర్గ,
	       నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం rebase_options options = REBASE_OPTIONS_INIT;
	స్థిరరాశి అక్షరం *branch_name;
	స్థిరరాశి అక్షరం *strategy_opt = ఖాళీ;
	పూర్ణాంకం ret, flags, మొత్తం_argc, లోపల_పురోగతి = 0;
	పూర్ణాంకం keep_పునాది = 0;
	పూర్ణాంకం ok_గమ్యం_skip_pre_rebase = 0;
	నిర్మాణం strbuf సందేశం = STRBUF_INIT;
	నిర్మాణం strbuf revisions = STRBUF_INIT;
	నిర్మాణం strbuf buf = STRBUF_INIT;
	నిర్మాణం object_id branch_పునాది;
	పూర్ణాంకం ignore_whitespace = 0;
	స్థిరరాశి అక్షరం *gpg_sign = ఖాళీ;
	నిర్మాణం object_id squash_onto;
	అక్షరం *squash_onto_name = ఖాళీ;
	అక్షరం *keep_పునాది_onto_name = ఖాళీ;
	పూర్ణాంకం reschedule_failed_exec = -1;
	పూర్ణాంకం allow_preemptive_ff = 1;
	పూర్ణాంకం preserve_merges_selected = 0;
	నిర్మాణం reset_working_వృక్షం_options ropts = { 0 };
	నిర్మాణం option builtin_rebase_options[] = {
		OPT_STRING(0, "onto", &options.onto_name,
			   N_("revision"),
			   N_("rebase onto given branch instead of upstream")),
		OPT_BOOL(0, "keep-base", &keep_పునాది,
			 N_("use the merge-base of upstream and branch as the current base")),
		OPT_BOOL(0, "no-verify", &ok_గమ్యం_skip_pre_rebase,
			 N_("allow pre-rebase hook to run")),
		OPT_NEGBIT('q', "quiet", &options.flags,
			   N_("be quiet. implies --no-stat"),
			   REBASE_NO_QUIET | REBASE_VERBOSE | REBASE_DIFFSTAT),
		OPT_బిట్టు('v', "verbose", &options.flags,
			N_("display a diffstat of what changed upstream"),
			REBASE_NO_QUIET | REBASE_VERBOSE | REBASE_DIFFSTAT),
		{
			.type = OPTION_NEGBIT,
			.short_name = 'n',
			.long_name = "no-stat",
			.విలువ = &options.flags,
			.precision = పరిమాణం(options.flags),
			.help = N_("do not show diffstat of what changed upstream"),
			.flags = PARSE_OPT_NOARG,
			.defval = REBASE_DIFFSTAT,
		},
		OPT_STRVEC(0, "trailer", &options.trailer_వాదనలు, N_("trailer"),
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
		OPT_PASSTHRU_ARGV('C', ఖాళీ, &options.git_am_opts, N_("n"),
				  N_("passed to 'git apply'"), 0),
		OPT_BOOL(0, "ignore-whitespace", &ignore_whitespace,
			 N_("ignore changes in whitespace")),
		OPT_PASSTHRU_ARGV(0, "whitespace", &options.git_am_opts,
				  N_("action"), N_("passed to 'git apply'"), 0),
		OPT_బిట్టు('f', "force-rebase", &options.flags,
			N_("cherry-pick all commits, even if unchanged"),
			REBASE_FORCE),
		OPT_బిట్టు(0, "no-ff", &options.flags,
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
		OPT_CALLBACK_F(0, "apply", &options, ఖాళీ,
			N_("use apply strategies to rebase"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			parse_opt_am),
		OPT_CALLBACK_F('m', "merge", &options, ఖాళీ,
			N_("use merging strategies to rebase"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			parse_opt_విలీనం),
		OPT_CALLBACK_F('i', "interactive", &options, ఖాళీ,
			N_("let the user edit the list of commits to rebase"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			parse_opt_interactive),
		OPT_సమితి_INT_F('p', "preserve-merges", &preserve_merges_selected,
			      N_("(REMOVED) was: try to recreate merges "
				 "instead of ignoring them"),
			      1, PARSE_OPT_HIDDEN),
		OPT_RERERE_AUTOUPDATE(&options.allow_rerere_autoupdate),
		OPT_CALLBACK_F(0, "empty", &options, "(drop|keep|stop)",
			       N_("how to handle commits that become empty"),
			       PARSE_OPT_NONEG, parse_opt_ఖాళీలు),
		OPT_CALLBACK_F('k', "keep-empty", &options, ఖాళీ,
			N_("keep commits which start empty"),
			PARSE_OPT_NOARG | PARSE_OPT_HIDDEN,
			parse_opt_keep_ఖాళీలు),
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
			.విలువ = &gpg_sign,
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
			   &options.allow_ఖాళీలు_సందేశం,
			   N_("allow rebasing commits with empty messages"),
			   PARSE_OPT_HIDDEN),
		OPT_CALLBACK_F('r', "rebase-merges", &options, N_("mode"),
			N_("try to rebase merges instead of skipping them"),
			PARSE_OPT_OPTARG, parse_opt_rebase_merges),
		OPT_BOOL(0, "fork-point", &options.fork_బిందువు,
			 N_("use 'merge-base --fork-point' to refine upstream")),
		OPT_STRING('s', "strategy", &strategy_opt,
			   N_("strategy"), N_("use the given merge strategy")),
		OPT_STRING_LIST('X', "strategy-option", &options.strategy_opts,
				N_("option"),
				N_("pass the argument through to the merge "
				   "strategy")),
		OPT_BOOL(0, "root", &options.మూలం,
			 N_("rebase all reachable commits up to the root(s)")),
		OPT_BOOL(0, "reschedule-failed-exec",
			 &reschedule_failed_exec,
			 N_("automatically re-schedule any `exec` that fails")),
		OPT_BOOL(0, "reapply-cherry-picks", &options.reapply_cherry_picks,
			 N_("apply all changes, even those already present upstream")),
		OPT_చివర(),
	};
	పూర్ణాంకం i;

	show_usage_with_options_if_asked(argc, argv,
					 builtin_rebase_usage,
					 builtin_rebase_options);

#నిర్వచితం_కాకపోతే WITH_BREAKING_CHANGES
	warn_on_auto_comment_char = true;
#అయితే_ముగింపు /* !WITH_BREAKING_CHANGES */
	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_నిండిన_సూచీ = 0;

	repo_config(the_repository, rebase_config, &options);
	/* options.gpg_sign_opt will be either "-S" or NULL */
	gpg_sign = options.gpg_sign_opt ? "" : ఖాళీ;
	FREE_AND_NULL(options.gpg_sign_opt);

	strbuf_reset(&buf);
	strbuf_addf(&buf, "%s/applying", apply_dir());
	ఒకవేళ(file_exists(buf.buf))
		die(_("It looks like 'git am' is in progress. Cannot rebase."));

	ఒకవేళ (is_directory(apply_dir())) {
		options.type = REBASE_APPLY;
		options.state_dir = apply_dir();
	} లేకపోతే ఒకవేళ (is_directory(విలీనం_dir())) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "%s/rewritten", విలీనం_dir());
		ఒకవేళ (!(options.action == ACTION_ABORT) && is_directory(buf.buf)) {
			die(_("`rebase --preserve-merges` (-p) is no longer supported.\n"
			"Use `git rebase --abort` to terminate current rebase.\n"
			"Or downgrade to v2.33, or earlier, to complete the rebase."));
		} లేకపోతే {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "%s/interactive", విలీనం_dir());
			options.type = REBASE_విలీనం;
			ఒకవేళ (file_exists(buf.buf))
				options.flags |= REBASE_INTERACTIVE_EXPLICIT;
		}
		options.state_dir = విలీనం_dir();
	}

	ఒకవేళ (options.type != REBASE_UNSPECIFIED)
		లోపల_పురోగతి = 1;

	మొత్తం_argc = argc;
	argc = parse_options(argc, argv, ఉపసర్గ,
			     builtin_rebase_options,
			     builtin_rebase_usage, 0);

	ఒకవేళ (options.trailer_వాదనలు.nr) {
		ఒకవేళ (validate_trailer_వాదనలు(&options.trailer_వాదనలు))
			die(ఖాళీ);
		options.flags |= REBASE_FORCE;
	}

	ఒకవేళ (preserve_merges_selected)
		die(_("--preserve-merges was replaced by --rebase-merges\n"
			"Note: Your `pull.rebase` configuration may also be set to 'preserve',\n"
			"which is no longer supported; use 'merges' instead"));

	ఒకవేళ (options.action != ACTION_NONE && మొత్తం_argc != 2) {
		usage_with_options(builtin_rebase_usage,
				   builtin_rebase_options);
	}

	ఒకవేళ (argc > 2)
		usage_with_options(builtin_rebase_usage,
				   builtin_rebase_options);

	ఒకవేళ (keep_పునాది) {
		ఒకవేళ (options.onto_name)
			die(_("options '%s' and '%s' cannot be used together"), "--keep-base", "--onto");
		ఒకవేళ (options.మూలం)
			die(_("options '%s' and '%s' cannot be used together"), "--keep-base", "--root");
		/*
		 * --keep-base defaults to --no-fork-point to keep the
		 * base the same.
		 */
		ఒకవేళ (options.fork_బిందువు < 0)
			options.fork_బిందువు = 0;
	}
	ఒకవేళ (options.మూలం && options.fork_బిందువు > 0)
		die(_("options '%s' and '%s' cannot be used together"), "--root", "--fork-point");

	ఒకవేళ (options.action != ACTION_NONE && !లోపల_పురోగతి)
		die(_("no rebase in progress"));

	ఒకవేళ (options.action == ACTION_EDIT_TODO && !is_విలీనం(&options))
		die(_("The --edit-todo action can only be used during "
		      "interactive rebase."));

	ఒకవేళ (trace2_is_enabled()) {
		ఒకవేళ (is_విలీనం(&options))
			trace2_cmd_బహుళకం("interactive");
		లేకపోతే ఒకవేళ (options.exec.nr)
			trace2_cmd_బహుళకం("interactive-exec");
		లేకపోతే
			trace2_cmd_బహుళకం(action_పేర్లు[options.action]);
	}

	options.reflog_action = getenv(GIT_REFLOG_ACTION_ENVIRONMENT);
	options.reflog_action =
		xstrdup(options.reflog_action ? options.reflog_action : "rebase");

	ఎంపిక (options.action) {
	స్థితి ACTION_CONTINUE: {
		నిర్మాణం object_id తల;
		నిర్మాణం తాళం_file తాళం_file = తాళం_INIT;
		పూర్ణాంకం fd;

		/* Sanity check */
		ఒకవేళ (repo_get_oid(the_repository, "HEAD", &తల))
			die(_("Cannot read HEAD"));

		fd = repo_hold_locked_సూచీ(the_repository, &తాళం_file, 0);
		ఒకవేళ (repo_read_సూచీ(the_repository) < 0)
			die(_("could not read index"));
		refresh_సూచీ(the_repository->సూచీ, REFRESH_QUIET, ఖాళీ, ఖాళీ,
			      ఖాళీ);
		ఒకవేళ (0 <= fd)
			repo_update_సూచీ_if_able(the_repository, &తాళం_file);
		rollback_తాళం_file(&తాళం_file);

		ఒకవేళ (has_unstaged_changes(the_repository, 1)) {
			puts(_("You must edit all merge conflicts and then\n"
			       "mark them as resolved using git add"));
			నిష్క్రమించు(1);
		}
		ఒకవేళ (read_basic_state(&options))
			నిష్క్రమించు(1);
		వెళ్ళు నిరంతరం_rebase;
	}
	స్థితి ACTION_SKIP: {
		నిర్మాణం string_list విలీనం_rr = STRING_LIST_INIT_DUP;

		rerere_clear(the_repository, &విలీనం_rr);
		string_list_clear(&విలీనం_rr, 1);
		ropts.flags = RESET_WORKING_వృక్షం_HARD |
			      RESET_WORKING_వృక్షం_UPDATE_తల;
		ఒకవేళ (reset_working_వృక్షం(the_repository, &ropts) < 0)
			die(_("could not discard worktree changes"));
		remove_branch_state(the_repository, 0);
		ఒకవేళ (read_basic_state(&options))
			నిష్క్రమించు(1);
		వెళ్ళు నిరంతరం_rebase;
	}
	స్థితి ACTION_ABORT: {
		నిర్మాణం string_list విలీనం_rr = STRING_LIST_INIT_DUP;
		నిర్మాణం strbuf తల_సందేశం = STRBUF_INIT;

		rerere_clear(the_repository, &విలీనం_rr);
		string_list_clear(&విలీనం_rr, 1);

		ఒకవేళ (read_basic_state(&options))
			నిష్క్రమించు(1);

		strbuf_addf(&తల_సందేశం, "%s (abort): returning to %s",
			    options.reflog_action,
			    options.తల_name ? options.తల_name
					      : oid_గమ్యం_hex(&options.orig_తల->object.oid));
		ropts.oid = &options.orig_తల->object.oid;
		ropts.తల_సందేశం = తల_సందేశం.buf;
		ropts.branch = options.తల_name;
		ropts.flags = RESET_WORKING_వృక్షం_HARD |
			      RESET_WORKING_వృక్షం_UPDATE_తల;
		ఒకవేళ (reset_working_వృక్షం(the_repository, &ropts) < 0)
			die(_("could not move back to %s"),
			    oid_గమ్యం_hex(&options.orig_తల->object.oid));
		strbuf_release(&తల_సందేశం);
		remove_branch_state(the_repository, 0);
		ret = ముగింపు_rebase(&options);
		వెళ్ళు cleanup;
	}
	స్థితి ACTION_QUIT: {
		save_autostash(state_dir_మార్గం("autostash", &options));
		ఒకవేళ (options.type == REBASE_విలీనం) {
			నిర్మాణం replay_opts replay = REPLAY_OPTS_INIT;

			replay.action = REPLAY_INTERACTIVE_REBASE;
			ret = sequencer_remove_state(&replay);
			replay_opts_release(&replay);
		} లేకపోతే {
			strbuf_reset(&buf);
			strbuf_addstr(&buf, options.state_dir);
			ret = remove_dir_recursively(&buf, 0);
			ఒకవేళ (ret)
				error(_("could not remove '%s'"),
				       options.state_dir);
		}
		వెళ్ళు cleanup;
	}
	స్థితి ACTION_EDIT_TODO:
		options.dont_ముగింపు_rebase = 1;
		వెళ్ళు నిరంతరం_rebase;
	స్థితి ACTION_SHOW_CURRENT_PATCH:
		options.dont_ముగింపు_rebase = 1;
		వెళ్ళు నిరంతరం_rebase;
	స్థితి ACTION_NONE:
		ఆపు;
	అప్రమేయ:
		BUG("action: %d", options.action);
	}

	/* Make sure no rebase is in progress */
	ఒకవేళ (లోపల_పురోగతి) {
		స్థిరరాశి అక్షరం *చివరి_slash = strrchr(options.state_dir, '/');
		స్థిరరాశి అక్షరం *state_dir_పునాది =
			చివరి_slash ? చివరి_slash + 1 : options.state_dir;
		స్థిరరాశి అక్షరం *cmd_సజీవ_rebase =
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
		    state_dir_పునాది, cmd_సజీవ_rebase, buf.buf);
	}

	ఒకవేళ ((options.flags & REBASE_INTERACTIVE_EXPLICIT) ||
	    (options.action != ACTION_NONE) ||
	    (options.exec.nr > 0) ||
	    options.autosquash == 1) {
		allow_preemptive_ff = 0;
	}
	ఒకవేళ (options.committer_date_is_author_date || options.ignore_date)
		options.flags |= REBASE_FORCE;

	కొరకు (i = 0; i < options.git_am_opts.nr; i++) {
		స్థిరరాశి అక్షరం *option = options.git_am_opts.v[i], *p;
		ఒకవేళ (!strcmp(option, "--whitespace=fix") ||
		    !strcmp(option, "--whitespace=strip"))
			allow_preemptive_ff = 0;
		లేకపోతే ఒకవేళ (skip_ఉపసర్గ(option, "-C", &p)) {
			అయితే (*p)
				ఒకవేళ (!అంకెనా(*(p++)))
					die(_("switch `C' expects a "
					      "numerical value"));
		} లేకపోతే ఒకవేళ (skip_ఉపసర్గ(option, "--whitespace=", &p)) {
			ఒకవేళ (*p && strcmp(p, "warn") && strcmp(p, "nowarn") &&
			    strcmp(p, "error") && strcmp(p, "error-all"))
				die("Invalid whitespace option: '%s'", p);
		}
	}

	కొరకు (i = 0; i < options.exec.nr; i++)
		ఒకవేళ (check_exec_cmd(options.exec.వస్తువులు[i].string))
			నిష్క్రమించు(1);

	ఒకవేళ (!(options.flags & REBASE_NO_QUIET))
		strvec_నెట్టు(&options.git_am_opts, "-q");

	ఒకవేళ (options.ఖాళీలు != ఖాళీలు_UNSPECIFIED)
		imply_విలీనం(&options, "--empty");

	ఒకవేళ (options.reapply_cherry_picks < 0)
		/*
		 * We default to --no-reapply-cherry-picks unless
		 * --keep-base is given; when --keep-base is given, we want
		 * to default to --reapply-cherry-picks.
		 */
		options.reapply_cherry_picks = keep_పునాది;
	లేకపోతే ఒకవేళ (!keep_పునాది)
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
		imply_విలీనం(&options, options.reapply_cherry_picks ?
					  "--reapply-cherry-picks" :
					  "--no-reapply-cherry-picks");

	ఒకవేళ (gpg_sign)
		options.gpg_sign_opt = xstrfmt("-S%s", gpg_sign);

	ఒకవేళ (options.exec.nr)
		imply_విలీనం(&options, "--exec");

	ఒకవేళ (options.type == REBASE_APPLY) {
		ఒకవేళ (ignore_whitespace)
			strvec_నెట్టు(&options.git_am_opts,
				    "--ignore-whitespace");
		ఒకవేళ (options.committer_date_is_author_date)
			strvec_నెట్టు(&options.git_am_opts,
				    "--committer-date-is-author-date");
		ఒకవేళ (options.ignore_date)
			strvec_నెట్టు(&options.git_am_opts, "--ignore-date");
	} లేకపోతే {
		/* REBASE_MERGE */
		ఒకవేళ (ignore_whitespace) {
			string_list_append(&options.strategy_opts,
					   "ignore-space-change");
		}
	}

	ఒకవేళ (strategy_opt)
		options.strategy = xstrdup(strategy_opt);
	లేకపోతే ఒకవేళ (options.strategy_opts.nr && !options.strategy)
		options.strategy = xstrdup("ort");
	ఒకవేళ (options.strategy)
		imply_విలీనం(&options, "--strategy");

	ఒకవేళ (options.మూలం && !options.onto_name)
		imply_విలీనం(&options, "--root without --onto");

	ఒకవేళ (options.trailer_వాదనలు.nr)
		imply_విలీనం(&options, "--trailer");

	ఒకవేళ (isatty(2) && options.flags & REBASE_NO_QUIET)
		strbuf_addstr(&options.git_format_patch_opt, " --progress");

	ఒకవేళ (options.git_am_opts.nr || options.type == REBASE_APPLY) {
		/* all am options except -q are compatible only with --apply */
		కొరకు (i = options.git_am_opts.nr - 1; i >= 0; i--)
			ఒకవేళ (strcmp(options.git_am_opts.v[i], "-q"))
				ఆపు;

		ఒకవేళ (i >= 0 || options.type == REBASE_APPLY) {
			ఒకవేళ (is_విలీనం(&options))
				die(_("apply options and merge options "
					  "cannot be used together"));
			లేకపోతే ఒకవేళ (options.rebase_merges == -1 && options.config_rebase_merges == 1)
				die(_("apply options are incompatible with rebase.rebaseMerges.  Consider adding --no-rebase-merges"));
			లేకపోతే ఒకవేళ (options.update_refs == -1 && options.config_update_refs == 1)
				die(_("apply options are incompatible with rebase.updateRefs.  Consider adding --no-update-refs"));
			లేకపోతే
				options.type = REBASE_APPLY;
		}
	}

	ఒకవేళ (options.update_refs == 1)
		imply_విలీనం(&options, "--update-refs");
	options.update_refs = (options.update_refs >= 0) ? options.update_refs :
			     ((options.config_update_refs >= 0) ? options.config_update_refs : 0);

	ఒకవేళ (options.rebase_merges == 1)
		imply_విలీనం(&options, "--rebase-merges");
	options.rebase_merges = (options.rebase_merges >= 0) ? options.rebase_merges :
				((options.config_rebase_merges >= 0) ? options.config_rebase_merges : 0);

	ఒకవేళ (options.autosquash == 1) {
		imply_విలీనం(&options, "--autosquash");
	} లేకపోతే ఒకవేళ (options.autosquash == -1) {
		options.autosquash =
			options.config_autosquash &&
			(options.flags & REBASE_INTERACTIVE_EXPLICIT);
	}

	ఒకవేళ (options.type == REBASE_UNSPECIFIED) {
		ఒకవేళ (!strcmp(options.default_backend, "merge"))
			options.type = REBASE_విలీనం;
		లేకపోతే ఒకవేళ (!strcmp(options.default_backend, "apply"))
			options.type = REBASE_APPLY;
		లేకపోతే
			die(_("Unknown rebase backend: %s"),
			    options.default_backend);
	}

	ఎంపిక (options.type) {
	స్థితి REBASE_విలీనం:
		options.state_dir = విలీనం_dir();
		ఆపు;
	స్థితి REBASE_APPLY:
		options.state_dir = apply_dir();
		ఆపు;
	అప్రమేయ:
		BUG("options.type was just set above; should be unreachable.");
	}

	ఒకవేళ (options.ఖాళీలు == ఖాళీలు_UNSPECIFIED) {
		ఒకవేళ (options.flags & REBASE_INTERACTIVE_EXPLICIT)
			options.ఖాళీలు = ఖాళీలు_STOP;
		లేకపోతే ఒకవేళ (options.exec.nr > 0)
			options.ఖాళీలు = ఖాళీలు_KEEP;
		లేకపోతే
			options.ఖాళీలు = ఖాళీలు_DROP;
	}
	ఒకవేళ (reschedule_failed_exec > 0 && !is_విలీనం(&options))
		die(_("--reschedule-failed-exec requires "
		      "--exec or --interactive"));
	ఒకవేళ (reschedule_failed_exec >= 0)
		options.reschedule_failed_exec = reschedule_failed_exec;

	ఒకవేళ (options.signoff) {
		strvec_నెట్టు(&options.git_am_opts, "--signoff");
		options.flags |= REBASE_FORCE;
	}

	ఒకవేళ (!options.మూలం) {
		ఒకవేళ (argc < 1) {
			నిర్మాణం branch *branch;

			branch = branch_get(ఖాళీ);
			options.upstream_name = branch_get_upstream(branch,
								    ఖాళీ);
			ఒకవేళ (!options.upstream_name)
				error_on_missing_default_upstream();
			ఒకవేళ (options.fork_బిందువు < 0)
				options.fork_బిందువు = 1;
		} లేకపోతే {
			options.upstream_name = argv[0];
			argc--;
			argv++;
			ఒకవేళ (!strcmp(options.upstream_name, "-"))
				options.upstream_name = "@{-1}";
		}
		options.upstream =
			lookup_commit_reference_by_name(options.upstream_name);
		ఒకవేళ (!options.upstream)
			die(_("invalid upstream '%s'"), options.upstream_name);
		options.upstream_arg = options.upstream_name;
	} లేకపోతే {
		ఒకవేళ (!options.onto_name) {
			ఒకవేళ (commit_వృక్షం("", 0, the_hash_algo->ఖాళీలు_వృక్షం, ఖాళీ,
					&squash_onto, ఖాళీ, ఖాళీ) < 0)
				die(_("Could not create new root commit"));
			options.squash_onto = &squash_onto;
			options.onto_name = squash_onto_name =
				xstrdup(oid_గమ్యం_hex(&squash_onto));
		} లేకపోతే
			options.మూలం_with_onto = 1;

		options.upstream_name = ఖాళీ;
		options.upstream = ఖాళీ;
		ఒకవేళ (argc > 1)
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
	ఒకవేళ (argc == 1) {
		/* Is it "rebase other branchname" or "rebase other commit"? */
		నిర్మాణం object_id branch_oid;
		branch_name = argv[0];
		options.switch_గమ్యం = argv[0];

		/* Is it a local branch? */
		strbuf_reset(&buf);
		strbuf_addf(&buf, "refs/heads/%s", branch_name);
		ఒకవేళ (!refs_read_ref(get_main_ref_store(the_repository), buf.buf, &branch_oid)) {
			die_if_checked_బహిర్గతం(buf.buf, 1);
			options.తల_name = xstrdup(buf.buf);
			options.orig_తల =
				lookup_commit_object(the_repository,
						     &branch_oid);
		/* If not is it a valid ref (branch or commit)? */
		} లేకపోతే {
			options.orig_తల =
				lookup_commit_reference_by_name(branch_name);
			options.తల_name = ఖాళీ;
		}
		ఒకవేళ (!options.orig_తల)
			die(_("no such branch/commit '%s'"), branch_name);
	} లేకపోతే ఒకవేళ (argc == 0) {
		/* Do not need to switch branches, we are already on it. */
		options.తల_name =
			xstrdup_or_null(refs_resolve_ref_unsafe(get_main_ref_store(the_repository), "HEAD", 0, ఖాళీ,
								&flags));
		ఒకవేళ (!options.తల_name)
			die(_("No such ref: %s"), "HEAD");
		ఒకవేళ (flags & REF_ISSYMREF) {
			ఒకవేళ (!skip_ఉపసర్గ(options.తల_name,
					 "refs/heads/", &branch_name))
				branch_name = options.తల_name;

		} లేకపోతే {
			FREE_AND_NULL(options.తల_name);
			branch_name = "HEAD";
		}
		options.orig_తల = lookup_commit_reference_by_name("HEAD");
		ఒకవేళ (!options.orig_తల)
			die(_("Could not resolve HEAD to a commit"));
	} లేకపోతే
		BUG("unexpected number of arguments left to parse");

	/* Make sure the branch to rebase onto is valid. */
	ఒకవేళ (keep_పునాది) {
		strbuf_reset(&buf);
		strbuf_addstr(&buf, options.upstream_name);
		strbuf_addstr(&buf, "...");
		strbuf_addstr(&buf, branch_name);
		options.onto_name = keep_పునాది_onto_name = xstrdup(buf.buf);
	} లేకపోతే ఒకవేళ (!options.onto_name)
		options.onto_name = options.upstream_name;
	ఒకవేళ (అక్షరశ్రేణి_శోధన(options.onto_name, "...")) {
		ఒకవేళ (repo_get_oid_mb(the_repository, options.onto_name, &branch_పునాది) < 0) {
			ఒకవేళ (keep_పునాది)
				die(_("'%s': need exactly one merge base with branch"),
				    options.upstream_name);
			లేకపోతే
				die(_("'%s': need exactly one merge base"),
				    options.onto_name);
		}
		options.onto = lookup_commit_or_die(&branch_పునాది,
						    options.onto_name);
	} లేకపోతే {
		options.onto =
			lookup_commit_reference_by_name(options.onto_name);
		ఒకవేళ (!options.onto)
			die(_("Does not point to a valid commit '%s'"),
				options.onto_name);
		fill_branch_పునాది(&options, &branch_పునాది);
	}

	ఒకవేళ (keep_పునాది && options.reapply_cherry_picks)
		options.upstream = options.onto;

	ఒకవేళ (options.fork_బిందువు > 0)
		options.restrict_revision =
			get_fork_బిందువు(options.upstream_name, options.orig_తల);

	ఒకవేళ (repo_read_సూచీ(the_repository) < 0)
		die(_("could not read index"));

	ఒకవేళ (options.autostash)
		create_autostash(the_repository,
				 state_dir_మార్గం("autostash", &options));


	ఒకవేళ (require_clean_work_వృక్షం(the_repository, "rebase",
				    _("Please commit or stash them."), 1, 1)) {
		ret = -1;
		వెళ్ళు cleanup_autostash;
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
	ఒకవేళ (allow_preemptive_ff &&
	    can_fast_forward(options.onto, options.upstream, options.restrict_revision,
			     options.orig_తల, &branch_పునాది)) {
		పూర్ణాంకం flag;

		ఒకవేళ (!(options.flags & REBASE_FORCE)) {
			/* Lazily switch to the target branch if needed... */
			ఒకవేళ (options.switch_గమ్యం) {
				ret = checkout_పైకి_గమ్యం_date(&options);
				ఒకవేళ (ret)
					వెళ్ళు cleanup_autostash;
			}

			ఒకవేళ (!(options.flags & REBASE_NO_QUIET))
				; /* be quiet */
			లేకపోతే ఒకవేళ (!strcmp(branch_name, "HEAD") &&
				 refs_resolve_ref_unsafe(get_main_ref_store(the_repository), "HEAD", 0, ఖాళీ, &flag))
				puts(_("HEAD is up to date."));
			లేకపోతే
				రాయి(_("Current branch %s is up to date.\n"),
				       branch_name);
			ret = ముగింపు_rebase(&options);
			వెళ్ళు cleanup;
		} లేకపోతే ఒకవేళ (!(options.flags & REBASE_NO_QUIET))
			; /* be quiet */
		లేకపోతే ఒకవేళ (!strcmp(branch_name, "HEAD") &&
			 refs_resolve_ref_unsafe(get_main_ref_store(the_repository), "HEAD", 0, ఖాళీ, &flag))
			puts(_("HEAD is up to date, rebase forced."));
		లేకపోతే
			రాయి(_("Current branch %s is up to date, rebase "
				 "forced.\n"), branch_name);
	}

	/* If a hook exists, give it a chance to interrupt*/
	ఒకవేళ (!ok_గమ్యం_skip_pre_rebase &&
	    నిరంతరం_hooks_l(the_repository, "pre-rebase", options.upstream_arg,
			argc ? argv[0] : ఖాళీ, ఖాళీ)) {
		ret = error(_("The pre-rebase hook refused to rebase."));
		వెళ్ళు cleanup_autostash;
	}

	ఒకవేళ (options.flags & REBASE_DIFFSTAT) {
		నిర్మాణం diff_options opts;

		ఒకవేళ (options.flags & REBASE_VERBOSE) {
			ఒకవేళ (is_null_oid(&branch_పునాది))
				రాయి(_("Changes to %s:\n"),
				       oid_గమ్యం_hex(&options.onto->object.oid));
			లేకపోతే
				రాయి(_("Changes from %s to %s:\n"),
				       oid_గమ్యం_hex(&branch_పునాది),
				       oid_గమ్యం_hex(&options.onto->object.oid));
		}

		/* We want color (if set), but no pager */
		repo_diff_అమరిక(the_repository, &opts);
		init_diffstat_widths(&opts);
		opts.output_format |=
			DIFF_FORMAT_SUMMARY | DIFF_FORMAT_DIFFSTAT;
		opts.detect_rename = DIFF_DETECT_RENAME;
		diff_అమరిక_పూర్తి(&opts);
		diff_వృక్షం_oid(is_null_oid(&branch_పునాది) ?
			      the_hash_algo->ఖాళీలు_వృక్షం : &branch_పునాది,
			      &options.onto->object.oid, "", &opts);
		diffcore_std(&opts);
		diff_flush(&opts);
	}

	ఒకవేళ (is_విలీనం(&options))
		వెళ్ళు నిరంతరం_rebase;

	/* Detach HEAD and reset the tree */
	ఒకవేళ (options.flags & REBASE_NO_QUIET)
		రాయి(_("First, rewinding head to replay your work on top of "
			 "it...\n"));

	strbuf_addf(&సందేశం, "%s (start): checkout %s",
		    options.reflog_action, options.onto_name);
	ropts.oid = &options.onto->object.oid;
	ropts.orig_తల = &options.orig_తల->object.oid;
	ropts.flags = RESET_WORKING_వృక్షం_DETACH |
		      RESET_WORKING_వృక్షం_UPDATE_తల |
		      RESET_WORKING_వృక్షం_UPDATE_ORIG_తల |
		      RESET_WORKING_వృక్షం_నిరంతరం_POST_CHECKOUT_HOOK;
	ropts.తల_సందేశం = సందేశం.buf;
	ropts.default_reflog_action = options.reflog_action;
	ఒకవేళ (reset_working_వృక్షం(the_repository, &ropts)) {
		ret = error(_("Could not detach HEAD"));
		వెళ్ళు cleanup_autostash;
	}

	/*
	 * If the onto is a proper descendant of the tip of the branch, then
	 * we just fast-forwarded.
	 */
	ఒకవేళ (oideq(&branch_పునాది, &options.orig_తల->object.oid)) {
		రాయి(_("Fast-forwarded %s to %s.\n"),
			branch_name, options.onto_name);
		move_గమ్యం_original_branch(&options);
		ret = ముగింపు_rebase(&options);
		వెళ్ళు cleanup;
	}

	strbuf_addf(&revisions, "%s..%s",
		    options.మూలం ? oid_గమ్యం_hex(&options.onto->object.oid) :
		    (options.restrict_revision ?
		     oid_గమ్యం_hex(&options.restrict_revision->object.oid) :
		     oid_గమ్యం_hex(&options.upstream->object.oid)),
		    oid_గమ్యం_hex(&options.orig_తల->object.oid));

	options.revisions = revisions.buf;

నిరంతరం_rebase:
	ret = నిరంతరం_specific_rebase(&options);

cleanup:
	strbuf_release(&buf);
	strbuf_release(&సందేశం);
	strbuf_release(&revisions);
	rebase_options_release(&options);
	విడుదల(squash_onto_name);
	విడుదల(keep_పునాది_onto_name);
	తిరిగి !!ret;

cleanup_autostash:
	ret |= !!cleanup_autostash(&options);
	వెళ్ళు cleanup;
}
