/*
 * Builtin "git merge"
 *
 * Copyright (c) 2008 Miklos Vajna <vmiklos@frugalware.org>
 *
 * Based on git-merge.sh by Junio C Hamano.
 */

#घोषणा USE_THE_REPOSITORY_VARIABLE
#घोषणा DISABLE_SIGN_COMPARE_WARNINGS

#समावेश "builtin.h"

#समावेश "abspath.h"
#समावेश "advice.h"
#समावेश "config.h"
#समावेश "editor.h"
#समावेश "environment.h"
#समावेश "gettext.h"
#समावेश "hex.h"
#समावेश "object-name.h"
#समावेश "parse-options.h"
#समावेश "lockfile.h"
#समावेश "repository.h"
#समावेश "run-command.h"
#समावेश "hook.h"
#समावेश "diff.h"
#समावेश "diff-merges.h"
#समावेश "refs.h"
#समावेश "refspec.h"
#समावेश "commit.h"
#समावेश "diffcore.h"
#समावेश "path.h"
#समावेश "revision.h"
#समावेश "unpack-trees.h"
#समावेश "cache-tree.h"
#समावेश "dir.h"
#समावेश "color.h"
#समावेश "rerere.h"
#समावेश "help.h"
#समावेश "merge.h"
#समावेश "merge-ort-wrappers.h"
#समावेश "resolve-undo.h"
#समावेश "remote.h"
#समावेश "fmt-merge-msg.h"
#समावेश "sequencer.h"
#समावेश "string-list.h"
#समावेश "tag.h"
#समावेश "alias.h"
#समावेश "branch.h"
#समावेश "commit-reach.h"
#समावेश "wt-status.h"
#समावेश "commit-graph.h"

#घोषणा DEFAULT_TWOHEAD (1<<0)
#घोषणा DEFAULT_OCTOPUS (1<<1)
#घोषणा NO_FAST_FORWARD (1<<2)
#घोषणा NO_TRIVIAL      (1<<3)

काष्ठा strategy {
	स्थिर अक्षर *name;
	अचिन्हित attr;
};

अटल स्थिर अक्षर * स्थिर builtin_merge_usage[] = {
	N_("git merge [<options>] [<commit>...]"),
	"git merge --abort",
	"git merge --continue",
	शून्य
};

#घोषणा MERGE_SHOW_DIFFSTAT 1
#घोषणा MERGE_SHOW_COMPACTSUMMARY 2

अटल पूर्णांक show_diffstat = MERGE_SHOW_DIFFSTAT, shortlog_len = -1, squash;
अटल पूर्णांक option_commit = -1;
अटल पूर्णांक option_edit = -1;
अटल पूर्णांक allow_trivial = 1, have_message, verify_signatures;
अटल पूर्णांक check_trust_level = 1;
अटल पूर्णांक overwrite_ignore = 1;
अटल काष्ठा strbuf merge_संदेश = STRBUF_INIT;
अटल काष्ठा strategy **use_strategies;
अटल माप_प्रकार use_strategies_nr, use_strategies_alloc;
अटल काष्ठा strvec xopts = STRVEC_INIT;
अटल स्थिर अक्षर *branch;
अटल अक्षर *branch_mergeoptions;
अटल पूर्णांक verbosity;
अटल पूर्णांक allow_rerere_auto;
अटल पूर्णांक abort_current_merge;
अटल पूर्णांक quit_current_merge;
अटल पूर्णांक continue_current_merge;
अटल पूर्णांक allow_unrelated_histories;
अटल पूर्णांक show_progress = -1;
अटल पूर्णांक default_to_upstream = 1;
अटल पूर्णांक signoff;
अटल स्थिर अक्षर *sign_commit;
अटल पूर्णांक autostash;
अटल पूर्णांक no_verify;
अटल अक्षर *into_name;

अटल काष्ठा strategy all_strategy[] = {
	{ "recursive",  NO_TRIVIAL },
	{ "octopus",    DEFAULT_OCTOPUS },
	{ "ort",        DEFAULT_TWOHEAD | NO_TRIVIAL },
	{ "resolve",    0 },
	{ "ours",       NO_FAST_FORWARD | NO_TRIVIAL },
	{ "subtree",    NO_FAST_FORWARD | NO_TRIVIAL },
};

अटल अक्षर *pull_twohead, *pull_octopus;

क्रमागत ff_type {
	FF_NO,
	FF_ALLOW,
	FF_ONLY
};

अटल क्रमागत ff_type fast_forward = FF_ALLOW;

अटल अक्षर *cleanup_arg;
अटल क्रमागत commit_संदेश_cleanup_mode cleanup_mode;

अटल पूर्णांक option_parse_message(स्थिर काष्ठा option *opt,
				स्थिर अक्षर *arg, पूर्णांक unset)
{
	काष्ठा strbuf *buf = opt->मूल्य;

	अगर (unset)
		strbuf_setlen(buf, 0);
	अन्यथा अगर (arg) {
		strbuf_addf(buf, "%s%s", buf->len ? "\n\n" : "", arg);
		have_message = 1;
	} अन्यथा
		वापस error(_("switch `m' requires a value"));
	वापस 0;
}

अटल क्रमागत parse_opt_परिणाम option_read_message(काष्ठा parse_opt_ctx_t *ctx,
						 स्थिर काष्ठा option *opt,
						 स्थिर अक्षर *arg_not_used,
						 पूर्णांक unset)
{
	काष्ठा strbuf *buf = opt->मूल्य;
	स्थिर अक्षर *arg;

	BUG_ON_OPT_ARG(arg_not_used);
	अगर (unset)
		BUG("-F cannot be negated");

	अगर (ctx->opt) {
		arg = ctx->opt;
		ctx->opt = शून्य;
	} अन्यथा अगर (ctx->argc > 1) {
		ctx->argc--;
		arg = *++ctx->argv;
	} अन्यथा
		वापस error(_("option `%s' requires a value"), opt->long_name);

	अगर (buf->len)
		strbuf_addch(buf, '\n');
	अगर (ctx->prefix && !is_absolute_path(arg))
		arg = prefix_filename(ctx->prefix, arg);
	अगर (strbuf_read_file(buf, arg, 0) < 0)
		वापस error(_("could not read file '%s'"), arg);
	have_message = 1;

	वापस 0;
}

अटल काष्ठा strategy *get_strategy(स्थिर अक्षर *name)
{
	पूर्णांक i;
	काष्ठा strategy *ret;
	अटल काष्ठा cmdnames main_cmds = {0}, other_cmds = {0};
	अटल पूर्णांक loaded;
	अक्षर *default_strategy = शून्य;

	अगर (!name)
		वापस शून्य;

	अगर (default_strategy &&
	    !म_भेद(default_strategy, "ort") &&
	    !म_भेद(name, "recursive")) {
		name = "ort";
	}

	क्रम (i = 0; i < ARRAY_आकार(all_strategy); i++)
		अगर (!म_भेद(name, all_strategy[i].name))
			वापस &all_strategy[i];

	अगर (!loaded) {
		काष्ठा cmdnames not_strategies = {0};
		loaded = 1;

		load_command_list("git-merge-", &main_cmds, &other_cmds);
		क्रम (i = 0; i < main_cmds.cnt; i++) {
			पूर्णांक j, found = 0;
			काष्ठा cmdname *ent = main_cmds.names[i];
			क्रम (j = 0; !found && j < ARRAY_आकार(all_strategy); j++)
				अगर (!xstrncmpz(all_strategy[j].name, ent->name, ent->len))
					found = 1;
			अगर (!found)
				add_cmdname(&not_strategies, ent->name, ent->len);
		}
		exclude_cmds(&main_cmds, &not_strategies);

		cmdnames_release(&not_strategies);
	}
	अगर (!is_in_cmdlist(&main_cmds, name) && !is_in_cmdlist(&other_cmds, name)) {
		ख_लिखो(मानक_त्रुटि, _("Could not find merge strategy '%s'.\n"), name);
		ख_लिखो(मानक_त्रुटि, _("Available strategies are:"));
		क्रम (i = 0; i < main_cmds.cnt; i++)
			ख_लिखो(मानक_त्रुटि, " %s", main_cmds.names[i]->name);
		ख_लिखो(मानक_त्रुटि, ".\n");
		अगर (other_cmds.cnt) {
			ख_लिखो(मानक_त्रुटि, _("Available custom strategies are:"));
			क्रम (i = 0; i < other_cmds.cnt; i++)
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
	वापस ret;
}

अटल व्योम append_strategy(काष्ठा strategy *s)
{
	ALLOC_GROW(use_strategies, use_strategies_nr + 1, use_strategies_alloc);
	use_strategies[use_strategies_nr++] = s;
}

अटल पूर्णांक option_parse_strategy(स्थिर काष्ठा option *opt UNUSED,
				 स्थिर अक्षर *name, पूर्णांक unset)
{
	अगर (unset)
		वापस 0;

	append_strategy(get_strategy(name));
	वापस 0;
}

अटल पूर्णांक option_parse_compact_summary(स्थिर काष्ठा option *opt,
					स्थिर अक्षर *name UNUSED, पूर्णांक unset)
{
	पूर्णांक *setting = opt->मूल्य;

	अगर (unset)
		*setting = 0;
	अन्यथा
		*setting = MERGE_SHOW_COMPACTSUMMARY;
	वापस 0;
}

अटल काष्ठा option builtin_merge_options[] = {
	OPT_SET_INT('n', शून्य, &show_diffstat,
		N_("do not show a diffstat at the end of the merge"), 0),
	OPT_BOOL(0, "stat", &show_diffstat,
		N_("show a diffstat at the end of the merge")),
	OPT_BOOL(0, "summary", &show_diffstat, N_("(synonym to --stat)")),
	OPT_CALLBACK_F(0, "compact-summary", &show_diffstat, शून्य,
		       N_("show a compact-summary at the end of the merge"),
		       PARSE_OPT_NOARG,
		       option_parse_compact_summary),
	{
		.type = OPTION_INTEGER,
		.long_name = "log",
		.मूल्य = &shortlog_len,
		.precision = माप(shortlog_len),
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
	OPT_CALLBACK('s', "strategy", शून्य, N_("strategy"),
		N_("merge strategy to use"), option_parse_strategy),
	OPT_STRVEC('X', "strategy-option", &xopts, N_("option=value"),
		N_("option for selected merge strategy")),
	OPT_CALLBACK('m', "message", &merge_संदेश, N_("message"),
		N_("merge commit message (for a non-fast-forward merge)"),
		option_parse_message),
	{
		.type = OPTION_LOWLEVEL_CALLBACK,
		.short_name = 'F',
		.long_name = "file",
		.मूल्य = &merge_संदेश,
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
		.मूल्य = &sign_commit,
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

अटल पूर्णांक save_state(काष्ठा object_id *stash)
{
	पूर्णांक len;
	काष्ठा child_process cp = CHILD_PROCESS_INIT;
	काष्ठा strbuf बफर = STRBUF_INIT;
	काष्ठा lock_file lock_file = LOCK_INIT;
	पूर्णांक fd;
	पूर्णांक rc = -1;

	fd = repo_hold_locked_सूचकांक(the_repository, &lock_file, 0);
	refresh_सूचकांक(the_repository->सूचकांक, REFRESH_QUIET, शून्य, शून्य, शून्य);
	अगर (0 <= fd)
		repo_update_सूचकांक_if_able(the_repository, &lock_file);
	rollback_lock_file(&lock_file);

	strvec_pushl(&cp.args, "stash", "create", शून्य);
	cp.out = -1;
	cp.git_cmd = 1;

	अगर (start_command(&cp))
		die(_("could not run stash."));
	len = strbuf_read(&बफर, cp.out, 1024);
	बंद(cp.out);

	अगर (finish_command(&cp) || len < 0)
		die(_("stash failed"));
	अन्यथा अगर (!len)		/* no changes */
		जाओ out;
	strbuf_setlen(&बफर, बफर.len-1);
	अगर (repo_get_oid(the_repository, बफर.buf, stash))
		die(_("not a valid object: %s"), बफर.buf);
	rc = 0;
out:
	strbuf_release(&बफर);
	वापस rc;
}

अटल व्योम read_empty(स्थिर काष्ठा object_id *oid)
{
	काष्ठा child_process cmd = CHILD_PROCESS_INIT;

	strvec_pushl(&cmd.args, "read-tree", "-m", "-u",
		     empty_वृक्ष_oid_hex(the_repository->hash_algo),
		     oid_to_hex(oid), शून्य);
	cmd.git_cmd = 1;

	अगर (run_command(&cmd))
		die(_("read-tree failed"));
}

अटल व्योम reset_hard(स्थिर काष्ठा object_id *oid)
{
	काष्ठा child_process cmd = CHILD_PROCESS_INIT;

	strvec_pushl(&cmd.args, "read-tree", "-v", "--reset", "-u",
		     oid_to_hex(oid), शून्य);
	cmd.git_cmd = 1;

	अगर (run_command(&cmd))
		die(_("read-tree failed"));
}

अटल व्योम restore_state(स्थिर काष्ठा object_id *शीर्ष,
			  स्थिर काष्ठा object_id *stash)
{
	काष्ठा child_process cmd = CHILD_PROCESS_INIT;

	reset_hard(शीर्ष);

	अगर (is_null_oid(stash))
		जाओ refresh_cache;

	strvec_pushl(&cmd.args, "stash", "apply", "--index", "--quiet", शून्य);
	strvec_धकेलो(&cmd.args, oid_to_hex(stash));

	/*
	 * It is OK to ignore error here, for example when there was
	 * nothing to restore.
	 */
	cmd.git_cmd = 1;
	run_command(&cmd);

refresh_cache:
	discard_सूचकांक(the_repository->सूचकांक);
	अगर (repo_read_सूचकांक(the_repository) < 0)
		die(_("could not read index"));
}

/* This is called when no merge was necessary. */
अटल व्योम finish_up_to_date(व्योम)
{
	अगर (verbosity >= 0) {
		अगर (squash)
			माला_दो(_("Already up to date. (nothing to squash)"));
		अन्यथा
			माला_दो(_("Already up to date."));
	}
	remove_merge_branch_state(the_repository);
}

अटल व्योम squash_message(काष्ठा commit *commit, काष्ठा commit_list *remoteheads)
{
	काष्ठा rev_info rev;
	काष्ठा strbuf out = STRBUF_INIT;
	काष्ठा commit_list *j;
	काष्ठा pretty_print_context ctx = {0};

	म_लिखो(_("Squash commit -- not updating HEAD\n"));

	repo_init_revisions(the_repository, &rev, शून्य);
	diff_merges_suppress(&rev);
	rev.commit_format = CMIT_FMT_MEDIUM;

	commit->object.flags |= UNINTERESTING;
	add_pending_object(&rev, &commit->object, शून्य);

	क्रम (j = remoteheads; j; j = j->अग्र)
		add_pending_object(&rev, &j->item->object, शून्य);

	setup_revisions(0, शून्य, &rev, शून्य);
	अगर (prepare_revision_walk(&rev))
		die(_("revision walk setup failed"));

	ctx.abbrev = rev.abbrev;
	ctx.date_mode = rev.date_mode;
	ctx.fmt = rev.commit_format;

	strbuf_addstr(&out, "Squashed commit of the following:\n");
	जबतक ((commit = get_revision(&rev)) != शून्य) {
		strbuf_addch(&out, '\n');
		strbuf_addf(&out, "commit %s\n",
			oid_to_hex(&commit->object.oid));
		pretty_print_commit(&ctx, commit, &out);
	}
	write_file_buf(git_path_squash_संदेश(the_repository), out.buf, out.len);
	strbuf_release(&out);
	release_revisions(&rev);
}

अटल व्योम finish(काष्ठा commit *शीर्ष_commit,
		   काष्ठा commit_list *remoteheads,
		   स्थिर काष्ठा object_id *new_शीर्ष, स्थिर अक्षर *संदेश)
{
	काष्ठा strbuf reflog_message = STRBUF_INIT;
	स्थिर काष्ठा object_id *शीर्ष = &शीर्ष_commit->object.oid;

	अगर (!संदेश)
		strbuf_addstr(&reflog_message, दो_पर्या("GIT_REFLOG_ACTION"));
	अन्यथा {
		अगर (verbosity >= 0)
			म_लिखो("%s\n", संदेश);
		strbuf_addf(&reflog_message, "%s: %s",
			दो_पर्या("GIT_REFLOG_ACTION"), संदेश);
	}
	अगर (squash) {
		squash_message(शीर्ष_commit, remoteheads);
	} अन्यथा {
		अगर (verbosity >= 0 && !merge_संदेश.len)
			म_लिखो(_("No merge message -- not updating HEAD\n"));
		अन्यथा {
			refs_update_ref(get_main_ref_store(the_repository),
					reflog_message.buf, "HEAD", new_शीर्ष,
					शीर्ष,
					0, UPDATE_REFS_DIE_ON_ERR);
			/*
			 * We ignore errors in 'gc --auto', since the
			 * user should see them.
			 */
			run_auto_maintenance(the_repository, verbosity < 0);
		}
	}
	अगर (new_शीर्ष && show_diffstat) {
		काष्ठा diff_options opts;
		repo_diff_setup(the_repository, &opts);
		init_diffstat_widths(&opts);

		चयन (show_diffstat) {
		हाल MERGE_SHOW_DIFFSTAT: /* 1 */
			opts.output_format |=
				DIFF_FORMAT_SUMMARY | DIFF_FORMAT_DIFFSTAT;
			अवरोध;
		हाल MERGE_SHOW_COMPACTSUMMARY: /* 2 */
			opts.output_format |= DIFF_FORMAT_DIFFSTAT;
			opts.flags.stat_with_summary = 1;
			अवरोध;
		शेष:
			अवरोध;
		}
		opts.detect_rename = DIFF_DETECT_RENAME;
		diff_setup_done(&opts);
		diff_वृक्ष_oid(शीर्ष, new_शीर्ष, "", &opts);
		diffcore_std(&opts);
		diff_flush(&opts);
	}

	/* Run a post-merge hook */
	run_hooks_l(the_repository, "post-merge", squash ? "1" : "0", शून्य);

	अगर (new_शीर्ष)
		apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
				    शून्य, शून्य, शून्य, शून्य);
	strbuf_release(&reflog_message);
}

/* Get the name for the merge commit's message. */
अटल व्योम merge_name(स्थिर अक्षर *remote, काष्ठा strbuf *संदेश)
{
	काष्ठा commit *remote_शीर्ष;
	काष्ठा object_id branch_शीर्ष_2;
	काष्ठा strbuf bname = STRBUF_INIT;
	काष्ठा merge_remote_desc *desc;
	स्थिर अक्षर *ptr;
	अक्षर *found_ref = शून्य;
	पूर्णांक len, early;

	copy_branchname(the_repository, &bname, remote, 0);
	remote = bname.buf;

	oidclr(&branch_शीर्ष_2, the_repository->hash_algo);
	remote_शीर्ष = get_merge_parent(remote);
	अगर (!remote_शीर्ष)
		die(_("'%s' does not point to a commit"), remote);

	अगर (repo_dwim_ref(the_repository, remote, म_माप(remote), &branch_शीर्ष_2,
			  &found_ref, 0) > 0) {
		अगर (starts_with(found_ref, "refs/heads/")) {
			strbuf_addf(संदेश, "%s\t\tbranch '%s' of .\n",
				    oid_to_hex(&branch_शीर्ष_2), remote);
			जाओ cleanup;
		}
		अगर (starts_with(found_ref, "refs/tags/")) {
			strbuf_addf(संदेश, "%s\t\ttag '%s' of .\n",
				    oid_to_hex(&branch_शीर्ष_2), remote);
			जाओ cleanup;
		}
		अगर (starts_with(found_ref, "refs/remotes/")) {
			strbuf_addf(संदेश, "%s\t\tremote-tracking branch '%s' of .\n",
				    oid_to_hex(&branch_शीर्ष_2), remote);
			जाओ cleanup;
		}
	}

	/* See if remote matches <name>^^^.. or <name>~<number> */
	क्रम (len = 0, ptr = remote + म_माप(remote);
	     remote < ptr && ptr[-1] == '^';
	     ptr--)
		len++;
	अगर (len)
		early = 1;
	अन्यथा {
		early = 0;
		ptr = म_खोजप(remote, '~');
		अगर (ptr) {
			पूर्णांक seen_nonzero = 0;

			len++; /* count ~ */
			जबतक (*++ptr && है_अंक(*ptr)) {
				seen_nonzero |= (*ptr != '0');
				len++;
			}
			अगर (*ptr)
				len = 0; /* not ...~<number> */
			अन्यथा अगर (seen_nonzero)
				early = 1;
			अन्यथा अगर (len == 1)
				early = 1; /* "name~" is "name~1"! */
		}
	}
	अगर (len) {
		काष्ठा strbuf truname = STRBUF_INIT;
		strbuf_addf(&truname, "refs/heads/%s", remote);
		strbuf_setlen(&truname, truname.len - len);
		अगर (refs_ref_exists(get_main_ref_store(the_repository), truname.buf)) {
			strbuf_addf(संदेश,
				    "%s\t\tbranch '%s'%s of .\n",
				    oid_to_hex(&remote_शीर्ष->object.oid),
				    truname.buf + 11,
				    (early ? " (early part)" : ""));
			strbuf_release(&truname);
			जाओ cleanup;
		}
		strbuf_release(&truname);
	}

	desc = merge_remote_util(remote_शीर्ष);
	अगर (desc && desc->obj && desc->obj->type == OBJ_TAG) {
		strbuf_addf(संदेश, "%s\t\t%s '%s'\n",
			    oid_to_hex(&desc->obj->oid),
			    type_name(desc->obj->type),
			    remote);
		जाओ cleanup;
	}

	strbuf_addf(संदेश, "%s\t\tcommit '%s'\n",
		oid_to_hex(&remote_शीर्ष->object.oid), remote);
cleanup:
	मुक्त(found_ref);
	strbuf_release(&bname);
}

अटल व्योम parse_branch_merge_options(अक्षर *bmo)
{
	स्थिर अक्षर **argv;
	पूर्णांक argc;

	अगर (!bmo)
		वापस;
	argc = split_cmdline(bmo, &argv);
	अगर (argc < 0)
		die(_("Bad branch.%s.mergeoptions string: %s"), branch,
		    _(split_cmdline_strerror(argc)));
	REALLOC_ARRAY(argv, argc + 2);
	MOVE_ARRAY(argv + 1, argv, argc + 1);
	argc++;
	argv[0] = "branch.*.mergeoptions";
	parse_options(argc, argv, शून्य, builtin_merge_options,
		      builtin_merge_usage, 0);
	मुक्त(argv);
}

अटल पूर्णांक git_merge_config(स्थिर अक्षर *k, स्थिर अक्षर *v,
			    स्थिर काष्ठा config_context *ctx, व्योम *cb)
{
	पूर्णांक status;
	स्थिर अक्षर *str;

	अगर (branch &&
	    skip_prefix(k, "branch.", &str) &&
	    skip_prefix(str, branch, &str) &&
	    !म_भेद(str, ".mergeoptions")) {
		मुक्त(branch_mergeoptions);
		branch_mergeoptions = xstrdup(v);
		वापस 0;
	}

	अगर (!म_भेद(k, "merge.diffstat") || !म_भेद(k, "merge.stat")) {
		पूर्णांक val = git_parse_maybe_bool_text(v);
		चयन (val) {
		हाल 0:
			show_diffstat = 0;
			अवरोध;
		हाल 1:
			show_diffstat = MERGE_SHOW_DIFFSTAT;
			अवरोध;
		शेष:
			अगर (!म_भेद(v, "compact"))
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
			अवरोध;
		}
	} अन्यथा अगर (!म_भेद(k, "merge.verifysignatures")) {
		verify_signatures = git_config_bool(k, v);
	} अन्यथा अगर (!म_भेद(k, "pull.twohead")) {
		FREE_AND_NULL(pull_twohead);
		वापस git_config_string(&pull_twohead, k, v);
	} अन्यथा अगर (!म_भेद(k, "pull.octopus")) {
		FREE_AND_NULL(pull_octopus);
		वापस git_config_string(&pull_octopus, k, v);
	} अन्यथा अगर (!म_भेद(k, "commit.cleanup")) {
		वापस git_config_string(&cleanup_arg, k, v);
	} अन्यथा अगर (!म_भेद(k, "merge.ff")) {
		पूर्णांक boolval = git_parse_maybe_bool(v);
		अगर (0 <= boolval) {
			fast_forward = boolval ? FF_ALLOW : FF_NO;
		} अन्यथा अगर (v && !म_भेद(v, "only")) {
			fast_forward = FF_ONLY;
		} /* do not barf on values from future versions of git */
		वापस 0;
	} अन्यथा अगर (!म_भेद(k, "merge.defaulttoupstream")) {
		default_to_upstream = git_config_bool(k, v);
		वापस 0;
	} अन्यथा अगर (!म_भेद(k, "commit.gpgsign")) {
		sign_commit = git_config_bool(k, v) ? "" : शून्य;
		वापस 0;
	} अन्यथा अगर (!म_भेद(k, "gpg.mintrustlevel")) {
		check_trust_level = 0;
	} अन्यथा अगर (!म_भेद(k, "merge.autostash")) {
		autostash = git_config_bool(k, v);
		वापस 0;
	}

	status = fmt_merge_संदेश_config(k, v, ctx, cb);
	अगर (status)
		वापस status;
	वापस git_diff_ui_config(k, v, ctx, cb);
}

अटल पूर्णांक read_वृक्ष_trivial(काष्ठा object_id *common, काष्ठा object_id *शीर्ष,
			     काष्ठा object_id *one)
{
	पूर्णांक i, nr_trees = 0;
	काष्ठा वृक्ष *trees[MAX_UNPACK_TREES];
	काष्ठा वृक्ष_desc t[MAX_UNPACK_TREES];
	काष्ठा unpack_trees_options opts;

	स_रखो(&opts, 0, माप(opts));
	opts.शीर्ष_idx = 2;
	opts.src_सूचकांक = the_repository->सूचकांक;
	opts.dst_सूचकांक = the_repository->सूचकांक;
	opts.update = 1;
	opts.verbose_update = 1;
	opts.trivial_merges_only = 1;
	opts.merge = 1;
	opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
	trees[nr_trees] = repo_parse_वृक्ष_indirect(the_repository, common);
	अगर (!trees[nr_trees++])
		वापस -1;
	trees[nr_trees] = repo_parse_वृक्ष_indirect(the_repository, शीर्ष);
	अगर (!trees[nr_trees++])
		वापस -1;
	trees[nr_trees] = repo_parse_वृक्ष_indirect(the_repository, one);
	अगर (!trees[nr_trees++])
		वापस -1;
	opts.fn = threeway_merge;
	cache_वृक्ष_free(&the_repository->सूचकांक->cache_वृक्ष);
	क्रम (i = 0; i < nr_trees; i++) {
		repo_parse_वृक्ष(the_repository, trees[i]);
		init_वृक्ष_desc(t+i, &trees[i]->object.oid,
			       trees[i]->बफर, trees[i]->आकार);
	}
	अगर (unpack_trees(nr_trees, t, &opts))
		वापस -1;
	वापस 0;
}

अटल व्योम write_वृक्ष_trivial(काष्ठा object_id *oid)
{
	अगर (write_सूचकांक_as_वृक्ष(oid, the_repository->सूचकांक,
				repo_get_सूचकांक_file(the_repository),
				0, शून्य))
		die(_("git write-tree failed to write a tree"));
}

अटल पूर्णांक try_merge_strategy(स्थिर अक्षर *strategy, काष्ठा commit_list *common,
			      काष्ठा commit_list *remoteheads,
			      काष्ठा commit *शीर्ष)
{
	स्थिर अक्षर *शीर्ष_arg = "HEAD";

	अगर (repo_refresh_and_write_सूचकांक(the_repository, REFRESH_QUIET,
					 SKIP_IF_UNCHANGED, 0, शून्य, शून्य,
					 शून्य) < 0)
		die(_("Unable to write index."));

	अगर (!म_भेद(strategy, "recursive") || !म_भेद(strategy, "subtree") ||
	    !म_भेद(strategy, "ort")) {
		काष्ठा lock_file lock = LOCK_INIT;
		पूर्णांक clean, x;
		काष्ठा commit *परिणाम;
		काष्ठा commit_list *reversed = शून्य;
		काष्ठा merge_options o;
		काष्ठा commit_list *j;

		अगर (remoteheads->अग्र) {
			error(_("Not handling anything other than two heads merge."));
			वापस 2;
		}

		init_ui_merge_options(&o, the_repository);
		अगर (!म_भेद(strategy, "subtree"))
			o.subtree_shift = "";

		o.show_rename_progress =
			show_progress == -1 ? isatty(2) : show_progress;

		क्रम (x = 0; x < xopts.nr; x++)
			अगर (parse_merge_opt(&o, xopts.v[x]))
				die(_("unknown strategy option: -X%s"), xopts.v[x]);

		o.branch1 = शीर्ष_arg;
		o.branch2 = merge_remote_util(remoteheads->item)->name;

		क्रम (j = common; j; j = j->अग्र)
			commit_list_जोड़ो(j->item, &reversed);

		repo_hold_locked_सूचकांक(the_repository, &lock,
				       LOCK_DIE_ON_ERROR);
		clean = merge_ort_recursive(&o, शीर्ष, remoteheads->item,
					    reversed, &परिणाम);
		commit_list_free(reversed);
		strbuf_release(&o.obuf);

		अगर (clean < 0) {
			rollback_lock_file(&lock);
			वापस 2;
		}
		अगर (write_locked_सूचकांक(the_repository->सूचकांक, &lock,
				       COMMIT_LOCK | SKIP_IF_UNCHANGED))
			die(_("unable to write %s"), repo_get_सूचकांक_file(the_repository));
		वापस clean ? 0 : 1;
	} अन्यथा {
		वापस try_merge_command(the_repository,
					 strategy, xopts.nr, xopts.v,
					 common, शीर्ष_arg, remoteheads);
	}
}

अटल व्योम गिनती_diff_files(काष्ठा diff_queue_struct *q,
			     काष्ठा diff_options *opt UNUSED, व्योम *तथ्य)
{
	पूर्णांक *गिनती = तथ्य;

	(*गिनती) += q->nr;
}

अटल पूर्णांक गिनती_unmerged_entries(व्योम)
{
	पूर्णांक i, ret = 0;

	क्रम (i = 0; i < the_repository->सूचकांक->cache_nr; i++)
		अगर (ce_stage(the_repository->सूचकांक->cache[i]))
			ret++;

	वापस ret;
}

अटल व्योम add_strategies(स्थिर अक्षर *string, अचिन्हित attr)
{
	पूर्णांक i;

	अगर (string) {
		काष्ठा string_list list = STRING_LIST_INIT_DUP;
		काष्ठा string_list_item *item;
		string_list_split(&list, string, " ", -1);
		for_each_string_list_item(item, &list)
			append_strategy(get_strategy(item->string));
		string_list_clear(&list, 0);
		वापस;
	}
	क्रम (i = 0; i < ARRAY_आकार(all_strategy); i++)
		अगर (all_strategy[i].attr & attr)
			append_strategy(&all_strategy[i]);

}

अटल व्योम read_merge_संदेश(काष्ठा strbuf *संदेश)
{
	स्थिर अक्षर *filename = git_path_merge_संदेश(the_repository);
	strbuf_reset(संदेश);
	अगर (strbuf_read_file(संदेश, filename, 0) < 0)
		die_errno(_("Could not read from '%s'"), filename);
}

अटल व्योम write_merge_state(काष्ठा commit_list *);
अटल व्योम abort_commit(काष्ठा commit_list *remoteheads, स्थिर अक्षर *err_संदेश)
{
	अगर (err_संदेश)
		error("%s", err_संदेश);
	ख_लिखो(मानक_त्रुटि,
		_("Not committing merge; use 'git commit' to complete the merge.\n"));
	write_merge_state(remoteheads);
	निकास(1);
}

अटल स्थिर अक्षर merge_editor_comment[] =
N_("Please enter a commit message to explain why this merge is necessary,\n"
   "especially if it merges an updated upstream into a topic branch.\n"
   "\n");

अटल स्थिर अक्षर scissors_editor_comment[] =
N_("An empty message aborts the commit.\n");

अटल स्थिर अक्षर no_scissors_editor_comment[] =
N_("Lines starting with '%s' will be ignored, and an empty message aborts\n"
   "the commit.\n");

अटल व्योम write_merge_heads(काष्ठा commit_list *);
अटल व्योम prepare_to_commit(काष्ठा commit_list *remoteheads)
{
	काष्ठा strbuf संदेश = STRBUF_INIT;
	स्थिर अक्षर *सूचकांक_file = repo_get_सूचकांक_file(the_repository);

	अगर (!no_verify) {
		पूर्णांक invoked_hook;

		अगर (run_commit_hook(0 < option_edit, सूचकांक_file, &invoked_hook,
				    "pre-merge-commit", शून्य))
			abort_commit(remoteheads, शून्य);
		/*
		 * Re-read the index as pre-merge-commit hook could have updated it,
		 * and write it out as a tree.  We must do this before we invoke
		 * the editor and after we invoke run_status above.
		 */
		अगर (invoked_hook)
			discard_सूचकांक(the_repository->सूचकांक);
	}
	read_सूचकांक_from(the_repository->सूचकांक, सूचकांक_file,
			repo_get_git_dir(the_repository));
	strbuf_addbuf(&संदेश, &merge_संदेश);
	अगर (squash)
		BUG("the control must not reach here under --squash");
	अगर (0 < option_edit) {
		strbuf_addch(&संदेश, '\n');
		अगर (cleanup_mode == COMMIT_संदेश_CLEANUP_SCISSORS) {
			wt_status_append_cut_पंक्ति(&संदेश);
			strbuf_commented_addf(&संदेश, comment_पंक्ति_str, "\n");
		}
		strbuf_commented_addf(&संदेश, comment_पंक्ति_str,
				      _(merge_editor_comment));
		अगर (cleanup_mode == COMMIT_संदेश_CLEANUP_SCISSORS)
			strbuf_commented_addf(&संदेश, comment_पंक्ति_str,
					      _(scissors_editor_comment));
		अन्यथा
			strbuf_commented_addf(&संदेश, comment_पंक्ति_str,
				_(no_scissors_editor_comment), comment_पंक्ति_str);
	}
	अगर (signoff)
		append_signoff(&संदेश, ignored_log_message_bytes(संदेश.buf, संदेश.len), 0);
	write_merge_heads(remoteheads);
	write_file_buf(git_path_merge_संदेश(the_repository), संदेश.buf, संदेश.len);
	अगर (run_commit_hook(0 < option_edit, repo_get_सूचकांक_file(the_repository),
			    शून्य, "prepare-commit-msg",
			    git_path_merge_संदेश(the_repository), "merge", शून्य))
		abort_commit(remoteheads, शून्य);
	अगर (0 < option_edit) {
		अगर (launch_editor(git_path_merge_संदेश(the_repository), शून्य, शून्य))
			abort_commit(remoteheads, शून्य);
	}

	अगर (!no_verify && run_commit_hook(0 < option_edit, repo_get_सूचकांक_file(the_repository),
					  शून्य, "commit-msg",
					  git_path_merge_संदेश(the_repository), शून्य))
		abort_commit(remoteheads, शून्य);

	read_merge_संदेश(&संदेश);
	cleanup_message(&संदेश, cleanup_mode, 0);
	अगर (!संदेश.len)
		abort_commit(remoteheads, _("Empty commit message."));
	strbuf_release(&merge_संदेश);
	strbuf_addbuf(&merge_संदेश, &संदेश);
	strbuf_release(&संदेश);
}

अटल पूर्णांक merge_trivial(काष्ठा commit *शीर्ष, काष्ठा commit_list *remoteheads)
{
	काष्ठा object_id परिणाम_वृक्ष, परिणाम_commit;
	काष्ठा commit_list *parents = शून्य, **pptr = &parents;

	अगर (repo_refresh_and_write_सूचकांक(the_repository, REFRESH_QUIET,
					 SKIP_IF_UNCHANGED, 0, शून्य, शून्य,
					 शून्य) < 0)
		वापस error(_("Unable to write index."));

	write_वृक्ष_trivial(&परिणाम_वृक्ष);
	म_लिखो(_("Wonderful.\n"));
	pptr = commit_list_append(शीर्ष, pptr);
	pptr = commit_list_append(remoteheads->item, pptr);
	prepare_to_commit(remoteheads);
	अगर (commit_वृक्ष(merge_संदेश.buf, merge_संदेश.len, &परिणाम_वृक्ष, parents,
			&परिणाम_commit, शून्य, sign_commit))
		die(_("failed to write commit object"));
	finish(शीर्ष, remoteheads, &परिणाम_commit, "In-index merge");

	remove_merge_branch_state(the_repository);
	commit_list_free(parents);
	वापस 0;
}

अटल पूर्णांक finish_automerge(काष्ठा commit *शीर्ष,
			    पूर्णांक शीर्ष_subsumed,
			    काष्ठा commit_list *common,
			    काष्ठा commit_list *remoteheads,
			    काष्ठा object_id *परिणाम_वृक्ष,
			    स्थिर अक्षर *wt_strategy)
{
	काष्ठा commit_list *parents = शून्य;
	काष्ठा strbuf buf = STRBUF_INIT;
	काष्ठा object_id परिणाम_commit;

	write_वृक्ष_trivial(परिणाम_वृक्ष);
	commit_list_free(common);
	parents = remoteheads;
	अगर (!शीर्ष_subsumed || fast_forward == FF_NO)
		commit_list_जोड़ो(शीर्ष, &parents);
	prepare_to_commit(remoteheads);
	अगर (commit_वृक्ष(merge_संदेश.buf, merge_संदेश.len, परिणाम_वृक्ष, parents,
			&परिणाम_commit, शून्य, sign_commit))
		die(_("failed to write commit object"));
	strbuf_addf(&buf, "Merge made by the '%s' strategy.", wt_strategy);
	finish(शीर्ष, remoteheads, &परिणाम_commit, buf.buf);

	strbuf_release(&buf);
	remove_merge_branch_state(the_repository);
	commit_list_free(parents);
	वापस 0;
}

अटल पूर्णांक suggest_conflicts(व्योम)
{
	स्थिर अक्षर *filename;
	खाता *fp;
	काष्ठा strbuf msgbuf = STRBUF_INIT;

	filename = git_path_merge_संदेश(the_repository);
	fp = xfopen(filename, "a");

	/*
	 * We can't use cleanup_mode because if we're not using the editor,
	 * get_cleanup_mode will return COMMIT_MSG_CLEANUP_SPACE instead, even
	 * though the message is meant to be processed later by git-commit.
	 * Thus, we will get the cleanup mode which is returned when we _are_
	 * using an editor.
	 */
	append_conflicts_hint(the_repository->सूचकांक, &msgbuf,
			      get_cleanup_mode(cleanup_arg, 1));
	ख_माला_दो(msgbuf.buf, fp);
	strbuf_release(&msgbuf);
	ख_बंद(fp);
	repo_rerere(the_repository, allow_rerere_auto);
	म_लिखो(_("Automatic merge failed; "
			"fix conflicts and then commit the result.\n"));
	वापस 1;
}

अटल पूर्णांक evaluate_परिणाम(व्योम)
{
	पूर्णांक cnt = 0;
	काष्ठा rev_info rev;

	/* Check how many files differ. */
	repo_init_revisions(the_repository, &rev, "");
	setup_revisions(0, शून्य, &rev, शून्य);
	rev.diffopt.output_format |=
		DIFF_FORMAT_CALLBACK;
	rev.diffopt.format_callback = गिनती_diff_files;
	rev.diffopt.format_callback_तथ्य = &cnt;
	run_diff_files(&rev, 0);

	/*
	 * Check how many unmerged entries are
	 * there.
	 */
	cnt += गिनती_unmerged_entries();

	release_revisions(&rev);
	वापस cnt;
}

/*
 * Pretend as if the user told us to merge with the remote-tracking
 * branch we have for the upstream of the current branch
 */
अटल पूर्णांक setup_with_upstream(स्थिर अक्षर ***argv)
{
	काष्ठा branch *branch = branch_get(शून्य);
	पूर्णांक i;
	स्थिर अक्षर **args;

	अगर (!branch)
		die(_("No current branch."));
	अगर (!branch->remote_name)
		die(_("No remote for the current branch."));
	अगर (!branch->merge_nr)
		die(_("No default upstream defined for the current branch."));

	args = xcalloc(st_add(branch->merge_nr, 1), माप(अक्षर *));
	क्रम (i = 0; i < branch->merge_nr; i++) {
		अगर (!branch->merge[i]->dst)
			die(_("No remote-tracking branch for %s from %s"),
			    branch->merge[i]->src, branch->remote_name);
		args[i] = branch->merge[i]->dst;
	}
	args[i] = शून्य;
	*argv = args;
	वापस i;
}

अटल व्योम write_merge_heads(काष्ठा commit_list *remoteheads)
{
	काष्ठा commit_list *j;
	काष्ठा strbuf buf = STRBUF_INIT;

	क्रम (j = remoteheads; j; j = j->अग्र) {
		काष्ठा object_id *oid;
		काष्ठा commit *c = j->item;
		काष्ठा merge_remote_desc *desc;

		desc = merge_remote_util(c);
		अगर (desc && desc->obj) {
			oid = &desc->obj->oid;
		} अन्यथा {
			oid = &c->object.oid;
		}
		strbuf_addf(&buf, "%s\n", oid_to_hex(oid));
	}
	write_file_buf(git_path_merge_शीर्ष(the_repository), buf.buf, buf.len);

	strbuf_reset(&buf);
	अगर (fast_forward == FF_NO)
		strbuf_addstr(&buf, "no-ff");
	write_file_buf(git_path_merge_mode(the_repository), buf.buf, buf.len);
	strbuf_release(&buf);
}

अटल व्योम write_merge_state(काष्ठा commit_list *remoteheads)
{
	write_merge_heads(remoteheads);
	strbuf_addch(&merge_संदेश, '\n');
	write_file_buf(git_path_merge_संदेश(the_repository), merge_संदेश.buf,
		       merge_संदेश.len);
}

अटल पूर्णांक default_edit_option(व्योम)
{
	अटल स्थिर अक्षर name[] = "GIT_MERGE_AUTOEDIT";
	स्थिर अक्षर *e = दो_पर्या(name);
	काष्ठा stat st_stdin, st_stdout;

	अगर (have_message)
		/* an explicit -m msg without --[no-]edit */
		वापस 0;

	अगर (e) {
		पूर्णांक v = git_parse_maybe_bool(e);
		अगर (v < 0)
			die(_("Bad value '%s' in environment '%s'"), e, name);
		वापस v;
	}

	/* Use editor if stdin and stdout are the same and is a tty */
	वापस (!ख_स्थिति(0, &st_stdin) &&
		!ख_स्थिति(1, &st_stdout) &&
		isatty(0) && isatty(1) &&
		st_stdin.st_dev == st_stdout.st_dev &&
		st_stdin.st_ino == st_stdout.st_ino &&
		st_stdin.st_mode == st_stdout.st_mode);
}

अटल काष्ठा commit_list *reduce_parents(काष्ठा commit *शीर्ष_commit,
					  पूर्णांक *शीर्ष_subsumed,
					  काष्ठा commit_list *remoteheads)
{
	काष्ठा commit_list *parents, **remotes;

	/*
	 * Is the current HEAD reachable from another commit being
	 * merged?  If so we do not want to record it as a parent of
	 * the resulting merge, unless --no-ff is given.  We will flip
	 * this variable to 0 when we find HEAD among the independent
	 * tips being merged.
	 */
	*शीर्ष_subsumed = 1;

	/* Find what parents to record by checking independent ones. */
	parents = reduce_heads(remoteheads);
	commit_list_free(remoteheads);

	remoteheads = शून्य;
	remotes = &remoteheads;
	जबतक (parents) {
		काष्ठा commit *commit = निकालो_commit(&parents);
		अगर (commit == शीर्ष_commit)
			*शीर्ष_subsumed = 0;
		अन्यथा
			remotes = &commit_list_जोड़ो(commit, remotes)->अग्र;
	}
	वापस remoteheads;
}

अटल व्योम prepare_merge_message(काष्ठा strbuf *merge_names, काष्ठा strbuf *merge_संदेश)
{
	काष्ठा fmt_merge_संदेश_opts opts;

	स_रखो(&opts, 0, माप(opts));
	opts.add_title = !have_message;
	opts.shortlog_len = shortlog_len;
	opts.credit_people = (0 < option_edit);
	opts.into_name = into_name;

	fmt_merge_संदेश(merge_names, merge_संदेश, &opts);
	अगर (merge_संदेश->len)
		strbuf_setlen(merge_संदेश, merge_संदेश->len - 1);
}

अटल व्योम handle_fetch_शीर्ष(काष्ठा commit_list **remotes, काष्ठा strbuf *merge_names)
{
	स्थिर अक्षर *filename;
	पूर्णांक fd, pos, npos;
	काष्ठा strbuf fetch_शीर्ष_file = STRBUF_INIT;
	स्थिर अचिन्हित hexsz = the_hash_algo->hexsz;

	अगर (!merge_names)
		merge_names = &fetch_शीर्ष_file;

	filename = git_path_fetch_शीर्ष(the_repository);
	fd = xopen(filename, O_RDONLY);

	अगर (strbuf_read(merge_names, fd, 0) < 0)
		die_errno(_("could not read '%s'"), filename);
	अगर (बंद(fd) < 0)
		die_errno(_("could not close '%s'"), filename);

	क्रम (pos = 0; pos < merge_names->len; pos = npos) {
		काष्ठा object_id oid;
		अक्षर *ptr;
		काष्ठा commit *commit;

		ptr = म_अक्षर(merge_names->buf + pos, '\n');
		अगर (ptr)
			npos = ptr - merge_names->buf + 1;
		अन्यथा
			npos = merge_names->len;

		अगर (npos - pos < hexsz + 2 ||
		    get_oid_hex(merge_names->buf + pos, &oid))
			commit = शून्य; /* bad */
		अन्यथा अगर (स_भेद(merge_names->buf + pos + hexsz, "\t\t", 2))
			जारी; /* not-for-merge */
		अन्यथा {
			अक्षर saved = merge_names->buf[pos + hexsz];
			merge_names->buf[pos + hexsz] = '\0';
			commit = get_merge_parent(merge_names->buf + pos);
			merge_names->buf[pos + hexsz] = saved;
		}
		अगर (!commit) {
			अगर (ptr)
				*ptr = '\0';
			die(_("not something we can merge in %s: %s"),
			    filename, merge_names->buf + pos);
		}
		remotes = &commit_list_जोड़ो(commit, remotes)->अग्र;
	}

	अगर (merge_names == &fetch_शीर्ष_file)
		strbuf_release(&fetch_शीर्ष_file);
}

अटल काष्ठा commit_list *collect_parents(काष्ठा commit *शीर्ष_commit,
					   पूर्णांक *शीर्ष_subsumed,
					   पूर्णांक argc, स्थिर अक्षर **argv,
					   काष्ठा strbuf *merge_संदेश)
{
	पूर्णांक i;
	काष्ठा commit_list *remoteheads = शून्य;
	काष्ठा commit_list **remotes = &remoteheads;
	काष्ठा strbuf merge_names = STRBUF_INIT, *autogen = शून्य;

	अगर (merge_संदेश && (!have_message || shortlog_len))
		autogen = &merge_names;

	अगर (शीर्ष_commit)
		remotes = &commit_list_जोड़ो(शीर्ष_commit, remotes)->अग्र;

	अगर (argc == 1 && !म_भेद(argv[0], "FETCH_HEAD")) {
		handle_fetch_शीर्ष(remotes, autogen);
		remoteheads = reduce_parents(शीर्ष_commit, शीर्ष_subsumed, remoteheads);
	} अन्यथा {
		क्रम (i = 0; i < argc; i++) {
			काष्ठा commit *commit = get_merge_parent(argv[i]);
			अगर (!commit)
				help_unknown_ref(argv[i], "merge",
						 _("not something we can merge"));
			remotes = &commit_list_जोड़ो(commit, remotes)->अग्र;
		}
		remoteheads = reduce_parents(शीर्ष_commit, शीर्ष_subsumed, remoteheads);
		अगर (autogen) {
			काष्ठा commit_list *p;
			क्रम (p = remoteheads; p; p = p->अग्र)
				merge_name(merge_remote_util(p->item)->name, autogen);
		}
	}

	अगर (autogen) {
		prepare_merge_message(autogen, merge_संदेश);
		strbuf_release(autogen);
	}

	वापस remoteheads;
}

अटल पूर्णांक merging_a_throwaway_tag(काष्ठा commit *commit)
{
	अक्षर *tag_ref;
	काष्ठा object_id oid;
	पूर्णांक is_throwaway_tag = 0;

	/* Are we merging a tag? */
	अगर (!merge_remote_util(commit) ||
	    !merge_remote_util(commit)->obj ||
	    merge_remote_util(commit)->obj->type != OBJ_TAG)
		वापस is_throwaway_tag;

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
			  ((काष्ठा tag *)merge_remote_util(commit)->obj)->tag);
	अगर (!refs_read_ref(get_main_ref_store(the_repository), tag_ref, &oid) &&
	    oideq(&oid, &merge_remote_util(commit)->obj->oid))
		is_throwaway_tag = 0;
	अन्यथा
		is_throwaway_tag = 1;
	मुक्त(tag_ref);
	वापस is_throwaway_tag;
}

पूर्णांक cmd_merge(पूर्णांक argc,
	      स्थिर अक्षर **argv,
	      स्थिर अक्षर *prefix,
	      काष्ठा repository *repo UNUSED)
{
	काष्ठा object_id परिणाम_वृक्ष, stash, शीर्ष_oid;
	काष्ठा commit *शीर्ष_commit;
	काष्ठा strbuf buf = STRBUF_INIT;
	पूर्णांक i, ret = 0, शीर्ष_subsumed;
	पूर्णांक best_cnt = -1, merge_was_ok = 0, automerge_was_ok = 0;
	काष्ठा commit_list *common = शून्य;
	स्थिर अक्षर *best_strategy = शून्य, *wt_strategy = शून्य;
	काष्ठा commit_list *remoteheads = शून्य, *p;
	व्योम *branch_to_free, *argv_to_free = शून्य;
	पूर्णांक orig_argc = argc;
	पूर्णांक merge_log_config = -1;

	show_usage_with_options_if_asked(argc, argv,
					 builtin_merge_usage, builtin_merge_options);

#अगर_अघोषित WITH_BREAKING_CHANGES
	warn_on_auto_comment_char = true;
#पूर्ण_अगर /* !WITH_BREAKING_CHANGES */
	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_सूचकांक = 0;

	/*
	 * Check if we are _not_ on a detached HEAD, i.e. if there is a
	 * current branch.
	 */
	branch = branch_to_free = refs_resolve_refdup(get_main_ref_store(the_repository),
						      "HEAD", 0, &शीर्ष_oid,
						      शून्य);
	अगर (branch)
		skip_prefix(branch, "refs/heads/", &branch);

	init_diff_ui_defaults();
	repo_config(the_repository, git_merge_config, &merge_log_config);

	अगर (!branch || is_null_oid(&शीर्ष_oid))
		शीर्ष_commit = शून्य;
	अन्यथा
		शीर्ष_commit = lookup_commit_or_die(&शीर्ष_oid, "HEAD");

	अगर (branch_mergeoptions)
		parse_branch_merge_options(branch_mergeoptions);
	argc = parse_options(argc, argv, prefix, builtin_merge_options,
			builtin_merge_usage, 0);
	अगर (shortlog_len < 0)
		shortlog_len = (merge_log_config > 0) ? merge_log_config : 0;

	अगर (verbosity < 0 && show_progress == -1)
		show_progress = 0;

	अगर (abort_current_merge) {
		पूर्णांक nargc = 2;
		स्थिर अक्षर *nargv[] = {"reset", "--merge", शून्य};
		अक्षर stash_oid_hex[GIT_MAX_HEXSZ + 1];
		काष्ठा object_id stash_oid = {0};

		अगर (orig_argc != 2)
			usage_संदेश_opt(_("--abort expects no arguments"),
			      builtin_merge_usage, builtin_merge_options);

		अगर (!file_exists(git_path_merge_शीर्ष(the_repository)))
			die(_("There is no merge to abort (MERGE_HEAD missing)."));

		अगर (!refs_read_ref(get_main_ref_store(the_repository), "MERGE_AUTOSTASH", &stash_oid))
			refs_delete_ref(get_main_ref_store(the_repository),
					"", "MERGE_AUTOSTASH", &stash_oid,
					REF_NO_DEREF);

		/* Invoke 'git reset --merge' */
		ret = cmd_reset(nargc, nargv, prefix, the_repository);

		अगर (!is_null_oid(&stash_oid)) {
			oid_to_hex_r(stash_oid_hex, &stash_oid);
			apply_autostash_oid(stash_oid_hex);
		}

		जाओ done;
	}

	अगर (quit_current_merge) {
		अगर (orig_argc != 2)
			usage_संदेश_opt(_("--quit expects no arguments"),
				      builtin_merge_usage,
				      builtin_merge_options);

		remove_merge_branch_state(the_repository);
		जाओ done;
	}

	अगर (continue_current_merge) {
		पूर्णांक nargc = 1;
		स्थिर अक्षर *nargv[] = {"commit", शून्य};

		अगर (orig_argc != 2)
			usage_संदेश_opt(_("--continue expects no arguments"),
			      builtin_merge_usage, builtin_merge_options);

		अगर (!file_exists(git_path_merge_शीर्ष(the_repository)))
			die(_("There is no merge in progress (MERGE_HEAD missing)."));

		/* Invoke 'git commit' */
		ret = cmd_commit(nargc, nargv, prefix, the_repository);
		जाओ done;
	}

	अगर (repo_read_सूचकांक_unmerged(the_repository))
		die_resolve_conflict("merge");

	अगर (file_exists(git_path_merge_शीर्ष(the_repository))) {
		/*
		 * There is no unmerged entry, don't advise 'git
		 * add/rm <file>', just 'git commit'.
		 */
		अगर (advice_enabled(ADVICE_RESOLVE_CONFLICT))
			die(_("You have not concluded your merge (MERGE_HEAD exists).\n"
				  "Please, commit your changes before you merge."));
		अन्यथा
			die(_("You have not concluded your merge (MERGE_HEAD exists)."));
	}
	अगर (refs_ref_exists(get_main_ref_store(the_repository), "CHERRY_PICK_HEAD")) {
		अगर (advice_enabled(ADVICE_RESOLVE_CONFLICT))
			die(_("You have not concluded your cherry-pick (CHERRY_PICK_HEAD exists).\n"
			    "Please, commit your changes before you merge."));
		अन्यथा
			die(_("You have not concluded your cherry-pick (CHERRY_PICK_HEAD exists)."));
	}
	resolve_undo_clear_सूचकांक(the_repository->सूचकांक);

	अगर (option_edit < 0)
		option_edit = default_edit_option();

	cleanup_mode = get_cleanup_mode(cleanup_arg, 0 < option_edit);

	अगर (verbosity < 0)
		show_diffstat = 0;

	अगर (squash) {
		अगर (fast_forward == FF_NO)
			die(_("options '%s' and '%s' cannot be used together"), "--squash", "--no-ff.");
		अगर (option_commit > 0)
			die(_("options '%s' and '%s' cannot be used together"), "--squash", "--commit.");
		/*
		 * squash can now silently disable option_commit - this is not
		 * a problem as it is only overriding the default, not a user
		 * supplied option.
		 */
		option_commit = 0;
	}

	अगर (option_commit < 0)
		option_commit = 1;

	अगर (!argc) {
		अगर (default_to_upstream) {
			argc = setup_with_upstream(&argv);
			argv_to_free = argv;
		}
		अन्यथा
			die(_("No commit specified and merge.defaultToUpstream not set."));
	} अन्यथा अगर (argc == 1 && !म_भेद(argv[0], "-")) {
		argv[0] = "@{-1}";
	}

	अगर (!argc)
		usage_with_options(builtin_merge_usage,
			builtin_merge_options);

	अगर (!शीर्ष_commit) {
		/*
		 * If the merged head is a valid one there is no reason
		 * to forbid "git merge" into a branch yet to be born.
		 * We do the same for "git pull".
		 */
		काष्ठा object_id *remote_शीर्ष_oid;
		अगर (squash)
			die(_("Squash commit into empty head not supported yet"));
		अगर (fast_forward == FF_NO)
			die(_("Non-fast-forward commit does not make sense into "
			    "an empty head"));
		remoteheads = collect_parents(शीर्ष_commit, &शीर्ष_subsumed,
					      argc, argv, शून्य);
		अगर (!remoteheads)
			die(_("%s - not something we can merge"), argv[0]);
		अगर (remoteheads->अग्र)
			die(_("Can merge only exactly one commit into empty head"));

		अगर (verify_signatures)
			verify_merge_signature(remoteheads->item, verbosity,
					       check_trust_level);

		remote_शीर्ष_oid = &remoteheads->item->object.oid;
		read_empty(remote_शीर्ष_oid);
		refs_update_ref(get_main_ref_store(the_repository),
				"initial pull", "HEAD", remote_शीर्ष_oid, शून्य,
				0,
				UPDATE_REFS_DIE_ON_ERR);
		जाओ done;
	}

	/*
	 * All the rest are the commits being merged; prepare
	 * the standard merge summary message to be appended
	 * to the given message.
	 */
	remoteheads = collect_parents(शीर्ष_commit, &शीर्ष_subsumed,
				      argc, argv, &merge_संदेश);

	अगर (!शीर्ष_commit || !argc)
		usage_with_options(builtin_merge_usage,
			builtin_merge_options);

	अगर (verify_signatures) {
		क्रम (p = remoteheads; p; p = p->अग्र) {
			verify_merge_signature(p->item, verbosity,
					       check_trust_level);
		}
	}

	strbuf_addstr(&buf, "merge");
	क्रम (p = remoteheads; p; p = p->अग्र)
		strbuf_addf(&buf, " %s", merge_remote_util(p->item)->name);
	setenv("GIT_REFLOG_ACTION", buf.buf, 0);
	strbuf_reset(&buf);

	क्रम (p = remoteheads; p; p = p->अग्र) {
		काष्ठा commit *commit = p->item;
		strbuf_addf(&buf, "GITHEAD_%s",
			    oid_to_hex(&commit->object.oid));
		setenv(buf.buf, merge_remote_util(commit)->name, 1);
		strbuf_reset(&buf);
		अगर (fast_forward != FF_ONLY && merging_a_throwaway_tag(commit))
			fast_forward = FF_NO;
	}

	अगर (!use_strategies) {
		अगर (!remoteheads)
			; /* already up-to-date */
		अन्यथा अगर (!remoteheads->अग्र)
			add_strategies(pull_twohead, DEFAULT_TWOHEAD);
		अन्यथा
			add_strategies(pull_octopus, DEFAULT_OCTOPUS);
	}

	क्रम (i = 0; i < use_strategies_nr; i++) {
		अगर (use_strategies[i]->attr & NO_FAST_FORWARD)
			fast_forward = FF_NO;
		अगर (use_strategies[i]->attr & NO_TRIVIAL)
			allow_trivial = 0;
	}

	अगर (!remoteheads)
		; /* already up-to-date */
	अन्यथा अगर (!remoteheads->अग्र) {
		अगर (repo_get_merge_bases(the_repository, शीर्ष_commit,
					 remoteheads->item, &common) < 0) {
			ret = 2;
			जाओ done;
		}
	} अन्यथा {
		काष्ठा commit_list *list = remoteheads;
		commit_list_जोड़ो(शीर्ष_commit, &list);
		अगर (get_octopus_merge_bases(list, &common) < 0) {
			मुक्त(list);
			ret = 2;
			जाओ done;
		}
		मुक्त(list);
	}

	refs_update_ref(get_main_ref_store(the_repository),
			"updating ORIG_HEAD", "ORIG_HEAD",
			&शीर्ष_commit->object.oid, शून्य, 0,
			UPDATE_REFS_DIE_ON_ERR);

	अगर (remoteheads && !common) {
		/* No common ancestors found. */
		अगर (!allow_unrelated_histories)
			die(_("refusing to merge unrelated histories"));
		/* otherwise, we need a real merge. */
	} अन्यथा अगर (!remoteheads ||
		 (!remoteheads->अग्र && !common->अग्र &&
		  common->item == remoteheads->item)) {
		/*
		 * If head can reach all the merge then we are up to date.
		 * but first the most common case of merging one remote.
		 */
		finish_up_to_date();
		जाओ done;
	} अन्यथा अगर (fast_forward != FF_NO && !remoteheads->अग्र &&
			!common->अग्र &&
			oideq(&common->item->object.oid, &शीर्ष_commit->object.oid)) {
		/* Again the most common case of merging one remote. */
		स्थिर अक्षर *संदेश = have_message ?
			"Fast-forward (no commit created; -m option ignored)" :
			"Fast-forward";
		काष्ठा commit *commit;

		अगर (verbosity >= 0) {
			म_लिखो(_("Updating %s..%s\n"),
			       repo_find_unique_abbrev(the_repository, &शीर्ष_commit->object.oid,
						       DEFAULT_ABBREV),
			       repo_find_unique_abbrev(the_repository, &remoteheads->item->object.oid,
						       DEFAULT_ABBREV));
		}
		commit = remoteheads->item;
		अगर (!commit) {
			ret = 1;
			जाओ done;
		}

		अगर (autostash)
			create_autostash_ref(the_repository, "MERGE_AUTOSTASH",
					     शून्य, false);
		अगर (checkout_fast_forward(the_repository,
					  &शीर्ष_commit->object.oid,
					  &commit->object.oid,
					  overwrite_ignore)) {
			apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
					    शून्य, शून्य, शून्य, शून्य);
			ret = 1;
			जाओ done;
		}

		finish(शीर्ष_commit, remoteheads, &commit->object.oid, संदेश);
		remove_merge_branch_state(the_repository);
		जाओ done;
	} अन्यथा अगर (!remoteheads->अग्र && common->अग्र)
		;
		/*
		 * We are not doing octopus and not fast-forward.  Need
		 * a real merge.
		 */
	अन्यथा अगर (!remoteheads->अग्र && !common->अग्र && option_commit) {
		/*
		 * We are not doing octopus, not fast-forward, and have
		 * only one common.
		 */
		refresh_सूचकांक(the_repository->सूचकांक, REFRESH_QUIET, शून्य, शून्य, शून्य);
		अगर (allow_trivial && fast_forward != FF_ONLY) {
			/*
			 * Must first ensure that index matches HEAD before
			 * attempting a trivial merge.
			 */
			काष्ठा वृक्ष *शीर्ष_वृक्ष = repo_get_commit_वृक्ष(the_repository,
								      शीर्ष_commit);
			काष्ठा strbuf sb = STRBUF_INIT;

			अगर (repo_सूचकांक_has_changes(the_repository, शीर्ष_वृक्ष,
						   &sb)) {
				error(_("Your local changes to the following files would be overwritten by merge:\n  %s"),
				      sb.buf);
				strbuf_release(&sb);
				ret = 2;
				जाओ done;
			}

			/* See if it is really trivial. */
			git_committer_info(IDENT_STRICT);
			म_लिखो(_("Trying really trivial in-index merge...\n"));
			अगर (!read_वृक्ष_trivial(&common->item->object.oid,
					       &शीर्ष_commit->object.oid,
					       &remoteheads->item->object.oid)) {
				ret = merge_trivial(शीर्ष_commit, remoteheads);
				जाओ done;
			}
			म_लिखो(_("Nope.\n"));
		}
	} अन्यथा {
		/*
		 * An octopus.  If we can reach all the remote we are up
		 * to date.
		 */
		पूर्णांक up_to_date = 1;
		काष्ठा commit_list *j;

		क्रम (j = remoteheads; j; j = j->अग्र) {
			पूर्णांक ret = repo_in_merge_bases(the_repository,
						      j->item, शीर्ष_commit);
			अगर (ret < 0)
				निकास(128);
			अगर (!ret) {
				up_to_date = 0;
				अवरोध;
			}
		}
		अगर (up_to_date) {
			finish_up_to_date();
			जाओ done;
		}
	}

	अगर (fast_forward == FF_ONLY)
		die_ff_impossible();

	अगर (autostash)
		create_autostash_ref(the_repository, "MERGE_AUTOSTASH",
				     शून्य, false);

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
	अगर (save_state(&stash))
		oidclr(&stash, the_repository->hash_algo);

	क्रम (i = 0; i < use_strategies_nr; i++) {
		पूर्णांक ret, cnt;
		अगर (i) {
			म_लिखो(_("Rewinding the tree to pristine...\n"));
			restore_state(&शीर्ष_commit->object.oid, &stash);
		}
		अगर (use_strategies_nr != 1)
			म_लिखो(_("Trying merge strategy %s...\n"),
				use_strategies[i]->name);
		/*
		 * Remember which strategy left the state in the working
		 * tree.
		 */
		wt_strategy = use_strategies[i]->name;

		ret = try_merge_strategy(wt_strategy,
					 common, remoteheads,
					 शीर्ष_commit);
		/*
		 * The backend exits with 1 when conflicts are
		 * left to be resolved, with 2 when it does not
		 * handle the given merge at all.
		 */
		अगर (ret < 2) {
			अगर (!ret) {
				/*
				 * This strategy worked; no point in trying
				 * another.
				 */
				merge_was_ok = 1;
				best_strategy = wt_strategy;
				अवरोध;
			}
			cnt = (use_strategies_nr > 1) ? evaluate_परिणाम() : 0;
			अगर (best_cnt <= 0 || cnt <= best_cnt) {
				best_strategy = wt_strategy;
				best_cnt = cnt;
			}
		}
	}

	/*
	 * If we have a resulting tree, that means the strategy module
	 * auto resolved the merge cleanly.
	 */
	अगर (merge_was_ok && option_commit) {
		automerge_was_ok = 1;
		ret = finish_automerge(शीर्ष_commit, शीर्ष_subsumed,
				       common, remoteheads,
				       &परिणाम_वृक्ष, wt_strategy);
		जाओ done;
	}

	/*
	 * Pick the result from the best strategy and have the user fix
	 * it up.
	 */
	अगर (!best_strategy) {
		restore_state(&शीर्ष_commit->object.oid, &stash);
		अगर (use_strategies_nr > 1)
			ख_लिखो(मानक_त्रुटि,
				_("No merge strategy handled the merge.\n"));
		अन्यथा
			ख_लिखो(मानक_त्रुटि, _("Merge with strategy %s failed.\n"),
				use_strategies[0]->name);
		apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
				    शून्य, शून्य, शून्य, शून्य);
		ret = 2;
		जाओ done;
	} अन्यथा अगर (best_strategy == wt_strategy)
		; /* We already have its result in the working tree. */
	अन्यथा {
		म_लिखो(_("Rewinding the tree to pristine...\n"));
		restore_state(&शीर्ष_commit->object.oid, &stash);
		म_लिखो(_("Using the %s strategy to prepare resolving by hand.\n"),
			best_strategy);
		try_merge_strategy(best_strategy, common, remoteheads,
				   शीर्ष_commit);
	}

	अगर (squash) {
		finish(शीर्ष_commit, remoteheads, शून्य, शून्य);

		git_test_write_commit_graph_or_die(the_repository);
	} अन्यथा
		write_merge_state(remoteheads);

	अगर (merge_was_ok)
		ख_लिखो(मानक_त्रुटि, _("Automatic merge went well; "
			"stopped before committing as requested\n"));
	अन्यथा
		ret = suggest_conflicts();
	अगर (autostash)
		म_लिखो(_("When finished, apply stashed changes with `git stash pop`\n"));

done:
	अगर (!automerge_was_ok) {
		commit_list_free(common);
		commit_list_free(remoteheads);
	}
	strbuf_release(&buf);
	मुक्त(branch_to_free);
	मुक्त(argv_to_free);
	मुक्त(pull_twohead);
	मुक्त(pull_octopus);
	discard_सूचकांक(the_repository->सूचकांक);
	वापस ret;
}
