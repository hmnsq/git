/*
 * Builtin "git merge"
 *
 * Copyright (c) 2008 Miklos Vajna <vmiklos@frugalware.org>
 *
 * Based on git-merge.sh by Junio C Hamano.
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#അന്തര്ഭാവ "builtin.h"

#അന്തര്ഭാവ "abspath.h"
#അന്തര്ഭാവ "advice.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "editor.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "hex.h"
#അന്തര്ഭാവ "object-name.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "lockfile.h"
#അന്തര്ഭാവ "repository.h"
#അന്തര്ഭാവ "run-command.h"
#അന്തര്ഭാവ "hook.h"
#അന്തര്ഭാവ "diff.h"
#അന്തര്ഭാവ "diff-merges.h"
#അന്തര്ഭാവ "refs.h"
#അന്തര്ഭാവ "refspec.h"
#അന്തര്ഭാവ "commit.h"
#അന്തര്ഭാവ "diffcore.h"
#അന്തര്ഭാവ "path.h"
#അന്തര്ഭാവ "revision.h"
#അന്തര്ഭാവ "unpack-trees.h"
#അന്തര്ഭാവ "cache-tree.h"
#അന്തര്ഭാവ "dir.h"
#അന്തര്ഭാവ "color.h"
#അന്തര്ഭാവ "rerere.h"
#അന്തര്ഭാവ "help.h"
#അന്തര്ഭാവ "merge.h"
#അന്തര്ഭാവ "merge-ort-wrappers.h"
#അന്തര്ഭാവ "resolve-undo.h"
#അന്തര്ഭാവ "remote.h"
#അന്തര്ഭാവ "fmt-merge-msg.h"
#അന്തര്ഭാവ "sequencer.h"
#അന്തര്ഭാവ "string-list.h"
#അന്തര്ഭാവ "tag.h"
#അന്തര്ഭാവ "alias.h"
#അന്തര്ഭാവ "branch.h"
#അന്തര്ഭാവ "commit-reach.h"
#അന്തര്ഭാവ "wt-status.h"
#അന്തര്ഭാവ "commit-graph.h"

#define DEFAULT_TWOHEAD (1<<0)
#define DEFAULT_OCTOPUS (1<<1)
#define NO_FAST_FORWARD (1<<2)
#define NO_TRIVIAL      (1<<3)

ഘടന strategy {
	സ്ഥിരം അക്ഷരം *name;
	unsigned attr;
};

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം builtin_merge_usage[] = {
	N_("git merge [<options>] [<commit>...]"),
	"git merge --abort",
	"git merge --continue",
	ശൂന്യ
};

#define MERGE_SHOW_DIFFSTAT 1
#define MERGE_SHOW_COMPACTSUMMARY 2

സ്ഥിതം പൂർണ്ണസംഖ്യ show_diffstat = MERGE_SHOW_DIFFSTAT, shortlog_len = -1, squash;
സ്ഥിതം പൂർണ്ണസംഖ്യ option_commit = -1;
സ്ഥിതം പൂർണ്ണസംഖ്യ option_edit = -1;
സ്ഥിതം പൂർണ്ണസംഖ്യ allow_trivial = 1, have_message, verify_signatures;
സ്ഥിതം പൂർണ്ണസംഖ്യ check_trust_level = 1;
സ്ഥിതം പൂർണ്ണസംഖ്യ overwrite_ignore = 1;
സ്ഥിതം ഘടന strbuf merge_സന്ദേശം = STRBUF_INIT;
സ്ഥിതം ഘടന strategy **use_strategies;
സ്ഥിതം size_t use_strategies_nr, use_strategies_alloc;
സ്ഥിതം ഘടന strvec xopts = STRVEC_INIT;
സ്ഥിതം സ്ഥിരം അക്ഷരം *branch;
സ്ഥിതം അക്ഷരം *branch_mergeoptions;
സ്ഥിതം പൂർണ്ണസംഖ്യ verbosity;
സ്ഥിതം പൂർണ്ണസംഖ്യ allow_rerere_auto;
സ്ഥിതം പൂർണ്ണസംഖ്യ abort_current_merge;
സ്ഥിതം പൂർണ്ണസംഖ്യ quit_current_merge;
സ്ഥിതം പൂർണ്ണസംഖ്യ continue_current_merge;
സ്ഥിതം പൂർണ്ണസംഖ്യ allow_unrelated_histories;
സ്ഥിതം പൂർണ്ണസംഖ്യ show_progress = -1;
സ്ഥിതം പൂർണ്ണസംഖ്യ default_to_upstream = 1;
സ്ഥിതം പൂർണ്ണസംഖ്യ signoff;
സ്ഥിതം സ്ഥിരം അക്ഷരം *sign_commit;
സ്ഥിതം പൂർണ്ണസംഖ്യ autostash;
സ്ഥിതം പൂർണ്ണസംഖ്യ no_verify;
സ്ഥിതം അക്ഷരം *into_name;

സ്ഥിതം ഘടന strategy all_strategy[] = {
	{ "recursive",  NO_TRIVIAL },
	{ "octopus",    DEFAULT_OCTOPUS },
	{ "ort",        DEFAULT_TWOHEAD | NO_TRIVIAL },
	{ "resolve",    0 },
	{ "ours",       NO_FAST_FORWARD | NO_TRIVIAL },
	{ "subtree",    NO_FAST_FORWARD | NO_TRIVIAL },
};

സ്ഥിതം അക്ഷരം *pull_twohead, *pull_octopus;

എണ്ണൽ ff_type {
	FF_NO,
	FF_ALLOW,
	FF_ONLY
};

സ്ഥിതം എണ്ണൽ ff_type fast_forward = FF_ALLOW;

സ്ഥിതം അക്ഷരം *cleanup_arg;
സ്ഥിതം എണ്ണൽ commit_സന്ദേശം_cleanup_mode cleanup_mode;

സ്ഥിതം പൂർണ്ണസംഖ്യ option_parse_message(സ്ഥിരം ഘടന option *opt,
				സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	ഘടന strbuf *buf = opt->മൂല്യം;

	എങ്കിൽ (unset)
		strbuf_setlen(buf, 0);
	അല്ലെങ്കിൽ എങ്കിൽ (arg) {
		strbuf_addf(buf, "%s%s", buf->len ? "\n\n" : "", arg);
		have_message = 1;
	} അല്ലെങ്കിൽ
		തിരികെ error(_("switch `m' requires a value"));
	തിരികെ 0;
}

സ്ഥിതം എണ്ണൽ parse_opt_result option_read_message(ഘടന parse_opt_ctx_t *ctx,
						 സ്ഥിരം ഘടന option *opt,
						 സ്ഥിരം അക്ഷരം *arg_not_used,
						 പൂർണ്ണസംഖ്യ unset)
{
	ഘടന strbuf *buf = opt->മൂല്യം;
	സ്ഥിരം അക്ഷരം *arg;

	BUG_ON_OPT_ARG(arg_not_used);
	എങ്കിൽ (unset)
		BUG("-F cannot be negated");

	എങ്കിൽ (ctx->opt) {
		arg = ctx->opt;
		ctx->opt = ശൂന്യ;
	} അല്ലെങ്കിൽ എങ്കിൽ (ctx->argc > 1) {
		ctx->argc--;
		arg = *++ctx->argv;
	} അല്ലെങ്കിൽ
		തിരികെ error(_("option `%s' requires a value"), opt->long_name);

	എങ്കിൽ (buf->len)
		strbuf_addch(buf, '\n');
	എങ്കിൽ (ctx->prefix && !is_absolute_path(arg))
		arg = prefix_filename(ctx->prefix, arg);
	എങ്കിൽ (strbuf_read_file(buf, arg, 0) < 0)
		തിരികെ error(_("could not read file '%s'"), arg);
	have_message = 1;

	തിരികെ 0;
}

സ്ഥിതം ഘടന strategy *get_strategy(സ്ഥിരം അക്ഷരം *name)
{
	പൂർണ്ണസംഖ്യ i;
	ഘടന strategy *ret;
	സ്ഥിതം ഘടന cmdnames main_cmds = {0}, other_cmds = {0};
	സ്ഥിതം പൂർണ്ണസംഖ്യ loaded;
	അക്ഷരം *default_strategy = ശൂന്യ;

	എങ്കിൽ (!name)
		തിരികെ ശൂന്യ;

	എങ്കിൽ (default_strategy &&
	    !strcmp(default_strategy, "ort") &&
	    !strcmp(name, "recursive")) {
		name = "ort";
	}

	വേണ്ടി (i = 0; i < ARRAY_SIZE(all_strategy); i++)
		എങ്കിൽ (!strcmp(name, all_strategy[i].name))
			തിരികെ &all_strategy[i];

	എങ്കിൽ (!loaded) {
		ഘടന cmdnames not_strategies = {0};
		loaded = 1;

		load_command_list("git-merge-", &main_cmds, &other_cmds);
		വേണ്ടി (i = 0; i < main_cmds.cnt; i++) {
			പൂർണ്ണസംഖ്യ j, found = 0;
			ഘടന cmdname *ent = main_cmds.names[i];
			വേണ്ടി (j = 0; !found && j < ARRAY_SIZE(all_strategy); j++)
				എങ്കിൽ (!xstrncmpz(all_strategy[j].name, ent->name, ent->len))
					found = 1;
			എങ്കിൽ (!found)
				add_cmdname(&not_strategies, ent->name, ent->len);
		}
		exclude_cmds(&main_cmds, &not_strategies);

		cmdnames_release(&not_strategies);
	}
	എങ്കിൽ (!is_in_cmdlist(&main_cmds, name) && !is_in_cmdlist(&other_cmds, name)) {
		fprintf(stderr, _("Could not find merge strategy '%s'.\n"), name);
		fprintf(stderr, _("Available strategies are:"));
		വേണ്ടി (i = 0; i < main_cmds.cnt; i++)
			fprintf(stderr, " %s", main_cmds.names[i]->name);
		fprintf(stderr, ".\n");
		എങ്കിൽ (other_cmds.cnt) {
			fprintf(stderr, _("Available custom strategies are:"));
			വേണ്ടി (i = 0; i < other_cmds.cnt; i++)
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
	തിരികെ ret;
}

സ്ഥിതം ശൂന്യം append_strategy(ഘടന strategy *s)
{
	ALLOC_GROW(use_strategies, use_strategies_nr + 1, use_strategies_alloc);
	use_strategies[use_strategies_nr++] = s;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ option_parse_strategy(സ്ഥിരം ഘടന option *opt UNUSED,
				 സ്ഥിരം അക്ഷരം *name, പൂർണ്ണസംഖ്യ unset)
{
	എങ്കിൽ (unset)
		തിരികെ 0;

	append_strategy(get_strategy(name));
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ option_parse_compact_summary(സ്ഥിരം ഘടന option *opt,
					സ്ഥിരം അക്ഷരം *name UNUSED, പൂർണ്ണസംഖ്യ unset)
{
	പൂർണ്ണസംഖ്യ *setting = opt->മൂല്യം;

	എങ്കിൽ (unset)
		*setting = 0;
	അല്ലെങ്കിൽ
		*setting = MERGE_SHOW_COMPACTSUMMARY;
	തിരികെ 0;
}

സ്ഥിതം ഘടന option builtin_merge_options[] = {
	OPT_SET_INT('n', ശൂന്യ, &show_diffstat,
		N_("do not show a diffstat at the end of the merge"), 0),
	OPT_BOOL(0, "stat", &show_diffstat,
		N_("show a diffstat at the end of the merge")),
	OPT_BOOL(0, "summary", &show_diffstat, N_("(synonym to --stat)")),
	OPT_CALLBACK_F(0, "compact-summary", &show_diffstat, ശൂന്യ,
		       N_("show a compact-summary at the end of the merge"),
		       PARSE_OPT_NOARG,
		       option_parse_compact_summary),
	{
		.type = OPTION_INTEGER,
		.long_name = "log",
		.മൂല്യം = &shortlog_len,
		.precision = വലുപ്പം(shortlog_len),
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
	OPT_CALLBACK('s', "strategy", ശൂന്യ, N_("strategy"),
		N_("merge strategy to use"), option_parse_strategy),
	OPT_STRVEC('X', "strategy-option", &xopts, N_("option=value"),
		N_("option for selected merge strategy")),
	OPT_CALLBACK('m', "message", &merge_സന്ദേശം, N_("message"),
		N_("merge commit message (for a non-fast-forward merge)"),
		option_parse_message),
	{
		.type = OPTION_LOWLEVEL_CALLBACK,
		.short_name = 'F',
		.long_name = "file",
		.മൂല്യം = &merge_സന്ദേശം,
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
		.മൂല്യം = &sign_commit,
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

സ്ഥിതം പൂർണ്ണസംഖ്യ save_state(ഘടന object_id *stash)
{
	പൂർണ്ണസംഖ്യ len;
	ഘടന child_process cp = CHILD_PROCESS_INIT;
	ഘടന strbuf buffer = STRBUF_INIT;
	ഘടന lock_file lock_file = LOCK_INIT;
	പൂർണ്ണസംഖ്യ fd;
	പൂർണ്ണസംഖ്യ rc = -1;

	fd = repo_hold_locked_index(the_repository, &lock_file, 0);
	refresh_index(the_repository->index, REFRESH_QUIET, ശൂന്യ, ശൂന്യ, ശൂന്യ);
	എങ്കിൽ (0 <= fd)
		repo_update_index_if_able(the_repository, &lock_file);
	rollback_lock_file(&lock_file);

	strvec_pushl(&cp.args, "stash", "create", ശൂന്യ);
	cp.out = -1;
	cp.git_cmd = 1;

	എങ്കിൽ (start_command(&cp))
		die(_("could not run stash."));
	len = strbuf_read(&buffer, cp.out, 1024);
	close(cp.out);

	എങ്കിൽ (finish_command(&cp) || len < 0)
		die(_("stash failed"));
	അല്ലെങ്കിൽ എങ്കിൽ (!len)		/* no changes */
		പോകുക out;
	strbuf_setlen(&buffer, buffer.len-1);
	എങ്കിൽ (repo_get_oid(the_repository, buffer.buf, stash))
		die(_("not a valid object: %s"), buffer.buf);
	rc = 0;
out:
	strbuf_release(&buffer);
	തിരികെ rc;
}

സ്ഥിതം ശൂന്യം read_empty(സ്ഥിരം ഘടന object_id *oid)
{
	ഘടന child_process cmd = CHILD_PROCESS_INIT;

	strvec_pushl(&cmd.args, "read-tree", "-m", "-u",
		     empty_വൃക്ഷം_oid_hex(the_repository->hash_algo),
		     oid_to_hex(oid), ശൂന്യ);
	cmd.git_cmd = 1;

	എങ്കിൽ (run_command(&cmd))
		die(_("read-tree failed"));
}

സ്ഥിതം ശൂന്യം reset_hard(സ്ഥിരം ഘടന object_id *oid)
{
	ഘടന child_process cmd = CHILD_PROCESS_INIT;

	strvec_pushl(&cmd.args, "read-tree", "-v", "--reset", "-u",
		     oid_to_hex(oid), ശൂന്യ);
	cmd.git_cmd = 1;

	എങ്കിൽ (run_command(&cmd))
		die(_("read-tree failed"));
}

സ്ഥിതം ശൂന്യം restore_state(സ്ഥിരം ഘടന object_id *head,
			  സ്ഥിരം ഘടന object_id *stash)
{
	ഘടന child_process cmd = CHILD_PROCESS_INIT;

	reset_hard(head);

	എങ്കിൽ (is_null_oid(stash))
		പോകുക refresh_cache;

	strvec_pushl(&cmd.args, "stash", "apply", "--index", "--quiet", ശൂന്യ);
	strvec_തള്ളുക(&cmd.args, oid_to_hex(stash));

	/*
	 * It is OK to ignore error here, for example when there was
	 * nothing to restore.
	 */
	cmd.git_cmd = 1;
	run_command(&cmd);

refresh_cache:
	discard_index(the_repository->index);
	എങ്കിൽ (repo_read_index(the_repository) < 0)
		die(_("could not read index"));
}

/* This is called when no merge was necessary. */
സ്ഥിതം ശൂന്യം finish_up_to_date(ശൂന്യം)
{
	എങ്കിൽ (verbosity >= 0) {
		എങ്കിൽ (squash)
			puts(_("Already up to date. (nothing to squash)"));
		അല്ലെങ്കിൽ
			puts(_("Already up to date."));
	}
	remove_merge_branch_state(the_repository);
}

സ്ഥിതം ശൂന്യം squash_message(ഘടന commit *commit, ഘടന commit_list *remoteheads)
{
	ഘടന rev_info rev;
	ഘടന strbuf out = STRBUF_INIT;
	ഘടന commit_list *j;
	ഘടന pretty_print_context ctx = {0};

	printf(_("Squash commit -- not updating HEAD\n"));

	repo_init_revisions(the_repository, &rev, ശൂന്യ);
	diff_merges_suppress(&rev);
	rev.commit_format = CMIT_FMT_MEDIUM;

	commit->object.flags |= UNINTERESTING;
	add_pending_object(&rev, &commit->object, ശൂന്യ);

	വേണ്ടി (j = remoteheads; j; j = j->next)
		add_pending_object(&rev, &j->item->object, ശൂന്യ);

	setup_revisions(0, ശൂന്യ, &rev, ശൂന്യ);
	എങ്കിൽ (prepare_revision_walk(&rev))
		die(_("revision walk setup failed"));

	ctx.abbrev = rev.abbrev;
	ctx.date_mode = rev.date_mode;
	ctx.fmt = rev.commit_format;

	strbuf_addstr(&out, "Squashed commit of the following:\n");
	ആകുമ്പോൾ ((commit = get_revision(&rev)) != ശൂന്യ) {
		strbuf_addch(&out, '\n');
		strbuf_addf(&out, "commit %s\n",
			oid_to_hex(&commit->object.oid));
		pretty_print_commit(&ctx, commit, &out);
	}
	write_file_buf(git_path_squash_സന്ദേശം(the_repository), out.buf, out.len);
	strbuf_release(&out);
	release_revisions(&rev);
}

സ്ഥിതം ശൂന്യം finish(ഘടന commit *head_commit,
		   ഘടന commit_list *remoteheads,
		   സ്ഥിരം ഘടന object_id *new_head, സ്ഥിരം അക്ഷരം *സന്ദേശം)
{
	ഘടന strbuf reflog_message = STRBUF_INIT;
	സ്ഥിരം ഘടന object_id *head = &head_commit->object.oid;

	എങ്കിൽ (!സന്ദേശം)
		strbuf_addstr(&reflog_message, getenv("GIT_REFLOG_ACTION"));
	അല്ലെങ്കിൽ {
		എങ്കിൽ (verbosity >= 0)
			printf("%s\n", സന്ദേശം);
		strbuf_addf(&reflog_message, "%s: %s",
			getenv("GIT_REFLOG_ACTION"), സന്ദേശം);
	}
	എങ്കിൽ (squash) {
		squash_message(head_commit, remoteheads);
	} അല്ലെങ്കിൽ {
		എങ്കിൽ (verbosity >= 0 && !merge_സന്ദേശം.len)
			printf(_("No merge message -- not updating HEAD\n"));
		അല്ലെങ്കിൽ {
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
	എങ്കിൽ (new_head && show_diffstat) {
		ഘടന diff_options opts;
		repo_diff_setup(the_repository, &opts);
		init_diffstat_widths(&opts);

		തിരഞ്ഞെടുപ്പ് (show_diffstat) {
		അവസ്ഥ MERGE_SHOW_DIFFSTAT: /* 1 */
			opts.output_format |=
				DIFF_FORMAT_SUMMARY | DIFF_FORMAT_DIFFSTAT;
			നിർത്തുക;
		അവസ്ഥ MERGE_SHOW_COMPACTSUMMARY: /* 2 */
			opts.output_format |= DIFF_FORMAT_DIFFSTAT;
			opts.flags.stat_with_summary = 1;
			നിർത്തുക;
		സ്ഥിരസ്ഥിതി:
			നിർത്തുക;
		}
		opts.detect_rename = DIFF_DETECT_RENAME;
		diff_setup_done(&opts);
		diff_വൃക്ഷം_oid(head, new_head, "", &opts);
		diffcore_std(&opts);
		diff_flush(&opts);
	}

	/* Run a post-merge hook */
	run_hooks_l(the_repository, "post-merge", squash ? "1" : "0", ശൂന്യ);

	എങ്കിൽ (new_head)
		apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
				    ശൂന്യ, ശൂന്യ, ശൂന്യ, ശൂന്യ);
	strbuf_release(&reflog_message);
}

/* Get the name for the merge commit's message. */
സ്ഥിതം ശൂന്യം merge_name(സ്ഥിരം അക്ഷരം *remote, ഘടന strbuf *സന്ദേശം)
{
	ഘടന commit *remote_head;
	ഘടന object_id branch_head;
	ഘടന strbuf bname = STRBUF_INIT;
	ഘടന merge_remote_desc *desc;
	സ്ഥിരം അക്ഷരം *ptr;
	അക്ഷരം *found_ref = ശൂന്യ;
	പൂർണ്ണസംഖ്യ len, early;

	copy_branchname(the_repository, &bname, remote, 0);
	remote = bname.buf;

	oidclr(&branch_head, the_repository->hash_algo);
	remote_head = get_merge_parent(remote);
	എങ്കിൽ (!remote_head)
		die(_("'%s' does not point to a commit"), remote);

	എങ്കിൽ (repo_dwim_ref(the_repository, remote, strlen(remote), &branch_head,
			  &found_ref, 0) > 0) {
		എങ്കിൽ (starts_with(found_ref, "refs/heads/")) {
			strbuf_addf(സന്ദേശം, "%s\t\tbranch '%s' of .\n",
				    oid_to_hex(&branch_head), remote);
			പോകുക cleanup;
		}
		എങ്കിൽ (starts_with(found_ref, "refs/tags/")) {
			strbuf_addf(സന്ദേശം, "%s\t\ttag '%s' of .\n",
				    oid_to_hex(&branch_head), remote);
			പോകുക cleanup;
		}
		എങ്കിൽ (starts_with(found_ref, "refs/remotes/")) {
			strbuf_addf(സന്ദേശം, "%s\t\tremote-tracking branch '%s' of .\n",
				    oid_to_hex(&branch_head), remote);
			പോകുക cleanup;
		}
	}

	/* See if remote matches <name>^^^.. or <name>~<number> */
	വേണ്ടി (len = 0, ptr = remote + strlen(remote);
	     remote < ptr && ptr[-1] == '^';
	     ptr--)
		len++;
	എങ്കിൽ (len)
		early = 1;
	അല്ലെങ്കിൽ {
		early = 0;
		ptr = strrchr(remote, '~');
		എങ്കിൽ (ptr) {
			പൂർണ്ണസംഖ്യ seen_nonzero = 0;

			len++; /* count ~ */
			ആകുമ്പോൾ (*++ptr && isdigit(*ptr)) {
				seen_nonzero |= (*ptr != '0');
				len++;
			}
			എങ്കിൽ (*ptr)
				len = 0; /* not ...~<number> */
			അല്ലെങ്കിൽ എങ്കിൽ (seen_nonzero)
				early = 1;
			അല്ലെങ്കിൽ എങ്കിൽ (len == 1)
				early = 1; /* "name~" is "name~1"! */
		}
	}
	എങ്കിൽ (len) {
		ഘടന strbuf truname = STRBUF_INIT;
		strbuf_addf(&truname, "refs/heads/%s", remote);
		strbuf_setlen(&truname, truname.len - len);
		എങ്കിൽ (refs_ref_exists(get_main_ref_store(the_repository), truname.buf)) {
			strbuf_addf(സന്ദേശം,
				    "%s\t\tbranch '%s'%s of .\n",
				    oid_to_hex(&remote_head->object.oid),
				    truname.buf + 11,
				    (early ? " (early part)" : ""));
			strbuf_release(&truname);
			പോകുക cleanup;
		}
		strbuf_release(&truname);
	}

	desc = merge_remote_util(remote_head);
	എങ്കിൽ (desc && desc->obj && desc->obj->type == OBJ_TAG) {
		strbuf_addf(സന്ദേശം, "%s\t\t%s '%s'\n",
			    oid_to_hex(&desc->obj->oid),
			    type_name(desc->obj->type),
			    remote);
		പോകുക cleanup;
	}

	strbuf_addf(സന്ദേശം, "%s\t\tcommit '%s'\n",
		oid_to_hex(&remote_head->object.oid), remote);
cleanup:
	സ്വതന്ത്രം(found_ref);
	strbuf_release(&bname);
}

സ്ഥിതം ശൂന്യം parse_branch_merge_options(അക്ഷരം *bmo)
{
	സ്ഥിരം അക്ഷരം **argv;
	പൂർണ്ണസംഖ്യ argc;

	എങ്കിൽ (!bmo)
		തിരികെ;
	argc = split_cmdline(bmo, &argv);
	എങ്കിൽ (argc < 0)
		die(_("Bad branch.%s.mergeoptions string: %s"), branch,
		    _(split_cmdline_strerror(argc)));
	REALLOC_ARRAY(argv, argc + 2);
	MOVE_ARRAY(argv + 1, argv, argc + 1);
	argc++;
	argv[0] = "branch.*.mergeoptions";
	parse_options(argc, argv, ശൂന്യ, builtin_merge_options,
		      builtin_merge_usage, 0);
	സ്വതന്ത്രം(argv);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ git_merge_config(സ്ഥിരം അക്ഷരം *k, സ്ഥിരം അക്ഷരം *v,
			    സ്ഥിരം ഘടന config_context *ctx, ശൂന്യം *cb)
{
	പൂർണ്ണസംഖ്യ status;
	സ്ഥിരം അക്ഷരം *str;

	എങ്കിൽ (branch &&
	    skip_prefix(k, "branch.", &str) &&
	    skip_prefix(str, branch, &str) &&
	    !strcmp(str, ".mergeoptions")) {
		സ്വതന്ത്രം(branch_mergeoptions);
		branch_mergeoptions = xstrdup(v);
		തിരികെ 0;
	}

	എങ്കിൽ (!strcmp(k, "merge.diffstat") || !strcmp(k, "merge.stat")) {
		പൂർണ്ണസംഖ്യ val = git_parse_maybe_bool_text(v);
		തിരഞ്ഞെടുപ്പ് (val) {
		അവസ്ഥ 0:
			show_diffstat = 0;
			നിർത്തുക;
		അവസ്ഥ 1:
			show_diffstat = MERGE_SHOW_DIFFSTAT;
			നിർത്തുക;
		സ്ഥിരസ്ഥിതി:
			എങ്കിൽ (!strcmp(v, "compact"))
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
			അല്ലെങ്കിൽ
				/*
				 * A setting from a future?  It is not an
				 * error grave enough to fail the command.
				 * proceed using the default one.
				 */
				show_diffstat = MERGE_SHOW_DIFFSTAT;
			നിർത്തുക;
		}
	} അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(k, "merge.verifysignatures")) {
		verify_signatures = git_config_bool(k, v);
	} അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(k, "pull.twohead")) {
		FREE_AND_NULL(pull_twohead);
		തിരികെ git_config_string(&pull_twohead, k, v);
	} അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(k, "pull.octopus")) {
		FREE_AND_NULL(pull_octopus);
		തിരികെ git_config_string(&pull_octopus, k, v);
	} അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(k, "commit.cleanup")) {
		തിരികെ git_config_string(&cleanup_arg, k, v);
	} അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(k, "merge.ff")) {
		പൂർണ്ണസംഖ്യ boolval = git_parse_maybe_bool(v);
		എങ്കിൽ (0 <= boolval) {
			fast_forward = boolval ? FF_ALLOW : FF_NO;
		} അല്ലെങ്കിൽ എങ്കിൽ (v && !strcmp(v, "only")) {
			fast_forward = FF_ONLY;
		} /* do not barf on values from future versions of git */
		തിരികെ 0;
	} അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(k, "merge.defaulttoupstream")) {
		default_to_upstream = git_config_bool(k, v);
		തിരികെ 0;
	} അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(k, "commit.gpgsign")) {
		sign_commit = git_config_bool(k, v) ? "" : ശൂന്യ;
		തിരികെ 0;
	} അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(k, "gpg.mintrustlevel")) {
		check_trust_level = 0;
	} അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(k, "merge.autostash")) {
		autostash = git_config_bool(k, v);
		തിരികെ 0;
	}

	status = fmt_merge_സന്ദേശം_config(k, v, ctx, cb);
	എങ്കിൽ (status)
		തിരികെ status;
	തിരികെ git_diff_ui_config(k, v, ctx, cb);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ read_വൃക്ഷം_trivial(ഘടന object_id *common, ഘടന object_id *head,
			     ഘടന object_id *one)
{
	പൂർണ്ണസംഖ്യ i, nr_trees = 0;
	ഘടന വൃക്ഷം *trees[MAX_UNPACK_TREES];
	ഘടന വൃക്ഷം_desc t[MAX_UNPACK_TREES];
	ഘടന unpack_trees_options opts;

	memset(&opts, 0, വലുപ്പം(opts));
	opts.head_idx = 2;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;
	opts.update = 1;
	opts.verbose_update = 1;
	opts.trivial_merges_only = 1;
	opts.merge = 1;
	opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
	trees[nr_trees] = repo_parse_വൃക്ഷം_indirect(the_repository, common);
	എങ്കിൽ (!trees[nr_trees++])
		തിരികെ -1;
	trees[nr_trees] = repo_parse_വൃക്ഷം_indirect(the_repository, head);
	എങ്കിൽ (!trees[nr_trees++])
		തിരികെ -1;
	trees[nr_trees] = repo_parse_വൃക്ഷം_indirect(the_repository, one);
	എങ്കിൽ (!trees[nr_trees++])
		തിരികെ -1;
	opts.fn = threeway_merge;
	cache_വൃക്ഷം_free(&the_repository->index->cache_വൃക്ഷം);
	വേണ്ടി (i = 0; i < nr_trees; i++) {
		repo_parse_വൃക്ഷം(the_repository, trees[i]);
		init_വൃക്ഷം_desc(t+i, &trees[i]->object.oid,
			       trees[i]->buffer, trees[i]->size);
	}
	എങ്കിൽ (unpack_trees(nr_trees, t, &opts))
		തിരികെ -1;
	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം write_വൃക്ഷം_trivial(ഘടന object_id *oid)
{
	എങ്കിൽ (write_index_as_വൃക്ഷം(oid, the_repository->index,
				repo_get_index_file(the_repository),
				0, ശൂന്യ))
		die(_("git write-tree failed to write a tree"));
}

സ്ഥിതം പൂർണ്ണസംഖ്യ try_merge_strategy(സ്ഥിരം അക്ഷരം *strategy, ഘടന commit_list *common,
			      ഘടന commit_list *remoteheads,
			      ഘടന commit *head)
{
	സ്ഥിരം അക്ഷരം *head_arg = "HEAD";

	എങ്കിൽ (repo_refresh_and_write_index(the_repository, REFRESH_QUIET,
					 SKIP_IF_UNCHANGED, 0, ശൂന്യ, ശൂന്യ,
					 ശൂന്യ) < 0)
		die(_("Unable to write index."));

	എങ്കിൽ (!strcmp(strategy, "recursive") || !strcmp(strategy, "subtree") ||
	    !strcmp(strategy, "ort")) {
		ഘടന lock_file lock = LOCK_INIT;
		പൂർണ്ണസംഖ്യ clean, x;
		ഘടന commit *result;
		ഘടന commit_list *reversed = ശൂന്യ;
		ഘടന merge_options o;
		ഘടന commit_list *j;

		എങ്കിൽ (remoteheads->next) {
			error(_("Not handling anything other than two heads merge."));
			തിരികെ 2;
		}

		init_ui_merge_options(&o, the_repository);
		എങ്കിൽ (!strcmp(strategy, "subtree"))
			o.subtree_shift = "";

		o.show_rename_progress =
			show_progress == -1 ? isatty(2) : show_progress;

		വേണ്ടി (x = 0; x < xopts.nr; x++)
			എങ്കിൽ (parse_merge_opt(&o, xopts.v[x]))
				die(_("unknown strategy option: -X%s"), xopts.v[x]);

		o.branch1 = head_arg;
		o.branch2 = merge_remote_util(remoteheads->item)->name;

		വേണ്ടി (j = common; j; j = j->next)
			commit_list_തിരുകുക(j->item, &reversed);

		repo_hold_locked_index(the_repository, &lock,
				       LOCK_DIE_ON_ERROR);
		clean = merge_ort_recursive(&o, head, remoteheads->item,
					    reversed, &result);
		commit_list_free(reversed);
		strbuf_release(&o.obuf);

		എങ്കിൽ (clean < 0) {
			rollback_lock_file(&lock);
			തിരികെ 2;
		}
		എങ്കിൽ (write_locked_index(the_repository->index, &lock,
				       COMMIT_LOCK | SKIP_IF_UNCHANGED))
			die(_("unable to write %s"), repo_get_index_file(the_repository));
		തിരികെ clean ? 0 : 1;
	} അല്ലെങ്കിൽ {
		തിരികെ try_merge_command(the_repository,
					 strategy, xopts.nr, xopts.v,
					 common, head_arg, remoteheads);
	}
}

സ്ഥിതം ശൂന്യം എണ്ണം_diff_files(ഘടന diff_queue_struct *q,
			     ഘടന diff_options *opt UNUSED, ശൂന്യം *data)
{
	പൂർണ്ണസംഖ്യ *എണ്ണം = data;

	(*എണ്ണം) += q->nr;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ എണ്ണം_unmerged_entries(ശൂന്യം)
{
	പൂർണ്ണസംഖ്യ i, ret = 0;

	വേണ്ടി (i = 0; i < the_repository->index->cache_nr; i++)
		എങ്കിൽ (ce_stage(the_repository->index->cache[i]))
			ret++;

	തിരികെ ret;
}

സ്ഥിതം ശൂന്യം add_strategies(സ്ഥിരം അക്ഷരം *string, unsigned attr)
{
	പൂർണ്ണസംഖ്യ i;

	എങ്കിൽ (string) {
		ഘടന string_list list = STRING_LIST_INIT_DUP;
		ഘടന string_list_item *item;
		string_list_split(&list, string, " ", -1);
		for_each_string_list_item(item, &list)
			append_strategy(get_strategy(item->string));
		string_list_clear(&list, 0);
		തിരികെ;
	}
	വേണ്ടി (i = 0; i < ARRAY_SIZE(all_strategy); i++)
		എങ്കിൽ (all_strategy[i].attr & attr)
			append_strategy(&all_strategy[i]);

}

സ്ഥിതം ശൂന്യം read_merge_സന്ദേശം(ഘടന strbuf *സന്ദേശം)
{
	സ്ഥിരം അക്ഷരം *filename = git_path_merge_സന്ദേശം(the_repository);
	strbuf_reset(സന്ദേശം);
	എങ്കിൽ (strbuf_read_file(സന്ദേശം, filename, 0) < 0)
		die_errno(_("Could not read from '%s'"), filename);
}

സ്ഥിതം ശൂന്യം write_merge_state(ഘടന commit_list *);
സ്ഥിതം ശൂന്യം abort_commit(ഘടന commit_list *remoteheads, സ്ഥിരം അക്ഷരം *err_സന്ദേശം)
{
	എങ്കിൽ (err_സന്ദേശം)
		error("%s", err_സന്ദേശം);
	fprintf(stderr,
		_("Not committing merge; use 'git commit' to complete the merge.\n"));
	write_merge_state(remoteheads);
	exit(1);
}

സ്ഥിതം സ്ഥിരം അക്ഷരം merge_editor_comment[] =
N_("Please enter a commit message to explain why this merge is necessary,\n"
   "especially if it merges an updated upstream into a topic branch.\n"
   "\n");

സ്ഥിതം സ്ഥിരം അക്ഷരം scissors_editor_comment[] =
N_("An empty message aborts the commit.\n");

സ്ഥിതം സ്ഥിരം അക്ഷരം no_scissors_editor_comment[] =
N_("Lines starting with '%s' will be ignored, and an empty message aborts\n"
   "the commit.\n");

സ്ഥിതം ശൂന്യം write_merge_heads(ഘടന commit_list *);
സ്ഥിതം ശൂന്യം prepare_to_commit(ഘടന commit_list *remoteheads)
{
	ഘടന strbuf സന്ദേശം = STRBUF_INIT;
	സ്ഥിരം അക്ഷരം *index_file = repo_get_index_file(the_repository);

	എങ്കിൽ (!no_verify) {
		പൂർണ്ണസംഖ്യ invoked_hook;

		എങ്കിൽ (run_commit_hook(0 < option_edit, index_file, &invoked_hook,
				    "pre-merge-commit", ശൂന്യ))
			abort_commit(remoteheads, ശൂന്യ);
		/*
		 * Re-read the index as pre-merge-commit hook could have updated it,
		 * and write it out as a tree.  We must do this before we invoke
		 * the editor and after we invoke run_status above.
		 */
		എങ്കിൽ (invoked_hook)
			discard_index(the_repository->index);
	}
	read_index_from(the_repository->index, index_file,
			repo_get_git_dir(the_repository));
	strbuf_addbuf(&സന്ദേശം, &merge_സന്ദേശം);
	എങ്കിൽ (squash)
		BUG("the control must not reach here under --squash");
	എങ്കിൽ (0 < option_edit) {
		strbuf_addch(&സന്ദേശം, '\n');
		എങ്കിൽ (cleanup_mode == COMMIT_സന്ദേശം_CLEANUP_SCISSORS) {
			wt_status_append_cut_വരി(&സന്ദേശം);
			strbuf_commented_addf(&സന്ദേശം, comment_വരി_str, "\n");
		}
		strbuf_commented_addf(&സന്ദേശം, comment_വരി_str,
				      _(merge_editor_comment));
		എങ്കിൽ (cleanup_mode == COMMIT_സന്ദേശം_CLEANUP_SCISSORS)
			strbuf_commented_addf(&സന്ദേശം, comment_വരി_str,
					      _(scissors_editor_comment));
		അല്ലെങ്കിൽ
			strbuf_commented_addf(&സന്ദേശം, comment_വരി_str,
				_(no_scissors_editor_comment), comment_വരി_str);
	}
	എങ്കിൽ (signoff)
		append_signoff(&സന്ദേശം, ignored_log_message_bytes(സന്ദേശം.buf, സന്ദേശം.len), 0);
	write_merge_heads(remoteheads);
	write_file_buf(git_path_merge_സന്ദേശം(the_repository), സന്ദേശം.buf, സന്ദേശം.len);
	എങ്കിൽ (run_commit_hook(0 < option_edit, repo_get_index_file(the_repository),
			    ശൂന്യ, "prepare-commit-msg",
			    git_path_merge_സന്ദേശം(the_repository), "merge", ശൂന്യ))
		abort_commit(remoteheads, ശൂന്യ);
	എങ്കിൽ (0 < option_edit) {
		എങ്കിൽ (launch_editor(git_path_merge_സന്ദേശം(the_repository), ശൂന്യ, ശൂന്യ))
			abort_commit(remoteheads, ശൂന്യ);
	}

	എങ്കിൽ (!no_verify && run_commit_hook(0 < option_edit, repo_get_index_file(the_repository),
					  ശൂന്യ, "commit-msg",
					  git_path_merge_സന്ദേശം(the_repository), ശൂന്യ))
		abort_commit(remoteheads, ശൂന്യ);

	read_merge_സന്ദേശം(&സന്ദേശം);
	cleanup_message(&സന്ദേശം, cleanup_mode, 0);
	എങ്കിൽ (!സന്ദേശം.len)
		abort_commit(remoteheads, _("Empty commit message."));
	strbuf_release(&merge_സന്ദേശം);
	strbuf_addbuf(&merge_സന്ദേശം, &സന്ദേശം);
	strbuf_release(&സന്ദേശം);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ merge_trivial(ഘടന commit *head, ഘടന commit_list *remoteheads)
{
	ഘടന object_id result_വൃക്ഷം, result_commit;
	ഘടന commit_list *parents = ശൂന്യ, **pptr = &parents;

	എങ്കിൽ (repo_refresh_and_write_index(the_repository, REFRESH_QUIET,
					 SKIP_IF_UNCHANGED, 0, ശൂന്യ, ശൂന്യ,
					 ശൂന്യ) < 0)
		തിരികെ error(_("Unable to write index."));

	write_വൃക്ഷം_trivial(&result_വൃക്ഷം);
	printf(_("Wonderful.\n"));
	pptr = commit_list_append(head, pptr);
	pptr = commit_list_append(remoteheads->item, pptr);
	prepare_to_commit(remoteheads);
	എങ്കിൽ (commit_വൃക്ഷം(merge_സന്ദേശം.buf, merge_സന്ദേശം.len, &result_വൃക്ഷം, parents,
			&result_commit, ശൂന്യ, sign_commit))
		die(_("failed to write commit object"));
	finish(head, remoteheads, &result_commit, "In-index merge");

	remove_merge_branch_state(the_repository);
	commit_list_free(parents);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ finish_automerge(ഘടന commit *head,
			    പൂർണ്ണസംഖ്യ head_subsumed,
			    ഘടന commit_list *common,
			    ഘടന commit_list *remoteheads,
			    ഘടന object_id *result_വൃക്ഷം,
			    സ്ഥിരം അക്ഷരം *wt_strategy)
{
	ഘടന commit_list *parents = ശൂന്യ;
	ഘടന strbuf buf = STRBUF_INIT;
	ഘടന object_id result_commit;

	write_വൃക്ഷം_trivial(result_വൃക്ഷം);
	commit_list_free(common);
	parents = remoteheads;
	എങ്കിൽ (!head_subsumed || fast_forward == FF_NO)
		commit_list_തിരുകുക(head, &parents);
	prepare_to_commit(remoteheads);
	എങ്കിൽ (commit_വൃക്ഷം(merge_സന്ദേശം.buf, merge_സന്ദേശം.len, result_വൃക്ഷം, parents,
			&result_commit, ശൂന്യ, sign_commit))
		die(_("failed to write commit object"));
	strbuf_addf(&buf, "Merge made by the '%s' strategy.", wt_strategy);
	finish(head, remoteheads, &result_commit, buf.buf);

	strbuf_release(&buf);
	remove_merge_branch_state(the_repository);
	commit_list_free(parents);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ suggest_conflicts(ശൂന്യം)
{
	സ്ഥിരം അക്ഷരം *filename;
	FILE *fp;
	ഘടന strbuf msgbuf = STRBUF_INIT;

	filename = git_path_merge_സന്ദേശം(the_repository);
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
	തിരികെ 1;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ evaluate_result(ശൂന്യം)
{
	പൂർണ്ണസംഖ്യ cnt = 0;
	ഘടന rev_info rev;

	/* Check how many files differ. */
	repo_init_revisions(the_repository, &rev, "");
	setup_revisions(0, ശൂന്യ, &rev, ശൂന്യ);
	rev.diffopt.output_format |=
		DIFF_FORMAT_CALLBACK;
	rev.diffopt.format_callback = എണ്ണം_diff_files;
	rev.diffopt.format_callback_data = &cnt;
	run_diff_files(&rev, 0);

	/*
	 * Check how many unmerged entries are
	 * there.
	 */
	cnt += എണ്ണം_unmerged_entries();

	release_revisions(&rev);
	തിരികെ cnt;
}

/*
 * Pretend as if the user told us to merge with the remote-tracking
 * branch we have for the upstream of the current branch
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ setup_with_upstream(സ്ഥിരം അക്ഷരം ***argv)
{
	ഘടന branch *branch = branch_get(ശൂന്യ);
	പൂർണ്ണസംഖ്യ i;
	സ്ഥിരം അക്ഷരം **args;

	എങ്കിൽ (!branch)
		die(_("No current branch."));
	എങ്കിൽ (!branch->remote_name)
		die(_("No remote for the current branch."));
	എങ്കിൽ (!branch->merge_nr)
		die(_("No default upstream defined for the current branch."));

	args = xcalloc(st_add(branch->merge_nr, 1), വലുപ്പം(അക്ഷരം *));
	വേണ്ടി (i = 0; i < branch->merge_nr; i++) {
		എങ്കിൽ (!branch->merge[i]->dst)
			die(_("No remote-tracking branch for %s from %s"),
			    branch->merge[i]->src, branch->remote_name);
		args[i] = branch->merge[i]->dst;
	}
	args[i] = ശൂന്യ;
	*argv = args;
	തിരികെ i;
}

സ്ഥിതം ശൂന്യം write_merge_heads(ഘടന commit_list *remoteheads)
{
	ഘടന commit_list *j;
	ഘടന strbuf buf = STRBUF_INIT;

	വേണ്ടി (j = remoteheads; j; j = j->next) {
		ഘടന object_id *oid;
		ഘടന commit *c = j->item;
		ഘടന merge_remote_desc *desc;

		desc = merge_remote_util(c);
		എങ്കിൽ (desc && desc->obj) {
			oid = &desc->obj->oid;
		} അല്ലെങ്കിൽ {
			oid = &c->object.oid;
		}
		strbuf_addf(&buf, "%s\n", oid_to_hex(oid));
	}
	write_file_buf(git_path_merge_head(the_repository), buf.buf, buf.len);

	strbuf_reset(&buf);
	എങ്കിൽ (fast_forward == FF_NO)
		strbuf_addstr(&buf, "no-ff");
	write_file_buf(git_path_merge_mode(the_repository), buf.buf, buf.len);
	strbuf_release(&buf);
}

സ്ഥിതം ശൂന്യം write_merge_state(ഘടന commit_list *remoteheads)
{
	write_merge_heads(remoteheads);
	strbuf_addch(&merge_സന്ദേശം, '\n');
	write_file_buf(git_path_merge_സന്ദേശം(the_repository), merge_സന്ദേശം.buf,
		       merge_സന്ദേശം.len);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ default_edit_option(ശൂന്യം)
{
	സ്ഥിതം സ്ഥിരം അക്ഷരം name[] = "GIT_MERGE_AUTOEDIT";
	സ്ഥിരം അക്ഷരം *e = getenv(name);
	ഘടന stat st_stdin, st_stdout;

	എങ്കിൽ (have_message)
		/* an explicit -m msg without --[no-]edit */
		തിരികെ 0;

	എങ്കിൽ (e) {
		പൂർണ്ണസംഖ്യ v = git_parse_maybe_bool(e);
		എങ്കിൽ (v < 0)
			die(_("Bad value '%s' in environment '%s'"), e, name);
		തിരികെ v;
	}

	/* Use editor if stdin and stdout are the same and is a tty */
	തിരികെ (!fstat(0, &st_stdin) &&
		!fstat(1, &st_stdout) &&
		isatty(0) && isatty(1) &&
		st_stdin.st_dev == st_stdout.st_dev &&
		st_stdin.st_ino == st_stdout.st_ino &&
		st_stdin.st_mode == st_stdout.st_mode);
}

സ്ഥിതം ഘടന commit_list *reduce_parents(ഘടന commit *head_commit,
					  പൂർണ്ണസംഖ്യ *head_subsumed,
					  ഘടന commit_list *remoteheads)
{
	ഘടന commit_list *parents, **remotes;

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

	remoteheads = ശൂന്യ;
	remotes = &remoteheads;
	ആകുമ്പോൾ (parents) {
		ഘടന commit *commit = എടുക്കുക_commit(&parents);
		എങ്കിൽ (commit == head_commit)
			*head_subsumed = 0;
		അല്ലെങ്കിൽ
			remotes = &commit_list_തിരുകുക(commit, remotes)->next;
	}
	തിരികെ remoteheads;
}

സ്ഥിതം ശൂന്യം prepare_merge_message(ഘടന strbuf *merge_names, ഘടന strbuf *merge_സന്ദേശം)
{
	ഘടന fmt_merge_സന്ദേശം_opts opts;

	memset(&opts, 0, വലുപ്പം(opts));
	opts.add_title = !have_message;
	opts.shortlog_len = shortlog_len;
	opts.credit_people = (0 < option_edit);
	opts.into_name = into_name;

	fmt_merge_സന്ദേശം(merge_names, merge_സന്ദേശം, &opts);
	എങ്കിൽ (merge_സന്ദേശം->len)
		strbuf_setlen(merge_സന്ദേശം, merge_സന്ദേശം->len - 1);
}

സ്ഥിതം ശൂന്യം handle_fetch_head(ഘടന commit_list **remotes, ഘടന strbuf *merge_names)
{
	സ്ഥിരം അക്ഷരം *filename;
	പൂർണ്ണസംഖ്യ fd, pos, npos;
	ഘടന strbuf fetch_head_file = STRBUF_INIT;
	സ്ഥിരം unsigned hexsz = the_hash_algo->hexsz;

	എങ്കിൽ (!merge_names)
		merge_names = &fetch_head_file;

	filename = git_path_fetch_head(the_repository);
	fd = xopen(filename, O_RDONLY);

	എങ്കിൽ (strbuf_read(merge_names, fd, 0) < 0)
		die_errno(_("could not read '%s'"), filename);
	എങ്കിൽ (close(fd) < 0)
		die_errno(_("could not close '%s'"), filename);

	വേണ്ടി (pos = 0; pos < merge_names->len; pos = npos) {
		ഘടന object_id oid;
		അക്ഷരം *ptr;
		ഘടന commit *commit;

		ptr = strchr(merge_names->buf + pos, '\n');
		എങ്കിൽ (ptr)
			npos = ptr - merge_names->buf + 1;
		അല്ലെങ്കിൽ
			npos = merge_names->len;

		എങ്കിൽ (npos - pos < hexsz + 2 ||
		    get_oid_hex(merge_names->buf + pos, &oid))
			commit = ശൂന്യ; /* bad */
		അല്ലെങ്കിൽ എങ്കിൽ (memcmp(merge_names->buf + pos + hexsz, "\t\t", 2))
			തുടരുക; /* not-for-merge */
		അല്ലെങ്കിൽ {
			അക്ഷരം saved = merge_names->buf[pos + hexsz];
			merge_names->buf[pos + hexsz] = '\0';
			commit = get_merge_parent(merge_names->buf + pos);
			merge_names->buf[pos + hexsz] = saved;
		}
		എങ്കിൽ (!commit) {
			എങ്കിൽ (ptr)
				*ptr = '\0';
			die(_("not something we can merge in %s: %s"),
			    filename, merge_names->buf + pos);
		}
		remotes = &commit_list_തിരുകുക(commit, remotes)->next;
	}

	എങ്കിൽ (merge_names == &fetch_head_file)
		strbuf_release(&fetch_head_file);
}

സ്ഥിതം ഘടന commit_list *collect_parents(ഘടന commit *head_commit,
					   പൂർണ്ണസംഖ്യ *head_subsumed,
					   പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv,
					   ഘടന strbuf *merge_സന്ദേശം)
{
	പൂർണ്ണസംഖ്യ i;
	ഘടന commit_list *remoteheads = ശൂന്യ;
	ഘടന commit_list **remotes = &remoteheads;
	ഘടന strbuf merge_names = STRBUF_INIT, *autogen = ശൂന്യ;

	എങ്കിൽ (merge_സന്ദേശം && (!have_message || shortlog_len))
		autogen = &merge_names;

	എങ്കിൽ (head_commit)
		remotes = &commit_list_തിരുകുക(head_commit, remotes)->next;

	എങ്കിൽ (argc == 1 && !strcmp(argv[0], "FETCH_HEAD")) {
		handle_fetch_head(remotes, autogen);
		remoteheads = reduce_parents(head_commit, head_subsumed, remoteheads);
	} അല്ലെങ്കിൽ {
		വേണ്ടി (i = 0; i < argc; i++) {
			ഘടന commit *commit = get_merge_parent(argv[i]);
			എങ്കിൽ (!commit)
				help_unknown_ref(argv[i], "merge",
						 _("not something we can merge"));
			remotes = &commit_list_തിരുകുക(commit, remotes)->next;
		}
		remoteheads = reduce_parents(head_commit, head_subsumed, remoteheads);
		എങ്കിൽ (autogen) {
			ഘടന commit_list *p;
			വേണ്ടി (p = remoteheads; p; p = p->next)
				merge_name(merge_remote_util(p->item)->name, autogen);
		}
	}

	എങ്കിൽ (autogen) {
		prepare_merge_message(autogen, merge_സന്ദേശം);
		strbuf_release(autogen);
	}

	തിരികെ remoteheads;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ merging_a_throwaway_tag(ഘടന commit *commit)
{
	അക്ഷരം *tag_ref;
	ഘടന object_id oid;
	പൂർണ്ണസംഖ്യ is_throwaway_tag = 0;

	/* Are we merging a tag? */
	എങ്കിൽ (!merge_remote_util(commit) ||
	    !merge_remote_util(commit)->obj ||
	    merge_remote_util(commit)->obj->type != OBJ_TAG)
		തിരികെ is_throwaway_tag;

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
			  ((ഘടന tag *)merge_remote_util(commit)->obj)->tag);
	എങ്കിൽ (!refs_read_ref(get_main_ref_store(the_repository), tag_ref, &oid) &&
	    oideq(&oid, &merge_remote_util(commit)->obj->oid))
		is_throwaway_tag = 0;
	അല്ലെങ്കിൽ
		is_throwaway_tag = 1;
	സ്വതന്ത്രം(tag_ref);
	തിരികെ is_throwaway_tag;
}

പൂർണ്ണസംഖ്യ cmd_merge(പൂർണ്ണസംഖ്യ argc,
	      സ്ഥിരം അക്ഷരം **argv,
	      സ്ഥിരം അക്ഷരം *prefix,
	      ഘടന repository *repo UNUSED)
{
	ഘടന object_id result_വൃക്ഷം, stash, head_oid;
	ഘടന commit *head_commit;
	ഘടന strbuf buf = STRBUF_INIT;
	പൂർണ്ണസംഖ്യ i, ret = 0, head_subsumed;
	പൂർണ്ണസംഖ്യ best_cnt = -1, merge_was_ok = 0, automerge_was_ok = 0;
	ഘടന commit_list *common = ശൂന്യ;
	സ്ഥിരം അക്ഷരം *best_strategy = ശൂന്യ, *wt_strategy = ശൂന്യ;
	ഘടന commit_list *remoteheads = ശൂന്യ, *p;
	ശൂന്യം *branch_to_free, *argv_to_free = ശൂന്യ;
	പൂർണ്ണസംഖ്യ orig_argc = argc;
	പൂർണ്ണസംഖ്യ merge_log_config = -1;

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
						      ശൂന്യ);
	എങ്കിൽ (branch)
		skip_prefix(branch, "refs/heads/", &branch);

	init_diff_ui_defaults();
	repo_config(the_repository, git_merge_config, &merge_log_config);

	എങ്കിൽ (!branch || is_null_oid(&head_oid))
		head_commit = ശൂന്യ;
	അല്ലെങ്കിൽ
		head_commit = lookup_commit_or_die(&head_oid, "HEAD");

	എങ്കിൽ (branch_mergeoptions)
		parse_branch_merge_options(branch_mergeoptions);
	argc = parse_options(argc, argv, prefix, builtin_merge_options,
			builtin_merge_usage, 0);
	എങ്കിൽ (shortlog_len < 0)
		shortlog_len = (merge_log_config > 0) ? merge_log_config : 0;

	എങ്കിൽ (verbosity < 0 && show_progress == -1)
		show_progress = 0;

	എങ്കിൽ (abort_current_merge) {
		പൂർണ്ണസംഖ്യ nargc = 2;
		സ്ഥിരം അക്ഷരം *nargv[] = {"reset", "--merge", ശൂന്യ};
		അക്ഷരം stash_oid_hex[GIT_MAX_HEXSZ + 1];
		ഘടന object_id stash_oid = {0};

		എങ്കിൽ (orig_argc != 2)
			usage_സന്ദേശം_opt(_("--abort expects no arguments"),
			      builtin_merge_usage, builtin_merge_options);

		എങ്കിൽ (!file_exists(git_path_merge_head(the_repository)))
			die(_("There is no merge to abort (MERGE_HEAD missing)."));

		എങ്കിൽ (!refs_read_ref(get_main_ref_store(the_repository), "MERGE_AUTOSTASH", &stash_oid))
			refs_delete_ref(get_main_ref_store(the_repository),
					"", "MERGE_AUTOSTASH", &stash_oid,
					REF_NO_DEREF);

		/* Invoke 'git reset --merge' */
		ret = cmd_reset(nargc, nargv, prefix, the_repository);

		എങ്കിൽ (!is_null_oid(&stash_oid)) {
			oid_to_hex_r(stash_oid_hex, &stash_oid);
			apply_autostash_oid(stash_oid_hex);
		}

		പോകുക done;
	}

	എങ്കിൽ (quit_current_merge) {
		എങ്കിൽ (orig_argc != 2)
			usage_സന്ദേശം_opt(_("--quit expects no arguments"),
				      builtin_merge_usage,
				      builtin_merge_options);

		remove_merge_branch_state(the_repository);
		പോകുക done;
	}

	എങ്കിൽ (continue_current_merge) {
		പൂർണ്ണസംഖ്യ nargc = 1;
		സ്ഥിരം അക്ഷരം *nargv[] = {"commit", ശൂന്യ};

		എങ്കിൽ (orig_argc != 2)
			usage_സന്ദേശം_opt(_("--continue expects no arguments"),
			      builtin_merge_usage, builtin_merge_options);

		എങ്കിൽ (!file_exists(git_path_merge_head(the_repository)))
			die(_("There is no merge in progress (MERGE_HEAD missing)."));

		/* Invoke 'git commit' */
		ret = cmd_commit(nargc, nargv, prefix, the_repository);
		പോകുക done;
	}

	എങ്കിൽ (repo_read_index_unmerged(the_repository))
		die_resolve_conflict("merge");

	എങ്കിൽ (file_exists(git_path_merge_head(the_repository))) {
		/*
		 * There is no unmerged entry, don't advise 'git
		 * add/rm <file>', just 'git commit'.
		 */
		എങ്കിൽ (advice_enabled(ADVICE_RESOLVE_CONFLICT))
			die(_("You have not concluded your merge (MERGE_HEAD exists).\n"
				  "Please, commit your changes before you merge."));
		അല്ലെങ്കിൽ
			die(_("You have not concluded your merge (MERGE_HEAD exists)."));
	}
	എങ്കിൽ (refs_ref_exists(get_main_ref_store(the_repository), "CHERRY_PICK_HEAD")) {
		എങ്കിൽ (advice_enabled(ADVICE_RESOLVE_CONFLICT))
			die(_("You have not concluded your cherry-pick (CHERRY_PICK_HEAD exists).\n"
			    "Please, commit your changes before you merge."));
		അല്ലെങ്കിൽ
			die(_("You have not concluded your cherry-pick (CHERRY_PICK_HEAD exists)."));
	}
	resolve_undo_clear_index(the_repository->index);

	എങ്കിൽ (option_edit < 0)
		option_edit = default_edit_option();

	cleanup_mode = get_cleanup_mode(cleanup_arg, 0 < option_edit);

	എങ്കിൽ (verbosity < 0)
		show_diffstat = 0;

	എങ്കിൽ (squash) {
		എങ്കിൽ (fast_forward == FF_NO)
			die(_("options '%s' and '%s' cannot be used together"), "--squash", "--no-ff.");
		എങ്കിൽ (option_commit > 0)
			die(_("options '%s' and '%s' cannot be used together"), "--squash", "--commit.");
		/*
		 * squash can now silently disable option_commit - this is not
		 * a problem as it is only overriding the default, not a user
		 * supplied option.
		 */
		option_commit = 0;
	}

	എങ്കിൽ (option_commit < 0)
		option_commit = 1;

	എങ്കിൽ (!argc) {
		എങ്കിൽ (default_to_upstream) {
			argc = setup_with_upstream(&argv);
			argv_to_free = argv;
		}
		അല്ലെങ്കിൽ
			die(_("No commit specified and merge.defaultToUpstream not set."));
	} അല്ലെങ്കിൽ എങ്കിൽ (argc == 1 && !strcmp(argv[0], "-")) {
		argv[0] = "@{-1}";
	}

	എങ്കിൽ (!argc)
		usage_with_options(builtin_merge_usage,
			builtin_merge_options);

	എങ്കിൽ (!head_commit) {
		/*
		 * If the merged head is a valid one there is no reason
		 * to forbid "git merge" into a branch yet to be born.
		 * We do the same for "git pull".
		 */
		ഘടന object_id *remote_head_oid;
		എങ്കിൽ (squash)
			die(_("Squash commit into empty head not supported yet"));
		എങ്കിൽ (fast_forward == FF_NO)
			die(_("Non-fast-forward commit does not make sense into "
			    "an empty head"));
		remoteheads = collect_parents(head_commit, &head_subsumed,
					      argc, argv, ശൂന്യ);
		എങ്കിൽ (!remoteheads)
			die(_("%s - not something we can merge"), argv[0]);
		എങ്കിൽ (remoteheads->next)
			die(_("Can merge only exactly one commit into empty head"));

		എങ്കിൽ (verify_signatures)
			verify_merge_signature(remoteheads->item, verbosity,
					       check_trust_level);

		remote_head_oid = &remoteheads->item->object.oid;
		read_empty(remote_head_oid);
		refs_update_ref(get_main_ref_store(the_repository),
				"initial pull", "HEAD", remote_head_oid, ശൂന്യ,
				0,
				UPDATE_REFS_DIE_ON_ERR);
		പോകുക done;
	}

	/*
	 * All the rest are the commits being merged; prepare
	 * the standard merge summary message to be appended
	 * to the given message.
	 */
	remoteheads = collect_parents(head_commit, &head_subsumed,
				      argc, argv, &merge_സന്ദേശം);

	എങ്കിൽ (!head_commit || !argc)
		usage_with_options(builtin_merge_usage,
			builtin_merge_options);

	എങ്കിൽ (verify_signatures) {
		വേണ്ടി (p = remoteheads; p; p = p->next) {
			verify_merge_signature(p->item, verbosity,
					       check_trust_level);
		}
	}

	strbuf_addstr(&buf, "merge");
	വേണ്ടി (p = remoteheads; p; p = p->next)
		strbuf_addf(&buf, " %s", merge_remote_util(p->item)->name);
	setenv("GIT_REFLOG_ACTION", buf.buf, 0);
	strbuf_reset(&buf);

	വേണ്ടി (p = remoteheads; p; p = p->next) {
		ഘടന commit *commit = p->item;
		strbuf_addf(&buf, "GITHEAD_%s",
			    oid_to_hex(&commit->object.oid));
		setenv(buf.buf, merge_remote_util(commit)->name, 1);
		strbuf_reset(&buf);
		എങ്കിൽ (fast_forward != FF_ONLY && merging_a_throwaway_tag(commit))
			fast_forward = FF_NO;
	}

	എങ്കിൽ (!use_strategies) {
		എങ്കിൽ (!remoteheads)
			; /* already up-to-date */
		അല്ലെങ്കിൽ എങ്കിൽ (!remoteheads->next)
			add_strategies(pull_twohead, DEFAULT_TWOHEAD);
		അല്ലെങ്കിൽ
			add_strategies(pull_octopus, DEFAULT_OCTOPUS);
	}

	വേണ്ടി (i = 0; i < use_strategies_nr; i++) {
		എങ്കിൽ (use_strategies[i]->attr & NO_FAST_FORWARD)
			fast_forward = FF_NO;
		എങ്കിൽ (use_strategies[i]->attr & NO_TRIVIAL)
			allow_trivial = 0;
	}

	എങ്കിൽ (!remoteheads)
		; /* already up-to-date */
	അല്ലെങ്കിൽ എങ്കിൽ (!remoteheads->next) {
		എങ്കിൽ (repo_get_merge_bases(the_repository, head_commit,
					 remoteheads->item, &common) < 0) {
			ret = 2;
			പോകുക done;
		}
	} അല്ലെങ്കിൽ {
		ഘടന commit_list *list = remoteheads;
		commit_list_തിരുകുക(head_commit, &list);
		എങ്കിൽ (get_octopus_merge_bases(list, &common) < 0) {
			സ്വതന്ത്രം(list);
			ret = 2;
			പോകുക done;
		}
		സ്വതന്ത്രം(list);
	}

	refs_update_ref(get_main_ref_store(the_repository),
			"updating ORIG_HEAD", "ORIG_HEAD",
			&head_commit->object.oid, ശൂന്യ, 0,
			UPDATE_REFS_DIE_ON_ERR);

	എങ്കിൽ (remoteheads && !common) {
		/* No common ancestors found. */
		എങ്കിൽ (!allow_unrelated_histories)
			die(_("refusing to merge unrelated histories"));
		/* otherwise, we need a real merge. */
	} അല്ലെങ്കിൽ എങ്കിൽ (!remoteheads ||
		 (!remoteheads->next && !common->next &&
		  common->item == remoteheads->item)) {
		/*
		 * If head can reach all the merge then we are up to date.
		 * but first the most common case of merging one remote.
		 */
		finish_up_to_date();
		പോകുക done;
	} അല്ലെങ്കിൽ എങ്കിൽ (fast_forward != FF_NO && !remoteheads->next &&
			!common->next &&
			oideq(&common->item->object.oid, &head_commit->object.oid)) {
		/* Again the most common case of merging one remote. */
		സ്ഥിരം അക്ഷരം *സന്ദേശം = have_message ?
			"Fast-forward (no commit created; -m option ignored)" :
			"Fast-forward";
		ഘടന commit *commit;

		എങ്കിൽ (verbosity >= 0) {
			printf(_("Updating %s..%s\n"),
			       repo_find_unique_abbrev(the_repository, &head_commit->object.oid,
						       DEFAULT_ABBREV),
			       repo_find_unique_abbrev(the_repository, &remoteheads->item->object.oid,
						       DEFAULT_ABBREV));
		}
		commit = remoteheads->item;
		എങ്കിൽ (!commit) {
			ret = 1;
			പോകുക done;
		}

		എങ്കിൽ (autostash)
			create_autostash_ref(the_repository, "MERGE_AUTOSTASH",
					     ശൂന്യ, false);
		എങ്കിൽ (checkout_fast_forward(the_repository,
					  &head_commit->object.oid,
					  &commit->object.oid,
					  overwrite_ignore)) {
			apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
					    ശൂന്യ, ശൂന്യ, ശൂന്യ, ശൂന്യ);
			ret = 1;
			പോകുക done;
		}

		finish(head_commit, remoteheads, &commit->object.oid, സന്ദേശം);
		remove_merge_branch_state(the_repository);
		പോകുക done;
	} അല്ലെങ്കിൽ എങ്കിൽ (!remoteheads->next && common->next)
		;
		/*
		 * We are not doing octopus and not fast-forward.  Need
		 * a real merge.
		 */
	അല്ലെങ്കിൽ എങ്കിൽ (!remoteheads->next && !common->next && option_commit) {
		/*
		 * We are not doing octopus, not fast-forward, and have
		 * only one common.
		 */
		refresh_index(the_repository->index, REFRESH_QUIET, ശൂന്യ, ശൂന്യ, ശൂന്യ);
		എങ്കിൽ (allow_trivial && fast_forward != FF_ONLY) {
			/*
			 * Must first ensure that index matches HEAD before
			 * attempting a trivial merge.
			 */
			ഘടന വൃക്ഷം *head_വൃക്ഷം = repo_get_commit_വൃക്ഷം(the_repository,
								      head_commit);
			ഘടന strbuf sb = STRBUF_INIT;

			എങ്കിൽ (repo_index_has_changes(the_repository, head_വൃക്ഷം,
						   &sb)) {
				error(_("Your local changes to the following files would be overwritten by merge:\n  %s"),
				      sb.buf);
				strbuf_release(&sb);
				ret = 2;
				പോകുക done;
			}

			/* See if it is really trivial. */
			git_committer_info(IDENT_STRICT);
			printf(_("Trying really trivial in-index merge...\n"));
			എങ്കിൽ (!read_വൃക്ഷം_trivial(&common->item->object.oid,
					       &head_commit->object.oid,
					       &remoteheads->item->object.oid)) {
				ret = merge_trivial(head_commit, remoteheads);
				പോകുക done;
			}
			printf(_("Nope.\n"));
		}
	} അല്ലെങ്കിൽ {
		/*
		 * An octopus.  If we can reach all the remote we are up
		 * to date.
		 */
		പൂർണ്ണസംഖ്യ up_to_date = 1;
		ഘടന commit_list *j;

		വേണ്ടി (j = remoteheads; j; j = j->next) {
			പൂർണ്ണസംഖ്യ ret = repo_in_merge_bases(the_repository,
						      j->item, head_commit);
			എങ്കിൽ (ret < 0)
				exit(128);
			എങ്കിൽ (!ret) {
				up_to_date = 0;
				നിർത്തുക;
			}
		}
		എങ്കിൽ (up_to_date) {
			finish_up_to_date();
			പോകുക done;
		}
	}

	എങ്കിൽ (fast_forward == FF_ONLY)
		die_ff_impossible();

	എങ്കിൽ (autostash)
		create_autostash_ref(the_repository, "MERGE_AUTOSTASH",
				     ശൂന്യ, false);

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
	എങ്കിൽ (save_state(&stash))
		oidclr(&stash, the_repository->hash_algo);

	വേണ്ടി (i = 0; i < use_strategies_nr; i++) {
		പൂർണ്ണസംഖ്യ ret, cnt;
		എങ്കിൽ (i) {
			printf(_("Rewinding the tree to pristine...\n"));
			restore_state(&head_commit->object.oid, &stash);
		}
		എങ്കിൽ (use_strategies_nr != 1)
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
		എങ്കിൽ (ret < 2) {
			എങ്കിൽ (!ret) {
				/*
				 * This strategy worked; no point in trying
				 * another.
				 */
				merge_was_ok = 1;
				best_strategy = wt_strategy;
				നിർത്തുക;
			}
			cnt = (use_strategies_nr > 1) ? evaluate_result() : 0;
			എങ്കിൽ (best_cnt <= 0 || cnt <= best_cnt) {
				best_strategy = wt_strategy;
				best_cnt = cnt;
			}
		}
	}

	/*
	 * If we have a resulting tree, that means the strategy module
	 * auto resolved the merge cleanly.
	 */
	എങ്കിൽ (merge_was_ok && option_commit) {
		automerge_was_ok = 1;
		ret = finish_automerge(head_commit, head_subsumed,
				       common, remoteheads,
				       &result_വൃക്ഷം, wt_strategy);
		പോകുക done;
	}

	/*
	 * Pick the result from the best strategy and have the user fix
	 * it up.
	 */
	എങ്കിൽ (!best_strategy) {
		restore_state(&head_commit->object.oid, &stash);
		എങ്കിൽ (use_strategies_nr > 1)
			fprintf(stderr,
				_("No merge strategy handled the merge.\n"));
		അല്ലെങ്കിൽ
			fprintf(stderr, _("Merge with strategy %s failed.\n"),
				use_strategies[0]->name);
		apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
				    ശൂന്യ, ശൂന്യ, ശൂന്യ, ശൂന്യ);
		ret = 2;
		പോകുക done;
	} അല്ലെങ്കിൽ എങ്കിൽ (best_strategy == wt_strategy)
		; /* We already have its result in the working tree. */
	അല്ലെങ്കിൽ {
		printf(_("Rewinding the tree to pristine...\n"));
		restore_state(&head_commit->object.oid, &stash);
		printf(_("Using the %s strategy to prepare resolving by hand.\n"),
			best_strategy);
		try_merge_strategy(best_strategy, common, remoteheads,
				   head_commit);
	}

	എങ്കിൽ (squash) {
		finish(head_commit, remoteheads, ശൂന്യ, ശൂന്യ);

		git_test_write_commit_graph_or_die(the_repository);
	} അല്ലെങ്കിൽ
		write_merge_state(remoteheads);

	എങ്കിൽ (merge_was_ok)
		fprintf(stderr, _("Automatic merge went well; "
			"stopped before committing as requested\n"));
	അല്ലെങ്കിൽ
		ret = suggest_conflicts();
	എങ്കിൽ (autostash)
		printf(_("When finished, apply stashed changes with `git stash pop`\n"));

done:
	എങ്കിൽ (!automerge_was_ok) {
		commit_list_free(common);
		commit_list_free(remoteheads);
	}
	strbuf_release(&buf);
	സ്വതന്ത്രം(branch_to_free);
	സ്വതന്ത്രം(argv_to_free);
	സ്വതന്ത്രം(pull_twohead);
	സ്വതന്ത്രം(pull_octopus);
	discard_index(the_repository->index);
	തിരികെ ret;
}
