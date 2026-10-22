#define USE_THE_REPOSITORY_VARIABLE

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

#define INCLUDE_ALL_FILES 2

#define BUILTIN_STASH_LIST_USAGE \
	N_("git stash list [<log-options>]")
#define BUILTIN_STASH_SHOW_USAGE \
	N_("git stash show [-u | --include-untracked | --only-untracked] [<diff-options>] [<stash>]")
#define BUILTIN_STASH_DROP_USAGE \
	N_("git stash drop [-q | --quiet] [<stash>]")
#define BUILTIN_STASH_برداشت_USAGE \
	N_("git stash pop [--index] [-q | --quiet] [<stash>]")
#define BUILTIN_STASH_APPLY_USAGE \
	N_("git stash apply [--index] [-q | --quiet] [--label-ours=<label>] [--label-theirs=<label>] [--label-base=<label>] [<stash>]")
#define BUILTIN_STASH_BRANCH_USAGE \
	N_("git stash branch <branchname> [<stash>]")
#define BUILTIN_STASH_STORE_USAGE \
	N_("git stash store [(-m | --message) <message>] [-q | --quiet] <commit>")
#define BUILTIN_STASH_افزودن_USAGE \
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

ایستا ثابت نویسه * ثابت git_stash_usage[] = {
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
	پوچ
};

ایستا ثابت نویسه * ثابت git_stash_list_usage[] = {
	BUILTIN_STASH_LIST_USAGE,
	پوچ
};

ایستا ثابت نویسه * ثابت git_stash_show_usage[] = {
	BUILTIN_STASH_SHOW_USAGE,
	پوچ
};

ایستا ثابت نویسه * ثابت git_stash_drop_usage[] = {
	BUILTIN_STASH_DROP_USAGE,
	پوچ
};

ایستا ثابت نویسه * ثابت git_stash_برداشت_usage[] = {
	BUILTIN_STASH_برداشت_USAGE,
	پوچ
};

ایستا ثابت نویسه * ثابت git_stash_apply_usage[] = {
	BUILTIN_STASH_APPLY_USAGE,
	پوچ
};

ایستا ثابت نویسه * ثابت git_stash_branch_usage[] = {
	BUILTIN_STASH_BRANCH_USAGE,
	پوچ
};

ایستا ثابت نویسه * ثابت git_stash_clear_usage[] = {
	BUILTIN_STASH_CLEAR_USAGE,
	پوچ
};

ایستا ثابت نویسه * ثابت git_stash_store_usage[] = {
	BUILTIN_STASH_STORE_USAGE,
	پوچ
};

ایستا ثابت نویسه * ثابت git_stash_افزودن_usage[] = {
	BUILTIN_STASH_افزودن_USAGE,
	پوچ
};

ایستا ثابت نویسه * ثابت git_stash_save_usage[] = {
	BUILTIN_STASH_SAVE_USAGE,
	پوچ
};

ایستا ثابت نویسه * ثابت git_stash_export_usage[] = {
	BUILTIN_STASH_EXPORT_USAGE,
	پوچ
};

ایستا ثابت نویسه * ثابت git_stash_import_usage[] = {
	BUILTIN_STASH_IMPORT_USAGE,
	پوچ
};

ایستا ثابت نویسه ref_stash[] = "refs/stash";
ایستا ساختار strbuf stash_index_path = STRBUF_INIT;

ایستا صحیح show_stat = 1;
ایستا صحیح show_patch;
ایستا صحیح show_include_untracked;
ایستا صحیح use_index;

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
ساختار stash_info {
	ساختار object_id w_commit;
	ساختار object_id b_commit;
	ساختار object_id i_commit;
	ساختار object_id u_commit;
	ساختار object_id c_commit;
	ساختار object_id w_درخت;
	ساختار object_id b_درخت;
	ساختار object_id i_درخت;
	ساختار object_id u_درخت;
	ساختار strbuf revision;
	صحیح is_stash_ref;
	صحیح has_u;
};

#define STASH_INFO_INIT { \
	.revision = STRBUF_INIT, \
}

ایستا تهی free_stash_info(ساختار stash_info *info)
{
	strbuf_release(&info->revision);
}

ایستا صحیح check_stash_topology(ساختار repository *r, ساختار commit *stash)
{
	ساختار commit *p1, *p2, *p3 = پوچ;

	/* stash must have two or three parents */
	اگر (!stash->parents || !stash->parents->next ||
			(stash->parents->next->next && stash->parents->next->next->next))
		بازگشت -1;
	p1 = stash->parents->item;
	p2 = stash->parents->next->item;
	اگر (stash->parents->next->next)
		p3 = stash->parents->next->next->item;
	اگر (repo_parse_commit(r, p1) || repo_parse_commit(r, p2) ||
			(p3 && repo_parse_commit(r, p3)))
		بازگشت -1;
	/* p2 must have a single parent, p3 must have no parents */
	اگر (!p2->parents || p2->parents->next || (p3 && p3->parents))
		بازگشت -1;
	اگر (repo_parse_commit(r, p2->parents->item))
		بازگشت -1;
	/* p2^1 must equal p1 */
	اگر (!oideq(&p1->object.oid, &p2->parents->item->object.oid))
		بازگشت -1;

	بازگشت 0;
}

ایستا تهی assert_stash_like(ساختار stash_info *info, ثابت نویسه *revision)
{
	اگر (get_oidf(&info->b_commit, "%s^1", revision) ||
	    get_oidf(&info->w_درخت, "%s:", revision) ||
	    get_oidf(&info->b_درخت, "%s^1:", revision) ||
	    get_oidf(&info->i_درخت, "%s^2:", revision))
		die(_("'%s' is not a stash-like commit"), revision);
}

ایستا صحیح parse_stash_revision(ساختار strbuf *revision, ثابت نویسه *commit, صحیح quiet)
{
	strbuf_reset(revision);
	اگر (!commit) {
		اگر (!refs_ref_exists(get_main_ref_store(the_repository), ref_stash)) {
			اگر (!quiet)
				fprintf_ln(stderr, _("No stash entries found."));
			بازگشت -1;
		}

		strbuf_addf(revision, "%s@{0}", ref_stash);
	} وگرنه اگر (strspn(commit, "0123456789") == strlen(commit)) {
		strbuf_addf(revision, "%s@{%s}", ref_stash, commit);
	} وگرنه {
		strbuf_addstr(revision, commit);
	}
	بازگشت 0;
}

ایستا صحیح get_stash_info(ساختار stash_info *info, صحیح argc, ثابت نویسه **argv)
{
	صحیح ret;
	نویسه *end_of_rev;
	نویسه *expanded_ref;
	ثابت نویسه *revision;
	ثابت نویسه *commit = پوچ;
	ساختار object_id dummy;
	ساختار strbuf symbolic = STRBUF_INIT;

	اگر (argc > 1) {
		صحیح i;
		ساختار strbuf refs_پیام = STRBUF_INIT;

		برای (i = 0; i < argc; i++)
			strbuf_addf(&refs_پیام, " '%s'", argv[i]);

		fprintf_ln(stderr, _("Too many revisions specified:%s"),
			   refs_پیام.buf);
		strbuf_release(&refs_پیام);

		بازگشت -1;
	}

	اگر (argc == 1)
		commit = argv[0];

	strbuf_init(&info->revision, 0);
	اگر (parse_stash_revision(&info->revision, commit, 0)) {
		بازگشت -1;
	}

	revision = info->revision.buf;

	اگر (repo_get_oid(the_repository, revision, &info->w_commit))
		بازگشت error(_("%s is not a valid reference"), revision);

	assert_stash_like(info, revision);

	info->has_u = !get_oidf(&info->u_درخت, "%s^3:", revision);

	end_of_rev = strchrnul(revision, '@');
	strbuf_add(&symbolic, revision, end_of_rev - revision);

	ret = repo_dwim_ref(the_repository, symbolic.buf, symbolic.len,
			    &dummy, &expanded_ref, 0);
	strbuf_release(&symbolic);
	انتخاب (ret) {
	حالت 0: /* Not found, but valid ref */
		info->is_stash_ref = 0;
		بشکن;
	حالت 1:
		info->is_stash_ref = !strcmp(expanded_ref, ref_stash);
		بشکن;
	پیشفرض: /* Invalid or ambiguous */
		بشکن;
	}

	آزاد(expanded_ref);
	بازگشت !(ret == 0 || ret == 1);
}

ایستا صحیح do_clear_stash(تهی)
{
	ساختار object_id obj;
	اگر (repo_get_oid(the_repository, ref_stash, &obj))
		بازگشت 0;

	بازگشت refs_delete_ref(get_main_ref_store(the_repository), پوچ,
			       ref_stash, &obj, 0);
}

ایستا صحیح clear_stash(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
		       ساختار repository *repo UNUSED)
{
	ساختار option options[] = {
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_clear_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	اگر (argc)
		بازگشت error(_("git stash clear with arguments is "
			       "unimplemented"));

	بازگشت do_clear_stash();
}

ایستا صحیح reset_درخت(ساختار object_id *i_درخت, صحیح update, صحیح reset)
{
	صحیح nr_trees = 1;
	ساختار unpack_trees_options opts;
	ساختار درخت_desc t[MAX_UNPACK_TREES];
	ساختار درخت *درخت;
	ساختار lock_file lock_file = LOCK_INIT;

	repo_read_index_preload(the_repository, پوچ, 0);
	اگر (refresh_index(the_repository->index, REFRESH_QUIET, پوچ, پوچ, پوچ))
		بازگشت -1;

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);

	memset(&opts, 0, اندازه(opts));

	درخت = repo_parse_درخت_indirect(the_repository, i_درخت);
	اگر (repo_parse_درخت(the_repository, درخت))
		بازگشت -1;

	init_درخت_desc(t, &درخت->object.oid, درخت->buffer, درخت->size);

	opts.head_idx = 1;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;
	opts.merge = 1;
	opts.reset = reset ? UNPACK_RESET_PROTECT_UNTRACKED : 0;
	opts.update = update;
	اگر (update)
		opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
	opts.fn = oneway_merge;

	اگر (unpack_trees(nr_trees, t, &opts))
		بازگشت -1;

	اگر (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
		بازگشت error(_("unable to write new index file"));

	بازگشت 0;
}

ایستا صحیح create_index_from_درخت(ثابت ساختار object_id *درخت_id,
				  ثابت نویسه *index_path)
{
	صحیح nr_trees = 1;
	صحیح ret = 0;
	ساختار unpack_trees_options opts;
	ساختار درخت_desc t[MAX_UNPACK_TREES];
	ساختار درخت *درخت;
	ساختار index_state dst_istate = INDEX_STATE_INIT(the_repository);
	ساختار lock_file lock_file = LOCK_INIT;

	repo_read_index_preload(the_repository, پوچ, 0);
	refresh_index(the_repository->index, REFRESH_QUIET, پوچ, پوچ, پوچ);

	hold_lock_file_for_update(&lock_file, index_path, LOCK_DIE_ON_ERROR);

	memset(&opts, 0, اندازه(opts));

	درخت = repo_parse_درخت_indirect(the_repository, درخت_id);
	اگر (!درخت || repo_parse_درخت(the_repository, درخت)) {
		ret = -1;
		برو done;
	}

	init_درخت_desc(t, &درخت->object.oid, درخت->buffer, درخت->size);

	opts.head_idx = 1;
	opts.src_index = the_repository->index;
	opts.dst_index = &dst_istate;
	opts.merge = 1;
	opts.reset = UNPACK_RESET_PROTECT_UNTRACKED;
	opts.fn = oneway_merge;

	اگر (unpack_trees(nr_trees, t, &opts)) {
		ret = -1;
		برو done;
	}

	اگر (write_locked_index(&dst_istate, &lock_file, COMMIT_LOCK)) {
		ret = error(_("unable to write new index file"));
		برو done;
	}

done:
	release_index(&dst_istate);
	اگر (ret)
		rollback_lock_file(&lock_file);
	بازگشت ret;
}

ایستا صحیح is_path_a_directory(ثابت نویسه *path)
{
	/*
	 * This function differs from abspath.c:is_directory() in that
	 * here we use lstat() instead of stat(); we do not want to
	 * follow symbolic links here.
	 */
	ساختار stat st;
	بازگشت (!lstat(path, &st) && S_ISDIR(st.st_mode));
}

ایستا تهی add_diff_to_buf(ساختار diff_queue_struct *q,
			    ساختار diff_options *options UNUSED,
			    تهی *data)
{
	صحیح i;

	برای (i = 0; i < q->nr; i++) {
		اگر (is_path_a_directory(q->queue[i]->one->path))
			ادامه;

		strbuf_addstr(data, q->queue[i]->one->path);

		/* NUL-terminate: will be fed to update-index -z */
		strbuf_addch(data, '\0');
	}
}

ایستا صحیح restore_untracked(ساختار object_id *u_درخت)
{
	صحیح res;
	ساختار child_process cp = CHILD_PROCESS_INIT;

	/*
	 * We need to run restore files from a given index, but without
	 * affecting the current index, so we use GIT_INDEX_FILE with
	 * run_command to fork processes that will not interfere.
	 */
	cp.git_cmd = 1;
	strvec_افزودن(&cp.args, "read-tree");
	strvec_افزودن(&cp.args, oid_to_hex(u_درخت));
	strvec_pushf(&cp.env, "GIT_INDEX_FILE=%s",
		     stash_index_path.buf);
	اگر (run_command(&cp)) {
		remove_path(stash_index_path.buf);
		بازگشت -1;
	}

	child_process_init(&cp);
	cp.git_cmd = 1;
	strvec_pushl(&cp.args, "checkout-index", "--all", پوچ);
	strvec_pushf(&cp.env, "GIT_INDEX_FILE=%s",
		     stash_index_path.buf);

	res = run_command(&cp);
	remove_path(stash_index_path.buf);
	بازگشت res;
}

ایستا تهی unstage_changes_unless_new(ساختار object_id *orig_درخت)
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

	ساختار checkout state = CHECKOUT_INIT;
	ساختار diff_options diff_opts;
	ساختار lock_file lock = LOCK_INIT;
	صحیح i;

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

	do_diff_cache(orig_درخت, &diff_opts);
	diffcore_std(&diff_opts);

	/* Iterate over the paths that changed due to the merge... */
	برای (i = 0; i < diff_queued_diff.nr; i++) {
		ساختار diff_filepair *p;
		ساختار cache_entry *ce;
		صحیح pos;

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
		اگر (pos >= 0 && ce_skip_worktree(the_repository->index->cache[pos])) {
			ساختار stat st;

			ce = the_repository->index->cache[pos];
			اگر (!lstat(ce->name, &st)) {
				/* Conflicting path present; relocate it */
				ساختار strbuf new_path = STRBUF_INIT;
				صحیح fd;

				strbuf_addf(&new_path,
					    "%s.stash.XXXXXX", ce->name);
				fd = xmkstemp(new_path.buf);
				close(fd);
				printf(_("WARNING: Untracked file in way of "
					 "tracked file!  Renaming\n "
					 "           %s -> %s\n"
					 "         to make room.\n"),
				       ce->name, new_path.buf);
				اگر (rename(ce->name, new_path.buf))
					die("Failed to move %s to %s",
					    ce->name, new_path.buf);
				strbuf_release(&new_path);
			}
			checkout_entry(ce, &state, پوچ, پوچ);
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
			صحیح option = ADD_CACHE_OK_TO_REPLACE;
			اگر (pos < 0)
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
	اگر (write_locked_index(the_repository->index, &lock,
			       COMMIT_LOCK | SKIP_IF_UNCHANGED))
		die(_("could not write index"));
}

ایستا شمارش stash_apply_result do_apply_stash(ثابت نویسه *prefix,
					      ساختار stash_info *info,
					      صحیح index, صحیح quiet,
					      ثابت نویسه *label_ours,
					      ثابت نویسه *label_theirs,
					      ثابت نویسه *label_base)
{
	صحیح clean, ret;
	صحیح has_index = index;
	ساختار merge_options o;
	ساختار object_id c_درخت;
	ساختار object_id index_درخت;
	ساختار درخت *head, *merge, *merge_base;
	ساختار lock_file lock = LOCK_INIT;

	repo_read_index_preload(the_repository, پوچ, 0);
	اگر (repo_refresh_and_write_index(the_repository, REFRESH_QUIET, 0, 0,
					 پوچ, پوچ, پوچ))
		بازگشت error(_("could not write index"));

	اگر (write_index_as_درخت(&c_درخت, the_repository->index,
				repo_get_index_file(the_repository), 0, پوچ))
		بازگشت error(_("cannot apply a stash in the middle of a merge"));

	init_ui_merge_options(&o, the_repository);

	اگر (quiet)
		o.verbosity = 0;

	اگر (index) {
		اگر (oideq(&info->b_درخت, &info->i_درخت) ||
		    oideq(&c_درخت, &info->i_درخت)) {
			has_index = 0;
		} وگرنه {
			ساختار merge_result result = { 0 };

			o.branch1 = "Current index";
			o.branch2 = "Stashed index changes";
			o.ancestor = "Stash base";

			head = lookup_درخت(o.repo, &c_درخت);
			merge = lookup_درخت(o.repo, &info->i_درخت);
			merge_base = lookup_درخت(o.repo, &info->b_درخت);

			merge_incore_nonrecursive(&o, merge_base, head, merge,
						  &result);

			اگر (result.clean < 0) {
				merge_finalize(&o, &result);
				بازگشت error(_("index merge failed"));
			} وگرنه اگر (!result.clean) {
				merge_finalize(&o, &result);
				بازگشت error(_("conflicts in index. "
					       "Try without --index."));
			} وگرنه {
				oidcpy(&index_درخت, &result.درخت->object.oid);
				merge_finalize(&o, &result);
			}
		}
	}

	o.branch1 = label_ours ? label_ours : "Updated upstream";
	o.branch2 = label_theirs ? label_theirs : "Stashed changes";
	o.ancestor = label_base ? label_base : "Stash base";

	اگر (oideq(&info->b_درخت, &c_درخت))
		o.branch1 = "Version stash was based on";

	اگر (o.verbosity >= 3)
		printf_ln(_("Merging %s with %s"), o.branch1, o.branch2);

	head = lookup_درخت(o.repo, &c_درخت);
	merge = lookup_درخت(o.repo, &info->w_درخت);
	merge_base = lookup_درخت(o.repo, &info->b_درخت);

	repo_hold_locked_index(o.repo, &lock, LOCK_DIE_ON_ERROR);
	clean = merge_ort_nonrecursive(&o, head, merge, merge_base);

	/*
	 * If 'clean' >= 0, reverse the value for 'ret' so 'ret' is 0 when the
	 * merge was clean, and 1 if the merge was unclean or a negative value
	 * if it encountered an error.
	 */
	ret = clean >= 0 ? !clean : clean;

	اگر (ret < 0)
		rollback_lock_file(&lock);
	وگرنه اگر (write_locked_index(o.repo->index, &lock,
				      COMMIT_LOCK | SKIP_IF_UNCHANGED))
		ret = error(_("could not write index"));

	اگر (ret) {
		repo_rerere(the_repository, 0);

		اگر (index)
			fprintf_ln(stderr, _("Index was not unstashed."));

		برو restore_untracked;
	}

	اگر (has_index) {
		اگر (reset_درخت(&index_درخت, 0, 0))
			ret = -1;
	} وگرنه {
		unstage_changes_unless_new(&c_درخت);
	}

restore_untracked:
	اگر (info->has_u && restore_untracked(&info->u_درخت))
		ret = error(_("could not restore untracked files from stash"));

	اگر (!quiet) {
		ساختار child_process cp = CHILD_PROCESS_INIT;

		/*
		 * Status is quite simple and could be replaced with calls to
		 * wt_status in the future, but it adds complexities which may
		 * require more tests.
		 */
		cp.git_cmd = 1;
		cp.dir = prefix;
		strvec_pushf(&cp.env, GIT_WORK_درخت_ENVIRONMENT"=%s",
			     absolute_path(repo_get_work_درخت(the_repository)));
		strvec_pushf(&cp.env, GIT_DIR_ENVIRONMENT"=%s",
			     absolute_path(repo_get_git_dir(the_repository)));
		strvec_افزودن(&cp.args, "status");
		run_command(&cp);
	}

	بازگشت ret;
}

ایستا صحیح apply_stash(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
		       ساختار repository *repo UNUSED)
{
	صحیح ret = -1;
	صحیح quiet = 0;
	صحیح index = use_index;
	ثابت نویسه *label_ours = پوچ, *label_theirs = پوچ, *label_base = پوچ;
	ساختار stash_info info = STASH_INFO_INIT;
	ساختار option options[] = {
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

	اگر (get_stash_info(&info, argc, argv))
		برو cleanup;

	ret = do_apply_stash(prefix, &info, index, quiet,
			     label_ours, label_theirs, label_base);
cleanup:
	free_stash_info(&info);
	بازگشت ret;
}

ایستا صحیح reject_reflog_ent(ثابت نویسه *refname UNUSED,
			     ساختار object_id *ooid UNUSED,
			     ساختار object_id *noid UNUSED,
			     ثابت نویسه *email UNUSED,
			     timestamp_t timestamp UNUSED,
			     صحیح tz UNUSED, ثابت نویسه *message UNUSED,
			     تهی *cb_data UNUSED)
{
	بازگشت 1;
}

ایستا صحیح reflog_is_empty(ثابت نویسه *refname)
{
	بازگشت !refs_for_each_reflog_ent(get_main_ref_store(the_repository),
					 refname, reject_reflog_ent, پوچ);
}

ایستا صحیح do_drop_stash(ساختار stash_info *info, صحیح quiet)
{
	اگر (!reflog_delete(info->revision.buf,
			   EXPIRE_REFLOGS_REWRITE | EXPIRE_REFLOGS_UPDATE_REF,
			   0)) {
		اگر (!quiet)
			printf_ln(_("Dropped %s (%s)"), info->revision.buf,
				  oid_to_hex(&info->w_commit));
	} وگرنه {
		بازگشت error(_("%s: Could not drop stash entry"),
			     info->revision.buf);
	}

	اگر (reflog_is_empty(ref_stash))
		do_clear_stash();

	بازگشت 0;
}

ایستا صحیح get_stash_info_assert(ساختار stash_info *info, صحیح argc,
				 ثابت نویسه **argv)
{
	صحیح ret = get_stash_info(info, argc, argv);

	اگر (ret < 0)
		بازگشت ret;

	اگر (!info->is_stash_ref)
		بازگشت error(_("'%s' is not a stash reference"), info->revision.buf);

	بازگشت 0;
}

ایستا صحیح drop_stash(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
		      ساختار repository *repo UNUSED)
{
	صحیح ret = -1;
	صحیح quiet = 0;
	ساختار stash_info info = STASH_INFO_INIT;
	ساختار option options[] = {
		OPT__QUIET(&quiet, N_("be quiet, only report errors")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_drop_usage, 0);

	اگر (get_stash_info_assert(&info, argc, argv))
		برو cleanup;

	ret = do_drop_stash(&info, quiet);
cleanup:
	free_stash_info(&info);
	بازگشت ret;
}

ایستا صحیح برداشت_stash(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
		     ساختار repository *repo UNUSED)
{
	صحیح ret = -1;
	صحیح index = use_index;
	صحیح quiet = 0;
	ساختار stash_info info = STASH_INFO_INIT;
	ساختار option options[] = {
		OPT__QUIET(&quiet, N_("be quiet, only report errors")),
		OPT_BOOL(0, "index", &index,
			 N_("attempt to recreate the index")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_برداشت_usage, 0);

	اگر (get_stash_info_assert(&info, argc, argv))
		برو cleanup;

	اگر ((ret = do_apply_stash(prefix, &info, index, quiet,
				  پوچ, پوچ, پوچ)))
		printf_ln(_("The stash entry is kept in case "
			    "you need it again."));
	وگرنه
		ret = do_drop_stash(&info, quiet);

cleanup:
	free_stash_info(&info);
	بازگشت ret;
}

ایستا صحیح branch_stash(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
			ساختار repository *repo UNUSED)
{
	صحیح ret = -1;
	ثابت نویسه *branch = پوچ;
	ساختار stash_info info = STASH_INFO_INIT;
	ساختار child_process cp = CHILD_PROCESS_INIT;
	ساختار option options[] = {
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_branch_usage, 0);

	اگر (!argc) {
		fprintf_ln(stderr, _("No branch name specified"));
		بازگشت -1;
	}

	branch = argv[0];

	اگر (get_stash_info(&info, argc - 1, argv + 1))
		برو cleanup;

	cp.git_cmd = 1;
	strvec_pushl(&cp.args, "checkout", "-b", پوچ);
	strvec_افزودن(&cp.args, branch);
	strvec_افزودن(&cp.args, oid_to_hex(&info.b_commit));
	ret = run_command(&cp);
	اگر (!ret)
		ret = do_apply_stash(prefix, &info, 1, 0,
				     پوچ, پوچ, پوچ);
	اگر (!ret && info.is_stash_ref)
		ret = do_drop_stash(&info, 0);

cleanup:
	free_stash_info(&info);
	بازگشت ret;
}

ایستا صحیح list_stash(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
		      ساختار repository *repo UNUSED)
{
	ساختار child_process cp = CHILD_PROCESS_INIT;
	ساختار option options[] = {
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_list_usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT);

	اگر (!refs_ref_exists(get_main_ref_store(the_repository), ref_stash))
		بازگشت 0;

	cp.git_cmd = 1;
	strvec_pushl(&cp.args, "log", "--format=%gd: %gs", "-g",
		     "--first-parent", پوچ);
	strvec_pushv(&cp.args, argv);
	strvec_افزودن(&cp.args, ref_stash);
	strvec_افزودن(&cp.args, "--");
	بازگشت run_command(&cp);
}

ایستا صحیح git_stash_config(ثابت نویسه *var, ثابت نویسه *مقدار,
			    ثابت ساختار config_context *ctx, تهی *cb)
{
	اگر (!strcmp(var, "stash.showstat")) {
		show_stat = git_config_bool(var, مقدار);
		بازگشت 0;
	}
	اگر (!strcmp(var, "stash.showpatch")) {
		show_patch = git_config_bool(var, مقدار);
		بازگشت 0;
	}
	اگر (!strcmp(var, "stash.showincludeuntracked")) {
		show_include_untracked = git_config_bool(var, مقدار);
		بازگشت 0;
	}
	اگر (!strcmp(var, "stash.index")) {
		use_index = git_config_bool(var, مقدار);
		بازگشت 0;
	}
	بازگشت git_diff_basic_config(var, مقدار, ctx, cb);
}

ایستا تهی diff_include_untracked(ثابت ساختار stash_info *info, ساختار diff_options *diff_opt)
{
	ثابت ساختار object_id *oid[] = { &info->w_commit, &info->u_درخت };
	ساختار درخت *درخت[ARRAY_SIZE(oid)];
	ساختار درخت_desc درخت_desc[ARRAY_SIZE(oid)];
	ساختار unpack_trees_options unpack_درخت_opt = { 0 };

	برای (size_t i = 0; i < ARRAY_SIZE(oid); i++) {
		درخت[i] = repo_parse_درخت_indirect(the_repository, oid[i]);
		اگر (repo_parse_درخت(the_repository, درخت[i]) < 0)
			die(_("failed to parse tree"));
		init_درخت_desc(&درخت_desc[i], &درخت[i]->object.oid,
			       درخت[i]->buffer, درخت[i]->size);
	}

	unpack_درخت_opt.head_idx = -1;
	unpack_درخت_opt.src_index = the_repository->index;
	unpack_درخت_opt.dst_index = the_repository->index;
	unpack_درخت_opt.merge = 1;
	unpack_درخت_opt.fn = stash_worktree_untracked_merge;

	اگر (unpack_trees(ARRAY_SIZE(درخت_desc), درخت_desc, &unpack_درخت_opt))
		die(_("failed to unpack trees"));

	do_diff_cache(&info->b_commit, diff_opt);
}

ایستا صحیح show_stash(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
		      ساختار repository *repo UNUSED)
{
	صحیح i;
	صحیح ret = -1;
	ساختار stash_info info = STASH_INFO_INIT;
	ساختار rev_info rev;
	ساختار strvec stash_args = STRVEC_INIT;
	ساختار strvec revision_args = STRVEC_INIT;
	شمارش {
		UNTRACKED_NONE,
		UNTRACKED_INCLUDE,
		UNTRACKED_ONLY
	} show_untracked = show_include_untracked ? UNTRACKED_INCLUDE : UNTRACKED_NONE;
	ساختار option options[] = {
		OPT_SET_INT('u', "include-untracked", &show_untracked,
			    N_("include untracked files in the stash"),
			    UNTRACKED_INCLUDE),
		OPT_SET_INT_F(0, "only-untracked", &show_untracked,
			      N_("only show untracked files in the stash"),
			      UNTRACKED_ONLY, PARSE_OPT_NONEG),
		OPT_END()
	};
	صحیح do_usage = 0;

	init_diff_ui_defaults();
	repo_config(the_repository, git_diff_ui_config, پوچ);
	repo_init_revisions(the_repository, &rev, prefix);

	argc = parse_options(argc, argv, prefix, options, git_stash_show_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	strvec_افزودن(&revision_args, argv[0]);
	برای (i = 1; i < argc; i++) {
		اگر (argv[i][0] != '-')
			strvec_افزودن(&stash_args, argv[i]);
		وگرنه
			strvec_افزودن(&revision_args, argv[i]);
	}

	اگر (get_stash_info(&info, stash_args.nr, stash_args.v))
		برو cleanup;

	/*
	 * The config settings are applied only if there are not passed
	 * any options.
	 */
	اگر (revision_args.nr == 1) {
		اگر (show_stat)
			rev.diffopt.output_format = DIFF_FORMAT_DIFFSTAT;

		اگر (show_patch)
			rev.diffopt.output_format |= DIFF_FORMAT_PATCH;

		اگر (!show_stat && !show_patch) {
			ret = 0;
			برو cleanup;
		}
	}

	setup_revisions_from_strvec(&revision_args, &rev, پوچ);
	اگر (revision_args.nr > 1)
		برو usage;
	اگر (!rev.diffopt.output_format) {
		rev.diffopt.output_format = DIFF_FORMAT_PATCH;
		diff_setup_done(&rev.diffopt);
	}

	rev.diffopt.flags.recursive = 1;
	setup_diff_pager(&rev.diffopt);
	انتخاب (show_untracked) {
	حالت UNTRACKED_NONE:
		diff_درخت_oid(&info.b_commit, &info.w_commit, "", &rev.diffopt);
		بشکن;
	حالت UNTRACKED_ONLY:
		اگر (info.has_u)
			diff_ریشه_درخت_oid(&info.u_درخت, "", &rev.diffopt);
		بشکن;
	حالت UNTRACKED_INCLUDE:
		اگر (info.has_u)
			diff_include_untracked(&info, &rev.diffopt);
		وگرنه
			diff_درخت_oid(&info.b_commit, &info.w_commit, "", &rev.diffopt);
		بشکن;
	}
	log_درخت_diff_flush(&rev);

	ret = diff_result_code(&rev);

cleanup:
	strvec_clear(&revision_args);
	strvec_clear(&stash_args);
	free_stash_info(&info);
	release_revisions(&rev);
	اگر (do_usage)
		usage_with_options(git_stash_show_usage, options);
	بازگشت ret;
usage:
	do_usage = 1;
	برو cleanup;
}

ایستا صحیح do_store_stash(ثابت ساختار object_id *w_commit, ثابت نویسه *stash_پیام,
			  صحیح quiet)
{
	ساختار stash_info info;
	نویسه revision[GIT_MAX_HEXSZ];

	oid_to_hex_r(revision, w_commit);
	assert_stash_like(&info, revision);

	اگر (!stash_پیام)
		stash_پیام = "Created via \"git stash store\".";

	اگر (refs_update_ref(get_main_ref_store(the_repository), stash_پیام, ref_stash, w_commit, پوچ,
			    REF_FORCE_CREATE_REFLOG,
			    quiet ? UPDATE_REFS_QUIET_ON_ERR :
			    UPDATE_REFS_پیام_ON_ERR)) {
		اگر (!quiet) {
			fprintf_ln(stderr, _("Cannot update %s with %s"),
				   ref_stash, oid_to_hex(w_commit));
		}
		بازگشت -1;
	}

	بازگشت 0;
}

ایستا صحیح store_stash(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
		       ساختار repository *repo UNUSED)
{
	صحیح quiet = 0;
	ثابت نویسه *stash_پیام = پوچ;
	ساختار object_id obj;
	ساختار option options[] = {
		OPT__QUIET(&quiet, N_("be quiet")),
		OPT_STRING('m', "message", &stash_پیام, "message",
			   N_("stash message")),
		OPT_END()
	};
	صحیح ret;

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_store_usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT);

	اگر (argc != 1) {
		اگر (!quiet)
			fprintf_ln(stderr, _("\"git stash store\" requires one "
					     "<commit> argument"));
		بازگشت -1;
	}

	اگر (repo_get_oid_with_flags(the_repository, argv[0], &obj,
				    quiet ? GET_OID_QUIETLY : 0)) {
		اگر (!quiet)
			fprintf_ln(stderr, _("Cannot update %s with %s"),
					     ref_stash, argv[0]);
		ret = -1;
		برو out;
	}

	ret = do_store_stash(&obj, stash_پیام, quiet);

out:
	بازگشت ret;
}

ایستا تهی add_pathspecs(ساختار strvec *args,
			  ثابت ساختار pathspec *ps) {
	صحیح i;

	برای (i = 0; i < ps->nr; i++)
		strvec_افزودن(args, ps->اقلام[i].original);
}

/*
 * `untracked_files` will be filled with the names of untracked files.
 * The return value is:
 *
 * = 0 if there are not any untracked files
 * > 0 if there are untracked files
 */
ایستا صحیح get_untracked_files(ثابت ساختار pathspec *ps, صحیح include_untracked,
			       ساختار strbuf *untracked_files)
{
	صحیح i;
	صحیح found = 0;
	ساختار dir_struct dir = DIR_INIT;

	اگر (include_untracked != INCLUDE_ALL_FILES)
		setup_standard_excludes(&dir);

	fill_directory(&dir, the_repository->index, ps);
	برای (i = 0; i < dir.nr; i++) {
		ساختار dir_entry *ent = dir.entries[i];
		found++;
		strbuf_addstr(untracked_files, ent->name);
		/* NUL-terminate: will be fed to update-index -z */
		strbuf_addch(untracked_files, '\0');
	}

	dir_clear(&dir);
	بازگشت found;
}

/*
 * The return value of `check_changes_tracked_files()` can be:
 *
 * < 0 if there was an error
 * = 0 if there are no changes.
 * > 0 if there are changes.
 */
ایستا صحیح check_changes_tracked_files(ثابت ساختار pathspec *ps)
{
	ساختار rev_info rev;
	ساختار object_id dummy;
	صحیح ret = 0;

	/* No initial commit. */
	اگر (repo_get_oid(the_repository, "HEAD", &dummy))
		بازگشت -1;

	اگر (repo_read_index(the_repository) < 0)
		بازگشت -1;

	repo_init_revisions(the_repository, &rev, پوچ);
	copy_pathspec(&rev.prune_data, ps);

	rev.diffopt.flags.quick = 1;
	rev.diffopt.flags.ignore_submodules = 1;
	rev.abbrev = 0;

	add_head_to_pending(&rev);
	diff_setup_done(&rev.diffopt);

	run_diff_index(&rev, DIFF_INDEX_CACHED);
	اگر (diff_result_code(&rev)) {
		ret = 1;
		برو done;
	}

	run_diff_files(&rev, 0);
	اگر (diff_result_code(&rev)) {
		ret = 1;
		برو done;
	}

done:
	release_revisions(&rev);
	بازگشت ret;
}

/*
 * The function will fill `untracked_files` with the names of untracked files
 * It will return 1 if there were any changes and 0 if there were not.
 */
ایستا صحیح check_changes(ثابت ساختار pathspec *ps, صحیح include_untracked,
			 ساختار strbuf *untracked_files)
{
	صحیح ret = 0;
	اگر (check_changes_tracked_files(ps))
		ret = 1;

	اگر (include_untracked && get_untracked_files(ps, include_untracked,
						     untracked_files))
		ret = 1;

	بازگشت ret;
}

ایستا صحیح save_untracked_files(ساختار stash_info *info, ساختار strbuf *پیام,
				ساختار strbuf *files)
{
	صحیح ret = 0;
	ساختار strbuf untracked_پیام = STRBUF_INIT;
	ساختار child_process cp_upd_index = CHILD_PROCESS_INIT;
	ساختار index_state istate = INDEX_STATE_INIT(the_repository);

	cp_upd_index.git_cmd = 1;
	strvec_pushl(&cp_upd_index.args, "update-index", "-z", "--add",
		     "--remove", "--stdin", پوچ);
	strvec_pushf(&cp_upd_index.env, "GIT_INDEX_FILE=%s",
			 stash_index_path.buf);

	strbuf_addf(&untracked_پیام, "untracked files on %s\n", پیام->buf);
	اگر (pipe_command(&cp_upd_index, files->buf, files->len, پوچ, 0,
			 پوچ, 0)) {
		ret = -1;
		برو done;
	}

	اگر (write_index_as_درخت(&info->u_درخت, &istate, stash_index_path.buf, 0,
				پوچ)) {
		ret = -1;
		برو done;
	}

	اگر (commit_درخت(untracked_پیام.buf, untracked_پیام.len,
			&info->u_درخت, پوچ, &info->u_commit, پوچ, پوچ)) {
		ret = -1;
		برو done;
	}

done:
	release_index(&istate);
	strbuf_release(&untracked_پیام);
	remove_path(stash_index_path.buf);
	بازگشت ret;
}

ایستا صحیح stash_staged(ساختار stash_info *info, ساختار strbuf *out_patch,
			صحیح quiet)
{
	صحیح ret = 0;
	ساختار child_process cp_diff_درخت = CHILD_PROCESS_INIT;
	ساختار index_state istate = INDEX_STATE_INIT(the_repository);

	اگر (write_index_as_درخت(&info->w_درخت, &istate, the_repository->index_file,
				0, پوچ)) {
		ret = -1;
		برو done;
	}

	cp_diff_درخت.git_cmd = 1;
	strvec_pushl(&cp_diff_درخت.args, "diff-tree", "-p", "--binary",
		     "--no-color",
		     "-U1", "HEAD", oid_to_hex(&info->w_درخت), "--", پوچ);
	اگر (pipe_command(&cp_diff_درخت, پوچ, 0, out_patch, 0, پوچ, 0)) {
		ret = -1;
		برو done;
	}

	اگر (!out_patch->len) {
		اگر (!quiet)
			fprintf_ln(stderr, _("No staged changes"));
		ret = 1;
	}

done:
	release_index(&istate);
	بازگشت ret;
}

ایستا صحیح stash_patch(ساختار stash_info *info, ثابت ساختار pathspec *ps,
		       ساختار strbuf *out_patch, صحیح quiet,
		       ساختار interactive_options *interactive_opts)
{
	صحیح ret = 0;
	ساختار child_process cp_diff_درخت = CHILD_PROCESS_INIT;
	ساختار commit *head_commit;
	ثابت ساختار object_id *head_درخت;
	ساختار index_state istate = INDEX_STATE_INIT(the_repository);
	نویسه *old_index_env = پوچ, *old_repo_index_file;

	remove_path(stash_index_path.buf);

	head_commit = lookup_commit(the_repository, &info->b_commit);
	اگر (!head_commit || repo_parse_commit(the_repository, head_commit)) {
		ret = -1;
		برو done;
	}
	head_درخت = get_commit_درخت_oid(head_commit);
	اگر (!head_درخت) {
		ret = -1;
		برو done;
	}

	اگر (create_index_from_درخت(head_درخت, stash_index_path.buf)) {
		ret = -1;
		برو done;
	}

	/* Find out what the user wants. */
	old_repo_index_file = the_repository->index_file;
	the_repository->index_file = stash_index_path.buf;
	old_index_env = xstrdup_or_null(getenv(INDEX_ENVIRONMENT));
	setenv(INDEX_ENVIRONMENT, the_repository->index_file, 1);

	ret = !!run_add_p(the_repository, ADD_P_STASH, interactive_opts, پوچ, ps, 0);

	the_repository->index_file = old_repo_index_file;
	اگر (old_index_env && *old_index_env)
		setenv(INDEX_ENVIRONMENT, old_index_env, 1);
	وگرنه
		unsetenv(INDEX_ENVIRONMENT);
	FREE_AND_NULL(old_index_env);

	/* State of the working tree. */
	اگر (write_index_as_درخت(&info->w_درخت, &istate, stash_index_path.buf, 0,
				پوچ)) {
		ret = -1;
		برو done;
	}

	cp_diff_درخت.git_cmd = 1;
	strvec_pushl(&cp_diff_درخت.args, "diff-tree", "-p", "-U1", "HEAD",
		     "--no-color",
		     oid_to_hex(&info->w_درخت), "--", پوچ);
	اگر (pipe_command(&cp_diff_درخت, پوچ, 0, out_patch, 0, پوچ, 0)) {
		ret = -1;
		برو done;
	}

	اگر (!out_patch->len) {
		اگر (!quiet)
			fprintf_ln(stderr, _("No changes selected"));
		ret = 1;
	}

done:
	release_index(&istate);
	remove_path(stash_index_path.buf);
	بازگشت ret;
}

ایستا صحیح stash_working_درخت(ساختار stash_info *info, ثابت ساختار pathspec *ps)
{
	صحیح ret = 0;
	ساختار rev_info rev;
	ساختار child_process cp_upd_index = CHILD_PROCESS_INIT;
	ساختار strbuf diff_output = STRBUF_INIT;
	ساختار index_state istate = INDEX_STATE_INIT(the_repository);

	repo_init_revisions(the_repository, &rev, پوچ);
	copy_pathspec(&rev.prune_data, ps);

	set_alternate_index_output(stash_index_path.buf);
	اگر (reset_درخت(&info->i_درخت, 0, 0)) {
		ret = -1;
		برو done;
	}
	set_alternate_index_output(پوچ);

	rev.diffopt.output_format = DIFF_FORMAT_CALLBACK;
	rev.diffopt.format_callback = add_diff_to_buf;
	rev.diffopt.format_callback_data = &diff_output;

	اگر (repo_read_index_preload(the_repository, &rev.diffopt.pathspec, 0) < 0) {
		ret = -1;
		برو done;
	}

	add_pending_object(&rev, parse_object(the_repository, &info->b_commit),
			   "");
	run_diff_index(&rev, 0);

	cp_upd_index.git_cmd = 1;
	strvec_pushl(&cp_upd_index.args, "update-index",
		     "--ignore-skip-worktree-entries",
		     "-z", "--add", "--remove", "--stdin", پوچ);
	strvec_pushf(&cp_upd_index.env, "GIT_INDEX_FILE=%s",
		     stash_index_path.buf);

	اگر (pipe_command(&cp_upd_index, diff_output.buf, diff_output.len,
			 پوچ, 0, پوچ, 0)) {
		ret = -1;
		برو done;
	}

	اگر (write_index_as_درخت(&info->w_درخت, &istate, stash_index_path.buf, 0,
				پوچ)) {
		ret = -1;
		برو done;
	}

done:
	release_index(&istate);
	release_revisions(&rev);
	strbuf_release(&diff_output);
	remove_path(stash_index_path.buf);
	بازگشت ret;
}

ایستا صحیح do_create_stash(ثابت ساختار pathspec *ps, ساختار strbuf *stash_پیام_buf,
			   صحیح include_untracked, صحیح patch_mode,
			   ساختار interactive_options *interactive_opts,
			   صحیح only_staged, ساختار stash_info *info, ساختار strbuf *patch,
			   صحیح quiet)
{
	صحیح ret = 0;
	صحیح flags = 0;
	صحیح untracked_commit_option = 0;
	ثابت نویسه *head_short_sha1 = پوچ;
	ثابت نویسه *branch_ref = پوچ;
	ثابت نویسه *branch_name = "(no branch)";
	نویسه *branch_name_buf = پوچ;
	ساختار commit *head_commit = پوچ;
	ساختار commit_list *parents = پوچ;
	ساختار strbuf پیام = STRBUF_INIT;
	ساختار strbuf commit_درخت_label = STRBUF_INIT;
	ساختار strbuf untracked_files = STRBUF_INIT;

	prepare_fallback_ident("git stash", "git@stash");

	repo_read_index_preload(the_repository, پوچ, 0);
	اگر (repo_refresh_and_write_index(the_repository, REFRESH_QUIET, 0, 0,
					 پوچ, پوچ, پوچ) < 0) {
		ret = error(_("could not write index"));
		برو done;
	}

	اگر (repo_get_oid(the_repository, "HEAD", &info->b_commit)) {
		اگر (!quiet)
			fprintf_ln(stderr, _("You do not have "
					     "the initial commit yet"));
		ret = -1;
		برو done;
	} وگرنه {
		head_commit = lookup_commit(the_repository, &info->b_commit);
	}

	اگر (!check_changes(ps, include_untracked, &untracked_files)) {
		ret = 1;
		برو done;
	}

	branch_ref = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
					     "HEAD", 0, پوچ, &flags);

	اگر (flags & REF_ISSYMREF) {
		اگر (skip_prefix(branch_ref, "refs/heads/", &branch_name))
			branch_name = branch_name_buf = xstrdup(branch_name);
	}

	head_short_sha1 = repo_find_unique_abbrev(the_repository,
						  &head_commit->object.oid,
						  DEFAULT_ABBREV);
	strbuf_addf(&پیام, "%s: %s ", branch_name, head_short_sha1);
	pp_commit_easy(CMIT_FMT_ONELINE, head_commit, &پیام);

	strbuf_addf(&commit_درخت_label, "index on %s\n", پیام.buf);
	commit_list_درج(head_commit, &parents);
	اگر (write_index_as_درخت(&info->i_درخت, the_repository->index,
				repo_get_index_file(the_repository), 0, پوچ) ||
	    commit_درخت(commit_درخت_label.buf, commit_درخت_label.len,
			&info->i_درخت, parents, &info->i_commit, پوچ, پوچ)) {
		اگر (!quiet)
			fprintf_ln(stderr, _("Cannot save the current "
					     "index state"));
		ret = -1;
		برو done;
	}

	commit_list_free(parents);
	parents = پوچ;

	اگر (include_untracked) {
		اگر (save_untracked_files(info, &پیام, &untracked_files)) {
			اگر (!quiet)
				fprintf_ln(stderr, _("Cannot save "
						     "the untracked files"));
			ret = -1;
			برو done;
		}
		untracked_commit_option = 1;
	}
	اگر (patch_mode) {
		ret = stash_patch(info, ps, patch, quiet, interactive_opts);
		اگر (ret < 0) {
			اگر (!quiet)
				fprintf_ln(stderr, _("Cannot save the current "
						     "worktree state"));
			برو done;
		} وگرنه اگر (ret > 0) {
			برو done;
		}
	} وگرنه اگر (only_staged) {
		ret = stash_staged(info, patch, quiet);
		اگر (ret < 0) {
			اگر (!quiet)
				fprintf_ln(stderr, _("Cannot save the current "
						     "staged state"));
			برو done;
		} وگرنه اگر (ret > 0) {
			برو done;
		}
	} وگرنه {
		اگر (stash_working_درخت(info, ps)) {
			اگر (!quiet)
				fprintf_ln(stderr, _("Cannot save the current "
						     "worktree state"));
			ret = -1;
			برو done;
		}
	}

	اگر (!stash_پیام_buf->len)
		strbuf_addf(stash_پیام_buf, "WIP on %s", پیام.buf);
	وگرنه
		strbuf_insertf(stash_پیام_buf, 0, "On %s: ", branch_name);

	اگر (untracked_commit_option)
		commit_list_درج(lookup_commit(the_repository,
						 &info->u_commit),
				   &parents);
	commit_list_درج(lookup_commit(the_repository, &info->i_commit),
			   &parents);
	commit_list_درج(head_commit, &parents);

	اگر (commit_درخت(stash_پیام_buf->buf, stash_پیام_buf->len, &info->w_درخت,
			parents, &info->w_commit, پوچ, پوچ)) {
		اگر (!quiet)
			fprintf_ln(stderr, _("Cannot record "
					     "working tree state"));
		ret = -1;
		برو done;
	}

done:
	strbuf_release(&commit_درخت_label);
	strbuf_release(&پیام);
	strbuf_release(&untracked_files);
	commit_list_free(parents);
	آزاد(branch_name_buf);
	بازگشت ret;
}

ایستا صحیح create_stash(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix UNUSED,
			ساختار repository *repo UNUSED)
{
	صحیح ret;
	ساختار strbuf stash_پیام_buf = STRBUF_INIT;
	ساختار stash_info info = STASH_INFO_INIT;
	ساختار pathspec ps;

	/* Starting with argv[1], since argv[0] is "create" */
	strbuf_join_argv(&stash_پیام_buf, argc - 1, ++argv, ' ');

	memset(&ps, 0, اندازه(ps));
	اگر (!check_changes_tracked_files(&ps))
		بازگشت 0;

	ret = do_create_stash(&ps, &stash_پیام_buf, 0, 0, پوچ, 0, &info,
			      پوچ, 0);
	اگر (!ret)
		printf_ln("%s", oid_to_hex(&info.w_commit));

	free_stash_info(&info);
	strbuf_release(&stash_پیام_buf);
	بازگشت ret;
}

ایستا صحیح do_افزودن_stash(ثابت ساختار pathspec *ps, ثابت نویسه *stash_پیام, صحیح quiet,
			 صحیح keep_index, صحیح patch_mode,
			 ساختار interactive_options *interactive_opts,
			 صحیح include_untracked, صحیح only_staged)
{
	صحیح ret = 0;
	ساختار stash_info info = STASH_INFO_INIT;
	ساختار strbuf patch = STRBUF_INIT;
	ساختار strbuf stash_پیام_buf = STRBUF_INIT;
	ساختار strbuf untracked_files = STRBUF_INIT;
	ساختار strbuf out = STRBUF_INIT;

	اگر (patch_mode && keep_index == -1)
		keep_index = 1;

	اگر (patch_mode && include_untracked) {
		fprintf_ln(stderr, _("Can't use --patch and --include-untracked"
				     " or --all at the same time"));
		ret = -1;
		برو done;
	}

	/* --patch overrides --staged */
	اگر (patch_mode)
		only_staged = 0;

	اگر (only_staged && include_untracked) {
		fprintf_ln(stderr, _("Can't use --staged and --include-untracked"
				     " or --all at the same time"));
		ret = -1;
		برو done;
	}

	repo_read_index_preload(the_repository, پوچ, 0);
	اگر (!include_untracked && ps->nr) {
		نویسه *ps_matched = xcalloc(ps->nr, 1);

		اگر (pathspec_needs_expanded_index(the_repository->index, ps))
			ensure_full_index(the_repository->index);
		برای (size_t i = 0; i < the_repository->index->cache_nr; i++)
			ce_path_match(the_repository->index, the_repository->index->cache[i], ps,
				      ps_matched);

		اگر (report_path_error(ps_matched, ps)) {
			fprintf_ln(stderr, _("Did you forget to 'git add'?"));
			ret = -1;
			آزاد(ps_matched);
			برو done;
		}
		آزاد(ps_matched);
	}

	اگر (repo_refresh_and_write_index(the_repository, REFRESH_QUIET, 0, 0,
					 پوچ, پوچ, پوچ)) {
		ret = error(_("could not write index"));
		برو done;
	}

	اگر (!check_changes(ps, include_untracked, &untracked_files)) {
		اگر (!quiet)
			printf_ln(_("No local changes to save"));
		برو done;
	}

	اگر (!refs_reflog_exists(get_main_ref_store(the_repository), ref_stash) && do_clear_stash()) {
		ret = -1;
		اگر (!quiet)
			fprintf_ln(stderr, _("Cannot initialize stash"));
		برو done;
	}

	اگر (stash_پیام)
		strbuf_addstr(&stash_پیام_buf, stash_پیام);
	اگر (do_create_stash(ps, &stash_پیام_buf, include_untracked, patch_mode,
			    interactive_opts, only_staged, &info, &patch, quiet)) {
		ret = -1;
		برو done;
	}

	اگر (do_store_stash(&info.w_commit, stash_پیام_buf.buf, 1)) {
		ret = -1;
		اگر (!quiet)
			fprintf_ln(stderr, _("Cannot save the current status"));
		برو done;
	}

	اگر (!quiet)
		printf_ln(_("Saved working directory and index state %s"),
			  stash_پیام_buf.buf);

	اگر (!(patch_mode || only_staged)) {
		اگر (include_untracked && !ps->nr) {
			ساختار child_process cp = CHILD_PROCESS_INIT;

			cp.git_cmd = 1;
			اگر (startup_info->original_cwd) {
				cp.dir = startup_info->original_cwd;
				strvec_pushf(&cp.env, "%s=%s",
					     GIT_WORK_درخت_ENVIRONMENT,
					     the_repository->worktree);
			}
			strvec_pushl(&cp.args, "clean", "--force",
				     "--quiet", "-d", ":/", پوچ);
			اگر (include_untracked == INCLUDE_ALL_FILES)
				strvec_افزودن(&cp.args, "-x");
			اگر (run_command(&cp)) {
				ret = -1;
				برو done;
			}
		}
		discard_index(the_repository->index);
		اگر (ps->nr) {
			ساختار child_process cp_add = CHILD_PROCESS_INIT;
			ساختار child_process cp_diff = CHILD_PROCESS_INIT;
			ساختار child_process cp_apply = CHILD_PROCESS_INIT;

			cp_add.git_cmd = 1;
			strvec_افزودن(&cp_add.args, "add");
			اگر (!include_untracked)
				strvec_افزودن(&cp_add.args, "-u");
			اگر (include_untracked == INCLUDE_ALL_FILES)
				strvec_افزودن(&cp_add.args, "--force");
			strvec_افزودن(&cp_add.args, "--");
			add_pathspecs(&cp_add.args, ps);
			اگر (run_command(&cp_add)) {
				ret = -1;
				برو done;
			}

			cp_diff.git_cmd = 1;
			strvec_pushl(&cp_diff.args, "diff-index", "-p",
				     "--no-color",
				     "--cached", "--binary", "HEAD", "--",
				     پوچ);
			add_pathspecs(&cp_diff.args, ps);
			اگر (pipe_command(&cp_diff, پوچ, 0, &out, 0, پوچ, 0)) {
				ret = -1;
				برو done;
			}

			cp_apply.git_cmd = 1;
			strvec_pushl(&cp_apply.args, "apply", "--index",
				     "-R", پوچ);
			اگر (pipe_command(&cp_apply, out.buf, out.len, پوچ, 0,
					 پوچ, 0)) {
				ret = -1;
				برو done;
			}
		} وگرنه {
			ساختار child_process cp = CHILD_PROCESS_INIT;
			cp.git_cmd = 1;
			/* BUG: this nukes untracked files in the way */
			strvec_pushl(&cp.args, "reset", "--hard", "-q",
				     "--no-recurse-submodules", پوچ);
			اگر (run_command(&cp)) {
				ret = -1;
				برو done;
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
		اگر (keep_index == 1 && !is_null_oid(&info.i_درخت) &&
		    !is_empty_درخت_oid(&info.i_درخت, the_repository->hash_algo)) {
			ساختار child_process cp = CHILD_PROCESS_INIT;

			cp.git_cmd = 1;
			strvec_pushl(&cp.args, "checkout", "--no-overlay",
				     oid_to_hex(&info.i_درخت), "--", پوچ);
			اگر (!ps->nr)
				strvec_افزودن(&cp.args, ":/");
			وگرنه
				add_pathspecs(&cp.args, ps);
			اگر (run_command(&cp)) {
				ret = -1;
				برو done;
			}
		}
		برو done;
	} وگرنه {
		ساختار child_process cp = CHILD_PROCESS_INIT;

		cp.git_cmd = 1;
		strvec_pushl(&cp.args, "apply", "-R", پوچ);

		اگر (pipe_command(&cp, patch.buf, patch.len, پوچ, 0, پوچ, 0)) {
			اگر (!quiet)
				fprintf_ln(stderr, _("Cannot remove "
						     "worktree changes"));
			ret = -1;
			برو done;
		}

		اگر (keep_index < 1) {
			ساختار child_process cp = CHILD_PROCESS_INIT;

			cp.git_cmd = 1;
			strvec_pushl(&cp.args, "reset", "-q", "--refresh", "--",
				     پوچ);
			add_pathspecs(&cp.args, ps);
			اگر (run_command(&cp)) {
				ret = -1;
				برو done;
			}
		}
		برو done;
	}

done:
	strbuf_release(&patch);
	strbuf_release(&out);
	free_stash_info(&info);
	strbuf_release(&stash_پیام_buf);
	strbuf_release(&untracked_files);
	بازگشت ret;
}

ایستا صحیح افزودن_stash(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
		      صحیح افزودن_assumed)
{
	صحیح force_assume = 0;
	صحیح keep_index = -1;
	صحیح only_staged = 0;
	صحیح patch_mode = 0;
	صحیح include_untracked = 0;
	صحیح quiet = 0;
	صحیح pathspec_file_nul = 0;
	ثابت نویسه *stash_پیام = پوچ;
	نویسه *pathspec_from_file = پوچ;
	ساختار pathspec ps;
	ساختار interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	ساختار option options[] = {
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
		OPT_STRING('m', "message", &stash_پیام, N_("message"),
			   N_("stash message")),
		OPT_PATHSPEC_FROM_FILE(&pathspec_from_file),
		OPT_PATHSPEC_FILE_NUL(&pathspec_file_nul),
		OPT_END()
	};
	صحیح ret;

	اگر (argc) {
		صحیح flags = PARSE_OPT_KEEP_DASHDASH;

		اگر (افزودن_assumed)
			flags |= PARSE_OPT_STOP_AT_NON_OPTION;

		argc = parse_options(argc, argv, prefix, options,
				     افزودن_assumed ? git_stash_usage :
				     git_stash_افزودن_usage, flags);
		force_assume |= patch_mode;
	}

	اگر (argc) {
		اگر (!strcmp(argv[0], "--")) {
			argc--;
			argv++;
		} وگرنه اگر (افزودن_assumed && !force_assume) {
			die("subcommand wasn't specified; 'push' can't be assumed due to unexpected token '%s'",
			    argv[0]);
		}
	}

	parse_pathspec(&ps, 0, PATHSPEC_PREFER_FULL | PATHSPEC_PREFIX_ORIGIN,
		       prefix, argv);

	اگر (pathspec_from_file) {
		اگر (patch_mode)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--patch");

		اگر (only_staged)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--staged");

		اگر (ps.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&ps, 0,
				    PATHSPEC_PREFER_FULL | PATHSPEC_PREFIX_ORIGIN,
				    prefix, pathspec_from_file, pathspec_file_nul);
	} وگرنه اگر (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	اگر (!patch_mode) {
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

	ret = do_افزودن_stash(&ps, stash_پیام, quiet, keep_index, patch_mode,
			    &interactive_opts, include_untracked, only_staged);

	clear_pathspec(&ps);
	آزاد(pathspec_from_file);
	بازگشت ret;
}

ایستا صحیح افزودن_stash_unassumed(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
				ساختار repository *repo UNUSED)
{
	بازگشت افزودن_stash(argc, argv, prefix, 0);
}

ایستا صحیح save_stash(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
		      ساختار repository *repo UNUSED)
{
	صحیح keep_index = -1;
	صحیح only_staged = 0;
	صحیح patch_mode = 0;
	صحیح include_untracked = 0;
	صحیح quiet = 0;
	صحیح ret = 0;
	ثابت نویسه *stash_پیام = پوچ;
	ساختار pathspec ps;
	ساختار strbuf stash_پیام_buf = STRBUF_INIT;
	ساختار interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	ساختار option options[] = {
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
		OPT_STRING('m', "message", &stash_پیام, "message",
			   N_("stash message")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_save_usage,
			     PARSE_OPT_KEEP_DASHDASH);

	اگر (argc)
		stash_پیام = strbuf_join_argv(&stash_پیام_buf, argc, argv, ' ');

	memset(&ps, 0, اندازه(ps));

	اگر (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	اگر (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	اگر (!patch_mode) {
		اگر (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--patch");
		اگر (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--patch");
		اگر (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--patch");
	}

	ret = do_افزودن_stash(&ps, stash_پیام, quiet, keep_index,
			    patch_mode, &interactive_opts, include_untracked,
			    only_staged);

	strbuf_release(&stash_پیام_buf);
	بازگشت ret;
}

ایستا صحیح write_commit_with_parents(ساختار repository *r,
				     ساختار object_id *out,
				     ثابت ساختار object_id *oid,
				     ساختار commit_list *parents)
{
	size_t author_len, committer_len;
	ساختار commit *this;
	ثابت نویسه *orig_author, *orig_committer;
	نویسه *author = پوچ, *committer = پوچ;
	ثابت نویسه *buffer;
	unsigned long bufsize;
	ثابت نویسه *p;
	ساختار strbuf پیام = STRBUF_INIT;
	صحیح ret = 0;
	ساختار ident_split id;

	this = lookup_commit_reference(r, oid);
	buffer = repo_get_commit_buffer(r, this, &bufsize);
	orig_author = find_commit_header(buffer, "author", &author_len);
	orig_committer = find_commit_header(buffer, "committer", &committer_len);

	اگر (!orig_author || !orig_committer) {
		ret = error(_("cannot parse commit %s"), oid_to_hex(oid));
		برو out;
	}

	اگر (split_ident_سطر(&id, orig_author, author_len) < 0 ||
	    split_ident_سطر(&id, orig_committer, committer_len) < 0) {
		ret = error(_("invalid author or committer for %s"), oid_to_hex(oid));
		برو out;
	}

	p = strstr(buffer, "\n\n");
	strbuf_addstr(&پیام, "git stash: ");

	اگر (p)
		strbuf_add(&پیام, p + 2, bufsize - (p + 2 - buffer));
	strbuf_complete_سطر(&پیام);

	author = xmemdupz(orig_author, author_len);
	committer = xmemdupz(orig_committer, committer_len);

	اگر (commit_درخت_extended(پیام.buf, پیام.len,
				 r->hash_algo->empty_درخت, parents,
				 out, author, committer,
				 پوچ, پوچ)) {
		ret = error(_("could not write commit"));
		برو out;
	}
out:
	strbuf_release(&پیام);
	repo_unuse_commit_buffer(r, this, buffer);
	آزاد(author);
	آزاد(committer);
	بازگشت ret;
}

ایستا صحیح do_import_stash(ساختار repository *r, ثابت نویسه *rev)
{
	ساختار object_id chain;
	صحیح res = 0;
	ثابت نویسه *buffer = پوچ;
	unsigned long bufsize;
	ساختار commit *this = پوچ;
	ساختار commit_list *اقلام = پوچ, *cur;
	نویسه *پیام = پوچ;

	اگر (repo_get_oid(r, rev, &chain))
		بازگشت error(_("not a valid revision: %s"), rev);

	this = lookup_commit_reference(r, &chain);
	اگر (!this)
		بازگشت error(_("not a commit: %s"), rev);

	/*
	 * Walk the commit history, finding each stash entry, and load data into
	 * the array.
	 */
	برای (;;) {
		ثابت نویسه *author, *committer;
		size_t author_len, committer_len;
		ثابت نویسه *p;
		ثابت نویسه *expected = "git stash <git@stash> 1000684800 +0000";
		ثابت نویسه *prefix = "git stash: ";
		ساختار commit *stash;
		ساختار درخت *درخت = repo_get_commit_درخت(r, this);

		اگر (!درخت ||
		    !oideq(&درخت->object.oid, r->hash_algo->empty_درخت) ||
		    (this->parents &&
		     (!this->parents->next || this->parents->next->next))) {
			res = error(_("%s is not a valid exported stash commit"),
					oid_to_hex(&this->object.oid));
			برو out;
		}

		buffer = repo_get_commit_buffer(r, this, &bufsize);

		اگر (!this->parents) {
			/*
			 * We don't have any parents.  Make sure this is our
			 * root commit.
			 */
			author = find_commit_header(buffer, "author", &author_len);
			committer = find_commit_header(buffer, "committer", &committer_len);

			اگر (!author || !committer) {
				error(_("cannot parse commit %s"), oid_to_hex(&this->object.oid));
				برو out;
			}

			اگر (author_len != strlen(expected) ||
			    committer_len != strlen(expected) ||
			    memcmp(author, expected, author_len) ||
			    memcmp(committer, expected, committer_len)) {
				res = error(_("found root commit %s with invalid data"), oid_to_hex(&this->object.oid));
				برو out;
			}
			بشکن;
		}

		p = strstr(buffer, "\n\n");
		اگر (!p) {
			res = error(_("cannot parse commit %s"), oid_to_hex(&this->object.oid));
			برو out;
		}

		p += 2;
		اگر (((size_t)(bufsize - (p - buffer)) < strlen(prefix)) ||
		    memcmp(prefix, p, strlen(prefix))) {
			res = error(_("found stash commit %s without expected prefix"), oid_to_hex(&this->object.oid));
			برو out;
		}

		stash = this->parents->next->item;

		اگر (repo_parse_commit(r, this->parents->item) ||
		    repo_parse_commit(r, stash)) {
			res = error(_("cannot parse parents of commit: %s"),
					oid_to_hex(&this->object.oid));
			برو out;
		}

		اگر (check_stash_topology(r, stash)) {
			res = error(_("%s does not look like a stash commit"),
					oid_to_hex(&stash->object.oid));
			برو out;
		}

		repo_unuse_commit_buffer(r, this, buffer);
		buffer = پوچ;
		اقلام = commit_list_درج(stash, &اقلام);
		this = this->parents->item;
	}

	/*
	 * Now, walk each entry, adding it to the stash as a normal stash
	 * commit.
	 */
	برای (cur = اقلام; cur; cur = cur->next) {
		ثابت نویسه *p;
		ساختار object_id *oid;

		this = cur->item;
		oid = &this->object.oid;
		buffer = repo_get_commit_buffer(r, this, &bufsize);
		اگر (!buffer) {
			res = error(_("cannot read commit buffer for %s"), oid_to_hex(oid));
			برو out;
		}

		p = strstr(buffer, "\n\n");
		اگر (!p) {
			res = error(_("cannot parse commit %s"), oid_to_hex(oid));
			برو out;
		}

		p += 2;
		پیام = xmemdupz(p, bufsize - (p - buffer));
		repo_unuse_commit_buffer(r, this, buffer);
		buffer = پوچ;

		اگر (do_store_stash(oid, پیام, 1)) {
			res = error(_("cannot save the stash for %s"), oid_to_hex(oid));
			برو out;
		}
		FREE_AND_NULL(پیام);
	}
out:
	اگر (this && buffer)
		repo_unuse_commit_buffer(r, this, buffer);
	commit_list_free(اقلام);
	آزاد(پیام);

	بازگشت res;
}

ایستا صحیح import_stash(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
			ساختار repository *repo)
{
	ساختار option options[] = {
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_import_usage,
			     PARSE_OPT_KEEP_DASHDASH);

	اگر (argc != 1)
		usage_پیام_opt("a revision is required", git_stash_import_usage, options);

	بازگشت do_import_stash(repo, argv[0]);
}

ساختار stash_entry_data {
	ساختار repository *r;
	ساختار commit_list **اقلام;
	size_t شمار;
};

ایستا صحیح collect_stash_entries(ثابت نویسه *refname UNUSED,
				 ساختار object_id *old_oid UNUSED,
				 ساختار object_id *new_oid,
				 ثابت نویسه *committer UNUSED,
				 timestamp_t timestamp UNUSED,
				 صحیح tz UNUSED, ثابت نویسه *پیام UNUSED,
				 تهی *cb_data)
{
	ساختار stash_entry_data *data = cb_data;
	ساختار commit *stash;

	data->شمار++;
	stash = lookup_commit_reference(data->r, new_oid);
	اگر (!stash || check_stash_topology(data->r, stash)) {
		بازگشت error(_("%s does not look like a stash commit"),
			     oid_to_hex(new_oid));
	}
	data->اقلام = commit_list_append(stash, data->اقلام);
	بازگشت 0;
}

ایستا صحیح do_export_stash(ساختار repository *r,
			   ثابت نویسه *ref,
			   صحیح argc,
			   ثابت نویسه **argv)
{
	ساختار object_id base;
	ساختار commit *prev;
	ساختار commit_list *اقلام = پوچ, **iter = &اقلام, *cur;
	صحیح res = 0;
	صحیح i;
	ساختار strbuf revision = STRBUF_INIT;
	ثابت نویسه *author, *committer;

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
	اگر (commit_درخت_extended("", 0, r->hash_algo->empty_درخت, پوچ,
				 &base, author, committer, پوچ, پوچ))
		بازگشت error(_("unable to write base commit"));

	prev = lookup_commit_reference(r, &base);

	اگر (argc) {
		/*
		 * Find each specified stash, and load data into the array.
		 */
		برای (i = 0; i < argc; i++) {
			ساختار object_id oid;
			ساختار commit *stash;

			اگر (parse_stash_revision(&revision, argv[i], 1) ||
			    repo_get_oid_with_flags(r, revision.buf, &oid,
						    GET_OID_QUIETLY |
						    GET_OID_GENTLY)) {
				res = error(_("unable to find stash entry %s"), argv[i]);
				برو out;
			}

			stash = lookup_commit_reference(r, &oid);
			اگر (!stash || check_stash_topology(r, stash)) {
				res = error(_("%s does not look like a stash commit"),
						revision.buf);
				برو out;
			}
			iter = commit_list_append(stash, iter);
		}
	} وگرنه {
		/*
		 * Walk the reflog, finding each stash entry, and load data into the
		 * array.
		 */
		ساختار stash_entry_data cb_data = {
			.r = r, .اقلام = iter,
		};
		اگر (refs_for_each_reflog_ent_reverse(get_main_ref_store(r),
						     "refs/stash",
						     collect_stash_entries,
						     &cb_data) && cb_data.شمار)
			برو out;
	}

	/*
	 * Now, create a set of commits identical to the regular stash commits,
	 * but where their first parents form a chain to our original empty
	 * base commit.
	 */
	اقلام = commit_list_reverse(اقلام);
	برای (cur = اقلام; cur; cur = cur->next) {
		ساختار commit_list *parents = پوچ;
		ساختار commit_list **next = &parents;
		ساختار object_id out;
		ساختار commit *stash = cur->item;

		next = commit_list_append(prev, next);
		next = commit_list_append(stash, next);
		res = write_commit_with_parents(r, &out, &stash->object.oid, parents);
		commit_list_free(parents);
		اگر (res)
			برو out;
		prev = lookup_commit_reference(r, &out);
	}
	اگر (ref)
		refs_update_ref(get_main_ref_store(r), پوچ, ref,
				&prev->object.oid, پوچ, 0, UPDATE_REFS_DIE_ON_ERR);
	وگرنه
		puts(oid_to_hex(&prev->object.oid));
out:
	strbuf_release(&revision);
	commit_list_free(اقلام);

	بازگشت res;
}

شمارش export_action {
	ACTION_NONE,
	ACTION_PRINT,
	ACTION_TO_REF,
};

ایستا صحیح export_stash(صحیح argc,
			ثابت نویسه **argv,
			ثابت نویسه *prefix,
			ساختار repository *repo)
{
	ثابت نویسه *ref = پوچ;
	شمارش export_action action = ACTION_NONE;
	ساختار option options[] = {
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

	اگر (ref && action == ACTION_NONE)
		action = ACTION_TO_REF;

	اگر (action == ACTION_NONE || (ref && action == ACTION_PRINT))
		بازگشت error(_("exactly one of --print and --to-ref is required"));

	بازگشت do_export_stash(repo, ref, argc, argv);
}

صحیح cmd_stash(صحیح argc,
	      ثابت نویسه **argv,
	      ثابت نویسه *prefix,
	      ساختار repository *repo)
{
	pid_t pid = getpid();
	ثابت نویسه *index_file;
	ساختار strvec args = STRVEC_INIT;
	parse_opt_subcommand_fn *fn = پوچ;
	ساختار option options[] = {
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
		OPT_SUBCOMMAND_F("save", &fn, save_stash, پوچ, PARSE_OPT_NOCOMPLETE),
		OPT_END()
	};
	ثابت نویسه **args_copy;
	صحیح ret;

	repo_config(the_repository, git_stash_config, پوچ);

	argc = parse_options(argc, argv, prefix, options, git_stash_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL |
			     PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	index_file = repo_get_index_file(the_repository);
	strbuf_addf(&stash_index_path, "%s.stash.%" PRIuMAX, index_file,
		    (uintmax_t)pid);

	اگر (fn) {
		ret = fn(argc, argv, prefix, repo);

		/*
		 * The subcommand implementations return 0 on success, a
		 * negative value on failure, and STASH_APPLY_CONFLICT
		 * when applying a stash entry resulted in conflicts.
		 * Map failures to 128, the status die() uses, so that
		 * exit status 1 unambiguously indicates conflicts.
		 */
		اگر (ret < 0)
			بازگشت 128;
		بازگشت ret;
	} وگرنه اگر (!argc) {
		بازگشت !!افزودن_stash_unassumed(0, پوچ, prefix, repo);
	}

	/* Assume 'stash push' */
	strvec_افزودن(&args, "push");
	strvec_pushv(&args, argv);

	/*
	 * `push_stash()` ends up modifying the array, which causes memory
	 * leaks if we didn't copy the array here.
	 */
	DUP_ARRAY(args_copy, args.v, args.nr);

	ret = !!افزودن_stash(args.nr, args_copy, prefix, 1);

	strvec_clear(&args);
	آزاد(args_copy);
	بازگشت ret;
}
