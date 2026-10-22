/*
 * Builtin "git merge"
 *
 * Copyright (c) 2008 Miklos Vajna <vmiklos@frugalware.org>
 *
 * Based on git-merge.sh by Junio C Hamano.
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#include "builtin.h"

#include "abspath.h"
#include "advice.h"
#include "config.h"
#include "editor.h"
#include "environment.h"
#include "gettext.h"
#include "hex.h"
#include "object-name.h"
#include "parse-options.h"
#include "lockfile.h"
#include "repository.h"
#include "run-command.h"
#include "hook.h"
#include "diff.h"
#include "diff-merges.h"
#include "refs.h"
#include "refspec.h"
#include "commit.h"
#include "diffcore.h"
#include "path.h"
#include "revision.h"
#include "unpack-trees.h"
#include "cache-tree.h"
#include "dir.h"
#include "color.h"
#include "rerere.h"
#include "help.h"
#include "merge.h"
#include "merge-ort-wrappers.h"
#include "resolve-undo.h"
#include "remote.h"
#include "fmt-merge-msg.h"
#include "sequencer.h"
#include "string-list.h"
#include "tag.h"
#include "alias.h"
#include "branch.h"
#include "commit-reach.h"
#include "wt-status.h"
#include "commit-graph.h"

#define DEFAULT_TWOHEAD (1<<0)
#define DEFAULT_OCTOPUS (1<<1)
#define NO_FAST_FORWARD (1<<2)
#define NO_TRIVIAL      (1<<3)

sa_nracanaa strategy {
	const var_na *name;
	unsigned attr;
};

static const var_na * const builtin_merge_usage[] = {
	N_("git merge [<options>] [<commit>...]"),
	"git merge --abort",
	"git merge --continue",
	NULL
};

#define MERGE_SHOW_DIFFSTAT 1
#define MERGE_SHOW_COMPACTSUMMARY 2

static จำนวนเต็ม show_diffstat = MERGE_SHOW_DIFFSTAT, shortlog_len = -1, squash;
static จำนวนเต็ม option_commit = -1;
static จำนวนเต็ม option_edit = -1;
static จำนวนเต็ม allow_trivial = 1, have_message, verify_signatures;
static จำนวนเต็ม check_trust_level = 1;
static จำนวนเต็ม overwrite_ignore = 1;
static sa_nracanaa strbuf merge_msg = STRBUF_INIT;
static sa_nracanaa strategy **use_strategies;
static size_t use_strategies_nr, use_strategies_alloc;
static sa_nracanaa strvec xopts = STRVEC_INIT;
static const var_na *branch;
static var_na *branch_mergeoptions;
static จำนวนเต็ม verbosity;
static จำนวนเต็ม allow_rerere_auto;
static จำนวนเต็ม abort_current_merge;
static จำนวนเต็ม quit_current_merge;
static จำนวนเต็ม continue_current_merge;
static จำนวนเต็ม allow_unrelated_histories;
static จำนวนเต็ม show_progress = -1;
static จำนวนเต็ม default_to_upstream = 1;
static จำนวนเต็ม signoff;
static const var_na *sign_commit;
static จำนวนเต็ม autostash;
static จำนวนเต็ม no_verify;
static var_na *into_name;

static sa_nracanaa strategy all_strategy[] = {
	{ "recursive",  NO_TRIVIAL },
	{ "octopus",    DEFAULT_OCTOPUS },
	{ "ort",        DEFAULT_TWOHEAD | NO_TRIVIAL },
	{ "resolve",    0 },
	{ "ours",       NO_FAST_FORWARD | NO_TRIVIAL },
	{ "subtree",    NO_FAST_FORWARD | NO_TRIVIAL },
};

static var_na *pull_twohead, *pull_octopus;

ga_nanaa ff_type {
	FF_NO,
	FF_ALLOW,
	FF_ONLY
};

static ga_nanaa ff_type fast_forward = FF_ALLOW;

static var_na *cleanup_arg;
static ga_nanaa commit_msg_cleanup_mode cleanup_mode;

static จำนวนเต็ม option_parse_message(const sa_nracanaa option *opt,
				const var_na *arg, จำนวนเต็ม unset)
{
	sa_nracanaa strbuf *buf = opt->value;

	ถ้า (unset)
		strbuf_setlen(buf, 0);
	มิฉะนั้น ถ้า (arg) {
		strbuf_addf(buf, "%s%s", buf->len ? "\n\n" : "", arg);
		have_message = 1;
	} มิฉะนั้น
		คืน error(_("switch `m' requires a value"));
	คืน 0;
}

static ga_nanaa parse_opt_result option_read_message(sa_nracanaa parse_opt_ctx_t *ctx,
						 const sa_nracanaa option *opt,
						 const var_na *arg_not_used,
						 จำนวนเต็ม unset)
{
	sa_nracanaa strbuf *buf = opt->value;
	const var_na *arg;

	BUG_ON_OPT_ARG(arg_not_used);
	ถ้า (unset)
		BUG("-F cannot be negated");

	ถ้า (ctx->opt) {
		arg = ctx->opt;
		ctx->opt = NULL;
	} มิฉะนั้น ถ้า (ctx->argc > 1) {
		ctx->argc--;
		arg = *++ctx->argv;
	} มิฉะนั้น
		คืน error(_("option `%s' requires a value"), opt->long_name);

	ถ้า (buf->len)
		strbuf_addch(buf, '\n');
	ถ้า (ctx->prefix && !is_absolute_path(arg))
		arg = prefix_filename(ctx->prefix, arg);
	ถ้า (strbuf_read_file(buf, arg, 0) < 0)
		คืน error(_("could not read file '%s'"), arg);
	have_message = 1;

	คืน 0;
}

static sa_nracanaa strategy *get_strategy(const var_na *name)
{
	จำนวนเต็ม i;
	sa_nracanaa strategy *ret;
	static sa_nracanaa cmdnames main_cmds = {0}, other_cmds = {0};
	static จำนวนเต็ม loaded;
	var_na *default_strategy = NULL;

	ถ้า (!name)
		คืน NULL;

	ถ้า (default_strategy &&
	    !strcmp(default_strategy, "ort") &&
	    !strcmp(name, "recursive")) {
		name = "ort";
	}

	สำหรับ (i = 0; i < ARRAY_SIZE(all_strategy); i++)
		ถ้า (!strcmp(name, all_strategy[i].name))
			คืน &all_strategy[i];

	ถ้า (!loaded) {
		sa_nracanaa cmdnames not_strategies = {0};
		loaded = 1;

		load_command_list("git-merge-", &main_cmds, &other_cmds);
		สำหรับ (i = 0; i < main_cmds.cnt; i++) {
			จำนวนเต็ม j, found = 0;
			sa_nracanaa cmdname *ent = main_cmds.names[i];
			สำหรับ (j = 0; !found && j < ARRAY_SIZE(all_strategy); j++)
				ถ้า (!xstrncmpz(all_strategy[j].name, ent->name, ent->len))
					found = 1;
			ถ้า (!found)
				add_cmdname(&not_strategies, ent->name, ent->len);
		}
		exclude_cmds(&main_cmds, &not_strategies);

		cmdnames_release(&not_strategies);
	}
	ถ้า (!is_in_cmdlist(&main_cmds, name) && !is_in_cmdlist(&other_cmds, name)) {
		fprintf(stderr, _("Could not find merge strategy '%s'.\n"), name);
		fprintf(stderr, _("Available strategies are:"));
		สำหรับ (i = 0; i < main_cmds.cnt; i++)
			fprintf(stderr, " %s", main_cmds.names[i]->name);
		fprintf(stderr, ".\n");
		ถ้า (other_cmds.cnt) {
			fprintf(stderr, _("Available custom strategies are:"));
			สำหรับ (i = 0; i < other_cmds.cnt; i++)
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
	คืน ret;
}

static void append_strategy(sa_nracanaa strategy *s)
{
	ALLOC_GROW(use_strategies, use_strategies_nr + 1, use_strategies_alloc);
	use_strategies[use_strategies_nr++] = s;
}

static จำนวนเต็ม option_parse_strategy(const sa_nracanaa option *opt UNUSED,
				 const var_na *name, จำนวนเต็ม unset)
{
	ถ้า (unset)
		คืน 0;

	append_strategy(get_strategy(name));
	คืน 0;
}

static จำนวนเต็ม option_parse_compact_summary(const sa_nracanaa option *opt,
					const var_na *name UNUSED, จำนวนเต็ม unset)
{
	จำนวนเต็ม *setting = opt->value;

	ถ้า (unset)
		*setting = 0;
	มิฉะนั้น
		*setting = MERGE_SHOW_COMPACTSUMMARY;
	คืน 0;
}

static sa_nracanaa option builtin_merge_options[] = {
	OPT_SET_INT('n', NULL, &show_diffstat,
		N_("do not show a diffstat at the end of the merge"), 0),
	OPT_BOOL(0, "stat", &show_diffstat,
		N_("show a diffstat at the end of the merge")),
	OPT_BOOL(0, "summary", &show_diffstat, N_("(synonym to --stat)")),
	OPT_CALLBACK_F(0, "compact-summary", &show_diffstat, NULL,
		       N_("show a compact-summary at the end of the merge"),
		       PARSE_OPT_NOARG,
		       option_parse_compact_summary),
	{
		.type = OPTION_INTEGER,
		.long_name = "log",
		.value = &shortlog_len,
		.precision = parimaa_na(shortlog_len),
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
	OPT_CALLBACK('s', "strategy", NULL, N_("strategy"),
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

static จำนวนเต็ม save_state(sa_nracanaa object_id *stash)
{
	จำนวนเต็ม len;
	sa_nracanaa child_process cp = CHILD_PROCESS_INIT;
	sa_nracanaa strbuf buffer = STRBUF_INIT;
	sa_nracanaa lock_file lock_file = LOCK_INIT;
	จำนวนเต็ม fd;
	จำนวนเต็ม rc = -1;

	fd = repo_hold_locked_index(the_repository, &lock_file, 0);
	refresh_index(the_repository->index, REFRESH_QUIET, NULL, NULL, NULL);
	ถ้า (0 <= fd)
		repo_update_index_if_able(the_repository, &lock_file);
	rollback_lock_file(&lock_file);

	strvec_pushl(&cp.args, "stash", "create", NULL);
	cp.out = -1;
	cp.git_cmd = 1;

	ถ้า (start_command(&cp))
		die(_("could not run stash."));
	len = strbuf_read(&buffer, cp.out, 1024);
	close(cp.out);

	ถ้า (finish_command(&cp) || len < 0)
		die(_("stash failed"));
	มิฉะนั้น ถ้า (!len)		/* no changes */
		goto out;
	strbuf_setlen(&buffer, buffer.len-1);
	ถ้า (repo_get_oid(the_repository, buffer.buf, stash))
		die(_("not a valid object: %s"), buffer.buf);
	rc = 0;
out:
	strbuf_release(&buffer);
	คืน rc;
}

static void read_empty(const sa_nracanaa object_id *oid)
{
	sa_nracanaa child_process cmd = CHILD_PROCESS_INIT;

	strvec_pushl(&cmd.args, "read-tree", "-m", "-u",
		     empty_tree_oid_hex(the_repository->hash_algo),
		     oid_to_hex(oid), NULL);
	cmd.git_cmd = 1;

	ถ้า (run_command(&cmd))
		die(_("read-tree failed"));
}

static void reset_hard(const sa_nracanaa object_id *oid)
{
	sa_nracanaa child_process cmd = CHILD_PROCESS_INIT;

	strvec_pushl(&cmd.args, "read-tree", "-v", "--reset", "-u",
		     oid_to_hex(oid), NULL);
	cmd.git_cmd = 1;

	ถ้า (run_command(&cmd))
		die(_("read-tree failed"));
}

static void restore_state(const sa_nracanaa object_id *head,
			  const sa_nracanaa object_id *stash)
{
	sa_nracanaa child_process cmd = CHILD_PROCESS_INIT;

	reset_hard(head);

	ถ้า (is_null_oid(stash))
		goto refresh_cache;

	strvec_pushl(&cmd.args, "stash", "apply", "--index", "--quiet", NULL);
	strvec_push(&cmd.args, oid_to_hex(stash));

	/*
	 * It is OK to ignore error here, for example when there was
	 * nothing to restore.
	 */
	cmd.git_cmd = 1;
	run_command(&cmd);

refresh_cache:
	discard_index(the_repository->index);
	ถ้า (repo_read_index(the_repository) < 0)
		die(_("could not read index"));
}

/* This is called when no merge was necessary. */
static void finish_up_to_date(void)
{
	ถ้า (verbosity >= 0) {
		ถ้า (squash)
			puts(_("Already up to date. (nothing to squash)"));
		มิฉะนั้น
			puts(_("Already up to date."));
	}
	remove_merge_branch_state(the_repository);
}

static void squash_message(sa_nracanaa commit *commit, sa_nracanaa commit_list *remoteheads)
{
	sa_nracanaa rev_info rev;
	sa_nracanaa strbuf out = STRBUF_INIT;
	sa_nracanaa commit_list *j;
	sa_nracanaa pretty_print_context ctx = {0};

	printf(_("Squash commit -- not updating HEAD\n"));

	repo_init_revisions(the_repository, &rev, NULL);
	diff_merges_suppress(&rev);
	rev.commit_format = CMIT_FMT_MEDIUM;

	commit->object.flags |= UNINTERESTING;
	add_pending_object(&rev, &commit->object, NULL);

	สำหรับ (j = remoteheads; j; j = j->next)
		add_pending_object(&rev, &j->item->object, NULL);

	setup_revisions(0, NULL, &rev, NULL);
	ถ้า (prepare_revision_walk(&rev))
		die(_("revision walk setup failed"));

	ctx.abbrev = rev.abbrev;
	ctx.date_mode = rev.date_mode;
	ctx.fmt = rev.commit_format;

	strbuf_addstr(&out, "Squashed commit of the following:\n");
	ขณะ ((commit = get_revision(&rev)) != NULL) {
		strbuf_addch(&out, '\n');
		strbuf_addf(&out, "commit %s\n",
			oid_to_hex(&commit->object.oid));
		pretty_print_commit(&ctx, commit, &out);
	}
	write_file_buf(git_path_squash_msg(the_repository), out.buf, out.len);
	strbuf_release(&out);
	release_revisions(&rev);
}

static void finish(sa_nracanaa commit *head_commit,
		   sa_nracanaa commit_list *remoteheads,
		   const sa_nracanaa object_id *new_head, const var_na *msg)
{
	sa_nracanaa strbuf reflog_message = STRBUF_INIT;
	const sa_nracanaa object_id *head = &head_commit->object.oid;

	ถ้า (!msg)
		strbuf_addstr(&reflog_message, getenv("GIT_REFLOG_ACTION"));
	มิฉะนั้น {
		ถ้า (verbosity >= 0)
			printf("%s\n", msg);
		strbuf_addf(&reflog_message, "%s: %s",
			getenv("GIT_REFLOG_ACTION"), msg);
	}
	ถ้า (squash) {
		squash_message(head_commit, remoteheads);
	} มิฉะนั้น {
		ถ้า (verbosity >= 0 && !merge_msg.len)
			printf(_("No merge message -- not updating HEAD\n"));
		มิฉะนั้น {
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
	ถ้า (new_head && show_diffstat) {
		sa_nracanaa diff_options opts;
		repo_diff_setup(the_repository, &opts);
		init_diffstat_widths(&opts);

		switch (show_diffstat) {
		pak_sa MERGE_SHOW_DIFFSTAT: /* 1 */
			opts.output_format |=
				DIFF_FORMAT_SUMMARY | DIFF_FORMAT_DIFFSTAT;
			bha_nga;
		pak_sa MERGE_SHOW_COMPACTSUMMARY: /* 2 */
			opts.output_format |= DIFF_FORMAT_DIFFSTAT;
			opts.flags.stat_with_summary = 1;
			bha_nga;
		default:
			bha_nga;
		}
		opts.detect_rename = DIFF_DETECT_RENAME;
		diff_setup_done(&opts);
		diff_tree_oid(head, new_head, "", &opts);
		diffcore_std(&opts);
		diff_flush(&opts);
	}

	/* Run a post-merge hook */
	run_hooks_l(the_repository, "post-merge", squash ? "1" : "0", NULL);

	ถ้า (new_head)
		apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
				    NULL, NULL, NULL, NULL);
	strbuf_release(&reflog_message);
}

/* Get the name for the merge commit's message. */
static void merge_name(const var_na *remote, sa_nracanaa strbuf *msg)
{
	sa_nracanaa commit *remote_head;
	sa_nracanaa object_id branch_head;
	sa_nracanaa strbuf bname = STRBUF_INIT;
	sa_nracanaa merge_remote_desc *desc;
	const var_na *ptr;
	var_na *found_ref = NULL;
	จำนวนเต็ม len, early;

	copy_branchname(the_repository, &bname, remote, 0);
	remote = bname.buf;

	oidclr(&branch_head, the_repository->hash_algo);
	remote_head = get_merge_parent(remote);
	ถ้า (!remote_head)
		die(_("'%s' does not point to a commit"), remote);

	ถ้า (repo_dwim_ref(the_repository, remote, strlen(remote), &branch_head,
			  &found_ref, 0) > 0) {
		ถ้า (starts_with(found_ref, "refs/heads/")) {
			strbuf_addf(msg, "%s\t\tbranch '%s' of .\n",
				    oid_to_hex(&branch_head), remote);
			goto cleanup;
		}
		ถ้า (starts_with(found_ref, "refs/tags/")) {
			strbuf_addf(msg, "%s\t\ttag '%s' of .\n",
				    oid_to_hex(&branch_head), remote);
			goto cleanup;
		}
		ถ้า (starts_with(found_ref, "refs/remotes/")) {
			strbuf_addf(msg, "%s\t\tremote-tracking branch '%s' of .\n",
				    oid_to_hex(&branch_head), remote);
			goto cleanup;
		}
	}

	/* See if remote matches <name>^^^.. or <name>~<number> */
	สำหรับ (len = 0, ptr = remote + strlen(remote);
	     remote < ptr && ptr[-1] == '^';
	     ptr--)
		len++;
	ถ้า (len)
		early = 1;
	มิฉะนั้น {
		early = 0;
		ptr = strrchr(remote, '~');
		ถ้า (ptr) {
			จำนวนเต็ม seen_nonzero = 0;

			len++; /* count ~ */
			ขณะ (*++ptr && isdigit(*ptr)) {
				seen_nonzero |= (*ptr != '0');
				len++;
			}
			ถ้า (*ptr)
				len = 0; /* not ...~<number> */
			มิฉะนั้น ถ้า (seen_nonzero)
				early = 1;
			มิฉะนั้น ถ้า (len == 1)
				early = 1; /* "name~" is "name~1"! */
		}
	}
	ถ้า (len) {
		sa_nracanaa strbuf truname = STRBUF_INIT;
		strbuf_addf(&truname, "refs/heads/%s", remote);
		strbuf_setlen(&truname, truname.len - len);
		ถ้า (refs_ref_exists(get_main_ref_store(the_repository), truname.buf)) {
			strbuf_addf(msg,
				    "%s\t\tbranch '%s'%s of .\n",
				    oid_to_hex(&remote_head->object.oid),
				    truname.buf + 11,
				    (early ? " (early part)" : ""));
			strbuf_release(&truname);
			goto cleanup;
		}
		strbuf_release(&truname);
	}

	desc = merge_remote_util(remote_head);
	ถ้า (desc && desc->obj && desc->obj->type == OBJ_TAG) {
		strbuf_addf(msg, "%s\t\t%s '%s'\n",
			    oid_to_hex(&desc->obj->oid),
			    type_name(desc->obj->type),
			    remote);
		goto cleanup;
	}

	strbuf_addf(msg, "%s\t\tcommit '%s'\n",
		oid_to_hex(&remote_head->object.oid), remote);
cleanup:
	free(found_ref);
	strbuf_release(&bname);
}

static void parse_branch_merge_options(var_na *bmo)
{
	const var_na **argv;
	จำนวนเต็ม argc;

	ถ้า (!bmo)
		คืน;
	argc = split_cmdline(bmo, &argv);
	ถ้า (argc < 0)
		die(_("Bad branch.%s.mergeoptions string: %s"), branch,
		    _(split_cmdline_strerror(argc)));
	REALLOC_ARRAY(argv, argc + 2);
	MOVE_ARRAY(argv + 1, argv, argc + 1);
	argc++;
	argv[0] = "branch.*.mergeoptions";
	parse_options(argc, argv, NULL, builtin_merge_options,
		      builtin_merge_usage, 0);
	free(argv);
}

static จำนวนเต็ม git_merge_config(const var_na *k, const var_na *v,
			    const sa_nracanaa config_context *ctx, void *cb)
{
	จำนวนเต็ม status;
	const var_na *str;

	ถ้า (branch &&
	    skip_prefix(k, "branch.", &str) &&
	    skip_prefix(str, branch, &str) &&
	    !strcmp(str, ".mergeoptions")) {
		free(branch_mergeoptions);
		branch_mergeoptions = xstrdup(v);
		คืน 0;
	}

	ถ้า (!strcmp(k, "merge.diffstat") || !strcmp(k, "merge.stat")) {
		จำนวนเต็ม val = git_parse_maybe_bool_text(v);
		switch (val) {
		pak_sa 0:
			show_diffstat = 0;
			bha_nga;
		pak_sa 1:
			show_diffstat = MERGE_SHOW_DIFFSTAT;
			bha_nga;
		default:
			ถ้า (!strcmp(v, "compact"))
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
			มิฉะนั้น
				/*
				 * A setting from a future?  It is not an
				 * error grave enough to fail the command.
				 * proceed using the default one.
				 */
				show_diffstat = MERGE_SHOW_DIFFSTAT;
			bha_nga;
		}
	} มิฉะนั้น ถ้า (!strcmp(k, "merge.verifysignatures")) {
		verify_signatures = git_config_bool(k, v);
	} มิฉะนั้น ถ้า (!strcmp(k, "pull.twohead")) {
		FREE_AND_NULL(pull_twohead);
		คืน git_config_string(&pull_twohead, k, v);
	} มิฉะนั้น ถ้า (!strcmp(k, "pull.octopus")) {
		FREE_AND_NULL(pull_octopus);
		คืน git_config_string(&pull_octopus, k, v);
	} มิฉะนั้น ถ้า (!strcmp(k, "commit.cleanup")) {
		คืน git_config_string(&cleanup_arg, k, v);
	} มิฉะนั้น ถ้า (!strcmp(k, "merge.ff")) {
		จำนวนเต็ม boolval = git_parse_maybe_bool(v);
		ถ้า (0 <= boolval) {
			fast_forward = boolval ? FF_ALLOW : FF_NO;
		} มิฉะนั้น ถ้า (v && !strcmp(v, "only")) {
			fast_forward = FF_ONLY;
		} /* do not barf on values from future versions of git */
		คืน 0;
	} มิฉะนั้น ถ้า (!strcmp(k, "merge.defaulttoupstream")) {
		default_to_upstream = git_config_bool(k, v);
		คืน 0;
	} มิฉะนั้น ถ้า (!strcmp(k, "commit.gpgsign")) {
		sign_commit = git_config_bool(k, v) ? "" : NULL;
		คืน 0;
	} มิฉะนั้น ถ้า (!strcmp(k, "gpg.mintrustlevel")) {
		check_trust_level = 0;
	} มิฉะนั้น ถ้า (!strcmp(k, "merge.autostash")) {
		autostash = git_config_bool(k, v);
		คืน 0;
	}

	status = fmt_merge_msg_config(k, v, ctx, cb);
	ถ้า (status)
		คืน status;
	คืน git_diff_ui_config(k, v, ctx, cb);
}

static จำนวนเต็ม read_tree_trivial(sa_nracanaa object_id *common, sa_nracanaa object_id *head,
			     sa_nracanaa object_id *one)
{
	จำนวนเต็ม i, nr_trees = 0;
	sa_nracanaa tree *trees[MAX_UNPACK_TREES];
	sa_nracanaa tree_desc t[MAX_UNPACK_TREES];
	sa_nracanaa unpack_trees_options opts;

	memset(&opts, 0, parimaa_na(opts));
	opts.head_idx = 2;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;
	opts.update = 1;
	opts.verbose_update = 1;
	opts.trivial_merges_only = 1;
	opts.merge = 1;
	opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
	trees[nr_trees] = repo_parse_tree_indirect(the_repository, common);
	ถ้า (!trees[nr_trees++])
		คืน -1;
	trees[nr_trees] = repo_parse_tree_indirect(the_repository, head);
	ถ้า (!trees[nr_trees++])
		คืน -1;
	trees[nr_trees] = repo_parse_tree_indirect(the_repository, one);
	ถ้า (!trees[nr_trees++])
		คืน -1;
	opts.fn = threeway_merge;
	cache_tree_free(&the_repository->index->cache_tree);
	สำหรับ (i = 0; i < nr_trees; i++) {
		repo_parse_tree(the_repository, trees[i]);
		init_tree_desc(t+i, &trees[i]->object.oid,
			       trees[i]->buffer, trees[i]->size);
	}
	ถ้า (unpack_trees(nr_trees, t, &opts))
		คืน -1;
	คืน 0;
}

static void write_tree_trivial(sa_nracanaa object_id *oid)
{
	ถ้า (write_index_as_tree(oid, the_repository->index,
				repo_get_index_file(the_repository),
				0, NULL))
		die(_("git write-tree failed to write a tree"));
}

static จำนวนเต็ม try_merge_strategy(const var_na *strategy, sa_nracanaa commit_list *common,
			      sa_nracanaa commit_list *remoteheads,
			      sa_nracanaa commit *head)
{
	const var_na *head_arg = "HEAD";

	ถ้า (repo_refresh_and_write_index(the_repository, REFRESH_QUIET,
					 SKIP_IF_UNCHANGED, 0, NULL, NULL,
					 NULL) < 0)
		die(_("Unable to write index."));

	ถ้า (!strcmp(strategy, "recursive") || !strcmp(strategy, "subtree") ||
	    !strcmp(strategy, "ort")) {
		sa_nracanaa lock_file lock = LOCK_INIT;
		จำนวนเต็ม clean, x;
		sa_nracanaa commit *result;
		sa_nracanaa commit_list *reversed = NULL;
		sa_nracanaa merge_options o;
		sa_nracanaa commit_list *j;

		ถ้า (remoteheads->next) {
			error(_("Not handling anything other than two heads merge."));
			คืน 2;
		}

		init_ui_merge_options(&o, the_repository);
		ถ้า (!strcmp(strategy, "subtree"))
			o.subtree_shift = "";

		o.show_rename_progress =
			show_progress == -1 ? isatty(2) : show_progress;

		สำหรับ (x = 0; x < xopts.nr; x++)
			ถ้า (parse_merge_opt(&o, xopts.v[x]))
				die(_("unknown strategy option: -X%s"), xopts.v[x]);

		o.branch1 = head_arg;
		o.branch2 = merge_remote_util(remoteheads->item)->name;

		สำหรับ (j = common; j; j = j->next)
			commit_list_insert(j->item, &reversed);

		repo_hold_locked_index(the_repository, &lock,
				       LOCK_DIE_ON_ERROR);
		clean = merge_ort_recursive(&o, head, remoteheads->item,
					    reversed, &result);
		commit_list_free(reversed);
		strbuf_release(&o.obuf);

		ถ้า (clean < 0) {
			rollback_lock_file(&lock);
			คืน 2;
		}
		ถ้า (write_locked_index(the_repository->index, &lock,
				       COMMIT_LOCK | SKIP_IF_UNCHANGED))
			die(_("unable to write %s"), repo_get_index_file(the_repository));
		คืน clean ? 0 : 1;
	} มิฉะนั้น {
		คืน try_merge_command(the_repository,
					 strategy, xopts.nr, xopts.v,
					 common, head_arg, remoteheads);
	}
}

static void count_diff_files(sa_nracanaa diff_queue_struct *q,
			     sa_nracanaa diff_options *opt UNUSED, void *data)
{
	จำนวนเต็ม *count = data;

	(*count) += q->nr;
}

static จำนวนเต็ม count_unmerged_entries(void)
{
	จำนวนเต็ม i, ret = 0;

	สำหรับ (i = 0; i < the_repository->index->cache_nr; i++)
		ถ้า (ce_stage(the_repository->index->cache[i]))
			ret++;

	คืน ret;
}

static void add_strategies(const var_na *string, unsigned attr)
{
	จำนวนเต็ม i;

	ถ้า (string) {
		sa_nracanaa string_list list = STRING_LIST_INIT_DUP;
		sa_nracanaa string_list_item *item;
		string_list_split(&list, string, " ", -1);
		for_each_string_list_item(item, &list)
			append_strategy(get_strategy(item->string));
		string_list_clear(&list, 0);
		คืน;
	}
	สำหรับ (i = 0; i < ARRAY_SIZE(all_strategy); i++)
		ถ้า (all_strategy[i].attr & attr)
			append_strategy(&all_strategy[i]);

}

static void read_merge_msg(sa_nracanaa strbuf *msg)
{
	const var_na *filename = git_path_merge_msg(the_repository);
	strbuf_reset(msg);
	ถ้า (strbuf_read_file(msg, filename, 0) < 0)
		die_errno(_("Could not read from '%s'"), filename);
}

static void write_merge_state(sa_nracanaa commit_list *);
static void abort_commit(sa_nracanaa commit_list *remoteheads, const var_na *err_msg)
{
	ถ้า (err_msg)
		error("%s", err_msg);
	fprintf(stderr,
		_("Not committing merge; use 'git commit' to complete the merge.\n"));
	write_merge_state(remoteheads);
	exit(1);
}

static const var_na merge_editor_comment[] =
N_("Please enter a commit message to explain why this merge is necessary,\n"
   "especially if it merges an updated upstream into a topic branch.\n"
   "\n");

static const var_na scissors_editor_comment[] =
N_("An empty message aborts the commit.\n");

static const var_na no_scissors_editor_comment[] =
N_("Lines starting with '%s' will be ignored, and an empty message aborts\n"
   "the commit.\n");

static void write_merge_heads(sa_nracanaa commit_list *);
static void prepare_to_commit(sa_nracanaa commit_list *remoteheads)
{
	sa_nracanaa strbuf msg = STRBUF_INIT;
	const var_na *index_file = repo_get_index_file(the_repository);

	ถ้า (!no_verify) {
		จำนวนเต็ม invoked_hook;

		ถ้า (run_commit_hook(0 < option_edit, index_file, &invoked_hook,
				    "pre-merge-commit", NULL))
			abort_commit(remoteheads, NULL);
		/*
		 * Re-read the index as pre-merge-commit hook could have updated it,
		 * and write it out as a tree.  We must do this before we invoke
		 * the editor and after we invoke run_status above.
		 */
		ถ้า (invoked_hook)
			discard_index(the_repository->index);
	}
	read_index_from(the_repository->index, index_file,
			repo_get_git_dir(the_repository));
	strbuf_addbuf(&msg, &merge_msg);
	ถ้า (squash)
		BUG("the control must not reach here under --squash");
	ถ้า (0 < option_edit) {
		strbuf_addch(&msg, '\n');
		ถ้า (cleanup_mode == COMMIT_MSG_CLEANUP_SCISSORS) {
			wt_status_append_cut_line(&msg);
			strbuf_commented_addf(&msg, comment_line_str, "\n");
		}
		strbuf_commented_addf(&msg, comment_line_str,
				      _(merge_editor_comment));
		ถ้า (cleanup_mode == COMMIT_MSG_CLEANUP_SCISSORS)
			strbuf_commented_addf(&msg, comment_line_str,
					      _(scissors_editor_comment));
		มิฉะนั้น
			strbuf_commented_addf(&msg, comment_line_str,
				_(no_scissors_editor_comment), comment_line_str);
	}
	ถ้า (signoff)
		append_signoff(&msg, ignored_log_message_bytes(msg.buf, msg.len), 0);
	write_merge_heads(remoteheads);
	write_file_buf(git_path_merge_msg(the_repository), msg.buf, msg.len);
	ถ้า (run_commit_hook(0 < option_edit, repo_get_index_file(the_repository),
			    NULL, "prepare-commit-msg",
			    git_path_merge_msg(the_repository), "merge", NULL))
		abort_commit(remoteheads, NULL);
	ถ้า (0 < option_edit) {
		ถ้า (launch_editor(git_path_merge_msg(the_repository), NULL, NULL))
			abort_commit(remoteheads, NULL);
	}

	ถ้า (!no_verify && run_commit_hook(0 < option_edit, repo_get_index_file(the_repository),
					  NULL, "commit-msg",
					  git_path_merge_msg(the_repository), NULL))
		abort_commit(remoteheads, NULL);

	read_merge_msg(&msg);
	cleanup_message(&msg, cleanup_mode, 0);
	ถ้า (!msg.len)
		abort_commit(remoteheads, _("Empty commit message."));
	strbuf_release(&merge_msg);
	strbuf_addbuf(&merge_msg, &msg);
	strbuf_release(&msg);
}

static จำนวนเต็ม merge_trivial(sa_nracanaa commit *head, sa_nracanaa commit_list *remoteheads)
{
	sa_nracanaa object_id result_tree, result_commit;
	sa_nracanaa commit_list *parents = NULL, **pptr = &parents;

	ถ้า (repo_refresh_and_write_index(the_repository, REFRESH_QUIET,
					 SKIP_IF_UNCHANGED, 0, NULL, NULL,
					 NULL) < 0)
		คืน error(_("Unable to write index."));

	write_tree_trivial(&result_tree);
	printf(_("Wonderful.\n"));
	pptr = commit_list_append(head, pptr);
	pptr = commit_list_append(remoteheads->item, pptr);
	prepare_to_commit(remoteheads);
	ถ้า (commit_tree(merge_msg.buf, merge_msg.len, &result_tree, parents,
			&result_commit, NULL, sign_commit))
		die(_("failed to write commit object"));
	finish(head, remoteheads, &result_commit, "In-index merge");

	remove_merge_branch_state(the_repository);
	commit_list_free(parents);
	คืน 0;
}

static จำนวนเต็ม finish_automerge(sa_nracanaa commit *head,
			    จำนวนเต็ม head_subsumed,
			    sa_nracanaa commit_list *common,
			    sa_nracanaa commit_list *remoteheads,
			    sa_nracanaa object_id *result_tree,
			    const var_na *wt_strategy)
{
	sa_nracanaa commit_list *parents = NULL;
	sa_nracanaa strbuf buf = STRBUF_INIT;
	sa_nracanaa object_id result_commit;

	write_tree_trivial(result_tree);
	commit_list_free(common);
	parents = remoteheads;
	ถ้า (!head_subsumed || fast_forward == FF_NO)
		commit_list_insert(head, &parents);
	prepare_to_commit(remoteheads);
	ถ้า (commit_tree(merge_msg.buf, merge_msg.len, result_tree, parents,
			&result_commit, NULL, sign_commit))
		die(_("failed to write commit object"));
	strbuf_addf(&buf, "Merge made by the '%s' strategy.", wt_strategy);
	finish(head, remoteheads, &result_commit, buf.buf);

	strbuf_release(&buf);
	remove_merge_branch_state(the_repository);
	commit_list_free(parents);
	คืน 0;
}

static จำนวนเต็ม suggest_conflicts(void)
{
	const var_na *filename;
	FILE *fp;
	sa_nracanaa strbuf msgbuf = STRBUF_INIT;

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
	คืน 1;
}

static จำนวนเต็ม evaluate_result(void)
{
	จำนวนเต็ม cnt = 0;
	sa_nracanaa rev_info rev;

	/* Check how many files differ. */
	repo_init_revisions(the_repository, &rev, "");
	setup_revisions(0, NULL, &rev, NULL);
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
	คืน cnt;
}

/*
 * Pretend as if the user told us to merge with the remote-tracking
 * branch we have for the upstream of the current branch
 */
static จำนวนเต็ม setup_with_upstream(const var_na ***argv)
{
	sa_nracanaa branch *branch = branch_get(NULL);
	จำนวนเต็ม i;
	const var_na **args;

	ถ้า (!branch)
		die(_("No current branch."));
	ถ้า (!branch->remote_name)
		die(_("No remote for the current branch."));
	ถ้า (!branch->merge_nr)
		die(_("No default upstream defined for the current branch."));

	args = xcalloc(st_add(branch->merge_nr, 1), parimaa_na(var_na *));
	สำหรับ (i = 0; i < branch->merge_nr; i++) {
		ถ้า (!branch->merge[i]->dst)
			die(_("No remote-tracking branch for %s from %s"),
			    branch->merge[i]->src, branch->remote_name);
		args[i] = branch->merge[i]->dst;
	}
	args[i] = NULL;
	*argv = args;
	คืน i;
}

static void write_merge_heads(sa_nracanaa commit_list *remoteheads)
{
	sa_nracanaa commit_list *j;
	sa_nracanaa strbuf buf = STRBUF_INIT;

	สำหรับ (j = remoteheads; j; j = j->next) {
		sa_nracanaa object_id *oid;
		sa_nracanaa commit *c = j->item;
		sa_nracanaa merge_remote_desc *desc;

		desc = merge_remote_util(c);
		ถ้า (desc && desc->obj) {
			oid = &desc->obj->oid;
		} มิฉะนั้น {
			oid = &c->object.oid;
		}
		strbuf_addf(&buf, "%s\n", oid_to_hex(oid));
	}
	write_file_buf(git_path_merge_head(the_repository), buf.buf, buf.len);

	strbuf_reset(&buf);
	ถ้า (fast_forward == FF_NO)
		strbuf_addstr(&buf, "no-ff");
	write_file_buf(git_path_merge_mode(the_repository), buf.buf, buf.len);
	strbuf_release(&buf);
}

static void write_merge_state(sa_nracanaa commit_list *remoteheads)
{
	write_merge_heads(remoteheads);
	strbuf_addch(&merge_msg, '\n');
	write_file_buf(git_path_merge_msg(the_repository), merge_msg.buf,
		       merge_msg.len);
}

static จำนวนเต็ม default_edit_option(void)
{
	static const var_na name[] = "GIT_MERGE_AUTOEDIT";
	const var_na *e = getenv(name);
	sa_nracanaa stat st_stdin, st_stdout;

	ถ้า (have_message)
		/* an explicit -m msg without --[no-]edit */
		คืน 0;

	ถ้า (e) {
		จำนวนเต็ม v = git_parse_maybe_bool(e);
		ถ้า (v < 0)
			die(_("Bad value '%s' in environment '%s'"), e, name);
		คืน v;
	}

	/* Use editor if stdin and stdout are the same and is a tty */
	คืน (!fstat(0, &st_stdin) &&
		!fstat(1, &st_stdout) &&
		isatty(0) && isatty(1) &&
		st_stdin.st_dev == st_stdout.st_dev &&
		st_stdin.st_ino == st_stdout.st_ino &&
		st_stdin.st_mode == st_stdout.st_mode);
}

static sa_nracanaa commit_list *reduce_parents(sa_nracanaa commit *head_commit,
					  จำนวนเต็ม *head_subsumed,
					  sa_nracanaa commit_list *remoteheads)
{
	sa_nracanaa commit_list *parents, **remotes;

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

	remoteheads = NULL;
	remotes = &remoteheads;
	ขณะ (parents) {
		sa_nracanaa commit *commit = pop_commit(&parents);
		ถ้า (commit == head_commit)
			*head_subsumed = 0;
		มิฉะนั้น
			remotes = &commit_list_insert(commit, remotes)->next;
	}
	คืน remoteheads;
}

static void prepare_merge_message(sa_nracanaa strbuf *merge_names, sa_nracanaa strbuf *merge_msg)
{
	sa_nracanaa fmt_merge_msg_opts opts;

	memset(&opts, 0, parimaa_na(opts));
	opts.add_title = !have_message;
	opts.shortlog_len = shortlog_len;
	opts.credit_people = (0 < option_edit);
	opts.into_name = into_name;

	fmt_merge_msg(merge_names, merge_msg, &opts);
	ถ้า (merge_msg->len)
		strbuf_setlen(merge_msg, merge_msg->len - 1);
}

static void handle_fetch_head(sa_nracanaa commit_list **remotes, sa_nracanaa strbuf *merge_names)
{
	const var_na *filename;
	จำนวนเต็ม fd, pos, npos;
	sa_nracanaa strbuf fetch_head_file = STRBUF_INIT;
	const unsigned hexsz = the_hash_algo->hexsz;

	ถ้า (!merge_names)
		merge_names = &fetch_head_file;

	filename = git_path_fetch_head(the_repository);
	fd = xopen(filename, O_RDONLY);

	ถ้า (strbuf_read(merge_names, fd, 0) < 0)
		die_errno(_("could not read '%s'"), filename);
	ถ้า (close(fd) < 0)
		die_errno(_("could not close '%s'"), filename);

	สำหรับ (pos = 0; pos < merge_names->len; pos = npos) {
		sa_nracanaa object_id oid;
		var_na *ptr;
		sa_nracanaa commit *commit;

		ptr = strchr(merge_names->buf + pos, '\n');
		ถ้า (ptr)
			npos = ptr - merge_names->buf + 1;
		มิฉะนั้น
			npos = merge_names->len;

		ถ้า (npos - pos < hexsz + 2 ||
		    get_oid_hex(merge_names->buf + pos, &oid))
			commit = NULL; /* bad */
		มิฉะนั้น ถ้า (memcmp(merge_names->buf + pos + hexsz, "\t\t", 2))
			continue; /* not-for-merge */
		มิฉะนั้น {
			var_na saved = merge_names->buf[pos + hexsz];
			merge_names->buf[pos + hexsz] = '\0';
			commit = get_merge_parent(merge_names->buf + pos);
			merge_names->buf[pos + hexsz] = saved;
		}
		ถ้า (!commit) {
			ถ้า (ptr)
				*ptr = '\0';
			die(_("not something we can merge in %s: %s"),
			    filename, merge_names->buf + pos);
		}
		remotes = &commit_list_insert(commit, remotes)->next;
	}

	ถ้า (merge_names == &fetch_head_file)
		strbuf_release(&fetch_head_file);
}

static sa_nracanaa commit_list *collect_parents(sa_nracanaa commit *head_commit,
					   จำนวนเต็ม *head_subsumed,
					   จำนวนเต็ม argc, const var_na **argv,
					   sa_nracanaa strbuf *merge_msg)
{
	จำนวนเต็ม i;
	sa_nracanaa commit_list *remoteheads = NULL;
	sa_nracanaa commit_list **remotes = &remoteheads;
	sa_nracanaa strbuf merge_names = STRBUF_INIT, *autogen = NULL;

	ถ้า (merge_msg && (!have_message || shortlog_len))
		autogen = &merge_names;

	ถ้า (head_commit)
		remotes = &commit_list_insert(head_commit, remotes)->next;

	ถ้า (argc == 1 && !strcmp(argv[0], "FETCH_HEAD")) {
		handle_fetch_head(remotes, autogen);
		remoteheads = reduce_parents(head_commit, head_subsumed, remoteheads);
	} มิฉะนั้น {
		สำหรับ (i = 0; i < argc; i++) {
			sa_nracanaa commit *commit = get_merge_parent(argv[i]);
			ถ้า (!commit)
				help_unknown_ref(argv[i], "merge",
						 _("not something we can merge"));
			remotes = &commit_list_insert(commit, remotes)->next;
		}
		remoteheads = reduce_parents(head_commit, head_subsumed, remoteheads);
		ถ้า (autogen) {
			sa_nracanaa commit_list *p;
			สำหรับ (p = remoteheads; p; p = p->next)
				merge_name(merge_remote_util(p->item)->name, autogen);
		}
	}

	ถ้า (autogen) {
		prepare_merge_message(autogen, merge_msg);
		strbuf_release(autogen);
	}

	คืน remoteheads;
}

static จำนวนเต็ม merging_a_throwaway_tag(sa_nracanaa commit *commit)
{
	var_na *tag_ref;
	sa_nracanaa object_id oid;
	จำนวนเต็ม is_throwaway_tag = 0;

	/* Are we merging a tag? */
	ถ้า (!merge_remote_util(commit) ||
	    !merge_remote_util(commit)->obj ||
	    merge_remote_util(commit)->obj->type != OBJ_TAG)
		คืน is_throwaway_tag;

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
			  ((sa_nracanaa tag *)merge_remote_util(commit)->obj)->tag);
	ถ้า (!refs_read_ref(get_main_ref_store(the_repository), tag_ref, &oid) &&
	    oideq(&oid, &merge_remote_util(commit)->obj->oid))
		is_throwaway_tag = 0;
	มิฉะนั้น
		is_throwaway_tag = 1;
	free(tag_ref);
	คืน is_throwaway_tag;
}

จำนวนเต็ม cmd_merge(จำนวนเต็ม argc,
	      const var_na **argv,
	      const var_na *prefix,
	      sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa object_id result_tree, stash, head_oid;
	sa_nracanaa commit *head_commit;
	sa_nracanaa strbuf buf = STRBUF_INIT;
	จำนวนเต็ม i, ret = 0, head_subsumed;
	จำนวนเต็ม best_cnt = -1, merge_was_ok = 0, automerge_was_ok = 0;
	sa_nracanaa commit_list *common = NULL;
	const var_na *best_strategy = NULL, *wt_strategy = NULL;
	sa_nracanaa commit_list *remoteheads = NULL, *p;
	void *branch_to_free, *argv_to_free = NULL;
	จำนวนเต็ม orig_argc = argc;
	จำนวนเต็ม merge_log_config = -1;

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
						      NULL);
	ถ้า (branch)
		skip_prefix(branch, "refs/heads/", &branch);

	init_diff_ui_defaults();
	repo_config(the_repository, git_merge_config, &merge_log_config);

	ถ้า (!branch || is_null_oid(&head_oid))
		head_commit = NULL;
	มิฉะนั้น
		head_commit = lookup_commit_or_die(&head_oid, "HEAD");

	ถ้า (branch_mergeoptions)
		parse_branch_merge_options(branch_mergeoptions);
	argc = parse_options(argc, argv, prefix, builtin_merge_options,
			builtin_merge_usage, 0);
	ถ้า (shortlog_len < 0)
		shortlog_len = (merge_log_config > 0) ? merge_log_config : 0;

	ถ้า (verbosity < 0 && show_progress == -1)
		show_progress = 0;

	ถ้า (abort_current_merge) {
		จำนวนเต็ม nargc = 2;
		const var_na *nargv[] = {"reset", "--merge", NULL};
		var_na stash_oid_hex[GIT_MAX_HEXSZ + 1];
		sa_nracanaa object_id stash_oid = {0};

		ถ้า (orig_argc != 2)
			usage_msg_opt(_("--abort expects no arguments"),
			      builtin_merge_usage, builtin_merge_options);

		ถ้า (!file_exists(git_path_merge_head(the_repository)))
			die(_("There is no merge to abort (MERGE_HEAD missing)."));

		ถ้า (!refs_read_ref(get_main_ref_store(the_repository), "MERGE_AUTOSTASH", &stash_oid))
			refs_delete_ref(get_main_ref_store(the_repository),
					"", "MERGE_AUTOSTASH", &stash_oid,
					REF_NO_DEREF);

		/* Invoke 'git reset --merge' */
		ret = cmd_reset(nargc, nargv, prefix, the_repository);

		ถ้า (!is_null_oid(&stash_oid)) {
			oid_to_hex_r(stash_oid_hex, &stash_oid);
			apply_autostash_oid(stash_oid_hex);
		}

		goto done;
	}

	ถ้า (quit_current_merge) {
		ถ้า (orig_argc != 2)
			usage_msg_opt(_("--quit expects no arguments"),
				      builtin_merge_usage,
				      builtin_merge_options);

		remove_merge_branch_state(the_repository);
		goto done;
	}

	ถ้า (continue_current_merge) {
		จำนวนเต็ม nargc = 1;
		const var_na *nargv[] = {"commit", NULL};

		ถ้า (orig_argc != 2)
			usage_msg_opt(_("--continue expects no arguments"),
			      builtin_merge_usage, builtin_merge_options);

		ถ้า (!file_exists(git_path_merge_head(the_repository)))
			die(_("There is no merge in progress (MERGE_HEAD missing)."));

		/* Invoke 'git commit' */
		ret = cmd_commit(nargc, nargv, prefix, the_repository);
		goto done;
	}

	ถ้า (repo_read_index_unmerged(the_repository))
		die_resolve_conflict("merge");

	ถ้า (file_exists(git_path_merge_head(the_repository))) {
		/*
		 * There is no unmerged entry, don't advise 'git
		 * add/rm <file>', just 'git commit'.
		 */
		ถ้า (advice_enabled(ADVICE_RESOLVE_CONFLICT))
			die(_("You have not concluded your merge (MERGE_HEAD exists).\n"
				  "Please, commit your changes before you merge."));
		มิฉะนั้น
			die(_("You have not concluded your merge (MERGE_HEAD exists)."));
	}
	ถ้า (refs_ref_exists(get_main_ref_store(the_repository), "CHERRY_PICK_HEAD")) {
		ถ้า (advice_enabled(ADVICE_RESOLVE_CONFLICT))
			die(_("You have not concluded your cherry-pick (CHERRY_PICK_HEAD exists).\n"
			    "Please, commit your changes before you merge."));
		มิฉะนั้น
			die(_("You have not concluded your cherry-pick (CHERRY_PICK_HEAD exists)."));
	}
	resolve_undo_clear_index(the_repository->index);

	ถ้า (option_edit < 0)
		option_edit = default_edit_option();

	cleanup_mode = get_cleanup_mode(cleanup_arg, 0 < option_edit);

	ถ้า (verbosity < 0)
		show_diffstat = 0;

	ถ้า (squash) {
		ถ้า (fast_forward == FF_NO)
			die(_("options '%s' and '%s' cannot be used together"), "--squash", "--no-ff.");
		ถ้า (option_commit > 0)
			die(_("options '%s' and '%s' cannot be used together"), "--squash", "--commit.");
		/*
		 * squash can now silently disable option_commit - this is not
		 * a problem as it is only overriding the default, not a user
		 * supplied option.
		 */
		option_commit = 0;
	}

	ถ้า (option_commit < 0)
		option_commit = 1;

	ถ้า (!argc) {
		ถ้า (default_to_upstream) {
			argc = setup_with_upstream(&argv);
			argv_to_free = argv;
		}
		มิฉะนั้น
			die(_("No commit specified and merge.defaultToUpstream not set."));
	} มิฉะนั้น ถ้า (argc == 1 && !strcmp(argv[0], "-")) {
		argv[0] = "@{-1}";
	}

	ถ้า (!argc)
		usage_with_options(builtin_merge_usage,
			builtin_merge_options);

	ถ้า (!head_commit) {
		/*
		 * If the merged head is a valid one there is no reason
		 * to forbid "git merge" into a branch yet to be born.
		 * We do the same for "git pull".
		 */
		sa_nracanaa object_id *remote_head_oid;
		ถ้า (squash)
			die(_("Squash commit into empty head not supported yet"));
		ถ้า (fast_forward == FF_NO)
			die(_("Non-fast-forward commit does not make sense into "
			    "an empty head"));
		remoteheads = collect_parents(head_commit, &head_subsumed,
					      argc, argv, NULL);
		ถ้า (!remoteheads)
			die(_("%s - not something we can merge"), argv[0]);
		ถ้า (remoteheads->next)
			die(_("Can merge only exactly one commit into empty head"));

		ถ้า (verify_signatures)
			verify_merge_signature(remoteheads->item, verbosity,
					       check_trust_level);

		remote_head_oid = &remoteheads->item->object.oid;
		read_empty(remote_head_oid);
		refs_update_ref(get_main_ref_store(the_repository),
				"initial pull", "HEAD", remote_head_oid, NULL,
				0,
				UPDATE_REFS_DIE_ON_ERR);
		goto done;
	}

	/*
	 * All the rest are the commits being merged; prepare
	 * the standard merge summary message to be appended
	 * to the given message.
	 */
	remoteheads = collect_parents(head_commit, &head_subsumed,
				      argc, argv, &merge_msg);

	ถ้า (!head_commit || !argc)
		usage_with_options(builtin_merge_usage,
			builtin_merge_options);

	ถ้า (verify_signatures) {
		สำหรับ (p = remoteheads; p; p = p->next) {
			verify_merge_signature(p->item, verbosity,
					       check_trust_level);
		}
	}

	strbuf_addstr(&buf, "merge");
	สำหรับ (p = remoteheads; p; p = p->next)
		strbuf_addf(&buf, " %s", merge_remote_util(p->item)->name);
	setenv("GIT_REFLOG_ACTION", buf.buf, 0);
	strbuf_reset(&buf);

	สำหรับ (p = remoteheads; p; p = p->next) {
		sa_nracanaa commit *commit = p->item;
		strbuf_addf(&buf, "GITHEAD_%s",
			    oid_to_hex(&commit->object.oid));
		setenv(buf.buf, merge_remote_util(commit)->name, 1);
		strbuf_reset(&buf);
		ถ้า (fast_forward != FF_ONLY && merging_a_throwaway_tag(commit))
			fast_forward = FF_NO;
	}

	ถ้า (!use_strategies) {
		ถ้า (!remoteheads)
			; /* already up-to-date */
		มิฉะนั้น ถ้า (!remoteheads->next)
			add_strategies(pull_twohead, DEFAULT_TWOHEAD);
		มิฉะนั้น
			add_strategies(pull_octopus, DEFAULT_OCTOPUS);
	}

	สำหรับ (i = 0; i < use_strategies_nr; i++) {
		ถ้า (use_strategies[i]->attr & NO_FAST_FORWARD)
			fast_forward = FF_NO;
		ถ้า (use_strategies[i]->attr & NO_TRIVIAL)
			allow_trivial = 0;
	}

	ถ้า (!remoteheads)
		; /* already up-to-date */
	มิฉะนั้น ถ้า (!remoteheads->next) {
		ถ้า (repo_get_merge_bases(the_repository, head_commit,
					 remoteheads->item, &common) < 0) {
			ret = 2;
			goto done;
		}
	} มิฉะนั้น {
		sa_nracanaa commit_list *list = remoteheads;
		commit_list_insert(head_commit, &list);
		ถ้า (get_octopus_merge_bases(list, &common) < 0) {
			free(list);
			ret = 2;
			goto done;
		}
		free(list);
	}

	refs_update_ref(get_main_ref_store(the_repository),
			"updating ORIG_HEAD", "ORIG_HEAD",
			&head_commit->object.oid, NULL, 0,
			UPDATE_REFS_DIE_ON_ERR);

	ถ้า (remoteheads && !common) {
		/* No common ancestors found. */
		ถ้า (!allow_unrelated_histories)
			die(_("refusing to merge unrelated histories"));
		/* otherwise, we need a real merge. */
	} มิฉะนั้น ถ้า (!remoteheads ||
		 (!remoteheads->next && !common->next &&
		  common->item == remoteheads->item)) {
		/*
		 * If head can reach all the merge then we are up to date.
		 * but first the most common case of merging one remote.
		 */
		finish_up_to_date();
		goto done;
	} มิฉะนั้น ถ้า (fast_forward != FF_NO && !remoteheads->next &&
			!common->next &&
			oideq(&common->item->object.oid, &head_commit->object.oid)) {
		/* Again the most common case of merging one remote. */
		const var_na *msg = have_message ?
			"Fast-forward (no commit created; -m option ignored)" :
			"Fast-forward";
		sa_nracanaa commit *commit;

		ถ้า (verbosity >= 0) {
			printf(_("Updating %s..%s\n"),
			       repo_find_unique_abbrev(the_repository, &head_commit->object.oid,
						       DEFAULT_ABBREV),
			       repo_find_unique_abbrev(the_repository, &remoteheads->item->object.oid,
						       DEFAULT_ABBREV));
		}
		commit = remoteheads->item;
		ถ้า (!commit) {
			ret = 1;
			goto done;
		}

		ถ้า (autostash)
			create_autostash_ref(the_repository, "MERGE_AUTOSTASH",
					     NULL, false);
		ถ้า (checkout_fast_forward(the_repository,
					  &head_commit->object.oid,
					  &commit->object.oid,
					  overwrite_ignore)) {
			apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
					    NULL, NULL, NULL, NULL);
			ret = 1;
			goto done;
		}

		finish(head_commit, remoteheads, &commit->object.oid, msg);
		remove_merge_branch_state(the_repository);
		goto done;
	} มิฉะนั้น ถ้า (!remoteheads->next && common->next)
		;
		/*
		 * We are not doing octopus and not fast-forward.  Need
		 * a real merge.
		 */
	มิฉะนั้น ถ้า (!remoteheads->next && !common->next && option_commit) {
		/*
		 * We are not doing octopus, not fast-forward, and have
		 * only one common.
		 */
		refresh_index(the_repository->index, REFRESH_QUIET, NULL, NULL, NULL);
		ถ้า (allow_trivial && fast_forward != FF_ONLY) {
			/*
			 * Must first ensure that index matches HEAD before
			 * attempting a trivial merge.
			 */
			sa_nracanaa tree *head_tree = repo_get_commit_tree(the_repository,
								      head_commit);
			sa_nracanaa strbuf sb = STRBUF_INIT;

			ถ้า (repo_index_has_changes(the_repository, head_tree,
						   &sb)) {
				error(_("Your local changes to the following files would be overwritten by merge:\n  %s"),
				      sb.buf);
				strbuf_release(&sb);
				ret = 2;
				goto done;
			}

			/* See if it is really trivial. */
			git_committer_info(IDENT_STRICT);
			printf(_("Trying really trivial in-index merge...\n"));
			ถ้า (!read_tree_trivial(&common->item->object.oid,
					       &head_commit->object.oid,
					       &remoteheads->item->object.oid)) {
				ret = merge_trivial(head_commit, remoteheads);
				goto done;
			}
			printf(_("Nope.\n"));
		}
	} มิฉะนั้น {
		/*
		 * An octopus.  If we can reach all the remote we are up
		 * to date.
		 */
		จำนวนเต็ม up_to_date = 1;
		sa_nracanaa commit_list *j;

		สำหรับ (j = remoteheads; j; j = j->next) {
			จำนวนเต็ม ret = repo_in_merge_bases(the_repository,
						      j->item, head_commit);
			ถ้า (ret < 0)
				exit(128);
			ถ้า (!ret) {
				up_to_date = 0;
				bha_nga;
			}
		}
		ถ้า (up_to_date) {
			finish_up_to_date();
			goto done;
		}
	}

	ถ้า (fast_forward == FF_ONLY)
		die_ff_impossible();

	ถ้า (autostash)
		create_autostash_ref(the_repository, "MERGE_AUTOSTASH",
				     NULL, false);

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
	ถ้า (save_state(&stash))
		oidclr(&stash, the_repository->hash_algo);

	สำหรับ (i = 0; i < use_strategies_nr; i++) {
		จำนวนเต็ม ret, cnt;
		ถ้า (i) {
			printf(_("Rewinding the tree to pristine...\n"));
			restore_state(&head_commit->object.oid, &stash);
		}
		ถ้า (use_strategies_nr != 1)
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
		ถ้า (ret < 2) {
			ถ้า (!ret) {
				/*
				 * This strategy worked; no point in trying
				 * another.
				 */
				merge_was_ok = 1;
				best_strategy = wt_strategy;
				bha_nga;
			}
			cnt = (use_strategies_nr > 1) ? evaluate_result() : 0;
			ถ้า (best_cnt <= 0 || cnt <= best_cnt) {
				best_strategy = wt_strategy;
				best_cnt = cnt;
			}
		}
	}

	/*
	 * If we have a resulting tree, that means the strategy module
	 * auto resolved the merge cleanly.
	 */
	ถ้า (merge_was_ok && option_commit) {
		automerge_was_ok = 1;
		ret = finish_automerge(head_commit, head_subsumed,
				       common, remoteheads,
				       &result_tree, wt_strategy);
		goto done;
	}

	/*
	 * Pick the result from the best strategy and have the user fix
	 * it up.
	 */
	ถ้า (!best_strategy) {
		restore_state(&head_commit->object.oid, &stash);
		ถ้า (use_strategies_nr > 1)
			fprintf(stderr,
				_("No merge strategy handled the merge.\n"));
		มิฉะนั้น
			fprintf(stderr, _("Merge with strategy %s failed.\n"),
				use_strategies[0]->name);
		apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
				    NULL, NULL, NULL, NULL);
		ret = 2;
		goto done;
	} มิฉะนั้น ถ้า (best_strategy == wt_strategy)
		; /* We already have its result in the working tree. */
	มิฉะนั้น {
		printf(_("Rewinding the tree to pristine...\n"));
		restore_state(&head_commit->object.oid, &stash);
		printf(_("Using the %s strategy to prepare resolving by hand.\n"),
			best_strategy);
		try_merge_strategy(best_strategy, common, remoteheads,
				   head_commit);
	}

	ถ้า (squash) {
		finish(head_commit, remoteheads, NULL, NULL);

		git_test_write_commit_graph_or_die(the_repository);
	} มิฉะนั้น
		write_merge_state(remoteheads);

	ถ้า (merge_was_ok)
		fprintf(stderr, _("Automatic merge went well; "
			"stopped before committing as requested\n"));
	มิฉะนั้น
		ret = suggest_conflicts();
	ถ้า (autostash)
		printf(_("When finished, apply stashed changes with `git stash pop`\n"));

done:
	ถ้า (!automerge_was_ok) {
		commit_list_free(common);
		commit_list_free(remoteheads);
	}
	strbuf_release(&buf);
	free(branch_to_free);
	free(argv_to_free);
	free(pull_twohead);
	free(pull_octopus);
	discard_index(the_repository->index);
	คืน ret;
}
