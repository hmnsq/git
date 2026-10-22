#define USE_THE_REPOSITORY_VARIABLE

#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "abspath.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "hash.h"
#அந்தர்பாவ "hex.h"
#அந்தர்பாவ "object-name.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "refs.h"
#அந்தர்பாவ "stash.h"
#அந்தர்பாவ "lockfile.h"
#அந்தர்பாவ "cache-tree.h"
#அந்தர்பாவ "unpack-trees.h"
#அந்தர்பாவ "merge-ort-wrappers.h"
#அந்தர்பாவ "strvec.h"
#அந்தர்பாவ "run-command.h"
#அந்தர்பாவ "dir.h"
#அந்தர்பாவ "entry.h"
#அந்தர்பாவ "preload-index.h"
#அந்தர்பாவ "read-cache.h"
#அந்தர்பாவ "repository.h"
#அந்தர்பாவ "rerere.h"
#அந்தர்பாவ "revision.h"
#அந்தர்பாவ "setup.h"
#அந்தர்பாவ "sparse-index.h"
#அந்தர்பாவ "log-tree.h"
#அந்தர்பாவ "diffcore.h"
#அந்தர்பாவ "reflog.h"
#அந்தர்பாவ "reflog-walk.h"
#அந்தர்பாவ "add-interactive.h"
#அந்தர்பாவ "commit.h"

#define INCLUDE_ALL_FILES 2

#define BUILTIN_STASH_LIST_USAGE \
	N_("git stash list [<log-options>]")
#define BUILTIN_STASH_SHOW_USAGE \
	N_("git stash show [-u | --include-untracked | --only-untracked] [<diff-options>] [<stash>]")
#define BUILTIN_STASH_DROP_USAGE \
	N_("git stash drop [-q | --quiet] [<stash>]")
#define BUILTIN_STASH_எடு_USAGE \
	N_("git stash pop [--index] [-q | --quiet] [<stash>]")
#define BUILTIN_STASH_APPLY_USAGE \
	N_("git stash apply [--index] [-q | --quiet] [--label-ours=<label>] [--label-theirs=<label>] [--label-base=<label>] [<stash>]")
#define BUILTIN_STASH_BRANCH_USAGE \
	N_("git stash branch <branchname> [<stash>]")
#define BUILTIN_STASH_STORE_USAGE \
	N_("git stash store [(-m | --message) <message>] [-q | --quiet] <commit>")
#define BUILTIN_STASH_தள்ளு_USAGE \
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

நிலையான மாறிலி எழுத்து * மாறிலி git_stash_usage[] = {
	BUILTIN_STASH_LIST_USAGE,
	BUILTIN_STASH_SHOW_USAGE,
	BUILTIN_STASH_DROP_USAGE,
	BUILTIN_STASH_எடு_USAGE,
	BUILTIN_STASH_APPLY_USAGE,
	BUILTIN_STASH_BRANCH_USAGE,
	BUILTIN_STASH_தள்ளு_USAGE,
	BUILTIN_STASH_SAVE_USAGE,
	BUILTIN_STASH_CLEAR_USAGE,
	BUILTIN_STASH_CREATE_USAGE,
	BUILTIN_STASH_STORE_USAGE,
	BUILTIN_STASH_EXPORT_USAGE,
	BUILTIN_STASH_IMPORT_USAGE,
	வெற்று
};

நிலையான மாறிலி எழுத்து * மாறிலி git_stash_list_usage[] = {
	BUILTIN_STASH_LIST_USAGE,
	வெற்று
};

நிலையான மாறிலி எழுத்து * மாறிலி git_stash_show_usage[] = {
	BUILTIN_STASH_SHOW_USAGE,
	வெற்று
};

நிலையான மாறிலி எழுத்து * மாறிலி git_stash_drop_usage[] = {
	BUILTIN_STASH_DROP_USAGE,
	வெற்று
};

நிலையான மாறிலி எழுத்து * மாறிலி git_stash_எடு_usage[] = {
	BUILTIN_STASH_எடு_USAGE,
	வெற்று
};

நிலையான மாறிலி எழுத்து * மாறிலி git_stash_apply_usage[] = {
	BUILTIN_STASH_APPLY_USAGE,
	வெற்று
};

நிலையான மாறிலி எழுத்து * மாறிலி git_stash_branch_usage[] = {
	BUILTIN_STASH_BRANCH_USAGE,
	வெற்று
};

நிலையான மாறிலி எழுத்து * மாறிலி git_stash_clear_usage[] = {
	BUILTIN_STASH_CLEAR_USAGE,
	வெற்று
};

நிலையான மாறிலி எழுத்து * மாறிலி git_stash_store_usage[] = {
	BUILTIN_STASH_STORE_USAGE,
	வெற்று
};

நிலையான மாறிலி எழுத்து * மாறிலி git_stash_தள்ளு_usage[] = {
	BUILTIN_STASH_தள்ளு_USAGE,
	வெற்று
};

நிலையான மாறிலி எழுத்து * மாறிலி git_stash_save_usage[] = {
	BUILTIN_STASH_SAVE_USAGE,
	வெற்று
};

நிலையான மாறிலி எழுத்து * மாறிலி git_stash_export_usage[] = {
	BUILTIN_STASH_EXPORT_USAGE,
	வெற்று
};

நிலையான மாறிலி எழுத்து * மாறிலி git_stash_import_usage[] = {
	BUILTIN_STASH_IMPORT_USAGE,
	வெற்று
};

நிலையான மாறிலி எழுத்து ref_stash[] = "refs/stash";
நிலையான கட்டமைப்பு strbuf stash_index_path = STRBUF_INIT;

நிலையான முழு show_stat = 1;
நிலையான முழு show_patch;
நிலையான முழு show_include_untracked;
நிலையான முழு use_index;

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
கட்டமைப்பு stash_info {
	கட்டமைப்பு object_id w_commit;
	கட்டமைப்பு object_id b_commit;
	கட்டமைப்பு object_id i_commit;
	கட்டமைப்பு object_id u_commit;
	கட்டமைப்பு object_id c_commit;
	கட்டமைப்பு object_id w_மரம்;
	கட்டமைப்பு object_id b_மரம்;
	கட்டமைப்பு object_id i_மரம்;
	கட்டமைப்பு object_id u_மரம்;
	கட்டமைப்பு strbuf revision;
	முழு is_stash_ref;
	முழு has_u;
};

#define STASH_INFO_INIT { \
	.revision = STRBUF_INIT, \
}

நிலையான வெறுமை free_stash_info(கட்டமைப்பு stash_info *info)
{
	strbuf_release(&info->revision);
}

நிலையான முழு check_stash_topology(கட்டமைப்பு repository *r, கட்டமைப்பு commit *stash)
{
	கட்டமைப்பு commit *p1, *p2, *p3 = வெற்று;

	/* stash must have two or three parents */
	எனில் (!stash->parents || !stash->parents->next ||
			(stash->parents->next->next && stash->parents->next->next->next))
		திரும்பு -1;
	p1 = stash->parents->item;
	p2 = stash->parents->next->item;
	எனில் (stash->parents->next->next)
		p3 = stash->parents->next->next->item;
	எனில் (repo_parse_commit(r, p1) || repo_parse_commit(r, p2) ||
			(p3 && repo_parse_commit(r, p3)))
		திரும்பு -1;
	/* p2 must have a single parent, p3 must have no parents */
	எனில் (!p2->parents || p2->parents->next || (p3 && p3->parents))
		திரும்பு -1;
	எனில் (repo_parse_commit(r, p2->parents->item))
		திரும்பு -1;
	/* p2^1 must equal p1 */
	எனில் (!oideq(&p1->object.oid, &p2->parents->item->object.oid))
		திரும்பு -1;

	திரும்பு 0;
}

நிலையான வெறுமை assert_stash_like(கட்டமைப்பு stash_info *info, மாறிலி எழுத்து *revision)
{
	எனில் (get_oidf(&info->b_commit, "%s^1", revision) ||
	    get_oidf(&info->w_மரம், "%s:", revision) ||
	    get_oidf(&info->b_மரம், "%s^1:", revision) ||
	    get_oidf(&info->i_மரம், "%s^2:", revision))
		die(_("'%s' is not a stash-like commit"), revision);
}

நிலையான முழு parse_stash_revision(கட்டமைப்பு strbuf *revision, மாறிலி எழுத்து *commit, முழு quiet)
{
	strbuf_reset(revision);
	எனில் (!commit) {
		எனில் (!refs_ref_exists(get_main_ref_store(the_repository), ref_stash)) {
			எனில் (!quiet)
				fprintf_ln(stderr, _("No stash entries found."));
			திரும்பு -1;
		}

		strbuf_addf(revision, "%s@{0}", ref_stash);
	} இல்லையெனில் எனில் (strspn(commit, "0123456789") == strlen(commit)) {
		strbuf_addf(revision, "%s@{%s}", ref_stash, commit);
	} இல்லையெனில் {
		strbuf_addstr(revision, commit);
	}
	திரும்பு 0;
}

நிலையான முழு get_stash_info(கட்டமைப்பு stash_info *info, முழு argc, மாறிலி எழுத்து **argv)
{
	முழு ret;
	எழுத்து *end_of_rev;
	எழுத்து *expanded_ref;
	மாறிலி எழுத்து *revision;
	மாறிலி எழுத்து *commit = வெற்று;
	கட்டமைப்பு object_id dummy;
	கட்டமைப்பு strbuf symbolic = STRBUF_INIT;

	எனில் (argc > 1) {
		முழு i;
		கட்டமைப்பு strbuf refs_செய்தி = STRBUF_INIT;

		க்கு (i = 0; i < argc; i++)
			strbuf_addf(&refs_செய்தி, " '%s'", argv[i]);

		fprintf_ln(stderr, _("Too many revisions specified:%s"),
			   refs_செய்தி.buf);
		strbuf_release(&refs_செய்தி);

		திரும்பு -1;
	}

	எனில் (argc == 1)
		commit = argv[0];

	strbuf_init(&info->revision, 0);
	எனில் (parse_stash_revision(&info->revision, commit, 0)) {
		திரும்பு -1;
	}

	revision = info->revision.buf;

	எனில் (repo_get_oid(the_repository, revision, &info->w_commit))
		திரும்பு error(_("%s is not a valid reference"), revision);

	assert_stash_like(info, revision);

	info->has_u = !get_oidf(&info->u_மரம், "%s^3:", revision);

	end_of_rev = strchrnul(revision, '@');
	strbuf_add(&symbolic, revision, end_of_rev - revision);

	ret = repo_dwim_ref(the_repository, symbolic.buf, symbolic.len,
			    &dummy, &expanded_ref, 0);
	strbuf_release(&symbolic);
	தேர்வு (ret) {
	நிலை 0: /* Not found, but valid ref */
		info->is_stash_ref = 0;
		நிறுத்து;
	நிலை 1:
		info->is_stash_ref = !strcmp(expanded_ref, ref_stash);
		நிறுத்து;
	இயல்பு: /* Invalid or ambiguous */
		நிறுத்து;
	}

	விடுவி(expanded_ref);
	திரும்பு !(ret == 0 || ret == 1);
}

நிலையான முழு do_clear_stash(வெறுமை)
{
	கட்டமைப்பு object_id obj;
	எனில் (repo_get_oid(the_repository, ref_stash, &obj))
		திரும்பு 0;

	திரும்பு refs_delete_ref(get_main_ref_store(the_repository), வெற்று,
			       ref_stash, &obj, 0);
}

நிலையான முழு clear_stash(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
		       கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு option options[] = {
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_clear_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	எனில் (argc)
		திரும்பு error(_("git stash clear with arguments is "
			       "unimplemented"));

	திரும்பு do_clear_stash();
}

நிலையான முழு reset_மரம்(கட்டமைப்பு object_id *i_மரம், முழு update, முழு reset)
{
	முழு nr_trees = 1;
	கட்டமைப்பு unpack_trees_options opts;
	கட்டமைப்பு மரம்_desc t[MAX_UNPACK_TREES];
	கட்டமைப்பு மரம் *மரம்;
	கட்டமைப்பு lock_file lock_file = LOCK_INIT;

	repo_read_index_preload(the_repository, வெற்று, 0);
	எனில் (refresh_index(the_repository->index, REFRESH_QUIET, வெற்று, வெற்று, வெற்று))
		திரும்பு -1;

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);

	memset(&opts, 0, அளவு(opts));

	மரம் = repo_parse_மரம்_indirect(the_repository, i_மரம்);
	எனில் (repo_parse_மரம்(the_repository, மரம்))
		திரும்பு -1;

	init_மரம்_desc(t, &மரம்->object.oid, மரம்->buffer, மரம்->size);

	opts.head_idx = 1;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;
	opts.merge = 1;
	opts.reset = reset ? UNPACK_RESET_PROTECT_UNTRACKED : 0;
	opts.update = update;
	எனில் (update)
		opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
	opts.fn = oneway_merge;

	எனில் (unpack_trees(nr_trees, t, &opts))
		திரும்பு -1;

	எனில் (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
		திரும்பு error(_("unable to write new index file"));

	திரும்பு 0;
}

நிலையான முழு create_index_from_மரம்(மாறிலி கட்டமைப்பு object_id *மரம்_id,
				  மாறிலி எழுத்து *index_path)
{
	முழு nr_trees = 1;
	முழு ret = 0;
	கட்டமைப்பு unpack_trees_options opts;
	கட்டமைப்பு மரம்_desc t[MAX_UNPACK_TREES];
	கட்டமைப்பு மரம் *மரம்;
	கட்டமைப்பு index_state dst_istate = INDEX_STATE_INIT(the_repository);
	கட்டமைப்பு lock_file lock_file = LOCK_INIT;

	repo_read_index_preload(the_repository, வெற்று, 0);
	refresh_index(the_repository->index, REFRESH_QUIET, வெற்று, வெற்று, வெற்று);

	hold_lock_file_for_update(&lock_file, index_path, LOCK_DIE_ON_ERROR);

	memset(&opts, 0, அளவு(opts));

	மரம் = repo_parse_மரம்_indirect(the_repository, மரம்_id);
	எனில் (!மரம் || repo_parse_மரம்(the_repository, மரம்)) {
		ret = -1;
		செல் done;
	}

	init_மரம்_desc(t, &மரம்->object.oid, மரம்->buffer, மரம்->size);

	opts.head_idx = 1;
	opts.src_index = the_repository->index;
	opts.dst_index = &dst_istate;
	opts.merge = 1;
	opts.reset = UNPACK_RESET_PROTECT_UNTRACKED;
	opts.fn = oneway_merge;

	எனில் (unpack_trees(nr_trees, t, &opts)) {
		ret = -1;
		செல் done;
	}

	எனில் (write_locked_index(&dst_istate, &lock_file, COMMIT_LOCK)) {
		ret = error(_("unable to write new index file"));
		செல் done;
	}

done:
	release_index(&dst_istate);
	எனில் (ret)
		rollback_lock_file(&lock_file);
	திரும்பு ret;
}

நிலையான முழு is_path_a_directory(மாறிலி எழுத்து *path)
{
	/*
	 * This function differs from abspath.c:is_directory() in that
	 * here we use lstat() instead of stat(); we do not want to
	 * follow symbolic links here.
	 */
	கட்டமைப்பு stat st;
	திரும்பு (!lstat(path, &st) && S_ISDIR(st.st_mode));
}

நிலையான வெறுமை add_diff_to_buf(கட்டமைப்பு diff_queue_struct *q,
			    கட்டமைப்பு diff_options *options UNUSED,
			    வெறுமை *data)
{
	முழு i;

	க்கு (i = 0; i < q->nr; i++) {
		எனில் (is_path_a_directory(q->queue[i]->one->path))
			தொடர்;

		strbuf_addstr(data, q->queue[i]->one->path);

		/* NUL-terminate: will be fed to update-index -z */
		strbuf_addch(data, '\0');
	}
}

நிலையான முழு restore_untracked(கட்டமைப்பு object_id *u_மரம்)
{
	முழு res;
	கட்டமைப்பு child_process cp = CHILD_PROCESS_INIT;

	/*
	 * We need to run restore files from a given index, but without
	 * affecting the current index, so we use GIT_INDEX_FILE with
	 * run_command to fork processes that will not interfere.
	 */
	cp.git_cmd = 1;
	strvec_தள்ளு(&cp.args, "read-tree");
	strvec_தள்ளு(&cp.args, oid_to_hex(u_மரம்));
	strvec_pushf(&cp.env, "GIT_INDEX_FILE=%s",
		     stash_index_path.buf);
	எனில் (run_command(&cp)) {
		remove_path(stash_index_path.buf);
		திரும்பு -1;
	}

	child_process_init(&cp);
	cp.git_cmd = 1;
	strvec_pushl(&cp.args, "checkout-index", "--all", வெற்று);
	strvec_pushf(&cp.env, "GIT_INDEX_FILE=%s",
		     stash_index_path.buf);

	res = run_command(&cp);
	remove_path(stash_index_path.buf);
	திரும்பு res;
}

நிலையான வெறுமை unstage_changes_unless_new(கட்டமைப்பு object_id *orig_மரம்)
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

	கட்டமைப்பு checkout state = CHECKOUT_INIT;
	கட்டமைப்பு diff_options diff_opts;
	கட்டமைப்பு lock_file lock = LOCK_INIT;
	முழு i;

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

	do_diff_cache(orig_மரம், &diff_opts);
	diffcore_std(&diff_opts);

	/* Iterate over the paths that changed due to the merge... */
	க்கு (i = 0; i < diff_queued_diff.nr; i++) {
		கட்டமைப்பு diff_filepair *p;
		கட்டமைப்பு cache_entry *ce;
		முழு pos;

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
		எனில் (pos >= 0 && ce_skip_worktree(the_repository->index->cache[pos])) {
			கட்டமைப்பு stat st;

			ce = the_repository->index->cache[pos];
			எனில் (!lstat(ce->name, &st)) {
				/* Conflicting path present; relocate it */
				கட்டமைப்பு strbuf new_path = STRBUF_INIT;
				முழு fd;

				strbuf_addf(&new_path,
					    "%s.stash.XXXXXX", ce->name);
				fd = xmkstemp(new_path.buf);
				close(fd);
				printf(_("WARNING: Untracked file in way of "
					 "tracked file!  Renaming\n "
					 "           %s -> %s\n"
					 "         to make room.\n"),
				       ce->name, new_path.buf);
				எனில் (rename(ce->name, new_path.buf))
					die("Failed to move %s to %s",
					    ce->name, new_path.buf);
				strbuf_release(&new_path);
			}
			checkout_entry(ce, &state, வெற்று, வெற்று);
			ce->ce_flags &= ~CE_SKIP_WORKTREE;
		}

		/*
		 * Step 3: "unstage" changes, as long as they are still tracked
		 */
		எனில் (p->one->oid_valid) {
			/*
			 * Path existed in orig_tree; restore index entry
			 * from that tree in order to "unstage" the changes.
			 */
			முழு option = ADD_CACHE_OK_TO_REPLACE;
			எனில் (pos < 0)
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
	எனில் (write_locked_index(the_repository->index, &lock,
			       COMMIT_LOCK | SKIP_IF_UNCHANGED))
		die(_("could not write index"));
}

நிலையான எண்ணிக்கை stash_apply_result do_apply_stash(மாறிலி எழுத்து *prefix,
					      கட்டமைப்பு stash_info *info,
					      முழு index, முழு quiet,
					      மாறிலி எழுத்து *label_ours,
					      மாறிலி எழுத்து *label_theirs,
					      மாறிலி எழுத்து *label_base)
{
	முழு clean, ret;
	முழு has_index = index;
	கட்டமைப்பு merge_options o;
	கட்டமைப்பு object_id c_மரம்;
	கட்டமைப்பு object_id index_மரம்;
	கட்டமைப்பு மரம் *head, *merge, *merge_base;
	கட்டமைப்பு lock_file lock = LOCK_INIT;

	repo_read_index_preload(the_repository, வெற்று, 0);
	எனில் (repo_refresh_and_write_index(the_repository, REFRESH_QUIET, 0, 0,
					 வெற்று, வெற்று, வெற்று))
		திரும்பு error(_("could not write index"));

	எனில் (write_index_as_மரம்(&c_மரம், the_repository->index,
				repo_get_index_file(the_repository), 0, வெற்று))
		திரும்பு error(_("cannot apply a stash in the middle of a merge"));

	init_ui_merge_options(&o, the_repository);

	எனில் (quiet)
		o.verbosity = 0;

	எனில் (index) {
		எனில் (oideq(&info->b_மரம், &info->i_மரம்) ||
		    oideq(&c_மரம், &info->i_மரம்)) {
			has_index = 0;
		} இல்லையெனில் {
			கட்டமைப்பு merge_result result = { 0 };

			o.branch1 = "Current index";
			o.branch2 = "Stashed index changes";
			o.ancestor = "Stash base";

			head = lookup_மரம்(o.repo, &c_மரம்);
			merge = lookup_மரம்(o.repo, &info->i_மரம்);
			merge_base = lookup_மரம்(o.repo, &info->b_மரம்);

			merge_incore_nonrecursive(&o, merge_base, head, merge,
						  &result);

			எனில் (result.clean < 0) {
				merge_finalize(&o, &result);
				திரும்பு error(_("index merge failed"));
			} இல்லையெனில் எனில் (!result.clean) {
				merge_finalize(&o, &result);
				திரும்பு error(_("conflicts in index. "
					       "Try without --index."));
			} இல்லையெனில் {
				oidcpy(&index_மரம், &result.மரம்->object.oid);
				merge_finalize(&o, &result);
			}
		}
	}

	o.branch1 = label_ours ? label_ours : "Updated upstream";
	o.branch2 = label_theirs ? label_theirs : "Stashed changes";
	o.ancestor = label_base ? label_base : "Stash base";

	எனில் (oideq(&info->b_மரம், &c_மரம்))
		o.branch1 = "Version stash was based on";

	எனில் (o.verbosity >= 3)
		printf_ln(_("Merging %s with %s"), o.branch1, o.branch2);

	head = lookup_மரம்(o.repo, &c_மரம்);
	merge = lookup_மரம்(o.repo, &info->w_மரம்);
	merge_base = lookup_மரம்(o.repo, &info->b_மரம்);

	repo_hold_locked_index(o.repo, &lock, LOCK_DIE_ON_ERROR);
	clean = merge_ort_nonrecursive(&o, head, merge, merge_base);

	/*
	 * If 'clean' >= 0, reverse the value for 'ret' so 'ret' is 0 when the
	 * merge was clean, and 1 if the merge was unclean or a negative value
	 * if it encountered an error.
	 */
	ret = clean >= 0 ? !clean : clean;

	எனில் (ret < 0)
		rollback_lock_file(&lock);
	இல்லையெனில் எனில் (write_locked_index(o.repo->index, &lock,
				      COMMIT_LOCK | SKIP_IF_UNCHANGED))
		ret = error(_("could not write index"));

	எனில் (ret) {
		repo_rerere(the_repository, 0);

		எனில் (index)
			fprintf_ln(stderr, _("Index was not unstashed."));

		செல் restore_untracked;
	}

	எனில் (has_index) {
		எனில் (reset_மரம்(&index_மரம், 0, 0))
			ret = -1;
	} இல்லையெனில் {
		unstage_changes_unless_new(&c_மரம்);
	}

restore_untracked:
	எனில் (info->has_u && restore_untracked(&info->u_மரம்))
		ret = error(_("could not restore untracked files from stash"));

	எனில் (!quiet) {
		கட்டமைப்பு child_process cp = CHILD_PROCESS_INIT;

		/*
		 * Status is quite simple and could be replaced with calls to
		 * wt_status in the future, but it adds complexities which may
		 * require more tests.
		 */
		cp.git_cmd = 1;
		cp.dir = prefix;
		strvec_pushf(&cp.env, GIT_WORK_மரம்_ENVIRONMENT"=%s",
			     absolute_path(repo_get_work_மரம்(the_repository)));
		strvec_pushf(&cp.env, GIT_DIR_ENVIRONMENT"=%s",
			     absolute_path(repo_get_git_dir(the_repository)));
		strvec_தள்ளு(&cp.args, "status");
		run_command(&cp);
	}

	திரும்பு ret;
}

நிலையான முழு apply_stash(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
		       கட்டமைப்பு repository *repo UNUSED)
{
	முழு ret = -1;
	முழு quiet = 0;
	முழு index = use_index;
	மாறிலி எழுத்து *label_ours = வெற்று, *label_theirs = வெற்று, *label_base = வெற்று;
	கட்டமைப்பு stash_info info = STASH_INFO_INIT;
	கட்டமைப்பு option options[] = {
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

	எனில் (get_stash_info(&info, argc, argv))
		செல் cleanup;

	ret = do_apply_stash(prefix, &info, index, quiet,
			     label_ours, label_theirs, label_base);
cleanup:
	free_stash_info(&info);
	திரும்பு ret;
}

நிலையான முழு reject_reflog_ent(மாறிலி எழுத்து *refname UNUSED,
			     கட்டமைப்பு object_id *ooid UNUSED,
			     கட்டமைப்பு object_id *noid UNUSED,
			     மாறிலி எழுத்து *email UNUSED,
			     timestamp_t timestamp UNUSED,
			     முழு tz UNUSED, மாறிலி எழுத்து *message UNUSED,
			     வெறுமை *cb_data UNUSED)
{
	திரும்பு 1;
}

நிலையான முழு reflog_is_empty(மாறிலி எழுத்து *refname)
{
	திரும்பு !refs_for_each_reflog_ent(get_main_ref_store(the_repository),
					 refname, reject_reflog_ent, வெற்று);
}

நிலையான முழு do_drop_stash(கட்டமைப்பு stash_info *info, முழு quiet)
{
	எனில் (!reflog_delete(info->revision.buf,
			   EXPIRE_REFLOGS_REWRITE | EXPIRE_REFLOGS_UPDATE_REF,
			   0)) {
		எனில் (!quiet)
			printf_ln(_("Dropped %s (%s)"), info->revision.buf,
				  oid_to_hex(&info->w_commit));
	} இல்லையெனில் {
		திரும்பு error(_("%s: Could not drop stash entry"),
			     info->revision.buf);
	}

	எனில் (reflog_is_empty(ref_stash))
		do_clear_stash();

	திரும்பு 0;
}

நிலையான முழு get_stash_info_assert(கட்டமைப்பு stash_info *info, முழு argc,
				 மாறிலி எழுத்து **argv)
{
	முழு ret = get_stash_info(info, argc, argv);

	எனில் (ret < 0)
		திரும்பு ret;

	எனில் (!info->is_stash_ref)
		திரும்பு error(_("'%s' is not a stash reference"), info->revision.buf);

	திரும்பு 0;
}

நிலையான முழு drop_stash(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
		      கட்டமைப்பு repository *repo UNUSED)
{
	முழு ret = -1;
	முழு quiet = 0;
	கட்டமைப்பு stash_info info = STASH_INFO_INIT;
	கட்டமைப்பு option options[] = {
		OPT__QUIET(&quiet, N_("be quiet, only report errors")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_drop_usage, 0);

	எனில் (get_stash_info_assert(&info, argc, argv))
		செல் cleanup;

	ret = do_drop_stash(&info, quiet);
cleanup:
	free_stash_info(&info);
	திரும்பு ret;
}

நிலையான முழு எடு_stash(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
		     கட்டமைப்பு repository *repo UNUSED)
{
	முழு ret = -1;
	முழு index = use_index;
	முழு quiet = 0;
	கட்டமைப்பு stash_info info = STASH_INFO_INIT;
	கட்டமைப்பு option options[] = {
		OPT__QUIET(&quiet, N_("be quiet, only report errors")),
		OPT_BOOL(0, "index", &index,
			 N_("attempt to recreate the index")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_எடு_usage, 0);

	எனில் (get_stash_info_assert(&info, argc, argv))
		செல் cleanup;

	எனில் ((ret = do_apply_stash(prefix, &info, index, quiet,
				  வெற்று, வெற்று, வெற்று)))
		printf_ln(_("The stash entry is kept in case "
			    "you need it again."));
	இல்லையெனில்
		ret = do_drop_stash(&info, quiet);

cleanup:
	free_stash_info(&info);
	திரும்பு ret;
}

நிலையான முழு branch_stash(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
			கட்டமைப்பு repository *repo UNUSED)
{
	முழு ret = -1;
	மாறிலி எழுத்து *branch = வெற்று;
	கட்டமைப்பு stash_info info = STASH_INFO_INIT;
	கட்டமைப்பு child_process cp = CHILD_PROCESS_INIT;
	கட்டமைப்பு option options[] = {
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_branch_usage, 0);

	எனில் (!argc) {
		fprintf_ln(stderr, _("No branch name specified"));
		திரும்பு -1;
	}

	branch = argv[0];

	எனில் (get_stash_info(&info, argc - 1, argv + 1))
		செல் cleanup;

	cp.git_cmd = 1;
	strvec_pushl(&cp.args, "checkout", "-b", வெற்று);
	strvec_தள்ளு(&cp.args, branch);
	strvec_தள்ளு(&cp.args, oid_to_hex(&info.b_commit));
	ret = run_command(&cp);
	எனில் (!ret)
		ret = do_apply_stash(prefix, &info, 1, 0,
				     வெற்று, வெற்று, வெற்று);
	எனில் (!ret && info.is_stash_ref)
		ret = do_drop_stash(&info, 0);

cleanup:
	free_stash_info(&info);
	திரும்பு ret;
}

நிலையான முழு list_stash(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
		      கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு child_process cp = CHILD_PROCESS_INIT;
	கட்டமைப்பு option options[] = {
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_list_usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT);

	எனில் (!refs_ref_exists(get_main_ref_store(the_repository), ref_stash))
		திரும்பு 0;

	cp.git_cmd = 1;
	strvec_pushl(&cp.args, "log", "--format=%gd: %gs", "-g",
		     "--first-parent", வெற்று);
	strvec_pushv(&cp.args, argv);
	strvec_தள்ளு(&cp.args, ref_stash);
	strvec_தள்ளு(&cp.args, "--");
	திரும்பு run_command(&cp);
}

நிலையான முழு git_stash_config(மாறிலி எழுத்து *var, மாறிலி எழுத்து *மதிப்பு,
			    மாறிலி கட்டமைப்பு config_context *ctx, வெறுமை *cb)
{
	எனில் (!strcmp(var, "stash.showstat")) {
		show_stat = git_config_bool(var, மதிப்பு);
		திரும்பு 0;
	}
	எனில் (!strcmp(var, "stash.showpatch")) {
		show_patch = git_config_bool(var, மதிப்பு);
		திரும்பு 0;
	}
	எனில் (!strcmp(var, "stash.showincludeuntracked")) {
		show_include_untracked = git_config_bool(var, மதிப்பு);
		திரும்பு 0;
	}
	எனில் (!strcmp(var, "stash.index")) {
		use_index = git_config_bool(var, மதிப்பு);
		திரும்பு 0;
	}
	திரும்பு git_diff_basic_config(var, மதிப்பு, ctx, cb);
}

நிலையான வெறுமை diff_include_untracked(மாறிலி கட்டமைப்பு stash_info *info, கட்டமைப்பு diff_options *diff_opt)
{
	மாறிலி கட்டமைப்பு object_id *oid[] = { &info->w_commit, &info->u_மரம் };
	கட்டமைப்பு மரம் *மரம்[ARRAY_SIZE(oid)];
	கட்டமைப்பு மரம்_desc மரம்_desc[ARRAY_SIZE(oid)];
	கட்டமைப்பு unpack_trees_options unpack_மரம்_opt = { 0 };

	க்கு (size_t i = 0; i < ARRAY_SIZE(oid); i++) {
		மரம்[i] = repo_parse_மரம்_indirect(the_repository, oid[i]);
		எனில் (repo_parse_மரம்(the_repository, மரம்[i]) < 0)
			die(_("failed to parse tree"));
		init_மரம்_desc(&மரம்_desc[i], &மரம்[i]->object.oid,
			       மரம்[i]->buffer, மரம்[i]->size);
	}

	unpack_மரம்_opt.head_idx = -1;
	unpack_மரம்_opt.src_index = the_repository->index;
	unpack_மரம்_opt.dst_index = the_repository->index;
	unpack_மரம்_opt.merge = 1;
	unpack_மரம்_opt.fn = stash_worktree_untracked_merge;

	எனில் (unpack_trees(ARRAY_SIZE(மரம்_desc), மரம்_desc, &unpack_மரம்_opt))
		die(_("failed to unpack trees"));

	do_diff_cache(&info->b_commit, diff_opt);
}

நிலையான முழு show_stash(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
		      கட்டமைப்பு repository *repo UNUSED)
{
	முழு i;
	முழு ret = -1;
	கட்டமைப்பு stash_info info = STASH_INFO_INIT;
	கட்டமைப்பு rev_info rev;
	கட்டமைப்பு strvec stash_args = STRVEC_INIT;
	கட்டமைப்பு strvec revision_args = STRVEC_INIT;
	எண்ணிக்கை {
		UNTRACKED_NONE,
		UNTRACKED_INCLUDE,
		UNTRACKED_ONLY
	} show_untracked = show_include_untracked ? UNTRACKED_INCLUDE : UNTRACKED_NONE;
	கட்டமைப்பு option options[] = {
		OPT_SET_INT('u', "include-untracked", &show_untracked,
			    N_("include untracked files in the stash"),
			    UNTRACKED_INCLUDE),
		OPT_SET_INT_F(0, "only-untracked", &show_untracked,
			      N_("only show untracked files in the stash"),
			      UNTRACKED_ONLY, PARSE_OPT_NONEG),
		OPT_END()
	};
	முழு do_usage = 0;

	init_diff_ui_defaults();
	repo_config(the_repository, git_diff_ui_config, வெற்று);
	repo_init_revisions(the_repository, &rev, prefix);

	argc = parse_options(argc, argv, prefix, options, git_stash_show_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	strvec_தள்ளு(&revision_args, argv[0]);
	க்கு (i = 1; i < argc; i++) {
		எனில் (argv[i][0] != '-')
			strvec_தள்ளு(&stash_args, argv[i]);
		இல்லையெனில்
			strvec_தள்ளு(&revision_args, argv[i]);
	}

	எனில் (get_stash_info(&info, stash_args.nr, stash_args.v))
		செல் cleanup;

	/*
	 * The config settings are applied only if there are not passed
	 * any options.
	 */
	எனில் (revision_args.nr == 1) {
		எனில் (show_stat)
			rev.diffopt.output_format = DIFF_FORMAT_DIFFSTAT;

		எனில் (show_patch)
			rev.diffopt.output_format |= DIFF_FORMAT_PATCH;

		எனில் (!show_stat && !show_patch) {
			ret = 0;
			செல் cleanup;
		}
	}

	setup_revisions_from_strvec(&revision_args, &rev, வெற்று);
	எனில் (revision_args.nr > 1)
		செல் usage;
	எனில் (!rev.diffopt.output_format) {
		rev.diffopt.output_format = DIFF_FORMAT_PATCH;
		diff_setup_done(&rev.diffopt);
	}

	rev.diffopt.flags.recursive = 1;
	setup_diff_pager(&rev.diffopt);
	தேர்வு (show_untracked) {
	நிலை UNTRACKED_NONE:
		diff_மரம்_oid(&info.b_commit, &info.w_commit, "", &rev.diffopt);
		நிறுத்து;
	நிலை UNTRACKED_ONLY:
		எனில் (info.has_u)
			diff_வேர்_மரம்_oid(&info.u_மரம், "", &rev.diffopt);
		நிறுத்து;
	நிலை UNTRACKED_INCLUDE:
		எனில் (info.has_u)
			diff_include_untracked(&info, &rev.diffopt);
		இல்லையெனில்
			diff_மரம்_oid(&info.b_commit, &info.w_commit, "", &rev.diffopt);
		நிறுத்து;
	}
	log_மரம்_diff_flush(&rev);

	ret = diff_result_code(&rev);

cleanup:
	strvec_clear(&revision_args);
	strvec_clear(&stash_args);
	free_stash_info(&info);
	release_revisions(&rev);
	எனில் (do_usage)
		usage_with_options(git_stash_show_usage, options);
	திரும்பு ret;
usage:
	do_usage = 1;
	செல் cleanup;
}

நிலையான முழு do_store_stash(மாறிலி கட்டமைப்பு object_id *w_commit, மாறிலி எழுத்து *stash_செய்தி,
			  முழு quiet)
{
	கட்டமைப்பு stash_info info;
	எழுத்து revision[GIT_MAX_HEXSZ];

	oid_to_hex_r(revision, w_commit);
	assert_stash_like(&info, revision);

	எனில் (!stash_செய்தி)
		stash_செய்தி = "Created via \"git stash store\".";

	எனில் (refs_update_ref(get_main_ref_store(the_repository), stash_செய்தி, ref_stash, w_commit, வெற்று,
			    REF_FORCE_CREATE_REFLOG,
			    quiet ? UPDATE_REFS_QUIET_ON_ERR :
			    UPDATE_REFS_செய்தி_ON_ERR)) {
		எனில் (!quiet) {
			fprintf_ln(stderr, _("Cannot update %s with %s"),
				   ref_stash, oid_to_hex(w_commit));
		}
		திரும்பு -1;
	}

	திரும்பு 0;
}

நிலையான முழு store_stash(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
		       கட்டமைப்பு repository *repo UNUSED)
{
	முழு quiet = 0;
	மாறிலி எழுத்து *stash_செய்தி = வெற்று;
	கட்டமைப்பு object_id obj;
	கட்டமைப்பு option options[] = {
		OPT__QUIET(&quiet, N_("be quiet")),
		OPT_STRING('m', "message", &stash_செய்தி, "message",
			   N_("stash message")),
		OPT_END()
	};
	முழு ret;

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_store_usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT);

	எனில் (argc != 1) {
		எனில் (!quiet)
			fprintf_ln(stderr, _("\"git stash store\" requires one "
					     "<commit> argument"));
		திரும்பு -1;
	}

	எனில் (repo_get_oid_with_flags(the_repository, argv[0], &obj,
				    quiet ? GET_OID_QUIETLY : 0)) {
		எனில் (!quiet)
			fprintf_ln(stderr, _("Cannot update %s with %s"),
					     ref_stash, argv[0]);
		ret = -1;
		செல் out;
	}

	ret = do_store_stash(&obj, stash_செய்தி, quiet);

out:
	திரும்பு ret;
}

நிலையான வெறுமை add_pathspecs(கட்டமைப்பு strvec *args,
			  மாறிலி கட்டமைப்பு pathspec *ps) {
	முழு i;

	க்கு (i = 0; i < ps->nr; i++)
		strvec_தள்ளு(args, ps->பொருட்கள்[i].original);
}

/*
 * `untracked_files` will be filled with the names of untracked files.
 * The return value is:
 *
 * = 0 if there are not any untracked files
 * > 0 if there are untracked files
 */
நிலையான முழு get_untracked_files(மாறிலி கட்டமைப்பு pathspec *ps, முழு include_untracked,
			       கட்டமைப்பு strbuf *untracked_files)
{
	முழு i;
	முழு found = 0;
	கட்டமைப்பு dir_struct dir = DIR_INIT;

	எனில் (include_untracked != INCLUDE_ALL_FILES)
		setup_standard_excludes(&dir);

	fill_directory(&dir, the_repository->index, ps);
	க்கு (i = 0; i < dir.nr; i++) {
		கட்டமைப்பு dir_entry *ent = dir.entries[i];
		found++;
		strbuf_addstr(untracked_files, ent->name);
		/* NUL-terminate: will be fed to update-index -z */
		strbuf_addch(untracked_files, '\0');
	}

	dir_clear(&dir);
	திரும்பு found;
}

/*
 * The return value of `check_changes_tracked_files()` can be:
 *
 * < 0 if there was an error
 * = 0 if there are no changes.
 * > 0 if there are changes.
 */
நிலையான முழு check_changes_tracked_files(மாறிலி கட்டமைப்பு pathspec *ps)
{
	கட்டமைப்பு rev_info rev;
	கட்டமைப்பு object_id dummy;
	முழு ret = 0;

	/* No initial commit. */
	எனில் (repo_get_oid(the_repository, "HEAD", &dummy))
		திரும்பு -1;

	எனில் (repo_read_index(the_repository) < 0)
		திரும்பு -1;

	repo_init_revisions(the_repository, &rev, வெற்று);
	copy_pathspec(&rev.prune_data, ps);

	rev.diffopt.flags.quick = 1;
	rev.diffopt.flags.ignore_submodules = 1;
	rev.abbrev = 0;

	add_head_to_pending(&rev);
	diff_setup_done(&rev.diffopt);

	run_diff_index(&rev, DIFF_INDEX_CACHED);
	எனில் (diff_result_code(&rev)) {
		ret = 1;
		செல் done;
	}

	run_diff_files(&rev, 0);
	எனில் (diff_result_code(&rev)) {
		ret = 1;
		செல் done;
	}

done:
	release_revisions(&rev);
	திரும்பு ret;
}

/*
 * The function will fill `untracked_files` with the names of untracked files
 * It will return 1 if there were any changes and 0 if there were not.
 */
நிலையான முழு check_changes(மாறிலி கட்டமைப்பு pathspec *ps, முழு include_untracked,
			 கட்டமைப்பு strbuf *untracked_files)
{
	முழு ret = 0;
	எனில் (check_changes_tracked_files(ps))
		ret = 1;

	எனில் (include_untracked && get_untracked_files(ps, include_untracked,
						     untracked_files))
		ret = 1;

	திரும்பு ret;
}

நிலையான முழு save_untracked_files(கட்டமைப்பு stash_info *info, கட்டமைப்பு strbuf *செய்தி,
				கட்டமைப்பு strbuf *files)
{
	முழு ret = 0;
	கட்டமைப்பு strbuf untracked_செய்தி = STRBUF_INIT;
	கட்டமைப்பு child_process cp_upd_index = CHILD_PROCESS_INIT;
	கட்டமைப்பு index_state istate = INDEX_STATE_INIT(the_repository);

	cp_upd_index.git_cmd = 1;
	strvec_pushl(&cp_upd_index.args, "update-index", "-z", "--add",
		     "--remove", "--stdin", வெற்று);
	strvec_pushf(&cp_upd_index.env, "GIT_INDEX_FILE=%s",
			 stash_index_path.buf);

	strbuf_addf(&untracked_செய்தி, "untracked files on %s\n", செய்தி->buf);
	எனில் (pipe_command(&cp_upd_index, files->buf, files->len, வெற்று, 0,
			 வெற்று, 0)) {
		ret = -1;
		செல் done;
	}

	எனில் (write_index_as_மரம்(&info->u_மரம், &istate, stash_index_path.buf, 0,
				வெற்று)) {
		ret = -1;
		செல் done;
	}

	எனில் (commit_மரம்(untracked_செய்தி.buf, untracked_செய்தி.len,
			&info->u_மரம், வெற்று, &info->u_commit, வெற்று, வெற்று)) {
		ret = -1;
		செல் done;
	}

done:
	release_index(&istate);
	strbuf_release(&untracked_செய்தி);
	remove_path(stash_index_path.buf);
	திரும்பு ret;
}

நிலையான முழு stash_staged(கட்டமைப்பு stash_info *info, கட்டமைப்பு strbuf *out_patch,
			முழு quiet)
{
	முழு ret = 0;
	கட்டமைப்பு child_process cp_diff_மரம் = CHILD_PROCESS_INIT;
	கட்டமைப்பு index_state istate = INDEX_STATE_INIT(the_repository);

	எனில் (write_index_as_மரம்(&info->w_மரம், &istate, the_repository->index_file,
				0, வெற்று)) {
		ret = -1;
		செல் done;
	}

	cp_diff_மரம்.git_cmd = 1;
	strvec_pushl(&cp_diff_மரம்.args, "diff-tree", "-p", "--binary",
		     "--no-color",
		     "-U1", "HEAD", oid_to_hex(&info->w_மரம்), "--", வெற்று);
	எனில் (pipe_command(&cp_diff_மரம், வெற்று, 0, out_patch, 0, வெற்று, 0)) {
		ret = -1;
		செல் done;
	}

	எனில் (!out_patch->len) {
		எனில் (!quiet)
			fprintf_ln(stderr, _("No staged changes"));
		ret = 1;
	}

done:
	release_index(&istate);
	திரும்பு ret;
}

நிலையான முழு stash_patch(கட்டமைப்பு stash_info *info, மாறிலி கட்டமைப்பு pathspec *ps,
		       கட்டமைப்பு strbuf *out_patch, முழு quiet,
		       கட்டமைப்பு interactive_options *interactive_opts)
{
	முழு ret = 0;
	கட்டமைப்பு child_process cp_diff_மரம் = CHILD_PROCESS_INIT;
	கட்டமைப்பு commit *head_commit;
	மாறிலி கட்டமைப்பு object_id *head_மரம்;
	கட்டமைப்பு index_state istate = INDEX_STATE_INIT(the_repository);
	எழுத்து *old_index_env = வெற்று, *old_repo_index_file;

	remove_path(stash_index_path.buf);

	head_commit = lookup_commit(the_repository, &info->b_commit);
	எனில் (!head_commit || repo_parse_commit(the_repository, head_commit)) {
		ret = -1;
		செல் done;
	}
	head_மரம் = get_commit_மரம்_oid(head_commit);
	எனில் (!head_மரம்) {
		ret = -1;
		செல் done;
	}

	எனில் (create_index_from_மரம்(head_மரம், stash_index_path.buf)) {
		ret = -1;
		செல் done;
	}

	/* Find out what the user wants. */
	old_repo_index_file = the_repository->index_file;
	the_repository->index_file = stash_index_path.buf;
	old_index_env = xstrdup_or_null(getenv(INDEX_ENVIRONMENT));
	setenv(INDEX_ENVIRONMENT, the_repository->index_file, 1);

	ret = !!run_add_p(the_repository, ADD_P_STASH, interactive_opts, வெற்று, ps, 0);

	the_repository->index_file = old_repo_index_file;
	எனில் (old_index_env && *old_index_env)
		setenv(INDEX_ENVIRONMENT, old_index_env, 1);
	இல்லையெனில்
		unsetenv(INDEX_ENVIRONMENT);
	FREE_AND_NULL(old_index_env);

	/* State of the working tree. */
	எனில் (write_index_as_மரம்(&info->w_மரம், &istate, stash_index_path.buf, 0,
				வெற்று)) {
		ret = -1;
		செல் done;
	}

	cp_diff_மரம்.git_cmd = 1;
	strvec_pushl(&cp_diff_மரம்.args, "diff-tree", "-p", "-U1", "HEAD",
		     "--no-color",
		     oid_to_hex(&info->w_மரம்), "--", வெற்று);
	எனில் (pipe_command(&cp_diff_மரம், வெற்று, 0, out_patch, 0, வெற்று, 0)) {
		ret = -1;
		செல் done;
	}

	எனில் (!out_patch->len) {
		எனில் (!quiet)
			fprintf_ln(stderr, _("No changes selected"));
		ret = 1;
	}

done:
	release_index(&istate);
	remove_path(stash_index_path.buf);
	திரும்பு ret;
}

நிலையான முழு stash_working_மரம்(கட்டமைப்பு stash_info *info, மாறிலி கட்டமைப்பு pathspec *ps)
{
	முழு ret = 0;
	கட்டமைப்பு rev_info rev;
	கட்டமைப்பு child_process cp_upd_index = CHILD_PROCESS_INIT;
	கட்டமைப்பு strbuf diff_output = STRBUF_INIT;
	கட்டமைப்பு index_state istate = INDEX_STATE_INIT(the_repository);

	repo_init_revisions(the_repository, &rev, வெற்று);
	copy_pathspec(&rev.prune_data, ps);

	set_alternate_index_output(stash_index_path.buf);
	எனில் (reset_மரம்(&info->i_மரம், 0, 0)) {
		ret = -1;
		செல் done;
	}
	set_alternate_index_output(வெற்று);

	rev.diffopt.output_format = DIFF_FORMAT_CALLBACK;
	rev.diffopt.format_callback = add_diff_to_buf;
	rev.diffopt.format_callback_data = &diff_output;

	எனில் (repo_read_index_preload(the_repository, &rev.diffopt.pathspec, 0) < 0) {
		ret = -1;
		செல் done;
	}

	add_pending_object(&rev, parse_object(the_repository, &info->b_commit),
			   "");
	run_diff_index(&rev, 0);

	cp_upd_index.git_cmd = 1;
	strvec_pushl(&cp_upd_index.args, "update-index",
		     "--ignore-skip-worktree-entries",
		     "-z", "--add", "--remove", "--stdin", வெற்று);
	strvec_pushf(&cp_upd_index.env, "GIT_INDEX_FILE=%s",
		     stash_index_path.buf);

	எனில் (pipe_command(&cp_upd_index, diff_output.buf, diff_output.len,
			 வெற்று, 0, வெற்று, 0)) {
		ret = -1;
		செல் done;
	}

	எனில் (write_index_as_மரம்(&info->w_மரம், &istate, stash_index_path.buf, 0,
				வெற்று)) {
		ret = -1;
		செல் done;
	}

done:
	release_index(&istate);
	release_revisions(&rev);
	strbuf_release(&diff_output);
	remove_path(stash_index_path.buf);
	திரும்பு ret;
}

நிலையான முழு do_create_stash(மாறிலி கட்டமைப்பு pathspec *ps, கட்டமைப்பு strbuf *stash_செய்தி_buf,
			   முழு include_untracked, முழு patch_mode,
			   கட்டமைப்பு interactive_options *interactive_opts,
			   முழு only_staged, கட்டமைப்பு stash_info *info, கட்டமைப்பு strbuf *patch,
			   முழு quiet)
{
	முழு ret = 0;
	முழு flags = 0;
	முழு untracked_commit_option = 0;
	மாறிலி எழுத்து *head_short_sha1 = வெற்று;
	மாறிலி எழுத்து *branch_ref = வெற்று;
	மாறிலி எழுத்து *branch_name = "(no branch)";
	எழுத்து *branch_name_buf = வெற்று;
	கட்டமைப்பு commit *head_commit = வெற்று;
	கட்டமைப்பு commit_list *parents = வெற்று;
	கட்டமைப்பு strbuf செய்தி = STRBUF_INIT;
	கட்டமைப்பு strbuf commit_மரம்_label = STRBUF_INIT;
	கட்டமைப்பு strbuf untracked_files = STRBUF_INIT;

	prepare_fallback_ident("git stash", "git@stash");

	repo_read_index_preload(the_repository, வெற்று, 0);
	எனில் (repo_refresh_and_write_index(the_repository, REFRESH_QUIET, 0, 0,
					 வெற்று, வெற்று, வெற்று) < 0) {
		ret = error(_("could not write index"));
		செல் done;
	}

	எனில் (repo_get_oid(the_repository, "HEAD", &info->b_commit)) {
		எனில் (!quiet)
			fprintf_ln(stderr, _("You do not have "
					     "the initial commit yet"));
		ret = -1;
		செல் done;
	} இல்லையெனில் {
		head_commit = lookup_commit(the_repository, &info->b_commit);
	}

	எனில் (!check_changes(ps, include_untracked, &untracked_files)) {
		ret = 1;
		செல் done;
	}

	branch_ref = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
					     "HEAD", 0, வெற்று, &flags);

	எனில் (flags & REF_ISSYMREF) {
		எனில் (skip_prefix(branch_ref, "refs/heads/", &branch_name))
			branch_name = branch_name_buf = xstrdup(branch_name);
	}

	head_short_sha1 = repo_find_unique_abbrev(the_repository,
						  &head_commit->object.oid,
						  DEFAULT_ABBREV);
	strbuf_addf(&செய்தி, "%s: %s ", branch_name, head_short_sha1);
	pp_commit_easy(CMIT_FMT_ONELINE, head_commit, &செய்தி);

	strbuf_addf(&commit_மரம்_label, "index on %s\n", செய்தி.buf);
	commit_list_செருகு(head_commit, &parents);
	எனில் (write_index_as_மரம்(&info->i_மரம், the_repository->index,
				repo_get_index_file(the_repository), 0, வெற்று) ||
	    commit_மரம்(commit_மரம்_label.buf, commit_மரம்_label.len,
			&info->i_மரம், parents, &info->i_commit, வெற்று, வெற்று)) {
		எனில் (!quiet)
			fprintf_ln(stderr, _("Cannot save the current "
					     "index state"));
		ret = -1;
		செல் done;
	}

	commit_list_free(parents);
	parents = வெற்று;

	எனில் (include_untracked) {
		எனில் (save_untracked_files(info, &செய்தி, &untracked_files)) {
			எனில் (!quiet)
				fprintf_ln(stderr, _("Cannot save "
						     "the untracked files"));
			ret = -1;
			செல் done;
		}
		untracked_commit_option = 1;
	}
	எனில் (patch_mode) {
		ret = stash_patch(info, ps, patch, quiet, interactive_opts);
		எனில் (ret < 0) {
			எனில் (!quiet)
				fprintf_ln(stderr, _("Cannot save the current "
						     "worktree state"));
			செல் done;
		} இல்லையெனில் எனில் (ret > 0) {
			செல் done;
		}
	} இல்லையெனில் எனில் (only_staged) {
		ret = stash_staged(info, patch, quiet);
		எனில் (ret < 0) {
			எனில் (!quiet)
				fprintf_ln(stderr, _("Cannot save the current "
						     "staged state"));
			செல் done;
		} இல்லையெனில் எனில் (ret > 0) {
			செல் done;
		}
	} இல்லையெனில் {
		எனில் (stash_working_மரம்(info, ps)) {
			எனில் (!quiet)
				fprintf_ln(stderr, _("Cannot save the current "
						     "worktree state"));
			ret = -1;
			செல் done;
		}
	}

	எனில் (!stash_செய்தி_buf->len)
		strbuf_addf(stash_செய்தி_buf, "WIP on %s", செய்தி.buf);
	இல்லையெனில்
		strbuf_insertf(stash_செய்தி_buf, 0, "On %s: ", branch_name);

	எனில் (untracked_commit_option)
		commit_list_செருகு(lookup_commit(the_repository,
						 &info->u_commit),
				   &parents);
	commit_list_செருகு(lookup_commit(the_repository, &info->i_commit),
			   &parents);
	commit_list_செருகு(head_commit, &parents);

	எனில் (commit_மரம்(stash_செய்தி_buf->buf, stash_செய்தி_buf->len, &info->w_மரம்,
			parents, &info->w_commit, வெற்று, வெற்று)) {
		எனில் (!quiet)
			fprintf_ln(stderr, _("Cannot record "
					     "working tree state"));
		ret = -1;
		செல் done;
	}

done:
	strbuf_release(&commit_மரம்_label);
	strbuf_release(&செய்தி);
	strbuf_release(&untracked_files);
	commit_list_free(parents);
	விடுவி(branch_name_buf);
	திரும்பு ret;
}

நிலையான முழு create_stash(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix UNUSED,
			கட்டமைப்பு repository *repo UNUSED)
{
	முழு ret;
	கட்டமைப்பு strbuf stash_செய்தி_buf = STRBUF_INIT;
	கட்டமைப்பு stash_info info = STASH_INFO_INIT;
	கட்டமைப்பு pathspec ps;

	/* Starting with argv[1], since argv[0] is "create" */
	strbuf_join_argv(&stash_செய்தி_buf, argc - 1, ++argv, ' ');

	memset(&ps, 0, அளவு(ps));
	எனில் (!check_changes_tracked_files(&ps))
		திரும்பு 0;

	ret = do_create_stash(&ps, &stash_செய்தி_buf, 0, 0, வெற்று, 0, &info,
			      வெற்று, 0);
	எனில் (!ret)
		printf_ln("%s", oid_to_hex(&info.w_commit));

	free_stash_info(&info);
	strbuf_release(&stash_செய்தி_buf);
	திரும்பு ret;
}

நிலையான முழு do_தள்ளு_stash(மாறிலி கட்டமைப்பு pathspec *ps, மாறிலி எழுத்து *stash_செய்தி, முழு quiet,
			 முழு keep_index, முழு patch_mode,
			 கட்டமைப்பு interactive_options *interactive_opts,
			 முழு include_untracked, முழு only_staged)
{
	முழு ret = 0;
	கட்டமைப்பு stash_info info = STASH_INFO_INIT;
	கட்டமைப்பு strbuf patch = STRBUF_INIT;
	கட்டமைப்பு strbuf stash_செய்தி_buf = STRBUF_INIT;
	கட்டமைப்பு strbuf untracked_files = STRBUF_INIT;
	கட்டமைப்பு strbuf out = STRBUF_INIT;

	எனில் (patch_mode && keep_index == -1)
		keep_index = 1;

	எனில் (patch_mode && include_untracked) {
		fprintf_ln(stderr, _("Can't use --patch and --include-untracked"
				     " or --all at the same time"));
		ret = -1;
		செல் done;
	}

	/* --patch overrides --staged */
	எனில் (patch_mode)
		only_staged = 0;

	எனில் (only_staged && include_untracked) {
		fprintf_ln(stderr, _("Can't use --staged and --include-untracked"
				     " or --all at the same time"));
		ret = -1;
		செல் done;
	}

	repo_read_index_preload(the_repository, வெற்று, 0);
	எனில் (!include_untracked && ps->nr) {
		எழுத்து *ps_matched = xcalloc(ps->nr, 1);

		எனில் (pathspec_needs_expanded_index(the_repository->index, ps))
			ensure_full_index(the_repository->index);
		க்கு (size_t i = 0; i < the_repository->index->cache_nr; i++)
			ce_path_match(the_repository->index, the_repository->index->cache[i], ps,
				      ps_matched);

		எனில் (report_path_error(ps_matched, ps)) {
			fprintf_ln(stderr, _("Did you forget to 'git add'?"));
			ret = -1;
			விடுவி(ps_matched);
			செல் done;
		}
		விடுவி(ps_matched);
	}

	எனில் (repo_refresh_and_write_index(the_repository, REFRESH_QUIET, 0, 0,
					 வெற்று, வெற்று, வெற்று)) {
		ret = error(_("could not write index"));
		செல் done;
	}

	எனில் (!check_changes(ps, include_untracked, &untracked_files)) {
		எனில் (!quiet)
			printf_ln(_("No local changes to save"));
		செல் done;
	}

	எனில் (!refs_reflog_exists(get_main_ref_store(the_repository), ref_stash) && do_clear_stash()) {
		ret = -1;
		எனில் (!quiet)
			fprintf_ln(stderr, _("Cannot initialize stash"));
		செல் done;
	}

	எனில் (stash_செய்தி)
		strbuf_addstr(&stash_செய்தி_buf, stash_செய்தி);
	எனில் (do_create_stash(ps, &stash_செய்தி_buf, include_untracked, patch_mode,
			    interactive_opts, only_staged, &info, &patch, quiet)) {
		ret = -1;
		செல் done;
	}

	எனில் (do_store_stash(&info.w_commit, stash_செய்தி_buf.buf, 1)) {
		ret = -1;
		எனில் (!quiet)
			fprintf_ln(stderr, _("Cannot save the current status"));
		செல் done;
	}

	எனில் (!quiet)
		printf_ln(_("Saved working directory and index state %s"),
			  stash_செய்தி_buf.buf);

	எனில் (!(patch_mode || only_staged)) {
		எனில் (include_untracked && !ps->nr) {
			கட்டமைப்பு child_process cp = CHILD_PROCESS_INIT;

			cp.git_cmd = 1;
			எனில் (startup_info->original_cwd) {
				cp.dir = startup_info->original_cwd;
				strvec_pushf(&cp.env, "%s=%s",
					     GIT_WORK_மரம்_ENVIRONMENT,
					     the_repository->worktree);
			}
			strvec_pushl(&cp.args, "clean", "--force",
				     "--quiet", "-d", ":/", வெற்று);
			எனில் (include_untracked == INCLUDE_ALL_FILES)
				strvec_தள்ளு(&cp.args, "-x");
			எனில் (run_command(&cp)) {
				ret = -1;
				செல் done;
			}
		}
		discard_index(the_repository->index);
		எனில் (ps->nr) {
			கட்டமைப்பு child_process cp_add = CHILD_PROCESS_INIT;
			கட்டமைப்பு child_process cp_diff = CHILD_PROCESS_INIT;
			கட்டமைப்பு child_process cp_apply = CHILD_PROCESS_INIT;

			cp_add.git_cmd = 1;
			strvec_தள்ளு(&cp_add.args, "add");
			எனில் (!include_untracked)
				strvec_தள்ளு(&cp_add.args, "-u");
			எனில் (include_untracked == INCLUDE_ALL_FILES)
				strvec_தள்ளு(&cp_add.args, "--force");
			strvec_தள்ளு(&cp_add.args, "--");
			add_pathspecs(&cp_add.args, ps);
			எனில் (run_command(&cp_add)) {
				ret = -1;
				செல் done;
			}

			cp_diff.git_cmd = 1;
			strvec_pushl(&cp_diff.args, "diff-index", "-p",
				     "--no-color",
				     "--cached", "--binary", "HEAD", "--",
				     வெற்று);
			add_pathspecs(&cp_diff.args, ps);
			எனில் (pipe_command(&cp_diff, வெற்று, 0, &out, 0, வெற்று, 0)) {
				ret = -1;
				செல் done;
			}

			cp_apply.git_cmd = 1;
			strvec_pushl(&cp_apply.args, "apply", "--index",
				     "-R", வெற்று);
			எனில் (pipe_command(&cp_apply, out.buf, out.len, வெற்று, 0,
					 வெற்று, 0)) {
				ret = -1;
				செல் done;
			}
		} இல்லையெனில் {
			கட்டமைப்பு child_process cp = CHILD_PROCESS_INIT;
			cp.git_cmd = 1;
			/* BUG: this nukes untracked files in the way */
			strvec_pushl(&cp.args, "reset", "--hard", "-q",
				     "--no-recurse-submodules", வெற்று);
			எனில் (run_command(&cp)) {
				ret = -1;
				செல் done;
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
		எனில் (keep_index == 1 && !is_null_oid(&info.i_மரம்) &&
		    !is_empty_மரம்_oid(&info.i_மரம், the_repository->hash_algo)) {
			கட்டமைப்பு child_process cp = CHILD_PROCESS_INIT;

			cp.git_cmd = 1;
			strvec_pushl(&cp.args, "checkout", "--no-overlay",
				     oid_to_hex(&info.i_மரம்), "--", வெற்று);
			எனில் (!ps->nr)
				strvec_தள்ளு(&cp.args, ":/");
			இல்லையெனில்
				add_pathspecs(&cp.args, ps);
			எனில் (run_command(&cp)) {
				ret = -1;
				செல் done;
			}
		}
		செல் done;
	} இல்லையெனில் {
		கட்டமைப்பு child_process cp = CHILD_PROCESS_INIT;

		cp.git_cmd = 1;
		strvec_pushl(&cp.args, "apply", "-R", வெற்று);

		எனில் (pipe_command(&cp, patch.buf, patch.len, வெற்று, 0, வெற்று, 0)) {
			எனில் (!quiet)
				fprintf_ln(stderr, _("Cannot remove "
						     "worktree changes"));
			ret = -1;
			செல் done;
		}

		எனில் (keep_index < 1) {
			கட்டமைப்பு child_process cp = CHILD_PROCESS_INIT;

			cp.git_cmd = 1;
			strvec_pushl(&cp.args, "reset", "-q", "--refresh", "--",
				     வெற்று);
			add_pathspecs(&cp.args, ps);
			எனில் (run_command(&cp)) {
				ret = -1;
				செல் done;
			}
		}
		செல் done;
	}

done:
	strbuf_release(&patch);
	strbuf_release(&out);
	free_stash_info(&info);
	strbuf_release(&stash_செய்தி_buf);
	strbuf_release(&untracked_files);
	திரும்பு ret;
}

நிலையான முழு தள்ளு_stash(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
		      முழு தள்ளு_assumed)
{
	முழு force_assume = 0;
	முழு keep_index = -1;
	முழு only_staged = 0;
	முழு patch_mode = 0;
	முழு include_untracked = 0;
	முழு quiet = 0;
	முழு pathspec_file_nul = 0;
	மாறிலி எழுத்து *stash_செய்தி = வெற்று;
	எழுத்து *pathspec_from_file = வெற்று;
	கட்டமைப்பு pathspec ps;
	கட்டமைப்பு interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	கட்டமைப்பு option options[] = {
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
		OPT_STRING('m', "message", &stash_செய்தி, N_("message"),
			   N_("stash message")),
		OPT_PATHSPEC_FROM_FILE(&pathspec_from_file),
		OPT_PATHSPEC_FILE_NUL(&pathspec_file_nul),
		OPT_END()
	};
	முழு ret;

	எனில் (argc) {
		முழு flags = PARSE_OPT_KEEP_DASHDASH;

		எனில் (தள்ளு_assumed)
			flags |= PARSE_OPT_STOP_AT_NON_OPTION;

		argc = parse_options(argc, argv, prefix, options,
				     தள்ளு_assumed ? git_stash_usage :
				     git_stash_தள்ளு_usage, flags);
		force_assume |= patch_mode;
	}

	எனில் (argc) {
		எனில் (!strcmp(argv[0], "--")) {
			argc--;
			argv++;
		} இல்லையெனில் எனில் (தள்ளு_assumed && !force_assume) {
			die("subcommand wasn't specified; 'push' can't be assumed due to unexpected token '%s'",
			    argv[0]);
		}
	}

	parse_pathspec(&ps, 0, PATHSPEC_PREFER_FULL | PATHSPEC_PREFIX_ORIGIN,
		       prefix, argv);

	எனில் (pathspec_from_file) {
		எனில் (patch_mode)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--patch");

		எனில் (only_staged)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--staged");

		எனில் (ps.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&ps, 0,
				    PATHSPEC_PREFER_FULL | PATHSPEC_PREFIX_ORIGIN,
				    prefix, pathspec_from_file, pathspec_file_nul);
	} இல்லையெனில் எனில் (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	எனில் (!patch_mode) {
		எனில் (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--patch");
		எனில் (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--patch");
		எனில் (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--patch");
	}

	எனில் (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	எனில் (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	ret = do_தள்ளு_stash(&ps, stash_செய்தி, quiet, keep_index, patch_mode,
			    &interactive_opts, include_untracked, only_staged);

	clear_pathspec(&ps);
	விடுவி(pathspec_from_file);
	திரும்பு ret;
}

நிலையான முழு தள்ளு_stash_unassumed(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
				கட்டமைப்பு repository *repo UNUSED)
{
	திரும்பு தள்ளு_stash(argc, argv, prefix, 0);
}

நிலையான முழு save_stash(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
		      கட்டமைப்பு repository *repo UNUSED)
{
	முழு keep_index = -1;
	முழு only_staged = 0;
	முழு patch_mode = 0;
	முழு include_untracked = 0;
	முழு quiet = 0;
	முழு ret = 0;
	மாறிலி எழுத்து *stash_செய்தி = வெற்று;
	கட்டமைப்பு pathspec ps;
	கட்டமைப்பு strbuf stash_செய்தி_buf = STRBUF_INIT;
	கட்டமைப்பு interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	கட்டமைப்பு option options[] = {
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
		OPT_STRING('m', "message", &stash_செய்தி, "message",
			   N_("stash message")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_save_usage,
			     PARSE_OPT_KEEP_DASHDASH);

	எனில் (argc)
		stash_செய்தி = strbuf_join_argv(&stash_செய்தி_buf, argc, argv, ' ');

	memset(&ps, 0, அளவு(ps));

	எனில் (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	எனில் (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	எனில் (!patch_mode) {
		எனில் (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--patch");
		எனில் (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--patch");
		எனில் (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--patch");
	}

	ret = do_தள்ளு_stash(&ps, stash_செய்தி, quiet, keep_index,
			    patch_mode, &interactive_opts, include_untracked,
			    only_staged);

	strbuf_release(&stash_செய்தி_buf);
	திரும்பு ret;
}

நிலையான முழு write_commit_with_parents(கட்டமைப்பு repository *r,
				     கட்டமைப்பு object_id *out,
				     மாறிலி கட்டமைப்பு object_id *oid,
				     கட்டமைப்பு commit_list *parents)
{
	size_t author_len, committer_len;
	கட்டமைப்பு commit *this;
	மாறிலி எழுத்து *orig_author, *orig_committer;
	எழுத்து *author = வெற்று, *committer = வெற்று;
	மாறிலி எழுத்து *buffer;
	unsigned long bufsize;
	மாறிலி எழுத்து *p;
	கட்டமைப்பு strbuf செய்தி = STRBUF_INIT;
	முழு ret = 0;
	கட்டமைப்பு ident_split id;

	this = lookup_commit_reference(r, oid);
	buffer = repo_get_commit_buffer(r, this, &bufsize);
	orig_author = find_commit_header(buffer, "author", &author_len);
	orig_committer = find_commit_header(buffer, "committer", &committer_len);

	எனில் (!orig_author || !orig_committer) {
		ret = error(_("cannot parse commit %s"), oid_to_hex(oid));
		செல் out;
	}

	எனில் (split_ident_வரி(&id, orig_author, author_len) < 0 ||
	    split_ident_வரி(&id, orig_committer, committer_len) < 0) {
		ret = error(_("invalid author or committer for %s"), oid_to_hex(oid));
		செல் out;
	}

	p = strstr(buffer, "\n\n");
	strbuf_addstr(&செய்தி, "git stash: ");

	எனில் (p)
		strbuf_add(&செய்தி, p + 2, bufsize - (p + 2 - buffer));
	strbuf_complete_வரி(&செய்தி);

	author = xmemdupz(orig_author, author_len);
	committer = xmemdupz(orig_committer, committer_len);

	எனில் (commit_மரம்_extended(செய்தி.buf, செய்தி.len,
				 r->hash_algo->empty_மரம், parents,
				 out, author, committer,
				 வெற்று, வெற்று)) {
		ret = error(_("could not write commit"));
		செல் out;
	}
out:
	strbuf_release(&செய்தி);
	repo_unuse_commit_buffer(r, this, buffer);
	விடுவி(author);
	விடுவி(committer);
	திரும்பு ret;
}

நிலையான முழு do_import_stash(கட்டமைப்பு repository *r, மாறிலி எழுத்து *rev)
{
	கட்டமைப்பு object_id chain;
	முழு res = 0;
	மாறிலி எழுத்து *buffer = வெற்று;
	unsigned long bufsize;
	கட்டமைப்பு commit *this = வெற்று;
	கட்டமைப்பு commit_list *பொருட்கள் = வெற்று, *cur;
	எழுத்து *செய்தி = வெற்று;

	எனில் (repo_get_oid(r, rev, &chain))
		திரும்பு error(_("not a valid revision: %s"), rev);

	this = lookup_commit_reference(r, &chain);
	எனில் (!this)
		திரும்பு error(_("not a commit: %s"), rev);

	/*
	 * Walk the commit history, finding each stash entry, and load data into
	 * the array.
	 */
	க்கு (;;) {
		மாறிலி எழுத்து *author, *committer;
		size_t author_len, committer_len;
		மாறிலி எழுத்து *p;
		மாறிலி எழுத்து *expected = "git stash <git@stash> 1000684800 +0000";
		மாறிலி எழுத்து *prefix = "git stash: ";
		கட்டமைப்பு commit *stash;
		கட்டமைப்பு மரம் *மரம் = repo_get_commit_மரம்(r, this);

		எனில் (!மரம் ||
		    !oideq(&மரம்->object.oid, r->hash_algo->empty_மரம்) ||
		    (this->parents &&
		     (!this->parents->next || this->parents->next->next))) {
			res = error(_("%s is not a valid exported stash commit"),
					oid_to_hex(&this->object.oid));
			செல் out;
		}

		buffer = repo_get_commit_buffer(r, this, &bufsize);

		எனில் (!this->parents) {
			/*
			 * We don't have any parents.  Make sure this is our
			 * root commit.
			 */
			author = find_commit_header(buffer, "author", &author_len);
			committer = find_commit_header(buffer, "committer", &committer_len);

			எனில் (!author || !committer) {
				error(_("cannot parse commit %s"), oid_to_hex(&this->object.oid));
				செல் out;
			}

			எனில் (author_len != strlen(expected) ||
			    committer_len != strlen(expected) ||
			    memcmp(author, expected, author_len) ||
			    memcmp(committer, expected, committer_len)) {
				res = error(_("found root commit %s with invalid data"), oid_to_hex(&this->object.oid));
				செல் out;
			}
			நிறுத்து;
		}

		p = strstr(buffer, "\n\n");
		எனில் (!p) {
			res = error(_("cannot parse commit %s"), oid_to_hex(&this->object.oid));
			செல் out;
		}

		p += 2;
		எனில் (((size_t)(bufsize - (p - buffer)) < strlen(prefix)) ||
		    memcmp(prefix, p, strlen(prefix))) {
			res = error(_("found stash commit %s without expected prefix"), oid_to_hex(&this->object.oid));
			செல் out;
		}

		stash = this->parents->next->item;

		எனில் (repo_parse_commit(r, this->parents->item) ||
		    repo_parse_commit(r, stash)) {
			res = error(_("cannot parse parents of commit: %s"),
					oid_to_hex(&this->object.oid));
			செல் out;
		}

		எனில் (check_stash_topology(r, stash)) {
			res = error(_("%s does not look like a stash commit"),
					oid_to_hex(&stash->object.oid));
			செல் out;
		}

		repo_unuse_commit_buffer(r, this, buffer);
		buffer = வெற்று;
		பொருட்கள் = commit_list_செருகு(stash, &பொருட்கள்);
		this = this->parents->item;
	}

	/*
	 * Now, walk each entry, adding it to the stash as a normal stash
	 * commit.
	 */
	க்கு (cur = பொருட்கள்; cur; cur = cur->next) {
		மாறிலி எழுத்து *p;
		கட்டமைப்பு object_id *oid;

		this = cur->item;
		oid = &this->object.oid;
		buffer = repo_get_commit_buffer(r, this, &bufsize);
		எனில் (!buffer) {
			res = error(_("cannot read commit buffer for %s"), oid_to_hex(oid));
			செல் out;
		}

		p = strstr(buffer, "\n\n");
		எனில் (!p) {
			res = error(_("cannot parse commit %s"), oid_to_hex(oid));
			செல் out;
		}

		p += 2;
		செய்தி = xmemdupz(p, bufsize - (p - buffer));
		repo_unuse_commit_buffer(r, this, buffer);
		buffer = வெற்று;

		எனில் (do_store_stash(oid, செய்தி, 1)) {
			res = error(_("cannot save the stash for %s"), oid_to_hex(oid));
			செல் out;
		}
		FREE_AND_NULL(செய்தி);
	}
out:
	எனில் (this && buffer)
		repo_unuse_commit_buffer(r, this, buffer);
	commit_list_free(பொருட்கள்);
	விடுவி(செய்தி);

	திரும்பு res;
}

நிலையான முழு import_stash(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
			கட்டமைப்பு repository *repo)
{
	கட்டமைப்பு option options[] = {
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_import_usage,
			     PARSE_OPT_KEEP_DASHDASH);

	எனில் (argc != 1)
		usage_செய்தி_opt("a revision is required", git_stash_import_usage, options);

	திரும்பு do_import_stash(repo, argv[0]);
}

கட்டமைப்பு stash_entry_data {
	கட்டமைப்பு repository *r;
	கட்டமைப்பு commit_list **பொருட்கள்;
	size_t மொத்தம்;
};

நிலையான முழு collect_stash_entries(மாறிலி எழுத்து *refname UNUSED,
				 கட்டமைப்பு object_id *old_oid UNUSED,
				 கட்டமைப்பு object_id *new_oid,
				 மாறிலி எழுத்து *committer UNUSED,
				 timestamp_t timestamp UNUSED,
				 முழு tz UNUSED, மாறிலி எழுத்து *செய்தி UNUSED,
				 வெறுமை *cb_data)
{
	கட்டமைப்பு stash_entry_data *data = cb_data;
	கட்டமைப்பு commit *stash;

	data->மொத்தம்++;
	stash = lookup_commit_reference(data->r, new_oid);
	எனில் (!stash || check_stash_topology(data->r, stash)) {
		திரும்பு error(_("%s does not look like a stash commit"),
			     oid_to_hex(new_oid));
	}
	data->பொருட்கள் = commit_list_append(stash, data->பொருட்கள்);
	திரும்பு 0;
}

நிலையான முழு do_export_stash(கட்டமைப்பு repository *r,
			   மாறிலி எழுத்து *ref,
			   முழு argc,
			   மாறிலி எழுத்து **argv)
{
	கட்டமைப்பு object_id base;
	கட்டமைப்பு commit *prev;
	கட்டமைப்பு commit_list *பொருட்கள் = வெற்று, **iter = &பொருட்கள், *cur;
	முழு res = 0;
	முழு i;
	கட்டமைப்பு strbuf revision = STRBUF_INIT;
	மாறிலி எழுத்து *author, *committer;

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
	எனில் (commit_மரம்_extended("", 0, r->hash_algo->empty_மரம், வெற்று,
				 &base, author, committer, வெற்று, வெற்று))
		திரும்பு error(_("unable to write base commit"));

	prev = lookup_commit_reference(r, &base);

	எனில் (argc) {
		/*
		 * Find each specified stash, and load data into the array.
		 */
		க்கு (i = 0; i < argc; i++) {
			கட்டமைப்பு object_id oid;
			கட்டமைப்பு commit *stash;

			எனில் (parse_stash_revision(&revision, argv[i], 1) ||
			    repo_get_oid_with_flags(r, revision.buf, &oid,
						    GET_OID_QUIETLY |
						    GET_OID_GENTLY)) {
				res = error(_("unable to find stash entry %s"), argv[i]);
				செல் out;
			}

			stash = lookup_commit_reference(r, &oid);
			எனில் (!stash || check_stash_topology(r, stash)) {
				res = error(_("%s does not look like a stash commit"),
						revision.buf);
				செல் out;
			}
			iter = commit_list_append(stash, iter);
		}
	} இல்லையெனில் {
		/*
		 * Walk the reflog, finding each stash entry, and load data into the
		 * array.
		 */
		கட்டமைப்பு stash_entry_data cb_data = {
			.r = r, .பொருட்கள் = iter,
		};
		எனில் (refs_for_each_reflog_ent_reverse(get_main_ref_store(r),
						     "refs/stash",
						     collect_stash_entries,
						     &cb_data) && cb_data.மொத்தம்)
			செல் out;
	}

	/*
	 * Now, create a set of commits identical to the regular stash commits,
	 * but where their first parents form a chain to our original empty
	 * base commit.
	 */
	பொருட்கள் = commit_list_reverse(பொருட்கள்);
	க்கு (cur = பொருட்கள்; cur; cur = cur->next) {
		கட்டமைப்பு commit_list *parents = வெற்று;
		கட்டமைப்பு commit_list **next = &parents;
		கட்டமைப்பு object_id out;
		கட்டமைப்பு commit *stash = cur->item;

		next = commit_list_append(prev, next);
		next = commit_list_append(stash, next);
		res = write_commit_with_parents(r, &out, &stash->object.oid, parents);
		commit_list_free(parents);
		எனில் (res)
			செல் out;
		prev = lookup_commit_reference(r, &out);
	}
	எனில் (ref)
		refs_update_ref(get_main_ref_store(r), வெற்று, ref,
				&prev->object.oid, வெற்று, 0, UPDATE_REFS_DIE_ON_ERR);
	இல்லையெனில்
		puts(oid_to_hex(&prev->object.oid));
out:
	strbuf_release(&revision);
	commit_list_free(பொருட்கள்);

	திரும்பு res;
}

எண்ணிக்கை export_action {
	ACTION_NONE,
	ACTION_PRINT,
	ACTION_TO_REF,
};

நிலையான முழு export_stash(முழு argc,
			மாறிலி எழுத்து **argv,
			மாறிலி எழுத்து *prefix,
			கட்டமைப்பு repository *repo)
{
	மாறிலி எழுத்து *ref = வெற்று;
	எண்ணிக்கை export_action action = ACTION_NONE;
	கட்டமைப்பு option options[] = {
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

	எனில் (ref && action == ACTION_NONE)
		action = ACTION_TO_REF;

	எனில் (action == ACTION_NONE || (ref && action == ACTION_PRINT))
		திரும்பு error(_("exactly one of --print and --to-ref is required"));

	திரும்பு do_export_stash(repo, ref, argc, argv);
}

முழு cmd_stash(முழு argc,
	      மாறிலி எழுத்து **argv,
	      மாறிலி எழுத்து *prefix,
	      கட்டமைப்பு repository *repo)
{
	pid_t pid = getpid();
	மாறிலி எழுத்து *index_file;
	கட்டமைப்பு strvec args = STRVEC_INIT;
	parse_opt_subcommand_fn *fn = வெற்று;
	கட்டமைப்பு option options[] = {
		OPT_SUBCOMMAND("apply", &fn, apply_stash),
		OPT_SUBCOMMAND("clear", &fn, clear_stash),
		OPT_SUBCOMMAND("drop", &fn, drop_stash),
		OPT_SUBCOMMAND("pop", &fn, எடு_stash),
		OPT_SUBCOMMAND("branch", &fn, branch_stash),
		OPT_SUBCOMMAND("list", &fn, list_stash),
		OPT_SUBCOMMAND("show", &fn, show_stash),
		OPT_SUBCOMMAND("store", &fn, store_stash),
		OPT_SUBCOMMAND("create", &fn, create_stash),
		OPT_SUBCOMMAND("push", &fn, தள்ளு_stash_unassumed),
		OPT_SUBCOMMAND("export", &fn, export_stash),
		OPT_SUBCOMMAND("import", &fn, import_stash),
		OPT_SUBCOMMAND_F("save", &fn, save_stash, வெற்று, PARSE_OPT_NOCOMPLETE),
		OPT_END()
	};
	மாறிலி எழுத்து **args_copy;
	முழு ret;

	repo_config(the_repository, git_stash_config, வெற்று);

	argc = parse_options(argc, argv, prefix, options, git_stash_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL |
			     PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	index_file = repo_get_index_file(the_repository);
	strbuf_addf(&stash_index_path, "%s.stash.%" PRIuMAX, index_file,
		    (uintmax_t)pid);

	எனில் (fn) {
		ret = fn(argc, argv, prefix, repo);

		/*
		 * The subcommand implementations return 0 on success, a
		 * negative value on failure, and STASH_APPLY_CONFLICT
		 * when applying a stash entry resulted in conflicts.
		 * Map failures to 128, the status die() uses, so that
		 * exit status 1 unambiguously indicates conflicts.
		 */
		எனில் (ret < 0)
			திரும்பு 128;
		திரும்பு ret;
	} இல்லையெனில் எனில் (!argc) {
		திரும்பு !!தள்ளு_stash_unassumed(0, வெற்று, prefix, repo);
	}

	/* Assume 'stash push' */
	strvec_தள்ளு(&args, "push");
	strvec_pushv(&args, argv);

	/*
	 * `push_stash()` ends up modifying the array, which causes memory
	 * leaks if we didn't copy the array here.
	 */
	DUP_ARRAY(args_copy, args.v, args.nr);

	ret = !!தள்ளு_stash(args.nr, args_copy, prefix, 1);

	strvec_clear(&args);
	விடுவி(args_copy);
	திரும்பு ret;
}
