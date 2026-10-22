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

static const char * const git_stash_usage[] = {
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

static const char * const git_stash_list_usage[] = {
	BUILTIN_STASH_LIST_USAGE,
	NULL
};

static const char * const git_stash_show_usage[] = {
	BUILTIN_STASH_SHOW_USAGE,
	NULL
};

static const char * const git_stash_drop_usage[] = {
	BUILTIN_STASH_DROP_USAGE,
	NULL
};

static const char * const git_stash_pop_usage[] = {
	BUILTIN_STASH_POP_USAGE,
	NULL
};

static const char * const git_stash_apply_usage[] = {
	BUILTIN_STASH_APPLY_USAGE,
	NULL
};

static const char * const git_stash_branch_usage[] = {
	BUILTIN_STASH_BRANCH_USAGE,
	NULL
};

static const char * const git_stash_clear_usage[] = {
	BUILTIN_STASH_CLEAR_USAGE,
	NULL
};

static const char * const git_stash_store_usage[] = {
	BUILTIN_STASH_STORE_USAGE,
	NULL
};

static const char * const git_stash_push_usage[] = {
	BUILTIN_STASH_PUSH_USAGE,
	NULL
};

static const char * const git_stash_save_usage[] = {
	BUILTIN_STASH_SAVE_USAGE,
	NULL
};

static const char * const git_stash_export_usage[] = {
	BUILTIN_STASH_EXPORT_USAGE,
	NULL
};

static const char * const git_stash_import_usage[] = {
	BUILTIN_STASH_IMPORT_USAGE,
	NULL
};

static const char ref_stash[] = "refs/stash";
static struct strbuf stash_index_path = STRBUF_INIT;

static ціле show_stat = 1;
static ціле show_patch;
static ціле show_include_untracked;
static ціле use_index;

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
struct stash_info {
	struct object_id w_commit;
	struct object_id b_commit;
	struct object_id i_commit;
	struct object_id u_commit;
	struct object_id c_commit;
	struct object_id w_tree;
	struct object_id b_tree;
	struct object_id i_tree;
	struct object_id u_tree;
	struct strbuf revision;
	ціле is_stash_ref;
	ціле has_u;
};

#define STASH_INFO_INIT { \
	.revision = STRBUF_INIT, \
}

static void free_stash_info(struct stash_info *info)
{
	strbuf_release(&info->revision);
}

static ціле check_stash_topology(struct repository *r, struct commit *stash)
{
	struct commit *p1, *p2, *p3 = NULL;

	/* stash must have two or three parents */
	якщо (!stash->parents || !stash->parents->next ||
			(stash->parents->next->next && stash->parents->next->next->next))
		повернути -1;
	p1 = stash->parents->item;
	p2 = stash->parents->next->item;
	якщо (stash->parents->next->next)
		p3 = stash->parents->next->next->item;
	якщо (repo_parse_commit(r, p1) || repo_parse_commit(r, p2) ||
			(p3 && repo_parse_commit(r, p3)))
		повернути -1;
	/* p2 must have a single parent, p3 must have no parents */
	якщо (!p2->parents || p2->parents->next || (p3 && p3->parents))
		повернути -1;
	якщо (repo_parse_commit(r, p2->parents->item))
		повернути -1;
	/* p2^1 must equal p1 */
	якщо (!oideq(&p1->object.oid, &p2->parents->item->object.oid))
		повернути -1;

	повернути 0;
}

static void assert_stash_like(struct stash_info *info, const char *revision)
{
	якщо (get_oidf(&info->b_commit, "%s^1", revision) ||
	    get_oidf(&info->w_tree, "%s:", revision) ||
	    get_oidf(&info->b_tree, "%s^1:", revision) ||
	    get_oidf(&info->i_tree, "%s^2:", revision))
		die(_("'%s' is not a stash-like commit"), revision);
}

static ціле parse_stash_revision(struct strbuf *revision, const char *commit, ціле quiet)
{
	strbuf_reset(revision);
	якщо (!commit) {
		якщо (!refs_ref_exists(get_main_ref_store(the_repository), ref_stash)) {
			якщо (!quiet)
				fprintf_ln(stderr, _("No stash entries found."));
			повернути -1;
		}

		strbuf_addf(revision, "%s@{0}", ref_stash);
	} інакше якщо (strspn(commit, "0123456789") == strlen(commit)) {
		strbuf_addf(revision, "%s@{%s}", ref_stash, commit);
	} інакше {
		strbuf_addstr(revision, commit);
	}
	повернути 0;
}

static ціле get_stash_info(struct stash_info *info, ціле argc, const char **argv)
{
	ціле ret;
	char *end_of_rev;
	char *expanded_ref;
	const char *revision;
	const char *commit = NULL;
	struct object_id dummy;
	struct strbuf symbolic = STRBUF_INIT;

	якщо (argc > 1) {
		ціле i;
		struct strbuf refs_msg = STRBUF_INIT;

		для (i = 0; i < argc; i++)
			strbuf_addf(&refs_msg, " '%s'", argv[i]);

		fprintf_ln(stderr, _("Too many revisions specified:%s"),
			   refs_msg.buf);
		strbuf_release(&refs_msg);

		повернути -1;
	}

	якщо (argc == 1)
		commit = argv[0];

	strbuf_init(&info->revision, 0);
	якщо (parse_stash_revision(&info->revision, commit, 0)) {
		повернути -1;
	}

	revision = info->revision.buf;

	якщо (repo_get_oid(the_repository, revision, &info->w_commit))
		повернути error(_("%s is not a valid reference"), revision);

	assert_stash_like(info, revision);

	info->has_u = !get_oidf(&info->u_tree, "%s^3:", revision);

	end_of_rev = strchrnul(revision, '@');
	strbuf_add(&symbolic, revision, end_of_rev - revision);

	ret = repo_dwim_ref(the_repository, symbolic.buf, symbolic.len,
			    &dummy, &expanded_ref, 0);
	strbuf_release(&symbolic);
	switch (ret) {
	case 0: /* Not found, but valid ref */
		info->is_stash_ref = 0;
		break;
	case 1:
		info->is_stash_ref = !strcmp(expanded_ref, ref_stash);
		break;
	default: /* Invalid or ambiguous */
		break;
	}

	free(expanded_ref);
	повернути !(ret == 0 || ret == 1);
}

static ціле do_clear_stash(void)
{
	struct object_id obj;
	якщо (repo_get_oid(the_repository, ref_stash, &obj))
		повернути 0;

	повернути refs_delete_ref(get_main_ref_store(the_repository), NULL,
			       ref_stash, &obj, 0);
}

static ціле clear_stash(ціле argc, const char **argv, const char *prefix,
		       struct repository *repo UNUSED)
{
	struct option options[] = {
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_clear_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	якщо (argc)
		повернути error(_("git stash clear with arguments is "
			       "unimplemented"));

	повернути do_clear_stash();
}

static ціле reset_tree(struct object_id *i_tree, ціле update, ціле reset)
{
	ціле nr_trees = 1;
	struct unpack_trees_options opts;
	struct tree_desc t[MAX_UNPACK_TREES];
	struct tree *tree;
	struct lock_file lock_file = LOCK_INIT;

	repo_read_index_preload(the_repository, NULL, 0);
	якщо (refresh_index(the_repository->index, REFRESH_QUIET, NULL, NULL, NULL))
		повернути -1;

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);

	memset(&opts, 0, sizeof(opts));

	tree = repo_parse_tree_indirect(the_repository, i_tree);
	якщо (repo_parse_tree(the_repository, tree))
		повернути -1;

	init_tree_desc(t, &tree->object.oid, tree->buffer, tree->size);

	opts.head_idx = 1;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;
	opts.merge = 1;
	opts.reset = reset ? UNPACK_RESET_PROTECT_UNTRACKED : 0;
	opts.update = update;
	якщо (update)
		opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
	opts.fn = oneway_merge;

	якщо (unpack_trees(nr_trees, t, &opts))
		повернути -1;

	якщо (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
		повернути error(_("unable to write new index file"));

	повернути 0;
}

static ціле create_index_from_tree(const struct object_id *tree_id,
				  const char *index_path)
{
	ціле nr_trees = 1;
	ціле ret = 0;
	struct unpack_trees_options opts;
	struct tree_desc t[MAX_UNPACK_TREES];
	struct tree *tree;
	struct index_state dst_istate = INDEX_STATE_INIT(the_repository);
	struct lock_file lock_file = LOCK_INIT;

	repo_read_index_preload(the_repository, NULL, 0);
	refresh_index(the_repository->index, REFRESH_QUIET, NULL, NULL, NULL);

	hold_lock_file_for_update(&lock_file, index_path, LOCK_DIE_ON_ERROR);

	memset(&opts, 0, sizeof(opts));

	tree = repo_parse_tree_indirect(the_repository, tree_id);
	якщо (!tree || repo_parse_tree(the_repository, tree)) {
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

	якщо (unpack_trees(nr_trees, t, &opts)) {
		ret = -1;
		goto done;
	}

	якщо (write_locked_index(&dst_istate, &lock_file, COMMIT_LOCK)) {
		ret = error(_("unable to write new index file"));
		goto done;
	}

done:
	release_index(&dst_istate);
	якщо (ret)
		rollback_lock_file(&lock_file);
	повернути ret;
}

static ціле is_path_a_directory(const char *path)
{
	/*
	 * This function differs from abspath.c:is_directory() in that
	 * here we use lstat() instead of stat(); we do not want to
	 * follow symbolic links here.
	 */
	struct stat st;
	повернути (!lstat(path, &st) && S_ISDIR(st.st_mode));
}

static void add_diff_to_buf(struct diff_queue_struct *q,
			    struct diff_options *options UNUSED,
			    void *data)
{
	ціле i;

	для (i = 0; i < q->nr; i++) {
		якщо (is_path_a_directory(q->queue[i]->one->path))
			continue;

		strbuf_addstr(data, q->queue[i]->one->path);

		/* NUL-terminate: will be fed to update-index -z */
		strbuf_addch(data, '\0');
	}
}

static ціле restore_untracked(struct object_id *u_tree)
{
	ціле res;
	struct child_process cp = CHILD_PROCESS_INIT;

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
	якщо (run_command(&cp)) {
		remove_path(stash_index_path.buf);
		повернути -1;
	}

	child_process_init(&cp);
	cp.git_cmd = 1;
	strvec_pushl(&cp.args, "checkout-index", "--all", NULL);
	strvec_pushf(&cp.env, "GIT_INDEX_FILE=%s",
		     stash_index_path.buf);

	res = run_command(&cp);
	remove_path(stash_index_path.buf);
	повернути res;
}

static void unstage_changes_unless_new(struct object_id *orig_tree)
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

	struct checkout state = CHECKOUT_INIT;
	struct diff_options diff_opts;
	struct lock_file lock = LOCK_INIT;
	ціле i;

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
	для (i = 0; i < diff_queued_diff.nr; i++) {
		struct diff_filepair *p;
		struct cache_entry *ce;
		ціле pos;

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
		якщо (pos >= 0 && ce_skip_worktree(the_repository->index->cache[pos])) {
			struct stat st;

			ce = the_repository->index->cache[pos];
			якщо (!lstat(ce->name, &st)) {
				/* Conflicting path present; relocate it */
				struct strbuf new_path = STRBUF_INIT;
				ціле fd;

				strbuf_addf(&new_path,
					    "%s.stash.XXXXXX", ce->name);
				fd = xmkstemp(new_path.buf);
				close(fd);
				printf(_("WARNING: Untracked file in way of "
					 "tracked file!  Renaming\n "
					 "           %s -> %s\n"
					 "         to make room.\n"),
				       ce->name, new_path.buf);
				якщо (rename(ce->name, new_path.buf))
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
		якщо (p->one->oid_valid) {
			/*
			 * Path existed in orig_tree; restore index entry
			 * from that tree in order to "unstage" the changes.
			 */
			ціле option = ADD_CACHE_OK_TO_REPLACE;
			якщо (pos < 0)
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
	якщо (write_locked_index(the_repository->index, &lock,
			       COMMIT_LOCK | SKIP_IF_UNCHANGED))
		die(_("could not write index"));
}

static enum stash_apply_result do_apply_stash(const char *prefix,
					      struct stash_info *info,
					      ціле index, ціле quiet,
					      const char *label_ours,
					      const char *label_theirs,
					      const char *label_base)
{
	ціле clean, ret;
	ціле has_index = index;
	struct merge_options o;
	struct object_id c_tree;
	struct object_id index_tree;
	struct tree *head, *merge, *merge_base;
	struct lock_file lock = LOCK_INIT;

	repo_read_index_preload(the_repository, NULL, 0);
	якщо (repo_refresh_and_write_index(the_repository, REFRESH_QUIET, 0, 0,
					 NULL, NULL, NULL))
		повернути error(_("could not write index"));

	якщо (write_index_as_tree(&c_tree, the_repository->index,
				repo_get_index_file(the_repository), 0, NULL))
		повернути error(_("cannot apply a stash in the middle of a merge"));

	init_ui_merge_options(&o, the_repository);

	якщо (quiet)
		o.verbosity = 0;

	якщо (index) {
		якщо (oideq(&info->b_tree, &info->i_tree) ||
		    oideq(&c_tree, &info->i_tree)) {
			has_index = 0;
		} інакше {
			struct merge_result result = { 0 };

			o.branch1 = "Current index";
			o.branch2 = "Stashed index changes";
			o.ancestor = "Stash base";

			head = lookup_tree(o.repo, &c_tree);
			merge = lookup_tree(o.repo, &info->i_tree);
			merge_base = lookup_tree(o.repo, &info->b_tree);

			merge_incore_nonrecursive(&o, merge_base, head, merge,
						  &result);

			якщо (result.clean < 0) {
				merge_finalize(&o, &result);
				повернути error(_("index merge failed"));
			} інакше якщо (!result.clean) {
				merge_finalize(&o, &result);
				повернути error(_("conflicts in index. "
					       "Try without --index."));
			} інакше {
				oidcpy(&index_tree, &result.tree->object.oid);
				merge_finalize(&o, &result);
			}
		}
	}

	o.branch1 = label_ours ? label_ours : "Updated upstream";
	o.branch2 = label_theirs ? label_theirs : "Stashed changes";
	o.ancestor = label_base ? label_base : "Stash base";

	якщо (oideq(&info->b_tree, &c_tree))
		o.branch1 = "Version stash was based on";

	якщо (o.verbosity >= 3)
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

	якщо (ret < 0)
		rollback_lock_file(&lock);
	інакше якщо (write_locked_index(o.repo->index, &lock,
				      COMMIT_LOCK | SKIP_IF_UNCHANGED))
		ret = error(_("could not write index"));

	якщо (ret) {
		repo_rerere(the_repository, 0);

		якщо (index)
			fprintf_ln(stderr, _("Index was not unstashed."));

		goto restore_untracked;
	}

	якщо (has_index) {
		якщо (reset_tree(&index_tree, 0, 0))
			ret = -1;
	} інакше {
		unstage_changes_unless_new(&c_tree);
	}

restore_untracked:
	якщо (info->has_u && restore_untracked(&info->u_tree))
		ret = error(_("could not restore untracked files from stash"));

	якщо (!quiet) {
		struct child_process cp = CHILD_PROCESS_INIT;

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

	повернути ret;
}

static ціле apply_stash(ціле argc, const char **argv, const char *prefix,
		       struct repository *repo UNUSED)
{
	ціле ret = -1;
	ціле quiet = 0;
	ціле index = use_index;
	const char *label_ours = NULL, *label_theirs = NULL, *label_base = NULL;
	struct stash_info info = STASH_INFO_INIT;
	struct option options[] = {
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

	якщо (get_stash_info(&info, argc, argv))
		goto cleanup;

	ret = do_apply_stash(prefix, &info, index, quiet,
			     label_ours, label_theirs, label_base);
cleanup:
	free_stash_info(&info);
	повернути ret;
}

static ціле reject_reflog_ent(const char *refname UNUSED,
			     struct object_id *ooid UNUSED,
			     struct object_id *noid UNUSED,
			     const char *email UNUSED,
			     timestamp_t timestamp UNUSED,
			     ціле tz UNUSED, const char *message UNUSED,
			     void *cb_data UNUSED)
{
	повернути 1;
}

static ціле reflog_is_empty(const char *refname)
{
	повернути !refs_for_each_reflog_ent(get_main_ref_store(the_repository),
					 refname, reject_reflog_ent, NULL);
}

static ціле do_drop_stash(struct stash_info *info, ціле quiet)
{
	якщо (!reflog_delete(info->revision.buf,
			   EXPIRE_REFLOGS_REWRITE | EXPIRE_REFLOGS_UPDATE_REF,
			   0)) {
		якщо (!quiet)
			printf_ln(_("Dropped %s (%s)"), info->revision.buf,
				  oid_to_hex(&info->w_commit));
	} інакше {
		повернути error(_("%s: Could not drop stash entry"),
			     info->revision.buf);
	}

	якщо (reflog_is_empty(ref_stash))
		do_clear_stash();

	повернути 0;
}

static ціле get_stash_info_assert(struct stash_info *info, ціле argc,
				 const char **argv)
{
	ціле ret = get_stash_info(info, argc, argv);

	якщо (ret < 0)
		повернути ret;

	якщо (!info->is_stash_ref)
		повернути error(_("'%s' is not a stash reference"), info->revision.buf);

	повернути 0;
}

static ціле drop_stash(ціле argc, const char **argv, const char *prefix,
		      struct repository *repo UNUSED)
{
	ціле ret = -1;
	ціле quiet = 0;
	struct stash_info info = STASH_INFO_INIT;
	struct option options[] = {
		OPT__QUIET(&quiet, N_("be quiet, only report errors")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_drop_usage, 0);

	якщо (get_stash_info_assert(&info, argc, argv))
		goto cleanup;

	ret = do_drop_stash(&info, quiet);
cleanup:
	free_stash_info(&info);
	повернути ret;
}

static ціле pop_stash(ціле argc, const char **argv, const char *prefix,
		     struct repository *repo UNUSED)
{
	ціле ret = -1;
	ціле index = use_index;
	ціле quiet = 0;
	struct stash_info info = STASH_INFO_INIT;
	struct option options[] = {
		OPT__QUIET(&quiet, N_("be quiet, only report errors")),
		OPT_BOOL(0, "index", &index,
			 N_("attempt to recreate the index")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_pop_usage, 0);

	якщо (get_stash_info_assert(&info, argc, argv))
		goto cleanup;

	якщо ((ret = do_apply_stash(prefix, &info, index, quiet,
				  NULL, NULL, NULL)))
		printf_ln(_("The stash entry is kept in case "
			    "you need it again."));
	інакше
		ret = do_drop_stash(&info, quiet);

cleanup:
	free_stash_info(&info);
	повернути ret;
}

static ціле branch_stash(ціле argc, const char **argv, const char *prefix,
			struct repository *repo UNUSED)
{
	ціле ret = -1;
	const char *branch = NULL;
	struct stash_info info = STASH_INFO_INIT;
	struct child_process cp = CHILD_PROCESS_INIT;
	struct option options[] = {
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_branch_usage, 0);

	якщо (!argc) {
		fprintf_ln(stderr, _("No branch name specified"));
		повернути -1;
	}

	branch = argv[0];

	якщо (get_stash_info(&info, argc - 1, argv + 1))
		goto cleanup;

	cp.git_cmd = 1;
	strvec_pushl(&cp.args, "checkout", "-b", NULL);
	strvec_push(&cp.args, branch);
	strvec_push(&cp.args, oid_to_hex(&info.b_commit));
	ret = run_command(&cp);
	якщо (!ret)
		ret = do_apply_stash(prefix, &info, 1, 0,
				     NULL, NULL, NULL);
	якщо (!ret && info.is_stash_ref)
		ret = do_drop_stash(&info, 0);

cleanup:
	free_stash_info(&info);
	повернути ret;
}

static ціле list_stash(ціле argc, const char **argv, const char *prefix,
		      struct repository *repo UNUSED)
{
	struct child_process cp = CHILD_PROCESS_INIT;
	struct option options[] = {
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_list_usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT);

	якщо (!refs_ref_exists(get_main_ref_store(the_repository), ref_stash))
		повернути 0;

	cp.git_cmd = 1;
	strvec_pushl(&cp.args, "log", "--format=%gd: %gs", "-g",
		     "--first-parent", NULL);
	strvec_pushv(&cp.args, argv);
	strvec_push(&cp.args, ref_stash);
	strvec_push(&cp.args, "--");
	повернути run_command(&cp);
}

static ціле git_stash_config(const char *var, const char *value,
			    const struct config_context *ctx, void *cb)
{
	якщо (!strcmp(var, "stash.showstat")) {
		show_stat = git_config_bool(var, value);
		повернути 0;
	}
	якщо (!strcmp(var, "stash.showpatch")) {
		show_patch = git_config_bool(var, value);
		повернути 0;
	}
	якщо (!strcmp(var, "stash.showincludeuntracked")) {
		show_include_untracked = git_config_bool(var, value);
		повернути 0;
	}
	якщо (!strcmp(var, "stash.index")) {
		use_index = git_config_bool(var, value);
		повернути 0;
	}
	повернути git_diff_basic_config(var, value, ctx, cb);
}

static void diff_include_untracked(const struct stash_info *info, struct diff_options *diff_opt)
{
	const struct object_id *oid[] = { &info->w_commit, &info->u_tree };
	struct tree *tree[ARRAY_SIZE(oid)];
	struct tree_desc tree_desc[ARRAY_SIZE(oid)];
	struct unpack_trees_options unpack_tree_opt = { 0 };

	для (size_t i = 0; i < ARRAY_SIZE(oid); i++) {
		tree[i] = repo_parse_tree_indirect(the_repository, oid[i]);
		якщо (repo_parse_tree(the_repository, tree[i]) < 0)
			die(_("failed to parse tree"));
		init_tree_desc(&tree_desc[i], &tree[i]->object.oid,
			       tree[i]->buffer, tree[i]->size);
	}

	unpack_tree_opt.head_idx = -1;
	unpack_tree_opt.src_index = the_repository->index;
	unpack_tree_opt.dst_index = the_repository->index;
	unpack_tree_opt.merge = 1;
	unpack_tree_opt.fn = stash_worktree_untracked_merge;

	якщо (unpack_trees(ARRAY_SIZE(tree_desc), tree_desc, &unpack_tree_opt))
		die(_("failed to unpack trees"));

	do_diff_cache(&info->b_commit, diff_opt);
}

static ціле show_stash(ціле argc, const char **argv, const char *prefix,
		      struct repository *repo UNUSED)
{
	ціле i;
	ціле ret = -1;
	struct stash_info info = STASH_INFO_INIT;
	struct rev_info rev;
	struct strvec stash_args = STRVEC_INIT;
	struct strvec revision_args = STRVEC_INIT;
	enum {
		UNTRACKED_NONE,
		UNTRACKED_INCLUDE,
		UNTRACKED_ONLY
	} show_untracked = show_include_untracked ? UNTRACKED_INCLUDE : UNTRACKED_NONE;
	struct option options[] = {
		OPT_SET_INT('u', "include-untracked", &show_untracked,
			    N_("include untracked files in the stash"),
			    UNTRACKED_INCLUDE),
		OPT_SET_INT_F(0, "only-untracked", &show_untracked,
			      N_("only show untracked files in the stash"),
			      UNTRACKED_ONLY, PARSE_OPT_NONEG),
		OPT_END()
	};
	ціле do_usage = 0;

	init_diff_ui_defaults();
	repo_config(the_repository, git_diff_ui_config, NULL);
	repo_init_revisions(the_repository, &rev, prefix);

	argc = parse_options(argc, argv, prefix, options, git_stash_show_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	strvec_push(&revision_args, argv[0]);
	для (i = 1; i < argc; i++) {
		якщо (argv[i][0] != '-')
			strvec_push(&stash_args, argv[i]);
		інакше
			strvec_push(&revision_args, argv[i]);
	}

	якщо (get_stash_info(&info, stash_args.nr, stash_args.v))
		goto cleanup;

	/*
	 * The config settings are applied only if there are not passed
	 * any options.
	 */
	якщо (revision_args.nr == 1) {
		якщо (show_stat)
			rev.diffopt.output_format = DIFF_FORMAT_DIFFSTAT;

		якщо (show_patch)
			rev.diffopt.output_format |= DIFF_FORMAT_PATCH;

		якщо (!show_stat && !show_patch) {
			ret = 0;
			goto cleanup;
		}
	}

	setup_revisions_from_strvec(&revision_args, &rev, NULL);
	якщо (revision_args.nr > 1)
		goto usage;
	якщо (!rev.diffopt.output_format) {
		rev.diffopt.output_format = DIFF_FORMAT_PATCH;
		diff_setup_done(&rev.diffopt);
	}

	rev.diffopt.flags.recursive = 1;
	setup_diff_pager(&rev.diffopt);
	switch (show_untracked) {
	case UNTRACKED_NONE:
		diff_tree_oid(&info.b_commit, &info.w_commit, "", &rev.diffopt);
		break;
	case UNTRACKED_ONLY:
		якщо (info.has_u)
			diff_root_tree_oid(&info.u_tree, "", &rev.diffopt);
		break;
	case UNTRACKED_INCLUDE:
		якщо (info.has_u)
			diff_include_untracked(&info, &rev.diffopt);
		інакше
			diff_tree_oid(&info.b_commit, &info.w_commit, "", &rev.diffopt);
		break;
	}
	log_tree_diff_flush(&rev);

	ret = diff_result_code(&rev);

cleanup:
	strvec_clear(&revision_args);
	strvec_clear(&stash_args);
	free_stash_info(&info);
	release_revisions(&rev);
	якщо (do_usage)
		usage_with_options(git_stash_show_usage, options);
	повернути ret;
usage:
	do_usage = 1;
	goto cleanup;
}

static ціле do_store_stash(const struct object_id *w_commit, const char *stash_msg,
			  ціле quiet)
{
	struct stash_info info;
	char revision[GIT_MAX_HEXSZ];

	oid_to_hex_r(revision, w_commit);
	assert_stash_like(&info, revision);

	якщо (!stash_msg)
		stash_msg = "Created via \"git stash store\".";

	якщо (refs_update_ref(get_main_ref_store(the_repository), stash_msg, ref_stash, w_commit, NULL,
			    REF_FORCE_CREATE_REFLOG,
			    quiet ? UPDATE_REFS_QUIET_ON_ERR :
			    UPDATE_REFS_MSG_ON_ERR)) {
		якщо (!quiet) {
			fprintf_ln(stderr, _("Cannot update %s with %s"),
				   ref_stash, oid_to_hex(w_commit));
		}
		повернути -1;
	}

	повернути 0;
}

static ціле store_stash(ціле argc, const char **argv, const char *prefix,
		       struct repository *repo UNUSED)
{
	ціле quiet = 0;
	const char *stash_msg = NULL;
	struct object_id obj;
	struct option options[] = {
		OPT__QUIET(&quiet, N_("be quiet")),
		OPT_STRING('m', "message", &stash_msg, "message",
			   N_("stash message")),
		OPT_END()
	};
	ціле ret;

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_store_usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT);

	якщо (argc != 1) {
		якщо (!quiet)
			fprintf_ln(stderr, _("\"git stash store\" requires one "
					     "<commit> argument"));
		повернути -1;
	}

	якщо (repo_get_oid_with_flags(the_repository, argv[0], &obj,
				    quiet ? GET_OID_QUIETLY : 0)) {
		якщо (!quiet)
			fprintf_ln(stderr, _("Cannot update %s with %s"),
					     ref_stash, argv[0]);
		ret = -1;
		goto out;
	}

	ret = do_store_stash(&obj, stash_msg, quiet);

out:
	повернути ret;
}

static void add_pathspecs(struct strvec *args,
			  const struct pathspec *ps) {
	ціле i;

	для (i = 0; i < ps->nr; i++)
		strvec_push(args, ps->items[i].original);
}

/*
 * `untracked_files` will be filled with the names of untracked files.
 * The return value is:
 *
 * = 0 if there are not any untracked files
 * > 0 if there are untracked files
 */
static ціле get_untracked_files(const struct pathspec *ps, ціле include_untracked,
			       struct strbuf *untracked_files)
{
	ціле i;
	ціле found = 0;
	struct dir_struct dir = DIR_INIT;

	якщо (include_untracked != INCLUDE_ALL_FILES)
		setup_standard_excludes(&dir);

	fill_directory(&dir, the_repository->index, ps);
	для (i = 0; i < dir.nr; i++) {
		struct dir_entry *ent = dir.entries[i];
		found++;
		strbuf_addstr(untracked_files, ent->name);
		/* NUL-terminate: will be fed to update-index -z */
		strbuf_addch(untracked_files, '\0');
	}

	dir_clear(&dir);
	повернути found;
}

/*
 * The return value of `check_changes_tracked_files()` can be:
 *
 * < 0 if there was an error
 * = 0 if there are no changes.
 * > 0 if there are changes.
 */
static ціле check_changes_tracked_files(const struct pathspec *ps)
{
	struct rev_info rev;
	struct object_id dummy;
	ціле ret = 0;

	/* No initial commit. */
	якщо (repo_get_oid(the_repository, "HEAD", &dummy))
		повернути -1;

	якщо (repo_read_index(the_repository) < 0)
		повернути -1;

	repo_init_revisions(the_repository, &rev, NULL);
	copy_pathspec(&rev.prune_data, ps);

	rev.diffopt.flags.quick = 1;
	rev.diffopt.flags.ignore_submodules = 1;
	rev.abbrev = 0;

	add_head_to_pending(&rev);
	diff_setup_done(&rev.diffopt);

	run_diff_index(&rev, DIFF_INDEX_CACHED);
	якщо (diff_result_code(&rev)) {
		ret = 1;
		goto done;
	}

	run_diff_files(&rev, 0);
	якщо (diff_result_code(&rev)) {
		ret = 1;
		goto done;
	}

done:
	release_revisions(&rev);
	повернути ret;
}

/*
 * The function will fill `untracked_files` with the names of untracked files
 * It will return 1 if there were any changes and 0 if there were not.
 */
static ціле check_changes(const struct pathspec *ps, ціле include_untracked,
			 struct strbuf *untracked_files)
{
	ціле ret = 0;
	якщо (check_changes_tracked_files(ps))
		ret = 1;

	якщо (include_untracked && get_untracked_files(ps, include_untracked,
						     untracked_files))
		ret = 1;

	повернути ret;
}

static ціле save_untracked_files(struct stash_info *info, struct strbuf *msg,
				struct strbuf *files)
{
	ціле ret = 0;
	struct strbuf untracked_msg = STRBUF_INIT;
	struct child_process cp_upd_index = CHILD_PROCESS_INIT;
	struct index_state istate = INDEX_STATE_INIT(the_repository);

	cp_upd_index.git_cmd = 1;
	strvec_pushl(&cp_upd_index.args, "update-index", "-z", "--add",
		     "--remove", "--stdin", NULL);
	strvec_pushf(&cp_upd_index.env, "GIT_INDEX_FILE=%s",
			 stash_index_path.buf);

	strbuf_addf(&untracked_msg, "untracked files on %s\n", msg->buf);
	якщо (pipe_command(&cp_upd_index, files->buf, files->len, NULL, 0,
			 NULL, 0)) {
		ret = -1;
		goto done;
	}

	якщо (write_index_as_tree(&info->u_tree, &istate, stash_index_path.buf, 0,
				NULL)) {
		ret = -1;
		goto done;
	}

	якщо (commit_tree(untracked_msg.buf, untracked_msg.len,
			&info->u_tree, NULL, &info->u_commit, NULL, NULL)) {
		ret = -1;
		goto done;
	}

done:
	release_index(&istate);
	strbuf_release(&untracked_msg);
	remove_path(stash_index_path.buf);
	повернути ret;
}

static ціле stash_staged(struct stash_info *info, struct strbuf *out_patch,
			ціле quiet)
{
	ціле ret = 0;
	struct child_process cp_diff_tree = CHILD_PROCESS_INIT;
	struct index_state istate = INDEX_STATE_INIT(the_repository);

	якщо (write_index_as_tree(&info->w_tree, &istate, the_repository->index_file,
				0, NULL)) {
		ret = -1;
		goto done;
	}

	cp_diff_tree.git_cmd = 1;
	strvec_pushl(&cp_diff_tree.args, "diff-tree", "-p", "--binary",
		     "--no-color",
		     "-U1", "HEAD", oid_to_hex(&info->w_tree), "--", NULL);
	якщо (pipe_command(&cp_diff_tree, NULL, 0, out_patch, 0, NULL, 0)) {
		ret = -1;
		goto done;
	}

	якщо (!out_patch->len) {
		якщо (!quiet)
			fprintf_ln(stderr, _("No staged changes"));
		ret = 1;
	}

done:
	release_index(&istate);
	повернути ret;
}

static ціле stash_patch(struct stash_info *info, const struct pathspec *ps,
		       struct strbuf *out_patch, ціле quiet,
		       struct interactive_options *interactive_opts)
{
	ціле ret = 0;
	struct child_process cp_diff_tree = CHILD_PROCESS_INIT;
	struct commit *head_commit;
	const struct object_id *head_tree;
	struct index_state istate = INDEX_STATE_INIT(the_repository);
	char *old_index_env = NULL, *old_repo_index_file;

	remove_path(stash_index_path.buf);

	head_commit = lookup_commit(the_repository, &info->b_commit);
	якщо (!head_commit || repo_parse_commit(the_repository, head_commit)) {
		ret = -1;
		goto done;
	}
	head_tree = get_commit_tree_oid(head_commit);
	якщо (!head_tree) {
		ret = -1;
		goto done;
	}

	якщо (create_index_from_tree(head_tree, stash_index_path.buf)) {
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
	якщо (old_index_env && *old_index_env)
		setenv(INDEX_ENVIRONMENT, old_index_env, 1);
	інакше
		unsetenv(INDEX_ENVIRONMENT);
	FREE_AND_NULL(old_index_env);

	/* State of the working tree. */
	якщо (write_index_as_tree(&info->w_tree, &istate, stash_index_path.buf, 0,
				NULL)) {
		ret = -1;
		goto done;
	}

	cp_diff_tree.git_cmd = 1;
	strvec_pushl(&cp_diff_tree.args, "diff-tree", "-p", "-U1", "HEAD",
		     "--no-color",
		     oid_to_hex(&info->w_tree), "--", NULL);
	якщо (pipe_command(&cp_diff_tree, NULL, 0, out_patch, 0, NULL, 0)) {
		ret = -1;
		goto done;
	}

	якщо (!out_patch->len) {
		якщо (!quiet)
			fprintf_ln(stderr, _("No changes selected"));
		ret = 1;
	}

done:
	release_index(&istate);
	remove_path(stash_index_path.buf);
	повернути ret;
}

static ціле stash_working_tree(struct stash_info *info, const struct pathspec *ps)
{
	ціле ret = 0;
	struct rev_info rev;
	struct child_process cp_upd_index = CHILD_PROCESS_INIT;
	struct strbuf diff_output = STRBUF_INIT;
	struct index_state istate = INDEX_STATE_INIT(the_repository);

	repo_init_revisions(the_repository, &rev, NULL);
	copy_pathspec(&rev.prune_data, ps);

	set_alternate_index_output(stash_index_path.buf);
	якщо (reset_tree(&info->i_tree, 0, 0)) {
		ret = -1;
		goto done;
	}
	set_alternate_index_output(NULL);

	rev.diffopt.output_format = DIFF_FORMAT_CALLBACK;
	rev.diffopt.format_callback = add_diff_to_buf;
	rev.diffopt.format_callback_data = &diff_output;

	якщо (repo_read_index_preload(the_repository, &rev.diffopt.pathspec, 0) < 0) {
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

	якщо (pipe_command(&cp_upd_index, diff_output.buf, diff_output.len,
			 NULL, 0, NULL, 0)) {
		ret = -1;
		goto done;
	}

	якщо (write_index_as_tree(&info->w_tree, &istate, stash_index_path.buf, 0,
				NULL)) {
		ret = -1;
		goto done;
	}

done:
	release_index(&istate);
	release_revisions(&rev);
	strbuf_release(&diff_output);
	remove_path(stash_index_path.buf);
	повернути ret;
}

static ціле do_create_stash(const struct pathspec *ps, struct strbuf *stash_msg_buf,
			   ціле include_untracked, ціле patch_mode,
			   struct interactive_options *interactive_opts,
			   ціле only_staged, struct stash_info *info, struct strbuf *patch,
			   ціле quiet)
{
	ціле ret = 0;
	ціле flags = 0;
	ціле untracked_commit_option = 0;
	const char *head_short_sha1 = NULL;
	const char *branch_ref = NULL;
	const char *branch_name = "(no branch)";
	char *branch_name_buf = NULL;
	struct commit *head_commit = NULL;
	struct commit_list *parents = NULL;
	struct strbuf msg = STRBUF_INIT;
	struct strbuf commit_tree_label = STRBUF_INIT;
	struct strbuf untracked_files = STRBUF_INIT;

	prepare_fallback_ident("git stash", "git@stash");

	repo_read_index_preload(the_repository, NULL, 0);
	якщо (repo_refresh_and_write_index(the_repository, REFRESH_QUIET, 0, 0,
					 NULL, NULL, NULL) < 0) {
		ret = error(_("could not write index"));
		goto done;
	}

	якщо (repo_get_oid(the_repository, "HEAD", &info->b_commit)) {
		якщо (!quiet)
			fprintf_ln(stderr, _("You do not have "
					     "the initial commit yet"));
		ret = -1;
		goto done;
	} інакше {
		head_commit = lookup_commit(the_repository, &info->b_commit);
	}

	якщо (!check_changes(ps, include_untracked, &untracked_files)) {
		ret = 1;
		goto done;
	}

	branch_ref = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
					     "HEAD", 0, NULL, &flags);

	якщо (flags & REF_ISSYMREF) {
		якщо (skip_prefix(branch_ref, "refs/heads/", &branch_name))
			branch_name = branch_name_buf = xstrdup(branch_name);
	}

	head_short_sha1 = repo_find_unique_abbrev(the_repository,
						  &head_commit->object.oid,
						  DEFAULT_ABBREV);
	strbuf_addf(&msg, "%s: %s ", branch_name, head_short_sha1);
	pp_commit_easy(CMIT_FMT_ONELINE, head_commit, &msg);

	strbuf_addf(&commit_tree_label, "index on %s\n", msg.buf);
	commit_list_insert(head_commit, &parents);
	якщо (write_index_as_tree(&info->i_tree, the_repository->index,
				repo_get_index_file(the_repository), 0, NULL) ||
	    commit_tree(commit_tree_label.buf, commit_tree_label.len,
			&info->i_tree, parents, &info->i_commit, NULL, NULL)) {
		якщо (!quiet)
			fprintf_ln(stderr, _("Cannot save the current "
					     "index state"));
		ret = -1;
		goto done;
	}

	commit_list_free(parents);
	parents = NULL;

	якщо (include_untracked) {
		якщо (save_untracked_files(info, &msg, &untracked_files)) {
			якщо (!quiet)
				fprintf_ln(stderr, _("Cannot save "
						     "the untracked files"));
			ret = -1;
			goto done;
		}
		untracked_commit_option = 1;
	}
	якщо (patch_mode) {
		ret = stash_patch(info, ps, patch, quiet, interactive_opts);
		якщо (ret < 0) {
			якщо (!quiet)
				fprintf_ln(stderr, _("Cannot save the current "
						     "worktree state"));
			goto done;
		} інакше якщо (ret > 0) {
			goto done;
		}
	} інакше якщо (only_staged) {
		ret = stash_staged(info, patch, quiet);
		якщо (ret < 0) {
			якщо (!quiet)
				fprintf_ln(stderr, _("Cannot save the current "
						     "staged state"));
			goto done;
		} інакше якщо (ret > 0) {
			goto done;
		}
	} інакше {
		якщо (stash_working_tree(info, ps)) {
			якщо (!quiet)
				fprintf_ln(stderr, _("Cannot save the current "
						     "worktree state"));
			ret = -1;
			goto done;
		}
	}

	якщо (!stash_msg_buf->len)
		strbuf_addf(stash_msg_buf, "WIP on %s", msg.buf);
	інакше
		strbuf_insertf(stash_msg_buf, 0, "On %s: ", branch_name);

	якщо (untracked_commit_option)
		commit_list_insert(lookup_commit(the_repository,
						 &info->u_commit),
				   &parents);
	commit_list_insert(lookup_commit(the_repository, &info->i_commit),
			   &parents);
	commit_list_insert(head_commit, &parents);

	якщо (commit_tree(stash_msg_buf->buf, stash_msg_buf->len, &info->w_tree,
			parents, &info->w_commit, NULL, NULL)) {
		якщо (!quiet)
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
	повернути ret;
}

static ціле create_stash(ціле argc, const char **argv, const char *prefix UNUSED,
			struct repository *repo UNUSED)
{
	ціле ret;
	struct strbuf stash_msg_buf = STRBUF_INIT;
	struct stash_info info = STASH_INFO_INIT;
	struct pathspec ps;

	/* Starting with argv[1], since argv[0] is "create" */
	strbuf_join_argv(&stash_msg_buf, argc - 1, ++argv, ' ');

	memset(&ps, 0, sizeof(ps));
	якщо (!check_changes_tracked_files(&ps))
		повернути 0;

	ret = do_create_stash(&ps, &stash_msg_buf, 0, 0, NULL, 0, &info,
			      NULL, 0);
	якщо (!ret)
		printf_ln("%s", oid_to_hex(&info.w_commit));

	free_stash_info(&info);
	strbuf_release(&stash_msg_buf);
	повернути ret;
}

static ціле do_push_stash(const struct pathspec *ps, const char *stash_msg, ціле quiet,
			 ціле keep_index, ціле patch_mode,
			 struct interactive_options *interactive_opts,
			 ціле include_untracked, ціле only_staged)
{
	ціле ret = 0;
	struct stash_info info = STASH_INFO_INIT;
	struct strbuf patch = STRBUF_INIT;
	struct strbuf stash_msg_buf = STRBUF_INIT;
	struct strbuf untracked_files = STRBUF_INIT;
	struct strbuf out = STRBUF_INIT;

	якщо (patch_mode && keep_index == -1)
		keep_index = 1;

	якщо (patch_mode && include_untracked) {
		fprintf_ln(stderr, _("Can't use --patch and --include-untracked"
				     " or --all at the same time"));
		ret = -1;
		goto done;
	}

	/* --patch overrides --staged */
	якщо (patch_mode)
		only_staged = 0;

	якщо (only_staged && include_untracked) {
		fprintf_ln(stderr, _("Can't use --staged and --include-untracked"
				     " or --all at the same time"));
		ret = -1;
		goto done;
	}

	repo_read_index_preload(the_repository, NULL, 0);
	якщо (!include_untracked && ps->nr) {
		char *ps_matched = xcalloc(ps->nr, 1);

		якщо (pathspec_needs_expanded_index(the_repository->index, ps))
			ensure_full_index(the_repository->index);
		для (size_t i = 0; i < the_repository->index->cache_nr; i++)
			ce_path_match(the_repository->index, the_repository->index->cache[i], ps,
				      ps_matched);

		якщо (report_path_error(ps_matched, ps)) {
			fprintf_ln(stderr, _("Did you forget to 'git add'?"));
			ret = -1;
			free(ps_matched);
			goto done;
		}
		free(ps_matched);
	}

	якщо (repo_refresh_and_write_index(the_repository, REFRESH_QUIET, 0, 0,
					 NULL, NULL, NULL)) {
		ret = error(_("could not write index"));
		goto done;
	}

	якщо (!check_changes(ps, include_untracked, &untracked_files)) {
		якщо (!quiet)
			printf_ln(_("No local changes to save"));
		goto done;
	}

	якщо (!refs_reflog_exists(get_main_ref_store(the_repository), ref_stash) && do_clear_stash()) {
		ret = -1;
		якщо (!quiet)
			fprintf_ln(stderr, _("Cannot initialize stash"));
		goto done;
	}

	якщо (stash_msg)
		strbuf_addstr(&stash_msg_buf, stash_msg);
	якщо (do_create_stash(ps, &stash_msg_buf, include_untracked, patch_mode,
			    interactive_opts, only_staged, &info, &patch, quiet)) {
		ret = -1;
		goto done;
	}

	якщо (do_store_stash(&info.w_commit, stash_msg_buf.buf, 1)) {
		ret = -1;
		якщо (!quiet)
			fprintf_ln(stderr, _("Cannot save the current status"));
		goto done;
	}

	якщо (!quiet)
		printf_ln(_("Saved working directory and index state %s"),
			  stash_msg_buf.buf);

	якщо (!(patch_mode || only_staged)) {
		якщо (include_untracked && !ps->nr) {
			struct child_process cp = CHILD_PROCESS_INIT;

			cp.git_cmd = 1;
			якщо (startup_info->original_cwd) {
				cp.dir = startup_info->original_cwd;
				strvec_pushf(&cp.env, "%s=%s",
					     GIT_WORK_TREE_ENVIRONMENT,
					     the_repository->worktree);
			}
			strvec_pushl(&cp.args, "clean", "--force",
				     "--quiet", "-d", ":/", NULL);
			якщо (include_untracked == INCLUDE_ALL_FILES)
				strvec_push(&cp.args, "-x");
			якщо (run_command(&cp)) {
				ret = -1;
				goto done;
			}
		}
		discard_index(the_repository->index);
		якщо (ps->nr) {
			struct child_process cp_add = CHILD_PROCESS_INIT;
			struct child_process cp_diff = CHILD_PROCESS_INIT;
			struct child_process cp_apply = CHILD_PROCESS_INIT;

			cp_add.git_cmd = 1;
			strvec_push(&cp_add.args, "add");
			якщо (!include_untracked)
				strvec_push(&cp_add.args, "-u");
			якщо (include_untracked == INCLUDE_ALL_FILES)
				strvec_push(&cp_add.args, "--force");
			strvec_push(&cp_add.args, "--");
			add_pathspecs(&cp_add.args, ps);
			якщо (run_command(&cp_add)) {
				ret = -1;
				goto done;
			}

			cp_diff.git_cmd = 1;
			strvec_pushl(&cp_diff.args, "diff-index", "-p",
				     "--no-color",
				     "--cached", "--binary", "HEAD", "--",
				     NULL);
			add_pathspecs(&cp_diff.args, ps);
			якщо (pipe_command(&cp_diff, NULL, 0, &out, 0, NULL, 0)) {
				ret = -1;
				goto done;
			}

			cp_apply.git_cmd = 1;
			strvec_pushl(&cp_apply.args, "apply", "--index",
				     "-R", NULL);
			якщо (pipe_command(&cp_apply, out.buf, out.len, NULL, 0,
					 NULL, 0)) {
				ret = -1;
				goto done;
			}
		} інакше {
			struct child_process cp = CHILD_PROCESS_INIT;
			cp.git_cmd = 1;
			/* BUG: this nukes untracked files in the way */
			strvec_pushl(&cp.args, "reset", "--hard", "-q",
				     "--no-recurse-submodules", NULL);
			якщо (run_command(&cp)) {
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
		якщо (keep_index == 1 && !is_null_oid(&info.i_tree) &&
		    !is_empty_tree_oid(&info.i_tree, the_repository->hash_algo)) {
			struct child_process cp = CHILD_PROCESS_INIT;

			cp.git_cmd = 1;
			strvec_pushl(&cp.args, "checkout", "--no-overlay",
				     oid_to_hex(&info.i_tree), "--", NULL);
			якщо (!ps->nr)
				strvec_push(&cp.args, ":/");
			інакше
				add_pathspecs(&cp.args, ps);
			якщо (run_command(&cp)) {
				ret = -1;
				goto done;
			}
		}
		goto done;
	} інакше {
		struct child_process cp = CHILD_PROCESS_INIT;

		cp.git_cmd = 1;
		strvec_pushl(&cp.args, "apply", "-R", NULL);

		якщо (pipe_command(&cp, patch.buf, patch.len, NULL, 0, NULL, 0)) {
			якщо (!quiet)
				fprintf_ln(stderr, _("Cannot remove "
						     "worktree changes"));
			ret = -1;
			goto done;
		}

		якщо (keep_index < 1) {
			struct child_process cp = CHILD_PROCESS_INIT;

			cp.git_cmd = 1;
			strvec_pushl(&cp.args, "reset", "-q", "--refresh", "--",
				     NULL);
			add_pathspecs(&cp.args, ps);
			якщо (run_command(&cp)) {
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
	повернути ret;
}

static ціле push_stash(ціле argc, const char **argv, const char *prefix,
		      ціле push_assumed)
{
	ціле force_assume = 0;
	ціле keep_index = -1;
	ціле only_staged = 0;
	ціле patch_mode = 0;
	ціле include_untracked = 0;
	ціле quiet = 0;
	ціле pathspec_file_nul = 0;
	const char *stash_msg = NULL;
	char *pathspec_from_file = NULL;
	struct pathspec ps;
	struct interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	struct option options[] = {
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
	ціле ret;

	якщо (argc) {
		ціле flags = PARSE_OPT_KEEP_DASHDASH;

		якщо (push_assumed)
			flags |= PARSE_OPT_STOP_AT_NON_OPTION;

		argc = parse_options(argc, argv, prefix, options,
				     push_assumed ? git_stash_usage :
				     git_stash_push_usage, flags);
		force_assume |= patch_mode;
	}

	якщо (argc) {
		якщо (!strcmp(argv[0], "--")) {
			argc--;
			argv++;
		} інакше якщо (push_assumed && !force_assume) {
			die("subcommand wasn't specified; 'push' can't be assumed due to unexpected token '%s'",
			    argv[0]);
		}
	}

	parse_pathspec(&ps, 0, PATHSPEC_PREFER_FULL | PATHSPEC_PREFIX_ORIGIN,
		       prefix, argv);

	якщо (pathspec_from_file) {
		якщо (patch_mode)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--patch");

		якщо (only_staged)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--staged");

		якщо (ps.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&ps, 0,
				    PATHSPEC_PREFER_FULL | PATHSPEC_PREFIX_ORIGIN,
				    prefix, pathspec_from_file, pathspec_file_nul);
	} інакше якщо (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	якщо (!patch_mode) {
		якщо (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--patch");
		якщо (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--patch");
		якщо (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--patch");
	}

	якщо (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	якщо (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	ret = do_push_stash(&ps, stash_msg, quiet, keep_index, patch_mode,
			    &interactive_opts, include_untracked, only_staged);

	clear_pathspec(&ps);
	free(pathspec_from_file);
	повернути ret;
}

static ціле push_stash_unassumed(ціле argc, const char **argv, const char *prefix,
				struct repository *repo UNUSED)
{
	повернути push_stash(argc, argv, prefix, 0);
}

static ціле save_stash(ціле argc, const char **argv, const char *prefix,
		      struct repository *repo UNUSED)
{
	ціле keep_index = -1;
	ціле only_staged = 0;
	ціле patch_mode = 0;
	ціле include_untracked = 0;
	ціле quiet = 0;
	ціле ret = 0;
	const char *stash_msg = NULL;
	struct pathspec ps;
	struct strbuf stash_msg_buf = STRBUF_INIT;
	struct interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	struct option options[] = {
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

	якщо (argc)
		stash_msg = strbuf_join_argv(&stash_msg_buf, argc, argv, ' ');

	memset(&ps, 0, sizeof(ps));

	якщо (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	якщо (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	якщо (!patch_mode) {
		якщо (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--patch");
		якщо (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--patch");
		якщо (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--patch");
	}

	ret = do_push_stash(&ps, stash_msg, quiet, keep_index,
			    patch_mode, &interactive_opts, include_untracked,
			    only_staged);

	strbuf_release(&stash_msg_buf);
	повернути ret;
}

static ціле write_commit_with_parents(struct repository *r,
				     struct object_id *out,
				     const struct object_id *oid,
				     struct commit_list *parents)
{
	size_t author_len, committer_len;
	struct commit *this;
	const char *orig_author, *orig_committer;
	char *author = NULL, *committer = NULL;
	const char *buffer;
	unsigned long bufsize;
	const char *p;
	struct strbuf msg = STRBUF_INIT;
	ціле ret = 0;
	struct ident_split id;

	this = lookup_commit_reference(r, oid);
	buffer = repo_get_commit_buffer(r, this, &bufsize);
	orig_author = find_commit_header(buffer, "author", &author_len);
	orig_committer = find_commit_header(buffer, "committer", &committer_len);

	якщо (!orig_author || !orig_committer) {
		ret = error(_("cannot parse commit %s"), oid_to_hex(oid));
		goto out;
	}

	якщо (split_ident_line(&id, orig_author, author_len) < 0 ||
	    split_ident_line(&id, orig_committer, committer_len) < 0) {
		ret = error(_("invalid author or committer for %s"), oid_to_hex(oid));
		goto out;
	}

	p = strstr(buffer, "\n\n");
	strbuf_addstr(&msg, "git stash: ");

	якщо (p)
		strbuf_add(&msg, p + 2, bufsize - (p + 2 - buffer));
	strbuf_complete_line(&msg);

	author = xmemdupz(orig_author, author_len);
	committer = xmemdupz(orig_committer, committer_len);

	якщо (commit_tree_extended(msg.buf, msg.len,
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
	повернути ret;
}

static ціле do_import_stash(struct repository *r, const char *rev)
{
	struct object_id chain;
	ціле res = 0;
	const char *buffer = NULL;
	unsigned long bufsize;
	struct commit *this = NULL;
	struct commit_list *items = NULL, *cur;
	char *msg = NULL;

	якщо (repo_get_oid(r, rev, &chain))
		повернути error(_("not a valid revision: %s"), rev);

	this = lookup_commit_reference(r, &chain);
	якщо (!this)
		повернути error(_("not a commit: %s"), rev);

	/*
	 * Walk the commit history, finding each stash entry, and load data into
	 * the array.
	 */
	для (;;) {
		const char *author, *committer;
		size_t author_len, committer_len;
		const char *p;
		const char *expected = "git stash <git@stash> 1000684800 +0000";
		const char *prefix = "git stash: ";
		struct commit *stash;
		struct tree *tree = repo_get_commit_tree(r, this);

		якщо (!tree ||
		    !oideq(&tree->object.oid, r->hash_algo->empty_tree) ||
		    (this->parents &&
		     (!this->parents->next || this->parents->next->next))) {
			res = error(_("%s is not a valid exported stash commit"),
					oid_to_hex(&this->object.oid));
			goto out;
		}

		buffer = repo_get_commit_buffer(r, this, &bufsize);

		якщо (!this->parents) {
			/*
			 * We don't have any parents.  Make sure this is our
			 * root commit.
			 */
			author = find_commit_header(buffer, "author", &author_len);
			committer = find_commit_header(buffer, "committer", &committer_len);

			якщо (!author || !committer) {
				error(_("cannot parse commit %s"), oid_to_hex(&this->object.oid));
				goto out;
			}

			якщо (author_len != strlen(expected) ||
			    committer_len != strlen(expected) ||
			    memcmp(author, expected, author_len) ||
			    memcmp(committer, expected, committer_len)) {
				res = error(_("found root commit %s with invalid data"), oid_to_hex(&this->object.oid));
				goto out;
			}
			break;
		}

		p = strstr(buffer, "\n\n");
		якщо (!p) {
			res = error(_("cannot parse commit %s"), oid_to_hex(&this->object.oid));
			goto out;
		}

		p += 2;
		якщо (((size_t)(bufsize - (p - buffer)) < strlen(prefix)) ||
		    memcmp(prefix, p, strlen(prefix))) {
			res = error(_("found stash commit %s without expected prefix"), oid_to_hex(&this->object.oid));
			goto out;
		}

		stash = this->parents->next->item;

		якщо (repo_parse_commit(r, this->parents->item) ||
		    repo_parse_commit(r, stash)) {
			res = error(_("cannot parse parents of commit: %s"),
					oid_to_hex(&this->object.oid));
			goto out;
		}

		якщо (check_stash_topology(r, stash)) {
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
	для (cur = items; cur; cur = cur->next) {
		const char *p;
		struct object_id *oid;

		this = cur->item;
		oid = &this->object.oid;
		buffer = repo_get_commit_buffer(r, this, &bufsize);
		якщо (!buffer) {
			res = error(_("cannot read commit buffer for %s"), oid_to_hex(oid));
			goto out;
		}

		p = strstr(buffer, "\n\n");
		якщо (!p) {
			res = error(_("cannot parse commit %s"), oid_to_hex(oid));
			goto out;
		}

		p += 2;
		msg = xmemdupz(p, bufsize - (p - buffer));
		repo_unuse_commit_buffer(r, this, buffer);
		buffer = NULL;

		якщо (do_store_stash(oid, msg, 1)) {
			res = error(_("cannot save the stash for %s"), oid_to_hex(oid));
			goto out;
		}
		FREE_AND_NULL(msg);
	}
out:
	якщо (this && buffer)
		repo_unuse_commit_buffer(r, this, buffer);
	commit_list_free(items);
	free(msg);

	повернути res;
}

static ціле import_stash(ціле argc, const char **argv, const char *prefix,
			struct repository *repo)
{
	struct option options[] = {
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_import_usage,
			     PARSE_OPT_KEEP_DASHDASH);

	якщо (argc != 1)
		usage_msg_opt("a revision is required", git_stash_import_usage, options);

	повернути do_import_stash(repo, argv[0]);
}

struct stash_entry_data {
	struct repository *r;
	struct commit_list **items;
	size_t count;
};

static ціле collect_stash_entries(const char *refname UNUSED,
				 struct object_id *old_oid UNUSED,
				 struct object_id *new_oid,
				 const char *committer UNUSED,
				 timestamp_t timestamp UNUSED,
				 ціле tz UNUSED, const char *msg UNUSED,
				 void *cb_data)
{
	struct stash_entry_data *data = cb_data;
	struct commit *stash;

	data->count++;
	stash = lookup_commit_reference(data->r, new_oid);
	якщо (!stash || check_stash_topology(data->r, stash)) {
		повернути error(_("%s does not look like a stash commit"),
			     oid_to_hex(new_oid));
	}
	data->items = commit_list_append(stash, data->items);
	повернути 0;
}

static ціле do_export_stash(struct repository *r,
			   const char *ref,
			   ціле argc,
			   const char **argv)
{
	struct object_id base;
	struct commit *prev;
	struct commit_list *items = NULL, **iter = &items, *cur;
	ціле res = 0;
	ціле i;
	struct strbuf revision = STRBUF_INIT;
	const char *author, *committer;

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
	якщо (commit_tree_extended("", 0, r->hash_algo->empty_tree, NULL,
				 &base, author, committer, NULL, NULL))
		повернути error(_("unable to write base commit"));

	prev = lookup_commit_reference(r, &base);

	якщо (argc) {
		/*
		 * Find each specified stash, and load data into the array.
		 */
		для (i = 0; i < argc; i++) {
			struct object_id oid;
			struct commit *stash;

			якщо (parse_stash_revision(&revision, argv[i], 1) ||
			    repo_get_oid_with_flags(r, revision.buf, &oid,
						    GET_OID_QUIETLY |
						    GET_OID_GENTLY)) {
				res = error(_("unable to find stash entry %s"), argv[i]);
				goto out;
			}

			stash = lookup_commit_reference(r, &oid);
			якщо (!stash || check_stash_topology(r, stash)) {
				res = error(_("%s does not look like a stash commit"),
						revision.buf);
				goto out;
			}
			iter = commit_list_append(stash, iter);
		}
	} інакше {
		/*
		 * Walk the reflog, finding each stash entry, and load data into the
		 * array.
		 */
		struct stash_entry_data cb_data = {
			.r = r, .items = iter,
		};
		якщо (refs_for_each_reflog_ent_reverse(get_main_ref_store(r),
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
	для (cur = items; cur; cur = cur->next) {
		struct commit_list *parents = NULL;
		struct commit_list **next = &parents;
		struct object_id out;
		struct commit *stash = cur->item;

		next = commit_list_append(prev, next);
		next = commit_list_append(stash, next);
		res = write_commit_with_parents(r, &out, &stash->object.oid, parents);
		commit_list_free(parents);
		якщо (res)
			goto out;
		prev = lookup_commit_reference(r, &out);
	}
	якщо (ref)
		refs_update_ref(get_main_ref_store(r), NULL, ref,
				&prev->object.oid, NULL, 0, UPDATE_REFS_DIE_ON_ERR);
	інакше
		puts(oid_to_hex(&prev->object.oid));
out:
	strbuf_release(&revision);
	commit_list_free(items);

	повернути res;
}

enum export_action {
	ACTION_NONE,
	ACTION_PRINT,
	ACTION_TO_REF,
};

static ціле export_stash(ціле argc,
			const char **argv,
			const char *prefix,
			struct repository *repo)
{
	const char *ref = NULL;
	enum export_action action = ACTION_NONE;
	struct option options[] = {
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

	якщо (ref && action == ACTION_NONE)
		action = ACTION_TO_REF;

	якщо (action == ACTION_NONE || (ref && action == ACTION_PRINT))
		повернути error(_("exactly one of --print and --to-ref is required"));

	повернути do_export_stash(repo, ref, argc, argv);
}

ціле cmd_stash(ціле argc,
	      const char **argv,
	      const char *prefix,
	      struct repository *repo)
{
	pid_t pid = getpid();
	const char *index_file;
	struct strvec args = STRVEC_INIT;
	parse_opt_subcommand_fn *fn = NULL;
	struct option options[] = {
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
	const char **args_copy;
	ціле ret;

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

	якщо (fn) {
		ret = fn(argc, argv, prefix, repo);

		/*
		 * The subcommand implementations return 0 on success, a
		 * negative value on failure, and STASH_APPLY_CONFLICT
		 * when applying a stash entry resulted in conflicts.
		 * Map failures to 128, the status die() uses, so that
		 * exit status 1 unambiguously indicates conflicts.
		 */
		якщо (ret < 0)
			повернути 128;
		повернути ret;
	} інакше якщо (!argc) {
		повернути !!push_stash_unassumed(0, NULL, prefix, repo);
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
	повернути ret;
}
