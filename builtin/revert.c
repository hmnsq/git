#నిర్వచించు USE_THE_REPOSITORY_VARIABLE

#అన్తర్భావ "git-compat-util.h"
#అన్తర్భావ "builtin.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "diff.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "revision.h"
#అన్తర్భావ "rerere.h"
#అన్తర్భావ "run-command.h"
#అన్తర్భావ "sequencer.h"
#అన్తర్భావ "branch.h"

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

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి revert_usage[] = {
	N_("git revert [--[no-]edit] [-n] [-m <parent-number>] [-s] [-S[<keyid>]] <commit>..."),
	N_("git revert (--continue | --skip | --abort | --quit)"),
	ఖాళీ
};

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి cherry_pick_usage[] = {
	N_("git cherry-pick [--edit] [-n] [-m <parent-number>] [-s] [-x] [--ff]\n"
	   "                [-S[<keyid>]] <commit>..."),
	N_("git cherry-pick (--continue | --skip | --abort | --quit)"),
	ఖాళీ
};

స్థిర స్థిరరాశి అక్షరం *action_name(స్థిరరాశి నిర్మాణం replay_opts *opts)
{
	తిరిగి opts->action == REPLAY_REVERT ? "revert" : "cherry-pick";
}

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి *revert_or_cherry_pick_usage(నిర్మాణం replay_opts *opts)
{
	తిరిగి opts->action == REPLAY_REVERT ? revert_usage : cherry_pick_usage;
}

గణన ఖాళీలు_action {
	ఖాళీలు_COMMIT_UNSPECIFIED = -1,
	STOP_ON_ఖాళీలు_COMMIT,      /* output errors and stop in the middle of a cherry-pick */
	DROP_ఖాళీలు_COMMIT,         /* skip with a notice message */
	KEEP_ఖాళీలు_COMMIT,         /* keep recording as empty commits */
};

స్థిర పూర్ణాంకం parse_opt_ఖాళీలు(స్థిరరాశి నిర్మాణం option *opt, స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	పూర్ణాంకం *opt_విలువ = opt->విలువ;

	BUG_ON_OPT_NEG(unset);

	ఒకవేళ (!strcmp(arg, "stop"))
		*opt_విలువ = STOP_ON_ఖాళీలు_COMMIT;
	లేకపోతే ఒకవేళ (!strcmp(arg, "drop"))
		*opt_విలువ = DROP_ఖాళీలు_COMMIT;
	లేకపోతే ఒకవేళ (!strcmp(arg, "keep"))
		*opt_విలువ = KEEP_ఖాళీలు_COMMIT;
	లేకపోతే
		తిరిగి error(_("invalid value for '%s': '%s'"), "--empty", arg);

	తిరిగి 0;
}

స్థిర పూర్ణాంకం option_parse_m(స్థిరరాశి నిర్మాణం option *opt,
			  స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	నిర్మాణం replay_opts *replay = opt->విలువ;
	అక్షరం *చివర;

	ఒకవేళ (unset) {
		replay->mainline = 0;
		తిరిగి 0;
	}

	replay->mainline = strtol(arg, &చివర, 10);
	ఒకవేళ (*చివర || replay->mainline <= 0)
		తిరిగి error(_("option `%s' expects a number greater than zero"),
			     opt->long_name);

	తిరిగి 0;
}

చివరి_ARG_MUST_BE_NULL
స్థిర శూన్య verify_opt_compatible(స్థిరరాశి అక్షరం *me, స్థిరరాశి అక్షరం *పునాది_opt, ...)
{
	స్థిరరాశి అక్షరం *this_opt;
	va_list ap;

	va_ప్రారంభం(ap, పునాది_opt);
	అయితే ((this_opt = va_arg(ap, స్థిరరాశి అక్షరం *))) {
		ఒకవేళ (va_arg(ap, పూర్ణాంకం))
			ఆపు;
	}
	va_చివర(ap);

	ఒకవేళ (this_opt)
		die(_("%s: %s cannot be used with %s"), me, this_opt, పునాది_opt);
}

స్థిర పూర్ణాంకం నిరంతరం_sequencer(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
			 నిర్మాణం replay_opts *opts)
{
	స్థిరరాశి అక్షరం * స్థిరరాశి * usage_str = revert_or_cherry_pick_usage(opts);
	స్థిరరాశి అక్షరం *me = action_name(opts);
	స్థిరరాశి అక్షరం *cleanup_arg = ఖాళీ;
	స్థిరరాశి అక్షరం sentinel_విలువ = 0; /* value not important */
	స్థిరరాశి అక్షరం *strategy = &sentinel_విలువ;
	స్థిరరాశి అక్షరం *gpg_sign = &sentinel_విలువ;
	గణన ఖాళీలు_action ఖాళీలు_opt = ఖాళీలు_COMMIT_UNSPECIFIED;
	పూర్ణాంకం cmd = 0, ret;
	నిర్మాణం option పునాది_options[] = {
		OPT_CMDMODE(0, "quit", &cmd, N_("end revert or cherry-pick sequence"), 'q'),
		OPT_CMDMODE(0, "continue", &cmd, N_("resume revert or cherry-pick sequence"), 'c'),
		OPT_CMDMODE(0, "abort", &cmd, N_("cancel revert or cherry-pick sequence"), 'a'),
		OPT_CMDMODE(0, "skip", &cmd, N_("skip current commit and continue"), 's'),
		OPT_CLEANUP(&cleanup_arg),
		OPT_BOOL('n', "no-commit", &opts->no_commit, N_("don't automatically commit")),
		OPT_BOOL('e', "edit", &opts->edit, N_("edit the commit message")),
		OPT_NOOP_NOARG('r', ఖాళీ),
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
			.విలువ = &gpg_sign,
			.argh = N_("key-id"),
			.help = N_("GPG sign commit"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "",
		},
		OPT_చివర()
	};
	నిర్మాణం option *options = పునాది_options;

	ఒకవేళ (opts->action == REPLAY_PICK) {
		నిర్మాణం option cp_extra[] = {
			OPT_BOOL('x', ఖాళీ, &opts->రికార్డు_origin, N_("append commit name")),
			OPT_BOOL(0, "ff", &opts->allow_ff, N_("allow fast-forward")),
			OPT_BOOL(0, "allow-empty", &opts->allow_ఖాళీలు, N_("preserve initially empty commits")),
			OPT_BOOL(0, "allow-empty-message", &opts->allow_ఖాళీలు_సందేశం, N_("allow commits with empty messages")),
			OPT_BOOL(0, "keep-redundant-commits", &opts->keep_redundant_commits, N_("deprecated: use --empty=keep instead")),
			OPT_CALLBACK_F(0, "empty", &ఖాళీలు_opt, "(stop|drop|keep)",
				       N_("how to handle commits that become empty"),
				       PARSE_OPT_NONEG, parse_opt_ఖాళీలు),
			OPT_చివర(),
		};
		options = parse_options_concat(options, cp_extra);
	} లేకపోతే ఒకవేళ (opts->action == REPLAY_REVERT) {
		నిర్మాణం option cp_extra[] = {
			OPT_BOOL(0, "reference", &opts->commit_use_reference,
				 N_("use the 'reference' format to refer to commits")),
			OPT_చివర(),
		};
		options = parse_options_concat(options, cp_extra);
	}

	argc = parse_options(argc, argv, ఉపసర్గ, options, usage_str,
			PARSE_OPT_KEEP_ARGV0 |
			PARSE_OPT_KEEP_UNKNOWN_OPT);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_నిండిన_సూచీ = 0;

	ఒకవేళ (opts->action == REPLAY_PICK) {
		opts->drop_redundant_commits = (ఖాళీలు_opt == DROP_ఖాళీలు_COMMIT);
		opts->keep_redundant_commits = opts->keep_redundant_commits || (ఖాళీలు_opt == KEEP_ఖాళీలు_COMMIT);
	}

	/* implies allow_empty */
	ఒకవేళ (opts->keep_redundant_commits)
		opts->allow_ఖాళీలు = 1;

	ఒకవేళ (cleanup_arg) {
		opts->default_సందేశం_cleanup = get_cleanup_బహుళకం(cleanup_arg, 1);
		opts->explicit_cleanup = 1;
	}

	/* Check for incompatible command line arguments */
	ఒకవేళ (cmd) {
		స్థిరరాశి అక్షరం *this_operation;
		ఒకవేళ (cmd == 'q')
			this_operation = "--quit";
		లేకపోతే ఒకవేళ (cmd == 'c')
			this_operation = "--continue";
		లేకపోతే ఒకవేళ (cmd == 's')
			this_operation = "--skip";
		లేకపోతే {
			ప్రతిజ్ఞా(cmd == 'a');
			this_operation = "--abort";
		}

		verify_opt_compatible(me, this_operation,
				"--no-commit", opts->no_commit,
				"--signoff", opts->signoff,
				"--mainline", opts->mainline,
				"--strategy", opts->strategy ? 1 : 0,
				"--strategy-option", opts->xopts.nr ? 1 : 0,
				"-x", opts->రికార్డు_origin,
				"--ff", opts->allow_ff,
				"--rerere-autoupdate", opts->allow_rerere_auto == RERERE_AUTOUPDATE,
				"--no-rerere-autoupdate", opts->allow_rerere_auto == RERERE_NOAUTOUPDATE,
				"--keep-redundant-commits", opts->keep_redundant_commits,
				"--empty", ఖాళీలు_opt != ఖాళీలు_COMMIT_UNSPECIFIED,
				ఖాళీ);
	}

	ఒకవేళ (!opts->strategy && opts->default_strategy) {
		opts->strategy = opts->default_strategy;
		opts->default_strategy = ఖాళీ;
	}

	ఒకవేళ (opts->allow_ff)
		verify_opt_compatible(me, "--ff",
				"--signoff", opts->signoff,
				"--no-commit", opts->no_commit,
				"-x", opts->రికార్డు_origin,
				"--edit", opts->edit > 0,
				ఖాళీ);

	ఒకవేళ (cmd) {
		opts->revs = ఖాళీ;
	} లేకపోతే {
		నిర్మాణం అమరిక_revision_opt s_r_opt;
		opts->revs = xmalloc(పరిమాణం(*opts->revs));
		repo_init_revisions(the_repository, opts->revs, ఖాళీ);
		opts->revs->no_walk = 1;
		opts->revs->unsorted_ఆదానం = 1;
		ఒకవేళ (argc < 2)
			usage_with_options(usage_str, options);
		ఒకవేళ (!strcmp(argv[1], "-"))
			argv[1] = "@{-1}";
		memset(&s_r_opt, 0, పరిమాణం(s_r_opt));
		s_r_opt.assume_dashdash = 1;
		argc = అమరిక_revisions(argc, argv, opts->revs, &s_r_opt);
	}

	ఒకవేళ (argc > 1)
		usage_with_options(usage_str, options);

	/* These option values will be free()d */
	ఒకవేళ (gpg_sign != &sentinel_విలువ) {
		విడుదల(opts->gpg_sign);
		opts->gpg_sign = xstrdup_or_null(gpg_sign);
	}
	ఒకవేళ (strategy != &sentinel_విలువ) {
		విడుదల(opts->strategy);
		opts->strategy = xstrdup_or_null(strategy);
	}
	విడుదల(options);

	ఒకవేళ (cmd == 'q') {
		ret = sequencer_remove_state(opts);
		ఒకవేళ (!ret)
			remove_branch_state(the_repository, 0);
		తిరిగి ret;
	}
	ఒకవేళ (cmd == 'a')
		తిరిగి sequencer_rollback(the_repository, opts);
	ఒకవేళ (cmd == 'c')
		ret = sequencer_continue(the_repository, opts);
	లేకపోతే ఒకవేళ (cmd == 's')
		ret = sequencer_skip(the_repository, opts);
	లేకపోతే
		ret = sequencer_pick_revisions(the_repository, opts);
	ఒకవేళ (!ret)
		నిరంతరం_auto_maintenance(the_repository, opts->quiet);
	తిరిగి ret;
}

పూర్ణాంకం cmd_revert(పూర్ణాంకం argc,
	       స్థిరరాశి అక్షరం **argv,
	       స్థిరరాశి అక్షరం *ఉపసర్గ,
	       నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం replay_opts opts = REPLAY_OPTS_INIT;
	పూర్ణాంకం res;

#నిర్వచితం_కాకపోతే WITH_BREAKING_CHANGES
	warn_on_auto_comment_char = true;
#అయితే_ముగింపు /* !WITH_BREAKING_CHANGES */
	opts.action = REPLAY_REVERT;
	sequencer_init_config(&opts);
	res = నిరంతరం_sequencer(argc, argv, ఉపసర్గ, &opts);
	ఒకవేళ (res < 0)
		die(_("revert failed"));
	replay_opts_release(&opts);
	తిరిగి res;
}

పూర్ణాంకం cmd_cherry_pick(పూర్ణాంకం argc,
స్థిరరాశి అక్షరం **argv,
స్థిరరాశి అక్షరం *ఉపసర్గ,
నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం replay_opts opts = REPLAY_OPTS_INIT;
	పూర్ణాంకం res;

#నిర్వచితం_కాకపోతే WITH_BREAKING_CHANGES
	warn_on_auto_comment_char = true;
#అయితే_ముగింపు /* !WITH_BREAKING_CHANGES */
	opts.action = REPLAY_PICK;
	sequencer_init_config(&opts);
	res = నిరంతరం_sequencer(argc, argv, ఉపసర్గ, &opts);
	ఒకవేళ (res < 0)
		die(_("cherry-pick failed"));
	replay_opts_release(&opts);
	తిరిగి res;
}
