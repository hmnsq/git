/*
 * Builtin "git merge"
 *
 * Copyright (c) 2008 Miklos Vajna <vmiklos@frugalware.org>
 *
 * Based on git-merge.sh by Junio C Hamano.
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DISABLE_SIGN_COMPARE_WARNINGS

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "abspath.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "advice.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "editor.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hex.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-name.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "lockfile.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "repository.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "run-command.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hook.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "diff.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "diff-merges.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "refs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "refspec.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "commit.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "diffcore.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "path.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "revision.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "unpack-trees.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cache-tree.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "dir.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "color.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "rerere.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "help.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "merge.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "merge-ort-wrappers.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "resolve-undo.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "remote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "fmt-merge-msg.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "sequencer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "string-list.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "tag.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "alias.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "branch.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "commit-reach.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "wt-status.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "commit-graph.h"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEFAULT_TWOHEAD (1<<0)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEFAULT_OCTOPUS (1<<1)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NO_FAST_FORWARD (1<<2)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NO_TRIVIAL      (1<<3)

ਬਣਤਰ strategy {
	ਅਚਲ ਅੱਖਰ *name;
	ਬਿਨਾਨਿਸ਼ਾਨੀ attr;
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ builtin_ਮਿਲਾਓ_usage[] = {
	N_("git merge [<options>] [<commit>...]"),
	"git merge --abort",
	"git merge --continue",
	ਨਿਰਾ
};

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਮਿਲਾਓ_SHOW_DIFFSTAT 1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਮਿਲਾਓ_SHOW_COMPACTSUMMARY 2

ਸਥਿਰ ਪੂਰਨ show_diffstat = ਮਿਲਾਓ_SHOW_DIFFSTAT, shortlog_len = -1, squash;
ਸਥਿਰ ਪੂਰਨ option_commit = -1;
ਸਥਿਰ ਪੂਰਨ option_edit = -1;
ਸਥਿਰ ਪੂਰਨ allow_trivial = 1, have_ਸੁਨੇਹਾ, verify_signatures;
ਸਥਿਰ ਪੂਰਨ check_trust_level = 1;
ਸਥਿਰ ਪੂਰਨ overwrite_ignore = 1;
ਸਥਿਰ ਬਣਤਰ strbuf ਮਿਲਾਓ_ਸੁਨੇਹਾ = STRBUF_INIT;
ਸਥਿਰ ਬਣਤਰ strategy **use_strategies;
ਸਥਿਰ ਮਾਪ_t use_strategies_nr, use_strategies_alloc;
ਸਥਿਰ ਬਣਤਰ strvec xopts = STRVEC_INIT;
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *branch;
ਸਥਿਰ ਅੱਖਰ *branch_mergeoptions;
ਸਥਿਰ ਪੂਰਨ verbosity;
ਸਥਿਰ ਪੂਰਨ allow_rerere_auto;
ਸਥਿਰ ਪੂਰਨ abort_current_ਮਿਲਾਓ;
ਸਥਿਰ ਪੂਰਨ quit_current_ਮਿਲਾਓ;
ਸਥਿਰ ਪੂਰਨ continue_current_ਮਿਲਾਓ;
ਸਥਿਰ ਪੂਰਨ allow_unrelated_histories;
ਸਥਿਰ ਪੂਰਨ show_ਤਰੱਕੀ = -1;
ਸਥਿਰ ਪੂਰਨ default_ਮੰਜ਼ਿਲ_upstream = 1;
ਸਥਿਰ ਪੂਰਨ signoff;
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *sign_commit;
ਸਥਿਰ ਪੂਰਨ autostash;
ਸਥਿਰ ਪੂਰਨ no_verify;
ਸਥਿਰ ਅੱਖਰ *into_name;

ਸਥਿਰ ਬਣਤਰ strategy all_strategy[] = {
	{ "recursive",  NO_TRIVIAL },
	{ "octopus",    DEFAULT_OCTOPUS },
	{ "ort",        DEFAULT_TWOHEAD | NO_TRIVIAL },
	{ "resolve",    0 },
	{ "ours",       NO_FAST_FORWARD | NO_TRIVIAL },
	{ "subtree",    NO_FAST_FORWARD | NO_TRIVIAL },
};

ਸਥਿਰ ਅੱਖਰ *pull_twohead, *pull_octopus;

ਗਿਣਤੀ ff_type {
	FF_NO,
	FF_ALLOW,
	FF_ONLY
};

ਸਥਿਰ ਗਿਣਤੀ ff_type fast_forward = FF_ALLOW;

ਸਥਿਰ ਅੱਖਰ *cleanup_arg;
ਸਥਿਰ ਗਿਣਤੀ commit_ਸੁਨੇਹਾ_cleanup_ਬਹੁਲਕ cleanup_ਬਹੁਲਕ;

ਸਥਿਰ ਪੂਰਨ option_parse_ਸੁਨੇਹਾ(ਅਚਲ ਬਣਤਰ option *opt,
				ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਬਣਤਰ strbuf *buf = opt->ਮੁੱਲ;

	ਜੇ (unset)
		strbuf_setlen(buf, 0);
	ਨਹੀਂਤਾਂ ਜੇ (arg) {
		strbuf_addf(buf, "%s%s", buf->len ? "\n\n" : "", arg);
		have_ਸੁਨੇਹਾ = 1;
	} ਨਹੀਂਤਾਂ
		ਵਾਪਸ error(_("switch `m' requires a value"));
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਗਿਣਤੀ parse_opt_ਨਤੀਜਾ option_read_ਸੁਨੇਹਾ(ਬਣਤਰ parse_opt_ctx_t *ctx,
						 ਅਚਲ ਬਣਤਰ option *opt,
						 ਅਚਲ ਅੱਖਰ *arg_not_ਵਰਤਿਆ,
						 ਪੂਰਨ unset)
{
	ਬਣਤਰ strbuf *buf = opt->ਮੁੱਲ;
	ਅਚਲ ਅੱਖਰ *arg;

	BUG_ON_OPT_ARG(arg_not_ਵਰਤਿਆ);
	ਜੇ (unset)
		BUG("-F cannot be negated");

	ਜੇ (ctx->opt) {
		arg = ctx->opt;
		ctx->opt = ਨਿਰਾ;
	} ਨਹੀਂਤਾਂ ਜੇ (ctx->argc > 1) {
		ctx->argc--;
		arg = *++ctx->argv;
	} ਨਹੀਂਤਾਂ
		ਵਾਪਸ error(_("option `%s' requires a value"), opt->long_name);

	ਜੇ (buf->len)
		strbuf_addch(buf, '\n');
	ਜੇ (ctx->ਅਗੇਤਰ && !is_absolute_ਰਸਤਾ(arg))
		arg = ਅਗੇਤਰ_filename(ctx->ਅਗੇਤਰ, arg);
	ਜੇ (strbuf_read_file(buf, arg, 0) < 0)
		ਵਾਪਸ error(_("could not read file '%s'"), arg);
	have_ਸੁਨੇਹਾ = 1;

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਬਣਤਰ strategy *get_strategy(ਅਚਲ ਅੱਖਰ *name)
{
	ਪੂਰਨ i;
	ਬਣਤਰ strategy *ret;
	ਸਥਿਰ ਬਣਤਰ cmdnames main_cmds = {0}, other_cmds = {0};
	ਸਥਿਰ ਪੂਰਨ loaded;
	ਅੱਖਰ *default_strategy = ਨਿਰਾ;

	ਜੇ (!name)
		ਵਾਪਸ ਨਿਰਾ;

	ਜੇ (default_strategy &&
	    !strcmp(default_strategy, "ort") &&
	    !strcmp(name, "recursive")) {
		name = "ort";
	}

	ਲਈ (i = 0; i < ARRAY_ਮਾਪ(all_strategy); i++)
		ਜੇ (!strcmp(name, all_strategy[i].name))
			ਵਾਪਸ &all_strategy[i];

	ਜੇ (!loaded) {
		ਬਣਤਰ cmdnames not_strategies = {0};
		loaded = 1;

		ਲੱਦੋ_command_list("git-merge-", &main_cmds, &other_cmds);
		ਲਈ (i = 0; i < main_cmds.cnt; i++) {
			ਪੂਰਨ j, ਮਿਲਿਆ_2 = 0;
			ਬਣਤਰ cmdname *ent = main_cmds.ਨਾਂ[i];
			ਲਈ (j = 0; !ਮਿਲਿਆ_2 && j < ARRAY_ਮਾਪ(all_strategy); j++)
				ਜੇ (!xstrncmpz(all_strategy[j].name, ent->name, ent->len))
					ਮਿਲਿਆ_2 = 1;
			ਜੇ (!ਮਿਲਿਆ_2)
				ਜੋੜੋ_cmdname(&not_strategies, ent->name, ent->len);
		}
		exclude_cmds(&main_cmds, &not_strategies);

		cmdnames_release(&not_strategies);
	}
	ਜੇ (!is_ਵਿੱਚ_cmdlist(&main_cmds, name) && !is_ਵਿੱਚ_cmdlist(&other_cmds, name)) {
		fprintf(stderr, _("Could not find merge strategy '%s'.\n"), name);
		fprintf(stderr, _("Available strategies are:"));
		ਲਈ (i = 0; i < main_cmds.cnt; i++)
			fprintf(stderr, " %s", main_cmds.ਨਾਂ[i]->name);
		fprintf(stderr, ".\n");
		ਜੇ (other_cmds.cnt) {
			fprintf(stderr, _("Available custom strategies are:"));
			ਲਈ (i = 0; i < other_cmds.cnt; i++)
				fprintf(stderr, " %s", other_cmds.ਨਾਂ[i]->name);
			fprintf(stderr, ".\n");
		}
		ਬਾਹਰ_ਨਿਕਲੋ(1);
	}

	CALLOC_ARRAY(ret, 1);
	ret->name = xstrdup(name);
	ret->attr = NO_TRIVIAL;

	cmdnames_release(&main_cmds);
	cmdnames_release(&other_cmds);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਖਾਲੀ append_strategy(ਬਣਤਰ strategy *s)
{
	ALLOC_GROW(use_strategies, use_strategies_nr + 1, use_strategies_alloc);
	use_strategies[use_strategies_nr++] = s;
}

ਸਥਿਰ ਪੂਰਨ option_parse_strategy(ਅਚਲ ਬਣਤਰ option *opt ਅਣਵਰਤਿਆ,
				 ਅਚਲ ਅੱਖਰ *name, ਪੂਰਨ unset)
{
	ਜੇ (unset)
		ਵਾਪਸ 0;

	append_strategy(get_strategy(name));
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ option_parse_compact_summary(ਅਚਲ ਬਣਤਰ option *opt,
					ਅਚਲ ਅੱਖਰ *name ਅਣਵਰਤਿਆ, ਪੂਰਨ unset)
{
	ਪੂਰਨ *setting = opt->ਮੁੱਲ;

	ਜੇ (unset)
		*setting = 0;
	ਨਹੀਂਤਾਂ
		*setting = ਮਿਲਾਓ_SHOW_COMPACTSUMMARY;
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਬਣਤਰ option builtin_ਮਿਲਾਓ_options[] = {
	OPT_ਸੈੱਟ_INT('n', ਨਿਰਾ, &show_diffstat,
		N_("do not show a diffstat at the end of the merge"), 0),
	OPT_BOOL(0, "stat", &show_diffstat,
		N_("show a diffstat at the end of the merge")),
	OPT_BOOL(0, "summary", &show_diffstat, N_("(synonym to --stat)")),
	OPT_CALLBACK_F(0, "compact-summary", &show_diffstat, ਨਿਰਾ,
		       N_("show a compact-summary at the end of the merge"),
		       PARSE_OPT_NOARG,
		       option_parse_compact_summary),
	{
		.type = OPTION_INTEGER,
		.long_name = "log",
		.ਮੁੱਲ = &shortlog_len,
		.precision = ਆਕਾਰ(shortlog_len),
		.argh = N_("n"),
		.help = N_("add (at most <n>) entries from shortlog to merge commit message"),
		.flags = PARSE_OPT_OPTARG,
		.defval = DEFAULT_ਮਿਲਾਓ_LOG_LEN,
	},
	OPT_BOOL(0, "squash", &squash,
		N_("create a single commit instead of doing a merge")),
	OPT_BOOL(0, "commit", &option_commit,
		N_("perform a commit if the merge succeeds (default)")),
	OPT_BOOL('e', "edit", &option_edit,
		N_("edit message before committing")),
	OPT_CLEANUP(&cleanup_arg),
	OPT_ਸੈੱਟ_INT(0, "ff", &fast_forward, N_("allow fast-forward (default)"), FF_ALLOW),
	OPT_ਸੈੱਟ_INT_F(0, "ff-only", &fast_forward,
		      N_("abort if fast-forward is not possible"),
		      FF_ONLY, PARSE_OPT_NONEG),
	OPT_RERERE_AUTOUPDATE(&allow_rerere_auto),
	OPT_BOOL(0, "verify-signatures", &verify_signatures,
		N_("verify that the named commit has a valid GPG signature")),
	OPT_CALLBACK('s', "strategy", ਨਿਰਾ, N_("strategy"),
		N_("merge strategy to use"), option_parse_strategy),
	OPT_STRVEC('X', "strategy-option", &xopts, N_("option=value"),
		N_("option for selected merge strategy")),
	OPT_CALLBACK('m', "message", &ਮਿਲਾਓ_ਸੁਨੇਹਾ, N_("message"),
		N_("merge commit message (for a non-fast-forward merge)"),
		option_parse_ਸੁਨੇਹਾ),
	{
		.type = OPTION_LOWLEVEL_CALLBACK,
		.short_name = 'F',
		.long_name = "file",
		.ਮੁੱਲ = &ਮਿਲਾਓ_ਸੁਨੇਹਾ,
		.argh = N_("path"),
		.help = N_("read message from file"),
		.flags = PARSE_OPT_NONEG,
		.ll_callback = option_read_ਸੁਨੇਹਾ,
	},
	OPT_STRING(0, "into-name", &into_name, N_("name"),
		   N_("use <name> instead of the real target")),
	OPT__VERBOSITY(&verbosity),
	OPT_BOOL(0, "abort", &abort_current_ਮਿਲਾਓ,
		N_("abort the current in-progress merge")),
	OPT_BOOL(0, "quit", &quit_current_ਮਿਲਾਓ,
		N_("--abort but leave index and working tree alone")),
	OPT_BOOL(0, "continue", &continue_current_ਮਿਲਾਓ,
		N_("continue the current in-progress merge")),
	OPT_BOOL(0, "allow-unrelated-histories", &allow_unrelated_histories,
		 N_("allow merging unrelated histories")),
	OPT_ਸੈੱਟ_INT(0, "progress", &show_ਤਰੱਕੀ, N_("force progress reporting"), 1),
	{
		.type = OPTION_STRING,
		.short_name = 'S',
		.long_name = "gpg-sign",
		.ਮੁੱਲ = &sign_commit,
		.argh = N_("key-id"),
		.help = N_("GPG sign commit"),
		.flags = PARSE_OPT_OPTARG,
		.defval = (intptr_t) "",
	},
	OPT_AUTOSTASH(&autostash),
	OPT_BOOL(0, "overwrite-ignore", &overwrite_ignore, N_("update ignored files (default)")),
	OPT_BOOL(0, "signoff", &signoff, N_("add a Signed-off-by trailer")),
	OPT_BOOL(0, "no-verify", &no_verify, N_("bypass pre-merge-commit and commit-msg hooks")),
	OPT_ਸਿਰਾ()
};

ਸਥਿਰ ਪੂਰਨ save_state(ਬਣਤਰ object_id *stash)
{
	ਪੂਰਨ len;
	ਬਣਤਰ ਬੱਚਾ_process cp = ਬੱਚਾ_PROCESS_INIT;
	ਬਣਤਰ strbuf ਬਫਰ = STRBUF_INIT;
	ਬਣਤਰ ਤਾਲਾ_file ਤਾਲਾ_file = ਤਾਲਾ_INIT;
	ਪੂਰਨ fd;
	ਪੂਰਨ rc = -1;

	fd = repo_hold_locked_ਸੂਚਕਾਂਕ(the_repository, &ਤਾਲਾ_file, 0);
	refresh_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ, REFRESH_QUIET, ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ);
	ਜੇ (0 <= fd)
		repo_update_ਸੂਚਕਾਂਕ_if_able(the_repository, &ਤਾਲਾ_file);
	rollback_ਤਾਲਾ_file(&ਤਾਲਾ_file);

	strvec_pushl(&cp.ਦਲੀਲਾਂ, "stash", "create", ਨਿਰਾ);
	cp.ਬਾਹਰਲਾ = -1;
	cp.git_cmd = 1;

	ਜੇ (ਸ਼ੁਰੂ_command(&cp))
		die(_("could not run stash."));
	len = strbuf_read(&ਬਫਰ, cp.ਬਾਹਰਲਾ, 1024);
	close(cp.ਬਾਹਰਲਾ);

	ਜੇ (ਅੰਤ_command(&cp) || len < 0)
		die(_("stash failed"));
	ਨਹੀਂਤਾਂ ਜੇ (!len)		/* no changes */
		ਜਾਓ ਬਾਹਰਲਾ;
	strbuf_setlen(&ਬਫਰ, ਬਫਰ.len-1);
	ਜੇ (repo_get_oid(the_repository, ਬਫਰ.buf, stash))
		die(_("not a valid object: %s"), ਬਫਰ.buf);
	rc = 0;
ਬਾਹਰਲਾ:
	strbuf_release(&ਬਫਰ);
	ਵਾਪਸ rc;
}

ਸਥਿਰ ਖਾਲੀ read_ਖਾਲੀ_ਥਾਵਾਂ(ਅਚਲ ਬਣਤਰ object_id *oid)
{
	ਬਣਤਰ ਬੱਚਾ_process cmd = ਬੱਚਾ_PROCESS_INIT;

	strvec_pushl(&cmd.ਦਲੀਲਾਂ, "read-tree", "-m", "-u",
		     ਖਾਲੀ_ਥਾਵਾਂ_ਰੁੱਖ_oid_hex(the_repository->hash_algo),
		     oid_ਮੰਜ਼ਿਲ_hex(oid), ਨਿਰਾ);
	cmd.git_cmd = 1;

	ਜੇ (ਲੜੀਵਾਰ_command(&cmd))
		die(_("read-tree failed"));
}

ਸਥਿਰ ਖਾਲੀ reset_hard(ਅਚਲ ਬਣਤਰ object_id *oid)
{
	ਬਣਤਰ ਬੱਚਾ_process cmd = ਬੱਚਾ_PROCESS_INIT;

	strvec_pushl(&cmd.ਦਲੀਲਾਂ, "read-tree", "-v", "--reset", "-u",
		     oid_ਮੰਜ਼ਿਲ_hex(oid), ਨਿਰਾ);
	cmd.git_cmd = 1;

	ਜੇ (ਲੜੀਵਾਰ_command(&cmd))
		die(_("read-tree failed"));
}

ਸਥਿਰ ਖਾਲੀ restore_state(ਅਚਲ ਬਣਤਰ object_id *ਸਿਰ,
			  ਅਚਲ ਬਣਤਰ object_id *stash)
{
	ਬਣਤਰ ਬੱਚਾ_process cmd = ਬੱਚਾ_PROCESS_INIT;

	reset_hard(ਸਿਰ);

	ਜੇ (is_null_oid(stash))
		ਜਾਓ refresh_cache;

	strvec_pushl(&cmd.ਦਲੀਲਾਂ, "stash", "apply", "--index", "--quiet", ਨਿਰਾ);
	strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, oid_ਮੰਜ਼ਿਲ_hex(stash));

	/*
	 * It is OK to ignore error here, for example when there was
	 * nothing to restore.
	 */
	cmd.git_cmd = 1;
	ਲੜੀਵਾਰ_command(&cmd);

refresh_cache:
	discard_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ);
	ਜੇ (repo_read_ਸੂਚਕਾਂਕ(the_repository) < 0)
		die(_("could not read index"));
}

/* This is called when no merge was necessary. */
ਸਥਿਰ ਖਾਲੀ ਅੰਤ_ਉੱਪਰ_ਮੰਜ਼ਿਲ_date(ਖਾਲੀ)
{
	ਜੇ (verbosity >= 0) {
		ਜੇ (squash)
			puts(_("Already up to date. (nothing to squash)"));
		ਨਹੀਂਤਾਂ
			puts(_("Already up to date."));
	}
	remove_ਮਿਲਾਓ_branch_state(the_repository);
}

ਸਥਿਰ ਖਾਲੀ squash_ਸੁਨੇਹਾ(ਬਣਤਰ commit *commit, ਬਣਤਰ commit_list *remoteheads)
{
	ਬਣਤਰ rev_info rev;
	ਬਣਤਰ strbuf ਬਾਹਰਲਾ = STRBUF_INIT;
	ਬਣਤਰ commit_list *j;
	ਬਣਤਰ pretty_print_context ctx = {0};

	ਲਿਖੋ(_("Squash commit -- not updating HEAD\n"));

	repo_init_revisions(the_repository, &rev, ਨਿਰਾ);
	diff_merges_suppress(&rev);
	rev.commit_format = CMIT_FMT_MEDIUM;

	commit->object.flags |= UNINTERESTING;
	ਜੋੜੋ_pending_object(&rev, &commit->object, ਨਿਰਾ);

	ਲਈ (j = remoteheads; j; j = j->ਅਗਲਾ)
		ਜੋੜੋ_pending_object(&rev, &j->ਵਸਤੂ->object, ਨਿਰਾ);

	ਤਿਆਰੀ_revisions(0, ਨਿਰਾ, &rev, ਨਿਰਾ);
	ਜੇ (prepare_revision_walk(&rev))
		die(_("revision walk setup failed"));

	ctx.abbrev = rev.abbrev;
	ctx.date_ਬਹੁਲਕ = rev.date_ਬਹੁਲਕ;
	ctx.fmt = rev.commit_format;

	strbuf_addstr(&ਬਾਹਰਲਾ, "Squashed commit of the following:\n");
	ਜਦੋਂ ((commit = get_revision(&rev)) != ਨਿਰਾ) {
		strbuf_addch(&ਬਾਹਰਲਾ, '\n');
		strbuf_addf(&ਬਾਹਰਲਾ, "commit %s\n",
			oid_ਮੰਜ਼ਿਲ_hex(&commit->object.oid));
		pretty_print_commit(&ctx, commit, &ਬਾਹਰਲਾ);
	}
	write_file_buf(git_ਰਸਤਾ_squash_ਸੁਨੇਹਾ(the_repository), ਬਾਹਰਲਾ.buf, ਬਾਹਰਲਾ.len);
	strbuf_release(&ਬਾਹਰਲਾ);
	release_revisions(&rev);
}

ਸਥਿਰ ਖਾਲੀ ਅੰਤ(ਬਣਤਰ commit *ਸਿਰ_commit,
		   ਬਣਤਰ commit_list *remoteheads,
		   ਅਚਲ ਬਣਤਰ object_id *new_ਸਿਰ, ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ)
{
	ਬਣਤਰ strbuf reflog_ਸੁਨੇਹਾ_2 = STRBUF_INIT;
	ਅਚਲ ਬਣਤਰ object_id *ਸਿਰ = &ਸਿਰ_commit->object.oid;

	ਜੇ (!ਸੁਨੇਹਾ)
		strbuf_addstr(&reflog_ਸੁਨੇਹਾ_2, getenv("GIT_REFLOG_ACTION"));
	ਨਹੀਂਤਾਂ {
		ਜੇ (verbosity >= 0)
			ਲਿਖੋ("%s\n", ਸੁਨੇਹਾ);
		strbuf_addf(&reflog_ਸੁਨੇਹਾ_2, "%s: %s",
			getenv("GIT_REFLOG_ACTION"), ਸੁਨੇਹਾ);
	}
	ਜੇ (squash) {
		squash_ਸੁਨੇਹਾ(ਸਿਰ_commit, remoteheads);
	} ਨਹੀਂਤਾਂ {
		ਜੇ (verbosity >= 0 && !ਮਿਲਾਓ_ਸੁਨੇਹਾ.len)
			ਲਿਖੋ(_("No merge message -- not updating HEAD\n"));
		ਨਹੀਂਤਾਂ {
			refs_update_ref(get_main_ref_store(the_repository),
					reflog_ਸੁਨੇਹਾ_2.buf, "HEAD", new_ਸਿਰ,
					ਸਿਰ,
					0, UPDATE_REFS_DIE_ON_ERR);
			/*
			 * We ignore errors in 'gc --auto', since the
			 * user should see them.
			 */
			ਲੜੀਵਾਰ_auto_maintenance(the_repository, verbosity < 0);
		}
	}
	ਜੇ (new_ਸਿਰ && show_diffstat) {
		ਬਣਤਰ diff_options opts;
		repo_diff_ਤਿਆਰੀ(the_repository, &opts);
		init_diffstat_widths(&opts);

		ਚੋਣ (show_diffstat) {
		ਹਾਲਤ ਮਿਲਾਓ_SHOW_DIFFSTAT: /* 1 */
			opts.output_format |=
				DIFF_FORMAT_SUMMARY | DIFF_FORMAT_DIFFSTAT;
			ਰੋਕੋ;
		ਹਾਲਤ ਮਿਲਾਓ_SHOW_COMPACTSUMMARY: /* 2 */
			opts.output_format |= DIFF_FORMAT_DIFFSTAT;
			opts.flags.stat_with_summary = 1;
			ਰੋਕੋ;
		ਮੂਲ:
			ਰੋਕੋ;
		}
		opts.detect_rename = DIFF_DETECT_RENAME;
		diff_ਤਿਆਰੀ_ਪੂਰਾ(&opts);
		diff_ਰੁੱਖ_oid(ਸਿਰ, new_ਸਿਰ, "", &opts);
		diffcore_std(&opts);
		diff_flush(&opts);
	}

	/* Run a post-merge hook */
	ਲੜੀਵਾਰ_hooks_l(the_repository, "post-merge", squash ? "1" : "0", ਨਿਰਾ);

	ਜੇ (new_ਸਿਰ)
		apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
				    ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ);
	strbuf_release(&reflog_ਸੁਨੇਹਾ_2);
}

/* Get the name for the merge commit's message. */
ਸਥਿਰ ਖਾਲੀ ਮਿਲਾਓ_name(ਅਚਲ ਅੱਖਰ *remote, ਬਣਤਰ strbuf *ਸੁਨੇਹਾ)
{
	ਬਣਤਰ commit *remote_ਸਿਰ;
	ਬਣਤਰ object_id branch_ਸਿਰ;
	ਬਣਤਰ strbuf bname = STRBUF_INIT;
	ਬਣਤਰ ਮਿਲਾਓ_remote_desc *desc;
	ਅਚਲ ਅੱਖਰ *ptr;
	ਅੱਖਰ *ਮਿਲਿਆ_ref = ਨਿਰਾ;
	ਪੂਰਨ len, early;

	copy_branchname(the_repository, &bname, remote, 0);
	remote = bname.buf;

	oidclr(&branch_ਸਿਰ, the_repository->hash_algo);
	remote_ਸਿਰ = get_ਮਿਲਾਓ_ਮਾਪਾ(remote);
	ਜੇ (!remote_ਸਿਰ)
		die(_("'%s' does not point to a commit"), remote);

	ਜੇ (repo_dwim_ref(the_repository, remote, ਲੜੀ_ਲੰਬਾਈ(remote), &branch_ਸਿਰ,
			  &ਮਿਲਿਆ_ref, 0) > 0) {
		ਜੇ (starts_with(ਮਿਲਿਆ_ref, "refs/heads/")) {
			strbuf_addf(ਸੁਨੇਹਾ, "%s\t\tbranch '%s' of .\n",
				    oid_ਮੰਜ਼ਿਲ_hex(&branch_ਸਿਰ), remote);
			ਜਾਓ cleanup;
		}
		ਜੇ (starts_with(ਮਿਲਿਆ_ref, "refs/tags/")) {
			strbuf_addf(ਸੁਨੇਹਾ, "%s\t\ttag '%s' of .\n",
				    oid_ਮੰਜ਼ਿਲ_hex(&branch_ਸਿਰ), remote);
			ਜਾਓ cleanup;
		}
		ਜੇ (starts_with(ਮਿਲਿਆ_ref, "refs/remotes/")) {
			strbuf_addf(ਸੁਨੇਹਾ, "%s\t\tremote-tracking branch '%s' of .\n",
				    oid_ਮੰਜ਼ਿਲ_hex(&branch_ਸਿਰ), remote);
			ਜਾਓ cleanup;
		}
	}

	/* See if remote matches <name>^^^.. or <name>~<number> */
	ਲਈ (len = 0, ptr = remote + ਲੜੀ_ਲੰਬਾਈ(remote);
	     remote < ptr && ptr[-1] == '^';
	     ptr--)
		len++;
	ਜੇ (len)
		early = 1;
	ਨਹੀਂਤਾਂ {
		early = 0;
		ptr = strrchr(remote, '~');
		ਜੇ (ptr) {
			ਪੂਰਨ ਵੇਖਿਆ_nonzero = 0;

			len++; /* count ~ */
			ਜਦੋਂ (*++ptr && ਅੰਕ_ਹੈ(*ptr)) {
				ਵੇਖਿਆ_nonzero |= (*ptr != '0');
				len++;
			}
			ਜੇ (*ptr)
				len = 0; /* not ...~<number> */
			ਨਹੀਂਤਾਂ ਜੇ (ਵੇਖਿਆ_nonzero)
				early = 1;
			ਨਹੀਂਤਾਂ ਜੇ (len == 1)
				early = 1; /* "name~" is "name~1"! */
		}
	}
	ਜੇ (len) {
		ਬਣਤਰ strbuf truname = STRBUF_INIT;
		strbuf_addf(&truname, "refs/heads/%s", remote);
		strbuf_setlen(&truname, truname.len - len);
		ਜੇ (refs_ref_exists(get_main_ref_store(the_repository), truname.buf)) {
			strbuf_addf(ਸੁਨੇਹਾ,
				    "%s\t\tbranch '%s'%s of .\n",
				    oid_ਮੰਜ਼ਿਲ_hex(&remote_ਸਿਰ->object.oid),
				    truname.buf + 11,
				    (early ? " (early part)" : ""));
			strbuf_release(&truname);
			ਜਾਓ cleanup;
		}
		strbuf_release(&truname);
	}

	desc = ਮਿਲਾਓ_remote_util(remote_ਸਿਰ);
	ਜੇ (desc && desc->obj && desc->obj->type == OBJ_ਟੈਗ) {
		strbuf_addf(ਸੁਨੇਹਾ, "%s\t\t%s '%s'\n",
			    oid_ਮੰਜ਼ਿਲ_hex(&desc->obj->oid),
			    type_name(desc->obj->type),
			    remote);
		ਜਾਓ cleanup;
	}

	strbuf_addf(ਸੁਨੇਹਾ, "%s\t\tcommit '%s'\n",
		oid_ਮੰਜ਼ਿਲ_hex(&remote_ਸਿਰ->object.oid), remote);
cleanup:
	ਮੁਕਤ(ਮਿਲਿਆ_ref);
	strbuf_release(&bname);
}

ਸਥਿਰ ਖਾਲੀ parse_branch_ਮਿਲਾਓ_options(ਅੱਖਰ *bmo)
{
	ਅਚਲ ਅੱਖਰ **argv;
	ਪੂਰਨ argc;

	ਜੇ (!bmo)
		ਵਾਪਸ;
	argc = ਵੰਡੋ_cmdline(bmo, &argv);
	ਜੇ (argc < 0)
		die(_("Bad branch.%s.mergeoptions string: %s"), branch,
		    _(ਵੰਡੋ_cmdline_strerror(argc)));
	REALLOC_ARRAY(argv, argc + 2);
	MOVE_ARRAY(argv + 1, argv, argc + 1);
	argc++;
	argv[0] = "branch.*.mergeoptions";
	parse_options(argc, argv, ਨਿਰਾ, builtin_ਮਿਲਾਓ_options,
		      builtin_ਮਿਲਾਓ_usage, 0);
	ਮੁਕਤ(argv);
}

ਸਥਿਰ ਪੂਰਨ git_ਮਿਲਾਓ_config(ਅਚਲ ਅੱਖਰ *k, ਅਚਲ ਅੱਖਰ *v,
			    ਅਚਲ ਬਣਤਰ config_context *ctx, ਖਾਲੀ *cb)
{
	ਪੂਰਨ ਸਥਿਤੀ;
	ਅਚਲ ਅੱਖਰ *str;

	ਜੇ (branch &&
	    skip_ਅਗੇਤਰ(k, "branch.", &str) &&
	    skip_ਅਗੇਤਰ(str, branch, &str) &&
	    !strcmp(str, ".mergeoptions")) {
		ਮੁਕਤ(branch_mergeoptions);
		branch_mergeoptions = xstrdup(v);
		ਵਾਪਸ 0;
	}

	ਜੇ (!strcmp(k, "merge.diffstat") || !strcmp(k, "merge.stat")) {
		ਪੂਰਨ val = git_parse_maybe_bool_ਪਾਠ(v);
		ਚੋਣ (val) {
		ਹਾਲਤ 0:
			show_diffstat = 0;
			ਰੋਕੋ;
		ਹਾਲਤ 1:
			show_diffstat = ਮਿਲਾਓ_SHOW_DIFFSTAT;
			ਰੋਕੋ;
		ਮੂਲ:
			ਜੇ (!strcmp(v, "compact"))
				show_diffstat = ਮਿਲਾਓ_SHOW_COMPACTSUMMARY;
			/*
			 * We do not need to have an explicit
			 *
			 * else if (!strcmp(v, "diffstat"))
			 *	show_diffstat = MERGE_SHOW_DIFFSTAT;
			 *
			 * here, because the catch-all uses the
			 * diffstat style anyway.
			 */
			ਨਹੀਂਤਾਂ
				/*
				 * A setting from a future?  It is not an
				 * error grave enough to fail the command.
				 * proceed using the default one.
				 */
				show_diffstat = ਮਿਲਾਓ_SHOW_DIFFSTAT;
			ਰੋਕੋ;
		}
	} ਨਹੀਂਤਾਂ ਜੇ (!strcmp(k, "merge.verifysignatures")) {
		verify_signatures = git_config_bool(k, v);
	} ਨਹੀਂਤਾਂ ਜੇ (!strcmp(k, "pull.twohead")) {
		FREE_AND_NULL(pull_twohead);
		ਵਾਪਸ git_config_string(&pull_twohead, k, v);
	} ਨਹੀਂਤਾਂ ਜੇ (!strcmp(k, "pull.octopus")) {
		FREE_AND_NULL(pull_octopus);
		ਵਾਪਸ git_config_string(&pull_octopus, k, v);
	} ਨਹੀਂਤਾਂ ਜੇ (!strcmp(k, "commit.cleanup")) {
		ਵਾਪਸ git_config_string(&cleanup_arg, k, v);
	} ਨਹੀਂਤਾਂ ਜੇ (!strcmp(k, "merge.ff")) {
		ਪੂਰਨ boolval = git_parse_maybe_bool(v);
		ਜੇ (0 <= boolval) {
			fast_forward = boolval ? FF_ALLOW : FF_NO;
		} ਨਹੀਂਤਾਂ ਜੇ (v && !strcmp(v, "only")) {
			fast_forward = FF_ONLY;
		} /* do not barf on values from future versions of git */
		ਵਾਪਸ 0;
	} ਨਹੀਂਤਾਂ ਜੇ (!strcmp(k, "merge.defaulttoupstream")) {
		default_ਮੰਜ਼ਿਲ_upstream = git_config_bool(k, v);
		ਵਾਪਸ 0;
	} ਨਹੀਂਤਾਂ ਜੇ (!strcmp(k, "commit.gpgsign")) {
		sign_commit = git_config_bool(k, v) ? "" : ਨਿਰਾ;
		ਵਾਪਸ 0;
	} ਨਹੀਂਤਾਂ ਜੇ (!strcmp(k, "gpg.mintrustlevel")) {
		check_trust_level = 0;
	} ਨਹੀਂਤਾਂ ਜੇ (!strcmp(k, "merge.autostash")) {
		autostash = git_config_bool(k, v);
		ਵਾਪਸ 0;
	}

	ਸਥਿਤੀ = fmt_ਮਿਲਾਓ_ਸੁਨੇਹਾ_config(k, v, ctx, cb);
	ਜੇ (ਸਥਿਤੀ)
		ਵਾਪਸ ਸਥਿਤੀ;
	ਵਾਪਸ git_diff_ui_config(k, v, ctx, cb);
}

ਸਥਿਰ ਪੂਰਨ read_ਰੁੱਖ_trivial(ਬਣਤਰ object_id *common, ਬਣਤਰ object_id *ਸਿਰ,
			     ਬਣਤਰ object_id *one)
{
	ਪੂਰਨ i, nr_trees = 0;
	ਬਣਤਰ ਰੁੱਖ *trees[MAX_UNPACK_TREES];
	ਬਣਤਰ ਰੁੱਖ_desc t[MAX_UNPACK_TREES];
	ਬਣਤਰ unpack_trees_options opts;

	memset(&opts, 0, ਆਕਾਰ(opts));
	opts.ਸਿਰ_idx = 2;
	opts.src_ਸੂਚਕਾਂਕ = the_repository->ਸੂਚਕਾਂਕ;
	opts.dst_ਸੂਚਕਾਂਕ = the_repository->ਸੂਚਕਾਂਕ;
	opts.update = 1;
	opts.verbose_update = 1;
	opts.trivial_merges_only = 1;
	opts.ਮਿਲਾਓ = 1;
	opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
	trees[nr_trees] = repo_parse_ਰੁੱਖ_ਅਸਿੱਧਾ(the_repository, common);
	ਜੇ (!trees[nr_trees++])
		ਵਾਪਸ -1;
	trees[nr_trees] = repo_parse_ਰੁੱਖ_ਅਸਿੱਧਾ(the_repository, ਸਿਰ);
	ਜੇ (!trees[nr_trees++])
		ਵਾਪਸ -1;
	trees[nr_trees] = repo_parse_ਰੁੱਖ_ਅਸਿੱਧਾ(the_repository, one);
	ਜੇ (!trees[nr_trees++])
		ਵਾਪਸ -1;
	opts.fn = threeway_ਮਿਲਾਓ;
	cache_ਰੁੱਖ_free(&the_repository->ਸੂਚਕਾਂਕ->cache_ਰੁੱਖ);
	ਲਈ (i = 0; i < nr_trees; i++) {
		repo_parse_ਰੁੱਖ(the_repository, trees[i]);
		init_ਰੁੱਖ_desc(t+i, &trees[i]->object.oid,
			       trees[i]->ਬਫਰ, trees[i]->ਮਾਪ);
	}
	ਜੇ (unpack_trees(nr_trees, t, &opts))
		ਵਾਪਸ -1;
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ write_ਰੁੱਖ_trivial(ਬਣਤਰ object_id *oid)
{
	ਜੇ (write_ਸੂਚਕਾਂਕ_as_ਰੁੱਖ(oid, the_repository->ਸੂਚਕਾਂਕ,
				repo_get_ਸੂਚਕਾਂਕ_file(the_repository),
				0, ਨਿਰਾ))
		die(_("git write-tree failed to write a tree"));
}

ਸਥਿਰ ਪੂਰਨ try_ਮਿਲਾਓ_strategy(ਅਚਲ ਅੱਖਰ *strategy, ਬਣਤਰ commit_list *common,
			      ਬਣਤਰ commit_list *remoteheads,
			      ਬਣਤਰ commit *ਸਿਰ)
{
	ਅਚਲ ਅੱਖਰ *ਸਿਰ_arg = "HEAD";

	ਜੇ (repo_refresh_and_write_ਸੂਚਕਾਂਕ(the_repository, REFRESH_QUIET,
					 SKIP_IF_UNCHANGED, 0, ਨਿਰਾ, ਨਿਰਾ,
					 ਨਿਰਾ) < 0)
		die(_("Unable to write index."));

	ਜੇ (!strcmp(strategy, "recursive") || !strcmp(strategy, "subtree") ||
	    !strcmp(strategy, "ort")) {
		ਬਣਤਰ ਤਾਲਾ_file ਤਾਲਾ = ਤਾਲਾ_INIT;
		ਪੂਰਨ clean, x;
		ਬਣਤਰ commit *ਨਤੀਜਾ;
		ਬਣਤਰ commit_list *reversed = ਨਿਰਾ;
		ਬਣਤਰ ਮਿਲਾਓ_options o;
		ਬਣਤਰ commit_list *j;

		ਜੇ (remoteheads->ਅਗਲਾ) {
			error(_("Not handling anything other than two heads merge."));
			ਵਾਪਸ 2;
		}

		init_ui_ਮਿਲਾਓ_options(&o, the_repository);
		ਜੇ (!strcmp(strategy, "subtree"))
			o.subtree_ਖਿਸਕਾਓ = "";

		o.show_rename_ਤਰੱਕੀ =
			show_ਤਰੱਕੀ == -1 ? isatty(2) : show_ਤਰੱਕੀ;

		ਲਈ (x = 0; x < xopts.nr; x++)
			ਜੇ (parse_ਮਿਲਾਓ_opt(&o, xopts.v[x]))
				die(_("unknown strategy option: -X%s"), xopts.v[x]);

		o.branch1 = ਸਿਰ_arg;
		o.branch2 = ਮਿਲਾਓ_remote_util(remoteheads->ਵਸਤੂ)->name;

		ਲਈ (j = common; j; j = j->ਅਗਲਾ)
			commit_list_ਪਾਓ(j->ਵਸਤੂ, &reversed);

		repo_hold_locked_ਸੂਚਕਾਂਕ(the_repository, &ਤਾਲਾ,
				       ਤਾਲਾ_DIE_ON_ERROR);
		clean = ਮਿਲਾਓ_ort_recursive(&o, ਸਿਰ, remoteheads->ਵਸਤੂ,
					    reversed, &ਨਤੀਜਾ);
		commit_list_free(reversed);
		strbuf_release(&o.obuf);

		ਜੇ (clean < 0) {
			rollback_ਤਾਲਾ_file(&ਤਾਲਾ);
			ਵਾਪਸ 2;
		}
		ਜੇ (write_locked_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ, &ਤਾਲਾ,
				       COMMIT_ਤਾਲਾ | SKIP_IF_UNCHANGED))
			die(_("unable to write %s"), repo_get_ਸੂਚਕਾਂਕ_file(the_repository));
		ਵਾਪਸ clean ? 0 : 1;
	} ਨਹੀਂਤਾਂ {
		ਵਾਪਸ try_ਮਿਲਾਓ_command(the_repository,
					 strategy, xopts.nr, xopts.v,
					 common, ਸਿਰ_arg, remoteheads);
	}
}

ਸਥਿਰ ਖਾਲੀ ਗਿਣਤੀ_diff_files(ਬਣਤਰ diff_ਕਤਾਰ_struct *q,
			     ਬਣਤਰ diff_options *opt ਅਣਵਰਤਿਆ, ਖਾਲੀ *ਅੰਕੜੇ)
{
	ਪੂਰਨ *ਗਿਣਤੀ_2 = ਅੰਕੜੇ;

	(*ਗਿਣਤੀ_2) += q->nr;
}

ਸਥਿਰ ਪੂਰਨ ਗਿਣਤੀ_unmerged_entries(ਖਾਲੀ)
{
	ਪੂਰਨ i, ret = 0;

	ਲਈ (i = 0; i < the_repository->ਸੂਚਕਾਂਕ->cache_nr; i++)
		ਜੇ (ce_stage(the_repository->ਸੂਚਕਾਂਕ->cache[i]))
			ret++;

	ਵਾਪਸ ret;
}

ਸਥਿਰ ਖਾਲੀ ਜੋੜੋ_strategies(ਅਚਲ ਅੱਖਰ *string, ਬਿਨਾਨਿਸ਼ਾਨੀ attr)
{
	ਪੂਰਨ i;

	ਜੇ (string) {
		ਬਣਤਰ string_list list = STRING_LIST_INIT_DUP;
		ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ;
		string_list_ਵੰਡੋ(&list, string, " ", -1);
		for_each_string_list_ਵਸਤੂ(ਵਸਤੂ, &list)
			append_strategy(get_strategy(ਵਸਤੂ->string));
		string_list_clear(&list, 0);
		ਵਾਪਸ;
	}
	ਲਈ (i = 0; i < ARRAY_ਮਾਪ(all_strategy); i++)
		ਜੇ (all_strategy[i].attr & attr)
			append_strategy(&all_strategy[i]);

}

ਸਥਿਰ ਖਾਲੀ read_ਮਿਲਾਓ_ਸੁਨੇਹਾ(ਬਣਤਰ strbuf *ਸੁਨੇਹਾ)
{
	ਅਚਲ ਅੱਖਰ *filename = git_ਰਸਤਾ_ਮਿਲਾਓ_ਸੁਨੇਹਾ(the_repository);
	strbuf_reset(ਸੁਨੇਹਾ);
	ਜੇ (strbuf_read_file(ਸੁਨੇਹਾ, filename, 0) < 0)
		die_errno(_("Could not read from '%s'"), filename);
}

ਸਥਿਰ ਖਾਲੀ write_ਮਿਲਾਓ_state(ਬਣਤਰ commit_list *);
ਸਥਿਰ ਖਾਲੀ abort_commit(ਬਣਤਰ commit_list *remoteheads, ਅਚਲ ਅੱਖਰ *err_ਸੁਨੇਹਾ)
{
	ਜੇ (err_ਸੁਨੇਹਾ)
		error("%s", err_ਸੁਨੇਹਾ);
	fprintf(stderr,
		_("Not committing merge; use 'git commit' to complete the merge.\n"));
	write_ਮਿਲਾਓ_state(remoteheads);
	ਬਾਹਰ_ਨਿਕਲੋ(1);
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ ਮਿਲਾਓ_editor_comment[] =
N_("Please enter a commit message to explain why this merge is necessary,\n"
   "especially if it merges an updated upstream into a topic branch.\n"
   "\n");

ਸਥਿਰ ਅਚਲ ਅੱਖਰ scissors_editor_comment[] =
N_("An empty message aborts the commit.\n");

ਸਥਿਰ ਅਚਲ ਅੱਖਰ no_scissors_editor_comment[] =
N_("Lines starting with '%s' will be ignored, and an empty message aborts\n"
   "the commit.\n");

ਸਥਿਰ ਖਾਲੀ write_ਮਿਲਾਓ_heads(ਬਣਤਰ commit_list *);
ਸਥਿਰ ਖਾਲੀ prepare_ਮੰਜ਼ਿਲ_commit(ਬਣਤਰ commit_list *remoteheads)
{
	ਬਣਤਰ strbuf ਸੁਨੇਹਾ = STRBUF_INIT;
	ਅਚਲ ਅੱਖਰ *ਸੂਚਕਾਂਕ_file = repo_get_ਸੂਚਕਾਂਕ_file(the_repository);

	ਜੇ (!no_verify) {
		ਪੂਰਨ invoked_hook;

		ਜੇ (ਲੜੀਵਾਰ_commit_hook(0 < option_edit, ਸੂਚਕਾਂਕ_file, &invoked_hook,
				    "pre-merge-commit", ਨਿਰਾ))
			abort_commit(remoteheads, ਨਿਰਾ);
		/*
		 * Re-read the index as pre-merge-commit hook could have updated it,
		 * and write it out as a tree.  We must do this before we invoke
		 * the editor and after we invoke run_status above.
		 */
		ਜੇ (invoked_hook)
			discard_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ);
	}
	read_ਸੂਚਕਾਂਕ_from(the_repository->ਸੂਚਕਾਂਕ, ਸੂਚਕਾਂਕ_file,
			repo_get_git_dir(the_repository));
	strbuf_addbuf(&ਸੁਨੇਹਾ, &ਮਿਲਾਓ_ਸੁਨੇਹਾ);
	ਜੇ (squash)
		BUG("the control must not reach here under --squash");
	ਜੇ (0 < option_edit) {
		strbuf_addch(&ਸੁਨੇਹਾ, '\n');
		ਜੇ (cleanup_ਬਹੁਲਕ == COMMIT_ਸੁਨੇਹਾ_CLEANUP_SCISSORS) {
			wt_ਸਥਿਤੀ_append_cut_ਸਤਰ(&ਸੁਨੇਹਾ);
			strbuf_commented_addf(&ਸੁਨੇਹਾ, comment_ਸਤਰ_str, "\n");
		}
		strbuf_commented_addf(&ਸੁਨੇਹਾ, comment_ਸਤਰ_str,
				      _(ਮਿਲਾਓ_editor_comment));
		ਜੇ (cleanup_ਬਹੁਲਕ == COMMIT_ਸੁਨੇਹਾ_CLEANUP_SCISSORS)
			strbuf_commented_addf(&ਸੁਨੇਹਾ, comment_ਸਤਰ_str,
					      _(scissors_editor_comment));
		ਨਹੀਂਤਾਂ
			strbuf_commented_addf(&ਸੁਨੇਹਾ, comment_ਸਤਰ_str,
				_(no_scissors_editor_comment), comment_ਸਤਰ_str);
	}
	ਜੇ (signoff)
		append_signoff(&ਸੁਨੇਹਾ, ignored_log_ਸੁਨੇਹਾ_ਬਾਈਟਾਂ(ਸੁਨੇਹਾ.buf, ਸੁਨੇਹਾ.len), 0);
	write_ਮਿਲਾਓ_heads(remoteheads);
	write_file_buf(git_ਰਸਤਾ_ਮਿਲਾਓ_ਸੁਨੇਹਾ(the_repository), ਸੁਨੇਹਾ.buf, ਸੁਨੇਹਾ.len);
	ਜੇ (ਲੜੀਵਾਰ_commit_hook(0 < option_edit, repo_get_ਸੂਚਕਾਂਕ_file(the_repository),
			    ਨਿਰਾ, "prepare-commit-msg",
			    git_ਰਸਤਾ_ਮਿਲਾਓ_ਸੁਨੇਹਾ(the_repository), "merge", ਨਿਰਾ))
		abort_commit(remoteheads, ਨਿਰਾ);
	ਜੇ (0 < option_edit) {
		ਜੇ (launch_editor(git_ਰਸਤਾ_ਮਿਲਾਓ_ਸੁਨੇਹਾ(the_repository), ਨਿਰਾ, ਨਿਰਾ))
			abort_commit(remoteheads, ਨਿਰਾ);
	}

	ਜੇ (!no_verify && ਲੜੀਵਾਰ_commit_hook(0 < option_edit, repo_get_ਸੂਚਕਾਂਕ_file(the_repository),
					  ਨਿਰਾ, "commit-msg",
					  git_ਰਸਤਾ_ਮਿਲਾਓ_ਸੁਨੇਹਾ(the_repository), ਨਿਰਾ))
		abort_commit(remoteheads, ਨਿਰਾ);

	read_ਮਿਲਾਓ_ਸੁਨੇਹਾ(&ਸੁਨੇਹਾ);
	cleanup_ਸੁਨੇਹਾ(&ਸੁਨੇਹਾ, cleanup_ਬਹੁਲਕ, 0);
	ਜੇ (!ਸੁਨੇਹਾ.len)
		abort_commit(remoteheads, _("Empty commit message."));
	strbuf_release(&ਮਿਲਾਓ_ਸੁਨੇਹਾ);
	strbuf_addbuf(&ਮਿਲਾਓ_ਸੁਨੇਹਾ, &ਸੁਨੇਹਾ);
	strbuf_release(&ਸੁਨੇਹਾ);
}

ਸਥਿਰ ਪੂਰਨ ਮਿਲਾਓ_trivial(ਬਣਤਰ commit *ਸਿਰ, ਬਣਤਰ commit_list *remoteheads)
{
	ਬਣਤਰ object_id ਨਤੀਜਾ_ਰੁੱਖ, ਨਤੀਜਾ_commit;
	ਬਣਤਰ commit_list *parents = ਨਿਰਾ, **pptr = &parents;

	ਜੇ (repo_refresh_and_write_ਸੂਚਕਾਂਕ(the_repository, REFRESH_QUIET,
					 SKIP_IF_UNCHANGED, 0, ਨਿਰਾ, ਨਿਰਾ,
					 ਨਿਰਾ) < 0)
		ਵਾਪਸ error(_("Unable to write index."));

	write_ਰੁੱਖ_trivial(&ਨਤੀਜਾ_ਰੁੱਖ);
	ਲਿਖੋ(_("Wonderful.\n"));
	pptr = commit_list_append(ਸਿਰ, pptr);
	pptr = commit_list_append(remoteheads->ਵਸਤੂ, pptr);
	prepare_ਮੰਜ਼ਿਲ_commit(remoteheads);
	ਜੇ (commit_ਰੁੱਖ(ਮਿਲਾਓ_ਸੁਨੇਹਾ.buf, ਮਿਲਾਓ_ਸੁਨੇਹਾ.len, &ਨਤੀਜਾ_ਰੁੱਖ, parents,
			&ਨਤੀਜਾ_commit, ਨਿਰਾ, sign_commit))
		die(_("failed to write commit object"));
	ਅੰਤ(ਸਿਰ, remoteheads, &ਨਤੀਜਾ_commit, "In-index merge");

	remove_ਮਿਲਾਓ_branch_state(the_repository);
	commit_list_free(parents);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ ਅੰਤ_automerge(ਬਣਤਰ commit *ਸਿਰ,
			    ਪੂਰਨ ਸਿਰ_subsumed,
			    ਬਣਤਰ commit_list *common,
			    ਬਣਤਰ commit_list *remoteheads,
			    ਬਣਤਰ object_id *ਨਤੀਜਾ_ਰੁੱਖ,
			    ਅਚਲ ਅੱਖਰ *wt_strategy)
{
	ਬਣਤਰ commit_list *parents = ਨਿਰਾ;
	ਬਣਤਰ strbuf buf = STRBUF_INIT;
	ਬਣਤਰ object_id ਨਤੀਜਾ_commit;

	write_ਰੁੱਖ_trivial(ਨਤੀਜਾ_ਰੁੱਖ);
	commit_list_free(common);
	parents = remoteheads;
	ਜੇ (!ਸਿਰ_subsumed || fast_forward == FF_NO)
		commit_list_ਪਾਓ(ਸਿਰ, &parents);
	prepare_ਮੰਜ਼ਿਲ_commit(remoteheads);
	ਜੇ (commit_ਰੁੱਖ(ਮਿਲਾਓ_ਸੁਨੇਹਾ.buf, ਮਿਲਾਓ_ਸੁਨੇਹਾ.len, ਨਤੀਜਾ_ਰੁੱਖ, parents,
			&ਨਤੀਜਾ_commit, ਨਿਰਾ, sign_commit))
		die(_("failed to write commit object"));
	strbuf_addf(&buf, "Merge made by the '%s' strategy.", wt_strategy);
	ਅੰਤ(ਸਿਰ, remoteheads, &ਨਤੀਜਾ_commit, buf.buf);

	strbuf_release(&buf);
	remove_ਮਿਲਾਓ_branch_state(the_repository);
	commit_list_free(parents);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ suggest_conflicts(ਖਾਲੀ)
{
	ਅਚਲ ਅੱਖਰ *filename;
	FILE *fp;
	ਬਣਤਰ strbuf msgbuf = STRBUF_INIT;

	filename = git_ਰਸਤਾ_ਮਿਲਾਓ_ਸੁਨੇਹਾ(the_repository);
	fp = xfopen(filename, "a");

	/*
	 * We can't use cleanup_mode because if we're not using the editor,
	 * get_cleanup_mode will return COMMIT_MSG_CLEANUP_SPACE instead, even
	 * though the message is meant to be processed later by git-commit.
	 * Thus, we will get the cleanup mode which is returned when we _are_
	 * using an editor.
	 */
	append_conflicts_hint(the_repository->ਸੂਚਕਾਂਕ, &msgbuf,
			      get_cleanup_ਬਹੁਲਕ(cleanup_arg, 1));
	fputs(msgbuf.buf, fp);
	strbuf_release(&msgbuf);
	fclose(fp);
	repo_rerere(the_repository, allow_rerere_auto);
	ਲਿਖੋ(_("Automatic merge failed; "
			"fix conflicts and then commit the result.\n"));
	ਵਾਪਸ 1;
}

ਸਥਿਰ ਪੂਰਨ evaluate_ਨਤੀਜਾ(ਖਾਲੀ)
{
	ਪੂਰਨ cnt = 0;
	ਬਣਤਰ rev_info rev;

	/* Check how many files differ. */
	repo_init_revisions(the_repository, &rev, "");
	ਤਿਆਰੀ_revisions(0, ਨਿਰਾ, &rev, ਨਿਰਾ);
	rev.diffopt.output_format |=
		DIFF_FORMAT_CALLBACK;
	rev.diffopt.format_callback = ਗਿਣਤੀ_diff_files;
	rev.diffopt.format_callback_ਅੰਕੜੇ = &cnt;
	ਲੜੀਵਾਰ_diff_files(&rev, 0);

	/*
	 * Check how many unmerged entries are
	 * there.
	 */
	cnt += ਗਿਣਤੀ_unmerged_entries();

	release_revisions(&rev);
	ਵਾਪਸ cnt;
}

/*
 * Pretend as if the user told us to merge with the remote-tracking
 * branch we have for the upstream of the current branch
 */
ਸਥਿਰ ਪੂਰਨ ਤਿਆਰੀ_with_upstream(ਅਚਲ ਅੱਖਰ ***argv)
{
	ਬਣਤਰ branch *branch = branch_get(ਨਿਰਾ);
	ਪੂਰਨ i;
	ਅਚਲ ਅੱਖਰ **ਦਲੀਲਾਂ;

	ਜੇ (!branch)
		die(_("No current branch."));
	ਜੇ (!branch->remote_name)
		die(_("No remote for the current branch."));
	ਜੇ (!branch->ਮਿਲਾਓ_nr)
		die(_("No default upstream defined for the current branch."));

	ਦਲੀਲਾਂ = xcalloc(st_ਜੋੜੋ(branch->ਮਿਲਾਓ_nr, 1), ਆਕਾਰ(ਅੱਖਰ *));
	ਲਈ (i = 0; i < branch->ਮਿਲਾਓ_nr; i++) {
		ਜੇ (!branch->ਮਿਲਾਓ[i]->dst)
			die(_("No remote-tracking branch for %s from %s"),
			    branch->ਮਿਲਾਓ[i]->src, branch->remote_name);
		ਦਲੀਲਾਂ[i] = branch->ਮਿਲਾਓ[i]->dst;
	}
	ਦਲੀਲਾਂ[i] = ਨਿਰਾ;
	*argv = ਦਲੀਲਾਂ;
	ਵਾਪਸ i;
}

ਸਥਿਰ ਖਾਲੀ write_ਮਿਲਾਓ_heads(ਬਣਤਰ commit_list *remoteheads)
{
	ਬਣਤਰ commit_list *j;
	ਬਣਤਰ strbuf buf = STRBUF_INIT;

	ਲਈ (j = remoteheads; j; j = j->ਅਗਲਾ) {
		ਬਣਤਰ object_id *oid;
		ਬਣਤਰ commit *c = j->ਵਸਤੂ;
		ਬਣਤਰ ਮਿਲਾਓ_remote_desc *desc;

		desc = ਮਿਲਾਓ_remote_util(c);
		ਜੇ (desc && desc->obj) {
			oid = &desc->obj->oid;
		} ਨਹੀਂਤਾਂ {
			oid = &c->object.oid;
		}
		strbuf_addf(&buf, "%s\n", oid_ਮੰਜ਼ਿਲ_hex(oid));
	}
	write_file_buf(git_ਰਸਤਾ_ਮਿਲਾਓ_ਸਿਰ(the_repository), buf.buf, buf.len);

	strbuf_reset(&buf);
	ਜੇ (fast_forward == FF_NO)
		strbuf_addstr(&buf, "no-ff");
	write_file_buf(git_ਰਸਤਾ_ਮਿਲਾਓ_ਬਹੁਲਕ(the_repository), buf.buf, buf.len);
	strbuf_release(&buf);
}

ਸਥਿਰ ਖਾਲੀ write_ਮਿਲਾਓ_state(ਬਣਤਰ commit_list *remoteheads)
{
	write_ਮਿਲਾਓ_heads(remoteheads);
	strbuf_addch(&ਮਿਲਾਓ_ਸੁਨੇਹਾ, '\n');
	write_file_buf(git_ਰਸਤਾ_ਮਿਲਾਓ_ਸੁਨੇਹਾ(the_repository), ਮਿਲਾਓ_ਸੁਨੇਹਾ.buf,
		       ਮਿਲਾਓ_ਸੁਨੇਹਾ.len);
}

ਸਥਿਰ ਪੂਰਨ default_edit_option(ਖਾਲੀ)
{
	ਸਥਿਰ ਅਚਲ ਅੱਖਰ name[] = "GIT_MERGE_AUTOEDIT";
	ਅਚਲ ਅੱਖਰ *e = getenv(name);
	ਬਣਤਰ stat st_stdin, st_stdout;

	ਜੇ (have_ਸੁਨੇਹਾ)
		/* an explicit -m msg without --[no-]edit */
		ਵਾਪਸ 0;

	ਜੇ (e) {
		ਪੂਰਨ v = git_parse_maybe_bool(e);
		ਜੇ (v < 0)
			die(_("Bad value '%s' in environment '%s'"), e, name);
		ਵਾਪਸ v;
	}

	/* Use editor if stdin and stdout are the same and is a tty */
	ਵਾਪਸ (!fstat(0, &st_stdin) &&
		!fstat(1, &st_stdout) &&
		isatty(0) && isatty(1) &&
		st_stdin.st_dev == st_stdout.st_dev &&
		st_stdin.st_ino == st_stdout.st_ino &&
		st_stdin.st_ਬਹੁਲਕ == st_stdout.st_ਬਹੁਲਕ);
}

ਸਥਿਰ ਬਣਤਰ commit_list *reduce_parents(ਬਣਤਰ commit *ਸਿਰ_commit,
					  ਪੂਰਨ *ਸਿਰ_subsumed,
					  ਬਣਤਰ commit_list *remoteheads)
{
	ਬਣਤਰ commit_list *parents, **remotes;

	/*
	 * Is the current HEAD reachable from another commit being
	 * merged?  If so we do not want to record it as a parent of
	 * the resulting merge, unless --no-ff is given.  We will flip
	 * this variable to 0 when we find HEAD among the independent
	 * tips being merged.
	 */
	*ਸਿਰ_subsumed = 1;

	/* Find what parents to record by checking independent ones. */
	parents = reduce_heads(remoteheads);
	commit_list_free(remoteheads);

	remoteheads = ਨਿਰਾ;
	remotes = &remoteheads;
	ਜਦੋਂ (parents) {
		ਬਣਤਰ commit *commit = ਕੱਢੋ_commit(&parents);
		ਜੇ (commit == ਸਿਰ_commit)
			*ਸਿਰ_subsumed = 0;
		ਨਹੀਂਤਾਂ
			remotes = &commit_list_ਪਾਓ(commit, remotes)->ਅਗਲਾ;
	}
	ਵਾਪਸ remoteheads;
}

ਸਥਿਰ ਖਾਲੀ prepare_ਮਿਲਾਓ_ਸੁਨੇਹਾ(ਬਣਤਰ strbuf *ਮਿਲਾਓ_ਨਾਂ, ਬਣਤਰ strbuf *ਮਿਲਾਓ_ਸੁਨੇਹਾ)
{
	ਬਣਤਰ fmt_ਮਿਲਾਓ_ਸੁਨੇਹਾ_opts opts;

	memset(&opts, 0, ਆਕਾਰ(opts));
	opts.ਜੋੜੋ_title = !have_ਸੁਨੇਹਾ;
	opts.shortlog_len = shortlog_len;
	opts.credit_people = (0 < option_edit);
	opts.into_name = into_name;

	fmt_ਮਿਲਾਓ_ਸੁਨੇਹਾ(ਮਿਲਾਓ_ਨਾਂ, ਮਿਲਾਓ_ਸੁਨੇਹਾ, &opts);
	ਜੇ (ਮਿਲਾਓ_ਸੁਨੇਹਾ->len)
		strbuf_setlen(ਮਿਲਾਓ_ਸੁਨੇਹਾ, ਮਿਲਾਓ_ਸੁਨੇਹਾ->len - 1);
}

ਸਥਿਰ ਖਾਲੀ handle_fetch_ਸਿਰ(ਬਣਤਰ commit_list **remotes, ਬਣਤਰ strbuf *ਮਿਲਾਓ_ਨਾਂ)
{
	ਅਚਲ ਅੱਖਰ *filename;
	ਪੂਰਨ fd, pos, npos;
	ਬਣਤਰ strbuf fetch_ਸਿਰ_file = STRBUF_INIT;
	ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ hexsz = the_hash_algo->hexsz;

	ਜੇ (!ਮਿਲਾਓ_ਨਾਂ)
		ਮਿਲਾਓ_ਨਾਂ = &fetch_ਸਿਰ_file;

	filename = git_ਰਸਤਾ_fetch_ਸਿਰ(the_repository);
	fd = xopen(filename, O_RDONLY);

	ਜੇ (strbuf_read(ਮਿਲਾਓ_ਨਾਂ, fd, 0) < 0)
		die_errno(_("could not read '%s'"), filename);
	ਜੇ (close(fd) < 0)
		die_errno(_("could not close '%s'"), filename);

	ਲਈ (pos = 0; pos < ਮਿਲਾਓ_ਨਾਂ->len; pos = npos) {
		ਬਣਤਰ object_id oid;
		ਅੱਖਰ *ptr;
		ਬਣਤਰ commit *commit;

		ptr = strchr(ਮਿਲਾਓ_ਨਾਂ->buf + pos, '\n');
		ਜੇ (ptr)
			npos = ptr - ਮਿਲਾਓ_ਨਾਂ->buf + 1;
		ਨਹੀਂਤਾਂ
			npos = ਮਿਲਾਓ_ਨਾਂ->len;

		ਜੇ (npos - pos < hexsz + 2 ||
		    get_oid_hex(ਮਿਲਾਓ_ਨਾਂ->buf + pos, &oid))
			commit = ਨਿਰਾ; /* bad */
		ਨਹੀਂਤਾਂ ਜੇ (memcmp(ਮਿਲਾਓ_ਨਾਂ->buf + pos + hexsz, "\t\t", 2))
			ਜਾਰੀ; /* not-for-merge */
		ਨਹੀਂਤਾਂ {
			ਅੱਖਰ saved = ਮਿਲਾਓ_ਨਾਂ->buf[pos + hexsz];
			ਮਿਲਾਓ_ਨਾਂ->buf[pos + hexsz] = '\0';
			commit = get_ਮਿਲਾਓ_ਮਾਪਾ(ਮਿਲਾਓ_ਨਾਂ->buf + pos);
			ਮਿਲਾਓ_ਨਾਂ->buf[pos + hexsz] = saved;
		}
		ਜੇ (!commit) {
			ਜੇ (ptr)
				*ptr = '\0';
			die(_("not something we can merge in %s: %s"),
			    filename, ਮਿਲਾਓ_ਨਾਂ->buf + pos);
		}
		remotes = &commit_list_ਪਾਓ(commit, remotes)->ਅਗਲਾ;
	}

	ਜੇ (ਮਿਲਾਓ_ਨਾਂ == &fetch_ਸਿਰ_file)
		strbuf_release(&fetch_ਸਿਰ_file);
}

ਸਥਿਰ ਬਣਤਰ commit_list *collect_parents(ਬਣਤਰ commit *ਸਿਰ_commit,
					   ਪੂਰਨ *ਸਿਰ_subsumed,
					   ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv,
					   ਬਣਤਰ strbuf *ਮਿਲਾਓ_ਸੁਨੇਹਾ)
{
	ਪੂਰਨ i;
	ਬਣਤਰ commit_list *remoteheads = ਨਿਰਾ;
	ਬਣਤਰ commit_list **remotes = &remoteheads;
	ਬਣਤਰ strbuf ਮਿਲਾਓ_ਨਾਂ = STRBUF_INIT, *autogen = ਨਿਰਾ;

	ਜੇ (ਮਿਲਾਓ_ਸੁਨੇਹਾ && (!have_ਸੁਨੇਹਾ || shortlog_len))
		autogen = &ਮਿਲਾਓ_ਨਾਂ;

	ਜੇ (ਸਿਰ_commit)
		remotes = &commit_list_ਪਾਓ(ਸਿਰ_commit, remotes)->ਅਗਲਾ;

	ਜੇ (argc == 1 && !strcmp(argv[0], "FETCH_HEAD")) {
		handle_fetch_ਸਿਰ(remotes, autogen);
		remoteheads = reduce_parents(ਸਿਰ_commit, ਸਿਰ_subsumed, remoteheads);
	} ਨਹੀਂਤਾਂ {
		ਲਈ (i = 0; i < argc; i++) {
			ਬਣਤਰ commit *commit = get_ਮਿਲਾਓ_ਮਾਪਾ(argv[i]);
			ਜੇ (!commit)
				help_unknown_ref(argv[i], "merge",
						 _("not something we can merge"));
			remotes = &commit_list_ਪਾਓ(commit, remotes)->ਅਗਲਾ;
		}
		remoteheads = reduce_parents(ਸਿਰ_commit, ਸਿਰ_subsumed, remoteheads);
		ਜੇ (autogen) {
			ਬਣਤਰ commit_list *p;
			ਲਈ (p = remoteheads; p; p = p->ਅਗਲਾ)
				ਮਿਲਾਓ_name(ਮਿਲਾਓ_remote_util(p->ਵਸਤੂ)->name, autogen);
		}
	}

	ਜੇ (autogen) {
		prepare_ਮਿਲਾਓ_ਸੁਨੇਹਾ(autogen, ਮਿਲਾਓ_ਸੁਨੇਹਾ);
		strbuf_release(autogen);
	}

	ਵਾਪਸ remoteheads;
}

ਸਥਿਰ ਪੂਰਨ merging_a_throwaway_ਟੈਗ(ਬਣਤਰ commit *commit)
{
	ਅੱਖਰ *ਟੈਗ_ref;
	ਬਣਤਰ object_id oid;
	ਪੂਰਨ is_throwaway_ਟੈਗ = 0;

	/* Are we merging a tag? */
	ਜੇ (!ਮਿਲਾਓ_remote_util(commit) ||
	    !ਮਿਲਾਓ_remote_util(commit)->obj ||
	    ਮਿਲਾਓ_remote_util(commit)->obj->type != OBJ_ਟੈਗ)
		ਵਾਪਸ is_throwaway_ਟੈਗ;

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
	ਟੈਗ_ref = xstrfmt("refs/tags/%s",
			  ((ਬਣਤਰ ਟੈਗ *)ਮਿਲਾਓ_remote_util(commit)->obj)->ਟੈਗ);
	ਜੇ (!refs_read_ref(get_main_ref_store(the_repository), ਟੈਗ_ref, &oid) &&
	    oideq(&oid, &ਮਿਲਾਓ_remote_util(commit)->obj->oid))
		is_throwaway_ਟੈਗ = 0;
	ਨਹੀਂਤਾਂ
		is_throwaway_ਟੈਗ = 1;
	ਮੁਕਤ(ਟੈਗ_ref);
	ਵਾਪਸ is_throwaway_ਟੈਗ;
}

ਪੂਰਨ cmd_ਮਿਲਾਓ(ਪੂਰਨ argc,
	      ਅਚਲ ਅੱਖਰ **argv,
	      ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
	      ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ object_id ਨਤੀਜਾ_ਰੁੱਖ, stash, ਸਿਰ_oid;
	ਬਣਤਰ commit *ਸਿਰ_commit;
	ਬਣਤਰ strbuf buf = STRBUF_INIT;
	ਪੂਰਨ i, ret = 0, ਸਿਰ_subsumed;
	ਪੂਰਨ ਉੱਤਮ_cnt = -1, ਮਿਲਾਓ_was_ok = 0, automerge_was_ok = 0;
	ਬਣਤਰ commit_list *common = ਨਿਰਾ;
	ਅਚਲ ਅੱਖਰ *ਉੱਤਮ_strategy = ਨਿਰਾ, *wt_strategy = ਨਿਰਾ;
	ਬਣਤਰ commit_list *remoteheads = ਨਿਰਾ, *p;
	ਖਾਲੀ *branch_ਮੰਜ਼ਿਲ_free, *argv_ਮੰਜ਼ਿਲ_free = ਨਿਰਾ;
	ਪੂਰਨ orig_argc = argc;
	ਪੂਰਨ ਮਿਲਾਓ_log_config = -1;

	show_usage_with_options_if_asked(argc, argv,
					 builtin_ਮਿਲਾਓ_usage, builtin_ਮਿਲਾਓ_options);

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ WITH_BREAKING_CHANGES
	warn_on_auto_comment_char = true;
#ਜੇ_ਅੰਤ /* !WITH_BREAKING_CHANGES */
	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_ਭਰਿਆ_ਸੂਚਕਾਂਕ = 0;

	/*
	 * Check if we are _not_ on a detached HEAD, i.e. if there is a
	 * current branch.
	 */
	branch = branch_ਮੰਜ਼ਿਲ_free = refs_resolve_refdup(get_main_ref_store(the_repository),
						      "HEAD", 0, &ਸਿਰ_oid,
						      ਨਿਰਾ);
	ਜੇ (branch)
		skip_ਅਗੇਤਰ(branch, "refs/heads/", &branch);

	init_diff_ui_defaults();
	repo_config(the_repository, git_ਮਿਲਾਓ_config, &ਮਿਲਾਓ_log_config);

	ਜੇ (!branch || is_null_oid(&ਸਿਰ_oid))
		ਸਿਰ_commit = ਨਿਰਾ;
	ਨਹੀਂਤਾਂ
		ਸਿਰ_commit = lookup_commit_or_die(&ਸਿਰ_oid, "HEAD");

	ਜੇ (branch_mergeoptions)
		parse_branch_ਮਿਲਾਓ_options(branch_mergeoptions);
	argc = parse_options(argc, argv, ਅਗੇਤਰ, builtin_ਮਿਲਾਓ_options,
			builtin_ਮਿਲਾਓ_usage, 0);
	ਜੇ (shortlog_len < 0)
		shortlog_len = (ਮਿਲਾਓ_log_config > 0) ? ਮਿਲਾਓ_log_config : 0;

	ਜੇ (verbosity < 0 && show_ਤਰੱਕੀ == -1)
		show_ਤਰੱਕੀ = 0;

	ਜੇ (abort_current_ਮਿਲਾਓ) {
		ਪੂਰਨ nargc = 2;
		ਅਚਲ ਅੱਖਰ *nargv[] = {"reset", "--merge", ਨਿਰਾ};
		ਅੱਖਰ stash_oid_hex[GIT_MAX_HEXSZ + 1];
		ਬਣਤਰ object_id stash_oid = {0};

		ਜੇ (orig_argc != 2)
			usage_ਸੁਨੇਹਾ_opt(_("--abort expects no arguments"),
			      builtin_ਮਿਲਾਓ_usage, builtin_ਮਿਲਾਓ_options);

		ਜੇ (!file_exists(git_ਰਸਤਾ_ਮਿਲਾਓ_ਸਿਰ(the_repository)))
			die(_("There is no merge to abort (MERGE_HEAD missing)."));

		ਜੇ (!refs_read_ref(get_main_ref_store(the_repository), "MERGE_AUTOSTASH", &stash_oid))
			refs_delete_ref(get_main_ref_store(the_repository),
					"", "MERGE_AUTOSTASH", &stash_oid,
					REF_NO_DEREF);

		/* Invoke 'git reset --merge' */
		ret = cmd_reset(nargc, nargv, ਅਗੇਤਰ, the_repository);

		ਜੇ (!is_null_oid(&stash_oid)) {
			oid_ਮੰਜ਼ਿਲ_hex_r(stash_oid_hex, &stash_oid);
			apply_autostash_oid(stash_oid_hex);
		}

		ਜਾਓ ਪੂਰਾ;
	}

	ਜੇ (quit_current_ਮਿਲਾਓ) {
		ਜੇ (orig_argc != 2)
			usage_ਸੁਨੇਹਾ_opt(_("--quit expects no arguments"),
				      builtin_ਮਿਲਾਓ_usage,
				      builtin_ਮਿਲਾਓ_options);

		remove_ਮਿਲਾਓ_branch_state(the_repository);
		ਜਾਓ ਪੂਰਾ;
	}

	ਜੇ (continue_current_ਮਿਲਾਓ) {
		ਪੂਰਨ nargc = 1;
		ਅਚਲ ਅੱਖਰ *nargv[] = {"commit", ਨਿਰਾ};

		ਜੇ (orig_argc != 2)
			usage_ਸੁਨੇਹਾ_opt(_("--continue expects no arguments"),
			      builtin_ਮਿਲਾਓ_usage, builtin_ਮਿਲਾਓ_options);

		ਜੇ (!file_exists(git_ਰਸਤਾ_ਮਿਲਾਓ_ਸਿਰ(the_repository)))
			die(_("There is no merge in progress (MERGE_HEAD missing)."));

		/* Invoke 'git commit' */
		ret = cmd_commit(nargc, nargv, ਅਗੇਤਰ, the_repository);
		ਜਾਓ ਪੂਰਾ;
	}

	ਜੇ (repo_read_ਸੂਚਕਾਂਕ_unmerged(the_repository))
		die_resolve_conflict("merge");

	ਜੇ (file_exists(git_ਰਸਤਾ_ਮਿਲਾਓ_ਸਿਰ(the_repository))) {
		/*
		 * There is no unmerged entry, don't advise 'git
		 * add/rm <file>', just 'git commit'.
		 */
		ਜੇ (advice_enabled(ADVICE_RESOLVE_CONFLICT))
			die(_("You have not concluded your merge (MERGE_HEAD exists).\n"
				  "Please, commit your changes before you merge."));
		ਨਹੀਂਤਾਂ
			die(_("You have not concluded your merge (MERGE_HEAD exists)."));
	}
	ਜੇ (refs_ref_exists(get_main_ref_store(the_repository), "CHERRY_PICK_HEAD")) {
		ਜੇ (advice_enabled(ADVICE_RESOLVE_CONFLICT))
			die(_("You have not concluded your cherry-pick (CHERRY_PICK_HEAD exists).\n"
			    "Please, commit your changes before you merge."));
		ਨਹੀਂਤਾਂ
			die(_("You have not concluded your cherry-pick (CHERRY_PICK_HEAD exists)."));
	}
	resolve_undo_clear_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ);

	ਜੇ (option_edit < 0)
		option_edit = default_edit_option();

	cleanup_ਬਹੁਲਕ = get_cleanup_ਬਹੁਲਕ(cleanup_arg, 0 < option_edit);

	ਜੇ (verbosity < 0)
		show_diffstat = 0;

	ਜੇ (squash) {
		ਜੇ (fast_forward == FF_NO)
			die(_("options '%s' and '%s' cannot be used together"), "--squash", "--no-ff.");
		ਜੇ (option_commit > 0)
			die(_("options '%s' and '%s' cannot be used together"), "--squash", "--commit.");
		/*
		 * squash can now silently disable option_commit - this is not
		 * a problem as it is only overriding the default, not a user
		 * supplied option.
		 */
		option_commit = 0;
	}

	ਜੇ (option_commit < 0)
		option_commit = 1;

	ਜੇ (!argc) {
		ਜੇ (default_ਮੰਜ਼ਿਲ_upstream) {
			argc = ਤਿਆਰੀ_with_upstream(&argv);
			argv_ਮੰਜ਼ਿਲ_free = argv;
		}
		ਨਹੀਂਤਾਂ
			die(_("No commit specified and merge.defaultToUpstream not set."));
	} ਨਹੀਂਤਾਂ ਜੇ (argc == 1 && !strcmp(argv[0], "-")) {
		argv[0] = "@{-1}";
	}

	ਜੇ (!argc)
		usage_with_options(builtin_ਮਿਲਾਓ_usage,
			builtin_ਮਿਲਾਓ_options);

	ਜੇ (!ਸਿਰ_commit) {
		/*
		 * If the merged head is a valid one there is no reason
		 * to forbid "git merge" into a branch yet to be born.
		 * We do the same for "git pull".
		 */
		ਬਣਤਰ object_id *remote_ਸਿਰ_oid;
		ਜੇ (squash)
			die(_("Squash commit into empty head not supported yet"));
		ਜੇ (fast_forward == FF_NO)
			die(_("Non-fast-forward commit does not make sense into "
			    "an empty head"));
		remoteheads = collect_parents(ਸਿਰ_commit, &ਸਿਰ_subsumed,
					      argc, argv, ਨਿਰਾ);
		ਜੇ (!remoteheads)
			die(_("%s - not something we can merge"), argv[0]);
		ਜੇ (remoteheads->ਅਗਲਾ)
			die(_("Can merge only exactly one commit into empty head"));

		ਜੇ (verify_signatures)
			verify_ਮਿਲਾਓ_signature(remoteheads->ਵਸਤੂ, verbosity,
					       check_trust_level);

		remote_ਸਿਰ_oid = &remoteheads->ਵਸਤੂ->object.oid;
		read_ਖਾਲੀ_ਥਾਵਾਂ(remote_ਸਿਰ_oid);
		refs_update_ref(get_main_ref_store(the_repository),
				"initial pull", "HEAD", remote_ਸਿਰ_oid, ਨਿਰਾ,
				0,
				UPDATE_REFS_DIE_ON_ERR);
		ਜਾਓ ਪੂਰਾ;
	}

	/*
	 * All the rest are the commits being merged; prepare
	 * the standard merge summary message to be appended
	 * to the given message.
	 */
	remoteheads = collect_parents(ਸਿਰ_commit, &ਸਿਰ_subsumed,
				      argc, argv, &ਮਿਲਾਓ_ਸੁਨੇਹਾ);

	ਜੇ (!ਸਿਰ_commit || !argc)
		usage_with_options(builtin_ਮਿਲਾਓ_usage,
			builtin_ਮਿਲਾਓ_options);

	ਜੇ (verify_signatures) {
		ਲਈ (p = remoteheads; p; p = p->ਅਗਲਾ) {
			verify_ਮਿਲਾਓ_signature(p->ਵਸਤੂ, verbosity,
					       check_trust_level);
		}
	}

	strbuf_addstr(&buf, "merge");
	ਲਈ (p = remoteheads; p; p = p->ਅਗਲਾ)
		strbuf_addf(&buf, " %s", ਮਿਲਾਓ_remote_util(p->ਵਸਤੂ)->name);
	setenv("GIT_REFLOG_ACTION", buf.buf, 0);
	strbuf_reset(&buf);

	ਲਈ (p = remoteheads; p; p = p->ਅਗਲਾ) {
		ਬਣਤਰ commit *commit = p->ਵਸਤੂ;
		strbuf_addf(&buf, "GITHEAD_%s",
			    oid_ਮੰਜ਼ਿਲ_hex(&commit->object.oid));
		setenv(buf.buf, ਮਿਲਾਓ_remote_util(commit)->name, 1);
		strbuf_reset(&buf);
		ਜੇ (fast_forward != FF_ONLY && merging_a_throwaway_ਟੈਗ(commit))
			fast_forward = FF_NO;
	}

	ਜੇ (!use_strategies) {
		ਜੇ (!remoteheads)
			; /* already up-to-date */
		ਨਹੀਂਤਾਂ ਜੇ (!remoteheads->ਅਗਲਾ)
			ਜੋੜੋ_strategies(pull_twohead, DEFAULT_TWOHEAD);
		ਨਹੀਂਤਾਂ
			ਜੋੜੋ_strategies(pull_octopus, DEFAULT_OCTOPUS);
	}

	ਲਈ (i = 0; i < use_strategies_nr; i++) {
		ਜੇ (use_strategies[i]->attr & NO_FAST_FORWARD)
			fast_forward = FF_NO;
		ਜੇ (use_strategies[i]->attr & NO_TRIVIAL)
			allow_trivial = 0;
	}

	ਜੇ (!remoteheads)
		; /* already up-to-date */
	ਨਹੀਂਤਾਂ ਜੇ (!remoteheads->ਅਗਲਾ) {
		ਜੇ (repo_get_ਮਿਲਾਓ_bases(the_repository, ਸਿਰ_commit,
					 remoteheads->ਵਸਤੂ, &common) < 0) {
			ret = 2;
			ਜਾਓ ਪੂਰਾ;
		}
	} ਨਹੀਂਤਾਂ {
		ਬਣਤਰ commit_list *list = remoteheads;
		commit_list_ਪਾਓ(ਸਿਰ_commit, &list);
		ਜੇ (get_octopus_ਮਿਲਾਓ_bases(list, &common) < 0) {
			ਮੁਕਤ(list);
			ret = 2;
			ਜਾਓ ਪੂਰਾ;
		}
		ਮੁਕਤ(list);
	}

	refs_update_ref(get_main_ref_store(the_repository),
			"updating ORIG_HEAD", "ORIG_HEAD",
			&ਸਿਰ_commit->object.oid, ਨਿਰਾ, 0,
			UPDATE_REFS_DIE_ON_ERR);

	ਜੇ (remoteheads && !common) {
		/* No common ancestors found. */
		ਜੇ (!allow_unrelated_histories)
			die(_("refusing to merge unrelated histories"));
		/* otherwise, we need a real merge. */
	} ਨਹੀਂਤਾਂ ਜੇ (!remoteheads ||
		 (!remoteheads->ਅਗਲਾ && !common->ਅਗਲਾ &&
		  common->ਵਸਤੂ == remoteheads->ਵਸਤੂ)) {
		/*
		 * If head can reach all the merge then we are up to date.
		 * but first the most common case of merging one remote.
		 */
		ਅੰਤ_ਉੱਪਰ_ਮੰਜ਼ਿਲ_date();
		ਜਾਓ ਪੂਰਾ;
	} ਨਹੀਂਤਾਂ ਜੇ (fast_forward != FF_NO && !remoteheads->ਅਗਲਾ &&
			!common->ਅਗਲਾ &&
			oideq(&common->ਵਸਤੂ->object.oid, &ਸਿਰ_commit->object.oid)) {
		/* Again the most common case of merging one remote. */
		ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ = have_ਸੁਨੇਹਾ ?
			"Fast-forward (no commit created; -m option ignored)" :
			"Fast-forward";
		ਬਣਤਰ commit *commit;

		ਜੇ (verbosity >= 0) {
			ਲਿਖੋ(_("Updating %s..%s\n"),
			       repo_ਲੱਭੋ_unique_abbrev(the_repository, &ਸਿਰ_commit->object.oid,
						       DEFAULT_ABBREV),
			       repo_ਲੱਭੋ_unique_abbrev(the_repository, &remoteheads->ਵਸਤੂ->object.oid,
						       DEFAULT_ABBREV));
		}
		commit = remoteheads->ਵਸਤੂ;
		ਜੇ (!commit) {
			ret = 1;
			ਜਾਓ ਪੂਰਾ;
		}

		ਜੇ (autostash)
			create_autostash_ref(the_repository, "MERGE_AUTOSTASH",
					     ਨਿਰਾ, false);
		ਜੇ (checkout_fast_forward(the_repository,
					  &ਸਿਰ_commit->object.oid,
					  &commit->object.oid,
					  overwrite_ignore)) {
			apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
					    ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ);
			ret = 1;
			ਜਾਓ ਪੂਰਾ;
		}

		ਅੰਤ(ਸਿਰ_commit, remoteheads, &commit->object.oid, ਸੁਨੇਹਾ);
		remove_ਮਿਲਾਓ_branch_state(the_repository);
		ਜਾਓ ਪੂਰਾ;
	} ਨਹੀਂਤਾਂ ਜੇ (!remoteheads->ਅਗਲਾ && common->ਅਗਲਾ)
		;
		/*
		 * We are not doing octopus and not fast-forward.  Need
		 * a real merge.
		 */
	ਨਹੀਂਤਾਂ ਜੇ (!remoteheads->ਅਗਲਾ && !common->ਅਗਲਾ && option_commit) {
		/*
		 * We are not doing octopus, not fast-forward, and have
		 * only one common.
		 */
		refresh_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ, REFRESH_QUIET, ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ);
		ਜੇ (allow_trivial && fast_forward != FF_ONLY) {
			/*
			 * Must first ensure that index matches HEAD before
			 * attempting a trivial merge.
			 */
			ਬਣਤਰ ਰੁੱਖ *ਸਿਰ_ਰੁੱਖ = repo_get_commit_ਰੁੱਖ(the_repository,
								      ਸਿਰ_commit);
			ਬਣਤਰ strbuf sb = STRBUF_INIT;

			ਜੇ (repo_ਸੂਚਕਾਂਕ_has_changes(the_repository, ਸਿਰ_ਰੁੱਖ,
						   &sb)) {
				error(_("Your local changes to the following files would be overwritten by merge:\n  %s"),
				      sb.buf);
				strbuf_release(&sb);
				ret = 2;
				ਜਾਓ ਪੂਰਾ;
			}

			/* See if it is really trivial. */
			git_committer_info(IDENT_STRICT);
			ਲਿਖੋ(_("Trying really trivial in-index merge...\n"));
			ਜੇ (!read_ਰੁੱਖ_trivial(&common->ਵਸਤੂ->object.oid,
					       &ਸਿਰ_commit->object.oid,
					       &remoteheads->ਵਸਤੂ->object.oid)) {
				ret = ਮਿਲਾਓ_trivial(ਸਿਰ_commit, remoteheads);
				ਜਾਓ ਪੂਰਾ;
			}
			ਲਿਖੋ(_("Nope.\n"));
		}
	} ਨਹੀਂਤਾਂ {
		/*
		 * An octopus.  If we can reach all the remote we are up
		 * to date.
		 */
		ਪੂਰਨ ਉੱਪਰ_ਮੰਜ਼ਿਲ_date = 1;
		ਬਣਤਰ commit_list *j;

		ਲਈ (j = remoteheads; j; j = j->ਅਗਲਾ) {
			ਪੂਰਨ ret = repo_ਵਿੱਚ_ਮਿਲਾਓ_bases(the_repository,
						      j->ਵਸਤੂ, ਸਿਰ_commit);
			ਜੇ (ret < 0)
				ਬਾਹਰ_ਨਿਕਲੋ(128);
			ਜੇ (!ret) {
				ਉੱਪਰ_ਮੰਜ਼ਿਲ_date = 0;
				ਰੋਕੋ;
			}
		}
		ਜੇ (ਉੱਪਰ_ਮੰਜ਼ਿਲ_date) {
			ਅੰਤ_ਉੱਪਰ_ਮੰਜ਼ਿਲ_date();
			ਜਾਓ ਪੂਰਾ;
		}
	}

	ਜੇ (fast_forward == FF_ONLY)
		die_ff_impossible();

	ਜੇ (autostash)
		create_autostash_ref(the_repository, "MERGE_AUTOSTASH",
				     ਨਿਰਾ, false);

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
	ਜੇ (save_state(&stash))
		oidclr(&stash, the_repository->hash_algo);

	ਲਈ (i = 0; i < use_strategies_nr; i++) {
		ਪੂਰਨ ret, cnt;
		ਜੇ (i) {
			ਲਿਖੋ(_("Rewinding the tree to pristine...\n"));
			restore_state(&ਸਿਰ_commit->object.oid, &stash);
		}
		ਜੇ (use_strategies_nr != 1)
			ਲਿਖੋ(_("Trying merge strategy %s...\n"),
				use_strategies[i]->name);
		/*
		 * Remember which strategy left the state in the working
		 * tree.
		 */
		wt_strategy = use_strategies[i]->name;

		ret = try_ਮਿਲਾਓ_strategy(wt_strategy,
					 common, remoteheads,
					 ਸਿਰ_commit);
		/*
		 * The backend exits with 1 when conflicts are
		 * left to be resolved, with 2 when it does not
		 * handle the given merge at all.
		 */
		ਜੇ (ret < 2) {
			ਜੇ (!ret) {
				/*
				 * This strategy worked; no point in trying
				 * another.
				 */
				ਮਿਲਾਓ_was_ok = 1;
				ਉੱਤਮ_strategy = wt_strategy;
				ਰੋਕੋ;
			}
			cnt = (use_strategies_nr > 1) ? evaluate_ਨਤੀਜਾ() : 0;
			ਜੇ (ਉੱਤਮ_cnt <= 0 || cnt <= ਉੱਤਮ_cnt) {
				ਉੱਤਮ_strategy = wt_strategy;
				ਉੱਤਮ_cnt = cnt;
			}
		}
	}

	/*
	 * If we have a resulting tree, that means the strategy module
	 * auto resolved the merge cleanly.
	 */
	ਜੇ (ਮਿਲਾਓ_was_ok && option_commit) {
		automerge_was_ok = 1;
		ret = ਅੰਤ_automerge(ਸਿਰ_commit, ਸਿਰ_subsumed,
				       common, remoteheads,
				       &ਨਤੀਜਾ_ਰੁੱਖ, wt_strategy);
		ਜਾਓ ਪੂਰਾ;
	}

	/*
	 * Pick the result from the best strategy and have the user fix
	 * it up.
	 */
	ਜੇ (!ਉੱਤਮ_strategy) {
		restore_state(&ਸਿਰ_commit->object.oid, &stash);
		ਜੇ (use_strategies_nr > 1)
			fprintf(stderr,
				_("No merge strategy handled the merge.\n"));
		ਨਹੀਂਤਾਂ
			fprintf(stderr, _("Merge with strategy %s failed.\n"),
				use_strategies[0]->name);
		apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
				    ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ);
		ret = 2;
		ਜਾਓ ਪੂਰਾ;
	} ਨਹੀਂਤਾਂ ਜੇ (ਉੱਤਮ_strategy == wt_strategy)
		; /* We already have its result in the working tree. */
	ਨਹੀਂਤਾਂ {
		ਲਿਖੋ(_("Rewinding the tree to pristine...\n"));
		restore_state(&ਸਿਰ_commit->object.oid, &stash);
		ਲਿਖੋ(_("Using the %s strategy to prepare resolving by hand.\n"),
			ਉੱਤਮ_strategy);
		try_ਮਿਲਾਓ_strategy(ਉੱਤਮ_strategy, common, remoteheads,
				   ਸਿਰ_commit);
	}

	ਜੇ (squash) {
		ਅੰਤ(ਸਿਰ_commit, remoteheads, ਨਿਰਾ, ਨਿਰਾ);

		git_test_write_commit_ਗ੍ਰਾਫ_or_die(the_repository);
	} ਨਹੀਂਤਾਂ
		write_ਮਿਲਾਓ_state(remoteheads);

	ਜੇ (ਮਿਲਾਓ_was_ok)
		fprintf(stderr, _("Automatic merge went well; "
			"stopped before committing as requested\n"));
	ਨਹੀਂਤਾਂ
		ret = suggest_conflicts();
	ਜੇ (autostash)
		ਲਿਖੋ(_("When finished, apply stashed changes with `git stash pop`\n"));

ਪੂਰਾ:
	ਜੇ (!automerge_was_ok) {
		commit_list_free(common);
		commit_list_free(remoteheads);
	}
	strbuf_release(&buf);
	ਮੁਕਤ(branch_ਮੰਜ਼ਿਲ_free);
	ਮੁਕਤ(argv_ਮੰਜ਼ਿਲ_free);
	ਮੁਕਤ(pull_twohead);
	ਮੁਕਤ(pull_octopus);
	discard_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ);
	ਵਾਪਸ ret;
}
