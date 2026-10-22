#define USE_THE_REPOSITORY_VARIABLE

#অন্তর্ভাব "git-compat-util.h"
#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "parse-options.h"
#অন্তর্ভাব "diff.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "revision.h"
#অন্তর্ভাব "rerere.h"
#অন্তর্ভাব "run-command.h"
#অন্তর্ভাব "sequencer.h"
#অন্তর্ভাব "branch.h"

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

স্থির ধ্রুব বর্ণ * ধ্রুব revert_usage[] = {
	N_("git revert [--[no-]edit] [-n] [-m <parent-number>] [-s] [-S[<keyid>]] <commit>..."),
	N_("git revert (--continue | --skip | --abort | --quit)"),
	রিক্ত
};

স্থির ধ্রুব বর্ণ * ধ্রুব cherry_pick_usage[] = {
	N_("git cherry-pick [--edit] [-n] [-m <parent-number>] [-s] [-x] [--ff]\n"
	   "                [-S[<keyid>]] <commit>..."),
	N_("git cherry-pick (--continue | --skip | --abort | --quit)"),
	রিক্ত
};

স্থির ধ্রুব বর্ণ *action_name(ধ্রুব সংরচনা replay_opts *opts)
{
	প্রত্যাগম opts->action == REPLAY_REVERT ? "revert" : "cherry-pick";
}

স্থির ধ্রুব বর্ণ * ধ্রুব *revert_or_cherry_pick_usage(সংরচনা replay_opts *opts)
{
	প্রত্যাগম opts->action == REPLAY_REVERT ? revert_usage : cherry_pick_usage;
}

গণনা empty_action {
	EMPTY_COMMIT_UNSPECIFIED = -1,
	STOP_ON_EMPTY_COMMIT,      /* output errors and stop in the middle of a cherry-pick */
	DROP_EMPTY_COMMIT,         /* skip with a notice message */
	KEEP_EMPTY_COMMIT,         /* keep recording as empty commits */
};

স্থির পূর্ণাঙ্ক parse_opt_empty(ধ্রুব সংরচনা option *opt, ধ্রুব বর্ণ *arg, পূর্ণাঙ্ক unset)
{
	পূর্ণাঙ্ক *opt_মান = opt->মান;

	BUG_ON_OPT_NEG(unset);

	যদি (!strcmp(arg, "stop"))
		*opt_মান = STOP_ON_EMPTY_COMMIT;
	অন্যথা যদি (!strcmp(arg, "drop"))
		*opt_মান = DROP_EMPTY_COMMIT;
	অন্যথা যদি (!strcmp(arg, "keep"))
		*opt_মান = KEEP_EMPTY_COMMIT;
	অন্যথা
		প্রত্যাগম error(_("invalid value for '%s': '%s'"), "--empty", arg);

	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক option_parse_m(ধ্রুব সংরচনা option *opt,
			  ধ্রুব বর্ণ *arg, পূর্ণাঙ্ক unset)
{
	সংরচনা replay_opts *replay = opt->মান;
	বর্ণ *end;

	যদি (unset) {
		replay->mainline = 0;
		প্রত্যাগম 0;
	}

	replay->mainline = strtol(arg, &end, 10);
	যদি (*end || replay->mainline <= 0)
		প্রত্যাগম error(_("option `%s' expects a number greater than zero"),
			     opt->long_name);

	প্রত্যাগম 0;
}

LAST_ARG_MUST_BE_NULL
স্থির শূন্য verify_opt_compatible(ধ্রুব বর্ণ *me, ধ্রুব বর্ণ *base_opt, ...)
{
	ধ্রুব বর্ণ *this_opt;
	va_list ap;

	va_start(ap, base_opt);
	যাবত্ ((this_opt = va_arg(ap, ধ্রুব বর্ণ *))) {
		যদি (va_arg(ap, পূর্ণাঙ্ক))
			ভঙ্গ;
	}
	va_end(ap);

	যদি (this_opt)
		die(_("%s: %s cannot be used with %s"), me, this_opt, base_opt);
}

স্থির পূর্ণাঙ্ক run_sequencer(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix,
			 সংরচনা replay_opts *opts)
{
	ধ্রুব বর্ণ * ধ্রুব * usage_str = revert_or_cherry_pick_usage(opts);
	ধ্রুব বর্ণ *me = action_name(opts);
	ধ্রুব বর্ণ *cleanup_arg = রিক্ত;
	ধ্রুব বর্ণ sentinel_মান = 0; /* value not important */
	ধ্রুব বর্ণ *strategy = &sentinel_মান;
	ধ্রুব বর্ণ *gpg_sign = &sentinel_মান;
	গণনা empty_action empty_opt = EMPTY_COMMIT_UNSPECIFIED;
	পূর্ণাঙ্ক cmd = 0, ret;
	সংরচনা option base_options[] = {
		OPT_CMDMODE(0, "quit", &cmd, N_("end revert or cherry-pick sequence"), 'q'),
		OPT_CMDMODE(0, "continue", &cmd, N_("resume revert or cherry-pick sequence"), 'c'),
		OPT_CMDMODE(0, "abort", &cmd, N_("cancel revert or cherry-pick sequence"), 'a'),
		OPT_CMDMODE(0, "skip", &cmd, N_("skip current commit and continue"), 's'),
		OPT_CLEANUP(&cleanup_arg),
		OPT_BOOL('n', "no-commit", &opts->no_commit, N_("don't automatically commit")),
		OPT_BOOL('e', "edit", &opts->edit, N_("edit the commit message")),
		OPT_NOOP_NOARG('r', রিক্ত),
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
			.মান = &gpg_sign,
			.argh = N_("key-id"),
			.help = N_("GPG sign commit"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "",
		},
		OPT_END()
	};
	সংরচনা option *options = base_options;

	যদি (opts->action == REPLAY_PICK) {
		সংরচনা option cp_extra[] = {
			OPT_BOOL('x', রিক্ত, &opts->record_origin, N_("append commit name")),
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
	} অন্যথা যদি (opts->action == REPLAY_REVERT) {
		সংরচনা option cp_extra[] = {
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

	যদি (opts->action == REPLAY_PICK) {
		opts->drop_redundant_commits = (empty_opt == DROP_EMPTY_COMMIT);
		opts->keep_redundant_commits = opts->keep_redundant_commits || (empty_opt == KEEP_EMPTY_COMMIT);
	}

	/* implies allow_empty */
	যদি (opts->keep_redundant_commits)
		opts->allow_empty = 1;

	যদি (cleanup_arg) {
		opts->default_বার্তা_cleanup = get_cleanup_mode(cleanup_arg, 1);
		opts->explicit_cleanup = 1;
	}

	/* Check for incompatible command line arguments */
	যদি (cmd) {
		ধ্রুব বর্ণ *this_operation;
		যদি (cmd == 'q')
			this_operation = "--quit";
		অন্যথা যদি (cmd == 'c')
			this_operation = "--continue";
		অন্যথা যদি (cmd == 's')
			this_operation = "--skip";
		অন্যথা {
			প্রতিজ্ঞা(cmd == 'a');
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
				রিক্ত);
	}

	যদি (!opts->strategy && opts->default_strategy) {
		opts->strategy = opts->default_strategy;
		opts->default_strategy = রিক্ত;
	}

	যদি (opts->allow_ff)
		verify_opt_compatible(me, "--ff",
				"--signoff", opts->signoff,
				"--no-commit", opts->no_commit,
				"-x", opts->record_origin,
				"--edit", opts->edit > 0,
				রিক্ত);

	যদি (cmd) {
		opts->revs = রিক্ত;
	} অন্যথা {
		সংরচনা setup_revision_opt s_r_opt;
		opts->revs = xmalloc(পরিমাণ(*opts->revs));
		repo_init_revisions(the_repository, opts->revs, রিক্ত);
		opts->revs->no_walk = 1;
		opts->revs->unsorted_প্রবেশ = 1;
		যদি (argc < 2)
			usage_with_options(usage_str, options);
		যদি (!strcmp(argv[1], "-"))
			argv[1] = "@{-1}";
		memset(&s_r_opt, 0, পরিমাণ(s_r_opt));
		s_r_opt.assume_dashdash = 1;
		argc = setup_revisions(argc, argv, opts->revs, &s_r_opt);
	}

	যদি (argc > 1)
		usage_with_options(usage_str, options);

	/* These option values will be free()d */
	যদি (gpg_sign != &sentinel_মান) {
		মুক্ত(opts->gpg_sign);
		opts->gpg_sign = xstrdup_or_null(gpg_sign);
	}
	যদি (strategy != &sentinel_মান) {
		মুক্ত(opts->strategy);
		opts->strategy = xstrdup_or_null(strategy);
	}
	মুক্ত(options);

	যদি (cmd == 'q') {
		ret = sequencer_remove_state(opts);
		যদি (!ret)
			remove_branch_state(the_repository, 0);
		প্রত্যাগম ret;
	}
	যদি (cmd == 'a')
		প্রত্যাগম sequencer_rollback(the_repository, opts);
	যদি (cmd == 'c')
		ret = sequencer_continue(the_repository, opts);
	অন্যথা যদি (cmd == 's')
		ret = sequencer_skip(the_repository, opts);
	অন্যথা
		ret = sequencer_pick_revisions(the_repository, opts);
	যদি (!ret)
		run_auto_maintenance(the_repository, opts->quiet);
	প্রত্যাগম ret;
}

পূর্ণাঙ্ক cmd_revert(পূর্ণাঙ্ক argc,
	       ধ্রুব বর্ণ **argv,
	       ধ্রুব বর্ণ *prefix,
	       সংরচনা repository *repo UNUSED)
{
	সংরচনা replay_opts opts = REPLAY_OPTS_INIT;
	পূর্ণাঙ্ক res;

#ifndef WITH_BREAKING_CHANGES
	warn_on_auto_comment_char = true;
#endif /* !WITH_BREAKING_CHANGES */
	opts.action = REPLAY_REVERT;
	sequencer_init_config(&opts);
	res = run_sequencer(argc, argv, prefix, &opts);
	যদি (res < 0)
		die(_("revert failed"));
	replay_opts_release(&opts);
	প্রত্যাগম res;
}

পূর্ণাঙ্ক cmd_cherry_pick(পূর্ণাঙ্ক argc,
ধ্রুব বর্ণ **argv,
ধ্রুব বর্ণ *prefix,
সংরচনা repository *repo UNUSED)
{
	সংরচনা replay_opts opts = REPLAY_OPTS_INIT;
	পূর্ণাঙ্ক res;

#ifndef WITH_BREAKING_CHANGES
	warn_on_auto_comment_char = true;
#endif /* !WITH_BREAKING_CHANGES */
	opts.action = REPLAY_PICK;
	sequencer_init_config(&opts);
	res = run_sequencer(argc, argv, prefix, &opts);
	যদি (res < 0)
		die(_("cherry-pick failed"));
	replay_opts_release(&opts);
	প্রত্যাগম res;
}
