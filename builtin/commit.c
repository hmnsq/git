/*
 * Builtin "git commit"
 *
 * Copyright (c) 2007 Kristian Høgsberg <krh@redhat.com>
 * Based on git-commit.sh by Junio C Hamano and Linus Torvalds
 */

#تعریف_کن USE_THE_REPOSITORY_VARIABLE
#تعریف_کن DISABLE_SIGN_COMPARE_WARNINGS

#ضمن "builtin.h"
#ضمن "advice.h"
#ضمن "config.h"
#ضمن "lockfile.h"
#ضمن "cache-tree.h"
#ضمن "color.h"
#ضمن "dir.h"
#ضمن "editor.h"
#ضمن "environment.h"
#ضمن "diff.h"
#ضمن "commit.h"
#ضمن "add-interactive.h"
#ضمن "gettext.h"
#ضمن "revision.h"
#ضمن "wt-status.h"
#ضمن "run-command.h"
#ضمن "strbuf.h"
#ضمن "object-name.h"
#ضمن "parse-options.h"
#ضمن "path.h"
#ضمن "preload-index.h"
#ضمن "read-cache.h"
#ضمن "repository.h"
#ضمن "string-list.h"
#ضمن "rerere.h"
#ضمن "unpack-trees.h"
#ضمن "column.h"
#ضمن "sequencer.h"
#ضمن "sparse-index.h"
#ضمن "mailmap.h"
#ضمن "help.h"
#ضمن "commit-reach.h"
#ضمن "commit-graph.h"
#ضمن "pretty.h"
#ضمن "trailer.h"

ساكن ثابت حرف * ثابت builtin_commit_usage[] = {
	N_("git commit [-a | --interactive | --patch] [-s] [-v] [-u[<mode>]] [--amend]\n"
	   "           [--dry-run] [(-c | -C | --squash) <commit> | --fixup [(amend|reword):]<commit>]\n"
	   "           [-F <file> | -m <msg>] [--reset-author] [--allow-empty]\n"
	   "           [--allow-empty-message] [--no-verify] [-e] [--author=<author>]\n"
	   "           [--date=<date>] [--cleanup=<mode>] [--[no-]status]\n"
	   "           [-i | -o] [--pathspec-from-file=<file> [--pathspec-file-nul]]\n"
	   "           [(--trailer <token>[(=|:)<value>])...] [-S[<keyid>]]\n"
	   "           [--] [<pathspec>...]"),
	عدم
};

ساكن ثابت حرف * ثابت builtin_وضعیت_usage[] = {
	N_("git status [<options>] [--] [<pathspec>...]"),
	عدم
};

ساكن ثابت حرف خالی_amend_advice[] =
N_("You asked to amend the most recent commit, but doing so would make\n"
"it empty. You can repeat your command with --allow-empty, or you can\n"
"remove the commit entirely with \"git reset HEAD^\".\n");

ساكن ثابت حرف خالی_cherry_pick_advice[] =
N_("The previous cherry-pick is now empty, possibly due to conflict resolution.\n"
"If you wish to commit it anyway, use:\n"
"\n"
"    git commit --allow-empty\n"
"\n");

ساكن ثابت حرف خالی_rebase_pick_advice[] =
N_("Otherwise, please use 'git rebase --skip'\n");

ساكن ثابت حرف خالی_cherry_pick_advice_single[] =
N_("Otherwise, please use 'git cherry-pick --skip'\n");

ساكن ثابت حرف خالی_cherry_pick_advice_multi[] =
N_("and then use:\n"
"\n"
"    git cherry-pick --continue\n"
"\n"
"to resume cherry-picking the remaining commits.\n"
"If you wish to skip this commit, use:\n"
"\n"
"    git cherry-pick --skip\n"
"\n");

ساكن ثابت حرف *color_وضعیت_slots[] = {
	[WT_وضعیت_سرآیند]	  = "header",
	[WT_وضعیت_UPDATED]	  = "updated",
	[WT_وضعیت_تغییریافته]	  = "changed",
	[WT_وضعیت_UNTRACKED]	  = "untracked",
	[WT_وضعیت_NOBRANCH]	  = "noBranch",
	[WT_وضعیت_UNMERGED]	  = "unmerged",
	[WT_وضعیت_LOCAL_BRANCH]  = "localBranch",
	[WT_وضعیت_REMOTE_BRANCH] = "remoteBranch",
	[WT_وضعیت_ONBRANCH]	  = "branch",
};

ساكن ثابت حرف *use_پیام_میانگیر;
ساكن بنية قفل_file شاخص_قفل; /* real index */
ساكن بنية قفل_file false_قفل; /* used only for partial commits */
ساكن تعداد {
	COMMIT_AS_IS = 1,
	COMMIT_نرمال,
	COMMIT_PARTIAL
} commit_style;

ساكن ثابت حرف *force_author;
ساكن حرف *logfile;
ساكن حرف *template_file;
/*
 * The _message variables are commit names from which to take
 * the commit message and/or authorship.
 */
ساكن ثابت حرف *author_پیام, *author_پیام_میانگیر;
ساكن ثابت حرف *edit_پیام, *use_پیام;
ساكن حرف *fixup_پیام, *fixup_commit, *squash_پیام;
ساكن ثابت حرف *fixup_پیشوند;
ساكن صحيح all, also, interactive, patch_interactive, only, amend, signoff;
ساكن بنية interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
ساكن صحيح edit_flag = -1; /* unspecified */
ساكن صحيح quiet, verbose, no_verify, allow_خالی, dry_پیاپی, renew_authorship;
ساكن صحيح config_commit_verbose = -1; /* unspecified */
ساكن صحيح no_post_rewrite, allow_خالی_پیام, pathspec_file_nul;
ساكن ثابت حرف *untracked_files_arg, *force_date, *ignore_submodule_arg, *ignored_arg;
ساكن ثابت حرف *sign_commit, *pathspec_from_file;
ساكن بنية strvec trailer_آرگومانها = STRVEC_INIT;

/*
 * The default commit message cleanup mode will remove the lines
 * beginning with # (shell comments) and leading and trailing
 * whitespaces (empty lines or containing only whitespaces)
 * if editor is used, and only the whitespaces if the message
 * is specified explicitly.
 */
ساكن تعداد commit_پیام_cleanup_نما cleanup_نما;
ساكن حرف *cleanup_config;

ساكن تعداد commit_whence whence;
ساكن صحيح use_editor = 1, include_وضعیت = 1;
ساكن صحيح have_option_m;
ساكن بنية strbuf پیام_2 = STRBUF_INIT;

ساكن تعداد wt_وضعیت_format وضعیت_format = وضعیت_FORMAT_UNSPECIFIED;

ساكن صحيح opt_parse_porcelain(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	تعداد wt_وضعیت_format *مقدار = (تعداد wt_وضعیت_format *)opt->مقدار;
	إذا (unset)
		*مقدار = وضعیت_FORMAT_NONE;
	وإلا إذا (!arg)
		*مقدار = وضعیت_FORMAT_PORCELAIN;
	وإلا إذا (!strcmp(arg, "v1") || !strcmp(arg, "1"))
		*مقدار = وضعیت_FORMAT_PORCELAIN;
	وإلا إذا (!strcmp(arg, "v2") || !strcmp(arg, "2"))
		*مقدار = وضعیت_FORMAT_PORCELAIN_V2;
	وإلا
		die("unsupported porcelain version '%s'", arg);

	أرجع 0;
}

ساكن صحيح opt_parse_m(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	بنية strbuf *buf = opt->مقدار;
	إذا (unset) {
		have_option_m = 0;
		strbuf_setlen(buf, 0);
	} وإلا {
		have_option_m = 1;
		إذا (buf->len)
			strbuf_addch(buf, '\n');
		strbuf_addstr(buf, arg);
		strbuf_complete_سطر(buf);
	}
	أرجع 0;
}

ساكن صحيح opt_parse_rename_score(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	ثابت حرف **مقدار = opt->مقدار;

	BUG_ON_OPT_NEG(unset);

	إذا (arg != عدم && *arg == '=')
		arg = arg + 1;

	*مقدار = arg;
	أرجع 0;
}

ساكن فراغ determine_whence(بنية wt_وضعیت *s)
{
	إذا (file_exists(git_مسیر_ادغام_سر(the_repository)))
		whence = FROM_ادغام;
	وإلا إذا (!sequencer_determine_whence(the_repository, &whence))
		whence = FROM_COMMIT;
	إذا (s)
		s->whence = whence;
}

ساكن فراغ وضعیت_init_config(بنية wt_وضعیت *s, config_fn_t fn)
{
	wt_وضعیت_prepare(the_repository, s);
	init_diff_ui_defaults();
	repo_config(the_repository, fn, s);
	determine_whence(s);
	s->hints = advice_enabled(ADVICE_وضعیت_HINTS); /* must come after repo_config() */
}

ساكن فراغ rollback_شاخص_files(فراغ)
{
	بدل (commit_style) {
	حالة COMMIT_AS_IS:
		اكسر; /* nothing to do */
	حالة COMMIT_نرمال:
		rollback_قفل_file(&شاخص_قفل);
		اكسر;
	حالة COMMIT_PARTIAL:
		rollback_قفل_file(&شاخص_قفل);
		rollback_قفل_file(&false_قفل);
		اكسر;
	}
}

ساكن صحيح commit_شاخص_files(فراغ)
{
	صحيح err = 0;

	بدل (commit_style) {
	حالة COMMIT_AS_IS:
		اكسر; /* nothing to do */
	حالة COMMIT_نرمال:
		err = commit_قفل_file(&شاخص_قفل);
		اكسر;
	حالة COMMIT_PARTIAL:
		err = commit_قفل_file(&شاخص_قفل);
		rollback_قفل_file(&false_قفل);
		اكسر;
	}

	أرجع err;
}

/*
 * Take a union of paths in the index and the named tree (typically, "HEAD"),
 * and return the paths that match the given pattern in list.
 */
ساكن صحيح list_paths(بنية string_list *list, ثابت حرف *with_درخت,
		      ثابت بنية pathspec *الگو)
{
	صحيح i, ret;
	حرف *m;

	إذا (!الگو->nr)
		أرجع 0;

	m = xcalloc(1, الگو->nr);

	إذا (with_درخت) {
		حرف *max_پیشوند = common_پیشوند(الگو);
		overlay_درخت_on_شاخص(the_repository->شاخص, with_درخت, max_پیشوند);
		حرر(max_پیشوند);
	}

	/* TODO: audit for interaction with sparse-index. */
	ensure_پر_شاخص(the_repository->شاخص);
	لكل (i = 0; i < the_repository->شاخص->cache_nr; i++) {
		ثابت بنية cache_مدخل *ce = the_repository->شاخص->cache[i];
		بنية string_list_قلم *قلم;

		إذا (ce->ce_flags & CE_UPDATE)
			استمر;
		إذا (!ce_مسیر_تطابق(the_repository->شاخص, ce, الگو, m))
			استمر;
		قلم = string_list_درج(list, ce->name);
		إذا (ce_skip_worktree(ce))
			قلم->util = قلم; /* better a valid pointer than a fake one */
	}

	ret = report_مسیر_error(m, الگو);
	حرر(m);
	أرجع ret;
}

ساكن فراغ جمع_کن_remove_files(بنية string_list *list)
{
	صحيح i;
	لكل (i = 0; i < list->nr; i++) {
		بنية stat st;
		بنية string_list_قلم *p = &(list->اقلام[i]);

		/* p->util is skip-worktree */
		إذا (p->util)
			استمر;

		إذا (!lstat(p->string, &st)) {
			إذا (جمع_کن_مقصد_شاخص(the_repository->شاخص, p->string, &st, 0))
				die(_("updating files failed"));
		} وإلا
			remove_file_from_شاخص(the_repository->شاخص, p->string);
	}
}

ساكن فراغ create_پایه_شاخص(ثابت بنية commit *current_سر)
{
	بنية درخت *درخت;
	بنية unpack_trees_options opts;
	بنية درخت_desc t;

	إذا (!current_سر) {
		discard_شاخص(the_repository->شاخص);
		أرجع;
	}

	memset(&opts, 0, حجم(opts));
	opts.سر_idx = 1;
	opts.شاخص_only = 1;
	opts.ادغام = 1;
	opts.src_شاخص = the_repository->شاخص;
	opts.dst_شاخص = the_repository->شاخص;

	opts.fn = oneway_ادغام;
	درخت = repo_parse_درخت_غیرمستقیم(the_repository,
					&current_سر->object.oid);
	إذا (!درخت)
		die(_("failed to unpack HEAD tree object"));
	إذا (repo_parse_درخت(the_repository, درخت) < 0)
		خروج(128);
	init_درخت_desc(&t, &درخت->object.oid, درخت->میانگیر, درخت->اندازه);
	إذا (unpack_trees(1, &t, &opts))
		خروج(128); /* We've already reported the error, finish dying */
}

ساكن فراغ refresh_cache_or_die(صحيح refresh_flags)
{
	/*
	 * refresh_flags contains REFRESH_QUIET, so the only errors
	 * are for unmerged entries.
	 */
	إذا (refresh_شاخص(the_repository->شاخص, refresh_flags | REFRESH_درون_PORCELAIN, عدم, عدم, عدم))
		die_resolve_conflict("commit");
}

ساكن ثابت حرف *prepare_شاخص(ثابت حرف **argv, ثابت حرف *پیشوند,
				 ثابت بنية commit *current_سر, صحيح is_وضعیت)
{
	بنية string_list partial = STRING_LIST_INIT_DUP;
	بنية pathspec pathspec;
	صحيح refresh_flags = REFRESH_QUIET;
	ثابت حرف *ret;
	حرف *مسیر = عدم;

	إذا (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	إذا (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	إذا (is_وضعیت)
		refresh_flags |= REFRESH_UNMERGED;
	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_پر,
		       پیشوند, argv);

	إذا (pathspec_from_file) {
		إذا (interactive)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--interactive/--patch");

		إذا (all)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "-a");

		إذا (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_پر,
				    پیشوند, pathspec_from_file, pathspec_file_nul);
	} وإلا إذا (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	إذا (!pathspec.nr && (also || (only && !allow_خالی &&
	    (!amend || (fixup_پیام && strcmp(fixup_پیشوند, "amend"))))))
		die(_("No paths with --include/--only does not make sense."));

	إذا (repo_read_شاخص_preload(the_repository, &pathspec, 0) < 0)
		die(_("index file corrupt"));

	إذا (interactive) {
		حرف *old_شاخص_env = عدم, *old_repo_شاخص_file;
		repo_hold_locked_شاخص(the_repository, &شاخص_قفل,
				       قفل_DIE_ON_ERROR);

		refresh_cache_or_die(refresh_flags);

		إذا (write_locked_شاخص(the_repository->شاخص, &شاخص_قفل, 0))
			die(_("unable to create temporary index"));

		old_repo_شاخص_file = the_repository->شاخص_file;
		the_repository->شاخص_file =
			(حرف *)get_قفل_file_مسیر(&شاخص_قفل);
		old_شاخص_env = xstrdup_or_null(getenv(شاخص_ENVIRONMENT));
		setenv(شاخص_ENVIRONMENT, the_repository->شاخص_file, 1);

		إذا (interactive_جمع_کن(the_repository, argv, پیشوند, patch_interactive, &interactive_opts) != 0)
			die(_("interactive add failed"));

		the_repository->شاخص_file = old_repo_شاخص_file;
		إذا (old_شاخص_env && *old_شاخص_env)
			setenv(شاخص_ENVIRONMENT, old_شاخص_env, 1);
		وإلا
			unsetenv(شاخص_ENVIRONMENT);
		FREE_AND_NULL(old_شاخص_env);

		discard_شاخص(the_repository->شاخص);
		read_شاخص_from(the_repository->شاخص, get_قفل_file_مسیر(&شاخص_قفل),
				repo_get_git_dir(the_repository));
		إذا (cache_درخت_update(the_repository->شاخص, WRITE_درخت_SILENT) == 0) {
			إذا (reopen_قفل_file(&شاخص_قفل) < 0)
				die(_("unable to write index file"));
			إذا (write_locked_شاخص(the_repository->شاخص, &شاخص_قفل, 0))
				die(_("unable to update temporary index"));
		} وإلا
			warning(_("Failed to update main cache tree"));

		commit_style = COMMIT_نرمال;
		ret = get_قفل_file_مسیر(&شاخص_قفل);
		اذهب بیرون;
	} وإلا {
		إذا (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--interactive/--patch");
		إذا (interactive_opts.interhunkcontext != -1)
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
	إذا (all || (also && pathspec.nr)) {
		حرف *ps_matched = xcalloc(pathspec.nr, 1);
		repo_hold_locked_شاخص(the_repository, &شاخص_قفل,
				       قفل_DIE_ON_ERROR);
		جمع_کن_files_مقصد_cache(the_repository, also ? پیشوند : عدم,
				   &pathspec, ps_matched, 0, 0, 0 );
		إذا (!all && report_مسیر_error(ps_matched, &pathspec))
			خروج(128);

		refresh_cache_or_die(refresh_flags);
		cache_درخت_update(the_repository->شاخص, WRITE_درخت_SILENT);
		إذا (write_locked_شاخص(the_repository->شاخص, &شاخص_قفل, 0))
			die(_("unable to write new index file"));
		commit_style = COMMIT_نرمال;
		ret = get_قفل_file_مسیر(&شاخص_قفل);
		حرر(ps_matched);
		اذهب بیرون;
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
	إذا (!only && !pathspec.nr) {
		repo_hold_locked_شاخص(the_repository, &شاخص_قفل,
				       قفل_DIE_ON_ERROR);
		refresh_cache_or_die(refresh_flags);
		إذا (the_repository->شاخص->cache_تغییریافته
		    || !cache_درخت_fully_valid(the_repository->شاخص))
			cache_درخت_update(the_repository->شاخص, WRITE_درخت_SILENT);
		إذا (write_locked_شاخص(the_repository->شاخص, &شاخص_قفل,
				       COMMIT_قفل | SKIP_IF_UNCHANGED))
			die(_("unable to write new index file"));
		commit_style = COMMIT_AS_IS;
		ret = repo_get_شاخص_file(the_repository);
		اذهب بیرون;
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

	إذا (whence != FROM_COMMIT) {
		إذا (whence == FROM_ادغام)
			die(_("cannot do a partial commit during a merge."));
		وإلا إذا (is_from_cherry_pick(whence))
			die(_("cannot do a partial commit during a cherry-pick."));
		وإلا إذا (is_from_rebase(whence))
			die(_("cannot do a partial commit during a rebase."));
	}

	إذا (list_paths(&partial, !current_سر ? عدم : "HEAD", &pathspec))
		خروج(1);

	discard_شاخص(the_repository->شاخص);
	إذا (repo_read_شاخص(the_repository) < 0)
		die(_("cannot read the index"));

	repo_hold_locked_شاخص(the_repository, &شاخص_قفل, قفل_DIE_ON_ERROR);
	جمع_کن_remove_files(&partial);
	refresh_شاخص(the_repository->شاخص, REFRESH_QUIET, عدم, عدم, عدم);
	cache_درخت_update(the_repository->شاخص, WRITE_درخت_SILENT);
	إذا (write_locked_شاخص(the_repository->شاخص, &شاخص_قفل, 0))
		die(_("unable to write new index file"));

	مسیر = repo_git_مسیر(the_repository, "next-index-%"PRIuMAX,
			     (uintmax_t) getpid());
	hold_قفل_file_for_update(&false_قفل, مسیر,
				  قفل_DIE_ON_ERROR);

	create_پایه_شاخص(current_سر);
	جمع_کن_remove_files(&partial);
	refresh_شاخص(the_repository->شاخص, REFRESH_QUIET, عدم, عدم, عدم);

	إذا (write_locked_شاخص(the_repository->شاخص, &false_قفل, 0))
		die(_("unable to write temporary index file"));

	discard_شاخص(the_repository->شاخص);
	ret = get_قفل_file_مسیر(&false_قفل);
	read_شاخص_from(the_repository->شاخص, ret, repo_get_git_dir(the_repository));
بیرون:
	string_list_clear(&partial, 0);
	clear_pathspec(&pathspec);
	حرر(مسیر);
	أرجع ret;
}

ساكن صحيح پیاپی_وضعیت(FILE *fp, ثابت حرف *شاخص_file, ثابت حرف *پیشوند, صحيح nowarn,
		      بنية wt_وضعیت *s)
{
	بنية object_id oid;

	إذا (s->relative_paths)
		s->پیشوند = پیشوند;

	إذا (amend) {
		s->amend = 1;
		s->reference = "HEAD^1";
	}
	s->verbose = verbose;
	s->شاخص_file = شاخص_file;
	s->fp = fp;
	s->nowarn = nowarn;
	s->is_initial = repo_get_oid(the_repository, s->reference, &oid) ? 1 : 0;
	إذا (!s->is_initial)
		oidcpy(&s->oid_commit, &oid);
	s->وضعیت_format = وضعیت_format;
	s->ignore_submodule_arg = ignore_submodule_arg;

	wt_وضعیت_collect(s);
	wt_وضعیت_print(s);
	wt_وضعیت_collect_free_buffers(s);

	أرجع s->committable;
}

ساكن صحيح is_a_ادغام(ثابت بنية commit *current_سر)
{
	أرجع !!(current_سر->parents && current_سر->parents->بعدی);
}

ساكن فراغ assert_تقسیم_ident(بنية ident_تقسیم *id, ثابت بنية strbuf *buf)
{
	إذا (تقسیم_ident_سطر(id, buf->buf, buf->len) || !id->date_begin)
		BUG("unable to parse our own ident: %s", buf->buf);
}

ساكن فراغ export_one(ثابت حرف *var, ثابت حرف *s, ثابت حرف *e, صحيح hack)
{
	بنية strbuf buf = STRBUF_INIT;
	إذا (hack)
		strbuf_addch(&buf, hack);
	strbuf_جمع_کن(&buf, s, e - s);
	setenv(var, buf.buf, 1);
	strbuf_release(&buf);
}

ساكن صحيح parse_force_date(ثابت حرف *درون, بنية strbuf *بیرون)
{
	strbuf_addch(بیرون, '@');

	إذا (parse_date(درون, بیرون) < 0) {
		صحيح errors = 0;
		بیعلامت دراز t = approxidate_careful(درون, &errors);
		إذا (errors)
			أرجع -1;
		strbuf_addf(بیرون, "%lu", t);
	}

	أرجع 0;
}

ساكن فراغ مجموعه_ident_var(حرف **buf, حرف *val)
{
	حرر(*buf);
	*buf = val;
}

ساكن فراغ determine_author_info(بنية strbuf *author_ident)
{
	حرف *name, *email, *date;
	بنية ident_تقسیم author;

	name = xstrdup_or_null(getenv("GIT_AUTHOR_NAME"));
	email = xstrdup_or_null(getenv("GIT_AUTHOR_EMAIL"));
	date = xstrdup_or_null(getenv("GIT_AUTHOR_DATE"));

	إذا (author_پیام) {
		بنية ident_تقسیم ident;
		اندازه_t len;
		ثابت حرف *a;

		a = پیدا_کن_commit_سرآیند(author_پیام_میانگیر, "author", &len);
		إذا (!a)
			die(_("commit '%s' lacks author header"), author_پیام);
		إذا (تقسیم_ident_سطر(&ident, a, len) < 0)
			die(_("commit '%s' has malformed author line"), author_پیام);

		مجموعه_ident_var(&name, xmemdupz(ident.name_begin, ident.name_انتها - ident.name_begin));
		مجموعه_ident_var(&email, xmemdupz(ident.mail_begin, ident.mail_انتها - ident.mail_begin));

		إذا (ident.date_begin) {
			بنية strbuf date_buf = STRBUF_INIT;
			strbuf_addch(&date_buf, '@');
			strbuf_جمع_کن(&date_buf, ident.date_begin, ident.date_انتها - ident.date_begin);
			strbuf_addch(&date_buf, ' ');
			strbuf_جمع_کن(&date_buf, ident.tz_begin, ident.tz_انتها - ident.tz_begin);
			مجموعه_ident_var(&date, strbuf_detach(&date_buf, عدم));
		}
	}

	إذا (force_author) {
		بنية ident_تقسیم ident;

		إذا (تقسیم_ident_سطر(&ident, force_author, طول_رشته(force_author)) < 0)
			die(_("malformed --author parameter"));
		مجموعه_ident_var(&name, xmemdupz(ident.name_begin, ident.name_انتها - ident.name_begin));
		مجموعه_ident_var(&email, xmemdupz(ident.mail_begin, ident.mail_انتها - ident.mail_begin));
	}

	إذا (force_date) {
		بنية strbuf date_buf = STRBUF_INIT;
		إذا (parse_force_date(force_date, &date_buf))
			die(_("invalid date format: %s"), force_date);
		مجموعه_ident_var(&date, strbuf_detach(&date_buf, عدم));
	}

	strbuf_addstr(author_ident, fmt_ident(name, email, WANT_AUTHOR_IDENT, date,
				IDENT_STRICT));
	assert_تقسیم_ident(&author, author_ident);
	export_one("GIT_AUTHOR_NAME", author.name_begin, author.name_انتها, 0);
	export_one("GIT_AUTHOR_EMAIL", author.mail_begin, author.mail_انتها, 0);
	export_one("GIT_AUTHOR_DATE", author.date_begin, author.tz_انتها, '@');
	حرر(name);
	حرر(email);
	حرر(date);
}

ساكن صحيح author_date_is_interesting(فراغ)
{
	أرجع author_پیام || force_date;
}

#اگر_تعریف_نشده WITH_BREAKING_CHANGES
ساكن فراغ adjust_comment_سطر_char(ثابت بنية strbuf *sb)
{
	حرف candidates[] = "#;@!$%^&|:";
	حرف *candidate;
	ثابت حرف *p;
	اندازه_t cutoff;

	/* Ignore comment chars in trailing comments (e.g., Conflicts:) */
	cutoff = sb->len - ignored_log_پیام_بایتها(sb->buf, sb->len);

	إذا (!memchr(sb->buf, candidates[0], sb->len)) {
		حرر(comment_سطر_str_مقصد_free);
		comment_سطر_str = comment_سطر_str_مقصد_free =
			xstrfmt("%c", candidates[0]);
		أرجع;
	}

	p = sb->buf;
	candidate = strchr(candidates, *p);
	إذا (candidate)
		*candidate = ' ';
	لكل (p = sb->buf; p + 1 < sb->buf + cutoff; p++) {
		إذا ((p[0] == '\n' || p[0] == '\r') && p[1]) {
			candidate = strchr(candidates, p[1]);
			إذا (candidate)
				*candidate = ' ';
		}
	}

	لكل (p = candidates; *p == ' '; p++)
		;
	إذا (!*p)
		die(_("unable to select a comment character that is not used\n"
		      "in the current commit message"));
	حرر(comment_سطر_str_مقصد_free);
	comment_سطر_str = comment_سطر_str_مقصد_free = xstrfmt("%c", *p);
}
#پایان_اگر /* !WITH_BREAKING_CHANGES */

ساكن فراغ prepare_amend_commit(بنية commit *commit, بنية strbuf *sb,
				بنية pretty_print_context *ctx)
{
	ثابت حرف *میانگیر, *subject, *fmt;

	میانگیر = repo_get_commit_میانگیر(the_repository, commit, عدم);
	پیدا_کن_commit_subject(میانگیر, &subject);
	/*
	 * If we amend the 'amend!' commit then we don't want to
	 * duplicate the subject line.
	 */
	fmt = starts_with(subject, "amend!") ? "%b" : "%B";
	repo_format_commit_پیام(the_repository, commit, fmt, sb, ctx);
	repo_unuse_commit_میانگیر(the_repository, commit, میانگیر);
}

ساكن فراغ change_داده_free(فراغ *util, ثابت حرف *str استفادهنشده)
{
	بنية wt_وضعیت_change_داده *d = util;
	حرر(d->rename_منبع);
	حرر(d);
}

ساكن صحيح prepare_مقصد_commit(ثابت حرف *شاخص_file, ثابت حرف *پیشوند,
			     بنية commit *current_سر,
			     بنية wt_وضعیت *s,
			     بنية strbuf *author_ident)
{
	بنية stat statbuf;
	بنية strbuf committer_ident = STRBUF_INIT;
	صحيح committable;
	بنية strbuf sb = STRBUF_INIT;
	ثابت حرف *hook_arg1 = عدم;
	ثابت حرف *hook_arg2 = عدم;
	صحيح clean_پیام_contents = (cleanup_نما != COMMIT_پیام_CLEANUP_NONE);
	صحيح old_display_comment_پیشوند;
	صحيح invoked_hook;

	/* This checks and barfs if author is badly specified */
	determine_author_info(author_ident);

	إذا (!no_verify && پیاپی_commit_hook(use_editor, شاخص_file, &invoked_hook,
					  "pre-commit", عدم))
		أرجع 0;

	إذا (squash_پیام) {
		/*
		 * Insert the proper subject line before other commit
		 * message options add their content.
		 */
		إذا (use_پیام && !strcmp(use_پیام, squash_پیام))
			strbuf_addstr(&sb, "squash! ");
		وإلا {
			بنية pretty_print_context ctx = {0};
			بنية commit *c;
			c = lookup_commit_reference_by_name(squash_پیام);
			إذا (!c)
				die(_("could not lookup commit '%s'"), squash_پیام);
			ctx.output_encoding = get_commit_output_encoding();
			repo_format_commit_پیام(the_repository, c,
						   "squash! %s\n\n", &sb,
						   &ctx);
		}
	}

	إذا (have_option_m && !fixup_پیام) {
		strbuf_addbuf(&sb, &پیام_2);
		hook_arg1 = "message";
	} وإلا إذا (logfile && !strcmp(logfile, "-")) {
		إذا (isatty(0))
			fprintf(stderr, _("(reading log message from standard input)\n"));
		إذا (strbuf_read(&sb, 0, 0) < 0)
			die_errno(_("could not read log from standard input"));
		hook_arg1 = "message";
	} وإلا إذا (logfile) {
		إذا (strbuf_read_file(&sb, logfile, 0) < 0)
			die_errno(_("could not read log file '%s'"),
				  logfile);
		hook_arg1 = "message";
	} وإلا إذا (use_پیام) {
		ثابت حرف *میانگیر;
		میانگیر = جستجوی_رشته(use_پیام_میانگیر, "\n\n");
		إذا (میانگیر)
			strbuf_addstr(&sb, skip_blank_سطرها(میانگیر + 2));
		hook_arg1 = "commit";
		hook_arg2 = use_پیام;
	} وإلا إذا (fixup_پیام) {
		بنية pretty_print_context ctx = {0};
		بنية commit *commit;
		حرف *fmt;
		commit = lookup_commit_reference_by_name(fixup_commit);
		إذا (!commit)
			die(_("could not lookup commit '%s'"), fixup_commit);
		ctx.output_encoding = get_commit_output_encoding();
		fmt = xstrfmt("%s! %%s\n\n", fixup_پیشوند);
		repo_format_commit_پیام(the_repository, commit, fmt, &sb,
					   &ctx);
		حرر(fmt);
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
		إذا (have_option_m && !strcmp(fixup_پیشوند, "fixup"))
			strbuf_addbuf(&sb, &پیام_2);

		إذا (!strcmp(fixup_پیشوند, "amend")) {
			إذا (have_option_m)
				die(_("options '%s' and '%s:%s' cannot be used together"), "-m", "--fixup", fixup_پیام);
			prepare_amend_commit(commit, &sb, &ctx);
		}
	} وإلا إذا (!stat(git_مسیر_ادغام_پیام(the_repository), &statbuf)) {
		اندازه_t ادغام_پیام_شروع;

		/*
		 * prepend SQUASH_MSG here if it exists and a
		 * "merge --squash" was originally performed
		 */
		إذا (!stat(git_مسیر_squash_پیام(the_repository), &statbuf)) {
			إذا (strbuf_read_file(&sb, git_مسیر_squash_پیام(the_repository), 0) < 0)
				die_errno(_("could not read SQUASH_MSG"));
			hook_arg1 = "squash";
		} وإلا
			hook_arg1 = "merge";

		ادغام_پیام_شروع = sb.len;
		إذا (strbuf_read_file(&sb, git_مسیر_ادغام_پیام(the_repository), 0) < 0)
			die_errno(_("could not read MERGE_MSG"));

		إذا (cleanup_نما == COMMIT_پیام_CLEANUP_SCISSORS &&
		    wt_وضعیت_locate_انتها(sb.buf + ادغام_پیام_شروع,
					 sb.len - ادغام_پیام_شروع) <
				sb.len - ادغام_پیام_شروع)
			s->added_cut_سطر = 1;
	} وإلا إذا (!stat(git_مسیر_squash_پیام(the_repository), &statbuf)) {
		إذا (strbuf_read_file(&sb, git_مسیر_squash_پیام(the_repository), 0) < 0)
			die_errno(_("could not read SQUASH_MSG"));
		hook_arg1 = "squash";
	} وإلا إذا (template_file) {
		إذا (strbuf_read_file(&sb, template_file, 0) < 0)
			die_errno(_("could not read '%s'"), template_file);
		hook_arg1 = "template";
		clean_پیام_contents = 0;
	}

	/*
	 * The remaining cases don't modify the template message, but
	 * just set the argument(s) to the prepare-commit-msg hook.
	 */
	وإلا إذا (whence == FROM_ادغام)
		hook_arg1 = "merge";
	وإلا إذا (is_from_cherry_pick(whence) || whence == FROM_REBASE_PICK) {
		hook_arg1 = "commit";
		hook_arg2 = "CHERRY_PICK_HEAD";
	}

	إذا (squash_پیام) {
		/*
		 * If squash_commit was used for the commit subject,
		 * then we're possibly hijacking other commit log options.
		 * Reset the hook args to tell the real story.
		 */
		hook_arg1 = "message";
		hook_arg2 = "";
	}

	s->fp = fopen_for_writing(git_مسیر_commit_editmsg());
	إذا (!s->fp)
		die_errno(_("could not open '%s'"), git_مسیر_commit_editmsg());

	/* Ignore status.displayCommentPrefix: we do need comments in COMMIT_EDITMSG. */
	old_display_comment_پیشوند = s->display_comment_پیشوند;
	s->display_comment_پیشوند = 1;

	/*
	 * Most hints are counter-productive when the commit has
	 * already started.
	 */
	s->hints = 0;

	إذا (clean_پیام_contents)
		strbuf_stripspace(&sb, عدم);

	إذا (signoff)
		append_signoff(&sb, ignored_log_پیام_بایتها(sb.buf, sb.len), 0);

	إذا (fwrite(sb.buf, 1, sb.len, s->fp) < sb.len)
		die_errno(_("could not write commit template"));

#اگر_تعریف_نشده WITH_BREAKING_CHANGES
	إذا (auto_comment_سطر_char)
		adjust_comment_سطر_char(&sb);
#پایان_اگر /* !WITH_BREAKING_CHANGES */
	strbuf_release(&sb);

	/* This checks if committer ident is explicitly given */
	strbuf_addstr(&committer_ident, git_committer_info(IDENT_STRICT));
	إذا (use_editor && include_وضعیت) {
		صحيح ident_shown = 0;
		تعداد git_colorbool saved_color_setting;
		بنية ident_تقسیم ci, ai;
		ثابت حرف *hint_cleanup_all = allow_خالی_پیام ?
			_("Please enter the commit message for your changes."
			  " Lines starting\nwith '%s' will be ignored.\n") :
			_("Please enter the commit message for your changes."
			  " Lines starting\nwith '%s' will be ignored, and an empty"
			  " message aborts the commit.\n");
		ثابت حرف *hint_cleanup_space = allow_خالی_پیام ?
			_("Please enter the commit message for your changes."
			  " Lines starting\n"
			  "with '%s' will be kept; you may remove them"
			  " yourself if you want to.\n") :
			_("Please enter the commit message for your changes."
			  " Lines starting\n"
			  "with '%s' will be kept; you may remove them"
			  " yourself if you want to.\n"
			  "An empty message aborts the commit.\n");
		إذا (whence != FROM_COMMIT) {
			إذا (cleanup_نما == COMMIT_پیام_CLEANUP_SCISSORS)
				wt_وضعیت_جمع_کن_cut_سطر(s);
			وضعیت_printf_ln(
				s, GIT_COLOR_نرمال,
				whence == FROM_ادغام ?
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
		إذا (cleanup_نما == COMMIT_پیام_CLEANUP_ALL)
			وضعیت_printf(s, GIT_COLOR_نرمال, hint_cleanup_all, comment_سطر_str);
		وإلا إذا (cleanup_نما == COMMIT_پیام_CLEANUP_SCISSORS) {
			إذا (whence == FROM_COMMIT)
				wt_وضعیت_جمع_کن_cut_سطر(s);
		} وإلا /* COMMIT_MSG_CLEANUP_SPACE, that is. */
			وضعیت_printf(s, GIT_COLOR_نرمال, hint_cleanup_space, comment_سطر_str);

		/*
		 * These should never fail because they come from our own
		 * fmt_ident. They may fail the sane_ident test, but we know
		 * that the name and mail pointers will at least be valid,
		 * which is enough for our tests and printing here.
		 */
		assert_تقسیم_ident(&ai, author_ident);
		assert_تقسیم_ident(&ci, &committer_ident);

		إذا (ident_cmp(&ai, &ci))
			وضعیت_printf_ln(s, GIT_COLOR_نرمال,
				_("%s"
				"Author:    %.*s <%.*s>"),
				ident_shown++ ? "" : "\n",
				(صحيح)(ai.name_انتها - ai.name_begin), ai.name_begin,
				(صحيح)(ai.mail_انتها - ai.mail_begin), ai.mail_begin);

		إذا (author_date_is_interesting())
			وضعیت_printf_ln(s, GIT_COLOR_نرمال,
				_("%s"
				"Date:      %s"),
				ident_shown++ ? "" : "\n",
				show_ident_date(&ai, DATE_نما(نرمال)));

		إذا (!committer_ident_sufficiently_given())
			وضعیت_printf_ln(s, GIT_COLOR_نرمال,
				_("%s"
				"Committer: %.*s <%.*s>"),
				ident_shown++ ? "" : "\n",
				(صحيح)(ci.name_انتها - ci.name_begin), ci.name_begin,
				(صحيح)(ci.mail_انتها - ci.mail_begin), ci.mail_begin);

		وضعیت_printf_ln(s, GIT_COLOR_نرمال, "%s", ""); /* Add new line for clarity */

		saved_color_setting = s->use_color;
		s->use_color = GIT_COLOR_NEVER;
		committable = پیاپی_وضعیت(s->fp, شاخص_file, پیشوند, 1, s);
		s->use_color = saved_color_setting;
		string_list_clear_func(&s->change, change_داده_free);
	} وإلا {
		بنية object_id oid;
		ثابت حرف *والد = "HEAD";

		إذا (!the_repository->شاخص->initialized && repo_read_شاخص(the_repository) < 0)
			die(_("Cannot read index"));

		إذا (amend)
			والد = "HEAD^1";

		إذا (repo_get_oid(the_repository, والد, &oid)) {
			صحيح i, ita_nr = 0;

			/* TODO: audit for interaction with sparse-index. */
			ensure_پر_شاخص(the_repository->شاخص);
			لكل (i = 0; i < the_repository->شاخص->cache_nr; i++)
				إذا (ce_intent_مقصد_جمع_کن(the_repository->شاخص->cache[i]))
					ita_nr++;
			committable = the_repository->شاخص->cache_nr > ita_nr;
		} وإلا {
			/*
			 * Unless the user did explicitly request a submodule
			 * ignore mode by passing a command line option we do
			 * not ignore any changed submodule SHA-1s when
			 * comparing index and parent, no matter what is
			 * configured. Otherwise we won't commit any
			 * submodules which were manually staged, which would
			 * be really confusing.
			 */
			بنية diff_flags flags = DIFF_FLAGS_INIT;
			flags.override_submodule_config = 1;
			إذا (ignore_submodule_arg &&
			    !strcmp(ignore_submodule_arg, "all"))
				flags.ignore_submodules = 1;
			committable = شاخص_differs_from(the_repository,
							 والد, &flags, 1);
		}
	}
	strbuf_release(&committer_ident);

	fclose(s->fp);

	إذا (trailer_آرگومانها.nr) {
		إذا (amend_file_with_trailers(git_مسیر_commit_editmsg(), &trailer_آرگومانها))
			die(_("unable to pass trailers to --trailers"));
		strvec_clear(&trailer_آرگومانها);
	}

	/*
	 * Reject an attempt to record a non-merge empty commit without
	 * explicit --allow-empty. In the cherry-pick case, it may be
	 * empty due to conflict resolution, which the user should okay.
	 */
	إذا (!committable && whence != FROM_ادغام && !allow_خالی &&
	    !(amend && is_a_ادغام(current_سر))) {
		s->hints = advice_enabled(ADVICE_وضعیت_HINTS);
		s->display_comment_پیشوند = old_display_comment_پیشوند;
		پیاپی_وضعیت(stdout, شاخص_file, پیشوند, 0, s);
		إذا (amend)
			fputs(_(خالی_amend_advice), stderr);
		وإلا إذا (is_from_cherry_pick(whence) ||
			 whence == FROM_REBASE_PICK) {
			fputs(_(خالی_cherry_pick_advice), stderr);
			إذا (whence == FROM_CHERRY_PICK_SINGLE)
				fputs(_(خالی_cherry_pick_advice_single), stderr);
			وإلا إذا (whence == FROM_CHERRY_PICK_MULTI)
				fputs(_(خالی_cherry_pick_advice_multi), stderr);
			وإلا
				fputs(_(خالی_rebase_pick_advice), stderr);
		}
		أرجع 0;
	}

	إذا (!no_verify && invoked_hook) {
		/*
		 * Re-read the index as the pre-commit-commit hook was invoked
		 * and could have updated it. We must do this before we invoke
		 * the editor and after we invoke run_status above.
		 */
		discard_شاخص(the_repository->شاخص);
	}
	read_شاخص_from(the_repository->شاخص, شاخص_file, repo_get_git_dir(the_repository));

	إذا (cache_درخت_update(the_repository->شاخص, 0)) {
		error(_("Error building trees"));
		أرجع 0;
	}

	إذا (پیاپی_commit_hook(use_editor, شاخص_file, عدم, "prepare-commit-msg",
			    git_مسیر_commit_editmsg(), hook_arg1, hook_arg2, عدم))
		أرجع 0;

	إذا (use_editor) {
		بنية strvec env = STRVEC_INIT;

		strvec_pushf(&env, "GIT_INDEX_FILE=%s", شاخص_file);
		إذا (launch_editor(git_مسیر_commit_editmsg(), عدم, env.v)) {
			fprintf(stderr,
			_("Please supply the message using either -m or -F option.\n"));
			خروج(1);
		}
		strvec_clear(&env);
	}

	إذا (!no_verify &&
	    پیاپی_commit_hook(use_editor, شاخص_file, عدم, "commit-msg",
			    git_مسیر_commit_editmsg(), عدم)) {
		أرجع 0;
	}

	أرجع 1;
}

ساكن ثابت حرف *پیدا_کن_author_by_nickname(ثابت حرف *name)
{
	بنية rev_info revs;
	بنية commit *commit;
	بنية strbuf buf = STRBUF_INIT;
	ثابت حرف *av[20];
	صحيح ac = 0;

	repo_init_revisions(the_repository, &revs, عدم);
	strbuf_addf(&buf, "--author=%s", name);
	av[++ac] = "--all";
	av[++ac] = "-i";
	av[++ac] = buf.buf;
	av[++ac] = عدم;
	راهاندازی_revisions(ac, av, &revs, عدم);
	revs.mailmap = xmalloc(حجم(بنية string_list));
	string_list_init_nodup(revs.mailmap);
	read_mailmap(the_repository, revs.mailmap);

	إذا (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	commit = get_revision(&revs);
	إذا (commit) {
		بنية pretty_print_context ctx = {0};
		ctx.date_نما.type = DATE_نرمال;
		strbuf_release(&buf);
		repo_format_commit_پیام(the_repository, commit,
					   "%aN <%aE>", &buf, &ctx);
		release_revisions(&revs);
		أرجع strbuf_detach(&buf, عدم);
	}
	die(_("--author '%s' is not 'Name <email>' and matches no existing author"), name);
}

ساكن فراغ handle_ignored_arg(بنية wt_وضعیت *s)
{
	إذا (!ignored_arg)
		; /* default already initialized */
	وإلا إذا (!strcmp(ignored_arg, "traditional"))
		s->show_ignored_نما = SHOW_TRADITIONAL_IGNORED;
	وإلا إذا (!strcmp(ignored_arg, "no"))
		s->show_ignored_نما = SHOW_NO_IGNORED;
	وإلا إذا (!strcmp(ignored_arg, "matching"))
		s->show_ignored_نما = SHOW_MATCHING_IGNORED;
	وإلا
		die(_("Invalid ignored mode '%s'"), ignored_arg);
}

ساكن تعداد untracked_وضعیت_type parse_untracked_setting_name(ثابت حرف *u)
{
	/*
	 * Please update $__git_untracked_file_modes in
	 * git-completion.bash when you add new options
	 */
	بدل (git_parse_maybe_bool(u)) {
	حالة 0:
		u = "no";
		اكسر;
	حالة 1:
		u = "normal";
		اكسر;
	افتراضي:
		اكسر;
	}

	إذا (!strcmp(u, "no"))
		أرجع SHOW_NO_UNTRACKED_FILES;
	وإلا إذا (!strcmp(u, "normal"))
		أرجع SHOW_نرمال_UNTRACKED_FILES;
	وإلا إذا (!strcmp(u, "all"))
		أرجع SHOW_ALL_UNTRACKED_FILES;
	وإلا
		أرجع SHOW_UNTRACKED_FILES_ERROR;
}

ساكن فراغ handle_untracked_files_arg(بنية wt_وضعیت *s)
{
	تعداد untracked_وضعیت_type u;

	إذا (!untracked_files_arg)
		أرجع; /* default already initialized */

	u = parse_untracked_setting_name(untracked_files_arg);
	إذا (u == SHOW_UNTRACKED_FILES_ERROR)
		die(_("Invalid untracked files mode '%s'"),
		    untracked_files_arg);
	s->show_untracked_files = u;
}

ساكن ثابت حرف *read_commit_پیام_2(ثابت حرف *name)
{
	ثابت حرف *بیرون_enc;
	بنية commit *commit;

	commit = lookup_commit_reference_by_name(name);
	إذا (!commit)
		die(_("could not lookup commit '%s'"), name);
	بیرون_enc = get_commit_output_encoding();
	أرجع repo_logmsg_reencode(the_repository, commit, عدم, بیرون_enc);
}

/*
 * Enumerate what needs to be propagated when --porcelain
 * is not in effect here.
 */
ساكن بنية وضعیت_deferred_config {
	تعداد wt_وضعیت_format وضعیت_format;
	صحيح show_branch;
	تعداد ahead_behind_flags ahead_behind;
} وضعیت_deferred_config = {
	وضعیت_FORMAT_UNSPECIFIED,
	-1, /* unspecified */
	AHEAD_BEHIND_UNSPECIFIED,
};

ساكن فراغ finalize_deferred_config(بنية wt_وضعیت *s)
{
	صحيح use_deferred_config = (وضعیت_format != وضعیت_FORMAT_PORCELAIN &&
				   وضعیت_format != وضعیت_FORMAT_PORCELAIN_V2 &&
				   !s->null_termination);

	إذا (s->null_termination) {
		إذا (وضعیت_format == وضعیت_FORMAT_NONE ||
		    وضعیت_format == وضعیت_FORMAT_UNSPECIFIED)
			وضعیت_format = وضعیت_FORMAT_PORCELAIN;
		وإلا إذا (وضعیت_format == وضعیت_FORMAT_LONG)
			die(_("options '%s' and '%s' cannot be used together"), "--long", "-z");
	}

	إذا (use_deferred_config && وضعیت_format == وضعیت_FORMAT_UNSPECIFIED)
		وضعیت_format = وضعیت_deferred_config.وضعیت_format;
	إذا (وضعیت_format == وضعیت_FORMAT_UNSPECIFIED)
		وضعیت_format = وضعیت_FORMAT_NONE;

	إذا (use_deferred_config && s->show_branch < 0)
		s->show_branch = وضعیت_deferred_config.show_branch;
	إذا (s->show_branch < 0)
		s->show_branch = 0;

	/*
	 * If the user did not give a "--[no]-ahead-behind" command
	 * line argument *AND* we will print in a human-readable format
	 * (short, long etc.) then we inherit from the status.aheadbehind
	 * config setting.  In all other cases (and porcelain V[12] formats
	 * in particular), we inherit _FULL for backwards compatibility.
	 */
	إذا (use_deferred_config &&
	    s->ahead_behind_flags == AHEAD_BEHIND_UNSPECIFIED)
		s->ahead_behind_flags = وضعیت_deferred_config.ahead_behind;

	إذا (s->ahead_behind_flags == AHEAD_BEHIND_UNSPECIFIED)
		s->ahead_behind_flags = AHEAD_BEHIND_پر;
}

ساكن فراغ check_fixup_reword_options(صحيح argc, ثابت حرف *argv[]) {
	إذا (whence != FROM_COMMIT) {
		إذا (whence == FROM_ادغام)
			die(_("You are in the middle of a merge -- cannot reword."));
		وإلا إذا (is_from_cherry_pick(whence))
			die(_("You are in the middle of a cherry-pick -- cannot reword."));
	}
	إذا (argc)
		die(_("reword option of '%s' and path '%s' cannot be used together"), "--fixup", *argv);
	إذا (patch_interactive || interactive || all || also || only)
		die(_("reword option of '%s' and '%s' cannot be used together"),
			"--fixup", "--patch/--interactive/--all/--include/--only");
}

ساكن صحيح parse_and_validate_options(صحيح argc, ثابت حرف *argv[],
				      ثابت بنية option *options,
				      ثابت حرف * ثابت usage[],
				      ثابت حرف *پیشوند,
				      بنية commit *current_سر,
				      بنية wt_وضعیت *s)
{
	argc = parse_options(argc, argv, پیشوند, options, usage, 0);
	finalize_deferred_config(s);

	إذا (force_author && !strchr(force_author, '>'))
		force_author = پیدا_کن_author_by_nickname(force_author);

	إذا (force_author && renew_authorship)
		die(_("options '%s' and '%s' cannot be used together"), "--reset-author", "--author");

	إذا (logfile || have_option_m || use_پیام)
		use_editor = 0;

	/* Sanity check options */
	إذا (amend && !current_سر)
		die(_("You have nothing to amend."));
	إذا (amend && whence != FROM_COMMIT) {
		إذا (whence == FROM_ادغام)
			die(_("You are in the middle of a merge -- cannot amend."));
		وإلا إذا (is_from_cherry_pick(whence))
			die(_("You are in the middle of a cherry-pick -- cannot amend."));
		وإلا إذا (whence == FROM_REBASE_PICK)
			die(_("You are in the middle of a rebase -- cannot amend."));
	}
	إذا (fixup_پیام && squash_پیام)
		die(_("options '%s' and '%s' cannot be used together"), "--squash", "--fixup");
	die_for_incompatible_opt4(!!use_پیام, "-C",
				  !!edit_پیام, "-c",
				  !!logfile, "-F",
				  !!fixup_پیام, "--fixup");
	die_for_incompatible_opt4(have_option_m, "-m",
				  !!edit_پیام, "-c",
				  !!use_پیام, "-C",
				  !!logfile, "-F");
	إذا (use_پیام || edit_پیام || logfile ||fixup_پیام || have_option_m)
		FREE_AND_NULL(template_file);
	إذا (edit_پیام)
		use_پیام = edit_پیام;
	إذا (amend && !use_پیام && !fixup_پیام)
		use_پیام = "HEAD";
	إذا (!use_پیام && !is_from_cherry_pick(whence) &&
	    !is_from_rebase(whence) && renew_authorship)
		die(_("--reset-author can be used only with -C, -c or --amend."));
	إذا (use_پیام) {
		use_پیام_میانگیر = read_commit_پیام_2(use_پیام);
		إذا (!renew_authorship) {
			author_پیام = use_پیام;
			author_پیام_میانگیر = use_پیام_میانگیر;
		}
	}
	إذا ((is_from_cherry_pick(whence) || whence == FROM_REBASE_PICK) &&
	    !renew_authorship) {
		author_پیام = "CHERRY_PICK_HEAD";
		author_پیام_میانگیر = read_commit_پیام_2(author_پیام);
	}

	إذا (patch_interactive)
		interactive = 1;

	die_for_incompatible_opt4(also, "-i/--include",
				  only, "-o/--only",
				  all, "-a/--all",
				  interactive, "--interactive/-p/--patch");
	إذا (fixup_پیام) {
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
		حرف *p = fixup_پیام;
		طالما (آیا_حرف(*p))
			p++;
		إذا (p > fixup_پیام && *p == ':') {
			*p = '\0';
			fixup_commit = p + 1;
			إذا (!strcmp("amend", fixup_پیام) ||
			    !strcmp("reword", fixup_پیام)) {
				fixup_پیشوند = "amend";
				allow_خالی = 1;
				إذا (*fixup_پیام == 'r') {
					check_fixup_reword_options(argc, argv);
					only = 1;
				}
			} وإلا {
				die(_("unknown option: --fixup=%s:%s"), fixup_پیام, fixup_commit);
			}
		} وإلا {
			fixup_commit = fixup_پیام;
			fixup_پیشوند = "fixup";
			use_editor = 0;
		}
	}

	إذا (0 <= edit_flag)
		use_editor = edit_flag;

	handle_untracked_files_arg(s);

	إذا (all && argc > 0)
		die(_("paths '%s ...' with -a does not make sense"),
		    argv[0]);

	إذا (وضعیت_format != وضعیت_FORMAT_NONE)
		dry_پیاپی = 1;

	أرجع argc;
}

ساكن صحيح dry_پیاپی_commit(ثابت حرف **argv, ثابت حرف *پیشوند,
			  ثابت بنية commit *current_سر, بنية wt_وضعیت *s)
{
	صحيح committable;
	ثابت حرف *شاخص_file;

	شاخص_file = prepare_شاخص(argv, پیشوند, current_سر, 1);
	committable = پیاپی_وضعیت(stdout, شاخص_file, پیشوند, 0, s);
	rollback_شاخص_files();

	أرجع committable ? 0 : 1;
}

define_list_config_array_extra(color_وضعیت_slots, {"added"});

ساكن صحيح parse_وضعیت_جایگاه(ثابت حرف *جایگاه)
{
	إذا (!strcasecmp(جایگاه, "added"))
		أرجع WT_وضعیت_UPDATED;

	أرجع LOOKUP_CONFIG(color_وضعیت_slots, جایگاه);
}

ساكن صحيح git_وضعیت_config(ثابت حرف *k, ثابت حرف *v,
			     ثابت بنية config_context *ctx, فراغ *cb)
{
	بنية wt_وضعیت *s = cb;
	ثابت حرف *جایگاه_name;

	إذا (starts_with(k, "column."))
		أرجع git_ستونی_config(k, v, "status", &s->colopts);
	إذا (!strcmp(k, "status.submodulesummary")) {
		صحيح is_bool;
		s->submodule_summary = git_config_bool_or_int(k, v, ctx->kvi,
							      &is_bool);
		إذا (is_bool && s->submodule_summary)
			s->submodule_summary = -1;
		أرجع 0;
	}
	إذا (!strcmp(k, "status.short")) {
		إذا (git_config_bool(k, v))
			وضعیت_deferred_config.وضعیت_format = وضعیت_FORMAT_SHORT;
		وإلا
			وضعیت_deferred_config.وضعیت_format = وضعیت_FORMAT_NONE;
		أرجع 0;
	}
	إذا (!strcmp(k, "status.branch")) {
		وضعیت_deferred_config.show_branch = git_config_bool(k, v);
		أرجع 0;
	}
	إذا (!strcmp(k, "status.aheadbehind")) {
		وضعیت_deferred_config.ahead_behind = git_config_bool(k, v);
		أرجع 0;
	}
	إذا (!strcmp(k, "status.showstash")) {
		s->show_stash = git_config_bool(k, v);
		أرجع 0;
	}
	إذا (!strcmp(k, "status.color") || !strcmp(k, "color.status")) {
		s->use_color = git_config_colorbool(k, v);
		أرجع 0;
	}
	إذا (!strcmp(k, "status.displaycommentprefix")) {
		s->display_comment_پیشوند = git_config_bool(k, v);
		أرجع 0;
	}
	إذا (skip_پیشوند(k, "status.color.", &جایگاه_name) ||
	    skip_پیشوند(k, "color.status.", &جایگاه_name)) {
		صحيح جایگاه = parse_وضعیت_جایگاه(جایگاه_name);
		إذا (جایگاه < 0)
			أرجع 0;
		إذا (!v)
			أرجع config_error_nonbool(k);
		أرجع color_parse(v, s->color_palette[جایگاه]);
	}
	إذا (!strcmp(k, "status.relativepaths")) {
		s->relative_paths = git_config_bool(k, v);
		أرجع 0;
	}
	إذا (!strcmp(k, "status.showuntrackedfiles")) {
		تعداد untracked_وضعیت_type u;

		u = parse_untracked_setting_name(v);
		إذا (u == SHOW_UNTRACKED_FILES_ERROR)
			أرجع error(_("Invalid untracked files mode '%s'"), v);
		s->show_untracked_files = u;
		أرجع 0;
	}
	إذا (!strcmp(k, "diff.renamelimit")) {
		إذا (s->rename_حد == -1)
			s->rename_حد = git_config_int(k, v, ctx->kvi);
		أرجع 0;
	}
	إذا (!strcmp(k, "status.renamelimit")) {
		s->rename_حد = git_config_int(k, v, ctx->kvi);
		أرجع 0;
	}
	إذا (!strcmp(k, "diff.renames")) {
		إذا (s->detect_rename == -1)
			s->detect_rename = git_config_rename(k, v);
		أرجع 0;
	}
	إذا (!strcmp(k, "status.renames")) {
		s->detect_rename = git_config_rename(k, v);
		أرجع 0;
	}
	أرجع git_diff_ui_config(k, v, ctx, عدم);
}

صحيح cmd_وضعیت(صحيح argc,
ثابت حرف **argv,
ثابت حرف *پیشوند,
بنية repository *repo استفادهنشده)
{
	ساكن صحيح no_renames = -1;
	ساكن ثابت حرف *rename_score_arg = (ثابت حرف *)-1;
	ساكن بنية wt_وضعیت s;
	بیعلامت صحيح پیشرفت_flag = 0;
	صحيح fd;
	بنية object_id oid;
	ساكن بنية option builtin_وضعیت_options[] = {
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT_مجموعه_INT('s', "short", &وضعیت_format,
			    N_("show status concisely"), وضعیت_FORMAT_SHORT),
		OPT_BOOL('b', "branch", &s.show_branch,
			 N_("show branch information")),
		OPT_BOOL(0, "show-stash", &s.show_stash,
			 N_("show stash information")),
		OPT_BOOL(0, "ahead-behind", &s.ahead_behind_flags,
			 N_("compute full ahead/behind values")),
		OPT_CALLBACK_F(0, "porcelain", &وضعیت_format,
		  N_("version"), N_("machine-readable output"),
		  PARSE_OPT_OPTARG, opt_parse_porcelain),
		OPT_مجموعه_INT(0, "long", &وضعیت_format,
			    N_("show status in long format (default)"),
			    وضعیت_FORMAT_LONG),
		OPT_BOOL('z', "null", &s.null_termination,
			 N_("terminate entries with NUL")),
		{
			.type = OPTION_STRING,
			.short_name = 'u',
			.long_name = "untracked-files",
			.مقدار = &untracked_files_arg,
			.argh = N_("mode"),
			.help = N_("show untracked files, optional modes: all, normal, no. (Default: all)"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t)"all",
		},
		{
			.type = OPTION_STRING,
			.long_name = "ignored",
			.مقدار = &ignored_arg,
			.argh = N_("mode"),
			.help = N_("show ignored files, optional modes: traditional, matching, no. (Default: traditional)"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t)"traditional",
		},
		{
			.type = OPTION_STRING,
			.long_name = "ignore-submodules",
			.مقدار = &ignore_submodule_arg,
			.argh = N_("when"),
			.help = N_("ignore changes to submodules, optional when: all, dirty, untracked. (Default: all)"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t)"all",
		},
		OPT_ستونی(0, "column", &s.colopts, N_("list untracked files in columns")),
		OPT_BOOL(0, "no-renames", &no_renames, N_("do not detect renames")),
		OPT_CALLBACK_F('M', "find-renames", &rename_score_arg,
		  N_("n"), N_("detect renames, optionally set similarity index"),
		  PARSE_OPT_OPTARG | PARSE_OPT_NONEG, opt_parse_rename_score),
		OPT_انتها(),
	};

	show_usage_with_options_if_asked(argc, argv,
					 builtin_وضعیت_usage, builtin_وضعیت_options);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_پر_شاخص = 0;

	وضعیت_init_config(&s, git_وضعیت_config);
	argc = parse_options(argc, argv, پیشوند,
			     builtin_وضعیت_options,
			     builtin_وضعیت_usage, 0);
	finalize_colopts(&s.colopts, -1);
	finalize_deferred_config(&s);

	handle_untracked_files_arg(&s);
	handle_ignored_arg(&s);

	إذا (s.show_ignored_نما == SHOW_MATCHING_IGNORED &&
	    s.show_untracked_files == SHOW_NO_UNTRACKED_FILES)
		die(_("Unsupported combination of ignored and untracked-files arguments"));

	parse_pathspec(&s.pathspec, 0,
		       PATHSPEC_PREFER_پر,
		       پیشوند, argv);

	إذا (وضعیت_format != وضعیت_FORMAT_PORCELAIN &&
	    وضعیت_format != وضعیت_FORMAT_PORCELAIN_V2)
		پیشرفت_flag = REFRESH_پیشرفت;
	repo_read_شاخص(the_repository);
	refresh_شاخص(the_repository->شاخص,
		      REFRESH_QUIET|REFRESH_UNMERGED|پیشرفت_flag,
		      &s.pathspec, عدم, عدم);

	إذا (use_optional_locks())
		fd = repo_hold_locked_شاخص(the_repository, &شاخص_قفل, 0);
	وإلا
		fd = -1;

	s.is_initial = repo_get_oid(the_repository, s.reference, &oid) ? 1 : 0;
	إذا (!s.is_initial)
		oidcpy(&s.oid_commit, &oid);

	s.ignore_submodule_arg = ignore_submodule_arg;
	s.وضعیت_format = وضعیت_format;
	s.verbose = verbose;
	إذا (no_renames != -1)
		s.detect_rename = !no_renames;
	إذا ((intptr_t)rename_score_arg != -1) {
		إذا (s.detect_rename < DIFF_DETECT_RENAME)
			s.detect_rename = DIFF_DETECT_RENAME;
		إذا (rename_score_arg)
			s.rename_score = parse_rename_score(&rename_score_arg);
	}

	wt_وضعیت_collect(&s);

	إذا (0 <= fd)
		repo_update_شاخص_if_able(the_repository, &شاخص_قفل);

	إذا (s.relative_paths)
		s.پیشوند = پیشوند;

	wt_وضعیت_print(&s);
	wt_وضعیت_collect_free_buffers(&s);

	أرجع 0;
}

ساكن صحيح git_commit_config(ثابت حرف *k, ثابت حرف *v,
			     ثابت بنية config_context *ctx, فراغ *cb)
{
	بنية wt_وضعیت *s = cb;

	إذا (!strcmp(k, "commit.template"))
		أرجع git_config_pathname(&template_file, k, v);
	إذا (!strcmp(k, "commit.status")) {
		include_وضعیت = git_config_bool(k, v);
		أرجع 0;
	}
	إذا (!strcmp(k, "commit.cleanup")) {
		FREE_AND_NULL(cleanup_config);
		أرجع git_config_string(&cleanup_config, k, v);
	}
	إذا (!strcmp(k, "commit.gpgsign")) {
		sign_commit = git_config_bool(k, v) ? "" : عدم;
		أرجع 0;
	}
	إذا (!strcmp(k, "commit.verbose")) {
		صحيح is_bool;
		config_commit_verbose = git_config_bool_or_int(k, v, ctx->kvi,
							       &is_bool);
		أرجع 0;
	}

	أرجع git_وضعیت_config(k, v, ctx, s);
}

صحيح cmd_commit(صحيح argc,
	       ثابت حرف **argv,
	       ثابت حرف *پیشوند,
	       بنية repository *repo استفادهنشده)
{
	ساكن بنية wt_وضعیت s;
	ساكن ثابت حرف *cleanup_arg = عدم;
	ساكن بنية option builtin_commit_options[] = {
		OPT__QUIET(&quiet, N_("suppress summary after successful commit")),
		OPT__VERBOSE(&verbose, N_("show diff in commit message template")),

		OPT_گروه(N_("Commit message options")),
		OPT_FILENAME('F', "file", &logfile, N_("read message from file")),
		OPT_STRING(0, "author", &force_author, N_("author"), N_("override author for commit")),
		OPT_STRING(0, "date", &force_date, N_("date"), N_("override date for commit")),
		OPT_CALLBACK('m', "message", &پیام_2, N_("message"), N_("commit message"), opt_parse_m),
		OPT_STRING('c', "reedit-message", &edit_پیام, N_("commit"), N_("reuse and edit message from specified commit")),
		OPT_STRING('C', "reuse-message", &use_پیام, N_("commit"), N_("reuse message from specified commit")),
		/*
		 * TRANSLATORS: Leave "[(amend|reword):]" as-is,
		 * and only translate <commit>.
		 */
		OPT_STRING(0, "fixup", &fixup_پیام, N_("[(amend|reword):]commit"), N_("use autosquash formatted message to fixup or amend/reword specified commit")),
		OPT_STRING(0, "squash", &squash_پیام, N_("commit"), N_("use autosquash formatted message to squash specified commit")),
		OPT_BOOL(0, "reset-author", &renew_authorship, N_("the commit is authored by me now (used with -C/-c/--amend)")),
		OPT_STRVEC(0, "trailer", &trailer_آرگومانها, N_("trailer"),
			   N_("add custom trailer(s)")),
		OPT_BOOL('s', "signoff", &signoff, N_("add a Signed-off-by trailer")),
		OPT_FILENAME('t', "template", &template_file, N_("use specified template file")),
		OPT_BOOL('e', "edit", &edit_flag, N_("force edit of commit")),
		OPT_CLEANUP(&cleanup_arg),
		OPT_BOOL(0, "status", &include_وضعیت, N_("include status in commit message template")),
		{
			.type = OPTION_STRING,
			.short_name = 'S',
			.long_name = "gpg-sign",
			.مقدار = &sign_commit,
			.argh = N_("key-id"),
			.help = N_("GPG sign commit"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "",
		},
		/* end commit message options */

		OPT_گروه(N_("Commit contents options")),
		OPT_BOOL('a', "all", &all, N_("commit all changed files")),
		OPT_BOOL('i', "include", &also, N_("add specified files to index for commit")),
		OPT_BOOL(0, "interactive", &interactive, N_("interactively add files")),
		OPT_BOOL('p', "patch", &patch_interactive, N_("interactively add changes")),
		OPT_DIFF_UNIFIED(&interactive_opts.context),
		OPT_DIFF_INTERHUNK_CONTEXT(&interactive_opts.interhunkcontext),
		OPT_BOOL('o', "only", &only, N_("commit only specified files")),
		OPT_BOOL('n', "no-verify", &no_verify, N_("bypass pre-commit and commit-msg hooks")),
		OPT_BOOL(0, "dry-run", &dry_پیاپی, N_("show what would be committed")),
		OPT_مجموعه_INT(0, "short", &وضعیت_format, N_("show status concisely"),
			    وضعیت_FORMAT_SHORT),
		OPT_BOOL(0, "branch", &s.show_branch, N_("show branch information")),
		OPT_BOOL(0, "ahead-behind", &s.ahead_behind_flags,
			 N_("compute full ahead/behind values")),
		OPT_مجموعه_INT(0, "porcelain", &وضعیت_format,
			    N_("machine-readable output"), وضعیت_FORMAT_PORCELAIN),
		OPT_مجموعه_INT(0, "long", &وضعیت_format,
			    N_("show status in long format (default)"),
			    وضعیت_FORMAT_LONG),
		OPT_BOOL('z', "null", &s.null_termination,
			 N_("terminate entries with NUL")),
		OPT_BOOL(0, "amend", &amend, N_("amend previous commit")),
		OPT_BOOL(0, "no-post-rewrite", &no_post_rewrite, N_("bypass post-rewrite hook")),
		{
			.type = OPTION_STRING,
			.short_name = 'u',
			.long_name = "untracked-files",
			.مقدار = &untracked_files_arg,
			.argh = N_("mode"),
			.help = N_("show untracked files, optional modes: all, normal, no. (Default: all)"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t)"all",
		},
		OPT_PATHSPEC_FROM_FILE(&pathspec_from_file),
		OPT_PATHSPEC_FILE_NUL(&pathspec_file_nul),
		/* end commit contents options */

		OPT_HIDDEN_BOOL(0, "allow-empty", &allow_خالی,
				N_("ok to record an empty change")),
		OPT_HIDDEN_BOOL(0, "allow-empty-message", &allow_خالی_پیام,
				N_("ok to record a change with an empty message")),

		OPT_انتها()
	};

	بنية strbuf sb = STRBUF_INIT;
	بنية strbuf author_ident = STRBUF_INIT;
	ثابت حرف *شاخص_file, *reflog_پیام;
	بنية object_id oid;
	بنية commit_list *parents = عدم;
	بنية stat statbuf;
	بنية commit *current_سر = عدم;
	بنية commit_extra_سرآیند *extra = عدم;
	بنية strbuf err = STRBUF_INIT;
	صحيح ret = 0;

	show_usage_with_options_if_asked(argc, argv,
					 builtin_commit_usage, builtin_commit_options);

#اگر_تعریف_نشده WITH_BREAKING_CHANGES
	warn_on_auto_comment_char = true;
#پایان_اگر /* !WITH_BREAKING_CHANGES */
	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_پر_شاخص = 0;

	وضعیت_init_config(&s, git_commit_config);
	s.commit_template = 1;
	وضعیت_format = وضعیت_FORMAT_NONE; /* Ignore status.short */
	s.colopts = 0;

	إذا (repo_get_oid(the_repository, "HEAD", &oid))
		current_سر = عدم;
	وإلا {
		current_سر = lookup_commit_or_die(&oid, "HEAD");
		إذا (repo_parse_commit(the_repository, current_سر))
			die(_("could not parse HEAD commit"));
	}
	verbose = -1; /* unspecified */
	argc = parse_and_validate_options(argc, argv, builtin_commit_options,
					  builtin_commit_usage,
					  پیشوند, current_سر, &s);
	إذا (trailer_آرگومانها.nr)
		trailer_config_init();

	إذا (verbose == -1)
		verbose = (config_commit_verbose < 0) ? 0 : config_commit_verbose;

	إذا (cleanup_arg) {
		حرر(cleanup_config);
		cleanup_config = xstrdup(cleanup_arg);
	}
	cleanup_نما = get_cleanup_نما(cleanup_config, use_editor);

	إذا (dry_پیاپی)
		أرجع dry_پیاپی_commit(argv, پیشوند, current_سر, &s);
	شاخص_file = prepare_شاخص(argv, پیشوند, current_سر, 0);

	/* Set up everything for writing the commit object.  This includes
	   running hooks, writing the trees, and interacting with the user.  */
	إذا (!prepare_مقصد_commit(شاخص_file, پیشوند,
			       current_سر, &s, &author_ident)) {
		ret = 1;
		rollback_شاخص_files();
		اذهب cleanup;
	}

	/* Determine parents */
	reflog_پیام = getenv("GIT_REFLOG_ACTION");
	إذا (!current_سر) {
		إذا (!reflog_پیام)
			reflog_پیام = "commit (initial)";
	} وإلا إذا (amend) {
		إذا (!reflog_پیام)
			reflog_پیام = "commit (amend)";
		parents = commit_list_copy(current_سر->parents);
	} وإلا إذا (whence == FROM_ادغام) {
		بنية strbuf m = STRBUF_INIT;
		FILE *fp;
		صحيح allow_fast_forward = 1;
		بنية commit_list **pptr = &parents;

		إذا (!reflog_پیام)
			reflog_پیام = "commit (merge)";
		pptr = commit_list_append(current_سر, pptr);
		fp = xfopen(git_مسیر_ادغام_سر(the_repository), "r");
		طالما (strbuf_getline_lf(&m, fp) != EOF) {
			بنية commit *والد;

			والد = get_ادغام_والد(m.buf);
			إذا (!والد)
				die(_("Corrupt MERGE_HEAD file (%s)"), m.buf);
			pptr = commit_list_append(والد, pptr);
		}
		fclose(fp);
		strbuf_release(&m);
		إذا (!stat(git_مسیر_ادغام_نما(the_repository), &statbuf)) {
			إذا (strbuf_read_file(&sb, git_مسیر_ادغام_نما(the_repository), 0) < 0)
				die_errno(_("could not read MERGE_MODE"));
			إذا (!strcmp(sb.buf, "no-ff"))
				allow_fast_forward = 0;
		}
		إذا (allow_fast_forward)
			reduce_heads_replace(&parents);
	} وإلا {
		إذا (!reflog_پیام)
			reflog_پیام = is_from_cherry_pick(whence)
					? "commit (cherry-pick)"
					: is_from_rebase(whence)
					? "commit (rebase)"
					: "commit";
		commit_list_درج(current_سر, &parents);
	}

	/* Finally, get the commit message */
	strbuf_reset(&sb);
	إذا (strbuf_read_file(&sb, git_مسیر_commit_editmsg(), 0) < 0) {
		صحيح saved_errno = errno;
		rollback_شاخص_files();
		die(_("could not read commit message: %s"), strerror(saved_errno));
	}

	cleanup_پیام(&sb, cleanup_نما, verbose);

	إذا (پیام_is_خالی(&sb, cleanup_نما) && !allow_خالی_پیام) {
		rollback_شاخص_files();
		fprintf(stderr, _("Aborting commit due to empty commit message.\n"));
		خروج(1);
	}
	إذا (template_untouched(&sb, template_file, cleanup_نما) && !allow_خالی_پیام) {
		rollback_شاخص_files();
		fprintf(stderr, _("Aborting commit; you did not edit the message.\n"));
		خروج(1);
	}

	إذا (fixup_پیام && starts_with(sb.buf, "amend! ") &&
	    !allow_خالی_پیام) {
		بنية strbuf body = STRBUF_INIT;
		اندازه_t len = commit_subject_درازی(sb.buf);
		strbuf_addstr(&body, sb.buf + len);
		إذا (پیام_is_خالی(&body, cleanup_نما)) {
			rollback_شاخص_files();
			fprintf(stderr, _("Aborting commit due to empty commit message body.\n"));
			خروج(1);
		}
		strbuf_release(&body);
	}

	إذا (amend) {
		ثابت حرف *exclude_gpgsig[3] = { "gpgsig", "gpgsig-sha256", عدم };
		extra = read_commit_extra_سرآیندها(current_سر, exclude_gpgsig);
	} وإلا {
		بنية commit_extra_سرآیند **دم = &extra;
		append_ادغام_برچسب_سرآیندها(parents, &دم);
	}

	إذا (commit_درخت_extended(sb.buf, sb.len, &the_repository->شاخص->cache_درخت->oid,
				 parents, &oid, author_ident.buf, عدم,
				 sign_commit, extra)) {
		rollback_شاخص_files();
		die(_("failed to write commit object"));
	}

	إذا (update_سر_with_reflog(current_سر, &oid, reflog_پیام, &sb,
				    &err)) {
		rollback_شاخص_files();
		die("%s", err.buf);
	}

	sequencer_post_commit_cleanup(the_repository, 0);
	unlink(git_مسیر_ادغام_سر(the_repository));
	unlink(git_مسیر_ادغام_پیام(the_repository));
	unlink(git_مسیر_ادغام_نما(the_repository));
	unlink(git_مسیر_squash_پیام(the_repository));

	إذا (commit_شاخص_files())
		die(_("repository has been updated, but unable to write\n"
		      "new index file. Check that disk is not full and quota is\n"
		      "not exceeded, and then \"git restore --staged :/\" to recover."));

	git_test_write_commit_گراف_or_die(the_repository);

	repo_rerere(the_repository, 0);
	پیاپی_auto_maintenance(the_repository, quiet);
	پیاپی_commit_hook(use_editor, repo_get_شاخص_file(the_repository),
			عدم, "post-commit", عدم);
	إذا (amend && !no_post_rewrite) {
		commit_post_rewrite(the_repository, current_سر, &oid);
	}
	إذا (!quiet) {
		بیعلامت صحيح flags = 0;

		إذا (!current_سر)
			flags |= SUMMARY_INITIAL_COMMIT;
		إذا (author_date_is_interesting())
			flags |= SUMMARY_SHOW_AUTHOR_DATE;
		print_commit_summary(the_repository, پیشوند,
				     &oid, flags);
	}

	apply_autostash_ref(the_repository, "MERGE_AUTOSTASH",
			    عدم, عدم, عدم, عدم);

cleanup:
	free_commit_extra_سرآیندها(extra);
	commit_list_free(parents);
	strbuf_release(&author_ident);
	strbuf_release(&err);
	strbuf_release(&sb);
	حرر(logfile);
	حرر(template_file);
	أرجع ret;
}
