/*
 * Builtin "git merge"
 *
 * Copyright (c) 2008 Miklos Vajna <vmiklos@frugalware.org>
 *
 * Based on git-merge.sh by Junio C Hamano.
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#অন্তর্ভাব "builtin.h"

#অন্তর্ভাব "abspath.h"
#অন্তর্ভাব "advice.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "editor.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "hex.h"
#অন্তর্ভাব "object-name.h"
#অন্তর্ভাব "parse-options.h"
#অন্তর্ভাব "lockfile.h"
#অন্তর্ভাব "repository.h"
#অন্তর্ভাব "run-command.h"
#অন্তর্ভাব "hook.h"
#অন্তর্ভাব "diff.h"
#অন্তর্ভাব "diff-merges.h"
#অন্তর্ভাব "refs.h"
#অন্তর্ভাব "refspec.h"
#অন্তর্ভাব "commit.h"
#অন্তর্ভাব "diffcore.h"
#অন্তর্ভাব "path.h"
#অন্তর্ভাব "revision.h"
#অন্তর্ভাব "unpack-trees.h"
#অন্তর্ভাব "cache-tree.h"
#অন্তর্ভাব "dir.h"
#অন্তর্ভাব "color.h"
#অন্তর্ভাব "rerere.h"
#অন্তর্ভাব "help.h"
#অন্তর্ভাব "merge.h"
#অন্তর্ভাব "merge-ort-wrappers.h"
#অন্তর্ভাব "resolve-undo.h"
#অন্তর্ভাব "remote.h"
#অন্তর্ভাব "fmt-merge-msg.h"
#অন্তর্ভাব "sequencer.h"
#অন্তর্ভাব "string-list.h"
#অন্তর্ভাব "tag.h"
#অন্তর্ভাব "alias.h"
#অন্তর্ভাব "branch.h"
#অন্তর্ভাব "commit-reach.h"
#অন্তর্ভাব "wt-status.h"
#অন্তর্ভাব "commit-graph.h"

#define DEFAULT_TWOHEAD (1<<0)
#define DEFAULT_OCTOPUS (1<<1)
#define NO_FAST_FORWARD (1<<2)
#define NO_TRIVIAL      (1<<3)

সংরচনা strategy {
	ধ্রুব বর্ণ *name;
	unsigned attr;
};

স্থির ধ্রুব বর্ণ * ধ্রুব builtin_merge_usage[] = {
	N_("git merge [<options>] [<commit>...]"),
	"git merge --abort",
	"git merge --continue",
	রিক্ত
};

#define MERGE_SHOW_DIFFSTAT 1
#define MERGE_SHOW_COMPACTSUMMARY 2

স্থির পূর্ণাঙ্ক show_diffstat = MERGE_SHOW_DIFFSTAT, shortlog_len = -1, squash;
স্থির পূর্ণাঙ্ক option_commit = -1;
স্থির পূর্ণাঙ্ক option_edit = -1;
স্থির পূর্ণাঙ্ক allow_trivial = 1, have_message, verify_signatures;
স্থির পূর্ণাঙ্ক check_trust_level = 1;
স্থির পূর্ণাঙ্ক overwrite_ignore = 1;
স্থির সংরচনা strbuf merge_বার্তা = STRBUF_INIT;
স্থির সংরচনা strategy **use_strategies;
স্থির size_t use_strategies_nr, use_strategies_alloc;
স্থির সংরচনা strvec xopts = STRVEC_INIT;
স্থির ধ্রুব বর্ণ *branch;
স্থির বর্ণ *branch_mergeoptions;
স্থির পূর্ণাঙ্ক verbosity;
স্থির পূর্ণাঙ্ক allow_rerere_auto;
স্থির পূর্ণাঙ্ক abort_current_merge;
স্থির পূর্ণাঙ্ক quit_current_merge;
স্থির পূর্ণাঙ্ক continue_current_merge;
স্থির পূর্ণাঙ্ক allow_unrelated_histories;
স্থির পূর্ণাঙ্ক show_progress = -1;
স্থির পূর্ণাঙ্ক default_to_upstream = 1;
স্থির পূর্ণাঙ্ক signoff;
স্থির ধ্রুব বর্ণ *sign_commit;
স্থির পূর্ণাঙ্ক autostash;
স্থির পূর্ণাঙ্ক no_verify;
স্থির বর্ণ *into_name;

স্থির সংরচনা strategy all_strategy[] = {
	{ "recursive",  NO_TRIVIAL },
	{ "octopus",    DEFAULT_OCTOPUS },
	{ "ort",        DEFAULT_TWOHEAD | NO_TRIVIAL },
	{ "resolve",    0 },
	{ "ours",       NO_FAST_FORWARD | NO_TRIVIAL },
	{ "subtree",    NO_FAST_FORWARD | NO_TRIVIAL },
};

স্থির বর্ণ *pull_twohead, *pull_octopus;

গণনা ff_type {
	FF_NO,
	FF_ALLOW,
	FF_ONLY
};

স্থির গণনা ff_type fast_forward = FF_ALLOW;

স্থির বর্ণ *cleanup_arg;
স্থির গণনা commit_বার্তা_cleanup_mode cleanup_mode;

স্থির পূর্ণাঙ্ক option_parse_message(ধ্রুব সংরচনা option *opt,
				ধ্রুব বর্ণ *arg, পূর্ণাঙ্ক unset)
{
	সংরচনা strbuf *buf = opt->মান;

	যদি (unset)
		strbuf_setlen(buf, 0);
	অন্যথা যদি (arg) {
		strbuf_addf(buf, "%s%s", buf->len ? "\n\n" : "", arg);
		have_message = 1;
	} অন্যথা
		প্রত্যাগম error(_("switch `m' requires a value"));
	প্রত্যাগম 0;
}

স্থির গণনা parse_opt_result option_read_message(সংরচনা parse_opt_ctx_t *ctx,
						 ধ্রুব সংরচনা option *opt,
						 ধ্রুব বর্ণ *arg_not_used,
						 পূর্ণাঙ্ক unset)
{
	সংরচনা strbuf *buf = opt->মান;
	ধ্রুব বর্ণ *arg;

	BUG_ON_OPT_ARG(arg_not_used);
	যদি (unset)
		BUG("-F cannot be negated");

	যদি (ctx->opt) {
		arg = ctx->opt;
		ctx->opt = রিক্ত;
	} অন্যথা যদি (ctx->argc > 1) {
		ctx->argc--;
		arg = *++ctx->argv;
	} অন্যথা
		প্রত্যাগম error(_("option `%s' requires a value"), opt->long_name);

	যদি (buf->len)
		strbuf_addch(buf, '\n');
	যদি (ctx->prefix && !is_absolute_path(arg))
		arg = prefix_filename(ctx->prefix, arg);
	যদি (strbuf_read_file(buf, arg, 0) < 0)
		প্রত্যাগম error(_("could not read file '%s'"), arg);
	have_message = 1;

	প্রত্যাগম 0;
}

স্থির সংরচনা strategy *get_strategy(ধ্রুব বর্ণ *name)
{
	পূর্ণাঙ্ক i;
	সংরচনা strategy *ret;
	স্থির সংরচনা cmdnames main_cmds = {0}, other_cmds = {0};
	স্থির পূর্ণাঙ্ক loaded;
	বর্ণ *default_strategy = রিক্ত;

	যদি (!name)
		প্রত্যাগম রিক্ত;

	যদি (default_strategy &&
	    !strcmp(default_strategy, "ort") &&
	    !strcmp(name, "recursive")) {
		name = "ort";
	}

	প্রতি (i = 0; i < ARRAY_SIZE(all_strategy); i++)
		যদি (!strcmp(name, all_strategy[i].name))
			প্রত্যাগম &all_strategy[i];

	যদি (!loaded) {
		সংরচনা cmdnames not_strategies = {0};
		loaded = 1;

		load_command_list("git-merge-", &main_cmds, &other_cmds);
		প্রতি (i = 0; i < main_cmds.cnt; i++) {
			পূর্ণাঙ্ক j, found = 0;
			সংরচনা cmdname *ent = main_cmds.names[i];
			প্রতি (j = 0; !found && j < ARRAY_SIZE(all_strategy); j++)
				যদি (!xstrncmpz(all_strategy[j].name, ent->name, ent->len))
					found = 1;
			যদি (!found)
				add_cmdname(&not_strategies, ent->name, ent->len);
		}
		exclude_cmds(&main_cmds, &not_strategies);

		cmdnames_release(&not_strategies);
	}
	যদি (!is_in_cmdlist(&main_cmds, name) && !is_in_cmdlist(&other_cmds, name)) {
		fprintf(stderr, _("Could not find merge strategy '%s'.\n"), name);
		fprintf(stderr, _("Available strategies are:"));
		প্রতি (i = 0; i < main_cmds.cnt; i++)
			fprintf(stderr, " %s", main_cmds.names[i]->name);
		fprintf(stderr, ".\n");
		যদি (other_cmds.cnt) {
			fprintf(stderr, _("Available custom strategies are:"));
			প্রতি (i = 0; i < other_cmds.cnt; i++)
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
	প্রত্যাগম ret;
}

স্থির শূন্য append_strategy(সংরচনা strategy *s)
{
	ALLOC_GROW(use_strategies, use_strategies_nr + 1, use_strategies_alloc);
	use_strategies[use_strategies_nr++] = s;
}

স্থির পূর্ণাঙ্ক option_parse_strategy(ধ্রুব সংরচনা option *opt UNUSED,
				 ধ্রুব বর্ণ *name, পূর্ণাঙ্ক unset)
{
	যদি (unset)
		প্রত্যাগম 0;

	append_strategy(get_strategy(name));
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক option_parse_compact_summary(ধ্রুব সংরচনা option *opt,
					ধ্রুব বর্ণ *name UNUSED, পূর্ণাঙ্ক unset)
{
	পূর্ণাঙ্ক *setting = opt->মান;

	যদি (unset)
		*setting = 0;
	অন্যথা
		*setting = MERGE_SHOW_COMPACTSUMMARY;
	প্রত্যাগম 0;
}

স্থির সংরচনা option builtin_merge_options[] = {
	OPT_SET_INT('n', রিক্ত, &show_diffstat,
		N_("do not show a diffstat at the end of the merge"), 0),
	OPT_BOOL(0, "stat", &show_diffstat,
		N_("show a diffstat at the end of the merge")),
	OPT_BOOL(0, "summary", &show_diffstat, N_("(synonym to --stat)")),
	OPT_CALLBACK_F(0, "compact-summary", &show_diffstat, রিক্ত,
		       N_("show a compact-summary at the end of the merge"),
		       PARSE_OPT_NOARG,
		       option_parse_compact_summary),
	{
		.type = OPTION_INTEGER,
		.long_name = "log",
		.মান = &shortlog_len,
		.precision = পরিমাণ(shortlog_len),
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
	OPT_CALLBACK('s', "strategy", রিক্ত, N_("strategy"),
		N_("merge strategy to use"), option_parse_strategy),
	OPT_STRVEC('X', "strategy-option", &xopts, N_("option=value"),
		N_("option for selected merge strategy")),
	OPT_CALLBACK('m', "message", &merge_বার্তা, N_("message"),
		N_("merge commit message (for a non-fast-forward merge)"),
		option_parse_message),
	{
		.type = OPTION_LOWLEVEL_CALLBACK,
		.short_name = 'F',
		.long_name = "file",
		.মান = &merge_বার্তা,
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
		.মান = &sign_commit,
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

স্থির পূর্ণাঙ্ক save_state(সংরচনা object_id *stash)
{
	পূর্ণাঙ্ক len;
	সংরচনা child_process cp = CHILD_PROCESS_INIT;
	সংরচনা strbuf buffer = STRBUF_INIT;
	সংরচনা lock_file lock_file = LOCK_INIT;
	পূর্ণাঙ্ক fd;
	পূর্ণাঙ্ক rc = -1;

	fd = repo_hold_locked_index(the_repository, &lock_file, 0);
	refresh_index(the_repository->index, REFRESH_QUIET, রিক্ত, রিক্ত, রিক্ত);
	যদি (0 <= fd)
		repo_update_index_if_able(the_repository, &lock_file);
	rollback_lock_file(&lock_file);

	strvec_pushl(&cp.args, "stash", "create", রিক্ত);
	cp.out = -1;
	cp.git_cmd = 1;

	যদি (start_command(&cp))
		die(_("could not run stash."));
	len = strbuf_read(&buffer, cp.out, 1024);
	close(cp.out);

	যদি (finish_command(&cp) || len < 0)
		die(_("stash failed"));
	অন্যথা যদি (!len)		/* no changes */
		গচ্ছ out;
	strbuf_setlen(&buffer, buffer.len-1);
	যদি (repo_get_oid(the_repository, buffer.buf, stash))
		die(_("not a valid object: %s"), buffer.buf);
	rc = 0;
out:
	strbuf_release(&buffer);
	প্রত্যাগম rc;
}

স্থির শূন্য read_empty(ধ্রুব সংরচনা object_id *oid)
{
	সংরচনা child_process cmd = CHILD_PROCESS_INIT;

	strvec_pushl(&cmd.args, "read-tree", "-m", "-u",
		     empty_বৃক্ষ_oid_hex(the_repository->hash_algo),
		     oid_to_hex(oid), রিক্ত);
	cmd.git_cmd = 1;

	যদি (run_command(&cmd))
		die(_("read-tree failed"));
}

স্থির শূন্য reset_hard(ধ্রুব সংরচনা object_id *oid)
{
	সংরচনা child_process cmd = CHILD_PROCESS_INIT;

	strvec_pushl(&cmd.args, "read-tree", "-v", "--reset", "-u",
		     oid_to_hex(oid), রিক্ত);
	cmd.git_cmd = 1;

	যদি (run_command(&cmd))
		die(_("read-tree failed"));
}

স্থির শূন্য restore_state(ধ্রুব সংরচনা object_id *head,
			  ধ্রুব সংরচনা object_id *stash)
{
	সংরচনা child_process cmd = CHILD_PROCESS_INIT;

	reset_hard(head);

	যদি (is_null_oid(stash))
		গচ্ছ refresh_cache;

	strvec_pushl(&cmd.args, "stash", "apply", "--index", "--quiet", রিক্ত);
	strvec_ঠেলো(&cmd.args, oid_to_hex(stash));

	/*
	 * It is OK to ignore error here, for example when there was
	 * nothing to restore.
	 */
	cmd.git_cmd = 1;
	run_command(&cmd);

refresh_cache:
	discard_index(the_repository->index);
	যদি (repo_read_index(the_repository) < 0)
		die(_("could not read index"));
}

/* This is called when no merge was necessary. */
স্থির শূন্য finish_up_to_date(শূন্য)
{
	যদি (verbosity >= 0) {
		যদি (squash)
			puts(_("Already up to date. (nothing to squash)"));
		অন্যথা
			puts(_("Already up to date."));
	}
	remove_merge_branch_state(the_repository);
}

স্থির শূন্য squash_message(সংরচনা commit *commit, সংরচনা commit_list *remoteheads)
{
	সংরচনা rev_info rev;
	সংরচনা strbuf out = STRBUF_INIT;
	সংরচনা commit_list *j;
	সংরচনা pretty_print_context ctx = {0};

	printf(_("Squash commit -- not updating HEAD\n"));

	repo_init_revisions(the_repository, &rev, রিক্ত);
	diff_merges_suppress(&rev);
	rev.commit_format = CMIT_FMT_MEDIUM;

	commit->object.flags |= UNINTERESTING;
	add_pending_object(&rev, &commit->object, রিক্ত);

	প্রতি (j = remoteheads; j; j = j->next)
		add_pending_object(&rev, &j->item->object, রিক্ত);

	setup_revisions(0, রিক্ত, &rev, রিক্ত);
	যদি (prepare_revision_walk(&rev))
		die(_("revision walk setup failed"));

	ctx.abbrev = rev.abbrev;
	ctx.date_mode = rev.date_mode;
	ctx.fmt = rev.commit_format;

	strbuf_addstr(&out, "Squashed commit of the following:\n");
	যাবত্ ((commit = get_revision(&rev)) != রিক্ত) {
		strbuf_addch(&out, '\n');
		strbuf_addf(&out, "commit %s\n",
			oid_to_hex(&commit->object.oid));
		pretty_print_commit(&ctx, commit, &out);
	}
	write_file_buf(git_path_squash_বার্তা(the_repository), out.buf, out.len);
	strbuf_release(&out);
	release_revisions(&rev);
}

স্থির শূন্য finish(সংরচনা commit *head_commit,
		   সংরচনা commit_list *remoteheads,
		   ধ্রুব সংরচনা object_id *new_head, ধ্রুব বর্ণ *বার্তা)
{
	সংরচনা strbuf reflog_message = STRBUF_INIT;
	ধ্রুব সংরচনা object_id *head = &head_commit->object.oid;

	যদি (!বার্তা)
		strbuf_addstr(&reflog_message, getenv("GIT_REFLOG_ACTION"));
	অন্যথা {
		যদি (verbosity >= 0)
			printf("%s\n", বার্তা);
		strbuf_addf(&reflog_message, "%s: %s",
			getenv("GIT_REFLOG_ACTION"), বার্তা);
	}
	যদি (squash) {
		squash_message(head_commit, remoteheads);
	} অন্যথা {
		যদি (verbosity >= 0 && !merge_বার্তা.len)
			printf(_("No merge message -- not updating HEAD\n"));
		অন্যথা {
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
	যদি (new_head && show_diffstat) {
		সংরচনা diff_options opts;
		repo_diff_setup(the_repository, &opts);
		init_diffstat_widths(&opts);

		বিকল্প (show_diffstat) {
		পক্ষ MERGE_SHOW_DIFFSTAT: /* 1 */
			opts.output_format |=
				DIFF_FORMAT_SUMMARY | DIFF_FORMAT_DIFFSTAT;
			ভঙ্গ;
		পক্ষ MERGE_SHOW_COMPACTSUMMARY: /* 2 */
			opts.output_format |= DIFF_FORMAT_DIFFSTAT;
			opts.flags.stat_with_summary = 1;
			ভঙ্গ;
		স্বভাব:
			ভঙ্গ;
		}
		opts.detect_rename = DIFF_DETECT_RENAME;
		diff_setup_done(&opts);
		diff_বৃক্ষ_oid(head, new_head, "", &opts);
		diffcore_std(&opts);
		diff_flush(&opts);
	}

	/* Run a post-merge hook */
	run_hooks_l(the_repository, "post-merge", squash ? "1" : "0", রিক্ত);

	যদি (new_head)
		apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
				    রিক্ত, রিক্ত, রিক্ত, রিক্ত);
	strbuf_release(&reflog_message);
}

/* Get the name for the merge commit's message. */
স্থির শূন্য merge_name(ধ্রুব বর্ণ *remote, সংরচনা strbuf *বার্তা)
{
	সংরচনা commit *remote_head;
	সংরচনা object_id branch_head;
	সংরচনা strbuf bname = STRBUF_INIT;
	সংরচনা merge_remote_desc *desc;
	ধ্রুব বর্ণ *ptr;
	বর্ণ *found_ref = রিক্ত;
	পূর্ণাঙ্ক len, early;

	copy_branchname(the_repository, &bname, remote, 0);
	remote = bname.buf;

	oidclr(&branch_head, the_repository->hash_algo);
	remote_head = get_merge_parent(remote);
	যদি (!remote_head)
		die(_("'%s' does not point to a commit"), remote);

	যদি (repo_dwim_ref(the_repository, remote, strlen(remote), &branch_head,
			  &found_ref, 0) > 0) {
		যদি (starts_with(found_ref, "refs/heads/")) {
			strbuf_addf(বার্তা, "%s\t\tbranch '%s' of .\n",
				    oid_to_hex(&branch_head), remote);
			গচ্ছ cleanup;
		}
		যদি (starts_with(found_ref, "refs/tags/")) {
			strbuf_addf(বার্তা, "%s\t\ttag '%s' of .\n",
				    oid_to_hex(&branch_head), remote);
			গচ্ছ cleanup;
		}
		যদি (starts_with(found_ref, "refs/remotes/")) {
			strbuf_addf(বার্তা, "%s\t\tremote-tracking branch '%s' of .\n",
				    oid_to_hex(&branch_head), remote);
			গচ্ছ cleanup;
		}
	}

	/* See if remote matches <name>^^^.. or <name>~<number> */
	প্রতি (len = 0, ptr = remote + strlen(remote);
	     remote < ptr && ptr[-1] == '^';
	     ptr--)
		len++;
	যদি (len)
		early = 1;
	অন্যথা {
		early = 0;
		ptr = strrchr(remote, '~');
		যদি (ptr) {
			পূর্ণাঙ্ক seen_nonzero = 0;

			len++; /* count ~ */
			যাবত্ (*++ptr && isdigit(*ptr)) {
				seen_nonzero |= (*ptr != '0');
				len++;
			}
			যদি (*ptr)
				len = 0; /* not ...~<number> */
			অন্যথা যদি (seen_nonzero)
				early = 1;
			অন্যথা যদি (len == 1)
				early = 1; /* "name~" is "name~1"! */
		}
	}
	যদি (len) {
		সংরচনা strbuf truname = STRBUF_INIT;
		strbuf_addf(&truname, "refs/heads/%s", remote);
		strbuf_setlen(&truname, truname.len - len);
		যদি (refs_ref_exists(get_main_ref_store(the_repository), truname.buf)) {
			strbuf_addf(বার্তা,
				    "%s\t\tbranch '%s'%s of .\n",
				    oid_to_hex(&remote_head->object.oid),
				    truname.buf + 11,
				    (early ? " (early part)" : ""));
			strbuf_release(&truname);
			গচ্ছ cleanup;
		}
		strbuf_release(&truname);
	}

	desc = merge_remote_util(remote_head);
	যদি (desc && desc->obj && desc->obj->type == OBJ_TAG) {
		strbuf_addf(বার্তা, "%s\t\t%s '%s'\n",
			    oid_to_hex(&desc->obj->oid),
			    type_name(desc->obj->type),
			    remote);
		গচ্ছ cleanup;
	}

	strbuf_addf(বার্তা, "%s\t\tcommit '%s'\n",
		oid_to_hex(&remote_head->object.oid), remote);
cleanup:
	মুক্ত(found_ref);
	strbuf_release(&bname);
}

স্থির শূন্য parse_branch_merge_options(বর্ণ *bmo)
{
	ধ্রুব বর্ণ **argv;
	পূর্ণাঙ্ক argc;

	যদি (!bmo)
		প্রত্যাগম;
	argc = split_cmdline(bmo, &argv);
	যদি (argc < 0)
		die(_("Bad branch.%s.mergeoptions string: %s"), branch,
		    _(split_cmdline_strerror(argc)));
	REALLOC_ARRAY(argv, argc + 2);
	MOVE_ARRAY(argv + 1, argv, argc + 1);
	argc++;
	argv[0] = "branch.*.mergeoptions";
	parse_options(argc, argv, রিক্ত, builtin_merge_options,
		      builtin_merge_usage, 0);
	মুক্ত(argv);
}

স্থির পূর্ণাঙ্ক git_merge_config(ধ্রুব বর্ণ *k, ধ্রুব বর্ণ *v,
			    ধ্রুব সংরচনা config_context *ctx, শূন্য *cb)
{
	পূর্ণাঙ্ক status;
	ধ্রুব বর্ণ *str;

	যদি (branch &&
	    skip_prefix(k, "branch.", &str) &&
	    skip_prefix(str, branch, &str) &&
	    !strcmp(str, ".mergeoptions")) {
		মুক্ত(branch_mergeoptions);
		branch_mergeoptions = xstrdup(v);
		প্রত্যাগম 0;
	}

	যদি (!strcmp(k, "merge.diffstat") || !strcmp(k, "merge.stat")) {
		পূর্ণাঙ্ক val = git_parse_maybe_bool_text(v);
		বিকল্প (val) {
		পক্ষ 0:
			show_diffstat = 0;
			ভঙ্গ;
		পক্ষ 1:
			show_diffstat = MERGE_SHOW_DIFFSTAT;
			ভঙ্গ;
		স্বভাব:
			যদি (!strcmp(v, "compact"))
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
			অন্যথা
				/*
				 * A setting from a future?  It is not an
				 * error grave enough to fail the command.
				 * proceed using the default one.
				 */
				show_diffstat = MERGE_SHOW_DIFFSTAT;
			ভঙ্গ;
		}
	} অন্যথা যদি (!strcmp(k, "merge.verifysignatures")) {
		verify_signatures = git_config_bool(k, v);
	} অন্যথা যদি (!strcmp(k, "pull.twohead")) {
		FREE_AND_NULL(pull_twohead);
		প্রত্যাগম git_config_string(&pull_twohead, k, v);
	} অন্যথা যদি (!strcmp(k, "pull.octopus")) {
		FREE_AND_NULL(pull_octopus);
		প্রত্যাগম git_config_string(&pull_octopus, k, v);
	} অন্যথা যদি (!strcmp(k, "commit.cleanup")) {
		প্রত্যাগম git_config_string(&cleanup_arg, k, v);
	} অন্যথা যদি (!strcmp(k, "merge.ff")) {
		পূর্ণাঙ্ক boolval = git_parse_maybe_bool(v);
		যদি (0 <= boolval) {
			fast_forward = boolval ? FF_ALLOW : FF_NO;
		} অন্যথা যদি (v && !strcmp(v, "only")) {
			fast_forward = FF_ONLY;
		} /* do not barf on values from future versions of git */
		প্রত্যাগম 0;
	} অন্যথা যদি (!strcmp(k, "merge.defaulttoupstream")) {
		default_to_upstream = git_config_bool(k, v);
		প্রত্যাগম 0;
	} অন্যথা যদি (!strcmp(k, "commit.gpgsign")) {
		sign_commit = git_config_bool(k, v) ? "" : রিক্ত;
		প্রত্যাগম 0;
	} অন্যথা যদি (!strcmp(k, "gpg.mintrustlevel")) {
		check_trust_level = 0;
	} অন্যথা যদি (!strcmp(k, "merge.autostash")) {
		autostash = git_config_bool(k, v);
		প্রত্যাগম 0;
	}

	status = fmt_merge_বার্তা_config(k, v, ctx, cb);
	যদি (status)
		প্রত্যাগম status;
	প্রত্যাগম git_diff_ui_config(k, v, ctx, cb);
}

স্থির পূর্ণাঙ্ক read_বৃক্ষ_trivial(সংরচনা object_id *common, সংরচনা object_id *head,
			     সংরচনা object_id *one)
{
	পূর্ণাঙ্ক i, nr_trees = 0;
	সংরচনা বৃক্ষ *trees[MAX_UNPACK_TREES];
	সংরচনা বৃক্ষ_desc t[MAX_UNPACK_TREES];
	সংরচনা unpack_trees_options opts;

	memset(&opts, 0, পরিমাণ(opts));
	opts.head_idx = 2;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;
	opts.update = 1;
	opts.verbose_update = 1;
	opts.trivial_merges_only = 1;
	opts.merge = 1;
	opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
	trees[nr_trees] = repo_parse_বৃক্ষ_indirect(the_repository, common);
	যদি (!trees[nr_trees++])
		প্রত্যাগম -1;
	trees[nr_trees] = repo_parse_বৃক্ষ_indirect(the_repository, head);
	যদি (!trees[nr_trees++])
		প্রত্যাগম -1;
	trees[nr_trees] = repo_parse_বৃক্ষ_indirect(the_repository, one);
	যদি (!trees[nr_trees++])
		প্রত্যাগম -1;
	opts.fn = threeway_merge;
	cache_বৃক্ষ_free(&the_repository->index->cache_বৃক্ষ);
	প্রতি (i = 0; i < nr_trees; i++) {
		repo_parse_বৃক্ষ(the_repository, trees[i]);
		init_বৃক্ষ_desc(t+i, &trees[i]->object.oid,
			       trees[i]->buffer, trees[i]->size);
	}
	যদি (unpack_trees(nr_trees, t, &opts))
		প্রত্যাগম -1;
	প্রত্যাগম 0;
}

স্থির শূন্য write_বৃক্ষ_trivial(সংরচনা object_id *oid)
{
	যদি (write_index_as_বৃক্ষ(oid, the_repository->index,
				repo_get_index_file(the_repository),
				0, রিক্ত))
		die(_("git write-tree failed to write a tree"));
}

স্থির পূর্ণাঙ্ক try_merge_strategy(ধ্রুব বর্ণ *strategy, সংরচনা commit_list *common,
			      সংরচনা commit_list *remoteheads,
			      সংরচনা commit *head)
{
	ধ্রুব বর্ণ *head_arg = "HEAD";

	যদি (repo_refresh_and_write_index(the_repository, REFRESH_QUIET,
					 SKIP_IF_UNCHANGED, 0, রিক্ত, রিক্ত,
					 রিক্ত) < 0)
		die(_("Unable to write index."));

	যদি (!strcmp(strategy, "recursive") || !strcmp(strategy, "subtree") ||
	    !strcmp(strategy, "ort")) {
		সংরচনা lock_file lock = LOCK_INIT;
		পূর্ণাঙ্ক clean, x;
		সংরচনা commit *result;
		সংরচনা commit_list *reversed = রিক্ত;
		সংরচনা merge_options o;
		সংরচনা commit_list *j;

		যদি (remoteheads->next) {
			error(_("Not handling anything other than two heads merge."));
			প্রত্যাগম 2;
		}

		init_ui_merge_options(&o, the_repository);
		যদি (!strcmp(strategy, "subtree"))
			o.subtree_shift = "";

		o.show_rename_progress =
			show_progress == -1 ? isatty(2) : show_progress;

		প্রতি (x = 0; x < xopts.nr; x++)
			যদি (parse_merge_opt(&o, xopts.v[x]))
				die(_("unknown strategy option: -X%s"), xopts.v[x]);

		o.branch1 = head_arg;
		o.branch2 = merge_remote_util(remoteheads->item)->name;

		প্রতি (j = common; j; j = j->next)
			commit_list_ঢোকাও(j->item, &reversed);

		repo_hold_locked_index(the_repository, &lock,
				       LOCK_DIE_ON_ERROR);
		clean = merge_ort_recursive(&o, head, remoteheads->item,
					    reversed, &result);
		commit_list_free(reversed);
		strbuf_release(&o.obuf);

		যদি (clean < 0) {
			rollback_lock_file(&lock);
			প্রত্যাগম 2;
		}
		যদি (write_locked_index(the_repository->index, &lock,
				       COMMIT_LOCK | SKIP_IF_UNCHANGED))
			die(_("unable to write %s"), repo_get_index_file(the_repository));
		প্রত্যাগম clean ? 0 : 1;
	} অন্যথা {
		প্রত্যাগম try_merge_command(the_repository,
					 strategy, xopts.nr, xopts.v,
					 common, head_arg, remoteheads);
	}
}

স্থির শূন্য গুনতি_diff_files(সংরচনা diff_queue_struct *q,
			     সংরচনা diff_options *opt UNUSED, শূন্য *data)
{
	পূর্ণাঙ্ক *গুনতি = data;

	(*গুনতি) += q->nr;
}

স্থির পূর্ণাঙ্ক গুনতি_unmerged_entries(শূন্য)
{
	পূর্ণাঙ্ক i, ret = 0;

	প্রতি (i = 0; i < the_repository->index->cache_nr; i++)
		যদি (ce_stage(the_repository->index->cache[i]))
			ret++;

	প্রত্যাগম ret;
}

স্থির শূন্য add_strategies(ধ্রুব বর্ণ *string, unsigned attr)
{
	পূর্ণাঙ্ক i;

	যদি (string) {
		সংরচনা string_list list = STRING_LIST_INIT_DUP;
		সংরচনা string_list_item *item;
		string_list_split(&list, string, " ", -1);
		for_each_string_list_item(item, &list)
			append_strategy(get_strategy(item->string));
		string_list_clear(&list, 0);
		প্রত্যাগম;
	}
	প্রতি (i = 0; i < ARRAY_SIZE(all_strategy); i++)
		যদি (all_strategy[i].attr & attr)
			append_strategy(&all_strategy[i]);

}

স্থির শূন্য read_merge_বার্তা(সংরচনা strbuf *বার্তা)
{
	ধ্রুব বর্ণ *filename = git_path_merge_বার্তা(the_repository);
	strbuf_reset(বার্তা);
	যদি (strbuf_read_file(বার্তা, filename, 0) < 0)
		die_errno(_("Could not read from '%s'"), filename);
}

স্থির শূন্য write_merge_state(সংরচনা commit_list *);
স্থির শূন্য abort_commit(সংরচনা commit_list *remoteheads, ধ্রুব বর্ণ *err_বার্তা)
{
	যদি (err_বার্তা)
		error("%s", err_বার্তা);
	fprintf(stderr,
		_("Not committing merge; use 'git commit' to complete the merge.\n"));
	write_merge_state(remoteheads);
	exit(1);
}

স্থির ধ্রুব বর্ণ merge_editor_comment[] =
N_("Please enter a commit message to explain why this merge is necessary,\n"
   "especially if it merges an updated upstream into a topic branch.\n"
   "\n");

স্থির ধ্রুব বর্ণ scissors_editor_comment[] =
N_("An empty message aborts the commit.\n");

স্থির ধ্রুব বর্ণ no_scissors_editor_comment[] =
N_("Lines starting with '%s' will be ignored, and an empty message aborts\n"
   "the commit.\n");

স্থির শূন্য write_merge_heads(সংরচনা commit_list *);
স্থির শূন্য prepare_to_commit(সংরচনা commit_list *remoteheads)
{
	সংরচনা strbuf বার্তা = STRBUF_INIT;
	ধ্রুব বর্ণ *index_file = repo_get_index_file(the_repository);

	যদি (!no_verify) {
		পূর্ণাঙ্ক invoked_hook;

		যদি (run_commit_hook(0 < option_edit, index_file, &invoked_hook,
				    "pre-merge-commit", রিক্ত))
			abort_commit(remoteheads, রিক্ত);
		/*
		 * Re-read the index as pre-merge-commit hook could have updated it,
		 * and write it out as a tree.  We must do this before we invoke
		 * the editor and after we invoke run_status above.
		 */
		যদি (invoked_hook)
			discard_index(the_repository->index);
	}
	read_index_from(the_repository->index, index_file,
			repo_get_git_dir(the_repository));
	strbuf_addbuf(&বার্তা, &merge_বার্তা);
	যদি (squash)
		BUG("the control must not reach here under --squash");
	যদি (0 < option_edit) {
		strbuf_addch(&বার্তা, '\n');
		যদি (cleanup_mode == COMMIT_বার্তা_CLEANUP_SCISSORS) {
			wt_status_append_cut_সারি(&বার্তা);
			strbuf_commented_addf(&বার্তা, comment_সারি_str, "\n");
		}
		strbuf_commented_addf(&বার্তা, comment_সারি_str,
				      _(merge_editor_comment));
		যদি (cleanup_mode == COMMIT_বার্তা_CLEANUP_SCISSORS)
			strbuf_commented_addf(&বার্তা, comment_সারি_str,
					      _(scissors_editor_comment));
		অন্যথা
			strbuf_commented_addf(&বার্তা, comment_সারি_str,
				_(no_scissors_editor_comment), comment_সারি_str);
	}
	যদি (signoff)
		append_signoff(&বার্তা, ignored_log_message_bytes(বার্তা.buf, বার্তা.len), 0);
	write_merge_heads(remoteheads);
	write_file_buf(git_path_merge_বার্তা(the_repository), বার্তা.buf, বার্তা.len);
	যদি (run_commit_hook(0 < option_edit, repo_get_index_file(the_repository),
			    রিক্ত, "prepare-commit-msg",
			    git_path_merge_বার্তা(the_repository), "merge", রিক্ত))
		abort_commit(remoteheads, রিক্ত);
	যদি (0 < option_edit) {
		যদি (launch_editor(git_path_merge_বার্তা(the_repository), রিক্ত, রিক্ত))
			abort_commit(remoteheads, রিক্ত);
	}

	যদি (!no_verify && run_commit_hook(0 < option_edit, repo_get_index_file(the_repository),
					  রিক্ত, "commit-msg",
					  git_path_merge_বার্তা(the_repository), রিক্ত))
		abort_commit(remoteheads, রিক্ত);

	read_merge_বার্তা(&বার্তা);
	cleanup_message(&বার্তা, cleanup_mode, 0);
	যদি (!বার্তা.len)
		abort_commit(remoteheads, _("Empty commit message."));
	strbuf_release(&merge_বার্তা);
	strbuf_addbuf(&merge_বার্তা, &বার্তা);
	strbuf_release(&বার্তা);
}

স্থির পূর্ণাঙ্ক merge_trivial(সংরচনা commit *head, সংরচনা commit_list *remoteheads)
{
	সংরচনা object_id result_বৃক্ষ, result_commit;
	সংরচনা commit_list *parents = রিক্ত, **pptr = &parents;

	যদি (repo_refresh_and_write_index(the_repository, REFRESH_QUIET,
					 SKIP_IF_UNCHANGED, 0, রিক্ত, রিক্ত,
					 রিক্ত) < 0)
		প্রত্যাগম error(_("Unable to write index."));

	write_বৃক্ষ_trivial(&result_বৃক্ষ);
	printf(_("Wonderful.\n"));
	pptr = commit_list_append(head, pptr);
	pptr = commit_list_append(remoteheads->item, pptr);
	prepare_to_commit(remoteheads);
	যদি (commit_বৃক্ষ(merge_বার্তা.buf, merge_বার্তা.len, &result_বৃক্ষ, parents,
			&result_commit, রিক্ত, sign_commit))
		die(_("failed to write commit object"));
	finish(head, remoteheads, &result_commit, "In-index merge");

	remove_merge_branch_state(the_repository);
	commit_list_free(parents);
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক finish_automerge(সংরচনা commit *head,
			    পূর্ণাঙ্ক head_subsumed,
			    সংরচনা commit_list *common,
			    সংরচনা commit_list *remoteheads,
			    সংরচনা object_id *result_বৃক্ষ,
			    ধ্রুব বর্ণ *wt_strategy)
{
	সংরচনা commit_list *parents = রিক্ত;
	সংরচনা strbuf buf = STRBUF_INIT;
	সংরচনা object_id result_commit;

	write_বৃক্ষ_trivial(result_বৃক্ষ);
	commit_list_free(common);
	parents = remoteheads;
	যদি (!head_subsumed || fast_forward == FF_NO)
		commit_list_ঢোকাও(head, &parents);
	prepare_to_commit(remoteheads);
	যদি (commit_বৃক্ষ(merge_বার্তা.buf, merge_বার্তা.len, result_বৃক্ষ, parents,
			&result_commit, রিক্ত, sign_commit))
		die(_("failed to write commit object"));
	strbuf_addf(&buf, "Merge made by the '%s' strategy.", wt_strategy);
	finish(head, remoteheads, &result_commit, buf.buf);

	strbuf_release(&buf);
	remove_merge_branch_state(the_repository);
	commit_list_free(parents);
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক suggest_conflicts(শূন্য)
{
	ধ্রুব বর্ণ *filename;
	FILE *fp;
	সংরচনা strbuf msgbuf = STRBUF_INIT;

	filename = git_path_merge_বার্তা(the_repository);
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
	প্রত্যাগম 1;
}

স্থির পূর্ণাঙ্ক evaluate_result(শূন্য)
{
	পূর্ণাঙ্ক cnt = 0;
	সংরচনা rev_info rev;

	/* Check how many files differ. */
	repo_init_revisions(the_repository, &rev, "");
	setup_revisions(0, রিক্ত, &rev, রিক্ত);
	rev.diffopt.output_format |=
		DIFF_FORMAT_CALLBACK;
	rev.diffopt.format_callback = গুনতি_diff_files;
	rev.diffopt.format_callback_data = &cnt;
	run_diff_files(&rev, 0);

	/*
	 * Check how many unmerged entries are
	 * there.
	 */
	cnt += গুনতি_unmerged_entries();

	release_revisions(&rev);
	প্রত্যাগম cnt;
}

/*
 * Pretend as if the user told us to merge with the remote-tracking
 * branch we have for the upstream of the current branch
 */
স্থির পূর্ণাঙ্ক setup_with_upstream(ধ্রুব বর্ণ ***argv)
{
	সংরচনা branch *branch = branch_get(রিক্ত);
	পূর্ণাঙ্ক i;
	ধ্রুব বর্ণ **args;

	যদি (!branch)
		die(_("No current branch."));
	যদি (!branch->remote_name)
		die(_("No remote for the current branch."));
	যদি (!branch->merge_nr)
		die(_("No default upstream defined for the current branch."));

	args = xcalloc(st_add(branch->merge_nr, 1), পরিমাণ(বর্ণ *));
	প্রতি (i = 0; i < branch->merge_nr; i++) {
		যদি (!branch->merge[i]->dst)
			die(_("No remote-tracking branch for %s from %s"),
			    branch->merge[i]->src, branch->remote_name);
		args[i] = branch->merge[i]->dst;
	}
	args[i] = রিক্ত;
	*argv = args;
	প্রত্যাগম i;
}

স্থির শূন্য write_merge_heads(সংরচনা commit_list *remoteheads)
{
	সংরচনা commit_list *j;
	সংরচনা strbuf buf = STRBUF_INIT;

	প্রতি (j = remoteheads; j; j = j->next) {
		সংরচনা object_id *oid;
		সংরচনা commit *c = j->item;
		সংরচনা merge_remote_desc *desc;

		desc = merge_remote_util(c);
		যদি (desc && desc->obj) {
			oid = &desc->obj->oid;
		} অন্যথা {
			oid = &c->object.oid;
		}
		strbuf_addf(&buf, "%s\n", oid_to_hex(oid));
	}
	write_file_buf(git_path_merge_head(the_repository), buf.buf, buf.len);

	strbuf_reset(&buf);
	যদি (fast_forward == FF_NO)
		strbuf_addstr(&buf, "no-ff");
	write_file_buf(git_path_merge_mode(the_repository), buf.buf, buf.len);
	strbuf_release(&buf);
}

স্থির শূন্য write_merge_state(সংরচনা commit_list *remoteheads)
{
	write_merge_heads(remoteheads);
	strbuf_addch(&merge_বার্তা, '\n');
	write_file_buf(git_path_merge_বার্তা(the_repository), merge_বার্তা.buf,
		       merge_বার্তা.len);
}

স্থির পূর্ণাঙ্ক default_edit_option(শূন্য)
{
	স্থির ধ্রুব বর্ণ name[] = "GIT_MERGE_AUTOEDIT";
	ধ্রুব বর্ণ *e = getenv(name);
	সংরচনা stat st_stdin, st_stdout;

	যদি (have_message)
		/* an explicit -m msg without --[no-]edit */
		প্রত্যাগম 0;

	যদি (e) {
		পূর্ণাঙ্ক v = git_parse_maybe_bool(e);
		যদি (v < 0)
			die(_("Bad value '%s' in environment '%s'"), e, name);
		প্রত্যাগম v;
	}

	/* Use editor if stdin and stdout are the same and is a tty */
	প্রত্যাগম (!fstat(0, &st_stdin) &&
		!fstat(1, &st_stdout) &&
		isatty(0) && isatty(1) &&
		st_stdin.st_dev == st_stdout.st_dev &&
		st_stdin.st_ino == st_stdout.st_ino &&
		st_stdin.st_mode == st_stdout.st_mode);
}

স্থির সংরচনা commit_list *reduce_parents(সংরচনা commit *head_commit,
					  পূর্ণাঙ্ক *head_subsumed,
					  সংরচনা commit_list *remoteheads)
{
	সংরচনা commit_list *parents, **remotes;

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

	remoteheads = রিক্ত;
	remotes = &remoteheads;
	যাবত্ (parents) {
		সংরচনা commit *commit = বের_করো_commit(&parents);
		যদি (commit == head_commit)
			*head_subsumed = 0;
		অন্যথা
			remotes = &commit_list_ঢোকাও(commit, remotes)->next;
	}
	প্রত্যাগম remoteheads;
}

স্থির শূন্য prepare_merge_message(সংরচনা strbuf *merge_names, সংরচনা strbuf *merge_বার্তা)
{
	সংরচনা fmt_merge_বার্তা_opts opts;

	memset(&opts, 0, পরিমাণ(opts));
	opts.add_title = !have_message;
	opts.shortlog_len = shortlog_len;
	opts.credit_people = (0 < option_edit);
	opts.into_name = into_name;

	fmt_merge_বার্তা(merge_names, merge_বার্তা, &opts);
	যদি (merge_বার্তা->len)
		strbuf_setlen(merge_বার্তা, merge_বার্তা->len - 1);
}

স্থির শূন্য handle_fetch_head(সংরচনা commit_list **remotes, সংরচনা strbuf *merge_names)
{
	ধ্রুব বর্ণ *filename;
	পূর্ণাঙ্ক fd, pos, npos;
	সংরচনা strbuf fetch_head_file = STRBUF_INIT;
	ধ্রুব unsigned hexsz = the_hash_algo->hexsz;

	যদি (!merge_names)
		merge_names = &fetch_head_file;

	filename = git_path_fetch_head(the_repository);
	fd = xopen(filename, O_RDONLY);

	যদি (strbuf_read(merge_names, fd, 0) < 0)
		die_errno(_("could not read '%s'"), filename);
	যদি (close(fd) < 0)
		die_errno(_("could not close '%s'"), filename);

	প্রতি (pos = 0; pos < merge_names->len; pos = npos) {
		সংরচনা object_id oid;
		বর্ণ *ptr;
		সংরচনা commit *commit;

		ptr = strchr(merge_names->buf + pos, '\n');
		যদি (ptr)
			npos = ptr - merge_names->buf + 1;
		অন্যথা
			npos = merge_names->len;

		যদি (npos - pos < hexsz + 2 ||
		    get_oid_hex(merge_names->buf + pos, &oid))
			commit = রিক্ত; /* bad */
		অন্যথা যদি (memcmp(merge_names->buf + pos + hexsz, "\t\t", 2))
			অনুবর্ত; /* not-for-merge */
		অন্যথা {
			বর্ণ saved = merge_names->buf[pos + hexsz];
			merge_names->buf[pos + hexsz] = '\0';
			commit = get_merge_parent(merge_names->buf + pos);
			merge_names->buf[pos + hexsz] = saved;
		}
		যদি (!commit) {
			যদি (ptr)
				*ptr = '\0';
			die(_("not something we can merge in %s: %s"),
			    filename, merge_names->buf + pos);
		}
		remotes = &commit_list_ঢোকাও(commit, remotes)->next;
	}

	যদি (merge_names == &fetch_head_file)
		strbuf_release(&fetch_head_file);
}

স্থির সংরচনা commit_list *collect_parents(সংরচনা commit *head_commit,
					   পূর্ণাঙ্ক *head_subsumed,
					   পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv,
					   সংরচনা strbuf *merge_বার্তা)
{
	পূর্ণাঙ্ক i;
	সংরচনা commit_list *remoteheads = রিক্ত;
	সংরচনা commit_list **remotes = &remoteheads;
	সংরচনা strbuf merge_names = STRBUF_INIT, *autogen = রিক্ত;

	যদি (merge_বার্তা && (!have_message || shortlog_len))
		autogen = &merge_names;

	যদি (head_commit)
		remotes = &commit_list_ঢোকাও(head_commit, remotes)->next;

	যদি (argc == 1 && !strcmp(argv[0], "FETCH_HEAD")) {
		handle_fetch_head(remotes, autogen);
		remoteheads = reduce_parents(head_commit, head_subsumed, remoteheads);
	} অন্যথা {
		প্রতি (i = 0; i < argc; i++) {
			সংরচনা commit *commit = get_merge_parent(argv[i]);
			যদি (!commit)
				help_unknown_ref(argv[i], "merge",
						 _("not something we can merge"));
			remotes = &commit_list_ঢোকাও(commit, remotes)->next;
		}
		remoteheads = reduce_parents(head_commit, head_subsumed, remoteheads);
		যদি (autogen) {
			সংরচনা commit_list *p;
			প্রতি (p = remoteheads; p; p = p->next)
				merge_name(merge_remote_util(p->item)->name, autogen);
		}
	}

	যদি (autogen) {
		prepare_merge_message(autogen, merge_বার্তা);
		strbuf_release(autogen);
	}

	প্রত্যাগম remoteheads;
}

স্থির পূর্ণাঙ্ক merging_a_throwaway_tag(সংরচনা commit *commit)
{
	বর্ণ *tag_ref;
	সংরচনা object_id oid;
	পূর্ণাঙ্ক is_throwaway_tag = 0;

	/* Are we merging a tag? */
	যদি (!merge_remote_util(commit) ||
	    !merge_remote_util(commit)->obj ||
	    merge_remote_util(commit)->obj->type != OBJ_TAG)
		প্রত্যাগম is_throwaway_tag;

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
			  ((সংরচনা tag *)merge_remote_util(commit)->obj)->tag);
	যদি (!refs_read_ref(get_main_ref_store(the_repository), tag_ref, &oid) &&
	    oideq(&oid, &merge_remote_util(commit)->obj->oid))
		is_throwaway_tag = 0;
	অন্যথা
		is_throwaway_tag = 1;
	মুক্ত(tag_ref);
	প্রত্যাগম is_throwaway_tag;
}

পূর্ণাঙ্ক cmd_merge(পূর্ণাঙ্ক argc,
	      ধ্রুব বর্ণ **argv,
	      ধ্রুব বর্ণ *prefix,
	      সংরচনা repository *repo UNUSED)
{
	সংরচনা object_id result_বৃক্ষ, stash, head_oid;
	সংরচনা commit *head_commit;
	সংরচনা strbuf buf = STRBUF_INIT;
	পূর্ণাঙ্ক i, ret = 0, head_subsumed;
	পূর্ণাঙ্ক best_cnt = -1, merge_was_ok = 0, automerge_was_ok = 0;
	সংরচনা commit_list *common = রিক্ত;
	ধ্রুব বর্ণ *best_strategy = রিক্ত, *wt_strategy = রিক্ত;
	সংরচনা commit_list *remoteheads = রিক্ত, *p;
	শূন্য *branch_to_free, *argv_to_free = রিক্ত;
	পূর্ণাঙ্ক orig_argc = argc;
	পূর্ণাঙ্ক merge_log_config = -1;

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
						      রিক্ত);
	যদি (branch)
		skip_prefix(branch, "refs/heads/", &branch);

	init_diff_ui_defaults();
	repo_config(the_repository, git_merge_config, &merge_log_config);

	যদি (!branch || is_null_oid(&head_oid))
		head_commit = রিক্ত;
	অন্যথা
		head_commit = lookup_commit_or_die(&head_oid, "HEAD");

	যদি (branch_mergeoptions)
		parse_branch_merge_options(branch_mergeoptions);
	argc = parse_options(argc, argv, prefix, builtin_merge_options,
			builtin_merge_usage, 0);
	যদি (shortlog_len < 0)
		shortlog_len = (merge_log_config > 0) ? merge_log_config : 0;

	যদি (verbosity < 0 && show_progress == -1)
		show_progress = 0;

	যদি (abort_current_merge) {
		পূর্ণাঙ্ক nargc = 2;
		ধ্রুব বর্ণ *nargv[] = {"reset", "--merge", রিক্ত};
		বর্ণ stash_oid_hex[GIT_MAX_HEXSZ + 1];
		সংরচনা object_id stash_oid = {0};

		যদি (orig_argc != 2)
			usage_বার্তা_opt(_("--abort expects no arguments"),
			      builtin_merge_usage, builtin_merge_options);

		যদি (!file_exists(git_path_merge_head(the_repository)))
			die(_("There is no merge to abort (MERGE_HEAD missing)."));

		যদি (!refs_read_ref(get_main_ref_store(the_repository), "MERGE_AUTOSTASH", &stash_oid))
			refs_delete_ref(get_main_ref_store(the_repository),
					"", "MERGE_AUTOSTASH", &stash_oid,
					REF_NO_DEREF);

		/* Invoke 'git reset --merge' */
		ret = cmd_reset(nargc, nargv, prefix, the_repository);

		যদি (!is_null_oid(&stash_oid)) {
			oid_to_hex_r(stash_oid_hex, &stash_oid);
			apply_autostash_oid(stash_oid_hex);
		}

		গচ্ছ done;
	}

	যদি (quit_current_merge) {
		যদি (orig_argc != 2)
			usage_বার্তা_opt(_("--quit expects no arguments"),
				      builtin_merge_usage,
				      builtin_merge_options);

		remove_merge_branch_state(the_repository);
		গচ্ছ done;
	}

	যদি (continue_current_merge) {
		পূর্ণাঙ্ক nargc = 1;
		ধ্রুব বর্ণ *nargv[] = {"commit", রিক্ত};

		যদি (orig_argc != 2)
			usage_বার্তা_opt(_("--continue expects no arguments"),
			      builtin_merge_usage, builtin_merge_options);

		যদি (!file_exists(git_path_merge_head(the_repository)))
			die(_("There is no merge in progress (MERGE_HEAD missing)."));

		/* Invoke 'git commit' */
		ret = cmd_commit(nargc, nargv, prefix, the_repository);
		গচ্ছ done;
	}

	যদি (repo_read_index_unmerged(the_repository))
		die_resolve_conflict("merge");

	যদি (file_exists(git_path_merge_head(the_repository))) {
		/*
		 * There is no unmerged entry, don't advise 'git
		 * add/rm <file>', just 'git commit'.
		 */
		যদি (advice_enabled(ADVICE_RESOLVE_CONFLICT))
			die(_("You have not concluded your merge (MERGE_HEAD exists).\n"
				  "Please, commit your changes before you merge."));
		অন্যথা
			die(_("You have not concluded your merge (MERGE_HEAD exists)."));
	}
	যদি (refs_ref_exists(get_main_ref_store(the_repository), "CHERRY_PICK_HEAD")) {
		যদি (advice_enabled(ADVICE_RESOLVE_CONFLICT))
			die(_("You have not concluded your cherry-pick (CHERRY_PICK_HEAD exists).\n"
			    "Please, commit your changes before you merge."));
		অন্যথা
			die(_("You have not concluded your cherry-pick (CHERRY_PICK_HEAD exists)."));
	}
	resolve_undo_clear_index(the_repository->index);

	যদি (option_edit < 0)
		option_edit = default_edit_option();

	cleanup_mode = get_cleanup_mode(cleanup_arg, 0 < option_edit);

	যদি (verbosity < 0)
		show_diffstat = 0;

	যদি (squash) {
		যদি (fast_forward == FF_NO)
			die(_("options '%s' and '%s' cannot be used together"), "--squash", "--no-ff.");
		যদি (option_commit > 0)
			die(_("options '%s' and '%s' cannot be used together"), "--squash", "--commit.");
		/*
		 * squash can now silently disable option_commit - this is not
		 * a problem as it is only overriding the default, not a user
		 * supplied option.
		 */
		option_commit = 0;
	}

	যদি (option_commit < 0)
		option_commit = 1;

	যদি (!argc) {
		যদি (default_to_upstream) {
			argc = setup_with_upstream(&argv);
			argv_to_free = argv;
		}
		অন্যথা
			die(_("No commit specified and merge.defaultToUpstream not set."));
	} অন্যথা যদি (argc == 1 && !strcmp(argv[0], "-")) {
		argv[0] = "@{-1}";
	}

	যদি (!argc)
		usage_with_options(builtin_merge_usage,
			builtin_merge_options);

	যদি (!head_commit) {
		/*
		 * If the merged head is a valid one there is no reason
		 * to forbid "git merge" into a branch yet to be born.
		 * We do the same for "git pull".
		 */
		সংরচনা object_id *remote_head_oid;
		যদি (squash)
			die(_("Squash commit into empty head not supported yet"));
		যদি (fast_forward == FF_NO)
			die(_("Non-fast-forward commit does not make sense into "
			    "an empty head"));
		remoteheads = collect_parents(head_commit, &head_subsumed,
					      argc, argv, রিক্ত);
		যদি (!remoteheads)
			die(_("%s - not something we can merge"), argv[0]);
		যদি (remoteheads->next)
			die(_("Can merge only exactly one commit into empty head"));

		যদি (verify_signatures)
			verify_merge_signature(remoteheads->item, verbosity,
					       check_trust_level);

		remote_head_oid = &remoteheads->item->object.oid;
		read_empty(remote_head_oid);
		refs_update_ref(get_main_ref_store(the_repository),
				"initial pull", "HEAD", remote_head_oid, রিক্ত,
				0,
				UPDATE_REFS_DIE_ON_ERR);
		গচ্ছ done;
	}

	/*
	 * All the rest are the commits being merged; prepare
	 * the standard merge summary message to be appended
	 * to the given message.
	 */
	remoteheads = collect_parents(head_commit, &head_subsumed,
				      argc, argv, &merge_বার্তা);

	যদি (!head_commit || !argc)
		usage_with_options(builtin_merge_usage,
			builtin_merge_options);

	যদি (verify_signatures) {
		প্রতি (p = remoteheads; p; p = p->next) {
			verify_merge_signature(p->item, verbosity,
					       check_trust_level);
		}
	}

	strbuf_addstr(&buf, "merge");
	প্রতি (p = remoteheads; p; p = p->next)
		strbuf_addf(&buf, " %s", merge_remote_util(p->item)->name);
	setenv("GIT_REFLOG_ACTION", buf.buf, 0);
	strbuf_reset(&buf);

	প্রতি (p = remoteheads; p; p = p->next) {
		সংরচনা commit *commit = p->item;
		strbuf_addf(&buf, "GITHEAD_%s",
			    oid_to_hex(&commit->object.oid));
		setenv(buf.buf, merge_remote_util(commit)->name, 1);
		strbuf_reset(&buf);
		যদি (fast_forward != FF_ONLY && merging_a_throwaway_tag(commit))
			fast_forward = FF_NO;
	}

	যদি (!use_strategies) {
		যদি (!remoteheads)
			; /* already up-to-date */
		অন্যথা যদি (!remoteheads->next)
			add_strategies(pull_twohead, DEFAULT_TWOHEAD);
		অন্যথা
			add_strategies(pull_octopus, DEFAULT_OCTOPUS);
	}

	প্রতি (i = 0; i < use_strategies_nr; i++) {
		যদি (use_strategies[i]->attr & NO_FAST_FORWARD)
			fast_forward = FF_NO;
		যদি (use_strategies[i]->attr & NO_TRIVIAL)
			allow_trivial = 0;
	}

	যদি (!remoteheads)
		; /* already up-to-date */
	অন্যথা যদি (!remoteheads->next) {
		যদি (repo_get_merge_bases(the_repository, head_commit,
					 remoteheads->item, &common) < 0) {
			ret = 2;
			গচ্ছ done;
		}
	} অন্যথা {
		সংরচনা commit_list *list = remoteheads;
		commit_list_ঢোকাও(head_commit, &list);
		যদি (get_octopus_merge_bases(list, &common) < 0) {
			মুক্ত(list);
			ret = 2;
			গচ্ছ done;
		}
		মুক্ত(list);
	}

	refs_update_ref(get_main_ref_store(the_repository),
			"updating ORIG_HEAD", "ORIG_HEAD",
			&head_commit->object.oid, রিক্ত, 0,
			UPDATE_REFS_DIE_ON_ERR);

	যদি (remoteheads && !common) {
		/* No common ancestors found. */
		যদি (!allow_unrelated_histories)
			die(_("refusing to merge unrelated histories"));
		/* otherwise, we need a real merge. */
	} অন্যথা যদি (!remoteheads ||
		 (!remoteheads->next && !common->next &&
		  common->item == remoteheads->item)) {
		/*
		 * If head can reach all the merge then we are up to date.
		 * but first the most common case of merging one remote.
		 */
		finish_up_to_date();
		গচ্ছ done;
	} অন্যথা যদি (fast_forward != FF_NO && !remoteheads->next &&
			!common->next &&
			oideq(&common->item->object.oid, &head_commit->object.oid)) {
		/* Again the most common case of merging one remote. */
		ধ্রুব বর্ণ *বার্তা = have_message ?
			"Fast-forward (no commit created; -m option ignored)" :
			"Fast-forward";
		সংরচনা commit *commit;

		যদি (verbosity >= 0) {
			printf(_("Updating %s..%s\n"),
			       repo_find_unique_abbrev(the_repository, &head_commit->object.oid,
						       DEFAULT_ABBREV),
			       repo_find_unique_abbrev(the_repository, &remoteheads->item->object.oid,
						       DEFAULT_ABBREV));
		}
		commit = remoteheads->item;
		যদি (!commit) {
			ret = 1;
			গচ্ছ done;
		}

		যদি (autostash)
			create_autostash_ref(the_repository, "MERGE_AUTOSTASH",
					     রিক্ত, false);
		যদি (checkout_fast_forward(the_repository,
					  &head_commit->object.oid,
					  &commit->object.oid,
					  overwrite_ignore)) {
			apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
					    রিক্ত, রিক্ত, রিক্ত, রিক্ত);
			ret = 1;
			গচ্ছ done;
		}

		finish(head_commit, remoteheads, &commit->object.oid, বার্তা);
		remove_merge_branch_state(the_repository);
		গচ্ছ done;
	} অন্যথা যদি (!remoteheads->next && common->next)
		;
		/*
		 * We are not doing octopus and not fast-forward.  Need
		 * a real merge.
		 */
	অন্যথা যদি (!remoteheads->next && !common->next && option_commit) {
		/*
		 * We are not doing octopus, not fast-forward, and have
		 * only one common.
		 */
		refresh_index(the_repository->index, REFRESH_QUIET, রিক্ত, রিক্ত, রিক্ত);
		যদি (allow_trivial && fast_forward != FF_ONLY) {
			/*
			 * Must first ensure that index matches HEAD before
			 * attempting a trivial merge.
			 */
			সংরচনা বৃক্ষ *head_বৃক্ষ = repo_get_commit_বৃক্ষ(the_repository,
								      head_commit);
			সংরচনা strbuf sb = STRBUF_INIT;

			যদি (repo_index_has_changes(the_repository, head_বৃক্ষ,
						   &sb)) {
				error(_("Your local changes to the following files would be overwritten by merge:\n  %s"),
				      sb.buf);
				strbuf_release(&sb);
				ret = 2;
				গচ্ছ done;
			}

			/* See if it is really trivial. */
			git_committer_info(IDENT_STRICT);
			printf(_("Trying really trivial in-index merge...\n"));
			যদি (!read_বৃক্ষ_trivial(&common->item->object.oid,
					       &head_commit->object.oid,
					       &remoteheads->item->object.oid)) {
				ret = merge_trivial(head_commit, remoteheads);
				গচ্ছ done;
			}
			printf(_("Nope.\n"));
		}
	} অন্যথা {
		/*
		 * An octopus.  If we can reach all the remote we are up
		 * to date.
		 */
		পূর্ণাঙ্ক up_to_date = 1;
		সংরচনা commit_list *j;

		প্রতি (j = remoteheads; j; j = j->next) {
			পূর্ণাঙ্ক ret = repo_in_merge_bases(the_repository,
						      j->item, head_commit);
			যদি (ret < 0)
				exit(128);
			যদি (!ret) {
				up_to_date = 0;
				ভঙ্গ;
			}
		}
		যদি (up_to_date) {
			finish_up_to_date();
			গচ্ছ done;
		}
	}

	যদি (fast_forward == FF_ONLY)
		die_ff_impossible();

	যদি (autostash)
		create_autostash_ref(the_repository, "MERGE_AUTOSTASH",
				     রিক্ত, false);

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
	যদি (save_state(&stash))
		oidclr(&stash, the_repository->hash_algo);

	প্রতি (i = 0; i < use_strategies_nr; i++) {
		পূর্ণাঙ্ক ret, cnt;
		যদি (i) {
			printf(_("Rewinding the tree to pristine...\n"));
			restore_state(&head_commit->object.oid, &stash);
		}
		যদি (use_strategies_nr != 1)
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
		যদি (ret < 2) {
			যদি (!ret) {
				/*
				 * This strategy worked; no point in trying
				 * another.
				 */
				merge_was_ok = 1;
				best_strategy = wt_strategy;
				ভঙ্গ;
			}
			cnt = (use_strategies_nr > 1) ? evaluate_result() : 0;
			যদি (best_cnt <= 0 || cnt <= best_cnt) {
				best_strategy = wt_strategy;
				best_cnt = cnt;
			}
		}
	}

	/*
	 * If we have a resulting tree, that means the strategy module
	 * auto resolved the merge cleanly.
	 */
	যদি (merge_was_ok && option_commit) {
		automerge_was_ok = 1;
		ret = finish_automerge(head_commit, head_subsumed,
				       common, remoteheads,
				       &result_বৃক্ষ, wt_strategy);
		গচ্ছ done;
	}

	/*
	 * Pick the result from the best strategy and have the user fix
	 * it up.
	 */
	যদি (!best_strategy) {
		restore_state(&head_commit->object.oid, &stash);
		যদি (use_strategies_nr > 1)
			fprintf(stderr,
				_("No merge strategy handled the merge.\n"));
		অন্যথা
			fprintf(stderr, _("Merge with strategy %s failed.\n"),
				use_strategies[0]->name);
		apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
				    রিক্ত, রিক্ত, রিক্ত, রিক্ত);
		ret = 2;
		গচ্ছ done;
	} অন্যথা যদি (best_strategy == wt_strategy)
		; /* We already have its result in the working tree. */
	অন্যথা {
		printf(_("Rewinding the tree to pristine...\n"));
		restore_state(&head_commit->object.oid, &stash);
		printf(_("Using the %s strategy to prepare resolving by hand.\n"),
			best_strategy);
		try_merge_strategy(best_strategy, common, remoteheads,
				   head_commit);
	}

	যদি (squash) {
		finish(head_commit, remoteheads, রিক্ত, রিক্ত);

		git_test_write_commit_graph_or_die(the_repository);
	} অন্যথা
		write_merge_state(remoteheads);

	যদি (merge_was_ok)
		fprintf(stderr, _("Automatic merge went well; "
			"stopped before committing as requested\n"));
	অন্যথা
		ret = suggest_conflicts();
	যদি (autostash)
		printf(_("When finished, apply stashed changes with `git stash pop`\n"));

done:
	যদি (!automerge_was_ok) {
		commit_list_free(common);
		commit_list_free(remoteheads);
	}
	strbuf_release(&buf);
	মুক্ত(branch_to_free);
	মুক্ত(argv_to_free);
	মুক্ত(pull_twohead);
	মুক্ত(pull_octopus);
	discard_index(the_repository->index);
	প্রত্যাগম ret;
}
