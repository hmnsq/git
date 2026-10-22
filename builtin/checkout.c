#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#تعریف_کرو DISABLE_SIGN_COMPARE_WARNINGS

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

ساخت checkout_opts {
	صحیح patch_منوال;
	صحیح patch_context;
	صحیح patch_interhunk_context;
	صحیح auto_advance;
	صحیح quiet;
	صحیح ملاؤ;
	صحیح force;
	صحیح force_detach;
	صحیح implicit_detach;
	صحیح writeout_stage;
	صحیح overwrite_ignore;
	صحیح ignore_skipworktree;
	صحیح ignore_other_worktrees;
	صحیح show_پیش_رفت;
	صحیح گنتی_checkout_paths;
	صحیح overlay_منوال;
	صحیح dwim_new_local_branch;
	صحیح discard_changes;
	صحیح accept_ref;
	صحیح accept_pathspec;
	صحیح switch_branch_doing_nothing_is_ok;
	صحیح only_ملاؤ_on_switching_branches;
	صحیح can_switch_when_اندر_پیش_رفت;
	صحیح orphan_from_خالی_خانے_درخت;
	صحیح خالی_خانے_pathspec_ok;
	صحیح checkout_اشاریہ;
	صحیح checkout_worktree;
	ثابت حرف *ignore_unmerged_opt;
	صحیح ignore_unmerged;
	صحیح pathspec_file_nul;
	حرف *pathspec_from_file;

	ثابت حرف *new_branch;
	ثابت حرف *new_branch_force;
	ثابت حرف *new_orphan_branch;
	صحیح new_branch_log;
	شمار branch_track track;
	ساخت diff_options diff_options;
	صحیح conflict_style;

	صحیح branch_exists;
	ثابت حرف *سابقہ;
	ساخت pathspec pathspec;
	ثابت حرف *from_treeish;
	ساخت درخت *ماخذ_درخت;
};

#تعریف_کرو CHECKOUT_OPTS_INIT { \
	.conflict_style = -1, \
	.ملاؤ = -1, \
	.patch_context = -1, \
	.patch_interhunk_context = -1, \
	.auto_advance = 1, \
}

#تعریف_کرو ملاؤ_WORKING_درخت_UNPACK_FAILED (-2)

ساخت branch_info {
	حرف *name; /* The short name used */
	حرف *راستہ; /* The full name of a real branch */
	ساخت commit *commit; /* The named commit */
	حرف *refname; /* The full name of the ref being checked out. */
	ساخت object_id oid; /* The object ID of the commit being checked out. */
	/*
	 * if not null the branch is detached because it's already
	 * checked out in this checkout
	 */
	حرف *checkout;
};

ساکن خالی branch_info_release(ساخت branch_info *info)
{
	آزاد(info->name);
	آزاد(info->راستہ);
	آزاد(info->refname);
	آزاد(info->checkout);
}

ساکن صحیح post_checkout_hook(ساخت commit *old_commit, ساخت commit *new_commit,
			      صحیح بدلا)
{
	ساخت تسلسل_hooks_opt opt = تسلسل_HOOKS_OPT_INIT_FORCE_SERIAL;

	/*
	 * "new_commit" can be NULL when checking out from the index before
	 * a commit exists.
	 */
	strvec_pushl(&opt.دلائل,
		     oid_مقصد_hex(old_commit ? &old_commit->object.oid : null_oid(the_hash_algo)),
		     oid_مقصد_hex(new_commit ? &new_commit->object.oid : null_oid(the_hash_algo)),
		     بدلا ? "1" : "0",
		     تہی);

	واپس تسلسل_hooks_opt(the_repository, "post-checkout", &opt);
}

/*
 * Handle a tree object and determine if we need to recurse into the
 * tree (READ_TREE_RECURSIVE) or skip it (0).
 */
ساکن صحیح try_update_sparse_directory(ثابت ساخت object_id *oid,
				       ساخت strbuf *بنیاد,
				       ثابت حرف *pathname,
				       صحیح overlay_منوال)
{
	ساخت strbuf dirpath = STRBUF_INIT;
	ساخت cache_اندراج *old;
	صحیح pos, نتیجہ = READ_درخت_RECURSIVE;

	اگر (!the_repository->اشاریہ->sparse_اشاریہ)
		واپس نتیجہ;

	strbuf_addbuf(&dirpath, بنیاد);
	strbuf_addstr(&dirpath, pathname);
	strbuf_addch(&dirpath, '/');

	pos = اشاریہ_name_pos_sparse(the_repository->اشاریہ,
				    dirpath.buf, dirpath.len);
	اگر (pos < 0)
		جاؤ cleanup;

	old = the_repository->اشاریہ->cache[pos];
	اگر (!S_ISSPARSEDIR(old->ce_منوال))
		جاؤ cleanup;

	اگر (oideq(oid, &old->oid)) {
		/* Tree content already matches; no need to descend. */
		نتیجہ = 0;
	} ورنہ اگر (!overlay_منوال) {
		/*
		 * In non-overlay mode (e.g., restore --staged), replace the
		 * sparse directory OID directly since files not present in
		 * the source tree should be removed anyway.
		 */
		oidcpy(&old->oid, oid);
		old->ce_flags |= CE_UPDATE;
		نتیجہ = 0;
	}

cleanup:
	strbuf_release(&dirpath);
	واپس نتیجہ;
}

ساکن صحیح update_some(ثابت ساخت object_id *oid, ساخت strbuf *بنیاد,
		       ثابت حرف *pathname, غیرعلامتی منوال, خالی *context)
{
	صحیح len;
	ساخت cache_اندراج *ce;
	صحیح pos;
	صحیح overlay_منوال = context ? *((صحیح *)context) : 1;

	اگر (S_ISDIR(منوال))
		واپس try_update_sparse_directory(oid, بنیاد, pathname,
						   overlay_منوال);

	len = بنیاد->len + لڑی_لمبائی(pathname);
	ce = make_خالی_خانے_cache_اندراج(the_repository->اشاریہ, len);
	oidcpy(&ce->oid, oid);
	حافظہ_نقل(ce->name, بنیاد->buf, بنیاد->len);
	حافظہ_نقل(ce->name + بنیاد->len, pathname, len - بنیاد->len);
	ce->ce_flags = create_ce_flags(0) | CE_UPDATE;
	ce->ce_namelen = len;
	ce->ce_منوال = create_ce_منوال(منوال);

	/*
	 * If the entry is the same as the current index, we can leave the old
	 * entry in place. Whether it is UPTODATE or not, checkout_entry will
	 * do the right thing.
	 */
	pos = اشاریہ_name_pos_sparse(the_repository->اشاریہ, ce->name, ce->ce_namelen);
	اگر (pos >= 0) {
		ساخت cache_اندراج *old = the_repository->اشاریہ->cache[pos];
		اگر (ce->ce_منوال == old->ce_منوال &&
		    !ce_intent_مقصد_جوڑو(old) &&
		    oideq(&ce->oid, &old->oid)) {
			old->ce_flags |= CE_UPDATE;
			discard_cache_اندراج(ce);
			واپس 0;
		}
	}

	جوڑو_اشاریہ_اندراج(the_repository->اشاریہ, ce,
			جوڑو_CACHE_OK_مقصد_جوڑو | جوڑو_CACHE_OK_مقصد_REPLACE);
	واپس 0;
}

ساکن صحیح read_درخت_some(ساخت درخت *درخت, ثابت ساخت pathspec *pathspec,
			  صحیح overlay_منوال)
{
	read_درخت(the_repository, درخت,
		  pathspec, update_some, &overlay_منوال);

	/* update the index with the given tree's info
	 * for all args, expanding wildcards, and exit
	 * with any non-zero return code.
	 */
	واپس 0;
}

ساکن صحیح skip_same_name(ثابت ساخت cache_اندراج *ce, صحیح pos)
{
	جب (++pos < the_repository->اشاریہ->cache_nr &&
	       !strcmp(the_repository->اشاریہ->cache[pos]->name, ce->name))
		; /* skip */
	واپس pos;
}

ساکن صحیح check_stage(صحیح stage, ثابت ساخت cache_اندراج *ce, صحیح pos,
		       صحیح overlay_منوال)
{
	جب (pos < the_repository->اشاریہ->cache_nr &&
	       !strcmp(the_repository->اشاریہ->cache[pos]->name, ce->name)) {
		اگر (ce_stage(the_repository->اشاریہ->cache[pos]) == stage)
			واپس 0;
		pos++;
	}
	اگر (!overlay_منوال)
		واپس 0;
	اگر (stage == 2)
		واپس error(_("path '%s' does not have our version"), ce->name);
	ورنہ
		واپس error(_("path '%s' does not have their version"), ce->name);
}

ساکن صحیح check_stages(غیرعلامتی stages, ثابت ساخت cache_اندراج *ce, صحیح pos)
{
	غیرعلامتی نظر_آیا = 0;
	ثابت حرف *name = ce->name;

	جب (pos < the_repository->اشاریہ->cache_nr) {
		ce = the_repository->اشاریہ->cache[pos];
		اگر (strcmp(name, ce->name))
			توڑو;
		نظر_آیا |= (1 << ce_stage(ce));
		pos++;
	}
	اگر ((stages & نظر_آیا) != stages)
		واپس error(_("path '%s' does not have all necessary versions"),
			     name);
	واپس 0;
}

ساکن صحیح checkout_stage(صحیح stage, ثابت ساخت cache_اندراج *ce, صحیح pos,
			  ثابت ساخت checkout *state, صحیح *nr_checkouts,
			  صحیح overlay_منوال)
{
	جب (pos < the_repository->اشاریہ->cache_nr &&
	       !strcmp(the_repository->اشاریہ->cache[pos]->name, ce->name)) {
		اگر (ce_stage(the_repository->اشاریہ->cache[pos]) == stage)
			واپس checkout_اندراج(the_repository->اشاریہ->cache[pos], state,
					      تہی, nr_checkouts);
		pos++;
	}
	اگر (!overlay_منوال) {
		unlink_اندراج(ce, تہی);
		واپس 0;
	}
	اگر (stage == 2)
		واپس error(_("path '%s' does not have our version"), ce->name);
	ورنہ
		واپس error(_("path '%s' does not have their version"), ce->name);
}

ساکن صحیح checkout_merged(صحیح pos, ثابت ساخت checkout *state,
			   صحیح *nr_checkouts, ساخت mem_pool *ce_mem_pool,
			   صحیح conflict_style)
{
	ساخت cache_اندراج *ce = the_repository->اشاریہ->cache[pos];
	ثابت حرف *راستہ = ce->name;
	mmfile_t ancestor, ours, theirs;
	شمار ll_ملاؤ_نتیجہ ملاؤ_حالت;
	صحیح حالت_2;
	ساخت object_id oid;
	mmfile_t نتیجہ_buf;
	ساخت object_id threeway[3];
	غیرعلامتی منوال = 0;
	ساخت ll_ملاؤ_options ll_opts = LL_ملاؤ_OPTIONS_INIT;
	صحیح renormalize = 0;

	memset(threeway, 0, حجم(threeway));
	جب (pos < the_repository->اشاریہ->cache_nr) {
		صحیح stage;
		stage = ce_stage(ce);
		اگر (!stage || strcmp(راستہ, ce->name))
			توڑو;
		oidcpy(&threeway[stage - 1], &ce->oid);
		اگر (stage == 2)
			منوال = create_ce_منوال(ce->ce_منوال);
		pos++;
		ce = the_repository->اشاریہ->cache[pos];
	}
	اگر (is_null_oid(&threeway[1]) || is_null_oid(&threeway[2]))
		واپس error(_("path '%s' does not have necessary versions"), راستہ);

	read_mmblob(&ancestor, the_repository->objects, &threeway[0]);
	read_mmblob(&ours, the_repository->objects, &threeway[1]);
	read_mmblob(&theirs, the_repository->objects, &threeway[2]);

	repo_config_get_bool(the_repository, "merge.renormalize", &renormalize);
	ll_opts.renormalize = renormalize;
	ll_opts.conflict_style = conflict_style;
	ملاؤ_حالت = ll_ملاؤ(&نتیجہ_buf, راستہ, &ancestor, "base",
				&ours, "ours", &theirs, "theirs",
				state->istate, &ll_opts);
	آزاد(ancestor.ptr);
	آزاد(ours.ptr);
	آزاد(theirs.ptr);
	اگر (ملاؤ_حالت == LL_ملاؤ_ثنائی_CONFLICT)
		warning("Cannot merge binary files: %s (%s vs. %s)",
			راستہ, "ours", "theirs");
	اگر (ملاؤ_حالت < 0 || !نتیجہ_buf.ptr) {
		آزاد(نتیجہ_buf.ptr);
		واپس error(_("path '%s': cannot merge"), راستہ);
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
	اگر (odb_write_object(the_repository->objects, نتیجہ_buf.ptr, نتیجہ_buf.سائز, OBJ_BLOB, &oid))
		die(_("Unable to add merge result for '%s'"), راستہ);
	آزاد(نتیجہ_buf.ptr);
	ce = make_transient_cache_اندراج(منوال, &oid, راستہ, 2, ce_mem_pool);
	اگر (!ce)
		die(_("make_cache_entry failed for path '%s'"), راستہ);
	حالت_2 = checkout_اندراج(ce, state, تہی, nr_checkouts);
	واپس حالت_2;
}

ساکن خالی mark_ce_for_checkout_overlay(ساخت cache_اندراج *ce,
					 حرف *ps_matched,
					 ثابت ساخت checkout_opts *opts)
{
	ce->ce_flags &= ~CE_MATCHED;
	اگر (!opts->ignore_skipworktree && ce_skip_worktree(ce))
		واپس;
	اگر (opts->ماخذ_درخت && !(ce->ce_flags & CE_UPDATE))
		/*
		 * "git checkout tree-ish -- path", but this entry
		 * is in the original index but is not in tree-ish
		 * or does not match the pathspec; it will not be
		 * checked out to the working tree.  We will not do
		 * anything to this entry at all.
		 */
		واپس;
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
	اگر (ce_راستہ_مطابقت(the_repository->اشاریہ, ce, &opts->pathspec, ps_matched))
		ce->ce_flags |= CE_MATCHED;
}

ساکن خالی mark_ce_for_checkout_no_overlay(ساخت cache_اندراج *ce,
					    حرف *ps_matched,
					    ثابت ساخت checkout_opts *opts)
{
	ce->ce_flags &= ~CE_MATCHED;
	اگر (!opts->ignore_skipworktree && ce_skip_worktree(ce))
		واپس;
	اگر (ce_راستہ_مطابقت(the_repository->اشاریہ, ce, &opts->pathspec, ps_matched)) {
		ce->ce_flags |= CE_MATCHED;
		اگر (opts->ماخذ_درخت && !(ce->ce_flags & CE_UPDATE))
			/*
			 * In overlay mode, but the path is not in
			 * tree-ish, which means we should remove it
			 * from the index and the working tree.
			 */
			ce->ce_flags |= CE_REMOVE | CE_WT_REMOVE;
	}
}

ساکن صحیح checkout_worktree(ثابت ساخت checkout_opts *opts,
			     ثابت ساخت branch_info *info)
{
	ساخت checkout state = CHECKOUT_INIT;
	صحیح nr_checkouts = 0, nr_unmerged = 0;
	صحیح errs = 0;
	صحیح pos;
	صحیح pc_workers, pc_حد;
	ساخت mem_pool ce_mem_pool;

	state.force = 1;
	state.refresh_cache = 1;
	state.istate = the_repository->اشاریہ;

	mem_pool_init(&ce_mem_pool, 0);
	get_parallel_checkout_configs(&pc_workers, &pc_حد);
	init_checkout_metadata(&state.meta, info->refname,
			       info->commit ? &info->commit->object.oid : &info->oid,
			       تہی);

	enable_delayed_checkout(&state);

	اگر (pc_workers > 1)
		init_parallel_checkout();

	برائے (pos = 0; pos < the_repository->اشاریہ->cache_nr; pos++) {
		ساخت cache_اندراج *ce = the_repository->اشاریہ->cache[pos];
		اگر (ce->ce_flags & CE_MATCHED) {
			اگر (!ce_stage(ce)) {
				errs |= checkout_اندراج(ce, &state,
						       تہی, &nr_checkouts);
				جاری;
			}
			اگر (opts->writeout_stage)
				errs |= checkout_stage(opts->writeout_stage,
						       ce, pos,
						       &state,
						       &nr_checkouts, opts->overlay_منوال);
			ورنہ اگر (opts->ملاؤ)
				errs |= checkout_merged(pos, &state,
							&nr_unmerged,
							&ce_mem_pool,
							opts->conflict_style);
			pos = skip_same_name(ce, pos) - 1;
		}
	}
	اگر (pc_workers > 1)
		errs |= تسلسل_parallel_checkout(&state, pc_workers, pc_حد,
					      تہی, تہی);
	mem_pool_discard(&ce_mem_pool, should_validate_cache_entries());
	remove_marked_cache_entries(the_repository->اشاریہ, 1);
	remove_scheduled_dirs();
	errs |= اختتام_delayed_checkout(&state, opts->show_پیش_رفت);

	اگر (opts->گنتی_checkout_paths) {
		اگر (nr_unmerged)
			fprintf_ln(stderr, Q_("Recreated %d merge conflict",
					      "Recreated %d merge conflicts",
					      nr_unmerged),
				   nr_unmerged);
		اگر (opts->ماخذ_درخت)
			fprintf_ln(stderr, Q_("Updated %d path from %s",
					      "Updated %d paths from %s",
					      nr_checkouts),
				   nr_checkouts,
				   repo_ڈھونڈو_unique_abbrev(the_repository, &opts->ماخذ_درخت->object.oid,
							   DEFAULT_ABBREV));
		ورنہ اگر (!nr_unmerged || nr_checkouts)
			fprintf_ln(stderr, Q_("Updated %d path from the index",
					      "Updated %d paths from the index",
					      nr_checkouts),
				   nr_checkouts);
	}

	واپس errs;
}

ساکن صحیح checkout_paths(ثابت ساخت checkout_opts *opts,
			  ثابت ساخت branch_info *new_branch_info)
{
	صحیح pos;
	ساکن حرف *ps_matched;
	ساخت object_id rev;
	ساخت commit *سر;
	صحیح errs = 0;
	ساخت تالا_file تالا_file = تالا_INIT;
	صحیح checkout_اشاریہ;

	trace2_cmd_منوال(opts->patch_منوال ? "patch" : "path");

	اگر (opts->track != BRANCH_TRACK_UNSPECIFIED)
		die(_("'%s' cannot be used with updating paths"), "--track");

	اگر (opts->new_branch_log)
		die(_("'%s' cannot be used with updating paths"), "-l");

	اگر (opts->ignore_unmerged && opts->patch_منوال)
		die(_("'%s' cannot be used with updating paths"),
		    opts->ignore_unmerged_opt);

	اگر (opts->force_detach)
		die(_("'%s' cannot be used with updating paths"), "--detach");

	اگر (opts->ملاؤ && opts->patch_منوال)
		die(_("options '%s' and '%s' cannot be used together"), "--merge", "--patch");

	اگر (opts->ignore_unmerged && opts->ملاؤ)
		die(_("options '%s' and '%s' cannot be used together"),
		    opts->ignore_unmerged_opt, "-m");

	اگر (opts->new_branch)
		die(_("Cannot update paths and switch to branch '%s' at the same time."),
		    opts->new_branch);

	اگر (!opts->checkout_worktree && !opts->checkout_اشاریہ)
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
	اگر (!opts->accept_ref && opts->checkout_اشاریہ) {
		اگر (opts->writeout_stage)
			die(_("'%s' or '%s' cannot be used with %s"),
			    "--ours", "--theirs", "--staged");

		اگر (opts->ملاؤ)
			die(_("'%s' or '%s' cannot be used with %s"),
			    "--merge", "--conflict", "--staged");
	}

	/*
	 * recreating unmerged index entries and writing out data from
	 * unmerged index entries would make no sense when checking out
	 * of a tree-ish.
	 */
	اگر ((opts->ملاؤ || opts->writeout_stage) && opts->ماخذ_درخت)
		die(_("'%s', '%s', or '%s' cannot be used when checking out of a tree"),
		    "--merge", "--ours", "--theirs");

	اگر (opts->patch_منوال) {
		شمار جوڑو_p_منوال patch_منوال;
		ساخت interactive_options interactive_opts = {
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
		اگر (rev && new_branch_info->commit && strcmp(rev, "HEAD"))
			rev = oid_مقصد_hex_r(rev_oid, &new_branch_info->commit->object.oid);

		اگر (opts->checkout_اشاریہ && opts->checkout_worktree)
			patch_منوال = جوڑو_P_CHECKOUT;
		ورنہ اگر (opts->checkout_اشاریہ && !opts->checkout_worktree)
			patch_منوال = جوڑو_P_RESET;
		ورنہ اگر (!opts->checkout_اشاریہ && opts->checkout_worktree)
			patch_منوال = جوڑو_P_WORKTREE;
		ورنہ
			BUG("either flag must have been set, worktree=%d, index=%d",
			    opts->checkout_worktree, opts->checkout_اشاریہ);
		واپس !!تسلسل_جوڑو_p(the_repository, patch_منوال, &interactive_opts,
				   rev, &opts->pathspec, 0);
	}

	repo_hold_locked_اشاریہ(the_repository, &تالا_file, تالا_DIE_ON_ERROR);
	اگر (repo_read_اشاریہ_preload(the_repository, &opts->pathspec, 0) < 0)
		واپس error(_("index file corrupt"));

	اگر (opts->ماخذ_درخت)
		read_درخت_some(opts->ماخذ_درخت, &opts->pathspec,
			       opts->overlay_منوال);
	اگر (opts->ملاؤ)
		unmerge_اشاریہ(the_repository->اشاریہ, &opts->pathspec, CE_MATCHED);

	ps_matched = xcalloc(opts->pathspec.nr, 1);

	/*
	 * Make sure all pathspecs participated in locating the paths
	 * to be checked out.
	 */
	برائے (pos = 0; pos < the_repository->اشاریہ->cache_nr; pos++)
		اگر (opts->overlay_منوال)
			mark_ce_for_checkout_overlay(the_repository->اشاریہ->cache[pos],
						     ps_matched,
						     opts);
		ورنہ
			mark_ce_for_checkout_no_overlay(the_repository->اشاریہ->cache[pos],
							ps_matched,
							opts);

	اگر (report_راستہ_error(ps_matched, &opts->pathspec)) {
		آزاد(ps_matched);
		واپس 1;
	}
	آزاد(ps_matched);

	/* Any unmerged paths? */
	برائے (pos = 0; pos < the_repository->اشاریہ->cache_nr; pos++) {
		ثابت ساخت cache_اندراج *ce = the_repository->اشاریہ->cache[pos];
		اگر (ce->ce_flags & CE_MATCHED) {
			اگر (!ce_stage(ce))
				جاری;
			اگر (opts->ignore_unmerged) {
				اگر (!opts->quiet)
					warning(_("path '%s' is unmerged"), ce->name);
			} ورنہ اگر (opts->writeout_stage) {
				errs |= check_stage(opts->writeout_stage, ce, pos, opts->overlay_منوال);
			} ورنہ اگر (opts->ملاؤ) {
				errs |= check_stages((1<<2) | (1<<3), ce, pos);
			} ورنہ {
				errs = 1;
				error(_("path '%s' is unmerged"), ce->name);
			}
			pos = skip_same_name(ce, pos) - 1;
		}
	}
	اگر (errs)
		واپس 1;

	/* Now we are committed to check them out */
	اگر (opts->checkout_worktree)
		errs |= checkout_worktree(opts, new_branch_info);
	ورنہ
		remove_marked_cache_entries(the_repository->اشاریہ, 1);

	/*
	 * Allow updating the index when checking out from the index.
	 * This is to save new stat info.
	 */
	اگر (opts->checkout_worktree && !opts->checkout_اشاریہ && !opts->ماخذ_درخت)
		checkout_اشاریہ = 1;
	ورنہ
		checkout_اشاریہ = opts->checkout_اشاریہ;

	اگر (checkout_اشاریہ) {
		اگر (write_locked_اشاریہ(the_repository->اشاریہ, &تالا_file, COMMIT_تالا))
			die(_("unable to write new index file"));
	} ورنہ {
		/*
		 * NEEDSWORK: if --worktree is not specified, we
		 * should save stat info of checked out files in the
		 * index to avoid the next (potentially costly)
		 * refresh. But it's a bit tricker to do...
		 */
		rollback_تالا_file(&تالا_file);
	}

	refs_read_ref_بھرا(get_main_ref_store(the_repository), "HEAD", 0,
			   &rev, تہی);
	سر = lookup_commit_reference_gently(the_repository, &rev, 1);

	errs |= post_checkout_hook(سر, سر, 0);
	واپس errs;
}

ساکن خالی show_local_changes(ساخت object *سر,
			       ثابت ساخت diff_options *opts)
{
	ساخت rev_info rev;
	/* I think we want full paths, even if we're in a subdirectory. */
	repo_init_revisions(the_repository, &rev, تہی);
	rev.diffopt.flags = opts->flags;
	rev.diffopt.output_format |= DIFF_FORMAT_NAME_حالت;
	rev.diffopt.flags.recursive = 1;
	diff_تیاری_مکمل(&rev.diffopt);
	جوڑو_pending_object(&rev, سر, تہی);
	تسلسل_diff_اشاریہ(&rev, 0);
	release_revisions(&rev);
}

ساکن خالی describe_detached_سر(ثابت حرف *پیغام, ساخت commit *commit)
{
	ساخت strbuf sb = STRBUF_INIT;

	اگر (!repo_parse_commit(the_repository, commit))
		pp_commit_easy(CMIT_FMT_ONELINE, commit, &sb);
	اگر (print_sha1_ellipsis()) {
		fprintf(stderr, "%s %s... %s\n", پیغام,
			repo_ڈھونڈو_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV),
			sb.buf);
	} ورنہ {
		fprintf(stderr, "%s %s %s\n", پیغام,
			repo_ڈھونڈو_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV),
			sb.buf);
	}
	strbuf_release(&sb);
}

ساکن صحیح reset_درخت(ساخت درخت *درخت, ثابت ساخت checkout_opts *o,
		      صحیح worktree, صحیح *writeout_error,
		      ساخت branch_info *info)
{
	ساخت unpack_trees_options opts;
	ساخت درخت_desc درخت_desc;

	memset(&opts, 0, حجم(opts));
	opts.سر_idx = -1;
	opts.update = worktree;
	opts.skip_unmerged = !worktree;
	opts.reset = o->force ? UNPACK_RESET_OVERWRITE_UNTRACKED :
				UNPACK_RESET_PROTECT_UNTRACKED;
	opts.preserve_ignored = (!o->force && !o->overwrite_ignore);
	opts.ملاؤ = 1;
	opts.fn = oneway_ملاؤ;
	opts.verbose_update = o->show_پیش_رفت;
	opts.src_اشاریہ = the_repository->اشاریہ;
	opts.dst_اشاریہ = the_repository->اشاریہ;
	init_checkout_metadata(&opts.meta, info->refname,
			       info->commit ? &info->commit->object.oid : null_oid(the_hash_algo),
			       تہی);
	اگر (repo_parse_درخت(the_repository, درخت) < 0)
		واپس 128;
	init_درخت_desc(&درخت_desc, &درخت->object.oid, درخت->بفر, درخت->سائز);
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
		واپس 0;
	طےشدہ:
		واپس 128;
	}
}

ساکن خالی تیاری_branch_راستہ(ساخت branch_info *branch)
{
	ساخت strbuf buf = STRBUF_INIT;

	/*
	 * If this is a ref, resolve it; otherwise, look up the OID for our
	 * expression.  Failure here is okay.
	 */
	اگر (!repo_dwim_ref(the_repository, branch->name, لڑی_لمبائی(branch->name),
			   &branch->oid, &branch->refname, 0))
		repo_get_oid_committish(the_repository, branch->name, &branch->oid);

	copy_branchname(the_repository, &buf, branch->name, INTERPRET_BRANCH_LOCAL);
	اگر (strcmp(buf.buf, branch->name)) {
		آزاد(branch->name);
		branch->name = xstrdup(buf.buf);
	}
	strbuf_splice(&buf, 0, 0, "refs/heads/", 11);
	آزاد(branch->راستہ);
	branch->راستہ = strbuf_detach(&buf, تہی);
}

ساکن خالی init_topts(ساخت unpack_trees_options *topts,
		       صحیح show_پیش_رفت, صحیح overwrite_ignore,
		       bool quiet)
{
	memset(topts, 0, حجم(*topts));
	topts->سر_idx = -1;
	topts->src_اشاریہ = the_repository->اشاریہ;
	topts->dst_اشاریہ = the_repository->اشاریہ;

	تیاری_unpack_trees_porcelain(topts, "checkout");

	topts->initial_checkout = is_اشاریہ_unborn(the_repository->اشاریہ);
	topts->update = 1;
	topts->ملاؤ = 1;
	topts->quiet = quiet;
	topts->verbose_update = show_پیش_رفت;
	topts->fn = twoway_ملاؤ;
	topts->preserve_ignored = !overwrite_ignore;
}

ساکن صحیح ملاؤ_working_درخت(ثابت ساخت checkout_opts *opts,
			      ساخت branch_info *old_branch_info,
			      ساخت branch_info *new_branch_info,
			      bool quiet,
			      صحیح *writeout_error)
{
	صحیح ret;
	ساخت تالا_file تالا_file = تالا_INIT;
	ساخت درخت *new_درخت;

	repo_hold_locked_اشاریہ(the_repository, &تالا_file, تالا_DIE_ON_ERROR);
	اگر (repo_read_اشاریہ_preload(the_repository, تہی, 0) < 0) {
		rollback_تالا_file(&تالا_file);
		واپس error(_("index file corrupt"));
	}

	resolve_undo_clear_اشاریہ(the_repository->اشاریہ);
	اگر (opts->new_orphan_branch && opts->orphan_from_خالی_خانے_درخت) {
		اگر (new_branch_info->commit)
			BUG("'switch --orphan' should never accept a commit as starting point");
		new_درخت = repo_parse_درخت_بالواسطہ(the_repository,
						    the_hash_algo->خالی_خانے_درخت);
		اگر (!new_درخت)
			BUG("unable to read empty tree");
	} ورنہ {
		new_درخت = repo_get_commit_درخت(the_repository,
						new_branch_info->commit);
		اگر (!new_درخت) {
			rollback_تالا_file(&تالا_file);
			واپس error(_("unable to read tree (%s)"),
				     oid_مقصد_hex(&new_branch_info->commit->object.oid));
		}
	}
	اگر (opts->discard_changes) {
		ret = reset_درخت(new_درخت, opts, 1, writeout_error, new_branch_info);
		اگر (ret) {
			rollback_تالا_file(&تالا_file);
			واپس ret;
		}
	} ورنہ {
		ساخت درخت_desc trees[2];
		ساخت درخت *درخت;
		ساخت unpack_trees_options topts;
		ثابت ساخت object_id *old_commit_oid;

		refresh_اشاریہ(the_repository->اشاریہ, REFRESH_QUIET, تہی, تہی, تہی);

		اگر (unmerged_اشاریہ(the_repository->اشاریہ)) {
			rollback_تالا_file(&تالا_file);
			error(_("you need to resolve your current index first"));
			واپس 1;
		}

		/* 2-way merge to the new branch */
		init_topts(&topts, opts->show_پیش_رفت,
			   opts->overwrite_ignore, quiet);
		init_checkout_metadata(&topts.meta, new_branch_info->refname,
				       new_branch_info->commit ?
				       &new_branch_info->commit->object.oid :
				       &new_branch_info->oid, تہی);

		old_commit_oid = old_branch_info->commit ?
			&old_branch_info->commit->object.oid :
			the_hash_algo->خالی_خانے_درخت;
		درخت = repo_parse_درخت_بالواسطہ(the_repository,
						old_commit_oid);
		اگر (!درخت)
			die(_("unable to parse commit %s"),
				oid_مقصد_hex(old_commit_oid));

		init_درخت_desc(&trees[0], &درخت->object.oid,
			       درخت->بفر, درخت->سائز);
		اگر (repo_parse_درخت(the_repository, new_درخت) < 0)
			die(تہی);
		درخت = new_درخت;
		init_درخت_desc(&trees[1], &درخت->object.oid,
			       درخت->بفر, درخت->سائز);

		ret = unpack_trees(2, trees, &topts);
		clear_unpack_trees_porcelain(&topts);
		اگر (ret == -1) {
			rollback_تالا_file(&تالا_file);
			واپس ملاؤ_WORKING_درخت_UNPACK_FAILED;
		}
	}

	اگر (!cache_درخت_fully_valid(the_repository->اشاریہ))
		cache_درخت_update(the_repository->اشاریہ, WRITE_درخت_SILENT | WRITE_درخت_REPAIR);

	اگر (write_locked_اشاریہ(the_repository->اشاریہ, &تالا_file, COMMIT_تالا))
		die(_("unable to write new index file"));

	اگر (!opts->discard_changes && !opts->quiet && new_branch_info->commit)
		show_local_changes(&new_branch_info->commit->object, &opts->diff_options);

	واپس 0;
}

ساکن خالی report_tracking(ساخت branch_info *new_branch_info)
{
	ساخت strbuf sb = STRBUF_INIT;
	ساخت branch *branch = branch_get(new_branch_info->name);

	اگر (!format_tracking_info(branch, &sb, AHEAD_BEHIND_بھرا, 1))
		واپس;
	fputs(sb.buf, stdout);
	strbuf_release(&sb);
}

ساکن خالی update_refs_for_switch(ثابت ساخت checkout_opts *opts,
				   ساخت branch_info *old_branch_info,
				   ساخت branch_info *new_branch_info)
{
	ساخت strbuf پیغام = STRBUF_INIT;
	ثابت حرف *old_desc, *reflog_پیغام;
	اگر (opts->new_branch) {
		اگر (opts->new_orphan_branch) {
			شمار log_refs_config log_all_ref_updates = LOG_REFS_UNSET;
			ثابت حرف *قدر;
			حرف *refname;

			اگر (!repo_config_get_string_tmp(the_repository, "core.logallrefupdates", &قدر))
				log_all_ref_updates = refs_parse_log_all_ref_updates_config(قدر);

			refname = mkpathdup("refs/heads/%s", opts->new_orphan_branch);
			اگر (opts->new_branch_log &&
			    !should_autocreate_reflog(log_all_ref_updates, refname)) {
				صحیح ret;
				ساخت strbuf err = STRBUF_INIT;

				ret = refs_create_reflog(get_main_ref_store(the_repository),
							 refname, &err);
				اگر (ret) {
					fprintf(stderr, _("Can not do reflog for '%s': %s\n"),
						opts->new_orphan_branch, err.buf);
					strbuf_release(&err);
					آزاد(refname);
					واپس;
				}
				strbuf_release(&err);
			}
			آزاد(refname);
		}
		ورنہ
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
		تیاری_branch_راستہ(new_branch_info);
	}

	old_desc = old_branch_info->name;
	اگر (!old_desc && old_branch_info->commit)
		old_desc = oid_مقصد_hex(&old_branch_info->commit->object.oid);

	reflog_پیغام = getenv("GIT_REFLOG_ACTION");
	اگر (!reflog_پیغام)
		strbuf_addf(&پیغام, "checkout: moving from %s to %s",
			old_desc ? old_desc : "(invalid)", new_branch_info->name);
	ورنہ
		strbuf_insertstr(&پیغام, 0, reflog_پیغام);

	اگر (!strcmp(new_branch_info->name, "HEAD") && !new_branch_info->راستہ && !opts->force_detach) {
		/* Nothing to do. */
	} ورنہ اگر (opts->force_detach || !new_branch_info->راستہ) {	/* No longer on any branch. */
		refs_update_ref(get_main_ref_store(the_repository), پیغام.buf,
				"HEAD", &new_branch_info->commit->object.oid,
				تہی,
				REF_NO_DEREF, UPDATE_REFS_DIE_ON_ERR);
		اگر (!opts->quiet) {
			اگر (old_branch_info->راستہ &&
			    advice_enabled(ADVICE_DETACHED_سر) && !opts->force_detach)
				detach_advice(new_branch_info->name);
			describe_detached_سر(_("HEAD is now at"), new_branch_info->commit);
		}
	} ورنہ اگر (new_branch_info->راستہ) {	/* Switch branches. */
		اگر (refs_update_symref(get_main_ref_store(the_repository), "HEAD", new_branch_info->راستہ, پیغام.buf) < 0)
			die(_("unable to update HEAD"));
		اگر (!opts->quiet) {
			اگر (old_branch_info->راستہ && !strcmp(new_branch_info->راستہ, old_branch_info->راستہ)) {
				اگر (opts->new_branch_force)
					fprintf(stderr, _("Reset branch '%s'\n"),
						new_branch_info->name);
				ورنہ
					fprintf(stderr, _("Already on '%s'\n"),
						new_branch_info->name);
			} ورنہ اگر (opts->new_branch) {
				اگر (opts->branch_exists)
					fprintf(stderr, _("Switched to and reset branch '%s'\n"), new_branch_info->name);
				ورنہ
					fprintf(stderr, _("Switched to a new branch '%s'\n"), new_branch_info->name);
			} ورنہ {
				fprintf(stderr, _("Switched to branch '%s'\n"),
					new_branch_info->name);
			}
		}
		اگر (old_branch_info->راستہ && old_branch_info->name) {
			اگر (!refs_ref_exists(get_main_ref_store(the_repository), old_branch_info->راستہ) && refs_reflog_exists(get_main_ref_store(the_repository), old_branch_info->راستہ))
				refs_delete_reflog(get_main_ref_store(the_repository),
						   old_branch_info->راستہ);
		}
	}
	remove_branch_state(the_repository, !opts->quiet);
	strbuf_release(&پیغام);
	اگر (!opts->quiet &&
	    !opts->force_detach &&
	    (new_branch_info->راستہ || !strcmp(new_branch_info->name, "HEAD")))
		report_tracking(new_branch_info);
}

ساکن صحیح جوڑو_pending_uninteresting_ref(ثابت ساخت reference *ref, خالی *cb_ڈیٹا)
{
	جوڑو_pending_oid(cb_ڈیٹا, ref->name, ref->oid, UNINTERESTING);
	واپس 0;
}

ساکن خالی describe_one_orphan(ساخت strbuf *sb, ساخت commit *commit)
{
	strbuf_addstr(sb, "  ");
	strbuf_جوڑو_unique_abbrev(sb, &commit->object.oid, DEFAULT_ABBREV);
	strbuf_addch(sb, ' ');
	اگر (!repo_parse_commit(the_repository, commit))
		pp_commit_easy(CMIT_FMT_ONELINE, commit, sb);
	strbuf_addch(sb, '\n');
}

#تعریف_کرو ORPHAN_CUTOFF 4
ساکن خالی suggest_reattach(ساخت commit *commit, ساخت rev_info *revs)
{
	ساخت commit *c, *آخری = تہی;
	ساخت strbuf sb = STRBUF_INIT;
	صحیح گمشدہ = 0;
	جب ((c = get_revision(revs)) != تہی) {
		اگر (گمشدہ < ORPHAN_CUTOFF)
			describe_one_orphan(&sb, c);
		آخری = c;
		گمشدہ++;
	}
	اگر (ORPHAN_CUTOFF < گمشدہ) {
		صحیح مزید = گمشدہ - ORPHAN_CUTOFF;
		اگر (مزید == 1)
			describe_one_orphan(&sb, آخری);
		ورنہ
			strbuf_addf(&sb, _(" ... and %d more.\n"), مزید);
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
		گمشدہ),
		گمشدہ,
		sb.buf);
	strbuf_release(&sb);

	اگر (advice_enabled(ADVICE_DETACHED_سر))
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
			گمشدہ),
			repo_ڈھونڈو_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV));
}

/*
 * We are about to leave commit that was at the tip of a detached
 * HEAD.  If it is not reachable from any ref, this is the last chance
 * for the user to do so without resorting to reflog.
 */
ساکن خالی orphaned_commit_warning(ساخت commit *old_commit, ساخت commit *new_commit)
{
	ساخت rev_info revs;
	ساخت object *object = &old_commit->object;

	repo_init_revisions(the_repository, &revs, تہی);
	تیاری_revisions(0, تہی, &revs, تہی);

	object->flags &= ~UNINTERESTING;
	جوڑو_pending_object(&revs, object, oid_مقصد_hex(&object->oid));

	refs_for_each_ref(get_main_ref_store(the_repository),
			  جوڑو_pending_uninteresting_ref, &revs);
	اگر (new_commit)
		جوڑو_pending_oid(&revs, "HEAD",
				&new_commit->object.oid,
				UNINTERESTING);

	اگر (prepare_revision_walk(&revs))
		die(_("internal error in revision walk"));
	اگر (!(old_commit->object.flags & UNINTERESTING))
		suggest_reattach(old_commit, &revs);
	ورنہ
		describe_detached_سر(_("Previous HEAD position was"), old_commit);

	/* Clean up objects used, as they will be reused. */
	repo_clear_commit_marks(the_repository, ALL_REV_FLAGS);
	release_revisions(&revs);
}

ساکن صحیح switch_branches(ثابت ساخت checkout_opts *opts,
			   ساخت branch_info *new_branch_info)
{
	صحیح ret = 0;
	ساخت branch_info old_branch_info = { 0 };
	ساخت object_id rev;
	صحیح flag, writeout_error = 0;
	صحیح do_ملاؤ = 1;
	صحیح created_autostash = 0;
	شمار stash_apply_نتیجہ autostash_res = STASH_APPLY_CLEAN;
	ساخت strbuf old_commit_shortname = STRBUF_INIT;
	ساخت strbuf autostash_پیغام = STRBUF_INIT;
	ثابت حرف *stash_label_بنیاد = تہی;

	trace2_cmd_منوال("branch");

	memset(&old_branch_info, 0, حجم(old_branch_info));
	old_branch_info.راستہ = refs_resolve_refdup(get_main_ref_store(the_repository),
						   "HEAD", 0, &rev, &flag);
	اگر (old_branch_info.راستہ)
		old_branch_info.commit = lookup_commit_reference_gently(the_repository, &rev, 1);
	اگر (!(flag & REF_ISSYMREF))
		FREE_AND_NULL(old_branch_info.راستہ);

	اگر (old_branch_info.راستہ) {
		ثابت حرف *ثابت سابقہ = "refs/heads/";
		ثابت حرف *p;
		اگر (skip_سابقہ(old_branch_info.راستہ, سابقہ, &p))
			old_branch_info.name = xstrdup(p);
	}

	اگر (opts->new_orphan_branch && opts->orphan_from_خالی_خانے_درخت) {
		اگر (new_branch_info->name)
			BUG("'switch --orphan' should never accept a commit as starting point");
		new_branch_info->commit = تہی;
		new_branch_info->name = xstrdup("(empty)");
		do_ملاؤ = 1;
	}

	اگر (!new_branch_info->name) {
		new_branch_info->name = xstrdup("HEAD");
		new_branch_info->commit = old_branch_info.commit;
		اگر (!new_branch_info->commit)
			die(_("You are on a branch yet to be born"));
		parse_commit_or_die(new_branch_info->commit);

		اگر (opts->only_ملاؤ_on_switching_branches)
			do_ملاؤ = 0;
	}

	اگر (old_branch_info.name) {
		stash_label_بنیاد = old_branch_info.name;
	} ورنہ اگر (old_branch_info.commit) {
		strbuf_جوڑو_unique_abbrev(&old_commit_shortname,
					 &old_branch_info.commit->object.oid,
					 DEFAULT_ABBREV);
		stash_label_بنیاد = old_commit_shortname.buf;
	}

	اگر (do_ملاؤ) {
		ret = ملاؤ_working_درخت(opts, &old_branch_info, new_branch_info,
					 opts->ملاؤ, &writeout_error);
		اگر (ret == ملاؤ_WORKING_درخت_UNPACK_FAILED && opts->ملاؤ) {
			strbuf_addf(&autostash_پیغام,
				    "autostash while switching to '%s'",
				    new_branch_info->name);
			create_autostash_ref(the_repository,
					     "CHECKOUT_AUTOSTASH_HEAD",
					     autostash_پیغام.buf, true);
			created_autostash = 1;
			ret = ملاؤ_working_درخت(opts, &old_branch_info, new_branch_info,
						 false, &writeout_error);
		}
		اگر (created_autostash) {
			اگر (opts->conflict_style >= 0) {
				ساخت strbuf cfg = STRBUF_INIT;
				strbuf_addf(&cfg, "merge.conflictStyle=%s",
					    conflict_style_name(opts->conflict_style));
				git_config_ڈالو_parameter(cfg.buf);
				strbuf_release(&cfg);
			}
			autostash_res = apply_autostash_ref(the_repository,
				    "CHECKOUT_AUTOSTASH_HEAD",
				    new_branch_info->name,
				    "local",
				    stash_label_بنیاد,
				    autostash_پیغام.buf);
		}
		اگر (ret) {
			branch_info_release(&old_branch_info);
			strbuf_release(&old_commit_shortname);
			strbuf_release(&autostash_پیغام);
			واپس ret < 0 ? 1 : ret;
		}
	}

	اگر (!opts->quiet && !old_branch_info.راستہ && old_branch_info.commit && new_branch_info->commit != old_branch_info.commit)
		orphaned_commit_warning(old_branch_info.commit, new_branch_info->commit);

	اگر (autostash_res == STASH_APPLY_CONFLICT && !opts->quiet)
		fputc('\n', stderr);
	update_refs_for_switch(opts, &old_branch_info, new_branch_info);

	اگر (created_autostash) {
		discard_اشاریہ(the_repository->اشاریہ);
		اگر (repo_read_اشاریہ(the_repository) < 0)
			die(_("index file corrupt"));

		اگر (!opts->quiet && new_branch_info->commit) {
			لکھو(_("The following paths have local changes:\n"));
			show_local_changes(&new_branch_info->commit->object,
					   &opts->diff_options);
		}
	}

	ret = post_checkout_hook(old_branch_info.commit, new_branch_info->commit, 1);
	branch_info_release(&old_branch_info);
	strbuf_release(&old_commit_shortname);
	strbuf_release(&autostash_پیغام);

	واپس ret || writeout_error;
}

ساکن صحیح git_checkout_config(ثابت حرف *var, ثابت حرف *قدر,
			       ثابت ساخت config_context *ctx, خالی *cb)
{
	ساخت checkout_opts *opts = cb;

	اگر (!strcmp(var, "diff.ignoresubmodules")) {
		اگر (!قدر)
			واپس config_error_nonbool(var);
		handle_ignore_submodules_arg(&opts->diff_options, قدر);
		واپس 0;
	}
	اگر (!strcmp(var, "checkout.guess")) {
		opts->dwim_new_local_branch = git_config_bool(var, قدر);
		واپس 0;
	}

	اگر (starts_with(var, "submodule."))
		واپس git_default_submodule_config(var, قدر, تہی);

	واپس git_xmerge_config(var, قدر, ctx, تہی);
}

ساکن خالی تیاری_new_branch_info_and_ماخذ_درخت(
	ساخت branch_info *new_branch_info,
	ساخت checkout_opts *opts,
	ساخت object_id *rev,
	ثابت حرف *arg)
{
	ساخت درخت **ماخذ_درخت = &opts->ماخذ_درخت;
	ساخت object_id branch_rev;

	/* treat '@' as a shortcut for 'HEAD' */
	new_branch_info->name = !strcmp(arg, "@") ? xstrdup("HEAD") :
						    xstrdup(arg);
	تیاری_branch_راستہ(new_branch_info);

	اگر (!check_refname_format(new_branch_info->راستہ, 0) &&
	    !refs_read_ref(get_main_ref_store(the_repository), new_branch_info->راستہ, &branch_rev))
		oidcpy(rev, &branch_rev);
	ورنہ
		/* not an existing branch */
		FREE_AND_NULL(new_branch_info->راستہ);

	new_branch_info->commit = lookup_commit_reference_gently(the_repository, rev, 1);
	اگر (!new_branch_info->commit) {
		/* not a commit */
		*ماخذ_درخت = repo_parse_درخت_بالواسطہ(the_repository, rev);
		اگر (!*ماخذ_درخت)
			die(_("unable to read tree (%s)"), oid_مقصد_hex(rev));
	} ورنہ {
		parse_commit_or_die(new_branch_info->commit);
		*ماخذ_درخت = repo_get_commit_درخت(the_repository,
						    new_branch_info->commit);
		اگر (!*ماخذ_درخت)
			die(_("unable to read tree (%s)"),
			    oid_مقصد_hex(&new_branch_info->commit->object.oid));
	}
}


شمار checkout_command {
	CHECKOUT_CHECKOUT = 1,
	CHECKOUT_SWITCH = 2,
	CHECKOUT_RESTORE = 3,
};

ساکن خالی advise_disambiguating_remotes(شمار checkout_command which_command,
					  ثابت حرف *branch,
					  ثابت ساخت string_list *matched_remote_نام)
{
	ثابت حرف *cmdname;
	ساخت string_list_شے *شے;

	انتخاب (which_command) {
	حالت CHECKOUT_CHECKOUT:
		cmdname = "checkout";
		توڑو;
	حالت CHECKOUT_SWITCH:
		cmdname = "switch";
		توڑو;
	طےشدہ:
		BUG("command <%d> should not reach advise_disambiguating_remotes",
		    which_command);
		توڑو;
	}

	advise(_("Branch name '%s' appears in multiple remotes:"), branch);
	for_each_string_list_شے(شے, matched_remote_نام) {
		advise(_("  %s"), شے->string);
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

ساکن حرف *parse_remote_branch(ثابت حرف *arg,
				 ساخت object_id *rev,
				 صحیح could_be_checkout_paths,
				 شمار checkout_command which_command)
{
	صحیح عدد_matches = 0;
	ساخت string_list matched_remote_نام = STRING_LIST_INIT_DUP;

	حرف *remote = unique_tracking_name(arg, rev, &عدد_matches,
					    &matched_remote_نام);

	اگر (remote && could_be_checkout_paths) {
		die(_("'%s' could be both a local file and a tracking branch.\n"
			"Please use -- (and optionally --no-guess) to disambiguate"),
		    arg);
	}

	اگر (!remote && عدد_matches > 1) {
		اگر (advice_enabled(ADVICE_CHECKOUT_AMBIGUOUS_REMOTE_BRANCH_NAME))
			advise_disambiguating_remotes(which_command, arg,
						      &matched_remote_نام);
		die(_("'%s' matched multiple (%d) remote tracking branches"),
		    arg, عدد_matches);
	}

	string_list_clear(&matched_remote_نام, 0);

	واپس remote;
}

ساکن صحیح parse_branchname_arg(صحیح argc, ثابت حرف **argv,
				صحیح dwim_new_local_branch_ok,
				شمار checkout_command which_command,
				ساخت branch_info *new_branch_info,
				ساخت checkout_opts *opts,
				ساخت object_id *rev)
{
	ثابت حرف **new_branch = &opts->new_branch;
	صحیح argcount = 0;
	ثابت حرف *arg;
	حرف *remote = تہی;
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
		واپس 0;

	اگر (!opts->accept_pathspec) {
		اگر (argc > 1)
			die(_("only one reference expected"));
		has_dash_dash = 1; /* helps disambiguate */
	}

	arg = argv[0];
	dash_dash_pos = -1;
	برائے (i = 0; i < argc; i++) {
		اگر (opts->accept_pathspec && !strcmp(argv[i], "--")) {
			dash_dash_pos = i;
			توڑو;
		}
	}
	اگر (dash_dash_pos == 0)
		واپس 1; /* case (2) */
	ورنہ اگر (dash_dash_pos == 1)
		has_dash_dash = 1; /* case (3) or (1) */
	ورنہ اگر (dash_dash_pos >= 2)
		die(_("only one reference expected, %d given."), dash_dash_pos);
	opts->گنتی_checkout_paths = !opts->quiet && !has_dash_dash;

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
			check_filename(opts->سابقہ, arg);

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
			} ورنہ {
				recover_with_dwim = 0;
			}
		}

		اگر (!recover_with_dwim) {
			اگر (has_dash_dash)
				die(_("invalid reference: %s"), arg);
			واپس argcount;
		}
	}

	/* we can't end up being in (2) anymore, eat the argument */
	argcount++;
	argv++;
	argc--;

	تیاری_new_branch_info_and_ماخذ_درخت(new_branch_info, opts, rev, arg);

	اگر (!opts->ماخذ_درخت)                   /* case (1): want a tree */
		die(_("reference is not a tree: %s"), arg);

	اگر (!has_dash_dash) {	/* case (3).(d) -> (1) */
		/*
		 * Do not complain the most common case
		 *	git checkout branch
		 * even if there happen to be a file called 'branch';
		 * it would be extremely annoying.
		 */
		اگر (argc)
			verify_non_filename(the_repository, opts->سابقہ, arg);
	} ورنہ اگر (opts->accept_pathspec) {
		argcount++;
		argv++;
		argc--;
	}

	آزاد(remote);
	واپس argcount;
}

ساکن صحیح switch_unborn_مقصد_new_branch(ثابت ساخت checkout_opts *opts)
{
	صحیح حالت_2;
	ساخت strbuf branch_ref = STRBUF_INIT;

	trace2_cmd_منوال("unborn");

	اگر (!opts->new_branch)
		die(_("You are on a branch yet to be born"));
	strbuf_addf(&branch_ref, "refs/heads/%s", opts->new_branch);
	حالت_2 = refs_update_symref(get_main_ref_store(the_repository),
				    "HEAD", branch_ref.buf, "checkout -b");
	strbuf_release(&branch_ref);
	اگر (!opts->quiet)
		fprintf(stderr, _("Switched to a new branch '%s'\n"),
			opts->new_branch);
	واپس حالت_2;
}

ساکن خالی die_expecting_a_branch(ثابت ساخت branch_info *branch_info)
{
	ساخت object_id oid;
	حرف *مقصد_free;
	صحیح رمز;

	اگر (repo_dwim_ref(the_repository, branch_info->name,
			  لڑی_لمبائی(branch_info->name), &oid, &مقصد_free, 0) == 1) {
		ثابت حرف *ref = مقصد_free;

		اگر (skip_سابقہ(ref, "refs/tags/", &ref))
			رمز = die_پیغام(_("a branch is expected, got tag '%s'"), ref);
		ورنہ اگر (skip_سابقہ(ref, "refs/remotes/", &ref))
			رمز = die_پیغام(_("a branch is expected, got remote branch '%s'"), ref);
		ورنہ
			رمز = die_پیغام(_("a branch is expected, got '%s'"), ref);
	}
	ورنہ اگر (branch_info->commit)
		رمز = die_پیغام(_("a branch is expected, got commit '%s'"), branch_info->name);
	ورنہ
		/*
		 * This case should never happen because we already die() on
		 * non-commit, but just in case.
		 */
		رمز = die_پیغام(_("a branch is expected, got '%s'"), branch_info->name);

	اگر (advice_enabled(ADVICE_SUGGEST_DETACHING_سر))
		advise(_("If you want to detach HEAD at the commit, try again with the --detach option."));

	باہر_نکلو(رمز);
}

ساکن خالی die_if_some_operation_اندر_پیش_رفت(خالی)
{
	ساخت wt_حالت_state state;

	memset(&state, 0, حجم(state));
	wt_حالت_get_state(the_repository, &state, 0);

	اگر (state.ملاؤ_اندر_پیش_رفت)
		die(_("cannot switch branch while merging\n"
		      "Consider \"git merge --quit\" "
		      "or \"git worktree add\"."));
	اگر (state.am_اندر_پیش_رفت)
		die(_("cannot switch branch in the middle of an am session\n"
		      "Consider \"git am --quit\" "
		      "or \"git worktree add\"."));
	اگر (state.rebase_interactive_اندر_پیش_رفت || state.rebase_اندر_پیش_رفت)
		die(_("cannot switch branch while rebasing\n"
		      "Consider \"git rebase --quit\" "
		      "or \"git worktree add\"."));
	اگر (state.cherry_pick_اندر_پیش_رفت)
		die(_("cannot switch branch while cherry-picking\n"
		      "Consider \"git cherry-pick --quit\" "
		      "or \"git worktree add\"."));
	اگر (state.revert_اندر_پیش_رفت)
		die(_("cannot switch branch while reverting\n"
		      "Consider \"git revert --quit\" "
		      "or \"git worktree add\"."));
	اگر (state.bisect_اندر_پیش_رفت)
		warning(_("you are switching branch while bisecting"));

	wt_حالت_state_free_buffers(&state);
}

/*
 * die if attempting to checkout an existing branch that is in use
 * in another worktree, unless ignore-other-wortrees option is given.
 * The check is bypassed when the branch is already the current one,
 * as it will not make things any worse.
 */
ساکن خالی die_if_switching_مقصد_a_branch_اندر_use(ساخت checkout_opts *opts,
						ثابت حرف *بھرا_ref)
{
	صحیح flags;
	حرف *سر_ref;

	اگر (opts->ignore_other_worktrees)
		واپس;
	سر_ref = refs_resolve_refdup(get_main_ref_store(the_repository),
				       "HEAD", 0, تہی, &flags);
	اگر (سر_ref && (!(flags & REF_ISSYMREF) || strcmp(سر_ref, بھرا_ref)))
		die_if_checked_برآمد(بھرا_ref, 1);
	آزاد(سر_ref);
}

ساکن صحیح checkout_branch(ساخت checkout_opts *opts,
			   ساخت branch_info *new_branch_info)
{
	ساخت repo_config_اقدار *cfg = repo_config_اقدار(the_repository);
	صحیح noop_switch = (!new_branch_info->name &&
			   !opts->new_branch &&
			   !opts->force_detach);

	اگر (opts->pathspec.nr)
		die(_("paths cannot be used with switching branches"));

	اگر (opts->patch_منوال)
		die(_("'%s' cannot be used with switching branches"),
		    "--patch");

	اگر (opts->overlay_منوال != -1)
		die(_("'%s' cannot be used with switching branches"),
		    "--[no]-overlay");

	اگر (opts->writeout_stage) {
		ثابت حرف *پیغام;
		اگر (noop_switch)
			پیغام = _("'%s' needs the paths to check out");
		ورنہ
			پیغام = _("'%s' cannot be used with switching branches");
		die(پیغام, "--ours/--theirs");
	}

	اگر (opts->force && opts->ملاؤ)
		die(_("'%s' cannot be used with '%s'"), "-f", "-m");

	اگر (opts->discard_changes && opts->ملاؤ)
		die(_("'%s' cannot be used with '%s'"), "--discard-changes", "--merge");

	اگر (opts->force_detach && opts->new_branch)
		die(_("'%s' cannot be used with '%s'"),
		    "--detach", "-b/-B/--orphan");

	اگر (opts->new_orphan_branch) {
		اگر (opts->track != BRANCH_TRACK_UNSPECIFIED)
			die(_("'%s' cannot be used with '%s'"), "--orphan", "-t");
		اگر (opts->orphan_from_خالی_خانے_درخت && new_branch_info->name)
			die(_("'%s' cannot take <start-point>"), "--orphan");
	} ورنہ اگر (opts->force_detach) {
		اگر (opts->track != BRANCH_TRACK_UNSPECIFIED)
			die(_("'%s' cannot be used with '%s'"), "--detach", "-t");
	} ورنہ اگر (opts->track == BRANCH_TRACK_UNSPECIFIED)
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
	    !new_branch_info->راستہ)
		die_expecting_a_branch(new_branch_info);

	اگر (!opts->can_switch_when_اندر_پیش_رفت)
		die_if_some_operation_اندر_پیش_رفت();

	/* "git checkout <branch>" */
	اگر (new_branch_info->راستہ && !opts->force_detach && !opts->new_branch)
		die_if_switching_مقصد_a_branch_اندر_use(opts, new_branch_info->راستہ);

	/* "git checkout -B <branch>" */
	اگر (opts->new_branch_force) {
		حرف *بھرا_ref = xstrfmt("refs/heads/%s", opts->new_branch);
		die_if_switching_مقصد_a_branch_اندر_use(opts, بھرا_ref);
		آزاد(بھرا_ref);
	}

	اگر (!new_branch_info->commit && opts->new_branch) {
		ساخت object_id rev;
		صحیح flag;

		اگر (!refs_read_ref_بھرا(get_main_ref_store(the_repository), "HEAD", 0, &rev, &flag) &&
		    (flag & REF_ISSYMREF) && is_null_oid(&rev))
			واپس switch_unborn_مقصد_new_branch(opts);
	}
	واپس switch_branches(opts, new_branch_info);
}

ساکن صحیح parse_opt_conflict(ثابت ساخت option *o, ثابت حرف *arg, صحیح unset)
{
	ساخت checkout_opts *opts = o->قدر;

	اگر (unset) {
		opts->conflict_style = -1;
		واپس 0;
	}
	opts->conflict_style = parse_conflict_style_name(arg);
	اگر (opts->conflict_style < 0)
		واپس error(_("unknown conflict style '%s'"), arg);
	/* --conflict overrides a previous --no-merge */
	اگر (!opts->ملاؤ)
		opts->ملاؤ = -1;

	واپس 0;
}

ساکن ساخت option *جوڑو_common_options(ساخت checkout_opts *opts,
					 ساخت option *prevopts)
{
	ساخت option options[] = {
		OPT__QUIET(&opts->quiet, N_("suppress progress reporting")),
		OPT_CALLBACK_F(0, "recurse-submodules", تہی,
			    "checkout", "control recursive updating of submodules",
			    PARSE_OPT_OPTARG, option_parse_recurse_submodules_worktree_updater),
		OPT_BOOL(0, "progress", &opts->show_پیش_رفت, N_("force progress reporting")),
		OPT_BOOL('m', "merge", &opts->ملاؤ, N_("perform a 3-way merge with the new branch")),
		OPT_CALLBACK(0, "conflict", opts, N_("style"),
			     N_("conflict style (merge, diff3, or zdiff3)"),
			     parse_opt_conflict),
		OPT_انتہا()
	};
	ساخت option *newopts = parse_options_concat(prevopts, options);
	آزاد(prevopts);
	واپس newopts;
}

ساکن ساخت option *جوڑو_common_switch_branch_options(
	ساخت checkout_opts *opts, ساخت option *prevopts)
{
	ساخت option options[] = {
		OPT_BOOL('d', "detach", &opts->force_detach, N_("detach HEAD at named commit")),
		OPT_CALLBACK_F('t', "track",  &opts->track, "(direct|inherit)",
			N_("set branch tracking configuration"),
			PARSE_OPT_OPTARG,
			parse_opt_tracking_منوال),
		OPT__FORCE(&opts->force, N_("force checkout (throw away local modifications)"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_STRING(0, "orphan", &opts->new_orphan_branch, N_("new-branch"), N_("new unborn branch")),
		OPT_BOOL_F(0, "overwrite-ignore", &opts->overwrite_ignore,
			   N_("update ignored files (default)"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "ignore-other-worktrees", &opts->ignore_other_worktrees,
			 N_("do not check if another worktree is using this branch")),
		OPT_انتہا()
	};
	ساخت option *newopts = parse_options_concat(prevopts, options);
	آزاد(prevopts);
	واپس newopts;
}

ساکن ساخت option *جوڑو_checkout_راستہ_options(ساخت checkout_opts *opts,
						ساخت option *prevopts)
{
	ساخت option options[] = {
		OPT_سیٹ_INT_F('2', "ours", &opts->writeout_stage,
			      N_("checkout our version for unmerged files"),
			      2, PARSE_OPT_NONEG),
		OPT_سیٹ_INT_F('3', "theirs", &opts->writeout_stage,
			      N_("checkout their version for unmerged files"),
			      3, PARSE_OPT_NONEG),
		OPT_BOOL('p', "patch", &opts->patch_منوال, N_("select hunks interactively")),
		OPT_DIFF_UNIFIED(&opts->patch_context),
		OPT_DIFF_INTERHUNK_CONTEXT(&opts->patch_interhunk_context),
		OPT_BOOL(0, "ignore-skip-worktree-bits", &opts->ignore_skipworktree,
			 N_("do not limit pathspecs to sparse entries only")),
		OPT_PATHSPEC_FROM_FILE(&opts->pathspec_from_file),
		OPT_PATHSPEC_FILE_NUL(&opts->pathspec_file_nul),
		OPT_انتہا()
	};
	ساخت option *newopts = parse_options_concat(prevopts, options);
	آزاد(prevopts);
	واپس newopts;
}

/* create-branch option (either b or c) */
ساکن حرف cb_option = 'b';

ساکن صحیح checkout_main(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
			 ساخت checkout_opts *opts, ساخت option *options,
			 شمار checkout_command which_command)
{
	صحیح parseopt_flags = 0;
	ساخت branch_info new_branch_info = { 0 };
	صحیح ret;

	ساکن ثابت حرف * ثابت checkout_usage[] = {
		N_("git checkout [<options>] <branch>"),
		N_("git checkout [<options>] [<branch>] -- <file>..."),
		تہی,
	};

	ساکن ثابت حرف * ثابت switch_branch_usage[] = {
		N_("git switch [<options>] [<branch>]"),
		تہی,
	};

	ساکن ثابت حرف * ثابت restore_usage[] = {
		N_("git restore [<options>] [--source=<branch>] <file>..."),
		تہی,
	};

	ثابت حرف * ثابت *usagestr;

	انتخاب (which_command) {
	حالت CHECKOUT_CHECKOUT:
		usagestr = checkout_usage;
		توڑو;
	حالت CHECKOUT_SWITCH:
		usagestr = switch_branch_usage;
		توڑو;
	حالت CHECKOUT_RESTORE:
		usagestr = restore_usage;
		توڑو;
	طےشدہ:
		BUG("no such checkout variant %d", which_command);
	}

	opts->overwrite_ignore = 1;
	opts->سابقہ = سابقہ;
	opts->show_پیش_رفت = -1;

	repo_config(the_repository, git_checkout_config, opts);
	اگر (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_بھرا_اشاریہ = 0;
	}

	opts->track = BRANCH_TRACK_UNSPECIFIED;

	اگر (!opts->accept_pathspec && !opts->accept_ref)
		BUG("make up your mind, you need to take _something_");
	اگر (opts->accept_pathspec && opts->accept_ref)
		parseopt_flags = PARSE_OPT_KEEP_DASHDASH;

	argc = parse_options(argc, argv, سابقہ, options,
			     usagestr, parseopt_flags);

	اگر (opts->patch_context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	اگر (opts->patch_interhunk_context < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	اگر (!opts->patch_منوال) {
		اگر (opts->patch_context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--patch");
		اگر (opts->patch_interhunk_context != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--patch");
		اگر (!opts->auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--patch");
	}

	اگر (opts->show_پیش_رفت < 0) {
		اگر (opts->quiet)
			opts->show_پیش_رفت = 0;
		ورنہ
			opts->show_پیش_رفت = isatty(2);
	}

	/* --conflicts implies --merge */
	اگر (opts->ملاؤ == -1)
		opts->ملاؤ = opts->conflict_style >= 0;

	اگر (opts->force) {
		opts->discard_changes = 1;
		opts->ignore_unmerged_opt = "--force";
		opts->ignore_unmerged = 1;
	}

	اگر ((!!opts->new_branch + !!opts->new_branch_force + !!opts->new_orphan_branch) > 1)
		die(_("options '-%c', '-%c', and '%s' cannot be used together"),
			cb_option, بڑا_حرف(cb_option), "--orphan");

	اگر (opts->overlay_منوال == 1 && opts->patch_منوال)
		die(_("options '%s' and '%s' cannot be used together"), "-p", "--overlay");

	اگر (opts->checkout_اشاریہ >= 0 || opts->checkout_worktree >= 0) {
		اگر (opts->checkout_اشاریہ < 0)
			opts->checkout_اشاریہ = 0;
		اگر (opts->checkout_worktree < 0)
			opts->checkout_worktree = 0;
	} ورنہ {
		اگر (opts->checkout_اشاریہ < 0)
			opts->checkout_اشاریہ = -opts->checkout_اشاریہ - 1;
		اگر (opts->checkout_worktree < 0)
			opts->checkout_worktree = -opts->checkout_worktree - 1;
	}
	اگر (opts->checkout_اشاریہ < 0 || opts->checkout_worktree < 0)
		BUG("these flags should be non-negative by now");
	/*
	 * convenient shortcut: "git restore --staged [--worktree]" equals
	 * "git restore --staged [--worktree] --source HEAD"
	 */
	اگر (!opts->from_treeish && opts->checkout_اشاریہ)
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
		ثابت حرف *argv0 = argv[0];
		اگر (!argc || !strcmp(argv0, "--"))
			die(_("--track needs a branch name"));
		skip_سابقہ(argv0, "refs/", &argv0);
		skip_سابقہ(argv0, "remotes/", &argv0);
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
		ساخت object_id rev;
		صحیح dwim_ok =
			!opts->patch_منوال &&
			opts->dwim_new_local_branch &&
			opts->track == BRANCH_TRACK_UNSPECIFIED &&
			!opts->new_branch;
		صحیح n = parse_branchname_arg(argc, argv, dwim_ok, which_command,
					     &new_branch_info, opts, &rev);
		argv += n;
		argc -= n;
	} ورنہ اگر (!opts->accept_ref && opts->from_treeish) {
		ساخت object_id rev;

		اگر (repo_get_oid_mb(the_repository, opts->from_treeish, &rev))
			die(_("could not resolve '%s'"), opts->from_treeish);

		تیاری_new_branch_info_and_ماخذ_درخت(&new_branch_info,
						      opts, &rev,
						      opts->from_treeish);

		اگر (!opts->ماخذ_درخت)
			die(_("reference is not a tree: %s"), opts->from_treeish);
	}

	اگر (argc) {
		parse_pathspec(&opts->pathspec, 0,
			       opts->patch_منوال ? PATHSPEC_سابقہ_ORIGIN : 0,
			       سابقہ, argv);

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

		اگر (opts->patch_منوال)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--patch");

		parse_pathspec_file(&opts->pathspec, 0,
				    0,
				    سابقہ, opts->pathspec_from_file, opts->pathspec_file_nul);
	} ورنہ اگر (opts->pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	opts->pathspec.recursive = 1;

	اگر (opts->pathspec.nr) {
		اگر (1 < !!opts->writeout_stage + !!opts->force + !!opts->ملاؤ)
			die(_("git checkout: --ours/--theirs, --force and --merge are incompatible when\n"
			      "checking out of the index."));
	} ورنہ {
		اگر (opts->accept_pathspec && !opts->خالی_خانے_pathspec_ok &&
		    !opts->patch_منوال)	/* patch mode is special */
			die(_("you must specify path(s) to restore"));
	}

	اگر (opts->new_branch) {
		ساخت strbuf buf = STRBUF_INIT;

		اگر (opts->new_branch_force)
			opts->branch_exists = validate_branchname(opts->new_branch, &buf);
		ورنہ
			opts->branch_exists =
				validate_new_branchname(opts->new_branch, &buf, 0);
		strbuf_release(&buf);
	}

	اگر (opts->patch_منوال || opts->pathspec.nr)
		ret = checkout_paths(opts, &new_branch_info);
	ورنہ
		ret = checkout_branch(opts, &new_branch_info);

	branch_info_release(&new_branch_info);
	clear_pathspec(&opts->pathspec);
	آزاد(opts->pathspec_from_file);
	آزاد(options);

	واپس ret;
}

صحیح cmd_checkout(صحیح argc,
		 ثابت حرف **argv,
		 ثابت حرف *سابقہ,
		 ساخت repository *repo غیر_مستعمل)
{
	ساخت checkout_opts opts = CHECKOUT_OPTS_INIT;
	ساخت option *options;
	ساخت option checkout_options[] = {
		OPT_STRING('b', تہی, &opts.new_branch, N_("branch"),
			   N_("create and checkout a new branch")),
		OPT_STRING('B', تہی, &opts.new_branch_force, N_("branch"),
			   N_("create/reset and checkout a branch")),
		OPT_BOOL('l', تہی, &opts.new_branch_log, N_("create reflog for new branch")),
		OPT_BOOL(0, "guess", &opts.dwim_new_local_branch,
			 N_("second guess 'git checkout <no-such-branch>' (default)")),
		OPT_BOOL(0, "overlay", &opts.overlay_منوال, N_("use overlay mode (default)")),
		OPT_BOOL(0, "auto-advance", &opts.auto_advance,
			 N_("auto advance to the next file when selecting hunks interactively")),
		OPT_انتہا()
	};

	opts.dwim_new_local_branch = 1;
	opts.switch_branch_doing_nothing_is_ok = 1;
	opts.only_ملاؤ_on_switching_branches = 0;
	opts.accept_ref = 1;
	opts.accept_pathspec = 1;
	opts.implicit_detach = 1;
	opts.can_switch_when_اندر_پیش_رفت = 1;
	opts.orphan_from_خالی_خانے_درخت = 0;
	opts.خالی_خانے_pathspec_ok = 1;
	opts.overlay_منوال = -1;
	opts.checkout_اشاریہ = -2;    /* default on */
	opts.checkout_worktree = -2; /* default on */

	اگر (argc == 3 && !strcmp(argv[1], "-b")) {
		/*
		 * User ran 'git checkout -b <branch>' and expects
		 * the same behavior as 'git switch -c <branch>'.
		 */
		opts.switch_branch_doing_nothing_is_ok = 0;
		opts.only_ملاؤ_on_switching_branches = 1;
	}

	options = parse_options_dup(checkout_options);
	options = جوڑو_common_options(&opts, options);
	options = جوڑو_common_switch_branch_options(&opts, options);
	options = جوڑو_checkout_راستہ_options(&opts, options);

	واپس checkout_main(argc, argv, سابقہ, &opts, options,
			     CHECKOUT_CHECKOUT);
}

صحیح cmd_switch(صحیح argc,
	       ثابت حرف **argv,
	       ثابت حرف *سابقہ,
	       ساخت repository *repo غیر_مستعمل)
{
	ساخت checkout_opts opts = CHECKOUT_OPTS_INIT;
	ساخت option *options = تہی;
	ساخت option switch_options[] = {
		OPT_STRING('c', "create", &opts.new_branch, N_("branch"),
			   N_("create and switch to a new branch")),
		OPT_STRING('C', "force-create", &opts.new_branch_force, N_("branch"),
			   N_("create/reset and switch to a branch")),
		OPT_BOOL(0, "guess", &opts.dwim_new_local_branch,
			 N_("second guess 'git switch <no-such-branch>'")),
		OPT_BOOL(0, "discard-changes", &opts.discard_changes,
			 N_("throw away local modifications")),
		OPT_انتہا()
	};

	opts.dwim_new_local_branch = 1;
	opts.accept_ref = 1;
	opts.accept_pathspec = 0;
	opts.switch_branch_doing_nothing_is_ok = 0;
	opts.only_ملاؤ_on_switching_branches = 1;
	opts.implicit_detach = 0;
	opts.can_switch_when_اندر_پیش_رفت = 0;
	opts.orphan_from_خالی_خانے_درخت = 1;
	opts.overlay_منوال = -1;

	options = parse_options_dup(switch_options);
	options = جوڑو_common_options(&opts, options);
	options = جوڑو_common_switch_branch_options(&opts, options);

	cb_option = 'c';

	واپس checkout_main(argc, argv, سابقہ, &opts, options,
			     CHECKOUT_SWITCH);
}

صحیح cmd_restore(صحیح argc,
		ثابت حرف **argv,
		ثابت حرف *سابقہ,
		ساخت repository *repo غیر_مستعمل)
{
	ساخت checkout_opts opts = CHECKOUT_OPTS_INIT;
	ساخت option *options;
	ساخت option restore_options[] = {
		OPT_STRING('s', "source", &opts.from_treeish, "<tree-ish>",
			   N_("which tree-ish to checkout from")),
		OPT_BOOL('S', "staged", &opts.checkout_اشاریہ,
			   N_("restore the index")),
		OPT_BOOL('W', "worktree", &opts.checkout_worktree,
			   N_("restore the working tree (default)")),
		OPT_BOOL(0, "ignore-unmerged", &opts.ignore_unmerged,
			 N_("ignore unmerged entries")),
		OPT_BOOL(0, "overlay", &opts.overlay_منوال, N_("use overlay mode")),
		OPT_انتہا()
	};

	opts.accept_ref = 0;
	opts.accept_pathspec = 1;
	opts.خالی_خانے_pathspec_ok = 0;
	opts.overlay_منوال = 0;
	opts.checkout_اشاریہ = -1;    /* default off */
	opts.checkout_worktree = -2; /* default on */
	opts.ignore_unmerged_opt = "--ignore-unmerged";

	options = parse_options_dup(restore_options);
	options = جوڑو_common_options(&opts, options);
	options = جوڑو_checkout_راستہ_options(&opts, options);

	واپس checkout_main(argc, argv, سابقہ, &opts, options,
			     CHECKOUT_RESTORE);
}
