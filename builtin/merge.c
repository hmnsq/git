/*
 * Builtin "git merge"
 *
 * Copyright (c) 2008 Miklos Vajna <vmiklos@frugalware.org>
 *
 * Based on git-merge.sh by Junio C Hamano.
 */

#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#تعریف_کرو DISABLE_SIGN_COMPARE_WARNINGS

#ضمن "builtin.h"

#ضمن "abspath.h"
#ضمن "advice.h"
#ضمن "config.h"
#ضمن "editor.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "hex.h"
#ضمن "object-name.h"
#ضمن "parse-options.h"
#ضمن "lockfile.h"
#ضمن "repository.h"
#ضمن "run-command.h"
#ضمن "hook.h"
#ضمن "diff.h"
#ضمن "diff-merges.h"
#ضمن "refs.h"
#ضمن "refspec.h"
#ضمن "commit.h"
#ضمن "diffcore.h"
#ضمن "path.h"
#ضمن "revision.h"
#ضمن "unpack-trees.h"
#ضمن "cache-tree.h"
#ضمن "dir.h"
#ضمن "color.h"
#ضمن "rerere.h"
#ضمن "help.h"
#ضمن "merge.h"
#ضمن "merge-ort-wrappers.h"
#ضمن "resolve-undo.h"
#ضمن "remote.h"
#ضمن "fmt-merge-msg.h"
#ضمن "sequencer.h"
#ضمن "string-list.h"
#ضمن "tag.h"
#ضمن "alias.h"
#ضمن "branch.h"
#ضمن "commit-reach.h"
#ضمن "wt-status.h"
#ضمن "commit-graph.h"

#تعریف_کرو DEFAULT_TWOHEAD (1<<0)
#تعریف_کرو DEFAULT_OCTOPUS (1<<1)
#تعریف_کرو NO_FAST_FORWARD (1<<2)
#تعریف_کرو NO_TRIVIAL      (1<<3)

بنية strategy {
	ثابت حرف *name;
	بنانشانی attr;
};

ساكن ثابت حرف * ثابت builtin_ملاؤ_usage[] = {
	N_("git merge [<options>] [<commit>...]"),
	"git merge --abort",
	"git merge --continue",
	عدم
};

#تعریف_کرو ملاؤ_SHOW_DIFFSTAT 1
#تعریف_کرو ملاؤ_SHOW_COMPACTSUMMARY 2

ساكن صحيح show_diffstat = ملاؤ_SHOW_DIFFSTAT, shortlog_len = -1, squash;
ساكن صحيح option_commit = -1;
ساكن صحيح option_edit = -1;
ساكن صحيح allow_trivial = 1, have_سنیہا, verify_signatures;
ساكن صحيح check_trust_level = 1;
ساكن صحيح overwrite_ignore = 1;
ساكن بنية strbuf ملاؤ_پیغام = STRBUF_INIT;
ساكن بنية strategy **use_strategies;
ساكن ناپ_t use_strategies_nr, use_strategies_alloc;
ساكن بنية strvec xopts = STRVEC_INIT;
ساكن ثابت حرف *branch;
ساكن حرف *branch_mergeoptions;
ساكن صحيح verbosity;
ساكن صحيح allow_rerere_auto;
ساكن صحيح abort_current_ملاؤ;
ساكن صحيح quit_current_ملاؤ;
ساكن صحيح continue_current_ملاؤ;
ساكن صحيح allow_unrelated_histories;
ساكن صحيح show_ترقی = -1;
ساكن صحيح default_منزل_upstream = 1;
ساكن صحيح signoff;
ساكن ثابت حرف *sign_commit;
ساكن صحيح autostash;
ساكن صحيح no_verify;
ساكن حرف *into_name;

ساكن بنية strategy all_strategy[] = {
	{ "recursive",  NO_TRIVIAL },
	{ "octopus",    DEFAULT_OCTOPUS },
	{ "ort",        DEFAULT_TWOHEAD | NO_TRIVIAL },
	{ "resolve",    0 },
	{ "ours",       NO_FAST_FORWARD | NO_TRIVIAL },
	{ "subtree",    NO_FAST_FORWARD | NO_TRIVIAL },
};

ساكن حرف *pull_twohead, *pull_octopus;

تعداد ff_type {
	FF_NO,
	FF_ALLOW,
	FF_ONLY
};

ساكن تعداد ff_type fast_forward = FF_ALLOW;

ساكن حرف *cleanup_arg;
ساكن تعداد commit_پیغام_cleanup_منوال cleanup_منوال;

ساكن صحيح option_parse_سنیہا(ثابت بنية option *opt,
				ثابت حرف *arg, صحيح unset)
{
	بنية strbuf *buf = opt->قدر;

	إذا (unset)
		strbuf_setlen(buf, 0);
	وإلا إذا (arg) {
		strbuf_addf(buf, "%s%s", buf->len ? "\n\n" : "", arg);
		have_سنیہا = 1;
	} وإلا
		أرجع error(_("switch `m' requires a value"));
	أرجع 0;
}

ساكن تعداد parse_opt_نتیجہ option_read_سنیہا(بنية parse_opt_ctx_t *ctx,
						 ثابت بنية option *opt,
						 ثابت حرف *arg_not_ورتیا,
						 صحيح unset)
{
	بنية strbuf *buf = opt->قدر;
	ثابت حرف *arg;

	BUG_ON_OPT_ARG(arg_not_ورتیا);
	إذا (unset)
		BUG("-F cannot be negated");

	إذا (ctx->opt) {
		arg = ctx->opt;
		ctx->opt = عدم;
	} وإلا إذا (ctx->argc > 1) {
		ctx->argc--;
		arg = *++ctx->argv;
	} وإلا
		أرجع error(_("option `%s' requires a value"), opt->long_name);

	إذا (buf->len)
		strbuf_addch(buf, '\n');
	إذا (ctx->اگیتر && !is_absolute_رستہ(arg))
		arg = اگیتر_filename(ctx->اگیتر, arg);
	إذا (strbuf_read_file(buf, arg, 0) < 0)
		أرجع error(_("could not read file '%s'"), arg);
	have_سنیہا = 1;

	أرجع 0;
}

ساكن بنية strategy *get_strategy(ثابت حرف *name)
{
	صحيح i;
	بنية strategy *ret;
	ساكن بنية cmdnames main_cmds = {0}, other_cmds = {0};
	ساكن صحيح loaded;
	حرف *default_strategy = عدم;

	إذا (!name)
		أرجع عدم;

	إذا (default_strategy &&
	    !strcmp(default_strategy, "ort") &&
	    !strcmp(name, "recursive")) {
		name = "ort";
	}

	لكل (i = 0; i < ARRAY_ناپ(all_strategy); i++)
		إذا (!strcmp(name, all_strategy[i].name))
			أرجع &all_strategy[i];

	إذا (!loaded) {
		بنية cmdnames not_strategies = {0};
		loaded = 1;

		لدو_command_list("git-merge-", &main_cmds, &other_cmds);
		لكل (i = 0; i < main_cmds.cnt; i++) {
			صحيح j, لبھیا_2 = 0;
			بنية cmdname *ent = main_cmds.ناں[i];
			لكل (j = 0; !لبھیا_2 && j < ARRAY_ناپ(all_strategy); j++)
				إذا (!xstrncmpz(all_strategy[j].name, ent->name, ent->len))
					لبھیا_2 = 1;
			إذا (!لبھیا_2)
				جوڑو_cmdname(&not_strategies, ent->name, ent->len);
		}
		exclude_cmds(&main_cmds, &not_strategies);

		cmdnames_release(&not_strategies);
	}
	إذا (!is_وچ_cmdlist(&main_cmds, name) && !is_وچ_cmdlist(&other_cmds, name)) {
		fprintf(stderr, _("Could not find merge strategy '%s'.\n"), name);
		fprintf(stderr, _("Available strategies are:"));
		لكل (i = 0; i < main_cmds.cnt; i++)
			fprintf(stderr, " %s", main_cmds.ناں[i]->name);
		fprintf(stderr, ".\n");
		إذا (other_cmds.cnt) {
			fprintf(stderr, _("Available custom strategies are:"));
			لكل (i = 0; i < other_cmds.cnt; i++)
				fprintf(stderr, " %s", other_cmds.ناں[i]->name);
			fprintf(stderr, ".\n");
		}
		باہر_نکلو(1);
	}

	CALLOC_ARRAY(ret, 1);
	ret->name = xstrdup(name);
	ret->attr = NO_TRIVIAL;

	cmdnames_release(&main_cmds);
	cmdnames_release(&other_cmds);
	أرجع ret;
}

ساكن فراغ append_strategy(بنية strategy *s)
{
	ALLOC_GROW(use_strategies, use_strategies_nr + 1, use_strategies_alloc);
	use_strategies[use_strategies_nr++] = s;
}

ساكن صحيح option_parse_strategy(ثابت بنية option *opt بن_ورتیا,
				 ثابت حرف *name, صحيح unset)
{
	إذا (unset)
		أرجع 0;

	append_strategy(get_strategy(name));
	أرجع 0;
}

ساكن صحيح option_parse_compact_summary(ثابت بنية option *opt,
					ثابت حرف *name بن_ورتیا, صحيح unset)
{
	صحيح *setting = opt->قدر;

	إذا (unset)
		*setting = 0;
	وإلا
		*setting = ملاؤ_SHOW_COMPACTSUMMARY;
	أرجع 0;
}

ساكن بنية option builtin_ملاؤ_options[] = {
	OPT_سیٹ_INT('n', عدم, &show_diffstat,
		N_("do not show a diffstat at the end of the merge"), 0),
	OPT_BOOL(0, "stat", &show_diffstat,
		N_("show a diffstat at the end of the merge")),
	OPT_BOOL(0, "summary", &show_diffstat, N_("(synonym to --stat)")),
	OPT_CALLBACK_F(0, "compact-summary", &show_diffstat, عدم,
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
	OPT_CALLBACK('s', "strategy", عدم, N_("strategy"),
		N_("merge strategy to use"), option_parse_strategy),
	OPT_STRVEC('X', "strategy-option", &xopts, N_("option=value"),
		N_("option for selected merge strategy")),
	OPT_CALLBACK('m', "message", &ملاؤ_پیغام, N_("message"),
		N_("merge commit message (for a non-fast-forward merge)"),
		option_parse_سنیہا),
	{
		.type = OPTION_LOWLEVEL_CALLBACK,
		.short_name = 'F',
		.long_name = "file",
		.قدر = &ملاؤ_پیغام,
		.argh = N_("path"),
		.help = N_("read message from file"),
		.flags = PARSE_OPT_NONEG,
		.ll_callback = option_read_سنیہا,
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
	OPT_سیٹ_INT(0, "progress", &show_ترقی, N_("force progress reporting"), 1),
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
	OPT_سرا()
};

ساكن صحيح save_state(بنية object_id *stash)
{
	صحيح len;
	بنية بچہ_process cp = بچہ_PROCESS_INIT;
	بنية strbuf بفر = STRBUF_INIT;
	بنية جندرا_file جندرا_file = جندرا_INIT;
	صحيح fd;
	صحيح rc = -1;

	fd = repo_hold_locked_اشاریہ(the_repository, &جندرا_file, 0);
	refresh_اشاریہ(the_repository->اشاریہ, REFRESH_QUIET, عدم, عدم, عدم);
	إذا (0 <= fd)
		repo_update_اشاریہ_if_able(the_repository, &جندرا_file);
	rollback_جندرا_file(&جندرا_file);

	strvec_pushl(&cp.دلیلاں, "stash", "create", عدم);
	cp.باہرلا = -1;
	cp.git_cmd = 1;

	إذا (شروع_command(&cp))
		die(_("could not run stash."));
	len = strbuf_read(&بفر, cp.باہرلا, 1024);
	close(cp.باہرلا);

	إذا (انت_command(&cp) || len < 0)
		die(_("stash failed"));
	وإلا إذا (!len)		/* no changes */
		اذهب باہرلا;
	strbuf_setlen(&بفر, بفر.len-1);
	إذا (repo_get_oid(the_repository, بفر.buf, stash))
		die(_("not a valid object: %s"), بفر.buf);
	rc = 0;
باہرلا:
	strbuf_release(&بفر);
	أرجع rc;
}

ساكن فراغ read_خالی_تھاواں(ثابت بنية object_id *oid)
{
	بنية بچہ_process cmd = بچہ_PROCESS_INIT;

	strvec_pushl(&cmd.دلیلاں, "read-tree", "-m", "-u",
		     خالی_تھاواں_درخت_oid_hex(the_repository->hash_algo),
		     oid_منزل_hex(oid), عدم);
	cmd.git_cmd = 1;

	إذا (لگاتار_command(&cmd))
		die(_("read-tree failed"));
}

ساكن فراغ reset_hard(ثابت بنية object_id *oid)
{
	بنية بچہ_process cmd = بچہ_PROCESS_INIT;

	strvec_pushl(&cmd.دلیلاں, "read-tree", "-v", "--reset", "-u",
		     oid_منزل_hex(oid), عدم);
	cmd.git_cmd = 1;

	إذا (لگاتار_command(&cmd))
		die(_("read-tree failed"));
}

ساكن فراغ restore_state(ثابت بنية object_id *سر,
			  ثابت بنية object_id *stash)
{
	بنية بچہ_process cmd = بچہ_PROCESS_INIT;

	reset_hard(سر);

	إذا (is_null_oid(stash))
		اذهب refresh_cache;

	strvec_pushl(&cmd.دلیلاں, "stash", "apply", "--index", "--quiet", عدم);
	strvec_ڈالو(&cmd.دلیلاں, oid_منزل_hex(stash));

	/*
	 * It is OK to ignore error here, for example when there was
	 * nothing to restore.
	 */
	cmd.git_cmd = 1;
	لگاتار_command(&cmd);

refresh_cache:
	discard_اشاریہ(the_repository->اشاریہ);
	إذا (repo_read_اشاریہ(the_repository) < 0)
		die(_("could not read index"));
}

/* This is called when no merge was necessary. */
ساكن فراغ انت_اتے_منزل_date(فراغ)
{
	إذا (verbosity >= 0) {
		إذا (squash)
			puts(_("Already up to date. (nothing to squash)"));
		وإلا
			puts(_("Already up to date."));
	}
	remove_ملاؤ_branch_state(the_repository);
}

ساكن فراغ squash_سنیہا(بنية commit *commit, بنية commit_list *remoteheads)
{
	بنية rev_info rev;
	بنية strbuf باہرلا = STRBUF_INIT;
	بنية commit_list *j;
	بنية pretty_print_context ctx = {0};

	لکھو(_("Squash commit -- not updating HEAD\n"));

	repo_init_revisions(the_repository, &rev, عدم);
	diff_merges_suppress(&rev);
	rev.commit_format = CMIT_FMT_MEDIUM;

	commit->object.flags |= UNINTERESTING;
	جوڑو_pending_object(&rev, &commit->object, عدم);

	لكل (j = remoteheads; j; j = j->اگلا)
		جوڑو_pending_object(&rev, &j->شے->object, عدم);

	تیاری_revisions(0, عدم, &rev, عدم);
	إذا (prepare_revision_walk(&rev))
		die(_("revision walk setup failed"));

	ctx.abbrev = rev.abbrev;
	ctx.date_منوال = rev.date_منوال;
	ctx.fmt = rev.commit_format;

	strbuf_addstr(&باہرلا, "Squashed commit of the following:\n");
	طالما ((commit = get_revision(&rev)) != عدم) {
		strbuf_addch(&باہرلا, '\n');
		strbuf_addf(&باہرلا, "commit %s\n",
			oid_منزل_hex(&commit->object.oid));
		pretty_print_commit(&ctx, commit, &باہرلا);
	}
	write_file_buf(git_رستہ_squash_پیغام(the_repository), باہرلا.buf, باہرلا.len);
	strbuf_release(&باہرلا);
	release_revisions(&rev);
}

ساكن فراغ انت(بنية commit *سر_commit,
		   بنية commit_list *remoteheads,
		   ثابت بنية object_id *new_سر, ثابت حرف *پیغام)
{
	بنية strbuf reflog_سنیہا = STRBUF_INIT;
	ثابت بنية object_id *سر = &سر_commit->object.oid;

	إذا (!پیغام)
		strbuf_addstr(&reflog_سنیہا, getenv("GIT_REFLOG_ACTION"));
	وإلا {
		إذا (verbosity >= 0)
			لکھو("%s\n", پیغام);
		strbuf_addf(&reflog_سنیہا, "%s: %s",
			getenv("GIT_REFLOG_ACTION"), پیغام);
	}
	إذا (squash) {
		squash_سنیہا(سر_commit, remoteheads);
	} وإلا {
		إذا (verbosity >= 0 && !ملاؤ_پیغام.len)
			لکھو(_("No merge message -- not updating HEAD\n"));
		وإلا {
			refs_update_ref(get_main_ref_store(the_repository),
					reflog_سنیہا.buf, "HEAD", new_سر,
					سر,
					0, UPDATE_REFS_DIE_ON_ERR);
			/*
			 * We ignore errors in 'gc --auto', since the
			 * user should see them.
			 */
			لگاتار_auto_maintenance(the_repository, verbosity < 0);
		}
	}
	إذا (new_سر && show_diffstat) {
		بنية diff_options opts;
		repo_diff_تیاری(the_repository, &opts);
		init_diffstat_widths(&opts);

		بدل (show_diffstat) {
		حالة ملاؤ_SHOW_DIFFSTAT: /* 1 */
			opts.output_format |=
				DIFF_FORMAT_SUMMARY | DIFF_FORMAT_DIFFSTAT;
			اكسر;
		حالة ملاؤ_SHOW_COMPACTSUMMARY: /* 2 */
			opts.output_format |= DIFF_FORMAT_DIFFSTAT;
			opts.flags.stat_with_summary = 1;
			اكسر;
		افتراضي:
			اكسر;
		}
		opts.detect_rename = DIFF_DETECT_RENAME;
		diff_تیاری_پورا(&opts);
		diff_درخت_oid(سر, new_سر, "", &opts);
		diffcore_std(&opts);
		diff_flush(&opts);
	}

	/* Run a post-merge hook */
	لگاتار_hooks_l(the_repository, "post-merge", squash ? "1" : "0", عدم);

	إذا (new_سر)
		apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
				    عدم, عدم, عدم, عدم);
	strbuf_release(&reflog_سنیہا);
}

/* Get the name for the merge commit's message. */
ساكن فراغ ملاؤ_name(ثابت حرف *remote, بنية strbuf *پیغام)
{
	بنية commit *remote_سر;
	بنية object_id branch_سر;
	بنية strbuf bname = STRBUF_INIT;
	بنية ملاؤ_remote_desc *desc;
	ثابت حرف *ptr;
	حرف *لبھیا_ref = عدم;
	صحيح len, early;

	copy_branchname(the_repository, &bname, remote, 0);
	remote = bname.buf;

	oidclr(&branch_سر, the_repository->hash_algo);
	remote_سر = get_ملاؤ_ماپا(remote);
	إذا (!remote_سر)
		die(_("'%s' does not point to a commit"), remote);

	إذا (repo_dwim_ref(the_repository, remote, لڑی_لمبائی(remote), &branch_سر,
			  &لبھیا_ref, 0) > 0) {
		إذا (starts_with(لبھیا_ref, "refs/heads/")) {
			strbuf_addf(پیغام, "%s\t\tbranch '%s' of .\n",
				    oid_منزل_hex(&branch_سر), remote);
			اذهب cleanup;
		}
		إذا (starts_with(لبھیا_ref, "refs/tags/")) {
			strbuf_addf(پیغام, "%s\t\ttag '%s' of .\n",
				    oid_منزل_hex(&branch_سر), remote);
			اذهب cleanup;
		}
		إذا (starts_with(لبھیا_ref, "refs/remotes/")) {
			strbuf_addf(پیغام, "%s\t\tremote-tracking branch '%s' of .\n",
				    oid_منزل_hex(&branch_سر), remote);
			اذهب cleanup;
		}
	}

	/* See if remote matches <name>^^^.. or <name>~<number> */
	لكل (len = 0, ptr = remote + لڑی_لمبائی(remote);
	     remote < ptr && ptr[-1] == '^';
	     ptr--)
		len++;
	إذا (len)
		early = 1;
	وإلا {
		early = 0;
		ptr = strrchr(remote, '~');
		إذا (ptr) {
			صحيح تکیا_nonzero = 0;

			len++; /* count ~ */
			طالما (*++ptr && ہندسہ_اے(*ptr)) {
				تکیا_nonzero |= (*ptr != '0');
				len++;
			}
			إذا (*ptr)
				len = 0; /* not ...~<number> */
			وإلا إذا (تکیا_nonzero)
				early = 1;
			وإلا إذا (len == 1)
				early = 1; /* "name~" is "name~1"! */
		}
	}
	إذا (len) {
		بنية strbuf truname = STRBUF_INIT;
		strbuf_addf(&truname, "refs/heads/%s", remote);
		strbuf_setlen(&truname, truname.len - len);
		إذا (refs_ref_exists(get_main_ref_store(the_repository), truname.buf)) {
			strbuf_addf(پیغام,
				    "%s\t\tbranch '%s'%s of .\n",
				    oid_منزل_hex(&remote_سر->object.oid),
				    truname.buf + 11,
				    (early ? " (early part)" : ""));
			strbuf_release(&truname);
			اذهب cleanup;
		}
		strbuf_release(&truname);
	}

	desc = ملاؤ_remote_util(remote_سر);
	إذا (desc && desc->obj && desc->obj->type == OBJ_ٹیگ) {
		strbuf_addf(پیغام, "%s\t\t%s '%s'\n",
			    oid_منزل_hex(&desc->obj->oid),
			    type_name(desc->obj->type),
			    remote);
		اذهب cleanup;
	}

	strbuf_addf(پیغام, "%s\t\tcommit '%s'\n",
		oid_منزل_hex(&remote_سر->object.oid), remote);
cleanup:
	حرر(لبھیا_ref);
	strbuf_release(&bname);
}

ساكن فراغ parse_branch_ملاؤ_options(حرف *bmo)
{
	ثابت حرف **argv;
	صحيح argc;

	إذا (!bmo)
		أرجع;
	argc = ونڈو_cmdline(bmo, &argv);
	إذا (argc < 0)
		die(_("Bad branch.%s.mergeoptions string: %s"), branch,
		    _(ونڈو_cmdline_strerror(argc)));
	REALLOC_ARRAY(argv, argc + 2);
	MOVE_ARRAY(argv + 1, argv, argc + 1);
	argc++;
	argv[0] = "branch.*.mergeoptions";
	parse_options(argc, argv, عدم, builtin_ملاؤ_options,
		      builtin_ملاؤ_usage, 0);
	حرر(argv);
}

ساكن صحيح git_ملاؤ_config(ثابت حرف *k, ثابت حرف *v,
			    ثابت بنية config_context *ctx, فراغ *cb)
{
	صحيح حالت;
	ثابت حرف *str;

	إذا (branch &&
	    skip_اگیتر(k, "branch.", &str) &&
	    skip_اگیتر(str, branch, &str) &&
	    !strcmp(str, ".mergeoptions")) {
		حرر(branch_mergeoptions);
		branch_mergeoptions = xstrdup(v);
		أرجع 0;
	}

	إذا (!strcmp(k, "merge.diffstat") || !strcmp(k, "merge.stat")) {
		صحيح val = git_parse_maybe_bool_متن(v);
		بدل (val) {
		حالة 0:
			show_diffstat = 0;
			اكسر;
		حالة 1:
			show_diffstat = ملاؤ_SHOW_DIFFSTAT;
			اكسر;
		افتراضي:
			إذا (!strcmp(v, "compact"))
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
			وإلا
				/*
				 * A setting from a future?  It is not an
				 * error grave enough to fail the command.
				 * proceed using the default one.
				 */
				show_diffstat = ملاؤ_SHOW_DIFFSTAT;
			اكسر;
		}
	} وإلا إذا (!strcmp(k, "merge.verifysignatures")) {
		verify_signatures = git_config_bool(k, v);
	} وإلا إذا (!strcmp(k, "pull.twohead")) {
		FREE_AND_NULL(pull_twohead);
		أرجع git_config_string(&pull_twohead, k, v);
	} وإلا إذا (!strcmp(k, "pull.octopus")) {
		FREE_AND_NULL(pull_octopus);
		أرجع git_config_string(&pull_octopus, k, v);
	} وإلا إذا (!strcmp(k, "commit.cleanup")) {
		أرجع git_config_string(&cleanup_arg, k, v);
	} وإلا إذا (!strcmp(k, "merge.ff")) {
		صحيح boolval = git_parse_maybe_bool(v);
		إذا (0 <= boolval) {
			fast_forward = boolval ? FF_ALLOW : FF_NO;
		} وإلا إذا (v && !strcmp(v, "only")) {
			fast_forward = FF_ONLY;
		} /* do not barf on values from future versions of git */
		أرجع 0;
	} وإلا إذا (!strcmp(k, "merge.defaulttoupstream")) {
		default_منزل_upstream = git_config_bool(k, v);
		أرجع 0;
	} وإلا إذا (!strcmp(k, "commit.gpgsign")) {
		sign_commit = git_config_bool(k, v) ? "" : عدم;
		أرجع 0;
	} وإلا إذا (!strcmp(k, "gpg.mintrustlevel")) {
		check_trust_level = 0;
	} وإلا إذا (!strcmp(k, "merge.autostash")) {
		autostash = git_config_bool(k, v);
		أرجع 0;
	}

	حالت = fmt_ملاؤ_پیغام_config(k, v, ctx, cb);
	إذا (حالت)
		أرجع حالت;
	أرجع git_diff_ui_config(k, v, ctx, cb);
}

ساكن صحيح read_درخت_trivial(بنية object_id *common, بنية object_id *سر,
			     بنية object_id *one)
{
	صحيح i, nr_trees = 0;
	بنية درخت *trees[MAX_UNPACK_TREES];
	بنية درخت_desc t[MAX_UNPACK_TREES];
	بنية unpack_trees_options opts;

	memset(&opts, 0, حجم(opts));
	opts.سر_idx = 2;
	opts.src_اشاریہ = the_repository->اشاریہ;
	opts.dst_اشاریہ = the_repository->اشاریہ;
	opts.update = 1;
	opts.verbose_update = 1;
	opts.trivial_merges_only = 1;
	opts.ملاؤ = 1;
	opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
	trees[nr_trees] = repo_parse_درخت_اسدھا(the_repository, common);
	إذا (!trees[nr_trees++])
		أرجع -1;
	trees[nr_trees] = repo_parse_درخت_اسدھا(the_repository, سر);
	إذا (!trees[nr_trees++])
		أرجع -1;
	trees[nr_trees] = repo_parse_درخت_اسدھا(the_repository, one);
	إذا (!trees[nr_trees++])
		أرجع -1;
	opts.fn = threeway_ملاؤ;
	cache_درخت_free(&the_repository->اشاریہ->cache_درخت);
	لكل (i = 0; i < nr_trees; i++) {
		repo_parse_درخت(the_repository, trees[i]);
		init_درخت_desc(t+i, &trees[i]->object.oid,
			       trees[i]->بفر, trees[i]->ناپ);
	}
	إذا (unpack_trees(nr_trees, t, &opts))
		أرجع -1;
	أرجع 0;
}

ساكن فراغ write_درخت_trivial(بنية object_id *oid)
{
	إذا (write_اشاریہ_as_درخت(oid, the_repository->اشاریہ,
				repo_get_اشاریہ_file(the_repository),
				0, عدم))
		die(_("git write-tree failed to write a tree"));
}

ساكن صحيح try_ملاؤ_strategy(ثابت حرف *strategy, بنية commit_list *common,
			      بنية commit_list *remoteheads,
			      بنية commit *سر)
{
	ثابت حرف *سر_arg = "HEAD";

	إذا (repo_refresh_and_write_اشاریہ(the_repository, REFRESH_QUIET,
					 SKIP_IF_UNCHANGED, 0, عدم, عدم,
					 عدم) < 0)
		die(_("Unable to write index."));

	إذا (!strcmp(strategy, "recursive") || !strcmp(strategy, "subtree") ||
	    !strcmp(strategy, "ort")) {
		بنية جندرا_file جندرا = جندرا_INIT;
		صحيح clean, x;
		بنية commit *نتیجہ;
		بنية commit_list *reversed = عدم;
		بنية ملاؤ_options o;
		بنية commit_list *j;

		إذا (remoteheads->اگلا) {
			error(_("Not handling anything other than two heads merge."));
			أرجع 2;
		}

		init_ui_ملاؤ_options(&o, the_repository);
		إذا (!strcmp(strategy, "subtree"))
			o.subtree_کھسکاؤ = "";

		o.show_rename_ترقی =
			show_ترقی == -1 ? isatty(2) : show_ترقی;

		لكل (x = 0; x < xopts.nr; x++)
			إذا (parse_ملاؤ_opt(&o, xopts.v[x]))
				die(_("unknown strategy option: -X%s"), xopts.v[x]);

		o.branch1 = سر_arg;
		o.branch2 = ملاؤ_remote_util(remoteheads->شے)->name;

		لكل (j = common; j; j = j->اگلا)
			commit_list_داخل_کرو(j->شے, &reversed);

		repo_hold_locked_اشاریہ(the_repository, &جندرا,
				       جندرا_DIE_ON_ERROR);
		clean = ملاؤ_ort_recursive(&o, سر, remoteheads->شے,
					    reversed, &نتیجہ);
		commit_list_free(reversed);
		strbuf_release(&o.obuf);

		إذا (clean < 0) {
			rollback_جندرا_file(&جندرا);
			أرجع 2;
		}
		إذا (write_locked_اشاریہ(the_repository->اشاریہ, &جندرا,
				       COMMIT_جندرا | SKIP_IF_UNCHANGED))
			die(_("unable to write %s"), repo_get_اشاریہ_file(the_repository));
		أرجع clean ? 0 : 1;
	} وإلا {
		أرجع try_ملاؤ_command(the_repository,
					 strategy, xopts.nr, xopts.v,
					 common, سر_arg, remoteheads);
	}
}

ساكن فراغ گنتی_diff_files(بنية diff_قطار_struct *q,
			     بنية diff_options *opt بن_ورتیا, فراغ *انکڑے)
{
	صحيح *گنتی = انکڑے;

	(*گنتی) += q->nr;
}

ساكن صحيح گنتی_unmerged_entries(فراغ)
{
	صحيح i, ret = 0;

	لكل (i = 0; i < the_repository->اشاریہ->cache_nr; i++)
		إذا (ce_stage(the_repository->اشاریہ->cache[i]))
			ret++;

	أرجع ret;
}

ساكن فراغ جوڑو_strategies(ثابت حرف *string, بنانشانی attr)
{
	صحيح i;

	إذا (string) {
		بنية string_list list = STRING_LIST_INIT_DUP;
		بنية string_list_شے *شے;
		string_list_ونڈو(&list, string, " ", -1);
		for_each_string_list_شے(شے, &list)
			append_strategy(get_strategy(شے->string));
		string_list_clear(&list, 0);
		أرجع;
	}
	لكل (i = 0; i < ARRAY_ناپ(all_strategy); i++)
		إذا (all_strategy[i].attr & attr)
			append_strategy(&all_strategy[i]);

}

ساكن فراغ read_ملاؤ_پیغام(بنية strbuf *پیغام)
{
	ثابت حرف *filename = git_رستہ_ملاؤ_پیغام(the_repository);
	strbuf_reset(پیغام);
	إذا (strbuf_read_file(پیغام, filename, 0) < 0)
		die_errno(_("Could not read from '%s'"), filename);
}

ساكن فراغ write_ملاؤ_state(بنية commit_list *);
ساكن فراغ abort_commit(بنية commit_list *remoteheads, ثابت حرف *err_پیغام)
{
	إذا (err_پیغام)
		error("%s", err_پیغام);
	fprintf(stderr,
		_("Not committing merge; use 'git commit' to complete the merge.\n"));
	write_ملاؤ_state(remoteheads);
	باہر_نکلو(1);
}

ساكن ثابت حرف ملاؤ_editor_comment[] =
N_("Please enter a commit message to explain why this merge is necessary,\n"
   "especially if it merges an updated upstream into a topic branch.\n"
   "\n");

ساكن ثابت حرف scissors_editor_comment[] =
N_("An empty message aborts the commit.\n");

ساكن ثابت حرف no_scissors_editor_comment[] =
N_("Lines starting with '%s' will be ignored, and an empty message aborts\n"
   "the commit.\n");

ساكن فراغ write_ملاؤ_heads(بنية commit_list *);
ساكن فراغ prepare_منزل_commit(بنية commit_list *remoteheads)
{
	بنية strbuf پیغام = STRBUF_INIT;
	ثابت حرف *اشاریہ_file = repo_get_اشاریہ_file(the_repository);

	إذا (!no_verify) {
		صحيح invoked_hook;

		إذا (لگاتار_commit_hook(0 < option_edit, اشاریہ_file, &invoked_hook,
				    "pre-merge-commit", عدم))
			abort_commit(remoteheads, عدم);
		/*
		 * Re-read the index as pre-merge-commit hook could have updated it,
		 * and write it out as a tree.  We must do this before we invoke
		 * the editor and after we invoke run_status above.
		 */
		إذا (invoked_hook)
			discard_اشاریہ(the_repository->اشاریہ);
	}
	read_اشاریہ_from(the_repository->اشاریہ, اشاریہ_file,
			repo_get_git_dir(the_repository));
	strbuf_addbuf(&پیغام, &ملاؤ_پیغام);
	إذا (squash)
		BUG("the control must not reach here under --squash");
	إذا (0 < option_edit) {
		strbuf_addch(&پیغام, '\n');
		إذا (cleanup_منوال == COMMIT_پیغام_CLEANUP_SCISSORS) {
			wt_حالت_append_cut_سطر(&پیغام);
			strbuf_commented_addf(&پیغام, comment_سطر_str, "\n");
		}
		strbuf_commented_addf(&پیغام, comment_سطر_str,
				      _(ملاؤ_editor_comment));
		إذا (cleanup_منوال == COMMIT_پیغام_CLEANUP_SCISSORS)
			strbuf_commented_addf(&پیغام, comment_سطر_str,
					      _(scissors_editor_comment));
		وإلا
			strbuf_commented_addf(&پیغام, comment_سطر_str,
				_(no_scissors_editor_comment), comment_سطر_str);
	}
	إذا (signoff)
		append_signoff(&پیغام, ignored_log_سنیہا_بائٹاں(پیغام.buf, پیغام.len), 0);
	write_ملاؤ_heads(remoteheads);
	write_file_buf(git_رستہ_ملاؤ_پیغام(the_repository), پیغام.buf, پیغام.len);
	إذا (لگاتار_commit_hook(0 < option_edit, repo_get_اشاریہ_file(the_repository),
			    عدم, "prepare-commit-msg",
			    git_رستہ_ملاؤ_پیغام(the_repository), "merge", عدم))
		abort_commit(remoteheads, عدم);
	إذا (0 < option_edit) {
		إذا (launch_editor(git_رستہ_ملاؤ_پیغام(the_repository), عدم, عدم))
			abort_commit(remoteheads, عدم);
	}

	إذا (!no_verify && لگاتار_commit_hook(0 < option_edit, repo_get_اشاریہ_file(the_repository),
					  عدم, "commit-msg",
					  git_رستہ_ملاؤ_پیغام(the_repository), عدم))
		abort_commit(remoteheads, عدم);

	read_ملاؤ_پیغام(&پیغام);
	cleanup_سنیہا(&پیغام, cleanup_منوال, 0);
	إذا (!پیغام.len)
		abort_commit(remoteheads, _("Empty commit message."));
	strbuf_release(&ملاؤ_پیغام);
	strbuf_addbuf(&ملاؤ_پیغام, &پیغام);
	strbuf_release(&پیغام);
}

ساكن صحيح ملاؤ_trivial(بنية commit *سر, بنية commit_list *remoteheads)
{
	بنية object_id نتیجہ_درخت, نتیجہ_commit;
	بنية commit_list *parents = عدم, **pptr = &parents;

	إذا (repo_refresh_and_write_اشاریہ(the_repository, REFRESH_QUIET,
					 SKIP_IF_UNCHANGED, 0, عدم, عدم,
					 عدم) < 0)
		أرجع error(_("Unable to write index."));

	write_درخت_trivial(&نتیجہ_درخت);
	لکھو(_("Wonderful.\n"));
	pptr = commit_list_append(سر, pptr);
	pptr = commit_list_append(remoteheads->شے, pptr);
	prepare_منزل_commit(remoteheads);
	إذا (commit_درخت(ملاؤ_پیغام.buf, ملاؤ_پیغام.len, &نتیجہ_درخت, parents,
			&نتیجہ_commit, عدم, sign_commit))
		die(_("failed to write commit object"));
	انت(سر, remoteheads, &نتیجہ_commit, "In-index merge");

	remove_ملاؤ_branch_state(the_repository);
	commit_list_free(parents);
	أرجع 0;
}

ساكن صحيح انت_automerge(بنية commit *سر,
			    صحيح سر_subsumed,
			    بنية commit_list *common,
			    بنية commit_list *remoteheads,
			    بنية object_id *نتیجہ_درخت,
			    ثابت حرف *wt_strategy)
{
	بنية commit_list *parents = عدم;
	بنية strbuf buf = STRBUF_INIT;
	بنية object_id نتیجہ_commit;

	write_درخت_trivial(نتیجہ_درخت);
	commit_list_free(common);
	parents = remoteheads;
	إذا (!سر_subsumed || fast_forward == FF_NO)
		commit_list_داخل_کرو(سر, &parents);
	prepare_منزل_commit(remoteheads);
	إذا (commit_درخت(ملاؤ_پیغام.buf, ملاؤ_پیغام.len, نتیجہ_درخت, parents,
			&نتیجہ_commit, عدم, sign_commit))
		die(_("failed to write commit object"));
	strbuf_addf(&buf, "Merge made by the '%s' strategy.", wt_strategy);
	انت(سر, remoteheads, &نتیجہ_commit, buf.buf);

	strbuf_release(&buf);
	remove_ملاؤ_branch_state(the_repository);
	commit_list_free(parents);
	أرجع 0;
}

ساكن صحيح suggest_conflicts(فراغ)
{
	ثابت حرف *filename;
	FILE *fp;
	بنية strbuf msgbuf = STRBUF_INIT;

	filename = git_رستہ_ملاؤ_پیغام(the_repository);
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
	أرجع 1;
}

ساكن صحيح evaluate_نتیجہ(فراغ)
{
	صحيح cnt = 0;
	بنية rev_info rev;

	/* Check how many files differ. */
	repo_init_revisions(the_repository, &rev, "");
	تیاری_revisions(0, عدم, &rev, عدم);
	rev.diffopt.output_format |=
		DIFF_FORMAT_CALLBACK;
	rev.diffopt.format_callback = گنتی_diff_files;
	rev.diffopt.format_callback_انکڑے = &cnt;
	لگاتار_diff_files(&rev, 0);

	/*
	 * Check how many unmerged entries are
	 * there.
	 */
	cnt += گنتی_unmerged_entries();

	release_revisions(&rev);
	أرجع cnt;
}

/*
 * Pretend as if the user told us to merge with the remote-tracking
 * branch we have for the upstream of the current branch
 */
ساكن صحيح تیاری_with_upstream(ثابت حرف ***argv)
{
	بنية branch *branch = branch_get(عدم);
	صحيح i;
	ثابت حرف **دلیلاں;

	إذا (!branch)
		die(_("No current branch."));
	إذا (!branch->remote_name)
		die(_("No remote for the current branch."));
	إذا (!branch->ملاؤ_nr)
		die(_("No default upstream defined for the current branch."));

	دلیلاں = xcalloc(st_جوڑو(branch->ملاؤ_nr, 1), حجم(حرف *));
	لكل (i = 0; i < branch->ملاؤ_nr; i++) {
		إذا (!branch->ملاؤ[i]->dst)
			die(_("No remote-tracking branch for %s from %s"),
			    branch->ملاؤ[i]->src, branch->remote_name);
		دلیلاں[i] = branch->ملاؤ[i]->dst;
	}
	دلیلاں[i] = عدم;
	*argv = دلیلاں;
	أرجع i;
}

ساكن فراغ write_ملاؤ_heads(بنية commit_list *remoteheads)
{
	بنية commit_list *j;
	بنية strbuf buf = STRBUF_INIT;

	لكل (j = remoteheads; j; j = j->اگلا) {
		بنية object_id *oid;
		بنية commit *c = j->شے;
		بنية ملاؤ_remote_desc *desc;

		desc = ملاؤ_remote_util(c);
		إذا (desc && desc->obj) {
			oid = &desc->obj->oid;
		} وإلا {
			oid = &c->object.oid;
		}
		strbuf_addf(&buf, "%s\n", oid_منزل_hex(oid));
	}
	write_file_buf(git_رستہ_ملاؤ_سر(the_repository), buf.buf, buf.len);

	strbuf_reset(&buf);
	إذا (fast_forward == FF_NO)
		strbuf_addstr(&buf, "no-ff");
	write_file_buf(git_رستہ_ملاؤ_منوال(the_repository), buf.buf, buf.len);
	strbuf_release(&buf);
}

ساكن فراغ write_ملاؤ_state(بنية commit_list *remoteheads)
{
	write_ملاؤ_heads(remoteheads);
	strbuf_addch(&ملاؤ_پیغام, '\n');
	write_file_buf(git_رستہ_ملاؤ_پیغام(the_repository), ملاؤ_پیغام.buf,
		       ملاؤ_پیغام.len);
}

ساكن صحيح default_edit_option(فراغ)
{
	ساكن ثابت حرف name[] = "GIT_MERGE_AUTOEDIT";
	ثابت حرف *e = getenv(name);
	بنية stat st_stdin, st_stdout;

	إذا (have_سنیہا)
		/* an explicit -m msg without --[no-]edit */
		أرجع 0;

	إذا (e) {
		صحيح v = git_parse_maybe_bool(e);
		إذا (v < 0)
			die(_("Bad value '%s' in environment '%s'"), e, name);
		أرجع v;
	}

	/* Use editor if stdin and stdout are the same and is a tty */
	أرجع (!fstat(0, &st_stdin) &&
		!fstat(1, &st_stdout) &&
		isatty(0) && isatty(1) &&
		st_stdin.st_dev == st_stdout.st_dev &&
		st_stdin.st_ino == st_stdout.st_ino &&
		st_stdin.st_منوال == st_stdout.st_منوال);
}

ساكن بنية commit_list *reduce_parents(بنية commit *سر_commit,
					  صحيح *سر_subsumed,
					  بنية commit_list *remoteheads)
{
	بنية commit_list *parents, **remotes;

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

	remoteheads = عدم;
	remotes = &remoteheads;
	طالما (parents) {
		بنية commit *commit = نکالو_commit(&parents);
		إذا (commit == سر_commit)
			*سر_subsumed = 0;
		وإلا
			remotes = &commit_list_داخل_کرو(commit, remotes)->اگلا;
	}
	أرجع remoteheads;
}

ساكن فراغ prepare_ملاؤ_سنیہا(بنية strbuf *ملاؤ_ناں, بنية strbuf *ملاؤ_پیغام)
{
	بنية fmt_ملاؤ_پیغام_opts opts;

	memset(&opts, 0, حجم(opts));
	opts.جوڑو_title = !have_سنیہا;
	opts.shortlog_len = shortlog_len;
	opts.credit_people = (0 < option_edit);
	opts.into_name = into_name;

	fmt_ملاؤ_پیغام(ملاؤ_ناں, ملاؤ_پیغام, &opts);
	إذا (ملاؤ_پیغام->len)
		strbuf_setlen(ملاؤ_پیغام, ملاؤ_پیغام->len - 1);
}

ساكن فراغ handle_fetch_سر(بنية commit_list **remotes, بنية strbuf *ملاؤ_ناں)
{
	ثابت حرف *filename;
	صحيح fd, pos, npos;
	بنية strbuf fetch_سر_file = STRBUF_INIT;
	ثابت بنانشانی hexsz = the_hash_algo->hexsz;

	إذا (!ملاؤ_ناں)
		ملاؤ_ناں = &fetch_سر_file;

	filename = git_رستہ_fetch_سر(the_repository);
	fd = xopen(filename, O_RDONLY);

	إذا (strbuf_read(ملاؤ_ناں, fd, 0) < 0)
		die_errno(_("could not read '%s'"), filename);
	إذا (close(fd) < 0)
		die_errno(_("could not close '%s'"), filename);

	لكل (pos = 0; pos < ملاؤ_ناں->len; pos = npos) {
		بنية object_id oid;
		حرف *ptr;
		بنية commit *commit;

		ptr = strchr(ملاؤ_ناں->buf + pos, '\n');
		إذا (ptr)
			npos = ptr - ملاؤ_ناں->buf + 1;
		وإلا
			npos = ملاؤ_ناں->len;

		إذا (npos - pos < hexsz + 2 ||
		    get_oid_hex(ملاؤ_ناں->buf + pos, &oid))
			commit = عدم; /* bad */
		وإلا إذا (memcmp(ملاؤ_ناں->buf + pos + hexsz, "\t\t", 2))
			استمر; /* not-for-merge */
		وإلا {
			حرف saved = ملاؤ_ناں->buf[pos + hexsz];
			ملاؤ_ناں->buf[pos + hexsz] = '\0';
			commit = get_ملاؤ_ماپا(ملاؤ_ناں->buf + pos);
			ملاؤ_ناں->buf[pos + hexsz] = saved;
		}
		إذا (!commit) {
			إذا (ptr)
				*ptr = '\0';
			die(_("not something we can merge in %s: %s"),
			    filename, ملاؤ_ناں->buf + pos);
		}
		remotes = &commit_list_داخل_کرو(commit, remotes)->اگلا;
	}

	إذا (ملاؤ_ناں == &fetch_سر_file)
		strbuf_release(&fetch_سر_file);
}

ساكن بنية commit_list *collect_parents(بنية commit *سر_commit,
					   صحيح *سر_subsumed,
					   صحيح argc, ثابت حرف **argv,
					   بنية strbuf *ملاؤ_پیغام)
{
	صحيح i;
	بنية commit_list *remoteheads = عدم;
	بنية commit_list **remotes = &remoteheads;
	بنية strbuf ملاؤ_ناں = STRBUF_INIT, *autogen = عدم;

	إذا (ملاؤ_پیغام && (!have_سنیہا || shortlog_len))
		autogen = &ملاؤ_ناں;

	إذا (سر_commit)
		remotes = &commit_list_داخل_کرو(سر_commit, remotes)->اگلا;

	إذا (argc == 1 && !strcmp(argv[0], "FETCH_HEAD")) {
		handle_fetch_سر(remotes, autogen);
		remoteheads = reduce_parents(سر_commit, سر_subsumed, remoteheads);
	} وإلا {
		لكل (i = 0; i < argc; i++) {
			بنية commit *commit = get_ملاؤ_ماپا(argv[i]);
			إذا (!commit)
				help_unknown_ref(argv[i], "merge",
						 _("not something we can merge"));
			remotes = &commit_list_داخل_کرو(commit, remotes)->اگلا;
		}
		remoteheads = reduce_parents(سر_commit, سر_subsumed, remoteheads);
		إذا (autogen) {
			بنية commit_list *p;
			لكل (p = remoteheads; p; p = p->اگلا)
				ملاؤ_name(ملاؤ_remote_util(p->شے)->name, autogen);
		}
	}

	إذا (autogen) {
		prepare_ملاؤ_سنیہا(autogen, ملاؤ_پیغام);
		strbuf_release(autogen);
	}

	أرجع remoteheads;
}

ساكن صحيح merging_a_throwaway_ٹیگ(بنية commit *commit)
{
	حرف *ٹیگ_ref;
	بنية object_id oid;
	صحيح is_throwaway_ٹیگ = 0;

	/* Are we merging a tag? */
	إذا (!ملاؤ_remote_util(commit) ||
	    !ملاؤ_remote_util(commit)->obj ||
	    ملاؤ_remote_util(commit)->obj->type != OBJ_ٹیگ)
		أرجع is_throwaway_ٹیگ;

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
	ٹیگ_ref = xstrfmt("refs/tags/%s",
			  ((بنية ٹیگ *)ملاؤ_remote_util(commit)->obj)->ٹیگ);
	إذا (!refs_read_ref(get_main_ref_store(the_repository), ٹیگ_ref, &oid) &&
	    oideq(&oid, &ملاؤ_remote_util(commit)->obj->oid))
		is_throwaway_ٹیگ = 0;
	وإلا
		is_throwaway_ٹیگ = 1;
	حرر(ٹیگ_ref);
	أرجع is_throwaway_ٹیگ;
}

صحيح cmd_ملاؤ(صحيح argc,
	      ثابت حرف **argv,
	      ثابت حرف *اگیتر,
	      بنية repository *repo بن_ورتیا)
{
	بنية object_id نتیجہ_درخت, stash, سر_oid;
	بنية commit *سر_commit;
	بنية strbuf buf = STRBUF_INIT;
	صحيح i, ret = 0, سر_subsumed;
	صحيح بہترین_cnt = -1, ملاؤ_was_ok = 0, automerge_was_ok = 0;
	بنية commit_list *common = عدم;
	ثابت حرف *بہترین_strategy = عدم, *wt_strategy = عدم;
	بنية commit_list *remoteheads = عدم, *p;
	فراغ *branch_منزل_free, *argv_منزل_free = عدم;
	صحيح orig_argc = argc;
	صحيح ملاؤ_log_config = -1;

	show_usage_with_options_if_asked(argc, argv,
					 builtin_ملاؤ_usage, builtin_ملاؤ_options);

#جے_بنا_تعریف WITH_BREAKING_CHANGES
	warn_on_auto_comment_char = true;
#جے_انت /* !WITH_BREAKING_CHANGES */
	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_بھریا_اشاریہ = 0;

	/*
	 * Check if we are _not_ on a detached HEAD, i.e. if there is a
	 * current branch.
	 */
	branch = branch_منزل_free = refs_resolve_refdup(get_main_ref_store(the_repository),
						      "HEAD", 0, &سر_oid,
						      عدم);
	إذا (branch)
		skip_اگیتر(branch, "refs/heads/", &branch);

	init_diff_ui_defaults();
	repo_config(the_repository, git_ملاؤ_config, &ملاؤ_log_config);

	إذا (!branch || is_null_oid(&سر_oid))
		سر_commit = عدم;
	وإلا
		سر_commit = lookup_commit_or_die(&سر_oid, "HEAD");

	إذا (branch_mergeoptions)
		parse_branch_ملاؤ_options(branch_mergeoptions);
	argc = parse_options(argc, argv, اگیتر, builtin_ملاؤ_options,
			builtin_ملاؤ_usage, 0);
	إذا (shortlog_len < 0)
		shortlog_len = (ملاؤ_log_config > 0) ? ملاؤ_log_config : 0;

	إذا (verbosity < 0 && show_ترقی == -1)
		show_ترقی = 0;

	إذا (abort_current_ملاؤ) {
		صحيح nargc = 2;
		ثابت حرف *nargv[] = {"reset", "--merge", عدم};
		حرف stash_oid_hex[GIT_MAX_HEXSZ + 1];
		بنية object_id stash_oid = {0};

		إذا (orig_argc != 2)
			usage_پیغام_opt(_("--abort expects no arguments"),
			      builtin_ملاؤ_usage, builtin_ملاؤ_options);

		إذا (!file_exists(git_رستہ_ملاؤ_سر(the_repository)))
			die(_("There is no merge to abort (MERGE_HEAD missing)."));

		إذا (!refs_read_ref(get_main_ref_store(the_repository), "MERGE_AUTOSTASH", &stash_oid))
			refs_delete_ref(get_main_ref_store(the_repository),
					"", "MERGE_AUTOSTASH", &stash_oid,
					REF_NO_DEREF);

		/* Invoke 'git reset --merge' */
		ret = cmd_reset(nargc, nargv, اگیتر, the_repository);

		إذا (!is_null_oid(&stash_oid)) {
			oid_منزل_hex_r(stash_oid_hex, &stash_oid);
			apply_autostash_oid(stash_oid_hex);
		}

		اذهب پورا;
	}

	إذا (quit_current_ملاؤ) {
		إذا (orig_argc != 2)
			usage_پیغام_opt(_("--quit expects no arguments"),
				      builtin_ملاؤ_usage,
				      builtin_ملاؤ_options);

		remove_ملاؤ_branch_state(the_repository);
		اذهب پورا;
	}

	إذا (continue_current_ملاؤ) {
		صحيح nargc = 1;
		ثابت حرف *nargv[] = {"commit", عدم};

		إذا (orig_argc != 2)
			usage_پیغام_opt(_("--continue expects no arguments"),
			      builtin_ملاؤ_usage, builtin_ملاؤ_options);

		إذا (!file_exists(git_رستہ_ملاؤ_سر(the_repository)))
			die(_("There is no merge in progress (MERGE_HEAD missing)."));

		/* Invoke 'git commit' */
		ret = cmd_commit(nargc, nargv, اگیتر, the_repository);
		اذهب پورا;
	}

	إذا (repo_read_اشاریہ_unmerged(the_repository))
		die_resolve_conflict("merge");

	إذا (file_exists(git_رستہ_ملاؤ_سر(the_repository))) {
		/*
		 * There is no unmerged entry, don't advise 'git
		 * add/rm <file>', just 'git commit'.
		 */
		إذا (advice_enabled(ADVICE_RESOLVE_CONFLICT))
			die(_("You have not concluded your merge (MERGE_HEAD exists).\n"
				  "Please, commit your changes before you merge."));
		وإلا
			die(_("You have not concluded your merge (MERGE_HEAD exists)."));
	}
	إذا (refs_ref_exists(get_main_ref_store(the_repository), "CHERRY_PICK_HEAD")) {
		إذا (advice_enabled(ADVICE_RESOLVE_CONFLICT))
			die(_("You have not concluded your cherry-pick (CHERRY_PICK_HEAD exists).\n"
			    "Please, commit your changes before you merge."));
		وإلا
			die(_("You have not concluded your cherry-pick (CHERRY_PICK_HEAD exists)."));
	}
	resolve_undo_clear_اشاریہ(the_repository->اشاریہ);

	إذا (option_edit < 0)
		option_edit = default_edit_option();

	cleanup_منوال = get_cleanup_منوال(cleanup_arg, 0 < option_edit);

	إذا (verbosity < 0)
		show_diffstat = 0;

	إذا (squash) {
		إذا (fast_forward == FF_NO)
			die(_("options '%s' and '%s' cannot be used together"), "--squash", "--no-ff.");
		إذا (option_commit > 0)
			die(_("options '%s' and '%s' cannot be used together"), "--squash", "--commit.");
		/*
		 * squash can now silently disable option_commit - this is not
		 * a problem as it is only overriding the default, not a user
		 * supplied option.
		 */
		option_commit = 0;
	}

	إذا (option_commit < 0)
		option_commit = 1;

	إذا (!argc) {
		إذا (default_منزل_upstream) {
			argc = تیاری_with_upstream(&argv);
			argv_منزل_free = argv;
		}
		وإلا
			die(_("No commit specified and merge.defaultToUpstream not set."));
	} وإلا إذا (argc == 1 && !strcmp(argv[0], "-")) {
		argv[0] = "@{-1}";
	}

	إذا (!argc)
		usage_with_options(builtin_ملاؤ_usage,
			builtin_ملاؤ_options);

	إذا (!سر_commit) {
		/*
		 * If the merged head is a valid one there is no reason
		 * to forbid "git merge" into a branch yet to be born.
		 * We do the same for "git pull".
		 */
		بنية object_id *remote_سر_oid;
		إذا (squash)
			die(_("Squash commit into empty head not supported yet"));
		إذا (fast_forward == FF_NO)
			die(_("Non-fast-forward commit does not make sense into "
			    "an empty head"));
		remoteheads = collect_parents(سر_commit, &سر_subsumed,
					      argc, argv, عدم);
		إذا (!remoteheads)
			die(_("%s - not something we can merge"), argv[0]);
		إذا (remoteheads->اگلا)
			die(_("Can merge only exactly one commit into empty head"));

		إذا (verify_signatures)
			verify_ملاؤ_signature(remoteheads->شے, verbosity,
					       check_trust_level);

		remote_سر_oid = &remoteheads->شے->object.oid;
		read_خالی_تھاواں(remote_سر_oid);
		refs_update_ref(get_main_ref_store(the_repository),
				"initial pull", "HEAD", remote_سر_oid, عدم,
				0,
				UPDATE_REFS_DIE_ON_ERR);
		اذهب پورا;
	}

	/*
	 * All the rest are the commits being merged; prepare
	 * the standard merge summary message to be appended
	 * to the given message.
	 */
	remoteheads = collect_parents(سر_commit, &سر_subsumed,
				      argc, argv, &ملاؤ_پیغام);

	إذا (!سر_commit || !argc)
		usage_with_options(builtin_ملاؤ_usage,
			builtin_ملاؤ_options);

	إذا (verify_signatures) {
		لكل (p = remoteheads; p; p = p->اگلا) {
			verify_ملاؤ_signature(p->شے, verbosity,
					       check_trust_level);
		}
	}

	strbuf_addstr(&buf, "merge");
	لكل (p = remoteheads; p; p = p->اگلا)
		strbuf_addf(&buf, " %s", ملاؤ_remote_util(p->شے)->name);
	setenv("GIT_REFLOG_ACTION", buf.buf, 0);
	strbuf_reset(&buf);

	لكل (p = remoteheads; p; p = p->اگلا) {
		بنية commit *commit = p->شے;
		strbuf_addf(&buf, "GITHEAD_%s",
			    oid_منزل_hex(&commit->object.oid));
		setenv(buf.buf, ملاؤ_remote_util(commit)->name, 1);
		strbuf_reset(&buf);
		إذا (fast_forward != FF_ONLY && merging_a_throwaway_ٹیگ(commit))
			fast_forward = FF_NO;
	}

	إذا (!use_strategies) {
		إذا (!remoteheads)
			; /* already up-to-date */
		وإلا إذا (!remoteheads->اگلا)
			جوڑو_strategies(pull_twohead, DEFAULT_TWOHEAD);
		وإلا
			جوڑو_strategies(pull_octopus, DEFAULT_OCTOPUS);
	}

	لكل (i = 0; i < use_strategies_nr; i++) {
		إذا (use_strategies[i]->attr & NO_FAST_FORWARD)
			fast_forward = FF_NO;
		إذا (use_strategies[i]->attr & NO_TRIVIAL)
			allow_trivial = 0;
	}

	إذا (!remoteheads)
		; /* already up-to-date */
	وإلا إذا (!remoteheads->اگلا) {
		إذا (repo_get_ملاؤ_bases(the_repository, سر_commit,
					 remoteheads->شے, &common) < 0) {
			ret = 2;
			اذهب پورا;
		}
	} وإلا {
		بنية commit_list *list = remoteheads;
		commit_list_داخل_کرو(سر_commit, &list);
		إذا (get_octopus_ملاؤ_bases(list, &common) < 0) {
			حرر(list);
			ret = 2;
			اذهب پورا;
		}
		حرر(list);
	}

	refs_update_ref(get_main_ref_store(the_repository),
			"updating ORIG_HEAD", "ORIG_HEAD",
			&سر_commit->object.oid, عدم, 0,
			UPDATE_REFS_DIE_ON_ERR);

	إذا (remoteheads && !common) {
		/* No common ancestors found. */
		إذا (!allow_unrelated_histories)
			die(_("refusing to merge unrelated histories"));
		/* otherwise, we need a real merge. */
	} وإلا إذا (!remoteheads ||
		 (!remoteheads->اگلا && !common->اگلا &&
		  common->شے == remoteheads->شے)) {
		/*
		 * If head can reach all the merge then we are up to date.
		 * but first the most common case of merging one remote.
		 */
		انت_اتے_منزل_date();
		اذهب پورا;
	} وإلا إذا (fast_forward != FF_NO && !remoteheads->اگلا &&
			!common->اگلا &&
			oideq(&common->شے->object.oid, &سر_commit->object.oid)) {
		/* Again the most common case of merging one remote. */
		ثابت حرف *پیغام = have_سنیہا ?
			"Fast-forward (no commit created; -m option ignored)" :
			"Fast-forward";
		بنية commit *commit;

		إذا (verbosity >= 0) {
			لکھو(_("Updating %s..%s\n"),
			       repo_لبھو_unique_abbrev(the_repository, &سر_commit->object.oid,
						       DEFAULT_ABBREV),
			       repo_لبھو_unique_abbrev(the_repository, &remoteheads->شے->object.oid,
						       DEFAULT_ABBREV));
		}
		commit = remoteheads->شے;
		إذا (!commit) {
			ret = 1;
			اذهب پورا;
		}

		إذا (autostash)
			create_autostash_ref(the_repository, "MERGE_AUTOSTASH",
					     عدم, false);
		إذا (checkout_fast_forward(the_repository,
					  &سر_commit->object.oid,
					  &commit->object.oid,
					  overwrite_ignore)) {
			apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
					    عدم, عدم, عدم, عدم);
			ret = 1;
			اذهب پورا;
		}

		انت(سر_commit, remoteheads, &commit->object.oid, پیغام);
		remove_ملاؤ_branch_state(the_repository);
		اذهب پورا;
	} وإلا إذا (!remoteheads->اگلا && common->اگلا)
		;
		/*
		 * We are not doing octopus and not fast-forward.  Need
		 * a real merge.
		 */
	وإلا إذا (!remoteheads->اگلا && !common->اگلا && option_commit) {
		/*
		 * We are not doing octopus, not fast-forward, and have
		 * only one common.
		 */
		refresh_اشاریہ(the_repository->اشاریہ, REFRESH_QUIET, عدم, عدم, عدم);
		إذا (allow_trivial && fast_forward != FF_ONLY) {
			/*
			 * Must first ensure that index matches HEAD before
			 * attempting a trivial merge.
			 */
			بنية درخت *سر_درخت = repo_get_commit_درخت(the_repository,
								      سر_commit);
			بنية strbuf sb = STRBUF_INIT;

			إذا (repo_اشاریہ_has_changes(the_repository, سر_درخت,
						   &sb)) {
				error(_("Your local changes to the following files would be overwritten by merge:\n  %s"),
				      sb.buf);
				strbuf_release(&sb);
				ret = 2;
				اذهب پورا;
			}

			/* See if it is really trivial. */
			git_committer_info(IDENT_STRICT);
			لکھو(_("Trying really trivial in-index merge...\n"));
			إذا (!read_درخت_trivial(&common->شے->object.oid,
					       &سر_commit->object.oid,
					       &remoteheads->شے->object.oid)) {
				ret = ملاؤ_trivial(سر_commit, remoteheads);
				اذهب پورا;
			}
			لکھو(_("Nope.\n"));
		}
	} وإلا {
		/*
		 * An octopus.  If we can reach all the remote we are up
		 * to date.
		 */
		صحيح اتے_منزل_date = 1;
		بنية commit_list *j;

		لكل (j = remoteheads; j; j = j->اگلا) {
			صحيح ret = repo_وچ_ملاؤ_bases(the_repository,
						      j->شے, سر_commit);
			إذا (ret < 0)
				باہر_نکلو(128);
			إذا (!ret) {
				اتے_منزل_date = 0;
				اكسر;
			}
		}
		إذا (اتے_منزل_date) {
			انت_اتے_منزل_date();
			اذهب پورا;
		}
	}

	إذا (fast_forward == FF_ONLY)
		die_ff_impossible();

	إذا (autostash)
		create_autostash_ref(the_repository, "MERGE_AUTOSTASH",
				     عدم, false);

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
	إذا (save_state(&stash))
		oidclr(&stash, the_repository->hash_algo);

	لكل (i = 0; i < use_strategies_nr; i++) {
		صحيح ret, cnt;
		إذا (i) {
			لکھو(_("Rewinding the tree to pristine...\n"));
			restore_state(&سر_commit->object.oid, &stash);
		}
		إذا (use_strategies_nr != 1)
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
		إذا (ret < 2) {
			إذا (!ret) {
				/*
				 * This strategy worked; no point in trying
				 * another.
				 */
				ملاؤ_was_ok = 1;
				بہترین_strategy = wt_strategy;
				اكسر;
			}
			cnt = (use_strategies_nr > 1) ? evaluate_نتیجہ() : 0;
			إذا (بہترین_cnt <= 0 || cnt <= بہترین_cnt) {
				بہترین_strategy = wt_strategy;
				بہترین_cnt = cnt;
			}
		}
	}

	/*
	 * If we have a resulting tree, that means the strategy module
	 * auto resolved the merge cleanly.
	 */
	إذا (ملاؤ_was_ok && option_commit) {
		automerge_was_ok = 1;
		ret = انت_automerge(سر_commit, سر_subsumed,
				       common, remoteheads,
				       &نتیجہ_درخت, wt_strategy);
		اذهب پورا;
	}

	/*
	 * Pick the result from the best strategy and have the user fix
	 * it up.
	 */
	إذا (!بہترین_strategy) {
		restore_state(&سر_commit->object.oid, &stash);
		إذا (use_strategies_nr > 1)
			fprintf(stderr,
				_("No merge strategy handled the merge.\n"));
		وإلا
			fprintf(stderr, _("Merge with strategy %s failed.\n"),
				use_strategies[0]->name);
		apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
				    عدم, عدم, عدم, عدم);
		ret = 2;
		اذهب پورا;
	} وإلا إذا (بہترین_strategy == wt_strategy)
		; /* We already have its result in the working tree. */
	وإلا {
		لکھو(_("Rewinding the tree to pristine...\n"));
		restore_state(&سر_commit->object.oid, &stash);
		لکھو(_("Using the %s strategy to prepare resolving by hand.\n"),
			بہترین_strategy);
		try_ملاؤ_strategy(بہترین_strategy, common, remoteheads,
				   سر_commit);
	}

	إذا (squash) {
		انت(سر_commit, remoteheads, عدم, عدم);

		git_test_write_commit_گراف_or_die(the_repository);
	} وإلا
		write_ملاؤ_state(remoteheads);

	إذا (ملاؤ_was_ok)
		fprintf(stderr, _("Automatic merge went well; "
			"stopped before committing as requested\n"));
	وإلا
		ret = suggest_conflicts();
	إذا (autostash)
		لکھو(_("When finished, apply stashed changes with `git stash pop`\n"));

پورا:
	إذا (!automerge_was_ok) {
		commit_list_free(common);
		commit_list_free(remoteheads);
	}
	strbuf_release(&buf);
	حرر(branch_منزل_free);
	حرر(argv_منزل_free);
	حرر(pull_twohead);
	حرر(pull_octopus);
	discard_اشاریہ(the_repository->اشاریہ);
	أرجع ret;
}
