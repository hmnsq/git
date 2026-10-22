#تعریف_کرو USE_THE_REPOSITORY_VARIABLE

#شامل "builtin.h"
#شامل "abspath.h"
#شامل "config.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "hash.h"
#شامل "hex.h"
#شامل "object-name.h"
#شامل "parse-options.h"
#شامل "refs.h"
#شامل "stash.h"
#شامل "lockfile.h"
#شامل "cache-tree.h"
#شامل "unpack-trees.h"
#شامل "merge-ort-wrappers.h"
#شامل "strvec.h"
#شامل "run-command.h"
#شامل "dir.h"
#شامل "entry.h"
#شامل "preload-index.h"
#شامل "read-cache.h"
#شامل "repository.h"
#شامل "rerere.h"
#شامل "revision.h"
#شامل "setup.h"
#شامل "sparse-index.h"
#شامل "log-tree.h"
#شامل "diffcore.h"
#شامل "reflog.h"
#شامل "reflog-walk.h"
#شامل "add-interactive.h"
#شامل "commit.h"

#تعریف_کرو INCLUDE_ALL_FILES 2

#تعریف_کرو BUILTIN_STASH_LIST_USAGE \
	N_("git stash list [<log-options>]")
#تعریف_کرو BUILTIN_STASH_SHOW_USAGE \
	N_("git stash show [-u | --include-untracked | --only-untracked] [<diff-options>] [<stash>]")
#تعریف_کرو BUILTIN_STASH_DROP_USAGE \
	N_("git stash drop [-q | --quiet] [<stash>]")
#تعریف_کرو BUILTIN_STASH_نکالو_USAGE \
	N_("git stash pop [--index] [-q | --quiet] [<stash>]")
#تعریف_کرو BUILTIN_STASH_APPLY_USAGE \
	N_("git stash apply [--index] [-q | --quiet] [--label-ours=<label>] [--label-theirs=<label>] [--label-base=<label>] [<stash>]")
#تعریف_کرو BUILTIN_STASH_BRANCH_USAGE \
	N_("git stash branch <branchname> [<stash>]")
#تعریف_کرو BUILTIN_STASH_STORE_USAGE \
	N_("git stash store [(-m | --message) <message>] [-q | --quiet] <commit>")
#تعریف_کرو BUILTIN_STASH_ڈالو_USAGE \
	N_("git stash [push] [-p | --patch] [-S | --staged] [-k | --[no-]keep-index] [-q | --quiet]\n" \
	   "          [-u | --include-untracked] [-a | --all] [(-m | --message) <message>]\n" \
	   "          [--pathspec-from-file=<file> [--pathspec-file-nul]]\n" \
	   "          [--] [<pathspec>...]")
#تعریف_کرو BUILTIN_STASH_SAVE_USAGE \
	N_("git stash save [-p | --patch] [-S | --staged] [-k | --[no-]keep-index] [-q | --quiet]\n" \
	   "          [-u | --include-untracked] [-a | --all] [<message>]")
#تعریف_کرو BUILTIN_STASH_CREATE_USAGE \
	N_("git stash create [<message>]")
#تعریف_کرو BUILTIN_STASH_EXPORT_USAGE \
	N_("git stash export (--print | --to-ref <ref>) [<stash>...]")
#تعریف_کرو BUILTIN_STASH_IMPORT_USAGE \
	N_("git stash import <commit>")
#تعریف_کرو BUILTIN_STASH_CLEAR_USAGE \
	"git stash clear"

ساکن ثابت حرف * ثابت git_stash_usage[] = {
	BUILTIN_STASH_LIST_USAGE,
	BUILTIN_STASH_SHOW_USAGE,
	BUILTIN_STASH_DROP_USAGE,
	BUILTIN_STASH_نکالو_USAGE,
	BUILTIN_STASH_APPLY_USAGE,
	BUILTIN_STASH_BRANCH_USAGE,
	BUILTIN_STASH_ڈالو_USAGE,
	BUILTIN_STASH_SAVE_USAGE,
	BUILTIN_STASH_CLEAR_USAGE,
	BUILTIN_STASH_CREATE_USAGE,
	BUILTIN_STASH_STORE_USAGE,
	BUILTIN_STASH_EXPORT_USAGE,
	BUILTIN_STASH_IMPORT_USAGE,
	تہی
};

ساکن ثابت حرف * ثابت git_stash_list_usage[] = {
	BUILTIN_STASH_LIST_USAGE,
	تہی
};

ساکن ثابت حرف * ثابت git_stash_show_usage[] = {
	BUILTIN_STASH_SHOW_USAGE,
	تہی
};

ساکن ثابت حرف * ثابت git_stash_drop_usage[] = {
	BUILTIN_STASH_DROP_USAGE,
	تہی
};

ساکن ثابت حرف * ثابت git_stash_نکالو_usage[] = {
	BUILTIN_STASH_نکالو_USAGE,
	تہی
};

ساکن ثابت حرف * ثابت git_stash_apply_usage[] = {
	BUILTIN_STASH_APPLY_USAGE,
	تہی
};

ساکن ثابت حرف * ثابت git_stash_branch_usage[] = {
	BUILTIN_STASH_BRANCH_USAGE,
	تہی
};

ساکن ثابت حرف * ثابت git_stash_clear_usage[] = {
	BUILTIN_STASH_CLEAR_USAGE,
	تہی
};

ساکن ثابت حرف * ثابت git_stash_store_usage[] = {
	BUILTIN_STASH_STORE_USAGE,
	تہی
};

ساکن ثابت حرف * ثابت git_stash_ڈالو_usage[] = {
	BUILTIN_STASH_ڈالو_USAGE,
	تہی
};

ساکن ثابت حرف * ثابت git_stash_save_usage[] = {
	BUILTIN_STASH_SAVE_USAGE,
	تہی
};

ساکن ثابت حرف * ثابت git_stash_export_usage[] = {
	BUILTIN_STASH_EXPORT_USAGE,
	تہی
};

ساکن ثابت حرف * ثابت git_stash_import_usage[] = {
	BUILTIN_STASH_IMPORT_USAGE,
	تہی
};

ساکن ثابت حرف ref_stash[] = "refs/stash";
ساکن ساخت strbuf stash_اشاریہ_راستہ = STRBUF_INIT;

ساکن صحیح show_stat = 1;
ساکن صحیح show_patch;
ساکن صحیح show_include_untracked;
ساکن صحیح use_اشاریہ;

/*
 * w_commit is set to the commit containing the working tree
 * b_commit is set to the base commit
 * i_commit is set to the commit containing the index tree
 * u_commit is set to the commit containing the untracked files tree
 * c_commit is set to the first parent (chain commit) when importing and is otherwise unset
 * w_tree is set to the working tree
 * b_tree is set to the base tree
 * i_tree is set to the index tree
 * u_tree is set to the untracked files tree
 */
ساخت stash_info {
	ساخت object_id w_commit;
	ساخت object_id b_commit;
	ساخت object_id i_commit;
	ساخت object_id u_commit;
	ساخت object_id c_commit;
	ساخت object_id w_درخت;
	ساخت object_id b_درخت;
	ساخت object_id i_درخت;
	ساخت object_id u_درخت;
	ساخت strbuf revision;
	صحیح is_stash_ref;
	صحیح has_u;
};

#تعریف_کرو STASH_INFO_INIT { \
	.revision = STRBUF_INIT, \
}

ساکن خالی free_stash_info(ساخت stash_info *info)
{
	strbuf_release(&info->revision);
}

ساکن صحیح check_stash_topology(ساخت repository *r, ساخت commit *stash)
{
	ساخت commit *p1, *p2, *p3 = تہی;

	/* stash must have two or three parents */
	اگر (!stash->parents || !stash->parents->اگلا ||
			(stash->parents->اگلا->اگلا && stash->parents->اگلا->اگلا->اگلا))
		واپس -1;
	p1 = stash->parents->شے;
	p2 = stash->parents->اگلا->شے;
	اگر (stash->parents->اگلا->اگلا)
		p3 = stash->parents->اگلا->اگلا->شے;
	اگر (repo_parse_commit(r, p1) || repo_parse_commit(r, p2) ||
			(p3 && repo_parse_commit(r, p3)))
		واپس -1;
	/* p2 must have a single parent, p3 must have no parents */
	اگر (!p2->parents || p2->parents->اگلا || (p3 && p3->parents))
		واپس -1;
	اگر (repo_parse_commit(r, p2->parents->شے))
		واپس -1;
	/* p2^1 must equal p1 */
	اگر (!oideq(&p1->object.oid, &p2->parents->شے->object.oid))
		واپس -1;

	واپس 0;
}

ساکن خالی assert_stash_like(ساخت stash_info *info, ثابت حرف *revision)
{
	اگر (get_oidf(&info->b_commit, "%s^1", revision) ||
	    get_oidf(&info->w_درخت, "%s:", revision) ||
	    get_oidf(&info->b_درخت, "%s^1:", revision) ||
	    get_oidf(&info->i_درخت, "%s^2:", revision))
		die(_("'%s' is not a stash-like commit"), revision);
}

ساکن صحیح parse_stash_revision(ساخت strbuf *revision, ثابت حرف *commit, صحیح quiet)
{
	strbuf_reset(revision);
	اگر (!commit) {
		اگر (!refs_ref_exists(get_main_ref_store(the_repository), ref_stash)) {
			اگر (!quiet)
				fprintf_ln(stderr, _("No stash entries found."));
			واپس -1;
		}

		strbuf_addf(revision, "%s@{0}", ref_stash);
	} ورنہ اگر (strspn(commit, "0123456789") == لڑی_لمبائی(commit)) {
		strbuf_addf(revision, "%s@{%s}", ref_stash, commit);
	} ورنہ {
		strbuf_addstr(revision, commit);
	}
	واپس 0;
}

ساکن صحیح get_stash_info(ساخت stash_info *info, صحیح argc, ثابت حرف **argv)
{
	صحیح ret;
	حرف *انتہا_کا_rev;
	حرف *expanded_ref;
	ثابت حرف *revision;
	ثابت حرف *commit = تہی;
	ساخت object_id dummy;
	ساخت strbuf symbolic = STRBUF_INIT;

	اگر (argc > 1) {
		صحیح i;
		ساخت strbuf refs_پیغام = STRBUF_INIT;

		برائے (i = 0; i < argc; i++)
			strbuf_addf(&refs_پیغام, " '%s'", argv[i]);

		fprintf_ln(stderr, _("Too many revisions specified:%s"),
			   refs_پیغام.buf);
		strbuf_release(&refs_پیغام);

		واپس -1;
	}

	اگر (argc == 1)
		commit = argv[0];

	strbuf_init(&info->revision, 0);
	اگر (parse_stash_revision(&info->revision, commit, 0)) {
		واپس -1;
	}

	revision = info->revision.buf;

	اگر (repo_get_oid(the_repository, revision, &info->w_commit))
		واپس error(_("%s is not a valid reference"), revision);

	assert_stash_like(info, revision);

	info->has_u = !get_oidf(&info->u_درخت, "%s^3:", revision);

	انتہا_کا_rev = strchrnul(revision, '@');
	strbuf_جوڑو(&symbolic, revision, انتہا_کا_rev - revision);

	ret = repo_dwim_ref(the_repository, symbolic.buf, symbolic.len,
			    &dummy, &expanded_ref, 0);
	strbuf_release(&symbolic);
	انتخاب (ret) {
	حالت 0: /* Not found, but valid ref */
		info->is_stash_ref = 0;
		توڑو;
	حالت 1:
		info->is_stash_ref = !strcmp(expanded_ref, ref_stash);
		توڑو;
	طےشدہ: /* Invalid or ambiguous */
		توڑو;
	}

	آزاد(expanded_ref);
	واپس !(ret == 0 || ret == 1);
}

ساکن صحیح do_clear_stash(خالی)
{
	ساخت object_id obj;
	اگر (repo_get_oid(the_repository, ref_stash, &obj))
		واپس 0;

	واپس refs_delete_ref(get_main_ref_store(the_repository), تہی,
			       ref_stash, &obj, 0);
}

ساکن صحیح clear_stash(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
		       ساخت repository *repo غیر_مستعمل)
{
	ساخت option options[] = {
		OPT_انتہا()
	};

	argc = parse_options(argc, argv, سابقہ, options,
			     git_stash_clear_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	اگر (argc)
		واپس error(_("git stash clear with arguments is "
			       "unimplemented"));

	واپس do_clear_stash();
}

ساکن صحیح reset_درخت(ساخت object_id *i_درخت, صحیح update, صحیح reset)
{
	صحیح nr_trees = 1;
	ساخت unpack_trees_options opts;
	ساخت درخت_desc t[MAX_UNPACK_TREES];
	ساخت درخت *درخت;
	ساخت تالا_file تالا_file = تالا_INIT;

	repo_read_اشاریہ_preload(the_repository, تہی, 0);
	اگر (refresh_اشاریہ(the_repository->اشاریہ, REFRESH_QUIET, تہی, تہی, تہی))
		واپس -1;

	repo_hold_locked_اشاریہ(the_repository, &تالا_file, تالا_DIE_ON_ERROR);

	memset(&opts, 0, حجم(opts));

	درخت = repo_parse_درخت_بالواسطہ(the_repository, i_درخت);
	اگر (repo_parse_درخت(the_repository, درخت))
		واپس -1;

	init_درخت_desc(t, &درخت->object.oid, درخت->بفر, درخت->سائز);

	opts.سر_idx = 1;
	opts.src_اشاریہ = the_repository->اشاریہ;
	opts.dst_اشاریہ = the_repository->اشاریہ;
	opts.ملاؤ = 1;
	opts.reset = reset ? UNPACK_RESET_PROTECT_UNTRACKED : 0;
	opts.update = update;
	اگر (update)
		opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
	opts.fn = oneway_ملاؤ;

	اگر (unpack_trees(nr_trees, t, &opts))
		واپس -1;

	اگر (write_locked_اشاریہ(the_repository->اشاریہ, &تالا_file, COMMIT_تالا))
		واپس error(_("unable to write new index file"));

	واپس 0;
}

ساکن صحیح create_اشاریہ_from_درخت(ثابت ساخت object_id *درخت_id,
				  ثابت حرف *اشاریہ_راستہ)
{
	صحیح nr_trees = 1;
	صحیح ret = 0;
	ساخت unpack_trees_options opts;
	ساخت درخت_desc t[MAX_UNPACK_TREES];
	ساخت درخت *درخت;
	ساخت اشاریہ_state dst_istate = اشاریہ_STATE_INIT(the_repository);
	ساخت تالا_file تالا_file = تالا_INIT;

	repo_read_اشاریہ_preload(the_repository, تہی, 0);
	refresh_اشاریہ(the_repository->اشاریہ, REFRESH_QUIET, تہی, تہی, تہی);

	hold_تالا_file_for_update(&تالا_file, اشاریہ_راستہ, تالا_DIE_ON_ERROR);

	memset(&opts, 0, حجم(opts));

	درخت = repo_parse_درخت_بالواسطہ(the_repository, درخت_id);
	اگر (!درخت || repo_parse_درخت(the_repository, درخت)) {
		ret = -1;
		جاؤ مکمل;
	}

	init_درخت_desc(t, &درخت->object.oid, درخت->بفر, درخت->سائز);

	opts.سر_idx = 1;
	opts.src_اشاریہ = the_repository->اشاریہ;
	opts.dst_اشاریہ = &dst_istate;
	opts.ملاؤ = 1;
	opts.reset = UNPACK_RESET_PROTECT_UNTRACKED;
	opts.fn = oneway_ملاؤ;

	اگر (unpack_trees(nr_trees, t, &opts)) {
		ret = -1;
		جاؤ مکمل;
	}

	اگر (write_locked_اشاریہ(&dst_istate, &تالا_file, COMMIT_تالا)) {
		ret = error(_("unable to write new index file"));
		جاؤ مکمل;
	}

مکمل:
	release_اشاریہ(&dst_istate);
	اگر (ret)
		rollback_تالا_file(&تالا_file);
	واپس ret;
}

ساکن صحیح is_راستہ_a_directory(ثابت حرف *راستہ)
{
	/*
	 * This function differs from abspath.c:is_directory() in that
	 * here we use lstat() instead of stat(); we do not want to
	 * follow symbolic links here.
	 */
	ساخت stat st;
	واپس (!lstat(راستہ, &st) && S_ISDIR(st.st_منوال));
}

ساکن خالی جوڑو_diff_مقصد_buf(ساخت diff_قطار_struct *q,
			    ساخت diff_options *options غیر_مستعمل,
			    خالی *ڈیٹا)
{
	صحیح i;

	برائے (i = 0; i < q->nr; i++) {
		اگر (is_راستہ_a_directory(q->قطار[i]->one->راستہ))
			جاری;

		strbuf_addstr(ڈیٹا, q->قطار[i]->one->راستہ);

		/* NUL-terminate: will be fed to update-index -z */
		strbuf_addch(ڈیٹا, '\0');
	}
}

ساکن صحیح restore_untracked(ساخت object_id *u_درخت)
{
	صحیح res;
	ساخت بچہ_process cp = بچہ_PROCESS_INIT;

	/*
	 * We need to run restore files from a given index, but without
	 * affecting the current index, so we use GIT_INDEX_FILE with
	 * run_command to fork processes that will not interfere.
	 */
	cp.git_cmd = 1;
	strvec_ڈالو(&cp.دلائل, "read-tree");
	strvec_ڈالو(&cp.دلائل, oid_مقصد_hex(u_درخت));
	strvec_pushf(&cp.env, "GIT_INDEX_FILE=%s",
		     stash_اشاریہ_راستہ.buf);
	اگر (تسلسل_command(&cp)) {
		remove_راستہ(stash_اشاریہ_راستہ.buf);
		واپس -1;
	}

	بچہ_process_init(&cp);
	cp.git_cmd = 1;
	strvec_pushl(&cp.دلائل, "checkout-index", "--all", تہی);
	strvec_pushf(&cp.env, "GIT_INDEX_FILE=%s",
		     stash_اشاریہ_راستہ.buf);

	res = تسلسل_command(&cp);
	remove_راستہ(stash_اشاریہ_راستہ.buf);
	واپس res;
}

ساکن خالی unstage_changes_unless_new(ساخت object_id *orig_درخت)
{
	/*
	 * When we enter this function, there has been a clean merge of
	 * relevant trees, and the merge logic always stages whatever merges
	 * cleanly.  We want to unstage those changes, unless it corresponds
	 * to a file that didn't exist as of orig_tree.
	 *
	 * However, if any SKIP_WORKTREE path is modified relative to
	 * orig_tree, then we want to clear the SKIP_WORKTREE bit and write
	 * it to the worktree before unstaging.
	 */

	ساخت checkout state = CHECKOUT_INIT;
	ساخت diff_options diff_opts;
	ساخت تالا_file تالا = تالا_INIT;
	صحیح i;

	/* If any entries have skip_worktree set, we'll have to check 'em out */
	state.force = 1;
	state.quiet = 1;
	state.refresh_cache = 1;
	state.istate = the_repository->اشاریہ;

	/*
	 * Step 1: get a difference between orig_tree (which corresponding
	 * to the index before a merge was run) and the current index
	 * (reflecting the changes brought in by the merge).
	 */
	repo_diff_تیاری(the_repository, &diff_opts);
	diff_opts.flags.recursive = 1;
	diff_opts.detect_rename = 0;
	diff_opts.output_format = DIFF_FORMAT_NO_OUTPUT;
	diff_تیاری_مکمل(&diff_opts);

	do_diff_cache(orig_درخت, &diff_opts);
	diffcore_std(&diff_opts);

	/* Iterate over the paths that changed due to the merge... */
	برائے (i = 0; i < diff_queued_diff.nr; i++) {
		ساخت diff_filepair *p;
		ساخت cache_اندراج *ce;
		صحیح pos;

		/* Look up the path's position in the current index. */
		p = diff_queued_diff.قطار[i];
		pos = اشاریہ_name_pos(the_repository->اشاریہ, p->two->راستہ,
				     لڑی_لمبائی(p->two->راستہ));

		/*
		 * Step 2: Place changes in the working tree
		 *
		 * Stash is about restoring changes *to the working tree*.
		 * So if the merge successfully got a new version of some
		 * path, but left it out of the working tree, then clear the
		 * SKIP_WORKTREE bit and write it to the working tree.
		 */
		اگر (pos >= 0 && ce_skip_worktree(the_repository->اشاریہ->cache[pos])) {
			ساخت stat st;

			ce = the_repository->اشاریہ->cache[pos];
			اگر (!lstat(ce->name, &st)) {
				/* Conflicting path present; relocate it */
				ساخت strbuf new_راستہ = STRBUF_INIT;
				صحیح fd;

				strbuf_addf(&new_راستہ,
					    "%s.stash.XXXXXX", ce->name);
				fd = xmkstemp(new_راستہ.buf);
				close(fd);
				لکھو(_("WARNING: Untracked file in way of "
					 "tracked file!  Renaming\n "
					 "           %s -> %s\n"
					 "         to make room.\n"),
				       ce->name, new_راستہ.buf);
				اگر (rename(ce->name, new_راستہ.buf))
					die("Failed to move %s to %s",
					    ce->name, new_راستہ.buf);
				strbuf_release(&new_راستہ);
			}
			checkout_اندراج(ce, &state, تہی, تہی);
			ce->ce_flags &= ~CE_SKIP_WORKTREE;
		}

		/*
		 * Step 3: "unstage" changes, as long as they are still tracked
		 */
		اگر (p->one->oid_valid) {
			/*
			 * Path existed in orig_tree; restore index entry
			 * from that tree in order to "unstage" the changes.
			 */
			صحیح option = جوڑو_CACHE_OK_مقصد_REPLACE;
			اگر (pos < 0)
				option = جوڑو_CACHE_OK_مقصد_جوڑو;

			ce = make_cache_اندراج(the_repository->اشاریہ,
					      p->one->منوال,
					      &p->one->oid,
					      p->one->راستہ,
					      0, 0);
			جوڑو_اشاریہ_اندراج(the_repository->اشاریہ, ce, option);
		}
	}
	diff_flush(&diff_opts);

	/*
	 * Step 4: write the new index to disk
	 */
	repo_hold_locked_اشاریہ(the_repository, &تالا, تالا_DIE_ON_ERROR);
	اگر (write_locked_اشاریہ(the_repository->اشاریہ, &تالا,
			       COMMIT_تالا | SKIP_IF_UNCHANGED))
		die(_("could not write index"));
}

ساکن شمار stash_apply_نتیجہ do_apply_stash(ثابت حرف *سابقہ,
					      ساخت stash_info *info,
					      صحیح اشاریہ, صحیح quiet,
					      ثابت حرف *label_ours,
					      ثابت حرف *label_theirs,
					      ثابت حرف *label_بنیاد)
{
	صحیح clean, ret;
	صحیح has_اشاریہ = اشاریہ;
	ساخت ملاؤ_options o;
	ساخت object_id c_درخت;
	ساخت object_id اشاریہ_درخت;
	ساخت درخت *سر, *ملاؤ, *ملاؤ_بنیاد;
	ساخت تالا_file تالا = تالا_INIT;

	repo_read_اشاریہ_preload(the_repository, تہی, 0);
	اگر (repo_refresh_and_write_اشاریہ(the_repository, REFRESH_QUIET, 0, 0,
					 تہی, تہی, تہی))
		واپس error(_("could not write index"));

	اگر (write_اشاریہ_as_درخت(&c_درخت, the_repository->اشاریہ,
				repo_get_اشاریہ_file(the_repository), 0, تہی))
		واپس error(_("cannot apply a stash in the middle of a merge"));

	init_ui_ملاؤ_options(&o, the_repository);

	اگر (quiet)
		o.verbosity = 0;

	اگر (اشاریہ) {
		اگر (oideq(&info->b_درخت, &info->i_درخت) ||
		    oideq(&c_درخت, &info->i_درخت)) {
			has_اشاریہ = 0;
		} ورنہ {
			ساخت ملاؤ_نتیجہ نتیجہ = { 0 };

			o.branch1 = "Current index";
			o.branch2 = "Stashed index changes";
			o.ancestor = "Stash base";

			سر = lookup_درخت(o.repo, &c_درخت);
			ملاؤ = lookup_درخت(o.repo, &info->i_درخت);
			ملاؤ_بنیاد = lookup_درخت(o.repo, &info->b_درخت);

			ملاؤ_incore_nonrecursive(&o, ملاؤ_بنیاد, سر, ملاؤ,
						  &نتیجہ);

			اگر (نتیجہ.clean < 0) {
				ملاؤ_finalize(&o, &نتیجہ);
				واپس error(_("index merge failed"));
			} ورنہ اگر (!نتیجہ.clean) {
				ملاؤ_finalize(&o, &نتیجہ);
				واپس error(_("conflicts in index. "
					       "Try without --index."));
			} ورنہ {
				oidcpy(&اشاریہ_درخت, &نتیجہ.درخت->object.oid);
				ملاؤ_finalize(&o, &نتیجہ);
			}
		}
	}

	o.branch1 = label_ours ? label_ours : "Updated upstream";
	o.branch2 = label_theirs ? label_theirs : "Stashed changes";
	o.ancestor = label_بنیاد ? label_بنیاد : "Stash base";

	اگر (oideq(&info->b_درخت, &c_درخت))
		o.branch1 = "Version stash was based on";

	اگر (o.verbosity >= 3)
		printf_ln(_("Merging %s with %s"), o.branch1, o.branch2);

	سر = lookup_درخت(o.repo, &c_درخت);
	ملاؤ = lookup_درخت(o.repo, &info->w_درخت);
	ملاؤ_بنیاد = lookup_درخت(o.repo, &info->b_درخت);

	repo_hold_locked_اشاریہ(o.repo, &تالا, تالا_DIE_ON_ERROR);
	clean = ملاؤ_ort_nonrecursive(&o, سر, ملاؤ, ملاؤ_بنیاد);

	/*
	 * If 'clean' >= 0, reverse the value for 'ret' so 'ret' is 0 when the
	 * merge was clean, and 1 if the merge was unclean or a negative value
	 * if it encountered an error.
	 */
	ret = clean >= 0 ? !clean : clean;

	اگر (ret < 0)
		rollback_تالا_file(&تالا);
	ورنہ اگر (write_locked_اشاریہ(o.repo->اشاریہ, &تالا,
				      COMMIT_تالا | SKIP_IF_UNCHANGED))
		ret = error(_("could not write index"));

	اگر (ret) {
		repo_rerere(the_repository, 0);

		اگر (اشاریہ)
			fprintf_ln(stderr, _("Index was not unstashed."));

		جاؤ restore_untracked;
	}

	اگر (has_اشاریہ) {
		اگر (reset_درخت(&اشاریہ_درخت, 0, 0))
			ret = -1;
	} ورنہ {
		unstage_changes_unless_new(&c_درخت);
	}

restore_untracked:
	اگر (info->has_u && restore_untracked(&info->u_درخت))
		ret = error(_("could not restore untracked files from stash"));

	اگر (!quiet) {
		ساخت بچہ_process cp = بچہ_PROCESS_INIT;

		/*
		 * Status is quite simple and could be replaced with calls to
		 * wt_status in the future, but it adds complexities which may
		 * require more tests.
		 */
		cp.git_cmd = 1;
		cp.dir = سابقہ;
		strvec_pushf(&cp.env, GIT_WORK_درخت_ENVIRONMENT"=%s",
			     absolute_راستہ(repo_get_work_درخت(the_repository)));
		strvec_pushf(&cp.env, GIT_DIR_ENVIRONMENT"=%s",
			     absolute_راستہ(repo_get_git_dir(the_repository)));
		strvec_ڈالو(&cp.دلائل, "status");
		تسلسل_command(&cp);
	}

	واپس ret;
}

ساکن صحیح apply_stash(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
		       ساخت repository *repo غیر_مستعمل)
{
	صحیح ret = -1;
	صحیح quiet = 0;
	صحیح اشاریہ = use_اشاریہ;
	ثابت حرف *label_ours = تہی, *label_theirs = تہی, *label_بنیاد = تہی;
	ساخت stash_info info = STASH_INFO_INIT;
	ساخت option options[] = {
		OPT__QUIET(&quiet, N_("be quiet, only report errors")),
		OPT_BOOL(0, "index", &اشاریہ,
			 N_("attempt to recreate the index")),
		OPT_STRING(0, "label-ours", &label_ours, N_("label"),
			   N_("label for the upstream side in conflict markers")),
		OPT_STRING(0, "label-theirs", &label_theirs, N_("label"),
			   N_("label for the stashed side in conflict markers")),
		OPT_STRING(0, "label-base", &label_بنیاد, N_("label"),
			   N_("label for the base in diff3 conflict markers")),
		OPT_انتہا()
	};

	argc = parse_options(argc, argv, سابقہ, options,
			     git_stash_apply_usage, 0);

	اگر (get_stash_info(&info, argc, argv))
		جاؤ cleanup;

	ret = do_apply_stash(سابقہ, &info, اشاریہ, quiet,
			     label_ours, label_theirs, label_بنیاد);
cleanup:
	free_stash_info(&info);
	واپس ret;
}

ساکن صحیح reject_reflog_ent(ثابت حرف *refname غیر_مستعمل,
			     ساخت object_id *ooid غیر_مستعمل,
			     ساخت object_id *noid غیر_مستعمل,
			     ثابت حرف *email غیر_مستعمل,
			     timestamp_t timestamp غیر_مستعمل,
			     صحیح tz غیر_مستعمل, ثابت حرف *پیغام_2 غیر_مستعمل,
			     خالی *cb_ڈیٹا غیر_مستعمل)
{
	واپس 1;
}

ساکن صحیح reflog_is_خالی_خانے(ثابت حرف *refname)
{
	واپس !refs_for_each_reflog_ent(get_main_ref_store(the_repository),
					 refname, reject_reflog_ent, تہی);
}

ساکن صحیح do_drop_stash(ساخت stash_info *info, صحیح quiet)
{
	اگر (!reflog_delete(info->revision.buf,
			   EXPIRE_REFLOGS_REWRITE | EXPIRE_REFLOGS_UPDATE_REF,
			   0)) {
		اگر (!quiet)
			printf_ln(_("Dropped %s (%s)"), info->revision.buf,
				  oid_مقصد_hex(&info->w_commit));
	} ورنہ {
		واپس error(_("%s: Could not drop stash entry"),
			     info->revision.buf);
	}

	اگر (reflog_is_خالی_خانے(ref_stash))
		do_clear_stash();

	واپس 0;
}

ساکن صحیح get_stash_info_assert(ساخت stash_info *info, صحیح argc,
				 ثابت حرف **argv)
{
	صحیح ret = get_stash_info(info, argc, argv);

	اگر (ret < 0)
		واپس ret;

	اگر (!info->is_stash_ref)
		واپس error(_("'%s' is not a stash reference"), info->revision.buf);

	واپس 0;
}

ساکن صحیح drop_stash(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
		      ساخت repository *repo غیر_مستعمل)
{
	صحیح ret = -1;
	صحیح quiet = 0;
	ساخت stash_info info = STASH_INFO_INIT;
	ساخت option options[] = {
		OPT__QUIET(&quiet, N_("be quiet, only report errors")),
		OPT_انتہا()
	};

	argc = parse_options(argc, argv, سابقہ, options,
			     git_stash_drop_usage, 0);

	اگر (get_stash_info_assert(&info, argc, argv))
		جاؤ cleanup;

	ret = do_drop_stash(&info, quiet);
cleanup:
	free_stash_info(&info);
	واپس ret;
}

ساکن صحیح نکالو_stash(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
		     ساخت repository *repo غیر_مستعمل)
{
	صحیح ret = -1;
	صحیح اشاریہ = use_اشاریہ;
	صحیح quiet = 0;
	ساخت stash_info info = STASH_INFO_INIT;
	ساخت option options[] = {
		OPT__QUIET(&quiet, N_("be quiet, only report errors")),
		OPT_BOOL(0, "index", &اشاریہ,
			 N_("attempt to recreate the index")),
		OPT_انتہا()
	};

	argc = parse_options(argc, argv, سابقہ, options,
			     git_stash_نکالو_usage, 0);

	اگر (get_stash_info_assert(&info, argc, argv))
		جاؤ cleanup;

	اگر ((ret = do_apply_stash(سابقہ, &info, اشاریہ, quiet,
				  تہی, تہی, تہی)))
		printf_ln(_("The stash entry is kept in case "
			    "you need it again."));
	ورنہ
		ret = do_drop_stash(&info, quiet);

cleanup:
	free_stash_info(&info);
	واپس ret;
}

ساکن صحیح branch_stash(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
			ساخت repository *repo غیر_مستعمل)
{
	صحیح ret = -1;
	ثابت حرف *branch = تہی;
	ساخت stash_info info = STASH_INFO_INIT;
	ساخت بچہ_process cp = بچہ_PROCESS_INIT;
	ساخت option options[] = {
		OPT_انتہا()
	};

	argc = parse_options(argc, argv, سابقہ, options,
			     git_stash_branch_usage, 0);

	اگر (!argc) {
		fprintf_ln(stderr, _("No branch name specified"));
		واپس -1;
	}

	branch = argv[0];

	اگر (get_stash_info(&info, argc - 1, argv + 1))
		جاؤ cleanup;

	cp.git_cmd = 1;
	strvec_pushl(&cp.دلائل, "checkout", "-b", تہی);
	strvec_ڈالو(&cp.دلائل, branch);
	strvec_ڈالو(&cp.دلائل, oid_مقصد_hex(&info.b_commit));
	ret = تسلسل_command(&cp);
	اگر (!ret)
		ret = do_apply_stash(سابقہ, &info, 1, 0,
				     تہی, تہی, تہی);
	اگر (!ret && info.is_stash_ref)
		ret = do_drop_stash(&info, 0);

cleanup:
	free_stash_info(&info);
	واپس ret;
}

ساکن صحیح list_stash(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
		      ساخت repository *repo غیر_مستعمل)
{
	ساخت بچہ_process cp = بچہ_PROCESS_INIT;
	ساخت option options[] = {
		OPT_انتہا()
	};

	argc = parse_options(argc, argv, سابقہ, options,
			     git_stash_list_usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT);

	اگر (!refs_ref_exists(get_main_ref_store(the_repository), ref_stash))
		واپس 0;

	cp.git_cmd = 1;
	strvec_pushl(&cp.دلائل, "log", "--format=%gd: %gs", "-g",
		     "--first-parent", تہی);
	strvec_pushv(&cp.دلائل, argv);
	strvec_ڈالو(&cp.دلائل, ref_stash);
	strvec_ڈالو(&cp.دلائل, "--");
	واپس تسلسل_command(&cp);
}

ساکن صحیح git_stash_config(ثابت حرف *var, ثابت حرف *قدر,
			    ثابت ساخت config_context *ctx, خالی *cb)
{
	اگر (!strcmp(var, "stash.showstat")) {
		show_stat = git_config_bool(var, قدر);
		واپس 0;
	}
	اگر (!strcmp(var, "stash.showpatch")) {
		show_patch = git_config_bool(var, قدر);
		واپس 0;
	}
	اگر (!strcmp(var, "stash.showincludeuntracked")) {
		show_include_untracked = git_config_bool(var, قدر);
		واپس 0;
	}
	اگر (!strcmp(var, "stash.index")) {
		use_اشاریہ = git_config_bool(var, قدر);
		واپس 0;
	}
	واپس git_diff_basic_config(var, قدر, ctx, cb);
}

ساکن خالی diff_include_untracked(ثابت ساخت stash_info *info, ساخت diff_options *diff_opt)
{
	ثابت ساخت object_id *oid[] = { &info->w_commit, &info->u_درخت };
	ساخت درخت *درخت[ARRAY_سائز(oid)];
	ساخت درخت_desc درخت_desc[ARRAY_سائز(oid)];
	ساخت unpack_trees_options unpack_درخت_opt = { 0 };

	برائے (سائز_t i = 0; i < ARRAY_سائز(oid); i++) {
		درخت[i] = repo_parse_درخت_بالواسطہ(the_repository, oid[i]);
		اگر (repo_parse_درخت(the_repository, درخت[i]) < 0)
			die(_("failed to parse tree"));
		init_درخت_desc(&درخت_desc[i], &درخت[i]->object.oid,
			       درخت[i]->بفر, درخت[i]->سائز);
	}

	unpack_درخت_opt.سر_idx = -1;
	unpack_درخت_opt.src_اشاریہ = the_repository->اشاریہ;
	unpack_درخت_opt.dst_اشاریہ = the_repository->اشاریہ;
	unpack_درخت_opt.ملاؤ = 1;
	unpack_درخت_opt.fn = stash_worktree_untracked_ملاؤ;

	اگر (unpack_trees(ARRAY_سائز(درخت_desc), درخت_desc, &unpack_درخت_opt))
		die(_("failed to unpack trees"));

	do_diff_cache(&info->b_commit, diff_opt);
}

ساکن صحیح show_stash(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
		      ساخت repository *repo غیر_مستعمل)
{
	صحیح i;
	صحیح ret = -1;
	ساخت stash_info info = STASH_INFO_INIT;
	ساخت rev_info rev;
	ساخت strvec stash_دلائل = STRVEC_INIT;
	ساخت strvec revision_دلائل = STRVEC_INIT;
	شمار {
		UNTRACKED_NONE,
		UNTRACKED_INCLUDE,
		UNTRACKED_ONLY
	} show_untracked = show_include_untracked ? UNTRACKED_INCLUDE : UNTRACKED_NONE;
	ساخت option options[] = {
		OPT_سیٹ_INT('u', "include-untracked", &show_untracked,
			    N_("include untracked files in the stash"),
			    UNTRACKED_INCLUDE),
		OPT_سیٹ_INT_F(0, "only-untracked", &show_untracked,
			      N_("only show untracked files in the stash"),
			      UNTRACKED_ONLY, PARSE_OPT_NONEG),
		OPT_انتہا()
	};
	صحیح do_usage = 0;

	init_diff_ui_defaults();
	repo_config(the_repository, git_diff_ui_config, تہی);
	repo_init_revisions(the_repository, &rev, سابقہ);

	argc = parse_options(argc, argv, سابقہ, options, git_stash_show_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	strvec_ڈالو(&revision_دلائل, argv[0]);
	برائے (i = 1; i < argc; i++) {
		اگر (argv[i][0] != '-')
			strvec_ڈالو(&stash_دلائل, argv[i]);
		ورنہ
			strvec_ڈالو(&revision_دلائل, argv[i]);
	}

	اگر (get_stash_info(&info, stash_دلائل.nr, stash_دلائل.v))
		جاؤ cleanup;

	/*
	 * The config settings are applied only if there are not passed
	 * any options.
	 */
	اگر (revision_دلائل.nr == 1) {
		اگر (show_stat)
			rev.diffopt.output_format = DIFF_FORMAT_DIFFSTAT;

		اگر (show_patch)
			rev.diffopt.output_format |= DIFF_FORMAT_PATCH;

		اگر (!show_stat && !show_patch) {
			ret = 0;
			جاؤ cleanup;
		}
	}

	تیاری_revisions_from_strvec(&revision_دلائل, &rev, تہی);
	اگر (revision_دلائل.nr > 1)
		جاؤ usage;
	اگر (!rev.diffopt.output_format) {
		rev.diffopt.output_format = DIFF_FORMAT_PATCH;
		diff_تیاری_مکمل(&rev.diffopt);
	}

	rev.diffopt.flags.recursive = 1;
	تیاری_diff_pager(&rev.diffopt);
	انتخاب (show_untracked) {
	حالت UNTRACKED_NONE:
		diff_درخت_oid(&info.b_commit, &info.w_commit, "", &rev.diffopt);
		توڑو;
	حالت UNTRACKED_ONLY:
		اگر (info.has_u)
			diff_جڑ_درخت_oid(&info.u_درخت, "", &rev.diffopt);
		توڑو;
	حالت UNTRACKED_INCLUDE:
		اگر (info.has_u)
			diff_include_untracked(&info, &rev.diffopt);
		ورنہ
			diff_درخت_oid(&info.b_commit, &info.w_commit, "", &rev.diffopt);
		توڑو;
	}
	log_درخت_diff_flush(&rev);

	ret = diff_نتیجہ_رمز(&rev);

cleanup:
	strvec_clear(&revision_دلائل);
	strvec_clear(&stash_دلائل);
	free_stash_info(&info);
	release_revisions(&rev);
	اگر (do_usage)
		usage_with_options(git_stash_show_usage, options);
	واپس ret;
usage:
	do_usage = 1;
	جاؤ cleanup;
}

ساکن صحیح do_store_stash(ثابت ساخت object_id *w_commit, ثابت حرف *stash_پیغام,
			  صحیح quiet)
{
	ساخت stash_info info;
	حرف revision[GIT_MAX_HEXSZ];

	oid_مقصد_hex_r(revision, w_commit);
	assert_stash_like(&info, revision);

	اگر (!stash_پیغام)
		stash_پیغام = "Created via \"git stash store\".";

	اگر (refs_update_ref(get_main_ref_store(the_repository), stash_پیغام, ref_stash, w_commit, تہی,
			    REF_FORCE_CREATE_REFLOG,
			    quiet ? UPDATE_REFS_QUIET_ON_ERR :
			    UPDATE_REFS_پیغام_ON_ERR)) {
		اگر (!quiet) {
			fprintf_ln(stderr, _("Cannot update %s with %s"),
				   ref_stash, oid_مقصد_hex(w_commit));
		}
		واپس -1;
	}

	واپس 0;
}

ساکن صحیح store_stash(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
		       ساخت repository *repo غیر_مستعمل)
{
	صحیح quiet = 0;
	ثابت حرف *stash_پیغام = تہی;
	ساخت object_id obj;
	ساخت option options[] = {
		OPT__QUIET(&quiet, N_("be quiet")),
		OPT_STRING('m', "message", &stash_پیغام, "message",
			   N_("stash message")),
		OPT_انتہا()
	};
	صحیح ret;

	argc = parse_options(argc, argv, سابقہ, options,
			     git_stash_store_usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT);

	اگر (argc != 1) {
		اگر (!quiet)
			fprintf_ln(stderr, _("\"git stash store\" requires one "
					     "<commit> argument"));
		واپس -1;
	}

	اگر (repo_get_oid_with_flags(the_repository, argv[0], &obj,
				    quiet ? GET_OID_QUIETLY : 0)) {
		اگر (!quiet)
			fprintf_ln(stderr, _("Cannot update %s with %s"),
					     ref_stash, argv[0]);
		ret = -1;
		جاؤ برآمد;
	}

	ret = do_store_stash(&obj, stash_پیغام, quiet);

برآمد:
	واپس ret;
}

ساکن خالی جوڑو_pathspecs(ساخت strvec *دلائل,
			  ثابت ساخت pathspec *ps) {
	صحیح i;

	برائے (i = 0; i < ps->nr; i++)
		strvec_ڈالو(دلائل, ps->اشیاء[i].original);
}

/*
 * `untracked_files` will be filled with the names of untracked files.
 * The return value is:
 *
 * = 0 if there are not any untracked files
 * > 0 if there are untracked files
 */
ساکن صحیح get_untracked_files(ثابت ساخت pathspec *ps, صحیح include_untracked,
			       ساخت strbuf *untracked_files)
{
	صحیح i;
	صحیح ملا_2 = 0;
	ساخت dir_struct dir = DIR_INIT;

	اگر (include_untracked != INCLUDE_ALL_FILES)
		تیاری_standard_excludes(&dir);

	fill_directory(&dir, the_repository->اشاریہ, ps);
	برائے (i = 0; i < dir.nr; i++) {
		ساخت dir_اندراج *ent = dir.entries[i];
		ملا_2++;
		strbuf_addstr(untracked_files, ent->name);
		/* NUL-terminate: will be fed to update-index -z */
		strbuf_addch(untracked_files, '\0');
	}

	dir_clear(&dir);
	واپس ملا_2;
}

/*
 * The return value of `check_changes_tracked_files()` can be:
 *
 * < 0 if there was an error
 * = 0 if there are no changes.
 * > 0 if there are changes.
 */
ساکن صحیح check_changes_tracked_files(ثابت ساخت pathspec *ps)
{
	ساخت rev_info rev;
	ساخت object_id dummy;
	صحیح ret = 0;

	/* No initial commit. */
	اگر (repo_get_oid(the_repository, "HEAD", &dummy))
		واپس -1;

	اگر (repo_read_اشاریہ(the_repository) < 0)
		واپس -1;

	repo_init_revisions(the_repository, &rev, تہی);
	copy_pathspec(&rev.prune_ڈیٹا, ps);

	rev.diffopt.flags.تیز = 1;
	rev.diffopt.flags.ignore_submodules = 1;
	rev.abbrev = 0;

	جوڑو_سر_مقصد_pending(&rev);
	diff_تیاری_مکمل(&rev.diffopt);

	تسلسل_diff_اشاریہ(&rev, DIFF_اشاریہ_CACHED);
	اگر (diff_نتیجہ_رمز(&rev)) {
		ret = 1;
		جاؤ مکمل;
	}

	تسلسل_diff_files(&rev, 0);
	اگر (diff_نتیجہ_رمز(&rev)) {
		ret = 1;
		جاؤ مکمل;
	}

مکمل:
	release_revisions(&rev);
	واپس ret;
}

/*
 * The function will fill `untracked_files` with the names of untracked files
 * It will return 1 if there were any changes and 0 if there were not.
 */
ساکن صحیح check_changes(ثابت ساخت pathspec *ps, صحیح include_untracked,
			 ساخت strbuf *untracked_files)
{
	صحیح ret = 0;
	اگر (check_changes_tracked_files(ps))
		ret = 1;

	اگر (include_untracked && get_untracked_files(ps, include_untracked,
						     untracked_files))
		ret = 1;

	واپس ret;
}

ساکن صحیح save_untracked_files(ساخت stash_info *info, ساخت strbuf *پیغام,
				ساخت strbuf *files)
{
	صحیح ret = 0;
	ساخت strbuf untracked_پیغام = STRBUF_INIT;
	ساخت بچہ_process cp_upd_اشاریہ = بچہ_PROCESS_INIT;
	ساخت اشاریہ_state istate = اشاریہ_STATE_INIT(the_repository);

	cp_upd_اشاریہ.git_cmd = 1;
	strvec_pushl(&cp_upd_اشاریہ.دلائل, "update-index", "-z", "--add",
		     "--remove", "--stdin", تہی);
	strvec_pushf(&cp_upd_اشاریہ.env, "GIT_INDEX_FILE=%s",
			 stash_اشاریہ_راستہ.buf);

	strbuf_addf(&untracked_پیغام, "untracked files on %s\n", پیغام->buf);
	اگر (pipe_command(&cp_upd_اشاریہ, files->buf, files->len, تہی, 0,
			 تہی, 0)) {
		ret = -1;
		جاؤ مکمل;
	}

	اگر (write_اشاریہ_as_درخت(&info->u_درخت, &istate, stash_اشاریہ_راستہ.buf, 0,
				تہی)) {
		ret = -1;
		جاؤ مکمل;
	}

	اگر (commit_درخت(untracked_پیغام.buf, untracked_پیغام.len,
			&info->u_درخت, تہی, &info->u_commit, تہی, تہی)) {
		ret = -1;
		جاؤ مکمل;
	}

مکمل:
	release_اشاریہ(&istate);
	strbuf_release(&untracked_پیغام);
	remove_راستہ(stash_اشاریہ_راستہ.buf);
	واپس ret;
}

ساکن صحیح stash_staged(ساخت stash_info *info, ساخت strbuf *برآمد_patch,
			صحیح quiet)
{
	صحیح ret = 0;
	ساخت بچہ_process cp_diff_درخت = بچہ_PROCESS_INIT;
	ساخت اشاریہ_state istate = اشاریہ_STATE_INIT(the_repository);

	اگر (write_اشاریہ_as_درخت(&info->w_درخت, &istate, the_repository->اشاریہ_file,
				0, تہی)) {
		ret = -1;
		جاؤ مکمل;
	}

	cp_diff_درخت.git_cmd = 1;
	strvec_pushl(&cp_diff_درخت.دلائل, "diff-tree", "-p", "--binary",
		     "--no-color",
		     "-U1", "HEAD", oid_مقصد_hex(&info->w_درخت), "--", تہی);
	اگر (pipe_command(&cp_diff_درخت, تہی, 0, برآمد_patch, 0, تہی, 0)) {
		ret = -1;
		جاؤ مکمل;
	}

	اگر (!برآمد_patch->len) {
		اگر (!quiet)
			fprintf_ln(stderr, _("No staged changes"));
		ret = 1;
	}

مکمل:
	release_اشاریہ(&istate);
	واپس ret;
}

ساکن صحیح stash_patch(ساخت stash_info *info, ثابت ساخت pathspec *ps,
		       ساخت strbuf *برآمد_patch, صحیح quiet,
		       ساخت interactive_options *interactive_opts)
{
	صحیح ret = 0;
	ساخت بچہ_process cp_diff_درخت = بچہ_PROCESS_INIT;
	ساخت commit *سر_commit;
	ثابت ساخت object_id *سر_درخت;
	ساخت اشاریہ_state istate = اشاریہ_STATE_INIT(the_repository);
	حرف *old_اشاریہ_env = تہی, *old_repo_اشاریہ_file;

	remove_راستہ(stash_اشاریہ_راستہ.buf);

	سر_commit = lookup_commit(the_repository, &info->b_commit);
	اگر (!سر_commit || repo_parse_commit(the_repository, سر_commit)) {
		ret = -1;
		جاؤ مکمل;
	}
	سر_درخت = get_commit_درخت_oid(سر_commit);
	اگر (!سر_درخت) {
		ret = -1;
		جاؤ مکمل;
	}

	اگر (create_اشاریہ_from_درخت(سر_درخت, stash_اشاریہ_راستہ.buf)) {
		ret = -1;
		جاؤ مکمل;
	}

	/* Find out what the user wants. */
	old_repo_اشاریہ_file = the_repository->اشاریہ_file;
	the_repository->اشاریہ_file = stash_اشاریہ_راستہ.buf;
	old_اشاریہ_env = xstrdup_or_null(getenv(اشاریہ_ENVIRONMENT));
	setenv(اشاریہ_ENVIRONMENT, the_repository->اشاریہ_file, 1);

	ret = !!تسلسل_جوڑو_p(the_repository, جوڑو_P_STASH, interactive_opts, تہی, ps, 0);

	the_repository->اشاریہ_file = old_repo_اشاریہ_file;
	اگر (old_اشاریہ_env && *old_اشاریہ_env)
		setenv(اشاریہ_ENVIRONMENT, old_اشاریہ_env, 1);
	ورنہ
		unsetenv(اشاریہ_ENVIRONMENT);
	FREE_AND_NULL(old_اشاریہ_env);

	/* State of the working tree. */
	اگر (write_اشاریہ_as_درخت(&info->w_درخت, &istate, stash_اشاریہ_راستہ.buf, 0,
				تہی)) {
		ret = -1;
		جاؤ مکمل;
	}

	cp_diff_درخت.git_cmd = 1;
	strvec_pushl(&cp_diff_درخت.دلائل, "diff-tree", "-p", "-U1", "HEAD",
		     "--no-color",
		     oid_مقصد_hex(&info->w_درخت), "--", تہی);
	اگر (pipe_command(&cp_diff_درخت, تہی, 0, برآمد_patch, 0, تہی, 0)) {
		ret = -1;
		جاؤ مکمل;
	}

	اگر (!برآمد_patch->len) {
		اگر (!quiet)
			fprintf_ln(stderr, _("No changes selected"));
		ret = 1;
	}

مکمل:
	release_اشاریہ(&istate);
	remove_راستہ(stash_اشاریہ_راستہ.buf);
	واپس ret;
}

ساکن صحیح stash_working_درخت(ساخت stash_info *info, ثابت ساخت pathspec *ps)
{
	صحیح ret = 0;
	ساخت rev_info rev;
	ساخت بچہ_process cp_upd_اشاریہ = بچہ_PROCESS_INIT;
	ساخت strbuf diff_output = STRBUF_INIT;
	ساخت اشاریہ_state istate = اشاریہ_STATE_INIT(the_repository);

	repo_init_revisions(the_repository, &rev, تہی);
	copy_pathspec(&rev.prune_ڈیٹا, ps);

	سیٹ_alternate_اشاریہ_output(stash_اشاریہ_راستہ.buf);
	اگر (reset_درخت(&info->i_درخت, 0, 0)) {
		ret = -1;
		جاؤ مکمل;
	}
	سیٹ_alternate_اشاریہ_output(تہی);

	rev.diffopt.output_format = DIFF_FORMAT_CALLBACK;
	rev.diffopt.format_callback = جوڑو_diff_مقصد_buf;
	rev.diffopt.format_callback_ڈیٹا = &diff_output;

	اگر (repo_read_اشاریہ_preload(the_repository, &rev.diffopt.pathspec, 0) < 0) {
		ret = -1;
		جاؤ مکمل;
	}

	جوڑو_pending_object(&rev, parse_object(the_repository, &info->b_commit),
			   "");
	تسلسل_diff_اشاریہ(&rev, 0);

	cp_upd_اشاریہ.git_cmd = 1;
	strvec_pushl(&cp_upd_اشاریہ.دلائل, "update-index",
		     "--ignore-skip-worktree-entries",
		     "-z", "--add", "--remove", "--stdin", تہی);
	strvec_pushf(&cp_upd_اشاریہ.env, "GIT_INDEX_FILE=%s",
		     stash_اشاریہ_راستہ.buf);

	اگر (pipe_command(&cp_upd_اشاریہ, diff_output.buf, diff_output.len,
			 تہی, 0, تہی, 0)) {
		ret = -1;
		جاؤ مکمل;
	}

	اگر (write_اشاریہ_as_درخت(&info->w_درخت, &istate, stash_اشاریہ_راستہ.buf, 0,
				تہی)) {
		ret = -1;
		جاؤ مکمل;
	}

مکمل:
	release_اشاریہ(&istate);
	release_revisions(&rev);
	strbuf_release(&diff_output);
	remove_راستہ(stash_اشاریہ_راستہ.buf);
	واپس ret;
}

ساکن صحیح do_create_stash(ثابت ساخت pathspec *ps, ساخت strbuf *stash_پیغام_buf,
			   صحیح include_untracked, صحیح patch_منوال,
			   ساخت interactive_options *interactive_opts,
			   صحیح only_staged, ساخت stash_info *info, ساخت strbuf *patch,
			   صحیح quiet)
{
	صحیح ret = 0;
	صحیح flags = 0;
	صحیح untracked_commit_option = 0;
	ثابت حرف *سر_short_sha1 = تہی;
	ثابت حرف *branch_ref = تہی;
	ثابت حرف *branch_name = "(no branch)";
	حرف *branch_name_buf = تہی;
	ساخت commit *سر_commit = تہی;
	ساخت commit_list *parents = تہی;
	ساخت strbuf پیغام = STRBUF_INIT;
	ساخت strbuf commit_درخت_label = STRBUF_INIT;
	ساخت strbuf untracked_files = STRBUF_INIT;

	prepare_fallback_ident("git stash", "git@stash");

	repo_read_اشاریہ_preload(the_repository, تہی, 0);
	اگر (repo_refresh_and_write_اشاریہ(the_repository, REFRESH_QUIET, 0, 0,
					 تہی, تہی, تہی) < 0) {
		ret = error(_("could not write index"));
		جاؤ مکمل;
	}

	اگر (repo_get_oid(the_repository, "HEAD", &info->b_commit)) {
		اگر (!quiet)
			fprintf_ln(stderr, _("You do not have "
					     "the initial commit yet"));
		ret = -1;
		جاؤ مکمل;
	} ورنہ {
		سر_commit = lookup_commit(the_repository, &info->b_commit);
	}

	اگر (!check_changes(ps, include_untracked, &untracked_files)) {
		ret = 1;
		جاؤ مکمل;
	}

	branch_ref = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
					     "HEAD", 0, تہی, &flags);

	اگر (flags & REF_ISSYMREF) {
		اگر (skip_سابقہ(branch_ref, "refs/heads/", &branch_name))
			branch_name = branch_name_buf = xstrdup(branch_name);
	}

	سر_short_sha1 = repo_ڈھونڈو_unique_abbrev(the_repository,
						  &سر_commit->object.oid,
						  DEFAULT_ABBREV);
	strbuf_addf(&پیغام, "%s: %s ", branch_name, سر_short_sha1);
	pp_commit_easy(CMIT_FMT_ONELINE, سر_commit, &پیغام);

	strbuf_addf(&commit_درخت_label, "index on %s\n", پیغام.buf);
	commit_list_داخل_کرو(سر_commit, &parents);
	اگر (write_اشاریہ_as_درخت(&info->i_درخت, the_repository->اشاریہ,
				repo_get_اشاریہ_file(the_repository), 0, تہی) ||
	    commit_درخت(commit_درخت_label.buf, commit_درخت_label.len,
			&info->i_درخت, parents, &info->i_commit, تہی, تہی)) {
		اگر (!quiet)
			fprintf_ln(stderr, _("Cannot save the current "
					     "index state"));
		ret = -1;
		جاؤ مکمل;
	}

	commit_list_free(parents);
	parents = تہی;

	اگر (include_untracked) {
		اگر (save_untracked_files(info, &پیغام, &untracked_files)) {
			اگر (!quiet)
				fprintf_ln(stderr, _("Cannot save "
						     "the untracked files"));
			ret = -1;
			جاؤ مکمل;
		}
		untracked_commit_option = 1;
	}
	اگر (patch_منوال) {
		ret = stash_patch(info, ps, patch, quiet, interactive_opts);
		اگر (ret < 0) {
			اگر (!quiet)
				fprintf_ln(stderr, _("Cannot save the current "
						     "worktree state"));
			جاؤ مکمل;
		} ورنہ اگر (ret > 0) {
			جاؤ مکمل;
		}
	} ورنہ اگر (only_staged) {
		ret = stash_staged(info, patch, quiet);
		اگر (ret < 0) {
			اگر (!quiet)
				fprintf_ln(stderr, _("Cannot save the current "
						     "staged state"));
			جاؤ مکمل;
		} ورنہ اگر (ret > 0) {
			جاؤ مکمل;
		}
	} ورنہ {
		اگر (stash_working_درخت(info, ps)) {
			اگر (!quiet)
				fprintf_ln(stderr, _("Cannot save the current "
						     "worktree state"));
			ret = -1;
			جاؤ مکمل;
		}
	}

	اگر (!stash_پیغام_buf->len)
		strbuf_addf(stash_پیغام_buf, "WIP on %s", پیغام.buf);
	ورنہ
		strbuf_insertf(stash_پیغام_buf, 0, "On %s: ", branch_name);

	اگر (untracked_commit_option)
		commit_list_داخل_کرو(lookup_commit(the_repository,
						 &info->u_commit),
				   &parents);
	commit_list_داخل_کرو(lookup_commit(the_repository, &info->i_commit),
			   &parents);
	commit_list_داخل_کرو(سر_commit, &parents);

	اگر (commit_درخت(stash_پیغام_buf->buf, stash_پیغام_buf->len, &info->w_درخت,
			parents, &info->w_commit, تہی, تہی)) {
		اگر (!quiet)
			fprintf_ln(stderr, _("Cannot record "
					     "working tree state"));
		ret = -1;
		جاؤ مکمل;
	}

مکمل:
	strbuf_release(&commit_درخت_label);
	strbuf_release(&پیغام);
	strbuf_release(&untracked_files);
	commit_list_free(parents);
	آزاد(branch_name_buf);
	واپس ret;
}

ساکن صحیح create_stash(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ غیر_مستعمل,
			ساخت repository *repo غیر_مستعمل)
{
	صحیح ret;
	ساخت strbuf stash_پیغام_buf = STRBUF_INIT;
	ساخت stash_info info = STASH_INFO_INIT;
	ساخت pathspec ps;

	/* Starting with argv[1], since argv[0] is "create" */
	strbuf_join_argv(&stash_پیغام_buf, argc - 1, ++argv, ' ');

	memset(&ps, 0, حجم(ps));
	اگر (!check_changes_tracked_files(&ps))
		واپس 0;

	ret = do_create_stash(&ps, &stash_پیغام_buf, 0, 0, تہی, 0, &info,
			      تہی, 0);
	اگر (!ret)
		printf_ln("%s", oid_مقصد_hex(&info.w_commit));

	free_stash_info(&info);
	strbuf_release(&stash_پیغام_buf);
	واپس ret;
}

ساکن صحیح do_ڈالو_stash(ثابت ساخت pathspec *ps, ثابت حرف *stash_پیغام, صحیح quiet,
			 صحیح keep_اشاریہ, صحیح patch_منوال,
			 ساخت interactive_options *interactive_opts,
			 صحیح include_untracked, صحیح only_staged)
{
	صحیح ret = 0;
	ساخت stash_info info = STASH_INFO_INIT;
	ساخت strbuf patch = STRBUF_INIT;
	ساخت strbuf stash_پیغام_buf = STRBUF_INIT;
	ساخت strbuf untracked_files = STRBUF_INIT;
	ساخت strbuf برآمد = STRBUF_INIT;

	اگر (patch_منوال && keep_اشاریہ == -1)
		keep_اشاریہ = 1;

	اگر (patch_منوال && include_untracked) {
		fprintf_ln(stderr, _("Can't use --patch and --include-untracked"
				     " or --all at the same time"));
		ret = -1;
		جاؤ مکمل;
	}

	/* --patch overrides --staged */
	اگر (patch_منوال)
		only_staged = 0;

	اگر (only_staged && include_untracked) {
		fprintf_ln(stderr, _("Can't use --staged and --include-untracked"
				     " or --all at the same time"));
		ret = -1;
		جاؤ مکمل;
	}

	repo_read_اشاریہ_preload(the_repository, تہی, 0);
	اگر (!include_untracked && ps->nr) {
		حرف *ps_matched = xcalloc(ps->nr, 1);

		اگر (pathspec_needs_expanded_اشاریہ(the_repository->اشاریہ, ps))
			ensure_بھرا_اشاریہ(the_repository->اشاریہ);
		برائے (سائز_t i = 0; i < the_repository->اشاریہ->cache_nr; i++)
			ce_راستہ_مطابقت(the_repository->اشاریہ, the_repository->اشاریہ->cache[i], ps,
				      ps_matched);

		اگر (report_راستہ_error(ps_matched, ps)) {
			fprintf_ln(stderr, _("Did you forget to 'git add'?"));
			ret = -1;
			آزاد(ps_matched);
			جاؤ مکمل;
		}
		آزاد(ps_matched);
	}

	اگر (repo_refresh_and_write_اشاریہ(the_repository, REFRESH_QUIET, 0, 0,
					 تہی, تہی, تہی)) {
		ret = error(_("could not write index"));
		جاؤ مکمل;
	}

	اگر (!check_changes(ps, include_untracked, &untracked_files)) {
		اگر (!quiet)
			printf_ln(_("No local changes to save"));
		جاؤ مکمل;
	}

	اگر (!refs_reflog_exists(get_main_ref_store(the_repository), ref_stash) && do_clear_stash()) {
		ret = -1;
		اگر (!quiet)
			fprintf_ln(stderr, _("Cannot initialize stash"));
		جاؤ مکمل;
	}

	اگر (stash_پیغام)
		strbuf_addstr(&stash_پیغام_buf, stash_پیغام);
	اگر (do_create_stash(ps, &stash_پیغام_buf, include_untracked, patch_منوال,
			    interactive_opts, only_staged, &info, &patch, quiet)) {
		ret = -1;
		جاؤ مکمل;
	}

	اگر (do_store_stash(&info.w_commit, stash_پیغام_buf.buf, 1)) {
		ret = -1;
		اگر (!quiet)
			fprintf_ln(stderr, _("Cannot save the current status"));
		جاؤ مکمل;
	}

	اگر (!quiet)
		printf_ln(_("Saved working directory and index state %s"),
			  stash_پیغام_buf.buf);

	اگر (!(patch_منوال || only_staged)) {
		اگر (include_untracked && !ps->nr) {
			ساخت بچہ_process cp = بچہ_PROCESS_INIT;

			cp.git_cmd = 1;
			اگر (startup_info->original_cwd) {
				cp.dir = startup_info->original_cwd;
				strvec_pushf(&cp.env, "%s=%s",
					     GIT_WORK_درخت_ENVIRONMENT,
					     the_repository->worktree);
			}
			strvec_pushl(&cp.دلائل, "clean", "--force",
				     "--quiet", "-d", ":/", تہی);
			اگر (include_untracked == INCLUDE_ALL_FILES)
				strvec_ڈالو(&cp.دلائل, "-x");
			اگر (تسلسل_command(&cp)) {
				ret = -1;
				جاؤ مکمل;
			}
		}
		discard_اشاریہ(the_repository->اشاریہ);
		اگر (ps->nr) {
			ساخت بچہ_process cp_جوڑو = بچہ_PROCESS_INIT;
			ساخت بچہ_process cp_diff = بچہ_PROCESS_INIT;
			ساخت بچہ_process cp_apply = بچہ_PROCESS_INIT;

			cp_جوڑو.git_cmd = 1;
			strvec_ڈالو(&cp_جوڑو.دلائل, "add");
			اگر (!include_untracked)
				strvec_ڈالو(&cp_جوڑو.دلائل, "-u");
			اگر (include_untracked == INCLUDE_ALL_FILES)
				strvec_ڈالو(&cp_جوڑو.دلائل, "--force");
			strvec_ڈالو(&cp_جوڑو.دلائل, "--");
			جوڑو_pathspecs(&cp_جوڑو.دلائل, ps);
			اگر (تسلسل_command(&cp_جوڑو)) {
				ret = -1;
				جاؤ مکمل;
			}

			cp_diff.git_cmd = 1;
			strvec_pushl(&cp_diff.دلائل, "diff-index", "-p",
				     "--no-color",
				     "--cached", "--binary", "HEAD", "--",
				     تہی);
			جوڑو_pathspecs(&cp_diff.دلائل, ps);
			اگر (pipe_command(&cp_diff, تہی, 0, &برآمد, 0, تہی, 0)) {
				ret = -1;
				جاؤ مکمل;
			}

			cp_apply.git_cmd = 1;
			strvec_pushl(&cp_apply.دلائل, "apply", "--index",
				     "-R", تہی);
			اگر (pipe_command(&cp_apply, برآمد.buf, برآمد.len, تہی, 0,
					 تہی, 0)) {
				ret = -1;
				جاؤ مکمل;
			}
		} ورنہ {
			ساخت بچہ_process cp = بچہ_PROCESS_INIT;
			cp.git_cmd = 1;
			/* BUG: this nukes untracked files in the way */
			strvec_pushl(&cp.دلائل, "reset", "--hard", "-q",
				     "--no-recurse-submodules", تہی);
			اگر (تسلسل_command(&cp)) {
				ret = -1;
				جاؤ مکمل;
			}
		}

		/*
		 * When keeping staged entries, we need to reset the working
		 * directory to match the state of our index. This can be
		 * skipped when the index is the empty tree, because there is
		 * nothing to reset in that case:
		 *
		 *   - When the index has any file, regardless of whether
		 *     staged or not, the tree cannot be empty by definition
		 *     and thus we enter the condition.
		 *
		 *   - When the index has no files, the only thing we need to
		 *     care about is untracked files when `--include-untracked`
		 *     is given. But as we already execute git-clean(1) further
		 *     up to delete such untracked files we don't have to do
		 *     anything here, either.
		 *
		 * We thus skip calling git-checkout(1) in this case, also
		 * because running it on an empty tree will cause it to fail
		 * due to the pathspec not matching anything.
		 */
		اگر (keep_اشاریہ == 1 && !is_null_oid(&info.i_درخت) &&
		    !is_خالی_خانے_درخت_oid(&info.i_درخت, the_repository->hash_algo)) {
			ساخت بچہ_process cp = بچہ_PROCESS_INIT;

			cp.git_cmd = 1;
			strvec_pushl(&cp.دلائل, "checkout", "--no-overlay",
				     oid_مقصد_hex(&info.i_درخت), "--", تہی);
			اگر (!ps->nr)
				strvec_ڈالو(&cp.دلائل, ":/");
			ورنہ
				جوڑو_pathspecs(&cp.دلائل, ps);
			اگر (تسلسل_command(&cp)) {
				ret = -1;
				جاؤ مکمل;
			}
		}
		جاؤ مکمل;
	} ورنہ {
		ساخت بچہ_process cp = بچہ_PROCESS_INIT;

		cp.git_cmd = 1;
		strvec_pushl(&cp.دلائل, "apply", "-R", تہی);

		اگر (pipe_command(&cp, patch.buf, patch.len, تہی, 0, تہی, 0)) {
			اگر (!quiet)
				fprintf_ln(stderr, _("Cannot remove "
						     "worktree changes"));
			ret = -1;
			جاؤ مکمل;
		}

		اگر (keep_اشاریہ < 1) {
			ساخت بچہ_process cp = بچہ_PROCESS_INIT;

			cp.git_cmd = 1;
			strvec_pushl(&cp.دلائل, "reset", "-q", "--refresh", "--",
				     تہی);
			جوڑو_pathspecs(&cp.دلائل, ps);
			اگر (تسلسل_command(&cp)) {
				ret = -1;
				جاؤ مکمل;
			}
		}
		جاؤ مکمل;
	}

مکمل:
	strbuf_release(&patch);
	strbuf_release(&برآمد);
	free_stash_info(&info);
	strbuf_release(&stash_پیغام_buf);
	strbuf_release(&untracked_files);
	واپس ret;
}

ساکن صحیح ڈالو_stash(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
		      صحیح ڈالو_assumed)
{
	صحیح force_assume = 0;
	صحیح keep_اشاریہ = -1;
	صحیح only_staged = 0;
	صحیح patch_منوال = 0;
	صحیح include_untracked = 0;
	صحیح quiet = 0;
	صحیح pathspec_file_nul = 0;
	ثابت حرف *stash_پیغام = تہی;
	حرف *pathspec_from_file = تہی;
	ساخت pathspec ps;
	ساخت interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	ساخت option options[] = {
		OPT_BOOL('k', "keep-index", &keep_اشاریہ,
			 N_("keep index")),
		OPT_BOOL('S', "staged", &only_staged,
			 N_("stash staged changes only")),
		OPT_BOOL('p', "patch", &patch_منوال,
			 N_("stash in patch mode")),
		OPT_BOOL(0, "auto-advance", &interactive_opts.auto_advance,
			 N_("auto advance to the next file when selecting hunks interactively")),
		OPT_DIFF_UNIFIED(&interactive_opts.context),
		OPT_DIFF_INTERHUNK_CONTEXT(&interactive_opts.interhunkcontext),
		OPT__QUIET(&quiet, N_("quiet mode")),
		OPT_BOOL('u', "include-untracked", &include_untracked,
			 N_("include untracked files in stash")),
		OPT_سیٹ_INT('a', "all", &include_untracked,
			    N_("include ignore files"), INCLUDE_ALL_FILES),
		OPT_STRING('m', "message", &stash_پیغام, N_("message"),
			   N_("stash message")),
		OPT_PATHSPEC_FROM_FILE(&pathspec_from_file),
		OPT_PATHSPEC_FILE_NUL(&pathspec_file_nul),
		OPT_انتہا()
	};
	صحیح ret;

	اگر (argc) {
		صحیح flags = PARSE_OPT_KEEP_DASHDASH;

		اگر (ڈالو_assumed)
			flags |= PARSE_OPT_STOP_AT_NON_OPTION;

		argc = parse_options(argc, argv, سابقہ, options,
				     ڈالو_assumed ? git_stash_usage :
				     git_stash_ڈالو_usage, flags);
		force_assume |= patch_منوال;
	}

	اگر (argc) {
		اگر (!strcmp(argv[0], "--")) {
			argc--;
			argv++;
		} ورنہ اگر (ڈالو_assumed && !force_assume) {
			die("subcommand wasn't specified; 'push' can't be assumed due to unexpected token '%s'",
			    argv[0]);
		}
	}

	parse_pathspec(&ps, 0, PATHSPEC_PREFER_بھرا | PATHSPEC_سابقہ_ORIGIN,
		       سابقہ, argv);

	اگر (pathspec_from_file) {
		اگر (patch_منوال)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--patch");

		اگر (only_staged)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--staged");

		اگر (ps.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&ps, 0,
				    PATHSPEC_PREFER_بھرا | PATHSPEC_سابقہ_ORIGIN,
				    سابقہ, pathspec_from_file, pathspec_file_nul);
	} ورنہ اگر (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	اگر (!patch_منوال) {
		اگر (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--patch");
		اگر (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--patch");
		اگر (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--patch");
	}

	اگر (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	اگر (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	ret = do_ڈالو_stash(&ps, stash_پیغام, quiet, keep_اشاریہ, patch_منوال,
			    &interactive_opts, include_untracked, only_staged);

	clear_pathspec(&ps);
	آزاد(pathspec_from_file);
	واپس ret;
}

ساکن صحیح ڈالو_stash_unassumed(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
				ساخت repository *repo غیر_مستعمل)
{
	واپس ڈالو_stash(argc, argv, سابقہ, 0);
}

ساکن صحیح save_stash(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
		      ساخت repository *repo غیر_مستعمل)
{
	صحیح keep_اشاریہ = -1;
	صحیح only_staged = 0;
	صحیح patch_منوال = 0;
	صحیح include_untracked = 0;
	صحیح quiet = 0;
	صحیح ret = 0;
	ثابت حرف *stash_پیغام = تہی;
	ساخت pathspec ps;
	ساخت strbuf stash_پیغام_buf = STRBUF_INIT;
	ساخت interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	ساخت option options[] = {
		OPT_BOOL('k', "keep-index", &keep_اشاریہ,
			 N_("keep index")),
		OPT_BOOL('S', "staged", &only_staged,
			 N_("stash staged changes only")),
		OPT_BOOL('p', "patch", &patch_منوال,
			 N_("stash in patch mode")),
		OPT_BOOL(0, "auto-advance", &interactive_opts.auto_advance,
			 N_("auto advance to the next file when selecting hunks interactively")),
		OPT_DIFF_UNIFIED(&interactive_opts.context),
		OPT_DIFF_INTERHUNK_CONTEXT(&interactive_opts.interhunkcontext),
		OPT__QUIET(&quiet, N_("quiet mode")),
		OPT_BOOL('u', "include-untracked", &include_untracked,
			 N_("include untracked files in stash")),
		OPT_سیٹ_INT('a', "all", &include_untracked,
			    N_("include ignore files"), INCLUDE_ALL_FILES),
		OPT_STRING('m', "message", &stash_پیغام, "message",
			   N_("stash message")),
		OPT_انتہا()
	};

	argc = parse_options(argc, argv, سابقہ, options,
			     git_stash_save_usage,
			     PARSE_OPT_KEEP_DASHDASH);

	اگر (argc)
		stash_پیغام = strbuf_join_argv(&stash_پیغام_buf, argc, argv, ' ');

	memset(&ps, 0, حجم(ps));

	اگر (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	اگر (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	اگر (!patch_منوال) {
		اگر (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--patch");
		اگر (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--patch");
		اگر (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--patch");
	}

	ret = do_ڈالو_stash(&ps, stash_پیغام, quiet, keep_اشاریہ,
			    patch_منوال, &interactive_opts, include_untracked,
			    only_staged);

	strbuf_release(&stash_پیغام_buf);
	واپس ret;
}

ساکن صحیح write_commit_with_parents(ساخت repository *r,
				     ساخت object_id *برآمد,
				     ثابت ساخت object_id *oid,
				     ساخت commit_list *parents)
{
	سائز_t author_len, committer_len;
	ساخت commit *this;
	ثابت حرف *orig_author, *orig_committer;
	حرف *author = تہی, *committer = تہی;
	ثابت حرف *بفر;
	غیرعلامتی طویل bufsize;
	ثابت حرف *p;
	ساخت strbuf پیغام = STRBUF_INIT;
	صحیح ret = 0;
	ساخت ident_بانٹو id;

	this = lookup_commit_reference(r, oid);
	بفر = repo_get_commit_بفر(r, this, &bufsize);
	orig_author = ڈھونڈو_commit_سرخی(بفر, "author", &author_len);
	orig_committer = ڈھونڈو_commit_سرخی(بفر, "committer", &committer_len);

	اگر (!orig_author || !orig_committer) {
		ret = error(_("cannot parse commit %s"), oid_مقصد_hex(oid));
		جاؤ برآمد;
	}

	اگر (بانٹو_ident_سطر(&id, orig_author, author_len) < 0 ||
	    بانٹو_ident_سطر(&id, orig_committer, committer_len) < 0) {
		ret = error(_("invalid author or committer for %s"), oid_مقصد_hex(oid));
		جاؤ برآمد;
	}

	p = لڑی_تلاش(بفر, "\n\n");
	strbuf_addstr(&پیغام, "git stash: ");

	اگر (p)
		strbuf_جوڑو(&پیغام, p + 2, bufsize - (p + 2 - بفر));
	strbuf_complete_سطر(&پیغام);

	author = xmemdupz(orig_author, author_len);
	committer = xmemdupz(orig_committer, committer_len);

	اگر (commit_درخت_extended(پیغام.buf, پیغام.len,
				 r->hash_algo->خالی_خانے_درخت, parents,
				 برآمد, author, committer,
				 تہی, تہی)) {
		ret = error(_("could not write commit"));
		جاؤ برآمد;
	}
برآمد:
	strbuf_release(&پیغام);
	repo_unuse_commit_بفر(r, this, بفر);
	آزاد(author);
	آزاد(committer);
	واپس ret;
}

ساکن صحیح do_import_stash(ساخت repository *r, ثابت حرف *rev)
{
	ساخت object_id chain;
	صحیح res = 0;
	ثابت حرف *بفر = تہی;
	غیرعلامتی طویل bufsize;
	ساخت commit *this = تہی;
	ساخت commit_list *اشیاء = تہی, *cur;
	حرف *پیغام = تہی;

	اگر (repo_get_oid(r, rev, &chain))
		واپس error(_("not a valid revision: %s"), rev);

	this = lookup_commit_reference(r, &chain);
	اگر (!this)
		واپس error(_("not a commit: %s"), rev);

	/*
	 * Walk the commit history, finding each stash entry, and load data into
	 * the array.
	 */
	برائے (;;) {
		ثابت حرف *author, *committer;
		سائز_t author_len, committer_len;
		ثابت حرف *p;
		ثابت حرف *expected = "git stash <git@stash> 1000684800 +0000";
		ثابت حرف *سابقہ = "git stash: ";
		ساخت commit *stash;
		ساخت درخت *درخت = repo_get_commit_درخت(r, this);

		اگر (!درخت ||
		    !oideq(&درخت->object.oid, r->hash_algo->خالی_خانے_درخت) ||
		    (this->parents &&
		     (!this->parents->اگلا || this->parents->اگلا->اگلا))) {
			res = error(_("%s is not a valid exported stash commit"),
					oid_مقصد_hex(&this->object.oid));
			جاؤ برآمد;
		}

		بفر = repo_get_commit_بفر(r, this, &bufsize);

		اگر (!this->parents) {
			/*
			 * We don't have any parents.  Make sure this is our
			 * root commit.
			 */
			author = ڈھونڈو_commit_سرخی(بفر, "author", &author_len);
			committer = ڈھونڈو_commit_سرخی(بفر, "committer", &committer_len);

			اگر (!author || !committer) {
				error(_("cannot parse commit %s"), oid_مقصد_hex(&this->object.oid));
				جاؤ برآمد;
			}

			اگر (author_len != لڑی_لمبائی(expected) ||
			    committer_len != لڑی_لمبائی(expected) ||
			    memcmp(author, expected, author_len) ||
			    memcmp(committer, expected, committer_len)) {
				res = error(_("found root commit %s with invalid data"), oid_مقصد_hex(&this->object.oid));
				جاؤ برآمد;
			}
			توڑو;
		}

		p = لڑی_تلاش(بفر, "\n\n");
		اگر (!p) {
			res = error(_("cannot parse commit %s"), oid_مقصد_hex(&this->object.oid));
			جاؤ برآمد;
		}

		p += 2;
		اگر (((سائز_t)(bufsize - (p - بفر)) < لڑی_لمبائی(سابقہ)) ||
		    memcmp(سابقہ, p, لڑی_لمبائی(سابقہ))) {
			res = error(_("found stash commit %s without expected prefix"), oid_مقصد_hex(&this->object.oid));
			جاؤ برآمد;
		}

		stash = this->parents->اگلا->شے;

		اگر (repo_parse_commit(r, this->parents->شے) ||
		    repo_parse_commit(r, stash)) {
			res = error(_("cannot parse parents of commit: %s"),
					oid_مقصد_hex(&this->object.oid));
			جاؤ برآمد;
		}

		اگر (check_stash_topology(r, stash)) {
			res = error(_("%s does not look like a stash commit"),
					oid_مقصد_hex(&stash->object.oid));
			جاؤ برآمد;
		}

		repo_unuse_commit_بفر(r, this, بفر);
		بفر = تہی;
		اشیاء = commit_list_داخل_کرو(stash, &اشیاء);
		this = this->parents->شے;
	}

	/*
	 * Now, walk each entry, adding it to the stash as a normal stash
	 * commit.
	 */
	برائے (cur = اشیاء; cur; cur = cur->اگلا) {
		ثابت حرف *p;
		ساخت object_id *oid;

		this = cur->شے;
		oid = &this->object.oid;
		بفر = repo_get_commit_بفر(r, this, &bufsize);
		اگر (!بفر) {
			res = error(_("cannot read commit buffer for %s"), oid_مقصد_hex(oid));
			جاؤ برآمد;
		}

		p = لڑی_تلاش(بفر, "\n\n");
		اگر (!p) {
			res = error(_("cannot parse commit %s"), oid_مقصد_hex(oid));
			جاؤ برآمد;
		}

		p += 2;
		پیغام = xmemdupz(p, bufsize - (p - بفر));
		repo_unuse_commit_بفر(r, this, بفر);
		بفر = تہی;

		اگر (do_store_stash(oid, پیغام, 1)) {
			res = error(_("cannot save the stash for %s"), oid_مقصد_hex(oid));
			جاؤ برآمد;
		}
		FREE_AND_NULL(پیغام);
	}
برآمد:
	اگر (this && بفر)
		repo_unuse_commit_بفر(r, this, بفر);
	commit_list_free(اشیاء);
	آزاد(پیغام);

	واپس res;
}

ساکن صحیح import_stash(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
			ساخت repository *repo)
{
	ساخت option options[] = {
		OPT_انتہا()
	};

	argc = parse_options(argc, argv, سابقہ, options,
			     git_stash_import_usage,
			     PARSE_OPT_KEEP_DASHDASH);

	اگر (argc != 1)
		usage_پیغام_opt("a revision is required", git_stash_import_usage, options);

	واپس do_import_stash(repo, argv[0]);
}

ساخت stash_اندراج_ڈیٹا {
	ساخت repository *r;
	ساخت commit_list **اشیاء;
	سائز_t گنتی;
};

ساکن صحیح collect_stash_entries(ثابت حرف *refname غیر_مستعمل,
				 ساخت object_id *old_oid غیر_مستعمل,
				 ساخت object_id *new_oid,
				 ثابت حرف *committer غیر_مستعمل,
				 timestamp_t timestamp غیر_مستعمل,
				 صحیح tz غیر_مستعمل, ثابت حرف *پیغام غیر_مستعمل,
				 خالی *cb_ڈیٹا)
{
	ساخت stash_اندراج_ڈیٹا *ڈیٹا = cb_ڈیٹا;
	ساخت commit *stash;

	ڈیٹا->گنتی++;
	stash = lookup_commit_reference(ڈیٹا->r, new_oid);
	اگر (!stash || check_stash_topology(ڈیٹا->r, stash)) {
		واپس error(_("%s does not look like a stash commit"),
			     oid_مقصد_hex(new_oid));
	}
	ڈیٹا->اشیاء = commit_list_append(stash, ڈیٹا->اشیاء);
	واپس 0;
}

ساکن صحیح do_export_stash(ساخت repository *r,
			   ثابت حرف *ref,
			   صحیح argc,
			   ثابت حرف **argv)
{
	ساخت object_id بنیاد;
	ساخت commit *پچھلا_2;
	ساخت commit_list *اشیاء = تہی, **iter = &اشیاء, *cur;
	صحیح res = 0;
	صحیح i;
	ساخت strbuf revision = STRBUF_INIT;
	ثابت حرف *author, *committer;

	/*
	 * This is an arbitrary, fixed date, specifically the one used by git
	 * format-patch.  The goal is merely to produce reproducible output.
	 */
	prepare_fallback_ident("git stash", "git@stash");
	author = fmt_ident("git stash", "git@stash", WANT_BLANK_IDENT,
			   "2001-09-17T00:00:00Z", 0);
	committer = fmt_ident("git stash", "git@stash", WANT_BLANK_IDENT,
			      "2001-09-17T00:00:00Z", 0);

	/* First, we create a single empty commit. */
	اگر (commit_درخت_extended("", 0, r->hash_algo->خالی_خانے_درخت, تہی,
				 &بنیاد, author, committer, تہی, تہی))
		واپس error(_("unable to write base commit"));

	پچھلا_2 = lookup_commit_reference(r, &بنیاد);

	اگر (argc) {
		/*
		 * Find each specified stash, and load data into the array.
		 */
		برائے (i = 0; i < argc; i++) {
			ساخت object_id oid;
			ساخت commit *stash;

			اگر (parse_stash_revision(&revision, argv[i], 1) ||
			    repo_get_oid_with_flags(r, revision.buf, &oid,
						    GET_OID_QUIETLY |
						    GET_OID_GENTLY)) {
				res = error(_("unable to find stash entry %s"), argv[i]);
				جاؤ برآمد;
			}

			stash = lookup_commit_reference(r, &oid);
			اگر (!stash || check_stash_topology(r, stash)) {
				res = error(_("%s does not look like a stash commit"),
						revision.buf);
				جاؤ برآمد;
			}
			iter = commit_list_append(stash, iter);
		}
	} ورنہ {
		/*
		 * Walk the reflog, finding each stash entry, and load data into the
		 * array.
		 */
		ساخت stash_اندراج_ڈیٹا cb_ڈیٹا = {
			.r = r, .اشیاء = iter,
		};
		اگر (refs_for_each_reflog_ent_الٹ(get_main_ref_store(r),
						     "refs/stash",
						     collect_stash_entries,
						     &cb_ڈیٹا) && cb_ڈیٹا.گنتی)
			جاؤ برآمد;
	}

	/*
	 * Now, create a set of commits identical to the regular stash commits,
	 * but where their first parents form a chain to our original empty
	 * base commit.
	 */
	اشیاء = commit_list_الٹ(اشیاء);
	برائے (cur = اشیاء; cur; cur = cur->اگلا) {
		ساخت commit_list *parents = تہی;
		ساخت commit_list **اگلا = &parents;
		ساخت object_id برآمد;
		ساخت commit *stash = cur->شے;

		اگلا = commit_list_append(پچھلا_2, اگلا);
		اگلا = commit_list_append(stash, اگلا);
		res = write_commit_with_parents(r, &برآمد, &stash->object.oid, parents);
		commit_list_free(parents);
		اگر (res)
			جاؤ برآمد;
		پچھلا_2 = lookup_commit_reference(r, &برآمد);
	}
	اگر (ref)
		refs_update_ref(get_main_ref_store(r), تہی, ref,
				&پچھلا_2->object.oid, تہی, 0, UPDATE_REFS_DIE_ON_ERR);
	ورنہ
		puts(oid_مقصد_hex(&پچھلا_2->object.oid));
برآمد:
	strbuf_release(&revision);
	commit_list_free(اشیاء);

	واپس res;
}

شمار export_action {
	ACTION_NONE,
	ACTION_PRINT,
	ACTION_مقصد_REF,
};

ساکن صحیح export_stash(صحیح argc,
			ثابت حرف **argv,
			ثابت حرف *سابقہ,
			ساخت repository *repo)
{
	ثابت حرف *ref = تہی;
	شمار export_action action = ACTION_NONE;
	ساخت option options[] = {
		OPT_CMDMODE(0, "print", &action,
			    N_("print the object ID instead of writing it to a ref"),
			    ACTION_PRINT),
		OPT_STRING(0, "to-ref", &ref, "ref",
			    N_("save the data to the given ref")),
		OPT_انتہا()
	};

	argc = parse_options(argc, argv, سابقہ, options,
			     git_stash_export_usage,
			     PARSE_OPT_KEEP_DASHDASH);

	اگر (ref && action == ACTION_NONE)
		action = ACTION_مقصد_REF;

	اگر (action == ACTION_NONE || (ref && action == ACTION_PRINT))
		واپس error(_("exactly one of --print and --to-ref is required"));

	واپس do_export_stash(repo, ref, argc, argv);
}

صحیح cmd_stash(صحیح argc,
	      ثابت حرف **argv,
	      ثابت حرف *سابقہ,
	      ساخت repository *repo)
{
	pid_t pid = getpid();
	ثابت حرف *اشاریہ_file;
	ساخت strvec دلائل = STRVEC_INIT;
	parse_opt_subcommand_fn *fn = تہی;
	ساخت option options[] = {
		OPT_SUBCOMMAND("apply", &fn, apply_stash),
		OPT_SUBCOMMAND("clear", &fn, clear_stash),
		OPT_SUBCOMMAND("drop", &fn, drop_stash),
		OPT_SUBCOMMAND("pop", &fn, نکالو_stash),
		OPT_SUBCOMMAND("branch", &fn, branch_stash),
		OPT_SUBCOMMAND("list", &fn, list_stash),
		OPT_SUBCOMMAND("show", &fn, show_stash),
		OPT_SUBCOMMAND("store", &fn, store_stash),
		OPT_SUBCOMMAND("create", &fn, create_stash),
		OPT_SUBCOMMAND("push", &fn, ڈالو_stash_unassumed),
		OPT_SUBCOMMAND("export", &fn, export_stash),
		OPT_SUBCOMMAND("import", &fn, import_stash),
		OPT_SUBCOMMAND_F("save", &fn, save_stash, تہی, PARSE_OPT_NOCOMPLETE),
		OPT_انتہا()
	};
	ثابت حرف **دلائل_copy;
	صحیح ret;

	repo_config(the_repository, git_stash_config, تہی);

	argc = parse_options(argc, argv, سابقہ, options, git_stash_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL |
			     PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_بھرا_اشاریہ = 0;

	اشاریہ_file = repo_get_اشاریہ_file(the_repository);
	strbuf_addf(&stash_اشاریہ_راستہ, "%s.stash.%" PRIuMAX, اشاریہ_file,
		    (uintmax_t)pid);

	اگر (fn) {
		ret = fn(argc, argv, سابقہ, repo);

		/*
		 * The subcommand implementations return 0 on success, a
		 * negative value on failure, and STASH_APPLY_CONFLICT
		 * when applying a stash entry resulted in conflicts.
		 * Map failures to 128, the status die() uses, so that
		 * exit status 1 unambiguously indicates conflicts.
		 */
		اگر (ret < 0)
			واپس 128;
		واپس ret;
	} ورنہ اگر (!argc) {
		واپس !!ڈالو_stash_unassumed(0, تہی, سابقہ, repo);
	}

	/* Assume 'stash push' */
	strvec_ڈالو(&دلائل, "push");
	strvec_pushv(&دلائل, argv);

	/*
	 * `push_stash()` ends up modifying the array, which causes memory
	 * leaks if we didn't copy the array here.
	 */
	DUP_ARRAY(دلائل_copy, دلائل.v, دلائل.nr);

	ret = !!ڈالو_stash(دلائل.nr, دلائل_copy, سابقہ, 1);

	strvec_clear(&دلائل);
	آزاد(دلائل_copy);
	واپس ret;
}
