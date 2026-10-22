#تعریف_کن USE_THE_REPOSITORY_VARIABLE

#ضمن "builtin.h"
#ضمن "abspath.h"
#ضمن "config.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "hash.h"
#ضمن "hex.h"
#ضمن "object-name.h"
#ضمن "parse-options.h"
#ضمن "refs.h"
#ضمن "stash.h"
#ضمن "lockfile.h"
#ضمن "cache-tree.h"
#ضمن "unpack-trees.h"
#ضمن "merge-ort-wrappers.h"
#ضمن "strvec.h"
#ضمن "run-command.h"
#ضمن "dir.h"
#ضمن "entry.h"
#ضمن "preload-index.h"
#ضمن "read-cache.h"
#ضمن "repository.h"
#ضمن "rerere.h"
#ضمن "revision.h"
#ضمن "setup.h"
#ضمن "sparse-index.h"
#ضمن "log-tree.h"
#ضمن "diffcore.h"
#ضمن "reflog.h"
#ضمن "reflog-walk.h"
#ضمن "add-interactive.h"
#ضمن "commit.h"

#تعریف_کن INCLUDE_ALL_FILES 2

#تعریف_کن BUILTIN_STASH_LIST_USAGE \
	N_("git stash list [<log-options>]")
#تعریف_کن BUILTIN_STASH_SHOW_USAGE \
	N_("git stash show [-u | --include-untracked | --only-untracked] [<diff-options>] [<stash>]")
#تعریف_کن BUILTIN_STASH_DROP_USAGE \
	N_("git stash drop [-q | --quiet] [<stash>]")
#تعریف_کن BUILTIN_STASH_برداشت_USAGE \
	N_("git stash pop [--index] [-q | --quiet] [<stash>]")
#تعریف_کن BUILTIN_STASH_APPLY_USAGE \
	N_("git stash apply [--index] [-q | --quiet] [--label-ours=<label>] [--label-theirs=<label>] [--label-base=<label>] [<stash>]")
#تعریف_کن BUILTIN_STASH_BRANCH_USAGE \
	N_("git stash branch <branchname> [<stash>]")
#تعریف_کن BUILTIN_STASH_STORE_USAGE \
	N_("git stash store [(-m | --message) <message>] [-q | --quiet] <commit>")
#تعریف_کن BUILTIN_STASH_افزودن_USAGE \
	N_("git stash [push] [-p | --patch] [-S | --staged] [-k | --[no-]keep-index] [-q | --quiet]\n" \
	   "          [-u | --include-untracked] [-a | --all] [(-m | --message) <message>]\n" \
	   "          [--pathspec-from-file=<file> [--pathspec-file-nul]]\n" \
	   "          [--] [<pathspec>...]")
#تعریف_کن BUILTIN_STASH_SAVE_USAGE \
	N_("git stash save [-p | --patch] [-S | --staged] [-k | --[no-]keep-index] [-q | --quiet]\n" \
	   "          [-u | --include-untracked] [-a | --all] [<message>]")
#تعریف_کن BUILTIN_STASH_CREATE_USAGE \
	N_("git stash create [<message>]")
#تعریف_کن BUILTIN_STASH_EXPORT_USAGE \
	N_("git stash export (--print | --to-ref <ref>) [<stash>...]")
#تعریف_کن BUILTIN_STASH_IMPORT_USAGE \
	N_("git stash import <commit>")
#تعریف_کن BUILTIN_STASH_CLEAR_USAGE \
	"git stash clear"

ساكن ثابت حرف * ثابت git_stash_usage[] = {
	BUILTIN_STASH_LIST_USAGE,
	BUILTIN_STASH_SHOW_USAGE,
	BUILTIN_STASH_DROP_USAGE,
	BUILTIN_STASH_برداشت_USAGE,
	BUILTIN_STASH_APPLY_USAGE,
	BUILTIN_STASH_BRANCH_USAGE,
	BUILTIN_STASH_افزودن_USAGE,
	BUILTIN_STASH_SAVE_USAGE,
	BUILTIN_STASH_CLEAR_USAGE,
	BUILTIN_STASH_CREATE_USAGE,
	BUILTIN_STASH_STORE_USAGE,
	BUILTIN_STASH_EXPORT_USAGE,
	BUILTIN_STASH_IMPORT_USAGE,
	عدم
};

ساكن ثابت حرف * ثابت git_stash_list_usage[] = {
	BUILTIN_STASH_LIST_USAGE,
	عدم
};

ساكن ثابت حرف * ثابت git_stash_show_usage[] = {
	BUILTIN_STASH_SHOW_USAGE,
	عدم
};

ساكن ثابت حرف * ثابت git_stash_drop_usage[] = {
	BUILTIN_STASH_DROP_USAGE,
	عدم
};

ساكن ثابت حرف * ثابت git_stash_برداشت_usage[] = {
	BUILTIN_STASH_برداشت_USAGE,
	عدم
};

ساكن ثابت حرف * ثابت git_stash_apply_usage[] = {
	BUILTIN_STASH_APPLY_USAGE,
	عدم
};

ساكن ثابت حرف * ثابت git_stash_branch_usage[] = {
	BUILTIN_STASH_BRANCH_USAGE,
	عدم
};

ساكن ثابت حرف * ثابت git_stash_clear_usage[] = {
	BUILTIN_STASH_CLEAR_USAGE,
	عدم
};

ساكن ثابت حرف * ثابت git_stash_store_usage[] = {
	BUILTIN_STASH_STORE_USAGE,
	عدم
};

ساكن ثابت حرف * ثابت git_stash_افزودن_usage[] = {
	BUILTIN_STASH_افزودن_USAGE,
	عدم
};

ساكن ثابت حرف * ثابت git_stash_save_usage[] = {
	BUILTIN_STASH_SAVE_USAGE,
	عدم
};

ساكن ثابت حرف * ثابت git_stash_export_usage[] = {
	BUILTIN_STASH_EXPORT_USAGE,
	عدم
};

ساكن ثابت حرف * ثابت git_stash_import_usage[] = {
	BUILTIN_STASH_IMPORT_USAGE,
	عدم
};

ساكن ثابت حرف ref_stash[] = "refs/stash";
ساكن بنية strbuf stash_شاخص_مسیر = STRBUF_INIT;

ساكن صحيح show_stat = 1;
ساكن صحيح show_patch;
ساكن صحيح show_include_untracked;
ساكن صحيح use_شاخص;

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
بنية stash_info {
	بنية object_id w_commit;
	بنية object_id b_commit;
	بنية object_id i_commit;
	بنية object_id u_commit;
	بنية object_id c_commit;
	بنية object_id w_درخت;
	بنية object_id b_درخت;
	بنية object_id i_درخت;
	بنية object_id u_درخت;
	بنية strbuf revision;
	صحيح is_stash_ref;
	صحيح has_u;
};

#تعریف_کن STASH_INFO_INIT { \
	.revision = STRBUF_INIT, \
}

ساكن فراغ free_stash_info(بنية stash_info *info)
{
	strbuf_release(&info->revision);
}

ساكن صحيح check_stash_topology(بنية repository *r, بنية commit *stash)
{
	بنية commit *p1, *p2, *p3 = عدم;

	/* stash must have two or three parents */
	إذا (!stash->parents || !stash->parents->بعدی ||
			(stash->parents->بعدی->بعدی && stash->parents->بعدی->بعدی->بعدی))
		أرجع -1;
	p1 = stash->parents->قلم;
	p2 = stash->parents->بعدی->قلم;
	إذا (stash->parents->بعدی->بعدی)
		p3 = stash->parents->بعدی->بعدی->قلم;
	إذا (repo_parse_commit(r, p1) || repo_parse_commit(r, p2) ||
			(p3 && repo_parse_commit(r, p3)))
		أرجع -1;
	/* p2 must have a single parent, p3 must have no parents */
	إذا (!p2->parents || p2->parents->بعدی || (p3 && p3->parents))
		أرجع -1;
	إذا (repo_parse_commit(r, p2->parents->قلم))
		أرجع -1;
	/* p2^1 must equal p1 */
	إذا (!oideq(&p1->object.oid, &p2->parents->قلم->object.oid))
		أرجع -1;

	أرجع 0;
}

ساكن فراغ assert_stash_like(بنية stash_info *info, ثابت حرف *revision)
{
	إذا (get_oidf(&info->b_commit, "%s^1", revision) ||
	    get_oidf(&info->w_درخت, "%s:", revision) ||
	    get_oidf(&info->b_درخت, "%s^1:", revision) ||
	    get_oidf(&info->i_درخت, "%s^2:", revision))
		die(_("'%s' is not a stash-like commit"), revision);
}

ساكن صحيح parse_stash_revision(بنية strbuf *revision, ثابت حرف *commit, صحيح quiet)
{
	strbuf_reset(revision);
	إذا (!commit) {
		إذا (!refs_ref_exists(get_main_ref_store(the_repository), ref_stash)) {
			إذا (!quiet)
				fprintf_ln(stderr, _("No stash entries found."));
			أرجع -1;
		}

		strbuf_addf(revision, "%s@{0}", ref_stash);
	} وإلا إذا (strspn(commit, "0123456789") == طول_رشته(commit)) {
		strbuf_addf(revision, "%s@{%s}", ref_stash, commit);
	} وإلا {
		strbuf_addstr(revision, commit);
	}
	أرجع 0;
}

ساكن صحيح get_stash_info(بنية stash_info *info, صحيح argc, ثابت حرف **argv)
{
	صحيح ret;
	حرف *انتها_از_rev;
	حرف *expanded_ref;
	ثابت حرف *revision;
	ثابت حرف *commit = عدم;
	بنية object_id dummy;
	بنية strbuf symbolic = STRBUF_INIT;

	إذا (argc > 1) {
		صحيح i;
		بنية strbuf refs_پیام = STRBUF_INIT;

		لكل (i = 0; i < argc; i++)
			strbuf_addf(&refs_پیام, " '%s'", argv[i]);

		fprintf_ln(stderr, _("Too many revisions specified:%s"),
			   refs_پیام.buf);
		strbuf_release(&refs_پیام);

		أرجع -1;
	}

	إذا (argc == 1)
		commit = argv[0];

	strbuf_init(&info->revision, 0);
	إذا (parse_stash_revision(&info->revision, commit, 0)) {
		أرجع -1;
	}

	revision = info->revision.buf;

	إذا (repo_get_oid(the_repository, revision, &info->w_commit))
		أرجع error(_("%s is not a valid reference"), revision);

	assert_stash_like(info, revision);

	info->has_u = !get_oidf(&info->u_درخت, "%s^3:", revision);

	انتها_از_rev = strchrnul(revision, '@');
	strbuf_جمع_کن(&symbolic, revision, انتها_از_rev - revision);

	ret = repo_dwim_ref(the_repository, symbolic.buf, symbolic.len,
			    &dummy, &expanded_ref, 0);
	strbuf_release(&symbolic);
	بدل (ret) {
	حالة 0: /* Not found, but valid ref */
		info->is_stash_ref = 0;
		اكسر;
	حالة 1:
		info->is_stash_ref = !strcmp(expanded_ref, ref_stash);
		اكسر;
	افتراضي: /* Invalid or ambiguous */
		اكسر;
	}

	حرر(expanded_ref);
	أرجع !(ret == 0 || ret == 1);
}

ساكن صحيح do_clear_stash(فراغ)
{
	بنية object_id obj;
	إذا (repo_get_oid(the_repository, ref_stash, &obj))
		أرجع 0;

	أرجع refs_delete_ref(get_main_ref_store(the_repository), عدم,
			       ref_stash, &obj, 0);
}

ساكن صحيح clear_stash(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
		       بنية repository *repo استفادهنشده)
{
	بنية option options[] = {
		OPT_انتها()
	};

	argc = parse_options(argc, argv, پیشوند, options,
			     git_stash_clear_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	إذا (argc)
		أرجع error(_("git stash clear with arguments is "
			       "unimplemented"));

	أرجع do_clear_stash();
}

ساكن صحيح reset_درخت(بنية object_id *i_درخت, صحيح update, صحيح reset)
{
	صحيح nr_trees = 1;
	بنية unpack_trees_options opts;
	بنية درخت_desc t[MAX_UNPACK_TREES];
	بنية درخت *درخت;
	بنية قفل_file قفل_file = قفل_INIT;

	repo_read_شاخص_preload(the_repository, عدم, 0);
	إذا (refresh_شاخص(the_repository->شاخص, REFRESH_QUIET, عدم, عدم, عدم))
		أرجع -1;

	repo_hold_locked_شاخص(the_repository, &قفل_file, قفل_DIE_ON_ERROR);

	memset(&opts, 0, حجم(opts));

	درخت = repo_parse_درخت_غیرمستقیم(the_repository, i_درخت);
	إذا (repo_parse_درخت(the_repository, درخت))
		أرجع -1;

	init_درخت_desc(t, &درخت->object.oid, درخت->میانگیر, درخت->اندازه);

	opts.سر_idx = 1;
	opts.src_شاخص = the_repository->شاخص;
	opts.dst_شاخص = the_repository->شاخص;
	opts.ادغام = 1;
	opts.reset = reset ? UNPACK_RESET_PROTECT_UNTRACKED : 0;
	opts.update = update;
	إذا (update)
		opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
	opts.fn = oneway_ادغام;

	إذا (unpack_trees(nr_trees, t, &opts))
		أرجع -1;

	إذا (write_locked_شاخص(the_repository->شاخص, &قفل_file, COMMIT_قفل))
		أرجع error(_("unable to write new index file"));

	أرجع 0;
}

ساكن صحيح create_شاخص_from_درخت(ثابت بنية object_id *درخت_id,
				  ثابت حرف *شاخص_مسیر)
{
	صحيح nr_trees = 1;
	صحيح ret = 0;
	بنية unpack_trees_options opts;
	بنية درخت_desc t[MAX_UNPACK_TREES];
	بنية درخت *درخت;
	بنية شاخص_state dst_istate = شاخص_STATE_INIT(the_repository);
	بنية قفل_file قفل_file = قفل_INIT;

	repo_read_شاخص_preload(the_repository, عدم, 0);
	refresh_شاخص(the_repository->شاخص, REFRESH_QUIET, عدم, عدم, عدم);

	hold_قفل_file_for_update(&قفل_file, شاخص_مسیر, قفل_DIE_ON_ERROR);

	memset(&opts, 0, حجم(opts));

	درخت = repo_parse_درخت_غیرمستقیم(the_repository, درخت_id);
	إذا (!درخت || repo_parse_درخت(the_repository, درخت)) {
		ret = -1;
		اذهب انجامشده;
	}

	init_درخت_desc(t, &درخت->object.oid, درخت->میانگیر, درخت->اندازه);

	opts.سر_idx = 1;
	opts.src_شاخص = the_repository->شاخص;
	opts.dst_شاخص = &dst_istate;
	opts.ادغام = 1;
	opts.reset = UNPACK_RESET_PROTECT_UNTRACKED;
	opts.fn = oneway_ادغام;

	إذا (unpack_trees(nr_trees, t, &opts)) {
		ret = -1;
		اذهب انجامشده;
	}

	إذا (write_locked_شاخص(&dst_istate, &قفل_file, COMMIT_قفل)) {
		ret = error(_("unable to write new index file"));
		اذهب انجامشده;
	}

انجامشده:
	release_شاخص(&dst_istate);
	إذا (ret)
		rollback_قفل_file(&قفل_file);
	أرجع ret;
}

ساكن صحيح is_مسیر_a_directory(ثابت حرف *مسیر)
{
	/*
	 * This function differs from abspath.c:is_directory() in that
	 * here we use lstat() instead of stat(); we do not want to
	 * follow symbolic links here.
	 */
	بنية stat st;
	أرجع (!lstat(مسیر, &st) && S_ISDIR(st.st_نما));
}

ساكن فراغ جمع_کن_diff_مقصد_buf(بنية diff_صف_struct *q,
			    بنية diff_options *options استفادهنشده,
			    فراغ *داده)
{
	صحيح i;

	لكل (i = 0; i < q->nr; i++) {
		إذا (is_مسیر_a_directory(q->صف[i]->one->مسیر))
			استمر;

		strbuf_addstr(داده, q->صف[i]->one->مسیر);

		/* NUL-terminate: will be fed to update-index -z */
		strbuf_addch(داده, '\0');
	}
}

ساكن صحيح restore_untracked(بنية object_id *u_درخت)
{
	صحيح res;
	بنية فرزند_process cp = فرزند_PROCESS_INIT;

	/*
	 * We need to run restore files from a given index, but without
	 * affecting the current index, so we use GIT_INDEX_FILE with
	 * run_command to fork processes that will not interfere.
	 */
	cp.git_cmd = 1;
	strvec_افزودن(&cp.آرگومانها, "read-tree");
	strvec_افزودن(&cp.آرگومانها, oid_مقصد_hex(u_درخت));
	strvec_pushf(&cp.env, "GIT_INDEX_FILE=%s",
		     stash_شاخص_مسیر.buf);
	إذا (پیاپی_command(&cp)) {
		remove_مسیر(stash_شاخص_مسیر.buf);
		أرجع -1;
	}

	فرزند_process_init(&cp);
	cp.git_cmd = 1;
	strvec_pushl(&cp.آرگومانها, "checkout-index", "--all", عدم);
	strvec_pushf(&cp.env, "GIT_INDEX_FILE=%s",
		     stash_شاخص_مسیر.buf);

	res = پیاپی_command(&cp);
	remove_مسیر(stash_شاخص_مسیر.buf);
	أرجع res;
}

ساكن فراغ unstage_changes_unless_new(بنية object_id *orig_درخت)
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

	بنية checkout state = CHECKOUT_INIT;
	بنية diff_options diff_opts;
	بنية قفل_file قفل = قفل_INIT;
	صحيح i;

	/* If any entries have skip_worktree set, we'll have to check 'em out */
	state.force = 1;
	state.quiet = 1;
	state.refresh_cache = 1;
	state.istate = the_repository->شاخص;

	/*
	 * Step 1: get a difference between orig_tree (which corresponding
	 * to the index before a merge was run) and the current index
	 * (reflecting the changes brought in by the merge).
	 */
	repo_diff_راهاندازی(the_repository, &diff_opts);
	diff_opts.flags.recursive = 1;
	diff_opts.detect_rename = 0;
	diff_opts.output_format = DIFF_FORMAT_NO_OUTPUT;
	diff_راهاندازی_انجامشده(&diff_opts);

	do_diff_cache(orig_درخت, &diff_opts);
	diffcore_std(&diff_opts);

	/* Iterate over the paths that changed due to the merge... */
	لكل (i = 0; i < diff_queued_diff.nr; i++) {
		بنية diff_filepair *p;
		بنية cache_مدخل *ce;
		صحيح pos;

		/* Look up the path's position in the current index. */
		p = diff_queued_diff.صف[i];
		pos = شاخص_name_pos(the_repository->شاخص, p->two->مسیر,
				     طول_رشته(p->two->مسیر));

		/*
		 * Step 2: Place changes in the working tree
		 *
		 * Stash is about restoring changes *to the working tree*.
		 * So if the merge successfully got a new version of some
		 * path, but left it out of the working tree, then clear the
		 * SKIP_WORKTREE bit and write it to the working tree.
		 */
		إذا (pos >= 0 && ce_skip_worktree(the_repository->شاخص->cache[pos])) {
			بنية stat st;

			ce = the_repository->شاخص->cache[pos];
			إذا (!lstat(ce->name, &st)) {
				/* Conflicting path present; relocate it */
				بنية strbuf new_مسیر = STRBUF_INIT;
				صحيح fd;

				strbuf_addf(&new_مسیر,
					    "%s.stash.XXXXXX", ce->name);
				fd = xmkstemp(new_مسیر.buf);
				close(fd);
				چاپ_کن(_("WARNING: Untracked file in way of "
					 "tracked file!  Renaming\n "
					 "           %s -> %s\n"
					 "         to make room.\n"),
				       ce->name, new_مسیر.buf);
				إذا (rename(ce->name, new_مسیر.buf))
					die("Failed to move %s to %s",
					    ce->name, new_مسیر.buf);
				strbuf_release(&new_مسیر);
			}
			checkout_مدخل(ce, &state, عدم, عدم);
			ce->ce_flags &= ~CE_SKIP_WORKTREE;
		}

		/*
		 * Step 3: "unstage" changes, as long as they are still tracked
		 */
		إذا (p->one->oid_valid) {
			/*
			 * Path existed in orig_tree; restore index entry
			 * from that tree in order to "unstage" the changes.
			 */
			صحيح option = جمع_کن_CACHE_OK_مقصد_REPLACE;
			إذا (pos < 0)
				option = جمع_کن_CACHE_OK_مقصد_جمع_کن;

			ce = make_cache_مدخل(the_repository->شاخص,
					      p->one->نما,
					      &p->one->oid,
					      p->one->مسیر,
					      0, 0);
			جمع_کن_شاخص_مدخل(the_repository->شاخص, ce, option);
		}
	}
	diff_flush(&diff_opts);

	/*
	 * Step 4: write the new index to disk
	 */
	repo_hold_locked_شاخص(the_repository, &قفل, قفل_DIE_ON_ERROR);
	إذا (write_locked_شاخص(the_repository->شاخص, &قفل,
			       COMMIT_قفل | SKIP_IF_UNCHANGED))
		die(_("could not write index"));
}

ساكن تعداد stash_apply_نتیجه do_apply_stash(ثابت حرف *پیشوند,
					      بنية stash_info *info,
					      صحيح شاخص, صحيح quiet,
					      ثابت حرف *label_ours,
					      ثابت حرف *label_theirs,
					      ثابت حرف *label_پایه)
{
	صحيح clean, ret;
	صحيح has_شاخص = شاخص;
	بنية ادغام_options o;
	بنية object_id c_درخت;
	بنية object_id شاخص_درخت;
	بنية درخت *سر, *ادغام, *ادغام_پایه;
	بنية قفل_file قفل = قفل_INIT;

	repo_read_شاخص_preload(the_repository, عدم, 0);
	إذا (repo_refresh_and_write_شاخص(the_repository, REFRESH_QUIET, 0, 0,
					 عدم, عدم, عدم))
		أرجع error(_("could not write index"));

	إذا (write_شاخص_as_درخت(&c_درخت, the_repository->شاخص,
				repo_get_شاخص_file(the_repository), 0, عدم))
		أرجع error(_("cannot apply a stash in the middle of a merge"));

	init_ui_ادغام_options(&o, the_repository);

	إذا (quiet)
		o.verbosity = 0;

	إذا (شاخص) {
		إذا (oideq(&info->b_درخت, &info->i_درخت) ||
		    oideq(&c_درخت, &info->i_درخت)) {
			has_شاخص = 0;
		} وإلا {
			بنية ادغام_نتیجه نتیجه = { 0 };

			o.branch1 = "Current index";
			o.branch2 = "Stashed index changes";
			o.ancestor = "Stash base";

			سر = lookup_درخت(o.repo, &c_درخت);
			ادغام = lookup_درخت(o.repo, &info->i_درخت);
			ادغام_پایه = lookup_درخت(o.repo, &info->b_درخت);

			ادغام_incore_nonrecursive(&o, ادغام_پایه, سر, ادغام,
						  &نتیجه);

			إذا (نتیجه.clean < 0) {
				ادغام_finalize(&o, &نتیجه);
				أرجع error(_("index merge failed"));
			} وإلا إذا (!نتیجه.clean) {
				ادغام_finalize(&o, &نتیجه);
				أرجع error(_("conflicts in index. "
					       "Try without --index."));
			} وإلا {
				oidcpy(&شاخص_درخت, &نتیجه.درخت->object.oid);
				ادغام_finalize(&o, &نتیجه);
			}
		}
	}

	o.branch1 = label_ours ? label_ours : "Updated upstream";
	o.branch2 = label_theirs ? label_theirs : "Stashed changes";
	o.ancestor = label_پایه ? label_پایه : "Stash base";

	إذا (oideq(&info->b_درخت, &c_درخت))
		o.branch1 = "Version stash was based on";

	إذا (o.verbosity >= 3)
		printf_ln(_("Merging %s with %s"), o.branch1, o.branch2);

	سر = lookup_درخت(o.repo, &c_درخت);
	ادغام = lookup_درخت(o.repo, &info->w_درخت);
	ادغام_پایه = lookup_درخت(o.repo, &info->b_درخت);

	repo_hold_locked_شاخص(o.repo, &قفل, قفل_DIE_ON_ERROR);
	clean = ادغام_ort_nonrecursive(&o, سر, ادغام, ادغام_پایه);

	/*
	 * If 'clean' >= 0, reverse the value for 'ret' so 'ret' is 0 when the
	 * merge was clean, and 1 if the merge was unclean or a negative value
	 * if it encountered an error.
	 */
	ret = clean >= 0 ? !clean : clean;

	إذا (ret < 0)
		rollback_قفل_file(&قفل);
	وإلا إذا (write_locked_شاخص(o.repo->شاخص, &قفل,
				      COMMIT_قفل | SKIP_IF_UNCHANGED))
		ret = error(_("could not write index"));

	إذا (ret) {
		repo_rerere(the_repository, 0);

		إذا (شاخص)
			fprintf_ln(stderr, _("Index was not unstashed."));

		اذهب restore_untracked;
	}

	إذا (has_شاخص) {
		إذا (reset_درخت(&شاخص_درخت, 0, 0))
			ret = -1;
	} وإلا {
		unstage_changes_unless_new(&c_درخت);
	}

restore_untracked:
	إذا (info->has_u && restore_untracked(&info->u_درخت))
		ret = error(_("could not restore untracked files from stash"));

	إذا (!quiet) {
		بنية فرزند_process cp = فرزند_PROCESS_INIT;

		/*
		 * Status is quite simple and could be replaced with calls to
		 * wt_status in the future, but it adds complexities which may
		 * require more tests.
		 */
		cp.git_cmd = 1;
		cp.dir = پیشوند;
		strvec_pushf(&cp.env, GIT_WORK_درخت_ENVIRONMENT"=%s",
			     absolute_مسیر(repo_get_work_درخت(the_repository)));
		strvec_pushf(&cp.env, GIT_DIR_ENVIRONMENT"=%s",
			     absolute_مسیر(repo_get_git_dir(the_repository)));
		strvec_افزودن(&cp.آرگومانها, "status");
		پیاپی_command(&cp);
	}

	أرجع ret;
}

ساكن صحيح apply_stash(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
		       بنية repository *repo استفادهنشده)
{
	صحيح ret = -1;
	صحيح quiet = 0;
	صحيح شاخص = use_شاخص;
	ثابت حرف *label_ours = عدم, *label_theirs = عدم, *label_پایه = عدم;
	بنية stash_info info = STASH_INFO_INIT;
	بنية option options[] = {
		OPT__QUIET(&quiet, N_("be quiet, only report errors")),
		OPT_BOOL(0, "index", &شاخص,
			 N_("attempt to recreate the index")),
		OPT_STRING(0, "label-ours", &label_ours, N_("label"),
			   N_("label for the upstream side in conflict markers")),
		OPT_STRING(0, "label-theirs", &label_theirs, N_("label"),
			   N_("label for the stashed side in conflict markers")),
		OPT_STRING(0, "label-base", &label_پایه, N_("label"),
			   N_("label for the base in diff3 conflict markers")),
		OPT_انتها()
	};

	argc = parse_options(argc, argv, پیشوند, options,
			     git_stash_apply_usage, 0);

	إذا (get_stash_info(&info, argc, argv))
		اذهب cleanup;

	ret = do_apply_stash(پیشوند, &info, شاخص, quiet,
			     label_ours, label_theirs, label_پایه);
cleanup:
	free_stash_info(&info);
	أرجع ret;
}

ساكن صحيح reject_reflog_ent(ثابت حرف *refname استفادهنشده,
			     بنية object_id *ooid استفادهنشده,
			     بنية object_id *noid استفادهنشده,
			     ثابت حرف *email استفادهنشده,
			     timestamp_t timestamp استفادهنشده,
			     صحيح tz استفادهنشده, ثابت حرف *پیام_2 استفادهنشده,
			     فراغ *cb_داده استفادهنشده)
{
	أرجع 1;
}

ساكن صحيح reflog_is_خالی(ثابت حرف *refname)
{
	أرجع !refs_for_each_reflog_ent(get_main_ref_store(the_repository),
					 refname, reject_reflog_ent, عدم);
}

ساكن صحيح do_drop_stash(بنية stash_info *info, صحيح quiet)
{
	إذا (!reflog_delete(info->revision.buf,
			   EXPIRE_REFLOGS_REWRITE | EXPIRE_REFLOGS_UPDATE_REF,
			   0)) {
		إذا (!quiet)
			printf_ln(_("Dropped %s (%s)"), info->revision.buf,
				  oid_مقصد_hex(&info->w_commit));
	} وإلا {
		أرجع error(_("%s: Could not drop stash entry"),
			     info->revision.buf);
	}

	إذا (reflog_is_خالی(ref_stash))
		do_clear_stash();

	أرجع 0;
}

ساكن صحيح get_stash_info_assert(بنية stash_info *info, صحيح argc,
				 ثابت حرف **argv)
{
	صحيح ret = get_stash_info(info, argc, argv);

	إذا (ret < 0)
		أرجع ret;

	إذا (!info->is_stash_ref)
		أرجع error(_("'%s' is not a stash reference"), info->revision.buf);

	أرجع 0;
}

ساكن صحيح drop_stash(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
		      بنية repository *repo استفادهنشده)
{
	صحيح ret = -1;
	صحيح quiet = 0;
	بنية stash_info info = STASH_INFO_INIT;
	بنية option options[] = {
		OPT__QUIET(&quiet, N_("be quiet, only report errors")),
		OPT_انتها()
	};

	argc = parse_options(argc, argv, پیشوند, options,
			     git_stash_drop_usage, 0);

	إذا (get_stash_info_assert(&info, argc, argv))
		اذهب cleanup;

	ret = do_drop_stash(&info, quiet);
cleanup:
	free_stash_info(&info);
	أرجع ret;
}

ساكن صحيح برداشت_stash(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
		     بنية repository *repo استفادهنشده)
{
	صحيح ret = -1;
	صحيح شاخص = use_شاخص;
	صحيح quiet = 0;
	بنية stash_info info = STASH_INFO_INIT;
	بنية option options[] = {
		OPT__QUIET(&quiet, N_("be quiet, only report errors")),
		OPT_BOOL(0, "index", &شاخص,
			 N_("attempt to recreate the index")),
		OPT_انتها()
	};

	argc = parse_options(argc, argv, پیشوند, options,
			     git_stash_برداشت_usage, 0);

	إذا (get_stash_info_assert(&info, argc, argv))
		اذهب cleanup;

	إذا ((ret = do_apply_stash(پیشوند, &info, شاخص, quiet,
				  عدم, عدم, عدم)))
		printf_ln(_("The stash entry is kept in case "
			    "you need it again."));
	وإلا
		ret = do_drop_stash(&info, quiet);

cleanup:
	free_stash_info(&info);
	أرجع ret;
}

ساكن صحيح branch_stash(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
			بنية repository *repo استفادهنشده)
{
	صحيح ret = -1;
	ثابت حرف *branch = عدم;
	بنية stash_info info = STASH_INFO_INIT;
	بنية فرزند_process cp = فرزند_PROCESS_INIT;
	بنية option options[] = {
		OPT_انتها()
	};

	argc = parse_options(argc, argv, پیشوند, options,
			     git_stash_branch_usage, 0);

	إذا (!argc) {
		fprintf_ln(stderr, _("No branch name specified"));
		أرجع -1;
	}

	branch = argv[0];

	إذا (get_stash_info(&info, argc - 1, argv + 1))
		اذهب cleanup;

	cp.git_cmd = 1;
	strvec_pushl(&cp.آرگومانها, "checkout", "-b", عدم);
	strvec_افزودن(&cp.آرگومانها, branch);
	strvec_افزودن(&cp.آرگومانها, oid_مقصد_hex(&info.b_commit));
	ret = پیاپی_command(&cp);
	إذا (!ret)
		ret = do_apply_stash(پیشوند, &info, 1, 0,
				     عدم, عدم, عدم);
	إذا (!ret && info.is_stash_ref)
		ret = do_drop_stash(&info, 0);

cleanup:
	free_stash_info(&info);
	أرجع ret;
}

ساكن صحيح list_stash(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
		      بنية repository *repo استفادهنشده)
{
	بنية فرزند_process cp = فرزند_PROCESS_INIT;
	بنية option options[] = {
		OPT_انتها()
	};

	argc = parse_options(argc, argv, پیشوند, options,
			     git_stash_list_usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT);

	إذا (!refs_ref_exists(get_main_ref_store(the_repository), ref_stash))
		أرجع 0;

	cp.git_cmd = 1;
	strvec_pushl(&cp.آرگومانها, "log", "--format=%gd: %gs", "-g",
		     "--first-parent", عدم);
	strvec_pushv(&cp.آرگومانها, argv);
	strvec_افزودن(&cp.آرگومانها, ref_stash);
	strvec_افزودن(&cp.آرگومانها, "--");
	أرجع پیاپی_command(&cp);
}

ساكن صحيح git_stash_config(ثابت حرف *var, ثابت حرف *مقدار,
			    ثابت بنية config_context *ctx, فراغ *cb)
{
	إذا (!strcmp(var, "stash.showstat")) {
		show_stat = git_config_bool(var, مقدار);
		أرجع 0;
	}
	إذا (!strcmp(var, "stash.showpatch")) {
		show_patch = git_config_bool(var, مقدار);
		أرجع 0;
	}
	إذا (!strcmp(var, "stash.showincludeuntracked")) {
		show_include_untracked = git_config_bool(var, مقدار);
		أرجع 0;
	}
	إذا (!strcmp(var, "stash.index")) {
		use_شاخص = git_config_bool(var, مقدار);
		أرجع 0;
	}
	أرجع git_diff_basic_config(var, مقدار, ctx, cb);
}

ساكن فراغ diff_include_untracked(ثابت بنية stash_info *info, بنية diff_options *diff_opt)
{
	ثابت بنية object_id *oid[] = { &info->w_commit, &info->u_درخت };
	بنية درخت *درخت[ARRAY_اندازه(oid)];
	بنية درخت_desc درخت_desc[ARRAY_اندازه(oid)];
	بنية unpack_trees_options unpack_درخت_opt = { 0 };

	لكل (اندازه_t i = 0; i < ARRAY_اندازه(oid); i++) {
		درخت[i] = repo_parse_درخت_غیرمستقیم(the_repository, oid[i]);
		إذا (repo_parse_درخت(the_repository, درخت[i]) < 0)
			die(_("failed to parse tree"));
		init_درخت_desc(&درخت_desc[i], &درخت[i]->object.oid,
			       درخت[i]->میانگیر, درخت[i]->اندازه);
	}

	unpack_درخت_opt.سر_idx = -1;
	unpack_درخت_opt.src_شاخص = the_repository->شاخص;
	unpack_درخت_opt.dst_شاخص = the_repository->شاخص;
	unpack_درخت_opt.ادغام = 1;
	unpack_درخت_opt.fn = stash_worktree_untracked_ادغام;

	إذا (unpack_trees(ARRAY_اندازه(درخت_desc), درخت_desc, &unpack_درخت_opt))
		die(_("failed to unpack trees"));

	do_diff_cache(&info->b_commit, diff_opt);
}

ساكن صحيح show_stash(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
		      بنية repository *repo استفادهنشده)
{
	صحيح i;
	صحيح ret = -1;
	بنية stash_info info = STASH_INFO_INIT;
	بنية rev_info rev;
	بنية strvec stash_آرگومانها = STRVEC_INIT;
	بنية strvec revision_آرگومانها = STRVEC_INIT;
	تعداد {
		UNTRACKED_NONE,
		UNTRACKED_INCLUDE,
		UNTRACKED_ONLY
	} show_untracked = show_include_untracked ? UNTRACKED_INCLUDE : UNTRACKED_NONE;
	بنية option options[] = {
		OPT_مجموعه_INT('u', "include-untracked", &show_untracked,
			    N_("include untracked files in the stash"),
			    UNTRACKED_INCLUDE),
		OPT_مجموعه_INT_F(0, "only-untracked", &show_untracked,
			      N_("only show untracked files in the stash"),
			      UNTRACKED_ONLY, PARSE_OPT_NONEG),
		OPT_انتها()
	};
	صحيح do_usage = 0;

	init_diff_ui_defaults();
	repo_config(the_repository, git_diff_ui_config, عدم);
	repo_init_revisions(the_repository, &rev, پیشوند);

	argc = parse_options(argc, argv, پیشوند, options, git_stash_show_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	strvec_افزودن(&revision_آرگومانها, argv[0]);
	لكل (i = 1; i < argc; i++) {
		إذا (argv[i][0] != '-')
			strvec_افزودن(&stash_آرگومانها, argv[i]);
		وإلا
			strvec_افزودن(&revision_آرگومانها, argv[i]);
	}

	إذا (get_stash_info(&info, stash_آرگومانها.nr, stash_آرگومانها.v))
		اذهب cleanup;

	/*
	 * The config settings are applied only if there are not passed
	 * any options.
	 */
	إذا (revision_آرگومانها.nr == 1) {
		إذا (show_stat)
			rev.diffopt.output_format = DIFF_FORMAT_DIFFSTAT;

		إذا (show_patch)
			rev.diffopt.output_format |= DIFF_FORMAT_PATCH;

		إذا (!show_stat && !show_patch) {
			ret = 0;
			اذهب cleanup;
		}
	}

	راهاندازی_revisions_from_strvec(&revision_آرگومانها, &rev, عدم);
	إذا (revision_آرگومانها.nr > 1)
		اذهب usage;
	إذا (!rev.diffopt.output_format) {
		rev.diffopt.output_format = DIFF_FORMAT_PATCH;
		diff_راهاندازی_انجامشده(&rev.diffopt);
	}

	rev.diffopt.flags.recursive = 1;
	راهاندازی_diff_pager(&rev.diffopt);
	بدل (show_untracked) {
	حالة UNTRACKED_NONE:
		diff_درخت_oid(&info.b_commit, &info.w_commit, "", &rev.diffopt);
		اكسر;
	حالة UNTRACKED_ONLY:
		إذا (info.has_u)
			diff_ریشه_درخت_oid(&info.u_درخت, "", &rev.diffopt);
		اكسر;
	حالة UNTRACKED_INCLUDE:
		إذا (info.has_u)
			diff_include_untracked(&info, &rev.diffopt);
		وإلا
			diff_درخت_oid(&info.b_commit, &info.w_commit, "", &rev.diffopt);
		اكسر;
	}
	log_درخت_diff_flush(&rev);

	ret = diff_نتیجه_کد(&rev);

cleanup:
	strvec_clear(&revision_آرگومانها);
	strvec_clear(&stash_آرگومانها);
	free_stash_info(&info);
	release_revisions(&rev);
	إذا (do_usage)
		usage_with_options(git_stash_show_usage, options);
	أرجع ret;
usage:
	do_usage = 1;
	اذهب cleanup;
}

ساكن صحيح do_store_stash(ثابت بنية object_id *w_commit, ثابت حرف *stash_پیام,
			  صحيح quiet)
{
	بنية stash_info info;
	حرف revision[GIT_MAX_HEXSZ];

	oid_مقصد_hex_r(revision, w_commit);
	assert_stash_like(&info, revision);

	إذا (!stash_پیام)
		stash_پیام = "Created via \"git stash store\".";

	إذا (refs_update_ref(get_main_ref_store(the_repository), stash_پیام, ref_stash, w_commit, عدم,
			    REF_FORCE_CREATE_REFLOG,
			    quiet ? UPDATE_REFS_QUIET_ON_ERR :
			    UPDATE_REFS_پیام_ON_ERR)) {
		إذا (!quiet) {
			fprintf_ln(stderr, _("Cannot update %s with %s"),
				   ref_stash, oid_مقصد_hex(w_commit));
		}
		أرجع -1;
	}

	أرجع 0;
}

ساكن صحيح store_stash(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
		       بنية repository *repo استفادهنشده)
{
	صحيح quiet = 0;
	ثابت حرف *stash_پیام = عدم;
	بنية object_id obj;
	بنية option options[] = {
		OPT__QUIET(&quiet, N_("be quiet")),
		OPT_STRING('m', "message", &stash_پیام, "message",
			   N_("stash message")),
		OPT_انتها()
	};
	صحيح ret;

	argc = parse_options(argc, argv, پیشوند, options,
			     git_stash_store_usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT);

	إذا (argc != 1) {
		إذا (!quiet)
			fprintf_ln(stderr, _("\"git stash store\" requires one "
					     "<commit> argument"));
		أرجع -1;
	}

	إذا (repo_get_oid_with_flags(the_repository, argv[0], &obj,
				    quiet ? GET_OID_QUIETLY : 0)) {
		إذا (!quiet)
			fprintf_ln(stderr, _("Cannot update %s with %s"),
					     ref_stash, argv[0]);
		ret = -1;
		اذهب بیرون;
	}

	ret = do_store_stash(&obj, stash_پیام, quiet);

بیرون:
	أرجع ret;
}

ساكن فراغ جمع_کن_pathspecs(بنية strvec *آرگومانها,
			  ثابت بنية pathspec *ps) {
	صحيح i;

	لكل (i = 0; i < ps->nr; i++)
		strvec_افزودن(آرگومانها, ps->اقلام[i].original);
}

/*
 * `untracked_files` will be filled with the names of untracked files.
 * The return value is:
 *
 * = 0 if there are not any untracked files
 * > 0 if there are untracked files
 */
ساكن صحيح get_untracked_files(ثابت بنية pathspec *ps, صحيح include_untracked,
			       بنية strbuf *untracked_files)
{
	صحيح i;
	صحيح یافت_2 = 0;
	بنية dir_struct dir = DIR_INIT;

	إذا (include_untracked != INCLUDE_ALL_FILES)
		راهاندازی_standard_excludes(&dir);

	fill_directory(&dir, the_repository->شاخص, ps);
	لكل (i = 0; i < dir.nr; i++) {
		بنية dir_مدخل *ent = dir.entries[i];
		یافت_2++;
		strbuf_addstr(untracked_files, ent->name);
		/* NUL-terminate: will be fed to update-index -z */
		strbuf_addch(untracked_files, '\0');
	}

	dir_clear(&dir);
	أرجع یافت_2;
}

/*
 * The return value of `check_changes_tracked_files()` can be:
 *
 * < 0 if there was an error
 * = 0 if there are no changes.
 * > 0 if there are changes.
 */
ساكن صحيح check_changes_tracked_files(ثابت بنية pathspec *ps)
{
	بنية rev_info rev;
	بنية object_id dummy;
	صحيح ret = 0;

	/* No initial commit. */
	إذا (repo_get_oid(the_repository, "HEAD", &dummy))
		أرجع -1;

	إذا (repo_read_شاخص(the_repository) < 0)
		أرجع -1;

	repo_init_revisions(the_repository, &rev, عدم);
	copy_pathspec(&rev.prune_داده, ps);

	rev.diffopt.flags.سریع = 1;
	rev.diffopt.flags.ignore_submodules = 1;
	rev.abbrev = 0;

	جمع_کن_سر_مقصد_pending(&rev);
	diff_راهاندازی_انجامشده(&rev.diffopt);

	پیاپی_diff_شاخص(&rev, DIFF_شاخص_CACHED);
	إذا (diff_نتیجه_کد(&rev)) {
		ret = 1;
		اذهب انجامشده;
	}

	پیاپی_diff_files(&rev, 0);
	إذا (diff_نتیجه_کد(&rev)) {
		ret = 1;
		اذهب انجامشده;
	}

انجامشده:
	release_revisions(&rev);
	أرجع ret;
}

/*
 * The function will fill `untracked_files` with the names of untracked files
 * It will return 1 if there were any changes and 0 if there were not.
 */
ساكن صحيح check_changes(ثابت بنية pathspec *ps, صحيح include_untracked,
			 بنية strbuf *untracked_files)
{
	صحيح ret = 0;
	إذا (check_changes_tracked_files(ps))
		ret = 1;

	إذا (include_untracked && get_untracked_files(ps, include_untracked,
						     untracked_files))
		ret = 1;

	أرجع ret;
}

ساكن صحيح save_untracked_files(بنية stash_info *info, بنية strbuf *پیام,
				بنية strbuf *files)
{
	صحيح ret = 0;
	بنية strbuf untracked_پیام = STRBUF_INIT;
	بنية فرزند_process cp_upd_شاخص = فرزند_PROCESS_INIT;
	بنية شاخص_state istate = شاخص_STATE_INIT(the_repository);

	cp_upd_شاخص.git_cmd = 1;
	strvec_pushl(&cp_upd_شاخص.آرگومانها, "update-index", "-z", "--add",
		     "--remove", "--stdin", عدم);
	strvec_pushf(&cp_upd_شاخص.env, "GIT_INDEX_FILE=%s",
			 stash_شاخص_مسیر.buf);

	strbuf_addf(&untracked_پیام, "untracked files on %s\n", پیام->buf);
	إذا (pipe_command(&cp_upd_شاخص, files->buf, files->len, عدم, 0,
			 عدم, 0)) {
		ret = -1;
		اذهب انجامشده;
	}

	إذا (write_شاخص_as_درخت(&info->u_درخت, &istate, stash_شاخص_مسیر.buf, 0,
				عدم)) {
		ret = -1;
		اذهب انجامشده;
	}

	إذا (commit_درخت(untracked_پیام.buf, untracked_پیام.len,
			&info->u_درخت, عدم, &info->u_commit, عدم, عدم)) {
		ret = -1;
		اذهب انجامشده;
	}

انجامشده:
	release_شاخص(&istate);
	strbuf_release(&untracked_پیام);
	remove_مسیر(stash_شاخص_مسیر.buf);
	أرجع ret;
}

ساكن صحيح stash_staged(بنية stash_info *info, بنية strbuf *بیرون_patch,
			صحيح quiet)
{
	صحيح ret = 0;
	بنية فرزند_process cp_diff_درخت = فرزند_PROCESS_INIT;
	بنية شاخص_state istate = شاخص_STATE_INIT(the_repository);

	إذا (write_شاخص_as_درخت(&info->w_درخت, &istate, the_repository->شاخص_file,
				0, عدم)) {
		ret = -1;
		اذهب انجامشده;
	}

	cp_diff_درخت.git_cmd = 1;
	strvec_pushl(&cp_diff_درخت.آرگومانها, "diff-tree", "-p", "--binary",
		     "--no-color",
		     "-U1", "HEAD", oid_مقصد_hex(&info->w_درخت), "--", عدم);
	إذا (pipe_command(&cp_diff_درخت, عدم, 0, بیرون_patch, 0, عدم, 0)) {
		ret = -1;
		اذهب انجامشده;
	}

	إذا (!بیرون_patch->len) {
		إذا (!quiet)
			fprintf_ln(stderr, _("No staged changes"));
		ret = 1;
	}

انجامشده:
	release_شاخص(&istate);
	أرجع ret;
}

ساكن صحيح stash_patch(بنية stash_info *info, ثابت بنية pathspec *ps,
		       بنية strbuf *بیرون_patch, صحيح quiet,
		       بنية interactive_options *interactive_opts)
{
	صحيح ret = 0;
	بنية فرزند_process cp_diff_درخت = فرزند_PROCESS_INIT;
	بنية commit *سر_commit;
	ثابت بنية object_id *سر_درخت;
	بنية شاخص_state istate = شاخص_STATE_INIT(the_repository);
	حرف *old_شاخص_env = عدم, *old_repo_شاخص_file;

	remove_مسیر(stash_شاخص_مسیر.buf);

	سر_commit = lookup_commit(the_repository, &info->b_commit);
	إذا (!سر_commit || repo_parse_commit(the_repository, سر_commit)) {
		ret = -1;
		اذهب انجامشده;
	}
	سر_درخت = get_commit_درخت_oid(سر_commit);
	إذا (!سر_درخت) {
		ret = -1;
		اذهب انجامشده;
	}

	إذا (create_شاخص_from_درخت(سر_درخت, stash_شاخص_مسیر.buf)) {
		ret = -1;
		اذهب انجامشده;
	}

	/* Find out what the user wants. */
	old_repo_شاخص_file = the_repository->شاخص_file;
	the_repository->شاخص_file = stash_شاخص_مسیر.buf;
	old_شاخص_env = xstrdup_or_null(getenv(شاخص_ENVIRONMENT));
	setenv(شاخص_ENVIRONMENT, the_repository->شاخص_file, 1);

	ret = !!پیاپی_جمع_کن_p(the_repository, جمع_کن_P_STASH, interactive_opts, عدم, ps, 0);

	the_repository->شاخص_file = old_repo_شاخص_file;
	إذا (old_شاخص_env && *old_شاخص_env)
		setenv(شاخص_ENVIRONMENT, old_شاخص_env, 1);
	وإلا
		unsetenv(شاخص_ENVIRONMENT);
	FREE_AND_NULL(old_شاخص_env);

	/* State of the working tree. */
	إذا (write_شاخص_as_درخت(&info->w_درخت, &istate, stash_شاخص_مسیر.buf, 0,
				عدم)) {
		ret = -1;
		اذهب انجامشده;
	}

	cp_diff_درخت.git_cmd = 1;
	strvec_pushl(&cp_diff_درخت.آرگومانها, "diff-tree", "-p", "-U1", "HEAD",
		     "--no-color",
		     oid_مقصد_hex(&info->w_درخت), "--", عدم);
	إذا (pipe_command(&cp_diff_درخت, عدم, 0, بیرون_patch, 0, عدم, 0)) {
		ret = -1;
		اذهب انجامشده;
	}

	إذا (!بیرون_patch->len) {
		إذا (!quiet)
			fprintf_ln(stderr, _("No changes selected"));
		ret = 1;
	}

انجامشده:
	release_شاخص(&istate);
	remove_مسیر(stash_شاخص_مسیر.buf);
	أرجع ret;
}

ساكن صحيح stash_working_درخت(بنية stash_info *info, ثابت بنية pathspec *ps)
{
	صحيح ret = 0;
	بنية rev_info rev;
	بنية فرزند_process cp_upd_شاخص = فرزند_PROCESS_INIT;
	بنية strbuf diff_output = STRBUF_INIT;
	بنية شاخص_state istate = شاخص_STATE_INIT(the_repository);

	repo_init_revisions(the_repository, &rev, عدم);
	copy_pathspec(&rev.prune_داده, ps);

	مجموعه_alternate_شاخص_output(stash_شاخص_مسیر.buf);
	إذا (reset_درخت(&info->i_درخت, 0, 0)) {
		ret = -1;
		اذهب انجامشده;
	}
	مجموعه_alternate_شاخص_output(عدم);

	rev.diffopt.output_format = DIFF_FORMAT_CALLBACK;
	rev.diffopt.format_callback = جمع_کن_diff_مقصد_buf;
	rev.diffopt.format_callback_داده = &diff_output;

	إذا (repo_read_شاخص_preload(the_repository, &rev.diffopt.pathspec, 0) < 0) {
		ret = -1;
		اذهب انجامشده;
	}

	جمع_کن_pending_object(&rev, parse_object(the_repository, &info->b_commit),
			   "");
	پیاپی_diff_شاخص(&rev, 0);

	cp_upd_شاخص.git_cmd = 1;
	strvec_pushl(&cp_upd_شاخص.آرگومانها, "update-index",
		     "--ignore-skip-worktree-entries",
		     "-z", "--add", "--remove", "--stdin", عدم);
	strvec_pushf(&cp_upd_شاخص.env, "GIT_INDEX_FILE=%s",
		     stash_شاخص_مسیر.buf);

	إذا (pipe_command(&cp_upd_شاخص, diff_output.buf, diff_output.len,
			 عدم, 0, عدم, 0)) {
		ret = -1;
		اذهب انجامشده;
	}

	إذا (write_شاخص_as_درخت(&info->w_درخت, &istate, stash_شاخص_مسیر.buf, 0,
				عدم)) {
		ret = -1;
		اذهب انجامشده;
	}

انجامشده:
	release_شاخص(&istate);
	release_revisions(&rev);
	strbuf_release(&diff_output);
	remove_مسیر(stash_شاخص_مسیر.buf);
	أرجع ret;
}

ساكن صحيح do_create_stash(ثابت بنية pathspec *ps, بنية strbuf *stash_پیام_buf,
			   صحيح include_untracked, صحيح patch_نما,
			   بنية interactive_options *interactive_opts,
			   صحيح only_staged, بنية stash_info *info, بنية strbuf *patch,
			   صحيح quiet)
{
	صحيح ret = 0;
	صحيح flags = 0;
	صحيح untracked_commit_option = 0;
	ثابت حرف *سر_short_sha1 = عدم;
	ثابت حرف *branch_ref = عدم;
	ثابت حرف *branch_name = "(no branch)";
	حرف *branch_name_buf = عدم;
	بنية commit *سر_commit = عدم;
	بنية commit_list *parents = عدم;
	بنية strbuf پیام = STRBUF_INIT;
	بنية strbuf commit_درخت_label = STRBUF_INIT;
	بنية strbuf untracked_files = STRBUF_INIT;

	prepare_fallback_ident("git stash", "git@stash");

	repo_read_شاخص_preload(the_repository, عدم, 0);
	إذا (repo_refresh_and_write_شاخص(the_repository, REFRESH_QUIET, 0, 0,
					 عدم, عدم, عدم) < 0) {
		ret = error(_("could not write index"));
		اذهب انجامشده;
	}

	إذا (repo_get_oid(the_repository, "HEAD", &info->b_commit)) {
		إذا (!quiet)
			fprintf_ln(stderr, _("You do not have "
					     "the initial commit yet"));
		ret = -1;
		اذهب انجامشده;
	} وإلا {
		سر_commit = lookup_commit(the_repository, &info->b_commit);
	}

	إذا (!check_changes(ps, include_untracked, &untracked_files)) {
		ret = 1;
		اذهب انجامشده;
	}

	branch_ref = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
					     "HEAD", 0, عدم, &flags);

	إذا (flags & REF_ISSYMREF) {
		إذا (skip_پیشوند(branch_ref, "refs/heads/", &branch_name))
			branch_name = branch_name_buf = xstrdup(branch_name);
	}

	سر_short_sha1 = repo_پیدا_کن_unique_abbrev(the_repository,
						  &سر_commit->object.oid,
						  DEFAULT_ABBREV);
	strbuf_addf(&پیام, "%s: %s ", branch_name, سر_short_sha1);
	pp_commit_easy(CMIT_FMT_ONELINE, سر_commit, &پیام);

	strbuf_addf(&commit_درخت_label, "index on %s\n", پیام.buf);
	commit_list_درج(سر_commit, &parents);
	إذا (write_شاخص_as_درخت(&info->i_درخت, the_repository->شاخص,
				repo_get_شاخص_file(the_repository), 0, عدم) ||
	    commit_درخت(commit_درخت_label.buf, commit_درخت_label.len,
			&info->i_درخت, parents, &info->i_commit, عدم, عدم)) {
		إذا (!quiet)
			fprintf_ln(stderr, _("Cannot save the current "
					     "index state"));
		ret = -1;
		اذهب انجامشده;
	}

	commit_list_free(parents);
	parents = عدم;

	إذا (include_untracked) {
		إذا (save_untracked_files(info, &پیام, &untracked_files)) {
			إذا (!quiet)
				fprintf_ln(stderr, _("Cannot save "
						     "the untracked files"));
			ret = -1;
			اذهب انجامشده;
		}
		untracked_commit_option = 1;
	}
	إذا (patch_نما) {
		ret = stash_patch(info, ps, patch, quiet, interactive_opts);
		إذا (ret < 0) {
			إذا (!quiet)
				fprintf_ln(stderr, _("Cannot save the current "
						     "worktree state"));
			اذهب انجامشده;
		} وإلا إذا (ret > 0) {
			اذهب انجامشده;
		}
	} وإلا إذا (only_staged) {
		ret = stash_staged(info, patch, quiet);
		إذا (ret < 0) {
			إذا (!quiet)
				fprintf_ln(stderr, _("Cannot save the current "
						     "staged state"));
			اذهب انجامشده;
		} وإلا إذا (ret > 0) {
			اذهب انجامشده;
		}
	} وإلا {
		إذا (stash_working_درخت(info, ps)) {
			إذا (!quiet)
				fprintf_ln(stderr, _("Cannot save the current "
						     "worktree state"));
			ret = -1;
			اذهب انجامشده;
		}
	}

	إذا (!stash_پیام_buf->len)
		strbuf_addf(stash_پیام_buf, "WIP on %s", پیام.buf);
	وإلا
		strbuf_insertf(stash_پیام_buf, 0, "On %s: ", branch_name);

	إذا (untracked_commit_option)
		commit_list_درج(lookup_commit(the_repository,
						 &info->u_commit),
				   &parents);
	commit_list_درج(lookup_commit(the_repository, &info->i_commit),
			   &parents);
	commit_list_درج(سر_commit, &parents);

	إذا (commit_درخت(stash_پیام_buf->buf, stash_پیام_buf->len, &info->w_درخت,
			parents, &info->w_commit, عدم, عدم)) {
		إذا (!quiet)
			fprintf_ln(stderr, _("Cannot record "
					     "working tree state"));
		ret = -1;
		اذهب انجامشده;
	}

انجامشده:
	strbuf_release(&commit_درخت_label);
	strbuf_release(&پیام);
	strbuf_release(&untracked_files);
	commit_list_free(parents);
	حرر(branch_name_buf);
	أرجع ret;
}

ساكن صحيح create_stash(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند استفادهنشده,
			بنية repository *repo استفادهنشده)
{
	صحيح ret;
	بنية strbuf stash_پیام_buf = STRBUF_INIT;
	بنية stash_info info = STASH_INFO_INIT;
	بنية pathspec ps;

	/* Starting with argv[1], since argv[0] is "create" */
	strbuf_join_argv(&stash_پیام_buf, argc - 1, ++argv, ' ');

	memset(&ps, 0, حجم(ps));
	إذا (!check_changes_tracked_files(&ps))
		أرجع 0;

	ret = do_create_stash(&ps, &stash_پیام_buf, 0, 0, عدم, 0, &info,
			      عدم, 0);
	إذا (!ret)
		printf_ln("%s", oid_مقصد_hex(&info.w_commit));

	free_stash_info(&info);
	strbuf_release(&stash_پیام_buf);
	أرجع ret;
}

ساكن صحيح do_افزودن_stash(ثابت بنية pathspec *ps, ثابت حرف *stash_پیام, صحيح quiet,
			 صحيح keep_شاخص, صحيح patch_نما,
			 بنية interactive_options *interactive_opts,
			 صحيح include_untracked, صحيح only_staged)
{
	صحيح ret = 0;
	بنية stash_info info = STASH_INFO_INIT;
	بنية strbuf patch = STRBUF_INIT;
	بنية strbuf stash_پیام_buf = STRBUF_INIT;
	بنية strbuf untracked_files = STRBUF_INIT;
	بنية strbuf بیرون = STRBUF_INIT;

	إذا (patch_نما && keep_شاخص == -1)
		keep_شاخص = 1;

	إذا (patch_نما && include_untracked) {
		fprintf_ln(stderr, _("Can't use --patch and --include-untracked"
				     " or --all at the same time"));
		ret = -1;
		اذهب انجامشده;
	}

	/* --patch overrides --staged */
	إذا (patch_نما)
		only_staged = 0;

	إذا (only_staged && include_untracked) {
		fprintf_ln(stderr, _("Can't use --staged and --include-untracked"
				     " or --all at the same time"));
		ret = -1;
		اذهب انجامشده;
	}

	repo_read_شاخص_preload(the_repository, عدم, 0);
	إذا (!include_untracked && ps->nr) {
		حرف *ps_matched = xcalloc(ps->nr, 1);

		إذا (pathspec_needs_expanded_شاخص(the_repository->شاخص, ps))
			ensure_پر_شاخص(the_repository->شاخص);
		لكل (اندازه_t i = 0; i < the_repository->شاخص->cache_nr; i++)
			ce_مسیر_تطابق(the_repository->شاخص, the_repository->شاخص->cache[i], ps,
				      ps_matched);

		إذا (report_مسیر_error(ps_matched, ps)) {
			fprintf_ln(stderr, _("Did you forget to 'git add'?"));
			ret = -1;
			حرر(ps_matched);
			اذهب انجامشده;
		}
		حرر(ps_matched);
	}

	إذا (repo_refresh_and_write_شاخص(the_repository, REFRESH_QUIET, 0, 0,
					 عدم, عدم, عدم)) {
		ret = error(_("could not write index"));
		اذهب انجامشده;
	}

	إذا (!check_changes(ps, include_untracked, &untracked_files)) {
		إذا (!quiet)
			printf_ln(_("No local changes to save"));
		اذهب انجامشده;
	}

	إذا (!refs_reflog_exists(get_main_ref_store(the_repository), ref_stash) && do_clear_stash()) {
		ret = -1;
		إذا (!quiet)
			fprintf_ln(stderr, _("Cannot initialize stash"));
		اذهب انجامشده;
	}

	إذا (stash_پیام)
		strbuf_addstr(&stash_پیام_buf, stash_پیام);
	إذا (do_create_stash(ps, &stash_پیام_buf, include_untracked, patch_نما,
			    interactive_opts, only_staged, &info, &patch, quiet)) {
		ret = -1;
		اذهب انجامشده;
	}

	إذا (do_store_stash(&info.w_commit, stash_پیام_buf.buf, 1)) {
		ret = -1;
		إذا (!quiet)
			fprintf_ln(stderr, _("Cannot save the current status"));
		اذهب انجامشده;
	}

	إذا (!quiet)
		printf_ln(_("Saved working directory and index state %s"),
			  stash_پیام_buf.buf);

	إذا (!(patch_نما || only_staged)) {
		إذا (include_untracked && !ps->nr) {
			بنية فرزند_process cp = فرزند_PROCESS_INIT;

			cp.git_cmd = 1;
			إذا (startup_info->original_cwd) {
				cp.dir = startup_info->original_cwd;
				strvec_pushf(&cp.env, "%s=%s",
					     GIT_WORK_درخت_ENVIRONMENT,
					     the_repository->worktree);
			}
			strvec_pushl(&cp.آرگومانها, "clean", "--force",
				     "--quiet", "-d", ":/", عدم);
			إذا (include_untracked == INCLUDE_ALL_FILES)
				strvec_افزودن(&cp.آرگومانها, "-x");
			إذا (پیاپی_command(&cp)) {
				ret = -1;
				اذهب انجامشده;
			}
		}
		discard_شاخص(the_repository->شاخص);
		إذا (ps->nr) {
			بنية فرزند_process cp_جمع_کن = فرزند_PROCESS_INIT;
			بنية فرزند_process cp_diff = فرزند_PROCESS_INIT;
			بنية فرزند_process cp_apply = فرزند_PROCESS_INIT;

			cp_جمع_کن.git_cmd = 1;
			strvec_افزودن(&cp_جمع_کن.آرگومانها, "add");
			إذا (!include_untracked)
				strvec_افزودن(&cp_جمع_کن.آرگومانها, "-u");
			إذا (include_untracked == INCLUDE_ALL_FILES)
				strvec_افزودن(&cp_جمع_کن.آرگومانها, "--force");
			strvec_افزودن(&cp_جمع_کن.آرگومانها, "--");
			جمع_کن_pathspecs(&cp_جمع_کن.آرگومانها, ps);
			إذا (پیاپی_command(&cp_جمع_کن)) {
				ret = -1;
				اذهب انجامشده;
			}

			cp_diff.git_cmd = 1;
			strvec_pushl(&cp_diff.آرگومانها, "diff-index", "-p",
				     "--no-color",
				     "--cached", "--binary", "HEAD", "--",
				     عدم);
			جمع_کن_pathspecs(&cp_diff.آرگومانها, ps);
			إذا (pipe_command(&cp_diff, عدم, 0, &بیرون, 0, عدم, 0)) {
				ret = -1;
				اذهب انجامشده;
			}

			cp_apply.git_cmd = 1;
			strvec_pushl(&cp_apply.آرگومانها, "apply", "--index",
				     "-R", عدم);
			إذا (pipe_command(&cp_apply, بیرون.buf, بیرون.len, عدم, 0,
					 عدم, 0)) {
				ret = -1;
				اذهب انجامشده;
			}
		} وإلا {
			بنية فرزند_process cp = فرزند_PROCESS_INIT;
			cp.git_cmd = 1;
			/* BUG: this nukes untracked files in the way */
			strvec_pushl(&cp.آرگومانها, "reset", "--hard", "-q",
				     "--no-recurse-submodules", عدم);
			إذا (پیاپی_command(&cp)) {
				ret = -1;
				اذهب انجامشده;
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
		إذا (keep_شاخص == 1 && !is_null_oid(&info.i_درخت) &&
		    !is_خالی_درخت_oid(&info.i_درخت, the_repository->hash_algo)) {
			بنية فرزند_process cp = فرزند_PROCESS_INIT;

			cp.git_cmd = 1;
			strvec_pushl(&cp.آرگومانها, "checkout", "--no-overlay",
				     oid_مقصد_hex(&info.i_درخت), "--", عدم);
			إذا (!ps->nr)
				strvec_افزودن(&cp.آرگومانها, ":/");
			وإلا
				جمع_کن_pathspecs(&cp.آرگومانها, ps);
			إذا (پیاپی_command(&cp)) {
				ret = -1;
				اذهب انجامشده;
			}
		}
		اذهب انجامشده;
	} وإلا {
		بنية فرزند_process cp = فرزند_PROCESS_INIT;

		cp.git_cmd = 1;
		strvec_pushl(&cp.آرگومانها, "apply", "-R", عدم);

		إذا (pipe_command(&cp, patch.buf, patch.len, عدم, 0, عدم, 0)) {
			إذا (!quiet)
				fprintf_ln(stderr, _("Cannot remove "
						     "worktree changes"));
			ret = -1;
			اذهب انجامشده;
		}

		إذا (keep_شاخص < 1) {
			بنية فرزند_process cp = فرزند_PROCESS_INIT;

			cp.git_cmd = 1;
			strvec_pushl(&cp.آرگومانها, "reset", "-q", "--refresh", "--",
				     عدم);
			جمع_کن_pathspecs(&cp.آرگومانها, ps);
			إذا (پیاپی_command(&cp)) {
				ret = -1;
				اذهب انجامشده;
			}
		}
		اذهب انجامشده;
	}

انجامشده:
	strbuf_release(&patch);
	strbuf_release(&بیرون);
	free_stash_info(&info);
	strbuf_release(&stash_پیام_buf);
	strbuf_release(&untracked_files);
	أرجع ret;
}

ساكن صحيح افزودن_stash(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
		      صحيح افزودن_assumed)
{
	صحيح force_assume = 0;
	صحيح keep_شاخص = -1;
	صحيح only_staged = 0;
	صحيح patch_نما = 0;
	صحيح include_untracked = 0;
	صحيح quiet = 0;
	صحيح pathspec_file_nul = 0;
	ثابت حرف *stash_پیام = عدم;
	حرف *pathspec_from_file = عدم;
	بنية pathspec ps;
	بنية interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	بنية option options[] = {
		OPT_BOOL('k', "keep-index", &keep_شاخص,
			 N_("keep index")),
		OPT_BOOL('S', "staged", &only_staged,
			 N_("stash staged changes only")),
		OPT_BOOL('p', "patch", &patch_نما,
			 N_("stash in patch mode")),
		OPT_BOOL(0, "auto-advance", &interactive_opts.auto_advance,
			 N_("auto advance to the next file when selecting hunks interactively")),
		OPT_DIFF_UNIFIED(&interactive_opts.context),
		OPT_DIFF_INTERHUNK_CONTEXT(&interactive_opts.interhunkcontext),
		OPT__QUIET(&quiet, N_("quiet mode")),
		OPT_BOOL('u', "include-untracked", &include_untracked,
			 N_("include untracked files in stash")),
		OPT_مجموعه_INT('a', "all", &include_untracked,
			    N_("include ignore files"), INCLUDE_ALL_FILES),
		OPT_STRING('m', "message", &stash_پیام, N_("message"),
			   N_("stash message")),
		OPT_PATHSPEC_FROM_FILE(&pathspec_from_file),
		OPT_PATHSPEC_FILE_NUL(&pathspec_file_nul),
		OPT_انتها()
	};
	صحيح ret;

	إذا (argc) {
		صحيح flags = PARSE_OPT_KEEP_DASHDASH;

		إذا (افزودن_assumed)
			flags |= PARSE_OPT_STOP_AT_NON_OPTION;

		argc = parse_options(argc, argv, پیشوند, options,
				     افزودن_assumed ? git_stash_usage :
				     git_stash_افزودن_usage, flags);
		force_assume |= patch_نما;
	}

	إذا (argc) {
		إذا (!strcmp(argv[0], "--")) {
			argc--;
			argv++;
		} وإلا إذا (افزودن_assumed && !force_assume) {
			die("subcommand wasn't specified; 'push' can't be assumed due to unexpected token '%s'",
			    argv[0]);
		}
	}

	parse_pathspec(&ps, 0, PATHSPEC_PREFER_پر | PATHSPEC_پیشوند_ORIGIN,
		       پیشوند, argv);

	إذا (pathspec_from_file) {
		إذا (patch_نما)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--patch");

		إذا (only_staged)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--staged");

		إذا (ps.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&ps, 0,
				    PATHSPEC_PREFER_پر | PATHSPEC_پیشوند_ORIGIN,
				    پیشوند, pathspec_from_file, pathspec_file_nul);
	} وإلا إذا (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	إذا (!patch_نما) {
		إذا (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--patch");
		إذا (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--patch");
		إذا (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--patch");
	}

	إذا (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	إذا (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	ret = do_افزودن_stash(&ps, stash_پیام, quiet, keep_شاخص, patch_نما,
			    &interactive_opts, include_untracked, only_staged);

	clear_pathspec(&ps);
	حرر(pathspec_from_file);
	أرجع ret;
}

ساكن صحيح افزودن_stash_unassumed(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
				بنية repository *repo استفادهنشده)
{
	أرجع افزودن_stash(argc, argv, پیشوند, 0);
}

ساكن صحيح save_stash(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
		      بنية repository *repo استفادهنشده)
{
	صحيح keep_شاخص = -1;
	صحيح only_staged = 0;
	صحيح patch_نما = 0;
	صحيح include_untracked = 0;
	صحيح quiet = 0;
	صحيح ret = 0;
	ثابت حرف *stash_پیام = عدم;
	بنية pathspec ps;
	بنية strbuf stash_پیام_buf = STRBUF_INIT;
	بنية interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	بنية option options[] = {
		OPT_BOOL('k', "keep-index", &keep_شاخص,
			 N_("keep index")),
		OPT_BOOL('S', "staged", &only_staged,
			 N_("stash staged changes only")),
		OPT_BOOL('p', "patch", &patch_نما,
			 N_("stash in patch mode")),
		OPT_BOOL(0, "auto-advance", &interactive_opts.auto_advance,
			 N_("auto advance to the next file when selecting hunks interactively")),
		OPT_DIFF_UNIFIED(&interactive_opts.context),
		OPT_DIFF_INTERHUNK_CONTEXT(&interactive_opts.interhunkcontext),
		OPT__QUIET(&quiet, N_("quiet mode")),
		OPT_BOOL('u', "include-untracked", &include_untracked,
			 N_("include untracked files in stash")),
		OPT_مجموعه_INT('a', "all", &include_untracked,
			    N_("include ignore files"), INCLUDE_ALL_FILES),
		OPT_STRING('m', "message", &stash_پیام, "message",
			   N_("stash message")),
		OPT_انتها()
	};

	argc = parse_options(argc, argv, پیشوند, options,
			     git_stash_save_usage,
			     PARSE_OPT_KEEP_DASHDASH);

	إذا (argc)
		stash_پیام = strbuf_join_argv(&stash_پیام_buf, argc, argv, ' ');

	memset(&ps, 0, حجم(ps));

	إذا (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	إذا (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	إذا (!patch_نما) {
		إذا (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--patch");
		إذا (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--patch");
		إذا (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--patch");
	}

	ret = do_افزودن_stash(&ps, stash_پیام, quiet, keep_شاخص,
			    patch_نما, &interactive_opts, include_untracked,
			    only_staged);

	strbuf_release(&stash_پیام_buf);
	أرجع ret;
}

ساكن صحيح write_commit_with_parents(بنية repository *r,
				     بنية object_id *بیرون,
				     ثابت بنية object_id *oid,
				     بنية commit_list *parents)
{
	اندازه_t author_len, committer_len;
	بنية commit *this;
	ثابت حرف *orig_author, *orig_committer;
	حرف *author = عدم, *committer = عدم;
	ثابت حرف *میانگیر;
	بیعلامت دراز bufsize;
	ثابت حرف *p;
	بنية strbuf پیام = STRBUF_INIT;
	صحيح ret = 0;
	بنية ident_تقسیم id;

	this = lookup_commit_reference(r, oid);
	میانگیر = repo_get_commit_میانگیر(r, this, &bufsize);
	orig_author = پیدا_کن_commit_سرآیند(میانگیر, "author", &author_len);
	orig_committer = پیدا_کن_commit_سرآیند(میانگیر, "committer", &committer_len);

	إذا (!orig_author || !orig_committer) {
		ret = error(_("cannot parse commit %s"), oid_مقصد_hex(oid));
		اذهب بیرون;
	}

	إذا (تقسیم_ident_سطر(&id, orig_author, author_len) < 0 ||
	    تقسیم_ident_سطر(&id, orig_committer, committer_len) < 0) {
		ret = error(_("invalid author or committer for %s"), oid_مقصد_hex(oid));
		اذهب بیرون;
	}

	p = جستجوی_رشته(میانگیر, "\n\n");
	strbuf_addstr(&پیام, "git stash: ");

	إذا (p)
		strbuf_جمع_کن(&پیام, p + 2, bufsize - (p + 2 - میانگیر));
	strbuf_complete_سطر(&پیام);

	author = xmemdupz(orig_author, author_len);
	committer = xmemdupz(orig_committer, committer_len);

	إذا (commit_درخت_extended(پیام.buf, پیام.len,
				 r->hash_algo->خالی_درخت, parents,
				 بیرون, author, committer,
				 عدم, عدم)) {
		ret = error(_("could not write commit"));
		اذهب بیرون;
	}
بیرون:
	strbuf_release(&پیام);
	repo_unuse_commit_میانگیر(r, this, میانگیر);
	حرر(author);
	حرر(committer);
	أرجع ret;
}

ساكن صحيح do_import_stash(بنية repository *r, ثابت حرف *rev)
{
	بنية object_id chain;
	صحيح res = 0;
	ثابت حرف *میانگیر = عدم;
	بیعلامت دراز bufsize;
	بنية commit *this = عدم;
	بنية commit_list *اقلام = عدم, *cur;
	حرف *پیام = عدم;

	إذا (repo_get_oid(r, rev, &chain))
		أرجع error(_("not a valid revision: %s"), rev);

	this = lookup_commit_reference(r, &chain);
	إذا (!this)
		أرجع error(_("not a commit: %s"), rev);

	/*
	 * Walk the commit history, finding each stash entry, and load data into
	 * the array.
	 */
	لكل (;;) {
		ثابت حرف *author, *committer;
		اندازه_t author_len, committer_len;
		ثابت حرف *p;
		ثابت حرف *expected = "git stash <git@stash> 1000684800 +0000";
		ثابت حرف *پیشوند = "git stash: ";
		بنية commit *stash;
		بنية درخت *درخت = repo_get_commit_درخت(r, this);

		إذا (!درخت ||
		    !oideq(&درخت->object.oid, r->hash_algo->خالی_درخت) ||
		    (this->parents &&
		     (!this->parents->بعدی || this->parents->بعدی->بعدی))) {
			res = error(_("%s is not a valid exported stash commit"),
					oid_مقصد_hex(&this->object.oid));
			اذهب بیرون;
		}

		میانگیر = repo_get_commit_میانگیر(r, this, &bufsize);

		إذا (!this->parents) {
			/*
			 * We don't have any parents.  Make sure this is our
			 * root commit.
			 */
			author = پیدا_کن_commit_سرآیند(میانگیر, "author", &author_len);
			committer = پیدا_کن_commit_سرآیند(میانگیر, "committer", &committer_len);

			إذا (!author || !committer) {
				error(_("cannot parse commit %s"), oid_مقصد_hex(&this->object.oid));
				اذهب بیرون;
			}

			إذا (author_len != طول_رشته(expected) ||
			    committer_len != طول_رشته(expected) ||
			    memcmp(author, expected, author_len) ||
			    memcmp(committer, expected, committer_len)) {
				res = error(_("found root commit %s with invalid data"), oid_مقصد_hex(&this->object.oid));
				اذهب بیرون;
			}
			اكسر;
		}

		p = جستجوی_رشته(میانگیر, "\n\n");
		إذا (!p) {
			res = error(_("cannot parse commit %s"), oid_مقصد_hex(&this->object.oid));
			اذهب بیرون;
		}

		p += 2;
		إذا (((اندازه_t)(bufsize - (p - میانگیر)) < طول_رشته(پیشوند)) ||
		    memcmp(پیشوند, p, طول_رشته(پیشوند))) {
			res = error(_("found stash commit %s without expected prefix"), oid_مقصد_hex(&this->object.oid));
			اذهب بیرون;
		}

		stash = this->parents->بعدی->قلم;

		إذا (repo_parse_commit(r, this->parents->قلم) ||
		    repo_parse_commit(r, stash)) {
			res = error(_("cannot parse parents of commit: %s"),
					oid_مقصد_hex(&this->object.oid));
			اذهب بیرون;
		}

		إذا (check_stash_topology(r, stash)) {
			res = error(_("%s does not look like a stash commit"),
					oid_مقصد_hex(&stash->object.oid));
			اذهب بیرون;
		}

		repo_unuse_commit_میانگیر(r, this, میانگیر);
		میانگیر = عدم;
		اقلام = commit_list_درج(stash, &اقلام);
		this = this->parents->قلم;
	}

	/*
	 * Now, walk each entry, adding it to the stash as a normal stash
	 * commit.
	 */
	لكل (cur = اقلام; cur; cur = cur->بعدی) {
		ثابت حرف *p;
		بنية object_id *oid;

		this = cur->قلم;
		oid = &this->object.oid;
		میانگیر = repo_get_commit_میانگیر(r, this, &bufsize);
		إذا (!میانگیر) {
			res = error(_("cannot read commit buffer for %s"), oid_مقصد_hex(oid));
			اذهب بیرون;
		}

		p = جستجوی_رشته(میانگیر, "\n\n");
		إذا (!p) {
			res = error(_("cannot parse commit %s"), oid_مقصد_hex(oid));
			اذهب بیرون;
		}

		p += 2;
		پیام = xmemdupz(p, bufsize - (p - میانگیر));
		repo_unuse_commit_میانگیر(r, this, میانگیر);
		میانگیر = عدم;

		إذا (do_store_stash(oid, پیام, 1)) {
			res = error(_("cannot save the stash for %s"), oid_مقصد_hex(oid));
			اذهب بیرون;
		}
		FREE_AND_NULL(پیام);
	}
بیرون:
	إذا (this && میانگیر)
		repo_unuse_commit_میانگیر(r, this, میانگیر);
	commit_list_free(اقلام);
	حرر(پیام);

	أرجع res;
}

ساكن صحيح import_stash(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
			بنية repository *repo)
{
	بنية option options[] = {
		OPT_انتها()
	};

	argc = parse_options(argc, argv, پیشوند, options,
			     git_stash_import_usage,
			     PARSE_OPT_KEEP_DASHDASH);

	إذا (argc != 1)
		usage_پیام_opt("a revision is required", git_stash_import_usage, options);

	أرجع do_import_stash(repo, argv[0]);
}

بنية stash_مدخل_داده {
	بنية repository *r;
	بنية commit_list **اقلام;
	اندازه_t شمار;
};

ساكن صحيح collect_stash_entries(ثابت حرف *refname استفادهنشده,
				 بنية object_id *old_oid استفادهنشده,
				 بنية object_id *new_oid,
				 ثابت حرف *committer استفادهنشده,
				 timestamp_t timestamp استفادهنشده,
				 صحيح tz استفادهنشده, ثابت حرف *پیام استفادهنشده,
				 فراغ *cb_داده)
{
	بنية stash_مدخل_داده *داده = cb_داده;
	بنية commit *stash;

	داده->شمار++;
	stash = lookup_commit_reference(داده->r, new_oid);
	إذا (!stash || check_stash_topology(داده->r, stash)) {
		أرجع error(_("%s does not look like a stash commit"),
			     oid_مقصد_hex(new_oid));
	}
	داده->اقلام = commit_list_append(stash, داده->اقلام);
	أرجع 0;
}

ساكن صحيح do_export_stash(بنية repository *r,
			   ثابت حرف *ref,
			   صحيح argc,
			   ثابت حرف **argv)
{
	بنية object_id پایه;
	بنية commit *prev;
	بنية commit_list *اقلام = عدم, **iter = &اقلام, *cur;
	صحيح res = 0;
	صحيح i;
	بنية strbuf revision = STRBUF_INIT;
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
	إذا (commit_درخت_extended("", 0, r->hash_algo->خالی_درخت, عدم,
				 &پایه, author, committer, عدم, عدم))
		أرجع error(_("unable to write base commit"));

	prev = lookup_commit_reference(r, &پایه);

	إذا (argc) {
		/*
		 * Find each specified stash, and load data into the array.
		 */
		لكل (i = 0; i < argc; i++) {
			بنية object_id oid;
			بنية commit *stash;

			إذا (parse_stash_revision(&revision, argv[i], 1) ||
			    repo_get_oid_with_flags(r, revision.buf, &oid,
						    GET_OID_QUIETLY |
						    GET_OID_GENTLY)) {
				res = error(_("unable to find stash entry %s"), argv[i]);
				اذهب بیرون;
			}

			stash = lookup_commit_reference(r, &oid);
			إذا (!stash || check_stash_topology(r, stash)) {
				res = error(_("%s does not look like a stash commit"),
						revision.buf);
				اذهب بیرون;
			}
			iter = commit_list_append(stash, iter);
		}
	} وإلا {
		/*
		 * Walk the reflog, finding each stash entry, and load data into the
		 * array.
		 */
		بنية stash_مدخل_داده cb_داده = {
			.r = r, .اقلام = iter,
		};
		إذا (refs_for_each_reflog_ent_معکوس(get_main_ref_store(r),
						     "refs/stash",
						     collect_stash_entries,
						     &cb_داده) && cb_داده.شمار)
			اذهب بیرون;
	}

	/*
	 * Now, create a set of commits identical to the regular stash commits,
	 * but where their first parents form a chain to our original empty
	 * base commit.
	 */
	اقلام = commit_list_معکوس(اقلام);
	لكل (cur = اقلام; cur; cur = cur->بعدی) {
		بنية commit_list *parents = عدم;
		بنية commit_list **بعدی = &parents;
		بنية object_id بیرون;
		بنية commit *stash = cur->قلم;

		بعدی = commit_list_append(prev, بعدی);
		بعدی = commit_list_append(stash, بعدی);
		res = write_commit_with_parents(r, &بیرون, &stash->object.oid, parents);
		commit_list_free(parents);
		إذا (res)
			اذهب بیرون;
		prev = lookup_commit_reference(r, &بیرون);
	}
	إذا (ref)
		refs_update_ref(get_main_ref_store(r), عدم, ref,
				&prev->object.oid, عدم, 0, UPDATE_REFS_DIE_ON_ERR);
	وإلا
		puts(oid_مقصد_hex(&prev->object.oid));
بیرون:
	strbuf_release(&revision);
	commit_list_free(اقلام);

	أرجع res;
}

تعداد export_action {
	ACTION_NONE,
	ACTION_PRINT,
	ACTION_مقصد_REF,
};

ساكن صحيح export_stash(صحيح argc,
			ثابت حرف **argv,
			ثابت حرف *پیشوند,
			بنية repository *repo)
{
	ثابت حرف *ref = عدم;
	تعداد export_action action = ACTION_NONE;
	بنية option options[] = {
		OPT_CMDMODE(0, "print", &action,
			    N_("print the object ID instead of writing it to a ref"),
			    ACTION_PRINT),
		OPT_STRING(0, "to-ref", &ref, "ref",
			    N_("save the data to the given ref")),
		OPT_انتها()
	};

	argc = parse_options(argc, argv, پیشوند, options,
			     git_stash_export_usage,
			     PARSE_OPT_KEEP_DASHDASH);

	إذا (ref && action == ACTION_NONE)
		action = ACTION_مقصد_REF;

	إذا (action == ACTION_NONE || (ref && action == ACTION_PRINT))
		أرجع error(_("exactly one of --print and --to-ref is required"));

	أرجع do_export_stash(repo, ref, argc, argv);
}

صحيح cmd_stash(صحيح argc,
	      ثابت حرف **argv,
	      ثابت حرف *پیشوند,
	      بنية repository *repo)
{
	pid_t pid = getpid();
	ثابت حرف *شاخص_file;
	بنية strvec آرگومانها = STRVEC_INIT;
	parse_opt_subcommand_fn *fn = عدم;
	بنية option options[] = {
		OPT_SUBCOMMAND("apply", &fn, apply_stash),
		OPT_SUBCOMMAND("clear", &fn, clear_stash),
		OPT_SUBCOMMAND("drop", &fn, drop_stash),
		OPT_SUBCOMMAND("pop", &fn, برداشت_stash),
		OPT_SUBCOMMAND("branch", &fn, branch_stash),
		OPT_SUBCOMMAND("list", &fn, list_stash),
		OPT_SUBCOMMAND("show", &fn, show_stash),
		OPT_SUBCOMMAND("store", &fn, store_stash),
		OPT_SUBCOMMAND("create", &fn, create_stash),
		OPT_SUBCOMMAND("push", &fn, افزودن_stash_unassumed),
		OPT_SUBCOMMAND("export", &fn, export_stash),
		OPT_SUBCOMMAND("import", &fn, import_stash),
		OPT_SUBCOMMAND_F("save", &fn, save_stash, عدم, PARSE_OPT_NOCOMPLETE),
		OPT_انتها()
	};
	ثابت حرف **آرگومانها_copy;
	صحيح ret;

	repo_config(the_repository, git_stash_config, عدم);

	argc = parse_options(argc, argv, پیشوند, options, git_stash_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL |
			     PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_پر_شاخص = 0;

	شاخص_file = repo_get_شاخص_file(the_repository);
	strbuf_addf(&stash_شاخص_مسیر, "%s.stash.%" PRIuMAX, شاخص_file,
		    (uintmax_t)pid);

	إذا (fn) {
		ret = fn(argc, argv, پیشوند, repo);

		/*
		 * The subcommand implementations return 0 on success, a
		 * negative value on failure, and STASH_APPLY_CONFLICT
		 * when applying a stash entry resulted in conflicts.
		 * Map failures to 128, the status die() uses, so that
		 * exit status 1 unambiguously indicates conflicts.
		 */
		إذا (ret < 0)
			أرجع 128;
		أرجع ret;
	} وإلا إذا (!argc) {
		أرجع !!افزودن_stash_unassumed(0, عدم, پیشوند, repo);
	}

	/* Assume 'stash push' */
	strvec_افزودن(&آرگومانها, "push");
	strvec_pushv(&آرگومانها, argv);

	/*
	 * `push_stash()` ends up modifying the array, which causes memory
	 * leaks if we didn't copy the array here.
	 */
	DUP_ARRAY(آرگومانها_copy, آرگومانها.v, آرگومانها.nr);

	ret = !!افزودن_stash(آرگومانها.nr, آرگومانها_copy, پیشوند, 1);

	strvec_clear(&آرگومانها);
	حرر(آرگومانها_copy);
	أرجع ret;
}
