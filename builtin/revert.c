#define USE_THE_REPOSITORY_VARIABLE

#include "git-compat-util.h"
#include "builtin.h"
#include "parse-options.h"
#include "diff.h"
#include "environment.h"
#include "gettext.h"
#include "revision.h"
#include "rerere.h"
#include "run-command.h"
#include "sequencer.h"
#include "branch.h"

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

static const var_na * const revert_usage[] = {
	N_("git revert [--[no-]edit] [-n] [-m <parent-number>] [-s] [-S[<keyid>]] <commit>..."),
	N_("git revert (--continue | --skip | --abort | --quit)"),
	NULL
};

static const var_na * const cherry_pick_usage[] = {
	N_("git cherry-pick [--edit] [-n] [-m <parent-number>] [-s] [-x] [--ff]\n"
	   "                [-S[<keyid>]] <commit>..."),
	N_("git cherry-pick (--continue | --skip | --abort | --quit)"),
	NULL
};

static const var_na *action_name(const sa_nracanaa replay_opts *opts)
{
	ᤀᤡᤴ opts->action == REPLAY_REVERT ? "revert" : "cherry-pick";
}

static const var_na * const *revert_or_cherry_pick_usage(sa_nracanaa replay_opts *opts)
{
	ᤀᤡᤴ opts->action == REPLAY_REVERT ? revert_usage : cherry_pick_usage;
}

ga_nanaa empty_action {
	EMPTY_COMMIT_UNSPECIFIED = -1,
	STOP_ON_EMPTY_COMMIT,      /* output errors and stop in the middle of a cherry-pick */
	DROP_EMPTY_COMMIT,         /* skip with a notice message */
	KEEP_EMPTY_COMMIT,         /* keep recording as empty commits */
};

static puur_naa_nka parse_opt_empty(const sa_nracanaa option *opt, const var_na *arg, puur_naa_nka unset)
{
	puur_naa_nka *opt_value = opt->value;

	BUG_ON_OPT_NEG(unset);

	ᤀᤡ᤺ (!strcmp(arg, "stop"))
		*opt_value = STOP_ON_EMPTY_COMMIT;
	ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (!strcmp(arg, "drop"))
		*opt_value = DROP_EMPTY_COMMIT;
	ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (!strcmp(arg, "keep"))
		*opt_value = KEEP_EMPTY_COMMIT;
	ᤀᤡ᤺ᤆᤡ
		ᤀᤡᤴ error(_("invalid value for '%s': '%s'"), "--empty", arg);

	ᤀᤡᤴ 0;
}

static puur_naa_nka option_parse_m(const sa_nracanaa option *opt,
			  const var_na *arg, puur_naa_nka unset)
{
	sa_nracanaa replay_opts *replay = opt->value;
	var_na *end;

	ᤀᤡ᤺ (unset) {
		replay->mainline = 0;
		ᤀᤡᤴ 0;
	}

	replay->mainline = strtol(arg, &end, 10);
	ᤀᤡ᤺ (*end || replay->mainline <= 0)
		ᤀᤡᤴ error(_("option `%s' expects a number greater than zero"),
			     opt->long_name);

	ᤀᤡᤴ 0;
}

LAST_ARG_MUST_BE_NULL
static void verify_opt_compatible(const var_na *me, const var_na *base_opt, ...)
{
	const var_na *this_opt;
	va_list ap;

	va_start(ap, base_opt);
	ᤀᤠ᤺ ((this_opt = va_arg(ap, const var_na *))) {
		ᤀᤡ᤺ (va_arg(ap, puur_naa_nka))
			bha_nga;
	}
	va_end(ap);

	ᤀᤡ᤺ (this_opt)
		die(_("%s: %s cannot be used with %s"), me, this_opt, base_opt);
}

static puur_naa_nka run_sequencer(puur_naa_nka argc, const var_na **argv, const var_na *prefix,
			 sa_nracanaa replay_opts *opts)
{
	const var_na * const * usage_str = revert_or_cherry_pick_usage(opts);
	const var_na *me = action_name(opts);
	const var_na *cleanup_arg = NULL;
	const var_na sentinel_value = 0; /* value not important */
	const var_na *strategy = &sentinel_value;
	const var_na *gpg_sign = &sentinel_value;
	ga_nanaa empty_action empty_opt = EMPTY_COMMIT_UNSPECIFIED;
	puur_naa_nka cmd = 0, ret;
	sa_nracanaa option base_options[] = {
		OPT_CMDMODE(0, "quit", &cmd, N_("end revert or cherry-pick sequence"), 'q'),
		OPT_CMDMODE(0, "continue", &cmd, N_("resume revert or cherry-pick sequence"), 'c'),
		OPT_CMDMODE(0, "abort", &cmd, N_("cancel revert or cherry-pick sequence"), 'a'),
		OPT_CMDMODE(0, "skip", &cmd, N_("skip current commit and continue"), 's'),
		OPT_CLEANUP(&cleanup_arg),
		OPT_BOOL('n', "no-commit", &opts->no_commit, N_("don't automatically commit")),
		OPT_BOOL('e', "edit", &opts->edit, N_("edit the commit message")),
		OPT_NOOP_NOARG('r', NULL),
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
			.value = &gpg_sign,
			.argh = N_("key-id"),
			.help = N_("GPG sign commit"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "",
		},
		OPT_END()
	};
	sa_nracanaa option *options = base_options;

	ᤀᤡ᤺ (opts->action == REPLAY_PICK) {
		sa_nracanaa option cp_extra[] = {
			OPT_BOOL('x', NULL, &opts->record_origin, N_("append commit name")),
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
	} ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (opts->action == REPLAY_REVERT) {
		sa_nracanaa option cp_extra[] = {
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

	ᤀᤡ᤺ (opts->action == REPLAY_PICK) {
		opts->drop_redundant_commits = (empty_opt == DROP_EMPTY_COMMIT);
		opts->keep_redundant_commits = opts->keep_redundant_commits || (empty_opt == KEEP_EMPTY_COMMIT);
	}

	/* implies allow_empty */
	ᤀᤡ᤺ (opts->keep_redundant_commits)
		opts->allow_empty = 1;

	ᤀᤡ᤺ (cleanup_arg) {
		opts->default_msg_cleanup = get_cleanup_mode(cleanup_arg, 1);
		opts->explicit_cleanup = 1;
	}

	/* Check for incompatible command line arguments */
	ᤀᤡ᤺ (cmd) {
		const var_na *this_operation;
		ᤀᤡ᤺ (cmd == 'q')
			this_operation = "--quit";
		ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (cmd == 'c')
			this_operation = "--continue";
		ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (cmd == 's')
			this_operation = "--skip";
		ᤀᤡ᤺ᤆᤡ {
			assert(cmd == 'a');
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
				NULL);
	}

	ᤀᤡ᤺ (!opts->strategy && opts->default_strategy) {
		opts->strategy = opts->default_strategy;
		opts->default_strategy = NULL;
	}

	ᤀᤡ᤺ (opts->allow_ff)
		verify_opt_compatible(me, "--ff",
				"--signoff", opts->signoff,
				"--no-commit", opts->no_commit,
				"-x", opts->record_origin,
				"--edit", opts->edit > 0,
				NULL);

	ᤀᤡ᤺ (cmd) {
		opts->revs = NULL;
	} ᤀᤡ᤺ᤆᤡ {
		sa_nracanaa setup_revision_opt s_r_opt;
		opts->revs = xmalloc(parimaa_na(*opts->revs));
		repo_init_revisions(the_repository, opts->revs, NULL);
		opts->revs->no_walk = 1;
		opts->revs->unsorted_input = 1;
		ᤀᤡ᤺ (argc < 2)
			usage_with_options(usage_str, options);
		ᤀᤡ᤺ (!strcmp(argv[1], "-"))
			argv[1] = "@{-1}";
		memset(&s_r_opt, 0, parimaa_na(s_r_opt));
		s_r_opt.assume_dashdash = 1;
		argc = setup_revisions(argc, argv, opts->revs, &s_r_opt);
	}

	ᤀᤡ᤺ (argc > 1)
		usage_with_options(usage_str, options);

	/* These option values will be free()d */
	ᤀᤡ᤺ (gpg_sign != &sentinel_value) {
		free(opts->gpg_sign);
		opts->gpg_sign = xstrdup_or_null(gpg_sign);
	}
	ᤀᤡ᤺ (strategy != &sentinel_value) {
		free(opts->strategy);
		opts->strategy = xstrdup_or_null(strategy);
	}
	free(options);

	ᤀᤡ᤺ (cmd == 'q') {
		ret = sequencer_remove_state(opts);
		ᤀᤡ᤺ (!ret)
			remove_branch_state(the_repository, 0);
		ᤀᤡᤴ ret;
	}
	ᤀᤡ᤺ (cmd == 'a')
		ᤀᤡᤴ sequencer_rollback(the_repository, opts);
	ᤀᤡ᤺ (cmd == 'c')
		ret = sequencer_continue(the_repository, opts);
	ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (cmd == 's')
		ret = sequencer_skip(the_repository, opts);
	ᤀᤡ᤺ᤆᤡ
		ret = sequencer_pick_revisions(the_repository, opts);
	ᤀᤡ᤺ (!ret)
		run_auto_maintenance(the_repository, opts->quiet);
	ᤀᤡᤴ ret;
}

puur_naa_nka cmd_revert(puur_naa_nka argc,
	       const var_na **argv,
	       const var_na *prefix,
	       sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa replay_opts opts = REPLAY_OPTS_INIT;
	puur_naa_nka res;

#ifndef WITH_BREAKING_CHANGES
	warn_on_auto_comment_char = true;
#endif /* !WITH_BREAKING_CHANGES */
	opts.action = REPLAY_REVERT;
	sequencer_init_config(&opts);
	res = run_sequencer(argc, argv, prefix, &opts);
	ᤀᤡ᤺ (res < 0)
		die(_("revert failed"));
	replay_opts_release(&opts);
	ᤀᤡᤴ res;
}

puur_naa_nka cmd_cherry_pick(puur_naa_nka argc,
const var_na **argv,
const var_na *prefix,
sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa replay_opts opts = REPLAY_OPTS_INIT;
	puur_naa_nka res;

#ifndef WITH_BREAKING_CHANGES
	warn_on_auto_comment_char = true;
#endif /* !WITH_BREAKING_CHANGES */
	opts.action = REPLAY_PICK;
	sequencer_init_config(&opts);
	res = run_sequencer(argc, argv, prefix, &opts);
	ᤀᤡ᤺ (res < 0)
		die(_("cherry-pick failed"));
	replay_opts_release(&opts);
	ᤀᤡᤴ res;
}
