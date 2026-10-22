/*
 * Builtin "git merge"
 *
 * Copyright (c) 2008 Miklos Vajna <vmiklos@frugalware.org>
 *
 * Based on git-merge.sh by Junio C Hamano.
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#અન્તર્ભાવ "builtin.h"

#અન્તર્ભાવ "abspath.h"
#અન્તર્ભાવ "advice.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "editor.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "hex.h"
#અન્તર્ભાવ "object-name.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "lockfile.h"
#અન્તર્ભાવ "repository.h"
#અન્તર્ભાવ "run-command.h"
#અન્તર્ભાવ "hook.h"
#અન્તર્ભાવ "diff.h"
#અન્તર્ભાવ "diff-merges.h"
#અન્તર્ભાવ "refs.h"
#અન્તર્ભાવ "refspec.h"
#અન્તર્ભાવ "commit.h"
#અન્તર્ભાવ "diffcore.h"
#અન્તર્ભાવ "path.h"
#અન્તર્ભાવ "revision.h"
#અન્તર્ભાવ "unpack-trees.h"
#અન્તર્ભાવ "cache-tree.h"
#અન્તર્ભાવ "dir.h"
#અન્તર્ભાવ "color.h"
#અન્તર્ભાવ "rerere.h"
#અન્તર્ભાવ "help.h"
#અન્તર્ભાવ "merge.h"
#અન્તર્ભાવ "merge-ort-wrappers.h"
#અન્તર્ભાવ "resolve-undo.h"
#અન્તર્ભાવ "remote.h"
#અન્તર્ભાવ "fmt-merge-msg.h"
#અન્તર્ભાવ "sequencer.h"
#અન્તર્ભાવ "string-list.h"
#અન્તર્ભાવ "tag.h"
#અન્તર્ભાવ "alias.h"
#અન્તર્ભાવ "branch.h"
#અન્તર્ભાવ "commit-reach.h"
#અન્તર્ભાવ "wt-status.h"
#અન્તર્ભાવ "commit-graph.h"

#define DEFAULT_TWOHEAD (1<<0)
#define DEFAULT_OCTOPUS (1<<1)
#define NO_FAST_FORWARD (1<<2)
#define NO_TRIVIAL      (1<<3)

સંરચના strategy {
	અચલ અક્ષર *name;
	unsigned attr;
};

સ્થિર અચલ અક્ષર * અચલ builtin_merge_usage[] = {
	N_("git merge [<options>] [<commit>...]"),
	"git merge --abort",
	"git merge --continue",
	ખાલી
};

#define MERGE_SHOW_DIFFSTAT 1
#define MERGE_SHOW_COMPACTSUMMARY 2

સ્થિર પૂર્ણાંક show_diffstat = MERGE_SHOW_DIFFSTAT, shortlog_len = -1, squash;
સ્થિર પૂર્ણાંક option_commit = -1;
સ્થિર પૂર્ણાંક option_edit = -1;
સ્થિર પૂર્ણાંક allow_trivial = 1, have_message, verify_signatures;
સ્થિર પૂર્ણાંક check_trust_level = 1;
સ્થિર પૂર્ણાંક overwrite_ignore = 1;
સ્થિર સંરચના strbuf merge_સંદેશ = STRBUF_INIT;
સ્થિર સંરચના strategy **use_strategies;
સ્થિર size_t use_strategies_nr, use_strategies_alloc;
સ્થિર સંરચના strvec xopts = STRVEC_INIT;
સ્થિર અચલ અક્ષર *branch;
સ્થિર અક્ષર *branch_mergeoptions;
સ્થિર પૂર્ણાંક verbosity;
સ્થિર પૂર્ણાંક allow_rerere_auto;
સ્થિર પૂર્ણાંક abort_current_merge;
સ્થિર પૂર્ણાંક quit_current_merge;
સ્થિર પૂર્ણાંક continue_current_merge;
સ્થિર પૂર્ણાંક allow_unrelated_histories;
સ્થિર પૂર્ણાંક show_progress = -1;
સ્થિર પૂર્ણાંક default_to_upstream = 1;
સ્થિર પૂર્ણાંક signoff;
સ્થિર અચલ અક્ષર *sign_commit;
સ્થિર પૂર્ણાંક autostash;
સ્થિર પૂર્ણાંક no_verify;
સ્થિર અક્ષર *into_name;

સ્થિર સંરચના strategy all_strategy[] = {
	{ "recursive",  NO_TRIVIAL },
	{ "octopus",    DEFAULT_OCTOPUS },
	{ "ort",        DEFAULT_TWOHEAD | NO_TRIVIAL },
	{ "resolve",    0 },
	{ "ours",       NO_FAST_FORWARD | NO_TRIVIAL },
	{ "subtree",    NO_FAST_FORWARD | NO_TRIVIAL },
};

સ્થિર અક્ષર *pull_twohead, *pull_octopus;

ગણના ff_type {
	FF_NO,
	FF_ALLOW,
	FF_ONLY
};

સ્થિર ગણના ff_type fast_forward = FF_ALLOW;

સ્થિર અક્ષર *cleanup_arg;
સ્થિર ગણના commit_સંદેશ_cleanup_mode cleanup_mode;

સ્થિર પૂર્ણાંક option_parse_message(અચલ સંરચના option *opt,
				અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	સંરચના strbuf *buf = opt->કિંમત;

	જો (unset)
		strbuf_setlen(buf, 0);
	નહીંતર જો (arg) {
		strbuf_addf(buf, "%s%s", buf->len ? "\n\n" : "", arg);
		have_message = 1;
	} નહીંતર
		પાછા error(_("switch `m' requires a value"));
	પાછા 0;
}

સ્થિર ગણના parse_opt_result option_read_message(સંરચના parse_opt_ctx_t *ctx,
						 અચલ સંરચના option *opt,
						 અચલ અક્ષર *arg_not_used,
						 પૂર્ણાંક unset)
{
	સંરચના strbuf *buf = opt->કિંમત;
	અચલ અક્ષર *arg;

	BUG_ON_OPT_ARG(arg_not_used);
	જો (unset)
		BUG("-F cannot be negated");

	જો (ctx->opt) {
		arg = ctx->opt;
		ctx->opt = ખાલી;
	} નહીંતર જો (ctx->argc > 1) {
		ctx->argc--;
		arg = *++ctx->argv;
	} નહીંતર
		પાછા error(_("option `%s' requires a value"), opt->long_name);

	જો (buf->len)
		strbuf_addch(buf, '\n');
	જો (ctx->prefix && !is_absolute_path(arg))
		arg = prefix_filename(ctx->prefix, arg);
	જો (strbuf_read_file(buf, arg, 0) < 0)
		પાછા error(_("could not read file '%s'"), arg);
	have_message = 1;

	પાછા 0;
}

સ્થિર સંરચના strategy *get_strategy(અચલ અક્ષર *name)
{
	પૂર્ણાંક i;
	સંરચના strategy *ret;
	સ્થિર સંરચના cmdnames main_cmds = {0}, other_cmds = {0};
	સ્થિર પૂર્ણાંક loaded;
	અક્ષર *default_strategy = ખાલી;

	જો (!name)
		પાછા ખાલી;

	જો (default_strategy &&
	    !strcmp(default_strategy, "ort") &&
	    !strcmp(name, "recursive")) {
		name = "ort";
	}

	માટે (i = 0; i < ARRAY_SIZE(all_strategy); i++)
		જો (!strcmp(name, all_strategy[i].name))
			પાછા &all_strategy[i];

	જો (!loaded) {
		સંરચના cmdnames not_strategies = {0};
		loaded = 1;

		load_command_list("git-merge-", &main_cmds, &other_cmds);
		માટે (i = 0; i < main_cmds.cnt; i++) {
			પૂર્ણાંક j, found = 0;
			સંરચના cmdname *ent = main_cmds.names[i];
			માટે (j = 0; !found && j < ARRAY_SIZE(all_strategy); j++)
				જો (!xstrncmpz(all_strategy[j].name, ent->name, ent->len))
					found = 1;
			જો (!found)
				add_cmdname(&not_strategies, ent->name, ent->len);
		}
		exclude_cmds(&main_cmds, &not_strategies);

		cmdnames_release(&not_strategies);
	}
	જો (!is_in_cmdlist(&main_cmds, name) && !is_in_cmdlist(&other_cmds, name)) {
		fprintf(stderr, _("Could not find merge strategy '%s'.\n"), name);
		fprintf(stderr, _("Available strategies are:"));
		માટે (i = 0; i < main_cmds.cnt; i++)
			fprintf(stderr, " %s", main_cmds.names[i]->name);
		fprintf(stderr, ".\n");
		જો (other_cmds.cnt) {
			fprintf(stderr, _("Available custom strategies are:"));
			માટે (i = 0; i < other_cmds.cnt; i++)
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
	પાછા ret;
}

સ્થિર શૂન્ય append_strategy(સંરચના strategy *s)
{
	ALLOC_GROW(use_strategies, use_strategies_nr + 1, use_strategies_alloc);
	use_strategies[use_strategies_nr++] = s;
}

સ્થિર પૂર્ણાંક option_parse_strategy(અચલ સંરચના option *opt UNUSED,
				 અચલ અક્ષર *name, પૂર્ણાંક unset)
{
	જો (unset)
		પાછા 0;

	append_strategy(get_strategy(name));
	પાછા 0;
}

સ્થિર પૂર્ણાંક option_parse_compact_summary(અચલ સંરચના option *opt,
					અચલ અક્ષર *name UNUSED, પૂર્ણાંક unset)
{
	પૂર્ણાંક *setting = opt->કિંમત;

	જો (unset)
		*setting = 0;
	નહીંતર
		*setting = MERGE_SHOW_COMPACTSUMMARY;
	પાછા 0;
}

સ્થિર સંરચના option builtin_merge_options[] = {
	OPT_SET_INT('n', ખાલી, &show_diffstat,
		N_("do not show a diffstat at the end of the merge"), 0),
	OPT_BOOL(0, "stat", &show_diffstat,
		N_("show a diffstat at the end of the merge")),
	OPT_BOOL(0, "summary", &show_diffstat, N_("(synonym to --stat)")),
	OPT_CALLBACK_F(0, "compact-summary", &show_diffstat, ખાલી,
		       N_("show a compact-summary at the end of the merge"),
		       PARSE_OPT_NOARG,
		       option_parse_compact_summary),
	{
		.type = OPTION_INTEGER,
		.long_name = "log",
		.કિંમત = &shortlog_len,
		.precision = કદ(shortlog_len),
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
	OPT_CALLBACK('s', "strategy", ખાલી, N_("strategy"),
		N_("merge strategy to use"), option_parse_strategy),
	OPT_STRVEC('X', "strategy-option", &xopts, N_("option=value"),
		N_("option for selected merge strategy")),
	OPT_CALLBACK('m', "message", &merge_સંદેશ, N_("message"),
		N_("merge commit message (for a non-fast-forward merge)"),
		option_parse_message),
	{
		.type = OPTION_LOWLEVEL_CALLBACK,
		.short_name = 'F',
		.long_name = "file",
		.કિંમત = &merge_સંદેશ,
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
		.કિંમત = &sign_commit,
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

સ્થિર પૂર્ણાંક save_state(સંરચના object_id *stash)
{
	પૂર્ણાંક len;
	સંરચના child_process cp = CHILD_PROCESS_INIT;
	સંરચના strbuf buffer = STRBUF_INIT;
	સંરચના lock_file lock_file = LOCK_INIT;
	પૂર્ણાંક fd;
	પૂર્ણાંક rc = -1;

	fd = repo_hold_locked_index(the_repository, &lock_file, 0);
	refresh_index(the_repository->index, REFRESH_QUIET, ખાલી, ખાલી, ખાલી);
	જો (0 <= fd)
		repo_update_index_if_able(the_repository, &lock_file);
	rollback_lock_file(&lock_file);

	strvec_pushl(&cp.args, "stash", "create", ખાલી);
	cp.out = -1;
	cp.git_cmd = 1;

	જો (start_command(&cp))
		die(_("could not run stash."));
	len = strbuf_read(&buffer, cp.out, 1024);
	close(cp.out);

	જો (finish_command(&cp) || len < 0)
		die(_("stash failed"));
	નહીંતર જો (!len)		/* no changes */
		જાઓ out;
	strbuf_setlen(&buffer, buffer.len-1);
	જો (repo_get_oid(the_repository, buffer.buf, stash))
		die(_("not a valid object: %s"), buffer.buf);
	rc = 0;
out:
	strbuf_release(&buffer);
	પાછા rc;
}

સ્થિર શૂન્ય read_empty(અચલ સંરચના object_id *oid)
{
	સંરચના child_process cmd = CHILD_PROCESS_INIT;

	strvec_pushl(&cmd.args, "read-tree", "-m", "-u",
		     empty_વૃક્ષ_oid_hex(the_repository->hash_algo),
		     oid_to_hex(oid), ખાલી);
	cmd.git_cmd = 1;

	જો (run_command(&cmd))
		die(_("read-tree failed"));
}

સ્થિર શૂન્ય reset_hard(અચલ સંરચના object_id *oid)
{
	સંરચના child_process cmd = CHILD_PROCESS_INIT;

	strvec_pushl(&cmd.args, "read-tree", "-v", "--reset", "-u",
		     oid_to_hex(oid), ખાલી);
	cmd.git_cmd = 1;

	જો (run_command(&cmd))
		die(_("read-tree failed"));
}

સ્થિર શૂન્ય restore_state(અચલ સંરચના object_id *head,
			  અચલ સંરચના object_id *stash)
{
	સંરચના child_process cmd = CHILD_PROCESS_INIT;

	reset_hard(head);

	જો (is_null_oid(stash))
		જાઓ refresh_cache;

	strvec_pushl(&cmd.args, "stash", "apply", "--index", "--quiet", ખાલી);
	strvec_ધકેલો(&cmd.args, oid_to_hex(stash));

	/*
	 * It is OK to ignore error here, for example when there was
	 * nothing to restore.
	 */
	cmd.git_cmd = 1;
	run_command(&cmd);

refresh_cache:
	discard_index(the_repository->index);
	જો (repo_read_index(the_repository) < 0)
		die(_("could not read index"));
}

/* This is called when no merge was necessary. */
સ્થિર શૂન્ય finish_up_to_date(શૂન્ય)
{
	જો (verbosity >= 0) {
		જો (squash)
			puts(_("Already up to date. (nothing to squash)"));
		નહીંતર
			puts(_("Already up to date."));
	}
	remove_merge_branch_state(the_repository);
}

સ્થિર શૂન્ય squash_message(સંરચના commit *commit, સંરચના commit_list *remoteheads)
{
	સંરચના rev_info rev;
	સંરચના strbuf out = STRBUF_INIT;
	સંરચના commit_list *j;
	સંરચના pretty_print_context ctx = {0};

	printf(_("Squash commit -- not updating HEAD\n"));

	repo_init_revisions(the_repository, &rev, ખાલી);
	diff_merges_suppress(&rev);
	rev.commit_format = CMIT_FMT_MEDIUM;

	commit->object.flags |= UNINTERESTING;
	add_pending_object(&rev, &commit->object, ખાલી);

	માટે (j = remoteheads; j; j = j->next)
		add_pending_object(&rev, &j->item->object, ખાલી);

	setup_revisions(0, ખાલી, &rev, ખાલી);
	જો (prepare_revision_walk(&rev))
		die(_("revision walk setup failed"));

	ctx.abbrev = rev.abbrev;
	ctx.date_mode = rev.date_mode;
	ctx.fmt = rev.commit_format;

	strbuf_addstr(&out, "Squashed commit of the following:\n");
	જ્યારે ((commit = get_revision(&rev)) != ખાલી) {
		strbuf_addch(&out, '\n');
		strbuf_addf(&out, "commit %s\n",
			oid_to_hex(&commit->object.oid));
		pretty_print_commit(&ctx, commit, &out);
	}
	write_file_buf(git_path_squash_સંદેશ(the_repository), out.buf, out.len);
	strbuf_release(&out);
	release_revisions(&rev);
}

સ્થિર શૂન્ય finish(સંરચના commit *head_commit,
		   સંરચના commit_list *remoteheads,
		   અચલ સંરચના object_id *new_head, અચલ અક્ષર *સંદેશ)
{
	સંરચના strbuf reflog_message = STRBUF_INIT;
	અચલ સંરચના object_id *head = &head_commit->object.oid;

	જો (!સંદેશ)
		strbuf_addstr(&reflog_message, getenv("GIT_REFLOG_ACTION"));
	નહીંતર {
		જો (verbosity >= 0)
			printf("%s\n", સંદેશ);
		strbuf_addf(&reflog_message, "%s: %s",
			getenv("GIT_REFLOG_ACTION"), સંદેશ);
	}
	જો (squash) {
		squash_message(head_commit, remoteheads);
	} નહીંતર {
		જો (verbosity >= 0 && !merge_સંદેશ.len)
			printf(_("No merge message -- not updating HEAD\n"));
		નહીંતર {
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
	જો (new_head && show_diffstat) {
		સંરચના diff_options opts;
		repo_diff_setup(the_repository, &opts);
		init_diffstat_widths(&opts);

		પસંદગી (show_diffstat) {
		સ્થિતિ MERGE_SHOW_DIFFSTAT: /* 1 */
			opts.output_format |=
				DIFF_FORMAT_SUMMARY | DIFF_FORMAT_DIFFSTAT;
			તોડો;
		સ્થિતિ MERGE_SHOW_COMPACTSUMMARY: /* 2 */
			opts.output_format |= DIFF_FORMAT_DIFFSTAT;
			opts.flags.stat_with_summary = 1;
			તોડો;
		મૂળભૂત:
			તોડો;
		}
		opts.detect_rename = DIFF_DETECT_RENAME;
		diff_setup_done(&opts);
		diff_વૃક્ષ_oid(head, new_head, "", &opts);
		diffcore_std(&opts);
		diff_flush(&opts);
	}

	/* Run a post-merge hook */
	run_hooks_l(the_repository, "post-merge", squash ? "1" : "0", ખાલી);

	જો (new_head)
		apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
				    ખાલી, ખાલી, ખાલી, ખાલી);
	strbuf_release(&reflog_message);
}

/* Get the name for the merge commit's message. */
સ્થિર શૂન્ય merge_name(અચલ અક્ષર *remote, સંરચના strbuf *સંદેશ)
{
	સંરચના commit *remote_head;
	સંરચના object_id branch_head;
	સંરચના strbuf bname = STRBUF_INIT;
	સંરચના merge_remote_desc *desc;
	અચલ અક્ષર *ptr;
	અક્ષર *found_ref = ખાલી;
	પૂર્ણાંક len, early;

	copy_branchname(the_repository, &bname, remote, 0);
	remote = bname.buf;

	oidclr(&branch_head, the_repository->hash_algo);
	remote_head = get_merge_parent(remote);
	જો (!remote_head)
		die(_("'%s' does not point to a commit"), remote);

	જો (repo_dwim_ref(the_repository, remote, strlen(remote), &branch_head,
			  &found_ref, 0) > 0) {
		જો (starts_with(found_ref, "refs/heads/")) {
			strbuf_addf(સંદેશ, "%s\t\tbranch '%s' of .\n",
				    oid_to_hex(&branch_head), remote);
			જાઓ cleanup;
		}
		જો (starts_with(found_ref, "refs/tags/")) {
			strbuf_addf(સંદેશ, "%s\t\ttag '%s' of .\n",
				    oid_to_hex(&branch_head), remote);
			જાઓ cleanup;
		}
		જો (starts_with(found_ref, "refs/remotes/")) {
			strbuf_addf(સંદેશ, "%s\t\tremote-tracking branch '%s' of .\n",
				    oid_to_hex(&branch_head), remote);
			જાઓ cleanup;
		}
	}

	/* See if remote matches <name>^^^.. or <name>~<number> */
	માટે (len = 0, ptr = remote + strlen(remote);
	     remote < ptr && ptr[-1] == '^';
	     ptr--)
		len++;
	જો (len)
		early = 1;
	નહીંતર {
		early = 0;
		ptr = strrchr(remote, '~');
		જો (ptr) {
			પૂર્ણાંક seen_nonzero = 0;

			len++; /* count ~ */
			જ્યારે (*++ptr && isdigit(*ptr)) {
				seen_nonzero |= (*ptr != '0');
				len++;
			}
			જો (*ptr)
				len = 0; /* not ...~<number> */
			નહીંતર જો (seen_nonzero)
				early = 1;
			નહીંતર જો (len == 1)
				early = 1; /* "name~" is "name~1"! */
		}
	}
	જો (len) {
		સંરચના strbuf truname = STRBUF_INIT;
		strbuf_addf(&truname, "refs/heads/%s", remote);
		strbuf_setlen(&truname, truname.len - len);
		જો (refs_ref_exists(get_main_ref_store(the_repository), truname.buf)) {
			strbuf_addf(સંદેશ,
				    "%s\t\tbranch '%s'%s of .\n",
				    oid_to_hex(&remote_head->object.oid),
				    truname.buf + 11,
				    (early ? " (early part)" : ""));
			strbuf_release(&truname);
			જાઓ cleanup;
		}
		strbuf_release(&truname);
	}

	desc = merge_remote_util(remote_head);
	જો (desc && desc->obj && desc->obj->type == OBJ_TAG) {
		strbuf_addf(સંદેશ, "%s\t\t%s '%s'\n",
			    oid_to_hex(&desc->obj->oid),
			    type_name(desc->obj->type),
			    remote);
		જાઓ cleanup;
	}

	strbuf_addf(સંદેશ, "%s\t\tcommit '%s'\n",
		oid_to_hex(&remote_head->object.oid), remote);
cleanup:
	મુક્ત(found_ref);
	strbuf_release(&bname);
}

સ્થિર શૂન્ય parse_branch_merge_options(અક્ષર *bmo)
{
	અચલ અક્ષર **argv;
	પૂર્ણાંક argc;

	જો (!bmo)
		પાછા;
	argc = split_cmdline(bmo, &argv);
	જો (argc < 0)
		die(_("Bad branch.%s.mergeoptions string: %s"), branch,
		    _(split_cmdline_strerror(argc)));
	REALLOC_ARRAY(argv, argc + 2);
	MOVE_ARRAY(argv + 1, argv, argc + 1);
	argc++;
	argv[0] = "branch.*.mergeoptions";
	parse_options(argc, argv, ખાલી, builtin_merge_options,
		      builtin_merge_usage, 0);
	મુક્ત(argv);
}

સ્થિર પૂર્ણાંક git_merge_config(અચલ અક્ષર *k, અચલ અક્ષર *v,
			    અચલ સંરચના config_context *ctx, શૂન્ય *cb)
{
	પૂર્ણાંક status;
	અચલ અક્ષર *str;

	જો (branch &&
	    skip_prefix(k, "branch.", &str) &&
	    skip_prefix(str, branch, &str) &&
	    !strcmp(str, ".mergeoptions")) {
		મુક્ત(branch_mergeoptions);
		branch_mergeoptions = xstrdup(v);
		પાછા 0;
	}

	જો (!strcmp(k, "merge.diffstat") || !strcmp(k, "merge.stat")) {
		પૂર્ણાંક val = git_parse_maybe_bool_text(v);
		પસંદગી (val) {
		સ્થિતિ 0:
			show_diffstat = 0;
			તોડો;
		સ્થિતિ 1:
			show_diffstat = MERGE_SHOW_DIFFSTAT;
			તોડો;
		મૂળભૂત:
			જો (!strcmp(v, "compact"))
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
			નહીંતર
				/*
				 * A setting from a future?  It is not an
				 * error grave enough to fail the command.
				 * proceed using the default one.
				 */
				show_diffstat = MERGE_SHOW_DIFFSTAT;
			તોડો;
		}
	} નહીંતર જો (!strcmp(k, "merge.verifysignatures")) {
		verify_signatures = git_config_bool(k, v);
	} નહીંતર જો (!strcmp(k, "pull.twohead")) {
		FREE_AND_NULL(pull_twohead);
		પાછા git_config_string(&pull_twohead, k, v);
	} નહીંતર જો (!strcmp(k, "pull.octopus")) {
		FREE_AND_NULL(pull_octopus);
		પાછા git_config_string(&pull_octopus, k, v);
	} નહીંતર જો (!strcmp(k, "commit.cleanup")) {
		પાછા git_config_string(&cleanup_arg, k, v);
	} નહીંતર જો (!strcmp(k, "merge.ff")) {
		પૂર્ણાંક boolval = git_parse_maybe_bool(v);
		જો (0 <= boolval) {
			fast_forward = boolval ? FF_ALLOW : FF_NO;
		} નહીંતર જો (v && !strcmp(v, "only")) {
			fast_forward = FF_ONLY;
		} /* do not barf on values from future versions of git */
		પાછા 0;
	} નહીંતર જો (!strcmp(k, "merge.defaulttoupstream")) {
		default_to_upstream = git_config_bool(k, v);
		પાછા 0;
	} નહીંતર જો (!strcmp(k, "commit.gpgsign")) {
		sign_commit = git_config_bool(k, v) ? "" : ખાલી;
		પાછા 0;
	} નહીંતર જો (!strcmp(k, "gpg.mintrustlevel")) {
		check_trust_level = 0;
	} નહીંતર જો (!strcmp(k, "merge.autostash")) {
		autostash = git_config_bool(k, v);
		પાછા 0;
	}

	status = fmt_merge_સંદેશ_config(k, v, ctx, cb);
	જો (status)
		પાછા status;
	પાછા git_diff_ui_config(k, v, ctx, cb);
}

સ્થિર પૂર્ણાંક read_વૃક્ષ_trivial(સંરચના object_id *common, સંરચના object_id *head,
			     સંરચના object_id *one)
{
	પૂર્ણાંક i, nr_trees = 0;
	સંરચના વૃક્ષ *trees[MAX_UNPACK_TREES];
	સંરચના વૃક્ષ_desc t[MAX_UNPACK_TREES];
	સંરચના unpack_trees_options opts;

	memset(&opts, 0, કદ(opts));
	opts.head_idx = 2;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;
	opts.update = 1;
	opts.verbose_update = 1;
	opts.trivial_merges_only = 1;
	opts.merge = 1;
	opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
	trees[nr_trees] = repo_parse_વૃક્ષ_indirect(the_repository, common);
	જો (!trees[nr_trees++])
		પાછા -1;
	trees[nr_trees] = repo_parse_વૃક્ષ_indirect(the_repository, head);
	જો (!trees[nr_trees++])
		પાછા -1;
	trees[nr_trees] = repo_parse_વૃક્ષ_indirect(the_repository, one);
	જો (!trees[nr_trees++])
		પાછા -1;
	opts.fn = threeway_merge;
	cache_વૃક્ષ_free(&the_repository->index->cache_વૃક્ષ);
	માટે (i = 0; i < nr_trees; i++) {
		repo_parse_વૃક્ષ(the_repository, trees[i]);
		init_વૃક્ષ_desc(t+i, &trees[i]->object.oid,
			       trees[i]->buffer, trees[i]->size);
	}
	જો (unpack_trees(nr_trees, t, &opts))
		પાછા -1;
	પાછા 0;
}

સ્થિર શૂન્ય write_વૃક્ષ_trivial(સંરચના object_id *oid)
{
	જો (write_index_as_વૃક્ષ(oid, the_repository->index,
				repo_get_index_file(the_repository),
				0, ખાલી))
		die(_("git write-tree failed to write a tree"));
}

સ્થિર પૂર્ણાંક try_merge_strategy(અચલ અક્ષર *strategy, સંરચના commit_list *common,
			      સંરચના commit_list *remoteheads,
			      સંરચના commit *head)
{
	અચલ અક્ષર *head_arg = "HEAD";

	જો (repo_refresh_and_write_index(the_repository, REFRESH_QUIET,
					 SKIP_IF_UNCHANGED, 0, ખાલી, ખાલી,
					 ખાલી) < 0)
		die(_("Unable to write index."));

	જો (!strcmp(strategy, "recursive") || !strcmp(strategy, "subtree") ||
	    !strcmp(strategy, "ort")) {
		સંરચના lock_file lock = LOCK_INIT;
		પૂર્ણાંક clean, x;
		સંરચના commit *result;
		સંરચના commit_list *reversed = ખાલી;
		સંરચના merge_options o;
		સંરચના commit_list *j;

		જો (remoteheads->next) {
			error(_("Not handling anything other than two heads merge."));
			પાછા 2;
		}

		init_ui_merge_options(&o, the_repository);
		જો (!strcmp(strategy, "subtree"))
			o.subtree_shift = "";

		o.show_rename_progress =
			show_progress == -1 ? isatty(2) : show_progress;

		માટે (x = 0; x < xopts.nr; x++)
			જો (parse_merge_opt(&o, xopts.v[x]))
				die(_("unknown strategy option: -X%s"), xopts.v[x]);

		o.branch1 = head_arg;
		o.branch2 = merge_remote_util(remoteheads->item)->name;

		માટે (j = common; j; j = j->next)
			commit_list_ઉમેરો(j->item, &reversed);

		repo_hold_locked_index(the_repository, &lock,
				       LOCK_DIE_ON_ERROR);
		clean = merge_ort_recursive(&o, head, remoteheads->item,
					    reversed, &result);
		commit_list_free(reversed);
		strbuf_release(&o.obuf);

		જો (clean < 0) {
			rollback_lock_file(&lock);
			પાછા 2;
		}
		જો (write_locked_index(the_repository->index, &lock,
				       COMMIT_LOCK | SKIP_IF_UNCHANGED))
			die(_("unable to write %s"), repo_get_index_file(the_repository));
		પાછા clean ? 0 : 1;
	} નહીંતર {
		પાછા try_merge_command(the_repository,
					 strategy, xopts.nr, xopts.v,
					 common, head_arg, remoteheads);
	}
}

સ્થિર શૂન્ય ગણતરી_diff_files(સંરચના diff_queue_struct *q,
			     સંરચના diff_options *opt UNUSED, શૂન્ય *data)
{
	પૂર્ણાંક *ગણતરી = data;

	(*ગણતરી) += q->nr;
}

સ્થિર પૂર્ણાંક ગણતરી_unmerged_entries(શૂન્ય)
{
	પૂર્ણાંક i, ret = 0;

	માટે (i = 0; i < the_repository->index->cache_nr; i++)
		જો (ce_stage(the_repository->index->cache[i]))
			ret++;

	પાછા ret;
}

સ્થિર શૂન્ય add_strategies(અચલ અક્ષર *string, unsigned attr)
{
	પૂર્ણાંક i;

	જો (string) {
		સંરચના string_list list = STRING_LIST_INIT_DUP;
		સંરચના string_list_item *item;
		string_list_split(&list, string, " ", -1);
		for_each_string_list_item(item, &list)
			append_strategy(get_strategy(item->string));
		string_list_clear(&list, 0);
		પાછા;
	}
	માટે (i = 0; i < ARRAY_SIZE(all_strategy); i++)
		જો (all_strategy[i].attr & attr)
			append_strategy(&all_strategy[i]);

}

સ્થિર શૂન્ય read_merge_સંદેશ(સંરચના strbuf *સંદેશ)
{
	અચલ અક્ષર *filename = git_path_merge_સંદેશ(the_repository);
	strbuf_reset(સંદેશ);
	જો (strbuf_read_file(સંદેશ, filename, 0) < 0)
		die_errno(_("Could not read from '%s'"), filename);
}

સ્થિર શૂન્ય write_merge_state(સંરચના commit_list *);
સ્થિર શૂન્ય abort_commit(સંરચના commit_list *remoteheads, અચલ અક્ષર *err_સંદેશ)
{
	જો (err_સંદેશ)
		error("%s", err_સંદેશ);
	fprintf(stderr,
		_("Not committing merge; use 'git commit' to complete the merge.\n"));
	write_merge_state(remoteheads);
	exit(1);
}

સ્થિર અચલ અક્ષર merge_editor_comment[] =
N_("Please enter a commit message to explain why this merge is necessary,\n"
   "especially if it merges an updated upstream into a topic branch.\n"
   "\n");

સ્થિર અચલ અક્ષર scissors_editor_comment[] =
N_("An empty message aborts the commit.\n");

સ્થિર અચલ અક્ષર no_scissors_editor_comment[] =
N_("Lines starting with '%s' will be ignored, and an empty message aborts\n"
   "the commit.\n");

સ્થિર શૂન્ય write_merge_heads(સંરચના commit_list *);
સ્થિર શૂન્ય prepare_to_commit(સંરચના commit_list *remoteheads)
{
	સંરચના strbuf સંદેશ = STRBUF_INIT;
	અચલ અક્ષર *index_file = repo_get_index_file(the_repository);

	જો (!no_verify) {
		પૂર્ણાંક invoked_hook;

		જો (run_commit_hook(0 < option_edit, index_file, &invoked_hook,
				    "pre-merge-commit", ખાલી))
			abort_commit(remoteheads, ખાલી);
		/*
		 * Re-read the index as pre-merge-commit hook could have updated it,
		 * and write it out as a tree.  We must do this before we invoke
		 * the editor and after we invoke run_status above.
		 */
		જો (invoked_hook)
			discard_index(the_repository->index);
	}
	read_index_from(the_repository->index, index_file,
			repo_get_git_dir(the_repository));
	strbuf_addbuf(&સંદેશ, &merge_સંદેશ);
	જો (squash)
		BUG("the control must not reach here under --squash");
	જો (0 < option_edit) {
		strbuf_addch(&સંદેશ, '\n');
		જો (cleanup_mode == COMMIT_સંદેશ_CLEANUP_SCISSORS) {
			wt_status_append_cut_લીટી(&સંદેશ);
			strbuf_commented_addf(&સંદેશ, comment_લીટી_str, "\n");
		}
		strbuf_commented_addf(&સંદેશ, comment_લીટી_str,
				      _(merge_editor_comment));
		જો (cleanup_mode == COMMIT_સંદેશ_CLEANUP_SCISSORS)
			strbuf_commented_addf(&સંદેશ, comment_લીટી_str,
					      _(scissors_editor_comment));
		નહીંતર
			strbuf_commented_addf(&સંદેશ, comment_લીટી_str,
				_(no_scissors_editor_comment), comment_લીટી_str);
	}
	જો (signoff)
		append_signoff(&સંદેશ, ignored_log_message_bytes(સંદેશ.buf, સંદેશ.len), 0);
	write_merge_heads(remoteheads);
	write_file_buf(git_path_merge_સંદેશ(the_repository), સંદેશ.buf, સંદેશ.len);
	જો (run_commit_hook(0 < option_edit, repo_get_index_file(the_repository),
			    ખાલી, "prepare-commit-msg",
			    git_path_merge_સંદેશ(the_repository), "merge", ખાલી))
		abort_commit(remoteheads, ખાલી);
	જો (0 < option_edit) {
		જો (launch_editor(git_path_merge_સંદેશ(the_repository), ખાલી, ખાલી))
			abort_commit(remoteheads, ખાલી);
	}

	જો (!no_verify && run_commit_hook(0 < option_edit, repo_get_index_file(the_repository),
					  ખાલી, "commit-msg",
					  git_path_merge_સંદેશ(the_repository), ખાલી))
		abort_commit(remoteheads, ખાલી);

	read_merge_સંદેશ(&સંદેશ);
	cleanup_message(&સંદેશ, cleanup_mode, 0);
	જો (!સંદેશ.len)
		abort_commit(remoteheads, _("Empty commit message."));
	strbuf_release(&merge_સંદેશ);
	strbuf_addbuf(&merge_સંદેશ, &સંદેશ);
	strbuf_release(&સંદેશ);
}

સ્થિર પૂર્ણાંક merge_trivial(સંરચના commit *head, સંરચના commit_list *remoteheads)
{
	સંરચના object_id result_વૃક્ષ, result_commit;
	સંરચના commit_list *parents = ખાલી, **pptr = &parents;

	જો (repo_refresh_and_write_index(the_repository, REFRESH_QUIET,
					 SKIP_IF_UNCHANGED, 0, ખાલી, ખાલી,
					 ખાલી) < 0)
		પાછા error(_("Unable to write index."));

	write_વૃક્ષ_trivial(&result_વૃક્ષ);
	printf(_("Wonderful.\n"));
	pptr = commit_list_append(head, pptr);
	pptr = commit_list_append(remoteheads->item, pptr);
	prepare_to_commit(remoteheads);
	જો (commit_વૃક્ષ(merge_સંદેશ.buf, merge_સંદેશ.len, &result_વૃક્ષ, parents,
			&result_commit, ખાલી, sign_commit))
		die(_("failed to write commit object"));
	finish(head, remoteheads, &result_commit, "In-index merge");

	remove_merge_branch_state(the_repository);
	commit_list_free(parents);
	પાછા 0;
}

સ્થિર પૂર્ણાંક finish_automerge(સંરચના commit *head,
			    પૂર્ણાંક head_subsumed,
			    સંરચના commit_list *common,
			    સંરચના commit_list *remoteheads,
			    સંરચના object_id *result_વૃક્ષ,
			    અચલ અક્ષર *wt_strategy)
{
	સંરચના commit_list *parents = ખાલી;
	સંરચના strbuf buf = STRBUF_INIT;
	સંરચના object_id result_commit;

	write_વૃક્ષ_trivial(result_વૃક્ષ);
	commit_list_free(common);
	parents = remoteheads;
	જો (!head_subsumed || fast_forward == FF_NO)
		commit_list_ઉમેરો(head, &parents);
	prepare_to_commit(remoteheads);
	જો (commit_વૃક્ષ(merge_સંદેશ.buf, merge_સંદેશ.len, result_વૃક્ષ, parents,
			&result_commit, ખાલી, sign_commit))
		die(_("failed to write commit object"));
	strbuf_addf(&buf, "Merge made by the '%s' strategy.", wt_strategy);
	finish(head, remoteheads, &result_commit, buf.buf);

	strbuf_release(&buf);
	remove_merge_branch_state(the_repository);
	commit_list_free(parents);
	પાછા 0;
}

સ્થિર પૂર્ણાંક suggest_conflicts(શૂન્ય)
{
	અચલ અક્ષર *filename;
	FILE *fp;
	સંરચના strbuf msgbuf = STRBUF_INIT;

	filename = git_path_merge_સંદેશ(the_repository);
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
	પાછા 1;
}

સ્થિર પૂર્ણાંક evaluate_result(શૂન્ય)
{
	પૂર્ણાંક cnt = 0;
	સંરચના rev_info rev;

	/* Check how many files differ. */
	repo_init_revisions(the_repository, &rev, "");
	setup_revisions(0, ખાલી, &rev, ખાલી);
	rev.diffopt.output_format |=
		DIFF_FORMAT_CALLBACK;
	rev.diffopt.format_callback = ગણતરી_diff_files;
	rev.diffopt.format_callback_data = &cnt;
	run_diff_files(&rev, 0);

	/*
	 * Check how many unmerged entries are
	 * there.
	 */
	cnt += ગણતરી_unmerged_entries();

	release_revisions(&rev);
	પાછા cnt;
}

/*
 * Pretend as if the user told us to merge with the remote-tracking
 * branch we have for the upstream of the current branch
 */
સ્થિર પૂર્ણાંક setup_with_upstream(અચલ અક્ષર ***argv)
{
	સંરચના branch *branch = branch_get(ખાલી);
	પૂર્ણાંક i;
	અચલ અક્ષર **args;

	જો (!branch)
		die(_("No current branch."));
	જો (!branch->remote_name)
		die(_("No remote for the current branch."));
	જો (!branch->merge_nr)
		die(_("No default upstream defined for the current branch."));

	args = xcalloc(st_add(branch->merge_nr, 1), કદ(અક્ષર *));
	માટે (i = 0; i < branch->merge_nr; i++) {
		જો (!branch->merge[i]->dst)
			die(_("No remote-tracking branch for %s from %s"),
			    branch->merge[i]->src, branch->remote_name);
		args[i] = branch->merge[i]->dst;
	}
	args[i] = ખાલી;
	*argv = args;
	પાછા i;
}

સ્થિર શૂન્ય write_merge_heads(સંરચના commit_list *remoteheads)
{
	સંરચના commit_list *j;
	સંરચના strbuf buf = STRBUF_INIT;

	માટે (j = remoteheads; j; j = j->next) {
		સંરચના object_id *oid;
		સંરચના commit *c = j->item;
		સંરચના merge_remote_desc *desc;

		desc = merge_remote_util(c);
		જો (desc && desc->obj) {
			oid = &desc->obj->oid;
		} નહીંતર {
			oid = &c->object.oid;
		}
		strbuf_addf(&buf, "%s\n", oid_to_hex(oid));
	}
	write_file_buf(git_path_merge_head(the_repository), buf.buf, buf.len);

	strbuf_reset(&buf);
	જો (fast_forward == FF_NO)
		strbuf_addstr(&buf, "no-ff");
	write_file_buf(git_path_merge_mode(the_repository), buf.buf, buf.len);
	strbuf_release(&buf);
}

સ્થિર શૂન્ય write_merge_state(સંરચના commit_list *remoteheads)
{
	write_merge_heads(remoteheads);
	strbuf_addch(&merge_સંદેશ, '\n');
	write_file_buf(git_path_merge_સંદેશ(the_repository), merge_સંદેશ.buf,
		       merge_સંદેશ.len);
}

સ્થિર પૂર્ણાંક default_edit_option(શૂન્ય)
{
	સ્થિર અચલ અક્ષર name[] = "GIT_MERGE_AUTOEDIT";
	અચલ અક્ષર *e = getenv(name);
	સંરચના stat st_stdin, st_stdout;

	જો (have_message)
		/* an explicit -m msg without --[no-]edit */
		પાછા 0;

	જો (e) {
		પૂર્ણાંક v = git_parse_maybe_bool(e);
		જો (v < 0)
			die(_("Bad value '%s' in environment '%s'"), e, name);
		પાછા v;
	}

	/* Use editor if stdin and stdout are the same and is a tty */
	પાછા (!fstat(0, &st_stdin) &&
		!fstat(1, &st_stdout) &&
		isatty(0) && isatty(1) &&
		st_stdin.st_dev == st_stdout.st_dev &&
		st_stdin.st_ino == st_stdout.st_ino &&
		st_stdin.st_mode == st_stdout.st_mode);
}

સ્થિર સંરચના commit_list *reduce_parents(સંરચના commit *head_commit,
					  પૂર્ણાંક *head_subsumed,
					  સંરચના commit_list *remoteheads)
{
	સંરચના commit_list *parents, **remotes;

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

	remoteheads = ખાલી;
	remotes = &remoteheads;
	જ્યારે (parents) {
		સંરચના commit *commit = કાઢો_commit(&parents);
		જો (commit == head_commit)
			*head_subsumed = 0;
		નહીંતર
			remotes = &commit_list_ઉમેરો(commit, remotes)->next;
	}
	પાછા remoteheads;
}

સ્થિર શૂન્ય prepare_merge_message(સંરચના strbuf *merge_names, સંરચના strbuf *merge_સંદેશ)
{
	સંરચના fmt_merge_સંદેશ_opts opts;

	memset(&opts, 0, કદ(opts));
	opts.add_title = !have_message;
	opts.shortlog_len = shortlog_len;
	opts.credit_people = (0 < option_edit);
	opts.into_name = into_name;

	fmt_merge_સંદેશ(merge_names, merge_સંદેશ, &opts);
	જો (merge_સંદેશ->len)
		strbuf_setlen(merge_સંદેશ, merge_સંદેશ->len - 1);
}

સ્થિર શૂન્ય handle_fetch_head(સંરચના commit_list **remotes, સંરચના strbuf *merge_names)
{
	અચલ અક્ષર *filename;
	પૂર્ણાંક fd, pos, npos;
	સંરચના strbuf fetch_head_file = STRBUF_INIT;
	અચલ unsigned hexsz = the_hash_algo->hexsz;

	જો (!merge_names)
		merge_names = &fetch_head_file;

	filename = git_path_fetch_head(the_repository);
	fd = xopen(filename, O_RDONLY);

	જો (strbuf_read(merge_names, fd, 0) < 0)
		die_errno(_("could not read '%s'"), filename);
	જો (close(fd) < 0)
		die_errno(_("could not close '%s'"), filename);

	માટે (pos = 0; pos < merge_names->len; pos = npos) {
		સંરચના object_id oid;
		અક્ષર *ptr;
		સંરચના commit *commit;

		ptr = strchr(merge_names->buf + pos, '\n');
		જો (ptr)
			npos = ptr - merge_names->buf + 1;
		નહીંતર
			npos = merge_names->len;

		જો (npos - pos < hexsz + 2 ||
		    get_oid_hex(merge_names->buf + pos, &oid))
			commit = ખાલી; /* bad */
		નહીંતર જો (memcmp(merge_names->buf + pos + hexsz, "\t\t", 2))
			ચાલુ; /* not-for-merge */
		નહીંતર {
			અક્ષર saved = merge_names->buf[pos + hexsz];
			merge_names->buf[pos + hexsz] = '\0';
			commit = get_merge_parent(merge_names->buf + pos);
			merge_names->buf[pos + hexsz] = saved;
		}
		જો (!commit) {
			જો (ptr)
				*ptr = '\0';
			die(_("not something we can merge in %s: %s"),
			    filename, merge_names->buf + pos);
		}
		remotes = &commit_list_ઉમેરો(commit, remotes)->next;
	}

	જો (merge_names == &fetch_head_file)
		strbuf_release(&fetch_head_file);
}

સ્થિર સંરચના commit_list *collect_parents(સંરચના commit *head_commit,
					   પૂર્ણાંક *head_subsumed,
					   પૂર્ણાંક argc, અચલ અક્ષર **argv,
					   સંરચના strbuf *merge_સંદેશ)
{
	પૂર્ણાંક i;
	સંરચના commit_list *remoteheads = ખાલી;
	સંરચના commit_list **remotes = &remoteheads;
	સંરચના strbuf merge_names = STRBUF_INIT, *autogen = ખાલી;

	જો (merge_સંદેશ && (!have_message || shortlog_len))
		autogen = &merge_names;

	જો (head_commit)
		remotes = &commit_list_ઉમેરો(head_commit, remotes)->next;

	જો (argc == 1 && !strcmp(argv[0], "FETCH_HEAD")) {
		handle_fetch_head(remotes, autogen);
		remoteheads = reduce_parents(head_commit, head_subsumed, remoteheads);
	} નહીંતર {
		માટે (i = 0; i < argc; i++) {
			સંરચના commit *commit = get_merge_parent(argv[i]);
			જો (!commit)
				help_unknown_ref(argv[i], "merge",
						 _("not something we can merge"));
			remotes = &commit_list_ઉમેરો(commit, remotes)->next;
		}
		remoteheads = reduce_parents(head_commit, head_subsumed, remoteheads);
		જો (autogen) {
			સંરચના commit_list *p;
			માટે (p = remoteheads; p; p = p->next)
				merge_name(merge_remote_util(p->item)->name, autogen);
		}
	}

	જો (autogen) {
		prepare_merge_message(autogen, merge_સંદેશ);
		strbuf_release(autogen);
	}

	પાછા remoteheads;
}

સ્થિર પૂર્ણાંક merging_a_throwaway_tag(સંરચના commit *commit)
{
	અક્ષર *tag_ref;
	સંરચના object_id oid;
	પૂર્ણાંક is_throwaway_tag = 0;

	/* Are we merging a tag? */
	જો (!merge_remote_util(commit) ||
	    !merge_remote_util(commit)->obj ||
	    merge_remote_util(commit)->obj->type != OBJ_TAG)
		પાછા is_throwaway_tag;

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
			  ((સંરચના tag *)merge_remote_util(commit)->obj)->tag);
	જો (!refs_read_ref(get_main_ref_store(the_repository), tag_ref, &oid) &&
	    oideq(&oid, &merge_remote_util(commit)->obj->oid))
		is_throwaway_tag = 0;
	નહીંતર
		is_throwaway_tag = 1;
	મુક્ત(tag_ref);
	પાછા is_throwaway_tag;
}

પૂર્ણાંક cmd_merge(પૂર્ણાંક argc,
	      અચલ અક્ષર **argv,
	      અચલ અક્ષર *prefix,
	      સંરચના repository *repo UNUSED)
{
	સંરચના object_id result_વૃક્ષ, stash, head_oid;
	સંરચના commit *head_commit;
	સંરચના strbuf buf = STRBUF_INIT;
	પૂર્ણાંક i, ret = 0, head_subsumed;
	પૂર્ણાંક best_cnt = -1, merge_was_ok = 0, automerge_was_ok = 0;
	સંરચના commit_list *common = ખાલી;
	અચલ અક્ષર *best_strategy = ખાલી, *wt_strategy = ખાલી;
	સંરચના commit_list *remoteheads = ખાલી, *p;
	શૂન્ય *branch_to_free, *argv_to_free = ખાલી;
	પૂર્ણાંક orig_argc = argc;
	પૂર્ણાંક merge_log_config = -1;

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
						      ખાલી);
	જો (branch)
		skip_prefix(branch, "refs/heads/", &branch);

	init_diff_ui_defaults();
	repo_config(the_repository, git_merge_config, &merge_log_config);

	જો (!branch || is_null_oid(&head_oid))
		head_commit = ખાલી;
	નહીંતર
		head_commit = lookup_commit_or_die(&head_oid, "HEAD");

	જો (branch_mergeoptions)
		parse_branch_merge_options(branch_mergeoptions);
	argc = parse_options(argc, argv, prefix, builtin_merge_options,
			builtin_merge_usage, 0);
	જો (shortlog_len < 0)
		shortlog_len = (merge_log_config > 0) ? merge_log_config : 0;

	જો (verbosity < 0 && show_progress == -1)
		show_progress = 0;

	જો (abort_current_merge) {
		પૂર્ણાંક nargc = 2;
		અચલ અક્ષર *nargv[] = {"reset", "--merge", ખાલી};
		અક્ષર stash_oid_hex[GIT_MAX_HEXSZ + 1];
		સંરચના object_id stash_oid = {0};

		જો (orig_argc != 2)
			usage_સંદેશ_opt(_("--abort expects no arguments"),
			      builtin_merge_usage, builtin_merge_options);

		જો (!file_exists(git_path_merge_head(the_repository)))
			die(_("There is no merge to abort (MERGE_HEAD missing)."));

		જો (!refs_read_ref(get_main_ref_store(the_repository), "MERGE_AUTOSTASH", &stash_oid))
			refs_delete_ref(get_main_ref_store(the_repository),
					"", "MERGE_AUTOSTASH", &stash_oid,
					REF_NO_DEREF);

		/* Invoke 'git reset --merge' */
		ret = cmd_reset(nargc, nargv, prefix, the_repository);

		જો (!is_null_oid(&stash_oid)) {
			oid_to_hex_r(stash_oid_hex, &stash_oid);
			apply_autostash_oid(stash_oid_hex);
		}

		જાઓ done;
	}

	જો (quit_current_merge) {
		જો (orig_argc != 2)
			usage_સંદેશ_opt(_("--quit expects no arguments"),
				      builtin_merge_usage,
				      builtin_merge_options);

		remove_merge_branch_state(the_repository);
		જાઓ done;
	}

	જો (continue_current_merge) {
		પૂર્ણાંક nargc = 1;
		અચલ અક્ષર *nargv[] = {"commit", ખાલી};

		જો (orig_argc != 2)
			usage_સંદેશ_opt(_("--continue expects no arguments"),
			      builtin_merge_usage, builtin_merge_options);

		જો (!file_exists(git_path_merge_head(the_repository)))
			die(_("There is no merge in progress (MERGE_HEAD missing)."));

		/* Invoke 'git commit' */
		ret = cmd_commit(nargc, nargv, prefix, the_repository);
		જાઓ done;
	}

	જો (repo_read_index_unmerged(the_repository))
		die_resolve_conflict("merge");

	જો (file_exists(git_path_merge_head(the_repository))) {
		/*
		 * There is no unmerged entry, don't advise 'git
		 * add/rm <file>', just 'git commit'.
		 */
		જો (advice_enabled(ADVICE_RESOLVE_CONFLICT))
			die(_("You have not concluded your merge (MERGE_HEAD exists).\n"
				  "Please, commit your changes before you merge."));
		નહીંતર
			die(_("You have not concluded your merge (MERGE_HEAD exists)."));
	}
	જો (refs_ref_exists(get_main_ref_store(the_repository), "CHERRY_PICK_HEAD")) {
		જો (advice_enabled(ADVICE_RESOLVE_CONFLICT))
			die(_("You have not concluded your cherry-pick (CHERRY_PICK_HEAD exists).\n"
			    "Please, commit your changes before you merge."));
		નહીંતર
			die(_("You have not concluded your cherry-pick (CHERRY_PICK_HEAD exists)."));
	}
	resolve_undo_clear_index(the_repository->index);

	જો (option_edit < 0)
		option_edit = default_edit_option();

	cleanup_mode = get_cleanup_mode(cleanup_arg, 0 < option_edit);

	જો (verbosity < 0)
		show_diffstat = 0;

	જો (squash) {
		જો (fast_forward == FF_NO)
			die(_("options '%s' and '%s' cannot be used together"), "--squash", "--no-ff.");
		જો (option_commit > 0)
			die(_("options '%s' and '%s' cannot be used together"), "--squash", "--commit.");
		/*
		 * squash can now silently disable option_commit - this is not
		 * a problem as it is only overriding the default, not a user
		 * supplied option.
		 */
		option_commit = 0;
	}

	જો (option_commit < 0)
		option_commit = 1;

	જો (!argc) {
		જો (default_to_upstream) {
			argc = setup_with_upstream(&argv);
			argv_to_free = argv;
		}
		નહીંતર
			die(_("No commit specified and merge.defaultToUpstream not set."));
	} નહીંતર જો (argc == 1 && !strcmp(argv[0], "-")) {
		argv[0] = "@{-1}";
	}

	જો (!argc)
		usage_with_options(builtin_merge_usage,
			builtin_merge_options);

	જો (!head_commit) {
		/*
		 * If the merged head is a valid one there is no reason
		 * to forbid "git merge" into a branch yet to be born.
		 * We do the same for "git pull".
		 */
		સંરચના object_id *remote_head_oid;
		જો (squash)
			die(_("Squash commit into empty head not supported yet"));
		જો (fast_forward == FF_NO)
			die(_("Non-fast-forward commit does not make sense into "
			    "an empty head"));
		remoteheads = collect_parents(head_commit, &head_subsumed,
					      argc, argv, ખાલી);
		જો (!remoteheads)
			die(_("%s - not something we can merge"), argv[0]);
		જો (remoteheads->next)
			die(_("Can merge only exactly one commit into empty head"));

		જો (verify_signatures)
			verify_merge_signature(remoteheads->item, verbosity,
					       check_trust_level);

		remote_head_oid = &remoteheads->item->object.oid;
		read_empty(remote_head_oid);
		refs_update_ref(get_main_ref_store(the_repository),
				"initial pull", "HEAD", remote_head_oid, ખાલી,
				0,
				UPDATE_REFS_DIE_ON_ERR);
		જાઓ done;
	}

	/*
	 * All the rest are the commits being merged; prepare
	 * the standard merge summary message to be appended
	 * to the given message.
	 */
	remoteheads = collect_parents(head_commit, &head_subsumed,
				      argc, argv, &merge_સંદેશ);

	જો (!head_commit || !argc)
		usage_with_options(builtin_merge_usage,
			builtin_merge_options);

	જો (verify_signatures) {
		માટે (p = remoteheads; p; p = p->next) {
			verify_merge_signature(p->item, verbosity,
					       check_trust_level);
		}
	}

	strbuf_addstr(&buf, "merge");
	માટે (p = remoteheads; p; p = p->next)
		strbuf_addf(&buf, " %s", merge_remote_util(p->item)->name);
	setenv("GIT_REFLOG_ACTION", buf.buf, 0);
	strbuf_reset(&buf);

	માટે (p = remoteheads; p; p = p->next) {
		સંરચના commit *commit = p->item;
		strbuf_addf(&buf, "GITHEAD_%s",
			    oid_to_hex(&commit->object.oid));
		setenv(buf.buf, merge_remote_util(commit)->name, 1);
		strbuf_reset(&buf);
		જો (fast_forward != FF_ONLY && merging_a_throwaway_tag(commit))
			fast_forward = FF_NO;
	}

	જો (!use_strategies) {
		જો (!remoteheads)
			; /* already up-to-date */
		નહીંતર જો (!remoteheads->next)
			add_strategies(pull_twohead, DEFAULT_TWOHEAD);
		નહીંતર
			add_strategies(pull_octopus, DEFAULT_OCTOPUS);
	}

	માટે (i = 0; i < use_strategies_nr; i++) {
		જો (use_strategies[i]->attr & NO_FAST_FORWARD)
			fast_forward = FF_NO;
		જો (use_strategies[i]->attr & NO_TRIVIAL)
			allow_trivial = 0;
	}

	જો (!remoteheads)
		; /* already up-to-date */
	નહીંતર જો (!remoteheads->next) {
		જો (repo_get_merge_bases(the_repository, head_commit,
					 remoteheads->item, &common) < 0) {
			ret = 2;
			જાઓ done;
		}
	} નહીંતર {
		સંરચના commit_list *list = remoteheads;
		commit_list_ઉમેરો(head_commit, &list);
		જો (get_octopus_merge_bases(list, &common) < 0) {
			મુક્ત(list);
			ret = 2;
			જાઓ done;
		}
		મુક્ત(list);
	}

	refs_update_ref(get_main_ref_store(the_repository),
			"updating ORIG_HEAD", "ORIG_HEAD",
			&head_commit->object.oid, ખાલી, 0,
			UPDATE_REFS_DIE_ON_ERR);

	જો (remoteheads && !common) {
		/* No common ancestors found. */
		જો (!allow_unrelated_histories)
			die(_("refusing to merge unrelated histories"));
		/* otherwise, we need a real merge. */
	} નહીંતર જો (!remoteheads ||
		 (!remoteheads->next && !common->next &&
		  common->item == remoteheads->item)) {
		/*
		 * If head can reach all the merge then we are up to date.
		 * but first the most common case of merging one remote.
		 */
		finish_up_to_date();
		જાઓ done;
	} નહીંતર જો (fast_forward != FF_NO && !remoteheads->next &&
			!common->next &&
			oideq(&common->item->object.oid, &head_commit->object.oid)) {
		/* Again the most common case of merging one remote. */
		અચલ અક્ષર *સંદેશ = have_message ?
			"Fast-forward (no commit created; -m option ignored)" :
			"Fast-forward";
		સંરચના commit *commit;

		જો (verbosity >= 0) {
			printf(_("Updating %s..%s\n"),
			       repo_find_unique_abbrev(the_repository, &head_commit->object.oid,
						       DEFAULT_ABBREV),
			       repo_find_unique_abbrev(the_repository, &remoteheads->item->object.oid,
						       DEFAULT_ABBREV));
		}
		commit = remoteheads->item;
		જો (!commit) {
			ret = 1;
			જાઓ done;
		}

		જો (autostash)
			create_autostash_ref(the_repository, "MERGE_AUTOSTASH",
					     ખાલી, false);
		જો (checkout_fast_forward(the_repository,
					  &head_commit->object.oid,
					  &commit->object.oid,
					  overwrite_ignore)) {
			apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
					    ખાલી, ખાલી, ખાલી, ખાલી);
			ret = 1;
			જાઓ done;
		}

		finish(head_commit, remoteheads, &commit->object.oid, સંદેશ);
		remove_merge_branch_state(the_repository);
		જાઓ done;
	} નહીંતર જો (!remoteheads->next && common->next)
		;
		/*
		 * We are not doing octopus and not fast-forward.  Need
		 * a real merge.
		 */
	નહીંતર જો (!remoteheads->next && !common->next && option_commit) {
		/*
		 * We are not doing octopus, not fast-forward, and have
		 * only one common.
		 */
		refresh_index(the_repository->index, REFRESH_QUIET, ખાલી, ખાલી, ખાલી);
		જો (allow_trivial && fast_forward != FF_ONLY) {
			/*
			 * Must first ensure that index matches HEAD before
			 * attempting a trivial merge.
			 */
			સંરચના વૃક્ષ *head_વૃક્ષ = repo_get_commit_વૃક્ષ(the_repository,
								      head_commit);
			સંરચના strbuf sb = STRBUF_INIT;

			જો (repo_index_has_changes(the_repository, head_વૃક્ષ,
						   &sb)) {
				error(_("Your local changes to the following files would be overwritten by merge:\n  %s"),
				      sb.buf);
				strbuf_release(&sb);
				ret = 2;
				જાઓ done;
			}

			/* See if it is really trivial. */
			git_committer_info(IDENT_STRICT);
			printf(_("Trying really trivial in-index merge...\n"));
			જો (!read_વૃક્ષ_trivial(&common->item->object.oid,
					       &head_commit->object.oid,
					       &remoteheads->item->object.oid)) {
				ret = merge_trivial(head_commit, remoteheads);
				જાઓ done;
			}
			printf(_("Nope.\n"));
		}
	} નહીંતર {
		/*
		 * An octopus.  If we can reach all the remote we are up
		 * to date.
		 */
		પૂર્ણાંક up_to_date = 1;
		સંરચના commit_list *j;

		માટે (j = remoteheads; j; j = j->next) {
			પૂર્ણાંક ret = repo_in_merge_bases(the_repository,
						      j->item, head_commit);
			જો (ret < 0)
				exit(128);
			જો (!ret) {
				up_to_date = 0;
				તોડો;
			}
		}
		જો (up_to_date) {
			finish_up_to_date();
			જાઓ done;
		}
	}

	જો (fast_forward == FF_ONLY)
		die_ff_impossible();

	જો (autostash)
		create_autostash_ref(the_repository, "MERGE_AUTOSTASH",
				     ખાલી, false);

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
	જો (save_state(&stash))
		oidclr(&stash, the_repository->hash_algo);

	માટે (i = 0; i < use_strategies_nr; i++) {
		પૂર્ણાંક ret, cnt;
		જો (i) {
			printf(_("Rewinding the tree to pristine...\n"));
			restore_state(&head_commit->object.oid, &stash);
		}
		જો (use_strategies_nr != 1)
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
		જો (ret < 2) {
			જો (!ret) {
				/*
				 * This strategy worked; no point in trying
				 * another.
				 */
				merge_was_ok = 1;
				best_strategy = wt_strategy;
				તોડો;
			}
			cnt = (use_strategies_nr > 1) ? evaluate_result() : 0;
			જો (best_cnt <= 0 || cnt <= best_cnt) {
				best_strategy = wt_strategy;
				best_cnt = cnt;
			}
		}
	}

	/*
	 * If we have a resulting tree, that means the strategy module
	 * auto resolved the merge cleanly.
	 */
	જો (merge_was_ok && option_commit) {
		automerge_was_ok = 1;
		ret = finish_automerge(head_commit, head_subsumed,
				       common, remoteheads,
				       &result_વૃક્ષ, wt_strategy);
		જાઓ done;
	}

	/*
	 * Pick the result from the best strategy and have the user fix
	 * it up.
	 */
	જો (!best_strategy) {
		restore_state(&head_commit->object.oid, &stash);
		જો (use_strategies_nr > 1)
			fprintf(stderr,
				_("No merge strategy handled the merge.\n"));
		નહીંતર
			fprintf(stderr, _("Merge with strategy %s failed.\n"),
				use_strategies[0]->name);
		apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
				    ખાલી, ખાલી, ખાલી, ખાલી);
		ret = 2;
		જાઓ done;
	} નહીંતર જો (best_strategy == wt_strategy)
		; /* We already have its result in the working tree. */
	નહીંતર {
		printf(_("Rewinding the tree to pristine...\n"));
		restore_state(&head_commit->object.oid, &stash);
		printf(_("Using the %s strategy to prepare resolving by hand.\n"),
			best_strategy);
		try_merge_strategy(best_strategy, common, remoteheads,
				   head_commit);
	}

	જો (squash) {
		finish(head_commit, remoteheads, ખાલી, ખાલી);

		git_test_write_commit_graph_or_die(the_repository);
	} નહીંતર
		write_merge_state(remoteheads);

	જો (merge_was_ok)
		fprintf(stderr, _("Automatic merge went well; "
			"stopped before committing as requested\n"));
	નહીંતર
		ret = suggest_conflicts();
	જો (autostash)
		printf(_("When finished, apply stashed changes with `git stash pop`\n"));

done:
	જો (!automerge_was_ok) {
		commit_list_free(common);
		commit_list_free(remoteheads);
	}
	strbuf_release(&buf);
	મુક્ત(branch_to_free);
	મુક્ત(argv_to_free);
	મુક્ત(pull_twohead);
	મુક્ત(pull_octopus);
	discard_index(the_repository->index);
	પાછા ret;
}
