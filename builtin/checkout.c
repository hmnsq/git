#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "advice.h"
#અન્તર્ભાવ "branch.h"
#અન્તર્ભાવ "cache-tree.h"
#અન્તર્ભાવ "checkout.h"
#અન્તર્ભાવ "commit.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "diff.h"
#અન્તર્ભાવ "dir.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "hex.h"
#અન્તર્ભાવ "hook.h"
#અન્તર્ભાવ "merge-ll.h"
#અન્તર્ભાવ "lockfile.h"
#અન્તર્ભાવ "mem-pool.h"
#અન્તર્ભાવ "object-file.h"
#અન્તર્ભાવ "object-name.h"
#અન્તર્ભાવ "odb.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "path.h"
#અન્તર્ભાવ "preload-index.h"
#અન્તર્ભાવ "read-cache.h"
#અન્તર્ભાવ "refs.h"
#અન્તર્ભાવ "remote.h"
#અન્તર્ભાવ "repo-settings.h"
#અન્તર્ભાવ "resolve-undo.h"
#અન્તર્ભાવ "revision.h"
#અન્તર્ભાવ "sequencer.h"
#અન્તર્ભાવ "setup.h"
#અન્તર્ભાવ "sparse-index.h"
#અન્તર્ભાવ "strvec.h"
#અન્તર્ભાવ "submodule.h"
#અન્તર્ભાવ "symlinks.h"
#અન્તર્ભાવ "trace2.h"
#અન્તર્ભાવ "tree.h"
#અન્તર્ભાવ "tree-walk.h"
#અન્તર્ભાવ "unpack-trees.h"
#અન્તર્ભાવ "wt-status.h"
#અન્તર્ભાવ "xdiff-interface.h"
#અન્તર્ભાવ "entry.h"
#અન્તર્ભાવ "parallel-checkout.h"
#અન્તર્ભાવ "add-interactive.h"

સંરચના checkout_opts {
	પૂર્ણાંક patch_mode;
	પૂર્ણાંક patch_context;
	પૂર્ણાંક patch_interhunk_context;
	પૂર્ણાંક auto_advance;
	પૂર્ણાંક quiet;
	પૂર્ણાંક merge;
	પૂર્ણાંક force;
	પૂર્ણાંક force_detach;
	પૂર્ણાંક implicit_detach;
	પૂર્ણાંક writeout_stage;
	પૂર્ણાંક overwrite_ignore;
	પૂર્ણાંક ignore_skipworktree;
	પૂર્ણાંક ignore_other_worktrees;
	પૂર્ણાંક show_progress;
	પૂર્ણાંક ગણતરી_checkout_paths;
	પૂર્ણાંક overlay_mode;
	પૂર્ણાંક dwim_new_local_branch;
	પૂર્ણાંક discard_changes;
	પૂર્ણાંક accept_ref;
	પૂર્ણાંક accept_pathspec;
	પૂર્ણાંક switch_branch_doing_nothing_is_ok;
	પૂર્ણાંક only_merge_on_switching_branches;
	પૂર્ણાંક can_switch_when_in_progress;
	પૂર્ણાંક orphan_from_empty_વૃક્ષ;
	પૂર્ણાંક empty_pathspec_ok;
	પૂર્ણાંક checkout_index;
	પૂર્ણાંક checkout_worktree;
	અચલ અક્ષર *ignore_unmerged_opt;
	પૂર્ણાંક ignore_unmerged;
	પૂર્ણાંક pathspec_file_nul;
	અક્ષર *pathspec_from_file;

	અચલ અક્ષર *new_branch;
	અચલ અક્ષર *new_branch_force;
	અચલ અક્ષર *new_orphan_branch;
	પૂર્ણાંક new_branch_log;
	ગણના branch_track track;
	સંરચના diff_options diff_options;
	પૂર્ણાંક conflict_style;

	પૂર્ણાંક branch_exists;
	અચલ અક્ષર *prefix;
	સંરચના pathspec pathspec;
	અચલ અક્ષર *from_treeish;
	સંરચના વૃક્ષ *source_વૃક્ષ;
};

#define CHECKOUT_OPTS_INIT { \
	.conflict_style = -1, \
	.merge = -1, \
	.patch_context = -1, \
	.patch_interhunk_context = -1, \
	.auto_advance = 1, \
}

#define MERGE_WORKING_વૃક્ષ_UNPACK_FAILED (-2)

સંરચના branch_info {
	અક્ષર *name; /* The short name used */
	અક્ષર *path; /* The full name of a real branch */
	સંરચના commit *commit; /* The named commit */
	અક્ષર *refname; /* The full name of the ref being checked out. */
	સંરચના object_id oid; /* The object ID of the commit being checked out. */
	/*
	 * if not null the branch is detached because it's already
	 * checked out in this checkout
	 */
	અક્ષર *checkout;
};

સ્થિર શૂન્ય branch_info_release(સંરચના branch_info *info)
{
	મુક્ત(info->name);
	મુક્ત(info->path);
	મુક્ત(info->refname);
	મુક્ત(info->checkout);
}

સ્થિર પૂર્ણાંક post_checkout_hook(સંરચના commit *old_commit, સંરચના commit *new_commit,
			      પૂર્ણાંક changed)
{
	સંરચના run_hooks_opt opt = RUN_HOOKS_OPT_INIT_FORCE_SERIAL;

	/*
	 * "new_commit" can be NULL when checking out from the index before
	 * a commit exists.
	 */
	strvec_pushl(&opt.args,
		     oid_to_hex(old_commit ? &old_commit->object.oid : null_oid(the_hash_algo)),
		     oid_to_hex(new_commit ? &new_commit->object.oid : null_oid(the_hash_algo)),
		     changed ? "1" : "0",
		     ખાલી);

	પાછા run_hooks_opt(the_repository, "post-checkout", &opt);
}

/*
 * Handle a tree object and determine if we need to recurse into the
 * tree (READ_TREE_RECURSIVE) or skip it (0).
 */
સ્થિર પૂર્ણાંક try_update_sparse_directory(અચલ સંરચના object_id *oid,
				       સંરચના strbuf *base,
				       અચલ અક્ષર *pathname,
				       પૂર્ણાંક overlay_mode)
{
	સંરચના strbuf dirpath = STRBUF_INIT;
	સંરચના cache_entry *old;
	પૂર્ણાંક pos, result = READ_વૃક્ષ_RECURSIVE;

	જો (!the_repository->index->sparse_index)
		પાછા result;

	strbuf_addbuf(&dirpath, base);
	strbuf_addstr(&dirpath, pathname);
	strbuf_addch(&dirpath, '/');

	pos = index_name_pos_sparse(the_repository->index,
				    dirpath.buf, dirpath.len);
	જો (pos < 0)
		જાઓ cleanup;

	old = the_repository->index->cache[pos];
	જો (!S_ISSPARSEDIR(old->ce_mode))
		જાઓ cleanup;

	જો (oideq(oid, &old->oid)) {
		/* Tree content already matches; no need to descend. */
		result = 0;
	} નહીંતર જો (!overlay_mode) {
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
	પાછા result;
}

સ્થિર પૂર્ણાંક update_some(અચલ સંરચના object_id *oid, સંરચના strbuf *base,
		       અચલ અક્ષર *pathname, unsigned mode, શૂન્ય *context)
{
	પૂર્ણાંક len;
	સંરચના cache_entry *ce;
	પૂર્ણાંક pos;
	પૂર્ણાંક overlay_mode = context ? *((પૂર્ણાંક *)context) : 1;

	જો (S_ISDIR(mode))
		પાછા try_update_sparse_directory(oid, base, pathname,
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
	જો (pos >= 0) {
		સંરચના cache_entry *old = the_repository->index->cache[pos];
		જો (ce->ce_mode == old->ce_mode &&
		    !ce_intent_to_add(old) &&
		    oideq(&ce->oid, &old->oid)) {
			old->ce_flags |= CE_UPDATE;
			discard_cache_entry(ce);
			પાછા 0;
		}
	}

	add_index_entry(the_repository->index, ce,
			ADD_CACHE_OK_TO_ADD | ADD_CACHE_OK_TO_REPLACE);
	પાછા 0;
}

સ્થિર પૂર્ણાંક read_વૃક્ષ_some(સંરચના વૃક્ષ *વૃક્ષ, અચલ સંરચના pathspec *pathspec,
			  પૂર્ણાંક overlay_mode)
{
	read_વૃક્ષ(the_repository, વૃક્ષ,
		  pathspec, update_some, &overlay_mode);

	/* update the index with the given tree's info
	 * for all args, expanding wildcards, and exit
	 * with any non-zero return code.
	 */
	પાછા 0;
}

સ્થિર પૂર્ણાંક skip_same_name(અચલ સંરચના cache_entry *ce, પૂર્ણાંક pos)
{
	જ્યારે (++pos < the_repository->index->cache_nr &&
	       !strcmp(the_repository->index->cache[pos]->name, ce->name))
		; /* skip */
	પાછા pos;
}

સ્થિર પૂર્ણાંક check_stage(પૂર્ણાંક stage, અચલ સંરચના cache_entry *ce, પૂર્ણાંક pos,
		       પૂર્ણાંક overlay_mode)
{
	જ્યારે (pos < the_repository->index->cache_nr &&
	       !strcmp(the_repository->index->cache[pos]->name, ce->name)) {
		જો (ce_stage(the_repository->index->cache[pos]) == stage)
			પાછા 0;
		pos++;
	}
	જો (!overlay_mode)
		પાછા 0;
	જો (stage == 2)
		પાછા error(_("path '%s' does not have our version"), ce->name);
	નહીંતર
		પાછા error(_("path '%s' does not have their version"), ce->name);
}

સ્થિર પૂર્ણાંક check_stages(unsigned stages, અચલ સંરચના cache_entry *ce, પૂર્ણાંક pos)
{
	unsigned seen = 0;
	અચલ અક્ષર *name = ce->name;

	જ્યારે (pos < the_repository->index->cache_nr) {
		ce = the_repository->index->cache[pos];
		જો (strcmp(name, ce->name))
			તોડો;
		seen |= (1 << ce_stage(ce));
		pos++;
	}
	જો ((stages & seen) != stages)
		પાછા error(_("path '%s' does not have all necessary versions"),
			     name);
	પાછા 0;
}

સ્થિર પૂર્ણાંક checkout_stage(પૂર્ણાંક stage, અચલ સંરચના cache_entry *ce, પૂર્ણાંક pos,
			  અચલ સંરચના checkout *state, પૂર્ણાંક *nr_checkouts,
			  પૂર્ણાંક overlay_mode)
{
	જ્યારે (pos < the_repository->index->cache_nr &&
	       !strcmp(the_repository->index->cache[pos]->name, ce->name)) {
		જો (ce_stage(the_repository->index->cache[pos]) == stage)
			પાછા checkout_entry(the_repository->index->cache[pos], state,
					      ખાલી, nr_checkouts);
		pos++;
	}
	જો (!overlay_mode) {
		unlink_entry(ce, ખાલી);
		પાછા 0;
	}
	જો (stage == 2)
		પાછા error(_("path '%s' does not have our version"), ce->name);
	નહીંતર
		પાછા error(_("path '%s' does not have their version"), ce->name);
}

સ્થિર પૂર્ણાંક checkout_merged(પૂર્ણાંક pos, અચલ સંરચના checkout *state,
			   પૂર્ણાંક *nr_checkouts, સંરચના mem_pool *ce_mem_pool,
			   પૂર્ણાંક conflict_style)
{
	સંરચના cache_entry *ce = the_repository->index->cache[pos];
	અચલ અક્ષર *path = ce->name;
	mmfile_t ancestor, ours, theirs;
	ગણના ll_merge_result merge_status;
	પૂર્ણાંક status;
	સંરચના object_id oid;
	mmfile_t result_buf;
	સંરચના object_id threeway[3];
	unsigned mode = 0;
	સંરચના ll_merge_options ll_opts = LL_MERGE_OPTIONS_INIT;
	પૂર્ણાંક renormalize = 0;

	memset(threeway, 0, કદ(threeway));
	જ્યારે (pos < the_repository->index->cache_nr) {
		પૂર્ણાંક stage;
		stage = ce_stage(ce);
		જો (!stage || strcmp(path, ce->name))
			તોડો;
		oidcpy(&threeway[stage - 1], &ce->oid);
		જો (stage == 2)
			mode = create_ce_mode(ce->ce_mode);
		pos++;
		ce = the_repository->index->cache[pos];
	}
	જો (is_null_oid(&threeway[1]) || is_null_oid(&threeway[2]))
		પાછા error(_("path '%s' does not have necessary versions"), path);

	read_mmblob(&ancestor, the_repository->objects, &threeway[0]);
	read_mmblob(&ours, the_repository->objects, &threeway[1]);
	read_mmblob(&theirs, the_repository->objects, &threeway[2]);

	repo_config_get_bool(the_repository, "merge.renormalize", &renormalize);
	ll_opts.renormalize = renormalize;
	ll_opts.conflict_style = conflict_style;
	merge_status = ll_merge(&result_buf, path, &ancestor, "base",
				&ours, "ours", &theirs, "theirs",
				state->istate, &ll_opts);
	મુક્ત(ancestor.ptr);
	મુક્ત(ours.ptr);
	મુક્ત(theirs.ptr);
	જો (merge_status == LL_MERGE_દ્વિઅંકી_CONFLICT)
		warning("Cannot merge binary files: %s (%s vs. %s)",
			path, "ours", "theirs");
	જો (merge_status < 0 || !result_buf.ptr) {
		મુક્ત(result_buf.ptr);
		પાછા error(_("path '%s': cannot merge"), path);
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
	જો (odb_write_object(the_repository->objects, result_buf.ptr, result_buf.size, OBJ_BLOB, &oid))
		die(_("Unable to add merge result for '%s'"), path);
	મુક્ત(result_buf.ptr);
	ce = make_transient_cache_entry(mode, &oid, path, 2, ce_mem_pool);
	જો (!ce)
		die(_("make_cache_entry failed for path '%s'"), path);
	status = checkout_entry(ce, state, ખાલી, nr_checkouts);
	પાછા status;
}

સ્થિર શૂન્ય mark_ce_for_checkout_overlay(સંરચના cache_entry *ce,
					 અક્ષર *ps_matched,
					 અચલ સંરચના checkout_opts *opts)
{
	ce->ce_flags &= ~CE_MATCHED;
	જો (!opts->ignore_skipworktree && ce_skip_worktree(ce))
		પાછા;
	જો (opts->source_વૃક્ષ && !(ce->ce_flags & CE_UPDATE))
		/*
		 * "git checkout tree-ish -- path", but this entry
		 * is in the original index but is not in tree-ish
		 * or does not match the pathspec; it will not be
		 * checked out to the working tree.  We will not do
		 * anything to this entry at all.
		 */
		પાછા;
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
	જો (ce_path_match(the_repository->index, ce, &opts->pathspec, ps_matched))
		ce->ce_flags |= CE_MATCHED;
}

સ્થિર શૂન્ય mark_ce_for_checkout_no_overlay(સંરચના cache_entry *ce,
					    અક્ષર *ps_matched,
					    અચલ સંરચના checkout_opts *opts)
{
	ce->ce_flags &= ~CE_MATCHED;
	જો (!opts->ignore_skipworktree && ce_skip_worktree(ce))
		પાછા;
	જો (ce_path_match(the_repository->index, ce, &opts->pathspec, ps_matched)) {
		ce->ce_flags |= CE_MATCHED;
		જો (opts->source_વૃક્ષ && !(ce->ce_flags & CE_UPDATE))
			/*
			 * In overlay mode, but the path is not in
			 * tree-ish, which means we should remove it
			 * from the index and the working tree.
			 */
			ce->ce_flags |= CE_REMOVE | CE_WT_REMOVE;
	}
}

સ્થિર પૂર્ણાંક checkout_worktree(અચલ સંરચના checkout_opts *opts,
			     અચલ સંરચના branch_info *info)
{
	સંરચના checkout state = CHECKOUT_INIT;
	પૂર્ણાંક nr_checkouts = 0, nr_unmerged = 0;
	પૂર્ણાંક errs = 0;
	પૂર્ણાંક pos;
	પૂર્ણાંક pc_workers, pc_threshold;
	સંરચના mem_pool ce_mem_pool;

	state.force = 1;
	state.refresh_cache = 1;
	state.istate = the_repository->index;

	mem_pool_init(&ce_mem_pool, 0);
	get_parallel_checkout_configs(&pc_workers, &pc_threshold);
	init_checkout_metadata(&state.meta, info->refname,
			       info->commit ? &info->commit->object.oid : &info->oid,
			       ખાલી);

	enable_delayed_checkout(&state);

	જો (pc_workers > 1)
		init_parallel_checkout();

	માટે (pos = 0; pos < the_repository->index->cache_nr; pos++) {
		સંરચના cache_entry *ce = the_repository->index->cache[pos];
		જો (ce->ce_flags & CE_MATCHED) {
			જો (!ce_stage(ce)) {
				errs |= checkout_entry(ce, &state,
						       ખાલી, &nr_checkouts);
				ચાલુ;
			}
			જો (opts->writeout_stage)
				errs |= checkout_stage(opts->writeout_stage,
						       ce, pos,
						       &state,
						       &nr_checkouts, opts->overlay_mode);
			નહીંતર જો (opts->merge)
				errs |= checkout_merged(pos, &state,
							&nr_unmerged,
							&ce_mem_pool,
							opts->conflict_style);
			pos = skip_same_name(ce, pos) - 1;
		}
	}
	જો (pc_workers > 1)
		errs |= run_parallel_checkout(&state, pc_workers, pc_threshold,
					      ખાલી, ખાલી);
	mem_pool_discard(&ce_mem_pool, should_validate_cache_entries());
	remove_marked_cache_entries(the_repository->index, 1);
	remove_scheduled_dirs();
	errs |= finish_delayed_checkout(&state, opts->show_progress);

	જો (opts->ગણતરી_checkout_paths) {
		જો (nr_unmerged)
			fprintf_ln(stderr, Q_("Recreated %d merge conflict",
					      "Recreated %d merge conflicts",
					      nr_unmerged),
				   nr_unmerged);
		જો (opts->source_વૃક્ષ)
			fprintf_ln(stderr, Q_("Updated %d path from %s",
					      "Updated %d paths from %s",
					      nr_checkouts),
				   nr_checkouts,
				   repo_find_unique_abbrev(the_repository, &opts->source_વૃક્ષ->object.oid,
							   DEFAULT_ABBREV));
		નહીંતર જો (!nr_unmerged || nr_checkouts)
			fprintf_ln(stderr, Q_("Updated %d path from the index",
					      "Updated %d paths from the index",
					      nr_checkouts),
				   nr_checkouts);
	}

	પાછા errs;
}

સ્થિર પૂર્ણાંક checkout_paths(અચલ સંરચના checkout_opts *opts,
			  અચલ સંરચના branch_info *new_branch_info)
{
	પૂર્ણાંક pos;
	સ્થિર અક્ષર *ps_matched;
	સંરચના object_id rev;
	સંરચના commit *head;
	પૂર્ણાંક errs = 0;
	સંરચના lock_file lock_file = LOCK_INIT;
	પૂર્ણાંક checkout_index;

	trace2_cmd_mode(opts->patch_mode ? "patch" : "path");

	જો (opts->track != BRANCH_TRACK_UNSPECIFIED)
		die(_("'%s' cannot be used with updating paths"), "--track");

	જો (opts->new_branch_log)
		die(_("'%s' cannot be used with updating paths"), "-l");

	જો (opts->ignore_unmerged && opts->patch_mode)
		die(_("'%s' cannot be used with updating paths"),
		    opts->ignore_unmerged_opt);

	જો (opts->force_detach)
		die(_("'%s' cannot be used with updating paths"), "--detach");

	જો (opts->merge && opts->patch_mode)
		die(_("options '%s' and '%s' cannot be used together"), "--merge", "--patch");

	જો (opts->ignore_unmerged && opts->merge)
		die(_("options '%s' and '%s' cannot be used together"),
		    opts->ignore_unmerged_opt, "-m");

	જો (opts->new_branch)
		die(_("Cannot update paths and switch to branch '%s' at the same time."),
		    opts->new_branch);

	જો (!opts->checkout_worktree && !opts->checkout_index)
		die(_("neither '%s' or '%s' is specified"),
		    "--staged", "--worktree");

	જો (!opts->checkout_worktree && !opts->from_treeish)
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
	જો (!opts->accept_ref && opts->checkout_index) {
		જો (opts->writeout_stage)
			die(_("'%s' or '%s' cannot be used with %s"),
			    "--ours", "--theirs", "--staged");

		જો (opts->merge)
			die(_("'%s' or '%s' cannot be used with %s"),
			    "--merge", "--conflict", "--staged");
	}

	/*
	 * recreating unmerged index entries and writing out data from
	 * unmerged index entries would make no sense when checking out
	 * of a tree-ish.
	 */
	જો ((opts->merge || opts->writeout_stage) && opts->source_વૃક્ષ)
		die(_("'%s', '%s', or '%s' cannot be used when checking out of a tree"),
		    "--merge", "--ours", "--theirs");

	જો (opts->patch_mode) {
		ગણના add_p_mode patch_mode;
		સંરચના interactive_options interactive_opts = {
			.context = opts->patch_context,
			.interhunkcontext = opts->patch_interhunk_context,
			.auto_advance = opts->auto_advance
		};
		અચલ અક્ષર *rev = new_branch_info->name;
		અક્ષર rev_oid[GIT_MAX_HEXSZ + 1];

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
		જો (rev && new_branch_info->commit && strcmp(rev, "HEAD"))
			rev = oid_to_hex_r(rev_oid, &new_branch_info->commit->object.oid);

		જો (opts->checkout_index && opts->checkout_worktree)
			patch_mode = ADD_P_CHECKOUT;
		નહીંતર જો (opts->checkout_index && !opts->checkout_worktree)
			patch_mode = ADD_P_RESET;
		નહીંતર જો (!opts->checkout_index && opts->checkout_worktree)
			patch_mode = ADD_P_WORKTREE;
		નહીંતર
			BUG("either flag must have been set, worktree=%d, index=%d",
			    opts->checkout_worktree, opts->checkout_index);
		પાછા !!run_add_p(the_repository, patch_mode, &interactive_opts,
				   rev, &opts->pathspec, 0);
	}

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);
	જો (repo_read_index_preload(the_repository, &opts->pathspec, 0) < 0)
		પાછા error(_("index file corrupt"));

	જો (opts->source_વૃક્ષ)
		read_વૃક્ષ_some(opts->source_વૃક્ષ, &opts->pathspec,
			       opts->overlay_mode);
	જો (opts->merge)
		unmerge_index(the_repository->index, &opts->pathspec, CE_MATCHED);

	ps_matched = xcalloc(opts->pathspec.nr, 1);

	/*
	 * Make sure all pathspecs participated in locating the paths
	 * to be checked out.
	 */
	માટે (pos = 0; pos < the_repository->index->cache_nr; pos++)
		જો (opts->overlay_mode)
			mark_ce_for_checkout_overlay(the_repository->index->cache[pos],
						     ps_matched,
						     opts);
		નહીંતર
			mark_ce_for_checkout_no_overlay(the_repository->index->cache[pos],
							ps_matched,
							opts);

	જો (report_path_error(ps_matched, &opts->pathspec)) {
		મુક્ત(ps_matched);
		પાછા 1;
	}
	મુક્ત(ps_matched);

	/* Any unmerged paths? */
	માટે (pos = 0; pos < the_repository->index->cache_nr; pos++) {
		અચલ સંરચના cache_entry *ce = the_repository->index->cache[pos];
		જો (ce->ce_flags & CE_MATCHED) {
			જો (!ce_stage(ce))
				ચાલુ;
			જો (opts->ignore_unmerged) {
				જો (!opts->quiet)
					warning(_("path '%s' is unmerged"), ce->name);
			} નહીંતર જો (opts->writeout_stage) {
				errs |= check_stage(opts->writeout_stage, ce, pos, opts->overlay_mode);
			} નહીંતર જો (opts->merge) {
				errs |= check_stages((1<<2) | (1<<3), ce, pos);
			} નહીંતર {
				errs = 1;
				error(_("path '%s' is unmerged"), ce->name);
			}
			pos = skip_same_name(ce, pos) - 1;
		}
	}
	જો (errs)
		પાછા 1;

	/* Now we are committed to check them out */
	જો (opts->checkout_worktree)
		errs |= checkout_worktree(opts, new_branch_info);
	નહીંતર
		remove_marked_cache_entries(the_repository->index, 1);

	/*
	 * Allow updating the index when checking out from the index.
	 * This is to save new stat info.
	 */
	જો (opts->checkout_worktree && !opts->checkout_index && !opts->source_વૃક્ષ)
		checkout_index = 1;
	નહીંતર
		checkout_index = opts->checkout_index;

	જો (checkout_index) {
		જો (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
			die(_("unable to write new index file"));
	} નહીંતર {
		/*
		 * NEEDSWORK: if --worktree is not specified, we
		 * should save stat info of checked out files in the
		 * index to avoid the next (potentially costly)
		 * refresh. But it's a bit tricker to do...
		 */
		rollback_lock_file(&lock_file);
	}

	refs_read_ref_full(get_main_ref_store(the_repository), "HEAD", 0,
			   &rev, ખાલી);
	head = lookup_commit_reference_gently(the_repository, &rev, 1);

	errs |= post_checkout_hook(head, head, 0);
	પાછા errs;
}

સ્થિર શૂન્ય show_local_changes(સંરચના object *head,
			       અચલ સંરચના diff_options *opts)
{
	સંરચના rev_info rev;
	/* I think we want full paths, even if we're in a subdirectory. */
	repo_init_revisions(the_repository, &rev, ખાલી);
	rev.diffopt.flags = opts->flags;
	rev.diffopt.output_format |= DIFF_FORMAT_NAME_STATUS;
	rev.diffopt.flags.recursive = 1;
	diff_setup_done(&rev.diffopt);
	add_pending_object(&rev, head, ખાલી);
	run_diff_index(&rev, 0);
	release_revisions(&rev);
}

સ્થિર શૂન્ય describe_detached_head(અચલ અક્ષર *સંદેશ, સંરચના commit *commit)
{
	સંરચના strbuf sb = STRBUF_INIT;

	જો (!repo_parse_commit(the_repository, commit))
		pp_commit_easy(CMIT_FMT_ONELINE, commit, &sb);
	જો (print_sha1_ellipsis()) {
		fprintf(stderr, "%s %s... %s\n", સંદેશ,
			repo_find_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV),
			sb.buf);
	} નહીંતર {
		fprintf(stderr, "%s %s %s\n", સંદેશ,
			repo_find_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV),
			sb.buf);
	}
	strbuf_release(&sb);
}

સ્થિર પૂર્ણાંક reset_વૃક્ષ(સંરચના વૃક્ષ *વૃક્ષ, અચલ સંરચના checkout_opts *o,
		      પૂર્ણાંક worktree, પૂર્ણાંક *writeout_error,
		      સંરચના branch_info *info)
{
	સંરચના unpack_trees_options opts;
	સંરચના વૃક્ષ_desc વૃક્ષ_desc;

	memset(&opts, 0, કદ(opts));
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
			       ખાલી);
	જો (repo_parse_વૃક્ષ(the_repository, વૃક્ષ) < 0)
		પાછા 128;
	init_વૃક્ષ_desc(&વૃક્ષ_desc, &વૃક્ષ->object.oid, વૃક્ષ->buffer, વૃક્ષ->size);
	પસંદગી (unpack_trees(1, &વૃક્ષ_desc, &opts)) {
	સ્થિતિ -2:
		*writeout_error = 1;
		/*
		 * We return 0 nevertheless, as the index is all right
		 * and more importantly we have made best efforts to
		 * update paths in the work tree, and we cannot revert
		 * them.
		 */
		/* fallthrough */
	સ્થિતિ 0:
		પાછા 0;
	મૂળભૂત:
		પાછા 128;
	}
}

સ્થિર શૂન્ય setup_branch_path(સંરચના branch_info *branch)
{
	સંરચના strbuf buf = STRBUF_INIT;

	/*
	 * If this is a ref, resolve it; otherwise, look up the OID for our
	 * expression.  Failure here is okay.
	 */
	જો (!repo_dwim_ref(the_repository, branch->name, strlen(branch->name),
			   &branch->oid, &branch->refname, 0))
		repo_get_oid_committish(the_repository, branch->name, &branch->oid);

	copy_branchname(the_repository, &buf, branch->name, INTERPRET_BRANCH_LOCAL);
	જો (strcmp(buf.buf, branch->name)) {
		મુક્ત(branch->name);
		branch->name = xstrdup(buf.buf);
	}
	strbuf_splice(&buf, 0, 0, "refs/heads/", 11);
	મુક્ત(branch->path);
	branch->path = strbuf_detach(&buf, ખાલી);
}

સ્થિર શૂન્ય init_topts(સંરચના unpack_trees_options *topts,
		       પૂર્ણાંક show_progress, પૂર્ણાંક overwrite_ignore,
		       bool quiet)
{
	memset(topts, 0, કદ(*topts));
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

સ્થિર પૂર્ણાંક merge_working_વૃક્ષ(અચલ સંરચના checkout_opts *opts,
			      સંરચના branch_info *old_branch_info,
			      સંરચના branch_info *new_branch_info,
			      bool quiet,
			      પૂર્ણાંક *writeout_error)
{
	પૂર્ણાંક ret;
	સંરચના lock_file lock_file = LOCK_INIT;
	સંરચના વૃક્ષ *new_વૃક્ષ;

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);
	જો (repo_read_index_preload(the_repository, ખાલી, 0) < 0) {
		rollback_lock_file(&lock_file);
		પાછા error(_("index file corrupt"));
	}

	resolve_undo_clear_index(the_repository->index);
	જો (opts->new_orphan_branch && opts->orphan_from_empty_વૃક્ષ) {
		જો (new_branch_info->commit)
			BUG("'switch --orphan' should never accept a commit as starting point");
		new_વૃક્ષ = repo_parse_વૃક્ષ_indirect(the_repository,
						    the_hash_algo->empty_વૃક્ષ);
		જો (!new_વૃક્ષ)
			BUG("unable to read empty tree");
	} નહીંતર {
		new_વૃક્ષ = repo_get_commit_વૃક્ષ(the_repository,
						new_branch_info->commit);
		જો (!new_વૃક્ષ) {
			rollback_lock_file(&lock_file);
			પાછા error(_("unable to read tree (%s)"),
				     oid_to_hex(&new_branch_info->commit->object.oid));
		}
	}
	જો (opts->discard_changes) {
		ret = reset_વૃક્ષ(new_વૃક્ષ, opts, 1, writeout_error, new_branch_info);
		જો (ret) {
			rollback_lock_file(&lock_file);
			પાછા ret;
		}
	} નહીંતર {
		સંરચના વૃક્ષ_desc trees[2];
		સંરચના વૃક્ષ *વૃક્ષ;
		સંરચના unpack_trees_options topts;
		અચલ સંરચના object_id *old_commit_oid;

		refresh_index(the_repository->index, REFRESH_QUIET, ખાલી, ખાલી, ખાલી);

		જો (unmerged_index(the_repository->index)) {
			rollback_lock_file(&lock_file);
			error(_("you need to resolve your current index first"));
			પાછા 1;
		}

		/* 2-way merge to the new branch */
		init_topts(&topts, opts->show_progress,
			   opts->overwrite_ignore, quiet);
		init_checkout_metadata(&topts.meta, new_branch_info->refname,
				       new_branch_info->commit ?
				       &new_branch_info->commit->object.oid :
				       &new_branch_info->oid, ખાલી);

		old_commit_oid = old_branch_info->commit ?
			&old_branch_info->commit->object.oid :
			the_hash_algo->empty_વૃક્ષ;
		વૃક્ષ = repo_parse_વૃક્ષ_indirect(the_repository,
						old_commit_oid);
		જો (!વૃક્ષ)
			die(_("unable to parse commit %s"),
				oid_to_hex(old_commit_oid));

		init_વૃક્ષ_desc(&trees[0], &વૃક્ષ->object.oid,
			       વૃક્ષ->buffer, વૃક્ષ->size);
		જો (repo_parse_વૃક્ષ(the_repository, new_વૃક્ષ) < 0)
			die(ખાલી);
		વૃક્ષ = new_વૃક્ષ;
		init_વૃક્ષ_desc(&trees[1], &વૃક્ષ->object.oid,
			       વૃક્ષ->buffer, વૃક્ષ->size);

		ret = unpack_trees(2, trees, &topts);
		clear_unpack_trees_porcelain(&topts);
		જો (ret == -1) {
			rollback_lock_file(&lock_file);
			પાછા MERGE_WORKING_વૃક્ષ_UNPACK_FAILED;
		}
	}

	જો (!cache_વૃક્ષ_fully_valid(the_repository->index))
		cache_વૃક્ષ_update(the_repository->index, WRITE_વૃક્ષ_SILENT | WRITE_વૃક્ષ_REPAIR);

	જો (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
		die(_("unable to write new index file"));

	જો (!opts->discard_changes && !opts->quiet && new_branch_info->commit)
		show_local_changes(&new_branch_info->commit->object, &opts->diff_options);

	પાછા 0;
}

સ્થિર શૂન્ય report_tracking(સંરચના branch_info *new_branch_info)
{
	સંરચના strbuf sb = STRBUF_INIT;
	સંરચના branch *branch = branch_get(new_branch_info->name);

	જો (!format_tracking_info(branch, &sb, AHEAD_BEHIND_FULL, 1))
		પાછા;
	fputs(sb.buf, stdout);
	strbuf_release(&sb);
}

સ્થિર શૂન્ય update_refs_for_switch(અચલ સંરચના checkout_opts *opts,
				   સંરચના branch_info *old_branch_info,
				   સંરચના branch_info *new_branch_info)
{
	સંરચના strbuf સંદેશ = STRBUF_INIT;
	અચલ અક્ષર *old_desc, *reflog_સંદેશ;
	જો (opts->new_branch) {
		જો (opts->new_orphan_branch) {
			ગણના log_refs_config log_all_ref_updates = LOG_REFS_UNSET;
			અચલ અક્ષર *કિંમત;
			અક્ષર *refname;

			જો (!repo_config_get_string_tmp(the_repository, "core.logallrefupdates", &કિંમત))
				log_all_ref_updates = refs_parse_log_all_ref_updates_config(કિંમત);

			refname = mkpathdup("refs/heads/%s", opts->new_orphan_branch);
			જો (opts->new_branch_log &&
			    !should_autocreate_reflog(log_all_ref_updates, refname)) {
				પૂર્ણાંક ret;
				સંરચના strbuf err = STRBUF_INIT;

				ret = refs_create_reflog(get_main_ref_store(the_repository),
							 refname, &err);
				જો (ret) {
					fprintf(stderr, _("Can not do reflog for '%s': %s\n"),
						opts->new_orphan_branch, err.buf);
					strbuf_release(&err);
					મુક્ત(refname);
					પાછા;
				}
				strbuf_release(&err);
			}
			મુક્ત(refname);
		}
		નહીંતર
			create_branch(the_repository,
				      opts->new_branch, new_branch_info->name,
				      opts->new_branch_force ? 1 : 0,
				      opts->new_branch_force ? 1 : 0,
				      opts->new_branch_log,
				      opts->quiet,
				      opts->track,
				      0);
		મુક્ત(new_branch_info->name);
		મુક્ત(new_branch_info->refname);
		new_branch_info->name = xstrdup(opts->new_branch);
		setup_branch_path(new_branch_info);
	}

	old_desc = old_branch_info->name;
	જો (!old_desc && old_branch_info->commit)
		old_desc = oid_to_hex(&old_branch_info->commit->object.oid);

	reflog_સંદેશ = getenv("GIT_REFLOG_ACTION");
	જો (!reflog_સંદેશ)
		strbuf_addf(&સંદેશ, "checkout: moving from %s to %s",
			old_desc ? old_desc : "(invalid)", new_branch_info->name);
	નહીંતર
		strbuf_insertstr(&સંદેશ, 0, reflog_સંદેશ);

	જો (!strcmp(new_branch_info->name, "HEAD") && !new_branch_info->path && !opts->force_detach) {
		/* Nothing to do. */
	} નહીંતર જો (opts->force_detach || !new_branch_info->path) {	/* No longer on any branch. */
		refs_update_ref(get_main_ref_store(the_repository), સંદેશ.buf,
				"HEAD", &new_branch_info->commit->object.oid,
				ખાલી,
				REF_NO_DEREF, UPDATE_REFS_DIE_ON_ERR);
		જો (!opts->quiet) {
			જો (old_branch_info->path &&
			    advice_enabled(ADVICE_DETACHED_HEAD) && !opts->force_detach)
				detach_advice(new_branch_info->name);
			describe_detached_head(_("HEAD is now at"), new_branch_info->commit);
		}
	} નહીંતર જો (new_branch_info->path) {	/* Switch branches. */
		જો (refs_update_symref(get_main_ref_store(the_repository), "HEAD", new_branch_info->path, સંદેશ.buf) < 0)
			die(_("unable to update HEAD"));
		જો (!opts->quiet) {
			જો (old_branch_info->path && !strcmp(new_branch_info->path, old_branch_info->path)) {
				જો (opts->new_branch_force)
					fprintf(stderr, _("Reset branch '%s'\n"),
						new_branch_info->name);
				નહીંતર
					fprintf(stderr, _("Already on '%s'\n"),
						new_branch_info->name);
			} નહીંતર જો (opts->new_branch) {
				જો (opts->branch_exists)
					fprintf(stderr, _("Switched to and reset branch '%s'\n"), new_branch_info->name);
				નહીંતર
					fprintf(stderr, _("Switched to a new branch '%s'\n"), new_branch_info->name);
			} નહીંતર {
				fprintf(stderr, _("Switched to branch '%s'\n"),
					new_branch_info->name);
			}
		}
		જો (old_branch_info->path && old_branch_info->name) {
			જો (!refs_ref_exists(get_main_ref_store(the_repository), old_branch_info->path) && refs_reflog_exists(get_main_ref_store(the_repository), old_branch_info->path))
				refs_delete_reflog(get_main_ref_store(the_repository),
						   old_branch_info->path);
		}
	}
	remove_branch_state(the_repository, !opts->quiet);
	strbuf_release(&સંદેશ);
	જો (!opts->quiet &&
	    !opts->force_detach &&
	    (new_branch_info->path || !strcmp(new_branch_info->name, "HEAD")))
		report_tracking(new_branch_info);
}

સ્થિર પૂર્ણાંક add_pending_uninteresting_ref(અચલ સંરચના reference *ref, શૂન્ય *cb_data)
{
	add_pending_oid(cb_data, ref->name, ref->oid, UNINTERESTING);
	પાછા 0;
}

સ્થિર શૂન્ય describe_one_orphan(સંરચના strbuf *sb, સંરચના commit *commit)
{
	strbuf_addstr(sb, "  ");
	strbuf_add_unique_abbrev(sb, &commit->object.oid, DEFAULT_ABBREV);
	strbuf_addch(sb, ' ');
	જો (!repo_parse_commit(the_repository, commit))
		pp_commit_easy(CMIT_FMT_ONELINE, commit, sb);
	strbuf_addch(sb, '\n');
}

#define ORPHAN_CUTOFF 4
સ્થિર શૂન્ય suggest_reattach(સંરચના commit *commit, સંરચના rev_info *revs)
{
	સંરચના commit *c, *last = ખાલી;
	સંરચના strbuf sb = STRBUF_INIT;
	પૂર્ણાંક lost = 0;
	જ્યારે ((c = get_revision(revs)) != ખાલી) {
		જો (lost < ORPHAN_CUTOFF)
			describe_one_orphan(&sb, c);
		last = c;
		lost++;
	}
	જો (ORPHAN_CUTOFF < lost) {
		પૂર્ણાંક more = lost - ORPHAN_CUTOFF;
		જો (more == 1)
			describe_one_orphan(&sb, last);
		નહીંતર
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

	જો (advice_enabled(ADVICE_DETACHED_HEAD))
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
સ્થિર શૂન્ય orphaned_commit_warning(સંરચના commit *old_commit, સંરચના commit *new_commit)
{
	સંરચના rev_info revs;
	સંરચના object *object = &old_commit->object;

	repo_init_revisions(the_repository, &revs, ખાલી);
	setup_revisions(0, ખાલી, &revs, ખાલી);

	object->flags &= ~UNINTERESTING;
	add_pending_object(&revs, object, oid_to_hex(&object->oid));

	refs_for_each_ref(get_main_ref_store(the_repository),
			  add_pending_uninteresting_ref, &revs);
	જો (new_commit)
		add_pending_oid(&revs, "HEAD",
				&new_commit->object.oid,
				UNINTERESTING);

	જો (prepare_revision_walk(&revs))
		die(_("internal error in revision walk"));
	જો (!(old_commit->object.flags & UNINTERESTING))
		suggest_reattach(old_commit, &revs);
	નહીંતર
		describe_detached_head(_("Previous HEAD position was"), old_commit);

	/* Clean up objects used, as they will be reused. */
	repo_clear_commit_marks(the_repository, ALL_REV_FLAGS);
	release_revisions(&revs);
}

સ્થિર પૂર્ણાંક switch_branches(અચલ સંરચના checkout_opts *opts,
			   સંરચના branch_info *new_branch_info)
{
	પૂર્ણાંક ret = 0;
	સંરચના branch_info old_branch_info = { 0 };
	સંરચના object_id rev;
	પૂર્ણાંક flag, writeout_error = 0;
	પૂર્ણાંક do_merge = 1;
	પૂર્ણાંક created_autostash = 0;
	ગણના stash_apply_result autostash_res = STASH_APPLY_CLEAN;
	સંરચના strbuf old_commit_shortname = STRBUF_INIT;
	સંરચના strbuf autostash_સંદેશ = STRBUF_INIT;
	અચલ અક્ષર *stash_label_base = ખાલી;

	trace2_cmd_mode("branch");

	memset(&old_branch_info, 0, કદ(old_branch_info));
	old_branch_info.path = refs_resolve_refdup(get_main_ref_store(the_repository),
						   "HEAD", 0, &rev, &flag);
	જો (old_branch_info.path)
		old_branch_info.commit = lookup_commit_reference_gently(the_repository, &rev, 1);
	જો (!(flag & REF_ISSYMREF))
		FREE_AND_NULL(old_branch_info.path);

	જો (old_branch_info.path) {
		અચલ અક્ષર *અચલ prefix = "refs/heads/";
		અચલ અક્ષર *p;
		જો (skip_prefix(old_branch_info.path, prefix, &p))
			old_branch_info.name = xstrdup(p);
	}

	જો (opts->new_orphan_branch && opts->orphan_from_empty_વૃક્ષ) {
		જો (new_branch_info->name)
			BUG("'switch --orphan' should never accept a commit as starting point");
		new_branch_info->commit = ખાલી;
		new_branch_info->name = xstrdup("(empty)");
		do_merge = 1;
	}

	જો (!new_branch_info->name) {
		new_branch_info->name = xstrdup("HEAD");
		new_branch_info->commit = old_branch_info.commit;
		જો (!new_branch_info->commit)
			die(_("You are on a branch yet to be born"));
		parse_commit_or_die(new_branch_info->commit);

		જો (opts->only_merge_on_switching_branches)
			do_merge = 0;
	}

	જો (old_branch_info.name) {
		stash_label_base = old_branch_info.name;
	} નહીંતર જો (old_branch_info.commit) {
		strbuf_add_unique_abbrev(&old_commit_shortname,
					 &old_branch_info.commit->object.oid,
					 DEFAULT_ABBREV);
		stash_label_base = old_commit_shortname.buf;
	}

	જો (do_merge) {
		ret = merge_working_વૃક્ષ(opts, &old_branch_info, new_branch_info,
					 opts->merge, &writeout_error);
		જો (ret == MERGE_WORKING_વૃક્ષ_UNPACK_FAILED && opts->merge) {
			strbuf_addf(&autostash_સંદેશ,
				    "autostash while switching to '%s'",
				    new_branch_info->name);
			create_autostash_ref(the_repository,
					     "CHECKOUT_AUTOSTASH_HEAD",
					     autostash_સંદેશ.buf, true);
			created_autostash = 1;
			ret = merge_working_વૃક્ષ(opts, &old_branch_info, new_branch_info,
						 false, &writeout_error);
		}
		જો (created_autostash) {
			જો (opts->conflict_style >= 0) {
				સંરચના strbuf cfg = STRBUF_INIT;
				strbuf_addf(&cfg, "merge.conflictStyle=%s",
					    conflict_style_name(opts->conflict_style));
				git_config_ધકેલો_parameter(cfg.buf);
				strbuf_release(&cfg);
			}
			autostash_res = apply_autostash_ref(the_repository,
				    "CHECKOUT_AUTOSTASH_HEAD",
				    new_branch_info->name,
				    "local",
				    stash_label_base,
				    autostash_સંદેશ.buf);
		}
		જો (ret) {
			branch_info_release(&old_branch_info);
			strbuf_release(&old_commit_shortname);
			strbuf_release(&autostash_સંદેશ);
			પાછા ret < 0 ? 1 : ret;
		}
	}

	જો (!opts->quiet && !old_branch_info.path && old_branch_info.commit && new_branch_info->commit != old_branch_info.commit)
		orphaned_commit_warning(old_branch_info.commit, new_branch_info->commit);

	જો (autostash_res == STASH_APPLY_CONFLICT && !opts->quiet)
		fputc('\n', stderr);
	update_refs_for_switch(opts, &old_branch_info, new_branch_info);

	જો (created_autostash) {
		discard_index(the_repository->index);
		જો (repo_read_index(the_repository) < 0)
			die(_("index file corrupt"));

		જો (!opts->quiet && new_branch_info->commit) {
			printf(_("The following paths have local changes:\n"));
			show_local_changes(&new_branch_info->commit->object,
					   &opts->diff_options);
		}
	}

	ret = post_checkout_hook(old_branch_info.commit, new_branch_info->commit, 1);
	branch_info_release(&old_branch_info);
	strbuf_release(&old_commit_shortname);
	strbuf_release(&autostash_સંદેશ);

	પાછા ret || writeout_error;
}

સ્થિર પૂર્ણાંક git_checkout_config(અચલ અક્ષર *var, અચલ અક્ષર *કિંમત,
			       અચલ સંરચના config_context *ctx, શૂન્ય *cb)
{
	સંરચના checkout_opts *opts = cb;

	જો (!strcmp(var, "diff.ignoresubmodules")) {
		જો (!કિંમત)
			પાછા config_error_nonbool(var);
		handle_ignore_submodules_arg(&opts->diff_options, કિંમત);
		પાછા 0;
	}
	જો (!strcmp(var, "checkout.guess")) {
		opts->dwim_new_local_branch = git_config_bool(var, કિંમત);
		પાછા 0;
	}

	જો (starts_with(var, "submodule."))
		પાછા git_default_submodule_config(var, કિંમત, ખાલી);

	પાછા git_xmerge_config(var, કિંમત, ctx, ખાલી);
}

સ્થિર શૂન્ય setup_new_branch_info_and_source_વૃક્ષ(
	સંરચના branch_info *new_branch_info,
	સંરચના checkout_opts *opts,
	સંરચના object_id *rev,
	અચલ અક્ષર *arg)
{
	સંરચના વૃક્ષ **source_વૃક્ષ = &opts->source_વૃક્ષ;
	સંરચના object_id branch_rev;

	/* treat '@' as a shortcut for 'HEAD' */
	new_branch_info->name = !strcmp(arg, "@") ? xstrdup("HEAD") :
						    xstrdup(arg);
	setup_branch_path(new_branch_info);

	જો (!check_refname_format(new_branch_info->path, 0) &&
	    !refs_read_ref(get_main_ref_store(the_repository), new_branch_info->path, &branch_rev))
		oidcpy(rev, &branch_rev);
	નહીંતર
		/* not an existing branch */
		FREE_AND_NULL(new_branch_info->path);

	new_branch_info->commit = lookup_commit_reference_gently(the_repository, rev, 1);
	જો (!new_branch_info->commit) {
		/* not a commit */
		*source_વૃક્ષ = repo_parse_વૃક્ષ_indirect(the_repository, rev);
		જો (!*source_વૃક્ષ)
			die(_("unable to read tree (%s)"), oid_to_hex(rev));
	} નહીંતર {
		parse_commit_or_die(new_branch_info->commit);
		*source_વૃક્ષ = repo_get_commit_વૃક્ષ(the_repository,
						    new_branch_info->commit);
		જો (!*source_વૃક્ષ)
			die(_("unable to read tree (%s)"),
			    oid_to_hex(&new_branch_info->commit->object.oid));
	}
}


ગણના checkout_command {
	CHECKOUT_CHECKOUT = 1,
	CHECKOUT_SWITCH = 2,
	CHECKOUT_RESTORE = 3,
};

સ્થિર શૂન્ય advise_disambiguating_remotes(ગણના checkout_command which_command,
					  અચલ અક્ષર *branch,
					  અચલ સંરચના string_list *matched_remote_names)
{
	અચલ અક્ષર *cmdname;
	સંરચના string_list_item *item;

	પસંદગી (which_command) {
	સ્થિતિ CHECKOUT_CHECKOUT:
		cmdname = "checkout";
		તોડો;
	સ્થિતિ CHECKOUT_SWITCH:
		cmdname = "switch";
		તોડો;
	મૂળભૂત:
		BUG("command <%d> should not reach advise_disambiguating_remotes",
		    which_command);
		તોડો;
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

સ્થિર અક્ષર *parse_remote_branch(અચલ અક્ષર *arg,
				 સંરચના object_id *rev,
				 પૂર્ણાંક could_be_checkout_paths,
				 ગણના checkout_command which_command)
{
	પૂર્ણાંક અંક_matches = 0;
	સંરચના string_list matched_remote_names = STRING_LIST_INIT_DUP;

	અક્ષર *remote = unique_tracking_name(arg, rev, &અંક_matches,
					    &matched_remote_names);

	જો (remote && could_be_checkout_paths) {
		die(_("'%s' could be both a local file and a tracking branch.\n"
			"Please use -- (and optionally --no-guess) to disambiguate"),
		    arg);
	}

	જો (!remote && અંક_matches > 1) {
		જો (advice_enabled(ADVICE_CHECKOUT_AMBIGUOUS_REMOTE_BRANCH_NAME))
			advise_disambiguating_remotes(which_command, arg,
						      &matched_remote_names);
		die(_("'%s' matched multiple (%d) remote tracking branches"),
		    arg, અંક_matches);
	}

	string_list_clear(&matched_remote_names, 0);

	પાછા remote;
}

સ્થિર પૂર્ણાંક parse_branchname_arg(પૂર્ણાંક argc, અચલ અક્ષર **argv,
				પૂર્ણાંક dwim_new_local_branch_ok,
				ગણના checkout_command which_command,
				સંરચના branch_info *new_branch_info,
				સંરચના checkout_opts *opts,
				સંરચના object_id *rev)
{
	અચલ અક્ષર **new_branch = &opts->new_branch;
	પૂર્ણાંક argcount = 0;
	અચલ અક્ષર *arg;
	અક્ષર *remote = ખાલી;
	પૂર્ણાંક dash_dash_pos;
	પૂર્ણાંક has_dash_dash = 0;
	પૂર્ણાંક i;

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
	જો (!argc)
		પાછા 0;

	જો (!opts->accept_pathspec) {
		જો (argc > 1)
			die(_("only one reference expected"));
		has_dash_dash = 1; /* helps disambiguate */
	}

	arg = argv[0];
	dash_dash_pos = -1;
	માટે (i = 0; i < argc; i++) {
		જો (opts->accept_pathspec && !strcmp(argv[i], "--")) {
			dash_dash_pos = i;
			તોડો;
		}
	}
	જો (dash_dash_pos == 0)
		પાછા 1; /* case (2) */
	નહીંતર જો (dash_dash_pos == 1)
		has_dash_dash = 1; /* case (3) or (1) */
	નહીંતર જો (dash_dash_pos >= 2)
		die(_("only one reference expected, %d given."), dash_dash_pos);
	opts->ગણતરી_checkout_paths = !opts->quiet && !has_dash_dash;

	જો (!strcmp(arg, "-"))
		arg = "@{-1}";

	જો (repo_get_oid_mb(the_repository, arg, rev)) {
		/*
		 * Either case (3) or (4), with <something> not being
		 * a commit, or an attempt to use case (1) with an
		 * invalid ref.
		 *
		 * It's likely an error, but we need to find out if
		 * we should auto-create the branch, case (3).(b).
		 */
		પૂર્ણાંક recover_with_dwim = dwim_new_local_branch_ok;

		પૂર્ણાંક could_be_checkout_paths = !has_dash_dash &&
			check_filename(opts->prefix, arg);

		જો (!has_dash_dash && !no_wildcard(arg))
			recover_with_dwim = 0;

		/*
		 * Accept "git checkout foo", "git checkout foo --"
		 * and "git switch foo" as candidates for dwim.
		 */
		જો (!(argc == 1 && !has_dash_dash) &&
		    !(argc == 2 && has_dash_dash) &&
		    opts->accept_pathspec)
			recover_with_dwim = 0;

		જો (recover_with_dwim) {
			remote = parse_remote_branch(arg, rev,
						     could_be_checkout_paths,
						     which_command);
			જો (remote) {
				*new_branch = arg;
				arg = remote;
				/* DWIMmed to create local branch, case (3).(b) */
			} નહીંતર {
				recover_with_dwim = 0;
			}
		}

		જો (!recover_with_dwim) {
			જો (has_dash_dash)
				die(_("invalid reference: %s"), arg);
			પાછા argcount;
		}
	}

	/* we can't end up being in (2) anymore, eat the argument */
	argcount++;
	argv++;
	argc--;

	setup_new_branch_info_and_source_વૃક્ષ(new_branch_info, opts, rev, arg);

	જો (!opts->source_વૃક્ષ)                   /* case (1): want a tree */
		die(_("reference is not a tree: %s"), arg);

	જો (!has_dash_dash) {	/* case (3).(d) -> (1) */
		/*
		 * Do not complain the most common case
		 *	git checkout branch
		 * even if there happen to be a file called 'branch';
		 * it would be extremely annoying.
		 */
		જો (argc)
			verify_non_filename(the_repository, opts->prefix, arg);
	} નહીંતર જો (opts->accept_pathspec) {
		argcount++;
		argv++;
		argc--;
	}

	મુક્ત(remote);
	પાછા argcount;
}

સ્થિર પૂર્ણાંક switch_unborn_to_new_branch(અચલ સંરચના checkout_opts *opts)
{
	પૂર્ણાંક status;
	સંરચના strbuf branch_ref = STRBUF_INIT;

	trace2_cmd_mode("unborn");

	જો (!opts->new_branch)
		die(_("You are on a branch yet to be born"));
	strbuf_addf(&branch_ref, "refs/heads/%s", opts->new_branch);
	status = refs_update_symref(get_main_ref_store(the_repository),
				    "HEAD", branch_ref.buf, "checkout -b");
	strbuf_release(&branch_ref);
	જો (!opts->quiet)
		fprintf(stderr, _("Switched to a new branch '%s'\n"),
			opts->new_branch);
	પાછા status;
}

સ્થિર શૂન્ય die_expecting_a_branch(અચલ સંરચના branch_info *branch_info)
{
	સંરચના object_id oid;
	અક્ષર *to_free;
	પૂર્ણાંક code;

	જો (repo_dwim_ref(the_repository, branch_info->name,
			  strlen(branch_info->name), &oid, &to_free, 0) == 1) {
		અચલ અક્ષર *ref = to_free;

		જો (skip_prefix(ref, "refs/tags/", &ref))
			code = die_message(_("a branch is expected, got tag '%s'"), ref);
		નહીંતર જો (skip_prefix(ref, "refs/remotes/", &ref))
			code = die_message(_("a branch is expected, got remote branch '%s'"), ref);
		નહીંતર
			code = die_message(_("a branch is expected, got '%s'"), ref);
	}
	નહીંતર જો (branch_info->commit)
		code = die_message(_("a branch is expected, got commit '%s'"), branch_info->name);
	નહીંતર
		/*
		 * This case should never happen because we already die() on
		 * non-commit, but just in case.
		 */
		code = die_message(_("a branch is expected, got '%s'"), branch_info->name);

	જો (advice_enabled(ADVICE_SUGGEST_DETACHING_HEAD))
		advise(_("If you want to detach HEAD at the commit, try again with the --detach option."));

	exit(code);
}

સ્થિર શૂન્ય die_if_some_operation_in_progress(શૂન્ય)
{
	સંરચના wt_status_state state;

	memset(&state, 0, કદ(state));
	wt_status_get_state(the_repository, &state, 0);

	જો (state.merge_in_progress)
		die(_("cannot switch branch while merging\n"
		      "Consider \"git merge --quit\" "
		      "or \"git worktree add\"."));
	જો (state.am_in_progress)
		die(_("cannot switch branch in the middle of an am session\n"
		      "Consider \"git am --quit\" "
		      "or \"git worktree add\"."));
	જો (state.rebase_interactive_in_progress || state.rebase_in_progress)
		die(_("cannot switch branch while rebasing\n"
		      "Consider \"git rebase --quit\" "
		      "or \"git worktree add\"."));
	જો (state.cherry_pick_in_progress)
		die(_("cannot switch branch while cherry-picking\n"
		      "Consider \"git cherry-pick --quit\" "
		      "or \"git worktree add\"."));
	જો (state.revert_in_progress)
		die(_("cannot switch branch while reverting\n"
		      "Consider \"git revert --quit\" "
		      "or \"git worktree add\"."));
	જો (state.bisect_in_progress)
		warning(_("you are switching branch while bisecting"));

	wt_status_state_free_buffers(&state);
}

/*
 * die if attempting to checkout an existing branch that is in use
 * in another worktree, unless ignore-other-wortrees option is given.
 * The check is bypassed when the branch is already the current one,
 * as it will not make things any worse.
 */
સ્થિર શૂન્ય die_if_switching_to_a_branch_in_use(સંરચના checkout_opts *opts,
						અચલ અક્ષર *full_ref)
{
	પૂર્ણાંક flags;
	અક્ષર *head_ref;

	જો (opts->ignore_other_worktrees)
		પાછા;
	head_ref = refs_resolve_refdup(get_main_ref_store(the_repository),
				       "HEAD", 0, ખાલી, &flags);
	જો (head_ref && (!(flags & REF_ISSYMREF) || strcmp(head_ref, full_ref)))
		die_if_checked_out(full_ref, 1);
	મુક્ત(head_ref);
}

સ્થિર પૂર્ણાંક checkout_branch(સંરચના checkout_opts *opts,
			   સંરચના branch_info *new_branch_info)
{
	સંરચના repo_config_કિંમતો *cfg = repo_config_કિંમતો(the_repository);
	પૂર્ણાંક noop_switch = (!new_branch_info->name &&
			   !opts->new_branch &&
			   !opts->force_detach);

	જો (opts->pathspec.nr)
		die(_("paths cannot be used with switching branches"));

	જો (opts->patch_mode)
		die(_("'%s' cannot be used with switching branches"),
		    "--patch");

	જો (opts->overlay_mode != -1)
		die(_("'%s' cannot be used with switching branches"),
		    "--[no]-overlay");

	જો (opts->writeout_stage) {
		અચલ અક્ષર *સંદેશ;
		જો (noop_switch)
			સંદેશ = _("'%s' needs the paths to check out");
		નહીંતર
			સંદેશ = _("'%s' cannot be used with switching branches");
		die(સંદેશ, "--ours/--theirs");
	}

	જો (opts->force && opts->merge)
		die(_("'%s' cannot be used with '%s'"), "-f", "-m");

	જો (opts->discard_changes && opts->merge)
		die(_("'%s' cannot be used with '%s'"), "--discard-changes", "--merge");

	જો (opts->force_detach && opts->new_branch)
		die(_("'%s' cannot be used with '%s'"),
		    "--detach", "-b/-B/--orphan");

	જો (opts->new_orphan_branch) {
		જો (opts->track != BRANCH_TRACK_UNSPECIFIED)
			die(_("'%s' cannot be used with '%s'"), "--orphan", "-t");
		જો (opts->orphan_from_empty_વૃક્ષ && new_branch_info->name)
			die(_("'%s' cannot take <start-point>"), "--orphan");
	} નહીંતર જો (opts->force_detach) {
		જો (opts->track != BRANCH_TRACK_UNSPECIFIED)
			die(_("'%s' cannot be used with '%s'"), "--detach", "-t");
	} નહીંતર જો (opts->track == BRANCH_TRACK_UNSPECIFIED)
		opts->track = cfg->branch_track;

	જો (new_branch_info->name && !new_branch_info->commit)
		die(_("Cannot switch branch to a non-commit '%s'"),
		    new_branch_info->name);

	જો (noop_switch &&
	    !opts->switch_branch_doing_nothing_is_ok)
		die(_("missing branch or commit argument"));

	જો (!opts->implicit_detach &&
	    !opts->force_detach &&
	    !opts->new_branch &&
	    !opts->new_branch_force &&
	    new_branch_info->name &&
	    !new_branch_info->path)
		die_expecting_a_branch(new_branch_info);

	જો (!opts->can_switch_when_in_progress)
		die_if_some_operation_in_progress();

	/* "git checkout <branch>" */
	જો (new_branch_info->path && !opts->force_detach && !opts->new_branch)
		die_if_switching_to_a_branch_in_use(opts, new_branch_info->path);

	/* "git checkout -B <branch>" */
	જો (opts->new_branch_force) {
		અક્ષર *full_ref = xstrfmt("refs/heads/%s", opts->new_branch);
		die_if_switching_to_a_branch_in_use(opts, full_ref);
		મુક્ત(full_ref);
	}

	જો (!new_branch_info->commit && opts->new_branch) {
		સંરચના object_id rev;
		પૂર્ણાંક flag;

		જો (!refs_read_ref_full(get_main_ref_store(the_repository), "HEAD", 0, &rev, &flag) &&
		    (flag & REF_ISSYMREF) && is_null_oid(&rev))
			પાછા switch_unborn_to_new_branch(opts);
	}
	પાછા switch_branches(opts, new_branch_info);
}

સ્થિર પૂર્ણાંક parse_opt_conflict(અચલ સંરચના option *o, અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	સંરચના checkout_opts *opts = o->કિંમત;

	જો (unset) {
		opts->conflict_style = -1;
		પાછા 0;
	}
	opts->conflict_style = parse_conflict_style_name(arg);
	જો (opts->conflict_style < 0)
		પાછા error(_("unknown conflict style '%s'"), arg);
	/* --conflict overrides a previous --no-merge */
	જો (!opts->merge)
		opts->merge = -1;

	પાછા 0;
}

સ્થિર સંરચના option *add_common_options(સંરચના checkout_opts *opts,
					 સંરચના option *prevopts)
{
	સંરચના option options[] = {
		OPT__QUIET(&opts->quiet, N_("suppress progress reporting")),
		OPT_CALLBACK_F(0, "recurse-submodules", ખાલી,
			    "checkout", "control recursive updating of submodules",
			    PARSE_OPT_OPTARG, option_parse_recurse_submodules_worktree_updater),
		OPT_BOOL(0, "progress", &opts->show_progress, N_("force progress reporting")),
		OPT_BOOL('m', "merge", &opts->merge, N_("perform a 3-way merge with the new branch")),
		OPT_CALLBACK(0, "conflict", opts, N_("style"),
			     N_("conflict style (merge, diff3, or zdiff3)"),
			     parse_opt_conflict),
		OPT_END()
	};
	સંરચના option *newopts = parse_options_concat(prevopts, options);
	મુક્ત(prevopts);
	પાછા newopts;
}

સ્થિર સંરચના option *add_common_switch_branch_options(
	સંરચના checkout_opts *opts, સંરચના option *prevopts)
{
	સંરચના option options[] = {
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
	સંરચના option *newopts = parse_options_concat(prevopts, options);
	મુક્ત(prevopts);
	પાછા newopts;
}

સ્થિર સંરચના option *add_checkout_path_options(સંરચના checkout_opts *opts,
						સંરચના option *prevopts)
{
	સંરચના option options[] = {
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
	સંરચના option *newopts = parse_options_concat(prevopts, options);
	મુક્ત(prevopts);
	પાછા newopts;
}

/* create-branch option (either b or c) */
સ્થિર અક્ષર cb_option = 'b';

સ્થિર પૂર્ણાંક checkout_main(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
			 સંરચના checkout_opts *opts, સંરચના option *options,
			 ગણના checkout_command which_command)
{
	પૂર્ણાંક parseopt_flags = 0;
	સંરચના branch_info new_branch_info = { 0 };
	પૂર્ણાંક ret;

	સ્થિર અચલ અક્ષર * અચલ checkout_usage[] = {
		N_("git checkout [<options>] <branch>"),
		N_("git checkout [<options>] [<branch>] -- <file>..."),
		ખાલી,
	};

	સ્થિર અચલ અક્ષર * અચલ switch_branch_usage[] = {
		N_("git switch [<options>] [<branch>]"),
		ખાલી,
	};

	સ્થિર અચલ અક્ષર * અચલ restore_usage[] = {
		N_("git restore [<options>] [--source=<branch>] <file>..."),
		ખાલી,
	};

	અચલ અક્ષર * અચલ *usagestr;

	પસંદગી (which_command) {
	સ્થિતિ CHECKOUT_CHECKOUT:
		usagestr = checkout_usage;
		તોડો;
	સ્થિતિ CHECKOUT_SWITCH:
		usagestr = switch_branch_usage;
		તોડો;
	સ્થિતિ CHECKOUT_RESTORE:
		usagestr = restore_usage;
		તોડો;
	મૂળભૂત:
		BUG("no such checkout variant %d", which_command);
	}

	opts->overwrite_ignore = 1;
	opts->prefix = prefix;
	opts->show_progress = -1;

	repo_config(the_repository, git_checkout_config, opts);
	જો (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_full_index = 0;
	}

	opts->track = BRANCH_TRACK_UNSPECIFIED;

	જો (!opts->accept_pathspec && !opts->accept_ref)
		BUG("make up your mind, you need to take _something_");
	જો (opts->accept_pathspec && opts->accept_ref)
		parseopt_flags = PARSE_OPT_KEEP_DASHDASH;

	argc = parse_options(argc, argv, prefix, options,
			     usagestr, parseopt_flags);

	જો (opts->patch_context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	જો (opts->patch_interhunk_context < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	જો (!opts->patch_mode) {
		જો (opts->patch_context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--patch");
		જો (opts->patch_interhunk_context != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--patch");
		જો (!opts->auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--patch");
	}

	જો (opts->show_progress < 0) {
		જો (opts->quiet)
			opts->show_progress = 0;
		નહીંતર
			opts->show_progress = isatty(2);
	}

	/* --conflicts implies --merge */
	જો (opts->merge == -1)
		opts->merge = opts->conflict_style >= 0;

	જો (opts->force) {
		opts->discard_changes = 1;
		opts->ignore_unmerged_opt = "--force";
		opts->ignore_unmerged = 1;
	}

	જો ((!!opts->new_branch + !!opts->new_branch_force + !!opts->new_orphan_branch) > 1)
		die(_("options '-%c', '-%c', and '%s' cannot be used together"),
			cb_option, toupper(cb_option), "--orphan");

	જો (opts->overlay_mode == 1 && opts->patch_mode)
		die(_("options '%s' and '%s' cannot be used together"), "-p", "--overlay");

	જો (opts->checkout_index >= 0 || opts->checkout_worktree >= 0) {
		જો (opts->checkout_index < 0)
			opts->checkout_index = 0;
		જો (opts->checkout_worktree < 0)
			opts->checkout_worktree = 0;
	} નહીંતર {
		જો (opts->checkout_index < 0)
			opts->checkout_index = -opts->checkout_index - 1;
		જો (opts->checkout_worktree < 0)
			opts->checkout_worktree = -opts->checkout_worktree - 1;
	}
	જો (opts->checkout_index < 0 || opts->checkout_worktree < 0)
		BUG("these flags should be non-negative by now");
	/*
	 * convenient shortcut: "git restore --staged [--worktree]" equals
	 * "git restore --staged [--worktree] --source HEAD"
	 */
	જો (!opts->from_treeish && opts->checkout_index)
		opts->from_treeish = "HEAD";

	/*
	 * From here on, new_branch will contain the branch to be checked out,
	 * and new_branch_force and new_orphan_branch will tell us which one of
	 * -b/-B/-c/-C/--orphan is being used.
	 */
	જો (opts->new_branch_force)
		opts->new_branch = opts->new_branch_force;

	જો (opts->new_orphan_branch)
		opts->new_branch = opts->new_orphan_branch;

	/* --track without -c/-C/-b/-B/--orphan should DWIM */
	જો (opts->track != BRANCH_TRACK_UNSPECIFIED && !opts->new_branch) {
		અચલ અક્ષર *argv0 = argv[0];
		જો (!argc || !strcmp(argv0, "--"))
			die(_("--track needs a branch name"));
		skip_prefix(argv0, "refs/", &argv0);
		skip_prefix(argv0, "remotes/", &argv0);
		argv0 = strchr(argv0, '/');
		જો (!argv0 || !argv0[1])
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
	જો (argc && opts->accept_ref) {
		સંરચના object_id rev;
		પૂર્ણાંક dwim_ok =
			!opts->patch_mode &&
			opts->dwim_new_local_branch &&
			opts->track == BRANCH_TRACK_UNSPECIFIED &&
			!opts->new_branch;
		પૂર્ણાંક n = parse_branchname_arg(argc, argv, dwim_ok, which_command,
					     &new_branch_info, opts, &rev);
		argv += n;
		argc -= n;
	} નહીંતર જો (!opts->accept_ref && opts->from_treeish) {
		સંરચના object_id rev;

		જો (repo_get_oid_mb(the_repository, opts->from_treeish, &rev))
			die(_("could not resolve '%s'"), opts->from_treeish);

		setup_new_branch_info_and_source_વૃક્ષ(&new_branch_info,
						      opts, &rev,
						      opts->from_treeish);

		જો (!opts->source_વૃક્ષ)
			die(_("reference is not a tree: %s"), opts->from_treeish);
	}

	જો (argc) {
		parse_pathspec(&opts->pathspec, 0,
			       opts->patch_mode ? PATHSPEC_PREFIX_ORIGIN : 0,
			       prefix, argv);

		જો (!opts->pathspec.nr)
			die(_("invalid path specification"));

		/*
		 * Try to give more helpful suggestion.
		 * new_branch && argc > 1 will be caught later.
		 */
		જો (opts->new_branch && argc == 1 && !new_branch_info.commit)
			die(_("'%s' is not a commit and a branch '%s' cannot be created from it"),
				argv[0], opts->new_branch);

		જો (opts->force_detach)
			die(_("git checkout: --detach does not take a path argument '%s'"),
			    argv[0]);
	}

	જો (opts->pathspec_from_file) {
		જો (opts->pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		જો (opts->force_detach)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--detach");

		જો (opts->patch_mode)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--patch");

		parse_pathspec_file(&opts->pathspec, 0,
				    0,
				    prefix, opts->pathspec_from_file, opts->pathspec_file_nul);
	} નહીંતર જો (opts->pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	opts->pathspec.recursive = 1;

	જો (opts->pathspec.nr) {
		જો (1 < !!opts->writeout_stage + !!opts->force + !!opts->merge)
			die(_("git checkout: --ours/--theirs, --force and --merge are incompatible when\n"
			      "checking out of the index."));
	} નહીંતર {
		જો (opts->accept_pathspec && !opts->empty_pathspec_ok &&
		    !opts->patch_mode)	/* patch mode is special */
			die(_("you must specify path(s) to restore"));
	}

	જો (opts->new_branch) {
		સંરચના strbuf buf = STRBUF_INIT;

		જો (opts->new_branch_force)
			opts->branch_exists = validate_branchname(opts->new_branch, &buf);
		નહીંતર
			opts->branch_exists =
				validate_new_branchname(opts->new_branch, &buf, 0);
		strbuf_release(&buf);
	}

	જો (opts->patch_mode || opts->pathspec.nr)
		ret = checkout_paths(opts, &new_branch_info);
	નહીંતર
		ret = checkout_branch(opts, &new_branch_info);

	branch_info_release(&new_branch_info);
	clear_pathspec(&opts->pathspec);
	મુક્ત(opts->pathspec_from_file);
	મુક્ત(options);

	પાછા ret;
}

પૂર્ણાંક cmd_checkout(પૂર્ણાંક argc,
		 અચલ અક્ષર **argv,
		 અચલ અક્ષર *prefix,
		 સંરચના repository *repo UNUSED)
{
	સંરચના checkout_opts opts = CHECKOUT_OPTS_INIT;
	સંરચના option *options;
	સંરચના option checkout_options[] = {
		OPT_STRING('b', ખાલી, &opts.new_branch, N_("branch"),
			   N_("create and checkout a new branch")),
		OPT_STRING('B', ખાલી, &opts.new_branch_force, N_("branch"),
			   N_("create/reset and checkout a branch")),
		OPT_BOOL('l', ખાલી, &opts.new_branch_log, N_("create reflog for new branch")),
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
	opts.orphan_from_empty_વૃક્ષ = 0;
	opts.empty_pathspec_ok = 1;
	opts.overlay_mode = -1;
	opts.checkout_index = -2;    /* default on */
	opts.checkout_worktree = -2; /* default on */

	જો (argc == 3 && !strcmp(argv[1], "-b")) {
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

	પાછા checkout_main(argc, argv, prefix, &opts, options,
			     CHECKOUT_CHECKOUT);
}

પૂર્ણાંક cmd_switch(પૂર્ણાંક argc,
	       અચલ અક્ષર **argv,
	       અચલ અક્ષર *prefix,
	       સંરચના repository *repo UNUSED)
{
	સંરચના checkout_opts opts = CHECKOUT_OPTS_INIT;
	સંરચના option *options = ખાલી;
	સંરચના option switch_options[] = {
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
	opts.orphan_from_empty_વૃક્ષ = 1;
	opts.overlay_mode = -1;

	options = parse_options_dup(switch_options);
	options = add_common_options(&opts, options);
	options = add_common_switch_branch_options(&opts, options);

	cb_option = 'c';

	પાછા checkout_main(argc, argv, prefix, &opts, options,
			     CHECKOUT_SWITCH);
}

પૂર્ણાંક cmd_restore(પૂર્ણાંક argc,
		અચલ અક્ષર **argv,
		અચલ અક્ષર *prefix,
		સંરચના repository *repo UNUSED)
{
	સંરચના checkout_opts opts = CHECKOUT_OPTS_INIT;
	સંરચના option *options;
	સંરચના option restore_options[] = {
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

	પાછા checkout_main(argc, argv, prefix, &opts, options,
			     CHECKOUT_RESTORE);
}
