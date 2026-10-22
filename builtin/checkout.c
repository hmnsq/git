#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "advice.h"
#অন্তর্ভাব "branch.h"
#অন্তর্ভাব "cache-tree.h"
#অন্তর্ভাব "checkout.h"
#অন্তর্ভাব "commit.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "diff.h"
#অন্তর্ভাব "dir.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "hex.h"
#অন্তর্ভাব "hook.h"
#অন্তর্ভাব "merge-ll.h"
#অন্তর্ভাব "lockfile.h"
#অন্তর্ভাব "mem-pool.h"
#অন্তর্ভাব "object-file.h"
#অন্তর্ভাব "object-name.h"
#অন্তর্ভাব "odb.h"
#অন্তর্ভাব "parse-options.h"
#অন্তর্ভাব "path.h"
#অন্তর্ভাব "preload-index.h"
#অন্তর্ভাব "read-cache.h"
#অন্তর্ভাব "refs.h"
#অন্তর্ভাব "remote.h"
#অন্তর্ভাব "repo-settings.h"
#অন্তর্ভাব "resolve-undo.h"
#অন্তর্ভাব "revision.h"
#অন্তর্ভাব "sequencer.h"
#অন্তর্ভাব "setup.h"
#অন্তর্ভাব "sparse-index.h"
#অন্তর্ভাব "strvec.h"
#অন্তর্ভাব "submodule.h"
#অন্তর্ভাব "symlinks.h"
#অন্তর্ভাব "trace2.h"
#অন্তর্ভাব "tree.h"
#অন্তর্ভাব "tree-walk.h"
#অন্তর্ভাব "unpack-trees.h"
#অন্তর্ভাব "wt-status.h"
#অন্তর্ভাব "xdiff-interface.h"
#অন্তর্ভাব "entry.h"
#অন্তর্ভাব "parallel-checkout.h"
#অন্তর্ভাব "add-interactive.h"

সংরচনা checkout_opts {
	পূর্ণাঙ্ক patch_mode;
	পূর্ণাঙ্ক patch_context;
	পূর্ণাঙ্ক patch_interhunk_context;
	পূর্ণাঙ্ক auto_advance;
	পূর্ণাঙ্ক quiet;
	পূর্ণাঙ্ক merge;
	পূর্ণাঙ্ক force;
	পূর্ণাঙ্ক force_detach;
	পূর্ণাঙ্ক implicit_detach;
	পূর্ণাঙ্ক writeout_stage;
	পূর্ণাঙ্ক overwrite_ignore;
	পূর্ণাঙ্ক ignore_skipworktree;
	পূর্ণাঙ্ক ignore_other_worktrees;
	পূর্ণাঙ্ক show_progress;
	পূর্ণাঙ্ক গুনতি_checkout_paths;
	পূর্ণাঙ্ক overlay_mode;
	পূর্ণাঙ্ক dwim_new_local_branch;
	পূর্ণাঙ্ক discard_changes;
	পূর্ণাঙ্ক accept_ref;
	পূর্ণাঙ্ক accept_pathspec;
	পূর্ণাঙ্ক switch_branch_doing_nothing_is_ok;
	পূর্ণাঙ্ক only_merge_on_switching_branches;
	পূর্ণাঙ্ক can_switch_when_in_progress;
	পূর্ণাঙ্ক orphan_from_empty_বৃক্ষ;
	পূর্ণাঙ্ক empty_pathspec_ok;
	পূর্ণাঙ্ক checkout_index;
	পূর্ণাঙ্ক checkout_worktree;
	ধ্রুব বর্ণ *ignore_unmerged_opt;
	পূর্ণাঙ্ক ignore_unmerged;
	পূর্ণাঙ্ক pathspec_file_nul;
	বর্ণ *pathspec_from_file;

	ধ্রুব বর্ণ *new_branch;
	ধ্রুব বর্ণ *new_branch_force;
	ধ্রুব বর্ণ *new_orphan_branch;
	পূর্ণাঙ্ক new_branch_log;
	গণনা branch_track track;
	সংরচনা diff_options diff_options;
	পূর্ণাঙ্ক conflict_style;

	পূর্ণাঙ্ক branch_exists;
	ধ্রুব বর্ণ *prefix;
	সংরচনা pathspec pathspec;
	ধ্রুব বর্ণ *from_treeish;
	সংরচনা বৃক্ষ *source_বৃক্ষ;
};

#define CHECKOUT_OPTS_INIT { \
	.conflict_style = -1, \
	.merge = -1, \
	.patch_context = -1, \
	.patch_interhunk_context = -1, \
	.auto_advance = 1, \
}

#define MERGE_WORKING_বৃক্ষ_UNPACK_FAILED (-2)

সংরচনা branch_info {
	বর্ণ *name; /* The short name used */
	বর্ণ *path; /* The full name of a real branch */
	সংরচনা commit *commit; /* The named commit */
	বর্ণ *refname; /* The full name of the ref being checked out. */
	সংরচনা object_id oid; /* The object ID of the commit being checked out. */
	/*
	 * if not null the branch is detached because it's already
	 * checked out in this checkout
	 */
	বর্ণ *checkout;
};

স্থির শূন্য branch_info_release(সংরচনা branch_info *info)
{
	মুক্ত(info->name);
	মুক্ত(info->path);
	মুক্ত(info->refname);
	মুক্ত(info->checkout);
}

স্থির পূর্ণাঙ্ক post_checkout_hook(সংরচনা commit *old_commit, সংরচনা commit *new_commit,
			      পূর্ণাঙ্ক changed)
{
	সংরচনা run_hooks_opt opt = RUN_HOOKS_OPT_INIT_FORCE_SERIAL;

	/*
	 * "new_commit" can be NULL when checking out from the index before
	 * a commit exists.
	 */
	strvec_pushl(&opt.args,
		     oid_to_hex(old_commit ? &old_commit->object.oid : null_oid(the_hash_algo)),
		     oid_to_hex(new_commit ? &new_commit->object.oid : null_oid(the_hash_algo)),
		     changed ? "1" : "0",
		     রিক্ত);

	প্রত্যাগম run_hooks_opt(the_repository, "post-checkout", &opt);
}

/*
 * Handle a tree object and determine if we need to recurse into the
 * tree (READ_TREE_RECURSIVE) or skip it (0).
 */
স্থির পূর্ণাঙ্ক try_update_sparse_directory(ধ্রুব সংরচনা object_id *oid,
				       সংরচনা strbuf *base,
				       ধ্রুব বর্ণ *pathname,
				       পূর্ণাঙ্ক overlay_mode)
{
	সংরচনা strbuf dirpath = STRBUF_INIT;
	সংরচনা cache_entry *old;
	পূর্ণাঙ্ক pos, result = READ_বৃক্ষ_RECURSIVE;

	যদি (!the_repository->index->sparse_index)
		প্রত্যাগম result;

	strbuf_addbuf(&dirpath, base);
	strbuf_addstr(&dirpath, pathname);
	strbuf_addch(&dirpath, '/');

	pos = index_name_pos_sparse(the_repository->index,
				    dirpath.buf, dirpath.len);
	যদি (pos < 0)
		গচ্ছ cleanup;

	old = the_repository->index->cache[pos];
	যদি (!S_ISSPARSEDIR(old->ce_mode))
		গচ্ছ cleanup;

	যদি (oideq(oid, &old->oid)) {
		/* Tree content already matches; no need to descend. */
		result = 0;
	} অন্যথা যদি (!overlay_mode) {
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
	প্রত্যাগম result;
}

স্থির পূর্ণাঙ্ক update_some(ধ্রুব সংরচনা object_id *oid, সংরচনা strbuf *base,
		       ধ্রুব বর্ণ *pathname, unsigned mode, শূন্য *context)
{
	পূর্ণাঙ্ক len;
	সংরচনা cache_entry *ce;
	পূর্ণাঙ্ক pos;
	পূর্ণাঙ্ক overlay_mode = context ? *((পূর্ণাঙ্ক *)context) : 1;

	যদি (S_ISDIR(mode))
		প্রত্যাগম try_update_sparse_directory(oid, base, pathname,
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
	যদি (pos >= 0) {
		সংরচনা cache_entry *old = the_repository->index->cache[pos];
		যদি (ce->ce_mode == old->ce_mode &&
		    !ce_intent_to_add(old) &&
		    oideq(&ce->oid, &old->oid)) {
			old->ce_flags |= CE_UPDATE;
			discard_cache_entry(ce);
			প্রত্যাগম 0;
		}
	}

	add_index_entry(the_repository->index, ce,
			ADD_CACHE_OK_TO_ADD | ADD_CACHE_OK_TO_REPLACE);
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক read_বৃক্ষ_some(সংরচনা বৃক্ষ *বৃক্ষ, ধ্রুব সংরচনা pathspec *pathspec,
			  পূর্ণাঙ্ক overlay_mode)
{
	read_বৃক্ষ(the_repository, বৃক্ষ,
		  pathspec, update_some, &overlay_mode);

	/* update the index with the given tree's info
	 * for all args, expanding wildcards, and exit
	 * with any non-zero return code.
	 */
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক skip_same_name(ধ্রুব সংরচনা cache_entry *ce, পূর্ণাঙ্ক pos)
{
	যাবত্ (++pos < the_repository->index->cache_nr &&
	       !strcmp(the_repository->index->cache[pos]->name, ce->name))
		; /* skip */
	প্রত্যাগম pos;
}

স্থির পূর্ণাঙ্ক check_stage(পূর্ণাঙ্ক stage, ধ্রুব সংরচনা cache_entry *ce, পূর্ণাঙ্ক pos,
		       পূর্ণাঙ্ক overlay_mode)
{
	যাবত্ (pos < the_repository->index->cache_nr &&
	       !strcmp(the_repository->index->cache[pos]->name, ce->name)) {
		যদি (ce_stage(the_repository->index->cache[pos]) == stage)
			প্রত্যাগম 0;
		pos++;
	}
	যদি (!overlay_mode)
		প্রত্যাগম 0;
	যদি (stage == 2)
		প্রত্যাগম error(_("path '%s' does not have our version"), ce->name);
	অন্যথা
		প্রত্যাগম error(_("path '%s' does not have their version"), ce->name);
}

স্থির পূর্ণাঙ্ক check_stages(unsigned stages, ধ্রুব সংরচনা cache_entry *ce, পূর্ণাঙ্ক pos)
{
	unsigned seen = 0;
	ধ্রুব বর্ণ *name = ce->name;

	যাবত্ (pos < the_repository->index->cache_nr) {
		ce = the_repository->index->cache[pos];
		যদি (strcmp(name, ce->name))
			ভঙ্গ;
		seen |= (1 << ce_stage(ce));
		pos++;
	}
	যদি ((stages & seen) != stages)
		প্রত্যাগম error(_("path '%s' does not have all necessary versions"),
			     name);
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক checkout_stage(পূর্ণাঙ্ক stage, ধ্রুব সংরচনা cache_entry *ce, পূর্ণাঙ্ক pos,
			  ধ্রুব সংরচনা checkout *state, পূর্ণাঙ্ক *nr_checkouts,
			  পূর্ণাঙ্ক overlay_mode)
{
	যাবত্ (pos < the_repository->index->cache_nr &&
	       !strcmp(the_repository->index->cache[pos]->name, ce->name)) {
		যদি (ce_stage(the_repository->index->cache[pos]) == stage)
			প্রত্যাগম checkout_entry(the_repository->index->cache[pos], state,
					      রিক্ত, nr_checkouts);
		pos++;
	}
	যদি (!overlay_mode) {
		unlink_entry(ce, রিক্ত);
		প্রত্যাগম 0;
	}
	যদি (stage == 2)
		প্রত্যাগম error(_("path '%s' does not have our version"), ce->name);
	অন্যথা
		প্রত্যাগম error(_("path '%s' does not have their version"), ce->name);
}

স্থির পূর্ণাঙ্ক checkout_merged(পূর্ণাঙ্ক pos, ধ্রুব সংরচনা checkout *state,
			   পূর্ণাঙ্ক *nr_checkouts, সংরচনা mem_pool *ce_mem_pool,
			   পূর্ণাঙ্ক conflict_style)
{
	সংরচনা cache_entry *ce = the_repository->index->cache[pos];
	ধ্রুব বর্ণ *path = ce->name;
	mmfile_t ancestor, ours, theirs;
	গণনা ll_merge_result merge_status;
	পূর্ণাঙ্ক status;
	সংরচনা object_id oid;
	mmfile_t result_buf;
	সংরচনা object_id threeway[3];
	unsigned mode = 0;
	সংরচনা ll_merge_options ll_opts = LL_MERGE_OPTIONS_INIT;
	পূর্ণাঙ্ক renormalize = 0;

	memset(threeway, 0, পরিমাণ(threeway));
	যাবত্ (pos < the_repository->index->cache_nr) {
		পূর্ণাঙ্ক stage;
		stage = ce_stage(ce);
		যদি (!stage || strcmp(path, ce->name))
			ভঙ্গ;
		oidcpy(&threeway[stage - 1], &ce->oid);
		যদি (stage == 2)
			mode = create_ce_mode(ce->ce_mode);
		pos++;
		ce = the_repository->index->cache[pos];
	}
	যদি (is_null_oid(&threeway[1]) || is_null_oid(&threeway[2]))
		প্রত্যাগম error(_("path '%s' does not have necessary versions"), path);

	read_mmblob(&ancestor, the_repository->objects, &threeway[0]);
	read_mmblob(&ours, the_repository->objects, &threeway[1]);
	read_mmblob(&theirs, the_repository->objects, &threeway[2]);

	repo_config_get_bool(the_repository, "merge.renormalize", &renormalize);
	ll_opts.renormalize = renormalize;
	ll_opts.conflict_style = conflict_style;
	merge_status = ll_merge(&result_buf, path, &ancestor, "base",
				&ours, "ours", &theirs, "theirs",
				state->istate, &ll_opts);
	মুক্ত(ancestor.ptr);
	মুক্ত(ours.ptr);
	মুক্ত(theirs.ptr);
	যদি (merge_status == LL_MERGE_দ্বিমিক_CONFLICT)
		warning("Cannot merge binary files: %s (%s vs. %s)",
			path, "ours", "theirs");
	যদি (merge_status < 0 || !result_buf.ptr) {
		মুক্ত(result_buf.ptr);
		প্রত্যাগম error(_("path '%s': cannot merge"), path);
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
	যদি (odb_write_object(the_repository->objects, result_buf.ptr, result_buf.size, OBJ_BLOB, &oid))
		die(_("Unable to add merge result for '%s'"), path);
	মুক্ত(result_buf.ptr);
	ce = make_transient_cache_entry(mode, &oid, path, 2, ce_mem_pool);
	যদি (!ce)
		die(_("make_cache_entry failed for path '%s'"), path);
	status = checkout_entry(ce, state, রিক্ত, nr_checkouts);
	প্রত্যাগম status;
}

স্থির শূন্য mark_ce_for_checkout_overlay(সংরচনা cache_entry *ce,
					 বর্ণ *ps_matched,
					 ধ্রুব সংরচনা checkout_opts *opts)
{
	ce->ce_flags &= ~CE_MATCHED;
	যদি (!opts->ignore_skipworktree && ce_skip_worktree(ce))
		প্রত্যাগম;
	যদি (opts->source_বৃক্ষ && !(ce->ce_flags & CE_UPDATE))
		/*
		 * "git checkout tree-ish -- path", but this entry
		 * is in the original index but is not in tree-ish
		 * or does not match the pathspec; it will not be
		 * checked out to the working tree.  We will not do
		 * anything to this entry at all.
		 */
		প্রত্যাগম;
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
	যদি (ce_path_match(the_repository->index, ce, &opts->pathspec, ps_matched))
		ce->ce_flags |= CE_MATCHED;
}

স্থির শূন্য mark_ce_for_checkout_no_overlay(সংরচনা cache_entry *ce,
					    বর্ণ *ps_matched,
					    ধ্রুব সংরচনা checkout_opts *opts)
{
	ce->ce_flags &= ~CE_MATCHED;
	যদি (!opts->ignore_skipworktree && ce_skip_worktree(ce))
		প্রত্যাগম;
	যদি (ce_path_match(the_repository->index, ce, &opts->pathspec, ps_matched)) {
		ce->ce_flags |= CE_MATCHED;
		যদি (opts->source_বৃক্ষ && !(ce->ce_flags & CE_UPDATE))
			/*
			 * In overlay mode, but the path is not in
			 * tree-ish, which means we should remove it
			 * from the index and the working tree.
			 */
			ce->ce_flags |= CE_REMOVE | CE_WT_REMOVE;
	}
}

স্থির পূর্ণাঙ্ক checkout_worktree(ধ্রুব সংরচনা checkout_opts *opts,
			     ধ্রুব সংরচনা branch_info *info)
{
	সংরচনা checkout state = CHECKOUT_INIT;
	পূর্ণাঙ্ক nr_checkouts = 0, nr_unmerged = 0;
	পূর্ণাঙ্ক errs = 0;
	পূর্ণাঙ্ক pos;
	পূর্ণাঙ্ক pc_workers, pc_threshold;
	সংরচনা mem_pool ce_mem_pool;

	state.force = 1;
	state.refresh_cache = 1;
	state.istate = the_repository->index;

	mem_pool_init(&ce_mem_pool, 0);
	get_parallel_checkout_configs(&pc_workers, &pc_threshold);
	init_checkout_metadata(&state.meta, info->refname,
			       info->commit ? &info->commit->object.oid : &info->oid,
			       রিক্ত);

	enable_delayed_checkout(&state);

	যদি (pc_workers > 1)
		init_parallel_checkout();

	প্রতি (pos = 0; pos < the_repository->index->cache_nr; pos++) {
		সংরচনা cache_entry *ce = the_repository->index->cache[pos];
		যদি (ce->ce_flags & CE_MATCHED) {
			যদি (!ce_stage(ce)) {
				errs |= checkout_entry(ce, &state,
						       রিক্ত, &nr_checkouts);
				অনুবর্ত;
			}
			যদি (opts->writeout_stage)
				errs |= checkout_stage(opts->writeout_stage,
						       ce, pos,
						       &state,
						       &nr_checkouts, opts->overlay_mode);
			অন্যথা যদি (opts->merge)
				errs |= checkout_merged(pos, &state,
							&nr_unmerged,
							&ce_mem_pool,
							opts->conflict_style);
			pos = skip_same_name(ce, pos) - 1;
		}
	}
	যদি (pc_workers > 1)
		errs |= run_parallel_checkout(&state, pc_workers, pc_threshold,
					      রিক্ত, রিক্ত);
	mem_pool_discard(&ce_mem_pool, should_validate_cache_entries());
	remove_marked_cache_entries(the_repository->index, 1);
	remove_scheduled_dirs();
	errs |= finish_delayed_checkout(&state, opts->show_progress);

	যদি (opts->গুনতি_checkout_paths) {
		যদি (nr_unmerged)
			fprintf_ln(stderr, Q_("Recreated %d merge conflict",
					      "Recreated %d merge conflicts",
					      nr_unmerged),
				   nr_unmerged);
		যদি (opts->source_বৃক্ষ)
			fprintf_ln(stderr, Q_("Updated %d path from %s",
					      "Updated %d paths from %s",
					      nr_checkouts),
				   nr_checkouts,
				   repo_find_unique_abbrev(the_repository, &opts->source_বৃক্ষ->object.oid,
							   DEFAULT_ABBREV));
		অন্যথা যদি (!nr_unmerged || nr_checkouts)
			fprintf_ln(stderr, Q_("Updated %d path from the index",
					      "Updated %d paths from the index",
					      nr_checkouts),
				   nr_checkouts);
	}

	প্রত্যাগম errs;
}

স্থির পূর্ণাঙ্ক checkout_paths(ধ্রুব সংরচনা checkout_opts *opts,
			  ধ্রুব সংরচনা branch_info *new_branch_info)
{
	পূর্ণাঙ্ক pos;
	স্থির বর্ণ *ps_matched;
	সংরচনা object_id rev;
	সংরচনা commit *head;
	পূর্ণাঙ্ক errs = 0;
	সংরচনা lock_file lock_file = LOCK_INIT;
	পূর্ণাঙ্ক checkout_index;

	trace2_cmd_mode(opts->patch_mode ? "patch" : "path");

	যদি (opts->track != BRANCH_TRACK_UNSPECIFIED)
		die(_("'%s' cannot be used with updating paths"), "--track");

	যদি (opts->new_branch_log)
		die(_("'%s' cannot be used with updating paths"), "-l");

	যদি (opts->ignore_unmerged && opts->patch_mode)
		die(_("'%s' cannot be used with updating paths"),
		    opts->ignore_unmerged_opt);

	যদি (opts->force_detach)
		die(_("'%s' cannot be used with updating paths"), "--detach");

	যদি (opts->merge && opts->patch_mode)
		die(_("options '%s' and '%s' cannot be used together"), "--merge", "--patch");

	যদি (opts->ignore_unmerged && opts->merge)
		die(_("options '%s' and '%s' cannot be used together"),
		    opts->ignore_unmerged_opt, "-m");

	যদি (opts->new_branch)
		die(_("Cannot update paths and switch to branch '%s' at the same time."),
		    opts->new_branch);

	যদি (!opts->checkout_worktree && !opts->checkout_index)
		die(_("neither '%s' or '%s' is specified"),
		    "--staged", "--worktree");

	যদি (!opts->checkout_worktree && !opts->from_treeish)
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
	যদি (!opts->accept_ref && opts->checkout_index) {
		যদি (opts->writeout_stage)
			die(_("'%s' or '%s' cannot be used with %s"),
			    "--ours", "--theirs", "--staged");

		যদি (opts->merge)
			die(_("'%s' or '%s' cannot be used with %s"),
			    "--merge", "--conflict", "--staged");
	}

	/*
	 * recreating unmerged index entries and writing out data from
	 * unmerged index entries would make no sense when checking out
	 * of a tree-ish.
	 */
	যদি ((opts->merge || opts->writeout_stage) && opts->source_বৃক্ষ)
		die(_("'%s', '%s', or '%s' cannot be used when checking out of a tree"),
		    "--merge", "--ours", "--theirs");

	যদি (opts->patch_mode) {
		গণনা add_p_mode patch_mode;
		সংরচনা interactive_options interactive_opts = {
			.context = opts->patch_context,
			.interhunkcontext = opts->patch_interhunk_context,
			.auto_advance = opts->auto_advance
		};
		ধ্রুব বর্ণ *rev = new_branch_info->name;
		বর্ণ rev_oid[GIT_MAX_HEXSZ + 1];

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
		যদি (rev && new_branch_info->commit && strcmp(rev, "HEAD"))
			rev = oid_to_hex_r(rev_oid, &new_branch_info->commit->object.oid);

		যদি (opts->checkout_index && opts->checkout_worktree)
			patch_mode = ADD_P_CHECKOUT;
		অন্যথা যদি (opts->checkout_index && !opts->checkout_worktree)
			patch_mode = ADD_P_RESET;
		অন্যথা যদি (!opts->checkout_index && opts->checkout_worktree)
			patch_mode = ADD_P_WORKTREE;
		অন্যথা
			BUG("either flag must have been set, worktree=%d, index=%d",
			    opts->checkout_worktree, opts->checkout_index);
		প্রত্যাগম !!run_add_p(the_repository, patch_mode, &interactive_opts,
				   rev, &opts->pathspec, 0);
	}

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);
	যদি (repo_read_index_preload(the_repository, &opts->pathspec, 0) < 0)
		প্রত্যাগম error(_("index file corrupt"));

	যদি (opts->source_বৃক্ষ)
		read_বৃক্ষ_some(opts->source_বৃক্ষ, &opts->pathspec,
			       opts->overlay_mode);
	যদি (opts->merge)
		unmerge_index(the_repository->index, &opts->pathspec, CE_MATCHED);

	ps_matched = xcalloc(opts->pathspec.nr, 1);

	/*
	 * Make sure all pathspecs participated in locating the paths
	 * to be checked out.
	 */
	প্রতি (pos = 0; pos < the_repository->index->cache_nr; pos++)
		যদি (opts->overlay_mode)
			mark_ce_for_checkout_overlay(the_repository->index->cache[pos],
						     ps_matched,
						     opts);
		অন্যথা
			mark_ce_for_checkout_no_overlay(the_repository->index->cache[pos],
							ps_matched,
							opts);

	যদি (report_path_error(ps_matched, &opts->pathspec)) {
		মুক্ত(ps_matched);
		প্রত্যাগম 1;
	}
	মুক্ত(ps_matched);

	/* Any unmerged paths? */
	প্রতি (pos = 0; pos < the_repository->index->cache_nr; pos++) {
		ধ্রুব সংরচনা cache_entry *ce = the_repository->index->cache[pos];
		যদি (ce->ce_flags & CE_MATCHED) {
			যদি (!ce_stage(ce))
				অনুবর্ত;
			যদি (opts->ignore_unmerged) {
				যদি (!opts->quiet)
					warning(_("path '%s' is unmerged"), ce->name);
			} অন্যথা যদি (opts->writeout_stage) {
				errs |= check_stage(opts->writeout_stage, ce, pos, opts->overlay_mode);
			} অন্যথা যদি (opts->merge) {
				errs |= check_stages((1<<2) | (1<<3), ce, pos);
			} অন্যথা {
				errs = 1;
				error(_("path '%s' is unmerged"), ce->name);
			}
			pos = skip_same_name(ce, pos) - 1;
		}
	}
	যদি (errs)
		প্রত্যাগম 1;

	/* Now we are committed to check them out */
	যদি (opts->checkout_worktree)
		errs |= checkout_worktree(opts, new_branch_info);
	অন্যথা
		remove_marked_cache_entries(the_repository->index, 1);

	/*
	 * Allow updating the index when checking out from the index.
	 * This is to save new stat info.
	 */
	যদি (opts->checkout_worktree && !opts->checkout_index && !opts->source_বৃক্ষ)
		checkout_index = 1;
	অন্যথা
		checkout_index = opts->checkout_index;

	যদি (checkout_index) {
		যদি (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
			die(_("unable to write new index file"));
	} অন্যথা {
		/*
		 * NEEDSWORK: if --worktree is not specified, we
		 * should save stat info of checked out files in the
		 * index to avoid the next (potentially costly)
		 * refresh. But it's a bit tricker to do...
		 */
		rollback_lock_file(&lock_file);
	}

	refs_read_ref_full(get_main_ref_store(the_repository), "HEAD", 0,
			   &rev, রিক্ত);
	head = lookup_commit_reference_gently(the_repository, &rev, 1);

	errs |= post_checkout_hook(head, head, 0);
	প্রত্যাগম errs;
}

স্থির শূন্য show_local_changes(সংরচনা object *head,
			       ধ্রুব সংরচনা diff_options *opts)
{
	সংরচনা rev_info rev;
	/* I think we want full paths, even if we're in a subdirectory. */
	repo_init_revisions(the_repository, &rev, রিক্ত);
	rev.diffopt.flags = opts->flags;
	rev.diffopt.output_format |= DIFF_FORMAT_NAME_STATUS;
	rev.diffopt.flags.recursive = 1;
	diff_setup_done(&rev.diffopt);
	add_pending_object(&rev, head, রিক্ত);
	run_diff_index(&rev, 0);
	release_revisions(&rev);
}

স্থির শূন্য describe_detached_head(ধ্রুব বর্ণ *বার্তা, সংরচনা commit *commit)
{
	সংরচনা strbuf sb = STRBUF_INIT;

	যদি (!repo_parse_commit(the_repository, commit))
		pp_commit_easy(CMIT_FMT_ONELINE, commit, &sb);
	যদি (print_sha1_ellipsis()) {
		fprintf(stderr, "%s %s... %s\n", বার্তা,
			repo_find_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV),
			sb.buf);
	} অন্যথা {
		fprintf(stderr, "%s %s %s\n", বার্তা,
			repo_find_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV),
			sb.buf);
	}
	strbuf_release(&sb);
}

স্থির পূর্ণাঙ্ক reset_বৃক্ষ(সংরচনা বৃক্ষ *বৃক্ষ, ধ্রুব সংরচনা checkout_opts *o,
		      পূর্ণাঙ্ক worktree, পূর্ণাঙ্ক *writeout_error,
		      সংরচনা branch_info *info)
{
	সংরচনা unpack_trees_options opts;
	সংরচনা বৃক্ষ_desc বৃক্ষ_desc;

	memset(&opts, 0, পরিমাণ(opts));
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
			       রিক্ত);
	যদি (repo_parse_বৃক্ষ(the_repository, বৃক্ষ) < 0)
		প্রত্যাগম 128;
	init_বৃক্ষ_desc(&বৃক্ষ_desc, &বৃক্ষ->object.oid, বৃক্ষ->buffer, বৃক্ষ->size);
	বিকল্প (unpack_trees(1, &বৃক্ষ_desc, &opts)) {
	পক্ষ -2:
		*writeout_error = 1;
		/*
		 * We return 0 nevertheless, as the index is all right
		 * and more importantly we have made best efforts to
		 * update paths in the work tree, and we cannot revert
		 * them.
		 */
		/* fallthrough */
	পক্ষ 0:
		প্রত্যাগম 0;
	স্বভাব:
		প্রত্যাগম 128;
	}
}

স্থির শূন্য setup_branch_path(সংরচনা branch_info *branch)
{
	সংরচনা strbuf buf = STRBUF_INIT;

	/*
	 * If this is a ref, resolve it; otherwise, look up the OID for our
	 * expression.  Failure here is okay.
	 */
	যদি (!repo_dwim_ref(the_repository, branch->name, strlen(branch->name),
			   &branch->oid, &branch->refname, 0))
		repo_get_oid_committish(the_repository, branch->name, &branch->oid);

	copy_branchname(the_repository, &buf, branch->name, INTERPRET_BRANCH_LOCAL);
	যদি (strcmp(buf.buf, branch->name)) {
		মুক্ত(branch->name);
		branch->name = xstrdup(buf.buf);
	}
	strbuf_splice(&buf, 0, 0, "refs/heads/", 11);
	মুক্ত(branch->path);
	branch->path = strbuf_detach(&buf, রিক্ত);
}

স্থির শূন্য init_topts(সংরচনা unpack_trees_options *topts,
		       পূর্ণাঙ্ক show_progress, পূর্ণাঙ্ক overwrite_ignore,
		       bool quiet)
{
	memset(topts, 0, পরিমাণ(*topts));
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

স্থির পূর্ণাঙ্ক merge_working_বৃক্ষ(ধ্রুব সংরচনা checkout_opts *opts,
			      সংরচনা branch_info *old_branch_info,
			      সংরচনা branch_info *new_branch_info,
			      bool quiet,
			      পূর্ণাঙ্ক *writeout_error)
{
	পূর্ণাঙ্ক ret;
	সংরচনা lock_file lock_file = LOCK_INIT;
	সংরচনা বৃক্ষ *new_বৃক্ষ;

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);
	যদি (repo_read_index_preload(the_repository, রিক্ত, 0) < 0) {
		rollback_lock_file(&lock_file);
		প্রত্যাগম error(_("index file corrupt"));
	}

	resolve_undo_clear_index(the_repository->index);
	যদি (opts->new_orphan_branch && opts->orphan_from_empty_বৃক্ষ) {
		যদি (new_branch_info->commit)
			BUG("'switch --orphan' should never accept a commit as starting point");
		new_বৃক্ষ = repo_parse_বৃক্ষ_indirect(the_repository,
						    the_hash_algo->empty_বৃক্ষ);
		যদি (!new_বৃক্ষ)
			BUG("unable to read empty tree");
	} অন্যথা {
		new_বৃক্ষ = repo_get_commit_বৃক্ষ(the_repository,
						new_branch_info->commit);
		যদি (!new_বৃক্ষ) {
			rollback_lock_file(&lock_file);
			প্রত্যাগম error(_("unable to read tree (%s)"),
				     oid_to_hex(&new_branch_info->commit->object.oid));
		}
	}
	যদি (opts->discard_changes) {
		ret = reset_বৃক্ষ(new_বৃক্ষ, opts, 1, writeout_error, new_branch_info);
		যদি (ret) {
			rollback_lock_file(&lock_file);
			প্রত্যাগম ret;
		}
	} অন্যথা {
		সংরচনা বৃক্ষ_desc trees[2];
		সংরচনা বৃক্ষ *বৃক্ষ;
		সংরচনা unpack_trees_options topts;
		ধ্রুব সংরচনা object_id *old_commit_oid;

		refresh_index(the_repository->index, REFRESH_QUIET, রিক্ত, রিক্ত, রিক্ত);

		যদি (unmerged_index(the_repository->index)) {
			rollback_lock_file(&lock_file);
			error(_("you need to resolve your current index first"));
			প্রত্যাগম 1;
		}

		/* 2-way merge to the new branch */
		init_topts(&topts, opts->show_progress,
			   opts->overwrite_ignore, quiet);
		init_checkout_metadata(&topts.meta, new_branch_info->refname,
				       new_branch_info->commit ?
				       &new_branch_info->commit->object.oid :
				       &new_branch_info->oid, রিক্ত);

		old_commit_oid = old_branch_info->commit ?
			&old_branch_info->commit->object.oid :
			the_hash_algo->empty_বৃক্ষ;
		বৃক্ষ = repo_parse_বৃক্ষ_indirect(the_repository,
						old_commit_oid);
		যদি (!বৃক্ষ)
			die(_("unable to parse commit %s"),
				oid_to_hex(old_commit_oid));

		init_বৃক্ষ_desc(&trees[0], &বৃক্ষ->object.oid,
			       বৃক্ষ->buffer, বৃক্ষ->size);
		যদি (repo_parse_বৃক্ষ(the_repository, new_বৃক্ষ) < 0)
			die(রিক্ত);
		বৃক্ষ = new_বৃক্ষ;
		init_বৃক্ষ_desc(&trees[1], &বৃক্ষ->object.oid,
			       বৃক্ষ->buffer, বৃক্ষ->size);

		ret = unpack_trees(2, trees, &topts);
		clear_unpack_trees_porcelain(&topts);
		যদি (ret == -1) {
			rollback_lock_file(&lock_file);
			প্রত্যাগম MERGE_WORKING_বৃক্ষ_UNPACK_FAILED;
		}
	}

	যদি (!cache_বৃক্ষ_fully_valid(the_repository->index))
		cache_বৃক্ষ_update(the_repository->index, WRITE_বৃক্ষ_SILENT | WRITE_বৃক্ষ_REPAIR);

	যদি (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
		die(_("unable to write new index file"));

	যদি (!opts->discard_changes && !opts->quiet && new_branch_info->commit)
		show_local_changes(&new_branch_info->commit->object, &opts->diff_options);

	প্রত্যাগম 0;
}

স্থির শূন্য report_tracking(সংরচনা branch_info *new_branch_info)
{
	সংরচনা strbuf sb = STRBUF_INIT;
	সংরচনা branch *branch = branch_get(new_branch_info->name);

	যদি (!format_tracking_info(branch, &sb, AHEAD_BEHIND_FULL, 1))
		প্রত্যাগম;
	fputs(sb.buf, stdout);
	strbuf_release(&sb);
}

স্থির শূন্য update_refs_for_switch(ধ্রুব সংরচনা checkout_opts *opts,
				   সংরচনা branch_info *old_branch_info,
				   সংরচনা branch_info *new_branch_info)
{
	সংরচনা strbuf বার্তা = STRBUF_INIT;
	ধ্রুব বর্ণ *old_desc, *reflog_বার্তা;
	যদি (opts->new_branch) {
		যদি (opts->new_orphan_branch) {
			গণনা log_refs_config log_all_ref_updates = LOG_REFS_UNSET;
			ধ্রুব বর্ণ *মান;
			বর্ণ *refname;

			যদি (!repo_config_get_string_tmp(the_repository, "core.logallrefupdates", &মান))
				log_all_ref_updates = refs_parse_log_all_ref_updates_config(মান);

			refname = mkpathdup("refs/heads/%s", opts->new_orphan_branch);
			যদি (opts->new_branch_log &&
			    !should_autocreate_reflog(log_all_ref_updates, refname)) {
				পূর্ণাঙ্ক ret;
				সংরচনা strbuf err = STRBUF_INIT;

				ret = refs_create_reflog(get_main_ref_store(the_repository),
							 refname, &err);
				যদি (ret) {
					fprintf(stderr, _("Can not do reflog for '%s': %s\n"),
						opts->new_orphan_branch, err.buf);
					strbuf_release(&err);
					মুক্ত(refname);
					প্রত্যাগম;
				}
				strbuf_release(&err);
			}
			মুক্ত(refname);
		}
		অন্যথা
			create_branch(the_repository,
				      opts->new_branch, new_branch_info->name,
				      opts->new_branch_force ? 1 : 0,
				      opts->new_branch_force ? 1 : 0,
				      opts->new_branch_log,
				      opts->quiet,
				      opts->track,
				      0);
		মুক্ত(new_branch_info->name);
		মুক্ত(new_branch_info->refname);
		new_branch_info->name = xstrdup(opts->new_branch);
		setup_branch_path(new_branch_info);
	}

	old_desc = old_branch_info->name;
	যদি (!old_desc && old_branch_info->commit)
		old_desc = oid_to_hex(&old_branch_info->commit->object.oid);

	reflog_বার্তা = getenv("GIT_REFLOG_ACTION");
	যদি (!reflog_বার্তা)
		strbuf_addf(&বার্তা, "checkout: moving from %s to %s",
			old_desc ? old_desc : "(invalid)", new_branch_info->name);
	অন্যথা
		strbuf_insertstr(&বার্তা, 0, reflog_বার্তা);

	যদি (!strcmp(new_branch_info->name, "HEAD") && !new_branch_info->path && !opts->force_detach) {
		/* Nothing to do. */
	} অন্যথা যদি (opts->force_detach || !new_branch_info->path) {	/* No longer on any branch. */
		refs_update_ref(get_main_ref_store(the_repository), বার্তা.buf,
				"HEAD", &new_branch_info->commit->object.oid,
				রিক্ত,
				REF_NO_DEREF, UPDATE_REFS_DIE_ON_ERR);
		যদি (!opts->quiet) {
			যদি (old_branch_info->path &&
			    advice_enabled(ADVICE_DETACHED_HEAD) && !opts->force_detach)
				detach_advice(new_branch_info->name);
			describe_detached_head(_("HEAD is now at"), new_branch_info->commit);
		}
	} অন্যথা যদি (new_branch_info->path) {	/* Switch branches. */
		যদি (refs_update_symref(get_main_ref_store(the_repository), "HEAD", new_branch_info->path, বার্তা.buf) < 0)
			die(_("unable to update HEAD"));
		যদি (!opts->quiet) {
			যদি (old_branch_info->path && !strcmp(new_branch_info->path, old_branch_info->path)) {
				যদি (opts->new_branch_force)
					fprintf(stderr, _("Reset branch '%s'\n"),
						new_branch_info->name);
				অন্যথা
					fprintf(stderr, _("Already on '%s'\n"),
						new_branch_info->name);
			} অন্যথা যদি (opts->new_branch) {
				যদি (opts->branch_exists)
					fprintf(stderr, _("Switched to and reset branch '%s'\n"), new_branch_info->name);
				অন্যথা
					fprintf(stderr, _("Switched to a new branch '%s'\n"), new_branch_info->name);
			} অন্যথা {
				fprintf(stderr, _("Switched to branch '%s'\n"),
					new_branch_info->name);
			}
		}
		যদি (old_branch_info->path && old_branch_info->name) {
			যদি (!refs_ref_exists(get_main_ref_store(the_repository), old_branch_info->path) && refs_reflog_exists(get_main_ref_store(the_repository), old_branch_info->path))
				refs_delete_reflog(get_main_ref_store(the_repository),
						   old_branch_info->path);
		}
	}
	remove_branch_state(the_repository, !opts->quiet);
	strbuf_release(&বার্তা);
	যদি (!opts->quiet &&
	    !opts->force_detach &&
	    (new_branch_info->path || !strcmp(new_branch_info->name, "HEAD")))
		report_tracking(new_branch_info);
}

স্থির পূর্ণাঙ্ক add_pending_uninteresting_ref(ধ্রুব সংরচনা reference *ref, শূন্য *cb_data)
{
	add_pending_oid(cb_data, ref->name, ref->oid, UNINTERESTING);
	প্রত্যাগম 0;
}

স্থির শূন্য describe_one_orphan(সংরচনা strbuf *sb, সংরচনা commit *commit)
{
	strbuf_addstr(sb, "  ");
	strbuf_add_unique_abbrev(sb, &commit->object.oid, DEFAULT_ABBREV);
	strbuf_addch(sb, ' ');
	যদি (!repo_parse_commit(the_repository, commit))
		pp_commit_easy(CMIT_FMT_ONELINE, commit, sb);
	strbuf_addch(sb, '\n');
}

#define ORPHAN_CUTOFF 4
স্থির শূন্য suggest_reattach(সংরচনা commit *commit, সংরচনা rev_info *revs)
{
	সংরচনা commit *c, *last = রিক্ত;
	সংরচনা strbuf sb = STRBUF_INIT;
	পূর্ণাঙ্ক lost = 0;
	যাবত্ ((c = get_revision(revs)) != রিক্ত) {
		যদি (lost < ORPHAN_CUTOFF)
			describe_one_orphan(&sb, c);
		last = c;
		lost++;
	}
	যদি (ORPHAN_CUTOFF < lost) {
		পূর্ণাঙ্ক more = lost - ORPHAN_CUTOFF;
		যদি (more == 1)
			describe_one_orphan(&sb, last);
		অন্যথা
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

	যদি (advice_enabled(ADVICE_DETACHED_HEAD))
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
স্থির শূন্য orphaned_commit_warning(সংরচনা commit *old_commit, সংরচনা commit *new_commit)
{
	সংরচনা rev_info revs;
	সংরচনা object *object = &old_commit->object;

	repo_init_revisions(the_repository, &revs, রিক্ত);
	setup_revisions(0, রিক্ত, &revs, রিক্ত);

	object->flags &= ~UNINTERESTING;
	add_pending_object(&revs, object, oid_to_hex(&object->oid));

	refs_for_each_ref(get_main_ref_store(the_repository),
			  add_pending_uninteresting_ref, &revs);
	যদি (new_commit)
		add_pending_oid(&revs, "HEAD",
				&new_commit->object.oid,
				UNINTERESTING);

	যদি (prepare_revision_walk(&revs))
		die(_("internal error in revision walk"));
	যদি (!(old_commit->object.flags & UNINTERESTING))
		suggest_reattach(old_commit, &revs);
	অন্যথা
		describe_detached_head(_("Previous HEAD position was"), old_commit);

	/* Clean up objects used, as they will be reused. */
	repo_clear_commit_marks(the_repository, ALL_REV_FLAGS);
	release_revisions(&revs);
}

স্থির পূর্ণাঙ্ক switch_branches(ধ্রুব সংরচনা checkout_opts *opts,
			   সংরচনা branch_info *new_branch_info)
{
	পূর্ণাঙ্ক ret = 0;
	সংরচনা branch_info old_branch_info = { 0 };
	সংরচনা object_id rev;
	পূর্ণাঙ্ক flag, writeout_error = 0;
	পূর্ণাঙ্ক do_merge = 1;
	পূর্ণাঙ্ক created_autostash = 0;
	গণনা stash_apply_result autostash_res = STASH_APPLY_CLEAN;
	সংরচনা strbuf old_commit_shortname = STRBUF_INIT;
	সংরচনা strbuf autostash_বার্তা = STRBUF_INIT;
	ধ্রুব বর্ণ *stash_label_base = রিক্ত;

	trace2_cmd_mode("branch");

	memset(&old_branch_info, 0, পরিমাণ(old_branch_info));
	old_branch_info.path = refs_resolve_refdup(get_main_ref_store(the_repository),
						   "HEAD", 0, &rev, &flag);
	যদি (old_branch_info.path)
		old_branch_info.commit = lookup_commit_reference_gently(the_repository, &rev, 1);
	যদি (!(flag & REF_ISSYMREF))
		FREE_AND_NULL(old_branch_info.path);

	যদি (old_branch_info.path) {
		ধ্রুব বর্ণ *ধ্রুব prefix = "refs/heads/";
		ধ্রুব বর্ণ *p;
		যদি (skip_prefix(old_branch_info.path, prefix, &p))
			old_branch_info.name = xstrdup(p);
	}

	যদি (opts->new_orphan_branch && opts->orphan_from_empty_বৃক্ষ) {
		যদি (new_branch_info->name)
			BUG("'switch --orphan' should never accept a commit as starting point");
		new_branch_info->commit = রিক্ত;
		new_branch_info->name = xstrdup("(empty)");
		do_merge = 1;
	}

	যদি (!new_branch_info->name) {
		new_branch_info->name = xstrdup("HEAD");
		new_branch_info->commit = old_branch_info.commit;
		যদি (!new_branch_info->commit)
			die(_("You are on a branch yet to be born"));
		parse_commit_or_die(new_branch_info->commit);

		যদি (opts->only_merge_on_switching_branches)
			do_merge = 0;
	}

	যদি (old_branch_info.name) {
		stash_label_base = old_branch_info.name;
	} অন্যথা যদি (old_branch_info.commit) {
		strbuf_add_unique_abbrev(&old_commit_shortname,
					 &old_branch_info.commit->object.oid,
					 DEFAULT_ABBREV);
		stash_label_base = old_commit_shortname.buf;
	}

	যদি (do_merge) {
		ret = merge_working_বৃক্ষ(opts, &old_branch_info, new_branch_info,
					 opts->merge, &writeout_error);
		যদি (ret == MERGE_WORKING_বৃক্ষ_UNPACK_FAILED && opts->merge) {
			strbuf_addf(&autostash_বার্তা,
				    "autostash while switching to '%s'",
				    new_branch_info->name);
			create_autostash_ref(the_repository,
					     "CHECKOUT_AUTOSTASH_HEAD",
					     autostash_বার্তা.buf, true);
			created_autostash = 1;
			ret = merge_working_বৃক্ষ(opts, &old_branch_info, new_branch_info,
						 false, &writeout_error);
		}
		যদি (created_autostash) {
			যদি (opts->conflict_style >= 0) {
				সংরচনা strbuf cfg = STRBUF_INIT;
				strbuf_addf(&cfg, "merge.conflictStyle=%s",
					    conflict_style_name(opts->conflict_style));
				git_config_ঠেলো_parameter(cfg.buf);
				strbuf_release(&cfg);
			}
			autostash_res = apply_autostash_ref(the_repository,
				    "CHECKOUT_AUTOSTASH_HEAD",
				    new_branch_info->name,
				    "local",
				    stash_label_base,
				    autostash_বার্তা.buf);
		}
		যদি (ret) {
			branch_info_release(&old_branch_info);
			strbuf_release(&old_commit_shortname);
			strbuf_release(&autostash_বার্তা);
			প্রত্যাগম ret < 0 ? 1 : ret;
		}
	}

	যদি (!opts->quiet && !old_branch_info.path && old_branch_info.commit && new_branch_info->commit != old_branch_info.commit)
		orphaned_commit_warning(old_branch_info.commit, new_branch_info->commit);

	যদি (autostash_res == STASH_APPLY_CONFLICT && !opts->quiet)
		fputc('\n', stderr);
	update_refs_for_switch(opts, &old_branch_info, new_branch_info);

	যদি (created_autostash) {
		discard_index(the_repository->index);
		যদি (repo_read_index(the_repository) < 0)
			die(_("index file corrupt"));

		যদি (!opts->quiet && new_branch_info->commit) {
			printf(_("The following paths have local changes:\n"));
			show_local_changes(&new_branch_info->commit->object,
					   &opts->diff_options);
		}
	}

	ret = post_checkout_hook(old_branch_info.commit, new_branch_info->commit, 1);
	branch_info_release(&old_branch_info);
	strbuf_release(&old_commit_shortname);
	strbuf_release(&autostash_বার্তা);

	প্রত্যাগম ret || writeout_error;
}

স্থির পূর্ণাঙ্ক git_checkout_config(ধ্রুব বর্ণ *var, ধ্রুব বর্ণ *মান,
			       ধ্রুব সংরচনা config_context *ctx, শূন্য *cb)
{
	সংরচনা checkout_opts *opts = cb;

	যদি (!strcmp(var, "diff.ignoresubmodules")) {
		যদি (!মান)
			প্রত্যাগম config_error_nonbool(var);
		handle_ignore_submodules_arg(&opts->diff_options, মান);
		প্রত্যাগম 0;
	}
	যদি (!strcmp(var, "checkout.guess")) {
		opts->dwim_new_local_branch = git_config_bool(var, মান);
		প্রত্যাগম 0;
	}

	যদি (starts_with(var, "submodule."))
		প্রত্যাগম git_default_submodule_config(var, মান, রিক্ত);

	প্রত্যাগম git_xmerge_config(var, মান, ctx, রিক্ত);
}

স্থির শূন্য setup_new_branch_info_and_source_বৃক্ষ(
	সংরচনা branch_info *new_branch_info,
	সংরচনা checkout_opts *opts,
	সংরচনা object_id *rev,
	ধ্রুব বর্ণ *arg)
{
	সংরচনা বৃক্ষ **source_বৃক্ষ = &opts->source_বৃক্ষ;
	সংরচনা object_id branch_rev;

	/* treat '@' as a shortcut for 'HEAD' */
	new_branch_info->name = !strcmp(arg, "@") ? xstrdup("HEAD") :
						    xstrdup(arg);
	setup_branch_path(new_branch_info);

	যদি (!check_refname_format(new_branch_info->path, 0) &&
	    !refs_read_ref(get_main_ref_store(the_repository), new_branch_info->path, &branch_rev))
		oidcpy(rev, &branch_rev);
	অন্যথা
		/* not an existing branch */
		FREE_AND_NULL(new_branch_info->path);

	new_branch_info->commit = lookup_commit_reference_gently(the_repository, rev, 1);
	যদি (!new_branch_info->commit) {
		/* not a commit */
		*source_বৃক্ষ = repo_parse_বৃক্ষ_indirect(the_repository, rev);
		যদি (!*source_বৃক্ষ)
			die(_("unable to read tree (%s)"), oid_to_hex(rev));
	} অন্যথা {
		parse_commit_or_die(new_branch_info->commit);
		*source_বৃক্ষ = repo_get_commit_বৃক্ষ(the_repository,
						    new_branch_info->commit);
		যদি (!*source_বৃক্ষ)
			die(_("unable to read tree (%s)"),
			    oid_to_hex(&new_branch_info->commit->object.oid));
	}
}


গণনা checkout_command {
	CHECKOUT_CHECKOUT = 1,
	CHECKOUT_SWITCH = 2,
	CHECKOUT_RESTORE = 3,
};

স্থির শূন্য advise_disambiguating_remotes(গণনা checkout_command which_command,
					  ধ্রুব বর্ণ *branch,
					  ধ্রুব সংরচনা string_list *matched_remote_names)
{
	ধ্রুব বর্ণ *cmdname;
	সংরচনা string_list_item *item;

	বিকল্প (which_command) {
	পক্ষ CHECKOUT_CHECKOUT:
		cmdname = "checkout";
		ভঙ্গ;
	পক্ষ CHECKOUT_SWITCH:
		cmdname = "switch";
		ভঙ্গ;
	স্বভাব:
		BUG("command <%d> should not reach advise_disambiguating_remotes",
		    which_command);
		ভঙ্গ;
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

স্থির বর্ণ *parse_remote_branch(ধ্রুব বর্ণ *arg,
				 সংরচনা object_id *rev,
				 পূর্ণাঙ্ক could_be_checkout_paths,
				 গণনা checkout_command which_command)
{
	পূর্ণাঙ্ক অঙ্ক_matches = 0;
	সংরচনা string_list matched_remote_names = STRING_LIST_INIT_DUP;

	বর্ণ *remote = unique_tracking_name(arg, rev, &অঙ্ক_matches,
					    &matched_remote_names);

	যদি (remote && could_be_checkout_paths) {
		die(_("'%s' could be both a local file and a tracking branch.\n"
			"Please use -- (and optionally --no-guess) to disambiguate"),
		    arg);
	}

	যদি (!remote && অঙ্ক_matches > 1) {
		যদি (advice_enabled(ADVICE_CHECKOUT_AMBIGUOUS_REMOTE_BRANCH_NAME))
			advise_disambiguating_remotes(which_command, arg,
						      &matched_remote_names);
		die(_("'%s' matched multiple (%d) remote tracking branches"),
		    arg, অঙ্ক_matches);
	}

	string_list_clear(&matched_remote_names, 0);

	প্রত্যাগম remote;
}

স্থির পূর্ণাঙ্ক parse_branchname_arg(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv,
				পূর্ণাঙ্ক dwim_new_local_branch_ok,
				গণনা checkout_command which_command,
				সংরচনা branch_info *new_branch_info,
				সংরচনা checkout_opts *opts,
				সংরচনা object_id *rev)
{
	ধ্রুব বর্ণ **new_branch = &opts->new_branch;
	পূর্ণাঙ্ক argcount = 0;
	ধ্রুব বর্ণ *arg;
	বর্ণ *remote = রিক্ত;
	পূর্ণাঙ্ক dash_dash_pos;
	পূর্ণাঙ্ক has_dash_dash = 0;
	পূর্ণাঙ্ক i;

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
	যদি (!argc)
		প্রত্যাগম 0;

	যদি (!opts->accept_pathspec) {
		যদি (argc > 1)
			die(_("only one reference expected"));
		has_dash_dash = 1; /* helps disambiguate */
	}

	arg = argv[0];
	dash_dash_pos = -1;
	প্রতি (i = 0; i < argc; i++) {
		যদি (opts->accept_pathspec && !strcmp(argv[i], "--")) {
			dash_dash_pos = i;
			ভঙ্গ;
		}
	}
	যদি (dash_dash_pos == 0)
		প্রত্যাগম 1; /* case (2) */
	অন্যথা যদি (dash_dash_pos == 1)
		has_dash_dash = 1; /* case (3) or (1) */
	অন্যথা যদি (dash_dash_pos >= 2)
		die(_("only one reference expected, %d given."), dash_dash_pos);
	opts->গুনতি_checkout_paths = !opts->quiet && !has_dash_dash;

	যদি (!strcmp(arg, "-"))
		arg = "@{-1}";

	যদি (repo_get_oid_mb(the_repository, arg, rev)) {
		/*
		 * Either case (3) or (4), with <something> not being
		 * a commit, or an attempt to use case (1) with an
		 * invalid ref.
		 *
		 * It's likely an error, but we need to find out if
		 * we should auto-create the branch, case (3).(b).
		 */
		পূর্ণাঙ্ক recover_with_dwim = dwim_new_local_branch_ok;

		পূর্ণাঙ্ক could_be_checkout_paths = !has_dash_dash &&
			check_filename(opts->prefix, arg);

		যদি (!has_dash_dash && !no_wildcard(arg))
			recover_with_dwim = 0;

		/*
		 * Accept "git checkout foo", "git checkout foo --"
		 * and "git switch foo" as candidates for dwim.
		 */
		যদি (!(argc == 1 && !has_dash_dash) &&
		    !(argc == 2 && has_dash_dash) &&
		    opts->accept_pathspec)
			recover_with_dwim = 0;

		যদি (recover_with_dwim) {
			remote = parse_remote_branch(arg, rev,
						     could_be_checkout_paths,
						     which_command);
			যদি (remote) {
				*new_branch = arg;
				arg = remote;
				/* DWIMmed to create local branch, case (3).(b) */
			} অন্যথা {
				recover_with_dwim = 0;
			}
		}

		যদি (!recover_with_dwim) {
			যদি (has_dash_dash)
				die(_("invalid reference: %s"), arg);
			প্রত্যাগম argcount;
		}
	}

	/* we can't end up being in (2) anymore, eat the argument */
	argcount++;
	argv++;
	argc--;

	setup_new_branch_info_and_source_বৃক্ষ(new_branch_info, opts, rev, arg);

	যদি (!opts->source_বৃক্ষ)                   /* case (1): want a tree */
		die(_("reference is not a tree: %s"), arg);

	যদি (!has_dash_dash) {	/* case (3).(d) -> (1) */
		/*
		 * Do not complain the most common case
		 *	git checkout branch
		 * even if there happen to be a file called 'branch';
		 * it would be extremely annoying.
		 */
		যদি (argc)
			verify_non_filename(the_repository, opts->prefix, arg);
	} অন্যথা যদি (opts->accept_pathspec) {
		argcount++;
		argv++;
		argc--;
	}

	মুক্ত(remote);
	প্রত্যাগম argcount;
}

স্থির পূর্ণাঙ্ক switch_unborn_to_new_branch(ধ্রুব সংরচনা checkout_opts *opts)
{
	পূর্ণাঙ্ক status;
	সংরচনা strbuf branch_ref = STRBUF_INIT;

	trace2_cmd_mode("unborn");

	যদি (!opts->new_branch)
		die(_("You are on a branch yet to be born"));
	strbuf_addf(&branch_ref, "refs/heads/%s", opts->new_branch);
	status = refs_update_symref(get_main_ref_store(the_repository),
				    "HEAD", branch_ref.buf, "checkout -b");
	strbuf_release(&branch_ref);
	যদি (!opts->quiet)
		fprintf(stderr, _("Switched to a new branch '%s'\n"),
			opts->new_branch);
	প্রত্যাগম status;
}

স্থির শূন্য die_expecting_a_branch(ধ্রুব সংরচনা branch_info *branch_info)
{
	সংরচনা object_id oid;
	বর্ণ *to_free;
	পূর্ণাঙ্ক code;

	যদি (repo_dwim_ref(the_repository, branch_info->name,
			  strlen(branch_info->name), &oid, &to_free, 0) == 1) {
		ধ্রুব বর্ণ *ref = to_free;

		যদি (skip_prefix(ref, "refs/tags/", &ref))
			code = die_message(_("a branch is expected, got tag '%s'"), ref);
		অন্যথা যদি (skip_prefix(ref, "refs/remotes/", &ref))
			code = die_message(_("a branch is expected, got remote branch '%s'"), ref);
		অন্যথা
			code = die_message(_("a branch is expected, got '%s'"), ref);
	}
	অন্যথা যদি (branch_info->commit)
		code = die_message(_("a branch is expected, got commit '%s'"), branch_info->name);
	অন্যথা
		/*
		 * This case should never happen because we already die() on
		 * non-commit, but just in case.
		 */
		code = die_message(_("a branch is expected, got '%s'"), branch_info->name);

	যদি (advice_enabled(ADVICE_SUGGEST_DETACHING_HEAD))
		advise(_("If you want to detach HEAD at the commit, try again with the --detach option."));

	exit(code);
}

স্থির শূন্য die_if_some_operation_in_progress(শূন্য)
{
	সংরচনা wt_status_state state;

	memset(&state, 0, পরিমাণ(state));
	wt_status_get_state(the_repository, &state, 0);

	যদি (state.merge_in_progress)
		die(_("cannot switch branch while merging\n"
		      "Consider \"git merge --quit\" "
		      "or \"git worktree add\"."));
	যদি (state.am_in_progress)
		die(_("cannot switch branch in the middle of an am session\n"
		      "Consider \"git am --quit\" "
		      "or \"git worktree add\"."));
	যদি (state.rebase_interactive_in_progress || state.rebase_in_progress)
		die(_("cannot switch branch while rebasing\n"
		      "Consider \"git rebase --quit\" "
		      "or \"git worktree add\"."));
	যদি (state.cherry_pick_in_progress)
		die(_("cannot switch branch while cherry-picking\n"
		      "Consider \"git cherry-pick --quit\" "
		      "or \"git worktree add\"."));
	যদি (state.revert_in_progress)
		die(_("cannot switch branch while reverting\n"
		      "Consider \"git revert --quit\" "
		      "or \"git worktree add\"."));
	যদি (state.bisect_in_progress)
		warning(_("you are switching branch while bisecting"));

	wt_status_state_free_buffers(&state);
}

/*
 * die if attempting to checkout an existing branch that is in use
 * in another worktree, unless ignore-other-wortrees option is given.
 * The check is bypassed when the branch is already the current one,
 * as it will not make things any worse.
 */
স্থির শূন্য die_if_switching_to_a_branch_in_use(সংরচনা checkout_opts *opts,
						ধ্রুব বর্ণ *full_ref)
{
	পূর্ণাঙ্ক flags;
	বর্ণ *head_ref;

	যদি (opts->ignore_other_worktrees)
		প্রত্যাগম;
	head_ref = refs_resolve_refdup(get_main_ref_store(the_repository),
				       "HEAD", 0, রিক্ত, &flags);
	যদি (head_ref && (!(flags & REF_ISSYMREF) || strcmp(head_ref, full_ref)))
		die_if_checked_out(full_ref, 1);
	মুক্ত(head_ref);
}

স্থির পূর্ণাঙ্ক checkout_branch(সংরচনা checkout_opts *opts,
			   সংরচনা branch_info *new_branch_info)
{
	সংরচনা repo_config_মানসমূহ *cfg = repo_config_মানসমূহ(the_repository);
	পূর্ণাঙ্ক noop_switch = (!new_branch_info->name &&
			   !opts->new_branch &&
			   !opts->force_detach);

	যদি (opts->pathspec.nr)
		die(_("paths cannot be used with switching branches"));

	যদি (opts->patch_mode)
		die(_("'%s' cannot be used with switching branches"),
		    "--patch");

	যদি (opts->overlay_mode != -1)
		die(_("'%s' cannot be used with switching branches"),
		    "--[no]-overlay");

	যদি (opts->writeout_stage) {
		ধ্রুব বর্ণ *বার্তা;
		যদি (noop_switch)
			বার্তা = _("'%s' needs the paths to check out");
		অন্যথা
			বার্তা = _("'%s' cannot be used with switching branches");
		die(বার্তা, "--ours/--theirs");
	}

	যদি (opts->force && opts->merge)
		die(_("'%s' cannot be used with '%s'"), "-f", "-m");

	যদি (opts->discard_changes && opts->merge)
		die(_("'%s' cannot be used with '%s'"), "--discard-changes", "--merge");

	যদি (opts->force_detach && opts->new_branch)
		die(_("'%s' cannot be used with '%s'"),
		    "--detach", "-b/-B/--orphan");

	যদি (opts->new_orphan_branch) {
		যদি (opts->track != BRANCH_TRACK_UNSPECIFIED)
			die(_("'%s' cannot be used with '%s'"), "--orphan", "-t");
		যদি (opts->orphan_from_empty_বৃক্ষ && new_branch_info->name)
			die(_("'%s' cannot take <start-point>"), "--orphan");
	} অন্যথা যদি (opts->force_detach) {
		যদি (opts->track != BRANCH_TRACK_UNSPECIFIED)
			die(_("'%s' cannot be used with '%s'"), "--detach", "-t");
	} অন্যথা যদি (opts->track == BRANCH_TRACK_UNSPECIFIED)
		opts->track = cfg->branch_track;

	যদি (new_branch_info->name && !new_branch_info->commit)
		die(_("Cannot switch branch to a non-commit '%s'"),
		    new_branch_info->name);

	যদি (noop_switch &&
	    !opts->switch_branch_doing_nothing_is_ok)
		die(_("missing branch or commit argument"));

	যদি (!opts->implicit_detach &&
	    !opts->force_detach &&
	    !opts->new_branch &&
	    !opts->new_branch_force &&
	    new_branch_info->name &&
	    !new_branch_info->path)
		die_expecting_a_branch(new_branch_info);

	যদি (!opts->can_switch_when_in_progress)
		die_if_some_operation_in_progress();

	/* "git checkout <branch>" */
	যদি (new_branch_info->path && !opts->force_detach && !opts->new_branch)
		die_if_switching_to_a_branch_in_use(opts, new_branch_info->path);

	/* "git checkout -B <branch>" */
	যদি (opts->new_branch_force) {
		বর্ণ *full_ref = xstrfmt("refs/heads/%s", opts->new_branch);
		die_if_switching_to_a_branch_in_use(opts, full_ref);
		মুক্ত(full_ref);
	}

	যদি (!new_branch_info->commit && opts->new_branch) {
		সংরচনা object_id rev;
		পূর্ণাঙ্ক flag;

		যদি (!refs_read_ref_full(get_main_ref_store(the_repository), "HEAD", 0, &rev, &flag) &&
		    (flag & REF_ISSYMREF) && is_null_oid(&rev))
			প্রত্যাগম switch_unborn_to_new_branch(opts);
	}
	প্রত্যাগম switch_branches(opts, new_branch_info);
}

স্থির পূর্ণাঙ্ক parse_opt_conflict(ধ্রুব সংরচনা option *o, ধ্রুব বর্ণ *arg, পূর্ণাঙ্ক unset)
{
	সংরচনা checkout_opts *opts = o->মান;

	যদি (unset) {
		opts->conflict_style = -1;
		প্রত্যাগম 0;
	}
	opts->conflict_style = parse_conflict_style_name(arg);
	যদি (opts->conflict_style < 0)
		প্রত্যাগম error(_("unknown conflict style '%s'"), arg);
	/* --conflict overrides a previous --no-merge */
	যদি (!opts->merge)
		opts->merge = -1;

	প্রত্যাগম 0;
}

স্থির সংরচনা option *add_common_options(সংরচনা checkout_opts *opts,
					 সংরচনা option *prevopts)
{
	সংরচনা option options[] = {
		OPT__QUIET(&opts->quiet, N_("suppress progress reporting")),
		OPT_CALLBACK_F(0, "recurse-submodules", রিক্ত,
			    "checkout", "control recursive updating of submodules",
			    PARSE_OPT_OPTARG, option_parse_recurse_submodules_worktree_updater),
		OPT_BOOL(0, "progress", &opts->show_progress, N_("force progress reporting")),
		OPT_BOOL('m', "merge", &opts->merge, N_("perform a 3-way merge with the new branch")),
		OPT_CALLBACK(0, "conflict", opts, N_("style"),
			     N_("conflict style (merge, diff3, or zdiff3)"),
			     parse_opt_conflict),
		OPT_END()
	};
	সংরচনা option *newopts = parse_options_concat(prevopts, options);
	মুক্ত(prevopts);
	প্রত্যাগম newopts;
}

স্থির সংরচনা option *add_common_switch_branch_options(
	সংরচনা checkout_opts *opts, সংরচনা option *prevopts)
{
	সংরচনা option options[] = {
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
	সংরচনা option *newopts = parse_options_concat(prevopts, options);
	মুক্ত(prevopts);
	প্রত্যাগম newopts;
}

স্থির সংরচনা option *add_checkout_path_options(সংরচনা checkout_opts *opts,
						সংরচনা option *prevopts)
{
	সংরচনা option options[] = {
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
	সংরচনা option *newopts = parse_options_concat(prevopts, options);
	মুক্ত(prevopts);
	প্রত্যাগম newopts;
}

/* create-branch option (either b or c) */
স্থির বর্ণ cb_option = 'b';

স্থির পূর্ণাঙ্ক checkout_main(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix,
			 সংরচনা checkout_opts *opts, সংরচনা option *options,
			 গণনা checkout_command which_command)
{
	পূর্ণাঙ্ক parseopt_flags = 0;
	সংরচনা branch_info new_branch_info = { 0 };
	পূর্ণাঙ্ক ret;

	স্থির ধ্রুব বর্ণ * ধ্রুব checkout_usage[] = {
		N_("git checkout [<options>] <branch>"),
		N_("git checkout [<options>] [<branch>] -- <file>..."),
		রিক্ত,
	};

	স্থির ধ্রুব বর্ণ * ধ্রুব switch_branch_usage[] = {
		N_("git switch [<options>] [<branch>]"),
		রিক্ত,
	};

	স্থির ধ্রুব বর্ণ * ধ্রুব restore_usage[] = {
		N_("git restore [<options>] [--source=<branch>] <file>..."),
		রিক্ত,
	};

	ধ্রুব বর্ণ * ধ্রুব *usagestr;

	বিকল্প (which_command) {
	পক্ষ CHECKOUT_CHECKOUT:
		usagestr = checkout_usage;
		ভঙ্গ;
	পক্ষ CHECKOUT_SWITCH:
		usagestr = switch_branch_usage;
		ভঙ্গ;
	পক্ষ CHECKOUT_RESTORE:
		usagestr = restore_usage;
		ভঙ্গ;
	স্বভাব:
		BUG("no such checkout variant %d", which_command);
	}

	opts->overwrite_ignore = 1;
	opts->prefix = prefix;
	opts->show_progress = -1;

	repo_config(the_repository, git_checkout_config, opts);
	যদি (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_full_index = 0;
	}

	opts->track = BRANCH_TRACK_UNSPECIFIED;

	যদি (!opts->accept_pathspec && !opts->accept_ref)
		BUG("make up your mind, you need to take _something_");
	যদি (opts->accept_pathspec && opts->accept_ref)
		parseopt_flags = PARSE_OPT_KEEP_DASHDASH;

	argc = parse_options(argc, argv, prefix, options,
			     usagestr, parseopt_flags);

	যদি (opts->patch_context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	যদি (opts->patch_interhunk_context < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	যদি (!opts->patch_mode) {
		যদি (opts->patch_context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--patch");
		যদি (opts->patch_interhunk_context != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--patch");
		যদি (!opts->auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--patch");
	}

	যদি (opts->show_progress < 0) {
		যদি (opts->quiet)
			opts->show_progress = 0;
		অন্যথা
			opts->show_progress = isatty(2);
	}

	/* --conflicts implies --merge */
	যদি (opts->merge == -1)
		opts->merge = opts->conflict_style >= 0;

	যদি (opts->force) {
		opts->discard_changes = 1;
		opts->ignore_unmerged_opt = "--force";
		opts->ignore_unmerged = 1;
	}

	যদি ((!!opts->new_branch + !!opts->new_branch_force + !!opts->new_orphan_branch) > 1)
		die(_("options '-%c', '-%c', and '%s' cannot be used together"),
			cb_option, toupper(cb_option), "--orphan");

	যদি (opts->overlay_mode == 1 && opts->patch_mode)
		die(_("options '%s' and '%s' cannot be used together"), "-p", "--overlay");

	যদি (opts->checkout_index >= 0 || opts->checkout_worktree >= 0) {
		যদি (opts->checkout_index < 0)
			opts->checkout_index = 0;
		যদি (opts->checkout_worktree < 0)
			opts->checkout_worktree = 0;
	} অন্যথা {
		যদি (opts->checkout_index < 0)
			opts->checkout_index = -opts->checkout_index - 1;
		যদি (opts->checkout_worktree < 0)
			opts->checkout_worktree = -opts->checkout_worktree - 1;
	}
	যদি (opts->checkout_index < 0 || opts->checkout_worktree < 0)
		BUG("these flags should be non-negative by now");
	/*
	 * convenient shortcut: "git restore --staged [--worktree]" equals
	 * "git restore --staged [--worktree] --source HEAD"
	 */
	যদি (!opts->from_treeish && opts->checkout_index)
		opts->from_treeish = "HEAD";

	/*
	 * From here on, new_branch will contain the branch to be checked out,
	 * and new_branch_force and new_orphan_branch will tell us which one of
	 * -b/-B/-c/-C/--orphan is being used.
	 */
	যদি (opts->new_branch_force)
		opts->new_branch = opts->new_branch_force;

	যদি (opts->new_orphan_branch)
		opts->new_branch = opts->new_orphan_branch;

	/* --track without -c/-C/-b/-B/--orphan should DWIM */
	যদি (opts->track != BRANCH_TRACK_UNSPECIFIED && !opts->new_branch) {
		ধ্রুব বর্ণ *argv0 = argv[0];
		যদি (!argc || !strcmp(argv0, "--"))
			die(_("--track needs a branch name"));
		skip_prefix(argv0, "refs/", &argv0);
		skip_prefix(argv0, "remotes/", &argv0);
		argv0 = strchr(argv0, '/');
		যদি (!argv0 || !argv0[1])
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
	যদি (argc && opts->accept_ref) {
		সংরচনা object_id rev;
		পূর্ণাঙ্ক dwim_ok =
			!opts->patch_mode &&
			opts->dwim_new_local_branch &&
			opts->track == BRANCH_TRACK_UNSPECIFIED &&
			!opts->new_branch;
		পূর্ণাঙ্ক n = parse_branchname_arg(argc, argv, dwim_ok, which_command,
					     &new_branch_info, opts, &rev);
		argv += n;
		argc -= n;
	} অন্যথা যদি (!opts->accept_ref && opts->from_treeish) {
		সংরচনা object_id rev;

		যদি (repo_get_oid_mb(the_repository, opts->from_treeish, &rev))
			die(_("could not resolve '%s'"), opts->from_treeish);

		setup_new_branch_info_and_source_বৃক্ষ(&new_branch_info,
						      opts, &rev,
						      opts->from_treeish);

		যদি (!opts->source_বৃক্ষ)
			die(_("reference is not a tree: %s"), opts->from_treeish);
	}

	যদি (argc) {
		parse_pathspec(&opts->pathspec, 0,
			       opts->patch_mode ? PATHSPEC_PREFIX_ORIGIN : 0,
			       prefix, argv);

		যদি (!opts->pathspec.nr)
			die(_("invalid path specification"));

		/*
		 * Try to give more helpful suggestion.
		 * new_branch && argc > 1 will be caught later.
		 */
		যদি (opts->new_branch && argc == 1 && !new_branch_info.commit)
			die(_("'%s' is not a commit and a branch '%s' cannot be created from it"),
				argv[0], opts->new_branch);

		যদি (opts->force_detach)
			die(_("git checkout: --detach does not take a path argument '%s'"),
			    argv[0]);
	}

	যদি (opts->pathspec_from_file) {
		যদি (opts->pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		যদি (opts->force_detach)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--detach");

		যদি (opts->patch_mode)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--patch");

		parse_pathspec_file(&opts->pathspec, 0,
				    0,
				    prefix, opts->pathspec_from_file, opts->pathspec_file_nul);
	} অন্যথা যদি (opts->pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	opts->pathspec.recursive = 1;

	যদি (opts->pathspec.nr) {
		যদি (1 < !!opts->writeout_stage + !!opts->force + !!opts->merge)
			die(_("git checkout: --ours/--theirs, --force and --merge are incompatible when\n"
			      "checking out of the index."));
	} অন্যথা {
		যদি (opts->accept_pathspec && !opts->empty_pathspec_ok &&
		    !opts->patch_mode)	/* patch mode is special */
			die(_("you must specify path(s) to restore"));
	}

	যদি (opts->new_branch) {
		সংরচনা strbuf buf = STRBUF_INIT;

		যদি (opts->new_branch_force)
			opts->branch_exists = validate_branchname(opts->new_branch, &buf);
		অন্যথা
			opts->branch_exists =
				validate_new_branchname(opts->new_branch, &buf, 0);
		strbuf_release(&buf);
	}

	যদি (opts->patch_mode || opts->pathspec.nr)
		ret = checkout_paths(opts, &new_branch_info);
	অন্যথা
		ret = checkout_branch(opts, &new_branch_info);

	branch_info_release(&new_branch_info);
	clear_pathspec(&opts->pathspec);
	মুক্ত(opts->pathspec_from_file);
	মুক্ত(options);

	প্রত্যাগম ret;
}

পূর্ণাঙ্ক cmd_checkout(পূর্ণাঙ্ক argc,
		 ধ্রুব বর্ণ **argv,
		 ধ্রুব বর্ণ *prefix,
		 সংরচনা repository *repo UNUSED)
{
	সংরচনা checkout_opts opts = CHECKOUT_OPTS_INIT;
	সংরচনা option *options;
	সংরচনা option checkout_options[] = {
		OPT_STRING('b', রিক্ত, &opts.new_branch, N_("branch"),
			   N_("create and checkout a new branch")),
		OPT_STRING('B', রিক্ত, &opts.new_branch_force, N_("branch"),
			   N_("create/reset and checkout a branch")),
		OPT_BOOL('l', রিক্ত, &opts.new_branch_log, N_("create reflog for new branch")),
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
	opts.orphan_from_empty_বৃক্ষ = 0;
	opts.empty_pathspec_ok = 1;
	opts.overlay_mode = -1;
	opts.checkout_index = -2;    /* default on */
	opts.checkout_worktree = -2; /* default on */

	যদি (argc == 3 && !strcmp(argv[1], "-b")) {
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

	প্রত্যাগম checkout_main(argc, argv, prefix, &opts, options,
			     CHECKOUT_CHECKOUT);
}

পূর্ণাঙ্ক cmd_switch(পূর্ণাঙ্ক argc,
	       ধ্রুব বর্ণ **argv,
	       ধ্রুব বর্ণ *prefix,
	       সংরচনা repository *repo UNUSED)
{
	সংরচনা checkout_opts opts = CHECKOUT_OPTS_INIT;
	সংরচনা option *options = রিক্ত;
	সংরচনা option switch_options[] = {
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
	opts.orphan_from_empty_বৃক্ষ = 1;
	opts.overlay_mode = -1;

	options = parse_options_dup(switch_options);
	options = add_common_options(&opts, options);
	options = add_common_switch_branch_options(&opts, options);

	cb_option = 'c';

	প্রত্যাগম checkout_main(argc, argv, prefix, &opts, options,
			     CHECKOUT_SWITCH);
}

পূর্ণাঙ্ক cmd_restore(পূর্ণাঙ্ক argc,
		ধ্রুব বর্ণ **argv,
		ধ্রুব বর্ণ *prefix,
		সংরচনা repository *repo UNUSED)
{
	সংরচনা checkout_opts opts = CHECKOUT_OPTS_INIT;
	সংরচনা option *options;
	সংরচনা option restore_options[] = {
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

	প্রত্যাগম checkout_main(argc, argv, prefix, &opts, options,
			     CHECKOUT_RESTORE);
}
