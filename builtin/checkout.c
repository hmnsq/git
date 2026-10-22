#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "advice.h"
#अन्तर्भाव "branch.h"
#अन्तर्भाव "cache-tree.h"
#अन्तर्भाव "checkout.h"
#अन्तर्भाव "commit.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "diff.h"
#अन्तर्भाव "dir.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "hook.h"
#अन्तर्भाव "merge-ll.h"
#अन्तर्भाव "lockfile.h"
#अन्तर्भाव "mem-pool.h"
#अन्तर्भाव "object-file.h"
#अन्तर्भाव "object-name.h"
#अन्तर्भाव "odb.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "path.h"
#अन्तर्भाव "preload-index.h"
#अन्तर्भाव "read-cache.h"
#अन्तर्भाव "refs.h"
#अन्तर्भाव "remote.h"
#अन्तर्भाव "repo-settings.h"
#अन्तर्भाव "resolve-undo.h"
#अन्तर्भाव "revision.h"
#अन्तर्भाव "sequencer.h"
#अन्तर्भाव "setup.h"
#अन्तर्भाव "sparse-index.h"
#अन्तर्भाव "strvec.h"
#अन्तर्भाव "submodule.h"
#अन्तर्भाव "symlinks.h"
#अन्तर्भाव "trace2.h"
#अन्तर्भाव "tree.h"
#अन्तर्भाव "tree-walk.h"
#अन्तर्भाव "unpack-trees.h"
#अन्तर्भाव "wt-status.h"
#अन्तर्भाव "xdiff-interface.h"
#अन्तर्भाव "entry.h"
#अन्तर्भाव "parallel-checkout.h"
#अन्तर्भाव "add-interactive.h"

संरचना checkout_opts {
	पूर्णाङ्क patch_mode;
	पूर्णाङ्क patch_context;
	पूर्णाङ्क patch_interhunk_context;
	पूर्णाङ्क auto_advance;
	पूर्णाङ्क quiet;
	पूर्णाङ्क merge;
	पूर्णाङ्क force;
	पूर्णाङ्क force_detach;
	पूर्णाङ्क implicit_detach;
	पूर्णाङ्क writeout_stage;
	पूर्णाङ्क overwrite_ignore;
	पूर्णाङ्क ignore_skipworktree;
	पूर्णाङ्क ignore_other_worktrees;
	पूर्णाङ्क show_progress;
	पूर्णाङ्क गिनती_checkout_paths;
	पूर्णाङ्क overlay_mode;
	पूर्णाङ्क dwim_new_local_branch;
	पूर्णाङ्क discard_changes;
	पूर्णाङ्क accept_ref;
	पूर्णाङ्क accept_pathspec;
	पूर्णाङ्क switch_branch_doing_nothing_is_ok;
	पूर्णाङ्क only_merge_on_switching_branches;
	पूर्णाङ्क can_switch_when_in_progress;
	पूर्णाङ्क orphan_from_empty_वृक्ष;
	पूर्णाङ्क empty_pathspec_ok;
	पूर्णाङ्क checkout_index;
	पूर्णाङ्क checkout_worktree;
	ध्रुव वर्ण *ignore_unmerged_opt;
	पूर्णाङ्क ignore_unmerged;
	पूर्णाङ्क pathspec_file_nul;
	वर्ण *pathspec_from_file;

	ध्रुव वर्ण *new_branch;
	ध्रुव वर्ण *new_branch_force;
	ध्रुव वर्ण *new_orphan_branch;
	पूर्णाङ्क new_branch_log;
	गणना branch_track track;
	संरचना diff_options diff_options;
	पूर्णाङ्क conflict_style;

	पूर्णाङ्क branch_exists;
	ध्रुव वर्ण *prefix;
	संरचना pathspec pathspec;
	ध्रुव वर्ण *from_treeish;
	संरचना वृक्ष *source_वृक्ष;
};

#define CHECKOUT_OPTS_INIT { \
	.conflict_style = -1, \
	.merge = -1, \
	.patch_context = -1, \
	.patch_interhunk_context = -1, \
	.auto_advance = 1, \
}

#define MERGE_WORKING_वृक्ष_UNPACK_FAILED (-2)

संरचना branch_info {
	वर्ण *name; /* The short name used */
	वर्ण *path; /* The full name of a real branch */
	संरचना commit *commit; /* The named commit */
	वर्ण *refname; /* The full name of the ref being checked out. */
	संरचना object_id oid; /* The object ID of the commit being checked out. */
	/*
	 * if not null the branch is detached because it's already
	 * checked out in this checkout
	 */
	वर्ण *checkout;
};

स्थिर शून्य branch_info_release(संरचना branch_info *info)
{
	मुक्त(info->name);
	मुक्त(info->path);
	मुक्त(info->refname);
	मुक्त(info->checkout);
}

स्थिर पूर्णाङ्क post_checkout_hook(संरचना commit *old_commit, संरचना commit *new_commit,
			      पूर्णाङ्क changed)
{
	संरचना run_hooks_opt opt = RUN_HOOKS_OPT_INIT_FORCE_SERIAL;

	/*
	 * "new_commit" can be NULL when checking out from the index before
	 * a commit exists.
	 */
	strvec_pushl(&opt.args,
		     oid_to_hex(old_commit ? &old_commit->object.oid : null_oid(the_hash_algo)),
		     oid_to_hex(new_commit ? &new_commit->object.oid : null_oid(the_hash_algo)),
		     changed ? "1" : "0",
		     रिक्त);

	प्रत्यागम run_hooks_opt(the_repository, "post-checkout", &opt);
}

/*
 * Handle a tree object and determine if we need to recurse into the
 * tree (READ_TREE_RECURSIVE) or skip it (0).
 */
स्थिर पूर्णाङ्क try_update_sparse_directory(ध्रुव संरचना object_id *oid,
				       संरचना strbuf *base,
				       ध्रुव वर्ण *pathname,
				       पूर्णाङ्क overlay_mode)
{
	संरचना strbuf dirpath = STRBUF_INIT;
	संरचना cache_entry *old;
	पूर्णाङ्क pos, result = READ_वृक्ष_RECURSIVE;

	यदि (!the_repository->index->sparse_index)
		प्रत्यागम result;

	strbuf_addbuf(&dirpath, base);
	strbuf_addstr(&dirpath, pathname);
	strbuf_addch(&dirpath, '/');

	pos = index_name_pos_sparse(the_repository->index,
				    dirpath.buf, dirpath.len);
	यदि (pos < 0)
		गच्छ cleanup;

	old = the_repository->index->cache[pos];
	यदि (!S_ISSPARSEDIR(old->ce_mode))
		गच्छ cleanup;

	यदि (oideq(oid, &old->oid)) {
		/* Tree content already matches; no need to descend. */
		result = 0;
	} अन्यथा यदि (!overlay_mode) {
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
	प्रत्यागम result;
}

स्थिर पूर्णाङ्क update_some(ध्रुव संरचना object_id *oid, संरचना strbuf *base,
		       ध्रुव वर्ण *pathname, अचिन्हित mode, शून्य *context)
{
	पूर्णाङ्क len;
	संरचना cache_entry *ce;
	पूर्णाङ्क pos;
	पूर्णाङ्क overlay_mode = context ? *((पूर्णाङ्क *)context) : 1;

	यदि (S_ISDIR(mode))
		प्रत्यागम try_update_sparse_directory(oid, base, pathname,
						   overlay_mode);

	len = base->len + म_माप(pathname);
	ce = make_empty_cache_entry(the_repository->index, len);
	oidcpy(&ce->oid, oid);
	स_नकल(ce->name, base->buf, base->len);
	स_नकल(ce->name + base->len, pathname, len - base->len);
	ce->ce_flags = create_ce_flags(0) | CE_UPDATE;
	ce->ce_namelen = len;
	ce->ce_mode = create_ce_mode(mode);

	/*
	 * If the entry is the same as the current index, we can leave the old
	 * entry in place. Whether it is UPTODATE or not, checkout_entry will
	 * do the right thing.
	 */
	pos = index_name_pos_sparse(the_repository->index, ce->name, ce->ce_namelen);
	यदि (pos >= 0) {
		संरचना cache_entry *old = the_repository->index->cache[pos];
		यदि (ce->ce_mode == old->ce_mode &&
		    !ce_intent_to_add(old) &&
		    oideq(&ce->oid, &old->oid)) {
			old->ce_flags |= CE_UPDATE;
			discard_cache_entry(ce);
			प्रत्यागम 0;
		}
	}

	add_index_entry(the_repository->index, ce,
			ADD_CACHE_OK_TO_ADD | ADD_CACHE_OK_TO_REPLACE);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क read_वृक्ष_some(संरचना वृक्ष *वृक्ष, ध्रुव संरचना pathspec *pathspec,
			  पूर्णाङ्क overlay_mode)
{
	read_वृक्ष(the_repository, वृक्ष,
		  pathspec, update_some, &overlay_mode);

	/* update the index with the given tree's info
	 * for all args, expanding wildcards, and exit
	 * with any non-zero return code.
	 */
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क skip_same_name(ध्रुव संरचना cache_entry *ce, पूर्णाङ्क pos)
{
	यावत् (++pos < the_repository->index->cache_nr &&
	       !म_भेद(the_repository->index->cache[pos]->name, ce->name))
		; /* skip */
	प्रत्यागम pos;
}

स्थिर पूर्णाङ्क check_stage(पूर्णाङ्क stage, ध्रुव संरचना cache_entry *ce, पूर्णाङ्क pos,
		       पूर्णाङ्क overlay_mode)
{
	यावत् (pos < the_repository->index->cache_nr &&
	       !म_भेद(the_repository->index->cache[pos]->name, ce->name)) {
		यदि (ce_stage(the_repository->index->cache[pos]) == stage)
			प्रत्यागम 0;
		pos++;
	}
	यदि (!overlay_mode)
		प्रत्यागम 0;
	यदि (stage == 2)
		प्रत्यागम error(_("path '%s' does not have our version"), ce->name);
	अन्यथा
		प्रत्यागम error(_("path '%s' does not have their version"), ce->name);
}

स्थिर पूर्णाङ्क check_stages(अचिन्हित stages, ध्रुव संरचना cache_entry *ce, पूर्णाङ्क pos)
{
	अचिन्हित seen = 0;
	ध्रुव वर्ण *name = ce->name;

	यावत् (pos < the_repository->index->cache_nr) {
		ce = the_repository->index->cache[pos];
		यदि (म_भेद(name, ce->name))
			भङ्ग;
		seen |= (1 << ce_stage(ce));
		pos++;
	}
	यदि ((stages & seen) != stages)
		प्रत्यागम error(_("path '%s' does not have all necessary versions"),
			     name);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क checkout_stage(पूर्णाङ्क stage, ध्रुव संरचना cache_entry *ce, पूर्णाङ्क pos,
			  ध्रुव संरचना checkout *state, पूर्णाङ्क *nr_checkouts,
			  पूर्णाङ्क overlay_mode)
{
	यावत् (pos < the_repository->index->cache_nr &&
	       !म_भेद(the_repository->index->cache[pos]->name, ce->name)) {
		यदि (ce_stage(the_repository->index->cache[pos]) == stage)
			प्रत्यागम checkout_entry(the_repository->index->cache[pos], state,
					      रिक्त, nr_checkouts);
		pos++;
	}
	यदि (!overlay_mode) {
		unlink_entry(ce, रिक्त);
		प्रत्यागम 0;
	}
	यदि (stage == 2)
		प्रत्यागम error(_("path '%s' does not have our version"), ce->name);
	अन्यथा
		प्रत्यागम error(_("path '%s' does not have their version"), ce->name);
}

स्थिर पूर्णाङ्क checkout_merged(पूर्णाङ्क pos, ध्रुव संरचना checkout *state,
			   पूर्णाङ्क *nr_checkouts, संरचना mem_pool *ce_mem_pool,
			   पूर्णाङ्क conflict_style)
{
	संरचना cache_entry *ce = the_repository->index->cache[pos];
	ध्रुव वर्ण *path = ce->name;
	mmfile_t ancestor, ours, theirs;
	गणना ll_merge_result merge_status;
	पूर्णाङ्क status;
	संरचना object_id oid;
	mmfile_t result_buf;
	संरचना object_id threeway[3];
	अचिन्हित mode = 0;
	संरचना ll_merge_options ll_opts = LL_MERGE_OPTIONS_INIT;
	पूर्णाङ्क renormalize = 0;

	स_रखो(threeway, 0, परिमाण(threeway));
	यावत् (pos < the_repository->index->cache_nr) {
		पूर्णाङ्क stage;
		stage = ce_stage(ce);
		यदि (!stage || म_भेद(path, ce->name))
			भङ्ग;
		oidcpy(&threeway[stage - 1], &ce->oid);
		यदि (stage == 2)
			mode = create_ce_mode(ce->ce_mode);
		pos++;
		ce = the_repository->index->cache[pos];
	}
	यदि (is_null_oid(&threeway[1]) || is_null_oid(&threeway[2]))
		प्रत्यागम error(_("path '%s' does not have necessary versions"), path);

	read_mmblob(&ancestor, the_repository->objects, &threeway[0]);
	read_mmblob(&ours, the_repository->objects, &threeway[1]);
	read_mmblob(&theirs, the_repository->objects, &threeway[2]);

	repo_config_get_bool(the_repository, "merge.renormalize", &renormalize);
	ll_opts.renormalize = renormalize;
	ll_opts.conflict_style = conflict_style;
	merge_status = ll_merge(&result_buf, path, &ancestor, "base",
				&ours, "ours", &theirs, "theirs",
				state->istate, &ll_opts);
	मुक्त(ancestor.ptr);
	मुक्त(ours.ptr);
	मुक्त(theirs.ptr);
	यदि (merge_status == LL_MERGE_द्विआधारी_CONFLICT)
		warning("Cannot merge binary files: %s (%s vs. %s)",
			path, "ours", "theirs");
	यदि (merge_status < 0 || !result_buf.ptr) {
		मुक्त(result_buf.ptr);
		प्रत्यागम error(_("path '%s': cannot merge"), path);
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
	यदि (odb_write_object(the_repository->objects, result_buf.ptr, result_buf.size, OBJ_BLOB, &oid))
		die(_("Unable to add merge result for '%s'"), path);
	मुक्त(result_buf.ptr);
	ce = make_transient_cache_entry(mode, &oid, path, 2, ce_mem_pool);
	यदि (!ce)
		die(_("make_cache_entry failed for path '%s'"), path);
	status = checkout_entry(ce, state, रिक्त, nr_checkouts);
	प्रत्यागम status;
}

स्थिर शून्य mark_ce_for_checkout_overlay(संरचना cache_entry *ce,
					 वर्ण *ps_matched,
					 ध्रुव संरचना checkout_opts *opts)
{
	ce->ce_flags &= ~CE_MATCHED;
	यदि (!opts->ignore_skipworktree && ce_skip_worktree(ce))
		प्रत्यागम;
	यदि (opts->source_वृक्ष && !(ce->ce_flags & CE_UPDATE))
		/*
		 * "git checkout tree-ish -- path", but this entry
		 * is in the original index but is not in tree-ish
		 * or does not match the pathspec; it will not be
		 * checked out to the working tree.  We will not do
		 * anything to this entry at all.
		 */
		प्रत्यागम;
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
	यदि (ce_path_match(the_repository->index, ce, &opts->pathspec, ps_matched))
		ce->ce_flags |= CE_MATCHED;
}

स्थिर शून्य mark_ce_for_checkout_no_overlay(संरचना cache_entry *ce,
					    वर्ण *ps_matched,
					    ध्रुव संरचना checkout_opts *opts)
{
	ce->ce_flags &= ~CE_MATCHED;
	यदि (!opts->ignore_skipworktree && ce_skip_worktree(ce))
		प्रत्यागम;
	यदि (ce_path_match(the_repository->index, ce, &opts->pathspec, ps_matched)) {
		ce->ce_flags |= CE_MATCHED;
		यदि (opts->source_वृक्ष && !(ce->ce_flags & CE_UPDATE))
			/*
			 * In overlay mode, but the path is not in
			 * tree-ish, which means we should remove it
			 * from the index and the working tree.
			 */
			ce->ce_flags |= CE_REMOVE | CE_WT_REMOVE;
	}
}

स्थिर पूर्णाङ्क checkout_worktree(ध्रुव संरचना checkout_opts *opts,
			     ध्रुव संरचना branch_info *info)
{
	संरचना checkout state = CHECKOUT_INIT;
	पूर्णाङ्क nr_checkouts = 0, nr_unmerged = 0;
	पूर्णाङ्क errs = 0;
	पूर्णाङ्क pos;
	पूर्णाङ्क pc_workers, pc_threshold;
	संरचना mem_pool ce_mem_pool;

	state.force = 1;
	state.refresh_cache = 1;
	state.istate = the_repository->index;

	mem_pool_init(&ce_mem_pool, 0);
	get_parallel_checkout_configs(&pc_workers, &pc_threshold);
	init_checkout_metadata(&state.meta, info->refname,
			       info->commit ? &info->commit->object.oid : &info->oid,
			       रिक्त);

	enable_delayed_checkout(&state);

	यदि (pc_workers > 1)
		init_parallel_checkout();

	प्रति (pos = 0; pos < the_repository->index->cache_nr; pos++) {
		संरचना cache_entry *ce = the_repository->index->cache[pos];
		यदि (ce->ce_flags & CE_MATCHED) {
			यदि (!ce_stage(ce)) {
				errs |= checkout_entry(ce, &state,
						       रिक्त, &nr_checkouts);
				अनुवर्त;
			}
			यदि (opts->writeout_stage)
				errs |= checkout_stage(opts->writeout_stage,
						       ce, pos,
						       &state,
						       &nr_checkouts, opts->overlay_mode);
			अन्यथा यदि (opts->merge)
				errs |= checkout_merged(pos, &state,
							&nr_unmerged,
							&ce_mem_pool,
							opts->conflict_style);
			pos = skip_same_name(ce, pos) - 1;
		}
	}
	यदि (pc_workers > 1)
		errs |= run_parallel_checkout(&state, pc_workers, pc_threshold,
					      रिक्त, रिक्त);
	mem_pool_discard(&ce_mem_pool, should_validate_cache_entries());
	remove_marked_cache_entries(the_repository->index, 1);
	remove_scheduled_dirs();
	errs |= finish_delayed_checkout(&state, opts->show_progress);

	यदि (opts->गिनती_checkout_paths) {
		यदि (nr_unmerged)
			fprintf_ln(मानक_त्रुटि, Q_("Recreated %d merge conflict",
					      "Recreated %d merge conflicts",
					      nr_unmerged),
				   nr_unmerged);
		यदि (opts->source_वृक्ष)
			fprintf_ln(मानक_त्रुटि, Q_("Updated %d path from %s",
					      "Updated %d paths from %s",
					      nr_checkouts),
				   nr_checkouts,
				   repo_find_unique_abbrev(the_repository, &opts->source_वृक्ष->object.oid,
							   DEFAULT_ABBREV));
		अन्यथा यदि (!nr_unmerged || nr_checkouts)
			fprintf_ln(मानक_त्रुटि, Q_("Updated %d path from the index",
					      "Updated %d paths from the index",
					      nr_checkouts),
				   nr_checkouts);
	}

	प्रत्यागम errs;
}

स्थिर पूर्णाङ्क checkout_paths(ध्रुव संरचना checkout_opts *opts,
			  ध्रुव संरचना branch_info *new_branch_info)
{
	पूर्णाङ्क pos;
	स्थिर वर्ण *ps_matched;
	संरचना object_id rev;
	संरचना commit *head;
	पूर्णाङ्क errs = 0;
	संरचना lock_file lock_file = LOCK_INIT;
	पूर्णाङ्क checkout_index;

	trace2_cmd_mode(opts->patch_mode ? "patch" : "path");

	यदि (opts->track != BRANCH_TRACK_UNSPECIFIED)
		die(_("'%s' cannot be used with updating paths"), "--track");

	यदि (opts->new_branch_log)
		die(_("'%s' cannot be used with updating paths"), "-l");

	यदि (opts->ignore_unmerged && opts->patch_mode)
		die(_("'%s' cannot be used with updating paths"),
		    opts->ignore_unmerged_opt);

	यदि (opts->force_detach)
		die(_("'%s' cannot be used with updating paths"), "--detach");

	यदि (opts->merge && opts->patch_mode)
		die(_("options '%s' and '%s' cannot be used together"), "--merge", "--patch");

	यदि (opts->ignore_unmerged && opts->merge)
		die(_("options '%s' and '%s' cannot be used together"),
		    opts->ignore_unmerged_opt, "-m");

	यदि (opts->new_branch)
		die(_("Cannot update paths and switch to branch '%s' at the same time."),
		    opts->new_branch);

	यदि (!opts->checkout_worktree && !opts->checkout_index)
		die(_("neither '%s' or '%s' is specified"),
		    "--staged", "--worktree");

	यदि (!opts->checkout_worktree && !opts->from_treeish)
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
	यदि (!opts->accept_ref && opts->checkout_index) {
		यदि (opts->writeout_stage)
			die(_("'%s' or '%s' cannot be used with %s"),
			    "--ours", "--theirs", "--staged");

		यदि (opts->merge)
			die(_("'%s' or '%s' cannot be used with %s"),
			    "--merge", "--conflict", "--staged");
	}

	/*
	 * recreating unmerged index entries and writing out data from
	 * unmerged index entries would make no sense when checking out
	 * of a tree-ish.
	 */
	यदि ((opts->merge || opts->writeout_stage) && opts->source_वृक्ष)
		die(_("'%s', '%s', or '%s' cannot be used when checking out of a tree"),
		    "--merge", "--ours", "--theirs");

	यदि (opts->patch_mode) {
		गणना add_p_mode patch_mode;
		संरचना interactive_options interactive_opts = {
			.context = opts->patch_context,
			.interhunkcontext = opts->patch_interhunk_context,
			.auto_advance = opts->auto_advance
		};
		ध्रुव वर्ण *rev = new_branch_info->name;
		वर्ण rev_oid[GIT_MAX_HEXSZ + 1];

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
		यदि (rev && new_branch_info->commit && म_भेद(rev, "HEAD"))
			rev = oid_to_hex_r(rev_oid, &new_branch_info->commit->object.oid);

		यदि (opts->checkout_index && opts->checkout_worktree)
			patch_mode = ADD_P_CHECKOUT;
		अन्यथा यदि (opts->checkout_index && !opts->checkout_worktree)
			patch_mode = ADD_P_RESET;
		अन्यथा यदि (!opts->checkout_index && opts->checkout_worktree)
			patch_mode = ADD_P_WORKTREE;
		अन्यथा
			BUG("either flag must have been set, worktree=%d, index=%d",
			    opts->checkout_worktree, opts->checkout_index);
		प्रत्यागम !!run_add_p(the_repository, patch_mode, &interactive_opts,
				   rev, &opts->pathspec, 0);
	}

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);
	यदि (repo_read_index_preload(the_repository, &opts->pathspec, 0) < 0)
		प्रत्यागम error(_("index file corrupt"));

	यदि (opts->source_वृक्ष)
		read_वृक्ष_some(opts->source_वृक्ष, &opts->pathspec,
			       opts->overlay_mode);
	यदि (opts->merge)
		unmerge_index(the_repository->index, &opts->pathspec, CE_MATCHED);

	ps_matched = xcalloc(opts->pathspec.nr, 1);

	/*
	 * Make sure all pathspecs participated in locating the paths
	 * to be checked out.
	 */
	प्रति (pos = 0; pos < the_repository->index->cache_nr; pos++)
		यदि (opts->overlay_mode)
			mark_ce_for_checkout_overlay(the_repository->index->cache[pos],
						     ps_matched,
						     opts);
		अन्यथा
			mark_ce_for_checkout_no_overlay(the_repository->index->cache[pos],
							ps_matched,
							opts);

	यदि (report_path_error(ps_matched, &opts->pathspec)) {
		मुक्त(ps_matched);
		प्रत्यागम 1;
	}
	मुक्त(ps_matched);

	/* Any unmerged paths? */
	प्रति (pos = 0; pos < the_repository->index->cache_nr; pos++) {
		ध्रुव संरचना cache_entry *ce = the_repository->index->cache[pos];
		यदि (ce->ce_flags & CE_MATCHED) {
			यदि (!ce_stage(ce))
				अनुवर्त;
			यदि (opts->ignore_unmerged) {
				यदि (!opts->quiet)
					warning(_("path '%s' is unmerged"), ce->name);
			} अन्यथा यदि (opts->writeout_stage) {
				errs |= check_stage(opts->writeout_stage, ce, pos, opts->overlay_mode);
			} अन्यथा यदि (opts->merge) {
				errs |= check_stages((1<<2) | (1<<3), ce, pos);
			} अन्यथा {
				errs = 1;
				error(_("path '%s' is unmerged"), ce->name);
			}
			pos = skip_same_name(ce, pos) - 1;
		}
	}
	यदि (errs)
		प्रत्यागम 1;

	/* Now we are committed to check them out */
	यदि (opts->checkout_worktree)
		errs |= checkout_worktree(opts, new_branch_info);
	अन्यथा
		remove_marked_cache_entries(the_repository->index, 1);

	/*
	 * Allow updating the index when checking out from the index.
	 * This is to save new stat info.
	 */
	यदि (opts->checkout_worktree && !opts->checkout_index && !opts->source_वृक्ष)
		checkout_index = 1;
	अन्यथा
		checkout_index = opts->checkout_index;

	यदि (checkout_index) {
		यदि (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
			die(_("unable to write new index file"));
	} अन्यथा {
		/*
		 * NEEDSWORK: if --worktree is not specified, we
		 * should save stat info of checked out files in the
		 * index to avoid the next (potentially costly)
		 * refresh. But it's a bit tricker to do...
		 */
		rollback_lock_file(&lock_file);
	}

	refs_read_ref_full(get_main_ref_store(the_repository), "HEAD", 0,
			   &rev, रिक्त);
	head = lookup_commit_reference_gently(the_repository, &rev, 1);

	errs |= post_checkout_hook(head, head, 0);
	प्रत्यागम errs;
}

स्थिर शून्य show_local_changes(संरचना object *head,
			       ध्रुव संरचना diff_options *opts)
{
	संरचना rev_info rev;
	/* I think we want full paths, even if we're in a subdirectory. */
	repo_init_revisions(the_repository, &rev, रिक्त);
	rev.diffopt.flags = opts->flags;
	rev.diffopt.output_format |= DIFF_FORMAT_NAME_STATUS;
	rev.diffopt.flags.recursive = 1;
	diff_setup_done(&rev.diffopt);
	add_pending_object(&rev, head, रिक्त);
	run_diff_index(&rev, 0);
	release_revisions(&rev);
}

स्थिर शून्य describe_detached_head(ध्रुव वर्ण *संदेश, संरचना commit *commit)
{
	संरचना strbuf sb = STRBUF_INIT;

	यदि (!repo_parse_commit(the_repository, commit))
		pp_commit_easy(CMIT_FMT_ONELINE, commit, &sb);
	यदि (print_sha1_ellipsis()) {
		ख_लिखो(मानक_त्रुटि, "%s %s... %s\n", संदेश,
			repo_find_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV),
			sb.buf);
	} अन्यथा {
		ख_लिखो(मानक_त्रुटि, "%s %s %s\n", संदेश,
			repo_find_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV),
			sb.buf);
	}
	strbuf_release(&sb);
}

स्थिर पूर्णाङ्क reset_वृक्ष(संरचना वृक्ष *वृक्ष, ध्रुव संरचना checkout_opts *o,
		      पूर्णाङ्क worktree, पूर्णाङ्क *writeout_error,
		      संरचना branch_info *info)
{
	संरचना unpack_trees_options opts;
	संरचना वृक्ष_desc वृक्ष_desc;

	स_रखो(&opts, 0, परिमाण(opts));
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
			       रिक्त);
	यदि (repo_parse_वृक्ष(the_repository, वृक्ष) < 0)
		प्रत्यागम 128;
	init_वृक्ष_desc(&वृक्ष_desc, &वृक्ष->object.oid, वृक्ष->buffer, वृक्ष->size);
	विकल्प (unpack_trees(1, &वृक्ष_desc, &opts)) {
	पक्ष -2:
		*writeout_error = 1;
		/*
		 * We return 0 nevertheless, as the index is all right
		 * and more importantly we have made best efforts to
		 * update paths in the work tree, and we cannot revert
		 * them.
		 */
		/* fallthrough */
	पक्ष 0:
		प्रत्यागम 0;
	स्वभाव:
		प्रत्यागम 128;
	}
}

स्थिर शून्य setup_branch_path(संरचना branch_info *branch)
{
	संरचना strbuf buf = STRBUF_INIT;

	/*
	 * If this is a ref, resolve it; otherwise, look up the OID for our
	 * expression.  Failure here is okay.
	 */
	यदि (!repo_dwim_ref(the_repository, branch->name, म_माप(branch->name),
			   &branch->oid, &branch->refname, 0))
		repo_get_oid_committish(the_repository, branch->name, &branch->oid);

	copy_branchname(the_repository, &buf, branch->name, INTERPRET_BRANCH_LOCAL);
	यदि (म_भेद(buf.buf, branch->name)) {
		मुक्त(branch->name);
		branch->name = xstrdup(buf.buf);
	}
	strbuf_splice(&buf, 0, 0, "refs/heads/", 11);
	मुक्त(branch->path);
	branch->path = strbuf_detach(&buf, रिक्त);
}

स्थिर शून्य init_topts(संरचना unpack_trees_options *topts,
		       पूर्णाङ्क show_progress, पूर्णाङ्क overwrite_ignore,
		       bool quiet)
{
	स_रखो(topts, 0, परिमाण(*topts));
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

स्थिर पूर्णाङ्क merge_working_वृक्ष(ध्रुव संरचना checkout_opts *opts,
			      संरचना branch_info *old_branch_info,
			      संरचना branch_info *new_branch_info,
			      bool quiet,
			      पूर्णाङ्क *writeout_error)
{
	पूर्णाङ्क ret;
	संरचना lock_file lock_file = LOCK_INIT;
	संरचना वृक्ष *new_वृक्ष;

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);
	यदि (repo_read_index_preload(the_repository, रिक्त, 0) < 0) {
		rollback_lock_file(&lock_file);
		प्रत्यागम error(_("index file corrupt"));
	}

	resolve_undo_clear_index(the_repository->index);
	यदि (opts->new_orphan_branch && opts->orphan_from_empty_वृक्ष) {
		यदि (new_branch_info->commit)
			BUG("'switch --orphan' should never accept a commit as starting point");
		new_वृक्ष = repo_parse_वृक्ष_indirect(the_repository,
						    the_hash_algo->empty_वृक्ष);
		यदि (!new_वृक्ष)
			BUG("unable to read empty tree");
	} अन्यथा {
		new_वृक्ष = repo_get_commit_वृक्ष(the_repository,
						new_branch_info->commit);
		यदि (!new_वृक्ष) {
			rollback_lock_file(&lock_file);
			प्रत्यागम error(_("unable to read tree (%s)"),
				     oid_to_hex(&new_branch_info->commit->object.oid));
		}
	}
	यदि (opts->discard_changes) {
		ret = reset_वृक्ष(new_वृक्ष, opts, 1, writeout_error, new_branch_info);
		यदि (ret) {
			rollback_lock_file(&lock_file);
			प्रत्यागम ret;
		}
	} अन्यथा {
		संरचना वृक्ष_desc trees[2];
		संरचना वृक्ष *वृक्ष;
		संरचना unpack_trees_options topts;
		ध्रुव संरचना object_id *old_commit_oid;

		refresh_index(the_repository->index, REFRESH_QUIET, रिक्त, रिक्त, रिक्त);

		यदि (unmerged_index(the_repository->index)) {
			rollback_lock_file(&lock_file);
			error(_("you need to resolve your current index first"));
			प्रत्यागम 1;
		}

		/* 2-way merge to the new branch */
		init_topts(&topts, opts->show_progress,
			   opts->overwrite_ignore, quiet);
		init_checkout_metadata(&topts.meta, new_branch_info->refname,
				       new_branch_info->commit ?
				       &new_branch_info->commit->object.oid :
				       &new_branch_info->oid, रिक्त);

		old_commit_oid = old_branch_info->commit ?
			&old_branch_info->commit->object.oid :
			the_hash_algo->empty_वृक्ष;
		वृक्ष = repo_parse_वृक्ष_indirect(the_repository,
						old_commit_oid);
		यदि (!वृक्ष)
			die(_("unable to parse commit %s"),
				oid_to_hex(old_commit_oid));

		init_वृक्ष_desc(&trees[0], &वृक्ष->object.oid,
			       वृक्ष->buffer, वृक्ष->size);
		यदि (repo_parse_वृक्ष(the_repository, new_वृक्ष) < 0)
			die(रिक्त);
		वृक्ष = new_वृक्ष;
		init_वृक्ष_desc(&trees[1], &वृक्ष->object.oid,
			       वृक्ष->buffer, वृक्ष->size);

		ret = unpack_trees(2, trees, &topts);
		clear_unpack_trees_porcelain(&topts);
		यदि (ret == -1) {
			rollback_lock_file(&lock_file);
			प्रत्यागम MERGE_WORKING_वृक्ष_UNPACK_FAILED;
		}
	}

	यदि (!cache_वृक्ष_fully_valid(the_repository->index))
		cache_वृक्ष_update(the_repository->index, WRITE_वृक्ष_SILENT | WRITE_वृक्ष_REPAIR);

	यदि (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
		die(_("unable to write new index file"));

	यदि (!opts->discard_changes && !opts->quiet && new_branch_info->commit)
		show_local_changes(&new_branch_info->commit->object, &opts->diff_options);

	प्रत्यागम 0;
}

स्थिर शून्य report_tracking(संरचना branch_info *new_branch_info)
{
	संरचना strbuf sb = STRBUF_INIT;
	संरचना branch *branch = branch_get(new_branch_info->name);

	यदि (!format_tracking_info(branch, &sb, AHEAD_BEHIND_FULL, 1))
		प्रत्यागम;
	ख_माला_दो(sb.buf, मानक_निकास);
	strbuf_release(&sb);
}

स्थिर शून्य update_refs_for_switch(ध्रुव संरचना checkout_opts *opts,
				   संरचना branch_info *old_branch_info,
				   संरचना branch_info *new_branch_info)
{
	संरचना strbuf संदेश = STRBUF_INIT;
	ध्रुव वर्ण *old_desc, *reflog_संदेश;
	यदि (opts->new_branch) {
		यदि (opts->new_orphan_branch) {
			गणना log_refs_config log_all_ref_updates = LOG_REFS_UNSET;
			ध्रुव वर्ण *मूल्य;
			वर्ण *refname;

			यदि (!repo_config_get_string_tmp(the_repository, "core.logallrefupdates", &मूल्य))
				log_all_ref_updates = refs_parse_log_all_ref_updates_config(मूल्य);

			refname = mkpathdup("refs/heads/%s", opts->new_orphan_branch);
			यदि (opts->new_branch_log &&
			    !should_autocreate_reflog(log_all_ref_updates, refname)) {
				पूर्णाङ्क ret;
				संरचना strbuf err = STRBUF_INIT;

				ret = refs_create_reflog(get_main_ref_store(the_repository),
							 refname, &err);
				यदि (ret) {
					ख_लिखो(मानक_त्रुटि, _("Can not do reflog for '%s': %s\n"),
						opts->new_orphan_branch, err.buf);
					strbuf_release(&err);
					मुक्त(refname);
					प्रत्यागम;
				}
				strbuf_release(&err);
			}
			मुक्त(refname);
		}
		अन्यथा
			create_branch(the_repository,
				      opts->new_branch, new_branch_info->name,
				      opts->new_branch_force ? 1 : 0,
				      opts->new_branch_force ? 1 : 0,
				      opts->new_branch_log,
				      opts->quiet,
				      opts->track,
				      0);
		मुक्त(new_branch_info->name);
		मुक्त(new_branch_info->refname);
		new_branch_info->name = xstrdup(opts->new_branch);
		setup_branch_path(new_branch_info);
	}

	old_desc = old_branch_info->name;
	यदि (!old_desc && old_branch_info->commit)
		old_desc = oid_to_hex(&old_branch_info->commit->object.oid);

	reflog_संदेश = दो_पर्या("GIT_REFLOG_ACTION");
	यदि (!reflog_संदेश)
		strbuf_addf(&संदेश, "checkout: moving from %s to %s",
			old_desc ? old_desc : "(invalid)", new_branch_info->name);
	अन्यथा
		strbuf_insertstr(&संदेश, 0, reflog_संदेश);

	यदि (!म_भेद(new_branch_info->name, "HEAD") && !new_branch_info->path && !opts->force_detach) {
		/* Nothing to do. */
	} अन्यथा यदि (opts->force_detach || !new_branch_info->path) {	/* No longer on any branch. */
		refs_update_ref(get_main_ref_store(the_repository), संदेश.buf,
				"HEAD", &new_branch_info->commit->object.oid,
				रिक्त,
				REF_NO_DEREF, UPDATE_REFS_DIE_ON_ERR);
		यदि (!opts->quiet) {
			यदि (old_branch_info->path &&
			    advice_enabled(ADVICE_DETACHED_HEAD) && !opts->force_detach)
				detach_advice(new_branch_info->name);
			describe_detached_head(_("HEAD is now at"), new_branch_info->commit);
		}
	} अन्यथा यदि (new_branch_info->path) {	/* Switch branches. */
		यदि (refs_update_symref(get_main_ref_store(the_repository), "HEAD", new_branch_info->path, संदेश.buf) < 0)
			die(_("unable to update HEAD"));
		यदि (!opts->quiet) {
			यदि (old_branch_info->path && !म_भेद(new_branch_info->path, old_branch_info->path)) {
				यदि (opts->new_branch_force)
					ख_लिखो(मानक_त्रुटि, _("Reset branch '%s'\n"),
						new_branch_info->name);
				अन्यथा
					ख_लिखो(मानक_त्रुटि, _("Already on '%s'\n"),
						new_branch_info->name);
			} अन्यथा यदि (opts->new_branch) {
				यदि (opts->branch_exists)
					ख_लिखो(मानक_त्रुटि, _("Switched to and reset branch '%s'\n"), new_branch_info->name);
				अन्यथा
					ख_लिखो(मानक_त्रुटि, _("Switched to a new branch '%s'\n"), new_branch_info->name);
			} अन्यथा {
				ख_लिखो(मानक_त्रुटि, _("Switched to branch '%s'\n"),
					new_branch_info->name);
			}
		}
		यदि (old_branch_info->path && old_branch_info->name) {
			यदि (!refs_ref_exists(get_main_ref_store(the_repository), old_branch_info->path) && refs_reflog_exists(get_main_ref_store(the_repository), old_branch_info->path))
				refs_delete_reflog(get_main_ref_store(the_repository),
						   old_branch_info->path);
		}
	}
	remove_branch_state(the_repository, !opts->quiet);
	strbuf_release(&संदेश);
	यदि (!opts->quiet &&
	    !opts->force_detach &&
	    (new_branch_info->path || !म_भेद(new_branch_info->name, "HEAD")))
		report_tracking(new_branch_info);
}

स्थिर पूर्णाङ्क add_pending_uninteresting_ref(ध्रुव संरचना reference *ref, शून्य *cb_data)
{
	add_pending_oid(cb_data, ref->name, ref->oid, UNINTERESTING);
	प्रत्यागम 0;
}

स्थिर शून्य describe_one_orphan(संरचना strbuf *sb, संरचना commit *commit)
{
	strbuf_addstr(sb, "  ");
	strbuf_add_unique_abbrev(sb, &commit->object.oid, DEFAULT_ABBREV);
	strbuf_addch(sb, ' ');
	यदि (!repo_parse_commit(the_repository, commit))
		pp_commit_easy(CMIT_FMT_ONELINE, commit, sb);
	strbuf_addch(sb, '\n');
}

#define ORPHAN_CUTOFF 4
स्थिर शून्य suggest_reattach(संरचना commit *commit, संरचना rev_info *revs)
{
	संरचना commit *c, *last = रिक्त;
	संरचना strbuf sb = STRBUF_INIT;
	पूर्णाङ्क lost = 0;
	यावत् ((c = get_revision(revs)) != रिक्त) {
		यदि (lost < ORPHAN_CUTOFF)
			describe_one_orphan(&sb, c);
		last = c;
		lost++;
	}
	यदि (ORPHAN_CUTOFF < lost) {
		पूर्णाङ्क more = lost - ORPHAN_CUTOFF;
		यदि (more == 1)
			describe_one_orphan(&sb, last);
		अन्यथा
			strbuf_addf(&sb, _(" ... and %d more.\n"), more);
	}

	ख_लिखो(मानक_त्रुटि,
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

	यदि (advice_enabled(ADVICE_DETACHED_HEAD))
		ख_लिखो(मानक_त्रुटि,
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
स्थिर शून्य orphaned_commit_warning(संरचना commit *old_commit, संरचना commit *new_commit)
{
	संरचना rev_info revs;
	संरचना object *object = &old_commit->object;

	repo_init_revisions(the_repository, &revs, रिक्त);
	setup_revisions(0, रिक्त, &revs, रिक्त);

	object->flags &= ~UNINTERESTING;
	add_pending_object(&revs, object, oid_to_hex(&object->oid));

	refs_for_each_ref(get_main_ref_store(the_repository),
			  add_pending_uninteresting_ref, &revs);
	यदि (new_commit)
		add_pending_oid(&revs, "HEAD",
				&new_commit->object.oid,
				UNINTERESTING);

	यदि (prepare_revision_walk(&revs))
		die(_("internal error in revision walk"));
	यदि (!(old_commit->object.flags & UNINTERESTING))
		suggest_reattach(old_commit, &revs);
	अन्यथा
		describe_detached_head(_("Previous HEAD position was"), old_commit);

	/* Clean up objects used, as they will be reused. */
	repo_clear_commit_marks(the_repository, ALL_REV_FLAGS);
	release_revisions(&revs);
}

स्थिर पूर्णाङ्क switch_branches(ध्रुव संरचना checkout_opts *opts,
			   संरचना branch_info *new_branch_info)
{
	पूर्णाङ्क ret = 0;
	संरचना branch_info old_branch_info = { 0 };
	संरचना object_id rev;
	पूर्णाङ्क flag, writeout_error = 0;
	पूर्णाङ्क do_merge = 1;
	पूर्णाङ्क created_autostash = 0;
	गणना stash_apply_result autostash_res = STASH_APPLY_CLEAN;
	संरचना strbuf old_commit_shortname = STRBUF_INIT;
	संरचना strbuf autostash_संदेश = STRBUF_INIT;
	ध्रुव वर्ण *stash_label_base = रिक्त;

	trace2_cmd_mode("branch");

	स_रखो(&old_branch_info, 0, परिमाण(old_branch_info));
	old_branch_info.path = refs_resolve_refdup(get_main_ref_store(the_repository),
						   "HEAD", 0, &rev, &flag);
	यदि (old_branch_info.path)
		old_branch_info.commit = lookup_commit_reference_gently(the_repository, &rev, 1);
	यदि (!(flag & REF_ISSYMREF))
		FREE_AND_NULL(old_branch_info.path);

	यदि (old_branch_info.path) {
		ध्रुव वर्ण *ध्रुव prefix = "refs/heads/";
		ध्रुव वर्ण *p;
		यदि (skip_prefix(old_branch_info.path, prefix, &p))
			old_branch_info.name = xstrdup(p);
	}

	यदि (opts->new_orphan_branch && opts->orphan_from_empty_वृक्ष) {
		यदि (new_branch_info->name)
			BUG("'switch --orphan' should never accept a commit as starting point");
		new_branch_info->commit = रिक्त;
		new_branch_info->name = xstrdup("(empty)");
		do_merge = 1;
	}

	यदि (!new_branch_info->name) {
		new_branch_info->name = xstrdup("HEAD");
		new_branch_info->commit = old_branch_info.commit;
		यदि (!new_branch_info->commit)
			die(_("You are on a branch yet to be born"));
		parse_commit_or_die(new_branch_info->commit);

		यदि (opts->only_merge_on_switching_branches)
			do_merge = 0;
	}

	यदि (old_branch_info.name) {
		stash_label_base = old_branch_info.name;
	} अन्यथा यदि (old_branch_info.commit) {
		strbuf_add_unique_abbrev(&old_commit_shortname,
					 &old_branch_info.commit->object.oid,
					 DEFAULT_ABBREV);
		stash_label_base = old_commit_shortname.buf;
	}

	यदि (do_merge) {
		ret = merge_working_वृक्ष(opts, &old_branch_info, new_branch_info,
					 opts->merge, &writeout_error);
		यदि (ret == MERGE_WORKING_वृक्ष_UNPACK_FAILED && opts->merge) {
			strbuf_addf(&autostash_संदेश,
				    "autostash while switching to '%s'",
				    new_branch_info->name);
			create_autostash_ref(the_repository,
					     "CHECKOUT_AUTOSTASH_HEAD",
					     autostash_संदेश.buf, true);
			created_autostash = 1;
			ret = merge_working_वृक्ष(opts, &old_branch_info, new_branch_info,
						 false, &writeout_error);
		}
		यदि (created_autostash) {
			यदि (opts->conflict_style >= 0) {
				संरचना strbuf cfg = STRBUF_INIT;
				strbuf_addf(&cfg, "merge.conflictStyle=%s",
					    conflict_style_name(opts->conflict_style));
				git_config_धकेलो_parameter(cfg.buf);
				strbuf_release(&cfg);
			}
			autostash_res = apply_autostash_ref(the_repository,
				    "CHECKOUT_AUTOSTASH_HEAD",
				    new_branch_info->name,
				    "local",
				    stash_label_base,
				    autostash_संदेश.buf);
		}
		यदि (ret) {
			branch_info_release(&old_branch_info);
			strbuf_release(&old_commit_shortname);
			strbuf_release(&autostash_संदेश);
			प्रत्यागम ret < 0 ? 1 : ret;
		}
	}

	यदि (!opts->quiet && !old_branch_info.path && old_branch_info.commit && new_branch_info->commit != old_branch_info.commit)
		orphaned_commit_warning(old_branch_info.commit, new_branch_info->commit);

	यदि (autostash_res == STASH_APPLY_CONFLICT && !opts->quiet)
		ख_अक्षर_दो('\n', मानक_त्रुटि);
	update_refs_for_switch(opts, &old_branch_info, new_branch_info);

	यदि (created_autostash) {
		discard_index(the_repository->index);
		यदि (repo_read_index(the_repository) < 0)
			die(_("index file corrupt"));

		यदि (!opts->quiet && new_branch_info->commit) {
			म_लिखो(_("The following paths have local changes:\n"));
			show_local_changes(&new_branch_info->commit->object,
					   &opts->diff_options);
		}
	}

	ret = post_checkout_hook(old_branch_info.commit, new_branch_info->commit, 1);
	branch_info_release(&old_branch_info);
	strbuf_release(&old_commit_shortname);
	strbuf_release(&autostash_संदेश);

	प्रत्यागम ret || writeout_error;
}

स्थिर पूर्णाङ्क git_checkout_config(ध्रुव वर्ण *var, ध्रुव वर्ण *मूल्य,
			       ध्रुव संरचना config_context *ctx, शून्य *cb)
{
	संरचना checkout_opts *opts = cb;

	यदि (!म_भेद(var, "diff.ignoresubmodules")) {
		यदि (!मूल्य)
			प्रत्यागम config_error_nonbool(var);
		handle_ignore_submodules_arg(&opts->diff_options, मूल्य);
		प्रत्यागम 0;
	}
	यदि (!म_भेद(var, "checkout.guess")) {
		opts->dwim_new_local_branch = git_config_bool(var, मूल्य);
		प्रत्यागम 0;
	}

	यदि (starts_with(var, "submodule."))
		प्रत्यागम git_default_submodule_config(var, मूल्य, रिक्त);

	प्रत्यागम git_xmerge_config(var, मूल्य, ctx, रिक्त);
}

स्थिर शून्य setup_new_branch_info_and_source_वृक्ष(
	संरचना branch_info *new_branch_info,
	संरचना checkout_opts *opts,
	संरचना object_id *rev,
	ध्रुव वर्ण *arg)
{
	संरचना वृक्ष **source_वृक्ष = &opts->source_वृक्ष;
	संरचना object_id branch_rev;

	/* treat '@' as a shortcut for 'HEAD' */
	new_branch_info->name = !म_भेद(arg, "@") ? xstrdup("HEAD") :
						    xstrdup(arg);
	setup_branch_path(new_branch_info);

	यदि (!check_refname_format(new_branch_info->path, 0) &&
	    !refs_read_ref(get_main_ref_store(the_repository), new_branch_info->path, &branch_rev))
		oidcpy(rev, &branch_rev);
	अन्यथा
		/* not an existing branch */
		FREE_AND_NULL(new_branch_info->path);

	new_branch_info->commit = lookup_commit_reference_gently(the_repository, rev, 1);
	यदि (!new_branch_info->commit) {
		/* not a commit */
		*source_वृक्ष = repo_parse_वृक्ष_indirect(the_repository, rev);
		यदि (!*source_वृक्ष)
			die(_("unable to read tree (%s)"), oid_to_hex(rev));
	} अन्यथा {
		parse_commit_or_die(new_branch_info->commit);
		*source_वृक्ष = repo_get_commit_वृक्ष(the_repository,
						    new_branch_info->commit);
		यदि (!*source_वृक्ष)
			die(_("unable to read tree (%s)"),
			    oid_to_hex(&new_branch_info->commit->object.oid));
	}
}


गणना checkout_command {
	CHECKOUT_CHECKOUT = 1,
	CHECKOUT_SWITCH = 2,
	CHECKOUT_RESTORE = 3,
};

स्थिर शून्य advise_disambiguating_remotes(गणना checkout_command which_command,
					  ध्रुव वर्ण *branch,
					  ध्रुव संरचना string_list *matched_remote_names)
{
	ध्रुव वर्ण *cmdname;
	संरचना string_list_item *item;

	विकल्प (which_command) {
	पक्ष CHECKOUT_CHECKOUT:
		cmdname = "checkout";
		भङ्ग;
	पक्ष CHECKOUT_SWITCH:
		cmdname = "switch";
		भङ्ग;
	स्वभाव:
		BUG("command <%d> should not reach advise_disambiguating_remotes",
		    which_command);
		भङ्ग;
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

स्थिर वर्ण *parse_remote_branch(ध्रुव वर्ण *arg,
				 संरचना object_id *rev,
				 पूर्णाङ्क could_be_checkout_paths,
				 गणना checkout_command which_command)
{
	पूर्णाङ्क अंक_matches = 0;
	संरचना string_list matched_remote_names = STRING_LIST_INIT_DUP;

	वर्ण *remote = unique_tracking_name(arg, rev, &अंक_matches,
					    &matched_remote_names);

	यदि (remote && could_be_checkout_paths) {
		die(_("'%s' could be both a local file and a tracking branch.\n"
			"Please use -- (and optionally --no-guess) to disambiguate"),
		    arg);
	}

	यदि (!remote && अंक_matches > 1) {
		यदि (advice_enabled(ADVICE_CHECKOUT_AMBIGUOUS_REMOTE_BRANCH_NAME))
			advise_disambiguating_remotes(which_command, arg,
						      &matched_remote_names);
		die(_("'%s' matched multiple (%d) remote tracking branches"),
		    arg, अंक_matches);
	}

	string_list_clear(&matched_remote_names, 0);

	प्रत्यागम remote;
}

स्थिर पूर्णाङ्क parse_branchname_arg(पूर्णाङ्क argc, ध्रुव वर्ण **argv,
				पूर्णाङ्क dwim_new_local_branch_ok,
				गणना checkout_command which_command,
				संरचना branch_info *new_branch_info,
				संरचना checkout_opts *opts,
				संरचना object_id *rev)
{
	ध्रुव वर्ण **new_branch = &opts->new_branch;
	पूर्णाङ्क argcount = 0;
	ध्रुव वर्ण *arg;
	वर्ण *remote = रिक्त;
	पूर्णाङ्क dash_dash_pos;
	पूर्णाङ्क has_dash_dash = 0;
	पूर्णाङ्क i;

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
	यदि (!argc)
		प्रत्यागम 0;

	यदि (!opts->accept_pathspec) {
		यदि (argc > 1)
			die(_("only one reference expected"));
		has_dash_dash = 1; /* helps disambiguate */
	}

	arg = argv[0];
	dash_dash_pos = -1;
	प्रति (i = 0; i < argc; i++) {
		यदि (opts->accept_pathspec && !म_भेद(argv[i], "--")) {
			dash_dash_pos = i;
			भङ्ग;
		}
	}
	यदि (dash_dash_pos == 0)
		प्रत्यागम 1; /* case (2) */
	अन्यथा यदि (dash_dash_pos == 1)
		has_dash_dash = 1; /* case (3) or (1) */
	अन्यथा यदि (dash_dash_pos >= 2)
		die(_("only one reference expected, %d given."), dash_dash_pos);
	opts->गिनती_checkout_paths = !opts->quiet && !has_dash_dash;

	यदि (!म_भेद(arg, "-"))
		arg = "@{-1}";

	यदि (repo_get_oid_mb(the_repository, arg, rev)) {
		/*
		 * Either case (3) or (4), with <something> not being
		 * a commit, or an attempt to use case (1) with an
		 * invalid ref.
		 *
		 * It's likely an error, but we need to find out if
		 * we should auto-create the branch, case (3).(b).
		 */
		पूर्णाङ्क recover_with_dwim = dwim_new_local_branch_ok;

		पूर्णाङ्क could_be_checkout_paths = !has_dash_dash &&
			check_filename(opts->prefix, arg);

		यदि (!has_dash_dash && !no_wildcard(arg))
			recover_with_dwim = 0;

		/*
		 * Accept "git checkout foo", "git checkout foo --"
		 * and "git switch foo" as candidates for dwim.
		 */
		यदि (!(argc == 1 && !has_dash_dash) &&
		    !(argc == 2 && has_dash_dash) &&
		    opts->accept_pathspec)
			recover_with_dwim = 0;

		यदि (recover_with_dwim) {
			remote = parse_remote_branch(arg, rev,
						     could_be_checkout_paths,
						     which_command);
			यदि (remote) {
				*new_branch = arg;
				arg = remote;
				/* DWIMmed to create local branch, case (3).(b) */
			} अन्यथा {
				recover_with_dwim = 0;
			}
		}

		यदि (!recover_with_dwim) {
			यदि (has_dash_dash)
				die(_("invalid reference: %s"), arg);
			प्रत्यागम argcount;
		}
	}

	/* we can't end up being in (2) anymore, eat the argument */
	argcount++;
	argv++;
	argc--;

	setup_new_branch_info_and_source_वृक्ष(new_branch_info, opts, rev, arg);

	यदि (!opts->source_वृक्ष)                   /* case (1): want a tree */
		die(_("reference is not a tree: %s"), arg);

	यदि (!has_dash_dash) {	/* case (3).(d) -> (1) */
		/*
		 * Do not complain the most common case
		 *	git checkout branch
		 * even if there happen to be a file called 'branch';
		 * it would be extremely annoying.
		 */
		यदि (argc)
			verify_non_filename(the_repository, opts->prefix, arg);
	} अन्यथा यदि (opts->accept_pathspec) {
		argcount++;
		argv++;
		argc--;
	}

	मुक्त(remote);
	प्रत्यागम argcount;
}

स्थिर पूर्णाङ्क switch_unborn_to_new_branch(ध्रुव संरचना checkout_opts *opts)
{
	पूर्णाङ्क status;
	संरचना strbuf branch_ref = STRBUF_INIT;

	trace2_cmd_mode("unborn");

	यदि (!opts->new_branch)
		die(_("You are on a branch yet to be born"));
	strbuf_addf(&branch_ref, "refs/heads/%s", opts->new_branch);
	status = refs_update_symref(get_main_ref_store(the_repository),
				    "HEAD", branch_ref.buf, "checkout -b");
	strbuf_release(&branch_ref);
	यदि (!opts->quiet)
		ख_लिखो(मानक_त्रुटि, _("Switched to a new branch '%s'\n"),
			opts->new_branch);
	प्रत्यागम status;
}

स्थिर शून्य die_expecting_a_branch(ध्रुव संरचना branch_info *branch_info)
{
	संरचना object_id oid;
	वर्ण *to_free;
	पूर्णाङ्क code;

	यदि (repo_dwim_ref(the_repository, branch_info->name,
			  म_माप(branch_info->name), &oid, &to_free, 0) == 1) {
		ध्रुव वर्ण *ref = to_free;

		यदि (skip_prefix(ref, "refs/tags/", &ref))
			code = die_message(_("a branch is expected, got tag '%s'"), ref);
		अन्यथा यदि (skip_prefix(ref, "refs/remotes/", &ref))
			code = die_message(_("a branch is expected, got remote branch '%s'"), ref);
		अन्यथा
			code = die_message(_("a branch is expected, got '%s'"), ref);
	}
	अन्यथा यदि (branch_info->commit)
		code = die_message(_("a branch is expected, got commit '%s'"), branch_info->name);
	अन्यथा
		/*
		 * This case should never happen because we already die() on
		 * non-commit, but just in case.
		 */
		code = die_message(_("a branch is expected, got '%s'"), branch_info->name);

	यदि (advice_enabled(ADVICE_SUGGEST_DETACHING_HEAD))
		advise(_("If you want to detach HEAD at the commit, try again with the --detach option."));

	निकास(code);
}

स्थिर शून्य die_if_some_operation_in_progress(शून्य)
{
	संरचना wt_status_state state;

	स_रखो(&state, 0, परिमाण(state));
	wt_status_get_state(the_repository, &state, 0);

	यदि (state.merge_in_progress)
		die(_("cannot switch branch while merging\n"
		      "Consider \"git merge --quit\" "
		      "or \"git worktree add\"."));
	यदि (state.am_in_progress)
		die(_("cannot switch branch in the middle of an am session\n"
		      "Consider \"git am --quit\" "
		      "or \"git worktree add\"."));
	यदि (state.rebase_interactive_in_progress || state.rebase_in_progress)
		die(_("cannot switch branch while rebasing\n"
		      "Consider \"git rebase --quit\" "
		      "or \"git worktree add\"."));
	यदि (state.cherry_pick_in_progress)
		die(_("cannot switch branch while cherry-picking\n"
		      "Consider \"git cherry-pick --quit\" "
		      "or \"git worktree add\"."));
	यदि (state.revert_in_progress)
		die(_("cannot switch branch while reverting\n"
		      "Consider \"git revert --quit\" "
		      "or \"git worktree add\"."));
	यदि (state.bisect_in_progress)
		warning(_("you are switching branch while bisecting"));

	wt_status_state_free_buffers(&state);
}

/*
 * die if attempting to checkout an existing branch that is in use
 * in another worktree, unless ignore-other-wortrees option is given.
 * The check is bypassed when the branch is already the current one,
 * as it will not make things any worse.
 */
स्थिर शून्य die_if_switching_to_a_branch_in_use(संरचना checkout_opts *opts,
						ध्रुव वर्ण *full_ref)
{
	पूर्णाङ्क flags;
	वर्ण *head_ref;

	यदि (opts->ignore_other_worktrees)
		प्रत्यागम;
	head_ref = refs_resolve_refdup(get_main_ref_store(the_repository),
				       "HEAD", 0, रिक्त, &flags);
	यदि (head_ref && (!(flags & REF_ISSYMREF) || म_भेद(head_ref, full_ref)))
		die_if_checked_out(full_ref, 1);
	मुक्त(head_ref);
}

स्थिर पूर्णाङ्क checkout_branch(संरचना checkout_opts *opts,
			   संरचना branch_info *new_branch_info)
{
	संरचना repo_config_मूल्यावली *cfg = repo_config_मूल्यावली(the_repository);
	पूर्णाङ्क noop_switch = (!new_branch_info->name &&
			   !opts->new_branch &&
			   !opts->force_detach);

	यदि (opts->pathspec.nr)
		die(_("paths cannot be used with switching branches"));

	यदि (opts->patch_mode)
		die(_("'%s' cannot be used with switching branches"),
		    "--patch");

	यदि (opts->overlay_mode != -1)
		die(_("'%s' cannot be used with switching branches"),
		    "--[no]-overlay");

	यदि (opts->writeout_stage) {
		ध्रुव वर्ण *संदेश;
		यदि (noop_switch)
			संदेश = _("'%s' needs the paths to check out");
		अन्यथा
			संदेश = _("'%s' cannot be used with switching branches");
		die(संदेश, "--ours/--theirs");
	}

	यदि (opts->force && opts->merge)
		die(_("'%s' cannot be used with '%s'"), "-f", "-m");

	यदि (opts->discard_changes && opts->merge)
		die(_("'%s' cannot be used with '%s'"), "--discard-changes", "--merge");

	यदि (opts->force_detach && opts->new_branch)
		die(_("'%s' cannot be used with '%s'"),
		    "--detach", "-b/-B/--orphan");

	यदि (opts->new_orphan_branch) {
		यदि (opts->track != BRANCH_TRACK_UNSPECIFIED)
			die(_("'%s' cannot be used with '%s'"), "--orphan", "-t");
		यदि (opts->orphan_from_empty_वृक्ष && new_branch_info->name)
			die(_("'%s' cannot take <start-point>"), "--orphan");
	} अन्यथा यदि (opts->force_detach) {
		यदि (opts->track != BRANCH_TRACK_UNSPECIFIED)
			die(_("'%s' cannot be used with '%s'"), "--detach", "-t");
	} अन्यथा यदि (opts->track == BRANCH_TRACK_UNSPECIFIED)
		opts->track = cfg->branch_track;

	यदि (new_branch_info->name && !new_branch_info->commit)
		die(_("Cannot switch branch to a non-commit '%s'"),
		    new_branch_info->name);

	यदि (noop_switch &&
	    !opts->switch_branch_doing_nothing_is_ok)
		die(_("missing branch or commit argument"));

	यदि (!opts->implicit_detach &&
	    !opts->force_detach &&
	    !opts->new_branch &&
	    !opts->new_branch_force &&
	    new_branch_info->name &&
	    !new_branch_info->path)
		die_expecting_a_branch(new_branch_info);

	यदि (!opts->can_switch_when_in_progress)
		die_if_some_operation_in_progress();

	/* "git checkout <branch>" */
	यदि (new_branch_info->path && !opts->force_detach && !opts->new_branch)
		die_if_switching_to_a_branch_in_use(opts, new_branch_info->path);

	/* "git checkout -B <branch>" */
	यदि (opts->new_branch_force) {
		वर्ण *full_ref = xstrfmt("refs/heads/%s", opts->new_branch);
		die_if_switching_to_a_branch_in_use(opts, full_ref);
		मुक्त(full_ref);
	}

	यदि (!new_branch_info->commit && opts->new_branch) {
		संरचना object_id rev;
		पूर्णाङ्क flag;

		यदि (!refs_read_ref_full(get_main_ref_store(the_repository), "HEAD", 0, &rev, &flag) &&
		    (flag & REF_ISSYMREF) && is_null_oid(&rev))
			प्रत्यागम switch_unborn_to_new_branch(opts);
	}
	प्रत्यागम switch_branches(opts, new_branch_info);
}

स्थिर पूर्णाङ्क parse_opt_conflict(ध्रुव संरचना option *o, ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना checkout_opts *opts = o->मूल्य;

	यदि (unset) {
		opts->conflict_style = -1;
		प्रत्यागम 0;
	}
	opts->conflict_style = parse_conflict_style_name(arg);
	यदि (opts->conflict_style < 0)
		प्रत्यागम error(_("unknown conflict style '%s'"), arg);
	/* --conflict overrides a previous --no-merge */
	यदि (!opts->merge)
		opts->merge = -1;

	प्रत्यागम 0;
}

स्थिर संरचना option *add_common_options(संरचना checkout_opts *opts,
					 संरचना option *prevopts)
{
	संरचना option options[] = {
		OPT__QUIET(&opts->quiet, N_("suppress progress reporting")),
		OPT_CALLBACK_F(0, "recurse-submodules", रिक्त,
			    "checkout", "control recursive updating of submodules",
			    PARSE_OPT_OPTARG, option_parse_recurse_submodules_worktree_updater),
		OPT_BOOL(0, "progress", &opts->show_progress, N_("force progress reporting")),
		OPT_BOOL('m', "merge", &opts->merge, N_("perform a 3-way merge with the new branch")),
		OPT_CALLBACK(0, "conflict", opts, N_("style"),
			     N_("conflict style (merge, diff3, or zdiff3)"),
			     parse_opt_conflict),
		OPT_END()
	};
	संरचना option *newopts = parse_options_concat(prevopts, options);
	मुक्त(prevopts);
	प्रत्यागम newopts;
}

स्थिर संरचना option *add_common_switch_branch_options(
	संरचना checkout_opts *opts, संरचना option *prevopts)
{
	संरचना option options[] = {
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
	संरचना option *newopts = parse_options_concat(prevopts, options);
	मुक्त(prevopts);
	प्रत्यागम newopts;
}

स्थिर संरचना option *add_checkout_path_options(संरचना checkout_opts *opts,
						संरचना option *prevopts)
{
	संरचना option options[] = {
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
	संरचना option *newopts = parse_options_concat(prevopts, options);
	मुक्त(prevopts);
	प्रत्यागम newopts;
}

/* create-branch option (either b or c) */
स्थिर वर्ण cb_option = 'b';

स्थिर पूर्णाङ्क checkout_main(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix,
			 संरचना checkout_opts *opts, संरचना option *options,
			 गणना checkout_command which_command)
{
	पूर्णाङ्क parseopt_flags = 0;
	संरचना branch_info new_branch_info = { 0 };
	पूर्णाङ्क ret;

	स्थिर ध्रुव वर्ण * ध्रुव checkout_usage[] = {
		N_("git checkout [<options>] <branch>"),
		N_("git checkout [<options>] [<branch>] -- <file>..."),
		रिक्त,
	};

	स्थिर ध्रुव वर्ण * ध्रुव switch_branch_usage[] = {
		N_("git switch [<options>] [<branch>]"),
		रिक्त,
	};

	स्थिर ध्रुव वर्ण * ध्रुव restore_usage[] = {
		N_("git restore [<options>] [--source=<branch>] <file>..."),
		रिक्त,
	};

	ध्रुव वर्ण * ध्रुव *usagestr;

	विकल्प (which_command) {
	पक्ष CHECKOUT_CHECKOUT:
		usagestr = checkout_usage;
		भङ्ग;
	पक्ष CHECKOUT_SWITCH:
		usagestr = switch_branch_usage;
		भङ्ग;
	पक्ष CHECKOUT_RESTORE:
		usagestr = restore_usage;
		भङ्ग;
	स्वभाव:
		BUG("no such checkout variant %d", which_command);
	}

	opts->overwrite_ignore = 1;
	opts->prefix = prefix;
	opts->show_progress = -1;

	repo_config(the_repository, git_checkout_config, opts);
	यदि (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_full_index = 0;
	}

	opts->track = BRANCH_TRACK_UNSPECIFIED;

	यदि (!opts->accept_pathspec && !opts->accept_ref)
		BUG("make up your mind, you need to take _something_");
	यदि (opts->accept_pathspec && opts->accept_ref)
		parseopt_flags = PARSE_OPT_KEEP_DASHDASH;

	argc = parse_options(argc, argv, prefix, options,
			     usagestr, parseopt_flags);

	यदि (opts->patch_context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	यदि (opts->patch_interhunk_context < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	यदि (!opts->patch_mode) {
		यदि (opts->patch_context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--patch");
		यदि (opts->patch_interhunk_context != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--patch");
		यदि (!opts->auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--patch");
	}

	यदि (opts->show_progress < 0) {
		यदि (opts->quiet)
			opts->show_progress = 0;
		अन्यथा
			opts->show_progress = isatty(2);
	}

	/* --conflicts implies --merge */
	यदि (opts->merge == -1)
		opts->merge = opts->conflict_style >= 0;

	यदि (opts->force) {
		opts->discard_changes = 1;
		opts->ignore_unmerged_opt = "--force";
		opts->ignore_unmerged = 1;
	}

	यदि ((!!opts->new_branch + !!opts->new_branch_force + !!opts->new_orphan_branch) > 1)
		die(_("options '-%c', '-%c', and '%s' cannot be used together"),
			cb_option, बड़े(cb_option), "--orphan");

	यदि (opts->overlay_mode == 1 && opts->patch_mode)
		die(_("options '%s' and '%s' cannot be used together"), "-p", "--overlay");

	यदि (opts->checkout_index >= 0 || opts->checkout_worktree >= 0) {
		यदि (opts->checkout_index < 0)
			opts->checkout_index = 0;
		यदि (opts->checkout_worktree < 0)
			opts->checkout_worktree = 0;
	} अन्यथा {
		यदि (opts->checkout_index < 0)
			opts->checkout_index = -opts->checkout_index - 1;
		यदि (opts->checkout_worktree < 0)
			opts->checkout_worktree = -opts->checkout_worktree - 1;
	}
	यदि (opts->checkout_index < 0 || opts->checkout_worktree < 0)
		BUG("these flags should be non-negative by now");
	/*
	 * convenient shortcut: "git restore --staged [--worktree]" equals
	 * "git restore --staged [--worktree] --source HEAD"
	 */
	यदि (!opts->from_treeish && opts->checkout_index)
		opts->from_treeish = "HEAD";

	/*
	 * From here on, new_branch will contain the branch to be checked out,
	 * and new_branch_force and new_orphan_branch will tell us which one of
	 * -b/-B/-c/-C/--orphan is being used.
	 */
	यदि (opts->new_branch_force)
		opts->new_branch = opts->new_branch_force;

	यदि (opts->new_orphan_branch)
		opts->new_branch = opts->new_orphan_branch;

	/* --track without -c/-C/-b/-B/--orphan should DWIM */
	यदि (opts->track != BRANCH_TRACK_UNSPECIFIED && !opts->new_branch) {
		ध्रुव वर्ण *argv0 = argv[0];
		यदि (!argc || !म_भेद(argv0, "--"))
			die(_("--track needs a branch name"));
		skip_prefix(argv0, "refs/", &argv0);
		skip_prefix(argv0, "remotes/", &argv0);
		argv0 = म_अक्षर(argv0, '/');
		यदि (!argv0 || !argv0[1])
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
	यदि (argc && opts->accept_ref) {
		संरचना object_id rev;
		पूर्णाङ्क dwim_ok =
			!opts->patch_mode &&
			opts->dwim_new_local_branch &&
			opts->track == BRANCH_TRACK_UNSPECIFIED &&
			!opts->new_branch;
		पूर्णाङ्क n = parse_branchname_arg(argc, argv, dwim_ok, which_command,
					     &new_branch_info, opts, &rev);
		argv += n;
		argc -= n;
	} अन्यथा यदि (!opts->accept_ref && opts->from_treeish) {
		संरचना object_id rev;

		यदि (repo_get_oid_mb(the_repository, opts->from_treeish, &rev))
			die(_("could not resolve '%s'"), opts->from_treeish);

		setup_new_branch_info_and_source_वृक्ष(&new_branch_info,
						      opts, &rev,
						      opts->from_treeish);

		यदि (!opts->source_वृक्ष)
			die(_("reference is not a tree: %s"), opts->from_treeish);
	}

	यदि (argc) {
		parse_pathspec(&opts->pathspec, 0,
			       opts->patch_mode ? PATHSPEC_PREFIX_ORIGIN : 0,
			       prefix, argv);

		यदि (!opts->pathspec.nr)
			die(_("invalid path specification"));

		/*
		 * Try to give more helpful suggestion.
		 * new_branch && argc > 1 will be caught later.
		 */
		यदि (opts->new_branch && argc == 1 && !new_branch_info.commit)
			die(_("'%s' is not a commit and a branch '%s' cannot be created from it"),
				argv[0], opts->new_branch);

		यदि (opts->force_detach)
			die(_("git checkout: --detach does not take a path argument '%s'"),
			    argv[0]);
	}

	यदि (opts->pathspec_from_file) {
		यदि (opts->pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		यदि (opts->force_detach)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--detach");

		यदि (opts->patch_mode)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--patch");

		parse_pathspec_file(&opts->pathspec, 0,
				    0,
				    prefix, opts->pathspec_from_file, opts->pathspec_file_nul);
	} अन्यथा यदि (opts->pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	opts->pathspec.recursive = 1;

	यदि (opts->pathspec.nr) {
		यदि (1 < !!opts->writeout_stage + !!opts->force + !!opts->merge)
			die(_("git checkout: --ours/--theirs, --force and --merge are incompatible when\n"
			      "checking out of the index."));
	} अन्यथा {
		यदि (opts->accept_pathspec && !opts->empty_pathspec_ok &&
		    !opts->patch_mode)	/* patch mode is special */
			die(_("you must specify path(s) to restore"));
	}

	यदि (opts->new_branch) {
		संरचना strbuf buf = STRBUF_INIT;

		यदि (opts->new_branch_force)
			opts->branch_exists = validate_branchname(opts->new_branch, &buf);
		अन्यथा
			opts->branch_exists =
				validate_new_branchname(opts->new_branch, &buf, 0);
		strbuf_release(&buf);
	}

	यदि (opts->patch_mode || opts->pathspec.nr)
		ret = checkout_paths(opts, &new_branch_info);
	अन्यथा
		ret = checkout_branch(opts, &new_branch_info);

	branch_info_release(&new_branch_info);
	clear_pathspec(&opts->pathspec);
	मुक्त(opts->pathspec_from_file);
	मुक्त(options);

	प्रत्यागम ret;
}

पूर्णाङ्क cmd_checkout(पूर्णाङ्क argc,
		 ध्रुव वर्ण **argv,
		 ध्रुव वर्ण *prefix,
		 संरचना repository *repo UNUSED)
{
	संरचना checkout_opts opts = CHECKOUT_OPTS_INIT;
	संरचना option *options;
	संरचना option checkout_options[] = {
		OPT_STRING('b', रिक्त, &opts.new_branch, N_("branch"),
			   N_("create and checkout a new branch")),
		OPT_STRING('B', रिक्त, &opts.new_branch_force, N_("branch"),
			   N_("create/reset and checkout a branch")),
		OPT_BOOL('l', रिक्त, &opts.new_branch_log, N_("create reflog for new branch")),
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
	opts.orphan_from_empty_वृक्ष = 0;
	opts.empty_pathspec_ok = 1;
	opts.overlay_mode = -1;
	opts.checkout_index = -2;    /* default on */
	opts.checkout_worktree = -2; /* default on */

	यदि (argc == 3 && !म_भेद(argv[1], "-b")) {
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

	प्रत्यागम checkout_main(argc, argv, prefix, &opts, options,
			     CHECKOUT_CHECKOUT);
}

पूर्णाङ्क cmd_switch(पूर्णाङ्क argc,
	       ध्रुव वर्ण **argv,
	       ध्रुव वर्ण *prefix,
	       संरचना repository *repo UNUSED)
{
	संरचना checkout_opts opts = CHECKOUT_OPTS_INIT;
	संरचना option *options = रिक्त;
	संरचना option switch_options[] = {
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
	opts.orphan_from_empty_वृक्ष = 1;
	opts.overlay_mode = -1;

	options = parse_options_dup(switch_options);
	options = add_common_options(&opts, options);
	options = add_common_switch_branch_options(&opts, options);

	cb_option = 'c';

	प्रत्यागम checkout_main(argc, argv, prefix, &opts, options,
			     CHECKOUT_SWITCH);
}

पूर्णाङ्क cmd_restore(पूर्णाङ्क argc,
		ध्रुव वर्ण **argv,
		ध्रुव वर्ण *prefix,
		संरचना repository *repo UNUSED)
{
	संरचना checkout_opts opts = CHECKOUT_OPTS_INIT;
	संरचना option *options;
	संरचना option restore_options[] = {
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

	प्रत्यागम checkout_main(argc, argv, prefix, &opts, options,
			     CHECKOUT_RESTORE);
}
