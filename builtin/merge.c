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

static ཧྲིལ show_diffstat = MERGE_SHOW_DIFFSTAT, shortlog_len = -1, squash;
static ཧྲིལ option_commit = -1;
static ཧྲིལ option_edit = -1;
static ཧྲིལ allow_trivial = 1, have_message, verify_signatures;
static ཧྲིལ check_trust_level = 1;
static ཧྲིལ overwrite_ignore = 1;
static sa_nracanaa strbuf merge_msg = STRBUF_INIT;
static sa_nracanaa strategy **use_strategies;
static size_t use_strategies_nr, use_strategies_alloc;
static sa_nracanaa strvec xopts = STRVEC_INIT;
static const var_na *branch;
static var_na *branch_mergeoptions;
static ཧྲིལ verbosity;
static ཧྲིལ allow_rerere_auto;
static ཧྲིལ abort_current_merge;
static ཧྲིལ quit_current_merge;
static ཧྲིལ continue_current_merge;
static ཧྲིལ allow_unrelated_histories;
static ཧྲིལ show_progress = -1;
static ཧྲིལ default_to_upstream = 1;
static ཧྲིལ signoff;
static const var_na *sign_commit;
static ཧྲིལ autostash;
static ཧྲིལ no_verify;
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

static ཧྲིལ option_parse_message(const sa_nracanaa option *opt,
				const var_na *arg, ཧྲིལ unset)
{
	sa_nracanaa strbuf *buf = opt->value;

	གལ་ཏེ (unset)
		strbuf_setlen(buf, 0);
	དེ་མིན གལ་ཏེ (arg) {
		strbuf_addf(buf, "%s%s", buf->len ? "\n\n" : "", arg);
		have_message = 1;
	} དེ་མིན
		ཕྱིར error(_("switch `m' requires a value"));
	ཕྱིར 0;
}

static ga_nanaa parse_opt_result option_read_message(sa_nracanaa parse_opt_ctx_t *ctx,
						 const sa_nracanaa option *opt,
						 const var_na *arg_not_used,
						 ཧྲིལ unset)
{
	sa_nracanaa strbuf *buf = opt->value;
	const var_na *arg;

	BUG_ON_OPT_ARG(arg_not_used);
	གལ་ཏེ (unset)
		BUG("-F cannot be negated");

	གལ་ཏེ (ctx->opt) {
		arg = ctx->opt;
		ctx->opt = NULL;
	} དེ་མིན གལ་ཏེ (ctx->argc > 1) {
		ctx->argc--;
		arg = *++ctx->argv;
	} དེ་མིན
		ཕྱིར error(_("option `%s' requires a value"), opt->long_name);

	གལ་ཏེ (buf->len)
		strbuf_addch(buf, '\n');
	གལ་ཏེ (ctx->prefix && !is_absolute_path(arg))
		arg = prefix_filename(ctx->prefix, arg);
	གལ་ཏེ (strbuf_read_file(buf, arg, 0) < 0)
		ཕྱིར error(_("could not read file '%s'"), arg);
	have_message = 1;

	ཕྱིར 0;
}

static sa_nracanaa strategy *get_strategy(const var_na *name)
{
	ཧྲིལ i;
	sa_nracanaa strategy *ret;
	static sa_nracanaa cmdnames main_cmds = {0}, other_cmds = {0};
	static ཧྲིལ loaded;
	var_na *default_strategy = NULL;

	གལ་ཏེ (!name)
		ཕྱིར NULL;

	གལ་ཏེ (default_strategy &&
	    !strcmp(default_strategy, "ort") &&
	    !strcmp(name, "recursive")) {
		name = "ort";
	}

	ཆེད་དུ (i = 0; i < ARRAY_SIZE(all_strategy); i++)
		གལ་ཏེ (!strcmp(name, all_strategy[i].name))
			ཕྱིར &all_strategy[i];

	གལ་ཏེ (!loaded) {
		sa_nracanaa cmdnames not_strategies = {0};
		loaded = 1;

		load_command_list("git-merge-", &main_cmds, &other_cmds);
		ཆེད་དུ (i = 0; i < main_cmds.cnt; i++) {
			ཧྲིལ j, found = 0;
			sa_nracanaa cmdname *ent = main_cmds.names[i];
			ཆེད་དུ (j = 0; !found && j < ARRAY_SIZE(all_strategy); j++)
				གལ་ཏེ (!xstrncmpz(all_strategy[j].name, ent->name, ent->len))
					found = 1;
			གལ་ཏེ (!found)
				add_cmdname(&not_strategies, ent->name, ent->len);
		}
		exclude_cmds(&main_cmds, &not_strategies);

		cmdnames_release(&not_strategies);
	}
	གལ་ཏེ (!is_in_cmdlist(&main_cmds, name) && !is_in_cmdlist(&other_cmds, name)) {
		fprintf(stderr, _("Could not find merge strategy '%s'.\n"), name);
		fprintf(stderr, _("Available strategies are:"));
		ཆེད་དུ (i = 0; i < main_cmds.cnt; i++)
			fprintf(stderr, " %s", main_cmds.names[i]->name);
		fprintf(stderr, ".\n");
		གལ་ཏེ (other_cmds.cnt) {
			fprintf(stderr, _("Available custom strategies are:"));
			ཆེད་དུ (i = 0; i < other_cmds.cnt; i++)
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
	ཕྱིར ret;
}

static void append_strategy(sa_nracanaa strategy *s)
{
	ALLOC_GROW(use_strategies, use_strategies_nr + 1, use_strategies_alloc);
	use_strategies[use_strategies_nr++] = s;
}

static ཧྲིལ option_parse_strategy(const sa_nracanaa option *opt UNUSED,
				 const var_na *name, ཧྲིལ unset)
{
	གལ་ཏེ (unset)
		ཕྱིར 0;

	append_strategy(get_strategy(name));
	ཕྱིར 0;
}

static ཧྲིལ option_parse_compact_summary(const sa_nracanaa option *opt,
					const var_na *name UNUSED, ཧྲིལ unset)
{
	ཧྲིལ *setting = opt->value;

	གལ་ཏེ (unset)
		*setting = 0;
	དེ་མིན
		*setting = MERGE_SHOW_COMPACTSUMMARY;
	ཕྱིར 0;
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

static ཧྲིལ save_state(sa_nracanaa object_id *stash)
{
	ཧྲིལ len;
	sa_nracanaa child_process cp = CHILD_PROCESS_INIT;
	sa_nracanaa strbuf buffer = STRBUF_INIT;
	sa_nracanaa lock_file lock_file = LOCK_INIT;
	ཧྲིལ fd;
	ཧྲིལ rc = -1;

	fd = repo_hold_locked_index(the_repository, &lock_file, 0);
	refresh_index(the_repository->index, REFRESH_QUIET, NULL, NULL, NULL);
	གལ་ཏེ (0 <= fd)
		repo_update_index_if_able(the_repository, &lock_file);
	rollback_lock_file(&lock_file);

	strvec_pushl(&cp.args, "stash", "create", NULL);
	cp.out = -1;
	cp.git_cmd = 1;

	གལ་ཏེ (start_command(&cp))
		die(_("could not run stash."));
	len = strbuf_read(&buffer, cp.out, 1024);
	close(cp.out);

	གལ་ཏེ (finish_command(&cp) || len < 0)
		die(_("stash failed"));
	དེ་མིན གལ་ཏེ (!len)		/* no changes */
		goto out;
	strbuf_setlen(&buffer, buffer.len-1);
	གལ་ཏེ (repo_get_oid(the_repository, buffer.buf, stash))
		die(_("not a valid object: %s"), buffer.buf);
	rc = 0;
out:
	strbuf_release(&buffer);
	ཕྱིར rc;
}

static void read_empty(const sa_nracanaa object_id *oid)
{
	sa_nracanaa child_process cmd = CHILD_PROCESS_INIT;

	strvec_pushl(&cmd.args, "read-tree", "-m", "-u",
		     empty_tree_oid_hex(the_repository->hash_algo),
		     oid_to_hex(oid), NULL);
	cmd.git_cmd = 1;

	གལ་ཏེ (run_command(&cmd))
		die(_("read-tree failed"));
}

static void reset_hard(const sa_nracanaa object_id *oid)
{
	sa_nracanaa child_process cmd = CHILD_PROCESS_INIT;

	strvec_pushl(&cmd.args, "read-tree", "-v", "--reset", "-u",
		     oid_to_hex(oid), NULL);
	cmd.git_cmd = 1;

	གལ་ཏེ (run_command(&cmd))
		die(_("read-tree failed"));
}

static void restore_state(const sa_nracanaa object_id *head,
			  const sa_nracanaa object_id *stash)
{
	sa_nracanaa child_process cmd = CHILD_PROCESS_INIT;

	reset_hard(head);

	གལ་ཏེ (is_null_oid(stash))
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
	གལ་ཏེ (repo_read_index(the_repository) < 0)
		die(_("could not read index"));
}

/* This is called when no merge was necessary. */
static void finish_up_to_date(void)
{
	གལ་ཏེ (verbosity >= 0) {
		གལ་ཏེ (squash)
			puts(_("Already up to date. (nothing to squash)"));
		དེ་མིན
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

	ཆེད་དུ (j = remoteheads; j; j = j->next)
		add_pending_object(&rev, &j->item->object, NULL);

	setup_revisions(0, NULL, &rev, NULL);
	གལ་ཏེ (prepare_revision_walk(&rev))
		die(_("revision walk setup failed"));

	ctx.abbrev = rev.abbrev;
	ctx.date_mode = rev.date_mode;
	ctx.fmt = rev.commit_format;

	strbuf_addstr(&out, "Squashed commit of the following:\n");
	སྐབས ((commit = get_revision(&rev)) != NULL) {
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

	གལ་ཏེ (!msg)
		strbuf_addstr(&reflog_message, getenv("GIT_REFLOG_ACTION"));
	དེ་མིན {
		གལ་ཏེ (verbosity >= 0)
			printf("%s\n", msg);
		strbuf_addf(&reflog_message, "%s: %s",
			getenv("GIT_REFLOG_ACTION"), msg);
	}
	གལ་ཏེ (squash) {
		squash_message(head_commit, remoteheads);
	} དེ་མིན {
		གལ་ཏེ (verbosity >= 0 && !merge_msg.len)
			printf(_("No merge message -- not updating HEAD\n"));
		དེ་མིན {
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
	གལ་ཏེ (new_head && show_diffstat) {
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

	གལ་ཏེ (new_head)
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
	ཧྲིལ len, early;

	copy_branchname(the_repository, &bname, remote, 0);
	remote = bname.buf;

	oidclr(&branch_head, the_repository->hash_algo);
	remote_head = get_merge_parent(remote);
	གལ་ཏེ (!remote_head)
		die(_("'%s' does not point to a commit"), remote);

	གལ་ཏེ (repo_dwim_ref(the_repository, remote, strlen(remote), &branch_head,
			  &found_ref, 0) > 0) {
		གལ་ཏེ (starts_with(found_ref, "refs/heads/")) {
			strbuf_addf(msg, "%s\t\tbranch '%s' of .\n",
				    oid_to_hex(&branch_head), remote);
			goto cleanup;
		}
		གལ་ཏེ (starts_with(found_ref, "refs/tags/")) {
			strbuf_addf(msg, "%s\t\ttag '%s' of .\n",
				    oid_to_hex(&branch_head), remote);
			goto cleanup;
		}
		གལ་ཏེ (starts_with(found_ref, "refs/remotes/")) {
			strbuf_addf(msg, "%s\t\tremote-tracking branch '%s' of .\n",
				    oid_to_hex(&branch_head), remote);
			goto cleanup;
		}
	}

	/* See if remote matches <name>^^^.. or <name>~<number> */
	ཆེད་དུ (len = 0, ptr = remote + strlen(remote);
	     remote < ptr && ptr[-1] == '^';
	     ptr--)
		len++;
	གལ་ཏེ (len)
		early = 1;
	དེ་མིན {
		early = 0;
		ptr = strrchr(remote, '~');
		གལ་ཏེ (ptr) {
			ཧྲིལ seen_nonzero = 0;

			len++; /* count ~ */
			སྐབས (*++ptr && isdigit(*ptr)) {
				seen_nonzero |= (*ptr != '0');
				len++;
			}
			གལ་ཏེ (*ptr)
				len = 0; /* not ...~<number> */
			དེ་མིན གལ་ཏེ (seen_nonzero)
				early = 1;
			དེ་མིན གལ་ཏེ (len == 1)
				early = 1; /* "name~" is "name~1"! */
		}
	}
	གལ་ཏེ (len) {
		sa_nracanaa strbuf truname = STRBUF_INIT;
		strbuf_addf(&truname, "refs/heads/%s", remote);
		strbuf_setlen(&truname, truname.len - len);
		གལ་ཏེ (refs_ref_exists(get_main_ref_store(the_repository), truname.buf)) {
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
	གལ་ཏེ (desc && desc->obj && desc->obj->type == OBJ_TAG) {
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
	ཧྲིལ argc;

	གལ་ཏེ (!bmo)
		ཕྱིར;
	argc = split_cmdline(bmo, &argv);
	གལ་ཏེ (argc < 0)
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

static ཧྲིལ git_merge_config(const var_na *k, const var_na *v,
			    const sa_nracanaa config_context *ctx, void *cb)
{
	ཧྲིལ status;
	const var_na *str;

	གལ་ཏེ (branch &&
	    skip_prefix(k, "branch.", &str) &&
	    skip_prefix(str, branch, &str) &&
	    !strcmp(str, ".mergeoptions")) {
		free(branch_mergeoptions);
		branch_mergeoptions = xstrdup(v);
		ཕྱིར 0;
	}

	གལ་ཏེ (!strcmp(k, "merge.diffstat") || !strcmp(k, "merge.stat")) {
		ཧྲིལ val = git_parse_maybe_bool_text(v);
		switch (val) {
		pak_sa 0:
			show_diffstat = 0;
			bha_nga;
		pak_sa 1:
			show_diffstat = MERGE_SHOW_DIFFSTAT;
			bha_nga;
		default:
			གལ་ཏེ (!strcmp(v, "compact"))
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
			དེ་མིན
				/*
				 * A setting from a future?  It is not an
				 * error grave enough to fail the command.
				 * proceed using the default one.
				 */
				show_diffstat = MERGE_SHOW_DIFFSTAT;
			bha_nga;
		}
	} དེ་མིན གལ་ཏེ (!strcmp(k, "merge.verifysignatures")) {
		verify_signatures = git_config_bool(k, v);
	} དེ་མིན གལ་ཏེ (!strcmp(k, "pull.twohead")) {
		FREE_AND_NULL(pull_twohead);
		ཕྱིར git_config_string(&pull_twohead, k, v);
	} དེ་མིན གལ་ཏེ (!strcmp(k, "pull.octopus")) {
		FREE_AND_NULL(pull_octopus);
		ཕྱིར git_config_string(&pull_octopus, k, v);
	} དེ་མིན གལ་ཏེ (!strcmp(k, "commit.cleanup")) {
		ཕྱིར git_config_string(&cleanup_arg, k, v);
	} དེ་མིན གལ་ཏེ (!strcmp(k, "merge.ff")) {
		ཧྲིལ boolval = git_parse_maybe_bool(v);
		གལ་ཏེ (0 <= boolval) {
			fast_forward = boolval ? FF_ALLOW : FF_NO;
		} དེ་མིན གལ་ཏེ (v && !strcmp(v, "only")) {
			fast_forward = FF_ONLY;
		} /* do not barf on values from future versions of git */
		ཕྱིར 0;
	} དེ་མིན གལ་ཏེ (!strcmp(k, "merge.defaulttoupstream")) {
		default_to_upstream = git_config_bool(k, v);
		ཕྱིར 0;
	} དེ་མིན གལ་ཏེ (!strcmp(k, "commit.gpgsign")) {
		sign_commit = git_config_bool(k, v) ? "" : NULL;
		ཕྱིར 0;
	} དེ་མིན གལ་ཏེ (!strcmp(k, "gpg.mintrustlevel")) {
		check_trust_level = 0;
	} དེ་མིན གལ་ཏེ (!strcmp(k, "merge.autostash")) {
		autostash = git_config_bool(k, v);
		ཕྱིར 0;
	}

	status = fmt_merge_msg_config(k, v, ctx, cb);
	གལ་ཏེ (status)
		ཕྱིར status;
	ཕྱིར git_diff_ui_config(k, v, ctx, cb);
}

static ཧྲིལ read_tree_trivial(sa_nracanaa object_id *common, sa_nracanaa object_id *head,
			     sa_nracanaa object_id *one)
{
	ཧྲིལ i, nr_trees = 0;
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
	གལ་ཏེ (!trees[nr_trees++])
		ཕྱིར -1;
	trees[nr_trees] = repo_parse_tree_indirect(the_repository, head);
	གལ་ཏེ (!trees[nr_trees++])
		ཕྱིར -1;
	trees[nr_trees] = repo_parse_tree_indirect(the_repository, one);
	གལ་ཏེ (!trees[nr_trees++])
		ཕྱིར -1;
	opts.fn = threeway_merge;
	cache_tree_free(&the_repository->index->cache_tree);
	ཆེད་དུ (i = 0; i < nr_trees; i++) {
		repo_parse_tree(the_repository, trees[i]);
		init_tree_desc(t+i, &trees[i]->object.oid,
			       trees[i]->buffer, trees[i]->size);
	}
	གལ་ཏེ (unpack_trees(nr_trees, t, &opts))
		ཕྱིར -1;
	ཕྱིར 0;
}

static void write_tree_trivial(sa_nracanaa object_id *oid)
{
	གལ་ཏེ (write_index_as_tree(oid, the_repository->index,
				repo_get_index_file(the_repository),
				0, NULL))
		die(_("git write-tree failed to write a tree"));
}

static ཧྲིལ try_merge_strategy(const var_na *strategy, sa_nracanaa commit_list *common,
			      sa_nracanaa commit_list *remoteheads,
			      sa_nracanaa commit *head)
{
	const var_na *head_arg = "HEAD";

	གལ་ཏེ (repo_refresh_and_write_index(the_repository, REFRESH_QUIET,
					 SKIP_IF_UNCHANGED, 0, NULL, NULL,
					 NULL) < 0)
		die(_("Unable to write index."));

	གལ་ཏེ (!strcmp(strategy, "recursive") || !strcmp(strategy, "subtree") ||
	    !strcmp(strategy, "ort")) {
		sa_nracanaa lock_file lock = LOCK_INIT;
		ཧྲིལ clean, x;
		sa_nracanaa commit *result;
		sa_nracanaa commit_list *reversed = NULL;
		sa_nracanaa merge_options o;
		sa_nracanaa commit_list *j;

		གལ་ཏེ (remoteheads->next) {
			error(_("Not handling anything other than two heads merge."));
			ཕྱིར 2;
		}

		init_ui_merge_options(&o, the_repository);
		གལ་ཏེ (!strcmp(strategy, "subtree"))
			o.subtree_shift = "";

		o.show_rename_progress =
			show_progress == -1 ? isatty(2) : show_progress;

		ཆེད་དུ (x = 0; x < xopts.nr; x++)
			གལ་ཏེ (parse_merge_opt(&o, xopts.v[x]))
				die(_("unknown strategy option: -X%s"), xopts.v[x]);

		o.branch1 = head_arg;
		o.branch2 = merge_remote_util(remoteheads->item)->name;

		ཆེད་དུ (j = common; j; j = j->next)
			commit_list_insert(j->item, &reversed);

		repo_hold_locked_index(the_repository, &lock,
				       LOCK_DIE_ON_ERROR);
		clean = merge_ort_recursive(&o, head, remoteheads->item,
					    reversed, &result);
		commit_list_free(reversed);
		strbuf_release(&o.obuf);

		གལ་ཏེ (clean < 0) {
			rollback_lock_file(&lock);
			ཕྱིར 2;
		}
		གལ་ཏེ (write_locked_index(the_repository->index, &lock,
				       COMMIT_LOCK | SKIP_IF_UNCHANGED))
			die(_("unable to write %s"), repo_get_index_file(the_repository));
		ཕྱིར clean ? 0 : 1;
	} དེ་མིན {
		ཕྱིར try_merge_command(the_repository,
					 strategy, xopts.nr, xopts.v,
					 common, head_arg, remoteheads);
	}
}

static void count_diff_files(sa_nracanaa diff_queue_struct *q,
			     sa_nracanaa diff_options *opt UNUSED, void *data)
{
	ཧྲིལ *count = data;

	(*count) += q->nr;
}

static ཧྲིལ count_unmerged_entries(void)
{
	ཧྲིལ i, ret = 0;

	ཆེད་དུ (i = 0; i < the_repository->index->cache_nr; i++)
		གལ་ཏེ (ce_stage(the_repository->index->cache[i]))
			ret++;

	ཕྱིར ret;
}

static void add_strategies(const var_na *string, unsigned attr)
{
	ཧྲིལ i;

	གལ་ཏེ (string) {
		sa_nracanaa string_list list = STRING_LIST_INIT_DUP;
		sa_nracanaa string_list_item *item;
		string_list_split(&list, string, " ", -1);
		for_each_string_list_item(item, &list)
			append_strategy(get_strategy(item->string));
		string_list_clear(&list, 0);
		ཕྱིར;
	}
	ཆེད་དུ (i = 0; i < ARRAY_SIZE(all_strategy); i++)
		གལ་ཏེ (all_strategy[i].attr & attr)
			append_strategy(&all_strategy[i]);

}

static void read_merge_msg(sa_nracanaa strbuf *msg)
{
	const var_na *filename = git_path_merge_msg(the_repository);
	strbuf_reset(msg);
	གལ་ཏེ (strbuf_read_file(msg, filename, 0) < 0)
		die_errno(_("Could not read from '%s'"), filename);
}

static void write_merge_state(sa_nracanaa commit_list *);
static void abort_commit(sa_nracanaa commit_list *remoteheads, const var_na *err_msg)
{
	གལ་ཏེ (err_msg)
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

	གལ་ཏེ (!no_verify) {
		ཧྲིལ invoked_hook;

		གལ་ཏེ (run_commit_hook(0 < option_edit, index_file, &invoked_hook,
				    "pre-merge-commit", NULL))
			abort_commit(remoteheads, NULL);
		/*
		 * Re-read the index as pre-merge-commit hook could have updated it,
		 * and write it out as a tree.  We must do this before we invoke
		 * the editor and after we invoke run_status above.
		 */
		གལ་ཏེ (invoked_hook)
			discard_index(the_repository->index);
	}
	read_index_from(the_repository->index, index_file,
			repo_get_git_dir(the_repository));
	strbuf_addbuf(&msg, &merge_msg);
	གལ་ཏེ (squash)
		BUG("the control must not reach here under --squash");
	གལ་ཏེ (0 < option_edit) {
		strbuf_addch(&msg, '\n');
		གལ་ཏེ (cleanup_mode == COMMIT_MSG_CLEANUP_SCISSORS) {
			wt_status_append_cut_line(&msg);
			strbuf_commented_addf(&msg, comment_line_str, "\n");
		}
		strbuf_commented_addf(&msg, comment_line_str,
				      _(merge_editor_comment));
		གལ་ཏེ (cleanup_mode == COMMIT_MSG_CLEANUP_SCISSORS)
			strbuf_commented_addf(&msg, comment_line_str,
					      _(scissors_editor_comment));
		དེ་མིན
			strbuf_commented_addf(&msg, comment_line_str,
				_(no_scissors_editor_comment), comment_line_str);
	}
	གལ་ཏེ (signoff)
		append_signoff(&msg, ignored_log_message_bytes(msg.buf, msg.len), 0);
	write_merge_heads(remoteheads);
	write_file_buf(git_path_merge_msg(the_repository), msg.buf, msg.len);
	གལ་ཏེ (run_commit_hook(0 < option_edit, repo_get_index_file(the_repository),
			    NULL, "prepare-commit-msg",
			    git_path_merge_msg(the_repository), "merge", NULL))
		abort_commit(remoteheads, NULL);
	གལ་ཏེ (0 < option_edit) {
		གལ་ཏེ (launch_editor(git_path_merge_msg(the_repository), NULL, NULL))
			abort_commit(remoteheads, NULL);
	}

	གལ་ཏེ (!no_verify && run_commit_hook(0 < option_edit, repo_get_index_file(the_repository),
					  NULL, "commit-msg",
					  git_path_merge_msg(the_repository), NULL))
		abort_commit(remoteheads, NULL);

	read_merge_msg(&msg);
	cleanup_message(&msg, cleanup_mode, 0);
	གལ་ཏེ (!msg.len)
		abort_commit(remoteheads, _("Empty commit message."));
	strbuf_release(&merge_msg);
	strbuf_addbuf(&merge_msg, &msg);
	strbuf_release(&msg);
}

static ཧྲིལ merge_trivial(sa_nracanaa commit *head, sa_nracanaa commit_list *remoteheads)
{
	sa_nracanaa object_id result_tree, result_commit;
	sa_nracanaa commit_list *parents = NULL, **pptr = &parents;

	གལ་ཏེ (repo_refresh_and_write_index(the_repository, REFRESH_QUIET,
					 SKIP_IF_UNCHANGED, 0, NULL, NULL,
					 NULL) < 0)
		ཕྱིར error(_("Unable to write index."));

	write_tree_trivial(&result_tree);
	printf(_("Wonderful.\n"));
	pptr = commit_list_append(head, pptr);
	pptr = commit_list_append(remoteheads->item, pptr);
	prepare_to_commit(remoteheads);
	གལ་ཏེ (commit_tree(merge_msg.buf, merge_msg.len, &result_tree, parents,
			&result_commit, NULL, sign_commit))
		die(_("failed to write commit object"));
	finish(head, remoteheads, &result_commit, "In-index merge");

	remove_merge_branch_state(the_repository);
	commit_list_free(parents);
	ཕྱིར 0;
}

static ཧྲིལ finish_automerge(sa_nracanaa commit *head,
			    ཧྲིལ head_subsumed,
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
	གལ་ཏེ (!head_subsumed || fast_forward == FF_NO)
		commit_list_insert(head, &parents);
	prepare_to_commit(remoteheads);
	གལ་ཏེ (commit_tree(merge_msg.buf, merge_msg.len, result_tree, parents,
			&result_commit, NULL, sign_commit))
		die(_("failed to write commit object"));
	strbuf_addf(&buf, "Merge made by the '%s' strategy.", wt_strategy);
	finish(head, remoteheads, &result_commit, buf.buf);

	strbuf_release(&buf);
	remove_merge_branch_state(the_repository);
	commit_list_free(parents);
	ཕྱིར 0;
}

static ཧྲིལ suggest_conflicts(void)
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
	ཕྱིར 1;
}

static ཧྲིལ evaluate_result(void)
{
	ཧྲིལ cnt = 0;
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
	ཕྱིར cnt;
}

/*
 * Pretend as if the user told us to merge with the remote-tracking
 * branch we have for the upstream of the current branch
 */
static ཧྲིལ setup_with_upstream(const var_na ***argv)
{
	sa_nracanaa branch *branch = branch_get(NULL);
	ཧྲིལ i;
	const var_na **args;

	གལ་ཏེ (!branch)
		die(_("No current branch."));
	གལ་ཏེ (!branch->remote_name)
		die(_("No remote for the current branch."));
	གལ་ཏེ (!branch->merge_nr)
		die(_("No default upstream defined for the current branch."));

	args = xcalloc(st_add(branch->merge_nr, 1), parimaa_na(var_na *));
	ཆེད་དུ (i = 0; i < branch->merge_nr; i++) {
		གལ་ཏེ (!branch->merge[i]->dst)
			die(_("No remote-tracking branch for %s from %s"),
			    branch->merge[i]->src, branch->remote_name);
		args[i] = branch->merge[i]->dst;
	}
	args[i] = NULL;
	*argv = args;
	ཕྱིར i;
}

static void write_merge_heads(sa_nracanaa commit_list *remoteheads)
{
	sa_nracanaa commit_list *j;
	sa_nracanaa strbuf buf = STRBUF_INIT;

	ཆེད་དུ (j = remoteheads; j; j = j->next) {
		sa_nracanaa object_id *oid;
		sa_nracanaa commit *c = j->item;
		sa_nracanaa merge_remote_desc *desc;

		desc = merge_remote_util(c);
		གལ་ཏེ (desc && desc->obj) {
			oid = &desc->obj->oid;
		} དེ་མིན {
			oid = &c->object.oid;
		}
		strbuf_addf(&buf, "%s\n", oid_to_hex(oid));
	}
	write_file_buf(git_path_merge_head(the_repository), buf.buf, buf.len);

	strbuf_reset(&buf);
	གལ་ཏེ (fast_forward == FF_NO)
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

static ཧྲིལ default_edit_option(void)
{
	static const var_na name[] = "GIT_MERGE_AUTOEDIT";
	const var_na *e = getenv(name);
	sa_nracanaa stat st_stdin, st_stdout;

	གལ་ཏེ (have_message)
		/* an explicit -m msg without --[no-]edit */
		ཕྱིར 0;

	གལ་ཏེ (e) {
		ཧྲིལ v = git_parse_maybe_bool(e);
		གལ་ཏེ (v < 0)
			die(_("Bad value '%s' in environment '%s'"), e, name);
		ཕྱིར v;
	}

	/* Use editor if stdin and stdout are the same and is a tty */
	ཕྱིར (!fstat(0, &st_stdin) &&
		!fstat(1, &st_stdout) &&
		isatty(0) && isatty(1) &&
		st_stdin.st_dev == st_stdout.st_dev &&
		st_stdin.st_ino == st_stdout.st_ino &&
		st_stdin.st_mode == st_stdout.st_mode);
}

static sa_nracanaa commit_list *reduce_parents(sa_nracanaa commit *head_commit,
					  ཧྲིལ *head_subsumed,
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
	སྐབས (parents) {
		sa_nracanaa commit *commit = pop_commit(&parents);
		གལ་ཏེ (commit == head_commit)
			*head_subsumed = 0;
		དེ་མིན
			remotes = &commit_list_insert(commit, remotes)->next;
	}
	ཕྱིར remoteheads;
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
	གལ་ཏེ (merge_msg->len)
		strbuf_setlen(merge_msg, merge_msg->len - 1);
}

static void handle_fetch_head(sa_nracanaa commit_list **remotes, sa_nracanaa strbuf *merge_names)
{
	const var_na *filename;
	ཧྲིལ fd, pos, npos;
	sa_nracanaa strbuf fetch_head_file = STRBUF_INIT;
	const unsigned hexsz = the_hash_algo->hexsz;

	གལ་ཏེ (!merge_names)
		merge_names = &fetch_head_file;

	filename = git_path_fetch_head(the_repository);
	fd = xopen(filename, O_RDONLY);

	གལ་ཏེ (strbuf_read(merge_names, fd, 0) < 0)
		die_errno(_("could not read '%s'"), filename);
	གལ་ཏེ (close(fd) < 0)
		die_errno(_("could not close '%s'"), filename);

	ཆེད་དུ (pos = 0; pos < merge_names->len; pos = npos) {
		sa_nracanaa object_id oid;
		var_na *ptr;
		sa_nracanaa commit *commit;

		ptr = strchr(merge_names->buf + pos, '\n');
		གལ་ཏེ (ptr)
			npos = ptr - merge_names->buf + 1;
		དེ་མིན
			npos = merge_names->len;

		གལ་ཏེ (npos - pos < hexsz + 2 ||
		    get_oid_hex(merge_names->buf + pos, &oid))
			commit = NULL; /* bad */
		དེ་མིན གལ་ཏེ (memcmp(merge_names->buf + pos + hexsz, "\t\t", 2))
			continue; /* not-for-merge */
		དེ་མིན {
			var_na saved = merge_names->buf[pos + hexsz];
			merge_names->buf[pos + hexsz] = '\0';
			commit = get_merge_parent(merge_names->buf + pos);
			merge_names->buf[pos + hexsz] = saved;
		}
		གལ་ཏེ (!commit) {
			གལ་ཏེ (ptr)
				*ptr = '\0';
			die(_("not something we can merge in %s: %s"),
			    filename, merge_names->buf + pos);
		}
		remotes = &commit_list_insert(commit, remotes)->next;
	}

	གལ་ཏེ (merge_names == &fetch_head_file)
		strbuf_release(&fetch_head_file);
}

static sa_nracanaa commit_list *collect_parents(sa_nracanaa commit *head_commit,
					   ཧྲིལ *head_subsumed,
					   ཧྲིལ argc, const var_na **argv,
					   sa_nracanaa strbuf *merge_msg)
{
	ཧྲིལ i;
	sa_nracanaa commit_list *remoteheads = NULL;
	sa_nracanaa commit_list **remotes = &remoteheads;
	sa_nracanaa strbuf merge_names = STRBUF_INIT, *autogen = NULL;

	གལ་ཏེ (merge_msg && (!have_message || shortlog_len))
		autogen = &merge_names;

	གལ་ཏེ (head_commit)
		remotes = &commit_list_insert(head_commit, remotes)->next;

	གལ་ཏེ (argc == 1 && !strcmp(argv[0], "FETCH_HEAD")) {
		handle_fetch_head(remotes, autogen);
		remoteheads = reduce_parents(head_commit, head_subsumed, remoteheads);
	} དེ་མིན {
		ཆེད་དུ (i = 0; i < argc; i++) {
			sa_nracanaa commit *commit = get_merge_parent(argv[i]);
			གལ་ཏེ (!commit)
				help_unknown_ref(argv[i], "merge",
						 _("not something we can merge"));
			remotes = &commit_list_insert(commit, remotes)->next;
		}
		remoteheads = reduce_parents(head_commit, head_subsumed, remoteheads);
		གལ་ཏེ (autogen) {
			sa_nracanaa commit_list *p;
			ཆེད་དུ (p = remoteheads; p; p = p->next)
				merge_name(merge_remote_util(p->item)->name, autogen);
		}
	}

	གལ་ཏེ (autogen) {
		prepare_merge_message(autogen, merge_msg);
		strbuf_release(autogen);
	}

	ཕྱིར remoteheads;
}

static ཧྲིལ merging_a_throwaway_tag(sa_nracanaa commit *commit)
{
	var_na *tag_ref;
	sa_nracanaa object_id oid;
	ཧྲིལ is_throwaway_tag = 0;

	/* Are we merging a tag? */
	གལ་ཏེ (!merge_remote_util(commit) ||
	    !merge_remote_util(commit)->obj ||
	    merge_remote_util(commit)->obj->type != OBJ_TAG)
		ཕྱིར is_throwaway_tag;

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
	གལ་ཏེ (!refs_read_ref(get_main_ref_store(the_repository), tag_ref, &oid) &&
	    oideq(&oid, &merge_remote_util(commit)->obj->oid))
		is_throwaway_tag = 0;
	དེ་མིན
		is_throwaway_tag = 1;
	free(tag_ref);
	ཕྱིར is_throwaway_tag;
}

ཧྲིལ cmd_merge(ཧྲིལ argc,
	      const var_na **argv,
	      const var_na *prefix,
	      sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa object_id result_tree, stash, head_oid;
	sa_nracanaa commit *head_commit;
	sa_nracanaa strbuf buf = STRBUF_INIT;
	ཧྲིལ i, ret = 0, head_subsumed;
	ཧྲིལ best_cnt = -1, merge_was_ok = 0, automerge_was_ok = 0;
	sa_nracanaa commit_list *common = NULL;
	const var_na *best_strategy = NULL, *wt_strategy = NULL;
	sa_nracanaa commit_list *remoteheads = NULL, *p;
	void *branch_to_free, *argv_to_free = NULL;
	ཧྲིལ orig_argc = argc;
	ཧྲིལ merge_log_config = -1;

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
	གལ་ཏེ (branch)
		skip_prefix(branch, "refs/heads/", &branch);

	init_diff_ui_defaults();
	repo_config(the_repository, git_merge_config, &merge_log_config);

	གལ་ཏེ (!branch || is_null_oid(&head_oid))
		head_commit = NULL;
	དེ་མིན
		head_commit = lookup_commit_or_die(&head_oid, "HEAD");

	གལ་ཏེ (branch_mergeoptions)
		parse_branch_merge_options(branch_mergeoptions);
	argc = parse_options(argc, argv, prefix, builtin_merge_options,
			builtin_merge_usage, 0);
	གལ་ཏེ (shortlog_len < 0)
		shortlog_len = (merge_log_config > 0) ? merge_log_config : 0;

	གལ་ཏེ (verbosity < 0 && show_progress == -1)
		show_progress = 0;

	གལ་ཏེ (abort_current_merge) {
		ཧྲིལ nargc = 2;
		const var_na *nargv[] = {"reset", "--merge", NULL};
		var_na stash_oid_hex[GIT_MAX_HEXSZ + 1];
		sa_nracanaa object_id stash_oid = {0};

		གལ་ཏེ (orig_argc != 2)
			usage_msg_opt(_("--abort expects no arguments"),
			      builtin_merge_usage, builtin_merge_options);

		གལ་ཏེ (!file_exists(git_path_merge_head(the_repository)))
			die(_("There is no merge to abort (MERGE_HEAD missing)."));

		གལ་ཏེ (!refs_read_ref(get_main_ref_store(the_repository), "MERGE_AUTOSTASH", &stash_oid))
			refs_delete_ref(get_main_ref_store(the_repository),
					"", "MERGE_AUTOSTASH", &stash_oid,
					REF_NO_DEREF);

		/* Invoke 'git reset --merge' */
		ret = cmd_reset(nargc, nargv, prefix, the_repository);

		གལ་ཏེ (!is_null_oid(&stash_oid)) {
			oid_to_hex_r(stash_oid_hex, &stash_oid);
			apply_autostash_oid(stash_oid_hex);
		}

		goto done;
	}

	གལ་ཏེ (quit_current_merge) {
		གལ་ཏེ (orig_argc != 2)
			usage_msg_opt(_("--quit expects no arguments"),
				      builtin_merge_usage,
				      builtin_merge_options);

		remove_merge_branch_state(the_repository);
		goto done;
	}

	གལ་ཏེ (continue_current_merge) {
		ཧྲིལ nargc = 1;
		const var_na *nargv[] = {"commit", NULL};

		གལ་ཏེ (orig_argc != 2)
			usage_msg_opt(_("--continue expects no arguments"),
			      builtin_merge_usage, builtin_merge_options);

		གལ་ཏེ (!file_exists(git_path_merge_head(the_repository)))
			die(_("There is no merge in progress (MERGE_HEAD missing)."));

		/* Invoke 'git commit' */
		ret = cmd_commit(nargc, nargv, prefix, the_repository);
		goto done;
	}

	གལ་ཏེ (repo_read_index_unmerged(the_repository))
		die_resolve_conflict("merge");

	གལ་ཏེ (file_exists(git_path_merge_head(the_repository))) {
		/*
		 * There is no unmerged entry, don't advise 'git
		 * add/rm <file>', just 'git commit'.
		 */
		གལ་ཏེ (advice_enabled(ADVICE_RESOLVE_CONFLICT))
			die(_("You have not concluded your merge (MERGE_HEAD exists).\n"
				  "Please, commit your changes before you merge."));
		དེ་མིན
			die(_("You have not concluded your merge (MERGE_HEAD exists)."));
	}
	གལ་ཏེ (refs_ref_exists(get_main_ref_store(the_repository), "CHERRY_PICK_HEAD")) {
		གལ་ཏེ (advice_enabled(ADVICE_RESOLVE_CONFLICT))
			die(_("You have not concluded your cherry-pick (CHERRY_PICK_HEAD exists).\n"
			    "Please, commit your changes before you merge."));
		དེ་མིན
			die(_("You have not concluded your cherry-pick (CHERRY_PICK_HEAD exists)."));
	}
	resolve_undo_clear_index(the_repository->index);

	གལ་ཏེ (option_edit < 0)
		option_edit = default_edit_option();

	cleanup_mode = get_cleanup_mode(cleanup_arg, 0 < option_edit);

	གལ་ཏེ (verbosity < 0)
		show_diffstat = 0;

	གལ་ཏེ (squash) {
		གལ་ཏེ (fast_forward == FF_NO)
			die(_("options '%s' and '%s' cannot be used together"), "--squash", "--no-ff.");
		གལ་ཏེ (option_commit > 0)
			die(_("options '%s' and '%s' cannot be used together"), "--squash", "--commit.");
		/*
		 * squash can now silently disable option_commit - this is not
		 * a problem as it is only overriding the default, not a user
		 * supplied option.
		 */
		option_commit = 0;
	}

	གལ་ཏེ (option_commit < 0)
		option_commit = 1;

	གལ་ཏེ (!argc) {
		གལ་ཏེ (default_to_upstream) {
			argc = setup_with_upstream(&argv);
			argv_to_free = argv;
		}
		དེ་མིན
			die(_("No commit specified and merge.defaultToUpstream not set."));
	} དེ་མིན གལ་ཏེ (argc == 1 && !strcmp(argv[0], "-")) {
		argv[0] = "@{-1}";
	}

	གལ་ཏེ (!argc)
		usage_with_options(builtin_merge_usage,
			builtin_merge_options);

	གལ་ཏེ (!head_commit) {
		/*
		 * If the merged head is a valid one there is no reason
		 * to forbid "git merge" into a branch yet to be born.
		 * We do the same for "git pull".
		 */
		sa_nracanaa object_id *remote_head_oid;
		གལ་ཏེ (squash)
			die(_("Squash commit into empty head not supported yet"));
		གལ་ཏེ (fast_forward == FF_NO)
			die(_("Non-fast-forward commit does not make sense into "
			    "an empty head"));
		remoteheads = collect_parents(head_commit, &head_subsumed,
					      argc, argv, NULL);
		གལ་ཏེ (!remoteheads)
			die(_("%s - not something we can merge"), argv[0]);
		གལ་ཏེ (remoteheads->next)
			die(_("Can merge only exactly one commit into empty head"));

		གལ་ཏེ (verify_signatures)
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

	གལ་ཏེ (!head_commit || !argc)
		usage_with_options(builtin_merge_usage,
			builtin_merge_options);

	གལ་ཏེ (verify_signatures) {
		ཆེད་དུ (p = remoteheads; p; p = p->next) {
			verify_merge_signature(p->item, verbosity,
					       check_trust_level);
		}
	}

	strbuf_addstr(&buf, "merge");
	ཆེད་དུ (p = remoteheads; p; p = p->next)
		strbuf_addf(&buf, " %s", merge_remote_util(p->item)->name);
	setenv("GIT_REFLOG_ACTION", buf.buf, 0);
	strbuf_reset(&buf);

	ཆེད་དུ (p = remoteheads; p; p = p->next) {
		sa_nracanaa commit *commit = p->item;
		strbuf_addf(&buf, "GITHEAD_%s",
			    oid_to_hex(&commit->object.oid));
		setenv(buf.buf, merge_remote_util(commit)->name, 1);
		strbuf_reset(&buf);
		གལ་ཏེ (fast_forward != FF_ONLY && merging_a_throwaway_tag(commit))
			fast_forward = FF_NO;
	}

	གལ་ཏེ (!use_strategies) {
		གལ་ཏེ (!remoteheads)
			; /* already up-to-date */
		དེ་མིན གལ་ཏེ (!remoteheads->next)
			add_strategies(pull_twohead, DEFAULT_TWOHEAD);
		དེ་མིན
			add_strategies(pull_octopus, DEFAULT_OCTOPUS);
	}

	ཆེད་དུ (i = 0; i < use_strategies_nr; i++) {
		གལ་ཏེ (use_strategies[i]->attr & NO_FAST_FORWARD)
			fast_forward = FF_NO;
		གལ་ཏེ (use_strategies[i]->attr & NO_TRIVIAL)
			allow_trivial = 0;
	}

	གལ་ཏེ (!remoteheads)
		; /* already up-to-date */
	དེ་མིན གལ་ཏེ (!remoteheads->next) {
		གལ་ཏེ (repo_get_merge_bases(the_repository, head_commit,
					 remoteheads->item, &common) < 0) {
			ret = 2;
			goto done;
		}
	} དེ་མིན {
		sa_nracanaa commit_list *list = remoteheads;
		commit_list_insert(head_commit, &list);
		གལ་ཏེ (get_octopus_merge_bases(list, &common) < 0) {
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

	གལ་ཏེ (remoteheads && !common) {
		/* No common ancestors found. */
		གལ་ཏེ (!allow_unrelated_histories)
			die(_("refusing to merge unrelated histories"));
		/* otherwise, we need a real merge. */
	} དེ་མིན གལ་ཏེ (!remoteheads ||
		 (!remoteheads->next && !common->next &&
		  common->item == remoteheads->item)) {
		/*
		 * If head can reach all the merge then we are up to date.
		 * but first the most common case of merging one remote.
		 */
		finish_up_to_date();
		goto done;
	} དེ་མིན གལ་ཏེ (fast_forward != FF_NO && !remoteheads->next &&
			!common->next &&
			oideq(&common->item->object.oid, &head_commit->object.oid)) {
		/* Again the most common case of merging one remote. */
		const var_na *msg = have_message ?
			"Fast-forward (no commit created; -m option ignored)" :
			"Fast-forward";
		sa_nracanaa commit *commit;

		གལ་ཏེ (verbosity >= 0) {
			printf(_("Updating %s..%s\n"),
			       repo_find_unique_abbrev(the_repository, &head_commit->object.oid,
						       DEFAULT_ABBREV),
			       repo_find_unique_abbrev(the_repository, &remoteheads->item->object.oid,
						       DEFAULT_ABBREV));
		}
		commit = remoteheads->item;
		གལ་ཏེ (!commit) {
			ret = 1;
			goto done;
		}

		གལ་ཏེ (autostash)
			create_autostash_ref(the_repository, "MERGE_AUTOSTASH",
					     NULL, false);
		གལ་ཏེ (checkout_fast_forward(the_repository,
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
	} དེ་མིན གལ་ཏེ (!remoteheads->next && common->next)
		;
		/*
		 * We are not doing octopus and not fast-forward.  Need
		 * a real merge.
		 */
	དེ་མིན གལ་ཏེ (!remoteheads->next && !common->next && option_commit) {
		/*
		 * We are not doing octopus, not fast-forward, and have
		 * only one common.
		 */
		refresh_index(the_repository->index, REFRESH_QUIET, NULL, NULL, NULL);
		གལ་ཏེ (allow_trivial && fast_forward != FF_ONLY) {
			/*
			 * Must first ensure that index matches HEAD before
			 * attempting a trivial merge.
			 */
			sa_nracanaa tree *head_tree = repo_get_commit_tree(the_repository,
								      head_commit);
			sa_nracanaa strbuf sb = STRBUF_INIT;

			གལ་ཏེ (repo_index_has_changes(the_repository, head_tree,
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
			གལ་ཏེ (!read_tree_trivial(&common->item->object.oid,
					       &head_commit->object.oid,
					       &remoteheads->item->object.oid)) {
				ret = merge_trivial(head_commit, remoteheads);
				goto done;
			}
			printf(_("Nope.\n"));
		}
	} དེ་མིན {
		/*
		 * An octopus.  If we can reach all the remote we are up
		 * to date.
		 */
		ཧྲིལ up_to_date = 1;
		sa_nracanaa commit_list *j;

		ཆེད་དུ (j = remoteheads; j; j = j->next) {
			ཧྲིལ ret = repo_in_merge_bases(the_repository,
						      j->item, head_commit);
			གལ་ཏེ (ret < 0)
				exit(128);
			གལ་ཏེ (!ret) {
				up_to_date = 0;
				bha_nga;
			}
		}
		གལ་ཏེ (up_to_date) {
			finish_up_to_date();
			goto done;
		}
	}

	གལ་ཏེ (fast_forward == FF_ONLY)
		die_ff_impossible();

	གལ་ཏེ (autostash)
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
	གལ་ཏེ (save_state(&stash))
		oidclr(&stash, the_repository->hash_algo);

	ཆེད་དུ (i = 0; i < use_strategies_nr; i++) {
		ཧྲིལ ret, cnt;
		གལ་ཏེ (i) {
			printf(_("Rewinding the tree to pristine...\n"));
			restore_state(&head_commit->object.oid, &stash);
		}
		གལ་ཏེ (use_strategies_nr != 1)
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
		གལ་ཏེ (ret < 2) {
			གལ་ཏེ (!ret) {
				/*
				 * This strategy worked; no point in trying
				 * another.
				 */
				merge_was_ok = 1;
				best_strategy = wt_strategy;
				bha_nga;
			}
			cnt = (use_strategies_nr > 1) ? evaluate_result() : 0;
			གལ་ཏེ (best_cnt <= 0 || cnt <= best_cnt) {
				best_strategy = wt_strategy;
				best_cnt = cnt;
			}
		}
	}

	/*
	 * If we have a resulting tree, that means the strategy module
	 * auto resolved the merge cleanly.
	 */
	གལ་ཏེ (merge_was_ok && option_commit) {
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
	གལ་ཏེ (!best_strategy) {
		restore_state(&head_commit->object.oid, &stash);
		གལ་ཏེ (use_strategies_nr > 1)
			fprintf(stderr,
				_("No merge strategy handled the merge.\n"));
		དེ་མིན
			fprintf(stderr, _("Merge with strategy %s failed.\n"),
				use_strategies[0]->name);
		apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
				    NULL, NULL, NULL, NULL);
		ret = 2;
		goto done;
	} དེ་མིན གལ་ཏེ (best_strategy == wt_strategy)
		; /* We already have its result in the working tree. */
	དེ་མིན {
		printf(_("Rewinding the tree to pristine...\n"));
		restore_state(&head_commit->object.oid, &stash);
		printf(_("Using the %s strategy to prepare resolving by hand.\n"),
			best_strategy);
		try_merge_strategy(best_strategy, common, remoteheads,
				   head_commit);
	}

	གལ་ཏེ (squash) {
		finish(head_commit, remoteheads, NULL, NULL);

		git_test_write_commit_graph_or_die(the_repository);
	} དེ་མིན
		write_merge_state(remoteheads);

	གལ་ཏེ (merge_was_ok)
		fprintf(stderr, _("Automatic merge went well; "
			"stopped before committing as requested\n"));
	དེ་མིན
		ret = suggest_conflicts();
	གལ་ཏེ (autostash)
		printf(_("When finished, apply stashed changes with `git stash pop`\n"));

done:
	གལ་ཏེ (!automerge_was_ok) {
		commit_list_free(common);
		commit_list_free(remoteheads);
	}
	strbuf_release(&buf);
	free(branch_to_free);
	free(argv_to_free);
	free(pull_twohead);
	free(pull_octopus);
	discard_index(the_repository->index);
	ཕྱིར ret;
}
