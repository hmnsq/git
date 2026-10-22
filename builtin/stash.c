#define USE_THE_REPOSITORY_VARIABLE

#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "abspath.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "hash.h"
#অন্তর্ভাব "hex.h"
#অন্তর্ভাব "object-name.h"
#অন্তর্ভাব "parse-options.h"
#অন্তর্ভাব "refs.h"
#অন্তর্ভাব "stash.h"
#অন্তর্ভাব "lockfile.h"
#অন্তর্ভাব "cache-tree.h"
#অন্তর্ভাব "unpack-trees.h"
#অন্তর্ভাব "merge-ort-wrappers.h"
#অন্তর্ভাব "strvec.h"
#অন্তর্ভাব "run-command.h"
#অন্তর্ভাব "dir.h"
#অন্তর্ভাব "entry.h"
#অন্তর্ভাব "preload-index.h"
#অন্তর্ভাব "read-cache.h"
#অন্তর্ভাব "repository.h"
#অন্তর্ভাব "rerere.h"
#অন্তর্ভাব "revision.h"
#অন্তর্ভাব "setup.h"
#অন্তর্ভাব "sparse-index.h"
#অন্তর্ভাব "log-tree.h"
#অন্তর্ভাব "diffcore.h"
#অন্তর্ভাব "reflog.h"
#অন্তর্ভাব "reflog-walk.h"
#অন্তর্ভাব "add-interactive.h"
#অন্তর্ভাব "commit.h"

#define INCLUDE_ALL_FILES 2

#define BUILTIN_STASH_LIST_USAGE \
	N_("git stash list [<log-options>]")
#define BUILTIN_STASH_SHOW_USAGE \
	N_("git stash show [-u | --include-untracked | --only-untracked] [<diff-options>] [<stash>]")
#define BUILTIN_STASH_DROP_USAGE \
	N_("git stash drop [-q | --quiet] [<stash>]")
#define BUILTIN_STASH_বের_করো_USAGE \
	N_("git stash pop [--index] [-q | --quiet] [<stash>]")
#define BUILTIN_STASH_APPLY_USAGE \
	N_("git stash apply [--index] [-q | --quiet] [--label-ours=<label>] [--label-theirs=<label>] [--label-base=<label>] [<stash>]")
#define BUILTIN_STASH_BRANCH_USAGE \
	N_("git stash branch <branchname> [<stash>]")
#define BUILTIN_STASH_STORE_USAGE \
	N_("git stash store [(-m | --message) <message>] [-q | --quiet] <commit>")
#define BUILTIN_STASH_ঠেলো_USAGE \
	N_("git stash [push] [-p | --patch] [-S | --staged] [-k | --[no-]keep-index] [-q | --quiet]\n" \
	   "          [-u | --include-untracked] [-a | --all] [(-m | --message) <message>]\n" \
	   "          [--pathspec-from-file=<file> [--pathspec-file-nul]]\n" \
	   "          [--] [<pathspec>...]")
#define BUILTIN_STASH_SAVE_USAGE \
	N_("git stash save [-p | --patch] [-S | --staged] [-k | --[no-]keep-index] [-q | --quiet]\n" \
	   "          [-u | --include-untracked] [-a | --all] [<message>]")
#define BUILTIN_STASH_CREATE_USAGE \
	N_("git stash create [<message>]")
#define BUILTIN_STASH_EXPORT_USAGE \
	N_("git stash export (--print | --to-ref <ref>) [<stash>...]")
#define BUILTIN_STASH_IMPORT_USAGE \
	N_("git stash import <commit>")
#define BUILTIN_STASH_CLEAR_USAGE \
	"git stash clear"

স্থির ধ্রুবক অক্ষর * ধ্রুবক git_stash_usage[] = {
	BUILTIN_STASH_LIST_USAGE,
	BUILTIN_STASH_SHOW_USAGE,
	BUILTIN_STASH_DROP_USAGE,
	BUILTIN_STASH_বের_করো_USAGE,
	BUILTIN_STASH_APPLY_USAGE,
	BUILTIN_STASH_BRANCH_USAGE,
	BUILTIN_STASH_ঠেলো_USAGE,
	BUILTIN_STASH_SAVE_USAGE,
	BUILTIN_STASH_CLEAR_USAGE,
	BUILTIN_STASH_CREATE_USAGE,
	BUILTIN_STASH_STORE_USAGE,
	BUILTIN_STASH_EXPORT_USAGE,
	BUILTIN_STASH_IMPORT_USAGE,
	ফাঁকা
};

স্থির ধ্রুবক অক্ষর * ধ্রুবক git_stash_list_usage[] = {
	BUILTIN_STASH_LIST_USAGE,
	ফাঁকা
};

স্থির ধ্রুবক অক্ষর * ধ্রুবক git_stash_show_usage[] = {
	BUILTIN_STASH_SHOW_USAGE,
	ফাঁকা
};

স্থির ধ্রুবক অক্ষর * ধ্রুবক git_stash_drop_usage[] = {
	BUILTIN_STASH_DROP_USAGE,
	ফাঁকা
};

স্থির ধ্রুবক অক্ষর * ধ্রুবক git_stash_বের_করো_usage[] = {
	BUILTIN_STASH_বের_করো_USAGE,
	ফাঁকা
};

স্থির ধ্রুবক অক্ষর * ধ্রুবক git_stash_apply_usage[] = {
	BUILTIN_STASH_APPLY_USAGE,
	ফাঁকা
};

স্থির ধ্রুবক অক্ষর * ধ্রুবক git_stash_branch_usage[] = {
	BUILTIN_STASH_BRANCH_USAGE,
	ফাঁকা
};

স্থির ধ্রুবক অক্ষর * ধ্রুবক git_stash_clear_usage[] = {
	BUILTIN_STASH_CLEAR_USAGE,
	ফাঁকা
};

স্থির ধ্রুবক অক্ষর * ধ্রুবক git_stash_store_usage[] = {
	BUILTIN_STASH_STORE_USAGE,
	ফাঁকা
};

স্থির ধ্রুবক অক্ষর * ধ্রুবক git_stash_ঠেলো_usage[] = {
	BUILTIN_STASH_ঠেলো_USAGE,
	ফাঁকা
};

স্থির ধ্রুবক অক্ষর * ধ্রুবক git_stash_save_usage[] = {
	BUILTIN_STASH_SAVE_USAGE,
	ফাঁকা
};

স্থির ধ্রুবক অক্ষর * ধ্রুবক git_stash_export_usage[] = {
	BUILTIN_STASH_EXPORT_USAGE,
	ফাঁকা
};

স্থির ধ্রুবক অক্ষর * ধ্রুবক git_stash_import_usage[] = {
	BUILTIN_STASH_IMPORT_USAGE,
	ফাঁকা
};

স্থির ধ্রুবক অক্ষর ref_stash[] = "refs/stash";
স্থির কাঠামো strbuf stash_সূচক_path = STRBUF_INIT;

স্থির পূর্ণসংখ্যা show_stat = 1;
স্থির পূর্ণসংখ্যা show_patch;
স্থির পূর্ণসংখ্যা show_include_untracked;
স্থির পূর্ণসংখ্যা use_সূচক;

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
কাঠামো stash_info {
	কাঠামো object_id w_commit;
	কাঠামো object_id b_commit;
	কাঠামো object_id i_commit;
	কাঠামো object_id u_commit;
	কাঠামো object_id c_commit;
	কাঠামো object_id w_বৃক্ষ;
	কাঠামো object_id b_বৃক্ষ;
	কাঠামো object_id i_বৃক্ষ;
	কাঠামো object_id u_বৃক্ষ;
	কাঠামো strbuf revision;
	পূর্ণসংখ্যা is_stash_ref;
	পূর্ণসংখ্যা has_u;
};

#define STASH_INFO_INIT { \
	.revision = STRBUF_INIT, \
}

স্থির শূন্য free_stash_info(কাঠামো stash_info *info)
{
	strbuf_release(&info->revision);
}

স্থির পূর্ণসংখ্যা check_stash_topology(কাঠামো repository *r, কাঠামো commit *stash)
{
	কাঠামো commit *p1, *p2, *p3 = ফাঁকা;

	/* stash must have two or three parents */
	যদি (!stash->parents || !stash->parents->পরবর্তী ||
			(stash->parents->পরবর্তী->পরবর্তী && stash->parents->পরবর্তী->পরবর্তী->পরবর্তী))
		ফেরত -1;
	p1 = stash->parents->item;
	p2 = stash->parents->পরবর্তী->item;
	যদি (stash->parents->পরবর্তী->পরবর্তী)
		p3 = stash->parents->পরবর্তী->পরবর্তী->item;
	যদি (repo_parse_commit(r, p1) || repo_parse_commit(r, p2) ||
			(p3 && repo_parse_commit(r, p3)))
		ফেরত -1;
	/* p2 must have a single parent, p3 must have no parents */
	যদি (!p2->parents || p2->parents->পরবর্তী || (p3 && p3->parents))
		ফেরত -1;
	যদি (repo_parse_commit(r, p2->parents->item))
		ফেরত -1;
	/* p2^1 must equal p1 */
	যদি (!oideq(&p1->object.oid, &p2->parents->item->object.oid))
		ফেরত -1;

	ফেরত 0;
}

স্থির শূন্য assert_stash_like(কাঠামো stash_info *info, ধ্রুবক অক্ষর *revision)
{
	যদি (get_oidf(&info->b_commit, "%s^1", revision) ||
	    get_oidf(&info->w_বৃক্ষ, "%s:", revision) ||
	    get_oidf(&info->b_বৃক্ষ, "%s^1:", revision) ||
	    get_oidf(&info->i_বৃক্ষ, "%s^2:", revision))
		die(_("'%s' is not a stash-like commit"), revision);
}

স্থির পূর্ণসংখ্যা parse_stash_revision(কাঠামো strbuf *revision, ধ্রুবক অক্ষর *commit, পূর্ণসংখ্যা quiet)
{
	strbuf_reset(revision);
	যদি (!commit) {
		যদি (!refs_ref_exists(get_main_ref_store(the_repository), ref_stash)) {
			যদি (!quiet)
				fprintf_ln(stderr, _("No stash entries found."));
			ফেরত -1;
		}

		strbuf_addf(revision, "%s@{0}", ref_stash);
	} অন্যথা যদি (strspn(commit, "0123456789") == strlen(commit)) {
		strbuf_addf(revision, "%s@{%s}", ref_stash, commit);
	} অন্যথা {
		strbuf_addstr(revision, commit);
	}
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা get_stash_info(কাঠামো stash_info *info, পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv)
{
	পূর্ণসংখ্যা ret;
	অক্ষর *end_of_rev;
	অক্ষর *expanded_ref;
	ধ্রুবক অক্ষর *revision;
	ধ্রুবক অক্ষর *commit = ফাঁকা;
	কাঠামো object_id dummy;
	কাঠামো strbuf symbolic = STRBUF_INIT;

	যদি (argc > 1) {
		পূর্ণসংখ্যা i;
		কাঠামো strbuf refs_বার্তা = STRBUF_INIT;

		জন্য (i = 0; i < argc; i++)
			strbuf_addf(&refs_বার্তা, " '%s'", argv[i]);

		fprintf_ln(stderr, _("Too many revisions specified:%s"),
			   refs_বার্তা.buf);
		strbuf_release(&refs_বার্তা);

		ফেরত -1;
	}

	যদি (argc == 1)
		commit = argv[0];

	strbuf_init(&info->revision, 0);
	যদি (parse_stash_revision(&info->revision, commit, 0)) {
		ফেরত -1;
	}

	revision = info->revision.buf;

	যদি (repo_get_oid(the_repository, revision, &info->w_commit))
		ফেরত error(_("%s is not a valid reference"), revision);

	assert_stash_like(info, revision);

	info->has_u = !get_oidf(&info->u_বৃক্ষ, "%s^3:", revision);

	end_of_rev = strchrnul(revision, '@');
	strbuf_add(&symbolic, revision, end_of_rev - revision);

	ret = repo_dwim_ref(the_repository, symbolic.buf, symbolic.len,
			    &dummy, &expanded_ref, 0);
	strbuf_release(&symbolic);
	নির্বাচন (ret) {
	ক্ষেত্র 0: /* Not found, but valid ref */
		info->is_stash_ref = 0;
		বিরতি;
	ক্ষেত্র 1:
		info->is_stash_ref = !strcmp(expanded_ref, ref_stash);
		বিরতি;
	পূর্বনির্ধারিত: /* Invalid or ambiguous */
		বিরতি;
	}

	মুক্ত(expanded_ref);
	ফেরত !(ret == 0 || ret == 1);
}

স্থির পূর্ণসংখ্যা do_clear_stash(শূন্য)
{
	কাঠামো object_id obj;
	যদি (repo_get_oid(the_repository, ref_stash, &obj))
		ফেরত 0;

	ফেরত refs_delete_ref(get_main_ref_store(the_repository), ফাঁকা,
			       ref_stash, &obj, 0);
}

স্থির পূর্ণসংখ্যা clear_stash(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix,
		       কাঠামো repository *repo UNUSED)
{
	কাঠামো option options[] = {
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_clear_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	যদি (argc)
		ফেরত error(_("git stash clear with arguments is "
			       "unimplemented"));

	ফেরত do_clear_stash();
}

স্থির পূর্ণসংখ্যা reset_বৃক্ষ(কাঠামো object_id *i_বৃক্ষ, পূর্ণসংখ্যা update, পূর্ণসংখ্যা reset)
{
	পূর্ণসংখ্যা nr_trees = 1;
	কাঠামো unpack_trees_options opts;
	কাঠামো বৃক্ষ_desc t[MAX_UNPACK_TREES];
	কাঠামো বৃক্ষ *বৃক্ষ;
	কাঠামো lock_file lock_file = LOCK_INIT;

	repo_read_সূচক_preload(the_repository, ফাঁকা, 0);
	যদি (refresh_সূচক(the_repository->সূচক, REFRESH_QUIET, ফাঁকা, ফাঁকা, ফাঁকা))
		ফেরত -1;

	repo_hold_locked_সূচক(the_repository, &lock_file, LOCK_DIE_ON_ERROR);

	memset(&opts, 0, আকার(opts));

	বৃক্ষ = repo_parse_বৃক্ষ_indirect(the_repository, i_বৃক্ষ);
	যদি (repo_parse_বৃক্ষ(the_repository, বৃক্ষ))
		ফেরত -1;

	init_বৃক্ষ_desc(t, &বৃক্ষ->object.oid, বৃক্ষ->বাফার, বৃক্ষ->আকার_2);

	opts.শীর্ষ_idx = 1;
	opts.src_সূচক = the_repository->সূচক;
	opts.dst_সূচক = the_repository->সূচক;
	opts.merge = 1;
	opts.reset = reset ? UNPACK_RESET_PROTECT_UNTRACKED : 0;
	opts.update = update;
	যদি (update)
		opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
	opts.fn = oneway_merge;

	যদি (unpack_trees(nr_trees, t, &opts))
		ফেরত -1;

	যদি (write_locked_সূচক(the_repository->সূচক, &lock_file, COMMIT_LOCK))
		ফেরত error(_("unable to write new index file"));

	ফেরত 0;
}

স্থির পূর্ণসংখ্যা create_সূচক_from_বৃক্ষ(ধ্রুবক কাঠামো object_id *বৃক্ষ_id,
				  ধ্রুবক অক্ষর *সূচক_path)
{
	পূর্ণসংখ্যা nr_trees = 1;
	পূর্ণসংখ্যা ret = 0;
	কাঠামো unpack_trees_options opts;
	কাঠামো বৃক্ষ_desc t[MAX_UNPACK_TREES];
	কাঠামো বৃক্ষ *বৃক্ষ;
	কাঠামো সূচক_state dst_istate = সূচক_STATE_INIT(the_repository);
	কাঠামো lock_file lock_file = LOCK_INIT;

	repo_read_সূচক_preload(the_repository, ফাঁকা, 0);
	refresh_সূচক(the_repository->সূচক, REFRESH_QUIET, ফাঁকা, ফাঁকা, ফাঁকা);

	hold_lock_file_for_update(&lock_file, সূচক_path, LOCK_DIE_ON_ERROR);

	memset(&opts, 0, আকার(opts));

	বৃক্ষ = repo_parse_বৃক্ষ_indirect(the_repository, বৃক্ষ_id);
	যদি (!বৃক্ষ || repo_parse_বৃক্ষ(the_repository, বৃক্ষ)) {
		ret = -1;
		যাও done;
	}

	init_বৃক্ষ_desc(t, &বৃক্ষ->object.oid, বৃক্ষ->বাফার, বৃক্ষ->আকার_2);

	opts.শীর্ষ_idx = 1;
	opts.src_সূচক = the_repository->সূচক;
	opts.dst_সূচক = &dst_istate;
	opts.merge = 1;
	opts.reset = UNPACK_RESET_PROTECT_UNTRACKED;
	opts.fn = oneway_merge;

	যদি (unpack_trees(nr_trees, t, &opts)) {
		ret = -1;
		যাও done;
	}

	যদি (write_locked_সূচক(&dst_istate, &lock_file, COMMIT_LOCK)) {
		ret = error(_("unable to write new index file"));
		যাও done;
	}

done:
	release_সূচক(&dst_istate);
	যদি (ret)
		rollback_lock_file(&lock_file);
	ফেরত ret;
}

স্থির পূর্ণসংখ্যা is_path_a_directory(ধ্রুবক অক্ষর *path)
{
	/*
	 * This function differs from abspath.c:is_directory() in that
	 * here we use lstat() instead of stat(); we do not want to
	 * follow symbolic links here.
	 */
	কাঠামো stat st;
	ফেরত (!lstat(path, &st) && S_ISDIR(st.st_mode));
}

স্থির শূন্য add_diff_to_buf(কাঠামো diff_queue_struct *q,
			    কাঠামো diff_options *options UNUSED,
			    শূন্য *তথ্য)
{
	পূর্ণসংখ্যা i;

	জন্য (i = 0; i < q->nr; i++) {
		যদি (is_path_a_directory(q->queue[i]->one->path))
			চালু;

		strbuf_addstr(তথ্য, q->queue[i]->one->path);

		/* NUL-terminate: will be fed to update-index -z */
		strbuf_addch(তথ্য, '\0');
	}
}

স্থির পূর্ণসংখ্যা restore_untracked(কাঠামো object_id *u_বৃক্ষ)
{
	পূর্ণসংখ্যা res;
	কাঠামো child_process cp = CHILD_PROCESS_INIT;

	/*
	 * We need to run restore files from a given index, but without
	 * affecting the current index, so we use GIT_INDEX_FILE with
	 * run_command to fork processes that will not interfere.
	 */
	cp.git_cmd = 1;
	strvec_ঠেলো(&cp.args, "read-tree");
	strvec_ঠেলো(&cp.args, oid_to_hex(u_বৃক্ষ));
	strvec_pushf(&cp.env, "GIT_INDEX_FILE=%s",
		     stash_সূচক_path.buf);
	যদি (run_command(&cp)) {
		remove_path(stash_সূচক_path.buf);
		ফেরত -1;
	}

	child_process_init(&cp);
	cp.git_cmd = 1;
	strvec_pushl(&cp.args, "checkout-index", "--all", ফাঁকা);
	strvec_pushf(&cp.env, "GIT_INDEX_FILE=%s",
		     stash_সূচক_path.buf);

	res = run_command(&cp);
	remove_path(stash_সূচক_path.buf);
	ফেরত res;
}

স্থির শূন্য unstage_changes_unless_new(কাঠামো object_id *orig_বৃক্ষ)
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

	কাঠামো checkout state = CHECKOUT_INIT;
	কাঠামো diff_options diff_opts;
	কাঠামো lock_file lock = LOCK_INIT;
	পূর্ণসংখ্যা i;

	/* If any entries have skip_worktree set, we'll have to check 'em out */
	state.force = 1;
	state.quiet = 1;
	state.refresh_cache = 1;
	state.istate = the_repository->সূচক;

	/*
	 * Step 1: get a difference between orig_tree (which corresponding
	 * to the index before a merge was run) and the current index
	 * (reflecting the changes brought in by the merge).
	 */
	repo_diff_setup(the_repository, &diff_opts);
	diff_opts.flags.recursive = 1;
	diff_opts.detect_rename = 0;
	diff_opts.output_format = DIFF_FORMAT_NO_OUTPUT;
	diff_setup_done(&diff_opts);

	do_diff_cache(orig_বৃক্ষ, &diff_opts);
	diffcore_std(&diff_opts);

	/* Iterate over the paths that changed due to the merge... */
	জন্য (i = 0; i < diff_queued_diff.nr; i++) {
		কাঠামো diff_filepair *p;
		কাঠামো cache_entry *ce;
		পূর্ণসংখ্যা pos;

		/* Look up the path's position in the current index. */
		p = diff_queued_diff.queue[i];
		pos = সূচক_name_pos(the_repository->সূচক, p->two->path,
				     strlen(p->two->path));

		/*
		 * Step 2: Place changes in the working tree
		 *
		 * Stash is about restoring changes *to the working tree*.
		 * So if the merge successfully got a new version of some
		 * path, but left it out of the working tree, then clear the
		 * SKIP_WORKTREE bit and write it to the working tree.
		 */
		যদি (pos >= 0 && ce_skip_worktree(the_repository->সূচক->cache[pos])) {
			কাঠামো stat st;

			ce = the_repository->সূচক->cache[pos];
			যদি (!lstat(ce->name, &st)) {
				/* Conflicting path present; relocate it */
				কাঠামো strbuf new_path = STRBUF_INIT;
				পূর্ণসংখ্যা fd;

				strbuf_addf(&new_path,
					    "%s.stash.XXXXXX", ce->name);
				fd = xmkstemp(new_path.buf);
				close(fd);
				printf(_("WARNING: Untracked file in way of "
					 "tracked file!  Renaming\n "
					 "           %s -> %s\n"
					 "         to make room.\n"),
				       ce->name, new_path.buf);
				যদি (rename(ce->name, new_path.buf))
					die("Failed to move %s to %s",
					    ce->name, new_path.buf);
				strbuf_release(&new_path);
			}
			checkout_entry(ce, &state, ফাঁকা, ফাঁকা);
			ce->ce_flags &= ~CE_SKIP_WORKTREE;
		}

		/*
		 * Step 3: "unstage" changes, as long as they are still tracked
		 */
		যদি (p->one->oid_valid) {
			/*
			 * Path existed in orig_tree; restore index entry
			 * from that tree in order to "unstage" the changes.
			 */
			পূর্ণসংখ্যা option = ADD_CACHE_OK_TO_REPLACE;
			যদি (pos < 0)
				option = ADD_CACHE_OK_TO_ADD;

			ce = make_cache_entry(the_repository->সূচক,
					      p->one->mode,
					      &p->one->oid,
					      p->one->path,
					      0, 0);
			add_সূচক_entry(the_repository->সূচক, ce, option);
		}
	}
	diff_flush(&diff_opts);

	/*
	 * Step 4: write the new index to disk
	 */
	repo_hold_locked_সূচক(the_repository, &lock, LOCK_DIE_ON_ERROR);
	যদি (write_locked_সূচক(the_repository->সূচক, &lock,
			       COMMIT_LOCK | SKIP_IF_UNCHANGED))
		die(_("could not write index"));
}

স্থির গণনা stash_apply_ফলাফল do_apply_stash(ধ্রুবক অক্ষর *prefix,
					      কাঠামো stash_info *info,
					      পূর্ণসংখ্যা সূচক, পূর্ণসংখ্যা quiet,
					      ধ্রুবক অক্ষর *label_ours,
					      ধ্রুবক অক্ষর *label_theirs,
					      ধ্রুবক অক্ষর *label_base)
{
	পূর্ণসংখ্যা clean, ret;
	পূর্ণসংখ্যা has_সূচক = সূচক;
	কাঠামো merge_options o;
	কাঠামো object_id c_বৃক্ষ;
	কাঠামো object_id সূচক_বৃক্ষ;
	কাঠামো বৃক্ষ *শীর্ষ, *merge, *merge_base;
	কাঠামো lock_file lock = LOCK_INIT;

	repo_read_সূচক_preload(the_repository, ফাঁকা, 0);
	যদি (repo_refresh_and_write_সূচক(the_repository, REFRESH_QUIET, 0, 0,
					 ফাঁকা, ফাঁকা, ফাঁকা))
		ফেরত error(_("could not write index"));

	যদি (write_সূচক_as_বৃক্ষ(&c_বৃক্ষ, the_repository->সূচক,
				repo_get_সূচক_file(the_repository), 0, ফাঁকা))
		ফেরত error(_("cannot apply a stash in the middle of a merge"));

	init_ui_merge_options(&o, the_repository);

	যদি (quiet)
		o.verbosity = 0;

	যদি (সূচক) {
		যদি (oideq(&info->b_বৃক্ষ, &info->i_বৃক্ষ) ||
		    oideq(&c_বৃক্ষ, &info->i_বৃক্ষ)) {
			has_সূচক = 0;
		} অন্যথা {
			কাঠামো merge_ফলাফল ফলাফল = { 0 };

			o.branch1 = "Current index";
			o.branch2 = "Stashed index changes";
			o.ancestor = "Stash base";

			শীর্ষ = lookup_বৃক্ষ(o.repo, &c_বৃক্ষ);
			merge = lookup_বৃক্ষ(o.repo, &info->i_বৃক্ষ);
			merge_base = lookup_বৃক্ষ(o.repo, &info->b_বৃক্ষ);

			merge_incore_nonrecursive(&o, merge_base, শীর্ষ, merge,
						  &ফলাফল);

			যদি (ফলাফল.clean < 0) {
				merge_finalize(&o, &ফলাফল);
				ফেরত error(_("index merge failed"));
			} অন্যথা যদি (!ফলাফল.clean) {
				merge_finalize(&o, &ফলাফল);
				ফেরত error(_("conflicts in index. "
					       "Try without --index."));
			} অন্যথা {
				oidcpy(&সূচক_বৃক্ষ, &ফলাফল.বৃক্ষ->object.oid);
				merge_finalize(&o, &ফলাফল);
			}
		}
	}

	o.branch1 = label_ours ? label_ours : "Updated upstream";
	o.branch2 = label_theirs ? label_theirs : "Stashed changes";
	o.ancestor = label_base ? label_base : "Stash base";

	যদি (oideq(&info->b_বৃক্ষ, &c_বৃক্ষ))
		o.branch1 = "Version stash was based on";

	যদি (o.verbosity >= 3)
		printf_ln(_("Merging %s with %s"), o.branch1, o.branch2);

	শীর্ষ = lookup_বৃক্ষ(o.repo, &c_বৃক্ষ);
	merge = lookup_বৃক্ষ(o.repo, &info->w_বৃক্ষ);
	merge_base = lookup_বৃক্ষ(o.repo, &info->b_বৃক্ষ);

	repo_hold_locked_সূচক(o.repo, &lock, LOCK_DIE_ON_ERROR);
	clean = merge_ort_nonrecursive(&o, শীর্ষ, merge, merge_base);

	/*
	 * If 'clean' >= 0, reverse the value for 'ret' so 'ret' is 0 when the
	 * merge was clean, and 1 if the merge was unclean or a negative value
	 * if it encountered an error.
	 */
	ret = clean >= 0 ? !clean : clean;

	যদি (ret < 0)
		rollback_lock_file(&lock);
	অন্যথা যদি (write_locked_সূচক(o.repo->সূচক, &lock,
				      COMMIT_LOCK | SKIP_IF_UNCHANGED))
		ret = error(_("could not write index"));

	যদি (ret) {
		repo_rerere(the_repository, 0);

		যদি (সূচক)
			fprintf_ln(stderr, _("Index was not unstashed."));

		যাও restore_untracked;
	}

	যদি (has_সূচক) {
		যদি (reset_বৃক্ষ(&সূচক_বৃক্ষ, 0, 0))
			ret = -1;
	} অন্যথা {
		unstage_changes_unless_new(&c_বৃক্ষ);
	}

restore_untracked:
	যদি (info->has_u && restore_untracked(&info->u_বৃক্ষ))
		ret = error(_("could not restore untracked files from stash"));

	যদি (!quiet) {
		কাঠামো child_process cp = CHILD_PROCESS_INIT;

		/*
		 * Status is quite simple and could be replaced with calls to
		 * wt_status in the future, but it adds complexities which may
		 * require more tests.
		 */
		cp.git_cmd = 1;
		cp.dir = prefix;
		strvec_pushf(&cp.env, GIT_WORK_বৃক্ষ_ENVIRONMENT"=%s",
			     absolute_path(repo_get_work_বৃক্ষ(the_repository)));
		strvec_pushf(&cp.env, GIT_DIR_ENVIRONMENT"=%s",
			     absolute_path(repo_get_git_dir(the_repository)));
		strvec_ঠেলো(&cp.args, "status");
		run_command(&cp);
	}

	ফেরত ret;
}

স্থির পূর্ণসংখ্যা apply_stash(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix,
		       কাঠামো repository *repo UNUSED)
{
	পূর্ণসংখ্যা ret = -1;
	পূর্ণসংখ্যা quiet = 0;
	পূর্ণসংখ্যা সূচক = use_সূচক;
	ধ্রুবক অক্ষর *label_ours = ফাঁকা, *label_theirs = ফাঁকা, *label_base = ফাঁকা;
	কাঠামো stash_info info = STASH_INFO_INIT;
	কাঠামো option options[] = {
		OPT__QUIET(&quiet, N_("be quiet, only report errors")),
		OPT_BOOL(0, "index", &সূচক,
			 N_("attempt to recreate the index")),
		OPT_STRING(0, "label-ours", &label_ours, N_("label"),
			   N_("label for the upstream side in conflict markers")),
		OPT_STRING(0, "label-theirs", &label_theirs, N_("label"),
			   N_("label for the stashed side in conflict markers")),
		OPT_STRING(0, "label-base", &label_base, N_("label"),
			   N_("label for the base in diff3 conflict markers")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_apply_usage, 0);

	যদি (get_stash_info(&info, argc, argv))
		যাও cleanup;

	ret = do_apply_stash(prefix, &info, সূচক, quiet,
			     label_ours, label_theirs, label_base);
cleanup:
	free_stash_info(&info);
	ফেরত ret;
}

স্থির পূর্ণসংখ্যা reject_reflog_ent(ধ্রুবক অক্ষর *refname UNUSED,
			     কাঠামো object_id *ooid UNUSED,
			     কাঠামো object_id *noid UNUSED,
			     ধ্রুবক অক্ষর *email UNUSED,
			     timestamp_t timestamp UNUSED,
			     পূর্ণসংখ্যা tz UNUSED, ধ্রুবক অক্ষর *message UNUSED,
			     শূন্য *cb_তথ্য UNUSED)
{
	ফেরত 1;
}

স্থির পূর্ণসংখ্যা reflog_is_empty(ধ্রুবক অক্ষর *refname)
{
	ফেরত !refs_for_each_reflog_ent(get_main_ref_store(the_repository),
					 refname, reject_reflog_ent, ফাঁকা);
}

স্থির পূর্ণসংখ্যা do_drop_stash(কাঠামো stash_info *info, পূর্ণসংখ্যা quiet)
{
	যদি (!reflog_delete(info->revision.buf,
			   EXPIRE_REFLOGS_REWRITE | EXPIRE_REFLOGS_UPDATE_REF,
			   0)) {
		যদি (!quiet)
			printf_ln(_("Dropped %s (%s)"), info->revision.buf,
				  oid_to_hex(&info->w_commit));
	} অন্যথা {
		ফেরত error(_("%s: Could not drop stash entry"),
			     info->revision.buf);
	}

	যদি (reflog_is_empty(ref_stash))
		do_clear_stash();

	ফেরত 0;
}

স্থির পূর্ণসংখ্যা get_stash_info_assert(কাঠামো stash_info *info, পূর্ণসংখ্যা argc,
				 ধ্রুবক অক্ষর **argv)
{
	পূর্ণসংখ্যা ret = get_stash_info(info, argc, argv);

	যদি (ret < 0)
		ফেরত ret;

	যদি (!info->is_stash_ref)
		ফেরত error(_("'%s' is not a stash reference"), info->revision.buf);

	ফেরত 0;
}

স্থির পূর্ণসংখ্যা drop_stash(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix,
		      কাঠামো repository *repo UNUSED)
{
	পূর্ণসংখ্যা ret = -1;
	পূর্ণসংখ্যা quiet = 0;
	কাঠামো stash_info info = STASH_INFO_INIT;
	কাঠামো option options[] = {
		OPT__QUIET(&quiet, N_("be quiet, only report errors")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_drop_usage, 0);

	যদি (get_stash_info_assert(&info, argc, argv))
		যাও cleanup;

	ret = do_drop_stash(&info, quiet);
cleanup:
	free_stash_info(&info);
	ফেরত ret;
}

স্থির পূর্ণসংখ্যা বের_করো_stash(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix,
		     কাঠামো repository *repo UNUSED)
{
	পূর্ণসংখ্যা ret = -1;
	পূর্ণসংখ্যা সূচক = use_সূচক;
	পূর্ণসংখ্যা quiet = 0;
	কাঠামো stash_info info = STASH_INFO_INIT;
	কাঠামো option options[] = {
		OPT__QUIET(&quiet, N_("be quiet, only report errors")),
		OPT_BOOL(0, "index", &সূচক,
			 N_("attempt to recreate the index")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_বের_করো_usage, 0);

	যদি (get_stash_info_assert(&info, argc, argv))
		যাও cleanup;

	যদি ((ret = do_apply_stash(prefix, &info, সূচক, quiet,
				  ফাঁকা, ফাঁকা, ফাঁকা)))
		printf_ln(_("The stash entry is kept in case "
			    "you need it again."));
	অন্যথা
		ret = do_drop_stash(&info, quiet);

cleanup:
	free_stash_info(&info);
	ফেরত ret;
}

স্থির পূর্ণসংখ্যা branch_stash(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix,
			কাঠামো repository *repo UNUSED)
{
	পূর্ণসংখ্যা ret = -1;
	ধ্রুবক অক্ষর *branch = ফাঁকা;
	কাঠামো stash_info info = STASH_INFO_INIT;
	কাঠামো child_process cp = CHILD_PROCESS_INIT;
	কাঠামো option options[] = {
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_branch_usage, 0);

	যদি (!argc) {
		fprintf_ln(stderr, _("No branch name specified"));
		ফেরত -1;
	}

	branch = argv[0];

	যদি (get_stash_info(&info, argc - 1, argv + 1))
		যাও cleanup;

	cp.git_cmd = 1;
	strvec_pushl(&cp.args, "checkout", "-b", ফাঁকা);
	strvec_ঠেলো(&cp.args, branch);
	strvec_ঠেলো(&cp.args, oid_to_hex(&info.b_commit));
	ret = run_command(&cp);
	যদি (!ret)
		ret = do_apply_stash(prefix, &info, 1, 0,
				     ফাঁকা, ফাঁকা, ফাঁকা);
	যদি (!ret && info.is_stash_ref)
		ret = do_drop_stash(&info, 0);

cleanup:
	free_stash_info(&info);
	ফেরত ret;
}

স্থির পূর্ণসংখ্যা list_stash(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix,
		      কাঠামো repository *repo UNUSED)
{
	কাঠামো child_process cp = CHILD_PROCESS_INIT;
	কাঠামো option options[] = {
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_list_usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT);

	যদি (!refs_ref_exists(get_main_ref_store(the_repository), ref_stash))
		ফেরত 0;

	cp.git_cmd = 1;
	strvec_pushl(&cp.args, "log", "--format=%gd: %gs", "-g",
		     "--first-parent", ফাঁকা);
	strvec_pushv(&cp.args, argv);
	strvec_ঠেলো(&cp.args, ref_stash);
	strvec_ঠেলো(&cp.args, "--");
	ফেরত run_command(&cp);
}

স্থির পূর্ণসংখ্যা git_stash_config(ধ্রুবক অক্ষর *var, ধ্রুবক অক্ষর *মান,
			    ধ্রুবক কাঠামো config_context *ctx, শূন্য *cb)
{
	যদি (!strcmp(var, "stash.showstat")) {
		show_stat = git_config_bool(var, মান);
		ফেরত 0;
	}
	যদি (!strcmp(var, "stash.showpatch")) {
		show_patch = git_config_bool(var, মান);
		ফেরত 0;
	}
	যদি (!strcmp(var, "stash.showincludeuntracked")) {
		show_include_untracked = git_config_bool(var, মান);
		ফেরত 0;
	}
	যদি (!strcmp(var, "stash.index")) {
		use_সূচক = git_config_bool(var, মান);
		ফেরত 0;
	}
	ফেরত git_diff_basic_config(var, মান, ctx, cb);
}

স্থির শূন্য diff_include_untracked(ধ্রুবক কাঠামো stash_info *info, কাঠামো diff_options *diff_opt)
{
	ধ্রুবক কাঠামো object_id *oid[] = { &info->w_commit, &info->u_বৃক্ষ };
	কাঠামো বৃক্ষ *বৃক্ষ[ARRAY_আকার(oid)];
	কাঠামো বৃক্ষ_desc বৃক্ষ_desc[ARRAY_আকার(oid)];
	কাঠামো unpack_trees_options unpack_বৃক্ষ_opt = { 0 };

	জন্য (আকার_t i = 0; i < ARRAY_আকার(oid); i++) {
		বৃক্ষ[i] = repo_parse_বৃক্ষ_indirect(the_repository, oid[i]);
		যদি (repo_parse_বৃক্ষ(the_repository, বৃক্ষ[i]) < 0)
			die(_("failed to parse tree"));
		init_বৃক্ষ_desc(&বৃক্ষ_desc[i], &বৃক্ষ[i]->object.oid,
			       বৃক্ষ[i]->বাফার, বৃক্ষ[i]->আকার_2);
	}

	unpack_বৃক্ষ_opt.শীর্ষ_idx = -1;
	unpack_বৃক্ষ_opt.src_সূচক = the_repository->সূচক;
	unpack_বৃক্ষ_opt.dst_সূচক = the_repository->সূচক;
	unpack_বৃক্ষ_opt.merge = 1;
	unpack_বৃক্ষ_opt.fn = stash_worktree_untracked_merge;

	যদি (unpack_trees(ARRAY_আকার(বৃক্ষ_desc), বৃক্ষ_desc, &unpack_বৃক্ষ_opt))
		die(_("failed to unpack trees"));

	do_diff_cache(&info->b_commit, diff_opt);
}

স্থির পূর্ণসংখ্যা show_stash(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix,
		      কাঠামো repository *repo UNUSED)
{
	পূর্ণসংখ্যা i;
	পূর্ণসংখ্যা ret = -1;
	কাঠামো stash_info info = STASH_INFO_INIT;
	কাঠামো rev_info rev;
	কাঠামো strvec stash_args = STRVEC_INIT;
	কাঠামো strvec revision_args = STRVEC_INIT;
	গণনা {
		UNTRACKED_NONE,
		UNTRACKED_INCLUDE,
		UNTRACKED_ONLY
	} show_untracked = show_include_untracked ? UNTRACKED_INCLUDE : UNTRACKED_NONE;
	কাঠামো option options[] = {
		OPT_SET_INT('u', "include-untracked", &show_untracked,
			    N_("include untracked files in the stash"),
			    UNTRACKED_INCLUDE),
		OPT_SET_INT_F(0, "only-untracked", &show_untracked,
			      N_("only show untracked files in the stash"),
			      UNTRACKED_ONLY, PARSE_OPT_NONEG),
		OPT_END()
	};
	পূর্ণসংখ্যা do_usage = 0;

	init_diff_ui_defaults();
	repo_config(the_repository, git_diff_ui_config, ফাঁকা);
	repo_init_revisions(the_repository, &rev, prefix);

	argc = parse_options(argc, argv, prefix, options, git_stash_show_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	strvec_ঠেলো(&revision_args, argv[0]);
	জন্য (i = 1; i < argc; i++) {
		যদি (argv[i][0] != '-')
			strvec_ঠেলো(&stash_args, argv[i]);
		অন্যথা
			strvec_ঠেলো(&revision_args, argv[i]);
	}

	যদি (get_stash_info(&info, stash_args.nr, stash_args.v))
		যাও cleanup;

	/*
	 * The config settings are applied only if there are not passed
	 * any options.
	 */
	যদি (revision_args.nr == 1) {
		যদি (show_stat)
			rev.diffopt.output_format = DIFF_FORMAT_DIFFSTAT;

		যদি (show_patch)
			rev.diffopt.output_format |= DIFF_FORMAT_PATCH;

		যদি (!show_stat && !show_patch) {
			ret = 0;
			যাও cleanup;
		}
	}

	setup_revisions_from_strvec(&revision_args, &rev, ফাঁকা);
	যদি (revision_args.nr > 1)
		যাও usage;
	যদি (!rev.diffopt.output_format) {
		rev.diffopt.output_format = DIFF_FORMAT_PATCH;
		diff_setup_done(&rev.diffopt);
	}

	rev.diffopt.flags.recursive = 1;
	setup_diff_pager(&rev.diffopt);
	নির্বাচন (show_untracked) {
	ক্ষেত্র UNTRACKED_NONE:
		diff_বৃক্ষ_oid(&info.b_commit, &info.w_commit, "", &rev.diffopt);
		বিরতি;
	ক্ষেত্র UNTRACKED_ONLY:
		যদি (info.has_u)
			diff_মূল_বৃক্ষ_oid(&info.u_বৃক্ষ, "", &rev.diffopt);
		বিরতি;
	ক্ষেত্র UNTRACKED_INCLUDE:
		যদি (info.has_u)
			diff_include_untracked(&info, &rev.diffopt);
		অন্যথা
			diff_বৃক্ষ_oid(&info.b_commit, &info.w_commit, "", &rev.diffopt);
		বিরতি;
	}
	log_বৃক্ষ_diff_flush(&rev);

	ret = diff_ফলাফল_code(&rev);

cleanup:
	strvec_clear(&revision_args);
	strvec_clear(&stash_args);
	free_stash_info(&info);
	release_revisions(&rev);
	যদি (do_usage)
		usage_with_options(git_stash_show_usage, options);
	ফেরত ret;
usage:
	do_usage = 1;
	যাও cleanup;
}

স্থির পূর্ণসংখ্যা do_store_stash(ধ্রুবক কাঠামো object_id *w_commit, ধ্রুবক অক্ষর *stash_বার্তা,
			  পূর্ণসংখ্যা quiet)
{
	কাঠামো stash_info info;
	অক্ষর revision[GIT_MAX_HEXSZ];

	oid_to_hex_r(revision, w_commit);
	assert_stash_like(&info, revision);

	যদি (!stash_বার্তা)
		stash_বার্তা = "Created via \"git stash store\".";

	যদি (refs_update_ref(get_main_ref_store(the_repository), stash_বার্তা, ref_stash, w_commit, ফাঁকা,
			    REF_FORCE_CREATE_REFLOG,
			    quiet ? UPDATE_REFS_QUIET_ON_ERR :
			    UPDATE_REFS_বার্তা_ON_ERR)) {
		যদি (!quiet) {
			fprintf_ln(stderr, _("Cannot update %s with %s"),
				   ref_stash, oid_to_hex(w_commit));
		}
		ফেরত -1;
	}

	ফেরত 0;
}

স্থির পূর্ণসংখ্যা store_stash(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix,
		       কাঠামো repository *repo UNUSED)
{
	পূর্ণসংখ্যা quiet = 0;
	ধ্রুবক অক্ষর *stash_বার্তা = ফাঁকা;
	কাঠামো object_id obj;
	কাঠামো option options[] = {
		OPT__QUIET(&quiet, N_("be quiet")),
		OPT_STRING('m', "message", &stash_বার্তা, "message",
			   N_("stash message")),
		OPT_END()
	};
	পূর্ণসংখ্যা ret;

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_store_usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT);

	যদি (argc != 1) {
		যদি (!quiet)
			fprintf_ln(stderr, _("\"git stash store\" requires one "
					     "<commit> argument"));
		ফেরত -1;
	}

	যদি (repo_get_oid_with_flags(the_repository, argv[0], &obj,
				    quiet ? GET_OID_QUIETLY : 0)) {
		যদি (!quiet)
			fprintf_ln(stderr, _("Cannot update %s with %s"),
					     ref_stash, argv[0]);
		ret = -1;
		যাও out;
	}

	ret = do_store_stash(&obj, stash_বার্তা, quiet);

out:
	ফেরত ret;
}

স্থির শূন্য add_pathspecs(কাঠামো strvec *args,
			  ধ্রুবক কাঠামো pathspec *ps) {
	পূর্ণসংখ্যা i;

	জন্য (i = 0; i < ps->nr; i++)
		strvec_ঠেলো(args, ps->বস্তুসমূহ[i].original);
}

/*
 * `untracked_files` will be filled with the names of untracked files.
 * The return value is:
 *
 * = 0 if there are not any untracked files
 * > 0 if there are untracked files
 */
স্থির পূর্ণসংখ্যা get_untracked_files(ধ্রুবক কাঠামো pathspec *ps, পূর্ণসংখ্যা include_untracked,
			       কাঠামো strbuf *untracked_files)
{
	পূর্ণসংখ্যা i;
	পূর্ণসংখ্যা found = 0;
	কাঠামো dir_struct dir = DIR_INIT;

	যদি (include_untracked != INCLUDE_ALL_FILES)
		setup_standard_excludes(&dir);

	fill_directory(&dir, the_repository->সূচক, ps);
	জন্য (i = 0; i < dir.nr; i++) {
		কাঠামো dir_entry *ent = dir.entries[i];
		found++;
		strbuf_addstr(untracked_files, ent->name);
		/* NUL-terminate: will be fed to update-index -z */
		strbuf_addch(untracked_files, '\0');
	}

	dir_clear(&dir);
	ফেরত found;
}

/*
 * The return value of `check_changes_tracked_files()` can be:
 *
 * < 0 if there was an error
 * = 0 if there are no changes.
 * > 0 if there are changes.
 */
স্থির পূর্ণসংখ্যা check_changes_tracked_files(ধ্রুবক কাঠামো pathspec *ps)
{
	কাঠামো rev_info rev;
	কাঠামো object_id dummy;
	পূর্ণসংখ্যা ret = 0;

	/* No initial commit. */
	যদি (repo_get_oid(the_repository, "HEAD", &dummy))
		ফেরত -1;

	যদি (repo_read_সূচক(the_repository) < 0)
		ফেরত -1;

	repo_init_revisions(the_repository, &rev, ফাঁকা);
	copy_pathspec(&rev.prune_তথ্য, ps);

	rev.diffopt.flags.quick = 1;
	rev.diffopt.flags.ignore_submodules = 1;
	rev.abbrev = 0;

	add_শীর্ষ_to_pending(&rev);
	diff_setup_done(&rev.diffopt);

	run_diff_সূচক(&rev, DIFF_সূচক_CACHED);
	যদি (diff_ফলাফল_code(&rev)) {
		ret = 1;
		যাও done;
	}

	run_diff_files(&rev, 0);
	যদি (diff_ফলাফল_code(&rev)) {
		ret = 1;
		যাও done;
	}

done:
	release_revisions(&rev);
	ফেরত ret;
}

/*
 * The function will fill `untracked_files` with the names of untracked files
 * It will return 1 if there were any changes and 0 if there were not.
 */
স্থির পূর্ণসংখ্যা check_changes(ধ্রুবক কাঠামো pathspec *ps, পূর্ণসংখ্যা include_untracked,
			 কাঠামো strbuf *untracked_files)
{
	পূর্ণসংখ্যা ret = 0;
	যদি (check_changes_tracked_files(ps))
		ret = 1;

	যদি (include_untracked && get_untracked_files(ps, include_untracked,
						     untracked_files))
		ret = 1;

	ফেরত ret;
}

স্থির পূর্ণসংখ্যা save_untracked_files(কাঠামো stash_info *info, কাঠামো strbuf *বার্তা,
				কাঠামো strbuf *files)
{
	পূর্ণসংখ্যা ret = 0;
	কাঠামো strbuf untracked_বার্তা = STRBUF_INIT;
	কাঠামো child_process cp_upd_সূচক = CHILD_PROCESS_INIT;
	কাঠামো সূচক_state istate = সূচক_STATE_INIT(the_repository);

	cp_upd_সূচক.git_cmd = 1;
	strvec_pushl(&cp_upd_সূচক.args, "update-index", "-z", "--add",
		     "--remove", "--stdin", ফাঁকা);
	strvec_pushf(&cp_upd_সূচক.env, "GIT_INDEX_FILE=%s",
			 stash_সূচক_path.buf);

	strbuf_addf(&untracked_বার্তা, "untracked files on %s\n", বার্তা->buf);
	যদি (pipe_command(&cp_upd_সূচক, files->buf, files->len, ফাঁকা, 0,
			 ফাঁকা, 0)) {
		ret = -1;
		যাও done;
	}

	যদি (write_সূচক_as_বৃক্ষ(&info->u_বৃক্ষ, &istate, stash_সূচক_path.buf, 0,
				ফাঁকা)) {
		ret = -1;
		যাও done;
	}

	যদি (commit_বৃক্ষ(untracked_বার্তা.buf, untracked_বার্তা.len,
			&info->u_বৃক্ষ, ফাঁকা, &info->u_commit, ফাঁকা, ফাঁকা)) {
		ret = -1;
		যাও done;
	}

done:
	release_সূচক(&istate);
	strbuf_release(&untracked_বার্তা);
	remove_path(stash_সূচক_path.buf);
	ফেরত ret;
}

স্থির পূর্ণসংখ্যা stash_staged(কাঠামো stash_info *info, কাঠামো strbuf *out_patch,
			পূর্ণসংখ্যা quiet)
{
	পূর্ণসংখ্যা ret = 0;
	কাঠামো child_process cp_diff_বৃক্ষ = CHILD_PROCESS_INIT;
	কাঠামো সূচক_state istate = সূচক_STATE_INIT(the_repository);

	যদি (write_সূচক_as_বৃক্ষ(&info->w_বৃক্ষ, &istate, the_repository->সূচক_file,
				0, ফাঁকা)) {
		ret = -1;
		যাও done;
	}

	cp_diff_বৃক্ষ.git_cmd = 1;
	strvec_pushl(&cp_diff_বৃক্ষ.args, "diff-tree", "-p", "--binary",
		     "--no-color",
		     "-U1", "HEAD", oid_to_hex(&info->w_বৃক্ষ), "--", ফাঁকা);
	যদি (pipe_command(&cp_diff_বৃক্ষ, ফাঁকা, 0, out_patch, 0, ফাঁকা, 0)) {
		ret = -1;
		যাও done;
	}

	যদি (!out_patch->len) {
		যদি (!quiet)
			fprintf_ln(stderr, _("No staged changes"));
		ret = 1;
	}

done:
	release_সূচক(&istate);
	ফেরত ret;
}

স্থির পূর্ণসংখ্যা stash_patch(কাঠামো stash_info *info, ধ্রুবক কাঠামো pathspec *ps,
		       কাঠামো strbuf *out_patch, পূর্ণসংখ্যা quiet,
		       কাঠামো interactive_options *interactive_opts)
{
	পূর্ণসংখ্যা ret = 0;
	কাঠামো child_process cp_diff_বৃক্ষ = CHILD_PROCESS_INIT;
	কাঠামো commit *শীর্ষ_commit;
	ধ্রুবক কাঠামো object_id *শীর্ষ_বৃক্ষ;
	কাঠামো সূচক_state istate = সূচক_STATE_INIT(the_repository);
	অক্ষর *old_সূচক_env = ফাঁকা, *old_repo_সূচক_file;

	remove_path(stash_সূচক_path.buf);

	শীর্ষ_commit = lookup_commit(the_repository, &info->b_commit);
	যদি (!শীর্ষ_commit || repo_parse_commit(the_repository, শীর্ষ_commit)) {
		ret = -1;
		যাও done;
	}
	শীর্ষ_বৃক্ষ = get_commit_বৃক্ষ_oid(শীর্ষ_commit);
	যদি (!শীর্ষ_বৃক্ষ) {
		ret = -1;
		যাও done;
	}

	যদি (create_সূচক_from_বৃক্ষ(শীর্ষ_বৃক্ষ, stash_সূচক_path.buf)) {
		ret = -1;
		যাও done;
	}

	/* Find out what the user wants. */
	old_repo_সূচক_file = the_repository->সূচক_file;
	the_repository->সূচক_file = stash_সূচক_path.buf;
	old_সূচক_env = xstrdup_or_null(getenv(সূচক_ENVIRONMENT));
	setenv(সূচক_ENVIRONMENT, the_repository->সূচক_file, 1);

	ret = !!run_add_p(the_repository, ADD_P_STASH, interactive_opts, ফাঁকা, ps, 0);

	the_repository->সূচক_file = old_repo_সূচক_file;
	যদি (old_সূচক_env && *old_সূচক_env)
		setenv(সূচক_ENVIRONMENT, old_সূচক_env, 1);
	অন্যথা
		unsetenv(সূচক_ENVIRONMENT);
	FREE_AND_NULL(old_সূচক_env);

	/* State of the working tree. */
	যদি (write_সূচক_as_বৃক্ষ(&info->w_বৃক্ষ, &istate, stash_সূচক_path.buf, 0,
				ফাঁকা)) {
		ret = -1;
		যাও done;
	}

	cp_diff_বৃক্ষ.git_cmd = 1;
	strvec_pushl(&cp_diff_বৃক্ষ.args, "diff-tree", "-p", "-U1", "HEAD",
		     "--no-color",
		     oid_to_hex(&info->w_বৃক্ষ), "--", ফাঁকা);
	যদি (pipe_command(&cp_diff_বৃক্ষ, ফাঁকা, 0, out_patch, 0, ফাঁকা, 0)) {
		ret = -1;
		যাও done;
	}

	যদি (!out_patch->len) {
		যদি (!quiet)
			fprintf_ln(stderr, _("No changes selected"));
		ret = 1;
	}

done:
	release_সূচক(&istate);
	remove_path(stash_সূচক_path.buf);
	ফেরত ret;
}

স্থির পূর্ণসংখ্যা stash_working_বৃক্ষ(কাঠামো stash_info *info, ধ্রুবক কাঠামো pathspec *ps)
{
	পূর্ণসংখ্যা ret = 0;
	কাঠামো rev_info rev;
	কাঠামো child_process cp_upd_সূচক = CHILD_PROCESS_INIT;
	কাঠামো strbuf diff_output = STRBUF_INIT;
	কাঠামো সূচক_state istate = সূচক_STATE_INIT(the_repository);

	repo_init_revisions(the_repository, &rev, ফাঁকা);
	copy_pathspec(&rev.prune_তথ্য, ps);

	set_alternate_সূচক_output(stash_সূচক_path.buf);
	যদি (reset_বৃক্ষ(&info->i_বৃক্ষ, 0, 0)) {
		ret = -1;
		যাও done;
	}
	set_alternate_সূচক_output(ফাঁকা);

	rev.diffopt.output_format = DIFF_FORMAT_CALLBACK;
	rev.diffopt.format_callback = add_diff_to_buf;
	rev.diffopt.format_callback_তথ্য = &diff_output;

	যদি (repo_read_সূচক_preload(the_repository, &rev.diffopt.pathspec, 0) < 0) {
		ret = -1;
		যাও done;
	}

	add_pending_object(&rev, parse_object(the_repository, &info->b_commit),
			   "");
	run_diff_সূচক(&rev, 0);

	cp_upd_সূচক.git_cmd = 1;
	strvec_pushl(&cp_upd_সূচক.args, "update-index",
		     "--ignore-skip-worktree-entries",
		     "-z", "--add", "--remove", "--stdin", ফাঁকা);
	strvec_pushf(&cp_upd_সূচক.env, "GIT_INDEX_FILE=%s",
		     stash_সূচক_path.buf);

	যদি (pipe_command(&cp_upd_সূচক, diff_output.buf, diff_output.len,
			 ফাঁকা, 0, ফাঁকা, 0)) {
		ret = -1;
		যাও done;
	}

	যদি (write_সূচক_as_বৃক্ষ(&info->w_বৃক্ষ, &istate, stash_সূচক_path.buf, 0,
				ফাঁকা)) {
		ret = -1;
		যাও done;
	}

done:
	release_সূচক(&istate);
	release_revisions(&rev);
	strbuf_release(&diff_output);
	remove_path(stash_সূচক_path.buf);
	ফেরত ret;
}

স্থির পূর্ণসংখ্যা do_create_stash(ধ্রুবক কাঠামো pathspec *ps, কাঠামো strbuf *stash_বার্তা_buf,
			   পূর্ণসংখ্যা include_untracked, পূর্ণসংখ্যা patch_mode,
			   কাঠামো interactive_options *interactive_opts,
			   পূর্ণসংখ্যা only_staged, কাঠামো stash_info *info, কাঠামো strbuf *patch,
			   পূর্ণসংখ্যা quiet)
{
	পূর্ণসংখ্যা ret = 0;
	পূর্ণসংখ্যা flags = 0;
	পূর্ণসংখ্যা untracked_commit_option = 0;
	ধ্রুবক অক্ষর *শীর্ষ_short_sha1 = ফাঁকা;
	ধ্রুবক অক্ষর *branch_ref = ফাঁকা;
	ধ্রুবক অক্ষর *branch_name = "(no branch)";
	অক্ষর *branch_name_buf = ফাঁকা;
	কাঠামো commit *শীর্ষ_commit = ফাঁকা;
	কাঠামো commit_list *parents = ফাঁকা;
	কাঠামো strbuf বার্তা = STRBUF_INIT;
	কাঠামো strbuf commit_বৃক্ষ_label = STRBUF_INIT;
	কাঠামো strbuf untracked_files = STRBUF_INIT;

	prepare_fallback_ident("git stash", "git@stash");

	repo_read_সূচক_preload(the_repository, ফাঁকা, 0);
	যদি (repo_refresh_and_write_সূচক(the_repository, REFRESH_QUIET, 0, 0,
					 ফাঁকা, ফাঁকা, ফাঁকা) < 0) {
		ret = error(_("could not write index"));
		যাও done;
	}

	যদি (repo_get_oid(the_repository, "HEAD", &info->b_commit)) {
		যদি (!quiet)
			fprintf_ln(stderr, _("You do not have "
					     "the initial commit yet"));
		ret = -1;
		যাও done;
	} অন্যথা {
		শীর্ষ_commit = lookup_commit(the_repository, &info->b_commit);
	}

	যদি (!check_changes(ps, include_untracked, &untracked_files)) {
		ret = 1;
		যাও done;
	}

	branch_ref = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
					     "HEAD", 0, ফাঁকা, &flags);

	যদি (flags & REF_ISSYMREF) {
		যদি (skip_prefix(branch_ref, "refs/heads/", &branch_name))
			branch_name = branch_name_buf = xstrdup(branch_name);
	}

	শীর্ষ_short_sha1 = repo_find_unique_abbrev(the_repository,
						  &শীর্ষ_commit->object.oid,
						  DEFAULT_ABBREV);
	strbuf_addf(&বার্তা, "%s: %s ", branch_name, শীর্ষ_short_sha1);
	pp_commit_easy(CMIT_FMT_ONELINE, শীর্ষ_commit, &বার্তা);

	strbuf_addf(&commit_বৃক্ষ_label, "index on %s\n", বার্তা.buf);
	commit_list_ঢোকাও(শীর্ষ_commit, &parents);
	যদি (write_সূচক_as_বৃক্ষ(&info->i_বৃক্ষ, the_repository->সূচক,
				repo_get_সূচক_file(the_repository), 0, ফাঁকা) ||
	    commit_বৃক্ষ(commit_বৃক্ষ_label.buf, commit_বৃক্ষ_label.len,
			&info->i_বৃক্ষ, parents, &info->i_commit, ফাঁকা, ফাঁকা)) {
		যদি (!quiet)
			fprintf_ln(stderr, _("Cannot save the current "
					     "index state"));
		ret = -1;
		যাও done;
	}

	commit_list_free(parents);
	parents = ফাঁকা;

	যদি (include_untracked) {
		যদি (save_untracked_files(info, &বার্তা, &untracked_files)) {
			যদি (!quiet)
				fprintf_ln(stderr, _("Cannot save "
						     "the untracked files"));
			ret = -1;
			যাও done;
		}
		untracked_commit_option = 1;
	}
	যদি (patch_mode) {
		ret = stash_patch(info, ps, patch, quiet, interactive_opts);
		যদি (ret < 0) {
			যদি (!quiet)
				fprintf_ln(stderr, _("Cannot save the current "
						     "worktree state"));
			যাও done;
		} অন্যথা যদি (ret > 0) {
			যাও done;
		}
	} অন্যথা যদি (only_staged) {
		ret = stash_staged(info, patch, quiet);
		যদি (ret < 0) {
			যদি (!quiet)
				fprintf_ln(stderr, _("Cannot save the current "
						     "staged state"));
			যাও done;
		} অন্যথা যদি (ret > 0) {
			যাও done;
		}
	} অন্যথা {
		যদি (stash_working_বৃক্ষ(info, ps)) {
			যদি (!quiet)
				fprintf_ln(stderr, _("Cannot save the current "
						     "worktree state"));
			ret = -1;
			যাও done;
		}
	}

	যদি (!stash_বার্তা_buf->len)
		strbuf_addf(stash_বার্তা_buf, "WIP on %s", বার্তা.buf);
	অন্যথা
		strbuf_insertf(stash_বার্তা_buf, 0, "On %s: ", branch_name);

	যদি (untracked_commit_option)
		commit_list_ঢোকাও(lookup_commit(the_repository,
						 &info->u_commit),
				   &parents);
	commit_list_ঢোকাও(lookup_commit(the_repository, &info->i_commit),
			   &parents);
	commit_list_ঢোকাও(শীর্ষ_commit, &parents);

	যদি (commit_বৃক্ষ(stash_বার্তা_buf->buf, stash_বার্তা_buf->len, &info->w_বৃক্ষ,
			parents, &info->w_commit, ফাঁকা, ফাঁকা)) {
		যদি (!quiet)
			fprintf_ln(stderr, _("Cannot record "
					     "working tree state"));
		ret = -1;
		যাও done;
	}

done:
	strbuf_release(&commit_বৃক্ষ_label);
	strbuf_release(&বার্তা);
	strbuf_release(&untracked_files);
	commit_list_free(parents);
	মুক্ত(branch_name_buf);
	ফেরত ret;
}

স্থির পূর্ণসংখ্যা create_stash(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix UNUSED,
			কাঠামো repository *repo UNUSED)
{
	পূর্ণসংখ্যা ret;
	কাঠামো strbuf stash_বার্তা_buf = STRBUF_INIT;
	কাঠামো stash_info info = STASH_INFO_INIT;
	কাঠামো pathspec ps;

	/* Starting with argv[1], since argv[0] is "create" */
	strbuf_join_argv(&stash_বার্তা_buf, argc - 1, ++argv, ' ');

	memset(&ps, 0, আকার(ps));
	যদি (!check_changes_tracked_files(&ps))
		ফেরত 0;

	ret = do_create_stash(&ps, &stash_বার্তা_buf, 0, 0, ফাঁকা, 0, &info,
			      ফাঁকা, 0);
	যদি (!ret)
		printf_ln("%s", oid_to_hex(&info.w_commit));

	free_stash_info(&info);
	strbuf_release(&stash_বার্তা_buf);
	ফেরত ret;
}

স্থির পূর্ণসংখ্যা do_ঠেলো_stash(ধ্রুবক কাঠামো pathspec *ps, ধ্রুবক অক্ষর *stash_বার্তা, পূর্ণসংখ্যা quiet,
			 পূর্ণসংখ্যা keep_সূচক, পূর্ণসংখ্যা patch_mode,
			 কাঠামো interactive_options *interactive_opts,
			 পূর্ণসংখ্যা include_untracked, পূর্ণসংখ্যা only_staged)
{
	পূর্ণসংখ্যা ret = 0;
	কাঠামো stash_info info = STASH_INFO_INIT;
	কাঠামো strbuf patch = STRBUF_INIT;
	কাঠামো strbuf stash_বার্তা_buf = STRBUF_INIT;
	কাঠামো strbuf untracked_files = STRBUF_INIT;
	কাঠামো strbuf out = STRBUF_INIT;

	যদি (patch_mode && keep_সূচক == -1)
		keep_সূচক = 1;

	যদি (patch_mode && include_untracked) {
		fprintf_ln(stderr, _("Can't use --patch and --include-untracked"
				     " or --all at the same time"));
		ret = -1;
		যাও done;
	}

	/* --patch overrides --staged */
	যদি (patch_mode)
		only_staged = 0;

	যদি (only_staged && include_untracked) {
		fprintf_ln(stderr, _("Can't use --staged and --include-untracked"
				     " or --all at the same time"));
		ret = -1;
		যাও done;
	}

	repo_read_সূচক_preload(the_repository, ফাঁকা, 0);
	যদি (!include_untracked && ps->nr) {
		অক্ষর *ps_matched = xcalloc(ps->nr, 1);

		যদি (pathspec_needs_expanded_সূচক(the_repository->সূচক, ps))
			ensure_full_সূচক(the_repository->সূচক);
		জন্য (আকার_t i = 0; i < the_repository->সূচক->cache_nr; i++)
			ce_path_match(the_repository->সূচক, the_repository->সূচক->cache[i], ps,
				      ps_matched);

		যদি (report_path_error(ps_matched, ps)) {
			fprintf_ln(stderr, _("Did you forget to 'git add'?"));
			ret = -1;
			মুক্ত(ps_matched);
			যাও done;
		}
		মুক্ত(ps_matched);
	}

	যদি (repo_refresh_and_write_সূচক(the_repository, REFRESH_QUIET, 0, 0,
					 ফাঁকা, ফাঁকা, ফাঁকা)) {
		ret = error(_("could not write index"));
		যাও done;
	}

	যদি (!check_changes(ps, include_untracked, &untracked_files)) {
		যদি (!quiet)
			printf_ln(_("No local changes to save"));
		যাও done;
	}

	যদি (!refs_reflog_exists(get_main_ref_store(the_repository), ref_stash) && do_clear_stash()) {
		ret = -1;
		যদি (!quiet)
			fprintf_ln(stderr, _("Cannot initialize stash"));
		যাও done;
	}

	যদি (stash_বার্তা)
		strbuf_addstr(&stash_বার্তা_buf, stash_বার্তা);
	যদি (do_create_stash(ps, &stash_বার্তা_buf, include_untracked, patch_mode,
			    interactive_opts, only_staged, &info, &patch, quiet)) {
		ret = -1;
		যাও done;
	}

	যদি (do_store_stash(&info.w_commit, stash_বার্তা_buf.buf, 1)) {
		ret = -1;
		যদি (!quiet)
			fprintf_ln(stderr, _("Cannot save the current status"));
		যাও done;
	}

	যদি (!quiet)
		printf_ln(_("Saved working directory and index state %s"),
			  stash_বার্তা_buf.buf);

	যদি (!(patch_mode || only_staged)) {
		যদি (include_untracked && !ps->nr) {
			কাঠামো child_process cp = CHILD_PROCESS_INIT;

			cp.git_cmd = 1;
			যদি (startup_info->original_cwd) {
				cp.dir = startup_info->original_cwd;
				strvec_pushf(&cp.env, "%s=%s",
					     GIT_WORK_বৃক্ষ_ENVIRONMENT,
					     the_repository->worktree);
			}
			strvec_pushl(&cp.args, "clean", "--force",
				     "--quiet", "-d", ":/", ফাঁকা);
			যদি (include_untracked == INCLUDE_ALL_FILES)
				strvec_ঠেলো(&cp.args, "-x");
			যদি (run_command(&cp)) {
				ret = -1;
				যাও done;
			}
		}
		discard_সূচক(the_repository->সূচক);
		যদি (ps->nr) {
			কাঠামো child_process cp_add = CHILD_PROCESS_INIT;
			কাঠামো child_process cp_diff = CHILD_PROCESS_INIT;
			কাঠামো child_process cp_apply = CHILD_PROCESS_INIT;

			cp_add.git_cmd = 1;
			strvec_ঠেলো(&cp_add.args, "add");
			যদি (!include_untracked)
				strvec_ঠেলো(&cp_add.args, "-u");
			যদি (include_untracked == INCLUDE_ALL_FILES)
				strvec_ঠেলো(&cp_add.args, "--force");
			strvec_ঠেলো(&cp_add.args, "--");
			add_pathspecs(&cp_add.args, ps);
			যদি (run_command(&cp_add)) {
				ret = -1;
				যাও done;
			}

			cp_diff.git_cmd = 1;
			strvec_pushl(&cp_diff.args, "diff-index", "-p",
				     "--no-color",
				     "--cached", "--binary", "HEAD", "--",
				     ফাঁকা);
			add_pathspecs(&cp_diff.args, ps);
			যদি (pipe_command(&cp_diff, ফাঁকা, 0, &out, 0, ফাঁকা, 0)) {
				ret = -1;
				যাও done;
			}

			cp_apply.git_cmd = 1;
			strvec_pushl(&cp_apply.args, "apply", "--index",
				     "-R", ফাঁকা);
			যদি (pipe_command(&cp_apply, out.buf, out.len, ফাঁকা, 0,
					 ফাঁকা, 0)) {
				ret = -1;
				যাও done;
			}
		} অন্যথা {
			কাঠামো child_process cp = CHILD_PROCESS_INIT;
			cp.git_cmd = 1;
			/* BUG: this nukes untracked files in the way */
			strvec_pushl(&cp.args, "reset", "--hard", "-q",
				     "--no-recurse-submodules", ফাঁকা);
			যদি (run_command(&cp)) {
				ret = -1;
				যাও done;
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
		যদি (keep_সূচক == 1 && !is_null_oid(&info.i_বৃক্ষ) &&
		    !is_empty_বৃক্ষ_oid(&info.i_বৃক্ষ, the_repository->hash_algo)) {
			কাঠামো child_process cp = CHILD_PROCESS_INIT;

			cp.git_cmd = 1;
			strvec_pushl(&cp.args, "checkout", "--no-overlay",
				     oid_to_hex(&info.i_বৃক্ষ), "--", ফাঁকা);
			যদি (!ps->nr)
				strvec_ঠেলো(&cp.args, ":/");
			অন্যথা
				add_pathspecs(&cp.args, ps);
			যদি (run_command(&cp)) {
				ret = -1;
				যাও done;
			}
		}
		যাও done;
	} অন্যথা {
		কাঠামো child_process cp = CHILD_PROCESS_INIT;

		cp.git_cmd = 1;
		strvec_pushl(&cp.args, "apply", "-R", ফাঁকা);

		যদি (pipe_command(&cp, patch.buf, patch.len, ফাঁকা, 0, ফাঁকা, 0)) {
			যদি (!quiet)
				fprintf_ln(stderr, _("Cannot remove "
						     "worktree changes"));
			ret = -1;
			যাও done;
		}

		যদি (keep_সূচক < 1) {
			কাঠামো child_process cp = CHILD_PROCESS_INIT;

			cp.git_cmd = 1;
			strvec_pushl(&cp.args, "reset", "-q", "--refresh", "--",
				     ফাঁকা);
			add_pathspecs(&cp.args, ps);
			যদি (run_command(&cp)) {
				ret = -1;
				যাও done;
			}
		}
		যাও done;
	}

done:
	strbuf_release(&patch);
	strbuf_release(&out);
	free_stash_info(&info);
	strbuf_release(&stash_বার্তা_buf);
	strbuf_release(&untracked_files);
	ফেরত ret;
}

স্থির পূর্ণসংখ্যা ঠেলো_stash(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix,
		      পূর্ণসংখ্যা ঠেলো_assumed)
{
	পূর্ণসংখ্যা force_assume = 0;
	পূর্ণসংখ্যা keep_সূচক = -1;
	পূর্ণসংখ্যা only_staged = 0;
	পূর্ণসংখ্যা patch_mode = 0;
	পূর্ণসংখ্যা include_untracked = 0;
	পূর্ণসংখ্যা quiet = 0;
	পূর্ণসংখ্যা pathspec_file_nul = 0;
	ধ্রুবক অক্ষর *stash_বার্তা = ফাঁকা;
	অক্ষর *pathspec_from_file = ফাঁকা;
	কাঠামো pathspec ps;
	কাঠামো interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	কাঠামো option options[] = {
		OPT_BOOL('k', "keep-index", &keep_সূচক,
			 N_("keep index")),
		OPT_BOOL('S', "staged", &only_staged,
			 N_("stash staged changes only")),
		OPT_BOOL('p', "patch", &patch_mode,
			 N_("stash in patch mode")),
		OPT_BOOL(0, "auto-advance", &interactive_opts.auto_advance,
			 N_("auto advance to the next file when selecting hunks interactively")),
		OPT_DIFF_UNIFIED(&interactive_opts.context),
		OPT_DIFF_INTERHUNK_CONTEXT(&interactive_opts.interhunkcontext),
		OPT__QUIET(&quiet, N_("quiet mode")),
		OPT_BOOL('u', "include-untracked", &include_untracked,
			 N_("include untracked files in stash")),
		OPT_SET_INT('a', "all", &include_untracked,
			    N_("include ignore files"), INCLUDE_ALL_FILES),
		OPT_STRING('m', "message", &stash_বার্তা, N_("message"),
			   N_("stash message")),
		OPT_PATHSPEC_FROM_FILE(&pathspec_from_file),
		OPT_PATHSPEC_FILE_NUL(&pathspec_file_nul),
		OPT_END()
	};
	পূর্ণসংখ্যা ret;

	যদি (argc) {
		পূর্ণসংখ্যা flags = PARSE_OPT_KEEP_DASHDASH;

		যদি (ঠেলো_assumed)
			flags |= PARSE_OPT_STOP_AT_NON_OPTION;

		argc = parse_options(argc, argv, prefix, options,
				     ঠেলো_assumed ? git_stash_usage :
				     git_stash_ঠেলো_usage, flags);
		force_assume |= patch_mode;
	}

	যদি (argc) {
		যদি (!strcmp(argv[0], "--")) {
			argc--;
			argv++;
		} অন্যথা যদি (ঠেলো_assumed && !force_assume) {
			die("subcommand wasn't specified; 'push' can't be assumed due to unexpected token '%s'",
			    argv[0]);
		}
	}

	parse_pathspec(&ps, 0, PATHSPEC_PREFER_FULL | PATHSPEC_PREFIX_ORIGIN,
		       prefix, argv);

	যদি (pathspec_from_file) {
		যদি (patch_mode)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--patch");

		যদি (only_staged)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--staged");

		যদি (ps.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&ps, 0,
				    PATHSPEC_PREFER_FULL | PATHSPEC_PREFIX_ORIGIN,
				    prefix, pathspec_from_file, pathspec_file_nul);
	} অন্যথা যদি (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	যদি (!patch_mode) {
		যদি (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--patch");
		যদি (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--patch");
		যদি (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--patch");
	}

	যদি (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	যদি (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	ret = do_ঠেলো_stash(&ps, stash_বার্তা, quiet, keep_সূচক, patch_mode,
			    &interactive_opts, include_untracked, only_staged);

	clear_pathspec(&ps);
	মুক্ত(pathspec_from_file);
	ফেরত ret;
}

স্থির পূর্ণসংখ্যা ঠেলো_stash_unassumed(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix,
				কাঠামো repository *repo UNUSED)
{
	ফেরত ঠেলো_stash(argc, argv, prefix, 0);
}

স্থির পূর্ণসংখ্যা save_stash(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix,
		      কাঠামো repository *repo UNUSED)
{
	পূর্ণসংখ্যা keep_সূচক = -1;
	পূর্ণসংখ্যা only_staged = 0;
	পূর্ণসংখ্যা patch_mode = 0;
	পূর্ণসংখ্যা include_untracked = 0;
	পূর্ণসংখ্যা quiet = 0;
	পূর্ণসংখ্যা ret = 0;
	ধ্রুবক অক্ষর *stash_বার্তা = ফাঁকা;
	কাঠামো pathspec ps;
	কাঠামো strbuf stash_বার্তা_buf = STRBUF_INIT;
	কাঠামো interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	কাঠামো option options[] = {
		OPT_BOOL('k', "keep-index", &keep_সূচক,
			 N_("keep index")),
		OPT_BOOL('S', "staged", &only_staged,
			 N_("stash staged changes only")),
		OPT_BOOL('p', "patch", &patch_mode,
			 N_("stash in patch mode")),
		OPT_BOOL(0, "auto-advance", &interactive_opts.auto_advance,
			 N_("auto advance to the next file when selecting hunks interactively")),
		OPT_DIFF_UNIFIED(&interactive_opts.context),
		OPT_DIFF_INTERHUNK_CONTEXT(&interactive_opts.interhunkcontext),
		OPT__QUIET(&quiet, N_("quiet mode")),
		OPT_BOOL('u', "include-untracked", &include_untracked,
			 N_("include untracked files in stash")),
		OPT_SET_INT('a', "all", &include_untracked,
			    N_("include ignore files"), INCLUDE_ALL_FILES),
		OPT_STRING('m', "message", &stash_বার্তা, "message",
			   N_("stash message")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_save_usage,
			     PARSE_OPT_KEEP_DASHDASH);

	যদি (argc)
		stash_বার্তা = strbuf_join_argv(&stash_বার্তা_buf, argc, argv, ' ');

	memset(&ps, 0, আকার(ps));

	যদি (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	যদি (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	যদি (!patch_mode) {
		যদি (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--patch");
		যদি (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--patch");
		যদি (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--patch");
	}

	ret = do_ঠেলো_stash(&ps, stash_বার্তা, quiet, keep_সূচক,
			    patch_mode, &interactive_opts, include_untracked,
			    only_staged);

	strbuf_release(&stash_বার্তা_buf);
	ফেরত ret;
}

স্থির পূর্ণসংখ্যা write_commit_with_parents(কাঠামো repository *r,
				     কাঠামো object_id *out,
				     ধ্রুবক কাঠামো object_id *oid,
				     কাঠামো commit_list *parents)
{
	আকার_t author_len, committer_len;
	কাঠামো commit *this;
	ধ্রুবক অক্ষর *orig_author, *orig_committer;
	অক্ষর *author = ফাঁকা, *committer = ফাঁকা;
	ধ্রুবক অক্ষর *বাফার;
	unsigned long bufsize;
	ধ্রুবক অক্ষর *p;
	কাঠামো strbuf বার্তা = STRBUF_INIT;
	পূর্ণসংখ্যা ret = 0;
	কাঠামো ident_split id;

	this = lookup_commit_reference(r, oid);
	বাফার = repo_get_commit_বাফার(r, this, &bufsize);
	orig_author = find_commit_header(বাফার, "author", &author_len);
	orig_committer = find_commit_header(বাফার, "committer", &committer_len);

	যদি (!orig_author || !orig_committer) {
		ret = error(_("cannot parse commit %s"), oid_to_hex(oid));
		যাও out;
	}

	যদি (split_ident_সারি(&id, orig_author, author_len) < 0 ||
	    split_ident_সারি(&id, orig_committer, committer_len) < 0) {
		ret = error(_("invalid author or committer for %s"), oid_to_hex(oid));
		যাও out;
	}

	p = strstr(বাফার, "\n\n");
	strbuf_addstr(&বার্তা, "git stash: ");

	যদি (p)
		strbuf_add(&বার্তা, p + 2, bufsize - (p + 2 - বাফার));
	strbuf_complete_সারি(&বার্তা);

	author = xmemdupz(orig_author, author_len);
	committer = xmemdupz(orig_committer, committer_len);

	যদি (commit_বৃক্ষ_extended(বার্তা.buf, বার্তা.len,
				 r->hash_algo->empty_বৃক্ষ, parents,
				 out, author, committer,
				 ফাঁকা, ফাঁকা)) {
		ret = error(_("could not write commit"));
		যাও out;
	}
out:
	strbuf_release(&বার্তা);
	repo_unuse_commit_বাফার(r, this, বাফার);
	মুক্ত(author);
	মুক্ত(committer);
	ফেরত ret;
}

স্থির পূর্ণসংখ্যা do_import_stash(কাঠামো repository *r, ধ্রুবক অক্ষর *rev)
{
	কাঠামো object_id chain;
	পূর্ণসংখ্যা res = 0;
	ধ্রুবক অক্ষর *বাফার = ফাঁকা;
	unsigned long bufsize;
	কাঠামো commit *this = ফাঁকা;
	কাঠামো commit_list *বস্তুসমূহ = ফাঁকা, *cur;
	অক্ষর *বার্তা = ফাঁকা;

	যদি (repo_get_oid(r, rev, &chain))
		ফেরত error(_("not a valid revision: %s"), rev);

	this = lookup_commit_reference(r, &chain);
	যদি (!this)
		ফেরত error(_("not a commit: %s"), rev);

	/*
	 * Walk the commit history, finding each stash entry, and load data into
	 * the array.
	 */
	জন্য (;;) {
		ধ্রুবক অক্ষর *author, *committer;
		আকার_t author_len, committer_len;
		ধ্রুবক অক্ষর *p;
		ধ্রুবক অক্ষর *expected = "git stash <git@stash> 1000684800 +0000";
		ধ্রুবক অক্ষর *prefix = "git stash: ";
		কাঠামো commit *stash;
		কাঠামো বৃক্ষ *বৃক্ষ = repo_get_commit_বৃক্ষ(r, this);

		যদি (!বৃক্ষ ||
		    !oideq(&বৃক্ষ->object.oid, r->hash_algo->empty_বৃক্ষ) ||
		    (this->parents &&
		     (!this->parents->পরবর্তী || this->parents->পরবর্তী->পরবর্তী))) {
			res = error(_("%s is not a valid exported stash commit"),
					oid_to_hex(&this->object.oid));
			যাও out;
		}

		বাফার = repo_get_commit_বাফার(r, this, &bufsize);

		যদি (!this->parents) {
			/*
			 * We don't have any parents.  Make sure this is our
			 * root commit.
			 */
			author = find_commit_header(বাফার, "author", &author_len);
			committer = find_commit_header(বাফার, "committer", &committer_len);

			যদি (!author || !committer) {
				error(_("cannot parse commit %s"), oid_to_hex(&this->object.oid));
				যাও out;
			}

			যদি (author_len != strlen(expected) ||
			    committer_len != strlen(expected) ||
			    memcmp(author, expected, author_len) ||
			    memcmp(committer, expected, committer_len)) {
				res = error(_("found root commit %s with invalid data"), oid_to_hex(&this->object.oid));
				যাও out;
			}
			বিরতি;
		}

		p = strstr(বাফার, "\n\n");
		যদি (!p) {
			res = error(_("cannot parse commit %s"), oid_to_hex(&this->object.oid));
			যাও out;
		}

		p += 2;
		যদি (((আকার_t)(bufsize - (p - বাফার)) < strlen(prefix)) ||
		    memcmp(prefix, p, strlen(prefix))) {
			res = error(_("found stash commit %s without expected prefix"), oid_to_hex(&this->object.oid));
			যাও out;
		}

		stash = this->parents->পরবর্তী->item;

		যদি (repo_parse_commit(r, this->parents->item) ||
		    repo_parse_commit(r, stash)) {
			res = error(_("cannot parse parents of commit: %s"),
					oid_to_hex(&this->object.oid));
			যাও out;
		}

		যদি (check_stash_topology(r, stash)) {
			res = error(_("%s does not look like a stash commit"),
					oid_to_hex(&stash->object.oid));
			যাও out;
		}

		repo_unuse_commit_বাফার(r, this, বাফার);
		বাফার = ফাঁকা;
		বস্তুসমূহ = commit_list_ঢোকাও(stash, &বস্তুসমূহ);
		this = this->parents->item;
	}

	/*
	 * Now, walk each entry, adding it to the stash as a normal stash
	 * commit.
	 */
	জন্য (cur = বস্তুসমূহ; cur; cur = cur->পরবর্তী) {
		ধ্রুবক অক্ষর *p;
		কাঠামো object_id *oid;

		this = cur->item;
		oid = &this->object.oid;
		বাফার = repo_get_commit_বাফার(r, this, &bufsize);
		যদি (!বাফার) {
			res = error(_("cannot read commit buffer for %s"), oid_to_hex(oid));
			যাও out;
		}

		p = strstr(বাফার, "\n\n");
		যদি (!p) {
			res = error(_("cannot parse commit %s"), oid_to_hex(oid));
			যাও out;
		}

		p += 2;
		বার্তা = xmemdupz(p, bufsize - (p - বাফার));
		repo_unuse_commit_বাফার(r, this, বাফার);
		বাফার = ফাঁকা;

		যদি (do_store_stash(oid, বার্তা, 1)) {
			res = error(_("cannot save the stash for %s"), oid_to_hex(oid));
			যাও out;
		}
		FREE_AND_NULL(বার্তা);
	}
out:
	যদি (this && বাফার)
		repo_unuse_commit_বাফার(r, this, বাফার);
	commit_list_free(বস্তুসমূহ);
	মুক্ত(বার্তা);

	ফেরত res;
}

স্থির পূর্ণসংখ্যা import_stash(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix,
			কাঠামো repository *repo)
{
	কাঠামো option options[] = {
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_import_usage,
			     PARSE_OPT_KEEP_DASHDASH);

	যদি (argc != 1)
		usage_বার্তা_opt("a revision is required", git_stash_import_usage, options);

	ফেরত do_import_stash(repo, argv[0]);
}

কাঠামো stash_entry_তথ্য {
	কাঠামো repository *r;
	কাঠামো commit_list **বস্তুসমূহ;
	আকার_t গুনতি;
};

স্থির পূর্ণসংখ্যা collect_stash_entries(ধ্রুবক অক্ষর *refname UNUSED,
				 কাঠামো object_id *old_oid UNUSED,
				 কাঠামো object_id *new_oid,
				 ধ্রুবক অক্ষর *committer UNUSED,
				 timestamp_t timestamp UNUSED,
				 পূর্ণসংখ্যা tz UNUSED, ধ্রুবক অক্ষর *বার্তা UNUSED,
				 শূন্য *cb_তথ্য)
{
	কাঠামো stash_entry_তথ্য *তথ্য = cb_তথ্য;
	কাঠামো commit *stash;

	তথ্য->গুনতি++;
	stash = lookup_commit_reference(তথ্য->r, new_oid);
	যদি (!stash || check_stash_topology(তথ্য->r, stash)) {
		ফেরত error(_("%s does not look like a stash commit"),
			     oid_to_hex(new_oid));
	}
	তথ্য->বস্তুসমূহ = commit_list_append(stash, তথ্য->বস্তুসমূহ);
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা do_export_stash(কাঠামো repository *r,
			   ধ্রুবক অক্ষর *ref,
			   পূর্ণসংখ্যা argc,
			   ধ্রুবক অক্ষর **argv)
{
	কাঠামো object_id base;
	কাঠামো commit *পূর্ব;
	কাঠামো commit_list *বস্তুসমূহ = ফাঁকা, **iter = &বস্তুসমূহ, *cur;
	পূর্ণসংখ্যা res = 0;
	পূর্ণসংখ্যা i;
	কাঠামো strbuf revision = STRBUF_INIT;
	ধ্রুবক অক্ষর *author, *committer;

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
	যদি (commit_বৃক্ষ_extended("", 0, r->hash_algo->empty_বৃক্ষ, ফাঁকা,
				 &base, author, committer, ফাঁকা, ফাঁকা))
		ফেরত error(_("unable to write base commit"));

	পূর্ব = lookup_commit_reference(r, &base);

	যদি (argc) {
		/*
		 * Find each specified stash, and load data into the array.
		 */
		জন্য (i = 0; i < argc; i++) {
			কাঠামো object_id oid;
			কাঠামো commit *stash;

			যদি (parse_stash_revision(&revision, argv[i], 1) ||
			    repo_get_oid_with_flags(r, revision.buf, &oid,
						    GET_OID_QUIETLY |
						    GET_OID_GENTLY)) {
				res = error(_("unable to find stash entry %s"), argv[i]);
				যাও out;
			}

			stash = lookup_commit_reference(r, &oid);
			যদি (!stash || check_stash_topology(r, stash)) {
				res = error(_("%s does not look like a stash commit"),
						revision.buf);
				যাও out;
			}
			iter = commit_list_append(stash, iter);
		}
	} অন্যথা {
		/*
		 * Walk the reflog, finding each stash entry, and load data into the
		 * array.
		 */
		কাঠামো stash_entry_তথ্য cb_তথ্য = {
			.r = r, .বস্তুসমূহ = iter,
		};
		যদি (refs_for_each_reflog_ent_reverse(get_main_ref_store(r),
						     "refs/stash",
						     collect_stash_entries,
						     &cb_তথ্য) && cb_তথ্য.গুনতি)
			যাও out;
	}

	/*
	 * Now, create a set of commits identical to the regular stash commits,
	 * but where their first parents form a chain to our original empty
	 * base commit.
	 */
	বস্তুসমূহ = commit_list_reverse(বস্তুসমূহ);
	জন্য (cur = বস্তুসমূহ; cur; cur = cur->পরবর্তী) {
		কাঠামো commit_list *parents = ফাঁকা;
		কাঠামো commit_list **পরবর্তী = &parents;
		কাঠামো object_id out;
		কাঠামো commit *stash = cur->item;

		পরবর্তী = commit_list_append(পূর্ব, পরবর্তী);
		পরবর্তী = commit_list_append(stash, পরবর্তী);
		res = write_commit_with_parents(r, &out, &stash->object.oid, parents);
		commit_list_free(parents);
		যদি (res)
			যাও out;
		পূর্ব = lookup_commit_reference(r, &out);
	}
	যদি (ref)
		refs_update_ref(get_main_ref_store(r), ফাঁকা, ref,
				&পূর্ব->object.oid, ফাঁকা, 0, UPDATE_REFS_DIE_ON_ERR);
	অন্যথা
		puts(oid_to_hex(&পূর্ব->object.oid));
out:
	strbuf_release(&revision);
	commit_list_free(বস্তুসমূহ);

	ফেরত res;
}

গণনা export_action {
	ACTION_NONE,
	ACTION_PRINT,
	ACTION_TO_REF,
};

স্থির পূর্ণসংখ্যা export_stash(পূর্ণসংখ্যা argc,
			ধ্রুবক অক্ষর **argv,
			ধ্রুবক অক্ষর *prefix,
			কাঠামো repository *repo)
{
	ধ্রুবক অক্ষর *ref = ফাঁকা;
	গণনা export_action action = ACTION_NONE;
	কাঠামো option options[] = {
		OPT_CMDMODE(0, "print", &action,
			    N_("print the object ID instead of writing it to a ref"),
			    ACTION_PRINT),
		OPT_STRING(0, "to-ref", &ref, "ref",
			    N_("save the data to the given ref")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_export_usage,
			     PARSE_OPT_KEEP_DASHDASH);

	যদি (ref && action == ACTION_NONE)
		action = ACTION_TO_REF;

	যদি (action == ACTION_NONE || (ref && action == ACTION_PRINT))
		ফেরত error(_("exactly one of --print and --to-ref is required"));

	ফেরত do_export_stash(repo, ref, argc, argv);
}

পূর্ণসংখ্যা cmd_stash(পূর্ণসংখ্যা argc,
	      ধ্রুবক অক্ষর **argv,
	      ধ্রুবক অক্ষর *prefix,
	      কাঠামো repository *repo)
{
	pid_t pid = getpid();
	ধ্রুবক অক্ষর *সূচক_file;
	কাঠামো strvec args = STRVEC_INIT;
	parse_opt_subcommand_fn *fn = ফাঁকা;
	কাঠামো option options[] = {
		OPT_SUBCOMMAND("apply", &fn, apply_stash),
		OPT_SUBCOMMAND("clear", &fn, clear_stash),
		OPT_SUBCOMMAND("drop", &fn, drop_stash),
		OPT_SUBCOMMAND("pop", &fn, বের_করো_stash),
		OPT_SUBCOMMAND("branch", &fn, branch_stash),
		OPT_SUBCOMMAND("list", &fn, list_stash),
		OPT_SUBCOMMAND("show", &fn, show_stash),
		OPT_SUBCOMMAND("store", &fn, store_stash),
		OPT_SUBCOMMAND("create", &fn, create_stash),
		OPT_SUBCOMMAND("push", &fn, ঠেলো_stash_unassumed),
		OPT_SUBCOMMAND("export", &fn, export_stash),
		OPT_SUBCOMMAND("import", &fn, import_stash),
		OPT_SUBCOMMAND_F("save", &fn, save_stash, ফাঁকা, PARSE_OPT_NOCOMPLETE),
		OPT_END()
	};
	ধ্রুবক অক্ষর **args_copy;
	পূর্ণসংখ্যা ret;

	repo_config(the_repository, git_stash_config, ফাঁকা);

	argc = parse_options(argc, argv, prefix, options, git_stash_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL |
			     PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_সূচক = 0;

	সূচক_file = repo_get_সূচক_file(the_repository);
	strbuf_addf(&stash_সূচক_path, "%s.stash.%" PRIuMAX, সূচক_file,
		    (uintmax_t)pid);

	যদি (fn) {
		ret = fn(argc, argv, prefix, repo);

		/*
		 * The subcommand implementations return 0 on success, a
		 * negative value on failure, and STASH_APPLY_CONFLICT
		 * when applying a stash entry resulted in conflicts.
		 * Map failures to 128, the status die() uses, so that
		 * exit status 1 unambiguously indicates conflicts.
		 */
		যদি (ret < 0)
			ফেরত 128;
		ফেরত ret;
	} অন্যথা যদি (!argc) {
		ফেরত !!ঠেলো_stash_unassumed(0, ফাঁকা, prefix, repo);
	}

	/* Assume 'stash push' */
	strvec_ঠেলো(&args, "push");
	strvec_pushv(&args, argv);

	/*
	 * `push_stash()` ends up modifying the array, which causes memory
	 * leaks if we didn't copy the array here.
	 */
	DUP_ARRAY(args_copy, args.v, args.nr);

	ret = !!ঠেলো_stash(args.nr, args_copy, prefix, 1);

	strvec_clear(&args);
	মুক্ত(args_copy);
	ফেরত ret;
}
