#define USE_THE_REPOSITORY_VARIABLE

#অন্তর্ভাৱ "builtin.h"
#অন্তর্ভাৱ "abspath.h"
#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ "environment.h"
#অন্তর্ভাৱ "gettext.h"
#অন্তর্ভাৱ "hash.h"
#অন্তর্ভাৱ "hex.h"
#অন্তর্ভাৱ "object-name.h"
#অন্তর্ভাৱ "parse-options.h"
#অন্তর্ভাৱ "refs.h"
#অন্তর্ভাৱ "stash.h"
#অন্তর্ভাৱ "lockfile.h"
#অন্তর্ভাৱ "cache-tree.h"
#অন্তর্ভাৱ "unpack-trees.h"
#অন্তর্ভাৱ "merge-ort-wrappers.h"
#অন্তর্ভাৱ "strvec.h"
#অন্তর্ভাৱ "run-command.h"
#অন্তর্ভাৱ "dir.h"
#অন্তর্ভাৱ "entry.h"
#অন্তর্ভাৱ "preload-index.h"
#অন্তর্ভাৱ "read-cache.h"
#অন্তর্ভাৱ "repository.h"
#অন্তর্ভাৱ "rerere.h"
#অন্তর্ভাৱ "revision.h"
#অন্তর্ভাৱ "setup.h"
#অন্তর্ভাৱ "sparse-index.h"
#অন্তর্ভাৱ "log-tree.h"
#অন্তর্ভাৱ "diffcore.h"
#অন্তর্ভাৱ "reflog.h"
#অন্তর্ভাৱ "reflog-walk.h"
#অন্তর্ভাৱ "add-interactive.h"
#অন্তর্ভাৱ "commit.h"

#define INCLUDE_ALL_FILES 2

#define BUILTIN_STASH_LIST_USAGE \
	N_("git stash list [<log-options>]")
#define BUILTIN_STASH_SHOW_USAGE \
	N_("git stash show [-u | --include-untracked | --only-untracked] [<diff-options>] [<stash>]")
#define BUILTIN_STASH_DROP_USAGE \
	N_("git stash drop [-q | --quiet] [<stash>]")
#define BUILTIN_STASH_বেৰ_কৰো_USAGE \
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

স্থির ধ্রুৱ ৱর্ণ * ধ্রুৱ git_stash_usage[] = {
	BUILTIN_STASH_LIST_USAGE,
	BUILTIN_STASH_SHOW_USAGE,
	BUILTIN_STASH_DROP_USAGE,
	BUILTIN_STASH_বেৰ_কৰো_USAGE,
	BUILTIN_STASH_APPLY_USAGE,
	BUILTIN_STASH_BRANCH_USAGE,
	BUILTIN_STASH_ঠেলো_USAGE,
	BUILTIN_STASH_SAVE_USAGE,
	BUILTIN_STASH_CLEAR_USAGE,
	BUILTIN_STASH_CREATE_USAGE,
	BUILTIN_STASH_STORE_USAGE,
	BUILTIN_STASH_EXPORT_USAGE,
	BUILTIN_STASH_IMPORT_USAGE,
	রিক্ত
};

স্থির ধ্রুৱ ৱর্ণ * ধ্রুৱ git_stash_list_usage[] = {
	BUILTIN_STASH_LIST_USAGE,
	রিক্ত
};

স্থির ধ্রুৱ ৱর্ণ * ধ্রুৱ git_stash_show_usage[] = {
	BUILTIN_STASH_SHOW_USAGE,
	রিক্ত
};

স্থির ধ্রুৱ ৱর্ণ * ধ্রুৱ git_stash_drop_usage[] = {
	BUILTIN_STASH_DROP_USAGE,
	রিক্ত
};

স্থির ধ্রুৱ ৱর্ণ * ধ্রুৱ git_stash_বেৰ_কৰো_usage[] = {
	BUILTIN_STASH_বেৰ_কৰো_USAGE,
	রিক্ত
};

স্থির ধ্রুৱ ৱর্ণ * ধ্রুৱ git_stash_apply_usage[] = {
	BUILTIN_STASH_APPLY_USAGE,
	রিক্ত
};

স্থির ধ্রুৱ ৱর্ণ * ধ্রুৱ git_stash_branch_usage[] = {
	BUILTIN_STASH_BRANCH_USAGE,
	রিক্ত
};

স্থির ধ্রুৱ ৱর্ণ * ধ্রুৱ git_stash_clear_usage[] = {
	BUILTIN_STASH_CLEAR_USAGE,
	রিক্ত
};

স্থির ধ্রুৱ ৱর্ণ * ধ্রুৱ git_stash_store_usage[] = {
	BUILTIN_STASH_STORE_USAGE,
	রিক্ত
};

স্থির ধ্রুৱ ৱর্ণ * ধ্রুৱ git_stash_ঠেলো_usage[] = {
	BUILTIN_STASH_ঠেলো_USAGE,
	রিক্ত
};

স্থির ধ্রুৱ ৱর্ণ * ধ্রুৱ git_stash_save_usage[] = {
	BUILTIN_STASH_SAVE_USAGE,
	রিক্ত
};

স্থির ধ্রুৱ ৱর্ণ * ধ্রুৱ git_stash_export_usage[] = {
	BUILTIN_STASH_EXPORT_USAGE,
	রিক্ত
};

স্থির ধ্রুৱ ৱর্ণ * ধ্রুৱ git_stash_import_usage[] = {
	BUILTIN_STASH_IMPORT_USAGE,
	রিক্ত
};

স্থির ধ্রুৱ ৱর্ণ ref_stash[] = "refs/stash";
স্থির সংরচনা strbuf stash_index_path = STRBUF_INIT;

স্থির পূর্ণাঙ্ক show_stat = 1;
স্থির পূর্ণাঙ্ক show_patch;
স্থির পূর্ণাঙ্ক show_include_untracked;
স্থির পূর্ণাঙ্ক use_index;

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
সংরচনা stash_info {
	সংরচনা object_id w_commit;
	সংরচনা object_id b_commit;
	সংরচনা object_id i_commit;
	সংরচনা object_id u_commit;
	সংরচনা object_id c_commit;
	সংরচনা object_id w_বৃক্ষ;
	সংরচনা object_id b_বৃক্ষ;
	সংরচনা object_id i_বৃক্ষ;
	সংরচনা object_id u_বৃক্ষ;
	সংরচনা strbuf revision;
	পূর্ণাঙ্ক is_stash_ref;
	পূর্ণাঙ্ক has_u;
};

#define STASH_INFO_INIT { \
	.revision = STRBUF_INIT, \
}

স্থির শূন্য free_stash_info(সংরচনা stash_info *info)
{
	strbuf_release(&info->revision);
}

স্থির পূর্ণাঙ্ক check_stash_topology(সংরচনা repository *r, সংরচনা commit *stash)
{
	সংরচনা commit *p1, *p2, *p3 = রিক্ত;

	/* stash must have two or three parents */
	যদি (!stash->parents || !stash->parents->next ||
			(stash->parents->next->next && stash->parents->next->next->next))
		ঘূৰাই -1;
	p1 = stash->parents->item;
	p2 = stash->parents->next->item;
	যদি (stash->parents->next->next)
		p3 = stash->parents->next->next->item;
	যদি (repo_parse_commit(r, p1) || repo_parse_commit(r, p2) ||
			(p3 && repo_parse_commit(r, p3)))
		ঘূৰাই -1;
	/* p2 must have a single parent, p3 must have no parents */
	যদি (!p2->parents || p2->parents->next || (p3 && p3->parents))
		ঘূৰাই -1;
	যদি (repo_parse_commit(r, p2->parents->item))
		ঘূৰাই -1;
	/* p2^1 must equal p1 */
	যদি (!oideq(&p1->object.oid, &p2->parents->item->object.oid))
		ঘূৰাই -1;

	ঘূৰাই 0;
}

স্থির শূন্য assert_stash_like(সংরচনা stash_info *info, ধ্রুৱ ৱর্ণ *revision)
{
	যদি (get_oidf(&info->b_commit, "%s^1", revision) ||
	    get_oidf(&info->w_বৃক্ষ, "%s:", revision) ||
	    get_oidf(&info->b_বৃক্ষ, "%s^1:", revision) ||
	    get_oidf(&info->i_বৃক্ষ, "%s^2:", revision))
		die(_("'%s' is not a stash-like commit"), revision);
}

স্থির পূর্ণাঙ্ক parse_stash_revision(সংরচনা strbuf *revision, ধ্রুৱ ৱর্ণ *commit, পূর্ণাঙ্ক quiet)
{
	strbuf_reset(revision);
	যদি (!commit) {
		যদি (!refs_ref_exists(get_main_ref_store(the_repository), ref_stash)) {
			যদি (!quiet)
				fprintf_ln(stderr, _("No stash entries found."));
			ঘূৰাই -1;
		}

		strbuf_addf(revision, "%s@{0}", ref_stash);
	} অন্যথা যদি (strspn(commit, "0123456789") == strlen(commit)) {
		strbuf_addf(revision, "%s@{%s}", ref_stash, commit);
	} অন্যথা {
		strbuf_addstr(revision, commit);
	}
	ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক get_stash_info(সংরচনা stash_info *info, পূর্ণাঙ্ক argc, ধ্রুৱ ৱর্ণ **argv)
{
	পূর্ণাঙ্ক ret;
	ৱর্ণ *end_of_rev;
	ৱর্ণ *expanded_ref;
	ধ্রুৱ ৱর্ণ *revision;
	ধ্রুৱ ৱর্ণ *commit = রিক্ত;
	সংরচনা object_id dummy;
	সংরচনা strbuf symbolic = STRBUF_INIT;

	যদি (argc > 1) {
		পূর্ণাঙ্ক i;
		সংরচনা strbuf refs_বাৰ্তা = STRBUF_INIT;

		বাবে (i = 0; i < argc; i++)
			strbuf_addf(&refs_বাৰ্তা, " '%s'", argv[i]);

		fprintf_ln(stderr, _("Too many revisions specified:%s"),
			   refs_বাৰ্তা.buf);
		strbuf_release(&refs_বাৰ্তা);

		ঘূৰাই -1;
	}

	যদি (argc == 1)
		commit = argv[0];

	strbuf_init(&info->revision, 0);
	যদি (parse_stash_revision(&info->revision, commit, 0)) {
		ঘূৰাই -1;
	}

	revision = info->revision.buf;

	যদি (repo_get_oid(the_repository, revision, &info->w_commit))
		ঘূৰাই error(_("%s is not a valid reference"), revision);

	assert_stash_like(info, revision);

	info->has_u = !get_oidf(&info->u_বৃক্ষ, "%s^3:", revision);

	end_of_rev = strchrnul(revision, '@');
	strbuf_add(&symbolic, revision, end_of_rev - revision);

	ret = repo_dwim_ref(the_repository, symbolic.buf, symbolic.len,
			    &dummy, &expanded_ref, 0);
	strbuf_release(&symbolic);
	ৱিকল্প (ret) {
	পক্ষ 0: /* Not found, but valid ref */
		info->is_stash_ref = 0;
		ভঙ্গ;
	পক্ষ 1:
		info->is_stash_ref = !strcmp(expanded_ref, ref_stash);
		ভঙ্গ;
	স্ৱভাৱ: /* Invalid or ambiguous */
		ভঙ্গ;
	}

	মুক্ত(expanded_ref);
	ঘূৰাই !(ret == 0 || ret == 1);
}

স্থির পূর্ণাঙ্ক do_clear_stash(শূন্য)
{
	সংরচনা object_id obj;
	যদি (repo_get_oid(the_repository, ref_stash, &obj))
		ঘূৰাই 0;

	ঘূৰাই refs_delete_ref(get_main_ref_store(the_repository), রিক্ত,
			       ref_stash, &obj, 0);
}

স্থির পূর্ণাঙ্ক clear_stash(পূর্ণাঙ্ক argc, ধ্রুৱ ৱর্ণ **argv, ধ্রুৱ ৱর্ণ *prefix,
		       সংরচনা repository *repo UNUSED)
{
	সংরচনা option options[] = {
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_clear_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	যদি (argc)
		ঘূৰাই error(_("git stash clear with arguments is "
			       "unimplemented"));

	ঘূৰাই do_clear_stash();
}

স্থির পূর্ণাঙ্ক reset_বৃক্ষ(সংরচনা object_id *i_বৃক্ষ, পূর্ণাঙ্ক update, পূর্ণাঙ্ক reset)
{
	পূর্ণাঙ্ক nr_trees = 1;
	সংরচনা unpack_trees_options opts;
	সংরচনা বৃক্ষ_desc t[MAX_UNPACK_TREES];
	সংরচনা বৃক্ষ *বৃক্ষ;
	সংরচনা lock_file lock_file = LOCK_INIT;

	repo_read_index_preload(the_repository, রিক্ত, 0);
	যদি (refresh_index(the_repository->index, REFRESH_QUIET, রিক্ত, রিক্ত, রিক্ত))
		ঘূৰাই -1;

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);

	memset(&opts, 0, পরিমাণ(opts));

	বৃক্ষ = repo_parse_বৃক্ষ_indirect(the_repository, i_বৃক্ষ);
	যদি (repo_parse_বৃক্ষ(the_repository, বৃক্ষ))
		ঘূৰাই -1;

	init_বৃক্ষ_desc(t, &বৃক্ষ->object.oid, বৃক্ষ->buffer, বৃক্ষ->size);

	opts.head_idx = 1;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;
	opts.merge = 1;
	opts.reset = reset ? UNPACK_RESET_PROTECT_UNTRACKED : 0;
	opts.update = update;
	যদি (update)
		opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
	opts.fn = oneway_merge;

	যদি (unpack_trees(nr_trees, t, &opts))
		ঘূৰাই -1;

	যদি (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
		ঘূৰাই error(_("unable to write new index file"));

	ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক create_index_from_বৃক্ষ(ধ্রুৱ সংরচনা object_id *বৃক্ষ_id,
				  ধ্রুৱ ৱর্ণ *index_path)
{
	পূর্ণাঙ্ক nr_trees = 1;
	পূর্ণাঙ্ক ret = 0;
	সংরচনা unpack_trees_options opts;
	সংরচনা বৃক্ষ_desc t[MAX_UNPACK_TREES];
	সংরচনা বৃক্ষ *বৃক্ষ;
	সংরচনা index_state dst_istate = INDEX_STATE_INIT(the_repository);
	সংরচনা lock_file lock_file = LOCK_INIT;

	repo_read_index_preload(the_repository, রিক্ত, 0);
	refresh_index(the_repository->index, REFRESH_QUIET, রিক্ত, রিক্ত, রিক্ত);

	hold_lock_file_for_update(&lock_file, index_path, LOCK_DIE_ON_ERROR);

	memset(&opts, 0, পরিমাণ(opts));

	বৃক্ষ = repo_parse_বৃক্ষ_indirect(the_repository, বৃক্ষ_id);
	যদি (!বৃক্ষ || repo_parse_বৃক্ষ(the_repository, বৃক্ষ)) {
		ret = -1;
		গচ্ছ done;
	}

	init_বৃক্ষ_desc(t, &বৃক্ষ->object.oid, বৃক্ষ->buffer, বৃক্ষ->size);

	opts.head_idx = 1;
	opts.src_index = the_repository->index;
	opts.dst_index = &dst_istate;
	opts.merge = 1;
	opts.reset = UNPACK_RESET_PROTECT_UNTRACKED;
	opts.fn = oneway_merge;

	যদি (unpack_trees(nr_trees, t, &opts)) {
		ret = -1;
		গচ্ছ done;
	}

	যদি (write_locked_index(&dst_istate, &lock_file, COMMIT_LOCK)) {
		ret = error(_("unable to write new index file"));
		গচ্ছ done;
	}

done:
	release_index(&dst_istate);
	যদি (ret)
		rollback_lock_file(&lock_file);
	ঘূৰাই ret;
}

স্থির পূর্ণাঙ্ক is_path_a_directory(ধ্রুৱ ৱর্ণ *path)
{
	/*
	 * This function differs from abspath.c:is_directory() in that
	 * here we use lstat() instead of stat(); we do not want to
	 * follow symbolic links here.
	 */
	সংরচনা stat st;
	ঘূৰাই (!lstat(path, &st) && S_ISDIR(st.st_mode));
}

স্থির শূন্য add_diff_to_buf(সংরচনা diff_queue_struct *q,
			    সংরচনা diff_options *options UNUSED,
			    শূন্য *data)
{
	পূর্ণাঙ্ক i;

	বাবে (i = 0; i < q->nr; i++) {
		যদি (is_path_a_directory(q->queue[i]->one->path))
			অনুৱর্ত;

		strbuf_addstr(data, q->queue[i]->one->path);

		/* NUL-terminate: will be fed to update-index -z */
		strbuf_addch(data, '\0');
	}
}

স্থির পূর্ণাঙ্ক restore_untracked(সংরচনা object_id *u_বৃক্ষ)
{
	পূর্ণাঙ্ক res;
	সংরচনা child_process cp = CHILD_PROCESS_INIT;

	/*
	 * We need to run restore files from a given index, but without
	 * affecting the current index, so we use GIT_INDEX_FILE with
	 * run_command to fork processes that will not interfere.
	 */
	cp.git_cmd = 1;
	strvec_ঠেলো(&cp.args, "read-tree");
	strvec_ঠেলো(&cp.args, oid_to_hex(u_বৃক্ষ));
	strvec_pushf(&cp.env, "GIT_INDEX_FILE=%s",
		     stash_index_path.buf);
	যদি (run_command(&cp)) {
		remove_path(stash_index_path.buf);
		ঘূৰাই -1;
	}

	child_process_init(&cp);
	cp.git_cmd = 1;
	strvec_pushl(&cp.args, "checkout-index", "--all", রিক্ত);
	strvec_pushf(&cp.env, "GIT_INDEX_FILE=%s",
		     stash_index_path.buf);

	res = run_command(&cp);
	remove_path(stash_index_path.buf);
	ঘূৰাই res;
}

স্থির শূন্য unstage_changes_unless_new(সংরচনা object_id *orig_বৃক্ষ)
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

	সংরচনা checkout state = CHECKOUT_INIT;
	সংরচনা diff_options diff_opts;
	সংরচনা lock_file lock = LOCK_INIT;
	পূর্ণাঙ্ক i;

	/* If any entries have skip_worktree set, we'll have to check 'em out */
	state.force = 1;
	state.quiet = 1;
	state.refresh_cache = 1;
	state.istate = the_repository->index;

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
	বাবে (i = 0; i < diff_queued_diff.nr; i++) {
		সংরচনা diff_filepair *p;
		সংরচনা cache_entry *ce;
		পূর্ণাঙ্ক pos;

		/* Look up the path's position in the current index. */
		p = diff_queued_diff.queue[i];
		pos = index_name_pos(the_repository->index, p->two->path,
				     strlen(p->two->path));

		/*
		 * Step 2: Place changes in the working tree
		 *
		 * Stash is about restoring changes *to the working tree*.
		 * So if the merge successfully got a new version of some
		 * path, but left it out of the working tree, then clear the
		 * SKIP_WORKTREE bit and write it to the working tree.
		 */
		যদি (pos >= 0 && ce_skip_worktree(the_repository->index->cache[pos])) {
			সংরচনা stat st;

			ce = the_repository->index->cache[pos];
			যদি (!lstat(ce->name, &st)) {
				/* Conflicting path present; relocate it */
				সংরচনা strbuf new_path = STRBUF_INIT;
				পূর্ণাঙ্ক fd;

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
			checkout_entry(ce, &state, রিক্ত, রিক্ত);
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
			পূর্ণাঙ্ক option = ADD_CACHE_OK_TO_REPLACE;
			যদি (pos < 0)
				option = ADD_CACHE_OK_TO_ADD;

			ce = make_cache_entry(the_repository->index,
					      p->one->mode,
					      &p->one->oid,
					      p->one->path,
					      0, 0);
			add_index_entry(the_repository->index, ce, option);
		}
	}
	diff_flush(&diff_opts);

	/*
	 * Step 4: write the new index to disk
	 */
	repo_hold_locked_index(the_repository, &lock, LOCK_DIE_ON_ERROR);
	যদি (write_locked_index(the_repository->index, &lock,
			       COMMIT_LOCK | SKIP_IF_UNCHANGED))
		die(_("could not write index"));
}

স্থির গণনা stash_apply_result do_apply_stash(ধ্রুৱ ৱর্ণ *prefix,
					      সংরচনা stash_info *info,
					      পূর্ণাঙ্ক index, পূর্ণাঙ্ক quiet,
					      ধ্রুৱ ৱর্ণ *label_ours,
					      ধ্রুৱ ৱর্ণ *label_theirs,
					      ধ্রুৱ ৱর্ণ *label_base)
{
	পূর্ণাঙ্ক clean, ret;
	পূর্ণাঙ্ক has_index = index;
	সংরচনা merge_options o;
	সংরচনা object_id c_বৃক্ষ;
	সংরচনা object_id index_বৃক্ষ;
	সংরচনা বৃক্ষ *head, *merge, *merge_base;
	সংরচনা lock_file lock = LOCK_INIT;

	repo_read_index_preload(the_repository, রিক্ত, 0);
	যদি (repo_refresh_and_write_index(the_repository, REFRESH_QUIET, 0, 0,
					 রিক্ত, রিক্ত, রিক্ত))
		ঘূৰাই error(_("could not write index"));

	যদি (write_index_as_বৃক্ষ(&c_বৃক্ষ, the_repository->index,
				repo_get_index_file(the_repository), 0, রিক্ত))
		ঘূৰাই error(_("cannot apply a stash in the middle of a merge"));

	init_ui_merge_options(&o, the_repository);

	যদি (quiet)
		o.verbosity = 0;

	যদি (index) {
		যদি (oideq(&info->b_বৃক্ষ, &info->i_বৃক্ষ) ||
		    oideq(&c_বৃক্ষ, &info->i_বৃক্ষ)) {
			has_index = 0;
		} অন্যথা {
			সংরচনা merge_result result = { 0 };

			o.branch1 = "Current index";
			o.branch2 = "Stashed index changes";
			o.ancestor = "Stash base";

			head = lookup_বৃক্ষ(o.repo, &c_বৃক্ষ);
			merge = lookup_বৃক্ষ(o.repo, &info->i_বৃক্ষ);
			merge_base = lookup_বৃক্ষ(o.repo, &info->b_বৃক্ষ);

			merge_incore_nonrecursive(&o, merge_base, head, merge,
						  &result);

			যদি (result.clean < 0) {
				merge_finalize(&o, &result);
				ঘূৰাই error(_("index merge failed"));
			} অন্যথা যদি (!result.clean) {
				merge_finalize(&o, &result);
				ঘূৰাই error(_("conflicts in index. "
					       "Try without --index."));
			} অন্যথা {
				oidcpy(&index_বৃক্ষ, &result.বৃক্ষ->object.oid);
				merge_finalize(&o, &result);
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

	head = lookup_বৃক্ষ(o.repo, &c_বৃক্ষ);
	merge = lookup_বৃক্ষ(o.repo, &info->w_বৃক্ষ);
	merge_base = lookup_বৃক্ষ(o.repo, &info->b_বৃক্ষ);

	repo_hold_locked_index(o.repo, &lock, LOCK_DIE_ON_ERROR);
	clean = merge_ort_nonrecursive(&o, head, merge, merge_base);

	/*
	 * If 'clean' >= 0, reverse the value for 'ret' so 'ret' is 0 when the
	 * merge was clean, and 1 if the merge was unclean or a negative value
	 * if it encountered an error.
	 */
	ret = clean >= 0 ? !clean : clean;

	যদি (ret < 0)
		rollback_lock_file(&lock);
	অন্যথা যদি (write_locked_index(o.repo->index, &lock,
				      COMMIT_LOCK | SKIP_IF_UNCHANGED))
		ret = error(_("could not write index"));

	যদি (ret) {
		repo_rerere(the_repository, 0);

		যদি (index)
			fprintf_ln(stderr, _("Index was not unstashed."));

		গচ্ছ restore_untracked;
	}

	যদি (has_index) {
		যদি (reset_বৃক্ষ(&index_বৃক্ষ, 0, 0))
			ret = -1;
	} অন্যথা {
		unstage_changes_unless_new(&c_বৃক্ষ);
	}

restore_untracked:
	যদি (info->has_u && restore_untracked(&info->u_বৃক্ষ))
		ret = error(_("could not restore untracked files from stash"));

	যদি (!quiet) {
		সংরচনা child_process cp = CHILD_PROCESS_INIT;

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

	ঘূৰাই ret;
}

স্থির পূর্ণাঙ্ক apply_stash(পূর্ণাঙ্ক argc, ধ্রুৱ ৱর্ণ **argv, ধ্রুৱ ৱর্ণ *prefix,
		       সংরচনা repository *repo UNUSED)
{
	পূর্ণাঙ্ক ret = -1;
	পূর্ণাঙ্ক quiet = 0;
	পূর্ণাঙ্ক index = use_index;
	ধ্রুৱ ৱর্ণ *label_ours = রিক্ত, *label_theirs = রিক্ত, *label_base = রিক্ত;
	সংরচনা stash_info info = STASH_INFO_INIT;
	সংরচনা option options[] = {
		OPT__QUIET(&quiet, N_("be quiet, only report errors")),
		OPT_BOOL(0, "index", &index,
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
		গচ্ছ cleanup;

	ret = do_apply_stash(prefix, &info, index, quiet,
			     label_ours, label_theirs, label_base);
cleanup:
	free_stash_info(&info);
	ঘূৰাই ret;
}

স্থির পূর্ণাঙ্ক reject_reflog_ent(ধ্রুৱ ৱর্ণ *refname UNUSED,
			     সংরচনা object_id *ooid UNUSED,
			     সংরচনা object_id *noid UNUSED,
			     ধ্রুৱ ৱর্ণ *email UNUSED,
			     timestamp_t timestamp UNUSED,
			     পূর্ণাঙ্ক tz UNUSED, ধ্রুৱ ৱর্ণ *message UNUSED,
			     শূন্য *cb_data UNUSED)
{
	ঘূৰাই 1;
}

স্থির পূর্ণাঙ্ক reflog_is_empty(ধ্রুৱ ৱর্ণ *refname)
{
	ঘূৰাই !refs_for_each_reflog_ent(get_main_ref_store(the_repository),
					 refname, reject_reflog_ent, রিক্ত);
}

স্থির পূর্ণাঙ্ক do_drop_stash(সংরচনা stash_info *info, পূর্ণাঙ্ক quiet)
{
	যদি (!reflog_delete(info->revision.buf,
			   EXPIRE_REFLOGS_REWRITE | EXPIRE_REFLOGS_UPDATE_REF,
			   0)) {
		যদি (!quiet)
			printf_ln(_("Dropped %s (%s)"), info->revision.buf,
				  oid_to_hex(&info->w_commit));
	} অন্যথা {
		ঘূৰাই error(_("%s: Could not drop stash entry"),
			     info->revision.buf);
	}

	যদি (reflog_is_empty(ref_stash))
		do_clear_stash();

	ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক get_stash_info_assert(সংরচনা stash_info *info, পূর্ণাঙ্ক argc,
				 ধ্রুৱ ৱর্ণ **argv)
{
	পূর্ণাঙ্ক ret = get_stash_info(info, argc, argv);

	যদি (ret < 0)
		ঘূৰাই ret;

	যদি (!info->is_stash_ref)
		ঘূৰাই error(_("'%s' is not a stash reference"), info->revision.buf);

	ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক drop_stash(পূর্ণাঙ্ক argc, ধ্রুৱ ৱর্ণ **argv, ধ্রুৱ ৱর্ণ *prefix,
		      সংরচনা repository *repo UNUSED)
{
	পূর্ণাঙ্ক ret = -1;
	পূর্ণাঙ্ক quiet = 0;
	সংরচনা stash_info info = STASH_INFO_INIT;
	সংরচনা option options[] = {
		OPT__QUIET(&quiet, N_("be quiet, only report errors")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_drop_usage, 0);

	যদি (get_stash_info_assert(&info, argc, argv))
		গচ্ছ cleanup;

	ret = do_drop_stash(&info, quiet);
cleanup:
	free_stash_info(&info);
	ঘূৰাই ret;
}

স্থির পূর্ণাঙ্ক বেৰ_কৰো_stash(পূর্ণাঙ্ক argc, ধ্রুৱ ৱর্ণ **argv, ধ্রুৱ ৱর্ণ *prefix,
		     সংরচনা repository *repo UNUSED)
{
	পূর্ণাঙ্ক ret = -1;
	পূর্ণাঙ্ক index = use_index;
	পূর্ণাঙ্ক quiet = 0;
	সংরচনা stash_info info = STASH_INFO_INIT;
	সংরচনা option options[] = {
		OPT__QUIET(&quiet, N_("be quiet, only report errors")),
		OPT_BOOL(0, "index", &index,
			 N_("attempt to recreate the index")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_বেৰ_কৰো_usage, 0);

	যদি (get_stash_info_assert(&info, argc, argv))
		গচ্ছ cleanup;

	যদি ((ret = do_apply_stash(prefix, &info, index, quiet,
				  রিক্ত, রিক্ত, রিক্ত)))
		printf_ln(_("The stash entry is kept in case "
			    "you need it again."));
	অন্যথা
		ret = do_drop_stash(&info, quiet);

cleanup:
	free_stash_info(&info);
	ঘূৰাই ret;
}

স্থির পূর্ণাঙ্ক branch_stash(পূর্ণাঙ্ক argc, ধ্রুৱ ৱর্ণ **argv, ধ্রুৱ ৱর্ণ *prefix,
			সংরচনা repository *repo UNUSED)
{
	পূর্ণাঙ্ক ret = -1;
	ধ্রুৱ ৱর্ণ *branch = রিক্ত;
	সংরচনা stash_info info = STASH_INFO_INIT;
	সংরচনা child_process cp = CHILD_PROCESS_INIT;
	সংরচনা option options[] = {
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_branch_usage, 0);

	যদি (!argc) {
		fprintf_ln(stderr, _("No branch name specified"));
		ঘূৰাই -1;
	}

	branch = argv[0];

	যদি (get_stash_info(&info, argc - 1, argv + 1))
		গচ্ছ cleanup;

	cp.git_cmd = 1;
	strvec_pushl(&cp.args, "checkout", "-b", রিক্ত);
	strvec_ঠেলো(&cp.args, branch);
	strvec_ঠেলো(&cp.args, oid_to_hex(&info.b_commit));
	ret = run_command(&cp);
	যদি (!ret)
		ret = do_apply_stash(prefix, &info, 1, 0,
				     রিক্ত, রিক্ত, রিক্ত);
	যদি (!ret && info.is_stash_ref)
		ret = do_drop_stash(&info, 0);

cleanup:
	free_stash_info(&info);
	ঘূৰাই ret;
}

স্থির পূর্ণাঙ্ক list_stash(পূর্ণাঙ্ক argc, ধ্রুৱ ৱর্ণ **argv, ধ্রুৱ ৱর্ণ *prefix,
		      সংরচনা repository *repo UNUSED)
{
	সংরচনা child_process cp = CHILD_PROCESS_INIT;
	সংরচনা option options[] = {
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_list_usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT);

	যদি (!refs_ref_exists(get_main_ref_store(the_repository), ref_stash))
		ঘূৰাই 0;

	cp.git_cmd = 1;
	strvec_pushl(&cp.args, "log", "--format=%gd: %gs", "-g",
		     "--first-parent", রিক্ত);
	strvec_pushv(&cp.args, argv);
	strvec_ঠেলো(&cp.args, ref_stash);
	strvec_ঠেলো(&cp.args, "--");
	ঘূৰাই run_command(&cp);
}

স্থির পূর্ণাঙ্ক git_stash_config(ধ্রুৱ ৱর্ণ *var, ধ্রুৱ ৱর্ণ *মান,
			    ধ্রুৱ সংরচনা config_context *ctx, শূন্য *cb)
{
	যদি (!strcmp(var, "stash.showstat")) {
		show_stat = git_config_bool(var, মান);
		ঘূৰাই 0;
	}
	যদি (!strcmp(var, "stash.showpatch")) {
		show_patch = git_config_bool(var, মান);
		ঘূৰাই 0;
	}
	যদি (!strcmp(var, "stash.showincludeuntracked")) {
		show_include_untracked = git_config_bool(var, মান);
		ঘূৰাই 0;
	}
	যদি (!strcmp(var, "stash.index")) {
		use_index = git_config_bool(var, মান);
		ঘূৰাই 0;
	}
	ঘূৰাই git_diff_basic_config(var, মান, ctx, cb);
}

স্থির শূন্য diff_include_untracked(ধ্রুৱ সংরচনা stash_info *info, সংরচনা diff_options *diff_opt)
{
	ধ্রুৱ সংরচনা object_id *oid[] = { &info->w_commit, &info->u_বৃক্ষ };
	সংরচনা বৃক্ষ *বৃক্ষ[ARRAY_SIZE(oid)];
	সংরচনা বৃক্ষ_desc বৃক্ষ_desc[ARRAY_SIZE(oid)];
	সংরচনা unpack_trees_options unpack_বৃক্ষ_opt = { 0 };

	বাবে (size_t i = 0; i < ARRAY_SIZE(oid); i++) {
		বৃক্ষ[i] = repo_parse_বৃক্ষ_indirect(the_repository, oid[i]);
		যদি (repo_parse_বৃক্ষ(the_repository, বৃক্ষ[i]) < 0)
			die(_("failed to parse tree"));
		init_বৃক্ষ_desc(&বৃক্ষ_desc[i], &বৃক্ষ[i]->object.oid,
			       বৃক্ষ[i]->buffer, বৃক্ষ[i]->size);
	}

	unpack_বৃক্ষ_opt.head_idx = -1;
	unpack_বৃক্ষ_opt.src_index = the_repository->index;
	unpack_বৃক্ষ_opt.dst_index = the_repository->index;
	unpack_বৃক্ষ_opt.merge = 1;
	unpack_বৃক্ষ_opt.fn = stash_worktree_untracked_merge;

	যদি (unpack_trees(ARRAY_SIZE(বৃক্ষ_desc), বৃক্ষ_desc, &unpack_বৃক্ষ_opt))
		die(_("failed to unpack trees"));

	do_diff_cache(&info->b_commit, diff_opt);
}

স্থির পূর্ণাঙ্ক show_stash(পূর্ণাঙ্ক argc, ধ্রুৱ ৱর্ণ **argv, ধ্রুৱ ৱর্ণ *prefix,
		      সংরচনা repository *repo UNUSED)
{
	পূর্ণাঙ্ক i;
	পূর্ণাঙ্ক ret = -1;
	সংরচনা stash_info info = STASH_INFO_INIT;
	সংরচনা rev_info rev;
	সংরচনা strvec stash_args = STRVEC_INIT;
	সংরচনা strvec revision_args = STRVEC_INIT;
	গণনা {
		UNTRACKED_NONE,
		UNTRACKED_INCLUDE,
		UNTRACKED_ONLY
	} show_untracked = show_include_untracked ? UNTRACKED_INCLUDE : UNTRACKED_NONE;
	সংরচনা option options[] = {
		OPT_SET_INT('u', "include-untracked", &show_untracked,
			    N_("include untracked files in the stash"),
			    UNTRACKED_INCLUDE),
		OPT_SET_INT_F(0, "only-untracked", &show_untracked,
			      N_("only show untracked files in the stash"),
			      UNTRACKED_ONLY, PARSE_OPT_NONEG),
		OPT_END()
	};
	পূর্ণাঙ্ক do_usage = 0;

	init_diff_ui_defaults();
	repo_config(the_repository, git_diff_ui_config, রিক্ত);
	repo_init_revisions(the_repository, &rev, prefix);

	argc = parse_options(argc, argv, prefix, options, git_stash_show_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	strvec_ঠেলো(&revision_args, argv[0]);
	বাবে (i = 1; i < argc; i++) {
		যদি (argv[i][0] != '-')
			strvec_ঠেলো(&stash_args, argv[i]);
		অন্যথা
			strvec_ঠেলো(&revision_args, argv[i]);
	}

	যদি (get_stash_info(&info, stash_args.nr, stash_args.v))
		গচ্ছ cleanup;

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
			গচ্ছ cleanup;
		}
	}

	setup_revisions_from_strvec(&revision_args, &rev, রিক্ত);
	যদি (revision_args.nr > 1)
		গচ্ছ usage;
	যদি (!rev.diffopt.output_format) {
		rev.diffopt.output_format = DIFF_FORMAT_PATCH;
		diff_setup_done(&rev.diffopt);
	}

	rev.diffopt.flags.recursive = 1;
	setup_diff_pager(&rev.diffopt);
	ৱিকল্প (show_untracked) {
	পক্ষ UNTRACKED_NONE:
		diff_বৃক্ষ_oid(&info.b_commit, &info.w_commit, "", &rev.diffopt);
		ভঙ্গ;
	পক্ষ UNTRACKED_ONLY:
		যদি (info.has_u)
			diff_মূল_বৃক্ষ_oid(&info.u_বৃক্ষ, "", &rev.diffopt);
		ভঙ্গ;
	পক্ষ UNTRACKED_INCLUDE:
		যদি (info.has_u)
			diff_include_untracked(&info, &rev.diffopt);
		অন্যথা
			diff_বৃক্ষ_oid(&info.b_commit, &info.w_commit, "", &rev.diffopt);
		ভঙ্গ;
	}
	log_বৃক্ষ_diff_flush(&rev);

	ret = diff_result_code(&rev);

cleanup:
	strvec_clear(&revision_args);
	strvec_clear(&stash_args);
	free_stash_info(&info);
	release_revisions(&rev);
	যদি (do_usage)
		usage_with_options(git_stash_show_usage, options);
	ঘূৰাই ret;
usage:
	do_usage = 1;
	গচ্ছ cleanup;
}

স্থির পূর্ণাঙ্ক do_store_stash(ধ্রুৱ সংরচনা object_id *w_commit, ধ্রুৱ ৱর্ণ *stash_বাৰ্তা,
			  পূর্ণাঙ্ক quiet)
{
	সংরচনা stash_info info;
	ৱর্ণ revision[GIT_MAX_HEXSZ];

	oid_to_hex_r(revision, w_commit);
	assert_stash_like(&info, revision);

	যদি (!stash_বাৰ্তা)
		stash_বাৰ্তা = "Created via \"git stash store\".";

	যদি (refs_update_ref(get_main_ref_store(the_repository), stash_বাৰ্তা, ref_stash, w_commit, রিক্ত,
			    REF_FORCE_CREATE_REFLOG,
			    quiet ? UPDATE_REFS_QUIET_ON_ERR :
			    UPDATE_REFS_বাৰ্তা_ON_ERR)) {
		যদি (!quiet) {
			fprintf_ln(stderr, _("Cannot update %s with %s"),
				   ref_stash, oid_to_hex(w_commit));
		}
		ঘূৰাই -1;
	}

	ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক store_stash(পূর্ণাঙ্ক argc, ধ্রুৱ ৱর্ণ **argv, ধ্রুৱ ৱর্ণ *prefix,
		       সংরচনা repository *repo UNUSED)
{
	পূর্ণাঙ্ক quiet = 0;
	ধ্রুৱ ৱর্ণ *stash_বাৰ্তা = রিক্ত;
	সংরচনা object_id obj;
	সংরচনা option options[] = {
		OPT__QUIET(&quiet, N_("be quiet")),
		OPT_STRING('m', "message", &stash_বাৰ্তা, "message",
			   N_("stash message")),
		OPT_END()
	};
	পূর্ণাঙ্ক ret;

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_store_usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT);

	যদি (argc != 1) {
		যদি (!quiet)
			fprintf_ln(stderr, _("\"git stash store\" requires one "
					     "<commit> argument"));
		ঘূৰাই -1;
	}

	যদি (repo_get_oid_with_flags(the_repository, argv[0], &obj,
				    quiet ? GET_OID_QUIETLY : 0)) {
		যদি (!quiet)
			fprintf_ln(stderr, _("Cannot update %s with %s"),
					     ref_stash, argv[0]);
		ret = -1;
		গচ্ছ out;
	}

	ret = do_store_stash(&obj, stash_বাৰ্তা, quiet);

out:
	ঘূৰাই ret;
}

স্থির শূন্য add_pathspecs(সংরচনা strvec *args,
			  ধ্রুৱ সংরচনা pathspec *ps) {
	পূর্ণাঙ্ক i;

	বাবে (i = 0; i < ps->nr; i++)
		strvec_ঠেলো(args, ps->বস্তুসমূহ[i].original);
}

/*
 * `untracked_files` will be filled with the names of untracked files.
 * The return value is:
 *
 * = 0 if there are not any untracked files
 * > 0 if there are untracked files
 */
স্থির পূর্ণাঙ্ক get_untracked_files(ধ্রুৱ সংরচনা pathspec *ps, পূর্ণাঙ্ক include_untracked,
			       সংরচনা strbuf *untracked_files)
{
	পূর্ণাঙ্ক i;
	পূর্ণাঙ্ক found = 0;
	সংরচনা dir_struct dir = DIR_INIT;

	যদি (include_untracked != INCLUDE_ALL_FILES)
		setup_standard_excludes(&dir);

	fill_directory(&dir, the_repository->index, ps);
	বাবে (i = 0; i < dir.nr; i++) {
		সংরচনা dir_entry *ent = dir.entries[i];
		found++;
		strbuf_addstr(untracked_files, ent->name);
		/* NUL-terminate: will be fed to update-index -z */
		strbuf_addch(untracked_files, '\0');
	}

	dir_clear(&dir);
	ঘূৰাই found;
}

/*
 * The return value of `check_changes_tracked_files()` can be:
 *
 * < 0 if there was an error
 * = 0 if there are no changes.
 * > 0 if there are changes.
 */
স্থির পূর্ণাঙ্ক check_changes_tracked_files(ধ্রুৱ সংরচনা pathspec *ps)
{
	সংরচনা rev_info rev;
	সংরচনা object_id dummy;
	পূর্ণাঙ্ক ret = 0;

	/* No initial commit. */
	যদি (repo_get_oid(the_repository, "HEAD", &dummy))
		ঘূৰাই -1;

	যদি (repo_read_index(the_repository) < 0)
		ঘূৰাই -1;

	repo_init_revisions(the_repository, &rev, রিক্ত);
	copy_pathspec(&rev.prune_data, ps);

	rev.diffopt.flags.quick = 1;
	rev.diffopt.flags.ignore_submodules = 1;
	rev.abbrev = 0;

	add_head_to_pending(&rev);
	diff_setup_done(&rev.diffopt);

	run_diff_index(&rev, DIFF_INDEX_CACHED);
	যদি (diff_result_code(&rev)) {
		ret = 1;
		গচ্ছ done;
	}

	run_diff_files(&rev, 0);
	যদি (diff_result_code(&rev)) {
		ret = 1;
		গচ্ছ done;
	}

done:
	release_revisions(&rev);
	ঘূৰাই ret;
}

/*
 * The function will fill `untracked_files` with the names of untracked files
 * It will return 1 if there were any changes and 0 if there were not.
 */
স্থির পূর্ণাঙ্ক check_changes(ধ্রুৱ সংরচনা pathspec *ps, পূর্ণাঙ্ক include_untracked,
			 সংরচনা strbuf *untracked_files)
{
	পূর্ণাঙ্ক ret = 0;
	যদি (check_changes_tracked_files(ps))
		ret = 1;

	যদি (include_untracked && get_untracked_files(ps, include_untracked,
						     untracked_files))
		ret = 1;

	ঘূৰাই ret;
}

স্থির পূর্ণাঙ্ক save_untracked_files(সংরচনা stash_info *info, সংরচনা strbuf *বাৰ্তা,
				সংরচনা strbuf *files)
{
	পূর্ণাঙ্ক ret = 0;
	সংরচনা strbuf untracked_বাৰ্তা = STRBUF_INIT;
	সংরচনা child_process cp_upd_index = CHILD_PROCESS_INIT;
	সংরচনা index_state istate = INDEX_STATE_INIT(the_repository);

	cp_upd_index.git_cmd = 1;
	strvec_pushl(&cp_upd_index.args, "update-index", "-z", "--add",
		     "--remove", "--stdin", রিক্ত);
	strvec_pushf(&cp_upd_index.env, "GIT_INDEX_FILE=%s",
			 stash_index_path.buf);

	strbuf_addf(&untracked_বাৰ্তা, "untracked files on %s\n", বাৰ্তা->buf);
	যদি (pipe_command(&cp_upd_index, files->buf, files->len, রিক্ত, 0,
			 রিক্ত, 0)) {
		ret = -1;
		গচ্ছ done;
	}

	যদি (write_index_as_বৃক্ষ(&info->u_বৃক্ষ, &istate, stash_index_path.buf, 0,
				রিক্ত)) {
		ret = -1;
		গচ্ছ done;
	}

	যদি (commit_বৃক্ষ(untracked_বাৰ্তা.buf, untracked_বাৰ্তা.len,
			&info->u_বৃক্ষ, রিক্ত, &info->u_commit, রিক্ত, রিক্ত)) {
		ret = -1;
		গচ্ছ done;
	}

done:
	release_index(&istate);
	strbuf_release(&untracked_বাৰ্তা);
	remove_path(stash_index_path.buf);
	ঘূৰাই ret;
}

স্থির পূর্ণাঙ্ক stash_staged(সংরচনা stash_info *info, সংরচনা strbuf *out_patch,
			পূর্ণাঙ্ক quiet)
{
	পূর্ণাঙ্ক ret = 0;
	সংরচনা child_process cp_diff_বৃক্ষ = CHILD_PROCESS_INIT;
	সংরচনা index_state istate = INDEX_STATE_INIT(the_repository);

	যদি (write_index_as_বৃক্ষ(&info->w_বৃক্ষ, &istate, the_repository->index_file,
				0, রিক্ত)) {
		ret = -1;
		গচ্ছ done;
	}

	cp_diff_বৃক্ষ.git_cmd = 1;
	strvec_pushl(&cp_diff_বৃক্ষ.args, "diff-tree", "-p", "--binary",
		     "--no-color",
		     "-U1", "HEAD", oid_to_hex(&info->w_বৃক্ষ), "--", রিক্ত);
	যদি (pipe_command(&cp_diff_বৃক্ষ, রিক্ত, 0, out_patch, 0, রিক্ত, 0)) {
		ret = -1;
		গচ্ছ done;
	}

	যদি (!out_patch->len) {
		যদি (!quiet)
			fprintf_ln(stderr, _("No staged changes"));
		ret = 1;
	}

done:
	release_index(&istate);
	ঘূৰাই ret;
}

স্থির পূর্ণাঙ্ক stash_patch(সংরচনা stash_info *info, ধ্রুৱ সংরচনা pathspec *ps,
		       সংরচনা strbuf *out_patch, পূর্ণাঙ্ক quiet,
		       সংরচনা interactive_options *interactive_opts)
{
	পূর্ণাঙ্ক ret = 0;
	সংরচনা child_process cp_diff_বৃক্ষ = CHILD_PROCESS_INIT;
	সংরচনা commit *head_commit;
	ধ্রুৱ সংরচনা object_id *head_বৃক্ষ;
	সংরচনা index_state istate = INDEX_STATE_INIT(the_repository);
	ৱর্ণ *old_index_env = রিক্ত, *old_repo_index_file;

	remove_path(stash_index_path.buf);

	head_commit = lookup_commit(the_repository, &info->b_commit);
	যদি (!head_commit || repo_parse_commit(the_repository, head_commit)) {
		ret = -1;
		গচ্ছ done;
	}
	head_বৃক্ষ = get_commit_বৃক্ষ_oid(head_commit);
	যদি (!head_বৃক্ষ) {
		ret = -1;
		গচ্ছ done;
	}

	যদি (create_index_from_বৃক্ষ(head_বৃক্ষ, stash_index_path.buf)) {
		ret = -1;
		গচ্ছ done;
	}

	/* Find out what the user wants. */
	old_repo_index_file = the_repository->index_file;
	the_repository->index_file = stash_index_path.buf;
	old_index_env = xstrdup_or_null(getenv(INDEX_ENVIRONMENT));
	setenv(INDEX_ENVIRONMENT, the_repository->index_file, 1);

	ret = !!run_add_p(the_repository, ADD_P_STASH, interactive_opts, রিক্ত, ps, 0);

	the_repository->index_file = old_repo_index_file;
	যদি (old_index_env && *old_index_env)
		setenv(INDEX_ENVIRONMENT, old_index_env, 1);
	অন্যথা
		unsetenv(INDEX_ENVIRONMENT);
	FREE_AND_NULL(old_index_env);

	/* State of the working tree. */
	যদি (write_index_as_বৃক্ষ(&info->w_বৃক্ষ, &istate, stash_index_path.buf, 0,
				রিক্ত)) {
		ret = -1;
		গচ্ছ done;
	}

	cp_diff_বৃক্ষ.git_cmd = 1;
	strvec_pushl(&cp_diff_বৃক্ষ.args, "diff-tree", "-p", "-U1", "HEAD",
		     "--no-color",
		     oid_to_hex(&info->w_বৃক্ষ), "--", রিক্ত);
	যদি (pipe_command(&cp_diff_বৃক্ষ, রিক্ত, 0, out_patch, 0, রিক্ত, 0)) {
		ret = -1;
		গচ্ছ done;
	}

	যদি (!out_patch->len) {
		যদি (!quiet)
			fprintf_ln(stderr, _("No changes selected"));
		ret = 1;
	}

done:
	release_index(&istate);
	remove_path(stash_index_path.buf);
	ঘূৰাই ret;
}

স্থির পূর্ণাঙ্ক stash_working_বৃক্ষ(সংরচনা stash_info *info, ধ্রুৱ সংরচনা pathspec *ps)
{
	পূর্ণাঙ্ক ret = 0;
	সংরচনা rev_info rev;
	সংরচনা child_process cp_upd_index = CHILD_PROCESS_INIT;
	সংরচনা strbuf diff_output = STRBUF_INIT;
	সংরচনা index_state istate = INDEX_STATE_INIT(the_repository);

	repo_init_revisions(the_repository, &rev, রিক্ত);
	copy_pathspec(&rev.prune_data, ps);

	set_alternate_index_output(stash_index_path.buf);
	যদি (reset_বৃক্ষ(&info->i_বৃক্ষ, 0, 0)) {
		ret = -1;
		গচ্ছ done;
	}
	set_alternate_index_output(রিক্ত);

	rev.diffopt.output_format = DIFF_FORMAT_CALLBACK;
	rev.diffopt.format_callback = add_diff_to_buf;
	rev.diffopt.format_callback_data = &diff_output;

	যদি (repo_read_index_preload(the_repository, &rev.diffopt.pathspec, 0) < 0) {
		ret = -1;
		গচ্ছ done;
	}

	add_pending_object(&rev, parse_object(the_repository, &info->b_commit),
			   "");
	run_diff_index(&rev, 0);

	cp_upd_index.git_cmd = 1;
	strvec_pushl(&cp_upd_index.args, "update-index",
		     "--ignore-skip-worktree-entries",
		     "-z", "--add", "--remove", "--stdin", রিক্ত);
	strvec_pushf(&cp_upd_index.env, "GIT_INDEX_FILE=%s",
		     stash_index_path.buf);

	যদি (pipe_command(&cp_upd_index, diff_output.buf, diff_output.len,
			 রিক্ত, 0, রিক্ত, 0)) {
		ret = -1;
		গচ্ছ done;
	}

	যদি (write_index_as_বৃক্ষ(&info->w_বৃক্ষ, &istate, stash_index_path.buf, 0,
				রিক্ত)) {
		ret = -1;
		গচ্ছ done;
	}

done:
	release_index(&istate);
	release_revisions(&rev);
	strbuf_release(&diff_output);
	remove_path(stash_index_path.buf);
	ঘূৰাই ret;
}

স্থির পূর্ণাঙ্ক do_create_stash(ধ্রুৱ সংরচনা pathspec *ps, সংরচনা strbuf *stash_বাৰ্তা_buf,
			   পূর্ণাঙ্ক include_untracked, পূর্ণাঙ্ক patch_mode,
			   সংরচনা interactive_options *interactive_opts,
			   পূর্ণাঙ্ক only_staged, সংরচনা stash_info *info, সংরচনা strbuf *patch,
			   পূর্ণাঙ্ক quiet)
{
	পূর্ণাঙ্ক ret = 0;
	পূর্ণাঙ্ক flags = 0;
	পূর্ণাঙ্ক untracked_commit_option = 0;
	ধ্রুৱ ৱর্ণ *head_short_sha1 = রিক্ত;
	ধ্রুৱ ৱর্ণ *branch_ref = রিক্ত;
	ধ্রুৱ ৱর্ণ *branch_name = "(no branch)";
	ৱর্ণ *branch_name_buf = রিক্ত;
	সংরচনা commit *head_commit = রিক্ত;
	সংরচনা commit_list *parents = রিক্ত;
	সংরচনা strbuf বাৰ্তা = STRBUF_INIT;
	সংরচনা strbuf commit_বৃক্ষ_label = STRBUF_INIT;
	সংরচনা strbuf untracked_files = STRBUF_INIT;

	prepare_fallback_ident("git stash", "git@stash");

	repo_read_index_preload(the_repository, রিক্ত, 0);
	যদি (repo_refresh_and_write_index(the_repository, REFRESH_QUIET, 0, 0,
					 রিক্ত, রিক্ত, রিক্ত) < 0) {
		ret = error(_("could not write index"));
		গচ্ছ done;
	}

	যদি (repo_get_oid(the_repository, "HEAD", &info->b_commit)) {
		যদি (!quiet)
			fprintf_ln(stderr, _("You do not have "
					     "the initial commit yet"));
		ret = -1;
		গচ্ছ done;
	} অন্যথা {
		head_commit = lookup_commit(the_repository, &info->b_commit);
	}

	যদি (!check_changes(ps, include_untracked, &untracked_files)) {
		ret = 1;
		গচ্ছ done;
	}

	branch_ref = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
					     "HEAD", 0, রিক্ত, &flags);

	যদি (flags & REF_ISSYMREF) {
		যদি (skip_prefix(branch_ref, "refs/heads/", &branch_name))
			branch_name = branch_name_buf = xstrdup(branch_name);
	}

	head_short_sha1 = repo_find_unique_abbrev(the_repository,
						  &head_commit->object.oid,
						  DEFAULT_ABBREV);
	strbuf_addf(&বাৰ্তা, "%s: %s ", branch_name, head_short_sha1);
	pp_commit_easy(CMIT_FMT_ONELINE, head_commit, &বাৰ্তা);

	strbuf_addf(&commit_বৃক্ষ_label, "index on %s\n", বাৰ্তা.buf);
	commit_list_ঢোকাও(head_commit, &parents);
	যদি (write_index_as_বৃক্ষ(&info->i_বৃক্ষ, the_repository->index,
				repo_get_index_file(the_repository), 0, রিক্ত) ||
	    commit_বৃক্ষ(commit_বৃক্ষ_label.buf, commit_বৃক্ষ_label.len,
			&info->i_বৃক্ষ, parents, &info->i_commit, রিক্ত, রিক্ত)) {
		যদি (!quiet)
			fprintf_ln(stderr, _("Cannot save the current "
					     "index state"));
		ret = -1;
		গচ্ছ done;
	}

	commit_list_free(parents);
	parents = রিক্ত;

	যদি (include_untracked) {
		যদি (save_untracked_files(info, &বাৰ্তা, &untracked_files)) {
			যদি (!quiet)
				fprintf_ln(stderr, _("Cannot save "
						     "the untracked files"));
			ret = -1;
			গচ্ছ done;
		}
		untracked_commit_option = 1;
	}
	যদি (patch_mode) {
		ret = stash_patch(info, ps, patch, quiet, interactive_opts);
		যদি (ret < 0) {
			যদি (!quiet)
				fprintf_ln(stderr, _("Cannot save the current "
						     "worktree state"));
			গচ্ছ done;
		} অন্যথা যদি (ret > 0) {
			গচ্ছ done;
		}
	} অন্যথা যদি (only_staged) {
		ret = stash_staged(info, patch, quiet);
		যদি (ret < 0) {
			যদি (!quiet)
				fprintf_ln(stderr, _("Cannot save the current "
						     "staged state"));
			গচ্ছ done;
		} অন্যথা যদি (ret > 0) {
			গচ্ছ done;
		}
	} অন্যথা {
		যদি (stash_working_বৃক্ষ(info, ps)) {
			যদি (!quiet)
				fprintf_ln(stderr, _("Cannot save the current "
						     "worktree state"));
			ret = -1;
			গচ্ছ done;
		}
	}

	যদি (!stash_বাৰ্তা_buf->len)
		strbuf_addf(stash_বাৰ্তা_buf, "WIP on %s", বাৰ্তা.buf);
	অন্যথা
		strbuf_insertf(stash_বাৰ্তা_buf, 0, "On %s: ", branch_name);

	যদি (untracked_commit_option)
		commit_list_ঢোকাও(lookup_commit(the_repository,
						 &info->u_commit),
				   &parents);
	commit_list_ঢোকাও(lookup_commit(the_repository, &info->i_commit),
			   &parents);
	commit_list_ঢোকাও(head_commit, &parents);

	যদি (commit_বৃক্ষ(stash_বাৰ্তা_buf->buf, stash_বাৰ্তা_buf->len, &info->w_বৃক্ষ,
			parents, &info->w_commit, রিক্ত, রিক্ত)) {
		যদি (!quiet)
			fprintf_ln(stderr, _("Cannot record "
					     "working tree state"));
		ret = -1;
		গচ্ছ done;
	}

done:
	strbuf_release(&commit_বৃক্ষ_label);
	strbuf_release(&বাৰ্তা);
	strbuf_release(&untracked_files);
	commit_list_free(parents);
	মুক্ত(branch_name_buf);
	ঘূৰাই ret;
}

স্থির পূর্ণাঙ্ক create_stash(পূর্ণাঙ্ক argc, ধ্রুৱ ৱর্ণ **argv, ধ্রুৱ ৱর্ণ *prefix UNUSED,
			সংরচনা repository *repo UNUSED)
{
	পূর্ণাঙ্ক ret;
	সংরচনা strbuf stash_বাৰ্তা_buf = STRBUF_INIT;
	সংরচনা stash_info info = STASH_INFO_INIT;
	সংরচনা pathspec ps;

	/* Starting with argv[1], since argv[0] is "create" */
	strbuf_join_argv(&stash_বাৰ্তা_buf, argc - 1, ++argv, ' ');

	memset(&ps, 0, পরিমাণ(ps));
	যদি (!check_changes_tracked_files(&ps))
		ঘূৰাই 0;

	ret = do_create_stash(&ps, &stash_বাৰ্তা_buf, 0, 0, রিক্ত, 0, &info,
			      রিক্ত, 0);
	যদি (!ret)
		printf_ln("%s", oid_to_hex(&info.w_commit));

	free_stash_info(&info);
	strbuf_release(&stash_বাৰ্তা_buf);
	ঘূৰাই ret;
}

স্থির পূর্ণাঙ্ক do_ঠেলো_stash(ধ্রুৱ সংরচনা pathspec *ps, ধ্রুৱ ৱর্ণ *stash_বাৰ্তা, পূর্ণাঙ্ক quiet,
			 পূর্ণাঙ্ক keep_index, পূর্ণাঙ্ক patch_mode,
			 সংরচনা interactive_options *interactive_opts,
			 পূর্ণাঙ্ক include_untracked, পূর্ণাঙ্ক only_staged)
{
	পূর্ণাঙ্ক ret = 0;
	সংরচনা stash_info info = STASH_INFO_INIT;
	সংরচনা strbuf patch = STRBUF_INIT;
	সংরচনা strbuf stash_বাৰ্তা_buf = STRBUF_INIT;
	সংরচনা strbuf untracked_files = STRBUF_INIT;
	সংরচনা strbuf out = STRBUF_INIT;

	যদি (patch_mode && keep_index == -1)
		keep_index = 1;

	যদি (patch_mode && include_untracked) {
		fprintf_ln(stderr, _("Can't use --patch and --include-untracked"
				     " or --all at the same time"));
		ret = -1;
		গচ্ছ done;
	}

	/* --patch overrides --staged */
	যদি (patch_mode)
		only_staged = 0;

	যদি (only_staged && include_untracked) {
		fprintf_ln(stderr, _("Can't use --staged and --include-untracked"
				     " or --all at the same time"));
		ret = -1;
		গচ্ছ done;
	}

	repo_read_index_preload(the_repository, রিক্ত, 0);
	যদি (!include_untracked && ps->nr) {
		ৱর্ণ *ps_matched = xcalloc(ps->nr, 1);

		যদি (pathspec_needs_expanded_index(the_repository->index, ps))
			ensure_full_index(the_repository->index);
		বাবে (size_t i = 0; i < the_repository->index->cache_nr; i++)
			ce_path_match(the_repository->index, the_repository->index->cache[i], ps,
				      ps_matched);

		যদি (report_path_error(ps_matched, ps)) {
			fprintf_ln(stderr, _("Did you forget to 'git add'?"));
			ret = -1;
			মুক্ত(ps_matched);
			গচ্ছ done;
		}
		মুক্ত(ps_matched);
	}

	যদি (repo_refresh_and_write_index(the_repository, REFRESH_QUIET, 0, 0,
					 রিক্ত, রিক্ত, রিক্ত)) {
		ret = error(_("could not write index"));
		গচ্ছ done;
	}

	যদি (!check_changes(ps, include_untracked, &untracked_files)) {
		যদি (!quiet)
			printf_ln(_("No local changes to save"));
		গচ্ছ done;
	}

	যদি (!refs_reflog_exists(get_main_ref_store(the_repository), ref_stash) && do_clear_stash()) {
		ret = -1;
		যদি (!quiet)
			fprintf_ln(stderr, _("Cannot initialize stash"));
		গচ্ছ done;
	}

	যদি (stash_বাৰ্তা)
		strbuf_addstr(&stash_বাৰ্তা_buf, stash_বাৰ্তা);
	যদি (do_create_stash(ps, &stash_বাৰ্তা_buf, include_untracked, patch_mode,
			    interactive_opts, only_staged, &info, &patch, quiet)) {
		ret = -1;
		গচ্ছ done;
	}

	যদি (do_store_stash(&info.w_commit, stash_বাৰ্তা_buf.buf, 1)) {
		ret = -1;
		যদি (!quiet)
			fprintf_ln(stderr, _("Cannot save the current status"));
		গচ্ছ done;
	}

	যদি (!quiet)
		printf_ln(_("Saved working directory and index state %s"),
			  stash_বাৰ্তা_buf.buf);

	যদি (!(patch_mode || only_staged)) {
		যদি (include_untracked && !ps->nr) {
			সংরচনা child_process cp = CHILD_PROCESS_INIT;

			cp.git_cmd = 1;
			যদি (startup_info->original_cwd) {
				cp.dir = startup_info->original_cwd;
				strvec_pushf(&cp.env, "%s=%s",
					     GIT_WORK_বৃক্ষ_ENVIRONMENT,
					     the_repository->worktree);
			}
			strvec_pushl(&cp.args, "clean", "--force",
				     "--quiet", "-d", ":/", রিক্ত);
			যদি (include_untracked == INCLUDE_ALL_FILES)
				strvec_ঠেলো(&cp.args, "-x");
			যদি (run_command(&cp)) {
				ret = -1;
				গচ্ছ done;
			}
		}
		discard_index(the_repository->index);
		যদি (ps->nr) {
			সংরচনা child_process cp_add = CHILD_PROCESS_INIT;
			সংরচনা child_process cp_diff = CHILD_PROCESS_INIT;
			সংরচনা child_process cp_apply = CHILD_PROCESS_INIT;

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
				গচ্ছ done;
			}

			cp_diff.git_cmd = 1;
			strvec_pushl(&cp_diff.args, "diff-index", "-p",
				     "--no-color",
				     "--cached", "--binary", "HEAD", "--",
				     রিক্ত);
			add_pathspecs(&cp_diff.args, ps);
			যদি (pipe_command(&cp_diff, রিক্ত, 0, &out, 0, রিক্ত, 0)) {
				ret = -1;
				গচ্ছ done;
			}

			cp_apply.git_cmd = 1;
			strvec_pushl(&cp_apply.args, "apply", "--index",
				     "-R", রিক্ত);
			যদি (pipe_command(&cp_apply, out.buf, out.len, রিক্ত, 0,
					 রিক্ত, 0)) {
				ret = -1;
				গচ্ছ done;
			}
		} অন্যথা {
			সংরচনা child_process cp = CHILD_PROCESS_INIT;
			cp.git_cmd = 1;
			/* BUG: this nukes untracked files in the way */
			strvec_pushl(&cp.args, "reset", "--hard", "-q",
				     "--no-recurse-submodules", রিক্ত);
			যদি (run_command(&cp)) {
				ret = -1;
				গচ্ছ done;
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
		যদি (keep_index == 1 && !is_null_oid(&info.i_বৃক্ষ) &&
		    !is_empty_বৃক্ষ_oid(&info.i_বৃক্ষ, the_repository->hash_algo)) {
			সংরচনা child_process cp = CHILD_PROCESS_INIT;

			cp.git_cmd = 1;
			strvec_pushl(&cp.args, "checkout", "--no-overlay",
				     oid_to_hex(&info.i_বৃক্ষ), "--", রিক্ত);
			যদি (!ps->nr)
				strvec_ঠেলো(&cp.args, ":/");
			অন্যথা
				add_pathspecs(&cp.args, ps);
			যদি (run_command(&cp)) {
				ret = -1;
				গচ্ছ done;
			}
		}
		গচ্ছ done;
	} অন্যথা {
		সংরচনা child_process cp = CHILD_PROCESS_INIT;

		cp.git_cmd = 1;
		strvec_pushl(&cp.args, "apply", "-R", রিক্ত);

		যদি (pipe_command(&cp, patch.buf, patch.len, রিক্ত, 0, রিক্ত, 0)) {
			যদি (!quiet)
				fprintf_ln(stderr, _("Cannot remove "
						     "worktree changes"));
			ret = -1;
			গচ্ছ done;
		}

		যদি (keep_index < 1) {
			সংরচনা child_process cp = CHILD_PROCESS_INIT;

			cp.git_cmd = 1;
			strvec_pushl(&cp.args, "reset", "-q", "--refresh", "--",
				     রিক্ত);
			add_pathspecs(&cp.args, ps);
			যদি (run_command(&cp)) {
				ret = -1;
				গচ্ছ done;
			}
		}
		গচ্ছ done;
	}

done:
	strbuf_release(&patch);
	strbuf_release(&out);
	free_stash_info(&info);
	strbuf_release(&stash_বাৰ্তা_buf);
	strbuf_release(&untracked_files);
	ঘূৰাই ret;
}

স্থির পূর্ণাঙ্ক ঠেলো_stash(পূর্ণাঙ্ক argc, ধ্রুৱ ৱর্ণ **argv, ধ্রুৱ ৱর্ণ *prefix,
		      পূর্ণাঙ্ক ঠেলো_assumed)
{
	পূর্ণাঙ্ক force_assume = 0;
	পূর্ণাঙ্ক keep_index = -1;
	পূর্ণাঙ্ক only_staged = 0;
	পূর্ণাঙ্ক patch_mode = 0;
	পূর্ণাঙ্ক include_untracked = 0;
	পূর্ণাঙ্ক quiet = 0;
	পূর্ণাঙ্ক pathspec_file_nul = 0;
	ধ্রুৱ ৱর্ণ *stash_বাৰ্তা = রিক্ত;
	ৱর্ণ *pathspec_from_file = রিক্ত;
	সংরচনা pathspec ps;
	সংরচনা interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	সংরচনা option options[] = {
		OPT_BOOL('k', "keep-index", &keep_index,
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
		OPT_STRING('m', "message", &stash_বাৰ্তা, N_("message"),
			   N_("stash message")),
		OPT_PATHSPEC_FROM_FILE(&pathspec_from_file),
		OPT_PATHSPEC_FILE_NUL(&pathspec_file_nul),
		OPT_END()
	};
	পূর্ণাঙ্ক ret;

	যদি (argc) {
		পূর্ণাঙ্ক flags = PARSE_OPT_KEEP_DASHDASH;

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

	ret = do_ঠেলো_stash(&ps, stash_বাৰ্তা, quiet, keep_index, patch_mode,
			    &interactive_opts, include_untracked, only_staged);

	clear_pathspec(&ps);
	মুক্ত(pathspec_from_file);
	ঘূৰাই ret;
}

স্থির পূর্ণাঙ্ক ঠেলো_stash_unassumed(পূর্ণাঙ্ক argc, ধ্রুৱ ৱর্ণ **argv, ধ্রুৱ ৱর্ণ *prefix,
				সংরচনা repository *repo UNUSED)
{
	ঘূৰাই ঠেলো_stash(argc, argv, prefix, 0);
}

স্থির পূর্ণাঙ্ক save_stash(পূর্ণাঙ্ক argc, ধ্রুৱ ৱর্ণ **argv, ধ্রুৱ ৱর্ণ *prefix,
		      সংরচনা repository *repo UNUSED)
{
	পূর্ণাঙ্ক keep_index = -1;
	পূর্ণাঙ্ক only_staged = 0;
	পূর্ণাঙ্ক patch_mode = 0;
	পূর্ণাঙ্ক include_untracked = 0;
	পূর্ণাঙ্ক quiet = 0;
	পূর্ণাঙ্ক ret = 0;
	ধ্রুৱ ৱর্ণ *stash_বাৰ্তা = রিক্ত;
	সংরচনা pathspec ps;
	সংরচনা strbuf stash_বাৰ্তা_buf = STRBUF_INIT;
	সংরচনা interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	সংরচনা option options[] = {
		OPT_BOOL('k', "keep-index", &keep_index,
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
		OPT_STRING('m', "message", &stash_বাৰ্তা, "message",
			   N_("stash message")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_save_usage,
			     PARSE_OPT_KEEP_DASHDASH);

	যদি (argc)
		stash_বাৰ্তা = strbuf_join_argv(&stash_বাৰ্তা_buf, argc, argv, ' ');

	memset(&ps, 0, পরিমাণ(ps));

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

	ret = do_ঠেলো_stash(&ps, stash_বাৰ্তা, quiet, keep_index,
			    patch_mode, &interactive_opts, include_untracked,
			    only_staged);

	strbuf_release(&stash_বাৰ্তা_buf);
	ঘূৰাই ret;
}

স্থির পূর্ণাঙ্ক write_commit_with_parents(সংরচনা repository *r,
				     সংরচনা object_id *out,
				     ধ্রুৱ সংরচনা object_id *oid,
				     সংরচনা commit_list *parents)
{
	size_t author_len, committer_len;
	সংরচনা commit *this;
	ধ্রুৱ ৱর্ণ *orig_author, *orig_committer;
	ৱর্ণ *author = রিক্ত, *committer = রিক্ত;
	ধ্রুৱ ৱর্ণ *buffer;
	unsigned long bufsize;
	ধ্রুৱ ৱর্ণ *p;
	সংরচনা strbuf বাৰ্তা = STRBUF_INIT;
	পূর্ণাঙ্ক ret = 0;
	সংরচনা ident_split id;

	this = lookup_commit_reference(r, oid);
	buffer = repo_get_commit_buffer(r, this, &bufsize);
	orig_author = find_commit_header(buffer, "author", &author_len);
	orig_committer = find_commit_header(buffer, "committer", &committer_len);

	যদি (!orig_author || !orig_committer) {
		ret = error(_("cannot parse commit %s"), oid_to_hex(oid));
		গচ্ছ out;
	}

	যদি (split_ident_সাৰি(&id, orig_author, author_len) < 0 ||
	    split_ident_সাৰি(&id, orig_committer, committer_len) < 0) {
		ret = error(_("invalid author or committer for %s"), oid_to_hex(oid));
		গচ্ছ out;
	}

	p = strstr(buffer, "\n\n");
	strbuf_addstr(&বাৰ্তা, "git stash: ");

	যদি (p)
		strbuf_add(&বাৰ্তা, p + 2, bufsize - (p + 2 - buffer));
	strbuf_complete_সাৰি(&বাৰ্তা);

	author = xmemdupz(orig_author, author_len);
	committer = xmemdupz(orig_committer, committer_len);

	যদি (commit_বৃক্ষ_extended(বাৰ্তা.buf, বাৰ্তা.len,
				 r->hash_algo->empty_বৃক্ষ, parents,
				 out, author, committer,
				 রিক্ত, রিক্ত)) {
		ret = error(_("could not write commit"));
		গচ্ছ out;
	}
out:
	strbuf_release(&বাৰ্তা);
	repo_unuse_commit_buffer(r, this, buffer);
	মুক্ত(author);
	মুক্ত(committer);
	ঘূৰাই ret;
}

স্থির পূর্ণাঙ্ক do_import_stash(সংরচনা repository *r, ধ্রুৱ ৱর্ণ *rev)
{
	সংরচনা object_id chain;
	পূর্ণাঙ্ক res = 0;
	ধ্রুৱ ৱর্ণ *buffer = রিক্ত;
	unsigned long bufsize;
	সংরচনা commit *this = রিক্ত;
	সংরচনা commit_list *বস্তুসমূহ = রিক্ত, *cur;
	ৱর্ণ *বাৰ্তা = রিক্ত;

	যদি (repo_get_oid(r, rev, &chain))
		ঘূৰাই error(_("not a valid revision: %s"), rev);

	this = lookup_commit_reference(r, &chain);
	যদি (!this)
		ঘূৰাই error(_("not a commit: %s"), rev);

	/*
	 * Walk the commit history, finding each stash entry, and load data into
	 * the array.
	 */
	বাবে (;;) {
		ধ্রুৱ ৱর্ণ *author, *committer;
		size_t author_len, committer_len;
		ধ্রুৱ ৱর্ণ *p;
		ধ্রুৱ ৱর্ণ *expected = "git stash <git@stash> 1000684800 +0000";
		ধ্রুৱ ৱর্ণ *prefix = "git stash: ";
		সংরচনা commit *stash;
		সংরচনা বৃক্ষ *বৃক্ষ = repo_get_commit_বৃক্ষ(r, this);

		যদি (!বৃক্ষ ||
		    !oideq(&বৃক্ষ->object.oid, r->hash_algo->empty_বৃক্ষ) ||
		    (this->parents &&
		     (!this->parents->next || this->parents->next->next))) {
			res = error(_("%s is not a valid exported stash commit"),
					oid_to_hex(&this->object.oid));
			গচ্ছ out;
		}

		buffer = repo_get_commit_buffer(r, this, &bufsize);

		যদি (!this->parents) {
			/*
			 * We don't have any parents.  Make sure this is our
			 * root commit.
			 */
			author = find_commit_header(buffer, "author", &author_len);
			committer = find_commit_header(buffer, "committer", &committer_len);

			যদি (!author || !committer) {
				error(_("cannot parse commit %s"), oid_to_hex(&this->object.oid));
				গচ্ছ out;
			}

			যদি (author_len != strlen(expected) ||
			    committer_len != strlen(expected) ||
			    memcmp(author, expected, author_len) ||
			    memcmp(committer, expected, committer_len)) {
				res = error(_("found root commit %s with invalid data"), oid_to_hex(&this->object.oid));
				গচ্ছ out;
			}
			ভঙ্গ;
		}

		p = strstr(buffer, "\n\n");
		যদি (!p) {
			res = error(_("cannot parse commit %s"), oid_to_hex(&this->object.oid));
			গচ্ছ out;
		}

		p += 2;
		যদি (((size_t)(bufsize - (p - buffer)) < strlen(prefix)) ||
		    memcmp(prefix, p, strlen(prefix))) {
			res = error(_("found stash commit %s without expected prefix"), oid_to_hex(&this->object.oid));
			গচ্ছ out;
		}

		stash = this->parents->next->item;

		যদি (repo_parse_commit(r, this->parents->item) ||
		    repo_parse_commit(r, stash)) {
			res = error(_("cannot parse parents of commit: %s"),
					oid_to_hex(&this->object.oid));
			গচ্ছ out;
		}

		যদি (check_stash_topology(r, stash)) {
			res = error(_("%s does not look like a stash commit"),
					oid_to_hex(&stash->object.oid));
			গচ্ছ out;
		}

		repo_unuse_commit_buffer(r, this, buffer);
		buffer = রিক্ত;
		বস্তুসমূহ = commit_list_ঢোকাও(stash, &বস্তুসমূহ);
		this = this->parents->item;
	}

	/*
	 * Now, walk each entry, adding it to the stash as a normal stash
	 * commit.
	 */
	বাবে (cur = বস্তুসমূহ; cur; cur = cur->next) {
		ধ্রুৱ ৱর্ণ *p;
		সংরচনা object_id *oid;

		this = cur->item;
		oid = &this->object.oid;
		buffer = repo_get_commit_buffer(r, this, &bufsize);
		যদি (!buffer) {
			res = error(_("cannot read commit buffer for %s"), oid_to_hex(oid));
			গচ্ছ out;
		}

		p = strstr(buffer, "\n\n");
		যদি (!p) {
			res = error(_("cannot parse commit %s"), oid_to_hex(oid));
			গচ্ছ out;
		}

		p += 2;
		বাৰ্তা = xmemdupz(p, bufsize - (p - buffer));
		repo_unuse_commit_buffer(r, this, buffer);
		buffer = রিক্ত;

		যদি (do_store_stash(oid, বাৰ্তা, 1)) {
			res = error(_("cannot save the stash for %s"), oid_to_hex(oid));
			গচ্ছ out;
		}
		FREE_AND_NULL(বাৰ্তা);
	}
out:
	যদি (this && buffer)
		repo_unuse_commit_buffer(r, this, buffer);
	commit_list_free(বস্তুসমূহ);
	মুক্ত(বাৰ্তা);

	ঘূৰাই res;
}

স্থির পূর্ণাঙ্ক import_stash(পূর্ণাঙ্ক argc, ধ্রুৱ ৱর্ণ **argv, ধ্রুৱ ৱর্ণ *prefix,
			সংরচনা repository *repo)
{
	সংরচনা option options[] = {
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_import_usage,
			     PARSE_OPT_KEEP_DASHDASH);

	যদি (argc != 1)
		usage_বাৰ্তা_opt("a revision is required", git_stash_import_usage, options);

	ঘূৰাই do_import_stash(repo, argv[0]);
}

সংরচনা stash_entry_data {
	সংরচনা repository *r;
	সংরচনা commit_list **বস্তুসমূহ;
	size_t গুনতি;
};

স্থির পূর্ণাঙ্ক collect_stash_entries(ধ্রুৱ ৱর্ণ *refname UNUSED,
				 সংরচনা object_id *old_oid UNUSED,
				 সংরচনা object_id *new_oid,
				 ধ্রুৱ ৱর্ণ *committer UNUSED,
				 timestamp_t timestamp UNUSED,
				 পূর্ণাঙ্ক tz UNUSED, ধ্রুৱ ৱর্ণ *বাৰ্তা UNUSED,
				 শূন্য *cb_data)
{
	সংরচনা stash_entry_data *data = cb_data;
	সংরচনা commit *stash;

	data->গুনতি++;
	stash = lookup_commit_reference(data->r, new_oid);
	যদি (!stash || check_stash_topology(data->r, stash)) {
		ঘূৰাই error(_("%s does not look like a stash commit"),
			     oid_to_hex(new_oid));
	}
	data->বস্তুসমূহ = commit_list_append(stash, data->বস্তুসমূহ);
	ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক do_export_stash(সংরচনা repository *r,
			   ধ্রুৱ ৱর্ণ *ref,
			   পূর্ণাঙ্ক argc,
			   ধ্রুৱ ৱর্ণ **argv)
{
	সংরচনা object_id base;
	সংরচনা commit *prev;
	সংরচনা commit_list *বস্তুসমূহ = রিক্ত, **iter = &বস্তুসমূহ, *cur;
	পূর্ণাঙ্ক res = 0;
	পূর্ণাঙ্ক i;
	সংরচনা strbuf revision = STRBUF_INIT;
	ধ্রুৱ ৱর্ণ *author, *committer;

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
	যদি (commit_বৃক্ষ_extended("", 0, r->hash_algo->empty_বৃক্ষ, রিক্ত,
				 &base, author, committer, রিক্ত, রিক্ত))
		ঘূৰাই error(_("unable to write base commit"));

	prev = lookup_commit_reference(r, &base);

	যদি (argc) {
		/*
		 * Find each specified stash, and load data into the array.
		 */
		বাবে (i = 0; i < argc; i++) {
			সংরচনা object_id oid;
			সংরচনা commit *stash;

			যদি (parse_stash_revision(&revision, argv[i], 1) ||
			    repo_get_oid_with_flags(r, revision.buf, &oid,
						    GET_OID_QUIETLY |
						    GET_OID_GENTLY)) {
				res = error(_("unable to find stash entry %s"), argv[i]);
				গচ্ছ out;
			}

			stash = lookup_commit_reference(r, &oid);
			যদি (!stash || check_stash_topology(r, stash)) {
				res = error(_("%s does not look like a stash commit"),
						revision.buf);
				গচ্ছ out;
			}
			iter = commit_list_append(stash, iter);
		}
	} অন্যথা {
		/*
		 * Walk the reflog, finding each stash entry, and load data into the
		 * array.
		 */
		সংরচনা stash_entry_data cb_data = {
			.r = r, .বস্তুসমূহ = iter,
		};
		যদি (refs_for_each_reflog_ent_reverse(get_main_ref_store(r),
						     "refs/stash",
						     collect_stash_entries,
						     &cb_data) && cb_data.গুনতি)
			গচ্ছ out;
	}

	/*
	 * Now, create a set of commits identical to the regular stash commits,
	 * but where their first parents form a chain to our original empty
	 * base commit.
	 */
	বস্তুসমূহ = commit_list_reverse(বস্তুসমূহ);
	বাবে (cur = বস্তুসমূহ; cur; cur = cur->next) {
		সংরচনা commit_list *parents = রিক্ত;
		সংরচনা commit_list **next = &parents;
		সংরচনা object_id out;
		সংরচনা commit *stash = cur->item;

		next = commit_list_append(prev, next);
		next = commit_list_append(stash, next);
		res = write_commit_with_parents(r, &out, &stash->object.oid, parents);
		commit_list_free(parents);
		যদি (res)
			গচ্ছ out;
		prev = lookup_commit_reference(r, &out);
	}
	যদি (ref)
		refs_update_ref(get_main_ref_store(r), রিক্ত, ref,
				&prev->object.oid, রিক্ত, 0, UPDATE_REFS_DIE_ON_ERR);
	অন্যথা
		puts(oid_to_hex(&prev->object.oid));
out:
	strbuf_release(&revision);
	commit_list_free(বস্তুসমূহ);

	ঘূৰাই res;
}

গণনা export_action {
	ACTION_NONE,
	ACTION_PRINT,
	ACTION_TO_REF,
};

স্থির পূর্ণাঙ্ক export_stash(পূর্ণাঙ্ক argc,
			ধ্রুৱ ৱর্ণ **argv,
			ধ্রুৱ ৱর্ণ *prefix,
			সংরচনা repository *repo)
{
	ধ্রুৱ ৱর্ণ *ref = রিক্ত;
	গণনা export_action action = ACTION_NONE;
	সংরচনা option options[] = {
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
		ঘূৰাই error(_("exactly one of --print and --to-ref is required"));

	ঘূৰাই do_export_stash(repo, ref, argc, argv);
}

পূর্ণাঙ্ক cmd_stash(পূর্ণাঙ্ক argc,
	      ধ্রুৱ ৱর্ণ **argv,
	      ধ্রুৱ ৱর্ণ *prefix,
	      সংরচনা repository *repo)
{
	pid_t pid = getpid();
	ধ্রুৱ ৱর্ণ *index_file;
	সংরচনা strvec args = STRVEC_INIT;
	parse_opt_subcommand_fn *fn = রিক্ত;
	সংরচনা option options[] = {
		OPT_SUBCOMMAND("apply", &fn, apply_stash),
		OPT_SUBCOMMAND("clear", &fn, clear_stash),
		OPT_SUBCOMMAND("drop", &fn, drop_stash),
		OPT_SUBCOMMAND("pop", &fn, বেৰ_কৰো_stash),
		OPT_SUBCOMMAND("branch", &fn, branch_stash),
		OPT_SUBCOMMAND("list", &fn, list_stash),
		OPT_SUBCOMMAND("show", &fn, show_stash),
		OPT_SUBCOMMAND("store", &fn, store_stash),
		OPT_SUBCOMMAND("create", &fn, create_stash),
		OPT_SUBCOMMAND("push", &fn, ঠেলো_stash_unassumed),
		OPT_SUBCOMMAND("export", &fn, export_stash),
		OPT_SUBCOMMAND("import", &fn, import_stash),
		OPT_SUBCOMMAND_F("save", &fn, save_stash, রিক্ত, PARSE_OPT_NOCOMPLETE),
		OPT_END()
	};
	ধ্রুৱ ৱর্ণ **args_copy;
	পূর্ণাঙ্ক ret;

	repo_config(the_repository, git_stash_config, রিক্ত);

	argc = parse_options(argc, argv, prefix, options, git_stash_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL |
			     PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	index_file = repo_get_index_file(the_repository);
	strbuf_addf(&stash_index_path, "%s.stash.%" PRIuMAX, index_file,
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
			ঘূৰাই 128;
		ঘূৰাই ret;
	} অন্যথা যদি (!argc) {
		ঘূৰাই !!ঠেলো_stash_unassumed(0, রিক্ত, prefix, repo);
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
	ঘূৰাই ret;
}
