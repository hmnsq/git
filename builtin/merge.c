/*
 * Builtin "git merge"
 *
 * Copyright (c) 2008 Miklos Vajna <vmiklos@frugalware.org>
 *
 * Based on git-merge.sh by Junio C Hamano.
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

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

#define DEFAULT_TWOHEAD (1<<0)
#define DEFAULT_OCTOPUS (1<<1)
#define NO_FAST_FORWARD (1<<2)
#define NO_TRIVIAL      (1<<3)

ಸಂರಚನಾ strategy {
	ಧ್ರುವ ವರ್ಣ *name;
	unsigned attr;
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ builtin_merge_usage[] = {
	N_("git merge [<options>] [<commit>...]"),
	"git merge --abort",
	"git merge --continue",
	ರಿಕ್ತ
};

#define MERGE_SHOW_DIFFSTAT 1
#define MERGE_SHOW_COMPACTSUMMARY 2

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_diffstat = MERGE_SHOW_DIFFSTAT, shortlog_len = -1, squash;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ option_commit = -1;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ option_edit = -1;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ allow_trivial = 1, have_message, verify_signatures;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ check_trust_level = 1;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ overwrite_ignore = 1;
ಸ್ಥಿರ ಸಂರಚನಾ strbuf merge_ಸಂದೇಶ = STRBUF_INIT;
ಸ್ಥಿರ ಸಂರಚನಾ strategy **use_strategies;
ಸ್ಥಿರ size_t use_strategies_nr, use_strategies_alloc;
ಸ್ಥಿರ ಸಂರಚನಾ strvec xopts = STRVEC_INIT;
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *branch;
ಸ್ಥಿರ ವರ್ಣ *branch_mergeoptions;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ verbosity;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ allow_rerere_auto;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ abort_current_merge;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ quit_current_merge;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ continue_current_merge;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ allow_unrelated_histories;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_progress = -1;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ default_to_upstream = 1;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ signoff;
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *sign_commit;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ autostash;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ no_verify;
ಸ್ಥಿರ ವರ್ಣ *into_name;

ಸ್ಥಿರ ಸಂರಚನಾ strategy all_strategy[] = {
	{ "recursive",  NO_TRIVIAL },
	{ "octopus",    DEFAULT_OCTOPUS },
	{ "ort",        DEFAULT_TWOHEAD | NO_TRIVIAL },
	{ "resolve",    0 },
	{ "ours",       NO_FAST_FORWARD | NO_TRIVIAL },
	{ "subtree",    NO_FAST_FORWARD | NO_TRIVIAL },
};

ಸ್ಥಿರ ವರ್ಣ *pull_twohead, *pull_octopus;

ಗಣನಾ ff_type {
	FF_NO,
	FF_ALLOW,
	FF_ONLY
};

ಸ್ಥಿರ ಗಣನಾ ff_type fast_forward = FF_ALLOW;

ಸ್ಥಿರ ವರ್ಣ *cleanup_arg;
ಸ್ಥಿರ ಗಣನಾ commit_ಸಂದೇಶ_cleanup_mode cleanup_mode;

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ option_parse_message(ಧ್ರುವ ಸಂರಚನಾ option *opt,
				ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಸಂರಚನಾ strbuf *buf = opt->ಮೌಲ್ಯ;

	ಯದಿ (unset)
		strbuf_setlen(buf, 0);
	ಅನ್ಯಥಾ ಯದಿ (arg) {
		strbuf_addf(buf, "%s%s", buf->len ? "\n\n" : "", arg);
		have_message = 1;
	} ಅನ್ಯಥಾ
		ಪ್ರತ್ಯಾಗಮ error(_("switch `m' requires a value"));
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಗಣನಾ parse_opt_result option_read_message(ಸಂರಚನಾ parse_opt_ctx_t *ctx,
						 ಧ್ರುವ ಸಂರಚನಾ option *opt,
						 ಧ್ರುವ ವರ್ಣ *arg_not_used,
						 ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಸಂರಚನಾ strbuf *buf = opt->ಮೌಲ್ಯ;
	ಧ್ರುವ ವರ್ಣ *arg;

	BUG_ON_OPT_ARG(arg_not_used);
	ಯದಿ (unset)
		BUG("-F cannot be negated");

	ಯದಿ (ctx->opt) {
		arg = ctx->opt;
		ctx->opt = ರಿಕ್ತ;
	} ಅನ್ಯಥಾ ಯದಿ (ctx->argc > 1) {
		ctx->argc--;
		arg = *++ctx->argv;
	} ಅನ್ಯಥಾ
		ಪ್ರತ್ಯಾಗಮ error(_("option `%s' requires a value"), opt->long_name);

	ಯದಿ (buf->len)
		strbuf_addch(buf, '\n');
	ಯದಿ (ctx->prefix && !is_absolute_path(arg))
		arg = prefix_filename(ctx->prefix, arg);
	ಯದಿ (strbuf_read_file(buf, arg, 0) < 0)
		ಪ್ರತ್ಯಾಗಮ error(_("could not read file '%s'"), arg);
	have_message = 1;

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಸಂರಚನಾ strategy *get_strategy(ಧ್ರುವ ವರ್ಣ *name)
{
	ಪೂರ್ಣಾಙ್ಕ i;
	ಸಂರಚನಾ strategy *ret;
	ಸ್ಥಿರ ಸಂರಚನಾ cmdnames main_cmds = {0}, other_cmds = {0};
	ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ loaded;
	ವರ್ಣ *default_strategy = ರಿಕ್ತ;

	ಯದಿ (!name)
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

	ಯದಿ (default_strategy &&
	    !strcmp(default_strategy, "ort") &&
	    !strcmp(name, "recursive")) {
		name = "ort";
	}

	ಪ್ರತಿ (i = 0; i < ARRAY_SIZE(all_strategy); i++)
		ಯದಿ (!strcmp(name, all_strategy[i].name))
			ಪ್ರತ್ಯಾಗಮ &all_strategy[i];

	ಯದಿ (!loaded) {
		ಸಂರಚನಾ cmdnames not_strategies = {0};
		loaded = 1;

		load_command_list("git-merge-", &main_cmds, &other_cmds);
		ಪ್ರತಿ (i = 0; i < main_cmds.cnt; i++) {
			ಪೂರ್ಣಾಙ್ಕ j, found = 0;
			ಸಂರಚನಾ cmdname *ent = main_cmds.names[i];
			ಪ್ರತಿ (j = 0; !found && j < ARRAY_SIZE(all_strategy); j++)
				ಯದಿ (!xstrncmpz(all_strategy[j].name, ent->name, ent->len))
					found = 1;
			ಯದಿ (!found)
				add_cmdname(&not_strategies, ent->name, ent->len);
		}
		exclude_cmds(&main_cmds, &not_strategies);

		cmdnames_release(&not_strategies);
	}
	ಯದಿ (!is_in_cmdlist(&main_cmds, name) && !is_in_cmdlist(&other_cmds, name)) {
		fprintf(stderr, _("Could not find merge strategy '%s'.\n"), name);
		fprintf(stderr, _("Available strategies are:"));
		ಪ್ರತಿ (i = 0; i < main_cmds.cnt; i++)
			fprintf(stderr, " %s", main_cmds.names[i]->name);
		fprintf(stderr, ".\n");
		ಯದಿ (other_cmds.cnt) {
			fprintf(stderr, _("Available custom strategies are:"));
			ಪ್ರತಿ (i = 0; i < other_cmds.cnt; i++)
				fprintf(stderr, " %s", other_cmds.names[i]->name);
			fprintf(stderr, ".\n");
		}
		exit(1);
	}

	CALLOC_ARRAY(ret, 1);
	ret->name = xstrdup(name);
	ret->attr = NO_TRIVIAL;

	cmdnames_release(&main_cmds);
	cmdnames_release(&other_cmds);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಶೂನ್ಯ append_strategy(ಸಂರಚನಾ strategy *s)
{
	ALLOC_GROW(use_strategies, use_strategies_nr + 1, use_strategies_alloc);
	use_strategies[use_strategies_nr++] = s;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ option_parse_strategy(ಧ್ರುವ ಸಂರಚನಾ option *opt UNUSED,
				 ಧ್ರುವ ವರ್ಣ *name, ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಯದಿ (unset)
		ಪ್ರತ್ಯಾಗಮ 0;

	append_strategy(get_strategy(name));
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ option_parse_compact_summary(ಧ್ರುವ ಸಂರಚನಾ option *opt,
					ಧ್ರುವ ವರ್ಣ *name UNUSED, ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಪೂರ್ಣಾಙ್ಕ *setting = opt->ಮೌಲ್ಯ;

	ಯದಿ (unset)
		*setting = 0;
	ಅನ್ಯಥಾ
		*setting = MERGE_SHOW_COMPACTSUMMARY;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಸಂರಚನಾ option builtin_merge_options[] = {
	OPT_SET_INT('n', ರಿಕ್ತ, &show_diffstat,
		N_("do not show a diffstat at the end of the merge"), 0),
	OPT_BOOL(0, "stat", &show_diffstat,
		N_("show a diffstat at the end of the merge")),
	OPT_BOOL(0, "summary", &show_diffstat, N_("(synonym to --stat)")),
	OPT_CALLBACK_F(0, "compact-summary", &show_diffstat, ರಿಕ್ತ,
		       N_("show a compact-summary at the end of the merge"),
		       PARSE_OPT_NOARG,
		       option_parse_compact_summary),
	{
		.type = OPTION_INTEGER,
		.long_name = "log",
		.ಮೌಲ್ಯ = &shortlog_len,
		.precision = ಪರಿಮಾಣ(shortlog_len),
		.argh = N_("n"),
		.help = N_("add (at most <n>) entries from shortlog to merge commit message"),
		.flags = PARSE_OPT_OPTARG,
		.defval = DEFAULT_MERGE_LOG_LEN,
	},
	OPT_BOOL(0, "squash", &squash,
		N_("create a single commit instead of doing a merge")),
	OPT_BOOL(0, "commit", &option_commit,
		N_("perform a commit if the merge succeeds (default)")),
	OPT_BOOL('e', "edit", &option_edit,
		N_("edit message before committing")),
	OPT_CLEANUP(&cleanup_arg),
	OPT_SET_INT(0, "ff", &fast_forward, N_("allow fast-forward (default)"), FF_ALLOW),
	OPT_SET_INT_F(0, "ff-only", &fast_forward,
		      N_("abort if fast-forward is not possible"),
		      FF_ONLY, PARSE_OPT_NONEG),
	OPT_RERERE_AUTOUPDATE(&allow_rerere_auto),
	OPT_BOOL(0, "verify-signatures", &verify_signatures,
		N_("verify that the named commit has a valid GPG signature")),
	OPT_CALLBACK('s', "strategy", ರಿಕ್ತ, N_("strategy"),
		N_("merge strategy to use"), option_parse_strategy),
	OPT_STRVEC('X', "strategy-option", &xopts, N_("option=value"),
		N_("option for selected merge strategy")),
	OPT_CALLBACK('m', "message", &merge_ಸಂದೇಶ, N_("message"),
		N_("merge commit message (for a non-fast-forward merge)"),
		option_parse_message),
	{
		.type = OPTION_LOWLEVEL_CALLBACK,
		.short_name = 'F',
		.long_name = "file",
		.ಮೌಲ್ಯ = &merge_ಸಂದೇಶ,
		.argh = N_("path"),
		.help = N_("read message from file"),
		.flags = PARSE_OPT_NONEG,
		.ll_callback = option_read_message,
	},
	OPT_STRING(0, "into-name", &into_name, N_("name"),
		   N_("use <name> instead of the real target")),
	OPT__VERBOSITY(&verbosity),
	OPT_BOOL(0, "abort", &abort_current_merge,
		N_("abort the current in-progress merge")),
	OPT_BOOL(0, "quit", &quit_current_merge,
		N_("--abort but leave index and working tree alone")),
	OPT_BOOL(0, "continue", &continue_current_merge,
		N_("continue the current in-progress merge")),
	OPT_BOOL(0, "allow-unrelated-histories", &allow_unrelated_histories,
		 N_("allow merging unrelated histories")),
	OPT_SET_INT(0, "progress", &show_progress, N_("force progress reporting"), 1),
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
	OPT_END()
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ save_state(ಸಂರಚನಾ object_id *stash)
{
	ಪೂರ್ಣಾಙ್ಕ len;
	ಸಂರಚನಾ child_process cp = CHILD_PROCESS_INIT;
	ಸಂರಚನಾ strbuf buffer = STRBUF_INIT;
	ಸಂರಚನಾ lock_file lock_file = LOCK_INIT;
	ಪೂರ್ಣಾಙ್ಕ fd;
	ಪೂರ್ಣಾಙ್ಕ rc = -1;

	fd = repo_hold_locked_index(the_repository, &lock_file, 0);
	refresh_index(the_repository->index, REFRESH_QUIET, ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ);
	ಯದಿ (0 <= fd)
		repo_update_index_if_able(the_repository, &lock_file);
	rollback_lock_file(&lock_file);

	strvec_pushl(&cp.args, "stash", "create", ರಿಕ್ತ);
	cp.out = -1;
	cp.git_cmd = 1;

	ಯದಿ (start_command(&cp))
		die(_("could not run stash."));
	len = strbuf_read(&buffer, cp.out, 1024);
	close(cp.out);

	ಯದಿ (finish_command(&cp) || len < 0)
		die(_("stash failed"));
	ಅನ್ಯಥಾ ಯದಿ (!len)		/* no changes */
		ಗಚ್ಛ out;
	strbuf_setlen(&buffer, buffer.len-1);
	ಯದಿ (repo_get_oid(the_repository, buffer.buf, stash))
		die(_("not a valid object: %s"), buffer.buf);
	rc = 0;
out:
	strbuf_release(&buffer);
	ಪ್ರತ್ಯಾಗಮ rc;
}

ಸ್ಥಿರ ಶೂನ್ಯ read_empty(ಧ್ರುವ ಸಂರಚನಾ object_id *oid)
{
	ಸಂರಚನಾ child_process cmd = CHILD_PROCESS_INIT;

	strvec_pushl(&cmd.args, "read-tree", "-m", "-u",
		     empty_ವೃಕ್ಷ_oid_hex(the_repository->hash_algo),
		     oid_to_hex(oid), ರಿಕ್ತ);
	cmd.git_cmd = 1;

	ಯದಿ (run_command(&cmd))
		die(_("read-tree failed"));
}

ಸ್ಥಿರ ಶೂನ್ಯ reset_hard(ಧ್ರುವ ಸಂರಚನಾ object_id *oid)
{
	ಸಂರಚನಾ child_process cmd = CHILD_PROCESS_INIT;

	strvec_pushl(&cmd.args, "read-tree", "-v", "--reset", "-u",
		     oid_to_hex(oid), ರಿಕ್ತ);
	cmd.git_cmd = 1;

	ಯದಿ (run_command(&cmd))
		die(_("read-tree failed"));
}

ಸ್ಥಿರ ಶೂನ್ಯ restore_state(ಧ್ರುವ ಸಂರಚನಾ object_id *head,
			  ಧ್ರುವ ಸಂರಚನಾ object_id *stash)
{
	ಸಂರಚನಾ child_process cmd = CHILD_PROCESS_INIT;

	reset_hard(head);

	ಯದಿ (is_null_oid(stash))
		ಗಚ್ಛ refresh_cache;

	strvec_pushl(&cmd.args, "stash", "apply", "--index", "--quiet", ರಿಕ್ತ);
	strvec_ತಳ್ಳು(&cmd.args, oid_to_hex(stash));

	/*
	 * It is OK to ignore error here, for example when there was
	 * nothing to restore.
	 */
	cmd.git_cmd = 1;
	run_command(&cmd);

refresh_cache:
	discard_index(the_repository->index);
	ಯದಿ (repo_read_index(the_repository) < 0)
		die(_("could not read index"));
}

/* This is called when no merge was necessary. */
ಸ್ಥಿರ ಶೂನ್ಯ finish_up_to_date(ಶೂನ್ಯ)
{
	ಯದಿ (verbosity >= 0) {
		ಯದಿ (squash)
			puts(_("Already up to date. (nothing to squash)"));
		ಅನ್ಯಥಾ
			puts(_("Already up to date."));
	}
	remove_merge_branch_state(the_repository);
}

ಸ್ಥಿರ ಶೂನ್ಯ squash_message(ಸಂರಚನಾ commit *commit, ಸಂರಚನಾ commit_list *remoteheads)
{
	ಸಂರಚನಾ rev_info rev;
	ಸಂರಚನಾ strbuf out = STRBUF_INIT;
	ಸಂರಚನಾ commit_list *j;
	ಸಂರಚನಾ pretty_print_context ctx = {0};

	printf(_("Squash commit -- not updating HEAD\n"));

	repo_init_revisions(the_repository, &rev, ರಿಕ್ತ);
	diff_merges_suppress(&rev);
	rev.commit_format = CMIT_FMT_MEDIUM;

	commit->object.flags |= UNINTERESTING;
	add_pending_object(&rev, &commit->object, ರಿಕ್ತ);

	ಪ್ರತಿ (j = remoteheads; j; j = j->next)
		add_pending_object(&rev, &j->item->object, ರಿಕ್ತ);

	setup_revisions(0, ರಿಕ್ತ, &rev, ರಿಕ್ತ);
	ಯದಿ (prepare_revision_walk(&rev))
		die(_("revision walk setup failed"));

	ctx.abbrev = rev.abbrev;
	ctx.date_mode = rev.date_mode;
	ctx.fmt = rev.commit_format;

	strbuf_addstr(&out, "Squashed commit of the following:\n");
	ಯಾವತ್ ((commit = get_revision(&rev)) != ರಿಕ್ತ) {
		strbuf_addch(&out, '\n');
		strbuf_addf(&out, "commit %s\n",
			oid_to_hex(&commit->object.oid));
		pretty_print_commit(&ctx, commit, &out);
	}
	write_file_buf(git_path_squash_ಸಂದೇಶ(the_repository), out.buf, out.len);
	strbuf_release(&out);
	release_revisions(&rev);
}

ಸ್ಥಿರ ಶೂನ್ಯ finish(ಸಂರಚನಾ commit *head_commit,
		   ಸಂರಚನಾ commit_list *remoteheads,
		   ಧ್ರುವ ಸಂರಚನಾ object_id *new_head, ಧ್ರುವ ವರ್ಣ *ಸಂದೇಶ)
{
	ಸಂರಚನಾ strbuf reflog_message = STRBUF_INIT;
	ಧ್ರುವ ಸಂರಚನಾ object_id *head = &head_commit->object.oid;

	ಯದಿ (!ಸಂದೇಶ)
		strbuf_addstr(&reflog_message, getenv("GIT_REFLOG_ACTION"));
	ಅನ್ಯಥಾ {
		ಯದಿ (verbosity >= 0)
			printf("%s\n", ಸಂದೇಶ);
		strbuf_addf(&reflog_message, "%s: %s",
			getenv("GIT_REFLOG_ACTION"), ಸಂದೇಶ);
	}
	ಯದಿ (squash) {
		squash_message(head_commit, remoteheads);
	} ಅನ್ಯಥಾ {
		ಯದಿ (verbosity >= 0 && !merge_ಸಂದೇಶ.len)
			printf(_("No merge message -- not updating HEAD\n"));
		ಅನ್ಯಥಾ {
			refs_update_ref(get_main_ref_store(the_repository),
					reflog_message.buf, "HEAD", new_head,
					head,
					0, UPDATE_REFS_DIE_ON_ERR);
			/*
			 * We ignore errors in 'gc --auto', since the
			 * user should see them.
			 */
			run_auto_maintenance(the_repository, verbosity < 0);
		}
	}
	ಯದಿ (new_head && show_diffstat) {
		ಸಂರಚನಾ diff_options opts;
		repo_diff_setup(the_repository, &opts);
		init_diffstat_widths(&opts);

		ವಿಕಲ್ಪ (show_diffstat) {
		ಪಕ್ಷ MERGE_SHOW_DIFFSTAT: /* 1 */
			opts.output_format |=
				DIFF_FORMAT_SUMMARY | DIFF_FORMAT_DIFFSTAT;
			ಭಙ್ಗ;
		ಪಕ್ಷ MERGE_SHOW_COMPACTSUMMARY: /* 2 */
			opts.output_format |= DIFF_FORMAT_DIFFSTAT;
			opts.flags.stat_with_summary = 1;
			ಭಙ್ಗ;
		ಸ್ವಭಾವ:
			ಭಙ್ಗ;
		}
		opts.detect_rename = DIFF_DETECT_RENAME;
		diff_setup_done(&opts);
		diff_ವೃಕ್ಷ_oid(head, new_head, "", &opts);
		diffcore_std(&opts);
		diff_flush(&opts);
	}

	/* Run a post-merge hook */
	run_hooks_l(the_repository, "post-merge", squash ? "1" : "0", ರಿಕ್ತ);

	ಯದಿ (new_head)
		apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
				    ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ);
	strbuf_release(&reflog_message);
}

/* Get the name for the merge commit's message. */
ಸ್ಥಿರ ಶೂನ್ಯ merge_name(ಧ್ರುವ ವರ್ಣ *remote, ಸಂರಚನಾ strbuf *ಸಂದೇಶ)
{
	ಸಂರಚನಾ commit *remote_head;
	ಸಂರಚನಾ object_id branch_head;
	ಸಂರಚನಾ strbuf bname = STRBUF_INIT;
	ಸಂರಚನಾ merge_remote_desc *desc;
	ಧ್ರುವ ವರ್ಣ *ptr;
	ವರ್ಣ *found_ref = ರಿಕ್ತ;
	ಪೂರ್ಣಾಙ್ಕ len, early;

	copy_branchname(the_repository, &bname, remote, 0);
	remote = bname.buf;

	oidclr(&branch_head, the_repository->hash_algo);
	remote_head = get_merge_parent(remote);
	ಯದಿ (!remote_head)
		die(_("'%s' does not point to a commit"), remote);

	ಯದಿ (repo_dwim_ref(the_repository, remote, strlen(remote), &branch_head,
			  &found_ref, 0) > 0) {
		ಯದಿ (starts_with(found_ref, "refs/heads/")) {
			strbuf_addf(ಸಂದೇಶ, "%s\t\tbranch '%s' of .\n",
				    oid_to_hex(&branch_head), remote);
			ಗಚ್ಛ cleanup;
		}
		ಯದಿ (starts_with(found_ref, "refs/tags/")) {
			strbuf_addf(ಸಂದೇಶ, "%s\t\ttag '%s' of .\n",
				    oid_to_hex(&branch_head), remote);
			ಗಚ್ಛ cleanup;
		}
		ಯದಿ (starts_with(found_ref, "refs/remotes/")) {
			strbuf_addf(ಸಂದೇಶ, "%s\t\tremote-tracking branch '%s' of .\n",
				    oid_to_hex(&branch_head), remote);
			ಗಚ್ಛ cleanup;
		}
	}

	/* See if remote matches <name>^^^.. or <name>~<number> */
	ಪ್ರತಿ (len = 0, ptr = remote + strlen(remote);
	     remote < ptr && ptr[-1] == '^';
	     ptr--)
		len++;
	ಯದಿ (len)
		early = 1;
	ಅನ್ಯಥಾ {
		early = 0;
		ptr = strrchr(remote, '~');
		ಯದಿ (ptr) {
			ಪೂರ್ಣಾಙ್ಕ seen_nonzero = 0;

			len++; /* count ~ */
			ಯಾವತ್ (*++ptr && isdigit(*ptr)) {
				seen_nonzero |= (*ptr != '0');
				len++;
			}
			ಯದಿ (*ptr)
				len = 0; /* not ...~<number> */
			ಅನ್ಯಥಾ ಯದಿ (seen_nonzero)
				early = 1;
			ಅನ್ಯಥಾ ಯದಿ (len == 1)
				early = 1; /* "name~" is "name~1"! */
		}
	}
	ಯದಿ (len) {
		ಸಂರಚನಾ strbuf truname = STRBUF_INIT;
		strbuf_addf(&truname, "refs/heads/%s", remote);
		strbuf_setlen(&truname, truname.len - len);
		ಯದಿ (refs_ref_exists(get_main_ref_store(the_repository), truname.buf)) {
			strbuf_addf(ಸಂದೇಶ,
				    "%s\t\tbranch '%s'%s of .\n",
				    oid_to_hex(&remote_head->object.oid),
				    truname.buf + 11,
				    (early ? " (early part)" : ""));
			strbuf_release(&truname);
			ಗಚ್ಛ cleanup;
		}
		strbuf_release(&truname);
	}

	desc = merge_remote_util(remote_head);
	ಯದಿ (desc && desc->obj && desc->obj->type == OBJ_TAG) {
		strbuf_addf(ಸಂದೇಶ, "%s\t\t%s '%s'\n",
			    oid_to_hex(&desc->obj->oid),
			    type_name(desc->obj->type),
			    remote);
		ಗಚ್ಛ cleanup;
	}

	strbuf_addf(ಸಂದೇಶ, "%s\t\tcommit '%s'\n",
		oid_to_hex(&remote_head->object.oid), remote);
cleanup:
	ಮುಕ್ತ(found_ref);
	strbuf_release(&bname);
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_branch_merge_options(ವರ್ಣ *bmo)
{
	ಧ್ರುವ ವರ್ಣ **argv;
	ಪೂರ್ಣಾಙ್ಕ argc;

	ಯದಿ (!bmo)
		ಪ್ರತ್ಯಾಗಮ;
	argc = split_cmdline(bmo, &argv);
	ಯದಿ (argc < 0)
		die(_("Bad branch.%s.mergeoptions string: %s"), branch,
		    _(split_cmdline_strerror(argc)));
	REALLOC_ARRAY(argv, argc + 2);
	MOVE_ARRAY(argv + 1, argv, argc + 1);
	argc++;
	argv[0] = "branch.*.mergeoptions";
	parse_options(argc, argv, ರಿಕ್ತ, builtin_merge_options,
		      builtin_merge_usage, 0);
	ಮುಕ್ತ(argv);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ git_merge_config(ಧ್ರುವ ವರ್ಣ *k, ಧ್ರುವ ವರ್ಣ *v,
			    ಧ್ರುವ ಸಂರಚನಾ config_context *ctx, ಶೂನ್ಯ *cb)
{
	ಪೂರ್ಣಾಙ್ಕ status;
	ಧ್ರುವ ವರ್ಣ *str;

	ಯದಿ (branch &&
	    skip_prefix(k, "branch.", &str) &&
	    skip_prefix(str, branch, &str) &&
	    !strcmp(str, ".mergeoptions")) {
		ಮುಕ್ತ(branch_mergeoptions);
		branch_mergeoptions = xstrdup(v);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (!strcmp(k, "merge.diffstat") || !strcmp(k, "merge.stat")) {
		ಪೂರ್ಣಾಙ್ಕ val = git_parse_maybe_bool_text(v);
		ವಿಕಲ್ಪ (val) {
		ಪಕ್ಷ 0:
			show_diffstat = 0;
			ಭಙ್ಗ;
		ಪಕ್ಷ 1:
			show_diffstat = MERGE_SHOW_DIFFSTAT;
			ಭಙ್ಗ;
		ಸ್ವಭಾವ:
			ಯದಿ (!strcmp(v, "compact"))
				show_diffstat = MERGE_SHOW_COMPACTSUMMARY;
			/*
			 * We do not need to have an explicit
			 *
			 * else if (!strcmp(v, "diffstat"))
			 *	show_diffstat = MERGE_SHOW_DIFFSTAT;
			 *
			 * here, because the catch-all uses the
			 * diffstat style anyway.
			 */
			ಅನ್ಯಥಾ
				/*
				 * A setting from a future?  It is not an
				 * error grave enough to fail the command.
				 * proceed using the default one.
				 */
				show_diffstat = MERGE_SHOW_DIFFSTAT;
			ಭಙ್ಗ;
		}
	} ಅನ್ಯಥಾ ಯದಿ (!strcmp(k, "merge.verifysignatures")) {
		verify_signatures = git_config_bool(k, v);
	} ಅನ್ಯಥಾ ಯದಿ (!strcmp(k, "pull.twohead")) {
		FREE_AND_NULL(pull_twohead);
		ಪ್ರತ್ಯಾಗಮ git_config_string(&pull_twohead, k, v);
	} ಅನ್ಯಥಾ ಯದಿ (!strcmp(k, "pull.octopus")) {
		FREE_AND_NULL(pull_octopus);
		ಪ್ರತ್ಯಾಗಮ git_config_string(&pull_octopus, k, v);
	} ಅನ್ಯಥಾ ಯದಿ (!strcmp(k, "commit.cleanup")) {
		ಪ್ರತ್ಯಾಗಮ git_config_string(&cleanup_arg, k, v);
	} ಅನ್ಯಥಾ ಯದಿ (!strcmp(k, "merge.ff")) {
		ಪೂರ್ಣಾಙ್ಕ boolval = git_parse_maybe_bool(v);
		ಯದಿ (0 <= boolval) {
			fast_forward = boolval ? FF_ALLOW : FF_NO;
		} ಅನ್ಯಥಾ ಯದಿ (v && !strcmp(v, "only")) {
			fast_forward = FF_ONLY;
		} /* do not barf on values from future versions of git */
		ಪ್ರತ್ಯಾಗಮ 0;
	} ಅನ್ಯಥಾ ಯದಿ (!strcmp(k, "merge.defaulttoupstream")) {
		default_to_upstream = git_config_bool(k, v);
		ಪ್ರತ್ಯಾಗಮ 0;
	} ಅನ್ಯಥಾ ಯದಿ (!strcmp(k, "commit.gpgsign")) {
		sign_commit = git_config_bool(k, v) ? "" : ರಿಕ್ತ;
		ಪ್ರತ್ಯಾಗಮ 0;
	} ಅನ್ಯಥಾ ಯದಿ (!strcmp(k, "gpg.mintrustlevel")) {
		check_trust_level = 0;
	} ಅನ್ಯಥಾ ಯದಿ (!strcmp(k, "merge.autostash")) {
		autostash = git_config_bool(k, v);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	status = fmt_merge_ಸಂದೇಶ_config(k, v, ctx, cb);
	ಯದಿ (status)
		ಪ್ರತ್ಯಾಗಮ status;
	ಪ್ರತ್ಯಾಗಮ git_diff_ui_config(k, v, ctx, cb);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ read_ವೃಕ್ಷ_trivial(ಸಂರಚನಾ object_id *common, ಸಂರಚನಾ object_id *head,
			     ಸಂರಚನಾ object_id *one)
{
	ಪೂರ್ಣಾಙ್ಕ i, nr_trees = 0;
	ಸಂರಚನಾ ವೃಕ್ಷ *trees[MAX_UNPACK_TREES];
	ಸಂರಚನಾ ವೃಕ್ಷ_desc t[MAX_UNPACK_TREES];
	ಸಂರಚನಾ unpack_trees_options opts;

	memset(&opts, 0, ಪರಿಮಾಣ(opts));
	opts.head_idx = 2;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;
	opts.update = 1;
	opts.verbose_update = 1;
	opts.trivial_merges_only = 1;
	opts.merge = 1;
	opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
	trees[nr_trees] = repo_parse_ವೃಕ್ಷ_indirect(the_repository, common);
	ಯದಿ (!trees[nr_trees++])
		ಪ್ರತ್ಯಾಗಮ -1;
	trees[nr_trees] = repo_parse_ವೃಕ್ಷ_indirect(the_repository, head);
	ಯದಿ (!trees[nr_trees++])
		ಪ್ರತ್ಯಾಗಮ -1;
	trees[nr_trees] = repo_parse_ವೃಕ್ಷ_indirect(the_repository, one);
	ಯದಿ (!trees[nr_trees++])
		ಪ್ರತ್ಯಾಗಮ -1;
	opts.fn = threeway_merge;
	cache_ವೃಕ್ಷ_free(&the_repository->index->cache_ವೃಕ್ಷ);
	ಪ್ರತಿ (i = 0; i < nr_trees; i++) {
		repo_parse_ವೃಕ್ಷ(the_repository, trees[i]);
		init_ವೃಕ್ಷ_desc(t+i, &trees[i]->object.oid,
			       trees[i]->buffer, trees[i]->size);
	}
	ಯದಿ (unpack_trees(nr_trees, t, &opts))
		ಪ್ರತ್ಯಾಗಮ -1;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ write_ವೃಕ್ಷ_trivial(ಸಂರಚನಾ object_id *oid)
{
	ಯದಿ (write_index_as_ವೃಕ್ಷ(oid, the_repository->index,
				repo_get_index_file(the_repository),
				0, ರಿಕ್ತ))
		die(_("git write-tree failed to write a tree"));
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ try_merge_strategy(ಧ್ರುವ ವರ್ಣ *strategy, ಸಂರಚನಾ commit_list *common,
			      ಸಂರಚನಾ commit_list *remoteheads,
			      ಸಂರಚನಾ commit *head)
{
	ಧ್ರುವ ವರ್ಣ *head_arg = "HEAD";

	ಯದಿ (repo_refresh_and_write_index(the_repository, REFRESH_QUIET,
					 SKIP_IF_UNCHANGED, 0, ರಿಕ್ತ, ರಿಕ್ತ,
					 ರಿಕ್ತ) < 0)
		die(_("Unable to write index."));

	ಯದಿ (!strcmp(strategy, "recursive") || !strcmp(strategy, "subtree") ||
	    !strcmp(strategy, "ort")) {
		ಸಂರಚನಾ lock_file lock = LOCK_INIT;
		ಪೂರ್ಣಾಙ್ಕ clean, x;
		ಸಂರಚನಾ commit *result;
		ಸಂರಚನಾ commit_list *reversed = ರಿಕ್ತ;
		ಸಂರಚನಾ merge_options o;
		ಸಂರಚನಾ commit_list *j;

		ಯದಿ (remoteheads->next) {
			error(_("Not handling anything other than two heads merge."));
			ಪ್ರತ್ಯಾಗಮ 2;
		}

		init_ui_merge_options(&o, the_repository);
		ಯದಿ (!strcmp(strategy, "subtree"))
			o.subtree_shift = "";

		o.show_rename_progress =
			show_progress == -1 ? isatty(2) : show_progress;

		ಪ್ರತಿ (x = 0; x < xopts.nr; x++)
			ಯದಿ (parse_merge_opt(&o, xopts.v[x]))
				die(_("unknown strategy option: -X%s"), xopts.v[x]);

		o.branch1 = head_arg;
		o.branch2 = merge_remote_util(remoteheads->item)->name;

		ಪ್ರತಿ (j = common; j; j = j->next)
			commit_list_ಸೇರಿಸು(j->item, &reversed);

		repo_hold_locked_index(the_repository, &lock,
				       LOCK_DIE_ON_ERROR);
		clean = merge_ort_recursive(&o, head, remoteheads->item,
					    reversed, &result);
		commit_list_free(reversed);
		strbuf_release(&o.obuf);

		ಯದಿ (clean < 0) {
			rollback_lock_file(&lock);
			ಪ್ರತ್ಯಾಗಮ 2;
		}
		ಯದಿ (write_locked_index(the_repository->index, &lock,
				       COMMIT_LOCK | SKIP_IF_UNCHANGED))
			die(_("unable to write %s"), repo_get_index_file(the_repository));
		ಪ್ರತ್ಯಾಗಮ clean ? 0 : 1;
	} ಅನ್ಯಥಾ {
		ಪ್ರತ್ಯಾಗಮ try_merge_command(the_repository,
					 strategy, xopts.nr, xopts.v,
					 common, head_arg, remoteheads);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ ಎಣಿಕೆ_diff_files(ಸಂರಚನಾ diff_queue_struct *q,
			     ಸಂರಚನಾ diff_options *opt UNUSED, ಶೂನ್ಯ *data)
{
	ಪೂರ್ಣಾಙ್ಕ *ಎಣಿಕೆ = data;

	(*ಎಣಿಕೆ) += q->nr;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ_unmerged_entries(ಶೂನ್ಯ)
{
	ಪೂರ್ಣಾಙ್ಕ i, ret = 0;

	ಪ್ರತಿ (i = 0; i < the_repository->index->cache_nr; i++)
		ಯದಿ (ce_stage(the_repository->index->cache[i]))
			ret++;

	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಶೂನ್ಯ add_strategies(ಧ್ರುವ ವರ್ಣ *string, unsigned attr)
{
	ಪೂರ್ಣಾಙ್ಕ i;

	ಯದಿ (string) {
		ಸಂರಚನಾ string_list list = STRING_LIST_INIT_DUP;
		ಸಂರಚನಾ string_list_item *item;
		string_list_split(&list, string, " ", -1);
		for_each_string_list_item(item, &list)
			append_strategy(get_strategy(item->string));
		string_list_clear(&list, 0);
		ಪ್ರತ್ಯಾಗಮ;
	}
	ಪ್ರತಿ (i = 0; i < ARRAY_SIZE(all_strategy); i++)
		ಯದಿ (all_strategy[i].attr & attr)
			append_strategy(&all_strategy[i]);

}

ಸ್ಥಿರ ಶೂನ್ಯ read_merge_ಸಂದೇಶ(ಸಂರಚನಾ strbuf *ಸಂದೇಶ)
{
	ಧ್ರುವ ವರ್ಣ *filename = git_path_merge_ಸಂದೇಶ(the_repository);
	strbuf_reset(ಸಂದೇಶ);
	ಯದಿ (strbuf_read_file(ಸಂದೇಶ, filename, 0) < 0)
		die_errno(_("Could not read from '%s'"), filename);
}

ಸ್ಥಿರ ಶೂನ್ಯ write_merge_state(ಸಂರಚನಾ commit_list *);
ಸ್ಥಿರ ಶೂನ್ಯ abort_commit(ಸಂರಚನಾ commit_list *remoteheads, ಧ್ರುವ ವರ್ಣ *err_ಸಂದೇಶ)
{
	ಯದಿ (err_ಸಂದೇಶ)
		error("%s", err_ಸಂದೇಶ);
	fprintf(stderr,
		_("Not committing merge; use 'git commit' to complete the merge.\n"));
	write_merge_state(remoteheads);
	exit(1);
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ merge_editor_comment[] =
N_("Please enter a commit message to explain why this merge is necessary,\n"
   "especially if it merges an updated upstream into a topic branch.\n"
   "\n");

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ scissors_editor_comment[] =
N_("An empty message aborts the commit.\n");

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ no_scissors_editor_comment[] =
N_("Lines starting with '%s' will be ignored, and an empty message aborts\n"
   "the commit.\n");

ಸ್ಥಿರ ಶೂನ್ಯ write_merge_heads(ಸಂರಚನಾ commit_list *);
ಸ್ಥಿರ ಶೂನ್ಯ prepare_to_commit(ಸಂರಚನಾ commit_list *remoteheads)
{
	ಸಂರಚನಾ strbuf ಸಂದೇಶ = STRBUF_INIT;
	ಧ್ರುವ ವರ್ಣ *index_file = repo_get_index_file(the_repository);

	ಯದಿ (!no_verify) {
		ಪೂರ್ಣಾಙ್ಕ invoked_hook;

		ಯದಿ (run_commit_hook(0 < option_edit, index_file, &invoked_hook,
				    "pre-merge-commit", ರಿಕ್ತ))
			abort_commit(remoteheads, ರಿಕ್ತ);
		/*
		 * Re-read the index as pre-merge-commit hook could have updated it,
		 * and write it out as a tree.  We must do this before we invoke
		 * the editor and after we invoke run_status above.
		 */
		ಯದಿ (invoked_hook)
			discard_index(the_repository->index);
	}
	read_index_from(the_repository->index, index_file,
			repo_get_git_dir(the_repository));
	strbuf_addbuf(&ಸಂದೇಶ, &merge_ಸಂದೇಶ);
	ಯದಿ (squash)
		BUG("the control must not reach here under --squash");
	ಯದಿ (0 < option_edit) {
		strbuf_addch(&ಸಂದೇಶ, '\n');
		ಯದಿ (cleanup_mode == COMMIT_ಸಂದೇಶ_CLEANUP_SCISSORS) {
			wt_status_append_cut_ಸಾಲು(&ಸಂದೇಶ);
			strbuf_commented_addf(&ಸಂದೇಶ, comment_ಸಾಲು_str, "\n");
		}
		strbuf_commented_addf(&ಸಂದೇಶ, comment_ಸಾಲು_str,
				      _(merge_editor_comment));
		ಯದಿ (cleanup_mode == COMMIT_ಸಂದೇಶ_CLEANUP_SCISSORS)
			strbuf_commented_addf(&ಸಂದೇಶ, comment_ಸಾಲು_str,
					      _(scissors_editor_comment));
		ಅನ್ಯಥಾ
			strbuf_commented_addf(&ಸಂದೇಶ, comment_ಸಾಲು_str,
				_(no_scissors_editor_comment), comment_ಸಾಲು_str);
	}
	ಯದಿ (signoff)
		append_signoff(&ಸಂದೇಶ, ignored_log_message_bytes(ಸಂದೇಶ.buf, ಸಂದೇಶ.len), 0);
	write_merge_heads(remoteheads);
	write_file_buf(git_path_merge_ಸಂದೇಶ(the_repository), ಸಂದೇಶ.buf, ಸಂದೇಶ.len);
	ಯದಿ (run_commit_hook(0 < option_edit, repo_get_index_file(the_repository),
			    ರಿಕ್ತ, "prepare-commit-msg",
			    git_path_merge_ಸಂದೇಶ(the_repository), "merge", ರಿಕ್ತ))
		abort_commit(remoteheads, ರಿಕ್ತ);
	ಯದಿ (0 < option_edit) {
		ಯದಿ (launch_editor(git_path_merge_ಸಂದೇಶ(the_repository), ರಿಕ್ತ, ರಿಕ್ತ))
			abort_commit(remoteheads, ರಿಕ್ತ);
	}

	ಯದಿ (!no_verify && run_commit_hook(0 < option_edit, repo_get_index_file(the_repository),
					  ರಿಕ್ತ, "commit-msg",
					  git_path_merge_ಸಂದೇಶ(the_repository), ರಿಕ್ತ))
		abort_commit(remoteheads, ರಿಕ್ತ);

	read_merge_ಸಂದೇಶ(&ಸಂದೇಶ);
	cleanup_message(&ಸಂದೇಶ, cleanup_mode, 0);
	ಯದಿ (!ಸಂದೇಶ.len)
		abort_commit(remoteheads, _("Empty commit message."));
	strbuf_release(&merge_ಸಂದೇಶ);
	strbuf_addbuf(&merge_ಸಂದೇಶ, &ಸಂದೇಶ);
	strbuf_release(&ಸಂದೇಶ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ merge_trivial(ಸಂರಚನಾ commit *head, ಸಂರಚನಾ commit_list *remoteheads)
{
	ಸಂರಚನಾ object_id result_ವೃಕ್ಷ, result_commit;
	ಸಂರಚನಾ commit_list *parents = ರಿಕ್ತ, **pptr = &parents;

	ಯದಿ (repo_refresh_and_write_index(the_repository, REFRESH_QUIET,
					 SKIP_IF_UNCHANGED, 0, ರಿಕ್ತ, ರಿಕ್ತ,
					 ರಿಕ್ತ) < 0)
		ಪ್ರತ್ಯಾಗಮ error(_("Unable to write index."));

	write_ವೃಕ್ಷ_trivial(&result_ವೃಕ್ಷ);
	printf(_("Wonderful.\n"));
	pptr = commit_list_append(head, pptr);
	pptr = commit_list_append(remoteheads->item, pptr);
	prepare_to_commit(remoteheads);
	ಯದಿ (commit_ವೃಕ್ಷ(merge_ಸಂದೇಶ.buf, merge_ಸಂದೇಶ.len, &result_ವೃಕ್ಷ, parents,
			&result_commit, ರಿಕ್ತ, sign_commit))
		die(_("failed to write commit object"));
	finish(head, remoteheads, &result_commit, "In-index merge");

	remove_merge_branch_state(the_repository);
	commit_list_free(parents);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ finish_automerge(ಸಂರಚನಾ commit *head,
			    ಪೂರ್ಣಾಙ್ಕ head_subsumed,
			    ಸಂರಚನಾ commit_list *common,
			    ಸಂರಚನಾ commit_list *remoteheads,
			    ಸಂರಚನಾ object_id *result_ವೃಕ್ಷ,
			    ಧ್ರುವ ವರ್ಣ *wt_strategy)
{
	ಸಂರಚನಾ commit_list *parents = ರಿಕ್ತ;
	ಸಂರಚನಾ strbuf buf = STRBUF_INIT;
	ಸಂರಚನಾ object_id result_commit;

	write_ವೃಕ್ಷ_trivial(result_ವೃಕ್ಷ);
	commit_list_free(common);
	parents = remoteheads;
	ಯದಿ (!head_subsumed || fast_forward == FF_NO)
		commit_list_ಸೇರಿಸು(head, &parents);
	prepare_to_commit(remoteheads);
	ಯದಿ (commit_ವೃಕ್ಷ(merge_ಸಂದೇಶ.buf, merge_ಸಂದೇಶ.len, result_ವೃಕ್ಷ, parents,
			&result_commit, ರಿಕ್ತ, sign_commit))
		die(_("failed to write commit object"));
	strbuf_addf(&buf, "Merge made by the '%s' strategy.", wt_strategy);
	finish(head, remoteheads, &result_commit, buf.buf);

	strbuf_release(&buf);
	remove_merge_branch_state(the_repository);
	commit_list_free(parents);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ suggest_conflicts(ಶೂನ್ಯ)
{
	ಧ್ರುವ ವರ್ಣ *filename;
	FILE *fp;
	ಸಂರಚನಾ strbuf msgbuf = STRBUF_INIT;

	filename = git_path_merge_ಸಂದೇಶ(the_repository);
	fp = xfopen(filename, "a");

	/*
	 * We can't use cleanup_mode because if we're not using the editor,
	 * get_cleanup_mode will return COMMIT_MSG_CLEANUP_SPACE instead, even
	 * though the message is meant to be processed later by git-commit.
	 * Thus, we will get the cleanup mode which is returned when we _are_
	 * using an editor.
	 */
	append_conflicts_hint(the_repository->index, &msgbuf,
			      get_cleanup_mode(cleanup_arg, 1));
	fputs(msgbuf.buf, fp);
	strbuf_release(&msgbuf);
	fclose(fp);
	repo_rerere(the_repository, allow_rerere_auto);
	printf(_("Automatic merge failed; "
			"fix conflicts and then commit the result.\n"));
	ಪ್ರತ್ಯಾಗಮ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ evaluate_result(ಶೂನ್ಯ)
{
	ಪೂರ್ಣಾಙ್ಕ cnt = 0;
	ಸಂರಚನಾ rev_info rev;

	/* Check how many files differ. */
	repo_init_revisions(the_repository, &rev, "");
	setup_revisions(0, ರಿಕ್ತ, &rev, ರಿಕ್ತ);
	rev.diffopt.output_format |=
		DIFF_FORMAT_CALLBACK;
	rev.diffopt.format_callback = ಎಣಿಕೆ_diff_files;
	rev.diffopt.format_callback_data = &cnt;
	run_diff_files(&rev, 0);

	/*
	 * Check how many unmerged entries are
	 * there.
	 */
	cnt += ಎಣಿಕೆ_unmerged_entries();

	release_revisions(&rev);
	ಪ್ರತ್ಯಾಗಮ cnt;
}

/*
 * Pretend as if the user told us to merge with the remote-tracking
 * branch we have for the upstream of the current branch
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ setup_with_upstream(ಧ್ರುವ ವರ್ಣ ***argv)
{
	ಸಂರಚನಾ branch *branch = branch_get(ರಿಕ್ತ);
	ಪೂರ್ಣಾಙ್ಕ i;
	ಧ್ರುವ ವರ್ಣ **args;

	ಯದಿ (!branch)
		die(_("No current branch."));
	ಯದಿ (!branch->remote_name)
		die(_("No remote for the current branch."));
	ಯದಿ (!branch->merge_nr)
		die(_("No default upstream defined for the current branch."));

	args = xcalloc(st_add(branch->merge_nr, 1), ಪರಿಮಾಣ(ವರ್ಣ *));
	ಪ್ರತಿ (i = 0; i < branch->merge_nr; i++) {
		ಯದಿ (!branch->merge[i]->dst)
			die(_("No remote-tracking branch for %s from %s"),
			    branch->merge[i]->src, branch->remote_name);
		args[i] = branch->merge[i]->dst;
	}
	args[i] = ರಿಕ್ತ;
	*argv = args;
	ಪ್ರತ್ಯಾಗಮ i;
}

ಸ್ಥಿರ ಶೂನ್ಯ write_merge_heads(ಸಂರಚನಾ commit_list *remoteheads)
{
	ಸಂರಚನಾ commit_list *j;
	ಸಂರಚನಾ strbuf buf = STRBUF_INIT;

	ಪ್ರತಿ (j = remoteheads; j; j = j->next) {
		ಸಂರಚನಾ object_id *oid;
		ಸಂರಚನಾ commit *c = j->item;
		ಸಂರಚನಾ merge_remote_desc *desc;

		desc = merge_remote_util(c);
		ಯದಿ (desc && desc->obj) {
			oid = &desc->obj->oid;
		} ಅನ್ಯಥಾ {
			oid = &c->object.oid;
		}
		strbuf_addf(&buf, "%s\n", oid_to_hex(oid));
	}
	write_file_buf(git_path_merge_head(the_repository), buf.buf, buf.len);

	strbuf_reset(&buf);
	ಯದಿ (fast_forward == FF_NO)
		strbuf_addstr(&buf, "no-ff");
	write_file_buf(git_path_merge_mode(the_repository), buf.buf, buf.len);
	strbuf_release(&buf);
}

ಸ್ಥಿರ ಶೂನ್ಯ write_merge_state(ಸಂರಚನಾ commit_list *remoteheads)
{
	write_merge_heads(remoteheads);
	strbuf_addch(&merge_ಸಂದೇಶ, '\n');
	write_file_buf(git_path_merge_ಸಂದೇಶ(the_repository), merge_ಸಂದೇಶ.buf,
		       merge_ಸಂದೇಶ.len);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ default_edit_option(ಶೂನ್ಯ)
{
	ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ name[] = "GIT_MERGE_AUTOEDIT";
	ಧ್ರುವ ವರ್ಣ *e = getenv(name);
	ಸಂರಚನಾ stat st_stdin, st_stdout;

	ಯದಿ (have_message)
		/* an explicit -m msg without --[no-]edit */
		ಪ್ರತ್ಯಾಗಮ 0;

	ಯದಿ (e) {
		ಪೂರ್ಣಾಙ್ಕ v = git_parse_maybe_bool(e);
		ಯದಿ (v < 0)
			die(_("Bad value '%s' in environment '%s'"), e, name);
		ಪ್ರತ್ಯಾಗಮ v;
	}

	/* Use editor if stdin and stdout are the same and is a tty */
	ಪ್ರತ್ಯಾಗಮ (!fstat(0, &st_stdin) &&
		!fstat(1, &st_stdout) &&
		isatty(0) && isatty(1) &&
		st_stdin.st_dev == st_stdout.st_dev &&
		st_stdin.st_ino == st_stdout.st_ino &&
		st_stdin.st_mode == st_stdout.st_mode);
}

ಸ್ಥಿರ ಸಂರಚನಾ commit_list *reduce_parents(ಸಂರಚನಾ commit *head_commit,
					  ಪೂರ್ಣಾಙ್ಕ *head_subsumed,
					  ಸಂರಚನಾ commit_list *remoteheads)
{
	ಸಂರಚನಾ commit_list *parents, **remotes;

	/*
	 * Is the current HEAD reachable from another commit being
	 * merged?  If so we do not want to record it as a parent of
	 * the resulting merge, unless --no-ff is given.  We will flip
	 * this variable to 0 when we find HEAD among the independent
	 * tips being merged.
	 */
	*head_subsumed = 1;

	/* Find what parents to record by checking independent ones. */
	parents = reduce_heads(remoteheads);
	commit_list_free(remoteheads);

	remoteheads = ರಿಕ್ತ;
	remotes = &remoteheads;
	ಯಾವತ್ (parents) {
		ಸಂರಚನಾ commit *commit = ತೆಗೆ_commit(&parents);
		ಯದಿ (commit == head_commit)
			*head_subsumed = 0;
		ಅನ್ಯಥಾ
			remotes = &commit_list_ಸೇರಿಸು(commit, remotes)->next;
	}
	ಪ್ರತ್ಯಾಗಮ remoteheads;
}

ಸ್ಥಿರ ಶೂನ್ಯ prepare_merge_message(ಸಂರಚನಾ strbuf *merge_names, ಸಂರಚನಾ strbuf *merge_ಸಂದೇಶ)
{
	ಸಂರಚನಾ fmt_merge_ಸಂದೇಶ_opts opts;

	memset(&opts, 0, ಪರಿಮಾಣ(opts));
	opts.add_title = !have_message;
	opts.shortlog_len = shortlog_len;
	opts.credit_people = (0 < option_edit);
	opts.into_name = into_name;

	fmt_merge_ಸಂದೇಶ(merge_names, merge_ಸಂದೇಶ, &opts);
	ಯದಿ (merge_ಸಂದೇಶ->len)
		strbuf_setlen(merge_ಸಂದೇಶ, merge_ಸಂದೇಶ->len - 1);
}

ಸ್ಥಿರ ಶೂನ್ಯ handle_fetch_head(ಸಂರಚನಾ commit_list **remotes, ಸಂರಚನಾ strbuf *merge_names)
{
	ಧ್ರುವ ವರ್ಣ *filename;
	ಪೂರ್ಣಾಙ್ಕ fd, pos, npos;
	ಸಂರಚನಾ strbuf fetch_head_file = STRBUF_INIT;
	ಧ್ರುವ unsigned hexsz = the_hash_algo->hexsz;

	ಯದಿ (!merge_names)
		merge_names = &fetch_head_file;

	filename = git_path_fetch_head(the_repository);
	fd = xopen(filename, O_RDONLY);

	ಯದಿ (strbuf_read(merge_names, fd, 0) < 0)
		die_errno(_("could not read '%s'"), filename);
	ಯದಿ (close(fd) < 0)
		die_errno(_("could not close '%s'"), filename);

	ಪ್ರತಿ (pos = 0; pos < merge_names->len; pos = npos) {
		ಸಂರಚನಾ object_id oid;
		ವರ್ಣ *ptr;
		ಸಂರಚನಾ commit *commit;

		ptr = strchr(merge_names->buf + pos, '\n');
		ಯದಿ (ptr)
			npos = ptr - merge_names->buf + 1;
		ಅನ್ಯಥಾ
			npos = merge_names->len;

		ಯದಿ (npos - pos < hexsz + 2 ||
		    get_oid_hex(merge_names->buf + pos, &oid))
			commit = ರಿಕ್ತ; /* bad */
		ಅನ್ಯಥಾ ಯದಿ (memcmp(merge_names->buf + pos + hexsz, "\t\t", 2))
			ಅನುವರ್ತ; /* not-for-merge */
		ಅನ್ಯಥಾ {
			ವರ್ಣ saved = merge_names->buf[pos + hexsz];
			merge_names->buf[pos + hexsz] = '\0';
			commit = get_merge_parent(merge_names->buf + pos);
			merge_names->buf[pos + hexsz] = saved;
		}
		ಯದಿ (!commit) {
			ಯದಿ (ptr)
				*ptr = '\0';
			die(_("not something we can merge in %s: %s"),
			    filename, merge_names->buf + pos);
		}
		remotes = &commit_list_ಸೇರಿಸು(commit, remotes)->next;
	}

	ಯದಿ (merge_names == &fetch_head_file)
		strbuf_release(&fetch_head_file);
}

ಸ್ಥಿರ ಸಂರಚನಾ commit_list *collect_parents(ಸಂರಚನಾ commit *head_commit,
					   ಪೂರ್ಣಾಙ್ಕ *head_subsumed,
					   ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv,
					   ಸಂರಚನಾ strbuf *merge_ಸಂದೇಶ)
{
	ಪೂರ್ಣಾಙ್ಕ i;
	ಸಂರಚನಾ commit_list *remoteheads = ರಿಕ್ತ;
	ಸಂರಚನಾ commit_list **remotes = &remoteheads;
	ಸಂರಚನಾ strbuf merge_names = STRBUF_INIT, *autogen = ರಿಕ್ತ;

	ಯದಿ (merge_ಸಂದೇಶ && (!have_message || shortlog_len))
		autogen = &merge_names;

	ಯದಿ (head_commit)
		remotes = &commit_list_ಸೇರಿಸು(head_commit, remotes)->next;

	ಯದಿ (argc == 1 && !strcmp(argv[0], "FETCH_HEAD")) {
		handle_fetch_head(remotes, autogen);
		remoteheads = reduce_parents(head_commit, head_subsumed, remoteheads);
	} ಅನ್ಯಥಾ {
		ಪ್ರತಿ (i = 0; i < argc; i++) {
			ಸಂರಚನಾ commit *commit = get_merge_parent(argv[i]);
			ಯದಿ (!commit)
				help_unknown_ref(argv[i], "merge",
						 _("not something we can merge"));
			remotes = &commit_list_ಸೇರಿಸು(commit, remotes)->next;
		}
		remoteheads = reduce_parents(head_commit, head_subsumed, remoteheads);
		ಯದಿ (autogen) {
			ಸಂರಚನಾ commit_list *p;
			ಪ್ರತಿ (p = remoteheads; p; p = p->next)
				merge_name(merge_remote_util(p->item)->name, autogen);
		}
	}

	ಯದಿ (autogen) {
		prepare_merge_message(autogen, merge_ಸಂದೇಶ);
		strbuf_release(autogen);
	}

	ಪ್ರತ್ಯಾಗಮ remoteheads;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ merging_a_throwaway_tag(ಸಂರಚನಾ commit *commit)
{
	ವರ್ಣ *tag_ref;
	ಸಂರಚನಾ object_id oid;
	ಪೂರ್ಣಾಙ್ಕ is_throwaway_tag = 0;

	/* Are we merging a tag? */
	ಯದಿ (!merge_remote_util(commit) ||
	    !merge_remote_util(commit)->obj ||
	    merge_remote_util(commit)->obj->type != OBJ_TAG)
		ಪ್ರತ್ಯಾಗಮ is_throwaway_tag;

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
	tag_ref = xstrfmt("refs/tags/%s",
			  ((ಸಂರಚನಾ tag *)merge_remote_util(commit)->obj)->tag);
	ಯದಿ (!refs_read_ref(get_main_ref_store(the_repository), tag_ref, &oid) &&
	    oideq(&oid, &merge_remote_util(commit)->obj->oid))
		is_throwaway_tag = 0;
	ಅನ್ಯಥಾ
		is_throwaway_tag = 1;
	ಮುಕ್ತ(tag_ref);
	ಪ್ರತ್ಯಾಗಮ is_throwaway_tag;
}

ಪೂರ್ಣಾಙ್ಕ cmd_merge(ಪೂರ್ಣಾಙ್ಕ argc,
	      ಧ್ರುವ ವರ್ಣ **argv,
	      ಧ್ರುವ ವರ್ಣ *prefix,
	      ಸಂರಚನಾ repository *repo UNUSED)
{
	ಸಂರಚನಾ object_id result_ವೃಕ್ಷ, stash, head_oid;
	ಸಂರಚನಾ commit *head_commit;
	ಸಂರಚನಾ strbuf buf = STRBUF_INIT;
	ಪೂರ್ಣಾಙ್ಕ i, ret = 0, head_subsumed;
	ಪೂರ್ಣಾಙ್ಕ best_cnt = -1, merge_was_ok = 0, automerge_was_ok = 0;
	ಸಂರಚನಾ commit_list *common = ರಿಕ್ತ;
	ಧ್ರುವ ವರ್ಣ *best_strategy = ರಿಕ್ತ, *wt_strategy = ರಿಕ್ತ;
	ಸಂರಚನಾ commit_list *remoteheads = ರಿಕ್ತ, *p;
	ಶೂನ್ಯ *branch_to_free, *argv_to_free = ರಿಕ್ತ;
	ಪೂರ್ಣಾಙ್ಕ orig_argc = argc;
	ಪೂರ್ಣಾಙ್ಕ merge_log_config = -1;

	show_usage_with_options_if_asked(argc, argv,
					 builtin_merge_usage, builtin_merge_options);

#ifndef WITH_BREAKING_CHANGES
	warn_on_auto_comment_char = true;
#endif /* !WITH_BREAKING_CHANGES */
	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	/*
	 * Check if we are _not_ on a detached HEAD, i.e. if there is a
	 * current branch.
	 */
	branch = branch_to_free = refs_resolve_refdup(get_main_ref_store(the_repository),
						      "HEAD", 0, &head_oid,
						      ರಿಕ್ತ);
	ಯದಿ (branch)
		skip_prefix(branch, "refs/heads/", &branch);

	init_diff_ui_defaults();
	repo_config(the_repository, git_merge_config, &merge_log_config);

	ಯದಿ (!branch || is_null_oid(&head_oid))
		head_commit = ರಿಕ್ತ;
	ಅನ್ಯಥಾ
		head_commit = lookup_commit_or_die(&head_oid, "HEAD");

	ಯದಿ (branch_mergeoptions)
		parse_branch_merge_options(branch_mergeoptions);
	argc = parse_options(argc, argv, prefix, builtin_merge_options,
			builtin_merge_usage, 0);
	ಯದಿ (shortlog_len < 0)
		shortlog_len = (merge_log_config > 0) ? merge_log_config : 0;

	ಯದಿ (verbosity < 0 && show_progress == -1)
		show_progress = 0;

	ಯದಿ (abort_current_merge) {
		ಪೂರ್ಣಾಙ್ಕ nargc = 2;
		ಧ್ರುವ ವರ್ಣ *nargv[] = {"reset", "--merge", ರಿಕ್ತ};
		ವರ್ಣ stash_oid_hex[GIT_MAX_HEXSZ + 1];
		ಸಂರಚನಾ object_id stash_oid = {0};

		ಯದಿ (orig_argc != 2)
			usage_ಸಂದೇಶ_opt(_("--abort expects no arguments"),
			      builtin_merge_usage, builtin_merge_options);

		ಯದಿ (!file_exists(git_path_merge_head(the_repository)))
			die(_("There is no merge to abort (MERGE_HEAD missing)."));

		ಯದಿ (!refs_read_ref(get_main_ref_store(the_repository), "MERGE_AUTOSTASH", &stash_oid))
			refs_delete_ref(get_main_ref_store(the_repository),
					"", "MERGE_AUTOSTASH", &stash_oid,
					REF_NO_DEREF);

		/* Invoke 'git reset --merge' */
		ret = cmd_reset(nargc, nargv, prefix, the_repository);

		ಯದಿ (!is_null_oid(&stash_oid)) {
			oid_to_hex_r(stash_oid_hex, &stash_oid);
			apply_autostash_oid(stash_oid_hex);
		}

		ಗಚ್ಛ done;
	}

	ಯದಿ (quit_current_merge) {
		ಯದಿ (orig_argc != 2)
			usage_ಸಂದೇಶ_opt(_("--quit expects no arguments"),
				      builtin_merge_usage,
				      builtin_merge_options);

		remove_merge_branch_state(the_repository);
		ಗಚ್ಛ done;
	}

	ಯದಿ (continue_current_merge) {
		ಪೂರ್ಣಾಙ್ಕ nargc = 1;
		ಧ್ರುವ ವರ್ಣ *nargv[] = {"commit", ರಿಕ್ತ};

		ಯದಿ (orig_argc != 2)
			usage_ಸಂದೇಶ_opt(_("--continue expects no arguments"),
			      builtin_merge_usage, builtin_merge_options);

		ಯದಿ (!file_exists(git_path_merge_head(the_repository)))
			die(_("There is no merge in progress (MERGE_HEAD missing)."));

		/* Invoke 'git commit' */
		ret = cmd_commit(nargc, nargv, prefix, the_repository);
		ಗಚ್ಛ done;
	}

	ಯದಿ (repo_read_index_unmerged(the_repository))
		die_resolve_conflict("merge");

	ಯದಿ (file_exists(git_path_merge_head(the_repository))) {
		/*
		 * There is no unmerged entry, don't advise 'git
		 * add/rm <file>', just 'git commit'.
		 */
		ಯದಿ (advice_enabled(ADVICE_RESOLVE_CONFLICT))
			die(_("You have not concluded your merge (MERGE_HEAD exists).\n"
				  "Please, commit your changes before you merge."));
		ಅನ್ಯಥಾ
			die(_("You have not concluded your merge (MERGE_HEAD exists)."));
	}
	ಯದಿ (refs_ref_exists(get_main_ref_store(the_repository), "CHERRY_PICK_HEAD")) {
		ಯದಿ (advice_enabled(ADVICE_RESOLVE_CONFLICT))
			die(_("You have not concluded your cherry-pick (CHERRY_PICK_HEAD exists).\n"
			    "Please, commit your changes before you merge."));
		ಅನ್ಯಥಾ
			die(_("You have not concluded your cherry-pick (CHERRY_PICK_HEAD exists)."));
	}
	resolve_undo_clear_index(the_repository->index);

	ಯದಿ (option_edit < 0)
		option_edit = default_edit_option();

	cleanup_mode = get_cleanup_mode(cleanup_arg, 0 < option_edit);

	ಯದಿ (verbosity < 0)
		show_diffstat = 0;

	ಯದಿ (squash) {
		ಯದಿ (fast_forward == FF_NO)
			die(_("options '%s' and '%s' cannot be used together"), "--squash", "--no-ff.");
		ಯದಿ (option_commit > 0)
			die(_("options '%s' and '%s' cannot be used together"), "--squash", "--commit.");
		/*
		 * squash can now silently disable option_commit - this is not
		 * a problem as it is only overriding the default, not a user
		 * supplied option.
		 */
		option_commit = 0;
	}

	ಯದಿ (option_commit < 0)
		option_commit = 1;

	ಯದಿ (!argc) {
		ಯದಿ (default_to_upstream) {
			argc = setup_with_upstream(&argv);
			argv_to_free = argv;
		}
		ಅನ್ಯಥಾ
			die(_("No commit specified and merge.defaultToUpstream not set."));
	} ಅನ್ಯಥಾ ಯದಿ (argc == 1 && !strcmp(argv[0], "-")) {
		argv[0] = "@{-1}";
	}

	ಯದಿ (!argc)
		usage_with_options(builtin_merge_usage,
			builtin_merge_options);

	ಯದಿ (!head_commit) {
		/*
		 * If the merged head is a valid one there is no reason
		 * to forbid "git merge" into a branch yet to be born.
		 * We do the same for "git pull".
		 */
		ಸಂರಚನಾ object_id *remote_head_oid;
		ಯದಿ (squash)
			die(_("Squash commit into empty head not supported yet"));
		ಯದಿ (fast_forward == FF_NO)
			die(_("Non-fast-forward commit does not make sense into "
			    "an empty head"));
		remoteheads = collect_parents(head_commit, &head_subsumed,
					      argc, argv, ರಿಕ್ತ);
		ಯದಿ (!remoteheads)
			die(_("%s - not something we can merge"), argv[0]);
		ಯದಿ (remoteheads->next)
			die(_("Can merge only exactly one commit into empty head"));

		ಯದಿ (verify_signatures)
			verify_merge_signature(remoteheads->item, verbosity,
					       check_trust_level);

		remote_head_oid = &remoteheads->item->object.oid;
		read_empty(remote_head_oid);
		refs_update_ref(get_main_ref_store(the_repository),
				"initial pull", "HEAD", remote_head_oid, ರಿಕ್ತ,
				0,
				UPDATE_REFS_DIE_ON_ERR);
		ಗಚ್ಛ done;
	}

	/*
	 * All the rest are the commits being merged; prepare
	 * the standard merge summary message to be appended
	 * to the given message.
	 */
	remoteheads = collect_parents(head_commit, &head_subsumed,
				      argc, argv, &merge_ಸಂದೇಶ);

	ಯದಿ (!head_commit || !argc)
		usage_with_options(builtin_merge_usage,
			builtin_merge_options);

	ಯದಿ (verify_signatures) {
		ಪ್ರತಿ (p = remoteheads; p; p = p->next) {
			verify_merge_signature(p->item, verbosity,
					       check_trust_level);
		}
	}

	strbuf_addstr(&buf, "merge");
	ಪ್ರತಿ (p = remoteheads; p; p = p->next)
		strbuf_addf(&buf, " %s", merge_remote_util(p->item)->name);
	setenv("GIT_REFLOG_ACTION", buf.buf, 0);
	strbuf_reset(&buf);

	ಪ್ರತಿ (p = remoteheads; p; p = p->next) {
		ಸಂರಚನಾ commit *commit = p->item;
		strbuf_addf(&buf, "GITHEAD_%s",
			    oid_to_hex(&commit->object.oid));
		setenv(buf.buf, merge_remote_util(commit)->name, 1);
		strbuf_reset(&buf);
		ಯದಿ (fast_forward != FF_ONLY && merging_a_throwaway_tag(commit))
			fast_forward = FF_NO;
	}

	ಯದಿ (!use_strategies) {
		ಯದಿ (!remoteheads)
			; /* already up-to-date */
		ಅನ್ಯಥಾ ಯದಿ (!remoteheads->next)
			add_strategies(pull_twohead, DEFAULT_TWOHEAD);
		ಅನ್ಯಥಾ
			add_strategies(pull_octopus, DEFAULT_OCTOPUS);
	}

	ಪ್ರತಿ (i = 0; i < use_strategies_nr; i++) {
		ಯದಿ (use_strategies[i]->attr & NO_FAST_FORWARD)
			fast_forward = FF_NO;
		ಯದಿ (use_strategies[i]->attr & NO_TRIVIAL)
			allow_trivial = 0;
	}

	ಯದಿ (!remoteheads)
		; /* already up-to-date */
	ಅನ್ಯಥಾ ಯದಿ (!remoteheads->next) {
		ಯದಿ (repo_get_merge_bases(the_repository, head_commit,
					 remoteheads->item, &common) < 0) {
			ret = 2;
			ಗಚ್ಛ done;
		}
	} ಅನ್ಯಥಾ {
		ಸಂರಚನಾ commit_list *list = remoteheads;
		commit_list_ಸೇರಿಸು(head_commit, &list);
		ಯದಿ (get_octopus_merge_bases(list, &common) < 0) {
			ಮುಕ್ತ(list);
			ret = 2;
			ಗಚ್ಛ done;
		}
		ಮುಕ್ತ(list);
	}

	refs_update_ref(get_main_ref_store(the_repository),
			"updating ORIG_HEAD", "ORIG_HEAD",
			&head_commit->object.oid, ರಿಕ್ತ, 0,
			UPDATE_REFS_DIE_ON_ERR);

	ಯದಿ (remoteheads && !common) {
		/* No common ancestors found. */
		ಯದಿ (!allow_unrelated_histories)
			die(_("refusing to merge unrelated histories"));
		/* otherwise, we need a real merge. */
	} ಅನ್ಯಥಾ ಯದಿ (!remoteheads ||
		 (!remoteheads->next && !common->next &&
		  common->item == remoteheads->item)) {
		/*
		 * If head can reach all the merge then we are up to date.
		 * but first the most common case of merging one remote.
		 */
		finish_up_to_date();
		ಗಚ್ಛ done;
	} ಅನ್ಯಥಾ ಯದಿ (fast_forward != FF_NO && !remoteheads->next &&
			!common->next &&
			oideq(&common->item->object.oid, &head_commit->object.oid)) {
		/* Again the most common case of merging one remote. */
		ಧ್ರುವ ವರ್ಣ *ಸಂದೇಶ = have_message ?
			"Fast-forward (no commit created; -m option ignored)" :
			"Fast-forward";
		ಸಂರಚನಾ commit *commit;

		ಯದಿ (verbosity >= 0) {
			printf(_("Updating %s..%s\n"),
			       repo_find_unique_abbrev(the_repository, &head_commit->object.oid,
						       DEFAULT_ABBREV),
			       repo_find_unique_abbrev(the_repository, &remoteheads->item->object.oid,
						       DEFAULT_ABBREV));
		}
		commit = remoteheads->item;
		ಯದಿ (!commit) {
			ret = 1;
			ಗಚ್ಛ done;
		}

		ಯದಿ (autostash)
			create_autostash_ref(the_repository, "MERGE_AUTOSTASH",
					     ರಿಕ್ತ, false);
		ಯದಿ (checkout_fast_forward(the_repository,
					  &head_commit->object.oid,
					  &commit->object.oid,
					  overwrite_ignore)) {
			apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
					    ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ);
			ret = 1;
			ಗಚ್ಛ done;
		}

		finish(head_commit, remoteheads, &commit->object.oid, ಸಂದೇಶ);
		remove_merge_branch_state(the_repository);
		ಗಚ್ಛ done;
	} ಅನ್ಯಥಾ ಯದಿ (!remoteheads->next && common->next)
		;
		/*
		 * We are not doing octopus and not fast-forward.  Need
		 * a real merge.
		 */
	ಅನ್ಯಥಾ ಯದಿ (!remoteheads->next && !common->next && option_commit) {
		/*
		 * We are not doing octopus, not fast-forward, and have
		 * only one common.
		 */
		refresh_index(the_repository->index, REFRESH_QUIET, ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ);
		ಯದಿ (allow_trivial && fast_forward != FF_ONLY) {
			/*
			 * Must first ensure that index matches HEAD before
			 * attempting a trivial merge.
			 */
			ಸಂರಚನಾ ವೃಕ್ಷ *head_ವೃಕ್ಷ = repo_get_commit_ವೃಕ್ಷ(the_repository,
								      head_commit);
			ಸಂರಚನಾ strbuf sb = STRBUF_INIT;

			ಯದಿ (repo_index_has_changes(the_repository, head_ವೃಕ್ಷ,
						   &sb)) {
				error(_("Your local changes to the following files would be overwritten by merge:\n  %s"),
				      sb.buf);
				strbuf_release(&sb);
				ret = 2;
				ಗಚ್ಛ done;
			}

			/* See if it is really trivial. */
			git_committer_info(IDENT_STRICT);
			printf(_("Trying really trivial in-index merge...\n"));
			ಯದಿ (!read_ವೃಕ್ಷ_trivial(&common->item->object.oid,
					       &head_commit->object.oid,
					       &remoteheads->item->object.oid)) {
				ret = merge_trivial(head_commit, remoteheads);
				ಗಚ್ಛ done;
			}
			printf(_("Nope.\n"));
		}
	} ಅನ್ಯಥಾ {
		/*
		 * An octopus.  If we can reach all the remote we are up
		 * to date.
		 */
		ಪೂರ್ಣಾಙ್ಕ up_to_date = 1;
		ಸಂರಚನಾ commit_list *j;

		ಪ್ರತಿ (j = remoteheads; j; j = j->next) {
			ಪೂರ್ಣಾಙ್ಕ ret = repo_in_merge_bases(the_repository,
						      j->item, head_commit);
			ಯದಿ (ret < 0)
				exit(128);
			ಯದಿ (!ret) {
				up_to_date = 0;
				ಭಙ್ಗ;
			}
		}
		ಯದಿ (up_to_date) {
			finish_up_to_date();
			ಗಚ್ಛ done;
		}
	}

	ಯದಿ (fast_forward == FF_ONLY)
		die_ff_impossible();

	ಯದಿ (autostash)
		create_autostash_ref(the_repository, "MERGE_AUTOSTASH",
				     ರಿಕ್ತ, false);

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
	ಯದಿ (save_state(&stash))
		oidclr(&stash, the_repository->hash_algo);

	ಪ್ರತಿ (i = 0; i < use_strategies_nr; i++) {
		ಪೂರ್ಣಾಙ್ಕ ret, cnt;
		ಯದಿ (i) {
			printf(_("Rewinding the tree to pristine...\n"));
			restore_state(&head_commit->object.oid, &stash);
		}
		ಯದಿ (use_strategies_nr != 1)
			printf(_("Trying merge strategy %s...\n"),
				use_strategies[i]->name);
		/*
		 * Remember which strategy left the state in the working
		 * tree.
		 */
		wt_strategy = use_strategies[i]->name;

		ret = try_merge_strategy(wt_strategy,
					 common, remoteheads,
					 head_commit);
		/*
		 * The backend exits with 1 when conflicts are
		 * left to be resolved, with 2 when it does not
		 * handle the given merge at all.
		 */
		ಯದಿ (ret < 2) {
			ಯದಿ (!ret) {
				/*
				 * This strategy worked; no point in trying
				 * another.
				 */
				merge_was_ok = 1;
				best_strategy = wt_strategy;
				ಭಙ್ಗ;
			}
			cnt = (use_strategies_nr > 1) ? evaluate_result() : 0;
			ಯದಿ (best_cnt <= 0 || cnt <= best_cnt) {
				best_strategy = wt_strategy;
				best_cnt = cnt;
			}
		}
	}

	/*
	 * If we have a resulting tree, that means the strategy module
	 * auto resolved the merge cleanly.
	 */
	ಯದಿ (merge_was_ok && option_commit) {
		automerge_was_ok = 1;
		ret = finish_automerge(head_commit, head_subsumed,
				       common, remoteheads,
				       &result_ವೃಕ್ಷ, wt_strategy);
		ಗಚ್ಛ done;
	}

	/*
	 * Pick the result from the best strategy and have the user fix
	 * it up.
	 */
	ಯದಿ (!best_strategy) {
		restore_state(&head_commit->object.oid, &stash);
		ಯದಿ (use_strategies_nr > 1)
			fprintf(stderr,
				_("No merge strategy handled the merge.\n"));
		ಅನ್ಯಥಾ
			fprintf(stderr, _("Merge with strategy %s failed.\n"),
				use_strategies[0]->name);
		apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
				    ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ);
		ret = 2;
		ಗಚ್ಛ done;
	} ಅನ್ಯಥಾ ಯದಿ (best_strategy == wt_strategy)
		; /* We already have its result in the working tree. */
	ಅನ್ಯಥಾ {
		printf(_("Rewinding the tree to pristine...\n"));
		restore_state(&head_commit->object.oid, &stash);
		printf(_("Using the %s strategy to prepare resolving by hand.\n"),
			best_strategy);
		try_merge_strategy(best_strategy, common, remoteheads,
				   head_commit);
	}

	ಯದಿ (squash) {
		finish(head_commit, remoteheads, ರಿಕ್ತ, ರಿಕ್ತ);

		git_test_write_commit_graph_or_die(the_repository);
	} ಅನ್ಯಥಾ
		write_merge_state(remoteheads);

	ಯದಿ (merge_was_ok)
		fprintf(stderr, _("Automatic merge went well; "
			"stopped before committing as requested\n"));
	ಅನ್ಯಥಾ
		ret = suggest_conflicts();
	ಯದಿ (autostash)
		printf(_("When finished, apply stashed changes with `git stash pop`\n"));

done:
	ಯದಿ (!automerge_was_ok) {
		commit_list_free(common);
		commit_list_free(remoteheads);
	}
	strbuf_release(&buf);
	ಮುಕ್ತ(branch_to_free);
	ಮುಕ್ತ(argv_to_free);
	ಮುಕ್ತ(pull_twohead);
	ಮುಕ್ತ(pull_octopus);
	discard_index(the_repository->index);
	ಪ್ರತ್ಯಾಗಮ ret;
}
