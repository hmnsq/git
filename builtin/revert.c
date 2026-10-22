#define USE_THE_REPOSITORY_VARIABLE

#അന്തര്ഭാവ "git-compat-util.h"
#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "diff.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "revision.h"
#അന്തര്ഭാവ "rerere.h"
#അന്തര്ഭാവ "run-command.h"
#അന്തര്ഭാവ "sequencer.h"
#അന്തര്ഭാവ "branch.h"

/*
 * This implements the builtins revert and cherry-pick.
 *
 * Copyright (c) 2007 Johannes E. Schindelin
 *
 * Based on git-revert.sh, which is
 *
 * Copyright (c) 2005 Linus Torvalds
 * Copyright (c) 2005 Junio C Hamano
 */

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം revert_usage[] = {
	N_("git revert [--[no-]edit] [-n] [-m <parent-number>] [-s] [-S[<keyid>]] <commit>..."),
	N_("git revert (--continue | --skip | --abort | --quit)"),
	ശൂന്യ
};

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം cherry_pick_usage[] = {
	N_("git cherry-pick [--edit] [-n] [-m <parent-number>] [-s] [-x] [--ff]\n"
	   "                [-S[<keyid>]] <commit>..."),
	N_("git cherry-pick (--continue | --skip | --abort | --quit)"),
	ശൂന്യ
};

സ്ഥിതം സ്ഥിരം അക്ഷരം *action_name(സ്ഥിരം ഘടന replay_opts *opts)
{
	തിരികെ opts->action == REPLAY_REVERT ? "revert" : "cherry-pick";
}

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം *revert_or_cherry_pick_usage(ഘടന replay_opts *opts)
{
	തിരികെ opts->action == REPLAY_REVERT ? revert_usage : cherry_pick_usage;
}

എണ്ണൽ empty_action {
	EMPTY_COMMIT_UNSPECIFIED = -1,
	STOP_ON_EMPTY_COMMIT,      /* output errors and stop in the middle of a cherry-pick */
	DROP_EMPTY_COMMIT,         /* skip with a notice message */
	KEEP_EMPTY_COMMIT,         /* keep recording as empty commits */
};

സ്ഥിതം പൂർണ്ണസംഖ്യ parse_opt_empty(സ്ഥിരം ഘടന option *opt, സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	പൂർണ്ണസംഖ്യ *opt_മൂല്യം = opt->മൂല്യം;

	BUG_ON_OPT_NEG(unset);

	എങ്കിൽ (!strcmp(arg, "stop"))
		*opt_മൂല്യം = STOP_ON_EMPTY_COMMIT;
	അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(arg, "drop"))
		*opt_മൂല്യം = DROP_EMPTY_COMMIT;
	അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(arg, "keep"))
		*opt_മൂല്യം = KEEP_EMPTY_COMMIT;
	അല്ലെങ്കിൽ
		തിരികെ error(_("invalid value for '%s': '%s'"), "--empty", arg);

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ option_parse_m(സ്ഥിരം ഘടന option *opt,
			  സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	ഘടന replay_opts *replay = opt->മൂല്യം;
	അക്ഷരം *end;

	എങ്കിൽ (unset) {
		replay->mainline = 0;
		തിരികെ 0;
	}

	replay->mainline = strtol(arg, &end, 10);
	എങ്കിൽ (*end || replay->mainline <= 0)
		തിരികെ error(_("option `%s' expects a number greater than zero"),
			     opt->long_name);

	തിരികെ 0;
}

LAST_ARG_MUST_BE_NULL
സ്ഥിതം ശൂന്യം verify_opt_compatible(സ്ഥിരം അക്ഷരം *me, സ്ഥിരം അക്ഷരം *base_opt, ...)
{
	സ്ഥിരം അക്ഷരം *this_opt;
	va_list ap;

	va_start(ap, base_opt);
	ആകുമ്പോൾ ((this_opt = va_arg(ap, സ്ഥിരം അക്ഷരം *))) {
		എങ്കിൽ (va_arg(ap, പൂർണ്ണസംഖ്യ))
			നിർത്തുക;
	}
	va_end(ap);

	എങ്കിൽ (this_opt)
		die(_("%s: %s cannot be used with %s"), me, this_opt, base_opt);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ run_sequencer(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
			 ഘടന replay_opts *opts)
{
	സ്ഥിരം അക്ഷരം * സ്ഥിരം * usage_str = revert_or_cherry_pick_usage(opts);
	സ്ഥിരം അക്ഷരം *me = action_name(opts);
	സ്ഥിരം അക്ഷരം *cleanup_arg = ശൂന്യ;
	സ്ഥിരം അക്ഷരം sentinel_മൂല്യം = 0; /* value not important */
	സ്ഥിരം അക്ഷരം *strategy = &sentinel_മൂല്യം;
	സ്ഥിരം അക്ഷരം *gpg_sign = &sentinel_മൂല്യം;
	എണ്ണൽ empty_action empty_opt = EMPTY_COMMIT_UNSPECIFIED;
	പൂർണ്ണസംഖ്യ cmd = 0, ret;
	ഘടന option base_options[] = {
		OPT_CMDMODE(0, "quit", &cmd, N_("end revert or cherry-pick sequence"), 'q'),
		OPT_CMDMODE(0, "continue", &cmd, N_("resume revert or cherry-pick sequence"), 'c'),
		OPT_CMDMODE(0, "abort", &cmd, N_("cancel revert or cherry-pick sequence"), 'a'),
		OPT_CMDMODE(0, "skip", &cmd, N_("skip current commit and continue"), 's'),
		OPT_CLEANUP(&cleanup_arg),
		OPT_BOOL('n', "no-commit", &opts->no_commit, N_("don't automatically commit")),
		OPT_BOOL('e', "edit", &opts->edit, N_("edit the commit message")),
		OPT_NOOP_NOARG('r', ശൂന്യ),
		OPT_BOOL('s', "signoff", &opts->signoff, N_("add a Signed-off-by trailer")),
		OPT_CALLBACK('m', "mainline", opts, N_("parent-number"),
			     N_("select mainline parent"), option_parse_m),
		OPT_RERERE_AUTOUPDATE(&opts->allow_rerere_auto),
		OPT_STRING(0, "strategy", &strategy, N_("strategy"), N_("merge strategy")),
		OPT_STRVEC('X', "strategy-option", &opts->xopts, N_("option"),
			N_("option for merge strategy")),
		{
			.type = OPTION_STRING,
			.short_name = 'S',
			.long_name = "gpg-sign",
			.മൂല്യം = &gpg_sign,
			.argh = N_("key-id"),
			.help = N_("GPG sign commit"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "",
		},
		OPT_END()
	};
	ഘടന option *options = base_options;

	എങ്കിൽ (opts->action == REPLAY_PICK) {
		ഘടന option cp_extra[] = {
			OPT_BOOL('x', ശൂന്യ, &opts->record_origin, N_("append commit name")),
			OPT_BOOL(0, "ff", &opts->allow_ff, N_("allow fast-forward")),
			OPT_BOOL(0, "allow-empty", &opts->allow_empty, N_("preserve initially empty commits")),
			OPT_BOOL(0, "allow-empty-message", &opts->allow_empty_message, N_("allow commits with empty messages")),
			OPT_BOOL(0, "keep-redundant-commits", &opts->keep_redundant_commits, N_("deprecated: use --empty=keep instead")),
			OPT_CALLBACK_F(0, "empty", &empty_opt, "(stop|drop|keep)",
				       N_("how to handle commits that become empty"),
				       PARSE_OPT_NONEG, parse_opt_empty),
			OPT_END(),
		};
		options = parse_options_concat(options, cp_extra);
	} അല്ലെങ്കിൽ എങ്കിൽ (opts->action == REPLAY_REVERT) {
		ഘടന option cp_extra[] = {
			OPT_BOOL(0, "reference", &opts->commit_use_reference,
				 N_("use the 'reference' format to refer to commits")),
			OPT_END(),
		};
		options = parse_options_concat(options, cp_extra);
	}

	argc = parse_options(argc, argv, prefix, options, usage_str,
			PARSE_OPT_KEEP_ARGV0 |
			PARSE_OPT_KEEP_UNKNOWN_OPT);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	എങ്കിൽ (opts->action == REPLAY_PICK) {
		opts->drop_redundant_commits = (empty_opt == DROP_EMPTY_COMMIT);
		opts->keep_redundant_commits = opts->keep_redundant_commits || (empty_opt == KEEP_EMPTY_COMMIT);
	}

	/* implies allow_empty */
	എങ്കിൽ (opts->keep_redundant_commits)
		opts->allow_empty = 1;

	എങ്കിൽ (cleanup_arg) {
		opts->default_സന്ദേശം_cleanup = get_cleanup_mode(cleanup_arg, 1);
		opts->explicit_cleanup = 1;
	}

	/* Check for incompatible command line arguments */
	എങ്കിൽ (cmd) {
		സ്ഥിരം അക്ഷരം *this_operation;
		എങ്കിൽ (cmd == 'q')
			this_operation = "--quit";
		അല്ലെങ്കിൽ എങ്കിൽ (cmd == 'c')
			this_operation = "--continue";
		അല്ലെങ്കിൽ എങ്കിൽ (cmd == 's')
			this_operation = "--skip";
		അല്ലെങ്കിൽ {
			പ്രതിജ്ഞാ(cmd == 'a');
			this_operation = "--abort";
		}

		verify_opt_compatible(me, this_operation,
				"--no-commit", opts->no_commit,
				"--signoff", opts->signoff,
				"--mainline", opts->mainline,
				"--strategy", opts->strategy ? 1 : 0,
				"--strategy-option", opts->xopts.nr ? 1 : 0,
				"-x", opts->record_origin,
				"--ff", opts->allow_ff,
				"--rerere-autoupdate", opts->allow_rerere_auto == RERERE_AUTOUPDATE,
				"--no-rerere-autoupdate", opts->allow_rerere_auto == RERERE_NOAUTOUPDATE,
				"--keep-redundant-commits", opts->keep_redundant_commits,
				"--empty", empty_opt != EMPTY_COMMIT_UNSPECIFIED,
				ശൂന്യ);
	}

	എങ്കിൽ (!opts->strategy && opts->default_strategy) {
		opts->strategy = opts->default_strategy;
		opts->default_strategy = ശൂന്യ;
	}

	എങ്കിൽ (opts->allow_ff)
		verify_opt_compatible(me, "--ff",
				"--signoff", opts->signoff,
				"--no-commit", opts->no_commit,
				"-x", opts->record_origin,
				"--edit", opts->edit > 0,
				ശൂന്യ);

	എങ്കിൽ (cmd) {
		opts->revs = ശൂന്യ;
	} അല്ലെങ്കിൽ {
		ഘടന setup_revision_opt s_r_opt;
		opts->revs = xmalloc(വലുപ്പം(*opts->revs));
		repo_init_revisions(the_repository, opts->revs, ശൂന്യ);
		opts->revs->no_walk = 1;
		opts->revs->unsorted_നിവേശം = 1;
		എങ്കിൽ (argc < 2)
			usage_with_options(usage_str, options);
		എങ്കിൽ (!strcmp(argv[1], "-"))
			argv[1] = "@{-1}";
		memset(&s_r_opt, 0, വലുപ്പം(s_r_opt));
		s_r_opt.assume_dashdash = 1;
		argc = setup_revisions(argc, argv, opts->revs, &s_r_opt);
	}

	എങ്കിൽ (argc > 1)
		usage_with_options(usage_str, options);

	/* These option values will be free()d */
	എങ്കിൽ (gpg_sign != &sentinel_മൂല്യം) {
		സ്വതന്ത്രം(opts->gpg_sign);
		opts->gpg_sign = xstrdup_or_null(gpg_sign);
	}
	എങ്കിൽ (strategy != &sentinel_മൂല്യം) {
		സ്വതന്ത്രം(opts->strategy);
		opts->strategy = xstrdup_or_null(strategy);
	}
	സ്വതന്ത്രം(options);

	എങ്കിൽ (cmd == 'q') {
		ret = sequencer_remove_state(opts);
		എങ്കിൽ (!ret)
			remove_branch_state(the_repository, 0);
		തിരികെ ret;
	}
	എങ്കിൽ (cmd == 'a')
		തിരികെ sequencer_rollback(the_repository, opts);
	എങ്കിൽ (cmd == 'c')
		ret = sequencer_continue(the_repository, opts);
	അല്ലെങ്കിൽ എങ്കിൽ (cmd == 's')
		ret = sequencer_skip(the_repository, opts);
	അല്ലെങ്കിൽ
		ret = sequencer_pick_revisions(the_repository, opts);
	എങ്കിൽ (!ret)
		run_auto_maintenance(the_repository, opts->quiet);
	തിരികെ ret;
}

പൂർണ്ണസംഖ്യ cmd_revert(പൂർണ്ണസംഖ്യ argc,
	       സ്ഥിരം അക്ഷരം **argv,
	       സ്ഥിരം അക്ഷരം *prefix,
	       ഘടന repository *repo UNUSED)
{
	ഘടന replay_opts opts = REPLAY_OPTS_INIT;
	പൂർണ്ണസംഖ്യ res;

#ifndef WITH_BREAKING_CHANGES
	warn_on_auto_comment_char = true;
#endif /* !WITH_BREAKING_CHANGES */
	opts.action = REPLAY_REVERT;
	sequencer_init_config(&opts);
	res = run_sequencer(argc, argv, prefix, &opts);
	എങ്കിൽ (res < 0)
		die(_("revert failed"));
	replay_opts_release(&opts);
	തിരികെ res;
}

പൂർണ്ണസംഖ്യ cmd_cherry_pick(പൂർണ്ണസംഖ്യ argc,
സ്ഥിരം അക്ഷരം **argv,
സ്ഥിരം അക്ഷരം *prefix,
ഘടന repository *repo UNUSED)
{
	ഘടന replay_opts opts = REPLAY_OPTS_INIT;
	പൂർണ്ണസംഖ്യ res;

#ifndef WITH_BREAKING_CHANGES
	warn_on_auto_comment_char = true;
#endif /* !WITH_BREAKING_CHANGES */
	opts.action = REPLAY_PICK;
	sequencer_init_config(&opts);
	res = run_sequencer(argc, argv, prefix, &opts);
	എങ്കിൽ (res < 0)
		die(_("cherry-pick failed"));
	replay_opts_release(&opts);
	തിരികെ res;
}
