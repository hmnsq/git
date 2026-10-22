/*
 * Builtin "git commit"
 *
 * Copyright (c) 2007 Kristian Høgsberg <krh@redhat.com>
 * Based on git-commit.sh by Junio C Hamano and Linus Torvalds
 */

#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE
#ವ್ಯಾಖ್ಯಾನಿಸು DISABLE_SIGN_COMPARE_WARNINGS

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "advice.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "lockfile.h"
#ಅನ್ತರ್ಭಾವ "cache-tree.h"
#ಅನ್ತರ್ಭಾವ "color.h"
#ಅನ್ತರ್ಭಾವ "dir.h"
#ಅನ್ತರ್ಭಾವ "editor.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "diff.h"
#ಅನ್ತರ್ಭಾವ "commit.h"
#ಅನ್ತರ್ಭಾವ "add-interactive.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "revision.h"
#ಅನ್ತರ್ಭಾವ "wt-status.h"
#ಅನ್ತರ್ಭಾವ "run-command.h"
#ಅನ್ತರ್ಭಾವ "strbuf.h"
#ಅನ್ತರ್ಭಾವ "object-name.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "path.h"
#ಅನ್ತರ್ಭಾವ "preload-index.h"
#ಅನ್ತರ್ಭಾವ "read-cache.h"
#ಅನ್ತರ್ಭಾವ "repository.h"
#ಅನ್ತರ್ಭಾವ "string-list.h"
#ಅನ್ತರ್ಭಾವ "rerere.h"
#ಅನ್ತರ್ಭಾವ "unpack-trees.h"
#ಅನ್ತರ್ಭಾವ "column.h"
#ಅನ್ತರ್ಭಾವ "sequencer.h"
#ಅನ್ತರ್ಭಾವ "sparse-index.h"
#ಅನ್ತರ್ಭಾವ "mailmap.h"
#ಅನ್ತರ್ಭಾವ "help.h"
#ಅನ್ತರ್ಭಾವ "commit-reach.h"
#ಅನ್ತರ್ಭಾವ "commit-graph.h"
#ಅನ್ತರ್ಭಾವ "pretty.h"
#ಅನ್ತರ್ಭಾವ "trailer.h"

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ builtin_commit_usage[] = {
	N_("git commit [-a | --interactive | --patch] [-s] [-v] [-u[<mode>]] [--amend]\n"
	   "           [--dry-run] [(-c | -C | --squash) <commit> | --fixup [(amend|reword):]<commit>]\n"
	   "           [-F <file> | -m <msg>] [--reset-author] [--allow-empty]\n"
	   "           [--allow-empty-message] [--no-verify] [-e] [--author=<author>]\n"
	   "           [--date=<date>] [--cleanup=<mode>] [--[no-]status]\n"
	   "           [-i | -o] [--pathspec-from-file=<file> [--pathspec-file-nul]]\n"
	   "           [(--trailer <token>[(=|:)<value>])...] [-S[<keyid>]]\n"
	   "           [--] [<pathspec>...]"),
	ಖಾಲಿ
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ builtin_ಸ್ಥಿತಿ_usage[] = {
	N_("git status [<options>] [--] [<pathspec>...]"),
	ಖಾಲಿ
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ ಖಾಲಿಗಳು_amend_advice[] =
N_("You asked to amend the most recent commit, but doing so would make\n"
"it empty. You can repeat your command with --allow-empty, or you can\n"
"remove the commit entirely with \"git reset HEAD^\".\n");

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ ಖಾಲಿಗಳು_cherry_pick_advice[] =
N_("The previous cherry-pick is now empty, possibly due to conflict resolution.\n"
"If you wish to commit it anyway, use:\n"
"\n"
"    git commit --allow-empty\n"
"\n");

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ ಖಾಲಿಗಳು_rebase_pick_advice[] =
N_("Otherwise, please use 'git rebase --skip'\n");

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ ಖಾಲಿಗಳು_cherry_pick_advice_single[] =
N_("Otherwise, please use 'git cherry-pick --skip'\n");

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ ಖಾಲಿಗಳು_cherry_pick_advice_multi[] =
N_("and then use:\n"
"\n"
"    git cherry-pick --continue\n"
"\n"
"to resume cherry-picking the remaining commits.\n"
"If you wish to skip this commit, use:\n"
"\n"
"    git cherry-pick --skip\n"
"\n");

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *color_ಸ್ಥಿತಿ_slots[] = {
	[WT_ಸ್ಥಿತಿ_ಶೀರ್ಷಿಕೆ]	  = "header",
	[WT_ಸ್ಥಿತಿ_UPDATED]	  = "updated",
	[WT_ಸ್ಥಿತಿ_ಬದಲಾಯಿತು]	  = "changed",
	[WT_ಸ್ಥಿತಿ_UNTRACKED]	  = "untracked",
	[WT_ಸ್ಥಿತಿ_NOBRANCH]	  = "noBranch",
	[WT_ಸ್ಥಿತಿ_UNMERGED]	  = "unmerged",
	[WT_ಸ್ಥಿತಿ_LOCAL_BRANCH]  = "localBranch",
	[WT_ಸ್ಥಿತಿ_REMOTE_BRANCH] = "remoteBranch",
	[WT_ಸ್ಥಿತಿ_ONBRANCH]	  = "branch",
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *use_ಸಂದೇಶ_ಬಫರ್;
ಸ್ಥಿರ ರಚನೆ ಬೀಗ_file ಸೂಚ್ಯಂಕ_ಬೀಗ; /* real index */
ಸ್ಥಿರ ರಚನೆ ಬೀಗ_file false_ಬೀಗ; /* used only for partial commits */
ಸ್ಥಿರ ಎಣಿಕೆ {
	COMMIT_AS_IS = 1,
	COMMIT_ಸಾಮಾನ್ಯ,
	COMMIT_PARTIAL
} commit_style;

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *force_author;
ಸ್ಥಿರ ಅಕ್ಷರ *logfile;
ಸ್ಥಿರ ಅಕ್ಷರ *template_file;
/*
 * The _message variables are commit names from which to take
 * the commit message and/or authorship.
 */
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *author_ಸಂದೇಶ, *author_ಸಂದೇಶ_ಬಫರ್;
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *edit_ಸಂದೇಶ, *use_ಸಂದೇಶ;
ಸ್ಥಿರ ಅಕ್ಷರ *fixup_ಸಂದೇಶ, *fixup_commit, *squash_ಸಂದೇಶ;
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fixup_ಪೂರ್ವಪ್ರತ್ಯಯ;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ all, also, interactive, patch_interactive, only, amend, signoff;
ಸ್ಥಿರ ರಚನೆ interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ edit_flag = -1; /* unspecified */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ quiet, verbose, no_verify, allow_ಖಾಲಿಗಳು, dry_ಸತತ, renew_authorship;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ config_commit_verbose = -1; /* unspecified */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ no_post_rewrite, allow_ಖಾಲಿಗಳು_ಸಂದೇಶ, pathspec_file_nul;
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *untracked_files_arg, *force_date, *ignore_submodule_arg, *ignored_arg;
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *sign_commit, *pathspec_from_file;
ಸ್ಥಿರ ರಚನೆ strvec trailer_ವಾದಗಳು = STRVEC_INIT;

/*
 * The default commit message cleanup mode will remove the lines
 * beginning with # (shell comments) and leading and trailing
 * whitespaces (empty lines or containing only whitespaces)
 * if editor is used, and only the whitespaces if the message
 * is specified explicitly.
 */
ಸ್ಥಿರ ಎಣಿಕೆ commit_ಸಂದೇಶ_cleanup_ಬಹುಲಕ cleanup_ಬಹುಲಕ;
ಸ್ಥಿರ ಅಕ್ಷರ *cleanup_config;

ಸ್ಥಿರ ಎಣಿಕೆ commit_whence whence;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ use_editor = 1, include_ಸ್ಥಿತಿ = 1;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ have_option_m;
ಸ್ಥಿರ ರಚನೆ strbuf ಸಂದೇಶ_2 = STRBUF_INIT;

ಸ್ಥಿರ ಎಣಿಕೆ wt_ಸ್ಥಿತಿ_format ಸ್ಥಿತಿ_format = ಸ್ಥಿತಿ_FORMAT_UNSPECIFIED;

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ opt_parse_porcelain(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ಎಣಿಕೆ wt_ಸ್ಥಿತಿ_format *ಮೌಲ್ಯ = (ಎಣಿಕೆ wt_ಸ್ಥಿತಿ_format *)opt->ಮೌಲ್ಯ;
	ಆದರೆ (unset)
		*ಮೌಲ್ಯ = ಸ್ಥಿತಿ_FORMAT_NONE;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!arg)
		*ಮೌಲ್ಯ = ಸ್ಥಿತಿ_FORMAT_PORCELAIN;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(arg, "v1") || !strcmp(arg, "1"))
		*ಮೌಲ್ಯ = ಸ್ಥಿತಿ_FORMAT_PORCELAIN;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(arg, "v2") || !strcmp(arg, "2"))
		*ಮೌಲ್ಯ = ಸ್ಥಿತಿ_FORMAT_PORCELAIN_V2;
	ಇಲ್ಲದಿದ್ದರೆ
		die("unsupported porcelain version '%s'", arg);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ opt_parse_m(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ರಚನೆ strbuf *buf = opt->ಮೌಲ್ಯ;
	ಆದರೆ (unset) {
		have_option_m = 0;
		strbuf_setlen(buf, 0);
	} ಇಲ್ಲದಿದ್ದರೆ {
		have_option_m = 1;
		ಆದರೆ (buf->len)
			strbuf_addch(buf, '\n');
		strbuf_addstr(buf, arg);
		strbuf_complete_ಸಾಲು(buf);
	}
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ opt_parse_rename_score(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **ಮೌಲ್ಯ = opt->ಮೌಲ್ಯ;

	BUG_ON_OPT_NEG(unset);

	ಆದರೆ (arg != ಖಾಲಿ && *arg == '=')
		arg = arg + 1;

	*ಮೌಲ್ಯ = arg;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ determine_whence(ರಚನೆ wt_ಸ್ಥಿತಿ *s)
{
	ಆದರೆ (file_exists(git_ಮಾರ್ಗ_ವಿಲೀನ_ತಲೆ(the_repository)))
		whence = FROM_ವಿಲೀನ;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!sequencer_determine_whence(the_repository, &whence))
		whence = FROM_COMMIT;
	ಆದರೆ (s)
		s->whence = whence;
}

ಸ್ಥಿರ ಶೂನ್ಯ ಸ್ಥಿತಿ_init_config(ರಚನೆ wt_ಸ್ಥಿತಿ *s, config_fn_t fn)
{
	wt_ಸ್ಥಿತಿ_prepare(the_repository, s);
	init_diff_ui_defaults();
	repo_config(the_repository, fn, s);
	determine_whence(s);
	s->hints = advice_enabled(ADVICE_ಸ್ಥಿತಿ_HINTS); /* must come after repo_config() */
}

ಸ್ಥಿರ ಶೂನ್ಯ rollback_ಸೂಚ್ಯಂಕ_files(ಶೂನ್ಯ)
{
	ಆಯ್ಕೆ (commit_style) {
	ಸ್ಥಿತಿ COMMIT_AS_IS:
		ನಿಲ್ಲಿಸು; /* nothing to do */
	ಸ್ಥಿತಿ COMMIT_ಸಾಮಾನ್ಯ:
		rollback_ಬೀಗ_file(&ಸೂಚ್ಯಂಕ_ಬೀಗ);
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ COMMIT_PARTIAL:
		rollback_ಬೀಗ_file(&ಸೂಚ್ಯಂಕ_ಬೀಗ);
		rollback_ಬೀಗ_file(&false_ಬೀಗ);
		ನಿಲ್ಲಿಸು;
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ commit_ಸೂಚ್ಯಂಕ_files(ಶೂನ್ಯ)
{
	ಪೂರ್ಣಾಂಕ err = 0;

	ಆಯ್ಕೆ (commit_style) {
	ಸ್ಥಿತಿ COMMIT_AS_IS:
		ನಿಲ್ಲಿಸು; /* nothing to do */
	ಸ್ಥಿತಿ COMMIT_ಸಾಮಾನ್ಯ:
		err = commit_ಬೀಗ_file(&ಸೂಚ್ಯಂಕ_ಬೀಗ);
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ COMMIT_PARTIAL:
		err = commit_ಬೀಗ_file(&ಸೂಚ್ಯಂಕ_ಬೀಗ);
		rollback_ಬೀಗ_file(&false_ಬೀಗ);
		ನಿಲ್ಲಿಸು;
	}

	ಹಿಂತಿರುಗಿ err;
}

/*
 * Take a union of paths in the index and the named tree (typically, "HEAD"),
 * and return the paths that match the given pattern in list.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ list_paths(ರಚನೆ string_list *list, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *with_ವೃಕ್ಷ,
		      ಸ್ಥಿರಾಂಕ ರಚನೆ pathspec *ನಮೂನೆ)
{
	ಪೂರ್ಣಾಂಕ i, ret;
	ಅಕ್ಷರ *m;

	ಆದರೆ (!ನಮೂನೆ->nr)
		ಹಿಂತಿರುಗಿ 0;

	m = xcalloc(1, ನಮೂನೆ->nr);

	ಆದರೆ (with_ವೃಕ್ಷ) {
		ಅಕ್ಷರ *max_ಪೂರ್ವಪ್ರತ್ಯಯ = common_ಪೂರ್ವಪ್ರತ್ಯಯ(ನಮೂನೆ);
		overlay_ವೃಕ್ಷ_on_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ, with_ವೃಕ್ಷ, max_ಪೂರ್ವಪ್ರತ್ಯಯ);
		ಬಿಡುಗಡೆ(max_ಪೂರ್ವಪ್ರತ್ಯಯ);
	}

	/* TODO: audit for interaction with sparse-index. */
	ensure_ತುಂಬಿದ_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ);
	ಗಾಗಿ (i = 0; i < the_repository->ಸೂಚ್ಯಂಕ->cache_nr; i++) {
		ಸ್ಥಿರಾಂಕ ರಚನೆ cache_ನಮೂದು *ce = the_repository->ಸೂಚ್ಯಂಕ->cache[i];
		ರಚನೆ string_list_ಅಂಶ *ಅಂಶ;

		ಆದರೆ (ce->ce_flags & CE_UPDATE)
			ಮುಂದುವರಿ;
		ಆದರೆ (!ce_ಮಾರ್ಗ_ಹೊಂದಿಕೆ(the_repository->ಸೂಚ್ಯಂಕ, ce, ನಮೂನೆ, m))
			ಮುಂದುವರಿ;
		ಅಂಶ = string_list_ಸೇರಿಸು(list, ce->name);
		ಆದರೆ (ce_skip_worktree(ce))
			ಅಂಶ->util = ಅಂಶ; /* better a valid pointer than a fake one */
	}

	ret = report_ಮಾರ್ಗ_error(m, ನಮೂನೆ);
	ಬಿಡುಗಡೆ(m);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಶೂನ್ಯ ಕೂಡಿಸು_remove_files(ರಚನೆ string_list *list)
{
	ಪೂರ್ಣಾಂಕ i;
	ಗಾಗಿ (i = 0; i < list->nr; i++) {
		ರಚನೆ stat st;
		ರಚನೆ string_list_ಅಂಶ *p = &(list->ವಸ್ತುಗಳು[i]);

		/* p->util is skip-worktree */
		ಆದರೆ (p->util)
			ಮುಂದುವರಿ;

		ಆದರೆ (!lstat(p->string, &st)) {
			ಆದರೆ (ಕೂಡಿಸು_ಗಮ್ಯ_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ, p->string, &st, 0))
				die(_("updating files failed"));
		} ಇಲ್ಲದಿದ್ದರೆ
			remove_file_from_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ, p->string);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ create_ತಳ_ಸೂಚ್ಯಂಕ(ಸ್ಥಿರಾಂಕ ರಚನೆ commit *current_ತಲೆ)
{
	ರಚನೆ ವೃಕ್ಷ *ವೃಕ್ಷ;
	ರಚನೆ unpack_trees_options opts;
	ರಚನೆ ವೃಕ್ಷ_desc t;

	ಆದರೆ (!current_ತಲೆ) {
		discard_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ);
		ಹಿಂತಿರುಗಿ;
	}

	memset(&opts, 0, ಗಾತ್ರ(opts));
	opts.ತಲೆ_idx = 1;
	opts.ಸೂಚ್ಯಂಕ_only = 1;
	opts.ವಿಲೀನ = 1;
	opts.src_ಸೂಚ್ಯಂಕ = the_repository->ಸೂಚ್ಯಂಕ;
	opts.dst_ಸೂಚ್ಯಂಕ = the_repository->ಸೂಚ್ಯಂಕ;

	opts.fn = oneway_ವಿಲೀನ;
	ವೃಕ್ಷ = repo_parse_ವೃಕ್ಷ_ಪರೋಕ್ಷ(the_repository,
					&current_ತಲೆ->object.oid);
	ಆದರೆ (!ವೃಕ್ಷ)
		die(_("failed to unpack HEAD tree object"));
	ಆದರೆ (repo_parse_ವೃಕ್ಷ(the_repository, ವೃಕ್ಷ) < 0)
		ನಿರ್ಗಮಿಸು(128);
	init_ವೃಕ್ಷ_desc(&t, &ವೃಕ್ಷ->object.oid, ವೃಕ್ಷ->ಬಫರ್, ವೃಕ್ಷ->ಗಾತ್ರ_2);
	ಆದರೆ (unpack_trees(1, &t, &opts))
		ನಿರ್ಗಮಿಸು(128); /* We've already reported the error, finish dying */
}

ಸ್ಥಿರ ಶೂನ್ಯ refresh_cache_or_die(ಪೂರ್ಣಾಂಕ refresh_flags)
{
	/*
	 * refresh_flags contains REFRESH_QUIET, so the only errors
	 * are for unmerged entries.
	 */
	ಆದರೆ (refresh_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ, refresh_flags | REFRESH_ಒಳ_PORCELAIN, ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ))
		die_resolve_conflict("commit");
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *prepare_ಸೂಚ್ಯಂಕ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
				 ಸ್ಥಿರಾಂಕ ರಚನೆ commit *current_ತಲೆ, ಪೂರ್ಣಾಂಕ is_ಸ್ಥಿತಿ)
{
	ರಚನೆ string_list partial = STRING_LIST_INIT_DUP;
	ರಚನೆ pathspec pathspec;
	ಪೂರ್ಣಾಂಕ refresh_flags = REFRESH_QUIET;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ret;
	ಅಕ್ಷರ *ಮಾರ್ಗ = ಖಾಲಿ;

	ಆದರೆ (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	ಆದರೆ (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	ಆದರೆ (is_ಸ್ಥಿತಿ)
		refresh_flags |= REFRESH_UNMERGED;
	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_ತುಂಬಿದ,
		       ಪೂರ್ವಪ್ರತ್ಯಯ, argv);

	ಆದರೆ (pathspec_from_file) {
		ಆದರೆ (interactive)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--interactive/--patch");

		ಆದರೆ (all)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "-a");

		ಆದರೆ (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_ತುಂಬಿದ,
				    ಪೂರ್ವಪ್ರತ್ಯಯ, pathspec_from_file, pathspec_file_nul);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	ಆದರೆ (!pathspec.nr && (also || (only && !allow_ಖಾಲಿಗಳು &&
	    (!amend || (fixup_ಸಂದೇಶ && strcmp(fixup_ಪೂರ್ವಪ್ರತ್ಯಯ, "amend"))))))
		die(_("No paths with --include/--only does not make sense."));

	ಆದರೆ (repo_read_ಸೂಚ್ಯಂಕ_preload(the_repository, &pathspec, 0) < 0)
		die(_("index file corrupt"));

	ಆದರೆ (interactive) {
		ಅಕ್ಷರ *old_ಸೂಚ್ಯಂಕ_env = ಖಾಲಿ, *old_repo_ಸೂಚ್ಯಂಕ_file;
		repo_hold_locked_ಸೂಚ್ಯಂಕ(the_repository, &ಸೂಚ್ಯಂಕ_ಬೀಗ,
				       ಬೀಗ_DIE_ON_ERROR);

		refresh_cache_or_die(refresh_flags);

		ಆದರೆ (write_locked_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ, &ಸೂಚ್ಯಂಕ_ಬೀಗ, 0))
			die(_("unable to create temporary index"));

		old_repo_ಸೂಚ್ಯಂಕ_file = the_repository->ಸೂಚ್ಯಂಕ_file;
		the_repository->ಸೂಚ್ಯಂಕ_file =
			(ಅಕ್ಷರ *)get_ಬೀಗ_file_ಮಾರ್ಗ(&ಸೂಚ್ಯಂಕ_ಬೀಗ);
		old_ಸೂಚ್ಯಂಕ_env = xstrdup_or_null(getenv(ಸೂಚ್ಯಂಕ_ENVIRONMENT));
		setenv(ಸೂಚ್ಯಂಕ_ENVIRONMENT, the_repository->ಸೂಚ್ಯಂಕ_file, 1);

		ಆದರೆ (interactive_ಕೂಡಿಸು(the_repository, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, patch_interactive, &interactive_opts) != 0)
			die(_("interactive add failed"));

		the_repository->ಸೂಚ್ಯಂಕ_file = old_repo_ಸೂಚ್ಯಂಕ_file;
		ಆದರೆ (old_ಸೂಚ್ಯಂಕ_env && *old_ಸೂಚ್ಯಂಕ_env)
			setenv(ಸೂಚ್ಯಂಕ_ENVIRONMENT, old_ಸೂಚ್ಯಂಕ_env, 1);
		ಇಲ್ಲದಿದ್ದರೆ
			unsetenv(ಸೂಚ್ಯಂಕ_ENVIRONMENT);
		FREE_AND_NULL(old_ಸೂಚ್ಯಂಕ_env);

		discard_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ);
		read_ಸೂಚ್ಯಂಕ_from(the_repository->ಸೂಚ್ಯಂಕ, get_ಬೀಗ_file_ಮಾರ್ಗ(&ಸೂಚ್ಯಂಕ_ಬೀಗ),
				repo_get_git_dir(the_repository));
		ಆದರೆ (cache_ವೃಕ್ಷ_update(the_repository->ಸೂಚ್ಯಂಕ, WRITE_ವೃಕ್ಷ_SILENT) == 0) {
			ಆದರೆ (reopen_ಬೀಗ_file(&ಸೂಚ್ಯಂಕ_ಬೀಗ) < 0)
				die(_("unable to write index file"));
			ಆದರೆ (write_locked_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ, &ಸೂಚ್ಯಂಕ_ಬೀಗ, 0))
				die(_("unable to update temporary index"));
		} ಇಲ್ಲದಿದ್ದರೆ
			warning(_("Failed to update main cache tree"));

		commit_style = COMMIT_ಸಾಮಾನ್ಯ;
		ret = get_ಬೀಗ_file_ಮಾರ್ಗ(&ಸೂಚ್ಯಂಕ_ಬೀಗ);
		ಹೋಗು ಹೊರಹೋಗು;
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಆದರೆ (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--interactive/--patch");
		ಆದರೆ (interactive_opts.interhunkcontext != -1)
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
	ಆದರೆ (all || (also && pathspec.nr)) {
		ಅಕ್ಷರ *ps_matched = xcalloc(pathspec.nr, 1);
		repo_hold_locked_ಸೂಚ್ಯಂಕ(the_repository, &ಸೂಚ್ಯಂಕ_ಬೀಗ,
				       ಬೀಗ_DIE_ON_ERROR);
		ಕೂಡಿಸು_files_ಗಮ್ಯ_cache(the_repository, also ? ಪೂರ್ವಪ್ರತ್ಯಯ : ಖಾಲಿ,
				   &pathspec, ps_matched, 0, 0, 0 );
		ಆದರೆ (!all && report_ಮಾರ್ಗ_error(ps_matched, &pathspec))
			ನಿರ್ಗಮಿಸು(128);

		refresh_cache_or_die(refresh_flags);
		cache_ವೃಕ್ಷ_update(the_repository->ಸೂಚ್ಯಂಕ, WRITE_ವೃಕ್ಷ_SILENT);
		ಆದರೆ (write_locked_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ, &ಸೂಚ್ಯಂಕ_ಬೀಗ, 0))
			die(_("unable to write new index file"));
		commit_style = COMMIT_ಸಾಮಾನ್ಯ;
		ret = get_ಬೀಗ_file_ಮಾರ್ಗ(&ಸೂಚ್ಯಂಕ_ಬೀಗ);
		ಬಿಡುಗಡೆ(ps_matched);
		ಹೋಗು ಹೊರಹೋಗು;
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
	ಆದರೆ (!only && !pathspec.nr) {
		repo_hold_locked_ಸೂಚ್ಯಂಕ(the_repository, &ಸೂಚ್ಯಂಕ_ಬೀಗ,
				       ಬೀಗ_DIE_ON_ERROR);
		refresh_cache_or_die(refresh_flags);
		ಆದರೆ (the_repository->ಸೂಚ್ಯಂಕ->cache_ಬದಲಾಯಿತು
		    || !cache_ವೃಕ್ಷ_fully_valid(the_repository->ಸೂಚ್ಯಂಕ))
			cache_ವೃಕ್ಷ_update(the_repository->ಸೂಚ್ಯಂಕ, WRITE_ವೃಕ್ಷ_SILENT);
		ಆದರೆ (write_locked_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ, &ಸೂಚ್ಯಂಕ_ಬೀಗ,
				       COMMIT_ಬೀಗ | SKIP_IF_UNCHANGED))
			die(_("unable to write new index file"));
		commit_style = COMMIT_AS_IS;
		ret = repo_get_ಸೂಚ್ಯಂಕ_file(the_repository);
		ಹೋಗು ಹೊರಹೋಗು;
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

	ಆದರೆ (whence != FROM_COMMIT) {
		ಆದರೆ (whence == FROM_ವಿಲೀನ)
			die(_("cannot do a partial commit during a merge."));
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (is_from_cherry_pick(whence))
			die(_("cannot do a partial commit during a cherry-pick."));
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (is_from_rebase(whence))
			die(_("cannot do a partial commit during a rebase."));
	}

	ಆದರೆ (list_paths(&partial, !current_ತಲೆ ? ಖಾಲಿ : "HEAD", &pathspec))
		ನಿರ್ಗಮಿಸು(1);

	discard_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ);
	ಆದರೆ (repo_read_ಸೂಚ್ಯಂಕ(the_repository) < 0)
		die(_("cannot read the index"));

	repo_hold_locked_ಸೂಚ್ಯಂಕ(the_repository, &ಸೂಚ್ಯಂಕ_ಬೀಗ, ಬೀಗ_DIE_ON_ERROR);
	ಕೂಡಿಸು_remove_files(&partial);
	refresh_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ, REFRESH_QUIET, ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ);
	cache_ವೃಕ್ಷ_update(the_repository->ಸೂಚ್ಯಂಕ, WRITE_ವೃಕ್ಷ_SILENT);
	ಆದರೆ (write_locked_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ, &ಸೂಚ್ಯಂಕ_ಬೀಗ, 0))
		die(_("unable to write new index file"));

	ಮಾರ್ಗ = repo_git_ಮಾರ್ಗ(the_repository, "next-index-%"PRIuMAX,
			     (uintmax_t) getpid());
	hold_ಬೀಗ_file_for_update(&false_ಬೀಗ, ಮಾರ್ಗ,
				  ಬೀಗ_DIE_ON_ERROR);

	create_ತಳ_ಸೂಚ್ಯಂಕ(current_ತಲೆ);
	ಕೂಡಿಸು_remove_files(&partial);
	refresh_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ, REFRESH_QUIET, ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ);

	ಆದರೆ (write_locked_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ, &false_ಬೀಗ, 0))
		die(_("unable to write temporary index file"));

	discard_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ);
	ret = get_ಬೀಗ_file_ಮಾರ್ಗ(&false_ಬೀಗ);
	read_ಸೂಚ್ಯಂಕ_from(the_repository->ಸೂಚ್ಯಂಕ, ret, repo_get_git_dir(the_repository));
ಹೊರಹೋಗು:
	string_list_clear(&partial, 0);
	clear_pathspec(&pathspec);
	ಬಿಡುಗಡೆ(ಮಾರ್ಗ);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಸತತ_ಸ್ಥಿತಿ(FILE *fp, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸೂಚ್ಯಂಕ_file, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ, ಪೂರ್ಣಾಂಕ nowarn,
		      ರಚನೆ wt_ಸ್ಥಿತಿ *s)
{
	ರಚನೆ object_id oid;

	ಆದರೆ (s->relative_paths)
		s->ಪೂರ್ವಪ್ರತ್ಯಯ = ಪೂರ್ವಪ್ರತ್ಯಯ;

	ಆದರೆ (amend) {
		s->amend = 1;
		s->reference = "HEAD^1";
	}
	s->verbose = verbose;
	s->ಸೂಚ್ಯಂಕ_file = ಸೂಚ್ಯಂಕ_file;
	s->fp = fp;
	s->nowarn = nowarn;
	s->is_initial = repo_get_oid(the_repository, s->reference, &oid) ? 1 : 0;
	ಆದರೆ (!s->is_initial)
		oidcpy(&s->oid_commit, &oid);
	s->ಸ್ಥಿತಿ_format = ಸ್ಥಿತಿ_format;
	s->ignore_submodule_arg = ignore_submodule_arg;

	wt_ಸ್ಥಿತಿ_collect(s);
	wt_ಸ್ಥಿತಿ_print(s);
	wt_ಸ್ಥಿತಿ_collect_free_buffers(s);

	ಹಿಂತಿರುಗಿ s->committable;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ is_a_ವಿಲೀನ(ಸ್ಥಿರಾಂಕ ರಚನೆ commit *current_ತಲೆ)
{
	ಹಿಂತಿರುಗಿ !!(current_ತಲೆ->parents && current_ತಲೆ->parents->ಮುಂದಿನ);
}

ಸ್ಥಿರ ಶೂನ್ಯ assert_ವಿಭಜಿಸು_ident(ರಚನೆ ident_ವಿಭಜಿಸು *id, ಸ್ಥಿರಾಂಕ ರಚನೆ strbuf *buf)
{
	ಆದರೆ (ವಿಭಜಿಸು_ident_ಸಾಲು(id, buf->buf, buf->len) || !id->date_begin)
		BUG("unable to parse our own ident: %s", buf->buf);
}

ಸ್ಥಿರ ಶೂನ್ಯ export_one(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *var, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *e, ಪೂರ್ಣಾಂಕ hack)
{
	ರಚನೆ strbuf buf = STRBUF_INIT;
	ಆದರೆ (hack)
		strbuf_addch(&buf, hack);
	strbuf_ಕೂಡಿಸು(&buf, s, e - s);
	setenv(var, buf.buf, 1);
	strbuf_release(&buf);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ parse_force_date(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಒಳ, ರಚನೆ strbuf *ಹೊರಹೋಗು)
{
	strbuf_addch(ಹೊರಹೋಗು, '@');

	ಆದರೆ (parse_date(ಒಳ, ಹೊರಹೋಗು) < 0) {
		ಪೂರ್ಣಾಂಕ errors = 0;
		ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ t = approxidate_careful(ಒಳ, &errors);
		ಆದರೆ (errors)
			ಹಿಂತಿರುಗಿ -1;
		strbuf_addf(ಹೊರಹೋಗು, "%lu", t);
	}

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ ಗಣ_ident_var(ಅಕ್ಷರ **buf, ಅಕ್ಷರ *val)
{
	ಬಿಡುಗಡೆ(*buf);
	*buf = val;
}

ಸ್ಥಿರ ಶೂನ್ಯ determine_author_info(ರಚನೆ strbuf *author_ident)
{
	ಅಕ್ಷರ *name, *email, *date;
	ರಚನೆ ident_ವಿಭಜಿಸು author;

	name = xstrdup_or_null(getenv("GIT_AUTHOR_NAME"));
	email = xstrdup_or_null(getenv("GIT_AUTHOR_EMAIL"));
	date = xstrdup_or_null(getenv("GIT_AUTHOR_DATE"));

	ಆದರೆ (author_ಸಂದೇಶ) {
		ರಚನೆ ident_ವಿಭಜಿಸು ident;
		ಗಾತ್ರ_t len;
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *a;

		a = ಕಂಡುಹಿಡಿ_commit_ಶೀರ್ಷಿಕೆ(author_ಸಂದೇಶ_ಬಫರ್, "author", &len);
		ಆದರೆ (!a)
			die(_("commit '%s' lacks author header"), author_ಸಂದೇಶ);
		ಆದರೆ (ವಿಭಜಿಸು_ident_ಸಾಲು(&ident, a, len) < 0)
			die(_("commit '%s' has malformed author line"), author_ಸಂದೇಶ);

		ಗಣ_ident_var(&name, xmemdupz(ident.name_begin, ident.name_ಕೊನೆ - ident.name_begin));
		ಗಣ_ident_var(&email, xmemdupz(ident.mail_begin, ident.mail_ಕೊನೆ - ident.mail_begin));

		ಆದರೆ (ident.date_begin) {
			ರಚನೆ strbuf date_buf = STRBUF_INIT;
			strbuf_addch(&date_buf, '@');
			strbuf_ಕೂಡಿಸು(&date_buf, ident.date_begin, ident.date_ಕೊನೆ - ident.date_begin);
			strbuf_addch(&date_buf, ' ');
			strbuf_ಕೂಡಿಸು(&date_buf, ident.tz_begin, ident.tz_ಕೊನೆ - ident.tz_begin);
			ಗಣ_ident_var(&date, strbuf_detach(&date_buf, ಖಾಲಿ));
		}
	}

	ಆದರೆ (force_author) {
		ರಚನೆ ident_ವಿಭಜಿಸು ident;

		ಆದರೆ (ವಿಭಜಿಸು_ident_ಸಾಲು(&ident, force_author, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(force_author)) < 0)
			die(_("malformed --author parameter"));
		ಗಣ_ident_var(&name, xmemdupz(ident.name_begin, ident.name_ಕೊನೆ - ident.name_begin));
		ಗಣ_ident_var(&email, xmemdupz(ident.mail_begin, ident.mail_ಕೊನೆ - ident.mail_begin));
	}

	ಆದರೆ (force_date) {
		ರಚನೆ strbuf date_buf = STRBUF_INIT;
		ಆದರೆ (parse_force_date(force_date, &date_buf))
			die(_("invalid date format: %s"), force_date);
		ಗಣ_ident_var(&date, strbuf_detach(&date_buf, ಖಾಲಿ));
	}

	strbuf_addstr(author_ident, fmt_ident(name, email, WANT_AUTHOR_IDENT, date,
				IDENT_STRICT));
	assert_ವಿಭಜಿಸು_ident(&author, author_ident);
	export_one("GIT_AUTHOR_NAME", author.name_begin, author.name_ಕೊನೆ, 0);
	export_one("GIT_AUTHOR_EMAIL", author.mail_begin, author.mail_ಕೊನೆ, 0);
	export_one("GIT_AUTHOR_DATE", author.date_begin, author.tz_ಕೊನೆ, '@');
	ಬಿಡುಗಡೆ(name);
	ಬಿಡುಗಡೆ(email);
	ಬಿಡುಗಡೆ(date);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ author_date_is_interesting(ಶೂನ್ಯ)
{
	ಹಿಂತಿರುಗಿ author_ಸಂದೇಶ || force_date;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ WITH_BREAKING_CHANGES
ಸ್ಥಿರ ಶೂನ್ಯ adjust_comment_ಸಾಲು_char(ಸ್ಥಿರಾಂಕ ರಚನೆ strbuf *sb)
{
	ಅಕ್ಷರ candidates[] = "#;@!$%^&|:";
	ಅಕ್ಷರ *candidate;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *p;
	ಗಾತ್ರ_t cutoff;

	/* Ignore comment chars in trailing comments (e.g., Conflicts:) */
	cutoff = sb->len - ignored_log_ಸಂದೇಶ_ಬೈಟುಗಳು(sb->buf, sb->len);

	ಆದರೆ (!memchr(sb->buf, candidates[0], sb->len)) {
		ಬಿಡುಗಡೆ(comment_ಸಾಲು_str_ಗಮ್ಯ_free);
		comment_ಸಾಲು_str = comment_ಸಾಲು_str_ಗಮ್ಯ_free =
			xstrfmt("%c", candidates[0]);
		ಹಿಂತಿರುಗಿ;
	}

	p = sb->buf;
	candidate = strchr(candidates, *p);
	ಆದರೆ (candidate)
		*candidate = ' ';
	ಗಾಗಿ (p = sb->buf; p + 1 < sb->buf + cutoff; p++) {
		ಆದರೆ ((p[0] == '\n' || p[0] == '\r') && p[1]) {
			candidate = strchr(candidates, p[1]);
			ಆದರೆ (candidate)
				*candidate = ' ';
		}
	}

	ಗಾಗಿ (p = candidates; *p == ' '; p++)
		;
	ಆದರೆ (!*p)
		die(_("unable to select a comment character that is not used\n"
		      "in the current commit message"));
	ಬಿಡುಗಡೆ(comment_ಸಾಲು_str_ಗಮ್ಯ_free);
	comment_ಸಾಲು_str = comment_ಸಾಲು_str_ಗಮ್ಯ_free = xstrfmt("%c", *p);
}
#ಆದರೆ_ಅಂತ್ಯ /* !WITH_BREAKING_CHANGES */

ಸ್ಥಿರ ಶೂನ್ಯ prepare_amend_commit(ರಚನೆ commit *commit, ರಚನೆ strbuf *sb,
				ರಚನೆ pretty_print_context *ctx)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಬಫರ್, *subject, *fmt;

	ಬಫರ್ = repo_get_commit_ಬಫರ್(the_repository, commit, ಖಾಲಿ);
	ಕಂಡುಹಿಡಿ_commit_subject(ಬಫರ್, &subject);
	/*
	 * If we amend the 'amend!' commit then we don't want to
	 * duplicate the subject line.
	 */
	fmt = starts_with(subject, "amend!") ? "%b" : "%B";
	repo_format_commit_ಸಂದೇಶ(the_repository, commit, fmt, sb, ctx);
	repo_unuse_commit_ಬಫರ್(the_repository, commit, ಬಫರ್);
}

ಸ್ಥಿರ ಶೂನ್ಯ change_ದತ್ತಾಂಶ_free(ಶೂನ್ಯ *util, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *str ಬಳಸದ)
{
	ರಚನೆ wt_ಸ್ಥಿತಿ_change_ದತ್ತಾಂಶ *d = util;
	ಬಿಡುಗಡೆ(d->rename_ಮೂಲ);
	ಬಿಡುಗಡೆ(d);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ prepare_ಗಮ್ಯ_commit(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸೂಚ್ಯಂಕ_file, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			     ರಚನೆ commit *current_ತಲೆ,
			     ರಚನೆ wt_ಸ್ಥಿತಿ *s,
			     ರಚನೆ strbuf *author_ident)
{
	ರಚನೆ stat statbuf;
	ರಚನೆ strbuf committer_ident = STRBUF_INIT;
	ಪೂರ್ಣಾಂಕ committable;
	ರಚನೆ strbuf sb = STRBUF_INIT;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *hook_arg1 = ಖಾಲಿ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *hook_arg2 = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ clean_ಸಂದೇಶ_contents = (cleanup_ಬಹುಲಕ != COMMIT_ಸಂದೇಶ_CLEANUP_NONE);
	ಪೂರ್ಣಾಂಕ old_display_comment_ಪೂರ್ವಪ್ರತ್ಯಯ;
	ಪೂರ್ಣಾಂಕ invoked_hook;

	/* This checks and barfs if author is badly specified */
	determine_author_info(author_ident);

	ಆದರೆ (!no_verify && ಸತತ_commit_hook(use_editor, ಸೂಚ್ಯಂಕ_file, &invoked_hook,
					  "pre-commit", ಖಾಲಿ))
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (squash_ಸಂದೇಶ) {
		/*
		 * Insert the proper subject line before other commit
		 * message options add their content.
		 */
		ಆದರೆ (use_ಸಂದೇಶ && !strcmp(use_ಸಂದೇಶ, squash_ಸಂದೇಶ))
			strbuf_addstr(&sb, "squash! ");
		ಇಲ್ಲದಿದ್ದರೆ {
			ರಚನೆ pretty_print_context ctx = {0};
			ರಚನೆ commit *c;
			c = lookup_commit_reference_by_name(squash_ಸಂದೇಶ);
			ಆದರೆ (!c)
				die(_("could not lookup commit '%s'"), squash_ಸಂದೇಶ);
			ctx.output_encoding = get_commit_output_encoding();
			repo_format_commit_ಸಂದೇಶ(the_repository, c,
						   "squash! %s\n\n", &sb,
						   &ctx);
		}
	}

	ಆದರೆ (have_option_m && !fixup_ಸಂದೇಶ) {
		strbuf_addbuf(&sb, &ಸಂದೇಶ_2);
		hook_arg1 = "message";
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (logfile && !strcmp(logfile, "-")) {
		ಆದರೆ (isatty(0))
			fprintf(stderr, _("(reading log message from standard input)\n"));
		ಆದರೆ (strbuf_read(&sb, 0, 0) < 0)
			die_errno(_("could not read log from standard input"));
		hook_arg1 = "message";
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (logfile) {
		ಆದರೆ (strbuf_read_file(&sb, logfile, 0) < 0)
			die_errno(_("could not read log file '%s'"),
				  logfile);
		hook_arg1 = "message";
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (use_ಸಂದೇಶ) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಬಫರ್;
		ಬಫರ್ = ಅಕ್ಷರಸರಣಿ_ಹುಡುಕು(use_ಸಂದೇಶ_ಬಫರ್, "\n\n");
		ಆದರೆ (ಬಫರ್)
			strbuf_addstr(&sb, skip_blank_ಸಾಲುಗಳು(ಬಫರ್ + 2));
		hook_arg1 = "commit";
		hook_arg2 = use_ಸಂದೇಶ;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (fixup_ಸಂದೇಶ) {
		ರಚನೆ pretty_print_context ctx = {0};
		ರಚನೆ commit *commit;
		ಅಕ್ಷರ *fmt;
		commit = lookup_commit_reference_by_name(fixup_commit);
		ಆದರೆ (!commit)
			die(_("could not lookup commit '%s'"), fixup_commit);
		ctx.output_encoding = get_commit_output_encoding();
		fmt = xstrfmt("%s! %%s\n\n", fixup_ಪೂರ್ವಪ್ರತ್ಯಯ);
		repo_format_commit_ಸಂದೇಶ(the_repository, commit, fmt, &sb,
					   &ctx);
		ಬಿಡುಗಡೆ(fmt);
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
		ಆದರೆ (have_option_m && !strcmp(fixup_ಪೂರ್ವಪ್ರತ್ಯಯ, "fixup"))
			strbuf_addbuf(&sb, &ಸಂದೇಶ_2);

		ಆದರೆ (!strcmp(fixup_ಪೂರ್ವಪ್ರತ್ಯಯ, "amend")) {
			ಆದರೆ (have_option_m)
				die(_("options '%s' and '%s:%s' cannot be used together"), "-m", "--fixup", fixup_ಸಂದೇಶ);
			prepare_amend_commit(commit, &sb, &ctx);
		}
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!stat(git_ಮಾರ್ಗ_ವಿಲೀನ_ಸಂದೇಶ(the_repository), &statbuf)) {
		ಗಾತ್ರ_t ವಿಲೀನ_ಸಂದೇಶ_ಆರಂಭ;

		/*
		 * prepend SQUASH_MSG here if it exists and a
		 * "merge --squash" was originally performed
		 */
		ಆದರೆ (!stat(git_ಮಾರ್ಗ_squash_ಸಂದೇಶ(the_repository), &statbuf)) {
			ಆದರೆ (strbuf_read_file(&sb, git_ಮಾರ್ಗ_squash_ಸಂದೇಶ(the_repository), 0) < 0)
				die_errno(_("could not read SQUASH_MSG"));
			hook_arg1 = "squash";
		} ಇಲ್ಲದಿದ್ದರೆ
			hook_arg1 = "merge";

		ವಿಲೀನ_ಸಂದೇಶ_ಆರಂಭ = sb.len;
		ಆದರೆ (strbuf_read_file(&sb, git_ಮಾರ್ಗ_ವಿಲೀನ_ಸಂದೇಶ(the_repository), 0) < 0)
			die_errno(_("could not read MERGE_MSG"));

		ಆದರೆ (cleanup_ಬಹುಲಕ == COMMIT_ಸಂದೇಶ_CLEANUP_SCISSORS &&
		    wt_ಸ್ಥಿತಿ_locate_ಕೊನೆ(sb.buf + ವಿಲೀನ_ಸಂದೇಶ_ಆರಂಭ,
					 sb.len - ವಿಲೀನ_ಸಂದೇಶ_ಆರಂಭ) <
				sb.len - ವಿಲೀನ_ಸಂದೇಶ_ಆರಂಭ)
			s->added_cut_ಸಾಲು = 1;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!stat(git_ಮಾರ್ಗ_squash_ಸಂದೇಶ(the_repository), &statbuf)) {
		ಆದರೆ (strbuf_read_file(&sb, git_ಮಾರ್ಗ_squash_ಸಂದೇಶ(the_repository), 0) < 0)
			die_errno(_("could not read SQUASH_MSG"));
		hook_arg1 = "squash";
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (template_file) {
		ಆದರೆ (strbuf_read_file(&sb, template_file, 0) < 0)
			die_errno(_("could not read '%s'"), template_file);
		hook_arg1 = "template";
		clean_ಸಂದೇಶ_contents = 0;
	}

	/*
	 * The remaining cases don't modify the template message, but
	 * just set the argument(s) to the prepare-commit-msg hook.
	 */
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (whence == FROM_ವಿಲೀನ)
		hook_arg1 = "merge";
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (is_from_cherry_pick(whence) || whence == FROM_REBASE_PICK) {
		hook_arg1 = "commit";
		hook_arg2 = "CHERRY_PICK_HEAD";
	}

	ಆದರೆ (squash_ಸಂದೇಶ) {
		/*
		 * If squash_commit was used for the commit subject,
		 * then we're possibly hijacking other commit log options.
		 * Reset the hook args to tell the real story.
		 */
		hook_arg1 = "message";
		hook_arg2 = "";
	}

	s->fp = fopen_for_writing(git_ಮಾರ್ಗ_commit_editmsg());
	ಆದರೆ (!s->fp)
		die_errno(_("could not open '%s'"), git_ಮಾರ್ಗ_commit_editmsg());

	/* Ignore status.displayCommentPrefix: we do need comments in COMMIT_EDITMSG. */
	old_display_comment_ಪೂರ್ವಪ್ರತ್ಯಯ = s->display_comment_ಪೂರ್ವಪ್ರತ್ಯಯ;
	s->display_comment_ಪೂರ್ವಪ್ರತ್ಯಯ = 1;

	/*
	 * Most hints are counter-productive when the commit has
	 * already started.
	 */
	s->hints = 0;

	ಆದರೆ (clean_ಸಂದೇಶ_contents)
		strbuf_stripspace(&sb, ಖಾಲಿ);

	ಆದರೆ (signoff)
		append_signoff(&sb, ignored_log_ಸಂದೇಶ_ಬೈಟುಗಳು(sb.buf, sb.len), 0);

	ಆದರೆ (fwrite(sb.buf, 1, sb.len, s->fp) < sb.len)
		die_errno(_("could not write commit template"));

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ WITH_BREAKING_CHANGES
	ಆದರೆ (auto_comment_ಸಾಲು_char)
		adjust_comment_ಸಾಲು_char(&sb);
#ಆದರೆ_ಅಂತ್ಯ /* !WITH_BREAKING_CHANGES */
	strbuf_release(&sb);

	/* This checks if committer ident is explicitly given */
	strbuf_addstr(&committer_ident, git_committer_info(IDENT_STRICT));
	ಆದರೆ (use_editor && include_ಸ್ಥಿತಿ) {
		ಪೂರ್ಣಾಂಕ ident_shown = 0;
		ಎಣಿಕೆ git_colorbool saved_color_setting;
		ರಚನೆ ident_ವಿಭಜಿಸು ci, ai;
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *hint_cleanup_all = allow_ಖಾಲಿಗಳು_ಸಂದೇಶ ?
			_("Please enter the commit message for your changes."
			  " Lines starting\nwith '%s' will be ignored.\n") :
			_("Please enter the commit message for your changes."
			  " Lines starting\nwith '%s' will be ignored, and an empty"
			  " message aborts the commit.\n");
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *hint_cleanup_space = allow_ಖಾಲಿಗಳು_ಸಂದೇಶ ?
			_("Please enter the commit message for your changes."
			  " Lines starting\n"
			  "with '%s' will be kept; you may remove them"
			  " yourself if you want to.\n") :
			_("Please enter the commit message for your changes."
			  " Lines starting\n"
			  "with '%s' will be kept; you may remove them"
			  " yourself if you want to.\n"
			  "An empty message aborts the commit.\n");
		ಆದರೆ (whence != FROM_COMMIT) {
			ಆದರೆ (cleanup_ಬಹುಲಕ == COMMIT_ಸಂದೇಶ_CLEANUP_SCISSORS)
				wt_ಸ್ಥಿತಿ_ಕೂಡಿಸು_cut_ಸಾಲು(s);
			ಸ್ಥಿತಿ_printf_ln(
				s, GIT_COLOR_ಸಾಮಾನ್ಯ,
				whence == FROM_ವಿಲೀನ ?
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
		ಆದರೆ (cleanup_ಬಹುಲಕ == COMMIT_ಸಂದೇಶ_CLEANUP_ALL)
			ಸ್ಥಿತಿ_printf(s, GIT_COLOR_ಸಾಮಾನ್ಯ, hint_cleanup_all, comment_ಸಾಲು_str);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (cleanup_ಬಹುಲಕ == COMMIT_ಸಂದೇಶ_CLEANUP_SCISSORS) {
			ಆದರೆ (whence == FROM_COMMIT)
				wt_ಸ್ಥಿತಿ_ಕೂಡಿಸು_cut_ಸಾಲು(s);
		} ಇಲ್ಲದಿದ್ದರೆ /* COMMIT_MSG_CLEANUP_SPACE, that is. */
			ಸ್ಥಿತಿ_printf(s, GIT_COLOR_ಸಾಮಾನ್ಯ, hint_cleanup_space, comment_ಸಾಲು_str);

		/*
		 * These should never fail because they come from our own
		 * fmt_ident. They may fail the sane_ident test, but we know
		 * that the name and mail pointers will at least be valid,
		 * which is enough for our tests and printing here.
		 */
		assert_ವಿಭಜಿಸು_ident(&ai, author_ident);
		assert_ವಿಭಜಿಸು_ident(&ci, &committer_ident);

		ಆದರೆ (ident_cmp(&ai, &ci))
			ಸ್ಥಿತಿ_printf_ln(s, GIT_COLOR_ಸಾಮಾನ್ಯ,
				_("%s"
				"Author:    %.*s <%.*s>"),
				ident_shown++ ? "" : "\n",
				(ಪೂರ್ಣಾಂಕ)(ai.name_ಕೊನೆ - ai.name_begin), ai.name_begin,
				(ಪೂರ್ಣಾಂಕ)(ai.mail_ಕೊನೆ - ai.mail_begin), ai.mail_begin);

		ಆದರೆ (author_date_is_interesting())
			ಸ್ಥಿತಿ_printf_ln(s, GIT_COLOR_ಸಾಮಾನ್ಯ,
				_("%s"
				"Date:      %s"),
				ident_shown++ ? "" : "\n",
				show_ident_date(&ai, DATE_ಬಹುಲಕ(ಸಾಮಾನ್ಯ)));

		ಆದರೆ (!committer_ident_sufficiently_given())
			ಸ್ಥಿತಿ_printf_ln(s, GIT_COLOR_ಸಾಮಾನ್ಯ,
				_("%s"
				"Committer: %.*s <%.*s>"),
				ident_shown++ ? "" : "\n",
				(ಪೂರ್ಣಾಂಕ)(ci.name_ಕೊನೆ - ci.name_begin), ci.name_begin,
				(ಪೂರ್ಣಾಂಕ)(ci.mail_ಕೊನೆ - ci.mail_begin), ci.mail_begin);

		ಸ್ಥಿತಿ_printf_ln(s, GIT_COLOR_ಸಾಮಾನ್ಯ, "%s", ""); /* Add new line for clarity */

		saved_color_setting = s->use_color;
		s->use_color = GIT_COLOR_NEVER;
		committable = ಸತತ_ಸ್ಥಿತಿ(s->fp, ಸೂಚ್ಯಂಕ_file, ಪೂರ್ವಪ್ರತ್ಯಯ, 1, s);
		s->use_color = saved_color_setting;
		string_list_clear_func(&s->change, change_ದತ್ತಾಂಶ_free);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ರಚನೆ object_id oid;
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೋಷಕ = "HEAD";

		ಆದರೆ (!the_repository->ಸೂಚ್ಯಂಕ->initialized && repo_read_ಸೂಚ್ಯಂಕ(the_repository) < 0)
			die(_("Cannot read index"));

		ಆದರೆ (amend)
			ಪೋಷಕ = "HEAD^1";

		ಆದರೆ (repo_get_oid(the_repository, ಪೋಷಕ, &oid)) {
			ಪೂರ್ಣಾಂಕ i, ita_nr = 0;

			/* TODO: audit for interaction with sparse-index. */
			ensure_ತುಂಬಿದ_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ);
			ಗಾಗಿ (i = 0; i < the_repository->ಸೂಚ್ಯಂಕ->cache_nr; i++)
				ಆದರೆ (ce_intent_ಗಮ್ಯ_ಕೂಡಿಸು(the_repository->ಸೂಚ್ಯಂಕ->cache[i]))
					ita_nr++;
			committable = the_repository->ಸೂಚ್ಯಂಕ->cache_nr > ita_nr;
		} ಇಲ್ಲದಿದ್ದರೆ {
			/*
			 * Unless the user did explicitly request a submodule
			 * ignore mode by passing a command line option we do
			 * not ignore any changed submodule SHA-1s when
			 * comparing index and parent, no matter what is
			 * configured. Otherwise we won't commit any
			 * submodules which were manually staged, which would
			 * be really confusing.
			 */
			ರಚನೆ diff_flags flags = DIFF_FLAGS_INIT;
			flags.override_submodule_config = 1;
			ಆದರೆ (ignore_submodule_arg &&
			    !strcmp(ignore_submodule_arg, "all"))
				flags.ignore_submodules = 1;
			committable = ಸೂಚ್ಯಂಕ_differs_from(the_repository,
							 ಪೋಷಕ, &flags, 1);
		}
	}
	strbuf_release(&committer_ident);

	fclose(s->fp);

	ಆದರೆ (trailer_ವಾದಗಳು.nr) {
		ಆದರೆ (amend_file_with_trailers(git_ಮಾರ್ಗ_commit_editmsg(), &trailer_ವಾದಗಳು))
			die(_("unable to pass trailers to --trailers"));
		strvec_clear(&trailer_ವಾದಗಳು);
	}

	/*
	 * Reject an attempt to record a non-merge empty commit without
	 * explicit --allow-empty. In the cherry-pick case, it may be
	 * empty due to conflict resolution, which the user should okay.
	 */
	ಆದರೆ (!committable && whence != FROM_ವಿಲೀನ && !allow_ಖಾಲಿಗಳು &&
	    !(amend && is_a_ವಿಲೀನ(current_ತಲೆ))) {
		s->hints = advice_enabled(ADVICE_ಸ್ಥಿತಿ_HINTS);
		s->display_comment_ಪೂರ್ವಪ್ರತ್ಯಯ = old_display_comment_ಪೂರ್ವಪ್ರತ್ಯಯ;
		ಸತತ_ಸ್ಥಿತಿ(stdout, ಸೂಚ್ಯಂಕ_file, ಪೂರ್ವಪ್ರತ್ಯಯ, 0, s);
		ಆದರೆ (amend)
			fputs(_(ಖಾಲಿಗಳು_amend_advice), stderr);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (is_from_cherry_pick(whence) ||
			 whence == FROM_REBASE_PICK) {
			fputs(_(ಖಾಲಿಗಳು_cherry_pick_advice), stderr);
			ಆದರೆ (whence == FROM_CHERRY_PICK_SINGLE)
				fputs(_(ಖಾಲಿಗಳು_cherry_pick_advice_single), stderr);
			ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (whence == FROM_CHERRY_PICK_MULTI)
				fputs(_(ಖಾಲಿಗಳು_cherry_pick_advice_multi), stderr);
			ಇಲ್ಲದಿದ್ದರೆ
				fputs(_(ಖಾಲಿಗಳು_rebase_pick_advice), stderr);
		}
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (!no_verify && invoked_hook) {
		/*
		 * Re-read the index as the pre-commit-commit hook was invoked
		 * and could have updated it. We must do this before we invoke
		 * the editor and after we invoke run_status above.
		 */
		discard_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ);
	}
	read_ಸೂಚ್ಯಂಕ_from(the_repository->ಸೂಚ್ಯಂಕ, ಸೂಚ್ಯಂಕ_file, repo_get_git_dir(the_repository));

	ಆದರೆ (cache_ವೃಕ್ಷ_update(the_repository->ಸೂಚ್ಯಂಕ, 0)) {
		error(_("Error building trees"));
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (ಸತತ_commit_hook(use_editor, ಸೂಚ್ಯಂಕ_file, ಖಾಲಿ, "prepare-commit-msg",
			    git_ಮಾರ್ಗ_commit_editmsg(), hook_arg1, hook_arg2, ಖಾಲಿ))
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (use_editor) {
		ರಚನೆ strvec env = STRVEC_INIT;

		strvec_pushf(&env, "GIT_INDEX_FILE=%s", ಸೂಚ್ಯಂಕ_file);
		ಆದರೆ (launch_editor(git_ಮಾರ್ಗ_commit_editmsg(), ಖಾಲಿ, env.v)) {
			fprintf(stderr,
			_("Please supply the message using either -m or -F option.\n"));
			ನಿರ್ಗಮಿಸು(1);
		}
		strvec_clear(&env);
	}

	ಆದರೆ (!no_verify &&
	    ಸತತ_commit_hook(use_editor, ಸೂಚ್ಯಂಕ_file, ಖಾಲಿ, "commit-msg",
			    git_ಮಾರ್ಗ_commit_editmsg(), ಖಾಲಿ)) {
		ಹಿಂತಿರುಗಿ 0;
	}

	ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕಂಡುಹಿಡಿ_author_by_nickname(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name)
{
	ರಚನೆ rev_info revs;
	ರಚನೆ commit *commit;
	ರಚನೆ strbuf buf = STRBUF_INIT;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *av[20];
	ಪೂರ್ಣಾಂಕ ac = 0;

	repo_init_revisions(the_repository, &revs, ಖಾಲಿ);
	strbuf_addf(&buf, "--author=%s", name);
	av[++ac] = "--all";
	av[++ac] = "-i";
	av[++ac] = buf.buf;
	av[++ac] = ಖಾಲಿ;
	ಸಿದ್ಧತೆ_revisions(ac, av, &revs, ಖಾಲಿ);
	revs.mailmap = xmalloc(ಗಾತ್ರ(ರಚನೆ string_list));
	string_list_init_nodup(revs.mailmap);
	read_mailmap(the_repository, revs.mailmap);

	ಆದರೆ (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	commit = get_revision(&revs);
	ಆದರೆ (commit) {
		ರಚನೆ pretty_print_context ctx = {0};
		ctx.date_ಬಹುಲಕ.type = DATE_ಸಾಮಾನ್ಯ;
		strbuf_release(&buf);
		repo_format_commit_ಸಂದೇಶ(the_repository, commit,
					   "%aN <%aE>", &buf, &ctx);
		release_revisions(&revs);
		ಹಿಂತಿರುಗಿ strbuf_detach(&buf, ಖಾಲಿ);
	}
	die(_("--author '%s' is not 'Name <email>' and matches no existing author"), name);
}

ಸ್ಥಿರ ಶೂನ್ಯ handle_ignored_arg(ರಚನೆ wt_ಸ್ಥಿತಿ *s)
{
	ಆದರೆ (!ignored_arg)
		; /* default already initialized */
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(ignored_arg, "traditional"))
		s->show_ignored_ಬಹುಲಕ = SHOW_TRADITIONAL_IGNORED;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(ignored_arg, "no"))
		s->show_ignored_ಬಹುಲಕ = SHOW_NO_IGNORED;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(ignored_arg, "matching"))
		s->show_ignored_ಬಹುಲಕ = SHOW_MATCHING_IGNORED;
	ಇಲ್ಲದಿದ್ದರೆ
		die(_("Invalid ignored mode '%s'"), ignored_arg);
}

ಸ್ಥಿರ ಎಣಿಕೆ untracked_ಸ್ಥಿತಿ_type parse_untracked_setting_name(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *u)
{
	/*
	 * Please update $__git_untracked_file_modes in
	 * git-completion.bash when you add new options
	 */
	ಆಯ್ಕೆ (git_parse_maybe_bool(u)) {
	ಸ್ಥಿತಿ 0:
		u = "no";
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ 1:
		u = "normal";
		ನಿಲ್ಲಿಸು;
	ಪೂರ್ವನಿಯೋಜಿತ:
		ನಿಲ್ಲಿಸು;
	}

	ಆದರೆ (!strcmp(u, "no"))
		ಹಿಂತಿರುಗಿ SHOW_NO_UNTRACKED_FILES;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(u, "normal"))
		ಹಿಂತಿರುಗಿ SHOW_ಸಾಮಾನ್ಯ_UNTRACKED_FILES;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(u, "all"))
		ಹಿಂತಿರುಗಿ SHOW_ALL_UNTRACKED_FILES;
	ಇಲ್ಲದಿದ್ದರೆ
		ಹಿಂತಿರುಗಿ SHOW_UNTRACKED_FILES_ERROR;
}

ಸ್ಥಿರ ಶೂನ್ಯ handle_untracked_files_arg(ರಚನೆ wt_ಸ್ಥಿತಿ *s)
{
	ಎಣಿಕೆ untracked_ಸ್ಥಿತಿ_type u;

	ಆದರೆ (!untracked_files_arg)
		ಹಿಂತಿರುಗಿ; /* default already initialized */

	u = parse_untracked_setting_name(untracked_files_arg);
	ಆದರೆ (u == SHOW_UNTRACKED_FILES_ERROR)
		die(_("Invalid untracked files mode '%s'"),
		    untracked_files_arg);
	s->show_untracked_files = u;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *read_commit_ಸಂದೇಶ_2(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಹೊರಹೋಗು_enc;
	ರಚನೆ commit *commit;

	commit = lookup_commit_reference_by_name(name);
	ಆದರೆ (!commit)
		die(_("could not lookup commit '%s'"), name);
	ಹೊರಹೋಗು_enc = get_commit_output_encoding();
	ಹಿಂತಿರುಗಿ repo_logmsg_reencode(the_repository, commit, ಖಾಲಿ, ಹೊರಹೋಗು_enc);
}

/*
 * Enumerate what needs to be propagated when --porcelain
 * is not in effect here.
 */
ಸ್ಥಿರ ರಚನೆ ಸ್ಥಿತಿ_deferred_config {
	ಎಣಿಕೆ wt_ಸ್ಥಿತಿ_format ಸ್ಥಿತಿ_format;
	ಪೂರ್ಣಾಂಕ show_branch;
	ಎಣಿಕೆ ahead_behind_flags ahead_behind;
} ಸ್ಥಿತಿ_deferred_config = {
	ಸ್ಥಿತಿ_FORMAT_UNSPECIFIED,
	-1, /* unspecified */
	AHEAD_BEHIND_UNSPECIFIED,
};

ಸ್ಥಿರ ಶೂನ್ಯ finalize_deferred_config(ರಚನೆ wt_ಸ್ಥಿತಿ *s)
{
	ಪೂರ್ಣಾಂಕ use_deferred_config = (ಸ್ಥಿತಿ_format != ಸ್ಥಿತಿ_FORMAT_PORCELAIN &&
				   ಸ್ಥಿತಿ_format != ಸ್ಥಿತಿ_FORMAT_PORCELAIN_V2 &&
				   !s->null_termination);

	ಆದರೆ (s->null_termination) {
		ಆದರೆ (ಸ್ಥಿತಿ_format == ಸ್ಥಿತಿ_FORMAT_NONE ||
		    ಸ್ಥಿತಿ_format == ಸ್ಥಿತಿ_FORMAT_UNSPECIFIED)
			ಸ್ಥಿತಿ_format = ಸ್ಥಿತಿ_FORMAT_PORCELAIN;
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಸ್ಥಿತಿ_format == ಸ್ಥಿತಿ_FORMAT_LONG)
			die(_("options '%s' and '%s' cannot be used together"), "--long", "-z");
	}

	ಆದರೆ (use_deferred_config && ಸ್ಥಿತಿ_format == ಸ್ಥಿತಿ_FORMAT_UNSPECIFIED)
		ಸ್ಥಿತಿ_format = ಸ್ಥಿತಿ_deferred_config.ಸ್ಥಿತಿ_format;
	ಆದರೆ (ಸ್ಥಿತಿ_format == ಸ್ಥಿತಿ_FORMAT_UNSPECIFIED)
		ಸ್ಥಿತಿ_format = ಸ್ಥಿತಿ_FORMAT_NONE;

	ಆದರೆ (use_deferred_config && s->show_branch < 0)
		s->show_branch = ಸ್ಥಿತಿ_deferred_config.show_branch;
	ಆದರೆ (s->show_branch < 0)
		s->show_branch = 0;

	/*
	 * If the user did not give a "--[no]-ahead-behind" command
	 * line argument *AND* we will print in a human-readable format
	 * (short, long etc.) then we inherit from the status.aheadbehind
	 * config setting.  In all other cases (and porcelain V[12] formats
	 * in particular), we inherit _FULL for backwards compatibility.
	 */
	ಆದರೆ (use_deferred_config &&
	    s->ahead_behind_flags == AHEAD_BEHIND_UNSPECIFIED)
		s->ahead_behind_flags = ಸ್ಥಿತಿ_deferred_config.ahead_behind;

	ಆದರೆ (s->ahead_behind_flags == AHEAD_BEHIND_UNSPECIFIED)
		s->ahead_behind_flags = AHEAD_BEHIND_ತುಂಬಿದ;
}

ಸ್ಥಿರ ಶೂನ್ಯ check_fixup_reword_options(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *argv[]) {
	ಆದರೆ (whence != FROM_COMMIT) {
		ಆದರೆ (whence == FROM_ವಿಲೀನ)
			die(_("You are in the middle of a merge -- cannot reword."));
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (is_from_cherry_pick(whence))
			die(_("You are in the middle of a cherry-pick -- cannot reword."));
	}
	ಆದರೆ (argc)
		die(_("reword option of '%s' and path '%s' cannot be used together"), "--fixup", *argv);
	ಆದರೆ (patch_interactive || interactive || all || also || only)
		die(_("reword option of '%s' and '%s' cannot be used together"),
			"--fixup", "--patch/--interactive/--all/--include/--only");
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ parse_and_validate_options(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *argv[],
				      ಸ್ಥಿರಾಂಕ ರಚನೆ option *options,
				      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ usage[],
				      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
				      ರಚನೆ commit *current_ತಲೆ,
				      ರಚನೆ wt_ಸ್ಥಿತಿ *s)
{
	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options, usage, 0);
	finalize_deferred_config(s);

	ಆದರೆ (force_author && !strchr(force_author, '>'))
		force_author = ಕಂಡುಹಿಡಿ_author_by_nickname(force_author);

	ಆದರೆ (force_author && renew_authorship)
		die(_("options '%s' and '%s' cannot be used together"), "--reset-author", "--author");

	ಆದರೆ (logfile || have_option_m || use_ಸಂದೇಶ)
		use_editor = 0;

	/* Sanity check options */
	ಆದರೆ (amend && !current_ತಲೆ)
		die(_("You have nothing to amend."));
	ಆದರೆ (amend && whence != FROM_COMMIT) {
		ಆದರೆ (whence == FROM_ವಿಲೀನ)
			die(_("You are in the middle of a merge -- cannot amend."));
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (is_from_cherry_pick(whence))
			die(_("You are in the middle of a cherry-pick -- cannot amend."));
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (whence == FROM_REBASE_PICK)
			die(_("You are in the middle of a rebase -- cannot amend."));
	}
	ಆದರೆ (fixup_ಸಂದೇಶ && squash_ಸಂದೇಶ)
		die(_("options '%s' and '%s' cannot be used together"), "--squash", "--fixup");
	die_for_incompatible_opt4(!!use_ಸಂದೇಶ, "-C",
				  !!edit_ಸಂದೇಶ, "-c",
				  !!logfile, "-F",
				  !!fixup_ಸಂದೇಶ, "--fixup");
	die_for_incompatible_opt4(have_option_m, "-m",
				  !!edit_ಸಂದೇಶ, "-c",
				  !!use_ಸಂದೇಶ, "-C",
				  !!logfile, "-F");
	ಆದರೆ (use_ಸಂದೇಶ || edit_ಸಂದೇಶ || logfile ||fixup_ಸಂದೇಶ || have_option_m)
		FREE_AND_NULL(template_file);
	ಆದರೆ (edit_ಸಂದೇಶ)
		use_ಸಂದೇಶ = edit_ಸಂದೇಶ;
	ಆದರೆ (amend && !use_ಸಂದೇಶ && !fixup_ಸಂದೇಶ)
		use_ಸಂದೇಶ = "HEAD";
	ಆದರೆ (!use_ಸಂದೇಶ && !is_from_cherry_pick(whence) &&
	    !is_from_rebase(whence) && renew_authorship)
		die(_("--reset-author can be used only with -C, -c or --amend."));
	ಆದರೆ (use_ಸಂದೇಶ) {
		use_ಸಂದೇಶ_ಬಫರ್ = read_commit_ಸಂದೇಶ_2(use_ಸಂದೇಶ);
		ಆದರೆ (!renew_authorship) {
			author_ಸಂದೇಶ = use_ಸಂದೇಶ;
			author_ಸಂದೇಶ_ಬಫರ್ = use_ಸಂದೇಶ_ಬಫರ್;
		}
	}
	ಆದರೆ ((is_from_cherry_pick(whence) || whence == FROM_REBASE_PICK) &&
	    !renew_authorship) {
		author_ಸಂದೇಶ = "CHERRY_PICK_HEAD";
		author_ಸಂದೇಶ_ಬಫರ್ = read_commit_ಸಂದೇಶ_2(author_ಸಂದೇಶ);
	}

	ಆದರೆ (patch_interactive)
		interactive = 1;

	die_for_incompatible_opt4(also, "-i/--include",
				  only, "-o/--only",
				  all, "-a/--all",
				  interactive, "--interactive/-p/--patch");
	ಆದರೆ (fixup_ಸಂದೇಶ) {
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
		ಅಕ್ಷರ *p = fixup_ಸಂದೇಶ;
		ಆಗ (ಅಕ್ಷರವೇ(*p))
			p++;
		ಆದರೆ (p > fixup_ಸಂದೇಶ && *p == ':') {
			*p = '\0';
			fixup_commit = p + 1;
			ಆದರೆ (!strcmp("amend", fixup_ಸಂದೇಶ) ||
			    !strcmp("reword", fixup_ಸಂದೇಶ)) {
				fixup_ಪೂರ್ವಪ್ರತ್ಯಯ = "amend";
				allow_ಖಾಲಿಗಳು = 1;
				ಆದರೆ (*fixup_ಸಂದೇಶ == 'r') {
					check_fixup_reword_options(argc, argv);
					only = 1;
				}
			} ಇಲ್ಲದಿದ್ದರೆ {
				die(_("unknown option: --fixup=%s:%s"), fixup_ಸಂದೇಶ, fixup_commit);
			}
		} ಇಲ್ಲದಿದ್ದರೆ {
			fixup_commit = fixup_ಸಂದೇಶ;
			fixup_ಪೂರ್ವಪ್ರತ್ಯಯ = "fixup";
			use_editor = 0;
		}
	}

	ಆದರೆ (0 <= edit_flag)
		use_editor = edit_flag;

	handle_untracked_files_arg(s);

	ಆದರೆ (all && argc > 0)
		die(_("paths '%s ...' with -a does not make sense"),
		    argv[0]);

	ಆದರೆ (ಸ್ಥಿತಿ_format != ಸ್ಥಿತಿ_FORMAT_NONE)
		dry_ಸತತ = 1;

	ಹಿಂತಿರುಗಿ argc;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ dry_ಸತತ_commit(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			  ಸ್ಥಿರಾಂಕ ರಚನೆ commit *current_ತಲೆ, ರಚನೆ wt_ಸ್ಥಿತಿ *s)
{
	ಪೂರ್ಣಾಂಕ committable;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸೂಚ್ಯಂಕ_file;

	ಸೂಚ್ಯಂಕ_file = prepare_ಸೂಚ್ಯಂಕ(argv, ಪೂರ್ವಪ್ರತ್ಯಯ, current_ತಲೆ, 1);
	committable = ಸತತ_ಸ್ಥಿತಿ(stdout, ಸೂಚ್ಯಂಕ_file, ಪೂರ್ವಪ್ರತ್ಯಯ, 0, s);
	rollback_ಸೂಚ್ಯಂಕ_files();

	ಹಿಂತಿರುಗಿ committable ? 0 : 1;
}

define_list_config_array_extra(color_ಸ್ಥಿತಿ_slots, {"added"});

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ parse_ಸ್ಥಿತಿ_ಸ್ಥಾನ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಾನ)
{
	ಆದರೆ (!strcasecmp(ಸ್ಥಾನ, "added"))
		ಹಿಂತಿರುಗಿ WT_ಸ್ಥಿತಿ_UPDATED;

	ಹಿಂತಿರುಗಿ LOOKUP_CONFIG(color_ಸ್ಥಿತಿ_slots, ಸ್ಥಾನ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ git_ಸ್ಥಿತಿ_config(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *k, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *v,
			     ಸ್ಥಿರಾಂಕ ರಚನೆ config_context *ctx, ಶೂನ್ಯ *cb)
{
	ರಚನೆ wt_ಸ್ಥಿತಿ *s = cb;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಾನ_name;

	ಆದರೆ (starts_with(k, "column."))
		ಹಿಂತಿರುಗಿ git_ಸ್ತಂಭ_config(k, v, "status", &s->colopts);
	ಆದರೆ (!strcmp(k, "status.submodulesummary")) {
		ಪೂರ್ಣಾಂಕ is_bool;
		s->submodule_summary = git_config_bool_or_int(k, v, ctx->kvi,
							      &is_bool);
		ಆದರೆ (is_bool && s->submodule_summary)
			s->submodule_summary = -1;
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(k, "status.short")) {
		ಆದರೆ (git_config_bool(k, v))
			ಸ್ಥಿತಿ_deferred_config.ಸ್ಥಿತಿ_format = ಸ್ಥಿತಿ_FORMAT_SHORT;
		ಇಲ್ಲದಿದ್ದರೆ
			ಸ್ಥಿತಿ_deferred_config.ಸ್ಥಿತಿ_format = ಸ್ಥಿತಿ_FORMAT_NONE;
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(k, "status.branch")) {
		ಸ್ಥಿತಿ_deferred_config.show_branch = git_config_bool(k, v);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(k, "status.aheadbehind")) {
		ಸ್ಥಿತಿ_deferred_config.ahead_behind = git_config_bool(k, v);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(k, "status.showstash")) {
		s->show_stash = git_config_bool(k, v);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(k, "status.color") || !strcmp(k, "color.status")) {
		s->use_color = git_config_colorbool(k, v);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(k, "status.displaycommentprefix")) {
		s->display_comment_ಪೂರ್ವಪ್ರತ್ಯಯ = git_config_bool(k, v);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(k, "status.color.", &ಸ್ಥಾನ_name) ||
	    skip_ಪೂರ್ವಪ್ರತ್ಯಯ(k, "color.status.", &ಸ್ಥಾನ_name)) {
		ಪೂರ್ಣಾಂಕ ಸ್ಥಾನ = parse_ಸ್ಥಿತಿ_ಸ್ಥಾನ(ಸ್ಥಾನ_name);
		ಆದರೆ (ಸ್ಥಾನ < 0)
			ಹಿಂತಿರುಗಿ 0;
		ಆದರೆ (!v)
			ಹಿಂತಿರುಗಿ config_error_nonbool(k);
		ಹಿಂತಿರುಗಿ color_parse(v, s->color_palette[ಸ್ಥಾನ]);
	}
	ಆದರೆ (!strcmp(k, "status.relativepaths")) {
		s->relative_paths = git_config_bool(k, v);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(k, "status.showuntrackedfiles")) {
		ಎಣಿಕೆ untracked_ಸ್ಥಿತಿ_type u;

		u = parse_untracked_setting_name(v);
		ಆದರೆ (u == SHOW_UNTRACKED_FILES_ERROR)
			ಹಿಂತಿರುಗಿ error(_("Invalid untracked files mode '%s'"), v);
		s->show_untracked_files = u;
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(k, "diff.renamelimit")) {
		ಆದರೆ (s->rename_ಎಲ್ಲೆ == -1)
			s->rename_ಎಲ್ಲೆ = git_config_int(k, v, ctx->kvi);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(k, "status.renamelimit")) {
		s->rename_ಎಲ್ಲೆ = git_config_int(k, v, ctx->kvi);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(k, "diff.renames")) {
		ಆದರೆ (s->detect_rename == -1)
			s->detect_rename = git_config_rename(k, v);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(k, "status.renames")) {
		s->detect_rename = git_config_rename(k, v);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಹಿಂತಿರುಗಿ git_diff_ui_config(k, v, ctx, ಖಾಲಿ);
}

ಪೂರ್ಣಾಂಕ cmd_ಸ್ಥಿತಿ(ಪೂರ್ಣಾಂಕ argc,
ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
ರಚನೆ repository *repo ಬಳಸದ)
{
	ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ no_renames = -1;
	ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rename_score_arg = (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *)-1;
	ಸ್ಥಿರ ರಚನೆ wt_ಸ್ಥಿತಿ s;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಪ್ರಗತಿ_flag = 0;
	ಪೂರ್ಣಾಂಕ fd;
	ರಚನೆ object_id oid;
	ಸ್ಥಿರ ರಚನೆ option builtin_ಸ್ಥಿತಿ_options[] = {
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT_ಗಣ_INT('s', "short", &ಸ್ಥಿತಿ_format,
			    N_("show status concisely"), ಸ್ಥಿತಿ_FORMAT_SHORT),
		OPT_BOOL('b', "branch", &s.show_branch,
			 N_("show branch information")),
		OPT_BOOL(0, "show-stash", &s.show_stash,
			 N_("show stash information")),
		OPT_BOOL(0, "ahead-behind", &s.ahead_behind_flags,
			 N_("compute full ahead/behind values")),
		OPT_CALLBACK_F(0, "porcelain", &ಸ್ಥಿತಿ_format,
		  N_("version"), N_("machine-readable output"),
		  PARSE_OPT_OPTARG, opt_parse_porcelain),
		OPT_ಗಣ_INT(0, "long", &ಸ್ಥಿತಿ_format,
			    N_("show status in long format (default)"),
			    ಸ್ಥಿತಿ_FORMAT_LONG),
		OPT_BOOL('z', "null", &s.null_termination,
			 N_("terminate entries with NUL")),
		{
			.type = OPTION_STRING,
			.short_name = 'u',
			.long_name = "untracked-files",
			.ಮೌಲ್ಯ = &untracked_files_arg,
			.argh = N_("mode"),
			.help = N_("show untracked files, optional modes: all, normal, no. (Default: all)"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t)"all",
		},
		{
			.type = OPTION_STRING,
			.long_name = "ignored",
			.ಮೌಲ್ಯ = &ignored_arg,
			.argh = N_("mode"),
			.help = N_("show ignored files, optional modes: traditional, matching, no. (Default: traditional)"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t)"traditional",
		},
		{
			.type = OPTION_STRING,
			.long_name = "ignore-submodules",
			.ಮೌಲ್ಯ = &ignore_submodule_arg,
			.argh = N_("when"),
			.help = N_("ignore changes to submodules, optional when: all, dirty, untracked. (Default: all)"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t)"all",
		},
		OPT_ಸ್ತಂಭ(0, "column", &s.colopts, N_("list untracked files in columns")),
		OPT_BOOL(0, "no-renames", &no_renames, N_("do not detect renames")),
		OPT_CALLBACK_F('M', "find-renames", &rename_score_arg,
		  N_("n"), N_("detect renames, optionally set similarity index"),
		  PARSE_OPT_OPTARG | PARSE_OPT_NONEG, opt_parse_rename_score),
		OPT_ಕೊನೆ(),
	};

	show_usage_with_options_if_asked(argc, argv,
					 builtin_ಸ್ಥಿತಿ_usage, builtin_ಸ್ಥಿತಿ_options);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_ತುಂಬಿದ_ಸೂಚ್ಯಂಕ = 0;

	ಸ್ಥಿತಿ_init_config(&s, git_ಸ್ಥಿತಿ_config);
	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ,
			     builtin_ಸ್ಥಿತಿ_options,
			     builtin_ಸ್ಥಿತಿ_usage, 0);
	finalize_colopts(&s.colopts, -1);
	finalize_deferred_config(&s);

	handle_untracked_files_arg(&s);
	handle_ignored_arg(&s);

	ಆದರೆ (s.show_ignored_ಬಹುಲಕ == SHOW_MATCHING_IGNORED &&
	    s.show_untracked_files == SHOW_NO_UNTRACKED_FILES)
		die(_("Unsupported combination of ignored and untracked-files arguments"));

	parse_pathspec(&s.pathspec, 0,
		       PATHSPEC_PREFER_ತುಂಬಿದ,
		       ಪೂರ್ವಪ್ರತ್ಯಯ, argv);

	ಆದರೆ (ಸ್ಥಿತಿ_format != ಸ್ಥಿತಿ_FORMAT_PORCELAIN &&
	    ಸ್ಥಿತಿ_format != ಸ್ಥಿತಿ_FORMAT_PORCELAIN_V2)
		ಪ್ರಗತಿ_flag = REFRESH_ಪ್ರಗತಿ;
	repo_read_ಸೂಚ್ಯಂಕ(the_repository);
	refresh_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ,
		      REFRESH_QUIET|REFRESH_UNMERGED|ಪ್ರಗತಿ_flag,
		      &s.pathspec, ಖಾಲಿ, ಖಾಲಿ);

	ಆದರೆ (use_optional_locks())
		fd = repo_hold_locked_ಸೂಚ್ಯಂಕ(the_repository, &ಸೂಚ್ಯಂಕ_ಬೀಗ, 0);
	ಇಲ್ಲದಿದ್ದರೆ
		fd = -1;

	s.is_initial = repo_get_oid(the_repository, s.reference, &oid) ? 1 : 0;
	ಆದರೆ (!s.is_initial)
		oidcpy(&s.oid_commit, &oid);

	s.ignore_submodule_arg = ignore_submodule_arg;
	s.ಸ್ಥಿತಿ_format = ಸ್ಥಿತಿ_format;
	s.verbose = verbose;
	ಆದರೆ (no_renames != -1)
		s.detect_rename = !no_renames;
	ಆದರೆ ((intptr_t)rename_score_arg != -1) {
		ಆದರೆ (s.detect_rename < DIFF_DETECT_RENAME)
			s.detect_rename = DIFF_DETECT_RENAME;
		ಆದರೆ (rename_score_arg)
			s.rename_score = parse_rename_score(&rename_score_arg);
	}

	wt_ಸ್ಥಿತಿ_collect(&s);

	ಆದರೆ (0 <= fd)
		repo_update_ಸೂಚ್ಯಂಕ_if_able(the_repository, &ಸೂಚ್ಯಂಕ_ಬೀಗ);

	ಆದರೆ (s.relative_paths)
		s.ಪೂರ್ವಪ್ರತ್ಯಯ = ಪೂರ್ವಪ್ರತ್ಯಯ;

	wt_ಸ್ಥಿತಿ_print(&s);
	wt_ಸ್ಥಿತಿ_collect_free_buffers(&s);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ git_commit_config(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *k, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *v,
			     ಸ್ಥಿರಾಂಕ ರಚನೆ config_context *ctx, ಶೂನ್ಯ *cb)
{
	ರಚನೆ wt_ಸ್ಥಿತಿ *s = cb;

	ಆದರೆ (!strcmp(k, "commit.template"))
		ಹಿಂತಿರುಗಿ git_config_pathname(&template_file, k, v);
	ಆದರೆ (!strcmp(k, "commit.status")) {
		include_ಸ್ಥಿತಿ = git_config_bool(k, v);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(k, "commit.cleanup")) {
		FREE_AND_NULL(cleanup_config);
		ಹಿಂತಿರುಗಿ git_config_string(&cleanup_config, k, v);
	}
	ಆದರೆ (!strcmp(k, "commit.gpgsign")) {
		sign_commit = git_config_bool(k, v) ? "" : ಖಾಲಿ;
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(k, "commit.verbose")) {
		ಪೂರ್ಣಾಂಕ is_bool;
		config_commit_verbose = git_config_bool_or_int(k, v, ctx->kvi,
							       &is_bool);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಹಿಂತಿರುಗಿ git_ಸ್ಥಿತಿ_config(k, v, ctx, s);
}

ಪೂರ್ಣಾಂಕ cmd_commit(ಪೂರ್ಣಾಂಕ argc,
	       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
	       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
	       ರಚನೆ repository *repo ಬಳಸದ)
{
	ಸ್ಥಿರ ರಚನೆ wt_ಸ್ಥಿತಿ s;
	ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cleanup_arg = ಖಾಲಿ;
	ಸ್ಥಿರ ರಚನೆ option builtin_commit_options[] = {
		OPT__QUIET(&quiet, N_("suppress summary after successful commit")),
		OPT__VERBOSE(&verbose, N_("show diff in commit message template")),

		OPT_ಗುಂಪು(N_("Commit message options")),
		OPT_FILENAME('F', "file", &logfile, N_("read message from file")),
		OPT_STRING(0, "author", &force_author, N_("author"), N_("override author for commit")),
		OPT_STRING(0, "date", &force_date, N_("date"), N_("override date for commit")),
		OPT_CALLBACK('m', "message", &ಸಂದೇಶ_2, N_("message"), N_("commit message"), opt_parse_m),
		OPT_STRING('c', "reedit-message", &edit_ಸಂದೇಶ, N_("commit"), N_("reuse and edit message from specified commit")),
		OPT_STRING('C', "reuse-message", &use_ಸಂದೇಶ, N_("commit"), N_("reuse message from specified commit")),
		/*
		 * TRANSLATORS: Leave "[(amend|reword):]" as-is,
		 * and only translate <commit>.
		 */
		OPT_STRING(0, "fixup", &fixup_ಸಂದೇಶ, N_("[(amend|reword):]commit"), N_("use autosquash formatted message to fixup or amend/reword specified commit")),
		OPT_STRING(0, "squash", &squash_ಸಂದೇಶ, N_("commit"), N_("use autosquash formatted message to squash specified commit")),
		OPT_BOOL(0, "reset-author", &renew_authorship, N_("the commit is authored by me now (used with -C/-c/--amend)")),
		OPT_STRVEC(0, "trailer", &trailer_ವಾದಗಳು, N_("trailer"),
			   N_("add custom trailer(s)")),
		OPT_BOOL('s', "signoff", &signoff, N_("add a Signed-off-by trailer")),
		OPT_FILENAME('t', "template", &template_file, N_("use specified template file")),
		OPT_BOOL('e', "edit", &edit_flag, N_("force edit of commit")),
		OPT_CLEANUP(&cleanup_arg),
		OPT_BOOL(0, "status", &include_ಸ್ಥಿತಿ, N_("include status in commit message template")),
		{
			.type = OPTION_STRING,
			.short_name = 'S',
			.long_name = "gpg-sign",
			.ಮೌಲ್ಯ = &sign_commit,
			.argh = N_("key-id"),
			.help = N_("GPG sign commit"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "",
		},
		/* end commit message options */

		OPT_ಗುಂಪು(N_("Commit contents options")),
		OPT_BOOL('a', "all", &all, N_("commit all changed files")),
		OPT_BOOL('i', "include", &also, N_("add specified files to index for commit")),
		OPT_BOOL(0, "interactive", &interactive, N_("interactively add files")),
		OPT_BOOL('p', "patch", &patch_interactive, N_("interactively add changes")),
		OPT_DIFF_UNIFIED(&interactive_opts.context),
		OPT_DIFF_INTERHUNK_CONTEXT(&interactive_opts.interhunkcontext),
		OPT_BOOL('o', "only", &only, N_("commit only specified files")),
		OPT_BOOL('n', "no-verify", &no_verify, N_("bypass pre-commit and commit-msg hooks")),
		OPT_BOOL(0, "dry-run", &dry_ಸತತ, N_("show what would be committed")),
		OPT_ಗಣ_INT(0, "short", &ಸ್ಥಿತಿ_format, N_("show status concisely"),
			    ಸ್ಥಿತಿ_FORMAT_SHORT),
		OPT_BOOL(0, "branch", &s.show_branch, N_("show branch information")),
		OPT_BOOL(0, "ahead-behind", &s.ahead_behind_flags,
			 N_("compute full ahead/behind values")),
		OPT_ಗಣ_INT(0, "porcelain", &ಸ್ಥಿತಿ_format,
			    N_("machine-readable output"), ಸ್ಥಿತಿ_FORMAT_PORCELAIN),
		OPT_ಗಣ_INT(0, "long", &ಸ್ಥಿತಿ_format,
			    N_("show status in long format (default)"),
			    ಸ್ಥಿತಿ_FORMAT_LONG),
		OPT_BOOL('z', "null", &s.null_termination,
			 N_("terminate entries with NUL")),
		OPT_BOOL(0, "amend", &amend, N_("amend previous commit")),
		OPT_BOOL(0, "no-post-rewrite", &no_post_rewrite, N_("bypass post-rewrite hook")),
		{
			.type = OPTION_STRING,
			.short_name = 'u',
			.long_name = "untracked-files",
			.ಮೌಲ್ಯ = &untracked_files_arg,
			.argh = N_("mode"),
			.help = N_("show untracked files, optional modes: all, normal, no. (Default: all)"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t)"all",
		},
		OPT_PATHSPEC_FROM_FILE(&pathspec_from_file),
		OPT_PATHSPEC_FILE_NUL(&pathspec_file_nul),
		/* end commit contents options */

		OPT_HIDDEN_BOOL(0, "allow-empty", &allow_ಖಾಲಿಗಳು,
				N_("ok to record an empty change")),
		OPT_HIDDEN_BOOL(0, "allow-empty-message", &allow_ಖಾಲಿಗಳು_ಸಂದೇಶ,
				N_("ok to record a change with an empty message")),

		OPT_ಕೊನೆ()
	};

	ರಚನೆ strbuf sb = STRBUF_INIT;
	ರಚನೆ strbuf author_ident = STRBUF_INIT;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸೂಚ್ಯಂಕ_file, *reflog_ಸಂದೇಶ;
	ರಚನೆ object_id oid;
	ರಚನೆ commit_list *parents = ಖಾಲಿ;
	ರಚನೆ stat statbuf;
	ರಚನೆ commit *current_ತಲೆ = ಖಾಲಿ;
	ರಚನೆ commit_extra_ಶೀರ್ಷಿಕೆ *extra = ಖಾಲಿ;
	ರಚನೆ strbuf err = STRBUF_INIT;
	ಪೂರ್ಣಾಂಕ ret = 0;

	show_usage_with_options_if_asked(argc, argv,
					 builtin_commit_usage, builtin_commit_options);

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ WITH_BREAKING_CHANGES
	warn_on_auto_comment_char = true;
#ಆದರೆ_ಅಂತ್ಯ /* !WITH_BREAKING_CHANGES */
	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_ತುಂಬಿದ_ಸೂಚ್ಯಂಕ = 0;

	ಸ್ಥಿತಿ_init_config(&s, git_commit_config);
	s.commit_template = 1;
	ಸ್ಥಿತಿ_format = ಸ್ಥಿತಿ_FORMAT_NONE; /* Ignore status.short */
	s.colopts = 0;

	ಆದರೆ (repo_get_oid(the_repository, "HEAD", &oid))
		current_ತಲೆ = ಖಾಲಿ;
	ಇಲ್ಲದಿದ್ದರೆ {
		current_ತಲೆ = lookup_commit_or_die(&oid, "HEAD");
		ಆದರೆ (repo_parse_commit(the_repository, current_ತಲೆ))
			die(_("could not parse HEAD commit"));
	}
	verbose = -1; /* unspecified */
	argc = parse_and_validate_options(argc, argv, builtin_commit_options,
					  builtin_commit_usage,
					  ಪೂರ್ವಪ್ರತ್ಯಯ, current_ತಲೆ, &s);
	ಆದರೆ (trailer_ವಾದಗಳು.nr)
		trailer_config_init();

	ಆದರೆ (verbose == -1)
		verbose = (config_commit_verbose < 0) ? 0 : config_commit_verbose;

	ಆದರೆ (cleanup_arg) {
		ಬಿಡುಗಡೆ(cleanup_config);
		cleanup_config = xstrdup(cleanup_arg);
	}
	cleanup_ಬಹುಲಕ = get_cleanup_ಬಹುಲಕ(cleanup_config, use_editor);

	ಆದರೆ (dry_ಸತತ)
		ಹಿಂತಿರುಗಿ dry_ಸತತ_commit(argv, ಪೂರ್ವಪ್ರತ್ಯಯ, current_ತಲೆ, &s);
	ಸೂಚ್ಯಂಕ_file = prepare_ಸೂಚ್ಯಂಕ(argv, ಪೂರ್ವಪ್ರತ್ಯಯ, current_ತಲೆ, 0);

	/* Set up everything for writing the commit object.  This includes
	   running hooks, writing the trees, and interacting with the user.  */
	ಆದರೆ (!prepare_ಗಮ್ಯ_commit(ಸೂಚ್ಯಂಕ_file, ಪೂರ್ವಪ್ರತ್ಯಯ,
			       current_ತಲೆ, &s, &author_ident)) {
		ret = 1;
		rollback_ಸೂಚ್ಯಂಕ_files();
		ಹೋಗು cleanup;
	}

	/* Determine parents */
	reflog_ಸಂದೇಶ = getenv("GIT_REFLOG_ACTION");
	ಆದರೆ (!current_ತಲೆ) {
		ಆದರೆ (!reflog_ಸಂದೇಶ)
			reflog_ಸಂದೇಶ = "commit (initial)";
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (amend) {
		ಆದರೆ (!reflog_ಸಂದೇಶ)
			reflog_ಸಂದೇಶ = "commit (amend)";
		parents = commit_list_copy(current_ತಲೆ->parents);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (whence == FROM_ವಿಲೀನ) {
		ರಚನೆ strbuf m = STRBUF_INIT;
		FILE *fp;
		ಪೂರ್ಣಾಂಕ allow_fast_forward = 1;
		ರಚನೆ commit_list **pptr = &parents;

		ಆದರೆ (!reflog_ಸಂದೇಶ)
			reflog_ಸಂದೇಶ = "commit (merge)";
		pptr = commit_list_append(current_ತಲೆ, pptr);
		fp = xfopen(git_ಮಾರ್ಗ_ವಿಲೀನ_ತಲೆ(the_repository), "r");
		ಆಗ (strbuf_getline_lf(&m, fp) != EOF) {
			ರಚನೆ commit *ಪೋಷಕ;

			ಪೋಷಕ = get_ವಿಲೀನ_ಪೋಷಕ(m.buf);
			ಆದರೆ (!ಪೋಷಕ)
				die(_("Corrupt MERGE_HEAD file (%s)"), m.buf);
			pptr = commit_list_append(ಪೋಷಕ, pptr);
		}
		fclose(fp);
		strbuf_release(&m);
		ಆದರೆ (!stat(git_ಮಾರ್ಗ_ವಿಲೀನ_ಬಹುಲಕ(the_repository), &statbuf)) {
			ಆದರೆ (strbuf_read_file(&sb, git_ಮಾರ್ಗ_ವಿಲೀನ_ಬಹುಲಕ(the_repository), 0) < 0)
				die_errno(_("could not read MERGE_MODE"));
			ಆದರೆ (!strcmp(sb.buf, "no-ff"))
				allow_fast_forward = 0;
		}
		ಆದರೆ (allow_fast_forward)
			reduce_heads_replace(&parents);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಆದರೆ (!reflog_ಸಂದೇಶ)
			reflog_ಸಂದೇಶ = is_from_cherry_pick(whence)
					? "commit (cherry-pick)"
					: is_from_rebase(whence)
					? "commit (rebase)"
					: "commit";
		commit_list_ಸೇರಿಸು(current_ತಲೆ, &parents);
	}

	/* Finally, get the commit message */
	strbuf_reset(&sb);
	ಆದರೆ (strbuf_read_file(&sb, git_ಮಾರ್ಗ_commit_editmsg(), 0) < 0) {
		ಪೂರ್ಣಾಂಕ saved_errno = errno;
		rollback_ಸೂಚ್ಯಂಕ_files();
		die(_("could not read commit message: %s"), strerror(saved_errno));
	}

	cleanup_ಸಂದೇಶ(&sb, cleanup_ಬಹುಲಕ, verbose);

	ಆದರೆ (ಸಂದೇಶ_is_ಖಾಲಿಗಳು(&sb, cleanup_ಬಹುಲಕ) && !allow_ಖಾಲಿಗಳು_ಸಂದೇಶ) {
		rollback_ಸೂಚ್ಯಂಕ_files();
		fprintf(stderr, _("Aborting commit due to empty commit message.\n"));
		ನಿರ್ಗಮಿಸು(1);
	}
	ಆದರೆ (template_untouched(&sb, template_file, cleanup_ಬಹುಲಕ) && !allow_ಖಾಲಿಗಳು_ಸಂದೇಶ) {
		rollback_ಸೂಚ್ಯಂಕ_files();
		fprintf(stderr, _("Aborting commit; you did not edit the message.\n"));
		ನಿರ್ಗಮಿಸು(1);
	}

	ಆದರೆ (fixup_ಸಂದೇಶ && starts_with(sb.buf, "amend! ") &&
	    !allow_ಖಾಲಿಗಳು_ಸಂದೇಶ) {
		ರಚನೆ strbuf body = STRBUF_INIT;
		ಗಾತ್ರ_t len = commit_subject_ಉದ್ದಳತೆ(sb.buf);
		strbuf_addstr(&body, sb.buf + len);
		ಆದರೆ (ಸಂದೇಶ_is_ಖಾಲಿಗಳು(&body, cleanup_ಬಹುಲಕ)) {
			rollback_ಸೂಚ್ಯಂಕ_files();
			fprintf(stderr, _("Aborting commit due to empty commit message body.\n"));
			ನಿರ್ಗಮಿಸು(1);
		}
		strbuf_release(&body);
	}

	ಆದರೆ (amend) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *exclude_gpgsig[3] = { "gpgsig", "gpgsig-sha256", ಖಾಲಿ };
		extra = read_commit_extra_ಶೀರ್ಷಿಕೆಗಳು(current_ತಲೆ, exclude_gpgsig);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ರಚನೆ commit_extra_ಶೀರ್ಷಿಕೆ **ಬಾಲ = &extra;
		append_ವಿಲೀನ_ಟ್ಯಾಗ್_ಶೀರ್ಷಿಕೆಗಳು(parents, &ಬಾಲ);
	}

	ಆದರೆ (commit_ವೃಕ್ಷ_extended(sb.buf, sb.len, &the_repository->ಸೂಚ್ಯಂಕ->cache_ವೃಕ್ಷ->oid,
				 parents, &oid, author_ident.buf, ಖಾಲಿ,
				 sign_commit, extra)) {
		rollback_ಸೂಚ್ಯಂಕ_files();
		die(_("failed to write commit object"));
	}

	ಆದರೆ (update_ತಲೆ_with_reflog(current_ತಲೆ, &oid, reflog_ಸಂದೇಶ, &sb,
				    &err)) {
		rollback_ಸೂಚ್ಯಂಕ_files();
		die("%s", err.buf);
	}

	sequencer_post_commit_cleanup(the_repository, 0);
	unlink(git_ಮಾರ್ಗ_ವಿಲೀನ_ತಲೆ(the_repository));
	unlink(git_ಮಾರ್ಗ_ವಿಲೀನ_ಸಂದೇಶ(the_repository));
	unlink(git_ಮಾರ್ಗ_ವಿಲೀನ_ಬಹುಲಕ(the_repository));
	unlink(git_ಮಾರ್ಗ_squash_ಸಂದೇಶ(the_repository));

	ಆದರೆ (commit_ಸೂಚ್ಯಂಕ_files())
		die(_("repository has been updated, but unable to write\n"
		      "new index file. Check that disk is not full and quota is\n"
		      "not exceeded, and then \"git restore --staged :/\" to recover."));

	git_test_write_commit_ನಕ್ಷೆ_or_die(the_repository);

	repo_rerere(the_repository, 0);
	ಸತತ_auto_maintenance(the_repository, quiet);
	ಸತತ_commit_hook(use_editor, repo_get_ಸೂಚ್ಯಂಕ_file(the_repository),
			ಖಾಲಿ, "post-commit", ಖಾಲಿ);
	ಆದರೆ (amend && !no_post_rewrite) {
		commit_post_rewrite(the_repository, current_ತಲೆ, &oid);
	}
	ಆದರೆ (!quiet) {
		ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags = 0;

		ಆದರೆ (!current_ತಲೆ)
			flags |= SUMMARY_INITIAL_COMMIT;
		ಆದರೆ (author_date_is_interesting())
			flags |= SUMMARY_SHOW_AUTHOR_DATE;
		print_commit_summary(the_repository, ಪೂರ್ವಪ್ರತ್ಯಯ,
				     &oid, flags);
	}

	apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
			    ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ);

cleanup:
	free_commit_extra_ಶೀರ್ಷಿಕೆಗಳು(extra);
	commit_list_free(parents);
	strbuf_release(&author_ident);
	strbuf_release(&err);
	strbuf_release(&sb);
	ಬಿಡುಗಡೆ(logfile);
	ಬಿಡುಗಡೆ(template_file);
	ಹಿಂತಿರುಗಿ ret;
}
