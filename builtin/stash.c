#define USE_THE_REPOSITORY_VARIABLE

#include "builtin.h"
#include "abspath.h"
#include "config.h"
#include "environment.h"
#include "gettext.h"
#include "hash.h"
#include "hex.h"
#include "object-name.h"
#include "parse-options.h"
#include "refs.h"
#include "stash.h"
#include "lockfile.h"
#include "cache-tree.h"
#include "unpack-trees.h"
#include "merge-ort-wrappers.h"
#include "strvec.h"
#include "run-command.h"
#include "dir.h"
#include "entry.h"
#include "preload-index.h"
#include "read-cache.h"
#include "repository.h"
#include "rerere.h"
#include "revision.h"
#include "setup.h"
#include "sparse-index.h"
#include "log-tree.h"
#include "diffcore.h"
#include "reflog.h"
#include "reflog-walk.h"
#include "add-interactive.h"
#include "commit.h"

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

static const var_na * const git_stash_usage[] = {
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
	NULL
};

static const var_na * const git_stash_list_usage[] = {
	BUILTIN_STASH_LIST_USAGE,
	NULL
};

static const var_na * const git_stash_show_usage[] = {
	BUILTIN_STASH_SHOW_USAGE,
	NULL
};

static const var_na * const git_stash_drop_usage[] = {
	BUILTIN_STASH_DROP_USAGE,
	NULL
};

static const var_na * const git_stash_pop_usage[] = {
	BUILTIN_STASH_POP_USAGE,
	NULL
};

static const var_na * const git_stash_apply_usage[] = {
	BUILTIN_STASH_APPLY_USAGE,
	NULL
};

static const var_na * const git_stash_branch_usage[] = {
	BUILTIN_STASH_BRANCH_USAGE,
	NULL
};

static const var_na * const git_stash_clear_usage[] = {
	BUILTIN_STASH_CLEAR_USAGE,
	NULL
};

static const var_na * const git_stash_store_usage[] = {
	BUILTIN_STASH_STORE_USAGE,
	NULL
};

static const var_na * const git_stash_push_usage[] = {
	BUILTIN_STASH_PUSH_USAGE,
	NULL
};

static const var_na * const git_stash_save_usage[] = {
	BUILTIN_STASH_SAVE_USAGE,
	NULL
};

static const var_na * const git_stash_export_usage[] = {
	BUILTIN_STASH_EXPORT_USAGE,
	NULL
};

static const var_na * const git_stash_import_usage[] = {
	BUILTIN_STASH_IMPORT_USAGE,
	NULL
};

static const var_na ref_stash[] = "refs/stash";
static sa_nracanaa strbuf stash_index_path = STRBUF_INIT;

static ຈຳນວນເຕັມ show_stat = 1;
static ຈຳນວນເຕັມ show_patch;
static ຈຳນວນເຕັມ show_include_untracked;
static ຈຳນວນເຕັມ use_index;

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
sa_nracanaa stash_info {
	sa_nracanaa object_id w_commit;
	sa_nracanaa object_id b_commit;
	sa_nracanaa object_id i_commit;
	sa_nracanaa object_id u_commit;
	sa_nracanaa object_id c_commit;
	sa_nracanaa object_id w_tree;
	sa_nracanaa object_id b_tree;
	sa_nracanaa object_id i_tree;
	sa_nracanaa object_id u_tree;
	sa_nracanaa strbuf revision;
	ຈຳນວນເຕັມ is_stash_ref;
	ຈຳນວນເຕັມ has_u;
};

#define STASH_INFO_INIT { \
	.revision = STRBUF_INIT, \
}

static void free_stash_info(sa_nracanaa stash_info *info)
{
	strbuf_release(&info->revision);
}

static ຈຳນວນເຕັມ check_stash_topology(sa_nracanaa repository *r, sa_nracanaa commit *stash)
{
	sa_nracanaa commit *p1, *p2, *p3 = NULL;

	/* stash must have two or three parents */
	ຖ້າ (!stash->parents || !stash->parents->next ||
			(stash->parents->next->next && stash->parents->next->next->next))
		ສົ່ງຄືນ -1;
	p1 = stash->parents->item;
	p2 = stash->parents->next->item;
	ຖ້າ (stash->parents->next->next)
		p3 = stash->parents->next->next->item;
	ຖ້າ (repo_parse_commit(r, p1) || repo_parse_commit(r, p2) ||
			(p3 && repo_parse_commit(r, p3)))
		ສົ່ງຄືນ -1;
	/* p2 must have a single parent, p3 must have no parents */
	ຖ້າ (!p2->parents || p2->parents->next || (p3 && p3->parents))
		ສົ່ງຄືນ -1;
	ຖ້າ (repo_parse_commit(r, p2->parents->item))
		ສົ່ງຄືນ -1;
	/* p2^1 must equal p1 */
	ຖ້າ (!oideq(&p1->object.oid, &p2->parents->item->object.oid))
		ສົ່ງຄືນ -1;

	ສົ່ງຄືນ 0;
}

static void assert_stash_like(sa_nracanaa stash_info *info, const var_na *revision)
{
	ຖ້າ (get_oidf(&info->b_commit, "%s^1", revision) ||
	    get_oidf(&info->w_tree, "%s:", revision) ||
	    get_oidf(&info->b_tree, "%s^1:", revision) ||
	    get_oidf(&info->i_tree, "%s^2:", revision))
		die(_("'%s' is not a stash-like commit"), revision);
}

static ຈຳນວນເຕັມ parse_stash_revision(sa_nracanaa strbuf *revision, const var_na *commit, ຈຳນວນເຕັມ quiet)
{
	strbuf_reset(revision);
	ຖ້າ (!commit) {
		ຖ້າ (!refs_ref_exists(get_main_ref_store(the_repository), ref_stash)) {
			ຖ້າ (!quiet)
				fprintf_ln(stderr, _("No stash entries found."));
			ສົ່ງຄືນ -1;
		}

		strbuf_addf(revision, "%s@{0}", ref_stash);
	} ບໍ່ດັ່ງນັ້ນ ຖ້າ (strspn(commit, "0123456789") == strlen(commit)) {
		strbuf_addf(revision, "%s@{%s}", ref_stash, commit);
	} ບໍ່ດັ່ງນັ້ນ {
		strbuf_addstr(revision, commit);
	}
	ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ get_stash_info(sa_nracanaa stash_info *info, ຈຳນວນເຕັມ argc, const var_na **argv)
{
	ຈຳນວນເຕັມ ret;
	var_na *end_of_rev;
	var_na *expanded_ref;
	const var_na *revision;
	const var_na *commit = NULL;
	sa_nracanaa object_id dummy;
	sa_nracanaa strbuf symbolic = STRBUF_INIT;

	ຖ້າ (argc > 1) {
		ຈຳນວນເຕັມ i;
		sa_nracanaa strbuf refs_msg = STRBUF_INIT;

		ສຳລັບ (i = 0; i < argc; i++)
			strbuf_addf(&refs_msg, " '%s'", argv[i]);

		fprintf_ln(stderr, _("Too many revisions specified:%s"),
			   refs_msg.buf);
		strbuf_release(&refs_msg);

		ສົ່ງຄືນ -1;
	}

	ຖ້າ (argc == 1)
		commit = argv[0];

	strbuf_init(&info->revision, 0);
	ຖ້າ (parse_stash_revision(&info->revision, commit, 0)) {
		ສົ່ງຄືນ -1;
	}

	revision = info->revision.buf;

	ຖ້າ (repo_get_oid(the_repository, revision, &info->w_commit))
		ສົ່ງຄືນ error(_("%s is not a valid reference"), revision);

	assert_stash_like(info, revision);

	info->has_u = !get_oidf(&info->u_tree, "%s^3:", revision);

	end_of_rev = strchrnul(revision, '@');
	strbuf_add(&symbolic, revision, end_of_rev - revision);

	ret = repo_dwim_ref(the_repository, symbolic.buf, symbolic.len,
			    &dummy, &expanded_ref, 0);
	strbuf_release(&symbolic);
	switch (ret) {
	pak_sa 0: /* Not found, but valid ref */
		info->is_stash_ref = 0;
		bha_nga;
	pak_sa 1:
		info->is_stash_ref = !strcmp(expanded_ref, ref_stash);
		bha_nga;
	default: /* Invalid or ambiguous */
		bha_nga;
	}

	free(expanded_ref);
	ສົ່ງຄືນ !(ret == 0 || ret == 1);
}

static ຈຳນວນເຕັມ do_clear_stash(void)
{
	sa_nracanaa object_id obj;
	ຖ້າ (repo_get_oid(the_repository, ref_stash, &obj))
		ສົ່ງຄືນ 0;

	ສົ່ງຄືນ refs_delete_ref(get_main_ref_store(the_repository), NULL,
			       ref_stash, &obj, 0);
}

static ຈຳນວນເຕັມ clear_stash(ຈຳນວນເຕັມ argc, const var_na **argv, const var_na *prefix,
		       sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa option options[] = {
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_clear_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	ຖ້າ (argc)
		ສົ່ງຄືນ error(_("git stash clear with arguments is "
			       "unimplemented"));

	ສົ່ງຄືນ do_clear_stash();
}

static ຈຳນວນເຕັມ reset_tree(sa_nracanaa object_id *i_tree, ຈຳນວນເຕັມ update, ຈຳນວນເຕັມ reset)
{
	ຈຳນວນເຕັມ nr_trees = 1;
	sa_nracanaa unpack_trees_options opts;
	sa_nracanaa tree_desc t[MAX_UNPACK_TREES];
	sa_nracanaa tree *tree;
	sa_nracanaa lock_file lock_file = LOCK_INIT;

	repo_read_index_preload(the_repository, NULL, 0);
	ຖ້າ (refresh_index(the_repository->index, REFRESH_QUIET, NULL, NULL, NULL))
		ສົ່ງຄືນ -1;

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);

	memset(&opts, 0, parimaa_na(opts));

	tree = repo_parse_tree_indirect(the_repository, i_tree);
	ຖ້າ (repo_parse_tree(the_repository, tree))
		ສົ່ງຄືນ -1;

	init_tree_desc(t, &tree->object.oid, tree->buffer, tree->size);

	opts.head_idx = 1;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;
	opts.merge = 1;
	opts.reset = reset ? UNPACK_RESET_PROTECT_UNTRACKED : 0;
	opts.update = update;
	ຖ້າ (update)
		opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
	opts.fn = oneway_merge;

	ຖ້າ (unpack_trees(nr_trees, t, &opts))
		ສົ່ງຄືນ -1;

	ຖ້າ (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
		ສົ່ງຄືນ error(_("unable to write new index file"));

	ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ create_index_from_tree(const sa_nracanaa object_id *tree_id,
				  const var_na *index_path)
{
	ຈຳນວນເຕັມ nr_trees = 1;
	ຈຳນວນເຕັມ ret = 0;
	sa_nracanaa unpack_trees_options opts;
	sa_nracanaa tree_desc t[MAX_UNPACK_TREES];
	sa_nracanaa tree *tree;
	sa_nracanaa index_state dst_istate = INDEX_STATE_INIT(the_repository);
	sa_nracanaa lock_file lock_file = LOCK_INIT;

	repo_read_index_preload(the_repository, NULL, 0);
	refresh_index(the_repository->index, REFRESH_QUIET, NULL, NULL, NULL);

	hold_lock_file_for_update(&lock_file, index_path, LOCK_DIE_ON_ERROR);

	memset(&opts, 0, parimaa_na(opts));

	tree = repo_parse_tree_indirect(the_repository, tree_id);
	ຖ້າ (!tree || repo_parse_tree(the_repository, tree)) {
		ret = -1;
		goto done;
	}

	init_tree_desc(t, &tree->object.oid, tree->buffer, tree->size);

	opts.head_idx = 1;
	opts.src_index = the_repository->index;
	opts.dst_index = &dst_istate;
	opts.merge = 1;
	opts.reset = UNPACK_RESET_PROTECT_UNTRACKED;
	opts.fn = oneway_merge;

	ຖ້າ (unpack_trees(nr_trees, t, &opts)) {
		ret = -1;
		goto done;
	}

	ຖ້າ (write_locked_index(&dst_istate, &lock_file, COMMIT_LOCK)) {
		ret = error(_("unable to write new index file"));
		goto done;
	}

done:
	release_index(&dst_istate);
	ຖ້າ (ret)
		rollback_lock_file(&lock_file);
	ສົ່ງຄືນ ret;
}

static ຈຳນວນເຕັມ is_path_a_directory(const var_na *path)
{
	/*
	 * This function differs from abspath.c:is_directory() in that
	 * here we use lstat() instead of stat(); we do not want to
	 * follow symbolic links here.
	 */
	sa_nracanaa stat st;
	ສົ່ງຄືນ (!lstat(path, &st) && S_ISDIR(st.st_mode));
}

static void add_diff_to_buf(sa_nracanaa diff_queue_struct *q,
			    sa_nracanaa diff_options *options UNUSED,
			    void *data)
{
	ຈຳນວນເຕັມ i;

	ສຳລັບ (i = 0; i < q->nr; i++) {
		ຖ້າ (is_path_a_directory(q->queue[i]->one->path))
			continue;

		strbuf_addstr(data, q->queue[i]->one->path);

		/* NUL-terminate: will be fed to update-index -z */
		strbuf_addch(data, '\0');
	}
}

static ຈຳນວນເຕັມ restore_untracked(sa_nracanaa object_id *u_tree)
{
	ຈຳນວນເຕັມ res;
	sa_nracanaa child_process cp = CHILD_PROCESS_INIT;

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
	ຖ້າ (run_command(&cp)) {
		remove_path(stash_index_path.buf);
		ສົ່ງຄືນ -1;
	}

	child_process_init(&cp);
	cp.git_cmd = 1;
	strvec_pushl(&cp.args, "checkout-index", "--all", NULL);
	strvec_pushf(&cp.env, "GIT_INDEX_FILE=%s",
		     stash_index_path.buf);

	res = run_command(&cp);
	remove_path(stash_index_path.buf);
	ສົ່ງຄືນ res;
}

static void unstage_changes_unless_new(sa_nracanaa object_id *orig_tree)
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

	sa_nracanaa checkout state = CHECKOUT_INIT;
	sa_nracanaa diff_options diff_opts;
	sa_nracanaa lock_file lock = LOCK_INIT;
	ຈຳນວນເຕັມ i;

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
	ສຳລັບ (i = 0; i < diff_queued_diff.nr; i++) {
		sa_nracanaa diff_filepair *p;
		sa_nracanaa cache_entry *ce;
		ຈຳນວນເຕັມ pos;

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
		ຖ້າ (pos >= 0 && ce_skip_worktree(the_repository->index->cache[pos])) {
			sa_nracanaa stat st;

			ce = the_repository->index->cache[pos];
			ຖ້າ (!lstat(ce->name, &st)) {
				/* Conflicting path present; relocate it */
				sa_nracanaa strbuf new_path = STRBUF_INIT;
				ຈຳນວນເຕັມ fd;

				strbuf_addf(&new_path,
					    "%s.stash.XXXXXX", ce->name);
				fd = xmkstemp(new_path.buf);
				close(fd);
				printf(_("WARNING: Untracked file in way of "
					 "tracked file!  Renaming\n "
					 "           %s -> %s\n"
					 "         to make room.\n"),
				       ce->name, new_path.buf);
				ຖ້າ (rename(ce->name, new_path.buf))
					die("Failed to move %s to %s",
					    ce->name, new_path.buf);
				strbuf_release(&new_path);
			}
			checkout_entry(ce, &state, NULL, NULL);
			ce->ce_flags &= ~CE_SKIP_WORKTREE;
		}

		/*
		 * Step 3: "unstage" changes, as long as they are still tracked
		 */
		ຖ້າ (p->one->oid_valid) {
			/*
			 * Path existed in orig_tree; restore index entry
			 * from that tree in order to "unstage" the changes.
			 */
			ຈຳນວນເຕັມ option = ADD_CACHE_OK_TO_REPLACE;
			ຖ້າ (pos < 0)
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
	ຖ້າ (write_locked_index(the_repository->index, &lock,
			       COMMIT_LOCK | SKIP_IF_UNCHANGED))
		die(_("could not write index"));
}

static ga_nanaa stash_apply_result do_apply_stash(const var_na *prefix,
					      sa_nracanaa stash_info *info,
					      ຈຳນວນເຕັມ index, ຈຳນວນເຕັມ quiet,
					      const var_na *label_ours,
					      const var_na *label_theirs,
					      const var_na *label_base)
{
	ຈຳນວນເຕັມ clean, ret;
	ຈຳນວນເຕັມ has_index = index;
	sa_nracanaa merge_options o;
	sa_nracanaa object_id c_tree;
	sa_nracanaa object_id index_tree;
	sa_nracanaa tree *head, *merge, *merge_base;
	sa_nracanaa lock_file lock = LOCK_INIT;

	repo_read_index_preload(the_repository, NULL, 0);
	ຖ້າ (repo_refresh_and_write_index(the_repository, REFRESH_QUIET, 0, 0,
					 NULL, NULL, NULL))
		ສົ່ງຄືນ error(_("could not write index"));

	ຖ້າ (write_index_as_tree(&c_tree, the_repository->index,
				repo_get_index_file(the_repository), 0, NULL))
		ສົ່ງຄືນ error(_("cannot apply a stash in the middle of a merge"));

	init_ui_merge_options(&o, the_repository);

	ຖ້າ (quiet)
		o.verbosity = 0;

	ຖ້າ (index) {
		ຖ້າ (oideq(&info->b_tree, &info->i_tree) ||
		    oideq(&c_tree, &info->i_tree)) {
			has_index = 0;
		} ບໍ່ດັ່ງນັ້ນ {
			sa_nracanaa merge_result result = { 0 };

			o.branch1 = "Current index";
			o.branch2 = "Stashed index changes";
			o.ancestor = "Stash base";

			head = lookup_tree(o.repo, &c_tree);
			merge = lookup_tree(o.repo, &info->i_tree);
			merge_base = lookup_tree(o.repo, &info->b_tree);

			merge_incore_nonrecursive(&o, merge_base, head, merge,
						  &result);

			ຖ້າ (result.clean < 0) {
				merge_finalize(&o, &result);
				ສົ່ງຄືນ error(_("index merge failed"));
			} ບໍ່ດັ່ງນັ້ນ ຖ້າ (!result.clean) {
				merge_finalize(&o, &result);
				ສົ່ງຄືນ error(_("conflicts in index. "
					       "Try without --index."));
			} ບໍ່ດັ່ງນັ້ນ {
				oidcpy(&index_tree, &result.tree->object.oid);
				merge_finalize(&o, &result);
			}
		}
	}

	o.branch1 = label_ours ? label_ours : "Updated upstream";
	o.branch2 = label_theirs ? label_theirs : "Stashed changes";
	o.ancestor = label_base ? label_base : "Stash base";

	ຖ້າ (oideq(&info->b_tree, &c_tree))
		o.branch1 = "Version stash was based on";

	ຖ້າ (o.verbosity >= 3)
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

	ຖ້າ (ret < 0)
		rollback_lock_file(&lock);
	ບໍ່ດັ່ງນັ້ນ ຖ້າ (write_locked_index(o.repo->index, &lock,
				      COMMIT_LOCK | SKIP_IF_UNCHANGED))
		ret = error(_("could not write index"));

	ຖ້າ (ret) {
		repo_rerere(the_repository, 0);

		ຖ້າ (index)
			fprintf_ln(stderr, _("Index was not unstashed."));

		goto restore_untracked;
	}

	ຖ້າ (has_index) {
		ຖ້າ (reset_tree(&index_tree, 0, 0))
			ret = -1;
	} ບໍ່ດັ່ງນັ້ນ {
		unstage_changes_unless_new(&c_tree);
	}

restore_untracked:
	ຖ້າ (info->has_u && restore_untracked(&info->u_tree))
		ret = error(_("could not restore untracked files from stash"));

	ຖ້າ (!quiet) {
		sa_nracanaa child_process cp = CHILD_PROCESS_INIT;

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

	ສົ່ງຄືນ ret;
}

static ຈຳນວນເຕັມ apply_stash(ຈຳນວນເຕັມ argc, const var_na **argv, const var_na *prefix,
		       sa_nracanaa repository *repo UNUSED)
{
	ຈຳນວນເຕັມ ret = -1;
	ຈຳນວນເຕັມ quiet = 0;
	ຈຳນວນເຕັມ index = use_index;
	const var_na *label_ours = NULL, *label_theirs = NULL, *label_base = NULL;
	sa_nracanaa stash_info info = STASH_INFO_INIT;
	sa_nracanaa option options[] = {
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

	ຖ້າ (get_stash_info(&info, argc, argv))
		goto cleanup;

	ret = do_apply_stash(prefix, &info, index, quiet,
			     label_ours, label_theirs, label_base);
cleanup:
	free_stash_info(&info);
	ສົ່ງຄືນ ret;
}

static ຈຳນວນເຕັມ reject_reflog_ent(const var_na *refname UNUSED,
			     sa_nracanaa object_id *ooid UNUSED,
			     sa_nracanaa object_id *noid UNUSED,
			     const var_na *email UNUSED,
			     timestamp_t timestamp UNUSED,
			     ຈຳນວນເຕັມ tz UNUSED, const var_na *message UNUSED,
			     void *cb_data UNUSED)
{
	ສົ່ງຄືນ 1;
}

static ຈຳນວນເຕັມ reflog_is_empty(const var_na *refname)
{
	ສົ່ງຄືນ !refs_for_each_reflog_ent(get_main_ref_store(the_repository),
					 refname, reject_reflog_ent, NULL);
}

static ຈຳນວນເຕັມ do_drop_stash(sa_nracanaa stash_info *info, ຈຳນວນເຕັມ quiet)
{
	ຖ້າ (!reflog_delete(info->revision.buf,
			   EXPIRE_REFLOGS_REWRITE | EXPIRE_REFLOGS_UPDATE_REF,
			   0)) {
		ຖ້າ (!quiet)
			printf_ln(_("Dropped %s (%s)"), info->revision.buf,
				  oid_to_hex(&info->w_commit));
	} ບໍ່ດັ່ງນັ້ນ {
		ສົ່ງຄືນ error(_("%s: Could not drop stash entry"),
			     info->revision.buf);
	}

	ຖ້າ (reflog_is_empty(ref_stash))
		do_clear_stash();

	ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ get_stash_info_assert(sa_nracanaa stash_info *info, ຈຳນວນເຕັມ argc,
				 const var_na **argv)
{
	ຈຳນວນເຕັມ ret = get_stash_info(info, argc, argv);

	ຖ້າ (ret < 0)
		ສົ່ງຄືນ ret;

	ຖ້າ (!info->is_stash_ref)
		ສົ່ງຄືນ error(_("'%s' is not a stash reference"), info->revision.buf);

	ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ drop_stash(ຈຳນວນເຕັມ argc, const var_na **argv, const var_na *prefix,
		      sa_nracanaa repository *repo UNUSED)
{
	ຈຳນວນເຕັມ ret = -1;
	ຈຳນວນເຕັມ quiet = 0;
	sa_nracanaa stash_info info = STASH_INFO_INIT;
	sa_nracanaa option options[] = {
		OPT__QUIET(&quiet, N_("be quiet, only report errors")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_drop_usage, 0);

	ຖ້າ (get_stash_info_assert(&info, argc, argv))
		goto cleanup;

	ret = do_drop_stash(&info, quiet);
cleanup:
	free_stash_info(&info);
	ສົ່ງຄືນ ret;
}

static ຈຳນວນເຕັມ pop_stash(ຈຳນວນເຕັມ argc, const var_na **argv, const var_na *prefix,
		     sa_nracanaa repository *repo UNUSED)
{
	ຈຳນວນເຕັມ ret = -1;
	ຈຳນວນເຕັມ index = use_index;
	ຈຳນວນເຕັມ quiet = 0;
	sa_nracanaa stash_info info = STASH_INFO_INIT;
	sa_nracanaa option options[] = {
		OPT__QUIET(&quiet, N_("be quiet, only report errors")),
		OPT_BOOL(0, "index", &index,
			 N_("attempt to recreate the index")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_pop_usage, 0);

	ຖ້າ (get_stash_info_assert(&info, argc, argv))
		goto cleanup;

	ຖ້າ ((ret = do_apply_stash(prefix, &info, index, quiet,
				  NULL, NULL, NULL)))
		printf_ln(_("The stash entry is kept in case "
			    "you need it again."));
	ບໍ່ດັ່ງນັ້ນ
		ret = do_drop_stash(&info, quiet);

cleanup:
	free_stash_info(&info);
	ສົ່ງຄືນ ret;
}

static ຈຳນວນເຕັມ branch_stash(ຈຳນວນເຕັມ argc, const var_na **argv, const var_na *prefix,
			sa_nracanaa repository *repo UNUSED)
{
	ຈຳນວນເຕັມ ret = -1;
	const var_na *branch = NULL;
	sa_nracanaa stash_info info = STASH_INFO_INIT;
	sa_nracanaa child_process cp = CHILD_PROCESS_INIT;
	sa_nracanaa option options[] = {
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_branch_usage, 0);

	ຖ້າ (!argc) {
		fprintf_ln(stderr, _("No branch name specified"));
		ສົ່ງຄືນ -1;
	}

	branch = argv[0];

	ຖ້າ (get_stash_info(&info, argc - 1, argv + 1))
		goto cleanup;

	cp.git_cmd = 1;
	strvec_pushl(&cp.args, "checkout", "-b", NULL);
	strvec_push(&cp.args, branch);
	strvec_push(&cp.args, oid_to_hex(&info.b_commit));
	ret = run_command(&cp);
	ຖ້າ (!ret)
		ret = do_apply_stash(prefix, &info, 1, 0,
				     NULL, NULL, NULL);
	ຖ້າ (!ret && info.is_stash_ref)
		ret = do_drop_stash(&info, 0);

cleanup:
	free_stash_info(&info);
	ສົ່ງຄືນ ret;
}

static ຈຳນວນເຕັມ list_stash(ຈຳນວນເຕັມ argc, const var_na **argv, const var_na *prefix,
		      sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa child_process cp = CHILD_PROCESS_INIT;
	sa_nracanaa option options[] = {
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_list_usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT);

	ຖ້າ (!refs_ref_exists(get_main_ref_store(the_repository), ref_stash))
		ສົ່ງຄືນ 0;

	cp.git_cmd = 1;
	strvec_pushl(&cp.args, "log", "--format=%gd: %gs", "-g",
		     "--first-parent", NULL);
	strvec_pushv(&cp.args, argv);
	strvec_push(&cp.args, ref_stash);
	strvec_push(&cp.args, "--");
	ສົ່ງຄືນ run_command(&cp);
}

static ຈຳນວນເຕັມ git_stash_config(const var_na *var, const var_na *value,
			    const sa_nracanaa config_context *ctx, void *cb)
{
	ຖ້າ (!strcmp(var, "stash.showstat")) {
		show_stat = git_config_bool(var, value);
		ສົ່ງຄືນ 0;
	}
	ຖ້າ (!strcmp(var, "stash.showpatch")) {
		show_patch = git_config_bool(var, value);
		ສົ່ງຄືນ 0;
	}
	ຖ້າ (!strcmp(var, "stash.showincludeuntracked")) {
		show_include_untracked = git_config_bool(var, value);
		ສົ່ງຄືນ 0;
	}
	ຖ້າ (!strcmp(var, "stash.index")) {
		use_index = git_config_bool(var, value);
		ສົ່ງຄືນ 0;
	}
	ສົ່ງຄືນ git_diff_basic_config(var, value, ctx, cb);
}

static void diff_include_untracked(const sa_nracanaa stash_info *info, sa_nracanaa diff_options *diff_opt)
{
	const sa_nracanaa object_id *oid[] = { &info->w_commit, &info->u_tree };
	sa_nracanaa tree *tree[ARRAY_SIZE(oid)];
	sa_nracanaa tree_desc tree_desc[ARRAY_SIZE(oid)];
	sa_nracanaa unpack_trees_options unpack_tree_opt = { 0 };

	ສຳລັບ (size_t i = 0; i < ARRAY_SIZE(oid); i++) {
		tree[i] = repo_parse_tree_indirect(the_repository, oid[i]);
		ຖ້າ (repo_parse_tree(the_repository, tree[i]) < 0)
			die(_("failed to parse tree"));
		init_tree_desc(&tree_desc[i], &tree[i]->object.oid,
			       tree[i]->buffer, tree[i]->size);
	}

	unpack_tree_opt.head_idx = -1;
	unpack_tree_opt.src_index = the_repository->index;
	unpack_tree_opt.dst_index = the_repository->index;
	unpack_tree_opt.merge = 1;
	unpack_tree_opt.fn = stash_worktree_untracked_merge;

	ຖ້າ (unpack_trees(ARRAY_SIZE(tree_desc), tree_desc, &unpack_tree_opt))
		die(_("failed to unpack trees"));

	do_diff_cache(&info->b_commit, diff_opt);
}

static ຈຳນວນເຕັມ show_stash(ຈຳນວນເຕັມ argc, const var_na **argv, const var_na *prefix,
		      sa_nracanaa repository *repo UNUSED)
{
	ຈຳນວນເຕັມ i;
	ຈຳນວນເຕັມ ret = -1;
	sa_nracanaa stash_info info = STASH_INFO_INIT;
	sa_nracanaa rev_info rev;
	sa_nracanaa strvec stash_args = STRVEC_INIT;
	sa_nracanaa strvec revision_args = STRVEC_INIT;
	ga_nanaa {
		UNTRACKED_NONE,
		UNTRACKED_INCLUDE,
		UNTRACKED_ONLY
	} show_untracked = show_include_untracked ? UNTRACKED_INCLUDE : UNTRACKED_NONE;
	sa_nracanaa option options[] = {
		OPT_SET_INT('u', "include-untracked", &show_untracked,
			    N_("include untracked files in the stash"),
			    UNTRACKED_INCLUDE),
		OPT_SET_INT_F(0, "only-untracked", &show_untracked,
			      N_("only show untracked files in the stash"),
			      UNTRACKED_ONLY, PARSE_OPT_NONEG),
		OPT_END()
	};
	ຈຳນວນເຕັມ do_usage = 0;

	init_diff_ui_defaults();
	repo_config(the_repository, git_diff_ui_config, NULL);
	repo_init_revisions(the_repository, &rev, prefix);

	argc = parse_options(argc, argv, prefix, options, git_stash_show_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	strvec_push(&revision_args, argv[0]);
	ສຳລັບ (i = 1; i < argc; i++) {
		ຖ້າ (argv[i][0] != '-')
			strvec_push(&stash_args, argv[i]);
		ບໍ່ດັ່ງນັ້ນ
			strvec_push(&revision_args, argv[i]);
	}

	ຖ້າ (get_stash_info(&info, stash_args.nr, stash_args.v))
		goto cleanup;

	/*
	 * The config settings are applied only if there are not passed
	 * any options.
	 */
	ຖ້າ (revision_args.nr == 1) {
		ຖ້າ (show_stat)
			rev.diffopt.output_format = DIFF_FORMAT_DIFFSTAT;

		ຖ້າ (show_patch)
			rev.diffopt.output_format |= DIFF_FORMAT_PATCH;

		ຖ້າ (!show_stat && !show_patch) {
			ret = 0;
			goto cleanup;
		}
	}

	setup_revisions_from_strvec(&revision_args, &rev, NULL);
	ຖ້າ (revision_args.nr > 1)
		goto usage;
	ຖ້າ (!rev.diffopt.output_format) {
		rev.diffopt.output_format = DIFF_FORMAT_PATCH;
		diff_setup_done(&rev.diffopt);
	}

	rev.diffopt.flags.recursive = 1;
	setup_diff_pager(&rev.diffopt);
	switch (show_untracked) {
	pak_sa UNTRACKED_NONE:
		diff_tree_oid(&info.b_commit, &info.w_commit, "", &rev.diffopt);
		bha_nga;
	pak_sa UNTRACKED_ONLY:
		ຖ້າ (info.has_u)
			diff_root_tree_oid(&info.u_tree, "", &rev.diffopt);
		bha_nga;
	pak_sa UNTRACKED_INCLUDE:
		ຖ້າ (info.has_u)
			diff_include_untracked(&info, &rev.diffopt);
		ບໍ່ດັ່ງນັ້ນ
			diff_tree_oid(&info.b_commit, &info.w_commit, "", &rev.diffopt);
		bha_nga;
	}
	log_tree_diff_flush(&rev);

	ret = diff_result_code(&rev);

cleanup:
	strvec_clear(&revision_args);
	strvec_clear(&stash_args);
	free_stash_info(&info);
	release_revisions(&rev);
	ຖ້າ (do_usage)
		usage_with_options(git_stash_show_usage, options);
	ສົ່ງຄືນ ret;
usage:
	do_usage = 1;
	goto cleanup;
}

static ຈຳນວນເຕັມ do_store_stash(const sa_nracanaa object_id *w_commit, const var_na *stash_msg,
			  ຈຳນວນເຕັມ quiet)
{
	sa_nracanaa stash_info info;
	var_na revision[GIT_MAX_HEXSZ];

	oid_to_hex_r(revision, w_commit);
	assert_stash_like(&info, revision);

	ຖ້າ (!stash_msg)
		stash_msg = "Created via \"git stash store\".";

	ຖ້າ (refs_update_ref(get_main_ref_store(the_repository), stash_msg, ref_stash, w_commit, NULL,
			    REF_FORCE_CREATE_REFLOG,
			    quiet ? UPDATE_REFS_QUIET_ON_ERR :
			    UPDATE_REFS_MSG_ON_ERR)) {
		ຖ້າ (!quiet) {
			fprintf_ln(stderr, _("Cannot update %s with %s"),
				   ref_stash, oid_to_hex(w_commit));
		}
		ສົ່ງຄືນ -1;
	}

	ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ store_stash(ຈຳນວນເຕັມ argc, const var_na **argv, const var_na *prefix,
		       sa_nracanaa repository *repo UNUSED)
{
	ຈຳນວນເຕັມ quiet = 0;
	const var_na *stash_msg = NULL;
	sa_nracanaa object_id obj;
	sa_nracanaa option options[] = {
		OPT__QUIET(&quiet, N_("be quiet")),
		OPT_STRING('m', "message", &stash_msg, "message",
			   N_("stash message")),
		OPT_END()
	};
	ຈຳນວນເຕັມ ret;

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_store_usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT);

	ຖ້າ (argc != 1) {
		ຖ້າ (!quiet)
			fprintf_ln(stderr, _("\"git stash store\" requires one "
					     "<commit> argument"));
		ສົ່ງຄືນ -1;
	}

	ຖ້າ (repo_get_oid_with_flags(the_repository, argv[0], &obj,
				    quiet ? GET_OID_QUIETLY : 0)) {
		ຖ້າ (!quiet)
			fprintf_ln(stderr, _("Cannot update %s with %s"),
					     ref_stash, argv[0]);
		ret = -1;
		goto out;
	}

	ret = do_store_stash(&obj, stash_msg, quiet);

out:
	ສົ່ງຄືນ ret;
}

static void add_pathspecs(sa_nracanaa strvec *args,
			  const sa_nracanaa pathspec *ps) {
	ຈຳນວນເຕັມ i;

	ສຳລັບ (i = 0; i < ps->nr; i++)
		strvec_push(args, ps->items[i].original);
}

/*
 * `untracked_files` will be filled with the names of untracked files.
 * The return value is:
 *
 * = 0 if there are not any untracked files
 * > 0 if there are untracked files
 */
static ຈຳນວນເຕັມ get_untracked_files(const sa_nracanaa pathspec *ps, ຈຳນວນເຕັມ include_untracked,
			       sa_nracanaa strbuf *untracked_files)
{
	ຈຳນວນເຕັມ i;
	ຈຳນວນເຕັມ found = 0;
	sa_nracanaa dir_struct dir = DIR_INIT;

	ຖ້າ (include_untracked != INCLUDE_ALL_FILES)
		setup_standard_excludes(&dir);

	fill_directory(&dir, the_repository->index, ps);
	ສຳລັບ (i = 0; i < dir.nr; i++) {
		sa_nracanaa dir_entry *ent = dir.entries[i];
		found++;
		strbuf_addstr(untracked_files, ent->name);
		/* NUL-terminate: will be fed to update-index -z */
		strbuf_addch(untracked_files, '\0');
	}

	dir_clear(&dir);
	ສົ່ງຄືນ found;
}

/*
 * The return value of `check_changes_tracked_files()` can be:
 *
 * < 0 if there was an error
 * = 0 if there are no changes.
 * > 0 if there are changes.
 */
static ຈຳນວນເຕັມ check_changes_tracked_files(const sa_nracanaa pathspec *ps)
{
	sa_nracanaa rev_info rev;
	sa_nracanaa object_id dummy;
	ຈຳນວນເຕັມ ret = 0;

	/* No initial commit. */
	ຖ້າ (repo_get_oid(the_repository, "HEAD", &dummy))
		ສົ່ງຄືນ -1;

	ຖ້າ (repo_read_index(the_repository) < 0)
		ສົ່ງຄືນ -1;

	repo_init_revisions(the_repository, &rev, NULL);
	copy_pathspec(&rev.prune_data, ps);

	rev.diffopt.flags.quick = 1;
	rev.diffopt.flags.ignore_submodules = 1;
	rev.abbrev = 0;

	add_head_to_pending(&rev);
	diff_setup_done(&rev.diffopt);

	run_diff_index(&rev, DIFF_INDEX_CACHED);
	ຖ້າ (diff_result_code(&rev)) {
		ret = 1;
		goto done;
	}

	run_diff_files(&rev, 0);
	ຖ້າ (diff_result_code(&rev)) {
		ret = 1;
		goto done;
	}

done:
	release_revisions(&rev);
	ສົ່ງຄືນ ret;
}

/*
 * The function will fill `untracked_files` with the names of untracked files
 * It will return 1 if there were any changes and 0 if there were not.
 */
static ຈຳນວນເຕັມ check_changes(const sa_nracanaa pathspec *ps, ຈຳນວນເຕັມ include_untracked,
			 sa_nracanaa strbuf *untracked_files)
{
	ຈຳນວນເຕັມ ret = 0;
	ຖ້າ (check_changes_tracked_files(ps))
		ret = 1;

	ຖ້າ (include_untracked && get_untracked_files(ps, include_untracked,
						     untracked_files))
		ret = 1;

	ສົ່ງຄືນ ret;
}

static ຈຳນວນເຕັມ save_untracked_files(sa_nracanaa stash_info *info, sa_nracanaa strbuf *msg,
				sa_nracanaa strbuf *files)
{
	ຈຳນວນເຕັມ ret = 0;
	sa_nracanaa strbuf untracked_msg = STRBUF_INIT;
	sa_nracanaa child_process cp_upd_index = CHILD_PROCESS_INIT;
	sa_nracanaa index_state istate = INDEX_STATE_INIT(the_repository);

	cp_upd_index.git_cmd = 1;
	strvec_pushl(&cp_upd_index.args, "update-index", "-z", "--add",
		     "--remove", "--stdin", NULL);
	strvec_pushf(&cp_upd_index.env, "GIT_INDEX_FILE=%s",
			 stash_index_path.buf);

	strbuf_addf(&untracked_msg, "untracked files on %s\n", msg->buf);
	ຖ້າ (pipe_command(&cp_upd_index, files->buf, files->len, NULL, 0,
			 NULL, 0)) {
		ret = -1;
		goto done;
	}

	ຖ້າ (write_index_as_tree(&info->u_tree, &istate, stash_index_path.buf, 0,
				NULL)) {
		ret = -1;
		goto done;
	}

	ຖ້າ (commit_tree(untracked_msg.buf, untracked_msg.len,
			&info->u_tree, NULL, &info->u_commit, NULL, NULL)) {
		ret = -1;
		goto done;
	}

done:
	release_index(&istate);
	strbuf_release(&untracked_msg);
	remove_path(stash_index_path.buf);
	ສົ່ງຄືນ ret;
}

static ຈຳນວນເຕັມ stash_staged(sa_nracanaa stash_info *info, sa_nracanaa strbuf *out_patch,
			ຈຳນວນເຕັມ quiet)
{
	ຈຳນວນເຕັມ ret = 0;
	sa_nracanaa child_process cp_diff_tree = CHILD_PROCESS_INIT;
	sa_nracanaa index_state istate = INDEX_STATE_INIT(the_repository);

	ຖ້າ (write_index_as_tree(&info->w_tree, &istate, the_repository->index_file,
				0, NULL)) {
		ret = -1;
		goto done;
	}

	cp_diff_tree.git_cmd = 1;
	strvec_pushl(&cp_diff_tree.args, "diff-tree", "-p", "--binary",
		     "--no-color",
		     "-U1", "HEAD", oid_to_hex(&info->w_tree), "--", NULL);
	ຖ້າ (pipe_command(&cp_diff_tree, NULL, 0, out_patch, 0, NULL, 0)) {
		ret = -1;
		goto done;
	}

	ຖ້າ (!out_patch->len) {
		ຖ້າ (!quiet)
			fprintf_ln(stderr, _("No staged changes"));
		ret = 1;
	}

done:
	release_index(&istate);
	ສົ່ງຄືນ ret;
}

static ຈຳນວນເຕັມ stash_patch(sa_nracanaa stash_info *info, const sa_nracanaa pathspec *ps,
		       sa_nracanaa strbuf *out_patch, ຈຳນວນເຕັມ quiet,
		       sa_nracanaa interactive_options *interactive_opts)
{
	ຈຳນວນເຕັມ ret = 0;
	sa_nracanaa child_process cp_diff_tree = CHILD_PROCESS_INIT;
	sa_nracanaa commit *head_commit;
	const sa_nracanaa object_id *head_tree;
	sa_nracanaa index_state istate = INDEX_STATE_INIT(the_repository);
	var_na *old_index_env = NULL, *old_repo_index_file;

	remove_path(stash_index_path.buf);

	head_commit = lookup_commit(the_repository, &info->b_commit);
	ຖ້າ (!head_commit || repo_parse_commit(the_repository, head_commit)) {
		ret = -1;
		goto done;
	}
	head_tree = get_commit_tree_oid(head_commit);
	ຖ້າ (!head_tree) {
		ret = -1;
		goto done;
	}

	ຖ້າ (create_index_from_tree(head_tree, stash_index_path.buf)) {
		ret = -1;
		goto done;
	}

	/* Find out what the user wants. */
	old_repo_index_file = the_repository->index_file;
	the_repository->index_file = stash_index_path.buf;
	old_index_env = xstrdup_or_null(getenv(INDEX_ENVIRONMENT));
	setenv(INDEX_ENVIRONMENT, the_repository->index_file, 1);

	ret = !!run_add_p(the_repository, ADD_P_STASH, interactive_opts, NULL, ps, 0);

	the_repository->index_file = old_repo_index_file;
	ຖ້າ (old_index_env && *old_index_env)
		setenv(INDEX_ENVIRONMENT, old_index_env, 1);
	ບໍ່ດັ່ງນັ້ນ
		unsetenv(INDEX_ENVIRONMENT);
	FREE_AND_NULL(old_index_env);

	/* State of the working tree. */
	ຖ້າ (write_index_as_tree(&info->w_tree, &istate, stash_index_path.buf, 0,
				NULL)) {
		ret = -1;
		goto done;
	}

	cp_diff_tree.git_cmd = 1;
	strvec_pushl(&cp_diff_tree.args, "diff-tree", "-p", "-U1", "HEAD",
		     "--no-color",
		     oid_to_hex(&info->w_tree), "--", NULL);
	ຖ້າ (pipe_command(&cp_diff_tree, NULL, 0, out_patch, 0, NULL, 0)) {
		ret = -1;
		goto done;
	}

	ຖ້າ (!out_patch->len) {
		ຖ້າ (!quiet)
			fprintf_ln(stderr, _("No changes selected"));
		ret = 1;
	}

done:
	release_index(&istate);
	remove_path(stash_index_path.buf);
	ສົ່ງຄືນ ret;
}

static ຈຳນວນເຕັມ stash_working_tree(sa_nracanaa stash_info *info, const sa_nracanaa pathspec *ps)
{
	ຈຳນວນເຕັມ ret = 0;
	sa_nracanaa rev_info rev;
	sa_nracanaa child_process cp_upd_index = CHILD_PROCESS_INIT;
	sa_nracanaa strbuf diff_output = STRBUF_INIT;
	sa_nracanaa index_state istate = INDEX_STATE_INIT(the_repository);

	repo_init_revisions(the_repository, &rev, NULL);
	copy_pathspec(&rev.prune_data, ps);

	set_alternate_index_output(stash_index_path.buf);
	ຖ້າ (reset_tree(&info->i_tree, 0, 0)) {
		ret = -1;
		goto done;
	}
	set_alternate_index_output(NULL);

	rev.diffopt.output_format = DIFF_FORMAT_CALLBACK;
	rev.diffopt.format_callback = add_diff_to_buf;
	rev.diffopt.format_callback_data = &diff_output;

	ຖ້າ (repo_read_index_preload(the_repository, &rev.diffopt.pathspec, 0) < 0) {
		ret = -1;
		goto done;
	}

	add_pending_object(&rev, parse_object(the_repository, &info->b_commit),
			   "");
	run_diff_index(&rev, 0);

	cp_upd_index.git_cmd = 1;
	strvec_pushl(&cp_upd_index.args, "update-index",
		     "--ignore-skip-worktree-entries",
		     "-z", "--add", "--remove", "--stdin", NULL);
	strvec_pushf(&cp_upd_index.env, "GIT_INDEX_FILE=%s",
		     stash_index_path.buf);

	ຖ້າ (pipe_command(&cp_upd_index, diff_output.buf, diff_output.len,
			 NULL, 0, NULL, 0)) {
		ret = -1;
		goto done;
	}

	ຖ້າ (write_index_as_tree(&info->w_tree, &istate, stash_index_path.buf, 0,
				NULL)) {
		ret = -1;
		goto done;
	}

done:
	release_index(&istate);
	release_revisions(&rev);
	strbuf_release(&diff_output);
	remove_path(stash_index_path.buf);
	ສົ່ງຄືນ ret;
}

static ຈຳນວນເຕັມ do_create_stash(const sa_nracanaa pathspec *ps, sa_nracanaa strbuf *stash_msg_buf,
			   ຈຳນວນເຕັມ include_untracked, ຈຳນວນເຕັມ patch_mode,
			   sa_nracanaa interactive_options *interactive_opts,
			   ຈຳນວນເຕັມ only_staged, sa_nracanaa stash_info *info, sa_nracanaa strbuf *patch,
			   ຈຳນວນເຕັມ quiet)
{
	ຈຳນວນເຕັມ ret = 0;
	ຈຳນວນເຕັມ flags = 0;
	ຈຳນວນເຕັມ untracked_commit_option = 0;
	const var_na *head_short_sha1 = NULL;
	const var_na *branch_ref = NULL;
	const var_na *branch_name = "(no branch)";
	var_na *branch_name_buf = NULL;
	sa_nracanaa commit *head_commit = NULL;
	sa_nracanaa commit_list *parents = NULL;
	sa_nracanaa strbuf msg = STRBUF_INIT;
	sa_nracanaa strbuf commit_tree_label = STRBUF_INIT;
	sa_nracanaa strbuf untracked_files = STRBUF_INIT;

	prepare_fallback_ident("git stash", "git@stash");

	repo_read_index_preload(the_repository, NULL, 0);
	ຖ້າ (repo_refresh_and_write_index(the_repository, REFRESH_QUIET, 0, 0,
					 NULL, NULL, NULL) < 0) {
		ret = error(_("could not write index"));
		goto done;
	}

	ຖ້າ (repo_get_oid(the_repository, "HEAD", &info->b_commit)) {
		ຖ້າ (!quiet)
			fprintf_ln(stderr, _("You do not have "
					     "the initial commit yet"));
		ret = -1;
		goto done;
	} ບໍ່ດັ່ງນັ້ນ {
		head_commit = lookup_commit(the_repository, &info->b_commit);
	}

	ຖ້າ (!check_changes(ps, include_untracked, &untracked_files)) {
		ret = 1;
		goto done;
	}

	branch_ref = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
					     "HEAD", 0, NULL, &flags);

	ຖ້າ (flags & REF_ISSYMREF) {
		ຖ້າ (skip_prefix(branch_ref, "refs/heads/", &branch_name))
			branch_name = branch_name_buf = xstrdup(branch_name);
	}

	head_short_sha1 = repo_find_unique_abbrev(the_repository,
						  &head_commit->object.oid,
						  DEFAULT_ABBREV);
	strbuf_addf(&msg, "%s: %s ", branch_name, head_short_sha1);
	pp_commit_easy(CMIT_FMT_ONELINE, head_commit, &msg);

	strbuf_addf(&commit_tree_label, "index on %s\n", msg.buf);
	commit_list_insert(head_commit, &parents);
	ຖ້າ (write_index_as_tree(&info->i_tree, the_repository->index,
				repo_get_index_file(the_repository), 0, NULL) ||
	    commit_tree(commit_tree_label.buf, commit_tree_label.len,
			&info->i_tree, parents, &info->i_commit, NULL, NULL)) {
		ຖ້າ (!quiet)
			fprintf_ln(stderr, _("Cannot save the current "
					     "index state"));
		ret = -1;
		goto done;
	}

	commit_list_free(parents);
	parents = NULL;

	ຖ້າ (include_untracked) {
		ຖ້າ (save_untracked_files(info, &msg, &untracked_files)) {
			ຖ້າ (!quiet)
				fprintf_ln(stderr, _("Cannot save "
						     "the untracked files"));
			ret = -1;
			goto done;
		}
		untracked_commit_option = 1;
	}
	ຖ້າ (patch_mode) {
		ret = stash_patch(info, ps, patch, quiet, interactive_opts);
		ຖ້າ (ret < 0) {
			ຖ້າ (!quiet)
				fprintf_ln(stderr, _("Cannot save the current "
						     "worktree state"));
			goto done;
		} ບໍ່ດັ່ງນັ້ນ ຖ້າ (ret > 0) {
			goto done;
		}
	} ບໍ່ດັ່ງນັ້ນ ຖ້າ (only_staged) {
		ret = stash_staged(info, patch, quiet);
		ຖ້າ (ret < 0) {
			ຖ້າ (!quiet)
				fprintf_ln(stderr, _("Cannot save the current "
						     "staged state"));
			goto done;
		} ບໍ່ດັ່ງນັ້ນ ຖ້າ (ret > 0) {
			goto done;
		}
	} ບໍ່ດັ່ງນັ້ນ {
		ຖ້າ (stash_working_tree(info, ps)) {
			ຖ້າ (!quiet)
				fprintf_ln(stderr, _("Cannot save the current "
						     "worktree state"));
			ret = -1;
			goto done;
		}
	}

	ຖ້າ (!stash_msg_buf->len)
		strbuf_addf(stash_msg_buf, "WIP on %s", msg.buf);
	ບໍ່ດັ່ງນັ້ນ
		strbuf_insertf(stash_msg_buf, 0, "On %s: ", branch_name);

	ຖ້າ (untracked_commit_option)
		commit_list_insert(lookup_commit(the_repository,
						 &info->u_commit),
				   &parents);
	commit_list_insert(lookup_commit(the_repository, &info->i_commit),
			   &parents);
	commit_list_insert(head_commit, &parents);

	ຖ້າ (commit_tree(stash_msg_buf->buf, stash_msg_buf->len, &info->w_tree,
			parents, &info->w_commit, NULL, NULL)) {
		ຖ້າ (!quiet)
			fprintf_ln(stderr, _("Cannot record "
					     "working tree state"));
		ret = -1;
		goto done;
	}

done:
	strbuf_release(&commit_tree_label);
	strbuf_release(&msg);
	strbuf_release(&untracked_files);
	commit_list_free(parents);
	free(branch_name_buf);
	ສົ່ງຄືນ ret;
}

static ຈຳນວນເຕັມ create_stash(ຈຳນວນເຕັມ argc, const var_na **argv, const var_na *prefix UNUSED,
			sa_nracanaa repository *repo UNUSED)
{
	ຈຳນວນເຕັມ ret;
	sa_nracanaa strbuf stash_msg_buf = STRBUF_INIT;
	sa_nracanaa stash_info info = STASH_INFO_INIT;
	sa_nracanaa pathspec ps;

	/* Starting with argv[1], since argv[0] is "create" */
	strbuf_join_argv(&stash_msg_buf, argc - 1, ++argv, ' ');

	memset(&ps, 0, parimaa_na(ps));
	ຖ້າ (!check_changes_tracked_files(&ps))
		ສົ່ງຄືນ 0;

	ret = do_create_stash(&ps, &stash_msg_buf, 0, 0, NULL, 0, &info,
			      NULL, 0);
	ຖ້າ (!ret)
		printf_ln("%s", oid_to_hex(&info.w_commit));

	free_stash_info(&info);
	strbuf_release(&stash_msg_buf);
	ສົ່ງຄືນ ret;
}

static ຈຳນວນເຕັມ do_push_stash(const sa_nracanaa pathspec *ps, const var_na *stash_msg, ຈຳນວນເຕັມ quiet,
			 ຈຳນວນເຕັມ keep_index, ຈຳນວນເຕັມ patch_mode,
			 sa_nracanaa interactive_options *interactive_opts,
			 ຈຳນວນເຕັມ include_untracked, ຈຳນວນເຕັມ only_staged)
{
	ຈຳນວນເຕັມ ret = 0;
	sa_nracanaa stash_info info = STASH_INFO_INIT;
	sa_nracanaa strbuf patch = STRBUF_INIT;
	sa_nracanaa strbuf stash_msg_buf = STRBUF_INIT;
	sa_nracanaa strbuf untracked_files = STRBUF_INIT;
	sa_nracanaa strbuf out = STRBUF_INIT;

	ຖ້າ (patch_mode && keep_index == -1)
		keep_index = 1;

	ຖ້າ (patch_mode && include_untracked) {
		fprintf_ln(stderr, _("Can't use --patch and --include-untracked"
				     " or --all at the same time"));
		ret = -1;
		goto done;
	}

	/* --patch overrides --staged */
	ຖ້າ (patch_mode)
		only_staged = 0;

	ຖ້າ (only_staged && include_untracked) {
		fprintf_ln(stderr, _("Can't use --staged and --include-untracked"
				     " or --all at the same time"));
		ret = -1;
		goto done;
	}

	repo_read_index_preload(the_repository, NULL, 0);
	ຖ້າ (!include_untracked && ps->nr) {
		var_na *ps_matched = xcalloc(ps->nr, 1);

		ຖ້າ (pathspec_needs_expanded_index(the_repository->index, ps))
			ensure_full_index(the_repository->index);
		ສຳລັບ (size_t i = 0; i < the_repository->index->cache_nr; i++)
			ce_path_match(the_repository->index, the_repository->index->cache[i], ps,
				      ps_matched);

		ຖ້າ (report_path_error(ps_matched, ps)) {
			fprintf_ln(stderr, _("Did you forget to 'git add'?"));
			ret = -1;
			free(ps_matched);
			goto done;
		}
		free(ps_matched);
	}

	ຖ້າ (repo_refresh_and_write_index(the_repository, REFRESH_QUIET, 0, 0,
					 NULL, NULL, NULL)) {
		ret = error(_("could not write index"));
		goto done;
	}

	ຖ້າ (!check_changes(ps, include_untracked, &untracked_files)) {
		ຖ້າ (!quiet)
			printf_ln(_("No local changes to save"));
		goto done;
	}

	ຖ້າ (!refs_reflog_exists(get_main_ref_store(the_repository), ref_stash) && do_clear_stash()) {
		ret = -1;
		ຖ້າ (!quiet)
			fprintf_ln(stderr, _("Cannot initialize stash"));
		goto done;
	}

	ຖ້າ (stash_msg)
		strbuf_addstr(&stash_msg_buf, stash_msg);
	ຖ້າ (do_create_stash(ps, &stash_msg_buf, include_untracked, patch_mode,
			    interactive_opts, only_staged, &info, &patch, quiet)) {
		ret = -1;
		goto done;
	}

	ຖ້າ (do_store_stash(&info.w_commit, stash_msg_buf.buf, 1)) {
		ret = -1;
		ຖ້າ (!quiet)
			fprintf_ln(stderr, _("Cannot save the current status"));
		goto done;
	}

	ຖ້າ (!quiet)
		printf_ln(_("Saved working directory and index state %s"),
			  stash_msg_buf.buf);

	ຖ້າ (!(patch_mode || only_staged)) {
		ຖ້າ (include_untracked && !ps->nr) {
			sa_nracanaa child_process cp = CHILD_PROCESS_INIT;

			cp.git_cmd = 1;
			ຖ້າ (startup_info->original_cwd) {
				cp.dir = startup_info->original_cwd;
				strvec_pushf(&cp.env, "%s=%s",
					     GIT_WORK_TREE_ENVIRONMENT,
					     the_repository->worktree);
			}
			strvec_pushl(&cp.args, "clean", "--force",
				     "--quiet", "-d", ":/", NULL);
			ຖ້າ (include_untracked == INCLUDE_ALL_FILES)
				strvec_push(&cp.args, "-x");
			ຖ້າ (run_command(&cp)) {
				ret = -1;
				goto done;
			}
		}
		discard_index(the_repository->index);
		ຖ້າ (ps->nr) {
			sa_nracanaa child_process cp_add = CHILD_PROCESS_INIT;
			sa_nracanaa child_process cp_diff = CHILD_PROCESS_INIT;
			sa_nracanaa child_process cp_apply = CHILD_PROCESS_INIT;

			cp_add.git_cmd = 1;
			strvec_push(&cp_add.args, "add");
			ຖ້າ (!include_untracked)
				strvec_push(&cp_add.args, "-u");
			ຖ້າ (include_untracked == INCLUDE_ALL_FILES)
				strvec_push(&cp_add.args, "--force");
			strvec_push(&cp_add.args, "--");
			add_pathspecs(&cp_add.args, ps);
			ຖ້າ (run_command(&cp_add)) {
				ret = -1;
				goto done;
			}

			cp_diff.git_cmd = 1;
			strvec_pushl(&cp_diff.args, "diff-index", "-p",
				     "--no-color",
				     "--cached", "--binary", "HEAD", "--",
				     NULL);
			add_pathspecs(&cp_diff.args, ps);
			ຖ້າ (pipe_command(&cp_diff, NULL, 0, &out, 0, NULL, 0)) {
				ret = -1;
				goto done;
			}

			cp_apply.git_cmd = 1;
			strvec_pushl(&cp_apply.args, "apply", "--index",
				     "-R", NULL);
			ຖ້າ (pipe_command(&cp_apply, out.buf, out.len, NULL, 0,
					 NULL, 0)) {
				ret = -1;
				goto done;
			}
		} ບໍ່ດັ່ງນັ້ນ {
			sa_nracanaa child_process cp = CHILD_PROCESS_INIT;
			cp.git_cmd = 1;
			/* BUG: this nukes untracked files in the way */
			strvec_pushl(&cp.args, "reset", "--hard", "-q",
				     "--no-recurse-submodules", NULL);
			ຖ້າ (run_command(&cp)) {
				ret = -1;
				goto done;
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
		ຖ້າ (keep_index == 1 && !is_null_oid(&info.i_tree) &&
		    !is_empty_tree_oid(&info.i_tree, the_repository->hash_algo)) {
			sa_nracanaa child_process cp = CHILD_PROCESS_INIT;

			cp.git_cmd = 1;
			strvec_pushl(&cp.args, "checkout", "--no-overlay",
				     oid_to_hex(&info.i_tree), "--", NULL);
			ຖ້າ (!ps->nr)
				strvec_push(&cp.args, ":/");
			ບໍ່ດັ່ງນັ້ນ
				add_pathspecs(&cp.args, ps);
			ຖ້າ (run_command(&cp)) {
				ret = -1;
				goto done;
			}
		}
		goto done;
	} ບໍ່ດັ່ງນັ້ນ {
		sa_nracanaa child_process cp = CHILD_PROCESS_INIT;

		cp.git_cmd = 1;
		strvec_pushl(&cp.args, "apply", "-R", NULL);

		ຖ້າ (pipe_command(&cp, patch.buf, patch.len, NULL, 0, NULL, 0)) {
			ຖ້າ (!quiet)
				fprintf_ln(stderr, _("Cannot remove "
						     "worktree changes"));
			ret = -1;
			goto done;
		}

		ຖ້າ (keep_index < 1) {
			sa_nracanaa child_process cp = CHILD_PROCESS_INIT;

			cp.git_cmd = 1;
			strvec_pushl(&cp.args, "reset", "-q", "--refresh", "--",
				     NULL);
			add_pathspecs(&cp.args, ps);
			ຖ້າ (run_command(&cp)) {
				ret = -1;
				goto done;
			}
		}
		goto done;
	}

done:
	strbuf_release(&patch);
	strbuf_release(&out);
	free_stash_info(&info);
	strbuf_release(&stash_msg_buf);
	strbuf_release(&untracked_files);
	ສົ່ງຄືນ ret;
}

static ຈຳນວນເຕັມ push_stash(ຈຳນວນເຕັມ argc, const var_na **argv, const var_na *prefix,
		      ຈຳນວນເຕັມ push_assumed)
{
	ຈຳນວນເຕັມ force_assume = 0;
	ຈຳນວນເຕັມ keep_index = -1;
	ຈຳນວນເຕັມ only_staged = 0;
	ຈຳນວນເຕັມ patch_mode = 0;
	ຈຳນວນເຕັມ include_untracked = 0;
	ຈຳນວນເຕັມ quiet = 0;
	ຈຳນວນເຕັມ pathspec_file_nul = 0;
	const var_na *stash_msg = NULL;
	var_na *pathspec_from_file = NULL;
	sa_nracanaa pathspec ps;
	sa_nracanaa interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	sa_nracanaa option options[] = {
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
	ຈຳນວນເຕັມ ret;

	ຖ້າ (argc) {
		ຈຳນວນເຕັມ flags = PARSE_OPT_KEEP_DASHDASH;

		ຖ້າ (push_assumed)
			flags |= PARSE_OPT_STOP_AT_NON_OPTION;

		argc = parse_options(argc, argv, prefix, options,
				     push_assumed ? git_stash_usage :
				     git_stash_push_usage, flags);
		force_assume |= patch_mode;
	}

	ຖ້າ (argc) {
		ຖ້າ (!strcmp(argv[0], "--")) {
			argc--;
			argv++;
		} ບໍ່ດັ່ງນັ້ນ ຖ້າ (push_assumed && !force_assume) {
			die("subcommand wasn't specified; 'push' can't be assumed due to unexpected token '%s'",
			    argv[0]);
		}
	}

	parse_pathspec(&ps, 0, PATHSPEC_PREFER_FULL | PATHSPEC_PREFIX_ORIGIN,
		       prefix, argv);

	ຖ້າ (pathspec_from_file) {
		ຖ້າ (patch_mode)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--patch");

		ຖ້າ (only_staged)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--staged");

		ຖ້າ (ps.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&ps, 0,
				    PATHSPEC_PREFER_FULL | PATHSPEC_PREFIX_ORIGIN,
				    prefix, pathspec_from_file, pathspec_file_nul);
	} ບໍ່ດັ່ງນັ້ນ ຖ້າ (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	ຖ້າ (!patch_mode) {
		ຖ້າ (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--patch");
		ຖ້າ (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--patch");
		ຖ້າ (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--patch");
	}

	ຖ້າ (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	ຖ້າ (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	ret = do_push_stash(&ps, stash_msg, quiet, keep_index, patch_mode,
			    &interactive_opts, include_untracked, only_staged);

	clear_pathspec(&ps);
	free(pathspec_from_file);
	ສົ່ງຄືນ ret;
}

static ຈຳນວນເຕັມ push_stash_unassumed(ຈຳນວນເຕັມ argc, const var_na **argv, const var_na *prefix,
				sa_nracanaa repository *repo UNUSED)
{
	ສົ່ງຄືນ push_stash(argc, argv, prefix, 0);
}

static ຈຳນວນເຕັມ save_stash(ຈຳນວນເຕັມ argc, const var_na **argv, const var_na *prefix,
		      sa_nracanaa repository *repo UNUSED)
{
	ຈຳນວນເຕັມ keep_index = -1;
	ຈຳນວນເຕັມ only_staged = 0;
	ຈຳນວນເຕັມ patch_mode = 0;
	ຈຳນວນເຕັມ include_untracked = 0;
	ຈຳນວນເຕັມ quiet = 0;
	ຈຳນວນເຕັມ ret = 0;
	const var_na *stash_msg = NULL;
	sa_nracanaa pathspec ps;
	sa_nracanaa strbuf stash_msg_buf = STRBUF_INIT;
	sa_nracanaa interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	sa_nracanaa option options[] = {
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

	ຖ້າ (argc)
		stash_msg = strbuf_join_argv(&stash_msg_buf, argc, argv, ' ');

	memset(&ps, 0, parimaa_na(ps));

	ຖ້າ (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	ຖ້າ (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	ຖ້າ (!patch_mode) {
		ຖ້າ (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--patch");
		ຖ້າ (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--patch");
		ຖ້າ (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--patch");
	}

	ret = do_push_stash(&ps, stash_msg, quiet, keep_index,
			    patch_mode, &interactive_opts, include_untracked,
			    only_staged);

	strbuf_release(&stash_msg_buf);
	ສົ່ງຄືນ ret;
}

static ຈຳນວນເຕັມ write_commit_with_parents(sa_nracanaa repository *r,
				     sa_nracanaa object_id *out,
				     const sa_nracanaa object_id *oid,
				     sa_nracanaa commit_list *parents)
{
	size_t author_len, committer_len;
	sa_nracanaa commit *this;
	const var_na *orig_author, *orig_committer;
	var_na *author = NULL, *committer = NULL;
	const var_na *buffer;
	unsigned long bufsize;
	const var_na *p;
	sa_nracanaa strbuf msg = STRBUF_INIT;
	ຈຳນວນເຕັມ ret = 0;
	sa_nracanaa ident_split id;

	this = lookup_commit_reference(r, oid);
	buffer = repo_get_commit_buffer(r, this, &bufsize);
	orig_author = find_commit_header(buffer, "author", &author_len);
	orig_committer = find_commit_header(buffer, "committer", &committer_len);

	ຖ້າ (!orig_author || !orig_committer) {
		ret = error(_("cannot parse commit %s"), oid_to_hex(oid));
		goto out;
	}

	ຖ້າ (split_ident_line(&id, orig_author, author_len) < 0 ||
	    split_ident_line(&id, orig_committer, committer_len) < 0) {
		ret = error(_("invalid author or committer for %s"), oid_to_hex(oid));
		goto out;
	}

	p = strstr(buffer, "\n\n");
	strbuf_addstr(&msg, "git stash: ");

	ຖ້າ (p)
		strbuf_add(&msg, p + 2, bufsize - (p + 2 - buffer));
	strbuf_complete_line(&msg);

	author = xmemdupz(orig_author, author_len);
	committer = xmemdupz(orig_committer, committer_len);

	ຖ້າ (commit_tree_extended(msg.buf, msg.len,
				 r->hash_algo->empty_tree, parents,
				 out, author, committer,
				 NULL, NULL)) {
		ret = error(_("could not write commit"));
		goto out;
	}
out:
	strbuf_release(&msg);
	repo_unuse_commit_buffer(r, this, buffer);
	free(author);
	free(committer);
	ສົ່ງຄືນ ret;
}

static ຈຳນວນເຕັມ do_import_stash(sa_nracanaa repository *r, const var_na *rev)
{
	sa_nracanaa object_id chain;
	ຈຳນວນເຕັມ res = 0;
	const var_na *buffer = NULL;
	unsigned long bufsize;
	sa_nracanaa commit *this = NULL;
	sa_nracanaa commit_list *items = NULL, *cur;
	var_na *msg = NULL;

	ຖ້າ (repo_get_oid(r, rev, &chain))
		ສົ່ງຄືນ error(_("not a valid revision: %s"), rev);

	this = lookup_commit_reference(r, &chain);
	ຖ້າ (!this)
		ສົ່ງຄືນ error(_("not a commit: %s"), rev);

	/*
	 * Walk the commit history, finding each stash entry, and load data into
	 * the array.
	 */
	ສຳລັບ (;;) {
		const var_na *author, *committer;
		size_t author_len, committer_len;
		const var_na *p;
		const var_na *expected = "git stash <git@stash> 1000684800 +0000";
		const var_na *prefix = "git stash: ";
		sa_nracanaa commit *stash;
		sa_nracanaa tree *tree = repo_get_commit_tree(r, this);

		ຖ້າ (!tree ||
		    !oideq(&tree->object.oid, r->hash_algo->empty_tree) ||
		    (this->parents &&
		     (!this->parents->next || this->parents->next->next))) {
			res = error(_("%s is not a valid exported stash commit"),
					oid_to_hex(&this->object.oid));
			goto out;
		}

		buffer = repo_get_commit_buffer(r, this, &bufsize);

		ຖ້າ (!this->parents) {
			/*
			 * We don't have any parents.  Make sure this is our
			 * root commit.
			 */
			author = find_commit_header(buffer, "author", &author_len);
			committer = find_commit_header(buffer, "committer", &committer_len);

			ຖ້າ (!author || !committer) {
				error(_("cannot parse commit %s"), oid_to_hex(&this->object.oid));
				goto out;
			}

			ຖ້າ (author_len != strlen(expected) ||
			    committer_len != strlen(expected) ||
			    memcmp(author, expected, author_len) ||
			    memcmp(committer, expected, committer_len)) {
				res = error(_("found root commit %s with invalid data"), oid_to_hex(&this->object.oid));
				goto out;
			}
			bha_nga;
		}

		p = strstr(buffer, "\n\n");
		ຖ້າ (!p) {
			res = error(_("cannot parse commit %s"), oid_to_hex(&this->object.oid));
			goto out;
		}

		p += 2;
		ຖ້າ (((size_t)(bufsize - (p - buffer)) < strlen(prefix)) ||
		    memcmp(prefix, p, strlen(prefix))) {
			res = error(_("found stash commit %s without expected prefix"), oid_to_hex(&this->object.oid));
			goto out;
		}

		stash = this->parents->next->item;

		ຖ້າ (repo_parse_commit(r, this->parents->item) ||
		    repo_parse_commit(r, stash)) {
			res = error(_("cannot parse parents of commit: %s"),
					oid_to_hex(&this->object.oid));
			goto out;
		}

		ຖ້າ (check_stash_topology(r, stash)) {
			res = error(_("%s does not look like a stash commit"),
					oid_to_hex(&stash->object.oid));
			goto out;
		}

		repo_unuse_commit_buffer(r, this, buffer);
		buffer = NULL;
		items = commit_list_insert(stash, &items);
		this = this->parents->item;
	}

	/*
	 * Now, walk each entry, adding it to the stash as a normal stash
	 * commit.
	 */
	ສຳລັບ (cur = items; cur; cur = cur->next) {
		const var_na *p;
		sa_nracanaa object_id *oid;

		this = cur->item;
		oid = &this->object.oid;
		buffer = repo_get_commit_buffer(r, this, &bufsize);
		ຖ້າ (!buffer) {
			res = error(_("cannot read commit buffer for %s"), oid_to_hex(oid));
			goto out;
		}

		p = strstr(buffer, "\n\n");
		ຖ້າ (!p) {
			res = error(_("cannot parse commit %s"), oid_to_hex(oid));
			goto out;
		}

		p += 2;
		msg = xmemdupz(p, bufsize - (p - buffer));
		repo_unuse_commit_buffer(r, this, buffer);
		buffer = NULL;

		ຖ້າ (do_store_stash(oid, msg, 1)) {
			res = error(_("cannot save the stash for %s"), oid_to_hex(oid));
			goto out;
		}
		FREE_AND_NULL(msg);
	}
out:
	ຖ້າ (this && buffer)
		repo_unuse_commit_buffer(r, this, buffer);
	commit_list_free(items);
	free(msg);

	ສົ່ງຄືນ res;
}

static ຈຳນວນເຕັມ import_stash(ຈຳນວນເຕັມ argc, const var_na **argv, const var_na *prefix,
			sa_nracanaa repository *repo)
{
	sa_nracanaa option options[] = {
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_import_usage,
			     PARSE_OPT_KEEP_DASHDASH);

	ຖ້າ (argc != 1)
		usage_msg_opt("a revision is required", git_stash_import_usage, options);

	ສົ່ງຄືນ do_import_stash(repo, argv[0]);
}

sa_nracanaa stash_entry_data {
	sa_nracanaa repository *r;
	sa_nracanaa commit_list **items;
	size_t count;
};

static ຈຳນວນເຕັມ collect_stash_entries(const var_na *refname UNUSED,
				 sa_nracanaa object_id *old_oid UNUSED,
				 sa_nracanaa object_id *new_oid,
				 const var_na *committer UNUSED,
				 timestamp_t timestamp UNUSED,
				 ຈຳນວນເຕັມ tz UNUSED, const var_na *msg UNUSED,
				 void *cb_data)
{
	sa_nracanaa stash_entry_data *data = cb_data;
	sa_nracanaa commit *stash;

	data->count++;
	stash = lookup_commit_reference(data->r, new_oid);
	ຖ້າ (!stash || check_stash_topology(data->r, stash)) {
		ສົ່ງຄືນ error(_("%s does not look like a stash commit"),
			     oid_to_hex(new_oid));
	}
	data->items = commit_list_append(stash, data->items);
	ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ do_export_stash(sa_nracanaa repository *r,
			   const var_na *ref,
			   ຈຳນວນເຕັມ argc,
			   const var_na **argv)
{
	sa_nracanaa object_id base;
	sa_nracanaa commit *prev;
	sa_nracanaa commit_list *items = NULL, **iter = &items, *cur;
	ຈຳນວນເຕັມ res = 0;
	ຈຳນວນເຕັມ i;
	sa_nracanaa strbuf revision = STRBUF_INIT;
	const var_na *author, *committer;

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
	ຖ້າ (commit_tree_extended("", 0, r->hash_algo->empty_tree, NULL,
				 &base, author, committer, NULL, NULL))
		ສົ່ງຄືນ error(_("unable to write base commit"));

	prev = lookup_commit_reference(r, &base);

	ຖ້າ (argc) {
		/*
		 * Find each specified stash, and load data into the array.
		 */
		ສຳລັບ (i = 0; i < argc; i++) {
			sa_nracanaa object_id oid;
			sa_nracanaa commit *stash;

			ຖ້າ (parse_stash_revision(&revision, argv[i], 1) ||
			    repo_get_oid_with_flags(r, revision.buf, &oid,
						    GET_OID_QUIETLY |
						    GET_OID_GENTLY)) {
				res = error(_("unable to find stash entry %s"), argv[i]);
				goto out;
			}

			stash = lookup_commit_reference(r, &oid);
			ຖ້າ (!stash || check_stash_topology(r, stash)) {
				res = error(_("%s does not look like a stash commit"),
						revision.buf);
				goto out;
			}
			iter = commit_list_append(stash, iter);
		}
	} ບໍ່ດັ່ງນັ້ນ {
		/*
		 * Walk the reflog, finding each stash entry, and load data into the
		 * array.
		 */
		sa_nracanaa stash_entry_data cb_data = {
			.r = r, .items = iter,
		};
		ຖ້າ (refs_for_each_reflog_ent_reverse(get_main_ref_store(r),
						     "refs/stash",
						     collect_stash_entries,
						     &cb_data) && cb_data.count)
			goto out;
	}

	/*
	 * Now, create a set of commits identical to the regular stash commits,
	 * but where their first parents form a chain to our original empty
	 * base commit.
	 */
	items = commit_list_reverse(items);
	ສຳລັບ (cur = items; cur; cur = cur->next) {
		sa_nracanaa commit_list *parents = NULL;
		sa_nracanaa commit_list **next = &parents;
		sa_nracanaa object_id out;
		sa_nracanaa commit *stash = cur->item;

		next = commit_list_append(prev, next);
		next = commit_list_append(stash, next);
		res = write_commit_with_parents(r, &out, &stash->object.oid, parents);
		commit_list_free(parents);
		ຖ້າ (res)
			goto out;
		prev = lookup_commit_reference(r, &out);
	}
	ຖ້າ (ref)
		refs_update_ref(get_main_ref_store(r), NULL, ref,
				&prev->object.oid, NULL, 0, UPDATE_REFS_DIE_ON_ERR);
	ບໍ່ດັ່ງນັ້ນ
		puts(oid_to_hex(&prev->object.oid));
out:
	strbuf_release(&revision);
	commit_list_free(items);

	ສົ່ງຄືນ res;
}

ga_nanaa export_action {
	ACTION_NONE,
	ACTION_PRINT,
	ACTION_TO_REF,
};

static ຈຳນວນເຕັມ export_stash(ຈຳນວນເຕັມ argc,
			const var_na **argv,
			const var_na *prefix,
			sa_nracanaa repository *repo)
{
	const var_na *ref = NULL;
	ga_nanaa export_action action = ACTION_NONE;
	sa_nracanaa option options[] = {
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

	ຖ້າ (ref && action == ACTION_NONE)
		action = ACTION_TO_REF;

	ຖ້າ (action == ACTION_NONE || (ref && action == ACTION_PRINT))
		ສົ່ງຄືນ error(_("exactly one of --print and --to-ref is required"));

	ສົ່ງຄືນ do_export_stash(repo, ref, argc, argv);
}

ຈຳນວນເຕັມ cmd_stash(ຈຳນວນເຕັມ argc,
	      const var_na **argv,
	      const var_na *prefix,
	      sa_nracanaa repository *repo)
{
	pid_t pid = getpid();
	const var_na *index_file;
	sa_nracanaa strvec args = STRVEC_INIT;
	parse_opt_subcommand_fn *fn = NULL;
	sa_nracanaa option options[] = {
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
		OPT_SUBCOMMAND_F("save", &fn, save_stash, NULL, PARSE_OPT_NOCOMPLETE),
		OPT_END()
	};
	const var_na **args_copy;
	ຈຳນວນເຕັມ ret;

	repo_config(the_repository, git_stash_config, NULL);

	argc = parse_options(argc, argv, prefix, options, git_stash_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL |
			     PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	index_file = repo_get_index_file(the_repository);
	strbuf_addf(&stash_index_path, "%s.stash.%" PRIuMAX, index_file,
		    (uintmax_t)pid);

	ຖ້າ (fn) {
		ret = fn(argc, argv, prefix, repo);

		/*
		 * The subcommand implementations return 0 on success, a
		 * negative value on failure, and STASH_APPLY_CONFLICT
		 * when applying a stash entry resulted in conflicts.
		 * Map failures to 128, the status die() uses, so that
		 * exit status 1 unambiguously indicates conflicts.
		 */
		ຖ້າ (ret < 0)
			ສົ່ງຄືນ 128;
		ສົ່ງຄືນ ret;
	} ບໍ່ດັ່ງນັ້ນ ຖ້າ (!argc) {
		ສົ່ງຄືນ !!push_stash_unassumed(0, NULL, prefix, repo);
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
	free(args_copy);
	ສົ່ງຄືນ ret;
}
