#define USE_THE_REPOSITORY_VARIABLE

#ଅନ୍ତର୍ଭାଵ "git-compat-util.h"
#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "diff.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "revision.h"
#ଅନ୍ତର୍ଭାଵ "rerere.h"
#ଅନ୍ତର୍ଭାଵ "run-command.h"
#ଅନ୍ତର୍ଭାଵ "sequencer.h"
#ଅନ୍ତର୍ଭାଵ "branch.h"

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

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ revert_usage[] = {
	N_("git revert [--[no-]edit] [-n] [-m <parent-number>] [-s] [-S[<keyid>]] <commit>..."),
	N_("git revert (--continue | --skip | --abort | --quit)"),
	ଖାଲି
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ cherry_pick_usage[] = {
	N_("git cherry-pick [--edit] [-n] [-m <parent-number>] [-s] [-x] [--ff]\n"
	   "                [-S[<keyid>]] <commit>..."),
	N_("git cherry-pick (--continue | --skip | --abort | --quit)"),
	ଖାଲି
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *action_name(ଅଚଳ ସଂରଚନା replay_opts *opts)
{
	ଫେରନ୍ତୁ opts->action == REPLAY_REVERT ? "revert" : "cherry-pick";
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ *revert_or_cherry_pick_usage(ସଂରଚନା replay_opts *opts)
{
	ଫେରନ୍ତୁ opts->action == REPLAY_REVERT ? revert_usage : cherry_pick_usage;
}

ଗଣନା empty_action {
	EMPTY_COMMIT_UNSPECIFIED = -1,
	STOP_ON_EMPTY_COMMIT,      /* output errors and stop in the middle of a cherry-pick */
	DROP_EMPTY_COMMIT,         /* skip with a notice message */
	KEEP_EMPTY_COMMIT,         /* keep recording as empty commits */
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା parse_opt_empty(ଅଚଳ ସଂରଚନା option *opt, ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା *opt_ମୂଲ୍ୟ = opt->ମୂଲ୍ୟ;

	BUG_ON_OPT_NEG(unset);

	ଯଦି (!strcmp(arg, "stop"))
		*opt_ମୂଲ୍ୟ = STOP_ON_EMPTY_COMMIT;
	ଅନ୍ୟଥା ଯଦି (!strcmp(arg, "drop"))
		*opt_ମୂଲ୍ୟ = DROP_EMPTY_COMMIT;
	ଅନ୍ୟଥା ଯଦି (!strcmp(arg, "keep"))
		*opt_ମୂଲ୍ୟ = KEEP_EMPTY_COMMIT;
	ଅନ୍ୟଥା
		ଫେରନ୍ତୁ error(_("invalid value for '%s': '%s'"), "--empty", arg);

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା option_parse_m(ଅଚଳ ସଂରଚନା option *opt,
			  ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ସଂରଚନା replay_opts *replay = opt->ମୂଲ୍ୟ;
	ଅକ୍ଷର *end;

	ଯଦି (unset) {
		replay->mainline = 0;
		ଫେରନ୍ତୁ 0;
	}

	replay->mainline = strtol(arg, &end, 10);
	ଯଦି (*end || replay->mainline <= 0)
		ଫେରନ୍ତୁ error(_("option `%s' expects a number greater than zero"),
			     opt->long_name);

	ଫେରନ୍ତୁ 0;
}

LAST_ARG_MUST_BE_NULL
ସ୍ଥିର ଶୂନ୍ୟ verify_opt_compatible(ଅଚଳ ଅକ୍ଷର *me, ଅଚଳ ଅକ୍ଷର *base_opt, ...)
{
	ଅଚଳ ଅକ୍ଷର *this_opt;
	va_list ap;

	va_start(ap, base_opt);
	ଯେତେବେଳେ ((this_opt = va_arg(ap, ଅଚଳ ଅକ୍ଷର *))) {
		ଯଦି (va_arg(ap, ପୂର୍ଣ୍ଣସଂଖ୍ୟା))
			ଭାଙ୍ଗ;
	}
	va_end(ap);

	ଯଦି (this_opt)
		die(_("%s: %s cannot be used with %s"), me, this_opt, base_opt);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା run_sequencer(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
			 ସଂରଚନା replay_opts *opts)
{
	ଅଚଳ ଅକ୍ଷର * ଅଚଳ * usage_str = revert_or_cherry_pick_usage(opts);
	ଅଚଳ ଅକ୍ଷର *me = action_name(opts);
	ଅଚଳ ଅକ୍ଷର *cleanup_arg = ଖାଲି;
	ଅଚଳ ଅକ୍ଷର sentinel_ମୂଲ୍ୟ = 0; /* value not important */
	ଅଚଳ ଅକ୍ଷର *strategy = &sentinel_ମୂଲ୍ୟ;
	ଅଚଳ ଅକ୍ଷର *gpg_sign = &sentinel_ମୂଲ୍ୟ;
	ଗଣନା empty_action empty_opt = EMPTY_COMMIT_UNSPECIFIED;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd = 0, ret;
	ସଂରଚନା option base_options[] = {
		OPT_CMDMODE(0, "quit", &cmd, N_("end revert or cherry-pick sequence"), 'q'),
		OPT_CMDMODE(0, "continue", &cmd, N_("resume revert or cherry-pick sequence"), 'c'),
		OPT_CMDMODE(0, "abort", &cmd, N_("cancel revert or cherry-pick sequence"), 'a'),
		OPT_CMDMODE(0, "skip", &cmd, N_("skip current commit and continue"), 's'),
		OPT_CLEANUP(&cleanup_arg),
		OPT_BOOL('n', "no-commit", &opts->no_commit, N_("don't automatically commit")),
		OPT_BOOL('e', "edit", &opts->edit, N_("edit the commit message")),
		OPT_NOOP_NOARG('r', ଖାଲି),
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
			.ମୂଲ୍ୟ = &gpg_sign,
			.argh = N_("key-id"),
			.help = N_("GPG sign commit"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "",
		},
		OPT_END()
	};
	ସଂରଚନା option *options = base_options;

	ଯଦି (opts->action == REPLAY_PICK) {
		ସଂରଚନା option cp_extra[] = {
			OPT_BOOL('x', ଖାଲି, &opts->record_origin, N_("append commit name")),
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
	} ଅନ୍ୟଥା ଯଦି (opts->action == REPLAY_REVERT) {
		ସଂରଚନା option cp_extra[] = {
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

	ଯଦି (opts->action == REPLAY_PICK) {
		opts->drop_redundant_commits = (empty_opt == DROP_EMPTY_COMMIT);
		opts->keep_redundant_commits = opts->keep_redundant_commits || (empty_opt == KEEP_EMPTY_COMMIT);
	}

	/* implies allow_empty */
	ଯଦି (opts->keep_redundant_commits)
		opts->allow_empty = 1;

	ଯଦି (cleanup_arg) {
		opts->default_ସନ୍ଦେଶ_cleanup = get_cleanup_mode(cleanup_arg, 1);
		opts->explicit_cleanup = 1;
	}

	/* Check for incompatible command line arguments */
	ଯଦି (cmd) {
		ଅଚଳ ଅକ୍ଷର *this_operation;
		ଯଦି (cmd == 'q')
			this_operation = "--quit";
		ଅନ୍ୟଥା ଯଦି (cmd == 'c')
			this_operation = "--continue";
		ଅନ୍ୟଥା ଯଦି (cmd == 's')
			this_operation = "--skip";
		ଅନ୍ୟଥା {
			ପ୍ରତିଜ୍ଞା(cmd == 'a');
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
				ଖାଲି);
	}

	ଯଦି (!opts->strategy && opts->default_strategy) {
		opts->strategy = opts->default_strategy;
		opts->default_strategy = ଖାଲି;
	}

	ଯଦି (opts->allow_ff)
		verify_opt_compatible(me, "--ff",
				"--signoff", opts->signoff,
				"--no-commit", opts->no_commit,
				"-x", opts->record_origin,
				"--edit", opts->edit > 0,
				ଖାଲି);

	ଯଦି (cmd) {
		opts->revs = ଖାଲି;
	} ଅନ୍ୟଥା {
		ସଂରଚନା setup_revision_opt s_r_opt;
		opts->revs = xmalloc(ଆକାର(*opts->revs));
		repo_init_revisions(the_repository, opts->revs, ଖାଲି);
		opts->revs->no_walk = 1;
		opts->revs->unsorted_ନିବେଶ = 1;
		ଯଦି (argc < 2)
			usage_with_options(usage_str, options);
		ଯଦି (!strcmp(argv[1], "-"))
			argv[1] = "@{-1}";
		memset(&s_r_opt, 0, ଆକାର(s_r_opt));
		s_r_opt.assume_dashdash = 1;
		argc = setup_revisions(argc, argv, opts->revs, &s_r_opt);
	}

	ଯଦି (argc > 1)
		usage_with_options(usage_str, options);

	/* These option values will be free()d */
	ଯଦି (gpg_sign != &sentinel_ମୂଲ୍ୟ) {
		ମୁକ୍ତ(opts->gpg_sign);
		opts->gpg_sign = xstrdup_or_null(gpg_sign);
	}
	ଯଦି (strategy != &sentinel_ମୂଲ୍ୟ) {
		ମୁକ୍ତ(opts->strategy);
		opts->strategy = xstrdup_or_null(strategy);
	}
	ମୁକ୍ତ(options);

	ଯଦି (cmd == 'q') {
		ret = sequencer_remove_state(opts);
		ଯଦି (!ret)
			remove_branch_state(the_repository, 0);
		ଫେରନ୍ତୁ ret;
	}
	ଯଦି (cmd == 'a')
		ଫେରନ୍ତୁ sequencer_rollback(the_repository, opts);
	ଯଦି (cmd == 'c')
		ret = sequencer_continue(the_repository, opts);
	ଅନ୍ୟଥା ଯଦି (cmd == 's')
		ret = sequencer_skip(the_repository, opts);
	ଅନ୍ୟଥା
		ret = sequencer_pick_revisions(the_repository, opts);
	ଯଦି (!ret)
		run_auto_maintenance(the_repository, opts->quiet);
	ଫେରନ୍ତୁ ret;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_revert(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
	       ଅଚଳ ଅକ୍ଷର **argv,
	       ଅଚଳ ଅକ୍ଷର *prefix,
	       ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା replay_opts opts = REPLAY_OPTS_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା res;

#ifndef WITH_BREAKING_CHANGES
	warn_on_auto_comment_char = true;
#endif /* !WITH_BREAKING_CHANGES */
	opts.action = REPLAY_REVERT;
	sequencer_init_config(&opts);
	res = run_sequencer(argc, argv, prefix, &opts);
	ଯଦି (res < 0)
		die(_("revert failed"));
	replay_opts_release(&opts);
	ଫେରନ୍ତୁ res;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_cherry_pick(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
ଅଚଳ ଅକ୍ଷର **argv,
ଅଚଳ ଅକ୍ଷର *prefix,
ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା replay_opts opts = REPLAY_OPTS_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା res;

#ifndef WITH_BREAKING_CHANGES
	warn_on_auto_comment_char = true;
#endif /* !WITH_BREAKING_CHANGES */
	opts.action = REPLAY_PICK;
	sequencer_init_config(&opts);
	res = run_sequencer(argc, argv, prefix, &opts);
	ଯଦି (res < 0)
		die(_("cherry-pick failed"));
	replay_opts_release(&opts);
	ଫେରନ୍ତୁ res;
}
