/*
 * Builtin "git merge"
 *
 * Copyright (c) 2008 Miklos Vajna <vmiklos@frugalware.org>
 *
 * Based on git-merge.sh by Junio C Hamano.
 */

#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE
#ವ್ಯಾಖ್ಯಾನಿಸು DISABLE_SIGN_COMPARE_WARNINGS

#ಅನ್ತರ್ಭಾವ "builtin.h"

#ಅನ್ತರ್ಭಾವ "abspath.h"
#ಅನ್ತರ್ಭಾವ "advice.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "editor.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "object-name.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "lockfile.h"
#ಅನ್ತರ್ಭಾವ "repository.h"
#ಅನ್ತರ್ಭಾವ "run-command.h"
#ಅನ್ತರ್ಭಾವ "hook.h"
#ಅನ್ತರ್ಭಾವ "diff.h"
#ಅನ್ತರ್ಭಾವ "diff-merges.h"
#ಅನ್ತರ್ಭಾವ "refs.h"
#ಅನ್ತರ್ಭಾವ "refspec.h"
#ಅನ್ತರ್ಭಾವ "commit.h"
#ಅನ್ತರ್ಭಾವ "diffcore.h"
#ಅನ್ತರ್ಭಾವ "path.h"
#ಅನ್ತರ್ಭಾವ "revision.h"
#ಅನ್ತರ್ಭಾವ "unpack-trees.h"
#ಅನ್ತರ್ಭಾವ "cache-tree.h"
#ಅನ್ತರ್ಭಾವ "dir.h"
#ಅನ್ತರ್ಭಾವ "color.h"
#ಅನ್ತರ್ಭಾವ "rerere.h"
#ಅನ್ತರ್ಭಾವ "help.h"
#ಅನ್ತರ್ಭಾವ "merge.h"
#ಅನ್ತರ್ಭಾವ "merge-ort-wrappers.h"
#ಅನ್ತರ್ಭಾವ "resolve-undo.h"
#ಅನ್ತರ್ಭಾವ "remote.h"
#ಅನ್ತರ್ಭಾವ "fmt-merge-msg.h"
#ಅನ್ತರ್ಭಾವ "sequencer.h"
#ಅನ್ತರ್ಭಾವ "string-list.h"
#ಅನ್ತರ್ಭಾವ "tag.h"
#ಅನ್ತರ್ಭಾವ "alias.h"
#ಅನ್ತರ್ಭಾವ "branch.h"
#ಅನ್ತರ್ಭಾವ "commit-reach.h"
#ಅನ್ತರ್ಭಾವ "wt-status.h"
#ಅನ್ತರ್ಭಾವ "commit-graph.h"

#ವ್ಯಾಖ್ಯಾನಿಸು DEFAULT_TWOHEAD (1<<0)
#ವ್ಯಾಖ್ಯಾನಿಸು DEFAULT_OCTOPUS (1<<1)
#ವ್ಯಾಖ್ಯಾನಿಸು NO_FAST_FORWARD (1<<2)
#ವ್ಯಾಖ್ಯಾನಿಸು NO_TRIVIAL      (1<<3)

ರಚನೆ strategy {
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name;
	ಚಿಹ್ನೆರಹಿತ attr;
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ builtin_ವಿಲೀನ_usage[] = {
	N_("git merge [<options>] [<commit>...]"),
	"git merge --abort",
	"git merge --continue",
	ಖಾಲಿ
};

#ವ್ಯಾಖ್ಯಾನಿಸು ವಿಲೀನ_SHOW_DIFFSTAT 1
#ವ್ಯಾಖ್ಯಾನಿಸು ವಿಲೀನ_SHOW_COMPACTSUMMARY 2

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_diffstat = ವಿಲೀನ_SHOW_DIFFSTAT, shortlog_len = -1, squash;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ option_commit = -1;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ option_edit = -1;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ allow_trivial = 1, have_ಸಂದೇಶ, verify_signatures;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ check_trust_level = 1;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ overwrite_ignore = 1;
ಸ್ಥಿರ ರಚನೆ strbuf ವಿಲೀನ_ಸಂದೇಶ = STRBUF_INIT;
ಸ್ಥಿರ ರಚನೆ strategy **use_strategies;
ಸ್ಥಿರ ಗಾತ್ರ_t use_strategies_nr, use_strategies_alloc;
ಸ್ಥಿರ ರಚನೆ strvec xopts = STRVEC_INIT;
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *branch;
ಸ್ಥಿರ ಅಕ್ಷರ *branch_mergeoptions;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ verbosity;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ allow_rerere_auto;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ abort_current_ವಿಲೀನ;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ quit_current_ವಿಲೀನ;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ continue_current_ವಿಲೀನ;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ allow_unrelated_histories;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_ಪ್ರಗತಿ = -1;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ default_ಗಮ್ಯ_upstream = 1;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ signoff;
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *sign_commit;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ autostash;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ no_verify;
ಸ್ಥಿರ ಅಕ್ಷರ *into_name;

ಸ್ಥಿರ ರಚನೆ strategy all_strategy[] = {
	{ "recursive",  NO_TRIVIAL },
	{ "octopus",    DEFAULT_OCTOPUS },
	{ "ort",        DEFAULT_TWOHEAD | NO_TRIVIAL },
	{ "resolve",    0 },
	{ "ours",       NO_FAST_FORWARD | NO_TRIVIAL },
	{ "subtree",    NO_FAST_FORWARD | NO_TRIVIAL },
};

ಸ್ಥಿರ ಅಕ್ಷರ *pull_twohead, *pull_octopus;

ಎಣಿಕೆ ff_type {
	FF_NO,
	FF_ALLOW,
	FF_ONLY
};

ಸ್ಥಿರ ಎಣಿಕೆ ff_type fast_forward = FF_ALLOW;

ಸ್ಥಿರ ಅಕ್ಷರ *cleanup_arg;
ಸ್ಥಿರ ಎಣಿಕೆ commit_ಸಂದೇಶ_cleanup_ಬಹುಲಕ cleanup_ಬಹುಲಕ;

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ option_parse_ಸಂದೇಶ(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt,
				ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ರಚನೆ strbuf *buf = opt->ಮೌಲ್ಯ;

	ಆದರೆ (unset)
		strbuf_setlen(buf, 0);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (arg) {
		strbuf_addf(buf, "%s%s", buf->len ? "\n\n" : "", arg);
		have_ಸಂದೇಶ = 1;
	} ಇಲ್ಲದಿದ್ದರೆ
		ಹಿಂತಿರುಗಿ error(_("switch `m' requires a value"));
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಎಣಿಕೆ parse_opt_ಫಲಿತಾಂಶ option_read_ಸಂದೇಶ(ರಚನೆ parse_opt_ctx_t *ctx,
						 ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt,
						 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg_not_ಬಳಸಿದ,
						 ಪೂರ್ಣಾಂಕ unset)
{
	ರಚನೆ strbuf *buf = opt->ಮೌಲ್ಯ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg;

	BUG_ON_OPT_ARG(arg_not_ಬಳಸಿದ);
	ಆದರೆ (unset)
		BUG("-F cannot be negated");

	ಆದರೆ (ctx->opt) {
		arg = ctx->opt;
		ctx->opt = ಖಾಲಿ;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ctx->argc > 1) {
		ctx->argc--;
		arg = *++ctx->argv;
	} ಇಲ್ಲದಿದ್ದರೆ
		ಹಿಂತಿರುಗಿ error(_("option `%s' requires a value"), opt->long_name);

	ಆದರೆ (buf->len)
		strbuf_addch(buf, '\n');
	ಆದರೆ (ctx->ಪೂರ್ವಪ್ರತ್ಯಯ && !is_absolute_ಮಾರ್ಗ(arg))
		arg = ಪೂರ್ವಪ್ರತ್ಯಯ_filename(ctx->ಪೂರ್ವಪ್ರತ್ಯಯ, arg);
	ಆದರೆ (strbuf_read_file(buf, arg, 0) < 0)
		ಹಿಂತಿರುಗಿ error(_("could not read file '%s'"), arg);
	have_ಸಂದೇಶ = 1;

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ರಚನೆ strategy *get_strategy(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name)
{
	ಪೂರ್ಣಾಂಕ i;
	ರಚನೆ strategy *ret;
	ಸ್ಥಿರ ರಚನೆ cmdnames main_cmds = {0}, other_cmds = {0};
	ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ loaded;
	ಅಕ್ಷರ *default_strategy = ಖಾಲಿ;

	ಆದರೆ (!name)
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;

	ಆದರೆ (default_strategy &&
	    !strcmp(default_strategy, "ort") &&
	    !strcmp(name, "recursive")) {
		name = "ort";
	}

	ಗಾಗಿ (i = 0; i < ARRAY_ಗಾತ್ರ(all_strategy); i++)
		ಆದರೆ (!strcmp(name, all_strategy[i].name))
			ಹಿಂತಿರುಗಿ &all_strategy[i];

	ಆದರೆ (!loaded) {
		ರಚನೆ cmdnames not_strategies = {0};
		loaded = 1;

		ತುಂಬು_command_list("git-merge-", &main_cmds, &other_cmds);
		ಗಾಗಿ (i = 0; i < main_cmds.cnt; i++) {
			ಪೂರ್ಣಾಂಕ j, ಸಿಕ್ಕಿತು_2 = 0;
			ರಚನೆ cmdname *ent = main_cmds.ಹೆಸರುಗಳು[i];
			ಗಾಗಿ (j = 0; !ಸಿಕ್ಕಿತು_2 && j < ARRAY_ಗಾತ್ರ(all_strategy); j++)
				ಆದರೆ (!xstrncmpz(all_strategy[j].name, ent->name, ent->len))
					ಸಿಕ್ಕಿತು_2 = 1;
			ಆದರೆ (!ಸಿಕ್ಕಿತು_2)
				ಕೂಡಿಸು_cmdname(&not_strategies, ent->name, ent->len);
		}
		exclude_cmds(&main_cmds, &not_strategies);

		cmdnames_release(&not_strategies);
	}
	ಆದರೆ (!is_ಒಳ_cmdlist(&main_cmds, name) && !is_ಒಳ_cmdlist(&other_cmds, name)) {
		fprintf(stderr, _("Could not find merge strategy '%s'.\n"), name);
		fprintf(stderr, _("Available strategies are:"));
		ಗಾಗಿ (i = 0; i < main_cmds.cnt; i++)
			fprintf(stderr, " %s", main_cmds.ಹೆಸರುಗಳು[i]->name);
		fprintf(stderr, ".\n");
		ಆದರೆ (other_cmds.cnt) {
			fprintf(stderr, _("Available custom strategies are:"));
			ಗಾಗಿ (i = 0; i < other_cmds.cnt; i++)
				fprintf(stderr, " %s", other_cmds.ಹೆಸರುಗಳು[i]->name);
			fprintf(stderr, ".\n");
		}
		ನಿರ್ಗಮಿಸು(1);
	}

	CALLOC_ARRAY(ret, 1);
	ret->name = xstrdup(name);
	ret->attr = NO_TRIVIAL;

	cmdnames_release(&main_cmds);
	cmdnames_release(&other_cmds);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಶೂನ್ಯ append_strategy(ರಚನೆ strategy *s)
{
	ALLOC_GROW(use_strategies, use_strategies_nr + 1, use_strategies_alloc);
	use_strategies[use_strategies_nr++] = s;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ option_parse_strategy(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt ಬಳಸದ,
				 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಪೂರ್ಣಾಂಕ unset)
{
	ಆದರೆ (unset)
		ಹಿಂತಿರುಗಿ 0;

	append_strategy(get_strategy(name));
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ option_parse_compact_summary(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt,
					ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name ಬಳಸದ, ಪೂರ್ಣಾಂಕ unset)
{
	ಪೂರ್ಣಾಂಕ *setting = opt->ಮೌಲ್ಯ;

	ಆದರೆ (unset)
		*setting = 0;
	ಇಲ್ಲದಿದ್ದರೆ
		*setting = ವಿಲೀನ_SHOW_COMPACTSUMMARY;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ರಚನೆ option builtin_ವಿಲೀನ_options[] = {
	OPT_ಗಣ_INT('n', ಖಾಲಿ, &show_diffstat,
		N_("do not show a diffstat at the end of the merge"), 0),
	OPT_BOOL(0, "stat", &show_diffstat,
		N_("show a diffstat at the end of the merge")),
	OPT_BOOL(0, "summary", &show_diffstat, N_("(synonym to --stat)")),
	OPT_CALLBACK_F(0, "compact-summary", &show_diffstat, ಖಾಲಿ,
		       N_("show a compact-summary at the end of the merge"),
		       PARSE_OPT_NOARG,
		       option_parse_compact_summary),
	{
		.type = OPTION_INTEGER,
		.long_name = "log",
		.ಮೌಲ್ಯ = &shortlog_len,
		.precision = ಗಾತ್ರ(shortlog_len),
		.argh = N_("n"),
		.help = N_("add (at most <n>) entries from shortlog to merge commit message"),
		.flags = PARSE_OPT_OPTARG,
		.defval = DEFAULT_ವಿಲೀನ_LOG_LEN,
	},
	OPT_BOOL(0, "squash", &squash,
		N_("create a single commit instead of doing a merge")),
	OPT_BOOL(0, "commit", &option_commit,
		N_("perform a commit if the merge succeeds (default)")),
	OPT_BOOL('e', "edit", &option_edit,
		N_("edit message before committing")),
	OPT_CLEANUP(&cleanup_arg),
	OPT_ಗಣ_INT(0, "ff", &fast_forward, N_("allow fast-forward (default)"), FF_ALLOW),
	OPT_ಗಣ_INT_F(0, "ff-only", &fast_forward,
		      N_("abort if fast-forward is not possible"),
		      FF_ONLY, PARSE_OPT_NONEG),
	OPT_RERERE_AUTOUPDATE(&allow_rerere_auto),
	OPT_BOOL(0, "verify-signatures", &verify_signatures,
		N_("verify that the named commit has a valid GPG signature")),
	OPT_CALLBACK('s', "strategy", ಖಾಲಿ, N_("strategy"),
		N_("merge strategy to use"), option_parse_strategy),
	OPT_STRVEC('X', "strategy-option", &xopts, N_("option=value"),
		N_("option for selected merge strategy")),
	OPT_CALLBACK('m', "message", &ವಿಲೀನ_ಸಂದೇಶ, N_("message"),
		N_("merge commit message (for a non-fast-forward merge)"),
		option_parse_ಸಂದೇಶ),
	{
		.type = OPTION_LOWLEVEL_CALLBACK,
		.short_name = 'F',
		.long_name = "file",
		.ಮೌಲ್ಯ = &ವಿಲೀನ_ಸಂದೇಶ,
		.argh = N_("path"),
		.help = N_("read message from file"),
		.flags = PARSE_OPT_NONEG,
		.ll_callback = option_read_ಸಂದೇಶ,
	},
	OPT_STRING(0, "into-name", &into_name, N_("name"),
		   N_("use <name> instead of the real target")),
	OPT__VERBOSITY(&verbosity),
	OPT_BOOL(0, "abort", &abort_current_ವಿಲೀನ,
		N_("abort the current in-progress merge")),
	OPT_BOOL(0, "quit", &quit_current_ವಿಲೀನ,
		N_("--abort but leave index and working tree alone")),
	OPT_BOOL(0, "continue", &continue_current_ವಿಲೀನ,
		N_("continue the current in-progress merge")),
	OPT_BOOL(0, "allow-unrelated-histories", &allow_unrelated_histories,
		 N_("allow merging unrelated histories")),
	OPT_ಗಣ_INT(0, "progress", &show_ಪ್ರಗತಿ, N_("force progress reporting"), 1),
	{
		.type = OPTION_STRING,
		.short_name = 'S',
		.long_name = "gpg-sign",
		.ಮೌಲ್ಯ = &sign_commit,
		.argh = N_("key-id"),
		.help = N_("GPG sign commit"),
		.flags = PARSE_OPT_OPTARG,
		.defval = (intptr_t) "",
	},
	OPT_AUTOSTASH(&autostash),
	OPT_BOOL(0, "overwrite-ignore", &overwrite_ignore, N_("update ignored files (default)")),
	OPT_BOOL(0, "signoff", &signoff, N_("add a Signed-off-by trailer")),
	OPT_BOOL(0, "no-verify", &no_verify, N_("bypass pre-merge-commit and commit-msg hooks")),
	OPT_ಕೊನೆ()
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ save_state(ರಚನೆ object_id *stash)
{
	ಪೂರ್ಣಾಂಕ len;
	ರಚನೆ ಮಗು_process cp = ಮಗು_PROCESS_INIT;
	ರಚನೆ strbuf ಬಫರ್ = STRBUF_INIT;
	ರಚನೆ ಬೀಗ_file ಬೀಗ_file = ಬೀಗ_INIT;
	ಪೂರ್ಣಾಂಕ fd;
	ಪೂರ್ಣಾಂಕ rc = -1;

	fd = repo_hold_locked_ಸೂಚ್ಯಂಕ(the_repository, &ಬೀಗ_file, 0);
	refresh_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ, REFRESH_QUIET, ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ);
	ಆದರೆ (0 <= fd)
		repo_update_ಸೂಚ್ಯಂಕ_if_able(the_repository, &ಬೀಗ_file);
	rollback_ಬೀಗ_file(&ಬೀಗ_file);

	strvec_pushl(&cp.ವಾದಗಳು, "stash", "create", ಖಾಲಿ);
	cp.ಹೊರಹೋಗು = -1;
	cp.git_cmd = 1;

	ಆದರೆ (ಆರಂಭ_command(&cp))
		die(_("could not run stash."));
	len = strbuf_read(&ಬಫರ್, cp.ಹೊರಹೋಗು, 1024);
	close(cp.ಹೊರಹೋಗು);

	ಆದರೆ (ಮುಕ್ತಾಯ_command(&cp) || len < 0)
		die(_("stash failed"));
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!len)		/* no changes */
		ಹೋಗು ಹೊರಹೋಗು;
	strbuf_setlen(&ಬಫರ್, ಬಫರ್.len-1);
	ಆದರೆ (repo_get_oid(the_repository, ಬಫರ್.buf, stash))
		die(_("not a valid object: %s"), ಬಫರ್.buf);
	rc = 0;
ಹೊರಹೋಗು:
	strbuf_release(&ಬಫರ್);
	ಹಿಂತಿರುಗಿ rc;
}

ಸ್ಥಿರ ಶೂನ್ಯ read_ಖಾಲಿಗಳು(ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid)
{
	ರಚನೆ ಮಗು_process cmd = ಮಗು_PROCESS_INIT;

	strvec_pushl(&cmd.ವಾದಗಳು, "read-tree", "-m", "-u",
		     ಖಾಲಿಗಳು_ವೃಕ್ಷ_oid_hex(the_repository->hash_algo),
		     oid_ಗಮ್ಯ_hex(oid), ಖಾಲಿ);
	cmd.git_cmd = 1;

	ಆದರೆ (ಸತತ_command(&cmd))
		die(_("read-tree failed"));
}

ಸ್ಥಿರ ಶೂನ್ಯ reset_hard(ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid)
{
	ರಚನೆ ಮಗು_process cmd = ಮಗು_PROCESS_INIT;

	strvec_pushl(&cmd.ವಾದಗಳು, "read-tree", "-v", "--reset", "-u",
		     oid_ಗಮ್ಯ_hex(oid), ಖಾಲಿ);
	cmd.git_cmd = 1;

	ಆದರೆ (ಸತತ_command(&cmd))
		die(_("read-tree failed"));
}

ಸ್ಥಿರ ಶೂನ್ಯ restore_state(ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *ತಲೆ,
			  ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *stash)
{
	ರಚನೆ ಮಗು_process cmd = ಮಗು_PROCESS_INIT;

	reset_hard(ತಲೆ);

	ಆದರೆ (is_null_oid(stash))
		ಹೋಗು refresh_cache;

	strvec_pushl(&cmd.ವಾದಗಳು, "stash", "apply", "--index", "--quiet", ಖಾಲಿ);
	strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, oid_ಗಮ್ಯ_hex(stash));

	/*
	 * It is OK to ignore error here, for example when there was
	 * nothing to restore.
	 */
	cmd.git_cmd = 1;
	ಸತತ_command(&cmd);

refresh_cache:
	discard_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ);
	ಆದರೆ (repo_read_ಸೂಚ್ಯಂಕ(the_repository) < 0)
		die(_("could not read index"));
}

/* This is called when no merge was necessary. */
ಸ್ಥಿರ ಶೂನ್ಯ ಮುಕ್ತಾಯ_ಮೇಲೆ_ಗಮ್ಯ_date(ಶೂನ್ಯ)
{
	ಆದರೆ (verbosity >= 0) {
		ಆದರೆ (squash)
			puts(_("Already up to date. (nothing to squash)"));
		ಇಲ್ಲದಿದ್ದರೆ
			puts(_("Already up to date."));
	}
	remove_ವಿಲೀನ_branch_state(the_repository);
}

ಸ್ಥಿರ ಶೂನ್ಯ squash_ಸಂದೇಶ(ರಚನೆ commit *commit, ರಚನೆ commit_list *remoteheads)
{
	ರಚನೆ rev_info rev;
	ರಚನೆ strbuf ಹೊರಹೋಗು = STRBUF_INIT;
	ರಚನೆ commit_list *j;
	ರಚನೆ pretty_print_context ctx = {0};

	ಬರೆ(_("Squash commit -- not updating HEAD\n"));

	repo_init_revisions(the_repository, &rev, ಖಾಲಿ);
	diff_merges_suppress(&rev);
	rev.commit_format = CMIT_FMT_MEDIUM;

	commit->object.flags |= UNINTERESTING;
	ಕೂಡಿಸು_pending_object(&rev, &commit->object, ಖಾಲಿ);

	ಗಾಗಿ (j = remoteheads; j; j = j->ಮುಂದಿನ)
		ಕೂಡಿಸು_pending_object(&rev, &j->ಅಂಶ->object, ಖಾಲಿ);

	ಸಿದ್ಧತೆ_revisions(0, ಖಾಲಿ, &rev, ಖಾಲಿ);
	ಆದರೆ (prepare_revision_walk(&rev))
		die(_("revision walk setup failed"));

	ctx.abbrev = rev.abbrev;
	ctx.date_ಬಹುಲಕ = rev.date_ಬಹುಲಕ;
	ctx.fmt = rev.commit_format;

	strbuf_addstr(&ಹೊರಹೋಗು, "Squashed commit of the following:\n");
	ಆಗ ((commit = get_revision(&rev)) != ಖಾಲಿ) {
		strbuf_addch(&ಹೊರಹೋಗು, '\n');
		strbuf_addf(&ಹೊರಹೋಗು, "commit %s\n",
			oid_ಗಮ್ಯ_hex(&commit->object.oid));
		pretty_print_commit(&ctx, commit, &ಹೊರಹೋಗು);
	}
	write_file_buf(git_ಮಾರ್ಗ_squash_ಸಂದೇಶ(the_repository), ಹೊರಹೋಗು.buf, ಹೊರಹೋಗು.len);
	strbuf_release(&ಹೊರಹೋಗು);
	release_revisions(&rev);
}

ಸ್ಥಿರ ಶೂನ್ಯ ಮುಕ್ತಾಯ(ರಚನೆ commit *ತಲೆ_commit,
		   ರಚನೆ commit_list *remoteheads,
		   ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *new_ತಲೆ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ)
{
	ರಚನೆ strbuf reflog_ಸಂದೇಶ_2 = STRBUF_INIT;
	ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *ತಲೆ = &ತಲೆ_commit->object.oid;

	ಆದರೆ (!ಸಂದೇಶ)
		strbuf_addstr(&reflog_ಸಂದೇಶ_2, getenv("GIT_REFLOG_ACTION"));
	ಇಲ್ಲದಿದ್ದರೆ {
		ಆದರೆ (verbosity >= 0)
			ಬರೆ("%s\n", ಸಂದೇಶ);
		strbuf_addf(&reflog_ಸಂದೇಶ_2, "%s: %s",
			getenv("GIT_REFLOG_ACTION"), ಸಂದೇಶ);
	}
	ಆದರೆ (squash) {
		squash_ಸಂದೇಶ(ತಲೆ_commit, remoteheads);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಆದರೆ (verbosity >= 0 && !ವಿಲೀನ_ಸಂದೇಶ.len)
			ಬರೆ(_("No merge message -- not updating HEAD\n"));
		ಇಲ್ಲದಿದ್ದರೆ {
			refs_update_ref(get_main_ref_store(the_repository),
					reflog_ಸಂದೇಶ_2.buf, "HEAD", new_ತಲೆ,
					ತಲೆ,
					0, UPDATE_REFS_DIE_ON_ERR);
			/*
			 * We ignore errors in 'gc --auto', since the
			 * user should see them.
			 */
			ಸತತ_auto_maintenance(the_repository, verbosity < 0);
		}
	}
	ಆದರೆ (new_ತಲೆ && show_diffstat) {
		ರಚನೆ diff_options opts;
		repo_diff_ಸಿದ್ಧತೆ(the_repository, &opts);
		init_diffstat_widths(&opts);

		ಆಯ್ಕೆ (show_diffstat) {
		ಸ್ಥಿತಿ ವಿಲೀನ_SHOW_DIFFSTAT: /* 1 */
			opts.output_format |=
				DIFF_FORMAT_SUMMARY | DIFF_FORMAT_DIFFSTAT;
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ ವಿಲೀನ_SHOW_COMPACTSUMMARY: /* 2 */
			opts.output_format |= DIFF_FORMAT_DIFFSTAT;
			opts.flags.stat_with_summary = 1;
			ನಿಲ್ಲಿಸು;
		ಪೂರ್ವನಿಯೋಜಿತ:
			ನಿಲ್ಲಿಸು;
		}
		opts.detect_rename = DIFF_DETECT_RENAME;
		diff_ಸಿದ್ಧತೆ_ಮುಗಿದ(&opts);
		diff_ವೃಕ್ಷ_oid(ತಲೆ, new_ತಲೆ, "", &opts);
		diffcore_std(&opts);
		diff_flush(&opts);
	}

	/* Run a post-merge hook */
	ಸತತ_hooks_l(the_repository, "post-merge", squash ? "1" : "0", ಖಾಲಿ);

	ಆದರೆ (new_ತಲೆ)
		apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
				    ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ);
	strbuf_release(&reflog_ಸಂದೇಶ_2);
}

/* Get the name for the merge commit's message. */
ಸ್ಥಿರ ಶೂನ್ಯ ವಿಲೀನ_name(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *remote, ರಚನೆ strbuf *ಸಂದೇಶ)
{
	ರಚನೆ commit *remote_ತಲೆ;
	ರಚನೆ object_id branch_ತಲೆ;
	ರಚನೆ strbuf bname = STRBUF_INIT;
	ರಚನೆ ವಿಲೀನ_remote_desc *desc;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ptr;
	ಅಕ್ಷರ *ಸಿಕ್ಕಿತು_ref = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ len, early;

	copy_branchname(the_repository, &bname, remote, 0);
	remote = bname.buf;

	oidclr(&branch_ತಲೆ, the_repository->hash_algo);
	remote_ತಲೆ = get_ವಿಲೀನ_ಪೋಷಕ(remote);
	ಆದರೆ (!remote_ತಲೆ)
		die(_("'%s' does not point to a commit"), remote);

	ಆದರೆ (repo_dwim_ref(the_repository, remote, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(remote), &branch_ತಲೆ,
			  &ಸಿಕ್ಕಿತು_ref, 0) > 0) {
		ಆದರೆ (starts_with(ಸಿಕ್ಕಿತು_ref, "refs/heads/")) {
			strbuf_addf(ಸಂದೇಶ, "%s\t\tbranch '%s' of .\n",
				    oid_ಗಮ್ಯ_hex(&branch_ತಲೆ), remote);
			ಹೋಗು cleanup;
		}
		ಆದರೆ (starts_with(ಸಿಕ್ಕಿತು_ref, "refs/tags/")) {
			strbuf_addf(ಸಂದೇಶ, "%s\t\ttag '%s' of .\n",
				    oid_ಗಮ್ಯ_hex(&branch_ತಲೆ), remote);
			ಹೋಗು cleanup;
		}
		ಆದರೆ (starts_with(ಸಿಕ್ಕಿತು_ref, "refs/remotes/")) {
			strbuf_addf(ಸಂದೇಶ, "%s\t\tremote-tracking branch '%s' of .\n",
				    oid_ಗಮ್ಯ_hex(&branch_ತಲೆ), remote);
			ಹೋಗು cleanup;
		}
	}

	/* See if remote matches <name>^^^.. or <name>~<number> */
	ಗಾಗಿ (len = 0, ptr = remote + ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(remote);
	     remote < ptr && ptr[-1] == '^';
	     ptr--)
		len++;
	ಆದರೆ (len)
		early = 1;
	ಇಲ್ಲದಿದ್ದರೆ {
		early = 0;
		ptr = strrchr(remote, '~');
		ಆದರೆ (ptr) {
			ಪೂರ್ಣಾಂಕ ನೋಡಿದ_nonzero = 0;

			len++; /* count ~ */
			ಆಗ (*++ptr && ಅಂಕಿಯೇ(*ptr)) {
				ನೋಡಿದ_nonzero |= (*ptr != '0');
				len++;
			}
			ಆದರೆ (*ptr)
				len = 0; /* not ...~<number> */
			ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ನೋಡಿದ_nonzero)
				early = 1;
			ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (len == 1)
				early = 1; /* "name~" is "name~1"! */
		}
	}
	ಆದರೆ (len) {
		ರಚನೆ strbuf truname = STRBUF_INIT;
		strbuf_addf(&truname, "refs/heads/%s", remote);
		strbuf_setlen(&truname, truname.len - len);
		ಆದರೆ (refs_ref_exists(get_main_ref_store(the_repository), truname.buf)) {
			strbuf_addf(ಸಂದೇಶ,
				    "%s\t\tbranch '%s'%s of .\n",
				    oid_ಗಮ್ಯ_hex(&remote_ತಲೆ->object.oid),
				    truname.buf + 11,
				    (early ? " (early part)" : ""));
			strbuf_release(&truname);
			ಹೋಗು cleanup;
		}
		strbuf_release(&truname);
	}

	desc = ವಿಲೀನ_remote_util(remote_ತಲೆ);
	ಆದರೆ (desc && desc->obj && desc->obj->type == OBJ_ಟ್ಯಾಗ್) {
		strbuf_addf(ಸಂದೇಶ, "%s\t\t%s '%s'\n",
			    oid_ಗಮ್ಯ_hex(&desc->obj->oid),
			    type_name(desc->obj->type),
			    remote);
		ಹೋಗು cleanup;
	}

	strbuf_addf(ಸಂದೇಶ, "%s\t\tcommit '%s'\n",
		oid_ಗಮ್ಯ_hex(&remote_ತಲೆ->object.oid), remote);
cleanup:
	ಬಿಡುಗಡೆ(ಸಿಕ್ಕಿತು_ref);
	strbuf_release(&bname);
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_branch_ವಿಲೀನ_options(ಅಕ್ಷರ *bmo)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv;
	ಪೂರ್ಣಾಂಕ argc;

	ಆದರೆ (!bmo)
		ಹಿಂತಿರುಗಿ;
	argc = ವಿಭಜಿಸು_cmdline(bmo, &argv);
	ಆದರೆ (argc < 0)
		die(_("Bad branch.%s.mergeoptions string: %s"), branch,
		    _(ವಿಭಜಿಸು_cmdline_strerror(argc)));
	REALLOC_ARRAY(argv, argc + 2);
	MOVE_ARRAY(argv + 1, argv, argc + 1);
	argc++;
	argv[0] = "branch.*.mergeoptions";
	parse_options(argc, argv, ಖಾಲಿ, builtin_ವಿಲೀನ_options,
		      builtin_ವಿಲೀನ_usage, 0);
	ಬಿಡುಗಡೆ(argv);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ git_ವಿಲೀನ_config(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *k, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *v,
			    ಸ್ಥಿರಾಂಕ ರಚನೆ config_context *ctx, ಶೂನ್ಯ *cb)
{
	ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *str;

	ಆದರೆ (branch &&
	    skip_ಪೂರ್ವಪ್ರತ್ಯಯ(k, "branch.", &str) &&
	    skip_ಪೂರ್ವಪ್ರತ್ಯಯ(str, branch, &str) &&
	    !strcmp(str, ".mergeoptions")) {
		ಬಿಡುಗಡೆ(branch_mergeoptions);
		branch_mergeoptions = xstrdup(v);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (!strcmp(k, "merge.diffstat") || !strcmp(k, "merge.stat")) {
		ಪೂರ್ಣಾಂಕ val = git_parse_maybe_bool_ಪಠ್ಯ(v);
		ಆಯ್ಕೆ (val) {
		ಸ್ಥಿತಿ 0:
			show_diffstat = 0;
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ 1:
			show_diffstat = ವಿಲೀನ_SHOW_DIFFSTAT;
			ನಿಲ್ಲಿಸು;
		ಪೂರ್ವನಿಯೋಜಿತ:
			ಆದರೆ (!strcmp(v, "compact"))
				show_diffstat = ವಿಲೀನ_SHOW_COMPACTSUMMARY;
			/*
			 * We do not need to have an explicit
			 *
			 * else if (!strcmp(v, "diffstat"))
			 *	show_diffstat = MERGE_SHOW_DIFFSTAT;
			 *
			 * here, because the catch-all uses the
			 * diffstat style anyway.
			 */
			ಇಲ್ಲದಿದ್ದರೆ
				/*
				 * A setting from a future?  It is not an
				 * error grave enough to fail the command.
				 * proceed using the default one.
				 */
				show_diffstat = ವಿಲೀನ_SHOW_DIFFSTAT;
			ನಿಲ್ಲಿಸು;
		}
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(k, "merge.verifysignatures")) {
		verify_signatures = git_config_bool(k, v);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(k, "pull.twohead")) {
		FREE_AND_NULL(pull_twohead);
		ಹಿಂತಿರುಗಿ git_config_string(&pull_twohead, k, v);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(k, "pull.octopus")) {
		FREE_AND_NULL(pull_octopus);
		ಹಿಂತಿರುಗಿ git_config_string(&pull_octopus, k, v);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(k, "commit.cleanup")) {
		ಹಿಂತಿರುಗಿ git_config_string(&cleanup_arg, k, v);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(k, "merge.ff")) {
		ಪೂರ್ಣಾಂಕ boolval = git_parse_maybe_bool(v);
		ಆದರೆ (0 <= boolval) {
			fast_forward = boolval ? FF_ALLOW : FF_NO;
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (v && !strcmp(v, "only")) {
			fast_forward = FF_ONLY;
		} /* do not barf on values from future versions of git */
		ಹಿಂತಿರುಗಿ 0;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(k, "merge.defaulttoupstream")) {
		default_ಗಮ್ಯ_upstream = git_config_bool(k, v);
		ಹಿಂತಿರುಗಿ 0;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(k, "commit.gpgsign")) {
		sign_commit = git_config_bool(k, v) ? "" : ಖಾಲಿ;
		ಹಿಂತಿರುಗಿ 0;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(k, "gpg.mintrustlevel")) {
		check_trust_level = 0;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(k, "merge.autostash")) {
		autostash = git_config_bool(k, v);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಸ್ಥಿತಿ_2 = fmt_ವಿಲೀನ_ಸಂದೇಶ_config(k, v, ctx, cb);
	ಆದರೆ (ಸ್ಥಿತಿ_2)
		ಹಿಂತಿರುಗಿ ಸ್ಥಿತಿ_2;
	ಹಿಂತಿರುಗಿ git_diff_ui_config(k, v, ctx, cb);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ read_ವೃಕ್ಷ_trivial(ರಚನೆ object_id *common, ರಚನೆ object_id *ತಲೆ,
			     ರಚನೆ object_id *one)
{
	ಪೂರ್ಣಾಂಕ i, nr_trees = 0;
	ರಚನೆ ವೃಕ್ಷ *trees[MAX_UNPACK_TREES];
	ರಚನೆ ವೃಕ್ಷ_desc t[MAX_UNPACK_TREES];
	ರಚನೆ unpack_trees_options opts;

	memset(&opts, 0, ಗಾತ್ರ(opts));
	opts.ತಲೆ_idx = 2;
	opts.src_ಸೂಚ್ಯಂಕ = the_repository->ಸೂಚ್ಯಂಕ;
	opts.dst_ಸೂಚ್ಯಂಕ = the_repository->ಸೂಚ್ಯಂಕ;
	opts.update = 1;
	opts.verbose_update = 1;
	opts.trivial_merges_only = 1;
	opts.ವಿಲೀನ = 1;
	opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
	trees[nr_trees] = repo_parse_ವೃಕ್ಷ_ಪರೋಕ್ಷ(the_repository, common);
	ಆದರೆ (!trees[nr_trees++])
		ಹಿಂತಿರುಗಿ -1;
	trees[nr_trees] = repo_parse_ವೃಕ್ಷ_ಪರೋಕ್ಷ(the_repository, ತಲೆ);
	ಆದರೆ (!trees[nr_trees++])
		ಹಿಂತಿರುಗಿ -1;
	trees[nr_trees] = repo_parse_ವೃಕ್ಷ_ಪರೋಕ್ಷ(the_repository, one);
	ಆದರೆ (!trees[nr_trees++])
		ಹಿಂತಿರುಗಿ -1;
	opts.fn = threeway_ವಿಲೀನ;
	cache_ವೃಕ್ಷ_free(&the_repository->ಸೂಚ್ಯಂಕ->cache_ವೃಕ್ಷ);
	ಗಾಗಿ (i = 0; i < nr_trees; i++) {
		repo_parse_ವೃಕ್ಷ(the_repository, trees[i]);
		init_ವೃಕ್ಷ_desc(t+i, &trees[i]->object.oid,
			       trees[i]->ಬಫರ್, trees[i]->ಗಾತ್ರ_2);
	}
	ಆದರೆ (unpack_trees(nr_trees, t, &opts))
		ಹಿಂತಿರುಗಿ -1;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ write_ವೃಕ್ಷ_trivial(ರಚನೆ object_id *oid)
{
	ಆದರೆ (write_ಸೂಚ್ಯಂಕ_as_ವೃಕ್ಷ(oid, the_repository->ಸೂಚ್ಯಂಕ,
				repo_get_ಸೂಚ್ಯಂಕ_file(the_repository),
				0, ಖಾಲಿ))
		die(_("git write-tree failed to write a tree"));
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ try_ವಿಲೀನ_strategy(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *strategy, ರಚನೆ commit_list *common,
			      ರಚನೆ commit_list *remoteheads,
			      ರಚನೆ commit *ತಲೆ)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ತಲೆ_arg = "HEAD";

	ಆದರೆ (repo_refresh_and_write_ಸೂಚ್ಯಂಕ(the_repository, REFRESH_QUIET,
					 SKIP_IF_UNCHANGED, 0, ಖಾಲಿ, ಖಾಲಿ,
					 ಖಾಲಿ) < 0)
		die(_("Unable to write index."));

	ಆದರೆ (!strcmp(strategy, "recursive") || !strcmp(strategy, "subtree") ||
	    !strcmp(strategy, "ort")) {
		ರಚನೆ ಬೀಗ_file ಬೀಗ = ಬೀಗ_INIT;
		ಪೂರ್ಣಾಂಕ clean, x;
		ರಚನೆ commit *ಫಲಿತಾಂಶ;
		ರಚನೆ commit_list *reversed = ಖಾಲಿ;
		ರಚನೆ ವಿಲೀನ_options o;
		ರಚನೆ commit_list *j;

		ಆದರೆ (remoteheads->ಮುಂದಿನ) {
			error(_("Not handling anything other than two heads merge."));
			ಹಿಂತಿರುಗಿ 2;
		}

		init_ui_ವಿಲೀನ_options(&o, the_repository);
		ಆದರೆ (!strcmp(strategy, "subtree"))
			o.subtree_ಸರಿಸು = "";

		o.show_rename_ಪ್ರಗತಿ =
			show_ಪ್ರಗತಿ == -1 ? isatty(2) : show_ಪ್ರಗತಿ;

		ಗಾಗಿ (x = 0; x < xopts.nr; x++)
			ಆದರೆ (parse_ವಿಲೀನ_opt(&o, xopts.v[x]))
				die(_("unknown strategy option: -X%s"), xopts.v[x]);

		o.branch1 = ತಲೆ_arg;
		o.branch2 = ವಿಲೀನ_remote_util(remoteheads->ಅಂಶ)->name;

		ಗಾಗಿ (j = common; j; j = j->ಮುಂದಿನ)
			commit_list_ಸೇರಿಸು(j->ಅಂಶ, &reversed);

		repo_hold_locked_ಸೂಚ್ಯಂಕ(the_repository, &ಬೀಗ,
				       ಬೀಗ_DIE_ON_ERROR);
		clean = ವಿಲೀನ_ort_recursive(&o, ತಲೆ, remoteheads->ಅಂಶ,
					    reversed, &ಫಲಿತಾಂಶ);
		commit_list_free(reversed);
		strbuf_release(&o.obuf);

		ಆದರೆ (clean < 0) {
			rollback_ಬೀಗ_file(&ಬೀಗ);
			ಹಿಂತಿರುಗಿ 2;
		}
		ಆದರೆ (write_locked_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ, &ಬೀಗ,
				       COMMIT_ಬೀಗ | SKIP_IF_UNCHANGED))
			die(_("unable to write %s"), repo_get_ಸೂಚ್ಯಂಕ_file(the_repository));
		ಹಿಂತಿರುಗಿ clean ? 0 : 1;
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಹಿಂತಿರುಗಿ try_ವಿಲೀನ_command(the_repository,
					 strategy, xopts.nr, xopts.v,
					 common, ತಲೆ_arg, remoteheads);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ ಎಣಿಕೆ_diff_files(ರಚನೆ diff_ಸರತಿ_struct *q,
			     ರಚನೆ diff_options *opt ಬಳಸದ, ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
	ಪೂರ್ಣಾಂಕ *ಎಣಿಕೆ_2 = ದತ್ತಾಂಶ;

	(*ಎಣಿಕೆ_2) += q->nr;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_unmerged_entries(ಶೂನ್ಯ)
{
	ಪೂರ್ಣಾಂಕ i, ret = 0;

	ಗಾಗಿ (i = 0; i < the_repository->ಸೂಚ್ಯಂಕ->cache_nr; i++)
		ಆದರೆ (ce_stage(the_repository->ಸೂಚ್ಯಂಕ->cache[i]))
			ret++;

	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಶೂನ್ಯ ಕೂಡಿಸು_strategies(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *string, ಚಿಹ್ನೆರಹಿತ attr)
{
	ಪೂರ್ಣಾಂಕ i;

	ಆದರೆ (string) {
		ರಚನೆ string_list list = STRING_LIST_INIT_DUP;
		ರಚನೆ string_list_ಅಂಶ *ಅಂಶ;
		string_list_ವಿಭಜಿಸು(&list, string, " ", -1);
		for_each_string_list_ಅಂಶ(ಅಂಶ, &list)
			append_strategy(get_strategy(ಅಂಶ->string));
		string_list_clear(&list, 0);
		ಹಿಂತಿರುಗಿ;
	}
	ಗಾಗಿ (i = 0; i < ARRAY_ಗಾತ್ರ(all_strategy); i++)
		ಆದರೆ (all_strategy[i].attr & attr)
			append_strategy(&all_strategy[i]);

}

ಸ್ಥಿರ ಶೂನ್ಯ read_ವಿಲೀನ_ಸಂದೇಶ(ರಚನೆ strbuf *ಸಂದೇಶ)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *filename = git_ಮಾರ್ಗ_ವಿಲೀನ_ಸಂದೇಶ(the_repository);
	strbuf_reset(ಸಂದೇಶ);
	ಆದರೆ (strbuf_read_file(ಸಂದೇಶ, filename, 0) < 0)
		die_errno(_("Could not read from '%s'"), filename);
}

ಸ್ಥಿರ ಶೂನ್ಯ write_ವಿಲೀನ_state(ರಚನೆ commit_list *);
ಸ್ಥಿರ ಶೂನ್ಯ abort_commit(ರಚನೆ commit_list *remoteheads, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *err_ಸಂದೇಶ)
{
	ಆದರೆ (err_ಸಂದೇಶ)
		error("%s", err_ಸಂದೇಶ);
	fprintf(stderr,
		_("Not committing merge; use 'git commit' to complete the merge.\n"));
	write_ವಿಲೀನ_state(remoteheads);
	ನಿರ್ಗಮಿಸು(1);
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ ವಿಲೀನ_editor_comment[] =
N_("Please enter a commit message to explain why this merge is necessary,\n"
   "especially if it merges an updated upstream into a topic branch.\n"
   "\n");

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ scissors_editor_comment[] =
N_("An empty message aborts the commit.\n");

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ no_scissors_editor_comment[] =
N_("Lines starting with '%s' will be ignored, and an empty message aborts\n"
   "the commit.\n");

ಸ್ಥಿರ ಶೂನ್ಯ write_ವಿಲೀನ_heads(ರಚನೆ commit_list *);
ಸ್ಥಿರ ಶೂನ್ಯ prepare_ಗಮ್ಯ_commit(ರಚನೆ commit_list *remoteheads)
{
	ರಚನೆ strbuf ಸಂದೇಶ = STRBUF_INIT;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸೂಚ್ಯಂಕ_file = repo_get_ಸೂಚ್ಯಂಕ_file(the_repository);

	ಆದರೆ (!no_verify) {
		ಪೂರ್ಣಾಂಕ invoked_hook;

		ಆದರೆ (ಸತತ_commit_hook(0 < option_edit, ಸೂಚ್ಯಂಕ_file, &invoked_hook,
				    "pre-merge-commit", ಖಾಲಿ))
			abort_commit(remoteheads, ಖಾಲಿ);
		/*
		 * Re-read the index as pre-merge-commit hook could have updated it,
		 * and write it out as a tree.  We must do this before we invoke
		 * the editor and after we invoke run_status above.
		 */
		ಆದರೆ (invoked_hook)
			discard_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ);
	}
	read_ಸೂಚ್ಯಂಕ_from(the_repository->ಸೂಚ್ಯಂಕ, ಸೂಚ್ಯಂಕ_file,
			repo_get_git_dir(the_repository));
	strbuf_addbuf(&ಸಂದೇಶ, &ವಿಲೀನ_ಸಂದೇಶ);
	ಆದರೆ (squash)
		BUG("the control must not reach here under --squash");
	ಆದರೆ (0 < option_edit) {
		strbuf_addch(&ಸಂದೇಶ, '\n');
		ಆದರೆ (cleanup_ಬಹುಲಕ == COMMIT_ಸಂದೇಶ_CLEANUP_SCISSORS) {
			wt_ಸ್ಥಿತಿ_append_cut_ಸಾಲು(&ಸಂದೇಶ);
			strbuf_commented_addf(&ಸಂದೇಶ, comment_ಸಾಲು_str, "\n");
		}
		strbuf_commented_addf(&ಸಂದೇಶ, comment_ಸಾಲು_str,
				      _(ವಿಲೀನ_editor_comment));
		ಆದರೆ (cleanup_ಬಹುಲಕ == COMMIT_ಸಂದೇಶ_CLEANUP_SCISSORS)
			strbuf_commented_addf(&ಸಂದೇಶ, comment_ಸಾಲು_str,
					      _(scissors_editor_comment));
		ಇಲ್ಲದಿದ್ದರೆ
			strbuf_commented_addf(&ಸಂದೇಶ, comment_ಸಾಲು_str,
				_(no_scissors_editor_comment), comment_ಸಾಲು_str);
	}
	ಆದರೆ (signoff)
		append_signoff(&ಸಂದೇಶ, ignored_log_ಸಂದೇಶ_ಬೈಟುಗಳು(ಸಂದೇಶ.buf, ಸಂದೇಶ.len), 0);
	write_ವಿಲೀನ_heads(remoteheads);
	write_file_buf(git_ಮಾರ್ಗ_ವಿಲೀನ_ಸಂದೇಶ(the_repository), ಸಂದೇಶ.buf, ಸಂದೇಶ.len);
	ಆದರೆ (ಸತತ_commit_hook(0 < option_edit, repo_get_ಸೂಚ್ಯಂಕ_file(the_repository),
			    ಖಾಲಿ, "prepare-commit-msg",
			    git_ಮಾರ್ಗ_ವಿಲೀನ_ಸಂದೇಶ(the_repository), "merge", ಖಾಲಿ))
		abort_commit(remoteheads, ಖಾಲಿ);
	ಆದರೆ (0 < option_edit) {
		ಆದರೆ (launch_editor(git_ಮಾರ್ಗ_ವಿಲೀನ_ಸಂದೇಶ(the_repository), ಖಾಲಿ, ಖಾಲಿ))
			abort_commit(remoteheads, ಖಾಲಿ);
	}

	ಆದರೆ (!no_verify && ಸತತ_commit_hook(0 < option_edit, repo_get_ಸೂಚ್ಯಂಕ_file(the_repository),
					  ಖಾಲಿ, "commit-msg",
					  git_ಮಾರ್ಗ_ವಿಲೀನ_ಸಂದೇಶ(the_repository), ಖಾಲಿ))
		abort_commit(remoteheads, ಖಾಲಿ);

	read_ವಿಲೀನ_ಸಂದೇಶ(&ಸಂದೇಶ);
	cleanup_ಸಂದೇಶ(&ಸಂದೇಶ, cleanup_ಬಹುಲಕ, 0);
	ಆದರೆ (!ಸಂದೇಶ.len)
		abort_commit(remoteheads, _("Empty commit message."));
	strbuf_release(&ವಿಲೀನ_ಸಂದೇಶ);
	strbuf_addbuf(&ವಿಲೀನ_ಸಂದೇಶ, &ಸಂದೇಶ);
	strbuf_release(&ಸಂದೇಶ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ವಿಲೀನ_trivial(ರಚನೆ commit *ತಲೆ, ರಚನೆ commit_list *remoteheads)
{
	ರಚನೆ object_id ಫಲಿತಾಂಶ_ವೃಕ್ಷ, ಫಲಿತಾಂಶ_commit;
	ರಚನೆ commit_list *parents = ಖಾಲಿ, **pptr = &parents;

	ಆದರೆ (repo_refresh_and_write_ಸೂಚ್ಯಂಕ(the_repository, REFRESH_QUIET,
					 SKIP_IF_UNCHANGED, 0, ಖಾಲಿ, ಖಾಲಿ,
					 ಖಾಲಿ) < 0)
		ಹಿಂತಿರುಗಿ error(_("Unable to write index."));

	write_ವೃಕ್ಷ_trivial(&ಫಲಿತಾಂಶ_ವೃಕ್ಷ);
	ಬರೆ(_("Wonderful.\n"));
	pptr = commit_list_append(ತಲೆ, pptr);
	pptr = commit_list_append(remoteheads->ಅಂಶ, pptr);
	prepare_ಗಮ್ಯ_commit(remoteheads);
	ಆದರೆ (commit_ವೃಕ್ಷ(ವಿಲೀನ_ಸಂದೇಶ.buf, ವಿಲೀನ_ಸಂದೇಶ.len, &ಫಲಿತಾಂಶ_ವೃಕ್ಷ, parents,
			&ಫಲಿತಾಂಶ_commit, ಖಾಲಿ, sign_commit))
		die(_("failed to write commit object"));
	ಮುಕ್ತಾಯ(ತಲೆ, remoteheads, &ಫಲಿತಾಂಶ_commit, "In-index merge");

	remove_ವಿಲೀನ_branch_state(the_repository);
	commit_list_free(parents);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಮುಕ್ತಾಯ_automerge(ರಚನೆ commit *ತಲೆ,
			    ಪೂರ್ಣಾಂಕ ತಲೆ_subsumed,
			    ರಚನೆ commit_list *common,
			    ರಚನೆ commit_list *remoteheads,
			    ರಚನೆ object_id *ಫಲಿತಾಂಶ_ವೃಕ್ಷ,
			    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *wt_strategy)
{
	ರಚನೆ commit_list *parents = ಖಾಲಿ;
	ರಚನೆ strbuf buf = STRBUF_INIT;
	ರಚನೆ object_id ಫಲಿತಾಂಶ_commit;

	write_ವೃಕ್ಷ_trivial(ಫಲಿತಾಂಶ_ವೃಕ್ಷ);
	commit_list_free(common);
	parents = remoteheads;
	ಆದರೆ (!ತಲೆ_subsumed || fast_forward == FF_NO)
		commit_list_ಸೇರಿಸು(ತಲೆ, &parents);
	prepare_ಗಮ್ಯ_commit(remoteheads);
	ಆದರೆ (commit_ವೃಕ್ಷ(ವಿಲೀನ_ಸಂದೇಶ.buf, ವಿಲೀನ_ಸಂದೇಶ.len, ಫಲಿತಾಂಶ_ವೃಕ್ಷ, parents,
			&ಫಲಿತಾಂಶ_commit, ಖಾಲಿ, sign_commit))
		die(_("failed to write commit object"));
	strbuf_addf(&buf, "Merge made by the '%s' strategy.", wt_strategy);
	ಮುಕ್ತಾಯ(ತಲೆ, remoteheads, &ಫಲಿತಾಂಶ_commit, buf.buf);

	strbuf_release(&buf);
	remove_ವಿಲೀನ_branch_state(the_repository);
	commit_list_free(parents);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ suggest_conflicts(ಶೂನ್ಯ)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *filename;
	FILE *fp;
	ರಚನೆ strbuf msgbuf = STRBUF_INIT;

	filename = git_ಮಾರ್ಗ_ವಿಲೀನ_ಸಂದೇಶ(the_repository);
	fp = xfopen(filename, "a");

	/*
	 * We can't use cleanup_mode because if we're not using the editor,
	 * get_cleanup_mode will return COMMIT_MSG_CLEANUP_SPACE instead, even
	 * though the message is meant to be processed later by git-commit.
	 * Thus, we will get the cleanup mode which is returned when we _are_
	 * using an editor.
	 */
	append_conflicts_hint(the_repository->ಸೂಚ್ಯಂಕ, &msgbuf,
			      get_cleanup_ಬಹುಲಕ(cleanup_arg, 1));
	fputs(msgbuf.buf, fp);
	strbuf_release(&msgbuf);
	fclose(fp);
	repo_rerere(the_repository, allow_rerere_auto);
	ಬರೆ(_("Automatic merge failed; "
			"fix conflicts and then commit the result.\n"));
	ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ evaluate_ಫಲಿತಾಂಶ(ಶೂನ್ಯ)
{
	ಪೂರ್ಣಾಂಕ cnt = 0;
	ರಚನೆ rev_info rev;

	/* Check how many files differ. */
	repo_init_revisions(the_repository, &rev, "");
	ಸಿದ್ಧತೆ_revisions(0, ಖಾಲಿ, &rev, ಖಾಲಿ);
	rev.diffopt.output_format |=
		DIFF_FORMAT_CALLBACK;
	rev.diffopt.format_callback = ಎಣಿಕೆ_diff_files;
	rev.diffopt.format_callback_ದತ್ತಾಂಶ = &cnt;
	ಸತತ_diff_files(&rev, 0);

	/*
	 * Check how many unmerged entries are
	 * there.
	 */
	cnt += ಎಣಿಕೆ_unmerged_entries();

	release_revisions(&rev);
	ಹಿಂತಿರುಗಿ cnt;
}

/*
 * Pretend as if the user told us to merge with the remote-tracking
 * branch we have for the upstream of the current branch
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಸಿದ್ಧತೆ_with_upstream(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ ***argv)
{
	ರಚನೆ branch *branch = branch_get(ಖಾಲಿ);
	ಪೂರ್ಣಾಂಕ i;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **ವಾದಗಳು;

	ಆದರೆ (!branch)
		die(_("No current branch."));
	ಆದರೆ (!branch->remote_name)
		die(_("No remote for the current branch."));
	ಆದರೆ (!branch->ವಿಲೀನ_nr)
		die(_("No default upstream defined for the current branch."));

	ವಾದಗಳು = xcalloc(st_ಕೂಡಿಸು(branch->ವಿಲೀನ_nr, 1), ಗಾತ್ರ(ಅಕ್ಷರ *));
	ಗಾಗಿ (i = 0; i < branch->ವಿಲೀನ_nr; i++) {
		ಆದರೆ (!branch->ವಿಲೀನ[i]->dst)
			die(_("No remote-tracking branch for %s from %s"),
			    branch->ವಿಲೀನ[i]->src, branch->remote_name);
		ವಾದಗಳು[i] = branch->ವಿಲೀನ[i]->dst;
	}
	ವಾದಗಳು[i] = ಖಾಲಿ;
	*argv = ವಾದಗಳು;
	ಹಿಂತಿರುಗಿ i;
}

ಸ್ಥಿರ ಶೂನ್ಯ write_ವಿಲೀನ_heads(ರಚನೆ commit_list *remoteheads)
{
	ರಚನೆ commit_list *j;
	ರಚನೆ strbuf buf = STRBUF_INIT;

	ಗಾಗಿ (j = remoteheads; j; j = j->ಮುಂದಿನ) {
		ರಚನೆ object_id *oid;
		ರಚನೆ commit *c = j->ಅಂಶ;
		ರಚನೆ ವಿಲೀನ_remote_desc *desc;

		desc = ವಿಲೀನ_remote_util(c);
		ಆದರೆ (desc && desc->obj) {
			oid = &desc->obj->oid;
		} ಇಲ್ಲದಿದ್ದರೆ {
			oid = &c->object.oid;
		}
		strbuf_addf(&buf, "%s\n", oid_ಗಮ್ಯ_hex(oid));
	}
	write_file_buf(git_ಮಾರ್ಗ_ವಿಲೀನ_ತಲೆ(the_repository), buf.buf, buf.len);

	strbuf_reset(&buf);
	ಆದರೆ (fast_forward == FF_NO)
		strbuf_addstr(&buf, "no-ff");
	write_file_buf(git_ಮಾರ್ಗ_ವಿಲೀನ_ಬಹುಲಕ(the_repository), buf.buf, buf.len);
	strbuf_release(&buf);
}

ಸ್ಥಿರ ಶೂನ್ಯ write_ವಿಲೀನ_state(ರಚನೆ commit_list *remoteheads)
{
	write_ವಿಲೀನ_heads(remoteheads);
	strbuf_addch(&ವಿಲೀನ_ಸಂದೇಶ, '\n');
	write_file_buf(git_ಮಾರ್ಗ_ವಿಲೀನ_ಸಂದೇಶ(the_repository), ವಿಲೀನ_ಸಂದೇಶ.buf,
		       ವಿಲೀನ_ಸಂದೇಶ.len);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ default_edit_option(ಶೂನ್ಯ)
{
	ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ name[] = "GIT_MERGE_AUTOEDIT";
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *e = getenv(name);
	ರಚನೆ stat st_stdin, st_stdout;

	ಆದರೆ (have_ಸಂದೇಶ)
		/* an explicit -m msg without --[no-]edit */
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (e) {
		ಪೂರ್ಣಾಂಕ v = git_parse_maybe_bool(e);
		ಆದರೆ (v < 0)
			die(_("Bad value '%s' in environment '%s'"), e, name);
		ಹಿಂತಿರುಗಿ v;
	}

	/* Use editor if stdin and stdout are the same and is a tty */
	ಹಿಂತಿರುಗಿ (!fstat(0, &st_stdin) &&
		!fstat(1, &st_stdout) &&
		isatty(0) && isatty(1) &&
		st_stdin.st_dev == st_stdout.st_dev &&
		st_stdin.st_ino == st_stdout.st_ino &&
		st_stdin.st_ಬಹುಲಕ == st_stdout.st_ಬಹುಲಕ);
}

ಸ್ಥಿರ ರಚನೆ commit_list *reduce_parents(ರಚನೆ commit *ತಲೆ_commit,
					  ಪೂರ್ಣಾಂಕ *ತಲೆ_subsumed,
					  ರಚನೆ commit_list *remoteheads)
{
	ರಚನೆ commit_list *parents, **remotes;

	/*
	 * Is the current HEAD reachable from another commit being
	 * merged?  If so we do not want to record it as a parent of
	 * the resulting merge, unless --no-ff is given.  We will flip
	 * this variable to 0 when we find HEAD among the independent
	 * tips being merged.
	 */
	*ತಲೆ_subsumed = 1;

	/* Find what parents to record by checking independent ones. */
	parents = reduce_heads(remoteheads);
	commit_list_free(remoteheads);

	remoteheads = ಖಾಲಿ;
	remotes = &remoteheads;
	ಆಗ (parents) {
		ರಚನೆ commit *commit = ತೆಗೆ_commit(&parents);
		ಆದರೆ (commit == ತಲೆ_commit)
			*ತಲೆ_subsumed = 0;
		ಇಲ್ಲದಿದ್ದರೆ
			remotes = &commit_list_ಸೇರಿಸು(commit, remotes)->ಮುಂದಿನ;
	}
	ಹಿಂತಿರುಗಿ remoteheads;
}

ಸ್ಥಿರ ಶೂನ್ಯ prepare_ವಿಲೀನ_ಸಂದೇಶ(ರಚನೆ strbuf *ವಿಲೀನ_ಹೆಸರುಗಳು, ರಚನೆ strbuf *ವಿಲೀನ_ಸಂದೇಶ)
{
	ರಚನೆ fmt_ವಿಲೀನ_ಸಂದೇಶ_opts opts;

	memset(&opts, 0, ಗಾತ್ರ(opts));
	opts.ಕೂಡಿಸು_title = !have_ಸಂದೇಶ;
	opts.shortlog_len = shortlog_len;
	opts.credit_people = (0 < option_edit);
	opts.into_name = into_name;

	fmt_ವಿಲೀನ_ಸಂದೇಶ(ವಿಲೀನ_ಹೆಸರುಗಳು, ವಿಲೀನ_ಸಂದೇಶ, &opts);
	ಆದರೆ (ವಿಲೀನ_ಸಂದೇಶ->len)
		strbuf_setlen(ವಿಲೀನ_ಸಂದೇಶ, ವಿಲೀನ_ಸಂದೇಶ->len - 1);
}

ಸ್ಥಿರ ಶೂನ್ಯ handle_fetch_ತಲೆ(ರಚನೆ commit_list **remotes, ರಚನೆ strbuf *ವಿಲೀನ_ಹೆಸರುಗಳು)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *filename;
	ಪೂರ್ಣಾಂಕ fd, pos, npos;
	ರಚನೆ strbuf fetch_ತಲೆ_file = STRBUF_INIT;
	ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ hexsz = the_hash_algo->hexsz;

	ಆದರೆ (!ವಿಲೀನ_ಹೆಸರುಗಳು)
		ವಿಲೀನ_ಹೆಸರುಗಳು = &fetch_ತಲೆ_file;

	filename = git_ಮಾರ್ಗ_fetch_ತಲೆ(the_repository);
	fd = xopen(filename, O_RDONLY);

	ಆದರೆ (strbuf_read(ವಿಲೀನ_ಹೆಸರುಗಳು, fd, 0) < 0)
		die_errno(_("could not read '%s'"), filename);
	ಆದರೆ (close(fd) < 0)
		die_errno(_("could not close '%s'"), filename);

	ಗಾಗಿ (pos = 0; pos < ವಿಲೀನ_ಹೆಸರುಗಳು->len; pos = npos) {
		ರಚನೆ object_id oid;
		ಅಕ್ಷರ *ptr;
		ರಚನೆ commit *commit;

		ptr = strchr(ವಿಲೀನ_ಹೆಸರುಗಳು->buf + pos, '\n');
		ಆದರೆ (ptr)
			npos = ptr - ವಿಲೀನ_ಹೆಸರುಗಳು->buf + 1;
		ಇಲ್ಲದಿದ್ದರೆ
			npos = ವಿಲೀನ_ಹೆಸರುಗಳು->len;

		ಆದರೆ (npos - pos < hexsz + 2 ||
		    get_oid_hex(ವಿಲೀನ_ಹೆಸರುಗಳು->buf + pos, &oid))
			commit = ಖಾಲಿ; /* bad */
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (memcmp(ವಿಲೀನ_ಹೆಸರುಗಳು->buf + pos + hexsz, "\t\t", 2))
			ಮುಂದುವರಿ; /* not-for-merge */
		ಇಲ್ಲದಿದ್ದರೆ {
			ಅಕ್ಷರ saved = ವಿಲೀನ_ಹೆಸರುಗಳು->buf[pos + hexsz];
			ವಿಲೀನ_ಹೆಸರುಗಳು->buf[pos + hexsz] = '\0';
			commit = get_ವಿಲೀನ_ಪೋಷಕ(ವಿಲೀನ_ಹೆಸರುಗಳು->buf + pos);
			ವಿಲೀನ_ಹೆಸರುಗಳು->buf[pos + hexsz] = saved;
		}
		ಆದರೆ (!commit) {
			ಆದರೆ (ptr)
				*ptr = '\0';
			die(_("not something we can merge in %s: %s"),
			    filename, ವಿಲೀನ_ಹೆಸರುಗಳು->buf + pos);
		}
		remotes = &commit_list_ಸೇರಿಸು(commit, remotes)->ಮುಂದಿನ;
	}

	ಆದರೆ (ವಿಲೀನ_ಹೆಸರುಗಳು == &fetch_ತಲೆ_file)
		strbuf_release(&fetch_ತಲೆ_file);
}

ಸ್ಥಿರ ರಚನೆ commit_list *collect_parents(ರಚನೆ commit *ತಲೆ_commit,
					   ಪೂರ್ಣಾಂಕ *ತಲೆ_subsumed,
					   ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
					   ರಚನೆ strbuf *ವಿಲೀನ_ಸಂದೇಶ)
{
	ಪೂರ್ಣಾಂಕ i;
	ರಚನೆ commit_list *remoteheads = ಖಾಲಿ;
	ರಚನೆ commit_list **remotes = &remoteheads;
	ರಚನೆ strbuf ವಿಲೀನ_ಹೆಸರುಗಳು = STRBUF_INIT, *autogen = ಖಾಲಿ;

	ಆದರೆ (ವಿಲೀನ_ಸಂದೇಶ && (!have_ಸಂದೇಶ || shortlog_len))
		autogen = &ವಿಲೀನ_ಹೆಸರುಗಳು;

	ಆದರೆ (ತಲೆ_commit)
		remotes = &commit_list_ಸೇರಿಸು(ತಲೆ_commit, remotes)->ಮುಂದಿನ;

	ಆದರೆ (argc == 1 && !strcmp(argv[0], "FETCH_HEAD")) {
		handle_fetch_ತಲೆ(remotes, autogen);
		remoteheads = reduce_parents(ತಲೆ_commit, ತಲೆ_subsumed, remoteheads);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಗಾಗಿ (i = 0; i < argc; i++) {
			ರಚನೆ commit *commit = get_ವಿಲೀನ_ಪೋಷಕ(argv[i]);
			ಆದರೆ (!commit)
				help_unknown_ref(argv[i], "merge",
						 _("not something we can merge"));
			remotes = &commit_list_ಸೇರಿಸು(commit, remotes)->ಮುಂದಿನ;
		}
		remoteheads = reduce_parents(ತಲೆ_commit, ತಲೆ_subsumed, remoteheads);
		ಆದರೆ (autogen) {
			ರಚನೆ commit_list *p;
			ಗಾಗಿ (p = remoteheads; p; p = p->ಮುಂದಿನ)
				ವಿಲೀನ_name(ವಿಲೀನ_remote_util(p->ಅಂಶ)->name, autogen);
		}
	}

	ಆದರೆ (autogen) {
		prepare_ವಿಲೀನ_ಸಂದೇಶ(autogen, ವಿಲೀನ_ಸಂದೇಶ);
		strbuf_release(autogen);
	}

	ಹಿಂತಿರುಗಿ remoteheads;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ merging_a_throwaway_ಟ್ಯಾಗ್(ರಚನೆ commit *commit)
{
	ಅಕ್ಷರ *ಟ್ಯಾಗ್_ref;
	ರಚನೆ object_id oid;
	ಪೂರ್ಣಾಂಕ is_throwaway_ಟ್ಯಾಗ್ = 0;

	/* Are we merging a tag? */
	ಆದರೆ (!ವಿಲೀನ_remote_util(commit) ||
	    !ವಿಲೀನ_remote_util(commit)->obj ||
	    ವಿಲೀನ_remote_util(commit)->obj->type != OBJ_ಟ್ಯಾಗ್)
		ಹಿಂತಿರುಗಿ is_throwaway_ಟ್ಯಾಗ್;

	/*
	 * Now we know we are merging a tag object.  Are we downstream
	 * and following the tags from upstream?  If so, we must have
	 * the tag object pointed at by "refs/tags/$T" where $T is the
	 * tagname recorded in the tag object.  We want to allow such
	 * a "just to catch up" merge to fast-forward.
	 *
	 * Otherwise, we are playing an integrator's role, making a
	 * merge with a throw-away tag from a contributor with
	 * something like "git pull $contributor $signed_tag".
	 * We want to forbid such a merge from fast-forwarding
	 * by default; otherwise we would not keep the signature
	 * anywhere.
	 */
	ಟ್ಯಾಗ್_ref = xstrfmt("refs/tags/%s",
			  ((ರಚನೆ ಟ್ಯಾಗ್ *)ವಿಲೀನ_remote_util(commit)->obj)->ಟ್ಯಾಗ್);
	ಆದರೆ (!refs_read_ref(get_main_ref_store(the_repository), ಟ್ಯಾಗ್_ref, &oid) &&
	    oideq(&oid, &ವಿಲೀನ_remote_util(commit)->obj->oid))
		is_throwaway_ಟ್ಯಾಗ್ = 0;
	ಇಲ್ಲದಿದ್ದರೆ
		is_throwaway_ಟ್ಯಾಗ್ = 1;
	ಬಿಡುಗಡೆ(ಟ್ಯಾಗ್_ref);
	ಹಿಂತಿರುಗಿ is_throwaway_ಟ್ಯಾಗ್;
}

ಪೂರ್ಣಾಂಕ cmd_ವಿಲೀನ(ಪೂರ್ಣಾಂಕ argc,
	      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
	      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
	      ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ object_id ಫಲಿತಾಂಶ_ವೃಕ್ಷ, stash, ತಲೆ_oid;
	ರಚನೆ commit *ತಲೆ_commit;
	ರಚನೆ strbuf buf = STRBUF_INIT;
	ಪೂರ್ಣಾಂಕ i, ret = 0, ತಲೆ_subsumed;
	ಪೂರ್ಣಾಂಕ ಉತ್ತಮ_cnt = -1, ವಿಲೀನ_was_ok = 0, automerge_was_ok = 0;
	ರಚನೆ commit_list *common = ಖಾಲಿ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಉತ್ತಮ_strategy = ಖಾಲಿ, *wt_strategy = ಖಾಲಿ;
	ರಚನೆ commit_list *remoteheads = ಖಾಲಿ, *p;
	ಶೂನ್ಯ *branch_ಗಮ್ಯ_free, *argv_ಗಮ್ಯ_free = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ orig_argc = argc;
	ಪೂರ್ಣಾಂಕ ವಿಲೀನ_log_config = -1;

	show_usage_with_options_if_asked(argc, argv,
					 builtin_ವಿಲೀನ_usage, builtin_ವಿಲೀನ_options);

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ WITH_BREAKING_CHANGES
	warn_on_auto_comment_char = true;
#ಆದರೆ_ಅಂತ್ಯ /* !WITH_BREAKING_CHANGES */
	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_ತುಂಬಿದ_ಸೂಚ್ಯಂಕ = 0;

	/*
	 * Check if we are _not_ on a detached HEAD, i.e. if there is a
	 * current branch.
	 */
	branch = branch_ಗಮ್ಯ_free = refs_resolve_refdup(get_main_ref_store(the_repository),
						      "HEAD", 0, &ತಲೆ_oid,
						      ಖಾಲಿ);
	ಆದರೆ (branch)
		skip_ಪೂರ್ವಪ್ರತ್ಯಯ(branch, "refs/heads/", &branch);

	init_diff_ui_defaults();
	repo_config(the_repository, git_ವಿಲೀನ_config, &ವಿಲೀನ_log_config);

	ಆದರೆ (!branch || is_null_oid(&ತಲೆ_oid))
		ತಲೆ_commit = ಖಾಲಿ;
	ಇಲ್ಲದಿದ್ದರೆ
		ತಲೆ_commit = lookup_commit_or_die(&ತಲೆ_oid, "HEAD");

	ಆದರೆ (branch_mergeoptions)
		parse_branch_ವಿಲೀನ_options(branch_mergeoptions);
	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, builtin_ವಿಲೀನ_options,
			builtin_ವಿಲೀನ_usage, 0);
	ಆದರೆ (shortlog_len < 0)
		shortlog_len = (ವಿಲೀನ_log_config > 0) ? ವಿಲೀನ_log_config : 0;

	ಆದರೆ (verbosity < 0 && show_ಪ್ರಗತಿ == -1)
		show_ಪ್ರಗತಿ = 0;

	ಆದರೆ (abort_current_ವಿಲೀನ) {
		ಪೂರ್ಣಾಂಕ nargc = 2;
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *nargv[] = {"reset", "--merge", ಖಾಲಿ};
		ಅಕ್ಷರ stash_oid_hex[GIT_MAX_HEXSZ + 1];
		ರಚನೆ object_id stash_oid = {0};

		ಆದರೆ (orig_argc != 2)
			usage_ಸಂದೇಶ_opt(_("--abort expects no arguments"),
			      builtin_ವಿಲೀನ_usage, builtin_ವಿಲೀನ_options);

		ಆದರೆ (!file_exists(git_ಮಾರ್ಗ_ವಿಲೀನ_ತಲೆ(the_repository)))
			die(_("There is no merge to abort (MERGE_HEAD missing)."));

		ಆದರೆ (!refs_read_ref(get_main_ref_store(the_repository), "MERGE_AUTOSTASH", &stash_oid))
			refs_delete_ref(get_main_ref_store(the_repository),
					"", "MERGE_AUTOSTASH", &stash_oid,
					REF_NO_DEREF);

		/* Invoke 'git reset --merge' */
		ret = cmd_reset(nargc, nargv, ಪೂರ್ವಪ್ರತ್ಯಯ, the_repository);

		ಆದರೆ (!is_null_oid(&stash_oid)) {
			oid_ಗಮ್ಯ_hex_r(stash_oid_hex, &stash_oid);
			apply_autostash_oid(stash_oid_hex);
		}

		ಹೋಗು ಮುಗಿದ;
	}

	ಆದರೆ (quit_current_ವಿಲೀನ) {
		ಆದರೆ (orig_argc != 2)
			usage_ಸಂದೇಶ_opt(_("--quit expects no arguments"),
				      builtin_ವಿಲೀನ_usage,
				      builtin_ವಿಲೀನ_options);

		remove_ವಿಲೀನ_branch_state(the_repository);
		ಹೋಗು ಮುಗಿದ;
	}

	ಆದರೆ (continue_current_ವಿಲೀನ) {
		ಪೂರ್ಣಾಂಕ nargc = 1;
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *nargv[] = {"commit", ಖಾಲಿ};

		ಆದರೆ (orig_argc != 2)
			usage_ಸಂದೇಶ_opt(_("--continue expects no arguments"),
			      builtin_ವಿಲೀನ_usage, builtin_ವಿಲೀನ_options);

		ಆದರೆ (!file_exists(git_ಮಾರ್ಗ_ವಿಲೀನ_ತಲೆ(the_repository)))
			die(_("There is no merge in progress (MERGE_HEAD missing)."));

		/* Invoke 'git commit' */
		ret = cmd_commit(nargc, nargv, ಪೂರ್ವಪ್ರತ್ಯಯ, the_repository);
		ಹೋಗು ಮುಗಿದ;
	}

	ಆದರೆ (repo_read_ಸೂಚ್ಯಂಕ_unmerged(the_repository))
		die_resolve_conflict("merge");

	ಆದರೆ (file_exists(git_ಮಾರ್ಗ_ವಿಲೀನ_ತಲೆ(the_repository))) {
		/*
		 * There is no unmerged entry, don't advise 'git
		 * add/rm <file>', just 'git commit'.
		 */
		ಆದರೆ (advice_enabled(ADVICE_RESOLVE_CONFLICT))
			die(_("You have not concluded your merge (MERGE_HEAD exists).\n"
				  "Please, commit your changes before you merge."));
		ಇಲ್ಲದಿದ್ದರೆ
			die(_("You have not concluded your merge (MERGE_HEAD exists)."));
	}
	ಆದರೆ (refs_ref_exists(get_main_ref_store(the_repository), "CHERRY_PICK_HEAD")) {
		ಆದರೆ (advice_enabled(ADVICE_RESOLVE_CONFLICT))
			die(_("You have not concluded your cherry-pick (CHERRY_PICK_HEAD exists).\n"
			    "Please, commit your changes before you merge."));
		ಇಲ್ಲದಿದ್ದರೆ
			die(_("You have not concluded your cherry-pick (CHERRY_PICK_HEAD exists)."));
	}
	resolve_undo_clear_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ);

	ಆದರೆ (option_edit < 0)
		option_edit = default_edit_option();

	cleanup_ಬಹುಲಕ = get_cleanup_ಬಹುಲಕ(cleanup_arg, 0 < option_edit);

	ಆದರೆ (verbosity < 0)
		show_diffstat = 0;

	ಆದರೆ (squash) {
		ಆದರೆ (fast_forward == FF_NO)
			die(_("options '%s' and '%s' cannot be used together"), "--squash", "--no-ff.");
		ಆದರೆ (option_commit > 0)
			die(_("options '%s' and '%s' cannot be used together"), "--squash", "--commit.");
		/*
		 * squash can now silently disable option_commit - this is not
		 * a problem as it is only overriding the default, not a user
		 * supplied option.
		 */
		option_commit = 0;
	}

	ಆದರೆ (option_commit < 0)
		option_commit = 1;

	ಆದರೆ (!argc) {
		ಆದರೆ (default_ಗಮ್ಯ_upstream) {
			argc = ಸಿದ್ಧತೆ_with_upstream(&argv);
			argv_ಗಮ್ಯ_free = argv;
		}
		ಇಲ್ಲದಿದ್ದರೆ
			die(_("No commit specified and merge.defaultToUpstream not set."));
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (argc == 1 && !strcmp(argv[0], "-")) {
		argv[0] = "@{-1}";
	}

	ಆದರೆ (!argc)
		usage_with_options(builtin_ವಿಲೀನ_usage,
			builtin_ವಿಲೀನ_options);

	ಆದರೆ (!ತಲೆ_commit) {
		/*
		 * If the merged head is a valid one there is no reason
		 * to forbid "git merge" into a branch yet to be born.
		 * We do the same for "git pull".
		 */
		ರಚನೆ object_id *remote_ತಲೆ_oid;
		ಆದರೆ (squash)
			die(_("Squash commit into empty head not supported yet"));
		ಆದರೆ (fast_forward == FF_NO)
			die(_("Non-fast-forward commit does not make sense into "
			    "an empty head"));
		remoteheads = collect_parents(ತಲೆ_commit, &ತಲೆ_subsumed,
					      argc, argv, ಖಾಲಿ);
		ಆದರೆ (!remoteheads)
			die(_("%s - not something we can merge"), argv[0]);
		ಆದರೆ (remoteheads->ಮುಂದಿನ)
			die(_("Can merge only exactly one commit into empty head"));

		ಆದರೆ (verify_signatures)
			verify_ವಿಲೀನ_signature(remoteheads->ಅಂಶ, verbosity,
					       check_trust_level);

		remote_ತಲೆ_oid = &remoteheads->ಅಂಶ->object.oid;
		read_ಖಾಲಿಗಳು(remote_ತಲೆ_oid);
		refs_update_ref(get_main_ref_store(the_repository),
				"initial pull", "HEAD", remote_ತಲೆ_oid, ಖಾಲಿ,
				0,
				UPDATE_REFS_DIE_ON_ERR);
		ಹೋಗು ಮುಗಿದ;
	}

	/*
	 * All the rest are the commits being merged; prepare
	 * the standard merge summary message to be appended
	 * to the given message.
	 */
	remoteheads = collect_parents(ತಲೆ_commit, &ತಲೆ_subsumed,
				      argc, argv, &ವಿಲೀನ_ಸಂದೇಶ);

	ಆದರೆ (!ತಲೆ_commit || !argc)
		usage_with_options(builtin_ವಿಲೀನ_usage,
			builtin_ವಿಲೀನ_options);

	ಆದರೆ (verify_signatures) {
		ಗಾಗಿ (p = remoteheads; p; p = p->ಮುಂದಿನ) {
			verify_ವಿಲೀನ_signature(p->ಅಂಶ, verbosity,
					       check_trust_level);
		}
	}

	strbuf_addstr(&buf, "merge");
	ಗಾಗಿ (p = remoteheads; p; p = p->ಮುಂದಿನ)
		strbuf_addf(&buf, " %s", ವಿಲೀನ_remote_util(p->ಅಂಶ)->name);
	setenv("GIT_REFLOG_ACTION", buf.buf, 0);
	strbuf_reset(&buf);

	ಗಾಗಿ (p = remoteheads; p; p = p->ಮುಂದಿನ) {
		ರಚನೆ commit *commit = p->ಅಂಶ;
		strbuf_addf(&buf, "GITHEAD_%s",
			    oid_ಗಮ್ಯ_hex(&commit->object.oid));
		setenv(buf.buf, ವಿಲೀನ_remote_util(commit)->name, 1);
		strbuf_reset(&buf);
		ಆದರೆ (fast_forward != FF_ONLY && merging_a_throwaway_ಟ್ಯಾಗ್(commit))
			fast_forward = FF_NO;
	}

	ಆದರೆ (!use_strategies) {
		ಆದರೆ (!remoteheads)
			; /* already up-to-date */
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!remoteheads->ಮುಂದಿನ)
			ಕೂಡಿಸು_strategies(pull_twohead, DEFAULT_TWOHEAD);
		ಇಲ್ಲದಿದ್ದರೆ
			ಕೂಡಿಸು_strategies(pull_octopus, DEFAULT_OCTOPUS);
	}

	ಗಾಗಿ (i = 0; i < use_strategies_nr; i++) {
		ಆದರೆ (use_strategies[i]->attr & NO_FAST_FORWARD)
			fast_forward = FF_NO;
		ಆದರೆ (use_strategies[i]->attr & NO_TRIVIAL)
			allow_trivial = 0;
	}

	ಆದರೆ (!remoteheads)
		; /* already up-to-date */
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!remoteheads->ಮುಂದಿನ) {
		ಆದರೆ (repo_get_ವಿಲೀನ_bases(the_repository, ತಲೆ_commit,
					 remoteheads->ಅಂಶ, &common) < 0) {
			ret = 2;
			ಹೋಗು ಮುಗಿದ;
		}
	} ಇಲ್ಲದಿದ್ದರೆ {
		ರಚನೆ commit_list *list = remoteheads;
		commit_list_ಸೇರಿಸು(ತಲೆ_commit, &list);
		ಆದರೆ (get_octopus_ವಿಲೀನ_bases(list, &common) < 0) {
			ಬಿಡುಗಡೆ(list);
			ret = 2;
			ಹೋಗು ಮುಗಿದ;
		}
		ಬಿಡುಗಡೆ(list);
	}

	refs_update_ref(get_main_ref_store(the_repository),
			"updating ORIG_HEAD", "ORIG_HEAD",
			&ತಲೆ_commit->object.oid, ಖಾಲಿ, 0,
			UPDATE_REFS_DIE_ON_ERR);

	ಆದರೆ (remoteheads && !common) {
		/* No common ancestors found. */
		ಆದರೆ (!allow_unrelated_histories)
			die(_("refusing to merge unrelated histories"));
		/* otherwise, we need a real merge. */
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!remoteheads ||
		 (!remoteheads->ಮುಂದಿನ && !common->ಮುಂದಿನ &&
		  common->ಅಂಶ == remoteheads->ಅಂಶ)) {
		/*
		 * If head can reach all the merge then we are up to date.
		 * but first the most common case of merging one remote.
		 */
		ಮುಕ್ತಾಯ_ಮೇಲೆ_ಗಮ್ಯ_date();
		ಹೋಗು ಮುಗಿದ;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (fast_forward != FF_NO && !remoteheads->ಮುಂದಿನ &&
			!common->ಮುಂದಿನ &&
			oideq(&common->ಅಂಶ->object.oid, &ತಲೆ_commit->object.oid)) {
		/* Again the most common case of merging one remote. */
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ = have_ಸಂದೇಶ ?
			"Fast-forward (no commit created; -m option ignored)" :
			"Fast-forward";
		ರಚನೆ commit *commit;

		ಆದರೆ (verbosity >= 0) {
			ಬರೆ(_("Updating %s..%s\n"),
			       repo_ಕಂಡುಹಿಡಿ_unique_abbrev(the_repository, &ತಲೆ_commit->object.oid,
						       DEFAULT_ABBREV),
			       repo_ಕಂಡುಹಿಡಿ_unique_abbrev(the_repository, &remoteheads->ಅಂಶ->object.oid,
						       DEFAULT_ABBREV));
		}
		commit = remoteheads->ಅಂಶ;
		ಆದರೆ (!commit) {
			ret = 1;
			ಹೋಗು ಮುಗಿದ;
		}

		ಆದರೆ (autostash)
			create_autostash_ref(the_repository, "MERGE_AUTOSTASH",
					     ಖಾಲಿ, false);
		ಆದರೆ (checkout_fast_forward(the_repository,
					  &ತಲೆ_commit->object.oid,
					  &commit->object.oid,
					  overwrite_ignore)) {
			apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
					    ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ);
			ret = 1;
			ಹೋಗು ಮುಗಿದ;
		}

		ಮುಕ್ತಾಯ(ತಲೆ_commit, remoteheads, &commit->object.oid, ಸಂದೇಶ);
		remove_ವಿಲೀನ_branch_state(the_repository);
		ಹೋಗು ಮುಗಿದ;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!remoteheads->ಮುಂದಿನ && common->ಮುಂದಿನ)
		;
		/*
		 * We are not doing octopus and not fast-forward.  Need
		 * a real merge.
		 */
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!remoteheads->ಮುಂದಿನ && !common->ಮುಂದಿನ && option_commit) {
		/*
		 * We are not doing octopus, not fast-forward, and have
		 * only one common.
		 */
		refresh_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ, REFRESH_QUIET, ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ);
		ಆದರೆ (allow_trivial && fast_forward != FF_ONLY) {
			/*
			 * Must first ensure that index matches HEAD before
			 * attempting a trivial merge.
			 */
			ರಚನೆ ವೃಕ್ಷ *ತಲೆ_ವೃಕ್ಷ = repo_get_commit_ವೃಕ್ಷ(the_repository,
								      ತಲೆ_commit);
			ರಚನೆ strbuf sb = STRBUF_INIT;

			ಆದರೆ (repo_ಸೂಚ್ಯಂಕ_has_changes(the_repository, ತಲೆ_ವೃಕ್ಷ,
						   &sb)) {
				error(_("Your local changes to the following files would be overwritten by merge:\n  %s"),
				      sb.buf);
				strbuf_release(&sb);
				ret = 2;
				ಹೋಗು ಮುಗಿದ;
			}

			/* See if it is really trivial. */
			git_committer_info(IDENT_STRICT);
			ಬರೆ(_("Trying really trivial in-index merge...\n"));
			ಆದರೆ (!read_ವೃಕ್ಷ_trivial(&common->ಅಂಶ->object.oid,
					       &ತಲೆ_commit->object.oid,
					       &remoteheads->ಅಂಶ->object.oid)) {
				ret = ವಿಲೀನ_trivial(ತಲೆ_commit, remoteheads);
				ಹೋಗು ಮುಗಿದ;
			}
			ಬರೆ(_("Nope.\n"));
		}
	} ಇಲ್ಲದಿದ್ದರೆ {
		/*
		 * An octopus.  If we can reach all the remote we are up
		 * to date.
		 */
		ಪೂರ್ಣಾಂಕ ಮೇಲೆ_ಗಮ್ಯ_date = 1;
		ರಚನೆ commit_list *j;

		ಗಾಗಿ (j = remoteheads; j; j = j->ಮುಂದಿನ) {
			ಪೂರ್ಣಾಂಕ ret = repo_ಒಳ_ವಿಲೀನ_bases(the_repository,
						      j->ಅಂಶ, ತಲೆ_commit);
			ಆದರೆ (ret < 0)
				ನಿರ್ಗಮಿಸು(128);
			ಆದರೆ (!ret) {
				ಮೇಲೆ_ಗಮ್ಯ_date = 0;
				ನಿಲ್ಲಿಸು;
			}
		}
		ಆದರೆ (ಮೇಲೆ_ಗಮ್ಯ_date) {
			ಮುಕ್ತಾಯ_ಮೇಲೆ_ಗಮ್ಯ_date();
			ಹೋಗು ಮುಗಿದ;
		}
	}

	ಆದರೆ (fast_forward == FF_ONLY)
		die_ff_impossible();

	ಆದರೆ (autostash)
		create_autostash_ref(the_repository, "MERGE_AUTOSTASH",
				     ಖಾಲಿ, false);

	/* We are going to make a new commit. */
	git_committer_info(IDENT_STRICT);

	/*
	 * At this point, we need a real merge.  No matter what strategy
	 * we use, it would operate on the index, possibly affecting the
	 * working tree, and when resolved cleanly, have the desired
	 * tree in the index -- this means that the index must be in
	 * sync with the head commit.  The strategies are responsible
	 * to ensure this.
	 *
	 * Stash away the local changes so that we can try more than one
	 * and/or recover from merge strategies bailing while leaving the
	 * index and working tree polluted.
	 */
	ಆದರೆ (save_state(&stash))
		oidclr(&stash, the_repository->hash_algo);

	ಗಾಗಿ (i = 0; i < use_strategies_nr; i++) {
		ಪೂರ್ಣಾಂಕ ret, cnt;
		ಆದರೆ (i) {
			ಬರೆ(_("Rewinding the tree to pristine...\n"));
			restore_state(&ತಲೆ_commit->object.oid, &stash);
		}
		ಆದರೆ (use_strategies_nr != 1)
			ಬರೆ(_("Trying merge strategy %s...\n"),
				use_strategies[i]->name);
		/*
		 * Remember which strategy left the state in the working
		 * tree.
		 */
		wt_strategy = use_strategies[i]->name;

		ret = try_ವಿಲೀನ_strategy(wt_strategy,
					 common, remoteheads,
					 ತಲೆ_commit);
		/*
		 * The backend exits with 1 when conflicts are
		 * left to be resolved, with 2 when it does not
		 * handle the given merge at all.
		 */
		ಆದರೆ (ret < 2) {
			ಆದರೆ (!ret) {
				/*
				 * This strategy worked; no point in trying
				 * another.
				 */
				ವಿಲೀನ_was_ok = 1;
				ಉತ್ತಮ_strategy = wt_strategy;
				ನಿಲ್ಲಿಸು;
			}
			cnt = (use_strategies_nr > 1) ? evaluate_ಫಲಿತಾಂಶ() : 0;
			ಆದರೆ (ಉತ್ತಮ_cnt <= 0 || cnt <= ಉತ್ತಮ_cnt) {
				ಉತ್ತಮ_strategy = wt_strategy;
				ಉತ್ತಮ_cnt = cnt;
			}
		}
	}

	/*
	 * If we have a resulting tree, that means the strategy module
	 * auto resolved the merge cleanly.
	 */
	ಆದರೆ (ವಿಲೀನ_was_ok && option_commit) {
		automerge_was_ok = 1;
		ret = ಮುಕ್ತಾಯ_automerge(ತಲೆ_commit, ತಲೆ_subsumed,
				       common, remoteheads,
				       &ಫಲಿತಾಂಶ_ವೃಕ್ಷ, wt_strategy);
		ಹೋಗು ಮುಗಿದ;
	}

	/*
	 * Pick the result from the best strategy and have the user fix
	 * it up.
	 */
	ಆದರೆ (!ಉತ್ತಮ_strategy) {
		restore_state(&ತಲೆ_commit->object.oid, &stash);
		ಆದರೆ (use_strategies_nr > 1)
			fprintf(stderr,
				_("No merge strategy handled the merge.\n"));
		ಇಲ್ಲದಿದ್ದರೆ
			fprintf(stderr, _("Merge with strategy %s failed.\n"),
				use_strategies[0]->name);
		apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
				    ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ);
		ret = 2;
		ಹೋಗು ಮುಗಿದ;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಉತ್ತಮ_strategy == wt_strategy)
		; /* We already have its result in the working tree. */
	ಇಲ್ಲದಿದ್ದರೆ {
		ಬರೆ(_("Rewinding the tree to pristine...\n"));
		restore_state(&ತಲೆ_commit->object.oid, &stash);
		ಬರೆ(_("Using the %s strategy to prepare resolving by hand.\n"),
			ಉತ್ತಮ_strategy);
		try_ವಿಲೀನ_strategy(ಉತ್ತಮ_strategy, common, remoteheads,
				   ತಲೆ_commit);
	}

	ಆದರೆ (squash) {
		ಮುಕ್ತಾಯ(ತಲೆ_commit, remoteheads, ಖಾಲಿ, ಖಾಲಿ);

		git_test_write_commit_ನಕ್ಷೆ_or_die(the_repository);
	} ಇಲ್ಲದಿದ್ದರೆ
		write_ವಿಲೀನ_state(remoteheads);

	ಆದರೆ (ವಿಲೀನ_was_ok)
		fprintf(stderr, _("Automatic merge went well; "
			"stopped before committing as requested\n"));
	ಇಲ್ಲದಿದ್ದರೆ
		ret = suggest_conflicts();
	ಆದರೆ (autostash)
		ಬರೆ(_("When finished, apply stashed changes with `git stash pop`\n"));

ಮುಗಿದ:
	ಆದರೆ (!automerge_was_ok) {
		commit_list_free(common);
		commit_list_free(remoteheads);
	}
	strbuf_release(&buf);
	ಬಿಡುಗಡೆ(branch_ಗಮ್ಯ_free);
	ಬಿಡುಗಡೆ(argv_ಗಮ್ಯ_free);
	ಬಿಡುಗಡೆ(pull_twohead);
	ಬಿಡುಗಡೆ(pull_octopus);
	discard_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ);
	ಹಿಂತಿರುಗಿ ret;
}
