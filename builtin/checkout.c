#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#شامل "builtin.h"
#شامل "advice.h"
#شامل "branch.h"
#شامل "cache-tree.h"
#شامل "checkout.h"
#شامل "commit.h"
#شامل "config.h"
#شامل "diff.h"
#شامل "dir.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "hex.h"
#شامل "hook.h"
#شامل "merge-ll.h"
#شامل "lockfile.h"
#شامل "mem-pool.h"
#شامل "object-file.h"
#شامل "object-name.h"
#شامل "odb.h"
#شامل "parse-options.h"
#شامل "path.h"
#شامل "preload-index.h"
#شامل "read-cache.h"
#شامل "refs.h"
#شامل "remote.h"
#شامل "repo-settings.h"
#شامل "resolve-undo.h"
#شامل "revision.h"
#شامل "sequencer.h"
#شامل "setup.h"
#شامل "sparse-index.h"
#شامل "strvec.h"
#شامل "submodule.h"
#شامل "symlinks.h"
#شامل "trace2.h"
#شامل "tree.h"
#شامل "tree-walk.h"
#شامل "unpack-trees.h"
#شامل "wt-status.h"
#شامل "xdiff-interface.h"
#شامل "entry.h"
#شامل "parallel-checkout.h"
#شامل "add-interactive.h"

ساختار checkout_opts {
	صحیح patch_mode;
	صحیح patch_context;
	صحیح patch_interhunk_context;
	صحیح auto_advance;
	صحیح quiet;
	صحیح merge;
	صحیح force;
	صحیح force_detach;
	صحیح implicit_detach;
	صحیح writeout_stage;
	صحیح overwrite_ignore;
	صحیح ignore_skipworktree;
	صحیح ignore_other_worktrees;
	صحیح show_progress;
	صحیح شمار_checkout_paths;
	صحیح overlay_mode;
	صحیح dwim_new_local_branch;
	صحیح discard_changes;
	صحیح accept_ref;
	صحیح accept_pathspec;
	صحیح switch_branch_doing_nothing_is_ok;
	صحیح only_merge_on_switching_branches;
	صحیح can_switch_when_in_progress;
	صحیح orphan_from_empty_درخت;
	صحیح empty_pathspec_ok;
	صحیح checkout_index;
	صحیح checkout_worktree;
	ثابت نویسه *ignore_unmerged_opt;
	صحیح ignore_unmerged;
	صحیح pathspec_file_nul;
	نویسه *pathspec_from_file;

	ثابت نویسه *new_branch;
	ثابت نویسه *new_branch_force;
	ثابت نویسه *new_orphan_branch;
	صحیح new_branch_log;
	شمارش branch_track track;
	ساختار diff_options diff_options;
	صحیح conflict_style;

	صحیح branch_exists;
	ثابت نویسه *prefix;
	ساختار pathspec pathspec;
	ثابت نویسه *from_treeish;
	ساختار درخت *source_درخت;
};

#define CHECKOUT_OPTS_INIT { \
	.conflict_style = -1, \
	.merge = -1, \
	.patch_context = -1, \
	.patch_interhunk_context = -1, \
	.auto_advance = 1, \
}

#define MERGE_WORKING_درخت_UNPACK_FAILED (-2)

ساختار branch_info {
	نویسه *name; /* The short name used */
	نویسه *path; /* The full name of a real branch */
	ساختار commit *commit; /* The named commit */
	نویسه *refname; /* The full name of the ref being checked out. */
	ساختار object_id oid; /* The object ID of the commit being checked out. */
	/*
	 * if not null the branch is detached because it's already
	 * checked out in this checkout
	 */
	نویسه *checkout;
};

ایستا تهی branch_info_release(ساختار branch_info *info)
{
	آزاد(info->name);
	آزاد(info->path);
	آزاد(info->refname);
	آزاد(info->checkout);
}

ایستا صحیح post_checkout_hook(ساختار commit *old_commit, ساختار commit *new_commit,
			      صحیح changed)
{
	ساختار run_hooks_opt opt = RUN_HOOKS_OPT_INIT_FORCE_SERIAL;

	/*
	 * "new_commit" can be NULL when checking out from the index before
	 * a commit exists.
	 */
	strvec_pushl(&opt.args,
		     oid_to_hex(old_commit ? &old_commit->object.oid : null_oid(the_hash_algo)),
		     oid_to_hex(new_commit ? &new_commit->object.oid : null_oid(the_hash_algo)),
		     changed ? "1" : "0",
		     پوچ);

	بازگشت run_hooks_opt(the_repository, "post-checkout", &opt);
}

/*
 * Handle a tree object and determine if we need to recurse into the
 * tree (READ_TREE_RECURSIVE) or skip it (0).
 */
ایستا صحیح try_update_sparse_directory(ثابت ساختار object_id *oid,
				       ساختار strbuf *base,
				       ثابت نویسه *pathname,
				       صحیح overlay_mode)
{
	ساختار strbuf dirpath = STRBUF_INIT;
	ساختار cache_entry *old;
	صحیح pos, result = READ_درخت_RECURSIVE;

	اگر (!the_repository->index->sparse_index)
		بازگشت result;

	strbuf_addbuf(&dirpath, base);
	strbuf_addstr(&dirpath, pathname);
	strbuf_addch(&dirpath, '/');

	pos = index_name_pos_sparse(the_repository->index,
				    dirpath.buf, dirpath.len);
	اگر (pos < 0)
		برو cleanup;

	old = the_repository->index->cache[pos];
	اگر (!S_ISSPARSEDIR(old->ce_mode))
		برو cleanup;

	اگر (oideq(oid, &old->oid)) {
		/* Tree content already matches; no need to descend. */
		result = 0;
	} وگرنه اگر (!overlay_mode) {
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
	بازگشت result;
}

ایستا صحیح update_some(ثابت ساختار object_id *oid, ساختار strbuf *base,
		       ثابت نویسه *pathname, unsigned mode, تهی *context)
{
	صحیح len;
	ساختار cache_entry *ce;
	صحیح pos;
	صحیح overlay_mode = context ? *((صحیح *)context) : 1;

	اگر (S_ISDIR(mode))
		بازگشت try_update_sparse_directory(oid, base, pathname,
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
	اگر (pos >= 0) {
		ساختار cache_entry *old = the_repository->index->cache[pos];
		اگر (ce->ce_mode == old->ce_mode &&
		    !ce_intent_to_add(old) &&
		    oideq(&ce->oid, &old->oid)) {
			old->ce_flags |= CE_UPDATE;
			discard_cache_entry(ce);
			بازگشت 0;
		}
	}

	add_index_entry(the_repository->index, ce,
			ADD_CACHE_OK_TO_ADD | ADD_CACHE_OK_TO_REPLACE);
	بازگشت 0;
}

ایستا صحیح read_درخت_some(ساختار درخت *درخت, ثابت ساختار pathspec *pathspec,
			  صحیح overlay_mode)
{
	read_درخت(the_repository, درخت,
		  pathspec, update_some, &overlay_mode);

	/* update the index with the given tree's info
	 * for all args, expanding wildcards, and exit
	 * with any non-zero return code.
	 */
	بازگشت 0;
}

ایستا صحیح skip_same_name(ثابت ساختار cache_entry *ce, صحیح pos)
{
	وقتی (++pos < the_repository->index->cache_nr &&
	       !strcmp(the_repository->index->cache[pos]->name, ce->name))
		; /* skip */
	بازگشت pos;
}

ایستا صحیح check_stage(صحیح stage, ثابت ساختار cache_entry *ce, صحیح pos,
		       صحیح overlay_mode)
{
	وقتی (pos < the_repository->index->cache_nr &&
	       !strcmp(the_repository->index->cache[pos]->name, ce->name)) {
		اگر (ce_stage(the_repository->index->cache[pos]) == stage)
			بازگشت 0;
		pos++;
	}
	اگر (!overlay_mode)
		بازگشت 0;
	اگر (stage == 2)
		بازگشت error(_("path '%s' does not have our version"), ce->name);
	وگرنه
		بازگشت error(_("path '%s' does not have their version"), ce->name);
}

ایستا صحیح check_stages(unsigned stages, ثابت ساختار cache_entry *ce, صحیح pos)
{
	unsigned seen = 0;
	ثابت نویسه *name = ce->name;

	وقتی (pos < the_repository->index->cache_nr) {
		ce = the_repository->index->cache[pos];
		اگر (strcmp(name, ce->name))
			بشکن;
		seen |= (1 << ce_stage(ce));
		pos++;
	}
	اگر ((stages & seen) != stages)
		بازگشت error(_("path '%s' does not have all necessary versions"),
			     name);
	بازگشت 0;
}

ایستا صحیح checkout_stage(صحیح stage, ثابت ساختار cache_entry *ce, صحیح pos,
			  ثابت ساختار checkout *state, صحیح *nr_checkouts,
			  صحیح overlay_mode)
{
	وقتی (pos < the_repository->index->cache_nr &&
	       !strcmp(the_repository->index->cache[pos]->name, ce->name)) {
		اگر (ce_stage(the_repository->index->cache[pos]) == stage)
			بازگشت checkout_entry(the_repository->index->cache[pos], state,
					      پوچ, nr_checkouts);
		pos++;
	}
	اگر (!overlay_mode) {
		unlink_entry(ce, پوچ);
		بازگشت 0;
	}
	اگر (stage == 2)
		بازگشت error(_("path '%s' does not have our version"), ce->name);
	وگرنه
		بازگشت error(_("path '%s' does not have their version"), ce->name);
}

ایستا صحیح checkout_merged(صحیح pos, ثابت ساختار checkout *state,
			   صحیح *nr_checkouts, ساختار mem_pool *ce_mem_pool,
			   صحیح conflict_style)
{
	ساختار cache_entry *ce = the_repository->index->cache[pos];
	ثابت نویسه *path = ce->name;
	mmfile_t ancestor, ours, theirs;
	شمارش ll_merge_result merge_status;
	صحیح status;
	ساختار object_id oid;
	mmfile_t result_buf;
	ساختار object_id threeway[3];
	unsigned mode = 0;
	ساختار ll_merge_options ll_opts = LL_MERGE_OPTIONS_INIT;
	صحیح renormalize = 0;

	memset(threeway, 0, اندازه(threeway));
	وقتی (pos < the_repository->index->cache_nr) {
		صحیح stage;
		stage = ce_stage(ce);
		اگر (!stage || strcmp(path, ce->name))
			بشکن;
		oidcpy(&threeway[stage - 1], &ce->oid);
		اگر (stage == 2)
			mode = create_ce_mode(ce->ce_mode);
		pos++;
		ce = the_repository->index->cache[pos];
	}
	اگر (is_null_oid(&threeway[1]) || is_null_oid(&threeway[2]))
		بازگشت error(_("path '%s' does not have necessary versions"), path);

	read_mmblob(&ancestor, the_repository->objects, &threeway[0]);
	read_mmblob(&ours, the_repository->objects, &threeway[1]);
	read_mmblob(&theirs, the_repository->objects, &threeway[2]);

	repo_config_get_bool(the_repository, "merge.renormalize", &renormalize);
	ll_opts.renormalize = renormalize;
	ll_opts.conflict_style = conflict_style;
	merge_status = ll_merge(&result_buf, path, &ancestor, "base",
				&ours, "ours", &theirs, "theirs",
				state->istate, &ll_opts);
	آزاد(ancestor.ptr);
	آزاد(ours.ptr);
	آزاد(theirs.ptr);
	اگر (merge_status == LL_MERGE_دودویی_CONFLICT)
		warning("Cannot merge binary files: %s (%s vs. %s)",
			path, "ours", "theirs");
	اگر (merge_status < 0 || !result_buf.ptr) {
		آزاد(result_buf.ptr);
		بازگشت error(_("path '%s': cannot merge"), path);
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
	اگر (odb_write_object(the_repository->objects, result_buf.ptr, result_buf.size, OBJ_BLOB, &oid))
		die(_("Unable to add merge result for '%s'"), path);
	آزاد(result_buf.ptr);
	ce = make_transient_cache_entry(mode, &oid, path, 2, ce_mem_pool);
	اگر (!ce)
		die(_("make_cache_entry failed for path '%s'"), path);
	status = checkout_entry(ce, state, پوچ, nr_checkouts);
	بازگشت status;
}

ایستا تهی mark_ce_for_checkout_overlay(ساختار cache_entry *ce,
					 نویسه *ps_matched,
					 ثابت ساختار checkout_opts *opts)
{
	ce->ce_flags &= ~CE_MATCHED;
	اگر (!opts->ignore_skipworktree && ce_skip_worktree(ce))
		بازگشت;
	اگر (opts->source_درخت && !(ce->ce_flags & CE_UPDATE))
		/*
		 * "git checkout tree-ish -- path", but this entry
		 * is in the original index but is not in tree-ish
		 * or does not match the pathspec; it will not be
		 * checked out to the working tree.  We will not do
		 * anything to this entry at all.
		 */
		بازگشت;
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
	اگر (ce_path_match(the_repository->index, ce, &opts->pathspec, ps_matched))
		ce->ce_flags |= CE_MATCHED;
}

ایستا تهی mark_ce_for_checkout_no_overlay(ساختار cache_entry *ce,
					    نویسه *ps_matched,
					    ثابت ساختار checkout_opts *opts)
{
	ce->ce_flags &= ~CE_MATCHED;
	اگر (!opts->ignore_skipworktree && ce_skip_worktree(ce))
		بازگشت;
	اگر (ce_path_match(the_repository->index, ce, &opts->pathspec, ps_matched)) {
		ce->ce_flags |= CE_MATCHED;
		اگر (opts->source_درخت && !(ce->ce_flags & CE_UPDATE))
			/*
			 * In overlay mode, but the path is not in
			 * tree-ish, which means we should remove it
			 * from the index and the working tree.
			 */
			ce->ce_flags |= CE_REMOVE | CE_WT_REMOVE;
	}
}

ایستا صحیح checkout_worktree(ثابت ساختار checkout_opts *opts,
			     ثابت ساختار branch_info *info)
{
	ساختار checkout state = CHECKOUT_INIT;
	صحیح nr_checkouts = 0, nr_unmerged = 0;
	صحیح errs = 0;
	صحیح pos;
	صحیح pc_workers, pc_threshold;
	ساختار mem_pool ce_mem_pool;

	state.force = 1;
	state.refresh_cache = 1;
	state.istate = the_repository->index;

	mem_pool_init(&ce_mem_pool, 0);
	get_parallel_checkout_configs(&pc_workers, &pc_threshold);
	init_checkout_metadata(&state.meta, info->refname,
			       info->commit ? &info->commit->object.oid : &info->oid,
			       پوچ);

	enable_delayed_checkout(&state);

	اگر (pc_workers > 1)
		init_parallel_checkout();

	برای (pos = 0; pos < the_repository->index->cache_nr; pos++) {
		ساختار cache_entry *ce = the_repository->index->cache[pos];
		اگر (ce->ce_flags & CE_MATCHED) {
			اگر (!ce_stage(ce)) {
				errs |= checkout_entry(ce, &state,
						       پوچ, &nr_checkouts);
				ادامه;
			}
			اگر (opts->writeout_stage)
				errs |= checkout_stage(opts->writeout_stage,
						       ce, pos,
						       &state,
						       &nr_checkouts, opts->overlay_mode);
			وگرنه اگر (opts->merge)
				errs |= checkout_merged(pos, &state,
							&nr_unmerged,
							&ce_mem_pool,
							opts->conflict_style);
			pos = skip_same_name(ce, pos) - 1;
		}
	}
	اگر (pc_workers > 1)
		errs |= run_parallel_checkout(&state, pc_workers, pc_threshold,
					      پوچ, پوچ);
	mem_pool_discard(&ce_mem_pool, should_validate_cache_entries());
	remove_marked_cache_entries(the_repository->index, 1);
	remove_scheduled_dirs();
	errs |= finish_delayed_checkout(&state, opts->show_progress);

	اگر (opts->شمار_checkout_paths) {
		اگر (nr_unmerged)
			fprintf_ln(stderr, Q_("Recreated %d merge conflict",
					      "Recreated %d merge conflicts",
					      nr_unmerged),
				   nr_unmerged);
		اگر (opts->source_درخت)
			fprintf_ln(stderr, Q_("Updated %d path from %s",
					      "Updated %d paths from %s",
					      nr_checkouts),
				   nr_checkouts,
				   repo_find_unique_abbrev(the_repository, &opts->source_درخت->object.oid,
							   DEFAULT_ABBREV));
		وگرنه اگر (!nr_unmerged || nr_checkouts)
			fprintf_ln(stderr, Q_("Updated %d path from the index",
					      "Updated %d paths from the index",
					      nr_checkouts),
				   nr_checkouts);
	}

	بازگشت errs;
}

ایستا صحیح checkout_paths(ثابت ساختار checkout_opts *opts,
			  ثابت ساختار branch_info *new_branch_info)
{
	صحیح pos;
	ایستا نویسه *ps_matched;
	ساختار object_id rev;
	ساختار commit *head;
	صحیح errs = 0;
	ساختار lock_file lock_file = LOCK_INIT;
	صحیح checkout_index;

	trace2_cmd_mode(opts->patch_mode ? "patch" : "path");

	اگر (opts->track != BRANCH_TRACK_UNSPECIFIED)
		die(_("'%s' cannot be used with updating paths"), "--track");

	اگر (opts->new_branch_log)
		die(_("'%s' cannot be used with updating paths"), "-l");

	اگر (opts->ignore_unmerged && opts->patch_mode)
		die(_("'%s' cannot be used with updating paths"),
		    opts->ignore_unmerged_opt);

	اگر (opts->force_detach)
		die(_("'%s' cannot be used with updating paths"), "--detach");

	اگر (opts->merge && opts->patch_mode)
		die(_("options '%s' and '%s' cannot be used together"), "--merge", "--patch");

	اگر (opts->ignore_unmerged && opts->merge)
		die(_("options '%s' and '%s' cannot be used together"),
		    opts->ignore_unmerged_opt, "-m");

	اگر (opts->new_branch)
		die(_("Cannot update paths and switch to branch '%s' at the same time."),
		    opts->new_branch);

	اگر (!opts->checkout_worktree && !opts->checkout_index)
		die(_("neither '%s' or '%s' is specified"),
		    "--staged", "--worktree");

	اگر (!opts->checkout_worktree && !opts->from_treeish)
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
	اگر (!opts->accept_ref && opts->checkout_index) {
		اگر (opts->writeout_stage)
			die(_("'%s' or '%s' cannot be used with %s"),
			    "--ours", "--theirs", "--staged");

		اگر (opts->merge)
			die(_("'%s' or '%s' cannot be used with %s"),
			    "--merge", "--conflict", "--staged");
	}

	/*
	 * recreating unmerged index entries and writing out data from
	 * unmerged index entries would make no sense when checking out
	 * of a tree-ish.
	 */
	اگر ((opts->merge || opts->writeout_stage) && opts->source_درخت)
		die(_("'%s', '%s', or '%s' cannot be used when checking out of a tree"),
		    "--merge", "--ours", "--theirs");

	اگر (opts->patch_mode) {
		شمارش add_p_mode patch_mode;
		ساختار interactive_options interactive_opts = {
			.context = opts->patch_context,
			.interhunkcontext = opts->patch_interhunk_context,
			.auto_advance = opts->auto_advance
		};
		ثابت نویسه *rev = new_branch_info->name;
		نویسه rev_oid[GIT_MAX_HEXSZ + 1];

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
		اگر (rev && new_branch_info->commit && strcmp(rev, "HEAD"))
			rev = oid_to_hex_r(rev_oid, &new_branch_info->commit->object.oid);

		اگر (opts->checkout_index && opts->checkout_worktree)
			patch_mode = ADD_P_CHECKOUT;
		وگرنه اگر (opts->checkout_index && !opts->checkout_worktree)
			patch_mode = ADD_P_RESET;
		وگرنه اگر (!opts->checkout_index && opts->checkout_worktree)
			patch_mode = ADD_P_WORKTREE;
		وگرنه
			BUG("either flag must have been set, worktree=%d, index=%d",
			    opts->checkout_worktree, opts->checkout_index);
		بازگشت !!run_add_p(the_repository, patch_mode, &interactive_opts,
				   rev, &opts->pathspec, 0);
	}

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);
	اگر (repo_read_index_preload(the_repository, &opts->pathspec, 0) < 0)
		بازگشت error(_("index file corrupt"));

	اگر (opts->source_درخت)
		read_درخت_some(opts->source_درخت, &opts->pathspec,
			       opts->overlay_mode);
	اگر (opts->merge)
		unmerge_index(the_repository->index, &opts->pathspec, CE_MATCHED);

	ps_matched = xcalloc(opts->pathspec.nr, 1);

	/*
	 * Make sure all pathspecs participated in locating the paths
	 * to be checked out.
	 */
	برای (pos = 0; pos < the_repository->index->cache_nr; pos++)
		اگر (opts->overlay_mode)
			mark_ce_for_checkout_overlay(the_repository->index->cache[pos],
						     ps_matched,
						     opts);
		وگرنه
			mark_ce_for_checkout_no_overlay(the_repository->index->cache[pos],
							ps_matched,
							opts);

	اگر (report_path_error(ps_matched, &opts->pathspec)) {
		آزاد(ps_matched);
		بازگشت 1;
	}
	آزاد(ps_matched);

	/* Any unmerged paths? */
	برای (pos = 0; pos < the_repository->index->cache_nr; pos++) {
		ثابت ساختار cache_entry *ce = the_repository->index->cache[pos];
		اگر (ce->ce_flags & CE_MATCHED) {
			اگر (!ce_stage(ce))
				ادامه;
			اگر (opts->ignore_unmerged) {
				اگر (!opts->quiet)
					warning(_("path '%s' is unmerged"), ce->name);
			} وگرنه اگر (opts->writeout_stage) {
				errs |= check_stage(opts->writeout_stage, ce, pos, opts->overlay_mode);
			} وگرنه اگر (opts->merge) {
				errs |= check_stages((1<<2) | (1<<3), ce, pos);
			} وگرنه {
				errs = 1;
				error(_("path '%s' is unmerged"), ce->name);
			}
			pos = skip_same_name(ce, pos) - 1;
		}
	}
	اگر (errs)
		بازگشت 1;

	/* Now we are committed to check them out */
	اگر (opts->checkout_worktree)
		errs |= checkout_worktree(opts, new_branch_info);
	وگرنه
		remove_marked_cache_entries(the_repository->index, 1);

	/*
	 * Allow updating the index when checking out from the index.
	 * This is to save new stat info.
	 */
	اگر (opts->checkout_worktree && !opts->checkout_index && !opts->source_درخت)
		checkout_index = 1;
	وگرنه
		checkout_index = opts->checkout_index;

	اگر (checkout_index) {
		اگر (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
			die(_("unable to write new index file"));
	} وگرنه {
		/*
		 * NEEDSWORK: if --worktree is not specified, we
		 * should save stat info of checked out files in the
		 * index to avoid the next (potentially costly)
		 * refresh. But it's a bit tricker to do...
		 */
		rollback_lock_file(&lock_file);
	}

	refs_read_ref_full(get_main_ref_store(the_repository), "HEAD", 0,
			   &rev, پوچ);
	head = lookup_commit_reference_gently(the_repository, &rev, 1);

	errs |= post_checkout_hook(head, head, 0);
	بازگشت errs;
}

ایستا تهی show_local_changes(ساختار object *head,
			       ثابت ساختار diff_options *opts)
{
	ساختار rev_info rev;
	/* I think we want full paths, even if we're in a subdirectory. */
	repo_init_revisions(the_repository, &rev, پوچ);
	rev.diffopt.flags = opts->flags;
	rev.diffopt.output_format |= DIFF_FORMAT_NAME_STATUS;
	rev.diffopt.flags.recursive = 1;
	diff_setup_done(&rev.diffopt);
	add_pending_object(&rev, head, پوچ);
	run_diff_index(&rev, 0);
	release_revisions(&rev);
}

ایستا تهی describe_detached_head(ثابت نویسه *پیام, ساختار commit *commit)
{
	ساختار strbuf sb = STRBUF_INIT;

	اگر (!repo_parse_commit(the_repository, commit))
		pp_commit_easy(CMIT_FMT_ONELINE, commit, &sb);
	اگر (print_sha1_ellipsis()) {
		fprintf(stderr, "%s %s... %s\n", پیام,
			repo_find_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV),
			sb.buf);
	} وگرنه {
		fprintf(stderr, "%s %s %s\n", پیام,
			repo_find_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV),
			sb.buf);
	}
	strbuf_release(&sb);
}

ایستا صحیح reset_درخت(ساختار درخت *درخت, ثابت ساختار checkout_opts *o,
		      صحیح worktree, صحیح *writeout_error,
		      ساختار branch_info *info)
{
	ساختار unpack_trees_options opts;
	ساختار درخت_desc درخت_desc;

	memset(&opts, 0, اندازه(opts));
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
			       پوچ);
	اگر (repo_parse_درخت(the_repository, درخت) < 0)
		بازگشت 128;
	init_درخت_desc(&درخت_desc, &درخت->object.oid, درخت->buffer, درخت->size);
	انتخاب (unpack_trees(1, &درخت_desc, &opts)) {
	حالت -2:
		*writeout_error = 1;
		/*
		 * We return 0 nevertheless, as the index is all right
		 * and more importantly we have made best efforts to
		 * update paths in the work tree, and we cannot revert
		 * them.
		 */
		/* fallthrough */
	حالت 0:
		بازگشت 0;
	پیشفرض:
		بازگشت 128;
	}
}

ایستا تهی setup_branch_path(ساختار branch_info *branch)
{
	ساختار strbuf buf = STRBUF_INIT;

	/*
	 * If this is a ref, resolve it; otherwise, look up the OID for our
	 * expression.  Failure here is okay.
	 */
	اگر (!repo_dwim_ref(the_repository, branch->name, strlen(branch->name),
			   &branch->oid, &branch->refname, 0))
		repo_get_oid_committish(the_repository, branch->name, &branch->oid);

	copy_branchname(the_repository, &buf, branch->name, INTERPRET_BRANCH_LOCAL);
	اگر (strcmp(buf.buf, branch->name)) {
		آزاد(branch->name);
		branch->name = xstrdup(buf.buf);
	}
	strbuf_splice(&buf, 0, 0, "refs/heads/", 11);
	آزاد(branch->path);
	branch->path = strbuf_detach(&buf, پوچ);
}

ایستا تهی init_topts(ساختار unpack_trees_options *topts,
		       صحیح show_progress, صحیح overwrite_ignore,
		       bool quiet)
{
	memset(topts, 0, اندازه(*topts));
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

ایستا صحیح merge_working_درخت(ثابت ساختار checkout_opts *opts,
			      ساختار branch_info *old_branch_info,
			      ساختار branch_info *new_branch_info,
			      bool quiet,
			      صحیح *writeout_error)
{
	صحیح ret;
	ساختار lock_file lock_file = LOCK_INIT;
	ساختار درخت *new_درخت;

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);
	اگر (repo_read_index_preload(the_repository, پوچ, 0) < 0) {
		rollback_lock_file(&lock_file);
		بازگشت error(_("index file corrupt"));
	}

	resolve_undo_clear_index(the_repository->index);
	اگر (opts->new_orphan_branch && opts->orphan_from_empty_درخت) {
		اگر (new_branch_info->commit)
			BUG("'switch --orphan' should never accept a commit as starting point");
		new_درخت = repo_parse_درخت_indirect(the_repository,
						    the_hash_algo->empty_درخت);
		اگر (!new_درخت)
			BUG("unable to read empty tree");
	} وگرنه {
		new_درخت = repo_get_commit_درخت(the_repository,
						new_branch_info->commit);
		اگر (!new_درخت) {
			rollback_lock_file(&lock_file);
			بازگشت error(_("unable to read tree (%s)"),
				     oid_to_hex(&new_branch_info->commit->object.oid));
		}
	}
	اگر (opts->discard_changes) {
		ret = reset_درخت(new_درخت, opts, 1, writeout_error, new_branch_info);
		اگر (ret) {
			rollback_lock_file(&lock_file);
			بازگشت ret;
		}
	} وگرنه {
		ساختار درخت_desc trees[2];
		ساختار درخت *درخت;
		ساختار unpack_trees_options topts;
		ثابت ساختار object_id *old_commit_oid;

		refresh_index(the_repository->index, REFRESH_QUIET, پوچ, پوچ, پوچ);

		اگر (unmerged_index(the_repository->index)) {
			rollback_lock_file(&lock_file);
			error(_("you need to resolve your current index first"));
			بازگشت 1;
		}

		/* 2-way merge to the new branch */
		init_topts(&topts, opts->show_progress,
			   opts->overwrite_ignore, quiet);
		init_checkout_metadata(&topts.meta, new_branch_info->refname,
				       new_branch_info->commit ?
				       &new_branch_info->commit->object.oid :
				       &new_branch_info->oid, پوچ);

		old_commit_oid = old_branch_info->commit ?
			&old_branch_info->commit->object.oid :
			the_hash_algo->empty_درخت;
		درخت = repo_parse_درخت_indirect(the_repository,
						old_commit_oid);
		اگر (!درخت)
			die(_("unable to parse commit %s"),
				oid_to_hex(old_commit_oid));

		init_درخت_desc(&trees[0], &درخت->object.oid,
			       درخت->buffer, درخت->size);
		اگر (repo_parse_درخت(the_repository, new_درخت) < 0)
			die(پوچ);
		درخت = new_درخت;
		init_درخت_desc(&trees[1], &درخت->object.oid,
			       درخت->buffer, درخت->size);

		ret = unpack_trees(2, trees, &topts);
		clear_unpack_trees_porcelain(&topts);
		اگر (ret == -1) {
			rollback_lock_file(&lock_file);
			بازگشت MERGE_WORKING_درخت_UNPACK_FAILED;
		}
	}

	اگر (!cache_درخت_fully_valid(the_repository->index))
		cache_درخت_update(the_repository->index, WRITE_درخت_SILENT | WRITE_درخت_REPAIR);

	اگر (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
		die(_("unable to write new index file"));

	اگر (!opts->discard_changes && !opts->quiet && new_branch_info->commit)
		show_local_changes(&new_branch_info->commit->object, &opts->diff_options);

	بازگشت 0;
}

ایستا تهی report_tracking(ساختار branch_info *new_branch_info)
{
	ساختار strbuf sb = STRBUF_INIT;
	ساختار branch *branch = branch_get(new_branch_info->name);

	اگر (!format_tracking_info(branch, &sb, AHEAD_BEHIND_FULL, 1))
		بازگشت;
	fputs(sb.buf, stdout);
	strbuf_release(&sb);
}

ایستا تهی update_refs_for_switch(ثابت ساختار checkout_opts *opts,
				   ساختار branch_info *old_branch_info,
				   ساختار branch_info *new_branch_info)
{
	ساختار strbuf پیام = STRBUF_INIT;
	ثابت نویسه *old_desc, *reflog_پیام;
	اگر (opts->new_branch) {
		اگر (opts->new_orphan_branch) {
			شمارش log_refs_config log_all_ref_updates = LOG_REFS_UNSET;
			ثابت نویسه *مقدار;
			نویسه *refname;

			اگر (!repo_config_get_string_tmp(the_repository, "core.logallrefupdates", &مقدار))
				log_all_ref_updates = refs_parse_log_all_ref_updates_config(مقدار);

			refname = mkpathdup("refs/heads/%s", opts->new_orphan_branch);
			اگر (opts->new_branch_log &&
			    !should_autocreate_reflog(log_all_ref_updates, refname)) {
				صحیح ret;
				ساختار strbuf err = STRBUF_INIT;

				ret = refs_create_reflog(get_main_ref_store(the_repository),
							 refname, &err);
				اگر (ret) {
					fprintf(stderr, _("Can not do reflog for '%s': %s\n"),
						opts->new_orphan_branch, err.buf);
					strbuf_release(&err);
					آزاد(refname);
					بازگشت;
				}
				strbuf_release(&err);
			}
			آزاد(refname);
		}
		وگرنه
			create_branch(the_repository,
				      opts->new_branch, new_branch_info->name,
				      opts->new_branch_force ? 1 : 0,
				      opts->new_branch_force ? 1 : 0,
				      opts->new_branch_log,
				      opts->quiet,
				      opts->track,
				      0);
		آزاد(new_branch_info->name);
		آزاد(new_branch_info->refname);
		new_branch_info->name = xstrdup(opts->new_branch);
		setup_branch_path(new_branch_info);
	}

	old_desc = old_branch_info->name;
	اگر (!old_desc && old_branch_info->commit)
		old_desc = oid_to_hex(&old_branch_info->commit->object.oid);

	reflog_پیام = getenv("GIT_REFLOG_ACTION");
	اگر (!reflog_پیام)
		strbuf_addf(&پیام, "checkout: moving from %s to %s",
			old_desc ? old_desc : "(invalid)", new_branch_info->name);
	وگرنه
		strbuf_insertstr(&پیام, 0, reflog_پیام);

	اگر (!strcmp(new_branch_info->name, "HEAD") && !new_branch_info->path && !opts->force_detach) {
		/* Nothing to do. */
	} وگرنه اگر (opts->force_detach || !new_branch_info->path) {	/* No longer on any branch. */
		refs_update_ref(get_main_ref_store(the_repository), پیام.buf,
				"HEAD", &new_branch_info->commit->object.oid,
				پوچ,
				REF_NO_DEREF, UPDATE_REFS_DIE_ON_ERR);
		اگر (!opts->quiet) {
			اگر (old_branch_info->path &&
			    advice_enabled(ADVICE_DETACHED_HEAD) && !opts->force_detach)
				detach_advice(new_branch_info->name);
			describe_detached_head(_("HEAD is now at"), new_branch_info->commit);
		}
	} وگرنه اگر (new_branch_info->path) {	/* Switch branches. */
		اگر (refs_update_symref(get_main_ref_store(the_repository), "HEAD", new_branch_info->path, پیام.buf) < 0)
			die(_("unable to update HEAD"));
		اگر (!opts->quiet) {
			اگر (old_branch_info->path && !strcmp(new_branch_info->path, old_branch_info->path)) {
				اگر (opts->new_branch_force)
					fprintf(stderr, _("Reset branch '%s'\n"),
						new_branch_info->name);
				وگرنه
					fprintf(stderr, _("Already on '%s'\n"),
						new_branch_info->name);
			} وگرنه اگر (opts->new_branch) {
				اگر (opts->branch_exists)
					fprintf(stderr, _("Switched to and reset branch '%s'\n"), new_branch_info->name);
				وگرنه
					fprintf(stderr, _("Switched to a new branch '%s'\n"), new_branch_info->name);
			} وگرنه {
				fprintf(stderr, _("Switched to branch '%s'\n"),
					new_branch_info->name);
			}
		}
		اگر (old_branch_info->path && old_branch_info->name) {
			اگر (!refs_ref_exists(get_main_ref_store(the_repository), old_branch_info->path) && refs_reflog_exists(get_main_ref_store(the_repository), old_branch_info->path))
				refs_delete_reflog(get_main_ref_store(the_repository),
						   old_branch_info->path);
		}
	}
	remove_branch_state(the_repository, !opts->quiet);
	strbuf_release(&پیام);
	اگر (!opts->quiet &&
	    !opts->force_detach &&
	    (new_branch_info->path || !strcmp(new_branch_info->name, "HEAD")))
		report_tracking(new_branch_info);
}

ایستا صحیح add_pending_uninteresting_ref(ثابت ساختار reference *ref, تهی *cb_data)
{
	add_pending_oid(cb_data, ref->name, ref->oid, UNINTERESTING);
	بازگشت 0;
}

ایستا تهی describe_one_orphan(ساختار strbuf *sb, ساختار commit *commit)
{
	strbuf_addstr(sb, "  ");
	strbuf_add_unique_abbrev(sb, &commit->object.oid, DEFAULT_ABBREV);
	strbuf_addch(sb, ' ');
	اگر (!repo_parse_commit(the_repository, commit))
		pp_commit_easy(CMIT_FMT_ONELINE, commit, sb);
	strbuf_addch(sb, '\n');
}

#define ORPHAN_CUTOFF 4
ایستا تهی suggest_reattach(ساختار commit *commit, ساختار rev_info *revs)
{
	ساختار commit *c, *last = پوچ;
	ساختار strbuf sb = STRBUF_INIT;
	صحیح lost = 0;
	وقتی ((c = get_revision(revs)) != پوچ) {
		اگر (lost < ORPHAN_CUTOFF)
			describe_one_orphan(&sb, c);
		last = c;
		lost++;
	}
	اگر (ORPHAN_CUTOFF < lost) {
		صحیح more = lost - ORPHAN_CUTOFF;
		اگر (more == 1)
			describe_one_orphan(&sb, last);
		وگرنه
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

	اگر (advice_enabled(ADVICE_DETACHED_HEAD))
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
ایستا تهی orphaned_commit_warning(ساختار commit *old_commit, ساختار commit *new_commit)
{
	ساختار rev_info revs;
	ساختار object *object = &old_commit->object;

	repo_init_revisions(the_repository, &revs, پوچ);
	setup_revisions(0, پوچ, &revs, پوچ);

	object->flags &= ~UNINTERESTING;
	add_pending_object(&revs, object, oid_to_hex(&object->oid));

	refs_for_each_ref(get_main_ref_store(the_repository),
			  add_pending_uninteresting_ref, &revs);
	اگر (new_commit)
		add_pending_oid(&revs, "HEAD",
				&new_commit->object.oid,
				UNINTERESTING);

	اگر (prepare_revision_walk(&revs))
		die(_("internal error in revision walk"));
	اگر (!(old_commit->object.flags & UNINTERESTING))
		suggest_reattach(old_commit, &revs);
	وگرنه
		describe_detached_head(_("Previous HEAD position was"), old_commit);

	/* Clean up objects used, as they will be reused. */
	repo_clear_commit_marks(the_repository, ALL_REV_FLAGS);
	release_revisions(&revs);
}

ایستا صحیح switch_branches(ثابت ساختار checkout_opts *opts,
			   ساختار branch_info *new_branch_info)
{
	صحیح ret = 0;
	ساختار branch_info old_branch_info = { 0 };
	ساختار object_id rev;
	صحیح flag, writeout_error = 0;
	صحیح do_merge = 1;
	صحیح created_autostash = 0;
	شمارش stash_apply_result autostash_res = STASH_APPLY_CLEAN;
	ساختار strbuf old_commit_shortname = STRBUF_INIT;
	ساختار strbuf autostash_پیام = STRBUF_INIT;
	ثابت نویسه *stash_label_base = پوچ;

	trace2_cmd_mode("branch");

	memset(&old_branch_info, 0, اندازه(old_branch_info));
	old_branch_info.path = refs_resolve_refdup(get_main_ref_store(the_repository),
						   "HEAD", 0, &rev, &flag);
	اگر (old_branch_info.path)
		old_branch_info.commit = lookup_commit_reference_gently(the_repository, &rev, 1);
	اگر (!(flag & REF_ISSYMREF))
		FREE_AND_NULL(old_branch_info.path);

	اگر (old_branch_info.path) {
		ثابت نویسه *ثابت prefix = "refs/heads/";
		ثابت نویسه *p;
		اگر (skip_prefix(old_branch_info.path, prefix, &p))
			old_branch_info.name = xstrdup(p);
	}

	اگر (opts->new_orphan_branch && opts->orphan_from_empty_درخت) {
		اگر (new_branch_info->name)
			BUG("'switch --orphan' should never accept a commit as starting point");
		new_branch_info->commit = پوچ;
		new_branch_info->name = xstrdup("(empty)");
		do_merge = 1;
	}

	اگر (!new_branch_info->name) {
		new_branch_info->name = xstrdup("HEAD");
		new_branch_info->commit = old_branch_info.commit;
		اگر (!new_branch_info->commit)
			die(_("You are on a branch yet to be born"));
		parse_commit_or_die(new_branch_info->commit);

		اگر (opts->only_merge_on_switching_branches)
			do_merge = 0;
	}

	اگر (old_branch_info.name) {
		stash_label_base = old_branch_info.name;
	} وگرنه اگر (old_branch_info.commit) {
		strbuf_add_unique_abbrev(&old_commit_shortname,
					 &old_branch_info.commit->object.oid,
					 DEFAULT_ABBREV);
		stash_label_base = old_commit_shortname.buf;
	}

	اگر (do_merge) {
		ret = merge_working_درخت(opts, &old_branch_info, new_branch_info,
					 opts->merge, &writeout_error);
		اگر (ret == MERGE_WORKING_درخت_UNPACK_FAILED && opts->merge) {
			strbuf_addf(&autostash_پیام,
				    "autostash while switching to '%s'",
				    new_branch_info->name);
			create_autostash_ref(the_repository,
					     "CHECKOUT_AUTOSTASH_HEAD",
					     autostash_پیام.buf, true);
			created_autostash = 1;
			ret = merge_working_درخت(opts, &old_branch_info, new_branch_info,
						 false, &writeout_error);
		}
		اگر (created_autostash) {
			اگر (opts->conflict_style >= 0) {
				ساختار strbuf cfg = STRBUF_INIT;
				strbuf_addf(&cfg, "merge.conflictStyle=%s",
					    conflict_style_name(opts->conflict_style));
				git_config_افزودن_parameter(cfg.buf);
				strbuf_release(&cfg);
			}
			autostash_res = apply_autostash_ref(the_repository,
				    "CHECKOUT_AUTOSTASH_HEAD",
				    new_branch_info->name,
				    "local",
				    stash_label_base,
				    autostash_پیام.buf);
		}
		اگر (ret) {
			branch_info_release(&old_branch_info);
			strbuf_release(&old_commit_shortname);
			strbuf_release(&autostash_پیام);
			بازگشت ret < 0 ? 1 : ret;
		}
	}

	اگر (!opts->quiet && !old_branch_info.path && old_branch_info.commit && new_branch_info->commit != old_branch_info.commit)
		orphaned_commit_warning(old_branch_info.commit, new_branch_info->commit);

	اگر (autostash_res == STASH_APPLY_CONFLICT && !opts->quiet)
		fputc('\n', stderr);
	update_refs_for_switch(opts, &old_branch_info, new_branch_info);

	اگر (created_autostash) {
		discard_index(the_repository->index);
		اگر (repo_read_index(the_repository) < 0)
			die(_("index file corrupt"));

		اگر (!opts->quiet && new_branch_info->commit) {
			printf(_("The following paths have local changes:\n"));
			show_local_changes(&new_branch_info->commit->object,
					   &opts->diff_options);
		}
	}

	ret = post_checkout_hook(old_branch_info.commit, new_branch_info->commit, 1);
	branch_info_release(&old_branch_info);
	strbuf_release(&old_commit_shortname);
	strbuf_release(&autostash_پیام);

	بازگشت ret || writeout_error;
}

ایستا صحیح git_checkout_config(ثابت نویسه *var, ثابت نویسه *مقدار,
			       ثابت ساختار config_context *ctx, تهی *cb)
{
	ساختار checkout_opts *opts = cb;

	اگر (!strcmp(var, "diff.ignoresubmodules")) {
		اگر (!مقدار)
			بازگشت config_error_nonbool(var);
		handle_ignore_submodules_arg(&opts->diff_options, مقدار);
		بازگشت 0;
	}
	اگر (!strcmp(var, "checkout.guess")) {
		opts->dwim_new_local_branch = git_config_bool(var, مقدار);
		بازگشت 0;
	}

	اگر (starts_with(var, "submodule."))
		بازگشت git_default_submodule_config(var, مقدار, پوچ);

	بازگشت git_xmerge_config(var, مقدار, ctx, پوچ);
}

ایستا تهی setup_new_branch_info_and_source_درخت(
	ساختار branch_info *new_branch_info,
	ساختار checkout_opts *opts,
	ساختار object_id *rev,
	ثابت نویسه *arg)
{
	ساختار درخت **source_درخت = &opts->source_درخت;
	ساختار object_id branch_rev;

	/* treat '@' as a shortcut for 'HEAD' */
	new_branch_info->name = !strcmp(arg, "@") ? xstrdup("HEAD") :
						    xstrdup(arg);
	setup_branch_path(new_branch_info);

	اگر (!check_refname_format(new_branch_info->path, 0) &&
	    !refs_read_ref(get_main_ref_store(the_repository), new_branch_info->path, &branch_rev))
		oidcpy(rev, &branch_rev);
	وگرنه
		/* not an existing branch */
		FREE_AND_NULL(new_branch_info->path);

	new_branch_info->commit = lookup_commit_reference_gently(the_repository, rev, 1);
	اگر (!new_branch_info->commit) {
		/* not a commit */
		*source_درخت = repo_parse_درخت_indirect(the_repository, rev);
		اگر (!*source_درخت)
			die(_("unable to read tree (%s)"), oid_to_hex(rev));
	} وگرنه {
		parse_commit_or_die(new_branch_info->commit);
		*source_درخت = repo_get_commit_درخت(the_repository,
						    new_branch_info->commit);
		اگر (!*source_درخت)
			die(_("unable to read tree (%s)"),
			    oid_to_hex(&new_branch_info->commit->object.oid));
	}
}


شمارش checkout_command {
	CHECKOUT_CHECKOUT = 1,
	CHECKOUT_SWITCH = 2,
	CHECKOUT_RESTORE = 3,
};

ایستا تهی advise_disambiguating_remotes(شمارش checkout_command which_command,
					  ثابت نویسه *branch,
					  ثابت ساختار string_list *matched_remote_names)
{
	ثابت نویسه *cmdname;
	ساختار string_list_item *item;

	انتخاب (which_command) {
	حالت CHECKOUT_CHECKOUT:
		cmdname = "checkout";
		بشکن;
	حالت CHECKOUT_SWITCH:
		cmdname = "switch";
		بشکن;
	پیشفرض:
		BUG("command <%d> should not reach advise_disambiguating_remotes",
		    which_command);
		بشکن;
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

ایستا نویسه *parse_remote_branch(ثابت نویسه *arg,
				 ساختار object_id *rev,
				 صحیح could_be_checkout_paths,
				 شمارش checkout_command which_command)
{
	صحیح رقم_matches = 0;
	ساختار string_list matched_remote_names = STRING_LIST_INIT_DUP;

	نویسه *remote = unique_tracking_name(arg, rev, &رقم_matches,
					    &matched_remote_names);

	اگر (remote && could_be_checkout_paths) {
		die(_("'%s' could be both a local file and a tracking branch.\n"
			"Please use -- (and optionally --no-guess) to disambiguate"),
		    arg);
	}

	اگر (!remote && رقم_matches > 1) {
		اگر (advice_enabled(ADVICE_CHECKOUT_AMBIGUOUS_REMOTE_BRANCH_NAME))
			advise_disambiguating_remotes(which_command, arg,
						      &matched_remote_names);
		die(_("'%s' matched multiple (%d) remote tracking branches"),
		    arg, رقم_matches);
	}

	string_list_clear(&matched_remote_names, 0);

	بازگشت remote;
}

ایستا صحیح parse_branchname_arg(صحیح argc, ثابت نویسه **argv,
				صحیح dwim_new_local_branch_ok,
				شمارش checkout_command which_command,
				ساختار branch_info *new_branch_info,
				ساختار checkout_opts *opts,
				ساختار object_id *rev)
{
	ثابت نویسه **new_branch = &opts->new_branch;
	صحیح argcount = 0;
	ثابت نویسه *arg;
	نویسه *remote = پوچ;
	صحیح dash_dash_pos;
	صحیح has_dash_dash = 0;
	صحیح i;

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
	اگر (!argc)
		بازگشت 0;

	اگر (!opts->accept_pathspec) {
		اگر (argc > 1)
			die(_("only one reference expected"));
		has_dash_dash = 1; /* helps disambiguate */
	}

	arg = argv[0];
	dash_dash_pos = -1;
	برای (i = 0; i < argc; i++) {
		اگر (opts->accept_pathspec && !strcmp(argv[i], "--")) {
			dash_dash_pos = i;
			بشکن;
		}
	}
	اگر (dash_dash_pos == 0)
		بازگشت 1; /* case (2) */
	وگرنه اگر (dash_dash_pos == 1)
		has_dash_dash = 1; /* case (3) or (1) */
	وگرنه اگر (dash_dash_pos >= 2)
		die(_("only one reference expected, %d given."), dash_dash_pos);
	opts->شمار_checkout_paths = !opts->quiet && !has_dash_dash;

	اگر (!strcmp(arg, "-"))
		arg = "@{-1}";

	اگر (repo_get_oid_mb(the_repository, arg, rev)) {
		/*
		 * Either case (3) or (4), with <something> not being
		 * a commit, or an attempt to use case (1) with an
		 * invalid ref.
		 *
		 * It's likely an error, but we need to find out if
		 * we should auto-create the branch, case (3).(b).
		 */
		صحیح recover_with_dwim = dwim_new_local_branch_ok;

		صحیح could_be_checkout_paths = !has_dash_dash &&
			check_filename(opts->prefix, arg);

		اگر (!has_dash_dash && !no_wildcard(arg))
			recover_with_dwim = 0;

		/*
		 * Accept "git checkout foo", "git checkout foo --"
		 * and "git switch foo" as candidates for dwim.
		 */
		اگر (!(argc == 1 && !has_dash_dash) &&
		    !(argc == 2 && has_dash_dash) &&
		    opts->accept_pathspec)
			recover_with_dwim = 0;

		اگر (recover_with_dwim) {
			remote = parse_remote_branch(arg, rev,
						     could_be_checkout_paths,
						     which_command);
			اگر (remote) {
				*new_branch = arg;
				arg = remote;
				/* DWIMmed to create local branch, case (3).(b) */
			} وگرنه {
				recover_with_dwim = 0;
			}
		}

		اگر (!recover_with_dwim) {
			اگر (has_dash_dash)
				die(_("invalid reference: %s"), arg);
			بازگشت argcount;
		}
	}

	/* we can't end up being in (2) anymore, eat the argument */
	argcount++;
	argv++;
	argc--;

	setup_new_branch_info_and_source_درخت(new_branch_info, opts, rev, arg);

	اگر (!opts->source_درخت)                   /* case (1): want a tree */
		die(_("reference is not a tree: %s"), arg);

	اگر (!has_dash_dash) {	/* case (3).(d) -> (1) */
		/*
		 * Do not complain the most common case
		 *	git checkout branch
		 * even if there happen to be a file called 'branch';
		 * it would be extremely annoying.
		 */
		اگر (argc)
			verify_non_filename(the_repository, opts->prefix, arg);
	} وگرنه اگر (opts->accept_pathspec) {
		argcount++;
		argv++;
		argc--;
	}

	آزاد(remote);
	بازگشت argcount;
}

ایستا صحیح switch_unborn_to_new_branch(ثابت ساختار checkout_opts *opts)
{
	صحیح status;
	ساختار strbuf branch_ref = STRBUF_INIT;

	trace2_cmd_mode("unborn");

	اگر (!opts->new_branch)
		die(_("You are on a branch yet to be born"));
	strbuf_addf(&branch_ref, "refs/heads/%s", opts->new_branch);
	status = refs_update_symref(get_main_ref_store(the_repository),
				    "HEAD", branch_ref.buf, "checkout -b");
	strbuf_release(&branch_ref);
	اگر (!opts->quiet)
		fprintf(stderr, _("Switched to a new branch '%s'\n"),
			opts->new_branch);
	بازگشت status;
}

ایستا تهی die_expecting_a_branch(ثابت ساختار branch_info *branch_info)
{
	ساختار object_id oid;
	نویسه *to_free;
	صحیح code;

	اگر (repo_dwim_ref(the_repository, branch_info->name,
			  strlen(branch_info->name), &oid, &to_free, 0) == 1) {
		ثابت نویسه *ref = to_free;

		اگر (skip_prefix(ref, "refs/tags/", &ref))
			code = die_message(_("a branch is expected, got tag '%s'"), ref);
		وگرنه اگر (skip_prefix(ref, "refs/remotes/", &ref))
			code = die_message(_("a branch is expected, got remote branch '%s'"), ref);
		وگرنه
			code = die_message(_("a branch is expected, got '%s'"), ref);
	}
	وگرنه اگر (branch_info->commit)
		code = die_message(_("a branch is expected, got commit '%s'"), branch_info->name);
	وگرنه
		/*
		 * This case should never happen because we already die() on
		 * non-commit, but just in case.
		 */
		code = die_message(_("a branch is expected, got '%s'"), branch_info->name);

	اگر (advice_enabled(ADVICE_SUGGEST_DETACHING_HEAD))
		advise(_("If you want to detach HEAD at the commit, try again with the --detach option."));

	exit(code);
}

ایستا تهی die_if_some_operation_in_progress(تهی)
{
	ساختار wt_status_state state;

	memset(&state, 0, اندازه(state));
	wt_status_get_state(the_repository, &state, 0);

	اگر (state.merge_in_progress)
		die(_("cannot switch branch while merging\n"
		      "Consider \"git merge --quit\" "
		      "or \"git worktree add\"."));
	اگر (state.am_in_progress)
		die(_("cannot switch branch in the middle of an am session\n"
		      "Consider \"git am --quit\" "
		      "or \"git worktree add\"."));
	اگر (state.rebase_interactive_in_progress || state.rebase_in_progress)
		die(_("cannot switch branch while rebasing\n"
		      "Consider \"git rebase --quit\" "
		      "or \"git worktree add\"."));
	اگر (state.cherry_pick_in_progress)
		die(_("cannot switch branch while cherry-picking\n"
		      "Consider \"git cherry-pick --quit\" "
		      "or \"git worktree add\"."));
	اگر (state.revert_in_progress)
		die(_("cannot switch branch while reverting\n"
		      "Consider \"git revert --quit\" "
		      "or \"git worktree add\"."));
	اگر (state.bisect_in_progress)
		warning(_("you are switching branch while bisecting"));

	wt_status_state_free_buffers(&state);
}

/*
 * die if attempting to checkout an existing branch that is in use
 * in another worktree, unless ignore-other-wortrees option is given.
 * The check is bypassed when the branch is already the current one,
 * as it will not make things any worse.
 */
ایستا تهی die_if_switching_to_a_branch_in_use(ساختار checkout_opts *opts,
						ثابت نویسه *full_ref)
{
	صحیح flags;
	نویسه *head_ref;

	اگر (opts->ignore_other_worktrees)
		بازگشت;
	head_ref = refs_resolve_refdup(get_main_ref_store(the_repository),
				       "HEAD", 0, پوچ, &flags);
	اگر (head_ref && (!(flags & REF_ISSYMREF) || strcmp(head_ref, full_ref)))
		die_if_checked_out(full_ref, 1);
	آزاد(head_ref);
}

ایستا صحیح checkout_branch(ساختار checkout_opts *opts,
			   ساختار branch_info *new_branch_info)
{
	ساختار repo_config_مقادیر *cfg = repo_config_مقادیر(the_repository);
	صحیح noop_switch = (!new_branch_info->name &&
			   !opts->new_branch &&
			   !opts->force_detach);

	اگر (opts->pathspec.nr)
		die(_("paths cannot be used with switching branches"));

	اگر (opts->patch_mode)
		die(_("'%s' cannot be used with switching branches"),
		    "--patch");

	اگر (opts->overlay_mode != -1)
		die(_("'%s' cannot be used with switching branches"),
		    "--[no]-overlay");

	اگر (opts->writeout_stage) {
		ثابت نویسه *پیام;
		اگر (noop_switch)
			پیام = _("'%s' needs the paths to check out");
		وگرنه
			پیام = _("'%s' cannot be used with switching branches");
		die(پیام, "--ours/--theirs");
	}

	اگر (opts->force && opts->merge)
		die(_("'%s' cannot be used with '%s'"), "-f", "-m");

	اگر (opts->discard_changes && opts->merge)
		die(_("'%s' cannot be used with '%s'"), "--discard-changes", "--merge");

	اگر (opts->force_detach && opts->new_branch)
		die(_("'%s' cannot be used with '%s'"),
		    "--detach", "-b/-B/--orphan");

	اگر (opts->new_orphan_branch) {
		اگر (opts->track != BRANCH_TRACK_UNSPECIFIED)
			die(_("'%s' cannot be used with '%s'"), "--orphan", "-t");
		اگر (opts->orphan_from_empty_درخت && new_branch_info->name)
			die(_("'%s' cannot take <start-point>"), "--orphan");
	} وگرنه اگر (opts->force_detach) {
		اگر (opts->track != BRANCH_TRACK_UNSPECIFIED)
			die(_("'%s' cannot be used with '%s'"), "--detach", "-t");
	} وگرنه اگر (opts->track == BRANCH_TRACK_UNSPECIFIED)
		opts->track = cfg->branch_track;

	اگر (new_branch_info->name && !new_branch_info->commit)
		die(_("Cannot switch branch to a non-commit '%s'"),
		    new_branch_info->name);

	اگر (noop_switch &&
	    !opts->switch_branch_doing_nothing_is_ok)
		die(_("missing branch or commit argument"));

	اگر (!opts->implicit_detach &&
	    !opts->force_detach &&
	    !opts->new_branch &&
	    !opts->new_branch_force &&
	    new_branch_info->name &&
	    !new_branch_info->path)
		die_expecting_a_branch(new_branch_info);

	اگر (!opts->can_switch_when_in_progress)
		die_if_some_operation_in_progress();

	/* "git checkout <branch>" */
	اگر (new_branch_info->path && !opts->force_detach && !opts->new_branch)
		die_if_switching_to_a_branch_in_use(opts, new_branch_info->path);

	/* "git checkout -B <branch>" */
	اگر (opts->new_branch_force) {
		نویسه *full_ref = xstrfmt("refs/heads/%s", opts->new_branch);
		die_if_switching_to_a_branch_in_use(opts, full_ref);
		آزاد(full_ref);
	}

	اگر (!new_branch_info->commit && opts->new_branch) {
		ساختار object_id rev;
		صحیح flag;

		اگر (!refs_read_ref_full(get_main_ref_store(the_repository), "HEAD", 0, &rev, &flag) &&
		    (flag & REF_ISSYMREF) && is_null_oid(&rev))
			بازگشت switch_unborn_to_new_branch(opts);
	}
	بازگشت switch_branches(opts, new_branch_info);
}

ایستا صحیح parse_opt_conflict(ثابت ساختار option *o, ثابت نویسه *arg, صحیح unset)
{
	ساختار checkout_opts *opts = o->مقدار;

	اگر (unset) {
		opts->conflict_style = -1;
		بازگشت 0;
	}
	opts->conflict_style = parse_conflict_style_name(arg);
	اگر (opts->conflict_style < 0)
		بازگشت error(_("unknown conflict style '%s'"), arg);
	/* --conflict overrides a previous --no-merge */
	اگر (!opts->merge)
		opts->merge = -1;

	بازگشت 0;
}

ایستا ساختار option *add_common_options(ساختار checkout_opts *opts,
					 ساختار option *prevopts)
{
	ساختار option options[] = {
		OPT__QUIET(&opts->quiet, N_("suppress progress reporting")),
		OPT_CALLBACK_F(0, "recurse-submodules", پوچ,
			    "checkout", "control recursive updating of submodules",
			    PARSE_OPT_OPTARG, option_parse_recurse_submodules_worktree_updater),
		OPT_BOOL(0, "progress", &opts->show_progress, N_("force progress reporting")),
		OPT_BOOL('m', "merge", &opts->merge, N_("perform a 3-way merge with the new branch")),
		OPT_CALLBACK(0, "conflict", opts, N_("style"),
			     N_("conflict style (merge, diff3, or zdiff3)"),
			     parse_opt_conflict),
		OPT_END()
	};
	ساختار option *newopts = parse_options_concat(prevopts, options);
	آزاد(prevopts);
	بازگشت newopts;
}

ایستا ساختار option *add_common_switch_branch_options(
	ساختار checkout_opts *opts, ساختار option *prevopts)
{
	ساختار option options[] = {
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
	ساختار option *newopts = parse_options_concat(prevopts, options);
	آزاد(prevopts);
	بازگشت newopts;
}

ایستا ساختار option *add_checkout_path_options(ساختار checkout_opts *opts,
						ساختار option *prevopts)
{
	ساختار option options[] = {
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
	ساختار option *newopts = parse_options_concat(prevopts, options);
	آزاد(prevopts);
	بازگشت newopts;
}

/* create-branch option (either b or c) */
ایستا نویسه cb_option = 'b';

ایستا صحیح checkout_main(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
			 ساختار checkout_opts *opts, ساختار option *options,
			 شمارش checkout_command which_command)
{
	صحیح parseopt_flags = 0;
	ساختار branch_info new_branch_info = { 0 };
	صحیح ret;

	ایستا ثابت نویسه * ثابت checkout_usage[] = {
		N_("git checkout [<options>] <branch>"),
		N_("git checkout [<options>] [<branch>] -- <file>..."),
		پوچ,
	};

	ایستا ثابت نویسه * ثابت switch_branch_usage[] = {
		N_("git switch [<options>] [<branch>]"),
		پوچ,
	};

	ایستا ثابت نویسه * ثابت restore_usage[] = {
		N_("git restore [<options>] [--source=<branch>] <file>..."),
		پوچ,
	};

	ثابت نویسه * ثابت *usagestr;

	انتخاب (which_command) {
	حالت CHECKOUT_CHECKOUT:
		usagestr = checkout_usage;
		بشکن;
	حالت CHECKOUT_SWITCH:
		usagestr = switch_branch_usage;
		بشکن;
	حالت CHECKOUT_RESTORE:
		usagestr = restore_usage;
		بشکن;
	پیشفرض:
		BUG("no such checkout variant %d", which_command);
	}

	opts->overwrite_ignore = 1;
	opts->prefix = prefix;
	opts->show_progress = -1;

	repo_config(the_repository, git_checkout_config, opts);
	اگر (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_full_index = 0;
	}

	opts->track = BRANCH_TRACK_UNSPECIFIED;

	اگر (!opts->accept_pathspec && !opts->accept_ref)
		BUG("make up your mind, you need to take _something_");
	اگر (opts->accept_pathspec && opts->accept_ref)
		parseopt_flags = PARSE_OPT_KEEP_DASHDASH;

	argc = parse_options(argc, argv, prefix, options,
			     usagestr, parseopt_flags);

	اگر (opts->patch_context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	اگر (opts->patch_interhunk_context < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	اگر (!opts->patch_mode) {
		اگر (opts->patch_context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--patch");
		اگر (opts->patch_interhunk_context != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--patch");
		اگر (!opts->auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--patch");
	}

	اگر (opts->show_progress < 0) {
		اگر (opts->quiet)
			opts->show_progress = 0;
		وگرنه
			opts->show_progress = isatty(2);
	}

	/* --conflicts implies --merge */
	اگر (opts->merge == -1)
		opts->merge = opts->conflict_style >= 0;

	اگر (opts->force) {
		opts->discard_changes = 1;
		opts->ignore_unmerged_opt = "--force";
		opts->ignore_unmerged = 1;
	}

	اگر ((!!opts->new_branch + !!opts->new_branch_force + !!opts->new_orphan_branch) > 1)
		die(_("options '-%c', '-%c', and '%s' cannot be used together"),
			cb_option, toupper(cb_option), "--orphan");

	اگر (opts->overlay_mode == 1 && opts->patch_mode)
		die(_("options '%s' and '%s' cannot be used together"), "-p", "--overlay");

	اگر (opts->checkout_index >= 0 || opts->checkout_worktree >= 0) {
		اگر (opts->checkout_index < 0)
			opts->checkout_index = 0;
		اگر (opts->checkout_worktree < 0)
			opts->checkout_worktree = 0;
	} وگرنه {
		اگر (opts->checkout_index < 0)
			opts->checkout_index = -opts->checkout_index - 1;
		اگر (opts->checkout_worktree < 0)
			opts->checkout_worktree = -opts->checkout_worktree - 1;
	}
	اگر (opts->checkout_index < 0 || opts->checkout_worktree < 0)
		BUG("these flags should be non-negative by now");
	/*
	 * convenient shortcut: "git restore --staged [--worktree]" equals
	 * "git restore --staged [--worktree] --source HEAD"
	 */
	اگر (!opts->from_treeish && opts->checkout_index)
		opts->from_treeish = "HEAD";

	/*
	 * From here on, new_branch will contain the branch to be checked out,
	 * and new_branch_force and new_orphan_branch will tell us which one of
	 * -b/-B/-c/-C/--orphan is being used.
	 */
	اگر (opts->new_branch_force)
		opts->new_branch = opts->new_branch_force;

	اگر (opts->new_orphan_branch)
		opts->new_branch = opts->new_orphan_branch;

	/* --track without -c/-C/-b/-B/--orphan should DWIM */
	اگر (opts->track != BRANCH_TRACK_UNSPECIFIED && !opts->new_branch) {
		ثابت نویسه *argv0 = argv[0];
		اگر (!argc || !strcmp(argv0, "--"))
			die(_("--track needs a branch name"));
		skip_prefix(argv0, "refs/", &argv0);
		skip_prefix(argv0, "remotes/", &argv0);
		argv0 = strchr(argv0, '/');
		اگر (!argv0 || !argv0[1])
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
	اگر (argc && opts->accept_ref) {
		ساختار object_id rev;
		صحیح dwim_ok =
			!opts->patch_mode &&
			opts->dwim_new_local_branch &&
			opts->track == BRANCH_TRACK_UNSPECIFIED &&
			!opts->new_branch;
		صحیح n = parse_branchname_arg(argc, argv, dwim_ok, which_command,
					     &new_branch_info, opts, &rev);
		argv += n;
		argc -= n;
	} وگرنه اگر (!opts->accept_ref && opts->from_treeish) {
		ساختار object_id rev;

		اگر (repo_get_oid_mb(the_repository, opts->from_treeish, &rev))
			die(_("could not resolve '%s'"), opts->from_treeish);

		setup_new_branch_info_and_source_درخت(&new_branch_info,
						      opts, &rev,
						      opts->from_treeish);

		اگر (!opts->source_درخت)
			die(_("reference is not a tree: %s"), opts->from_treeish);
	}

	اگر (argc) {
		parse_pathspec(&opts->pathspec, 0,
			       opts->patch_mode ? PATHSPEC_PREFIX_ORIGIN : 0,
			       prefix, argv);

		اگر (!opts->pathspec.nr)
			die(_("invalid path specification"));

		/*
		 * Try to give more helpful suggestion.
		 * new_branch && argc > 1 will be caught later.
		 */
		اگر (opts->new_branch && argc == 1 && !new_branch_info.commit)
			die(_("'%s' is not a commit and a branch '%s' cannot be created from it"),
				argv[0], opts->new_branch);

		اگر (opts->force_detach)
			die(_("git checkout: --detach does not take a path argument '%s'"),
			    argv[0]);
	}

	اگر (opts->pathspec_from_file) {
		اگر (opts->pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		اگر (opts->force_detach)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--detach");

		اگر (opts->patch_mode)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--patch");

		parse_pathspec_file(&opts->pathspec, 0,
				    0,
				    prefix, opts->pathspec_from_file, opts->pathspec_file_nul);
	} وگرنه اگر (opts->pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	opts->pathspec.recursive = 1;

	اگر (opts->pathspec.nr) {
		اگر (1 < !!opts->writeout_stage + !!opts->force + !!opts->merge)
			die(_("git checkout: --ours/--theirs, --force and --merge are incompatible when\n"
			      "checking out of the index."));
	} وگرنه {
		اگر (opts->accept_pathspec && !opts->empty_pathspec_ok &&
		    !opts->patch_mode)	/* patch mode is special */
			die(_("you must specify path(s) to restore"));
	}

	اگر (opts->new_branch) {
		ساختار strbuf buf = STRBUF_INIT;

		اگر (opts->new_branch_force)
			opts->branch_exists = validate_branchname(opts->new_branch, &buf);
		وگرنه
			opts->branch_exists =
				validate_new_branchname(opts->new_branch, &buf, 0);
		strbuf_release(&buf);
	}

	اگر (opts->patch_mode || opts->pathspec.nr)
		ret = checkout_paths(opts, &new_branch_info);
	وگرنه
		ret = checkout_branch(opts, &new_branch_info);

	branch_info_release(&new_branch_info);
	clear_pathspec(&opts->pathspec);
	آزاد(opts->pathspec_from_file);
	آزاد(options);

	بازگشت ret;
}

صحیح cmd_checkout(صحیح argc,
		 ثابت نویسه **argv,
		 ثابت نویسه *prefix,
		 ساختار repository *repo UNUSED)
{
	ساختار checkout_opts opts = CHECKOUT_OPTS_INIT;
	ساختار option *options;
	ساختار option checkout_options[] = {
		OPT_STRING('b', پوچ, &opts.new_branch, N_("branch"),
			   N_("create and checkout a new branch")),
		OPT_STRING('B', پوچ, &opts.new_branch_force, N_("branch"),
			   N_("create/reset and checkout a branch")),
		OPT_BOOL('l', پوچ, &opts.new_branch_log, N_("create reflog for new branch")),
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
	opts.orphan_from_empty_درخت = 0;
	opts.empty_pathspec_ok = 1;
	opts.overlay_mode = -1;
	opts.checkout_index = -2;    /* default on */
	opts.checkout_worktree = -2; /* default on */

	اگر (argc == 3 && !strcmp(argv[1], "-b")) {
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

	بازگشت checkout_main(argc, argv, prefix, &opts, options,
			     CHECKOUT_CHECKOUT);
}

صحیح cmd_switch(صحیح argc,
	       ثابت نویسه **argv,
	       ثابت نویسه *prefix,
	       ساختار repository *repo UNUSED)
{
	ساختار checkout_opts opts = CHECKOUT_OPTS_INIT;
	ساختار option *options = پوچ;
	ساختار option switch_options[] = {
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
	opts.orphan_from_empty_درخت = 1;
	opts.overlay_mode = -1;

	options = parse_options_dup(switch_options);
	options = add_common_options(&opts, options);
	options = add_common_switch_branch_options(&opts, options);

	cb_option = 'c';

	بازگشت checkout_main(argc, argv, prefix, &opts, options,
			     CHECKOUT_SWITCH);
}

صحیح cmd_restore(صحیح argc,
		ثابت نویسه **argv,
		ثابت نویسه *prefix,
		ساختار repository *repo UNUSED)
{
	ساختار checkout_opts opts = CHECKOUT_OPTS_INIT;
	ساختار option *options;
	ساختار option restore_options[] = {
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

	بازگشت checkout_main(argc, argv, prefix, &opts, options,
			     CHECKOUT_RESTORE);
}
