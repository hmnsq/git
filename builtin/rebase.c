/*
 * "git rebase" builtin command
 *
 * Copyright (c) 2018 Pratik Karki
 */

#परिभाषय USE_THE_REPOSITORY_VARIABLE
#परिभाषय DISABLE_SIGN_COMPARE_WARNINGS

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

स्थिर GIT_मार्गः_FUNC(मार्गः_squash_onto, "rebase-merge/squash-onto")
स्थिर GIT_मार्गः_FUNC(मार्गः_interactive, "rebase-merge/interactive")
स्थिर GIT_मार्गः_FUNC(apply_dir, "rebase-apply")
स्थिर GIT_मार्गः_FUNC(merge_dir, "rebase-merge")

गणना rebase_type {
	REBASE_UNSPECIFIED = -1,
	REBASE_APPLY,
	REBASE_MERGE
};

गणना रिक्तानि_type {
	रिक्तानि_UNSPECIFIED = -1,
	रिक्तानि_DROP,
	रिक्तानि_KEEP,
	रिक्तानि_STOP
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

स्थिर ध्रुव वर्ण *action_नामानि[] = {
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
	गणना रिक्तानि_type रिक्तानि;
	वर्ण *default_backend;
	ध्रुव वर्ण *state_dir;
	संरचना commit *upstream;
	ध्रुव वर्ण *upstream_name;
	ध्रुव वर्ण *upstream_arg;
	वर्ण *शीर्ष_name;
	संरचना commit *orig_शीर्ष;
	संरचना commit *onto;
	ध्रुव वर्ण *onto_name;
	ध्रुव वर्ण *revisions;
	ध्रुव वर्ण *switch_गन्तव्यम्;
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
	संरचना strvec trailer_तर्काः;
	पूर्णाङ्क allow_rerere_autoupdate;
	पूर्णाङ्क keep_रिक्तानि;
	पूर्णाङ्क autosquash;
	वर्ण *gpg_sign_opt;
	पूर्णाङ्क autostash;
	पूर्णाङ्क committer_date_is_author_date;
	पूर्णाङ्क ignore_date;
	संरचना string_list exec;
	पूर्णाङ्क allow_रिक्तानि_सन्देशः;
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

#परिभाषय REBASE_OPTIONS_INIT {			  	\
		.type = REBASE_UNSPECIFIED,	  	\
		.रिक्तानि = रिक्तानि_UNSPECIFIED,	  	\
		.keep_रिक्तानि = 1,			\
		.default_backend = xstrdup("merge"),  	\
		.flags = REBASE_NO_QUIET, 		\
		.git_am_opts = STRVEC_INIT,		\
		.exec = STRING_LIST_INIT_NODUP,		\
		.trailer_तर्काः = STRVEC_INIT,		\
		.git_format_patch_opt = STRBUF_INIT,	\
		.fork_point = -1,			\
		.reapply_cherry_picks = -1,             \
		.allow_रिक्तानि_सन्देशः = 1,               \
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
	मुक्त(opts->शीर्ष_name);
	strvec_clear(&opts->git_am_opts);
	मुक्त(opts->gpg_sign_opt);
	string_list_clear(&opts->exec, 0);
	मुक्त(opts->strategy);
	string_list_clear(&opts->strategy_opts, 0);
	strbuf_release(&opts->git_format_patch_opt);
	strvec_clear(&opts->trailer_तर्काः);
}

स्थिर संरचना replay_opts get_replay_opts(ध्रुव संरचना rebase_options *opts)
{
	संरचना replay_opts replay = REPLAY_OPTS_INIT;

	replay.action = REPLAY_INTERACTIVE_REBASE;
	replay.strategy = रिक्त;
	sequencer_init_config(&replay);

	replay.signoff = opts->signoff;

	strvec_pushv(&replay.trailer_तर्काः, opts->trailer_तर्काः.v);

	replay.allow_ff = !(opts->flags & REBASE_FORCE);
	यदि (opts->allow_rerere_autoupdate)
		replay.allow_rerere_auto = opts->allow_rerere_autoupdate;
	replay.allow_रिक्तानि = 1;
	replay.allow_रिक्तानि_सन्देशः = opts->allow_रिक्तानि_सन्देशः;
	replay.drop_redundant_commits = (opts->रिक्तानि == रिक्तानि_DROP);
	replay.keep_redundant_commits = (opts->रिक्तानि == रिक्तानि_KEEP);
	replay.quiet = !(opts->flags & REBASE_NO_QUIET);
	replay.verbose = opts->flags & REBASE_VERBOSE;
	replay.reschedule_failed_exec = opts->reschedule_failed_exec;
	replay.committer_date_is_author_date =
					opts->committer_date_is_author_date;
	replay.ignore_date = opts->ignore_date;
	मुक्त(replay.gpg_sign);
	replay.gpg_sign = xstrdup_or_null(opts->gpg_sign_opt);
	replay.reflog_action = xstrdup(opts->reflog_action);
	यदि (opts->strategy)
		replay.strategy = xstrdup_or_null(opts->strategy);
	अन्यथा यदि (!replay.strategy && replay.default_strategy) {
		replay.strategy = replay.default_strategy;
		replay.default_strategy = रिक्त;
	}

	प्रति (आकार_t i = 0; i < opts->strategy_opts.nr; i++)
		strvec_प्रक्षिप(&replay.xopts, opts->strategy_opts.वस्तूनि[i].string);

	यदि (opts->squash_onto) {
		oidcpy(&replay.squash_onto, opts->squash_onto);
		replay.have_squash_onto = 1;
	}

	प्रत्यागम replay;
}

स्थिर पूर्णाङ्क edit_todo_file(अचिह्नित flags, संरचना replay_opts *opts)
{
	ध्रुव वर्ण *todo_file = rebase_मार्गः_todo();
	संरचना todo_list todo_list = TODO_LIST_INIT,
		new_todo = TODO_LIST_INIT;
	पूर्णाङ्क res = 0;

	यदि (strbuf_read_file(&todo_list.buf, todo_file, 0) < 0)
		प्रत्यागम error_errno(_("could not read '%s'."), todo_file);

	strbuf_stripspace(&todo_list.buf, comment_पङ्क्तिः_str);
	res = edit_todo_list(the_repository, opts, &todo_list, &new_todo,
			     रिक्त, रिक्त, flags);
	यदि (!res && todo_list_write_गन्तव्यम्_file(the_repository, &new_todo, todo_file,
					    रिक्त, रिक्त, -1, flags & ~(TODO_LIST_SHORTEN_IDS)))
		res = error_errno(_("could not write '%s'"), todo_file);

	todo_list_release(&todo_list);
	todo_list_release(&new_todo);

	प्रत्यागम res;
}

स्थिर पूर्णाङ्क get_revision_ranges(संरचना commit *upstream, संरचना commit *onto,
			       संरचना object_id *orig_शीर्ष, वर्ण **revisions,
			       वर्ण **shortrevisions)
{
	संरचना commit *आधारः_rev = upstream ? upstream : onto;
	ध्रुव वर्ण *shorthead;

	*revisions = xstrfmt("%s...%s", oid_गन्तव्यम्_hex(&आधारः_rev->object.oid),
			     oid_गन्तव्यम्_hex(orig_शीर्ष));

	shorthead = repo_find_unique_abbrev(the_repository, orig_शीर्ष,
					    DEFAULT_ABBREV);

	यदि (upstream) {
		ध्रुव वर्ण *shortrev;

		shortrev = repo_find_unique_abbrev(the_repository,
						   &आधारः_rev->object.oid,
						   DEFAULT_ABBREV);

		*shortrevisions = xstrfmt("%s..%s", shortrev, shorthead);
	} अन्यथा
		*shortrevisions = xstrdup(shorthead);

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क init_basic_state(संरचना replay_opts *opts, ध्रुव वर्ण *शीर्ष_name,
			    संरचना commit *onto,
			    ध्रुव संरचना object_id *orig_शीर्ष)
{
	FILE *interactive;

	यदि (!is_directory(merge_dir()) &&
	    safe_create_dir_अन्तर्_gitdir(the_repository, merge_dir()))
		प्रत्यागम error_errno(_("could not create temporary %s"), merge_dir());

	refs_delete_reflog(get_main_ref_store(the_repository), "REBASE_HEAD");

	interactive = fopen(मार्गः_interactive(), "w");
	यदि (!interactive)
		प्रत्यागम error_errno(_("could not mark as interactive"));
	fclose(interactive);

	प्रत्यागम write_basic_state(opts, शीर्ष_name, onto, orig_शीर्ष);
}

स्थिर पूर्णाङ्क do_interactive_rebase(संरचना rebase_options *opts, अचिह्नित flags)
{
	पूर्णाङ्क ret = -1;
	वर्ण *revisions = रिक्त, *shortrevisions = रिक्त;
	संरचना strvec make_script_तर्काः = STRVEC_INIT;
	संरचना todo_list todo_list = TODO_LIST_INIT;
	संरचना replay_opts replay = get_replay_opts(opts);

	यदि (get_revision_ranges(opts->upstream, opts->onto, &opts->orig_शीर्ष->object.oid,
				&revisions, &shortrevisions))
		गच्छ cleanup;

	strvec_pushl(&make_script_तर्काः, "", revisions, रिक्त);
	यदि (opts->restrict_revision)
		strvec_pushf(&make_script_तर्काः, "^%s",
			     oid_गन्तव्यम्_hex(&opts->restrict_revision->object.oid));

	ret = sequencer_make_script(the_repository, &todo_list.buf,
				    &make_script_तर्काः, flags);
	यदि (ret) {
		error(_("could not generate todo list"));
		गच्छ cleanup;
	}

	यदि (init_basic_state(&replay,
			     opts->शीर्ष_name ? opts->शीर्ष_name : "detached HEAD",
			     opts->onto, &opts->orig_शीर्ष->object.oid))
		गच्छ cleanup;

	यदि (!opts->upstream && opts->squash_onto)
		write_file(मार्गः_squash_onto(), "%s\n",
			   oid_गन्तव्यम्_hex(opts->squash_onto));

	discard_सूचकांक(the_repository->सूचकांक);
	यदि (todo_list_parse_insn_बफर(the_repository, &replay,
					todo_list.buf.buf, &todo_list))
		BUG("unusable todo list");

	ret = complete_action(the_repository, &replay, flags,
		shortrevisions, opts->onto_name, opts->onto,
		&opts->orig_शीर्ष->object.oid, &opts->exec,
		opts->autosquash, opts->update_refs, &todo_list);

cleanup:
	replay_opts_release(&replay);
	मुक्त(revisions);
	मुक्त(shortrevisions);
	todo_list_release(&todo_list);
	strvec_clear(&make_script_तर्काः);

	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क सततम्_sequencer_rebase(संरचना rebase_options *opts)
{
	अचिह्नित flags = 0;
	पूर्णाङ्क abbreviate_commands = 0, ret = 0;

	repo_config_get_bool(the_repository, "rebase.abbreviatecommands", &abbreviate_commands);

	flags |= opts->keep_रिक्तानि ? TODO_LIST_KEEP_रिक्तानि : 0;
	flags |= abbreviate_commands ? TODO_LIST_ABBREVIATE_CMDS : 0;
	flags |= opts->rebase_merges ? TODO_LIST_REBASE_MERGES : 0;
	flags |= opts->rebase_cousins > 0 ? TODO_LIST_REBASE_COUSINS : 0;
	flags |= opts->मूलम्_with_onto ? TODO_LIST_मूलम्_WITH_ONTO : 0;
	flags |= opts->reapply_cherry_picks ? TODO_LIST_REAPPLY_CHERRY_PICKS : 0;
	flags |= opts->flags & REBASE_NO_QUIET ? TODO_LIST_WARN_SKIPPED_CHERRY_PICKS : 0;

	विकल्प (opts->action) {
	पक्ष ACTION_NONE: {
		यदि (!opts->onto && !opts->upstream)
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
		strvec_pushl(&cmd.तर्काः, "show", "REBASE_HEAD", "--", रिक्त);
		ret = सततम्_command(&cmd);

		भङ्ग;
	}
	स्वभाव:
		BUG("invalid command '%d'", opts->action);
	}

	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क is_merge(संरचना rebase_options *opts)
{
	प्रत्यागम opts->type == REBASE_MERGE;
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
स्थिर ध्रुव वर्ण *state_dir_मार्गः(ध्रुव वर्ण *filename, संरचना rebase_options *opts)
{
	स्थिर संरचना strbuf मार्गः = STRBUF_INIT;
	स्थिर आकार_t उपसर्गः_len;

	यदि (!उपसर्गः_len) {
		strbuf_addf(&मार्गः, "%s/", opts->state_dir);
		उपसर्गः_len = मार्गः.len;
	}

	strbuf_setlen(&मार्गः, उपसर्गः_len);
	strbuf_addstr(&मार्गः, filename);
	प्रत्यागम मार्गः.buf;
}

/* Initialize the rebase options from the state directory. */
स्थिर पूर्णाङ्क read_basic_state(संरचना rebase_options *opts)
{
	संरचना strbuf शीर्ष_name = STRBUF_INIT;
	संरचना strbuf buf = STRBUF_INIT;
	संरचना object_id oid;

	यदि (!read_oneliner(&शीर्ष_name, state_dir_मार्गः("head-name", opts),
			   READ_ONELINER_WARN_MISSING) ||
	    !read_oneliner(&buf, state_dir_मार्गः("onto", opts),
			   READ_ONELINER_WARN_MISSING))
		प्रत्यागम -1;
	opts->शीर्ष_name = starts_with(शीर्ष_name.buf, "refs/") ?
		xstrdup(शीर्ष_name.buf) : रिक्त;
	strbuf_release(&शीर्ष_name);
	यदि (get_oid_hex(buf.buf, &oid) ||
	    !(opts->onto = lookup_commit_object(the_repository, &oid)))
		प्रत्यागम error(_("invalid onto: '%s'"), buf.buf);

	/*
	 * We always write to orig-head, but interactive rebase used to write to
	 * head. Fall back to reading from head to cover for the case that the
	 * user upgraded git with an ongoing interactive rebase.
	 */
	strbuf_reset(&buf);
	यदि (file_exists(state_dir_मार्गः("orig-head", opts))) {
		यदि (!read_oneliner(&buf, state_dir_मार्गः("orig-head", opts),
				   READ_ONELINER_WARN_MISSING))
			प्रत्यागम -1;
	} अन्यथा यदि (!read_oneliner(&buf, state_dir_मार्गः("head", opts),
				  READ_ONELINER_WARN_MISSING))
		प्रत्यागम -1;
	यदि (get_oid_hex(buf.buf, &oid) ||
	    !(opts->orig_शीर्ष = lookup_commit_object(the_repository, &oid)))
		प्रत्यागम error(_("invalid orig-head: '%s'"), buf.buf);

	यदि (file_exists(state_dir_मार्गः("quiet", opts)))
		opts->flags &= ~REBASE_NO_QUIET;
	अन्यथा
		opts->flags |= REBASE_NO_QUIET;

	यदि (file_exists(state_dir_मार्गः("verbose", opts)))
		opts->flags |= REBASE_VERBOSE;

	यदि (file_exists(state_dir_मार्गः("signoff", opts))) {
		opts->signoff = 1;
		opts->flags |= REBASE_FORCE;
	}

	यदि (file_exists(state_dir_मार्गः("allow_rerere_autoupdate", opts))) {
		strbuf_reset(&buf);
		यदि (!read_oneliner(&buf, state_dir_मार्गः("allow_rerere_autoupdate", opts),
				   READ_ONELINER_WARN_MISSING))
			प्रत्यागम -1;
		यदि (!वाक्यं_तुलय(buf.buf, "--rerere-autoupdate"))
			opts->allow_rerere_autoupdate = RERERE_AUTOUPDATE;
		अन्यथा यदि (!वाक्यं_तुलय(buf.buf, "--no-rerere-autoupdate"))
			opts->allow_rerere_autoupdate = RERERE_NOAUTOUPDATE;
		अन्यथा
			warning(_("ignoring invalid allow_rerere_autoupdate: "
				  "'%s'"), buf.buf);
	}

	यदि (file_exists(state_dir_मार्गः("gpg_sign_opt", opts))) {
		strbuf_reset(&buf);
		यदि (!read_oneliner(&buf, state_dir_मार्गः("gpg_sign_opt", opts),
				   READ_ONELINER_WARN_MISSING))
			प्रत्यागम -1;
		मुक्त(opts->gpg_sign_opt);
		opts->gpg_sign_opt = xstrdup(buf.buf);
	}

	strbuf_release(&buf);

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क rebase_write_basic_state(संरचना rebase_options *opts)
{
	write_file(state_dir_मार्गः("head-name", opts), "%s",
		   opts->शीर्ष_name ? opts->शीर्ष_name : "detached HEAD");
	write_file(state_dir_मार्गः("onto", opts), "%s",
		   opts->onto ? oid_गन्तव्यम्_hex(&opts->onto->object.oid) : "");
	write_file(state_dir_मार्गः("orig-head", opts), "%s",
		   oid_गन्तव्यम्_hex(&opts->orig_शीर्ष->object.oid));
	यदि (!(opts->flags & REBASE_NO_QUIET))
		write_file(state_dir_मार्गः("quiet", opts), "%s", "");
	यदि (opts->flags & REBASE_VERBOSE)
		write_file(state_dir_मार्गः("verbose", opts), "%s", "");
	यदि (opts->allow_rerere_autoupdate > 0)
		write_file(state_dir_मार्गः("allow_rerere_autoupdate", opts),
			   "-%s-rerere-autoupdate",
			   opts->allow_rerere_autoupdate == RERERE_AUTOUPDATE ?
				"" : "-no");
	यदि (opts->gpg_sign_opt)
		write_file(state_dir_मार्गः("gpg_sign_opt", opts), "%s",
			   opts->gpg_sign_opt);
	यदि (opts->signoff)
		write_file(state_dir_मार्गः("signoff", opts), "--signoff");

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क cleanup_autostash(संरचना rebase_options *opts)
{
	पूर्णाङ्क ret;
	संरचना strbuf dir = STRBUF_INIT;
	ध्रुव वर्ण *मार्गः = state_dir_मार्गः("autostash", opts);

	यदि (!file_exists(मार्गः))
		प्रत्यागम 0;
	ret = apply_autostash(मार्गः);
	strbuf_addstr(&dir, opts->state_dir);
	यदि (remove_dir_recursively(&dir, 0))
		ret = error_errno(_("could not remove '%s'"), opts->state_dir);
	strbuf_release(&dir);

	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क finish_rebase(संरचना rebase_options *opts)
{
	संरचना strbuf dir = STRBUF_INIT;
	पूर्णाङ्क ret = 0;

	refs_delete_ref(get_main_ref_store(the_repository), रिक्त,
			"REBASE_HEAD", रिक्त, REF_NO_DEREF);
	refs_delete_ref(get_main_ref_store(the_repository), रिक्त,
			"AUTO_MERGE", रिक्त, REF_NO_DEREF);
	apply_autostash(state_dir_मार्गः("autostash", opts));
	/*
	 * We ignore errors in 'git maintenance run --auto', since the
	 * user should see them.
	 */
	सततम्_auto_maintenance(the_repository,
			!(opts->flags & (REBASE_NO_QUIET|REBASE_VERBOSE)));

	यदि (opts->type == REBASE_MERGE) {
		संरचना replay_opts replay = REPLAY_OPTS_INIT;

		replay.action = REPLAY_INTERACTIVE_REBASE;
		ret = sequencer_remove_state(&replay);
		replay_opts_release(&replay);
	} अन्यथा {
		strbuf_addstr(&dir, opts->state_dir);
		यदि (remove_dir_recursively(&dir, 0))
			ret = error(_("could not remove '%s'"),
				    opts->state_dir);
		strbuf_release(&dir);
	}

	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क move_गन्तव्यम्_original_branch(संरचना rebase_options *opts)
{
	संरचना strbuf branch_reflog = STRBUF_INIT, शीर्ष_reflog = STRBUF_INIT;
	संरचना reset_working_वृक्षः_options ropts = { 0 };
	पूर्णाङ्क ret;

	यदि (!opts->शीर्ष_name)
		प्रत्यागम 0; /* nothing to move back to */

	यदि (!opts->onto)
		BUG("move_to_original_branch without onto");

	strbuf_addf(&branch_reflog, "%s (finish): %s onto %s",
		    opts->reflog_action,
		    opts->शीर्ष_name, oid_गन्तव्यम्_hex(&opts->onto->object.oid));
	strbuf_addf(&शीर्ष_reflog, "%s (finish): returning to %s",
		    opts->reflog_action, opts->शीर्ष_name);
	ropts.branch = opts->शीर्ष_name;
	ropts.flags = RESET_WORKING_वृक्षः_REFS_ONLY |
		      RESET_WORKING_वृक्षः_UPDATE_शीर्ष;
	ropts.branch_सन्देशः = branch_reflog.buf;
	ropts.शीर्ष_सन्देशः = शीर्ष_reflog.buf;
	ret = reset_working_वृक्षः(the_repository, &ropts);

	strbuf_release(&branch_reflog);
	strbuf_release(&शीर्ष_reflog);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क सततम्_am(संरचना rebase_options *opts)
{
	संरचना child_process am = CHILD_PROCESS_INIT;
	संरचना child_process format_patch = CHILD_PROCESS_INIT;
	पूर्णाङ्क स्थितिः;
	वर्ण *rebased_patches;

	am.git_cmd = 1;
	strvec_प्रक्षिप(&am.तर्काः, "am");
	strvec_pushf(&am.env, GIT_REFLOG_ACTION_ENVIRONMENT "=%s (pick)",
		     opts->reflog_action);
	यदि (opts->action == ACTION_CONTINUE) {
		strvec_प्रक्षिप(&am.तर्काः, "--resolved");
		strvec_pushf(&am.तर्काः, "--resolvemsg=%s", rebase_resolvemsg);
		यदि (opts->gpg_sign_opt)
			strvec_प्रक्षिप(&am.तर्काः, opts->gpg_sign_opt);
		स्थितिः = सततम्_command(&am);
		यदि (स्थितिः)
			प्रत्यागम स्थितिः;

		प्रत्यागम move_गन्तव्यम्_original_branch(opts);
	}
	यदि (opts->action == ACTION_SKIP) {
		strvec_प्रक्षिप(&am.तर्काः, "--skip");
		strvec_pushf(&am.तर्काः, "--resolvemsg=%s", rebase_resolvemsg);
		स्थितिः = सततम्_command(&am);
		यदि (स्थितिः)
			प्रत्यागम स्थितिः;

		प्रत्यागम move_गन्तव्यम्_original_branch(opts);
	}
	यदि (opts->action == ACTION_SHOW_CURRENT_PATCH) {
		strvec_प्रक्षिप(&am.तर्काः, "--show-current-patch");
		प्रत्यागम सततम्_command(&am);
	}

	rebased_patches = repo_git_मार्गः(the_repository, "rebased-patches");
	format_patch.बहिर्गमनम् = उद्घाटय(rebased_patches,
				O_WRONLY | O_CREAT | O_TRUNC, 0666);
	यदि (format_patch.बहिर्गमनम् < 0) {
		स्थितिः = error_errno(_("could not open '%s' for writing"),
				     rebased_patches);
		मुक्त(rebased_patches);
		child_process_clear(&am);
		प्रत्यागम स्थितिः;
	}

	format_patch.git_cmd = 1;
	strvec_pushl(&format_patch.तर्काः, "format-patch", "-k", "--stdout",
		     "--full-index", "--cherry-pick", "--right-only",
		     "--default-prefix", "--no-renames",
		     "--no-cover-letter", "--pretty=mboxrd", "--topo-order",
		     "--no-base", रिक्त);
	यदि (opts->git_format_patch_opt.len)
		strvec_विभज(&format_patch.तर्काः,
			     opts->git_format_patch_opt.buf);
	strvec_pushf(&format_patch.तर्काः, "%s...%s",
		     oid_गन्तव्यम्_hex(opts->मूलम् ?
				/* this is now equivalent to !opts->upstream */
				&opts->onto->object.oid :
				&opts->upstream->object.oid),
		     oid_गन्तव्यम्_hex(&opts->orig_शीर्ष->object.oid));
	यदि (opts->restrict_revision)
		strvec_pushf(&format_patch.तर्काः, "^%s",
			     oid_गन्तव्यम्_hex(&opts->restrict_revision->object.oid));

	स्थितिः = सततम्_command(&format_patch);
	यदि (स्थितिः) {
		संरचना reset_working_वृक्षः_options ropts = { 0 };
		unlink(rebased_patches);
		मुक्त(rebased_patches);
		child_process_clear(&am);

		ropts.oid = &opts->orig_शीर्ष->object.oid;
		ropts.branch = opts->शीर्ष_name;
		ropts.default_reflog_action = opts->reflog_action;
		ropts.flags = RESET_WORKING_वृक्षः_UPDATE_शीर्ष;
		reset_working_वृक्षः(the_repository, &ropts);
		error(_("\ngit encountered an error while preparing the "
			"patches to replay\n"
			"these revisions:\n"
			"\n    %s\n\n"
			"As a result, git cannot rebase them."),
		      opts->revisions);

		प्रत्यागम स्थितिः;
	}

	am.अन्तर् = उद्घाटय(rebased_patches, O_RDONLY);
	यदि (am.अन्तर् < 0) {
		स्थितिः = error_errno(_("could not open '%s' for reading"),
				     rebased_patches);
		मुक्त(rebased_patches);
		child_process_clear(&am);
		प्रत्यागम स्थितिः;
	}

	strvec_pushv(&am.तर्काः, opts->git_am_opts.v);
	strvec_प्रक्षिप(&am.तर्काः, "--rebasing");
	strvec_pushf(&am.तर्काः, "--resolvemsg=%s", rebase_resolvemsg);
	strvec_प्रक्षिप(&am.तर्काः, "--patch-format=mboxrd");
	यदि (opts->allow_rerere_autoupdate == RERERE_AUTOUPDATE)
		strvec_प्रक्षिप(&am.तर्काः, "--rerere-autoupdate");
	अन्यथा यदि (opts->allow_rerere_autoupdate == RERERE_NOAUTOUPDATE)
		strvec_प्रक्षिप(&am.तर्काः, "--no-rerere-autoupdate");
	यदि (opts->gpg_sign_opt)
		strvec_प्रक्षिप(&am.तर्काः, opts->gpg_sign_opt);
	स्थितिः = सततम्_command(&am);
	unlink(rebased_patches);
	मुक्त(rebased_patches);

	यदि (!स्थितिः) {
		प्रत्यागम move_गन्तव्यम्_original_branch(opts);
	}

	यदि (is_directory(opts->state_dir))
		rebase_write_basic_state(opts);

	प्रत्यागम स्थितिः;
}

स्थिर पूर्णाङ्क सततम्_specific_rebase(संरचना rebase_options *opts)
{
	पूर्णाङ्क स्थितिः;

	यदि (opts->type == REBASE_MERGE) {
		/* Run sequencer-based rebase */
		यदि (!(opts->flags & REBASE_INTERACTIVE_EXPLICIT))
			setenv("GIT_SEQUENCE_EDITOR", ":", 1);
		यदि (opts->gpg_sign_opt) {
			/* remove the leading "-S" */
			वर्ण *tmp = xstrdup(opts->gpg_sign_opt + 2);
			मुक्त(opts->gpg_sign_opt);
			opts->gpg_sign_opt = tmp;
		}

		स्थितिः = सततम्_sequencer_rebase(opts);
	} अन्यथा यदि (opts->type == REBASE_APPLY)
		स्थितिः = सततम्_am(opts);
	अन्यथा
		BUG("Unhandled rebase type %d", opts->type);

	यदि (opts->dont_finish_rebase)
		; /* do nothing */
	अन्यथा यदि (opts->type == REBASE_MERGE) {
		पूर्णाङ्क quiet = !(opts->flags & (REBASE_NO_QUIET|REBASE_VERBOSE));

		/*
		 * The sequencer cleans up after itself. Its state directory
		 * is gone once it is done, and stays while it is stopped.
		 */
		यदि (स्थितिः == 0 && !is_directory(opts->state_dir))
			सततम्_auto_maintenance(the_repository, quiet);
	} अन्यथा यदि (स्थितिः == 0) {
		यदि (!file_exists(state_dir_मार्गः("stopped-sha", opts)))
			finish_rebase(opts);
	} अन्यथा यदि (स्थितिः == 2) {
		संरचना strbuf dir = STRBUF_INIT;

		apply_autostash(state_dir_मार्गः("autostash", opts));
		strbuf_addstr(&dir, opts->state_dir);
		remove_dir_recursively(&dir, 0);
		strbuf_release(&dir);
		die("Nothing to do");
	}

	प्रत्यागम स्थितिः ? -1 : 0;
}

स्थिर शून्य parse_rebase_merges_मानम्(संरचना rebase_options *options, ध्रुव वर्ण *मानम्)
{
	यदि (!वाक्यं_तुलय("no-rebase-cousins", मानम्))
		options->rebase_cousins = 0;
	अन्यथा यदि (!वाक्यं_तुलय("rebase-cousins", मानम्))
		options->rebase_cousins = 1;
	अन्यथा
		die(_("Unknown rebase-merges mode: %s"), मानम्);
}

स्थिर पूर्णाङ्क rebase_config(ध्रुव वर्ण *var, ध्रुव वर्ण *मानम्,
			 ध्रुव संरचना config_context *ctx, शून्य *दत्त)
{
	संरचना rebase_options *opts = दत्त;

	यदि (!वाक्यं_तुलय(var, "rebase.stat")) {
		यदि (git_config_bool(var, मानम्))
			opts->flags |= REBASE_DIFFSTAT;
		अन्यथा
			opts->flags &= ~REBASE_DIFFSTAT;
		प्रत्यागम 0;
	}

	यदि (!वाक्यं_तुलय(var, "rebase.autosquash")) {
		opts->config_autosquash = git_config_bool(var, मानम्);
		प्रत्यागम 0;
	}

	यदि (!वाक्यं_तुलय(var, "commit.gpgsign")) {
		मुक्त(opts->gpg_sign_opt);
		opts->gpg_sign_opt = git_config_bool(var, मानम्) ?
			xstrdup("-S") : रिक्त;
		प्रत्यागम 0;
	}

	यदि (!वाक्यं_तुलय(var, "rebase.autostash")) {
		opts->autostash = git_config_bool(var, मानम्);
		प्रत्यागम 0;
	}

	यदि (!वाक्यं_तुलय(var, "rebase.rebasemerges")) {
		opts->config_rebase_merges = git_parse_maybe_bool(मानम्);
		यदि (opts->config_rebase_merges < 0) {
			opts->config_rebase_merges = 1;
			parse_rebase_merges_मानम्(opts, मानम्);
		} अन्यथा {
			opts->rebase_cousins = 0;
		}
		प्रत्यागम 0;
	}

	यदि (!वाक्यं_तुलय(var, "rebase.updaterefs")) {
		opts->config_update_refs = git_config_bool(var, मानम्);
		प्रत्यागम 0;
	}

	यदि (!वाक्यं_तुलय(var, "rebase.reschedulefailedexec")) {
		opts->reschedule_failed_exec = git_config_bool(var, मानम्);
		प्रत्यागम 0;
	}

	यदि (!वाक्यं_तुलय(var, "rebase.forkpoint")) {
		opts->fork_point = git_config_bool(var, मानम्) ? -1 : 0;
		प्रत्यागम 0;
	}

	यदि (!वाक्यं_तुलय(var, "rebase.backend")) {
		FREE_AND_NULL(opts->default_backend);
		प्रत्यागम git_config_string(&opts->default_backend, var, मानम्);
	}

	प्रत्यागम git_default_config(var, मानम्, ctx, दत्त);
}

स्थिर पूर्णाङ्क checkout_ऊर्ध्वम्_गन्तव्यम्_date(संरचना rebase_options *options)
{
	संरचना strbuf buf = STRBUF_INIT;
	संरचना reset_working_वृक्षः_options ropts = { 0 };
	पूर्णाङ्क ret = 0;

	strbuf_addf(&buf, "%s: checkout %s",
		    options->reflog_action, options->switch_गन्तव्यम्);
	ropts.oid = &options->orig_शीर्ष->object.oid;
	ropts.branch = options->शीर्ष_name;
	ropts.flags = RESET_WORKING_वृक्षः_सततम्_POST_CHECKOUT_HOOK |
		      RESET_WORKING_वृक्षः_UPDATE_शीर्ष;
	यदि (!ropts.branch)
		ropts.flags |=  RESET_WORKING_वृक्षः_DETACH;
	ropts.शीर्ष_सन्देशः = buf.buf;
	यदि (reset_working_वृक्षः(the_repository, &ropts) < 0)
		ret = error(_("could not switch to %s"), options->switch_गन्तव्यम्);
	strbuf_release(&buf);

	प्रत्यागम ret;
}

/*
 * Determines whether the commits in from..to are linear, i.e. contain
 * no merge commits. This function *expects* `from` to be an ancestor of
 * `to`.
 */
स्थिर पूर्णाङ्क is_रैखिकम्_history(संरचना commit *from, संरचना commit *गन्तव्यम्)
{
	यावत् (गन्तव्यम् && गन्तव्यम् != from) {
		repo_parse_commit(the_repository, गन्तव्यम्);
		यदि (!गन्तव्यम्->parents)
			प्रत्यागम 1;
		यदि (गन्तव्यम्->parents->अग्र)
			प्रत्यागम 0;
		गन्तव्यम् = गन्तव्यम्->parents->item;
	}
	प्रत्यागम 1;
}

स्थिर पूर्णाङ्क can_fast_forward(संरचना commit *onto, संरचना commit *upstream,
			    संरचना commit *restrict_revision,
			    संरचना commit *शीर्ष, संरचना object_id *branch_आधारः)
{
	संरचना commit_list *merge_bases = रिक्त;
	पूर्णाङ्क res = 0;

	यदि (is_null_oid(branch_आधारः))
		गच्छ समाप्तम्; /* fill_branch_base() found multiple merge bases */

	यदि (!oideq(branch_आधारः, &onto->object.oid))
		गच्छ समाप्तम्;

	यदि (restrict_revision && !oideq(&restrict_revision->object.oid, branch_आधारः))
		गच्छ समाप्तम्;

	यदि (!upstream)
		गच्छ समाप्तम्;

	यदि (repo_get_merge_bases(the_repository, upstream, शीर्ष, &merge_bases) < 0)
		निर्गच्छ(128);
	यदि (!merge_bases || merge_bases->अग्र)
		गच्छ समाप्तम्;

	यदि (!oideq(&onto->object.oid, &merge_bases->item->object.oid))
		गच्छ समाप्तम्;

	res = 1;

समाप्तम्:
	commit_list_free(merge_bases);
	प्रत्यागम res && is_रैखिकम्_history(onto, शीर्ष);
}

स्थिर शून्य fill_branch_आधारः(संरचना rebase_options *options,
			    संरचना object_id *branch_आधारः)
{
	संरचना commit_list *merge_bases = रिक्त;

	यदि (repo_get_merge_bases(the_repository, options->onto,
				 options->orig_शीर्ष, &merge_bases) < 0)
		निर्गच्छ(128);
	यदि (!merge_bases || merge_bases->अग्र)
		oidcpy(branch_आधारः, null_oid(the_hash_algo));
	अन्यथा
		oidcpy(branch_आधारः, &merge_bases->item->object.oid);

	commit_list_free(merge_bases);
}

स्थिर पूर्णाङ्क parse_opt_am(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना rebase_options *opts = opt->मानम्;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	यदि (opts->type != REBASE_UNSPECIFIED && opts->type != REBASE_APPLY)
	    die(_("apply options and merge options cannot be used together"));

	opts->type = REBASE_APPLY;

	प्रत्यागम 0;
}

/* -i followed by -m is still -i */
स्थिर पूर्णाङ्क parse_opt_merge(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना rebase_options *opts = opt->मानम्;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	यदि (opts->type != REBASE_UNSPECIFIED && opts->type != REBASE_MERGE)
	    die(_("apply options and merge options cannot be used together"));

	opts->type = REBASE_MERGE;

	प्रत्यागम 0;
}

/* -i followed by -r is still explicitly interactive, but -r alone is not */
स्थिर पूर्णाङ्क parse_opt_interactive(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg,
				 पूर्णाङ्क unset)
{
	संरचना rebase_options *opts = opt->मानम्;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	यदि (opts->type != REBASE_UNSPECIFIED && opts->type != REBASE_MERGE)
	    die(_("apply options and merge options cannot be used together"));

	opts->type = REBASE_MERGE;
	opts->flags |= REBASE_INTERACTIVE_EXPLICIT;

	प्रत्यागम 0;
}

स्थिर गणना रिक्तानि_type parse_रिक्तानि_मानम्(ध्रुव वर्ण *मानम्)
{
	यदि (!strcasecmp(मानम्, "drop"))
		प्रत्यागम रिक्तानि_DROP;
	अन्यथा यदि (!strcasecmp(मानम्, "keep"))
		प्रत्यागम रिक्तानि_KEEP;
	अन्यथा यदि (!strcasecmp(मानम्, "stop"))
		प्रत्यागम रिक्तानि_STOP;
	अन्यथा यदि (!strcasecmp(मानम्, "ask")) {
		warning(_("--empty=ask is deprecated; use '--empty=stop' instead."));
		प्रत्यागम रिक्तानि_STOP;
	}

	die(_("unrecognized empty type '%s'; valid values are \"drop\", \"keep\", and \"stop\"."), मानम्);
}

स्थिर पूर्णाङ्क parse_opt_keep_रिक्तानि(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg,
				पूर्णाङ्क unset)
{
	संरचना rebase_options *opts = opt->मानम्;

	BUG_ON_OPT_ARG(arg);

	imply_merge(opts, unset ? "--no-keep-empty" : "--keep-empty");
	opts->keep_रिक्तानि = !unset;
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क parse_opt_रिक्तानि(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना rebase_options *options = opt->मानम्;
	गणना रिक्तानि_type मानम् = parse_रिक्तानि_मानम्(arg);

	BUG_ON_OPT_NEG(unset);

	options->रिक्तानि = मानम्;
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क parse_opt_rebase_merges(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना rebase_options *options = opt->मानम्;

	options->rebase_merges = !unset;
	options->rebase_cousins = 0;

	यदि (arg) {
		यदि (!*arg) {
			warning(_("--rebase-merges with an empty string "
				  "argument is deprecated and will stop "
				  "working in a future version of Git. Use "
				  "--rebase-merges without an argument "
				  "instead, which does the same thing."));
			प्रत्यागम 0;
		}
		parse_rebase_merges_मानम्(options, arg);
	}

	प्रत्यागम 0;
}

स्थिर शून्य NORETURN error_on_missing_default_upstream(शून्य)
{
	संरचना branch *current_branch = branch_get(रिक्त);

	प्रारूप_लिख(_("%s\n"
		 "Please specify which branch you want to rebase against.\n"
		 "See git-rebase(1) for details.\n"
		 "\n"
		 "    git rebase '<branch>'\n"
		 "\n"),
		current_branch ? _("There is no tracking information for "
			"the current branch.") :
			_("You are not currently on a branch."));

	यदि (current_branch) {
		ध्रुव वर्ण *remote = current_branch->remote_name;

		यदि (!remote)
			remote = _("<remote>");

		प्रारूप_लिख(_("If you wish to set tracking information for this "
			 "branch you can do so with:\n"
			 "\n"
			 "    git branch --set-upstream-to=%s/<branch> %s\n"
			 "\n"),
		       remote, current_branch->name);
	}
	निर्गच्छ(1);
}

स्थिर पूर्णाङ्क check_exec_cmd(ध्रुव वर्ण *cmd)
{
	यदि (strchr(cmd, '\n'))
		प्रत्यागम error(_("exec commands cannot contain newlines"));

	/* Does the command consist purely of whitespace? */
	यदि (!cmd[strspn(cmd, " \t\r\f\v")])
		प्रत्यागम error(_("empty exec command"));

	प्रत्यागम 0;
}

पूर्णाङ्क cmd_rebase(पूर्णाङ्क argc,
	       ध्रुव वर्ण **argv,
	       ध्रुव वर्ण *उपसर्गः,
	       संरचना repository *repo अप्रयुक्तम्)
{
	संरचना rebase_options options = REBASE_OPTIONS_INIT;
	ध्रुव वर्ण *branch_name;
	ध्रुव वर्ण *strategy_opt = रिक्त;
	पूर्णाङ्क ret, flags, योग_argc, अन्तर्_प्रगतिः = 0;
	पूर्णाङ्क keep_आधारः = 0;
	पूर्णाङ्क ok_गन्तव्यम्_skip_pre_rebase = 0;
	संरचना strbuf सन्देशः = STRBUF_INIT;
	संरचना strbuf revisions = STRBUF_INIT;
	संरचना strbuf buf = STRBUF_INIT;
	संरचना object_id branch_आधारः;
	पूर्णाङ्क ignore_whitespace = 0;
	ध्रुव वर्ण *gpg_sign = रिक्त;
	संरचना object_id squash_onto;
	वर्ण *squash_onto_name = रिक्त;
	वर्ण *keep_आधारः_onto_name = रिक्त;
	पूर्णाङ्क reschedule_failed_exec = -1;
	पूर्णाङ्क allow_preemptive_ff = 1;
	पूर्णाङ्क preserve_merges_selected = 0;
	संरचना reset_working_वृक्षः_options ropts = { 0 };
	संरचना option builtin_rebase_options[] = {
		OPT_STRING(0, "onto", &options.onto_name,
			   N_("revision"),
			   N_("rebase onto given branch instead of upstream")),
		OPT_BOOL(0, "keep-base", &keep_आधारः,
			 N_("use the merge-base of upstream and branch as the current base")),
		OPT_BOOL(0, "no-verify", &ok_गन्तव्यम्_skip_pre_rebase,
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
		OPT_STRVEC(0, "trailer", &options.trailer_तर्काः, N_("trailer"),
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
		OPT_समुच्चयः_INT_F('p', "preserve-merges", &preserve_merges_selected,
			      N_("(REMOVED) was: try to recreate merges "
				 "instead of ignoring them"),
			      1, PARSE_OPT_HIDDEN),
		OPT_RERERE_AUTOUPDATE(&options.allow_rerere_autoupdate),
		OPT_CALLBACK_F(0, "empty", &options, "(drop|keep|stop)",
			       N_("how to handle commits that become empty"),
			       PARSE_OPT_NONEG, parse_opt_रिक्तानि),
		OPT_CALLBACK_F('k', "keep-empty", &options, रिक्त,
			N_("keep commits which start empty"),
			PARSE_OPT_NOARG | PARSE_OPT_HIDDEN,
			parse_opt_keep_रिक्तानि),
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
			   &options.allow_रिक्तानि_सन्देशः,
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
		OPT_अन्तः(),
	};
	पूर्णाङ्क i;

	show_usage_with_options_if_asked(argc, argv,
					 builtin_rebase_usage,
					 builtin_rebase_options);

#यदि_अपरिभाषितम् WITH_BREAKING_CHANGES
	warn_on_auto_comment_char = true;
#यदि_अन्तः /* !WITH_BREAKING_CHANGES */
	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_पूर्णम्_सूचकांक = 0;

	repo_config(the_repository, rebase_config, &options);
	/* options.gpg_sign_opt will be either "-S" or NULL */
	gpg_sign = options.gpg_sign_opt ? "" : रिक्त;
	FREE_AND_NULL(options.gpg_sign_opt);

	strbuf_reset(&buf);
	strbuf_addf(&buf, "%s/applying", apply_dir());
	यदि(file_exists(buf.buf))
		die(_("It looks like 'git am' is in progress. Cannot rebase."));

	यदि (is_directory(apply_dir())) {
		options.type = REBASE_APPLY;
		options.state_dir = apply_dir();
	} अन्यथा यदि (is_directory(merge_dir())) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "%s/rewritten", merge_dir());
		यदि (!(options.action == ACTION_ABORT) && is_directory(buf.buf)) {
			die(_("`rebase --preserve-merges` (-p) is no longer supported.\n"
			"Use `git rebase --abort` to terminate current rebase.\n"
			"Or downgrade to v2.33, or earlier, to complete the rebase."));
		} अन्यथा {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "%s/interactive", merge_dir());
			options.type = REBASE_MERGE;
			यदि (file_exists(buf.buf))
				options.flags |= REBASE_INTERACTIVE_EXPLICIT;
		}
		options.state_dir = merge_dir();
	}

	यदि (options.type != REBASE_UNSPECIFIED)
		अन्तर्_प्रगतिः = 1;

	योग_argc = argc;
	argc = parse_options(argc, argv, उपसर्गः,
			     builtin_rebase_options,
			     builtin_rebase_usage, 0);

	यदि (options.trailer_तर्काः.nr) {
		यदि (validate_trailer_तर्काः(&options.trailer_तर्काः))
			die(रिक्त);
		options.flags |= REBASE_FORCE;
	}

	यदि (preserve_merges_selected)
		die(_("--preserve-merges was replaced by --rebase-merges\n"
			"Note: Your `pull.rebase` configuration may also be set to 'preserve',\n"
			"which is no longer supported; use 'merges' instead"));

	यदि (options.action != ACTION_NONE && योग_argc != 2) {
		usage_with_options(builtin_rebase_usage,
				   builtin_rebase_options);
	}

	यदि (argc > 2)
		usage_with_options(builtin_rebase_usage,
				   builtin_rebase_options);

	यदि (keep_आधारः) {
		यदि (options.onto_name)
			die(_("options '%s' and '%s' cannot be used together"), "--keep-base", "--onto");
		यदि (options.मूलम्)
			die(_("options '%s' and '%s' cannot be used together"), "--keep-base", "--root");
		/*
		 * --keep-base defaults to --no-fork-point to keep the
		 * base the same.
		 */
		यदि (options.fork_point < 0)
			options.fork_point = 0;
	}
	यदि (options.मूलम् && options.fork_point > 0)
		die(_("options '%s' and '%s' cannot be used together"), "--root", "--fork-point");

	यदि (options.action != ACTION_NONE && !अन्तर्_प्रगतिः)
		die(_("no rebase in progress"));

	यदि (options.action == ACTION_EDIT_TODO && !is_merge(&options))
		die(_("The --edit-todo action can only be used during "
		      "interactive rebase."));

	यदि (trace2_is_enabled()) {
		यदि (is_merge(&options))
			trace2_cmd_बहुलकम्("interactive");
		अन्यथा यदि (options.exec.nr)
			trace2_cmd_बहुलकम्("interactive-exec");
		अन्यथा
			trace2_cmd_बहुलकम्(action_नामानि[options.action]);
	}

	options.reflog_action = getenv(GIT_REFLOG_ACTION_ENVIRONMENT);
	options.reflog_action =
		xstrdup(options.reflog_action ? options.reflog_action : "rebase");

	विकल्प (options.action) {
	पक्ष ACTION_CONTINUE: {
		संरचना object_id शीर्ष;
		संरचना तालकम्_file तालकम्_file = तालकम्_INIT;
		पूर्णाङ्क fd;

		/* Sanity check */
		यदि (repo_get_oid(the_repository, "HEAD", &शीर्ष))
			die(_("Cannot read HEAD"));

		fd = repo_hold_locked_सूचकांक(the_repository, &तालकम्_file, 0);
		यदि (repo_read_सूचकांक(the_repository) < 0)
			die(_("could not read index"));
		refresh_सूचकांक(the_repository->सूचकांक, REFRESH_QUIET, रिक्त, रिक्त,
			      रिक्त);
		यदि (0 <= fd)
			repo_update_सूचकांक_if_able(the_repository, &तालकम्_file);
		rollback_तालकम्_file(&तालकम्_file);

		यदि (has_unstaged_changes(the_repository, 1)) {
			पङ्क्तिं_लिख(_("You must edit all merge conflicts and then\n"
			       "mark them as resolved using git add"));
			निर्गच्छ(1);
		}
		यदि (read_basic_state(&options))
			निर्गच्छ(1);
		गच्छ सततम्_rebase;
	}
	पक्ष ACTION_SKIP: {
		संरचना string_list merge_rr = STRING_LIST_INIT_DUP;

		rerere_clear(the_repository, &merge_rr);
		string_list_clear(&merge_rr, 1);
		ropts.flags = RESET_WORKING_वृक्षः_HARD |
			      RESET_WORKING_वृक्षः_UPDATE_शीर्ष;
		यदि (reset_working_वृक्षः(the_repository, &ropts) < 0)
			die(_("could not discard worktree changes"));
		remove_branch_state(the_repository, 0);
		यदि (read_basic_state(&options))
			निर्गच्छ(1);
		गच्छ सततम्_rebase;
	}
	पक्ष ACTION_ABORT: {
		संरचना string_list merge_rr = STRING_LIST_INIT_DUP;
		संरचना strbuf शीर्ष_सन्देशः = STRBUF_INIT;

		rerere_clear(the_repository, &merge_rr);
		string_list_clear(&merge_rr, 1);

		यदि (read_basic_state(&options))
			निर्गच्छ(1);

		strbuf_addf(&शीर्ष_सन्देशः, "%s (abort): returning to %s",
			    options.reflog_action,
			    options.शीर्ष_name ? options.शीर्ष_name
					      : oid_गन्तव्यम्_hex(&options.orig_शीर्ष->object.oid));
		ropts.oid = &options.orig_शीर्ष->object.oid;
		ropts.शीर्ष_सन्देशः = शीर्ष_सन्देशः.buf;
		ropts.branch = options.शीर्ष_name;
		ropts.flags = RESET_WORKING_वृक्षः_HARD |
			      RESET_WORKING_वृक्षः_UPDATE_शीर्ष;
		यदि (reset_working_वृक्षः(the_repository, &ropts) < 0)
			die(_("could not move back to %s"),
			    oid_गन्तव्यम्_hex(&options.orig_शीर्ष->object.oid));
		strbuf_release(&शीर्ष_सन्देशः);
		remove_branch_state(the_repository, 0);
		ret = finish_rebase(&options);
		गच्छ cleanup;
	}
	पक्ष ACTION_QUIT: {
		save_autostash(state_dir_मार्गः("autostash", &options));
		यदि (options.type == REBASE_MERGE) {
			संरचना replay_opts replay = REPLAY_OPTS_INIT;

			replay.action = REPLAY_INTERACTIVE_REBASE;
			ret = sequencer_remove_state(&replay);
			replay_opts_release(&replay);
		} अन्यथा {
			strbuf_reset(&buf);
			strbuf_addstr(&buf, options.state_dir);
			ret = remove_dir_recursively(&buf, 0);
			यदि (ret)
				error(_("could not remove '%s'"),
				       options.state_dir);
		}
		गच्छ cleanup;
	}
	पक्ष ACTION_EDIT_TODO:
		options.dont_finish_rebase = 1;
		गच्छ सततम्_rebase;
	पक्ष ACTION_SHOW_CURRENT_PATCH:
		options.dont_finish_rebase = 1;
		गच्छ सततम्_rebase;
	पक्ष ACTION_NONE:
		भङ्ग;
	स्वभाव:
		BUG("action: %d", options.action);
	}

	/* Make sure no rebase is in progress */
	यदि (अन्तर्_प्रगतिः) {
		ध्रुव वर्ण *चरमः_slash = strrchr(options.state_dir, '/');
		ध्रुव वर्ण *state_dir_आधारः =
			चरमः_slash ? चरमः_slash + 1 : options.state_dir;
		ध्रुव वर्ण *cmd_जीवितम्_rebase =
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
		    state_dir_आधारः, cmd_जीवितम्_rebase, buf.buf);
	}

	यदि ((options.flags & REBASE_INTERACTIVE_EXPLICIT) ||
	    (options.action != ACTION_NONE) ||
	    (options.exec.nr > 0) ||
	    options.autosquash == 1) {
		allow_preemptive_ff = 0;
	}
	यदि (options.committer_date_is_author_date || options.ignore_date)
		options.flags |= REBASE_FORCE;

	प्रति (i = 0; i < options.git_am_opts.nr; i++) {
		ध्रुव वर्ण *option = options.git_am_opts.v[i], *p;
		यदि (!वाक्यं_तुलय(option, "--whitespace=fix") ||
		    !वाक्यं_तुलय(option, "--whitespace=strip"))
			allow_preemptive_ff = 0;
		अन्यथा यदि (skip_उपसर्गः(option, "-C", &p)) {
			यावत् (*p)
				यदि (!अङ्कः_वा(*(p++)))
					die(_("switch `C' expects a "
					      "numerical value"));
		} अन्यथा यदि (skip_उपसर्गः(option, "--whitespace=", &p)) {
			यदि (*p && वाक्यं_तुलय(p, "warn") && वाक्यं_तुलय(p, "nowarn") &&
			    वाक्यं_तुलय(p, "error") && वाक्यं_तुलय(p, "error-all"))
				die("Invalid whitespace option: '%s'", p);
		}
	}

	प्रति (i = 0; i < options.exec.nr; i++)
		यदि (check_exec_cmd(options.exec.वस्तूनि[i].string))
			निर्गच्छ(1);

	यदि (!(options.flags & REBASE_NO_QUIET))
		strvec_प्रक्षिप(&options.git_am_opts, "-q");

	यदि (options.रिक्तानि != रिक्तानि_UNSPECIFIED)
		imply_merge(&options, "--empty");

	यदि (options.reapply_cherry_picks < 0)
		/*
		 * We default to --no-reapply-cherry-picks unless
		 * --keep-base is given; when --keep-base is given, we want
		 * to default to --reapply-cherry-picks.
		 */
		options.reapply_cherry_picks = keep_आधारः;
	अन्यथा यदि (!keep_आधारः)
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

	यदि (gpg_sign)
		options.gpg_sign_opt = xstrfmt("-S%s", gpg_sign);

	यदि (options.exec.nr)
		imply_merge(&options, "--exec");

	यदि (options.type == REBASE_APPLY) {
		यदि (ignore_whitespace)
			strvec_प्रक्षिप(&options.git_am_opts,
				    "--ignore-whitespace");
		यदि (options.committer_date_is_author_date)
			strvec_प्रक्षिप(&options.git_am_opts,
				    "--committer-date-is-author-date");
		यदि (options.ignore_date)
			strvec_प्रक्षिप(&options.git_am_opts, "--ignore-date");
	} अन्यथा {
		/* REBASE_MERGE */
		यदि (ignore_whitespace) {
			string_list_append(&options.strategy_opts,
					   "ignore-space-change");
		}
	}

	यदि (strategy_opt)
		options.strategy = xstrdup(strategy_opt);
	अन्यथा यदि (options.strategy_opts.nr && !options.strategy)
		options.strategy = xstrdup("ort");
	यदि (options.strategy)
		imply_merge(&options, "--strategy");

	यदि (options.मूलम् && !options.onto_name)
		imply_merge(&options, "--root without --onto");

	यदि (options.trailer_तर्काः.nr)
		imply_merge(&options, "--trailer");

	यदि (isatty(2) && options.flags & REBASE_NO_QUIET)
		strbuf_addstr(&options.git_format_patch_opt, " --progress");

	यदि (options.git_am_opts.nr || options.type == REBASE_APPLY) {
		/* all am options except -q are compatible only with --apply */
		प्रति (i = options.git_am_opts.nr - 1; i >= 0; i--)
			यदि (वाक्यं_तुलय(options.git_am_opts.v[i], "-q"))
				भङ्ग;

		यदि (i >= 0 || options.type == REBASE_APPLY) {
			यदि (is_merge(&options))
				die(_("apply options and merge options "
					  "cannot be used together"));
			अन्यथा यदि (options.rebase_merges == -1 && options.config_rebase_merges == 1)
				die(_("apply options are incompatible with rebase.rebaseMerges.  Consider adding --no-rebase-merges"));
			अन्यथा यदि (options.update_refs == -1 && options.config_update_refs == 1)
				die(_("apply options are incompatible with rebase.updateRefs.  Consider adding --no-update-refs"));
			अन्यथा
				options.type = REBASE_APPLY;
		}
	}

	यदि (options.update_refs == 1)
		imply_merge(&options, "--update-refs");
	options.update_refs = (options.update_refs >= 0) ? options.update_refs :
			     ((options.config_update_refs >= 0) ? options.config_update_refs : 0);

	यदि (options.rebase_merges == 1)
		imply_merge(&options, "--rebase-merges");
	options.rebase_merges = (options.rebase_merges >= 0) ? options.rebase_merges :
				((options.config_rebase_merges >= 0) ? options.config_rebase_merges : 0);

	यदि (options.autosquash == 1) {
		imply_merge(&options, "--autosquash");
	} अन्यथा यदि (options.autosquash == -1) {
		options.autosquash =
			options.config_autosquash &&
			(options.flags & REBASE_INTERACTIVE_EXPLICIT);
	}

	यदि (options.type == REBASE_UNSPECIFIED) {
		यदि (!वाक्यं_तुलय(options.default_backend, "merge"))
			options.type = REBASE_MERGE;
		अन्यथा यदि (!वाक्यं_तुलय(options.default_backend, "apply"))
			options.type = REBASE_APPLY;
		अन्यथा
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

	यदि (options.रिक्तानि == रिक्तानि_UNSPECIFIED) {
		यदि (options.flags & REBASE_INTERACTIVE_EXPLICIT)
			options.रिक्तानि = रिक्तानि_STOP;
		अन्यथा यदि (options.exec.nr > 0)
			options.रिक्तानि = रिक्तानि_KEEP;
		अन्यथा
			options.रिक्तानि = रिक्तानि_DROP;
	}
	यदि (reschedule_failed_exec > 0 && !is_merge(&options))
		die(_("--reschedule-failed-exec requires "
		      "--exec or --interactive"));
	यदि (reschedule_failed_exec >= 0)
		options.reschedule_failed_exec = reschedule_failed_exec;

	यदि (options.signoff) {
		strvec_प्रक्षिप(&options.git_am_opts, "--signoff");
		options.flags |= REBASE_FORCE;
	}

	यदि (!options.मूलम्) {
		यदि (argc < 1) {
			संरचना branch *branch;

			branch = branch_get(रिक्त);
			options.upstream_name = branch_get_upstream(branch,
								    रिक्त);
			यदि (!options.upstream_name)
				error_on_missing_default_upstream();
			यदि (options.fork_point < 0)
				options.fork_point = 1;
		} अन्यथा {
			options.upstream_name = argv[0];
			argc--;
			argv++;
			यदि (!वाक्यं_तुलय(options.upstream_name, "-"))
				options.upstream_name = "@{-1}";
		}
		options.upstream =
			lookup_commit_reference_by_name(options.upstream_name);
		यदि (!options.upstream)
			die(_("invalid upstream '%s'"), options.upstream_name);
		options.upstream_arg = options.upstream_name;
	} अन्यथा {
		यदि (!options.onto_name) {
			यदि (commit_वृक्षः("", 0, the_hash_algo->रिक्तानि_वृक्षः, रिक्त,
					&squash_onto, रिक्त, रिक्त) < 0)
				die(_("Could not create new root commit"));
			options.squash_onto = &squash_onto;
			options.onto_name = squash_onto_name =
				xstrdup(oid_गन्तव्यम्_hex(&squash_onto));
		} अन्यथा
			options.मूलम्_with_onto = 1;

		options.upstream_name = रिक्त;
		options.upstream = रिक्त;
		यदि (argc > 1)
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
	यदि (argc == 1) {
		/* Is it "rebase other branchname" or "rebase other commit"? */
		संरचना object_id branch_oid;
		branch_name = argv[0];
		options.switch_गन्तव्यम् = argv[0];

		/* Is it a local branch? */
		strbuf_reset(&buf);
		strbuf_addf(&buf, "refs/heads/%s", branch_name);
		यदि (!refs_read_ref(get_main_ref_store(the_repository), buf.buf, &branch_oid)) {
			die_if_checked_बहिर्गमनम्(buf.buf, 1);
			options.शीर्ष_name = xstrdup(buf.buf);
			options.orig_शीर्ष =
				lookup_commit_object(the_repository,
						     &branch_oid);
		/* If not is it a valid ref (branch or commit)? */
		} अन्यथा {
			options.orig_शीर्ष =
				lookup_commit_reference_by_name(branch_name);
			options.शीर्ष_name = रिक्त;
		}
		यदि (!options.orig_शीर्ष)
			die(_("no such branch/commit '%s'"), branch_name);
	} अन्यथा यदि (argc == 0) {
		/* Do not need to switch branches, we are already on it. */
		options.शीर्ष_name =
			xstrdup_or_null(refs_resolve_ref_unsafe(get_main_ref_store(the_repository), "HEAD", 0, रिक्त,
								&flags));
		यदि (!options.शीर्ष_name)
			die(_("No such ref: %s"), "HEAD");
		यदि (flags & REF_ISSYMREF) {
			यदि (!skip_उपसर्गः(options.शीर्ष_name,
					 "refs/heads/", &branch_name))
				branch_name = options.शीर्ष_name;

		} अन्यथा {
			FREE_AND_NULL(options.शीर्ष_name);
			branch_name = "HEAD";
		}
		options.orig_शीर्ष = lookup_commit_reference_by_name("HEAD");
		यदि (!options.orig_शीर्ष)
			die(_("Could not resolve HEAD to a commit"));
	} अन्यथा
		BUG("unexpected number of arguments left to parse");

	/* Make sure the branch to rebase onto is valid. */
	यदि (keep_आधारः) {
		strbuf_reset(&buf);
		strbuf_addstr(&buf, options.upstream_name);
		strbuf_addstr(&buf, "...");
		strbuf_addstr(&buf, branch_name);
		options.onto_name = keep_आधारः_onto_name = xstrdup(buf.buf);
	} अन्यथा यदि (!options.onto_name)
		options.onto_name = options.upstream_name;
	यदि (वाक्ये_अन्विष(options.onto_name, "...")) {
		यदि (repo_get_oid_mb(the_repository, options.onto_name, &branch_आधारः) < 0) {
			यदि (keep_आधारः)
				die(_("'%s': need exactly one merge base with branch"),
				    options.upstream_name);
			अन्यथा
				die(_("'%s': need exactly one merge base"),
				    options.onto_name);
		}
		options.onto = lookup_commit_or_die(&branch_आधारः,
						    options.onto_name);
	} अन्यथा {
		options.onto =
			lookup_commit_reference_by_name(options.onto_name);
		यदि (!options.onto)
			die(_("Does not point to a valid commit '%s'"),
				options.onto_name);
		fill_branch_आधारः(&options, &branch_आधारः);
	}

	यदि (keep_आधारः && options.reapply_cherry_picks)
		options.upstream = options.onto;

	यदि (options.fork_point > 0)
		options.restrict_revision =
			get_fork_point(options.upstream_name, options.orig_शीर्ष);

	यदि (repo_read_सूचकांक(the_repository) < 0)
		die(_("could not read index"));

	यदि (options.autostash)
		create_autostash(the_repository,
				 state_dir_मार्गः("autostash", &options));


	यदि (require_clean_work_वृक्षः(the_repository, "rebase",
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
	यदि (allow_preemptive_ff &&
	    can_fast_forward(options.onto, options.upstream, options.restrict_revision,
			     options.orig_शीर्ष, &branch_आधारः)) {
		पूर्णाङ्क flag;

		यदि (!(options.flags & REBASE_FORCE)) {
			/* Lazily switch to the target branch if needed... */
			यदि (options.switch_गन्तव्यम्) {
				ret = checkout_ऊर्ध्वम्_गन्तव्यम्_date(&options);
				यदि (ret)
					गच्छ cleanup_autostash;
			}

			यदि (!(options.flags & REBASE_NO_QUIET))
				; /* be quiet */
			अन्यथा यदि (!वाक्यं_तुलय(branch_name, "HEAD") &&
				 refs_resolve_ref_unsafe(get_main_ref_store(the_repository), "HEAD", 0, रिक्त, &flag))
				पङ्क्तिं_लिख(_("HEAD is up to date."));
			अन्यथा
				प्रारूप_लिख(_("Current branch %s is up to date.\n"),
				       branch_name);
			ret = finish_rebase(&options);
			गच्छ cleanup;
		} अन्यथा यदि (!(options.flags & REBASE_NO_QUIET))
			; /* be quiet */
		अन्यथा यदि (!वाक्यं_तुलय(branch_name, "HEAD") &&
			 refs_resolve_ref_unsafe(get_main_ref_store(the_repository), "HEAD", 0, रिक्त, &flag))
			पङ्क्तिं_लिख(_("HEAD is up to date, rebase forced."));
		अन्यथा
			प्रारूप_लिख(_("Current branch %s is up to date, rebase "
				 "forced.\n"), branch_name);
	}

	/* If a hook exists, give it a chance to interrupt*/
	यदि (!ok_गन्तव्यम्_skip_pre_rebase &&
	    सततम्_hooks_l(the_repository, "pre-rebase", options.upstream_arg,
			argc ? argv[0] : रिक्त, रिक्त)) {
		ret = error(_("The pre-rebase hook refused to rebase."));
		गच्छ cleanup_autostash;
	}

	यदि (options.flags & REBASE_DIFFSTAT) {
		संरचना diff_options opts;

		यदि (options.flags & REBASE_VERBOSE) {
			यदि (is_null_oid(&branch_आधारः))
				प्रारूप_लिख(_("Changes to %s:\n"),
				       oid_गन्तव्यम्_hex(&options.onto->object.oid));
			अन्यथा
				प्रारूप_लिख(_("Changes from %s to %s:\n"),
				       oid_गन्तव्यम्_hex(&branch_आधारः),
				       oid_गन्तव्यम्_hex(&options.onto->object.oid));
		}

		/* We want color (if set), but no pager */
		repo_diff_सज्जता(the_repository, &opts);
		init_diffstat_widths(&opts);
		opts.output_format |=
			DIFF_FORMAT_SUMMARY | DIFF_FORMAT_DIFFSTAT;
		opts.detect_rename = DIFF_DETECT_RENAME;
		diff_सज्जता_समाप्तम्(&opts);
		diff_वृक्षः_oid(is_null_oid(&branch_आधारः) ?
			      the_hash_algo->रिक्तानि_वृक्षः : &branch_आधारः,
			      &options.onto->object.oid, "", &opts);
		diffcore_std(&opts);
		diff_flush(&opts);
	}

	यदि (is_merge(&options))
		गच्छ सततम्_rebase;

	/* Detach HEAD and reset the tree */
	यदि (options.flags & REBASE_NO_QUIET)
		प्रारूप_लिख(_("First, rewinding head to replay your work on top of "
			 "it...\n"));

	strbuf_addf(&सन्देशः, "%s (start): checkout %s",
		    options.reflog_action, options.onto_name);
	ropts.oid = &options.onto->object.oid;
	ropts.orig_शीर्ष = &options.orig_शीर्ष->object.oid;
	ropts.flags = RESET_WORKING_वृक्षः_DETACH |
		      RESET_WORKING_वृक्षः_UPDATE_शीर्ष |
		      RESET_WORKING_वृक्षः_UPDATE_ORIG_शीर्ष |
		      RESET_WORKING_वृक्षः_सततम्_POST_CHECKOUT_HOOK;
	ropts.शीर्ष_सन्देशः = सन्देशः.buf;
	ropts.default_reflog_action = options.reflog_action;
	यदि (reset_working_वृक्षः(the_repository, &ropts)) {
		ret = error(_("Could not detach HEAD"));
		गच्छ cleanup_autostash;
	}

	/*
	 * If the onto is a proper descendant of the tip of the branch, then
	 * we just fast-forwarded.
	 */
	यदि (oideq(&branch_आधारः, &options.orig_शीर्ष->object.oid)) {
		प्रारूप_लिख(_("Fast-forwarded %s to %s.\n"),
			branch_name, options.onto_name);
		move_गन्तव्यम्_original_branch(&options);
		ret = finish_rebase(&options);
		गच्छ cleanup;
	}

	strbuf_addf(&revisions, "%s..%s",
		    options.मूलम् ? oid_गन्तव्यम्_hex(&options.onto->object.oid) :
		    (options.restrict_revision ?
		     oid_गन्तव्यम्_hex(&options.restrict_revision->object.oid) :
		     oid_गन्तव्यम्_hex(&options.upstream->object.oid)),
		    oid_गन्तव्यम्_hex(&options.orig_शीर्ष->object.oid));

	options.revisions = revisions.buf;

सततम्_rebase:
	ret = सततम्_specific_rebase(&options);

cleanup:
	strbuf_release(&buf);
	strbuf_release(&सन्देशः);
	strbuf_release(&revisions);
	rebase_options_release(&options);
	मुक्त(squash_onto_name);
	मुक्त(keep_आधारः_onto_name);
	प्रत्यागम !!ret;

cleanup_autostash:
	ret |= !!cleanup_autostash(&options);
	गच्छ cleanup;
}
