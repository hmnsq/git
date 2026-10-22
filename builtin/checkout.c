#وصف_ڪريو USE_THE_REPOSITORY_VARIABLE
#وصف_ڪريو DISABLE_SIGN_COMPARE_WARNINGS

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
	صحيح patch_گهڻائي;
	صحيح patch_context;
	صحيح patch_interhunk_context;
	صحيح auto_advance;
	صحيح quiet;
	صحيح ملايو;
	صحيح force;
	صحيح force_detach;
	صحيح implicit_detach;
	صحيح writeout_stage;
	صحيح overwrite_ignore;
	صحيح ignore_skipworktree;
	صحيح ignore_other_worktrees;
	صحيح show_ترقي;
	صحيح ڳڻپ_checkout_paths;
	صحيح overlay_گهڻائي;
	صحيح dwim_new_local_branch;
	صحيح discard_changes;
	صحيح accept_ref;
	صحيح accept_pathspec;
	صحيح switch_branch_doing_nothing_is_ok;
	صحيح only_ملايو_on_switching_branches;
	صحيح can_switch_when_اندريون_ترقي;
	صحيح orphan_from_خالي_جايون_وڻ;
	صحيح خالي_جايون_pathspec_ok;
	صحيح checkout_اشاريو;
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
	ثابت حرف *اڳياڙي;
	بنية pathspec pathspec;
	ثابت حرف *from_treeish;
	بنية وڻ *ذريعو_وڻ;
};

#وصف_ڪريو CHECKOUT_OPTS_INIT { \
	.conflict_style = -1, \
	.ملايو = -1, \
	.patch_context = -1, \
	.patch_interhunk_context = -1, \
	.auto_advance = 1, \
}

#وصف_ڪريو ملايو_WORKING_وڻ_UNPACK_FAILED (-2)

بنية branch_info {
	حرف *name; /* The short name used */
	حرف *رستو; /* The full name of a real branch */
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
	حرر(info->رستو);
	حرر(info->refname);
	حرر(info->checkout);
}

ساكن صحيح post_checkout_hook(بنية commit *old_commit, بنية commit *new_commit,
			      صحيح بدليو)
{
	بنية لاڳيتو_hooks_opt opt = لاڳيتو_HOOKS_OPT_INIT_FORCE_SERIAL;

	/*
	 * "new_commit" can be NULL when checking out from the index before
	 * a commit exists.
	 */
	strvec_pushl(&opt.دليلون,
		     oid_منزل_hex(old_commit ? &old_commit->object.oid : null_oid(the_hash_algo)),
		     oid_منزل_hex(new_commit ? &new_commit->object.oid : null_oid(the_hash_algo)),
		     بدليو ? "1" : "0",
		     عدم);

	أرجع لاڳيتو_hooks_opt(the_repository, "post-checkout", &opt);
}

/*
 * Handle a tree object and determine if we need to recurse into the
 * tree (READ_TREE_RECURSIVE) or skip it (0).
 */
ساكن صحيح try_update_sparse_directory(ثابت بنية object_id *oid,
				       بنية strbuf *بنياد,
				       ثابت حرف *pathname,
				       صحيح overlay_گهڻائي)
{
	بنية strbuf dirpath = STRBUF_INIT;
	بنية cache_داخلا *old;
	صحيح pos, نتيجو = READ_وڻ_RECURSIVE;

	إذا (!the_repository->اشاريو->sparse_اشاريو)
		أرجع نتيجو;

	strbuf_addbuf(&dirpath, بنياد);
	strbuf_addstr(&dirpath, pathname);
	strbuf_addch(&dirpath, '/');

	pos = اشاريو_name_pos_sparse(the_repository->اشاريو,
				    dirpath.buf, dirpath.len);
	إذا (pos < 0)
		اذهب cleanup;

	old = the_repository->اشاريو->cache[pos];
	إذا (!S_ISSPARSEDIR(old->ce_گهڻائي))
		اذهب cleanup;

	إذا (oideq(oid, &old->oid)) {
		/* Tree content already matches; no need to descend. */
		نتيجو = 0;
	} وإلا إذا (!overlay_گهڻائي) {
		/*
		 * In non-overlay mode (e.g., restore --staged), replace the
		 * sparse directory OID directly since files not present in
		 * the source tree should be removed anyway.
		 */
		oidcpy(&old->oid, oid);
		old->ce_flags |= CE_UPDATE;
		نتيجو = 0;
	}

cleanup:
	strbuf_release(&dirpath);
	أرجع نتيجو;
}

ساكن صحيح update_some(ثابت بنية object_id *oid, بنية strbuf *بنياد,
		       ثابت حرف *pathname, بنانشان گهڻائي, فراغ *context)
{
	صحيح len;
	بنية cache_داخلا *ce;
	صحيح pos;
	صحيح overlay_گهڻائي = context ? *((صحيح *)context) : 1;

	إذا (S_ISDIR(گهڻائي))
		أرجع try_update_sparse_directory(oid, بنياد, pathname,
						   overlay_گهڻائي);

	len = بنياد->len + لڙي_ڊيگهه(pathname);
	ce = make_خالي_جايون_cache_داخلا(the_repository->اشاريو, len);
	oidcpy(&ce->oid, oid);
	يادگيري_نقل(ce->name, بنياد->buf, بنياد->len);
	يادگيري_نقل(ce->name + بنياد->len, pathname, len - بنياد->len);
	ce->ce_flags = create_ce_flags(0) | CE_UPDATE;
	ce->ce_namelen = len;
	ce->ce_گهڻائي = create_ce_گهڻائي(گهڻائي);

	/*
	 * If the entry is the same as the current index, we can leave the old
	 * entry in place. Whether it is UPTODATE or not, checkout_entry will
	 * do the right thing.
	 */
	pos = اشاريو_name_pos_sparse(the_repository->اشاريو, ce->name, ce->ce_namelen);
	إذا (pos >= 0) {
		بنية cache_داخلا *old = the_repository->اشاريو->cache[pos];
		إذا (ce->ce_گهڻائي == old->ce_گهڻائي &&
		    !ce_intent_منزل_جوڙيو(old) &&
		    oideq(&ce->oid, &old->oid)) {
			old->ce_flags |= CE_UPDATE;
			discard_cache_داخلا(ce);
			أرجع 0;
		}
	}

	جوڙيو_اشاريو_داخلا(the_repository->اشاريو, ce,
			جوڙيو_CACHE_OK_منزل_جوڙيو | جوڙيو_CACHE_OK_منزل_REPLACE);
	أرجع 0;
}

ساكن صحيح read_وڻ_some(بنية وڻ *وڻ, ثابت بنية pathspec *pathspec,
			  صحيح overlay_گهڻائي)
{
	read_وڻ(the_repository, وڻ,
		  pathspec, update_some, &overlay_گهڻائي);

	/* update the index with the given tree's info
	 * for all args, expanding wildcards, and exit
	 * with any non-zero return code.
	 */
	أرجع 0;
}

ساكن صحيح skip_same_name(ثابت بنية cache_داخلا *ce, صحيح pos)
{
	طالما (++pos < the_repository->اشاريو->cache_nr &&
	       !strcmp(the_repository->اشاريو->cache[pos]->name, ce->name))
		; /* skip */
	أرجع pos;
}

ساكن صحيح check_stage(صحيح stage, ثابت بنية cache_داخلا *ce, صحيح pos,
		       صحيح overlay_گهڻائي)
{
	طالما (pos < the_repository->اشاريو->cache_nr &&
	       !strcmp(the_repository->اشاريو->cache[pos]->name, ce->name)) {
		إذا (ce_stage(the_repository->اشاريو->cache[pos]) == stage)
			أرجع 0;
		pos++;
	}
	إذا (!overlay_گهڻائي)
		أرجع 0;
	إذا (stage == 2)
		أرجع error(_("path '%s' does not have our version"), ce->name);
	وإلا
		أرجع error(_("path '%s' does not have their version"), ce->name);
}

ساكن صحيح check_stages(بنانشان stages, ثابت بنية cache_داخلا *ce, صحيح pos)
{
	بنانشان ڏٺل = 0;
	ثابت حرف *name = ce->name;

	طالما (pos < the_repository->اشاريو->cache_nr) {
		ce = the_repository->اشاريو->cache[pos];
		إذا (strcmp(name, ce->name))
			اكسر;
		ڏٺل |= (1 << ce_stage(ce));
		pos++;
	}
	إذا ((stages & ڏٺل) != stages)
		أرجع error(_("path '%s' does not have all necessary versions"),
			     name);
	أرجع 0;
}

ساكن صحيح checkout_stage(صحيح stage, ثابت بنية cache_داخلا *ce, صحيح pos,
			  ثابت بنية checkout *state, صحيح *nr_checkouts,
			  صحيح overlay_گهڻائي)
{
	طالما (pos < the_repository->اشاريو->cache_nr &&
	       !strcmp(the_repository->اشاريو->cache[pos]->name, ce->name)) {
		إذا (ce_stage(the_repository->اشاريو->cache[pos]) == stage)
			أرجع checkout_داخلا(the_repository->اشاريو->cache[pos], state,
					      عدم, nr_checkouts);
		pos++;
	}
	إذا (!overlay_گهڻائي) {
		unlink_داخلا(ce, عدم);
		أرجع 0;
	}
	إذا (stage == 2)
		أرجع error(_("path '%s' does not have our version"), ce->name);
	وإلا
		أرجع error(_("path '%s' does not have their version"), ce->name);
}

ساكن صحيح checkout_merged(صحيح pos, ثابت بنية checkout *state,
			   صحيح *nr_checkouts, بنية mem_pool *ce_mem_pool,
			   صحيح conflict_style)
{
	بنية cache_داخلا *ce = the_repository->اشاريو->cache[pos];
	ثابت حرف *رستو = ce->name;
	mmfile_t ancestor, ours, theirs;
	تعداد ll_ملايو_نتيجو ملايو_حالت;
	صحيح حالت;
	بنية object_id oid;
	mmfile_t نتيجو_buf;
	بنية object_id threeway[3];
	بنانشان گهڻائي = 0;
	بنية ll_ملايو_options ll_opts = LL_ملايو_OPTIONS_INIT;
	صحيح renormalize = 0;

	memset(threeway, 0, حجم(threeway));
	طالما (pos < the_repository->اشاريو->cache_nr) {
		صحيح stage;
		stage = ce_stage(ce);
		إذا (!stage || strcmp(رستو, ce->name))
			اكسر;
		oidcpy(&threeway[stage - 1], &ce->oid);
		إذا (stage == 2)
			گهڻائي = create_ce_گهڻائي(ce->ce_گهڻائي);
		pos++;
		ce = the_repository->اشاريو->cache[pos];
	}
	إذا (is_null_oid(&threeway[1]) || is_null_oid(&threeway[2]))
		أرجع error(_("path '%s' does not have necessary versions"), رستو);

	read_mmblob(&ancestor, the_repository->objects, &threeway[0]);
	read_mmblob(&ours, the_repository->objects, &threeway[1]);
	read_mmblob(&theirs, the_repository->objects, &threeway[2]);

	repo_config_get_bool(the_repository, "merge.renormalize", &renormalize);
	ll_opts.renormalize = renormalize;
	ll_opts.conflict_style = conflict_style;
	ملايو_حالت = ll_ملايو(&نتيجو_buf, رستو, &ancestor, "base",
				&ours, "ours", &theirs, "theirs",
				state->istate, &ll_opts);
	حرر(ancestor.ptr);
	حرر(ours.ptr);
	حرر(theirs.ptr);
	إذا (ملايو_حالت == LL_ملايو_ٻٽو_CONFLICT)
		warning("Cannot merge binary files: %s (%s vs. %s)",
			رستو, "ours", "theirs");
	إذا (ملايو_حالت < 0 || !نتيجو_buf.ptr) {
		حرر(نتيجو_buf.ptr);
		أرجع error(_("path '%s': cannot merge"), رستو);
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
	إذا (odb_write_object(the_repository->objects, نتيجو_buf.ptr, نتيجو_buf.سائيز, OBJ_BLOB, &oid))
		die(_("Unable to add merge result for '%s'"), رستو);
	حرر(نتيجو_buf.ptr);
	ce = make_transient_cache_داخلا(گهڻائي, &oid, رستو, 2, ce_mem_pool);
	إذا (!ce)
		die(_("make_cache_entry failed for path '%s'"), رستو);
	حالت = checkout_داخلا(ce, state, عدم, nr_checkouts);
	أرجع حالت;
}

ساكن فراغ mark_ce_for_checkout_overlay(بنية cache_داخلا *ce,
					 حرف *ps_matched,
					 ثابت بنية checkout_opts *opts)
{
	ce->ce_flags &= ~CE_MATCHED;
	إذا (!opts->ignore_skipworktree && ce_skip_worktree(ce))
		أرجع;
	إذا (opts->ذريعو_وڻ && !(ce->ce_flags & CE_UPDATE))
		/*
		 * "git checkout tree-ish -- path", but this entry
		 * is in the original index but is not in tree-ish
		 * or does not match the pathspec; it will not be
		 * checked out to the working tree.  We will not do
		 * anything to this entry at all.
		 */
		أرجع;
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
	إذا (ce_رستو_ميلاپ(the_repository->اشاريو, ce, &opts->pathspec, ps_matched))
		ce->ce_flags |= CE_MATCHED;
}

ساكن فراغ mark_ce_for_checkout_no_overlay(بنية cache_داخلا *ce,
					    حرف *ps_matched,
					    ثابت بنية checkout_opts *opts)
{
	ce->ce_flags &= ~CE_MATCHED;
	إذا (!opts->ignore_skipworktree && ce_skip_worktree(ce))
		أرجع;
	إذا (ce_رستو_ميلاپ(the_repository->اشاريو, ce, &opts->pathspec, ps_matched)) {
		ce->ce_flags |= CE_MATCHED;
		إذا (opts->ذريعو_وڻ && !(ce->ce_flags & CE_UPDATE))
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
	صحيح pc_workers, pc_حد;
	بنية mem_pool ce_mem_pool;

	state.force = 1;
	state.refresh_cache = 1;
	state.istate = the_repository->اشاريو;

	mem_pool_init(&ce_mem_pool, 0);
	get_parallel_checkout_configs(&pc_workers, &pc_حد);
	init_checkout_metadata(&state.meta, info->refname,
			       info->commit ? &info->commit->object.oid : &info->oid,
			       عدم);

	enable_delayed_checkout(&state);

	إذا (pc_workers > 1)
		init_parallel_checkout();

	لكل (pos = 0; pos < the_repository->اشاريو->cache_nr; pos++) {
		بنية cache_داخلا *ce = the_repository->اشاريو->cache[pos];
		إذا (ce->ce_flags & CE_MATCHED) {
			إذا (!ce_stage(ce)) {
				errs |= checkout_داخلا(ce, &state,
						       عدم, &nr_checkouts);
				استمر;
			}
			إذا (opts->writeout_stage)
				errs |= checkout_stage(opts->writeout_stage,
						       ce, pos,
						       &state,
						       &nr_checkouts, opts->overlay_گهڻائي);
			وإلا إذا (opts->ملايو)
				errs |= checkout_merged(pos, &state,
							&nr_unmerged,
							&ce_mem_pool,
							opts->conflict_style);
			pos = skip_same_name(ce, pos) - 1;
		}
	}
	إذا (pc_workers > 1)
		errs |= لاڳيتو_parallel_checkout(&state, pc_workers, pc_حد,
					      عدم, عدم);
	mem_pool_discard(&ce_mem_pool, should_validate_cache_entries());
	remove_marked_cache_entries(the_repository->اشاريو, 1);
	remove_scheduled_dirs();
	errs |= پڄاڻي_delayed_checkout(&state, opts->show_ترقي);

	إذا (opts->ڳڻپ_checkout_paths) {
		إذا (nr_unmerged)
			fprintf_ln(stderr, Q_("Recreated %d merge conflict",
					      "Recreated %d merge conflicts",
					      nr_unmerged),
				   nr_unmerged);
		إذا (opts->ذريعو_وڻ)
			fprintf_ln(stderr, Q_("Updated %d path from %s",
					      "Updated %d paths from %s",
					      nr_checkouts),
				   nr_checkouts,
				   repo_ڳوليو_unique_abbrev(the_repository, &opts->ذريعو_وڻ->object.oid,
							   DEFAULT_ABBREV));
		وإلا إذا (!nr_unmerged || nr_checkouts)
			fprintf_ln(stderr, Q_("Updated %d path from the index",
					      "Updated %d paths from the index",
					      nr_checkouts),
				   nr_checkouts);
	}

	أرجع errs;
}

ساكن صحيح checkout_paths(ثابت بنية checkout_opts *opts,
			  ثابت بنية branch_info *new_branch_info)
{
	صحيح pos;
	ساكن حرف *ps_matched;
	بنية object_id rev;
	بنية commit *سر;
	صحيح errs = 0;
	بنية تالو_file تالو_file = تالو_INIT;
	صحيح checkout_اشاريو;

	trace2_cmd_گهڻائي(opts->patch_گهڻائي ? "patch" : "path");

	إذا (opts->track != BRANCH_TRACK_UNSPECIFIED)
		die(_("'%s' cannot be used with updating paths"), "--track");

	إذا (opts->new_branch_log)
		die(_("'%s' cannot be used with updating paths"), "-l");

	إذا (opts->ignore_unmerged && opts->patch_گهڻائي)
		die(_("'%s' cannot be used with updating paths"),
		    opts->ignore_unmerged_opt);

	إذا (opts->force_detach)
		die(_("'%s' cannot be used with updating paths"), "--detach");

	إذا (opts->ملايو && opts->patch_گهڻائي)
		die(_("options '%s' and '%s' cannot be used together"), "--merge", "--patch");

	إذا (opts->ignore_unmerged && opts->ملايو)
		die(_("options '%s' and '%s' cannot be used together"),
		    opts->ignore_unmerged_opt, "-m");

	إذا (opts->new_branch)
		die(_("Cannot update paths and switch to branch '%s' at the same time."),
		    opts->new_branch);

	إذا (!opts->checkout_worktree && !opts->checkout_اشاريو)
		die(_("neither '%s' or '%s' is specified"),
		    "--staged", "--worktree");

	إذا (!opts->checkout_worktree && !opts->from_treeish)
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
	إذا (!opts->accept_ref && opts->checkout_اشاريو) {
		إذا (opts->writeout_stage)
			die(_("'%s' or '%s' cannot be used with %s"),
			    "--ours", "--theirs", "--staged");

		إذا (opts->ملايو)
			die(_("'%s' or '%s' cannot be used with %s"),
			    "--merge", "--conflict", "--staged");
	}

	/*
	 * recreating unmerged index entries and writing out data from
	 * unmerged index entries would make no sense when checking out
	 * of a tree-ish.
	 */
	إذا ((opts->ملايو || opts->writeout_stage) && opts->ذريعو_وڻ)
		die(_("'%s', '%s', or '%s' cannot be used when checking out of a tree"),
		    "--merge", "--ours", "--theirs");

	إذا (opts->patch_گهڻائي) {
		تعداد جوڙيو_p_گهڻائي patch_گهڻائي;
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
		إذا (rev && new_branch_info->commit && strcmp(rev, "HEAD"))
			rev = oid_منزل_hex_r(rev_oid, &new_branch_info->commit->object.oid);

		إذا (opts->checkout_اشاريو && opts->checkout_worktree)
			patch_گهڻائي = جوڙيو_P_CHECKOUT;
		وإلا إذا (opts->checkout_اشاريو && !opts->checkout_worktree)
			patch_گهڻائي = جوڙيو_P_RESET;
		وإلا إذا (!opts->checkout_اشاريو && opts->checkout_worktree)
			patch_گهڻائي = جوڙيو_P_WORKTREE;
		وإلا
			BUG("either flag must have been set, worktree=%d, index=%d",
			    opts->checkout_worktree, opts->checkout_اشاريو);
		أرجع !!لاڳيتو_جوڙيو_p(the_repository, patch_گهڻائي, &interactive_opts,
				   rev, &opts->pathspec, 0);
	}

	repo_hold_locked_اشاريو(the_repository, &تالو_file, تالو_DIE_ON_ERROR);
	إذا (repo_read_اشاريو_preload(the_repository, &opts->pathspec, 0) < 0)
		أرجع error(_("index file corrupt"));

	إذا (opts->ذريعو_وڻ)
		read_وڻ_some(opts->ذريعو_وڻ, &opts->pathspec,
			       opts->overlay_گهڻائي);
	إذا (opts->ملايو)
		unmerge_اشاريو(the_repository->اشاريو, &opts->pathspec, CE_MATCHED);

	ps_matched = xcalloc(opts->pathspec.nr, 1);

	/*
	 * Make sure all pathspecs participated in locating the paths
	 * to be checked out.
	 */
	لكل (pos = 0; pos < the_repository->اشاريو->cache_nr; pos++)
		إذا (opts->overlay_گهڻائي)
			mark_ce_for_checkout_overlay(the_repository->اشاريو->cache[pos],
						     ps_matched,
						     opts);
		وإلا
			mark_ce_for_checkout_no_overlay(the_repository->اشاريو->cache[pos],
							ps_matched,
							opts);

	إذا (report_رستو_error(ps_matched, &opts->pathspec)) {
		حرر(ps_matched);
		أرجع 1;
	}
	حرر(ps_matched);

	/* Any unmerged paths? */
	لكل (pos = 0; pos < the_repository->اشاريو->cache_nr; pos++) {
		ثابت بنية cache_داخلا *ce = the_repository->اشاريو->cache[pos];
		إذا (ce->ce_flags & CE_MATCHED) {
			إذا (!ce_stage(ce))
				استمر;
			إذا (opts->ignore_unmerged) {
				إذا (!opts->quiet)
					warning(_("path '%s' is unmerged"), ce->name);
			} وإلا إذا (opts->writeout_stage) {
				errs |= check_stage(opts->writeout_stage, ce, pos, opts->overlay_گهڻائي);
			} وإلا إذا (opts->ملايو) {
				errs |= check_stages((1<<2) | (1<<3), ce, pos);
			} وإلا {
				errs = 1;
				error(_("path '%s' is unmerged"), ce->name);
			}
			pos = skip_same_name(ce, pos) - 1;
		}
	}
	إذا (errs)
		أرجع 1;

	/* Now we are committed to check them out */
	إذا (opts->checkout_worktree)
		errs |= checkout_worktree(opts, new_branch_info);
	وإلا
		remove_marked_cache_entries(the_repository->اشاريو, 1);

	/*
	 * Allow updating the index when checking out from the index.
	 * This is to save new stat info.
	 */
	إذا (opts->checkout_worktree && !opts->checkout_اشاريو && !opts->ذريعو_وڻ)
		checkout_اشاريو = 1;
	وإلا
		checkout_اشاريو = opts->checkout_اشاريو;

	إذا (checkout_اشاريو) {
		إذا (write_locked_اشاريو(the_repository->اشاريو, &تالو_file, COMMIT_تالو))
			die(_("unable to write new index file"));
	} وإلا {
		/*
		 * NEEDSWORK: if --worktree is not specified, we
		 * should save stat info of checked out files in the
		 * index to avoid the next (potentially costly)
		 * refresh. But it's a bit tricker to do...
		 */
		rollback_تالو_file(&تالو_file);
	}

	refs_read_ref_ڀريل(get_main_ref_store(the_repository), "HEAD", 0,
			   &rev, عدم);
	سر = lookup_commit_reference_gently(the_repository, &rev, 1);

	errs |= post_checkout_hook(سر, سر, 0);
	أرجع errs;
}

ساكن فراغ show_local_changes(بنية object *سر,
			       ثابت بنية diff_options *opts)
{
	بنية rev_info rev;
	/* I think we want full paths, even if we're in a subdirectory. */
	repo_init_revisions(the_repository, &rev, عدم);
	rev.diffopt.flags = opts->flags;
	rev.diffopt.output_format |= DIFF_FORMAT_NAME_حالت;
	rev.diffopt.flags.recursive = 1;
	diff_تياري_پورو(&rev.diffopt);
	جوڙيو_pending_object(&rev, سر, عدم);
	لاڳيتو_diff_اشاريو(&rev, 0);
	release_revisions(&rev);
}

ساكن فراغ describe_detached_سر(ثابت حرف *نياپو, بنية commit *commit)
{
	بنية strbuf sb = STRBUF_INIT;

	إذا (!repo_parse_commit(the_repository, commit))
		pp_commit_easy(CMIT_FMT_ONELINE, commit, &sb);
	إذا (print_sha1_ellipsis()) {
		fprintf(stderr, "%s %s... %s\n", نياپو,
			repo_ڳوليو_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV),
			sb.buf);
	} وإلا {
		fprintf(stderr, "%s %s %s\n", نياپو,
			repo_ڳوليو_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV),
			sb.buf);
	}
	strbuf_release(&sb);
}

ساكن صحيح reset_وڻ(بنية وڻ *وڻ, ثابت بنية checkout_opts *o,
		      صحيح worktree, صحيح *writeout_error,
		      بنية branch_info *info)
{
	بنية unpack_trees_options opts;
	بنية وڻ_desc وڻ_desc;

	memset(&opts, 0, حجم(opts));
	opts.سر_idx = -1;
	opts.update = worktree;
	opts.skip_unmerged = !worktree;
	opts.reset = o->force ? UNPACK_RESET_OVERWRITE_UNTRACKED :
				UNPACK_RESET_PROTECT_UNTRACKED;
	opts.preserve_ignored = (!o->force && !o->overwrite_ignore);
	opts.ملايو = 1;
	opts.fn = oneway_ملايو;
	opts.verbose_update = o->show_ترقي;
	opts.src_اشاريو = the_repository->اشاريو;
	opts.dst_اشاريو = the_repository->اشاريو;
	init_checkout_metadata(&opts.meta, info->refname,
			       info->commit ? &info->commit->object.oid : null_oid(the_hash_algo),
			       عدم);
	إذا (repo_parse_وڻ(the_repository, وڻ) < 0)
		أرجع 128;
	init_وڻ_desc(&وڻ_desc, &وڻ->object.oid, وڻ->بفر, وڻ->سائيز);
	بدل (unpack_trees(1, &وڻ_desc, &opts)) {
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
		أرجع 0;
	افتراضي:
		أرجع 128;
	}
}

ساكن فراغ تياري_branch_رستو(بنية branch_info *branch)
{
	بنية strbuf buf = STRBUF_INIT;

	/*
	 * If this is a ref, resolve it; otherwise, look up the OID for our
	 * expression.  Failure here is okay.
	 */
	إذا (!repo_dwim_ref(the_repository, branch->name, لڙي_ڊيگهه(branch->name),
			   &branch->oid, &branch->refname, 0))
		repo_get_oid_committish(the_repository, branch->name, &branch->oid);

	copy_branchname(the_repository, &buf, branch->name, INTERPRET_BRANCH_LOCAL);
	إذا (strcmp(buf.buf, branch->name)) {
		حرر(branch->name);
		branch->name = xstrdup(buf.buf);
	}
	strbuf_splice(&buf, 0, 0, "refs/heads/", 11);
	حرر(branch->رستو);
	branch->رستو = strbuf_detach(&buf, عدم);
}

ساكن فراغ init_topts(بنية unpack_trees_options *topts,
		       صحيح show_ترقي, صحيح overwrite_ignore,
		       bool quiet)
{
	memset(topts, 0, حجم(*topts));
	topts->سر_idx = -1;
	topts->src_اشاريو = the_repository->اشاريو;
	topts->dst_اشاريو = the_repository->اشاريو;

	تياري_unpack_trees_porcelain(topts, "checkout");

	topts->initial_checkout = is_اشاريو_unborn(the_repository->اشاريو);
	topts->update = 1;
	topts->ملايو = 1;
	topts->quiet = quiet;
	topts->verbose_update = show_ترقي;
	topts->fn = twoway_ملايو;
	topts->preserve_ignored = !overwrite_ignore;
}

ساكن صحيح ملايو_working_وڻ(ثابت بنية checkout_opts *opts,
			      بنية branch_info *old_branch_info,
			      بنية branch_info *new_branch_info,
			      bool quiet,
			      صحيح *writeout_error)
{
	صحيح ret;
	بنية تالو_file تالو_file = تالو_INIT;
	بنية وڻ *new_وڻ;

	repo_hold_locked_اشاريو(the_repository, &تالو_file, تالو_DIE_ON_ERROR);
	إذا (repo_read_اشاريو_preload(the_repository, عدم, 0) < 0) {
		rollback_تالو_file(&تالو_file);
		أرجع error(_("index file corrupt"));
	}

	resolve_undo_clear_اشاريو(the_repository->اشاريو);
	إذا (opts->new_orphan_branch && opts->orphan_from_خالي_جايون_وڻ) {
		إذا (new_branch_info->commit)
			BUG("'switch --orphan' should never accept a commit as starting point");
		new_وڻ = repo_parse_وڻ_اڻسڌو(the_repository,
						    the_hash_algo->خالي_جايون_وڻ);
		إذا (!new_وڻ)
			BUG("unable to read empty tree");
	} وإلا {
		new_وڻ = repo_get_commit_وڻ(the_repository,
						new_branch_info->commit);
		إذا (!new_وڻ) {
			rollback_تالو_file(&تالو_file);
			أرجع error(_("unable to read tree (%s)"),
				     oid_منزل_hex(&new_branch_info->commit->object.oid));
		}
	}
	إذا (opts->discard_changes) {
		ret = reset_وڻ(new_وڻ, opts, 1, writeout_error, new_branch_info);
		إذا (ret) {
			rollback_تالو_file(&تالو_file);
			أرجع ret;
		}
	} وإلا {
		بنية وڻ_desc trees[2];
		بنية وڻ *وڻ;
		بنية unpack_trees_options topts;
		ثابت بنية object_id *old_commit_oid;

		refresh_اشاريو(the_repository->اشاريو, REFRESH_QUIET, عدم, عدم, عدم);

		إذا (unmerged_اشاريو(the_repository->اشاريو)) {
			rollback_تالو_file(&تالو_file);
			error(_("you need to resolve your current index first"));
			أرجع 1;
		}

		/* 2-way merge to the new branch */
		init_topts(&topts, opts->show_ترقي,
			   opts->overwrite_ignore, quiet);
		init_checkout_metadata(&topts.meta, new_branch_info->refname,
				       new_branch_info->commit ?
				       &new_branch_info->commit->object.oid :
				       &new_branch_info->oid, عدم);

		old_commit_oid = old_branch_info->commit ?
			&old_branch_info->commit->object.oid :
			the_hash_algo->خالي_جايون_وڻ;
		وڻ = repo_parse_وڻ_اڻسڌو(the_repository,
						old_commit_oid);
		إذا (!وڻ)
			die(_("unable to parse commit %s"),
				oid_منزل_hex(old_commit_oid));

		init_وڻ_desc(&trees[0], &وڻ->object.oid,
			       وڻ->بفر, وڻ->سائيز);
		إذا (repo_parse_وڻ(the_repository, new_وڻ) < 0)
			die(عدم);
		وڻ = new_وڻ;
		init_وڻ_desc(&trees[1], &وڻ->object.oid,
			       وڻ->بفر, وڻ->سائيز);

		ret = unpack_trees(2, trees, &topts);
		clear_unpack_trees_porcelain(&topts);
		إذا (ret == -1) {
			rollback_تالو_file(&تالو_file);
			أرجع ملايو_WORKING_وڻ_UNPACK_FAILED;
		}
	}

	إذا (!cache_وڻ_fully_valid(the_repository->اشاريو))
		cache_وڻ_update(the_repository->اشاريو, WRITE_وڻ_SILENT | WRITE_وڻ_REPAIR);

	إذا (write_locked_اشاريو(the_repository->اشاريو, &تالو_file, COMMIT_تالو))
		die(_("unable to write new index file"));

	إذا (!opts->discard_changes && !opts->quiet && new_branch_info->commit)
		show_local_changes(&new_branch_info->commit->object, &opts->diff_options);

	أرجع 0;
}

ساكن فراغ report_tracking(بنية branch_info *new_branch_info)
{
	بنية strbuf sb = STRBUF_INIT;
	بنية branch *branch = branch_get(new_branch_info->name);

	إذا (!format_tracking_info(branch, &sb, AHEAD_BEHIND_ڀريل, 1))
		أرجع;
	fputs(sb.buf, stdout);
	strbuf_release(&sb);
}

ساكن فراغ update_refs_for_switch(ثابت بنية checkout_opts *opts,
				   بنية branch_info *old_branch_info,
				   بنية branch_info *new_branch_info)
{
	بنية strbuf نياپو = STRBUF_INIT;
	ثابت حرف *old_desc, *reflog_نياپو;
	إذا (opts->new_branch) {
		إذا (opts->new_orphan_branch) {
			تعداد log_refs_config log_all_ref_updates = LOG_REFS_UNSET;
			ثابت حرف *قدر;
			حرف *refname;

			إذا (!repo_config_get_string_tmp(the_repository, "core.logallrefupdates", &قدر))
				log_all_ref_updates = refs_parse_log_all_ref_updates_config(قدر);

			refname = mkpathdup("refs/heads/%s", opts->new_orphan_branch);
			إذا (opts->new_branch_log &&
			    !should_autocreate_reflog(log_all_ref_updates, refname)) {
				صحيح ret;
				بنية strbuf err = STRBUF_INIT;

				ret = refs_create_reflog(get_main_ref_store(the_repository),
							 refname, &err);
				إذا (ret) {
					fprintf(stderr, _("Can not do reflog for '%s': %s\n"),
						opts->new_orphan_branch, err.buf);
					strbuf_release(&err);
					حرر(refname);
					أرجع;
				}
				strbuf_release(&err);
			}
			حرر(refname);
		}
		وإلا
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
		تياري_branch_رستو(new_branch_info);
	}

	old_desc = old_branch_info->name;
	إذا (!old_desc && old_branch_info->commit)
		old_desc = oid_منزل_hex(&old_branch_info->commit->object.oid);

	reflog_نياپو = getenv("GIT_REFLOG_ACTION");
	إذا (!reflog_نياپو)
		strbuf_addf(&نياپو, "checkout: moving from %s to %s",
			old_desc ? old_desc : "(invalid)", new_branch_info->name);
	وإلا
		strbuf_insertstr(&نياپو, 0, reflog_نياپو);

	إذا (!strcmp(new_branch_info->name, "HEAD") && !new_branch_info->رستو && !opts->force_detach) {
		/* Nothing to do. */
	} وإلا إذا (opts->force_detach || !new_branch_info->رستو) {	/* No longer on any branch. */
		refs_update_ref(get_main_ref_store(the_repository), نياپو.buf,
				"HEAD", &new_branch_info->commit->object.oid,
				عدم,
				REF_NO_DEREF, UPDATE_REFS_DIE_ON_ERR);
		إذا (!opts->quiet) {
			إذا (old_branch_info->رستو &&
			    advice_enabled(ADVICE_DETACHED_سر) && !opts->force_detach)
				detach_advice(new_branch_info->name);
			describe_detached_سر(_("HEAD is now at"), new_branch_info->commit);
		}
	} وإلا إذا (new_branch_info->رستو) {	/* Switch branches. */
		إذا (refs_update_symref(get_main_ref_store(the_repository), "HEAD", new_branch_info->رستو, نياپو.buf) < 0)
			die(_("unable to update HEAD"));
		إذا (!opts->quiet) {
			إذا (old_branch_info->رستو && !strcmp(new_branch_info->رستو, old_branch_info->رستو)) {
				إذا (opts->new_branch_force)
					fprintf(stderr, _("Reset branch '%s'\n"),
						new_branch_info->name);
				وإلا
					fprintf(stderr, _("Already on '%s'\n"),
						new_branch_info->name);
			} وإلا إذا (opts->new_branch) {
				إذا (opts->branch_exists)
					fprintf(stderr, _("Switched to and reset branch '%s'\n"), new_branch_info->name);
				وإلا
					fprintf(stderr, _("Switched to a new branch '%s'\n"), new_branch_info->name);
			} وإلا {
				fprintf(stderr, _("Switched to branch '%s'\n"),
					new_branch_info->name);
			}
		}
		إذا (old_branch_info->رستو && old_branch_info->name) {
			إذا (!refs_ref_exists(get_main_ref_store(the_repository), old_branch_info->رستو) && refs_reflog_exists(get_main_ref_store(the_repository), old_branch_info->رستو))
				refs_delete_reflog(get_main_ref_store(the_repository),
						   old_branch_info->رستو);
		}
	}
	remove_branch_state(the_repository, !opts->quiet);
	strbuf_release(&نياپو);
	إذا (!opts->quiet &&
	    !opts->force_detach &&
	    (new_branch_info->رستو || !strcmp(new_branch_info->name, "HEAD")))
		report_tracking(new_branch_info);
}

ساكن صحيح جوڙيو_pending_uninteresting_ref(ثابت بنية reference *ref, فراغ *cb_معلومات)
{
	جوڙيو_pending_oid(cb_معلومات, ref->name, ref->oid, UNINTERESTING);
	أرجع 0;
}

ساكن فراغ describe_one_orphan(بنية strbuf *sb, بنية commit *commit)
{
	strbuf_addstr(sb, "  ");
	strbuf_جوڙيو_unique_abbrev(sb, &commit->object.oid, DEFAULT_ABBREV);
	strbuf_addch(sb, ' ');
	إذا (!repo_parse_commit(the_repository, commit))
		pp_commit_easy(CMIT_FMT_ONELINE, commit, sb);
	strbuf_addch(sb, '\n');
}

#وصف_ڪريو ORPHAN_CUTOFF 4
ساكن فراغ suggest_reattach(بنية commit *commit, بنية rev_info *revs)
{
	بنية commit *c, *آخري = عدم;
	بنية strbuf sb = STRBUF_INIT;
	صحيح گم = 0;
	طالما ((c = get_revision(revs)) != عدم) {
		إذا (گم < ORPHAN_CUTOFF)
			describe_one_orphan(&sb, c);
		آخري = c;
		گم++;
	}
	إذا (ORPHAN_CUTOFF < گم) {
		صحيح وڌيڪ = گم - ORPHAN_CUTOFF;
		إذا (وڌيڪ == 1)
			describe_one_orphan(&sb, آخري);
		وإلا
			strbuf_addf(&sb, _(" ... and %d more.\n"), وڌيڪ);
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
		گم),
		گم,
		sb.buf);
	strbuf_release(&sb);

	إذا (advice_enabled(ADVICE_DETACHED_سر))
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
			گم),
			repo_ڳوليو_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV));
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
	تياري_revisions(0, عدم, &revs, عدم);

	object->flags &= ~UNINTERESTING;
	جوڙيو_pending_object(&revs, object, oid_منزل_hex(&object->oid));

	refs_for_each_ref(get_main_ref_store(the_repository),
			  جوڙيو_pending_uninteresting_ref, &revs);
	إذا (new_commit)
		جوڙيو_pending_oid(&revs, "HEAD",
				&new_commit->object.oid,
				UNINTERESTING);

	إذا (prepare_revision_walk(&revs))
		die(_("internal error in revision walk"));
	إذا (!(old_commit->object.flags & UNINTERESTING))
		suggest_reattach(old_commit, &revs);
	وإلا
		describe_detached_سر(_("Previous HEAD position was"), old_commit);

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
	صحيح do_ملايو = 1;
	صحيح created_autostash = 0;
	تعداد stash_apply_نتيجو autostash_res = STASH_APPLY_CLEAN;
	بنية strbuf old_commit_shortname = STRBUF_INIT;
	بنية strbuf autostash_نياپو = STRBUF_INIT;
	ثابت حرف *stash_label_بنياد = عدم;

	trace2_cmd_گهڻائي("branch");

	memset(&old_branch_info, 0, حجم(old_branch_info));
	old_branch_info.رستو = refs_resolve_refdup(get_main_ref_store(the_repository),
						   "HEAD", 0, &rev, &flag);
	إذا (old_branch_info.رستو)
		old_branch_info.commit = lookup_commit_reference_gently(the_repository, &rev, 1);
	إذا (!(flag & REF_ISSYMREF))
		FREE_AND_NULL(old_branch_info.رستو);

	إذا (old_branch_info.رستو) {
		ثابت حرف *ثابت اڳياڙي = "refs/heads/";
		ثابت حرف *p;
		إذا (skip_اڳياڙي(old_branch_info.رستو, اڳياڙي, &p))
			old_branch_info.name = xstrdup(p);
	}

	إذا (opts->new_orphan_branch && opts->orphan_from_خالي_جايون_وڻ) {
		إذا (new_branch_info->name)
			BUG("'switch --orphan' should never accept a commit as starting point");
		new_branch_info->commit = عدم;
		new_branch_info->name = xstrdup("(empty)");
		do_ملايو = 1;
	}

	إذا (!new_branch_info->name) {
		new_branch_info->name = xstrdup("HEAD");
		new_branch_info->commit = old_branch_info.commit;
		إذا (!new_branch_info->commit)
			die(_("You are on a branch yet to be born"));
		parse_commit_or_die(new_branch_info->commit);

		إذا (opts->only_ملايو_on_switching_branches)
			do_ملايو = 0;
	}

	إذا (old_branch_info.name) {
		stash_label_بنياد = old_branch_info.name;
	} وإلا إذا (old_branch_info.commit) {
		strbuf_جوڙيو_unique_abbrev(&old_commit_shortname,
					 &old_branch_info.commit->object.oid,
					 DEFAULT_ABBREV);
		stash_label_بنياد = old_commit_shortname.buf;
	}

	إذا (do_ملايو) {
		ret = ملايو_working_وڻ(opts, &old_branch_info, new_branch_info,
					 opts->ملايو, &writeout_error);
		إذا (ret == ملايو_WORKING_وڻ_UNPACK_FAILED && opts->ملايو) {
			strbuf_addf(&autostash_نياپو,
				    "autostash while switching to '%s'",
				    new_branch_info->name);
			create_autostash_ref(the_repository,
					     "CHECKOUT_AUTOSTASH_HEAD",
					     autostash_نياپو.buf, true);
			created_autostash = 1;
			ret = ملايو_working_وڻ(opts, &old_branch_info, new_branch_info,
						 false, &writeout_error);
		}
		إذا (created_autostash) {
			إذا (opts->conflict_style >= 0) {
				بنية strbuf cfg = STRBUF_INIT;
				strbuf_addf(&cfg, "merge.conflictStyle=%s",
					    conflict_style_name(opts->conflict_style));
				git_config_ڌڪو_parameter(cfg.buf);
				strbuf_release(&cfg);
			}
			autostash_res = apply_autostash_ref(the_repository,
				    "CHECKOUT_AUTOSTASH_HEAD",
				    new_branch_info->name,
				    "local",
				    stash_label_بنياد,
				    autostash_نياپو.buf);
		}
		إذا (ret) {
			branch_info_release(&old_branch_info);
			strbuf_release(&old_commit_shortname);
			strbuf_release(&autostash_نياپو);
			أرجع ret < 0 ? 1 : ret;
		}
	}

	إذا (!opts->quiet && !old_branch_info.رستو && old_branch_info.commit && new_branch_info->commit != old_branch_info.commit)
		orphaned_commit_warning(old_branch_info.commit, new_branch_info->commit);

	إذا (autostash_res == STASH_APPLY_CONFLICT && !opts->quiet)
		fputc('\n', stderr);
	update_refs_for_switch(opts, &old_branch_info, new_branch_info);

	إذا (created_autostash) {
		discard_اشاريو(the_repository->اشاريو);
		إذا (repo_read_اشاريو(the_repository) < 0)
			die(_("index file corrupt"));

		إذا (!opts->quiet && new_branch_info->commit) {
			لکو(_("The following paths have local changes:\n"));
			show_local_changes(&new_branch_info->commit->object,
					   &opts->diff_options);
		}
	}

	ret = post_checkout_hook(old_branch_info.commit, new_branch_info->commit, 1);
	branch_info_release(&old_branch_info);
	strbuf_release(&old_commit_shortname);
	strbuf_release(&autostash_نياپو);

	أرجع ret || writeout_error;
}

ساكن صحيح git_checkout_config(ثابت حرف *var, ثابت حرف *قدر,
			       ثابت بنية config_context *ctx, فراغ *cb)
{
	بنية checkout_opts *opts = cb;

	إذا (!strcmp(var, "diff.ignoresubmodules")) {
		إذا (!قدر)
			أرجع config_error_nonbool(var);
		handle_ignore_submodules_arg(&opts->diff_options, قدر);
		أرجع 0;
	}
	إذا (!strcmp(var, "checkout.guess")) {
		opts->dwim_new_local_branch = git_config_bool(var, قدر);
		أرجع 0;
	}

	إذا (starts_with(var, "submodule."))
		أرجع git_default_submodule_config(var, قدر, عدم);

	أرجع git_xmerge_config(var, قدر, ctx, عدم);
}

ساكن فراغ تياري_new_branch_info_and_ذريعو_وڻ(
	بنية branch_info *new_branch_info,
	بنية checkout_opts *opts,
	بنية object_id *rev,
	ثابت حرف *arg)
{
	بنية وڻ **ذريعو_وڻ = &opts->ذريعو_وڻ;
	بنية object_id branch_rev;

	/* treat '@' as a shortcut for 'HEAD' */
	new_branch_info->name = !strcmp(arg, "@") ? xstrdup("HEAD") :
						    xstrdup(arg);
	تياري_branch_رستو(new_branch_info);

	إذا (!check_refname_format(new_branch_info->رستو, 0) &&
	    !refs_read_ref(get_main_ref_store(the_repository), new_branch_info->رستو, &branch_rev))
		oidcpy(rev, &branch_rev);
	وإلا
		/* not an existing branch */
		FREE_AND_NULL(new_branch_info->رستو);

	new_branch_info->commit = lookup_commit_reference_gently(the_repository, rev, 1);
	إذا (!new_branch_info->commit) {
		/* not a commit */
		*ذريعو_وڻ = repo_parse_وڻ_اڻسڌو(the_repository, rev);
		إذا (!*ذريعو_وڻ)
			die(_("unable to read tree (%s)"), oid_منزل_hex(rev));
	} وإلا {
		parse_commit_or_die(new_branch_info->commit);
		*ذريعو_وڻ = repo_get_commit_وڻ(the_repository,
						    new_branch_info->commit);
		إذا (!*ذريعو_وڻ)
			die(_("unable to read tree (%s)"),
			    oid_منزل_hex(&new_branch_info->commit->object.oid));
	}
}


تعداد checkout_command {
	CHECKOUT_CHECKOUT = 1,
	CHECKOUT_SWITCH = 2,
	CHECKOUT_RESTORE = 3,
};

ساكن فراغ advise_disambiguating_remotes(تعداد checkout_command which_command,
					  ثابت حرف *branch,
					  ثابت بنية string_list *matched_remote_نالا)
{
	ثابت حرف *cmdname;
	بنية string_list_شي *شي;

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
	for_each_string_list_شي(شي, matched_remote_نالا) {
		advise(_("  %s"), شي->string);
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
	صحيح انگ_matches = 0;
	بنية string_list matched_remote_نالا = STRING_LIST_INIT_DUP;

	حرف *remote = unique_tracking_name(arg, rev, &انگ_matches,
					    &matched_remote_نالا);

	إذا (remote && could_be_checkout_paths) {
		die(_("'%s' could be both a local file and a tracking branch.\n"
			"Please use -- (and optionally --no-guess) to disambiguate"),
		    arg);
	}

	إذا (!remote && انگ_matches > 1) {
		إذا (advice_enabled(ADVICE_CHECKOUT_AMBIGUOUS_REMOTE_BRANCH_NAME))
			advise_disambiguating_remotes(which_command, arg,
						      &matched_remote_نالا);
		die(_("'%s' matched multiple (%d) remote tracking branches"),
		    arg, انگ_matches);
	}

	string_list_clear(&matched_remote_نالا, 0);

	أرجع remote;
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
	إذا (!argc)
		أرجع 0;

	إذا (!opts->accept_pathspec) {
		إذا (argc > 1)
			die(_("only one reference expected"));
		has_dash_dash = 1; /* helps disambiguate */
	}

	arg = argv[0];
	dash_dash_pos = -1;
	لكل (i = 0; i < argc; i++) {
		إذا (opts->accept_pathspec && !strcmp(argv[i], "--")) {
			dash_dash_pos = i;
			اكسر;
		}
	}
	إذا (dash_dash_pos == 0)
		أرجع 1; /* case (2) */
	وإلا إذا (dash_dash_pos == 1)
		has_dash_dash = 1; /* case (3) or (1) */
	وإلا إذا (dash_dash_pos >= 2)
		die(_("only one reference expected, %d given."), dash_dash_pos);
	opts->ڳڻپ_checkout_paths = !opts->quiet && !has_dash_dash;

	إذا (!strcmp(arg, "-"))
		arg = "@{-1}";

	إذا (repo_get_oid_mb(the_repository, arg, rev)) {
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
			check_filename(opts->اڳياڙي, arg);

		إذا (!has_dash_dash && !no_wildcard(arg))
			recover_with_dwim = 0;

		/*
		 * Accept "git checkout foo", "git checkout foo --"
		 * and "git switch foo" as candidates for dwim.
		 */
		إذا (!(argc == 1 && !has_dash_dash) &&
		    !(argc == 2 && has_dash_dash) &&
		    opts->accept_pathspec)
			recover_with_dwim = 0;

		إذا (recover_with_dwim) {
			remote = parse_remote_branch(arg, rev,
						     could_be_checkout_paths,
						     which_command);
			إذا (remote) {
				*new_branch = arg;
				arg = remote;
				/* DWIMmed to create local branch, case (3).(b) */
			} وإلا {
				recover_with_dwim = 0;
			}
		}

		إذا (!recover_with_dwim) {
			إذا (has_dash_dash)
				die(_("invalid reference: %s"), arg);
			أرجع argcount;
		}
	}

	/* we can't end up being in (2) anymore, eat the argument */
	argcount++;
	argv++;
	argc--;

	تياري_new_branch_info_and_ذريعو_وڻ(new_branch_info, opts, rev, arg);

	إذا (!opts->ذريعو_وڻ)                   /* case (1): want a tree */
		die(_("reference is not a tree: %s"), arg);

	إذا (!has_dash_dash) {	/* case (3).(d) -> (1) */
		/*
		 * Do not complain the most common case
		 *	git checkout branch
		 * even if there happen to be a file called 'branch';
		 * it would be extremely annoying.
		 */
		إذا (argc)
			verify_non_filename(the_repository, opts->اڳياڙي, arg);
	} وإلا إذا (opts->accept_pathspec) {
		argcount++;
		argv++;
		argc--;
	}

	حرر(remote);
	أرجع argcount;
}

ساكن صحيح switch_unborn_منزل_new_branch(ثابت بنية checkout_opts *opts)
{
	صحيح حالت;
	بنية strbuf branch_ref = STRBUF_INIT;

	trace2_cmd_گهڻائي("unborn");

	إذا (!opts->new_branch)
		die(_("You are on a branch yet to be born"));
	strbuf_addf(&branch_ref, "refs/heads/%s", opts->new_branch);
	حالت = refs_update_symref(get_main_ref_store(the_repository),
				    "HEAD", branch_ref.buf, "checkout -b");
	strbuf_release(&branch_ref);
	إذا (!opts->quiet)
		fprintf(stderr, _("Switched to a new branch '%s'\n"),
			opts->new_branch);
	أرجع حالت;
}

ساكن فراغ die_expecting_a_branch(ثابت بنية branch_info *branch_info)
{
	بنية object_id oid;
	حرف *منزل_free;
	صحيح ڪوڊ;

	إذا (repo_dwim_ref(the_repository, branch_info->name,
			  لڙي_ڊيگهه(branch_info->name), &oid, &منزل_free, 0) == 1) {
		ثابت حرف *ref = منزل_free;

		إذا (skip_اڳياڙي(ref, "refs/tags/", &ref))
			ڪوڊ = die_پيغام(_("a branch is expected, got tag '%s'"), ref);
		وإلا إذا (skip_اڳياڙي(ref, "refs/remotes/", &ref))
			ڪوڊ = die_پيغام(_("a branch is expected, got remote branch '%s'"), ref);
		وإلا
			ڪوڊ = die_پيغام(_("a branch is expected, got '%s'"), ref);
	}
	وإلا إذا (branch_info->commit)
		ڪوڊ = die_پيغام(_("a branch is expected, got commit '%s'"), branch_info->name);
	وإلا
		/*
		 * This case should never happen because we already die() on
		 * non-commit, but just in case.
		 */
		ڪوڊ = die_پيغام(_("a branch is expected, got '%s'"), branch_info->name);

	إذا (advice_enabled(ADVICE_SUGGEST_DETACHING_سر))
		advise(_("If you want to detach HEAD at the commit, try again with the --detach option."));

	ٻاهر_نڪرو(ڪوڊ);
}

ساكن فراغ die_if_some_operation_اندريون_ترقي(فراغ)
{
	بنية wt_حالت_state state;

	memset(&state, 0, حجم(state));
	wt_حالت_get_state(the_repository, &state, 0);

	إذا (state.ملايو_اندريون_ترقي)
		die(_("cannot switch branch while merging\n"
		      "Consider \"git merge --quit\" "
		      "or \"git worktree add\"."));
	إذا (state.am_اندريون_ترقي)
		die(_("cannot switch branch in the middle of an am session\n"
		      "Consider \"git am --quit\" "
		      "or \"git worktree add\"."));
	إذا (state.rebase_interactive_اندريون_ترقي || state.rebase_اندريون_ترقي)
		die(_("cannot switch branch while rebasing\n"
		      "Consider \"git rebase --quit\" "
		      "or \"git worktree add\"."));
	إذا (state.cherry_pick_اندريون_ترقي)
		die(_("cannot switch branch while cherry-picking\n"
		      "Consider \"git cherry-pick --quit\" "
		      "or \"git worktree add\"."));
	إذا (state.revert_اندريون_ترقي)
		die(_("cannot switch branch while reverting\n"
		      "Consider \"git revert --quit\" "
		      "or \"git worktree add\"."));
	إذا (state.bisect_اندريون_ترقي)
		warning(_("you are switching branch while bisecting"));

	wt_حالت_state_free_buffers(&state);
}

/*
 * die if attempting to checkout an existing branch that is in use
 * in another worktree, unless ignore-other-wortrees option is given.
 * The check is bypassed when the branch is already the current one,
 * as it will not make things any worse.
 */
ساكن فراغ die_if_switching_منزل_a_branch_اندريون_use(بنية checkout_opts *opts,
						ثابت حرف *ڀريل_ref)
{
	صحيح flags;
	حرف *سر_ref;

	إذا (opts->ignore_other_worktrees)
		أرجع;
	سر_ref = refs_resolve_refdup(get_main_ref_store(the_repository),
				       "HEAD", 0, عدم, &flags);
	إذا (سر_ref && (!(flags & REF_ISSYMREF) || strcmp(سر_ref, ڀريل_ref)))
		die_if_checked_ٻاهريون(ڀريل_ref, 1);
	حرر(سر_ref);
}

ساكن صحيح checkout_branch(بنية checkout_opts *opts,
			   بنية branch_info *new_branch_info)
{
	بنية repo_config_قدرون *cfg = repo_config_قدرون(the_repository);
	صحيح noop_switch = (!new_branch_info->name &&
			   !opts->new_branch &&
			   !opts->force_detach);

	إذا (opts->pathspec.nr)
		die(_("paths cannot be used with switching branches"));

	إذا (opts->patch_گهڻائي)
		die(_("'%s' cannot be used with switching branches"),
		    "--patch");

	إذا (opts->overlay_گهڻائي != -1)
		die(_("'%s' cannot be used with switching branches"),
		    "--[no]-overlay");

	إذا (opts->writeout_stage) {
		ثابت حرف *نياپو;
		إذا (noop_switch)
			نياپو = _("'%s' needs the paths to check out");
		وإلا
			نياپو = _("'%s' cannot be used with switching branches");
		die(نياپو, "--ours/--theirs");
	}

	إذا (opts->force && opts->ملايو)
		die(_("'%s' cannot be used with '%s'"), "-f", "-m");

	إذا (opts->discard_changes && opts->ملايو)
		die(_("'%s' cannot be used with '%s'"), "--discard-changes", "--merge");

	إذا (opts->force_detach && opts->new_branch)
		die(_("'%s' cannot be used with '%s'"),
		    "--detach", "-b/-B/--orphan");

	إذا (opts->new_orphan_branch) {
		إذا (opts->track != BRANCH_TRACK_UNSPECIFIED)
			die(_("'%s' cannot be used with '%s'"), "--orphan", "-t");
		إذا (opts->orphan_from_خالي_جايون_وڻ && new_branch_info->name)
			die(_("'%s' cannot take <start-point>"), "--orphan");
	} وإلا إذا (opts->force_detach) {
		إذا (opts->track != BRANCH_TRACK_UNSPECIFIED)
			die(_("'%s' cannot be used with '%s'"), "--detach", "-t");
	} وإلا إذا (opts->track == BRANCH_TRACK_UNSPECIFIED)
		opts->track = cfg->branch_track;

	إذا (new_branch_info->name && !new_branch_info->commit)
		die(_("Cannot switch branch to a non-commit '%s'"),
		    new_branch_info->name);

	إذا (noop_switch &&
	    !opts->switch_branch_doing_nothing_is_ok)
		die(_("missing branch or commit argument"));

	إذا (!opts->implicit_detach &&
	    !opts->force_detach &&
	    !opts->new_branch &&
	    !opts->new_branch_force &&
	    new_branch_info->name &&
	    !new_branch_info->رستو)
		die_expecting_a_branch(new_branch_info);

	إذا (!opts->can_switch_when_اندريون_ترقي)
		die_if_some_operation_اندريون_ترقي();

	/* "git checkout <branch>" */
	إذا (new_branch_info->رستو && !opts->force_detach && !opts->new_branch)
		die_if_switching_منزل_a_branch_اندريون_use(opts, new_branch_info->رستو);

	/* "git checkout -B <branch>" */
	إذا (opts->new_branch_force) {
		حرف *ڀريل_ref = xstrfmt("refs/heads/%s", opts->new_branch);
		die_if_switching_منزل_a_branch_اندريون_use(opts, ڀريل_ref);
		حرر(ڀريل_ref);
	}

	إذا (!new_branch_info->commit && opts->new_branch) {
		بنية object_id rev;
		صحيح flag;

		إذا (!refs_read_ref_ڀريل(get_main_ref_store(the_repository), "HEAD", 0, &rev, &flag) &&
		    (flag & REF_ISSYMREF) && is_null_oid(&rev))
			أرجع switch_unborn_منزل_new_branch(opts);
	}
	أرجع switch_branches(opts, new_branch_info);
}

ساكن صحيح parse_opt_conflict(ثابت بنية option *o, ثابت حرف *arg, صحيح unset)
{
	بنية checkout_opts *opts = o->قدر;

	إذا (unset) {
		opts->conflict_style = -1;
		أرجع 0;
	}
	opts->conflict_style = parse_conflict_style_name(arg);
	إذا (opts->conflict_style < 0)
		أرجع error(_("unknown conflict style '%s'"), arg);
	/* --conflict overrides a previous --no-merge */
	إذا (!opts->ملايو)
		opts->ملايو = -1;

	أرجع 0;
}

ساكن بنية option *جوڙيو_common_options(بنية checkout_opts *opts,
					 بنية option *prevopts)
{
	بنية option options[] = {
		OPT__QUIET(&opts->quiet, N_("suppress progress reporting")),
		OPT_CALLBACK_F(0, "recurse-submodules", عدم,
			    "checkout", "control recursive updating of submodules",
			    PARSE_OPT_OPTARG, option_parse_recurse_submodules_worktree_updater),
		OPT_BOOL(0, "progress", &opts->show_ترقي, N_("force progress reporting")),
		OPT_BOOL('m', "merge", &opts->ملايو, N_("perform a 3-way merge with the new branch")),
		OPT_CALLBACK(0, "conflict", opts, N_("style"),
			     N_("conflict style (merge, diff3, or zdiff3)"),
			     parse_opt_conflict),
		OPT_پڇاڙي()
	};
	بنية option *newopts = parse_options_concat(prevopts, options);
	حرر(prevopts);
	أرجع newopts;
}

ساكن بنية option *جوڙيو_common_switch_branch_options(
	بنية checkout_opts *opts, بنية option *prevopts)
{
	بنية option options[] = {
		OPT_BOOL('d', "detach", &opts->force_detach, N_("detach HEAD at named commit")),
		OPT_CALLBACK_F('t', "track",  &opts->track, "(direct|inherit)",
			N_("set branch tracking configuration"),
			PARSE_OPT_OPTARG,
			parse_opt_tracking_گهڻائي),
		OPT__FORCE(&opts->force, N_("force checkout (throw away local modifications)"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_STRING(0, "orphan", &opts->new_orphan_branch, N_("new-branch"), N_("new unborn branch")),
		OPT_BOOL_F(0, "overwrite-ignore", &opts->overwrite_ignore,
			   N_("update ignored files (default)"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "ignore-other-worktrees", &opts->ignore_other_worktrees,
			 N_("do not check if another worktree is using this branch")),
		OPT_پڇاڙي()
	};
	بنية option *newopts = parse_options_concat(prevopts, options);
	حرر(prevopts);
	أرجع newopts;
}

ساكن بنية option *جوڙيو_checkout_رستو_options(بنية checkout_opts *opts,
						بنية option *prevopts)
{
	بنية option options[] = {
		OPT_سيٽ_INT_F('2', "ours", &opts->writeout_stage,
			      N_("checkout our version for unmerged files"),
			      2, PARSE_OPT_NONEG),
		OPT_سيٽ_INT_F('3', "theirs", &opts->writeout_stage,
			      N_("checkout their version for unmerged files"),
			      3, PARSE_OPT_NONEG),
		OPT_BOOL('p', "patch", &opts->patch_گهڻائي, N_("select hunks interactively")),
		OPT_DIFF_UNIFIED(&opts->patch_context),
		OPT_DIFF_INTERHUNK_CONTEXT(&opts->patch_interhunk_context),
		OPT_BOOL(0, "ignore-skip-worktree-bits", &opts->ignore_skipworktree,
			 N_("do not limit pathspecs to sparse entries only")),
		OPT_PATHSPEC_FROM_FILE(&opts->pathspec_from_file),
		OPT_PATHSPEC_FILE_NUL(&opts->pathspec_file_nul),
		OPT_پڇاڙي()
	};
	بنية option *newopts = parse_options_concat(prevopts, options);
	حرر(prevopts);
	أرجع newopts;
}

/* create-branch option (either b or c) */
ساكن حرف cb_option = 'b';

ساكن صحيح checkout_main(صحيح argc, ثابت حرف **argv, ثابت حرف *اڳياڙي,
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
	opts->اڳياڙي = اڳياڙي;
	opts->show_ترقي = -1;

	repo_config(the_repository, git_checkout_config, opts);
	إذا (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_ڀريل_اشاريو = 0;
	}

	opts->track = BRANCH_TRACK_UNSPECIFIED;

	إذا (!opts->accept_pathspec && !opts->accept_ref)
		BUG("make up your mind, you need to take _something_");
	إذا (opts->accept_pathspec && opts->accept_ref)
		parseopt_flags = PARSE_OPT_KEEP_DASHDASH;

	argc = parse_options(argc, argv, اڳياڙي, options,
			     usagestr, parseopt_flags);

	إذا (opts->patch_context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	إذا (opts->patch_interhunk_context < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	إذا (!opts->patch_گهڻائي) {
		إذا (opts->patch_context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--patch");
		إذا (opts->patch_interhunk_context != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--patch");
		إذا (!opts->auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--patch");
	}

	إذا (opts->show_ترقي < 0) {
		إذا (opts->quiet)
			opts->show_ترقي = 0;
		وإلا
			opts->show_ترقي = isatty(2);
	}

	/* --conflicts implies --merge */
	إذا (opts->ملايو == -1)
		opts->ملايو = opts->conflict_style >= 0;

	إذا (opts->force) {
		opts->discard_changes = 1;
		opts->ignore_unmerged_opt = "--force";
		opts->ignore_unmerged = 1;
	}

	إذا ((!!opts->new_branch + !!opts->new_branch_force + !!opts->new_orphan_branch) > 1)
		die(_("options '-%c', '-%c', and '%s' cannot be used together"),
			cb_option, وڏو_اکر(cb_option), "--orphan");

	إذا (opts->overlay_گهڻائي == 1 && opts->patch_گهڻائي)
		die(_("options '%s' and '%s' cannot be used together"), "-p", "--overlay");

	إذا (opts->checkout_اشاريو >= 0 || opts->checkout_worktree >= 0) {
		إذا (opts->checkout_اشاريو < 0)
			opts->checkout_اشاريو = 0;
		إذا (opts->checkout_worktree < 0)
			opts->checkout_worktree = 0;
	} وإلا {
		إذا (opts->checkout_اشاريو < 0)
			opts->checkout_اشاريو = -opts->checkout_اشاريو - 1;
		إذا (opts->checkout_worktree < 0)
			opts->checkout_worktree = -opts->checkout_worktree - 1;
	}
	إذا (opts->checkout_اشاريو < 0 || opts->checkout_worktree < 0)
		BUG("these flags should be non-negative by now");
	/*
	 * convenient shortcut: "git restore --staged [--worktree]" equals
	 * "git restore --staged [--worktree] --source HEAD"
	 */
	إذا (!opts->from_treeish && opts->checkout_اشاريو)
		opts->from_treeish = "HEAD";

	/*
	 * From here on, new_branch will contain the branch to be checked out,
	 * and new_branch_force and new_orphan_branch will tell us which one of
	 * -b/-B/-c/-C/--orphan is being used.
	 */
	إذا (opts->new_branch_force)
		opts->new_branch = opts->new_branch_force;

	إذا (opts->new_orphan_branch)
		opts->new_branch = opts->new_orphan_branch;

	/* --track without -c/-C/-b/-B/--orphan should DWIM */
	إذا (opts->track != BRANCH_TRACK_UNSPECIFIED && !opts->new_branch) {
		ثابت حرف *argv0 = argv[0];
		إذا (!argc || !strcmp(argv0, "--"))
			die(_("--track needs a branch name"));
		skip_اڳياڙي(argv0, "refs/", &argv0);
		skip_اڳياڙي(argv0, "remotes/", &argv0);
		argv0 = strchr(argv0, '/');
		إذا (!argv0 || !argv0[1])
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
	إذا (argc && opts->accept_ref) {
		بنية object_id rev;
		صحيح dwim_ok =
			!opts->patch_گهڻائي &&
			opts->dwim_new_local_branch &&
			opts->track == BRANCH_TRACK_UNSPECIFIED &&
			!opts->new_branch;
		صحيح n = parse_branchname_arg(argc, argv, dwim_ok, which_command,
					     &new_branch_info, opts, &rev);
		argv += n;
		argc -= n;
	} وإلا إذا (!opts->accept_ref && opts->from_treeish) {
		بنية object_id rev;

		إذا (repo_get_oid_mb(the_repository, opts->from_treeish, &rev))
			die(_("could not resolve '%s'"), opts->from_treeish);

		تياري_new_branch_info_and_ذريعو_وڻ(&new_branch_info,
						      opts, &rev,
						      opts->from_treeish);

		إذا (!opts->ذريعو_وڻ)
			die(_("reference is not a tree: %s"), opts->from_treeish);
	}

	إذا (argc) {
		parse_pathspec(&opts->pathspec, 0,
			       opts->patch_گهڻائي ? PATHSPEC_اڳياڙي_ORIGIN : 0,
			       اڳياڙي, argv);

		إذا (!opts->pathspec.nr)
			die(_("invalid path specification"));

		/*
		 * Try to give more helpful suggestion.
		 * new_branch && argc > 1 will be caught later.
		 */
		إذا (opts->new_branch && argc == 1 && !new_branch_info.commit)
			die(_("'%s' is not a commit and a branch '%s' cannot be created from it"),
				argv[0], opts->new_branch);

		إذا (opts->force_detach)
			die(_("git checkout: --detach does not take a path argument '%s'"),
			    argv[0]);
	}

	إذا (opts->pathspec_from_file) {
		إذا (opts->pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		إذا (opts->force_detach)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--detach");

		إذا (opts->patch_گهڻائي)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--patch");

		parse_pathspec_file(&opts->pathspec, 0,
				    0,
				    اڳياڙي, opts->pathspec_from_file, opts->pathspec_file_nul);
	} وإلا إذا (opts->pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	opts->pathspec.recursive = 1;

	إذا (opts->pathspec.nr) {
		إذا (1 < !!opts->writeout_stage + !!opts->force + !!opts->ملايو)
			die(_("git checkout: --ours/--theirs, --force and --merge are incompatible when\n"
			      "checking out of the index."));
	} وإلا {
		إذا (opts->accept_pathspec && !opts->خالي_جايون_pathspec_ok &&
		    !opts->patch_گهڻائي)	/* patch mode is special */
			die(_("you must specify path(s) to restore"));
	}

	إذا (opts->new_branch) {
		بنية strbuf buf = STRBUF_INIT;

		إذا (opts->new_branch_force)
			opts->branch_exists = validate_branchname(opts->new_branch, &buf);
		وإلا
			opts->branch_exists =
				validate_new_branchname(opts->new_branch, &buf, 0);
		strbuf_release(&buf);
	}

	إذا (opts->patch_گهڻائي || opts->pathspec.nr)
		ret = checkout_paths(opts, &new_branch_info);
	وإلا
		ret = checkout_branch(opts, &new_branch_info);

	branch_info_release(&new_branch_info);
	clear_pathspec(&opts->pathspec);
	حرر(opts->pathspec_from_file);
	حرر(options);

	أرجع ret;
}

صحيح cmd_checkout(صحيح argc,
		 ثابت حرف **argv,
		 ثابت حرف *اڳياڙي,
		 بنية repository *repo اڻ_استعمال_ٿيل)
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
		OPT_BOOL(0, "overlay", &opts.overlay_گهڻائي, N_("use overlay mode (default)")),
		OPT_BOOL(0, "auto-advance", &opts.auto_advance,
			 N_("auto advance to the next file when selecting hunks interactively")),
		OPT_پڇاڙي()
	};

	opts.dwim_new_local_branch = 1;
	opts.switch_branch_doing_nothing_is_ok = 1;
	opts.only_ملايو_on_switching_branches = 0;
	opts.accept_ref = 1;
	opts.accept_pathspec = 1;
	opts.implicit_detach = 1;
	opts.can_switch_when_اندريون_ترقي = 1;
	opts.orphan_from_خالي_جايون_وڻ = 0;
	opts.خالي_جايون_pathspec_ok = 1;
	opts.overlay_گهڻائي = -1;
	opts.checkout_اشاريو = -2;    /* default on */
	opts.checkout_worktree = -2; /* default on */

	إذا (argc == 3 && !strcmp(argv[1], "-b")) {
		/*
		 * User ran 'git checkout -b <branch>' and expects
		 * the same behavior as 'git switch -c <branch>'.
		 */
		opts.switch_branch_doing_nothing_is_ok = 0;
		opts.only_ملايو_on_switching_branches = 1;
	}

	options = parse_options_dup(checkout_options);
	options = جوڙيو_common_options(&opts, options);
	options = جوڙيو_common_switch_branch_options(&opts, options);
	options = جوڙيو_checkout_رستو_options(&opts, options);

	أرجع checkout_main(argc, argv, اڳياڙي, &opts, options,
			     CHECKOUT_CHECKOUT);
}

صحيح cmd_switch(صحيح argc,
	       ثابت حرف **argv,
	       ثابت حرف *اڳياڙي,
	       بنية repository *repo اڻ_استعمال_ٿيل)
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
		OPT_پڇاڙي()
	};

	opts.dwim_new_local_branch = 1;
	opts.accept_ref = 1;
	opts.accept_pathspec = 0;
	opts.switch_branch_doing_nothing_is_ok = 0;
	opts.only_ملايو_on_switching_branches = 1;
	opts.implicit_detach = 0;
	opts.can_switch_when_اندريون_ترقي = 0;
	opts.orphan_from_خالي_جايون_وڻ = 1;
	opts.overlay_گهڻائي = -1;

	options = parse_options_dup(switch_options);
	options = جوڙيو_common_options(&opts, options);
	options = جوڙيو_common_switch_branch_options(&opts, options);

	cb_option = 'c';

	أرجع checkout_main(argc, argv, اڳياڙي, &opts, options,
			     CHECKOUT_SWITCH);
}

صحيح cmd_restore(صحيح argc,
		ثابت حرف **argv,
		ثابت حرف *اڳياڙي,
		بنية repository *repo اڻ_استعمال_ٿيل)
{
	بنية checkout_opts opts = CHECKOUT_OPTS_INIT;
	بنية option *options;
	بنية option restore_options[] = {
		OPT_STRING('s', "source", &opts.from_treeish, "<tree-ish>",
			   N_("which tree-ish to checkout from")),
		OPT_BOOL('S', "staged", &opts.checkout_اشاريو,
			   N_("restore the index")),
		OPT_BOOL('W', "worktree", &opts.checkout_worktree,
			   N_("restore the working tree (default)")),
		OPT_BOOL(0, "ignore-unmerged", &opts.ignore_unmerged,
			 N_("ignore unmerged entries")),
		OPT_BOOL(0, "overlay", &opts.overlay_گهڻائي, N_("use overlay mode")),
		OPT_پڇاڙي()
	};

	opts.accept_ref = 0;
	opts.accept_pathspec = 1;
	opts.خالي_جايون_pathspec_ok = 0;
	opts.overlay_گهڻائي = 0;
	opts.checkout_اشاريو = -1;    /* default off */
	opts.checkout_worktree = -2; /* default on */
	opts.ignore_unmerged_opt = "--ignore-unmerged";

	options = parse_options_dup(restore_options);
	options = جوڙيو_common_options(&opts, options);
	options = جوڙيو_checkout_رستو_options(&opts, options);

	أرجع checkout_main(argc, argv, اڳياڙي, &opts, options,
			     CHECKOUT_RESTORE);
}
