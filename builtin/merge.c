/*
 * Builtin "git merge"
 *
 * Copyright (c) 2008 Miklos Vajna <vmiklos@frugalware.org>
 *
 * Based on git-merge.sh by Junio C Hamano.
 */

#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#تعریف_کرو DISABLE_SIGN_COMPARE_WARNINGS

#شامل "builtin.h"

#شامل "abspath.h"
#شامل "advice.h"
#شامل "config.h"
#شامل "editor.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "hex.h"
#شامل "object-name.h"
#شامل "parse-options.h"
#شامل "lockfile.h"
#شامل "repository.h"
#شامل "run-command.h"
#شامل "hook.h"
#شامل "diff.h"
#شامل "diff-merges.h"
#شامل "refs.h"
#شامل "refspec.h"
#شامل "commit.h"
#شامل "diffcore.h"
#شامل "path.h"
#شامل "revision.h"
#شامل "unpack-trees.h"
#شامل "cache-tree.h"
#شامل "dir.h"
#شامل "color.h"
#شامل "rerere.h"
#شامل "help.h"
#شامل "merge.h"
#شامل "merge-ort-wrappers.h"
#شامل "resolve-undo.h"
#شامل "remote.h"
#شامل "fmt-merge-msg.h"
#شامل "sequencer.h"
#شامل "string-list.h"
#شامل "tag.h"
#شامل "alias.h"
#شامل "branch.h"
#شامل "commit-reach.h"
#شامل "wt-status.h"
#شامل "commit-graph.h"

#تعریف_کرو DEFAULT_TWOHEAD (1<<0)
#تعریف_کرو DEFAULT_OCTOPUS (1<<1)
#تعریف_کرو NO_FAST_FORWARD (1<<2)
#تعریف_کرو NO_TRIVIAL      (1<<3)

ساخت strategy {
	ثابت حرف *name;
	غیرعلامتی attr;
};

ساکن ثابت حرف * ثابت builtin_ملاؤ_usage[] = {
	N_("git merge [<options>] [<commit>...]"),
	"git merge --abort",
	"git merge --continue",
	تہی
};

#تعریف_کرو ملاؤ_SHOW_DIFFSTAT 1
#تعریف_کرو ملاؤ_SHOW_COMPACTSUMMARY 2

ساکن صحیح show_diffstat = ملاؤ_SHOW_DIFFSTAT, shortlog_len = -1, squash;
ساکن صحیح option_commit = -1;
ساکن صحیح option_edit = -1;
ساکن صحیح allow_trivial = 1, have_پیغام, verify_signatures;
ساکن صحیح check_trust_level = 1;
ساکن صحیح overwrite_ignore = 1;
ساکن ساخت strbuf ملاؤ_پیغام = STRBUF_INIT;
ساکن ساخت strategy **use_strategies;
ساکن سائز_t use_strategies_nr, use_strategies_alloc;
ساکن ساخت strvec xopts = STRVEC_INIT;
ساکن ثابت حرف *branch;
ساکن حرف *branch_mergeoptions;
ساکن صحیح verbosity;
ساکن صحیح allow_rerere_auto;
ساکن صحیح abort_current_ملاؤ;
ساکن صحیح quit_current_ملاؤ;
ساکن صحیح continue_current_ملاؤ;
ساکن صحیح allow_unrelated_histories;
ساکن صحیح show_پیش_رفت = -1;
ساکن صحیح default_مقصد_upstream = 1;
ساکن صحیح signoff;
ساکن ثابت حرف *sign_commit;
ساکن صحیح autostash;
ساکن صحیح no_verify;
ساکن حرف *into_name;

ساکن ساخت strategy all_strategy[] = {
	{ "recursive",  NO_TRIVIAL },
	{ "octopus",    DEFAULT_OCTOPUS },
	{ "ort",        DEFAULT_TWOHEAD | NO_TRIVIAL },
	{ "resolve",    0 },
	{ "ours",       NO_FAST_FORWARD | NO_TRIVIAL },
	{ "subtree",    NO_FAST_FORWARD | NO_TRIVIAL },
};

ساکن حرف *pull_twohead, *pull_octopus;

شمار ff_type {
	FF_NO,
	FF_ALLOW,
	FF_ONLY
};

ساکن شمار ff_type fast_forward = FF_ALLOW;

ساکن حرف *cleanup_arg;
ساکن شمار commit_پیغام_cleanup_منوال cleanup_منوال;

ساکن صحیح option_parse_پیغام(ثابت ساخت option *opt,
				ثابت حرف *arg, صحیح unset)
{
	ساخت strbuf *buf = opt->قدر;

	اگر (unset)
		strbuf_setlen(buf, 0);
	ورنہ اگر (arg) {
		strbuf_addf(buf, "%s%s", buf->len ? "\n\n" : "", arg);
		have_پیغام = 1;
	} ورنہ
		واپس error(_("switch `m' requires a value"));
	واپس 0;
}

ساکن شمار parse_opt_نتیجہ option_read_پیغام(ساخت parse_opt_ctx_t *ctx,
						 ثابت ساخت option *opt,
						 ثابت حرف *arg_not_مستعمل,
						 صحیح unset)
{
	ساخت strbuf *buf = opt->قدر;
	ثابت حرف *arg;

	BUG_ON_OPT_ARG(arg_not_مستعمل);
	اگر (unset)
		BUG("-F cannot be negated");

	اگر (ctx->opt) {
		arg = ctx->opt;
		ctx->opt = تہی;
	} ورنہ اگر (ctx->argc > 1) {
		ctx->argc--;
		arg = *++ctx->argv;
	} ورنہ
		واپس error(_("option `%s' requires a value"), opt->long_name);

	اگر (buf->len)
		strbuf_addch(buf, '\n');
	اگر (ctx->سابقہ && !is_absolute_راستہ(arg))
		arg = سابقہ_filename(ctx->سابقہ, arg);
	اگر (strbuf_read_file(buf, arg, 0) < 0)
		واپس error(_("could not read file '%s'"), arg);
	have_پیغام = 1;

	واپس 0;
}

ساکن ساخت strategy *get_strategy(ثابت حرف *name)
{
	صحیح i;
	ساخت strategy *ret;
	ساکن ساخت cmdnames main_cmds = {0}, other_cmds = {0};
	ساکن صحیح loaded;
	حرف *default_strategy = تہی;

	اگر (!name)
		واپس تہی;

	اگر (default_strategy &&
	    !strcmp(default_strategy, "ort") &&
	    !strcmp(name, "recursive")) {
		name = "ort";
	}

	برائے (i = 0; i < ARRAY_سائز(all_strategy); i++)
		اگر (!strcmp(name, all_strategy[i].name))
			واپس &all_strategy[i];

	اگر (!loaded) {
		ساخت cmdnames not_strategies = {0};
		loaded = 1;

		بھرو_command_list("git-merge-", &main_cmds, &other_cmds);
		برائے (i = 0; i < main_cmds.cnt; i++) {
			صحیح j, ملا_2 = 0;
			ساخت cmdname *ent = main_cmds.نام[i];
			برائے (j = 0; !ملا_2 && j < ARRAY_سائز(all_strategy); j++)
				اگر (!xstrncmpz(all_strategy[j].name, ent->name, ent->len))
					ملا_2 = 1;
			اگر (!ملا_2)
				جوڑو_cmdname(&not_strategies, ent->name, ent->len);
		}
		exclude_cmds(&main_cmds, &not_strategies);

		cmdnames_release(&not_strategies);
	}
	اگر (!is_اندر_cmdlist(&main_cmds, name) && !is_اندر_cmdlist(&other_cmds, name)) {
		fprintf(stderr, _("Could not find merge strategy '%s'.\n"), name);
		fprintf(stderr, _("Available strategies are:"));
		برائے (i = 0; i < main_cmds.cnt; i++)
			fprintf(stderr, " %s", main_cmds.نام[i]->name);
		fprintf(stderr, ".\n");
		اگر (other_cmds.cnt) {
			fprintf(stderr, _("Available custom strategies are:"));
			برائے (i = 0; i < other_cmds.cnt; i++)
				fprintf(stderr, " %s", other_cmds.نام[i]->name);
			fprintf(stderr, ".\n");
		}
		باہر_نکلو(1);
	}

	CALLOC_ARRAY(ret, 1);
	ret->name = xstrdup(name);
	ret->attr = NO_TRIVIAL;

	cmdnames_release(&main_cmds);
	cmdnames_release(&other_cmds);
	واپس ret;
}

ساکن خالی append_strategy(ساخت strategy *s)
{
	ALLOC_GROW(use_strategies, use_strategies_nr + 1, use_strategies_alloc);
	use_strategies[use_strategies_nr++] = s;
}

ساکن صحیح option_parse_strategy(ثابت ساخت option *opt غیر_مستعمل,
				 ثابت حرف *name, صحیح unset)
{
	اگر (unset)
		واپس 0;

	append_strategy(get_strategy(name));
	واپس 0;
}

ساکن صحیح option_parse_compact_summary(ثابت ساخت option *opt,
					ثابت حرف *name غیر_مستعمل, صحیح unset)
{
	صحیح *setting = opt->قدر;

	اگر (unset)
		*setting = 0;
	ورنہ
		*setting = ملاؤ_SHOW_COMPACTSUMMARY;
	واپس 0;
}

ساکن ساخت option builtin_ملاؤ_options[] = {
	OPT_سیٹ_INT('n', تہی, &show_diffstat,
		N_("do not show a diffstat at the end of the merge"), 0),
	OPT_BOOL(0, "stat", &show_diffstat,
		N_("show a diffstat at the end of the merge")),
	OPT_BOOL(0, "summary", &show_diffstat, N_("(synonym to --stat)")),
	OPT_CALLBACK_F(0, "compact-summary", &show_diffstat, تہی,
		       N_("show a compact-summary at the end of the merge"),
		       PARSE_OPT_NOARG,
		       option_parse_compact_summary),
	{
		.type = OPTION_INTEGER,
		.long_name = "log",
		.قدر = &shortlog_len,
		.precision = حجم(shortlog_len),
		.argh = N_("n"),
		.help = N_("add (at most <n>) entries from shortlog to merge commit message"),
		.flags = PARSE_OPT_OPTARG,
		.defval = DEFAULT_ملاؤ_LOG_LEN,
	},
	OPT_BOOL(0, "squash", &squash,
		N_("create a single commit instead of doing a merge")),
	OPT_BOOL(0, "commit", &option_commit,
		N_("perform a commit if the merge succeeds (default)")),
	OPT_BOOL('e', "edit", &option_edit,
		N_("edit message before committing")),
	OPT_CLEANUP(&cleanup_arg),
	OPT_سیٹ_INT(0, "ff", &fast_forward, N_("allow fast-forward (default)"), FF_ALLOW),
	OPT_سیٹ_INT_F(0, "ff-only", &fast_forward,
		      N_("abort if fast-forward is not possible"),
		      FF_ONLY, PARSE_OPT_NONEG),
	OPT_RERERE_AUTOUPDATE(&allow_rerere_auto),
	OPT_BOOL(0, "verify-signatures", &verify_signatures,
		N_("verify that the named commit has a valid GPG signature")),
	OPT_CALLBACK('s', "strategy", تہی, N_("strategy"),
		N_("merge strategy to use"), option_parse_strategy),
	OPT_STRVEC('X', "strategy-option", &xopts, N_("option=value"),
		N_("option for selected merge strategy")),
	OPT_CALLBACK('m', "message", &ملاؤ_پیغام, N_("message"),
		N_("merge commit message (for a non-fast-forward merge)"),
		option_parse_پیغام),
	{
		.type = OPTION_LOWLEVEL_CALLBACK,
		.short_name = 'F',
		.long_name = "file",
		.قدر = &ملاؤ_پیغام,
		.argh = N_("path"),
		.help = N_("read message from file"),
		.flags = PARSE_OPT_NONEG,
		.ll_callback = option_read_پیغام,
	},
	OPT_STRING(0, "into-name", &into_name, N_("name"),
		   N_("use <name> instead of the real target")),
	OPT__VERBOSITY(&verbosity),
	OPT_BOOL(0, "abort", &abort_current_ملاؤ,
		N_("abort the current in-progress merge")),
	OPT_BOOL(0, "quit", &quit_current_ملاؤ,
		N_("--abort but leave index and working tree alone")),
	OPT_BOOL(0, "continue", &continue_current_ملاؤ,
		N_("continue the current in-progress merge")),
	OPT_BOOL(0, "allow-unrelated-histories", &allow_unrelated_histories,
		 N_("allow merging unrelated histories")),
	OPT_سیٹ_INT(0, "progress", &show_پیش_رفت, N_("force progress reporting"), 1),
	{
		.type = OPTION_STRING,
		.short_name = 'S',
		.long_name = "gpg-sign",
		.قدر = &sign_commit,
		.argh = N_("key-id"),
		.help = N_("GPG sign commit"),
		.flags = PARSE_OPT_OPTARG,
		.defval = (intptr_t) "",
	},
	OPT_AUTOSTASH(&autostash),
	OPT_BOOL(0, "overwrite-ignore", &overwrite_ignore, N_("update ignored files (default)")),
	OPT_BOOL(0, "signoff", &signoff, N_("add a Signed-off-by trailer")),
	OPT_BOOL(0, "no-verify", &no_verify, N_("bypass pre-merge-commit and commit-msg hooks")),
	OPT_انتہا()
};

ساکن صحیح save_state(ساخت object_id *stash)
{
	صحیح len;
	ساخت بچہ_process cp = بچہ_PROCESS_INIT;
	ساخت strbuf بفر = STRBUF_INIT;
	ساخت تالا_file تالا_file = تالا_INIT;
	صحیح fd;
	صحیح rc = -1;

	fd = repo_hold_locked_اشاریہ(the_repository, &تالا_file, 0);
	refresh_اشاریہ(the_repository->اشاریہ, REFRESH_QUIET, تہی, تہی, تہی);
	اگر (0 <= fd)
		repo_update_اشاریہ_if_able(the_repository, &تالا_file);
	rollback_تالا_file(&تالا_file);

	strvec_pushl(&cp.دلائل, "stash", "create", تہی);
	cp.برآمد = -1;
	cp.git_cmd = 1;

	اگر (آغاز_command(&cp))
		die(_("could not run stash."));
	len = strbuf_read(&بفر, cp.برآمد, 1024);
	close(cp.برآمد);

	اگر (اختتام_command(&cp) || len < 0)
		die(_("stash failed"));
	ورنہ اگر (!len)		/* no changes */
		جاؤ برآمد;
	strbuf_setlen(&بفر, بفر.len-1);
	اگر (repo_get_oid(the_repository, بفر.buf, stash))
		die(_("not a valid object: %s"), بفر.buf);
	rc = 0;
برآمد:
	strbuf_release(&بفر);
	واپس rc;
}

ساکن خالی read_خالی_خانے(ثابت ساخت object_id *oid)
{
	ساخت بچہ_process cmd = بچہ_PROCESS_INIT;

	strvec_pushl(&cmd.دلائل, "read-tree", "-m", "-u",
		     خالی_خانے_درخت_oid_hex(the_repository->hash_algo),
		     oid_مقصد_hex(oid), تہی);
	cmd.git_cmd = 1;

	اگر (تسلسل_command(&cmd))
		die(_("read-tree failed"));
}

ساکن خالی reset_hard(ثابت ساخت object_id *oid)
{
	ساخت بچہ_process cmd = بچہ_PROCESS_INIT;

	strvec_pushl(&cmd.دلائل, "read-tree", "-v", "--reset", "-u",
		     oid_مقصد_hex(oid), تہی);
	cmd.git_cmd = 1;

	اگر (تسلسل_command(&cmd))
		die(_("read-tree failed"));
}

ساکن خالی restore_state(ثابت ساخت object_id *سر,
			  ثابت ساخت object_id *stash)
{
	ساخت بچہ_process cmd = بچہ_PROCESS_INIT;

	reset_hard(سر);

	اگر (is_null_oid(stash))
		جاؤ refresh_cache;

	strvec_pushl(&cmd.دلائل, "stash", "apply", "--index", "--quiet", تہی);
	strvec_ڈالو(&cmd.دلائل, oid_مقصد_hex(stash));

	/*
	 * It is OK to ignore error here, for example when there was
	 * nothing to restore.
	 */
	cmd.git_cmd = 1;
	تسلسل_command(&cmd);

refresh_cache:
	discard_اشاریہ(the_repository->اشاریہ);
	اگر (repo_read_اشاریہ(the_repository) < 0)
		die(_("could not read index"));
}

/* This is called when no merge was necessary. */
ساکن خالی اختتام_اوپر_مقصد_date(خالی)
{
	اگر (verbosity >= 0) {
		اگر (squash)
			puts(_("Already up to date. (nothing to squash)"));
		ورنہ
			puts(_("Already up to date."));
	}
	remove_ملاؤ_branch_state(the_repository);
}

ساکن خالی squash_پیغام(ساخت commit *commit, ساخت commit_list *remoteheads)
{
	ساخت rev_info rev;
	ساخت strbuf برآمد = STRBUF_INIT;
	ساخت commit_list *j;
	ساخت pretty_print_context ctx = {0};

	لکھو(_("Squash commit -- not updating HEAD\n"));

	repo_init_revisions(the_repository, &rev, تہی);
	diff_merges_suppress(&rev);
	rev.commit_format = CMIT_FMT_MEDIUM;

	commit->object.flags |= UNINTERESTING;
	جوڑو_pending_object(&rev, &commit->object, تہی);

	برائے (j = remoteheads; j; j = j->اگلا)
		جوڑو_pending_object(&rev, &j->شے->object, تہی);

	تیاری_revisions(0, تہی, &rev, تہی);
	اگر (prepare_revision_walk(&rev))
		die(_("revision walk setup failed"));

	ctx.abbrev = rev.abbrev;
	ctx.date_منوال = rev.date_منوال;
	ctx.fmt = rev.commit_format;

	strbuf_addstr(&برآمد, "Squashed commit of the following:\n");
	جب ((commit = get_revision(&rev)) != تہی) {
		strbuf_addch(&برآمد, '\n');
		strbuf_addf(&برآمد, "commit %s\n",
			oid_مقصد_hex(&commit->object.oid));
		pretty_print_commit(&ctx, commit, &برآمد);
	}
	write_file_buf(git_راستہ_squash_پیغام(the_repository), برآمد.buf, برآمد.len);
	strbuf_release(&برآمد);
	release_revisions(&rev);
}

ساکن خالی اختتام(ساخت commit *سر_commit,
		   ساخت commit_list *remoteheads,
		   ثابت ساخت object_id *new_سر, ثابت حرف *پیغام)
{
	ساخت strbuf reflog_پیغام_2 = STRBUF_INIT;
	ثابت ساخت object_id *سر = &سر_commit->object.oid;

	اگر (!پیغام)
		strbuf_addstr(&reflog_پیغام_2, getenv("GIT_REFLOG_ACTION"));
	ورنہ {
		اگر (verbosity >= 0)
			لکھو("%s\n", پیغام);
		strbuf_addf(&reflog_پیغام_2, "%s: %s",
			getenv("GIT_REFLOG_ACTION"), پیغام);
	}
	اگر (squash) {
		squash_پیغام(سر_commit, remoteheads);
	} ورنہ {
		اگر (verbosity >= 0 && !ملاؤ_پیغام.len)
			لکھو(_("No merge message -- not updating HEAD\n"));
		ورنہ {
			refs_update_ref(get_main_ref_store(the_repository),
					reflog_پیغام_2.buf, "HEAD", new_سر,
					سر,
					0, UPDATE_REFS_DIE_ON_ERR);
			/*
			 * We ignore errors in 'gc --auto', since the
			 * user should see them.
			 */
			تسلسل_auto_maintenance(the_repository, verbosity < 0);
		}
	}
	اگر (new_سر && show_diffstat) {
		ساخت diff_options opts;
		repo_diff_تیاری(the_repository, &opts);
		init_diffstat_widths(&opts);

		انتخاب (show_diffstat) {
		حالت ملاؤ_SHOW_DIFFSTAT: /* 1 */
			opts.output_format |=
				DIFF_FORMAT_SUMMARY | DIFF_FORMAT_DIFFSTAT;
			توڑو;
		حالت ملاؤ_SHOW_COMPACTSUMMARY: /* 2 */
			opts.output_format |= DIFF_FORMAT_DIFFSTAT;
			opts.flags.stat_with_summary = 1;
			توڑو;
		طےشدہ:
			توڑو;
		}
		opts.detect_rename = DIFF_DETECT_RENAME;
		diff_تیاری_مکمل(&opts);
		diff_درخت_oid(سر, new_سر, "", &opts);
		diffcore_std(&opts);
		diff_flush(&opts);
	}

	/* Run a post-merge hook */
	تسلسل_hooks_l(the_repository, "post-merge", squash ? "1" : "0", تہی);

	اگر (new_سر)
		apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
				    تہی, تہی, تہی, تہی);
	strbuf_release(&reflog_پیغام_2);
}

/* Get the name for the merge commit's message. */
ساکن خالی ملاؤ_name(ثابت حرف *remote, ساخت strbuf *پیغام)
{
	ساخت commit *remote_سر;
	ساخت object_id branch_سر;
	ساخت strbuf bname = STRBUF_INIT;
	ساخت ملاؤ_remote_desc *desc;
	ثابت حرف *ptr;
	حرف *ملا_ref = تہی;
	صحیح len, early;

	copy_branchname(the_repository, &bname, remote, 0);
	remote = bname.buf;

	oidclr(&branch_سر, the_repository->hash_algo);
	remote_سر = get_ملاؤ_والد(remote);
	اگر (!remote_سر)
		die(_("'%s' does not point to a commit"), remote);

	اگر (repo_dwim_ref(the_repository, remote, لڑی_لمبائی(remote), &branch_سر,
			  &ملا_ref, 0) > 0) {
		اگر (starts_with(ملا_ref, "refs/heads/")) {
			strbuf_addf(پیغام, "%s\t\tbranch '%s' of .\n",
				    oid_مقصد_hex(&branch_سر), remote);
			جاؤ cleanup;
		}
		اگر (starts_with(ملا_ref, "refs/tags/")) {
			strbuf_addf(پیغام, "%s\t\ttag '%s' of .\n",
				    oid_مقصد_hex(&branch_سر), remote);
			جاؤ cleanup;
		}
		اگر (starts_with(ملا_ref, "refs/remotes/")) {
			strbuf_addf(پیغام, "%s\t\tremote-tracking branch '%s' of .\n",
				    oid_مقصد_hex(&branch_سر), remote);
			جاؤ cleanup;
		}
	}

	/* See if remote matches <name>^^^.. or <name>~<number> */
	برائے (len = 0, ptr = remote + لڑی_لمبائی(remote);
	     remote < ptr && ptr[-1] == '^';
	     ptr--)
		len++;
	اگر (len)
		early = 1;
	ورنہ {
		early = 0;
		ptr = strrchr(remote, '~');
		اگر (ptr) {
			صحیح نظر_آیا_nonzero = 0;

			len++; /* count ~ */
			جب (*++ptr && ہندسہ_ہے(*ptr)) {
				نظر_آیا_nonzero |= (*ptr != '0');
				len++;
			}
			اگر (*ptr)
				len = 0; /* not ...~<number> */
			ورنہ اگر (نظر_آیا_nonzero)
				early = 1;
			ورنہ اگر (len == 1)
				early = 1; /* "name~" is "name~1"! */
		}
	}
	اگر (len) {
		ساخت strbuf truname = STRBUF_INIT;
		strbuf_addf(&truname, "refs/heads/%s", remote);
		strbuf_setlen(&truname, truname.len - len);
		اگر (refs_ref_exists(get_main_ref_store(the_repository), truname.buf)) {
			strbuf_addf(پیغام,
				    "%s\t\tbranch '%s'%s of .\n",
				    oid_مقصد_hex(&remote_سر->object.oid),
				    truname.buf + 11,
				    (early ? " (early part)" : ""));
			strbuf_release(&truname);
			جاؤ cleanup;
		}
		strbuf_release(&truname);
	}

	desc = ملاؤ_remote_util(remote_سر);
	اگر (desc && desc->obj && desc->obj->type == OBJ_لیبل) {
		strbuf_addf(پیغام, "%s\t\t%s '%s'\n",
			    oid_مقصد_hex(&desc->obj->oid),
			    type_name(desc->obj->type),
			    remote);
		جاؤ cleanup;
	}

	strbuf_addf(پیغام, "%s\t\tcommit '%s'\n",
		oid_مقصد_hex(&remote_سر->object.oid), remote);
cleanup:
	آزاد(ملا_ref);
	strbuf_release(&bname);
}

ساکن خالی parse_branch_ملاؤ_options(حرف *bmo)
{
	ثابت حرف **argv;
	صحیح argc;

	اگر (!bmo)
		واپس;
	argc = بانٹو_cmdline(bmo, &argv);
	اگر (argc < 0)
		die(_("Bad branch.%s.mergeoptions string: %s"), branch,
		    _(بانٹو_cmdline_strerror(argc)));
	REALLOC_ARRAY(argv, argc + 2);
	MOVE_ARRAY(argv + 1, argv, argc + 1);
	argc++;
	argv[0] = "branch.*.mergeoptions";
	parse_options(argc, argv, تہی, builtin_ملاؤ_options,
		      builtin_ملاؤ_usage, 0);
	آزاد(argv);
}

ساکن صحیح git_ملاؤ_config(ثابت حرف *k, ثابت حرف *v,
			    ثابت ساخت config_context *ctx, خالی *cb)
{
	صحیح حالت_2;
	ثابت حرف *str;

	اگر (branch &&
	    skip_سابقہ(k, "branch.", &str) &&
	    skip_سابقہ(str, branch, &str) &&
	    !strcmp(str, ".mergeoptions")) {
		آزاد(branch_mergeoptions);
		branch_mergeoptions = xstrdup(v);
		واپس 0;
	}

	اگر (!strcmp(k, "merge.diffstat") || !strcmp(k, "merge.stat")) {
		صحیح val = git_parse_maybe_bool_متن(v);
		انتخاب (val) {
		حالت 0:
			show_diffstat = 0;
			توڑو;
		حالت 1:
			show_diffstat = ملاؤ_SHOW_DIFFSTAT;
			توڑو;
		طےشدہ:
			اگر (!strcmp(v, "compact"))
				show_diffstat = ملاؤ_SHOW_COMPACTSUMMARY;
			/*
			 * We do not need to have an explicit
			 *
			 * else if (!strcmp(v, "diffstat"))
			 *	show_diffstat = MERGE_SHOW_DIFFSTAT;
			 *
			 * here, because the catch-all uses the
			 * diffstat style anyway.
			 */
			ورنہ
				/*
				 * A setting from a future?  It is not an
				 * error grave enough to fail the command.
				 * proceed using the default one.
				 */
				show_diffstat = ملاؤ_SHOW_DIFFSTAT;
			توڑو;
		}
	} ورنہ اگر (!strcmp(k, "merge.verifysignatures")) {
		verify_signatures = git_config_bool(k, v);
	} ورنہ اگر (!strcmp(k, "pull.twohead")) {
		FREE_AND_NULL(pull_twohead);
		واپس git_config_string(&pull_twohead, k, v);
	} ورنہ اگر (!strcmp(k, "pull.octopus")) {
		FREE_AND_NULL(pull_octopus);
		واپس git_config_string(&pull_octopus, k, v);
	} ورنہ اگر (!strcmp(k, "commit.cleanup")) {
		واپس git_config_string(&cleanup_arg, k, v);
	} ورنہ اگر (!strcmp(k, "merge.ff")) {
		صحیح boolval = git_parse_maybe_bool(v);
		اگر (0 <= boolval) {
			fast_forward = boolval ? FF_ALLOW : FF_NO;
		} ورنہ اگر (v && !strcmp(v, "only")) {
			fast_forward = FF_ONLY;
		} /* do not barf on values from future versions of git */
		واپس 0;
	} ورنہ اگر (!strcmp(k, "merge.defaulttoupstream")) {
		default_مقصد_upstream = git_config_bool(k, v);
		واپس 0;
	} ورنہ اگر (!strcmp(k, "commit.gpgsign")) {
		sign_commit = git_config_bool(k, v) ? "" : تہی;
		واپس 0;
	} ورنہ اگر (!strcmp(k, "gpg.mintrustlevel")) {
		check_trust_level = 0;
	} ورنہ اگر (!strcmp(k, "merge.autostash")) {
		autostash = git_config_bool(k, v);
		واپس 0;
	}

	حالت_2 = fmt_ملاؤ_پیغام_config(k, v, ctx, cb);
	اگر (حالت_2)
		واپس حالت_2;
	واپس git_diff_ui_config(k, v, ctx, cb);
}

ساکن صحیح read_درخت_trivial(ساخت object_id *common, ساخت object_id *سر,
			     ساخت object_id *one)
{
	صحیح i, nr_trees = 0;
	ساخت درخت *trees[MAX_UNPACK_TREES];
	ساخت درخت_desc t[MAX_UNPACK_TREES];
	ساخت unpack_trees_options opts;

	memset(&opts, 0, حجم(opts));
	opts.سر_idx = 2;
	opts.src_اشاریہ = the_repository->اشاریہ;
	opts.dst_اشاریہ = the_repository->اشاریہ;
	opts.update = 1;
	opts.verbose_update = 1;
	opts.trivial_merges_only = 1;
	opts.ملاؤ = 1;
	opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
	trees[nr_trees] = repo_parse_درخت_بالواسطہ(the_repository, common);
	اگر (!trees[nr_trees++])
		واپس -1;
	trees[nr_trees] = repo_parse_درخت_بالواسطہ(the_repository, سر);
	اگر (!trees[nr_trees++])
		واپس -1;
	trees[nr_trees] = repo_parse_درخت_بالواسطہ(the_repository, one);
	اگر (!trees[nr_trees++])
		واپس -1;
	opts.fn = threeway_ملاؤ;
	cache_درخت_free(&the_repository->اشاریہ->cache_درخت);
	برائے (i = 0; i < nr_trees; i++) {
		repo_parse_درخت(the_repository, trees[i]);
		init_درخت_desc(t+i, &trees[i]->object.oid,
			       trees[i]->بفر, trees[i]->سائز);
	}
	اگر (unpack_trees(nr_trees, t, &opts))
		واپس -1;
	واپس 0;
}

ساکن خالی write_درخت_trivial(ساخت object_id *oid)
{
	اگر (write_اشاریہ_as_درخت(oid, the_repository->اشاریہ,
				repo_get_اشاریہ_file(the_repository),
				0, تہی))
		die(_("git write-tree failed to write a tree"));
}

ساکن صحیح try_ملاؤ_strategy(ثابت حرف *strategy, ساخت commit_list *common,
			      ساخت commit_list *remoteheads,
			      ساخت commit *سر)
{
	ثابت حرف *سر_arg = "HEAD";

	اگر (repo_refresh_and_write_اشاریہ(the_repository, REFRESH_QUIET,
					 SKIP_IF_UNCHANGED, 0, تہی, تہی,
					 تہی) < 0)
		die(_("Unable to write index."));

	اگر (!strcmp(strategy, "recursive") || !strcmp(strategy, "subtree") ||
	    !strcmp(strategy, "ort")) {
		ساخت تالا_file تالا = تالا_INIT;
		صحیح clean, x;
		ساخت commit *نتیجہ;
		ساخت commit_list *reversed = تہی;
		ساخت ملاؤ_options o;
		ساخت commit_list *j;

		اگر (remoteheads->اگلا) {
			error(_("Not handling anything other than two heads merge."));
			واپس 2;
		}

		init_ui_ملاؤ_options(&o, the_repository);
		اگر (!strcmp(strategy, "subtree"))
			o.subtree_سرکاؤ = "";

		o.show_rename_پیش_رفت =
			show_پیش_رفت == -1 ? isatty(2) : show_پیش_رفت;

		برائے (x = 0; x < xopts.nr; x++)
			اگر (parse_ملاؤ_opt(&o, xopts.v[x]))
				die(_("unknown strategy option: -X%s"), xopts.v[x]);

		o.branch1 = سر_arg;
		o.branch2 = ملاؤ_remote_util(remoteheads->شے)->name;

		برائے (j = common; j; j = j->اگلا)
			commit_list_داخل_کرو(j->شے, &reversed);

		repo_hold_locked_اشاریہ(the_repository, &تالا,
				       تالا_DIE_ON_ERROR);
		clean = ملاؤ_ort_recursive(&o, سر, remoteheads->شے,
					    reversed, &نتیجہ);
		commit_list_free(reversed);
		strbuf_release(&o.obuf);

		اگر (clean < 0) {
			rollback_تالا_file(&تالا);
			واپس 2;
		}
		اگر (write_locked_اشاریہ(the_repository->اشاریہ, &تالا,
				       COMMIT_تالا | SKIP_IF_UNCHANGED))
			die(_("unable to write %s"), repo_get_اشاریہ_file(the_repository));
		واپس clean ? 0 : 1;
	} ورنہ {
		واپس try_ملاؤ_command(the_repository,
					 strategy, xopts.nr, xopts.v,
					 common, سر_arg, remoteheads);
	}
}

ساکن خالی گنتی_diff_files(ساخت diff_قطار_struct *q,
			     ساخت diff_options *opt غیر_مستعمل, خالی *ڈیٹا)
{
	صحیح *گنتی = ڈیٹا;

	(*گنتی) += q->nr;
}

ساکن صحیح گنتی_unmerged_entries(خالی)
{
	صحیح i, ret = 0;

	برائے (i = 0; i < the_repository->اشاریہ->cache_nr; i++)
		اگر (ce_stage(the_repository->اشاریہ->cache[i]))
			ret++;

	واپس ret;
}

ساکن خالی جوڑو_strategies(ثابت حرف *string, غیرعلامتی attr)
{
	صحیح i;

	اگر (string) {
		ساخت string_list list = STRING_LIST_INIT_DUP;
		ساخت string_list_شے *شے;
		string_list_بانٹو(&list, string, " ", -1);
		for_each_string_list_شے(شے, &list)
			append_strategy(get_strategy(شے->string));
		string_list_clear(&list, 0);
		واپس;
	}
	برائے (i = 0; i < ARRAY_سائز(all_strategy); i++)
		اگر (all_strategy[i].attr & attr)
			append_strategy(&all_strategy[i]);

}

ساکن خالی read_ملاؤ_پیغام(ساخت strbuf *پیغام)
{
	ثابت حرف *filename = git_راستہ_ملاؤ_پیغام(the_repository);
	strbuf_reset(پیغام);
	اگر (strbuf_read_file(پیغام, filename, 0) < 0)
		die_errno(_("Could not read from '%s'"), filename);
}

ساکن خالی write_ملاؤ_state(ساخت commit_list *);
ساکن خالی abort_commit(ساخت commit_list *remoteheads, ثابت حرف *err_پیغام)
{
	اگر (err_پیغام)
		error("%s", err_پیغام);
	fprintf(stderr,
		_("Not committing merge; use 'git commit' to complete the merge.\n"));
	write_ملاؤ_state(remoteheads);
	باہر_نکلو(1);
}

ساکن ثابت حرف ملاؤ_editor_comment[] =
N_("Please enter a commit message to explain why this merge is necessary,\n"
   "especially if it merges an updated upstream into a topic branch.\n"
   "\n");

ساکن ثابت حرف scissors_editor_comment[] =
N_("An empty message aborts the commit.\n");

ساکن ثابت حرف no_scissors_editor_comment[] =
N_("Lines starting with '%s' will be ignored, and an empty message aborts\n"
   "the commit.\n");

ساکن خالی write_ملاؤ_heads(ساخت commit_list *);
ساکن خالی prepare_مقصد_commit(ساخت commit_list *remoteheads)
{
	ساخت strbuf پیغام = STRBUF_INIT;
	ثابت حرف *اشاریہ_file = repo_get_اشاریہ_file(the_repository);

	اگر (!no_verify) {
		صحیح invoked_hook;

		اگر (تسلسل_commit_hook(0 < option_edit, اشاریہ_file, &invoked_hook,
				    "pre-merge-commit", تہی))
			abort_commit(remoteheads, تہی);
		/*
		 * Re-read the index as pre-merge-commit hook could have updated it,
		 * and write it out as a tree.  We must do this before we invoke
		 * the editor and after we invoke run_status above.
		 */
		اگر (invoked_hook)
			discard_اشاریہ(the_repository->اشاریہ);
	}
	read_اشاریہ_from(the_repository->اشاریہ, اشاریہ_file,
			repo_get_git_dir(the_repository));
	strbuf_addbuf(&پیغام, &ملاؤ_پیغام);
	اگر (squash)
		BUG("the control must not reach here under --squash");
	اگر (0 < option_edit) {
		strbuf_addch(&پیغام, '\n');
		اگر (cleanup_منوال == COMMIT_پیغام_CLEANUP_SCISSORS) {
			wt_حالت_append_cut_سطر(&پیغام);
			strbuf_commented_addf(&پیغام, comment_سطر_str, "\n");
		}
		strbuf_commented_addf(&پیغام, comment_سطر_str,
				      _(ملاؤ_editor_comment));
		اگر (cleanup_منوال == COMMIT_پیغام_CLEANUP_SCISSORS)
			strbuf_commented_addf(&پیغام, comment_سطر_str,
					      _(scissors_editor_comment));
		ورنہ
			strbuf_commented_addf(&پیغام, comment_سطر_str,
				_(no_scissors_editor_comment), comment_سطر_str);
	}
	اگر (signoff)
		append_signoff(&پیغام, ignored_log_پیغام_بائٹیں(پیغام.buf, پیغام.len), 0);
	write_ملاؤ_heads(remoteheads);
	write_file_buf(git_راستہ_ملاؤ_پیغام(the_repository), پیغام.buf, پیغام.len);
	اگر (تسلسل_commit_hook(0 < option_edit, repo_get_اشاریہ_file(the_repository),
			    تہی, "prepare-commit-msg",
			    git_راستہ_ملاؤ_پیغام(the_repository), "merge", تہی))
		abort_commit(remoteheads, تہی);
	اگر (0 < option_edit) {
		اگر (launch_editor(git_راستہ_ملاؤ_پیغام(the_repository), تہی, تہی))
			abort_commit(remoteheads, تہی);
	}

	اگر (!no_verify && تسلسل_commit_hook(0 < option_edit, repo_get_اشاریہ_file(the_repository),
					  تہی, "commit-msg",
					  git_راستہ_ملاؤ_پیغام(the_repository), تہی))
		abort_commit(remoteheads, تہی);

	read_ملاؤ_پیغام(&پیغام);
	cleanup_پیغام(&پیغام, cleanup_منوال, 0);
	اگر (!پیغام.len)
		abort_commit(remoteheads, _("Empty commit message."));
	strbuf_release(&ملاؤ_پیغام);
	strbuf_addbuf(&ملاؤ_پیغام, &پیغام);
	strbuf_release(&پیغام);
}

ساکن صحیح ملاؤ_trivial(ساخت commit *سر, ساخت commit_list *remoteheads)
{
	ساخت object_id نتیجہ_درخت, نتیجہ_commit;
	ساخت commit_list *parents = تہی, **pptr = &parents;

	اگر (repo_refresh_and_write_اشاریہ(the_repository, REFRESH_QUIET,
					 SKIP_IF_UNCHANGED, 0, تہی, تہی,
					 تہی) < 0)
		واپس error(_("Unable to write index."));

	write_درخت_trivial(&نتیجہ_درخت);
	لکھو(_("Wonderful.\n"));
	pptr = commit_list_append(سر, pptr);
	pptr = commit_list_append(remoteheads->شے, pptr);
	prepare_مقصد_commit(remoteheads);
	اگر (commit_درخت(ملاؤ_پیغام.buf, ملاؤ_پیغام.len, &نتیجہ_درخت, parents,
			&نتیجہ_commit, تہی, sign_commit))
		die(_("failed to write commit object"));
	اختتام(سر, remoteheads, &نتیجہ_commit, "In-index merge");

	remove_ملاؤ_branch_state(the_repository);
	commit_list_free(parents);
	واپس 0;
}

ساکن صحیح اختتام_automerge(ساخت commit *سر,
			    صحیح سر_subsumed,
			    ساخت commit_list *common,
			    ساخت commit_list *remoteheads,
			    ساخت object_id *نتیجہ_درخت,
			    ثابت حرف *wt_strategy)
{
	ساخت commit_list *parents = تہی;
	ساخت strbuf buf = STRBUF_INIT;
	ساخت object_id نتیجہ_commit;

	write_درخت_trivial(نتیجہ_درخت);
	commit_list_free(common);
	parents = remoteheads;
	اگر (!سر_subsumed || fast_forward == FF_NO)
		commit_list_داخل_کرو(سر, &parents);
	prepare_مقصد_commit(remoteheads);
	اگر (commit_درخت(ملاؤ_پیغام.buf, ملاؤ_پیغام.len, نتیجہ_درخت, parents,
			&نتیجہ_commit, تہی, sign_commit))
		die(_("failed to write commit object"));
	strbuf_addf(&buf, "Merge made by the '%s' strategy.", wt_strategy);
	اختتام(سر, remoteheads, &نتیجہ_commit, buf.buf);

	strbuf_release(&buf);
	remove_ملاؤ_branch_state(the_repository);
	commit_list_free(parents);
	واپس 0;
}

ساکن صحیح suggest_conflicts(خالی)
{
	ثابت حرف *filename;
	FILE *fp;
	ساخت strbuf msgbuf = STRBUF_INIT;

	filename = git_راستہ_ملاؤ_پیغام(the_repository);
	fp = xfopen(filename, "a");

	/*
	 * We can't use cleanup_mode because if we're not using the editor,
	 * get_cleanup_mode will return COMMIT_MSG_CLEANUP_SPACE instead, even
	 * though the message is meant to be processed later by git-commit.
	 * Thus, we will get the cleanup mode which is returned when we _are_
	 * using an editor.
	 */
	append_conflicts_hint(the_repository->اشاریہ, &msgbuf,
			      get_cleanup_منوال(cleanup_arg, 1));
	fputs(msgbuf.buf, fp);
	strbuf_release(&msgbuf);
	fclose(fp);
	repo_rerere(the_repository, allow_rerere_auto);
	لکھو(_("Automatic merge failed; "
			"fix conflicts and then commit the result.\n"));
	واپس 1;
}

ساکن صحیح evaluate_نتیجہ(خالی)
{
	صحیح cnt = 0;
	ساخت rev_info rev;

	/* Check how many files differ. */
	repo_init_revisions(the_repository, &rev, "");
	تیاری_revisions(0, تہی, &rev, تہی);
	rev.diffopt.output_format |=
		DIFF_FORMAT_CALLBACK;
	rev.diffopt.format_callback = گنتی_diff_files;
	rev.diffopt.format_callback_ڈیٹا = &cnt;
	تسلسل_diff_files(&rev, 0);

	/*
	 * Check how many unmerged entries are
	 * there.
	 */
	cnt += گنتی_unmerged_entries();

	release_revisions(&rev);
	واپس cnt;
}

/*
 * Pretend as if the user told us to merge with the remote-tracking
 * branch we have for the upstream of the current branch
 */
ساکن صحیح تیاری_with_upstream(ثابت حرف ***argv)
{
	ساخت branch *branch = branch_get(تہی);
	صحیح i;
	ثابت حرف **دلائل;

	اگر (!branch)
		die(_("No current branch."));
	اگر (!branch->remote_name)
		die(_("No remote for the current branch."));
	اگر (!branch->ملاؤ_nr)
		die(_("No default upstream defined for the current branch."));

	دلائل = xcalloc(st_جوڑو(branch->ملاؤ_nr, 1), حجم(حرف *));
	برائے (i = 0; i < branch->ملاؤ_nr; i++) {
		اگر (!branch->ملاؤ[i]->dst)
			die(_("No remote-tracking branch for %s from %s"),
			    branch->ملاؤ[i]->src, branch->remote_name);
		دلائل[i] = branch->ملاؤ[i]->dst;
	}
	دلائل[i] = تہی;
	*argv = دلائل;
	واپس i;
}

ساکن خالی write_ملاؤ_heads(ساخت commit_list *remoteheads)
{
	ساخت commit_list *j;
	ساخت strbuf buf = STRBUF_INIT;

	برائے (j = remoteheads; j; j = j->اگلا) {
		ساخت object_id *oid;
		ساخت commit *c = j->شے;
		ساخت ملاؤ_remote_desc *desc;

		desc = ملاؤ_remote_util(c);
		اگر (desc && desc->obj) {
			oid = &desc->obj->oid;
		} ورنہ {
			oid = &c->object.oid;
		}
		strbuf_addf(&buf, "%s\n", oid_مقصد_hex(oid));
	}
	write_file_buf(git_راستہ_ملاؤ_سر(the_repository), buf.buf, buf.len);

	strbuf_reset(&buf);
	اگر (fast_forward == FF_NO)
		strbuf_addstr(&buf, "no-ff");
	write_file_buf(git_راستہ_ملاؤ_منوال(the_repository), buf.buf, buf.len);
	strbuf_release(&buf);
}

ساکن خالی write_ملاؤ_state(ساخت commit_list *remoteheads)
{
	write_ملاؤ_heads(remoteheads);
	strbuf_addch(&ملاؤ_پیغام, '\n');
	write_file_buf(git_راستہ_ملاؤ_پیغام(the_repository), ملاؤ_پیغام.buf,
		       ملاؤ_پیغام.len);
}

ساکن صحیح default_edit_option(خالی)
{
	ساکن ثابت حرف name[] = "GIT_MERGE_AUTOEDIT";
	ثابت حرف *e = getenv(name);
	ساخت stat st_stdin, st_stdout;

	اگر (have_پیغام)
		/* an explicit -m msg without --[no-]edit */
		واپس 0;

	اگر (e) {
		صحیح v = git_parse_maybe_bool(e);
		اگر (v < 0)
			die(_("Bad value '%s' in environment '%s'"), e, name);
		واپس v;
	}

	/* Use editor if stdin and stdout are the same and is a tty */
	واپس (!fstat(0, &st_stdin) &&
		!fstat(1, &st_stdout) &&
		isatty(0) && isatty(1) &&
		st_stdin.st_dev == st_stdout.st_dev &&
		st_stdin.st_ino == st_stdout.st_ino &&
		st_stdin.st_منوال == st_stdout.st_منوال);
}

ساکن ساخت commit_list *reduce_parents(ساخت commit *سر_commit,
					  صحیح *سر_subsumed,
					  ساخت commit_list *remoteheads)
{
	ساخت commit_list *parents, **remotes;

	/*
	 * Is the current HEAD reachable from another commit being
	 * merged?  If so we do not want to record it as a parent of
	 * the resulting merge, unless --no-ff is given.  We will flip
	 * this variable to 0 when we find HEAD among the independent
	 * tips being merged.
	 */
	*سر_subsumed = 1;

	/* Find what parents to record by checking independent ones. */
	parents = reduce_heads(remoteheads);
	commit_list_free(remoteheads);

	remoteheads = تہی;
	remotes = &remoteheads;
	جب (parents) {
		ساخت commit *commit = نکالو_commit(&parents);
		اگر (commit == سر_commit)
			*سر_subsumed = 0;
		ورنہ
			remotes = &commit_list_داخل_کرو(commit, remotes)->اگلا;
	}
	واپس remoteheads;
}

ساکن خالی prepare_ملاؤ_پیغام(ساخت strbuf *ملاؤ_نام, ساخت strbuf *ملاؤ_پیغام)
{
	ساخت fmt_ملاؤ_پیغام_opts opts;

	memset(&opts, 0, حجم(opts));
	opts.جوڑو_title = !have_پیغام;
	opts.shortlog_len = shortlog_len;
	opts.credit_people = (0 < option_edit);
	opts.into_name = into_name;

	fmt_ملاؤ_پیغام(ملاؤ_نام, ملاؤ_پیغام, &opts);
	اگر (ملاؤ_پیغام->len)
		strbuf_setlen(ملاؤ_پیغام, ملاؤ_پیغام->len - 1);
}

ساکن خالی handle_fetch_سر(ساخت commit_list **remotes, ساخت strbuf *ملاؤ_نام)
{
	ثابت حرف *filename;
	صحیح fd, pos, npos;
	ساخت strbuf fetch_سر_file = STRBUF_INIT;
	ثابت غیرعلامتی hexsz = the_hash_algo->hexsz;

	اگر (!ملاؤ_نام)
		ملاؤ_نام = &fetch_سر_file;

	filename = git_راستہ_fetch_سر(the_repository);
	fd = xopen(filename, O_RDONLY);

	اگر (strbuf_read(ملاؤ_نام, fd, 0) < 0)
		die_errno(_("could not read '%s'"), filename);
	اگر (close(fd) < 0)
		die_errno(_("could not close '%s'"), filename);

	برائے (pos = 0; pos < ملاؤ_نام->len; pos = npos) {
		ساخت object_id oid;
		حرف *ptr;
		ساخت commit *commit;

		ptr = strchr(ملاؤ_نام->buf + pos, '\n');
		اگر (ptr)
			npos = ptr - ملاؤ_نام->buf + 1;
		ورنہ
			npos = ملاؤ_نام->len;

		اگر (npos - pos < hexsz + 2 ||
		    get_oid_hex(ملاؤ_نام->buf + pos, &oid))
			commit = تہی; /* bad */
		ورنہ اگر (memcmp(ملاؤ_نام->buf + pos + hexsz, "\t\t", 2))
			جاری; /* not-for-merge */
		ورنہ {
			حرف saved = ملاؤ_نام->buf[pos + hexsz];
			ملاؤ_نام->buf[pos + hexsz] = '\0';
			commit = get_ملاؤ_والد(ملاؤ_نام->buf + pos);
			ملاؤ_نام->buf[pos + hexsz] = saved;
		}
		اگر (!commit) {
			اگر (ptr)
				*ptr = '\0';
			die(_("not something we can merge in %s: %s"),
			    filename, ملاؤ_نام->buf + pos);
		}
		remotes = &commit_list_داخل_کرو(commit, remotes)->اگلا;
	}

	اگر (ملاؤ_نام == &fetch_سر_file)
		strbuf_release(&fetch_سر_file);
}

ساکن ساخت commit_list *collect_parents(ساخت commit *سر_commit,
					   صحیح *سر_subsumed,
					   صحیح argc, ثابت حرف **argv,
					   ساخت strbuf *ملاؤ_پیغام)
{
	صحیح i;
	ساخت commit_list *remoteheads = تہی;
	ساخت commit_list **remotes = &remoteheads;
	ساخت strbuf ملاؤ_نام = STRBUF_INIT, *autogen = تہی;

	اگر (ملاؤ_پیغام && (!have_پیغام || shortlog_len))
		autogen = &ملاؤ_نام;

	اگر (سر_commit)
		remotes = &commit_list_داخل_کرو(سر_commit, remotes)->اگلا;

	اگر (argc == 1 && !strcmp(argv[0], "FETCH_HEAD")) {
		handle_fetch_سر(remotes, autogen);
		remoteheads = reduce_parents(سر_commit, سر_subsumed, remoteheads);
	} ورنہ {
		برائے (i = 0; i < argc; i++) {
			ساخت commit *commit = get_ملاؤ_والد(argv[i]);
			اگر (!commit)
				help_unknown_ref(argv[i], "merge",
						 _("not something we can merge"));
			remotes = &commit_list_داخل_کرو(commit, remotes)->اگلا;
		}
		remoteheads = reduce_parents(سر_commit, سر_subsumed, remoteheads);
		اگر (autogen) {
			ساخت commit_list *p;
			برائے (p = remoteheads; p; p = p->اگلا)
				ملاؤ_name(ملاؤ_remote_util(p->شے)->name, autogen);
		}
	}

	اگر (autogen) {
		prepare_ملاؤ_پیغام(autogen, ملاؤ_پیغام);
		strbuf_release(autogen);
	}

	واپس remoteheads;
}

ساکن صحیح merging_a_throwaway_لیبل(ساخت commit *commit)
{
	حرف *لیبل_ref;
	ساخت object_id oid;
	صحیح is_throwaway_لیبل = 0;

	/* Are we merging a tag? */
	اگر (!ملاؤ_remote_util(commit) ||
	    !ملاؤ_remote_util(commit)->obj ||
	    ملاؤ_remote_util(commit)->obj->type != OBJ_لیبل)
		واپس is_throwaway_لیبل;

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
	لیبل_ref = xstrfmt("refs/tags/%s",
			  ((ساخت لیبل *)ملاؤ_remote_util(commit)->obj)->لیبل);
	اگر (!refs_read_ref(get_main_ref_store(the_repository), لیبل_ref, &oid) &&
	    oideq(&oid, &ملاؤ_remote_util(commit)->obj->oid))
		is_throwaway_لیبل = 0;
	ورنہ
		is_throwaway_لیبل = 1;
	آزاد(لیبل_ref);
	واپس is_throwaway_لیبل;
}

صحیح cmd_ملاؤ(صحیح argc,
	      ثابت حرف **argv,
	      ثابت حرف *سابقہ,
	      ساخت repository *repo غیر_مستعمل)
{
	ساخت object_id نتیجہ_درخت, stash, سر_oid;
	ساخت commit *سر_commit;
	ساخت strbuf buf = STRBUF_INIT;
	صحیح i, ret = 0, سر_subsumed;
	صحیح بہترین_cnt = -1, ملاؤ_was_ok = 0, automerge_was_ok = 0;
	ساخت commit_list *common = تہی;
	ثابت حرف *بہترین_strategy = تہی, *wt_strategy = تہی;
	ساخت commit_list *remoteheads = تہی, *p;
	خالی *branch_مقصد_free, *argv_مقصد_free = تہی;
	صحیح orig_argc = argc;
	صحیح ملاؤ_log_config = -1;

	show_usage_with_options_if_asked(argc, argv,
					 builtin_ملاؤ_usage, builtin_ملاؤ_options);

#اگر_غیر_تعریف_شدہ WITH_BREAKING_CHANGES
	warn_on_auto_comment_char = true;
#اگر_ختم /* !WITH_BREAKING_CHANGES */
	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_بھرا_اشاریہ = 0;

	/*
	 * Check if we are _not_ on a detached HEAD, i.e. if there is a
	 * current branch.
	 */
	branch = branch_مقصد_free = refs_resolve_refdup(get_main_ref_store(the_repository),
						      "HEAD", 0, &سر_oid,
						      تہی);
	اگر (branch)
		skip_سابقہ(branch, "refs/heads/", &branch);

	init_diff_ui_defaults();
	repo_config(the_repository, git_ملاؤ_config, &ملاؤ_log_config);

	اگر (!branch || is_null_oid(&سر_oid))
		سر_commit = تہی;
	ورنہ
		سر_commit = lookup_commit_or_die(&سر_oid, "HEAD");

	اگر (branch_mergeoptions)
		parse_branch_ملاؤ_options(branch_mergeoptions);
	argc = parse_options(argc, argv, سابقہ, builtin_ملاؤ_options,
			builtin_ملاؤ_usage, 0);
	اگر (shortlog_len < 0)
		shortlog_len = (ملاؤ_log_config > 0) ? ملاؤ_log_config : 0;

	اگر (verbosity < 0 && show_پیش_رفت == -1)
		show_پیش_رفت = 0;

	اگر (abort_current_ملاؤ) {
		صحیح nargc = 2;
		ثابت حرف *nargv[] = {"reset", "--merge", تہی};
		حرف stash_oid_hex[GIT_MAX_HEXSZ + 1];
		ساخت object_id stash_oid = {0};

		اگر (orig_argc != 2)
			usage_پیغام_opt(_("--abort expects no arguments"),
			      builtin_ملاؤ_usage, builtin_ملاؤ_options);

		اگر (!file_exists(git_راستہ_ملاؤ_سر(the_repository)))
			die(_("There is no merge to abort (MERGE_HEAD missing)."));

		اگر (!refs_read_ref(get_main_ref_store(the_repository), "MERGE_AUTOSTASH", &stash_oid))
			refs_delete_ref(get_main_ref_store(the_repository),
					"", "MERGE_AUTOSTASH", &stash_oid,
					REF_NO_DEREF);

		/* Invoke 'git reset --merge' */
		ret = cmd_reset(nargc, nargv, سابقہ, the_repository);

		اگر (!is_null_oid(&stash_oid)) {
			oid_مقصد_hex_r(stash_oid_hex, &stash_oid);
			apply_autostash_oid(stash_oid_hex);
		}

		جاؤ مکمل;
	}

	اگر (quit_current_ملاؤ) {
		اگر (orig_argc != 2)
			usage_پیغام_opt(_("--quit expects no arguments"),
				      builtin_ملاؤ_usage,
				      builtin_ملاؤ_options);

		remove_ملاؤ_branch_state(the_repository);
		جاؤ مکمل;
	}

	اگر (continue_current_ملاؤ) {
		صحیح nargc = 1;
		ثابت حرف *nargv[] = {"commit", تہی};

		اگر (orig_argc != 2)
			usage_پیغام_opt(_("--continue expects no arguments"),
			      builtin_ملاؤ_usage, builtin_ملاؤ_options);

		اگر (!file_exists(git_راستہ_ملاؤ_سر(the_repository)))
			die(_("There is no merge in progress (MERGE_HEAD missing)."));

		/* Invoke 'git commit' */
		ret = cmd_commit(nargc, nargv, سابقہ, the_repository);
		جاؤ مکمل;
	}

	اگر (repo_read_اشاریہ_unmerged(the_repository))
		die_resolve_conflict("merge");

	اگر (file_exists(git_راستہ_ملاؤ_سر(the_repository))) {
		/*
		 * There is no unmerged entry, don't advise 'git
		 * add/rm <file>', just 'git commit'.
		 */
		اگر (advice_enabled(ADVICE_RESOLVE_CONFLICT))
			die(_("You have not concluded your merge (MERGE_HEAD exists).\n"
				  "Please, commit your changes before you merge."));
		ورنہ
			die(_("You have not concluded your merge (MERGE_HEAD exists)."));
	}
	اگر (refs_ref_exists(get_main_ref_store(the_repository), "CHERRY_PICK_HEAD")) {
		اگر (advice_enabled(ADVICE_RESOLVE_CONFLICT))
			die(_("You have not concluded your cherry-pick (CHERRY_PICK_HEAD exists).\n"
			    "Please, commit your changes before you merge."));
		ورنہ
			die(_("You have not concluded your cherry-pick (CHERRY_PICK_HEAD exists)."));
	}
	resolve_undo_clear_اشاریہ(the_repository->اشاریہ);

	اگر (option_edit < 0)
		option_edit = default_edit_option();

	cleanup_منوال = get_cleanup_منوال(cleanup_arg, 0 < option_edit);

	اگر (verbosity < 0)
		show_diffstat = 0;

	اگر (squash) {
		اگر (fast_forward == FF_NO)
			die(_("options '%s' and '%s' cannot be used together"), "--squash", "--no-ff.");
		اگر (option_commit > 0)
			die(_("options '%s' and '%s' cannot be used together"), "--squash", "--commit.");
		/*
		 * squash can now silently disable option_commit - this is not
		 * a problem as it is only overriding the default, not a user
		 * supplied option.
		 */
		option_commit = 0;
	}

	اگر (option_commit < 0)
		option_commit = 1;

	اگر (!argc) {
		اگر (default_مقصد_upstream) {
			argc = تیاری_with_upstream(&argv);
			argv_مقصد_free = argv;
		}
		ورنہ
			die(_("No commit specified and merge.defaultToUpstream not set."));
	} ورنہ اگر (argc == 1 && !strcmp(argv[0], "-")) {
		argv[0] = "@{-1}";
	}

	اگر (!argc)
		usage_with_options(builtin_ملاؤ_usage,
			builtin_ملاؤ_options);

	اگر (!سر_commit) {
		/*
		 * If the merged head is a valid one there is no reason
		 * to forbid "git merge" into a branch yet to be born.
		 * We do the same for "git pull".
		 */
		ساخت object_id *remote_سر_oid;
		اگر (squash)
			die(_("Squash commit into empty head not supported yet"));
		اگر (fast_forward == FF_NO)
			die(_("Non-fast-forward commit does not make sense into "
			    "an empty head"));
		remoteheads = collect_parents(سر_commit, &سر_subsumed,
					      argc, argv, تہی);
		اگر (!remoteheads)
			die(_("%s - not something we can merge"), argv[0]);
		اگر (remoteheads->اگلا)
			die(_("Can merge only exactly one commit into empty head"));

		اگر (verify_signatures)
			verify_ملاؤ_signature(remoteheads->شے, verbosity,
					       check_trust_level);

		remote_سر_oid = &remoteheads->شے->object.oid;
		read_خالی_خانے(remote_سر_oid);
		refs_update_ref(get_main_ref_store(the_repository),
				"initial pull", "HEAD", remote_سر_oid, تہی,
				0,
				UPDATE_REFS_DIE_ON_ERR);
		جاؤ مکمل;
	}

	/*
	 * All the rest are the commits being merged; prepare
	 * the standard merge summary message to be appended
	 * to the given message.
	 */
	remoteheads = collect_parents(سر_commit, &سر_subsumed,
				      argc, argv, &ملاؤ_پیغام);

	اگر (!سر_commit || !argc)
		usage_with_options(builtin_ملاؤ_usage,
			builtin_ملاؤ_options);

	اگر (verify_signatures) {
		برائے (p = remoteheads; p; p = p->اگلا) {
			verify_ملاؤ_signature(p->شے, verbosity,
					       check_trust_level);
		}
	}

	strbuf_addstr(&buf, "merge");
	برائے (p = remoteheads; p; p = p->اگلا)
		strbuf_addf(&buf, " %s", ملاؤ_remote_util(p->شے)->name);
	setenv("GIT_REFLOG_ACTION", buf.buf, 0);
	strbuf_reset(&buf);

	برائے (p = remoteheads; p; p = p->اگلا) {
		ساخت commit *commit = p->شے;
		strbuf_addf(&buf, "GITHEAD_%s",
			    oid_مقصد_hex(&commit->object.oid));
		setenv(buf.buf, ملاؤ_remote_util(commit)->name, 1);
		strbuf_reset(&buf);
		اگر (fast_forward != FF_ONLY && merging_a_throwaway_لیبل(commit))
			fast_forward = FF_NO;
	}

	اگر (!use_strategies) {
		اگر (!remoteheads)
			; /* already up-to-date */
		ورنہ اگر (!remoteheads->اگلا)
			جوڑو_strategies(pull_twohead, DEFAULT_TWOHEAD);
		ورنہ
			جوڑو_strategies(pull_octopus, DEFAULT_OCTOPUS);
	}

	برائے (i = 0; i < use_strategies_nr; i++) {
		اگر (use_strategies[i]->attr & NO_FAST_FORWARD)
			fast_forward = FF_NO;
		اگر (use_strategies[i]->attr & NO_TRIVIAL)
			allow_trivial = 0;
	}

	اگر (!remoteheads)
		; /* already up-to-date */
	ورنہ اگر (!remoteheads->اگلا) {
		اگر (repo_get_ملاؤ_bases(the_repository, سر_commit,
					 remoteheads->شے, &common) < 0) {
			ret = 2;
			جاؤ مکمل;
		}
	} ورنہ {
		ساخت commit_list *list = remoteheads;
		commit_list_داخل_کرو(سر_commit, &list);
		اگر (get_octopus_ملاؤ_bases(list, &common) < 0) {
			آزاد(list);
			ret = 2;
			جاؤ مکمل;
		}
		آزاد(list);
	}

	refs_update_ref(get_main_ref_store(the_repository),
			"updating ORIG_HEAD", "ORIG_HEAD",
			&سر_commit->object.oid, تہی, 0,
			UPDATE_REFS_DIE_ON_ERR);

	اگر (remoteheads && !common) {
		/* No common ancestors found. */
		اگر (!allow_unrelated_histories)
			die(_("refusing to merge unrelated histories"));
		/* otherwise, we need a real merge. */
	} ورنہ اگر (!remoteheads ||
		 (!remoteheads->اگلا && !common->اگلا &&
		  common->شے == remoteheads->شے)) {
		/*
		 * If head can reach all the merge then we are up to date.
		 * but first the most common case of merging one remote.
		 */
		اختتام_اوپر_مقصد_date();
		جاؤ مکمل;
	} ورنہ اگر (fast_forward != FF_NO && !remoteheads->اگلا &&
			!common->اگلا &&
			oideq(&common->شے->object.oid, &سر_commit->object.oid)) {
		/* Again the most common case of merging one remote. */
		ثابت حرف *پیغام = have_پیغام ?
			"Fast-forward (no commit created; -m option ignored)" :
			"Fast-forward";
		ساخت commit *commit;

		اگر (verbosity >= 0) {
			لکھو(_("Updating %s..%s\n"),
			       repo_ڈھونڈو_unique_abbrev(the_repository, &سر_commit->object.oid,
						       DEFAULT_ABBREV),
			       repo_ڈھونڈو_unique_abbrev(the_repository, &remoteheads->شے->object.oid,
						       DEFAULT_ABBREV));
		}
		commit = remoteheads->شے;
		اگر (!commit) {
			ret = 1;
			جاؤ مکمل;
		}

		اگر (autostash)
			create_autostash_ref(the_repository, "MERGE_AUTOSTASH",
					     تہی, false);
		اگر (checkout_fast_forward(the_repository,
					  &سر_commit->object.oid,
					  &commit->object.oid,
					  overwrite_ignore)) {
			apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
					    تہی, تہی, تہی, تہی);
			ret = 1;
			جاؤ مکمل;
		}

		اختتام(سر_commit, remoteheads, &commit->object.oid, پیغام);
		remove_ملاؤ_branch_state(the_repository);
		جاؤ مکمل;
	} ورنہ اگر (!remoteheads->اگلا && common->اگلا)
		;
		/*
		 * We are not doing octopus and not fast-forward.  Need
		 * a real merge.
		 */
	ورنہ اگر (!remoteheads->اگلا && !common->اگلا && option_commit) {
		/*
		 * We are not doing octopus, not fast-forward, and have
		 * only one common.
		 */
		refresh_اشاریہ(the_repository->اشاریہ, REFRESH_QUIET, تہی, تہی, تہی);
		اگر (allow_trivial && fast_forward != FF_ONLY) {
			/*
			 * Must first ensure that index matches HEAD before
			 * attempting a trivial merge.
			 */
			ساخت درخت *سر_درخت = repo_get_commit_درخت(the_repository,
								      سر_commit);
			ساخت strbuf sb = STRBUF_INIT;

			اگر (repo_اشاریہ_has_changes(the_repository, سر_درخت,
						   &sb)) {
				error(_("Your local changes to the following files would be overwritten by merge:\n  %s"),
				      sb.buf);
				strbuf_release(&sb);
				ret = 2;
				جاؤ مکمل;
			}

			/* See if it is really trivial. */
			git_committer_info(IDENT_STRICT);
			لکھو(_("Trying really trivial in-index merge...\n"));
			اگر (!read_درخت_trivial(&common->شے->object.oid,
					       &سر_commit->object.oid,
					       &remoteheads->شے->object.oid)) {
				ret = ملاؤ_trivial(سر_commit, remoteheads);
				جاؤ مکمل;
			}
			لکھو(_("Nope.\n"));
		}
	} ورنہ {
		/*
		 * An octopus.  If we can reach all the remote we are up
		 * to date.
		 */
		صحیح اوپر_مقصد_date = 1;
		ساخت commit_list *j;

		برائے (j = remoteheads; j; j = j->اگلا) {
			صحیح ret = repo_اندر_ملاؤ_bases(the_repository,
						      j->شے, سر_commit);
			اگر (ret < 0)
				باہر_نکلو(128);
			اگر (!ret) {
				اوپر_مقصد_date = 0;
				توڑو;
			}
		}
		اگر (اوپر_مقصد_date) {
			اختتام_اوپر_مقصد_date();
			جاؤ مکمل;
		}
	}

	اگر (fast_forward == FF_ONLY)
		die_ff_impossible();

	اگر (autostash)
		create_autostash_ref(the_repository, "MERGE_AUTOSTASH",
				     تہی, false);

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
	اگر (save_state(&stash))
		oidclr(&stash, the_repository->hash_algo);

	برائے (i = 0; i < use_strategies_nr; i++) {
		صحیح ret, cnt;
		اگر (i) {
			لکھو(_("Rewinding the tree to pristine...\n"));
			restore_state(&سر_commit->object.oid, &stash);
		}
		اگر (use_strategies_nr != 1)
			لکھو(_("Trying merge strategy %s...\n"),
				use_strategies[i]->name);
		/*
		 * Remember which strategy left the state in the working
		 * tree.
		 */
		wt_strategy = use_strategies[i]->name;

		ret = try_ملاؤ_strategy(wt_strategy,
					 common, remoteheads,
					 سر_commit);
		/*
		 * The backend exits with 1 when conflicts are
		 * left to be resolved, with 2 when it does not
		 * handle the given merge at all.
		 */
		اگر (ret < 2) {
			اگر (!ret) {
				/*
				 * This strategy worked; no point in trying
				 * another.
				 */
				ملاؤ_was_ok = 1;
				بہترین_strategy = wt_strategy;
				توڑو;
			}
			cnt = (use_strategies_nr > 1) ? evaluate_نتیجہ() : 0;
			اگر (بہترین_cnt <= 0 || cnt <= بہترین_cnt) {
				بہترین_strategy = wt_strategy;
				بہترین_cnt = cnt;
			}
		}
	}

	/*
	 * If we have a resulting tree, that means the strategy module
	 * auto resolved the merge cleanly.
	 */
	اگر (ملاؤ_was_ok && option_commit) {
		automerge_was_ok = 1;
		ret = اختتام_automerge(سر_commit, سر_subsumed,
				       common, remoteheads,
				       &نتیجہ_درخت, wt_strategy);
		جاؤ مکمل;
	}

	/*
	 * Pick the result from the best strategy and have the user fix
	 * it up.
	 */
	اگر (!بہترین_strategy) {
		restore_state(&سر_commit->object.oid, &stash);
		اگر (use_strategies_nr > 1)
			fprintf(stderr,
				_("No merge strategy handled the merge.\n"));
		ورنہ
			fprintf(stderr, _("Merge with strategy %s failed.\n"),
				use_strategies[0]->name);
		apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
				    تہی, تہی, تہی, تہی);
		ret = 2;
		جاؤ مکمل;
	} ورنہ اگر (بہترین_strategy == wt_strategy)
		; /* We already have its result in the working tree. */
	ورنہ {
		لکھو(_("Rewinding the tree to pristine...\n"));
		restore_state(&سر_commit->object.oid, &stash);
		لکھو(_("Using the %s strategy to prepare resolving by hand.\n"),
			بہترین_strategy);
		try_ملاؤ_strategy(بہترین_strategy, common, remoteheads,
				   سر_commit);
	}

	اگر (squash) {
		اختتام(سر_commit, remoteheads, تہی, تہی);

		git_test_write_commit_گراف_or_die(the_repository);
	} ورنہ
		write_ملاؤ_state(remoteheads);

	اگر (ملاؤ_was_ok)
		fprintf(stderr, _("Automatic merge went well; "
			"stopped before committing as requested\n"));
	ورنہ
		ret = suggest_conflicts();
	اگر (autostash)
		لکھو(_("When finished, apply stashed changes with `git stash pop`\n"));

مکمل:
	اگر (!automerge_was_ok) {
		commit_list_free(common);
		commit_list_free(remoteheads);
	}
	strbuf_release(&buf);
	آزاد(branch_مقصد_free);
	آزاد(argv_مقصد_free);
	آزاد(pull_twohead);
	آزاد(pull_octopus);
	discard_اشاریہ(the_repository->اشاریہ);
	واپس ret;
}
