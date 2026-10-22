/*
 * Builtin "git commit"
 *
 * Copyright (c) 2007 Kristian Høgsberg <krh@redhat.com>
 * Based on git-commit.sh by Junio C Hamano and Linus Torvalds
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DISABLE_SIGN_COMPARE_WARNINGS

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "advice.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "lockfile.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cache-tree.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "color.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "dir.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "editor.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "diff.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "commit.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "add-interactive.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "revision.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "wt-status.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "run-command.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "strbuf.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-name.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "path.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "preload-index.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "read-cache.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "repository.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "string-list.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "rerere.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "unpack-trees.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "column.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "sequencer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "sparse-index.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "mailmap.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "help.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "commit-reach.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "commit-graph.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "pretty.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "trailer.h"

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ builtin_commit_usage[] = {
	N_("git commit [-a | --interactive | --patch] [-s] [-v] [-u[<mode>]] [--amend]\n"
	   "           [--dry-run] [(-c | -C | --squash) <commit> | --fixup [(amend|reword):]<commit>]\n"
	   "           [-F <file> | -m <msg>] [--reset-author] [--allow-empty]\n"
	   "           [--allow-empty-message] [--no-verify] [-e] [--author=<author>]\n"
	   "           [--date=<date>] [--cleanup=<mode>] [--[no-]status]\n"
	   "           [-i | -o] [--pathspec-from-file=<file> [--pathspec-file-nul]]\n"
	   "           [(--trailer <token>[(=|:)<value>])...] [-S[<keyid>]]\n"
	   "           [--] [<pathspec>...]"),
	ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ builtin_ਸਥਿਤੀ_usage[] = {
	N_("git status [<options>] [--] [<pathspec>...]"),
	ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ ਖਾਲੀ_ਥਾਵਾਂ_amend_advice[] =
N_("You asked to amend the most recent commit, but doing so would make\n"
"it empty. You can repeat your command with --allow-empty, or you can\n"
"remove the commit entirely with \"git reset HEAD^\".\n");

ਸਥਿਰ ਅਚਲ ਅੱਖਰ ਖਾਲੀ_ਥਾਵਾਂ_cherry_pick_advice[] =
N_("The previous cherry-pick is now empty, possibly due to conflict resolution.\n"
"If you wish to commit it anyway, use:\n"
"\n"
"    git commit --allow-empty\n"
"\n");

ਸਥਿਰ ਅਚਲ ਅੱਖਰ ਖਾਲੀ_ਥਾਵਾਂ_rebase_pick_advice[] =
N_("Otherwise, please use 'git rebase --skip'\n");

ਸਥਿਰ ਅਚਲ ਅੱਖਰ ਖਾਲੀ_ਥਾਵਾਂ_cherry_pick_advice_single[] =
N_("Otherwise, please use 'git cherry-pick --skip'\n");

ਸਥਿਰ ਅਚਲ ਅੱਖਰ ਖਾਲੀ_ਥਾਵਾਂ_cherry_pick_advice_multi[] =
N_("and then use:\n"
"\n"
"    git cherry-pick --continue\n"
"\n"
"to resume cherry-picking the remaining commits.\n"
"If you wish to skip this commit, use:\n"
"\n"
"    git cherry-pick --skip\n"
"\n");

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *color_ਸਥਿਤੀ_slots[] = {
	[WT_ਸਥਿਤੀ_ਸਿਰਲੇਖ]	  = "header",
	[WT_ਸਥਿਤੀ_UPDATED]	  = "updated",
	[WT_ਸਥਿਤੀ_ਬਦਲਿਆ]	  = "changed",
	[WT_ਸਥਿਤੀ_UNTRACKED]	  = "untracked",
	[WT_ਸਥਿਤੀ_NOBRANCH]	  = "noBranch",
	[WT_ਸਥਿਤੀ_UNMERGED]	  = "unmerged",
	[WT_ਸਥਿਤੀ_LOCAL_BRANCH]  = "localBranch",
	[WT_ਸਥਿਤੀ_REMOTE_BRANCH] = "remoteBranch",
	[WT_ਸਥਿਤੀ_ONBRANCH]	  = "branch",
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *use_ਸੁਨੇਹਾ_ਬਫਰ;
ਸਥਿਰ ਬਣਤਰ ਤਾਲਾ_file ਸੂਚਕਾਂਕ_ਤਾਲਾ; /* real index */
ਸਥਿਰ ਬਣਤਰ ਤਾਲਾ_file false_ਤਾਲਾ; /* used only for partial commits */
ਸਥਿਰ ਗਿਣਤੀ {
	COMMIT_AS_IS = 1,
	COMMIT_ਸਾਧਾਰਨ,
	COMMIT_PARTIAL
} commit_style;

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *force_author;
ਸਥਿਰ ਅੱਖਰ *logfile;
ਸਥਿਰ ਅੱਖਰ *template_file;
/*
 * The _message variables are commit names from which to take
 * the commit message and/or authorship.
 */
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *author_ਸੁਨੇਹਾ, *author_ਸੁਨੇਹਾ_ਬਫਰ;
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *edit_ਸੁਨੇਹਾ, *use_ਸੁਨੇਹਾ;
ਸਥਿਰ ਅੱਖਰ *fixup_ਸੁਨੇਹਾ, *fixup_commit, *squash_ਸੁਨੇਹਾ;
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *fixup_ਅਗੇਤਰ;
ਸਥਿਰ ਪੂਰਨ all, also, interactive, patch_interactive, only, amend, signoff;
ਸਥਿਰ ਬਣਤਰ interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
ਸਥਿਰ ਪੂਰਨ edit_flag = -1; /* unspecified */
ਸਥਿਰ ਪੂਰਨ quiet, verbose, no_verify, allow_ਖਾਲੀ_ਥਾਵਾਂ, dry_ਲੜੀਵਾਰ, renew_authorship;
ਸਥਿਰ ਪੂਰਨ config_commit_verbose = -1; /* unspecified */
ਸਥਿਰ ਪੂਰਨ no_post_rewrite, allow_ਖਾਲੀ_ਥਾਵਾਂ_ਸੁਨੇਹਾ, pathspec_file_nul;
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *untracked_files_arg, *force_date, *ignore_submodule_arg, *ignored_arg;
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *sign_commit, *pathspec_from_file;
ਸਥਿਰ ਬਣਤਰ strvec trailer_ਦਲੀਲਾਂ = STRVEC_INIT;

/*
 * The default commit message cleanup mode will remove the lines
 * beginning with # (shell comments) and leading and trailing
 * whitespaces (empty lines or containing only whitespaces)
 * if editor is used, and only the whitespaces if the message
 * is specified explicitly.
 */
ਸਥਿਰ ਗਿਣਤੀ commit_ਸੁਨੇਹਾ_cleanup_ਬਹੁਲਕ cleanup_ਬਹੁਲਕ;
ਸਥਿਰ ਅੱਖਰ *cleanup_config;

ਸਥਿਰ ਗਿਣਤੀ commit_whence whence;
ਸਥਿਰ ਪੂਰਨ use_editor = 1, include_ਸਥਿਤੀ = 1;
ਸਥਿਰ ਪੂਰਨ have_option_m;
ਸਥਿਰ ਬਣਤਰ strbuf ਸੁਨੇਹਾ_2 = STRBUF_INIT;

ਸਥਿਰ ਗਿਣਤੀ wt_ਸਥਿਤੀ_format ਸਥਿਤੀ_format = ਸਥਿਤੀ_FORMAT_UNSPECIFIED;

ਸਥਿਰ ਪੂਰਨ opt_parse_porcelain(ਅਚਲ ਬਣਤਰ option *opt, ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਗਿਣਤੀ wt_ਸਥਿਤੀ_format *ਮੁੱਲ = (ਗਿਣਤੀ wt_ਸਥਿਤੀ_format *)opt->ਮੁੱਲ;
	ਜੇ (unset)
		*ਮੁੱਲ = ਸਥਿਤੀ_FORMAT_NONE;
	ਨਹੀਂਤਾਂ ਜੇ (!arg)
		*ਮੁੱਲ = ਸਥਿਤੀ_FORMAT_PORCELAIN;
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(arg, "v1") || !strcmp(arg, "1"))
		*ਮੁੱਲ = ਸਥਿਤੀ_FORMAT_PORCELAIN;
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(arg, "v2") || !strcmp(arg, "2"))
		*ਮੁੱਲ = ਸਥਿਤੀ_FORMAT_PORCELAIN_V2;
	ਨਹੀਂਤਾਂ
		die("unsupported porcelain version '%s'", arg);

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ opt_parse_m(ਅਚਲ ਬਣਤਰ option *opt, ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਬਣਤਰ strbuf *buf = opt->ਮੁੱਲ;
	ਜੇ (unset) {
		have_option_m = 0;
		strbuf_setlen(buf, 0);
	} ਨਹੀਂਤਾਂ {
		have_option_m = 1;
		ਜੇ (buf->len)
			strbuf_addch(buf, '\n');
		strbuf_addstr(buf, arg);
		strbuf_complete_ਸਤਰ(buf);
	}
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ opt_parse_rename_score(ਅਚਲ ਬਣਤਰ option *opt, ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਅਚਲ ਅੱਖਰ **ਮੁੱਲ = opt->ਮੁੱਲ;

	BUG_ON_OPT_NEG(unset);

	ਜੇ (arg != ਨਿਰਾ && *arg == '=')
		arg = arg + 1;

	*ਮੁੱਲ = arg;
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ determine_whence(ਬਣਤਰ wt_ਸਥਿਤੀ *s)
{
	ਜੇ (file_exists(git_ਰਸਤਾ_ਮਿਲਾਓ_ਸਿਰ(the_repository)))
		whence = FROM_ਮਿਲਾਓ;
	ਨਹੀਂਤਾਂ ਜੇ (!sequencer_determine_whence(the_repository, &whence))
		whence = FROM_COMMIT;
	ਜੇ (s)
		s->whence = whence;
}

ਸਥਿਰ ਖਾਲੀ ਸਥਿਤੀ_init_config(ਬਣਤਰ wt_ਸਥਿਤੀ *s, config_fn_t fn)
{
	wt_ਸਥਿਤੀ_prepare(the_repository, s);
	init_diff_ui_defaults();
	repo_config(the_repository, fn, s);
	determine_whence(s);
	s->hints = advice_enabled(ADVICE_ਸਥਿਤੀ_HINTS); /* must come after repo_config() */
}

ਸਥਿਰ ਖਾਲੀ rollback_ਸੂਚਕਾਂਕ_files(ਖਾਲੀ)
{
	ਚੋਣ (commit_style) {
	ਹਾਲਤ COMMIT_AS_IS:
		ਰੋਕੋ; /* nothing to do */
	ਹਾਲਤ COMMIT_ਸਾਧਾਰਨ:
		rollback_ਤਾਲਾ_file(&ਸੂਚਕਾਂਕ_ਤਾਲਾ);
		ਰੋਕੋ;
	ਹਾਲਤ COMMIT_PARTIAL:
		rollback_ਤਾਲਾ_file(&ਸੂਚਕਾਂਕ_ਤਾਲਾ);
		rollback_ਤਾਲਾ_file(&false_ਤਾਲਾ);
		ਰੋਕੋ;
	}
}

ਸਥਿਰ ਪੂਰਨ commit_ਸੂਚਕਾਂਕ_files(ਖਾਲੀ)
{
	ਪੂਰਨ err = 0;

	ਚੋਣ (commit_style) {
	ਹਾਲਤ COMMIT_AS_IS:
		ਰੋਕੋ; /* nothing to do */
	ਹਾਲਤ COMMIT_ਸਾਧਾਰਨ:
		err = commit_ਤਾਲਾ_file(&ਸੂਚਕਾਂਕ_ਤਾਲਾ);
		ਰੋਕੋ;
	ਹਾਲਤ COMMIT_PARTIAL:
		err = commit_ਤਾਲਾ_file(&ਸੂਚਕਾਂਕ_ਤਾਲਾ);
		rollback_ਤਾਲਾ_file(&false_ਤਾਲਾ);
		ਰੋਕੋ;
	}

	ਵਾਪਸ err;
}

/*
 * Take a union of paths in the index and the named tree (typically, "HEAD"),
 * and return the paths that match the given pattern in list.
 */
ਸਥਿਰ ਪੂਰਨ list_paths(ਬਣਤਰ string_list *list, ਅਚਲ ਅੱਖਰ *with_ਰੁੱਖ,
		      ਅਚਲ ਬਣਤਰ pathspec *ਨਮੂਨਾ)
{
	ਪੂਰਨ i, ret;
	ਅੱਖਰ *m;

	ਜੇ (!ਨਮੂਨਾ->nr)
		ਵਾਪਸ 0;

	m = xcalloc(1, ਨਮੂਨਾ->nr);

	ਜੇ (with_ਰੁੱਖ) {
		ਅੱਖਰ *max_ਅਗੇਤਰ = common_ਅਗੇਤਰ(ਨਮੂਨਾ);
		overlay_ਰੁੱਖ_on_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ, with_ਰੁੱਖ, max_ਅਗੇਤਰ);
		ਮੁਕਤ(max_ਅਗੇਤਰ);
	}

	/* TODO: audit for interaction with sparse-index. */
	ensure_ਭਰਿਆ_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ);
	ਲਈ (i = 0; i < the_repository->ਸੂਚਕਾਂਕ->cache_nr; i++) {
		ਅਚਲ ਬਣਤਰ cache_ਇੰਦਰਾਜ *ce = the_repository->ਸੂਚਕਾਂਕ->cache[i];
		ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ;

		ਜੇ (ce->ce_flags & CE_UPDATE)
			ਜਾਰੀ;
		ਜੇ (!ce_ਰਸਤਾ_ਮੇਲ(the_repository->ਸੂਚਕਾਂਕ, ce, ਨਮੂਨਾ, m))
			ਜਾਰੀ;
		ਵਸਤੂ = string_list_ਪਾਓ(list, ce->name);
		ਜੇ (ce_skip_worktree(ce))
			ਵਸਤੂ->util = ਵਸਤੂ; /* better a valid pointer than a fake one */
	}

	ret = report_ਰਸਤਾ_error(m, ਨਮੂਨਾ);
	ਮੁਕਤ(m);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਖਾਲੀ ਜੋੜੋ_remove_files(ਬਣਤਰ string_list *list)
{
	ਪੂਰਨ i;
	ਲਈ (i = 0; i < list->nr; i++) {
		ਬਣਤਰ stat st;
		ਬਣਤਰ string_list_ਵਸਤੂ *p = &(list->ਚੀਜ਼ਾਂ[i]);

		/* p->util is skip-worktree */
		ਜੇ (p->util)
			ਜਾਰੀ;

		ਜੇ (!lstat(p->string, &st)) {
			ਜੇ (ਜੋੜੋ_ਮੰਜ਼ਿਲ_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ, p->string, &st, 0))
				die(_("updating files failed"));
		} ਨਹੀਂਤਾਂ
			remove_file_from_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ, p->string);
	}
}

ਸਥਿਰ ਖਾਲੀ create_ਅਧਾਰ_ਸੂਚਕਾਂਕ(ਅਚਲ ਬਣਤਰ commit *current_ਸਿਰ)
{
	ਬਣਤਰ ਰੁੱਖ *ਰੁੱਖ;
	ਬਣਤਰ unpack_trees_options opts;
	ਬਣਤਰ ਰੁੱਖ_desc t;

	ਜੇ (!current_ਸਿਰ) {
		discard_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ);
		ਵਾਪਸ;
	}

	memset(&opts, 0, ਆਕਾਰ(opts));
	opts.ਸਿਰ_idx = 1;
	opts.ਸੂਚਕਾਂਕ_only = 1;
	opts.ਮਿਲਾਓ = 1;
	opts.src_ਸੂਚਕਾਂਕ = the_repository->ਸੂਚਕਾਂਕ;
	opts.dst_ਸੂਚਕਾਂਕ = the_repository->ਸੂਚਕਾਂਕ;

	opts.fn = oneway_ਮਿਲਾਓ;
	ਰੁੱਖ = repo_parse_ਰੁੱਖ_ਅਸਿੱਧਾ(the_repository,
					&current_ਸਿਰ->object.oid);
	ਜੇ (!ਰੁੱਖ)
		die(_("failed to unpack HEAD tree object"));
	ਜੇ (repo_parse_ਰੁੱਖ(the_repository, ਰੁੱਖ) < 0)
		ਬਾਹਰ_ਨਿਕਲੋ(128);
	init_ਰੁੱਖ_desc(&t, &ਰੁੱਖ->object.oid, ਰੁੱਖ->ਬਫਰ, ਰੁੱਖ->ਮਾਪ);
	ਜੇ (unpack_trees(1, &t, &opts))
		ਬਾਹਰ_ਨਿਕਲੋ(128); /* We've already reported the error, finish dying */
}

ਸਥਿਰ ਖਾਲੀ refresh_cache_or_die(ਪੂਰਨ refresh_flags)
{
	/*
	 * refresh_flags contains REFRESH_QUIET, so the only errors
	 * are for unmerged entries.
	 */
	ਜੇ (refresh_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ, refresh_flags | REFRESH_ਵਿੱਚ_PORCELAIN, ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ))
		die_resolve_conflict("commit");
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *prepare_ਸੂਚਕਾਂਕ(ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
				 ਅਚਲ ਬਣਤਰ commit *current_ਸਿਰ, ਪੂਰਨ is_ਸਥਿਤੀ)
{
	ਬਣਤਰ string_list partial = STRING_LIST_INIT_DUP;
	ਬਣਤਰ pathspec pathspec;
	ਪੂਰਨ refresh_flags = REFRESH_QUIET;
	ਅਚਲ ਅੱਖਰ *ret;
	ਅੱਖਰ *ਰਸਤਾ = ਨਿਰਾ;

	ਜੇ (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	ਜੇ (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	ਜੇ (is_ਸਥਿਤੀ)
		refresh_flags |= REFRESH_UNMERGED;
	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_ਭਰਿਆ,
		       ਅਗੇਤਰ, argv);

	ਜੇ (pathspec_from_file) {
		ਜੇ (interactive)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--interactive/--patch");

		ਜੇ (all)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "-a");

		ਜੇ (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_ਭਰਿਆ,
				    ਅਗੇਤਰ, pathspec_from_file, pathspec_file_nul);
	} ਨਹੀਂਤਾਂ ਜੇ (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	ਜੇ (!pathspec.nr && (also || (only && !allow_ਖਾਲੀ_ਥਾਵਾਂ &&
	    (!amend || (fixup_ਸੁਨੇਹਾ && strcmp(fixup_ਅਗੇਤਰ, "amend"))))))
		die(_("No paths with --include/--only does not make sense."));

	ਜੇ (repo_read_ਸੂਚਕਾਂਕ_preload(the_repository, &pathspec, 0) < 0)
		die(_("index file corrupt"));

	ਜੇ (interactive) {
		ਅੱਖਰ *old_ਸੂਚਕਾਂਕ_env = ਨਿਰਾ, *old_repo_ਸੂਚਕਾਂਕ_file;
		repo_hold_locked_ਸੂਚਕਾਂਕ(the_repository, &ਸੂਚਕਾਂਕ_ਤਾਲਾ,
				       ਤਾਲਾ_DIE_ON_ERROR);

		refresh_cache_or_die(refresh_flags);

		ਜੇ (write_locked_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ, &ਸੂਚਕਾਂਕ_ਤਾਲਾ, 0))
			die(_("unable to create temporary index"));

		old_repo_ਸੂਚਕਾਂਕ_file = the_repository->ਸੂਚਕਾਂਕ_file;
		the_repository->ਸੂਚਕਾਂਕ_file =
			(ਅੱਖਰ *)get_ਤਾਲਾ_file_ਰਸਤਾ(&ਸੂਚਕਾਂਕ_ਤਾਲਾ);
		old_ਸੂਚਕਾਂਕ_env = xstrdup_or_null(getenv(ਸੂਚਕਾਂਕ_ENVIRONMENT));
		setenv(ਸੂਚਕਾਂਕ_ENVIRONMENT, the_repository->ਸੂਚਕਾਂਕ_file, 1);

		ਜੇ (interactive_ਜੋੜੋ(the_repository, argv, ਅਗੇਤਰ, patch_interactive, &interactive_opts) != 0)
			die(_("interactive add failed"));

		the_repository->ਸੂਚਕਾਂਕ_file = old_repo_ਸੂਚਕਾਂਕ_file;
		ਜੇ (old_ਸੂਚਕਾਂਕ_env && *old_ਸੂਚਕਾਂਕ_env)
			setenv(ਸੂਚਕਾਂਕ_ENVIRONMENT, old_ਸੂਚਕਾਂਕ_env, 1);
		ਨਹੀਂਤਾਂ
			unsetenv(ਸੂਚਕਾਂਕ_ENVIRONMENT);
		FREE_AND_NULL(old_ਸੂਚਕਾਂਕ_env);

		discard_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ);
		read_ਸੂਚਕਾਂਕ_from(the_repository->ਸੂਚਕਾਂਕ, get_ਤਾਲਾ_file_ਰਸਤਾ(&ਸੂਚਕਾਂਕ_ਤਾਲਾ),
				repo_get_git_dir(the_repository));
		ਜੇ (cache_ਰੁੱਖ_update(the_repository->ਸੂਚਕਾਂਕ, WRITE_ਰੁੱਖ_SILENT) == 0) {
			ਜੇ (reopen_ਤਾਲਾ_file(&ਸੂਚਕਾਂਕ_ਤਾਲਾ) < 0)
				die(_("unable to write index file"));
			ਜੇ (write_locked_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ, &ਸੂਚਕਾਂਕ_ਤਾਲਾ, 0))
				die(_("unable to update temporary index"));
		} ਨਹੀਂਤਾਂ
			warning(_("Failed to update main cache tree"));

		commit_style = COMMIT_ਸਾਧਾਰਨ;
		ret = get_ਤਾਲਾ_file_ਰਸਤਾ(&ਸੂਚਕਾਂਕ_ਤਾਲਾ);
		ਜਾਓ ਬਾਹਰਲਾ;
	} ਨਹੀਂਤਾਂ {
		ਜੇ (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--interactive/--patch");
		ਜੇ (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--interactive/--patch");
	}

	/*
	 * Non partial, non as-is commit.
	 *
	 * (1) get the real index;
	 * (2) update the_index as necessary;
	 * (3) write the_index out to the real index (still locked);
	 * (4) return the name of the locked index file.
	 *
	 * The caller should run hooks on the locked real index, and
	 * (A) if all goes well, commit the real index;
	 * (B) on failure, rollback the real index.
	 */
	ਜੇ (all || (also && pathspec.nr)) {
		ਅੱਖਰ *ps_matched = xcalloc(pathspec.nr, 1);
		repo_hold_locked_ਸੂਚਕਾਂਕ(the_repository, &ਸੂਚਕਾਂਕ_ਤਾਲਾ,
				       ਤਾਲਾ_DIE_ON_ERROR);
		ਜੋੜੋ_files_ਮੰਜ਼ਿਲ_cache(the_repository, also ? ਅਗੇਤਰ : ਨਿਰਾ,
				   &pathspec, ps_matched, 0, 0, 0 );
		ਜੇ (!all && report_ਰਸਤਾ_error(ps_matched, &pathspec))
			ਬਾਹਰ_ਨਿਕਲੋ(128);

		refresh_cache_or_die(refresh_flags);
		cache_ਰੁੱਖ_update(the_repository->ਸੂਚਕਾਂਕ, WRITE_ਰੁੱਖ_SILENT);
		ਜੇ (write_locked_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ, &ਸੂਚਕਾਂਕ_ਤਾਲਾ, 0))
			die(_("unable to write new index file"));
		commit_style = COMMIT_ਸਾਧਾਰਨ;
		ret = get_ਤਾਲਾ_file_ਰਸਤਾ(&ਸੂਚਕਾਂਕ_ਤਾਲਾ);
		ਮੁਕਤ(ps_matched);
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	/*
	 * As-is commit.
	 *
	 * (1) return the name of the real index file.
	 *
	 * The caller should run hooks on the real index,
	 * and create commit from the_index.
	 * We still need to refresh the index here.
	 */
	ਜੇ (!only && !pathspec.nr) {
		repo_hold_locked_ਸੂਚਕਾਂਕ(the_repository, &ਸੂਚਕਾਂਕ_ਤਾਲਾ,
				       ਤਾਲਾ_DIE_ON_ERROR);
		refresh_cache_or_die(refresh_flags);
		ਜੇ (the_repository->ਸੂਚਕਾਂਕ->cache_ਬਦਲਿਆ
		    || !cache_ਰੁੱਖ_fully_valid(the_repository->ਸੂਚਕਾਂਕ))
			cache_ਰੁੱਖ_update(the_repository->ਸੂਚਕਾਂਕ, WRITE_ਰੁੱਖ_SILENT);
		ਜੇ (write_locked_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ, &ਸੂਚਕਾਂਕ_ਤਾਲਾ,
				       COMMIT_ਤਾਲਾ | SKIP_IF_UNCHANGED))
			die(_("unable to write new index file"));
		commit_style = COMMIT_AS_IS;
		ret = repo_get_ਸੂਚਕਾਂਕ_file(the_repository);
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	/*
	 * A partial commit.
	 *
	 * (0) find the set of affected paths;
	 * (1) get lock on the real index file;
	 * (2) update the_index with the given paths;
	 * (3) write the_index out to the real index (still locked);
	 * (4) get lock on the false index file;
	 * (5) reset the_index from HEAD;
	 * (6) update the_index the same way as (2);
	 * (7) write the_index out to the false index file;
	 * (8) return the name of the false index file (still locked);
	 *
	 * The caller should run hooks on the locked false index, and
	 * create commit from it.  Then
	 * (A) if all goes well, commit the real index;
	 * (B) on failure, rollback the real index;
	 * In either case, rollback the false index.
	 */
	commit_style = COMMIT_PARTIAL;

	ਜੇ (whence != FROM_COMMIT) {
		ਜੇ (whence == FROM_ਮਿਲਾਓ)
			die(_("cannot do a partial commit during a merge."));
		ਨਹੀਂਤਾਂ ਜੇ (is_from_cherry_pick(whence))
			die(_("cannot do a partial commit during a cherry-pick."));
		ਨਹੀਂਤਾਂ ਜੇ (is_from_rebase(whence))
			die(_("cannot do a partial commit during a rebase."));
	}

	ਜੇ (list_paths(&partial, !current_ਸਿਰ ? ਨਿਰਾ : "HEAD", &pathspec))
		ਬਾਹਰ_ਨਿਕਲੋ(1);

	discard_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ);
	ਜੇ (repo_read_ਸੂਚਕਾਂਕ(the_repository) < 0)
		die(_("cannot read the index"));

	repo_hold_locked_ਸੂਚਕਾਂਕ(the_repository, &ਸੂਚਕਾਂਕ_ਤਾਲਾ, ਤਾਲਾ_DIE_ON_ERROR);
	ਜੋੜੋ_remove_files(&partial);
	refresh_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ, REFRESH_QUIET, ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ);
	cache_ਰੁੱਖ_update(the_repository->ਸੂਚਕਾਂਕ, WRITE_ਰੁੱਖ_SILENT);
	ਜੇ (write_locked_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ, &ਸੂਚਕਾਂਕ_ਤਾਲਾ, 0))
		die(_("unable to write new index file"));

	ਰਸਤਾ = repo_git_ਰਸਤਾ(the_repository, "next-index-%"PRIuMAX,
			     (uintmax_t) getpid());
	hold_ਤਾਲਾ_file_for_update(&false_ਤਾਲਾ, ਰਸਤਾ,
				  ਤਾਲਾ_DIE_ON_ERROR);

	create_ਅਧਾਰ_ਸੂਚਕਾਂਕ(current_ਸਿਰ);
	ਜੋੜੋ_remove_files(&partial);
	refresh_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ, REFRESH_QUIET, ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ);

	ਜੇ (write_locked_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ, &false_ਤਾਲਾ, 0))
		die(_("unable to write temporary index file"));

	discard_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ);
	ret = get_ਤਾਲਾ_file_ਰਸਤਾ(&false_ਤਾਲਾ);
	read_ਸੂਚਕਾਂਕ_from(the_repository->ਸੂਚਕਾਂਕ, ret, repo_get_git_dir(the_repository));
ਬਾਹਰਲਾ:
	string_list_clear(&partial, 0);
	clear_pathspec(&pathspec);
	ਮੁਕਤ(ਰਸਤਾ);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ ਲੜੀਵਾਰ_ਸਥਿਤੀ(FILE *fp, ਅਚਲ ਅੱਖਰ *ਸੂਚਕਾਂਕ_file, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ, ਪੂਰਨ nowarn,
		      ਬਣਤਰ wt_ਸਥਿਤੀ *s)
{
	ਬਣਤਰ object_id oid;

	ਜੇ (s->relative_paths)
		s->ਅਗੇਤਰ = ਅਗੇਤਰ;

	ਜੇ (amend) {
		s->amend = 1;
		s->reference = "HEAD^1";
	}
	s->verbose = verbose;
	s->ਸੂਚਕਾਂਕ_file = ਸੂਚਕਾਂਕ_file;
	s->fp = fp;
	s->nowarn = nowarn;
	s->is_initial = repo_get_oid(the_repository, s->reference, &oid) ? 1 : 0;
	ਜੇ (!s->is_initial)
		oidcpy(&s->oid_commit, &oid);
	s->ਸਥਿਤੀ_format = ਸਥਿਤੀ_format;
	s->ignore_submodule_arg = ignore_submodule_arg;

	wt_ਸਥਿਤੀ_collect(s);
	wt_ਸਥਿਤੀ_print(s);
	wt_ਸਥਿਤੀ_collect_free_buffers(s);

	ਵਾਪਸ s->committable;
}

ਸਥਿਰ ਪੂਰਨ is_a_ਮਿਲਾਓ(ਅਚਲ ਬਣਤਰ commit *current_ਸਿਰ)
{
	ਵਾਪਸ !!(current_ਸਿਰ->parents && current_ਸਿਰ->parents->ਅਗਲਾ);
}

ਸਥਿਰ ਖਾਲੀ assert_ਵੰਡੋ_ident(ਬਣਤਰ ident_ਵੰਡੋ *id, ਅਚਲ ਬਣਤਰ strbuf *buf)
{
	ਜੇ (ਵੰਡੋ_ident_ਸਤਰ(id, buf->buf, buf->len) || !id->date_begin)
		BUG("unable to parse our own ident: %s", buf->buf);
}

ਸਥਿਰ ਖਾਲੀ export_one(ਅਚਲ ਅੱਖਰ *var, ਅਚਲ ਅੱਖਰ *s, ਅਚਲ ਅੱਖਰ *e, ਪੂਰਨ hack)
{
	ਬਣਤਰ strbuf buf = STRBUF_INIT;
	ਜੇ (hack)
		strbuf_addch(&buf, hack);
	strbuf_ਜੋੜੋ(&buf, s, e - s);
	setenv(var, buf.buf, 1);
	strbuf_release(&buf);
}

ਸਥਿਰ ਪੂਰਨ parse_force_date(ਅਚਲ ਅੱਖਰ *ਵਿੱਚ, ਬਣਤਰ strbuf *ਬਾਹਰਲਾ)
{
	strbuf_addch(ਬਾਹਰਲਾ, '@');

	ਜੇ (parse_date(ਵਿੱਚ, ਬਾਹਰਲਾ) < 0) {
		ਪੂਰਨ errors = 0;
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ t = approxidate_careful(ਵਿੱਚ, &errors);
		ਜੇ (errors)
			ਵਾਪਸ -1;
		strbuf_addf(ਬਾਹਰਲਾ, "%lu", t);
	}

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ ਸੈੱਟ_ident_var(ਅੱਖਰ **buf, ਅੱਖਰ *val)
{
	ਮੁਕਤ(*buf);
	*buf = val;
}

ਸਥਿਰ ਖਾਲੀ determine_author_info(ਬਣਤਰ strbuf *author_ident)
{
	ਅੱਖਰ *name, *email, *date;
	ਬਣਤਰ ident_ਵੰਡੋ author;

	name = xstrdup_or_null(getenv("GIT_AUTHOR_NAME"));
	email = xstrdup_or_null(getenv("GIT_AUTHOR_EMAIL"));
	date = xstrdup_or_null(getenv("GIT_AUTHOR_DATE"));

	ਜੇ (author_ਸੁਨੇਹਾ) {
		ਬਣਤਰ ident_ਵੰਡੋ ident;
		ਮਾਪ_t len;
		ਅਚਲ ਅੱਖਰ *a;

		a = ਲੱਭੋ_commit_ਸਿਰਲੇਖ(author_ਸੁਨੇਹਾ_ਬਫਰ, "author", &len);
		ਜੇ (!a)
			die(_("commit '%s' lacks author header"), author_ਸੁਨੇਹਾ);
		ਜੇ (ਵੰਡੋ_ident_ਸਤਰ(&ident, a, len) < 0)
			die(_("commit '%s' has malformed author line"), author_ਸੁਨੇਹਾ);

		ਸੈੱਟ_ident_var(&name, xmemdupz(ident.name_begin, ident.name_ਸਿਰਾ - ident.name_begin));
		ਸੈੱਟ_ident_var(&email, xmemdupz(ident.mail_begin, ident.mail_ਸਿਰਾ - ident.mail_begin));

		ਜੇ (ident.date_begin) {
			ਬਣਤਰ strbuf date_buf = STRBUF_INIT;
			strbuf_addch(&date_buf, '@');
			strbuf_ਜੋੜੋ(&date_buf, ident.date_begin, ident.date_ਸਿਰਾ - ident.date_begin);
			strbuf_addch(&date_buf, ' ');
			strbuf_ਜੋੜੋ(&date_buf, ident.tz_begin, ident.tz_ਸਿਰਾ - ident.tz_begin);
			ਸੈੱਟ_ident_var(&date, strbuf_detach(&date_buf, ਨਿਰਾ));
		}
	}

	ਜੇ (force_author) {
		ਬਣਤਰ ident_ਵੰਡੋ ident;

		ਜੇ (ਵੰਡੋ_ident_ਸਤਰ(&ident, force_author, ਲੜੀ_ਲੰਬਾਈ(force_author)) < 0)
			die(_("malformed --author parameter"));
		ਸੈੱਟ_ident_var(&name, xmemdupz(ident.name_begin, ident.name_ਸਿਰਾ - ident.name_begin));
		ਸੈੱਟ_ident_var(&email, xmemdupz(ident.mail_begin, ident.mail_ਸਿਰਾ - ident.mail_begin));
	}

	ਜੇ (force_date) {
		ਬਣਤਰ strbuf date_buf = STRBUF_INIT;
		ਜੇ (parse_force_date(force_date, &date_buf))
			die(_("invalid date format: %s"), force_date);
		ਸੈੱਟ_ident_var(&date, strbuf_detach(&date_buf, ਨਿਰਾ));
	}

	strbuf_addstr(author_ident, fmt_ident(name, email, WANT_AUTHOR_IDENT, date,
				IDENT_STRICT));
	assert_ਵੰਡੋ_ident(&author, author_ident);
	export_one("GIT_AUTHOR_NAME", author.name_begin, author.name_ਸਿਰਾ, 0);
	export_one("GIT_AUTHOR_EMAIL", author.mail_begin, author.mail_ਸਿਰਾ, 0);
	export_one("GIT_AUTHOR_DATE", author.date_begin, author.tz_ਸਿਰਾ, '@');
	ਮੁਕਤ(name);
	ਮੁਕਤ(email);
	ਮੁਕਤ(date);
}

ਸਥਿਰ ਪੂਰਨ author_date_is_interesting(ਖਾਲੀ)
{
	ਵਾਪਸ author_ਸੁਨੇਹਾ || force_date;
}

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ WITH_BREAKING_CHANGES
ਸਥਿਰ ਖਾਲੀ adjust_comment_ਸਤਰ_char(ਅਚਲ ਬਣਤਰ strbuf *sb)
{
	ਅੱਖਰ candidates[] = "#;@!$%^&|:";
	ਅੱਖਰ *candidate;
	ਅਚਲ ਅੱਖਰ *p;
	ਮਾਪ_t cutoff;

	/* Ignore comment chars in trailing comments (e.g., Conflicts:) */
	cutoff = sb->len - ignored_log_ਸੁਨੇਹਾ_ਬਾਈਟਾਂ(sb->buf, sb->len);

	ਜੇ (!memchr(sb->buf, candidates[0], sb->len)) {
		ਮੁਕਤ(comment_ਸਤਰ_str_ਮੰਜ਼ਿਲ_free);
		comment_ਸਤਰ_str = comment_ਸਤਰ_str_ਮੰਜ਼ਿਲ_free =
			xstrfmt("%c", candidates[0]);
		ਵਾਪਸ;
	}

	p = sb->buf;
	candidate = strchr(candidates, *p);
	ਜੇ (candidate)
		*candidate = ' ';
	ਲਈ (p = sb->buf; p + 1 < sb->buf + cutoff; p++) {
		ਜੇ ((p[0] == '\n' || p[0] == '\r') && p[1]) {
			candidate = strchr(candidates, p[1]);
			ਜੇ (candidate)
				*candidate = ' ';
		}
	}

	ਲਈ (p = candidates; *p == ' '; p++)
		;
	ਜੇ (!*p)
		die(_("unable to select a comment character that is not used\n"
		      "in the current commit message"));
	ਮੁਕਤ(comment_ਸਤਰ_str_ਮੰਜ਼ਿਲ_free);
	comment_ਸਤਰ_str = comment_ਸਤਰ_str_ਮੰਜ਼ਿਲ_free = xstrfmt("%c", *p);
}
#ਜੇ_ਅੰਤ /* !WITH_BREAKING_CHANGES */

ਸਥਿਰ ਖਾਲੀ prepare_amend_commit(ਬਣਤਰ commit *commit, ਬਣਤਰ strbuf *sb,
				ਬਣਤਰ pretty_print_context *ctx)
{
	ਅਚਲ ਅੱਖਰ *ਬਫਰ, *subject, *fmt;

	ਬਫਰ = repo_get_commit_ਬਫਰ(the_repository, commit, ਨਿਰਾ);
	ਲੱਭੋ_commit_subject(ਬਫਰ, &subject);
	/*
	 * If we amend the 'amend!' commit then we don't want to
	 * duplicate the subject line.
	 */
	fmt = starts_with(subject, "amend!") ? "%b" : "%B";
	repo_format_commit_ਸੁਨੇਹਾ(the_repository, commit, fmt, sb, ctx);
	repo_unuse_commit_ਬਫਰ(the_repository, commit, ਬਫਰ);
}

ਸਥਿਰ ਖਾਲੀ change_ਅੰਕੜੇ_free(ਖਾਲੀ *util, ਅਚਲ ਅੱਖਰ *str ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ wt_ਸਥਿਤੀ_change_ਅੰਕੜੇ *d = util;
	ਮੁਕਤ(d->rename_ਸਰੋਤ);
	ਮੁਕਤ(d);
}

ਸਥਿਰ ਪੂਰਨ prepare_ਮੰਜ਼ਿਲ_commit(ਅਚਲ ਅੱਖਰ *ਸੂਚਕਾਂਕ_file, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			     ਬਣਤਰ commit *current_ਸਿਰ,
			     ਬਣਤਰ wt_ਸਥਿਤੀ *s,
			     ਬਣਤਰ strbuf *author_ident)
{
	ਬਣਤਰ stat statbuf;
	ਬਣਤਰ strbuf committer_ident = STRBUF_INIT;
	ਪੂਰਨ committable;
	ਬਣਤਰ strbuf sb = STRBUF_INIT;
	ਅਚਲ ਅੱਖਰ *hook_arg1 = ਨਿਰਾ;
	ਅਚਲ ਅੱਖਰ *hook_arg2 = ਨਿਰਾ;
	ਪੂਰਨ clean_ਸੁਨੇਹਾ_contents = (cleanup_ਬਹੁਲਕ != COMMIT_ਸੁਨੇਹਾ_CLEANUP_NONE);
	ਪੂਰਨ old_display_comment_ਅਗੇਤਰ;
	ਪੂਰਨ invoked_hook;

	/* This checks and barfs if author is badly specified */
	determine_author_info(author_ident);

	ਜੇ (!no_verify && ਲੜੀਵਾਰ_commit_hook(use_editor, ਸੂਚਕਾਂਕ_file, &invoked_hook,
					  "pre-commit", ਨਿਰਾ))
		ਵਾਪਸ 0;

	ਜੇ (squash_ਸੁਨੇਹਾ) {
		/*
		 * Insert the proper subject line before other commit
		 * message options add their content.
		 */
		ਜੇ (use_ਸੁਨੇਹਾ && !strcmp(use_ਸੁਨੇਹਾ, squash_ਸੁਨੇਹਾ))
			strbuf_addstr(&sb, "squash! ");
		ਨਹੀਂਤਾਂ {
			ਬਣਤਰ pretty_print_context ctx = {0};
			ਬਣਤਰ commit *c;
			c = lookup_commit_reference_by_name(squash_ਸੁਨੇਹਾ);
			ਜੇ (!c)
				die(_("could not lookup commit '%s'"), squash_ਸੁਨੇਹਾ);
			ctx.output_encoding = get_commit_output_encoding();
			repo_format_commit_ਸੁਨੇਹਾ(the_repository, c,
						   "squash! %s\n\n", &sb,
						   &ctx);
		}
	}

	ਜੇ (have_option_m && !fixup_ਸੁਨੇਹਾ) {
		strbuf_addbuf(&sb, &ਸੁਨੇਹਾ_2);
		hook_arg1 = "message";
	} ਨਹੀਂਤਾਂ ਜੇ (logfile && !strcmp(logfile, "-")) {
		ਜੇ (isatty(0))
			fprintf(stderr, _("(reading log message from standard input)\n"));
		ਜੇ (strbuf_read(&sb, 0, 0) < 0)
			die_errno(_("could not read log from standard input"));
		hook_arg1 = "message";
	} ਨਹੀਂਤਾਂ ਜੇ (logfile) {
		ਜੇ (strbuf_read_file(&sb, logfile, 0) < 0)
			die_errno(_("could not read log file '%s'"),
				  logfile);
		hook_arg1 = "message";
	} ਨਹੀਂਤਾਂ ਜੇ (use_ਸੁਨੇਹਾ) {
		ਅਚਲ ਅੱਖਰ *ਬਫਰ;
		ਬਫਰ = ਲੜੀ_ਖੋਜ(use_ਸੁਨੇਹਾ_ਬਫਰ, "\n\n");
		ਜੇ (ਬਫਰ)
			strbuf_addstr(&sb, skip_blank_ਸਤਰਾਂ(ਬਫਰ + 2));
		hook_arg1 = "commit";
		hook_arg2 = use_ਸੁਨੇਹਾ;
	} ਨਹੀਂਤਾਂ ਜੇ (fixup_ਸੁਨੇਹਾ) {
		ਬਣਤਰ pretty_print_context ctx = {0};
		ਬਣਤਰ commit *commit;
		ਅੱਖਰ *fmt;
		commit = lookup_commit_reference_by_name(fixup_commit);
		ਜੇ (!commit)
			die(_("could not lookup commit '%s'"), fixup_commit);
		ctx.output_encoding = get_commit_output_encoding();
		fmt = xstrfmt("%s! %%s\n\n", fixup_ਅਗੇਤਰ);
		repo_format_commit_ਸੁਨੇਹਾ(the_repository, commit, fmt, &sb,
					   &ctx);
		ਮੁਕਤ(fmt);
		hook_arg1 = "message";

		/*
		 * Only `-m` commit message option is checked here, as
		 * it supports `--fixup` to append the commit message.
		 *
		 * The other commit message options `-c`/`-C`/`-F` are
		 * incompatible with all the forms of `--fixup` and
		 * have already errored out while parsing the `git commit`
		 * options.
		 */
		ਜੇ (have_option_m && !strcmp(fixup_ਅਗੇਤਰ, "fixup"))
			strbuf_addbuf(&sb, &ਸੁਨੇਹਾ_2);

		ਜੇ (!strcmp(fixup_ਅਗੇਤਰ, "amend")) {
			ਜੇ (have_option_m)
				die(_("options '%s' and '%s:%s' cannot be used together"), "-m", "--fixup", fixup_ਸੁਨੇਹਾ);
			prepare_amend_commit(commit, &sb, &ctx);
		}
	} ਨਹੀਂਤਾਂ ਜੇ (!stat(git_ਰਸਤਾ_ਮਿਲਾਓ_ਸੁਨੇਹਾ(the_repository), &statbuf)) {
		ਮਾਪ_t ਮਿਲਾਓ_ਸੁਨੇਹਾ_ਸ਼ੁਰੂ;

		/*
		 * prepend SQUASH_MSG here if it exists and a
		 * "merge --squash" was originally performed
		 */
		ਜੇ (!stat(git_ਰਸਤਾ_squash_ਸੁਨੇਹਾ(the_repository), &statbuf)) {
			ਜੇ (strbuf_read_file(&sb, git_ਰਸਤਾ_squash_ਸੁਨੇਹਾ(the_repository), 0) < 0)
				die_errno(_("could not read SQUASH_MSG"));
			hook_arg1 = "squash";
		} ਨਹੀਂਤਾਂ
			hook_arg1 = "merge";

		ਮਿਲਾਓ_ਸੁਨੇਹਾ_ਸ਼ੁਰੂ = sb.len;
		ਜੇ (strbuf_read_file(&sb, git_ਰਸਤਾ_ਮਿਲਾਓ_ਸੁਨੇਹਾ(the_repository), 0) < 0)
			die_errno(_("could not read MERGE_MSG"));

		ਜੇ (cleanup_ਬਹੁਲਕ == COMMIT_ਸੁਨੇਹਾ_CLEANUP_SCISSORS &&
		    wt_ਸਥਿਤੀ_locate_ਸਿਰਾ(sb.buf + ਮਿਲਾਓ_ਸੁਨੇਹਾ_ਸ਼ੁਰੂ,
					 sb.len - ਮਿਲਾਓ_ਸੁਨੇਹਾ_ਸ਼ੁਰੂ) <
				sb.len - ਮਿਲਾਓ_ਸੁਨੇਹਾ_ਸ਼ੁਰੂ)
			s->added_cut_ਸਤਰ = 1;
	} ਨਹੀਂਤਾਂ ਜੇ (!stat(git_ਰਸਤਾ_squash_ਸੁਨੇਹਾ(the_repository), &statbuf)) {
		ਜੇ (strbuf_read_file(&sb, git_ਰਸਤਾ_squash_ਸੁਨੇਹਾ(the_repository), 0) < 0)
			die_errno(_("could not read SQUASH_MSG"));
		hook_arg1 = "squash";
	} ਨਹੀਂਤਾਂ ਜੇ (template_file) {
		ਜੇ (strbuf_read_file(&sb, template_file, 0) < 0)
			die_errno(_("could not read '%s'"), template_file);
		hook_arg1 = "template";
		clean_ਸੁਨੇਹਾ_contents = 0;
	}

	/*
	 * The remaining cases don't modify the template message, but
	 * just set the argument(s) to the prepare-commit-msg hook.
	 */
	ਨਹੀਂਤਾਂ ਜੇ (whence == FROM_ਮਿਲਾਓ)
		hook_arg1 = "merge";
	ਨਹੀਂਤਾਂ ਜੇ (is_from_cherry_pick(whence) || whence == FROM_REBASE_PICK) {
		hook_arg1 = "commit";
		hook_arg2 = "CHERRY_PICK_HEAD";
	}

	ਜੇ (squash_ਸੁਨੇਹਾ) {
		/*
		 * If squash_commit was used for the commit subject,
		 * then we're possibly hijacking other commit log options.
		 * Reset the hook args to tell the real story.
		 */
		hook_arg1 = "message";
		hook_arg2 = "";
	}

	s->fp = fopen_for_writing(git_ਰਸਤਾ_commit_editmsg());
	ਜੇ (!s->fp)
		die_errno(_("could not open '%s'"), git_ਰਸਤਾ_commit_editmsg());

	/* Ignore status.displayCommentPrefix: we do need comments in COMMIT_EDITMSG. */
	old_display_comment_ਅਗੇਤਰ = s->display_comment_ਅਗੇਤਰ;
	s->display_comment_ਅਗੇਤਰ = 1;

	/*
	 * Most hints are counter-productive when the commit has
	 * already started.
	 */
	s->hints = 0;

	ਜੇ (clean_ਸੁਨੇਹਾ_contents)
		strbuf_stripspace(&sb, ਨਿਰਾ);

	ਜੇ (signoff)
		append_signoff(&sb, ignored_log_ਸੁਨੇਹਾ_ਬਾਈਟਾਂ(sb.buf, sb.len), 0);

	ਜੇ (fwrite(sb.buf, 1, sb.len, s->fp) < sb.len)
		die_errno(_("could not write commit template"));

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ WITH_BREAKING_CHANGES
	ਜੇ (auto_comment_ਸਤਰ_char)
		adjust_comment_ਸਤਰ_char(&sb);
#ਜੇ_ਅੰਤ /* !WITH_BREAKING_CHANGES */
	strbuf_release(&sb);

	/* This checks if committer ident is explicitly given */
	strbuf_addstr(&committer_ident, git_committer_info(IDENT_STRICT));
	ਜੇ (use_editor && include_ਸਥਿਤੀ) {
		ਪੂਰਨ ident_shown = 0;
		ਗਿਣਤੀ git_colorbool saved_color_setting;
		ਬਣਤਰ ident_ਵੰਡੋ ci, ai;
		ਅਚਲ ਅੱਖਰ *hint_cleanup_all = allow_ਖਾਲੀ_ਥਾਵਾਂ_ਸੁਨੇਹਾ ?
			_("Please enter the commit message for your changes."
			  " Lines starting\nwith '%s' will be ignored.\n") :
			_("Please enter the commit message for your changes."
			  " Lines starting\nwith '%s' will be ignored, and an empty"
			  " message aborts the commit.\n");
		ਅਚਲ ਅੱਖਰ *hint_cleanup_space = allow_ਖਾਲੀ_ਥਾਵਾਂ_ਸੁਨੇਹਾ ?
			_("Please enter the commit message for your changes."
			  " Lines starting\n"
			  "with '%s' will be kept; you may remove them"
			  " yourself if you want to.\n") :
			_("Please enter the commit message for your changes."
			  " Lines starting\n"
			  "with '%s' will be kept; you may remove them"
			  " yourself if you want to.\n"
			  "An empty message aborts the commit.\n");
		ਜੇ (whence != FROM_COMMIT) {
			ਜੇ (cleanup_ਬਹੁਲਕ == COMMIT_ਸੁਨੇਹਾ_CLEANUP_SCISSORS)
				wt_ਸਥਿਤੀ_ਜੋੜੋ_cut_ਸਤਰ(s);
			ਸਥਿਤੀ_printf_ln(
				s, GIT_COLOR_ਸਾਧਾਰਨ,
				whence == FROM_ਮਿਲਾਓ ?
					      _("\n"
					  "It looks like you may be committing a merge.\n"
					  "If this is not correct, please run\n"
					  "	git update-ref -d MERGE_HEAD\n"
					  "and try again.\n") :
					      _("\n"
					  "It looks like you may be committing a cherry-pick.\n"
					  "If this is not correct, please run\n"
					  "	git update-ref -d CHERRY_PICK_HEAD\n"
					  "and try again.\n"));
		}

		fprintf(s->fp, "\n");
		ਜੇ (cleanup_ਬਹੁਲਕ == COMMIT_ਸੁਨੇਹਾ_CLEANUP_ALL)
			ਸਥਿਤੀ_printf(s, GIT_COLOR_ਸਾਧਾਰਨ, hint_cleanup_all, comment_ਸਤਰ_str);
		ਨਹੀਂਤਾਂ ਜੇ (cleanup_ਬਹੁਲਕ == COMMIT_ਸੁਨੇਹਾ_CLEANUP_SCISSORS) {
			ਜੇ (whence == FROM_COMMIT)
				wt_ਸਥਿਤੀ_ਜੋੜੋ_cut_ਸਤਰ(s);
		} ਨਹੀਂਤਾਂ /* COMMIT_MSG_CLEANUP_SPACE, that is. */
			ਸਥਿਤੀ_printf(s, GIT_COLOR_ਸਾਧਾਰਨ, hint_cleanup_space, comment_ਸਤਰ_str);

		/*
		 * These should never fail because they come from our own
		 * fmt_ident. They may fail the sane_ident test, but we know
		 * that the name and mail pointers will at least be valid,
		 * which is enough for our tests and printing here.
		 */
		assert_ਵੰਡੋ_ident(&ai, author_ident);
		assert_ਵੰਡੋ_ident(&ci, &committer_ident);

		ਜੇ (ident_cmp(&ai, &ci))
			ਸਥਿਤੀ_printf_ln(s, GIT_COLOR_ਸਾਧਾਰਨ,
				_("%s"
				"Author:    %.*s <%.*s>"),
				ident_shown++ ? "" : "\n",
				(ਪੂਰਨ)(ai.name_ਸਿਰਾ - ai.name_begin), ai.name_begin,
				(ਪੂਰਨ)(ai.mail_ਸਿਰਾ - ai.mail_begin), ai.mail_begin);

		ਜੇ (author_date_is_interesting())
			ਸਥਿਤੀ_printf_ln(s, GIT_COLOR_ਸਾਧਾਰਨ,
				_("%s"
				"Date:      %s"),
				ident_shown++ ? "" : "\n",
				show_ident_date(&ai, DATE_ਬਹੁਲਕ(ਸਾਧਾਰਨ)));

		ਜੇ (!committer_ident_sufficiently_given())
			ਸਥਿਤੀ_printf_ln(s, GIT_COLOR_ਸਾਧਾਰਨ,
				_("%s"
				"Committer: %.*s <%.*s>"),
				ident_shown++ ? "" : "\n",
				(ਪੂਰਨ)(ci.name_ਸਿਰਾ - ci.name_begin), ci.name_begin,
				(ਪੂਰਨ)(ci.mail_ਸਿਰਾ - ci.mail_begin), ci.mail_begin);

		ਸਥਿਤੀ_printf_ln(s, GIT_COLOR_ਸਾਧਾਰਨ, "%s", ""); /* Add new line for clarity */

		saved_color_setting = s->use_color;
		s->use_color = GIT_COLOR_NEVER;
		committable = ਲੜੀਵਾਰ_ਸਥਿਤੀ(s->fp, ਸੂਚਕਾਂਕ_file, ਅਗੇਤਰ, 1, s);
		s->use_color = saved_color_setting;
		string_list_clear_func(&s->change, change_ਅੰਕੜੇ_free);
	} ਨਹੀਂਤਾਂ {
		ਬਣਤਰ object_id oid;
		ਅਚਲ ਅੱਖਰ *ਮਾਪਾ = "HEAD";

		ਜੇ (!the_repository->ਸੂਚਕਾਂਕ->initialized && repo_read_ਸੂਚਕਾਂਕ(the_repository) < 0)
			die(_("Cannot read index"));

		ਜੇ (amend)
			ਮਾਪਾ = "HEAD^1";

		ਜੇ (repo_get_oid(the_repository, ਮਾਪਾ, &oid)) {
			ਪੂਰਨ i, ita_nr = 0;

			/* TODO: audit for interaction with sparse-index. */
			ensure_ਭਰਿਆ_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ);
			ਲਈ (i = 0; i < the_repository->ਸੂਚਕਾਂਕ->cache_nr; i++)
				ਜੇ (ce_intent_ਮੰਜ਼ਿਲ_ਜੋੜੋ(the_repository->ਸੂਚਕਾਂਕ->cache[i]))
					ita_nr++;
			committable = the_repository->ਸੂਚਕਾਂਕ->cache_nr > ita_nr;
		} ਨਹੀਂਤਾਂ {
			/*
			 * Unless the user did explicitly request a submodule
			 * ignore mode by passing a command line option we do
			 * not ignore any changed submodule SHA-1s when
			 * comparing index and parent, no matter what is
			 * configured. Otherwise we won't commit any
			 * submodules which were manually staged, which would
			 * be really confusing.
			 */
			ਬਣਤਰ diff_flags flags = DIFF_FLAGS_INIT;
			flags.override_submodule_config = 1;
			ਜੇ (ignore_submodule_arg &&
			    !strcmp(ignore_submodule_arg, "all"))
				flags.ignore_submodules = 1;
			committable = ਸੂਚਕਾਂਕ_differs_from(the_repository,
							 ਮਾਪਾ, &flags, 1);
		}
	}
	strbuf_release(&committer_ident);

	fclose(s->fp);

	ਜੇ (trailer_ਦਲੀਲਾਂ.nr) {
		ਜੇ (amend_file_with_trailers(git_ਰਸਤਾ_commit_editmsg(), &trailer_ਦਲੀਲਾਂ))
			die(_("unable to pass trailers to --trailers"));
		strvec_clear(&trailer_ਦਲੀਲਾਂ);
	}

	/*
	 * Reject an attempt to record a non-merge empty commit without
	 * explicit --allow-empty. In the cherry-pick case, it may be
	 * empty due to conflict resolution, which the user should okay.
	 */
	ਜੇ (!committable && whence != FROM_ਮਿਲਾਓ && !allow_ਖਾਲੀ_ਥਾਵਾਂ &&
	    !(amend && is_a_ਮਿਲਾਓ(current_ਸਿਰ))) {
		s->hints = advice_enabled(ADVICE_ਸਥਿਤੀ_HINTS);
		s->display_comment_ਅਗੇਤਰ = old_display_comment_ਅਗੇਤਰ;
		ਲੜੀਵਾਰ_ਸਥਿਤੀ(stdout, ਸੂਚਕਾਂਕ_file, ਅਗੇਤਰ, 0, s);
		ਜੇ (amend)
			fputs(_(ਖਾਲੀ_ਥਾਵਾਂ_amend_advice), stderr);
		ਨਹੀਂਤਾਂ ਜੇ (is_from_cherry_pick(whence) ||
			 whence == FROM_REBASE_PICK) {
			fputs(_(ਖਾਲੀ_ਥਾਵਾਂ_cherry_pick_advice), stderr);
			ਜੇ (whence == FROM_CHERRY_PICK_SINGLE)
				fputs(_(ਖਾਲੀ_ਥਾਵਾਂ_cherry_pick_advice_single), stderr);
			ਨਹੀਂਤਾਂ ਜੇ (whence == FROM_CHERRY_PICK_MULTI)
				fputs(_(ਖਾਲੀ_ਥਾਵਾਂ_cherry_pick_advice_multi), stderr);
			ਨਹੀਂਤਾਂ
				fputs(_(ਖਾਲੀ_ਥਾਵਾਂ_rebase_pick_advice), stderr);
		}
		ਵਾਪਸ 0;
	}

	ਜੇ (!no_verify && invoked_hook) {
		/*
		 * Re-read the index as the pre-commit-commit hook was invoked
		 * and could have updated it. We must do this before we invoke
		 * the editor and after we invoke run_status above.
		 */
		discard_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ);
	}
	read_ਸੂਚਕਾਂਕ_from(the_repository->ਸੂਚਕਾਂਕ, ਸੂਚਕਾਂਕ_file, repo_get_git_dir(the_repository));

	ਜੇ (cache_ਰੁੱਖ_update(the_repository->ਸੂਚਕਾਂਕ, 0)) {
		error(_("Error building trees"));
		ਵਾਪਸ 0;
	}

	ਜੇ (ਲੜੀਵਾਰ_commit_hook(use_editor, ਸੂਚਕਾਂਕ_file, ਨਿਰਾ, "prepare-commit-msg",
			    git_ਰਸਤਾ_commit_editmsg(), hook_arg1, hook_arg2, ਨਿਰਾ))
		ਵਾਪਸ 0;

	ਜੇ (use_editor) {
		ਬਣਤਰ strvec env = STRVEC_INIT;

		strvec_pushf(&env, "GIT_INDEX_FILE=%s", ਸੂਚਕਾਂਕ_file);
		ਜੇ (launch_editor(git_ਰਸਤਾ_commit_editmsg(), ਨਿਰਾ, env.v)) {
			fprintf(stderr,
			_("Please supply the message using either -m or -F option.\n"));
			ਬਾਹਰ_ਨਿਕਲੋ(1);
		}
		strvec_clear(&env);
	}

	ਜੇ (!no_verify &&
	    ਲੜੀਵਾਰ_commit_hook(use_editor, ਸੂਚਕਾਂਕ_file, ਨਿਰਾ, "commit-msg",
			    git_ਰਸਤਾ_commit_editmsg(), ਨਿਰਾ)) {
		ਵਾਪਸ 0;
	}

	ਵਾਪਸ 1;
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਲੱਭੋ_author_by_nickname(ਅਚਲ ਅੱਖਰ *name)
{
	ਬਣਤਰ rev_info revs;
	ਬਣਤਰ commit *commit;
	ਬਣਤਰ strbuf buf = STRBUF_INIT;
	ਅਚਲ ਅੱਖਰ *av[20];
	ਪੂਰਨ ac = 0;

	repo_init_revisions(the_repository, &revs, ਨਿਰਾ);
	strbuf_addf(&buf, "--author=%s", name);
	av[++ac] = "--all";
	av[++ac] = "-i";
	av[++ac] = buf.buf;
	av[++ac] = ਨਿਰਾ;
	ਤਿਆਰੀ_revisions(ac, av, &revs, ਨਿਰਾ);
	revs.mailmap = xmalloc(ਆਕਾਰ(ਬਣਤਰ string_list));
	string_list_init_nodup(revs.mailmap);
	read_mailmap(the_repository, revs.mailmap);

	ਜੇ (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	commit = get_revision(&revs);
	ਜੇ (commit) {
		ਬਣਤਰ pretty_print_context ctx = {0};
		ctx.date_ਬਹੁਲਕ.type = DATE_ਸਾਧਾਰਨ;
		strbuf_release(&buf);
		repo_format_commit_ਸੁਨੇਹਾ(the_repository, commit,
					   "%aN <%aE>", &buf, &ctx);
		release_revisions(&revs);
		ਵਾਪਸ strbuf_detach(&buf, ਨਿਰਾ);
	}
	die(_("--author '%s' is not 'Name <email>' and matches no existing author"), name);
}

ਸਥਿਰ ਖਾਲੀ handle_ignored_arg(ਬਣਤਰ wt_ਸਥਿਤੀ *s)
{
	ਜੇ (!ignored_arg)
		; /* default already initialized */
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(ignored_arg, "traditional"))
		s->show_ignored_ਬਹੁਲਕ = SHOW_TRADITIONAL_IGNORED;
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(ignored_arg, "no"))
		s->show_ignored_ਬਹੁਲਕ = SHOW_NO_IGNORED;
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(ignored_arg, "matching"))
		s->show_ignored_ਬਹੁਲਕ = SHOW_MATCHING_IGNORED;
	ਨਹੀਂਤਾਂ
		die(_("Invalid ignored mode '%s'"), ignored_arg);
}

ਸਥਿਰ ਗਿਣਤੀ untracked_ਸਥਿਤੀ_type parse_untracked_setting_name(ਅਚਲ ਅੱਖਰ *u)
{
	/*
	 * Please update $__git_untracked_file_modes in
	 * git-completion.bash when you add new options
	 */
	ਚੋਣ (git_parse_maybe_bool(u)) {
	ਹਾਲਤ 0:
		u = "no";
		ਰੋਕੋ;
	ਹਾਲਤ 1:
		u = "normal";
		ਰੋਕੋ;
	ਮੂਲ:
		ਰੋਕੋ;
	}

	ਜੇ (!strcmp(u, "no"))
		ਵਾਪਸ SHOW_NO_UNTRACKED_FILES;
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(u, "normal"))
		ਵਾਪਸ SHOW_ਸਾਧਾਰਨ_UNTRACKED_FILES;
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(u, "all"))
		ਵਾਪਸ SHOW_ALL_UNTRACKED_FILES;
	ਨਹੀਂਤਾਂ
		ਵਾਪਸ SHOW_UNTRACKED_FILES_ERROR;
}

ਸਥਿਰ ਖਾਲੀ handle_untracked_files_arg(ਬਣਤਰ wt_ਸਥਿਤੀ *s)
{
	ਗਿਣਤੀ untracked_ਸਥਿਤੀ_type u;

	ਜੇ (!untracked_files_arg)
		ਵਾਪਸ; /* default already initialized */

	u = parse_untracked_setting_name(untracked_files_arg);
	ਜੇ (u == SHOW_UNTRACKED_FILES_ERROR)
		die(_("Invalid untracked files mode '%s'"),
		    untracked_files_arg);
	s->show_untracked_files = u;
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *read_commit_ਸੁਨੇਹਾ_2(ਅਚਲ ਅੱਖਰ *name)
{
	ਅਚਲ ਅੱਖਰ *ਬਾਹਰਲਾ_enc;
	ਬਣਤਰ commit *commit;

	commit = lookup_commit_reference_by_name(name);
	ਜੇ (!commit)
		die(_("could not lookup commit '%s'"), name);
	ਬਾਹਰਲਾ_enc = get_commit_output_encoding();
	ਵਾਪਸ repo_logmsg_reencode(the_repository, commit, ਨਿਰਾ, ਬਾਹਰਲਾ_enc);
}

/*
 * Enumerate what needs to be propagated when --porcelain
 * is not in effect here.
 */
ਸਥਿਰ ਬਣਤਰ ਸਥਿਤੀ_deferred_config {
	ਗਿਣਤੀ wt_ਸਥਿਤੀ_format ਸਥਿਤੀ_format;
	ਪੂਰਨ show_branch;
	ਗਿਣਤੀ ahead_behind_flags ahead_behind;
} ਸਥਿਤੀ_deferred_config = {
	ਸਥਿਤੀ_FORMAT_UNSPECIFIED,
	-1, /* unspecified */
	AHEAD_BEHIND_UNSPECIFIED,
};

ਸਥਿਰ ਖਾਲੀ finalize_deferred_config(ਬਣਤਰ wt_ਸਥਿਤੀ *s)
{
	ਪੂਰਨ use_deferred_config = (ਸਥਿਤੀ_format != ਸਥਿਤੀ_FORMAT_PORCELAIN &&
				   ਸਥਿਤੀ_format != ਸਥਿਤੀ_FORMAT_PORCELAIN_V2 &&
				   !s->null_termination);

	ਜੇ (s->null_termination) {
		ਜੇ (ਸਥਿਤੀ_format == ਸਥਿਤੀ_FORMAT_NONE ||
		    ਸਥਿਤੀ_format == ਸਥਿਤੀ_FORMAT_UNSPECIFIED)
			ਸਥਿਤੀ_format = ਸਥਿਤੀ_FORMAT_PORCELAIN;
		ਨਹੀਂਤਾਂ ਜੇ (ਸਥਿਤੀ_format == ਸਥਿਤੀ_FORMAT_LONG)
			die(_("options '%s' and '%s' cannot be used together"), "--long", "-z");
	}

	ਜੇ (use_deferred_config && ਸਥਿਤੀ_format == ਸਥਿਤੀ_FORMAT_UNSPECIFIED)
		ਸਥਿਤੀ_format = ਸਥਿਤੀ_deferred_config.ਸਥਿਤੀ_format;
	ਜੇ (ਸਥਿਤੀ_format == ਸਥਿਤੀ_FORMAT_UNSPECIFIED)
		ਸਥਿਤੀ_format = ਸਥਿਤੀ_FORMAT_NONE;

	ਜੇ (use_deferred_config && s->show_branch < 0)
		s->show_branch = ਸਥਿਤੀ_deferred_config.show_branch;
	ਜੇ (s->show_branch < 0)
		s->show_branch = 0;

	/*
	 * If the user did not give a "--[no]-ahead-behind" command
	 * line argument *AND* we will print in a human-readable format
	 * (short, long etc.) then we inherit from the status.aheadbehind
	 * config setting.  In all other cases (and porcelain V[12] formats
	 * in particular), we inherit _FULL for backwards compatibility.
	 */
	ਜੇ (use_deferred_config &&
	    s->ahead_behind_flags == AHEAD_BEHIND_UNSPECIFIED)
		s->ahead_behind_flags = ਸਥਿਤੀ_deferred_config.ahead_behind;

	ਜੇ (s->ahead_behind_flags == AHEAD_BEHIND_UNSPECIFIED)
		s->ahead_behind_flags = AHEAD_BEHIND_ਭਰਿਆ;
}

ਸਥਿਰ ਖਾਲੀ check_fixup_reword_options(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ *argv[]) {
	ਜੇ (whence != FROM_COMMIT) {
		ਜੇ (whence == FROM_ਮਿਲਾਓ)
			die(_("You are in the middle of a merge -- cannot reword."));
		ਨਹੀਂਤਾਂ ਜੇ (is_from_cherry_pick(whence))
			die(_("You are in the middle of a cherry-pick -- cannot reword."));
	}
	ਜੇ (argc)
		die(_("reword option of '%s' and path '%s' cannot be used together"), "--fixup", *argv);
	ਜੇ (patch_interactive || interactive || all || also || only)
		die(_("reword option of '%s' and '%s' cannot be used together"),
			"--fixup", "--patch/--interactive/--all/--include/--only");
}

ਸਥਿਰ ਪੂਰਨ parse_and_validate_options(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ *argv[],
				      ਅਚਲ ਬਣਤਰ option *options,
				      ਅਚਲ ਅੱਖਰ * ਅਚਲ usage[],
				      ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
				      ਬਣਤਰ commit *current_ਸਿਰ,
				      ਬਣਤਰ wt_ਸਥਿਤੀ *s)
{
	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, usage, 0);
	finalize_deferred_config(s);

	ਜੇ (force_author && !strchr(force_author, '>'))
		force_author = ਲੱਭੋ_author_by_nickname(force_author);

	ਜੇ (force_author && renew_authorship)
		die(_("options '%s' and '%s' cannot be used together"), "--reset-author", "--author");

	ਜੇ (logfile || have_option_m || use_ਸੁਨੇਹਾ)
		use_editor = 0;

	/* Sanity check options */
	ਜੇ (amend && !current_ਸਿਰ)
		die(_("You have nothing to amend."));
	ਜੇ (amend && whence != FROM_COMMIT) {
		ਜੇ (whence == FROM_ਮਿਲਾਓ)
			die(_("You are in the middle of a merge -- cannot amend."));
		ਨਹੀਂਤਾਂ ਜੇ (is_from_cherry_pick(whence))
			die(_("You are in the middle of a cherry-pick -- cannot amend."));
		ਨਹੀਂਤਾਂ ਜੇ (whence == FROM_REBASE_PICK)
			die(_("You are in the middle of a rebase -- cannot amend."));
	}
	ਜੇ (fixup_ਸੁਨੇਹਾ && squash_ਸੁਨੇਹਾ)
		die(_("options '%s' and '%s' cannot be used together"), "--squash", "--fixup");
	die_for_incompatible_opt4(!!use_ਸੁਨੇਹਾ, "-C",
				  !!edit_ਸੁਨੇਹਾ, "-c",
				  !!logfile, "-F",
				  !!fixup_ਸੁਨੇਹਾ, "--fixup");
	die_for_incompatible_opt4(have_option_m, "-m",
				  !!edit_ਸੁਨੇਹਾ, "-c",
				  !!use_ਸੁਨੇਹਾ, "-C",
				  !!logfile, "-F");
	ਜੇ (use_ਸੁਨੇਹਾ || edit_ਸੁਨੇਹਾ || logfile ||fixup_ਸੁਨੇਹਾ || have_option_m)
		FREE_AND_NULL(template_file);
	ਜੇ (edit_ਸੁਨੇਹਾ)
		use_ਸੁਨੇਹਾ = edit_ਸੁਨੇਹਾ;
	ਜੇ (amend && !use_ਸੁਨੇਹਾ && !fixup_ਸੁਨੇਹਾ)
		use_ਸੁਨੇਹਾ = "HEAD";
	ਜੇ (!use_ਸੁਨੇਹਾ && !is_from_cherry_pick(whence) &&
	    !is_from_rebase(whence) && renew_authorship)
		die(_("--reset-author can be used only with -C, -c or --amend."));
	ਜੇ (use_ਸੁਨੇਹਾ) {
		use_ਸੁਨੇਹਾ_ਬਫਰ = read_commit_ਸੁਨੇਹਾ_2(use_ਸੁਨੇਹਾ);
		ਜੇ (!renew_authorship) {
			author_ਸੁਨੇਹਾ = use_ਸੁਨੇਹਾ;
			author_ਸੁਨੇਹਾ_ਬਫਰ = use_ਸੁਨੇਹਾ_ਬਫਰ;
		}
	}
	ਜੇ ((is_from_cherry_pick(whence) || whence == FROM_REBASE_PICK) &&
	    !renew_authorship) {
		author_ਸੁਨੇਹਾ = "CHERRY_PICK_HEAD";
		author_ਸੁਨੇਹਾ_ਬਫਰ = read_commit_ਸੁਨੇਹਾ_2(author_ਸੁਨੇਹਾ);
	}

	ਜੇ (patch_interactive)
		interactive = 1;

	die_for_incompatible_opt4(also, "-i/--include",
				  only, "-o/--only",
				  all, "-a/--all",
				  interactive, "--interactive/-p/--patch");
	ਜੇ (fixup_ਸੁਨੇਹਾ) {
		/*
		 * We limit --fixup's suboptions to only alpha characters.
		 * If the first character after a run of alpha is colon,
		 * then the part before the colon may be a known suboption
		 * name like `amend` or `reword`, or a misspelt suboption
		 * name. In either case, we treat it as
		 * --fixup=<suboption>:<arg>.
		 *
		 * Otherwise, we are dealing with --fixup=<commit>.
		 */
		ਅੱਖਰ *p = fixup_ਸੁਨੇਹਾ;
		ਜਦੋਂ (ਅੱਖਰ_ਹੈ(*p))
			p++;
		ਜੇ (p > fixup_ਸੁਨੇਹਾ && *p == ':') {
			*p = '\0';
			fixup_commit = p + 1;
			ਜੇ (!strcmp("amend", fixup_ਸੁਨੇਹਾ) ||
			    !strcmp("reword", fixup_ਸੁਨੇਹਾ)) {
				fixup_ਅਗੇਤਰ = "amend";
				allow_ਖਾਲੀ_ਥਾਵਾਂ = 1;
				ਜੇ (*fixup_ਸੁਨੇਹਾ == 'r') {
					check_fixup_reword_options(argc, argv);
					only = 1;
				}
			} ਨਹੀਂਤਾਂ {
				die(_("unknown option: --fixup=%s:%s"), fixup_ਸੁਨੇਹਾ, fixup_commit);
			}
		} ਨਹੀਂਤਾਂ {
			fixup_commit = fixup_ਸੁਨੇਹਾ;
			fixup_ਅਗੇਤਰ = "fixup";
			use_editor = 0;
		}
	}

	ਜੇ (0 <= edit_flag)
		use_editor = edit_flag;

	handle_untracked_files_arg(s);

	ਜੇ (all && argc > 0)
		die(_("paths '%s ...' with -a does not make sense"),
		    argv[0]);

	ਜੇ (ਸਥਿਤੀ_format != ਸਥਿਤੀ_FORMAT_NONE)
		dry_ਲੜੀਵਾਰ = 1;

	ਵਾਪਸ argc;
}

ਸਥਿਰ ਪੂਰਨ dry_ਲੜੀਵਾਰ_commit(ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			  ਅਚਲ ਬਣਤਰ commit *current_ਸਿਰ, ਬਣਤਰ wt_ਸਥਿਤੀ *s)
{
	ਪੂਰਨ committable;
	ਅਚਲ ਅੱਖਰ *ਸੂਚਕਾਂਕ_file;

	ਸੂਚਕਾਂਕ_file = prepare_ਸੂਚਕਾਂਕ(argv, ਅਗੇਤਰ, current_ਸਿਰ, 1);
	committable = ਲੜੀਵਾਰ_ਸਥਿਤੀ(stdout, ਸੂਚਕਾਂਕ_file, ਅਗੇਤਰ, 0, s);
	rollback_ਸੂਚਕਾਂਕ_files();

	ਵਾਪਸ committable ? 0 : 1;
}

define_list_config_array_extra(color_ਸਥਿਤੀ_slots, {"added"});

ਸਥਿਰ ਪੂਰਨ parse_ਸਥਿਤੀ_ਥਾਂ(ਅਚਲ ਅੱਖਰ *ਥਾਂ)
{
	ਜੇ (!strcasecmp(ਥਾਂ, "added"))
		ਵਾਪਸ WT_ਸਥਿਤੀ_UPDATED;

	ਵਾਪਸ LOOKUP_CONFIG(color_ਸਥਿਤੀ_slots, ਥਾਂ);
}

ਸਥਿਰ ਪੂਰਨ git_ਸਥਿਤੀ_config(ਅਚਲ ਅੱਖਰ *k, ਅਚਲ ਅੱਖਰ *v,
			     ਅਚਲ ਬਣਤਰ config_context *ctx, ਖਾਲੀ *cb)
{
	ਬਣਤਰ wt_ਸਥਿਤੀ *s = cb;
	ਅਚਲ ਅੱਖਰ *ਥਾਂ_name;

	ਜੇ (starts_with(k, "column."))
		ਵਾਪਸ git_ਕਾਲਮ_config(k, v, "status", &s->colopts);
	ਜੇ (!strcmp(k, "status.submodulesummary")) {
		ਪੂਰਨ is_bool;
		s->submodule_summary = git_config_bool_or_int(k, v, ctx->kvi,
							      &is_bool);
		ਜੇ (is_bool && s->submodule_summary)
			s->submodule_summary = -1;
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(k, "status.short")) {
		ਜੇ (git_config_bool(k, v))
			ਸਥਿਤੀ_deferred_config.ਸਥਿਤੀ_format = ਸਥਿਤੀ_FORMAT_SHORT;
		ਨਹੀਂਤਾਂ
			ਸਥਿਤੀ_deferred_config.ਸਥਿਤੀ_format = ਸਥਿਤੀ_FORMAT_NONE;
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(k, "status.branch")) {
		ਸਥਿਤੀ_deferred_config.show_branch = git_config_bool(k, v);
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(k, "status.aheadbehind")) {
		ਸਥਿਤੀ_deferred_config.ahead_behind = git_config_bool(k, v);
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(k, "status.showstash")) {
		s->show_stash = git_config_bool(k, v);
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(k, "status.color") || !strcmp(k, "color.status")) {
		s->use_color = git_config_colorbool(k, v);
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(k, "status.displaycommentprefix")) {
		s->display_comment_ਅਗੇਤਰ = git_config_bool(k, v);
		ਵਾਪਸ 0;
	}
	ਜੇ (skip_ਅਗੇਤਰ(k, "status.color.", &ਥਾਂ_name) ||
	    skip_ਅਗੇਤਰ(k, "color.status.", &ਥਾਂ_name)) {
		ਪੂਰਨ ਥਾਂ = parse_ਸਥਿਤੀ_ਥਾਂ(ਥਾਂ_name);
		ਜੇ (ਥਾਂ < 0)
			ਵਾਪਸ 0;
		ਜੇ (!v)
			ਵਾਪਸ config_error_nonbool(k);
		ਵਾਪਸ color_parse(v, s->color_palette[ਥਾਂ]);
	}
	ਜੇ (!strcmp(k, "status.relativepaths")) {
		s->relative_paths = git_config_bool(k, v);
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(k, "status.showuntrackedfiles")) {
		ਗਿਣਤੀ untracked_ਸਥਿਤੀ_type u;

		u = parse_untracked_setting_name(v);
		ਜੇ (u == SHOW_UNTRACKED_FILES_ERROR)
			ਵਾਪਸ error(_("Invalid untracked files mode '%s'"), v);
		s->show_untracked_files = u;
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(k, "diff.renamelimit")) {
		ਜੇ (s->rename_ਹੱਦਬੰਦੀ == -1)
			s->rename_ਹੱਦਬੰਦੀ = git_config_int(k, v, ctx->kvi);
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(k, "status.renamelimit")) {
		s->rename_ਹੱਦਬੰਦੀ = git_config_int(k, v, ctx->kvi);
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(k, "diff.renames")) {
		ਜੇ (s->detect_rename == -1)
			s->detect_rename = git_config_rename(k, v);
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(k, "status.renames")) {
		s->detect_rename = git_config_rename(k, v);
		ਵਾਪਸ 0;
	}
	ਵਾਪਸ git_diff_ui_config(k, v, ctx, ਨਿਰਾ);
}

ਪੂਰਨ cmd_ਸਥਿਤੀ(ਪੂਰਨ argc,
ਅਚਲ ਅੱਖਰ **argv,
ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਸਥਿਰ ਪੂਰਨ no_renames = -1;
	ਸਥਿਰ ਅਚਲ ਅੱਖਰ *rename_score_arg = (ਅਚਲ ਅੱਖਰ *)-1;
	ਸਥਿਰ ਬਣਤਰ wt_ਸਥਿਤੀ s;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਤਰੱਕੀ_flag = 0;
	ਪੂਰਨ fd;
	ਬਣਤਰ object_id oid;
	ਸਥਿਰ ਬਣਤਰ option builtin_ਸਥਿਤੀ_options[] = {
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT_ਸੈੱਟ_INT('s', "short", &ਸਥਿਤੀ_format,
			    N_("show status concisely"), ਸਥਿਤੀ_FORMAT_SHORT),
		OPT_BOOL('b', "branch", &s.show_branch,
			 N_("show branch information")),
		OPT_BOOL(0, "show-stash", &s.show_stash,
			 N_("show stash information")),
		OPT_BOOL(0, "ahead-behind", &s.ahead_behind_flags,
			 N_("compute full ahead/behind values")),
		OPT_CALLBACK_F(0, "porcelain", &ਸਥਿਤੀ_format,
		  N_("version"), N_("machine-readable output"),
		  PARSE_OPT_OPTARG, opt_parse_porcelain),
		OPT_ਸੈੱਟ_INT(0, "long", &ਸਥਿਤੀ_format,
			    N_("show status in long format (default)"),
			    ਸਥਿਤੀ_FORMAT_LONG),
		OPT_BOOL('z', "null", &s.null_termination,
			 N_("terminate entries with NUL")),
		{
			.type = OPTION_STRING,
			.short_name = 'u',
			.long_name = "untracked-files",
			.ਮੁੱਲ = &untracked_files_arg,
			.argh = N_("mode"),
			.help = N_("show untracked files, optional modes: all, normal, no. (Default: all)"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t)"all",
		},
		{
			.type = OPTION_STRING,
			.long_name = "ignored",
			.ਮੁੱਲ = &ignored_arg,
			.argh = N_("mode"),
			.help = N_("show ignored files, optional modes: traditional, matching, no. (Default: traditional)"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t)"traditional",
		},
		{
			.type = OPTION_STRING,
			.long_name = "ignore-submodules",
			.ਮੁੱਲ = &ignore_submodule_arg,
			.argh = N_("when"),
			.help = N_("ignore changes to submodules, optional when: all, dirty, untracked. (Default: all)"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t)"all",
		},
		OPT_ਕਾਲਮ(0, "column", &s.colopts, N_("list untracked files in columns")),
		OPT_BOOL(0, "no-renames", &no_renames, N_("do not detect renames")),
		OPT_CALLBACK_F('M', "find-renames", &rename_score_arg,
		  N_("n"), N_("detect renames, optionally set similarity index"),
		  PARSE_OPT_OPTARG | PARSE_OPT_NONEG, opt_parse_rename_score),
		OPT_ਸਿਰਾ(),
	};

	show_usage_with_options_if_asked(argc, argv,
					 builtin_ਸਥਿਤੀ_usage, builtin_ਸਥਿਤੀ_options);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_ਭਰਿਆ_ਸੂਚਕਾਂਕ = 0;

	ਸਥਿਤੀ_init_config(&s, git_ਸਥਿਤੀ_config);
	argc = parse_options(argc, argv, ਅਗੇਤਰ,
			     builtin_ਸਥਿਤੀ_options,
			     builtin_ਸਥਿਤੀ_usage, 0);
	finalize_colopts(&s.colopts, -1);
	finalize_deferred_config(&s);

	handle_untracked_files_arg(&s);
	handle_ignored_arg(&s);

	ਜੇ (s.show_ignored_ਬਹੁਲਕ == SHOW_MATCHING_IGNORED &&
	    s.show_untracked_files == SHOW_NO_UNTRACKED_FILES)
		die(_("Unsupported combination of ignored and untracked-files arguments"));

	parse_pathspec(&s.pathspec, 0,
		       PATHSPEC_PREFER_ਭਰਿਆ,
		       ਅਗੇਤਰ, argv);

	ਜੇ (ਸਥਿਤੀ_format != ਸਥਿਤੀ_FORMAT_PORCELAIN &&
	    ਸਥਿਤੀ_format != ਸਥਿਤੀ_FORMAT_PORCELAIN_V2)
		ਤਰੱਕੀ_flag = REFRESH_ਤਰੱਕੀ;
	repo_read_ਸੂਚਕਾਂਕ(the_repository);
	refresh_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ,
		      REFRESH_QUIET|REFRESH_UNMERGED|ਤਰੱਕੀ_flag,
		      &s.pathspec, ਨਿਰਾ, ਨਿਰਾ);

	ਜੇ (use_optional_locks())
		fd = repo_hold_locked_ਸੂਚਕਾਂਕ(the_repository, &ਸੂਚਕਾਂਕ_ਤਾਲਾ, 0);
	ਨਹੀਂਤਾਂ
		fd = -1;

	s.is_initial = repo_get_oid(the_repository, s.reference, &oid) ? 1 : 0;
	ਜੇ (!s.is_initial)
		oidcpy(&s.oid_commit, &oid);

	s.ignore_submodule_arg = ignore_submodule_arg;
	s.ਸਥਿਤੀ_format = ਸਥਿਤੀ_format;
	s.verbose = verbose;
	ਜੇ (no_renames != -1)
		s.detect_rename = !no_renames;
	ਜੇ ((intptr_t)rename_score_arg != -1) {
		ਜੇ (s.detect_rename < DIFF_DETECT_RENAME)
			s.detect_rename = DIFF_DETECT_RENAME;
		ਜੇ (rename_score_arg)
			s.rename_score = parse_rename_score(&rename_score_arg);
	}

	wt_ਸਥਿਤੀ_collect(&s);

	ਜੇ (0 <= fd)
		repo_update_ਸੂਚਕਾਂਕ_if_able(the_repository, &ਸੂਚਕਾਂਕ_ਤਾਲਾ);

	ਜੇ (s.relative_paths)
		s.ਅਗੇਤਰ = ਅਗੇਤਰ;

	wt_ਸਥਿਤੀ_print(&s);
	wt_ਸਥਿਤੀ_collect_free_buffers(&s);

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ git_commit_config(ਅਚਲ ਅੱਖਰ *k, ਅਚਲ ਅੱਖਰ *v,
			     ਅਚਲ ਬਣਤਰ config_context *ctx, ਖਾਲੀ *cb)
{
	ਬਣਤਰ wt_ਸਥਿਤੀ *s = cb;

	ਜੇ (!strcmp(k, "commit.template"))
		ਵਾਪਸ git_config_pathname(&template_file, k, v);
	ਜੇ (!strcmp(k, "commit.status")) {
		include_ਸਥਿਤੀ = git_config_bool(k, v);
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(k, "commit.cleanup")) {
		FREE_AND_NULL(cleanup_config);
		ਵਾਪਸ git_config_string(&cleanup_config, k, v);
	}
	ਜੇ (!strcmp(k, "commit.gpgsign")) {
		sign_commit = git_config_bool(k, v) ? "" : ਨਿਰਾ;
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(k, "commit.verbose")) {
		ਪੂਰਨ is_bool;
		config_commit_verbose = git_config_bool_or_int(k, v, ctx->kvi,
							       &is_bool);
		ਵਾਪਸ 0;
	}

	ਵਾਪਸ git_ਸਥਿਤੀ_config(k, v, ctx, s);
}

ਪੂਰਨ cmd_commit(ਪੂਰਨ argc,
	       ਅਚਲ ਅੱਖਰ **argv,
	       ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
	       ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਸਥਿਰ ਬਣਤਰ wt_ਸਥਿਤੀ s;
	ਸਥਿਰ ਅਚਲ ਅੱਖਰ *cleanup_arg = ਨਿਰਾ;
	ਸਥਿਰ ਬਣਤਰ option builtin_commit_options[] = {
		OPT__QUIET(&quiet, N_("suppress summary after successful commit")),
		OPT__VERBOSE(&verbose, N_("show diff in commit message template")),

		OPT_ਸਮੂਹ(N_("Commit message options")),
		OPT_FILENAME('F', "file", &logfile, N_("read message from file")),
		OPT_STRING(0, "author", &force_author, N_("author"), N_("override author for commit")),
		OPT_STRING(0, "date", &force_date, N_("date"), N_("override date for commit")),
		OPT_CALLBACK('m', "message", &ਸੁਨੇਹਾ_2, N_("message"), N_("commit message"), opt_parse_m),
		OPT_STRING('c', "reedit-message", &edit_ਸੁਨੇਹਾ, N_("commit"), N_("reuse and edit message from specified commit")),
		OPT_STRING('C', "reuse-message", &use_ਸੁਨੇਹਾ, N_("commit"), N_("reuse message from specified commit")),
		/*
		 * TRANSLATORS: Leave "[(amend|reword):]" as-is,
		 * and only translate <commit>.
		 */
		OPT_STRING(0, "fixup", &fixup_ਸੁਨੇਹਾ, N_("[(amend|reword):]commit"), N_("use autosquash formatted message to fixup or amend/reword specified commit")),
		OPT_STRING(0, "squash", &squash_ਸੁਨੇਹਾ, N_("commit"), N_("use autosquash formatted message to squash specified commit")),
		OPT_BOOL(0, "reset-author", &renew_authorship, N_("the commit is authored by me now (used with -C/-c/--amend)")),
		OPT_STRVEC(0, "trailer", &trailer_ਦਲੀਲਾਂ, N_("trailer"),
			   N_("add custom trailer(s)")),
		OPT_BOOL('s', "signoff", &signoff, N_("add a Signed-off-by trailer")),
		OPT_FILENAME('t', "template", &template_file, N_("use specified template file")),
		OPT_BOOL('e', "edit", &edit_flag, N_("force edit of commit")),
		OPT_CLEANUP(&cleanup_arg),
		OPT_BOOL(0, "status", &include_ਸਥਿਤੀ, N_("include status in commit message template")),
		{
			.type = OPTION_STRING,
			.short_name = 'S',
			.long_name = "gpg-sign",
			.ਮੁੱਲ = &sign_commit,
			.argh = N_("key-id"),
			.help = N_("GPG sign commit"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "",
		},
		/* end commit message options */

		OPT_ਸਮੂਹ(N_("Commit contents options")),
		OPT_BOOL('a', "all", &all, N_("commit all changed files")),
		OPT_BOOL('i', "include", &also, N_("add specified files to index for commit")),
		OPT_BOOL(0, "interactive", &interactive, N_("interactively add files")),
		OPT_BOOL('p', "patch", &patch_interactive, N_("interactively add changes")),
		OPT_DIFF_UNIFIED(&interactive_opts.context),
		OPT_DIFF_INTERHUNK_CONTEXT(&interactive_opts.interhunkcontext),
		OPT_BOOL('o', "only", &only, N_("commit only specified files")),
		OPT_BOOL('n', "no-verify", &no_verify, N_("bypass pre-commit and commit-msg hooks")),
		OPT_BOOL(0, "dry-run", &dry_ਲੜੀਵਾਰ, N_("show what would be committed")),
		OPT_ਸੈੱਟ_INT(0, "short", &ਸਥਿਤੀ_format, N_("show status concisely"),
			    ਸਥਿਤੀ_FORMAT_SHORT),
		OPT_BOOL(0, "branch", &s.show_branch, N_("show branch information")),
		OPT_BOOL(0, "ahead-behind", &s.ahead_behind_flags,
			 N_("compute full ahead/behind values")),
		OPT_ਸੈੱਟ_INT(0, "porcelain", &ਸਥਿਤੀ_format,
			    N_("machine-readable output"), ਸਥਿਤੀ_FORMAT_PORCELAIN),
		OPT_ਸੈੱਟ_INT(0, "long", &ਸਥਿਤੀ_format,
			    N_("show status in long format (default)"),
			    ਸਥਿਤੀ_FORMAT_LONG),
		OPT_BOOL('z', "null", &s.null_termination,
			 N_("terminate entries with NUL")),
		OPT_BOOL(0, "amend", &amend, N_("amend previous commit")),
		OPT_BOOL(0, "no-post-rewrite", &no_post_rewrite, N_("bypass post-rewrite hook")),
		{
			.type = OPTION_STRING,
			.short_name = 'u',
			.long_name = "untracked-files",
			.ਮੁੱਲ = &untracked_files_arg,
			.argh = N_("mode"),
			.help = N_("show untracked files, optional modes: all, normal, no. (Default: all)"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t)"all",
		},
		OPT_PATHSPEC_FROM_FILE(&pathspec_from_file),
		OPT_PATHSPEC_FILE_NUL(&pathspec_file_nul),
		/* end commit contents options */

		OPT_HIDDEN_BOOL(0, "allow-empty", &allow_ਖਾਲੀ_ਥਾਵਾਂ,
				N_("ok to record an empty change")),
		OPT_HIDDEN_BOOL(0, "allow-empty-message", &allow_ਖਾਲੀ_ਥਾਵਾਂ_ਸੁਨੇਹਾ,
				N_("ok to record a change with an empty message")),

		OPT_ਸਿਰਾ()
	};

	ਬਣਤਰ strbuf sb = STRBUF_INIT;
	ਬਣਤਰ strbuf author_ident = STRBUF_INIT;
	ਅਚਲ ਅੱਖਰ *ਸੂਚਕਾਂਕ_file, *reflog_ਸੁਨੇਹਾ;
	ਬਣਤਰ object_id oid;
	ਬਣਤਰ commit_list *parents = ਨਿਰਾ;
	ਬਣਤਰ stat statbuf;
	ਬਣਤਰ commit *current_ਸਿਰ = ਨਿਰਾ;
	ਬਣਤਰ commit_extra_ਸਿਰਲੇਖ *extra = ਨਿਰਾ;
	ਬਣਤਰ strbuf err = STRBUF_INIT;
	ਪੂਰਨ ret = 0;

	show_usage_with_options_if_asked(argc, argv,
					 builtin_commit_usage, builtin_commit_options);

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ WITH_BREAKING_CHANGES
	warn_on_auto_comment_char = true;
#ਜੇ_ਅੰਤ /* !WITH_BREAKING_CHANGES */
	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_ਭਰਿਆ_ਸੂਚਕਾਂਕ = 0;

	ਸਥਿਤੀ_init_config(&s, git_commit_config);
	s.commit_template = 1;
	ਸਥਿਤੀ_format = ਸਥਿਤੀ_FORMAT_NONE; /* Ignore status.short */
	s.colopts = 0;

	ਜੇ (repo_get_oid(the_repository, "HEAD", &oid))
		current_ਸਿਰ = ਨਿਰਾ;
	ਨਹੀਂਤਾਂ {
		current_ਸਿਰ = lookup_commit_or_die(&oid, "HEAD");
		ਜੇ (repo_parse_commit(the_repository, current_ਸਿਰ))
			die(_("could not parse HEAD commit"));
	}
	verbose = -1; /* unspecified */
	argc = parse_and_validate_options(argc, argv, builtin_commit_options,
					  builtin_commit_usage,
					  ਅਗੇਤਰ, current_ਸਿਰ, &s);
	ਜੇ (trailer_ਦਲੀਲਾਂ.nr)
		trailer_config_init();

	ਜੇ (verbose == -1)
		verbose = (config_commit_verbose < 0) ? 0 : config_commit_verbose;

	ਜੇ (cleanup_arg) {
		ਮੁਕਤ(cleanup_config);
		cleanup_config = xstrdup(cleanup_arg);
	}
	cleanup_ਬਹੁਲਕ = get_cleanup_ਬਹੁਲਕ(cleanup_config, use_editor);

	ਜੇ (dry_ਲੜੀਵਾਰ)
		ਵਾਪਸ dry_ਲੜੀਵਾਰ_commit(argv, ਅਗੇਤਰ, current_ਸਿਰ, &s);
	ਸੂਚਕਾਂਕ_file = prepare_ਸੂਚਕਾਂਕ(argv, ਅਗੇਤਰ, current_ਸਿਰ, 0);

	/* Set up everything for writing the commit object.  This includes
	   running hooks, writing the trees, and interacting with the user.  */
	ਜੇ (!prepare_ਮੰਜ਼ਿਲ_commit(ਸੂਚਕਾਂਕ_file, ਅਗੇਤਰ,
			       current_ਸਿਰ, &s, &author_ident)) {
		ret = 1;
		rollback_ਸੂਚਕਾਂਕ_files();
		ਜਾਓ cleanup;
	}

	/* Determine parents */
	reflog_ਸੁਨੇਹਾ = getenv("GIT_REFLOG_ACTION");
	ਜੇ (!current_ਸਿਰ) {
		ਜੇ (!reflog_ਸੁਨੇਹਾ)
			reflog_ਸੁਨੇਹਾ = "commit (initial)";
	} ਨਹੀਂਤਾਂ ਜੇ (amend) {
		ਜੇ (!reflog_ਸੁਨੇਹਾ)
			reflog_ਸੁਨੇਹਾ = "commit (amend)";
		parents = commit_list_copy(current_ਸਿਰ->parents);
	} ਨਹੀਂਤਾਂ ਜੇ (whence == FROM_ਮਿਲਾਓ) {
		ਬਣਤਰ strbuf m = STRBUF_INIT;
		FILE *fp;
		ਪੂਰਨ allow_fast_forward = 1;
		ਬਣਤਰ commit_list **pptr = &parents;

		ਜੇ (!reflog_ਸੁਨੇਹਾ)
			reflog_ਸੁਨੇਹਾ = "commit (merge)";
		pptr = commit_list_append(current_ਸਿਰ, pptr);
		fp = xfopen(git_ਰਸਤਾ_ਮਿਲਾਓ_ਸਿਰ(the_repository), "r");
		ਜਦੋਂ (strbuf_getline_lf(&m, fp) != EOF) {
			ਬਣਤਰ commit *ਮਾਪਾ;

			ਮਾਪਾ = get_ਮਿਲਾਓ_ਮਾਪਾ(m.buf);
			ਜੇ (!ਮਾਪਾ)
				die(_("Corrupt MERGE_HEAD file (%s)"), m.buf);
			pptr = commit_list_append(ਮਾਪਾ, pptr);
		}
		fclose(fp);
		strbuf_release(&m);
		ਜੇ (!stat(git_ਰਸਤਾ_ਮਿਲਾਓ_ਬਹੁਲਕ(the_repository), &statbuf)) {
			ਜੇ (strbuf_read_file(&sb, git_ਰਸਤਾ_ਮਿਲਾਓ_ਬਹੁਲਕ(the_repository), 0) < 0)
				die_errno(_("could not read MERGE_MODE"));
			ਜੇ (!strcmp(sb.buf, "no-ff"))
				allow_fast_forward = 0;
		}
		ਜੇ (allow_fast_forward)
			reduce_heads_replace(&parents);
	} ਨਹੀਂਤਾਂ {
		ਜੇ (!reflog_ਸੁਨੇਹਾ)
			reflog_ਸੁਨੇਹਾ = is_from_cherry_pick(whence)
					? "commit (cherry-pick)"
					: is_from_rebase(whence)
					? "commit (rebase)"
					: "commit";
		commit_list_ਪਾਓ(current_ਸਿਰ, &parents);
	}

	/* Finally, get the commit message */
	strbuf_reset(&sb);
	ਜੇ (strbuf_read_file(&sb, git_ਰਸਤਾ_commit_editmsg(), 0) < 0) {
		ਪੂਰਨ saved_errno = errno;
		rollback_ਸੂਚਕਾਂਕ_files();
		die(_("could not read commit message: %s"), strerror(saved_errno));
	}

	cleanup_ਸੁਨੇਹਾ(&sb, cleanup_ਬਹੁਲਕ, verbose);

	ਜੇ (ਸੁਨੇਹਾ_is_ਖਾਲੀ_ਥਾਵਾਂ(&sb, cleanup_ਬਹੁਲਕ) && !allow_ਖਾਲੀ_ਥਾਵਾਂ_ਸੁਨੇਹਾ) {
		rollback_ਸੂਚਕਾਂਕ_files();
		fprintf(stderr, _("Aborting commit due to empty commit message.\n"));
		ਬਾਹਰ_ਨਿਕਲੋ(1);
	}
	ਜੇ (template_untouched(&sb, template_file, cleanup_ਬਹੁਲਕ) && !allow_ਖਾਲੀ_ਥਾਵਾਂ_ਸੁਨੇਹਾ) {
		rollback_ਸੂਚਕਾਂਕ_files();
		fprintf(stderr, _("Aborting commit; you did not edit the message.\n"));
		ਬਾਹਰ_ਨਿਕਲੋ(1);
	}

	ਜੇ (fixup_ਸੁਨੇਹਾ && starts_with(sb.buf, "amend! ") &&
	    !allow_ਖਾਲੀ_ਥਾਵਾਂ_ਸੁਨੇਹਾ) {
		ਬਣਤਰ strbuf body = STRBUF_INIT;
		ਮਾਪ_t len = commit_subject_ਲੰਮਾਈ(sb.buf);
		strbuf_addstr(&body, sb.buf + len);
		ਜੇ (ਸੁਨੇਹਾ_is_ਖਾਲੀ_ਥਾਵਾਂ(&body, cleanup_ਬਹੁਲਕ)) {
			rollback_ਸੂਚਕਾਂਕ_files();
			fprintf(stderr, _("Aborting commit due to empty commit message body.\n"));
			ਬਾਹਰ_ਨਿਕਲੋ(1);
		}
		strbuf_release(&body);
	}

	ਜੇ (amend) {
		ਅਚਲ ਅੱਖਰ *exclude_gpgsig[3] = { "gpgsig", "gpgsig-sha256", ਨਿਰਾ };
		extra = read_commit_extra_ਸਿਰਲੇਖਾਂ(current_ਸਿਰ, exclude_gpgsig);
	} ਨਹੀਂਤਾਂ {
		ਬਣਤਰ commit_extra_ਸਿਰਲੇਖ **ਪੂਛ = &extra;
		append_ਮਿਲਾਓ_ਟੈਗ_ਸਿਰਲੇਖਾਂ(parents, &ਪੂਛ);
	}

	ਜੇ (commit_ਰੁੱਖ_extended(sb.buf, sb.len, &the_repository->ਸੂਚਕਾਂਕ->cache_ਰੁੱਖ->oid,
				 parents, &oid, author_ident.buf, ਨਿਰਾ,
				 sign_commit, extra)) {
		rollback_ਸੂਚਕਾਂਕ_files();
		die(_("failed to write commit object"));
	}

	ਜੇ (update_ਸਿਰ_with_reflog(current_ਸਿਰ, &oid, reflog_ਸੁਨੇਹਾ, &sb,
				    &err)) {
		rollback_ਸੂਚਕਾਂਕ_files();
		die("%s", err.buf);
	}

	sequencer_post_commit_cleanup(the_repository, 0);
	unlink(git_ਰਸਤਾ_ਮਿਲਾਓ_ਸਿਰ(the_repository));
	unlink(git_ਰਸਤਾ_ਮਿਲਾਓ_ਸੁਨੇਹਾ(the_repository));
	unlink(git_ਰਸਤਾ_ਮਿਲਾਓ_ਬਹੁਲਕ(the_repository));
	unlink(git_ਰਸਤਾ_squash_ਸੁਨੇਹਾ(the_repository));

	ਜੇ (commit_ਸੂਚਕਾਂਕ_files())
		die(_("repository has been updated, but unable to write\n"
		      "new index file. Check that disk is not full and quota is\n"
		      "not exceeded, and then \"git restore --staged :/\" to recover."));

	git_test_write_commit_ਗ੍ਰਾਫ_or_die(the_repository);

	repo_rerere(the_repository, 0);
	ਲੜੀਵਾਰ_auto_maintenance(the_repository, quiet);
	ਲੜੀਵਾਰ_commit_hook(use_editor, repo_get_ਸੂਚਕਾਂਕ_file(the_repository),
			ਨਿਰਾ, "post-commit", ਨਿਰਾ);
	ਜੇ (amend && !no_post_rewrite) {
		commit_post_rewrite(the_repository, current_ਸਿਰ, &oid);
	}
	ਜੇ (!quiet) {
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags = 0;

		ਜੇ (!current_ਸਿਰ)
			flags |= SUMMARY_INITIAL_COMMIT;
		ਜੇ (author_date_is_interesting())
			flags |= SUMMARY_SHOW_AUTHOR_DATE;
		print_commit_summary(the_repository, ਅਗੇਤਰ,
				     &oid, flags);
	}

	apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
			    ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ);

cleanup:
	free_commit_extra_ਸਿਰਲੇਖਾਂ(extra);
	commit_list_free(parents);
	strbuf_release(&author_ident);
	strbuf_release(&err);
	strbuf_release(&sb);
	ਮੁਕਤ(logfile);
	ਮੁਕਤ(template_file);
	ਵਾਪਸ ret;
}
