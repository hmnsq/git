#घोषणा USE_THE_REPOSITORY_VARIABLE

#समावेश "git-compat-util.h"
#समावेश "builtin.h"
#समावेश "parse-options.h"
#समावेश "diff.h"
#समावेश "environment.h"
#समावेश "gettext.h"
#समावेश "revision.h"
#समावेश "rerere.h"
#समावेश "run-command.h"
#समावेश "sequencer.h"
#समावेश "branch.h"

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

अटल स्थिर अक्षर * स्थिर revert_usage[] = {
	N_("git revert [--[no-]edit] [-n] [-m <parent-number>] [-s] [-S[<keyid>]] <commit>..."),
	N_("git revert (--continue | --skip | --abort | --quit)"),
	शून्य
};

अटल स्थिर अक्षर * स्थिर cherry_pick_usage[] = {
	N_("git cherry-pick [--edit] [-n] [-m <parent-number>] [-s] [-x] [--ff]\n"
	   "                [-S[<keyid>]] <commit>..."),
	N_("git cherry-pick (--continue | --skip | --abort | --quit)"),
	शून्य
};

अटल स्थिर अक्षर *action_name(स्थिर काष्ठा replay_opts *opts)
{
	वापस opts->action == REPLAY_REVERT ? "revert" : "cherry-pick";
}

अटल स्थिर अक्षर * स्थिर *revert_or_cherry_pick_usage(काष्ठा replay_opts *opts)
{
	वापस opts->action == REPLAY_REVERT ? revert_usage : cherry_pick_usage;
}

क्रमागत empty_action {
	EMPTY_COMMIT_UNSPECIFIED = -1,
	STOP_ON_EMPTY_COMMIT,      /* output errors and stop in the middle of a cherry-pick */
	DROP_EMPTY_COMMIT,         /* skip with a notice message */
	KEEP_EMPTY_COMMIT,         /* keep recording as empty commits */
};

अटल पूर्णांक parse_opt_empty(स्थिर काष्ठा option *opt, स्थिर अक्षर *arg, पूर्णांक unset)
{
	पूर्णांक *opt_मूल्य = opt->मूल्य;

	BUG_ON_OPT_NEG(unset);

	अगर (!म_भेद(arg, "stop"))
		*opt_मूल्य = STOP_ON_EMPTY_COMMIT;
	अन्यथा अगर (!म_भेद(arg, "drop"))
		*opt_मूल्य = DROP_EMPTY_COMMIT;
	अन्यथा अगर (!म_भेद(arg, "keep"))
		*opt_मूल्य = KEEP_EMPTY_COMMIT;
	अन्यथा
		वापस error(_("invalid value for '%s': '%s'"), "--empty", arg);

	वापस 0;
}

अटल पूर्णांक option_parse_m(स्थिर काष्ठा option *opt,
			  स्थिर अक्षर *arg, पूर्णांक unset)
{
	काष्ठा replay_opts *replay = opt->मूल्य;
	अक्षर *end;

	अगर (unset) {
		replay->mainline = 0;
		वापस 0;
	}

	replay->mainline = म_से_दीर्घ(arg, &end, 10);
	अगर (*end || replay->mainline <= 0)
		वापस error(_("option `%s' expects a number greater than zero"),
			     opt->long_name);

	वापस 0;
}

LAST_ARG_MUST_BE_NULL
अटल व्योम verify_opt_compatible(स्थिर अक्षर *me, स्थिर अक्षर *base_opt, ...)
{
	स्थिर अक्षर *this_opt;
	बहु_सूची ap;

	बहु_शुरू(ap, base_opt);
	जबतक ((this_opt = बहु_तर्क(ap, स्थिर अक्षर *))) {
		अगर (बहु_तर्क(ap, पूर्णांक))
			अवरोध;
	}
	बहु_पूर्ण(ap);

	अगर (this_opt)
		die(_("%s: %s cannot be used with %s"), me, this_opt, base_opt);
}

अटल पूर्णांक run_sequencer(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
			 काष्ठा replay_opts *opts)
{
	स्थिर अक्षर * स्थिर * usage_str = revert_or_cherry_pick_usage(opts);
	स्थिर अक्षर *me = action_name(opts);
	स्थिर अक्षर *cleanup_arg = शून्य;
	स्थिर अक्षर sentinel_मूल्य = 0; /* value not important */
	स्थिर अक्षर *strategy = &sentinel_मूल्य;
	स्थिर अक्षर *gpg_sign = &sentinel_मूल्य;
	क्रमागत empty_action empty_opt = EMPTY_COMMIT_UNSPECIFIED;
	पूर्णांक cmd = 0, ret;
	काष्ठा option base_options[] = {
		OPT_CMDMODE(0, "quit", &cmd, N_("end revert or cherry-pick sequence"), 'q'),
		OPT_CMDMODE(0, "continue", &cmd, N_("resume revert or cherry-pick sequence"), 'c'),
		OPT_CMDMODE(0, "abort", &cmd, N_("cancel revert or cherry-pick sequence"), 'a'),
		OPT_CMDMODE(0, "skip", &cmd, N_("skip current commit and continue"), 's'),
		OPT_CLEANUP(&cleanup_arg),
		OPT_BOOL('n', "no-commit", &opts->no_commit, N_("don't automatically commit")),
		OPT_BOOL('e', "edit", &opts->edit, N_("edit the commit message")),
		OPT_NOOP_NOARG('r', शून्य),
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
			.मूल्य = &gpg_sign,
			.argh = N_("key-id"),
			.help = N_("GPG sign commit"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "",
		},
		OPT_END()
	};
	काष्ठा option *options = base_options;

	अगर (opts->action == REPLAY_PICK) {
		काष्ठा option cp_extra[] = {
			OPT_BOOL('x', शून्य, &opts->record_origin, N_("append commit name")),
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
	} अन्यथा अगर (opts->action == REPLAY_REVERT) {
		काष्ठा option cp_extra[] = {
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
	the_repository->settings.command_requires_full_सूचकांक = 0;

	अगर (opts->action == REPLAY_PICK) {
		opts->drop_redundant_commits = (empty_opt == DROP_EMPTY_COMMIT);
		opts->keep_redundant_commits = opts->keep_redundant_commits || (empty_opt == KEEP_EMPTY_COMMIT);
	}

	/* implies allow_empty */
	अगर (opts->keep_redundant_commits)
		opts->allow_empty = 1;

	अगर (cleanup_arg) {
		opts->default_संदेश_cleanup = get_cleanup_mode(cleanup_arg, 1);
		opts->explicit_cleanup = 1;
	}

	/* Check for incompatible command line arguments */
	अगर (cmd) {
		स्थिर अक्षर *this_operation;
		अगर (cmd == 'q')
			this_operation = "--quit";
		अन्यथा अगर (cmd == 'c')
			this_operation = "--continue";
		अन्यथा अगर (cmd == 's')
			this_operation = "--skip";
		अन्यथा {
			निश्चित(cmd == 'a');
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
				शून्य);
	}

	अगर (!opts->strategy && opts->default_strategy) {
		opts->strategy = opts->default_strategy;
		opts->default_strategy = शून्य;
	}

	अगर (opts->allow_ff)
		verify_opt_compatible(me, "--ff",
				"--signoff", opts->signoff,
				"--no-commit", opts->no_commit,
				"-x", opts->record_origin,
				"--edit", opts->edit > 0,
				शून्य);

	अगर (cmd) {
		opts->revs = शून्य;
	} अन्यथा {
		काष्ठा setup_revision_opt s_r_opt;
		opts->revs = xmalloc(माप(*opts->revs));
		repo_init_revisions(the_repository, opts->revs, शून्य);
		opts->revs->no_walk = 1;
		opts->revs->unsorted_निवेश = 1;
		अगर (argc < 2)
			usage_with_options(usage_str, options);
		अगर (!म_भेद(argv[1], "-"))
			argv[1] = "@{-1}";
		स_रखो(&s_r_opt, 0, माप(s_r_opt));
		s_r_opt.assume_dashdash = 1;
		argc = setup_revisions(argc, argv, opts->revs, &s_r_opt);
	}

	अगर (argc > 1)
		usage_with_options(usage_str, options);

	/* These option values will be free()d */
	अगर (gpg_sign != &sentinel_मूल्य) {
		मुक्त(opts->gpg_sign);
		opts->gpg_sign = xstrdup_or_null(gpg_sign);
	}
	अगर (strategy != &sentinel_मूल्य) {
		मुक्त(opts->strategy);
		opts->strategy = xstrdup_or_null(strategy);
	}
	मुक्त(options);

	अगर (cmd == 'q') {
		ret = sequencer_remove_state(opts);
		अगर (!ret)
			remove_branch_state(the_repository, 0);
		वापस ret;
	}
	अगर (cmd == 'a')
		वापस sequencer_rollback(the_repository, opts);
	अगर (cmd == 'c')
		ret = sequencer_continue(the_repository, opts);
	अन्यथा अगर (cmd == 's')
		ret = sequencer_skip(the_repository, opts);
	अन्यथा
		ret = sequencer_pick_revisions(the_repository, opts);
	अगर (!ret)
		run_auto_maintenance(the_repository, opts->quiet);
	वापस ret;
}

पूर्णांक cmd_revert(पूर्णांक argc,
	       स्थिर अक्षर **argv,
	       स्थिर अक्षर *prefix,
	       काष्ठा repository *repo UNUSED)
{
	काष्ठा replay_opts opts = REPLAY_OPTS_INIT;
	पूर्णांक res;

#अगर_अघोषित WITH_BREAKING_CHANGES
	warn_on_auto_comment_char = true;
#पूर्ण_अगर /* !WITH_BREAKING_CHANGES */
	opts.action = REPLAY_REVERT;
	sequencer_init_config(&opts);
	res = run_sequencer(argc, argv, prefix, &opts);
	अगर (res < 0)
		die(_("revert failed"));
	replay_opts_release(&opts);
	वापस res;
}

पूर्णांक cmd_cherry_pick(पूर्णांक argc,
स्थिर अक्षर **argv,
स्थिर अक्षर *prefix,
काष्ठा repository *repo UNUSED)
{
	काष्ठा replay_opts opts = REPLAY_OPTS_INIT;
	पूर्णांक res;

#अगर_अघोषित WITH_BREAKING_CHANGES
	warn_on_auto_comment_char = true;
#पूर्ण_अगर /* !WITH_BREAKING_CHANGES */
	opts.action = REPLAY_PICK;
	sequencer_init_config(&opts);
	res = run_sequencer(argc, argv, prefix, &opts);
	अगर (res < 0)
		die(_("cherry-pick failed"));
	replay_opts_release(&opts);
	वापस res;
}
