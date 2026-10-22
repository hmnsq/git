#تعریف_کرو USE_THE_REPOSITORY_VARIABLE

#شامل "git-compat-util.h"
#شامل "builtin.h"
#شامل "parse-options.h"
#شامل "diff.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "revision.h"
#شامل "rerere.h"
#شامل "run-command.h"
#شامل "sequencer.h"
#شامل "branch.h"

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

ساکن ثابت حرف * ثابت revert_usage[] = {
	N_("git revert [--[no-]edit] [-n] [-m <parent-number>] [-s] [-S[<keyid>]] <commit>..."),
	N_("git revert (--continue | --skip | --abort | --quit)"),
	تہی
};

ساکن ثابت حرف * ثابت cherry_pick_usage[] = {
	N_("git cherry-pick [--edit] [-n] [-m <parent-number>] [-s] [-x] [--ff]\n"
	   "                [-S[<keyid>]] <commit>..."),
	N_("git cherry-pick (--continue | --skip | --abort | --quit)"),
	تہی
};

ساکن ثابت حرف *action_name(ثابت ساخت replay_opts *opts)
{
	واپس opts->action == REPLAY_REVERT ? "revert" : "cherry-pick";
}

ساکن ثابت حرف * ثابت *revert_or_cherry_pick_usage(ساخت replay_opts *opts)
{
	واپس opts->action == REPLAY_REVERT ? revert_usage : cherry_pick_usage;
}

شمار خالی_خانے_action {
	خالی_خانے_COMMIT_UNSPECIFIED = -1,
	STOP_ON_خالی_خانے_COMMIT,      /* output errors and stop in the middle of a cherry-pick */
	DROP_خالی_خانے_COMMIT,         /* skip with a notice message */
	KEEP_خالی_خانے_COMMIT,         /* keep recording as empty commits */
};

ساکن صحیح parse_opt_خالی_خانے(ثابت ساخت option *opt, ثابت حرف *arg, صحیح unset)
{
	صحیح *opt_قدر = opt->قدر;

	BUG_ON_OPT_NEG(unset);

	اگر (!strcmp(arg, "stop"))
		*opt_قدر = STOP_ON_خالی_خانے_COMMIT;
	ورنہ اگر (!strcmp(arg, "drop"))
		*opt_قدر = DROP_خالی_خانے_COMMIT;
	ورنہ اگر (!strcmp(arg, "keep"))
		*opt_قدر = KEEP_خالی_خانے_COMMIT;
	ورنہ
		واپس error(_("invalid value for '%s': '%s'"), "--empty", arg);

	واپس 0;
}

ساکن صحیح option_parse_m(ثابت ساخت option *opt,
			  ثابت حرف *arg, صحیح unset)
{
	ساخت replay_opts *replay = opt->قدر;
	حرف *انتہا;

	اگر (unset) {
		replay->mainline = 0;
		واپس 0;
	}

	replay->mainline = strtol(arg, &انتہا, 10);
	اگر (*انتہا || replay->mainline <= 0)
		واپس error(_("option `%s' expects a number greater than zero"),
			     opt->long_name);

	واپس 0;
}

آخری_ARG_MUST_BE_NULL
ساکن خالی verify_opt_compatible(ثابت حرف *me, ثابت حرف *بنیاد_opt, ...)
{
	ثابت حرف *this_opt;
	va_list ap;

	va_آغاز(ap, بنیاد_opt);
	جب ((this_opt = va_arg(ap, ثابت حرف *))) {
		اگر (va_arg(ap, صحیح))
			توڑو;
	}
	va_انتہا(ap);

	اگر (this_opt)
		die(_("%s: %s cannot be used with %s"), me, this_opt, بنیاد_opt);
}

ساکن صحیح تسلسل_sequencer(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
			 ساخت replay_opts *opts)
{
	ثابت حرف * ثابت * usage_str = revert_or_cherry_pick_usage(opts);
	ثابت حرف *me = action_name(opts);
	ثابت حرف *cleanup_arg = تہی;
	ثابت حرف sentinel_قدر = 0; /* value not important */
	ثابت حرف *strategy = &sentinel_قدر;
	ثابت حرف *gpg_sign = &sentinel_قدر;
	شمار خالی_خانے_action خالی_خانے_opt = خالی_خانے_COMMIT_UNSPECIFIED;
	صحیح cmd = 0, ret;
	ساخت option بنیاد_options[] = {
		OPT_CMDMODE(0, "quit", &cmd, N_("end revert or cherry-pick sequence"), 'q'),
		OPT_CMDMODE(0, "continue", &cmd, N_("resume revert or cherry-pick sequence"), 'c'),
		OPT_CMDMODE(0, "abort", &cmd, N_("cancel revert or cherry-pick sequence"), 'a'),
		OPT_CMDMODE(0, "skip", &cmd, N_("skip current commit and continue"), 's'),
		OPT_CLEANUP(&cleanup_arg),
		OPT_BOOL('n', "no-commit", &opts->no_commit, N_("don't automatically commit")),
		OPT_BOOL('e', "edit", &opts->edit, N_("edit the commit message")),
		OPT_NOOP_NOARG('r', تہی),
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
			.قدر = &gpg_sign,
			.argh = N_("key-id"),
			.help = N_("GPG sign commit"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "",
		},
		OPT_انتہا()
	};
	ساخت option *options = بنیاد_options;

	اگر (opts->action == REPLAY_PICK) {
		ساخت option cp_extra[] = {
			OPT_BOOL('x', تہی, &opts->ریکارڈ_origin, N_("append commit name")),
			OPT_BOOL(0, "ff", &opts->allow_ff, N_("allow fast-forward")),
			OPT_BOOL(0, "allow-empty", &opts->allow_خالی_خانے, N_("preserve initially empty commits")),
			OPT_BOOL(0, "allow-empty-message", &opts->allow_خالی_خانے_پیغام, N_("allow commits with empty messages")),
			OPT_BOOL(0, "keep-redundant-commits", &opts->keep_redundant_commits, N_("deprecated: use --empty=keep instead")),
			OPT_CALLBACK_F(0, "empty", &خالی_خانے_opt, "(stop|drop|keep)",
				       N_("how to handle commits that become empty"),
				       PARSE_OPT_NONEG, parse_opt_خالی_خانے),
			OPT_انتہا(),
		};
		options = parse_options_concat(options, cp_extra);
	} ورنہ اگر (opts->action == REPLAY_REVERT) {
		ساخت option cp_extra[] = {
			OPT_BOOL(0, "reference", &opts->commit_use_reference,
				 N_("use the 'reference' format to refer to commits")),
			OPT_انتہا(),
		};
		options = parse_options_concat(options, cp_extra);
	}

	argc = parse_options(argc, argv, سابقہ, options, usage_str,
			PARSE_OPT_KEEP_ARGV0 |
			PARSE_OPT_KEEP_UNKNOWN_OPT);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_بھرا_اشاریہ = 0;

	اگر (opts->action == REPLAY_PICK) {
		opts->drop_redundant_commits = (خالی_خانے_opt == DROP_خالی_خانے_COMMIT);
		opts->keep_redundant_commits = opts->keep_redundant_commits || (خالی_خانے_opt == KEEP_خالی_خانے_COMMIT);
	}

	/* implies allow_empty */
	اگر (opts->keep_redundant_commits)
		opts->allow_خالی_خانے = 1;

	اگر (cleanup_arg) {
		opts->default_پیغام_cleanup = get_cleanup_منوال(cleanup_arg, 1);
		opts->explicit_cleanup = 1;
	}

	/* Check for incompatible command line arguments */
	اگر (cmd) {
		ثابت حرف *this_operation;
		اگر (cmd == 'q')
			this_operation = "--quit";
		ورنہ اگر (cmd == 'c')
			this_operation = "--continue";
		ورنہ اگر (cmd == 's')
			this_operation = "--skip";
		ورنہ {
			أكد(cmd == 'a');
			this_operation = "--abort";
		}

		verify_opt_compatible(me, this_operation,
				"--no-commit", opts->no_commit,
				"--signoff", opts->signoff,
				"--mainline", opts->mainline,
				"--strategy", opts->strategy ? 1 : 0,
				"--strategy-option", opts->xopts.nr ? 1 : 0,
				"-x", opts->ریکارڈ_origin,
				"--ff", opts->allow_ff,
				"--rerere-autoupdate", opts->allow_rerere_auto == RERERE_AUTOUPDATE,
				"--no-rerere-autoupdate", opts->allow_rerere_auto == RERERE_NOAUTOUPDATE,
				"--keep-redundant-commits", opts->keep_redundant_commits,
				"--empty", خالی_خانے_opt != خالی_خانے_COMMIT_UNSPECIFIED,
				تہی);
	}

	اگر (!opts->strategy && opts->default_strategy) {
		opts->strategy = opts->default_strategy;
		opts->default_strategy = تہی;
	}

	اگر (opts->allow_ff)
		verify_opt_compatible(me, "--ff",
				"--signoff", opts->signoff,
				"--no-commit", opts->no_commit,
				"-x", opts->ریکارڈ_origin,
				"--edit", opts->edit > 0,
				تہی);

	اگر (cmd) {
		opts->revs = تہی;
	} ورنہ {
		ساخت تیاری_revision_opt s_r_opt;
		opts->revs = xmalloc(حجم(*opts->revs));
		repo_init_revisions(the_repository, opts->revs, تہی);
		opts->revs->no_walk = 1;
		opts->revs->unsorted_داخلہ = 1;
		اگر (argc < 2)
			usage_with_options(usage_str, options);
		اگر (!strcmp(argv[1], "-"))
			argv[1] = "@{-1}";
		memset(&s_r_opt, 0, حجم(s_r_opt));
		s_r_opt.assume_dashdash = 1;
		argc = تیاری_revisions(argc, argv, opts->revs, &s_r_opt);
	}

	اگر (argc > 1)
		usage_with_options(usage_str, options);

	/* These option values will be free()d */
	اگر (gpg_sign != &sentinel_قدر) {
		آزاد(opts->gpg_sign);
		opts->gpg_sign = xstrdup_or_null(gpg_sign);
	}
	اگر (strategy != &sentinel_قدر) {
		آزاد(opts->strategy);
		opts->strategy = xstrdup_or_null(strategy);
	}
	آزاد(options);

	اگر (cmd == 'q') {
		ret = sequencer_remove_state(opts);
		اگر (!ret)
			remove_branch_state(the_repository, 0);
		واپس ret;
	}
	اگر (cmd == 'a')
		واپس sequencer_rollback(the_repository, opts);
	اگر (cmd == 'c')
		ret = sequencer_continue(the_repository, opts);
	ورنہ اگر (cmd == 's')
		ret = sequencer_skip(the_repository, opts);
	ورنہ
		ret = sequencer_pick_revisions(the_repository, opts);
	اگر (!ret)
		تسلسل_auto_maintenance(the_repository, opts->quiet);
	واپس ret;
}

صحیح cmd_revert(صحیح argc,
	       ثابت حرف **argv,
	       ثابت حرف *سابقہ,
	       ساخت repository *repo غیر_مستعمل)
{
	ساخت replay_opts opts = REPLAY_OPTS_INIT;
	صحیح res;

#اگر_غیر_تعریف_شدہ WITH_BREAKING_CHANGES
	warn_on_auto_comment_char = true;
#اگر_ختم /* !WITH_BREAKING_CHANGES */
	opts.action = REPLAY_REVERT;
	sequencer_init_config(&opts);
	res = تسلسل_sequencer(argc, argv, سابقہ, &opts);
	اگر (res < 0)
		die(_("revert failed"));
	replay_opts_release(&opts);
	واپس res;
}

صحیح cmd_cherry_pick(صحیح argc,
ثابت حرف **argv,
ثابت حرف *سابقہ,
ساخت repository *repo غیر_مستعمل)
{
	ساخت replay_opts opts = REPLAY_OPTS_INIT;
	صحیح res;

#اگر_غیر_تعریف_شدہ WITH_BREAKING_CHANGES
	warn_on_auto_comment_char = true;
#اگر_ختم /* !WITH_BREAKING_CHANGES */
	opts.action = REPLAY_PICK;
	sequencer_init_config(&opts);
	res = تسلسل_sequencer(argc, argv, سابقہ, &opts);
	اگر (res < 0)
		die(_("cherry-pick failed"));
	replay_opts_release(&opts);
	واپس res;
}
