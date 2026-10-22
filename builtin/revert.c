#define USE_THE_REPOSITORY_VARIABLE

#અન્તર્ભાવ "git-compat-util.h"
#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "diff.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "revision.h"
#અન્તર્ભાવ "rerere.h"
#અન્તર્ભાવ "run-command.h"
#અન્તર્ભાવ "sequencer.h"
#અન્તર્ભાવ "branch.h"

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

સ્થિર અચલ અક્ષર * અચલ revert_usage[] = {
	N_("git revert [--[no-]edit] [-n] [-m <parent-number>] [-s] [-S[<keyid>]] <commit>..."),
	N_("git revert (--continue | --skip | --abort | --quit)"),
	ખાલી
};

સ્થિર અચલ અક્ષર * અચલ cherry_pick_usage[] = {
	N_("git cherry-pick [--edit] [-n] [-m <parent-number>] [-s] [-x] [--ff]\n"
	   "                [-S[<keyid>]] <commit>..."),
	N_("git cherry-pick (--continue | --skip | --abort | --quit)"),
	ખાલી
};

સ્થિર અચલ અક્ષર *action_name(અચલ સંરચના replay_opts *opts)
{
	પાછા opts->action == REPLAY_REVERT ? "revert" : "cherry-pick";
}

સ્થિર અચલ અક્ષર * અચલ *revert_or_cherry_pick_usage(સંરચના replay_opts *opts)
{
	પાછા opts->action == REPLAY_REVERT ? revert_usage : cherry_pick_usage;
}

ગણના empty_action {
	EMPTY_COMMIT_UNSPECIFIED = -1,
	STOP_ON_EMPTY_COMMIT,      /* output errors and stop in the middle of a cherry-pick */
	DROP_EMPTY_COMMIT,         /* skip with a notice message */
	KEEP_EMPTY_COMMIT,         /* keep recording as empty commits */
};

સ્થિર પૂર્ણાંક parse_opt_empty(અચલ સંરચના option *opt, અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	પૂર્ણાંક *opt_કિંમત = opt->કિંમત;

	BUG_ON_OPT_NEG(unset);

	જો (!strcmp(arg, "stop"))
		*opt_કિંમત = STOP_ON_EMPTY_COMMIT;
	નહીંતર જો (!strcmp(arg, "drop"))
		*opt_કિંમત = DROP_EMPTY_COMMIT;
	નહીંતર જો (!strcmp(arg, "keep"))
		*opt_કિંમત = KEEP_EMPTY_COMMIT;
	નહીંતર
		પાછા error(_("invalid value for '%s': '%s'"), "--empty", arg);

	પાછા 0;
}

સ્થિર પૂર્ણાંક option_parse_m(અચલ સંરચના option *opt,
			  અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	સંરચના replay_opts *replay = opt->કિંમત;
	અક્ષર *end;

	જો (unset) {
		replay->mainline = 0;
		પાછા 0;
	}

	replay->mainline = strtol(arg, &end, 10);
	જો (*end || replay->mainline <= 0)
		પાછા error(_("option `%s' expects a number greater than zero"),
			     opt->long_name);

	પાછા 0;
}

LAST_ARG_MUST_BE_NULL
સ્થિર શૂન્ય verify_opt_compatible(અચલ અક્ષર *me, અચલ અક્ષર *base_opt, ...)
{
	અચલ અક્ષર *this_opt;
	va_list ap;

	va_start(ap, base_opt);
	જ્યારે ((this_opt = va_arg(ap, અચલ અક્ષર *))) {
		જો (va_arg(ap, પૂર્ણાંક))
			તોડો;
	}
	va_end(ap);

	જો (this_opt)
		die(_("%s: %s cannot be used with %s"), me, this_opt, base_opt);
}

સ્થિર પૂર્ણાંક run_sequencer(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
			 સંરચના replay_opts *opts)
{
	અચલ અક્ષર * અચલ * usage_str = revert_or_cherry_pick_usage(opts);
	અચલ અક્ષર *me = action_name(opts);
	અચલ અક્ષર *cleanup_arg = ખાલી;
	અચલ અક્ષર sentinel_કિંમત = 0; /* value not important */
	અચલ અક્ષર *strategy = &sentinel_કિંમત;
	અચલ અક્ષર *gpg_sign = &sentinel_કિંમત;
	ગણના empty_action empty_opt = EMPTY_COMMIT_UNSPECIFIED;
	પૂર્ણાંક cmd = 0, ret;
	સંરચના option base_options[] = {
		OPT_CMDMODE(0, "quit", &cmd, N_("end revert or cherry-pick sequence"), 'q'),
		OPT_CMDMODE(0, "continue", &cmd, N_("resume revert or cherry-pick sequence"), 'c'),
		OPT_CMDMODE(0, "abort", &cmd, N_("cancel revert or cherry-pick sequence"), 'a'),
		OPT_CMDMODE(0, "skip", &cmd, N_("skip current commit and continue"), 's'),
		OPT_CLEANUP(&cleanup_arg),
		OPT_BOOL('n', "no-commit", &opts->no_commit, N_("don't automatically commit")),
		OPT_BOOL('e', "edit", &opts->edit, N_("edit the commit message")),
		OPT_NOOP_NOARG('r', ખાલી),
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
			.કિંમત = &gpg_sign,
			.argh = N_("key-id"),
			.help = N_("GPG sign commit"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "",
		},
		OPT_END()
	};
	સંરચના option *options = base_options;

	જો (opts->action == REPLAY_PICK) {
		સંરચના option cp_extra[] = {
			OPT_BOOL('x', ખાલી, &opts->record_origin, N_("append commit name")),
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
	} નહીંતર જો (opts->action == REPLAY_REVERT) {
		સંરચના option cp_extra[] = {
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

	જો (opts->action == REPLAY_PICK) {
		opts->drop_redundant_commits = (empty_opt == DROP_EMPTY_COMMIT);
		opts->keep_redundant_commits = opts->keep_redundant_commits || (empty_opt == KEEP_EMPTY_COMMIT);
	}

	/* implies allow_empty */
	જો (opts->keep_redundant_commits)
		opts->allow_empty = 1;

	જો (cleanup_arg) {
		opts->default_સંદેશ_cleanup = get_cleanup_mode(cleanup_arg, 1);
		opts->explicit_cleanup = 1;
	}

	/* Check for incompatible command line arguments */
	જો (cmd) {
		અચલ અક્ષર *this_operation;
		જો (cmd == 'q')
			this_operation = "--quit";
		નહીંતર જો (cmd == 'c')
			this_operation = "--continue";
		નહીંતર જો (cmd == 's')
			this_operation = "--skip";
		નહીંતર {
			પ્રતિજ્ઞા(cmd == 'a');
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
				ખાલી);
	}

	જો (!opts->strategy && opts->default_strategy) {
		opts->strategy = opts->default_strategy;
		opts->default_strategy = ખાલી;
	}

	જો (opts->allow_ff)
		verify_opt_compatible(me, "--ff",
				"--signoff", opts->signoff,
				"--no-commit", opts->no_commit,
				"-x", opts->record_origin,
				"--edit", opts->edit > 0,
				ખાલી);

	જો (cmd) {
		opts->revs = ખાલી;
	} નહીંતર {
		સંરચના setup_revision_opt s_r_opt;
		opts->revs = xmalloc(કદ(*opts->revs));
		repo_init_revisions(the_repository, opts->revs, ખાલી);
		opts->revs->no_walk = 1;
		opts->revs->unsorted_આગત = 1;
		જો (argc < 2)
			usage_with_options(usage_str, options);
		જો (!strcmp(argv[1], "-"))
			argv[1] = "@{-1}";
		memset(&s_r_opt, 0, કદ(s_r_opt));
		s_r_opt.assume_dashdash = 1;
		argc = setup_revisions(argc, argv, opts->revs, &s_r_opt);
	}

	જો (argc > 1)
		usage_with_options(usage_str, options);

	/* These option values will be free()d */
	જો (gpg_sign != &sentinel_કિંમત) {
		મુક્ત(opts->gpg_sign);
		opts->gpg_sign = xstrdup_or_null(gpg_sign);
	}
	જો (strategy != &sentinel_કિંમત) {
		મુક્ત(opts->strategy);
		opts->strategy = xstrdup_or_null(strategy);
	}
	મુક્ત(options);

	જો (cmd == 'q') {
		ret = sequencer_remove_state(opts);
		જો (!ret)
			remove_branch_state(the_repository, 0);
		પાછા ret;
	}
	જો (cmd == 'a')
		પાછા sequencer_rollback(the_repository, opts);
	જો (cmd == 'c')
		ret = sequencer_continue(the_repository, opts);
	નહીંતર જો (cmd == 's')
		ret = sequencer_skip(the_repository, opts);
	નહીંતર
		ret = sequencer_pick_revisions(the_repository, opts);
	જો (!ret)
		run_auto_maintenance(the_repository, opts->quiet);
	પાછા ret;
}

પૂર્ણાંક cmd_revert(પૂર્ણાંક argc,
	       અચલ અક્ષર **argv,
	       અચલ અક્ષર *prefix,
	       સંરચના repository *repo UNUSED)
{
	સંરચના replay_opts opts = REPLAY_OPTS_INIT;
	પૂર્ણાંક res;

#ifndef WITH_BREAKING_CHANGES
	warn_on_auto_comment_char = true;
#endif /* !WITH_BREAKING_CHANGES */
	opts.action = REPLAY_REVERT;
	sequencer_init_config(&opts);
	res = run_sequencer(argc, argv, prefix, &opts);
	જો (res < 0)
		die(_("revert failed"));
	replay_opts_release(&opts);
	પાછા res;
}

પૂર્ણાંક cmd_cherry_pick(પૂર્ણાંક argc,
અચલ અક્ષર **argv,
અચલ અક્ષર *prefix,
સંરચના repository *repo UNUSED)
{
	સંરચના replay_opts opts = REPLAY_OPTS_INIT;
	પૂર્ણાંક res;

#ifndef WITH_BREAKING_CHANGES
	warn_on_auto_comment_char = true;
#endif /* !WITH_BREAKING_CHANGES */
	opts.action = REPLAY_PICK;
	sequencer_init_config(&opts);
	res = run_sequencer(argc, argv, prefix, &opts);
	જો (res < 0)
		die(_("cherry-pick failed"));
	replay_opts_release(&opts);
	પાછા res;
}
