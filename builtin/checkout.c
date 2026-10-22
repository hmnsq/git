#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ضمن "builtin.h"
#ضمن "advice.h"
#ضمن "branch.h"
#ضمن "cache-tree.h"
#ضمن "checkout.h"
#ضمن "commit.h"
#ضمن "config.h"
#ضمن "diff.h"
#ضمن "dir.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "hex.h"
#ضمن "hook.h"
#ضمن "merge-ll.h"
#ضمن "lockfile.h"
#ضمن "mem-pool.h"
#ضمن "object-file.h"
#ضمن "object-name.h"
#ضمن "odb.h"
#ضمن "parse-options.h"
#ضمن "path.h"
#ضمن "preload-index.h"
#ضمن "read-cache.h"
#ضمن "refs.h"
#ضمن "remote.h"
#ضمن "repo-settings.h"
#ضمن "resolve-undo.h"
#ضمن "revision.h"
#ضمن "sequencer.h"
#ضمن "setup.h"
#ضمن "sparse-index.h"
#ضمن "strvec.h"
#ضمن "submodule.h"
#ضمن "symlinks.h"
#ضمن "trace2.h"
#ضمن "tree.h"
#ضمن "tree-walk.h"
#ضمن "unpack-trees.h"
#ضمن "wt-status.h"
#ضمن "xdiff-interface.h"
#ضمن "entry.h"
#ضمن "parallel-checkout.h"
#ضمن "add-interactive.h"

بنية checkout_opts {
	صحيح patch_mode;
	صحيح patch_context;
	صحيح patch_interhunk_context;
	صحيح auto_advance;
	صحيح quiet;
	صحيح merge;
	صحيح force;
	صحيح force_detach;
	صحيح implicit_detach;
	صحيح writeout_stage;
	صحيح overwrite_ignore;
	صحيح ignore_skipworktree;
	صحيح ignore_other_worktrees;
	صحيح show_progress;
	صحيح count_checkout_paths;
	صحيح overlay_mode;
	صحيح dwim_new_local_branch;
	صحيح discard_changes;
	صحيح accept_ref;
	صحيح accept_pathspec;
	صحيح switch_branch_doing_nothing_is_ok;
	صحيح only_merge_on_switching_branches;
	صحيح can_switch_when_in_progress;
	صحيح orphan_from_empty_tree;
	صحيح empty_pathspec_ok;
	صحيح checkout_index;
	صحيح checkout_worktree;
	ثابت حرف *ignore_unmerged_opt;
	صحيح ignore_unmerged;
	صحيح pathspec_file_nul;
	حرف *pathspec_from_file;

	ثابت حرف *new_branch;
	ثابت حرف *new_branch_force;
	ثابت حرف *new_orphan_branch;
	صحيح new_branch_log;
	تعداد branch_track track;
	بنية diff_options diff_options;
	صحيح conflict_style;

	صحيح branch_exists;
	ثابت حرف *prefix;
	بنية pathspec pathspec;
	ثابت حرف *from_treeish;
	بنية tree *source_tree;
};

#define CHECKOUT_OPTS_INIT { \
	.conflict_style = -1, \
	.merge = -1, \
	.patch_context = -1, \
	.patch_interhunk_context = -1, \
	.auto_advance = 1, \
}

#define MERGE_WORKING_TREE_UNPACK_FAILED (-2)

بنية branch_info {
	حرف *name; /* The short name used */
	حرف *path; /* The full name of a real branch */
	بنية commit *commit; /* The named commit */
	حرف *refname; /* The full name of the ref being checked out. */
	بنية object_id oid; /* The object ID of the commit being checked out. */
	/*
	 * if not null the branch is detached because it's already
	 * checked out in this checkout
	 */
	حرف *checkout;
};

ساكن فراغ branch_info_release(بنية branch_info *info)
{
	حرر(info->name);
	حرر(info->path);
	حرر(info->refname);
	حرر(info->checkout);
}

ساكن صحيح post_checkout_hook(بنية commit *old_commit, بنية commit *new_commit,
			      صحيح changed)
{
	بنية run_hooks_opt opt = RUN_HOOKS_OPT_INIT_FORCE_SERIAL;

	/*
	 * "new_commit" can be NULL when checking out from the index before
	 * a commit exists.
	 */
	strvec_pushl(&opt.args,
		     oid_to_hex(old_commit ? &old_commit->object.oid : null_oid(the_hash_algo)),
		     oid_to_hex(new_commit ? &new_commit->object.oid : null_oid(the_hash_algo)),
		     changed ? "1" : "0",
		     عدم);

	החזר run_hooks_opt(the_repository, "post-checkout", &opt);
}

/*
 * Handle a tree object and determine if we need to recurse into the
 * tree (READ_TREE_RECURSIVE) or skip it (0).
 */
ساكن صحيح try_update_sparse_directory(ثابت بنية object_id *oid,
				       بنية strbuf *base,
				       ثابت حرف *pathname,
				       صحيح overlay_mode)
{
	بنية strbuf dirpath = STRBUF_INIT;
	بنية cache_entry *old;
	صحيح pos, result = READ_TREE_RECURSIVE;

	אם (!the_repository->index->sparse_index)
		החזר result;

	strbuf_addbuf(&dirpath, base);
	strbuf_addstr(&dirpath, pathname);
	strbuf_addch(&dirpath, '/');

	pos = index_name_pos_sparse(the_repository->index,
				    dirpath.buf, dirpath.len);
	אם (pos < 0)
		اذهب cleanup;

	old = the_repository->index->cache[pos];
	אם (!S_ISSPARSEDIR(old->ce_mode))
		اذهب cleanup;

	אם (oideq(oid, &old->oid)) {
		/* Tree content already matches; no need to descend. */
		result = 0;
	} אחרת אם (!overlay_mode) {
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
	החזר result;
}

ساكن صحيح update_some(ثابت بنية object_id *oid, بنية strbuf *base,
		       ثابت حرف *pathname, unsigned mode, فراغ *context)
{
	صحيح len;
	بنية cache_entry *ce;
	صحيح pos;
	صحيح overlay_mode = context ? *((صحيح *)context) : 1;

	אם (S_ISDIR(mode))
		החזר try_update_sparse_directory(oid, base, pathname,
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
	אם (pos >= 0) {
		بنية cache_entry *old = the_repository->index->cache[pos];
		אם (ce->ce_mode == old->ce_mode &&
		    !ce_intent_to_add(old) &&
		    oideq(&ce->oid, &old->oid)) {
			old->ce_flags |= CE_UPDATE;
			discard_cache_entry(ce);
			החזר 0;
		}
	}

	add_index_entry(the_repository->index, ce,
			ADD_CACHE_OK_TO_ADD | ADD_CACHE_OK_TO_REPLACE);
	החזר 0;
}

ساكن صحيح read_tree_some(بنية tree *tree, ثابت بنية pathspec *pathspec,
			  صحيح overlay_mode)
{
	read_tree(the_repository, tree,
		  pathspec, update_some, &overlay_mode);

	/* update the index with the given tree's info
	 * for all args, expanding wildcards, and exit
	 * with any non-zero return code.
	 */
	החזר 0;
}

ساكن صحيح skip_same_name(ثابت بنية cache_entry *ce, صحيح pos)
{
	כלעוד (++pos < the_repository->index->cache_nr &&
	       !strcmp(the_repository->index->cache[pos]->name, ce->name))
		; /* skip */
	החזר pos;
}

ساكن صحيح check_stage(صحيح stage, ثابت بنية cache_entry *ce, صحيح pos,
		       صحيح overlay_mode)
{
	כלעוד (pos < the_repository->index->cache_nr &&
	       !strcmp(the_repository->index->cache[pos]->name, ce->name)) {
		אם (ce_stage(the_repository->index->cache[pos]) == stage)
			החזר 0;
		pos++;
	}
	אם (!overlay_mode)
		החזר 0;
	אם (stage == 2)
		החזר error(_("path '%s' does not have our version"), ce->name);
	אחרת
		החזר error(_("path '%s' does not have their version"), ce->name);
}

ساكن صحيح check_stages(unsigned stages, ثابت بنية cache_entry *ce, صحيح pos)
{
	unsigned seen = 0;
	ثابت حرف *name = ce->name;

	כלעוד (pos < the_repository->index->cache_nr) {
		ce = the_repository->index->cache[pos];
		אם (strcmp(name, ce->name))
			اكسر;
		seen |= (1 << ce_stage(ce));
		pos++;
	}
	אם ((stages & seen) != stages)
		החזר error(_("path '%s' does not have all necessary versions"),
			     name);
	החזר 0;
}

ساكن صحيح checkout_stage(صحيح stage, ثابت بنية cache_entry *ce, صحيح pos,
			  ثابت بنية checkout *state, صحيح *nr_checkouts,
			  صحيح overlay_mode)
{
	כלעוד (pos < the_repository->index->cache_nr &&
	       !strcmp(the_repository->index->cache[pos]->name, ce->name)) {
		אם (ce_stage(the_repository->index->cache[pos]) == stage)
			החזר checkout_entry(the_repository->index->cache[pos], state,
					      عدم, nr_checkouts);
		pos++;
	}
	אם (!overlay_mode) {
		unlink_entry(ce, عدم);
		החזר 0;
	}
	אם (stage == 2)
		החזר error(_("path '%s' does not have our version"), ce->name);
	אחרת
		החזר error(_("path '%s' does not have their version"), ce->name);
}

ساكن صحيح checkout_merged(صحيح pos, ثابت بنية checkout *state,
			   صحيح *nr_checkouts, بنية mem_pool *ce_mem_pool,
			   صحيح conflict_style)
{
	بنية cache_entry *ce = the_repository->index->cache[pos];
	ثابت حرف *path = ce->name;
	mmfile_t ancestor, ours, theirs;
	تعداد ll_merge_result merge_status;
	صحيح status;
	بنية object_id oid;
	mmfile_t result_buf;
	بنية object_id threeway[3];
	unsigned mode = 0;
	بنية ll_merge_options ll_opts = LL_MERGE_OPTIONS_INIT;
	صحيح renormalize = 0;

	memset(threeway, 0, حجم(threeway));
	כלעוד (pos < the_repository->index->cache_nr) {
		صحيح stage;
		stage = ce_stage(ce);
		אם (!stage || strcmp(path, ce->name))
			اكسر;
		oidcpy(&threeway[stage - 1], &ce->oid);
		אם (stage == 2)
			mode = create_ce_mode(ce->ce_mode);
		pos++;
		ce = the_repository->index->cache[pos];
	}
	אם (is_null_oid(&threeway[1]) || is_null_oid(&threeway[2]))
		החזר error(_("path '%s' does not have necessary versions"), path);

	read_mmblob(&ancestor, the_repository->objects, &threeway[0]);
	read_mmblob(&ours, the_repository->objects, &threeway[1]);
	read_mmblob(&theirs, the_repository->objects, &threeway[2]);

	repo_config_get_bool(the_repository, "merge.renormalize", &renormalize);
	ll_opts.renormalize = renormalize;
	ll_opts.conflict_style = conflict_style;
	merge_status = ll_merge(&result_buf, path, &ancestor, "base",
				&ours, "ours", &theirs, "theirs",
				state->istate, &ll_opts);
	حرر(ancestor.ptr);
	حرر(ours.ptr);
	حرر(theirs.ptr);
	אם (merge_status == LL_MERGE_BINARY_CONFLICT)
		warning("Cannot merge binary files: %s (%s vs. %s)",
			path, "ours", "theirs");
	אם (merge_status < 0 || !result_buf.ptr) {
		حرر(result_buf.ptr);
		החזר error(_("path '%s': cannot merge"), path);
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
	אם (odb_write_object(the_repository->objects, result_buf.ptr, result_buf.size, OBJ_BLOB, &oid))
		die(_("Unable to add merge result for '%s'"), path);
	حرر(result_buf.ptr);
	ce = make_transient_cache_entry(mode, &oid, path, 2, ce_mem_pool);
	אם (!ce)
		die(_("make_cache_entry failed for path '%s'"), path);
	status = checkout_entry(ce, state, عدم, nr_checkouts);
	החזר status;
}

ساكن فراغ mark_ce_for_checkout_overlay(بنية cache_entry *ce,
					 حرف *ps_matched,
					 ثابت بنية checkout_opts *opts)
{
	ce->ce_flags &= ~CE_MATCHED;
	אם (!opts->ignore_skipworktree && ce_skip_worktree(ce))
		החזר;
	אם (opts->source_tree && !(ce->ce_flags & CE_UPDATE))
		/*
		 * "git checkout tree-ish -- path", but this entry
		 * is in the original index but is not in tree-ish
		 * or does not match the pathspec; it will not be
		 * checked out to the working tree.  We will not do
		 * anything to this entry at all.
		 */
		החזר;
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
	אם (ce_path_match(the_repository->index, ce, &opts->pathspec, ps_matched))
		ce->ce_flags |= CE_MATCHED;
}

ساكن فراغ mark_ce_for_checkout_no_overlay(بنية cache_entry *ce,
					    حرف *ps_matched,
					    ثابت بنية checkout_opts *opts)
{
	ce->ce_flags &= ~CE_MATCHED;
	אם (!opts->ignore_skipworktree && ce_skip_worktree(ce))
		החזר;
	אם (ce_path_match(the_repository->index, ce, &opts->pathspec, ps_matched)) {
		ce->ce_flags |= CE_MATCHED;
		אם (opts->source_tree && !(ce->ce_flags & CE_UPDATE))
			/*
			 * In overlay mode, but the path is not in
			 * tree-ish, which means we should remove it
			 * from the index and the working tree.
			 */
			ce->ce_flags |= CE_REMOVE | CE_WT_REMOVE;
	}
}

ساكن صحيح checkout_worktree(ثابت بنية checkout_opts *opts,
			     ثابت بنية branch_info *info)
{
	بنية checkout state = CHECKOUT_INIT;
	صحيح nr_checkouts = 0, nr_unmerged = 0;
	صحيح errs = 0;
	صحيح pos;
	صحيح pc_workers, pc_threshold;
	بنية mem_pool ce_mem_pool;

	state.force = 1;
	state.refresh_cache = 1;
	state.istate = the_repository->index;

	mem_pool_init(&ce_mem_pool, 0);
	get_parallel_checkout_configs(&pc_workers, &pc_threshold);
	init_checkout_metadata(&state.meta, info->refname,
			       info->commit ? &info->commit->object.oid : &info->oid,
			       عدم);

	enable_delayed_checkout(&state);

	אם (pc_workers > 1)
		init_parallel_checkout();

	עבור (pos = 0; pos < the_repository->index->cache_nr; pos++) {
		بنية cache_entry *ce = the_repository->index->cache[pos];
		אם (ce->ce_flags & CE_MATCHED) {
			אם (!ce_stage(ce)) {
				errs |= checkout_entry(ce, &state,
						       عدم, &nr_checkouts);
				استمر;
			}
			אם (opts->writeout_stage)
				errs |= checkout_stage(opts->writeout_stage,
						       ce, pos,
						       &state,
						       &nr_checkouts, opts->overlay_mode);
			אחרת אם (opts->merge)
				errs |= checkout_merged(pos, &state,
							&nr_unmerged,
							&ce_mem_pool,
							opts->conflict_style);
			pos = skip_same_name(ce, pos) - 1;
		}
	}
	אם (pc_workers > 1)
		errs |= run_parallel_checkout(&state, pc_workers, pc_threshold,
					      عدم, عدم);
	mem_pool_discard(&ce_mem_pool, should_validate_cache_entries());
	remove_marked_cache_entries(the_repository->index, 1);
	remove_scheduled_dirs();
	errs |= finish_delayed_checkout(&state, opts->show_progress);

	אם (opts->count_checkout_paths) {
		אם (nr_unmerged)
			fprintf_ln(stderr, Q_("Recreated %d merge conflict",
					      "Recreated %d merge conflicts",
					      nr_unmerged),
				   nr_unmerged);
		אם (opts->source_tree)
			fprintf_ln(stderr, Q_("Updated %d path from %s",
					      "Updated %d paths from %s",
					      nr_checkouts),
				   nr_checkouts,
				   repo_find_unique_abbrev(the_repository, &opts->source_tree->object.oid,
							   DEFAULT_ABBREV));
		אחרת אם (!nr_unmerged || nr_checkouts)
			fprintf_ln(stderr, Q_("Updated %d path from the index",
					      "Updated %d paths from the index",
					      nr_checkouts),
				   nr_checkouts);
	}

	החזר errs;
}

ساكن صحيح checkout_paths(ثابت بنية checkout_opts *opts,
			  ثابت بنية branch_info *new_branch_info)
{
	صحيح pos;
	ساكن حرف *ps_matched;
	بنية object_id rev;
	بنية commit *head;
	صحيح errs = 0;
	بنية lock_file lock_file = LOCK_INIT;
	صحيح checkout_index;

	trace2_cmd_mode(opts->patch_mode ? "patch" : "path");

	אם (opts->track != BRANCH_TRACK_UNSPECIFIED)
		die(_("'%s' cannot be used with updating paths"), "--track");

	אם (opts->new_branch_log)
		die(_("'%s' cannot be used with updating paths"), "-l");

	אם (opts->ignore_unmerged && opts->patch_mode)
		die(_("'%s' cannot be used with updating paths"),
		    opts->ignore_unmerged_opt);

	אם (opts->force_detach)
		die(_("'%s' cannot be used with updating paths"), "--detach");

	אם (opts->merge && opts->patch_mode)
		die(_("options '%s' and '%s' cannot be used together"), "--merge", "--patch");

	אם (opts->ignore_unmerged && opts->merge)
		die(_("options '%s' and '%s' cannot be used together"),
		    opts->ignore_unmerged_opt, "-m");

	אם (opts->new_branch)
		die(_("Cannot update paths and switch to branch '%s' at the same time."),
		    opts->new_branch);

	אם (!opts->checkout_worktree && !opts->checkout_index)
		die(_("neither '%s' or '%s' is specified"),
		    "--staged", "--worktree");

	אם (!opts->checkout_worktree && !opts->from_treeish)
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
	אם (!opts->accept_ref && opts->checkout_index) {
		אם (opts->writeout_stage)
			die(_("'%s' or '%s' cannot be used with %s"),
			    "--ours", "--theirs", "--staged");

		אם (opts->merge)
			die(_("'%s' or '%s' cannot be used with %s"),
			    "--merge", "--conflict", "--staged");
	}

	/*
	 * recreating unmerged index entries and writing out data from
	 * unmerged index entries would make no sense when checking out
	 * of a tree-ish.
	 */
	אם ((opts->merge || opts->writeout_stage) && opts->source_tree)
		die(_("'%s', '%s', or '%s' cannot be used when checking out of a tree"),
		    "--merge", "--ours", "--theirs");

	אם (opts->patch_mode) {
		تعداد add_p_mode patch_mode;
		بنية interactive_options interactive_opts = {
			.context = opts->patch_context,
			.interhunkcontext = opts->patch_interhunk_context,
			.auto_advance = opts->auto_advance
		};
		ثابت حرف *rev = new_branch_info->name;
		حرف rev_oid[GIT_MAX_HEXSZ + 1];

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
		אם (rev && new_branch_info->commit && strcmp(rev, "HEAD"))
			rev = oid_to_hex_r(rev_oid, &new_branch_info->commit->object.oid);

		אם (opts->checkout_index && opts->checkout_worktree)
			patch_mode = ADD_P_CHECKOUT;
		אחרת אם (opts->checkout_index && !opts->checkout_worktree)
			patch_mode = ADD_P_RESET;
		אחרת אם (!opts->checkout_index && opts->checkout_worktree)
			patch_mode = ADD_P_WORKTREE;
		אחרת
			BUG("either flag must have been set, worktree=%d, index=%d",
			    opts->checkout_worktree, opts->checkout_index);
		החזר !!run_add_p(the_repository, patch_mode, &interactive_opts,
				   rev, &opts->pathspec, 0);
	}

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);
	אם (repo_read_index_preload(the_repository, &opts->pathspec, 0) < 0)
		החזר error(_("index file corrupt"));

	אם (opts->source_tree)
		read_tree_some(opts->source_tree, &opts->pathspec,
			       opts->overlay_mode);
	אם (opts->merge)
		unmerge_index(the_repository->index, &opts->pathspec, CE_MATCHED);

	ps_matched = xcalloc(opts->pathspec.nr, 1);

	/*
	 * Make sure all pathspecs participated in locating the paths
	 * to be checked out.
	 */
	עבור (pos = 0; pos < the_repository->index->cache_nr; pos++)
		אם (opts->overlay_mode)
			mark_ce_for_checkout_overlay(the_repository->index->cache[pos],
						     ps_matched,
						     opts);
		אחרת
			mark_ce_for_checkout_no_overlay(the_repository->index->cache[pos],
							ps_matched,
							opts);

	אם (report_path_error(ps_matched, &opts->pathspec)) {
		حرر(ps_matched);
		החזר 1;
	}
	حرر(ps_matched);

	/* Any unmerged paths? */
	עבור (pos = 0; pos < the_repository->index->cache_nr; pos++) {
		ثابت بنية cache_entry *ce = the_repository->index->cache[pos];
		אם (ce->ce_flags & CE_MATCHED) {
			אם (!ce_stage(ce))
				استمر;
			אם (opts->ignore_unmerged) {
				אם (!opts->quiet)
					warning(_("path '%s' is unmerged"), ce->name);
			} אחרת אם (opts->writeout_stage) {
				errs |= check_stage(opts->writeout_stage, ce, pos, opts->overlay_mode);
			} אחרת אם (opts->merge) {
				errs |= check_stages((1<<2) | (1<<3), ce, pos);
			} אחרת {
				errs = 1;
				error(_("path '%s' is unmerged"), ce->name);
			}
			pos = skip_same_name(ce, pos) - 1;
		}
	}
	אם (errs)
		החזר 1;

	/* Now we are committed to check them out */
	אם (opts->checkout_worktree)
		errs |= checkout_worktree(opts, new_branch_info);
	אחרת
		remove_marked_cache_entries(the_repository->index, 1);

	/*
	 * Allow updating the index when checking out from the index.
	 * This is to save new stat info.
	 */
	אם (opts->checkout_worktree && !opts->checkout_index && !opts->source_tree)
		checkout_index = 1;
	אחרת
		checkout_index = opts->checkout_index;

	אם (checkout_index) {
		אם (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
			die(_("unable to write new index file"));
	} אחרת {
		/*
		 * NEEDSWORK: if --worktree is not specified, we
		 * should save stat info of checked out files in the
		 * index to avoid the next (potentially costly)
		 * refresh. But it's a bit tricker to do...
		 */
		rollback_lock_file(&lock_file);
	}

	refs_read_ref_full(get_main_ref_store(the_repository), "HEAD", 0,
			   &rev, عدم);
	head = lookup_commit_reference_gently(the_repository, &rev, 1);

	errs |= post_checkout_hook(head, head, 0);
	החזר errs;
}

ساكن فراغ show_local_changes(بنية object *head,
			       ثابت بنية diff_options *opts)
{
	بنية rev_info rev;
	/* I think we want full paths, even if we're in a subdirectory. */
	repo_init_revisions(the_repository, &rev, عدم);
	rev.diffopt.flags = opts->flags;
	rev.diffopt.output_format |= DIFF_FORMAT_NAME_STATUS;
	rev.diffopt.flags.recursive = 1;
	diff_setup_done(&rev.diffopt);
	add_pending_object(&rev, head, عدم);
	run_diff_index(&rev, 0);
	release_revisions(&rev);
}

ساكن فراغ describe_detached_head(ثابت حرف *msg, بنية commit *commit)
{
	بنية strbuf sb = STRBUF_INIT;

	אם (!repo_parse_commit(the_repository, commit))
		pp_commit_easy(CMIT_FMT_ONELINE, commit, &sb);
	אם (print_sha1_ellipsis()) {
		fprintf(stderr, "%s %s... %s\n", msg,
			repo_find_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV),
			sb.buf);
	} אחרת {
		fprintf(stderr, "%s %s %s\n", msg,
			repo_find_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV),
			sb.buf);
	}
	strbuf_release(&sb);
}

ساكن صحيح reset_tree(بنية tree *tree, ثابت بنية checkout_opts *o,
		      صحيح worktree, صحيح *writeout_error,
		      بنية branch_info *info)
{
	بنية unpack_trees_options opts;
	بنية tree_desc tree_desc;

	memset(&opts, 0, حجم(opts));
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
			       عدم);
	אם (repo_parse_tree(the_repository, tree) < 0)
		החזר 128;
	init_tree_desc(&tree_desc, &tree->object.oid, tree->buffer, tree->size);
	بدل (unpack_trees(1, &tree_desc, &opts)) {
	حالة -2:
		*writeout_error = 1;
		/*
		 * We return 0 nevertheless, as the index is all right
		 * and more importantly we have made best efforts to
		 * update paths in the work tree, and we cannot revert
		 * them.
		 */
		/* fallthrough */
	حالة 0:
		החזר 0;
	افتراضي:
		החזר 128;
	}
}

ساكن فراغ setup_branch_path(بنية branch_info *branch)
{
	بنية strbuf buf = STRBUF_INIT;

	/*
	 * If this is a ref, resolve it; otherwise, look up the OID for our
	 * expression.  Failure here is okay.
	 */
	אם (!repo_dwim_ref(the_repository, branch->name, strlen(branch->name),
			   &branch->oid, &branch->refname, 0))
		repo_get_oid_committish(the_repository, branch->name, &branch->oid);

	copy_branchname(the_repository, &buf, branch->name, INTERPRET_BRANCH_LOCAL);
	אם (strcmp(buf.buf, branch->name)) {
		حرر(branch->name);
		branch->name = xstrdup(buf.buf);
	}
	strbuf_splice(&buf, 0, 0, "refs/heads/", 11);
	حرر(branch->path);
	branch->path = strbuf_detach(&buf, عدم);
}

ساكن فراغ init_topts(بنية unpack_trees_options *topts,
		       صحيح show_progress, صحيح overwrite_ignore,
		       bool quiet)
{
	memset(topts, 0, حجم(*topts));
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

ساكن صحيح merge_working_tree(ثابت بنية checkout_opts *opts,
			      بنية branch_info *old_branch_info,
			      بنية branch_info *new_branch_info,
			      bool quiet,
			      صحيح *writeout_error)
{
	صحيح ret;
	بنية lock_file lock_file = LOCK_INIT;
	بنية tree *new_tree;

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);
	אם (repo_read_index_preload(the_repository, عدم, 0) < 0) {
		rollback_lock_file(&lock_file);
		החזר error(_("index file corrupt"));
	}

	resolve_undo_clear_index(the_repository->index);
	אם (opts->new_orphan_branch && opts->orphan_from_empty_tree) {
		אם (new_branch_info->commit)
			BUG("'switch --orphan' should never accept a commit as starting point");
		new_tree = repo_parse_tree_indirect(the_repository,
						    the_hash_algo->empty_tree);
		אם (!new_tree)
			BUG("unable to read empty tree");
	} אחרת {
		new_tree = repo_get_commit_tree(the_repository,
						new_branch_info->commit);
		אם (!new_tree) {
			rollback_lock_file(&lock_file);
			החזר error(_("unable to read tree (%s)"),
				     oid_to_hex(&new_branch_info->commit->object.oid));
		}
	}
	אם (opts->discard_changes) {
		ret = reset_tree(new_tree, opts, 1, writeout_error, new_branch_info);
		אם (ret) {
			rollback_lock_file(&lock_file);
			החזר ret;
		}
	} אחרת {
		بنية tree_desc trees[2];
		بنية tree *tree;
		بنية unpack_trees_options topts;
		ثابت بنية object_id *old_commit_oid;

		refresh_index(the_repository->index, REFRESH_QUIET, عدم, عدم, عدم);

		אם (unmerged_index(the_repository->index)) {
			rollback_lock_file(&lock_file);
			error(_("you need to resolve your current index first"));
			החזר 1;
		}

		/* 2-way merge to the new branch */
		init_topts(&topts, opts->show_progress,
			   opts->overwrite_ignore, quiet);
		init_checkout_metadata(&topts.meta, new_branch_info->refname,
				       new_branch_info->commit ?
				       &new_branch_info->commit->object.oid :
				       &new_branch_info->oid, عدم);

		old_commit_oid = old_branch_info->commit ?
			&old_branch_info->commit->object.oid :
			the_hash_algo->empty_tree;
		tree = repo_parse_tree_indirect(the_repository,
						old_commit_oid);
		אם (!tree)
			die(_("unable to parse commit %s"),
				oid_to_hex(old_commit_oid));

		init_tree_desc(&trees[0], &tree->object.oid,
			       tree->buffer, tree->size);
		אם (repo_parse_tree(the_repository, new_tree) < 0)
			die(عدم);
		tree = new_tree;
		init_tree_desc(&trees[1], &tree->object.oid,
			       tree->buffer, tree->size);

		ret = unpack_trees(2, trees, &topts);
		clear_unpack_trees_porcelain(&topts);
		אם (ret == -1) {
			rollback_lock_file(&lock_file);
			החזר MERGE_WORKING_TREE_UNPACK_FAILED;
		}
	}

	אם (!cache_tree_fully_valid(the_repository->index))
		cache_tree_update(the_repository->index, WRITE_TREE_SILENT | WRITE_TREE_REPAIR);

	אם (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
		die(_("unable to write new index file"));

	אם (!opts->discard_changes && !opts->quiet && new_branch_info->commit)
		show_local_changes(&new_branch_info->commit->object, &opts->diff_options);

	החזר 0;
}

ساكن فراغ report_tracking(بنية branch_info *new_branch_info)
{
	بنية strbuf sb = STRBUF_INIT;
	بنية branch *branch = branch_get(new_branch_info->name);

	אם (!format_tracking_info(branch, &sb, AHEAD_BEHIND_FULL, 1))
		החזר;
	fputs(sb.buf, stdout);
	strbuf_release(&sb);
}

ساكن فراغ update_refs_for_switch(ثابت بنية checkout_opts *opts,
				   بنية branch_info *old_branch_info,
				   بنية branch_info *new_branch_info)
{
	بنية strbuf msg = STRBUF_INIT;
	ثابت حرف *old_desc, *reflog_msg;
	אם (opts->new_branch) {
		אם (opts->new_orphan_branch) {
			تعداد log_refs_config log_all_ref_updates = LOG_REFS_UNSET;
			ثابت حرف *value;
			حرف *refname;

			אם (!repo_config_get_string_tmp(the_repository, "core.logallrefupdates", &value))
				log_all_ref_updates = refs_parse_log_all_ref_updates_config(value);

			refname = mkpathdup("refs/heads/%s", opts->new_orphan_branch);
			אם (opts->new_branch_log &&
			    !should_autocreate_reflog(log_all_ref_updates, refname)) {
				صحيح ret;
				بنية strbuf err = STRBUF_INIT;

				ret = refs_create_reflog(get_main_ref_store(the_repository),
							 refname, &err);
				אם (ret) {
					fprintf(stderr, _("Can not do reflog for '%s': %s\n"),
						opts->new_orphan_branch, err.buf);
					strbuf_release(&err);
					حرر(refname);
					החזר;
				}
				strbuf_release(&err);
			}
			حرر(refname);
		}
		אחרת
			create_branch(the_repository,
				      opts->new_branch, new_branch_info->name,
				      opts->new_branch_force ? 1 : 0,
				      opts->new_branch_force ? 1 : 0,
				      opts->new_branch_log,
				      opts->quiet,
				      opts->track,
				      0);
		حرر(new_branch_info->name);
		حرر(new_branch_info->refname);
		new_branch_info->name = xstrdup(opts->new_branch);
		setup_branch_path(new_branch_info);
	}

	old_desc = old_branch_info->name;
	אם (!old_desc && old_branch_info->commit)
		old_desc = oid_to_hex(&old_branch_info->commit->object.oid);

	reflog_msg = getenv("GIT_REFLOG_ACTION");
	אם (!reflog_msg)
		strbuf_addf(&msg, "checkout: moving from %s to %s",
			old_desc ? old_desc : "(invalid)", new_branch_info->name);
	אחרת
		strbuf_insertstr(&msg, 0, reflog_msg);

	אם (!strcmp(new_branch_info->name, "HEAD") && !new_branch_info->path && !opts->force_detach) {
		/* Nothing to do. */
	} אחרת אם (opts->force_detach || !new_branch_info->path) {	/* No longer on any branch. */
		refs_update_ref(get_main_ref_store(the_repository), msg.buf,
				"HEAD", &new_branch_info->commit->object.oid,
				عدم,
				REF_NO_DEREF, UPDATE_REFS_DIE_ON_ERR);
		אם (!opts->quiet) {
			אם (old_branch_info->path &&
			    advice_enabled(ADVICE_DETACHED_HEAD) && !opts->force_detach)
				detach_advice(new_branch_info->name);
			describe_detached_head(_("HEAD is now at"), new_branch_info->commit);
		}
	} אחרת אם (new_branch_info->path) {	/* Switch branches. */
		אם (refs_update_symref(get_main_ref_store(the_repository), "HEAD", new_branch_info->path, msg.buf) < 0)
			die(_("unable to update HEAD"));
		אם (!opts->quiet) {
			אם (old_branch_info->path && !strcmp(new_branch_info->path, old_branch_info->path)) {
				אם (opts->new_branch_force)
					fprintf(stderr, _("Reset branch '%s'\n"),
						new_branch_info->name);
				אחרת
					fprintf(stderr, _("Already on '%s'\n"),
						new_branch_info->name);
			} אחרת אם (opts->new_branch) {
				אם (opts->branch_exists)
					fprintf(stderr, _("Switched to and reset branch '%s'\n"), new_branch_info->name);
				אחרת
					fprintf(stderr, _("Switched to a new branch '%s'\n"), new_branch_info->name);
			} אחרת {
				fprintf(stderr, _("Switched to branch '%s'\n"),
					new_branch_info->name);
			}
		}
		אם (old_branch_info->path && old_branch_info->name) {
			אם (!refs_ref_exists(get_main_ref_store(the_repository), old_branch_info->path) && refs_reflog_exists(get_main_ref_store(the_repository), old_branch_info->path))
				refs_delete_reflog(get_main_ref_store(the_repository),
						   old_branch_info->path);
		}
	}
	remove_branch_state(the_repository, !opts->quiet);
	strbuf_release(&msg);
	אם (!opts->quiet &&
	    !opts->force_detach &&
	    (new_branch_info->path || !strcmp(new_branch_info->name, "HEAD")))
		report_tracking(new_branch_info);
}

ساكن صحيح add_pending_uninteresting_ref(ثابت بنية reference *ref, فراغ *cb_data)
{
	add_pending_oid(cb_data, ref->name, ref->oid, UNINTERESTING);
	החזר 0;
}

ساكن فراغ describe_one_orphan(بنية strbuf *sb, بنية commit *commit)
{
	strbuf_addstr(sb, "  ");
	strbuf_add_unique_abbrev(sb, &commit->object.oid, DEFAULT_ABBREV);
	strbuf_addch(sb, ' ');
	אם (!repo_parse_commit(the_repository, commit))
		pp_commit_easy(CMIT_FMT_ONELINE, commit, sb);
	strbuf_addch(sb, '\n');
}

#define ORPHAN_CUTOFF 4
ساكن فراغ suggest_reattach(بنية commit *commit, بنية rev_info *revs)
{
	بنية commit *c, *last = عدم;
	بنية strbuf sb = STRBUF_INIT;
	صحيح lost = 0;
	כלעוד ((c = get_revision(revs)) != عدم) {
		אם (lost < ORPHAN_CUTOFF)
			describe_one_orphan(&sb, c);
		last = c;
		lost++;
	}
	אם (ORPHAN_CUTOFF < lost) {
		صحيح more = lost - ORPHAN_CUTOFF;
		אם (more == 1)
			describe_one_orphan(&sb, last);
		אחרת
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

	אם (advice_enabled(ADVICE_DETACHED_HEAD))
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
ساكن فراغ orphaned_commit_warning(بنية commit *old_commit, بنية commit *new_commit)
{
	بنية rev_info revs;
	بنية object *object = &old_commit->object;

	repo_init_revisions(the_repository, &revs, عدم);
	setup_revisions(0, عدم, &revs, عدم);

	object->flags &= ~UNINTERESTING;
	add_pending_object(&revs, object, oid_to_hex(&object->oid));

	refs_for_each_ref(get_main_ref_store(the_repository),
			  add_pending_uninteresting_ref, &revs);
	אם (new_commit)
		add_pending_oid(&revs, "HEAD",
				&new_commit->object.oid,
				UNINTERESTING);

	אם (prepare_revision_walk(&revs))
		die(_("internal error in revision walk"));
	אם (!(old_commit->object.flags & UNINTERESTING))
		suggest_reattach(old_commit, &revs);
	אחרת
		describe_detached_head(_("Previous HEAD position was"), old_commit);

	/* Clean up objects used, as they will be reused. */
	repo_clear_commit_marks(the_repository, ALL_REV_FLAGS);
	release_revisions(&revs);
}

ساكن صحيح switch_branches(ثابت بنية checkout_opts *opts,
			   بنية branch_info *new_branch_info)
{
	صحيح ret = 0;
	بنية branch_info old_branch_info = { 0 };
	بنية object_id rev;
	صحيح flag, writeout_error = 0;
	صحيح do_merge = 1;
	صحيح created_autostash = 0;
	تعداد stash_apply_result autostash_res = STASH_APPLY_CLEAN;
	بنية strbuf old_commit_shortname = STRBUF_INIT;
	بنية strbuf autostash_msg = STRBUF_INIT;
	ثابت حرف *stash_label_base = عدم;

	trace2_cmd_mode("branch");

	memset(&old_branch_info, 0, حجم(old_branch_info));
	old_branch_info.path = refs_resolve_refdup(get_main_ref_store(the_repository),
						   "HEAD", 0, &rev, &flag);
	אם (old_branch_info.path)
		old_branch_info.commit = lookup_commit_reference_gently(the_repository, &rev, 1);
	אם (!(flag & REF_ISSYMREF))
		FREE_AND_NULL(old_branch_info.path);

	אם (old_branch_info.path) {
		ثابت حرف *ثابت prefix = "refs/heads/";
		ثابت حرف *p;
		אם (skip_prefix(old_branch_info.path, prefix, &p))
			old_branch_info.name = xstrdup(p);
	}

	אם (opts->new_orphan_branch && opts->orphan_from_empty_tree) {
		אם (new_branch_info->name)
			BUG("'switch --orphan' should never accept a commit as starting point");
		new_branch_info->commit = عدم;
		new_branch_info->name = xstrdup("(empty)");
		do_merge = 1;
	}

	אם (!new_branch_info->name) {
		new_branch_info->name = xstrdup("HEAD");
		new_branch_info->commit = old_branch_info.commit;
		אם (!new_branch_info->commit)
			die(_("You are on a branch yet to be born"));
		parse_commit_or_die(new_branch_info->commit);

		אם (opts->only_merge_on_switching_branches)
			do_merge = 0;
	}

	אם (old_branch_info.name) {
		stash_label_base = old_branch_info.name;
	} אחרת אם (old_branch_info.commit) {
		strbuf_add_unique_abbrev(&old_commit_shortname,
					 &old_branch_info.commit->object.oid,
					 DEFAULT_ABBREV);
		stash_label_base = old_commit_shortname.buf;
	}

	אם (do_merge) {
		ret = merge_working_tree(opts, &old_branch_info, new_branch_info,
					 opts->merge, &writeout_error);
		אם (ret == MERGE_WORKING_TREE_UNPACK_FAILED && opts->merge) {
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
		אם (created_autostash) {
			אם (opts->conflict_style >= 0) {
				بنية strbuf cfg = STRBUF_INIT;
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
		אם (ret) {
			branch_info_release(&old_branch_info);
			strbuf_release(&old_commit_shortname);
			strbuf_release(&autostash_msg);
			החזר ret < 0 ? 1 : ret;
		}
	}

	אם (!opts->quiet && !old_branch_info.path && old_branch_info.commit && new_branch_info->commit != old_branch_info.commit)
		orphaned_commit_warning(old_branch_info.commit, new_branch_info->commit);

	אם (autostash_res == STASH_APPLY_CONFLICT && !opts->quiet)
		fputc('\n', stderr);
	update_refs_for_switch(opts, &old_branch_info, new_branch_info);

	אם (created_autostash) {
		discard_index(the_repository->index);
		אם (repo_read_index(the_repository) < 0)
			die(_("index file corrupt"));

		אם (!opts->quiet && new_branch_info->commit) {
			printf(_("The following paths have local changes:\n"));
			show_local_changes(&new_branch_info->commit->object,
					   &opts->diff_options);
		}
	}

	ret = post_checkout_hook(old_branch_info.commit, new_branch_info->commit, 1);
	branch_info_release(&old_branch_info);
	strbuf_release(&old_commit_shortname);
	strbuf_release(&autostash_msg);

	החזר ret || writeout_error;
}

ساكن صحيح git_checkout_config(ثابت حرف *var, ثابت حرف *value,
			       ثابت بنية config_context *ctx, فراغ *cb)
{
	بنية checkout_opts *opts = cb;

	אם (!strcmp(var, "diff.ignoresubmodules")) {
		אם (!value)
			החזר config_error_nonbool(var);
		handle_ignore_submodules_arg(&opts->diff_options, value);
		החזר 0;
	}
	אם (!strcmp(var, "checkout.guess")) {
		opts->dwim_new_local_branch = git_config_bool(var, value);
		החזר 0;
	}

	אם (starts_with(var, "submodule."))
		החזר git_default_submodule_config(var, value, عدم);

	החזר git_xmerge_config(var, value, ctx, عدم);
}

ساكن فراغ setup_new_branch_info_and_source_tree(
	بنية branch_info *new_branch_info,
	بنية checkout_opts *opts,
	بنية object_id *rev,
	ثابت حرف *arg)
{
	بنية tree **source_tree = &opts->source_tree;
	بنية object_id branch_rev;

	/* treat '@' as a shortcut for 'HEAD' */
	new_branch_info->name = !strcmp(arg, "@") ? xstrdup("HEAD") :
						    xstrdup(arg);
	setup_branch_path(new_branch_info);

	אם (!check_refname_format(new_branch_info->path, 0) &&
	    !refs_read_ref(get_main_ref_store(the_repository), new_branch_info->path, &branch_rev))
		oidcpy(rev, &branch_rev);
	אחרת
		/* not an existing branch */
		FREE_AND_NULL(new_branch_info->path);

	new_branch_info->commit = lookup_commit_reference_gently(the_repository, rev, 1);
	אם (!new_branch_info->commit) {
		/* not a commit */
		*source_tree = repo_parse_tree_indirect(the_repository, rev);
		אם (!*source_tree)
			die(_("unable to read tree (%s)"), oid_to_hex(rev));
	} אחרת {
		parse_commit_or_die(new_branch_info->commit);
		*source_tree = repo_get_commit_tree(the_repository,
						    new_branch_info->commit);
		אם (!*source_tree)
			die(_("unable to read tree (%s)"),
			    oid_to_hex(&new_branch_info->commit->object.oid));
	}
}


تعداد checkout_command {
	CHECKOUT_CHECKOUT = 1,
	CHECKOUT_SWITCH = 2,
	CHECKOUT_RESTORE = 3,
};

ساكن فراغ advise_disambiguating_remotes(تعداد checkout_command which_command,
					  ثابت حرف *branch,
					  ثابت بنية string_list *matched_remote_names)
{
	ثابت حرف *cmdname;
	بنية string_list_item *item;

	بدل (which_command) {
	حالة CHECKOUT_CHECKOUT:
		cmdname = "checkout";
		اكسر;
	حالة CHECKOUT_SWITCH:
		cmdname = "switch";
		اكسر;
	افتراضي:
		BUG("command <%d> should not reach advise_disambiguating_remotes",
		    which_command);
		اكسر;
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

ساكن حرف *parse_remote_branch(ثابت حرف *arg,
				 بنية object_id *rev,
				 صحيح could_be_checkout_paths,
				 تعداد checkout_command which_command)
{
	صحيح num_matches = 0;
	بنية string_list matched_remote_names = STRING_LIST_INIT_DUP;

	حرف *remote = unique_tracking_name(arg, rev, &num_matches,
					    &matched_remote_names);

	אם (remote && could_be_checkout_paths) {
		die(_("'%s' could be both a local file and a tracking branch.\n"
			"Please use -- (and optionally --no-guess) to disambiguate"),
		    arg);
	}

	אם (!remote && num_matches > 1) {
		אם (advice_enabled(ADVICE_CHECKOUT_AMBIGUOUS_REMOTE_BRANCH_NAME))
			advise_disambiguating_remotes(which_command, arg,
						      &matched_remote_names);
		die(_("'%s' matched multiple (%d) remote tracking branches"),
		    arg, num_matches);
	}

	string_list_clear(&matched_remote_names, 0);

	החזר remote;
}

ساكن صحيح parse_branchname_arg(صحيح argc, ثابت حرف **argv,
				صحيح dwim_new_local_branch_ok,
				تعداد checkout_command which_command,
				بنية branch_info *new_branch_info,
				بنية checkout_opts *opts,
				بنية object_id *rev)
{
	ثابت حرف **new_branch = &opts->new_branch;
	صحيح argcount = 0;
	ثابت حرف *arg;
	حرف *remote = عدم;
	صحيح dash_dash_pos;
	صحيح has_dash_dash = 0;
	صحيح i;

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
	אם (!argc)
		החזר 0;

	אם (!opts->accept_pathspec) {
		אם (argc > 1)
			die(_("only one reference expected"));
		has_dash_dash = 1; /* helps disambiguate */
	}

	arg = argv[0];
	dash_dash_pos = -1;
	עבור (i = 0; i < argc; i++) {
		אם (opts->accept_pathspec && !strcmp(argv[i], "--")) {
			dash_dash_pos = i;
			اكسر;
		}
	}
	אם (dash_dash_pos == 0)
		החזר 1; /* case (2) */
	אחרת אם (dash_dash_pos == 1)
		has_dash_dash = 1; /* case (3) or (1) */
	אחרת אם (dash_dash_pos >= 2)
		die(_("only one reference expected, %d given."), dash_dash_pos);
	opts->count_checkout_paths = !opts->quiet && !has_dash_dash;

	אם (!strcmp(arg, "-"))
		arg = "@{-1}";

	אם (repo_get_oid_mb(the_repository, arg, rev)) {
		/*
		 * Either case (3) or (4), with <something> not being
		 * a commit, or an attempt to use case (1) with an
		 * invalid ref.
		 *
		 * It's likely an error, but we need to find out if
		 * we should auto-create the branch, case (3).(b).
		 */
		صحيح recover_with_dwim = dwim_new_local_branch_ok;

		صحيح could_be_checkout_paths = !has_dash_dash &&
			check_filename(opts->prefix, arg);

		אם (!has_dash_dash && !no_wildcard(arg))
			recover_with_dwim = 0;

		/*
		 * Accept "git checkout foo", "git checkout foo --"
		 * and "git switch foo" as candidates for dwim.
		 */
		אם (!(argc == 1 && !has_dash_dash) &&
		    !(argc == 2 && has_dash_dash) &&
		    opts->accept_pathspec)
			recover_with_dwim = 0;

		אם (recover_with_dwim) {
			remote = parse_remote_branch(arg, rev,
						     could_be_checkout_paths,
						     which_command);
			אם (remote) {
				*new_branch = arg;
				arg = remote;
				/* DWIMmed to create local branch, case (3).(b) */
			} אחרת {
				recover_with_dwim = 0;
			}
		}

		אם (!recover_with_dwim) {
			אם (has_dash_dash)
				die(_("invalid reference: %s"), arg);
			החזר argcount;
		}
	}

	/* we can't end up being in (2) anymore, eat the argument */
	argcount++;
	argv++;
	argc--;

	setup_new_branch_info_and_source_tree(new_branch_info, opts, rev, arg);

	אם (!opts->source_tree)                   /* case (1): want a tree */
		die(_("reference is not a tree: %s"), arg);

	אם (!has_dash_dash) {	/* case (3).(d) -> (1) */
		/*
		 * Do not complain the most common case
		 *	git checkout branch
		 * even if there happen to be a file called 'branch';
		 * it would be extremely annoying.
		 */
		אם (argc)
			verify_non_filename(the_repository, opts->prefix, arg);
	} אחרת אם (opts->accept_pathspec) {
		argcount++;
		argv++;
		argc--;
	}

	حرر(remote);
	החזר argcount;
}

ساكن صحيح switch_unborn_to_new_branch(ثابت بنية checkout_opts *opts)
{
	صحيح status;
	بنية strbuf branch_ref = STRBUF_INIT;

	trace2_cmd_mode("unborn");

	אם (!opts->new_branch)
		die(_("You are on a branch yet to be born"));
	strbuf_addf(&branch_ref, "refs/heads/%s", opts->new_branch);
	status = refs_update_symref(get_main_ref_store(the_repository),
				    "HEAD", branch_ref.buf, "checkout -b");
	strbuf_release(&branch_ref);
	אם (!opts->quiet)
		fprintf(stderr, _("Switched to a new branch '%s'\n"),
			opts->new_branch);
	החזר status;
}

ساكن فراغ die_expecting_a_branch(ثابت بنية branch_info *branch_info)
{
	بنية object_id oid;
	حرف *to_free;
	صحيح code;

	אם (repo_dwim_ref(the_repository, branch_info->name,
			  strlen(branch_info->name), &oid, &to_free, 0) == 1) {
		ثابت حرف *ref = to_free;

		אם (skip_prefix(ref, "refs/tags/", &ref))
			code = die_message(_("a branch is expected, got tag '%s'"), ref);
		אחרת אם (skip_prefix(ref, "refs/remotes/", &ref))
			code = die_message(_("a branch is expected, got remote branch '%s'"), ref);
		אחרת
			code = die_message(_("a branch is expected, got '%s'"), ref);
	}
	אחרת אם (branch_info->commit)
		code = die_message(_("a branch is expected, got commit '%s'"), branch_info->name);
	אחרת
		/*
		 * This case should never happen because we already die() on
		 * non-commit, but just in case.
		 */
		code = die_message(_("a branch is expected, got '%s'"), branch_info->name);

	אם (advice_enabled(ADVICE_SUGGEST_DETACHING_HEAD))
		advise(_("If you want to detach HEAD at the commit, try again with the --detach option."));

	exit(code);
}

ساكن فراغ die_if_some_operation_in_progress(فراغ)
{
	بنية wt_status_state state;

	memset(&state, 0, حجم(state));
	wt_status_get_state(the_repository, &state, 0);

	אם (state.merge_in_progress)
		die(_("cannot switch branch while merging\n"
		      "Consider \"git merge --quit\" "
		      "or \"git worktree add\"."));
	אם (state.am_in_progress)
		die(_("cannot switch branch in the middle of an am session\n"
		      "Consider \"git am --quit\" "
		      "or \"git worktree add\"."));
	אם (state.rebase_interactive_in_progress || state.rebase_in_progress)
		die(_("cannot switch branch while rebasing\n"
		      "Consider \"git rebase --quit\" "
		      "or \"git worktree add\"."));
	אם (state.cherry_pick_in_progress)
		die(_("cannot switch branch while cherry-picking\n"
		      "Consider \"git cherry-pick --quit\" "
		      "or \"git worktree add\"."));
	אם (state.revert_in_progress)
		die(_("cannot switch branch while reverting\n"
		      "Consider \"git revert --quit\" "
		      "or \"git worktree add\"."));
	אם (state.bisect_in_progress)
		warning(_("you are switching branch while bisecting"));

	wt_status_state_free_buffers(&state);
}

/*
 * die if attempting to checkout an existing branch that is in use
 * in another worktree, unless ignore-other-wortrees option is given.
 * The check is bypassed when the branch is already the current one,
 * as it will not make things any worse.
 */
ساكن فراغ die_if_switching_to_a_branch_in_use(بنية checkout_opts *opts,
						ثابت حرف *full_ref)
{
	صحيح flags;
	حرف *head_ref;

	אם (opts->ignore_other_worktrees)
		החזר;
	head_ref = refs_resolve_refdup(get_main_ref_store(the_repository),
				       "HEAD", 0, عدم, &flags);
	אם (head_ref && (!(flags & REF_ISSYMREF) || strcmp(head_ref, full_ref)))
		die_if_checked_out(full_ref, 1);
	حرر(head_ref);
}

ساكن صحيح checkout_branch(بنية checkout_opts *opts,
			   بنية branch_info *new_branch_info)
{
	بنية repo_config_values *cfg = repo_config_values(the_repository);
	صحيح noop_switch = (!new_branch_info->name &&
			   !opts->new_branch &&
			   !opts->force_detach);

	אם (opts->pathspec.nr)
		die(_("paths cannot be used with switching branches"));

	אם (opts->patch_mode)
		die(_("'%s' cannot be used with switching branches"),
		    "--patch");

	אם (opts->overlay_mode != -1)
		die(_("'%s' cannot be used with switching branches"),
		    "--[no]-overlay");

	אם (opts->writeout_stage) {
		ثابت حرف *msg;
		אם (noop_switch)
			msg = _("'%s' needs the paths to check out");
		אחרת
			msg = _("'%s' cannot be used with switching branches");
		die(msg, "--ours/--theirs");
	}

	אם (opts->force && opts->merge)
		die(_("'%s' cannot be used with '%s'"), "-f", "-m");

	אם (opts->discard_changes && opts->merge)
		die(_("'%s' cannot be used with '%s'"), "--discard-changes", "--merge");

	אם (opts->force_detach && opts->new_branch)
		die(_("'%s' cannot be used with '%s'"),
		    "--detach", "-b/-B/--orphan");

	אם (opts->new_orphan_branch) {
		אם (opts->track != BRANCH_TRACK_UNSPECIFIED)
			die(_("'%s' cannot be used with '%s'"), "--orphan", "-t");
		אם (opts->orphan_from_empty_tree && new_branch_info->name)
			die(_("'%s' cannot take <start-point>"), "--orphan");
	} אחרת אם (opts->force_detach) {
		אם (opts->track != BRANCH_TRACK_UNSPECIFIED)
			die(_("'%s' cannot be used with '%s'"), "--detach", "-t");
	} אחרת אם (opts->track == BRANCH_TRACK_UNSPECIFIED)
		opts->track = cfg->branch_track;

	אם (new_branch_info->name && !new_branch_info->commit)
		die(_("Cannot switch branch to a non-commit '%s'"),
		    new_branch_info->name);

	אם (noop_switch &&
	    !opts->switch_branch_doing_nothing_is_ok)
		die(_("missing branch or commit argument"));

	אם (!opts->implicit_detach &&
	    !opts->force_detach &&
	    !opts->new_branch &&
	    !opts->new_branch_force &&
	    new_branch_info->name &&
	    !new_branch_info->path)
		die_expecting_a_branch(new_branch_info);

	אם (!opts->can_switch_when_in_progress)
		die_if_some_operation_in_progress();

	/* "git checkout <branch>" */
	אם (new_branch_info->path && !opts->force_detach && !opts->new_branch)
		die_if_switching_to_a_branch_in_use(opts, new_branch_info->path);

	/* "git checkout -B <branch>" */
	אם (opts->new_branch_force) {
		حرف *full_ref = xstrfmt("refs/heads/%s", opts->new_branch);
		die_if_switching_to_a_branch_in_use(opts, full_ref);
		حرر(full_ref);
	}

	אם (!new_branch_info->commit && opts->new_branch) {
		بنية object_id rev;
		صحيح flag;

		אם (!refs_read_ref_full(get_main_ref_store(the_repository), "HEAD", 0, &rev, &flag) &&
		    (flag & REF_ISSYMREF) && is_null_oid(&rev))
			החזר switch_unborn_to_new_branch(opts);
	}
	החזר switch_branches(opts, new_branch_info);
}

ساكن صحيح parse_opt_conflict(ثابت بنية option *o, ثابت حرف *arg, صحيح unset)
{
	بنية checkout_opts *opts = o->value;

	אם (unset) {
		opts->conflict_style = -1;
		החזר 0;
	}
	opts->conflict_style = parse_conflict_style_name(arg);
	אם (opts->conflict_style < 0)
		החזר error(_("unknown conflict style '%s'"), arg);
	/* --conflict overrides a previous --no-merge */
	אם (!opts->merge)
		opts->merge = -1;

	החזר 0;
}

ساكن بنية option *add_common_options(بنية checkout_opts *opts,
					 بنية option *prevopts)
{
	بنية option options[] = {
		OPT__QUIET(&opts->quiet, N_("suppress progress reporting")),
		OPT_CALLBACK_F(0, "recurse-submodules", عدم,
			    "checkout", "control recursive updating of submodules",
			    PARSE_OPT_OPTARG, option_parse_recurse_submodules_worktree_updater),
		OPT_BOOL(0, "progress", &opts->show_progress, N_("force progress reporting")),
		OPT_BOOL('m', "merge", &opts->merge, N_("perform a 3-way merge with the new branch")),
		OPT_CALLBACK(0, "conflict", opts, N_("style"),
			     N_("conflict style (merge, diff3, or zdiff3)"),
			     parse_opt_conflict),
		OPT_END()
	};
	بنية option *newopts = parse_options_concat(prevopts, options);
	حرر(prevopts);
	החזר newopts;
}

ساكن بنية option *add_common_switch_branch_options(
	بنية checkout_opts *opts, بنية option *prevopts)
{
	بنية option options[] = {
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
	بنية option *newopts = parse_options_concat(prevopts, options);
	حرر(prevopts);
	החזר newopts;
}

ساكن بنية option *add_checkout_path_options(بنية checkout_opts *opts,
						بنية option *prevopts)
{
	بنية option options[] = {
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
	بنية option *newopts = parse_options_concat(prevopts, options);
	حرر(prevopts);
	החזר newopts;
}

/* create-branch option (either b or c) */
ساكن حرف cb_option = 'b';

ساكن صحيح checkout_main(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
			 بنية checkout_opts *opts, بنية option *options,
			 تعداد checkout_command which_command)
{
	صحيح parseopt_flags = 0;
	بنية branch_info new_branch_info = { 0 };
	صحيح ret;

	ساكن ثابت حرف * ثابت checkout_usage[] = {
		N_("git checkout [<options>] <branch>"),
		N_("git checkout [<options>] [<branch>] -- <file>..."),
		عدم,
	};

	ساكن ثابت حرف * ثابت switch_branch_usage[] = {
		N_("git switch [<options>] [<branch>]"),
		عدم,
	};

	ساكن ثابت حرف * ثابت restore_usage[] = {
		N_("git restore [<options>] [--source=<branch>] <file>..."),
		عدم,
	};

	ثابت حرف * ثابت *usagestr;

	بدل (which_command) {
	حالة CHECKOUT_CHECKOUT:
		usagestr = checkout_usage;
		اكسر;
	حالة CHECKOUT_SWITCH:
		usagestr = switch_branch_usage;
		اكسر;
	حالة CHECKOUT_RESTORE:
		usagestr = restore_usage;
		اكسر;
	افتراضي:
		BUG("no such checkout variant %d", which_command);
	}

	opts->overwrite_ignore = 1;
	opts->prefix = prefix;
	opts->show_progress = -1;

	repo_config(the_repository, git_checkout_config, opts);
	אם (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_full_index = 0;
	}

	opts->track = BRANCH_TRACK_UNSPECIFIED;

	אם (!opts->accept_pathspec && !opts->accept_ref)
		BUG("make up your mind, you need to take _something_");
	אם (opts->accept_pathspec && opts->accept_ref)
		parseopt_flags = PARSE_OPT_KEEP_DASHDASH;

	argc = parse_options(argc, argv, prefix, options,
			     usagestr, parseopt_flags);

	אם (opts->patch_context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	אם (opts->patch_interhunk_context < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	אם (!opts->patch_mode) {
		אם (opts->patch_context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--patch");
		אם (opts->patch_interhunk_context != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--patch");
		אם (!opts->auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--patch");
	}

	אם (opts->show_progress < 0) {
		אם (opts->quiet)
			opts->show_progress = 0;
		אחרת
			opts->show_progress = isatty(2);
	}

	/* --conflicts implies --merge */
	אם (opts->merge == -1)
		opts->merge = opts->conflict_style >= 0;

	אם (opts->force) {
		opts->discard_changes = 1;
		opts->ignore_unmerged_opt = "--force";
		opts->ignore_unmerged = 1;
	}

	אם ((!!opts->new_branch + !!opts->new_branch_force + !!opts->new_orphan_branch) > 1)
		die(_("options '-%c', '-%c', and '%s' cannot be used together"),
			cb_option, toupper(cb_option), "--orphan");

	אם (opts->overlay_mode == 1 && opts->patch_mode)
		die(_("options '%s' and '%s' cannot be used together"), "-p", "--overlay");

	אם (opts->checkout_index >= 0 || opts->checkout_worktree >= 0) {
		אם (opts->checkout_index < 0)
			opts->checkout_index = 0;
		אם (opts->checkout_worktree < 0)
			opts->checkout_worktree = 0;
	} אחרת {
		אם (opts->checkout_index < 0)
			opts->checkout_index = -opts->checkout_index - 1;
		אם (opts->checkout_worktree < 0)
			opts->checkout_worktree = -opts->checkout_worktree - 1;
	}
	אם (opts->checkout_index < 0 || opts->checkout_worktree < 0)
		BUG("these flags should be non-negative by now");
	/*
	 * convenient shortcut: "git restore --staged [--worktree]" equals
	 * "git restore --staged [--worktree] --source HEAD"
	 */
	אם (!opts->from_treeish && opts->checkout_index)
		opts->from_treeish = "HEAD";

	/*
	 * From here on, new_branch will contain the branch to be checked out,
	 * and new_branch_force and new_orphan_branch will tell us which one of
	 * -b/-B/-c/-C/--orphan is being used.
	 */
	אם (opts->new_branch_force)
		opts->new_branch = opts->new_branch_force;

	אם (opts->new_orphan_branch)
		opts->new_branch = opts->new_orphan_branch;

	/* --track without -c/-C/-b/-B/--orphan should DWIM */
	אם (opts->track != BRANCH_TRACK_UNSPECIFIED && !opts->new_branch) {
		ثابت حرف *argv0 = argv[0];
		אם (!argc || !strcmp(argv0, "--"))
			die(_("--track needs a branch name"));
		skip_prefix(argv0, "refs/", &argv0);
		skip_prefix(argv0, "remotes/", &argv0);
		argv0 = strchr(argv0, '/');
		אם (!argv0 || !argv0[1])
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
	אם (argc && opts->accept_ref) {
		بنية object_id rev;
		صحيح dwim_ok =
			!opts->patch_mode &&
			opts->dwim_new_local_branch &&
			opts->track == BRANCH_TRACK_UNSPECIFIED &&
			!opts->new_branch;
		صحيح n = parse_branchname_arg(argc, argv, dwim_ok, which_command,
					     &new_branch_info, opts, &rev);
		argv += n;
		argc -= n;
	} אחרת אם (!opts->accept_ref && opts->from_treeish) {
		بنية object_id rev;

		אם (repo_get_oid_mb(the_repository, opts->from_treeish, &rev))
			die(_("could not resolve '%s'"), opts->from_treeish);

		setup_new_branch_info_and_source_tree(&new_branch_info,
						      opts, &rev,
						      opts->from_treeish);

		אם (!opts->source_tree)
			die(_("reference is not a tree: %s"), opts->from_treeish);
	}

	אם (argc) {
		parse_pathspec(&opts->pathspec, 0,
			       opts->patch_mode ? PATHSPEC_PREFIX_ORIGIN : 0,
			       prefix, argv);

		אם (!opts->pathspec.nr)
			die(_("invalid path specification"));

		/*
		 * Try to give more helpful suggestion.
		 * new_branch && argc > 1 will be caught later.
		 */
		אם (opts->new_branch && argc == 1 && !new_branch_info.commit)
			die(_("'%s' is not a commit and a branch '%s' cannot be created from it"),
				argv[0], opts->new_branch);

		אם (opts->force_detach)
			die(_("git checkout: --detach does not take a path argument '%s'"),
			    argv[0]);
	}

	אם (opts->pathspec_from_file) {
		אם (opts->pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		אם (opts->force_detach)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--detach");

		אם (opts->patch_mode)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--patch");

		parse_pathspec_file(&opts->pathspec, 0,
				    0,
				    prefix, opts->pathspec_from_file, opts->pathspec_file_nul);
	} אחרת אם (opts->pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	opts->pathspec.recursive = 1;

	אם (opts->pathspec.nr) {
		אם (1 < !!opts->writeout_stage + !!opts->force + !!opts->merge)
			die(_("git checkout: --ours/--theirs, --force and --merge are incompatible when\n"
			      "checking out of the index."));
	} אחרת {
		אם (opts->accept_pathspec && !opts->empty_pathspec_ok &&
		    !opts->patch_mode)	/* patch mode is special */
			die(_("you must specify path(s) to restore"));
	}

	אם (opts->new_branch) {
		بنية strbuf buf = STRBUF_INIT;

		אם (opts->new_branch_force)
			opts->branch_exists = validate_branchname(opts->new_branch, &buf);
		אחרת
			opts->branch_exists =
				validate_new_branchname(opts->new_branch, &buf, 0);
		strbuf_release(&buf);
	}

	אם (opts->patch_mode || opts->pathspec.nr)
		ret = checkout_paths(opts, &new_branch_info);
	אחרת
		ret = checkout_branch(opts, &new_branch_info);

	branch_info_release(&new_branch_info);
	clear_pathspec(&opts->pathspec);
	حرر(opts->pathspec_from_file);
	حرر(options);

	החזר ret;
}

صحيح cmd_checkout(صحيح argc,
		 ثابت حرف **argv,
		 ثابت حرف *prefix,
		 بنية repository *repo UNUSED)
{
	بنية checkout_opts opts = CHECKOUT_OPTS_INIT;
	بنية option *options;
	بنية option checkout_options[] = {
		OPT_STRING('b', عدم, &opts.new_branch, N_("branch"),
			   N_("create and checkout a new branch")),
		OPT_STRING('B', عدم, &opts.new_branch_force, N_("branch"),
			   N_("create/reset and checkout a branch")),
		OPT_BOOL('l', عدم, &opts.new_branch_log, N_("create reflog for new branch")),
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

	אם (argc == 3 && !strcmp(argv[1], "-b")) {
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

	החזר checkout_main(argc, argv, prefix, &opts, options,
			     CHECKOUT_CHECKOUT);
}

صحيح cmd_switch(صحيح argc,
	       ثابت حرف **argv,
	       ثابت حرف *prefix,
	       بنية repository *repo UNUSED)
{
	بنية checkout_opts opts = CHECKOUT_OPTS_INIT;
	بنية option *options = عدم;
	بنية option switch_options[] = {
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

	החזר checkout_main(argc, argv, prefix, &opts, options,
			     CHECKOUT_SWITCH);
}

صحيح cmd_restore(صحيح argc,
		ثابت حرف **argv,
		ثابت حرف *prefix,
		بنية repository *repo UNUSED)
{
	بنية checkout_opts opts = CHECKOUT_OPTS_INIT;
	بنية option *options;
	بنية option restore_options[] = {
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

	החזר checkout_main(argc, argv, prefix, &opts, options,
			     CHECKOUT_RESTORE);
}
