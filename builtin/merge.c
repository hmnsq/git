/*
 * Builtin "git merge"
 *
 * Copyright (c) 2008 Miklos Vajna <vmiklos@frugalware.org>
 *
 * Based on git-merge.sh by Junio C Hamano.
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#அந்தர்பாவ "builtin.h"

#அந்தர்பாவ "abspath.h"
#அந்தர்பாவ "advice.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "editor.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "hex.h"
#அந்தர்பாவ "object-name.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "lockfile.h"
#அந்தர்பாவ "repository.h"
#அந்தர்பாவ "run-command.h"
#அந்தர்பாவ "hook.h"
#அந்தர்பாவ "diff.h"
#அந்தர்பாவ "diff-merges.h"
#அந்தர்பாவ "refs.h"
#அந்தர்பாவ "refspec.h"
#அந்தர்பாவ "commit.h"
#அந்தர்பாவ "diffcore.h"
#அந்தர்பாவ "path.h"
#அந்தர்பாவ "revision.h"
#அந்தர்பாவ "unpack-trees.h"
#அந்தர்பாவ "cache-tree.h"
#அந்தர்பாவ "dir.h"
#அந்தர்பாவ "color.h"
#அந்தர்பாவ "rerere.h"
#அந்தர்பாவ "help.h"
#அந்தர்பாவ "merge.h"
#அந்தர்பாவ "merge-ort-wrappers.h"
#அந்தர்பாவ "resolve-undo.h"
#அந்தர்பாவ "remote.h"
#அந்தர்பாவ "fmt-merge-msg.h"
#அந்தர்பாவ "sequencer.h"
#அந்தர்பாவ "string-list.h"
#அந்தர்பாவ "tag.h"
#அந்தர்பாவ "alias.h"
#அந்தர்பாவ "branch.h"
#அந்தர்பாவ "commit-reach.h"
#அந்தர்பாவ "wt-status.h"
#அந்தர்பாவ "commit-graph.h"

#define DEFAULT_TWOHEAD (1<<0)
#define DEFAULT_OCTOPUS (1<<1)
#define NO_FAST_FORWARD (1<<2)
#define NO_TRIVIAL      (1<<3)

கட்டமைப்பு strategy {
	மாறிலி எழுத்து *name;
	unsigned attr;
};

நிலையான மாறிலி எழுத்து * மாறிலி builtin_merge_usage[] = {
	N_("git merge [<options>] [<commit>...]"),
	"git merge --abort",
	"git merge --continue",
	வெற்று
};

#define MERGE_SHOW_DIFFSTAT 1
#define MERGE_SHOW_COMPACTSUMMARY 2

நிலையான முழு show_diffstat = MERGE_SHOW_DIFFSTAT, shortlog_len = -1, squash;
நிலையான முழு option_commit = -1;
நிலையான முழு option_edit = -1;
நிலையான முழு allow_trivial = 1, have_message, verify_signatures;
நிலையான முழு check_trust_level = 1;
நிலையான முழு overwrite_ignore = 1;
நிலையான கட்டமைப்பு strbuf merge_செய்தி = STRBUF_INIT;
நிலையான கட்டமைப்பு strategy **use_strategies;
நிலையான size_t use_strategies_nr, use_strategies_alloc;
நிலையான கட்டமைப்பு strvec xopts = STRVEC_INIT;
நிலையான மாறிலி எழுத்து *branch;
நிலையான எழுத்து *branch_mergeoptions;
நிலையான முழு verbosity;
நிலையான முழு allow_rerere_auto;
நிலையான முழு abort_current_merge;
நிலையான முழு quit_current_merge;
நிலையான முழு continue_current_merge;
நிலையான முழு allow_unrelated_histories;
நிலையான முழு show_progress = -1;
நிலையான முழு default_to_upstream = 1;
நிலையான முழு signoff;
நிலையான மாறிலி எழுத்து *sign_commit;
நிலையான முழு autostash;
நிலையான முழு no_verify;
நிலையான எழுத்து *into_name;

நிலையான கட்டமைப்பு strategy all_strategy[] = {
	{ "recursive",  NO_TRIVIAL },
	{ "octopus",    DEFAULT_OCTOPUS },
	{ "ort",        DEFAULT_TWOHEAD | NO_TRIVIAL },
	{ "resolve",    0 },
	{ "ours",       NO_FAST_FORWARD | NO_TRIVIAL },
	{ "subtree",    NO_FAST_FORWARD | NO_TRIVIAL },
};

நிலையான எழுத்து *pull_twohead, *pull_octopus;

எண்ணிக்கை ff_type {
	FF_NO,
	FF_ALLOW,
	FF_ONLY
};

நிலையான எண்ணிக்கை ff_type fast_forward = FF_ALLOW;

நிலையான எழுத்து *cleanup_arg;
நிலையான எண்ணிக்கை commit_செய்தி_cleanup_mode cleanup_mode;

நிலையான முழு option_parse_message(மாறிலி கட்டமைப்பு option *opt,
				மாறிலி எழுத்து *arg, முழு unset)
{
	கட்டமைப்பு strbuf *buf = opt->மதிப்பு;

	எனில் (unset)
		strbuf_setlen(buf, 0);
	இல்லையெனில் எனில் (arg) {
		strbuf_addf(buf, "%s%s", buf->len ? "\n\n" : "", arg);
		have_message = 1;
	} இல்லையெனில்
		திரும்பு error(_("switch `m' requires a value"));
	திரும்பு 0;
}

நிலையான எண்ணிக்கை parse_opt_result option_read_message(கட்டமைப்பு parse_opt_ctx_t *ctx,
						 மாறிலி கட்டமைப்பு option *opt,
						 மாறிலி எழுத்து *arg_not_used,
						 முழு unset)
{
	கட்டமைப்பு strbuf *buf = opt->மதிப்பு;
	மாறிலி எழுத்து *arg;

	BUG_ON_OPT_ARG(arg_not_used);
	எனில் (unset)
		BUG("-F cannot be negated");

	எனில் (ctx->opt) {
		arg = ctx->opt;
		ctx->opt = வெற்று;
	} இல்லையெனில் எனில் (ctx->argc > 1) {
		ctx->argc--;
		arg = *++ctx->argv;
	} இல்லையெனில்
		திரும்பு error(_("option `%s' requires a value"), opt->long_name);

	எனில் (buf->len)
		strbuf_addch(buf, '\n');
	எனில் (ctx->prefix && !is_absolute_path(arg))
		arg = prefix_filename(ctx->prefix, arg);
	எனில் (strbuf_read_file(buf, arg, 0) < 0)
		திரும்பு error(_("could not read file '%s'"), arg);
	have_message = 1;

	திரும்பு 0;
}

நிலையான கட்டமைப்பு strategy *get_strategy(மாறிலி எழுத்து *name)
{
	முழு i;
	கட்டமைப்பு strategy *ret;
	நிலையான கட்டமைப்பு cmdnames main_cmds = {0}, other_cmds = {0};
	நிலையான முழு loaded;
	எழுத்து *default_strategy = வெற்று;

	எனில் (!name)
		திரும்பு வெற்று;

	எனில் (default_strategy &&
	    !strcmp(default_strategy, "ort") &&
	    !strcmp(name, "recursive")) {
		name = "ort";
	}

	க்கு (i = 0; i < ARRAY_SIZE(all_strategy); i++)
		எனில் (!strcmp(name, all_strategy[i].name))
			திரும்பு &all_strategy[i];

	எனில் (!loaded) {
		கட்டமைப்பு cmdnames not_strategies = {0};
		loaded = 1;

		load_command_list("git-merge-", &main_cmds, &other_cmds);
		க்கு (i = 0; i < main_cmds.cnt; i++) {
			முழு j, found = 0;
			கட்டமைப்பு cmdname *ent = main_cmds.names[i];
			க்கு (j = 0; !found && j < ARRAY_SIZE(all_strategy); j++)
				எனில் (!xstrncmpz(all_strategy[j].name, ent->name, ent->len))
					found = 1;
			எனில் (!found)
				add_cmdname(&not_strategies, ent->name, ent->len);
		}
		exclude_cmds(&main_cmds, &not_strategies);

		cmdnames_release(&not_strategies);
	}
	எனில் (!is_in_cmdlist(&main_cmds, name) && !is_in_cmdlist(&other_cmds, name)) {
		fprintf(stderr, _("Could not find merge strategy '%s'.\n"), name);
		fprintf(stderr, _("Available strategies are:"));
		க்கு (i = 0; i < main_cmds.cnt; i++)
			fprintf(stderr, " %s", main_cmds.names[i]->name);
		fprintf(stderr, ".\n");
		எனில் (other_cmds.cnt) {
			fprintf(stderr, _("Available custom strategies are:"));
			க்கு (i = 0; i < other_cmds.cnt; i++)
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
	திரும்பு ret;
}

நிலையான வெறுமை append_strategy(கட்டமைப்பு strategy *s)
{
	ALLOC_GROW(use_strategies, use_strategies_nr + 1, use_strategies_alloc);
	use_strategies[use_strategies_nr++] = s;
}

நிலையான முழு option_parse_strategy(மாறிலி கட்டமைப்பு option *opt UNUSED,
				 மாறிலி எழுத்து *name, முழு unset)
{
	எனில் (unset)
		திரும்பு 0;

	append_strategy(get_strategy(name));
	திரும்பு 0;
}

நிலையான முழு option_parse_compact_summary(மாறிலி கட்டமைப்பு option *opt,
					மாறிலி எழுத்து *name UNUSED, முழு unset)
{
	முழு *setting = opt->மதிப்பு;

	எனில் (unset)
		*setting = 0;
	இல்லையெனில்
		*setting = MERGE_SHOW_COMPACTSUMMARY;
	திரும்பு 0;
}

நிலையான கட்டமைப்பு option builtin_merge_options[] = {
	OPT_SET_INT('n', வெற்று, &show_diffstat,
		N_("do not show a diffstat at the end of the merge"), 0),
	OPT_BOOL(0, "stat", &show_diffstat,
		N_("show a diffstat at the end of the merge")),
	OPT_BOOL(0, "summary", &show_diffstat, N_("(synonym to --stat)")),
	OPT_CALLBACK_F(0, "compact-summary", &show_diffstat, வெற்று,
		       N_("show a compact-summary at the end of the merge"),
		       PARSE_OPT_NOARG,
		       option_parse_compact_summary),
	{
		.type = OPTION_INTEGER,
		.long_name = "log",
		.மதிப்பு = &shortlog_len,
		.precision = அளவு(shortlog_len),
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
	OPT_CALLBACK('s', "strategy", வெற்று, N_("strategy"),
		N_("merge strategy to use"), option_parse_strategy),
	OPT_STRVEC('X', "strategy-option", &xopts, N_("option=value"),
		N_("option for selected merge strategy")),
	OPT_CALLBACK('m', "message", &merge_செய்தி, N_("message"),
		N_("merge commit message (for a non-fast-forward merge)"),
		option_parse_message),
	{
		.type = OPTION_LOWLEVEL_CALLBACK,
		.short_name = 'F',
		.long_name = "file",
		.மதிப்பு = &merge_செய்தி,
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
		.மதிப்பு = &sign_commit,
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

நிலையான முழு save_state(கட்டமைப்பு object_id *stash)
{
	முழு len;
	கட்டமைப்பு child_process cp = CHILD_PROCESS_INIT;
	கட்டமைப்பு strbuf buffer = STRBUF_INIT;
	கட்டமைப்பு lock_file lock_file = LOCK_INIT;
	முழு fd;
	முழு rc = -1;

	fd = repo_hold_locked_index(the_repository, &lock_file, 0);
	refresh_index(the_repository->index, REFRESH_QUIET, வெற்று, வெற்று, வெற்று);
	எனில் (0 <= fd)
		repo_update_index_if_able(the_repository, &lock_file);
	rollback_lock_file(&lock_file);

	strvec_pushl(&cp.args, "stash", "create", வெற்று);
	cp.out = -1;
	cp.git_cmd = 1;

	எனில் (start_command(&cp))
		die(_("could not run stash."));
	len = strbuf_read(&buffer, cp.out, 1024);
	close(cp.out);

	எனில் (finish_command(&cp) || len < 0)
		die(_("stash failed"));
	இல்லையெனில் எனில் (!len)		/* no changes */
		செல் out;
	strbuf_setlen(&buffer, buffer.len-1);
	எனில் (repo_get_oid(the_repository, buffer.buf, stash))
		die(_("not a valid object: %s"), buffer.buf);
	rc = 0;
out:
	strbuf_release(&buffer);
	திரும்பு rc;
}

நிலையான வெறுமை read_empty(மாறிலி கட்டமைப்பு object_id *oid)
{
	கட்டமைப்பு child_process cmd = CHILD_PROCESS_INIT;

	strvec_pushl(&cmd.args, "read-tree", "-m", "-u",
		     empty_மரம்_oid_hex(the_repository->hash_algo),
		     oid_to_hex(oid), வெற்று);
	cmd.git_cmd = 1;

	எனில் (run_command(&cmd))
		die(_("read-tree failed"));
}

நிலையான வெறுமை reset_hard(மாறிலி கட்டமைப்பு object_id *oid)
{
	கட்டமைப்பு child_process cmd = CHILD_PROCESS_INIT;

	strvec_pushl(&cmd.args, "read-tree", "-v", "--reset", "-u",
		     oid_to_hex(oid), வெற்று);
	cmd.git_cmd = 1;

	எனில் (run_command(&cmd))
		die(_("read-tree failed"));
}

நிலையான வெறுமை restore_state(மாறிலி கட்டமைப்பு object_id *head,
			  மாறிலி கட்டமைப்பு object_id *stash)
{
	கட்டமைப்பு child_process cmd = CHILD_PROCESS_INIT;

	reset_hard(head);

	எனில் (is_null_oid(stash))
		செல் refresh_cache;

	strvec_pushl(&cmd.args, "stash", "apply", "--index", "--quiet", வெற்று);
	strvec_தள்ளு(&cmd.args, oid_to_hex(stash));

	/*
	 * It is OK to ignore error here, for example when there was
	 * nothing to restore.
	 */
	cmd.git_cmd = 1;
	run_command(&cmd);

refresh_cache:
	discard_index(the_repository->index);
	எனில் (repo_read_index(the_repository) < 0)
		die(_("could not read index"));
}

/* This is called when no merge was necessary. */
நிலையான வெறுமை finish_up_to_date(வெறுமை)
{
	எனில் (verbosity >= 0) {
		எனில் (squash)
			puts(_("Already up to date. (nothing to squash)"));
		இல்லையெனில்
			puts(_("Already up to date."));
	}
	remove_merge_branch_state(the_repository);
}

நிலையான வெறுமை squash_message(கட்டமைப்பு commit *commit, கட்டமைப்பு commit_list *remoteheads)
{
	கட்டமைப்பு rev_info rev;
	கட்டமைப்பு strbuf out = STRBUF_INIT;
	கட்டமைப்பு commit_list *j;
	கட்டமைப்பு pretty_print_context ctx = {0};

	printf(_("Squash commit -- not updating HEAD\n"));

	repo_init_revisions(the_repository, &rev, வெற்று);
	diff_merges_suppress(&rev);
	rev.commit_format = CMIT_FMT_MEDIUM;

	commit->object.flags |= UNINTERESTING;
	add_pending_object(&rev, &commit->object, வெற்று);

	க்கு (j = remoteheads; j; j = j->next)
		add_pending_object(&rev, &j->item->object, வெற்று);

	setup_revisions(0, வெற்று, &rev, வெற்று);
	எனில் (prepare_revision_walk(&rev))
		die(_("revision walk setup failed"));

	ctx.abbrev = rev.abbrev;
	ctx.date_mode = rev.date_mode;
	ctx.fmt = rev.commit_format;

	strbuf_addstr(&out, "Squashed commit of the following:\n");
	போது ((commit = get_revision(&rev)) != வெற்று) {
		strbuf_addch(&out, '\n');
		strbuf_addf(&out, "commit %s\n",
			oid_to_hex(&commit->object.oid));
		pretty_print_commit(&ctx, commit, &out);
	}
	write_file_buf(git_path_squash_செய்தி(the_repository), out.buf, out.len);
	strbuf_release(&out);
	release_revisions(&rev);
}

நிலையான வெறுமை finish(கட்டமைப்பு commit *head_commit,
		   கட்டமைப்பு commit_list *remoteheads,
		   மாறிலி கட்டமைப்பு object_id *new_head, மாறிலி எழுத்து *செய்தி)
{
	கட்டமைப்பு strbuf reflog_message = STRBUF_INIT;
	மாறிலி கட்டமைப்பு object_id *head = &head_commit->object.oid;

	எனில் (!செய்தி)
		strbuf_addstr(&reflog_message, getenv("GIT_REFLOG_ACTION"));
	இல்லையெனில் {
		எனில் (verbosity >= 0)
			printf("%s\n", செய்தி);
		strbuf_addf(&reflog_message, "%s: %s",
			getenv("GIT_REFLOG_ACTION"), செய்தி);
	}
	எனில் (squash) {
		squash_message(head_commit, remoteheads);
	} இல்லையெனில் {
		எனில் (verbosity >= 0 && !merge_செய்தி.len)
			printf(_("No merge message -- not updating HEAD\n"));
		இல்லையெனில் {
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
	எனில் (new_head && show_diffstat) {
		கட்டமைப்பு diff_options opts;
		repo_diff_setup(the_repository, &opts);
		init_diffstat_widths(&opts);

		தேர்வு (show_diffstat) {
		நிலை MERGE_SHOW_DIFFSTAT: /* 1 */
			opts.output_format |=
				DIFF_FORMAT_SUMMARY | DIFF_FORMAT_DIFFSTAT;
			நிறுத்து;
		நிலை MERGE_SHOW_COMPACTSUMMARY: /* 2 */
			opts.output_format |= DIFF_FORMAT_DIFFSTAT;
			opts.flags.stat_with_summary = 1;
			நிறுத்து;
		இயல்பு:
			நிறுத்து;
		}
		opts.detect_rename = DIFF_DETECT_RENAME;
		diff_setup_done(&opts);
		diff_மரம்_oid(head, new_head, "", &opts);
		diffcore_std(&opts);
		diff_flush(&opts);
	}

	/* Run a post-merge hook */
	run_hooks_l(the_repository, "post-merge", squash ? "1" : "0", வெற்று);

	எனில் (new_head)
		apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
				    வெற்று, வெற்று, வெற்று, வெற்று);
	strbuf_release(&reflog_message);
}

/* Get the name for the merge commit's message. */
நிலையான வெறுமை merge_name(மாறிலி எழுத்து *remote, கட்டமைப்பு strbuf *செய்தி)
{
	கட்டமைப்பு commit *remote_head;
	கட்டமைப்பு object_id branch_head;
	கட்டமைப்பு strbuf bname = STRBUF_INIT;
	கட்டமைப்பு merge_remote_desc *desc;
	மாறிலி எழுத்து *ptr;
	எழுத்து *found_ref = வெற்று;
	முழு len, early;

	copy_branchname(the_repository, &bname, remote, 0);
	remote = bname.buf;

	oidclr(&branch_head, the_repository->hash_algo);
	remote_head = get_merge_parent(remote);
	எனில் (!remote_head)
		die(_("'%s' does not point to a commit"), remote);

	எனில் (repo_dwim_ref(the_repository, remote, strlen(remote), &branch_head,
			  &found_ref, 0) > 0) {
		எனில் (starts_with(found_ref, "refs/heads/")) {
			strbuf_addf(செய்தி, "%s\t\tbranch '%s' of .\n",
				    oid_to_hex(&branch_head), remote);
			செல் cleanup;
		}
		எனில் (starts_with(found_ref, "refs/tags/")) {
			strbuf_addf(செய்தி, "%s\t\ttag '%s' of .\n",
				    oid_to_hex(&branch_head), remote);
			செல் cleanup;
		}
		எனில் (starts_with(found_ref, "refs/remotes/")) {
			strbuf_addf(செய்தி, "%s\t\tremote-tracking branch '%s' of .\n",
				    oid_to_hex(&branch_head), remote);
			செல் cleanup;
		}
	}

	/* See if remote matches <name>^^^.. or <name>~<number> */
	க்கு (len = 0, ptr = remote + strlen(remote);
	     remote < ptr && ptr[-1] == '^';
	     ptr--)
		len++;
	எனில் (len)
		early = 1;
	இல்லையெனில் {
		early = 0;
		ptr = strrchr(remote, '~');
		எனில் (ptr) {
			முழு seen_nonzero = 0;

			len++; /* count ~ */
			போது (*++ptr && isdigit(*ptr)) {
				seen_nonzero |= (*ptr != '0');
				len++;
			}
			எனில் (*ptr)
				len = 0; /* not ...~<number> */
			இல்லையெனில் எனில் (seen_nonzero)
				early = 1;
			இல்லையெனில் எனில் (len == 1)
				early = 1; /* "name~" is "name~1"! */
		}
	}
	எனில் (len) {
		கட்டமைப்பு strbuf truname = STRBUF_INIT;
		strbuf_addf(&truname, "refs/heads/%s", remote);
		strbuf_setlen(&truname, truname.len - len);
		எனில் (refs_ref_exists(get_main_ref_store(the_repository), truname.buf)) {
			strbuf_addf(செய்தி,
				    "%s\t\tbranch '%s'%s of .\n",
				    oid_to_hex(&remote_head->object.oid),
				    truname.buf + 11,
				    (early ? " (early part)" : ""));
			strbuf_release(&truname);
			செல் cleanup;
		}
		strbuf_release(&truname);
	}

	desc = merge_remote_util(remote_head);
	எனில் (desc && desc->obj && desc->obj->type == OBJ_TAG) {
		strbuf_addf(செய்தி, "%s\t\t%s '%s'\n",
			    oid_to_hex(&desc->obj->oid),
			    type_name(desc->obj->type),
			    remote);
		செல் cleanup;
	}

	strbuf_addf(செய்தி, "%s\t\tcommit '%s'\n",
		oid_to_hex(&remote_head->object.oid), remote);
cleanup:
	விடுவி(found_ref);
	strbuf_release(&bname);
}

நிலையான வெறுமை parse_branch_merge_options(எழுத்து *bmo)
{
	மாறிலி எழுத்து **argv;
	முழு argc;

	எனில் (!bmo)
		திரும்பு;
	argc = split_cmdline(bmo, &argv);
	எனில் (argc < 0)
		die(_("Bad branch.%s.mergeoptions string: %s"), branch,
		    _(split_cmdline_strerror(argc)));
	REALLOC_ARRAY(argv, argc + 2);
	MOVE_ARRAY(argv + 1, argv, argc + 1);
	argc++;
	argv[0] = "branch.*.mergeoptions";
	parse_options(argc, argv, வெற்று, builtin_merge_options,
		      builtin_merge_usage, 0);
	விடுவி(argv);
}

நிலையான முழு git_merge_config(மாறிலி எழுத்து *k, மாறிலி எழுத்து *v,
			    மாறிலி கட்டமைப்பு config_context *ctx, வெறுமை *cb)
{
	முழு status;
	மாறிலி எழுத்து *str;

	எனில் (branch &&
	    skip_prefix(k, "branch.", &str) &&
	    skip_prefix(str, branch, &str) &&
	    !strcmp(str, ".mergeoptions")) {
		விடுவி(branch_mergeoptions);
		branch_mergeoptions = xstrdup(v);
		திரும்பு 0;
	}

	எனில் (!strcmp(k, "merge.diffstat") || !strcmp(k, "merge.stat")) {
		முழு val = git_parse_maybe_bool_text(v);
		தேர்வு (val) {
		நிலை 0:
			show_diffstat = 0;
			நிறுத்து;
		நிலை 1:
			show_diffstat = MERGE_SHOW_DIFFSTAT;
			நிறுத்து;
		இயல்பு:
			எனில் (!strcmp(v, "compact"))
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
			இல்லையெனில்
				/*
				 * A setting from a future?  It is not an
				 * error grave enough to fail the command.
				 * proceed using the default one.
				 */
				show_diffstat = MERGE_SHOW_DIFFSTAT;
			நிறுத்து;
		}
	} இல்லையெனில் எனில் (!strcmp(k, "merge.verifysignatures")) {
		verify_signatures = git_config_bool(k, v);
	} இல்லையெனில் எனில் (!strcmp(k, "pull.twohead")) {
		FREE_AND_NULL(pull_twohead);
		திரும்பு git_config_string(&pull_twohead, k, v);
	} இல்லையெனில் எனில் (!strcmp(k, "pull.octopus")) {
		FREE_AND_NULL(pull_octopus);
		திரும்பு git_config_string(&pull_octopus, k, v);
	} இல்லையெனில் எனில் (!strcmp(k, "commit.cleanup")) {
		திரும்பு git_config_string(&cleanup_arg, k, v);
	} இல்லையெனில் எனில் (!strcmp(k, "merge.ff")) {
		முழு boolval = git_parse_maybe_bool(v);
		எனில் (0 <= boolval) {
			fast_forward = boolval ? FF_ALLOW : FF_NO;
		} இல்லையெனில் எனில் (v && !strcmp(v, "only")) {
			fast_forward = FF_ONLY;
		} /* do not barf on values from future versions of git */
		திரும்பு 0;
	} இல்லையெனில் எனில் (!strcmp(k, "merge.defaulttoupstream")) {
		default_to_upstream = git_config_bool(k, v);
		திரும்பு 0;
	} இல்லையெனில் எனில் (!strcmp(k, "commit.gpgsign")) {
		sign_commit = git_config_bool(k, v) ? "" : வெற்று;
		திரும்பு 0;
	} இல்லையெனில் எனில் (!strcmp(k, "gpg.mintrustlevel")) {
		check_trust_level = 0;
	} இல்லையெனில் எனில் (!strcmp(k, "merge.autostash")) {
		autostash = git_config_bool(k, v);
		திரும்பு 0;
	}

	status = fmt_merge_செய்தி_config(k, v, ctx, cb);
	எனில் (status)
		திரும்பு status;
	திரும்பு git_diff_ui_config(k, v, ctx, cb);
}

நிலையான முழு read_மரம்_trivial(கட்டமைப்பு object_id *common, கட்டமைப்பு object_id *head,
			     கட்டமைப்பு object_id *one)
{
	முழு i, nr_trees = 0;
	கட்டமைப்பு மரம் *trees[MAX_UNPACK_TREES];
	கட்டமைப்பு மரம்_desc t[MAX_UNPACK_TREES];
	கட்டமைப்பு unpack_trees_options opts;

	memset(&opts, 0, அளவு(opts));
	opts.head_idx = 2;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;
	opts.update = 1;
	opts.verbose_update = 1;
	opts.trivial_merges_only = 1;
	opts.merge = 1;
	opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
	trees[nr_trees] = repo_parse_மரம்_indirect(the_repository, common);
	எனில் (!trees[nr_trees++])
		திரும்பு -1;
	trees[nr_trees] = repo_parse_மரம்_indirect(the_repository, head);
	எனில் (!trees[nr_trees++])
		திரும்பு -1;
	trees[nr_trees] = repo_parse_மரம்_indirect(the_repository, one);
	எனில் (!trees[nr_trees++])
		திரும்பு -1;
	opts.fn = threeway_merge;
	cache_மரம்_free(&the_repository->index->cache_மரம்);
	க்கு (i = 0; i < nr_trees; i++) {
		repo_parse_மரம்(the_repository, trees[i]);
		init_மரம்_desc(t+i, &trees[i]->object.oid,
			       trees[i]->buffer, trees[i]->size);
	}
	எனில் (unpack_trees(nr_trees, t, &opts))
		திரும்பு -1;
	திரும்பு 0;
}

நிலையான வெறுமை write_மரம்_trivial(கட்டமைப்பு object_id *oid)
{
	எனில் (write_index_as_மரம்(oid, the_repository->index,
				repo_get_index_file(the_repository),
				0, வெற்று))
		die(_("git write-tree failed to write a tree"));
}

நிலையான முழு try_merge_strategy(மாறிலி எழுத்து *strategy, கட்டமைப்பு commit_list *common,
			      கட்டமைப்பு commit_list *remoteheads,
			      கட்டமைப்பு commit *head)
{
	மாறிலி எழுத்து *head_arg = "HEAD";

	எனில் (repo_refresh_and_write_index(the_repository, REFRESH_QUIET,
					 SKIP_IF_UNCHANGED, 0, வெற்று, வெற்று,
					 வெற்று) < 0)
		die(_("Unable to write index."));

	எனில் (!strcmp(strategy, "recursive") || !strcmp(strategy, "subtree") ||
	    !strcmp(strategy, "ort")) {
		கட்டமைப்பு lock_file lock = LOCK_INIT;
		முழு clean, x;
		கட்டமைப்பு commit *result;
		கட்டமைப்பு commit_list *reversed = வெற்று;
		கட்டமைப்பு merge_options o;
		கட்டமைப்பு commit_list *j;

		எனில் (remoteheads->next) {
			error(_("Not handling anything other than two heads merge."));
			திரும்பு 2;
		}

		init_ui_merge_options(&o, the_repository);
		எனில் (!strcmp(strategy, "subtree"))
			o.subtree_shift = "";

		o.show_rename_progress =
			show_progress == -1 ? isatty(2) : show_progress;

		க்கு (x = 0; x < xopts.nr; x++)
			எனில் (parse_merge_opt(&o, xopts.v[x]))
				die(_("unknown strategy option: -X%s"), xopts.v[x]);

		o.branch1 = head_arg;
		o.branch2 = merge_remote_util(remoteheads->item)->name;

		க்கு (j = common; j; j = j->next)
			commit_list_செருகு(j->item, &reversed);

		repo_hold_locked_index(the_repository, &lock,
				       LOCK_DIE_ON_ERROR);
		clean = merge_ort_recursive(&o, head, remoteheads->item,
					    reversed, &result);
		commit_list_free(reversed);
		strbuf_release(&o.obuf);

		எனில் (clean < 0) {
			rollback_lock_file(&lock);
			திரும்பு 2;
		}
		எனில் (write_locked_index(the_repository->index, &lock,
				       COMMIT_LOCK | SKIP_IF_UNCHANGED))
			die(_("unable to write %s"), repo_get_index_file(the_repository));
		திரும்பு clean ? 0 : 1;
	} இல்லையெனில் {
		திரும்பு try_merge_command(the_repository,
					 strategy, xopts.nr, xopts.v,
					 common, head_arg, remoteheads);
	}
}

நிலையான வெறுமை மொத்தம்_diff_files(கட்டமைப்பு diff_queue_struct *q,
			     கட்டமைப்பு diff_options *opt UNUSED, வெறுமை *data)
{
	முழு *மொத்தம் = data;

	(*மொத்தம்) += q->nr;
}

நிலையான முழு மொத்தம்_unmerged_entries(வெறுமை)
{
	முழு i, ret = 0;

	க்கு (i = 0; i < the_repository->index->cache_nr; i++)
		எனில் (ce_stage(the_repository->index->cache[i]))
			ret++;

	திரும்பு ret;
}

நிலையான வெறுமை add_strategies(மாறிலி எழுத்து *string, unsigned attr)
{
	முழு i;

	எனில் (string) {
		கட்டமைப்பு string_list list = STRING_LIST_INIT_DUP;
		கட்டமைப்பு string_list_item *item;
		string_list_split(&list, string, " ", -1);
		for_each_string_list_item(item, &list)
			append_strategy(get_strategy(item->string));
		string_list_clear(&list, 0);
		திரும்பு;
	}
	க்கு (i = 0; i < ARRAY_SIZE(all_strategy); i++)
		எனில் (all_strategy[i].attr & attr)
			append_strategy(&all_strategy[i]);

}

நிலையான வெறுமை read_merge_செய்தி(கட்டமைப்பு strbuf *செய்தி)
{
	மாறிலி எழுத்து *filename = git_path_merge_செய்தி(the_repository);
	strbuf_reset(செய்தி);
	எனில் (strbuf_read_file(செய்தி, filename, 0) < 0)
		die_errno(_("Could not read from '%s'"), filename);
}

நிலையான வெறுமை write_merge_state(கட்டமைப்பு commit_list *);
நிலையான வெறுமை abort_commit(கட்டமைப்பு commit_list *remoteheads, மாறிலி எழுத்து *err_செய்தி)
{
	எனில் (err_செய்தி)
		error("%s", err_செய்தி);
	fprintf(stderr,
		_("Not committing merge; use 'git commit' to complete the merge.\n"));
	write_merge_state(remoteheads);
	exit(1);
}

நிலையான மாறிலி எழுத்து merge_editor_comment[] =
N_("Please enter a commit message to explain why this merge is necessary,\n"
   "especially if it merges an updated upstream into a topic branch.\n"
   "\n");

நிலையான மாறிலி எழுத்து scissors_editor_comment[] =
N_("An empty message aborts the commit.\n");

நிலையான மாறிலி எழுத்து no_scissors_editor_comment[] =
N_("Lines starting with '%s' will be ignored, and an empty message aborts\n"
   "the commit.\n");

நிலையான வெறுமை write_merge_heads(கட்டமைப்பு commit_list *);
நிலையான வெறுமை prepare_to_commit(கட்டமைப்பு commit_list *remoteheads)
{
	கட்டமைப்பு strbuf செய்தி = STRBUF_INIT;
	மாறிலி எழுத்து *index_file = repo_get_index_file(the_repository);

	எனில் (!no_verify) {
		முழு invoked_hook;

		எனில் (run_commit_hook(0 < option_edit, index_file, &invoked_hook,
				    "pre-merge-commit", வெற்று))
			abort_commit(remoteheads, வெற்று);
		/*
		 * Re-read the index as pre-merge-commit hook could have updated it,
		 * and write it out as a tree.  We must do this before we invoke
		 * the editor and after we invoke run_status above.
		 */
		எனில் (invoked_hook)
			discard_index(the_repository->index);
	}
	read_index_from(the_repository->index, index_file,
			repo_get_git_dir(the_repository));
	strbuf_addbuf(&செய்தி, &merge_செய்தி);
	எனில் (squash)
		BUG("the control must not reach here under --squash");
	எனில் (0 < option_edit) {
		strbuf_addch(&செய்தி, '\n');
		எனில் (cleanup_mode == COMMIT_செய்தி_CLEANUP_SCISSORS) {
			wt_status_append_cut_வரி(&செய்தி);
			strbuf_commented_addf(&செய்தி, comment_வரி_str, "\n");
		}
		strbuf_commented_addf(&செய்தி, comment_வரி_str,
				      _(merge_editor_comment));
		எனில் (cleanup_mode == COMMIT_செய்தி_CLEANUP_SCISSORS)
			strbuf_commented_addf(&செய்தி, comment_வரி_str,
					      _(scissors_editor_comment));
		இல்லையெனில்
			strbuf_commented_addf(&செய்தி, comment_வரி_str,
				_(no_scissors_editor_comment), comment_வரி_str);
	}
	எனில் (signoff)
		append_signoff(&செய்தி, ignored_log_message_bytes(செய்தி.buf, செய்தி.len), 0);
	write_merge_heads(remoteheads);
	write_file_buf(git_path_merge_செய்தி(the_repository), செய்தி.buf, செய்தி.len);
	எனில் (run_commit_hook(0 < option_edit, repo_get_index_file(the_repository),
			    வெற்று, "prepare-commit-msg",
			    git_path_merge_செய்தி(the_repository), "merge", வெற்று))
		abort_commit(remoteheads, வெற்று);
	எனில் (0 < option_edit) {
		எனில் (launch_editor(git_path_merge_செய்தி(the_repository), வெற்று, வெற்று))
			abort_commit(remoteheads, வெற்று);
	}

	எனில் (!no_verify && run_commit_hook(0 < option_edit, repo_get_index_file(the_repository),
					  வெற்று, "commit-msg",
					  git_path_merge_செய்தி(the_repository), வெற்று))
		abort_commit(remoteheads, வெற்று);

	read_merge_செய்தி(&செய்தி);
	cleanup_message(&செய்தி, cleanup_mode, 0);
	எனில் (!செய்தி.len)
		abort_commit(remoteheads, _("Empty commit message."));
	strbuf_release(&merge_செய்தி);
	strbuf_addbuf(&merge_செய்தி, &செய்தி);
	strbuf_release(&செய்தி);
}

நிலையான முழு merge_trivial(கட்டமைப்பு commit *head, கட்டமைப்பு commit_list *remoteheads)
{
	கட்டமைப்பு object_id result_மரம், result_commit;
	கட்டமைப்பு commit_list *parents = வெற்று, **pptr = &parents;

	எனில் (repo_refresh_and_write_index(the_repository, REFRESH_QUIET,
					 SKIP_IF_UNCHANGED, 0, வெற்று, வெற்று,
					 வெற்று) < 0)
		திரும்பு error(_("Unable to write index."));

	write_மரம்_trivial(&result_மரம்);
	printf(_("Wonderful.\n"));
	pptr = commit_list_append(head, pptr);
	pptr = commit_list_append(remoteheads->item, pptr);
	prepare_to_commit(remoteheads);
	எனில் (commit_மரம்(merge_செய்தி.buf, merge_செய்தி.len, &result_மரம், parents,
			&result_commit, வெற்று, sign_commit))
		die(_("failed to write commit object"));
	finish(head, remoteheads, &result_commit, "In-index merge");

	remove_merge_branch_state(the_repository);
	commit_list_free(parents);
	திரும்பு 0;
}

நிலையான முழு finish_automerge(கட்டமைப்பு commit *head,
			    முழு head_subsumed,
			    கட்டமைப்பு commit_list *common,
			    கட்டமைப்பு commit_list *remoteheads,
			    கட்டமைப்பு object_id *result_மரம்,
			    மாறிலி எழுத்து *wt_strategy)
{
	கட்டமைப்பு commit_list *parents = வெற்று;
	கட்டமைப்பு strbuf buf = STRBUF_INIT;
	கட்டமைப்பு object_id result_commit;

	write_மரம்_trivial(result_மரம்);
	commit_list_free(common);
	parents = remoteheads;
	எனில் (!head_subsumed || fast_forward == FF_NO)
		commit_list_செருகு(head, &parents);
	prepare_to_commit(remoteheads);
	எனில் (commit_மரம்(merge_செய்தி.buf, merge_செய்தி.len, result_மரம், parents,
			&result_commit, வெற்று, sign_commit))
		die(_("failed to write commit object"));
	strbuf_addf(&buf, "Merge made by the '%s' strategy.", wt_strategy);
	finish(head, remoteheads, &result_commit, buf.buf);

	strbuf_release(&buf);
	remove_merge_branch_state(the_repository);
	commit_list_free(parents);
	திரும்பு 0;
}

நிலையான முழு suggest_conflicts(வெறுமை)
{
	மாறிலி எழுத்து *filename;
	FILE *fp;
	கட்டமைப்பு strbuf msgbuf = STRBUF_INIT;

	filename = git_path_merge_செய்தி(the_repository);
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
	திரும்பு 1;
}

நிலையான முழு evaluate_result(வெறுமை)
{
	முழு cnt = 0;
	கட்டமைப்பு rev_info rev;

	/* Check how many files differ. */
	repo_init_revisions(the_repository, &rev, "");
	setup_revisions(0, வெற்று, &rev, வெற்று);
	rev.diffopt.output_format |=
		DIFF_FORMAT_CALLBACK;
	rev.diffopt.format_callback = மொத்தம்_diff_files;
	rev.diffopt.format_callback_data = &cnt;
	run_diff_files(&rev, 0);

	/*
	 * Check how many unmerged entries are
	 * there.
	 */
	cnt += மொத்தம்_unmerged_entries();

	release_revisions(&rev);
	திரும்பு cnt;
}

/*
 * Pretend as if the user told us to merge with the remote-tracking
 * branch we have for the upstream of the current branch
 */
நிலையான முழு setup_with_upstream(மாறிலி எழுத்து ***argv)
{
	கட்டமைப்பு branch *branch = branch_get(வெற்று);
	முழு i;
	மாறிலி எழுத்து **args;

	எனில் (!branch)
		die(_("No current branch."));
	எனில் (!branch->remote_name)
		die(_("No remote for the current branch."));
	எனில் (!branch->merge_nr)
		die(_("No default upstream defined for the current branch."));

	args = xcalloc(st_add(branch->merge_nr, 1), அளவு(எழுத்து *));
	க்கு (i = 0; i < branch->merge_nr; i++) {
		எனில் (!branch->merge[i]->dst)
			die(_("No remote-tracking branch for %s from %s"),
			    branch->merge[i]->src, branch->remote_name);
		args[i] = branch->merge[i]->dst;
	}
	args[i] = வெற்று;
	*argv = args;
	திரும்பு i;
}

நிலையான வெறுமை write_merge_heads(கட்டமைப்பு commit_list *remoteheads)
{
	கட்டமைப்பு commit_list *j;
	கட்டமைப்பு strbuf buf = STRBUF_INIT;

	க்கு (j = remoteheads; j; j = j->next) {
		கட்டமைப்பு object_id *oid;
		கட்டமைப்பு commit *c = j->item;
		கட்டமைப்பு merge_remote_desc *desc;

		desc = merge_remote_util(c);
		எனில் (desc && desc->obj) {
			oid = &desc->obj->oid;
		} இல்லையெனில் {
			oid = &c->object.oid;
		}
		strbuf_addf(&buf, "%s\n", oid_to_hex(oid));
	}
	write_file_buf(git_path_merge_head(the_repository), buf.buf, buf.len);

	strbuf_reset(&buf);
	எனில் (fast_forward == FF_NO)
		strbuf_addstr(&buf, "no-ff");
	write_file_buf(git_path_merge_mode(the_repository), buf.buf, buf.len);
	strbuf_release(&buf);
}

நிலையான வெறுமை write_merge_state(கட்டமைப்பு commit_list *remoteheads)
{
	write_merge_heads(remoteheads);
	strbuf_addch(&merge_செய்தி, '\n');
	write_file_buf(git_path_merge_செய்தி(the_repository), merge_செய்தி.buf,
		       merge_செய்தி.len);
}

நிலையான முழு default_edit_option(வெறுமை)
{
	நிலையான மாறிலி எழுத்து name[] = "GIT_MERGE_AUTOEDIT";
	மாறிலி எழுத்து *e = getenv(name);
	கட்டமைப்பு stat st_stdin, st_stdout;

	எனில் (have_message)
		/* an explicit -m msg without --[no-]edit */
		திரும்பு 0;

	எனில் (e) {
		முழு v = git_parse_maybe_bool(e);
		எனில் (v < 0)
			die(_("Bad value '%s' in environment '%s'"), e, name);
		திரும்பு v;
	}

	/* Use editor if stdin and stdout are the same and is a tty */
	திரும்பு (!fstat(0, &st_stdin) &&
		!fstat(1, &st_stdout) &&
		isatty(0) && isatty(1) &&
		st_stdin.st_dev == st_stdout.st_dev &&
		st_stdin.st_ino == st_stdout.st_ino &&
		st_stdin.st_mode == st_stdout.st_mode);
}

நிலையான கட்டமைப்பு commit_list *reduce_parents(கட்டமைப்பு commit *head_commit,
					  முழு *head_subsumed,
					  கட்டமைப்பு commit_list *remoteheads)
{
	கட்டமைப்பு commit_list *parents, **remotes;

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

	remoteheads = வெற்று;
	remotes = &remoteheads;
	போது (parents) {
		கட்டமைப்பு commit *commit = எடு_commit(&parents);
		எனில் (commit == head_commit)
			*head_subsumed = 0;
		இல்லையெனில்
			remotes = &commit_list_செருகு(commit, remotes)->next;
	}
	திரும்பு remoteheads;
}

நிலையான வெறுமை prepare_merge_message(கட்டமைப்பு strbuf *merge_names, கட்டமைப்பு strbuf *merge_செய்தி)
{
	கட்டமைப்பு fmt_merge_செய்தி_opts opts;

	memset(&opts, 0, அளவு(opts));
	opts.add_title = !have_message;
	opts.shortlog_len = shortlog_len;
	opts.credit_people = (0 < option_edit);
	opts.into_name = into_name;

	fmt_merge_செய்தி(merge_names, merge_செய்தி, &opts);
	எனில் (merge_செய்தி->len)
		strbuf_setlen(merge_செய்தி, merge_செய்தி->len - 1);
}

நிலையான வெறுமை handle_fetch_head(கட்டமைப்பு commit_list **remotes, கட்டமைப்பு strbuf *merge_names)
{
	மாறிலி எழுத்து *filename;
	முழு fd, pos, npos;
	கட்டமைப்பு strbuf fetch_head_file = STRBUF_INIT;
	மாறிலி unsigned hexsz = the_hash_algo->hexsz;

	எனில் (!merge_names)
		merge_names = &fetch_head_file;

	filename = git_path_fetch_head(the_repository);
	fd = xopen(filename, O_RDONLY);

	எனில் (strbuf_read(merge_names, fd, 0) < 0)
		die_errno(_("could not read '%s'"), filename);
	எனில் (close(fd) < 0)
		die_errno(_("could not close '%s'"), filename);

	க்கு (pos = 0; pos < merge_names->len; pos = npos) {
		கட்டமைப்பு object_id oid;
		எழுத்து *ptr;
		கட்டமைப்பு commit *commit;

		ptr = strchr(merge_names->buf + pos, '\n');
		எனில் (ptr)
			npos = ptr - merge_names->buf + 1;
		இல்லையெனில்
			npos = merge_names->len;

		எனில் (npos - pos < hexsz + 2 ||
		    get_oid_hex(merge_names->buf + pos, &oid))
			commit = வெற்று; /* bad */
		இல்லையெனில் எனில் (memcmp(merge_names->buf + pos + hexsz, "\t\t", 2))
			தொடர்; /* not-for-merge */
		இல்லையெனில் {
			எழுத்து saved = merge_names->buf[pos + hexsz];
			merge_names->buf[pos + hexsz] = '\0';
			commit = get_merge_parent(merge_names->buf + pos);
			merge_names->buf[pos + hexsz] = saved;
		}
		எனில் (!commit) {
			எனில் (ptr)
				*ptr = '\0';
			die(_("not something we can merge in %s: %s"),
			    filename, merge_names->buf + pos);
		}
		remotes = &commit_list_செருகு(commit, remotes)->next;
	}

	எனில் (merge_names == &fetch_head_file)
		strbuf_release(&fetch_head_file);
}

நிலையான கட்டமைப்பு commit_list *collect_parents(கட்டமைப்பு commit *head_commit,
					   முழு *head_subsumed,
					   முழு argc, மாறிலி எழுத்து **argv,
					   கட்டமைப்பு strbuf *merge_செய்தி)
{
	முழு i;
	கட்டமைப்பு commit_list *remoteheads = வெற்று;
	கட்டமைப்பு commit_list **remotes = &remoteheads;
	கட்டமைப்பு strbuf merge_names = STRBUF_INIT, *autogen = வெற்று;

	எனில் (merge_செய்தி && (!have_message || shortlog_len))
		autogen = &merge_names;

	எனில் (head_commit)
		remotes = &commit_list_செருகு(head_commit, remotes)->next;

	எனில் (argc == 1 && !strcmp(argv[0], "FETCH_HEAD")) {
		handle_fetch_head(remotes, autogen);
		remoteheads = reduce_parents(head_commit, head_subsumed, remoteheads);
	} இல்லையெனில் {
		க்கு (i = 0; i < argc; i++) {
			கட்டமைப்பு commit *commit = get_merge_parent(argv[i]);
			எனில் (!commit)
				help_unknown_ref(argv[i], "merge",
						 _("not something we can merge"));
			remotes = &commit_list_செருகு(commit, remotes)->next;
		}
		remoteheads = reduce_parents(head_commit, head_subsumed, remoteheads);
		எனில் (autogen) {
			கட்டமைப்பு commit_list *p;
			க்கு (p = remoteheads; p; p = p->next)
				merge_name(merge_remote_util(p->item)->name, autogen);
		}
	}

	எனில் (autogen) {
		prepare_merge_message(autogen, merge_செய்தி);
		strbuf_release(autogen);
	}

	திரும்பு remoteheads;
}

நிலையான முழு merging_a_throwaway_tag(கட்டமைப்பு commit *commit)
{
	எழுத்து *tag_ref;
	கட்டமைப்பு object_id oid;
	முழு is_throwaway_tag = 0;

	/* Are we merging a tag? */
	எனில் (!merge_remote_util(commit) ||
	    !merge_remote_util(commit)->obj ||
	    merge_remote_util(commit)->obj->type != OBJ_TAG)
		திரும்பு is_throwaway_tag;

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
			  ((கட்டமைப்பு tag *)merge_remote_util(commit)->obj)->tag);
	எனில் (!refs_read_ref(get_main_ref_store(the_repository), tag_ref, &oid) &&
	    oideq(&oid, &merge_remote_util(commit)->obj->oid))
		is_throwaway_tag = 0;
	இல்லையெனில்
		is_throwaway_tag = 1;
	விடுவி(tag_ref);
	திரும்பு is_throwaway_tag;
}

முழு cmd_merge(முழு argc,
	      மாறிலி எழுத்து **argv,
	      மாறிலி எழுத்து *prefix,
	      கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு object_id result_மரம், stash, head_oid;
	கட்டமைப்பு commit *head_commit;
	கட்டமைப்பு strbuf buf = STRBUF_INIT;
	முழு i, ret = 0, head_subsumed;
	முழு best_cnt = -1, merge_was_ok = 0, automerge_was_ok = 0;
	கட்டமைப்பு commit_list *common = வெற்று;
	மாறிலி எழுத்து *best_strategy = வெற்று, *wt_strategy = வெற்று;
	கட்டமைப்பு commit_list *remoteheads = வெற்று, *p;
	வெறுமை *branch_to_free, *argv_to_free = வெற்று;
	முழு orig_argc = argc;
	முழு merge_log_config = -1;

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
						      வெற்று);
	எனில் (branch)
		skip_prefix(branch, "refs/heads/", &branch);

	init_diff_ui_defaults();
	repo_config(the_repository, git_merge_config, &merge_log_config);

	எனில் (!branch || is_null_oid(&head_oid))
		head_commit = வெற்று;
	இல்லையெனில்
		head_commit = lookup_commit_or_die(&head_oid, "HEAD");

	எனில் (branch_mergeoptions)
		parse_branch_merge_options(branch_mergeoptions);
	argc = parse_options(argc, argv, prefix, builtin_merge_options,
			builtin_merge_usage, 0);
	எனில் (shortlog_len < 0)
		shortlog_len = (merge_log_config > 0) ? merge_log_config : 0;

	எனில் (verbosity < 0 && show_progress == -1)
		show_progress = 0;

	எனில் (abort_current_merge) {
		முழு nargc = 2;
		மாறிலி எழுத்து *nargv[] = {"reset", "--merge", வெற்று};
		எழுத்து stash_oid_hex[GIT_MAX_HEXSZ + 1];
		கட்டமைப்பு object_id stash_oid = {0};

		எனில் (orig_argc != 2)
			usage_செய்தி_opt(_("--abort expects no arguments"),
			      builtin_merge_usage, builtin_merge_options);

		எனில் (!file_exists(git_path_merge_head(the_repository)))
			die(_("There is no merge to abort (MERGE_HEAD missing)."));

		எனில் (!refs_read_ref(get_main_ref_store(the_repository), "MERGE_AUTOSTASH", &stash_oid))
			refs_delete_ref(get_main_ref_store(the_repository),
					"", "MERGE_AUTOSTASH", &stash_oid,
					REF_NO_DEREF);

		/* Invoke 'git reset --merge' */
		ret = cmd_reset(nargc, nargv, prefix, the_repository);

		எனில் (!is_null_oid(&stash_oid)) {
			oid_to_hex_r(stash_oid_hex, &stash_oid);
			apply_autostash_oid(stash_oid_hex);
		}

		செல் done;
	}

	எனில் (quit_current_merge) {
		எனில் (orig_argc != 2)
			usage_செய்தி_opt(_("--quit expects no arguments"),
				      builtin_merge_usage,
				      builtin_merge_options);

		remove_merge_branch_state(the_repository);
		செல் done;
	}

	எனில் (continue_current_merge) {
		முழு nargc = 1;
		மாறிலி எழுத்து *nargv[] = {"commit", வெற்று};

		எனில் (orig_argc != 2)
			usage_செய்தி_opt(_("--continue expects no arguments"),
			      builtin_merge_usage, builtin_merge_options);

		எனில் (!file_exists(git_path_merge_head(the_repository)))
			die(_("There is no merge in progress (MERGE_HEAD missing)."));

		/* Invoke 'git commit' */
		ret = cmd_commit(nargc, nargv, prefix, the_repository);
		செல் done;
	}

	எனில் (repo_read_index_unmerged(the_repository))
		die_resolve_conflict("merge");

	எனில் (file_exists(git_path_merge_head(the_repository))) {
		/*
		 * There is no unmerged entry, don't advise 'git
		 * add/rm <file>', just 'git commit'.
		 */
		எனில் (advice_enabled(ADVICE_RESOLVE_CONFLICT))
			die(_("You have not concluded your merge (MERGE_HEAD exists).\n"
				  "Please, commit your changes before you merge."));
		இல்லையெனில்
			die(_("You have not concluded your merge (MERGE_HEAD exists)."));
	}
	எனில் (refs_ref_exists(get_main_ref_store(the_repository), "CHERRY_PICK_HEAD")) {
		எனில் (advice_enabled(ADVICE_RESOLVE_CONFLICT))
			die(_("You have not concluded your cherry-pick (CHERRY_PICK_HEAD exists).\n"
			    "Please, commit your changes before you merge."));
		இல்லையெனில்
			die(_("You have not concluded your cherry-pick (CHERRY_PICK_HEAD exists)."));
	}
	resolve_undo_clear_index(the_repository->index);

	எனில் (option_edit < 0)
		option_edit = default_edit_option();

	cleanup_mode = get_cleanup_mode(cleanup_arg, 0 < option_edit);

	எனில் (verbosity < 0)
		show_diffstat = 0;

	எனில் (squash) {
		எனில் (fast_forward == FF_NO)
			die(_("options '%s' and '%s' cannot be used together"), "--squash", "--no-ff.");
		எனில் (option_commit > 0)
			die(_("options '%s' and '%s' cannot be used together"), "--squash", "--commit.");
		/*
		 * squash can now silently disable option_commit - this is not
		 * a problem as it is only overriding the default, not a user
		 * supplied option.
		 */
		option_commit = 0;
	}

	எனில் (option_commit < 0)
		option_commit = 1;

	எனில் (!argc) {
		எனில் (default_to_upstream) {
			argc = setup_with_upstream(&argv);
			argv_to_free = argv;
		}
		இல்லையெனில்
			die(_("No commit specified and merge.defaultToUpstream not set."));
	} இல்லையெனில் எனில் (argc == 1 && !strcmp(argv[0], "-")) {
		argv[0] = "@{-1}";
	}

	எனில் (!argc)
		usage_with_options(builtin_merge_usage,
			builtin_merge_options);

	எனில் (!head_commit) {
		/*
		 * If the merged head is a valid one there is no reason
		 * to forbid "git merge" into a branch yet to be born.
		 * We do the same for "git pull".
		 */
		கட்டமைப்பு object_id *remote_head_oid;
		எனில் (squash)
			die(_("Squash commit into empty head not supported yet"));
		எனில் (fast_forward == FF_NO)
			die(_("Non-fast-forward commit does not make sense into "
			    "an empty head"));
		remoteheads = collect_parents(head_commit, &head_subsumed,
					      argc, argv, வெற்று);
		எனில் (!remoteheads)
			die(_("%s - not something we can merge"), argv[0]);
		எனில் (remoteheads->next)
			die(_("Can merge only exactly one commit into empty head"));

		எனில் (verify_signatures)
			verify_merge_signature(remoteheads->item, verbosity,
					       check_trust_level);

		remote_head_oid = &remoteheads->item->object.oid;
		read_empty(remote_head_oid);
		refs_update_ref(get_main_ref_store(the_repository),
				"initial pull", "HEAD", remote_head_oid, வெற்று,
				0,
				UPDATE_REFS_DIE_ON_ERR);
		செல் done;
	}

	/*
	 * All the rest are the commits being merged; prepare
	 * the standard merge summary message to be appended
	 * to the given message.
	 */
	remoteheads = collect_parents(head_commit, &head_subsumed,
				      argc, argv, &merge_செய்தி);

	எனில் (!head_commit || !argc)
		usage_with_options(builtin_merge_usage,
			builtin_merge_options);

	எனில் (verify_signatures) {
		க்கு (p = remoteheads; p; p = p->next) {
			verify_merge_signature(p->item, verbosity,
					       check_trust_level);
		}
	}

	strbuf_addstr(&buf, "merge");
	க்கு (p = remoteheads; p; p = p->next)
		strbuf_addf(&buf, " %s", merge_remote_util(p->item)->name);
	setenv("GIT_REFLOG_ACTION", buf.buf, 0);
	strbuf_reset(&buf);

	க்கு (p = remoteheads; p; p = p->next) {
		கட்டமைப்பு commit *commit = p->item;
		strbuf_addf(&buf, "GITHEAD_%s",
			    oid_to_hex(&commit->object.oid));
		setenv(buf.buf, merge_remote_util(commit)->name, 1);
		strbuf_reset(&buf);
		எனில் (fast_forward != FF_ONLY && merging_a_throwaway_tag(commit))
			fast_forward = FF_NO;
	}

	எனில் (!use_strategies) {
		எனில் (!remoteheads)
			; /* already up-to-date */
		இல்லையெனில் எனில் (!remoteheads->next)
			add_strategies(pull_twohead, DEFAULT_TWOHEAD);
		இல்லையெனில்
			add_strategies(pull_octopus, DEFAULT_OCTOPUS);
	}

	க்கு (i = 0; i < use_strategies_nr; i++) {
		எனில் (use_strategies[i]->attr & NO_FAST_FORWARD)
			fast_forward = FF_NO;
		எனில் (use_strategies[i]->attr & NO_TRIVIAL)
			allow_trivial = 0;
	}

	எனில் (!remoteheads)
		; /* already up-to-date */
	இல்லையெனில் எனில் (!remoteheads->next) {
		எனில் (repo_get_merge_bases(the_repository, head_commit,
					 remoteheads->item, &common) < 0) {
			ret = 2;
			செல் done;
		}
	} இல்லையெனில் {
		கட்டமைப்பு commit_list *list = remoteheads;
		commit_list_செருகு(head_commit, &list);
		எனில் (get_octopus_merge_bases(list, &common) < 0) {
			விடுவி(list);
			ret = 2;
			செல் done;
		}
		விடுவி(list);
	}

	refs_update_ref(get_main_ref_store(the_repository),
			"updating ORIG_HEAD", "ORIG_HEAD",
			&head_commit->object.oid, வெற்று, 0,
			UPDATE_REFS_DIE_ON_ERR);

	எனில் (remoteheads && !common) {
		/* No common ancestors found. */
		எனில் (!allow_unrelated_histories)
			die(_("refusing to merge unrelated histories"));
		/* otherwise, we need a real merge. */
	} இல்லையெனில் எனில் (!remoteheads ||
		 (!remoteheads->next && !common->next &&
		  common->item == remoteheads->item)) {
		/*
		 * If head can reach all the merge then we are up to date.
		 * but first the most common case of merging one remote.
		 */
		finish_up_to_date();
		செல் done;
	} இல்லையெனில் எனில் (fast_forward != FF_NO && !remoteheads->next &&
			!common->next &&
			oideq(&common->item->object.oid, &head_commit->object.oid)) {
		/* Again the most common case of merging one remote. */
		மாறிலி எழுத்து *செய்தி = have_message ?
			"Fast-forward (no commit created; -m option ignored)" :
			"Fast-forward";
		கட்டமைப்பு commit *commit;

		எனில் (verbosity >= 0) {
			printf(_("Updating %s..%s\n"),
			       repo_find_unique_abbrev(the_repository, &head_commit->object.oid,
						       DEFAULT_ABBREV),
			       repo_find_unique_abbrev(the_repository, &remoteheads->item->object.oid,
						       DEFAULT_ABBREV));
		}
		commit = remoteheads->item;
		எனில் (!commit) {
			ret = 1;
			செல் done;
		}

		எனில் (autostash)
			create_autostash_ref(the_repository, "MERGE_AUTOSTASH",
					     வெற்று, false);
		எனில் (checkout_fast_forward(the_repository,
					  &head_commit->object.oid,
					  &commit->object.oid,
					  overwrite_ignore)) {
			apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
					    வெற்று, வெற்று, வெற்று, வெற்று);
			ret = 1;
			செல் done;
		}

		finish(head_commit, remoteheads, &commit->object.oid, செய்தி);
		remove_merge_branch_state(the_repository);
		செல் done;
	} இல்லையெனில் எனில் (!remoteheads->next && common->next)
		;
		/*
		 * We are not doing octopus and not fast-forward.  Need
		 * a real merge.
		 */
	இல்லையெனில் எனில் (!remoteheads->next && !common->next && option_commit) {
		/*
		 * We are not doing octopus, not fast-forward, and have
		 * only one common.
		 */
		refresh_index(the_repository->index, REFRESH_QUIET, வெற்று, வெற்று, வெற்று);
		எனில் (allow_trivial && fast_forward != FF_ONLY) {
			/*
			 * Must first ensure that index matches HEAD before
			 * attempting a trivial merge.
			 */
			கட்டமைப்பு மரம் *head_மரம் = repo_get_commit_மரம்(the_repository,
								      head_commit);
			கட்டமைப்பு strbuf sb = STRBUF_INIT;

			எனில் (repo_index_has_changes(the_repository, head_மரம்,
						   &sb)) {
				error(_("Your local changes to the following files would be overwritten by merge:\n  %s"),
				      sb.buf);
				strbuf_release(&sb);
				ret = 2;
				செல் done;
			}

			/* See if it is really trivial. */
			git_committer_info(IDENT_STRICT);
			printf(_("Trying really trivial in-index merge...\n"));
			எனில் (!read_மரம்_trivial(&common->item->object.oid,
					       &head_commit->object.oid,
					       &remoteheads->item->object.oid)) {
				ret = merge_trivial(head_commit, remoteheads);
				செல் done;
			}
			printf(_("Nope.\n"));
		}
	} இல்லையெனில் {
		/*
		 * An octopus.  If we can reach all the remote we are up
		 * to date.
		 */
		முழு up_to_date = 1;
		கட்டமைப்பு commit_list *j;

		க்கு (j = remoteheads; j; j = j->next) {
			முழு ret = repo_in_merge_bases(the_repository,
						      j->item, head_commit);
			எனில் (ret < 0)
				exit(128);
			எனில் (!ret) {
				up_to_date = 0;
				நிறுத்து;
			}
		}
		எனில் (up_to_date) {
			finish_up_to_date();
			செல் done;
		}
	}

	எனில் (fast_forward == FF_ONLY)
		die_ff_impossible();

	எனில் (autostash)
		create_autostash_ref(the_repository, "MERGE_AUTOSTASH",
				     வெற்று, false);

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
	எனில் (save_state(&stash))
		oidclr(&stash, the_repository->hash_algo);

	க்கு (i = 0; i < use_strategies_nr; i++) {
		முழு ret, cnt;
		எனில் (i) {
			printf(_("Rewinding the tree to pristine...\n"));
			restore_state(&head_commit->object.oid, &stash);
		}
		எனில் (use_strategies_nr != 1)
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
		எனில் (ret < 2) {
			எனில் (!ret) {
				/*
				 * This strategy worked; no point in trying
				 * another.
				 */
				merge_was_ok = 1;
				best_strategy = wt_strategy;
				நிறுத்து;
			}
			cnt = (use_strategies_nr > 1) ? evaluate_result() : 0;
			எனில் (best_cnt <= 0 || cnt <= best_cnt) {
				best_strategy = wt_strategy;
				best_cnt = cnt;
			}
		}
	}

	/*
	 * If we have a resulting tree, that means the strategy module
	 * auto resolved the merge cleanly.
	 */
	எனில் (merge_was_ok && option_commit) {
		automerge_was_ok = 1;
		ret = finish_automerge(head_commit, head_subsumed,
				       common, remoteheads,
				       &result_மரம், wt_strategy);
		செல் done;
	}

	/*
	 * Pick the result from the best strategy and have the user fix
	 * it up.
	 */
	எனில் (!best_strategy) {
		restore_state(&head_commit->object.oid, &stash);
		எனில் (use_strategies_nr > 1)
			fprintf(stderr,
				_("No merge strategy handled the merge.\n"));
		இல்லையெனில்
			fprintf(stderr, _("Merge with strategy %s failed.\n"),
				use_strategies[0]->name);
		apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
				    வெற்று, வெற்று, வெற்று, வெற்று);
		ret = 2;
		செல் done;
	} இல்லையெனில் எனில் (best_strategy == wt_strategy)
		; /* We already have its result in the working tree. */
	இல்லையெனில் {
		printf(_("Rewinding the tree to pristine...\n"));
		restore_state(&head_commit->object.oid, &stash);
		printf(_("Using the %s strategy to prepare resolving by hand.\n"),
			best_strategy);
		try_merge_strategy(best_strategy, common, remoteheads,
				   head_commit);
	}

	எனில் (squash) {
		finish(head_commit, remoteheads, வெற்று, வெற்று);

		git_test_write_commit_graph_or_die(the_repository);
	} இல்லையெனில்
		write_merge_state(remoteheads);

	எனில் (merge_was_ok)
		fprintf(stderr, _("Automatic merge went well; "
			"stopped before committing as requested\n"));
	இல்லையெனில்
		ret = suggest_conflicts();
	எனில் (autostash)
		printf(_("When finished, apply stashed changes with `git stash pop`\n"));

done:
	எனில் (!automerge_was_ok) {
		commit_list_free(common);
		commit_list_free(remoteheads);
	}
	strbuf_release(&buf);
	விடுவி(branch_to_free);
	விடுவி(argv_to_free);
	விடுவி(pull_twohead);
	விடுவி(pull_octopus);
	discard_index(the_repository->index);
	திரும்பு ret;
}
