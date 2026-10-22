/*
 * Builtin "git merge"
 *
 * Copyright (c) 2008 Miklos Vajna <vmiklos@frugalware.org>
 *
 * Based on git-merge.sh by Junio C Hamano.
 */

#وصف_ڪريو USE_THE_REPOSITORY_VARIABLE
#وصف_ڪريو DISABLE_SIGN_COMPARE_WARNINGS

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

#وصف_ڪريو DEFAULT_TWOHEAD (1<<0)
#وصف_ڪريو DEFAULT_OCTOPUS (1<<1)
#وصف_ڪريو NO_FAST_FORWARD (1<<2)
#وصف_ڪريو NO_TRIVIAL      (1<<3)

بنية strategy {
	ثابت حرف *name;
	بنانشان attr;
};

ساكن ثابت حرف * ثابت builtin_ملايو_usage[] = {
	N_("git merge [<options>] [<commit>...]"),
	"git merge --abort",
	"git merge --continue",
	عدم
};

#وصف_ڪريو ملايو_SHOW_DIFFSTAT 1
#وصف_ڪريو ملايو_SHOW_COMPACTSUMMARY 2

ساكن صحيح show_diffstat = ملايو_SHOW_DIFFSTAT, shortlog_len = -1, squash;
ساكن صحيح option_commit = -1;
ساكن صحيح option_edit = -1;
ساكن صحيح allow_trivial = 1, have_پيغام, verify_signatures;
ساكن صحيح check_trust_level = 1;
ساكن صحيح overwrite_ignore = 1;
ساكن بنية strbuf ملايو_نياپو = STRBUF_INIT;
ساكن بنية strategy **use_strategies;
ساكن سائيز_t use_strategies_nr, use_strategies_alloc;
ساكن بنية strvec xopts = STRVEC_INIT;
ساكن ثابت حرف *branch;
ساكن حرف *branch_mergeoptions;
ساكن صحيح verbosity;
ساكن صحيح allow_rerere_auto;
ساكن صحيح abort_current_ملايو;
ساكن صحيح quit_current_ملايو;
ساكن صحيح continue_current_ملايو;
ساكن صحيح allow_unrelated_histories;
ساكن صحيح show_ترقي = -1;
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
ساكن تعداد commit_نياپو_cleanup_گهڻائي cleanup_گهڻائي;

ساكن صحيح option_parse_پيغام(ثابت بنية option *opt,
				ثابت حرف *arg, صحيح unset)
{
	بنية strbuf *buf = opt->قدر;

	إذا (unset)
		strbuf_setlen(buf, 0);
	وإلا إذا (arg) {
		strbuf_addf(buf, "%s%s", buf->len ? "\n\n" : "", arg);
		have_پيغام = 1;
	} وإلا
		أرجع error(_("switch `m' requires a value"));
	أرجع 0;
}

ساكن تعداد parse_opt_نتيجو option_read_پيغام(بنية parse_opt_ctx_t *ctx,
						 ثابت بنية option *opt,
						 ثابت حرف *arg_not_استعمال_ٿيل,
						 صحيح unset)
{
	بنية strbuf *buf = opt->قدر;
	ثابت حرف *arg;

	BUG_ON_OPT_ARG(arg_not_استعمال_ٿيل);
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
	إذا (ctx->اڳياڙي && !is_absolute_رستو(arg))
		arg = اڳياڙي_filename(ctx->اڳياڙي, arg);
	إذا (strbuf_read_file(buf, arg, 0) < 0)
		أرجع error(_("could not read file '%s'"), arg);
	have_پيغام = 1;

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

	لكل (i = 0; i < ARRAY_سائيز(all_strategy); i++)
		إذا (!strcmp(name, all_strategy[i].name))
			أرجع &all_strategy[i];

	إذا (!loaded) {
		بنية cmdnames not_strategies = {0};
		loaded = 1;

		ڀريو_command_list("git-merge-", &main_cmds, &other_cmds);
		لكل (i = 0; i < main_cmds.cnt; i++) {
			صحيح j, مليو_2 = 0;
			بنية cmdname *ent = main_cmds.نالا[i];
			لكل (j = 0; !مليو_2 && j < ARRAY_سائيز(all_strategy); j++)
				إذا (!xstrncmpz(all_strategy[j].name, ent->name, ent->len))
					مليو_2 = 1;
			إذا (!مليو_2)
				جوڙيو_cmdname(&not_strategies, ent->name, ent->len);
		}
		exclude_cmds(&main_cmds, &not_strategies);

		cmdnames_release(&not_strategies);
	}
	إذا (!is_اندريون_cmdlist(&main_cmds, name) && !is_اندريون_cmdlist(&other_cmds, name)) {
		fprintf(stderr, _("Could not find merge strategy '%s'.\n"), name);
		fprintf(stderr, _("Available strategies are:"));
		لكل (i = 0; i < main_cmds.cnt; i++)
			fprintf(stderr, " %s", main_cmds.نالا[i]->name);
		fprintf(stderr, ".\n");
		إذا (other_cmds.cnt) {
			fprintf(stderr, _("Available custom strategies are:"));
			لكل (i = 0; i < other_cmds.cnt; i++)
				fprintf(stderr, " %s", other_cmds.نالا[i]->name);
			fprintf(stderr, ".\n");
		}
		ٻاهر_نڪرو(1);
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

ساكن صحيح option_parse_strategy(ثابت بنية option *opt اڻ_استعمال_ٿيل,
				 ثابت حرف *name, صحيح unset)
{
	إذا (unset)
		أرجع 0;

	append_strategy(get_strategy(name));
	أرجع 0;
}

ساكن صحيح option_parse_compact_summary(ثابت بنية option *opt,
					ثابت حرف *name اڻ_استعمال_ٿيل, صحيح unset)
{
	صحيح *setting = opt->قدر;

	إذا (unset)
		*setting = 0;
	وإلا
		*setting = ملايو_SHOW_COMPACTSUMMARY;
	أرجع 0;
}

ساكن بنية option builtin_ملايو_options[] = {
	OPT_سيٽ_INT('n', عدم, &show_diffstat,
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
		.defval = DEFAULT_ملايو_LOG_LEN,
	},
	OPT_BOOL(0, "squash", &squash,
		N_("create a single commit instead of doing a merge")),
	OPT_BOOL(0, "commit", &option_commit,
		N_("perform a commit if the merge succeeds (default)")),
	OPT_BOOL('e', "edit", &option_edit,
		N_("edit message before committing")),
	OPT_CLEANUP(&cleanup_arg),
	OPT_سيٽ_INT(0, "ff", &fast_forward, N_("allow fast-forward (default)"), FF_ALLOW),
	OPT_سيٽ_INT_F(0, "ff-only", &fast_forward,
		      N_("abort if fast-forward is not possible"),
		      FF_ONLY, PARSE_OPT_NONEG),
	OPT_RERERE_AUTOUPDATE(&allow_rerere_auto),
	OPT_BOOL(0, "verify-signatures", &verify_signatures,
		N_("verify that the named commit has a valid GPG signature")),
	OPT_CALLBACK('s', "strategy", عدم, N_("strategy"),
		N_("merge strategy to use"), option_parse_strategy),
	OPT_STRVEC('X', "strategy-option", &xopts, N_("option=value"),
		N_("option for selected merge strategy")),
	OPT_CALLBACK('m', "message", &ملايو_نياپو, N_("message"),
		N_("merge commit message (for a non-fast-forward merge)"),
		option_parse_پيغام),
	{
		.type = OPTION_LOWLEVEL_CALLBACK,
		.short_name = 'F',
		.long_name = "file",
		.قدر = &ملايو_نياپو,
		.argh = N_("path"),
		.help = N_("read message from file"),
		.flags = PARSE_OPT_NONEG,
		.ll_callback = option_read_پيغام,
	},
	OPT_STRING(0, "into-name", &into_name, N_("name"),
		   N_("use <name> instead of the real target")),
	OPT__VERBOSITY(&verbosity),
	OPT_BOOL(0, "abort", &abort_current_ملايو,
		N_("abort the current in-progress merge")),
	OPT_BOOL(0, "quit", &quit_current_ملايو,
		N_("--abort but leave index and working tree alone")),
	OPT_BOOL(0, "continue", &continue_current_ملايو,
		N_("continue the current in-progress merge")),
	OPT_BOOL(0, "allow-unrelated-histories", &allow_unrelated_histories,
		 N_("allow merging unrelated histories")),
	OPT_سيٽ_INT(0, "progress", &show_ترقي, N_("force progress reporting"), 1),
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
	OPT_پڇاڙي()
};

ساكن صحيح save_state(بنية object_id *stash)
{
	صحيح len;
	بنية ٻار_process cp = ٻار_PROCESS_INIT;
	بنية strbuf بفر = STRBUF_INIT;
	بنية تالو_file تالو_file = تالو_INIT;
	صحيح fd;
	صحيح rc = -1;

	fd = repo_hold_locked_اشاريو(the_repository, &تالو_file, 0);
	refresh_اشاريو(the_repository->اشاريو, REFRESH_QUIET, عدم, عدم, عدم);
	إذا (0 <= fd)
		repo_update_اشاريو_if_able(the_repository, &تالو_file);
	rollback_تالو_file(&تالو_file);

	strvec_pushl(&cp.دليلون, "stash", "create", عدم);
	cp.ٻاهريون = -1;
	cp.git_cmd = 1;

	إذا (شروعات_command(&cp))
		die(_("could not run stash."));
	len = strbuf_read(&بفر, cp.ٻاهريون, 1024);
	close(cp.ٻاهريون);

	إذا (پڄاڻي_command(&cp) || len < 0)
		die(_("stash failed"));
	وإلا إذا (!len)		/* no changes */
		اذهب ٻاهريون;
	strbuf_setlen(&بفر, بفر.len-1);
	إذا (repo_get_oid(the_repository, بفر.buf, stash))
		die(_("not a valid object: %s"), بفر.buf);
	rc = 0;
ٻاهريون:
	strbuf_release(&بفر);
	أرجع rc;
}

ساكن فراغ read_خالي_جايون(ثابت بنية object_id *oid)
{
	بنية ٻار_process cmd = ٻار_PROCESS_INIT;

	strvec_pushl(&cmd.دليلون, "read-tree", "-m", "-u",
		     خالي_جايون_وڻ_oid_hex(the_repository->hash_algo),
		     oid_منزل_hex(oid), عدم);
	cmd.git_cmd = 1;

	إذا (لاڳيتو_command(&cmd))
		die(_("read-tree failed"));
}

ساكن فراغ reset_hard(ثابت بنية object_id *oid)
{
	بنية ٻار_process cmd = ٻار_PROCESS_INIT;

	strvec_pushl(&cmd.دليلون, "read-tree", "-v", "--reset", "-u",
		     oid_منزل_hex(oid), عدم);
	cmd.git_cmd = 1;

	إذا (لاڳيتو_command(&cmd))
		die(_("read-tree failed"));
}

ساكن فراغ restore_state(ثابت بنية object_id *سر,
			  ثابت بنية object_id *stash)
{
	بنية ٻار_process cmd = ٻار_PROCESS_INIT;

	reset_hard(سر);

	إذا (is_null_oid(stash))
		اذهب refresh_cache;

	strvec_pushl(&cmd.دليلون, "stash", "apply", "--index", "--quiet", عدم);
	strvec_ڌڪو(&cmd.دليلون, oid_منزل_hex(stash));

	/*
	 * It is OK to ignore error here, for example when there was
	 * nothing to restore.
	 */
	cmd.git_cmd = 1;
	لاڳيتو_command(&cmd);

refresh_cache:
	discard_اشاريو(the_repository->اشاريو);
	إذا (repo_read_اشاريو(the_repository) < 0)
		die(_("could not read index"));
}

/* This is called when no merge was necessary. */
ساكن فراغ پڄاڻي_مٿي_منزل_date(فراغ)
{
	إذا (verbosity >= 0) {
		إذا (squash)
			puts(_("Already up to date. (nothing to squash)"));
		وإلا
			puts(_("Already up to date."));
	}
	remove_ملايو_branch_state(the_repository);
}

ساكن فراغ squash_پيغام(بنية commit *commit, بنية commit_list *remoteheads)
{
	بنية rev_info rev;
	بنية strbuf ٻاهريون = STRBUF_INIT;
	بنية commit_list *j;
	بنية pretty_print_context ctx = {0};

	لکو(_("Squash commit -- not updating HEAD\n"));

	repo_init_revisions(the_repository, &rev, عدم);
	diff_merges_suppress(&rev);
	rev.commit_format = CMIT_FMT_MEDIUM;

	commit->object.flags |= UNINTERESTING;
	جوڙيو_pending_object(&rev, &commit->object, عدم);

	لكل (j = remoteheads; j; j = j->ايندڙ)
		جوڙيو_pending_object(&rev, &j->شي->object, عدم);

	تياري_revisions(0, عدم, &rev, عدم);
	إذا (prepare_revision_walk(&rev))
		die(_("revision walk setup failed"));

	ctx.abbrev = rev.abbrev;
	ctx.date_گهڻائي = rev.date_گهڻائي;
	ctx.fmt = rev.commit_format;

	strbuf_addstr(&ٻاهريون, "Squashed commit of the following:\n");
	طالما ((commit = get_revision(&rev)) != عدم) {
		strbuf_addch(&ٻاهريون, '\n');
		strbuf_addf(&ٻاهريون, "commit %s\n",
			oid_منزل_hex(&commit->object.oid));
		pretty_print_commit(&ctx, commit, &ٻاهريون);
	}
	write_file_buf(git_رستو_squash_نياپو(the_repository), ٻاهريون.buf, ٻاهريون.len);
	strbuf_release(&ٻاهريون);
	release_revisions(&rev);
}

ساكن فراغ پڄاڻي(بنية commit *سر_commit,
		   بنية commit_list *remoteheads,
		   ثابت بنية object_id *new_سر, ثابت حرف *نياپو)
{
	بنية strbuf reflog_پيغام = STRBUF_INIT;
	ثابت بنية object_id *سر = &سر_commit->object.oid;

	إذا (!نياپو)
		strbuf_addstr(&reflog_پيغام, getenv("GIT_REFLOG_ACTION"));
	وإلا {
		إذا (verbosity >= 0)
			لکو("%s\n", نياپو);
		strbuf_addf(&reflog_پيغام, "%s: %s",
			getenv("GIT_REFLOG_ACTION"), نياپو);
	}
	إذا (squash) {
		squash_پيغام(سر_commit, remoteheads);
	} وإلا {
		إذا (verbosity >= 0 && !ملايو_نياپو.len)
			لکو(_("No merge message -- not updating HEAD\n"));
		وإلا {
			refs_update_ref(get_main_ref_store(the_repository),
					reflog_پيغام.buf, "HEAD", new_سر,
					سر,
					0, UPDATE_REFS_DIE_ON_ERR);
			/*
			 * We ignore errors in 'gc --auto', since the
			 * user should see them.
			 */
			لاڳيتو_auto_maintenance(the_repository, verbosity < 0);
		}
	}
	إذا (new_سر && show_diffstat) {
		بنية diff_options opts;
		repo_diff_تياري(the_repository, &opts);
		init_diffstat_widths(&opts);

		بدل (show_diffstat) {
		حالة ملايو_SHOW_DIFFSTAT: /* 1 */
			opts.output_format |=
				DIFF_FORMAT_SUMMARY | DIFF_FORMAT_DIFFSTAT;
			اكسر;
		حالة ملايو_SHOW_COMPACTSUMMARY: /* 2 */
			opts.output_format |= DIFF_FORMAT_DIFFSTAT;
			opts.flags.stat_with_summary = 1;
			اكسر;
		افتراضي:
			اكسر;
		}
		opts.detect_rename = DIFF_DETECT_RENAME;
		diff_تياري_پورو(&opts);
		diff_وڻ_oid(سر, new_سر, "", &opts);
		diffcore_std(&opts);
		diff_flush(&opts);
	}

	/* Run a post-merge hook */
	لاڳيتو_hooks_l(the_repository, "post-merge", squash ? "1" : "0", عدم);

	إذا (new_سر)
		apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
				    عدم, عدم, عدم, عدم);
	strbuf_release(&reflog_پيغام);
}

/* Get the name for the merge commit's message. */
ساكن فراغ ملايو_name(ثابت حرف *remote, بنية strbuf *نياپو)
{
	بنية commit *remote_سر;
	بنية object_id branch_سر;
	بنية strbuf bname = STRBUF_INIT;
	بنية ملايو_remote_desc *desc;
	ثابت حرف *ptr;
	حرف *مليو_ref = عدم;
	صحيح len, early;

	copy_branchname(the_repository, &bname, remote, 0);
	remote = bname.buf;

	oidclr(&branch_سر, the_repository->hash_algo);
	remote_سر = get_ملايو_والد(remote);
	إذا (!remote_سر)
		die(_("'%s' does not point to a commit"), remote);

	إذا (repo_dwim_ref(the_repository, remote, لڙي_ڊيگهه(remote), &branch_سر,
			  &مليو_ref, 0) > 0) {
		إذا (starts_with(مليو_ref, "refs/heads/")) {
			strbuf_addf(نياپو, "%s\t\tbranch '%s' of .\n",
				    oid_منزل_hex(&branch_سر), remote);
			اذهب cleanup;
		}
		إذا (starts_with(مليو_ref, "refs/tags/")) {
			strbuf_addf(نياپو, "%s\t\ttag '%s' of .\n",
				    oid_منزل_hex(&branch_سر), remote);
			اذهب cleanup;
		}
		إذا (starts_with(مليو_ref, "refs/remotes/")) {
			strbuf_addf(نياپو, "%s\t\tremote-tracking branch '%s' of .\n",
				    oid_منزل_hex(&branch_سر), remote);
			اذهب cleanup;
		}
	}

	/* See if remote matches <name>^^^.. or <name>~<number> */
	لكل (len = 0, ptr = remote + لڙي_ڊيگهه(remote);
	     remote < ptr && ptr[-1] == '^';
	     ptr--)
		len++;
	إذا (len)
		early = 1;
	وإلا {
		early = 0;
		ptr = strrchr(remote, '~');
		إذا (ptr) {
			صحيح ڏٺل_nonzero = 0;

			len++; /* count ~ */
			طالما (*++ptr && انگ_آهي(*ptr)) {
				ڏٺل_nonzero |= (*ptr != '0');
				len++;
			}
			إذا (*ptr)
				len = 0; /* not ...~<number> */
			وإلا إذا (ڏٺل_nonzero)
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
			strbuf_addf(نياپو,
				    "%s\t\tbranch '%s'%s of .\n",
				    oid_منزل_hex(&remote_سر->object.oid),
				    truname.buf + 11,
				    (early ? " (early part)" : ""));
			strbuf_release(&truname);
			اذهب cleanup;
		}
		strbuf_release(&truname);
	}

	desc = ملايو_remote_util(remote_سر);
	إذا (desc && desc->obj && desc->obj->type == OBJ_ٽيگ) {
		strbuf_addf(نياپو, "%s\t\t%s '%s'\n",
			    oid_منزل_hex(&desc->obj->oid),
			    type_name(desc->obj->type),
			    remote);
		اذهب cleanup;
	}

	strbuf_addf(نياپو, "%s\t\tcommit '%s'\n",
		oid_منزل_hex(&remote_سر->object.oid), remote);
cleanup:
	حرر(مليو_ref);
	strbuf_release(&bname);
}

ساكن فراغ parse_branch_ملايو_options(حرف *bmo)
{
	ثابت حرف **argv;
	صحيح argc;

	إذا (!bmo)
		أرجع;
	argc = ورهايو_cmdline(bmo, &argv);
	إذا (argc < 0)
		die(_("Bad branch.%s.mergeoptions string: %s"), branch,
		    _(ورهايو_cmdline_strerror(argc)));
	REALLOC_ARRAY(argv, argc + 2);
	MOVE_ARRAY(argv + 1, argv, argc + 1);
	argc++;
	argv[0] = "branch.*.mergeoptions";
	parse_options(argc, argv, عدم, builtin_ملايو_options,
		      builtin_ملايو_usage, 0);
	حرر(argv);
}

ساكن صحيح git_ملايو_config(ثابت حرف *k, ثابت حرف *v,
			    ثابت بنية config_context *ctx, فراغ *cb)
{
	صحيح حالت;
	ثابت حرف *str;

	إذا (branch &&
	    skip_اڳياڙي(k, "branch.", &str) &&
	    skip_اڳياڙي(str, branch, &str) &&
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
			show_diffstat = ملايو_SHOW_DIFFSTAT;
			اكسر;
		افتراضي:
			إذا (!strcmp(v, "compact"))
				show_diffstat = ملايو_SHOW_COMPACTSUMMARY;
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
				show_diffstat = ملايو_SHOW_DIFFSTAT;
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

	حالت = fmt_ملايو_نياپو_config(k, v, ctx, cb);
	إذا (حالت)
		أرجع حالت;
	أرجع git_diff_ui_config(k, v, ctx, cb);
}

ساكن صحيح read_وڻ_trivial(بنية object_id *common, بنية object_id *سر,
			     بنية object_id *one)
{
	صحيح i, nr_trees = 0;
	بنية وڻ *trees[MAX_UNPACK_TREES];
	بنية وڻ_desc t[MAX_UNPACK_TREES];
	بنية unpack_trees_options opts;

	memset(&opts, 0, حجم(opts));
	opts.سر_idx = 2;
	opts.src_اشاريو = the_repository->اشاريو;
	opts.dst_اشاريو = the_repository->اشاريو;
	opts.update = 1;
	opts.verbose_update = 1;
	opts.trivial_merges_only = 1;
	opts.ملايو = 1;
	opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
	trees[nr_trees] = repo_parse_وڻ_اڻسڌو(the_repository, common);
	إذا (!trees[nr_trees++])
		أرجع -1;
	trees[nr_trees] = repo_parse_وڻ_اڻسڌو(the_repository, سر);
	إذا (!trees[nr_trees++])
		أرجع -1;
	trees[nr_trees] = repo_parse_وڻ_اڻسڌو(the_repository, one);
	إذا (!trees[nr_trees++])
		أرجع -1;
	opts.fn = threeway_ملايو;
	cache_وڻ_free(&the_repository->اشاريو->cache_وڻ);
	لكل (i = 0; i < nr_trees; i++) {
		repo_parse_وڻ(the_repository, trees[i]);
		init_وڻ_desc(t+i, &trees[i]->object.oid,
			       trees[i]->بفر, trees[i]->سائيز);
	}
	إذا (unpack_trees(nr_trees, t, &opts))
		أرجع -1;
	أرجع 0;
}

ساكن فراغ write_وڻ_trivial(بنية object_id *oid)
{
	إذا (write_اشاريو_as_وڻ(oid, the_repository->اشاريو,
				repo_get_اشاريو_file(the_repository),
				0, عدم))
		die(_("git write-tree failed to write a tree"));
}

ساكن صحيح try_ملايو_strategy(ثابت حرف *strategy, بنية commit_list *common,
			      بنية commit_list *remoteheads,
			      بنية commit *سر)
{
	ثابت حرف *سر_arg = "HEAD";

	إذا (repo_refresh_and_write_اشاريو(the_repository, REFRESH_QUIET,
					 SKIP_IF_UNCHANGED, 0, عدم, عدم,
					 عدم) < 0)
		die(_("Unable to write index."));

	إذا (!strcmp(strategy, "recursive") || !strcmp(strategy, "subtree") ||
	    !strcmp(strategy, "ort")) {
		بنية تالو_file تالو = تالو_INIT;
		صحيح clean, x;
		بنية commit *نتيجو;
		بنية commit_list *reversed = عدم;
		بنية ملايو_options o;
		بنية commit_list *j;

		إذا (remoteheads->ايندڙ) {
			error(_("Not handling anything other than two heads merge."));
			أرجع 2;
		}

		init_ui_ملايو_options(&o, the_repository);
		إذا (!strcmp(strategy, "subtree"))
			o.subtree_سرڪايو = "";

		o.show_rename_ترقي =
			show_ترقي == -1 ? isatty(2) : show_ترقي;

		لكل (x = 0; x < xopts.nr; x++)
			إذا (parse_ملايو_opt(&o, xopts.v[x]))
				die(_("unknown strategy option: -X%s"), xopts.v[x]);

		o.branch1 = سر_arg;
		o.branch2 = ملايو_remote_util(remoteheads->شي)->name;

		لكل (j = common; j; j = j->ايندڙ)
			commit_list_وجهو(j->شي, &reversed);

		repo_hold_locked_اشاريو(the_repository, &تالو,
				       تالو_DIE_ON_ERROR);
		clean = ملايو_ort_recursive(&o, سر, remoteheads->شي,
					    reversed, &نتيجو);
		commit_list_free(reversed);
		strbuf_release(&o.obuf);

		إذا (clean < 0) {
			rollback_تالو_file(&تالو);
			أرجع 2;
		}
		إذا (write_locked_اشاريو(the_repository->اشاريو, &تالو,
				       COMMIT_تالو | SKIP_IF_UNCHANGED))
			die(_("unable to write %s"), repo_get_اشاريو_file(the_repository));
		أرجع clean ? 0 : 1;
	} وإلا {
		أرجع try_ملايو_command(the_repository,
					 strategy, xopts.nr, xopts.v,
					 common, سر_arg, remoteheads);
	}
}

ساكن فراغ ڳڻپ_diff_files(بنية diff_قطار_struct *q,
			     بنية diff_options *opt اڻ_استعمال_ٿيل, فراغ *معلومات)
{
	صحيح *ڳڻپ = معلومات;

	(*ڳڻپ) += q->nr;
}

ساكن صحيح ڳڻپ_unmerged_entries(فراغ)
{
	صحيح i, ret = 0;

	لكل (i = 0; i < the_repository->اشاريو->cache_nr; i++)
		إذا (ce_stage(the_repository->اشاريو->cache[i]))
			ret++;

	أرجع ret;
}

ساكن فراغ جوڙيو_strategies(ثابت حرف *string, بنانشان attr)
{
	صحيح i;

	إذا (string) {
		بنية string_list list = STRING_LIST_INIT_DUP;
		بنية string_list_شي *شي;
		string_list_ورهايو(&list, string, " ", -1);
		for_each_string_list_شي(شي, &list)
			append_strategy(get_strategy(شي->string));
		string_list_clear(&list, 0);
		أرجع;
	}
	لكل (i = 0; i < ARRAY_سائيز(all_strategy); i++)
		إذا (all_strategy[i].attr & attr)
			append_strategy(&all_strategy[i]);

}

ساكن فراغ read_ملايو_نياپو(بنية strbuf *نياپو)
{
	ثابت حرف *filename = git_رستو_ملايو_نياپو(the_repository);
	strbuf_reset(نياپو);
	إذا (strbuf_read_file(نياپو, filename, 0) < 0)
		die_errno(_("Could not read from '%s'"), filename);
}

ساكن فراغ write_ملايو_state(بنية commit_list *);
ساكن فراغ abort_commit(بنية commit_list *remoteheads, ثابت حرف *err_نياپو)
{
	إذا (err_نياپو)
		error("%s", err_نياپو);
	fprintf(stderr,
		_("Not committing merge; use 'git commit' to complete the merge.\n"));
	write_ملايو_state(remoteheads);
	ٻاهر_نڪرو(1);
}

ساكن ثابت حرف ملايو_editor_comment[] =
N_("Please enter a commit message to explain why this merge is necessary,\n"
   "especially if it merges an updated upstream into a topic branch.\n"
   "\n");

ساكن ثابت حرف scissors_editor_comment[] =
N_("An empty message aborts the commit.\n");

ساكن ثابت حرف no_scissors_editor_comment[] =
N_("Lines starting with '%s' will be ignored, and an empty message aborts\n"
   "the commit.\n");

ساكن فراغ write_ملايو_heads(بنية commit_list *);
ساكن فراغ prepare_منزل_commit(بنية commit_list *remoteheads)
{
	بنية strbuf نياپو = STRBUF_INIT;
	ثابت حرف *اشاريو_file = repo_get_اشاريو_file(the_repository);

	إذا (!no_verify) {
		صحيح invoked_hook;

		إذا (لاڳيتو_commit_hook(0 < option_edit, اشاريو_file, &invoked_hook,
				    "pre-merge-commit", عدم))
			abort_commit(remoteheads, عدم);
		/*
		 * Re-read the index as pre-merge-commit hook could have updated it,
		 * and write it out as a tree.  We must do this before we invoke
		 * the editor and after we invoke run_status above.
		 */
		إذا (invoked_hook)
			discard_اشاريو(the_repository->اشاريو);
	}
	read_اشاريو_from(the_repository->اشاريو, اشاريو_file,
			repo_get_git_dir(the_repository));
	strbuf_addbuf(&نياپو, &ملايو_نياپو);
	إذا (squash)
		BUG("the control must not reach here under --squash");
	إذا (0 < option_edit) {
		strbuf_addch(&نياپو, '\n');
		إذا (cleanup_گهڻائي == COMMIT_نياپو_CLEANUP_SCISSORS) {
			wt_حالت_append_cut_سٽ(&نياپو);
			strbuf_commented_addf(&نياپو, comment_سٽ_str, "\n");
		}
		strbuf_commented_addf(&نياپو, comment_سٽ_str,
				      _(ملايو_editor_comment));
		إذا (cleanup_گهڻائي == COMMIT_نياپو_CLEANUP_SCISSORS)
			strbuf_commented_addf(&نياپو, comment_سٽ_str,
					      _(scissors_editor_comment));
		وإلا
			strbuf_commented_addf(&نياپو, comment_سٽ_str,
				_(no_scissors_editor_comment), comment_سٽ_str);
	}
	إذا (signoff)
		append_signoff(&نياپو, ignored_log_پيغام_بائيٽون(نياپو.buf, نياپو.len), 0);
	write_ملايو_heads(remoteheads);
	write_file_buf(git_رستو_ملايو_نياپو(the_repository), نياپو.buf, نياپو.len);
	إذا (لاڳيتو_commit_hook(0 < option_edit, repo_get_اشاريو_file(the_repository),
			    عدم, "prepare-commit-msg",
			    git_رستو_ملايو_نياپو(the_repository), "merge", عدم))
		abort_commit(remoteheads, عدم);
	إذا (0 < option_edit) {
		إذا (launch_editor(git_رستو_ملايو_نياپو(the_repository), عدم, عدم))
			abort_commit(remoteheads, عدم);
	}

	إذا (!no_verify && لاڳيتو_commit_hook(0 < option_edit, repo_get_اشاريو_file(the_repository),
					  عدم, "commit-msg",
					  git_رستو_ملايو_نياپو(the_repository), عدم))
		abort_commit(remoteheads, عدم);

	read_ملايو_نياپو(&نياپو);
	cleanup_پيغام(&نياپو, cleanup_گهڻائي, 0);
	إذا (!نياپو.len)
		abort_commit(remoteheads, _("Empty commit message."));
	strbuf_release(&ملايو_نياپو);
	strbuf_addbuf(&ملايو_نياپو, &نياپو);
	strbuf_release(&نياپو);
}

ساكن صحيح ملايو_trivial(بنية commit *سر, بنية commit_list *remoteheads)
{
	بنية object_id نتيجو_وڻ, نتيجو_commit;
	بنية commit_list *parents = عدم, **pptr = &parents;

	إذا (repo_refresh_and_write_اشاريو(the_repository, REFRESH_QUIET,
					 SKIP_IF_UNCHANGED, 0, عدم, عدم,
					 عدم) < 0)
		أرجع error(_("Unable to write index."));

	write_وڻ_trivial(&نتيجو_وڻ);
	لکو(_("Wonderful.\n"));
	pptr = commit_list_append(سر, pptr);
	pptr = commit_list_append(remoteheads->شي, pptr);
	prepare_منزل_commit(remoteheads);
	إذا (commit_وڻ(ملايو_نياپو.buf, ملايو_نياپو.len, &نتيجو_وڻ, parents,
			&نتيجو_commit, عدم, sign_commit))
		die(_("failed to write commit object"));
	پڄاڻي(سر, remoteheads, &نتيجو_commit, "In-index merge");

	remove_ملايو_branch_state(the_repository);
	commit_list_free(parents);
	أرجع 0;
}

ساكن صحيح پڄاڻي_automerge(بنية commit *سر,
			    صحيح سر_subsumed,
			    بنية commit_list *common,
			    بنية commit_list *remoteheads,
			    بنية object_id *نتيجو_وڻ,
			    ثابت حرف *wt_strategy)
{
	بنية commit_list *parents = عدم;
	بنية strbuf buf = STRBUF_INIT;
	بنية object_id نتيجو_commit;

	write_وڻ_trivial(نتيجو_وڻ);
	commit_list_free(common);
	parents = remoteheads;
	إذا (!سر_subsumed || fast_forward == FF_NO)
		commit_list_وجهو(سر, &parents);
	prepare_منزل_commit(remoteheads);
	إذا (commit_وڻ(ملايو_نياپو.buf, ملايو_نياپو.len, نتيجو_وڻ, parents,
			&نتيجو_commit, عدم, sign_commit))
		die(_("failed to write commit object"));
	strbuf_addf(&buf, "Merge made by the '%s' strategy.", wt_strategy);
	پڄاڻي(سر, remoteheads, &نتيجو_commit, buf.buf);

	strbuf_release(&buf);
	remove_ملايو_branch_state(the_repository);
	commit_list_free(parents);
	أرجع 0;
}

ساكن صحيح suggest_conflicts(فراغ)
{
	ثابت حرف *filename;
	FILE *fp;
	بنية strbuf msgbuf = STRBUF_INIT;

	filename = git_رستو_ملايو_نياپو(the_repository);
	fp = xfopen(filename, "a");

	/*
	 * We can't use cleanup_mode because if we're not using the editor,
	 * get_cleanup_mode will return COMMIT_MSG_CLEANUP_SPACE instead, even
	 * though the message is meant to be processed later by git-commit.
	 * Thus, we will get the cleanup mode which is returned when we _are_
	 * using an editor.
	 */
	append_conflicts_hint(the_repository->اشاريو, &msgbuf,
			      get_cleanup_گهڻائي(cleanup_arg, 1));
	fputs(msgbuf.buf, fp);
	strbuf_release(&msgbuf);
	fclose(fp);
	repo_rerere(the_repository, allow_rerere_auto);
	لکو(_("Automatic merge failed; "
			"fix conflicts and then commit the result.\n"));
	أرجع 1;
}

ساكن صحيح evaluate_نتيجو(فراغ)
{
	صحيح cnt = 0;
	بنية rev_info rev;

	/* Check how many files differ. */
	repo_init_revisions(the_repository, &rev, "");
	تياري_revisions(0, عدم, &rev, عدم);
	rev.diffopt.output_format |=
		DIFF_FORMAT_CALLBACK;
	rev.diffopt.format_callback = ڳڻپ_diff_files;
	rev.diffopt.format_callback_معلومات = &cnt;
	لاڳيتو_diff_files(&rev, 0);

	/*
	 * Check how many unmerged entries are
	 * there.
	 */
	cnt += ڳڻپ_unmerged_entries();

	release_revisions(&rev);
	أرجع cnt;
}

/*
 * Pretend as if the user told us to merge with the remote-tracking
 * branch we have for the upstream of the current branch
 */
ساكن صحيح تياري_with_upstream(ثابت حرف ***argv)
{
	بنية branch *branch = branch_get(عدم);
	صحيح i;
	ثابت حرف **دليلون;

	إذا (!branch)
		die(_("No current branch."));
	إذا (!branch->remote_name)
		die(_("No remote for the current branch."));
	إذا (!branch->ملايو_nr)
		die(_("No default upstream defined for the current branch."));

	دليلون = xcalloc(st_جوڙيو(branch->ملايو_nr, 1), حجم(حرف *));
	لكل (i = 0; i < branch->ملايو_nr; i++) {
		إذا (!branch->ملايو[i]->dst)
			die(_("No remote-tracking branch for %s from %s"),
			    branch->ملايو[i]->src, branch->remote_name);
		دليلون[i] = branch->ملايو[i]->dst;
	}
	دليلون[i] = عدم;
	*argv = دليلون;
	أرجع i;
}

ساكن فراغ write_ملايو_heads(بنية commit_list *remoteheads)
{
	بنية commit_list *j;
	بنية strbuf buf = STRBUF_INIT;

	لكل (j = remoteheads; j; j = j->ايندڙ) {
		بنية object_id *oid;
		بنية commit *c = j->شي;
		بنية ملايو_remote_desc *desc;

		desc = ملايو_remote_util(c);
		إذا (desc && desc->obj) {
			oid = &desc->obj->oid;
		} وإلا {
			oid = &c->object.oid;
		}
		strbuf_addf(&buf, "%s\n", oid_منزل_hex(oid));
	}
	write_file_buf(git_رستو_ملايو_سر(the_repository), buf.buf, buf.len);

	strbuf_reset(&buf);
	إذا (fast_forward == FF_NO)
		strbuf_addstr(&buf, "no-ff");
	write_file_buf(git_رستو_ملايو_گهڻائي(the_repository), buf.buf, buf.len);
	strbuf_release(&buf);
}

ساكن فراغ write_ملايو_state(بنية commit_list *remoteheads)
{
	write_ملايو_heads(remoteheads);
	strbuf_addch(&ملايو_نياپو, '\n');
	write_file_buf(git_رستو_ملايو_نياپو(the_repository), ملايو_نياپو.buf,
		       ملايو_نياپو.len);
}

ساكن صحيح default_edit_option(فراغ)
{
	ساكن ثابت حرف name[] = "GIT_MERGE_AUTOEDIT";
	ثابت حرف *e = getenv(name);
	بنية stat st_stdin, st_stdout;

	إذا (have_پيغام)
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
		st_stdin.st_گهڻائي == st_stdout.st_گهڻائي);
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
		بنية commit *commit = ڪڍو_commit(&parents);
		إذا (commit == سر_commit)
			*سر_subsumed = 0;
		وإلا
			remotes = &commit_list_وجهو(commit, remotes)->ايندڙ;
	}
	أرجع remoteheads;
}

ساكن فراغ prepare_ملايو_پيغام(بنية strbuf *ملايو_نالا, بنية strbuf *ملايو_نياپو)
{
	بنية fmt_ملايو_نياپو_opts opts;

	memset(&opts, 0, حجم(opts));
	opts.جوڙيو_title = !have_پيغام;
	opts.shortlog_len = shortlog_len;
	opts.credit_people = (0 < option_edit);
	opts.into_name = into_name;

	fmt_ملايو_نياپو(ملايو_نالا, ملايو_نياپو, &opts);
	إذا (ملايو_نياپو->len)
		strbuf_setlen(ملايو_نياپو, ملايو_نياپو->len - 1);
}

ساكن فراغ handle_fetch_سر(بنية commit_list **remotes, بنية strbuf *ملايو_نالا)
{
	ثابت حرف *filename;
	صحيح fd, pos, npos;
	بنية strbuf fetch_سر_file = STRBUF_INIT;
	ثابت بنانشان hexsz = the_hash_algo->hexsz;

	إذا (!ملايو_نالا)
		ملايو_نالا = &fetch_سر_file;

	filename = git_رستو_fetch_سر(the_repository);
	fd = xopen(filename, O_RDONLY);

	إذا (strbuf_read(ملايو_نالا, fd, 0) < 0)
		die_errno(_("could not read '%s'"), filename);
	إذا (close(fd) < 0)
		die_errno(_("could not close '%s'"), filename);

	لكل (pos = 0; pos < ملايو_نالا->len; pos = npos) {
		بنية object_id oid;
		حرف *ptr;
		بنية commit *commit;

		ptr = strchr(ملايو_نالا->buf + pos, '\n');
		إذا (ptr)
			npos = ptr - ملايو_نالا->buf + 1;
		وإلا
			npos = ملايو_نالا->len;

		إذا (npos - pos < hexsz + 2 ||
		    get_oid_hex(ملايو_نالا->buf + pos, &oid))
			commit = عدم; /* bad */
		وإلا إذا (memcmp(ملايو_نالا->buf + pos + hexsz, "\t\t", 2))
			استمر; /* not-for-merge */
		وإلا {
			حرف saved = ملايو_نالا->buf[pos + hexsz];
			ملايو_نالا->buf[pos + hexsz] = '\0';
			commit = get_ملايو_والد(ملايو_نالا->buf + pos);
			ملايو_نالا->buf[pos + hexsz] = saved;
		}
		إذا (!commit) {
			إذا (ptr)
				*ptr = '\0';
			die(_("not something we can merge in %s: %s"),
			    filename, ملايو_نالا->buf + pos);
		}
		remotes = &commit_list_وجهو(commit, remotes)->ايندڙ;
	}

	إذا (ملايو_نالا == &fetch_سر_file)
		strbuf_release(&fetch_سر_file);
}

ساكن بنية commit_list *collect_parents(بنية commit *سر_commit,
					   صحيح *سر_subsumed,
					   صحيح argc, ثابت حرف **argv,
					   بنية strbuf *ملايو_نياپو)
{
	صحيح i;
	بنية commit_list *remoteheads = عدم;
	بنية commit_list **remotes = &remoteheads;
	بنية strbuf ملايو_نالا = STRBUF_INIT, *autogen = عدم;

	إذا (ملايو_نياپو && (!have_پيغام || shortlog_len))
		autogen = &ملايو_نالا;

	إذا (سر_commit)
		remotes = &commit_list_وجهو(سر_commit, remotes)->ايندڙ;

	إذا (argc == 1 && !strcmp(argv[0], "FETCH_HEAD")) {
		handle_fetch_سر(remotes, autogen);
		remoteheads = reduce_parents(سر_commit, سر_subsumed, remoteheads);
	} وإلا {
		لكل (i = 0; i < argc; i++) {
			بنية commit *commit = get_ملايو_والد(argv[i]);
			إذا (!commit)
				help_unknown_ref(argv[i], "merge",
						 _("not something we can merge"));
			remotes = &commit_list_وجهو(commit, remotes)->ايندڙ;
		}
		remoteheads = reduce_parents(سر_commit, سر_subsumed, remoteheads);
		إذا (autogen) {
			بنية commit_list *p;
			لكل (p = remoteheads; p; p = p->ايندڙ)
				ملايو_name(ملايو_remote_util(p->شي)->name, autogen);
		}
	}

	إذا (autogen) {
		prepare_ملايو_پيغام(autogen, ملايو_نياپو);
		strbuf_release(autogen);
	}

	أرجع remoteheads;
}

ساكن صحيح merging_a_throwaway_ٽيگ(بنية commit *commit)
{
	حرف *ٽيگ_ref;
	بنية object_id oid;
	صحيح is_throwaway_ٽيگ = 0;

	/* Are we merging a tag? */
	إذا (!ملايو_remote_util(commit) ||
	    !ملايو_remote_util(commit)->obj ||
	    ملايو_remote_util(commit)->obj->type != OBJ_ٽيگ)
		أرجع is_throwaway_ٽيگ;

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
	ٽيگ_ref = xstrfmt("refs/tags/%s",
			  ((بنية ٽيگ *)ملايو_remote_util(commit)->obj)->ٽيگ);
	إذا (!refs_read_ref(get_main_ref_store(the_repository), ٽيگ_ref, &oid) &&
	    oideq(&oid, &ملايو_remote_util(commit)->obj->oid))
		is_throwaway_ٽيگ = 0;
	وإلا
		is_throwaway_ٽيگ = 1;
	حرر(ٽيگ_ref);
	أرجع is_throwaway_ٽيگ;
}

صحيح cmd_ملايو(صحيح argc,
	      ثابت حرف **argv,
	      ثابت حرف *اڳياڙي,
	      بنية repository *repo اڻ_استعمال_ٿيل)
{
	بنية object_id نتيجو_وڻ, stash, سر_oid;
	بنية commit *سر_commit;
	بنية strbuf buf = STRBUF_INIT;
	صحيح i, ret = 0, سر_subsumed;
	صحيح بهترين_cnt = -1, ملايو_was_ok = 0, automerge_was_ok = 0;
	بنية commit_list *common = عدم;
	ثابت حرف *بهترين_strategy = عدم, *wt_strategy = عدم;
	بنية commit_list *remoteheads = عدم, *p;
	فراغ *branch_منزل_free, *argv_منزل_free = عدم;
	صحيح orig_argc = argc;
	صحيح ملايو_log_config = -1;

	show_usage_with_options_if_asked(argc, argv,
					 builtin_ملايو_usage, builtin_ملايو_options);

#جيڪڏهن_وصف_نه_ٿيل WITH_BREAKING_CHANGES
	warn_on_auto_comment_char = true;
#جيڪڏهن_پورو /* !WITH_BREAKING_CHANGES */
	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_ڀريل_اشاريو = 0;

	/*
	 * Check if we are _not_ on a detached HEAD, i.e. if there is a
	 * current branch.
	 */
	branch = branch_منزل_free = refs_resolve_refdup(get_main_ref_store(the_repository),
						      "HEAD", 0, &سر_oid,
						      عدم);
	إذا (branch)
		skip_اڳياڙي(branch, "refs/heads/", &branch);

	init_diff_ui_defaults();
	repo_config(the_repository, git_ملايو_config, &ملايو_log_config);

	إذا (!branch || is_null_oid(&سر_oid))
		سر_commit = عدم;
	وإلا
		سر_commit = lookup_commit_or_die(&سر_oid, "HEAD");

	إذا (branch_mergeoptions)
		parse_branch_ملايو_options(branch_mergeoptions);
	argc = parse_options(argc, argv, اڳياڙي, builtin_ملايو_options,
			builtin_ملايو_usage, 0);
	إذا (shortlog_len < 0)
		shortlog_len = (ملايو_log_config > 0) ? ملايو_log_config : 0;

	إذا (verbosity < 0 && show_ترقي == -1)
		show_ترقي = 0;

	إذا (abort_current_ملايو) {
		صحيح nargc = 2;
		ثابت حرف *nargv[] = {"reset", "--merge", عدم};
		حرف stash_oid_hex[GIT_MAX_HEXSZ + 1];
		بنية object_id stash_oid = {0};

		إذا (orig_argc != 2)
			usage_نياپو_opt(_("--abort expects no arguments"),
			      builtin_ملايو_usage, builtin_ملايو_options);

		إذا (!file_exists(git_رستو_ملايو_سر(the_repository)))
			die(_("There is no merge to abort (MERGE_HEAD missing)."));

		إذا (!refs_read_ref(get_main_ref_store(the_repository), "MERGE_AUTOSTASH", &stash_oid))
			refs_delete_ref(get_main_ref_store(the_repository),
					"", "MERGE_AUTOSTASH", &stash_oid,
					REF_NO_DEREF);

		/* Invoke 'git reset --merge' */
		ret = cmd_reset(nargc, nargv, اڳياڙي, the_repository);

		إذا (!is_null_oid(&stash_oid)) {
			oid_منزل_hex_r(stash_oid_hex, &stash_oid);
			apply_autostash_oid(stash_oid_hex);
		}

		اذهب پورو;
	}

	إذا (quit_current_ملايو) {
		إذا (orig_argc != 2)
			usage_نياپو_opt(_("--quit expects no arguments"),
				      builtin_ملايو_usage,
				      builtin_ملايو_options);

		remove_ملايو_branch_state(the_repository);
		اذهب پورو;
	}

	إذا (continue_current_ملايو) {
		صحيح nargc = 1;
		ثابت حرف *nargv[] = {"commit", عدم};

		إذا (orig_argc != 2)
			usage_نياپو_opt(_("--continue expects no arguments"),
			      builtin_ملايو_usage, builtin_ملايو_options);

		إذا (!file_exists(git_رستو_ملايو_سر(the_repository)))
			die(_("There is no merge in progress (MERGE_HEAD missing)."));

		/* Invoke 'git commit' */
		ret = cmd_commit(nargc, nargv, اڳياڙي, the_repository);
		اذهب پورو;
	}

	إذا (repo_read_اشاريو_unmerged(the_repository))
		die_resolve_conflict("merge");

	إذا (file_exists(git_رستو_ملايو_سر(the_repository))) {
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
	resolve_undo_clear_اشاريو(the_repository->اشاريو);

	إذا (option_edit < 0)
		option_edit = default_edit_option();

	cleanup_گهڻائي = get_cleanup_گهڻائي(cleanup_arg, 0 < option_edit);

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
			argc = تياري_with_upstream(&argv);
			argv_منزل_free = argv;
		}
		وإلا
			die(_("No commit specified and merge.defaultToUpstream not set."));
	} وإلا إذا (argc == 1 && !strcmp(argv[0], "-")) {
		argv[0] = "@{-1}";
	}

	إذا (!argc)
		usage_with_options(builtin_ملايو_usage,
			builtin_ملايو_options);

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
		إذا (remoteheads->ايندڙ)
			die(_("Can merge only exactly one commit into empty head"));

		إذا (verify_signatures)
			verify_ملايو_signature(remoteheads->شي, verbosity,
					       check_trust_level);

		remote_سر_oid = &remoteheads->شي->object.oid;
		read_خالي_جايون(remote_سر_oid);
		refs_update_ref(get_main_ref_store(the_repository),
				"initial pull", "HEAD", remote_سر_oid, عدم,
				0,
				UPDATE_REFS_DIE_ON_ERR);
		اذهب پورو;
	}

	/*
	 * All the rest are the commits being merged; prepare
	 * the standard merge summary message to be appended
	 * to the given message.
	 */
	remoteheads = collect_parents(سر_commit, &سر_subsumed,
				      argc, argv, &ملايو_نياپو);

	إذا (!سر_commit || !argc)
		usage_with_options(builtin_ملايو_usage,
			builtin_ملايو_options);

	إذا (verify_signatures) {
		لكل (p = remoteheads; p; p = p->ايندڙ) {
			verify_ملايو_signature(p->شي, verbosity,
					       check_trust_level);
		}
	}

	strbuf_addstr(&buf, "merge");
	لكل (p = remoteheads; p; p = p->ايندڙ)
		strbuf_addf(&buf, " %s", ملايو_remote_util(p->شي)->name);
	setenv("GIT_REFLOG_ACTION", buf.buf, 0);
	strbuf_reset(&buf);

	لكل (p = remoteheads; p; p = p->ايندڙ) {
		بنية commit *commit = p->شي;
		strbuf_addf(&buf, "GITHEAD_%s",
			    oid_منزل_hex(&commit->object.oid));
		setenv(buf.buf, ملايو_remote_util(commit)->name, 1);
		strbuf_reset(&buf);
		إذا (fast_forward != FF_ONLY && merging_a_throwaway_ٽيگ(commit))
			fast_forward = FF_NO;
	}

	إذا (!use_strategies) {
		إذا (!remoteheads)
			; /* already up-to-date */
		وإلا إذا (!remoteheads->ايندڙ)
			جوڙيو_strategies(pull_twohead, DEFAULT_TWOHEAD);
		وإلا
			جوڙيو_strategies(pull_octopus, DEFAULT_OCTOPUS);
	}

	لكل (i = 0; i < use_strategies_nr; i++) {
		إذا (use_strategies[i]->attr & NO_FAST_FORWARD)
			fast_forward = FF_NO;
		إذا (use_strategies[i]->attr & NO_TRIVIAL)
			allow_trivial = 0;
	}

	إذا (!remoteheads)
		; /* already up-to-date */
	وإلا إذا (!remoteheads->ايندڙ) {
		إذا (repo_get_ملايو_bases(the_repository, سر_commit,
					 remoteheads->شي, &common) < 0) {
			ret = 2;
			اذهب پورو;
		}
	} وإلا {
		بنية commit_list *list = remoteheads;
		commit_list_وجهو(سر_commit, &list);
		إذا (get_octopus_ملايو_bases(list, &common) < 0) {
			حرر(list);
			ret = 2;
			اذهب پورو;
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
		 (!remoteheads->ايندڙ && !common->ايندڙ &&
		  common->شي == remoteheads->شي)) {
		/*
		 * If head can reach all the merge then we are up to date.
		 * but first the most common case of merging one remote.
		 */
		پڄاڻي_مٿي_منزل_date();
		اذهب پورو;
	} وإلا إذا (fast_forward != FF_NO && !remoteheads->ايندڙ &&
			!common->ايندڙ &&
			oideq(&common->شي->object.oid, &سر_commit->object.oid)) {
		/* Again the most common case of merging one remote. */
		ثابت حرف *نياپو = have_پيغام ?
			"Fast-forward (no commit created; -m option ignored)" :
			"Fast-forward";
		بنية commit *commit;

		إذا (verbosity >= 0) {
			لکو(_("Updating %s..%s\n"),
			       repo_ڳوليو_unique_abbrev(the_repository, &سر_commit->object.oid,
						       DEFAULT_ABBREV),
			       repo_ڳوليو_unique_abbrev(the_repository, &remoteheads->شي->object.oid,
						       DEFAULT_ABBREV));
		}
		commit = remoteheads->شي;
		إذا (!commit) {
			ret = 1;
			اذهب پورو;
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
			اذهب پورو;
		}

		پڄاڻي(سر_commit, remoteheads, &commit->object.oid, نياپو);
		remove_ملايو_branch_state(the_repository);
		اذهب پورو;
	} وإلا إذا (!remoteheads->ايندڙ && common->ايندڙ)
		;
		/*
		 * We are not doing octopus and not fast-forward.  Need
		 * a real merge.
		 */
	وإلا إذا (!remoteheads->ايندڙ && !common->ايندڙ && option_commit) {
		/*
		 * We are not doing octopus, not fast-forward, and have
		 * only one common.
		 */
		refresh_اشاريو(the_repository->اشاريو, REFRESH_QUIET, عدم, عدم, عدم);
		إذا (allow_trivial && fast_forward != FF_ONLY) {
			/*
			 * Must first ensure that index matches HEAD before
			 * attempting a trivial merge.
			 */
			بنية وڻ *سر_وڻ = repo_get_commit_وڻ(the_repository,
								      سر_commit);
			بنية strbuf sb = STRBUF_INIT;

			إذا (repo_اشاريو_has_changes(the_repository, سر_وڻ,
						   &sb)) {
				error(_("Your local changes to the following files would be overwritten by merge:\n  %s"),
				      sb.buf);
				strbuf_release(&sb);
				ret = 2;
				اذهب پورو;
			}

			/* See if it is really trivial. */
			git_committer_info(IDENT_STRICT);
			لکو(_("Trying really trivial in-index merge...\n"));
			إذا (!read_وڻ_trivial(&common->شي->object.oid,
					       &سر_commit->object.oid,
					       &remoteheads->شي->object.oid)) {
				ret = ملايو_trivial(سر_commit, remoteheads);
				اذهب پورو;
			}
			لکو(_("Nope.\n"));
		}
	} وإلا {
		/*
		 * An octopus.  If we can reach all the remote we are up
		 * to date.
		 */
		صحيح مٿي_منزل_date = 1;
		بنية commit_list *j;

		لكل (j = remoteheads; j; j = j->ايندڙ) {
			صحيح ret = repo_اندريون_ملايو_bases(the_repository,
						      j->شي, سر_commit);
			إذا (ret < 0)
				ٻاهر_نڪرو(128);
			إذا (!ret) {
				مٿي_منزل_date = 0;
				اكسر;
			}
		}
		إذا (مٿي_منزل_date) {
			پڄاڻي_مٿي_منزل_date();
			اذهب پورو;
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
			لکو(_("Rewinding the tree to pristine...\n"));
			restore_state(&سر_commit->object.oid, &stash);
		}
		إذا (use_strategies_nr != 1)
			لکو(_("Trying merge strategy %s...\n"),
				use_strategies[i]->name);
		/*
		 * Remember which strategy left the state in the working
		 * tree.
		 */
		wt_strategy = use_strategies[i]->name;

		ret = try_ملايو_strategy(wt_strategy,
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
				ملايو_was_ok = 1;
				بهترين_strategy = wt_strategy;
				اكسر;
			}
			cnt = (use_strategies_nr > 1) ? evaluate_نتيجو() : 0;
			إذا (بهترين_cnt <= 0 || cnt <= بهترين_cnt) {
				بهترين_strategy = wt_strategy;
				بهترين_cnt = cnt;
			}
		}
	}

	/*
	 * If we have a resulting tree, that means the strategy module
	 * auto resolved the merge cleanly.
	 */
	إذا (ملايو_was_ok && option_commit) {
		automerge_was_ok = 1;
		ret = پڄاڻي_automerge(سر_commit, سر_subsumed,
				       common, remoteheads,
				       &نتيجو_وڻ, wt_strategy);
		اذهب پورو;
	}

	/*
	 * Pick the result from the best strategy and have the user fix
	 * it up.
	 */
	إذا (!بهترين_strategy) {
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
		اذهب پورو;
	} وإلا إذا (بهترين_strategy == wt_strategy)
		; /* We already have its result in the working tree. */
	وإلا {
		لکو(_("Rewinding the tree to pristine...\n"));
		restore_state(&سر_commit->object.oid, &stash);
		لکو(_("Using the %s strategy to prepare resolving by hand.\n"),
			بهترين_strategy);
		try_ملايو_strategy(بهترين_strategy, common, remoteheads,
				   سر_commit);
	}

	إذا (squash) {
		پڄاڻي(سر_commit, remoteheads, عدم, عدم);

		git_test_write_commit_گراف_or_die(the_repository);
	} وإلا
		write_ملايو_state(remoteheads);

	إذا (ملايو_was_ok)
		fprintf(stderr, _("Automatic merge went well; "
			"stopped before committing as requested\n"));
	وإلا
		ret = suggest_conflicts();
	إذا (autostash)
		لکو(_("When finished, apply stashed changes with `git stash pop`\n"));

پورو:
	إذا (!automerge_was_ok) {
		commit_list_free(common);
		commit_list_free(remoteheads);
	}
	strbuf_release(&buf);
	حرر(branch_منزل_free);
	حرر(argv_منزل_free);
	حرر(pull_twohead);
	حرر(pull_octopus);
	discard_اشاريو(the_repository->اشاريو);
	أرجع ret;
}
