/*
 * Builtin "git merge"
 *
 * Copyright (c) 2008 Miklos Vajna <vmiklos@frugalware.org>
 *
 * Based on git-merge.sh by Junio C Hamano.
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

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

#define DEFAULT_TWOHEAD (1<<0)
#define DEFAULT_OCTOPUS (1<<1)
#define NO_FAST_FORWARD (1<<2)
#define NO_TRIVIAL      (1<<3)

ساختار strategy {
	ثابت نویسه *name;
	unsigned attr;
};

ایستا ثابت نویسه * ثابت builtin_merge_usage[] = {
	N_("git merge [<options>] [<commit>...]"),
	"git merge --abort",
	"git merge --continue",
	پوچ
};

#define MERGE_SHOW_DIFFSTAT 1
#define MERGE_SHOW_COMPACTSUMMARY 2

ایستا صحیح show_diffstat = MERGE_SHOW_DIFFSTAT, shortlog_len = -1, squash;
ایستا صحیح option_commit = -1;
ایستا صحیح option_edit = -1;
ایستا صحیح allow_trivial = 1, have_message, verify_signatures;
ایستا صحیح check_trust_level = 1;
ایستا صحیح overwrite_ignore = 1;
ایستا ساختار strbuf merge_پیام = STRBUF_INIT;
ایستا ساختار strategy **use_strategies;
ایستا size_t use_strategies_nr, use_strategies_alloc;
ایستا ساختار strvec xopts = STRVEC_INIT;
ایستا ثابت نویسه *branch;
ایستا نویسه *branch_mergeoptions;
ایستا صحیح verbosity;
ایستا صحیح allow_rerere_auto;
ایستا صحیح abort_current_merge;
ایستا صحیح quit_current_merge;
ایستا صحیح continue_current_merge;
ایستا صحیح allow_unrelated_histories;
ایستا صحیح show_progress = -1;
ایستا صحیح default_to_upstream = 1;
ایستا صحیح signoff;
ایستا ثابت نویسه *sign_commit;
ایستا صحیح autostash;
ایستا صحیح no_verify;
ایستا نویسه *into_name;

ایستا ساختار strategy all_strategy[] = {
	{ "recursive",  NO_TRIVIAL },
	{ "octopus",    DEFAULT_OCTOPUS },
	{ "ort",        DEFAULT_TWOHEAD | NO_TRIVIAL },
	{ "resolve",    0 },
	{ "ours",       NO_FAST_FORWARD | NO_TRIVIAL },
	{ "subtree",    NO_FAST_FORWARD | NO_TRIVIAL },
};

ایستا نویسه *pull_twohead, *pull_octopus;

شمارش ff_type {
	FF_NO,
	FF_ALLOW,
	FF_ONLY
};

ایستا شمارش ff_type fast_forward = FF_ALLOW;

ایستا نویسه *cleanup_arg;
ایستا شمارش commit_پیام_cleanup_mode cleanup_mode;

ایستا صحیح option_parse_message(ثابت ساختار option *opt,
				ثابت نویسه *arg, صحیح unset)
{
	ساختار strbuf *buf = opt->مقدار;

	اگر (unset)
		strbuf_setlen(buf, 0);
	وگرنه اگر (arg) {
		strbuf_addf(buf, "%s%s", buf->len ? "\n\n" : "", arg);
		have_message = 1;
	} وگرنه
		بازگشت error(_("switch `m' requires a value"));
	بازگشت 0;
}

ایستا شمارش parse_opt_result option_read_message(ساختار parse_opt_ctx_t *ctx,
						 ثابت ساختار option *opt,
						 ثابت نویسه *arg_not_used,
						 صحیح unset)
{
	ساختار strbuf *buf = opt->مقدار;
	ثابت نویسه *arg;

	BUG_ON_OPT_ARG(arg_not_used);
	اگر (unset)
		BUG("-F cannot be negated");

	اگر (ctx->opt) {
		arg = ctx->opt;
		ctx->opt = پوچ;
	} وگرنه اگر (ctx->argc > 1) {
		ctx->argc--;
		arg = *++ctx->argv;
	} وگرنه
		بازگشت error(_("option `%s' requires a value"), opt->long_name);

	اگر (buf->len)
		strbuf_addch(buf, '\n');
	اگر (ctx->prefix && !is_absolute_path(arg))
		arg = prefix_filename(ctx->prefix, arg);
	اگر (strbuf_read_file(buf, arg, 0) < 0)
		بازگشت error(_("could not read file '%s'"), arg);
	have_message = 1;

	بازگشت 0;
}

ایستا ساختار strategy *get_strategy(ثابت نویسه *name)
{
	صحیح i;
	ساختار strategy *ret;
	ایستا ساختار cmdnames main_cmds = {0}, other_cmds = {0};
	ایستا صحیح loaded;
	نویسه *default_strategy = پوچ;

	اگر (!name)
		بازگشت پوچ;

	اگر (default_strategy &&
	    !strcmp(default_strategy, "ort") &&
	    !strcmp(name, "recursive")) {
		name = "ort";
	}

	برای (i = 0; i < ARRAY_SIZE(all_strategy); i++)
		اگر (!strcmp(name, all_strategy[i].name))
			بازگشت &all_strategy[i];

	اگر (!loaded) {
		ساختار cmdnames not_strategies = {0};
		loaded = 1;

		load_command_list("git-merge-", &main_cmds, &other_cmds);
		برای (i = 0; i < main_cmds.cnt; i++) {
			صحیح j, found = 0;
			ساختار cmdname *ent = main_cmds.names[i];
			برای (j = 0; !found && j < ARRAY_SIZE(all_strategy); j++)
				اگر (!xstrncmpz(all_strategy[j].name, ent->name, ent->len))
					found = 1;
			اگر (!found)
				add_cmdname(&not_strategies, ent->name, ent->len);
		}
		exclude_cmds(&main_cmds, &not_strategies);

		cmdnames_release(&not_strategies);
	}
	اگر (!is_in_cmdlist(&main_cmds, name) && !is_in_cmdlist(&other_cmds, name)) {
		fprintf(stderr, _("Could not find merge strategy '%s'.\n"), name);
		fprintf(stderr, _("Available strategies are:"));
		برای (i = 0; i < main_cmds.cnt; i++)
			fprintf(stderr, " %s", main_cmds.names[i]->name);
		fprintf(stderr, ".\n");
		اگر (other_cmds.cnt) {
			fprintf(stderr, _("Available custom strategies are:"));
			برای (i = 0; i < other_cmds.cnt; i++)
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
	بازگشت ret;
}

ایستا تهی append_strategy(ساختار strategy *s)
{
	ALLOC_GROW(use_strategies, use_strategies_nr + 1, use_strategies_alloc);
	use_strategies[use_strategies_nr++] = s;
}

ایستا صحیح option_parse_strategy(ثابت ساختار option *opt UNUSED,
				 ثابت نویسه *name, صحیح unset)
{
	اگر (unset)
		بازگشت 0;

	append_strategy(get_strategy(name));
	بازگشت 0;
}

ایستا صحیح option_parse_compact_summary(ثابت ساختار option *opt,
					ثابت نویسه *name UNUSED, صحیح unset)
{
	صحیح *setting = opt->مقدار;

	اگر (unset)
		*setting = 0;
	وگرنه
		*setting = MERGE_SHOW_COMPACTSUMMARY;
	بازگشت 0;
}

ایستا ساختار option builtin_merge_options[] = {
	OPT_SET_INT('n', پوچ, &show_diffstat,
		N_("do not show a diffstat at the end of the merge"), 0),
	OPT_BOOL(0, "stat", &show_diffstat,
		N_("show a diffstat at the end of the merge")),
	OPT_BOOL(0, "summary", &show_diffstat, N_("(synonym to --stat)")),
	OPT_CALLBACK_F(0, "compact-summary", &show_diffstat, پوچ,
		       N_("show a compact-summary at the end of the merge"),
		       PARSE_OPT_NOARG,
		       option_parse_compact_summary),
	{
		.type = OPTION_INTEGER,
		.long_name = "log",
		.مقدار = &shortlog_len,
		.precision = اندازه(shortlog_len),
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
	OPT_CALLBACK('s', "strategy", پوچ, N_("strategy"),
		N_("merge strategy to use"), option_parse_strategy),
	OPT_STRVEC('X', "strategy-option", &xopts, N_("option=value"),
		N_("option for selected merge strategy")),
	OPT_CALLBACK('m', "message", &merge_پیام, N_("message"),
		N_("merge commit message (for a non-fast-forward merge)"),
		option_parse_message),
	{
		.type = OPTION_LOWLEVEL_CALLBACK,
		.short_name = 'F',
		.long_name = "file",
		.مقدار = &merge_پیام,
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
		.مقدار = &sign_commit,
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

ایستا صحیح save_state(ساختار object_id *stash)
{
	صحیح len;
	ساختار child_process cp = CHILD_PROCESS_INIT;
	ساختار strbuf buffer = STRBUF_INIT;
	ساختار lock_file lock_file = LOCK_INIT;
	صحیح fd;
	صحیح rc = -1;

	fd = repo_hold_locked_index(the_repository, &lock_file, 0);
	refresh_index(the_repository->index, REFRESH_QUIET, پوچ, پوچ, پوچ);
	اگر (0 <= fd)
		repo_update_index_if_able(the_repository, &lock_file);
	rollback_lock_file(&lock_file);

	strvec_pushl(&cp.args, "stash", "create", پوچ);
	cp.out = -1;
	cp.git_cmd = 1;

	اگر (start_command(&cp))
		die(_("could not run stash."));
	len = strbuf_read(&buffer, cp.out, 1024);
	close(cp.out);

	اگر (finish_command(&cp) || len < 0)
		die(_("stash failed"));
	وگرنه اگر (!len)		/* no changes */
		برو out;
	strbuf_setlen(&buffer, buffer.len-1);
	اگر (repo_get_oid(the_repository, buffer.buf, stash))
		die(_("not a valid object: %s"), buffer.buf);
	rc = 0;
out:
	strbuf_release(&buffer);
	بازگشت rc;
}

ایستا تهی read_empty(ثابت ساختار object_id *oid)
{
	ساختار child_process cmd = CHILD_PROCESS_INIT;

	strvec_pushl(&cmd.args, "read-tree", "-m", "-u",
		     empty_درخت_oid_hex(the_repository->hash_algo),
		     oid_to_hex(oid), پوچ);
	cmd.git_cmd = 1;

	اگر (run_command(&cmd))
		die(_("read-tree failed"));
}

ایستا تهی reset_hard(ثابت ساختار object_id *oid)
{
	ساختار child_process cmd = CHILD_PROCESS_INIT;

	strvec_pushl(&cmd.args, "read-tree", "-v", "--reset", "-u",
		     oid_to_hex(oid), پوچ);
	cmd.git_cmd = 1;

	اگر (run_command(&cmd))
		die(_("read-tree failed"));
}

ایستا تهی restore_state(ثابت ساختار object_id *head,
			  ثابت ساختار object_id *stash)
{
	ساختار child_process cmd = CHILD_PROCESS_INIT;

	reset_hard(head);

	اگر (is_null_oid(stash))
		برو refresh_cache;

	strvec_pushl(&cmd.args, "stash", "apply", "--index", "--quiet", پوچ);
	strvec_افزودن(&cmd.args, oid_to_hex(stash));

	/*
	 * It is OK to ignore error here, for example when there was
	 * nothing to restore.
	 */
	cmd.git_cmd = 1;
	run_command(&cmd);

refresh_cache:
	discard_index(the_repository->index);
	اگر (repo_read_index(the_repository) < 0)
		die(_("could not read index"));
}

/* This is called when no merge was necessary. */
ایستا تهی finish_up_to_date(تهی)
{
	اگر (verbosity >= 0) {
		اگر (squash)
			puts(_("Already up to date. (nothing to squash)"));
		وگرنه
			puts(_("Already up to date."));
	}
	remove_merge_branch_state(the_repository);
}

ایستا تهی squash_message(ساختار commit *commit, ساختار commit_list *remoteheads)
{
	ساختار rev_info rev;
	ساختار strbuf out = STRBUF_INIT;
	ساختار commit_list *j;
	ساختار pretty_print_context ctx = {0};

	printf(_("Squash commit -- not updating HEAD\n"));

	repo_init_revisions(the_repository, &rev, پوچ);
	diff_merges_suppress(&rev);
	rev.commit_format = CMIT_FMT_MEDIUM;

	commit->object.flags |= UNINTERESTING;
	add_pending_object(&rev, &commit->object, پوچ);

	برای (j = remoteheads; j; j = j->next)
		add_pending_object(&rev, &j->item->object, پوچ);

	setup_revisions(0, پوچ, &rev, پوچ);
	اگر (prepare_revision_walk(&rev))
		die(_("revision walk setup failed"));

	ctx.abbrev = rev.abbrev;
	ctx.date_mode = rev.date_mode;
	ctx.fmt = rev.commit_format;

	strbuf_addstr(&out, "Squashed commit of the following:\n");
	وقتی ((commit = get_revision(&rev)) != پوچ) {
		strbuf_addch(&out, '\n');
		strbuf_addf(&out, "commit %s\n",
			oid_to_hex(&commit->object.oid));
		pretty_print_commit(&ctx, commit, &out);
	}
	write_file_buf(git_path_squash_پیام(the_repository), out.buf, out.len);
	strbuf_release(&out);
	release_revisions(&rev);
}

ایستا تهی finish(ساختار commit *head_commit,
		   ساختار commit_list *remoteheads,
		   ثابت ساختار object_id *new_head, ثابت نویسه *پیام)
{
	ساختار strbuf reflog_message = STRBUF_INIT;
	ثابت ساختار object_id *head = &head_commit->object.oid;

	اگر (!پیام)
		strbuf_addstr(&reflog_message, getenv("GIT_REFLOG_ACTION"));
	وگرنه {
		اگر (verbosity >= 0)
			printf("%s\n", پیام);
		strbuf_addf(&reflog_message, "%s: %s",
			getenv("GIT_REFLOG_ACTION"), پیام);
	}
	اگر (squash) {
		squash_message(head_commit, remoteheads);
	} وگرنه {
		اگر (verbosity >= 0 && !merge_پیام.len)
			printf(_("No merge message -- not updating HEAD\n"));
		وگرنه {
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
	اگر (new_head && show_diffstat) {
		ساختار diff_options opts;
		repo_diff_setup(the_repository, &opts);
		init_diffstat_widths(&opts);

		انتخاب (show_diffstat) {
		حالت MERGE_SHOW_DIFFSTAT: /* 1 */
			opts.output_format |=
				DIFF_FORMAT_SUMMARY | DIFF_FORMAT_DIFFSTAT;
			بشکن;
		حالت MERGE_SHOW_COMPACTSUMMARY: /* 2 */
			opts.output_format |= DIFF_FORMAT_DIFFSTAT;
			opts.flags.stat_with_summary = 1;
			بشکن;
		پیشفرض:
			بشکن;
		}
		opts.detect_rename = DIFF_DETECT_RENAME;
		diff_setup_done(&opts);
		diff_درخت_oid(head, new_head, "", &opts);
		diffcore_std(&opts);
		diff_flush(&opts);
	}

	/* Run a post-merge hook */
	run_hooks_l(the_repository, "post-merge", squash ? "1" : "0", پوچ);

	اگر (new_head)
		apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
				    پوچ, پوچ, پوچ, پوچ);
	strbuf_release(&reflog_message);
}

/* Get the name for the merge commit's message. */
ایستا تهی merge_name(ثابت نویسه *remote, ساختار strbuf *پیام)
{
	ساختار commit *remote_head;
	ساختار object_id branch_head;
	ساختار strbuf bname = STRBUF_INIT;
	ساختار merge_remote_desc *desc;
	ثابت نویسه *ptr;
	نویسه *found_ref = پوچ;
	صحیح len, early;

	copy_branchname(the_repository, &bname, remote, 0);
	remote = bname.buf;

	oidclr(&branch_head, the_repository->hash_algo);
	remote_head = get_merge_parent(remote);
	اگر (!remote_head)
		die(_("'%s' does not point to a commit"), remote);

	اگر (repo_dwim_ref(the_repository, remote, strlen(remote), &branch_head,
			  &found_ref, 0) > 0) {
		اگر (starts_with(found_ref, "refs/heads/")) {
			strbuf_addf(پیام, "%s\t\tbranch '%s' of .\n",
				    oid_to_hex(&branch_head), remote);
			برو cleanup;
		}
		اگر (starts_with(found_ref, "refs/tags/")) {
			strbuf_addf(پیام, "%s\t\ttag '%s' of .\n",
				    oid_to_hex(&branch_head), remote);
			برو cleanup;
		}
		اگر (starts_with(found_ref, "refs/remotes/")) {
			strbuf_addf(پیام, "%s\t\tremote-tracking branch '%s' of .\n",
				    oid_to_hex(&branch_head), remote);
			برو cleanup;
		}
	}

	/* See if remote matches <name>^^^.. or <name>~<number> */
	برای (len = 0, ptr = remote + strlen(remote);
	     remote < ptr && ptr[-1] == '^';
	     ptr--)
		len++;
	اگر (len)
		early = 1;
	وگرنه {
		early = 0;
		ptr = strrchr(remote, '~');
		اگر (ptr) {
			صحیح seen_nonzero = 0;

			len++; /* count ~ */
			وقتی (*++ptr && isdigit(*ptr)) {
				seen_nonzero |= (*ptr != '0');
				len++;
			}
			اگر (*ptr)
				len = 0; /* not ...~<number> */
			وگرنه اگر (seen_nonzero)
				early = 1;
			وگرنه اگر (len == 1)
				early = 1; /* "name~" is "name~1"! */
		}
	}
	اگر (len) {
		ساختار strbuf truname = STRBUF_INIT;
		strbuf_addf(&truname, "refs/heads/%s", remote);
		strbuf_setlen(&truname, truname.len - len);
		اگر (refs_ref_exists(get_main_ref_store(the_repository), truname.buf)) {
			strbuf_addf(پیام,
				    "%s\t\tbranch '%s'%s of .\n",
				    oid_to_hex(&remote_head->object.oid),
				    truname.buf + 11,
				    (early ? " (early part)" : ""));
			strbuf_release(&truname);
			برو cleanup;
		}
		strbuf_release(&truname);
	}

	desc = merge_remote_util(remote_head);
	اگر (desc && desc->obj && desc->obj->type == OBJ_TAG) {
		strbuf_addf(پیام, "%s\t\t%s '%s'\n",
			    oid_to_hex(&desc->obj->oid),
			    type_name(desc->obj->type),
			    remote);
		برو cleanup;
	}

	strbuf_addf(پیام, "%s\t\tcommit '%s'\n",
		oid_to_hex(&remote_head->object.oid), remote);
cleanup:
	آزاد(found_ref);
	strbuf_release(&bname);
}

ایستا تهی parse_branch_merge_options(نویسه *bmo)
{
	ثابت نویسه **argv;
	صحیح argc;

	اگر (!bmo)
		بازگشت;
	argc = split_cmdline(bmo, &argv);
	اگر (argc < 0)
		die(_("Bad branch.%s.mergeoptions string: %s"), branch,
		    _(split_cmdline_strerror(argc)));
	REALLOC_ARRAY(argv, argc + 2);
	MOVE_ARRAY(argv + 1, argv, argc + 1);
	argc++;
	argv[0] = "branch.*.mergeoptions";
	parse_options(argc, argv, پوچ, builtin_merge_options,
		      builtin_merge_usage, 0);
	آزاد(argv);
}

ایستا صحیح git_merge_config(ثابت نویسه *k, ثابت نویسه *v,
			    ثابت ساختار config_context *ctx, تهی *cb)
{
	صحیح status;
	ثابت نویسه *str;

	اگر (branch &&
	    skip_prefix(k, "branch.", &str) &&
	    skip_prefix(str, branch, &str) &&
	    !strcmp(str, ".mergeoptions")) {
		آزاد(branch_mergeoptions);
		branch_mergeoptions = xstrdup(v);
		بازگشت 0;
	}

	اگر (!strcmp(k, "merge.diffstat") || !strcmp(k, "merge.stat")) {
		صحیح val = git_parse_maybe_bool_text(v);
		انتخاب (val) {
		حالت 0:
			show_diffstat = 0;
			بشکن;
		حالت 1:
			show_diffstat = MERGE_SHOW_DIFFSTAT;
			بشکن;
		پیشفرض:
			اگر (!strcmp(v, "compact"))
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
			وگرنه
				/*
				 * A setting from a future?  It is not an
				 * error grave enough to fail the command.
				 * proceed using the default one.
				 */
				show_diffstat = MERGE_SHOW_DIFFSTAT;
			بشکن;
		}
	} وگرنه اگر (!strcmp(k, "merge.verifysignatures")) {
		verify_signatures = git_config_bool(k, v);
	} وگرنه اگر (!strcmp(k, "pull.twohead")) {
		FREE_AND_NULL(pull_twohead);
		بازگشت git_config_string(&pull_twohead, k, v);
	} وگرنه اگر (!strcmp(k, "pull.octopus")) {
		FREE_AND_NULL(pull_octopus);
		بازگشت git_config_string(&pull_octopus, k, v);
	} وگرنه اگر (!strcmp(k, "commit.cleanup")) {
		بازگشت git_config_string(&cleanup_arg, k, v);
	} وگرنه اگر (!strcmp(k, "merge.ff")) {
		صحیح boolval = git_parse_maybe_bool(v);
		اگر (0 <= boolval) {
			fast_forward = boolval ? FF_ALLOW : FF_NO;
		} وگرنه اگر (v && !strcmp(v, "only")) {
			fast_forward = FF_ONLY;
		} /* do not barf on values from future versions of git */
		بازگشت 0;
	} وگرنه اگر (!strcmp(k, "merge.defaulttoupstream")) {
		default_to_upstream = git_config_bool(k, v);
		بازگشت 0;
	} وگرنه اگر (!strcmp(k, "commit.gpgsign")) {
		sign_commit = git_config_bool(k, v) ? "" : پوچ;
		بازگشت 0;
	} وگرنه اگر (!strcmp(k, "gpg.mintrustlevel")) {
		check_trust_level = 0;
	} وگرنه اگر (!strcmp(k, "merge.autostash")) {
		autostash = git_config_bool(k, v);
		بازگشت 0;
	}

	status = fmt_merge_پیام_config(k, v, ctx, cb);
	اگر (status)
		بازگشت status;
	بازگشت git_diff_ui_config(k, v, ctx, cb);
}

ایستا صحیح read_درخت_trivial(ساختار object_id *common, ساختار object_id *head,
			     ساختار object_id *one)
{
	صحیح i, nr_trees = 0;
	ساختار درخت *trees[MAX_UNPACK_TREES];
	ساختار درخت_desc t[MAX_UNPACK_TREES];
	ساختار unpack_trees_options opts;

	memset(&opts, 0, اندازه(opts));
	opts.head_idx = 2;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;
	opts.update = 1;
	opts.verbose_update = 1;
	opts.trivial_merges_only = 1;
	opts.merge = 1;
	opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
	trees[nr_trees] = repo_parse_درخت_indirect(the_repository, common);
	اگر (!trees[nr_trees++])
		بازگشت -1;
	trees[nr_trees] = repo_parse_درخت_indirect(the_repository, head);
	اگر (!trees[nr_trees++])
		بازگشت -1;
	trees[nr_trees] = repo_parse_درخت_indirect(the_repository, one);
	اگر (!trees[nr_trees++])
		بازگشت -1;
	opts.fn = threeway_merge;
	cache_درخت_free(&the_repository->index->cache_درخت);
	برای (i = 0; i < nr_trees; i++) {
		repo_parse_درخت(the_repository, trees[i]);
		init_درخت_desc(t+i, &trees[i]->object.oid,
			       trees[i]->buffer, trees[i]->size);
	}
	اگر (unpack_trees(nr_trees, t, &opts))
		بازگشت -1;
	بازگشت 0;
}

ایستا تهی write_درخت_trivial(ساختار object_id *oid)
{
	اگر (write_index_as_درخت(oid, the_repository->index,
				repo_get_index_file(the_repository),
				0, پوچ))
		die(_("git write-tree failed to write a tree"));
}

ایستا صحیح try_merge_strategy(ثابت نویسه *strategy, ساختار commit_list *common,
			      ساختار commit_list *remoteheads,
			      ساختار commit *head)
{
	ثابت نویسه *head_arg = "HEAD";

	اگر (repo_refresh_and_write_index(the_repository, REFRESH_QUIET,
					 SKIP_IF_UNCHANGED, 0, پوچ, پوچ,
					 پوچ) < 0)
		die(_("Unable to write index."));

	اگر (!strcmp(strategy, "recursive") || !strcmp(strategy, "subtree") ||
	    !strcmp(strategy, "ort")) {
		ساختار lock_file lock = LOCK_INIT;
		صحیح clean, x;
		ساختار commit *result;
		ساختار commit_list *reversed = پوچ;
		ساختار merge_options o;
		ساختار commit_list *j;

		اگر (remoteheads->next) {
			error(_("Not handling anything other than two heads merge."));
			بازگشت 2;
		}

		init_ui_merge_options(&o, the_repository);
		اگر (!strcmp(strategy, "subtree"))
			o.subtree_shift = "";

		o.show_rename_progress =
			show_progress == -1 ? isatty(2) : show_progress;

		برای (x = 0; x < xopts.nr; x++)
			اگر (parse_merge_opt(&o, xopts.v[x]))
				die(_("unknown strategy option: -X%s"), xopts.v[x]);

		o.branch1 = head_arg;
		o.branch2 = merge_remote_util(remoteheads->item)->name;

		برای (j = common; j; j = j->next)
			commit_list_درج(j->item, &reversed);

		repo_hold_locked_index(the_repository, &lock,
				       LOCK_DIE_ON_ERROR);
		clean = merge_ort_recursive(&o, head, remoteheads->item,
					    reversed, &result);
		commit_list_free(reversed);
		strbuf_release(&o.obuf);

		اگر (clean < 0) {
			rollback_lock_file(&lock);
			بازگشت 2;
		}
		اگر (write_locked_index(the_repository->index, &lock,
				       COMMIT_LOCK | SKIP_IF_UNCHANGED))
			die(_("unable to write %s"), repo_get_index_file(the_repository));
		بازگشت clean ? 0 : 1;
	} وگرنه {
		بازگشت try_merge_command(the_repository,
					 strategy, xopts.nr, xopts.v,
					 common, head_arg, remoteheads);
	}
}

ایستا تهی شمار_diff_files(ساختار diff_queue_struct *q,
			     ساختار diff_options *opt UNUSED, تهی *data)
{
	صحیح *شمار = data;

	(*شمار) += q->nr;
}

ایستا صحیح شمار_unmerged_entries(تهی)
{
	صحیح i, ret = 0;

	برای (i = 0; i < the_repository->index->cache_nr; i++)
		اگر (ce_stage(the_repository->index->cache[i]))
			ret++;

	بازگشت ret;
}

ایستا تهی add_strategies(ثابت نویسه *string, unsigned attr)
{
	صحیح i;

	اگر (string) {
		ساختار string_list list = STRING_LIST_INIT_DUP;
		ساختار string_list_item *item;
		string_list_split(&list, string, " ", -1);
		for_each_string_list_item(item, &list)
			append_strategy(get_strategy(item->string));
		string_list_clear(&list, 0);
		بازگشت;
	}
	برای (i = 0; i < ARRAY_SIZE(all_strategy); i++)
		اگر (all_strategy[i].attr & attr)
			append_strategy(&all_strategy[i]);

}

ایستا تهی read_merge_پیام(ساختار strbuf *پیام)
{
	ثابت نویسه *filename = git_path_merge_پیام(the_repository);
	strbuf_reset(پیام);
	اگر (strbuf_read_file(پیام, filename, 0) < 0)
		die_errno(_("Could not read from '%s'"), filename);
}

ایستا تهی write_merge_state(ساختار commit_list *);
ایستا تهی abort_commit(ساختار commit_list *remoteheads, ثابت نویسه *err_پیام)
{
	اگر (err_پیام)
		error("%s", err_پیام);
	fprintf(stderr,
		_("Not committing merge; use 'git commit' to complete the merge.\n"));
	write_merge_state(remoteheads);
	exit(1);
}

ایستا ثابت نویسه merge_editor_comment[] =
N_("Please enter a commit message to explain why this merge is necessary,\n"
   "especially if it merges an updated upstream into a topic branch.\n"
   "\n");

ایستا ثابت نویسه scissors_editor_comment[] =
N_("An empty message aborts the commit.\n");

ایستا ثابت نویسه no_scissors_editor_comment[] =
N_("Lines starting with '%s' will be ignored, and an empty message aborts\n"
   "the commit.\n");

ایستا تهی write_merge_heads(ساختار commit_list *);
ایستا تهی prepare_to_commit(ساختار commit_list *remoteheads)
{
	ساختار strbuf پیام = STRBUF_INIT;
	ثابت نویسه *index_file = repo_get_index_file(the_repository);

	اگر (!no_verify) {
		صحیح invoked_hook;

		اگر (run_commit_hook(0 < option_edit, index_file, &invoked_hook,
				    "pre-merge-commit", پوچ))
			abort_commit(remoteheads, پوچ);
		/*
		 * Re-read the index as pre-merge-commit hook could have updated it,
		 * and write it out as a tree.  We must do this before we invoke
		 * the editor and after we invoke run_status above.
		 */
		اگر (invoked_hook)
			discard_index(the_repository->index);
	}
	read_index_from(the_repository->index, index_file,
			repo_get_git_dir(the_repository));
	strbuf_addbuf(&پیام, &merge_پیام);
	اگر (squash)
		BUG("the control must not reach here under --squash");
	اگر (0 < option_edit) {
		strbuf_addch(&پیام, '\n');
		اگر (cleanup_mode == COMMIT_پیام_CLEANUP_SCISSORS) {
			wt_status_append_cut_سطر(&پیام);
			strbuf_commented_addf(&پیام, comment_سطر_str, "\n");
		}
		strbuf_commented_addf(&پیام, comment_سطر_str,
				      _(merge_editor_comment));
		اگر (cleanup_mode == COMMIT_پیام_CLEANUP_SCISSORS)
			strbuf_commented_addf(&پیام, comment_سطر_str,
					      _(scissors_editor_comment));
		وگرنه
			strbuf_commented_addf(&پیام, comment_سطر_str,
				_(no_scissors_editor_comment), comment_سطر_str);
	}
	اگر (signoff)
		append_signoff(&پیام, ignored_log_message_bytes(پیام.buf, پیام.len), 0);
	write_merge_heads(remoteheads);
	write_file_buf(git_path_merge_پیام(the_repository), پیام.buf, پیام.len);
	اگر (run_commit_hook(0 < option_edit, repo_get_index_file(the_repository),
			    پوچ, "prepare-commit-msg",
			    git_path_merge_پیام(the_repository), "merge", پوچ))
		abort_commit(remoteheads, پوچ);
	اگر (0 < option_edit) {
		اگر (launch_editor(git_path_merge_پیام(the_repository), پوچ, پوچ))
			abort_commit(remoteheads, پوچ);
	}

	اگر (!no_verify && run_commit_hook(0 < option_edit, repo_get_index_file(the_repository),
					  پوچ, "commit-msg",
					  git_path_merge_پیام(the_repository), پوچ))
		abort_commit(remoteheads, پوچ);

	read_merge_پیام(&پیام);
	cleanup_message(&پیام, cleanup_mode, 0);
	اگر (!پیام.len)
		abort_commit(remoteheads, _("Empty commit message."));
	strbuf_release(&merge_پیام);
	strbuf_addbuf(&merge_پیام, &پیام);
	strbuf_release(&پیام);
}

ایستا صحیح merge_trivial(ساختار commit *head, ساختار commit_list *remoteheads)
{
	ساختار object_id result_درخت, result_commit;
	ساختار commit_list *parents = پوچ, **pptr = &parents;

	اگر (repo_refresh_and_write_index(the_repository, REFRESH_QUIET,
					 SKIP_IF_UNCHANGED, 0, پوچ, پوچ,
					 پوچ) < 0)
		بازگشت error(_("Unable to write index."));

	write_درخت_trivial(&result_درخت);
	printf(_("Wonderful.\n"));
	pptr = commit_list_append(head, pptr);
	pptr = commit_list_append(remoteheads->item, pptr);
	prepare_to_commit(remoteheads);
	اگر (commit_درخت(merge_پیام.buf, merge_پیام.len, &result_درخت, parents,
			&result_commit, پوچ, sign_commit))
		die(_("failed to write commit object"));
	finish(head, remoteheads, &result_commit, "In-index merge");

	remove_merge_branch_state(the_repository);
	commit_list_free(parents);
	بازگشت 0;
}

ایستا صحیح finish_automerge(ساختار commit *head,
			    صحیح head_subsumed,
			    ساختار commit_list *common,
			    ساختار commit_list *remoteheads,
			    ساختار object_id *result_درخت,
			    ثابت نویسه *wt_strategy)
{
	ساختار commit_list *parents = پوچ;
	ساختار strbuf buf = STRBUF_INIT;
	ساختار object_id result_commit;

	write_درخت_trivial(result_درخت);
	commit_list_free(common);
	parents = remoteheads;
	اگر (!head_subsumed || fast_forward == FF_NO)
		commit_list_درج(head, &parents);
	prepare_to_commit(remoteheads);
	اگر (commit_درخت(merge_پیام.buf, merge_پیام.len, result_درخت, parents,
			&result_commit, پوچ, sign_commit))
		die(_("failed to write commit object"));
	strbuf_addf(&buf, "Merge made by the '%s' strategy.", wt_strategy);
	finish(head, remoteheads, &result_commit, buf.buf);

	strbuf_release(&buf);
	remove_merge_branch_state(the_repository);
	commit_list_free(parents);
	بازگشت 0;
}

ایستا صحیح suggest_conflicts(تهی)
{
	ثابت نویسه *filename;
	FILE *fp;
	ساختار strbuf msgbuf = STRBUF_INIT;

	filename = git_path_merge_پیام(the_repository);
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
	بازگشت 1;
}

ایستا صحیح evaluate_result(تهی)
{
	صحیح cnt = 0;
	ساختار rev_info rev;

	/* Check how many files differ. */
	repo_init_revisions(the_repository, &rev, "");
	setup_revisions(0, پوچ, &rev, پوچ);
	rev.diffopt.output_format |=
		DIFF_FORMAT_CALLBACK;
	rev.diffopt.format_callback = شمار_diff_files;
	rev.diffopt.format_callback_data = &cnt;
	run_diff_files(&rev, 0);

	/*
	 * Check how many unmerged entries are
	 * there.
	 */
	cnt += شمار_unmerged_entries();

	release_revisions(&rev);
	بازگشت cnt;
}

/*
 * Pretend as if the user told us to merge with the remote-tracking
 * branch we have for the upstream of the current branch
 */
ایستا صحیح setup_with_upstream(ثابت نویسه ***argv)
{
	ساختار branch *branch = branch_get(پوچ);
	صحیح i;
	ثابت نویسه **args;

	اگر (!branch)
		die(_("No current branch."));
	اگر (!branch->remote_name)
		die(_("No remote for the current branch."));
	اگر (!branch->merge_nr)
		die(_("No default upstream defined for the current branch."));

	args = xcalloc(st_add(branch->merge_nr, 1), اندازه(نویسه *));
	برای (i = 0; i < branch->merge_nr; i++) {
		اگر (!branch->merge[i]->dst)
			die(_("No remote-tracking branch for %s from %s"),
			    branch->merge[i]->src, branch->remote_name);
		args[i] = branch->merge[i]->dst;
	}
	args[i] = پوچ;
	*argv = args;
	بازگشت i;
}

ایستا تهی write_merge_heads(ساختار commit_list *remoteheads)
{
	ساختار commit_list *j;
	ساختار strbuf buf = STRBUF_INIT;

	برای (j = remoteheads; j; j = j->next) {
		ساختار object_id *oid;
		ساختار commit *c = j->item;
		ساختار merge_remote_desc *desc;

		desc = merge_remote_util(c);
		اگر (desc && desc->obj) {
			oid = &desc->obj->oid;
		} وگرنه {
			oid = &c->object.oid;
		}
		strbuf_addf(&buf, "%s\n", oid_to_hex(oid));
	}
	write_file_buf(git_path_merge_head(the_repository), buf.buf, buf.len);

	strbuf_reset(&buf);
	اگر (fast_forward == FF_NO)
		strbuf_addstr(&buf, "no-ff");
	write_file_buf(git_path_merge_mode(the_repository), buf.buf, buf.len);
	strbuf_release(&buf);
}

ایستا تهی write_merge_state(ساختار commit_list *remoteheads)
{
	write_merge_heads(remoteheads);
	strbuf_addch(&merge_پیام, '\n');
	write_file_buf(git_path_merge_پیام(the_repository), merge_پیام.buf,
		       merge_پیام.len);
}

ایستا صحیح default_edit_option(تهی)
{
	ایستا ثابت نویسه name[] = "GIT_MERGE_AUTOEDIT";
	ثابت نویسه *e = getenv(name);
	ساختار stat st_stdin, st_stdout;

	اگر (have_message)
		/* an explicit -m msg without --[no-]edit */
		بازگشت 0;

	اگر (e) {
		صحیح v = git_parse_maybe_bool(e);
		اگر (v < 0)
			die(_("Bad value '%s' in environment '%s'"), e, name);
		بازگشت v;
	}

	/* Use editor if stdin and stdout are the same and is a tty */
	بازگشت (!fstat(0, &st_stdin) &&
		!fstat(1, &st_stdout) &&
		isatty(0) && isatty(1) &&
		st_stdin.st_dev == st_stdout.st_dev &&
		st_stdin.st_ino == st_stdout.st_ino &&
		st_stdin.st_mode == st_stdout.st_mode);
}

ایستا ساختار commit_list *reduce_parents(ساختار commit *head_commit,
					  صحیح *head_subsumed,
					  ساختار commit_list *remoteheads)
{
	ساختار commit_list *parents, **remotes;

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

	remoteheads = پوچ;
	remotes = &remoteheads;
	وقتی (parents) {
		ساختار commit *commit = برداشت_commit(&parents);
		اگر (commit == head_commit)
			*head_subsumed = 0;
		وگرنه
			remotes = &commit_list_درج(commit, remotes)->next;
	}
	بازگشت remoteheads;
}

ایستا تهی prepare_merge_message(ساختار strbuf *merge_names, ساختار strbuf *merge_پیام)
{
	ساختار fmt_merge_پیام_opts opts;

	memset(&opts, 0, اندازه(opts));
	opts.add_title = !have_message;
	opts.shortlog_len = shortlog_len;
	opts.credit_people = (0 < option_edit);
	opts.into_name = into_name;

	fmt_merge_پیام(merge_names, merge_پیام, &opts);
	اگر (merge_پیام->len)
		strbuf_setlen(merge_پیام, merge_پیام->len - 1);
}

ایستا تهی handle_fetch_head(ساختار commit_list **remotes, ساختار strbuf *merge_names)
{
	ثابت نویسه *filename;
	صحیح fd, pos, npos;
	ساختار strbuf fetch_head_file = STRBUF_INIT;
	ثابت unsigned hexsz = the_hash_algo->hexsz;

	اگر (!merge_names)
		merge_names = &fetch_head_file;

	filename = git_path_fetch_head(the_repository);
	fd = xopen(filename, O_RDONLY);

	اگر (strbuf_read(merge_names, fd, 0) < 0)
		die_errno(_("could not read '%s'"), filename);
	اگر (close(fd) < 0)
		die_errno(_("could not close '%s'"), filename);

	برای (pos = 0; pos < merge_names->len; pos = npos) {
		ساختار object_id oid;
		نویسه *ptr;
		ساختار commit *commit;

		ptr = strchr(merge_names->buf + pos, '\n');
		اگر (ptr)
			npos = ptr - merge_names->buf + 1;
		وگرنه
			npos = merge_names->len;

		اگر (npos - pos < hexsz + 2 ||
		    get_oid_hex(merge_names->buf + pos, &oid))
			commit = پوچ; /* bad */
		وگرنه اگر (memcmp(merge_names->buf + pos + hexsz, "\t\t", 2))
			ادامه; /* not-for-merge */
		وگرنه {
			نویسه saved = merge_names->buf[pos + hexsz];
			merge_names->buf[pos + hexsz] = '\0';
			commit = get_merge_parent(merge_names->buf + pos);
			merge_names->buf[pos + hexsz] = saved;
		}
		اگر (!commit) {
			اگر (ptr)
				*ptr = '\0';
			die(_("not something we can merge in %s: %s"),
			    filename, merge_names->buf + pos);
		}
		remotes = &commit_list_درج(commit, remotes)->next;
	}

	اگر (merge_names == &fetch_head_file)
		strbuf_release(&fetch_head_file);
}

ایستا ساختار commit_list *collect_parents(ساختار commit *head_commit,
					   صحیح *head_subsumed,
					   صحیح argc, ثابت نویسه **argv,
					   ساختار strbuf *merge_پیام)
{
	صحیح i;
	ساختار commit_list *remoteheads = پوچ;
	ساختار commit_list **remotes = &remoteheads;
	ساختار strbuf merge_names = STRBUF_INIT, *autogen = پوچ;

	اگر (merge_پیام && (!have_message || shortlog_len))
		autogen = &merge_names;

	اگر (head_commit)
		remotes = &commit_list_درج(head_commit, remotes)->next;

	اگر (argc == 1 && !strcmp(argv[0], "FETCH_HEAD")) {
		handle_fetch_head(remotes, autogen);
		remoteheads = reduce_parents(head_commit, head_subsumed, remoteheads);
	} وگرنه {
		برای (i = 0; i < argc; i++) {
			ساختار commit *commit = get_merge_parent(argv[i]);
			اگر (!commit)
				help_unknown_ref(argv[i], "merge",
						 _("not something we can merge"));
			remotes = &commit_list_درج(commit, remotes)->next;
		}
		remoteheads = reduce_parents(head_commit, head_subsumed, remoteheads);
		اگر (autogen) {
			ساختار commit_list *p;
			برای (p = remoteheads; p; p = p->next)
				merge_name(merge_remote_util(p->item)->name, autogen);
		}
	}

	اگر (autogen) {
		prepare_merge_message(autogen, merge_پیام);
		strbuf_release(autogen);
	}

	بازگشت remoteheads;
}

ایستا صحیح merging_a_throwaway_tag(ساختار commit *commit)
{
	نویسه *tag_ref;
	ساختار object_id oid;
	صحیح is_throwaway_tag = 0;

	/* Are we merging a tag? */
	اگر (!merge_remote_util(commit) ||
	    !merge_remote_util(commit)->obj ||
	    merge_remote_util(commit)->obj->type != OBJ_TAG)
		بازگشت is_throwaway_tag;

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
			  ((ساختار tag *)merge_remote_util(commit)->obj)->tag);
	اگر (!refs_read_ref(get_main_ref_store(the_repository), tag_ref, &oid) &&
	    oideq(&oid, &merge_remote_util(commit)->obj->oid))
		is_throwaway_tag = 0;
	وگرنه
		is_throwaway_tag = 1;
	آزاد(tag_ref);
	بازگشت is_throwaway_tag;
}

صحیح cmd_merge(صحیح argc,
	      ثابت نویسه **argv,
	      ثابت نویسه *prefix,
	      ساختار repository *repo UNUSED)
{
	ساختار object_id result_درخت, stash, head_oid;
	ساختار commit *head_commit;
	ساختار strbuf buf = STRBUF_INIT;
	صحیح i, ret = 0, head_subsumed;
	صحیح best_cnt = -1, merge_was_ok = 0, automerge_was_ok = 0;
	ساختار commit_list *common = پوچ;
	ثابت نویسه *best_strategy = پوچ, *wt_strategy = پوچ;
	ساختار commit_list *remoteheads = پوچ, *p;
	تهی *branch_to_free, *argv_to_free = پوچ;
	صحیح orig_argc = argc;
	صحیح merge_log_config = -1;

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
						      پوچ);
	اگر (branch)
		skip_prefix(branch, "refs/heads/", &branch);

	init_diff_ui_defaults();
	repo_config(the_repository, git_merge_config, &merge_log_config);

	اگر (!branch || is_null_oid(&head_oid))
		head_commit = پوچ;
	وگرنه
		head_commit = lookup_commit_or_die(&head_oid, "HEAD");

	اگر (branch_mergeoptions)
		parse_branch_merge_options(branch_mergeoptions);
	argc = parse_options(argc, argv, prefix, builtin_merge_options,
			builtin_merge_usage, 0);
	اگر (shortlog_len < 0)
		shortlog_len = (merge_log_config > 0) ? merge_log_config : 0;

	اگر (verbosity < 0 && show_progress == -1)
		show_progress = 0;

	اگر (abort_current_merge) {
		صحیح nargc = 2;
		ثابت نویسه *nargv[] = {"reset", "--merge", پوچ};
		نویسه stash_oid_hex[GIT_MAX_HEXSZ + 1];
		ساختار object_id stash_oid = {0};

		اگر (orig_argc != 2)
			usage_پیام_opt(_("--abort expects no arguments"),
			      builtin_merge_usage, builtin_merge_options);

		اگر (!file_exists(git_path_merge_head(the_repository)))
			die(_("There is no merge to abort (MERGE_HEAD missing)."));

		اگر (!refs_read_ref(get_main_ref_store(the_repository), "MERGE_AUTOSTASH", &stash_oid))
			refs_delete_ref(get_main_ref_store(the_repository),
					"", "MERGE_AUTOSTASH", &stash_oid,
					REF_NO_DEREF);

		/* Invoke 'git reset --merge' */
		ret = cmd_reset(nargc, nargv, prefix, the_repository);

		اگر (!is_null_oid(&stash_oid)) {
			oid_to_hex_r(stash_oid_hex, &stash_oid);
			apply_autostash_oid(stash_oid_hex);
		}

		برو done;
	}

	اگر (quit_current_merge) {
		اگر (orig_argc != 2)
			usage_پیام_opt(_("--quit expects no arguments"),
				      builtin_merge_usage,
				      builtin_merge_options);

		remove_merge_branch_state(the_repository);
		برو done;
	}

	اگر (continue_current_merge) {
		صحیح nargc = 1;
		ثابت نویسه *nargv[] = {"commit", پوچ};

		اگر (orig_argc != 2)
			usage_پیام_opt(_("--continue expects no arguments"),
			      builtin_merge_usage, builtin_merge_options);

		اگر (!file_exists(git_path_merge_head(the_repository)))
			die(_("There is no merge in progress (MERGE_HEAD missing)."));

		/* Invoke 'git commit' */
		ret = cmd_commit(nargc, nargv, prefix, the_repository);
		برو done;
	}

	اگر (repo_read_index_unmerged(the_repository))
		die_resolve_conflict("merge");

	اگر (file_exists(git_path_merge_head(the_repository))) {
		/*
		 * There is no unmerged entry, don't advise 'git
		 * add/rm <file>', just 'git commit'.
		 */
		اگر (advice_enabled(ADVICE_RESOLVE_CONFLICT))
			die(_("You have not concluded your merge (MERGE_HEAD exists).\n"
				  "Please, commit your changes before you merge."));
		وگرنه
			die(_("You have not concluded your merge (MERGE_HEAD exists)."));
	}
	اگر (refs_ref_exists(get_main_ref_store(the_repository), "CHERRY_PICK_HEAD")) {
		اگر (advice_enabled(ADVICE_RESOLVE_CONFLICT))
			die(_("You have not concluded your cherry-pick (CHERRY_PICK_HEAD exists).\n"
			    "Please, commit your changes before you merge."));
		وگرنه
			die(_("You have not concluded your cherry-pick (CHERRY_PICK_HEAD exists)."));
	}
	resolve_undo_clear_index(the_repository->index);

	اگر (option_edit < 0)
		option_edit = default_edit_option();

	cleanup_mode = get_cleanup_mode(cleanup_arg, 0 < option_edit);

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
		اگر (default_to_upstream) {
			argc = setup_with_upstream(&argv);
			argv_to_free = argv;
		}
		وگرنه
			die(_("No commit specified and merge.defaultToUpstream not set."));
	} وگرنه اگر (argc == 1 && !strcmp(argv[0], "-")) {
		argv[0] = "@{-1}";
	}

	اگر (!argc)
		usage_with_options(builtin_merge_usage,
			builtin_merge_options);

	اگر (!head_commit) {
		/*
		 * If the merged head is a valid one there is no reason
		 * to forbid "git merge" into a branch yet to be born.
		 * We do the same for "git pull".
		 */
		ساختار object_id *remote_head_oid;
		اگر (squash)
			die(_("Squash commit into empty head not supported yet"));
		اگر (fast_forward == FF_NO)
			die(_("Non-fast-forward commit does not make sense into "
			    "an empty head"));
		remoteheads = collect_parents(head_commit, &head_subsumed,
					      argc, argv, پوچ);
		اگر (!remoteheads)
			die(_("%s - not something we can merge"), argv[0]);
		اگر (remoteheads->next)
			die(_("Can merge only exactly one commit into empty head"));

		اگر (verify_signatures)
			verify_merge_signature(remoteheads->item, verbosity,
					       check_trust_level);

		remote_head_oid = &remoteheads->item->object.oid;
		read_empty(remote_head_oid);
		refs_update_ref(get_main_ref_store(the_repository),
				"initial pull", "HEAD", remote_head_oid, پوچ,
				0,
				UPDATE_REFS_DIE_ON_ERR);
		برو done;
	}

	/*
	 * All the rest are the commits being merged; prepare
	 * the standard merge summary message to be appended
	 * to the given message.
	 */
	remoteheads = collect_parents(head_commit, &head_subsumed,
				      argc, argv, &merge_پیام);

	اگر (!head_commit || !argc)
		usage_with_options(builtin_merge_usage,
			builtin_merge_options);

	اگر (verify_signatures) {
		برای (p = remoteheads; p; p = p->next) {
			verify_merge_signature(p->item, verbosity,
					       check_trust_level);
		}
	}

	strbuf_addstr(&buf, "merge");
	برای (p = remoteheads; p; p = p->next)
		strbuf_addf(&buf, " %s", merge_remote_util(p->item)->name);
	setenv("GIT_REFLOG_ACTION", buf.buf, 0);
	strbuf_reset(&buf);

	برای (p = remoteheads; p; p = p->next) {
		ساختار commit *commit = p->item;
		strbuf_addf(&buf, "GITHEAD_%s",
			    oid_to_hex(&commit->object.oid));
		setenv(buf.buf, merge_remote_util(commit)->name, 1);
		strbuf_reset(&buf);
		اگر (fast_forward != FF_ONLY && merging_a_throwaway_tag(commit))
			fast_forward = FF_NO;
	}

	اگر (!use_strategies) {
		اگر (!remoteheads)
			; /* already up-to-date */
		وگرنه اگر (!remoteheads->next)
			add_strategies(pull_twohead, DEFAULT_TWOHEAD);
		وگرنه
			add_strategies(pull_octopus, DEFAULT_OCTOPUS);
	}

	برای (i = 0; i < use_strategies_nr; i++) {
		اگر (use_strategies[i]->attr & NO_FAST_FORWARD)
			fast_forward = FF_NO;
		اگر (use_strategies[i]->attr & NO_TRIVIAL)
			allow_trivial = 0;
	}

	اگر (!remoteheads)
		; /* already up-to-date */
	وگرنه اگر (!remoteheads->next) {
		اگر (repo_get_merge_bases(the_repository, head_commit,
					 remoteheads->item, &common) < 0) {
			ret = 2;
			برو done;
		}
	} وگرنه {
		ساختار commit_list *list = remoteheads;
		commit_list_درج(head_commit, &list);
		اگر (get_octopus_merge_bases(list, &common) < 0) {
			آزاد(list);
			ret = 2;
			برو done;
		}
		آزاد(list);
	}

	refs_update_ref(get_main_ref_store(the_repository),
			"updating ORIG_HEAD", "ORIG_HEAD",
			&head_commit->object.oid, پوچ, 0,
			UPDATE_REFS_DIE_ON_ERR);

	اگر (remoteheads && !common) {
		/* No common ancestors found. */
		اگر (!allow_unrelated_histories)
			die(_("refusing to merge unrelated histories"));
		/* otherwise, we need a real merge. */
	} وگرنه اگر (!remoteheads ||
		 (!remoteheads->next && !common->next &&
		  common->item == remoteheads->item)) {
		/*
		 * If head can reach all the merge then we are up to date.
		 * but first the most common case of merging one remote.
		 */
		finish_up_to_date();
		برو done;
	} وگرنه اگر (fast_forward != FF_NO && !remoteheads->next &&
			!common->next &&
			oideq(&common->item->object.oid, &head_commit->object.oid)) {
		/* Again the most common case of merging one remote. */
		ثابت نویسه *پیام = have_message ?
			"Fast-forward (no commit created; -m option ignored)" :
			"Fast-forward";
		ساختار commit *commit;

		اگر (verbosity >= 0) {
			printf(_("Updating %s..%s\n"),
			       repo_find_unique_abbrev(the_repository, &head_commit->object.oid,
						       DEFAULT_ABBREV),
			       repo_find_unique_abbrev(the_repository, &remoteheads->item->object.oid,
						       DEFAULT_ABBREV));
		}
		commit = remoteheads->item;
		اگر (!commit) {
			ret = 1;
			برو done;
		}

		اگر (autostash)
			create_autostash_ref(the_repository, "MERGE_AUTOSTASH",
					     پوچ, false);
		اگر (checkout_fast_forward(the_repository,
					  &head_commit->object.oid,
					  &commit->object.oid,
					  overwrite_ignore)) {
			apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
					    پوچ, پوچ, پوچ, پوچ);
			ret = 1;
			برو done;
		}

		finish(head_commit, remoteheads, &commit->object.oid, پیام);
		remove_merge_branch_state(the_repository);
		برو done;
	} وگرنه اگر (!remoteheads->next && common->next)
		;
		/*
		 * We are not doing octopus and not fast-forward.  Need
		 * a real merge.
		 */
	وگرنه اگر (!remoteheads->next && !common->next && option_commit) {
		/*
		 * We are not doing octopus, not fast-forward, and have
		 * only one common.
		 */
		refresh_index(the_repository->index, REFRESH_QUIET, پوچ, پوچ, پوچ);
		اگر (allow_trivial && fast_forward != FF_ONLY) {
			/*
			 * Must first ensure that index matches HEAD before
			 * attempting a trivial merge.
			 */
			ساختار درخت *head_درخت = repo_get_commit_درخت(the_repository,
								      head_commit);
			ساختار strbuf sb = STRBUF_INIT;

			اگر (repo_index_has_changes(the_repository, head_درخت,
						   &sb)) {
				error(_("Your local changes to the following files would be overwritten by merge:\n  %s"),
				      sb.buf);
				strbuf_release(&sb);
				ret = 2;
				برو done;
			}

			/* See if it is really trivial. */
			git_committer_info(IDENT_STRICT);
			printf(_("Trying really trivial in-index merge...\n"));
			اگر (!read_درخت_trivial(&common->item->object.oid,
					       &head_commit->object.oid,
					       &remoteheads->item->object.oid)) {
				ret = merge_trivial(head_commit, remoteheads);
				برو done;
			}
			printf(_("Nope.\n"));
		}
	} وگرنه {
		/*
		 * An octopus.  If we can reach all the remote we are up
		 * to date.
		 */
		صحیح up_to_date = 1;
		ساختار commit_list *j;

		برای (j = remoteheads; j; j = j->next) {
			صحیح ret = repo_in_merge_bases(the_repository,
						      j->item, head_commit);
			اگر (ret < 0)
				exit(128);
			اگر (!ret) {
				up_to_date = 0;
				بشکن;
			}
		}
		اگر (up_to_date) {
			finish_up_to_date();
			برو done;
		}
	}

	اگر (fast_forward == FF_ONLY)
		die_ff_impossible();

	اگر (autostash)
		create_autostash_ref(the_repository, "MERGE_AUTOSTASH",
				     پوچ, false);

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

	برای (i = 0; i < use_strategies_nr; i++) {
		صحیح ret, cnt;
		اگر (i) {
			printf(_("Rewinding the tree to pristine...\n"));
			restore_state(&head_commit->object.oid, &stash);
		}
		اگر (use_strategies_nr != 1)
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
		اگر (ret < 2) {
			اگر (!ret) {
				/*
				 * This strategy worked; no point in trying
				 * another.
				 */
				merge_was_ok = 1;
				best_strategy = wt_strategy;
				بشکن;
			}
			cnt = (use_strategies_nr > 1) ? evaluate_result() : 0;
			اگر (best_cnt <= 0 || cnt <= best_cnt) {
				best_strategy = wt_strategy;
				best_cnt = cnt;
			}
		}
	}

	/*
	 * If we have a resulting tree, that means the strategy module
	 * auto resolved the merge cleanly.
	 */
	اگر (merge_was_ok && option_commit) {
		automerge_was_ok = 1;
		ret = finish_automerge(head_commit, head_subsumed,
				       common, remoteheads,
				       &result_درخت, wt_strategy);
		برو done;
	}

	/*
	 * Pick the result from the best strategy and have the user fix
	 * it up.
	 */
	اگر (!best_strategy) {
		restore_state(&head_commit->object.oid, &stash);
		اگر (use_strategies_nr > 1)
			fprintf(stderr,
				_("No merge strategy handled the merge.\n"));
		وگرنه
			fprintf(stderr, _("Merge with strategy %s failed.\n"),
				use_strategies[0]->name);
		apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
				    پوچ, پوچ, پوچ, پوچ);
		ret = 2;
		برو done;
	} وگرنه اگر (best_strategy == wt_strategy)
		; /* We already have its result in the working tree. */
	وگرنه {
		printf(_("Rewinding the tree to pristine...\n"));
		restore_state(&head_commit->object.oid, &stash);
		printf(_("Using the %s strategy to prepare resolving by hand.\n"),
			best_strategy);
		try_merge_strategy(best_strategy, common, remoteheads,
				   head_commit);
	}

	اگر (squash) {
		finish(head_commit, remoteheads, پوچ, پوچ);

		git_test_write_commit_graph_or_die(the_repository);
	} وگرنه
		write_merge_state(remoteheads);

	اگر (merge_was_ok)
		fprintf(stderr, _("Automatic merge went well; "
			"stopped before committing as requested\n"));
	وگرنه
		ret = suggest_conflicts();
	اگر (autostash)
		printf(_("When finished, apply stashed changes with `git stash pop`\n"));

done:
	اگر (!automerge_was_ok) {
		commit_list_free(common);
		commit_list_free(remoteheads);
	}
	strbuf_release(&buf);
	آزاد(branch_to_free);
	آزاد(argv_to_free);
	آزاد(pull_twohead);
	آزاد(pull_octopus);
	discard_index(the_repository->index);
	بازگشت ret;
}
