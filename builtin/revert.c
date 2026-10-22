#define USE_THE_REPOSITORY_VARIABLE

#அந்தர்பாவ "git-compat-util.h"
#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "diff.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "revision.h"
#அந்தர்பாவ "rerere.h"
#அந்தர்பாவ "run-command.h"
#அந்தர்பாவ "sequencer.h"
#அந்தர்பாவ "branch.h"

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

நிலையான மாறிலி எழுத்து * மாறிலி revert_usage[] = {
	N_("git revert [--[no-]edit] [-n] [-m <parent-number>] [-s] [-S[<keyid>]] <commit>..."),
	N_("git revert (--continue | --skip | --abort | --quit)"),
	வெற்று
};

நிலையான மாறிலி எழுத்து * மாறிலி cherry_pick_usage[] = {
	N_("git cherry-pick [--edit] [-n] [-m <parent-number>] [-s] [-x] [--ff]\n"
	   "                [-S[<keyid>]] <commit>..."),
	N_("git cherry-pick (--continue | --skip | --abort | --quit)"),
	வெற்று
};

நிலையான மாறிலி எழுத்து *action_name(மாறிலி கட்டமைப்பு replay_opts *opts)
{
	திரும்பு opts->action == REPLAY_REVERT ? "revert" : "cherry-pick";
}

நிலையான மாறிலி எழுத்து * மாறிலி *revert_or_cherry_pick_usage(கட்டமைப்பு replay_opts *opts)
{
	திரும்பு opts->action == REPLAY_REVERT ? revert_usage : cherry_pick_usage;
}

எண்ணிக்கை empty_action {
	EMPTY_COMMIT_UNSPECIFIED = -1,
	STOP_ON_EMPTY_COMMIT,      /* output errors and stop in the middle of a cherry-pick */
	DROP_EMPTY_COMMIT,         /* skip with a notice message */
	KEEP_EMPTY_COMMIT,         /* keep recording as empty commits */
};

நிலையான முழு parse_opt_empty(மாறிலி கட்டமைப்பு option *opt, மாறிலி எழுத்து *arg, முழு unset)
{
	முழு *opt_மதிப்பு = opt->மதிப்பு;

	BUG_ON_OPT_NEG(unset);

	எனில் (!strcmp(arg, "stop"))
		*opt_மதிப்பு = STOP_ON_EMPTY_COMMIT;
	இல்லையெனில் எனில் (!strcmp(arg, "drop"))
		*opt_மதிப்பு = DROP_EMPTY_COMMIT;
	இல்லையெனில் எனில் (!strcmp(arg, "keep"))
		*opt_மதிப்பு = KEEP_EMPTY_COMMIT;
	இல்லையெனில்
		திரும்பு error(_("invalid value for '%s': '%s'"), "--empty", arg);

	திரும்பு 0;
}

நிலையான முழு option_parse_m(மாறிலி கட்டமைப்பு option *opt,
			  மாறிலி எழுத்து *arg, முழு unset)
{
	கட்டமைப்பு replay_opts *replay = opt->மதிப்பு;
	எழுத்து *end;

	எனில் (unset) {
		replay->mainline = 0;
		திரும்பு 0;
	}

	replay->mainline = strtol(arg, &end, 10);
	எனில் (*end || replay->mainline <= 0)
		திரும்பு error(_("option `%s' expects a number greater than zero"),
			     opt->long_name);

	திரும்பு 0;
}

LAST_ARG_MUST_BE_NULL
நிலையான வெறுமை verify_opt_compatible(மாறிலி எழுத்து *me, மாறிலி எழுத்து *base_opt, ...)
{
	மாறிலி எழுத்து *this_opt;
	va_list ap;

	va_start(ap, base_opt);
	போது ((this_opt = va_arg(ap, மாறிலி எழுத்து *))) {
		எனில் (va_arg(ap, முழு))
			நிறுத்து;
	}
	va_end(ap);

	எனில் (this_opt)
		die(_("%s: %s cannot be used with %s"), me, this_opt, base_opt);
}

நிலையான முழு run_sequencer(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
			 கட்டமைப்பு replay_opts *opts)
{
	மாறிலி எழுத்து * மாறிலி * usage_str = revert_or_cherry_pick_usage(opts);
	மாறிலி எழுத்து *me = action_name(opts);
	மாறிலி எழுத்து *cleanup_arg = வெற்று;
	மாறிலி எழுத்து sentinel_மதிப்பு = 0; /* value not important */
	மாறிலி எழுத்து *strategy = &sentinel_மதிப்பு;
	மாறிலி எழுத்து *gpg_sign = &sentinel_மதிப்பு;
	எண்ணிக்கை empty_action empty_opt = EMPTY_COMMIT_UNSPECIFIED;
	முழு cmd = 0, ret;
	கட்டமைப்பு option base_options[] = {
		OPT_CMDMODE(0, "quit", &cmd, N_("end revert or cherry-pick sequence"), 'q'),
		OPT_CMDMODE(0, "continue", &cmd, N_("resume revert or cherry-pick sequence"), 'c'),
		OPT_CMDMODE(0, "abort", &cmd, N_("cancel revert or cherry-pick sequence"), 'a'),
		OPT_CMDMODE(0, "skip", &cmd, N_("skip current commit and continue"), 's'),
		OPT_CLEANUP(&cleanup_arg),
		OPT_BOOL('n', "no-commit", &opts->no_commit, N_("don't automatically commit")),
		OPT_BOOL('e', "edit", &opts->edit, N_("edit the commit message")),
		OPT_NOOP_NOARG('r', வெற்று),
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
			.மதிப்பு = &gpg_sign,
			.argh = N_("key-id"),
			.help = N_("GPG sign commit"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "",
		},
		OPT_END()
	};
	கட்டமைப்பு option *options = base_options;

	எனில் (opts->action == REPLAY_PICK) {
		கட்டமைப்பு option cp_extra[] = {
			OPT_BOOL('x', வெற்று, &opts->record_origin, N_("append commit name")),
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
	} இல்லையெனில் எனில் (opts->action == REPLAY_REVERT) {
		கட்டமைப்பு option cp_extra[] = {
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

	எனில் (opts->action == REPLAY_PICK) {
		opts->drop_redundant_commits = (empty_opt == DROP_EMPTY_COMMIT);
		opts->keep_redundant_commits = opts->keep_redundant_commits || (empty_opt == KEEP_EMPTY_COMMIT);
	}

	/* implies allow_empty */
	எனில் (opts->keep_redundant_commits)
		opts->allow_empty = 1;

	எனில் (cleanup_arg) {
		opts->default_செய்தி_cleanup = get_cleanup_mode(cleanup_arg, 1);
		opts->explicit_cleanup = 1;
	}

	/* Check for incompatible command line arguments */
	எனில் (cmd) {
		மாறிலி எழுத்து *this_operation;
		எனில் (cmd == 'q')
			this_operation = "--quit";
		இல்லையெனில் எனில் (cmd == 'c')
			this_operation = "--continue";
		இல்லையெனில் எனில் (cmd == 's')
			this_operation = "--skip";
		இல்லையெனில் {
			ப்ரதிஜ்ஞா(cmd == 'a');
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
				வெற்று);
	}

	எனில் (!opts->strategy && opts->default_strategy) {
		opts->strategy = opts->default_strategy;
		opts->default_strategy = வெற்று;
	}

	எனில் (opts->allow_ff)
		verify_opt_compatible(me, "--ff",
				"--signoff", opts->signoff,
				"--no-commit", opts->no_commit,
				"-x", opts->record_origin,
				"--edit", opts->edit > 0,
				வெற்று);

	எனில் (cmd) {
		opts->revs = வெற்று;
	} இல்லையெனில் {
		கட்டமைப்பு setup_revision_opt s_r_opt;
		opts->revs = xmalloc(அளவு(*opts->revs));
		repo_init_revisions(the_repository, opts->revs, வெற்று);
		opts->revs->no_walk = 1;
		opts->revs->unsorted_உள்ளீடு = 1;
		எனில் (argc < 2)
			usage_with_options(usage_str, options);
		எனில் (!strcmp(argv[1], "-"))
			argv[1] = "@{-1}";
		memset(&s_r_opt, 0, அளவு(s_r_opt));
		s_r_opt.assume_dashdash = 1;
		argc = setup_revisions(argc, argv, opts->revs, &s_r_opt);
	}

	எனில் (argc > 1)
		usage_with_options(usage_str, options);

	/* These option values will be free()d */
	எனில் (gpg_sign != &sentinel_மதிப்பு) {
		விடுவி(opts->gpg_sign);
		opts->gpg_sign = xstrdup_or_null(gpg_sign);
	}
	எனில் (strategy != &sentinel_மதிப்பு) {
		விடுவி(opts->strategy);
		opts->strategy = xstrdup_or_null(strategy);
	}
	விடுவி(options);

	எனில் (cmd == 'q') {
		ret = sequencer_remove_state(opts);
		எனில் (!ret)
			remove_branch_state(the_repository, 0);
		திரும்பு ret;
	}
	எனில் (cmd == 'a')
		திரும்பு sequencer_rollback(the_repository, opts);
	எனில் (cmd == 'c')
		ret = sequencer_continue(the_repository, opts);
	இல்லையெனில் எனில் (cmd == 's')
		ret = sequencer_skip(the_repository, opts);
	இல்லையெனில்
		ret = sequencer_pick_revisions(the_repository, opts);
	எனில் (!ret)
		run_auto_maintenance(the_repository, opts->quiet);
	திரும்பு ret;
}

முழு cmd_revert(முழு argc,
	       மாறிலி எழுத்து **argv,
	       மாறிலி எழுத்து *prefix,
	       கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு replay_opts opts = REPLAY_OPTS_INIT;
	முழு res;

#ifndef WITH_BREAKING_CHANGES
	warn_on_auto_comment_char = true;
#endif /* !WITH_BREAKING_CHANGES */
	opts.action = REPLAY_REVERT;
	sequencer_init_config(&opts);
	res = run_sequencer(argc, argv, prefix, &opts);
	எனில் (res < 0)
		die(_("revert failed"));
	replay_opts_release(&opts);
	திரும்பு res;
}

முழு cmd_cherry_pick(முழு argc,
மாறிலி எழுத்து **argv,
மாறிலி எழுத்து *prefix,
கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு replay_opts opts = REPLAY_OPTS_INIT;
	முழு res;

#ifndef WITH_BREAKING_CHANGES
	warn_on_auto_comment_char = true;
#endif /* !WITH_BREAKING_CHANGES */
	opts.action = REPLAY_PICK;
	sequencer_init_config(&opts);
	res = run_sequencer(argc, argv, prefix, &opts);
	எனில் (res < 0)
		die(_("cherry-pick failed"));
	replay_opts_release(&opts);
	திரும்பு res;
}
