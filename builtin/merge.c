/*
 * Builtin "git merge"
 *
 * Copyright (c) 2008 Miklos Vajna <vmiklos@frugalware.org>
 *
 * Based on git-merge.sh by Junio C Hamano.
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ଅନ୍ତର୍ଭାଵ "builtin.h"

#ଅନ୍ତର୍ଭାଵ "abspath.h"
#ଅନ୍ତର୍ଭାଵ "advice.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "editor.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "hex.h"
#ଅନ୍ତର୍ଭାଵ "object-name.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "lockfile.h"
#ଅନ୍ତର୍ଭାଵ "repository.h"
#ଅନ୍ତର୍ଭାଵ "run-command.h"
#ଅନ୍ତର୍ଭାଵ "hook.h"
#ଅନ୍ତର୍ଭାଵ "diff.h"
#ଅନ୍ତର୍ଭାଵ "diff-merges.h"
#ଅନ୍ତର୍ଭାଵ "refs.h"
#ଅନ୍ତର୍ଭାଵ "refspec.h"
#ଅନ୍ତର୍ଭାଵ "commit.h"
#ଅନ୍ତର୍ଭାଵ "diffcore.h"
#ଅନ୍ତର୍ଭାଵ "path.h"
#ଅନ୍ତର୍ଭାଵ "revision.h"
#ଅନ୍ତର୍ଭାଵ "unpack-trees.h"
#ଅନ୍ତର୍ଭାଵ "cache-tree.h"
#ଅନ୍ତର୍ଭାଵ "dir.h"
#ଅନ୍ତର୍ଭାଵ "color.h"
#ଅନ୍ତର୍ଭାଵ "rerere.h"
#ଅନ୍ତର୍ଭାଵ "help.h"
#ଅନ୍ତର୍ଭାଵ "merge.h"
#ଅନ୍ତର୍ଭାଵ "merge-ort-wrappers.h"
#ଅନ୍ତର୍ଭାଵ "resolve-undo.h"
#ଅନ୍ତର୍ଭାଵ "remote.h"
#ଅନ୍ତର୍ଭାଵ "fmt-merge-msg.h"
#ଅନ୍ତର୍ଭାଵ "sequencer.h"
#ଅନ୍ତର୍ଭାଵ "string-list.h"
#ଅନ୍ତର୍ଭାଵ "tag.h"
#ଅନ୍ତର୍ଭାଵ "alias.h"
#ଅନ୍ତର୍ଭାଵ "branch.h"
#ଅନ୍ତର୍ଭାଵ "commit-reach.h"
#ଅନ୍ତର୍ଭାଵ "wt-status.h"
#ଅନ୍ତର୍ଭାଵ "commit-graph.h"

#define DEFAULT_TWOHEAD (1<<0)
#define DEFAULT_OCTOPUS (1<<1)
#define NO_FAST_FORWARD (1<<2)
#define NO_TRIVIAL      (1<<3)

ସଂରଚନା strategy {
	ଅଚଳ ଅକ୍ଷର *name;
	unsigned attr;
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ builtin_merge_usage[] = {
	N_("git merge [<options>] [<commit>...]"),
	"git merge --abort",
	"git merge --continue",
	ଖାଲି
};

#define MERGE_SHOW_DIFFSTAT 1
#define MERGE_SHOW_COMPACTSUMMARY 2

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_diffstat = MERGE_SHOW_DIFFSTAT, shortlog_len = -1, squash;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା option_commit = -1;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା option_edit = -1;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା allow_trivial = 1, have_message, verify_signatures;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା check_trust_level = 1;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା overwrite_ignore = 1;
ସ୍ଥିର ସଂରଚନା strbuf merge_ସନ୍ଦେଶ = STRBUF_INIT;
ସ୍ଥିର ସଂରଚନା strategy **use_strategies;
ସ୍ଥିର size_t use_strategies_nr, use_strategies_alloc;
ସ୍ଥିର ସଂରଚନା strvec xopts = STRVEC_INIT;
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *branch;
ସ୍ଥିର ଅକ୍ଷର *branch_mergeoptions;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା verbosity;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା allow_rerere_auto;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା abort_current_merge;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା quit_current_merge;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା continue_current_merge;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା allow_unrelated_histories;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_progress = -1;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା default_to_upstream = 1;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା signoff;
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *sign_commit;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା autostash;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା no_verify;
ସ୍ଥିର ଅକ୍ଷର *into_name;

ସ୍ଥିର ସଂରଚନା strategy all_strategy[] = {
	{ "recursive",  NO_TRIVIAL },
	{ "octopus",    DEFAULT_OCTOPUS },
	{ "ort",        DEFAULT_TWOHEAD | NO_TRIVIAL },
	{ "resolve",    0 },
	{ "ours",       NO_FAST_FORWARD | NO_TRIVIAL },
	{ "subtree",    NO_FAST_FORWARD | NO_TRIVIAL },
};

ସ୍ଥିର ଅକ୍ଷର *pull_twohead, *pull_octopus;

ଗଣନା ff_type {
	FF_NO,
	FF_ALLOW,
	FF_ONLY
};

ସ୍ଥିର ଗଣନା ff_type fast_forward = FF_ALLOW;

ସ୍ଥିର ଅକ୍ଷର *cleanup_arg;
ସ୍ଥିର ଗଣନା commit_ସନ୍ଦେଶ_cleanup_mode cleanup_mode;

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା option_parse_message(ଅଚଳ ସଂରଚନା option *opt,
				ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ସଂରଚନା strbuf *buf = opt->ମୂଲ୍ୟ;

	ଯଦି (unset)
		strbuf_setlen(buf, 0);
	ଅନ୍ୟଥା ଯଦି (arg) {
		strbuf_addf(buf, "%s%s", buf->len ? "\n\n" : "", arg);
		have_message = 1;
	} ଅନ୍ୟଥା
		ଫେରନ୍ତୁ error(_("switch `m' requires a value"));
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଗଣନା parse_opt_result option_read_message(ସଂରଚନା parse_opt_ctx_t *ctx,
						 ଅଚଳ ସଂରଚନା option *opt,
						 ଅଚଳ ଅକ୍ଷର *arg_not_used,
						 ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ସଂରଚନା strbuf *buf = opt->ମୂଲ୍ୟ;
	ଅଚଳ ଅକ୍ଷର *arg;

	BUG_ON_OPT_ARG(arg_not_used);
	ଯଦି (unset)
		BUG("-F cannot be negated");

	ଯଦି (ctx->opt) {
		arg = ctx->opt;
		ctx->opt = ଖାଲି;
	} ଅନ୍ୟଥା ଯଦି (ctx->argc > 1) {
		ctx->argc--;
		arg = *++ctx->argv;
	} ଅନ୍ୟଥା
		ଫେରନ୍ତୁ error(_("option `%s' requires a value"), opt->long_name);

	ଯଦି (buf->len)
		strbuf_addch(buf, '\n');
	ଯଦି (ctx->prefix && !is_absolute_path(arg))
		arg = prefix_filename(ctx->prefix, arg);
	ଯଦି (strbuf_read_file(buf, arg, 0) < 0)
		ଫେରନ୍ତୁ error(_("could not read file '%s'"), arg);
	have_message = 1;

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ସଂରଚନା strategy *get_strategy(ଅଚଳ ଅକ୍ଷର *name)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	ସଂରଚନା strategy *ret;
	ସ୍ଥିର ସଂରଚନା cmdnames main_cmds = {0}, other_cmds = {0};
	ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା loaded;
	ଅକ୍ଷର *default_strategy = ଖାଲି;

	ଯଦି (!name)
		ଫେରନ୍ତୁ ଖାଲି;

	ଯଦି (default_strategy &&
	    !strcmp(default_strategy, "ort") &&
	    !strcmp(name, "recursive")) {
		name = "ort";
	}

	ପାଇଁ (i = 0; i < ARRAY_SIZE(all_strategy); i++)
		ଯଦି (!strcmp(name, all_strategy[i].name))
			ଫେରନ୍ତୁ &all_strategy[i];

	ଯଦି (!loaded) {
		ସଂରଚନା cmdnames not_strategies = {0};
		loaded = 1;

		load_command_list("git-merge-", &main_cmds, &other_cmds);
		ପାଇଁ (i = 0; i < main_cmds.cnt; i++) {
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା j, found = 0;
			ସଂରଚନା cmdname *ent = main_cmds.names[i];
			ପାଇଁ (j = 0; !found && j < ARRAY_SIZE(all_strategy); j++)
				ଯଦି (!xstrncmpz(all_strategy[j].name, ent->name, ent->len))
					found = 1;
			ଯଦି (!found)
				add_cmdname(&not_strategies, ent->name, ent->len);
		}
		exclude_cmds(&main_cmds, &not_strategies);

		cmdnames_release(&not_strategies);
	}
	ଯଦି (!is_in_cmdlist(&main_cmds, name) && !is_in_cmdlist(&other_cmds, name)) {
		fprintf(stderr, _("Could not find merge strategy '%s'.\n"), name);
		fprintf(stderr, _("Available strategies are:"));
		ପାଇଁ (i = 0; i < main_cmds.cnt; i++)
			fprintf(stderr, " %s", main_cmds.names[i]->name);
		fprintf(stderr, ".\n");
		ଯଦି (other_cmds.cnt) {
			fprintf(stderr, _("Available custom strategies are:"));
			ପାଇଁ (i = 0; i < other_cmds.cnt; i++)
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
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ଶୂନ୍ୟ append_strategy(ସଂରଚନା strategy *s)
{
	ALLOC_GROW(use_strategies, use_strategies_nr + 1, use_strategies_alloc);
	use_strategies[use_strategies_nr++] = s;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା option_parse_strategy(ଅଚଳ ସଂରଚନା option *opt UNUSED,
				 ଅଚଳ ଅକ୍ଷର *name, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ଯଦି (unset)
		ଫେରନ୍ତୁ 0;

	append_strategy(get_strategy(name));
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା option_parse_compact_summary(ଅଚଳ ସଂରଚନା option *opt,
					ଅଚଳ ଅକ୍ଷର *name UNUSED, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା *setting = opt->ମୂଲ୍ୟ;

	ଯଦି (unset)
		*setting = 0;
	ଅନ୍ୟଥା
		*setting = MERGE_SHOW_COMPACTSUMMARY;
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ସଂରଚନା option builtin_merge_options[] = {
	OPT_SET_INT('n', ଖାଲି, &show_diffstat,
		N_("do not show a diffstat at the end of the merge"), 0),
	OPT_BOOL(0, "stat", &show_diffstat,
		N_("show a diffstat at the end of the merge")),
	OPT_BOOL(0, "summary", &show_diffstat, N_("(synonym to --stat)")),
	OPT_CALLBACK_F(0, "compact-summary", &show_diffstat, ଖାଲି,
		       N_("show a compact-summary at the end of the merge"),
		       PARSE_OPT_NOARG,
		       option_parse_compact_summary),
	{
		.type = OPTION_INTEGER,
		.long_name = "log",
		.ମୂଲ୍ୟ = &shortlog_len,
		.precision = ଆକାର(shortlog_len),
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
	OPT_CALLBACK('s', "strategy", ଖାଲି, N_("strategy"),
		N_("merge strategy to use"), option_parse_strategy),
	OPT_STRVEC('X', "strategy-option", &xopts, N_("option=value"),
		N_("option for selected merge strategy")),
	OPT_CALLBACK('m', "message", &merge_ସନ୍ଦେଶ, N_("message"),
		N_("merge commit message (for a non-fast-forward merge)"),
		option_parse_message),
	{
		.type = OPTION_LOWLEVEL_CALLBACK,
		.short_name = 'F',
		.long_name = "file",
		.ମୂଲ୍ୟ = &merge_ସନ୍ଦେଶ,
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
		.ମୂଲ୍ୟ = &sign_commit,
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

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା save_state(ସଂରଚନା object_id *stash)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା len;
	ସଂରଚନା child_process cp = CHILD_PROCESS_INIT;
	ସଂରଚନା strbuf buffer = STRBUF_INIT;
	ସଂରଚନା lock_file lock_file = LOCK_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା rc = -1;

	fd = repo_hold_locked_index(the_repository, &lock_file, 0);
	refresh_index(the_repository->index, REFRESH_QUIET, ଖାଲି, ଖାଲି, ଖାଲି);
	ଯଦି (0 <= fd)
		repo_update_index_if_able(the_repository, &lock_file);
	rollback_lock_file(&lock_file);

	strvec_pushl(&cp.args, "stash", "create", ଖାଲି);
	cp.out = -1;
	cp.git_cmd = 1;

	ଯଦି (start_command(&cp))
		die(_("could not run stash."));
	len = strbuf_read(&buffer, cp.out, 1024);
	close(cp.out);

	ଯଦି (finish_command(&cp) || len < 0)
		die(_("stash failed"));
	ଅନ୍ୟଥା ଯଦି (!len)		/* no changes */
		ଯାଅ out;
	strbuf_setlen(&buffer, buffer.len-1);
	ଯଦି (repo_get_oid(the_repository, buffer.buf, stash))
		die(_("not a valid object: %s"), buffer.buf);
	rc = 0;
out:
	strbuf_release(&buffer);
	ଫେରନ୍ତୁ rc;
}

ସ୍ଥିର ଶୂନ୍ୟ read_empty(ଅଚଳ ସଂରଚନା object_id *oid)
{
	ସଂରଚନା child_process cmd = CHILD_PROCESS_INIT;

	strvec_pushl(&cmd.args, "read-tree", "-m", "-u",
		     empty_ବୃକ୍ଷ_oid_hex(the_repository->hash_algo),
		     oid_to_hex(oid), ଖାଲି);
	cmd.git_cmd = 1;

	ଯଦି (run_command(&cmd))
		die(_("read-tree failed"));
}

ସ୍ଥିର ଶୂନ୍ୟ reset_hard(ଅଚଳ ସଂରଚନା object_id *oid)
{
	ସଂରଚନା child_process cmd = CHILD_PROCESS_INIT;

	strvec_pushl(&cmd.args, "read-tree", "-v", "--reset", "-u",
		     oid_to_hex(oid), ଖାଲି);
	cmd.git_cmd = 1;

	ଯଦି (run_command(&cmd))
		die(_("read-tree failed"));
}

ସ୍ଥିର ଶୂନ୍ୟ restore_state(ଅଚଳ ସଂରଚନା object_id *head,
			  ଅଚଳ ସଂରଚନା object_id *stash)
{
	ସଂରଚନା child_process cmd = CHILD_PROCESS_INIT;

	reset_hard(head);

	ଯଦି (is_null_oid(stash))
		ଯାଅ refresh_cache;

	strvec_pushl(&cmd.args, "stash", "apply", "--index", "--quiet", ଖାଲି);
	strvec_ଠେଲ(&cmd.args, oid_to_hex(stash));

	/*
	 * It is OK to ignore error here, for example when there was
	 * nothing to restore.
	 */
	cmd.git_cmd = 1;
	run_command(&cmd);

refresh_cache:
	discard_index(the_repository->index);
	ଯଦି (repo_read_index(the_repository) < 0)
		die(_("could not read index"));
}

/* This is called when no merge was necessary. */
ସ୍ଥିର ଶୂନ୍ୟ finish_up_to_date(ଶୂନ୍ୟ)
{
	ଯଦି (verbosity >= 0) {
		ଯଦି (squash)
			puts(_("Already up to date. (nothing to squash)"));
		ଅନ୍ୟଥା
			puts(_("Already up to date."));
	}
	remove_merge_branch_state(the_repository);
}

ସ୍ଥିର ଶୂନ୍ୟ squash_message(ସଂରଚନା commit *commit, ସଂରଚନା commit_list *remoteheads)
{
	ସଂରଚନା rev_info rev;
	ସଂରଚନା strbuf out = STRBUF_INIT;
	ସଂରଚନା commit_list *j;
	ସଂରଚନା pretty_print_context ctx = {0};

	printf(_("Squash commit -- not updating HEAD\n"));

	repo_init_revisions(the_repository, &rev, ଖାଲି);
	diff_merges_suppress(&rev);
	rev.commit_format = CMIT_FMT_MEDIUM;

	commit->object.flags |= UNINTERESTING;
	add_pending_object(&rev, &commit->object, ଖାଲି);

	ପାଇଁ (j = remoteheads; j; j = j->next)
		add_pending_object(&rev, &j->item->object, ଖାଲି);

	setup_revisions(0, ଖାଲି, &rev, ଖାଲି);
	ଯଦି (prepare_revision_walk(&rev))
		die(_("revision walk setup failed"));

	ctx.abbrev = rev.abbrev;
	ctx.date_mode = rev.date_mode;
	ctx.fmt = rev.commit_format;

	strbuf_addstr(&out, "Squashed commit of the following:\n");
	ଯେତେବେଳେ ((commit = get_revision(&rev)) != ଖାଲି) {
		strbuf_addch(&out, '\n');
		strbuf_addf(&out, "commit %s\n",
			oid_to_hex(&commit->object.oid));
		pretty_print_commit(&ctx, commit, &out);
	}
	write_file_buf(git_path_squash_ସନ୍ଦେଶ(the_repository), out.buf, out.len);
	strbuf_release(&out);
	release_revisions(&rev);
}

ସ୍ଥିର ଶୂନ୍ୟ finish(ସଂରଚନା commit *head_commit,
		   ସଂରଚନା commit_list *remoteheads,
		   ଅଚଳ ସଂରଚନା object_id *new_head, ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ)
{
	ସଂରଚନା strbuf reflog_message = STRBUF_INIT;
	ଅଚଳ ସଂରଚନା object_id *head = &head_commit->object.oid;

	ଯଦି (!ସନ୍ଦେଶ)
		strbuf_addstr(&reflog_message, getenv("GIT_REFLOG_ACTION"));
	ଅନ୍ୟଥା {
		ଯଦି (verbosity >= 0)
			printf("%s\n", ସନ୍ଦେଶ);
		strbuf_addf(&reflog_message, "%s: %s",
			getenv("GIT_REFLOG_ACTION"), ସନ୍ଦେଶ);
	}
	ଯଦି (squash) {
		squash_message(head_commit, remoteheads);
	} ଅନ୍ୟଥା {
		ଯଦି (verbosity >= 0 && !merge_ସନ୍ଦେଶ.len)
			printf(_("No merge message -- not updating HEAD\n"));
		ଅନ୍ୟଥା {
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
	ଯଦି (new_head && show_diffstat) {
		ସଂରଚନା diff_options opts;
		repo_diff_setup(the_repository, &opts);
		init_diffstat_widths(&opts);

		ବାଛ (show_diffstat) {
		ସ୍ଥିତି MERGE_SHOW_DIFFSTAT: /* 1 */
			opts.output_format |=
				DIFF_FORMAT_SUMMARY | DIFF_FORMAT_DIFFSTAT;
			ଭାଙ୍ଗ;
		ସ୍ଥିତି MERGE_SHOW_COMPACTSUMMARY: /* 2 */
			opts.output_format |= DIFF_FORMAT_DIFFSTAT;
			opts.flags.stat_with_summary = 1;
			ଭାଙ୍ଗ;
		ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
			ଭାଙ୍ଗ;
		}
		opts.detect_rename = DIFF_DETECT_RENAME;
		diff_setup_done(&opts);
		diff_ବୃକ୍ଷ_oid(head, new_head, "", &opts);
		diffcore_std(&opts);
		diff_flush(&opts);
	}

	/* Run a post-merge hook */
	run_hooks_l(the_repository, "post-merge", squash ? "1" : "0", ଖାଲି);

	ଯଦି (new_head)
		apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
				    ଖାଲି, ଖାଲି, ଖାଲି, ଖାଲି);
	strbuf_release(&reflog_message);
}

/* Get the name for the merge commit's message. */
ସ୍ଥିର ଶୂନ୍ୟ merge_name(ଅଚଳ ଅକ୍ଷର *remote, ସଂରଚନା strbuf *ସନ୍ଦେଶ)
{
	ସଂରଚନା commit *remote_head;
	ସଂରଚନା object_id branch_head;
	ସଂରଚନା strbuf bname = STRBUF_INIT;
	ସଂରଚନା merge_remote_desc *desc;
	ଅଚଳ ଅକ୍ଷର *ptr;
	ଅକ୍ଷର *found_ref = ଖାଲି;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, early;

	copy_branchname(the_repository, &bname, remote, 0);
	remote = bname.buf;

	oidclr(&branch_head, the_repository->hash_algo);
	remote_head = get_merge_parent(remote);
	ଯଦି (!remote_head)
		die(_("'%s' does not point to a commit"), remote);

	ଯଦି (repo_dwim_ref(the_repository, remote, strlen(remote), &branch_head,
			  &found_ref, 0) > 0) {
		ଯଦି (starts_with(found_ref, "refs/heads/")) {
			strbuf_addf(ସନ୍ଦେଶ, "%s\t\tbranch '%s' of .\n",
				    oid_to_hex(&branch_head), remote);
			ଯାଅ cleanup;
		}
		ଯଦି (starts_with(found_ref, "refs/tags/")) {
			strbuf_addf(ସନ୍ଦେଶ, "%s\t\ttag '%s' of .\n",
				    oid_to_hex(&branch_head), remote);
			ଯାଅ cleanup;
		}
		ଯଦି (starts_with(found_ref, "refs/remotes/")) {
			strbuf_addf(ସନ୍ଦେଶ, "%s\t\tremote-tracking branch '%s' of .\n",
				    oid_to_hex(&branch_head), remote);
			ଯାଅ cleanup;
		}
	}

	/* See if remote matches <name>^^^.. or <name>~<number> */
	ପାଇଁ (len = 0, ptr = remote + strlen(remote);
	     remote < ptr && ptr[-1] == '^';
	     ptr--)
		len++;
	ଯଦି (len)
		early = 1;
	ଅନ୍ୟଥା {
		early = 0;
		ptr = strrchr(remote, '~');
		ଯଦି (ptr) {
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା seen_nonzero = 0;

			len++; /* count ~ */
			ଯେତେବେଳେ (*++ptr && isdigit(*ptr)) {
				seen_nonzero |= (*ptr != '0');
				len++;
			}
			ଯଦି (*ptr)
				len = 0; /* not ...~<number> */
			ଅନ୍ୟଥା ଯଦି (seen_nonzero)
				early = 1;
			ଅନ୍ୟଥା ଯଦି (len == 1)
				early = 1; /* "name~" is "name~1"! */
		}
	}
	ଯଦି (len) {
		ସଂରଚନା strbuf truname = STRBUF_INIT;
		strbuf_addf(&truname, "refs/heads/%s", remote);
		strbuf_setlen(&truname, truname.len - len);
		ଯଦି (refs_ref_exists(get_main_ref_store(the_repository), truname.buf)) {
			strbuf_addf(ସନ୍ଦେଶ,
				    "%s\t\tbranch '%s'%s of .\n",
				    oid_to_hex(&remote_head->object.oid),
				    truname.buf + 11,
				    (early ? " (early part)" : ""));
			strbuf_release(&truname);
			ଯାଅ cleanup;
		}
		strbuf_release(&truname);
	}

	desc = merge_remote_util(remote_head);
	ଯଦି (desc && desc->obj && desc->obj->type == OBJ_TAG) {
		strbuf_addf(ସନ୍ଦେଶ, "%s\t\t%s '%s'\n",
			    oid_to_hex(&desc->obj->oid),
			    type_name(desc->obj->type),
			    remote);
		ଯାଅ cleanup;
	}

	strbuf_addf(ସନ୍ଦେଶ, "%s\t\tcommit '%s'\n",
		oid_to_hex(&remote_head->object.oid), remote);
cleanup:
	ମୁକ୍ତ(found_ref);
	strbuf_release(&bname);
}

ସ୍ଥିର ଶୂନ୍ୟ parse_branch_merge_options(ଅକ୍ଷର *bmo)
{
	ଅଚଳ ଅକ୍ଷର **argv;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc;

	ଯଦି (!bmo)
		ଫେରନ୍ତୁ;
	argc = split_cmdline(bmo, &argv);
	ଯଦି (argc < 0)
		die(_("Bad branch.%s.mergeoptions string: %s"), branch,
		    _(split_cmdline_strerror(argc)));
	REALLOC_ARRAY(argv, argc + 2);
	MOVE_ARRAY(argv + 1, argv, argc + 1);
	argc++;
	argv[0] = "branch.*.mergeoptions";
	parse_options(argc, argv, ଖାଲି, builtin_merge_options,
		      builtin_merge_usage, 0);
	ମୁକ୍ତ(argv);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା git_merge_config(ଅଚଳ ଅକ୍ଷର *k, ଅଚଳ ଅକ୍ଷର *v,
			    ଅଚଳ ସଂରଚନା config_context *ctx, ଶୂନ୍ୟ *cb)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା status;
	ଅଚଳ ଅକ୍ଷର *str;

	ଯଦି (branch &&
	    skip_prefix(k, "branch.", &str) &&
	    skip_prefix(str, branch, &str) &&
	    !strcmp(str, ".mergeoptions")) {
		ମୁକ୍ତ(branch_mergeoptions);
		branch_mergeoptions = xstrdup(v);
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (!strcmp(k, "merge.diffstat") || !strcmp(k, "merge.stat")) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା val = git_parse_maybe_bool_text(v);
		ବାଛ (val) {
		ସ୍ଥିତି 0:
			show_diffstat = 0;
			ଭାଙ୍ଗ;
		ସ୍ଥିତି 1:
			show_diffstat = MERGE_SHOW_DIFFSTAT;
			ଭାଙ୍ଗ;
		ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
			ଯଦି (!strcmp(v, "compact"))
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
			ଅନ୍ୟଥା
				/*
				 * A setting from a future?  It is not an
				 * error grave enough to fail the command.
				 * proceed using the default one.
				 */
				show_diffstat = MERGE_SHOW_DIFFSTAT;
			ଭାଙ୍ଗ;
		}
	} ଅନ୍ୟଥା ଯଦି (!strcmp(k, "merge.verifysignatures")) {
		verify_signatures = git_config_bool(k, v);
	} ଅନ୍ୟଥା ଯଦି (!strcmp(k, "pull.twohead")) {
		FREE_AND_NULL(pull_twohead);
		ଫେରନ୍ତୁ git_config_string(&pull_twohead, k, v);
	} ଅନ୍ୟଥା ଯଦି (!strcmp(k, "pull.octopus")) {
		FREE_AND_NULL(pull_octopus);
		ଫେରନ୍ତୁ git_config_string(&pull_octopus, k, v);
	} ଅନ୍ୟଥା ଯଦି (!strcmp(k, "commit.cleanup")) {
		ଫେରନ୍ତୁ git_config_string(&cleanup_arg, k, v);
	} ଅନ୍ୟଥା ଯଦି (!strcmp(k, "merge.ff")) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା boolval = git_parse_maybe_bool(v);
		ଯଦି (0 <= boolval) {
			fast_forward = boolval ? FF_ALLOW : FF_NO;
		} ଅନ୍ୟଥା ଯଦି (v && !strcmp(v, "only")) {
			fast_forward = FF_ONLY;
		} /* do not barf on values from future versions of git */
		ଫେରନ୍ତୁ 0;
	} ଅନ୍ୟଥା ଯଦି (!strcmp(k, "merge.defaulttoupstream")) {
		default_to_upstream = git_config_bool(k, v);
		ଫେରନ୍ତୁ 0;
	} ଅନ୍ୟଥା ଯଦି (!strcmp(k, "commit.gpgsign")) {
		sign_commit = git_config_bool(k, v) ? "" : ଖାଲି;
		ଫେରନ୍ତୁ 0;
	} ଅନ୍ୟଥା ଯଦି (!strcmp(k, "gpg.mintrustlevel")) {
		check_trust_level = 0;
	} ଅନ୍ୟଥା ଯଦି (!strcmp(k, "merge.autostash")) {
		autostash = git_config_bool(k, v);
		ଫେରନ୍ତୁ 0;
	}

	status = fmt_merge_ସନ୍ଦେଶ_config(k, v, ctx, cb);
	ଯଦି (status)
		ଫେରନ୍ତୁ status;
	ଫେରନ୍ତୁ git_diff_ui_config(k, v, ctx, cb);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା read_ବୃକ୍ଷ_trivial(ସଂରଚନା object_id *common, ସଂରଚନା object_id *head,
			     ସଂରଚନା object_id *one)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, nr_trees = 0;
	ସଂରଚନା ବୃକ୍ଷ *trees[MAX_UNPACK_TREES];
	ସଂରଚନା ବୃକ୍ଷ_desc t[MAX_UNPACK_TREES];
	ସଂରଚନା unpack_trees_options opts;

	memset(&opts, 0, ଆକାର(opts));
	opts.head_idx = 2;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;
	opts.update = 1;
	opts.verbose_update = 1;
	opts.trivial_merges_only = 1;
	opts.merge = 1;
	opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
	trees[nr_trees] = repo_parse_ବୃକ୍ଷ_indirect(the_repository, common);
	ଯଦି (!trees[nr_trees++])
		ଫେରନ୍ତୁ -1;
	trees[nr_trees] = repo_parse_ବୃକ୍ଷ_indirect(the_repository, head);
	ଯଦି (!trees[nr_trees++])
		ଫେରନ୍ତୁ -1;
	trees[nr_trees] = repo_parse_ବୃକ୍ଷ_indirect(the_repository, one);
	ଯଦି (!trees[nr_trees++])
		ଫେରନ୍ତୁ -1;
	opts.fn = threeway_merge;
	cache_ବୃକ୍ଷ_free(&the_repository->index->cache_ବୃକ୍ଷ);
	ପାଇଁ (i = 0; i < nr_trees; i++) {
		repo_parse_ବୃକ୍ଷ(the_repository, trees[i]);
		init_ବୃକ୍ଷ_desc(t+i, &trees[i]->object.oid,
			       trees[i]->buffer, trees[i]->size);
	}
	ଯଦି (unpack_trees(nr_trees, t, &opts))
		ଫେରନ୍ତୁ -1;
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ write_ବୃକ୍ଷ_trivial(ସଂରଚନା object_id *oid)
{
	ଯଦି (write_index_as_ବୃକ୍ଷ(oid, the_repository->index,
				repo_get_index_file(the_repository),
				0, ଖାଲି))
		die(_("git write-tree failed to write a tree"));
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା try_merge_strategy(ଅଚଳ ଅକ୍ଷର *strategy, ସଂରଚନା commit_list *common,
			      ସଂରଚନା commit_list *remoteheads,
			      ସଂରଚନା commit *head)
{
	ଅଚଳ ଅକ୍ଷର *head_arg = "HEAD";

	ଯଦି (repo_refresh_and_write_index(the_repository, REFRESH_QUIET,
					 SKIP_IF_UNCHANGED, 0, ଖାଲି, ଖାଲି,
					 ଖାଲି) < 0)
		die(_("Unable to write index."));

	ଯଦି (!strcmp(strategy, "recursive") || !strcmp(strategy, "subtree") ||
	    !strcmp(strategy, "ort")) {
		ସଂରଚନା lock_file lock = LOCK_INIT;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା clean, x;
		ସଂରଚନା commit *result;
		ସଂରଚନା commit_list *reversed = ଖାଲି;
		ସଂରଚନା merge_options o;
		ସଂରଚନା commit_list *j;

		ଯଦି (remoteheads->next) {
			error(_("Not handling anything other than two heads merge."));
			ଫେରନ୍ତୁ 2;
		}

		init_ui_merge_options(&o, the_repository);
		ଯଦି (!strcmp(strategy, "subtree"))
			o.subtree_shift = "";

		o.show_rename_progress =
			show_progress == -1 ? isatty(2) : show_progress;

		ପାଇଁ (x = 0; x < xopts.nr; x++)
			ଯଦି (parse_merge_opt(&o, xopts.v[x]))
				die(_("unknown strategy option: -X%s"), xopts.v[x]);

		o.branch1 = head_arg;
		o.branch2 = merge_remote_util(remoteheads->item)->name;

		ପାଇଁ (j = common; j; j = j->next)
			commit_list_ଭର୍ତ୍ତି(j->item, &reversed);

		repo_hold_locked_index(the_repository, &lock,
				       LOCK_DIE_ON_ERROR);
		clean = merge_ort_recursive(&o, head, remoteheads->item,
					    reversed, &result);
		commit_list_free(reversed);
		strbuf_release(&o.obuf);

		ଯଦି (clean < 0) {
			rollback_lock_file(&lock);
			ଫେରନ୍ତୁ 2;
		}
		ଯଦି (write_locked_index(the_repository->index, &lock,
				       COMMIT_LOCK | SKIP_IF_UNCHANGED))
			die(_("unable to write %s"), repo_get_index_file(the_repository));
		ଫେରନ୍ତୁ clean ? 0 : 1;
	} ଅନ୍ୟଥା {
		ଫେରନ୍ତୁ try_merge_command(the_repository,
					 strategy, xopts.nr, xopts.v,
					 common, head_arg, remoteheads);
	}
}

ସ୍ଥିର ଶୂନ୍ୟ ଗଣନା_diff_files(ସଂରଚନା diff_queue_struct *q,
			     ସଂରଚନା diff_options *opt UNUSED, ଶୂନ୍ୟ *data)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା *ଗଣନା_2 = data;

	(*ଗଣନା_2) += q->nr;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_unmerged_entries(ଶୂନ୍ୟ)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, ret = 0;

	ପାଇଁ (i = 0; i < the_repository->index->cache_nr; i++)
		ଯଦି (ce_stage(the_repository->index->cache[i]))
			ret++;

	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ଶୂନ୍ୟ add_strategies(ଅଚଳ ଅକ୍ଷର *string, unsigned attr)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	ଯଦି (string) {
		ସଂରଚନା string_list list = STRING_LIST_INIT_DUP;
		ସଂରଚନା string_list_item *item;
		string_list_split(&list, string, " ", -1);
		for_each_string_list_item(item, &list)
			append_strategy(get_strategy(item->string));
		string_list_clear(&list, 0);
		ଫେରନ୍ତୁ;
	}
	ପାଇଁ (i = 0; i < ARRAY_SIZE(all_strategy); i++)
		ଯଦି (all_strategy[i].attr & attr)
			append_strategy(&all_strategy[i]);

}

ସ୍ଥିର ଶୂନ୍ୟ read_merge_ସନ୍ଦେଶ(ସଂରଚନା strbuf *ସନ୍ଦେଶ)
{
	ଅଚଳ ଅକ୍ଷର *filename = git_path_merge_ସନ୍ଦେଶ(the_repository);
	strbuf_reset(ସନ୍ଦେଶ);
	ଯଦି (strbuf_read_file(ସନ୍ଦେଶ, filename, 0) < 0)
		die_errno(_("Could not read from '%s'"), filename);
}

ସ୍ଥିର ଶୂନ୍ୟ write_merge_state(ସଂରଚନା commit_list *);
ସ୍ଥିର ଶୂନ୍ୟ abort_commit(ସଂରଚନା commit_list *remoteheads, ଅଚଳ ଅକ୍ଷର *err_ସନ୍ଦେଶ)
{
	ଯଦି (err_ସନ୍ଦେଶ)
		error("%s", err_ସନ୍ଦେଶ);
	fprintf(stderr,
		_("Not committing merge; use 'git commit' to complete the merge.\n"));
	write_merge_state(remoteheads);
	exit(1);
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର merge_editor_comment[] =
N_("Please enter a commit message to explain why this merge is necessary,\n"
   "especially if it merges an updated upstream into a topic branch.\n"
   "\n");

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର scissors_editor_comment[] =
N_("An empty message aborts the commit.\n");

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର no_scissors_editor_comment[] =
N_("Lines starting with '%s' will be ignored, and an empty message aborts\n"
   "the commit.\n");

ସ୍ଥିର ଶୂନ୍ୟ write_merge_heads(ସଂରଚନା commit_list *);
ସ୍ଥିର ଶୂନ୍ୟ prepare_to_commit(ସଂରଚନା commit_list *remoteheads)
{
	ସଂରଚନା strbuf ସନ୍ଦେଶ = STRBUF_INIT;
	ଅଚଳ ଅକ୍ଷର *index_file = repo_get_index_file(the_repository);

	ଯଦି (!no_verify) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା invoked_hook;

		ଯଦି (run_commit_hook(0 < option_edit, index_file, &invoked_hook,
				    "pre-merge-commit", ଖାଲି))
			abort_commit(remoteheads, ଖାଲି);
		/*
		 * Re-read the index as pre-merge-commit hook could have updated it,
		 * and write it out as a tree.  We must do this before we invoke
		 * the editor and after we invoke run_status above.
		 */
		ଯଦି (invoked_hook)
			discard_index(the_repository->index);
	}
	read_index_from(the_repository->index, index_file,
			repo_get_git_dir(the_repository));
	strbuf_addbuf(&ସନ୍ଦେଶ, &merge_ସନ୍ଦେଶ);
	ଯଦି (squash)
		BUG("the control must not reach here under --squash");
	ଯଦି (0 < option_edit) {
		strbuf_addch(&ସନ୍ଦେଶ, '\n');
		ଯଦି (cleanup_mode == COMMIT_ସନ୍ଦେଶ_CLEANUP_SCISSORS) {
			wt_status_append_cut_ଧାଡ଼ି(&ସନ୍ଦେଶ);
			strbuf_commented_addf(&ସନ୍ଦେଶ, comment_ଧାଡ଼ି_str, "\n");
		}
		strbuf_commented_addf(&ସନ୍ଦେଶ, comment_ଧାଡ଼ି_str,
				      _(merge_editor_comment));
		ଯଦି (cleanup_mode == COMMIT_ସନ୍ଦେଶ_CLEANUP_SCISSORS)
			strbuf_commented_addf(&ସନ୍ଦେଶ, comment_ଧାଡ଼ି_str,
					      _(scissors_editor_comment));
		ଅନ୍ୟଥା
			strbuf_commented_addf(&ସନ୍ଦେଶ, comment_ଧାଡ଼ି_str,
				_(no_scissors_editor_comment), comment_ଧାଡ଼ି_str);
	}
	ଯଦି (signoff)
		append_signoff(&ସନ୍ଦେଶ, ignored_log_message_bytes(ସନ୍ଦେଶ.buf, ସନ୍ଦେଶ.len), 0);
	write_merge_heads(remoteheads);
	write_file_buf(git_path_merge_ସନ୍ଦେଶ(the_repository), ସନ୍ଦେଶ.buf, ସନ୍ଦେଶ.len);
	ଯଦି (run_commit_hook(0 < option_edit, repo_get_index_file(the_repository),
			    ଖାଲି, "prepare-commit-msg",
			    git_path_merge_ସନ୍ଦେଶ(the_repository), "merge", ଖାଲି))
		abort_commit(remoteheads, ଖାଲି);
	ଯଦି (0 < option_edit) {
		ଯଦି (launch_editor(git_path_merge_ସନ୍ଦେଶ(the_repository), ଖାଲି, ଖାଲି))
			abort_commit(remoteheads, ଖାଲି);
	}

	ଯଦି (!no_verify && run_commit_hook(0 < option_edit, repo_get_index_file(the_repository),
					  ଖାଲି, "commit-msg",
					  git_path_merge_ସନ୍ଦେଶ(the_repository), ଖାଲି))
		abort_commit(remoteheads, ଖାଲି);

	read_merge_ସନ୍ଦେଶ(&ସନ୍ଦେଶ);
	cleanup_message(&ସନ୍ଦେଶ, cleanup_mode, 0);
	ଯଦି (!ସନ୍ଦେଶ.len)
		abort_commit(remoteheads, _("Empty commit message."));
	strbuf_release(&merge_ସନ୍ଦେଶ);
	strbuf_addbuf(&merge_ସନ୍ଦେଶ, &ସନ୍ଦେଶ);
	strbuf_release(&ସନ୍ଦେଶ);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା merge_trivial(ସଂରଚନା commit *head, ସଂରଚନା commit_list *remoteheads)
{
	ସଂରଚନା object_id result_ବୃକ୍ଷ, result_commit;
	ସଂରଚନା commit_list *parents = ଖାଲି, **pptr = &parents;

	ଯଦି (repo_refresh_and_write_index(the_repository, REFRESH_QUIET,
					 SKIP_IF_UNCHANGED, 0, ଖାଲି, ଖାଲି,
					 ଖାଲି) < 0)
		ଫେରନ୍ତୁ error(_("Unable to write index."));

	write_ବୃକ୍ଷ_trivial(&result_ବୃକ୍ଷ);
	printf(_("Wonderful.\n"));
	pptr = commit_list_append(head, pptr);
	pptr = commit_list_append(remoteheads->item, pptr);
	prepare_to_commit(remoteheads);
	ଯଦି (commit_ବୃକ୍ଷ(merge_ସନ୍ଦେଶ.buf, merge_ସନ୍ଦେଶ.len, &result_ବୃକ୍ଷ, parents,
			&result_commit, ଖାଲି, sign_commit))
		die(_("failed to write commit object"));
	finish(head, remoteheads, &result_commit, "In-index merge");

	remove_merge_branch_state(the_repository);
	commit_list_free(parents);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା finish_automerge(ସଂରଚନା commit *head,
			    ପୂର୍ଣ୍ଣସଂଖ୍ୟା head_subsumed,
			    ସଂରଚନା commit_list *common,
			    ସଂରଚନା commit_list *remoteheads,
			    ସଂରଚନା object_id *result_ବୃକ୍ଷ,
			    ଅଚଳ ଅକ୍ଷର *wt_strategy)
{
	ସଂରଚନା commit_list *parents = ଖାଲି;
	ସଂରଚନା strbuf buf = STRBUF_INIT;
	ସଂରଚନା object_id result_commit;

	write_ବୃକ୍ଷ_trivial(result_ବୃକ୍ଷ);
	commit_list_free(common);
	parents = remoteheads;
	ଯଦି (!head_subsumed || fast_forward == FF_NO)
		commit_list_ଭର୍ତ୍ତି(head, &parents);
	prepare_to_commit(remoteheads);
	ଯଦି (commit_ବୃକ୍ଷ(merge_ସନ୍ଦେଶ.buf, merge_ସନ୍ଦେଶ.len, result_ବୃକ୍ଷ, parents,
			&result_commit, ଖାଲି, sign_commit))
		die(_("failed to write commit object"));
	strbuf_addf(&buf, "Merge made by the '%s' strategy.", wt_strategy);
	finish(head, remoteheads, &result_commit, buf.buf);

	strbuf_release(&buf);
	remove_merge_branch_state(the_repository);
	commit_list_free(parents);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା suggest_conflicts(ଶୂନ୍ୟ)
{
	ଅଚଳ ଅକ୍ଷର *filename;
	FILE *fp;
	ସଂରଚନା strbuf msgbuf = STRBUF_INIT;

	filename = git_path_merge_ସନ୍ଦେଶ(the_repository);
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
	ଫେରନ୍ତୁ 1;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା evaluate_result(ଶୂନ୍ୟ)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cnt = 0;
	ସଂରଚନା rev_info rev;

	/* Check how many files differ. */
	repo_init_revisions(the_repository, &rev, "");
	setup_revisions(0, ଖାଲି, &rev, ଖାଲି);
	rev.diffopt.output_format |=
		DIFF_FORMAT_CALLBACK;
	rev.diffopt.format_callback = ଗଣନା_diff_files;
	rev.diffopt.format_callback_data = &cnt;
	run_diff_files(&rev, 0);

	/*
	 * Check how many unmerged entries are
	 * there.
	 */
	cnt += ଗଣନା_unmerged_entries();

	release_revisions(&rev);
	ଫେରନ୍ତୁ cnt;
}

/*
 * Pretend as if the user told us to merge with the remote-tracking
 * branch we have for the upstream of the current branch
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା setup_with_upstream(ଅଚଳ ଅକ୍ଷର ***argv)
{
	ସଂରଚନା branch *branch = branch_get(ଖାଲି);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	ଅଚଳ ଅକ୍ଷର **args;

	ଯଦି (!branch)
		die(_("No current branch."));
	ଯଦି (!branch->remote_name)
		die(_("No remote for the current branch."));
	ଯଦି (!branch->merge_nr)
		die(_("No default upstream defined for the current branch."));

	args = xcalloc(st_add(branch->merge_nr, 1), ଆକାର(ଅକ୍ଷର *));
	ପାଇଁ (i = 0; i < branch->merge_nr; i++) {
		ଯଦି (!branch->merge[i]->dst)
			die(_("No remote-tracking branch for %s from %s"),
			    branch->merge[i]->src, branch->remote_name);
		args[i] = branch->merge[i]->dst;
	}
	args[i] = ଖାଲି;
	*argv = args;
	ଫେରନ୍ତୁ i;
}

ସ୍ଥିର ଶୂନ୍ୟ write_merge_heads(ସଂରଚନା commit_list *remoteheads)
{
	ସଂରଚନା commit_list *j;
	ସଂରଚନା strbuf buf = STRBUF_INIT;

	ପାଇଁ (j = remoteheads; j; j = j->next) {
		ସଂରଚନା object_id *oid;
		ସଂରଚନା commit *c = j->item;
		ସଂରଚନା merge_remote_desc *desc;

		desc = merge_remote_util(c);
		ଯଦି (desc && desc->obj) {
			oid = &desc->obj->oid;
		} ଅନ୍ୟଥା {
			oid = &c->object.oid;
		}
		strbuf_addf(&buf, "%s\n", oid_to_hex(oid));
	}
	write_file_buf(git_path_merge_head(the_repository), buf.buf, buf.len);

	strbuf_reset(&buf);
	ଯଦି (fast_forward == FF_NO)
		strbuf_addstr(&buf, "no-ff");
	write_file_buf(git_path_merge_mode(the_repository), buf.buf, buf.len);
	strbuf_release(&buf);
}

ସ୍ଥିର ଶୂନ୍ୟ write_merge_state(ସଂରଚନା commit_list *remoteheads)
{
	write_merge_heads(remoteheads);
	strbuf_addch(&merge_ସନ୍ଦେଶ, '\n');
	write_file_buf(git_path_merge_ସନ୍ଦେଶ(the_repository), merge_ସନ୍ଦେଶ.buf,
		       merge_ସନ୍ଦେଶ.len);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା default_edit_option(ଶୂନ୍ୟ)
{
	ସ୍ଥିର ଅଚଳ ଅକ୍ଷର name[] = "GIT_MERGE_AUTOEDIT";
	ଅଚଳ ଅକ୍ଷର *e = getenv(name);
	ସଂରଚନା stat st_stdin, st_stdout;

	ଯଦି (have_message)
		/* an explicit -m msg without --[no-]edit */
		ଫେରନ୍ତୁ 0;

	ଯଦି (e) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା v = git_parse_maybe_bool(e);
		ଯଦି (v < 0)
			die(_("Bad value '%s' in environment '%s'"), e, name);
		ଫେରନ୍ତୁ v;
	}

	/* Use editor if stdin and stdout are the same and is a tty */
	ଫେରନ୍ତୁ (!fstat(0, &st_stdin) &&
		!fstat(1, &st_stdout) &&
		isatty(0) && isatty(1) &&
		st_stdin.st_dev == st_stdout.st_dev &&
		st_stdin.st_ino == st_stdout.st_ino &&
		st_stdin.st_mode == st_stdout.st_mode);
}

ସ୍ଥିର ସଂରଚନା commit_list *reduce_parents(ସଂରଚନା commit *head_commit,
					  ପୂର୍ଣ୍ଣସଂଖ୍ୟା *head_subsumed,
					  ସଂରଚନା commit_list *remoteheads)
{
	ସଂରଚନା commit_list *parents, **remotes;

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

	remoteheads = ଖାଲି;
	remotes = &remoteheads;
	ଯେତେବେଳେ (parents) {
		ସଂରଚନା commit *commit = କାଢ଼_commit(&parents);
		ଯଦି (commit == head_commit)
			*head_subsumed = 0;
		ଅନ୍ୟଥା
			remotes = &commit_list_ଭର୍ତ୍ତି(commit, remotes)->next;
	}
	ଫେରନ୍ତୁ remoteheads;
}

ସ୍ଥିର ଶୂନ୍ୟ prepare_merge_message(ସଂରଚନା strbuf *merge_names, ସଂରଚନା strbuf *merge_ସନ୍ଦେଶ)
{
	ସଂରଚନା fmt_merge_ସନ୍ଦେଶ_opts opts;

	memset(&opts, 0, ଆକାର(opts));
	opts.add_title = !have_message;
	opts.shortlog_len = shortlog_len;
	opts.credit_people = (0 < option_edit);
	opts.into_name = into_name;

	fmt_merge_ସନ୍ଦେଶ(merge_names, merge_ସନ୍ଦେଶ, &opts);
	ଯଦି (merge_ସନ୍ଦେଶ->len)
		strbuf_setlen(merge_ସନ୍ଦେଶ, merge_ସନ୍ଦେଶ->len - 1);
}

ସ୍ଥିର ଶୂନ୍ୟ handle_fetch_head(ସଂରଚନା commit_list **remotes, ସଂରଚନା strbuf *merge_names)
{
	ଅଚଳ ଅକ୍ଷର *filename;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, pos, npos;
	ସଂରଚନା strbuf fetch_head_file = STRBUF_INIT;
	ଅଚଳ unsigned hexsz = the_hash_algo->hexsz;

	ଯଦି (!merge_names)
		merge_names = &fetch_head_file;

	filename = git_path_fetch_head(the_repository);
	fd = xopen(filename, O_RDONLY);

	ଯଦି (strbuf_read(merge_names, fd, 0) < 0)
		die_errno(_("could not read '%s'"), filename);
	ଯଦି (close(fd) < 0)
		die_errno(_("could not close '%s'"), filename);

	ପାଇଁ (pos = 0; pos < merge_names->len; pos = npos) {
		ସଂରଚନା object_id oid;
		ଅକ୍ଷର *ptr;
		ସଂରଚନା commit *commit;

		ptr = strchr(merge_names->buf + pos, '\n');
		ଯଦି (ptr)
			npos = ptr - merge_names->buf + 1;
		ଅନ୍ୟଥା
			npos = merge_names->len;

		ଯଦି (npos - pos < hexsz + 2 ||
		    get_oid_hex(merge_names->buf + pos, &oid))
			commit = ଖାଲି; /* bad */
		ଅନ୍ୟଥା ଯଦି (memcmp(merge_names->buf + pos + hexsz, "\t\t", 2))
			ଜାରି; /* not-for-merge */
		ଅନ୍ୟଥା {
			ଅକ୍ଷର saved = merge_names->buf[pos + hexsz];
			merge_names->buf[pos + hexsz] = '\0';
			commit = get_merge_parent(merge_names->buf + pos);
			merge_names->buf[pos + hexsz] = saved;
		}
		ଯଦି (!commit) {
			ଯଦି (ptr)
				*ptr = '\0';
			die(_("not something we can merge in %s: %s"),
			    filename, merge_names->buf + pos);
		}
		remotes = &commit_list_ଭର୍ତ୍ତି(commit, remotes)->next;
	}

	ଯଦି (merge_names == &fetch_head_file)
		strbuf_release(&fetch_head_file);
}

ସ୍ଥିର ସଂରଚନା commit_list *collect_parents(ସଂରଚନା commit *head_commit,
					   ପୂର୍ଣ୍ଣସଂଖ୍ୟା *head_subsumed,
					   ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv,
					   ସଂରଚନା strbuf *merge_ସନ୍ଦେଶ)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	ସଂରଚନା commit_list *remoteheads = ଖାଲି;
	ସଂରଚନା commit_list **remotes = &remoteheads;
	ସଂରଚନା strbuf merge_names = STRBUF_INIT, *autogen = ଖାଲି;

	ଯଦି (merge_ସନ୍ଦେଶ && (!have_message || shortlog_len))
		autogen = &merge_names;

	ଯଦି (head_commit)
		remotes = &commit_list_ଭର୍ତ୍ତି(head_commit, remotes)->next;

	ଯଦି (argc == 1 && !strcmp(argv[0], "FETCH_HEAD")) {
		handle_fetch_head(remotes, autogen);
		remoteheads = reduce_parents(head_commit, head_subsumed, remoteheads);
	} ଅନ୍ୟଥା {
		ପାଇଁ (i = 0; i < argc; i++) {
			ସଂରଚନା commit *commit = get_merge_parent(argv[i]);
			ଯଦି (!commit)
				help_unknown_ref(argv[i], "merge",
						 _("not something we can merge"));
			remotes = &commit_list_ଭର୍ତ୍ତି(commit, remotes)->next;
		}
		remoteheads = reduce_parents(head_commit, head_subsumed, remoteheads);
		ଯଦି (autogen) {
			ସଂରଚନା commit_list *p;
			ପାଇଁ (p = remoteheads; p; p = p->next)
				merge_name(merge_remote_util(p->item)->name, autogen);
		}
	}

	ଯଦି (autogen) {
		prepare_merge_message(autogen, merge_ସନ୍ଦେଶ);
		strbuf_release(autogen);
	}

	ଫେରନ୍ତୁ remoteheads;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା merging_a_throwaway_tag(ସଂରଚନା commit *commit)
{
	ଅକ୍ଷର *tag_ref;
	ସଂରଚନା object_id oid;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା is_throwaway_tag = 0;

	/* Are we merging a tag? */
	ଯଦି (!merge_remote_util(commit) ||
	    !merge_remote_util(commit)->obj ||
	    merge_remote_util(commit)->obj->type != OBJ_TAG)
		ଫେରନ୍ତୁ is_throwaway_tag;

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
			  ((ସଂରଚନା tag *)merge_remote_util(commit)->obj)->tag);
	ଯଦି (!refs_read_ref(get_main_ref_store(the_repository), tag_ref, &oid) &&
	    oideq(&oid, &merge_remote_util(commit)->obj->oid))
		is_throwaway_tag = 0;
	ଅନ୍ୟଥା
		is_throwaway_tag = 1;
	ମୁକ୍ତ(tag_ref);
	ଫେରନ୍ତୁ is_throwaway_tag;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_merge(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
	      ଅଚଳ ଅକ୍ଷର **argv,
	      ଅଚଳ ଅକ୍ଷର *prefix,
	      ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା object_id result_ବୃକ୍ଷ, stash, head_oid;
	ସଂରଚନା commit *head_commit;
	ସଂରଚନା strbuf buf = STRBUF_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, ret = 0, head_subsumed;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା best_cnt = -1, merge_was_ok = 0, automerge_was_ok = 0;
	ସଂରଚନା commit_list *common = ଖାଲି;
	ଅଚଳ ଅକ୍ଷର *best_strategy = ଖାଲି, *wt_strategy = ଖାଲି;
	ସଂରଚନା commit_list *remoteheads = ଖାଲି, *p;
	ଶୂନ୍ୟ *branch_to_free, *argv_to_free = ଖାଲି;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା orig_argc = argc;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା merge_log_config = -1;

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
						      ଖାଲି);
	ଯଦି (branch)
		skip_prefix(branch, "refs/heads/", &branch);

	init_diff_ui_defaults();
	repo_config(the_repository, git_merge_config, &merge_log_config);

	ଯଦି (!branch || is_null_oid(&head_oid))
		head_commit = ଖାଲି;
	ଅନ୍ୟଥା
		head_commit = lookup_commit_or_die(&head_oid, "HEAD");

	ଯଦି (branch_mergeoptions)
		parse_branch_merge_options(branch_mergeoptions);
	argc = parse_options(argc, argv, prefix, builtin_merge_options,
			builtin_merge_usage, 0);
	ଯଦି (shortlog_len < 0)
		shortlog_len = (merge_log_config > 0) ? merge_log_config : 0;

	ଯଦି (verbosity < 0 && show_progress == -1)
		show_progress = 0;

	ଯଦି (abort_current_merge) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା nargc = 2;
		ଅଚଳ ଅକ୍ଷର *nargv[] = {"reset", "--merge", ଖାଲି};
		ଅକ୍ଷର stash_oid_hex[GIT_MAX_HEXSZ + 1];
		ସଂରଚନା object_id stash_oid = {0};

		ଯଦି (orig_argc != 2)
			usage_ସନ୍ଦେଶ_opt(_("--abort expects no arguments"),
			      builtin_merge_usage, builtin_merge_options);

		ଯଦି (!file_exists(git_path_merge_head(the_repository)))
			die(_("There is no merge to abort (MERGE_HEAD missing)."));

		ଯଦି (!refs_read_ref(get_main_ref_store(the_repository), "MERGE_AUTOSTASH", &stash_oid))
			refs_delete_ref(get_main_ref_store(the_repository),
					"", "MERGE_AUTOSTASH", &stash_oid,
					REF_NO_DEREF);

		/* Invoke 'git reset --merge' */
		ret = cmd_reset(nargc, nargv, prefix, the_repository);

		ଯଦି (!is_null_oid(&stash_oid)) {
			oid_to_hex_r(stash_oid_hex, &stash_oid);
			apply_autostash_oid(stash_oid_hex);
		}

		ଯାଅ done;
	}

	ଯଦି (quit_current_merge) {
		ଯଦି (orig_argc != 2)
			usage_ସନ୍ଦେଶ_opt(_("--quit expects no arguments"),
				      builtin_merge_usage,
				      builtin_merge_options);

		remove_merge_branch_state(the_repository);
		ଯାଅ done;
	}

	ଯଦି (continue_current_merge) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା nargc = 1;
		ଅଚଳ ଅକ୍ଷର *nargv[] = {"commit", ଖାଲି};

		ଯଦି (orig_argc != 2)
			usage_ସନ୍ଦେଶ_opt(_("--continue expects no arguments"),
			      builtin_merge_usage, builtin_merge_options);

		ଯଦି (!file_exists(git_path_merge_head(the_repository)))
			die(_("There is no merge in progress (MERGE_HEAD missing)."));

		/* Invoke 'git commit' */
		ret = cmd_commit(nargc, nargv, prefix, the_repository);
		ଯାଅ done;
	}

	ଯଦି (repo_read_index_unmerged(the_repository))
		die_resolve_conflict("merge");

	ଯଦି (file_exists(git_path_merge_head(the_repository))) {
		/*
		 * There is no unmerged entry, don't advise 'git
		 * add/rm <file>', just 'git commit'.
		 */
		ଯଦି (advice_enabled(ADVICE_RESOLVE_CONFLICT))
			die(_("You have not concluded your merge (MERGE_HEAD exists).\n"
				  "Please, commit your changes before you merge."));
		ଅନ୍ୟଥା
			die(_("You have not concluded your merge (MERGE_HEAD exists)."));
	}
	ଯଦି (refs_ref_exists(get_main_ref_store(the_repository), "CHERRY_PICK_HEAD")) {
		ଯଦି (advice_enabled(ADVICE_RESOLVE_CONFLICT))
			die(_("You have not concluded your cherry-pick (CHERRY_PICK_HEAD exists).\n"
			    "Please, commit your changes before you merge."));
		ଅନ୍ୟଥା
			die(_("You have not concluded your cherry-pick (CHERRY_PICK_HEAD exists)."));
	}
	resolve_undo_clear_index(the_repository->index);

	ଯଦି (option_edit < 0)
		option_edit = default_edit_option();

	cleanup_mode = get_cleanup_mode(cleanup_arg, 0 < option_edit);

	ଯଦି (verbosity < 0)
		show_diffstat = 0;

	ଯଦି (squash) {
		ଯଦି (fast_forward == FF_NO)
			die(_("options '%s' and '%s' cannot be used together"), "--squash", "--no-ff.");
		ଯଦି (option_commit > 0)
			die(_("options '%s' and '%s' cannot be used together"), "--squash", "--commit.");
		/*
		 * squash can now silently disable option_commit - this is not
		 * a problem as it is only overriding the default, not a user
		 * supplied option.
		 */
		option_commit = 0;
	}

	ଯଦି (option_commit < 0)
		option_commit = 1;

	ଯଦି (!argc) {
		ଯଦି (default_to_upstream) {
			argc = setup_with_upstream(&argv);
			argv_to_free = argv;
		}
		ଅନ୍ୟଥା
			die(_("No commit specified and merge.defaultToUpstream not set."));
	} ଅନ୍ୟଥା ଯଦି (argc == 1 && !strcmp(argv[0], "-")) {
		argv[0] = "@{-1}";
	}

	ଯଦି (!argc)
		usage_with_options(builtin_merge_usage,
			builtin_merge_options);

	ଯଦି (!head_commit) {
		/*
		 * If the merged head is a valid one there is no reason
		 * to forbid "git merge" into a branch yet to be born.
		 * We do the same for "git pull".
		 */
		ସଂରଚନା object_id *remote_head_oid;
		ଯଦି (squash)
			die(_("Squash commit into empty head not supported yet"));
		ଯଦି (fast_forward == FF_NO)
			die(_("Non-fast-forward commit does not make sense into "
			    "an empty head"));
		remoteheads = collect_parents(head_commit, &head_subsumed,
					      argc, argv, ଖାଲି);
		ଯଦି (!remoteheads)
			die(_("%s - not something we can merge"), argv[0]);
		ଯଦି (remoteheads->next)
			die(_("Can merge only exactly one commit into empty head"));

		ଯଦି (verify_signatures)
			verify_merge_signature(remoteheads->item, verbosity,
					       check_trust_level);

		remote_head_oid = &remoteheads->item->object.oid;
		read_empty(remote_head_oid);
		refs_update_ref(get_main_ref_store(the_repository),
				"initial pull", "HEAD", remote_head_oid, ଖାଲି,
				0,
				UPDATE_REFS_DIE_ON_ERR);
		ଯାଅ done;
	}

	/*
	 * All the rest are the commits being merged; prepare
	 * the standard merge summary message to be appended
	 * to the given message.
	 */
	remoteheads = collect_parents(head_commit, &head_subsumed,
				      argc, argv, &merge_ସନ୍ଦେଶ);

	ଯଦି (!head_commit || !argc)
		usage_with_options(builtin_merge_usage,
			builtin_merge_options);

	ଯଦି (verify_signatures) {
		ପାଇଁ (p = remoteheads; p; p = p->next) {
			verify_merge_signature(p->item, verbosity,
					       check_trust_level);
		}
	}

	strbuf_addstr(&buf, "merge");
	ପାଇଁ (p = remoteheads; p; p = p->next)
		strbuf_addf(&buf, " %s", merge_remote_util(p->item)->name);
	setenv("GIT_REFLOG_ACTION", buf.buf, 0);
	strbuf_reset(&buf);

	ପାଇଁ (p = remoteheads; p; p = p->next) {
		ସଂରଚନା commit *commit = p->item;
		strbuf_addf(&buf, "GITHEAD_%s",
			    oid_to_hex(&commit->object.oid));
		setenv(buf.buf, merge_remote_util(commit)->name, 1);
		strbuf_reset(&buf);
		ଯଦି (fast_forward != FF_ONLY && merging_a_throwaway_tag(commit))
			fast_forward = FF_NO;
	}

	ଯଦି (!use_strategies) {
		ଯଦି (!remoteheads)
			; /* already up-to-date */
		ଅନ୍ୟଥା ଯଦି (!remoteheads->next)
			add_strategies(pull_twohead, DEFAULT_TWOHEAD);
		ଅନ୍ୟଥା
			add_strategies(pull_octopus, DEFAULT_OCTOPUS);
	}

	ପାଇଁ (i = 0; i < use_strategies_nr; i++) {
		ଯଦି (use_strategies[i]->attr & NO_FAST_FORWARD)
			fast_forward = FF_NO;
		ଯଦି (use_strategies[i]->attr & NO_TRIVIAL)
			allow_trivial = 0;
	}

	ଯଦି (!remoteheads)
		; /* already up-to-date */
	ଅନ୍ୟଥା ଯଦି (!remoteheads->next) {
		ଯଦି (repo_get_merge_bases(the_repository, head_commit,
					 remoteheads->item, &common) < 0) {
			ret = 2;
			ଯାଅ done;
		}
	} ଅନ୍ୟଥା {
		ସଂରଚନା commit_list *list = remoteheads;
		commit_list_ଭର୍ତ୍ତି(head_commit, &list);
		ଯଦି (get_octopus_merge_bases(list, &common) < 0) {
			ମୁକ୍ତ(list);
			ret = 2;
			ଯାଅ done;
		}
		ମୁକ୍ତ(list);
	}

	refs_update_ref(get_main_ref_store(the_repository),
			"updating ORIG_HEAD", "ORIG_HEAD",
			&head_commit->object.oid, ଖାଲି, 0,
			UPDATE_REFS_DIE_ON_ERR);

	ଯଦି (remoteheads && !common) {
		/* No common ancestors found. */
		ଯଦି (!allow_unrelated_histories)
			die(_("refusing to merge unrelated histories"));
		/* otherwise, we need a real merge. */
	} ଅନ୍ୟଥା ଯଦି (!remoteheads ||
		 (!remoteheads->next && !common->next &&
		  common->item == remoteheads->item)) {
		/*
		 * If head can reach all the merge then we are up to date.
		 * but first the most common case of merging one remote.
		 */
		finish_up_to_date();
		ଯାଅ done;
	} ଅନ୍ୟଥା ଯଦି (fast_forward != FF_NO && !remoteheads->next &&
			!common->next &&
			oideq(&common->item->object.oid, &head_commit->object.oid)) {
		/* Again the most common case of merging one remote. */
		ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ = have_message ?
			"Fast-forward (no commit created; -m option ignored)" :
			"Fast-forward";
		ସଂରଚନା commit *commit;

		ଯଦି (verbosity >= 0) {
			printf(_("Updating %s..%s\n"),
			       repo_find_unique_abbrev(the_repository, &head_commit->object.oid,
						       DEFAULT_ABBREV),
			       repo_find_unique_abbrev(the_repository, &remoteheads->item->object.oid,
						       DEFAULT_ABBREV));
		}
		commit = remoteheads->item;
		ଯଦି (!commit) {
			ret = 1;
			ଯାଅ done;
		}

		ଯଦି (autostash)
			create_autostash_ref(the_repository, "MERGE_AUTOSTASH",
					     ଖାଲି, false);
		ଯଦି (checkout_fast_forward(the_repository,
					  &head_commit->object.oid,
					  &commit->object.oid,
					  overwrite_ignore)) {
			apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
					    ଖାଲି, ଖାଲି, ଖାଲି, ଖାଲି);
			ret = 1;
			ଯାଅ done;
		}

		finish(head_commit, remoteheads, &commit->object.oid, ସନ୍ଦେଶ);
		remove_merge_branch_state(the_repository);
		ଯାଅ done;
	} ଅନ୍ୟଥା ଯଦି (!remoteheads->next && common->next)
		;
		/*
		 * We are not doing octopus and not fast-forward.  Need
		 * a real merge.
		 */
	ଅନ୍ୟଥା ଯଦି (!remoteheads->next && !common->next && option_commit) {
		/*
		 * We are not doing octopus, not fast-forward, and have
		 * only one common.
		 */
		refresh_index(the_repository->index, REFRESH_QUIET, ଖାଲି, ଖାଲି, ଖାଲି);
		ଯଦି (allow_trivial && fast_forward != FF_ONLY) {
			/*
			 * Must first ensure that index matches HEAD before
			 * attempting a trivial merge.
			 */
			ସଂରଚନା ବୃକ୍ଷ *head_ବୃକ୍ଷ = repo_get_commit_ବୃକ୍ଷ(the_repository,
								      head_commit);
			ସଂରଚନା strbuf sb = STRBUF_INIT;

			ଯଦି (repo_index_has_changes(the_repository, head_ବୃକ୍ଷ,
						   &sb)) {
				error(_("Your local changes to the following files would be overwritten by merge:\n  %s"),
				      sb.buf);
				strbuf_release(&sb);
				ret = 2;
				ଯାଅ done;
			}

			/* See if it is really trivial. */
			git_committer_info(IDENT_STRICT);
			printf(_("Trying really trivial in-index merge...\n"));
			ଯଦି (!read_ବୃକ୍ଷ_trivial(&common->item->object.oid,
					       &head_commit->object.oid,
					       &remoteheads->item->object.oid)) {
				ret = merge_trivial(head_commit, remoteheads);
				ଯାଅ done;
			}
			printf(_("Nope.\n"));
		}
	} ଅନ୍ୟଥା {
		/*
		 * An octopus.  If we can reach all the remote we are up
		 * to date.
		 */
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା up_to_date = 1;
		ସଂରଚନା commit_list *j;

		ପାଇଁ (j = remoteheads; j; j = j->next) {
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = repo_in_merge_bases(the_repository,
						      j->item, head_commit);
			ଯଦି (ret < 0)
				exit(128);
			ଯଦି (!ret) {
				up_to_date = 0;
				ଭାଙ୍ଗ;
			}
		}
		ଯଦି (up_to_date) {
			finish_up_to_date();
			ଯାଅ done;
		}
	}

	ଯଦି (fast_forward == FF_ONLY)
		die_ff_impossible();

	ଯଦି (autostash)
		create_autostash_ref(the_repository, "MERGE_AUTOSTASH",
				     ଖାଲି, false);

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
	ଯଦି (save_state(&stash))
		oidclr(&stash, the_repository->hash_algo);

	ପାଇଁ (i = 0; i < use_strategies_nr; i++) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret, cnt;
		ଯଦି (i) {
			printf(_("Rewinding the tree to pristine...\n"));
			restore_state(&head_commit->object.oid, &stash);
		}
		ଯଦି (use_strategies_nr != 1)
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
		ଯଦି (ret < 2) {
			ଯଦି (!ret) {
				/*
				 * This strategy worked; no point in trying
				 * another.
				 */
				merge_was_ok = 1;
				best_strategy = wt_strategy;
				ଭାଙ୍ଗ;
			}
			cnt = (use_strategies_nr > 1) ? evaluate_result() : 0;
			ଯଦି (best_cnt <= 0 || cnt <= best_cnt) {
				best_strategy = wt_strategy;
				best_cnt = cnt;
			}
		}
	}

	/*
	 * If we have a resulting tree, that means the strategy module
	 * auto resolved the merge cleanly.
	 */
	ଯଦି (merge_was_ok && option_commit) {
		automerge_was_ok = 1;
		ret = finish_automerge(head_commit, head_subsumed,
				       common, remoteheads,
				       &result_ବୃକ୍ଷ, wt_strategy);
		ଯାଅ done;
	}

	/*
	 * Pick the result from the best strategy and have the user fix
	 * it up.
	 */
	ଯଦି (!best_strategy) {
		restore_state(&head_commit->object.oid, &stash);
		ଯଦି (use_strategies_nr > 1)
			fprintf(stderr,
				_("No merge strategy handled the merge.\n"));
		ଅନ୍ୟଥା
			fprintf(stderr, _("Merge with strategy %s failed.\n"),
				use_strategies[0]->name);
		apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
				    ଖାଲି, ଖାଲି, ଖାଲି, ଖାଲି);
		ret = 2;
		ଯାଅ done;
	} ଅନ୍ୟଥା ଯଦି (best_strategy == wt_strategy)
		; /* We already have its result in the working tree. */
	ଅନ୍ୟଥା {
		printf(_("Rewinding the tree to pristine...\n"));
		restore_state(&head_commit->object.oid, &stash);
		printf(_("Using the %s strategy to prepare resolving by hand.\n"),
			best_strategy);
		try_merge_strategy(best_strategy, common, remoteheads,
				   head_commit);
	}

	ଯଦି (squash) {
		finish(head_commit, remoteheads, ଖାଲି, ଖାଲି);

		git_test_write_commit_graph_or_die(the_repository);
	} ଅନ୍ୟଥା
		write_merge_state(remoteheads);

	ଯଦି (merge_was_ok)
		fprintf(stderr, _("Automatic merge went well; "
			"stopped before committing as requested\n"));
	ଅନ୍ୟଥା
		ret = suggest_conflicts();
	ଯଦି (autostash)
		printf(_("When finished, apply stashed changes with `git stash pop`\n"));

done:
	ଯଦି (!automerge_was_ok) {
		commit_list_free(common);
		commit_list_free(remoteheads);
	}
	strbuf_release(&buf);
	ମୁକ୍ତ(branch_to_free);
	ମୁକ୍ତ(argv_to_free);
	ମୁକ୍ତ(pull_twohead);
	ମୁକ୍ତ(pull_octopus);
	discard_index(the_repository->index);
	ଫେରନ୍ତୁ ret;
}
