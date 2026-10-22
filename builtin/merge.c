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

struct strategy {
	const char *name;
	unsigned attr;
};

static const char * const builtin_merge_usage[] = {
	N_("git merge [<options>] [<commit>...]"),
	"git merge --abort",
	"git merge --continue",
	NULL
};

#define MERGE_SHOW_DIFFSTAT 1
#define MERGE_SHOW_COMPACTSUMMARY 2

static ціле show_diffstat = MERGE_SHOW_DIFFSTAT, shortlog_len = -1, squash;
static ціле option_commit = -1;
static ціле option_edit = -1;
static ціле allow_trivial = 1, have_message, verify_signatures;
static ціле check_trust_level = 1;
static ціле overwrite_ignore = 1;
static struct strbuf merge_msg = STRBUF_INIT;
static struct strategy **use_strategies;
static size_t use_strategies_nr, use_strategies_alloc;
static struct strvec xopts = STRVEC_INIT;
static const char *branch;
static char *branch_mergeoptions;
static ціле verbosity;
static ціле allow_rerere_auto;
static ціле abort_current_merge;
static ціле quit_current_merge;
static ціле continue_current_merge;
static ціле allow_unrelated_histories;
static ціле show_progress = -1;
static ціле default_to_upstream = 1;
static ціле signoff;
static const char *sign_commit;
static ціле autostash;
static ціле no_verify;
static char *into_name;

static struct strategy all_strategy[] = {
	{ "recursive",  NO_TRIVIAL },
	{ "octopus",    DEFAULT_OCTOPUS },
	{ "ort",        DEFAULT_TWOHEAD | NO_TRIVIAL },
	{ "resolve",    0 },
	{ "ours",       NO_FAST_FORWARD | NO_TRIVIAL },
	{ "subtree",    NO_FAST_FORWARD | NO_TRIVIAL },
};

static char *pull_twohead, *pull_octopus;

enum ff_type {
	FF_NO,
	FF_ALLOW,
	FF_ONLY
};

static enum ff_type fast_forward = FF_ALLOW;

static char *cleanup_arg;
static enum commit_msg_cleanup_mode cleanup_mode;

static ціле option_parse_message(const struct option *opt,
				const char *arg, ціле unset)
{
	struct strbuf *buf = opt->value;

	якщо (unset)
		strbuf_setlen(buf, 0);
	інакше якщо (arg) {
		strbuf_addf(buf, "%s%s", buf->len ? "\n\n" : "", arg);
		have_message = 1;
	} інакше
		повернути error(_("switch `m' requires a value"));
	повернути 0;
}

static enum parse_opt_result option_read_message(struct parse_opt_ctx_t *ctx,
						 const struct option *opt,
						 const char *arg_not_used,
						 ціле unset)
{
	struct strbuf *buf = opt->value;
	const char *arg;

	BUG_ON_OPT_ARG(arg_not_used);
	якщо (unset)
		BUG("-F cannot be negated");

	якщо (ctx->opt) {
		arg = ctx->opt;
		ctx->opt = NULL;
	} інакше якщо (ctx->argc > 1) {
		ctx->argc--;
		arg = *++ctx->argv;
	} інакше
		повернути error(_("option `%s' requires a value"), opt->long_name);

	якщо (buf->len)
		strbuf_addch(buf, '\n');
	якщо (ctx->prefix && !is_absolute_path(arg))
		arg = prefix_filename(ctx->prefix, arg);
	якщо (strbuf_read_file(buf, arg, 0) < 0)
		повернути error(_("could not read file '%s'"), arg);
	have_message = 1;

	повернути 0;
}

static struct strategy *get_strategy(const char *name)
{
	ціле i;
	struct strategy *ret;
	static struct cmdnames main_cmds = {0}, other_cmds = {0};
	static ціле loaded;
	char *default_strategy = NULL;

	якщо (!name)
		повернути NULL;

	якщо (default_strategy &&
	    !strcmp(default_strategy, "ort") &&
	    !strcmp(name, "recursive")) {
		name = "ort";
	}

	для (i = 0; i < ARRAY_SIZE(all_strategy); i++)
		якщо (!strcmp(name, all_strategy[i].name))
			повернути &all_strategy[i];

	якщо (!loaded) {
		struct cmdnames not_strategies = {0};
		loaded = 1;

		load_command_list("git-merge-", &main_cmds, &other_cmds);
		для (i = 0; i < main_cmds.cnt; i++) {
			ціле j, found = 0;
			struct cmdname *ent = main_cmds.names[i];
			для (j = 0; !found && j < ARRAY_SIZE(all_strategy); j++)
				якщо (!xstrncmpz(all_strategy[j].name, ent->name, ent->len))
					found = 1;
			якщо (!found)
				add_cmdname(&not_strategies, ent->name, ent->len);
		}
		exclude_cmds(&main_cmds, &not_strategies);

		cmdnames_release(&not_strategies);
	}
	якщо (!is_in_cmdlist(&main_cmds, name) && !is_in_cmdlist(&other_cmds, name)) {
		fprintf(stderr, _("Could not find merge strategy '%s'.\n"), name);
		fprintf(stderr, _("Available strategies are:"));
		для (i = 0; i < main_cmds.cnt; i++)
			fprintf(stderr, " %s", main_cmds.names[i]->name);
		fprintf(stderr, ".\n");
		якщо (other_cmds.cnt) {
			fprintf(stderr, _("Available custom strategies are:"));
			для (i = 0; i < other_cmds.cnt; i++)
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
	повернути ret;
}

static void append_strategy(struct strategy *s)
{
	ALLOC_GROW(use_strategies, use_strategies_nr + 1, use_strategies_alloc);
	use_strategies[use_strategies_nr++] = s;
}

static ціле option_parse_strategy(const struct option *opt UNUSED,
				 const char *name, ціле unset)
{
	якщо (unset)
		повернути 0;

	append_strategy(get_strategy(name));
	повернути 0;
}

static ціле option_parse_compact_summary(const struct option *opt,
					const char *name UNUSED, ціле unset)
{
	ціле *setting = opt->value;

	якщо (unset)
		*setting = 0;
	інакше
		*setting = MERGE_SHOW_COMPACTSUMMARY;
	повернути 0;
}

static struct option builtin_merge_options[] = {
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
		.precision = sizeof(shortlog_len),
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

static ціле save_state(struct object_id *stash)
{
	ціле len;
	struct child_process cp = CHILD_PROCESS_INIT;
	struct strbuf buffer = STRBUF_INIT;
	struct lock_file lock_file = LOCK_INIT;
	ціле fd;
	ціле rc = -1;

	fd = repo_hold_locked_index(the_repository, &lock_file, 0);
	refresh_index(the_repository->index, REFRESH_QUIET, NULL, NULL, NULL);
	якщо (0 <= fd)
		repo_update_index_if_able(the_repository, &lock_file);
	rollback_lock_file(&lock_file);

	strvec_pushl(&cp.args, "stash", "create", NULL);
	cp.out = -1;
	cp.git_cmd = 1;

	якщо (start_command(&cp))
		die(_("could not run stash."));
	len = strbuf_read(&buffer, cp.out, 1024);
	close(cp.out);

	якщо (finish_command(&cp) || len < 0)
		die(_("stash failed"));
	інакше якщо (!len)		/* no changes */
		goto out;
	strbuf_setlen(&buffer, buffer.len-1);
	якщо (repo_get_oid(the_repository, buffer.buf, stash))
		die(_("not a valid object: %s"), buffer.buf);
	rc = 0;
out:
	strbuf_release(&buffer);
	повернути rc;
}

static void read_empty(const struct object_id *oid)
{
	struct child_process cmd = CHILD_PROCESS_INIT;

	strvec_pushl(&cmd.args, "read-tree", "-m", "-u",
		     empty_tree_oid_hex(the_repository->hash_algo),
		     oid_to_hex(oid), NULL);
	cmd.git_cmd = 1;

	якщо (run_command(&cmd))
		die(_("read-tree failed"));
}

static void reset_hard(const struct object_id *oid)
{
	struct child_process cmd = CHILD_PROCESS_INIT;

	strvec_pushl(&cmd.args, "read-tree", "-v", "--reset", "-u",
		     oid_to_hex(oid), NULL);
	cmd.git_cmd = 1;

	якщо (run_command(&cmd))
		die(_("read-tree failed"));
}

static void restore_state(const struct object_id *head,
			  const struct object_id *stash)
{
	struct child_process cmd = CHILD_PROCESS_INIT;

	reset_hard(head);

	якщо (is_null_oid(stash))
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
	якщо (repo_read_index(the_repository) < 0)
		die(_("could not read index"));
}

/* This is called when no merge was necessary. */
static void finish_up_to_date(void)
{
	якщо (verbosity >= 0) {
		якщо (squash)
			puts(_("Already up to date. (nothing to squash)"));
		інакше
			puts(_("Already up to date."));
	}
	remove_merge_branch_state(the_repository);
}

static void squash_message(struct commit *commit, struct commit_list *remoteheads)
{
	struct rev_info rev;
	struct strbuf out = STRBUF_INIT;
	struct commit_list *j;
	struct pretty_print_context ctx = {0};

	printf(_("Squash commit -- not updating HEAD\n"));

	repo_init_revisions(the_repository, &rev, NULL);
	diff_merges_suppress(&rev);
	rev.commit_format = CMIT_FMT_MEDIUM;

	commit->object.flags |= UNINTERESTING;
	add_pending_object(&rev, &commit->object, NULL);

	для (j = remoteheads; j; j = j->next)
		add_pending_object(&rev, &j->item->object, NULL);

	setup_revisions(0, NULL, &rev, NULL);
	якщо (prepare_revision_walk(&rev))
		die(_("revision walk setup failed"));

	ctx.abbrev = rev.abbrev;
	ctx.date_mode = rev.date_mode;
	ctx.fmt = rev.commit_format;

	strbuf_addstr(&out, "Squashed commit of the following:\n");
	поки ((commit = get_revision(&rev)) != NULL) {
		strbuf_addch(&out, '\n');
		strbuf_addf(&out, "commit %s\n",
			oid_to_hex(&commit->object.oid));
		pretty_print_commit(&ctx, commit, &out);
	}
	write_file_buf(git_path_squash_msg(the_repository), out.buf, out.len);
	strbuf_release(&out);
	release_revisions(&rev);
}

static void finish(struct commit *head_commit,
		   struct commit_list *remoteheads,
		   const struct object_id *new_head, const char *msg)
{
	struct strbuf reflog_message = STRBUF_INIT;
	const struct object_id *head = &head_commit->object.oid;

	якщо (!msg)
		strbuf_addstr(&reflog_message, getenv("GIT_REFLOG_ACTION"));
	інакше {
		якщо (verbosity >= 0)
			printf("%s\n", msg);
		strbuf_addf(&reflog_message, "%s: %s",
			getenv("GIT_REFLOG_ACTION"), msg);
	}
	якщо (squash) {
		squash_message(head_commit, remoteheads);
	} інакше {
		якщо (verbosity >= 0 && !merge_msg.len)
			printf(_("No merge message -- not updating HEAD\n"));
		інакше {
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
	якщо (new_head && show_diffstat) {
		struct diff_options opts;
		repo_diff_setup(the_repository, &opts);
		init_diffstat_widths(&opts);

		switch (show_diffstat) {
		case MERGE_SHOW_DIFFSTAT: /* 1 */
			opts.output_format |=
				DIFF_FORMAT_SUMMARY | DIFF_FORMAT_DIFFSTAT;
			break;
		case MERGE_SHOW_COMPACTSUMMARY: /* 2 */
			opts.output_format |= DIFF_FORMAT_DIFFSTAT;
			opts.flags.stat_with_summary = 1;
			break;
		default:
			break;
		}
		opts.detect_rename = DIFF_DETECT_RENAME;
		diff_setup_done(&opts);
		diff_tree_oid(head, new_head, "", &opts);
		diffcore_std(&opts);
		diff_flush(&opts);
	}

	/* Run a post-merge hook */
	run_hooks_l(the_repository, "post-merge", squash ? "1" : "0", NULL);

	якщо (new_head)
		apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
				    NULL, NULL, NULL, NULL);
	strbuf_release(&reflog_message);
}

/* Get the name for the merge commit's message. */
static void merge_name(const char *remote, struct strbuf *msg)
{
	struct commit *remote_head;
	struct object_id branch_head;
	struct strbuf bname = STRBUF_INIT;
	struct merge_remote_desc *desc;
	const char *ptr;
	char *found_ref = NULL;
	ціле len, early;

	copy_branchname(the_repository, &bname, remote, 0);
	remote = bname.buf;

	oidclr(&branch_head, the_repository->hash_algo);
	remote_head = get_merge_parent(remote);
	якщо (!remote_head)
		die(_("'%s' does not point to a commit"), remote);

	якщо (repo_dwim_ref(the_repository, remote, strlen(remote), &branch_head,
			  &found_ref, 0) > 0) {
		якщо (starts_with(found_ref, "refs/heads/")) {
			strbuf_addf(msg, "%s\t\tbranch '%s' of .\n",
				    oid_to_hex(&branch_head), remote);
			goto cleanup;
		}
		якщо (starts_with(found_ref, "refs/tags/")) {
			strbuf_addf(msg, "%s\t\ttag '%s' of .\n",
				    oid_to_hex(&branch_head), remote);
			goto cleanup;
		}
		якщо (starts_with(found_ref, "refs/remotes/")) {
			strbuf_addf(msg, "%s\t\tremote-tracking branch '%s' of .\n",
				    oid_to_hex(&branch_head), remote);
			goto cleanup;
		}
	}

	/* See if remote matches <name>^^^.. or <name>~<number> */
	для (len = 0, ptr = remote + strlen(remote);
	     remote < ptr && ptr[-1] == '^';
	     ptr--)
		len++;
	якщо (len)
		early = 1;
	інакше {
		early = 0;
		ptr = strrchr(remote, '~');
		якщо (ptr) {
			ціле seen_nonzero = 0;

			len++; /* count ~ */
			поки (*++ptr && isdigit(*ptr)) {
				seen_nonzero |= (*ptr != '0');
				len++;
			}
			якщо (*ptr)
				len = 0; /* not ...~<number> */
			інакше якщо (seen_nonzero)
				early = 1;
			інакше якщо (len == 1)
				early = 1; /* "name~" is "name~1"! */
		}
	}
	якщо (len) {
		struct strbuf truname = STRBUF_INIT;
		strbuf_addf(&truname, "refs/heads/%s", remote);
		strbuf_setlen(&truname, truname.len - len);
		якщо (refs_ref_exists(get_main_ref_store(the_repository), truname.buf)) {
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
	якщо (desc && desc->obj && desc->obj->type == OBJ_TAG) {
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

static void parse_branch_merge_options(char *bmo)
{
	const char **argv;
	ціле argc;

	якщо (!bmo)
		повернути;
	argc = split_cmdline(bmo, &argv);
	якщо (argc < 0)
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

static ціле git_merge_config(const char *k, const char *v,
			    const struct config_context *ctx, void *cb)
{
	ціле status;
	const char *str;

	якщо (branch &&
	    skip_prefix(k, "branch.", &str) &&
	    skip_prefix(str, branch, &str) &&
	    !strcmp(str, ".mergeoptions")) {
		free(branch_mergeoptions);
		branch_mergeoptions = xstrdup(v);
		повернути 0;
	}

	якщо (!strcmp(k, "merge.diffstat") || !strcmp(k, "merge.stat")) {
		ціле val = git_parse_maybe_bool_text(v);
		switch (val) {
		case 0:
			show_diffstat = 0;
			break;
		case 1:
			show_diffstat = MERGE_SHOW_DIFFSTAT;
			break;
		default:
			якщо (!strcmp(v, "compact"))
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
			інакше
				/*
				 * A setting from a future?  It is not an
				 * error grave enough to fail the command.
				 * proceed using the default one.
				 */
				show_diffstat = MERGE_SHOW_DIFFSTAT;
			break;
		}
	} інакше якщо (!strcmp(k, "merge.verifysignatures")) {
		verify_signatures = git_config_bool(k, v);
	} інакше якщо (!strcmp(k, "pull.twohead")) {
		FREE_AND_NULL(pull_twohead);
		повернути git_config_string(&pull_twohead, k, v);
	} інакше якщо (!strcmp(k, "pull.octopus")) {
		FREE_AND_NULL(pull_octopus);
		повернути git_config_string(&pull_octopus, k, v);
	} інакше якщо (!strcmp(k, "commit.cleanup")) {
		повернути git_config_string(&cleanup_arg, k, v);
	} інакше якщо (!strcmp(k, "merge.ff")) {
		ціле boolval = git_parse_maybe_bool(v);
		якщо (0 <= boolval) {
			fast_forward = boolval ? FF_ALLOW : FF_NO;
		} інакше якщо (v && !strcmp(v, "only")) {
			fast_forward = FF_ONLY;
		} /* do not barf on values from future versions of git */
		повернути 0;
	} інакше якщо (!strcmp(k, "merge.defaulttoupstream")) {
		default_to_upstream = git_config_bool(k, v);
		повернути 0;
	} інакше якщо (!strcmp(k, "commit.gpgsign")) {
		sign_commit = git_config_bool(k, v) ? "" : NULL;
		повернути 0;
	} інакше якщо (!strcmp(k, "gpg.mintrustlevel")) {
		check_trust_level = 0;
	} інакше якщо (!strcmp(k, "merge.autostash")) {
		autostash = git_config_bool(k, v);
		повернути 0;
	}

	status = fmt_merge_msg_config(k, v, ctx, cb);
	якщо (status)
		повернути status;
	повернути git_diff_ui_config(k, v, ctx, cb);
}

static ціле read_tree_trivial(struct object_id *common, struct object_id *head,
			     struct object_id *one)
{
	ціле i, nr_trees = 0;
	struct tree *trees[MAX_UNPACK_TREES];
	struct tree_desc t[MAX_UNPACK_TREES];
	struct unpack_trees_options opts;

	memset(&opts, 0, sizeof(opts));
	opts.head_idx = 2;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;
	opts.update = 1;
	opts.verbose_update = 1;
	opts.trivial_merges_only = 1;
	opts.merge = 1;
	opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
	trees[nr_trees] = repo_parse_tree_indirect(the_repository, common);
	якщо (!trees[nr_trees++])
		повернути -1;
	trees[nr_trees] = repo_parse_tree_indirect(the_repository, head);
	якщо (!trees[nr_trees++])
		повернути -1;
	trees[nr_trees] = repo_parse_tree_indirect(the_repository, one);
	якщо (!trees[nr_trees++])
		повернути -1;
	opts.fn = threeway_merge;
	cache_tree_free(&the_repository->index->cache_tree);
	для (i = 0; i < nr_trees; i++) {
		repo_parse_tree(the_repository, trees[i]);
		init_tree_desc(t+i, &trees[i]->object.oid,
			       trees[i]->buffer, trees[i]->size);
	}
	якщо (unpack_trees(nr_trees, t, &opts))
		повернути -1;
	повернути 0;
}

static void write_tree_trivial(struct object_id *oid)
{
	якщо (write_index_as_tree(oid, the_repository->index,
				repo_get_index_file(the_repository),
				0, NULL))
		die(_("git write-tree failed to write a tree"));
}

static ціле try_merge_strategy(const char *strategy, struct commit_list *common,
			      struct commit_list *remoteheads,
			      struct commit *head)
{
	const char *head_arg = "HEAD";

	якщо (repo_refresh_and_write_index(the_repository, REFRESH_QUIET,
					 SKIP_IF_UNCHANGED, 0, NULL, NULL,
					 NULL) < 0)
		die(_("Unable to write index."));

	якщо (!strcmp(strategy, "recursive") || !strcmp(strategy, "subtree") ||
	    !strcmp(strategy, "ort")) {
		struct lock_file lock = LOCK_INIT;
		ціле clean, x;
		struct commit *result;
		struct commit_list *reversed = NULL;
		struct merge_options o;
		struct commit_list *j;

		якщо (remoteheads->next) {
			error(_("Not handling anything other than two heads merge."));
			повернути 2;
		}

		init_ui_merge_options(&o, the_repository);
		якщо (!strcmp(strategy, "subtree"))
			o.subtree_shift = "";

		o.show_rename_progress =
			show_progress == -1 ? isatty(2) : show_progress;

		для (x = 0; x < xopts.nr; x++)
			якщо (parse_merge_opt(&o, xopts.v[x]))
				die(_("unknown strategy option: -X%s"), xopts.v[x]);

		o.branch1 = head_arg;
		o.branch2 = merge_remote_util(remoteheads->item)->name;

		для (j = common; j; j = j->next)
			commit_list_insert(j->item, &reversed);

		repo_hold_locked_index(the_repository, &lock,
				       LOCK_DIE_ON_ERROR);
		clean = merge_ort_recursive(&o, head, remoteheads->item,
					    reversed, &result);
		commit_list_free(reversed);
		strbuf_release(&o.obuf);

		якщо (clean < 0) {
			rollback_lock_file(&lock);
			повернути 2;
		}
		якщо (write_locked_index(the_repository->index, &lock,
				       COMMIT_LOCK | SKIP_IF_UNCHANGED))
			die(_("unable to write %s"), repo_get_index_file(the_repository));
		повернути clean ? 0 : 1;
	} інакше {
		повернути try_merge_command(the_repository,
					 strategy, xopts.nr, xopts.v,
					 common, head_arg, remoteheads);
	}
}

static void count_diff_files(struct diff_queue_struct *q,
			     struct diff_options *opt UNUSED, void *data)
{
	ціле *count = data;

	(*count) += q->nr;
}

static ціле count_unmerged_entries(void)
{
	ціле i, ret = 0;

	для (i = 0; i < the_repository->index->cache_nr; i++)
		якщо (ce_stage(the_repository->index->cache[i]))
			ret++;

	повернути ret;
}

static void add_strategies(const char *string, unsigned attr)
{
	ціле i;

	якщо (string) {
		struct string_list list = STRING_LIST_INIT_DUP;
		struct string_list_item *item;
		string_list_split(&list, string, " ", -1);
		for_each_string_list_item(item, &list)
			append_strategy(get_strategy(item->string));
		string_list_clear(&list, 0);
		повернути;
	}
	для (i = 0; i < ARRAY_SIZE(all_strategy); i++)
		якщо (all_strategy[i].attr & attr)
			append_strategy(&all_strategy[i]);

}

static void read_merge_msg(struct strbuf *msg)
{
	const char *filename = git_path_merge_msg(the_repository);
	strbuf_reset(msg);
	якщо (strbuf_read_file(msg, filename, 0) < 0)
		die_errno(_("Could not read from '%s'"), filename);
}

static void write_merge_state(struct commit_list *);
static void abort_commit(struct commit_list *remoteheads, const char *err_msg)
{
	якщо (err_msg)
		error("%s", err_msg);
	fprintf(stderr,
		_("Not committing merge; use 'git commit' to complete the merge.\n"));
	write_merge_state(remoteheads);
	exit(1);
}

static const char merge_editor_comment[] =
N_("Please enter a commit message to explain why this merge is necessary,\n"
   "especially if it merges an updated upstream into a topic branch.\n"
   "\n");

static const char scissors_editor_comment[] =
N_("An empty message aborts the commit.\n");

static const char no_scissors_editor_comment[] =
N_("Lines starting with '%s' will be ignored, and an empty message aborts\n"
   "the commit.\n");

static void write_merge_heads(struct commit_list *);
static void prepare_to_commit(struct commit_list *remoteheads)
{
	struct strbuf msg = STRBUF_INIT;
	const char *index_file = repo_get_index_file(the_repository);

	якщо (!no_verify) {
		ціле invoked_hook;

		якщо (run_commit_hook(0 < option_edit, index_file, &invoked_hook,
				    "pre-merge-commit", NULL))
			abort_commit(remoteheads, NULL);
		/*
		 * Re-read the index as pre-merge-commit hook could have updated it,
		 * and write it out as a tree.  We must do this before we invoke
		 * the editor and after we invoke run_status above.
		 */
		якщо (invoked_hook)
			discard_index(the_repository->index);
	}
	read_index_from(the_repository->index, index_file,
			repo_get_git_dir(the_repository));
	strbuf_addbuf(&msg, &merge_msg);
	якщо (squash)
		BUG("the control must not reach here under --squash");
	якщо (0 < option_edit) {
		strbuf_addch(&msg, '\n');
		якщо (cleanup_mode == COMMIT_MSG_CLEANUP_SCISSORS) {
			wt_status_append_cut_line(&msg);
			strbuf_commented_addf(&msg, comment_line_str, "\n");
		}
		strbuf_commented_addf(&msg, comment_line_str,
				      _(merge_editor_comment));
		якщо (cleanup_mode == COMMIT_MSG_CLEANUP_SCISSORS)
			strbuf_commented_addf(&msg, comment_line_str,
					      _(scissors_editor_comment));
		інакше
			strbuf_commented_addf(&msg, comment_line_str,
				_(no_scissors_editor_comment), comment_line_str);
	}
	якщо (signoff)
		append_signoff(&msg, ignored_log_message_bytes(msg.buf, msg.len), 0);
	write_merge_heads(remoteheads);
	write_file_buf(git_path_merge_msg(the_repository), msg.buf, msg.len);
	якщо (run_commit_hook(0 < option_edit, repo_get_index_file(the_repository),
			    NULL, "prepare-commit-msg",
			    git_path_merge_msg(the_repository), "merge", NULL))
		abort_commit(remoteheads, NULL);
	якщо (0 < option_edit) {
		якщо (launch_editor(git_path_merge_msg(the_repository), NULL, NULL))
			abort_commit(remoteheads, NULL);
	}

	якщо (!no_verify && run_commit_hook(0 < option_edit, repo_get_index_file(the_repository),
					  NULL, "commit-msg",
					  git_path_merge_msg(the_repository), NULL))
		abort_commit(remoteheads, NULL);

	read_merge_msg(&msg);
	cleanup_message(&msg, cleanup_mode, 0);
	якщо (!msg.len)
		abort_commit(remoteheads, _("Empty commit message."));
	strbuf_release(&merge_msg);
	strbuf_addbuf(&merge_msg, &msg);
	strbuf_release(&msg);
}

static ціле merge_trivial(struct commit *head, struct commit_list *remoteheads)
{
	struct object_id result_tree, result_commit;
	struct commit_list *parents = NULL, **pptr = &parents;

	якщо (repo_refresh_and_write_index(the_repository, REFRESH_QUIET,
					 SKIP_IF_UNCHANGED, 0, NULL, NULL,
					 NULL) < 0)
		повернути error(_("Unable to write index."));

	write_tree_trivial(&result_tree);
	printf(_("Wonderful.\n"));
	pptr = commit_list_append(head, pptr);
	pptr = commit_list_append(remoteheads->item, pptr);
	prepare_to_commit(remoteheads);
	якщо (commit_tree(merge_msg.buf, merge_msg.len, &result_tree, parents,
			&result_commit, NULL, sign_commit))
		die(_("failed to write commit object"));
	finish(head, remoteheads, &result_commit, "In-index merge");

	remove_merge_branch_state(the_repository);
	commit_list_free(parents);
	повернути 0;
}

static ціле finish_automerge(struct commit *head,
			    ціле head_subsumed,
			    struct commit_list *common,
			    struct commit_list *remoteheads,
			    struct object_id *result_tree,
			    const char *wt_strategy)
{
	struct commit_list *parents = NULL;
	struct strbuf buf = STRBUF_INIT;
	struct object_id result_commit;

	write_tree_trivial(result_tree);
	commit_list_free(common);
	parents = remoteheads;
	якщо (!head_subsumed || fast_forward == FF_NO)
		commit_list_insert(head, &parents);
	prepare_to_commit(remoteheads);
	якщо (commit_tree(merge_msg.buf, merge_msg.len, result_tree, parents,
			&result_commit, NULL, sign_commit))
		die(_("failed to write commit object"));
	strbuf_addf(&buf, "Merge made by the '%s' strategy.", wt_strategy);
	finish(head, remoteheads, &result_commit, buf.buf);

	strbuf_release(&buf);
	remove_merge_branch_state(the_repository);
	commit_list_free(parents);
	повернути 0;
}

static ціле suggest_conflicts(void)
{
	const char *filename;
	FILE *fp;
	struct strbuf msgbuf = STRBUF_INIT;

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
	повернути 1;
}

static ціле evaluate_result(void)
{
	ціле cnt = 0;
	struct rev_info rev;

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
	повернути cnt;
}

/*
 * Pretend as if the user told us to merge with the remote-tracking
 * branch we have for the upstream of the current branch
 */
static ціле setup_with_upstream(const char ***argv)
{
	struct branch *branch = branch_get(NULL);
	ціле i;
	const char **args;

	якщо (!branch)
		die(_("No current branch."));
	якщо (!branch->remote_name)
		die(_("No remote for the current branch."));
	якщо (!branch->merge_nr)
		die(_("No default upstream defined for the current branch."));

	args = xcalloc(st_add(branch->merge_nr, 1), sizeof(char *));
	для (i = 0; i < branch->merge_nr; i++) {
		якщо (!branch->merge[i]->dst)
			die(_("No remote-tracking branch for %s from %s"),
			    branch->merge[i]->src, branch->remote_name);
		args[i] = branch->merge[i]->dst;
	}
	args[i] = NULL;
	*argv = args;
	повернути i;
}

static void write_merge_heads(struct commit_list *remoteheads)
{
	struct commit_list *j;
	struct strbuf buf = STRBUF_INIT;

	для (j = remoteheads; j; j = j->next) {
		struct object_id *oid;
		struct commit *c = j->item;
		struct merge_remote_desc *desc;

		desc = merge_remote_util(c);
		якщо (desc && desc->obj) {
			oid = &desc->obj->oid;
		} інакше {
			oid = &c->object.oid;
		}
		strbuf_addf(&buf, "%s\n", oid_to_hex(oid));
	}
	write_file_buf(git_path_merge_head(the_repository), buf.buf, buf.len);

	strbuf_reset(&buf);
	якщо (fast_forward == FF_NO)
		strbuf_addstr(&buf, "no-ff");
	write_file_buf(git_path_merge_mode(the_repository), buf.buf, buf.len);
	strbuf_release(&buf);
}

static void write_merge_state(struct commit_list *remoteheads)
{
	write_merge_heads(remoteheads);
	strbuf_addch(&merge_msg, '\n');
	write_file_buf(git_path_merge_msg(the_repository), merge_msg.buf,
		       merge_msg.len);
}

static ціле default_edit_option(void)
{
	static const char name[] = "GIT_MERGE_AUTOEDIT";
	const char *e = getenv(name);
	struct stat st_stdin, st_stdout;

	якщо (have_message)
		/* an explicit -m msg without --[no-]edit */
		повернути 0;

	якщо (e) {
		ціле v = git_parse_maybe_bool(e);
		якщо (v < 0)
			die(_("Bad value '%s' in environment '%s'"), e, name);
		повернути v;
	}

	/* Use editor if stdin and stdout are the same and is a tty */
	повернути (!fstat(0, &st_stdin) &&
		!fstat(1, &st_stdout) &&
		isatty(0) && isatty(1) &&
		st_stdin.st_dev == st_stdout.st_dev &&
		st_stdin.st_ino == st_stdout.st_ino &&
		st_stdin.st_mode == st_stdout.st_mode);
}

static struct commit_list *reduce_parents(struct commit *head_commit,
					  ціле *head_subsumed,
					  struct commit_list *remoteheads)
{
	struct commit_list *parents, **remotes;

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
	поки (parents) {
		struct commit *commit = pop_commit(&parents);
		якщо (commit == head_commit)
			*head_subsumed = 0;
		інакше
			remotes = &commit_list_insert(commit, remotes)->next;
	}
	повернути remoteheads;
}

static void prepare_merge_message(struct strbuf *merge_names, struct strbuf *merge_msg)
{
	struct fmt_merge_msg_opts opts;

	memset(&opts, 0, sizeof(opts));
	opts.add_title = !have_message;
	opts.shortlog_len = shortlog_len;
	opts.credit_people = (0 < option_edit);
	opts.into_name = into_name;

	fmt_merge_msg(merge_names, merge_msg, &opts);
	якщо (merge_msg->len)
		strbuf_setlen(merge_msg, merge_msg->len - 1);
}

static void handle_fetch_head(struct commit_list **remotes, struct strbuf *merge_names)
{
	const char *filename;
	ціле fd, pos, npos;
	struct strbuf fetch_head_file = STRBUF_INIT;
	const unsigned hexsz = the_hash_algo->hexsz;

	якщо (!merge_names)
		merge_names = &fetch_head_file;

	filename = git_path_fetch_head(the_repository);
	fd = xopen(filename, O_RDONLY);

	якщо (strbuf_read(merge_names, fd, 0) < 0)
		die_errno(_("could not read '%s'"), filename);
	якщо (close(fd) < 0)
		die_errno(_("could not close '%s'"), filename);

	для (pos = 0; pos < merge_names->len; pos = npos) {
		struct object_id oid;
		char *ptr;
		struct commit *commit;

		ptr = strchr(merge_names->buf + pos, '\n');
		якщо (ptr)
			npos = ptr - merge_names->buf + 1;
		інакше
			npos = merge_names->len;

		якщо (npos - pos < hexsz + 2 ||
		    get_oid_hex(merge_names->buf + pos, &oid))
			commit = NULL; /* bad */
		інакше якщо (memcmp(merge_names->buf + pos + hexsz, "\t\t", 2))
			continue; /* not-for-merge */
		інакше {
			char saved = merge_names->buf[pos + hexsz];
			merge_names->buf[pos + hexsz] = '\0';
			commit = get_merge_parent(merge_names->buf + pos);
			merge_names->buf[pos + hexsz] = saved;
		}
		якщо (!commit) {
			якщо (ptr)
				*ptr = '\0';
			die(_("not something we can merge in %s: %s"),
			    filename, merge_names->buf + pos);
		}
		remotes = &commit_list_insert(commit, remotes)->next;
	}

	якщо (merge_names == &fetch_head_file)
		strbuf_release(&fetch_head_file);
}

static struct commit_list *collect_parents(struct commit *head_commit,
					   ціле *head_subsumed,
					   ціле argc, const char **argv,
					   struct strbuf *merge_msg)
{
	ціле i;
	struct commit_list *remoteheads = NULL;
	struct commit_list **remotes = &remoteheads;
	struct strbuf merge_names = STRBUF_INIT, *autogen = NULL;

	якщо (merge_msg && (!have_message || shortlog_len))
		autogen = &merge_names;

	якщо (head_commit)
		remotes = &commit_list_insert(head_commit, remotes)->next;

	якщо (argc == 1 && !strcmp(argv[0], "FETCH_HEAD")) {
		handle_fetch_head(remotes, autogen);
		remoteheads = reduce_parents(head_commit, head_subsumed, remoteheads);
	} інакше {
		для (i = 0; i < argc; i++) {
			struct commit *commit = get_merge_parent(argv[i]);
			якщо (!commit)
				help_unknown_ref(argv[i], "merge",
						 _("not something we can merge"));
			remotes = &commit_list_insert(commit, remotes)->next;
		}
		remoteheads = reduce_parents(head_commit, head_subsumed, remoteheads);
		якщо (autogen) {
			struct commit_list *p;
			для (p = remoteheads; p; p = p->next)
				merge_name(merge_remote_util(p->item)->name, autogen);
		}
	}

	якщо (autogen) {
		prepare_merge_message(autogen, merge_msg);
		strbuf_release(autogen);
	}

	повернути remoteheads;
}

static ціле merging_a_throwaway_tag(struct commit *commit)
{
	char *tag_ref;
	struct object_id oid;
	ціле is_throwaway_tag = 0;

	/* Are we merging a tag? */
	якщо (!merge_remote_util(commit) ||
	    !merge_remote_util(commit)->obj ||
	    merge_remote_util(commit)->obj->type != OBJ_TAG)
		повернути is_throwaway_tag;

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
			  ((struct tag *)merge_remote_util(commit)->obj)->tag);
	якщо (!refs_read_ref(get_main_ref_store(the_repository), tag_ref, &oid) &&
	    oideq(&oid, &merge_remote_util(commit)->obj->oid))
		is_throwaway_tag = 0;
	інакше
		is_throwaway_tag = 1;
	free(tag_ref);
	повернути is_throwaway_tag;
}

ціле cmd_merge(ціле argc,
	      const char **argv,
	      const char *prefix,
	      struct repository *repo UNUSED)
{
	struct object_id result_tree, stash, head_oid;
	struct commit *head_commit;
	struct strbuf buf = STRBUF_INIT;
	ціле i, ret = 0, head_subsumed;
	ціле best_cnt = -1, merge_was_ok = 0, automerge_was_ok = 0;
	struct commit_list *common = NULL;
	const char *best_strategy = NULL, *wt_strategy = NULL;
	struct commit_list *remoteheads = NULL, *p;
	void *branch_to_free, *argv_to_free = NULL;
	ціле orig_argc = argc;
	ціле merge_log_config = -1;

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
	якщо (branch)
		skip_prefix(branch, "refs/heads/", &branch);

	init_diff_ui_defaults();
	repo_config(the_repository, git_merge_config, &merge_log_config);

	якщо (!branch || is_null_oid(&head_oid))
		head_commit = NULL;
	інакше
		head_commit = lookup_commit_or_die(&head_oid, "HEAD");

	якщо (branch_mergeoptions)
		parse_branch_merge_options(branch_mergeoptions);
	argc = parse_options(argc, argv, prefix, builtin_merge_options,
			builtin_merge_usage, 0);
	якщо (shortlog_len < 0)
		shortlog_len = (merge_log_config > 0) ? merge_log_config : 0;

	якщо (verbosity < 0 && show_progress == -1)
		show_progress = 0;

	якщо (abort_current_merge) {
		ціле nargc = 2;
		const char *nargv[] = {"reset", "--merge", NULL};
		char stash_oid_hex[GIT_MAX_HEXSZ + 1];
		struct object_id stash_oid = {0};

		якщо (orig_argc != 2)
			usage_msg_opt(_("--abort expects no arguments"),
			      builtin_merge_usage, builtin_merge_options);

		якщо (!file_exists(git_path_merge_head(the_repository)))
			die(_("There is no merge to abort (MERGE_HEAD missing)."));

		якщо (!refs_read_ref(get_main_ref_store(the_repository), "MERGE_AUTOSTASH", &stash_oid))
			refs_delete_ref(get_main_ref_store(the_repository),
					"", "MERGE_AUTOSTASH", &stash_oid,
					REF_NO_DEREF);

		/* Invoke 'git reset --merge' */
		ret = cmd_reset(nargc, nargv, prefix, the_repository);

		якщо (!is_null_oid(&stash_oid)) {
			oid_to_hex_r(stash_oid_hex, &stash_oid);
			apply_autostash_oid(stash_oid_hex);
		}

		goto done;
	}

	якщо (quit_current_merge) {
		якщо (orig_argc != 2)
			usage_msg_opt(_("--quit expects no arguments"),
				      builtin_merge_usage,
				      builtin_merge_options);

		remove_merge_branch_state(the_repository);
		goto done;
	}

	якщо (continue_current_merge) {
		ціле nargc = 1;
		const char *nargv[] = {"commit", NULL};

		якщо (orig_argc != 2)
			usage_msg_opt(_("--continue expects no arguments"),
			      builtin_merge_usage, builtin_merge_options);

		якщо (!file_exists(git_path_merge_head(the_repository)))
			die(_("There is no merge in progress (MERGE_HEAD missing)."));

		/* Invoke 'git commit' */
		ret = cmd_commit(nargc, nargv, prefix, the_repository);
		goto done;
	}

	якщо (repo_read_index_unmerged(the_repository))
		die_resolve_conflict("merge");

	якщо (file_exists(git_path_merge_head(the_repository))) {
		/*
		 * There is no unmerged entry, don't advise 'git
		 * add/rm <file>', just 'git commit'.
		 */
		якщо (advice_enabled(ADVICE_RESOLVE_CONFLICT))
			die(_("You have not concluded your merge (MERGE_HEAD exists).\n"
				  "Please, commit your changes before you merge."));
		інакше
			die(_("You have not concluded your merge (MERGE_HEAD exists)."));
	}
	якщо (refs_ref_exists(get_main_ref_store(the_repository), "CHERRY_PICK_HEAD")) {
		якщо (advice_enabled(ADVICE_RESOLVE_CONFLICT))
			die(_("You have not concluded your cherry-pick (CHERRY_PICK_HEAD exists).\n"
			    "Please, commit your changes before you merge."));
		інакше
			die(_("You have not concluded your cherry-pick (CHERRY_PICK_HEAD exists)."));
	}
	resolve_undo_clear_index(the_repository->index);

	якщо (option_edit < 0)
		option_edit = default_edit_option();

	cleanup_mode = get_cleanup_mode(cleanup_arg, 0 < option_edit);

	якщо (verbosity < 0)
		show_diffstat = 0;

	якщо (squash) {
		якщо (fast_forward == FF_NO)
			die(_("options '%s' and '%s' cannot be used together"), "--squash", "--no-ff.");
		якщо (option_commit > 0)
			die(_("options '%s' and '%s' cannot be used together"), "--squash", "--commit.");
		/*
		 * squash can now silently disable option_commit - this is not
		 * a problem as it is only overriding the default, not a user
		 * supplied option.
		 */
		option_commit = 0;
	}

	якщо (option_commit < 0)
		option_commit = 1;

	якщо (!argc) {
		якщо (default_to_upstream) {
			argc = setup_with_upstream(&argv);
			argv_to_free = argv;
		}
		інакше
			die(_("No commit specified and merge.defaultToUpstream not set."));
	} інакше якщо (argc == 1 && !strcmp(argv[0], "-")) {
		argv[0] = "@{-1}";
	}

	якщо (!argc)
		usage_with_options(builtin_merge_usage,
			builtin_merge_options);

	якщо (!head_commit) {
		/*
		 * If the merged head is a valid one there is no reason
		 * to forbid "git merge" into a branch yet to be born.
		 * We do the same for "git pull".
		 */
		struct object_id *remote_head_oid;
		якщо (squash)
			die(_("Squash commit into empty head not supported yet"));
		якщо (fast_forward == FF_NO)
			die(_("Non-fast-forward commit does not make sense into "
			    "an empty head"));
		remoteheads = collect_parents(head_commit, &head_subsumed,
					      argc, argv, NULL);
		якщо (!remoteheads)
			die(_("%s - not something we can merge"), argv[0]);
		якщо (remoteheads->next)
			die(_("Can merge only exactly one commit into empty head"));

		якщо (verify_signatures)
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

	якщо (!head_commit || !argc)
		usage_with_options(builtin_merge_usage,
			builtin_merge_options);

	якщо (verify_signatures) {
		для (p = remoteheads; p; p = p->next) {
			verify_merge_signature(p->item, verbosity,
					       check_trust_level);
		}
	}

	strbuf_addstr(&buf, "merge");
	для (p = remoteheads; p; p = p->next)
		strbuf_addf(&buf, " %s", merge_remote_util(p->item)->name);
	setenv("GIT_REFLOG_ACTION", buf.buf, 0);
	strbuf_reset(&buf);

	для (p = remoteheads; p; p = p->next) {
		struct commit *commit = p->item;
		strbuf_addf(&buf, "GITHEAD_%s",
			    oid_to_hex(&commit->object.oid));
		setenv(buf.buf, merge_remote_util(commit)->name, 1);
		strbuf_reset(&buf);
		якщо (fast_forward != FF_ONLY && merging_a_throwaway_tag(commit))
			fast_forward = FF_NO;
	}

	якщо (!use_strategies) {
		якщо (!remoteheads)
			; /* already up-to-date */
		інакше якщо (!remoteheads->next)
			add_strategies(pull_twohead, DEFAULT_TWOHEAD);
		інакше
			add_strategies(pull_octopus, DEFAULT_OCTOPUS);
	}

	для (i = 0; i < use_strategies_nr; i++) {
		якщо (use_strategies[i]->attr & NO_FAST_FORWARD)
			fast_forward = FF_NO;
		якщо (use_strategies[i]->attr & NO_TRIVIAL)
			allow_trivial = 0;
	}

	якщо (!remoteheads)
		; /* already up-to-date */
	інакше якщо (!remoteheads->next) {
		якщо (repo_get_merge_bases(the_repository, head_commit,
					 remoteheads->item, &common) < 0) {
			ret = 2;
			goto done;
		}
	} інакше {
		struct commit_list *list = remoteheads;
		commit_list_insert(head_commit, &list);
		якщо (get_octopus_merge_bases(list, &common) < 0) {
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

	якщо (remoteheads && !common) {
		/* No common ancestors found. */
		якщо (!allow_unrelated_histories)
			die(_("refusing to merge unrelated histories"));
		/* otherwise, we need a real merge. */
	} інакше якщо (!remoteheads ||
		 (!remoteheads->next && !common->next &&
		  common->item == remoteheads->item)) {
		/*
		 * If head can reach all the merge then we are up to date.
		 * but first the most common case of merging one remote.
		 */
		finish_up_to_date();
		goto done;
	} інакше якщо (fast_forward != FF_NO && !remoteheads->next &&
			!common->next &&
			oideq(&common->item->object.oid, &head_commit->object.oid)) {
		/* Again the most common case of merging one remote. */
		const char *msg = have_message ?
			"Fast-forward (no commit created; -m option ignored)" :
			"Fast-forward";
		struct commit *commit;

		якщо (verbosity >= 0) {
			printf(_("Updating %s..%s\n"),
			       repo_find_unique_abbrev(the_repository, &head_commit->object.oid,
						       DEFAULT_ABBREV),
			       repo_find_unique_abbrev(the_repository, &remoteheads->item->object.oid,
						       DEFAULT_ABBREV));
		}
		commit = remoteheads->item;
		якщо (!commit) {
			ret = 1;
			goto done;
		}

		якщо (autostash)
			create_autostash_ref(the_repository, "MERGE_AUTOSTASH",
					     NULL, false);
		якщо (checkout_fast_forward(the_repository,
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
	} інакше якщо (!remoteheads->next && common->next)
		;
		/*
		 * We are not doing octopus and not fast-forward.  Need
		 * a real merge.
		 */
	інакше якщо (!remoteheads->next && !common->next && option_commit) {
		/*
		 * We are not doing octopus, not fast-forward, and have
		 * only one common.
		 */
		refresh_index(the_repository->index, REFRESH_QUIET, NULL, NULL, NULL);
		якщо (allow_trivial && fast_forward != FF_ONLY) {
			/*
			 * Must first ensure that index matches HEAD before
			 * attempting a trivial merge.
			 */
			struct tree *head_tree = repo_get_commit_tree(the_repository,
								      head_commit);
			struct strbuf sb = STRBUF_INIT;

			якщо (repo_index_has_changes(the_repository, head_tree,
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
			якщо (!read_tree_trivial(&common->item->object.oid,
					       &head_commit->object.oid,
					       &remoteheads->item->object.oid)) {
				ret = merge_trivial(head_commit, remoteheads);
				goto done;
			}
			printf(_("Nope.\n"));
		}
	} інакше {
		/*
		 * An octopus.  If we can reach all the remote we are up
		 * to date.
		 */
		ціле up_to_date = 1;
		struct commit_list *j;

		для (j = remoteheads; j; j = j->next) {
			ціле ret = repo_in_merge_bases(the_repository,
						      j->item, head_commit);
			якщо (ret < 0)
				exit(128);
			якщо (!ret) {
				up_to_date = 0;
				break;
			}
		}
		якщо (up_to_date) {
			finish_up_to_date();
			goto done;
		}
	}

	якщо (fast_forward == FF_ONLY)
		die_ff_impossible();

	якщо (autostash)
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
	якщо (save_state(&stash))
		oidclr(&stash, the_repository->hash_algo);

	для (i = 0; i < use_strategies_nr; i++) {
		ціле ret, cnt;
		якщо (i) {
			printf(_("Rewinding the tree to pristine...\n"));
			restore_state(&head_commit->object.oid, &stash);
		}
		якщо (use_strategies_nr != 1)
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
		якщо (ret < 2) {
			якщо (!ret) {
				/*
				 * This strategy worked; no point in trying
				 * another.
				 */
				merge_was_ok = 1;
				best_strategy = wt_strategy;
				break;
			}
			cnt = (use_strategies_nr > 1) ? evaluate_result() : 0;
			якщо (best_cnt <= 0 || cnt <= best_cnt) {
				best_strategy = wt_strategy;
				best_cnt = cnt;
			}
		}
	}

	/*
	 * If we have a resulting tree, that means the strategy module
	 * auto resolved the merge cleanly.
	 */
	якщо (merge_was_ok && option_commit) {
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
	якщо (!best_strategy) {
		restore_state(&head_commit->object.oid, &stash);
		якщо (use_strategies_nr > 1)
			fprintf(stderr,
				_("No merge strategy handled the merge.\n"));
		інакше
			fprintf(stderr, _("Merge with strategy %s failed.\n"),
				use_strategies[0]->name);
		apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
				    NULL, NULL, NULL, NULL);
		ret = 2;
		goto done;
	} інакше якщо (best_strategy == wt_strategy)
		; /* We already have its result in the working tree. */
	інакше {
		printf(_("Rewinding the tree to pristine...\n"));
		restore_state(&head_commit->object.oid, &stash);
		printf(_("Using the %s strategy to prepare resolving by hand.\n"),
			best_strategy);
		try_merge_strategy(best_strategy, common, remoteheads,
				   head_commit);
	}

	якщо (squash) {
		finish(head_commit, remoteheads, NULL, NULL);

		git_test_write_commit_graph_or_die(the_repository);
	} інакше
		write_merge_state(remoteheads);

	якщо (merge_was_ok)
		fprintf(stderr, _("Automatic merge went well; "
			"stopped before committing as requested\n"));
	інакше
		ret = suggest_conflicts();
	якщо (autostash)
		printf(_("When finished, apply stashed changes with `git stash pop`\n"));

done:
	якщо (!automerge_was_ok) {
		commit_list_free(common);
		commit_list_free(remoteheads);
	}
	strbuf_release(&buf);
	free(branch_to_free);
	free(argv_to_free);
	free(pull_twohead);
	free(pull_octopus);
	discard_index(the_repository->index);
	повернути ret;
}
