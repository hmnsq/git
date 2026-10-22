/*
 * Builtin "git merge"
 *
 * Copyright (c) 2008 Miklos Vajna <vmiklos@frugalware.org>
 *
 * Based on git-merge.sh by Junio C Hamano.
 */

#परिभाषय USE_THE_REPOSITORY_VARIABLE
#परिभाषय DISABLE_SIGN_COMPARE_WARNINGS

#अन्तर्भाव "builtin.h"

#अन्तर्भाव "abspath.h"
#अन्तर्भाव "advice.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "editor.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "object-name.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "lockfile.h"
#अन्तर्भाव "repository.h"
#अन्तर्भाव "run-command.h"
#अन्तर्भाव "hook.h"
#अन्तर्भाव "diff.h"
#अन्तर्भाव "diff-merges.h"
#अन्तर्भाव "refs.h"
#अन्तर्भाव "refspec.h"
#अन्तर्भाव "commit.h"
#अन्तर्भाव "diffcore.h"
#अन्तर्भाव "path.h"
#अन्तर्भाव "revision.h"
#अन्तर्भाव "unpack-trees.h"
#अन्तर्भाव "cache-tree.h"
#अन्तर्भाव "dir.h"
#अन्तर्भाव "color.h"
#अन्तर्भाव "rerere.h"
#अन्तर्भाव "help.h"
#अन्तर्भाव "merge.h"
#अन्तर्भाव "merge-ort-wrappers.h"
#अन्तर्भाव "resolve-undo.h"
#अन्तर्भाव "remote.h"
#अन्तर्भाव "fmt-merge-msg.h"
#अन्तर्भाव "sequencer.h"
#अन्तर्भाव "string-list.h"
#अन्तर्भाव "tag.h"
#अन्तर्भाव "alias.h"
#अन्तर्भाव "branch.h"
#अन्तर्भाव "commit-reach.h"
#अन्तर्भाव "wt-status.h"
#अन्तर्भाव "commit-graph.h"

#परिभाषय DEFAULT_TWOHEAD (1<<0)
#परिभाषय DEFAULT_OCTOPUS (1<<1)
#परिभाषय NO_FAST_FORWARD (1<<2)
#परिभाषय NO_TRIVIAL      (1<<3)

संरचना strategy {
	ध्रुव वर्ण *name;
	अचिह्नित attr;
};

स्थिर ध्रुव वर्ण * ध्रुव builtin_merge_usage[] = {
	N_("git merge [<options>] [<commit>...]"),
	"git merge --abort",
	"git merge --continue",
	रिक्त
};

#परिभाषय MERGE_SHOW_DIFFSTAT 1
#परिभाषय MERGE_SHOW_COMPACTSUMMARY 2

स्थिर पूर्णाङ्क show_diffstat = MERGE_SHOW_DIFFSTAT, shortlog_len = -1, squash;
स्थिर पूर्णाङ्क option_commit = -1;
स्थिर पूर्णाङ्क option_edit = -1;
स्थिर पूर्णाङ्क allow_trivial = 1, have_सन्देशः, verify_signatures;
स्थिर पूर्णाङ्क check_trust_level = 1;
स्थिर पूर्णाङ्क overwrite_ignore = 1;
स्थिर संरचना strbuf merge_सन्देशः = STRBUF_INIT;
स्थिर संरचना strategy **use_strategies;
स्थिर आकार_t use_strategies_nr, use_strategies_alloc;
स्थिर संरचना strvec xopts = STRVEC_INIT;
स्थिर ध्रुव वर्ण *branch;
स्थिर वर्ण *branch_mergeoptions;
स्थिर पूर्णाङ्क verbosity;
स्थिर पूर्णाङ्क allow_rerere_auto;
स्थिर पूर्णाङ्क abort_current_merge;
स्थिर पूर्णाङ्क quit_current_merge;
स्थिर पूर्णाङ्क continue_current_merge;
स्थिर पूर्णाङ्क allow_unrelated_histories;
स्थिर पूर्णाङ्क show_प्रगतिः = -1;
स्थिर पूर्णाङ्क default_गन्तव्यम्_upstream = 1;
स्थिर पूर्णाङ्क signoff;
स्थिर ध्रुव वर्ण *sign_commit;
स्थिर पूर्णाङ्क autostash;
स्थिर पूर्णाङ्क no_verify;
स्थिर वर्ण *into_name;

स्थिर संरचना strategy all_strategy[] = {
	{ "recursive",  NO_TRIVIAL },
	{ "octopus",    DEFAULT_OCTOPUS },
	{ "ort",        DEFAULT_TWOHEAD | NO_TRIVIAL },
	{ "resolve",    0 },
	{ "ours",       NO_FAST_FORWARD | NO_TRIVIAL },
	{ "subtree",    NO_FAST_FORWARD | NO_TRIVIAL },
};

स्थिर वर्ण *pull_twohead, *pull_octopus;

गणना ff_type {
	FF_NO,
	FF_ALLOW,
	FF_ONLY
};

स्थिर गणना ff_type fast_forward = FF_ALLOW;

स्थिर वर्ण *cleanup_arg;
स्थिर गणना commit_सन्देशः_cleanup_बहुलकम् cleanup_बहुलकम्;

स्थिर पूर्णाङ्क option_parse_सन्देशः(ध्रुव संरचना option *opt,
				ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना strbuf *buf = opt->मानम्;

	यदि (unset)
		strbuf_setlen(buf, 0);
	अन्यथा यदि (arg) {
		strbuf_addf(buf, "%s%s", buf->len ? "\n\n" : "", arg);
		have_सन्देशः = 1;
	} अन्यथा
		प्रत्यागम error(_("switch `m' requires a value"));
	प्रत्यागम 0;
}

स्थिर गणना parse_opt_परिणाम option_read_सन्देशः(संरचना parse_opt_ctx_t *ctx,
						 ध्रुव संरचना option *opt,
						 ध्रुव वर्ण *arg_not_प्रयुक्तम्,
						 पूर्णाङ्क unset)
{
	संरचना strbuf *buf = opt->मानम्;
	ध्रुव वर्ण *arg;

	BUG_ON_OPT_ARG(arg_not_प्रयुक्तम्);
	यदि (unset)
		BUG("-F cannot be negated");

	यदि (ctx->opt) {
		arg = ctx->opt;
		ctx->opt = रिक्त;
	} अन्यथा यदि (ctx->argc > 1) {
		ctx->argc--;
		arg = *++ctx->argv;
	} अन्यथा
		प्रत्यागम error(_("option `%s' requires a value"), opt->long_name);

	यदि (buf->len)
		strbuf_addch(buf, '\n');
	यदि (ctx->उपसर्गः && !is_absolute_मार्गः(arg))
		arg = उपसर्गः_filename(ctx->उपसर्गः, arg);
	यदि (strbuf_read_file(buf, arg, 0) < 0)
		प्रत्यागम error(_("could not read file '%s'"), arg);
	have_सन्देशः = 1;

	प्रत्यागम 0;
}

स्थिर संरचना strategy *get_strategy(ध्रुव वर्ण *name)
{
	पूर्णाङ्क i;
	संरचना strategy *ret;
	स्थिर संरचना cmdnames main_cmds = {0}, other_cmds = {0};
	स्थिर पूर्णाङ्क loaded;
	वर्ण *default_strategy = रिक्त;

	यदि (!name)
		प्रत्यागम रिक्त;

	यदि (default_strategy &&
	    !वाक्यं_तुलय(default_strategy, "ort") &&
	    !वाक्यं_तुलय(name, "recursive")) {
		name = "ort";
	}

	प्रति (i = 0; i < ARRAY_आकार(all_strategy); i++)
		यदि (!वाक्यं_तुलय(name, all_strategy[i].name))
			प्रत्यागम &all_strategy[i];

	यदि (!loaded) {
		संरचना cmdnames not_strategies = {0};
		loaded = 1;

		आरोपय_command_list("git-merge-", &main_cmds, &other_cmds);
		प्रति (i = 0; i < main_cmds.cnt; i++) {
			पूर्णाङ्क j, प्राप्तम्_2 = 0;
			संरचना cmdname *ent = main_cmds.नामानि[i];
			प्रति (j = 0; !प्राप्तम्_2 && j < ARRAY_आकार(all_strategy); j++)
				यदि (!xstrncmpz(all_strategy[j].name, ent->name, ent->len))
					प्राप्तम्_2 = 1;
			यदि (!प्राप्तम्_2)
				सङ्कलय_cmdname(&not_strategies, ent->name, ent->len);
		}
		exclude_cmds(&main_cmds, &not_strategies);

		cmdnames_release(&not_strategies);
	}
	यदि (!is_अन्तर्_cmdlist(&main_cmds, name) && !is_अन्तर्_cmdlist(&other_cmds, name)) {
		fprintf(stderr, _("Could not find merge strategy '%s'.\n"), name);
		fprintf(stderr, _("Available strategies are:"));
		प्रति (i = 0; i < main_cmds.cnt; i++)
			fprintf(stderr, " %s", main_cmds.नामानि[i]->name);
		fprintf(stderr, ".\n");
		यदि (other_cmds.cnt) {
			fprintf(stderr, _("Available custom strategies are:"));
			प्रति (i = 0; i < other_cmds.cnt; i++)
				fprintf(stderr, " %s", other_cmds.नामानि[i]->name);
			fprintf(stderr, ".\n");
		}
		निर्गच्छ(1);
	}

	CALLOC_ARRAY(ret, 1);
	ret->name = xstrdup(name);
	ret->attr = NO_TRIVIAL;

	cmdnames_release(&main_cmds);
	cmdnames_release(&other_cmds);
	प्रत्यागम ret;
}

स्थिर शून्य append_strategy(संरचना strategy *s)
{
	ALLOC_GROW(use_strategies, use_strategies_nr + 1, use_strategies_alloc);
	use_strategies[use_strategies_nr++] = s;
}

स्थिर पूर्णाङ्क option_parse_strategy(ध्रुव संरचना option *opt अप्रयुक्तम्,
				 ध्रुव वर्ण *name, पूर्णाङ्क unset)
{
	यदि (unset)
		प्रत्यागम 0;

	append_strategy(get_strategy(name));
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क option_parse_compact_summary(ध्रुव संरचना option *opt,
					ध्रुव वर्ण *name अप्रयुक्तम्, पूर्णाङ्क unset)
{
	पूर्णाङ्क *setting = opt->मानम्;

	यदि (unset)
		*setting = 0;
	अन्यथा
		*setting = MERGE_SHOW_COMPACTSUMMARY;
	प्रत्यागम 0;
}

स्थिर संरचना option builtin_merge_options[] = {
	OPT_समुच्चयः_INT('n', रिक्त, &show_diffstat,
		N_("do not show a diffstat at the end of the merge"), 0),
	OPT_BOOL(0, "stat", &show_diffstat,
		N_("show a diffstat at the end of the merge")),
	OPT_BOOL(0, "summary", &show_diffstat, N_("(synonym to --stat)")),
	OPT_CALLBACK_F(0, "compact-summary", &show_diffstat, रिक्त,
		       N_("show a compact-summary at the end of the merge"),
		       PARSE_OPT_NOARG,
		       option_parse_compact_summary),
	{
		.type = OPTION_INTEGER,
		.long_name = "log",
		.मानम् = &shortlog_len,
		.precision = परिमाण(shortlog_len),
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
	OPT_समुच्चयः_INT(0, "ff", &fast_forward, N_("allow fast-forward (default)"), FF_ALLOW),
	OPT_समुच्चयः_INT_F(0, "ff-only", &fast_forward,
		      N_("abort if fast-forward is not possible"),
		      FF_ONLY, PARSE_OPT_NONEG),
	OPT_RERERE_AUTOUPDATE(&allow_rerere_auto),
	OPT_BOOL(0, "verify-signatures", &verify_signatures,
		N_("verify that the named commit has a valid GPG signature")),
	OPT_CALLBACK('s', "strategy", रिक्त, N_("strategy"),
		N_("merge strategy to use"), option_parse_strategy),
	OPT_STRVEC('X', "strategy-option", &xopts, N_("option=value"),
		N_("option for selected merge strategy")),
	OPT_CALLBACK('m', "message", &merge_सन्देशः, N_("message"),
		N_("merge commit message (for a non-fast-forward merge)"),
		option_parse_सन्देशः),
	{
		.type = OPTION_LOWLEVEL_CALLBACK,
		.short_name = 'F',
		.long_name = "file",
		.मानम् = &merge_सन्देशः,
		.argh = N_("path"),
		.help = N_("read message from file"),
		.flags = PARSE_OPT_NONEG,
		.ll_callback = option_read_सन्देशः,
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
	OPT_समुच्चयः_INT(0, "progress", &show_प्रगतिः, N_("force progress reporting"), 1),
	{
		.type = OPTION_STRING,
		.short_name = 'S',
		.long_name = "gpg-sign",
		.मानम् = &sign_commit,
		.argh = N_("key-id"),
		.help = N_("GPG sign commit"),
		.flags = PARSE_OPT_OPTARG,
		.defval = (intptr_t) "",
	},
	OPT_AUTOSTASH(&autostash),
	OPT_BOOL(0, "overwrite-ignore", &overwrite_ignore, N_("update ignored files (default)")),
	OPT_BOOL(0, "signoff", &signoff, N_("add a Signed-off-by trailer")),
	OPT_BOOL(0, "no-verify", &no_verify, N_("bypass pre-merge-commit and commit-msg hooks")),
	OPT_अन्तः()
};

स्थिर पूर्णाङ्क save_state(संरचना object_id *stash)
{
	पूर्णाङ्क len;
	संरचना child_process cp = CHILD_PROCESS_INIT;
	संरचना strbuf बफर = STRBUF_INIT;
	संरचना तालकम्_file तालकम्_file = तालकम्_INIT;
	पूर्णाङ्क fd;
	पूर्णाङ्क rc = -1;

	fd = repo_hold_locked_सूचकांक(the_repository, &तालकम्_file, 0);
	refresh_सूचकांक(the_repository->सूचकांक, REFRESH_QUIET, रिक्त, रिक्त, रिक्त);
	यदि (0 <= fd)
		repo_update_सूचकांक_if_able(the_repository, &तालकम्_file);
	rollback_तालकम्_file(&तालकम्_file);

	strvec_pushl(&cp.तर्काः, "stash", "create", रिक्त);
	cp.बहिर्गमनम् = -1;
	cp.git_cmd = 1;

	यदि (start_command(&cp))
		die(_("could not run stash."));
	len = strbuf_read(&बफर, cp.बहिर्गमनम्, 1024);
	close(cp.बहिर्गमनम्);

	यदि (finish_command(&cp) || len < 0)
		die(_("stash failed"));
	अन्यथा यदि (!len)		/* no changes */
		गच्छ बहिर्गमनम्;
	strbuf_setlen(&बफर, बफर.len-1);
	यदि (repo_get_oid(the_repository, बफर.buf, stash))
		die(_("not a valid object: %s"), बफर.buf);
	rc = 0;
बहिर्गमनम्:
	strbuf_release(&बफर);
	प्रत्यागम rc;
}

स्थिर शून्य read_रिक्तानि(ध्रुव संरचना object_id *oid)
{
	संरचना child_process cmd = CHILD_PROCESS_INIT;

	strvec_pushl(&cmd.तर्काः, "read-tree", "-m", "-u",
		     रिक्तानि_वृक्षः_oid_hex(the_repository->hash_algo),
		     oid_गन्तव्यम्_hex(oid), रिक्त);
	cmd.git_cmd = 1;

	यदि (सततम्_command(&cmd))
		die(_("read-tree failed"));
}

स्थिर शून्य reset_hard(ध्रुव संरचना object_id *oid)
{
	संरचना child_process cmd = CHILD_PROCESS_INIT;

	strvec_pushl(&cmd.तर्काः, "read-tree", "-v", "--reset", "-u",
		     oid_गन्तव्यम्_hex(oid), रिक्त);
	cmd.git_cmd = 1;

	यदि (सततम्_command(&cmd))
		die(_("read-tree failed"));
}

स्थिर शून्य restore_state(ध्रुव संरचना object_id *शीर्ष,
			  ध्रुव संरचना object_id *stash)
{
	संरचना child_process cmd = CHILD_PROCESS_INIT;

	reset_hard(शीर्ष);

	यदि (is_null_oid(stash))
		गच्छ refresh_cache;

	strvec_pushl(&cmd.तर्काः, "stash", "apply", "--index", "--quiet", रिक्त);
	strvec_प्रक्षिप(&cmd.तर्काः, oid_गन्तव्यम्_hex(stash));

	/*
	 * It is OK to ignore error here, for example when there was
	 * nothing to restore.
	 */
	cmd.git_cmd = 1;
	सततम्_command(&cmd);

refresh_cache:
	discard_सूचकांक(the_repository->सूचकांक);
	यदि (repo_read_सूचकांक(the_repository) < 0)
		die(_("could not read index"));
}

/* This is called when no merge was necessary. */
स्थिर शून्य finish_ऊर्ध्वम्_गन्तव्यम्_date(शून्य)
{
	यदि (verbosity >= 0) {
		यदि (squash)
			पङ्क्तिं_लिख(_("Already up to date. (nothing to squash)"));
		अन्यथा
			पङ्क्तिं_लिख(_("Already up to date."));
	}
	remove_merge_branch_state(the_repository);
}

स्थिर शून्य squash_सन्देशः(संरचना commit *commit, संरचना commit_list *remoteheads)
{
	संरचना rev_info rev;
	संरचना strbuf बहिर्गमनम् = STRBUF_INIT;
	संरचना commit_list *j;
	संरचना pretty_print_context ctx = {0};

	प्रारूप_लिख(_("Squash commit -- not updating HEAD\n"));

	repo_init_revisions(the_repository, &rev, रिक्त);
	diff_merges_suppress(&rev);
	rev.commit_format = CMIT_FMT_MEDIUM;

	commit->object.flags |= UNINTERESTING;
	सङ्कलय_pending_object(&rev, &commit->object, रिक्त);

	प्रति (j = remoteheads; j; j = j->अग्र)
		सङ्कलय_pending_object(&rev, &j->item->object, रिक्त);

	सज्जता_revisions(0, रिक्त, &rev, रिक्त);
	यदि (prepare_revision_walk(&rev))
		die(_("revision walk setup failed"));

	ctx.abbrev = rev.abbrev;
	ctx.date_बहुलकम् = rev.date_बहुलकम्;
	ctx.fmt = rev.commit_format;

	strbuf_addstr(&बहिर्गमनम्, "Squashed commit of the following:\n");
	यावत् ((commit = get_revision(&rev)) != रिक्त) {
		strbuf_addch(&बहिर्गमनम्, '\n');
		strbuf_addf(&बहिर्गमनम्, "commit %s\n",
			oid_गन्तव्यम्_hex(&commit->object.oid));
		pretty_print_commit(&ctx, commit, &बहिर्गमनम्);
	}
	write_file_buf(git_मार्गः_squash_सन्देशः(the_repository), बहिर्गमनम्.buf, बहिर्गमनम्.len);
	strbuf_release(&बहिर्गमनम्);
	release_revisions(&rev);
}

स्थिर शून्य finish(संरचना commit *शीर्ष_commit,
		   संरचना commit_list *remoteheads,
		   ध्रुव संरचना object_id *new_शीर्ष, ध्रुव वर्ण *सन्देशः)
{
	संरचना strbuf reflog_सन्देशः_2 = STRBUF_INIT;
	ध्रुव संरचना object_id *शीर्ष = &शीर्ष_commit->object.oid;

	यदि (!सन्देशः)
		strbuf_addstr(&reflog_सन्देशः_2, getenv("GIT_REFLOG_ACTION"));
	अन्यथा {
		यदि (verbosity >= 0)
			प्रारूप_लिख("%s\n", सन्देशः);
		strbuf_addf(&reflog_सन्देशः_2, "%s: %s",
			getenv("GIT_REFLOG_ACTION"), सन्देशः);
	}
	यदि (squash) {
		squash_सन्देशः(शीर्ष_commit, remoteheads);
	} अन्यथा {
		यदि (verbosity >= 0 && !merge_सन्देशः.len)
			प्रारूप_लिख(_("No merge message -- not updating HEAD\n"));
		अन्यथा {
			refs_update_ref(get_main_ref_store(the_repository),
					reflog_सन्देशः_2.buf, "HEAD", new_शीर्ष,
					शीर्ष,
					0, UPDATE_REFS_DIE_ON_ERR);
			/*
			 * We ignore errors in 'gc --auto', since the
			 * user should see them.
			 */
			सततम्_auto_maintenance(the_repository, verbosity < 0);
		}
	}
	यदि (new_शीर्ष && show_diffstat) {
		संरचना diff_options opts;
		repo_diff_सज्जता(the_repository, &opts);
		init_diffstat_widths(&opts);

		विकल्प (show_diffstat) {
		पक्ष MERGE_SHOW_DIFFSTAT: /* 1 */
			opts.output_format |=
				DIFF_FORMAT_SUMMARY | DIFF_FORMAT_DIFFSTAT;
			भङ्ग;
		पक्ष MERGE_SHOW_COMPACTSUMMARY: /* 2 */
			opts.output_format |= DIFF_FORMAT_DIFFSTAT;
			opts.flags.stat_with_summary = 1;
			भङ्ग;
		स्वभाव:
			भङ्ग;
		}
		opts.detect_rename = DIFF_DETECT_RENAME;
		diff_सज्जता_समाप्तम्(&opts);
		diff_वृक्षः_oid(शीर्ष, new_शीर्ष, "", &opts);
		diffcore_std(&opts);
		diff_flush(&opts);
	}

	/* Run a post-merge hook */
	सततम्_hooks_l(the_repository, "post-merge", squash ? "1" : "0", रिक्त);

	यदि (new_शीर्ष)
		apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
				    रिक्त, रिक्त, रिक्त, रिक्त);
	strbuf_release(&reflog_सन्देशः_2);
}

/* Get the name for the merge commit's message. */
स्थिर शून्य merge_name(ध्रुव वर्ण *remote, संरचना strbuf *सन्देशः)
{
	संरचना commit *remote_शीर्ष;
	संरचना object_id branch_शीर्ष;
	संरचना strbuf bname = STRBUF_INIT;
	संरचना merge_remote_desc *desc;
	ध्रुव वर्ण *ptr;
	वर्ण *प्राप्तम्_ref = रिक्त;
	पूर्णाङ्क len, early;

	copy_branchname(the_repository, &bname, remote, 0);
	remote = bname.buf;

	oidclr(&branch_शीर्ष, the_repository->hash_algo);
	remote_शीर्ष = get_merge_parent(remote);
	यदि (!remote_शीर्ष)
		die(_("'%s' does not point to a commit"), remote);

	यदि (repo_dwim_ref(the_repository, remote, वाक्यदैर्घ्यम्(remote), &branch_शीर्ष,
			  &प्राप्तम्_ref, 0) > 0) {
		यदि (starts_with(प्राप्तम्_ref, "refs/heads/")) {
			strbuf_addf(सन्देशः, "%s\t\tbranch '%s' of .\n",
				    oid_गन्तव्यम्_hex(&branch_शीर्ष), remote);
			गच्छ cleanup;
		}
		यदि (starts_with(प्राप्तम्_ref, "refs/tags/")) {
			strbuf_addf(सन्देशः, "%s\t\ttag '%s' of .\n",
				    oid_गन्तव्यम्_hex(&branch_शीर्ष), remote);
			गच्छ cleanup;
		}
		यदि (starts_with(प्राप्तम्_ref, "refs/remotes/")) {
			strbuf_addf(सन्देशः, "%s\t\tremote-tracking branch '%s' of .\n",
				    oid_गन्तव्यम्_hex(&branch_शीर्ष), remote);
			गच्छ cleanup;
		}
	}

	/* See if remote matches <name>^^^.. or <name>~<number> */
	प्रति (len = 0, ptr = remote + वाक्यदैर्घ्यम्(remote);
	     remote < ptr && ptr[-1] == '^';
	     ptr--)
		len++;
	यदि (len)
		early = 1;
	अन्यथा {
		early = 0;
		ptr = strrchr(remote, '~');
		यदि (ptr) {
			पूर्णाङ्क आलोकितम्_nonzero = 0;

			len++; /* count ~ */
			यावत् (*++ptr && अङ्कः_वा(*ptr)) {
				आलोकितम्_nonzero |= (*ptr != '0');
				len++;
			}
			यदि (*ptr)
				len = 0; /* not ...~<number> */
			अन्यथा यदि (आलोकितम्_nonzero)
				early = 1;
			अन्यथा यदि (len == 1)
				early = 1; /* "name~" is "name~1"! */
		}
	}
	यदि (len) {
		संरचना strbuf truname = STRBUF_INIT;
		strbuf_addf(&truname, "refs/heads/%s", remote);
		strbuf_setlen(&truname, truname.len - len);
		यदि (refs_ref_exists(get_main_ref_store(the_repository), truname.buf)) {
			strbuf_addf(सन्देशः,
				    "%s\t\tbranch '%s'%s of .\n",
				    oid_गन्तव्यम्_hex(&remote_शीर्ष->object.oid),
				    truname.buf + 11,
				    (early ? " (early part)" : ""));
			strbuf_release(&truname);
			गच्छ cleanup;
		}
		strbuf_release(&truname);
	}

	desc = merge_remote_util(remote_शीर्ष);
	यदि (desc && desc->obj && desc->obj->type == OBJ_चिह्नकम्) {
		strbuf_addf(सन्देशः, "%s\t\t%s '%s'\n",
			    oid_गन्तव्यम्_hex(&desc->obj->oid),
			    type_name(desc->obj->type),
			    remote);
		गच्छ cleanup;
	}

	strbuf_addf(सन्देशः, "%s\t\tcommit '%s'\n",
		oid_गन्तव्यम्_hex(&remote_शीर्ष->object.oid), remote);
cleanup:
	मुक्त(प्राप्तम्_ref);
	strbuf_release(&bname);
}

स्थिर शून्य parse_branch_merge_options(वर्ण *bmo)
{
	ध्रुव वर्ण **argv;
	पूर्णाङ्क argc;

	यदि (!bmo)
		प्रत्यागम;
	argc = विभज_cmdline(bmo, &argv);
	यदि (argc < 0)
		die(_("Bad branch.%s.mergeoptions string: %s"), branch,
		    _(विभज_cmdline_strerror(argc)));
	REALLOC_ARRAY(argv, argc + 2);
	MOVE_ARRAY(argv + 1, argv, argc + 1);
	argc++;
	argv[0] = "branch.*.mergeoptions";
	parse_options(argc, argv, रिक्त, builtin_merge_options,
		      builtin_merge_usage, 0);
	मुक्त(argv);
}

स्थिर पूर्णाङ्क git_merge_config(ध्रुव वर्ण *k, ध्रुव वर्ण *v,
			    ध्रुव संरचना config_context *ctx, शून्य *cb)
{
	पूर्णाङ्क स्थितिः;
	ध्रुव वर्ण *str;

	यदि (branch &&
	    skip_उपसर्गः(k, "branch.", &str) &&
	    skip_उपसर्गः(str, branch, &str) &&
	    !वाक्यं_तुलय(str, ".mergeoptions")) {
		मुक्त(branch_mergeoptions);
		branch_mergeoptions = xstrdup(v);
		प्रत्यागम 0;
	}

	यदि (!वाक्यं_तुलय(k, "merge.diffstat") || !वाक्यं_तुलय(k, "merge.stat")) {
		पूर्णाङ्क val = git_parse_maybe_bool_text(v);
		विकल्प (val) {
		पक्ष 0:
			show_diffstat = 0;
			भङ्ग;
		पक्ष 1:
			show_diffstat = MERGE_SHOW_DIFFSTAT;
			भङ्ग;
		स्वभाव:
			यदि (!वाक्यं_तुलय(v, "compact"))
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
			अन्यथा
				/*
				 * A setting from a future?  It is not an
				 * error grave enough to fail the command.
				 * proceed using the default one.
				 */
				show_diffstat = MERGE_SHOW_DIFFSTAT;
			भङ्ग;
		}
	} अन्यथा यदि (!वाक्यं_तुलय(k, "merge.verifysignatures")) {
		verify_signatures = git_config_bool(k, v);
	} अन्यथा यदि (!वाक्यं_तुलय(k, "pull.twohead")) {
		FREE_AND_NULL(pull_twohead);
		प्रत्यागम git_config_string(&pull_twohead, k, v);
	} अन्यथा यदि (!वाक्यं_तुलय(k, "pull.octopus")) {
		FREE_AND_NULL(pull_octopus);
		प्रत्यागम git_config_string(&pull_octopus, k, v);
	} अन्यथा यदि (!वाक्यं_तुलय(k, "commit.cleanup")) {
		प्रत्यागम git_config_string(&cleanup_arg, k, v);
	} अन्यथा यदि (!वाक्यं_तुलय(k, "merge.ff")) {
		पूर्णाङ्क boolval = git_parse_maybe_bool(v);
		यदि (0 <= boolval) {
			fast_forward = boolval ? FF_ALLOW : FF_NO;
		} अन्यथा यदि (v && !वाक्यं_तुलय(v, "only")) {
			fast_forward = FF_ONLY;
		} /* do not barf on values from future versions of git */
		प्रत्यागम 0;
	} अन्यथा यदि (!वाक्यं_तुलय(k, "merge.defaulttoupstream")) {
		default_गन्तव्यम्_upstream = git_config_bool(k, v);
		प्रत्यागम 0;
	} अन्यथा यदि (!वाक्यं_तुलय(k, "commit.gpgsign")) {
		sign_commit = git_config_bool(k, v) ? "" : रिक्त;
		प्रत्यागम 0;
	} अन्यथा यदि (!वाक्यं_तुलय(k, "gpg.mintrustlevel")) {
		check_trust_level = 0;
	} अन्यथा यदि (!वाक्यं_तुलय(k, "merge.autostash")) {
		autostash = git_config_bool(k, v);
		प्रत्यागम 0;
	}

	स्थितिः = fmt_merge_सन्देशः_config(k, v, ctx, cb);
	यदि (स्थितिः)
		प्रत्यागम स्थितिः;
	प्रत्यागम git_diff_ui_config(k, v, ctx, cb);
}

स्थिर पूर्णाङ्क read_वृक्षः_trivial(संरचना object_id *common, संरचना object_id *शीर्ष,
			     संरचना object_id *one)
{
	पूर्णाङ्क i, nr_trees = 0;
	संरचना वृक्षः *trees[MAX_UNPACK_TREES];
	संरचना वृक्षः_desc t[MAX_UNPACK_TREES];
	संरचना unpack_trees_options opts;

	स्मृतिं_पूरय(&opts, 0, परिमाण(opts));
	opts.शीर्ष_idx = 2;
	opts.src_सूचकांक = the_repository->सूचकांक;
	opts.dst_सूचकांक = the_repository->सूचकांक;
	opts.update = 1;
	opts.verbose_update = 1;
	opts.trivial_merges_only = 1;
	opts.merge = 1;
	opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
	trees[nr_trees] = repo_parse_वृक्षः_परोक्षम्(the_repository, common);
	यदि (!trees[nr_trees++])
		प्रत्यागम -1;
	trees[nr_trees] = repo_parse_वृक्षः_परोक्षम्(the_repository, शीर्ष);
	यदि (!trees[nr_trees++])
		प्रत्यागम -1;
	trees[nr_trees] = repo_parse_वृक्षः_परोक्षम्(the_repository, one);
	यदि (!trees[nr_trees++])
		प्रत्यागम -1;
	opts.fn = threeway_merge;
	cache_वृक्षः_free(&the_repository->सूचकांक->cache_वृक्षः);
	प्रति (i = 0; i < nr_trees; i++) {
		repo_parse_वृक्षः(the_repository, trees[i]);
		init_वृक्षः_desc(t+i, &trees[i]->object.oid,
			       trees[i]->बफर, trees[i]->आकार);
	}
	यदि (unpack_trees(nr_trees, t, &opts))
		प्रत्यागम -1;
	प्रत्यागम 0;
}

स्थिर शून्य write_वृक्षः_trivial(संरचना object_id *oid)
{
	यदि (write_सूचकांक_as_वृक्षः(oid, the_repository->सूचकांक,
				repo_get_सूचकांक_file(the_repository),
				0, रिक्त))
		die(_("git write-tree failed to write a tree"));
}

स्थिर पूर्णाङ्क try_merge_strategy(ध्रुव वर्ण *strategy, संरचना commit_list *common,
			      संरचना commit_list *remoteheads,
			      संरचना commit *शीर्ष)
{
	ध्रुव वर्ण *शीर्ष_arg = "HEAD";

	यदि (repo_refresh_and_write_सूचकांक(the_repository, REFRESH_QUIET,
					 SKIP_IF_UNCHANGED, 0, रिक्त, रिक्त,
					 रिक्त) < 0)
		die(_("Unable to write index."));

	यदि (!वाक्यं_तुलय(strategy, "recursive") || !वाक्यं_तुलय(strategy, "subtree") ||
	    !वाक्यं_तुलय(strategy, "ort")) {
		संरचना तालकम्_file तालकम् = तालकम्_INIT;
		पूर्णाङ्क clean, x;
		संरचना commit *परिणाम;
		संरचना commit_list *reversed = रिक्त;
		संरचना merge_options o;
		संरचना commit_list *j;

		यदि (remoteheads->अग्र) {
			error(_("Not handling anything other than two heads merge."));
			प्रत्यागम 2;
		}

		init_ui_merge_options(&o, the_repository);
		यदि (!वाक्यं_तुलय(strategy, "subtree"))
			o.subtree_सरणम् = "";

		o.show_rename_प्रगतिः =
			show_प्रगतिः == -1 ? isatty(2) : show_प्रगतिः;

		प्रति (x = 0; x < xopts.nr; x++)
			यदि (parse_merge_opt(&o, xopts.v[x]))
				die(_("unknown strategy option: -X%s"), xopts.v[x]);

		o.branch1 = शीर्ष_arg;
		o.branch2 = merge_remote_util(remoteheads->item)->name;

		प्रति (j = common; j; j = j->अग्र)
			commit_list_निवेशय(j->item, &reversed);

		repo_hold_locked_सूचकांक(the_repository, &तालकम्,
				       तालकम्_DIE_ON_ERROR);
		clean = merge_ort_recursive(&o, शीर्ष, remoteheads->item,
					    reversed, &परिणाम);
		commit_list_free(reversed);
		strbuf_release(&o.obuf);

		यदि (clean < 0) {
			rollback_तालकम्_file(&तालकम्);
			प्रत्यागम 2;
		}
		यदि (write_locked_सूचकांक(the_repository->सूचकांक, &तालकम्,
				       COMMIT_तालकम् | SKIP_IF_UNCHANGED))
			die(_("unable to write %s"), repo_get_सूचकांक_file(the_repository));
		प्रत्यागम clean ? 0 : 1;
	} अन्यथा {
		प्रत्यागम try_merge_command(the_repository,
					 strategy, xopts.nr, xopts.v,
					 common, शीर्ष_arg, remoteheads);
	}
}

स्थिर शून्य गणनम्_diff_files(संरचना diff_queue_struct *q,
			     संरचना diff_options *opt अप्रयुक्तम्, शून्य *दत्त)
{
	पूर्णाङ्क *गणनम् = दत्त;

	(*गणनम्) += q->nr;
}

स्थिर पूर्णाङ्क गणनम्_unmerged_entries(शून्य)
{
	पूर्णाङ्क i, ret = 0;

	प्रति (i = 0; i < the_repository->सूचकांक->cache_nr; i++)
		यदि (ce_stage(the_repository->सूचकांक->cache[i]))
			ret++;

	प्रत्यागम ret;
}

स्थिर शून्य सङ्कलय_strategies(ध्रुव वर्ण *string, अचिह्नित attr)
{
	पूर्णाङ्क i;

	यदि (string) {
		संरचना string_list list = STRING_LIST_INIT_DUP;
		संरचना string_list_item *item;
		string_list_विभज(&list, string, " ", -1);
		for_each_string_list_item(item, &list)
			append_strategy(get_strategy(item->string));
		string_list_clear(&list, 0);
		प्रत्यागम;
	}
	प्रति (i = 0; i < ARRAY_आकार(all_strategy); i++)
		यदि (all_strategy[i].attr & attr)
			append_strategy(&all_strategy[i]);

}

स्थिर शून्य read_merge_सन्देशः(संरचना strbuf *सन्देशः)
{
	ध्रुव वर्ण *filename = git_मार्गः_merge_सन्देशः(the_repository);
	strbuf_reset(सन्देशः);
	यदि (strbuf_read_file(सन्देशः, filename, 0) < 0)
		die_errno(_("Could not read from '%s'"), filename);
}

स्थिर शून्य write_merge_state(संरचना commit_list *);
स्थिर शून्य abort_commit(संरचना commit_list *remoteheads, ध्रुव वर्ण *err_सन्देशः)
{
	यदि (err_सन्देशः)
		error("%s", err_सन्देशः);
	fprintf(stderr,
		_("Not committing merge; use 'git commit' to complete the merge.\n"));
	write_merge_state(remoteheads);
	निर्गच्छ(1);
}

स्थिर ध्रुव वर्ण merge_editor_comment[] =
N_("Please enter a commit message to explain why this merge is necessary,\n"
   "especially if it merges an updated upstream into a topic branch.\n"
   "\n");

स्थिर ध्रुव वर्ण scissors_editor_comment[] =
N_("An empty message aborts the commit.\n");

स्थिर ध्रुव वर्ण no_scissors_editor_comment[] =
N_("Lines starting with '%s' will be ignored, and an empty message aborts\n"
   "the commit.\n");

स्थिर शून्य write_merge_heads(संरचना commit_list *);
स्थिर शून्य prepare_गन्तव्यम्_commit(संरचना commit_list *remoteheads)
{
	संरचना strbuf सन्देशः = STRBUF_INIT;
	ध्रुव वर्ण *सूचकांक_file = repo_get_सूचकांक_file(the_repository);

	यदि (!no_verify) {
		पूर्णाङ्क invoked_hook;

		यदि (सततम्_commit_hook(0 < option_edit, सूचकांक_file, &invoked_hook,
				    "pre-merge-commit", रिक्त))
			abort_commit(remoteheads, रिक्त);
		/*
		 * Re-read the index as pre-merge-commit hook could have updated it,
		 * and write it out as a tree.  We must do this before we invoke
		 * the editor and after we invoke run_status above.
		 */
		यदि (invoked_hook)
			discard_सूचकांक(the_repository->सूचकांक);
	}
	read_सूचकांक_from(the_repository->सूचकांक, सूचकांक_file,
			repo_get_git_dir(the_repository));
	strbuf_addbuf(&सन्देशः, &merge_सन्देशः);
	यदि (squash)
		BUG("the control must not reach here under --squash");
	यदि (0 < option_edit) {
		strbuf_addch(&सन्देशः, '\n');
		यदि (cleanup_बहुलकम् == COMMIT_सन्देशः_CLEANUP_SCISSORS) {
			wt_स्थितिः_append_cut_पङ्क्तिः(&सन्देशः);
			strbuf_commented_addf(&सन्देशः, comment_पङ्क्तिः_str, "\n");
		}
		strbuf_commented_addf(&सन्देशः, comment_पङ्क्तिः_str,
				      _(merge_editor_comment));
		यदि (cleanup_बहुलकम् == COMMIT_सन्देशः_CLEANUP_SCISSORS)
			strbuf_commented_addf(&सन्देशः, comment_पङ्क्तिः_str,
					      _(scissors_editor_comment));
		अन्यथा
			strbuf_commented_addf(&सन्देशः, comment_पङ्क्तिः_str,
				_(no_scissors_editor_comment), comment_पङ्क्तिः_str);
	}
	यदि (signoff)
		append_signoff(&सन्देशः, ignored_log_सन्देशः_अष्टकानि(सन्देशः.buf, सन्देशः.len), 0);
	write_merge_heads(remoteheads);
	write_file_buf(git_मार्गः_merge_सन्देशः(the_repository), सन्देशः.buf, सन्देशः.len);
	यदि (सततम्_commit_hook(0 < option_edit, repo_get_सूचकांक_file(the_repository),
			    रिक्त, "prepare-commit-msg",
			    git_मार्गः_merge_सन्देशः(the_repository), "merge", रिक्त))
		abort_commit(remoteheads, रिक्त);
	यदि (0 < option_edit) {
		यदि (launch_editor(git_मार्गः_merge_सन्देशः(the_repository), रिक्त, रिक्त))
			abort_commit(remoteheads, रिक्त);
	}

	यदि (!no_verify && सततम्_commit_hook(0 < option_edit, repo_get_सूचकांक_file(the_repository),
					  रिक्त, "commit-msg",
					  git_मार्गः_merge_सन्देशः(the_repository), रिक्त))
		abort_commit(remoteheads, रिक्त);

	read_merge_सन्देशः(&सन्देशः);
	cleanup_सन्देशः(&सन्देशः, cleanup_बहुलकम्, 0);
	यदि (!सन्देशः.len)
		abort_commit(remoteheads, _("Empty commit message."));
	strbuf_release(&merge_सन्देशः);
	strbuf_addbuf(&merge_सन्देशः, &सन्देशः);
	strbuf_release(&सन्देशः);
}

स्थिर पूर्णाङ्क merge_trivial(संरचना commit *शीर्ष, संरचना commit_list *remoteheads)
{
	संरचना object_id परिणाम_वृक्षः, परिणाम_commit;
	संरचना commit_list *parents = रिक्त, **pptr = &parents;

	यदि (repo_refresh_and_write_सूचकांक(the_repository, REFRESH_QUIET,
					 SKIP_IF_UNCHANGED, 0, रिक्त, रिक्त,
					 रिक्त) < 0)
		प्रत्यागम error(_("Unable to write index."));

	write_वृक्षः_trivial(&परिणाम_वृक्षः);
	प्रारूप_लिख(_("Wonderful.\n"));
	pptr = commit_list_append(शीर्ष, pptr);
	pptr = commit_list_append(remoteheads->item, pptr);
	prepare_गन्तव्यम्_commit(remoteheads);
	यदि (commit_वृक्षः(merge_सन्देशः.buf, merge_सन्देशः.len, &परिणाम_वृक्षः, parents,
			&परिणाम_commit, रिक्त, sign_commit))
		die(_("failed to write commit object"));
	finish(शीर्ष, remoteheads, &परिणाम_commit, "In-index merge");

	remove_merge_branch_state(the_repository);
	commit_list_free(parents);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क finish_automerge(संरचना commit *शीर्ष,
			    पूर्णाङ्क शीर्ष_subsumed,
			    संरचना commit_list *common,
			    संरचना commit_list *remoteheads,
			    संरचना object_id *परिणाम_वृक्षः,
			    ध्रुव वर्ण *wt_strategy)
{
	संरचना commit_list *parents = रिक्त;
	संरचना strbuf buf = STRBUF_INIT;
	संरचना object_id परिणाम_commit;

	write_वृक्षः_trivial(परिणाम_वृक्षः);
	commit_list_free(common);
	parents = remoteheads;
	यदि (!शीर्ष_subsumed || fast_forward == FF_NO)
		commit_list_निवेशय(शीर्ष, &parents);
	prepare_गन्तव्यम्_commit(remoteheads);
	यदि (commit_वृक्षः(merge_सन्देशः.buf, merge_सन्देशः.len, परिणाम_वृक्षः, parents,
			&परिणाम_commit, रिक्त, sign_commit))
		die(_("failed to write commit object"));
	strbuf_addf(&buf, "Merge made by the '%s' strategy.", wt_strategy);
	finish(शीर्ष, remoteheads, &परिणाम_commit, buf.buf);

	strbuf_release(&buf);
	remove_merge_branch_state(the_repository);
	commit_list_free(parents);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क suggest_conflicts(शून्य)
{
	ध्रुव वर्ण *filename;
	FILE *fp;
	संरचना strbuf msgbuf = STRBUF_INIT;

	filename = git_मार्गः_merge_सन्देशः(the_repository);
	fp = xfopen(filename, "a");

	/*
	 * We can't use cleanup_mode because if we're not using the editor,
	 * get_cleanup_mode will return COMMIT_MSG_CLEANUP_SPACE instead, even
	 * though the message is meant to be processed later by git-commit.
	 * Thus, we will get the cleanup mode which is returned when we _are_
	 * using an editor.
	 */
	append_conflicts_hint(the_repository->सूचकांक, &msgbuf,
			      get_cleanup_बहुलकम्(cleanup_arg, 1));
	fputs(msgbuf.buf, fp);
	strbuf_release(&msgbuf);
	fclose(fp);
	repo_rerere(the_repository, allow_rerere_auto);
	प्रारूप_लिख(_("Automatic merge failed; "
			"fix conflicts and then commit the result.\n"));
	प्रत्यागम 1;
}

स्थिर पूर्णाङ्क evaluate_परिणाम(शून्य)
{
	पूर्णाङ्क cnt = 0;
	संरचना rev_info rev;

	/* Check how many files differ. */
	repo_init_revisions(the_repository, &rev, "");
	सज्जता_revisions(0, रिक्त, &rev, रिक्त);
	rev.diffopt.output_format |=
		DIFF_FORMAT_CALLBACK;
	rev.diffopt.format_callback = गणनम्_diff_files;
	rev.diffopt.format_callback_दत्त = &cnt;
	सततम्_diff_files(&rev, 0);

	/*
	 * Check how many unmerged entries are
	 * there.
	 */
	cnt += गणनम्_unmerged_entries();

	release_revisions(&rev);
	प्रत्यागम cnt;
}

/*
 * Pretend as if the user told us to merge with the remote-tracking
 * branch we have for the upstream of the current branch
 */
स्थिर पूर्णाङ्क सज्जता_with_upstream(ध्रुव वर्ण ***argv)
{
	संरचना branch *branch = branch_get(रिक्त);
	पूर्णाङ्क i;
	ध्रुव वर्ण **तर्काः;

	यदि (!branch)
		die(_("No current branch."));
	यदि (!branch->remote_name)
		die(_("No remote for the current branch."));
	यदि (!branch->merge_nr)
		die(_("No default upstream defined for the current branch."));

	तर्काः = xcalloc(st_सङ्कलय(branch->merge_nr, 1), परिमाण(वर्ण *));
	प्रति (i = 0; i < branch->merge_nr; i++) {
		यदि (!branch->merge[i]->dst)
			die(_("No remote-tracking branch for %s from %s"),
			    branch->merge[i]->src, branch->remote_name);
		तर्काः[i] = branch->merge[i]->dst;
	}
	तर्काः[i] = रिक्त;
	*argv = तर्काः;
	प्रत्यागम i;
}

स्थिर शून्य write_merge_heads(संरचना commit_list *remoteheads)
{
	संरचना commit_list *j;
	संरचना strbuf buf = STRBUF_INIT;

	प्रति (j = remoteheads; j; j = j->अग्र) {
		संरचना object_id *oid;
		संरचना commit *c = j->item;
		संरचना merge_remote_desc *desc;

		desc = merge_remote_util(c);
		यदि (desc && desc->obj) {
			oid = &desc->obj->oid;
		} अन्यथा {
			oid = &c->object.oid;
		}
		strbuf_addf(&buf, "%s\n", oid_गन्तव्यम्_hex(oid));
	}
	write_file_buf(git_मार्गः_merge_शीर्ष(the_repository), buf.buf, buf.len);

	strbuf_reset(&buf);
	यदि (fast_forward == FF_NO)
		strbuf_addstr(&buf, "no-ff");
	write_file_buf(git_मार्गः_merge_बहुलकम्(the_repository), buf.buf, buf.len);
	strbuf_release(&buf);
}

स्थिर शून्य write_merge_state(संरचना commit_list *remoteheads)
{
	write_merge_heads(remoteheads);
	strbuf_addch(&merge_सन्देशः, '\n');
	write_file_buf(git_मार्गः_merge_सन्देशः(the_repository), merge_सन्देशः.buf,
		       merge_सन्देशः.len);
}

स्थिर पूर्णाङ्क default_edit_option(शून्य)
{
	स्थिर ध्रुव वर्ण name[] = "GIT_MERGE_AUTOEDIT";
	ध्रुव वर्ण *e = getenv(name);
	संरचना stat st_stdin, st_stdout;

	यदि (have_सन्देशः)
		/* an explicit -m msg without --[no-]edit */
		प्रत्यागम 0;

	यदि (e) {
		पूर्णाङ्क v = git_parse_maybe_bool(e);
		यदि (v < 0)
			die(_("Bad value '%s' in environment '%s'"), e, name);
		प्रत्यागम v;
	}

	/* Use editor if stdin and stdout are the same and is a tty */
	प्रत्यागम (!fstat(0, &st_stdin) &&
		!fstat(1, &st_stdout) &&
		isatty(0) && isatty(1) &&
		st_stdin.st_dev == st_stdout.st_dev &&
		st_stdin.st_ino == st_stdout.st_ino &&
		st_stdin.st_बहुलकम् == st_stdout.st_बहुलकम्);
}

स्थिर संरचना commit_list *reduce_parents(संरचना commit *शीर्ष_commit,
					  पूर्णाङ्क *शीर्ष_subsumed,
					  संरचना commit_list *remoteheads)
{
	संरचना commit_list *parents, **remotes;

	/*
	 * Is the current HEAD reachable from another commit being
	 * merged?  If so we do not want to record it as a parent of
	 * the resulting merge, unless --no-ff is given.  We will flip
	 * this variable to 0 when we find HEAD among the independent
	 * tips being merged.
	 */
	*शीर्ष_subsumed = 1;

	/* Find what parents to record by checking independent ones. */
	parents = reduce_heads(remoteheads);
	commit_list_free(remoteheads);

	remoteheads = रिक्त;
	remotes = &remoteheads;
	यावत् (parents) {
		संरचना commit *commit = निष्कासय_commit(&parents);
		यदि (commit == शीर्ष_commit)
			*शीर्ष_subsumed = 0;
		अन्यथा
			remotes = &commit_list_निवेशय(commit, remotes)->अग्र;
	}
	प्रत्यागम remoteheads;
}

स्थिर शून्य prepare_merge_सन्देशः(संरचना strbuf *merge_नामानि, संरचना strbuf *merge_सन्देशः)
{
	संरचना fmt_merge_सन्देशः_opts opts;

	स्मृतिं_पूरय(&opts, 0, परिमाण(opts));
	opts.सङ्कलय_title = !have_सन्देशः;
	opts.shortlog_len = shortlog_len;
	opts.credit_people = (0 < option_edit);
	opts.into_name = into_name;

	fmt_merge_सन्देशः(merge_नामानि, merge_सन्देशः, &opts);
	यदि (merge_सन्देशः->len)
		strbuf_setlen(merge_सन्देशः, merge_सन्देशः->len - 1);
}

स्थिर शून्य handle_fetch_शीर्ष(संरचना commit_list **remotes, संरचना strbuf *merge_नामानि)
{
	ध्रुव वर्ण *filename;
	पूर्णाङ्क fd, pos, npos;
	संरचना strbuf fetch_शीर्ष_file = STRBUF_INIT;
	ध्रुव अचिह्नित hexsz = the_hash_algo->hexsz;

	यदि (!merge_नामानि)
		merge_नामानि = &fetch_शीर्ष_file;

	filename = git_मार्गः_fetch_शीर्ष(the_repository);
	fd = xopen(filename, O_RDONLY);

	यदि (strbuf_read(merge_नामानि, fd, 0) < 0)
		die_errno(_("could not read '%s'"), filename);
	यदि (close(fd) < 0)
		die_errno(_("could not close '%s'"), filename);

	प्रति (pos = 0; pos < merge_नामानि->len; pos = npos) {
		संरचना object_id oid;
		वर्ण *ptr;
		संरचना commit *commit;

		ptr = strchr(merge_नामानि->buf + pos, '\n');
		यदि (ptr)
			npos = ptr - merge_नामानि->buf + 1;
		अन्यथा
			npos = merge_नामानि->len;

		यदि (npos - pos < hexsz + 2 ||
		    get_oid_hex(merge_नामानि->buf + pos, &oid))
			commit = रिक्त; /* bad */
		अन्यथा यदि (memcmp(merge_नामानि->buf + pos + hexsz, "\t\t", 2))
			अनुवर्त; /* not-for-merge */
		अन्यथा {
			वर्ण saved = merge_नामानि->buf[pos + hexsz];
			merge_नामानि->buf[pos + hexsz] = '\0';
			commit = get_merge_parent(merge_नामानि->buf + pos);
			merge_नामानि->buf[pos + hexsz] = saved;
		}
		यदि (!commit) {
			यदि (ptr)
				*ptr = '\0';
			die(_("not something we can merge in %s: %s"),
			    filename, merge_नामानि->buf + pos);
		}
		remotes = &commit_list_निवेशय(commit, remotes)->अग्र;
	}

	यदि (merge_नामानि == &fetch_शीर्ष_file)
		strbuf_release(&fetch_शीर्ष_file);
}

स्थिर संरचना commit_list *collect_parents(संरचना commit *शीर्ष_commit,
					   पूर्णाङ्क *शीर्ष_subsumed,
					   पूर्णाङ्क argc, ध्रुव वर्ण **argv,
					   संरचना strbuf *merge_सन्देशः)
{
	पूर्णाङ्क i;
	संरचना commit_list *remoteheads = रिक्त;
	संरचना commit_list **remotes = &remoteheads;
	संरचना strbuf merge_नामानि = STRBUF_INIT, *autogen = रिक्त;

	यदि (merge_सन्देशः && (!have_सन्देशः || shortlog_len))
		autogen = &merge_नामानि;

	यदि (शीर्ष_commit)
		remotes = &commit_list_निवेशय(शीर्ष_commit, remotes)->अग्र;

	यदि (argc == 1 && !वाक्यं_तुलय(argv[0], "FETCH_HEAD")) {
		handle_fetch_शीर्ष(remotes, autogen);
		remoteheads = reduce_parents(शीर्ष_commit, शीर्ष_subsumed, remoteheads);
	} अन्यथा {
		प्रति (i = 0; i < argc; i++) {
			संरचना commit *commit = get_merge_parent(argv[i]);
			यदि (!commit)
				help_unknown_ref(argv[i], "merge",
						 _("not something we can merge"));
			remotes = &commit_list_निवेशय(commit, remotes)->अग्र;
		}
		remoteheads = reduce_parents(शीर्ष_commit, शीर्ष_subsumed, remoteheads);
		यदि (autogen) {
			संरचना commit_list *p;
			प्रति (p = remoteheads; p; p = p->अग्र)
				merge_name(merge_remote_util(p->item)->name, autogen);
		}
	}

	यदि (autogen) {
		prepare_merge_सन्देशः(autogen, merge_सन्देशः);
		strbuf_release(autogen);
	}

	प्रत्यागम remoteheads;
}

स्थिर पूर्णाङ्क merging_a_throwaway_चिह्नकम्(संरचना commit *commit)
{
	वर्ण *चिह्नकम्_ref;
	संरचना object_id oid;
	पूर्णाङ्क is_throwaway_चिह्नकम् = 0;

	/* Are we merging a tag? */
	यदि (!merge_remote_util(commit) ||
	    !merge_remote_util(commit)->obj ||
	    merge_remote_util(commit)->obj->type != OBJ_चिह्नकम्)
		प्रत्यागम is_throwaway_चिह्नकम्;

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
	चिह्नकम्_ref = xstrfmt("refs/tags/%s",
			  ((संरचना चिह्नकम् *)merge_remote_util(commit)->obj)->चिह्नकम्);
	यदि (!refs_read_ref(get_main_ref_store(the_repository), चिह्नकम्_ref, &oid) &&
	    oideq(&oid, &merge_remote_util(commit)->obj->oid))
		is_throwaway_चिह्नकम् = 0;
	अन्यथा
		is_throwaway_चिह्नकम् = 1;
	मुक्त(चिह्नकम्_ref);
	प्रत्यागम is_throwaway_चिह्नकम्;
}

पूर्णाङ्क cmd_merge(पूर्णाङ्क argc,
	      ध्रुव वर्ण **argv,
	      ध्रुव वर्ण *उपसर्गः,
	      संरचना repository *repo अप्रयुक्तम्)
{
	संरचना object_id परिणाम_वृक्षः, stash, शीर्ष_oid;
	संरचना commit *शीर्ष_commit;
	संरचना strbuf buf = STRBUF_INIT;
	पूर्णाङ्क i, ret = 0, शीर्ष_subsumed;
	पूर्णाङ्क best_cnt = -1, merge_was_ok = 0, automerge_was_ok = 0;
	संरचना commit_list *common = रिक्त;
	ध्रुव वर्ण *best_strategy = रिक्त, *wt_strategy = रिक्त;
	संरचना commit_list *remoteheads = रिक्त, *p;
	शून्य *branch_गन्तव्यम्_free, *argv_गन्तव्यम्_free = रिक्त;
	पूर्णाङ्क orig_argc = argc;
	पूर्णाङ्क merge_log_config = -1;

	show_usage_with_options_if_asked(argc, argv,
					 builtin_merge_usage, builtin_merge_options);

#यदि_अपरिभाषितम् WITH_BREAKING_CHANGES
	warn_on_auto_comment_char = true;
#यदि_अन्तः /* !WITH_BREAKING_CHANGES */
	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_पूर्णम्_सूचकांक = 0;

	/*
	 * Check if we are _not_ on a detached HEAD, i.e. if there is a
	 * current branch.
	 */
	branch = branch_गन्तव्यम्_free = refs_resolve_refdup(get_main_ref_store(the_repository),
						      "HEAD", 0, &शीर्ष_oid,
						      रिक्त);
	यदि (branch)
		skip_उपसर्गः(branch, "refs/heads/", &branch);

	init_diff_ui_defaults();
	repo_config(the_repository, git_merge_config, &merge_log_config);

	यदि (!branch || is_null_oid(&शीर्ष_oid))
		शीर्ष_commit = रिक्त;
	अन्यथा
		शीर्ष_commit = lookup_commit_or_die(&शीर्ष_oid, "HEAD");

	यदि (branch_mergeoptions)
		parse_branch_merge_options(branch_mergeoptions);
	argc = parse_options(argc, argv, उपसर्गः, builtin_merge_options,
			builtin_merge_usage, 0);
	यदि (shortlog_len < 0)
		shortlog_len = (merge_log_config > 0) ? merge_log_config : 0;

	यदि (verbosity < 0 && show_प्रगतिः == -1)
		show_प्रगतिः = 0;

	यदि (abort_current_merge) {
		पूर्णाङ्क nargc = 2;
		ध्रुव वर्ण *nargv[] = {"reset", "--merge", रिक्त};
		वर्ण stash_oid_hex[GIT_MAX_HEXSZ + 1];
		संरचना object_id stash_oid = {0};

		यदि (orig_argc != 2)
			usage_सन्देशः_opt(_("--abort expects no arguments"),
			      builtin_merge_usage, builtin_merge_options);

		यदि (!file_exists(git_मार्गः_merge_शीर्ष(the_repository)))
			die(_("There is no merge to abort (MERGE_HEAD missing)."));

		यदि (!refs_read_ref(get_main_ref_store(the_repository), "MERGE_AUTOSTASH", &stash_oid))
			refs_delete_ref(get_main_ref_store(the_repository),
					"", "MERGE_AUTOSTASH", &stash_oid,
					REF_NO_DEREF);

		/* Invoke 'git reset --merge' */
		ret = cmd_reset(nargc, nargv, उपसर्गः, the_repository);

		यदि (!is_null_oid(&stash_oid)) {
			oid_गन्तव्यम्_hex_r(stash_oid_hex, &stash_oid);
			apply_autostash_oid(stash_oid_hex);
		}

		गच्छ समाप्तम्;
	}

	यदि (quit_current_merge) {
		यदि (orig_argc != 2)
			usage_सन्देशः_opt(_("--quit expects no arguments"),
				      builtin_merge_usage,
				      builtin_merge_options);

		remove_merge_branch_state(the_repository);
		गच्छ समाप्तम्;
	}

	यदि (continue_current_merge) {
		पूर्णाङ्क nargc = 1;
		ध्रुव वर्ण *nargv[] = {"commit", रिक्त};

		यदि (orig_argc != 2)
			usage_सन्देशः_opt(_("--continue expects no arguments"),
			      builtin_merge_usage, builtin_merge_options);

		यदि (!file_exists(git_मार्गः_merge_शीर्ष(the_repository)))
			die(_("There is no merge in progress (MERGE_HEAD missing)."));

		/* Invoke 'git commit' */
		ret = cmd_commit(nargc, nargv, उपसर्गः, the_repository);
		गच्छ समाप्तम्;
	}

	यदि (repo_read_सूचकांक_unmerged(the_repository))
		die_resolve_conflict("merge");

	यदि (file_exists(git_मार्गः_merge_शीर्ष(the_repository))) {
		/*
		 * There is no unmerged entry, don't advise 'git
		 * add/rm <file>', just 'git commit'.
		 */
		यदि (advice_enabled(ADVICE_RESOLVE_CONFLICT))
			die(_("You have not concluded your merge (MERGE_HEAD exists).\n"
				  "Please, commit your changes before you merge."));
		अन्यथा
			die(_("You have not concluded your merge (MERGE_HEAD exists)."));
	}
	यदि (refs_ref_exists(get_main_ref_store(the_repository), "CHERRY_PICK_HEAD")) {
		यदि (advice_enabled(ADVICE_RESOLVE_CONFLICT))
			die(_("You have not concluded your cherry-pick (CHERRY_PICK_HEAD exists).\n"
			    "Please, commit your changes before you merge."));
		अन्यथा
			die(_("You have not concluded your cherry-pick (CHERRY_PICK_HEAD exists)."));
	}
	resolve_undo_clear_सूचकांक(the_repository->सूचकांक);

	यदि (option_edit < 0)
		option_edit = default_edit_option();

	cleanup_बहुलकम् = get_cleanup_बहुलकम्(cleanup_arg, 0 < option_edit);

	यदि (verbosity < 0)
		show_diffstat = 0;

	यदि (squash) {
		यदि (fast_forward == FF_NO)
			die(_("options '%s' and '%s' cannot be used together"), "--squash", "--no-ff.");
		यदि (option_commit > 0)
			die(_("options '%s' and '%s' cannot be used together"), "--squash", "--commit.");
		/*
		 * squash can now silently disable option_commit - this is not
		 * a problem as it is only overriding the default, not a user
		 * supplied option.
		 */
		option_commit = 0;
	}

	यदि (option_commit < 0)
		option_commit = 1;

	यदि (!argc) {
		यदि (default_गन्तव्यम्_upstream) {
			argc = सज्जता_with_upstream(&argv);
			argv_गन्तव्यम्_free = argv;
		}
		अन्यथा
			die(_("No commit specified and merge.defaultToUpstream not set."));
	} अन्यथा यदि (argc == 1 && !वाक्यं_तुलय(argv[0], "-")) {
		argv[0] = "@{-1}";
	}

	यदि (!argc)
		usage_with_options(builtin_merge_usage,
			builtin_merge_options);

	यदि (!शीर्ष_commit) {
		/*
		 * If the merged head is a valid one there is no reason
		 * to forbid "git merge" into a branch yet to be born.
		 * We do the same for "git pull".
		 */
		संरचना object_id *remote_शीर्ष_oid;
		यदि (squash)
			die(_("Squash commit into empty head not supported yet"));
		यदि (fast_forward == FF_NO)
			die(_("Non-fast-forward commit does not make sense into "
			    "an empty head"));
		remoteheads = collect_parents(शीर्ष_commit, &शीर्ष_subsumed,
					      argc, argv, रिक्त);
		यदि (!remoteheads)
			die(_("%s - not something we can merge"), argv[0]);
		यदि (remoteheads->अग्र)
			die(_("Can merge only exactly one commit into empty head"));

		यदि (verify_signatures)
			verify_merge_signature(remoteheads->item, verbosity,
					       check_trust_level);

		remote_शीर्ष_oid = &remoteheads->item->object.oid;
		read_रिक्तानि(remote_शीर्ष_oid);
		refs_update_ref(get_main_ref_store(the_repository),
				"initial pull", "HEAD", remote_शीर्ष_oid, रिक्त,
				0,
				UPDATE_REFS_DIE_ON_ERR);
		गच्छ समाप्तम्;
	}

	/*
	 * All the rest are the commits being merged; prepare
	 * the standard merge summary message to be appended
	 * to the given message.
	 */
	remoteheads = collect_parents(शीर्ष_commit, &शीर्ष_subsumed,
				      argc, argv, &merge_सन्देशः);

	यदि (!शीर्ष_commit || !argc)
		usage_with_options(builtin_merge_usage,
			builtin_merge_options);

	यदि (verify_signatures) {
		प्रति (p = remoteheads; p; p = p->अग्र) {
			verify_merge_signature(p->item, verbosity,
					       check_trust_level);
		}
	}

	strbuf_addstr(&buf, "merge");
	प्रति (p = remoteheads; p; p = p->अग्र)
		strbuf_addf(&buf, " %s", merge_remote_util(p->item)->name);
	setenv("GIT_REFLOG_ACTION", buf.buf, 0);
	strbuf_reset(&buf);

	प्रति (p = remoteheads; p; p = p->अग्र) {
		संरचना commit *commit = p->item;
		strbuf_addf(&buf, "GITHEAD_%s",
			    oid_गन्तव्यम्_hex(&commit->object.oid));
		setenv(buf.buf, merge_remote_util(commit)->name, 1);
		strbuf_reset(&buf);
		यदि (fast_forward != FF_ONLY && merging_a_throwaway_चिह्नकम्(commit))
			fast_forward = FF_NO;
	}

	यदि (!use_strategies) {
		यदि (!remoteheads)
			; /* already up-to-date */
		अन्यथा यदि (!remoteheads->अग्र)
			सङ्कलय_strategies(pull_twohead, DEFAULT_TWOHEAD);
		अन्यथा
			सङ्कलय_strategies(pull_octopus, DEFAULT_OCTOPUS);
	}

	प्रति (i = 0; i < use_strategies_nr; i++) {
		यदि (use_strategies[i]->attr & NO_FAST_FORWARD)
			fast_forward = FF_NO;
		यदि (use_strategies[i]->attr & NO_TRIVIAL)
			allow_trivial = 0;
	}

	यदि (!remoteheads)
		; /* already up-to-date */
	अन्यथा यदि (!remoteheads->अग्र) {
		यदि (repo_get_merge_bases(the_repository, शीर्ष_commit,
					 remoteheads->item, &common) < 0) {
			ret = 2;
			गच्छ समाप्तम्;
		}
	} अन्यथा {
		संरचना commit_list *list = remoteheads;
		commit_list_निवेशय(शीर्ष_commit, &list);
		यदि (get_octopus_merge_bases(list, &common) < 0) {
			मुक्त(list);
			ret = 2;
			गच्छ समाप्तम्;
		}
		मुक्त(list);
	}

	refs_update_ref(get_main_ref_store(the_repository),
			"updating ORIG_HEAD", "ORIG_HEAD",
			&शीर्ष_commit->object.oid, रिक्त, 0,
			UPDATE_REFS_DIE_ON_ERR);

	यदि (remoteheads && !common) {
		/* No common ancestors found. */
		यदि (!allow_unrelated_histories)
			die(_("refusing to merge unrelated histories"));
		/* otherwise, we need a real merge. */
	} अन्यथा यदि (!remoteheads ||
		 (!remoteheads->अग्र && !common->अग्र &&
		  common->item == remoteheads->item)) {
		/*
		 * If head can reach all the merge then we are up to date.
		 * but first the most common case of merging one remote.
		 */
		finish_ऊर्ध्वम्_गन्तव्यम्_date();
		गच्छ समाप्तम्;
	} अन्यथा यदि (fast_forward != FF_NO && !remoteheads->अग्र &&
			!common->अग्र &&
			oideq(&common->item->object.oid, &शीर्ष_commit->object.oid)) {
		/* Again the most common case of merging one remote. */
		ध्रुव वर्ण *सन्देशः = have_सन्देशः ?
			"Fast-forward (no commit created; -m option ignored)" :
			"Fast-forward";
		संरचना commit *commit;

		यदि (verbosity >= 0) {
			प्रारूप_लिख(_("Updating %s..%s\n"),
			       repo_find_unique_abbrev(the_repository, &शीर्ष_commit->object.oid,
						       DEFAULT_ABBREV),
			       repo_find_unique_abbrev(the_repository, &remoteheads->item->object.oid,
						       DEFAULT_ABBREV));
		}
		commit = remoteheads->item;
		यदि (!commit) {
			ret = 1;
			गच्छ समाप्तम्;
		}

		यदि (autostash)
			create_autostash_ref(the_repository, "MERGE_AUTOSTASH",
					     रिक्त, false);
		यदि (checkout_fast_forward(the_repository,
					  &शीर्ष_commit->object.oid,
					  &commit->object.oid,
					  overwrite_ignore)) {
			apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
					    रिक्त, रिक्त, रिक्त, रिक्त);
			ret = 1;
			गच्छ समाप्तम्;
		}

		finish(शीर्ष_commit, remoteheads, &commit->object.oid, सन्देशः);
		remove_merge_branch_state(the_repository);
		गच्छ समाप्तम्;
	} अन्यथा यदि (!remoteheads->अग्र && common->अग्र)
		;
		/*
		 * We are not doing octopus and not fast-forward.  Need
		 * a real merge.
		 */
	अन्यथा यदि (!remoteheads->अग्र && !common->अग्र && option_commit) {
		/*
		 * We are not doing octopus, not fast-forward, and have
		 * only one common.
		 */
		refresh_सूचकांक(the_repository->सूचकांक, REFRESH_QUIET, रिक्त, रिक्त, रिक्त);
		यदि (allow_trivial && fast_forward != FF_ONLY) {
			/*
			 * Must first ensure that index matches HEAD before
			 * attempting a trivial merge.
			 */
			संरचना वृक्षः *शीर्ष_वृक्षः = repo_get_commit_वृक्षः(the_repository,
								      शीर्ष_commit);
			संरचना strbuf sb = STRBUF_INIT;

			यदि (repo_सूचकांक_has_changes(the_repository, शीर्ष_वृक्षः,
						   &sb)) {
				error(_("Your local changes to the following files would be overwritten by merge:\n  %s"),
				      sb.buf);
				strbuf_release(&sb);
				ret = 2;
				गच्छ समाप्तम्;
			}

			/* See if it is really trivial. */
			git_committer_info(IDENT_STRICT);
			प्रारूप_लिख(_("Trying really trivial in-index merge...\n"));
			यदि (!read_वृक्षः_trivial(&common->item->object.oid,
					       &शीर्ष_commit->object.oid,
					       &remoteheads->item->object.oid)) {
				ret = merge_trivial(शीर्ष_commit, remoteheads);
				गच्छ समाप्तम्;
			}
			प्रारूप_लिख(_("Nope.\n"));
		}
	} अन्यथा {
		/*
		 * An octopus.  If we can reach all the remote we are up
		 * to date.
		 */
		पूर्णाङ्क ऊर्ध्वम्_गन्तव्यम्_date = 1;
		संरचना commit_list *j;

		प्रति (j = remoteheads; j; j = j->अग्र) {
			पूर्णाङ्क ret = repo_अन्तर्_merge_bases(the_repository,
						      j->item, शीर्ष_commit);
			यदि (ret < 0)
				निर्गच्छ(128);
			यदि (!ret) {
				ऊर्ध्वम्_गन्तव्यम्_date = 0;
				भङ्ग;
			}
		}
		यदि (ऊर्ध्वम्_गन्तव्यम्_date) {
			finish_ऊर्ध्वम्_गन्तव्यम्_date();
			गच्छ समाप्तम्;
		}
	}

	यदि (fast_forward == FF_ONLY)
		die_ff_impossible();

	यदि (autostash)
		create_autostash_ref(the_repository, "MERGE_AUTOSTASH",
				     रिक्त, false);

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
	यदि (save_state(&stash))
		oidclr(&stash, the_repository->hash_algo);

	प्रति (i = 0; i < use_strategies_nr; i++) {
		पूर्णाङ्क ret, cnt;
		यदि (i) {
			प्रारूप_लिख(_("Rewinding the tree to pristine...\n"));
			restore_state(&शीर्ष_commit->object.oid, &stash);
		}
		यदि (use_strategies_nr != 1)
			प्रारूप_लिख(_("Trying merge strategy %s...\n"),
				use_strategies[i]->name);
		/*
		 * Remember which strategy left the state in the working
		 * tree.
		 */
		wt_strategy = use_strategies[i]->name;

		ret = try_merge_strategy(wt_strategy,
					 common, remoteheads,
					 शीर्ष_commit);
		/*
		 * The backend exits with 1 when conflicts are
		 * left to be resolved, with 2 when it does not
		 * handle the given merge at all.
		 */
		यदि (ret < 2) {
			यदि (!ret) {
				/*
				 * This strategy worked; no point in trying
				 * another.
				 */
				merge_was_ok = 1;
				best_strategy = wt_strategy;
				भङ्ग;
			}
			cnt = (use_strategies_nr > 1) ? evaluate_परिणाम() : 0;
			यदि (best_cnt <= 0 || cnt <= best_cnt) {
				best_strategy = wt_strategy;
				best_cnt = cnt;
			}
		}
	}

	/*
	 * If we have a resulting tree, that means the strategy module
	 * auto resolved the merge cleanly.
	 */
	यदि (merge_was_ok && option_commit) {
		automerge_was_ok = 1;
		ret = finish_automerge(शीर्ष_commit, शीर्ष_subsumed,
				       common, remoteheads,
				       &परिणाम_वृक्षः, wt_strategy);
		गच्छ समाप्तम्;
	}

	/*
	 * Pick the result from the best strategy and have the user fix
	 * it up.
	 */
	यदि (!best_strategy) {
		restore_state(&शीर्ष_commit->object.oid, &stash);
		यदि (use_strategies_nr > 1)
			fprintf(stderr,
				_("No merge strategy handled the merge.\n"));
		अन्यथा
			fprintf(stderr, _("Merge with strategy %s failed.\n"),
				use_strategies[0]->name);
		apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
				    रिक्त, रिक्त, रिक्त, रिक्त);
		ret = 2;
		गच्छ समाप्तम्;
	} अन्यथा यदि (best_strategy == wt_strategy)
		; /* We already have its result in the working tree. */
	अन्यथा {
		प्रारूप_लिख(_("Rewinding the tree to pristine...\n"));
		restore_state(&शीर्ष_commit->object.oid, &stash);
		प्रारूप_लिख(_("Using the %s strategy to prepare resolving by hand.\n"),
			best_strategy);
		try_merge_strategy(best_strategy, common, remoteheads,
				   शीर्ष_commit);
	}

	यदि (squash) {
		finish(शीर्ष_commit, remoteheads, रिक्त, रिक्त);

		git_test_write_commit_graph_or_die(the_repository);
	} अन्यथा
		write_merge_state(remoteheads);

	यदि (merge_was_ok)
		fprintf(stderr, _("Automatic merge went well; "
			"stopped before committing as requested\n"));
	अन्यथा
		ret = suggest_conflicts();
	यदि (autostash)
		प्रारूप_लिख(_("When finished, apply stashed changes with `git stash pop`\n"));

समाप्तम्:
	यदि (!automerge_was_ok) {
		commit_list_free(common);
		commit_list_free(remoteheads);
	}
	strbuf_release(&buf);
	मुक्त(branch_गन्तव्यम्_free);
	मुक्त(argv_गन्तव्यम्_free);
	मुक्त(pull_twohead);
	मुक्त(pull_octopus);
	discard_सूचकांक(the_repository->सूचकांक);
	प्रत्यागम ret;
}
