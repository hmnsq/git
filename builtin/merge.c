/*
 * Builtin "git merge"
 *
 * Copyright (c) 2008 Miklos Vajna <vmiklos@frugalware.org>
 *
 * Based on git-merge.sh by Junio C Hamano.
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#अन्तर्भाव "builtin.h"

#अन्तर्भाव "abspath.h"
#अन्तर्भाव "advice.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "editor.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "object-name.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "lockfile.h"
#अन्तर्भाव "repository.h"
#अन्तर्भाव "run-command.h"
#अन्तर्भाव "hook.h"
#अन्तर्भाव "diff.h"
#अन्तर्भाव "diff-merges.h"
#अन्तर्भाव "refs.h"
#अन्तर्भाव "refspec.h"
#अन्तर्भाव "commit.h"
#अन्तर्भाव "diffcore.h"
#अन्तर्भाव "path.h"
#अन्तर्भाव "revision.h"
#अन्तर्भाव "unpack-trees.h"
#अन्तर्भाव "cache-tree.h"
#अन्तर्भाव "dir.h"
#अन्तर्भाव "color.h"
#अन्तर्भाव "rerere.h"
#अन्तर्भाव "help.h"
#अन्तर्भाव "merge.h"
#अन्तर्भाव "merge-ort-wrappers.h"
#अन्तर्भाव "resolve-undo.h"
#अन्तर्भाव "remote.h"
#अन्तर्भाव "fmt-merge-msg.h"
#अन्तर्भाव "sequencer.h"
#अन्तर्भाव "string-list.h"
#अन्तर्भाव "tag.h"
#अन्तर्भाव "alias.h"
#अन्तर्भाव "branch.h"
#अन्तर्भाव "commit-reach.h"
#अन्तर्भाव "wt-status.h"
#अन्तर्भाव "commit-graph.h"

#define DEFAULT_TWOHEAD (1<<0)
#define DEFAULT_OCTOPUS (1<<1)
#define NO_FAST_FORWARD (1<<2)
#define NO_TRIVIAL      (1<<3)

संरचना strategy {
	अचर वर्ण *name;
	अचिन्हित attr;
};

स्थिर अचर वर्ण * अचर builtin_merge_usage[] = {
	N_("git merge [<options>] [<commit>...]"),
	"git merge --abort",
	"git merge --continue",
	रिक्त
};

#define MERGE_SHOW_DIFFSTAT 1
#define MERGE_SHOW_COMPACTSUMMARY 2

स्थिर पूर्णाङ्क show_diffstat = MERGE_SHOW_DIFFSTAT, shortlog_len = -1, squash;
स्थिर पूर्णाङ्क option_commit = -1;
स्थिर पूर्णाङ्क option_edit = -1;
स्थिर पूर्णाङ्क allow_trivial = 1, have_message, verify_signatures;
स्थिर पूर्णाङ्क check_trust_level = 1;
स्थिर पूर्णाङ्क overwrite_ignore = 1;
स्थिर संरचना strbuf merge_सन्देश = STRBUF_INIT;
स्थिर संरचना strategy **use_strategies;
स्थिर माप_प्रकार use_strategies_nr, use_strategies_alloc;
स्थिर संरचना strvec xopts = STRVEC_INIT;
स्थिर अचर वर्ण *branch;
स्थिर वर्ण *branch_mergeoptions;
स्थिर पूर्णाङ्क verbosity;
स्थिर पूर्णाङ्क allow_rerere_auto;
स्थिर पूर्णाङ्क abort_current_merge;
स्थिर पूर्णाङ्क quit_current_merge;
स्थिर पूर्णाङ्क continue_current_merge;
स्थिर पूर्णाङ्क allow_unrelated_histories;
स्थिर पूर्णाङ्क show_progress = -1;
स्थिर पूर्णाङ्क default_to_upstream = 1;
स्थिर पूर्णाङ्क signoff;
स्थिर अचर वर्ण *sign_commit;
स्थिर पूर्णाङ्क autostash;
स्थिर पूर्णाङ्क no_verify;
स्थिर वर्ण *into_name;

स्थिर संरचना strategy all_strategy[] = {
	{ "recursive",  NO_TRIVIAL },
	{ "octopus",    DEFAULT_OCTOPUS },
	{ "ort",        DEFAULT_TWOHEAD | NO_TRIVIAL },
	{ "resolve",    0 },
	{ "ours",       NO_FAST_FORWARD | NO_TRIVIAL },
	{ "subtree",    NO_FAST_FORWARD | NO_TRIVIAL },
};

स्थिर वर्ण *pull_twohead, *pull_octopus;

गणना ff_type {
	FF_NO,
	FF_ALLOW,
	FF_ONLY
};

स्थिर गणना ff_type fast_forward = FF_ALLOW;

स्थिर वर्ण *cleanup_arg;
स्थिर गणना commit_सन्देश_cleanup_mode cleanup_mode;

स्थिर पूर्णाङ्क option_parse_message(अचर संरचना option *opt,
				अचर वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना strbuf *buf = opt->मान;

	यदि (unset)
		strbuf_setlen(buf, 0);
	अन्यथा यदि (arg) {
		strbuf_addf(buf, "%s%s", buf->len ? "\n\n" : "", arg);
		have_message = 1;
	} अन्यथा
		फर्काऊ error(_("switch `m' requires a value"));
	फर्काऊ 0;
}

स्थिर गणना parse_opt_result option_read_message(संरचना parse_opt_ctx_t *ctx,
						 अचर संरचना option *opt,
						 अचर वर्ण *arg_not_used,
						 पूर्णाङ्क unset)
{
	संरचना strbuf *buf = opt->मान;
	अचर वर्ण *arg;

	BUG_ON_OPT_ARG(arg_not_used);
	यदि (unset)
		BUG("-F cannot be negated");

	यदि (ctx->opt) {
		arg = ctx->opt;
		ctx->opt = रिक्त;
	} अन्यथा यदि (ctx->argc > 1) {
		ctx->argc--;
		arg = *++ctx->argv;
	} अन्यथा
		फर्काऊ error(_("option `%s' requires a value"), opt->long_name);

	यदि (buf->len)
		strbuf_addch(buf, '\n');
	यदि (ctx->prefix && !is_absolute_path(arg))
		arg = prefix_filename(ctx->prefix, arg);
	यदि (strbuf_read_file(buf, arg, 0) < 0)
		फर्काऊ error(_("could not read file '%s'"), arg);
	have_message = 1;

	फर्काऊ 0;
}

स्थिर संरचना strategy *get_strategy(अचर वर्ण *name)
{
	पूर्णाङ्क i;
	संरचना strategy *ret;
	स्थिर संरचना cmdnames main_cmds = {0}, other_cmds = {0};
	स्थिर पूर्णाङ्क loaded;
	वर्ण *default_strategy = रिक्त;

	यदि (!name)
		फर्काऊ रिक्त;

	यदि (default_strategy &&
	    !म_भेद(default_strategy, "ort") &&
	    !म_भेद(name, "recursive")) {
		name = "ort";
	}

	लागि (i = 0; i < ARRAY_SIZE(all_strategy); i++)
		यदि (!म_भेद(name, all_strategy[i].name))
			फर्काऊ &all_strategy[i];

	यदि (!loaded) {
		संरचना cmdnames not_strategies = {0};
		loaded = 1;

		load_command_list("git-merge-", &main_cmds, &other_cmds);
		लागि (i = 0; i < main_cmds.cnt; i++) {
			पूर्णाङ्क j, found = 0;
			संरचना cmdname *ent = main_cmds.names[i];
			लागि (j = 0; !found && j < ARRAY_SIZE(all_strategy); j++)
				यदि (!xstrncmpz(all_strategy[j].name, ent->name, ent->len))
					found = 1;
			यदि (!found)
				add_cmdname(&not_strategies, ent->name, ent->len);
		}
		exclude_cmds(&main_cmds, &not_strategies);

		cmdnames_release(&not_strategies);
	}
	यदि (!is_in_cmdlist(&main_cmds, name) && !is_in_cmdlist(&other_cmds, name)) {
		ख_लिखो(मानक_त्रुटि, _("Could not find merge strategy '%s'.\n"), name);
		ख_लिखो(मानक_त्रुटि, _("Available strategies are:"));
		लागि (i = 0; i < main_cmds.cnt; i++)
			ख_लिखो(मानक_त्रुटि, " %s", main_cmds.names[i]->name);
		ख_लिखो(मानक_त्रुटि, ".\n");
		यदि (other_cmds.cnt) {
			ख_लिखो(मानक_त्रुटि, _("Available custom strategies are:"));
			लागि (i = 0; i < other_cmds.cnt; i++)
				ख_लिखो(मानक_त्रुटि, " %s", other_cmds.names[i]->name);
			ख_लिखो(मानक_त्रुटि, ".\n");
		}
		निकास(1);
	}

	CALLOC_ARRAY(ret, 1);
	ret->name = xstrdup(name);
	ret->attr = NO_TRIVIAL;

	cmdnames_release(&main_cmds);
	cmdnames_release(&other_cmds);
	फर्काऊ ret;
}

स्थिर शून्य append_strategy(संरचना strategy *s)
{
	ALLOC_GROW(use_strategies, use_strategies_nr + 1, use_strategies_alloc);
	use_strategies[use_strategies_nr++] = s;
}

स्थिर पूर्णाङ्क option_parse_strategy(अचर संरचना option *opt UNUSED,
				 अचर वर्ण *name, पूर्णाङ्क unset)
{
	यदि (unset)
		फर्काऊ 0;

	append_strategy(get_strategy(name));
	फर्काऊ 0;
}

स्थिर पूर्णाङ्क option_parse_compact_summary(अचर संरचना option *opt,
					अचर वर्ण *name UNUSED, पूर्णाङ्क unset)
{
	पूर्णाङ्क *setting = opt->मान;

	यदि (unset)
		*setting = 0;
	अन्यथा
		*setting = MERGE_SHOW_COMPACTSUMMARY;
	फर्काऊ 0;
}

स्थिर संरचना option builtin_merge_options[] = {
	OPT_SET_INT('n', रिक्त, &show_diffstat,
		N_("do not show a diffstat at the end of the merge"), 0),
	OPT_BOOL(0, "stat", &show_diffstat,
		N_("show a diffstat at the end of the merge")),
	OPT_BOOL(0, "summary", &show_diffstat, N_("(synonym to --stat)")),
	OPT_CALLBACK_F(0, "compact-summary", &show_diffstat, रिक्त,
		       N_("show a compact-summary at the end of the merge"),
		       PARSE_OPT_NOARG,
		       option_parse_compact_summary),
	{
		.type = OPTION_INTEGER,
		.long_name = "log",
		.मान = &shortlog_len,
		.precision = परिमाण(shortlog_len),
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
	OPT_CALLBACK('s', "strategy", रिक्त, N_("strategy"),
		N_("merge strategy to use"), option_parse_strategy),
	OPT_STRVEC('X', "strategy-option", &xopts, N_("option=value"),
		N_("option for selected merge strategy")),
	OPT_CALLBACK('m', "message", &merge_सन्देश, N_("message"),
		N_("merge commit message (for a non-fast-forward merge)"),
		option_parse_message),
	{
		.type = OPTION_LOWLEVEL_CALLBACK,
		.short_name = 'F',
		.long_name = "file",
		.मान = &merge_सन्देश,
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
		.मान = &sign_commit,
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

स्थिर पूर्णाङ्क save_state(संरचना object_id *stash)
{
	पूर्णाङ्क len;
	संरचना child_process cp = CHILD_PROCESS_INIT;
	संरचना strbuf buffer = STRBUF_INIT;
	संरचना lock_file lock_file = LOCK_INIT;
	पूर्णाङ्क fd;
	पूर्णाङ्क rc = -1;

	fd = repo_hold_locked_index(the_repository, &lock_file, 0);
	refresh_index(the_repository->index, REFRESH_QUIET, रिक्त, रिक्त, रिक्त);
	यदि (0 <= fd)
		repo_update_index_if_able(the_repository, &lock_file);
	rollback_lock_file(&lock_file);

	strvec_pushl(&cp.args, "stash", "create", रिक्त);
	cp.out = -1;
	cp.git_cmd = 1;

	यदि (start_command(&cp))
		die(_("could not run stash."));
	len = strbuf_read(&buffer, cp.out, 1024);
	बंद(cp.out);

	यदि (finish_command(&cp) || len < 0)
		die(_("stash failed"));
	अन्यथा यदि (!len)		/* no changes */
		गच्छ out;
	strbuf_setlen(&buffer, buffer.len-1);
	यदि (repo_get_oid(the_repository, buffer.buf, stash))
		die(_("not a valid object: %s"), buffer.buf);
	rc = 0;
out:
	strbuf_release(&buffer);
	फर्काऊ rc;
}

स्थिर शून्य read_empty(अचर संरचना object_id *oid)
{
	संरचना child_process cmd = CHILD_PROCESS_INIT;

	strvec_pushl(&cmd.args, "read-tree", "-m", "-u",
		     empty_रूख_oid_hex(the_repository->hash_algo),
		     oid_to_hex(oid), रिक्त);
	cmd.git_cmd = 1;

	यदि (run_command(&cmd))
		die(_("read-tree failed"));
}

स्थिर शून्य reset_hard(अचर संरचना object_id *oid)
{
	संरचना child_process cmd = CHILD_PROCESS_INIT;

	strvec_pushl(&cmd.args, "read-tree", "-v", "--reset", "-u",
		     oid_to_hex(oid), रिक्त);
	cmd.git_cmd = 1;

	यदि (run_command(&cmd))
		die(_("read-tree failed"));
}

स्थिर शून्य restore_state(अचर संरचना object_id *head,
			  अचर संरचना object_id *stash)
{
	संरचना child_process cmd = CHILD_PROCESS_INIT;

	reset_hard(head);

	यदि (is_null_oid(stash))
		गच्छ refresh_cache;

	strvec_pushl(&cmd.args, "stash", "apply", "--index", "--quiet", रिक्त);
	strvec_थप(&cmd.args, oid_to_hex(stash));

	/*
	 * It is OK to ignore error here, for example when there was
	 * nothing to restore.
	 */
	cmd.git_cmd = 1;
	run_command(&cmd);

refresh_cache:
	discard_index(the_repository->index);
	यदि (repo_read_index(the_repository) < 0)
		die(_("could not read index"));
}

/* This is called when no merge was necessary. */
स्थिर शून्य finish_up_to_date(शून्य)
{
	यदि (verbosity >= 0) {
		यदि (squash)
			माला_दो(_("Already up to date. (nothing to squash)"));
		अन्यथा
			माला_दो(_("Already up to date."));
	}
	remove_merge_branch_state(the_repository);
}

स्थिर शून्य squash_message(संरचना commit *commit, संरचना commit_list *remoteheads)
{
	संरचना rev_info rev;
	संरचना strbuf out = STRBUF_INIT;
	संरचना commit_list *j;
	संरचना pretty_print_context ctx = {0};

	म_लिखो(_("Squash commit -- not updating HEAD\n"));

	repo_init_revisions(the_repository, &rev, रिक्त);
	diff_merges_suppress(&rev);
	rev.commit_format = CMIT_FMT_MEDIUM;

	commit->object.flags |= UNINTERESTING;
	add_pending_object(&rev, &commit->object, रिक्त);

	लागि (j = remoteheads; j; j = j->next)
		add_pending_object(&rev, &j->item->object, रिक्त);

	setup_revisions(0, रिक्त, &rev, रिक्त);
	यदि (prepare_revision_walk(&rev))
		die(_("revision walk setup failed"));

	ctx.abbrev = rev.abbrev;
	ctx.date_mode = rev.date_mode;
	ctx.fmt = rev.commit_format;

	strbuf_addstr(&out, "Squashed commit of the following:\n");
	जबसम्म ((commit = get_revision(&rev)) != रिक्त) {
		strbuf_addch(&out, '\n');
		strbuf_addf(&out, "commit %s\n",
			oid_to_hex(&commit->object.oid));
		pretty_print_commit(&ctx, commit, &out);
	}
	write_file_buf(git_path_squash_सन्देश(the_repository), out.buf, out.len);
	strbuf_release(&out);
	release_revisions(&rev);
}

स्थिर शून्य finish(संरचना commit *head_commit,
		   संरचना commit_list *remoteheads,
		   अचर संरचना object_id *new_head, अचर वर्ण *सन्देश)
{
	संरचना strbuf reflog_message = STRBUF_INIT;
	अचर संरचना object_id *head = &head_commit->object.oid;

	यदि (!सन्देश)
		strbuf_addstr(&reflog_message, दो_पर्या("GIT_REFLOG_ACTION"));
	अन्यथा {
		यदि (verbosity >= 0)
			म_लिखो("%s\n", सन्देश);
		strbuf_addf(&reflog_message, "%s: %s",
			दो_पर्या("GIT_REFLOG_ACTION"), सन्देश);
	}
	यदि (squash) {
		squash_message(head_commit, remoteheads);
	} अन्यथा {
		यदि (verbosity >= 0 && !merge_सन्देश.len)
			म_लिखो(_("No merge message -- not updating HEAD\n"));
		अन्यथा {
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
	यदि (new_head && show_diffstat) {
		संरचना diff_options opts;
		repo_diff_setup(the_repository, &opts);
		init_diffstat_widths(&opts);

		विकल्प (show_diffstat) {
		पक्ष MERGE_SHOW_DIFFSTAT: /* 1 */
			opts.output_format |=
				DIFF_FORMAT_SUMMARY | DIFF_FORMAT_DIFFSTAT;
			रोक;
		पक्ष MERGE_SHOW_COMPACTSUMMARY: /* 2 */
			opts.output_format |= DIFF_FORMAT_DIFFSTAT;
			opts.flags.stat_with_summary = 1;
			रोक;
		स्वभाव:
			रोक;
		}
		opts.detect_rename = DIFF_DETECT_RENAME;
		diff_setup_done(&opts);
		diff_रूख_oid(head, new_head, "", &opts);
		diffcore_std(&opts);
		diff_flush(&opts);
	}

	/* Run a post-merge hook */
	run_hooks_l(the_repository, "post-merge", squash ? "1" : "0", रिक्त);

	यदि (new_head)
		apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
				    रिक्त, रिक्त, रिक्त, रिक्त);
	strbuf_release(&reflog_message);
}

/* Get the name for the merge commit's message. */
स्थिर शून्य merge_name(अचर वर्ण *remote, संरचना strbuf *सन्देश)
{
	संरचना commit *remote_head;
	संरचना object_id branch_head;
	संरचना strbuf bname = STRBUF_INIT;
	संरचना merge_remote_desc *desc;
	अचर वर्ण *ptr;
	वर्ण *found_ref = रिक्त;
	पूर्णाङ्क len, early;

	copy_branchname(the_repository, &bname, remote, 0);
	remote = bname.buf;

	oidclr(&branch_head, the_repository->hash_algo);
	remote_head = get_merge_parent(remote);
	यदि (!remote_head)
		die(_("'%s' does not point to a commit"), remote);

	यदि (repo_dwim_ref(the_repository, remote, म_माप(remote), &branch_head,
			  &found_ref, 0) > 0) {
		यदि (starts_with(found_ref, "refs/heads/")) {
			strbuf_addf(सन्देश, "%s\t\tbranch '%s' of .\n",
				    oid_to_hex(&branch_head), remote);
			गच्छ cleanup;
		}
		यदि (starts_with(found_ref, "refs/tags/")) {
			strbuf_addf(सन्देश, "%s\t\ttag '%s' of .\n",
				    oid_to_hex(&branch_head), remote);
			गच्छ cleanup;
		}
		यदि (starts_with(found_ref, "refs/remotes/")) {
			strbuf_addf(सन्देश, "%s\t\tremote-tracking branch '%s' of .\n",
				    oid_to_hex(&branch_head), remote);
			गच्छ cleanup;
		}
	}

	/* See if remote matches <name>^^^.. or <name>~<number> */
	लागि (len = 0, ptr = remote + म_माप(remote);
	     remote < ptr && ptr[-1] == '^';
	     ptr--)
		len++;
	यदि (len)
		early = 1;
	अन्यथा {
		early = 0;
		ptr = म_खोजप(remote, '~');
		यदि (ptr) {
			पूर्णाङ्क seen_nonzero = 0;

			len++; /* count ~ */
			जबसम्म (*++ptr && है_अंक(*ptr)) {
				seen_nonzero |= (*ptr != '0');
				len++;
			}
			यदि (*ptr)
				len = 0; /* not ...~<number> */
			अन्यथा यदि (seen_nonzero)
				early = 1;
			अन्यथा यदि (len == 1)
				early = 1; /* "name~" is "name~1"! */
		}
	}
	यदि (len) {
		संरचना strbuf truname = STRBUF_INIT;
		strbuf_addf(&truname, "refs/heads/%s", remote);
		strbuf_setlen(&truname, truname.len - len);
		यदि (refs_ref_exists(get_main_ref_store(the_repository), truname.buf)) {
			strbuf_addf(सन्देश,
				    "%s\t\tbranch '%s'%s of .\n",
				    oid_to_hex(&remote_head->object.oid),
				    truname.buf + 11,
				    (early ? " (early part)" : ""));
			strbuf_release(&truname);
			गच्छ cleanup;
		}
		strbuf_release(&truname);
	}

	desc = merge_remote_util(remote_head);
	यदि (desc && desc->obj && desc->obj->type == OBJ_TAG) {
		strbuf_addf(सन्देश, "%s\t\t%s '%s'\n",
			    oid_to_hex(&desc->obj->oid),
			    type_name(desc->obj->type),
			    remote);
		गच्छ cleanup;
	}

	strbuf_addf(सन्देश, "%s\t\tcommit '%s'\n",
		oid_to_hex(&remote_head->object.oid), remote);
cleanup:
	मुक्त(found_ref);
	strbuf_release(&bname);
}

स्थिर शून्य parse_branch_merge_options(वर्ण *bmo)
{
	अचर वर्ण **argv;
	पूर्णाङ्क argc;

	यदि (!bmo)
		फर्काऊ;
	argc = split_cmdline(bmo, &argv);
	यदि (argc < 0)
		die(_("Bad branch.%s.mergeoptions string: %s"), branch,
		    _(split_cmdline_strerror(argc)));
	REALLOC_ARRAY(argv, argc + 2);
	MOVE_ARRAY(argv + 1, argv, argc + 1);
	argc++;
	argv[0] = "branch.*.mergeoptions";
	parse_options(argc, argv, रिक्त, builtin_merge_options,
		      builtin_merge_usage, 0);
	मुक्त(argv);
}

स्थिर पूर्णाङ्क git_merge_config(अचर वर्ण *k, अचर वर्ण *v,
			    अचर संरचना config_context *ctx, शून्य *cb)
{
	पूर्णाङ्क status;
	अचर वर्ण *str;

	यदि (branch &&
	    skip_prefix(k, "branch.", &str) &&
	    skip_prefix(str, branch, &str) &&
	    !म_भेद(str, ".mergeoptions")) {
		मुक्त(branch_mergeoptions);
		branch_mergeoptions = xstrdup(v);
		फर्काऊ 0;
	}

	यदि (!म_भेद(k, "merge.diffstat") || !म_भेद(k, "merge.stat")) {
		पूर्णाङ्क val = git_parse_maybe_bool_text(v);
		विकल्प (val) {
		पक्ष 0:
			show_diffstat = 0;
			रोक;
		पक्ष 1:
			show_diffstat = MERGE_SHOW_DIFFSTAT;
			रोक;
		स्वभाव:
			यदि (!म_भेद(v, "compact"))
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
			अन्यथा
				/*
				 * A setting from a future?  It is not an
				 * error grave enough to fail the command.
				 * proceed using the default one.
				 */
				show_diffstat = MERGE_SHOW_DIFFSTAT;
			रोक;
		}
	} अन्यथा यदि (!म_भेद(k, "merge.verifysignatures")) {
		verify_signatures = git_config_bool(k, v);
	} अन्यथा यदि (!म_भेद(k, "pull.twohead")) {
		FREE_AND_NULL(pull_twohead);
		फर्काऊ git_config_string(&pull_twohead, k, v);
	} अन्यथा यदि (!म_भेद(k, "pull.octopus")) {
		FREE_AND_NULL(pull_octopus);
		फर्काऊ git_config_string(&pull_octopus, k, v);
	} अन्यथा यदि (!म_भेद(k, "commit.cleanup")) {
		फर्काऊ git_config_string(&cleanup_arg, k, v);
	} अन्यथा यदि (!म_भेद(k, "merge.ff")) {
		पूर्णाङ्क boolval = git_parse_maybe_bool(v);
		यदि (0 <= boolval) {
			fast_forward = boolval ? FF_ALLOW : FF_NO;
		} अन्यथा यदि (v && !म_भेद(v, "only")) {
			fast_forward = FF_ONLY;
		} /* do not barf on values from future versions of git */
		फर्काऊ 0;
	} अन्यथा यदि (!म_भेद(k, "merge.defaulttoupstream")) {
		default_to_upstream = git_config_bool(k, v);
		फर्काऊ 0;
	} अन्यथा यदि (!म_भेद(k, "commit.gpgsign")) {
		sign_commit = git_config_bool(k, v) ? "" : रिक्त;
		फर्काऊ 0;
	} अन्यथा यदि (!म_भेद(k, "gpg.mintrustlevel")) {
		check_trust_level = 0;
	} अन्यथा यदि (!म_भेद(k, "merge.autostash")) {
		autostash = git_config_bool(k, v);
		फर्काऊ 0;
	}

	status = fmt_merge_सन्देश_config(k, v, ctx, cb);
	यदि (status)
		फर्काऊ status;
	फर्काऊ git_diff_ui_config(k, v, ctx, cb);
}

स्थिर पूर्णाङ्क read_रूख_trivial(संरचना object_id *common, संरचना object_id *head,
			     संरचना object_id *one)
{
	पूर्णाङ्क i, nr_trees = 0;
	संरचना रूख *trees[MAX_UNPACK_TREES];
	संरचना रूख_desc t[MAX_UNPACK_TREES];
	संरचना unpack_trees_options opts;

	स_रखो(&opts, 0, परिमाण(opts));
	opts.head_idx = 2;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;
	opts.update = 1;
	opts.verbose_update = 1;
	opts.trivial_merges_only = 1;
	opts.merge = 1;
	opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
	trees[nr_trees] = repo_parse_रूख_indirect(the_repository, common);
	यदि (!trees[nr_trees++])
		फर्काऊ -1;
	trees[nr_trees] = repo_parse_रूख_indirect(the_repository, head);
	यदि (!trees[nr_trees++])
		फर्काऊ -1;
	trees[nr_trees] = repo_parse_रूख_indirect(the_repository, one);
	यदि (!trees[nr_trees++])
		फर्काऊ -1;
	opts.fn = threeway_merge;
	cache_रूख_free(&the_repository->index->cache_रूख);
	लागि (i = 0; i < nr_trees; i++) {
		repo_parse_रूख(the_repository, trees[i]);
		init_रूख_desc(t+i, &trees[i]->object.oid,
			       trees[i]->buffer, trees[i]->size);
	}
	यदि (unpack_trees(nr_trees, t, &opts))
		फर्काऊ -1;
	फर्काऊ 0;
}

स्थिर शून्य write_रूख_trivial(संरचना object_id *oid)
{
	यदि (write_index_as_रूख(oid, the_repository->index,
				repo_get_index_file(the_repository),
				0, रिक्त))
		die(_("git write-tree failed to write a tree"));
}

स्थिर पूर्णाङ्क try_merge_strategy(अचर वर्ण *strategy, संरचना commit_list *common,
			      संरचना commit_list *remoteheads,
			      संरचना commit *head)
{
	अचर वर्ण *head_arg = "HEAD";

	यदि (repo_refresh_and_write_index(the_repository, REFRESH_QUIET,
					 SKIP_IF_UNCHANGED, 0, रिक्त, रिक्त,
					 रिक्त) < 0)
		die(_("Unable to write index."));

	यदि (!म_भेद(strategy, "recursive") || !म_भेद(strategy, "subtree") ||
	    !म_भेद(strategy, "ort")) {
		संरचना lock_file lock = LOCK_INIT;
		पूर्णाङ्क clean, x;
		संरचना commit *result;
		संरचना commit_list *reversed = रिक्त;
		संरचना merge_options o;
		संरचना commit_list *j;

		यदि (remoteheads->next) {
			error(_("Not handling anything other than two heads merge."));
			फर्काऊ 2;
		}

		init_ui_merge_options(&o, the_repository);
		यदि (!म_भेद(strategy, "subtree"))
			o.subtree_shift = "";

		o.show_rename_progress =
			show_progress == -1 ? isatty(2) : show_progress;

		लागि (x = 0; x < xopts.nr; x++)
			यदि (parse_merge_opt(&o, xopts.v[x]))
				die(_("unknown strategy option: -X%s"), xopts.v[x]);

		o.branch1 = head_arg;
		o.branch2 = merge_remote_util(remoteheads->item)->name;

		लागि (j = common; j; j = j->next)
			commit_list_घुसाउ(j->item, &reversed);

		repo_hold_locked_index(the_repository, &lock,
				       LOCK_DIE_ON_ERROR);
		clean = merge_ort_recursive(&o, head, remoteheads->item,
					    reversed, &result);
		commit_list_free(reversed);
		strbuf_release(&o.obuf);

		यदि (clean < 0) {
			rollback_lock_file(&lock);
			फर्काऊ 2;
		}
		यदि (write_locked_index(the_repository->index, &lock,
				       COMMIT_LOCK | SKIP_IF_UNCHANGED))
			die(_("unable to write %s"), repo_get_index_file(the_repository));
		फर्काऊ clean ? 0 : 1;
	} अन्यथा {
		फर्काऊ try_merge_command(the_repository,
					 strategy, xopts.nr, xopts.v,
					 common, head_arg, remoteheads);
	}
}

स्थिर शून्य गन्ती_diff_files(संरचना diff_queue_struct *q,
			     संरचना diff_options *opt UNUSED, शून्य *data)
{
	पूर्णाङ्क *गन्ती = data;

	(*गन्ती) += q->nr;
}

स्थिर पूर्णाङ्क गन्ती_unmerged_entries(शून्य)
{
	पूर्णाङ्क i, ret = 0;

	लागि (i = 0; i < the_repository->index->cache_nr; i++)
		यदि (ce_stage(the_repository->index->cache[i]))
			ret++;

	फर्काऊ ret;
}

स्थिर शून्य add_strategies(अचर वर्ण *string, अचिन्हित attr)
{
	पूर्णाङ्क i;

	यदि (string) {
		संरचना string_list list = STRING_LIST_INIT_DUP;
		संरचना string_list_item *item;
		string_list_split(&list, string, " ", -1);
		for_each_string_list_item(item, &list)
			append_strategy(get_strategy(item->string));
		string_list_clear(&list, 0);
		फर्काऊ;
	}
	लागि (i = 0; i < ARRAY_SIZE(all_strategy); i++)
		यदि (all_strategy[i].attr & attr)
			append_strategy(&all_strategy[i]);

}

स्थिर शून्य read_merge_सन्देश(संरचना strbuf *सन्देश)
{
	अचर वर्ण *filename = git_path_merge_सन्देश(the_repository);
	strbuf_reset(सन्देश);
	यदि (strbuf_read_file(सन्देश, filename, 0) < 0)
		die_errno(_("Could not read from '%s'"), filename);
}

स्थिर शून्य write_merge_state(संरचना commit_list *);
स्थिर शून्य abort_commit(संरचना commit_list *remoteheads, अचर वर्ण *err_सन्देश)
{
	यदि (err_सन्देश)
		error("%s", err_सन्देश);
	ख_लिखो(मानक_त्रुटि,
		_("Not committing merge; use 'git commit' to complete the merge.\n"));
	write_merge_state(remoteheads);
	निकास(1);
}

स्थिर अचर वर्ण merge_editor_comment[] =
N_("Please enter a commit message to explain why this merge is necessary,\n"
   "especially if it merges an updated upstream into a topic branch.\n"
   "\n");

स्थिर अचर वर्ण scissors_editor_comment[] =
N_("An empty message aborts the commit.\n");

स्थिर अचर वर्ण no_scissors_editor_comment[] =
N_("Lines starting with '%s' will be ignored, and an empty message aborts\n"
   "the commit.\n");

स्थिर शून्य write_merge_heads(संरचना commit_list *);
स्थिर शून्य prepare_to_commit(संरचना commit_list *remoteheads)
{
	संरचना strbuf सन्देश = STRBUF_INIT;
	अचर वर्ण *index_file = repo_get_index_file(the_repository);

	यदि (!no_verify) {
		पूर्णाङ्क invoked_hook;

		यदि (run_commit_hook(0 < option_edit, index_file, &invoked_hook,
				    "pre-merge-commit", रिक्त))
			abort_commit(remoteheads, रिक्त);
		/*
		 * Re-read the index as pre-merge-commit hook could have updated it,
		 * and write it out as a tree.  We must do this before we invoke
		 * the editor and after we invoke run_status above.
		 */
		यदि (invoked_hook)
			discard_index(the_repository->index);
	}
	read_index_from(the_repository->index, index_file,
			repo_get_git_dir(the_repository));
	strbuf_addbuf(&सन्देश, &merge_सन्देश);
	यदि (squash)
		BUG("the control must not reach here under --squash");
	यदि (0 < option_edit) {
		strbuf_addch(&सन्देश, '\n');
		यदि (cleanup_mode == COMMIT_सन्देश_CLEANUP_SCISSORS) {
			wt_status_append_cut_पङ्क्ति(&सन्देश);
			strbuf_commented_addf(&सन्देश, comment_पङ्क्ति_str, "\n");
		}
		strbuf_commented_addf(&सन्देश, comment_पङ्क्ति_str,
				      _(merge_editor_comment));
		यदि (cleanup_mode == COMMIT_सन्देश_CLEANUP_SCISSORS)
			strbuf_commented_addf(&सन्देश, comment_पङ्क्ति_str,
					      _(scissors_editor_comment));
		अन्यथा
			strbuf_commented_addf(&सन्देश, comment_पङ्क्ति_str,
				_(no_scissors_editor_comment), comment_पङ्क्ति_str);
	}
	यदि (signoff)
		append_signoff(&सन्देश, ignored_log_message_bytes(सन्देश.buf, सन्देश.len), 0);
	write_merge_heads(remoteheads);
	write_file_buf(git_path_merge_सन्देश(the_repository), सन्देश.buf, सन्देश.len);
	यदि (run_commit_hook(0 < option_edit, repo_get_index_file(the_repository),
			    रिक्त, "prepare-commit-msg",
			    git_path_merge_सन्देश(the_repository), "merge", रिक्त))
		abort_commit(remoteheads, रिक्त);
	यदि (0 < option_edit) {
		यदि (launch_editor(git_path_merge_सन्देश(the_repository), रिक्त, रिक्त))
			abort_commit(remoteheads, रिक्त);
	}

	यदि (!no_verify && run_commit_hook(0 < option_edit, repo_get_index_file(the_repository),
					  रिक्त, "commit-msg",
					  git_path_merge_सन्देश(the_repository), रिक्त))
		abort_commit(remoteheads, रिक्त);

	read_merge_सन्देश(&सन्देश);
	cleanup_message(&सन्देश, cleanup_mode, 0);
	यदि (!सन्देश.len)
		abort_commit(remoteheads, _("Empty commit message."));
	strbuf_release(&merge_सन्देश);
	strbuf_addbuf(&merge_सन्देश, &सन्देश);
	strbuf_release(&सन्देश);
}

स्थिर पूर्णाङ्क merge_trivial(संरचना commit *head, संरचना commit_list *remoteheads)
{
	संरचना object_id result_रूख, result_commit;
	संरचना commit_list *parents = रिक्त, **pptr = &parents;

	यदि (repo_refresh_and_write_index(the_repository, REFRESH_QUIET,
					 SKIP_IF_UNCHANGED, 0, रिक्त, रिक्त,
					 रिक्त) < 0)
		फर्काऊ error(_("Unable to write index."));

	write_रूख_trivial(&result_रूख);
	म_लिखो(_("Wonderful.\n"));
	pptr = commit_list_append(head, pptr);
	pptr = commit_list_append(remoteheads->item, pptr);
	prepare_to_commit(remoteheads);
	यदि (commit_रूख(merge_सन्देश.buf, merge_सन्देश.len, &result_रूख, parents,
			&result_commit, रिक्त, sign_commit))
		die(_("failed to write commit object"));
	finish(head, remoteheads, &result_commit, "In-index merge");

	remove_merge_branch_state(the_repository);
	commit_list_free(parents);
	फर्काऊ 0;
}

स्थिर पूर्णाङ्क finish_automerge(संरचना commit *head,
			    पूर्णाङ्क head_subsumed,
			    संरचना commit_list *common,
			    संरचना commit_list *remoteheads,
			    संरचना object_id *result_रूख,
			    अचर वर्ण *wt_strategy)
{
	संरचना commit_list *parents = रिक्त;
	संरचना strbuf buf = STRBUF_INIT;
	संरचना object_id result_commit;

	write_रूख_trivial(result_रूख);
	commit_list_free(common);
	parents = remoteheads;
	यदि (!head_subsumed || fast_forward == FF_NO)
		commit_list_घुसाउ(head, &parents);
	prepare_to_commit(remoteheads);
	यदि (commit_रूख(merge_सन्देश.buf, merge_सन्देश.len, result_रूख, parents,
			&result_commit, रिक्त, sign_commit))
		die(_("failed to write commit object"));
	strbuf_addf(&buf, "Merge made by the '%s' strategy.", wt_strategy);
	finish(head, remoteheads, &result_commit, buf.buf);

	strbuf_release(&buf);
	remove_merge_branch_state(the_repository);
	commit_list_free(parents);
	फर्काऊ 0;
}

स्थिर पूर्णाङ्क suggest_conflicts(शून्य)
{
	अचर वर्ण *filename;
	खाता *fp;
	संरचना strbuf msgbuf = STRBUF_INIT;

	filename = git_path_merge_सन्देश(the_repository);
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
	ख_माला_दो(msgbuf.buf, fp);
	strbuf_release(&msgbuf);
	ख_बंद(fp);
	repo_rerere(the_repository, allow_rerere_auto);
	म_लिखो(_("Automatic merge failed; "
			"fix conflicts and then commit the result.\n"));
	फर्काऊ 1;
}

स्थिर पूर्णाङ्क evaluate_result(शून्य)
{
	पूर्णाङ्क cnt = 0;
	संरचना rev_info rev;

	/* Check how many files differ. */
	repo_init_revisions(the_repository, &rev, "");
	setup_revisions(0, रिक्त, &rev, रिक्त);
	rev.diffopt.output_format |=
		DIFF_FORMAT_CALLBACK;
	rev.diffopt.format_callback = गन्ती_diff_files;
	rev.diffopt.format_callback_data = &cnt;
	run_diff_files(&rev, 0);

	/*
	 * Check how many unmerged entries are
	 * there.
	 */
	cnt += गन्ती_unmerged_entries();

	release_revisions(&rev);
	फर्काऊ cnt;
}

/*
 * Pretend as if the user told us to merge with the remote-tracking
 * branch we have for the upstream of the current branch
 */
स्थिर पूर्णाङ्क setup_with_upstream(अचर वर्ण ***argv)
{
	संरचना branch *branch = branch_get(रिक्त);
	पूर्णाङ्क i;
	अचर वर्ण **args;

	यदि (!branch)
		die(_("No current branch."));
	यदि (!branch->remote_name)
		die(_("No remote for the current branch."));
	यदि (!branch->merge_nr)
		die(_("No default upstream defined for the current branch."));

	args = xcalloc(st_add(branch->merge_nr, 1), परिमाण(वर्ण *));
	लागि (i = 0; i < branch->merge_nr; i++) {
		यदि (!branch->merge[i]->dst)
			die(_("No remote-tracking branch for %s from %s"),
			    branch->merge[i]->src, branch->remote_name);
		args[i] = branch->merge[i]->dst;
	}
	args[i] = रिक्त;
	*argv = args;
	फर्काऊ i;
}

स्थिर शून्य write_merge_heads(संरचना commit_list *remoteheads)
{
	संरचना commit_list *j;
	संरचना strbuf buf = STRBUF_INIT;

	लागि (j = remoteheads; j; j = j->next) {
		संरचना object_id *oid;
		संरचना commit *c = j->item;
		संरचना merge_remote_desc *desc;

		desc = merge_remote_util(c);
		यदि (desc && desc->obj) {
			oid = &desc->obj->oid;
		} अन्यथा {
			oid = &c->object.oid;
		}
		strbuf_addf(&buf, "%s\n", oid_to_hex(oid));
	}
	write_file_buf(git_path_merge_head(the_repository), buf.buf, buf.len);

	strbuf_reset(&buf);
	यदि (fast_forward == FF_NO)
		strbuf_addstr(&buf, "no-ff");
	write_file_buf(git_path_merge_mode(the_repository), buf.buf, buf.len);
	strbuf_release(&buf);
}

स्थिर शून्य write_merge_state(संरचना commit_list *remoteheads)
{
	write_merge_heads(remoteheads);
	strbuf_addch(&merge_सन्देश, '\n');
	write_file_buf(git_path_merge_सन्देश(the_repository), merge_सन्देश.buf,
		       merge_सन्देश.len);
}

स्थिर पूर्णाङ्क default_edit_option(शून्य)
{
	स्थिर अचर वर्ण name[] = "GIT_MERGE_AUTOEDIT";
	अचर वर्ण *e = दो_पर्या(name);
	संरचना stat st_stdin, st_stdout;

	यदि (have_message)
		/* an explicit -m msg without --[no-]edit */
		फर्काऊ 0;

	यदि (e) {
		पूर्णाङ्क v = git_parse_maybe_bool(e);
		यदि (v < 0)
			die(_("Bad value '%s' in environment '%s'"), e, name);
		फर्काऊ v;
	}

	/* Use editor if stdin and stdout are the same and is a tty */
	फर्काऊ (!ख_स्थिति(0, &st_stdin) &&
		!ख_स्थिति(1, &st_stdout) &&
		isatty(0) && isatty(1) &&
		st_stdin.st_dev == st_stdout.st_dev &&
		st_stdin.st_ino == st_stdout.st_ino &&
		st_stdin.st_mode == st_stdout.st_mode);
}

स्थिर संरचना commit_list *reduce_parents(संरचना commit *head_commit,
					  पूर्णाङ्क *head_subsumed,
					  संरचना commit_list *remoteheads)
{
	संरचना commit_list *parents, **remotes;

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

	remoteheads = रिक्त;
	remotes = &remoteheads;
	जबसम्म (parents) {
		संरचना commit *commit = झिक_commit(&parents);
		यदि (commit == head_commit)
			*head_subsumed = 0;
		अन्यथा
			remotes = &commit_list_घुसाउ(commit, remotes)->next;
	}
	फर्काऊ remoteheads;
}

स्थिर शून्य prepare_merge_message(संरचना strbuf *merge_names, संरचना strbuf *merge_सन्देश)
{
	संरचना fmt_merge_सन्देश_opts opts;

	स_रखो(&opts, 0, परिमाण(opts));
	opts.add_title = !have_message;
	opts.shortlog_len = shortlog_len;
	opts.credit_people = (0 < option_edit);
	opts.into_name = into_name;

	fmt_merge_सन्देश(merge_names, merge_सन्देश, &opts);
	यदि (merge_सन्देश->len)
		strbuf_setlen(merge_सन्देश, merge_सन्देश->len - 1);
}

स्थिर शून्य handle_fetch_head(संरचना commit_list **remotes, संरचना strbuf *merge_names)
{
	अचर वर्ण *filename;
	पूर्णाङ्क fd, pos, npos;
	संरचना strbuf fetch_head_file = STRBUF_INIT;
	अचर अचिन्हित hexsz = the_hash_algo->hexsz;

	यदि (!merge_names)
		merge_names = &fetch_head_file;

	filename = git_path_fetch_head(the_repository);
	fd = xopen(filename, O_RDONLY);

	यदि (strbuf_read(merge_names, fd, 0) < 0)
		die_errno(_("could not read '%s'"), filename);
	यदि (बंद(fd) < 0)
		die_errno(_("could not close '%s'"), filename);

	लागि (pos = 0; pos < merge_names->len; pos = npos) {
		संरचना object_id oid;
		वर्ण *ptr;
		संरचना commit *commit;

		ptr = म_अक्षर(merge_names->buf + pos, '\n');
		यदि (ptr)
			npos = ptr - merge_names->buf + 1;
		अन्यथा
			npos = merge_names->len;

		यदि (npos - pos < hexsz + 2 ||
		    get_oid_hex(merge_names->buf + pos, &oid))
			commit = रिक्त; /* bad */
		अन्यथा यदि (स_भेद(merge_names->buf + pos + hexsz, "\t\t", 2))
			जारी; /* not-for-merge */
		अन्यथा {
			वर्ण saved = merge_names->buf[pos + hexsz];
			merge_names->buf[pos + hexsz] = '\0';
			commit = get_merge_parent(merge_names->buf + pos);
			merge_names->buf[pos + hexsz] = saved;
		}
		यदि (!commit) {
			यदि (ptr)
				*ptr = '\0';
			die(_("not something we can merge in %s: %s"),
			    filename, merge_names->buf + pos);
		}
		remotes = &commit_list_घुसाउ(commit, remotes)->next;
	}

	यदि (merge_names == &fetch_head_file)
		strbuf_release(&fetch_head_file);
}

स्थिर संरचना commit_list *collect_parents(संरचना commit *head_commit,
					   पूर्णाङ्क *head_subsumed,
					   पूर्णाङ्क argc, अचर वर्ण **argv,
					   संरचना strbuf *merge_सन्देश)
{
	पूर्णाङ्क i;
	संरचना commit_list *remoteheads = रिक्त;
	संरचना commit_list **remotes = &remoteheads;
	संरचना strbuf merge_names = STRBUF_INIT, *autogen = रिक्त;

	यदि (merge_सन्देश && (!have_message || shortlog_len))
		autogen = &merge_names;

	यदि (head_commit)
		remotes = &commit_list_घुसाउ(head_commit, remotes)->next;

	यदि (argc == 1 && !म_भेद(argv[0], "FETCH_HEAD")) {
		handle_fetch_head(remotes, autogen);
		remoteheads = reduce_parents(head_commit, head_subsumed, remoteheads);
	} अन्यथा {
		लागि (i = 0; i < argc; i++) {
			संरचना commit *commit = get_merge_parent(argv[i]);
			यदि (!commit)
				help_unknown_ref(argv[i], "merge",
						 _("not something we can merge"));
			remotes = &commit_list_घुसाउ(commit, remotes)->next;
		}
		remoteheads = reduce_parents(head_commit, head_subsumed, remoteheads);
		यदि (autogen) {
			संरचना commit_list *p;
			लागि (p = remoteheads; p; p = p->next)
				merge_name(merge_remote_util(p->item)->name, autogen);
		}
	}

	यदि (autogen) {
		prepare_merge_message(autogen, merge_सन्देश);
		strbuf_release(autogen);
	}

	फर्काऊ remoteheads;
}

स्थिर पूर्णाङ्क merging_a_throwaway_tag(संरचना commit *commit)
{
	वर्ण *tag_ref;
	संरचना object_id oid;
	पूर्णाङ्क is_throwaway_tag = 0;

	/* Are we merging a tag? */
	यदि (!merge_remote_util(commit) ||
	    !merge_remote_util(commit)->obj ||
	    merge_remote_util(commit)->obj->type != OBJ_TAG)
		फर्काऊ is_throwaway_tag;

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
			  ((संरचना tag *)merge_remote_util(commit)->obj)->tag);
	यदि (!refs_read_ref(get_main_ref_store(the_repository), tag_ref, &oid) &&
	    oideq(&oid, &merge_remote_util(commit)->obj->oid))
		is_throwaway_tag = 0;
	अन्यथा
		is_throwaway_tag = 1;
	मुक्त(tag_ref);
	फर्काऊ is_throwaway_tag;
}

पूर्णाङ्क cmd_merge(पूर्णाङ्क argc,
	      अचर वर्ण **argv,
	      अचर वर्ण *prefix,
	      संरचना repository *repo UNUSED)
{
	संरचना object_id result_रूख, stash, head_oid;
	संरचना commit *head_commit;
	संरचना strbuf buf = STRBUF_INIT;
	पूर्णाङ्क i, ret = 0, head_subsumed;
	पूर्णाङ्क best_cnt = -1, merge_was_ok = 0, automerge_was_ok = 0;
	संरचना commit_list *common = रिक्त;
	अचर वर्ण *best_strategy = रिक्त, *wt_strategy = रिक्त;
	संरचना commit_list *remoteheads = रिक्त, *p;
	शून्य *branch_to_free, *argv_to_free = रिक्त;
	पूर्णाङ्क orig_argc = argc;
	पूर्णाङ्क merge_log_config = -1;

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
						      रिक्त);
	यदि (branch)
		skip_prefix(branch, "refs/heads/", &branch);

	init_diff_ui_defaults();
	repo_config(the_repository, git_merge_config, &merge_log_config);

	यदि (!branch || is_null_oid(&head_oid))
		head_commit = रिक्त;
	अन्यथा
		head_commit = lookup_commit_or_die(&head_oid, "HEAD");

	यदि (branch_mergeoptions)
		parse_branch_merge_options(branch_mergeoptions);
	argc = parse_options(argc, argv, prefix, builtin_merge_options,
			builtin_merge_usage, 0);
	यदि (shortlog_len < 0)
		shortlog_len = (merge_log_config > 0) ? merge_log_config : 0;

	यदि (verbosity < 0 && show_progress == -1)
		show_progress = 0;

	यदि (abort_current_merge) {
		पूर्णाङ्क nargc = 2;
		अचर वर्ण *nargv[] = {"reset", "--merge", रिक्त};
		वर्ण stash_oid_hex[GIT_MAX_HEXSZ + 1];
		संरचना object_id stash_oid = {0};

		यदि (orig_argc != 2)
			usage_सन्देश_opt(_("--abort expects no arguments"),
			      builtin_merge_usage, builtin_merge_options);

		यदि (!file_exists(git_path_merge_head(the_repository)))
			die(_("There is no merge to abort (MERGE_HEAD missing)."));

		यदि (!refs_read_ref(get_main_ref_store(the_repository), "MERGE_AUTOSTASH", &stash_oid))
			refs_delete_ref(get_main_ref_store(the_repository),
					"", "MERGE_AUTOSTASH", &stash_oid,
					REF_NO_DEREF);

		/* Invoke 'git reset --merge' */
		ret = cmd_reset(nargc, nargv, prefix, the_repository);

		यदि (!is_null_oid(&stash_oid)) {
			oid_to_hex_r(stash_oid_hex, &stash_oid);
			apply_autostash_oid(stash_oid_hex);
		}

		गच्छ done;
	}

	यदि (quit_current_merge) {
		यदि (orig_argc != 2)
			usage_सन्देश_opt(_("--quit expects no arguments"),
				      builtin_merge_usage,
				      builtin_merge_options);

		remove_merge_branch_state(the_repository);
		गच्छ done;
	}

	यदि (continue_current_merge) {
		पूर्णाङ्क nargc = 1;
		अचर वर्ण *nargv[] = {"commit", रिक्त};

		यदि (orig_argc != 2)
			usage_सन्देश_opt(_("--continue expects no arguments"),
			      builtin_merge_usage, builtin_merge_options);

		यदि (!file_exists(git_path_merge_head(the_repository)))
			die(_("There is no merge in progress (MERGE_HEAD missing)."));

		/* Invoke 'git commit' */
		ret = cmd_commit(nargc, nargv, prefix, the_repository);
		गच्छ done;
	}

	यदि (repo_read_index_unmerged(the_repository))
		die_resolve_conflict("merge");

	यदि (file_exists(git_path_merge_head(the_repository))) {
		/*
		 * There is no unmerged entry, don't advise 'git
		 * add/rm <file>', just 'git commit'.
		 */
		यदि (advice_enabled(ADVICE_RESOLVE_CONFLICT))
			die(_("You have not concluded your merge (MERGE_HEAD exists).\n"
				  "Please, commit your changes before you merge."));
		अन्यथा
			die(_("You have not concluded your merge (MERGE_HEAD exists)."));
	}
	यदि (refs_ref_exists(get_main_ref_store(the_repository), "CHERRY_PICK_HEAD")) {
		यदि (advice_enabled(ADVICE_RESOLVE_CONFLICT))
			die(_("You have not concluded your cherry-pick (CHERRY_PICK_HEAD exists).\n"
			    "Please, commit your changes before you merge."));
		अन्यथा
			die(_("You have not concluded your cherry-pick (CHERRY_PICK_HEAD exists)."));
	}
	resolve_undo_clear_index(the_repository->index);

	यदि (option_edit < 0)
		option_edit = default_edit_option();

	cleanup_mode = get_cleanup_mode(cleanup_arg, 0 < option_edit);

	यदि (verbosity < 0)
		show_diffstat = 0;

	यदि (squash) {
		यदि (fast_forward == FF_NO)
			die(_("options '%s' and '%s' cannot be used together"), "--squash", "--no-ff.");
		यदि (option_commit > 0)
			die(_("options '%s' and '%s' cannot be used together"), "--squash", "--commit.");
		/*
		 * squash can now silently disable option_commit - this is not
		 * a problem as it is only overriding the default, not a user
		 * supplied option.
		 */
		option_commit = 0;
	}

	यदि (option_commit < 0)
		option_commit = 1;

	यदि (!argc) {
		यदि (default_to_upstream) {
			argc = setup_with_upstream(&argv);
			argv_to_free = argv;
		}
		अन्यथा
			die(_("No commit specified and merge.defaultToUpstream not set."));
	} अन्यथा यदि (argc == 1 && !म_भेद(argv[0], "-")) {
		argv[0] = "@{-1}";
	}

	यदि (!argc)
		usage_with_options(builtin_merge_usage,
			builtin_merge_options);

	यदि (!head_commit) {
		/*
		 * If the merged head is a valid one there is no reason
		 * to forbid "git merge" into a branch yet to be born.
		 * We do the same for "git pull".
		 */
		संरचना object_id *remote_head_oid;
		यदि (squash)
			die(_("Squash commit into empty head not supported yet"));
		यदि (fast_forward == FF_NO)
			die(_("Non-fast-forward commit does not make sense into "
			    "an empty head"));
		remoteheads = collect_parents(head_commit, &head_subsumed,
					      argc, argv, रिक्त);
		यदि (!remoteheads)
			die(_("%s - not something we can merge"), argv[0]);
		यदि (remoteheads->next)
			die(_("Can merge only exactly one commit into empty head"));

		यदि (verify_signatures)
			verify_merge_signature(remoteheads->item, verbosity,
					       check_trust_level);

		remote_head_oid = &remoteheads->item->object.oid;
		read_empty(remote_head_oid);
		refs_update_ref(get_main_ref_store(the_repository),
				"initial pull", "HEAD", remote_head_oid, रिक्त,
				0,
				UPDATE_REFS_DIE_ON_ERR);
		गच्छ done;
	}

	/*
	 * All the rest are the commits being merged; prepare
	 * the standard merge summary message to be appended
	 * to the given message.
	 */
	remoteheads = collect_parents(head_commit, &head_subsumed,
				      argc, argv, &merge_सन्देश);

	यदि (!head_commit || !argc)
		usage_with_options(builtin_merge_usage,
			builtin_merge_options);

	यदि (verify_signatures) {
		लागि (p = remoteheads; p; p = p->next) {
			verify_merge_signature(p->item, verbosity,
					       check_trust_level);
		}
	}

	strbuf_addstr(&buf, "merge");
	लागि (p = remoteheads; p; p = p->next)
		strbuf_addf(&buf, " %s", merge_remote_util(p->item)->name);
	setenv("GIT_REFLOG_ACTION", buf.buf, 0);
	strbuf_reset(&buf);

	लागि (p = remoteheads; p; p = p->next) {
		संरचना commit *commit = p->item;
		strbuf_addf(&buf, "GITHEAD_%s",
			    oid_to_hex(&commit->object.oid));
		setenv(buf.buf, merge_remote_util(commit)->name, 1);
		strbuf_reset(&buf);
		यदि (fast_forward != FF_ONLY && merging_a_throwaway_tag(commit))
			fast_forward = FF_NO;
	}

	यदि (!use_strategies) {
		यदि (!remoteheads)
			; /* already up-to-date */
		अन्यथा यदि (!remoteheads->next)
			add_strategies(pull_twohead, DEFAULT_TWOHEAD);
		अन्यथा
			add_strategies(pull_octopus, DEFAULT_OCTOPUS);
	}

	लागि (i = 0; i < use_strategies_nr; i++) {
		यदि (use_strategies[i]->attr & NO_FAST_FORWARD)
			fast_forward = FF_NO;
		यदि (use_strategies[i]->attr & NO_TRIVIAL)
			allow_trivial = 0;
	}

	यदि (!remoteheads)
		; /* already up-to-date */
	अन्यथा यदि (!remoteheads->next) {
		यदि (repo_get_merge_bases(the_repository, head_commit,
					 remoteheads->item, &common) < 0) {
			ret = 2;
			गच्छ done;
		}
	} अन्यथा {
		संरचना commit_list *list = remoteheads;
		commit_list_घुसाउ(head_commit, &list);
		यदि (get_octopus_merge_bases(list, &common) < 0) {
			मुक्त(list);
			ret = 2;
			गच्छ done;
		}
		मुक्त(list);
	}

	refs_update_ref(get_main_ref_store(the_repository),
			"updating ORIG_HEAD", "ORIG_HEAD",
			&head_commit->object.oid, रिक्त, 0,
			UPDATE_REFS_DIE_ON_ERR);

	यदि (remoteheads && !common) {
		/* No common ancestors found. */
		यदि (!allow_unrelated_histories)
			die(_("refusing to merge unrelated histories"));
		/* otherwise, we need a real merge. */
	} अन्यथा यदि (!remoteheads ||
		 (!remoteheads->next && !common->next &&
		  common->item == remoteheads->item)) {
		/*
		 * If head can reach all the merge then we are up to date.
		 * but first the most common case of merging one remote.
		 */
		finish_up_to_date();
		गच्छ done;
	} अन्यथा यदि (fast_forward != FF_NO && !remoteheads->next &&
			!common->next &&
			oideq(&common->item->object.oid, &head_commit->object.oid)) {
		/* Again the most common case of merging one remote. */
		अचर वर्ण *सन्देश = have_message ?
			"Fast-forward (no commit created; -m option ignored)" :
			"Fast-forward";
		संरचना commit *commit;

		यदि (verbosity >= 0) {
			म_लिखो(_("Updating %s..%s\n"),
			       repo_find_unique_abbrev(the_repository, &head_commit->object.oid,
						       DEFAULT_ABBREV),
			       repo_find_unique_abbrev(the_repository, &remoteheads->item->object.oid,
						       DEFAULT_ABBREV));
		}
		commit = remoteheads->item;
		यदि (!commit) {
			ret = 1;
			गच्छ done;
		}

		यदि (autostash)
			create_autostash_ref(the_repository, "MERGE_AUTOSTASH",
					     रिक्त, false);
		यदि (checkout_fast_forward(the_repository,
					  &head_commit->object.oid,
					  &commit->object.oid,
					  overwrite_ignore)) {
			apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
					    रिक्त, रिक्त, रिक्त, रिक्त);
			ret = 1;
			गच्छ done;
		}

		finish(head_commit, remoteheads, &commit->object.oid, सन्देश);
		remove_merge_branch_state(the_repository);
		गच्छ done;
	} अन्यथा यदि (!remoteheads->next && common->next)
		;
		/*
		 * We are not doing octopus and not fast-forward.  Need
		 * a real merge.
		 */
	अन्यथा यदि (!remoteheads->next && !common->next && option_commit) {
		/*
		 * We are not doing octopus, not fast-forward, and have
		 * only one common.
		 */
		refresh_index(the_repository->index, REFRESH_QUIET, रिक्त, रिक्त, रिक्त);
		यदि (allow_trivial && fast_forward != FF_ONLY) {
			/*
			 * Must first ensure that index matches HEAD before
			 * attempting a trivial merge.
			 */
			संरचना रूख *head_रूख = repo_get_commit_रूख(the_repository,
								      head_commit);
			संरचना strbuf sb = STRBUF_INIT;

			यदि (repo_index_has_changes(the_repository, head_रूख,
						   &sb)) {
				error(_("Your local changes to the following files would be overwritten by merge:\n  %s"),
				      sb.buf);
				strbuf_release(&sb);
				ret = 2;
				गच्छ done;
			}

			/* See if it is really trivial. */
			git_committer_info(IDENT_STRICT);
			म_लिखो(_("Trying really trivial in-index merge...\n"));
			यदि (!read_रूख_trivial(&common->item->object.oid,
					       &head_commit->object.oid,
					       &remoteheads->item->object.oid)) {
				ret = merge_trivial(head_commit, remoteheads);
				गच्छ done;
			}
			म_लिखो(_("Nope.\n"));
		}
	} अन्यथा {
		/*
		 * An octopus.  If we can reach all the remote we are up
		 * to date.
		 */
		पूर्णाङ्क up_to_date = 1;
		संरचना commit_list *j;

		लागि (j = remoteheads; j; j = j->next) {
			पूर्णाङ्क ret = repo_in_merge_bases(the_repository,
						      j->item, head_commit);
			यदि (ret < 0)
				निकास(128);
			यदि (!ret) {
				up_to_date = 0;
				रोक;
			}
		}
		यदि (up_to_date) {
			finish_up_to_date();
			गच्छ done;
		}
	}

	यदि (fast_forward == FF_ONLY)
		die_ff_impossible();

	यदि (autostash)
		create_autostash_ref(the_repository, "MERGE_AUTOSTASH",
				     रिक्त, false);

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
	यदि (save_state(&stash))
		oidclr(&stash, the_repository->hash_algo);

	लागि (i = 0; i < use_strategies_nr; i++) {
		पूर्णाङ्क ret, cnt;
		यदि (i) {
			म_लिखो(_("Rewinding the tree to pristine...\n"));
			restore_state(&head_commit->object.oid, &stash);
		}
		यदि (use_strategies_nr != 1)
			म_लिखो(_("Trying merge strategy %s...\n"),
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
		यदि (ret < 2) {
			यदि (!ret) {
				/*
				 * This strategy worked; no point in trying
				 * another.
				 */
				merge_was_ok = 1;
				best_strategy = wt_strategy;
				रोक;
			}
			cnt = (use_strategies_nr > 1) ? evaluate_result() : 0;
			यदि (best_cnt <= 0 || cnt <= best_cnt) {
				best_strategy = wt_strategy;
				best_cnt = cnt;
			}
		}
	}

	/*
	 * If we have a resulting tree, that means the strategy module
	 * auto resolved the merge cleanly.
	 */
	यदि (merge_was_ok && option_commit) {
		automerge_was_ok = 1;
		ret = finish_automerge(head_commit, head_subsumed,
				       common, remoteheads,
				       &result_रूख, wt_strategy);
		गच्छ done;
	}

	/*
	 * Pick the result from the best strategy and have the user fix
	 * it up.
	 */
	यदि (!best_strategy) {
		restore_state(&head_commit->object.oid, &stash);
		यदि (use_strategies_nr > 1)
			ख_लिखो(मानक_त्रुटि,
				_("No merge strategy handled the merge.\n"));
		अन्यथा
			ख_लिखो(मानक_त्रुटि, _("Merge with strategy %s failed.\n"),
				use_strategies[0]->name);
		apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
				    रिक्त, रिक्त, रिक्त, रिक्त);
		ret = 2;
		गच्छ done;
	} अन्यथा यदि (best_strategy == wt_strategy)
		; /* We already have its result in the working tree. */
	अन्यथा {
		म_लिखो(_("Rewinding the tree to pristine...\n"));
		restore_state(&head_commit->object.oid, &stash);
		म_लिखो(_("Using the %s strategy to prepare resolving by hand.\n"),
			best_strategy);
		try_merge_strategy(best_strategy, common, remoteheads,
				   head_commit);
	}

	यदि (squash) {
		finish(head_commit, remoteheads, रिक्त, रिक्त);

		git_test_write_commit_graph_or_die(the_repository);
	} अन्यथा
		write_merge_state(remoteheads);

	यदि (merge_was_ok)
		ख_लिखो(मानक_त्रुटि, _("Automatic merge went well; "
			"stopped before committing as requested\n"));
	अन्यथा
		ret = suggest_conflicts();
	यदि (autostash)
		म_लिखो(_("When finished, apply stashed changes with `git stash pop`\n"));

done:
	यदि (!automerge_was_ok) {
		commit_list_free(common);
		commit_list_free(remoteheads);
	}
	strbuf_release(&buf);
	मुक्त(branch_to_free);
	मुक्त(argv_to_free);
	मुक्त(pull_twohead);
	मुक्त(pull_octopus);
	discard_index(the_repository->index);
	फर्काऊ ret;
}
