#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "advice.h"
#അന്തര്ഭാവ "branch.h"
#അന്തര്ഭാവ "cache-tree.h"
#അന്തര്ഭാവ "checkout.h"
#അന്തര്ഭാവ "commit.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "diff.h"
#അന്തര്ഭാവ "dir.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "hex.h"
#അന്തര്ഭാവ "hook.h"
#അന്തര്ഭാവ "merge-ll.h"
#അന്തര്ഭാവ "lockfile.h"
#അന്തര്ഭാവ "mem-pool.h"
#അന്തര്ഭാവ "object-file.h"
#അന്തര്ഭാവ "object-name.h"
#അന്തര്ഭാവ "odb.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "path.h"
#അന്തര്ഭാവ "preload-index.h"
#അന്തര്ഭാവ "read-cache.h"
#അന്തര്ഭാവ "refs.h"
#അന്തര്ഭാവ "remote.h"
#അന്തര്ഭാവ "repo-settings.h"
#അന്തര്ഭാവ "resolve-undo.h"
#അന്തര്ഭാവ "revision.h"
#അന്തര്ഭാവ "sequencer.h"
#അന്തര്ഭാവ "setup.h"
#അന്തര്ഭാവ "sparse-index.h"
#അന്തര്ഭാവ "strvec.h"
#അന്തര്ഭാവ "submodule.h"
#അന്തര്ഭാവ "symlinks.h"
#അന്തര്ഭാവ "trace2.h"
#അന്തര്ഭാവ "tree.h"
#അന്തര്ഭാവ "tree-walk.h"
#അന്തര്ഭാവ "unpack-trees.h"
#അന്തര്ഭാവ "wt-status.h"
#അന്തര്ഭാവ "xdiff-interface.h"
#അന്തര്ഭാവ "entry.h"
#അന്തര്ഭാവ "parallel-checkout.h"
#അന്തര്ഭാവ "add-interactive.h"

ഘടന checkout_opts {
	പൂർണ്ണസംഖ്യ patch_mode;
	പൂർണ്ണസംഖ്യ patch_context;
	പൂർണ്ണസംഖ്യ patch_interhunk_context;
	പൂർണ്ണസംഖ്യ auto_advance;
	പൂർണ്ണസംഖ്യ quiet;
	പൂർണ്ണസംഖ്യ merge;
	പൂർണ്ണസംഖ്യ force;
	പൂർണ്ണസംഖ്യ force_detach;
	പൂർണ്ണസംഖ്യ implicit_detach;
	പൂർണ്ണസംഖ്യ writeout_stage;
	പൂർണ്ണസംഖ്യ overwrite_ignore;
	പൂർണ്ണസംഖ്യ ignore_skipworktree;
	പൂർണ്ണസംഖ്യ ignore_other_worktrees;
	പൂർണ്ണസംഖ്യ show_progress;
	പൂർണ്ണസംഖ്യ എണ്ണം_checkout_paths;
	പൂർണ്ണസംഖ്യ overlay_mode;
	പൂർണ്ണസംഖ്യ dwim_new_local_branch;
	പൂർണ്ണസംഖ്യ discard_changes;
	പൂർണ്ണസംഖ്യ accept_ref;
	പൂർണ്ണസംഖ്യ accept_pathspec;
	പൂർണ്ണസംഖ്യ switch_branch_doing_nothing_is_ok;
	പൂർണ്ണസംഖ്യ only_merge_on_switching_branches;
	പൂർണ്ണസംഖ്യ can_switch_when_in_progress;
	പൂർണ്ണസംഖ്യ orphan_from_empty_വൃക്ഷം;
	പൂർണ്ണസംഖ്യ empty_pathspec_ok;
	പൂർണ്ണസംഖ്യ checkout_index;
	പൂർണ്ണസംഖ്യ checkout_worktree;
	സ്ഥിരം അക്ഷരം *ignore_unmerged_opt;
	പൂർണ്ണസംഖ്യ ignore_unmerged;
	പൂർണ്ണസംഖ്യ pathspec_file_nul;
	അക്ഷരം *pathspec_from_file;

	സ്ഥിരം അക്ഷരം *new_branch;
	സ്ഥിരം അക്ഷരം *new_branch_force;
	സ്ഥിരം അക്ഷരം *new_orphan_branch;
	പൂർണ്ണസംഖ്യ new_branch_log;
	എണ്ണൽ branch_track track;
	ഘടന diff_options diff_options;
	പൂർണ്ണസംഖ്യ conflict_style;

	പൂർണ്ണസംഖ്യ branch_exists;
	സ്ഥിരം അക്ഷരം *prefix;
	ഘടന pathspec pathspec;
	സ്ഥിരം അക്ഷരം *from_treeish;
	ഘടന വൃക്ഷം *source_വൃക്ഷം;
};

#define CHECKOUT_OPTS_INIT { \
	.conflict_style = -1, \
	.merge = -1, \
	.patch_context = -1, \
	.patch_interhunk_context = -1, \
	.auto_advance = 1, \
}

#define MERGE_WORKING_വൃക്ഷം_UNPACK_FAILED (-2)

ഘടന branch_info {
	അക്ഷരം *name; /* The short name used */
	അക്ഷരം *path; /* The full name of a real branch */
	ഘടന commit *commit; /* The named commit */
	അക്ഷരം *refname; /* The full name of the ref being checked out. */
	ഘടന object_id oid; /* The object ID of the commit being checked out. */
	/*
	 * if not null the branch is detached because it's already
	 * checked out in this checkout
	 */
	അക്ഷരം *checkout;
};

സ്ഥിതം ശൂന്യം branch_info_release(ഘടന branch_info *info)
{
	സ്വതന്ത്രം(info->name);
	സ്വതന്ത്രം(info->path);
	സ്വതന്ത്രം(info->refname);
	സ്വതന്ത്രം(info->checkout);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ post_checkout_hook(ഘടന commit *old_commit, ഘടന commit *new_commit,
			      പൂർണ്ണസംഖ്യ changed)
{
	ഘടന run_hooks_opt opt = RUN_HOOKS_OPT_INIT_FORCE_SERIAL;

	/*
	 * "new_commit" can be NULL when checking out from the index before
	 * a commit exists.
	 */
	strvec_pushl(&opt.args,
		     oid_to_hex(old_commit ? &old_commit->object.oid : null_oid(the_hash_algo)),
		     oid_to_hex(new_commit ? &new_commit->object.oid : null_oid(the_hash_algo)),
		     changed ? "1" : "0",
		     ശൂന്യ);

	തിരികെ run_hooks_opt(the_repository, "post-checkout", &opt);
}

/*
 * Handle a tree object and determine if we need to recurse into the
 * tree (READ_TREE_RECURSIVE) or skip it (0).
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ try_update_sparse_directory(സ്ഥിരം ഘടന object_id *oid,
				       ഘടന strbuf *base,
				       സ്ഥിരം അക്ഷരം *pathname,
				       പൂർണ്ണസംഖ്യ overlay_mode)
{
	ഘടന strbuf dirpath = STRBUF_INIT;
	ഘടന cache_entry *old;
	പൂർണ്ണസംഖ്യ pos, result = READ_വൃക്ഷം_RECURSIVE;

	എങ്കിൽ (!the_repository->index->sparse_index)
		തിരികെ result;

	strbuf_addbuf(&dirpath, base);
	strbuf_addstr(&dirpath, pathname);
	strbuf_addch(&dirpath, '/');

	pos = index_name_pos_sparse(the_repository->index,
				    dirpath.buf, dirpath.len);
	എങ്കിൽ (pos < 0)
		പോകുക cleanup;

	old = the_repository->index->cache[pos];
	എങ്കിൽ (!S_ISSPARSEDIR(old->ce_mode))
		പോകുക cleanup;

	എങ്കിൽ (oideq(oid, &old->oid)) {
		/* Tree content already matches; no need to descend. */
		result = 0;
	} അല്ലെങ്കിൽ എങ്കിൽ (!overlay_mode) {
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
	തിരികെ result;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ update_some(സ്ഥിരം ഘടന object_id *oid, ഘടന strbuf *base,
		       സ്ഥിരം അക്ഷരം *pathname, unsigned mode, ശൂന്യം *context)
{
	പൂർണ്ണസംഖ്യ len;
	ഘടന cache_entry *ce;
	പൂർണ്ണസംഖ്യ pos;
	പൂർണ്ണസംഖ്യ overlay_mode = context ? *((പൂർണ്ണസംഖ്യ *)context) : 1;

	എങ്കിൽ (S_ISDIR(mode))
		തിരികെ try_update_sparse_directory(oid, base, pathname,
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
	എങ്കിൽ (pos >= 0) {
		ഘടന cache_entry *old = the_repository->index->cache[pos];
		എങ്കിൽ (ce->ce_mode == old->ce_mode &&
		    !ce_intent_to_add(old) &&
		    oideq(&ce->oid, &old->oid)) {
			old->ce_flags |= CE_UPDATE;
			discard_cache_entry(ce);
			തിരികെ 0;
		}
	}

	add_index_entry(the_repository->index, ce,
			ADD_CACHE_OK_TO_ADD | ADD_CACHE_OK_TO_REPLACE);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ read_വൃക്ഷം_some(ഘടന വൃക്ഷം *വൃക്ഷം, സ്ഥിരം ഘടന pathspec *pathspec,
			  പൂർണ്ണസംഖ്യ overlay_mode)
{
	read_വൃക്ഷം(the_repository, വൃക്ഷം,
		  pathspec, update_some, &overlay_mode);

	/* update the index with the given tree's info
	 * for all args, expanding wildcards, and exit
	 * with any non-zero return code.
	 */
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ skip_same_name(സ്ഥിരം ഘടന cache_entry *ce, പൂർണ്ണസംഖ്യ pos)
{
	ആകുമ്പോൾ (++pos < the_repository->index->cache_nr &&
	       !strcmp(the_repository->index->cache[pos]->name, ce->name))
		; /* skip */
	തിരികെ pos;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ check_stage(പൂർണ്ണസംഖ്യ stage, സ്ഥിരം ഘടന cache_entry *ce, പൂർണ്ണസംഖ്യ pos,
		       പൂർണ്ണസംഖ്യ overlay_mode)
{
	ആകുമ്പോൾ (pos < the_repository->index->cache_nr &&
	       !strcmp(the_repository->index->cache[pos]->name, ce->name)) {
		എങ്കിൽ (ce_stage(the_repository->index->cache[pos]) == stage)
			തിരികെ 0;
		pos++;
	}
	എങ്കിൽ (!overlay_mode)
		തിരികെ 0;
	എങ്കിൽ (stage == 2)
		തിരികെ error(_("path '%s' does not have our version"), ce->name);
	അല്ലെങ്കിൽ
		തിരികെ error(_("path '%s' does not have their version"), ce->name);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ check_stages(unsigned stages, സ്ഥിരം ഘടന cache_entry *ce, പൂർണ്ണസംഖ്യ pos)
{
	unsigned seen = 0;
	സ്ഥിരം അക്ഷരം *name = ce->name;

	ആകുമ്പോൾ (pos < the_repository->index->cache_nr) {
		ce = the_repository->index->cache[pos];
		എങ്കിൽ (strcmp(name, ce->name))
			നിർത്തുക;
		seen |= (1 << ce_stage(ce));
		pos++;
	}
	എങ്കിൽ ((stages & seen) != stages)
		തിരികെ error(_("path '%s' does not have all necessary versions"),
			     name);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ checkout_stage(പൂർണ്ണസംഖ്യ stage, സ്ഥിരം ഘടന cache_entry *ce, പൂർണ്ണസംഖ്യ pos,
			  സ്ഥിരം ഘടന checkout *state, പൂർണ്ണസംഖ്യ *nr_checkouts,
			  പൂർണ്ണസംഖ്യ overlay_mode)
{
	ആകുമ്പോൾ (pos < the_repository->index->cache_nr &&
	       !strcmp(the_repository->index->cache[pos]->name, ce->name)) {
		എങ്കിൽ (ce_stage(the_repository->index->cache[pos]) == stage)
			തിരികെ checkout_entry(the_repository->index->cache[pos], state,
					      ശൂന്യ, nr_checkouts);
		pos++;
	}
	എങ്കിൽ (!overlay_mode) {
		unlink_entry(ce, ശൂന്യ);
		തിരികെ 0;
	}
	എങ്കിൽ (stage == 2)
		തിരികെ error(_("path '%s' does not have our version"), ce->name);
	അല്ലെങ്കിൽ
		തിരികെ error(_("path '%s' does not have their version"), ce->name);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ checkout_merged(പൂർണ്ണസംഖ്യ pos, സ്ഥിരം ഘടന checkout *state,
			   പൂർണ്ണസംഖ്യ *nr_checkouts, ഘടന mem_pool *ce_mem_pool,
			   പൂർണ്ണസംഖ്യ conflict_style)
{
	ഘടന cache_entry *ce = the_repository->index->cache[pos];
	സ്ഥിരം അക്ഷരം *path = ce->name;
	mmfile_t ancestor, ours, theirs;
	എണ്ണൽ ll_merge_result merge_status;
	പൂർണ്ണസംഖ്യ status;
	ഘടന object_id oid;
	mmfile_t result_buf;
	ഘടന object_id threeway[3];
	unsigned mode = 0;
	ഘടന ll_merge_options ll_opts = LL_MERGE_OPTIONS_INIT;
	പൂർണ്ണസംഖ്യ renormalize = 0;

	memset(threeway, 0, വലുപ്പം(threeway));
	ആകുമ്പോൾ (pos < the_repository->index->cache_nr) {
		പൂർണ്ണസംഖ്യ stage;
		stage = ce_stage(ce);
		എങ്കിൽ (!stage || strcmp(path, ce->name))
			നിർത്തുക;
		oidcpy(&threeway[stage - 1], &ce->oid);
		എങ്കിൽ (stage == 2)
			mode = create_ce_mode(ce->ce_mode);
		pos++;
		ce = the_repository->index->cache[pos];
	}
	എങ്കിൽ (is_null_oid(&threeway[1]) || is_null_oid(&threeway[2]))
		തിരികെ error(_("path '%s' does not have necessary versions"), path);

	read_mmblob(&ancestor, the_repository->objects, &threeway[0]);
	read_mmblob(&ours, the_repository->objects, &threeway[1]);
	read_mmblob(&theirs, the_repository->objects, &threeway[2]);

	repo_config_get_bool(the_repository, "merge.renormalize", &renormalize);
	ll_opts.renormalize = renormalize;
	ll_opts.conflict_style = conflict_style;
	merge_status = ll_merge(&result_buf, path, &ancestor, "base",
				&ours, "ours", &theirs, "theirs",
				state->istate, &ll_opts);
	സ്വതന്ത്രം(ancestor.ptr);
	സ്വതന്ത്രം(ours.ptr);
	സ്വതന്ത്രം(theirs.ptr);
	എങ്കിൽ (merge_status == LL_MERGE_ദ്വിമാന_CONFLICT)
		warning("Cannot merge binary files: %s (%s vs. %s)",
			path, "ours", "theirs");
	എങ്കിൽ (merge_status < 0 || !result_buf.ptr) {
		സ്വതന്ത്രം(result_buf.ptr);
		തിരികെ error(_("path '%s': cannot merge"), path);
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
	എങ്കിൽ (odb_write_object(the_repository->objects, result_buf.ptr, result_buf.size, OBJ_BLOB, &oid))
		die(_("Unable to add merge result for '%s'"), path);
	സ്വതന്ത്രം(result_buf.ptr);
	ce = make_transient_cache_entry(mode, &oid, path, 2, ce_mem_pool);
	എങ്കിൽ (!ce)
		die(_("make_cache_entry failed for path '%s'"), path);
	status = checkout_entry(ce, state, ശൂന്യ, nr_checkouts);
	തിരികെ status;
}

സ്ഥിതം ശൂന്യം mark_ce_for_checkout_overlay(ഘടന cache_entry *ce,
					 അക്ഷരം *ps_matched,
					 സ്ഥിരം ഘടന checkout_opts *opts)
{
	ce->ce_flags &= ~CE_MATCHED;
	എങ്കിൽ (!opts->ignore_skipworktree && ce_skip_worktree(ce))
		തിരികെ;
	എങ്കിൽ (opts->source_വൃക്ഷം && !(ce->ce_flags & CE_UPDATE))
		/*
		 * "git checkout tree-ish -- path", but this entry
		 * is in the original index but is not in tree-ish
		 * or does not match the pathspec; it will not be
		 * checked out to the working tree.  We will not do
		 * anything to this entry at all.
		 */
		തിരികെ;
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
	എങ്കിൽ (ce_path_match(the_repository->index, ce, &opts->pathspec, ps_matched))
		ce->ce_flags |= CE_MATCHED;
}

സ്ഥിതം ശൂന്യം mark_ce_for_checkout_no_overlay(ഘടന cache_entry *ce,
					    അക്ഷരം *ps_matched,
					    സ്ഥിരം ഘടന checkout_opts *opts)
{
	ce->ce_flags &= ~CE_MATCHED;
	എങ്കിൽ (!opts->ignore_skipworktree && ce_skip_worktree(ce))
		തിരികെ;
	എങ്കിൽ (ce_path_match(the_repository->index, ce, &opts->pathspec, ps_matched)) {
		ce->ce_flags |= CE_MATCHED;
		എങ്കിൽ (opts->source_വൃക്ഷം && !(ce->ce_flags & CE_UPDATE))
			/*
			 * In overlay mode, but the path is not in
			 * tree-ish, which means we should remove it
			 * from the index and the working tree.
			 */
			ce->ce_flags |= CE_REMOVE | CE_WT_REMOVE;
	}
}

സ്ഥിതം പൂർണ്ണസംഖ്യ checkout_worktree(സ്ഥിരം ഘടന checkout_opts *opts,
			     സ്ഥിരം ഘടന branch_info *info)
{
	ഘടന checkout state = CHECKOUT_INIT;
	പൂർണ്ണസംഖ്യ nr_checkouts = 0, nr_unmerged = 0;
	പൂർണ്ണസംഖ്യ errs = 0;
	പൂർണ്ണസംഖ്യ pos;
	പൂർണ്ണസംഖ്യ pc_workers, pc_threshold;
	ഘടന mem_pool ce_mem_pool;

	state.force = 1;
	state.refresh_cache = 1;
	state.istate = the_repository->index;

	mem_pool_init(&ce_mem_pool, 0);
	get_parallel_checkout_configs(&pc_workers, &pc_threshold);
	init_checkout_metadata(&state.meta, info->refname,
			       info->commit ? &info->commit->object.oid : &info->oid,
			       ശൂന്യ);

	enable_delayed_checkout(&state);

	എങ്കിൽ (pc_workers > 1)
		init_parallel_checkout();

	വേണ്ടി (pos = 0; pos < the_repository->index->cache_nr; pos++) {
		ഘടന cache_entry *ce = the_repository->index->cache[pos];
		എങ്കിൽ (ce->ce_flags & CE_MATCHED) {
			എങ്കിൽ (!ce_stage(ce)) {
				errs |= checkout_entry(ce, &state,
						       ശൂന്യ, &nr_checkouts);
				തുടരുക;
			}
			എങ്കിൽ (opts->writeout_stage)
				errs |= checkout_stage(opts->writeout_stage,
						       ce, pos,
						       &state,
						       &nr_checkouts, opts->overlay_mode);
			അല്ലെങ്കിൽ എങ്കിൽ (opts->merge)
				errs |= checkout_merged(pos, &state,
							&nr_unmerged,
							&ce_mem_pool,
							opts->conflict_style);
			pos = skip_same_name(ce, pos) - 1;
		}
	}
	എങ്കിൽ (pc_workers > 1)
		errs |= run_parallel_checkout(&state, pc_workers, pc_threshold,
					      ശൂന്യ, ശൂന്യ);
	mem_pool_discard(&ce_mem_pool, should_validate_cache_entries());
	remove_marked_cache_entries(the_repository->index, 1);
	remove_scheduled_dirs();
	errs |= finish_delayed_checkout(&state, opts->show_progress);

	എങ്കിൽ (opts->എണ്ണം_checkout_paths) {
		എങ്കിൽ (nr_unmerged)
			fprintf_ln(stderr, Q_("Recreated %d merge conflict",
					      "Recreated %d merge conflicts",
					      nr_unmerged),
				   nr_unmerged);
		എങ്കിൽ (opts->source_വൃക്ഷം)
			fprintf_ln(stderr, Q_("Updated %d path from %s",
					      "Updated %d paths from %s",
					      nr_checkouts),
				   nr_checkouts,
				   repo_find_unique_abbrev(the_repository, &opts->source_വൃക്ഷം->object.oid,
							   DEFAULT_ABBREV));
		അല്ലെങ്കിൽ എങ്കിൽ (!nr_unmerged || nr_checkouts)
			fprintf_ln(stderr, Q_("Updated %d path from the index",
					      "Updated %d paths from the index",
					      nr_checkouts),
				   nr_checkouts);
	}

	തിരികെ errs;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ checkout_paths(സ്ഥിരം ഘടന checkout_opts *opts,
			  സ്ഥിരം ഘടന branch_info *new_branch_info)
{
	പൂർണ്ണസംഖ്യ pos;
	സ്ഥിതം അക്ഷരം *ps_matched;
	ഘടന object_id rev;
	ഘടന commit *head;
	പൂർണ്ണസംഖ്യ errs = 0;
	ഘടന lock_file lock_file = LOCK_INIT;
	പൂർണ്ണസംഖ്യ checkout_index;

	trace2_cmd_mode(opts->patch_mode ? "patch" : "path");

	എങ്കിൽ (opts->track != BRANCH_TRACK_UNSPECIFIED)
		die(_("'%s' cannot be used with updating paths"), "--track");

	എങ്കിൽ (opts->new_branch_log)
		die(_("'%s' cannot be used with updating paths"), "-l");

	എങ്കിൽ (opts->ignore_unmerged && opts->patch_mode)
		die(_("'%s' cannot be used with updating paths"),
		    opts->ignore_unmerged_opt);

	എങ്കിൽ (opts->force_detach)
		die(_("'%s' cannot be used with updating paths"), "--detach");

	എങ്കിൽ (opts->merge && opts->patch_mode)
		die(_("options '%s' and '%s' cannot be used together"), "--merge", "--patch");

	എങ്കിൽ (opts->ignore_unmerged && opts->merge)
		die(_("options '%s' and '%s' cannot be used together"),
		    opts->ignore_unmerged_opt, "-m");

	എങ്കിൽ (opts->new_branch)
		die(_("Cannot update paths and switch to branch '%s' at the same time."),
		    opts->new_branch);

	എങ്കിൽ (!opts->checkout_worktree && !opts->checkout_index)
		die(_("neither '%s' or '%s' is specified"),
		    "--staged", "--worktree");

	എങ്കിൽ (!opts->checkout_worktree && !opts->from_treeish)
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
	എങ്കിൽ (!opts->accept_ref && opts->checkout_index) {
		എങ്കിൽ (opts->writeout_stage)
			die(_("'%s' or '%s' cannot be used with %s"),
			    "--ours", "--theirs", "--staged");

		എങ്കിൽ (opts->merge)
			die(_("'%s' or '%s' cannot be used with %s"),
			    "--merge", "--conflict", "--staged");
	}

	/*
	 * recreating unmerged index entries and writing out data from
	 * unmerged index entries would make no sense when checking out
	 * of a tree-ish.
	 */
	എങ്കിൽ ((opts->merge || opts->writeout_stage) && opts->source_വൃക്ഷം)
		die(_("'%s', '%s', or '%s' cannot be used when checking out of a tree"),
		    "--merge", "--ours", "--theirs");

	എങ്കിൽ (opts->patch_mode) {
		എണ്ണൽ add_p_mode patch_mode;
		ഘടന interactive_options interactive_opts = {
			.context = opts->patch_context,
			.interhunkcontext = opts->patch_interhunk_context,
			.auto_advance = opts->auto_advance
		};
		സ്ഥിരം അക്ഷരം *rev = new_branch_info->name;
		അക്ഷരം rev_oid[GIT_MAX_HEXSZ + 1];

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
		എങ്കിൽ (rev && new_branch_info->commit && strcmp(rev, "HEAD"))
			rev = oid_to_hex_r(rev_oid, &new_branch_info->commit->object.oid);

		എങ്കിൽ (opts->checkout_index && opts->checkout_worktree)
			patch_mode = ADD_P_CHECKOUT;
		അല്ലെങ്കിൽ എങ്കിൽ (opts->checkout_index && !opts->checkout_worktree)
			patch_mode = ADD_P_RESET;
		അല്ലെങ്കിൽ എങ്കിൽ (!opts->checkout_index && opts->checkout_worktree)
			patch_mode = ADD_P_WORKTREE;
		അല്ലെങ്കിൽ
			BUG("either flag must have been set, worktree=%d, index=%d",
			    opts->checkout_worktree, opts->checkout_index);
		തിരികെ !!run_add_p(the_repository, patch_mode, &interactive_opts,
				   rev, &opts->pathspec, 0);
	}

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);
	എങ്കിൽ (repo_read_index_preload(the_repository, &opts->pathspec, 0) < 0)
		തിരികെ error(_("index file corrupt"));

	എങ്കിൽ (opts->source_വൃക്ഷം)
		read_വൃക്ഷം_some(opts->source_വൃക്ഷം, &opts->pathspec,
			       opts->overlay_mode);
	എങ്കിൽ (opts->merge)
		unmerge_index(the_repository->index, &opts->pathspec, CE_MATCHED);

	ps_matched = xcalloc(opts->pathspec.nr, 1);

	/*
	 * Make sure all pathspecs participated in locating the paths
	 * to be checked out.
	 */
	വേണ്ടി (pos = 0; pos < the_repository->index->cache_nr; pos++)
		എങ്കിൽ (opts->overlay_mode)
			mark_ce_for_checkout_overlay(the_repository->index->cache[pos],
						     ps_matched,
						     opts);
		അല്ലെങ്കിൽ
			mark_ce_for_checkout_no_overlay(the_repository->index->cache[pos],
							ps_matched,
							opts);

	എങ്കിൽ (report_path_error(ps_matched, &opts->pathspec)) {
		സ്വതന്ത്രം(ps_matched);
		തിരികെ 1;
	}
	സ്വതന്ത്രം(ps_matched);

	/* Any unmerged paths? */
	വേണ്ടി (pos = 0; pos < the_repository->index->cache_nr; pos++) {
		സ്ഥിരം ഘടന cache_entry *ce = the_repository->index->cache[pos];
		എങ്കിൽ (ce->ce_flags & CE_MATCHED) {
			എങ്കിൽ (!ce_stage(ce))
				തുടരുക;
			എങ്കിൽ (opts->ignore_unmerged) {
				എങ്കിൽ (!opts->quiet)
					warning(_("path '%s' is unmerged"), ce->name);
			} അല്ലെങ്കിൽ എങ്കിൽ (opts->writeout_stage) {
				errs |= check_stage(opts->writeout_stage, ce, pos, opts->overlay_mode);
			} അല്ലെങ്കിൽ എങ്കിൽ (opts->merge) {
				errs |= check_stages((1<<2) | (1<<3), ce, pos);
			} അല്ലെങ്കിൽ {
				errs = 1;
				error(_("path '%s' is unmerged"), ce->name);
			}
			pos = skip_same_name(ce, pos) - 1;
		}
	}
	എങ്കിൽ (errs)
		തിരികെ 1;

	/* Now we are committed to check them out */
	എങ്കിൽ (opts->checkout_worktree)
		errs |= checkout_worktree(opts, new_branch_info);
	അല്ലെങ്കിൽ
		remove_marked_cache_entries(the_repository->index, 1);

	/*
	 * Allow updating the index when checking out from the index.
	 * This is to save new stat info.
	 */
	എങ്കിൽ (opts->checkout_worktree && !opts->checkout_index && !opts->source_വൃക്ഷം)
		checkout_index = 1;
	അല്ലെങ്കിൽ
		checkout_index = opts->checkout_index;

	എങ്കിൽ (checkout_index) {
		എങ്കിൽ (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
			die(_("unable to write new index file"));
	} അല്ലെങ്കിൽ {
		/*
		 * NEEDSWORK: if --worktree is not specified, we
		 * should save stat info of checked out files in the
		 * index to avoid the next (potentially costly)
		 * refresh. But it's a bit tricker to do...
		 */
		rollback_lock_file(&lock_file);
	}

	refs_read_ref_full(get_main_ref_store(the_repository), "HEAD", 0,
			   &rev, ശൂന്യ);
	head = lookup_commit_reference_gently(the_repository, &rev, 1);

	errs |= post_checkout_hook(head, head, 0);
	തിരികെ errs;
}

സ്ഥിതം ശൂന്യം show_local_changes(ഘടന object *head,
			       സ്ഥിരം ഘടന diff_options *opts)
{
	ഘടന rev_info rev;
	/* I think we want full paths, even if we're in a subdirectory. */
	repo_init_revisions(the_repository, &rev, ശൂന്യ);
	rev.diffopt.flags = opts->flags;
	rev.diffopt.output_format |= DIFF_FORMAT_NAME_STATUS;
	rev.diffopt.flags.recursive = 1;
	diff_setup_done(&rev.diffopt);
	add_pending_object(&rev, head, ശൂന്യ);
	run_diff_index(&rev, 0);
	release_revisions(&rev);
}

സ്ഥിതം ശൂന്യം describe_detached_head(സ്ഥിരം അക്ഷരം *സന്ദേശം, ഘടന commit *commit)
{
	ഘടന strbuf sb = STRBUF_INIT;

	എങ്കിൽ (!repo_parse_commit(the_repository, commit))
		pp_commit_easy(CMIT_FMT_ONELINE, commit, &sb);
	എങ്കിൽ (print_sha1_ellipsis()) {
		fprintf(stderr, "%s %s... %s\n", സന്ദേശം,
			repo_find_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV),
			sb.buf);
	} അല്ലെങ്കിൽ {
		fprintf(stderr, "%s %s %s\n", സന്ദേശം,
			repo_find_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV),
			sb.buf);
	}
	strbuf_release(&sb);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ reset_വൃക്ഷം(ഘടന വൃക്ഷം *വൃക്ഷം, സ്ഥിരം ഘടന checkout_opts *o,
		      പൂർണ്ണസംഖ്യ worktree, പൂർണ്ണസംഖ്യ *writeout_error,
		      ഘടന branch_info *info)
{
	ഘടന unpack_trees_options opts;
	ഘടന വൃക്ഷം_desc വൃക്ഷം_desc;

	memset(&opts, 0, വലുപ്പം(opts));
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
			       ശൂന്യ);
	എങ്കിൽ (repo_parse_വൃക്ഷം(the_repository, വൃക്ഷം) < 0)
		തിരികെ 128;
	init_വൃക്ഷം_desc(&വൃക്ഷം_desc, &വൃക്ഷം->object.oid, വൃക്ഷം->buffer, വൃക്ഷം->size);
	തിരഞ്ഞെടുപ്പ് (unpack_trees(1, &വൃക്ഷം_desc, &opts)) {
	അവസ്ഥ -2:
		*writeout_error = 1;
		/*
		 * We return 0 nevertheless, as the index is all right
		 * and more importantly we have made best efforts to
		 * update paths in the work tree, and we cannot revert
		 * them.
		 */
		/* fallthrough */
	അവസ്ഥ 0:
		തിരികെ 0;
	സ്ഥിരസ്ഥിതി:
		തിരികെ 128;
	}
}

സ്ഥിതം ശൂന്യം setup_branch_path(ഘടന branch_info *branch)
{
	ഘടന strbuf buf = STRBUF_INIT;

	/*
	 * If this is a ref, resolve it; otherwise, look up the OID for our
	 * expression.  Failure here is okay.
	 */
	എങ്കിൽ (!repo_dwim_ref(the_repository, branch->name, strlen(branch->name),
			   &branch->oid, &branch->refname, 0))
		repo_get_oid_committish(the_repository, branch->name, &branch->oid);

	copy_branchname(the_repository, &buf, branch->name, INTERPRET_BRANCH_LOCAL);
	എങ്കിൽ (strcmp(buf.buf, branch->name)) {
		സ്വതന്ത്രം(branch->name);
		branch->name = xstrdup(buf.buf);
	}
	strbuf_splice(&buf, 0, 0, "refs/heads/", 11);
	സ്വതന്ത്രം(branch->path);
	branch->path = strbuf_detach(&buf, ശൂന്യ);
}

സ്ഥിതം ശൂന്യം init_topts(ഘടന unpack_trees_options *topts,
		       പൂർണ്ണസംഖ്യ show_progress, പൂർണ്ണസംഖ്യ overwrite_ignore,
		       bool quiet)
{
	memset(topts, 0, വലുപ്പം(*topts));
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

സ്ഥിതം പൂർണ്ണസംഖ്യ merge_working_വൃക്ഷം(സ്ഥിരം ഘടന checkout_opts *opts,
			      ഘടന branch_info *old_branch_info,
			      ഘടന branch_info *new_branch_info,
			      bool quiet,
			      പൂർണ്ണസംഖ്യ *writeout_error)
{
	പൂർണ്ണസംഖ്യ ret;
	ഘടന lock_file lock_file = LOCK_INIT;
	ഘടന വൃക്ഷം *new_വൃക്ഷം;

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);
	എങ്കിൽ (repo_read_index_preload(the_repository, ശൂന്യ, 0) < 0) {
		rollback_lock_file(&lock_file);
		തിരികെ error(_("index file corrupt"));
	}

	resolve_undo_clear_index(the_repository->index);
	എങ്കിൽ (opts->new_orphan_branch && opts->orphan_from_empty_വൃക്ഷം) {
		എങ്കിൽ (new_branch_info->commit)
			BUG("'switch --orphan' should never accept a commit as starting point");
		new_വൃക്ഷം = repo_parse_വൃക്ഷം_indirect(the_repository,
						    the_hash_algo->empty_വൃക്ഷം);
		എങ്കിൽ (!new_വൃക്ഷം)
			BUG("unable to read empty tree");
	} അല്ലെങ്കിൽ {
		new_വൃക്ഷം = repo_get_commit_വൃക്ഷം(the_repository,
						new_branch_info->commit);
		എങ്കിൽ (!new_വൃക്ഷം) {
			rollback_lock_file(&lock_file);
			തിരികെ error(_("unable to read tree (%s)"),
				     oid_to_hex(&new_branch_info->commit->object.oid));
		}
	}
	എങ്കിൽ (opts->discard_changes) {
		ret = reset_വൃക്ഷം(new_വൃക്ഷം, opts, 1, writeout_error, new_branch_info);
		എങ്കിൽ (ret) {
			rollback_lock_file(&lock_file);
			തിരികെ ret;
		}
	} അല്ലെങ്കിൽ {
		ഘടന വൃക്ഷം_desc trees[2];
		ഘടന വൃക്ഷം *വൃക്ഷം;
		ഘടന unpack_trees_options topts;
		സ്ഥിരം ഘടന object_id *old_commit_oid;

		refresh_index(the_repository->index, REFRESH_QUIET, ശൂന്യ, ശൂന്യ, ശൂന്യ);

		എങ്കിൽ (unmerged_index(the_repository->index)) {
			rollback_lock_file(&lock_file);
			error(_("you need to resolve your current index first"));
			തിരികെ 1;
		}

		/* 2-way merge to the new branch */
		init_topts(&topts, opts->show_progress,
			   opts->overwrite_ignore, quiet);
		init_checkout_metadata(&topts.meta, new_branch_info->refname,
				       new_branch_info->commit ?
				       &new_branch_info->commit->object.oid :
				       &new_branch_info->oid, ശൂന്യ);

		old_commit_oid = old_branch_info->commit ?
			&old_branch_info->commit->object.oid :
			the_hash_algo->empty_വൃക്ഷം;
		വൃക്ഷം = repo_parse_വൃക്ഷം_indirect(the_repository,
						old_commit_oid);
		എങ്കിൽ (!വൃക്ഷം)
			die(_("unable to parse commit %s"),
				oid_to_hex(old_commit_oid));

		init_വൃക്ഷം_desc(&trees[0], &വൃക്ഷം->object.oid,
			       വൃക്ഷം->buffer, വൃക്ഷം->size);
		എങ്കിൽ (repo_parse_വൃക്ഷം(the_repository, new_വൃക്ഷം) < 0)
			die(ശൂന്യ);
		വൃക്ഷം = new_വൃക്ഷം;
		init_വൃക്ഷം_desc(&trees[1], &വൃക്ഷം->object.oid,
			       വൃക്ഷം->buffer, വൃക്ഷം->size);

		ret = unpack_trees(2, trees, &topts);
		clear_unpack_trees_porcelain(&topts);
		എങ്കിൽ (ret == -1) {
			rollback_lock_file(&lock_file);
			തിരികെ MERGE_WORKING_വൃക്ഷം_UNPACK_FAILED;
		}
	}

	എങ്കിൽ (!cache_വൃക്ഷം_fully_valid(the_repository->index))
		cache_വൃക്ഷം_update(the_repository->index, WRITE_വൃക്ഷം_SILENT | WRITE_വൃക്ഷം_REPAIR);

	എങ്കിൽ (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
		die(_("unable to write new index file"));

	എങ്കിൽ (!opts->discard_changes && !opts->quiet && new_branch_info->commit)
		show_local_changes(&new_branch_info->commit->object, &opts->diff_options);

	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം report_tracking(ഘടന branch_info *new_branch_info)
{
	ഘടന strbuf sb = STRBUF_INIT;
	ഘടന branch *branch = branch_get(new_branch_info->name);

	എങ്കിൽ (!format_tracking_info(branch, &sb, AHEAD_BEHIND_FULL, 1))
		തിരികെ;
	fputs(sb.buf, stdout);
	strbuf_release(&sb);
}

സ്ഥിതം ശൂന്യം update_refs_for_switch(സ്ഥിരം ഘടന checkout_opts *opts,
				   ഘടന branch_info *old_branch_info,
				   ഘടന branch_info *new_branch_info)
{
	ഘടന strbuf സന്ദേശം = STRBUF_INIT;
	സ്ഥിരം അക്ഷരം *old_desc, *reflog_സന്ദേശം;
	എങ്കിൽ (opts->new_branch) {
		എങ്കിൽ (opts->new_orphan_branch) {
			എണ്ണൽ log_refs_config log_all_ref_updates = LOG_REFS_UNSET;
			സ്ഥിരം അക്ഷരം *മൂല്യം;
			അക്ഷരം *refname;

			എങ്കിൽ (!repo_config_get_string_tmp(the_repository, "core.logallrefupdates", &മൂല്യം))
				log_all_ref_updates = refs_parse_log_all_ref_updates_config(മൂല്യം);

			refname = mkpathdup("refs/heads/%s", opts->new_orphan_branch);
			എങ്കിൽ (opts->new_branch_log &&
			    !should_autocreate_reflog(log_all_ref_updates, refname)) {
				പൂർണ്ണസംഖ്യ ret;
				ഘടന strbuf err = STRBUF_INIT;

				ret = refs_create_reflog(get_main_ref_store(the_repository),
							 refname, &err);
				എങ്കിൽ (ret) {
					fprintf(stderr, _("Can not do reflog for '%s': %s\n"),
						opts->new_orphan_branch, err.buf);
					strbuf_release(&err);
					സ്വതന്ത്രം(refname);
					തിരികെ;
				}
				strbuf_release(&err);
			}
			സ്വതന്ത്രം(refname);
		}
		അല്ലെങ്കിൽ
			create_branch(the_repository,
				      opts->new_branch, new_branch_info->name,
				      opts->new_branch_force ? 1 : 0,
				      opts->new_branch_force ? 1 : 0,
				      opts->new_branch_log,
				      opts->quiet,
				      opts->track,
				      0);
		സ്വതന്ത്രം(new_branch_info->name);
		സ്വതന്ത്രം(new_branch_info->refname);
		new_branch_info->name = xstrdup(opts->new_branch);
		setup_branch_path(new_branch_info);
	}

	old_desc = old_branch_info->name;
	എങ്കിൽ (!old_desc && old_branch_info->commit)
		old_desc = oid_to_hex(&old_branch_info->commit->object.oid);

	reflog_സന്ദേശം = getenv("GIT_REFLOG_ACTION");
	എങ്കിൽ (!reflog_സന്ദേശം)
		strbuf_addf(&സന്ദേശം, "checkout: moving from %s to %s",
			old_desc ? old_desc : "(invalid)", new_branch_info->name);
	അല്ലെങ്കിൽ
		strbuf_insertstr(&സന്ദേശം, 0, reflog_സന്ദേശം);

	എങ്കിൽ (!strcmp(new_branch_info->name, "HEAD") && !new_branch_info->path && !opts->force_detach) {
		/* Nothing to do. */
	} അല്ലെങ്കിൽ എങ്കിൽ (opts->force_detach || !new_branch_info->path) {	/* No longer on any branch. */
		refs_update_ref(get_main_ref_store(the_repository), സന്ദേശം.buf,
				"HEAD", &new_branch_info->commit->object.oid,
				ശൂന്യ,
				REF_NO_DEREF, UPDATE_REFS_DIE_ON_ERR);
		എങ്കിൽ (!opts->quiet) {
			എങ്കിൽ (old_branch_info->path &&
			    advice_enabled(ADVICE_DETACHED_HEAD) && !opts->force_detach)
				detach_advice(new_branch_info->name);
			describe_detached_head(_("HEAD is now at"), new_branch_info->commit);
		}
	} അല്ലെങ്കിൽ എങ്കിൽ (new_branch_info->path) {	/* Switch branches. */
		എങ്കിൽ (refs_update_symref(get_main_ref_store(the_repository), "HEAD", new_branch_info->path, സന്ദേശം.buf) < 0)
			die(_("unable to update HEAD"));
		എങ്കിൽ (!opts->quiet) {
			എങ്കിൽ (old_branch_info->path && !strcmp(new_branch_info->path, old_branch_info->path)) {
				എങ്കിൽ (opts->new_branch_force)
					fprintf(stderr, _("Reset branch '%s'\n"),
						new_branch_info->name);
				അല്ലെങ്കിൽ
					fprintf(stderr, _("Already on '%s'\n"),
						new_branch_info->name);
			} അല്ലെങ്കിൽ എങ്കിൽ (opts->new_branch) {
				എങ്കിൽ (opts->branch_exists)
					fprintf(stderr, _("Switched to and reset branch '%s'\n"), new_branch_info->name);
				അല്ലെങ്കിൽ
					fprintf(stderr, _("Switched to a new branch '%s'\n"), new_branch_info->name);
			} അല്ലെങ്കിൽ {
				fprintf(stderr, _("Switched to branch '%s'\n"),
					new_branch_info->name);
			}
		}
		എങ്കിൽ (old_branch_info->path && old_branch_info->name) {
			എങ്കിൽ (!refs_ref_exists(get_main_ref_store(the_repository), old_branch_info->path) && refs_reflog_exists(get_main_ref_store(the_repository), old_branch_info->path))
				refs_delete_reflog(get_main_ref_store(the_repository),
						   old_branch_info->path);
		}
	}
	remove_branch_state(the_repository, !opts->quiet);
	strbuf_release(&സന്ദേശം);
	എങ്കിൽ (!opts->quiet &&
	    !opts->force_detach &&
	    (new_branch_info->path || !strcmp(new_branch_info->name, "HEAD")))
		report_tracking(new_branch_info);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ add_pending_uninteresting_ref(സ്ഥിരം ഘടന reference *ref, ശൂന്യം *cb_data)
{
	add_pending_oid(cb_data, ref->name, ref->oid, UNINTERESTING);
	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം describe_one_orphan(ഘടന strbuf *sb, ഘടന commit *commit)
{
	strbuf_addstr(sb, "  ");
	strbuf_add_unique_abbrev(sb, &commit->object.oid, DEFAULT_ABBREV);
	strbuf_addch(sb, ' ');
	എങ്കിൽ (!repo_parse_commit(the_repository, commit))
		pp_commit_easy(CMIT_FMT_ONELINE, commit, sb);
	strbuf_addch(sb, '\n');
}

#define ORPHAN_CUTOFF 4
സ്ഥിതം ശൂന്യം suggest_reattach(ഘടന commit *commit, ഘടന rev_info *revs)
{
	ഘടന commit *c, *last = ശൂന്യ;
	ഘടന strbuf sb = STRBUF_INIT;
	പൂർണ്ണസംഖ്യ lost = 0;
	ആകുമ്പോൾ ((c = get_revision(revs)) != ശൂന്യ) {
		എങ്കിൽ (lost < ORPHAN_CUTOFF)
			describe_one_orphan(&sb, c);
		last = c;
		lost++;
	}
	എങ്കിൽ (ORPHAN_CUTOFF < lost) {
		പൂർണ്ണസംഖ്യ more = lost - ORPHAN_CUTOFF;
		എങ്കിൽ (more == 1)
			describe_one_orphan(&sb, last);
		അല്ലെങ്കിൽ
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

	എങ്കിൽ (advice_enabled(ADVICE_DETACHED_HEAD))
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
സ്ഥിതം ശൂന്യം orphaned_commit_warning(ഘടന commit *old_commit, ഘടന commit *new_commit)
{
	ഘടന rev_info revs;
	ഘടന object *object = &old_commit->object;

	repo_init_revisions(the_repository, &revs, ശൂന്യ);
	setup_revisions(0, ശൂന്യ, &revs, ശൂന്യ);

	object->flags &= ~UNINTERESTING;
	add_pending_object(&revs, object, oid_to_hex(&object->oid));

	refs_for_each_ref(get_main_ref_store(the_repository),
			  add_pending_uninteresting_ref, &revs);
	എങ്കിൽ (new_commit)
		add_pending_oid(&revs, "HEAD",
				&new_commit->object.oid,
				UNINTERESTING);

	എങ്കിൽ (prepare_revision_walk(&revs))
		die(_("internal error in revision walk"));
	എങ്കിൽ (!(old_commit->object.flags & UNINTERESTING))
		suggest_reattach(old_commit, &revs);
	അല്ലെങ്കിൽ
		describe_detached_head(_("Previous HEAD position was"), old_commit);

	/* Clean up objects used, as they will be reused. */
	repo_clear_commit_marks(the_repository, ALL_REV_FLAGS);
	release_revisions(&revs);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ switch_branches(സ്ഥിരം ഘടന checkout_opts *opts,
			   ഘടന branch_info *new_branch_info)
{
	പൂർണ്ണസംഖ്യ ret = 0;
	ഘടന branch_info old_branch_info = { 0 };
	ഘടന object_id rev;
	പൂർണ്ണസംഖ്യ flag, writeout_error = 0;
	പൂർണ്ണസംഖ്യ do_merge = 1;
	പൂർണ്ണസംഖ്യ created_autostash = 0;
	എണ്ണൽ stash_apply_result autostash_res = STASH_APPLY_CLEAN;
	ഘടന strbuf old_commit_shortname = STRBUF_INIT;
	ഘടന strbuf autostash_സന്ദേശം = STRBUF_INIT;
	സ്ഥിരം അക്ഷരം *stash_label_base = ശൂന്യ;

	trace2_cmd_mode("branch");

	memset(&old_branch_info, 0, വലുപ്പം(old_branch_info));
	old_branch_info.path = refs_resolve_refdup(get_main_ref_store(the_repository),
						   "HEAD", 0, &rev, &flag);
	എങ്കിൽ (old_branch_info.path)
		old_branch_info.commit = lookup_commit_reference_gently(the_repository, &rev, 1);
	എങ്കിൽ (!(flag & REF_ISSYMREF))
		FREE_AND_NULL(old_branch_info.path);

	എങ്കിൽ (old_branch_info.path) {
		സ്ഥിരം അക്ഷരം *സ്ഥിരം prefix = "refs/heads/";
		സ്ഥിരം അക്ഷരം *p;
		എങ്കിൽ (skip_prefix(old_branch_info.path, prefix, &p))
			old_branch_info.name = xstrdup(p);
	}

	എങ്കിൽ (opts->new_orphan_branch && opts->orphan_from_empty_വൃക്ഷം) {
		എങ്കിൽ (new_branch_info->name)
			BUG("'switch --orphan' should never accept a commit as starting point");
		new_branch_info->commit = ശൂന്യ;
		new_branch_info->name = xstrdup("(empty)");
		do_merge = 1;
	}

	എങ്കിൽ (!new_branch_info->name) {
		new_branch_info->name = xstrdup("HEAD");
		new_branch_info->commit = old_branch_info.commit;
		എങ്കിൽ (!new_branch_info->commit)
			die(_("You are on a branch yet to be born"));
		parse_commit_or_die(new_branch_info->commit);

		എങ്കിൽ (opts->only_merge_on_switching_branches)
			do_merge = 0;
	}

	എങ്കിൽ (old_branch_info.name) {
		stash_label_base = old_branch_info.name;
	} അല്ലെങ്കിൽ എങ്കിൽ (old_branch_info.commit) {
		strbuf_add_unique_abbrev(&old_commit_shortname,
					 &old_branch_info.commit->object.oid,
					 DEFAULT_ABBREV);
		stash_label_base = old_commit_shortname.buf;
	}

	എങ്കിൽ (do_merge) {
		ret = merge_working_വൃക്ഷം(opts, &old_branch_info, new_branch_info,
					 opts->merge, &writeout_error);
		എങ്കിൽ (ret == MERGE_WORKING_വൃക്ഷം_UNPACK_FAILED && opts->merge) {
			strbuf_addf(&autostash_സന്ദേശം,
				    "autostash while switching to '%s'",
				    new_branch_info->name);
			create_autostash_ref(the_repository,
					     "CHECKOUT_AUTOSTASH_HEAD",
					     autostash_സന്ദേശം.buf, true);
			created_autostash = 1;
			ret = merge_working_വൃക്ഷം(opts, &old_branch_info, new_branch_info,
						 false, &writeout_error);
		}
		എങ്കിൽ (created_autostash) {
			എങ്കിൽ (opts->conflict_style >= 0) {
				ഘടന strbuf cfg = STRBUF_INIT;
				strbuf_addf(&cfg, "merge.conflictStyle=%s",
					    conflict_style_name(opts->conflict_style));
				git_config_തള്ളുക_parameter(cfg.buf);
				strbuf_release(&cfg);
			}
			autostash_res = apply_autostash_ref(the_repository,
				    "CHECKOUT_AUTOSTASH_HEAD",
				    new_branch_info->name,
				    "local",
				    stash_label_base,
				    autostash_സന്ദേശം.buf);
		}
		എങ്കിൽ (ret) {
			branch_info_release(&old_branch_info);
			strbuf_release(&old_commit_shortname);
			strbuf_release(&autostash_സന്ദേശം);
			തിരികെ ret < 0 ? 1 : ret;
		}
	}

	എങ്കിൽ (!opts->quiet && !old_branch_info.path && old_branch_info.commit && new_branch_info->commit != old_branch_info.commit)
		orphaned_commit_warning(old_branch_info.commit, new_branch_info->commit);

	എങ്കിൽ (autostash_res == STASH_APPLY_CONFLICT && !opts->quiet)
		fputc('\n', stderr);
	update_refs_for_switch(opts, &old_branch_info, new_branch_info);

	എങ്കിൽ (created_autostash) {
		discard_index(the_repository->index);
		എങ്കിൽ (repo_read_index(the_repository) < 0)
			die(_("index file corrupt"));

		എങ്കിൽ (!opts->quiet && new_branch_info->commit) {
			printf(_("The following paths have local changes:\n"));
			show_local_changes(&new_branch_info->commit->object,
					   &opts->diff_options);
		}
	}

	ret = post_checkout_hook(old_branch_info.commit, new_branch_info->commit, 1);
	branch_info_release(&old_branch_info);
	strbuf_release(&old_commit_shortname);
	strbuf_release(&autostash_സന്ദേശം);

	തിരികെ ret || writeout_error;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ git_checkout_config(സ്ഥിരം അക്ഷരം *var, സ്ഥിരം അക്ഷരം *മൂല്യം,
			       സ്ഥിരം ഘടന config_context *ctx, ശൂന്യം *cb)
{
	ഘടന checkout_opts *opts = cb;

	എങ്കിൽ (!strcmp(var, "diff.ignoresubmodules")) {
		എങ്കിൽ (!മൂല്യം)
			തിരികെ config_error_nonbool(var);
		handle_ignore_submodules_arg(&opts->diff_options, മൂല്യം);
		തിരികെ 0;
	}
	എങ്കിൽ (!strcmp(var, "checkout.guess")) {
		opts->dwim_new_local_branch = git_config_bool(var, മൂല്യം);
		തിരികെ 0;
	}

	എങ്കിൽ (starts_with(var, "submodule."))
		തിരികെ git_default_submodule_config(var, മൂല്യം, ശൂന്യ);

	തിരികെ git_xmerge_config(var, മൂല്യം, ctx, ശൂന്യ);
}

സ്ഥിതം ശൂന്യം setup_new_branch_info_and_source_വൃക്ഷം(
	ഘടന branch_info *new_branch_info,
	ഘടന checkout_opts *opts,
	ഘടന object_id *rev,
	സ്ഥിരം അക്ഷരം *arg)
{
	ഘടന വൃക്ഷം **source_വൃക്ഷം = &opts->source_വൃക്ഷം;
	ഘടന object_id branch_rev;

	/* treat '@' as a shortcut for 'HEAD' */
	new_branch_info->name = !strcmp(arg, "@") ? xstrdup("HEAD") :
						    xstrdup(arg);
	setup_branch_path(new_branch_info);

	എങ്കിൽ (!check_refname_format(new_branch_info->path, 0) &&
	    !refs_read_ref(get_main_ref_store(the_repository), new_branch_info->path, &branch_rev))
		oidcpy(rev, &branch_rev);
	അല്ലെങ്കിൽ
		/* not an existing branch */
		FREE_AND_NULL(new_branch_info->path);

	new_branch_info->commit = lookup_commit_reference_gently(the_repository, rev, 1);
	എങ്കിൽ (!new_branch_info->commit) {
		/* not a commit */
		*source_വൃക്ഷം = repo_parse_വൃക്ഷം_indirect(the_repository, rev);
		എങ്കിൽ (!*source_വൃക്ഷം)
			die(_("unable to read tree (%s)"), oid_to_hex(rev));
	} അല്ലെങ്കിൽ {
		parse_commit_or_die(new_branch_info->commit);
		*source_വൃക്ഷം = repo_get_commit_വൃക്ഷം(the_repository,
						    new_branch_info->commit);
		എങ്കിൽ (!*source_വൃക്ഷം)
			die(_("unable to read tree (%s)"),
			    oid_to_hex(&new_branch_info->commit->object.oid));
	}
}


എണ്ണൽ checkout_command {
	CHECKOUT_CHECKOUT = 1,
	CHECKOUT_SWITCH = 2,
	CHECKOUT_RESTORE = 3,
};

സ്ഥിതം ശൂന്യം advise_disambiguating_remotes(എണ്ണൽ checkout_command which_command,
					  സ്ഥിരം അക്ഷരം *branch,
					  സ്ഥിരം ഘടന string_list *matched_remote_names)
{
	സ്ഥിരം അക്ഷരം *cmdname;
	ഘടന string_list_item *item;

	തിരഞ്ഞെടുപ്പ് (which_command) {
	അവസ്ഥ CHECKOUT_CHECKOUT:
		cmdname = "checkout";
		നിർത്തുക;
	അവസ്ഥ CHECKOUT_SWITCH:
		cmdname = "switch";
		നിർത്തുക;
	സ്ഥിരസ്ഥിതി:
		BUG("command <%d> should not reach advise_disambiguating_remotes",
		    which_command);
		നിർത്തുക;
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

സ്ഥിതം അക്ഷരം *parse_remote_branch(സ്ഥിരം അക്ഷരം *arg,
				 ഘടന object_id *rev,
				 പൂർണ്ണസംഖ്യ could_be_checkout_paths,
				 എണ്ണൽ checkout_command which_command)
{
	പൂർണ്ണസംഖ്യ അക്കം_matches = 0;
	ഘടന string_list matched_remote_names = STRING_LIST_INIT_DUP;

	അക്ഷരം *remote = unique_tracking_name(arg, rev, &അക്കം_matches,
					    &matched_remote_names);

	എങ്കിൽ (remote && could_be_checkout_paths) {
		die(_("'%s' could be both a local file and a tracking branch.\n"
			"Please use -- (and optionally --no-guess) to disambiguate"),
		    arg);
	}

	എങ്കിൽ (!remote && അക്കം_matches > 1) {
		എങ്കിൽ (advice_enabled(ADVICE_CHECKOUT_AMBIGUOUS_REMOTE_BRANCH_NAME))
			advise_disambiguating_remotes(which_command, arg,
						      &matched_remote_names);
		die(_("'%s' matched multiple (%d) remote tracking branches"),
		    arg, അക്കം_matches);
	}

	string_list_clear(&matched_remote_names, 0);

	തിരികെ remote;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ parse_branchname_arg(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv,
				പൂർണ്ണസംഖ്യ dwim_new_local_branch_ok,
				എണ്ണൽ checkout_command which_command,
				ഘടന branch_info *new_branch_info,
				ഘടന checkout_opts *opts,
				ഘടന object_id *rev)
{
	സ്ഥിരം അക്ഷരം **new_branch = &opts->new_branch;
	പൂർണ്ണസംഖ്യ argcount = 0;
	സ്ഥിരം അക്ഷരം *arg;
	അക്ഷരം *remote = ശൂന്യ;
	പൂർണ്ണസംഖ്യ dash_dash_pos;
	പൂർണ്ണസംഖ്യ has_dash_dash = 0;
	പൂർണ്ണസംഖ്യ i;

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
	എങ്കിൽ (!argc)
		തിരികെ 0;

	എങ്കിൽ (!opts->accept_pathspec) {
		എങ്കിൽ (argc > 1)
			die(_("only one reference expected"));
		has_dash_dash = 1; /* helps disambiguate */
	}

	arg = argv[0];
	dash_dash_pos = -1;
	വേണ്ടി (i = 0; i < argc; i++) {
		എങ്കിൽ (opts->accept_pathspec && !strcmp(argv[i], "--")) {
			dash_dash_pos = i;
			നിർത്തുക;
		}
	}
	എങ്കിൽ (dash_dash_pos == 0)
		തിരികെ 1; /* case (2) */
	അല്ലെങ്കിൽ എങ്കിൽ (dash_dash_pos == 1)
		has_dash_dash = 1; /* case (3) or (1) */
	അല്ലെങ്കിൽ എങ്കിൽ (dash_dash_pos >= 2)
		die(_("only one reference expected, %d given."), dash_dash_pos);
	opts->എണ്ണം_checkout_paths = !opts->quiet && !has_dash_dash;

	എങ്കിൽ (!strcmp(arg, "-"))
		arg = "@{-1}";

	എങ്കിൽ (repo_get_oid_mb(the_repository, arg, rev)) {
		/*
		 * Either case (3) or (4), with <something> not being
		 * a commit, or an attempt to use case (1) with an
		 * invalid ref.
		 *
		 * It's likely an error, but we need to find out if
		 * we should auto-create the branch, case (3).(b).
		 */
		പൂർണ്ണസംഖ്യ recover_with_dwim = dwim_new_local_branch_ok;

		പൂർണ്ണസംഖ്യ could_be_checkout_paths = !has_dash_dash &&
			check_filename(opts->prefix, arg);

		എങ്കിൽ (!has_dash_dash && !no_wildcard(arg))
			recover_with_dwim = 0;

		/*
		 * Accept "git checkout foo", "git checkout foo --"
		 * and "git switch foo" as candidates for dwim.
		 */
		എങ്കിൽ (!(argc == 1 && !has_dash_dash) &&
		    !(argc == 2 && has_dash_dash) &&
		    opts->accept_pathspec)
			recover_with_dwim = 0;

		എങ്കിൽ (recover_with_dwim) {
			remote = parse_remote_branch(arg, rev,
						     could_be_checkout_paths,
						     which_command);
			എങ്കിൽ (remote) {
				*new_branch = arg;
				arg = remote;
				/* DWIMmed to create local branch, case (3).(b) */
			} അല്ലെങ്കിൽ {
				recover_with_dwim = 0;
			}
		}

		എങ്കിൽ (!recover_with_dwim) {
			എങ്കിൽ (has_dash_dash)
				die(_("invalid reference: %s"), arg);
			തിരികെ argcount;
		}
	}

	/* we can't end up being in (2) anymore, eat the argument */
	argcount++;
	argv++;
	argc--;

	setup_new_branch_info_and_source_വൃക്ഷം(new_branch_info, opts, rev, arg);

	എങ്കിൽ (!opts->source_വൃക്ഷം)                   /* case (1): want a tree */
		die(_("reference is not a tree: %s"), arg);

	എങ്കിൽ (!has_dash_dash) {	/* case (3).(d) -> (1) */
		/*
		 * Do not complain the most common case
		 *	git checkout branch
		 * even if there happen to be a file called 'branch';
		 * it would be extremely annoying.
		 */
		എങ്കിൽ (argc)
			verify_non_filename(the_repository, opts->prefix, arg);
	} അല്ലെങ്കിൽ എങ്കിൽ (opts->accept_pathspec) {
		argcount++;
		argv++;
		argc--;
	}

	സ്വതന്ത്രം(remote);
	തിരികെ argcount;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ switch_unborn_to_new_branch(സ്ഥിരം ഘടന checkout_opts *opts)
{
	പൂർണ്ണസംഖ്യ status;
	ഘടന strbuf branch_ref = STRBUF_INIT;

	trace2_cmd_mode("unborn");

	എങ്കിൽ (!opts->new_branch)
		die(_("You are on a branch yet to be born"));
	strbuf_addf(&branch_ref, "refs/heads/%s", opts->new_branch);
	status = refs_update_symref(get_main_ref_store(the_repository),
				    "HEAD", branch_ref.buf, "checkout -b");
	strbuf_release(&branch_ref);
	എങ്കിൽ (!opts->quiet)
		fprintf(stderr, _("Switched to a new branch '%s'\n"),
			opts->new_branch);
	തിരികെ status;
}

സ്ഥിതം ശൂന്യം die_expecting_a_branch(സ്ഥിരം ഘടന branch_info *branch_info)
{
	ഘടന object_id oid;
	അക്ഷരം *to_free;
	പൂർണ്ണസംഖ്യ code;

	എങ്കിൽ (repo_dwim_ref(the_repository, branch_info->name,
			  strlen(branch_info->name), &oid, &to_free, 0) == 1) {
		സ്ഥിരം അക്ഷരം *ref = to_free;

		എങ്കിൽ (skip_prefix(ref, "refs/tags/", &ref))
			code = die_message(_("a branch is expected, got tag '%s'"), ref);
		അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(ref, "refs/remotes/", &ref))
			code = die_message(_("a branch is expected, got remote branch '%s'"), ref);
		അല്ലെങ്കിൽ
			code = die_message(_("a branch is expected, got '%s'"), ref);
	}
	അല്ലെങ്കിൽ എങ്കിൽ (branch_info->commit)
		code = die_message(_("a branch is expected, got commit '%s'"), branch_info->name);
	അല്ലെങ്കിൽ
		/*
		 * This case should never happen because we already die() on
		 * non-commit, but just in case.
		 */
		code = die_message(_("a branch is expected, got '%s'"), branch_info->name);

	എങ്കിൽ (advice_enabled(ADVICE_SUGGEST_DETACHING_HEAD))
		advise(_("If you want to detach HEAD at the commit, try again with the --detach option."));

	exit(code);
}

സ്ഥിതം ശൂന്യം die_if_some_operation_in_progress(ശൂന്യം)
{
	ഘടന wt_status_state state;

	memset(&state, 0, വലുപ്പം(state));
	wt_status_get_state(the_repository, &state, 0);

	എങ്കിൽ (state.merge_in_progress)
		die(_("cannot switch branch while merging\n"
		      "Consider \"git merge --quit\" "
		      "or \"git worktree add\"."));
	എങ്കിൽ (state.am_in_progress)
		die(_("cannot switch branch in the middle of an am session\n"
		      "Consider \"git am --quit\" "
		      "or \"git worktree add\"."));
	എങ്കിൽ (state.rebase_interactive_in_progress || state.rebase_in_progress)
		die(_("cannot switch branch while rebasing\n"
		      "Consider \"git rebase --quit\" "
		      "or \"git worktree add\"."));
	എങ്കിൽ (state.cherry_pick_in_progress)
		die(_("cannot switch branch while cherry-picking\n"
		      "Consider \"git cherry-pick --quit\" "
		      "or \"git worktree add\"."));
	എങ്കിൽ (state.revert_in_progress)
		die(_("cannot switch branch while reverting\n"
		      "Consider \"git revert --quit\" "
		      "or \"git worktree add\"."));
	എങ്കിൽ (state.bisect_in_progress)
		warning(_("you are switching branch while bisecting"));

	wt_status_state_free_buffers(&state);
}

/*
 * die if attempting to checkout an existing branch that is in use
 * in another worktree, unless ignore-other-wortrees option is given.
 * The check is bypassed when the branch is already the current one,
 * as it will not make things any worse.
 */
സ്ഥിതം ശൂന്യം die_if_switching_to_a_branch_in_use(ഘടന checkout_opts *opts,
						സ്ഥിരം അക്ഷരം *full_ref)
{
	പൂർണ്ണസംഖ്യ flags;
	അക്ഷരം *head_ref;

	എങ്കിൽ (opts->ignore_other_worktrees)
		തിരികെ;
	head_ref = refs_resolve_refdup(get_main_ref_store(the_repository),
				       "HEAD", 0, ശൂന്യ, &flags);
	എങ്കിൽ (head_ref && (!(flags & REF_ISSYMREF) || strcmp(head_ref, full_ref)))
		die_if_checked_out(full_ref, 1);
	സ്വതന്ത്രം(head_ref);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ checkout_branch(ഘടന checkout_opts *opts,
			   ഘടന branch_info *new_branch_info)
{
	ഘടന repo_config_മൂല്യങ്ങൾ *cfg = repo_config_മൂല്യങ്ങൾ(the_repository);
	പൂർണ്ണസംഖ്യ noop_switch = (!new_branch_info->name &&
			   !opts->new_branch &&
			   !opts->force_detach);

	എങ്കിൽ (opts->pathspec.nr)
		die(_("paths cannot be used with switching branches"));

	എങ്കിൽ (opts->patch_mode)
		die(_("'%s' cannot be used with switching branches"),
		    "--patch");

	എങ്കിൽ (opts->overlay_mode != -1)
		die(_("'%s' cannot be used with switching branches"),
		    "--[no]-overlay");

	എങ്കിൽ (opts->writeout_stage) {
		സ്ഥിരം അക്ഷരം *സന്ദേശം;
		എങ്കിൽ (noop_switch)
			സന്ദേശം = _("'%s' needs the paths to check out");
		അല്ലെങ്കിൽ
			സന്ദേശം = _("'%s' cannot be used with switching branches");
		die(സന്ദേശം, "--ours/--theirs");
	}

	എങ്കിൽ (opts->force && opts->merge)
		die(_("'%s' cannot be used with '%s'"), "-f", "-m");

	എങ്കിൽ (opts->discard_changes && opts->merge)
		die(_("'%s' cannot be used with '%s'"), "--discard-changes", "--merge");

	എങ്കിൽ (opts->force_detach && opts->new_branch)
		die(_("'%s' cannot be used with '%s'"),
		    "--detach", "-b/-B/--orphan");

	എങ്കിൽ (opts->new_orphan_branch) {
		എങ്കിൽ (opts->track != BRANCH_TRACK_UNSPECIFIED)
			die(_("'%s' cannot be used with '%s'"), "--orphan", "-t");
		എങ്കിൽ (opts->orphan_from_empty_വൃക്ഷം && new_branch_info->name)
			die(_("'%s' cannot take <start-point>"), "--orphan");
	} അല്ലെങ്കിൽ എങ്കിൽ (opts->force_detach) {
		എങ്കിൽ (opts->track != BRANCH_TRACK_UNSPECIFIED)
			die(_("'%s' cannot be used with '%s'"), "--detach", "-t");
	} അല്ലെങ്കിൽ എങ്കിൽ (opts->track == BRANCH_TRACK_UNSPECIFIED)
		opts->track = cfg->branch_track;

	എങ്കിൽ (new_branch_info->name && !new_branch_info->commit)
		die(_("Cannot switch branch to a non-commit '%s'"),
		    new_branch_info->name);

	എങ്കിൽ (noop_switch &&
	    !opts->switch_branch_doing_nothing_is_ok)
		die(_("missing branch or commit argument"));

	എങ്കിൽ (!opts->implicit_detach &&
	    !opts->force_detach &&
	    !opts->new_branch &&
	    !opts->new_branch_force &&
	    new_branch_info->name &&
	    !new_branch_info->path)
		die_expecting_a_branch(new_branch_info);

	എങ്കിൽ (!opts->can_switch_when_in_progress)
		die_if_some_operation_in_progress();

	/* "git checkout <branch>" */
	എങ്കിൽ (new_branch_info->path && !opts->force_detach && !opts->new_branch)
		die_if_switching_to_a_branch_in_use(opts, new_branch_info->path);

	/* "git checkout -B <branch>" */
	എങ്കിൽ (opts->new_branch_force) {
		അക്ഷരം *full_ref = xstrfmt("refs/heads/%s", opts->new_branch);
		die_if_switching_to_a_branch_in_use(opts, full_ref);
		സ്വതന്ത്രം(full_ref);
	}

	എങ്കിൽ (!new_branch_info->commit && opts->new_branch) {
		ഘടന object_id rev;
		പൂർണ്ണസംഖ്യ flag;

		എങ്കിൽ (!refs_read_ref_full(get_main_ref_store(the_repository), "HEAD", 0, &rev, &flag) &&
		    (flag & REF_ISSYMREF) && is_null_oid(&rev))
			തിരികെ switch_unborn_to_new_branch(opts);
	}
	തിരികെ switch_branches(opts, new_branch_info);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ parse_opt_conflict(സ്ഥിരം ഘടന option *o, സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	ഘടന checkout_opts *opts = o->മൂല്യം;

	എങ്കിൽ (unset) {
		opts->conflict_style = -1;
		തിരികെ 0;
	}
	opts->conflict_style = parse_conflict_style_name(arg);
	എങ്കിൽ (opts->conflict_style < 0)
		തിരികെ error(_("unknown conflict style '%s'"), arg);
	/* --conflict overrides a previous --no-merge */
	എങ്കിൽ (!opts->merge)
		opts->merge = -1;

	തിരികെ 0;
}

സ്ഥിതം ഘടന option *add_common_options(ഘടന checkout_opts *opts,
					 ഘടന option *prevopts)
{
	ഘടന option options[] = {
		OPT__QUIET(&opts->quiet, N_("suppress progress reporting")),
		OPT_CALLBACK_F(0, "recurse-submodules", ശൂന്യ,
			    "checkout", "control recursive updating of submodules",
			    PARSE_OPT_OPTARG, option_parse_recurse_submodules_worktree_updater),
		OPT_BOOL(0, "progress", &opts->show_progress, N_("force progress reporting")),
		OPT_BOOL('m', "merge", &opts->merge, N_("perform a 3-way merge with the new branch")),
		OPT_CALLBACK(0, "conflict", opts, N_("style"),
			     N_("conflict style (merge, diff3, or zdiff3)"),
			     parse_opt_conflict),
		OPT_END()
	};
	ഘടന option *newopts = parse_options_concat(prevopts, options);
	സ്വതന്ത്രം(prevopts);
	തിരികെ newopts;
}

സ്ഥിതം ഘടന option *add_common_switch_branch_options(
	ഘടന checkout_opts *opts, ഘടന option *prevopts)
{
	ഘടന option options[] = {
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
	ഘടന option *newopts = parse_options_concat(prevopts, options);
	സ്വതന്ത്രം(prevopts);
	തിരികെ newopts;
}

സ്ഥിതം ഘടന option *add_checkout_path_options(ഘടന checkout_opts *opts,
						ഘടന option *prevopts)
{
	ഘടന option options[] = {
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
	ഘടന option *newopts = parse_options_concat(prevopts, options);
	സ്വതന്ത്രം(prevopts);
	തിരികെ newopts;
}

/* create-branch option (either b or c) */
സ്ഥിതം അക്ഷരം cb_option = 'b';

സ്ഥിതം പൂർണ്ണസംഖ്യ checkout_main(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
			 ഘടന checkout_opts *opts, ഘടന option *options,
			 എണ്ണൽ checkout_command which_command)
{
	പൂർണ്ണസംഖ്യ parseopt_flags = 0;
	ഘടന branch_info new_branch_info = { 0 };
	പൂർണ്ണസംഖ്യ ret;

	സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം checkout_usage[] = {
		N_("git checkout [<options>] <branch>"),
		N_("git checkout [<options>] [<branch>] -- <file>..."),
		ശൂന്യ,
	};

	സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം switch_branch_usage[] = {
		N_("git switch [<options>] [<branch>]"),
		ശൂന്യ,
	};

	സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം restore_usage[] = {
		N_("git restore [<options>] [--source=<branch>] <file>..."),
		ശൂന്യ,
	};

	സ്ഥിരം അക്ഷരം * സ്ഥിരം *usagestr;

	തിരഞ്ഞെടുപ്പ് (which_command) {
	അവസ്ഥ CHECKOUT_CHECKOUT:
		usagestr = checkout_usage;
		നിർത്തുക;
	അവസ്ഥ CHECKOUT_SWITCH:
		usagestr = switch_branch_usage;
		നിർത്തുക;
	അവസ്ഥ CHECKOUT_RESTORE:
		usagestr = restore_usage;
		നിർത്തുക;
	സ്ഥിരസ്ഥിതി:
		BUG("no such checkout variant %d", which_command);
	}

	opts->overwrite_ignore = 1;
	opts->prefix = prefix;
	opts->show_progress = -1;

	repo_config(the_repository, git_checkout_config, opts);
	എങ്കിൽ (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_full_index = 0;
	}

	opts->track = BRANCH_TRACK_UNSPECIFIED;

	എങ്കിൽ (!opts->accept_pathspec && !opts->accept_ref)
		BUG("make up your mind, you need to take _something_");
	എങ്കിൽ (opts->accept_pathspec && opts->accept_ref)
		parseopt_flags = PARSE_OPT_KEEP_DASHDASH;

	argc = parse_options(argc, argv, prefix, options,
			     usagestr, parseopt_flags);

	എങ്കിൽ (opts->patch_context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	എങ്കിൽ (opts->patch_interhunk_context < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	എങ്കിൽ (!opts->patch_mode) {
		എങ്കിൽ (opts->patch_context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--patch");
		എങ്കിൽ (opts->patch_interhunk_context != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--patch");
		എങ്കിൽ (!opts->auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--patch");
	}

	എങ്കിൽ (opts->show_progress < 0) {
		എങ്കിൽ (opts->quiet)
			opts->show_progress = 0;
		അല്ലെങ്കിൽ
			opts->show_progress = isatty(2);
	}

	/* --conflicts implies --merge */
	എങ്കിൽ (opts->merge == -1)
		opts->merge = opts->conflict_style >= 0;

	എങ്കിൽ (opts->force) {
		opts->discard_changes = 1;
		opts->ignore_unmerged_opt = "--force";
		opts->ignore_unmerged = 1;
	}

	എങ്കിൽ ((!!opts->new_branch + !!opts->new_branch_force + !!opts->new_orphan_branch) > 1)
		die(_("options '-%c', '-%c', and '%s' cannot be used together"),
			cb_option, toupper(cb_option), "--orphan");

	എങ്കിൽ (opts->overlay_mode == 1 && opts->patch_mode)
		die(_("options '%s' and '%s' cannot be used together"), "-p", "--overlay");

	എങ്കിൽ (opts->checkout_index >= 0 || opts->checkout_worktree >= 0) {
		എങ്കിൽ (opts->checkout_index < 0)
			opts->checkout_index = 0;
		എങ്കിൽ (opts->checkout_worktree < 0)
			opts->checkout_worktree = 0;
	} അല്ലെങ്കിൽ {
		എങ്കിൽ (opts->checkout_index < 0)
			opts->checkout_index = -opts->checkout_index - 1;
		എങ്കിൽ (opts->checkout_worktree < 0)
			opts->checkout_worktree = -opts->checkout_worktree - 1;
	}
	എങ്കിൽ (opts->checkout_index < 0 || opts->checkout_worktree < 0)
		BUG("these flags should be non-negative by now");
	/*
	 * convenient shortcut: "git restore --staged [--worktree]" equals
	 * "git restore --staged [--worktree] --source HEAD"
	 */
	എങ്കിൽ (!opts->from_treeish && opts->checkout_index)
		opts->from_treeish = "HEAD";

	/*
	 * From here on, new_branch will contain the branch to be checked out,
	 * and new_branch_force and new_orphan_branch will tell us which one of
	 * -b/-B/-c/-C/--orphan is being used.
	 */
	എങ്കിൽ (opts->new_branch_force)
		opts->new_branch = opts->new_branch_force;

	എങ്കിൽ (opts->new_orphan_branch)
		opts->new_branch = opts->new_orphan_branch;

	/* --track without -c/-C/-b/-B/--orphan should DWIM */
	എങ്കിൽ (opts->track != BRANCH_TRACK_UNSPECIFIED && !opts->new_branch) {
		സ്ഥിരം അക്ഷരം *argv0 = argv[0];
		എങ്കിൽ (!argc || !strcmp(argv0, "--"))
			die(_("--track needs a branch name"));
		skip_prefix(argv0, "refs/", &argv0);
		skip_prefix(argv0, "remotes/", &argv0);
		argv0 = strchr(argv0, '/');
		എങ്കിൽ (!argv0 || !argv0[1])
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
	എങ്കിൽ (argc && opts->accept_ref) {
		ഘടന object_id rev;
		പൂർണ്ണസംഖ്യ dwim_ok =
			!opts->patch_mode &&
			opts->dwim_new_local_branch &&
			opts->track == BRANCH_TRACK_UNSPECIFIED &&
			!opts->new_branch;
		പൂർണ്ണസംഖ്യ n = parse_branchname_arg(argc, argv, dwim_ok, which_command,
					     &new_branch_info, opts, &rev);
		argv += n;
		argc -= n;
	} അല്ലെങ്കിൽ എങ്കിൽ (!opts->accept_ref && opts->from_treeish) {
		ഘടന object_id rev;

		എങ്കിൽ (repo_get_oid_mb(the_repository, opts->from_treeish, &rev))
			die(_("could not resolve '%s'"), opts->from_treeish);

		setup_new_branch_info_and_source_വൃക്ഷം(&new_branch_info,
						      opts, &rev,
						      opts->from_treeish);

		എങ്കിൽ (!opts->source_വൃക്ഷം)
			die(_("reference is not a tree: %s"), opts->from_treeish);
	}

	എങ്കിൽ (argc) {
		parse_pathspec(&opts->pathspec, 0,
			       opts->patch_mode ? PATHSPEC_PREFIX_ORIGIN : 0,
			       prefix, argv);

		എങ്കിൽ (!opts->pathspec.nr)
			die(_("invalid path specification"));

		/*
		 * Try to give more helpful suggestion.
		 * new_branch && argc > 1 will be caught later.
		 */
		എങ്കിൽ (opts->new_branch && argc == 1 && !new_branch_info.commit)
			die(_("'%s' is not a commit and a branch '%s' cannot be created from it"),
				argv[0], opts->new_branch);

		എങ്കിൽ (opts->force_detach)
			die(_("git checkout: --detach does not take a path argument '%s'"),
			    argv[0]);
	}

	എങ്കിൽ (opts->pathspec_from_file) {
		എങ്കിൽ (opts->pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		എങ്കിൽ (opts->force_detach)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--detach");

		എങ്കിൽ (opts->patch_mode)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--patch");

		parse_pathspec_file(&opts->pathspec, 0,
				    0,
				    prefix, opts->pathspec_from_file, opts->pathspec_file_nul);
	} അല്ലെങ്കിൽ എങ്കിൽ (opts->pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	opts->pathspec.recursive = 1;

	എങ്കിൽ (opts->pathspec.nr) {
		എങ്കിൽ (1 < !!opts->writeout_stage + !!opts->force + !!opts->merge)
			die(_("git checkout: --ours/--theirs, --force and --merge are incompatible when\n"
			      "checking out of the index."));
	} അല്ലെങ്കിൽ {
		എങ്കിൽ (opts->accept_pathspec && !opts->empty_pathspec_ok &&
		    !opts->patch_mode)	/* patch mode is special */
			die(_("you must specify path(s) to restore"));
	}

	എങ്കിൽ (opts->new_branch) {
		ഘടന strbuf buf = STRBUF_INIT;

		എങ്കിൽ (opts->new_branch_force)
			opts->branch_exists = validate_branchname(opts->new_branch, &buf);
		അല്ലെങ്കിൽ
			opts->branch_exists =
				validate_new_branchname(opts->new_branch, &buf, 0);
		strbuf_release(&buf);
	}

	എങ്കിൽ (opts->patch_mode || opts->pathspec.nr)
		ret = checkout_paths(opts, &new_branch_info);
	അല്ലെങ്കിൽ
		ret = checkout_branch(opts, &new_branch_info);

	branch_info_release(&new_branch_info);
	clear_pathspec(&opts->pathspec);
	സ്വതന്ത്രം(opts->pathspec_from_file);
	സ്വതന്ത്രം(options);

	തിരികെ ret;
}

പൂർണ്ണസംഖ്യ cmd_checkout(പൂർണ്ണസംഖ്യ argc,
		 സ്ഥിരം അക്ഷരം **argv,
		 സ്ഥിരം അക്ഷരം *prefix,
		 ഘടന repository *repo UNUSED)
{
	ഘടന checkout_opts opts = CHECKOUT_OPTS_INIT;
	ഘടന option *options;
	ഘടന option checkout_options[] = {
		OPT_STRING('b', ശൂന്യ, &opts.new_branch, N_("branch"),
			   N_("create and checkout a new branch")),
		OPT_STRING('B', ശൂന്യ, &opts.new_branch_force, N_("branch"),
			   N_("create/reset and checkout a branch")),
		OPT_BOOL('l', ശൂന്യ, &opts.new_branch_log, N_("create reflog for new branch")),
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
	opts.orphan_from_empty_വൃക്ഷം = 0;
	opts.empty_pathspec_ok = 1;
	opts.overlay_mode = -1;
	opts.checkout_index = -2;    /* default on */
	opts.checkout_worktree = -2; /* default on */

	എങ്കിൽ (argc == 3 && !strcmp(argv[1], "-b")) {
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

	തിരികെ checkout_main(argc, argv, prefix, &opts, options,
			     CHECKOUT_CHECKOUT);
}

പൂർണ്ണസംഖ്യ cmd_switch(പൂർണ്ണസംഖ്യ argc,
	       സ്ഥിരം അക്ഷരം **argv,
	       സ്ഥിരം അക്ഷരം *prefix,
	       ഘടന repository *repo UNUSED)
{
	ഘടന checkout_opts opts = CHECKOUT_OPTS_INIT;
	ഘടന option *options = ശൂന്യ;
	ഘടന option switch_options[] = {
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
	opts.orphan_from_empty_വൃക്ഷം = 1;
	opts.overlay_mode = -1;

	options = parse_options_dup(switch_options);
	options = add_common_options(&opts, options);
	options = add_common_switch_branch_options(&opts, options);

	cb_option = 'c';

	തിരികെ checkout_main(argc, argv, prefix, &opts, options,
			     CHECKOUT_SWITCH);
}

പൂർണ്ണസംഖ്യ cmd_restore(പൂർണ്ണസംഖ്യ argc,
		സ്ഥിരം അക്ഷരം **argv,
		സ്ഥിരം അക്ഷരം *prefix,
		ഘടന repository *repo UNUSED)
{
	ഘടന checkout_opts opts = CHECKOUT_OPTS_INIT;
	ഘടന option *options;
	ഘടന option restore_options[] = {
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

	തിരികെ checkout_main(argc, argv, prefix, &opts, options,
			     CHECKOUT_RESTORE);
}
