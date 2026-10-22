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

static const char * const revert_usage[] = {
	N_("git revert [--[no-]edit] [-n] [-m <parent-number>] [-s] [-S[<keyid>]] <commit>..."),
	N_("git revert (--continue | --skip | --abort | --quit)"),
	NULL
};

static const char * const cherry_pick_usage[] = {
	N_("git cherry-pick [--edit] [-n] [-m <parent-number>] [-s] [-x] [--ff]\n"
	   "                [-S[<keyid>]] <commit>..."),
	N_("git cherry-pick (--continue | --skip | --abort | --quit)"),
	NULL
};

static const char *action_name(const struct replay_opts *opts)
{
	반환 opts->action == REPLAY_REVERT ? "revert" : "cherry-pick";
}

static const char * const *revert_or_cherry_pick_usage(struct replay_opts *opts)
{
	반환 opts->action == REPLAY_REVERT ? revert_usage : cherry_pick_usage;
}

enum empty_action {
	EMPTY_COMMIT_UNSPECIFIED = -1,
	STOP_ON_EMPTY_COMMIT,      /* output errors and stop in the middle of a cherry-pick */
	DROP_EMPTY_COMMIT,         /* skip with a notice message */
	KEEP_EMPTY_COMMIT,         /* keep recording as empty commits */
};

static 정수 parse_opt_empty(const struct option *opt, const char *arg, 정수 unset)
{
	정수 *opt_value = opt->value;

	BUG_ON_OPT_NEG(unset);

	만약 (!strcmp(arg, "stop"))
		*opt_value = STOP_ON_EMPTY_COMMIT;
	아니면 만약 (!strcmp(arg, "drop"))
		*opt_value = DROP_EMPTY_COMMIT;
	아니면 만약 (!strcmp(arg, "keep"))
		*opt_value = KEEP_EMPTY_COMMIT;
	아니면
		반환 error(_("invalid value for '%s': '%s'"), "--empty", arg);

	반환 0;
}

static 정수 option_parse_m(const struct option *opt,
			  const char *arg, 정수 unset)
{
	struct replay_opts *replay = opt->value;
	char *end;

	만약 (unset) {
		replay->mainline = 0;
		반환 0;
	}

	replay->mainline = strtol(arg, &end, 10);
	만약 (*end || replay->mainline <= 0)
		반환 error(_("option `%s' expects a number greater than zero"),
			     opt->long_name);

	반환 0;
}

LAST_ARG_MUST_BE_NULL
static void verify_opt_compatible(const char *me, const char *base_opt, ...)
{
	const char *this_opt;
	va_list ap;

	va_start(ap, base_opt);
	동안 ((this_opt = va_arg(ap, const char *))) {
		만약 (va_arg(ap, 정수))
			break;
	}
	va_end(ap);

	만약 (this_opt)
		die(_("%s: %s cannot be used with %s"), me, this_opt, base_opt);
}

static 정수 run_sequencer(정수 argc, const char **argv, const char *prefix,
			 struct replay_opts *opts)
{
	const char * const * usage_str = revert_or_cherry_pick_usage(opts);
	const char *me = action_name(opts);
	const char *cleanup_arg = NULL;
	const char sentinel_value = 0; /* value not important */
	const char *strategy = &sentinel_value;
	const char *gpg_sign = &sentinel_value;
	enum empty_action empty_opt = EMPTY_COMMIT_UNSPECIFIED;
	정수 cmd = 0, ret;
	struct option base_options[] = {
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
	struct option *options = base_options;

	만약 (opts->action == REPLAY_PICK) {
		struct option cp_extra[] = {
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
	} 아니면 만약 (opts->action == REPLAY_REVERT) {
		struct option cp_extra[] = {
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

	만약 (opts->action == REPLAY_PICK) {
		opts->drop_redundant_commits = (empty_opt == DROP_EMPTY_COMMIT);
		opts->keep_redundant_commits = opts->keep_redundant_commits || (empty_opt == KEEP_EMPTY_COMMIT);
	}

	/* implies allow_empty */
	만약 (opts->keep_redundant_commits)
		opts->allow_empty = 1;

	만약 (cleanup_arg) {
		opts->default_msg_cleanup = get_cleanup_mode(cleanup_arg, 1);
		opts->explicit_cleanup = 1;
	}

	/* Check for incompatible command line arguments */
	만약 (cmd) {
		const char *this_operation;
		만약 (cmd == 'q')
			this_operation = "--quit";
		아니면 만약 (cmd == 'c')
			this_operation = "--continue";
		아니면 만약 (cmd == 's')
			this_operation = "--skip";
		아니면 {
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

	만약 (!opts->strategy && opts->default_strategy) {
		opts->strategy = opts->default_strategy;
		opts->default_strategy = NULL;
	}

	만약 (opts->allow_ff)
		verify_opt_compatible(me, "--ff",
				"--signoff", opts->signoff,
				"--no-commit", opts->no_commit,
				"-x", opts->record_origin,
				"--edit", opts->edit > 0,
				NULL);

	만약 (cmd) {
		opts->revs = NULL;
	} 아니면 {
		struct setup_revision_opt s_r_opt;
		opts->revs = xmalloc(sizeof(*opts->revs));
		repo_init_revisions(the_repository, opts->revs, NULL);
		opts->revs->no_walk = 1;
		opts->revs->unsorted_input = 1;
		만약 (argc < 2)
			usage_with_options(usage_str, options);
		만약 (!strcmp(argv[1], "-"))
			argv[1] = "@{-1}";
		memset(&s_r_opt, 0, sizeof(s_r_opt));
		s_r_opt.assume_dashdash = 1;
		argc = setup_revisions(argc, argv, opts->revs, &s_r_opt);
	}

	만약 (argc > 1)
		usage_with_options(usage_str, options);

	/* These option values will be free()d */
	만약 (gpg_sign != &sentinel_value) {
		free(opts->gpg_sign);
		opts->gpg_sign = xstrdup_or_null(gpg_sign);
	}
	만약 (strategy != &sentinel_value) {
		free(opts->strategy);
		opts->strategy = xstrdup_or_null(strategy);
	}
	free(options);

	만약 (cmd == 'q') {
		ret = sequencer_remove_state(opts);
		만약 (!ret)
			remove_branch_state(the_repository, 0);
		반환 ret;
	}
	만약 (cmd == 'a')
		반환 sequencer_rollback(the_repository, opts);
	만약 (cmd == 'c')
		ret = sequencer_continue(the_repository, opts);
	아니면 만약 (cmd == 's')
		ret = sequencer_skip(the_repository, opts);
	아니면
		ret = sequencer_pick_revisions(the_repository, opts);
	만약 (!ret)
		run_auto_maintenance(the_repository, opts->quiet);
	반환 ret;
}

정수 cmd_revert(정수 argc,
	       const char **argv,
	       const char *prefix,
	       struct repository *repo UNUSED)
{
	struct replay_opts opts = REPLAY_OPTS_INIT;
	정수 res;

#ifndef WITH_BREAKING_CHANGES
	warn_on_auto_comment_char = true;
#endif /* !WITH_BREAKING_CHANGES */
	opts.action = REPLAY_REVERT;
	sequencer_init_config(&opts);
	res = run_sequencer(argc, argv, prefix, &opts);
	만약 (res < 0)
		die(_("revert failed"));
	replay_opts_release(&opts);
	반환 res;
}

정수 cmd_cherry_pick(정수 argc,
const char **argv,
const char *prefix,
struct repository *repo UNUSED)
{
	struct replay_opts opts = REPLAY_OPTS_INIT;
	정수 res;

#ifndef WITH_BREAKING_CHANGES
	warn_on_auto_comment_char = true;
#endif /* !WITH_BREAKING_CHANGES */
	opts.action = REPLAY_PICK;
	sequencer_init_config(&opts);
	res = run_sequencer(argc, argv, prefix, &opts);
	만약 (res < 0)
		die(_("cherry-pick failed"));
	replay_opts_release(&opts);
	반환 res;
}
