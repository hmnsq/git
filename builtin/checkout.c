#నిర్వచించు USE_THE_REPOSITORY_VARIABLE
#నిర్వచించు DISABLE_SIGN_COMPARE_WARNINGS

#అన్తర్భావ "builtin.h"
#అన్తర్భావ "advice.h"
#అన్తర్భావ "branch.h"
#అన్తర్భావ "cache-tree.h"
#అన్తర్భావ "checkout.h"
#అన్తర్భావ "commit.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "diff.h"
#అన్తర్భావ "dir.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "hex.h"
#అన్తర్భావ "hook.h"
#అన్తర్భావ "merge-ll.h"
#అన్తర్భావ "lockfile.h"
#అన్తర్భావ "mem-pool.h"
#అన్తర్భావ "object-file.h"
#అన్తర్భావ "object-name.h"
#అన్తర్భావ "odb.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "path.h"
#అన్తర్భావ "preload-index.h"
#అన్తర్భావ "read-cache.h"
#అన్తర్భావ "refs.h"
#అన్తర్భావ "remote.h"
#అన్తర్భావ "repo-settings.h"
#అన్తర్భావ "resolve-undo.h"
#అన్తర్భావ "revision.h"
#అన్తర్భావ "sequencer.h"
#అన్తర్భావ "setup.h"
#అన్తర్భావ "sparse-index.h"
#అన్తర్భావ "strvec.h"
#అన్తర్భావ "submodule.h"
#అన్తర్భావ "symlinks.h"
#అన్తర్భావ "trace2.h"
#అన్తర్భావ "tree.h"
#అన్తర్భావ "tree-walk.h"
#అన్తర్భావ "unpack-trees.h"
#అన్తర్భావ "wt-status.h"
#అన్తర్భావ "xdiff-interface.h"
#అన్తర్భావ "entry.h"
#అన్తర్భావ "parallel-checkout.h"
#అన్తర్భావ "add-interactive.h"

నిర్మాణం checkout_opts {
	పూర్ణాంకం patch_బహుళకం;
	పూర్ణాంకం patch_context;
	పూర్ణాంకం patch_interhunk_context;
	పూర్ణాంకం auto_advance;
	పూర్ణాంకం quiet;
	పూర్ణాంకం విలీనం;
	పూర్ణాంకం force;
	పూర్ణాంకం force_detach;
	పూర్ణాంకం implicit_detach;
	పూర్ణాంకం writeout_stage;
	పూర్ణాంకం overwrite_ignore;
	పూర్ణాంకం ignore_skipworktree;
	పూర్ణాంకం ignore_other_worktrees;
	పూర్ణాంకం show_పురోగతి;
	పూర్ణాంకం లెక్క_checkout_paths;
	పూర్ణాంకం overlay_బహుళకం;
	పూర్ణాంకం dwim_new_local_branch;
	పూర్ణాంకం discard_changes;
	పూర్ణాంకం accept_ref;
	పూర్ణాంకం accept_pathspec;
	పూర్ణాంకం switch_branch_doing_nothing_is_ok;
	పూర్ణాంకం only_విలీనం_on_switching_branches;
	పూర్ణాంకం can_switch_when_లోపల_పురోగతి;
	పూర్ణాంకం orphan_from_ఖాళీలు_వృక్షం;
	పూర్ణాంకం ఖాళీలు_pathspec_ok;
	పూర్ణాంకం checkout_సూచీ;
	పూర్ణాంకం checkout_worktree;
	స్థిరరాశి అక్షరం *ignore_unmerged_opt;
	పూర్ణాంకం ignore_unmerged;
	పూర్ణాంకం pathspec_file_nul;
	అక్షరం *pathspec_from_file;

	స్థిరరాశి అక్షరం *new_branch;
	స్థిరరాశి అక్షరం *new_branch_force;
	స్థిరరాశి అక్షరం *new_orphan_branch;
	పూర్ణాంకం new_branch_log;
	గణన branch_track track;
	నిర్మాణం diff_options diff_options;
	పూర్ణాంకం conflict_style;

	పూర్ణాంకం branch_exists;
	స్థిరరాశి అక్షరం *ఉపసర్గ;
	నిర్మాణం pathspec pathspec;
	స్థిరరాశి అక్షరం *from_treeish;
	నిర్మాణం వృక్షం *ఆధారం_వృక్షం;
};

#నిర్వచించు CHECKOUT_OPTS_INIT { \
	.conflict_style = -1, \
	.విలీనం = -1, \
	.patch_context = -1, \
	.patch_interhunk_context = -1, \
	.auto_advance = 1, \
}

#నిర్వచించు విలీనం_WORKING_వృక్షం_UNPACK_FAILED (-2)

నిర్మాణం branch_info {
	అక్షరం *name; /* The short name used */
	అక్షరం *మార్గం; /* The full name of a real branch */
	నిర్మాణం commit *commit; /* The named commit */
	అక్షరం *refname; /* The full name of the ref being checked out. */
	నిర్మాణం object_id oid; /* The object ID of the commit being checked out. */
	/*
	 * if not null the branch is detached because it's already
	 * checked out in this checkout
	 */
	అక్షరం *checkout;
};

స్థిర శూన్య branch_info_release(నిర్మాణం branch_info *info)
{
	విడుదల(info->name);
	విడుదల(info->మార్గం);
	విడుదల(info->refname);
	విడుదల(info->checkout);
}

స్థిర పూర్ణాంకం post_checkout_hook(నిర్మాణం commit *old_commit, నిర్మాణం commit *new_commit,
			      పూర్ణాంకం మారింది)
{
	నిర్మాణం నిరంతరం_hooks_opt opt = నిరంతరం_HOOKS_OPT_INIT_FORCE_SERIAL;

	/*
	 * "new_commit" can be NULL when checking out from the index before
	 * a commit exists.
	 */
	strvec_pushl(&opt.వాదనలు,
		     oid_గమ్యం_hex(old_commit ? &old_commit->object.oid : null_oid(the_hash_algo)),
		     oid_గమ్యం_hex(new_commit ? &new_commit->object.oid : null_oid(the_hash_algo)),
		     మారింది ? "1" : "0",
		     ఖాళీ);

	తిరిగి నిరంతరం_hooks_opt(the_repository, "post-checkout", &opt);
}

/*
 * Handle a tree object and determine if we need to recurse into the
 * tree (READ_TREE_RECURSIVE) or skip it (0).
 */
స్థిర పూర్ణాంకం try_update_sparse_directory(స్థిరరాశి నిర్మాణం object_id *oid,
				       నిర్మాణం strbuf *పునాది,
				       స్థిరరాశి అక్షరం *pathname,
				       పూర్ణాంకం overlay_బహుళకం)
{
	నిర్మాణం strbuf dirpath = STRBUF_INIT;
	నిర్మాణం cache_నమోదు *old;
	పూర్ణాంకం pos, ఫలితం = READ_వృక్షం_RECURSIVE;

	ఒకవేళ (!the_repository->సూచీ->sparse_సూచీ)
		తిరిగి ఫలితం;

	strbuf_addbuf(&dirpath, పునాది);
	strbuf_addstr(&dirpath, pathname);
	strbuf_addch(&dirpath, '/');

	pos = సూచీ_name_pos_sparse(the_repository->సూచీ,
				    dirpath.buf, dirpath.len);
	ఒకవేళ (pos < 0)
		వెళ్ళు cleanup;

	old = the_repository->సూచీ->cache[pos];
	ఒకవేళ (!S_ISSPARSEDIR(old->ce_బహుళకం))
		వెళ్ళు cleanup;

	ఒకవేళ (oideq(oid, &old->oid)) {
		/* Tree content already matches; no need to descend. */
		ఫలితం = 0;
	} లేకపోతే ఒకవేళ (!overlay_బహుళకం) {
		/*
		 * In non-overlay mode (e.g., restore --staged), replace the
		 * sparse directory OID directly since files not present in
		 * the source tree should be removed anyway.
		 */
		oidcpy(&old->oid, oid);
		old->ce_flags |= CE_UPDATE;
		ఫలితం = 0;
	}

cleanup:
	strbuf_release(&dirpath);
	తిరిగి ఫలితం;
}

స్థిర పూర్ణాంకం update_some(స్థిరరాశి నిర్మాణం object_id *oid, నిర్మాణం strbuf *పునాది,
		       స్థిరరాశి అక్షరం *pathname, గుర్తులేని బహుళకం, శూన్య *context)
{
	పూర్ణాంకం len;
	నిర్మాణం cache_నమోదు *ce;
	పూర్ణాంకం pos;
	పూర్ణాంకం overlay_బహుళకం = context ? *((పూర్ణాంకం *)context) : 1;

	ఒకవేళ (S_ISDIR(బహుళకం))
		తిరిగి try_update_sparse_directory(oid, పునాది, pathname,
						   overlay_బహుళకం);

	len = పునాది->len + అక్షరశ్రేణి_పొడవు(pathname);
	ce = make_ఖాళీలు_cache_నమోదు(the_repository->సూచీ, len);
	oidcpy(&ce->oid, oid);
	జ్ఞాపకం_నకలు(ce->name, పునాది->buf, పునాది->len);
	జ్ఞాపకం_నకలు(ce->name + పునాది->len, pathname, len - పునాది->len);
	ce->ce_flags = create_ce_flags(0) | CE_UPDATE;
	ce->ce_namelen = len;
	ce->ce_బహుళకం = create_ce_బహుళకం(బహుళకం);

	/*
	 * If the entry is the same as the current index, we can leave the old
	 * entry in place. Whether it is UPTODATE or not, checkout_entry will
	 * do the right thing.
	 */
	pos = సూచీ_name_pos_sparse(the_repository->సూచీ, ce->name, ce->ce_namelen);
	ఒకవేళ (pos >= 0) {
		నిర్మాణం cache_నమోదు *old = the_repository->సూచీ->cache[pos];
		ఒకవేళ (ce->ce_బహుళకం == old->ce_బహుళకం &&
		    !ce_intent_గమ్యం_కూడు(old) &&
		    oideq(&ce->oid, &old->oid)) {
			old->ce_flags |= CE_UPDATE;
			discard_cache_నమోదు(ce);
			తిరిగి 0;
		}
	}

	కూడు_సూచీ_నమోదు(the_repository->సూచీ, ce,
			కూడు_CACHE_OK_గమ్యం_కూడు | కూడు_CACHE_OK_గమ్యం_REPLACE);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం read_వృక్షం_some(నిర్మాణం వృక్షం *వృక్షం, స్థిరరాశి నిర్మాణం pathspec *pathspec,
			  పూర్ణాంకం overlay_బహుళకం)
{
	read_వృక్షం(the_repository, వృక్షం,
		  pathspec, update_some, &overlay_బహుళకం);

	/* update the index with the given tree's info
	 * for all args, expanding wildcards, and exit
	 * with any non-zero return code.
	 */
	తిరిగి 0;
}

స్థిర పూర్ణాంకం skip_same_name(స్థిరరాశి నిర్మాణం cache_నమోదు *ce, పూర్ణాంకం pos)
{
	అయితే (++pos < the_repository->సూచీ->cache_nr &&
	       !strcmp(the_repository->సూచీ->cache[pos]->name, ce->name))
		; /* skip */
	తిరిగి pos;
}

స్థిర పూర్ణాంకం check_stage(పూర్ణాంకం stage, స్థిరరాశి నిర్మాణం cache_నమోదు *ce, పూర్ణాంకం pos,
		       పూర్ణాంకం overlay_బహుళకం)
{
	అయితే (pos < the_repository->సూచీ->cache_nr &&
	       !strcmp(the_repository->సూచీ->cache[pos]->name, ce->name)) {
		ఒకవేళ (ce_stage(the_repository->సూచీ->cache[pos]) == stage)
			తిరిగి 0;
		pos++;
	}
	ఒకవేళ (!overlay_బహుళకం)
		తిరిగి 0;
	ఒకవేళ (stage == 2)
		తిరిగి error(_("path '%s' does not have our version"), ce->name);
	లేకపోతే
		తిరిగి error(_("path '%s' does not have their version"), ce->name);
}

స్థిర పూర్ణాంకం check_stages(గుర్తులేని stages, స్థిరరాశి నిర్మాణం cache_నమోదు *ce, పూర్ణాంకం pos)
{
	గుర్తులేని చూసిన = 0;
	స్థిరరాశి అక్షరం *name = ce->name;

	అయితే (pos < the_repository->సూచీ->cache_nr) {
		ce = the_repository->సూచీ->cache[pos];
		ఒకవేళ (strcmp(name, ce->name))
			ఆపు;
		చూసిన |= (1 << ce_stage(ce));
		pos++;
	}
	ఒకవేళ ((stages & చూసిన) != stages)
		తిరిగి error(_("path '%s' does not have all necessary versions"),
			     name);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం checkout_stage(పూర్ణాంకం stage, స్థిరరాశి నిర్మాణం cache_నమోదు *ce, పూర్ణాంకం pos,
			  స్థిరరాశి నిర్మాణం checkout *state, పూర్ణాంకం *nr_checkouts,
			  పూర్ణాంకం overlay_బహుళకం)
{
	అయితే (pos < the_repository->సూచీ->cache_nr &&
	       !strcmp(the_repository->సూచీ->cache[pos]->name, ce->name)) {
		ఒకవేళ (ce_stage(the_repository->సూచీ->cache[pos]) == stage)
			తిరిగి checkout_నమోదు(the_repository->సూచీ->cache[pos], state,
					      ఖాళీ, nr_checkouts);
		pos++;
	}
	ఒకవేళ (!overlay_బహుళకం) {
		unlink_నమోదు(ce, ఖాళీ);
		తిరిగి 0;
	}
	ఒకవేళ (stage == 2)
		తిరిగి error(_("path '%s' does not have our version"), ce->name);
	లేకపోతే
		తిరిగి error(_("path '%s' does not have their version"), ce->name);
}

స్థిర పూర్ణాంకం checkout_merged(పూర్ణాంకం pos, స్థిరరాశి నిర్మాణం checkout *state,
			   పూర్ణాంకం *nr_checkouts, నిర్మాణం mem_pool *ce_mem_pool,
			   పూర్ణాంకం conflict_style)
{
	నిర్మాణం cache_నమోదు *ce = the_repository->సూచీ->cache[pos];
	స్థిరరాశి అక్షరం *మార్గం = ce->name;
	mmfile_t ancestor, ours, theirs;
	గణన ll_విలీనం_ఫలితం విలీనం_స్థితి;
	పూర్ణాంకం స్థితి_2;
	నిర్మాణం object_id oid;
	mmfile_t ఫలితం_buf;
	నిర్మాణం object_id threeway[3];
	గుర్తులేని బహుళకం = 0;
	నిర్మాణం ll_విలీనం_options ll_opts = LL_విలీనం_OPTIONS_INIT;
	పూర్ణాంకం renormalize = 0;

	memset(threeway, 0, పరిమాణం(threeway));
	అయితే (pos < the_repository->సూచీ->cache_nr) {
		పూర్ణాంకం stage;
		stage = ce_stage(ce);
		ఒకవేళ (!stage || strcmp(మార్గం, ce->name))
			ఆపు;
		oidcpy(&threeway[stage - 1], &ce->oid);
		ఒకవేళ (stage == 2)
			బహుళకం = create_ce_బహుళకం(ce->ce_బహుళకం);
		pos++;
		ce = the_repository->సూచీ->cache[pos];
	}
	ఒకవేళ (is_null_oid(&threeway[1]) || is_null_oid(&threeway[2]))
		తిరిగి error(_("path '%s' does not have necessary versions"), మార్గం);

	read_mmblob(&ancestor, the_repository->objects, &threeway[0]);
	read_mmblob(&ours, the_repository->objects, &threeway[1]);
	read_mmblob(&theirs, the_repository->objects, &threeway[2]);

	repo_config_get_bool(the_repository, "merge.renormalize", &renormalize);
	ll_opts.renormalize = renormalize;
	ll_opts.conflict_style = conflict_style;
	విలీనం_స్థితి = ll_విలీనం(&ఫలితం_buf, మార్గం, &ancestor, "base",
				&ours, "ours", &theirs, "theirs",
				state->istate, &ll_opts);
	విడుదల(ancestor.ptr);
	విడుదల(ours.ptr);
	విడుదల(theirs.ptr);
	ఒకవేళ (విలీనం_స్థితి == LL_విలీనం_ద్విమాన_CONFLICT)
		warning("Cannot merge binary files: %s (%s vs. %s)",
			మార్గం, "ours", "theirs");
	ఒకవేళ (విలీనం_స్థితి < 0 || !ఫలితం_buf.ptr) {
		విడుదల(ఫలితం_buf.ptr);
		తిరిగి error(_("path '%s': cannot merge"), మార్గం);
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
	ఒకవేళ (odb_write_object(the_repository->objects, ఫలితం_buf.ptr, ఫలితం_buf.కొలత, OBJ_BLOB, &oid))
		die(_("Unable to add merge result for '%s'"), మార్గం);
	విడుదల(ఫలితం_buf.ptr);
	ce = make_transient_cache_నమోదు(బహుళకం, &oid, మార్గం, 2, ce_mem_pool);
	ఒకవేళ (!ce)
		die(_("make_cache_entry failed for path '%s'"), మార్గం);
	స్థితి_2 = checkout_నమోదు(ce, state, ఖాళీ, nr_checkouts);
	తిరిగి స్థితి_2;
}

స్థిర శూన్య mark_ce_for_checkout_overlay(నిర్మాణం cache_నమోదు *ce,
					 అక్షరం *ps_matched,
					 స్థిరరాశి నిర్మాణం checkout_opts *opts)
{
	ce->ce_flags &= ~CE_MATCHED;
	ఒకవేళ (!opts->ignore_skipworktree && ce_skip_worktree(ce))
		తిరిగి;
	ఒకవేళ (opts->ఆధారం_వృక్షం && !(ce->ce_flags & CE_UPDATE))
		/*
		 * "git checkout tree-ish -- path", but this entry
		 * is in the original index but is not in tree-ish
		 * or does not match the pathspec; it will not be
		 * checked out to the working tree.  We will not do
		 * anything to this entry at all.
		 */
		తిరిగి;
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
	ఒకవేళ (ce_మార్గం_సరిపోలిక(the_repository->సూచీ, ce, &opts->pathspec, ps_matched))
		ce->ce_flags |= CE_MATCHED;
}

స్థిర శూన్య mark_ce_for_checkout_no_overlay(నిర్మాణం cache_నమోదు *ce,
					    అక్షరం *ps_matched,
					    స్థిరరాశి నిర్మాణం checkout_opts *opts)
{
	ce->ce_flags &= ~CE_MATCHED;
	ఒకవేళ (!opts->ignore_skipworktree && ce_skip_worktree(ce))
		తిరిగి;
	ఒకవేళ (ce_మార్గం_సరిపోలిక(the_repository->సూచీ, ce, &opts->pathspec, ps_matched)) {
		ce->ce_flags |= CE_MATCHED;
		ఒకవేళ (opts->ఆధారం_వృక్షం && !(ce->ce_flags & CE_UPDATE))
			/*
			 * In overlay mode, but the path is not in
			 * tree-ish, which means we should remove it
			 * from the index and the working tree.
			 */
			ce->ce_flags |= CE_REMOVE | CE_WT_REMOVE;
	}
}

స్థిర పూర్ణాంకం checkout_worktree(స్థిరరాశి నిర్మాణం checkout_opts *opts,
			     స్థిరరాశి నిర్మాణం branch_info *info)
{
	నిర్మాణం checkout state = CHECKOUT_INIT;
	పూర్ణాంకం nr_checkouts = 0, nr_unmerged = 0;
	పూర్ణాంకం errs = 0;
	పూర్ణాంకం pos;
	పూర్ణాంకం pc_workers, pc_పరిమితి;
	నిర్మాణం mem_pool ce_mem_pool;

	state.force = 1;
	state.refresh_cache = 1;
	state.istate = the_repository->సూచీ;

	mem_pool_init(&ce_mem_pool, 0);
	get_parallel_checkout_configs(&pc_workers, &pc_పరిమితి);
	init_checkout_metadata(&state.meta, info->refname,
			       info->commit ? &info->commit->object.oid : &info->oid,
			       ఖాళీ);

	enable_delayed_checkout(&state);

	ఒకవేళ (pc_workers > 1)
		init_parallel_checkout();

	కొరకు (pos = 0; pos < the_repository->సూచీ->cache_nr; pos++) {
		నిర్మాణం cache_నమోదు *ce = the_repository->సూచీ->cache[pos];
		ఒకవేళ (ce->ce_flags & CE_MATCHED) {
			ఒకవేళ (!ce_stage(ce)) {
				errs |= checkout_నమోదు(ce, &state,
						       ఖాళీ, &nr_checkouts);
				కొనసాగు;
			}
			ఒకవేళ (opts->writeout_stage)
				errs |= checkout_stage(opts->writeout_stage,
						       ce, pos,
						       &state,
						       &nr_checkouts, opts->overlay_బహుళకం);
			లేకపోతే ఒకవేళ (opts->విలీనం)
				errs |= checkout_merged(pos, &state,
							&nr_unmerged,
							&ce_mem_pool,
							opts->conflict_style);
			pos = skip_same_name(ce, pos) - 1;
		}
	}
	ఒకవేళ (pc_workers > 1)
		errs |= నిరంతరం_parallel_checkout(&state, pc_workers, pc_పరిమితి,
					      ఖాళీ, ఖాళీ);
	mem_pool_discard(&ce_mem_pool, should_validate_cache_entries());
	remove_marked_cache_entries(the_repository->సూచీ, 1);
	remove_scheduled_dirs();
	errs |= ముగింపు_delayed_checkout(&state, opts->show_పురోగతి);

	ఒకవేళ (opts->లెక్క_checkout_paths) {
		ఒకవేళ (nr_unmerged)
			fprintf_ln(stderr, Q_("Recreated %d merge conflict",
					      "Recreated %d merge conflicts",
					      nr_unmerged),
				   nr_unmerged);
		ఒకవేళ (opts->ఆధారం_వృక్షం)
			fprintf_ln(stderr, Q_("Updated %d path from %s",
					      "Updated %d paths from %s",
					      nr_checkouts),
				   nr_checkouts,
				   repo_కనుగొను_unique_abbrev(the_repository, &opts->ఆధారం_వృక్షం->object.oid,
							   DEFAULT_ABBREV));
		లేకపోతే ఒకవేళ (!nr_unmerged || nr_checkouts)
			fprintf_ln(stderr, Q_("Updated %d path from the index",
					      "Updated %d paths from the index",
					      nr_checkouts),
				   nr_checkouts);
	}

	తిరిగి errs;
}

స్థిర పూర్ణాంకం checkout_paths(స్థిరరాశి నిర్మాణం checkout_opts *opts,
			  స్థిరరాశి నిర్మాణం branch_info *new_branch_info)
{
	పూర్ణాంకం pos;
	స్థిర అక్షరం *ps_matched;
	నిర్మాణం object_id rev;
	నిర్మాణం commit *తల;
	పూర్ణాంకం errs = 0;
	నిర్మాణం తాళం_file తాళం_file = తాళం_INIT;
	పూర్ణాంకం checkout_సూచీ;

	trace2_cmd_బహుళకం(opts->patch_బహుళకం ? "patch" : "path");

	ఒకవేళ (opts->track != BRANCH_TRACK_UNSPECIFIED)
		die(_("'%s' cannot be used with updating paths"), "--track");

	ఒకవేళ (opts->new_branch_log)
		die(_("'%s' cannot be used with updating paths"), "-l");

	ఒకవేళ (opts->ignore_unmerged && opts->patch_బహుళకం)
		die(_("'%s' cannot be used with updating paths"),
		    opts->ignore_unmerged_opt);

	ఒకవేళ (opts->force_detach)
		die(_("'%s' cannot be used with updating paths"), "--detach");

	ఒకవేళ (opts->విలీనం && opts->patch_బహుళకం)
		die(_("options '%s' and '%s' cannot be used together"), "--merge", "--patch");

	ఒకవేళ (opts->ignore_unmerged && opts->విలీనం)
		die(_("options '%s' and '%s' cannot be used together"),
		    opts->ignore_unmerged_opt, "-m");

	ఒకవేళ (opts->new_branch)
		die(_("Cannot update paths and switch to branch '%s' at the same time."),
		    opts->new_branch);

	ఒకవేళ (!opts->checkout_worktree && !opts->checkout_సూచీ)
		die(_("neither '%s' or '%s' is specified"),
		    "--staged", "--worktree");

	ఒకవేళ (!opts->checkout_worktree && !opts->from_treeish)
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
	ఒకవేళ (!opts->accept_ref && opts->checkout_సూచీ) {
		ఒకవేళ (opts->writeout_stage)
			die(_("'%s' or '%s' cannot be used with %s"),
			    "--ours", "--theirs", "--staged");

		ఒకవేళ (opts->విలీనం)
			die(_("'%s' or '%s' cannot be used with %s"),
			    "--merge", "--conflict", "--staged");
	}

	/*
	 * recreating unmerged index entries and writing out data from
	 * unmerged index entries would make no sense when checking out
	 * of a tree-ish.
	 */
	ఒకవేళ ((opts->విలీనం || opts->writeout_stage) && opts->ఆధారం_వృక్షం)
		die(_("'%s', '%s', or '%s' cannot be used when checking out of a tree"),
		    "--merge", "--ours", "--theirs");

	ఒకవేళ (opts->patch_బహుళకం) {
		గణన కూడు_p_బహుళకం patch_బహుళకం;
		నిర్మాణం interactive_options interactive_opts = {
			.context = opts->patch_context,
			.interhunkcontext = opts->patch_interhunk_context,
			.auto_advance = opts->auto_advance
		};
		స్థిరరాశి అక్షరం *rev = new_branch_info->name;
		అక్షరం rev_oid[GIT_MAX_HEXSZ + 1];

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
		ఒకవేళ (rev && new_branch_info->commit && strcmp(rev, "HEAD"))
			rev = oid_గమ్యం_hex_r(rev_oid, &new_branch_info->commit->object.oid);

		ఒకవేళ (opts->checkout_సూచీ && opts->checkout_worktree)
			patch_బహుళకం = కూడు_P_CHECKOUT;
		లేకపోతే ఒకవేళ (opts->checkout_సూచీ && !opts->checkout_worktree)
			patch_బహుళకం = కూడు_P_RESET;
		లేకపోతే ఒకవేళ (!opts->checkout_సూచీ && opts->checkout_worktree)
			patch_బహుళకం = కూడు_P_WORKTREE;
		లేకపోతే
			BUG("either flag must have been set, worktree=%d, index=%d",
			    opts->checkout_worktree, opts->checkout_సూచీ);
		తిరిగి !!నిరంతరం_కూడు_p(the_repository, patch_బహుళకం, &interactive_opts,
				   rev, &opts->pathspec, 0);
	}

	repo_hold_locked_సూచీ(the_repository, &తాళం_file, తాళం_DIE_ON_ERROR);
	ఒకవేళ (repo_read_సూచీ_preload(the_repository, &opts->pathspec, 0) < 0)
		తిరిగి error(_("index file corrupt"));

	ఒకవేళ (opts->ఆధారం_వృక్షం)
		read_వృక్షం_some(opts->ఆధారం_వృక్షం, &opts->pathspec,
			       opts->overlay_బహుళకం);
	ఒకవేళ (opts->విలీనం)
		unmerge_సూచీ(the_repository->సూచీ, &opts->pathspec, CE_MATCHED);

	ps_matched = xcalloc(opts->pathspec.nr, 1);

	/*
	 * Make sure all pathspecs participated in locating the paths
	 * to be checked out.
	 */
	కొరకు (pos = 0; pos < the_repository->సూచీ->cache_nr; pos++)
		ఒకవేళ (opts->overlay_బహుళకం)
			mark_ce_for_checkout_overlay(the_repository->సూచీ->cache[pos],
						     ps_matched,
						     opts);
		లేకపోతే
			mark_ce_for_checkout_no_overlay(the_repository->సూచీ->cache[pos],
							ps_matched,
							opts);

	ఒకవేళ (report_మార్గం_error(ps_matched, &opts->pathspec)) {
		విడుదల(ps_matched);
		తిరిగి 1;
	}
	విడుదల(ps_matched);

	/* Any unmerged paths? */
	కొరకు (pos = 0; pos < the_repository->సూచీ->cache_nr; pos++) {
		స్థిరరాశి నిర్మాణం cache_నమోదు *ce = the_repository->సూచీ->cache[pos];
		ఒకవేళ (ce->ce_flags & CE_MATCHED) {
			ఒకవేళ (!ce_stage(ce))
				కొనసాగు;
			ఒకవేళ (opts->ignore_unmerged) {
				ఒకవేళ (!opts->quiet)
					warning(_("path '%s' is unmerged"), ce->name);
			} లేకపోతే ఒకవేళ (opts->writeout_stage) {
				errs |= check_stage(opts->writeout_stage, ce, pos, opts->overlay_బహుళకం);
			} లేకపోతే ఒకవేళ (opts->విలీనం) {
				errs |= check_stages((1<<2) | (1<<3), ce, pos);
			} లేకపోతే {
				errs = 1;
				error(_("path '%s' is unmerged"), ce->name);
			}
			pos = skip_same_name(ce, pos) - 1;
		}
	}
	ఒకవేళ (errs)
		తిరిగి 1;

	/* Now we are committed to check them out */
	ఒకవేళ (opts->checkout_worktree)
		errs |= checkout_worktree(opts, new_branch_info);
	లేకపోతే
		remove_marked_cache_entries(the_repository->సూచీ, 1);

	/*
	 * Allow updating the index when checking out from the index.
	 * This is to save new stat info.
	 */
	ఒకవేళ (opts->checkout_worktree && !opts->checkout_సూచీ && !opts->ఆధారం_వృక్షం)
		checkout_సూచీ = 1;
	లేకపోతే
		checkout_సూచీ = opts->checkout_సూచీ;

	ఒకవేళ (checkout_సూచీ) {
		ఒకవేళ (write_locked_సూచీ(the_repository->సూచీ, &తాళం_file, COMMIT_తాళం))
			die(_("unable to write new index file"));
	} లేకపోతే {
		/*
		 * NEEDSWORK: if --worktree is not specified, we
		 * should save stat info of checked out files in the
		 * index to avoid the next (potentially costly)
		 * refresh. But it's a bit tricker to do...
		 */
		rollback_తాళం_file(&తాళం_file);
	}

	refs_read_ref_నిండిన(get_main_ref_store(the_repository), "HEAD", 0,
			   &rev, ఖాళీ);
	తల = lookup_commit_reference_gently(the_repository, &rev, 1);

	errs |= post_checkout_hook(తల, తల, 0);
	తిరిగి errs;
}

స్థిర శూన్య show_local_changes(నిర్మాణం object *తల,
			       స్థిరరాశి నిర్మాణం diff_options *opts)
{
	నిర్మాణం rev_info rev;
	/* I think we want full paths, even if we're in a subdirectory. */
	repo_init_revisions(the_repository, &rev, ఖాళీ);
	rev.diffopt.flags = opts->flags;
	rev.diffopt.output_format |= DIFF_FORMAT_NAME_స్థితి;
	rev.diffopt.flags.recursive = 1;
	diff_అమరిక_పూర్తి(&rev.diffopt);
	కూడు_pending_object(&rev, తల, ఖాళీ);
	నిరంతరం_diff_సూచీ(&rev, 0);
	release_revisions(&rev);
}

స్థిర శూన్య describe_detached_తల(స్థిరరాశి అక్షరం *సందేశం, నిర్మాణం commit *commit)
{
	నిర్మాణం strbuf sb = STRBUF_INIT;

	ఒకవేళ (!repo_parse_commit(the_repository, commit))
		pp_commit_easy(CMIT_FMT_ONELINE, commit, &sb);
	ఒకవేళ (print_sha1_ellipsis()) {
		fprintf(stderr, "%s %s... %s\n", సందేశం,
			repo_కనుగొను_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV),
			sb.buf);
	} లేకపోతే {
		fprintf(stderr, "%s %s %s\n", సందేశం,
			repo_కనుగొను_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV),
			sb.buf);
	}
	strbuf_release(&sb);
}

స్థిర పూర్ణాంకం reset_వృక్షం(నిర్మాణం వృక్షం *వృక్షం, స్థిరరాశి నిర్మాణం checkout_opts *o,
		      పూర్ణాంకం worktree, పూర్ణాంకం *writeout_error,
		      నిర్మాణం branch_info *info)
{
	నిర్మాణం unpack_trees_options opts;
	నిర్మాణం వృక్షం_desc వృక్షం_desc;

	memset(&opts, 0, పరిమాణం(opts));
	opts.తల_idx = -1;
	opts.update = worktree;
	opts.skip_unmerged = !worktree;
	opts.reset = o->force ? UNPACK_RESET_OVERWRITE_UNTRACKED :
				UNPACK_RESET_PROTECT_UNTRACKED;
	opts.preserve_ignored = (!o->force && !o->overwrite_ignore);
	opts.విలీనం = 1;
	opts.fn = oneway_విలీనం;
	opts.verbose_update = o->show_పురోగతి;
	opts.src_సూచీ = the_repository->సూచీ;
	opts.dst_సూచీ = the_repository->సూచీ;
	init_checkout_metadata(&opts.meta, info->refname,
			       info->commit ? &info->commit->object.oid : null_oid(the_hash_algo),
			       ఖాళీ);
	ఒకవేళ (repo_parse_వృక్షం(the_repository, వృక్షం) < 0)
		తిరిగి 128;
	init_వృక్షం_desc(&వృక్షం_desc, &వృక్షం->object.oid, వృక్షం->బఫరు, వృక్షం->కొలత);
	ఎంపిక (unpack_trees(1, &వృక్షం_desc, &opts)) {
	స్థితి -2:
		*writeout_error = 1;
		/*
		 * We return 0 nevertheless, as the index is all right
		 * and more importantly we have made best efforts to
		 * update paths in the work tree, and we cannot revert
		 * them.
		 */
		/* fallthrough */
	స్థితి 0:
		తిరిగి 0;
	అప్రమేయ:
		తిరిగి 128;
	}
}

స్థిర శూన్య అమరిక_branch_మార్గం(నిర్మాణం branch_info *branch)
{
	నిర్మాణం strbuf buf = STRBUF_INIT;

	/*
	 * If this is a ref, resolve it; otherwise, look up the OID for our
	 * expression.  Failure here is okay.
	 */
	ఒకవేళ (!repo_dwim_ref(the_repository, branch->name, అక్షరశ్రేణి_పొడవు(branch->name),
			   &branch->oid, &branch->refname, 0))
		repo_get_oid_committish(the_repository, branch->name, &branch->oid);

	copy_branchname(the_repository, &buf, branch->name, INTERPRET_BRANCH_LOCAL);
	ఒకవేళ (strcmp(buf.buf, branch->name)) {
		విడుదల(branch->name);
		branch->name = xstrdup(buf.buf);
	}
	strbuf_splice(&buf, 0, 0, "refs/heads/", 11);
	విడుదల(branch->మార్గం);
	branch->మార్గం = strbuf_detach(&buf, ఖాళీ);
}

స్థిర శూన్య init_topts(నిర్మాణం unpack_trees_options *topts,
		       పూర్ణాంకం show_పురోగతి, పూర్ణాంకం overwrite_ignore,
		       bool quiet)
{
	memset(topts, 0, పరిమాణం(*topts));
	topts->తల_idx = -1;
	topts->src_సూచీ = the_repository->సూచీ;
	topts->dst_సూచీ = the_repository->సూచీ;

	అమరిక_unpack_trees_porcelain(topts, "checkout");

	topts->initial_checkout = is_సూచీ_unborn(the_repository->సూచీ);
	topts->update = 1;
	topts->విలీనం = 1;
	topts->quiet = quiet;
	topts->verbose_update = show_పురోగతి;
	topts->fn = twoway_విలీనం;
	topts->preserve_ignored = !overwrite_ignore;
}

స్థిర పూర్ణాంకం విలీనం_working_వృక్షం(స్థిరరాశి నిర్మాణం checkout_opts *opts,
			      నిర్మాణం branch_info *old_branch_info,
			      నిర్మాణం branch_info *new_branch_info,
			      bool quiet,
			      పూర్ణాంకం *writeout_error)
{
	పూర్ణాంకం ret;
	నిర్మాణం తాళం_file తాళం_file = తాళం_INIT;
	నిర్మాణం వృక్షం *new_వృక్షం;

	repo_hold_locked_సూచీ(the_repository, &తాళం_file, తాళం_DIE_ON_ERROR);
	ఒకవేళ (repo_read_సూచీ_preload(the_repository, ఖాళీ, 0) < 0) {
		rollback_తాళం_file(&తాళం_file);
		తిరిగి error(_("index file corrupt"));
	}

	resolve_undo_clear_సూచీ(the_repository->సూచీ);
	ఒకవేళ (opts->new_orphan_branch && opts->orphan_from_ఖాళీలు_వృక్షం) {
		ఒకవేళ (new_branch_info->commit)
			BUG("'switch --orphan' should never accept a commit as starting point");
		new_వృక్షం = repo_parse_వృక్షం_పరోక్ష(the_repository,
						    the_hash_algo->ఖాళీలు_వృక్షం);
		ఒకవేళ (!new_వృక్షం)
			BUG("unable to read empty tree");
	} లేకపోతే {
		new_వృక్షం = repo_get_commit_వృక్షం(the_repository,
						new_branch_info->commit);
		ఒకవేళ (!new_వృక్షం) {
			rollback_తాళం_file(&తాళం_file);
			తిరిగి error(_("unable to read tree (%s)"),
				     oid_గమ్యం_hex(&new_branch_info->commit->object.oid));
		}
	}
	ఒకవేళ (opts->discard_changes) {
		ret = reset_వృక్షం(new_వృక్షం, opts, 1, writeout_error, new_branch_info);
		ఒకవేళ (ret) {
			rollback_తాళం_file(&తాళం_file);
			తిరిగి ret;
		}
	} లేకపోతే {
		నిర్మాణం వృక్షం_desc trees[2];
		నిర్మాణం వృక్షం *వృక్షం;
		నిర్మాణం unpack_trees_options topts;
		స్థిరరాశి నిర్మాణం object_id *old_commit_oid;

		refresh_సూచీ(the_repository->సూచీ, REFRESH_QUIET, ఖాళీ, ఖాళీ, ఖాళీ);

		ఒకవేళ (unmerged_సూచీ(the_repository->సూచీ)) {
			rollback_తాళం_file(&తాళం_file);
			error(_("you need to resolve your current index first"));
			తిరిగి 1;
		}

		/* 2-way merge to the new branch */
		init_topts(&topts, opts->show_పురోగతి,
			   opts->overwrite_ignore, quiet);
		init_checkout_metadata(&topts.meta, new_branch_info->refname,
				       new_branch_info->commit ?
				       &new_branch_info->commit->object.oid :
				       &new_branch_info->oid, ఖాళీ);

		old_commit_oid = old_branch_info->commit ?
			&old_branch_info->commit->object.oid :
			the_hash_algo->ఖాళీలు_వృక్షం;
		వృక్షం = repo_parse_వృక్షం_పరోక్ష(the_repository,
						old_commit_oid);
		ఒకవేళ (!వృక్షం)
			die(_("unable to parse commit %s"),
				oid_గమ్యం_hex(old_commit_oid));

		init_వృక్షం_desc(&trees[0], &వృక్షం->object.oid,
			       వృక్షం->బఫరు, వృక్షం->కొలత);
		ఒకవేళ (repo_parse_వృక్షం(the_repository, new_వృక్షం) < 0)
			die(ఖాళీ);
		వృక్షం = new_వృక్షం;
		init_వృక్షం_desc(&trees[1], &వృక్షం->object.oid,
			       వృక్షం->బఫరు, వృక్షం->కొలత);

		ret = unpack_trees(2, trees, &topts);
		clear_unpack_trees_porcelain(&topts);
		ఒకవేళ (ret == -1) {
			rollback_తాళం_file(&తాళం_file);
			తిరిగి విలీనం_WORKING_వృక్షం_UNPACK_FAILED;
		}
	}

	ఒకవేళ (!cache_వృక్షం_fully_valid(the_repository->సూచీ))
		cache_వృక్షం_update(the_repository->సూచీ, WRITE_వృక్షం_SILENT | WRITE_వృక్షం_REPAIR);

	ఒకవేళ (write_locked_సూచీ(the_repository->సూచీ, &తాళం_file, COMMIT_తాళం))
		die(_("unable to write new index file"));

	ఒకవేళ (!opts->discard_changes && !opts->quiet && new_branch_info->commit)
		show_local_changes(&new_branch_info->commit->object, &opts->diff_options);

	తిరిగి 0;
}

స్థిర శూన్య report_tracking(నిర్మాణం branch_info *new_branch_info)
{
	నిర్మాణం strbuf sb = STRBUF_INIT;
	నిర్మాణం branch *branch = branch_get(new_branch_info->name);

	ఒకవేళ (!format_tracking_info(branch, &sb, AHEAD_BEHIND_నిండిన, 1))
		తిరిగి;
	fputs(sb.buf, stdout);
	strbuf_release(&sb);
}

స్థిర శూన్య update_refs_for_switch(స్థిరరాశి నిర్మాణం checkout_opts *opts,
				   నిర్మాణం branch_info *old_branch_info,
				   నిర్మాణం branch_info *new_branch_info)
{
	నిర్మాణం strbuf సందేశం = STRBUF_INIT;
	స్థిరరాశి అక్షరం *old_desc, *reflog_సందేశం;
	ఒకవేళ (opts->new_branch) {
		ఒకవేళ (opts->new_orphan_branch) {
			గణన log_refs_config log_all_ref_updates = LOG_REFS_UNSET;
			స్థిరరాశి అక్షరం *విలువ;
			అక్షరం *refname;

			ఒకవేళ (!repo_config_get_string_tmp(the_repository, "core.logallrefupdates", &విలువ))
				log_all_ref_updates = refs_parse_log_all_ref_updates_config(విలువ);

			refname = mkpathdup("refs/heads/%s", opts->new_orphan_branch);
			ఒకవేళ (opts->new_branch_log &&
			    !should_autocreate_reflog(log_all_ref_updates, refname)) {
				పూర్ణాంకం ret;
				నిర్మాణం strbuf err = STRBUF_INIT;

				ret = refs_create_reflog(get_main_ref_store(the_repository),
							 refname, &err);
				ఒకవేళ (ret) {
					fprintf(stderr, _("Can not do reflog for '%s': %s\n"),
						opts->new_orphan_branch, err.buf);
					strbuf_release(&err);
					విడుదల(refname);
					తిరిగి;
				}
				strbuf_release(&err);
			}
			విడుదల(refname);
		}
		లేకపోతే
			create_branch(the_repository,
				      opts->new_branch, new_branch_info->name,
				      opts->new_branch_force ? 1 : 0,
				      opts->new_branch_force ? 1 : 0,
				      opts->new_branch_log,
				      opts->quiet,
				      opts->track,
				      0);
		విడుదల(new_branch_info->name);
		విడుదల(new_branch_info->refname);
		new_branch_info->name = xstrdup(opts->new_branch);
		అమరిక_branch_మార్గం(new_branch_info);
	}

	old_desc = old_branch_info->name;
	ఒకవేళ (!old_desc && old_branch_info->commit)
		old_desc = oid_గమ్యం_hex(&old_branch_info->commit->object.oid);

	reflog_సందేశం = getenv("GIT_REFLOG_ACTION");
	ఒకవేళ (!reflog_సందేశం)
		strbuf_addf(&సందేశం, "checkout: moving from %s to %s",
			old_desc ? old_desc : "(invalid)", new_branch_info->name);
	లేకపోతే
		strbuf_insertstr(&సందేశం, 0, reflog_సందేశం);

	ఒకవేళ (!strcmp(new_branch_info->name, "HEAD") && !new_branch_info->మార్గం && !opts->force_detach) {
		/* Nothing to do. */
	} లేకపోతే ఒకవేళ (opts->force_detach || !new_branch_info->మార్గం) {	/* No longer on any branch. */
		refs_update_ref(get_main_ref_store(the_repository), సందేశం.buf,
				"HEAD", &new_branch_info->commit->object.oid,
				ఖాళీ,
				REF_NO_DEREF, UPDATE_REFS_DIE_ON_ERR);
		ఒకవేళ (!opts->quiet) {
			ఒకవేళ (old_branch_info->మార్గం &&
			    advice_enabled(ADVICE_DETACHED_తల) && !opts->force_detach)
				detach_advice(new_branch_info->name);
			describe_detached_తల(_("HEAD is now at"), new_branch_info->commit);
		}
	} లేకపోతే ఒకవేళ (new_branch_info->మార్గం) {	/* Switch branches. */
		ఒకవేళ (refs_update_symref(get_main_ref_store(the_repository), "HEAD", new_branch_info->మార్గం, సందేశం.buf) < 0)
			die(_("unable to update HEAD"));
		ఒకవేళ (!opts->quiet) {
			ఒకవేళ (old_branch_info->మార్గం && !strcmp(new_branch_info->మార్గం, old_branch_info->మార్గం)) {
				ఒకవేళ (opts->new_branch_force)
					fprintf(stderr, _("Reset branch '%s'\n"),
						new_branch_info->name);
				లేకపోతే
					fprintf(stderr, _("Already on '%s'\n"),
						new_branch_info->name);
			} లేకపోతే ఒకవేళ (opts->new_branch) {
				ఒకవేళ (opts->branch_exists)
					fprintf(stderr, _("Switched to and reset branch '%s'\n"), new_branch_info->name);
				లేకపోతే
					fprintf(stderr, _("Switched to a new branch '%s'\n"), new_branch_info->name);
			} లేకపోతే {
				fprintf(stderr, _("Switched to branch '%s'\n"),
					new_branch_info->name);
			}
		}
		ఒకవేళ (old_branch_info->మార్గం && old_branch_info->name) {
			ఒకవేళ (!refs_ref_exists(get_main_ref_store(the_repository), old_branch_info->మార్గం) && refs_reflog_exists(get_main_ref_store(the_repository), old_branch_info->మార్గం))
				refs_delete_reflog(get_main_ref_store(the_repository),
						   old_branch_info->మార్గం);
		}
	}
	remove_branch_state(the_repository, !opts->quiet);
	strbuf_release(&సందేశం);
	ఒకవేళ (!opts->quiet &&
	    !opts->force_detach &&
	    (new_branch_info->మార్గం || !strcmp(new_branch_info->name, "HEAD")))
		report_tracking(new_branch_info);
}

స్థిర పూర్ణాంకం కూడు_pending_uninteresting_ref(స్థిరరాశి నిర్మాణం reference *ref, శూన్య *cb_దత్తాంశం)
{
	కూడు_pending_oid(cb_దత్తాంశం, ref->name, ref->oid, UNINTERESTING);
	తిరిగి 0;
}

స్థిర శూన్య describe_one_orphan(నిర్మాణం strbuf *sb, నిర్మాణం commit *commit)
{
	strbuf_addstr(sb, "  ");
	strbuf_కూడు_unique_abbrev(sb, &commit->object.oid, DEFAULT_ABBREV);
	strbuf_addch(sb, ' ');
	ఒకవేళ (!repo_parse_commit(the_repository, commit))
		pp_commit_easy(CMIT_FMT_ONELINE, commit, sb);
	strbuf_addch(sb, '\n');
}

#నిర్వచించు ORPHAN_CUTOFF 4
స్థిర శూన్య suggest_reattach(నిర్మాణం commit *commit, నిర్మాణం rev_info *revs)
{
	నిర్మాణం commit *c, *చివరి = ఖాళీ;
	నిర్మాణం strbuf sb = STRBUF_INIT;
	పూర్ణాంకం కోల్పోయిన = 0;
	అయితే ((c = get_revision(revs)) != ఖాళీ) {
		ఒకవేళ (కోల్పోయిన < ORPHAN_CUTOFF)
			describe_one_orphan(&sb, c);
		చివరి = c;
		కోల్పోయిన++;
	}
	ఒకవేళ (ORPHAN_CUTOFF < కోల్పోయిన) {
		పూర్ణాంకం మరిన్ని = కోల్పోయిన - ORPHAN_CUTOFF;
		ఒకవేళ (మరిన్ని == 1)
			describe_one_orphan(&sb, చివరి);
		లేకపోతే
			strbuf_addf(&sb, _(" ... and %d more.\n"), మరిన్ని);
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
		కోల్పోయిన),
		కోల్పోయిన,
		sb.buf);
	strbuf_release(&sb);

	ఒకవేళ (advice_enabled(ADVICE_DETACHED_తల))
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
			కోల్పోయిన),
			repo_కనుగొను_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV));
}

/*
 * We are about to leave commit that was at the tip of a detached
 * HEAD.  If it is not reachable from any ref, this is the last chance
 * for the user to do so without resorting to reflog.
 */
స్థిర శూన్య orphaned_commit_warning(నిర్మాణం commit *old_commit, నిర్మాణం commit *new_commit)
{
	నిర్మాణం rev_info revs;
	నిర్మాణం object *object = &old_commit->object;

	repo_init_revisions(the_repository, &revs, ఖాళీ);
	అమరిక_revisions(0, ఖాళీ, &revs, ఖాళీ);

	object->flags &= ~UNINTERESTING;
	కూడు_pending_object(&revs, object, oid_గమ్యం_hex(&object->oid));

	refs_for_each_ref(get_main_ref_store(the_repository),
			  కూడు_pending_uninteresting_ref, &revs);
	ఒకవేళ (new_commit)
		కూడు_pending_oid(&revs, "HEAD",
				&new_commit->object.oid,
				UNINTERESTING);

	ఒకవేళ (prepare_revision_walk(&revs))
		die(_("internal error in revision walk"));
	ఒకవేళ (!(old_commit->object.flags & UNINTERESTING))
		suggest_reattach(old_commit, &revs);
	లేకపోతే
		describe_detached_తల(_("Previous HEAD position was"), old_commit);

	/* Clean up objects used, as they will be reused. */
	repo_clear_commit_marks(the_repository, ALL_REV_FLAGS);
	release_revisions(&revs);
}

స్థిర పూర్ణాంకం switch_branches(స్థిరరాశి నిర్మాణం checkout_opts *opts,
			   నిర్మాణం branch_info *new_branch_info)
{
	పూర్ణాంకం ret = 0;
	నిర్మాణం branch_info old_branch_info = { 0 };
	నిర్మాణం object_id rev;
	పూర్ణాంకం flag, writeout_error = 0;
	పూర్ణాంకం do_విలీనం = 1;
	పూర్ణాంకం created_autostash = 0;
	గణన stash_apply_ఫలితం autostash_res = STASH_APPLY_CLEAN;
	నిర్మాణం strbuf old_commit_shortname = STRBUF_INIT;
	నిర్మాణం strbuf autostash_సందేశం = STRBUF_INIT;
	స్థిరరాశి అక్షరం *stash_label_పునాది = ఖాళీ;

	trace2_cmd_బహుళకం("branch");

	memset(&old_branch_info, 0, పరిమాణం(old_branch_info));
	old_branch_info.మార్గం = refs_resolve_refdup(get_main_ref_store(the_repository),
						   "HEAD", 0, &rev, &flag);
	ఒకవేళ (old_branch_info.మార్గం)
		old_branch_info.commit = lookup_commit_reference_gently(the_repository, &rev, 1);
	ఒకవేళ (!(flag & REF_ISSYMREF))
		FREE_AND_NULL(old_branch_info.మార్గం);

	ఒకవేళ (old_branch_info.మార్గం) {
		స్థిరరాశి అక్షరం *స్థిరరాశి ఉపసర్గ = "refs/heads/";
		స్థిరరాశి అక్షరం *p;
		ఒకవేళ (skip_ఉపసర్గ(old_branch_info.మార్గం, ఉపసర్గ, &p))
			old_branch_info.name = xstrdup(p);
	}

	ఒకవేళ (opts->new_orphan_branch && opts->orphan_from_ఖాళీలు_వృక్షం) {
		ఒకవేళ (new_branch_info->name)
			BUG("'switch --orphan' should never accept a commit as starting point");
		new_branch_info->commit = ఖాళీ;
		new_branch_info->name = xstrdup("(empty)");
		do_విలీనం = 1;
	}

	ఒకవేళ (!new_branch_info->name) {
		new_branch_info->name = xstrdup("HEAD");
		new_branch_info->commit = old_branch_info.commit;
		ఒకవేళ (!new_branch_info->commit)
			die(_("You are on a branch yet to be born"));
		parse_commit_or_die(new_branch_info->commit);

		ఒకవేళ (opts->only_విలీనం_on_switching_branches)
			do_విలీనం = 0;
	}

	ఒకవేళ (old_branch_info.name) {
		stash_label_పునాది = old_branch_info.name;
	} లేకపోతే ఒకవేళ (old_branch_info.commit) {
		strbuf_కూడు_unique_abbrev(&old_commit_shortname,
					 &old_branch_info.commit->object.oid,
					 DEFAULT_ABBREV);
		stash_label_పునాది = old_commit_shortname.buf;
	}

	ఒకవేళ (do_విలీనం) {
		ret = విలీనం_working_వృక్షం(opts, &old_branch_info, new_branch_info,
					 opts->విలీనం, &writeout_error);
		ఒకవేళ (ret == విలీనం_WORKING_వృక్షం_UNPACK_FAILED && opts->విలీనం) {
			strbuf_addf(&autostash_సందేశం,
				    "autostash while switching to '%s'",
				    new_branch_info->name);
			create_autostash_ref(the_repository,
					     "CHECKOUT_AUTOSTASH_HEAD",
					     autostash_సందేశం.buf, true);
			created_autostash = 1;
			ret = విలీనం_working_వృక్షం(opts, &old_branch_info, new_branch_info,
						 false, &writeout_error);
		}
		ఒకవేళ (created_autostash) {
			ఒకవేళ (opts->conflict_style >= 0) {
				నిర్మాణం strbuf cfg = STRBUF_INIT;
				strbuf_addf(&cfg, "merge.conflictStyle=%s",
					    conflict_style_name(opts->conflict_style));
				git_config_నెట్టు_parameter(cfg.buf);
				strbuf_release(&cfg);
			}
			autostash_res = apply_autostash_ref(the_repository,
				    "CHECKOUT_AUTOSTASH_HEAD",
				    new_branch_info->name,
				    "local",
				    stash_label_పునాది,
				    autostash_సందేశం.buf);
		}
		ఒకవేళ (ret) {
			branch_info_release(&old_branch_info);
			strbuf_release(&old_commit_shortname);
			strbuf_release(&autostash_సందేశం);
			తిరిగి ret < 0 ? 1 : ret;
		}
	}

	ఒకవేళ (!opts->quiet && !old_branch_info.మార్గం && old_branch_info.commit && new_branch_info->commit != old_branch_info.commit)
		orphaned_commit_warning(old_branch_info.commit, new_branch_info->commit);

	ఒకవేళ (autostash_res == STASH_APPLY_CONFLICT && !opts->quiet)
		fputc('\n', stderr);
	update_refs_for_switch(opts, &old_branch_info, new_branch_info);

	ఒకవేళ (created_autostash) {
		discard_సూచీ(the_repository->సూచీ);
		ఒకవేళ (repo_read_సూచీ(the_repository) < 0)
			die(_("index file corrupt"));

		ఒకవేళ (!opts->quiet && new_branch_info->commit) {
			రాయి(_("The following paths have local changes:\n"));
			show_local_changes(&new_branch_info->commit->object,
					   &opts->diff_options);
		}
	}

	ret = post_checkout_hook(old_branch_info.commit, new_branch_info->commit, 1);
	branch_info_release(&old_branch_info);
	strbuf_release(&old_commit_shortname);
	strbuf_release(&autostash_సందేశం);

	తిరిగి ret || writeout_error;
}

స్థిర పూర్ణాంకం git_checkout_config(స్థిరరాశి అక్షరం *var, స్థిరరాశి అక్షరం *విలువ,
			       స్థిరరాశి నిర్మాణం config_context *ctx, శూన్య *cb)
{
	నిర్మాణం checkout_opts *opts = cb;

	ఒకవేళ (!strcmp(var, "diff.ignoresubmodules")) {
		ఒకవేళ (!విలువ)
			తిరిగి config_error_nonbool(var);
		handle_ignore_submodules_arg(&opts->diff_options, విలువ);
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(var, "checkout.guess")) {
		opts->dwim_new_local_branch = git_config_bool(var, విలువ);
		తిరిగి 0;
	}

	ఒకవేళ (starts_with(var, "submodule."))
		తిరిగి git_default_submodule_config(var, విలువ, ఖాళీ);

	తిరిగి git_xmerge_config(var, విలువ, ctx, ఖాళీ);
}

స్థిర శూన్య అమరిక_new_branch_info_and_ఆధారం_వృక్షం(
	నిర్మాణం branch_info *new_branch_info,
	నిర్మాణం checkout_opts *opts,
	నిర్మాణం object_id *rev,
	స్థిరరాశి అక్షరం *arg)
{
	నిర్మాణం వృక్షం **ఆధారం_వృక్షం = &opts->ఆధారం_వృక్షం;
	నిర్మాణం object_id branch_rev;

	/* treat '@' as a shortcut for 'HEAD' */
	new_branch_info->name = !strcmp(arg, "@") ? xstrdup("HEAD") :
						    xstrdup(arg);
	అమరిక_branch_మార్గం(new_branch_info);

	ఒకవేళ (!check_refname_format(new_branch_info->మార్గం, 0) &&
	    !refs_read_ref(get_main_ref_store(the_repository), new_branch_info->మార్గం, &branch_rev))
		oidcpy(rev, &branch_rev);
	లేకపోతే
		/* not an existing branch */
		FREE_AND_NULL(new_branch_info->మార్గం);

	new_branch_info->commit = lookup_commit_reference_gently(the_repository, rev, 1);
	ఒకవేళ (!new_branch_info->commit) {
		/* not a commit */
		*ఆధారం_వృక్షం = repo_parse_వృక్షం_పరోక్ష(the_repository, rev);
		ఒకవేళ (!*ఆధారం_వృక్షం)
			die(_("unable to read tree (%s)"), oid_గమ్యం_hex(rev));
	} లేకపోతే {
		parse_commit_or_die(new_branch_info->commit);
		*ఆధారం_వృక్షం = repo_get_commit_వృక్షం(the_repository,
						    new_branch_info->commit);
		ఒకవేళ (!*ఆధారం_వృక్షం)
			die(_("unable to read tree (%s)"),
			    oid_గమ్యం_hex(&new_branch_info->commit->object.oid));
	}
}


గణన checkout_command {
	CHECKOUT_CHECKOUT = 1,
	CHECKOUT_SWITCH = 2,
	CHECKOUT_RESTORE = 3,
};

స్థిర శూన్య advise_disambiguating_remotes(గణన checkout_command which_command,
					  స్థిరరాశి అక్షరం *branch,
					  స్థిరరాశి నిర్మాణం string_list *matched_remote_పేర్లు)
{
	స్థిరరాశి అక్షరం *cmdname;
	నిర్మాణం string_list_అంశం *అంశం;

	ఎంపిక (which_command) {
	స్థితి CHECKOUT_CHECKOUT:
		cmdname = "checkout";
		ఆపు;
	స్థితి CHECKOUT_SWITCH:
		cmdname = "switch";
		ఆపు;
	అప్రమేయ:
		BUG("command <%d> should not reach advise_disambiguating_remotes",
		    which_command);
		ఆపు;
	}

	advise(_("Branch name '%s' appears in multiple remotes:"), branch);
	for_each_string_list_అంశం(అంశం, matched_remote_పేర్లు) {
		advise(_("  %s"), అంశం->string);
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

స్థిర అక్షరం *parse_remote_branch(స్థిరరాశి అక్షరం *arg,
				 నిర్మాణం object_id *rev,
				 పూర్ణాంకం could_be_checkout_paths,
				 గణన checkout_command which_command)
{
	పూర్ణాంకం అంకె_matches = 0;
	నిర్మాణం string_list matched_remote_పేర్లు = STRING_LIST_INIT_DUP;

	అక్షరం *remote = unique_tracking_name(arg, rev, &అంకె_matches,
					    &matched_remote_పేర్లు);

	ఒకవేళ (remote && could_be_checkout_paths) {
		die(_("'%s' could be both a local file and a tracking branch.\n"
			"Please use -- (and optionally --no-guess) to disambiguate"),
		    arg);
	}

	ఒకవేళ (!remote && అంకె_matches > 1) {
		ఒకవేళ (advice_enabled(ADVICE_CHECKOUT_AMBIGUOUS_REMOTE_BRANCH_NAME))
			advise_disambiguating_remotes(which_command, arg,
						      &matched_remote_పేర్లు);
		die(_("'%s' matched multiple (%d) remote tracking branches"),
		    arg, అంకె_matches);
	}

	string_list_clear(&matched_remote_పేర్లు, 0);

	తిరిగి remote;
}

స్థిర పూర్ణాంకం parse_branchname_arg(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv,
				పూర్ణాంకం dwim_new_local_branch_ok,
				గణన checkout_command which_command,
				నిర్మాణం branch_info *new_branch_info,
				నిర్మాణం checkout_opts *opts,
				నిర్మాణం object_id *rev)
{
	స్థిరరాశి అక్షరం **new_branch = &opts->new_branch;
	పూర్ణాంకం argcount = 0;
	స్థిరరాశి అక్షరం *arg;
	అక్షరం *remote = ఖాళీ;
	పూర్ణాంకం dash_dash_pos;
	పూర్ణాంకం has_dash_dash = 0;
	పూర్ణాంకం i;

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
	ఒకవేళ (!argc)
		తిరిగి 0;

	ఒకవేళ (!opts->accept_pathspec) {
		ఒకవేళ (argc > 1)
			die(_("only one reference expected"));
		has_dash_dash = 1; /* helps disambiguate */
	}

	arg = argv[0];
	dash_dash_pos = -1;
	కొరకు (i = 0; i < argc; i++) {
		ఒకవేళ (opts->accept_pathspec && !strcmp(argv[i], "--")) {
			dash_dash_pos = i;
			ఆపు;
		}
	}
	ఒకవేళ (dash_dash_pos == 0)
		తిరిగి 1; /* case (2) */
	లేకపోతే ఒకవేళ (dash_dash_pos == 1)
		has_dash_dash = 1; /* case (3) or (1) */
	లేకపోతే ఒకవేళ (dash_dash_pos >= 2)
		die(_("only one reference expected, %d given."), dash_dash_pos);
	opts->లెక్క_checkout_paths = !opts->quiet && !has_dash_dash;

	ఒకవేళ (!strcmp(arg, "-"))
		arg = "@{-1}";

	ఒకవేళ (repo_get_oid_mb(the_repository, arg, rev)) {
		/*
		 * Either case (3) or (4), with <something> not being
		 * a commit, or an attempt to use case (1) with an
		 * invalid ref.
		 *
		 * It's likely an error, but we need to find out if
		 * we should auto-create the branch, case (3).(b).
		 */
		పూర్ణాంకం recover_with_dwim = dwim_new_local_branch_ok;

		పూర్ణాంకం could_be_checkout_paths = !has_dash_dash &&
			check_filename(opts->ఉపసర్గ, arg);

		ఒకవేళ (!has_dash_dash && !no_wildcard(arg))
			recover_with_dwim = 0;

		/*
		 * Accept "git checkout foo", "git checkout foo --"
		 * and "git switch foo" as candidates for dwim.
		 */
		ఒకవేళ (!(argc == 1 && !has_dash_dash) &&
		    !(argc == 2 && has_dash_dash) &&
		    opts->accept_pathspec)
			recover_with_dwim = 0;

		ఒకవేళ (recover_with_dwim) {
			remote = parse_remote_branch(arg, rev,
						     could_be_checkout_paths,
						     which_command);
			ఒకవేళ (remote) {
				*new_branch = arg;
				arg = remote;
				/* DWIMmed to create local branch, case (3).(b) */
			} లేకపోతే {
				recover_with_dwim = 0;
			}
		}

		ఒకవేళ (!recover_with_dwim) {
			ఒకవేళ (has_dash_dash)
				die(_("invalid reference: %s"), arg);
			తిరిగి argcount;
		}
	}

	/* we can't end up being in (2) anymore, eat the argument */
	argcount++;
	argv++;
	argc--;

	అమరిక_new_branch_info_and_ఆధారం_వృక్షం(new_branch_info, opts, rev, arg);

	ఒకవేళ (!opts->ఆధారం_వృక్షం)                   /* case (1): want a tree */
		die(_("reference is not a tree: %s"), arg);

	ఒకవేళ (!has_dash_dash) {	/* case (3).(d) -> (1) */
		/*
		 * Do not complain the most common case
		 *	git checkout branch
		 * even if there happen to be a file called 'branch';
		 * it would be extremely annoying.
		 */
		ఒకవేళ (argc)
			verify_non_filename(the_repository, opts->ఉపసర్గ, arg);
	} లేకపోతే ఒకవేళ (opts->accept_pathspec) {
		argcount++;
		argv++;
		argc--;
	}

	విడుదల(remote);
	తిరిగి argcount;
}

స్థిర పూర్ణాంకం switch_unborn_గమ్యం_new_branch(స్థిరరాశి నిర్మాణం checkout_opts *opts)
{
	పూర్ణాంకం స్థితి_2;
	నిర్మాణం strbuf branch_ref = STRBUF_INIT;

	trace2_cmd_బహుళకం("unborn");

	ఒకవేళ (!opts->new_branch)
		die(_("You are on a branch yet to be born"));
	strbuf_addf(&branch_ref, "refs/heads/%s", opts->new_branch);
	స్థితి_2 = refs_update_symref(get_main_ref_store(the_repository),
				    "HEAD", branch_ref.buf, "checkout -b");
	strbuf_release(&branch_ref);
	ఒకవేళ (!opts->quiet)
		fprintf(stderr, _("Switched to a new branch '%s'\n"),
			opts->new_branch);
	తిరిగి స్థితి_2;
}

స్థిర శూన్య die_expecting_a_branch(స్థిరరాశి నిర్మాణం branch_info *branch_info)
{
	నిర్మాణం object_id oid;
	అక్షరం *గమ్యం_free;
	పూర్ణాంకం సంకేతం_2;

	ఒకవేళ (repo_dwim_ref(the_repository, branch_info->name,
			  అక్షరశ్రేణి_పొడవు(branch_info->name), &oid, &గమ్యం_free, 0) == 1) {
		స్థిరరాశి అక్షరం *ref = గమ్యం_free;

		ఒకవేళ (skip_ఉపసర్గ(ref, "refs/tags/", &ref))
			సంకేతం_2 = die_సందేశం(_("a branch is expected, got tag '%s'"), ref);
		లేకపోతే ఒకవేళ (skip_ఉపసర్గ(ref, "refs/remotes/", &ref))
			సంకేతం_2 = die_సందేశం(_("a branch is expected, got remote branch '%s'"), ref);
		లేకపోతే
			సంకేతం_2 = die_సందేశం(_("a branch is expected, got '%s'"), ref);
	}
	లేకపోతే ఒకవేళ (branch_info->commit)
		సంకేతం_2 = die_సందేశం(_("a branch is expected, got commit '%s'"), branch_info->name);
	లేకపోతే
		/*
		 * This case should never happen because we already die() on
		 * non-commit, but just in case.
		 */
		సంకేతం_2 = die_సందేశం(_("a branch is expected, got '%s'"), branch_info->name);

	ఒకవేళ (advice_enabled(ADVICE_SUGGEST_DETACHING_తల))
		advise(_("If you want to detach HEAD at the commit, try again with the --detach option."));

	నిష్క్రమించు(సంకేతం_2);
}

స్థిర శూన్య die_if_some_operation_లోపల_పురోగతి(శూన్య)
{
	నిర్మాణం wt_స్థితి_state state;

	memset(&state, 0, పరిమాణం(state));
	wt_స్థితి_get_state(the_repository, &state, 0);

	ఒకవేళ (state.విలీనం_లోపల_పురోగతి)
		die(_("cannot switch branch while merging\n"
		      "Consider \"git merge --quit\" "
		      "or \"git worktree add\"."));
	ఒకవేళ (state.am_లోపల_పురోగతి)
		die(_("cannot switch branch in the middle of an am session\n"
		      "Consider \"git am --quit\" "
		      "or \"git worktree add\"."));
	ఒకవేళ (state.rebase_interactive_లోపల_పురోగతి || state.rebase_లోపల_పురోగతి)
		die(_("cannot switch branch while rebasing\n"
		      "Consider \"git rebase --quit\" "
		      "or \"git worktree add\"."));
	ఒకవేళ (state.cherry_pick_లోపల_పురోగతి)
		die(_("cannot switch branch while cherry-picking\n"
		      "Consider \"git cherry-pick --quit\" "
		      "or \"git worktree add\"."));
	ఒకవేళ (state.revert_లోపల_పురోగతి)
		die(_("cannot switch branch while reverting\n"
		      "Consider \"git revert --quit\" "
		      "or \"git worktree add\"."));
	ఒకవేళ (state.bisect_లోపల_పురోగతి)
		warning(_("you are switching branch while bisecting"));

	wt_స్థితి_state_free_buffers(&state);
}

/*
 * die if attempting to checkout an existing branch that is in use
 * in another worktree, unless ignore-other-wortrees option is given.
 * The check is bypassed when the branch is already the current one,
 * as it will not make things any worse.
 */
స్థిర శూన్య die_if_switching_గమ్యం_a_branch_లోపల_use(నిర్మాణం checkout_opts *opts,
						స్థిరరాశి అక్షరం *నిండిన_ref)
{
	పూర్ణాంకం flags;
	అక్షరం *తల_ref;

	ఒకవేళ (opts->ignore_other_worktrees)
		తిరిగి;
	తల_ref = refs_resolve_refdup(get_main_ref_store(the_repository),
				       "HEAD", 0, ఖాళీ, &flags);
	ఒకవేళ (తల_ref && (!(flags & REF_ISSYMREF) || strcmp(తల_ref, నిండిన_ref)))
		die_if_checked_బహిర్గతం(నిండిన_ref, 1);
	విడుదల(తల_ref);
}

స్థిర పూర్ణాంకం checkout_branch(నిర్మాణం checkout_opts *opts,
			   నిర్మాణం branch_info *new_branch_info)
{
	నిర్మాణం repo_config_విలువలు *cfg = repo_config_విలువలు(the_repository);
	పూర్ణాంకం noop_switch = (!new_branch_info->name &&
			   !opts->new_branch &&
			   !opts->force_detach);

	ఒకవేళ (opts->pathspec.nr)
		die(_("paths cannot be used with switching branches"));

	ఒకవేళ (opts->patch_బహుళకం)
		die(_("'%s' cannot be used with switching branches"),
		    "--patch");

	ఒకవేళ (opts->overlay_బహుళకం != -1)
		die(_("'%s' cannot be used with switching branches"),
		    "--[no]-overlay");

	ఒకవేళ (opts->writeout_stage) {
		స్థిరరాశి అక్షరం *సందేశం;
		ఒకవేళ (noop_switch)
			సందేశం = _("'%s' needs the paths to check out");
		లేకపోతే
			సందేశం = _("'%s' cannot be used with switching branches");
		die(సందేశం, "--ours/--theirs");
	}

	ఒకవేళ (opts->force && opts->విలీనం)
		die(_("'%s' cannot be used with '%s'"), "-f", "-m");

	ఒకవేళ (opts->discard_changes && opts->విలీనం)
		die(_("'%s' cannot be used with '%s'"), "--discard-changes", "--merge");

	ఒకవేళ (opts->force_detach && opts->new_branch)
		die(_("'%s' cannot be used with '%s'"),
		    "--detach", "-b/-B/--orphan");

	ఒకవేళ (opts->new_orphan_branch) {
		ఒకవేళ (opts->track != BRANCH_TRACK_UNSPECIFIED)
			die(_("'%s' cannot be used with '%s'"), "--orphan", "-t");
		ఒకవేళ (opts->orphan_from_ఖాళీలు_వృక్షం && new_branch_info->name)
			die(_("'%s' cannot take <start-point>"), "--orphan");
	} లేకపోతే ఒకవేళ (opts->force_detach) {
		ఒకవేళ (opts->track != BRANCH_TRACK_UNSPECIFIED)
			die(_("'%s' cannot be used with '%s'"), "--detach", "-t");
	} లేకపోతే ఒకవేళ (opts->track == BRANCH_TRACK_UNSPECIFIED)
		opts->track = cfg->branch_track;

	ఒకవేళ (new_branch_info->name && !new_branch_info->commit)
		die(_("Cannot switch branch to a non-commit '%s'"),
		    new_branch_info->name);

	ఒకవేళ (noop_switch &&
	    !opts->switch_branch_doing_nothing_is_ok)
		die(_("missing branch or commit argument"));

	ఒకవేళ (!opts->implicit_detach &&
	    !opts->force_detach &&
	    !opts->new_branch &&
	    !opts->new_branch_force &&
	    new_branch_info->name &&
	    !new_branch_info->మార్గం)
		die_expecting_a_branch(new_branch_info);

	ఒకవేళ (!opts->can_switch_when_లోపల_పురోగతి)
		die_if_some_operation_లోపల_పురోగతి();

	/* "git checkout <branch>" */
	ఒకవేళ (new_branch_info->మార్గం && !opts->force_detach && !opts->new_branch)
		die_if_switching_గమ్యం_a_branch_లోపల_use(opts, new_branch_info->మార్గం);

	/* "git checkout -B <branch>" */
	ఒకవేళ (opts->new_branch_force) {
		అక్షరం *నిండిన_ref = xstrfmt("refs/heads/%s", opts->new_branch);
		die_if_switching_గమ్యం_a_branch_లోపల_use(opts, నిండిన_ref);
		విడుదల(నిండిన_ref);
	}

	ఒకవేళ (!new_branch_info->commit && opts->new_branch) {
		నిర్మాణం object_id rev;
		పూర్ణాంకం flag;

		ఒకవేళ (!refs_read_ref_నిండిన(get_main_ref_store(the_repository), "HEAD", 0, &rev, &flag) &&
		    (flag & REF_ISSYMREF) && is_null_oid(&rev))
			తిరిగి switch_unborn_గమ్యం_new_branch(opts);
	}
	తిరిగి switch_branches(opts, new_branch_info);
}

స్థిర పూర్ణాంకం parse_opt_conflict(స్థిరరాశి నిర్మాణం option *o, స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	నిర్మాణం checkout_opts *opts = o->విలువ;

	ఒకవేళ (unset) {
		opts->conflict_style = -1;
		తిరిగి 0;
	}
	opts->conflict_style = parse_conflict_style_name(arg);
	ఒకవేళ (opts->conflict_style < 0)
		తిరిగి error(_("unknown conflict style '%s'"), arg);
	/* --conflict overrides a previous --no-merge */
	ఒకవేళ (!opts->విలీనం)
		opts->విలీనం = -1;

	తిరిగి 0;
}

స్థిర నిర్మాణం option *కూడు_common_options(నిర్మాణం checkout_opts *opts,
					 నిర్మాణం option *prevopts)
{
	నిర్మాణం option options[] = {
		OPT__QUIET(&opts->quiet, N_("suppress progress reporting")),
		OPT_CALLBACK_F(0, "recurse-submodules", ఖాళీ,
			    "checkout", "control recursive updating of submodules",
			    PARSE_OPT_OPTARG, option_parse_recurse_submodules_worktree_updater),
		OPT_BOOL(0, "progress", &opts->show_పురోగతి, N_("force progress reporting")),
		OPT_BOOL('m', "merge", &opts->విలీనం, N_("perform a 3-way merge with the new branch")),
		OPT_CALLBACK(0, "conflict", opts, N_("style"),
			     N_("conflict style (merge, diff3, or zdiff3)"),
			     parse_opt_conflict),
		OPT_చివర()
	};
	నిర్మాణం option *newopts = parse_options_concat(prevopts, options);
	విడుదల(prevopts);
	తిరిగి newopts;
}

స్థిర నిర్మాణం option *కూడు_common_switch_branch_options(
	నిర్మాణం checkout_opts *opts, నిర్మాణం option *prevopts)
{
	నిర్మాణం option options[] = {
		OPT_BOOL('d', "detach", &opts->force_detach, N_("detach HEAD at named commit")),
		OPT_CALLBACK_F('t', "track",  &opts->track, "(direct|inherit)",
			N_("set branch tracking configuration"),
			PARSE_OPT_OPTARG,
			parse_opt_tracking_బహుళకం),
		OPT__FORCE(&opts->force, N_("force checkout (throw away local modifications)"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_STRING(0, "orphan", &opts->new_orphan_branch, N_("new-branch"), N_("new unborn branch")),
		OPT_BOOL_F(0, "overwrite-ignore", &opts->overwrite_ignore,
			   N_("update ignored files (default)"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "ignore-other-worktrees", &opts->ignore_other_worktrees,
			 N_("do not check if another worktree is using this branch")),
		OPT_చివర()
	};
	నిర్మాణం option *newopts = parse_options_concat(prevopts, options);
	విడుదల(prevopts);
	తిరిగి newopts;
}

స్థిర నిర్మాణం option *కూడు_checkout_మార్గం_options(నిర్మాణం checkout_opts *opts,
						నిర్మాణం option *prevopts)
{
	నిర్మాణం option options[] = {
		OPT_సమితి_INT_F('2', "ours", &opts->writeout_stage,
			      N_("checkout our version for unmerged files"),
			      2, PARSE_OPT_NONEG),
		OPT_సమితి_INT_F('3', "theirs", &opts->writeout_stage,
			      N_("checkout their version for unmerged files"),
			      3, PARSE_OPT_NONEG),
		OPT_BOOL('p', "patch", &opts->patch_బహుళకం, N_("select hunks interactively")),
		OPT_DIFF_UNIFIED(&opts->patch_context),
		OPT_DIFF_INTERHUNK_CONTEXT(&opts->patch_interhunk_context),
		OPT_BOOL(0, "ignore-skip-worktree-bits", &opts->ignore_skipworktree,
			 N_("do not limit pathspecs to sparse entries only")),
		OPT_PATHSPEC_FROM_FILE(&opts->pathspec_from_file),
		OPT_PATHSPEC_FILE_NUL(&opts->pathspec_file_nul),
		OPT_చివర()
	};
	నిర్మాణం option *newopts = parse_options_concat(prevopts, options);
	విడుదల(prevopts);
	తిరిగి newopts;
}

/* create-branch option (either b or c) */
స్థిర అక్షరం cb_option = 'b';

స్థిర పూర్ణాంకం checkout_main(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
			 నిర్మాణం checkout_opts *opts, నిర్మాణం option *options,
			 గణన checkout_command which_command)
{
	పూర్ణాంకం parseopt_flags = 0;
	నిర్మాణం branch_info new_branch_info = { 0 };
	పూర్ణాంకం ret;

	స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి checkout_usage[] = {
		N_("git checkout [<options>] <branch>"),
		N_("git checkout [<options>] [<branch>] -- <file>..."),
		ఖాళీ,
	};

	స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి switch_branch_usage[] = {
		N_("git switch [<options>] [<branch>]"),
		ఖాళీ,
	};

	స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి restore_usage[] = {
		N_("git restore [<options>] [--source=<branch>] <file>..."),
		ఖాళీ,
	};

	స్థిరరాశి అక్షరం * స్థిరరాశి *usagestr;

	ఎంపిక (which_command) {
	స్థితి CHECKOUT_CHECKOUT:
		usagestr = checkout_usage;
		ఆపు;
	స్థితి CHECKOUT_SWITCH:
		usagestr = switch_branch_usage;
		ఆపు;
	స్థితి CHECKOUT_RESTORE:
		usagestr = restore_usage;
		ఆపు;
	అప్రమేయ:
		BUG("no such checkout variant %d", which_command);
	}

	opts->overwrite_ignore = 1;
	opts->ఉపసర్గ = ఉపసర్గ;
	opts->show_పురోగతి = -1;

	repo_config(the_repository, git_checkout_config, opts);
	ఒకవేళ (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_నిండిన_సూచీ = 0;
	}

	opts->track = BRANCH_TRACK_UNSPECIFIED;

	ఒకవేళ (!opts->accept_pathspec && !opts->accept_ref)
		BUG("make up your mind, you need to take _something_");
	ఒకవేళ (opts->accept_pathspec && opts->accept_ref)
		parseopt_flags = PARSE_OPT_KEEP_DASHDASH;

	argc = parse_options(argc, argv, ఉపసర్గ, options,
			     usagestr, parseopt_flags);

	ఒకవేళ (opts->patch_context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	ఒకవేళ (opts->patch_interhunk_context < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	ఒకవేళ (!opts->patch_బహుళకం) {
		ఒకవేళ (opts->patch_context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--patch");
		ఒకవేళ (opts->patch_interhunk_context != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--patch");
		ఒకవేళ (!opts->auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--patch");
	}

	ఒకవేళ (opts->show_పురోగతి < 0) {
		ఒకవేళ (opts->quiet)
			opts->show_పురోగతి = 0;
		లేకపోతే
			opts->show_పురోగతి = isatty(2);
	}

	/* --conflicts implies --merge */
	ఒకవేళ (opts->విలీనం == -1)
		opts->విలీనం = opts->conflict_style >= 0;

	ఒకవేళ (opts->force) {
		opts->discard_changes = 1;
		opts->ignore_unmerged_opt = "--force";
		opts->ignore_unmerged = 1;
	}

	ఒకవేళ ((!!opts->new_branch + !!opts->new_branch_force + !!opts->new_orphan_branch) > 1)
		die(_("options '-%c', '-%c', and '%s' cannot be used together"),
			cb_option, పెద్ద_అక్షరం(cb_option), "--orphan");

	ఒకవేళ (opts->overlay_బహుళకం == 1 && opts->patch_బహుళకం)
		die(_("options '%s' and '%s' cannot be used together"), "-p", "--overlay");

	ఒకవేళ (opts->checkout_సూచీ >= 0 || opts->checkout_worktree >= 0) {
		ఒకవేళ (opts->checkout_సూచీ < 0)
			opts->checkout_సూచీ = 0;
		ఒకవేళ (opts->checkout_worktree < 0)
			opts->checkout_worktree = 0;
	} లేకపోతే {
		ఒకవేళ (opts->checkout_సూచీ < 0)
			opts->checkout_సూచీ = -opts->checkout_సూచీ - 1;
		ఒకవేళ (opts->checkout_worktree < 0)
			opts->checkout_worktree = -opts->checkout_worktree - 1;
	}
	ఒకవేళ (opts->checkout_సూచీ < 0 || opts->checkout_worktree < 0)
		BUG("these flags should be non-negative by now");
	/*
	 * convenient shortcut: "git restore --staged [--worktree]" equals
	 * "git restore --staged [--worktree] --source HEAD"
	 */
	ఒకవేళ (!opts->from_treeish && opts->checkout_సూచీ)
		opts->from_treeish = "HEAD";

	/*
	 * From here on, new_branch will contain the branch to be checked out,
	 * and new_branch_force and new_orphan_branch will tell us which one of
	 * -b/-B/-c/-C/--orphan is being used.
	 */
	ఒకవేళ (opts->new_branch_force)
		opts->new_branch = opts->new_branch_force;

	ఒకవేళ (opts->new_orphan_branch)
		opts->new_branch = opts->new_orphan_branch;

	/* --track without -c/-C/-b/-B/--orphan should DWIM */
	ఒకవేళ (opts->track != BRANCH_TRACK_UNSPECIFIED && !opts->new_branch) {
		స్థిరరాశి అక్షరం *argv0 = argv[0];
		ఒకవేళ (!argc || !strcmp(argv0, "--"))
			die(_("--track needs a branch name"));
		skip_ఉపసర్గ(argv0, "refs/", &argv0);
		skip_ఉపసర్గ(argv0, "remotes/", &argv0);
		argv0 = strchr(argv0, '/');
		ఒకవేళ (!argv0 || !argv0[1])
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
	ఒకవేళ (argc && opts->accept_ref) {
		నిర్మాణం object_id rev;
		పూర్ణాంకం dwim_ok =
			!opts->patch_బహుళకం &&
			opts->dwim_new_local_branch &&
			opts->track == BRANCH_TRACK_UNSPECIFIED &&
			!opts->new_branch;
		పూర్ణాంకం n = parse_branchname_arg(argc, argv, dwim_ok, which_command,
					     &new_branch_info, opts, &rev);
		argv += n;
		argc -= n;
	} లేకపోతే ఒకవేళ (!opts->accept_ref && opts->from_treeish) {
		నిర్మాణం object_id rev;

		ఒకవేళ (repo_get_oid_mb(the_repository, opts->from_treeish, &rev))
			die(_("could not resolve '%s'"), opts->from_treeish);

		అమరిక_new_branch_info_and_ఆధారం_వృక్షం(&new_branch_info,
						      opts, &rev,
						      opts->from_treeish);

		ఒకవేళ (!opts->ఆధారం_వృక్షం)
			die(_("reference is not a tree: %s"), opts->from_treeish);
	}

	ఒకవేళ (argc) {
		parse_pathspec(&opts->pathspec, 0,
			       opts->patch_బహుళకం ? PATHSPEC_ఉపసర్గ_ORIGIN : 0,
			       ఉపసర్గ, argv);

		ఒకవేళ (!opts->pathspec.nr)
			die(_("invalid path specification"));

		/*
		 * Try to give more helpful suggestion.
		 * new_branch && argc > 1 will be caught later.
		 */
		ఒకవేళ (opts->new_branch && argc == 1 && !new_branch_info.commit)
			die(_("'%s' is not a commit and a branch '%s' cannot be created from it"),
				argv[0], opts->new_branch);

		ఒకవేళ (opts->force_detach)
			die(_("git checkout: --detach does not take a path argument '%s'"),
			    argv[0]);
	}

	ఒకవేళ (opts->pathspec_from_file) {
		ఒకవేళ (opts->pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		ఒకవేళ (opts->force_detach)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--detach");

		ఒకవేళ (opts->patch_బహుళకం)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--patch");

		parse_pathspec_file(&opts->pathspec, 0,
				    0,
				    ఉపసర్గ, opts->pathspec_from_file, opts->pathspec_file_nul);
	} లేకపోతే ఒకవేళ (opts->pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	opts->pathspec.recursive = 1;

	ఒకవేళ (opts->pathspec.nr) {
		ఒకవేళ (1 < !!opts->writeout_stage + !!opts->force + !!opts->విలీనం)
			die(_("git checkout: --ours/--theirs, --force and --merge are incompatible when\n"
			      "checking out of the index."));
	} లేకపోతే {
		ఒకవేళ (opts->accept_pathspec && !opts->ఖాళీలు_pathspec_ok &&
		    !opts->patch_బహుళకం)	/* patch mode is special */
			die(_("you must specify path(s) to restore"));
	}

	ఒకవేళ (opts->new_branch) {
		నిర్మాణం strbuf buf = STRBUF_INIT;

		ఒకవేళ (opts->new_branch_force)
			opts->branch_exists = validate_branchname(opts->new_branch, &buf);
		లేకపోతే
			opts->branch_exists =
				validate_new_branchname(opts->new_branch, &buf, 0);
		strbuf_release(&buf);
	}

	ఒకవేళ (opts->patch_బహుళకం || opts->pathspec.nr)
		ret = checkout_paths(opts, &new_branch_info);
	లేకపోతే
		ret = checkout_branch(opts, &new_branch_info);

	branch_info_release(&new_branch_info);
	clear_pathspec(&opts->pathspec);
	విడుదల(opts->pathspec_from_file);
	విడుదల(options);

	తిరిగి ret;
}

పూర్ణాంకం cmd_checkout(పూర్ణాంకం argc,
		 స్థిరరాశి అక్షరం **argv,
		 స్థిరరాశి అక్షరం *ఉపసర్గ,
		 నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం checkout_opts opts = CHECKOUT_OPTS_INIT;
	నిర్మాణం option *options;
	నిర్మాణం option checkout_options[] = {
		OPT_STRING('b', ఖాళీ, &opts.new_branch, N_("branch"),
			   N_("create and checkout a new branch")),
		OPT_STRING('B', ఖాళీ, &opts.new_branch_force, N_("branch"),
			   N_("create/reset and checkout a branch")),
		OPT_BOOL('l', ఖాళీ, &opts.new_branch_log, N_("create reflog for new branch")),
		OPT_BOOL(0, "guess", &opts.dwim_new_local_branch,
			 N_("second guess 'git checkout <no-such-branch>' (default)")),
		OPT_BOOL(0, "overlay", &opts.overlay_బహుళకం, N_("use overlay mode (default)")),
		OPT_BOOL(0, "auto-advance", &opts.auto_advance,
			 N_("auto advance to the next file when selecting hunks interactively")),
		OPT_చివర()
	};

	opts.dwim_new_local_branch = 1;
	opts.switch_branch_doing_nothing_is_ok = 1;
	opts.only_విలీనం_on_switching_branches = 0;
	opts.accept_ref = 1;
	opts.accept_pathspec = 1;
	opts.implicit_detach = 1;
	opts.can_switch_when_లోపల_పురోగతి = 1;
	opts.orphan_from_ఖాళీలు_వృక్షం = 0;
	opts.ఖాళీలు_pathspec_ok = 1;
	opts.overlay_బహుళకం = -1;
	opts.checkout_సూచీ = -2;    /* default on */
	opts.checkout_worktree = -2; /* default on */

	ఒకవేళ (argc == 3 && !strcmp(argv[1], "-b")) {
		/*
		 * User ran 'git checkout -b <branch>' and expects
		 * the same behavior as 'git switch -c <branch>'.
		 */
		opts.switch_branch_doing_nothing_is_ok = 0;
		opts.only_విలీనం_on_switching_branches = 1;
	}

	options = parse_options_dup(checkout_options);
	options = కూడు_common_options(&opts, options);
	options = కూడు_common_switch_branch_options(&opts, options);
	options = కూడు_checkout_మార్గం_options(&opts, options);

	తిరిగి checkout_main(argc, argv, ఉపసర్గ, &opts, options,
			     CHECKOUT_CHECKOUT);
}

పూర్ణాంకం cmd_switch(పూర్ణాంకం argc,
	       స్థిరరాశి అక్షరం **argv,
	       స్థిరరాశి అక్షరం *ఉపసర్గ,
	       నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం checkout_opts opts = CHECKOUT_OPTS_INIT;
	నిర్మాణం option *options = ఖాళీ;
	నిర్మాణం option switch_options[] = {
		OPT_STRING('c', "create", &opts.new_branch, N_("branch"),
			   N_("create and switch to a new branch")),
		OPT_STRING('C', "force-create", &opts.new_branch_force, N_("branch"),
			   N_("create/reset and switch to a branch")),
		OPT_BOOL(0, "guess", &opts.dwim_new_local_branch,
			 N_("second guess 'git switch <no-such-branch>'")),
		OPT_BOOL(0, "discard-changes", &opts.discard_changes,
			 N_("throw away local modifications")),
		OPT_చివర()
	};

	opts.dwim_new_local_branch = 1;
	opts.accept_ref = 1;
	opts.accept_pathspec = 0;
	opts.switch_branch_doing_nothing_is_ok = 0;
	opts.only_విలీనం_on_switching_branches = 1;
	opts.implicit_detach = 0;
	opts.can_switch_when_లోపల_పురోగతి = 0;
	opts.orphan_from_ఖాళీలు_వృక్షం = 1;
	opts.overlay_బహుళకం = -1;

	options = parse_options_dup(switch_options);
	options = కూడు_common_options(&opts, options);
	options = కూడు_common_switch_branch_options(&opts, options);

	cb_option = 'c';

	తిరిగి checkout_main(argc, argv, ఉపసర్గ, &opts, options,
			     CHECKOUT_SWITCH);
}

పూర్ణాంకం cmd_restore(పూర్ణాంకం argc,
		స్థిరరాశి అక్షరం **argv,
		స్థిరరాశి అక్షరం *ఉపసర్గ,
		నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం checkout_opts opts = CHECKOUT_OPTS_INIT;
	నిర్మాణం option *options;
	నిర్మాణం option restore_options[] = {
		OPT_STRING('s', "source", &opts.from_treeish, "<tree-ish>",
			   N_("which tree-ish to checkout from")),
		OPT_BOOL('S', "staged", &opts.checkout_సూచీ,
			   N_("restore the index")),
		OPT_BOOL('W', "worktree", &opts.checkout_worktree,
			   N_("restore the working tree (default)")),
		OPT_BOOL(0, "ignore-unmerged", &opts.ignore_unmerged,
			 N_("ignore unmerged entries")),
		OPT_BOOL(0, "overlay", &opts.overlay_బహుళకం, N_("use overlay mode")),
		OPT_చివర()
	};

	opts.accept_ref = 0;
	opts.accept_pathspec = 1;
	opts.ఖాళీలు_pathspec_ok = 0;
	opts.overlay_బహుళకం = 0;
	opts.checkout_సూచీ = -1;    /* default off */
	opts.checkout_worktree = -2; /* default on */
	opts.ignore_unmerged_opt = "--ignore-unmerged";

	options = parse_options_dup(restore_options);
	options = కూడు_common_options(&opts, options);
	options = కూడు_checkout_మార్గం_options(&opts, options);

	తిరిగి checkout_main(argc, argv, ఉపసర్గ, &opts, options,
			     CHECKOUT_RESTORE);
}
