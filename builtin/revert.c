#define USE_THE_REPOSITORY_VARIABLE

#ضمن "git-compat-util.h"
#ضمن "builtin.h"
#ضمن "parse-options.h"
#ضمن "diff.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "revision.h"
#ضمن "rerere.h"
#ضمن "run-command.h"
#ضمن "sequencer.h"
#ضمن "branch.h"

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

ساكن ثابت حرف * ثابت revert_usage[] = {
	N_("git revert [--[no-]edit] [-n] [-m <parent-number>] [-s] [-S[<keyid>]] <commit>..."),
	N_("git revert (--continue | --skip | --abort | --quit)"),
	عدم
};

ساكن ثابت حرف * ثابت cherry_pick_usage[] = {
	N_("git cherry-pick [--edit] [-n] [-m <parent-number>] [-s] [-x] [--ff]\n"
	   "                [-S[<keyid>]] <commit>..."),
	N_("git cherry-pick (--continue | --skip | --abort | --quit)"),
	عدم
};

ساكن ثابت حرف *action_name(ثابت بنية replay_opts *opts)
{
	إرجاع opts->action == REPLAY_REVERT ? "revert" : "cherry-pick";
}

ساكن ثابت حرف * ثابت *revert_or_cherry_pick_usage(بنية replay_opts *opts)
{
	إرجاع opts->action == REPLAY_REVERT ? revert_usage : cherry_pick_usage;
}

تعداد empty_action {
	EMPTY_COMMIT_UNSPECIFIED = -1,
	STOP_ON_EMPTY_COMMIT,      /* output errors and stop in the middle of a cherry-pick */
	DROP_EMPTY_COMMIT,         /* skip with a notice message */
	KEEP_EMPTY_COMMIT,         /* keep recording as empty commits */
};

ساكن صحيح parse_opt_empty(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	صحيح *opt_قيمة = opt->قيمة;

	BUG_ON_OPT_NEG(unset);

	إذا (!strcmp(arg, "stop"))
		*opt_قيمة = STOP_ON_EMPTY_COMMIT;
	وإلا إذا (!strcmp(arg, "drop"))
		*opt_قيمة = DROP_EMPTY_COMMIT;
	وإلا إذا (!strcmp(arg, "keep"))
		*opt_قيمة = KEEP_EMPTY_COMMIT;
	وإلا
		إرجاع error(_("invalid value for '%s': '%s'"), "--empty", arg);

	إرجاع 0;
}

ساكن صحيح option_parse_m(ثابت بنية option *opt,
			  ثابت حرف *arg, صحيح unset)
{
	بنية replay_opts *replay = opt->قيمة;
	حرف *end;

	إذا (unset) {
		replay->mainline = 0;
		إرجاع 0;
	}

	replay->mainline = strtol(arg, &end, 10);
	إذا (*end || replay->mainline <= 0)
		إرجاع error(_("option `%s' expects a number greater than zero"),
			     opt->long_name);

	إرجاع 0;
}

LAST_ARG_MUST_BE_NULL
ساكن فراغ verify_opt_compatible(ثابت حرف *me, ثابت حرف *base_opt, ...)
{
	ثابت حرف *this_opt;
	va_list ap;

	va_start(ap, base_opt);
	بينما ((this_opt = va_arg(ap, ثابت حرف *))) {
		إذا (va_arg(ap, صحيح))
			اكسر;
	}
	va_end(ap);

	إذا (this_opt)
		die(_("%s: %s cannot be used with %s"), me, this_opt, base_opt);
}

ساكن صحيح run_sequencer(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
			 بنية replay_opts *opts)
{
	ثابت حرف * ثابت * usage_str = revert_or_cherry_pick_usage(opts);
	ثابت حرف *me = action_name(opts);
	ثابت حرف *cleanup_arg = عدم;
	ثابت حرف sentinel_قيمة = 0; /* value not important */
	ثابت حرف *strategy = &sentinel_قيمة;
	ثابت حرف *gpg_sign = &sentinel_قيمة;
	تعداد empty_action empty_opt = EMPTY_COMMIT_UNSPECIFIED;
	صحيح cmd = 0, ret;
	بنية option base_options[] = {
		OPT_CMDMODE(0, "quit", &cmd, N_("end revert or cherry-pick sequence"), 'q'),
		OPT_CMDMODE(0, "continue", &cmd, N_("resume revert or cherry-pick sequence"), 'c'),
		OPT_CMDMODE(0, "abort", &cmd, N_("cancel revert or cherry-pick sequence"), 'a'),
		OPT_CMDMODE(0, "skip", &cmd, N_("skip current commit and continue"), 's'),
		OPT_CLEANUP(&cleanup_arg),
		OPT_BOOL('n', "no-commit", &opts->no_commit, N_("don't automatically commit")),
		OPT_BOOL('e', "edit", &opts->edit, N_("edit the commit message")),
		OPT_NOOP_NOARG('r', عدم),
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
			.قيمة = &gpg_sign,
			.argh = N_("key-id"),
			.help = N_("GPG sign commit"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "",
		},
		OPT_END()
	};
	بنية option *options = base_options;

	إذا (opts->action == REPLAY_PICK) {
		بنية option cp_extra[] = {
			OPT_BOOL('x', عدم, &opts->record_origin, N_("append commit name")),
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
	} وإلا إذا (opts->action == REPLAY_REVERT) {
		بنية option cp_extra[] = {
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

	إذا (opts->action == REPLAY_PICK) {
		opts->drop_redundant_commits = (empty_opt == DROP_EMPTY_COMMIT);
		opts->keep_redundant_commits = opts->keep_redundant_commits || (empty_opt == KEEP_EMPTY_COMMIT);
	}

	/* implies allow_empty */
	إذا (opts->keep_redundant_commits)
		opts->allow_empty = 1;

	إذا (cleanup_arg) {
		opts->default_رسالة_cleanup = get_cleanup_mode(cleanup_arg, 1);
		opts->explicit_cleanup = 1;
	}

	/* Check for incompatible command line arguments */
	إذا (cmd) {
		ثابت حرف *this_operation;
		إذا (cmd == 'q')
			this_operation = "--quit";
		وإلا إذا (cmd == 'c')
			this_operation = "--continue";
		وإلا إذا (cmd == 's')
			this_operation = "--skip";
		وإلا {
			أكد(cmd == 'a');
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
				عدم);
	}

	إذا (!opts->strategy && opts->default_strategy) {
		opts->strategy = opts->default_strategy;
		opts->default_strategy = عدم;
	}

	إذا (opts->allow_ff)
		verify_opt_compatible(me, "--ff",
				"--signoff", opts->signoff,
				"--no-commit", opts->no_commit,
				"-x", opts->record_origin,
				"--edit", opts->edit > 0,
				عدم);

	إذا (cmd) {
		opts->revs = عدم;
	} وإلا {
		بنية setup_revision_opt s_r_opt;
		opts->revs = xmalloc(حجم(*opts->revs));
		repo_init_revisions(the_repository, opts->revs, عدم);
		opts->revs->no_walk = 1;
		opts->revs->unsorted_إدخال = 1;
		إذا (argc < 2)
			usage_with_options(usage_str, options);
		إذا (!strcmp(argv[1], "-"))
			argv[1] = "@{-1}";
		memset(&s_r_opt, 0, حجم(s_r_opt));
		s_r_opt.assume_dashdash = 1;
		argc = setup_revisions(argc, argv, opts->revs, &s_r_opt);
	}

	إذا (argc > 1)
		usage_with_options(usage_str, options);

	/* These option values will be free()d */
	إذا (gpg_sign != &sentinel_قيمة) {
		حرر(opts->gpg_sign);
		opts->gpg_sign = xstrdup_or_null(gpg_sign);
	}
	إذا (strategy != &sentinel_قيمة) {
		حرر(opts->strategy);
		opts->strategy = xstrdup_or_null(strategy);
	}
	حرر(options);

	إذا (cmd == 'q') {
		ret = sequencer_remove_state(opts);
		إذا (!ret)
			remove_branch_state(the_repository, 0);
		إرجاع ret;
	}
	إذا (cmd == 'a')
		إرجاع sequencer_rollback(the_repository, opts);
	إذا (cmd == 'c')
		ret = sequencer_continue(the_repository, opts);
	وإلا إذا (cmd == 's')
		ret = sequencer_skip(the_repository, opts);
	وإلا
		ret = sequencer_pick_revisions(the_repository, opts);
	إذا (!ret)
		run_auto_maintenance(the_repository, opts->quiet);
	إرجاع ret;
}

صحيح cmd_revert(صحيح argc,
	       ثابت حرف **argv,
	       ثابت حرف *prefix,
	       بنية repository *repo UNUSED)
{
	بنية replay_opts opts = REPLAY_OPTS_INIT;
	صحيح res;

#ifndef WITH_BREAKING_CHANGES
	warn_on_auto_comment_char = true;
#endif /* !WITH_BREAKING_CHANGES */
	opts.action = REPLAY_REVERT;
	sequencer_init_config(&opts);
	res = run_sequencer(argc, argv, prefix, &opts);
	إذا (res < 0)
		die(_("revert failed"));
	replay_opts_release(&opts);
	إرجاع res;
}

صحيح cmd_cherry_pick(صحيح argc,
ثابت حرف **argv,
ثابت حرف *prefix,
بنية repository *repo UNUSED)
{
	بنية replay_opts opts = REPLAY_OPTS_INIT;
	صحيح res;

#ifndef WITH_BREAKING_CHANGES
	warn_on_auto_comment_char = true;
#endif /* !WITH_BREAKING_CHANGES */
	opts.action = REPLAY_PICK;
	sequencer_init_config(&opts);
	res = run_sequencer(argc, argv, prefix, &opts);
	إذا (res < 0)
		die(_("cherry-pick failed"));
	replay_opts_release(&opts);
	إرجاع res;
}
