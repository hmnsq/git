#define USE_THE_REPOSITORY_VARIABLE

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

ایستا ثابت نویسه * ثابت revert_usage[] = {
	N_("git revert [--[no-]edit] [-n] [-m <parent-number>] [-s] [-S[<keyid>]] <commit>..."),
	N_("git revert (--continue | --skip | --abort | --quit)"),
	پوچ
};

ایستا ثابت نویسه * ثابت cherry_pick_usage[] = {
	N_("git cherry-pick [--edit] [-n] [-m <parent-number>] [-s] [-x] [--ff]\n"
	   "                [-S[<keyid>]] <commit>..."),
	N_("git cherry-pick (--continue | --skip | --abort | --quit)"),
	پوچ
};

ایستا ثابت نویسه *action_name(ثابت ساختار replay_opts *opts)
{
	بازگشت opts->action == REPLAY_REVERT ? "revert" : "cherry-pick";
}

ایستا ثابت نویسه * ثابت *revert_or_cherry_pick_usage(ساختار replay_opts *opts)
{
	بازگشت opts->action == REPLAY_REVERT ? revert_usage : cherry_pick_usage;
}

شمارش empty_action {
	EMPTY_COMMIT_UNSPECIFIED = -1,
	STOP_ON_EMPTY_COMMIT,      /* output errors and stop in the middle of a cherry-pick */
	DROP_EMPTY_COMMIT,         /* skip with a notice message */
	KEEP_EMPTY_COMMIT,         /* keep recording as empty commits */
};

ایستا صحیح parse_opt_empty(ثابت ساختار option *opt, ثابت نویسه *arg, صحیح unset)
{
	صحیح *opt_مقدار = opt->مقدار;

	BUG_ON_OPT_NEG(unset);

	اگر (!strcmp(arg, "stop"))
		*opt_مقدار = STOP_ON_EMPTY_COMMIT;
	وگرنه اگر (!strcmp(arg, "drop"))
		*opt_مقدار = DROP_EMPTY_COMMIT;
	وگرنه اگر (!strcmp(arg, "keep"))
		*opt_مقدار = KEEP_EMPTY_COMMIT;
	وگرنه
		بازگشت error(_("invalid value for '%s': '%s'"), "--empty", arg);

	بازگشت 0;
}

ایستا صحیح option_parse_m(ثابت ساختار option *opt,
			  ثابت نویسه *arg, صحیح unset)
{
	ساختار replay_opts *replay = opt->مقدار;
	نویسه *end;

	اگر (unset) {
		replay->mainline = 0;
		بازگشت 0;
	}

	replay->mainline = strtol(arg, &end, 10);
	اگر (*end || replay->mainline <= 0)
		بازگشت error(_("option `%s' expects a number greater than zero"),
			     opt->long_name);

	بازگشت 0;
}

LAST_ARG_MUST_BE_NULL
ایستا تهی verify_opt_compatible(ثابت نویسه *me, ثابت نویسه *base_opt, ...)
{
	ثابت نویسه *this_opt;
	va_list ap;

	va_start(ap, base_opt);
	وقتی ((this_opt = va_arg(ap, ثابت نویسه *))) {
		اگر (va_arg(ap, صحیح))
			بشکن;
	}
	va_end(ap);

	اگر (this_opt)
		die(_("%s: %s cannot be used with %s"), me, this_opt, base_opt);
}

ایستا صحیح run_sequencer(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
			 ساختار replay_opts *opts)
{
	ثابت نویسه * ثابت * usage_str = revert_or_cherry_pick_usage(opts);
	ثابت نویسه *me = action_name(opts);
	ثابت نویسه *cleanup_arg = پوچ;
	ثابت نویسه sentinel_مقدار = 0; /* value not important */
	ثابت نویسه *strategy = &sentinel_مقدار;
	ثابت نویسه *gpg_sign = &sentinel_مقدار;
	شمارش empty_action empty_opt = EMPTY_COMMIT_UNSPECIFIED;
	صحیح cmd = 0, ret;
	ساختار option base_options[] = {
		OPT_CMDMODE(0, "quit", &cmd, N_("end revert or cherry-pick sequence"), 'q'),
		OPT_CMDMODE(0, "continue", &cmd, N_("resume revert or cherry-pick sequence"), 'c'),
		OPT_CMDMODE(0, "abort", &cmd, N_("cancel revert or cherry-pick sequence"), 'a'),
		OPT_CMDMODE(0, "skip", &cmd, N_("skip current commit and continue"), 's'),
		OPT_CLEANUP(&cleanup_arg),
		OPT_BOOL('n', "no-commit", &opts->no_commit, N_("don't automatically commit")),
		OPT_BOOL('e', "edit", &opts->edit, N_("edit the commit message")),
		OPT_NOOP_NOARG('r', پوچ),
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
			.مقدار = &gpg_sign,
			.argh = N_("key-id"),
			.help = N_("GPG sign commit"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "",
		},
		OPT_END()
	};
	ساختار option *options = base_options;

	اگر (opts->action == REPLAY_PICK) {
		ساختار option cp_extra[] = {
			OPT_BOOL('x', پوچ, &opts->record_origin, N_("append commit name")),
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
	} وگرنه اگر (opts->action == REPLAY_REVERT) {
		ساختار option cp_extra[] = {
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

	اگر (opts->action == REPLAY_PICK) {
		opts->drop_redundant_commits = (empty_opt == DROP_EMPTY_COMMIT);
		opts->keep_redundant_commits = opts->keep_redundant_commits || (empty_opt == KEEP_EMPTY_COMMIT);
	}

	/* implies allow_empty */
	اگر (opts->keep_redundant_commits)
		opts->allow_empty = 1;

	اگر (cleanup_arg) {
		opts->default_پیام_cleanup = get_cleanup_mode(cleanup_arg, 1);
		opts->explicit_cleanup = 1;
	}

	/* Check for incompatible command line arguments */
	اگر (cmd) {
		ثابت نویسه *this_operation;
		اگر (cmd == 'q')
			this_operation = "--quit";
		وگرنه اگر (cmd == 'c')
			this_operation = "--continue";
		وگرنه اگر (cmd == 's')
			this_operation = "--skip";
		وگرنه {
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
				پوچ);
	}

	اگر (!opts->strategy && opts->default_strategy) {
		opts->strategy = opts->default_strategy;
		opts->default_strategy = پوچ;
	}

	اگر (opts->allow_ff)
		verify_opt_compatible(me, "--ff",
				"--signoff", opts->signoff,
				"--no-commit", opts->no_commit,
				"-x", opts->record_origin,
				"--edit", opts->edit > 0,
				پوچ);

	اگر (cmd) {
		opts->revs = پوچ;
	} وگرنه {
		ساختار setup_revision_opt s_r_opt;
		opts->revs = xmalloc(اندازه(*opts->revs));
		repo_init_revisions(the_repository, opts->revs, پوچ);
		opts->revs->no_walk = 1;
		opts->revs->unsorted_درون_داد = 1;
		اگر (argc < 2)
			usage_with_options(usage_str, options);
		اگر (!strcmp(argv[1], "-"))
			argv[1] = "@{-1}";
		memset(&s_r_opt, 0, اندازه(s_r_opt));
		s_r_opt.assume_dashdash = 1;
		argc = setup_revisions(argc, argv, opts->revs, &s_r_opt);
	}

	اگر (argc > 1)
		usage_with_options(usage_str, options);

	/* These option values will be free()d */
	اگر (gpg_sign != &sentinel_مقدار) {
		آزاد(opts->gpg_sign);
		opts->gpg_sign = xstrdup_or_null(gpg_sign);
	}
	اگر (strategy != &sentinel_مقدار) {
		آزاد(opts->strategy);
		opts->strategy = xstrdup_or_null(strategy);
	}
	آزاد(options);

	اگر (cmd == 'q') {
		ret = sequencer_remove_state(opts);
		اگر (!ret)
			remove_branch_state(the_repository, 0);
		بازگشت ret;
	}
	اگر (cmd == 'a')
		بازگشت sequencer_rollback(the_repository, opts);
	اگر (cmd == 'c')
		ret = sequencer_continue(the_repository, opts);
	وگرنه اگر (cmd == 's')
		ret = sequencer_skip(the_repository, opts);
	وگرنه
		ret = sequencer_pick_revisions(the_repository, opts);
	اگر (!ret)
		run_auto_maintenance(the_repository, opts->quiet);
	بازگشت ret;
}

صحیح cmd_revert(صحیح argc,
	       ثابت نویسه **argv,
	       ثابت نویسه *prefix,
	       ساختار repository *repo UNUSED)
{
	ساختار replay_opts opts = REPLAY_OPTS_INIT;
	صحیح res;

#ifndef WITH_BREAKING_CHANGES
	warn_on_auto_comment_char = true;
#endif /* !WITH_BREAKING_CHANGES */
	opts.action = REPLAY_REVERT;
	sequencer_init_config(&opts);
	res = run_sequencer(argc, argv, prefix, &opts);
	اگر (res < 0)
		die(_("revert failed"));
	replay_opts_release(&opts);
	بازگشت res;
}

صحیح cmd_cherry_pick(صحیح argc,
ثابت نویسه **argv,
ثابت نویسه *prefix,
ساختار repository *repo UNUSED)
{
	ساختار replay_opts opts = REPLAY_OPTS_INIT;
	صحیح res;

#ifndef WITH_BREAKING_CHANGES
	warn_on_auto_comment_char = true;
#endif /* !WITH_BREAKING_CHANGES */
	opts.action = REPLAY_PICK;
	sequencer_init_config(&opts);
	res = run_sequencer(argc, argv, prefix, &opts);
	اگر (res < 0)
		die(_("cherry-pick failed"));
	replay_opts_release(&opts);
	بازگشت res;
}
