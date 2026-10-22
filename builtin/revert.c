#परिभाषय USE_THE_REPOSITORY_VARIABLE

#अन्तर्भाव "git-compat-util.h"
#अन्तर्भाव "builtin.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "diff.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "revision.h"
#अन्तर्भाव "rerere.h"
#अन्तर्भाव "run-command.h"
#अन्तर्भाव "sequencer.h"
#अन्तर्भाव "branch.h"

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

स्थिर ध्रुव वर्ण * ध्रुव revert_usage[] = {
	N_("git revert [--[no-]edit] [-n] [-m <parent-number>] [-s] [-S[<keyid>]] <commit>..."),
	N_("git revert (--continue | --skip | --abort | --quit)"),
	रिक्त
};

स्थिर ध्रुव वर्ण * ध्रुव cherry_pick_usage[] = {
	N_("git cherry-pick [--edit] [-n] [-m <parent-number>] [-s] [-x] [--ff]\n"
	   "                [-S[<keyid>]] <commit>..."),
	N_("git cherry-pick (--continue | --skip | --abort | --quit)"),
	रिक्त
};

स्थिर ध्रुव वर्ण *action_name(ध्रुव संरचना replay_opts *opts)
{
	प्रत्यागम opts->action == REPLAY_REVERT ? "revert" : "cherry-pick";
}

स्थिर ध्रुव वर्ण * ध्रुव *revert_or_cherry_pick_usage(संरचना replay_opts *opts)
{
	प्रत्यागम opts->action == REPLAY_REVERT ? revert_usage : cherry_pick_usage;
}

गणना रिक्तानि_action {
	रिक्तानि_COMMIT_UNSPECIFIED = -1,
	STOP_ON_रिक्तानि_COMMIT,      /* output errors and stop in the middle of a cherry-pick */
	DROP_रिक्तानि_COMMIT,         /* skip with a notice message */
	KEEP_रिक्तानि_COMMIT,         /* keep recording as empty commits */
};

स्थिर पूर्णाङ्क parse_opt_रिक्तानि(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	पूर्णाङ्क *opt_मानम् = opt->मानम्;

	BUG_ON_OPT_NEG(unset);

	यदि (!वाक्यं_तुलय(arg, "stop"))
		*opt_मानम् = STOP_ON_रिक्तानि_COMMIT;
	अन्यथा यदि (!वाक्यं_तुलय(arg, "drop"))
		*opt_मानम् = DROP_रिक्तानि_COMMIT;
	अन्यथा यदि (!वाक्यं_तुलय(arg, "keep"))
		*opt_मानम् = KEEP_रिक्तानि_COMMIT;
	अन्यथा
		प्रत्यागम error(_("invalid value for '%s': '%s'"), "--empty", arg);

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क option_parse_m(ध्रुव संरचना option *opt,
			  ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना replay_opts *replay = opt->मानम्;
	वर्ण *अन्तः;

	यदि (unset) {
		replay->mainline = 0;
		प्रत्यागम 0;
	}

	replay->mainline = strtol(arg, &अन्तः, 10);
	यदि (*अन्तः || replay->mainline <= 0)
		प्रत्यागम error(_("option `%s' expects a number greater than zero"),
			     opt->long_name);

	प्रत्यागम 0;
}

चरमः_ARG_MUST_BE_NULL
स्थिर शून्य verify_opt_compatible(ध्रुव वर्ण *me, ध्रुव वर्ण *आधारः_opt, ...)
{
	ध्रुव वर्ण *this_opt;
	va_list ap;

	va_start(ap, आधारः_opt);
	यावत् ((this_opt = va_arg(ap, ध्रुव वर्ण *))) {
		यदि (va_arg(ap, पूर्णाङ्क))
			भङ्ग;
	}
	va_अन्तः(ap);

	यदि (this_opt)
		die(_("%s: %s cannot be used with %s"), me, this_opt, आधारः_opt);
}

स्थिर पूर्णाङ्क सततम्_sequencer(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
			 संरचना replay_opts *opts)
{
	ध्रुव वर्ण * ध्रुव * usage_str = revert_or_cherry_pick_usage(opts);
	ध्रुव वर्ण *me = action_name(opts);
	ध्रुव वर्ण *cleanup_arg = रिक्त;
	ध्रुव वर्ण sentinel_मानम् = 0; /* value not important */
	ध्रुव वर्ण *strategy = &sentinel_मानम्;
	ध्रुव वर्ण *gpg_sign = &sentinel_मानम्;
	गणना रिक्तानि_action रिक्तानि_opt = रिक्तानि_COMMIT_UNSPECIFIED;
	पूर्णाङ्क cmd = 0, ret;
	संरचना option आधारः_options[] = {
		OPT_CMDMODE(0, "quit", &cmd, N_("end revert or cherry-pick sequence"), 'q'),
		OPT_CMDMODE(0, "continue", &cmd, N_("resume revert or cherry-pick sequence"), 'c'),
		OPT_CMDMODE(0, "abort", &cmd, N_("cancel revert or cherry-pick sequence"), 'a'),
		OPT_CMDMODE(0, "skip", &cmd, N_("skip current commit and continue"), 's'),
		OPT_CLEANUP(&cleanup_arg),
		OPT_BOOL('n', "no-commit", &opts->no_commit, N_("don't automatically commit")),
		OPT_BOOL('e', "edit", &opts->edit, N_("edit the commit message")),
		OPT_NOOP_NOARG('r', रिक्त),
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
			.मानम् = &gpg_sign,
			.argh = N_("key-id"),
			.help = N_("GPG sign commit"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "",
		},
		OPT_अन्तः()
	};
	संरचना option *options = आधारः_options;

	यदि (opts->action == REPLAY_PICK) {
		संरचना option cp_extra[] = {
			OPT_BOOL('x', रिक्त, &opts->लेखः_origin, N_("append commit name")),
			OPT_BOOL(0, "ff", &opts->allow_ff, N_("allow fast-forward")),
			OPT_BOOL(0, "allow-empty", &opts->allow_रिक्तानि, N_("preserve initially empty commits")),
			OPT_BOOL(0, "allow-empty-message", &opts->allow_रिक्तानि_सन्देशः, N_("allow commits with empty messages")),
			OPT_BOOL(0, "keep-redundant-commits", &opts->keep_redundant_commits, N_("deprecated: use --empty=keep instead")),
			OPT_CALLBACK_F(0, "empty", &रिक्तानि_opt, "(stop|drop|keep)",
				       N_("how to handle commits that become empty"),
				       PARSE_OPT_NONEG, parse_opt_रिक्तानि),
			OPT_अन्तः(),
		};
		options = parse_options_concat(options, cp_extra);
	} अन्यथा यदि (opts->action == REPLAY_REVERT) {
		संरचना option cp_extra[] = {
			OPT_BOOL(0, "reference", &opts->commit_use_reference,
				 N_("use the 'reference' format to refer to commits")),
			OPT_अन्तः(),
		};
		options = parse_options_concat(options, cp_extra);
	}

	argc = parse_options(argc, argv, उपसर्गः, options, usage_str,
			PARSE_OPT_KEEP_ARGV0 |
			PARSE_OPT_KEEP_UNKNOWN_OPT);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_पूर्णम्_सूचकांक = 0;

	यदि (opts->action == REPLAY_PICK) {
		opts->drop_redundant_commits = (रिक्तानि_opt == DROP_रिक्तानि_COMMIT);
		opts->keep_redundant_commits = opts->keep_redundant_commits || (रिक्तानि_opt == KEEP_रिक्तानि_COMMIT);
	}

	/* implies allow_empty */
	यदि (opts->keep_redundant_commits)
		opts->allow_रिक्तानि = 1;

	यदि (cleanup_arg) {
		opts->default_सन्देशः_cleanup = get_cleanup_बहुलकम्(cleanup_arg, 1);
		opts->explicit_cleanup = 1;
	}

	/* Check for incompatible command line arguments */
	यदि (cmd) {
		ध्रुव वर्ण *this_operation;
		यदि (cmd == 'q')
			this_operation = "--quit";
		अन्यथा यदि (cmd == 'c')
			this_operation = "--continue";
		अन्यथा यदि (cmd == 's')
			this_operation = "--skip";
		अन्यथा {
			प्रतिज्ञा(cmd == 'a');
			this_operation = "--abort";
		}

		verify_opt_compatible(me, this_operation,
				"--no-commit", opts->no_commit,
				"--signoff", opts->signoff,
				"--mainline", opts->mainline,
				"--strategy", opts->strategy ? 1 : 0,
				"--strategy-option", opts->xopts.nr ? 1 : 0,
				"-x", opts->लेखः_origin,
				"--ff", opts->allow_ff,
				"--rerere-autoupdate", opts->allow_rerere_auto == RERERE_AUTOUPDATE,
				"--no-rerere-autoupdate", opts->allow_rerere_auto == RERERE_NOAUTOUPDATE,
				"--keep-redundant-commits", opts->keep_redundant_commits,
				"--empty", रिक्तानि_opt != रिक्तानि_COMMIT_UNSPECIFIED,
				रिक्त);
	}

	यदि (!opts->strategy && opts->default_strategy) {
		opts->strategy = opts->default_strategy;
		opts->default_strategy = रिक्त;
	}

	यदि (opts->allow_ff)
		verify_opt_compatible(me, "--ff",
				"--signoff", opts->signoff,
				"--no-commit", opts->no_commit,
				"-x", opts->लेखः_origin,
				"--edit", opts->edit > 0,
				रिक्त);

	यदि (cmd) {
		opts->revs = रिक्त;
	} अन्यथा {
		संरचना सज्जता_revision_opt s_r_opt;
		opts->revs = xmalloc(परिमाण(*opts->revs));
		repo_init_revisions(the_repository, opts->revs, रिक्त);
		opts->revs->no_walk = 1;
		opts->revs->unsorted_निवेशः = 1;
		यदि (argc < 2)
			usage_with_options(usage_str, options);
		यदि (!वाक्यं_तुलय(argv[1], "-"))
			argv[1] = "@{-1}";
		स्मृतिं_पूरय(&s_r_opt, 0, परिमाण(s_r_opt));
		s_r_opt.assume_dashdash = 1;
		argc = सज्जता_revisions(argc, argv, opts->revs, &s_r_opt);
	}

	यदि (argc > 1)
		usage_with_options(usage_str, options);

	/* These option values will be free()d */
	यदि (gpg_sign != &sentinel_मानम्) {
		मुक्त(opts->gpg_sign);
		opts->gpg_sign = xstrdup_or_null(gpg_sign);
	}
	यदि (strategy != &sentinel_मानम्) {
		मुक्त(opts->strategy);
		opts->strategy = xstrdup_or_null(strategy);
	}
	मुक्त(options);

	यदि (cmd == 'q') {
		ret = sequencer_remove_state(opts);
		यदि (!ret)
			remove_branch_state(the_repository, 0);
		प्रत्यागम ret;
	}
	यदि (cmd == 'a')
		प्रत्यागम sequencer_rollback(the_repository, opts);
	यदि (cmd == 'c')
		ret = sequencer_continue(the_repository, opts);
	अन्यथा यदि (cmd == 's')
		ret = sequencer_skip(the_repository, opts);
	अन्यथा
		ret = sequencer_pick_revisions(the_repository, opts);
	यदि (!ret)
		सततम्_auto_maintenance(the_repository, opts->quiet);
	प्रत्यागम ret;
}

पूर्णाङ्क cmd_revert(पूर्णाङ्क argc,
	       ध्रुव वर्ण **argv,
	       ध्रुव वर्ण *उपसर्गः,
	       संरचना repository *repo अप्रयुक्तम्)
{
	संरचना replay_opts opts = REPLAY_OPTS_INIT;
	पूर्णाङ्क res;

#यदि_अपरिभाषितम् WITH_BREAKING_CHANGES
	warn_on_auto_comment_char = true;
#यदि_अन्तः /* !WITH_BREAKING_CHANGES */
	opts.action = REPLAY_REVERT;
	sequencer_init_config(&opts);
	res = सततम्_sequencer(argc, argv, उपसर्गः, &opts);
	यदि (res < 0)
		die(_("revert failed"));
	replay_opts_release(&opts);
	प्रत्यागम res;
}

पूर्णाङ्क cmd_cherry_pick(पूर्णाङ्क argc,
ध्रुव वर्ण **argv,
ध्रुव वर्ण *उपसर्गः,
संरचना repository *repo अप्रयुक्तम्)
{
	संरचना replay_opts opts = REPLAY_OPTS_INIT;
	पूर्णाङ्क res;

#यदि_अपरिभाषितम् WITH_BREAKING_CHANGES
	warn_on_auto_comment_char = true;
#यदि_अन्तः /* !WITH_BREAKING_CHANGES */
	opts.action = REPLAY_PICK;
	sequencer_init_config(&opts);
	res = सततम्_sequencer(argc, argv, उपसर्गः, &opts);
	यदि (res < 0)
		die(_("cherry-pick failed"));
	replay_opts_release(&opts);
	प्रत्यागम res;
}
