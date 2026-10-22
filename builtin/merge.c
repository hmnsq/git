/*
 * Builtin "git merge"
 *
 * Copyright (c) 2008 Miklos Vajna <vmiklos@frugalware.org>
 *
 * Based on git-merge.sh by Junio C Hamano.
 */

#నిర్వచించు USE_THE_REPOSITORY_VARIABLE
#నిర్వచించు DISABLE_SIGN_COMPARE_WARNINGS

#అన్తర్భావ "builtin.h"

#అన్తర్భావ "abspath.h"
#అన్తర్భావ "advice.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "editor.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "hex.h"
#అన్తర్భావ "object-name.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "lockfile.h"
#అన్తర్భావ "repository.h"
#అన్తర్భావ "run-command.h"
#అన్తర్భావ "hook.h"
#అన్తర్భావ "diff.h"
#అన్తర్భావ "diff-merges.h"
#అన్తర్భావ "refs.h"
#అన్తర్భావ "refspec.h"
#అన్తర్భావ "commit.h"
#అన్తర్భావ "diffcore.h"
#అన్తర్భావ "path.h"
#అన్తర్భావ "revision.h"
#అన్తర్భావ "unpack-trees.h"
#అన్తర్భావ "cache-tree.h"
#అన్తర్భావ "dir.h"
#అన్తర్భావ "color.h"
#అన్తర్భావ "rerere.h"
#అన్తర్భావ "help.h"
#అన్తర్భావ "merge.h"
#అన్తర్భావ "merge-ort-wrappers.h"
#అన్తర్భావ "resolve-undo.h"
#అన్తర్భావ "remote.h"
#అన్తర్భావ "fmt-merge-msg.h"
#అన్తర్భావ "sequencer.h"
#అన్తర్భావ "string-list.h"
#అన్తర్భావ "tag.h"
#అన్తర్భావ "alias.h"
#అన్తర్భావ "branch.h"
#అన్తర్భావ "commit-reach.h"
#అన్తర్భావ "wt-status.h"
#అన్తర్భావ "commit-graph.h"

#నిర్వచించు DEFAULT_TWOHEAD (1<<0)
#నిర్వచించు DEFAULT_OCTOPUS (1<<1)
#నిర్వచించు NO_FAST_FORWARD (1<<2)
#నిర్వచించు NO_TRIVIAL      (1<<3)

నిర్మాణం strategy {
	స్థిరరాశి అక్షరం *name;
	గుర్తులేని attr;
};

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి builtin_విలీనం_usage[] = {
	N_("git merge [<options>] [<commit>...]"),
	"git merge --abort",
	"git merge --continue",
	ఖాళీ
};

#నిర్వచించు విలీనం_SHOW_DIFFSTAT 1
#నిర్వచించు విలీనం_SHOW_COMPACTSUMMARY 2

స్థిర పూర్ణాంకం show_diffstat = విలీనం_SHOW_DIFFSTAT, shortlog_len = -1, squash;
స్థిర పూర్ణాంకం option_commit = -1;
స్థిర పూర్ణాంకం option_edit = -1;
స్థిర పూర్ణాంకం allow_trivial = 1, have_సందేశం, verify_signatures;
స్థిర పూర్ణాంకం check_trust_level = 1;
స్థిర పూర్ణాంకం overwrite_ignore = 1;
స్థిర నిర్మాణం strbuf విలీనం_సందేశం = STRBUF_INIT;
స్థిర నిర్మాణం strategy **use_strategies;
స్థిర కొలత_t use_strategies_nr, use_strategies_alloc;
స్థిర నిర్మాణం strvec xopts = STRVEC_INIT;
స్థిర స్థిరరాశి అక్షరం *branch;
స్థిర అక్షరం *branch_mergeoptions;
స్థిర పూర్ణాంకం verbosity;
స్థిర పూర్ణాంకం allow_rerere_auto;
స్థిర పూర్ణాంకం abort_current_విలీనం;
స్థిర పూర్ణాంకం quit_current_విలీనం;
స్థిర పూర్ణాంకం continue_current_విలీనం;
స్థిర పూర్ణాంకం allow_unrelated_histories;
స్థిర పూర్ణాంకం show_పురోగతి = -1;
స్థిర పూర్ణాంకం default_గమ్యం_upstream = 1;
స్థిర పూర్ణాంకం signoff;
స్థిర స్థిరరాశి అక్షరం *sign_commit;
స్థిర పూర్ణాంకం autostash;
స్థిర పూర్ణాంకం no_verify;
స్థిర అక్షరం *into_name;

స్థిర నిర్మాణం strategy all_strategy[] = {
	{ "recursive",  NO_TRIVIAL },
	{ "octopus",    DEFAULT_OCTOPUS },
	{ "ort",        DEFAULT_TWOHEAD | NO_TRIVIAL },
	{ "resolve",    0 },
	{ "ours",       NO_FAST_FORWARD | NO_TRIVIAL },
	{ "subtree",    NO_FAST_FORWARD | NO_TRIVIAL },
};

స్థిర అక్షరం *pull_twohead, *pull_octopus;

గణన ff_type {
	FF_NO,
	FF_ALLOW,
	FF_ONLY
};

స్థిర గణన ff_type fast_forward = FF_ALLOW;

స్థిర అక్షరం *cleanup_arg;
స్థిర గణన commit_సందేశం_cleanup_బహుళకం cleanup_బహుళకం;

స్థిర పూర్ణాంకం option_parse_సందేశం(స్థిరరాశి నిర్మాణం option *opt,
				స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	నిర్మాణం strbuf *buf = opt->విలువ;

	ఒకవేళ (unset)
		strbuf_setlen(buf, 0);
	లేకపోతే ఒకవేళ (arg) {
		strbuf_addf(buf, "%s%s", buf->len ? "\n\n" : "", arg);
		have_సందేశం = 1;
	} లేకపోతే
		తిరిగి error(_("switch `m' requires a value"));
	తిరిగి 0;
}

స్థిర గణన parse_opt_ఫలితం option_read_సందేశం(నిర్మాణం parse_opt_ctx_t *ctx,
						 స్థిరరాశి నిర్మాణం option *opt,
						 స్థిరరాశి అక్షరం *arg_not_వాడిన,
						 పూర్ణాంకం unset)
{
	నిర్మాణం strbuf *buf = opt->విలువ;
	స్థిరరాశి అక్షరం *arg;

	BUG_ON_OPT_ARG(arg_not_వాడిన);
	ఒకవేళ (unset)
		BUG("-F cannot be negated");

	ఒకవేళ (ctx->opt) {
		arg = ctx->opt;
		ctx->opt = ఖాళీ;
	} లేకపోతే ఒకవేళ (ctx->argc > 1) {
		ctx->argc--;
		arg = *++ctx->argv;
	} లేకపోతే
		తిరిగి error(_("option `%s' requires a value"), opt->long_name);

	ఒకవేళ (buf->len)
		strbuf_addch(buf, '\n');
	ఒకవేళ (ctx->ఉపసర్గ && !is_absolute_మార్గం(arg))
		arg = ఉపసర్గ_filename(ctx->ఉపసర్గ, arg);
	ఒకవేళ (strbuf_read_file(buf, arg, 0) < 0)
		తిరిగి error(_("could not read file '%s'"), arg);
	have_సందేశం = 1;

	తిరిగి 0;
}

స్థిర నిర్మాణం strategy *get_strategy(స్థిరరాశి అక్షరం *name)
{
	పూర్ణాంకం i;
	నిర్మాణం strategy *ret;
	స్థిర నిర్మాణం cmdnames main_cmds = {0}, other_cmds = {0};
	స్థిర పూర్ణాంకం loaded;
	అక్షరం *default_strategy = ఖాళీ;

	ఒకవేళ (!name)
		తిరిగి ఖాళీ;

	ఒకవేళ (default_strategy &&
	    !strcmp(default_strategy, "ort") &&
	    !strcmp(name, "recursive")) {
		name = "ort";
	}

	కొరకు (i = 0; i < ARRAY_కొలత(all_strategy); i++)
		ఒకవేళ (!strcmp(name, all_strategy[i].name))
			తిరిగి &all_strategy[i];

	ఒకవేళ (!loaded) {
		నిర్మాణం cmdnames not_strategies = {0};
		loaded = 1;

		ఎక్కించు_command_list("git-merge-", &main_cmds, &other_cmds);
		కొరకు (i = 0; i < main_cmds.cnt; i++) {
			పూర్ణాంకం j, దొరికింది_2 = 0;
			నిర్మాణం cmdname *ent = main_cmds.పేర్లు[i];
			కొరకు (j = 0; !దొరికింది_2 && j < ARRAY_కొలత(all_strategy); j++)
				ఒకవేళ (!xstrncmpz(all_strategy[j].name, ent->name, ent->len))
					దొరికింది_2 = 1;
			ఒకవేళ (!దొరికింది_2)
				కూడు_cmdname(&not_strategies, ent->name, ent->len);
		}
		exclude_cmds(&main_cmds, &not_strategies);

		cmdnames_release(&not_strategies);
	}
	ఒకవేళ (!is_లోపల_cmdlist(&main_cmds, name) && !is_లోపల_cmdlist(&other_cmds, name)) {
		fprintf(stderr, _("Could not find merge strategy '%s'.\n"), name);
		fprintf(stderr, _("Available strategies are:"));
		కొరకు (i = 0; i < main_cmds.cnt; i++)
			fprintf(stderr, " %s", main_cmds.పేర్లు[i]->name);
		fprintf(stderr, ".\n");
		ఒకవేళ (other_cmds.cnt) {
			fprintf(stderr, _("Available custom strategies are:"));
			కొరకు (i = 0; i < other_cmds.cnt; i++)
				fprintf(stderr, " %s", other_cmds.పేర్లు[i]->name);
			fprintf(stderr, ".\n");
		}
		నిష్క్రమించు(1);
	}

	CALLOC_ARRAY(ret, 1);
	ret->name = xstrdup(name);
	ret->attr = NO_TRIVIAL;

	cmdnames_release(&main_cmds);
	cmdnames_release(&other_cmds);
	తిరిగి ret;
}

స్థిర శూన్య append_strategy(నిర్మాణం strategy *s)
{
	ALLOC_GROW(use_strategies, use_strategies_nr + 1, use_strategies_alloc);
	use_strategies[use_strategies_nr++] = s;
}

స్థిర పూర్ణాంకం option_parse_strategy(స్థిరరాశి నిర్మాణం option *opt వాడని,
				 స్థిరరాశి అక్షరం *name, పూర్ణాంకం unset)
{
	ఒకవేళ (unset)
		తిరిగి 0;

	append_strategy(get_strategy(name));
	తిరిగి 0;
}

స్థిర పూర్ణాంకం option_parse_compact_summary(స్థిరరాశి నిర్మాణం option *opt,
					స్థిరరాశి అక్షరం *name వాడని, పూర్ణాంకం unset)
{
	పూర్ణాంకం *setting = opt->విలువ;

	ఒకవేళ (unset)
		*setting = 0;
	లేకపోతే
		*setting = విలీనం_SHOW_COMPACTSUMMARY;
	తిరిగి 0;
}

స్థిర నిర్మాణం option builtin_విలీనం_options[] = {
	OPT_సమితి_INT('n', ఖాళీ, &show_diffstat,
		N_("do not show a diffstat at the end of the merge"), 0),
	OPT_BOOL(0, "stat", &show_diffstat,
		N_("show a diffstat at the end of the merge")),
	OPT_BOOL(0, "summary", &show_diffstat, N_("(synonym to --stat)")),
	OPT_CALLBACK_F(0, "compact-summary", &show_diffstat, ఖాళీ,
		       N_("show a compact-summary at the end of the merge"),
		       PARSE_OPT_NOARG,
		       option_parse_compact_summary),
	{
		.type = OPTION_INTEGER,
		.long_name = "log",
		.విలువ = &shortlog_len,
		.precision = పరిమాణం(shortlog_len),
		.argh = N_("n"),
		.help = N_("add (at most <n>) entries from shortlog to merge commit message"),
		.flags = PARSE_OPT_OPTARG,
		.defval = DEFAULT_విలీనం_LOG_LEN,
	},
	OPT_BOOL(0, "squash", &squash,
		N_("create a single commit instead of doing a merge")),
	OPT_BOOL(0, "commit", &option_commit,
		N_("perform a commit if the merge succeeds (default)")),
	OPT_BOOL('e', "edit", &option_edit,
		N_("edit message before committing")),
	OPT_CLEANUP(&cleanup_arg),
	OPT_సమితి_INT(0, "ff", &fast_forward, N_("allow fast-forward (default)"), FF_ALLOW),
	OPT_సమితి_INT_F(0, "ff-only", &fast_forward,
		      N_("abort if fast-forward is not possible"),
		      FF_ONLY, PARSE_OPT_NONEG),
	OPT_RERERE_AUTOUPDATE(&allow_rerere_auto),
	OPT_BOOL(0, "verify-signatures", &verify_signatures,
		N_("verify that the named commit has a valid GPG signature")),
	OPT_CALLBACK('s', "strategy", ఖాళీ, N_("strategy"),
		N_("merge strategy to use"), option_parse_strategy),
	OPT_STRVEC('X', "strategy-option", &xopts, N_("option=value"),
		N_("option for selected merge strategy")),
	OPT_CALLBACK('m', "message", &విలీనం_సందేశం, N_("message"),
		N_("merge commit message (for a non-fast-forward merge)"),
		option_parse_సందేశం),
	{
		.type = OPTION_LOWLEVEL_CALLBACK,
		.short_name = 'F',
		.long_name = "file",
		.విలువ = &విలీనం_సందేశం,
		.argh = N_("path"),
		.help = N_("read message from file"),
		.flags = PARSE_OPT_NONEG,
		.ll_callback = option_read_సందేశం,
	},
	OPT_STRING(0, "into-name", &into_name, N_("name"),
		   N_("use <name> instead of the real target")),
	OPT__VERBOSITY(&verbosity),
	OPT_BOOL(0, "abort", &abort_current_విలీనం,
		N_("abort the current in-progress merge")),
	OPT_BOOL(0, "quit", &quit_current_విలీనం,
		N_("--abort but leave index and working tree alone")),
	OPT_BOOL(0, "continue", &continue_current_విలీనం,
		N_("continue the current in-progress merge")),
	OPT_BOOL(0, "allow-unrelated-histories", &allow_unrelated_histories,
		 N_("allow merging unrelated histories")),
	OPT_సమితి_INT(0, "progress", &show_పురోగతి, N_("force progress reporting"), 1),
	{
		.type = OPTION_STRING,
		.short_name = 'S',
		.long_name = "gpg-sign",
		.విలువ = &sign_commit,
		.argh = N_("key-id"),
		.help = N_("GPG sign commit"),
		.flags = PARSE_OPT_OPTARG,
		.defval = (intptr_t) "",
	},
	OPT_AUTOSTASH(&autostash),
	OPT_BOOL(0, "overwrite-ignore", &overwrite_ignore, N_("update ignored files (default)")),
	OPT_BOOL(0, "signoff", &signoff, N_("add a Signed-off-by trailer")),
	OPT_BOOL(0, "no-verify", &no_verify, N_("bypass pre-merge-commit and commit-msg hooks")),
	OPT_చివర()
};

స్థిర పూర్ణాంకం save_state(నిర్మాణం object_id *stash)
{
	పూర్ణాంకం len;
	నిర్మాణం బిడ్డ_process cp = బిడ్డ_PROCESS_INIT;
	నిర్మాణం strbuf బఫరు = STRBUF_INIT;
	నిర్మాణం తాళం_file తాళం_file = తాళం_INIT;
	పూర్ణాంకం fd;
	పూర్ణాంకం rc = -1;

	fd = repo_hold_locked_సూచీ(the_repository, &తాళం_file, 0);
	refresh_సూచీ(the_repository->సూచీ, REFRESH_QUIET, ఖాళీ, ఖాళీ, ఖాళీ);
	ఒకవేళ (0 <= fd)
		repo_update_సూచీ_if_able(the_repository, &తాళం_file);
	rollback_తాళం_file(&తాళం_file);

	strvec_pushl(&cp.వాదనలు, "stash", "create", ఖాళీ);
	cp.బహిర్గతం = -1;
	cp.git_cmd = 1;

	ఒకవేళ (ప్రారంభం_command(&cp))
		die(_("could not run stash."));
	len = strbuf_read(&బఫరు, cp.బహిర్గతం, 1024);
	close(cp.బహిర్గతం);

	ఒకవేళ (ముగింపు_command(&cp) || len < 0)
		die(_("stash failed"));
	లేకపోతే ఒకవేళ (!len)		/* no changes */
		వెళ్ళు బహిర్గతం;
	strbuf_setlen(&బఫరు, బఫరు.len-1);
	ఒకవేళ (repo_get_oid(the_repository, బఫరు.buf, stash))
		die(_("not a valid object: %s"), బఫరు.buf);
	rc = 0;
బహిర్గతం:
	strbuf_release(&బఫరు);
	తిరిగి rc;
}

స్థిర శూన్య read_ఖాళీలు(స్థిరరాశి నిర్మాణం object_id *oid)
{
	నిర్మాణం బిడ్డ_process cmd = బిడ్డ_PROCESS_INIT;

	strvec_pushl(&cmd.వాదనలు, "read-tree", "-m", "-u",
		     ఖాళీలు_వృక్షం_oid_hex(the_repository->hash_algo),
		     oid_గమ్యం_hex(oid), ఖాళీ);
	cmd.git_cmd = 1;

	ఒకవేళ (నిరంతరం_command(&cmd))
		die(_("read-tree failed"));
}

స్థిర శూన్య reset_hard(స్థిరరాశి నిర్మాణం object_id *oid)
{
	నిర్మాణం బిడ్డ_process cmd = బిడ్డ_PROCESS_INIT;

	strvec_pushl(&cmd.వాదనలు, "read-tree", "-v", "--reset", "-u",
		     oid_గమ్యం_hex(oid), ఖాళీ);
	cmd.git_cmd = 1;

	ఒకవేళ (నిరంతరం_command(&cmd))
		die(_("read-tree failed"));
}

స్థిర శూన్య restore_state(స్థిరరాశి నిర్మాణం object_id *తల,
			  స్థిరరాశి నిర్మాణం object_id *stash)
{
	నిర్మాణం బిడ్డ_process cmd = బిడ్డ_PROCESS_INIT;

	reset_hard(తల);

	ఒకవేళ (is_null_oid(stash))
		వెళ్ళు refresh_cache;

	strvec_pushl(&cmd.వాదనలు, "stash", "apply", "--index", "--quiet", ఖాళీ);
	strvec_నెట్టు(&cmd.వాదనలు, oid_గమ్యం_hex(stash));

	/*
	 * It is OK to ignore error here, for example when there was
	 * nothing to restore.
	 */
	cmd.git_cmd = 1;
	నిరంతరం_command(&cmd);

refresh_cache:
	discard_సూచీ(the_repository->సూచీ);
	ఒకవేళ (repo_read_సూచీ(the_repository) < 0)
		die(_("could not read index"));
}

/* This is called when no merge was necessary. */
స్థిర శూన్య ముగింపు_పైకి_గమ్యం_date(శూన్య)
{
	ఒకవేళ (verbosity >= 0) {
		ఒకవేళ (squash)
			puts(_("Already up to date. (nothing to squash)"));
		లేకపోతే
			puts(_("Already up to date."));
	}
	remove_విలీనం_branch_state(the_repository);
}

స్థిర శూన్య squash_సందేశం(నిర్మాణం commit *commit, నిర్మాణం commit_list *remoteheads)
{
	నిర్మాణం rev_info rev;
	నిర్మాణం strbuf బహిర్గతం = STRBUF_INIT;
	నిర్మాణం commit_list *j;
	నిర్మాణం pretty_print_context ctx = {0};

	రాయి(_("Squash commit -- not updating HEAD\n"));

	repo_init_revisions(the_repository, &rev, ఖాళీ);
	diff_merges_suppress(&rev);
	rev.commit_format = CMIT_FMT_MEDIUM;

	commit->object.flags |= UNINTERESTING;
	కూడు_pending_object(&rev, &commit->object, ఖాళీ);

	కొరకు (j = remoteheads; j; j = j->తదుపరి)
		కూడు_pending_object(&rev, &j->అంశం->object, ఖాళీ);

	అమరిక_revisions(0, ఖాళీ, &rev, ఖాళీ);
	ఒకవేళ (prepare_revision_walk(&rev))
		die(_("revision walk setup failed"));

	ctx.abbrev = rev.abbrev;
	ctx.date_బహుళకం = rev.date_బహుళకం;
	ctx.fmt = rev.commit_format;

	strbuf_addstr(&బహిర్గతం, "Squashed commit of the following:\n");
	అయితే ((commit = get_revision(&rev)) != ఖాళీ) {
		strbuf_addch(&బహిర్గతం, '\n');
		strbuf_addf(&బహిర్గతం, "commit %s\n",
			oid_గమ్యం_hex(&commit->object.oid));
		pretty_print_commit(&ctx, commit, &బహిర్గతం);
	}
	write_file_buf(git_మార్గం_squash_సందేశం(the_repository), బహిర్గతం.buf, బహిర్గతం.len);
	strbuf_release(&బహిర్గతం);
	release_revisions(&rev);
}

స్థిర శూన్య ముగింపు(నిర్మాణం commit *తల_commit,
		   నిర్మాణం commit_list *remoteheads,
		   స్థిరరాశి నిర్మాణం object_id *new_తల, స్థిరరాశి అక్షరం *సందేశం)
{
	నిర్మాణం strbuf reflog_సందేశం_2 = STRBUF_INIT;
	స్థిరరాశి నిర్మాణం object_id *తల = &తల_commit->object.oid;

	ఒకవేళ (!సందేశం)
		strbuf_addstr(&reflog_సందేశం_2, getenv("GIT_REFLOG_ACTION"));
	లేకపోతే {
		ఒకవేళ (verbosity >= 0)
			రాయి("%s\n", సందేశం);
		strbuf_addf(&reflog_సందేశం_2, "%s: %s",
			getenv("GIT_REFLOG_ACTION"), సందేశం);
	}
	ఒకవేళ (squash) {
		squash_సందేశం(తల_commit, remoteheads);
	} లేకపోతే {
		ఒకవేళ (verbosity >= 0 && !విలీనం_సందేశం.len)
			రాయి(_("No merge message -- not updating HEAD\n"));
		లేకపోతే {
			refs_update_ref(get_main_ref_store(the_repository),
					reflog_సందేశం_2.buf, "HEAD", new_తల,
					తల,
					0, UPDATE_REFS_DIE_ON_ERR);
			/*
			 * We ignore errors in 'gc --auto', since the
			 * user should see them.
			 */
			నిరంతరం_auto_maintenance(the_repository, verbosity < 0);
		}
	}
	ఒకవేళ (new_తల && show_diffstat) {
		నిర్మాణం diff_options opts;
		repo_diff_అమరిక(the_repository, &opts);
		init_diffstat_widths(&opts);

		ఎంపిక (show_diffstat) {
		స్థితి విలీనం_SHOW_DIFFSTAT: /* 1 */
			opts.output_format |=
				DIFF_FORMAT_SUMMARY | DIFF_FORMAT_DIFFSTAT;
			ఆపు;
		స్థితి విలీనం_SHOW_COMPACTSUMMARY: /* 2 */
			opts.output_format |= DIFF_FORMAT_DIFFSTAT;
			opts.flags.stat_with_summary = 1;
			ఆపు;
		అప్రమేయ:
			ఆపు;
		}
		opts.detect_rename = DIFF_DETECT_RENAME;
		diff_అమరిక_పూర్తి(&opts);
		diff_వృక్షం_oid(తల, new_తల, "", &opts);
		diffcore_std(&opts);
		diff_flush(&opts);
	}

	/* Run a post-merge hook */
	నిరంతరం_hooks_l(the_repository, "post-merge", squash ? "1" : "0", ఖాళీ);

	ఒకవేళ (new_తల)
		apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
				    ఖాళీ, ఖాళీ, ఖాళీ, ఖాళీ);
	strbuf_release(&reflog_సందేశం_2);
}

/* Get the name for the merge commit's message. */
స్థిర శూన్య విలీనం_name(స్థిరరాశి అక్షరం *remote, నిర్మాణం strbuf *సందేశం)
{
	నిర్మాణం commit *remote_తల;
	నిర్మాణం object_id branch_తల;
	నిర్మాణం strbuf bname = STRBUF_INIT;
	నిర్మాణం విలీనం_remote_desc *desc;
	స్థిరరాశి అక్షరం *ptr;
	అక్షరం *దొరికింది_ref = ఖాళీ;
	పూర్ణాంకం len, early;

	copy_branchname(the_repository, &bname, remote, 0);
	remote = bname.buf;

	oidclr(&branch_తల, the_repository->hash_algo);
	remote_తల = get_విలీనం_మాతృ(remote);
	ఒకవేళ (!remote_తల)
		die(_("'%s' does not point to a commit"), remote);

	ఒకవేళ (repo_dwim_ref(the_repository, remote, అక్షరశ్రేణి_పొడవు(remote), &branch_తల,
			  &దొరికింది_ref, 0) > 0) {
		ఒకవేళ (starts_with(దొరికింది_ref, "refs/heads/")) {
			strbuf_addf(సందేశం, "%s\t\tbranch '%s' of .\n",
				    oid_గమ్యం_hex(&branch_తల), remote);
			వెళ్ళు cleanup;
		}
		ఒకవేళ (starts_with(దొరికింది_ref, "refs/tags/")) {
			strbuf_addf(సందేశం, "%s\t\ttag '%s' of .\n",
				    oid_గమ్యం_hex(&branch_తల), remote);
			వెళ్ళు cleanup;
		}
		ఒకవేళ (starts_with(దొరికింది_ref, "refs/remotes/")) {
			strbuf_addf(సందేశం, "%s\t\tremote-tracking branch '%s' of .\n",
				    oid_గమ్యం_hex(&branch_తల), remote);
			వెళ్ళు cleanup;
		}
	}

	/* See if remote matches <name>^^^.. or <name>~<number> */
	కొరకు (len = 0, ptr = remote + అక్షరశ్రేణి_పొడవు(remote);
	     remote < ptr && ptr[-1] == '^';
	     ptr--)
		len++;
	ఒకవేళ (len)
		early = 1;
	లేకపోతే {
		early = 0;
		ptr = strrchr(remote, '~');
		ఒకవేళ (ptr) {
			పూర్ణాంకం చూసిన_nonzero = 0;

			len++; /* count ~ */
			అయితే (*++ptr && అంకెనా(*ptr)) {
				చూసిన_nonzero |= (*ptr != '0');
				len++;
			}
			ఒకవేళ (*ptr)
				len = 0; /* not ...~<number> */
			లేకపోతే ఒకవేళ (చూసిన_nonzero)
				early = 1;
			లేకపోతే ఒకవేళ (len == 1)
				early = 1; /* "name~" is "name~1"! */
		}
	}
	ఒకవేళ (len) {
		నిర్మాణం strbuf truname = STRBUF_INIT;
		strbuf_addf(&truname, "refs/heads/%s", remote);
		strbuf_setlen(&truname, truname.len - len);
		ఒకవేళ (refs_ref_exists(get_main_ref_store(the_repository), truname.buf)) {
			strbuf_addf(సందేశం,
				    "%s\t\tbranch '%s'%s of .\n",
				    oid_గమ్యం_hex(&remote_తల->object.oid),
				    truname.buf + 11,
				    (early ? " (early part)" : ""));
			strbuf_release(&truname);
			వెళ్ళు cleanup;
		}
		strbuf_release(&truname);
	}

	desc = విలీనం_remote_util(remote_తల);
	ఒకవేళ (desc && desc->obj && desc->obj->type == OBJ_ట్యాగ్) {
		strbuf_addf(సందేశం, "%s\t\t%s '%s'\n",
			    oid_గమ్యం_hex(&desc->obj->oid),
			    type_name(desc->obj->type),
			    remote);
		వెళ్ళు cleanup;
	}

	strbuf_addf(సందేశం, "%s\t\tcommit '%s'\n",
		oid_గమ్యం_hex(&remote_తల->object.oid), remote);
cleanup:
	విడుదల(దొరికింది_ref);
	strbuf_release(&bname);
}

స్థిర శూన్య parse_branch_విలీనం_options(అక్షరం *bmo)
{
	స్థిరరాశి అక్షరం **argv;
	పూర్ణాంకం argc;

	ఒకవేళ (!bmo)
		తిరిగి;
	argc = విభజించు_cmdline(bmo, &argv);
	ఒకవేళ (argc < 0)
		die(_("Bad branch.%s.mergeoptions string: %s"), branch,
		    _(విభజించు_cmdline_strerror(argc)));
	REALLOC_ARRAY(argv, argc + 2);
	MOVE_ARRAY(argv + 1, argv, argc + 1);
	argc++;
	argv[0] = "branch.*.mergeoptions";
	parse_options(argc, argv, ఖాళీ, builtin_విలీనం_options,
		      builtin_విలీనం_usage, 0);
	విడుదల(argv);
}

స్థిర పూర్ణాంకం git_విలీనం_config(స్థిరరాశి అక్షరం *k, స్థిరరాశి అక్షరం *v,
			    స్థిరరాశి నిర్మాణం config_context *ctx, శూన్య *cb)
{
	పూర్ణాంకం స్థితి_2;
	స్థిరరాశి అక్షరం *str;

	ఒకవేళ (branch &&
	    skip_ఉపసర్గ(k, "branch.", &str) &&
	    skip_ఉపసర్గ(str, branch, &str) &&
	    !strcmp(str, ".mergeoptions")) {
		విడుదల(branch_mergeoptions);
		branch_mergeoptions = xstrdup(v);
		తిరిగి 0;
	}

	ఒకవేళ (!strcmp(k, "merge.diffstat") || !strcmp(k, "merge.stat")) {
		పూర్ణాంకం val = git_parse_maybe_bool_పాఠ్యం(v);
		ఎంపిక (val) {
		స్థితి 0:
			show_diffstat = 0;
			ఆపు;
		స్థితి 1:
			show_diffstat = విలీనం_SHOW_DIFFSTAT;
			ఆపు;
		అప్రమేయ:
			ఒకవేళ (!strcmp(v, "compact"))
				show_diffstat = విలీనం_SHOW_COMPACTSUMMARY;
			/*
			 * We do not need to have an explicit
			 *
			 * else if (!strcmp(v, "diffstat"))
			 *	show_diffstat = MERGE_SHOW_DIFFSTAT;
			 *
			 * here, because the catch-all uses the
			 * diffstat style anyway.
			 */
			లేకపోతే
				/*
				 * A setting from a future?  It is not an
				 * error grave enough to fail the command.
				 * proceed using the default one.
				 */
				show_diffstat = విలీనం_SHOW_DIFFSTAT;
			ఆపు;
		}
	} లేకపోతే ఒకవేళ (!strcmp(k, "merge.verifysignatures")) {
		verify_signatures = git_config_bool(k, v);
	} లేకపోతే ఒకవేళ (!strcmp(k, "pull.twohead")) {
		FREE_AND_NULL(pull_twohead);
		తిరిగి git_config_string(&pull_twohead, k, v);
	} లేకపోతే ఒకవేళ (!strcmp(k, "pull.octopus")) {
		FREE_AND_NULL(pull_octopus);
		తిరిగి git_config_string(&pull_octopus, k, v);
	} లేకపోతే ఒకవేళ (!strcmp(k, "commit.cleanup")) {
		తిరిగి git_config_string(&cleanup_arg, k, v);
	} లేకపోతే ఒకవేళ (!strcmp(k, "merge.ff")) {
		పూర్ణాంకం boolval = git_parse_maybe_bool(v);
		ఒకవేళ (0 <= boolval) {
			fast_forward = boolval ? FF_ALLOW : FF_NO;
		} లేకపోతే ఒకవేళ (v && !strcmp(v, "only")) {
			fast_forward = FF_ONLY;
		} /* do not barf on values from future versions of git */
		తిరిగి 0;
	} లేకపోతే ఒకవేళ (!strcmp(k, "merge.defaulttoupstream")) {
		default_గమ్యం_upstream = git_config_bool(k, v);
		తిరిగి 0;
	} లేకపోతే ఒకవేళ (!strcmp(k, "commit.gpgsign")) {
		sign_commit = git_config_bool(k, v) ? "" : ఖాళీ;
		తిరిగి 0;
	} లేకపోతే ఒకవేళ (!strcmp(k, "gpg.mintrustlevel")) {
		check_trust_level = 0;
	} లేకపోతే ఒకవేళ (!strcmp(k, "merge.autostash")) {
		autostash = git_config_bool(k, v);
		తిరిగి 0;
	}

	స్థితి_2 = fmt_విలీనం_సందేశం_config(k, v, ctx, cb);
	ఒకవేళ (స్థితి_2)
		తిరిగి స్థితి_2;
	తిరిగి git_diff_ui_config(k, v, ctx, cb);
}

స్థిర పూర్ణాంకం read_వృక్షం_trivial(నిర్మాణం object_id *common, నిర్మాణం object_id *తల,
			     నిర్మాణం object_id *one)
{
	పూర్ణాంకం i, nr_trees = 0;
	నిర్మాణం వృక్షం *trees[MAX_UNPACK_TREES];
	నిర్మాణం వృక్షం_desc t[MAX_UNPACK_TREES];
	నిర్మాణం unpack_trees_options opts;

	memset(&opts, 0, పరిమాణం(opts));
	opts.తల_idx = 2;
	opts.src_సూచీ = the_repository->సూచీ;
	opts.dst_సూచీ = the_repository->సూచీ;
	opts.update = 1;
	opts.verbose_update = 1;
	opts.trivial_merges_only = 1;
	opts.విలీనం = 1;
	opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
	trees[nr_trees] = repo_parse_వృక్షం_పరోక్ష(the_repository, common);
	ఒకవేళ (!trees[nr_trees++])
		తిరిగి -1;
	trees[nr_trees] = repo_parse_వృక్షం_పరోక్ష(the_repository, తల);
	ఒకవేళ (!trees[nr_trees++])
		తిరిగి -1;
	trees[nr_trees] = repo_parse_వృక్షం_పరోక్ష(the_repository, one);
	ఒకవేళ (!trees[nr_trees++])
		తిరిగి -1;
	opts.fn = threeway_విలీనం;
	cache_వృక్షం_free(&the_repository->సూచీ->cache_వృక్షం);
	కొరకు (i = 0; i < nr_trees; i++) {
		repo_parse_వృక్షం(the_repository, trees[i]);
		init_వృక్షం_desc(t+i, &trees[i]->object.oid,
			       trees[i]->బఫరు, trees[i]->కొలత);
	}
	ఒకవేళ (unpack_trees(nr_trees, t, &opts))
		తిరిగి -1;
	తిరిగి 0;
}

స్థిర శూన్య write_వృక్షం_trivial(నిర్మాణం object_id *oid)
{
	ఒకవేళ (write_సూచీ_as_వృక్షం(oid, the_repository->సూచీ,
				repo_get_సూచీ_file(the_repository),
				0, ఖాళీ))
		die(_("git write-tree failed to write a tree"));
}

స్థిర పూర్ణాంకం try_విలీనం_strategy(స్థిరరాశి అక్షరం *strategy, నిర్మాణం commit_list *common,
			      నిర్మాణం commit_list *remoteheads,
			      నిర్మాణం commit *తల)
{
	స్థిరరాశి అక్షరం *తల_arg = "HEAD";

	ఒకవేళ (repo_refresh_and_write_సూచీ(the_repository, REFRESH_QUIET,
					 SKIP_IF_UNCHANGED, 0, ఖాళీ, ఖాళీ,
					 ఖాళీ) < 0)
		die(_("Unable to write index."));

	ఒకవేళ (!strcmp(strategy, "recursive") || !strcmp(strategy, "subtree") ||
	    !strcmp(strategy, "ort")) {
		నిర్మాణం తాళం_file తాళం = తాళం_INIT;
		పూర్ణాంకం clean, x;
		నిర్మాణం commit *ఫలితం;
		నిర్మాణం commit_list *reversed = ఖాళీ;
		నిర్మాణం విలీనం_options o;
		నిర్మాణం commit_list *j;

		ఒకవేళ (remoteheads->తదుపరి) {
			error(_("Not handling anything other than two heads merge."));
			తిరిగి 2;
		}

		init_ui_విలీనం_options(&o, the_repository);
		ఒకవేళ (!strcmp(strategy, "subtree"))
			o.subtree_జరుపు = "";

		o.show_rename_పురోగతి =
			show_పురోగతి == -1 ? isatty(2) : show_పురోగతి;

		కొరకు (x = 0; x < xopts.nr; x++)
			ఒకవేళ (parse_విలీనం_opt(&o, xopts.v[x]))
				die(_("unknown strategy option: -X%s"), xopts.v[x]);

		o.branch1 = తల_arg;
		o.branch2 = విలీనం_remote_util(remoteheads->అంశం)->name;

		కొరకు (j = common; j; j = j->తదుపరి)
			commit_list_చొప్పించు(j->అంశం, &reversed);

		repo_hold_locked_సూచీ(the_repository, &తాళం,
				       తాళం_DIE_ON_ERROR);
		clean = విలీనం_ort_recursive(&o, తల, remoteheads->అంశం,
					    reversed, &ఫలితం);
		commit_list_free(reversed);
		strbuf_release(&o.obuf);

		ఒకవేళ (clean < 0) {
			rollback_తాళం_file(&తాళం);
			తిరిగి 2;
		}
		ఒకవేళ (write_locked_సూచీ(the_repository->సూచీ, &తాళం,
				       COMMIT_తాళం | SKIP_IF_UNCHANGED))
			die(_("unable to write %s"), repo_get_సూచీ_file(the_repository));
		తిరిగి clean ? 0 : 1;
	} లేకపోతే {
		తిరిగి try_విలీనం_command(the_repository,
					 strategy, xopts.nr, xopts.v,
					 common, తల_arg, remoteheads);
	}
}

స్థిర శూన్య లెక్క_diff_files(నిర్మాణం diff_వరుస_struct *q,
			     నిర్మాణం diff_options *opt వాడని, శూన్య *దత్తాంశం)
{
	పూర్ణాంకం *లెక్క = దత్తాంశం;

	(*లెక్క) += q->nr;
}

స్థిర పూర్ణాంకం లెక్క_unmerged_entries(శూన్య)
{
	పూర్ణాంకం i, ret = 0;

	కొరకు (i = 0; i < the_repository->సూచీ->cache_nr; i++)
		ఒకవేళ (ce_stage(the_repository->సూచీ->cache[i]))
			ret++;

	తిరిగి ret;
}

స్థిర శూన్య కూడు_strategies(స్థిరరాశి అక్షరం *string, గుర్తులేని attr)
{
	పూర్ణాంకం i;

	ఒకవేళ (string) {
		నిర్మాణం string_list list = STRING_LIST_INIT_DUP;
		నిర్మాణం string_list_అంశం *అంశం;
		string_list_విభజించు(&list, string, " ", -1);
		for_each_string_list_అంశం(అంశం, &list)
			append_strategy(get_strategy(అంశం->string));
		string_list_clear(&list, 0);
		తిరిగి;
	}
	కొరకు (i = 0; i < ARRAY_కొలత(all_strategy); i++)
		ఒకవేళ (all_strategy[i].attr & attr)
			append_strategy(&all_strategy[i]);

}

స్థిర శూన్య read_విలీనం_సందేశం(నిర్మాణం strbuf *సందేశం)
{
	స్థిరరాశి అక్షరం *filename = git_మార్గం_విలీనం_సందేశం(the_repository);
	strbuf_reset(సందేశం);
	ఒకవేళ (strbuf_read_file(సందేశం, filename, 0) < 0)
		die_errno(_("Could not read from '%s'"), filename);
}

స్థిర శూన్య write_విలీనం_state(నిర్మాణం commit_list *);
స్థిర శూన్య abort_commit(నిర్మాణం commit_list *remoteheads, స్థిరరాశి అక్షరం *err_సందేశం)
{
	ఒకవేళ (err_సందేశం)
		error("%s", err_సందేశం);
	fprintf(stderr,
		_("Not committing merge; use 'git commit' to complete the merge.\n"));
	write_విలీనం_state(remoteheads);
	నిష్క్రమించు(1);
}

స్థిర స్థిరరాశి అక్షరం విలీనం_editor_comment[] =
N_("Please enter a commit message to explain why this merge is necessary,\n"
   "especially if it merges an updated upstream into a topic branch.\n"
   "\n");

స్థిర స్థిరరాశి అక్షరం scissors_editor_comment[] =
N_("An empty message aborts the commit.\n");

స్థిర స్థిరరాశి అక్షరం no_scissors_editor_comment[] =
N_("Lines starting with '%s' will be ignored, and an empty message aborts\n"
   "the commit.\n");

స్థిర శూన్య write_విలీనం_heads(నిర్మాణం commit_list *);
స్థిర శూన్య prepare_గమ్యం_commit(నిర్మాణం commit_list *remoteheads)
{
	నిర్మాణం strbuf సందేశం = STRBUF_INIT;
	స్థిరరాశి అక్షరం *సూచీ_file = repo_get_సూచీ_file(the_repository);

	ఒకవేళ (!no_verify) {
		పూర్ణాంకం invoked_hook;

		ఒకవేళ (నిరంతరం_commit_hook(0 < option_edit, సూచీ_file, &invoked_hook,
				    "pre-merge-commit", ఖాళీ))
			abort_commit(remoteheads, ఖాళీ);
		/*
		 * Re-read the index as pre-merge-commit hook could have updated it,
		 * and write it out as a tree.  We must do this before we invoke
		 * the editor and after we invoke run_status above.
		 */
		ఒకవేళ (invoked_hook)
			discard_సూచీ(the_repository->సూచీ);
	}
	read_సూచీ_from(the_repository->సూచీ, సూచీ_file,
			repo_get_git_dir(the_repository));
	strbuf_addbuf(&సందేశం, &విలీనం_సందేశం);
	ఒకవేళ (squash)
		BUG("the control must not reach here under --squash");
	ఒకవేళ (0 < option_edit) {
		strbuf_addch(&సందేశం, '\n');
		ఒకవేళ (cleanup_బహుళకం == COMMIT_సందేశం_CLEANUP_SCISSORS) {
			wt_స్థితి_append_cut_పంక్తి(&సందేశం);
			strbuf_commented_addf(&సందేశం, comment_పంక్తి_str, "\n");
		}
		strbuf_commented_addf(&సందేశం, comment_పంక్తి_str,
				      _(విలీనం_editor_comment));
		ఒకవేళ (cleanup_బహుళకం == COMMIT_సందేశం_CLEANUP_SCISSORS)
			strbuf_commented_addf(&సందేశం, comment_పంక్తి_str,
					      _(scissors_editor_comment));
		లేకపోతే
			strbuf_commented_addf(&సందేశం, comment_పంక్తి_str,
				_(no_scissors_editor_comment), comment_పంక్తి_str);
	}
	ఒకవేళ (signoff)
		append_signoff(&సందేశం, ignored_log_సందేశం_బైట్లు(సందేశం.buf, సందేశం.len), 0);
	write_విలీనం_heads(remoteheads);
	write_file_buf(git_మార్గం_విలీనం_సందేశం(the_repository), సందేశం.buf, సందేశం.len);
	ఒకవేళ (నిరంతరం_commit_hook(0 < option_edit, repo_get_సూచీ_file(the_repository),
			    ఖాళీ, "prepare-commit-msg",
			    git_మార్గం_విలీనం_సందేశం(the_repository), "merge", ఖాళీ))
		abort_commit(remoteheads, ఖాళీ);
	ఒకవేళ (0 < option_edit) {
		ఒకవేళ (launch_editor(git_మార్గం_విలీనం_సందేశం(the_repository), ఖాళీ, ఖాళీ))
			abort_commit(remoteheads, ఖాళీ);
	}

	ఒకవేళ (!no_verify && నిరంతరం_commit_hook(0 < option_edit, repo_get_సూచీ_file(the_repository),
					  ఖాళీ, "commit-msg",
					  git_మార్గం_విలీనం_సందేశం(the_repository), ఖాళీ))
		abort_commit(remoteheads, ఖాళీ);

	read_విలీనం_సందేశం(&సందేశం);
	cleanup_సందేశం(&సందేశం, cleanup_బహుళకం, 0);
	ఒకవేళ (!సందేశం.len)
		abort_commit(remoteheads, _("Empty commit message."));
	strbuf_release(&విలీనం_సందేశం);
	strbuf_addbuf(&విలీనం_సందేశం, &సందేశం);
	strbuf_release(&సందేశం);
}

స్థిర పూర్ణాంకం విలీనం_trivial(నిర్మాణం commit *తల, నిర్మాణం commit_list *remoteheads)
{
	నిర్మాణం object_id ఫలితం_వృక్షం, ఫలితం_commit;
	నిర్మాణం commit_list *parents = ఖాళీ, **pptr = &parents;

	ఒకవేళ (repo_refresh_and_write_సూచీ(the_repository, REFRESH_QUIET,
					 SKIP_IF_UNCHANGED, 0, ఖాళీ, ఖాళీ,
					 ఖాళీ) < 0)
		తిరిగి error(_("Unable to write index."));

	write_వృక్షం_trivial(&ఫలితం_వృక్షం);
	రాయి(_("Wonderful.\n"));
	pptr = commit_list_append(తల, pptr);
	pptr = commit_list_append(remoteheads->అంశం, pptr);
	prepare_గమ్యం_commit(remoteheads);
	ఒకవేళ (commit_వృక్షం(విలీనం_సందేశం.buf, విలీనం_సందేశం.len, &ఫలితం_వృక్షం, parents,
			&ఫలితం_commit, ఖాళీ, sign_commit))
		die(_("failed to write commit object"));
	ముగింపు(తల, remoteheads, &ఫలితం_commit, "In-index merge");

	remove_విలీనం_branch_state(the_repository);
	commit_list_free(parents);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం ముగింపు_automerge(నిర్మాణం commit *తల,
			    పూర్ణాంకం తల_subsumed,
			    నిర్మాణం commit_list *common,
			    నిర్మాణం commit_list *remoteheads,
			    నిర్మాణం object_id *ఫలితం_వృక్షం,
			    స్థిరరాశి అక్షరం *wt_strategy)
{
	నిర్మాణం commit_list *parents = ఖాళీ;
	నిర్మాణం strbuf buf = STRBUF_INIT;
	నిర్మాణం object_id ఫలితం_commit;

	write_వృక్షం_trivial(ఫలితం_వృక్షం);
	commit_list_free(common);
	parents = remoteheads;
	ఒకవేళ (!తల_subsumed || fast_forward == FF_NO)
		commit_list_చొప్పించు(తల, &parents);
	prepare_గమ్యం_commit(remoteheads);
	ఒకవేళ (commit_వృక్షం(విలీనం_సందేశం.buf, విలీనం_సందేశం.len, ఫలితం_వృక్షం, parents,
			&ఫలితం_commit, ఖాళీ, sign_commit))
		die(_("failed to write commit object"));
	strbuf_addf(&buf, "Merge made by the '%s' strategy.", wt_strategy);
	ముగింపు(తల, remoteheads, &ఫలితం_commit, buf.buf);

	strbuf_release(&buf);
	remove_విలీనం_branch_state(the_repository);
	commit_list_free(parents);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం suggest_conflicts(శూన్య)
{
	స్థిరరాశి అక్షరం *filename;
	FILE *fp;
	నిర్మాణం strbuf msgbuf = STRBUF_INIT;

	filename = git_మార్గం_విలీనం_సందేశం(the_repository);
	fp = xfopen(filename, "a");

	/*
	 * We can't use cleanup_mode because if we're not using the editor,
	 * get_cleanup_mode will return COMMIT_MSG_CLEANUP_SPACE instead, even
	 * though the message is meant to be processed later by git-commit.
	 * Thus, we will get the cleanup mode which is returned when we _are_
	 * using an editor.
	 */
	append_conflicts_hint(the_repository->సూచీ, &msgbuf,
			      get_cleanup_బహుళకం(cleanup_arg, 1));
	fputs(msgbuf.buf, fp);
	strbuf_release(&msgbuf);
	fclose(fp);
	repo_rerere(the_repository, allow_rerere_auto);
	రాయి(_("Automatic merge failed; "
			"fix conflicts and then commit the result.\n"));
	తిరిగి 1;
}

స్థిర పూర్ణాంకం evaluate_ఫలితం(శూన్య)
{
	పూర్ణాంకం cnt = 0;
	నిర్మాణం rev_info rev;

	/* Check how many files differ. */
	repo_init_revisions(the_repository, &rev, "");
	అమరిక_revisions(0, ఖాళీ, &rev, ఖాళీ);
	rev.diffopt.output_format |=
		DIFF_FORMAT_CALLBACK;
	rev.diffopt.format_callback = లెక్క_diff_files;
	rev.diffopt.format_callback_దత్తాంశం = &cnt;
	నిరంతరం_diff_files(&rev, 0);

	/*
	 * Check how many unmerged entries are
	 * there.
	 */
	cnt += లెక్క_unmerged_entries();

	release_revisions(&rev);
	తిరిగి cnt;
}

/*
 * Pretend as if the user told us to merge with the remote-tracking
 * branch we have for the upstream of the current branch
 */
స్థిర పూర్ణాంకం అమరిక_with_upstream(స్థిరరాశి అక్షరం ***argv)
{
	నిర్మాణం branch *branch = branch_get(ఖాళీ);
	పూర్ణాంకం i;
	స్థిరరాశి అక్షరం **వాదనలు;

	ఒకవేళ (!branch)
		die(_("No current branch."));
	ఒకవేళ (!branch->remote_name)
		die(_("No remote for the current branch."));
	ఒకవేళ (!branch->విలీనం_nr)
		die(_("No default upstream defined for the current branch."));

	వాదనలు = xcalloc(st_కూడు(branch->విలీనం_nr, 1), పరిమాణం(అక్షరం *));
	కొరకు (i = 0; i < branch->విలీనం_nr; i++) {
		ఒకవేళ (!branch->విలీనం[i]->dst)
			die(_("No remote-tracking branch for %s from %s"),
			    branch->విలీనం[i]->src, branch->remote_name);
		వాదనలు[i] = branch->విలీనం[i]->dst;
	}
	వాదనలు[i] = ఖాళీ;
	*argv = వాదనలు;
	తిరిగి i;
}

స్థిర శూన్య write_విలీనం_heads(నిర్మాణం commit_list *remoteheads)
{
	నిర్మాణం commit_list *j;
	నిర్మాణం strbuf buf = STRBUF_INIT;

	కొరకు (j = remoteheads; j; j = j->తదుపరి) {
		నిర్మాణం object_id *oid;
		నిర్మాణం commit *c = j->అంశం;
		నిర్మాణం విలీనం_remote_desc *desc;

		desc = విలీనం_remote_util(c);
		ఒకవేళ (desc && desc->obj) {
			oid = &desc->obj->oid;
		} లేకపోతే {
			oid = &c->object.oid;
		}
		strbuf_addf(&buf, "%s\n", oid_గమ్యం_hex(oid));
	}
	write_file_buf(git_మార్గం_విలీనం_తల(the_repository), buf.buf, buf.len);

	strbuf_reset(&buf);
	ఒకవేళ (fast_forward == FF_NO)
		strbuf_addstr(&buf, "no-ff");
	write_file_buf(git_మార్గం_విలీనం_బహుళకం(the_repository), buf.buf, buf.len);
	strbuf_release(&buf);
}

స్థిర శూన్య write_విలీనం_state(నిర్మాణం commit_list *remoteheads)
{
	write_విలీనం_heads(remoteheads);
	strbuf_addch(&విలీనం_సందేశం, '\n');
	write_file_buf(git_మార్గం_విలీనం_సందేశం(the_repository), విలీనం_సందేశం.buf,
		       విలీనం_సందేశం.len);
}

స్థిర పూర్ణాంకం default_edit_option(శూన్య)
{
	స్థిర స్థిరరాశి అక్షరం name[] = "GIT_MERGE_AUTOEDIT";
	స్థిరరాశి అక్షరం *e = getenv(name);
	నిర్మాణం stat st_stdin, st_stdout;

	ఒకవేళ (have_సందేశం)
		/* an explicit -m msg without --[no-]edit */
		తిరిగి 0;

	ఒకవేళ (e) {
		పూర్ణాంకం v = git_parse_maybe_bool(e);
		ఒకవేళ (v < 0)
			die(_("Bad value '%s' in environment '%s'"), e, name);
		తిరిగి v;
	}

	/* Use editor if stdin and stdout are the same and is a tty */
	తిరిగి (!fstat(0, &st_stdin) &&
		!fstat(1, &st_stdout) &&
		isatty(0) && isatty(1) &&
		st_stdin.st_dev == st_stdout.st_dev &&
		st_stdin.st_ino == st_stdout.st_ino &&
		st_stdin.st_బహుళకం == st_stdout.st_బహుళకం);
}

స్థిర నిర్మాణం commit_list *reduce_parents(నిర్మాణం commit *తల_commit,
					  పూర్ణాంకం *తల_subsumed,
					  నిర్మాణం commit_list *remoteheads)
{
	నిర్మాణం commit_list *parents, **remotes;

	/*
	 * Is the current HEAD reachable from another commit being
	 * merged?  If so we do not want to record it as a parent of
	 * the resulting merge, unless --no-ff is given.  We will flip
	 * this variable to 0 when we find HEAD among the independent
	 * tips being merged.
	 */
	*తల_subsumed = 1;

	/* Find what parents to record by checking independent ones. */
	parents = reduce_heads(remoteheads);
	commit_list_free(remoteheads);

	remoteheads = ఖాళీ;
	remotes = &remoteheads;
	అయితే (parents) {
		నిర్మాణం commit *commit = తీయి_commit(&parents);
		ఒకవేళ (commit == తల_commit)
			*తల_subsumed = 0;
		లేకపోతే
			remotes = &commit_list_చొప్పించు(commit, remotes)->తదుపరి;
	}
	తిరిగి remoteheads;
}

స్థిర శూన్య prepare_విలీనం_సందేశం(నిర్మాణం strbuf *విలీనం_పేర్లు, నిర్మాణం strbuf *విలీనం_సందేశం)
{
	నిర్మాణం fmt_విలీనం_సందేశం_opts opts;

	memset(&opts, 0, పరిమాణం(opts));
	opts.కూడు_title = !have_సందేశం;
	opts.shortlog_len = shortlog_len;
	opts.credit_people = (0 < option_edit);
	opts.into_name = into_name;

	fmt_విలీనం_సందేశం(విలీనం_పేర్లు, విలీనం_సందేశం, &opts);
	ఒకవేళ (విలీనం_సందేశం->len)
		strbuf_setlen(విలీనం_సందేశం, విలీనం_సందేశం->len - 1);
}

స్థిర శూన్య handle_fetch_తల(నిర్మాణం commit_list **remotes, నిర్మాణం strbuf *విలీనం_పేర్లు)
{
	స్థిరరాశి అక్షరం *filename;
	పూర్ణాంకం fd, pos, npos;
	నిర్మాణం strbuf fetch_తల_file = STRBUF_INIT;
	స్థిరరాశి గుర్తులేని hexsz = the_hash_algo->hexsz;

	ఒకవేళ (!విలీనం_పేర్లు)
		విలీనం_పేర్లు = &fetch_తల_file;

	filename = git_మార్గం_fetch_తల(the_repository);
	fd = xopen(filename, O_RDONLY);

	ఒకవేళ (strbuf_read(విలీనం_పేర్లు, fd, 0) < 0)
		die_errno(_("could not read '%s'"), filename);
	ఒకవేళ (close(fd) < 0)
		die_errno(_("could not close '%s'"), filename);

	కొరకు (pos = 0; pos < విలీనం_పేర్లు->len; pos = npos) {
		నిర్మాణం object_id oid;
		అక్షరం *ptr;
		నిర్మాణం commit *commit;

		ptr = strchr(విలీనం_పేర్లు->buf + pos, '\n');
		ఒకవేళ (ptr)
			npos = ptr - విలీనం_పేర్లు->buf + 1;
		లేకపోతే
			npos = విలీనం_పేర్లు->len;

		ఒకవేళ (npos - pos < hexsz + 2 ||
		    get_oid_hex(విలీనం_పేర్లు->buf + pos, &oid))
			commit = ఖాళీ; /* bad */
		లేకపోతే ఒకవేళ (memcmp(విలీనం_పేర్లు->buf + pos + hexsz, "\t\t", 2))
			కొనసాగు; /* not-for-merge */
		లేకపోతే {
			అక్షరం saved = విలీనం_పేర్లు->buf[pos + hexsz];
			విలీనం_పేర్లు->buf[pos + hexsz] = '\0';
			commit = get_విలీనం_మాతృ(విలీనం_పేర్లు->buf + pos);
			విలీనం_పేర్లు->buf[pos + hexsz] = saved;
		}
		ఒకవేళ (!commit) {
			ఒకవేళ (ptr)
				*ptr = '\0';
			die(_("not something we can merge in %s: %s"),
			    filename, విలీనం_పేర్లు->buf + pos);
		}
		remotes = &commit_list_చొప్పించు(commit, remotes)->తదుపరి;
	}

	ఒకవేళ (విలీనం_పేర్లు == &fetch_తల_file)
		strbuf_release(&fetch_తల_file);
}

స్థిర నిర్మాణం commit_list *collect_parents(నిర్మాణం commit *తల_commit,
					   పూర్ణాంకం *తల_subsumed,
					   పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv,
					   నిర్మాణం strbuf *విలీనం_సందేశం)
{
	పూర్ణాంకం i;
	నిర్మాణం commit_list *remoteheads = ఖాళీ;
	నిర్మాణం commit_list **remotes = &remoteheads;
	నిర్మాణం strbuf విలీనం_పేర్లు = STRBUF_INIT, *autogen = ఖాళీ;

	ఒకవేళ (విలీనం_సందేశం && (!have_సందేశం || shortlog_len))
		autogen = &విలీనం_పేర్లు;

	ఒకవేళ (తల_commit)
		remotes = &commit_list_చొప్పించు(తల_commit, remotes)->తదుపరి;

	ఒకవేళ (argc == 1 && !strcmp(argv[0], "FETCH_HEAD")) {
		handle_fetch_తల(remotes, autogen);
		remoteheads = reduce_parents(తల_commit, తల_subsumed, remoteheads);
	} లేకపోతే {
		కొరకు (i = 0; i < argc; i++) {
			నిర్మాణం commit *commit = get_విలీనం_మాతృ(argv[i]);
			ఒకవేళ (!commit)
				help_unknown_ref(argv[i], "merge",
						 _("not something we can merge"));
			remotes = &commit_list_చొప్పించు(commit, remotes)->తదుపరి;
		}
		remoteheads = reduce_parents(తల_commit, తల_subsumed, remoteheads);
		ఒకవేళ (autogen) {
			నిర్మాణం commit_list *p;
			కొరకు (p = remoteheads; p; p = p->తదుపరి)
				విలీనం_name(విలీనం_remote_util(p->అంశం)->name, autogen);
		}
	}

	ఒకవేళ (autogen) {
		prepare_విలీనం_సందేశం(autogen, విలీనం_సందేశం);
		strbuf_release(autogen);
	}

	తిరిగి remoteheads;
}

స్థిర పూర్ణాంకం merging_a_throwaway_ట్యాగ్(నిర్మాణం commit *commit)
{
	అక్షరం *ట్యాగ్_ref;
	నిర్మాణం object_id oid;
	పూర్ణాంకం is_throwaway_ట్యాగ్ = 0;

	/* Are we merging a tag? */
	ఒకవేళ (!విలీనం_remote_util(commit) ||
	    !విలీనం_remote_util(commit)->obj ||
	    విలీనం_remote_util(commit)->obj->type != OBJ_ట్యాగ్)
		తిరిగి is_throwaway_ట్యాగ్;

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
	ట్యాగ్_ref = xstrfmt("refs/tags/%s",
			  ((నిర్మాణం ట్యాగ్ *)విలీనం_remote_util(commit)->obj)->ట్యాగ్);
	ఒకవేళ (!refs_read_ref(get_main_ref_store(the_repository), ట్యాగ్_ref, &oid) &&
	    oideq(&oid, &విలీనం_remote_util(commit)->obj->oid))
		is_throwaway_ట్యాగ్ = 0;
	లేకపోతే
		is_throwaway_ట్యాగ్ = 1;
	విడుదల(ట్యాగ్_ref);
	తిరిగి is_throwaway_ట్యాగ్;
}

పూర్ణాంకం cmd_విలీనం(పూర్ణాంకం argc,
	      స్థిరరాశి అక్షరం **argv,
	      స్థిరరాశి అక్షరం *ఉపసర్గ,
	      నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం object_id ఫలితం_వృక్షం, stash, తల_oid;
	నిర్మాణం commit *తల_commit;
	నిర్మాణం strbuf buf = STRBUF_INIT;
	పూర్ణాంకం i, ret = 0, తల_subsumed;
	పూర్ణాంకం ఉత్తమ_cnt = -1, విలీనం_was_ok = 0, automerge_was_ok = 0;
	నిర్మాణం commit_list *common = ఖాళీ;
	స్థిరరాశి అక్షరం *ఉత్తమ_strategy = ఖాళీ, *wt_strategy = ఖాళీ;
	నిర్మాణం commit_list *remoteheads = ఖాళీ, *p;
	శూన్య *branch_గమ్యం_free, *argv_గమ్యం_free = ఖాళీ;
	పూర్ణాంకం orig_argc = argc;
	పూర్ణాంకం విలీనం_log_config = -1;

	show_usage_with_options_if_asked(argc, argv,
					 builtin_విలీనం_usage, builtin_విలీనం_options);

#నిర్వచితం_కాకపోతే WITH_BREAKING_CHANGES
	warn_on_auto_comment_char = true;
#అయితే_ముగింపు /* !WITH_BREAKING_CHANGES */
	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_నిండిన_సూచీ = 0;

	/*
	 * Check if we are _not_ on a detached HEAD, i.e. if there is a
	 * current branch.
	 */
	branch = branch_గమ్యం_free = refs_resolve_refdup(get_main_ref_store(the_repository),
						      "HEAD", 0, &తల_oid,
						      ఖాళీ);
	ఒకవేళ (branch)
		skip_ఉపసర్గ(branch, "refs/heads/", &branch);

	init_diff_ui_defaults();
	repo_config(the_repository, git_విలీనం_config, &విలీనం_log_config);

	ఒకవేళ (!branch || is_null_oid(&తల_oid))
		తల_commit = ఖాళీ;
	లేకపోతే
		తల_commit = lookup_commit_or_die(&తల_oid, "HEAD");

	ఒకవేళ (branch_mergeoptions)
		parse_branch_విలీనం_options(branch_mergeoptions);
	argc = parse_options(argc, argv, ఉపసర్గ, builtin_విలీనం_options,
			builtin_విలీనం_usage, 0);
	ఒకవేళ (shortlog_len < 0)
		shortlog_len = (విలీనం_log_config > 0) ? విలీనం_log_config : 0;

	ఒకవేళ (verbosity < 0 && show_పురోగతి == -1)
		show_పురోగతి = 0;

	ఒకవేళ (abort_current_విలీనం) {
		పూర్ణాంకం nargc = 2;
		స్థిరరాశి అక్షరం *nargv[] = {"reset", "--merge", ఖాళీ};
		అక్షరం stash_oid_hex[GIT_MAX_HEXSZ + 1];
		నిర్మాణం object_id stash_oid = {0};

		ఒకవేళ (orig_argc != 2)
			usage_సందేశం_opt(_("--abort expects no arguments"),
			      builtin_విలీనం_usage, builtin_విలీనం_options);

		ఒకవేళ (!file_exists(git_మార్గం_విలీనం_తల(the_repository)))
			die(_("There is no merge to abort (MERGE_HEAD missing)."));

		ఒకవేళ (!refs_read_ref(get_main_ref_store(the_repository), "MERGE_AUTOSTASH", &stash_oid))
			refs_delete_ref(get_main_ref_store(the_repository),
					"", "MERGE_AUTOSTASH", &stash_oid,
					REF_NO_DEREF);

		/* Invoke 'git reset --merge' */
		ret = cmd_reset(nargc, nargv, ఉపసర్గ, the_repository);

		ఒకవేళ (!is_null_oid(&stash_oid)) {
			oid_గమ్యం_hex_r(stash_oid_hex, &stash_oid);
			apply_autostash_oid(stash_oid_hex);
		}

		వెళ్ళు పూర్తి;
	}

	ఒకవేళ (quit_current_విలీనం) {
		ఒకవేళ (orig_argc != 2)
			usage_సందేశం_opt(_("--quit expects no arguments"),
				      builtin_విలీనం_usage,
				      builtin_విలీనం_options);

		remove_విలీనం_branch_state(the_repository);
		వెళ్ళు పూర్తి;
	}

	ఒకవేళ (continue_current_విలీనం) {
		పూర్ణాంకం nargc = 1;
		స్థిరరాశి అక్షరం *nargv[] = {"commit", ఖాళీ};

		ఒకవేళ (orig_argc != 2)
			usage_సందేశం_opt(_("--continue expects no arguments"),
			      builtin_విలీనం_usage, builtin_విలీనం_options);

		ఒకవేళ (!file_exists(git_మార్గం_విలీనం_తల(the_repository)))
			die(_("There is no merge in progress (MERGE_HEAD missing)."));

		/* Invoke 'git commit' */
		ret = cmd_commit(nargc, nargv, ఉపసర్గ, the_repository);
		వెళ్ళు పూర్తి;
	}

	ఒకవేళ (repo_read_సూచీ_unmerged(the_repository))
		die_resolve_conflict("merge");

	ఒకవేళ (file_exists(git_మార్గం_విలీనం_తల(the_repository))) {
		/*
		 * There is no unmerged entry, don't advise 'git
		 * add/rm <file>', just 'git commit'.
		 */
		ఒకవేళ (advice_enabled(ADVICE_RESOLVE_CONFLICT))
			die(_("You have not concluded your merge (MERGE_HEAD exists).\n"
				  "Please, commit your changes before you merge."));
		లేకపోతే
			die(_("You have not concluded your merge (MERGE_HEAD exists)."));
	}
	ఒకవేళ (refs_ref_exists(get_main_ref_store(the_repository), "CHERRY_PICK_HEAD")) {
		ఒకవేళ (advice_enabled(ADVICE_RESOLVE_CONFLICT))
			die(_("You have not concluded your cherry-pick (CHERRY_PICK_HEAD exists).\n"
			    "Please, commit your changes before you merge."));
		లేకపోతే
			die(_("You have not concluded your cherry-pick (CHERRY_PICK_HEAD exists)."));
	}
	resolve_undo_clear_సూచీ(the_repository->సూచీ);

	ఒకవేళ (option_edit < 0)
		option_edit = default_edit_option();

	cleanup_బహుళకం = get_cleanup_బహుళకం(cleanup_arg, 0 < option_edit);

	ఒకవేళ (verbosity < 0)
		show_diffstat = 0;

	ఒకవేళ (squash) {
		ఒకవేళ (fast_forward == FF_NO)
			die(_("options '%s' and '%s' cannot be used together"), "--squash", "--no-ff.");
		ఒకవేళ (option_commit > 0)
			die(_("options '%s' and '%s' cannot be used together"), "--squash", "--commit.");
		/*
		 * squash can now silently disable option_commit - this is not
		 * a problem as it is only overriding the default, not a user
		 * supplied option.
		 */
		option_commit = 0;
	}

	ఒకవేళ (option_commit < 0)
		option_commit = 1;

	ఒకవేళ (!argc) {
		ఒకవేళ (default_గమ్యం_upstream) {
			argc = అమరిక_with_upstream(&argv);
			argv_గమ్యం_free = argv;
		}
		లేకపోతే
			die(_("No commit specified and merge.defaultToUpstream not set."));
	} లేకపోతే ఒకవేళ (argc == 1 && !strcmp(argv[0], "-")) {
		argv[0] = "@{-1}";
	}

	ఒకవేళ (!argc)
		usage_with_options(builtin_విలీనం_usage,
			builtin_విలీనం_options);

	ఒకవేళ (!తల_commit) {
		/*
		 * If the merged head is a valid one there is no reason
		 * to forbid "git merge" into a branch yet to be born.
		 * We do the same for "git pull".
		 */
		నిర్మాణం object_id *remote_తల_oid;
		ఒకవేళ (squash)
			die(_("Squash commit into empty head not supported yet"));
		ఒకవేళ (fast_forward == FF_NO)
			die(_("Non-fast-forward commit does not make sense into "
			    "an empty head"));
		remoteheads = collect_parents(తల_commit, &తల_subsumed,
					      argc, argv, ఖాళీ);
		ఒకవేళ (!remoteheads)
			die(_("%s - not something we can merge"), argv[0]);
		ఒకవేళ (remoteheads->తదుపరి)
			die(_("Can merge only exactly one commit into empty head"));

		ఒకవేళ (verify_signatures)
			verify_విలీనం_signature(remoteheads->అంశం, verbosity,
					       check_trust_level);

		remote_తల_oid = &remoteheads->అంశం->object.oid;
		read_ఖాళీలు(remote_తల_oid);
		refs_update_ref(get_main_ref_store(the_repository),
				"initial pull", "HEAD", remote_తల_oid, ఖాళీ,
				0,
				UPDATE_REFS_DIE_ON_ERR);
		వెళ్ళు పూర్తి;
	}

	/*
	 * All the rest are the commits being merged; prepare
	 * the standard merge summary message to be appended
	 * to the given message.
	 */
	remoteheads = collect_parents(తల_commit, &తల_subsumed,
				      argc, argv, &విలీనం_సందేశం);

	ఒకవేళ (!తల_commit || !argc)
		usage_with_options(builtin_విలీనం_usage,
			builtin_విలీనం_options);

	ఒకవేళ (verify_signatures) {
		కొరకు (p = remoteheads; p; p = p->తదుపరి) {
			verify_విలీనం_signature(p->అంశం, verbosity,
					       check_trust_level);
		}
	}

	strbuf_addstr(&buf, "merge");
	కొరకు (p = remoteheads; p; p = p->తదుపరి)
		strbuf_addf(&buf, " %s", విలీనం_remote_util(p->అంశం)->name);
	setenv("GIT_REFLOG_ACTION", buf.buf, 0);
	strbuf_reset(&buf);

	కొరకు (p = remoteheads; p; p = p->తదుపరి) {
		నిర్మాణం commit *commit = p->అంశం;
		strbuf_addf(&buf, "GITHEAD_%s",
			    oid_గమ్యం_hex(&commit->object.oid));
		setenv(buf.buf, విలీనం_remote_util(commit)->name, 1);
		strbuf_reset(&buf);
		ఒకవేళ (fast_forward != FF_ONLY && merging_a_throwaway_ట్యాగ్(commit))
			fast_forward = FF_NO;
	}

	ఒకవేళ (!use_strategies) {
		ఒకవేళ (!remoteheads)
			; /* already up-to-date */
		లేకపోతే ఒకవేళ (!remoteheads->తదుపరి)
			కూడు_strategies(pull_twohead, DEFAULT_TWOHEAD);
		లేకపోతే
			కూడు_strategies(pull_octopus, DEFAULT_OCTOPUS);
	}

	కొరకు (i = 0; i < use_strategies_nr; i++) {
		ఒకవేళ (use_strategies[i]->attr & NO_FAST_FORWARD)
			fast_forward = FF_NO;
		ఒకవేళ (use_strategies[i]->attr & NO_TRIVIAL)
			allow_trivial = 0;
	}

	ఒకవేళ (!remoteheads)
		; /* already up-to-date */
	లేకపోతే ఒకవేళ (!remoteheads->తదుపరి) {
		ఒకవేళ (repo_get_విలీనం_bases(the_repository, తల_commit,
					 remoteheads->అంశం, &common) < 0) {
			ret = 2;
			వెళ్ళు పూర్తి;
		}
	} లేకపోతే {
		నిర్మాణం commit_list *list = remoteheads;
		commit_list_చొప్పించు(తల_commit, &list);
		ఒకవేళ (get_octopus_విలీనం_bases(list, &common) < 0) {
			విడుదల(list);
			ret = 2;
			వెళ్ళు పూర్తి;
		}
		విడుదల(list);
	}

	refs_update_ref(get_main_ref_store(the_repository),
			"updating ORIG_HEAD", "ORIG_HEAD",
			&తల_commit->object.oid, ఖాళీ, 0,
			UPDATE_REFS_DIE_ON_ERR);

	ఒకవేళ (remoteheads && !common) {
		/* No common ancestors found. */
		ఒకవేళ (!allow_unrelated_histories)
			die(_("refusing to merge unrelated histories"));
		/* otherwise, we need a real merge. */
	} లేకపోతే ఒకవేళ (!remoteheads ||
		 (!remoteheads->తదుపరి && !common->తదుపరి &&
		  common->అంశం == remoteheads->అంశం)) {
		/*
		 * If head can reach all the merge then we are up to date.
		 * but first the most common case of merging one remote.
		 */
		ముగింపు_పైకి_గమ్యం_date();
		వెళ్ళు పూర్తి;
	} లేకపోతే ఒకవేళ (fast_forward != FF_NO && !remoteheads->తదుపరి &&
			!common->తదుపరి &&
			oideq(&common->అంశం->object.oid, &తల_commit->object.oid)) {
		/* Again the most common case of merging one remote. */
		స్థిరరాశి అక్షరం *సందేశం = have_సందేశం ?
			"Fast-forward (no commit created; -m option ignored)" :
			"Fast-forward";
		నిర్మాణం commit *commit;

		ఒకవేళ (verbosity >= 0) {
			రాయి(_("Updating %s..%s\n"),
			       repo_కనుగొను_unique_abbrev(the_repository, &తల_commit->object.oid,
						       DEFAULT_ABBREV),
			       repo_కనుగొను_unique_abbrev(the_repository, &remoteheads->అంశం->object.oid,
						       DEFAULT_ABBREV));
		}
		commit = remoteheads->అంశం;
		ఒకవేళ (!commit) {
			ret = 1;
			వెళ్ళు పూర్తి;
		}

		ఒకవేళ (autostash)
			create_autostash_ref(the_repository, "MERGE_AUTOSTASH",
					     ఖాళీ, false);
		ఒకవేళ (checkout_fast_forward(the_repository,
					  &తల_commit->object.oid,
					  &commit->object.oid,
					  overwrite_ignore)) {
			apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
					    ఖాళీ, ఖాళీ, ఖాళీ, ఖాళీ);
			ret = 1;
			వెళ్ళు పూర్తి;
		}

		ముగింపు(తల_commit, remoteheads, &commit->object.oid, సందేశం);
		remove_విలీనం_branch_state(the_repository);
		వెళ్ళు పూర్తి;
	} లేకపోతే ఒకవేళ (!remoteheads->తదుపరి && common->తదుపరి)
		;
		/*
		 * We are not doing octopus and not fast-forward.  Need
		 * a real merge.
		 */
	లేకపోతే ఒకవేళ (!remoteheads->తదుపరి && !common->తదుపరి && option_commit) {
		/*
		 * We are not doing octopus, not fast-forward, and have
		 * only one common.
		 */
		refresh_సూచీ(the_repository->సూచీ, REFRESH_QUIET, ఖాళీ, ఖాళీ, ఖాళీ);
		ఒకవేళ (allow_trivial && fast_forward != FF_ONLY) {
			/*
			 * Must first ensure that index matches HEAD before
			 * attempting a trivial merge.
			 */
			నిర్మాణం వృక్షం *తల_వృక్షం = repo_get_commit_వృక్షం(the_repository,
								      తల_commit);
			నిర్మాణం strbuf sb = STRBUF_INIT;

			ఒకవేళ (repo_సూచీ_has_changes(the_repository, తల_వృక్షం,
						   &sb)) {
				error(_("Your local changes to the following files would be overwritten by merge:\n  %s"),
				      sb.buf);
				strbuf_release(&sb);
				ret = 2;
				వెళ్ళు పూర్తి;
			}

			/* See if it is really trivial. */
			git_committer_info(IDENT_STRICT);
			రాయి(_("Trying really trivial in-index merge...\n"));
			ఒకవేళ (!read_వృక్షం_trivial(&common->అంశం->object.oid,
					       &తల_commit->object.oid,
					       &remoteheads->అంశం->object.oid)) {
				ret = విలీనం_trivial(తల_commit, remoteheads);
				వెళ్ళు పూర్తి;
			}
			రాయి(_("Nope.\n"));
		}
	} లేకపోతే {
		/*
		 * An octopus.  If we can reach all the remote we are up
		 * to date.
		 */
		పూర్ణాంకం పైకి_గమ్యం_date = 1;
		నిర్మాణం commit_list *j;

		కొరకు (j = remoteheads; j; j = j->తదుపరి) {
			పూర్ణాంకం ret = repo_లోపల_విలీనం_bases(the_repository,
						      j->అంశం, తల_commit);
			ఒకవేళ (ret < 0)
				నిష్క్రమించు(128);
			ఒకవేళ (!ret) {
				పైకి_గమ్యం_date = 0;
				ఆపు;
			}
		}
		ఒకవేళ (పైకి_గమ్యం_date) {
			ముగింపు_పైకి_గమ్యం_date();
			వెళ్ళు పూర్తి;
		}
	}

	ఒకవేళ (fast_forward == FF_ONLY)
		die_ff_impossible();

	ఒకవేళ (autostash)
		create_autostash_ref(the_repository, "MERGE_AUTOSTASH",
				     ఖాళీ, false);

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
	ఒకవేళ (save_state(&stash))
		oidclr(&stash, the_repository->hash_algo);

	కొరకు (i = 0; i < use_strategies_nr; i++) {
		పూర్ణాంకం ret, cnt;
		ఒకవేళ (i) {
			రాయి(_("Rewinding the tree to pristine...\n"));
			restore_state(&తల_commit->object.oid, &stash);
		}
		ఒకవేళ (use_strategies_nr != 1)
			రాయి(_("Trying merge strategy %s...\n"),
				use_strategies[i]->name);
		/*
		 * Remember which strategy left the state in the working
		 * tree.
		 */
		wt_strategy = use_strategies[i]->name;

		ret = try_విలీనం_strategy(wt_strategy,
					 common, remoteheads,
					 తల_commit);
		/*
		 * The backend exits with 1 when conflicts are
		 * left to be resolved, with 2 when it does not
		 * handle the given merge at all.
		 */
		ఒకవేళ (ret < 2) {
			ఒకవేళ (!ret) {
				/*
				 * This strategy worked; no point in trying
				 * another.
				 */
				విలీనం_was_ok = 1;
				ఉత్తమ_strategy = wt_strategy;
				ఆపు;
			}
			cnt = (use_strategies_nr > 1) ? evaluate_ఫలితం() : 0;
			ఒకవేళ (ఉత్తమ_cnt <= 0 || cnt <= ఉత్తమ_cnt) {
				ఉత్తమ_strategy = wt_strategy;
				ఉత్తమ_cnt = cnt;
			}
		}
	}

	/*
	 * If we have a resulting tree, that means the strategy module
	 * auto resolved the merge cleanly.
	 */
	ఒకవేళ (విలీనం_was_ok && option_commit) {
		automerge_was_ok = 1;
		ret = ముగింపు_automerge(తల_commit, తల_subsumed,
				       common, remoteheads,
				       &ఫలితం_వృక్షం, wt_strategy);
		వెళ్ళు పూర్తి;
	}

	/*
	 * Pick the result from the best strategy and have the user fix
	 * it up.
	 */
	ఒకవేళ (!ఉత్తమ_strategy) {
		restore_state(&తల_commit->object.oid, &stash);
		ఒకవేళ (use_strategies_nr > 1)
			fprintf(stderr,
				_("No merge strategy handled the merge.\n"));
		లేకపోతే
			fprintf(stderr, _("Merge with strategy %s failed.\n"),
				use_strategies[0]->name);
		apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
				    ఖాళీ, ఖాళీ, ఖాళీ, ఖాళీ);
		ret = 2;
		వెళ్ళు పూర్తి;
	} లేకపోతే ఒకవేళ (ఉత్తమ_strategy == wt_strategy)
		; /* We already have its result in the working tree. */
	లేకపోతే {
		రాయి(_("Rewinding the tree to pristine...\n"));
		restore_state(&తల_commit->object.oid, &stash);
		రాయి(_("Using the %s strategy to prepare resolving by hand.\n"),
			ఉత్తమ_strategy);
		try_విలీనం_strategy(ఉత్తమ_strategy, common, remoteheads,
				   తల_commit);
	}

	ఒకవేళ (squash) {
		ముగింపు(తల_commit, remoteheads, ఖాళీ, ఖాళీ);

		git_test_write_commit_లేఖాచిత్రం_or_die(the_repository);
	} లేకపోతే
		write_విలీనం_state(remoteheads);

	ఒకవేళ (విలీనం_was_ok)
		fprintf(stderr, _("Automatic merge went well; "
			"stopped before committing as requested\n"));
	లేకపోతే
		ret = suggest_conflicts();
	ఒకవేళ (autostash)
		రాయి(_("When finished, apply stashed changes with `git stash pop`\n"));

పూర్తి:
	ఒకవేళ (!automerge_was_ok) {
		commit_list_free(common);
		commit_list_free(remoteheads);
	}
	strbuf_release(&buf);
	విడుదల(branch_గమ్యం_free);
	విడుదల(argv_గమ్యం_free);
	విడుదల(pull_twohead);
	విడుదల(pull_octopus);
	discard_సూచీ(the_repository->సూచీ);
	తిరిగి ret;
}
