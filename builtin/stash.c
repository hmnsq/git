#define USE_THE_REPOSITORY_VARIABLE

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

#define INCLUDE_ALL_FILES 2

#define BUILTIN_STASH_LIST_USAGE \
	N_("git stash list [<log-options>]")
#define BUILTIN_STASH_SHOW_USAGE \
	N_("git stash show [-u | --include-untracked | --only-untracked] [<diff-options>] [<stash>]")
#define BUILTIN_STASH_DROP_USAGE \
	N_("git stash drop [-q | --quiet] [<stash>]")
#define BUILTIN_STASH_POP_USAGE \
	N_("git stash pop [--index] [-q | --quiet] [<stash>]")
#define BUILTIN_STASH_APPLY_USAGE \
	N_("git stash apply [--index] [-q | --quiet] [--label-ours=<label>] [--label-theirs=<label>] [--label-base=<label>] [<stash>]")
#define BUILTIN_STASH_BRANCH_USAGE \
	N_("git stash branch <branchname> [<stash>]")
#define BUILTIN_STASH_STORE_USAGE \
	N_("git stash store [(-m | --message) <message>] [-q | --quiet] <commit>")
#define BUILTIN_STASH_PUSH_USAGE \
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

ساكن ثابت حرف * ثابت git_stash_usage[] = {
	BUILTIN_STASH_LIST_USAGE,
	BUILTIN_STASH_SHOW_USAGE,
	BUILTIN_STASH_DROP_USAGE,
	BUILTIN_STASH_POP_USAGE,
	BUILTIN_STASH_APPLY_USAGE,
	BUILTIN_STASH_BRANCH_USAGE,
	BUILTIN_STASH_PUSH_USAGE,
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

ساكن ثابت حرف * ثابت git_stash_pop_usage[] = {
	BUILTIN_STASH_POP_USAGE,
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

ساكن ثابت حرف * ثابت git_stash_push_usage[] = {
	BUILTIN_STASH_PUSH_USAGE,
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
ساكن بنية strbuf stash_index_path = STRBUF_INIT;

ساكن صحيح show_stat = 1;
ساكن صحيح show_patch;
ساكن صحيح show_include_untracked;
ساكن صحيح use_index;

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
	بنية object_id w_tree;
	بنية object_id b_tree;
	بنية object_id i_tree;
	بنية object_id u_tree;
	بنية strbuf revision;
	صحيح is_stash_ref;
	صحيح has_u;
};

#define STASH_INFO_INIT { \
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
	ܐܢ (!stash->parents || !stash->parents->next ||
			(stash->parents->next->next && stash->parents->next->next->next))
		ܦܢܐ -1;
	p1 = stash->parents->item;
	p2 = stash->parents->next->item;
	ܐܢ (stash->parents->next->next)
		p3 = stash->parents->next->next->item;
	ܐܢ (repo_parse_commit(r, p1) || repo_parse_commit(r, p2) ||
			(p3 && repo_parse_commit(r, p3)))
		ܦܢܐ -1;
	/* p2 must have a single parent, p3 must have no parents */
	ܐܢ (!p2->parents || p2->parents->next || (p3 && p3->parents))
		ܦܢܐ -1;
	ܐܢ (repo_parse_commit(r, p2->parents->item))
		ܦܢܐ -1;
	/* p2^1 must equal p1 */
	ܐܢ (!oideq(&p1->object.oid, &p2->parents->item->object.oid))
		ܦܢܐ -1;

	ܦܢܐ 0;
}

ساكن فراغ assert_stash_like(بنية stash_info *info, ثابت حرف *revision)
{
	ܐܢ (get_oidf(&info->b_commit, "%s^1", revision) ||
	    get_oidf(&info->w_tree, "%s:", revision) ||
	    get_oidf(&info->b_tree, "%s^1:", revision) ||
	    get_oidf(&info->i_tree, "%s^2:", revision))
		die(_("'%s' is not a stash-like commit"), revision);
}

ساكن صحيح parse_stash_revision(بنية strbuf *revision, ثابت حرف *commit, صحيح quiet)
{
	strbuf_reset(revision);
	ܐܢ (!commit) {
		ܐܢ (!refs_ref_exists(get_main_ref_store(the_repository), ref_stash)) {
			ܐܢ (!quiet)
				fprintf_ln(stderr, _("No stash entries found."));
			ܦܢܐ -1;
		}

		strbuf_addf(revision, "%s@{0}", ref_stash);
	} ܐܚܪܢܐ ܐܢ (strspn(commit, "0123456789") == strlen(commit)) {
		strbuf_addf(revision, "%s@{%s}", ref_stash, commit);
	} ܐܚܪܢܐ {
		strbuf_addstr(revision, commit);
	}
	ܦܢܐ 0;
}

ساكن صحيح get_stash_info(بنية stash_info *info, صحيح argc, ثابت حرف **argv)
{
	صحيح ret;
	حرف *end_of_rev;
	حرف *expanded_ref;
	ثابت حرف *revision;
	ثابت حرف *commit = عدم;
	بنية object_id dummy;
	بنية strbuf symbolic = STRBUF_INIT;

	ܐܢ (argc > 1) {
		صحيح i;
		بنية strbuf refs_msg = STRBUF_INIT;

		لكل (i = 0; i < argc; i++)
			strbuf_addf(&refs_msg, " '%s'", argv[i]);

		fprintf_ln(stderr, _("Too many revisions specified:%s"),
			   refs_msg.buf);
		strbuf_release(&refs_msg);

		ܦܢܐ -1;
	}

	ܐܢ (argc == 1)
		commit = argv[0];

	strbuf_init(&info->revision, 0);
	ܐܢ (parse_stash_revision(&info->revision, commit, 0)) {
		ܦܢܐ -1;
	}

	revision = info->revision.buf;

	ܐܢ (repo_get_oid(the_repository, revision, &info->w_commit))
		ܦܢܐ error(_("%s is not a valid reference"), revision);

	assert_stash_like(info, revision);

	info->has_u = !get_oidf(&info->u_tree, "%s^3:", revision);

	end_of_rev = strchrnul(revision, '@');
	strbuf_add(&symbolic, revision, end_of_rev - revision);

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
	ܦܢܐ !(ret == 0 || ret == 1);
}

ساكن صحيح do_clear_stash(فراغ)
{
	بنية object_id obj;
	ܐܢ (repo_get_oid(the_repository, ref_stash, &obj))
		ܦܢܐ 0;

	ܦܢܐ refs_delete_ref(get_main_ref_store(the_repository), عدم,
			       ref_stash, &obj, 0);
}

ساكن صحيح clear_stash(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
		       بنية repository *repo UNUSED)
{
	بنية option options[] = {
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_clear_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	ܐܢ (argc)
		ܦܢܐ error(_("git stash clear with arguments is "
			       "unimplemented"));

	ܦܢܐ do_clear_stash();
}

ساكن صحيح reset_tree(بنية object_id *i_tree, صحيح update, صحيح reset)
{
	صحيح nr_trees = 1;
	بنية unpack_trees_options opts;
	بنية tree_desc t[MAX_UNPACK_TREES];
	بنية tree *tree;
	بنية lock_file lock_file = LOCK_INIT;

	repo_read_index_preload(the_repository, عدم, 0);
	ܐܢ (refresh_index(the_repository->index, REFRESH_QUIET, عدم, عدم, عدم))
		ܦܢܐ -1;

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);

	memset(&opts, 0, حجم(opts));

	tree = repo_parse_tree_indirect(the_repository, i_tree);
	ܐܢ (repo_parse_tree(the_repository, tree))
		ܦܢܐ -1;

	init_tree_desc(t, &tree->object.oid, tree->buffer, tree->size);

	opts.head_idx = 1;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;
	opts.merge = 1;
	opts.reset = reset ? UNPACK_RESET_PROTECT_UNTRACKED : 0;
	opts.update = update;
	ܐܢ (update)
		opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
	opts.fn = oneway_merge;

	ܐܢ (unpack_trees(nr_trees, t, &opts))
		ܦܢܐ -1;

	ܐܢ (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
		ܦܢܐ error(_("unable to write new index file"));

	ܦܢܐ 0;
}

ساكن صحيح create_index_from_tree(ثابت بنية object_id *tree_id,
				  ثابت حرف *index_path)
{
	صحيح nr_trees = 1;
	صحيح ret = 0;
	بنية unpack_trees_options opts;
	بنية tree_desc t[MAX_UNPACK_TREES];
	بنية tree *tree;
	بنية index_state dst_istate = INDEX_STATE_INIT(the_repository);
	بنية lock_file lock_file = LOCK_INIT;

	repo_read_index_preload(the_repository, عدم, 0);
	refresh_index(the_repository->index, REFRESH_QUIET, عدم, عدم, عدم);

	hold_lock_file_for_update(&lock_file, index_path, LOCK_DIE_ON_ERROR);

	memset(&opts, 0, حجم(opts));

	tree = repo_parse_tree_indirect(the_repository, tree_id);
	ܐܢ (!tree || repo_parse_tree(the_repository, tree)) {
		ret = -1;
		اذهب done;
	}

	init_tree_desc(t, &tree->object.oid, tree->buffer, tree->size);

	opts.head_idx = 1;
	opts.src_index = the_repository->index;
	opts.dst_index = &dst_istate;
	opts.merge = 1;
	opts.reset = UNPACK_RESET_PROTECT_UNTRACKED;
	opts.fn = oneway_merge;

	ܐܢ (unpack_trees(nr_trees, t, &opts)) {
		ret = -1;
		اذهب done;
	}

	ܐܢ (write_locked_index(&dst_istate, &lock_file, COMMIT_LOCK)) {
		ret = error(_("unable to write new index file"));
		اذهب done;
	}

done:
	release_index(&dst_istate);
	ܐܢ (ret)
		rollback_lock_file(&lock_file);
	ܦܢܐ ret;
}

ساكن صحيح is_path_a_directory(ثابت حرف *path)
{
	/*
	 * This function differs from abspath.c:is_directory() in that
	 * here we use lstat() instead of stat(); we do not want to
	 * follow symbolic links here.
	 */
	بنية stat st;
	ܦܢܐ (!lstat(path, &st) && S_ISDIR(st.st_mode));
}

ساكن فراغ add_diff_to_buf(بنية diff_queue_struct *q,
			    بنية diff_options *options UNUSED,
			    فراغ *data)
{
	صحيح i;

	لكل (i = 0; i < q->nr; i++) {
		ܐܢ (is_path_a_directory(q->queue[i]->one->path))
			استمر;

		strbuf_addstr(data, q->queue[i]->one->path);

		/* NUL-terminate: will be fed to update-index -z */
		strbuf_addch(data, '\0');
	}
}

ساكن صحيح restore_untracked(بنية object_id *u_tree)
{
	صحيح res;
	بنية child_process cp = CHILD_PROCESS_INIT;

	/*
	 * We need to run restore files from a given index, but without
	 * affecting the current index, so we use GIT_INDEX_FILE with
	 * run_command to fork processes that will not interfere.
	 */
	cp.git_cmd = 1;
	strvec_push(&cp.args, "read-tree");
	strvec_push(&cp.args, oid_to_hex(u_tree));
	strvec_pushf(&cp.env, "GIT_INDEX_FILE=%s",
		     stash_index_path.buf);
	ܐܢ (run_command(&cp)) {
		remove_path(stash_index_path.buf);
		ܦܢܐ -1;
	}

	child_process_init(&cp);
	cp.git_cmd = 1;
	strvec_pushl(&cp.args, "checkout-index", "--all", عدم);
	strvec_pushf(&cp.env, "GIT_INDEX_FILE=%s",
		     stash_index_path.buf);

	res = run_command(&cp);
	remove_path(stash_index_path.buf);
	ܦܢܐ res;
}

ساكن فراغ unstage_changes_unless_new(بنية object_id *orig_tree)
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
	بنية lock_file lock = LOCK_INIT;
	صحيح i;

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

	do_diff_cache(orig_tree, &diff_opts);
	diffcore_std(&diff_opts);

	/* Iterate over the paths that changed due to the merge... */
	لكل (i = 0; i < diff_queued_diff.nr; i++) {
		بنية diff_filepair *p;
		بنية cache_entry *ce;
		صحيح pos;

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
		ܐܢ (pos >= 0 && ce_skip_worktree(the_repository->index->cache[pos])) {
			بنية stat st;

			ce = the_repository->index->cache[pos];
			ܐܢ (!lstat(ce->name, &st)) {
				/* Conflicting path present; relocate it */
				بنية strbuf new_path = STRBUF_INIT;
				صحيح fd;

				strbuf_addf(&new_path,
					    "%s.stash.XXXXXX", ce->name);
				fd = xmkstemp(new_path.buf);
				close(fd);
				printf(_("WARNING: Untracked file in way of "
					 "tracked file!  Renaming\n "
					 "           %s -> %s\n"
					 "         to make room.\n"),
				       ce->name, new_path.buf);
				ܐܢ (rename(ce->name, new_path.buf))
					die("Failed to move %s to %s",
					    ce->name, new_path.buf);
				strbuf_release(&new_path);
			}
			checkout_entry(ce, &state, عدم, عدم);
			ce->ce_flags &= ~CE_SKIP_WORKTREE;
		}

		/*
		 * Step 3: "unstage" changes, as long as they are still tracked
		 */
		ܐܢ (p->one->oid_valid) {
			/*
			 * Path existed in orig_tree; restore index entry
			 * from that tree in order to "unstage" the changes.
			 */
			صحيح option = ADD_CACHE_OK_TO_REPLACE;
			ܐܢ (pos < 0)
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
	ܐܢ (write_locked_index(the_repository->index, &lock,
			       COMMIT_LOCK | SKIP_IF_UNCHANGED))
		die(_("could not write index"));
}

ساكن تعداد stash_apply_result do_apply_stash(ثابت حرف *prefix,
					      بنية stash_info *info,
					      صحيح index, صحيح quiet,
					      ثابت حرف *label_ours,
					      ثابت حرف *label_theirs,
					      ثابت حرف *label_base)
{
	صحيح clean, ret;
	صحيح has_index = index;
	بنية merge_options o;
	بنية object_id c_tree;
	بنية object_id index_tree;
	بنية tree *head, *merge, *merge_base;
	بنية lock_file lock = LOCK_INIT;

	repo_read_index_preload(the_repository, عدم, 0);
	ܐܢ (repo_refresh_and_write_index(the_repository, REFRESH_QUIET, 0, 0,
					 عدم, عدم, عدم))
		ܦܢܐ error(_("could not write index"));

	ܐܢ (write_index_as_tree(&c_tree, the_repository->index,
				repo_get_index_file(the_repository), 0, عدم))
		ܦܢܐ error(_("cannot apply a stash in the middle of a merge"));

	init_ui_merge_options(&o, the_repository);

	ܐܢ (quiet)
		o.verbosity = 0;

	ܐܢ (index) {
		ܐܢ (oideq(&info->b_tree, &info->i_tree) ||
		    oideq(&c_tree, &info->i_tree)) {
			has_index = 0;
		} ܐܚܪܢܐ {
			بنية merge_result result = { 0 };

			o.branch1 = "Current index";
			o.branch2 = "Stashed index changes";
			o.ancestor = "Stash base";

			head = lookup_tree(o.repo, &c_tree);
			merge = lookup_tree(o.repo, &info->i_tree);
			merge_base = lookup_tree(o.repo, &info->b_tree);

			merge_incore_nonrecursive(&o, merge_base, head, merge,
						  &result);

			ܐܢ (result.clean < 0) {
				merge_finalize(&o, &result);
				ܦܢܐ error(_("index merge failed"));
			} ܐܚܪܢܐ ܐܢ (!result.clean) {
				merge_finalize(&o, &result);
				ܦܢܐ error(_("conflicts in index. "
					       "Try without --index."));
			} ܐܚܪܢܐ {
				oidcpy(&index_tree, &result.tree->object.oid);
				merge_finalize(&o, &result);
			}
		}
	}

	o.branch1 = label_ours ? label_ours : "Updated upstream";
	o.branch2 = label_theirs ? label_theirs : "Stashed changes";
	o.ancestor = label_base ? label_base : "Stash base";

	ܐܢ (oideq(&info->b_tree, &c_tree))
		o.branch1 = "Version stash was based on";

	ܐܢ (o.verbosity >= 3)
		printf_ln(_("Merging %s with %s"), o.branch1, o.branch2);

	head = lookup_tree(o.repo, &c_tree);
	merge = lookup_tree(o.repo, &info->w_tree);
	merge_base = lookup_tree(o.repo, &info->b_tree);

	repo_hold_locked_index(o.repo, &lock, LOCK_DIE_ON_ERROR);
	clean = merge_ort_nonrecursive(&o, head, merge, merge_base);

	/*
	 * If 'clean' >= 0, reverse the value for 'ret' so 'ret' is 0 when the
	 * merge was clean, and 1 if the merge was unclean or a negative value
	 * if it encountered an error.
	 */
	ret = clean >= 0 ? !clean : clean;

	ܐܢ (ret < 0)
		rollback_lock_file(&lock);
	ܐܚܪܢܐ ܐܢ (write_locked_index(o.repo->index, &lock,
				      COMMIT_LOCK | SKIP_IF_UNCHANGED))
		ret = error(_("could not write index"));

	ܐܢ (ret) {
		repo_rerere(the_repository, 0);

		ܐܢ (index)
			fprintf_ln(stderr, _("Index was not unstashed."));

		اذهب restore_untracked;
	}

	ܐܢ (has_index) {
		ܐܢ (reset_tree(&index_tree, 0, 0))
			ret = -1;
	} ܐܚܪܢܐ {
		unstage_changes_unless_new(&c_tree);
	}

restore_untracked:
	ܐܢ (info->has_u && restore_untracked(&info->u_tree))
		ret = error(_("could not restore untracked files from stash"));

	ܐܢ (!quiet) {
		بنية child_process cp = CHILD_PROCESS_INIT;

		/*
		 * Status is quite simple and could be replaced with calls to
		 * wt_status in the future, but it adds complexities which may
		 * require more tests.
		 */
		cp.git_cmd = 1;
		cp.dir = prefix;
		strvec_pushf(&cp.env, GIT_WORK_TREE_ENVIRONMENT"=%s",
			     absolute_path(repo_get_work_tree(the_repository)));
		strvec_pushf(&cp.env, GIT_DIR_ENVIRONMENT"=%s",
			     absolute_path(repo_get_git_dir(the_repository)));
		strvec_push(&cp.args, "status");
		run_command(&cp);
	}

	ܦܢܐ ret;
}

ساكن صحيح apply_stash(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
		       بنية repository *repo UNUSED)
{
	صحيح ret = -1;
	صحيح quiet = 0;
	صحيح index = use_index;
	ثابت حرف *label_ours = عدم, *label_theirs = عدم, *label_base = عدم;
	بنية stash_info info = STASH_INFO_INIT;
	بنية option options[] = {
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

	ܐܢ (get_stash_info(&info, argc, argv))
		اذهب cleanup;

	ret = do_apply_stash(prefix, &info, index, quiet,
			     label_ours, label_theirs, label_base);
cleanup:
	free_stash_info(&info);
	ܦܢܐ ret;
}

ساكن صحيح reject_reflog_ent(ثابت حرف *refname UNUSED,
			     بنية object_id *ooid UNUSED,
			     بنية object_id *noid UNUSED,
			     ثابت حرف *email UNUSED,
			     timestamp_t timestamp UNUSED,
			     صحيح tz UNUSED, ثابت حرف *message UNUSED,
			     فراغ *cb_data UNUSED)
{
	ܦܢܐ 1;
}

ساكن صحيح reflog_is_empty(ثابت حرف *refname)
{
	ܦܢܐ !refs_for_each_reflog_ent(get_main_ref_store(the_repository),
					 refname, reject_reflog_ent, عدم);
}

ساكن صحيح do_drop_stash(بنية stash_info *info, صحيح quiet)
{
	ܐܢ (!reflog_delete(info->revision.buf,
			   EXPIRE_REFLOGS_REWRITE | EXPIRE_REFLOGS_UPDATE_REF,
			   0)) {
		ܐܢ (!quiet)
			printf_ln(_("Dropped %s (%s)"), info->revision.buf,
				  oid_to_hex(&info->w_commit));
	} ܐܚܪܢܐ {
		ܦܢܐ error(_("%s: Could not drop stash entry"),
			     info->revision.buf);
	}

	ܐܢ (reflog_is_empty(ref_stash))
		do_clear_stash();

	ܦܢܐ 0;
}

ساكن صحيح get_stash_info_assert(بنية stash_info *info, صحيح argc,
				 ثابت حرف **argv)
{
	صحيح ret = get_stash_info(info, argc, argv);

	ܐܢ (ret < 0)
		ܦܢܐ ret;

	ܐܢ (!info->is_stash_ref)
		ܦܢܐ error(_("'%s' is not a stash reference"), info->revision.buf);

	ܦܢܐ 0;
}

ساكن صحيح drop_stash(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
		      بنية repository *repo UNUSED)
{
	صحيح ret = -1;
	صحيح quiet = 0;
	بنية stash_info info = STASH_INFO_INIT;
	بنية option options[] = {
		OPT__QUIET(&quiet, N_("be quiet, only report errors")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_drop_usage, 0);

	ܐܢ (get_stash_info_assert(&info, argc, argv))
		اذهب cleanup;

	ret = do_drop_stash(&info, quiet);
cleanup:
	free_stash_info(&info);
	ܦܢܐ ret;
}

ساكن صحيح pop_stash(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
		     بنية repository *repo UNUSED)
{
	صحيح ret = -1;
	صحيح index = use_index;
	صحيح quiet = 0;
	بنية stash_info info = STASH_INFO_INIT;
	بنية option options[] = {
		OPT__QUIET(&quiet, N_("be quiet, only report errors")),
		OPT_BOOL(0, "index", &index,
			 N_("attempt to recreate the index")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_pop_usage, 0);

	ܐܢ (get_stash_info_assert(&info, argc, argv))
		اذهب cleanup;

	ܐܢ ((ret = do_apply_stash(prefix, &info, index, quiet,
				  عدم, عدم, عدم)))
		printf_ln(_("The stash entry is kept in case "
			    "you need it again."));
	ܐܚܪܢܐ
		ret = do_drop_stash(&info, quiet);

cleanup:
	free_stash_info(&info);
	ܦܢܐ ret;
}

ساكن صحيح branch_stash(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
			بنية repository *repo UNUSED)
{
	صحيح ret = -1;
	ثابت حرف *branch = عدم;
	بنية stash_info info = STASH_INFO_INIT;
	بنية child_process cp = CHILD_PROCESS_INIT;
	بنية option options[] = {
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_branch_usage, 0);

	ܐܢ (!argc) {
		fprintf_ln(stderr, _("No branch name specified"));
		ܦܢܐ -1;
	}

	branch = argv[0];

	ܐܢ (get_stash_info(&info, argc - 1, argv + 1))
		اذهب cleanup;

	cp.git_cmd = 1;
	strvec_pushl(&cp.args, "checkout", "-b", عدم);
	strvec_push(&cp.args, branch);
	strvec_push(&cp.args, oid_to_hex(&info.b_commit));
	ret = run_command(&cp);
	ܐܢ (!ret)
		ret = do_apply_stash(prefix, &info, 1, 0,
				     عدم, عدم, عدم);
	ܐܢ (!ret && info.is_stash_ref)
		ret = do_drop_stash(&info, 0);

cleanup:
	free_stash_info(&info);
	ܦܢܐ ret;
}

ساكن صحيح list_stash(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
		      بنية repository *repo UNUSED)
{
	بنية child_process cp = CHILD_PROCESS_INIT;
	بنية option options[] = {
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_list_usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT);

	ܐܢ (!refs_ref_exists(get_main_ref_store(the_repository), ref_stash))
		ܦܢܐ 0;

	cp.git_cmd = 1;
	strvec_pushl(&cp.args, "log", "--format=%gd: %gs", "-g",
		     "--first-parent", عدم);
	strvec_pushv(&cp.args, argv);
	strvec_push(&cp.args, ref_stash);
	strvec_push(&cp.args, "--");
	ܦܢܐ run_command(&cp);
}

ساكن صحيح git_stash_config(ثابت حرف *var, ثابت حرف *value,
			    ثابت بنية config_context *ctx, فراغ *cb)
{
	ܐܢ (!strcmp(var, "stash.showstat")) {
		show_stat = git_config_bool(var, value);
		ܦܢܐ 0;
	}
	ܐܢ (!strcmp(var, "stash.showpatch")) {
		show_patch = git_config_bool(var, value);
		ܦܢܐ 0;
	}
	ܐܢ (!strcmp(var, "stash.showincludeuntracked")) {
		show_include_untracked = git_config_bool(var, value);
		ܦܢܐ 0;
	}
	ܐܢ (!strcmp(var, "stash.index")) {
		use_index = git_config_bool(var, value);
		ܦܢܐ 0;
	}
	ܦܢܐ git_diff_basic_config(var, value, ctx, cb);
}

ساكن فراغ diff_include_untracked(ثابت بنية stash_info *info, بنية diff_options *diff_opt)
{
	ثابت بنية object_id *oid[] = { &info->w_commit, &info->u_tree };
	بنية tree *tree[ARRAY_SIZE(oid)];
	بنية tree_desc tree_desc[ARRAY_SIZE(oid)];
	بنية unpack_trees_options unpack_tree_opt = { 0 };

	لكل (size_t i = 0; i < ARRAY_SIZE(oid); i++) {
		tree[i] = repo_parse_tree_indirect(the_repository, oid[i]);
		ܐܢ (repo_parse_tree(the_repository, tree[i]) < 0)
			die(_("failed to parse tree"));
		init_tree_desc(&tree_desc[i], &tree[i]->object.oid,
			       tree[i]->buffer, tree[i]->size);
	}

	unpack_tree_opt.head_idx = -1;
	unpack_tree_opt.src_index = the_repository->index;
	unpack_tree_opt.dst_index = the_repository->index;
	unpack_tree_opt.merge = 1;
	unpack_tree_opt.fn = stash_worktree_untracked_merge;

	ܐܢ (unpack_trees(ARRAY_SIZE(tree_desc), tree_desc, &unpack_tree_opt))
		die(_("failed to unpack trees"));

	do_diff_cache(&info->b_commit, diff_opt);
}

ساكن صحيح show_stash(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
		      بنية repository *repo UNUSED)
{
	صحيح i;
	صحيح ret = -1;
	بنية stash_info info = STASH_INFO_INIT;
	بنية rev_info rev;
	بنية strvec stash_args = STRVEC_INIT;
	بنية strvec revision_args = STRVEC_INIT;
	تعداد {
		UNTRACKED_NONE,
		UNTRACKED_INCLUDE,
		UNTRACKED_ONLY
	} show_untracked = show_include_untracked ? UNTRACKED_INCLUDE : UNTRACKED_NONE;
	بنية option options[] = {
		OPT_SET_INT('u', "include-untracked", &show_untracked,
			    N_("include untracked files in the stash"),
			    UNTRACKED_INCLUDE),
		OPT_SET_INT_F(0, "only-untracked", &show_untracked,
			      N_("only show untracked files in the stash"),
			      UNTRACKED_ONLY, PARSE_OPT_NONEG),
		OPT_END()
	};
	صحيح do_usage = 0;

	init_diff_ui_defaults();
	repo_config(the_repository, git_diff_ui_config, عدم);
	repo_init_revisions(the_repository, &rev, prefix);

	argc = parse_options(argc, argv, prefix, options, git_stash_show_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	strvec_push(&revision_args, argv[0]);
	لكل (i = 1; i < argc; i++) {
		ܐܢ (argv[i][0] != '-')
			strvec_push(&stash_args, argv[i]);
		ܐܚܪܢܐ
			strvec_push(&revision_args, argv[i]);
	}

	ܐܢ (get_stash_info(&info, stash_args.nr, stash_args.v))
		اذهب cleanup;

	/*
	 * The config settings are applied only if there are not passed
	 * any options.
	 */
	ܐܢ (revision_args.nr == 1) {
		ܐܢ (show_stat)
			rev.diffopt.output_format = DIFF_FORMAT_DIFFSTAT;

		ܐܢ (show_patch)
			rev.diffopt.output_format |= DIFF_FORMAT_PATCH;

		ܐܢ (!show_stat && !show_patch) {
			ret = 0;
			اذهب cleanup;
		}
	}

	setup_revisions_from_strvec(&revision_args, &rev, عدم);
	ܐܢ (revision_args.nr > 1)
		اذهب usage;
	ܐܢ (!rev.diffopt.output_format) {
		rev.diffopt.output_format = DIFF_FORMAT_PATCH;
		diff_setup_done(&rev.diffopt);
	}

	rev.diffopt.flags.recursive = 1;
	setup_diff_pager(&rev.diffopt);
	بدل (show_untracked) {
	حالة UNTRACKED_NONE:
		diff_tree_oid(&info.b_commit, &info.w_commit, "", &rev.diffopt);
		اكسر;
	حالة UNTRACKED_ONLY:
		ܐܢ (info.has_u)
			diff_root_tree_oid(&info.u_tree, "", &rev.diffopt);
		اكسر;
	حالة UNTRACKED_INCLUDE:
		ܐܢ (info.has_u)
			diff_include_untracked(&info, &rev.diffopt);
		ܐܚܪܢܐ
			diff_tree_oid(&info.b_commit, &info.w_commit, "", &rev.diffopt);
		اكسر;
	}
	log_tree_diff_flush(&rev);

	ret = diff_result_code(&rev);

cleanup:
	strvec_clear(&revision_args);
	strvec_clear(&stash_args);
	free_stash_info(&info);
	release_revisions(&rev);
	ܐܢ (do_usage)
		usage_with_options(git_stash_show_usage, options);
	ܦܢܐ ret;
usage:
	do_usage = 1;
	اذهب cleanup;
}

ساكن صحيح do_store_stash(ثابت بنية object_id *w_commit, ثابت حرف *stash_msg,
			  صحيح quiet)
{
	بنية stash_info info;
	حرف revision[GIT_MAX_HEXSZ];

	oid_to_hex_r(revision, w_commit);
	assert_stash_like(&info, revision);

	ܐܢ (!stash_msg)
		stash_msg = "Created via \"git stash store\".";

	ܐܢ (refs_update_ref(get_main_ref_store(the_repository), stash_msg, ref_stash, w_commit, عدم,
			    REF_FORCE_CREATE_REFLOG,
			    quiet ? UPDATE_REFS_QUIET_ON_ERR :
			    UPDATE_REFS_MSG_ON_ERR)) {
		ܐܢ (!quiet) {
			fprintf_ln(stderr, _("Cannot update %s with %s"),
				   ref_stash, oid_to_hex(w_commit));
		}
		ܦܢܐ -1;
	}

	ܦܢܐ 0;
}

ساكن صحيح store_stash(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
		       بنية repository *repo UNUSED)
{
	صحيح quiet = 0;
	ثابت حرف *stash_msg = عدم;
	بنية object_id obj;
	بنية option options[] = {
		OPT__QUIET(&quiet, N_("be quiet")),
		OPT_STRING('m', "message", &stash_msg, "message",
			   N_("stash message")),
		OPT_END()
	};
	صحيح ret;

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_store_usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT);

	ܐܢ (argc != 1) {
		ܐܢ (!quiet)
			fprintf_ln(stderr, _("\"git stash store\" requires one "
					     "<commit> argument"));
		ܦܢܐ -1;
	}

	ܐܢ (repo_get_oid_with_flags(the_repository, argv[0], &obj,
				    quiet ? GET_OID_QUIETLY : 0)) {
		ܐܢ (!quiet)
			fprintf_ln(stderr, _("Cannot update %s with %s"),
					     ref_stash, argv[0]);
		ret = -1;
		اذهب out;
	}

	ret = do_store_stash(&obj, stash_msg, quiet);

out:
	ܦܢܐ ret;
}

ساكن فراغ add_pathspecs(بنية strvec *args,
			  ثابت بنية pathspec *ps) {
	صحيح i;

	لكل (i = 0; i < ps->nr; i++)
		strvec_push(args, ps->items[i].original);
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
	صحيح found = 0;
	بنية dir_struct dir = DIR_INIT;

	ܐܢ (include_untracked != INCLUDE_ALL_FILES)
		setup_standard_excludes(&dir);

	fill_directory(&dir, the_repository->index, ps);
	لكل (i = 0; i < dir.nr; i++) {
		بنية dir_entry *ent = dir.entries[i];
		found++;
		strbuf_addstr(untracked_files, ent->name);
		/* NUL-terminate: will be fed to update-index -z */
		strbuf_addch(untracked_files, '\0');
	}

	dir_clear(&dir);
	ܦܢܐ found;
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
	ܐܢ (repo_get_oid(the_repository, "HEAD", &dummy))
		ܦܢܐ -1;

	ܐܢ (repo_read_index(the_repository) < 0)
		ܦܢܐ -1;

	repo_init_revisions(the_repository, &rev, عدم);
	copy_pathspec(&rev.prune_data, ps);

	rev.diffopt.flags.quick = 1;
	rev.diffopt.flags.ignore_submodules = 1;
	rev.abbrev = 0;

	add_head_to_pending(&rev);
	diff_setup_done(&rev.diffopt);

	run_diff_index(&rev, DIFF_INDEX_CACHED);
	ܐܢ (diff_result_code(&rev)) {
		ret = 1;
		اذهب done;
	}

	run_diff_files(&rev, 0);
	ܐܢ (diff_result_code(&rev)) {
		ret = 1;
		اذهب done;
	}

done:
	release_revisions(&rev);
	ܦܢܐ ret;
}

/*
 * The function will fill `untracked_files` with the names of untracked files
 * It will return 1 if there were any changes and 0 if there were not.
 */
ساكن صحيح check_changes(ثابت بنية pathspec *ps, صحيح include_untracked,
			 بنية strbuf *untracked_files)
{
	صحيح ret = 0;
	ܐܢ (check_changes_tracked_files(ps))
		ret = 1;

	ܐܢ (include_untracked && get_untracked_files(ps, include_untracked,
						     untracked_files))
		ret = 1;

	ܦܢܐ ret;
}

ساكن صحيح save_untracked_files(بنية stash_info *info, بنية strbuf *msg,
				بنية strbuf *files)
{
	صحيح ret = 0;
	بنية strbuf untracked_msg = STRBUF_INIT;
	بنية child_process cp_upd_index = CHILD_PROCESS_INIT;
	بنية index_state istate = INDEX_STATE_INIT(the_repository);

	cp_upd_index.git_cmd = 1;
	strvec_pushl(&cp_upd_index.args, "update-index", "-z", "--add",
		     "--remove", "--stdin", عدم);
	strvec_pushf(&cp_upd_index.env, "GIT_INDEX_FILE=%s",
			 stash_index_path.buf);

	strbuf_addf(&untracked_msg, "untracked files on %s\n", msg->buf);
	ܐܢ (pipe_command(&cp_upd_index, files->buf, files->len, عدم, 0,
			 عدم, 0)) {
		ret = -1;
		اذهب done;
	}

	ܐܢ (write_index_as_tree(&info->u_tree, &istate, stash_index_path.buf, 0,
				عدم)) {
		ret = -1;
		اذهب done;
	}

	ܐܢ (commit_tree(untracked_msg.buf, untracked_msg.len,
			&info->u_tree, عدم, &info->u_commit, عدم, عدم)) {
		ret = -1;
		اذهب done;
	}

done:
	release_index(&istate);
	strbuf_release(&untracked_msg);
	remove_path(stash_index_path.buf);
	ܦܢܐ ret;
}

ساكن صحيح stash_staged(بنية stash_info *info, بنية strbuf *out_patch,
			صحيح quiet)
{
	صحيح ret = 0;
	بنية child_process cp_diff_tree = CHILD_PROCESS_INIT;
	بنية index_state istate = INDEX_STATE_INIT(the_repository);

	ܐܢ (write_index_as_tree(&info->w_tree, &istate, the_repository->index_file,
				0, عدم)) {
		ret = -1;
		اذهب done;
	}

	cp_diff_tree.git_cmd = 1;
	strvec_pushl(&cp_diff_tree.args, "diff-tree", "-p", "--binary",
		     "--no-color",
		     "-U1", "HEAD", oid_to_hex(&info->w_tree), "--", عدم);
	ܐܢ (pipe_command(&cp_diff_tree, عدم, 0, out_patch, 0, عدم, 0)) {
		ret = -1;
		اذهب done;
	}

	ܐܢ (!out_patch->len) {
		ܐܢ (!quiet)
			fprintf_ln(stderr, _("No staged changes"));
		ret = 1;
	}

done:
	release_index(&istate);
	ܦܢܐ ret;
}

ساكن صحيح stash_patch(بنية stash_info *info, ثابت بنية pathspec *ps,
		       بنية strbuf *out_patch, صحيح quiet,
		       بنية interactive_options *interactive_opts)
{
	صحيح ret = 0;
	بنية child_process cp_diff_tree = CHILD_PROCESS_INIT;
	بنية commit *head_commit;
	ثابت بنية object_id *head_tree;
	بنية index_state istate = INDEX_STATE_INIT(the_repository);
	حرف *old_index_env = عدم, *old_repo_index_file;

	remove_path(stash_index_path.buf);

	head_commit = lookup_commit(the_repository, &info->b_commit);
	ܐܢ (!head_commit || repo_parse_commit(the_repository, head_commit)) {
		ret = -1;
		اذهب done;
	}
	head_tree = get_commit_tree_oid(head_commit);
	ܐܢ (!head_tree) {
		ret = -1;
		اذهب done;
	}

	ܐܢ (create_index_from_tree(head_tree, stash_index_path.buf)) {
		ret = -1;
		اذهب done;
	}

	/* Find out what the user wants. */
	old_repo_index_file = the_repository->index_file;
	the_repository->index_file = stash_index_path.buf;
	old_index_env = xstrdup_or_null(getenv(INDEX_ENVIRONMENT));
	setenv(INDEX_ENVIRONMENT, the_repository->index_file, 1);

	ret = !!run_add_p(the_repository, ADD_P_STASH, interactive_opts, عدم, ps, 0);

	the_repository->index_file = old_repo_index_file;
	ܐܢ (old_index_env && *old_index_env)
		setenv(INDEX_ENVIRONMENT, old_index_env, 1);
	ܐܚܪܢܐ
		unsetenv(INDEX_ENVIRONMENT);
	FREE_AND_NULL(old_index_env);

	/* State of the working tree. */
	ܐܢ (write_index_as_tree(&info->w_tree, &istate, stash_index_path.buf, 0,
				عدم)) {
		ret = -1;
		اذهب done;
	}

	cp_diff_tree.git_cmd = 1;
	strvec_pushl(&cp_diff_tree.args, "diff-tree", "-p", "-U1", "HEAD",
		     "--no-color",
		     oid_to_hex(&info->w_tree), "--", عدم);
	ܐܢ (pipe_command(&cp_diff_tree, عدم, 0, out_patch, 0, عدم, 0)) {
		ret = -1;
		اذهب done;
	}

	ܐܢ (!out_patch->len) {
		ܐܢ (!quiet)
			fprintf_ln(stderr, _("No changes selected"));
		ret = 1;
	}

done:
	release_index(&istate);
	remove_path(stash_index_path.buf);
	ܦܢܐ ret;
}

ساكن صحيح stash_working_tree(بنية stash_info *info, ثابت بنية pathspec *ps)
{
	صحيح ret = 0;
	بنية rev_info rev;
	بنية child_process cp_upd_index = CHILD_PROCESS_INIT;
	بنية strbuf diff_output = STRBUF_INIT;
	بنية index_state istate = INDEX_STATE_INIT(the_repository);

	repo_init_revisions(the_repository, &rev, عدم);
	copy_pathspec(&rev.prune_data, ps);

	set_alternate_index_output(stash_index_path.buf);
	ܐܢ (reset_tree(&info->i_tree, 0, 0)) {
		ret = -1;
		اذهب done;
	}
	set_alternate_index_output(عدم);

	rev.diffopt.output_format = DIFF_FORMAT_CALLBACK;
	rev.diffopt.format_callback = add_diff_to_buf;
	rev.diffopt.format_callback_data = &diff_output;

	ܐܢ (repo_read_index_preload(the_repository, &rev.diffopt.pathspec, 0) < 0) {
		ret = -1;
		اذهب done;
	}

	add_pending_object(&rev, parse_object(the_repository, &info->b_commit),
			   "");
	run_diff_index(&rev, 0);

	cp_upd_index.git_cmd = 1;
	strvec_pushl(&cp_upd_index.args, "update-index",
		     "--ignore-skip-worktree-entries",
		     "-z", "--add", "--remove", "--stdin", عدم);
	strvec_pushf(&cp_upd_index.env, "GIT_INDEX_FILE=%s",
		     stash_index_path.buf);

	ܐܢ (pipe_command(&cp_upd_index, diff_output.buf, diff_output.len,
			 عدم, 0, عدم, 0)) {
		ret = -1;
		اذهب done;
	}

	ܐܢ (write_index_as_tree(&info->w_tree, &istate, stash_index_path.buf, 0,
				عدم)) {
		ret = -1;
		اذهب done;
	}

done:
	release_index(&istate);
	release_revisions(&rev);
	strbuf_release(&diff_output);
	remove_path(stash_index_path.buf);
	ܦܢܐ ret;
}

ساكن صحيح do_create_stash(ثابت بنية pathspec *ps, بنية strbuf *stash_msg_buf,
			   صحيح include_untracked, صحيح patch_mode,
			   بنية interactive_options *interactive_opts,
			   صحيح only_staged, بنية stash_info *info, بنية strbuf *patch,
			   صحيح quiet)
{
	صحيح ret = 0;
	صحيح flags = 0;
	صحيح untracked_commit_option = 0;
	ثابت حرف *head_short_sha1 = عدم;
	ثابت حرف *branch_ref = عدم;
	ثابت حرف *branch_name = "(no branch)";
	حرف *branch_name_buf = عدم;
	بنية commit *head_commit = عدم;
	بنية commit_list *parents = عدم;
	بنية strbuf msg = STRBUF_INIT;
	بنية strbuf commit_tree_label = STRBUF_INIT;
	بنية strbuf untracked_files = STRBUF_INIT;

	prepare_fallback_ident("git stash", "git@stash");

	repo_read_index_preload(the_repository, عدم, 0);
	ܐܢ (repo_refresh_and_write_index(the_repository, REFRESH_QUIET, 0, 0,
					 عدم, عدم, عدم) < 0) {
		ret = error(_("could not write index"));
		اذهب done;
	}

	ܐܢ (repo_get_oid(the_repository, "HEAD", &info->b_commit)) {
		ܐܢ (!quiet)
			fprintf_ln(stderr, _("You do not have "
					     "the initial commit yet"));
		ret = -1;
		اذهب done;
	} ܐܚܪܢܐ {
		head_commit = lookup_commit(the_repository, &info->b_commit);
	}

	ܐܢ (!check_changes(ps, include_untracked, &untracked_files)) {
		ret = 1;
		اذهب done;
	}

	branch_ref = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
					     "HEAD", 0, عدم, &flags);

	ܐܢ (flags & REF_ISSYMREF) {
		ܐܢ (skip_prefix(branch_ref, "refs/heads/", &branch_name))
			branch_name = branch_name_buf = xstrdup(branch_name);
	}

	head_short_sha1 = repo_find_unique_abbrev(the_repository,
						  &head_commit->object.oid,
						  DEFAULT_ABBREV);
	strbuf_addf(&msg, "%s: %s ", branch_name, head_short_sha1);
	pp_commit_easy(CMIT_FMT_ONELINE, head_commit, &msg);

	strbuf_addf(&commit_tree_label, "index on %s\n", msg.buf);
	commit_list_insert(head_commit, &parents);
	ܐܢ (write_index_as_tree(&info->i_tree, the_repository->index,
				repo_get_index_file(the_repository), 0, عدم) ||
	    commit_tree(commit_tree_label.buf, commit_tree_label.len,
			&info->i_tree, parents, &info->i_commit, عدم, عدم)) {
		ܐܢ (!quiet)
			fprintf_ln(stderr, _("Cannot save the current "
					     "index state"));
		ret = -1;
		اذهب done;
	}

	commit_list_free(parents);
	parents = عدم;

	ܐܢ (include_untracked) {
		ܐܢ (save_untracked_files(info, &msg, &untracked_files)) {
			ܐܢ (!quiet)
				fprintf_ln(stderr, _("Cannot save "
						     "the untracked files"));
			ret = -1;
			اذهب done;
		}
		untracked_commit_option = 1;
	}
	ܐܢ (patch_mode) {
		ret = stash_patch(info, ps, patch, quiet, interactive_opts);
		ܐܢ (ret < 0) {
			ܐܢ (!quiet)
				fprintf_ln(stderr, _("Cannot save the current "
						     "worktree state"));
			اذهب done;
		} ܐܚܪܢܐ ܐܢ (ret > 0) {
			اذهب done;
		}
	} ܐܚܪܢܐ ܐܢ (only_staged) {
		ret = stash_staged(info, patch, quiet);
		ܐܢ (ret < 0) {
			ܐܢ (!quiet)
				fprintf_ln(stderr, _("Cannot save the current "
						     "staged state"));
			اذهب done;
		} ܐܚܪܢܐ ܐܢ (ret > 0) {
			اذهب done;
		}
	} ܐܚܪܢܐ {
		ܐܢ (stash_working_tree(info, ps)) {
			ܐܢ (!quiet)
				fprintf_ln(stderr, _("Cannot save the current "
						     "worktree state"));
			ret = -1;
			اذهب done;
		}
	}

	ܐܢ (!stash_msg_buf->len)
		strbuf_addf(stash_msg_buf, "WIP on %s", msg.buf);
	ܐܚܪܢܐ
		strbuf_insertf(stash_msg_buf, 0, "On %s: ", branch_name);

	ܐܢ (untracked_commit_option)
		commit_list_insert(lookup_commit(the_repository,
						 &info->u_commit),
				   &parents);
	commit_list_insert(lookup_commit(the_repository, &info->i_commit),
			   &parents);
	commit_list_insert(head_commit, &parents);

	ܐܢ (commit_tree(stash_msg_buf->buf, stash_msg_buf->len, &info->w_tree,
			parents, &info->w_commit, عدم, عدم)) {
		ܐܢ (!quiet)
			fprintf_ln(stderr, _("Cannot record "
					     "working tree state"));
		ret = -1;
		اذهب done;
	}

done:
	strbuf_release(&commit_tree_label);
	strbuf_release(&msg);
	strbuf_release(&untracked_files);
	commit_list_free(parents);
	حرر(branch_name_buf);
	ܦܢܐ ret;
}

ساكن صحيح create_stash(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix UNUSED,
			بنية repository *repo UNUSED)
{
	صحيح ret;
	بنية strbuf stash_msg_buf = STRBUF_INIT;
	بنية stash_info info = STASH_INFO_INIT;
	بنية pathspec ps;

	/* Starting with argv[1], since argv[0] is "create" */
	strbuf_join_argv(&stash_msg_buf, argc - 1, ++argv, ' ');

	memset(&ps, 0, حجم(ps));
	ܐܢ (!check_changes_tracked_files(&ps))
		ܦܢܐ 0;

	ret = do_create_stash(&ps, &stash_msg_buf, 0, 0, عدم, 0, &info,
			      عدم, 0);
	ܐܢ (!ret)
		printf_ln("%s", oid_to_hex(&info.w_commit));

	free_stash_info(&info);
	strbuf_release(&stash_msg_buf);
	ܦܢܐ ret;
}

ساكن صحيح do_push_stash(ثابت بنية pathspec *ps, ثابت حرف *stash_msg, صحيح quiet,
			 صحيح keep_index, صحيح patch_mode,
			 بنية interactive_options *interactive_opts,
			 صحيح include_untracked, صحيح only_staged)
{
	صحيح ret = 0;
	بنية stash_info info = STASH_INFO_INIT;
	بنية strbuf patch = STRBUF_INIT;
	بنية strbuf stash_msg_buf = STRBUF_INIT;
	بنية strbuf untracked_files = STRBUF_INIT;
	بنية strbuf out = STRBUF_INIT;

	ܐܢ (patch_mode && keep_index == -1)
		keep_index = 1;

	ܐܢ (patch_mode && include_untracked) {
		fprintf_ln(stderr, _("Can't use --patch and --include-untracked"
				     " or --all at the same time"));
		ret = -1;
		اذهب done;
	}

	/* --patch overrides --staged */
	ܐܢ (patch_mode)
		only_staged = 0;

	ܐܢ (only_staged && include_untracked) {
		fprintf_ln(stderr, _("Can't use --staged and --include-untracked"
				     " or --all at the same time"));
		ret = -1;
		اذهب done;
	}

	repo_read_index_preload(the_repository, عدم, 0);
	ܐܢ (!include_untracked && ps->nr) {
		حرف *ps_matched = xcalloc(ps->nr, 1);

		ܐܢ (pathspec_needs_expanded_index(the_repository->index, ps))
			ensure_full_index(the_repository->index);
		لكل (size_t i = 0; i < the_repository->index->cache_nr; i++)
			ce_path_match(the_repository->index, the_repository->index->cache[i], ps,
				      ps_matched);

		ܐܢ (report_path_error(ps_matched, ps)) {
			fprintf_ln(stderr, _("Did you forget to 'git add'?"));
			ret = -1;
			حرر(ps_matched);
			اذهب done;
		}
		حرر(ps_matched);
	}

	ܐܢ (repo_refresh_and_write_index(the_repository, REFRESH_QUIET, 0, 0,
					 عدم, عدم, عدم)) {
		ret = error(_("could not write index"));
		اذهب done;
	}

	ܐܢ (!check_changes(ps, include_untracked, &untracked_files)) {
		ܐܢ (!quiet)
			printf_ln(_("No local changes to save"));
		اذهب done;
	}

	ܐܢ (!refs_reflog_exists(get_main_ref_store(the_repository), ref_stash) && do_clear_stash()) {
		ret = -1;
		ܐܢ (!quiet)
			fprintf_ln(stderr, _("Cannot initialize stash"));
		اذهب done;
	}

	ܐܢ (stash_msg)
		strbuf_addstr(&stash_msg_buf, stash_msg);
	ܐܢ (do_create_stash(ps, &stash_msg_buf, include_untracked, patch_mode,
			    interactive_opts, only_staged, &info, &patch, quiet)) {
		ret = -1;
		اذهب done;
	}

	ܐܢ (do_store_stash(&info.w_commit, stash_msg_buf.buf, 1)) {
		ret = -1;
		ܐܢ (!quiet)
			fprintf_ln(stderr, _("Cannot save the current status"));
		اذهب done;
	}

	ܐܢ (!quiet)
		printf_ln(_("Saved working directory and index state %s"),
			  stash_msg_buf.buf);

	ܐܢ (!(patch_mode || only_staged)) {
		ܐܢ (include_untracked && !ps->nr) {
			بنية child_process cp = CHILD_PROCESS_INIT;

			cp.git_cmd = 1;
			ܐܢ (startup_info->original_cwd) {
				cp.dir = startup_info->original_cwd;
				strvec_pushf(&cp.env, "%s=%s",
					     GIT_WORK_TREE_ENVIRONMENT,
					     the_repository->worktree);
			}
			strvec_pushl(&cp.args, "clean", "--force",
				     "--quiet", "-d", ":/", عدم);
			ܐܢ (include_untracked == INCLUDE_ALL_FILES)
				strvec_push(&cp.args, "-x");
			ܐܢ (run_command(&cp)) {
				ret = -1;
				اذهب done;
			}
		}
		discard_index(the_repository->index);
		ܐܢ (ps->nr) {
			بنية child_process cp_add = CHILD_PROCESS_INIT;
			بنية child_process cp_diff = CHILD_PROCESS_INIT;
			بنية child_process cp_apply = CHILD_PROCESS_INIT;

			cp_add.git_cmd = 1;
			strvec_push(&cp_add.args, "add");
			ܐܢ (!include_untracked)
				strvec_push(&cp_add.args, "-u");
			ܐܢ (include_untracked == INCLUDE_ALL_FILES)
				strvec_push(&cp_add.args, "--force");
			strvec_push(&cp_add.args, "--");
			add_pathspecs(&cp_add.args, ps);
			ܐܢ (run_command(&cp_add)) {
				ret = -1;
				اذهب done;
			}

			cp_diff.git_cmd = 1;
			strvec_pushl(&cp_diff.args, "diff-index", "-p",
				     "--no-color",
				     "--cached", "--binary", "HEAD", "--",
				     عدم);
			add_pathspecs(&cp_diff.args, ps);
			ܐܢ (pipe_command(&cp_diff, عدم, 0, &out, 0, عدم, 0)) {
				ret = -1;
				اذهب done;
			}

			cp_apply.git_cmd = 1;
			strvec_pushl(&cp_apply.args, "apply", "--index",
				     "-R", عدم);
			ܐܢ (pipe_command(&cp_apply, out.buf, out.len, عدم, 0,
					 عدم, 0)) {
				ret = -1;
				اذهب done;
			}
		} ܐܚܪܢܐ {
			بنية child_process cp = CHILD_PROCESS_INIT;
			cp.git_cmd = 1;
			/* BUG: this nukes untracked files in the way */
			strvec_pushl(&cp.args, "reset", "--hard", "-q",
				     "--no-recurse-submodules", عدم);
			ܐܢ (run_command(&cp)) {
				ret = -1;
				اذهب done;
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
		ܐܢ (keep_index == 1 && !is_null_oid(&info.i_tree) &&
		    !is_empty_tree_oid(&info.i_tree, the_repository->hash_algo)) {
			بنية child_process cp = CHILD_PROCESS_INIT;

			cp.git_cmd = 1;
			strvec_pushl(&cp.args, "checkout", "--no-overlay",
				     oid_to_hex(&info.i_tree), "--", عدم);
			ܐܢ (!ps->nr)
				strvec_push(&cp.args, ":/");
			ܐܚܪܢܐ
				add_pathspecs(&cp.args, ps);
			ܐܢ (run_command(&cp)) {
				ret = -1;
				اذهب done;
			}
		}
		اذهب done;
	} ܐܚܪܢܐ {
		بنية child_process cp = CHILD_PROCESS_INIT;

		cp.git_cmd = 1;
		strvec_pushl(&cp.args, "apply", "-R", عدم);

		ܐܢ (pipe_command(&cp, patch.buf, patch.len, عدم, 0, عدم, 0)) {
			ܐܢ (!quiet)
				fprintf_ln(stderr, _("Cannot remove "
						     "worktree changes"));
			ret = -1;
			اذهب done;
		}

		ܐܢ (keep_index < 1) {
			بنية child_process cp = CHILD_PROCESS_INIT;

			cp.git_cmd = 1;
			strvec_pushl(&cp.args, "reset", "-q", "--refresh", "--",
				     عدم);
			add_pathspecs(&cp.args, ps);
			ܐܢ (run_command(&cp)) {
				ret = -1;
				اذهب done;
			}
		}
		اذهب done;
	}

done:
	strbuf_release(&patch);
	strbuf_release(&out);
	free_stash_info(&info);
	strbuf_release(&stash_msg_buf);
	strbuf_release(&untracked_files);
	ܦܢܐ ret;
}

ساكن صحيح push_stash(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
		      صحيح push_assumed)
{
	صحيح force_assume = 0;
	صحيح keep_index = -1;
	صحيح only_staged = 0;
	صحيح patch_mode = 0;
	صحيح include_untracked = 0;
	صحيح quiet = 0;
	صحيح pathspec_file_nul = 0;
	ثابت حرف *stash_msg = عدم;
	حرف *pathspec_from_file = عدم;
	بنية pathspec ps;
	بنية interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	بنية option options[] = {
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
		OPT_STRING('m', "message", &stash_msg, N_("message"),
			   N_("stash message")),
		OPT_PATHSPEC_FROM_FILE(&pathspec_from_file),
		OPT_PATHSPEC_FILE_NUL(&pathspec_file_nul),
		OPT_END()
	};
	صحيح ret;

	ܐܢ (argc) {
		صحيح flags = PARSE_OPT_KEEP_DASHDASH;

		ܐܢ (push_assumed)
			flags |= PARSE_OPT_STOP_AT_NON_OPTION;

		argc = parse_options(argc, argv, prefix, options,
				     push_assumed ? git_stash_usage :
				     git_stash_push_usage, flags);
		force_assume |= patch_mode;
	}

	ܐܢ (argc) {
		ܐܢ (!strcmp(argv[0], "--")) {
			argc--;
			argv++;
		} ܐܚܪܢܐ ܐܢ (push_assumed && !force_assume) {
			die("subcommand wasn't specified; 'push' can't be assumed due to unexpected token '%s'",
			    argv[0]);
		}
	}

	parse_pathspec(&ps, 0, PATHSPEC_PREFER_FULL | PATHSPEC_PREFIX_ORIGIN,
		       prefix, argv);

	ܐܢ (pathspec_from_file) {
		ܐܢ (patch_mode)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--patch");

		ܐܢ (only_staged)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--staged");

		ܐܢ (ps.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&ps, 0,
				    PATHSPEC_PREFER_FULL | PATHSPEC_PREFIX_ORIGIN,
				    prefix, pathspec_from_file, pathspec_file_nul);
	} ܐܚܪܢܐ ܐܢ (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	ܐܢ (!patch_mode) {
		ܐܢ (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--patch");
		ܐܢ (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--patch");
		ܐܢ (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--patch");
	}

	ܐܢ (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	ܐܢ (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	ret = do_push_stash(&ps, stash_msg, quiet, keep_index, patch_mode,
			    &interactive_opts, include_untracked, only_staged);

	clear_pathspec(&ps);
	حرر(pathspec_from_file);
	ܦܢܐ ret;
}

ساكن صحيح push_stash_unassumed(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
				بنية repository *repo UNUSED)
{
	ܦܢܐ push_stash(argc, argv, prefix, 0);
}

ساكن صحيح save_stash(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
		      بنية repository *repo UNUSED)
{
	صحيح keep_index = -1;
	صحيح only_staged = 0;
	صحيح patch_mode = 0;
	صحيح include_untracked = 0;
	صحيح quiet = 0;
	صحيح ret = 0;
	ثابت حرف *stash_msg = عدم;
	بنية pathspec ps;
	بنية strbuf stash_msg_buf = STRBUF_INIT;
	بنية interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	بنية option options[] = {
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
		OPT_STRING('m', "message", &stash_msg, "message",
			   N_("stash message")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_save_usage,
			     PARSE_OPT_KEEP_DASHDASH);

	ܐܢ (argc)
		stash_msg = strbuf_join_argv(&stash_msg_buf, argc, argv, ' ');

	memset(&ps, 0, حجم(ps));

	ܐܢ (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	ܐܢ (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	ܐܢ (!patch_mode) {
		ܐܢ (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--patch");
		ܐܢ (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--patch");
		ܐܢ (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--patch");
	}

	ret = do_push_stash(&ps, stash_msg, quiet, keep_index,
			    patch_mode, &interactive_opts, include_untracked,
			    only_staged);

	strbuf_release(&stash_msg_buf);
	ܦܢܐ ret;
}

ساكن صحيح write_commit_with_parents(بنية repository *r,
				     بنية object_id *out,
				     ثابت بنية object_id *oid,
				     بنية commit_list *parents)
{
	size_t author_len, committer_len;
	بنية commit *this;
	ثابت حرف *orig_author, *orig_committer;
	حرف *author = عدم, *committer = عدم;
	ثابت حرف *buffer;
	unsigned long bufsize;
	ثابت حرف *p;
	بنية strbuf msg = STRBUF_INIT;
	صحيح ret = 0;
	بنية ident_split id;

	this = lookup_commit_reference(r, oid);
	buffer = repo_get_commit_buffer(r, this, &bufsize);
	orig_author = find_commit_header(buffer, "author", &author_len);
	orig_committer = find_commit_header(buffer, "committer", &committer_len);

	ܐܢ (!orig_author || !orig_committer) {
		ret = error(_("cannot parse commit %s"), oid_to_hex(oid));
		اذهب out;
	}

	ܐܢ (split_ident_line(&id, orig_author, author_len) < 0 ||
	    split_ident_line(&id, orig_committer, committer_len) < 0) {
		ret = error(_("invalid author or committer for %s"), oid_to_hex(oid));
		اذهب out;
	}

	p = strstr(buffer, "\n\n");
	strbuf_addstr(&msg, "git stash: ");

	ܐܢ (p)
		strbuf_add(&msg, p + 2, bufsize - (p + 2 - buffer));
	strbuf_complete_line(&msg);

	author = xmemdupz(orig_author, author_len);
	committer = xmemdupz(orig_committer, committer_len);

	ܐܢ (commit_tree_extended(msg.buf, msg.len,
				 r->hash_algo->empty_tree, parents,
				 out, author, committer,
				 عدم, عدم)) {
		ret = error(_("could not write commit"));
		اذهب out;
	}
out:
	strbuf_release(&msg);
	repo_unuse_commit_buffer(r, this, buffer);
	حرر(author);
	حرر(committer);
	ܦܢܐ ret;
}

ساكن صحيح do_import_stash(بنية repository *r, ثابت حرف *rev)
{
	بنية object_id chain;
	صحيح res = 0;
	ثابت حرف *buffer = عدم;
	unsigned long bufsize;
	بنية commit *this = عدم;
	بنية commit_list *items = عدم, *cur;
	حرف *msg = عدم;

	ܐܢ (repo_get_oid(r, rev, &chain))
		ܦܢܐ error(_("not a valid revision: %s"), rev);

	this = lookup_commit_reference(r, &chain);
	ܐܢ (!this)
		ܦܢܐ error(_("not a commit: %s"), rev);

	/*
	 * Walk the commit history, finding each stash entry, and load data into
	 * the array.
	 */
	لكل (;;) {
		ثابت حرف *author, *committer;
		size_t author_len, committer_len;
		ثابت حرف *p;
		ثابت حرف *expected = "git stash <git@stash> 1000684800 +0000";
		ثابت حرف *prefix = "git stash: ";
		بنية commit *stash;
		بنية tree *tree = repo_get_commit_tree(r, this);

		ܐܢ (!tree ||
		    !oideq(&tree->object.oid, r->hash_algo->empty_tree) ||
		    (this->parents &&
		     (!this->parents->next || this->parents->next->next))) {
			res = error(_("%s is not a valid exported stash commit"),
					oid_to_hex(&this->object.oid));
			اذهب out;
		}

		buffer = repo_get_commit_buffer(r, this, &bufsize);

		ܐܢ (!this->parents) {
			/*
			 * We don't have any parents.  Make sure this is our
			 * root commit.
			 */
			author = find_commit_header(buffer, "author", &author_len);
			committer = find_commit_header(buffer, "committer", &committer_len);

			ܐܢ (!author || !committer) {
				error(_("cannot parse commit %s"), oid_to_hex(&this->object.oid));
				اذهب out;
			}

			ܐܢ (author_len != strlen(expected) ||
			    committer_len != strlen(expected) ||
			    memcmp(author, expected, author_len) ||
			    memcmp(committer, expected, committer_len)) {
				res = error(_("found root commit %s with invalid data"), oid_to_hex(&this->object.oid));
				اذهب out;
			}
			اكسر;
		}

		p = strstr(buffer, "\n\n");
		ܐܢ (!p) {
			res = error(_("cannot parse commit %s"), oid_to_hex(&this->object.oid));
			اذهب out;
		}

		p += 2;
		ܐܢ (((size_t)(bufsize - (p - buffer)) < strlen(prefix)) ||
		    memcmp(prefix, p, strlen(prefix))) {
			res = error(_("found stash commit %s without expected prefix"), oid_to_hex(&this->object.oid));
			اذهب out;
		}

		stash = this->parents->next->item;

		ܐܢ (repo_parse_commit(r, this->parents->item) ||
		    repo_parse_commit(r, stash)) {
			res = error(_("cannot parse parents of commit: %s"),
					oid_to_hex(&this->object.oid));
			اذهب out;
		}

		ܐܢ (check_stash_topology(r, stash)) {
			res = error(_("%s does not look like a stash commit"),
					oid_to_hex(&stash->object.oid));
			اذهب out;
		}

		repo_unuse_commit_buffer(r, this, buffer);
		buffer = عدم;
		items = commit_list_insert(stash, &items);
		this = this->parents->item;
	}

	/*
	 * Now, walk each entry, adding it to the stash as a normal stash
	 * commit.
	 */
	لكل (cur = items; cur; cur = cur->next) {
		ثابت حرف *p;
		بنية object_id *oid;

		this = cur->item;
		oid = &this->object.oid;
		buffer = repo_get_commit_buffer(r, this, &bufsize);
		ܐܢ (!buffer) {
			res = error(_("cannot read commit buffer for %s"), oid_to_hex(oid));
			اذهب out;
		}

		p = strstr(buffer, "\n\n");
		ܐܢ (!p) {
			res = error(_("cannot parse commit %s"), oid_to_hex(oid));
			اذهب out;
		}

		p += 2;
		msg = xmemdupz(p, bufsize - (p - buffer));
		repo_unuse_commit_buffer(r, this, buffer);
		buffer = عدم;

		ܐܢ (do_store_stash(oid, msg, 1)) {
			res = error(_("cannot save the stash for %s"), oid_to_hex(oid));
			اذهب out;
		}
		FREE_AND_NULL(msg);
	}
out:
	ܐܢ (this && buffer)
		repo_unuse_commit_buffer(r, this, buffer);
	commit_list_free(items);
	حرر(msg);

	ܦܢܐ res;
}

ساكن صحيح import_stash(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
			بنية repository *repo)
{
	بنية option options[] = {
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_import_usage,
			     PARSE_OPT_KEEP_DASHDASH);

	ܐܢ (argc != 1)
		usage_msg_opt("a revision is required", git_stash_import_usage, options);

	ܦܢܐ do_import_stash(repo, argv[0]);
}

بنية stash_entry_data {
	بنية repository *r;
	بنية commit_list **items;
	size_t count;
};

ساكن صحيح collect_stash_entries(ثابت حرف *refname UNUSED,
				 بنية object_id *old_oid UNUSED,
				 بنية object_id *new_oid,
				 ثابت حرف *committer UNUSED,
				 timestamp_t timestamp UNUSED,
				 صحيح tz UNUSED, ثابت حرف *msg UNUSED,
				 فراغ *cb_data)
{
	بنية stash_entry_data *data = cb_data;
	بنية commit *stash;

	data->count++;
	stash = lookup_commit_reference(data->r, new_oid);
	ܐܢ (!stash || check_stash_topology(data->r, stash)) {
		ܦܢܐ error(_("%s does not look like a stash commit"),
			     oid_to_hex(new_oid));
	}
	data->items = commit_list_append(stash, data->items);
	ܦܢܐ 0;
}

ساكن صحيح do_export_stash(بنية repository *r,
			   ثابت حرف *ref,
			   صحيح argc,
			   ثابت حرف **argv)
{
	بنية object_id base;
	بنية commit *prev;
	بنية commit_list *items = عدم, **iter = &items, *cur;
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
	ܐܢ (commit_tree_extended("", 0, r->hash_algo->empty_tree, عدم,
				 &base, author, committer, عدم, عدم))
		ܦܢܐ error(_("unable to write base commit"));

	prev = lookup_commit_reference(r, &base);

	ܐܢ (argc) {
		/*
		 * Find each specified stash, and load data into the array.
		 */
		لكل (i = 0; i < argc; i++) {
			بنية object_id oid;
			بنية commit *stash;

			ܐܢ (parse_stash_revision(&revision, argv[i], 1) ||
			    repo_get_oid_with_flags(r, revision.buf, &oid,
						    GET_OID_QUIETLY |
						    GET_OID_GENTLY)) {
				res = error(_("unable to find stash entry %s"), argv[i]);
				اذهب out;
			}

			stash = lookup_commit_reference(r, &oid);
			ܐܢ (!stash || check_stash_topology(r, stash)) {
				res = error(_("%s does not look like a stash commit"),
						revision.buf);
				اذهب out;
			}
			iter = commit_list_append(stash, iter);
		}
	} ܐܚܪܢܐ {
		/*
		 * Walk the reflog, finding each stash entry, and load data into the
		 * array.
		 */
		بنية stash_entry_data cb_data = {
			.r = r, .items = iter,
		};
		ܐܢ (refs_for_each_reflog_ent_reverse(get_main_ref_store(r),
						     "refs/stash",
						     collect_stash_entries,
						     &cb_data) && cb_data.count)
			اذهب out;
	}

	/*
	 * Now, create a set of commits identical to the regular stash commits,
	 * but where their first parents form a chain to our original empty
	 * base commit.
	 */
	items = commit_list_reverse(items);
	لكل (cur = items; cur; cur = cur->next) {
		بنية commit_list *parents = عدم;
		بنية commit_list **next = &parents;
		بنية object_id out;
		بنية commit *stash = cur->item;

		next = commit_list_append(prev, next);
		next = commit_list_append(stash, next);
		res = write_commit_with_parents(r, &out, &stash->object.oid, parents);
		commit_list_free(parents);
		ܐܢ (res)
			اذهب out;
		prev = lookup_commit_reference(r, &out);
	}
	ܐܢ (ref)
		refs_update_ref(get_main_ref_store(r), عدم, ref,
				&prev->object.oid, عدم, 0, UPDATE_REFS_DIE_ON_ERR);
	ܐܚܪܢܐ
		puts(oid_to_hex(&prev->object.oid));
out:
	strbuf_release(&revision);
	commit_list_free(items);

	ܦܢܐ res;
}

تعداد export_action {
	ACTION_NONE,
	ACTION_PRINT,
	ACTION_TO_REF,
};

ساكن صحيح export_stash(صحيح argc,
			ثابت حرف **argv,
			ثابت حرف *prefix,
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
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_export_usage,
			     PARSE_OPT_KEEP_DASHDASH);

	ܐܢ (ref && action == ACTION_NONE)
		action = ACTION_TO_REF;

	ܐܢ (action == ACTION_NONE || (ref && action == ACTION_PRINT))
		ܦܢܐ error(_("exactly one of --print and --to-ref is required"));

	ܦܢܐ do_export_stash(repo, ref, argc, argv);
}

صحيح cmd_stash(صحيح argc,
	      ثابت حرف **argv,
	      ثابت حرف *prefix,
	      بنية repository *repo)
{
	pid_t pid = getpid();
	ثابت حرف *index_file;
	بنية strvec args = STRVEC_INIT;
	parse_opt_subcommand_fn *fn = عدم;
	بنية option options[] = {
		OPT_SUBCOMMAND("apply", &fn, apply_stash),
		OPT_SUBCOMMAND("clear", &fn, clear_stash),
		OPT_SUBCOMMAND("drop", &fn, drop_stash),
		OPT_SUBCOMMAND("pop", &fn, pop_stash),
		OPT_SUBCOMMAND("branch", &fn, branch_stash),
		OPT_SUBCOMMAND("list", &fn, list_stash),
		OPT_SUBCOMMAND("show", &fn, show_stash),
		OPT_SUBCOMMAND("store", &fn, store_stash),
		OPT_SUBCOMMAND("create", &fn, create_stash),
		OPT_SUBCOMMAND("push", &fn, push_stash_unassumed),
		OPT_SUBCOMMAND("export", &fn, export_stash),
		OPT_SUBCOMMAND("import", &fn, import_stash),
		OPT_SUBCOMMAND_F("save", &fn, save_stash, عدم, PARSE_OPT_NOCOMPLETE),
		OPT_END()
	};
	ثابت حرف **args_copy;
	صحيح ret;

	repo_config(the_repository, git_stash_config, عدم);

	argc = parse_options(argc, argv, prefix, options, git_stash_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL |
			     PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	index_file = repo_get_index_file(the_repository);
	strbuf_addf(&stash_index_path, "%s.stash.%" PRIuMAX, index_file,
		    (uintmax_t)pid);

	ܐܢ (fn) {
		ret = fn(argc, argv, prefix, repo);

		/*
		 * The subcommand implementations return 0 on success, a
		 * negative value on failure, and STASH_APPLY_CONFLICT
		 * when applying a stash entry resulted in conflicts.
		 * Map failures to 128, the status die() uses, so that
		 * exit status 1 unambiguously indicates conflicts.
		 */
		ܐܢ (ret < 0)
			ܦܢܐ 128;
		ܦܢܐ ret;
	} ܐܚܪܢܐ ܐܢ (!argc) {
		ܦܢܐ !!push_stash_unassumed(0, عدم, prefix, repo);
	}

	/* Assume 'stash push' */
	strvec_push(&args, "push");
	strvec_pushv(&args, argv);

	/*
	 * `push_stash()` ends up modifying the array, which causes memory
	 * leaks if we didn't copy the array here.
	 */
	DUP_ARRAY(args_copy, args.v, args.nr);

	ret = !!push_stash(args.nr, args_copy, prefix, 1);

	strvec_clear(&args);
	حرر(args_copy);
	ܦܢܐ ret;
}
