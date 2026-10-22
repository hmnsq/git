#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE

#ਅਨ੍ਤਰ੍ਭਾਵ "git-compat-util.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "diff.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "revision.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "rerere.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "run-command.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "sequencer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "branch.h"

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

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ revert_usage[] = {
	N_("git revert [--[no-]edit] [-n] [-m <parent-number>] [-s] [-S[<keyid>]] <commit>..."),
	N_("git revert (--continue | --skip | --abort | --quit)"),
	ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ cherry_pick_usage[] = {
	N_("git cherry-pick [--edit] [-n] [-m <parent-number>] [-s] [-x] [--ff]\n"
	   "                [-S[<keyid>]] <commit>..."),
	N_("git cherry-pick (--continue | --skip | --abort | --quit)"),
	ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *action_name(ਅਚਲ ਬਣਤਰ replay_opts *opts)
{
	ਵਾਪਸ opts->action == REPLAY_REVERT ? "revert" : "cherry-pick";
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ *revert_or_cherry_pick_usage(ਬਣਤਰ replay_opts *opts)
{
	ਵਾਪਸ opts->action == REPLAY_REVERT ? revert_usage : cherry_pick_usage;
}

ਗਿਣਤੀ ਖਾਲੀ_ਥਾਵਾਂ_action {
	ਖਾਲੀ_ਥਾਵਾਂ_COMMIT_UNSPECIFIED = -1,
	STOP_ON_ਖਾਲੀ_ਥਾਵਾਂ_COMMIT,      /* output errors and stop in the middle of a cherry-pick */
	DROP_ਖਾਲੀ_ਥਾਵਾਂ_COMMIT,         /* skip with a notice message */
	KEEP_ਖਾਲੀ_ਥਾਵਾਂ_COMMIT,         /* keep recording as empty commits */
};

ਸਥਿਰ ਪੂਰਨ parse_opt_ਖਾਲੀ_ਥਾਵਾਂ(ਅਚਲ ਬਣਤਰ option *opt, ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਪੂਰਨ *opt_ਮੁੱਲ = opt->ਮੁੱਲ;

	BUG_ON_OPT_NEG(unset);

	ਜੇ (!strcmp(arg, "stop"))
		*opt_ਮੁੱਲ = STOP_ON_ਖਾਲੀ_ਥਾਵਾਂ_COMMIT;
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(arg, "drop"))
		*opt_ਮੁੱਲ = DROP_ਖਾਲੀ_ਥਾਵਾਂ_COMMIT;
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(arg, "keep"))
		*opt_ਮੁੱਲ = KEEP_ਖਾਲੀ_ਥਾਵਾਂ_COMMIT;
	ਨਹੀਂਤਾਂ
		ਵਾਪਸ error(_("invalid value for '%s': '%s'"), "--empty", arg);

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ option_parse_m(ਅਚਲ ਬਣਤਰ option *opt,
			  ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਬਣਤਰ replay_opts *replay = opt->ਮੁੱਲ;
	ਅੱਖਰ *ਸਿਰਾ;

	ਜੇ (unset) {
		replay->mainline = 0;
		ਵਾਪਸ 0;
	}

	replay->mainline = strtol(arg, &ਸਿਰਾ, 10);
	ਜੇ (*ਸਿਰਾ || replay->mainline <= 0)
		ਵਾਪਸ error(_("option `%s' expects a number greater than zero"),
			     opt->long_name);

	ਵਾਪਸ 0;
}

ਆਖਰੀ_ARG_MUST_BE_NULL
ਸਥਿਰ ਖਾਲੀ verify_opt_compatible(ਅਚਲ ਅੱਖਰ *me, ਅਚਲ ਅੱਖਰ *ਅਧਾਰ_opt, ...)
{
	ਅਚਲ ਅੱਖਰ *this_opt;
	va_list ap;

	va_ਸ਼ੁਰੂ(ap, ਅਧਾਰ_opt);
	ਜਦੋਂ ((this_opt = va_arg(ap, ਅਚਲ ਅੱਖਰ *))) {
		ਜੇ (va_arg(ap, ਪੂਰਨ))
			ਰੋਕੋ;
	}
	va_ਸਿਰਾ(ap);

	ਜੇ (this_opt)
		die(_("%s: %s cannot be used with %s"), me, this_opt, ਅਧਾਰ_opt);
}

ਸਥਿਰ ਪੂਰਨ ਲੜੀਵਾਰ_sequencer(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			 ਬਣਤਰ replay_opts *opts)
{
	ਅਚਲ ਅੱਖਰ * ਅਚਲ * usage_str = revert_or_cherry_pick_usage(opts);
	ਅਚਲ ਅੱਖਰ *me = action_name(opts);
	ਅਚਲ ਅੱਖਰ *cleanup_arg = ਨਿਰਾ;
	ਅਚਲ ਅੱਖਰ sentinel_ਮੁੱਲ = 0; /* value not important */
	ਅਚਲ ਅੱਖਰ *strategy = &sentinel_ਮੁੱਲ;
	ਅਚਲ ਅੱਖਰ *gpg_sign = &sentinel_ਮੁੱਲ;
	ਗਿਣਤੀ ਖਾਲੀ_ਥਾਵਾਂ_action ਖਾਲੀ_ਥਾਵਾਂ_opt = ਖਾਲੀ_ਥਾਵਾਂ_COMMIT_UNSPECIFIED;
	ਪੂਰਨ cmd = 0, ret;
	ਬਣਤਰ option ਅਧਾਰ_options[] = {
		OPT_CMDMODE(0, "quit", &cmd, N_("end revert or cherry-pick sequence"), 'q'),
		OPT_CMDMODE(0, "continue", &cmd, N_("resume revert or cherry-pick sequence"), 'c'),
		OPT_CMDMODE(0, "abort", &cmd, N_("cancel revert or cherry-pick sequence"), 'a'),
		OPT_CMDMODE(0, "skip", &cmd, N_("skip current commit and continue"), 's'),
		OPT_CLEANUP(&cleanup_arg),
		OPT_BOOL('n', "no-commit", &opts->no_commit, N_("don't automatically commit")),
		OPT_BOOL('e', "edit", &opts->edit, N_("edit the commit message")),
		OPT_NOOP_NOARG('r', ਨਿਰਾ),
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
			.ਮੁੱਲ = &gpg_sign,
			.argh = N_("key-id"),
			.help = N_("GPG sign commit"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "",
		},
		OPT_ਸਿਰਾ()
	};
	ਬਣਤਰ option *options = ਅਧਾਰ_options;

	ਜੇ (opts->action == REPLAY_PICK) {
		ਬਣਤਰ option cp_extra[] = {
			OPT_BOOL('x', ਨਿਰਾ, &opts->ਰਿਕਾਰਡ_origin, N_("append commit name")),
			OPT_BOOL(0, "ff", &opts->allow_ff, N_("allow fast-forward")),
			OPT_BOOL(0, "allow-empty", &opts->allow_ਖਾਲੀ_ਥਾਵਾਂ, N_("preserve initially empty commits")),
			OPT_BOOL(0, "allow-empty-message", &opts->allow_ਖਾਲੀ_ਥਾਵਾਂ_ਸੁਨੇਹਾ, N_("allow commits with empty messages")),
			OPT_BOOL(0, "keep-redundant-commits", &opts->keep_redundant_commits, N_("deprecated: use --empty=keep instead")),
			OPT_CALLBACK_F(0, "empty", &ਖਾਲੀ_ਥਾਵਾਂ_opt, "(stop|drop|keep)",
				       N_("how to handle commits that become empty"),
				       PARSE_OPT_NONEG, parse_opt_ਖਾਲੀ_ਥਾਵਾਂ),
			OPT_ਸਿਰਾ(),
		};
		options = parse_options_concat(options, cp_extra);
	} ਨਹੀਂਤਾਂ ਜੇ (opts->action == REPLAY_REVERT) {
		ਬਣਤਰ option cp_extra[] = {
			OPT_BOOL(0, "reference", &opts->commit_use_reference,
				 N_("use the 'reference' format to refer to commits")),
			OPT_ਸਿਰਾ(),
		};
		options = parse_options_concat(options, cp_extra);
	}

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, usage_str,
			PARSE_OPT_KEEP_ARGV0 |
			PARSE_OPT_KEEP_UNKNOWN_OPT);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_ਭਰਿਆ_ਸੂਚਕਾਂਕ = 0;

	ਜੇ (opts->action == REPLAY_PICK) {
		opts->drop_redundant_commits = (ਖਾਲੀ_ਥਾਵਾਂ_opt == DROP_ਖਾਲੀ_ਥਾਵਾਂ_COMMIT);
		opts->keep_redundant_commits = opts->keep_redundant_commits || (ਖਾਲੀ_ਥਾਵਾਂ_opt == KEEP_ਖਾਲੀ_ਥਾਵਾਂ_COMMIT);
	}

	/* implies allow_empty */
	ਜੇ (opts->keep_redundant_commits)
		opts->allow_ਖਾਲੀ_ਥਾਵਾਂ = 1;

	ਜੇ (cleanup_arg) {
		opts->default_ਸੁਨੇਹਾ_cleanup = get_cleanup_ਬਹੁਲਕ(cleanup_arg, 1);
		opts->explicit_cleanup = 1;
	}

	/* Check for incompatible command line arguments */
	ਜੇ (cmd) {
		ਅਚਲ ਅੱਖਰ *this_operation;
		ਜੇ (cmd == 'q')
			this_operation = "--quit";
		ਨਹੀਂਤਾਂ ਜੇ (cmd == 'c')
			this_operation = "--continue";
		ਨਹੀਂਤਾਂ ਜੇ (cmd == 's')
			this_operation = "--skip";
		ਨਹੀਂਤਾਂ {
			ਪ੍ਰਤਿਜ੍ਞਾ(cmd == 'a');
			this_operation = "--abort";
		}

		verify_opt_compatible(me, this_operation,
				"--no-commit", opts->no_commit,
				"--signoff", opts->signoff,
				"--mainline", opts->mainline,
				"--strategy", opts->strategy ? 1 : 0,
				"--strategy-option", opts->xopts.nr ? 1 : 0,
				"-x", opts->ਰਿਕਾਰਡ_origin,
				"--ff", opts->allow_ff,
				"--rerere-autoupdate", opts->allow_rerere_auto == RERERE_AUTOUPDATE,
				"--no-rerere-autoupdate", opts->allow_rerere_auto == RERERE_NOAUTOUPDATE,
				"--keep-redundant-commits", opts->keep_redundant_commits,
				"--empty", ਖਾਲੀ_ਥਾਵਾਂ_opt != ਖਾਲੀ_ਥਾਵਾਂ_COMMIT_UNSPECIFIED,
				ਨਿਰਾ);
	}

	ਜੇ (!opts->strategy && opts->default_strategy) {
		opts->strategy = opts->default_strategy;
		opts->default_strategy = ਨਿਰਾ;
	}

	ਜੇ (opts->allow_ff)
		verify_opt_compatible(me, "--ff",
				"--signoff", opts->signoff,
				"--no-commit", opts->no_commit,
				"-x", opts->ਰਿਕਾਰਡ_origin,
				"--edit", opts->edit > 0,
				ਨਿਰਾ);

	ਜੇ (cmd) {
		opts->revs = ਨਿਰਾ;
	} ਨਹੀਂਤਾਂ {
		ਬਣਤਰ ਤਿਆਰੀ_revision_opt s_r_opt;
		opts->revs = xmalloc(ਆਕਾਰ(*opts->revs));
		repo_init_revisions(the_repository, opts->revs, ਨਿਰਾ);
		opts->revs->no_walk = 1;
		opts->revs->unsorted_ਨਿਵੇਸ਼ = 1;
		ਜੇ (argc < 2)
			usage_with_options(usage_str, options);
		ਜੇ (!strcmp(argv[1], "-"))
			argv[1] = "@{-1}";
		memset(&s_r_opt, 0, ਆਕਾਰ(s_r_opt));
		s_r_opt.assume_dashdash = 1;
		argc = ਤਿਆਰੀ_revisions(argc, argv, opts->revs, &s_r_opt);
	}

	ਜੇ (argc > 1)
		usage_with_options(usage_str, options);

	/* These option values will be free()d */
	ਜੇ (gpg_sign != &sentinel_ਮੁੱਲ) {
		ਮੁਕਤ(opts->gpg_sign);
		opts->gpg_sign = xstrdup_or_null(gpg_sign);
	}
	ਜੇ (strategy != &sentinel_ਮੁੱਲ) {
		ਮੁਕਤ(opts->strategy);
		opts->strategy = xstrdup_or_null(strategy);
	}
	ਮੁਕਤ(options);

	ਜੇ (cmd == 'q') {
		ret = sequencer_remove_state(opts);
		ਜੇ (!ret)
			remove_branch_state(the_repository, 0);
		ਵਾਪਸ ret;
	}
	ਜੇ (cmd == 'a')
		ਵਾਪਸ sequencer_rollback(the_repository, opts);
	ਜੇ (cmd == 'c')
		ret = sequencer_continue(the_repository, opts);
	ਨਹੀਂਤਾਂ ਜੇ (cmd == 's')
		ret = sequencer_skip(the_repository, opts);
	ਨਹੀਂਤਾਂ
		ret = sequencer_pick_revisions(the_repository, opts);
	ਜੇ (!ret)
		ਲੜੀਵਾਰ_auto_maintenance(the_repository, opts->quiet);
	ਵਾਪਸ ret;
}

ਪੂਰਨ cmd_revert(ਪੂਰਨ argc,
	       ਅਚਲ ਅੱਖਰ **argv,
	       ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
	       ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ replay_opts opts = REPLAY_OPTS_INIT;
	ਪੂਰਨ res;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ WITH_BREAKING_CHANGES
	warn_on_auto_comment_char = true;
#ਜੇ_ਅੰਤ /* !WITH_BREAKING_CHANGES */
	opts.action = REPLAY_REVERT;
	sequencer_init_config(&opts);
	res = ਲੜੀਵਾਰ_sequencer(argc, argv, ਅਗੇਤਰ, &opts);
	ਜੇ (res < 0)
		die(_("revert failed"));
	replay_opts_release(&opts);
	ਵਾਪਸ res;
}

ਪੂਰਨ cmd_cherry_pick(ਪੂਰਨ argc,
ਅਚਲ ਅੱਖਰ **argv,
ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ replay_opts opts = REPLAY_OPTS_INIT;
	ਪੂਰਨ res;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ WITH_BREAKING_CHANGES
	warn_on_auto_comment_char = true;
#ਜੇ_ਅੰਤ /* !WITH_BREAKING_CHANGES */
	opts.action = REPLAY_PICK;
	sequencer_init_config(&opts);
	res = ਲੜੀਵਾਰ_sequencer(argc, argv, ਅਗੇਤਰ, &opts);
	ਜੇ (res < 0)
		die(_("cherry-pick failed"));
	replay_opts_release(&opts);
	ਵਾਪਸ res;
}
