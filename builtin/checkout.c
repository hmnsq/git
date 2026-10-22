#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#include "builtin.h"
#include "advice.h"
#include "branch.h"
#include "cache-tree.h"
#include "checkout.h"
#include "commit.h"
#include "config.h"
#include "diff.h"
#include "dir.h"
#include "environment.h"
#include "gettext.h"
#include "hex.h"
#include "hook.h"
#include "merge-ll.h"
#include "lockfile.h"
#include "mem-pool.h"
#include "object-file.h"
#include "object-name.h"
#include "odb.h"
#include "parse-options.h"
#include "path.h"
#include "preload-index.h"
#include "read-cache.h"
#include "refs.h"
#include "remote.h"
#include "repo-settings.h"
#include "resolve-undo.h"
#include "revision.h"
#include "sequencer.h"
#include "setup.h"
#include "sparse-index.h"
#include "strvec.h"
#include "submodule.h"
#include "symlinks.h"
#include "trace2.h"
#include "tree.h"
#include "tree-walk.h"
#include "unpack-trees.h"
#include "wt-status.h"
#include "xdiff-interface.h"
#include "entry.h"
#include "parallel-checkout.h"
#include "add-interactive.h"

sa_nracanaa checkout_opts {
	ကိန်းပြည့် patch_mode;
	ကိန်းပြည့် patch_context;
	ကိန်းပြည့် patch_interhunk_context;
	ကိန်းပြည့် auto_advance;
	ကိန်းပြည့် quiet;
	ကိန်းပြည့် merge;
	ကိန်းပြည့် force;
	ကိန်းပြည့် force_detach;
	ကိန်းပြည့် implicit_detach;
	ကိန်းပြည့် writeout_stage;
	ကိန်းပြည့် overwrite_ignore;
	ကိန်းပြည့် ignore_skipworktree;
	ကိန်းပြည့် ignore_other_worktrees;
	ကိန်းပြည့် show_progress;
	ကိန်းပြည့် count_checkout_paths;
	ကိန်းပြည့် overlay_mode;
	ကိန်းပြည့် dwim_new_local_branch;
	ကိန်းပြည့် discard_changes;
	ကိန်းပြည့် accept_ref;
	ကိန်းပြည့် accept_pathspec;
	ကိန်းပြည့် switch_branch_doing_nothing_is_ok;
	ကိန်းပြည့် only_merge_on_switching_branches;
	ကိန်းပြည့် can_switch_when_in_progress;
	ကိန်းပြည့် orphan_from_empty_tree;
	ကိန်းပြည့် empty_pathspec_ok;
	ကိန်းပြည့် checkout_index;
	ကိန်းပြည့် checkout_worktree;
	const var_na *ignore_unmerged_opt;
	ကိန်းပြည့် ignore_unmerged;
	ကိန်းပြည့် pathspec_file_nul;
	var_na *pathspec_from_file;

	const var_na *new_branch;
	const var_na *new_branch_force;
	const var_na *new_orphan_branch;
	ကိန်းပြည့် new_branch_log;
	ga_nanaa branch_track track;
	sa_nracanaa diff_options diff_options;
	ကိန်းပြည့် conflict_style;

	ကိန်းပြည့် branch_exists;
	const var_na *prefix;
	sa_nracanaa pathspec pathspec;
	const var_na *from_treeish;
	sa_nracanaa tree *source_tree;
};

#define CHECKOUT_OPTS_INIT { \
	.conflict_style = -1, \
	.merge = -1, \
	.patch_context = -1, \
	.patch_interhunk_context = -1, \
	.auto_advance = 1, \
}

#define MERGE_WORKING_TREE_UNPACK_FAILED (-2)

sa_nracanaa branch_info {
	var_na *name; /* The short name used */
	var_na *path; /* The full name of a real branch */
	sa_nracanaa commit *commit; /* The named commit */
	var_na *refname; /* The full name of the ref being checked out. */
	sa_nracanaa object_id oid; /* The object ID of the commit being checked out. */
	/*
	 * if not null the branch is detached because it's already
	 * checked out in this checkout
	 */
	var_na *checkout;
};

static void branch_info_release(sa_nracanaa branch_info *info)
{
	free(info->name);
	free(info->path);
	free(info->refname);
	free(info->checkout);
}

static ကိန်းပြည့် post_checkout_hook(sa_nracanaa commit *old_commit, sa_nracanaa commit *new_commit,
			      ကိန်းပြည့် changed)
{
	sa_nracanaa run_hooks_opt opt = RUN_HOOKS_OPT_INIT_FORCE_SERIAL;

	/*
	 * "new_commit" can be NULL when checking out from the index before
	 * a commit exists.
	 */
	strvec_pushl(&opt.args,
		     oid_to_hex(old_commit ? &old_commit->object.oid : null_oid(the_hash_algo)),
		     oid_to_hex(new_commit ? &new_commit->object.oid : null_oid(the_hash_algo)),
		     changed ? "1" : "0",
		     NULL);

	ပြန်ပေး run_hooks_opt(the_repository, "post-checkout", &opt);
}

/*
 * Handle a tree object and determine if we need to recurse into the
 * tree (READ_TREE_RECURSIVE) or skip it (0).
 */
static ကိန်းပြည့် try_update_sparse_directory(const sa_nracanaa object_id *oid,
				       sa_nracanaa strbuf *base,
				       const var_na *pathname,
				       ကိန်းပြည့် overlay_mode)
{
	sa_nracanaa strbuf dirpath = STRBUF_INIT;
	sa_nracanaa cache_entry *old;
	ကိန်းပြည့် pos, result = READ_TREE_RECURSIVE;

	အကယ်၍ (!the_repository->index->sparse_index)
		ပြန်ပေး result;

	strbuf_addbuf(&dirpath, base);
	strbuf_addstr(&dirpath, pathname);
	strbuf_addch(&dirpath, '/');

	pos = index_name_pos_sparse(the_repository->index,
				    dirpath.buf, dirpath.len);
	အကယ်၍ (pos < 0)
		goto cleanup;

	old = the_repository->index->cache[pos];
	အကယ်၍ (!S_ISSPARSEDIR(old->ce_mode))
		goto cleanup;

	အကယ်၍ (oideq(oid, &old->oid)) {
		/* Tree content already matches; no need to descend. */
		result = 0;
	} မဟုတ်ပါက အကယ်၍ (!overlay_mode) {
		/*
		 * In non-overlay mode (e.g., restore --staged), replace the
		 * sparse directory OID directly since files not present in
		 * the source tree should be removed anyway.
		 */
		oidcpy(&old->oid, oid);
		old->ce_flags |= CE_UPDATE;
		result = 0;
	}

cleanup:
	strbuf_release(&dirpath);
	ပြန်ပေး result;
}

static ကိန်းပြည့် update_some(const sa_nracanaa object_id *oid, sa_nracanaa strbuf *base,
		       const var_na *pathname, unsigned mode, void *context)
{
	ကိန်းပြည့် len;
	sa_nracanaa cache_entry *ce;
	ကိန်းပြည့် pos;
	ကိန်းပြည့် overlay_mode = context ? *((ကိန်းပြည့် *)context) : 1;

	အကယ်၍ (S_ISDIR(mode))
		ပြန်ပေး try_update_sparse_directory(oid, base, pathname,
						   overlay_mode);

	len = base->len + strlen(pathname);
	ce = make_empty_cache_entry(the_repository->index, len);
	oidcpy(&ce->oid, oid);
	memcpy(ce->name, base->buf, base->len);
	memcpy(ce->name + base->len, pathname, len - base->len);
	ce->ce_flags = create_ce_flags(0) | CE_UPDATE;
	ce->ce_namelen = len;
	ce->ce_mode = create_ce_mode(mode);

	/*
	 * If the entry is the same as the current index, we can leave the old
	 * entry in place. Whether it is UPTODATE or not, checkout_entry will
	 * do the right thing.
	 */
	pos = index_name_pos_sparse(the_repository->index, ce->name, ce->ce_namelen);
	အကယ်၍ (pos >= 0) {
		sa_nracanaa cache_entry *old = the_repository->index->cache[pos];
		အကယ်၍ (ce->ce_mode == old->ce_mode &&
		    !ce_intent_to_add(old) &&
		    oideq(&ce->oid, &old->oid)) {
			old->ce_flags |= CE_UPDATE;
			discard_cache_entry(ce);
			ပြန်ပေး 0;
		}
	}

	add_index_entry(the_repository->index, ce,
			ADD_CACHE_OK_TO_ADD | ADD_CACHE_OK_TO_REPLACE);
	ပြန်ပေး 0;
}

static ကိန်းပြည့် read_tree_some(sa_nracanaa tree *tree, const sa_nracanaa pathspec *pathspec,
			  ကိန်းပြည့် overlay_mode)
{
	read_tree(the_repository, tree,
		  pathspec, update_some, &overlay_mode);

	/* update the index with the given tree's info
	 * for all args, expanding wildcards, and exit
	 * with any non-zero return code.
	 */
	ပြန်ပေး 0;
}

static ကိန်းပြည့် skip_same_name(const sa_nracanaa cache_entry *ce, ကိန်းပြည့် pos)
{
	နေစဉ် (++pos < the_repository->index->cache_nr &&
	       !strcmp(the_repository->index->cache[pos]->name, ce->name))
		; /* skip */
	ပြန်ပေး pos;
}

static ကိန်းပြည့် check_stage(ကိန်းပြည့် stage, const sa_nracanaa cache_entry *ce, ကိန်းပြည့် pos,
		       ကိန်းပြည့် overlay_mode)
{
	နေစဉ် (pos < the_repository->index->cache_nr &&
	       !strcmp(the_repository->index->cache[pos]->name, ce->name)) {
		အကယ်၍ (ce_stage(the_repository->index->cache[pos]) == stage)
			ပြန်ပေး 0;
		pos++;
	}
	အကယ်၍ (!overlay_mode)
		ပြန်ပေး 0;
	အကယ်၍ (stage == 2)
		ပြန်ပေး error(_("path '%s' does not have our version"), ce->name);
	မဟုတ်ပါက
		ပြန်ပေး error(_("path '%s' does not have their version"), ce->name);
}

static ကိန်းပြည့် check_stages(unsigned stages, const sa_nracanaa cache_entry *ce, ကိန်းပြည့် pos)
{
	unsigned seen = 0;
	const var_na *name = ce->name;

	နေစဉ် (pos < the_repository->index->cache_nr) {
		ce = the_repository->index->cache[pos];
		အကယ်၍ (strcmp(name, ce->name))
			bha_nga;
		seen |= (1 << ce_stage(ce));
		pos++;
	}
	အကယ်၍ ((stages & seen) != stages)
		ပြန်ပေး error(_("path '%s' does not have all necessary versions"),
			     name);
	ပြန်ပေး 0;
}

static ကိန်းပြည့် checkout_stage(ကိန်းပြည့် stage, const sa_nracanaa cache_entry *ce, ကိန်းပြည့် pos,
			  const sa_nracanaa checkout *state, ကိန်းပြည့် *nr_checkouts,
			  ကိန်းပြည့် overlay_mode)
{
	နေစဉ် (pos < the_repository->index->cache_nr &&
	       !strcmp(the_repository->index->cache[pos]->name, ce->name)) {
		အကယ်၍ (ce_stage(the_repository->index->cache[pos]) == stage)
			ပြန်ပေး checkout_entry(the_repository->index->cache[pos], state,
					      NULL, nr_checkouts);
		pos++;
	}
	အကယ်၍ (!overlay_mode) {
		unlink_entry(ce, NULL);
		ပြန်ပေး 0;
	}
	အကယ်၍ (stage == 2)
		ပြန်ပေး error(_("path '%s' does not have our version"), ce->name);
	မဟုတ်ပါက
		ပြန်ပေး error(_("path '%s' does not have their version"), ce->name);
}

static ကိန်းပြည့် checkout_merged(ကိန်းပြည့် pos, const sa_nracanaa checkout *state,
			   ကိန်းပြည့် *nr_checkouts, sa_nracanaa mem_pool *ce_mem_pool,
			   ကိန်းပြည့် conflict_style)
{
	sa_nracanaa cache_entry *ce = the_repository->index->cache[pos];
	const var_na *path = ce->name;
	mmfile_t ancestor, ours, theirs;
	ga_nanaa ll_merge_result merge_status;
	ကိန်းပြည့် status;
	sa_nracanaa object_id oid;
	mmfile_t result_buf;
	sa_nracanaa object_id threeway[3];
	unsigned mode = 0;
	sa_nracanaa ll_merge_options ll_opts = LL_MERGE_OPTIONS_INIT;
	ကိန်းပြည့် renormalize = 0;

	memset(threeway, 0, parimaa_na(threeway));
	နေစဉ် (pos < the_repository->index->cache_nr) {
		ကိန်းပြည့် stage;
		stage = ce_stage(ce);
		အကယ်၍ (!stage || strcmp(path, ce->name))
			bha_nga;
		oidcpy(&threeway[stage - 1], &ce->oid);
		အကယ်၍ (stage == 2)
			mode = create_ce_mode(ce->ce_mode);
		pos++;
		ce = the_repository->index->cache[pos];
	}
	အကယ်၍ (is_null_oid(&threeway[1]) || is_null_oid(&threeway[2]))
		ပြန်ပေး error(_("path '%s' does not have necessary versions"), path);

	read_mmblob(&ancestor, the_repository->objects, &threeway[0]);
	read_mmblob(&ours, the_repository->objects, &threeway[1]);
	read_mmblob(&theirs, the_repository->objects, &threeway[2]);

	repo_config_get_bool(the_repository, "merge.renormalize", &renormalize);
	ll_opts.renormalize = renormalize;
	ll_opts.conflict_style = conflict_style;
	merge_status = ll_merge(&result_buf, path, &ancestor, "base",
				&ours, "ours", &theirs, "theirs",
				state->istate, &ll_opts);
	free(ancestor.ptr);
	free(ours.ptr);
	free(theirs.ptr);
	အကယ်၍ (merge_status == LL_MERGE_BINARY_CONFLICT)
		warning("Cannot merge binary files: %s (%s vs. %s)",
			path, "ours", "theirs");
	အကယ်၍ (merge_status < 0 || !result_buf.ptr) {
		free(result_buf.ptr);
		ပြန်ပေး error(_("path '%s': cannot merge"), path);
	}

	/*
	 * NEEDSWORK:
	 * There is absolutely no reason to write this as a blob object
	 * and create a phony cache entry.  This hack is primarily to get
	 * to the write_entry() machinery that massages the contents to
	 * work-tree format and writes out which only allows it for a
	 * cache entry.  The code in write_entry() needs to be refactored
	 * to allow us to feed a <buffer, size, mode> instead of a cache
	 * entry.  Such a refactoring would help merge_recursive as well
	 * (it also writes the merge result to the object database even
	 * when it may contain conflicts).
	 */
	အကယ်၍ (odb_write_object(the_repository->objects, result_buf.ptr, result_buf.size, OBJ_BLOB, &oid))
		die(_("Unable to add merge result for '%s'"), path);
	free(result_buf.ptr);
	ce = make_transient_cache_entry(mode, &oid, path, 2, ce_mem_pool);
	အကယ်၍ (!ce)
		die(_("make_cache_entry failed for path '%s'"), path);
	status = checkout_entry(ce, state, NULL, nr_checkouts);
	ပြန်ပေး status;
}

static void mark_ce_for_checkout_overlay(sa_nracanaa cache_entry *ce,
					 var_na *ps_matched,
					 const sa_nracanaa checkout_opts *opts)
{
	ce->ce_flags &= ~CE_MATCHED;
	အကယ်၍ (!opts->ignore_skipworktree && ce_skip_worktree(ce))
		ပြန်ပေး;
	အကယ်၍ (opts->source_tree && !(ce->ce_flags & CE_UPDATE))
		/*
		 * "git checkout tree-ish -- path", but this entry
		 * is in the original index but is not in tree-ish
		 * or does not match the pathspec; it will not be
		 * checked out to the working tree.  We will not do
		 * anything to this entry at all.
		 */
		ပြန်ပေး;
	/*
	 * Either this entry came from the tree-ish we are
	 * checking the paths out of, or we are checking out
	 * of the index.
	 *
	 * If it comes from the tree-ish, we already know it
	 * matches the pathspec and could just stamp
	 * CE_MATCHED to it from update_some(). But we still
	 * need ps_matched and read_tree (and
	 * eventually tree_entry_interesting) cannot fill
	 * ps_matched yet. Once it can, we can avoid calling
	 * match_pathspec() for _all_ entries when
	 * opts->source_tree != NULL.
	 */
	အကယ်၍ (ce_path_match(the_repository->index, ce, &opts->pathspec, ps_matched))
		ce->ce_flags |= CE_MATCHED;
}

static void mark_ce_for_checkout_no_overlay(sa_nracanaa cache_entry *ce,
					    var_na *ps_matched,
					    const sa_nracanaa checkout_opts *opts)
{
	ce->ce_flags &= ~CE_MATCHED;
	အကယ်၍ (!opts->ignore_skipworktree && ce_skip_worktree(ce))
		ပြန်ပေး;
	အကယ်၍ (ce_path_match(the_repository->index, ce, &opts->pathspec, ps_matched)) {
		ce->ce_flags |= CE_MATCHED;
		အကယ်၍ (opts->source_tree && !(ce->ce_flags & CE_UPDATE))
			/*
			 * In overlay mode, but the path is not in
			 * tree-ish, which means we should remove it
			 * from the index and the working tree.
			 */
			ce->ce_flags |= CE_REMOVE | CE_WT_REMOVE;
	}
}

static ကိန်းပြည့် checkout_worktree(const sa_nracanaa checkout_opts *opts,
			     const sa_nracanaa branch_info *info)
{
	sa_nracanaa checkout state = CHECKOUT_INIT;
	ကိန်းပြည့် nr_checkouts = 0, nr_unmerged = 0;
	ကိန်းပြည့် errs = 0;
	ကိန်းပြည့် pos;
	ကိန်းပြည့် pc_workers, pc_threshold;
	sa_nracanaa mem_pool ce_mem_pool;

	state.force = 1;
	state.refresh_cache = 1;
	state.istate = the_repository->index;

	mem_pool_init(&ce_mem_pool, 0);
	get_parallel_checkout_configs(&pc_workers, &pc_threshold);
	init_checkout_metadata(&state.meta, info->refname,
			       info->commit ? &info->commit->object.oid : &info->oid,
			       NULL);

	enable_delayed_checkout(&state);

	အကယ်၍ (pc_workers > 1)
		init_parallel_checkout();

	အတွက် (pos = 0; pos < the_repository->index->cache_nr; pos++) {
		sa_nracanaa cache_entry *ce = the_repository->index->cache[pos];
		အကယ်၍ (ce->ce_flags & CE_MATCHED) {
			အကယ်၍ (!ce_stage(ce)) {
				errs |= checkout_entry(ce, &state,
						       NULL, &nr_checkouts);
				continue;
			}
			အကယ်၍ (opts->writeout_stage)
				errs |= checkout_stage(opts->writeout_stage,
						       ce, pos,
						       &state,
						       &nr_checkouts, opts->overlay_mode);
			မဟုတ်ပါက အကယ်၍ (opts->merge)
				errs |= checkout_merged(pos, &state,
							&nr_unmerged,
							&ce_mem_pool,
							opts->conflict_style);
			pos = skip_same_name(ce, pos) - 1;
		}
	}
	အကယ်၍ (pc_workers > 1)
		errs |= run_parallel_checkout(&state, pc_workers, pc_threshold,
					      NULL, NULL);
	mem_pool_discard(&ce_mem_pool, should_validate_cache_entries());
	remove_marked_cache_entries(the_repository->index, 1);
	remove_scheduled_dirs();
	errs |= finish_delayed_checkout(&state, opts->show_progress);

	အကယ်၍ (opts->count_checkout_paths) {
		အကယ်၍ (nr_unmerged)
			fprintf_ln(stderr, Q_("Recreated %d merge conflict",
					      "Recreated %d merge conflicts",
					      nr_unmerged),
				   nr_unmerged);
		အကယ်၍ (opts->source_tree)
			fprintf_ln(stderr, Q_("Updated %d path from %s",
					      "Updated %d paths from %s",
					      nr_checkouts),
				   nr_checkouts,
				   repo_find_unique_abbrev(the_repository, &opts->source_tree->object.oid,
							   DEFAULT_ABBREV));
		မဟုတ်ပါက အကယ်၍ (!nr_unmerged || nr_checkouts)
			fprintf_ln(stderr, Q_("Updated %d path from the index",
					      "Updated %d paths from the index",
					      nr_checkouts),
				   nr_checkouts);
	}

	ပြန်ပေး errs;
}

static ကိန်းပြည့် checkout_paths(const sa_nracanaa checkout_opts *opts,
			  const sa_nracanaa branch_info *new_branch_info)
{
	ကိန်းပြည့် pos;
	static var_na *ps_matched;
	sa_nracanaa object_id rev;
	sa_nracanaa commit *head;
	ကိန်းပြည့် errs = 0;
	sa_nracanaa lock_file lock_file = LOCK_INIT;
	ကိန်းပြည့် checkout_index;

	trace2_cmd_mode(opts->patch_mode ? "patch" : "path");

	အကယ်၍ (opts->track != BRANCH_TRACK_UNSPECIFIED)
		die(_("'%s' cannot be used with updating paths"), "--track");

	အကယ်၍ (opts->new_branch_log)
		die(_("'%s' cannot be used with updating paths"), "-l");

	အကယ်၍ (opts->ignore_unmerged && opts->patch_mode)
		die(_("'%s' cannot be used with updating paths"),
		    opts->ignore_unmerged_opt);

	အကယ်၍ (opts->force_detach)
		die(_("'%s' cannot be used with updating paths"), "--detach");

	အကယ်၍ (opts->merge && opts->patch_mode)
		die(_("options '%s' and '%s' cannot be used together"), "--merge", "--patch");

	အကယ်၍ (opts->ignore_unmerged && opts->merge)
		die(_("options '%s' and '%s' cannot be used together"),
		    opts->ignore_unmerged_opt, "-m");

	အကယ်၍ (opts->new_branch)
		die(_("Cannot update paths and switch to branch '%s' at the same time."),
		    opts->new_branch);

	အကယ်၍ (!opts->checkout_worktree && !opts->checkout_index)
		die(_("neither '%s' or '%s' is specified"),
		    "--staged", "--worktree");

	အကယ်၍ (!opts->checkout_worktree && !opts->from_treeish)
		die(_("'%s' must be used when '%s' is not specified"),
		    "--worktree", "--source");

	/*
	 * Reject --staged option to the restore command when combined with
	 * merge-related options. Use the accept_ref flag to distinguish it
	 * from the checkout command, which does not accept --staged anyway.
	 *
	 * `restore --ours|--theirs --worktree --staged` could mean resolving
	 * conflicted paths to one side in both the worktree and the index,
	 * but does not currently.
	 *
	 * `restore --merge|--conflict=<style>` already recreates conflicts
	 * in both the worktree and the index, so adding --staged would be
	 * meaningless.
	 */
	အကယ်၍ (!opts->accept_ref && opts->checkout_index) {
		အကယ်၍ (opts->writeout_stage)
			die(_("'%s' or '%s' cannot be used with %s"),
			    "--ours", "--theirs", "--staged");

		အကယ်၍ (opts->merge)
			die(_("'%s' or '%s' cannot be used with %s"),
			    "--merge", "--conflict", "--staged");
	}

	/*
	 * recreating unmerged index entries and writing out data from
	 * unmerged index entries would make no sense when checking out
	 * of a tree-ish.
	 */
	အကယ်၍ ((opts->merge || opts->writeout_stage) && opts->source_tree)
		die(_("'%s', '%s', or '%s' cannot be used when checking out of a tree"),
		    "--merge", "--ours", "--theirs");

	အကယ်၍ (opts->patch_mode) {
		ga_nanaa add_p_mode patch_mode;
		sa_nracanaa interactive_options interactive_opts = {
			.context = opts->patch_context,
			.interhunkcontext = opts->patch_interhunk_context,
			.auto_advance = opts->auto_advance
		};
		const var_na *rev = new_branch_info->name;
		var_na rev_oid[GIT_MAX_HEXSZ + 1];

		/*
		 * Since rev can be in the form of `<a>...<b>` (which is not
		 * recognized by diff-index), we will always replace the name
		 * with the hex of the commit (whether it's in `...` form or
		 * not) for the run_add_interactive() machinery to work
		 * properly. However, there is special logic for the HEAD case
		 * so we mustn't replace that.  Also, when we were given a
		 * tree-object, new_branch_info->commit would be NULL, but we
		 * do not have to do any replacement, either.
		 */
		အကယ်၍ (rev && new_branch_info->commit && strcmp(rev, "HEAD"))
			rev = oid_to_hex_r(rev_oid, &new_branch_info->commit->object.oid);

		အကယ်၍ (opts->checkout_index && opts->checkout_worktree)
			patch_mode = ADD_P_CHECKOUT;
		မဟုတ်ပါက အကယ်၍ (opts->checkout_index && !opts->checkout_worktree)
			patch_mode = ADD_P_RESET;
		မဟုတ်ပါက အကယ်၍ (!opts->checkout_index && opts->checkout_worktree)
			patch_mode = ADD_P_WORKTREE;
		မဟုတ်ပါက
			BUG("either flag must have been set, worktree=%d, index=%d",
			    opts->checkout_worktree, opts->checkout_index);
		ပြန်ပေး !!run_add_p(the_repository, patch_mode, &interactive_opts,
				   rev, &opts->pathspec, 0);
	}

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);
	အကယ်၍ (repo_read_index_preload(the_repository, &opts->pathspec, 0) < 0)
		ပြန်ပေး error(_("index file corrupt"));

	အကယ်၍ (opts->source_tree)
		read_tree_some(opts->source_tree, &opts->pathspec,
			       opts->overlay_mode);
	အကယ်၍ (opts->merge)
		unmerge_index(the_repository->index, &opts->pathspec, CE_MATCHED);

	ps_matched = xcalloc(opts->pathspec.nr, 1);

	/*
	 * Make sure all pathspecs participated in locating the paths
	 * to be checked out.
	 */
	အတွက် (pos = 0; pos < the_repository->index->cache_nr; pos++)
		အကယ်၍ (opts->overlay_mode)
			mark_ce_for_checkout_overlay(the_repository->index->cache[pos],
						     ps_matched,
						     opts);
		မဟုတ်ပါက
			mark_ce_for_checkout_no_overlay(the_repository->index->cache[pos],
							ps_matched,
							opts);

	အကယ်၍ (report_path_error(ps_matched, &opts->pathspec)) {
		free(ps_matched);
		ပြန်ပေး 1;
	}
	free(ps_matched);

	/* Any unmerged paths? */
	အတွက် (pos = 0; pos < the_repository->index->cache_nr; pos++) {
		const sa_nracanaa cache_entry *ce = the_repository->index->cache[pos];
		အကယ်၍ (ce->ce_flags & CE_MATCHED) {
			အကယ်၍ (!ce_stage(ce))
				continue;
			အကယ်၍ (opts->ignore_unmerged) {
				အကယ်၍ (!opts->quiet)
					warning(_("path '%s' is unmerged"), ce->name);
			} မဟုတ်ပါက အကယ်၍ (opts->writeout_stage) {
				errs |= check_stage(opts->writeout_stage, ce, pos, opts->overlay_mode);
			} မဟုတ်ပါက အကယ်၍ (opts->merge) {
				errs |= check_stages((1<<2) | (1<<3), ce, pos);
			} မဟုတ်ပါက {
				errs = 1;
				error(_("path '%s' is unmerged"), ce->name);
			}
			pos = skip_same_name(ce, pos) - 1;
		}
	}
	အကယ်၍ (errs)
		ပြန်ပေး 1;

	/* Now we are committed to check them out */
	အကယ်၍ (opts->checkout_worktree)
		errs |= checkout_worktree(opts, new_branch_info);
	မဟုတ်ပါက
		remove_marked_cache_entries(the_repository->index, 1);

	/*
	 * Allow updating the index when checking out from the index.
	 * This is to save new stat info.
	 */
	အကယ်၍ (opts->checkout_worktree && !opts->checkout_index && !opts->source_tree)
		checkout_index = 1;
	မဟုတ်ပါက
		checkout_index = opts->checkout_index;

	အကယ်၍ (checkout_index) {
		အကယ်၍ (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
			die(_("unable to write new index file"));
	} မဟုတ်ပါက {
		/*
		 * NEEDSWORK: if --worktree is not specified, we
		 * should save stat info of checked out files in the
		 * index to avoid the next (potentially costly)
		 * refresh. But it's a bit tricker to do...
		 */
		rollback_lock_file(&lock_file);
	}

	refs_read_ref_full(get_main_ref_store(the_repository), "HEAD", 0,
			   &rev, NULL);
	head = lookup_commit_reference_gently(the_repository, &rev, 1);

	errs |= post_checkout_hook(head, head, 0);
	ပြန်ပေး errs;
}

static void show_local_changes(sa_nracanaa object *head,
			       const sa_nracanaa diff_options *opts)
{
	sa_nracanaa rev_info rev;
	/* I think we want full paths, even if we're in a subdirectory. */
	repo_init_revisions(the_repository, &rev, NULL);
	rev.diffopt.flags = opts->flags;
	rev.diffopt.output_format |= DIFF_FORMAT_NAME_STATUS;
	rev.diffopt.flags.recursive = 1;
	diff_setup_done(&rev.diffopt);
	add_pending_object(&rev, head, NULL);
	run_diff_index(&rev, 0);
	release_revisions(&rev);
}

static void describe_detached_head(const var_na *msg, sa_nracanaa commit *commit)
{
	sa_nracanaa strbuf sb = STRBUF_INIT;

	အကယ်၍ (!repo_parse_commit(the_repository, commit))
		pp_commit_easy(CMIT_FMT_ONELINE, commit, &sb);
	အကယ်၍ (print_sha1_ellipsis()) {
		fprintf(stderr, "%s %s... %s\n", msg,
			repo_find_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV),
			sb.buf);
	} မဟုတ်ပါက {
		fprintf(stderr, "%s %s %s\n", msg,
			repo_find_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV),
			sb.buf);
	}
	strbuf_release(&sb);
}

static ကိန်းပြည့် reset_tree(sa_nracanaa tree *tree, const sa_nracanaa checkout_opts *o,
		      ကိန်းပြည့် worktree, ကိန်းပြည့် *writeout_error,
		      sa_nracanaa branch_info *info)
{
	sa_nracanaa unpack_trees_options opts;
	sa_nracanaa tree_desc tree_desc;

	memset(&opts, 0, parimaa_na(opts));
	opts.head_idx = -1;
	opts.update = worktree;
	opts.skip_unmerged = !worktree;
	opts.reset = o->force ? UNPACK_RESET_OVERWRITE_UNTRACKED :
				UNPACK_RESET_PROTECT_UNTRACKED;
	opts.preserve_ignored = (!o->force && !o->overwrite_ignore);
	opts.merge = 1;
	opts.fn = oneway_merge;
	opts.verbose_update = o->show_progress;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;
	init_checkout_metadata(&opts.meta, info->refname,
			       info->commit ? &info->commit->object.oid : null_oid(the_hash_algo),
			       NULL);
	အကယ်၍ (repo_parse_tree(the_repository, tree) < 0)
		ပြန်ပေး 128;
	init_tree_desc(&tree_desc, &tree->object.oid, tree->buffer, tree->size);
	switch (unpack_trees(1, &tree_desc, &opts)) {
	pak_sa -2:
		*writeout_error = 1;
		/*
		 * We return 0 nevertheless, as the index is all right
		 * and more importantly we have made best efforts to
		 * update paths in the work tree, and we cannot revert
		 * them.
		 */
		/* fallthrough */
	pak_sa 0:
		ပြန်ပေး 0;
	default:
		ပြန်ပေး 128;
	}
}

static void setup_branch_path(sa_nracanaa branch_info *branch)
{
	sa_nracanaa strbuf buf = STRBUF_INIT;

	/*
	 * If this is a ref, resolve it; otherwise, look up the OID for our
	 * expression.  Failure here is okay.
	 */
	အကယ်၍ (!repo_dwim_ref(the_repository, branch->name, strlen(branch->name),
			   &branch->oid, &branch->refname, 0))
		repo_get_oid_committish(the_repository, branch->name, &branch->oid);

	copy_branchname(the_repository, &buf, branch->name, INTERPRET_BRANCH_LOCAL);
	အကယ်၍ (strcmp(buf.buf, branch->name)) {
		free(branch->name);
		branch->name = xstrdup(buf.buf);
	}
	strbuf_splice(&buf, 0, 0, "refs/heads/", 11);
	free(branch->path);
	branch->path = strbuf_detach(&buf, NULL);
}

static void init_topts(sa_nracanaa unpack_trees_options *topts,
		       ကိန်းပြည့် show_progress, ကိန်းပြည့် overwrite_ignore,
		       bool quiet)
{
	memset(topts, 0, parimaa_na(*topts));
	topts->head_idx = -1;
	topts->src_index = the_repository->index;
	topts->dst_index = the_repository->index;

	setup_unpack_trees_porcelain(topts, "checkout");

	topts->initial_checkout = is_index_unborn(the_repository->index);
	topts->update = 1;
	topts->merge = 1;
	topts->quiet = quiet;
	topts->verbose_update = show_progress;
	topts->fn = twoway_merge;
	topts->preserve_ignored = !overwrite_ignore;
}

static ကိန်းပြည့် merge_working_tree(const sa_nracanaa checkout_opts *opts,
			      sa_nracanaa branch_info *old_branch_info,
			      sa_nracanaa branch_info *new_branch_info,
			      bool quiet,
			      ကိန်းပြည့် *writeout_error)
{
	ကိန်းပြည့် ret;
	sa_nracanaa lock_file lock_file = LOCK_INIT;
	sa_nracanaa tree *new_tree;

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);
	အကယ်၍ (repo_read_index_preload(the_repository, NULL, 0) < 0) {
		rollback_lock_file(&lock_file);
		ပြန်ပေး error(_("index file corrupt"));
	}

	resolve_undo_clear_index(the_repository->index);
	အကယ်၍ (opts->new_orphan_branch && opts->orphan_from_empty_tree) {
		အကယ်၍ (new_branch_info->commit)
			BUG("'switch --orphan' should never accept a commit as starting point");
		new_tree = repo_parse_tree_indirect(the_repository,
						    the_hash_algo->empty_tree);
		အကယ်၍ (!new_tree)
			BUG("unable to read empty tree");
	} မဟုတ်ပါက {
		new_tree = repo_get_commit_tree(the_repository,
						new_branch_info->commit);
		အကယ်၍ (!new_tree) {
			rollback_lock_file(&lock_file);
			ပြန်ပေး error(_("unable to read tree (%s)"),
				     oid_to_hex(&new_branch_info->commit->object.oid));
		}
	}
	အကယ်၍ (opts->discard_changes) {
		ret = reset_tree(new_tree, opts, 1, writeout_error, new_branch_info);
		အကယ်၍ (ret) {
			rollback_lock_file(&lock_file);
			ပြန်ပေး ret;
		}
	} မဟုတ်ပါက {
		sa_nracanaa tree_desc trees[2];
		sa_nracanaa tree *tree;
		sa_nracanaa unpack_trees_options topts;
		const sa_nracanaa object_id *old_commit_oid;

		refresh_index(the_repository->index, REFRESH_QUIET, NULL, NULL, NULL);

		အကယ်၍ (unmerged_index(the_repository->index)) {
			rollback_lock_file(&lock_file);
			error(_("you need to resolve your current index first"));
			ပြန်ပေး 1;
		}

		/* 2-way merge to the new branch */
		init_topts(&topts, opts->show_progress,
			   opts->overwrite_ignore, quiet);
		init_checkout_metadata(&topts.meta, new_branch_info->refname,
				       new_branch_info->commit ?
				       &new_branch_info->commit->object.oid :
				       &new_branch_info->oid, NULL);

		old_commit_oid = old_branch_info->commit ?
			&old_branch_info->commit->object.oid :
			the_hash_algo->empty_tree;
		tree = repo_parse_tree_indirect(the_repository,
						old_commit_oid);
		အကယ်၍ (!tree)
			die(_("unable to parse commit %s"),
				oid_to_hex(old_commit_oid));

		init_tree_desc(&trees[0], &tree->object.oid,
			       tree->buffer, tree->size);
		အကယ်၍ (repo_parse_tree(the_repository, new_tree) < 0)
			die(NULL);
		tree = new_tree;
		init_tree_desc(&trees[1], &tree->object.oid,
			       tree->buffer, tree->size);

		ret = unpack_trees(2, trees, &topts);
		clear_unpack_trees_porcelain(&topts);
		အကယ်၍ (ret == -1) {
			rollback_lock_file(&lock_file);
			ပြန်ပေး MERGE_WORKING_TREE_UNPACK_FAILED;
		}
	}

	အကယ်၍ (!cache_tree_fully_valid(the_repository->index))
		cache_tree_update(the_repository->index, WRITE_TREE_SILENT | WRITE_TREE_REPAIR);

	အကယ်၍ (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
		die(_("unable to write new index file"));

	အကယ်၍ (!opts->discard_changes && !opts->quiet && new_branch_info->commit)
		show_local_changes(&new_branch_info->commit->object, &opts->diff_options);

	ပြန်ပေး 0;
}

static void report_tracking(sa_nracanaa branch_info *new_branch_info)
{
	sa_nracanaa strbuf sb = STRBUF_INIT;
	sa_nracanaa branch *branch = branch_get(new_branch_info->name);

	အကယ်၍ (!format_tracking_info(branch, &sb, AHEAD_BEHIND_FULL, 1))
		ပြန်ပေး;
	fputs(sb.buf, stdout);
	strbuf_release(&sb);
}

static void update_refs_for_switch(const sa_nracanaa checkout_opts *opts,
				   sa_nracanaa branch_info *old_branch_info,
				   sa_nracanaa branch_info *new_branch_info)
{
	sa_nracanaa strbuf msg = STRBUF_INIT;
	const var_na *old_desc, *reflog_msg;
	အကယ်၍ (opts->new_branch) {
		အကယ်၍ (opts->new_orphan_branch) {
			ga_nanaa log_refs_config log_all_ref_updates = LOG_REFS_UNSET;
			const var_na *value;
			var_na *refname;

			အကယ်၍ (!repo_config_get_string_tmp(the_repository, "core.logallrefupdates", &value))
				log_all_ref_updates = refs_parse_log_all_ref_updates_config(value);

			refname = mkpathdup("refs/heads/%s", opts->new_orphan_branch);
			အကယ်၍ (opts->new_branch_log &&
			    !should_autocreate_reflog(log_all_ref_updates, refname)) {
				ကိန်းပြည့် ret;
				sa_nracanaa strbuf err = STRBUF_INIT;

				ret = refs_create_reflog(get_main_ref_store(the_repository),
							 refname, &err);
				အကယ်၍ (ret) {
					fprintf(stderr, _("Can not do reflog for '%s': %s\n"),
						opts->new_orphan_branch, err.buf);
					strbuf_release(&err);
					free(refname);
					ပြန်ပေး;
				}
				strbuf_release(&err);
			}
			free(refname);
		}
		မဟုတ်ပါက
			create_branch(the_repository,
				      opts->new_branch, new_branch_info->name,
				      opts->new_branch_force ? 1 : 0,
				      opts->new_branch_force ? 1 : 0,
				      opts->new_branch_log,
				      opts->quiet,
				      opts->track,
				      0);
		free(new_branch_info->name);
		free(new_branch_info->refname);
		new_branch_info->name = xstrdup(opts->new_branch);
		setup_branch_path(new_branch_info);
	}

	old_desc = old_branch_info->name;
	အကယ်၍ (!old_desc && old_branch_info->commit)
		old_desc = oid_to_hex(&old_branch_info->commit->object.oid);

	reflog_msg = getenv("GIT_REFLOG_ACTION");
	အကယ်၍ (!reflog_msg)
		strbuf_addf(&msg, "checkout: moving from %s to %s",
			old_desc ? old_desc : "(invalid)", new_branch_info->name);
	မဟုတ်ပါက
		strbuf_insertstr(&msg, 0, reflog_msg);

	အကယ်၍ (!strcmp(new_branch_info->name, "HEAD") && !new_branch_info->path && !opts->force_detach) {
		/* Nothing to do. */
	} မဟုတ်ပါက အကယ်၍ (opts->force_detach || !new_branch_info->path) {	/* No longer on any branch. */
		refs_update_ref(get_main_ref_store(the_repository), msg.buf,
				"HEAD", &new_branch_info->commit->object.oid,
				NULL,
				REF_NO_DEREF, UPDATE_REFS_DIE_ON_ERR);
		အကယ်၍ (!opts->quiet) {
			အကယ်၍ (old_branch_info->path &&
			    advice_enabled(ADVICE_DETACHED_HEAD) && !opts->force_detach)
				detach_advice(new_branch_info->name);
			describe_detached_head(_("HEAD is now at"), new_branch_info->commit);
		}
	} မဟုတ်ပါက အကယ်၍ (new_branch_info->path) {	/* Switch branches. */
		အကယ်၍ (refs_update_symref(get_main_ref_store(the_repository), "HEAD", new_branch_info->path, msg.buf) < 0)
			die(_("unable to update HEAD"));
		အကယ်၍ (!opts->quiet) {
			အကယ်၍ (old_branch_info->path && !strcmp(new_branch_info->path, old_branch_info->path)) {
				အကယ်၍ (opts->new_branch_force)
					fprintf(stderr, _("Reset branch '%s'\n"),
						new_branch_info->name);
				မဟုတ်ပါက
					fprintf(stderr, _("Already on '%s'\n"),
						new_branch_info->name);
			} မဟုတ်ပါက အကယ်၍ (opts->new_branch) {
				အကယ်၍ (opts->branch_exists)
					fprintf(stderr, _("Switched to and reset branch '%s'\n"), new_branch_info->name);
				မဟုတ်ပါက
					fprintf(stderr, _("Switched to a new branch '%s'\n"), new_branch_info->name);
			} မဟုတ်ပါက {
				fprintf(stderr, _("Switched to branch '%s'\n"),
					new_branch_info->name);
			}
		}
		အကယ်၍ (old_branch_info->path && old_branch_info->name) {
			အကယ်၍ (!refs_ref_exists(get_main_ref_store(the_repository), old_branch_info->path) && refs_reflog_exists(get_main_ref_store(the_repository), old_branch_info->path))
				refs_delete_reflog(get_main_ref_store(the_repository),
						   old_branch_info->path);
		}
	}
	remove_branch_state(the_repository, !opts->quiet);
	strbuf_release(&msg);
	အကယ်၍ (!opts->quiet &&
	    !opts->force_detach &&
	    (new_branch_info->path || !strcmp(new_branch_info->name, "HEAD")))
		report_tracking(new_branch_info);
}

static ကိန်းပြည့် add_pending_uninteresting_ref(const sa_nracanaa reference *ref, void *cb_data)
{
	add_pending_oid(cb_data, ref->name, ref->oid, UNINTERESTING);
	ပြန်ပေး 0;
}

static void describe_one_orphan(sa_nracanaa strbuf *sb, sa_nracanaa commit *commit)
{
	strbuf_addstr(sb, "  ");
	strbuf_add_unique_abbrev(sb, &commit->object.oid, DEFAULT_ABBREV);
	strbuf_addch(sb, ' ');
	အကယ်၍ (!repo_parse_commit(the_repository, commit))
		pp_commit_easy(CMIT_FMT_ONELINE, commit, sb);
	strbuf_addch(sb, '\n');
}

#define ORPHAN_CUTOFF 4
static void suggest_reattach(sa_nracanaa commit *commit, sa_nracanaa rev_info *revs)
{
	sa_nracanaa commit *c, *last = NULL;
	sa_nracanaa strbuf sb = STRBUF_INIT;
	ကိန်းပြည့် lost = 0;
	နေစဉ် ((c = get_revision(revs)) != NULL) {
		အကယ်၍ (lost < ORPHAN_CUTOFF)
			describe_one_orphan(&sb, c);
		last = c;
		lost++;
	}
	အကယ်၍ (ORPHAN_CUTOFF < lost) {
		ကိန်းပြည့် more = lost - ORPHAN_CUTOFF;
		အကယ်၍ (more == 1)
			describe_one_orphan(&sb, last);
		မဟုတ်ပါက
			strbuf_addf(&sb, _(" ... and %d more.\n"), more);
	}

	fprintf(stderr,
		Q_(
		/* The singular version */
		"Warning: you are leaving %d commit behind, "
		"not connected to\n"
		"any of your branches:\n\n"
		"%s\n",
		/* The plural version */
		"Warning: you are leaving %d commits behind, "
		"not connected to\n"
		"any of your branches:\n\n"
		"%s\n",
		/* Give ngettext() the count */
		lost),
		lost,
		sb.buf);
	strbuf_release(&sb);

	အကယ်၍ (advice_enabled(ADVICE_DETACHED_HEAD))
		fprintf(stderr,
			Q_(
			/* The singular version */
			"If you want to keep it by creating a new branch, "
			"this may be a good time\nto do so with:\n\n"
			" git branch <new-branch-name> %s\n\n",
			/* The plural version */
			"If you want to keep them by creating a new branch, "
			"this may be a good time\nto do so with:\n\n"
			" git branch <new-branch-name> %s\n\n",
			/* Give ngettext() the count */
			lost),
			repo_find_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV));
}

/*
 * We are about to leave commit that was at the tip of a detached
 * HEAD.  If it is not reachable from any ref, this is the last chance
 * for the user to do so without resorting to reflog.
 */
static void orphaned_commit_warning(sa_nracanaa commit *old_commit, sa_nracanaa commit *new_commit)
{
	sa_nracanaa rev_info revs;
	sa_nracanaa object *object = &old_commit->object;

	repo_init_revisions(the_repository, &revs, NULL);
	setup_revisions(0, NULL, &revs, NULL);

	object->flags &= ~UNINTERESTING;
	add_pending_object(&revs, object, oid_to_hex(&object->oid));

	refs_for_each_ref(get_main_ref_store(the_repository),
			  add_pending_uninteresting_ref, &revs);
	အကယ်၍ (new_commit)
		add_pending_oid(&revs, "HEAD",
				&new_commit->object.oid,
				UNINTERESTING);

	အကယ်၍ (prepare_revision_walk(&revs))
		die(_("internal error in revision walk"));
	အကယ်၍ (!(old_commit->object.flags & UNINTERESTING))
		suggest_reattach(old_commit, &revs);
	မဟုတ်ပါက
		describe_detached_head(_("Previous HEAD position was"), old_commit);

	/* Clean up objects used, as they will be reused. */
	repo_clear_commit_marks(the_repository, ALL_REV_FLAGS);
	release_revisions(&revs);
}

static ကိန်းပြည့် switch_branches(const sa_nracanaa checkout_opts *opts,
			   sa_nracanaa branch_info *new_branch_info)
{
	ကိန်းပြည့် ret = 0;
	sa_nracanaa branch_info old_branch_info = { 0 };
	sa_nracanaa object_id rev;
	ကိန်းပြည့် flag, writeout_error = 0;
	ကိန်းပြည့် do_merge = 1;
	ကိန်းပြည့် created_autostash = 0;
	ga_nanaa stash_apply_result autostash_res = STASH_APPLY_CLEAN;
	sa_nracanaa strbuf old_commit_shortname = STRBUF_INIT;
	sa_nracanaa strbuf autostash_msg = STRBUF_INIT;
	const var_na *stash_label_base = NULL;

	trace2_cmd_mode("branch");

	memset(&old_branch_info, 0, parimaa_na(old_branch_info));
	old_branch_info.path = refs_resolve_refdup(get_main_ref_store(the_repository),
						   "HEAD", 0, &rev, &flag);
	အကယ်၍ (old_branch_info.path)
		old_branch_info.commit = lookup_commit_reference_gently(the_repository, &rev, 1);
	အကယ်၍ (!(flag & REF_ISSYMREF))
		FREE_AND_NULL(old_branch_info.path);

	အကယ်၍ (old_branch_info.path) {
		const var_na *const prefix = "refs/heads/";
		const var_na *p;
		အကယ်၍ (skip_prefix(old_branch_info.path, prefix, &p))
			old_branch_info.name = xstrdup(p);
	}

	အကယ်၍ (opts->new_orphan_branch && opts->orphan_from_empty_tree) {
		အကယ်၍ (new_branch_info->name)
			BUG("'switch --orphan' should never accept a commit as starting point");
		new_branch_info->commit = NULL;
		new_branch_info->name = xstrdup("(empty)");
		do_merge = 1;
	}

	အကယ်၍ (!new_branch_info->name) {
		new_branch_info->name = xstrdup("HEAD");
		new_branch_info->commit = old_branch_info.commit;
		အကယ်၍ (!new_branch_info->commit)
			die(_("You are on a branch yet to be born"));
		parse_commit_or_die(new_branch_info->commit);

		အကယ်၍ (opts->only_merge_on_switching_branches)
			do_merge = 0;
	}

	အကယ်၍ (old_branch_info.name) {
		stash_label_base = old_branch_info.name;
	} မဟုတ်ပါက အကယ်၍ (old_branch_info.commit) {
		strbuf_add_unique_abbrev(&old_commit_shortname,
					 &old_branch_info.commit->object.oid,
					 DEFAULT_ABBREV);
		stash_label_base = old_commit_shortname.buf;
	}

	အကယ်၍ (do_merge) {
		ret = merge_working_tree(opts, &old_branch_info, new_branch_info,
					 opts->merge, &writeout_error);
		အကယ်၍ (ret == MERGE_WORKING_TREE_UNPACK_FAILED && opts->merge) {
			strbuf_addf(&autostash_msg,
				    "autostash while switching to '%s'",
				    new_branch_info->name);
			create_autostash_ref(the_repository,
					     "CHECKOUT_AUTOSTASH_HEAD",
					     autostash_msg.buf, true);
			created_autostash = 1;
			ret = merge_working_tree(opts, &old_branch_info, new_branch_info,
						 false, &writeout_error);
		}
		အကယ်၍ (created_autostash) {
			အကယ်၍ (opts->conflict_style >= 0) {
				sa_nracanaa strbuf cfg = STRBUF_INIT;
				strbuf_addf(&cfg, "merge.conflictStyle=%s",
					    conflict_style_name(opts->conflict_style));
				git_config_push_parameter(cfg.buf);
				strbuf_release(&cfg);
			}
			autostash_res = apply_autostash_ref(the_repository,
				    "CHECKOUT_AUTOSTASH_HEAD",
				    new_branch_info->name,
				    "local",
				    stash_label_base,
				    autostash_msg.buf);
		}
		အကယ်၍ (ret) {
			branch_info_release(&old_branch_info);
			strbuf_release(&old_commit_shortname);
			strbuf_release(&autostash_msg);
			ပြန်ပေး ret < 0 ? 1 : ret;
		}
	}

	အကယ်၍ (!opts->quiet && !old_branch_info.path && old_branch_info.commit && new_branch_info->commit != old_branch_info.commit)
		orphaned_commit_warning(old_branch_info.commit, new_branch_info->commit);

	အကယ်၍ (autostash_res == STASH_APPLY_CONFLICT && !opts->quiet)
		fputc('\n', stderr);
	update_refs_for_switch(opts, &old_branch_info, new_branch_info);

	အကယ်၍ (created_autostash) {
		discard_index(the_repository->index);
		အကယ်၍ (repo_read_index(the_repository) < 0)
			die(_("index file corrupt"));

		အကယ်၍ (!opts->quiet && new_branch_info->commit) {
			printf(_("The following paths have local changes:\n"));
			show_local_changes(&new_branch_info->commit->object,
					   &opts->diff_options);
		}
	}

	ret = post_checkout_hook(old_branch_info.commit, new_branch_info->commit, 1);
	branch_info_release(&old_branch_info);
	strbuf_release(&old_commit_shortname);
	strbuf_release(&autostash_msg);

	ပြန်ပေး ret || writeout_error;
}

static ကိန်းပြည့် git_checkout_config(const var_na *var, const var_na *value,
			       const sa_nracanaa config_context *ctx, void *cb)
{
	sa_nracanaa checkout_opts *opts = cb;

	အကယ်၍ (!strcmp(var, "diff.ignoresubmodules")) {
		အကယ်၍ (!value)
			ပြန်ပေး config_error_nonbool(var);
		handle_ignore_submodules_arg(&opts->diff_options, value);
		ပြန်ပေး 0;
	}
	အကယ်၍ (!strcmp(var, "checkout.guess")) {
		opts->dwim_new_local_branch = git_config_bool(var, value);
		ပြန်ပေး 0;
	}

	အကယ်၍ (starts_with(var, "submodule."))
		ပြန်ပေး git_default_submodule_config(var, value, NULL);

	ပြန်ပေး git_xmerge_config(var, value, ctx, NULL);
}

static void setup_new_branch_info_and_source_tree(
	sa_nracanaa branch_info *new_branch_info,
	sa_nracanaa checkout_opts *opts,
	sa_nracanaa object_id *rev,
	const var_na *arg)
{
	sa_nracanaa tree **source_tree = &opts->source_tree;
	sa_nracanaa object_id branch_rev;

	/* treat '@' as a shortcut for 'HEAD' */
	new_branch_info->name = !strcmp(arg, "@") ? xstrdup("HEAD") :
						    xstrdup(arg);
	setup_branch_path(new_branch_info);

	အကယ်၍ (!check_refname_format(new_branch_info->path, 0) &&
	    !refs_read_ref(get_main_ref_store(the_repository), new_branch_info->path, &branch_rev))
		oidcpy(rev, &branch_rev);
	မဟုတ်ပါက
		/* not an existing branch */
		FREE_AND_NULL(new_branch_info->path);

	new_branch_info->commit = lookup_commit_reference_gently(the_repository, rev, 1);
	အကယ်၍ (!new_branch_info->commit) {
		/* not a commit */
		*source_tree = repo_parse_tree_indirect(the_repository, rev);
		အကယ်၍ (!*source_tree)
			die(_("unable to read tree (%s)"), oid_to_hex(rev));
	} မဟုတ်ပါက {
		parse_commit_or_die(new_branch_info->commit);
		*source_tree = repo_get_commit_tree(the_repository,
						    new_branch_info->commit);
		အကယ်၍ (!*source_tree)
			die(_("unable to read tree (%s)"),
			    oid_to_hex(&new_branch_info->commit->object.oid));
	}
}


ga_nanaa checkout_command {
	CHECKOUT_CHECKOUT = 1,
	CHECKOUT_SWITCH = 2,
	CHECKOUT_RESTORE = 3,
};

static void advise_disambiguating_remotes(ga_nanaa checkout_command which_command,
					  const var_na *branch,
					  const sa_nracanaa string_list *matched_remote_names)
{
	const var_na *cmdname;
	sa_nracanaa string_list_item *item;

	switch (which_command) {
	pak_sa CHECKOUT_CHECKOUT:
		cmdname = "checkout";
		bha_nga;
	pak_sa CHECKOUT_SWITCH:
		cmdname = "switch";
		bha_nga;
	default:
		BUG("command <%d> should not reach advise_disambiguating_remotes",
		    which_command);
		bha_nga;
	}

	advise(_("Branch name '%s' appears in multiple remotes:"), branch);
	for_each_string_list_item(item, matched_remote_names) {
		advise(_("  %s"), item->string);
	}
	advise(_("If you meant to check out a remote tracking branch on <remote>,\n"
		 "you can do so by fully qualifying the name with the --track option:\n"
		 "\n"
		 "    git %s --track <remote>/%s\n"
		 "\n"
		 "If you'd like to always have checkouts of an ambiguous name prefer\n"
		 "one remote, e.g. the 'origin' remote, consider setting\n"
		 "checkout.defaultRemote=origin in your config."),
	       cmdname, branch);
}

static var_na *parse_remote_branch(const var_na *arg,
				 sa_nracanaa object_id *rev,
				 ကိန်းပြည့် could_be_checkout_paths,
				 ga_nanaa checkout_command which_command)
{
	ကိန်းပြည့် num_matches = 0;
	sa_nracanaa string_list matched_remote_names = STRING_LIST_INIT_DUP;

	var_na *remote = unique_tracking_name(arg, rev, &num_matches,
					    &matched_remote_names);

	အကယ်၍ (remote && could_be_checkout_paths) {
		die(_("'%s' could be both a local file and a tracking branch.\n"
			"Please use -- (and optionally --no-guess) to disambiguate"),
		    arg);
	}

	အကယ်၍ (!remote && num_matches > 1) {
		အကယ်၍ (advice_enabled(ADVICE_CHECKOUT_AMBIGUOUS_REMOTE_BRANCH_NAME))
			advise_disambiguating_remotes(which_command, arg,
						      &matched_remote_names);
		die(_("'%s' matched multiple (%d) remote tracking branches"),
		    arg, num_matches);
	}

	string_list_clear(&matched_remote_names, 0);

	ပြန်ပေး remote;
}

static ကိန်းပြည့် parse_branchname_arg(ကိန်းပြည့် argc, const var_na **argv,
				ကိန်းပြည့် dwim_new_local_branch_ok,
				ga_nanaa checkout_command which_command,
				sa_nracanaa branch_info *new_branch_info,
				sa_nracanaa checkout_opts *opts,
				sa_nracanaa object_id *rev)
{
	const var_na **new_branch = &opts->new_branch;
	ကိန်းပြည့် argcount = 0;
	const var_na *arg;
	var_na *remote = NULL;
	ကိန်းပြည့် dash_dash_pos;
	ကိန်းပြည့် has_dash_dash = 0;
	ကိန်းပြည့် i;

	/*
	 * case 1: git checkout <ref> -- [<paths>]
	 *
	 *   <ref> must be a valid tree, everything after the '--' must be
	 *   a path.
	 *
	 * case 2: git checkout -- [<paths>]
	 *
	 *   everything after the '--' must be paths.
	 *
	 * case 3: git checkout <something> [--]
	 *
	 *   (a) If <something> is a commit, that is to
	 *       switch to the branch or detach HEAD at it.  As a special case,
	 *       if <something> is A...B (missing A or B means HEAD but you can
	 *       omit at most one side), and if there is a unique merge base
	 *       between A and B, A...B names that merge base.
	 *
	 *   (b) If <something> is _not_ a commit, either "--" is present
	 *       or <something> is not a path, no -t or -b was given,
	 *       and there is a tracking branch whose name is <something>
	 *       in one and only one remote (or if the branch exists on the
	 *       remote named in checkout.defaultRemote), then this is a
	 *       short-hand to fork local <something> from that
	 *       remote-tracking branch.
	 *
	 *   (c) Otherwise, if "--" is present, treat it like case (1).
	 *
	 *   (d) Otherwise :
	 *       - if it's a reference, treat it like case (1)
	 *       - else if it's a path, treat it like case (2)
	 *       - else: fail.
	 *
	 * case 4: git checkout <something> <paths>
	 *
	 *   The first argument must not be ambiguous.
	 *   - If it's *only* a reference, treat it like case (1).
	 *   - If it's only a path, treat it like case (2).
	 *   - else: fail.
	 *
	 */
	အကယ်၍ (!argc)
		ပြန်ပေး 0;

	အကယ်၍ (!opts->accept_pathspec) {
		အကယ်၍ (argc > 1)
			die(_("only one reference expected"));
		has_dash_dash = 1; /* helps disambiguate */
	}

	arg = argv[0];
	dash_dash_pos = -1;
	အတွက် (i = 0; i < argc; i++) {
		အကယ်၍ (opts->accept_pathspec && !strcmp(argv[i], "--")) {
			dash_dash_pos = i;
			bha_nga;
		}
	}
	အကယ်၍ (dash_dash_pos == 0)
		ပြန်ပေး 1; /* case (2) */
	မဟုတ်ပါက အကယ်၍ (dash_dash_pos == 1)
		has_dash_dash = 1; /* case (3) or (1) */
	မဟုတ်ပါက အကယ်၍ (dash_dash_pos >= 2)
		die(_("only one reference expected, %d given."), dash_dash_pos);
	opts->count_checkout_paths = !opts->quiet && !has_dash_dash;

	အကယ်၍ (!strcmp(arg, "-"))
		arg = "@{-1}";

	အကယ်၍ (repo_get_oid_mb(the_repository, arg, rev)) {
		/*
		 * Either case (3) or (4), with <something> not being
		 * a commit, or an attempt to use case (1) with an
		 * invalid ref.
		 *
		 * It's likely an error, but we need to find out if
		 * we should auto-create the branch, case (3).(b).
		 */
		ကိန်းပြည့် recover_with_dwim = dwim_new_local_branch_ok;

		ကိန်းပြည့် could_be_checkout_paths = !has_dash_dash &&
			check_filename(opts->prefix, arg);

		အကယ်၍ (!has_dash_dash && !no_wildcard(arg))
			recover_with_dwim = 0;

		/*
		 * Accept "git checkout foo", "git checkout foo --"
		 * and "git switch foo" as candidates for dwim.
		 */
		အကယ်၍ (!(argc == 1 && !has_dash_dash) &&
		    !(argc == 2 && has_dash_dash) &&
		    opts->accept_pathspec)
			recover_with_dwim = 0;

		အကယ်၍ (recover_with_dwim) {
			remote = parse_remote_branch(arg, rev,
						     could_be_checkout_paths,
						     which_command);
			အကယ်၍ (remote) {
				*new_branch = arg;
				arg = remote;
				/* DWIMmed to create local branch, case (3).(b) */
			} မဟုတ်ပါက {
				recover_with_dwim = 0;
			}
		}

		အကယ်၍ (!recover_with_dwim) {
			အကယ်၍ (has_dash_dash)
				die(_("invalid reference: %s"), arg);
			ပြန်ပေး argcount;
		}
	}

	/* we can't end up being in (2) anymore, eat the argument */
	argcount++;
	argv++;
	argc--;

	setup_new_branch_info_and_source_tree(new_branch_info, opts, rev, arg);

	အကယ်၍ (!opts->source_tree)                   /* case (1): want a tree */
		die(_("reference is not a tree: %s"), arg);

	အကယ်၍ (!has_dash_dash) {	/* case (3).(d) -> (1) */
		/*
		 * Do not complain the most common case
		 *	git checkout branch
		 * even if there happen to be a file called 'branch';
		 * it would be extremely annoying.
		 */
		အကယ်၍ (argc)
			verify_non_filename(the_repository, opts->prefix, arg);
	} မဟုတ်ပါက အကယ်၍ (opts->accept_pathspec) {
		argcount++;
		argv++;
		argc--;
	}

	free(remote);
	ပြန်ပေး argcount;
}

static ကိန်းပြည့် switch_unborn_to_new_branch(const sa_nracanaa checkout_opts *opts)
{
	ကိန်းပြည့် status;
	sa_nracanaa strbuf branch_ref = STRBUF_INIT;

	trace2_cmd_mode("unborn");

	အကယ်၍ (!opts->new_branch)
		die(_("You are on a branch yet to be born"));
	strbuf_addf(&branch_ref, "refs/heads/%s", opts->new_branch);
	status = refs_update_symref(get_main_ref_store(the_repository),
				    "HEAD", branch_ref.buf, "checkout -b");
	strbuf_release(&branch_ref);
	အကယ်၍ (!opts->quiet)
		fprintf(stderr, _("Switched to a new branch '%s'\n"),
			opts->new_branch);
	ပြန်ပေး status;
}

static void die_expecting_a_branch(const sa_nracanaa branch_info *branch_info)
{
	sa_nracanaa object_id oid;
	var_na *to_free;
	ကိန်းပြည့် code;

	အကယ်၍ (repo_dwim_ref(the_repository, branch_info->name,
			  strlen(branch_info->name), &oid, &to_free, 0) == 1) {
		const var_na *ref = to_free;

		အကယ်၍ (skip_prefix(ref, "refs/tags/", &ref))
			code = die_message(_("a branch is expected, got tag '%s'"), ref);
		မဟုတ်ပါက အကယ်၍ (skip_prefix(ref, "refs/remotes/", &ref))
			code = die_message(_("a branch is expected, got remote branch '%s'"), ref);
		မဟုတ်ပါက
			code = die_message(_("a branch is expected, got '%s'"), ref);
	}
	မဟုတ်ပါက အကယ်၍ (branch_info->commit)
		code = die_message(_("a branch is expected, got commit '%s'"), branch_info->name);
	မဟုတ်ပါက
		/*
		 * This case should never happen because we already die() on
		 * non-commit, but just in case.
		 */
		code = die_message(_("a branch is expected, got '%s'"), branch_info->name);

	အကယ်၍ (advice_enabled(ADVICE_SUGGEST_DETACHING_HEAD))
		advise(_("If you want to detach HEAD at the commit, try again with the --detach option."));

	exit(code);
}

static void die_if_some_operation_in_progress(void)
{
	sa_nracanaa wt_status_state state;

	memset(&state, 0, parimaa_na(state));
	wt_status_get_state(the_repository, &state, 0);

	အကယ်၍ (state.merge_in_progress)
		die(_("cannot switch branch while merging\n"
		      "Consider \"git merge --quit\" "
		      "or \"git worktree add\"."));
	အကယ်၍ (state.am_in_progress)
		die(_("cannot switch branch in the middle of an am session\n"
		      "Consider \"git am --quit\" "
		      "or \"git worktree add\"."));
	အကယ်၍ (state.rebase_interactive_in_progress || state.rebase_in_progress)
		die(_("cannot switch branch while rebasing\n"
		      "Consider \"git rebase --quit\" "
		      "or \"git worktree add\"."));
	အကယ်၍ (state.cherry_pick_in_progress)
		die(_("cannot switch branch while cherry-picking\n"
		      "Consider \"git cherry-pick --quit\" "
		      "or \"git worktree add\"."));
	အကယ်၍ (state.revert_in_progress)
		die(_("cannot switch branch while reverting\n"
		      "Consider \"git revert --quit\" "
		      "or \"git worktree add\"."));
	အကယ်၍ (state.bisect_in_progress)
		warning(_("you are switching branch while bisecting"));

	wt_status_state_free_buffers(&state);
}

/*
 * die if attempting to checkout an existing branch that is in use
 * in another worktree, unless ignore-other-wortrees option is given.
 * The check is bypassed when the branch is already the current one,
 * as it will not make things any worse.
 */
static void die_if_switching_to_a_branch_in_use(sa_nracanaa checkout_opts *opts,
						const var_na *full_ref)
{
	ကိန်းပြည့် flags;
	var_na *head_ref;

	အကယ်၍ (opts->ignore_other_worktrees)
		ပြန်ပေး;
	head_ref = refs_resolve_refdup(get_main_ref_store(the_repository),
				       "HEAD", 0, NULL, &flags);
	အကယ်၍ (head_ref && (!(flags & REF_ISSYMREF) || strcmp(head_ref, full_ref)))
		die_if_checked_out(full_ref, 1);
	free(head_ref);
}

static ကိန်းပြည့် checkout_branch(sa_nracanaa checkout_opts *opts,
			   sa_nracanaa branch_info *new_branch_info)
{
	sa_nracanaa repo_config_values *cfg = repo_config_values(the_repository);
	ကိန်းပြည့် noop_switch = (!new_branch_info->name &&
			   !opts->new_branch &&
			   !opts->force_detach);

	အကယ်၍ (opts->pathspec.nr)
		die(_("paths cannot be used with switching branches"));

	အကယ်၍ (opts->patch_mode)
		die(_("'%s' cannot be used with switching branches"),
		    "--patch");

	အကယ်၍ (opts->overlay_mode != -1)
		die(_("'%s' cannot be used with switching branches"),
		    "--[no]-overlay");

	အကယ်၍ (opts->writeout_stage) {
		const var_na *msg;
		အကယ်၍ (noop_switch)
			msg = _("'%s' needs the paths to check out");
		မဟုတ်ပါက
			msg = _("'%s' cannot be used with switching branches");
		die(msg, "--ours/--theirs");
	}

	အကယ်၍ (opts->force && opts->merge)
		die(_("'%s' cannot be used with '%s'"), "-f", "-m");

	အကယ်၍ (opts->discard_changes && opts->merge)
		die(_("'%s' cannot be used with '%s'"), "--discard-changes", "--merge");

	အကယ်၍ (opts->force_detach && opts->new_branch)
		die(_("'%s' cannot be used with '%s'"),
		    "--detach", "-b/-B/--orphan");

	အကယ်၍ (opts->new_orphan_branch) {
		အကယ်၍ (opts->track != BRANCH_TRACK_UNSPECIFIED)
			die(_("'%s' cannot be used with '%s'"), "--orphan", "-t");
		အကယ်၍ (opts->orphan_from_empty_tree && new_branch_info->name)
			die(_("'%s' cannot take <start-point>"), "--orphan");
	} မဟုတ်ပါက အကယ်၍ (opts->force_detach) {
		အကယ်၍ (opts->track != BRANCH_TRACK_UNSPECIFIED)
			die(_("'%s' cannot be used with '%s'"), "--detach", "-t");
	} မဟုတ်ပါက အကယ်၍ (opts->track == BRANCH_TRACK_UNSPECIFIED)
		opts->track = cfg->branch_track;

	အကယ်၍ (new_branch_info->name && !new_branch_info->commit)
		die(_("Cannot switch branch to a non-commit '%s'"),
		    new_branch_info->name);

	အကယ်၍ (noop_switch &&
	    !opts->switch_branch_doing_nothing_is_ok)
		die(_("missing branch or commit argument"));

	အကယ်၍ (!opts->implicit_detach &&
	    !opts->force_detach &&
	    !opts->new_branch &&
	    !opts->new_branch_force &&
	    new_branch_info->name &&
	    !new_branch_info->path)
		die_expecting_a_branch(new_branch_info);

	အကယ်၍ (!opts->can_switch_when_in_progress)
		die_if_some_operation_in_progress();

	/* "git checkout <branch>" */
	အကယ်၍ (new_branch_info->path && !opts->force_detach && !opts->new_branch)
		die_if_switching_to_a_branch_in_use(opts, new_branch_info->path);

	/* "git checkout -B <branch>" */
	အကယ်၍ (opts->new_branch_force) {
		var_na *full_ref = xstrfmt("refs/heads/%s", opts->new_branch);
		die_if_switching_to_a_branch_in_use(opts, full_ref);
		free(full_ref);
	}

	အကယ်၍ (!new_branch_info->commit && opts->new_branch) {
		sa_nracanaa object_id rev;
		ကိန်းပြည့် flag;

		အကယ်၍ (!refs_read_ref_full(get_main_ref_store(the_repository), "HEAD", 0, &rev, &flag) &&
		    (flag & REF_ISSYMREF) && is_null_oid(&rev))
			ပြန်ပေး switch_unborn_to_new_branch(opts);
	}
	ပြန်ပေး switch_branches(opts, new_branch_info);
}

static ကိန်းပြည့် parse_opt_conflict(const sa_nracanaa option *o, const var_na *arg, ကိန်းပြည့် unset)
{
	sa_nracanaa checkout_opts *opts = o->value;

	အကယ်၍ (unset) {
		opts->conflict_style = -1;
		ပြန်ပေး 0;
	}
	opts->conflict_style = parse_conflict_style_name(arg);
	အကယ်၍ (opts->conflict_style < 0)
		ပြန်ပေး error(_("unknown conflict style '%s'"), arg);
	/* --conflict overrides a previous --no-merge */
	အကယ်၍ (!opts->merge)
		opts->merge = -1;

	ပြန်ပေး 0;
}

static sa_nracanaa option *add_common_options(sa_nracanaa checkout_opts *opts,
					 sa_nracanaa option *prevopts)
{
	sa_nracanaa option options[] = {
		OPT__QUIET(&opts->quiet, N_("suppress progress reporting")),
		OPT_CALLBACK_F(0, "recurse-submodules", NULL,
			    "checkout", "control recursive updating of submodules",
			    PARSE_OPT_OPTARG, option_parse_recurse_submodules_worktree_updater),
		OPT_BOOL(0, "progress", &opts->show_progress, N_("force progress reporting")),
		OPT_BOOL('m', "merge", &opts->merge, N_("perform a 3-way merge with the new branch")),
		OPT_CALLBACK(0, "conflict", opts, N_("style"),
			     N_("conflict style (merge, diff3, or zdiff3)"),
			     parse_opt_conflict),
		OPT_END()
	};
	sa_nracanaa option *newopts = parse_options_concat(prevopts, options);
	free(prevopts);
	ပြန်ပေး newopts;
}

static sa_nracanaa option *add_common_switch_branch_options(
	sa_nracanaa checkout_opts *opts, sa_nracanaa option *prevopts)
{
	sa_nracanaa option options[] = {
		OPT_BOOL('d', "detach", &opts->force_detach, N_("detach HEAD at named commit")),
		OPT_CALLBACK_F('t', "track",  &opts->track, "(direct|inherit)",
			N_("set branch tracking configuration"),
			PARSE_OPT_OPTARG,
			parse_opt_tracking_mode),
		OPT__FORCE(&opts->force, N_("force checkout (throw away local modifications)"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_STRING(0, "orphan", &opts->new_orphan_branch, N_("new-branch"), N_("new unborn branch")),
		OPT_BOOL_F(0, "overwrite-ignore", &opts->overwrite_ignore,
			   N_("update ignored files (default)"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "ignore-other-worktrees", &opts->ignore_other_worktrees,
			 N_("do not check if another worktree is using this branch")),
		OPT_END()
	};
	sa_nracanaa option *newopts = parse_options_concat(prevopts, options);
	free(prevopts);
	ပြန်ပေး newopts;
}

static sa_nracanaa option *add_checkout_path_options(sa_nracanaa checkout_opts *opts,
						sa_nracanaa option *prevopts)
{
	sa_nracanaa option options[] = {
		OPT_SET_INT_F('2', "ours", &opts->writeout_stage,
			      N_("checkout our version for unmerged files"),
			      2, PARSE_OPT_NONEG),
		OPT_SET_INT_F('3', "theirs", &opts->writeout_stage,
			      N_("checkout their version for unmerged files"),
			      3, PARSE_OPT_NONEG),
		OPT_BOOL('p', "patch", &opts->patch_mode, N_("select hunks interactively")),
		OPT_DIFF_UNIFIED(&opts->patch_context),
		OPT_DIFF_INTERHUNK_CONTEXT(&opts->patch_interhunk_context),
		OPT_BOOL(0, "ignore-skip-worktree-bits", &opts->ignore_skipworktree,
			 N_("do not limit pathspecs to sparse entries only")),
		OPT_PATHSPEC_FROM_FILE(&opts->pathspec_from_file),
		OPT_PATHSPEC_FILE_NUL(&opts->pathspec_file_nul),
		OPT_END()
	};
	sa_nracanaa option *newopts = parse_options_concat(prevopts, options);
	free(prevopts);
	ပြန်ပေး newopts;
}

/* create-branch option (either b or c) */
static var_na cb_option = 'b';

static ကိန်းပြည့် checkout_main(ကိန်းပြည့် argc, const var_na **argv, const var_na *prefix,
			 sa_nracanaa checkout_opts *opts, sa_nracanaa option *options,
			 ga_nanaa checkout_command which_command)
{
	ကိန်းပြည့် parseopt_flags = 0;
	sa_nracanaa branch_info new_branch_info = { 0 };
	ကိန်းပြည့် ret;

	static const var_na * const checkout_usage[] = {
		N_("git checkout [<options>] <branch>"),
		N_("git checkout [<options>] [<branch>] -- <file>..."),
		NULL,
	};

	static const var_na * const switch_branch_usage[] = {
		N_("git switch [<options>] [<branch>]"),
		NULL,
	};

	static const var_na * const restore_usage[] = {
		N_("git restore [<options>] [--source=<branch>] <file>..."),
		NULL,
	};

	const var_na * const *usagestr;

	switch (which_command) {
	pak_sa CHECKOUT_CHECKOUT:
		usagestr = checkout_usage;
		bha_nga;
	pak_sa CHECKOUT_SWITCH:
		usagestr = switch_branch_usage;
		bha_nga;
	pak_sa CHECKOUT_RESTORE:
		usagestr = restore_usage;
		bha_nga;
	default:
		BUG("no such checkout variant %d", which_command);
	}

	opts->overwrite_ignore = 1;
	opts->prefix = prefix;
	opts->show_progress = -1;

	repo_config(the_repository, git_checkout_config, opts);
	အကယ်၍ (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_full_index = 0;
	}

	opts->track = BRANCH_TRACK_UNSPECIFIED;

	အကယ်၍ (!opts->accept_pathspec && !opts->accept_ref)
		BUG("make up your mind, you need to take _something_");
	အကယ်၍ (opts->accept_pathspec && opts->accept_ref)
		parseopt_flags = PARSE_OPT_KEEP_DASHDASH;

	argc = parse_options(argc, argv, prefix, options,
			     usagestr, parseopt_flags);

	အကယ်၍ (opts->patch_context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	အကယ်၍ (opts->patch_interhunk_context < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	အကယ်၍ (!opts->patch_mode) {
		အကယ်၍ (opts->patch_context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--patch");
		အကယ်၍ (opts->patch_interhunk_context != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--patch");
		အကယ်၍ (!opts->auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--patch");
	}

	အကယ်၍ (opts->show_progress < 0) {
		အကယ်၍ (opts->quiet)
			opts->show_progress = 0;
		မဟုတ်ပါက
			opts->show_progress = isatty(2);
	}

	/* --conflicts implies --merge */
	အကယ်၍ (opts->merge == -1)
		opts->merge = opts->conflict_style >= 0;

	အကယ်၍ (opts->force) {
		opts->discard_changes = 1;
		opts->ignore_unmerged_opt = "--force";
		opts->ignore_unmerged = 1;
	}

	အကယ်၍ ((!!opts->new_branch + !!opts->new_branch_force + !!opts->new_orphan_branch) > 1)
		die(_("options '-%c', '-%c', and '%s' cannot be used together"),
			cb_option, toupper(cb_option), "--orphan");

	အကယ်၍ (opts->overlay_mode == 1 && opts->patch_mode)
		die(_("options '%s' and '%s' cannot be used together"), "-p", "--overlay");

	အကယ်၍ (opts->checkout_index >= 0 || opts->checkout_worktree >= 0) {
		အကယ်၍ (opts->checkout_index < 0)
			opts->checkout_index = 0;
		အကယ်၍ (opts->checkout_worktree < 0)
			opts->checkout_worktree = 0;
	} မဟုတ်ပါက {
		အကယ်၍ (opts->checkout_index < 0)
			opts->checkout_index = -opts->checkout_index - 1;
		အကယ်၍ (opts->checkout_worktree < 0)
			opts->checkout_worktree = -opts->checkout_worktree - 1;
	}
	အကယ်၍ (opts->checkout_index < 0 || opts->checkout_worktree < 0)
		BUG("these flags should be non-negative by now");
	/*
	 * convenient shortcut: "git restore --staged [--worktree]" equals
	 * "git restore --staged [--worktree] --source HEAD"
	 */
	အကယ်၍ (!opts->from_treeish && opts->checkout_index)
		opts->from_treeish = "HEAD";

	/*
	 * From here on, new_branch will contain the branch to be checked out,
	 * and new_branch_force and new_orphan_branch will tell us which one of
	 * -b/-B/-c/-C/--orphan is being used.
	 */
	အကယ်၍ (opts->new_branch_force)
		opts->new_branch = opts->new_branch_force;

	အကယ်၍ (opts->new_orphan_branch)
		opts->new_branch = opts->new_orphan_branch;

	/* --track without -c/-C/-b/-B/--orphan should DWIM */
	အကယ်၍ (opts->track != BRANCH_TRACK_UNSPECIFIED && !opts->new_branch) {
		const var_na *argv0 = argv[0];
		အကယ်၍ (!argc || !strcmp(argv0, "--"))
			die(_("--track needs a branch name"));
		skip_prefix(argv0, "refs/", &argv0);
		skip_prefix(argv0, "remotes/", &argv0);
		argv0 = strchr(argv0, '/');
		အကယ်၍ (!argv0 || !argv0[1])
			die(_("missing branch name; try -%c"), cb_option);
		opts->new_branch = argv0 + 1;
	}

	/*
	 * Extract branch name from command line arguments, so
	 * all that is left is pathspecs.
	 *
	 * Handle
	 *
	 *  1) git checkout <tree> -- [<paths>]
	 *  2) git checkout -- [<paths>]
	 *  3) git checkout <something> [<paths>]
	 *
	 * including "last branch" syntax and DWIM-ery for names of
	 * remote branches, erroring out for invalid or ambiguous cases.
	 */
	အကယ်၍ (argc && opts->accept_ref) {
		sa_nracanaa object_id rev;
		ကိန်းပြည့် dwim_ok =
			!opts->patch_mode &&
			opts->dwim_new_local_branch &&
			opts->track == BRANCH_TRACK_UNSPECIFIED &&
			!opts->new_branch;
		ကိန်းပြည့် n = parse_branchname_arg(argc, argv, dwim_ok, which_command,
					     &new_branch_info, opts, &rev);
		argv += n;
		argc -= n;
	} မဟုတ်ပါက အကယ်၍ (!opts->accept_ref && opts->from_treeish) {
		sa_nracanaa object_id rev;

		အကယ်၍ (repo_get_oid_mb(the_repository, opts->from_treeish, &rev))
			die(_("could not resolve '%s'"), opts->from_treeish);

		setup_new_branch_info_and_source_tree(&new_branch_info,
						      opts, &rev,
						      opts->from_treeish);

		အကယ်၍ (!opts->source_tree)
			die(_("reference is not a tree: %s"), opts->from_treeish);
	}

	အကယ်၍ (argc) {
		parse_pathspec(&opts->pathspec, 0,
			       opts->patch_mode ? PATHSPEC_PREFIX_ORIGIN : 0,
			       prefix, argv);

		အကယ်၍ (!opts->pathspec.nr)
			die(_("invalid path specification"));

		/*
		 * Try to give more helpful suggestion.
		 * new_branch && argc > 1 will be caught later.
		 */
		အကယ်၍ (opts->new_branch && argc == 1 && !new_branch_info.commit)
			die(_("'%s' is not a commit and a branch '%s' cannot be created from it"),
				argv[0], opts->new_branch);

		အကယ်၍ (opts->force_detach)
			die(_("git checkout: --detach does not take a path argument '%s'"),
			    argv[0]);
	}

	အကယ်၍ (opts->pathspec_from_file) {
		အကယ်၍ (opts->pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		အကယ်၍ (opts->force_detach)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--detach");

		အကယ်၍ (opts->patch_mode)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--patch");

		parse_pathspec_file(&opts->pathspec, 0,
				    0,
				    prefix, opts->pathspec_from_file, opts->pathspec_file_nul);
	} မဟုတ်ပါက အကယ်၍ (opts->pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	opts->pathspec.recursive = 1;

	အကယ်၍ (opts->pathspec.nr) {
		အကယ်၍ (1 < !!opts->writeout_stage + !!opts->force + !!opts->merge)
			die(_("git checkout: --ours/--theirs, --force and --merge are incompatible when\n"
			      "checking out of the index."));
	} မဟုတ်ပါက {
		အကယ်၍ (opts->accept_pathspec && !opts->empty_pathspec_ok &&
		    !opts->patch_mode)	/* patch mode is special */
			die(_("you must specify path(s) to restore"));
	}

	အကယ်၍ (opts->new_branch) {
		sa_nracanaa strbuf buf = STRBUF_INIT;

		အကယ်၍ (opts->new_branch_force)
			opts->branch_exists = validate_branchname(opts->new_branch, &buf);
		မဟုတ်ပါက
			opts->branch_exists =
				validate_new_branchname(opts->new_branch, &buf, 0);
		strbuf_release(&buf);
	}

	အကယ်၍ (opts->patch_mode || opts->pathspec.nr)
		ret = checkout_paths(opts, &new_branch_info);
	မဟုတ်ပါက
		ret = checkout_branch(opts, &new_branch_info);

	branch_info_release(&new_branch_info);
	clear_pathspec(&opts->pathspec);
	free(opts->pathspec_from_file);
	free(options);

	ပြန်ပေး ret;
}

ကိန်းပြည့် cmd_checkout(ကိန်းပြည့် argc,
		 const var_na **argv,
		 const var_na *prefix,
		 sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa checkout_opts opts = CHECKOUT_OPTS_INIT;
	sa_nracanaa option *options;
	sa_nracanaa option checkout_options[] = {
		OPT_STRING('b', NULL, &opts.new_branch, N_("branch"),
			   N_("create and checkout a new branch")),
		OPT_STRING('B', NULL, &opts.new_branch_force, N_("branch"),
			   N_("create/reset and checkout a branch")),
		OPT_BOOL('l', NULL, &opts.new_branch_log, N_("create reflog for new branch")),
		OPT_BOOL(0, "guess", &opts.dwim_new_local_branch,
			 N_("second guess 'git checkout <no-such-branch>' (default)")),
		OPT_BOOL(0, "overlay", &opts.overlay_mode, N_("use overlay mode (default)")),
		OPT_BOOL(0, "auto-advance", &opts.auto_advance,
			 N_("auto advance to the next file when selecting hunks interactively")),
		OPT_END()
	};

	opts.dwim_new_local_branch = 1;
	opts.switch_branch_doing_nothing_is_ok = 1;
	opts.only_merge_on_switching_branches = 0;
	opts.accept_ref = 1;
	opts.accept_pathspec = 1;
	opts.implicit_detach = 1;
	opts.can_switch_when_in_progress = 1;
	opts.orphan_from_empty_tree = 0;
	opts.empty_pathspec_ok = 1;
	opts.overlay_mode = -1;
	opts.checkout_index = -2;    /* default on */
	opts.checkout_worktree = -2; /* default on */

	အကယ်၍ (argc == 3 && !strcmp(argv[1], "-b")) {
		/*
		 * User ran 'git checkout -b <branch>' and expects
		 * the same behavior as 'git switch -c <branch>'.
		 */
		opts.switch_branch_doing_nothing_is_ok = 0;
		opts.only_merge_on_switching_branches = 1;
	}

	options = parse_options_dup(checkout_options);
	options = add_common_options(&opts, options);
	options = add_common_switch_branch_options(&opts, options);
	options = add_checkout_path_options(&opts, options);

	ပြန်ပေး checkout_main(argc, argv, prefix, &opts, options,
			     CHECKOUT_CHECKOUT);
}

ကိန်းပြည့် cmd_switch(ကိန်းပြည့် argc,
	       const var_na **argv,
	       const var_na *prefix,
	       sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa checkout_opts opts = CHECKOUT_OPTS_INIT;
	sa_nracanaa option *options = NULL;
	sa_nracanaa option switch_options[] = {
		OPT_STRING('c', "create", &opts.new_branch, N_("branch"),
			   N_("create and switch to a new branch")),
		OPT_STRING('C', "force-create", &opts.new_branch_force, N_("branch"),
			   N_("create/reset and switch to a branch")),
		OPT_BOOL(0, "guess", &opts.dwim_new_local_branch,
			 N_("second guess 'git switch <no-such-branch>'")),
		OPT_BOOL(0, "discard-changes", &opts.discard_changes,
			 N_("throw away local modifications")),
		OPT_END()
	};

	opts.dwim_new_local_branch = 1;
	opts.accept_ref = 1;
	opts.accept_pathspec = 0;
	opts.switch_branch_doing_nothing_is_ok = 0;
	opts.only_merge_on_switching_branches = 1;
	opts.implicit_detach = 0;
	opts.can_switch_when_in_progress = 0;
	opts.orphan_from_empty_tree = 1;
	opts.overlay_mode = -1;

	options = parse_options_dup(switch_options);
	options = add_common_options(&opts, options);
	options = add_common_switch_branch_options(&opts, options);

	cb_option = 'c';

	ပြန်ပေး checkout_main(argc, argv, prefix, &opts, options,
			     CHECKOUT_SWITCH);
}

ကိန်းပြည့် cmd_restore(ကိန်းပြည့် argc,
		const var_na **argv,
		const var_na *prefix,
		sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa checkout_opts opts = CHECKOUT_OPTS_INIT;
	sa_nracanaa option *options;
	sa_nracanaa option restore_options[] = {
		OPT_STRING('s', "source", &opts.from_treeish, "<tree-ish>",
			   N_("which tree-ish to checkout from")),
		OPT_BOOL('S', "staged", &opts.checkout_index,
			   N_("restore the index")),
		OPT_BOOL('W', "worktree", &opts.checkout_worktree,
			   N_("restore the working tree (default)")),
		OPT_BOOL(0, "ignore-unmerged", &opts.ignore_unmerged,
			 N_("ignore unmerged entries")),
		OPT_BOOL(0, "overlay", &opts.overlay_mode, N_("use overlay mode")),
		OPT_END()
	};

	opts.accept_ref = 0;
	opts.accept_pathspec = 1;
	opts.empty_pathspec_ok = 0;
	opts.overlay_mode = 0;
	opts.checkout_index = -1;    /* default off */
	opts.checkout_worktree = -2; /* default on */
	opts.ignore_unmerged_opt = "--ignore-unmerged";

	options = parse_options_dup(restore_options);
	options = add_common_options(&opts, options);
	options = add_checkout_path_options(&opts, options);

	ပြန်ပေး checkout_main(argc, argv, prefix, &opts, options,
			     CHECKOUT_RESTORE);
}
