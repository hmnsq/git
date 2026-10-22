#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "advice.h"
#அந்தர்பாவ "branch.h"
#அந்தர்பாவ "cache-tree.h"
#அந்தர்பாவ "checkout.h"
#அந்தர்பாவ "commit.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "diff.h"
#அந்தர்பாவ "dir.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "hex.h"
#அந்தர்பாவ "hook.h"
#அந்தர்பாவ "merge-ll.h"
#அந்தர்பாவ "lockfile.h"
#அந்தர்பாவ "mem-pool.h"
#அந்தர்பாவ "object-file.h"
#அந்தர்பாவ "object-name.h"
#அந்தர்பாவ "odb.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "path.h"
#அந்தர்பாவ "preload-index.h"
#அந்தர்பாவ "read-cache.h"
#அந்தர்பாவ "refs.h"
#அந்தர்பாவ "remote.h"
#அந்தர்பாவ "repo-settings.h"
#அந்தர்பாவ "resolve-undo.h"
#அந்தர்பாவ "revision.h"
#அந்தர்பாவ "sequencer.h"
#அந்தர்பாவ "setup.h"
#அந்தர்பாவ "sparse-index.h"
#அந்தர்பாவ "strvec.h"
#அந்தர்பாவ "submodule.h"
#அந்தர்பாவ "symlinks.h"
#அந்தர்பாவ "trace2.h"
#அந்தர்பாவ "tree.h"
#அந்தர்பாவ "tree-walk.h"
#அந்தர்பாவ "unpack-trees.h"
#அந்தர்பாவ "wt-status.h"
#அந்தர்பாவ "xdiff-interface.h"
#அந்தர்பாவ "entry.h"
#அந்தர்பாவ "parallel-checkout.h"
#அந்தர்பாவ "add-interactive.h"

கட்டமைப்பு checkout_opts {
	முழு patch_mode;
	முழு patch_context;
	முழு patch_interhunk_context;
	முழு auto_advance;
	முழு quiet;
	முழு merge;
	முழு force;
	முழு force_detach;
	முழு implicit_detach;
	முழு writeout_stage;
	முழு overwrite_ignore;
	முழு ignore_skipworktree;
	முழு ignore_other_worktrees;
	முழு show_progress;
	முழு மொத்தம்_checkout_paths;
	முழு overlay_mode;
	முழு dwim_new_local_branch;
	முழு discard_changes;
	முழு accept_ref;
	முழு accept_pathspec;
	முழு switch_branch_doing_nothing_is_ok;
	முழு only_merge_on_switching_branches;
	முழு can_switch_when_in_progress;
	முழு orphan_from_empty_மரம்;
	முழு empty_pathspec_ok;
	முழு checkout_index;
	முழு checkout_worktree;
	மாறிலி எழுத்து *ignore_unmerged_opt;
	முழு ignore_unmerged;
	முழு pathspec_file_nul;
	எழுத்து *pathspec_from_file;

	மாறிலி எழுத்து *new_branch;
	மாறிலி எழுத்து *new_branch_force;
	மாறிலி எழுத்து *new_orphan_branch;
	முழு new_branch_log;
	எண்ணிக்கை branch_track track;
	கட்டமைப்பு diff_options diff_options;
	முழு conflict_style;

	முழு branch_exists;
	மாறிலி எழுத்து *prefix;
	கட்டமைப்பு pathspec pathspec;
	மாறிலி எழுத்து *from_treeish;
	கட்டமைப்பு மரம் *source_மரம்;
};

#define CHECKOUT_OPTS_INIT { \
	.conflict_style = -1, \
	.merge = -1, \
	.patch_context = -1, \
	.patch_interhunk_context = -1, \
	.auto_advance = 1, \
}

#define MERGE_WORKING_மரம்_UNPACK_FAILED (-2)

கட்டமைப்பு branch_info {
	எழுத்து *name; /* The short name used */
	எழுத்து *path; /* The full name of a real branch */
	கட்டமைப்பு commit *commit; /* The named commit */
	எழுத்து *refname; /* The full name of the ref being checked out. */
	கட்டமைப்பு object_id oid; /* The object ID of the commit being checked out. */
	/*
	 * if not null the branch is detached because it's already
	 * checked out in this checkout
	 */
	எழுத்து *checkout;
};

நிலையான வெறுமை branch_info_release(கட்டமைப்பு branch_info *info)
{
	விடுவி(info->name);
	விடுவி(info->path);
	விடுவி(info->refname);
	விடுவி(info->checkout);
}

நிலையான முழு post_checkout_hook(கட்டமைப்பு commit *old_commit, கட்டமைப்பு commit *new_commit,
			      முழு changed)
{
	கட்டமைப்பு run_hooks_opt opt = RUN_HOOKS_OPT_INIT_FORCE_SERIAL;

	/*
	 * "new_commit" can be NULL when checking out from the index before
	 * a commit exists.
	 */
	strvec_pushl(&opt.args,
		     oid_to_hex(old_commit ? &old_commit->object.oid : null_oid(the_hash_algo)),
		     oid_to_hex(new_commit ? &new_commit->object.oid : null_oid(the_hash_algo)),
		     changed ? "1" : "0",
		     வெற்று);

	திரும்பு run_hooks_opt(the_repository, "post-checkout", &opt);
}

/*
 * Handle a tree object and determine if we need to recurse into the
 * tree (READ_TREE_RECURSIVE) or skip it (0).
 */
நிலையான முழு try_update_sparse_directory(மாறிலி கட்டமைப்பு object_id *oid,
				       கட்டமைப்பு strbuf *base,
				       மாறிலி எழுத்து *pathname,
				       முழு overlay_mode)
{
	கட்டமைப்பு strbuf dirpath = STRBUF_INIT;
	கட்டமைப்பு cache_entry *old;
	முழு pos, result = READ_மரம்_RECURSIVE;

	எனில் (!the_repository->index->sparse_index)
		திரும்பு result;

	strbuf_addbuf(&dirpath, base);
	strbuf_addstr(&dirpath, pathname);
	strbuf_addch(&dirpath, '/');

	pos = index_name_pos_sparse(the_repository->index,
				    dirpath.buf, dirpath.len);
	எனில் (pos < 0)
		செல் cleanup;

	old = the_repository->index->cache[pos];
	எனில் (!S_ISSPARSEDIR(old->ce_mode))
		செல் cleanup;

	எனில் (oideq(oid, &old->oid)) {
		/* Tree content already matches; no need to descend. */
		result = 0;
	} இல்லையெனில் எனில் (!overlay_mode) {
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
	திரும்பு result;
}

நிலையான முழு update_some(மாறிலி கட்டமைப்பு object_id *oid, கட்டமைப்பு strbuf *base,
		       மாறிலி எழுத்து *pathname, unsigned mode, வெறுமை *context)
{
	முழு len;
	கட்டமைப்பு cache_entry *ce;
	முழு pos;
	முழு overlay_mode = context ? *((முழு *)context) : 1;

	எனில் (S_ISDIR(mode))
		திரும்பு try_update_sparse_directory(oid, base, pathname,
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
	எனில் (pos >= 0) {
		கட்டமைப்பு cache_entry *old = the_repository->index->cache[pos];
		எனில் (ce->ce_mode == old->ce_mode &&
		    !ce_intent_to_add(old) &&
		    oideq(&ce->oid, &old->oid)) {
			old->ce_flags |= CE_UPDATE;
			discard_cache_entry(ce);
			திரும்பு 0;
		}
	}

	add_index_entry(the_repository->index, ce,
			ADD_CACHE_OK_TO_ADD | ADD_CACHE_OK_TO_REPLACE);
	திரும்பு 0;
}

நிலையான முழு read_மரம்_some(கட்டமைப்பு மரம் *மரம், மாறிலி கட்டமைப்பு pathspec *pathspec,
			  முழு overlay_mode)
{
	read_மரம்(the_repository, மரம்,
		  pathspec, update_some, &overlay_mode);

	/* update the index with the given tree's info
	 * for all args, expanding wildcards, and exit
	 * with any non-zero return code.
	 */
	திரும்பு 0;
}

நிலையான முழு skip_same_name(மாறிலி கட்டமைப்பு cache_entry *ce, முழு pos)
{
	போது (++pos < the_repository->index->cache_nr &&
	       !strcmp(the_repository->index->cache[pos]->name, ce->name))
		; /* skip */
	திரும்பு pos;
}

நிலையான முழு check_stage(முழு stage, மாறிலி கட்டமைப்பு cache_entry *ce, முழு pos,
		       முழு overlay_mode)
{
	போது (pos < the_repository->index->cache_nr &&
	       !strcmp(the_repository->index->cache[pos]->name, ce->name)) {
		எனில் (ce_stage(the_repository->index->cache[pos]) == stage)
			திரும்பு 0;
		pos++;
	}
	எனில் (!overlay_mode)
		திரும்பு 0;
	எனில் (stage == 2)
		திரும்பு error(_("path '%s' does not have our version"), ce->name);
	இல்லையெனில்
		திரும்பு error(_("path '%s' does not have their version"), ce->name);
}

நிலையான முழு check_stages(unsigned stages, மாறிலி கட்டமைப்பு cache_entry *ce, முழு pos)
{
	unsigned seen = 0;
	மாறிலி எழுத்து *name = ce->name;

	போது (pos < the_repository->index->cache_nr) {
		ce = the_repository->index->cache[pos];
		எனில் (strcmp(name, ce->name))
			நிறுத்து;
		seen |= (1 << ce_stage(ce));
		pos++;
	}
	எனில் ((stages & seen) != stages)
		திரும்பு error(_("path '%s' does not have all necessary versions"),
			     name);
	திரும்பு 0;
}

நிலையான முழு checkout_stage(முழு stage, மாறிலி கட்டமைப்பு cache_entry *ce, முழு pos,
			  மாறிலி கட்டமைப்பு checkout *state, முழு *nr_checkouts,
			  முழு overlay_mode)
{
	போது (pos < the_repository->index->cache_nr &&
	       !strcmp(the_repository->index->cache[pos]->name, ce->name)) {
		எனில் (ce_stage(the_repository->index->cache[pos]) == stage)
			திரும்பு checkout_entry(the_repository->index->cache[pos], state,
					      வெற்று, nr_checkouts);
		pos++;
	}
	எனில் (!overlay_mode) {
		unlink_entry(ce, வெற்று);
		திரும்பு 0;
	}
	எனில் (stage == 2)
		திரும்பு error(_("path '%s' does not have our version"), ce->name);
	இல்லையெனில்
		திரும்பு error(_("path '%s' does not have their version"), ce->name);
}

நிலையான முழு checkout_merged(முழு pos, மாறிலி கட்டமைப்பு checkout *state,
			   முழு *nr_checkouts, கட்டமைப்பு mem_pool *ce_mem_pool,
			   முழு conflict_style)
{
	கட்டமைப்பு cache_entry *ce = the_repository->index->cache[pos];
	மாறிலி எழுத்து *path = ce->name;
	mmfile_t ancestor, ours, theirs;
	எண்ணிக்கை ll_merge_result merge_status;
	முழு status;
	கட்டமைப்பு object_id oid;
	mmfile_t result_buf;
	கட்டமைப்பு object_id threeway[3];
	unsigned mode = 0;
	கட்டமைப்பு ll_merge_options ll_opts = LL_MERGE_OPTIONS_INIT;
	முழு renormalize = 0;

	memset(threeway, 0, அளவு(threeway));
	போது (pos < the_repository->index->cache_nr) {
		முழு stage;
		stage = ce_stage(ce);
		எனில் (!stage || strcmp(path, ce->name))
			நிறுத்து;
		oidcpy(&threeway[stage - 1], &ce->oid);
		எனில் (stage == 2)
			mode = create_ce_mode(ce->ce_mode);
		pos++;
		ce = the_repository->index->cache[pos];
	}
	எனில் (is_null_oid(&threeway[1]) || is_null_oid(&threeway[2]))
		திரும்பு error(_("path '%s' does not have necessary versions"), path);

	read_mmblob(&ancestor, the_repository->objects, &threeway[0]);
	read_mmblob(&ours, the_repository->objects, &threeway[1]);
	read_mmblob(&theirs, the_repository->objects, &threeway[2]);

	repo_config_get_bool(the_repository, "merge.renormalize", &renormalize);
	ll_opts.renormalize = renormalize;
	ll_opts.conflict_style = conflict_style;
	merge_status = ll_merge(&result_buf, path, &ancestor, "base",
				&ours, "ours", &theirs, "theirs",
				state->istate, &ll_opts);
	விடுவி(ancestor.ptr);
	விடுவி(ours.ptr);
	விடுவி(theirs.ptr);
	எனில் (merge_status == LL_MERGE_இருமம்_CONFLICT)
		warning("Cannot merge binary files: %s (%s vs. %s)",
			path, "ours", "theirs");
	எனில் (merge_status < 0 || !result_buf.ptr) {
		விடுவி(result_buf.ptr);
		திரும்பு error(_("path '%s': cannot merge"), path);
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
	எனில் (odb_write_object(the_repository->objects, result_buf.ptr, result_buf.size, OBJ_BLOB, &oid))
		die(_("Unable to add merge result for '%s'"), path);
	விடுவி(result_buf.ptr);
	ce = make_transient_cache_entry(mode, &oid, path, 2, ce_mem_pool);
	எனில் (!ce)
		die(_("make_cache_entry failed for path '%s'"), path);
	status = checkout_entry(ce, state, வெற்று, nr_checkouts);
	திரும்பு status;
}

நிலையான வெறுமை mark_ce_for_checkout_overlay(கட்டமைப்பு cache_entry *ce,
					 எழுத்து *ps_matched,
					 மாறிலி கட்டமைப்பு checkout_opts *opts)
{
	ce->ce_flags &= ~CE_MATCHED;
	எனில் (!opts->ignore_skipworktree && ce_skip_worktree(ce))
		திரும்பு;
	எனில் (opts->source_மரம் && !(ce->ce_flags & CE_UPDATE))
		/*
		 * "git checkout tree-ish -- path", but this entry
		 * is in the original index but is not in tree-ish
		 * or does not match the pathspec; it will not be
		 * checked out to the working tree.  We will not do
		 * anything to this entry at all.
		 */
		திரும்பு;
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
	எனில் (ce_path_match(the_repository->index, ce, &opts->pathspec, ps_matched))
		ce->ce_flags |= CE_MATCHED;
}

நிலையான வெறுமை mark_ce_for_checkout_no_overlay(கட்டமைப்பு cache_entry *ce,
					    எழுத்து *ps_matched,
					    மாறிலி கட்டமைப்பு checkout_opts *opts)
{
	ce->ce_flags &= ~CE_MATCHED;
	எனில் (!opts->ignore_skipworktree && ce_skip_worktree(ce))
		திரும்பு;
	எனில் (ce_path_match(the_repository->index, ce, &opts->pathspec, ps_matched)) {
		ce->ce_flags |= CE_MATCHED;
		எனில் (opts->source_மரம் && !(ce->ce_flags & CE_UPDATE))
			/*
			 * In overlay mode, but the path is not in
			 * tree-ish, which means we should remove it
			 * from the index and the working tree.
			 */
			ce->ce_flags |= CE_REMOVE | CE_WT_REMOVE;
	}
}

நிலையான முழு checkout_worktree(மாறிலி கட்டமைப்பு checkout_opts *opts,
			     மாறிலி கட்டமைப்பு branch_info *info)
{
	கட்டமைப்பு checkout state = CHECKOUT_INIT;
	முழு nr_checkouts = 0, nr_unmerged = 0;
	முழு errs = 0;
	முழு pos;
	முழு pc_workers, pc_threshold;
	கட்டமைப்பு mem_pool ce_mem_pool;

	state.force = 1;
	state.refresh_cache = 1;
	state.istate = the_repository->index;

	mem_pool_init(&ce_mem_pool, 0);
	get_parallel_checkout_configs(&pc_workers, &pc_threshold);
	init_checkout_metadata(&state.meta, info->refname,
			       info->commit ? &info->commit->object.oid : &info->oid,
			       வெற்று);

	enable_delayed_checkout(&state);

	எனில் (pc_workers > 1)
		init_parallel_checkout();

	க்கு (pos = 0; pos < the_repository->index->cache_nr; pos++) {
		கட்டமைப்பு cache_entry *ce = the_repository->index->cache[pos];
		எனில் (ce->ce_flags & CE_MATCHED) {
			எனில் (!ce_stage(ce)) {
				errs |= checkout_entry(ce, &state,
						       வெற்று, &nr_checkouts);
				தொடர்;
			}
			எனில் (opts->writeout_stage)
				errs |= checkout_stage(opts->writeout_stage,
						       ce, pos,
						       &state,
						       &nr_checkouts, opts->overlay_mode);
			இல்லையெனில் எனில் (opts->merge)
				errs |= checkout_merged(pos, &state,
							&nr_unmerged,
							&ce_mem_pool,
							opts->conflict_style);
			pos = skip_same_name(ce, pos) - 1;
		}
	}
	எனில் (pc_workers > 1)
		errs |= run_parallel_checkout(&state, pc_workers, pc_threshold,
					      வெற்று, வெற்று);
	mem_pool_discard(&ce_mem_pool, should_validate_cache_entries());
	remove_marked_cache_entries(the_repository->index, 1);
	remove_scheduled_dirs();
	errs |= finish_delayed_checkout(&state, opts->show_progress);

	எனில் (opts->மொத்தம்_checkout_paths) {
		எனில் (nr_unmerged)
			fprintf_ln(stderr, Q_("Recreated %d merge conflict",
					      "Recreated %d merge conflicts",
					      nr_unmerged),
				   nr_unmerged);
		எனில் (opts->source_மரம்)
			fprintf_ln(stderr, Q_("Updated %d path from %s",
					      "Updated %d paths from %s",
					      nr_checkouts),
				   nr_checkouts,
				   repo_find_unique_abbrev(the_repository, &opts->source_மரம்->object.oid,
							   DEFAULT_ABBREV));
		இல்லையெனில் எனில் (!nr_unmerged || nr_checkouts)
			fprintf_ln(stderr, Q_("Updated %d path from the index",
					      "Updated %d paths from the index",
					      nr_checkouts),
				   nr_checkouts);
	}

	திரும்பு errs;
}

நிலையான முழு checkout_paths(மாறிலி கட்டமைப்பு checkout_opts *opts,
			  மாறிலி கட்டமைப்பு branch_info *new_branch_info)
{
	முழு pos;
	நிலையான எழுத்து *ps_matched;
	கட்டமைப்பு object_id rev;
	கட்டமைப்பு commit *head;
	முழு errs = 0;
	கட்டமைப்பு lock_file lock_file = LOCK_INIT;
	முழு checkout_index;

	trace2_cmd_mode(opts->patch_mode ? "patch" : "path");

	எனில் (opts->track != BRANCH_TRACK_UNSPECIFIED)
		die(_("'%s' cannot be used with updating paths"), "--track");

	எனில் (opts->new_branch_log)
		die(_("'%s' cannot be used with updating paths"), "-l");

	எனில் (opts->ignore_unmerged && opts->patch_mode)
		die(_("'%s' cannot be used with updating paths"),
		    opts->ignore_unmerged_opt);

	எனில் (opts->force_detach)
		die(_("'%s' cannot be used with updating paths"), "--detach");

	எனில் (opts->merge && opts->patch_mode)
		die(_("options '%s' and '%s' cannot be used together"), "--merge", "--patch");

	எனில் (opts->ignore_unmerged && opts->merge)
		die(_("options '%s' and '%s' cannot be used together"),
		    opts->ignore_unmerged_opt, "-m");

	எனில் (opts->new_branch)
		die(_("Cannot update paths and switch to branch '%s' at the same time."),
		    opts->new_branch);

	எனில் (!opts->checkout_worktree && !opts->checkout_index)
		die(_("neither '%s' or '%s' is specified"),
		    "--staged", "--worktree");

	எனில் (!opts->checkout_worktree && !opts->from_treeish)
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
	எனில் (!opts->accept_ref && opts->checkout_index) {
		எனில் (opts->writeout_stage)
			die(_("'%s' or '%s' cannot be used with %s"),
			    "--ours", "--theirs", "--staged");

		எனில் (opts->merge)
			die(_("'%s' or '%s' cannot be used with %s"),
			    "--merge", "--conflict", "--staged");
	}

	/*
	 * recreating unmerged index entries and writing out data from
	 * unmerged index entries would make no sense when checking out
	 * of a tree-ish.
	 */
	எனில் ((opts->merge || opts->writeout_stage) && opts->source_மரம்)
		die(_("'%s', '%s', or '%s' cannot be used when checking out of a tree"),
		    "--merge", "--ours", "--theirs");

	எனில் (opts->patch_mode) {
		எண்ணிக்கை add_p_mode patch_mode;
		கட்டமைப்பு interactive_options interactive_opts = {
			.context = opts->patch_context,
			.interhunkcontext = opts->patch_interhunk_context,
			.auto_advance = opts->auto_advance
		};
		மாறிலி எழுத்து *rev = new_branch_info->name;
		எழுத்து rev_oid[GIT_MAX_HEXSZ + 1];

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
		எனில் (rev && new_branch_info->commit && strcmp(rev, "HEAD"))
			rev = oid_to_hex_r(rev_oid, &new_branch_info->commit->object.oid);

		எனில் (opts->checkout_index && opts->checkout_worktree)
			patch_mode = ADD_P_CHECKOUT;
		இல்லையெனில் எனில் (opts->checkout_index && !opts->checkout_worktree)
			patch_mode = ADD_P_RESET;
		இல்லையெனில் எனில் (!opts->checkout_index && opts->checkout_worktree)
			patch_mode = ADD_P_WORKTREE;
		இல்லையெனில்
			BUG("either flag must have been set, worktree=%d, index=%d",
			    opts->checkout_worktree, opts->checkout_index);
		திரும்பு !!run_add_p(the_repository, patch_mode, &interactive_opts,
				   rev, &opts->pathspec, 0);
	}

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);
	எனில் (repo_read_index_preload(the_repository, &opts->pathspec, 0) < 0)
		திரும்பு error(_("index file corrupt"));

	எனில் (opts->source_மரம்)
		read_மரம்_some(opts->source_மரம், &opts->pathspec,
			       opts->overlay_mode);
	எனில் (opts->merge)
		unmerge_index(the_repository->index, &opts->pathspec, CE_MATCHED);

	ps_matched = xcalloc(opts->pathspec.nr, 1);

	/*
	 * Make sure all pathspecs participated in locating the paths
	 * to be checked out.
	 */
	க்கு (pos = 0; pos < the_repository->index->cache_nr; pos++)
		எனில் (opts->overlay_mode)
			mark_ce_for_checkout_overlay(the_repository->index->cache[pos],
						     ps_matched,
						     opts);
		இல்லையெனில்
			mark_ce_for_checkout_no_overlay(the_repository->index->cache[pos],
							ps_matched,
							opts);

	எனில் (report_path_error(ps_matched, &opts->pathspec)) {
		விடுவி(ps_matched);
		திரும்பு 1;
	}
	விடுவி(ps_matched);

	/* Any unmerged paths? */
	க்கு (pos = 0; pos < the_repository->index->cache_nr; pos++) {
		மாறிலி கட்டமைப்பு cache_entry *ce = the_repository->index->cache[pos];
		எனில் (ce->ce_flags & CE_MATCHED) {
			எனில் (!ce_stage(ce))
				தொடர்;
			எனில் (opts->ignore_unmerged) {
				எனில் (!opts->quiet)
					warning(_("path '%s' is unmerged"), ce->name);
			} இல்லையெனில் எனில் (opts->writeout_stage) {
				errs |= check_stage(opts->writeout_stage, ce, pos, opts->overlay_mode);
			} இல்லையெனில் எனில் (opts->merge) {
				errs |= check_stages((1<<2) | (1<<3), ce, pos);
			} இல்லையெனில் {
				errs = 1;
				error(_("path '%s' is unmerged"), ce->name);
			}
			pos = skip_same_name(ce, pos) - 1;
		}
	}
	எனில் (errs)
		திரும்பு 1;

	/* Now we are committed to check them out */
	எனில் (opts->checkout_worktree)
		errs |= checkout_worktree(opts, new_branch_info);
	இல்லையெனில்
		remove_marked_cache_entries(the_repository->index, 1);

	/*
	 * Allow updating the index when checking out from the index.
	 * This is to save new stat info.
	 */
	எனில் (opts->checkout_worktree && !opts->checkout_index && !opts->source_மரம்)
		checkout_index = 1;
	இல்லையெனில்
		checkout_index = opts->checkout_index;

	எனில் (checkout_index) {
		எனில் (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
			die(_("unable to write new index file"));
	} இல்லையெனில் {
		/*
		 * NEEDSWORK: if --worktree is not specified, we
		 * should save stat info of checked out files in the
		 * index to avoid the next (potentially costly)
		 * refresh. But it's a bit tricker to do...
		 */
		rollback_lock_file(&lock_file);
	}

	refs_read_ref_full(get_main_ref_store(the_repository), "HEAD", 0,
			   &rev, வெற்று);
	head = lookup_commit_reference_gently(the_repository, &rev, 1);

	errs |= post_checkout_hook(head, head, 0);
	திரும்பு errs;
}

நிலையான வெறுமை show_local_changes(கட்டமைப்பு object *head,
			       மாறிலி கட்டமைப்பு diff_options *opts)
{
	கட்டமைப்பு rev_info rev;
	/* I think we want full paths, even if we're in a subdirectory. */
	repo_init_revisions(the_repository, &rev, வெற்று);
	rev.diffopt.flags = opts->flags;
	rev.diffopt.output_format |= DIFF_FORMAT_NAME_STATUS;
	rev.diffopt.flags.recursive = 1;
	diff_setup_done(&rev.diffopt);
	add_pending_object(&rev, head, வெற்று);
	run_diff_index(&rev, 0);
	release_revisions(&rev);
}

நிலையான வெறுமை describe_detached_head(மாறிலி எழுத்து *செய்தி, கட்டமைப்பு commit *commit)
{
	கட்டமைப்பு strbuf sb = STRBUF_INIT;

	எனில் (!repo_parse_commit(the_repository, commit))
		pp_commit_easy(CMIT_FMT_ONELINE, commit, &sb);
	எனில் (print_sha1_ellipsis()) {
		fprintf(stderr, "%s %s... %s\n", செய்தி,
			repo_find_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV),
			sb.buf);
	} இல்லையெனில் {
		fprintf(stderr, "%s %s %s\n", செய்தி,
			repo_find_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV),
			sb.buf);
	}
	strbuf_release(&sb);
}

நிலையான முழு reset_மரம்(கட்டமைப்பு மரம் *மரம், மாறிலி கட்டமைப்பு checkout_opts *o,
		      முழு worktree, முழு *writeout_error,
		      கட்டமைப்பு branch_info *info)
{
	கட்டமைப்பு unpack_trees_options opts;
	கட்டமைப்பு மரம்_desc மரம்_desc;

	memset(&opts, 0, அளவு(opts));
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
			       வெற்று);
	எனில் (repo_parse_மரம்(the_repository, மரம்) < 0)
		திரும்பு 128;
	init_மரம்_desc(&மரம்_desc, &மரம்->object.oid, மரம்->buffer, மரம்->size);
	தேர்வு (unpack_trees(1, &மரம்_desc, &opts)) {
	நிலை -2:
		*writeout_error = 1;
		/*
		 * We return 0 nevertheless, as the index is all right
		 * and more importantly we have made best efforts to
		 * update paths in the work tree, and we cannot revert
		 * them.
		 */
		/* fallthrough */
	நிலை 0:
		திரும்பு 0;
	இயல்பு:
		திரும்பு 128;
	}
}

நிலையான வெறுமை setup_branch_path(கட்டமைப்பு branch_info *branch)
{
	கட்டமைப்பு strbuf buf = STRBUF_INIT;

	/*
	 * If this is a ref, resolve it; otherwise, look up the OID for our
	 * expression.  Failure here is okay.
	 */
	எனில் (!repo_dwim_ref(the_repository, branch->name, strlen(branch->name),
			   &branch->oid, &branch->refname, 0))
		repo_get_oid_committish(the_repository, branch->name, &branch->oid);

	copy_branchname(the_repository, &buf, branch->name, INTERPRET_BRANCH_LOCAL);
	எனில் (strcmp(buf.buf, branch->name)) {
		விடுவி(branch->name);
		branch->name = xstrdup(buf.buf);
	}
	strbuf_splice(&buf, 0, 0, "refs/heads/", 11);
	விடுவி(branch->path);
	branch->path = strbuf_detach(&buf, வெற்று);
}

நிலையான வெறுமை init_topts(கட்டமைப்பு unpack_trees_options *topts,
		       முழு show_progress, முழு overwrite_ignore,
		       bool quiet)
{
	memset(topts, 0, அளவு(*topts));
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

நிலையான முழு merge_working_மரம்(மாறிலி கட்டமைப்பு checkout_opts *opts,
			      கட்டமைப்பு branch_info *old_branch_info,
			      கட்டமைப்பு branch_info *new_branch_info,
			      bool quiet,
			      முழு *writeout_error)
{
	முழு ret;
	கட்டமைப்பு lock_file lock_file = LOCK_INIT;
	கட்டமைப்பு மரம் *new_மரம்;

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);
	எனில் (repo_read_index_preload(the_repository, வெற்று, 0) < 0) {
		rollback_lock_file(&lock_file);
		திரும்பு error(_("index file corrupt"));
	}

	resolve_undo_clear_index(the_repository->index);
	எனில் (opts->new_orphan_branch && opts->orphan_from_empty_மரம்) {
		எனில் (new_branch_info->commit)
			BUG("'switch --orphan' should never accept a commit as starting point");
		new_மரம் = repo_parse_மரம்_indirect(the_repository,
						    the_hash_algo->empty_மரம்);
		எனில் (!new_மரம்)
			BUG("unable to read empty tree");
	} இல்லையெனில் {
		new_மரம் = repo_get_commit_மரம்(the_repository,
						new_branch_info->commit);
		எனில் (!new_மரம்) {
			rollback_lock_file(&lock_file);
			திரும்பு error(_("unable to read tree (%s)"),
				     oid_to_hex(&new_branch_info->commit->object.oid));
		}
	}
	எனில் (opts->discard_changes) {
		ret = reset_மரம்(new_மரம், opts, 1, writeout_error, new_branch_info);
		எனில் (ret) {
			rollback_lock_file(&lock_file);
			திரும்பு ret;
		}
	} இல்லையெனில் {
		கட்டமைப்பு மரம்_desc trees[2];
		கட்டமைப்பு மரம் *மரம்;
		கட்டமைப்பு unpack_trees_options topts;
		மாறிலி கட்டமைப்பு object_id *old_commit_oid;

		refresh_index(the_repository->index, REFRESH_QUIET, வெற்று, வெற்று, வெற்று);

		எனில் (unmerged_index(the_repository->index)) {
			rollback_lock_file(&lock_file);
			error(_("you need to resolve your current index first"));
			திரும்பு 1;
		}

		/* 2-way merge to the new branch */
		init_topts(&topts, opts->show_progress,
			   opts->overwrite_ignore, quiet);
		init_checkout_metadata(&topts.meta, new_branch_info->refname,
				       new_branch_info->commit ?
				       &new_branch_info->commit->object.oid :
				       &new_branch_info->oid, வெற்று);

		old_commit_oid = old_branch_info->commit ?
			&old_branch_info->commit->object.oid :
			the_hash_algo->empty_மரம்;
		மரம் = repo_parse_மரம்_indirect(the_repository,
						old_commit_oid);
		எனில் (!மரம்)
			die(_("unable to parse commit %s"),
				oid_to_hex(old_commit_oid));

		init_மரம்_desc(&trees[0], &மரம்->object.oid,
			       மரம்->buffer, மரம்->size);
		எனில் (repo_parse_மரம்(the_repository, new_மரம்) < 0)
			die(வெற்று);
		மரம் = new_மரம்;
		init_மரம்_desc(&trees[1], &மரம்->object.oid,
			       மரம்->buffer, மரம்->size);

		ret = unpack_trees(2, trees, &topts);
		clear_unpack_trees_porcelain(&topts);
		எனில் (ret == -1) {
			rollback_lock_file(&lock_file);
			திரும்பு MERGE_WORKING_மரம்_UNPACK_FAILED;
		}
	}

	எனில் (!cache_மரம்_fully_valid(the_repository->index))
		cache_மரம்_update(the_repository->index, WRITE_மரம்_SILENT | WRITE_மரம்_REPAIR);

	எனில் (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
		die(_("unable to write new index file"));

	எனில் (!opts->discard_changes && !opts->quiet && new_branch_info->commit)
		show_local_changes(&new_branch_info->commit->object, &opts->diff_options);

	திரும்பு 0;
}

நிலையான வெறுமை report_tracking(கட்டமைப்பு branch_info *new_branch_info)
{
	கட்டமைப்பு strbuf sb = STRBUF_INIT;
	கட்டமைப்பு branch *branch = branch_get(new_branch_info->name);

	எனில் (!format_tracking_info(branch, &sb, AHEAD_BEHIND_FULL, 1))
		திரும்பு;
	fputs(sb.buf, stdout);
	strbuf_release(&sb);
}

நிலையான வெறுமை update_refs_for_switch(மாறிலி கட்டமைப்பு checkout_opts *opts,
				   கட்டமைப்பு branch_info *old_branch_info,
				   கட்டமைப்பு branch_info *new_branch_info)
{
	கட்டமைப்பு strbuf செய்தி = STRBUF_INIT;
	மாறிலி எழுத்து *old_desc, *reflog_செய்தி;
	எனில் (opts->new_branch) {
		எனில் (opts->new_orphan_branch) {
			எண்ணிக்கை log_refs_config log_all_ref_updates = LOG_REFS_UNSET;
			மாறிலி எழுத்து *மதிப்பு;
			எழுத்து *refname;

			எனில் (!repo_config_get_string_tmp(the_repository, "core.logallrefupdates", &மதிப்பு))
				log_all_ref_updates = refs_parse_log_all_ref_updates_config(மதிப்பு);

			refname = mkpathdup("refs/heads/%s", opts->new_orphan_branch);
			எனில் (opts->new_branch_log &&
			    !should_autocreate_reflog(log_all_ref_updates, refname)) {
				முழு ret;
				கட்டமைப்பு strbuf err = STRBUF_INIT;

				ret = refs_create_reflog(get_main_ref_store(the_repository),
							 refname, &err);
				எனில் (ret) {
					fprintf(stderr, _("Can not do reflog for '%s': %s\n"),
						opts->new_orphan_branch, err.buf);
					strbuf_release(&err);
					விடுவி(refname);
					திரும்பு;
				}
				strbuf_release(&err);
			}
			விடுவி(refname);
		}
		இல்லையெனில்
			create_branch(the_repository,
				      opts->new_branch, new_branch_info->name,
				      opts->new_branch_force ? 1 : 0,
				      opts->new_branch_force ? 1 : 0,
				      opts->new_branch_log,
				      opts->quiet,
				      opts->track,
				      0);
		விடுவி(new_branch_info->name);
		விடுவி(new_branch_info->refname);
		new_branch_info->name = xstrdup(opts->new_branch);
		setup_branch_path(new_branch_info);
	}

	old_desc = old_branch_info->name;
	எனில் (!old_desc && old_branch_info->commit)
		old_desc = oid_to_hex(&old_branch_info->commit->object.oid);

	reflog_செய்தி = getenv("GIT_REFLOG_ACTION");
	எனில் (!reflog_செய்தி)
		strbuf_addf(&செய்தி, "checkout: moving from %s to %s",
			old_desc ? old_desc : "(invalid)", new_branch_info->name);
	இல்லையெனில்
		strbuf_insertstr(&செய்தி, 0, reflog_செய்தி);

	எனில் (!strcmp(new_branch_info->name, "HEAD") && !new_branch_info->path && !opts->force_detach) {
		/* Nothing to do. */
	} இல்லையெனில் எனில் (opts->force_detach || !new_branch_info->path) {	/* No longer on any branch. */
		refs_update_ref(get_main_ref_store(the_repository), செய்தி.buf,
				"HEAD", &new_branch_info->commit->object.oid,
				வெற்று,
				REF_NO_DEREF, UPDATE_REFS_DIE_ON_ERR);
		எனில் (!opts->quiet) {
			எனில் (old_branch_info->path &&
			    advice_enabled(ADVICE_DETACHED_HEAD) && !opts->force_detach)
				detach_advice(new_branch_info->name);
			describe_detached_head(_("HEAD is now at"), new_branch_info->commit);
		}
	} இல்லையெனில் எனில் (new_branch_info->path) {	/* Switch branches. */
		எனில் (refs_update_symref(get_main_ref_store(the_repository), "HEAD", new_branch_info->path, செய்தி.buf) < 0)
			die(_("unable to update HEAD"));
		எனில் (!opts->quiet) {
			எனில் (old_branch_info->path && !strcmp(new_branch_info->path, old_branch_info->path)) {
				எனில் (opts->new_branch_force)
					fprintf(stderr, _("Reset branch '%s'\n"),
						new_branch_info->name);
				இல்லையெனில்
					fprintf(stderr, _("Already on '%s'\n"),
						new_branch_info->name);
			} இல்லையெனில் எனில் (opts->new_branch) {
				எனில் (opts->branch_exists)
					fprintf(stderr, _("Switched to and reset branch '%s'\n"), new_branch_info->name);
				இல்லையெனில்
					fprintf(stderr, _("Switched to a new branch '%s'\n"), new_branch_info->name);
			} இல்லையெனில் {
				fprintf(stderr, _("Switched to branch '%s'\n"),
					new_branch_info->name);
			}
		}
		எனில் (old_branch_info->path && old_branch_info->name) {
			எனில் (!refs_ref_exists(get_main_ref_store(the_repository), old_branch_info->path) && refs_reflog_exists(get_main_ref_store(the_repository), old_branch_info->path))
				refs_delete_reflog(get_main_ref_store(the_repository),
						   old_branch_info->path);
		}
	}
	remove_branch_state(the_repository, !opts->quiet);
	strbuf_release(&செய்தி);
	எனில் (!opts->quiet &&
	    !opts->force_detach &&
	    (new_branch_info->path || !strcmp(new_branch_info->name, "HEAD")))
		report_tracking(new_branch_info);
}

நிலையான முழு add_pending_uninteresting_ref(மாறிலி கட்டமைப்பு reference *ref, வெறுமை *cb_data)
{
	add_pending_oid(cb_data, ref->name, ref->oid, UNINTERESTING);
	திரும்பு 0;
}

நிலையான வெறுமை describe_one_orphan(கட்டமைப்பு strbuf *sb, கட்டமைப்பு commit *commit)
{
	strbuf_addstr(sb, "  ");
	strbuf_add_unique_abbrev(sb, &commit->object.oid, DEFAULT_ABBREV);
	strbuf_addch(sb, ' ');
	எனில் (!repo_parse_commit(the_repository, commit))
		pp_commit_easy(CMIT_FMT_ONELINE, commit, sb);
	strbuf_addch(sb, '\n');
}

#define ORPHAN_CUTOFF 4
நிலையான வெறுமை suggest_reattach(கட்டமைப்பு commit *commit, கட்டமைப்பு rev_info *revs)
{
	கட்டமைப்பு commit *c, *last = வெற்று;
	கட்டமைப்பு strbuf sb = STRBUF_INIT;
	முழு lost = 0;
	போது ((c = get_revision(revs)) != வெற்று) {
		எனில் (lost < ORPHAN_CUTOFF)
			describe_one_orphan(&sb, c);
		last = c;
		lost++;
	}
	எனில் (ORPHAN_CUTOFF < lost) {
		முழு more = lost - ORPHAN_CUTOFF;
		எனில் (more == 1)
			describe_one_orphan(&sb, last);
		இல்லையெனில்
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

	எனில் (advice_enabled(ADVICE_DETACHED_HEAD))
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
நிலையான வெறுமை orphaned_commit_warning(கட்டமைப்பு commit *old_commit, கட்டமைப்பு commit *new_commit)
{
	கட்டமைப்பு rev_info revs;
	கட்டமைப்பு object *object = &old_commit->object;

	repo_init_revisions(the_repository, &revs, வெற்று);
	setup_revisions(0, வெற்று, &revs, வெற்று);

	object->flags &= ~UNINTERESTING;
	add_pending_object(&revs, object, oid_to_hex(&object->oid));

	refs_for_each_ref(get_main_ref_store(the_repository),
			  add_pending_uninteresting_ref, &revs);
	எனில் (new_commit)
		add_pending_oid(&revs, "HEAD",
				&new_commit->object.oid,
				UNINTERESTING);

	எனில் (prepare_revision_walk(&revs))
		die(_("internal error in revision walk"));
	எனில் (!(old_commit->object.flags & UNINTERESTING))
		suggest_reattach(old_commit, &revs);
	இல்லையெனில்
		describe_detached_head(_("Previous HEAD position was"), old_commit);

	/* Clean up objects used, as they will be reused. */
	repo_clear_commit_marks(the_repository, ALL_REV_FLAGS);
	release_revisions(&revs);
}

நிலையான முழு switch_branches(மாறிலி கட்டமைப்பு checkout_opts *opts,
			   கட்டமைப்பு branch_info *new_branch_info)
{
	முழு ret = 0;
	கட்டமைப்பு branch_info old_branch_info = { 0 };
	கட்டமைப்பு object_id rev;
	முழு flag, writeout_error = 0;
	முழு do_merge = 1;
	முழு created_autostash = 0;
	எண்ணிக்கை stash_apply_result autostash_res = STASH_APPLY_CLEAN;
	கட்டமைப்பு strbuf old_commit_shortname = STRBUF_INIT;
	கட்டமைப்பு strbuf autostash_செய்தி = STRBUF_INIT;
	மாறிலி எழுத்து *stash_label_base = வெற்று;

	trace2_cmd_mode("branch");

	memset(&old_branch_info, 0, அளவு(old_branch_info));
	old_branch_info.path = refs_resolve_refdup(get_main_ref_store(the_repository),
						   "HEAD", 0, &rev, &flag);
	எனில் (old_branch_info.path)
		old_branch_info.commit = lookup_commit_reference_gently(the_repository, &rev, 1);
	எனில் (!(flag & REF_ISSYMREF))
		FREE_AND_NULL(old_branch_info.path);

	எனில் (old_branch_info.path) {
		மாறிலி எழுத்து *மாறிலி prefix = "refs/heads/";
		மாறிலி எழுத்து *p;
		எனில் (skip_prefix(old_branch_info.path, prefix, &p))
			old_branch_info.name = xstrdup(p);
	}

	எனில் (opts->new_orphan_branch && opts->orphan_from_empty_மரம்) {
		எனில் (new_branch_info->name)
			BUG("'switch --orphan' should never accept a commit as starting point");
		new_branch_info->commit = வெற்று;
		new_branch_info->name = xstrdup("(empty)");
		do_merge = 1;
	}

	எனில் (!new_branch_info->name) {
		new_branch_info->name = xstrdup("HEAD");
		new_branch_info->commit = old_branch_info.commit;
		எனில் (!new_branch_info->commit)
			die(_("You are on a branch yet to be born"));
		parse_commit_or_die(new_branch_info->commit);

		எனில் (opts->only_merge_on_switching_branches)
			do_merge = 0;
	}

	எனில் (old_branch_info.name) {
		stash_label_base = old_branch_info.name;
	} இல்லையெனில் எனில் (old_branch_info.commit) {
		strbuf_add_unique_abbrev(&old_commit_shortname,
					 &old_branch_info.commit->object.oid,
					 DEFAULT_ABBREV);
		stash_label_base = old_commit_shortname.buf;
	}

	எனில் (do_merge) {
		ret = merge_working_மரம்(opts, &old_branch_info, new_branch_info,
					 opts->merge, &writeout_error);
		எனில் (ret == MERGE_WORKING_மரம்_UNPACK_FAILED && opts->merge) {
			strbuf_addf(&autostash_செய்தி,
				    "autostash while switching to '%s'",
				    new_branch_info->name);
			create_autostash_ref(the_repository,
					     "CHECKOUT_AUTOSTASH_HEAD",
					     autostash_செய்தி.buf, true);
			created_autostash = 1;
			ret = merge_working_மரம்(opts, &old_branch_info, new_branch_info,
						 false, &writeout_error);
		}
		எனில் (created_autostash) {
			எனில் (opts->conflict_style >= 0) {
				கட்டமைப்பு strbuf cfg = STRBUF_INIT;
				strbuf_addf(&cfg, "merge.conflictStyle=%s",
					    conflict_style_name(opts->conflict_style));
				git_config_தள்ளு_parameter(cfg.buf);
				strbuf_release(&cfg);
			}
			autostash_res = apply_autostash_ref(the_repository,
				    "CHECKOUT_AUTOSTASH_HEAD",
				    new_branch_info->name,
				    "local",
				    stash_label_base,
				    autostash_செய்தி.buf);
		}
		எனில் (ret) {
			branch_info_release(&old_branch_info);
			strbuf_release(&old_commit_shortname);
			strbuf_release(&autostash_செய்தி);
			திரும்பு ret < 0 ? 1 : ret;
		}
	}

	எனில் (!opts->quiet && !old_branch_info.path && old_branch_info.commit && new_branch_info->commit != old_branch_info.commit)
		orphaned_commit_warning(old_branch_info.commit, new_branch_info->commit);

	எனில் (autostash_res == STASH_APPLY_CONFLICT && !opts->quiet)
		fputc('\n', stderr);
	update_refs_for_switch(opts, &old_branch_info, new_branch_info);

	எனில் (created_autostash) {
		discard_index(the_repository->index);
		எனில் (repo_read_index(the_repository) < 0)
			die(_("index file corrupt"));

		எனில் (!opts->quiet && new_branch_info->commit) {
			printf(_("The following paths have local changes:\n"));
			show_local_changes(&new_branch_info->commit->object,
					   &opts->diff_options);
		}
	}

	ret = post_checkout_hook(old_branch_info.commit, new_branch_info->commit, 1);
	branch_info_release(&old_branch_info);
	strbuf_release(&old_commit_shortname);
	strbuf_release(&autostash_செய்தி);

	திரும்பு ret || writeout_error;
}

நிலையான முழு git_checkout_config(மாறிலி எழுத்து *var, மாறிலி எழுத்து *மதிப்பு,
			       மாறிலி கட்டமைப்பு config_context *ctx, வெறுமை *cb)
{
	கட்டமைப்பு checkout_opts *opts = cb;

	எனில் (!strcmp(var, "diff.ignoresubmodules")) {
		எனில் (!மதிப்பு)
			திரும்பு config_error_nonbool(var);
		handle_ignore_submodules_arg(&opts->diff_options, மதிப்பு);
		திரும்பு 0;
	}
	எனில் (!strcmp(var, "checkout.guess")) {
		opts->dwim_new_local_branch = git_config_bool(var, மதிப்பு);
		திரும்பு 0;
	}

	எனில் (starts_with(var, "submodule."))
		திரும்பு git_default_submodule_config(var, மதிப்பு, வெற்று);

	திரும்பு git_xmerge_config(var, மதிப்பு, ctx, வெற்று);
}

நிலையான வெறுமை setup_new_branch_info_and_source_மரம்(
	கட்டமைப்பு branch_info *new_branch_info,
	கட்டமைப்பு checkout_opts *opts,
	கட்டமைப்பு object_id *rev,
	மாறிலி எழுத்து *arg)
{
	கட்டமைப்பு மரம் **source_மரம் = &opts->source_மரம்;
	கட்டமைப்பு object_id branch_rev;

	/* treat '@' as a shortcut for 'HEAD' */
	new_branch_info->name = !strcmp(arg, "@") ? xstrdup("HEAD") :
						    xstrdup(arg);
	setup_branch_path(new_branch_info);

	எனில் (!check_refname_format(new_branch_info->path, 0) &&
	    !refs_read_ref(get_main_ref_store(the_repository), new_branch_info->path, &branch_rev))
		oidcpy(rev, &branch_rev);
	இல்லையெனில்
		/* not an existing branch */
		FREE_AND_NULL(new_branch_info->path);

	new_branch_info->commit = lookup_commit_reference_gently(the_repository, rev, 1);
	எனில் (!new_branch_info->commit) {
		/* not a commit */
		*source_மரம் = repo_parse_மரம்_indirect(the_repository, rev);
		எனில் (!*source_மரம்)
			die(_("unable to read tree (%s)"), oid_to_hex(rev));
	} இல்லையெனில் {
		parse_commit_or_die(new_branch_info->commit);
		*source_மரம் = repo_get_commit_மரம்(the_repository,
						    new_branch_info->commit);
		எனில் (!*source_மரம்)
			die(_("unable to read tree (%s)"),
			    oid_to_hex(&new_branch_info->commit->object.oid));
	}
}


எண்ணிக்கை checkout_command {
	CHECKOUT_CHECKOUT = 1,
	CHECKOUT_SWITCH = 2,
	CHECKOUT_RESTORE = 3,
};

நிலையான வெறுமை advise_disambiguating_remotes(எண்ணிக்கை checkout_command which_command,
					  மாறிலி எழுத்து *branch,
					  மாறிலி கட்டமைப்பு string_list *matched_remote_names)
{
	மாறிலி எழுத்து *cmdname;
	கட்டமைப்பு string_list_item *item;

	தேர்வு (which_command) {
	நிலை CHECKOUT_CHECKOUT:
		cmdname = "checkout";
		நிறுத்து;
	நிலை CHECKOUT_SWITCH:
		cmdname = "switch";
		நிறுத்து;
	இயல்பு:
		BUG("command <%d> should not reach advise_disambiguating_remotes",
		    which_command);
		நிறுத்து;
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

நிலையான எழுத்து *parse_remote_branch(மாறிலி எழுத்து *arg,
				 கட்டமைப்பு object_id *rev,
				 முழு could_be_checkout_paths,
				 எண்ணிக்கை checkout_command which_command)
{
	முழு இலக்கம்_matches = 0;
	கட்டமைப்பு string_list matched_remote_names = STRING_LIST_INIT_DUP;

	எழுத்து *remote = unique_tracking_name(arg, rev, &இலக்கம்_matches,
					    &matched_remote_names);

	எனில் (remote && could_be_checkout_paths) {
		die(_("'%s' could be both a local file and a tracking branch.\n"
			"Please use -- (and optionally --no-guess) to disambiguate"),
		    arg);
	}

	எனில் (!remote && இலக்கம்_matches > 1) {
		எனில் (advice_enabled(ADVICE_CHECKOUT_AMBIGUOUS_REMOTE_BRANCH_NAME))
			advise_disambiguating_remotes(which_command, arg,
						      &matched_remote_names);
		die(_("'%s' matched multiple (%d) remote tracking branches"),
		    arg, இலக்கம்_matches);
	}

	string_list_clear(&matched_remote_names, 0);

	திரும்பு remote;
}

நிலையான முழு parse_branchname_arg(முழு argc, மாறிலி எழுத்து **argv,
				முழு dwim_new_local_branch_ok,
				எண்ணிக்கை checkout_command which_command,
				கட்டமைப்பு branch_info *new_branch_info,
				கட்டமைப்பு checkout_opts *opts,
				கட்டமைப்பு object_id *rev)
{
	மாறிலி எழுத்து **new_branch = &opts->new_branch;
	முழு argcount = 0;
	மாறிலி எழுத்து *arg;
	எழுத்து *remote = வெற்று;
	முழு dash_dash_pos;
	முழு has_dash_dash = 0;
	முழு i;

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
	எனில் (!argc)
		திரும்பு 0;

	எனில் (!opts->accept_pathspec) {
		எனில் (argc > 1)
			die(_("only one reference expected"));
		has_dash_dash = 1; /* helps disambiguate */
	}

	arg = argv[0];
	dash_dash_pos = -1;
	க்கு (i = 0; i < argc; i++) {
		எனில் (opts->accept_pathspec && !strcmp(argv[i], "--")) {
			dash_dash_pos = i;
			நிறுத்து;
		}
	}
	எனில் (dash_dash_pos == 0)
		திரும்பு 1; /* case (2) */
	இல்லையெனில் எனில் (dash_dash_pos == 1)
		has_dash_dash = 1; /* case (3) or (1) */
	இல்லையெனில் எனில் (dash_dash_pos >= 2)
		die(_("only one reference expected, %d given."), dash_dash_pos);
	opts->மொத்தம்_checkout_paths = !opts->quiet && !has_dash_dash;

	எனில் (!strcmp(arg, "-"))
		arg = "@{-1}";

	எனில் (repo_get_oid_mb(the_repository, arg, rev)) {
		/*
		 * Either case (3) or (4), with <something> not being
		 * a commit, or an attempt to use case (1) with an
		 * invalid ref.
		 *
		 * It's likely an error, but we need to find out if
		 * we should auto-create the branch, case (3).(b).
		 */
		முழு recover_with_dwim = dwim_new_local_branch_ok;

		முழு could_be_checkout_paths = !has_dash_dash &&
			check_filename(opts->prefix, arg);

		எனில் (!has_dash_dash && !no_wildcard(arg))
			recover_with_dwim = 0;

		/*
		 * Accept "git checkout foo", "git checkout foo --"
		 * and "git switch foo" as candidates for dwim.
		 */
		எனில் (!(argc == 1 && !has_dash_dash) &&
		    !(argc == 2 && has_dash_dash) &&
		    opts->accept_pathspec)
			recover_with_dwim = 0;

		எனில் (recover_with_dwim) {
			remote = parse_remote_branch(arg, rev,
						     could_be_checkout_paths,
						     which_command);
			எனில் (remote) {
				*new_branch = arg;
				arg = remote;
				/* DWIMmed to create local branch, case (3).(b) */
			} இல்லையெனில் {
				recover_with_dwim = 0;
			}
		}

		எனில் (!recover_with_dwim) {
			எனில் (has_dash_dash)
				die(_("invalid reference: %s"), arg);
			திரும்பு argcount;
		}
	}

	/* we can't end up being in (2) anymore, eat the argument */
	argcount++;
	argv++;
	argc--;

	setup_new_branch_info_and_source_மரம்(new_branch_info, opts, rev, arg);

	எனில் (!opts->source_மரம்)                   /* case (1): want a tree */
		die(_("reference is not a tree: %s"), arg);

	எனில் (!has_dash_dash) {	/* case (3).(d) -> (1) */
		/*
		 * Do not complain the most common case
		 *	git checkout branch
		 * even if there happen to be a file called 'branch';
		 * it would be extremely annoying.
		 */
		எனில் (argc)
			verify_non_filename(the_repository, opts->prefix, arg);
	} இல்லையெனில் எனில் (opts->accept_pathspec) {
		argcount++;
		argv++;
		argc--;
	}

	விடுவி(remote);
	திரும்பு argcount;
}

நிலையான முழு switch_unborn_to_new_branch(மாறிலி கட்டமைப்பு checkout_opts *opts)
{
	முழு status;
	கட்டமைப்பு strbuf branch_ref = STRBUF_INIT;

	trace2_cmd_mode("unborn");

	எனில் (!opts->new_branch)
		die(_("You are on a branch yet to be born"));
	strbuf_addf(&branch_ref, "refs/heads/%s", opts->new_branch);
	status = refs_update_symref(get_main_ref_store(the_repository),
				    "HEAD", branch_ref.buf, "checkout -b");
	strbuf_release(&branch_ref);
	எனில் (!opts->quiet)
		fprintf(stderr, _("Switched to a new branch '%s'\n"),
			opts->new_branch);
	திரும்பு status;
}

நிலையான வெறுமை die_expecting_a_branch(மாறிலி கட்டமைப்பு branch_info *branch_info)
{
	கட்டமைப்பு object_id oid;
	எழுத்து *to_free;
	முழு code;

	எனில் (repo_dwim_ref(the_repository, branch_info->name,
			  strlen(branch_info->name), &oid, &to_free, 0) == 1) {
		மாறிலி எழுத்து *ref = to_free;

		எனில் (skip_prefix(ref, "refs/tags/", &ref))
			code = die_message(_("a branch is expected, got tag '%s'"), ref);
		இல்லையெனில் எனில் (skip_prefix(ref, "refs/remotes/", &ref))
			code = die_message(_("a branch is expected, got remote branch '%s'"), ref);
		இல்லையெனில்
			code = die_message(_("a branch is expected, got '%s'"), ref);
	}
	இல்லையெனில் எனில் (branch_info->commit)
		code = die_message(_("a branch is expected, got commit '%s'"), branch_info->name);
	இல்லையெனில்
		/*
		 * This case should never happen because we already die() on
		 * non-commit, but just in case.
		 */
		code = die_message(_("a branch is expected, got '%s'"), branch_info->name);

	எனில் (advice_enabled(ADVICE_SUGGEST_DETACHING_HEAD))
		advise(_("If you want to detach HEAD at the commit, try again with the --detach option."));

	exit(code);
}

நிலையான வெறுமை die_if_some_operation_in_progress(வெறுமை)
{
	கட்டமைப்பு wt_status_state state;

	memset(&state, 0, அளவு(state));
	wt_status_get_state(the_repository, &state, 0);

	எனில் (state.merge_in_progress)
		die(_("cannot switch branch while merging\n"
		      "Consider \"git merge --quit\" "
		      "or \"git worktree add\"."));
	எனில் (state.am_in_progress)
		die(_("cannot switch branch in the middle of an am session\n"
		      "Consider \"git am --quit\" "
		      "or \"git worktree add\"."));
	எனில் (state.rebase_interactive_in_progress || state.rebase_in_progress)
		die(_("cannot switch branch while rebasing\n"
		      "Consider \"git rebase --quit\" "
		      "or \"git worktree add\"."));
	எனில் (state.cherry_pick_in_progress)
		die(_("cannot switch branch while cherry-picking\n"
		      "Consider \"git cherry-pick --quit\" "
		      "or \"git worktree add\"."));
	எனில் (state.revert_in_progress)
		die(_("cannot switch branch while reverting\n"
		      "Consider \"git revert --quit\" "
		      "or \"git worktree add\"."));
	எனில் (state.bisect_in_progress)
		warning(_("you are switching branch while bisecting"));

	wt_status_state_free_buffers(&state);
}

/*
 * die if attempting to checkout an existing branch that is in use
 * in another worktree, unless ignore-other-wortrees option is given.
 * The check is bypassed when the branch is already the current one,
 * as it will not make things any worse.
 */
நிலையான வெறுமை die_if_switching_to_a_branch_in_use(கட்டமைப்பு checkout_opts *opts,
						மாறிலி எழுத்து *full_ref)
{
	முழு flags;
	எழுத்து *head_ref;

	எனில் (opts->ignore_other_worktrees)
		திரும்பு;
	head_ref = refs_resolve_refdup(get_main_ref_store(the_repository),
				       "HEAD", 0, வெற்று, &flags);
	எனில் (head_ref && (!(flags & REF_ISSYMREF) || strcmp(head_ref, full_ref)))
		die_if_checked_out(full_ref, 1);
	விடுவி(head_ref);
}

நிலையான முழு checkout_branch(கட்டமைப்பு checkout_opts *opts,
			   கட்டமைப்பு branch_info *new_branch_info)
{
	கட்டமைப்பு repo_config_மதிப்புகள் *cfg = repo_config_மதிப்புகள்(the_repository);
	முழு noop_switch = (!new_branch_info->name &&
			   !opts->new_branch &&
			   !opts->force_detach);

	எனில் (opts->pathspec.nr)
		die(_("paths cannot be used with switching branches"));

	எனில் (opts->patch_mode)
		die(_("'%s' cannot be used with switching branches"),
		    "--patch");

	எனில் (opts->overlay_mode != -1)
		die(_("'%s' cannot be used with switching branches"),
		    "--[no]-overlay");

	எனில் (opts->writeout_stage) {
		மாறிலி எழுத்து *செய்தி;
		எனில் (noop_switch)
			செய்தி = _("'%s' needs the paths to check out");
		இல்லையெனில்
			செய்தி = _("'%s' cannot be used with switching branches");
		die(செய்தி, "--ours/--theirs");
	}

	எனில் (opts->force && opts->merge)
		die(_("'%s' cannot be used with '%s'"), "-f", "-m");

	எனில் (opts->discard_changes && opts->merge)
		die(_("'%s' cannot be used with '%s'"), "--discard-changes", "--merge");

	எனில் (opts->force_detach && opts->new_branch)
		die(_("'%s' cannot be used with '%s'"),
		    "--detach", "-b/-B/--orphan");

	எனில் (opts->new_orphan_branch) {
		எனில் (opts->track != BRANCH_TRACK_UNSPECIFIED)
			die(_("'%s' cannot be used with '%s'"), "--orphan", "-t");
		எனில் (opts->orphan_from_empty_மரம் && new_branch_info->name)
			die(_("'%s' cannot take <start-point>"), "--orphan");
	} இல்லையெனில் எனில் (opts->force_detach) {
		எனில் (opts->track != BRANCH_TRACK_UNSPECIFIED)
			die(_("'%s' cannot be used with '%s'"), "--detach", "-t");
	} இல்லையெனில் எனில் (opts->track == BRANCH_TRACK_UNSPECIFIED)
		opts->track = cfg->branch_track;

	எனில் (new_branch_info->name && !new_branch_info->commit)
		die(_("Cannot switch branch to a non-commit '%s'"),
		    new_branch_info->name);

	எனில் (noop_switch &&
	    !opts->switch_branch_doing_nothing_is_ok)
		die(_("missing branch or commit argument"));

	எனில் (!opts->implicit_detach &&
	    !opts->force_detach &&
	    !opts->new_branch &&
	    !opts->new_branch_force &&
	    new_branch_info->name &&
	    !new_branch_info->path)
		die_expecting_a_branch(new_branch_info);

	எனில் (!opts->can_switch_when_in_progress)
		die_if_some_operation_in_progress();

	/* "git checkout <branch>" */
	எனில் (new_branch_info->path && !opts->force_detach && !opts->new_branch)
		die_if_switching_to_a_branch_in_use(opts, new_branch_info->path);

	/* "git checkout -B <branch>" */
	எனில் (opts->new_branch_force) {
		எழுத்து *full_ref = xstrfmt("refs/heads/%s", opts->new_branch);
		die_if_switching_to_a_branch_in_use(opts, full_ref);
		விடுவி(full_ref);
	}

	எனில் (!new_branch_info->commit && opts->new_branch) {
		கட்டமைப்பு object_id rev;
		முழு flag;

		எனில் (!refs_read_ref_full(get_main_ref_store(the_repository), "HEAD", 0, &rev, &flag) &&
		    (flag & REF_ISSYMREF) && is_null_oid(&rev))
			திரும்பு switch_unborn_to_new_branch(opts);
	}
	திரும்பு switch_branches(opts, new_branch_info);
}

நிலையான முழு parse_opt_conflict(மாறிலி கட்டமைப்பு option *o, மாறிலி எழுத்து *arg, முழு unset)
{
	கட்டமைப்பு checkout_opts *opts = o->மதிப்பு;

	எனில் (unset) {
		opts->conflict_style = -1;
		திரும்பு 0;
	}
	opts->conflict_style = parse_conflict_style_name(arg);
	எனில் (opts->conflict_style < 0)
		திரும்பு error(_("unknown conflict style '%s'"), arg);
	/* --conflict overrides a previous --no-merge */
	எனில் (!opts->merge)
		opts->merge = -1;

	திரும்பு 0;
}

நிலையான கட்டமைப்பு option *add_common_options(கட்டமைப்பு checkout_opts *opts,
					 கட்டமைப்பு option *prevopts)
{
	கட்டமைப்பு option options[] = {
		OPT__QUIET(&opts->quiet, N_("suppress progress reporting")),
		OPT_CALLBACK_F(0, "recurse-submodules", வெற்று,
			    "checkout", "control recursive updating of submodules",
			    PARSE_OPT_OPTARG, option_parse_recurse_submodules_worktree_updater),
		OPT_BOOL(0, "progress", &opts->show_progress, N_("force progress reporting")),
		OPT_BOOL('m', "merge", &opts->merge, N_("perform a 3-way merge with the new branch")),
		OPT_CALLBACK(0, "conflict", opts, N_("style"),
			     N_("conflict style (merge, diff3, or zdiff3)"),
			     parse_opt_conflict),
		OPT_END()
	};
	கட்டமைப்பு option *newopts = parse_options_concat(prevopts, options);
	விடுவி(prevopts);
	திரும்பு newopts;
}

நிலையான கட்டமைப்பு option *add_common_switch_branch_options(
	கட்டமைப்பு checkout_opts *opts, கட்டமைப்பு option *prevopts)
{
	கட்டமைப்பு option options[] = {
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
	கட்டமைப்பு option *newopts = parse_options_concat(prevopts, options);
	விடுவி(prevopts);
	திரும்பு newopts;
}

நிலையான கட்டமைப்பு option *add_checkout_path_options(கட்டமைப்பு checkout_opts *opts,
						கட்டமைப்பு option *prevopts)
{
	கட்டமைப்பு option options[] = {
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
	கட்டமைப்பு option *newopts = parse_options_concat(prevopts, options);
	விடுவி(prevopts);
	திரும்பு newopts;
}

/* create-branch option (either b or c) */
நிலையான எழுத்து cb_option = 'b';

நிலையான முழு checkout_main(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
			 கட்டமைப்பு checkout_opts *opts, கட்டமைப்பு option *options,
			 எண்ணிக்கை checkout_command which_command)
{
	முழு parseopt_flags = 0;
	கட்டமைப்பு branch_info new_branch_info = { 0 };
	முழு ret;

	நிலையான மாறிலி எழுத்து * மாறிலி checkout_usage[] = {
		N_("git checkout [<options>] <branch>"),
		N_("git checkout [<options>] [<branch>] -- <file>..."),
		வெற்று,
	};

	நிலையான மாறிலி எழுத்து * மாறிலி switch_branch_usage[] = {
		N_("git switch [<options>] [<branch>]"),
		வெற்று,
	};

	நிலையான மாறிலி எழுத்து * மாறிலி restore_usage[] = {
		N_("git restore [<options>] [--source=<branch>] <file>..."),
		வெற்று,
	};

	மாறிலி எழுத்து * மாறிலி *usagestr;

	தேர்வு (which_command) {
	நிலை CHECKOUT_CHECKOUT:
		usagestr = checkout_usage;
		நிறுத்து;
	நிலை CHECKOUT_SWITCH:
		usagestr = switch_branch_usage;
		நிறுத்து;
	நிலை CHECKOUT_RESTORE:
		usagestr = restore_usage;
		நிறுத்து;
	இயல்பு:
		BUG("no such checkout variant %d", which_command);
	}

	opts->overwrite_ignore = 1;
	opts->prefix = prefix;
	opts->show_progress = -1;

	repo_config(the_repository, git_checkout_config, opts);
	எனில் (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_full_index = 0;
	}

	opts->track = BRANCH_TRACK_UNSPECIFIED;

	எனில் (!opts->accept_pathspec && !opts->accept_ref)
		BUG("make up your mind, you need to take _something_");
	எனில் (opts->accept_pathspec && opts->accept_ref)
		parseopt_flags = PARSE_OPT_KEEP_DASHDASH;

	argc = parse_options(argc, argv, prefix, options,
			     usagestr, parseopt_flags);

	எனில் (opts->patch_context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	எனில் (opts->patch_interhunk_context < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	எனில் (!opts->patch_mode) {
		எனில் (opts->patch_context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--patch");
		எனில் (opts->patch_interhunk_context != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--patch");
		எனில் (!opts->auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--patch");
	}

	எனில் (opts->show_progress < 0) {
		எனில் (opts->quiet)
			opts->show_progress = 0;
		இல்லையெனில்
			opts->show_progress = isatty(2);
	}

	/* --conflicts implies --merge */
	எனில் (opts->merge == -1)
		opts->merge = opts->conflict_style >= 0;

	எனில் (opts->force) {
		opts->discard_changes = 1;
		opts->ignore_unmerged_opt = "--force";
		opts->ignore_unmerged = 1;
	}

	எனில் ((!!opts->new_branch + !!opts->new_branch_force + !!opts->new_orphan_branch) > 1)
		die(_("options '-%c', '-%c', and '%s' cannot be used together"),
			cb_option, toupper(cb_option), "--orphan");

	எனில் (opts->overlay_mode == 1 && opts->patch_mode)
		die(_("options '%s' and '%s' cannot be used together"), "-p", "--overlay");

	எனில் (opts->checkout_index >= 0 || opts->checkout_worktree >= 0) {
		எனில் (opts->checkout_index < 0)
			opts->checkout_index = 0;
		எனில் (opts->checkout_worktree < 0)
			opts->checkout_worktree = 0;
	} இல்லையெனில் {
		எனில் (opts->checkout_index < 0)
			opts->checkout_index = -opts->checkout_index - 1;
		எனில் (opts->checkout_worktree < 0)
			opts->checkout_worktree = -opts->checkout_worktree - 1;
	}
	எனில் (opts->checkout_index < 0 || opts->checkout_worktree < 0)
		BUG("these flags should be non-negative by now");
	/*
	 * convenient shortcut: "git restore --staged [--worktree]" equals
	 * "git restore --staged [--worktree] --source HEAD"
	 */
	எனில் (!opts->from_treeish && opts->checkout_index)
		opts->from_treeish = "HEAD";

	/*
	 * From here on, new_branch will contain the branch to be checked out,
	 * and new_branch_force and new_orphan_branch will tell us which one of
	 * -b/-B/-c/-C/--orphan is being used.
	 */
	எனில் (opts->new_branch_force)
		opts->new_branch = opts->new_branch_force;

	எனில் (opts->new_orphan_branch)
		opts->new_branch = opts->new_orphan_branch;

	/* --track without -c/-C/-b/-B/--orphan should DWIM */
	எனில் (opts->track != BRANCH_TRACK_UNSPECIFIED && !opts->new_branch) {
		மாறிலி எழுத்து *argv0 = argv[0];
		எனில் (!argc || !strcmp(argv0, "--"))
			die(_("--track needs a branch name"));
		skip_prefix(argv0, "refs/", &argv0);
		skip_prefix(argv0, "remotes/", &argv0);
		argv0 = strchr(argv0, '/');
		எனில் (!argv0 || !argv0[1])
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
	எனில் (argc && opts->accept_ref) {
		கட்டமைப்பு object_id rev;
		முழு dwim_ok =
			!opts->patch_mode &&
			opts->dwim_new_local_branch &&
			opts->track == BRANCH_TRACK_UNSPECIFIED &&
			!opts->new_branch;
		முழு n = parse_branchname_arg(argc, argv, dwim_ok, which_command,
					     &new_branch_info, opts, &rev);
		argv += n;
		argc -= n;
	} இல்லையெனில் எனில் (!opts->accept_ref && opts->from_treeish) {
		கட்டமைப்பு object_id rev;

		எனில் (repo_get_oid_mb(the_repository, opts->from_treeish, &rev))
			die(_("could not resolve '%s'"), opts->from_treeish);

		setup_new_branch_info_and_source_மரம்(&new_branch_info,
						      opts, &rev,
						      opts->from_treeish);

		எனில் (!opts->source_மரம்)
			die(_("reference is not a tree: %s"), opts->from_treeish);
	}

	எனில் (argc) {
		parse_pathspec(&opts->pathspec, 0,
			       opts->patch_mode ? PATHSPEC_PREFIX_ORIGIN : 0,
			       prefix, argv);

		எனில் (!opts->pathspec.nr)
			die(_("invalid path specification"));

		/*
		 * Try to give more helpful suggestion.
		 * new_branch && argc > 1 will be caught later.
		 */
		எனில் (opts->new_branch && argc == 1 && !new_branch_info.commit)
			die(_("'%s' is not a commit and a branch '%s' cannot be created from it"),
				argv[0], opts->new_branch);

		எனில் (opts->force_detach)
			die(_("git checkout: --detach does not take a path argument '%s'"),
			    argv[0]);
	}

	எனில் (opts->pathspec_from_file) {
		எனில் (opts->pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		எனில் (opts->force_detach)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--detach");

		எனில் (opts->patch_mode)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--patch");

		parse_pathspec_file(&opts->pathspec, 0,
				    0,
				    prefix, opts->pathspec_from_file, opts->pathspec_file_nul);
	} இல்லையெனில் எனில் (opts->pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	opts->pathspec.recursive = 1;

	எனில் (opts->pathspec.nr) {
		எனில் (1 < !!opts->writeout_stage + !!opts->force + !!opts->merge)
			die(_("git checkout: --ours/--theirs, --force and --merge are incompatible when\n"
			      "checking out of the index."));
	} இல்லையெனில் {
		எனில் (opts->accept_pathspec && !opts->empty_pathspec_ok &&
		    !opts->patch_mode)	/* patch mode is special */
			die(_("you must specify path(s) to restore"));
	}

	எனில் (opts->new_branch) {
		கட்டமைப்பு strbuf buf = STRBUF_INIT;

		எனில் (opts->new_branch_force)
			opts->branch_exists = validate_branchname(opts->new_branch, &buf);
		இல்லையெனில்
			opts->branch_exists =
				validate_new_branchname(opts->new_branch, &buf, 0);
		strbuf_release(&buf);
	}

	எனில் (opts->patch_mode || opts->pathspec.nr)
		ret = checkout_paths(opts, &new_branch_info);
	இல்லையெனில்
		ret = checkout_branch(opts, &new_branch_info);

	branch_info_release(&new_branch_info);
	clear_pathspec(&opts->pathspec);
	விடுவி(opts->pathspec_from_file);
	விடுவி(options);

	திரும்பு ret;
}

முழு cmd_checkout(முழு argc,
		 மாறிலி எழுத்து **argv,
		 மாறிலி எழுத்து *prefix,
		 கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு checkout_opts opts = CHECKOUT_OPTS_INIT;
	கட்டமைப்பு option *options;
	கட்டமைப்பு option checkout_options[] = {
		OPT_STRING('b', வெற்று, &opts.new_branch, N_("branch"),
			   N_("create and checkout a new branch")),
		OPT_STRING('B', வெற்று, &opts.new_branch_force, N_("branch"),
			   N_("create/reset and checkout a branch")),
		OPT_BOOL('l', வெற்று, &opts.new_branch_log, N_("create reflog for new branch")),
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
	opts.orphan_from_empty_மரம் = 0;
	opts.empty_pathspec_ok = 1;
	opts.overlay_mode = -1;
	opts.checkout_index = -2;    /* default on */
	opts.checkout_worktree = -2; /* default on */

	எனில் (argc == 3 && !strcmp(argv[1], "-b")) {
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

	திரும்பு checkout_main(argc, argv, prefix, &opts, options,
			     CHECKOUT_CHECKOUT);
}

முழு cmd_switch(முழு argc,
	       மாறிலி எழுத்து **argv,
	       மாறிலி எழுத்து *prefix,
	       கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு checkout_opts opts = CHECKOUT_OPTS_INIT;
	கட்டமைப்பு option *options = வெற்று;
	கட்டமைப்பு option switch_options[] = {
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
	opts.orphan_from_empty_மரம் = 1;
	opts.overlay_mode = -1;

	options = parse_options_dup(switch_options);
	options = add_common_options(&opts, options);
	options = add_common_switch_branch_options(&opts, options);

	cb_option = 'c';

	திரும்பு checkout_main(argc, argv, prefix, &opts, options,
			     CHECKOUT_SWITCH);
}

முழு cmd_restore(முழு argc,
		மாறிலி எழுத்து **argv,
		மாறிலி எழுத்து *prefix,
		கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு checkout_opts opts = CHECKOUT_OPTS_INIT;
	கட்டமைப்பு option *options;
	கட்டமைப்பு option restore_options[] = {
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

	திரும்பு checkout_main(argc, argv, prefix, &opts, options,
			     CHECKOUT_RESTORE);
}
