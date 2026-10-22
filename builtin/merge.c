/*
 * Builtin "git merge"
 *
 * Copyright (c) 2008 Miklos Vajna <vmiklos@frugalware.org>
 *
 * Based on git-merge.sh by Junio C Hamano.
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

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

#define DEFAULT_TWOHEAD (1<<0)
#define DEFAULT_OCTOPUS (1<<1)
#define NO_FAST_FORWARD (1<<2)
#define NO_TRIVIAL      (1<<3)

بنية strategy {
	ثابت حرف *name;
	unsigned attr;
};

ساكن ثابت حرف * ثابت builtin_merge_usage[] = {
	N_("git merge [<options>] [<commit>...]"),
	"git merge --abort",
	"git merge --continue",
	عدم
};

#define MERGE_SHOW_DIFFSTAT 1
#define MERGE_SHOW_COMPACTSUMMARY 2

ساكن صحيح show_diffstat = MERGE_SHOW_DIFFSTAT, shortlog_len = -1, squash;
ساكن صحيح option_commit = -1;
ساكن صحيح option_edit = -1;
ساكن صحيح allow_trivial = 1, have_message, verify_signatures;
ساكن صحيح check_trust_level = 1;
ساكن صحيح overwrite_ignore = 1;
ساكن بنية strbuf merge_msg = STRBUF_INIT;
ساكن بنية strategy **use_strategies;
ساكن size_t use_strategies_nr, use_strategies_alloc;
ساكن بنية strvec xopts = STRVEC_INIT;
ساكن ثابت حرف *branch;
ساكن حرف *branch_mergeoptions;
ساكن صحيح verbosity;
ساكن صحيح allow_rerere_auto;
ساكن صحيح abort_current_merge;
ساكن صحيح quit_current_merge;
ساكن صحيح continue_current_merge;
ساكن صحيح allow_unrelated_histories;
ساكن صحيح show_progress = -1;
ساكن صحيح default_to_upstream = 1;
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
ساكن تعداد commit_msg_cleanup_mode cleanup_mode;

ساكن صحيح option_parse_message(ثابت بنية option *opt,
				ثابت حرف *arg, صحيح unset)
{
	بنية strbuf *buf = opt->value;

	ܐܢ (unset)
		strbuf_setlen(buf, 0);
	ܐܚܪܢܐ ܐܢ (arg) {
		strbuf_addf(buf, "%s%s", buf->len ? "\n\n" : "", arg);
		have_message = 1;
	} ܐܚܪܢܐ
		ܦܢܐ error(_("switch `m' requires a value"));
	ܦܢܐ 0;
}

ساكن تعداد parse_opt_result option_read_message(بنية parse_opt_ctx_t *ctx,
						 ثابت بنية option *opt,
						 ثابت حرف *arg_not_used,
						 صحيح unset)
{
	بنية strbuf *buf = opt->value;
	ثابت حرف *arg;

	BUG_ON_OPT_ARG(arg_not_used);
	ܐܢ (unset)
		BUG("-F cannot be negated");

	ܐܢ (ctx->opt) {
		arg = ctx->opt;
		ctx->opt = عدم;
	} ܐܚܪܢܐ ܐܢ (ctx->argc > 1) {
		ctx->argc--;
		arg = *++ctx->argv;
	} ܐܚܪܢܐ
		ܦܢܐ error(_("option `%s' requires a value"), opt->long_name);

	ܐܢ (buf->len)
		strbuf_addch(buf, '\n');
	ܐܢ (ctx->prefix && !is_absolute_path(arg))
		arg = prefix_filename(ctx->prefix, arg);
	ܐܢ (strbuf_read_file(buf, arg, 0) < 0)
		ܦܢܐ error(_("could not read file '%s'"), arg);
	have_message = 1;

	ܦܢܐ 0;
}

ساكن بنية strategy *get_strategy(ثابت حرف *name)
{
	صحيح i;
	بنية strategy *ret;
	ساكن بنية cmdnames main_cmds = {0}, other_cmds = {0};
	ساكن صحيح loaded;
	حرف *default_strategy = عدم;

	ܐܢ (!name)
		ܦܢܐ عدم;

	ܐܢ (default_strategy &&
	    !strcmp(default_strategy, "ort") &&
	    !strcmp(name, "recursive")) {
		name = "ort";
	}

	لكل (i = 0; i < ARRAY_SIZE(all_strategy); i++)
		ܐܢ (!strcmp(name, all_strategy[i].name))
			ܦܢܐ &all_strategy[i];

	ܐܢ (!loaded) {
		بنية cmdnames not_strategies = {0};
		loaded = 1;

		load_command_list("git-merge-", &main_cmds, &other_cmds);
		لكل (i = 0; i < main_cmds.cnt; i++) {
			صحيح j, found = 0;
			بنية cmdname *ent = main_cmds.names[i];
			لكل (j = 0; !found && j < ARRAY_SIZE(all_strategy); j++)
				ܐܢ (!xstrncmpz(all_strategy[j].name, ent->name, ent->len))
					found = 1;
			ܐܢ (!found)
				add_cmdname(&not_strategies, ent->name, ent->len);
		}
		exclude_cmds(&main_cmds, &not_strategies);

		cmdnames_release(&not_strategies);
	}
	ܐܢ (!is_in_cmdlist(&main_cmds, name) && !is_in_cmdlist(&other_cmds, name)) {
		fprintf(stderr, _("Could not find merge strategy '%s'.\n"), name);
		fprintf(stderr, _("Available strategies are:"));
		لكل (i = 0; i < main_cmds.cnt; i++)
			fprintf(stderr, " %s", main_cmds.names[i]->name);
		fprintf(stderr, ".\n");
		ܐܢ (other_cmds.cnt) {
			fprintf(stderr, _("Available custom strategies are:"));
			لكل (i = 0; i < other_cmds.cnt; i++)
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
	ܦܢܐ ret;
}

ساكن فراغ append_strategy(بنية strategy *s)
{
	ALLOC_GROW(use_strategies, use_strategies_nr + 1, use_strategies_alloc);
	use_strategies[use_strategies_nr++] = s;
}

ساكن صحيح option_parse_strategy(ثابت بنية option *opt UNUSED,
				 ثابت حرف *name, صحيح unset)
{
	ܐܢ (unset)
		ܦܢܐ 0;

	append_strategy(get_strategy(name));
	ܦܢܐ 0;
}

ساكن صحيح option_parse_compact_summary(ثابت بنية option *opt,
					ثابت حرف *name UNUSED, صحيح unset)
{
	صحيح *setting = opt->value;

	ܐܢ (unset)
		*setting = 0;
	ܐܚܪܢܐ
		*setting = MERGE_SHOW_COMPACTSUMMARY;
	ܦܢܐ 0;
}

ساكن بنية option builtin_merge_options[] = {
	OPT_SET_INT('n', عدم, &show_diffstat,
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
		.value = &shortlog_len,
		.precision = حجم(shortlog_len),
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
	OPT_CALLBACK('s', "strategy", عدم, N_("strategy"),
		N_("merge strategy to use"), option_parse_strategy),
	OPT_STRVEC('X', "strategy-option", &xopts, N_("option=value"),
		N_("option for selected merge strategy")),
	OPT_CALLBACK('m', "message", &merge_msg, N_("message"),
		N_("merge commit message (for a non-fast-forward merge)"),
		option_parse_message),
	{
		.type = OPTION_LOWLEVEL_CALLBACK,
		.short_name = 'F',
		.long_name = "file",
		.value = &merge_msg,
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
		.value = &sign_commit,
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

ساكن صحيح save_state(بنية object_id *stash)
{
	صحيح len;
	بنية child_process cp = CHILD_PROCESS_INIT;
	بنية strbuf buffer = STRBUF_INIT;
	بنية lock_file lock_file = LOCK_INIT;
	صحيح fd;
	صحيح rc = -1;

	fd = repo_hold_locked_index(the_repository, &lock_file, 0);
	refresh_index(the_repository->index, REFRESH_QUIET, عدم, عدم, عدم);
	ܐܢ (0 <= fd)
		repo_update_index_if_able(the_repository, &lock_file);
	rollback_lock_file(&lock_file);

	strvec_pushl(&cp.args, "stash", "create", عدم);
	cp.out = -1;
	cp.git_cmd = 1;

	ܐܢ (start_command(&cp))
		die(_("could not run stash."));
	len = strbuf_read(&buffer, cp.out, 1024);
	close(cp.out);

	ܐܢ (finish_command(&cp) || len < 0)
		die(_("stash failed"));
	ܐܚܪܢܐ ܐܢ (!len)		/* no changes */
		اذهب out;
	strbuf_setlen(&buffer, buffer.len-1);
	ܐܢ (repo_get_oid(the_repository, buffer.buf, stash))
		die(_("not a valid object: %s"), buffer.buf);
	rc = 0;
out:
	strbuf_release(&buffer);
	ܦܢܐ rc;
}

ساكن فراغ read_empty(ثابت بنية object_id *oid)
{
	بنية child_process cmd = CHILD_PROCESS_INIT;

	strvec_pushl(&cmd.args, "read-tree", "-m", "-u",
		     empty_tree_oid_hex(the_repository->hash_algo),
		     oid_to_hex(oid), عدم);
	cmd.git_cmd = 1;

	ܐܢ (run_command(&cmd))
		die(_("read-tree failed"));
}

ساكن فراغ reset_hard(ثابت بنية object_id *oid)
{
	بنية child_process cmd = CHILD_PROCESS_INIT;

	strvec_pushl(&cmd.args, "read-tree", "-v", "--reset", "-u",
		     oid_to_hex(oid), عدم);
	cmd.git_cmd = 1;

	ܐܢ (run_command(&cmd))
		die(_("read-tree failed"));
}

ساكن فراغ restore_state(ثابت بنية object_id *head,
			  ثابت بنية object_id *stash)
{
	بنية child_process cmd = CHILD_PROCESS_INIT;

	reset_hard(head);

	ܐܢ (is_null_oid(stash))
		اذهب refresh_cache;

	strvec_pushl(&cmd.args, "stash", "apply", "--index", "--quiet", عدم);
	strvec_push(&cmd.args, oid_to_hex(stash));

	/*
	 * It is OK to ignore error here, for example when there was
	 * nothing to restore.
	 */
	cmd.git_cmd = 1;
	run_command(&cmd);

refresh_cache:
	discard_index(the_repository->index);
	ܐܢ (repo_read_index(the_repository) < 0)
		die(_("could not read index"));
}

/* This is called when no merge was necessary. */
ساكن فراغ finish_up_to_date(فراغ)
{
	ܐܢ (verbosity >= 0) {
		ܐܢ (squash)
			puts(_("Already up to date. (nothing to squash)"));
		ܐܚܪܢܐ
			puts(_("Already up to date."));
	}
	remove_merge_branch_state(the_repository);
}

ساكن فراغ squash_message(بنية commit *commit, بنية commit_list *remoteheads)
{
	بنية rev_info rev;
	بنية strbuf out = STRBUF_INIT;
	بنية commit_list *j;
	بنية pretty_print_context ctx = {0};

	printf(_("Squash commit -- not updating HEAD\n"));

	repo_init_revisions(the_repository, &rev, عدم);
	diff_merges_suppress(&rev);
	rev.commit_format = CMIT_FMT_MEDIUM;

	commit->object.flags |= UNINTERESTING;
	add_pending_object(&rev, &commit->object, عدم);

	لكل (j = remoteheads; j; j = j->next)
		add_pending_object(&rev, &j->item->object, عدم);

	setup_revisions(0, عدم, &rev, عدم);
	ܐܢ (prepare_revision_walk(&rev))
		die(_("revision walk setup failed"));

	ctx.abbrev = rev.abbrev;
	ctx.date_mode = rev.date_mode;
	ctx.fmt = rev.commit_format;

	strbuf_addstr(&out, "Squashed commit of the following:\n");
	ܟܡܐ ((commit = get_revision(&rev)) != عدم) {
		strbuf_addch(&out, '\n');
		strbuf_addf(&out, "commit %s\n",
			oid_to_hex(&commit->object.oid));
		pretty_print_commit(&ctx, commit, &out);
	}
	write_file_buf(git_path_squash_msg(the_repository), out.buf, out.len);
	strbuf_release(&out);
	release_revisions(&rev);
}

ساكن فراغ finish(بنية commit *head_commit,
		   بنية commit_list *remoteheads,
		   ثابت بنية object_id *new_head, ثابت حرف *msg)
{
	بنية strbuf reflog_message = STRBUF_INIT;
	ثابت بنية object_id *head = &head_commit->object.oid;

	ܐܢ (!msg)
		strbuf_addstr(&reflog_message, getenv("GIT_REFLOG_ACTION"));
	ܐܚܪܢܐ {
		ܐܢ (verbosity >= 0)
			printf("%s\n", msg);
		strbuf_addf(&reflog_message, "%s: %s",
			getenv("GIT_REFLOG_ACTION"), msg);
	}
	ܐܢ (squash) {
		squash_message(head_commit, remoteheads);
	} ܐܚܪܢܐ {
		ܐܢ (verbosity >= 0 && !merge_msg.len)
			printf(_("No merge message -- not updating HEAD\n"));
		ܐܚܪܢܐ {
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
	ܐܢ (new_head && show_diffstat) {
		بنية diff_options opts;
		repo_diff_setup(the_repository, &opts);
		init_diffstat_widths(&opts);

		بدل (show_diffstat) {
		حالة MERGE_SHOW_DIFFSTAT: /* 1 */
			opts.output_format |=
				DIFF_FORMAT_SUMMARY | DIFF_FORMAT_DIFFSTAT;
			اكسر;
		حالة MERGE_SHOW_COMPACTSUMMARY: /* 2 */
			opts.output_format |= DIFF_FORMAT_DIFFSTAT;
			opts.flags.stat_with_summary = 1;
			اكسر;
		افتراضي:
			اكسر;
		}
		opts.detect_rename = DIFF_DETECT_RENAME;
		diff_setup_done(&opts);
		diff_tree_oid(head, new_head, "", &opts);
		diffcore_std(&opts);
		diff_flush(&opts);
	}

	/* Run a post-merge hook */
	run_hooks_l(the_repository, "post-merge", squash ? "1" : "0", عدم);

	ܐܢ (new_head)
		apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
				    عدم, عدم, عدم, عدم);
	strbuf_release(&reflog_message);
}

/* Get the name for the merge commit's message. */
ساكن فراغ merge_name(ثابت حرف *remote, بنية strbuf *msg)
{
	بنية commit *remote_head;
	بنية object_id branch_head;
	بنية strbuf bname = STRBUF_INIT;
	بنية merge_remote_desc *desc;
	ثابت حرف *ptr;
	حرف *found_ref = عدم;
	صحيح len, early;

	copy_branchname(the_repository, &bname, remote, 0);
	remote = bname.buf;

	oidclr(&branch_head, the_repository->hash_algo);
	remote_head = get_merge_parent(remote);
	ܐܢ (!remote_head)
		die(_("'%s' does not point to a commit"), remote);

	ܐܢ (repo_dwim_ref(the_repository, remote, strlen(remote), &branch_head,
			  &found_ref, 0) > 0) {
		ܐܢ (starts_with(found_ref, "refs/heads/")) {
			strbuf_addf(msg, "%s\t\tbranch '%s' of .\n",
				    oid_to_hex(&branch_head), remote);
			اذهب cleanup;
		}
		ܐܢ (starts_with(found_ref, "refs/tags/")) {
			strbuf_addf(msg, "%s\t\ttag '%s' of .\n",
				    oid_to_hex(&branch_head), remote);
			اذهب cleanup;
		}
		ܐܢ (starts_with(found_ref, "refs/remotes/")) {
			strbuf_addf(msg, "%s\t\tremote-tracking branch '%s' of .\n",
				    oid_to_hex(&branch_head), remote);
			اذهب cleanup;
		}
	}

	/* See if remote matches <name>^^^.. or <name>~<number> */
	لكل (len = 0, ptr = remote + strlen(remote);
	     remote < ptr && ptr[-1] == '^';
	     ptr--)
		len++;
	ܐܢ (len)
		early = 1;
	ܐܚܪܢܐ {
		early = 0;
		ptr = strrchr(remote, '~');
		ܐܢ (ptr) {
			صحيح seen_nonzero = 0;

			len++; /* count ~ */
			ܟܡܐ (*++ptr && isdigit(*ptr)) {
				seen_nonzero |= (*ptr != '0');
				len++;
			}
			ܐܢ (*ptr)
				len = 0; /* not ...~<number> */
			ܐܚܪܢܐ ܐܢ (seen_nonzero)
				early = 1;
			ܐܚܪܢܐ ܐܢ (len == 1)
				early = 1; /* "name~" is "name~1"! */
		}
	}
	ܐܢ (len) {
		بنية strbuf truname = STRBUF_INIT;
		strbuf_addf(&truname, "refs/heads/%s", remote);
		strbuf_setlen(&truname, truname.len - len);
		ܐܢ (refs_ref_exists(get_main_ref_store(the_repository), truname.buf)) {
			strbuf_addf(msg,
				    "%s\t\tbranch '%s'%s of .\n",
				    oid_to_hex(&remote_head->object.oid),
				    truname.buf + 11,
				    (early ? " (early part)" : ""));
			strbuf_release(&truname);
			اذهب cleanup;
		}
		strbuf_release(&truname);
	}

	desc = merge_remote_util(remote_head);
	ܐܢ (desc && desc->obj && desc->obj->type == OBJ_TAG) {
		strbuf_addf(msg, "%s\t\t%s '%s'\n",
			    oid_to_hex(&desc->obj->oid),
			    type_name(desc->obj->type),
			    remote);
		اذهب cleanup;
	}

	strbuf_addf(msg, "%s\t\tcommit '%s'\n",
		oid_to_hex(&remote_head->object.oid), remote);
cleanup:
	حرر(found_ref);
	strbuf_release(&bname);
}

ساكن فراغ parse_branch_merge_options(حرف *bmo)
{
	ثابت حرف **argv;
	صحيح argc;

	ܐܢ (!bmo)
		ܦܢܐ;
	argc = split_cmdline(bmo, &argv);
	ܐܢ (argc < 0)
		die(_("Bad branch.%s.mergeoptions string: %s"), branch,
		    _(split_cmdline_strerror(argc)));
	REALLOC_ARRAY(argv, argc + 2);
	MOVE_ARRAY(argv + 1, argv, argc + 1);
	argc++;
	argv[0] = "branch.*.mergeoptions";
	parse_options(argc, argv, عدم, builtin_merge_options,
		      builtin_merge_usage, 0);
	حرر(argv);
}

ساكن صحيح git_merge_config(ثابت حرف *k, ثابت حرف *v,
			    ثابت بنية config_context *ctx, فراغ *cb)
{
	صحيح status;
	ثابت حرف *str;

	ܐܢ (branch &&
	    skip_prefix(k, "branch.", &str) &&
	    skip_prefix(str, branch, &str) &&
	    !strcmp(str, ".mergeoptions")) {
		حرر(branch_mergeoptions);
		branch_mergeoptions = xstrdup(v);
		ܦܢܐ 0;
	}

	ܐܢ (!strcmp(k, "merge.diffstat") || !strcmp(k, "merge.stat")) {
		صحيح val = git_parse_maybe_bool_text(v);
		بدل (val) {
		حالة 0:
			show_diffstat = 0;
			اكسر;
		حالة 1:
			show_diffstat = MERGE_SHOW_DIFFSTAT;
			اكسر;
		افتراضي:
			ܐܢ (!strcmp(v, "compact"))
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
			ܐܚܪܢܐ
				/*
				 * A setting from a future?  It is not an
				 * error grave enough to fail the command.
				 * proceed using the default one.
				 */
				show_diffstat = MERGE_SHOW_DIFFSTAT;
			اكسر;
		}
	} ܐܚܪܢܐ ܐܢ (!strcmp(k, "merge.verifysignatures")) {
		verify_signatures = git_config_bool(k, v);
	} ܐܚܪܢܐ ܐܢ (!strcmp(k, "pull.twohead")) {
		FREE_AND_NULL(pull_twohead);
		ܦܢܐ git_config_string(&pull_twohead, k, v);
	} ܐܚܪܢܐ ܐܢ (!strcmp(k, "pull.octopus")) {
		FREE_AND_NULL(pull_octopus);
		ܦܢܐ git_config_string(&pull_octopus, k, v);
	} ܐܚܪܢܐ ܐܢ (!strcmp(k, "commit.cleanup")) {
		ܦܢܐ git_config_string(&cleanup_arg, k, v);
	} ܐܚܪܢܐ ܐܢ (!strcmp(k, "merge.ff")) {
		صحيح boolval = git_parse_maybe_bool(v);
		ܐܢ (0 <= boolval) {
			fast_forward = boolval ? FF_ALLOW : FF_NO;
		} ܐܚܪܢܐ ܐܢ (v && !strcmp(v, "only")) {
			fast_forward = FF_ONLY;
		} /* do not barf on values from future versions of git */
		ܦܢܐ 0;
	} ܐܚܪܢܐ ܐܢ (!strcmp(k, "merge.defaulttoupstream")) {
		default_to_upstream = git_config_bool(k, v);
		ܦܢܐ 0;
	} ܐܚܪܢܐ ܐܢ (!strcmp(k, "commit.gpgsign")) {
		sign_commit = git_config_bool(k, v) ? "" : عدم;
		ܦܢܐ 0;
	} ܐܚܪܢܐ ܐܢ (!strcmp(k, "gpg.mintrustlevel")) {
		check_trust_level = 0;
	} ܐܚܪܢܐ ܐܢ (!strcmp(k, "merge.autostash")) {
		autostash = git_config_bool(k, v);
		ܦܢܐ 0;
	}

	status = fmt_merge_msg_config(k, v, ctx, cb);
	ܐܢ (status)
		ܦܢܐ status;
	ܦܢܐ git_diff_ui_config(k, v, ctx, cb);
}

ساكن صحيح read_tree_trivial(بنية object_id *common, بنية object_id *head,
			     بنية object_id *one)
{
	صحيح i, nr_trees = 0;
	بنية tree *trees[MAX_UNPACK_TREES];
	بنية tree_desc t[MAX_UNPACK_TREES];
	بنية unpack_trees_options opts;

	memset(&opts, 0, حجم(opts));
	opts.head_idx = 2;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;
	opts.update = 1;
	opts.verbose_update = 1;
	opts.trivial_merges_only = 1;
	opts.merge = 1;
	opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
	trees[nr_trees] = repo_parse_tree_indirect(the_repository, common);
	ܐܢ (!trees[nr_trees++])
		ܦܢܐ -1;
	trees[nr_trees] = repo_parse_tree_indirect(the_repository, head);
	ܐܢ (!trees[nr_trees++])
		ܦܢܐ -1;
	trees[nr_trees] = repo_parse_tree_indirect(the_repository, one);
	ܐܢ (!trees[nr_trees++])
		ܦܢܐ -1;
	opts.fn = threeway_merge;
	cache_tree_free(&the_repository->index->cache_tree);
	لكل (i = 0; i < nr_trees; i++) {
		repo_parse_tree(the_repository, trees[i]);
		init_tree_desc(t+i, &trees[i]->object.oid,
			       trees[i]->buffer, trees[i]->size);
	}
	ܐܢ (unpack_trees(nr_trees, t, &opts))
		ܦܢܐ -1;
	ܦܢܐ 0;
}

ساكن فراغ write_tree_trivial(بنية object_id *oid)
{
	ܐܢ (write_index_as_tree(oid, the_repository->index,
				repo_get_index_file(the_repository),
				0, عدم))
		die(_("git write-tree failed to write a tree"));
}

ساكن صحيح try_merge_strategy(ثابت حرف *strategy, بنية commit_list *common,
			      بنية commit_list *remoteheads,
			      بنية commit *head)
{
	ثابت حرف *head_arg = "HEAD";

	ܐܢ (repo_refresh_and_write_index(the_repository, REFRESH_QUIET,
					 SKIP_IF_UNCHANGED, 0, عدم, عدم,
					 عدم) < 0)
		die(_("Unable to write index."));

	ܐܢ (!strcmp(strategy, "recursive") || !strcmp(strategy, "subtree") ||
	    !strcmp(strategy, "ort")) {
		بنية lock_file lock = LOCK_INIT;
		صحيح clean, x;
		بنية commit *result;
		بنية commit_list *reversed = عدم;
		بنية merge_options o;
		بنية commit_list *j;

		ܐܢ (remoteheads->next) {
			error(_("Not handling anything other than two heads merge."));
			ܦܢܐ 2;
		}

		init_ui_merge_options(&o, the_repository);
		ܐܢ (!strcmp(strategy, "subtree"))
			o.subtree_shift = "";

		o.show_rename_progress =
			show_progress == -1 ? isatty(2) : show_progress;

		لكل (x = 0; x < xopts.nr; x++)
			ܐܢ (parse_merge_opt(&o, xopts.v[x]))
				die(_("unknown strategy option: -X%s"), xopts.v[x]);

		o.branch1 = head_arg;
		o.branch2 = merge_remote_util(remoteheads->item)->name;

		لكل (j = common; j; j = j->next)
			commit_list_insert(j->item, &reversed);

		repo_hold_locked_index(the_repository, &lock,
				       LOCK_DIE_ON_ERROR);
		clean = merge_ort_recursive(&o, head, remoteheads->item,
					    reversed, &result);
		commit_list_free(reversed);
		strbuf_release(&o.obuf);

		ܐܢ (clean < 0) {
			rollback_lock_file(&lock);
			ܦܢܐ 2;
		}
		ܐܢ (write_locked_index(the_repository->index, &lock,
				       COMMIT_LOCK | SKIP_IF_UNCHANGED))
			die(_("unable to write %s"), repo_get_index_file(the_repository));
		ܦܢܐ clean ? 0 : 1;
	} ܐܚܪܢܐ {
		ܦܢܐ try_merge_command(the_repository,
					 strategy, xopts.nr, xopts.v,
					 common, head_arg, remoteheads);
	}
}

ساكن فراغ count_diff_files(بنية diff_queue_struct *q,
			     بنية diff_options *opt UNUSED, فراغ *data)
{
	صحيح *count = data;

	(*count) += q->nr;
}

ساكن صحيح count_unmerged_entries(فراغ)
{
	صحيح i, ret = 0;

	لكل (i = 0; i < the_repository->index->cache_nr; i++)
		ܐܢ (ce_stage(the_repository->index->cache[i]))
			ret++;

	ܦܢܐ ret;
}

ساكن فراغ add_strategies(ثابت حرف *string, unsigned attr)
{
	صحيح i;

	ܐܢ (string) {
		بنية string_list list = STRING_LIST_INIT_DUP;
		بنية string_list_item *item;
		string_list_split(&list, string, " ", -1);
		for_each_string_list_item(item, &list)
			append_strategy(get_strategy(item->string));
		string_list_clear(&list, 0);
		ܦܢܐ;
	}
	لكل (i = 0; i < ARRAY_SIZE(all_strategy); i++)
		ܐܢ (all_strategy[i].attr & attr)
			append_strategy(&all_strategy[i]);

}

ساكن فراغ read_merge_msg(بنية strbuf *msg)
{
	ثابت حرف *filename = git_path_merge_msg(the_repository);
	strbuf_reset(msg);
	ܐܢ (strbuf_read_file(msg, filename, 0) < 0)
		die_errno(_("Could not read from '%s'"), filename);
}

ساكن فراغ write_merge_state(بنية commit_list *);
ساكن فراغ abort_commit(بنية commit_list *remoteheads, ثابت حرف *err_msg)
{
	ܐܢ (err_msg)
		error("%s", err_msg);
	fprintf(stderr,
		_("Not committing merge; use 'git commit' to complete the merge.\n"));
	write_merge_state(remoteheads);
	exit(1);
}

ساكن ثابت حرف merge_editor_comment[] =
N_("Please enter a commit message to explain why this merge is necessary,\n"
   "especially if it merges an updated upstream into a topic branch.\n"
   "\n");

ساكن ثابت حرف scissors_editor_comment[] =
N_("An empty message aborts the commit.\n");

ساكن ثابت حرف no_scissors_editor_comment[] =
N_("Lines starting with '%s' will be ignored, and an empty message aborts\n"
   "the commit.\n");

ساكن فراغ write_merge_heads(بنية commit_list *);
ساكن فراغ prepare_to_commit(بنية commit_list *remoteheads)
{
	بنية strbuf msg = STRBUF_INIT;
	ثابت حرف *index_file = repo_get_index_file(the_repository);

	ܐܢ (!no_verify) {
		صحيح invoked_hook;

		ܐܢ (run_commit_hook(0 < option_edit, index_file, &invoked_hook,
				    "pre-merge-commit", عدم))
			abort_commit(remoteheads, عدم);
		/*
		 * Re-read the index as pre-merge-commit hook could have updated it,
		 * and write it out as a tree.  We must do this before we invoke
		 * the editor and after we invoke run_status above.
		 */
		ܐܢ (invoked_hook)
			discard_index(the_repository->index);
	}
	read_index_from(the_repository->index, index_file,
			repo_get_git_dir(the_repository));
	strbuf_addbuf(&msg, &merge_msg);
	ܐܢ (squash)
		BUG("the control must not reach here under --squash");
	ܐܢ (0 < option_edit) {
		strbuf_addch(&msg, '\n');
		ܐܢ (cleanup_mode == COMMIT_MSG_CLEANUP_SCISSORS) {
			wt_status_append_cut_line(&msg);
			strbuf_commented_addf(&msg, comment_line_str, "\n");
		}
		strbuf_commented_addf(&msg, comment_line_str,
				      _(merge_editor_comment));
		ܐܢ (cleanup_mode == COMMIT_MSG_CLEANUP_SCISSORS)
			strbuf_commented_addf(&msg, comment_line_str,
					      _(scissors_editor_comment));
		ܐܚܪܢܐ
			strbuf_commented_addf(&msg, comment_line_str,
				_(no_scissors_editor_comment), comment_line_str);
	}
	ܐܢ (signoff)
		append_signoff(&msg, ignored_log_message_bytes(msg.buf, msg.len), 0);
	write_merge_heads(remoteheads);
	write_file_buf(git_path_merge_msg(the_repository), msg.buf, msg.len);
	ܐܢ (run_commit_hook(0 < option_edit, repo_get_index_file(the_repository),
			    عدم, "prepare-commit-msg",
			    git_path_merge_msg(the_repository), "merge", عدم))
		abort_commit(remoteheads, عدم);
	ܐܢ (0 < option_edit) {
		ܐܢ (launch_editor(git_path_merge_msg(the_repository), عدم, عدم))
			abort_commit(remoteheads, عدم);
	}

	ܐܢ (!no_verify && run_commit_hook(0 < option_edit, repo_get_index_file(the_repository),
					  عدم, "commit-msg",
					  git_path_merge_msg(the_repository), عدم))
		abort_commit(remoteheads, عدم);

	read_merge_msg(&msg);
	cleanup_message(&msg, cleanup_mode, 0);
	ܐܢ (!msg.len)
		abort_commit(remoteheads, _("Empty commit message."));
	strbuf_release(&merge_msg);
	strbuf_addbuf(&merge_msg, &msg);
	strbuf_release(&msg);
}

ساكن صحيح merge_trivial(بنية commit *head, بنية commit_list *remoteheads)
{
	بنية object_id result_tree, result_commit;
	بنية commit_list *parents = عدم, **pptr = &parents;

	ܐܢ (repo_refresh_and_write_index(the_repository, REFRESH_QUIET,
					 SKIP_IF_UNCHANGED, 0, عدم, عدم,
					 عدم) < 0)
		ܦܢܐ error(_("Unable to write index."));

	write_tree_trivial(&result_tree);
	printf(_("Wonderful.\n"));
	pptr = commit_list_append(head, pptr);
	pptr = commit_list_append(remoteheads->item, pptr);
	prepare_to_commit(remoteheads);
	ܐܢ (commit_tree(merge_msg.buf, merge_msg.len, &result_tree, parents,
			&result_commit, عدم, sign_commit))
		die(_("failed to write commit object"));
	finish(head, remoteheads, &result_commit, "In-index merge");

	remove_merge_branch_state(the_repository);
	commit_list_free(parents);
	ܦܢܐ 0;
}

ساكن صحيح finish_automerge(بنية commit *head,
			    صحيح head_subsumed,
			    بنية commit_list *common,
			    بنية commit_list *remoteheads,
			    بنية object_id *result_tree,
			    ثابت حرف *wt_strategy)
{
	بنية commit_list *parents = عدم;
	بنية strbuf buf = STRBUF_INIT;
	بنية object_id result_commit;

	write_tree_trivial(result_tree);
	commit_list_free(common);
	parents = remoteheads;
	ܐܢ (!head_subsumed || fast_forward == FF_NO)
		commit_list_insert(head, &parents);
	prepare_to_commit(remoteheads);
	ܐܢ (commit_tree(merge_msg.buf, merge_msg.len, result_tree, parents,
			&result_commit, عدم, sign_commit))
		die(_("failed to write commit object"));
	strbuf_addf(&buf, "Merge made by the '%s' strategy.", wt_strategy);
	finish(head, remoteheads, &result_commit, buf.buf);

	strbuf_release(&buf);
	remove_merge_branch_state(the_repository);
	commit_list_free(parents);
	ܦܢܐ 0;
}

ساكن صحيح suggest_conflicts(فراغ)
{
	ثابت حرف *filename;
	FILE *fp;
	بنية strbuf msgbuf = STRBUF_INIT;

	filename = git_path_merge_msg(the_repository);
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
	ܦܢܐ 1;
}

ساكن صحيح evaluate_result(فراغ)
{
	صحيح cnt = 0;
	بنية rev_info rev;

	/* Check how many files differ. */
	repo_init_revisions(the_repository, &rev, "");
	setup_revisions(0, عدم, &rev, عدم);
	rev.diffopt.output_format |=
		DIFF_FORMAT_CALLBACK;
	rev.diffopt.format_callback = count_diff_files;
	rev.diffopt.format_callback_data = &cnt;
	run_diff_files(&rev, 0);

	/*
	 * Check how many unmerged entries are
	 * there.
	 */
	cnt += count_unmerged_entries();

	release_revisions(&rev);
	ܦܢܐ cnt;
}

/*
 * Pretend as if the user told us to merge with the remote-tracking
 * branch we have for the upstream of the current branch
 */
ساكن صحيح setup_with_upstream(ثابت حرف ***argv)
{
	بنية branch *branch = branch_get(عدم);
	صحيح i;
	ثابت حرف **args;

	ܐܢ (!branch)
		die(_("No current branch."));
	ܐܢ (!branch->remote_name)
		die(_("No remote for the current branch."));
	ܐܢ (!branch->merge_nr)
		die(_("No default upstream defined for the current branch."));

	args = xcalloc(st_add(branch->merge_nr, 1), حجم(حرف *));
	لكل (i = 0; i < branch->merge_nr; i++) {
		ܐܢ (!branch->merge[i]->dst)
			die(_("No remote-tracking branch for %s from %s"),
			    branch->merge[i]->src, branch->remote_name);
		args[i] = branch->merge[i]->dst;
	}
	args[i] = عدم;
	*argv = args;
	ܦܢܐ i;
}

ساكن فراغ write_merge_heads(بنية commit_list *remoteheads)
{
	بنية commit_list *j;
	بنية strbuf buf = STRBUF_INIT;

	لكل (j = remoteheads; j; j = j->next) {
		بنية object_id *oid;
		بنية commit *c = j->item;
		بنية merge_remote_desc *desc;

		desc = merge_remote_util(c);
		ܐܢ (desc && desc->obj) {
			oid = &desc->obj->oid;
		} ܐܚܪܢܐ {
			oid = &c->object.oid;
		}
		strbuf_addf(&buf, "%s\n", oid_to_hex(oid));
	}
	write_file_buf(git_path_merge_head(the_repository), buf.buf, buf.len);

	strbuf_reset(&buf);
	ܐܢ (fast_forward == FF_NO)
		strbuf_addstr(&buf, "no-ff");
	write_file_buf(git_path_merge_mode(the_repository), buf.buf, buf.len);
	strbuf_release(&buf);
}

ساكن فراغ write_merge_state(بنية commit_list *remoteheads)
{
	write_merge_heads(remoteheads);
	strbuf_addch(&merge_msg, '\n');
	write_file_buf(git_path_merge_msg(the_repository), merge_msg.buf,
		       merge_msg.len);
}

ساكن صحيح default_edit_option(فراغ)
{
	ساكن ثابت حرف name[] = "GIT_MERGE_AUTOEDIT";
	ثابت حرف *e = getenv(name);
	بنية stat st_stdin, st_stdout;

	ܐܢ (have_message)
		/* an explicit -m msg without --[no-]edit */
		ܦܢܐ 0;

	ܐܢ (e) {
		صحيح v = git_parse_maybe_bool(e);
		ܐܢ (v < 0)
			die(_("Bad value '%s' in environment '%s'"), e, name);
		ܦܢܐ v;
	}

	/* Use editor if stdin and stdout are the same and is a tty */
	ܦܢܐ (!fstat(0, &st_stdin) &&
		!fstat(1, &st_stdout) &&
		isatty(0) && isatty(1) &&
		st_stdin.st_dev == st_stdout.st_dev &&
		st_stdin.st_ino == st_stdout.st_ino &&
		st_stdin.st_mode == st_stdout.st_mode);
}

ساكن بنية commit_list *reduce_parents(بنية commit *head_commit,
					  صحيح *head_subsumed,
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
	*head_subsumed = 1;

	/* Find what parents to record by checking independent ones. */
	parents = reduce_heads(remoteheads);
	commit_list_free(remoteheads);

	remoteheads = عدم;
	remotes = &remoteheads;
	ܟܡܐ (parents) {
		بنية commit *commit = pop_commit(&parents);
		ܐܢ (commit == head_commit)
			*head_subsumed = 0;
		ܐܚܪܢܐ
			remotes = &commit_list_insert(commit, remotes)->next;
	}
	ܦܢܐ remoteheads;
}

ساكن فراغ prepare_merge_message(بنية strbuf *merge_names, بنية strbuf *merge_msg)
{
	بنية fmt_merge_msg_opts opts;

	memset(&opts, 0, حجم(opts));
	opts.add_title = !have_message;
	opts.shortlog_len = shortlog_len;
	opts.credit_people = (0 < option_edit);
	opts.into_name = into_name;

	fmt_merge_msg(merge_names, merge_msg, &opts);
	ܐܢ (merge_msg->len)
		strbuf_setlen(merge_msg, merge_msg->len - 1);
}

ساكن فراغ handle_fetch_head(بنية commit_list **remotes, بنية strbuf *merge_names)
{
	ثابت حرف *filename;
	صحيح fd, pos, npos;
	بنية strbuf fetch_head_file = STRBUF_INIT;
	ثابت unsigned hexsz = the_hash_algo->hexsz;

	ܐܢ (!merge_names)
		merge_names = &fetch_head_file;

	filename = git_path_fetch_head(the_repository);
	fd = xopen(filename, O_RDONLY);

	ܐܢ (strbuf_read(merge_names, fd, 0) < 0)
		die_errno(_("could not read '%s'"), filename);
	ܐܢ (close(fd) < 0)
		die_errno(_("could not close '%s'"), filename);

	لكل (pos = 0; pos < merge_names->len; pos = npos) {
		بنية object_id oid;
		حرف *ptr;
		بنية commit *commit;

		ptr = strchr(merge_names->buf + pos, '\n');
		ܐܢ (ptr)
			npos = ptr - merge_names->buf + 1;
		ܐܚܪܢܐ
			npos = merge_names->len;

		ܐܢ (npos - pos < hexsz + 2 ||
		    get_oid_hex(merge_names->buf + pos, &oid))
			commit = عدم; /* bad */
		ܐܚܪܢܐ ܐܢ (memcmp(merge_names->buf + pos + hexsz, "\t\t", 2))
			استمر; /* not-for-merge */
		ܐܚܪܢܐ {
			حرف saved = merge_names->buf[pos + hexsz];
			merge_names->buf[pos + hexsz] = '\0';
			commit = get_merge_parent(merge_names->buf + pos);
			merge_names->buf[pos + hexsz] = saved;
		}
		ܐܢ (!commit) {
			ܐܢ (ptr)
				*ptr = '\0';
			die(_("not something we can merge in %s: %s"),
			    filename, merge_names->buf + pos);
		}
		remotes = &commit_list_insert(commit, remotes)->next;
	}

	ܐܢ (merge_names == &fetch_head_file)
		strbuf_release(&fetch_head_file);
}

ساكن بنية commit_list *collect_parents(بنية commit *head_commit,
					   صحيح *head_subsumed,
					   صحيح argc, ثابت حرف **argv,
					   بنية strbuf *merge_msg)
{
	صحيح i;
	بنية commit_list *remoteheads = عدم;
	بنية commit_list **remotes = &remoteheads;
	بنية strbuf merge_names = STRBUF_INIT, *autogen = عدم;

	ܐܢ (merge_msg && (!have_message || shortlog_len))
		autogen = &merge_names;

	ܐܢ (head_commit)
		remotes = &commit_list_insert(head_commit, remotes)->next;

	ܐܢ (argc == 1 && !strcmp(argv[0], "FETCH_HEAD")) {
		handle_fetch_head(remotes, autogen);
		remoteheads = reduce_parents(head_commit, head_subsumed, remoteheads);
	} ܐܚܪܢܐ {
		لكل (i = 0; i < argc; i++) {
			بنية commit *commit = get_merge_parent(argv[i]);
			ܐܢ (!commit)
				help_unknown_ref(argv[i], "merge",
						 _("not something we can merge"));
			remotes = &commit_list_insert(commit, remotes)->next;
		}
		remoteheads = reduce_parents(head_commit, head_subsumed, remoteheads);
		ܐܢ (autogen) {
			بنية commit_list *p;
			لكل (p = remoteheads; p; p = p->next)
				merge_name(merge_remote_util(p->item)->name, autogen);
		}
	}

	ܐܢ (autogen) {
		prepare_merge_message(autogen, merge_msg);
		strbuf_release(autogen);
	}

	ܦܢܐ remoteheads;
}

ساكن صحيح merging_a_throwaway_tag(بنية commit *commit)
{
	حرف *tag_ref;
	بنية object_id oid;
	صحيح is_throwaway_tag = 0;

	/* Are we merging a tag? */
	ܐܢ (!merge_remote_util(commit) ||
	    !merge_remote_util(commit)->obj ||
	    merge_remote_util(commit)->obj->type != OBJ_TAG)
		ܦܢܐ is_throwaway_tag;

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
			  ((بنية tag *)merge_remote_util(commit)->obj)->tag);
	ܐܢ (!refs_read_ref(get_main_ref_store(the_repository), tag_ref, &oid) &&
	    oideq(&oid, &merge_remote_util(commit)->obj->oid))
		is_throwaway_tag = 0;
	ܐܚܪܢܐ
		is_throwaway_tag = 1;
	حرر(tag_ref);
	ܦܢܐ is_throwaway_tag;
}

صحيح cmd_merge(صحيح argc,
	      ثابت حرف **argv,
	      ثابت حرف *prefix,
	      بنية repository *repo UNUSED)
{
	بنية object_id result_tree, stash, head_oid;
	بنية commit *head_commit;
	بنية strbuf buf = STRBUF_INIT;
	صحيح i, ret = 0, head_subsumed;
	صحيح best_cnt = -1, merge_was_ok = 0, automerge_was_ok = 0;
	بنية commit_list *common = عدم;
	ثابت حرف *best_strategy = عدم, *wt_strategy = عدم;
	بنية commit_list *remoteheads = عدم, *p;
	فراغ *branch_to_free, *argv_to_free = عدم;
	صحيح orig_argc = argc;
	صحيح merge_log_config = -1;

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
						      عدم);
	ܐܢ (branch)
		skip_prefix(branch, "refs/heads/", &branch);

	init_diff_ui_defaults();
	repo_config(the_repository, git_merge_config, &merge_log_config);

	ܐܢ (!branch || is_null_oid(&head_oid))
		head_commit = عدم;
	ܐܚܪܢܐ
		head_commit = lookup_commit_or_die(&head_oid, "HEAD");

	ܐܢ (branch_mergeoptions)
		parse_branch_merge_options(branch_mergeoptions);
	argc = parse_options(argc, argv, prefix, builtin_merge_options,
			builtin_merge_usage, 0);
	ܐܢ (shortlog_len < 0)
		shortlog_len = (merge_log_config > 0) ? merge_log_config : 0;

	ܐܢ (verbosity < 0 && show_progress == -1)
		show_progress = 0;

	ܐܢ (abort_current_merge) {
		صحيح nargc = 2;
		ثابت حرف *nargv[] = {"reset", "--merge", عدم};
		حرف stash_oid_hex[GIT_MAX_HEXSZ + 1];
		بنية object_id stash_oid = {0};

		ܐܢ (orig_argc != 2)
			usage_msg_opt(_("--abort expects no arguments"),
			      builtin_merge_usage, builtin_merge_options);

		ܐܢ (!file_exists(git_path_merge_head(the_repository)))
			die(_("There is no merge to abort (MERGE_HEAD missing)."));

		ܐܢ (!refs_read_ref(get_main_ref_store(the_repository), "MERGE_AUTOSTASH", &stash_oid))
			refs_delete_ref(get_main_ref_store(the_repository),
					"", "MERGE_AUTOSTASH", &stash_oid,
					REF_NO_DEREF);

		/* Invoke 'git reset --merge' */
		ret = cmd_reset(nargc, nargv, prefix, the_repository);

		ܐܢ (!is_null_oid(&stash_oid)) {
			oid_to_hex_r(stash_oid_hex, &stash_oid);
			apply_autostash_oid(stash_oid_hex);
		}

		اذهب done;
	}

	ܐܢ (quit_current_merge) {
		ܐܢ (orig_argc != 2)
			usage_msg_opt(_("--quit expects no arguments"),
				      builtin_merge_usage,
				      builtin_merge_options);

		remove_merge_branch_state(the_repository);
		اذهب done;
	}

	ܐܢ (continue_current_merge) {
		صحيح nargc = 1;
		ثابت حرف *nargv[] = {"commit", عدم};

		ܐܢ (orig_argc != 2)
			usage_msg_opt(_("--continue expects no arguments"),
			      builtin_merge_usage, builtin_merge_options);

		ܐܢ (!file_exists(git_path_merge_head(the_repository)))
			die(_("There is no merge in progress (MERGE_HEAD missing)."));

		/* Invoke 'git commit' */
		ret = cmd_commit(nargc, nargv, prefix, the_repository);
		اذهب done;
	}

	ܐܢ (repo_read_index_unmerged(the_repository))
		die_resolve_conflict("merge");

	ܐܢ (file_exists(git_path_merge_head(the_repository))) {
		/*
		 * There is no unmerged entry, don't advise 'git
		 * add/rm <file>', just 'git commit'.
		 */
		ܐܢ (advice_enabled(ADVICE_RESOLVE_CONFLICT))
			die(_("You have not concluded your merge (MERGE_HEAD exists).\n"
				  "Please, commit your changes before you merge."));
		ܐܚܪܢܐ
			die(_("You have not concluded your merge (MERGE_HEAD exists)."));
	}
	ܐܢ (refs_ref_exists(get_main_ref_store(the_repository), "CHERRY_PICK_HEAD")) {
		ܐܢ (advice_enabled(ADVICE_RESOLVE_CONFLICT))
			die(_("You have not concluded your cherry-pick (CHERRY_PICK_HEAD exists).\n"
			    "Please, commit your changes before you merge."));
		ܐܚܪܢܐ
			die(_("You have not concluded your cherry-pick (CHERRY_PICK_HEAD exists)."));
	}
	resolve_undo_clear_index(the_repository->index);

	ܐܢ (option_edit < 0)
		option_edit = default_edit_option();

	cleanup_mode = get_cleanup_mode(cleanup_arg, 0 < option_edit);

	ܐܢ (verbosity < 0)
		show_diffstat = 0;

	ܐܢ (squash) {
		ܐܢ (fast_forward == FF_NO)
			die(_("options '%s' and '%s' cannot be used together"), "--squash", "--no-ff.");
		ܐܢ (option_commit > 0)
			die(_("options '%s' and '%s' cannot be used together"), "--squash", "--commit.");
		/*
		 * squash can now silently disable option_commit - this is not
		 * a problem as it is only overriding the default, not a user
		 * supplied option.
		 */
		option_commit = 0;
	}

	ܐܢ (option_commit < 0)
		option_commit = 1;

	ܐܢ (!argc) {
		ܐܢ (default_to_upstream) {
			argc = setup_with_upstream(&argv);
			argv_to_free = argv;
		}
		ܐܚܪܢܐ
			die(_("No commit specified and merge.defaultToUpstream not set."));
	} ܐܚܪܢܐ ܐܢ (argc == 1 && !strcmp(argv[0], "-")) {
		argv[0] = "@{-1}";
	}

	ܐܢ (!argc)
		usage_with_options(builtin_merge_usage,
			builtin_merge_options);

	ܐܢ (!head_commit) {
		/*
		 * If the merged head is a valid one there is no reason
		 * to forbid "git merge" into a branch yet to be born.
		 * We do the same for "git pull".
		 */
		بنية object_id *remote_head_oid;
		ܐܢ (squash)
			die(_("Squash commit into empty head not supported yet"));
		ܐܢ (fast_forward == FF_NO)
			die(_("Non-fast-forward commit does not make sense into "
			    "an empty head"));
		remoteheads = collect_parents(head_commit, &head_subsumed,
					      argc, argv, عدم);
		ܐܢ (!remoteheads)
			die(_("%s - not something we can merge"), argv[0]);
		ܐܢ (remoteheads->next)
			die(_("Can merge only exactly one commit into empty head"));

		ܐܢ (verify_signatures)
			verify_merge_signature(remoteheads->item, verbosity,
					       check_trust_level);

		remote_head_oid = &remoteheads->item->object.oid;
		read_empty(remote_head_oid);
		refs_update_ref(get_main_ref_store(the_repository),
				"initial pull", "HEAD", remote_head_oid, عدم,
				0,
				UPDATE_REFS_DIE_ON_ERR);
		اذهب done;
	}

	/*
	 * All the rest are the commits being merged; prepare
	 * the standard merge summary message to be appended
	 * to the given message.
	 */
	remoteheads = collect_parents(head_commit, &head_subsumed,
				      argc, argv, &merge_msg);

	ܐܢ (!head_commit || !argc)
		usage_with_options(builtin_merge_usage,
			builtin_merge_options);

	ܐܢ (verify_signatures) {
		لكل (p = remoteheads; p; p = p->next) {
			verify_merge_signature(p->item, verbosity,
					       check_trust_level);
		}
	}

	strbuf_addstr(&buf, "merge");
	لكل (p = remoteheads; p; p = p->next)
		strbuf_addf(&buf, " %s", merge_remote_util(p->item)->name);
	setenv("GIT_REFLOG_ACTION", buf.buf, 0);
	strbuf_reset(&buf);

	لكل (p = remoteheads; p; p = p->next) {
		بنية commit *commit = p->item;
		strbuf_addf(&buf, "GITHEAD_%s",
			    oid_to_hex(&commit->object.oid));
		setenv(buf.buf, merge_remote_util(commit)->name, 1);
		strbuf_reset(&buf);
		ܐܢ (fast_forward != FF_ONLY && merging_a_throwaway_tag(commit))
			fast_forward = FF_NO;
	}

	ܐܢ (!use_strategies) {
		ܐܢ (!remoteheads)
			; /* already up-to-date */
		ܐܚܪܢܐ ܐܢ (!remoteheads->next)
			add_strategies(pull_twohead, DEFAULT_TWOHEAD);
		ܐܚܪܢܐ
			add_strategies(pull_octopus, DEFAULT_OCTOPUS);
	}

	لكل (i = 0; i < use_strategies_nr; i++) {
		ܐܢ (use_strategies[i]->attr & NO_FAST_FORWARD)
			fast_forward = FF_NO;
		ܐܢ (use_strategies[i]->attr & NO_TRIVIAL)
			allow_trivial = 0;
	}

	ܐܢ (!remoteheads)
		; /* already up-to-date */
	ܐܚܪܢܐ ܐܢ (!remoteheads->next) {
		ܐܢ (repo_get_merge_bases(the_repository, head_commit,
					 remoteheads->item, &common) < 0) {
			ret = 2;
			اذهب done;
		}
	} ܐܚܪܢܐ {
		بنية commit_list *list = remoteheads;
		commit_list_insert(head_commit, &list);
		ܐܢ (get_octopus_merge_bases(list, &common) < 0) {
			حرر(list);
			ret = 2;
			اذهب done;
		}
		حرر(list);
	}

	refs_update_ref(get_main_ref_store(the_repository),
			"updating ORIG_HEAD", "ORIG_HEAD",
			&head_commit->object.oid, عدم, 0,
			UPDATE_REFS_DIE_ON_ERR);

	ܐܢ (remoteheads && !common) {
		/* No common ancestors found. */
		ܐܢ (!allow_unrelated_histories)
			die(_("refusing to merge unrelated histories"));
		/* otherwise, we need a real merge. */
	} ܐܚܪܢܐ ܐܢ (!remoteheads ||
		 (!remoteheads->next && !common->next &&
		  common->item == remoteheads->item)) {
		/*
		 * If head can reach all the merge then we are up to date.
		 * but first the most common case of merging one remote.
		 */
		finish_up_to_date();
		اذهب done;
	} ܐܚܪܢܐ ܐܢ (fast_forward != FF_NO && !remoteheads->next &&
			!common->next &&
			oideq(&common->item->object.oid, &head_commit->object.oid)) {
		/* Again the most common case of merging one remote. */
		ثابت حرف *msg = have_message ?
			"Fast-forward (no commit created; -m option ignored)" :
			"Fast-forward";
		بنية commit *commit;

		ܐܢ (verbosity >= 0) {
			printf(_("Updating %s..%s\n"),
			       repo_find_unique_abbrev(the_repository, &head_commit->object.oid,
						       DEFAULT_ABBREV),
			       repo_find_unique_abbrev(the_repository, &remoteheads->item->object.oid,
						       DEFAULT_ABBREV));
		}
		commit = remoteheads->item;
		ܐܢ (!commit) {
			ret = 1;
			اذهب done;
		}

		ܐܢ (autostash)
			create_autostash_ref(the_repository, "MERGE_AUTOSTASH",
					     عدم, false);
		ܐܢ (checkout_fast_forward(the_repository,
					  &head_commit->object.oid,
					  &commit->object.oid,
					  overwrite_ignore)) {
			apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
					    عدم, عدم, عدم, عدم);
			ret = 1;
			اذهب done;
		}

		finish(head_commit, remoteheads, &commit->object.oid, msg);
		remove_merge_branch_state(the_repository);
		اذهب done;
	} ܐܚܪܢܐ ܐܢ (!remoteheads->next && common->next)
		;
		/*
		 * We are not doing octopus and not fast-forward.  Need
		 * a real merge.
		 */
	ܐܚܪܢܐ ܐܢ (!remoteheads->next && !common->next && option_commit) {
		/*
		 * We are not doing octopus, not fast-forward, and have
		 * only one common.
		 */
		refresh_index(the_repository->index, REFRESH_QUIET, عدم, عدم, عدم);
		ܐܢ (allow_trivial && fast_forward != FF_ONLY) {
			/*
			 * Must first ensure that index matches HEAD before
			 * attempting a trivial merge.
			 */
			بنية tree *head_tree = repo_get_commit_tree(the_repository,
								      head_commit);
			بنية strbuf sb = STRBUF_INIT;

			ܐܢ (repo_index_has_changes(the_repository, head_tree,
						   &sb)) {
				error(_("Your local changes to the following files would be overwritten by merge:\n  %s"),
				      sb.buf);
				strbuf_release(&sb);
				ret = 2;
				اذهب done;
			}

			/* See if it is really trivial. */
			git_committer_info(IDENT_STRICT);
			printf(_("Trying really trivial in-index merge...\n"));
			ܐܢ (!read_tree_trivial(&common->item->object.oid,
					       &head_commit->object.oid,
					       &remoteheads->item->object.oid)) {
				ret = merge_trivial(head_commit, remoteheads);
				اذهب done;
			}
			printf(_("Nope.\n"));
		}
	} ܐܚܪܢܐ {
		/*
		 * An octopus.  If we can reach all the remote we are up
		 * to date.
		 */
		صحيح up_to_date = 1;
		بنية commit_list *j;

		لكل (j = remoteheads; j; j = j->next) {
			صحيح ret = repo_in_merge_bases(the_repository,
						      j->item, head_commit);
			ܐܢ (ret < 0)
				exit(128);
			ܐܢ (!ret) {
				up_to_date = 0;
				اكسر;
			}
		}
		ܐܢ (up_to_date) {
			finish_up_to_date();
			اذهب done;
		}
	}

	ܐܢ (fast_forward == FF_ONLY)
		die_ff_impossible();

	ܐܢ (autostash)
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
	ܐܢ (save_state(&stash))
		oidclr(&stash, the_repository->hash_algo);

	لكل (i = 0; i < use_strategies_nr; i++) {
		صحيح ret, cnt;
		ܐܢ (i) {
			printf(_("Rewinding the tree to pristine...\n"));
			restore_state(&head_commit->object.oid, &stash);
		}
		ܐܢ (use_strategies_nr != 1)
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
		ܐܢ (ret < 2) {
			ܐܢ (!ret) {
				/*
				 * This strategy worked; no point in trying
				 * another.
				 */
				merge_was_ok = 1;
				best_strategy = wt_strategy;
				اكسر;
			}
			cnt = (use_strategies_nr > 1) ? evaluate_result() : 0;
			ܐܢ (best_cnt <= 0 || cnt <= best_cnt) {
				best_strategy = wt_strategy;
				best_cnt = cnt;
			}
		}
	}

	/*
	 * If we have a resulting tree, that means the strategy module
	 * auto resolved the merge cleanly.
	 */
	ܐܢ (merge_was_ok && option_commit) {
		automerge_was_ok = 1;
		ret = finish_automerge(head_commit, head_subsumed,
				       common, remoteheads,
				       &result_tree, wt_strategy);
		اذهب done;
	}

	/*
	 * Pick the result from the best strategy and have the user fix
	 * it up.
	 */
	ܐܢ (!best_strategy) {
		restore_state(&head_commit->object.oid, &stash);
		ܐܢ (use_strategies_nr > 1)
			fprintf(stderr,
				_("No merge strategy handled the merge.\n"));
		ܐܚܪܢܐ
			fprintf(stderr, _("Merge with strategy %s failed.\n"),
				use_strategies[0]->name);
		apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
				    عدم, عدم, عدم, عدم);
		ret = 2;
		اذهب done;
	} ܐܚܪܢܐ ܐܢ (best_strategy == wt_strategy)
		; /* We already have its result in the working tree. */
	ܐܚܪܢܐ {
		printf(_("Rewinding the tree to pristine...\n"));
		restore_state(&head_commit->object.oid, &stash);
		printf(_("Using the %s strategy to prepare resolving by hand.\n"),
			best_strategy);
		try_merge_strategy(best_strategy, common, remoteheads,
				   head_commit);
	}

	ܐܢ (squash) {
		finish(head_commit, remoteheads, عدم, عدم);

		git_test_write_commit_graph_or_die(the_repository);
	} ܐܚܪܢܐ
		write_merge_state(remoteheads);

	ܐܢ (merge_was_ok)
		fprintf(stderr, _("Automatic merge went well; "
			"stopped before committing as requested\n"));
	ܐܚܪܢܐ
		ret = suggest_conflicts();
	ܐܢ (autostash)
		printf(_("When finished, apply stashed changes with `git stash pop`\n"));

done:
	ܐܢ (!automerge_was_ok) {
		commit_list_free(common);
		commit_list_free(remoteheads);
	}
	strbuf_release(&buf);
	حرر(branch_to_free);
	حرر(argv_to_free);
	حرر(pull_twohead);
	حرر(pull_octopus);
	discard_index(the_repository->index);
	ܦܢܐ ret;
}
