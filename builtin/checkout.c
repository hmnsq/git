#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DISABLE_SIGN_COMPARE_WARNINGS

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "advice.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "branch.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cache-tree.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "checkout.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "commit.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "diff.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "dir.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hex.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hook.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "merge-ll.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "lockfile.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "mem-pool.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-file.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-name.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "odb.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "path.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "preload-index.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "read-cache.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "refs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "remote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "repo-settings.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "resolve-undo.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "revision.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "sequencer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "setup.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "sparse-index.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "strvec.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "submodule.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "symlinks.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "trace2.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "tree.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "tree-walk.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "unpack-trees.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "wt-status.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xdiff-interface.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "entry.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parallel-checkout.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "add-interactive.h"

ਬਣਤਰ checkout_opts {
	ਪੂਰਨ patch_ਬਹੁਲਕ;
	ਪੂਰਨ patch_context;
	ਪੂਰਨ patch_interhunk_context;
	ਪੂਰਨ auto_advance;
	ਪੂਰਨ quiet;
	ਪੂਰਨ ਮਿਲਾਓ;
	ਪੂਰਨ force;
	ਪੂਰਨ force_detach;
	ਪੂਰਨ implicit_detach;
	ਪੂਰਨ writeout_stage;
	ਪੂਰਨ overwrite_ignore;
	ਪੂਰਨ ignore_skipworktree;
	ਪੂਰਨ ignore_other_worktrees;
	ਪੂਰਨ show_ਤਰੱਕੀ;
	ਪੂਰਨ ਗਿਣਤੀ_checkout_paths;
	ਪੂਰਨ overlay_ਬਹੁਲਕ;
	ਪੂਰਨ dwim_new_local_branch;
	ਪੂਰਨ discard_changes;
	ਪੂਰਨ accept_ref;
	ਪੂਰਨ accept_pathspec;
	ਪੂਰਨ switch_branch_doing_nothing_is_ok;
	ਪੂਰਨ only_ਮਿਲਾਓ_on_switching_branches;
	ਪੂਰਨ can_switch_when_ਵਿੱਚ_ਤਰੱਕੀ;
	ਪੂਰਨ orphan_from_ਖਾਲੀ_ਥਾਵਾਂ_ਰੁੱਖ;
	ਪੂਰਨ ਖਾਲੀ_ਥਾਵਾਂ_pathspec_ok;
	ਪੂਰਨ checkout_ਸੂਚਕਾਂਕ;
	ਪੂਰਨ checkout_worktree;
	ਅਚਲ ਅੱਖਰ *ignore_unmerged_opt;
	ਪੂਰਨ ignore_unmerged;
	ਪੂਰਨ pathspec_file_nul;
	ਅੱਖਰ *pathspec_from_file;

	ਅਚਲ ਅੱਖਰ *new_branch;
	ਅਚਲ ਅੱਖਰ *new_branch_force;
	ਅਚਲ ਅੱਖਰ *new_orphan_branch;
	ਪੂਰਨ new_branch_log;
	ਗਿਣਤੀ branch_track track;
	ਬਣਤਰ diff_options diff_options;
	ਪੂਰਨ conflict_style;

	ਪੂਰਨ branch_exists;
	ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ;
	ਬਣਤਰ pathspec pathspec;
	ਅਚਲ ਅੱਖਰ *from_treeish;
	ਬਣਤਰ ਰੁੱਖ *ਸਰੋਤ_ਰੁੱਖ;
};

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CHECKOUT_OPTS_INIT { \
	.conflict_style = -1, \
	.ਮਿਲਾਓ = -1, \
	.patch_context = -1, \
	.patch_interhunk_context = -1, \
	.auto_advance = 1, \
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਮਿਲਾਓ_WORKING_ਰੁੱਖ_UNPACK_FAILED (-2)

ਬਣਤਰ branch_info {
	ਅੱਖਰ *name; /* The short name used */
	ਅੱਖਰ *ਰਸਤਾ; /* The full name of a real branch */
	ਬਣਤਰ commit *commit; /* The named commit */
	ਅੱਖਰ *refname; /* The full name of the ref being checked out. */
	ਬਣਤਰ object_id oid; /* The object ID of the commit being checked out. */
	/*
	 * if not null the branch is detached because it's already
	 * checked out in this checkout
	 */
	ਅੱਖਰ *checkout;
};

ਸਥਿਰ ਖਾਲੀ branch_info_release(ਬਣਤਰ branch_info *info)
{
	ਮੁਕਤ(info->name);
	ਮੁਕਤ(info->ਰਸਤਾ);
	ਮੁਕਤ(info->refname);
	ਮੁਕਤ(info->checkout);
}

ਸਥਿਰ ਪੂਰਨ post_checkout_hook(ਬਣਤਰ commit *old_commit, ਬਣਤਰ commit *new_commit,
			      ਪੂਰਨ ਬਦਲਿਆ)
{
	ਬਣਤਰ ਲੜੀਵਾਰ_hooks_opt opt = ਲੜੀਵਾਰ_HOOKS_OPT_INIT_FORCE_SERIAL;

	/*
	 * "new_commit" can be NULL when checking out from the index before
	 * a commit exists.
	 */
	strvec_pushl(&opt.ਦਲੀਲਾਂ,
		     oid_ਮੰਜ਼ਿਲ_hex(old_commit ? &old_commit->object.oid : null_oid(the_hash_algo)),
		     oid_ਮੰਜ਼ਿਲ_hex(new_commit ? &new_commit->object.oid : null_oid(the_hash_algo)),
		     ਬਦਲਿਆ ? "1" : "0",
		     ਨਿਰਾ);

	ਵਾਪਸ ਲੜੀਵਾਰ_hooks_opt(the_repository, "post-checkout", &opt);
}

/*
 * Handle a tree object and determine if we need to recurse into the
 * tree (READ_TREE_RECURSIVE) or skip it (0).
 */
ਸਥਿਰ ਪੂਰਨ try_update_sparse_directory(ਅਚਲ ਬਣਤਰ object_id *oid,
				       ਬਣਤਰ strbuf *ਅਧਾਰ,
				       ਅਚਲ ਅੱਖਰ *pathname,
				       ਪੂਰਨ overlay_ਬਹੁਲਕ)
{
	ਬਣਤਰ strbuf dirpath = STRBUF_INIT;
	ਬਣਤਰ cache_ਇੰਦਰਾਜ *old;
	ਪੂਰਨ pos, ਨਤੀਜਾ = READ_ਰੁੱਖ_RECURSIVE;

	ਜੇ (!the_repository->ਸੂਚਕਾਂਕ->sparse_ਸੂਚਕਾਂਕ)
		ਵਾਪਸ ਨਤੀਜਾ;

	strbuf_addbuf(&dirpath, ਅਧਾਰ);
	strbuf_addstr(&dirpath, pathname);
	strbuf_addch(&dirpath, '/');

	pos = ਸੂਚਕਾਂਕ_name_pos_sparse(the_repository->ਸੂਚਕਾਂਕ,
				    dirpath.buf, dirpath.len);
	ਜੇ (pos < 0)
		ਜਾਓ cleanup;

	old = the_repository->ਸੂਚਕਾਂਕ->cache[pos];
	ਜੇ (!S_ISSPARSEDIR(old->ce_ਬਹੁਲਕ))
		ਜਾਓ cleanup;

	ਜੇ (oideq(oid, &old->oid)) {
		/* Tree content already matches; no need to descend. */
		ਨਤੀਜਾ = 0;
	} ਨਹੀਂਤਾਂ ਜੇ (!overlay_ਬਹੁਲਕ) {
		/*
		 * In non-overlay mode (e.g., restore --staged), replace the
		 * sparse directory OID directly since files not present in
		 * the source tree should be removed anyway.
		 */
		oidcpy(&old->oid, oid);
		old->ce_flags |= CE_UPDATE;
		ਨਤੀਜਾ = 0;
	}

cleanup:
	strbuf_release(&dirpath);
	ਵਾਪਸ ਨਤੀਜਾ;
}

ਸਥਿਰ ਪੂਰਨ update_some(ਅਚਲ ਬਣਤਰ object_id *oid, ਬਣਤਰ strbuf *ਅਧਾਰ,
		       ਅਚਲ ਅੱਖਰ *pathname, ਬਿਨਾਨਿਸ਼ਾਨੀ ਬਹੁਲਕ, ਖਾਲੀ *context)
{
	ਪੂਰਨ len;
	ਬਣਤਰ cache_ਇੰਦਰਾਜ *ce;
	ਪੂਰਨ pos;
	ਪੂਰਨ overlay_ਬਹੁਲਕ = context ? *((ਪੂਰਨ *)context) : 1;

	ਜੇ (S_ISDIR(ਬਹੁਲਕ))
		ਵਾਪਸ try_update_sparse_directory(oid, ਅਧਾਰ, pathname,
						   overlay_ਬਹੁਲਕ);

	len = ਅਧਾਰ->len + ਲੜੀ_ਲੰਬਾਈ(pathname);
	ce = make_ਖਾਲੀ_ਥਾਵਾਂ_cache_ਇੰਦਰਾਜ(the_repository->ਸੂਚਕਾਂਕ, len);
	oidcpy(&ce->oid, oid);
	ਸਮ੍ਰਿਤੀ_ਨਕਲ(ce->name, ਅਧਾਰ->buf, ਅਧਾਰ->len);
	ਸਮ੍ਰਿਤੀ_ਨਕਲ(ce->name + ਅਧਾਰ->len, pathname, len - ਅਧਾਰ->len);
	ce->ce_flags = create_ce_flags(0) | CE_UPDATE;
	ce->ce_namelen = len;
	ce->ce_ਬਹੁਲਕ = create_ce_ਬਹੁਲਕ(ਬਹੁਲਕ);

	/*
	 * If the entry is the same as the current index, we can leave the old
	 * entry in place. Whether it is UPTODATE or not, checkout_entry will
	 * do the right thing.
	 */
	pos = ਸੂਚਕਾਂਕ_name_pos_sparse(the_repository->ਸੂਚਕਾਂਕ, ce->name, ce->ce_namelen);
	ਜੇ (pos >= 0) {
		ਬਣਤਰ cache_ਇੰਦਰਾਜ *old = the_repository->ਸੂਚਕਾਂਕ->cache[pos];
		ਜੇ (ce->ce_ਬਹੁਲਕ == old->ce_ਬਹੁਲਕ &&
		    !ce_intent_ਮੰਜ਼ਿਲ_ਜੋੜੋ(old) &&
		    oideq(&ce->oid, &old->oid)) {
			old->ce_flags |= CE_UPDATE;
			discard_cache_ਇੰਦਰਾਜ(ce);
			ਵਾਪਸ 0;
		}
	}

	ਜੋੜੋ_ਸੂਚਕਾਂਕ_ਇੰਦਰਾਜ(the_repository->ਸੂਚਕਾਂਕ, ce,
			ਜੋੜੋ_CACHE_OK_ਮੰਜ਼ਿਲ_ਜੋੜੋ | ਜੋੜੋ_CACHE_OK_ਮੰਜ਼ਿਲ_REPLACE);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ read_ਰੁੱਖ_some(ਬਣਤਰ ਰੁੱਖ *ਰੁੱਖ, ਅਚਲ ਬਣਤਰ pathspec *pathspec,
			  ਪੂਰਨ overlay_ਬਹੁਲਕ)
{
	read_ਰੁੱਖ(the_repository, ਰੁੱਖ,
		  pathspec, update_some, &overlay_ਬਹੁਲਕ);

	/* update the index with the given tree's info
	 * for all args, expanding wildcards, and exit
	 * with any non-zero return code.
	 */
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ skip_same_name(ਅਚਲ ਬਣਤਰ cache_ਇੰਦਰਾਜ *ce, ਪੂਰਨ pos)
{
	ਜਦੋਂ (++pos < the_repository->ਸੂਚਕਾਂਕ->cache_nr &&
	       !strcmp(the_repository->ਸੂਚਕਾਂਕ->cache[pos]->name, ce->name))
		; /* skip */
	ਵਾਪਸ pos;
}

ਸਥਿਰ ਪੂਰਨ check_stage(ਪੂਰਨ stage, ਅਚਲ ਬਣਤਰ cache_ਇੰਦਰਾਜ *ce, ਪੂਰਨ pos,
		       ਪੂਰਨ overlay_ਬਹੁਲਕ)
{
	ਜਦੋਂ (pos < the_repository->ਸੂਚਕਾਂਕ->cache_nr &&
	       !strcmp(the_repository->ਸੂਚਕਾਂਕ->cache[pos]->name, ce->name)) {
		ਜੇ (ce_stage(the_repository->ਸੂਚਕਾਂਕ->cache[pos]) == stage)
			ਵਾਪਸ 0;
		pos++;
	}
	ਜੇ (!overlay_ਬਹੁਲਕ)
		ਵਾਪਸ 0;
	ਜੇ (stage == 2)
		ਵਾਪਸ error(_("path '%s' does not have our version"), ce->name);
	ਨਹੀਂਤਾਂ
		ਵਾਪਸ error(_("path '%s' does not have their version"), ce->name);
}

ਸਥਿਰ ਪੂਰਨ check_stages(ਬਿਨਾਨਿਸ਼ਾਨੀ stages, ਅਚਲ ਬਣਤਰ cache_ਇੰਦਰਾਜ *ce, ਪੂਰਨ pos)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਵੇਖਿਆ = 0;
	ਅਚਲ ਅੱਖਰ *name = ce->name;

	ਜਦੋਂ (pos < the_repository->ਸੂਚਕਾਂਕ->cache_nr) {
		ce = the_repository->ਸੂਚਕਾਂਕ->cache[pos];
		ਜੇ (strcmp(name, ce->name))
			ਰੋਕੋ;
		ਵੇਖਿਆ |= (1 << ce_stage(ce));
		pos++;
	}
	ਜੇ ((stages & ਵੇਖਿਆ) != stages)
		ਵਾਪਸ error(_("path '%s' does not have all necessary versions"),
			     name);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ checkout_stage(ਪੂਰਨ stage, ਅਚਲ ਬਣਤਰ cache_ਇੰਦਰਾਜ *ce, ਪੂਰਨ pos,
			  ਅਚਲ ਬਣਤਰ checkout *state, ਪੂਰਨ *nr_checkouts,
			  ਪੂਰਨ overlay_ਬਹੁਲਕ)
{
	ਜਦੋਂ (pos < the_repository->ਸੂਚਕਾਂਕ->cache_nr &&
	       !strcmp(the_repository->ਸੂਚਕਾਂਕ->cache[pos]->name, ce->name)) {
		ਜੇ (ce_stage(the_repository->ਸੂਚਕਾਂਕ->cache[pos]) == stage)
			ਵਾਪਸ checkout_ਇੰਦਰਾਜ(the_repository->ਸੂਚਕਾਂਕ->cache[pos], state,
					      ਨਿਰਾ, nr_checkouts);
		pos++;
	}
	ਜੇ (!overlay_ਬਹੁਲਕ) {
		unlink_ਇੰਦਰਾਜ(ce, ਨਿਰਾ);
		ਵਾਪਸ 0;
	}
	ਜੇ (stage == 2)
		ਵਾਪਸ error(_("path '%s' does not have our version"), ce->name);
	ਨਹੀਂਤਾਂ
		ਵਾਪਸ error(_("path '%s' does not have their version"), ce->name);
}

ਸਥਿਰ ਪੂਰਨ checkout_merged(ਪੂਰਨ pos, ਅਚਲ ਬਣਤਰ checkout *state,
			   ਪੂਰਨ *nr_checkouts, ਬਣਤਰ mem_pool *ce_mem_pool,
			   ਪੂਰਨ conflict_style)
{
	ਬਣਤਰ cache_ਇੰਦਰਾਜ *ce = the_repository->ਸੂਚਕਾਂਕ->cache[pos];
	ਅਚਲ ਅੱਖਰ *ਰਸਤਾ = ce->name;
	mmfile_t ancestor, ours, theirs;
	ਗਿਣਤੀ ll_ਮਿਲਾਓ_ਨਤੀਜਾ ਮਿਲਾਓ_ਸਥਿਤੀ;
	ਪੂਰਨ ਸਥਿਤੀ;
	ਬਣਤਰ object_id oid;
	mmfile_t ਨਤੀਜਾ_buf;
	ਬਣਤਰ object_id threeway[3];
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਬਹੁਲਕ = 0;
	ਬਣਤਰ ll_ਮਿਲਾਓ_options ll_opts = LL_ਮਿਲਾਓ_OPTIONS_INIT;
	ਪੂਰਨ renormalize = 0;

	memset(threeway, 0, ਆਕਾਰ(threeway));
	ਜਦੋਂ (pos < the_repository->ਸੂਚਕਾਂਕ->cache_nr) {
		ਪੂਰਨ stage;
		stage = ce_stage(ce);
		ਜੇ (!stage || strcmp(ਰਸਤਾ, ce->name))
			ਰੋਕੋ;
		oidcpy(&threeway[stage - 1], &ce->oid);
		ਜੇ (stage == 2)
			ਬਹੁਲਕ = create_ce_ਬਹੁਲਕ(ce->ce_ਬਹੁਲਕ);
		pos++;
		ce = the_repository->ਸੂਚਕਾਂਕ->cache[pos];
	}
	ਜੇ (is_null_oid(&threeway[1]) || is_null_oid(&threeway[2]))
		ਵਾਪਸ error(_("path '%s' does not have necessary versions"), ਰਸਤਾ);

	read_mmblob(&ancestor, the_repository->objects, &threeway[0]);
	read_mmblob(&ours, the_repository->objects, &threeway[1]);
	read_mmblob(&theirs, the_repository->objects, &threeway[2]);

	repo_config_get_bool(the_repository, "merge.renormalize", &renormalize);
	ll_opts.renormalize = renormalize;
	ll_opts.conflict_style = conflict_style;
	ਮਿਲਾਓ_ਸਥਿਤੀ = ll_ਮਿਲਾਓ(&ਨਤੀਜਾ_buf, ਰਸਤਾ, &ancestor, "base",
				&ours, "ours", &theirs, "theirs",
				state->istate, &ll_opts);
	ਮੁਕਤ(ancestor.ptr);
	ਮੁਕਤ(ours.ptr);
	ਮੁਕਤ(theirs.ptr);
	ਜੇ (ਮਿਲਾਓ_ਸਥਿਤੀ == LL_ਮਿਲਾਓ_ਦੁਆਧਾਰੀ_CONFLICT)
		warning("Cannot merge binary files: %s (%s vs. %s)",
			ਰਸਤਾ, "ours", "theirs");
	ਜੇ (ਮਿਲਾਓ_ਸਥਿਤੀ < 0 || !ਨਤੀਜਾ_buf.ptr) {
		ਮੁਕਤ(ਨਤੀਜਾ_buf.ptr);
		ਵਾਪਸ error(_("path '%s': cannot merge"), ਰਸਤਾ);
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
	ਜੇ (odb_write_object(the_repository->objects, ਨਤੀਜਾ_buf.ptr, ਨਤੀਜਾ_buf.ਮਾਪ, OBJ_BLOB, &oid))
		die(_("Unable to add merge result for '%s'"), ਰਸਤਾ);
	ਮੁਕਤ(ਨਤੀਜਾ_buf.ptr);
	ce = make_transient_cache_ਇੰਦਰਾਜ(ਬਹੁਲਕ, &oid, ਰਸਤਾ, 2, ce_mem_pool);
	ਜੇ (!ce)
		die(_("make_cache_entry failed for path '%s'"), ਰਸਤਾ);
	ਸਥਿਤੀ = checkout_ਇੰਦਰਾਜ(ce, state, ਨਿਰਾ, nr_checkouts);
	ਵਾਪਸ ਸਥਿਤੀ;
}

ਸਥਿਰ ਖਾਲੀ mark_ce_for_checkout_overlay(ਬਣਤਰ cache_ਇੰਦਰਾਜ *ce,
					 ਅੱਖਰ *ps_matched,
					 ਅਚਲ ਬਣਤਰ checkout_opts *opts)
{
	ce->ce_flags &= ~CE_MATCHED;
	ਜੇ (!opts->ignore_skipworktree && ce_skip_worktree(ce))
		ਵਾਪਸ;
	ਜੇ (opts->ਸਰੋਤ_ਰੁੱਖ && !(ce->ce_flags & CE_UPDATE))
		/*
		 * "git checkout tree-ish -- path", but this entry
		 * is in the original index but is not in tree-ish
		 * or does not match the pathspec; it will not be
		 * checked out to the working tree.  We will not do
		 * anything to this entry at all.
		 */
		ਵਾਪਸ;
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
	ਜੇ (ce_ਰਸਤਾ_ਮੇਲ(the_repository->ਸੂਚਕਾਂਕ, ce, &opts->pathspec, ps_matched))
		ce->ce_flags |= CE_MATCHED;
}

ਸਥਿਰ ਖਾਲੀ mark_ce_for_checkout_no_overlay(ਬਣਤਰ cache_ਇੰਦਰਾਜ *ce,
					    ਅੱਖਰ *ps_matched,
					    ਅਚਲ ਬਣਤਰ checkout_opts *opts)
{
	ce->ce_flags &= ~CE_MATCHED;
	ਜੇ (!opts->ignore_skipworktree && ce_skip_worktree(ce))
		ਵਾਪਸ;
	ਜੇ (ce_ਰਸਤਾ_ਮੇਲ(the_repository->ਸੂਚਕਾਂਕ, ce, &opts->pathspec, ps_matched)) {
		ce->ce_flags |= CE_MATCHED;
		ਜੇ (opts->ਸਰੋਤ_ਰੁੱਖ && !(ce->ce_flags & CE_UPDATE))
			/*
			 * In overlay mode, but the path is not in
			 * tree-ish, which means we should remove it
			 * from the index and the working tree.
			 */
			ce->ce_flags |= CE_REMOVE | CE_WT_REMOVE;
	}
}

ਸਥਿਰ ਪੂਰਨ checkout_worktree(ਅਚਲ ਬਣਤਰ checkout_opts *opts,
			     ਅਚਲ ਬਣਤਰ branch_info *info)
{
	ਬਣਤਰ checkout state = CHECKOUT_INIT;
	ਪੂਰਨ nr_checkouts = 0, nr_unmerged = 0;
	ਪੂਰਨ errs = 0;
	ਪੂਰਨ pos;
	ਪੂਰਨ pc_workers, pc_ਹੱਦ;
	ਬਣਤਰ mem_pool ce_mem_pool;

	state.force = 1;
	state.refresh_cache = 1;
	state.istate = the_repository->ਸੂਚਕਾਂਕ;

	mem_pool_init(&ce_mem_pool, 0);
	get_parallel_checkout_configs(&pc_workers, &pc_ਹੱਦ);
	init_checkout_metadata(&state.meta, info->refname,
			       info->commit ? &info->commit->object.oid : &info->oid,
			       ਨਿਰਾ);

	enable_delayed_checkout(&state);

	ਜੇ (pc_workers > 1)
		init_parallel_checkout();

	ਲਈ (pos = 0; pos < the_repository->ਸੂਚਕਾਂਕ->cache_nr; pos++) {
		ਬਣਤਰ cache_ਇੰਦਰਾਜ *ce = the_repository->ਸੂਚਕਾਂਕ->cache[pos];
		ਜੇ (ce->ce_flags & CE_MATCHED) {
			ਜੇ (!ce_stage(ce)) {
				errs |= checkout_ਇੰਦਰਾਜ(ce, &state,
						       ਨਿਰਾ, &nr_checkouts);
				ਜਾਰੀ;
			}
			ਜੇ (opts->writeout_stage)
				errs |= checkout_stage(opts->writeout_stage,
						       ce, pos,
						       &state,
						       &nr_checkouts, opts->overlay_ਬਹੁਲਕ);
			ਨਹੀਂਤਾਂ ਜੇ (opts->ਮਿਲਾਓ)
				errs |= checkout_merged(pos, &state,
							&nr_unmerged,
							&ce_mem_pool,
							opts->conflict_style);
			pos = skip_same_name(ce, pos) - 1;
		}
	}
	ਜੇ (pc_workers > 1)
		errs |= ਲੜੀਵਾਰ_parallel_checkout(&state, pc_workers, pc_ਹੱਦ,
					      ਨਿਰਾ, ਨਿਰਾ);
	mem_pool_discard(&ce_mem_pool, should_validate_cache_entries());
	remove_marked_cache_entries(the_repository->ਸੂਚਕਾਂਕ, 1);
	remove_scheduled_dirs();
	errs |= ਅੰਤ_delayed_checkout(&state, opts->show_ਤਰੱਕੀ);

	ਜੇ (opts->ਗਿਣਤੀ_checkout_paths) {
		ਜੇ (nr_unmerged)
			fprintf_ln(stderr, Q_("Recreated %d merge conflict",
					      "Recreated %d merge conflicts",
					      nr_unmerged),
				   nr_unmerged);
		ਜੇ (opts->ਸਰੋਤ_ਰੁੱਖ)
			fprintf_ln(stderr, Q_("Updated %d path from %s",
					      "Updated %d paths from %s",
					      nr_checkouts),
				   nr_checkouts,
				   repo_ਲੱਭੋ_unique_abbrev(the_repository, &opts->ਸਰੋਤ_ਰੁੱਖ->object.oid,
							   DEFAULT_ABBREV));
		ਨਹੀਂਤਾਂ ਜੇ (!nr_unmerged || nr_checkouts)
			fprintf_ln(stderr, Q_("Updated %d path from the index",
					      "Updated %d paths from the index",
					      nr_checkouts),
				   nr_checkouts);
	}

	ਵਾਪਸ errs;
}

ਸਥਿਰ ਪੂਰਨ checkout_paths(ਅਚਲ ਬਣਤਰ checkout_opts *opts,
			  ਅਚਲ ਬਣਤਰ branch_info *new_branch_info)
{
	ਪੂਰਨ pos;
	ਸਥਿਰ ਅੱਖਰ *ps_matched;
	ਬਣਤਰ object_id rev;
	ਬਣਤਰ commit *ਸਿਰ;
	ਪੂਰਨ errs = 0;
	ਬਣਤਰ ਤਾਲਾ_file ਤਾਲਾ_file = ਤਾਲਾ_INIT;
	ਪੂਰਨ checkout_ਸੂਚਕਾਂਕ;

	trace2_cmd_ਬਹੁਲਕ(opts->patch_ਬਹੁਲਕ ? "patch" : "path");

	ਜੇ (opts->track != BRANCH_TRACK_UNSPECIFIED)
		die(_("'%s' cannot be used with updating paths"), "--track");

	ਜੇ (opts->new_branch_log)
		die(_("'%s' cannot be used with updating paths"), "-l");

	ਜੇ (opts->ignore_unmerged && opts->patch_ਬਹੁਲਕ)
		die(_("'%s' cannot be used with updating paths"),
		    opts->ignore_unmerged_opt);

	ਜੇ (opts->force_detach)
		die(_("'%s' cannot be used with updating paths"), "--detach");

	ਜੇ (opts->ਮਿਲਾਓ && opts->patch_ਬਹੁਲਕ)
		die(_("options '%s' and '%s' cannot be used together"), "--merge", "--patch");

	ਜੇ (opts->ignore_unmerged && opts->ਮਿਲਾਓ)
		die(_("options '%s' and '%s' cannot be used together"),
		    opts->ignore_unmerged_opt, "-m");

	ਜੇ (opts->new_branch)
		die(_("Cannot update paths and switch to branch '%s' at the same time."),
		    opts->new_branch);

	ਜੇ (!opts->checkout_worktree && !opts->checkout_ਸੂਚਕਾਂਕ)
		die(_("neither '%s' or '%s' is specified"),
		    "--staged", "--worktree");

	ਜੇ (!opts->checkout_worktree && !opts->from_treeish)
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
	ਜੇ (!opts->accept_ref && opts->checkout_ਸੂਚਕਾਂਕ) {
		ਜੇ (opts->writeout_stage)
			die(_("'%s' or '%s' cannot be used with %s"),
			    "--ours", "--theirs", "--staged");

		ਜੇ (opts->ਮਿਲਾਓ)
			die(_("'%s' or '%s' cannot be used with %s"),
			    "--merge", "--conflict", "--staged");
	}

	/*
	 * recreating unmerged index entries and writing out data from
	 * unmerged index entries would make no sense when checking out
	 * of a tree-ish.
	 */
	ਜੇ ((opts->ਮਿਲਾਓ || opts->writeout_stage) && opts->ਸਰੋਤ_ਰੁੱਖ)
		die(_("'%s', '%s', or '%s' cannot be used when checking out of a tree"),
		    "--merge", "--ours", "--theirs");

	ਜੇ (opts->patch_ਬਹੁਲਕ) {
		ਗਿਣਤੀ ਜੋੜੋ_p_ਬਹੁਲਕ patch_ਬਹੁਲਕ;
		ਬਣਤਰ interactive_options interactive_opts = {
			.context = opts->patch_context,
			.interhunkcontext = opts->patch_interhunk_context,
			.auto_advance = opts->auto_advance
		};
		ਅਚਲ ਅੱਖਰ *rev = new_branch_info->name;
		ਅੱਖਰ rev_oid[GIT_MAX_HEXSZ + 1];

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
		ਜੇ (rev && new_branch_info->commit && strcmp(rev, "HEAD"))
			rev = oid_ਮੰਜ਼ਿਲ_hex_r(rev_oid, &new_branch_info->commit->object.oid);

		ਜੇ (opts->checkout_ਸੂਚਕਾਂਕ && opts->checkout_worktree)
			patch_ਬਹੁਲਕ = ਜੋੜੋ_P_CHECKOUT;
		ਨਹੀਂਤਾਂ ਜੇ (opts->checkout_ਸੂਚਕਾਂਕ && !opts->checkout_worktree)
			patch_ਬਹੁਲਕ = ਜੋੜੋ_P_RESET;
		ਨਹੀਂਤਾਂ ਜੇ (!opts->checkout_ਸੂਚਕਾਂਕ && opts->checkout_worktree)
			patch_ਬਹੁਲਕ = ਜੋੜੋ_P_WORKTREE;
		ਨਹੀਂਤਾਂ
			BUG("either flag must have been set, worktree=%d, index=%d",
			    opts->checkout_worktree, opts->checkout_ਸੂਚਕਾਂਕ);
		ਵਾਪਸ !!ਲੜੀਵਾਰ_ਜੋੜੋ_p(the_repository, patch_ਬਹੁਲਕ, &interactive_opts,
				   rev, &opts->pathspec, 0);
	}

	repo_hold_locked_ਸੂਚਕਾਂਕ(the_repository, &ਤਾਲਾ_file, ਤਾਲਾ_DIE_ON_ERROR);
	ਜੇ (repo_read_ਸੂਚਕਾਂਕ_preload(the_repository, &opts->pathspec, 0) < 0)
		ਵਾਪਸ error(_("index file corrupt"));

	ਜੇ (opts->ਸਰੋਤ_ਰੁੱਖ)
		read_ਰੁੱਖ_some(opts->ਸਰੋਤ_ਰੁੱਖ, &opts->pathspec,
			       opts->overlay_ਬਹੁਲਕ);
	ਜੇ (opts->ਮਿਲਾਓ)
		unmerge_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ, &opts->pathspec, CE_MATCHED);

	ps_matched = xcalloc(opts->pathspec.nr, 1);

	/*
	 * Make sure all pathspecs participated in locating the paths
	 * to be checked out.
	 */
	ਲਈ (pos = 0; pos < the_repository->ਸੂਚਕਾਂਕ->cache_nr; pos++)
		ਜੇ (opts->overlay_ਬਹੁਲਕ)
			mark_ce_for_checkout_overlay(the_repository->ਸੂਚਕਾਂਕ->cache[pos],
						     ps_matched,
						     opts);
		ਨਹੀਂਤਾਂ
			mark_ce_for_checkout_no_overlay(the_repository->ਸੂਚਕਾਂਕ->cache[pos],
							ps_matched,
							opts);

	ਜੇ (report_ਰਸਤਾ_error(ps_matched, &opts->pathspec)) {
		ਮੁਕਤ(ps_matched);
		ਵਾਪਸ 1;
	}
	ਮੁਕਤ(ps_matched);

	/* Any unmerged paths? */
	ਲਈ (pos = 0; pos < the_repository->ਸੂਚਕਾਂਕ->cache_nr; pos++) {
		ਅਚਲ ਬਣਤਰ cache_ਇੰਦਰਾਜ *ce = the_repository->ਸੂਚਕਾਂਕ->cache[pos];
		ਜੇ (ce->ce_flags & CE_MATCHED) {
			ਜੇ (!ce_stage(ce))
				ਜਾਰੀ;
			ਜੇ (opts->ignore_unmerged) {
				ਜੇ (!opts->quiet)
					warning(_("path '%s' is unmerged"), ce->name);
			} ਨਹੀਂਤਾਂ ਜੇ (opts->writeout_stage) {
				errs |= check_stage(opts->writeout_stage, ce, pos, opts->overlay_ਬਹੁਲਕ);
			} ਨਹੀਂਤਾਂ ਜੇ (opts->ਮਿਲਾਓ) {
				errs |= check_stages((1<<2) | (1<<3), ce, pos);
			} ਨਹੀਂਤਾਂ {
				errs = 1;
				error(_("path '%s' is unmerged"), ce->name);
			}
			pos = skip_same_name(ce, pos) - 1;
		}
	}
	ਜੇ (errs)
		ਵਾਪਸ 1;

	/* Now we are committed to check them out */
	ਜੇ (opts->checkout_worktree)
		errs |= checkout_worktree(opts, new_branch_info);
	ਨਹੀਂਤਾਂ
		remove_marked_cache_entries(the_repository->ਸੂਚਕਾਂਕ, 1);

	/*
	 * Allow updating the index when checking out from the index.
	 * This is to save new stat info.
	 */
	ਜੇ (opts->checkout_worktree && !opts->checkout_ਸੂਚਕਾਂਕ && !opts->ਸਰੋਤ_ਰੁੱਖ)
		checkout_ਸੂਚਕਾਂਕ = 1;
	ਨਹੀਂਤਾਂ
		checkout_ਸੂਚਕਾਂਕ = opts->checkout_ਸੂਚਕਾਂਕ;

	ਜੇ (checkout_ਸੂਚਕਾਂਕ) {
		ਜੇ (write_locked_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ, &ਤਾਲਾ_file, COMMIT_ਤਾਲਾ))
			die(_("unable to write new index file"));
	} ਨਹੀਂਤਾਂ {
		/*
		 * NEEDSWORK: if --worktree is not specified, we
		 * should save stat info of checked out files in the
		 * index to avoid the next (potentially costly)
		 * refresh. But it's a bit tricker to do...
		 */
		rollback_ਤਾਲਾ_file(&ਤਾਲਾ_file);
	}

	refs_read_ref_ਭਰਿਆ(get_main_ref_store(the_repository), "HEAD", 0,
			   &rev, ਨਿਰਾ);
	ਸਿਰ = lookup_commit_reference_gently(the_repository, &rev, 1);

	errs |= post_checkout_hook(ਸਿਰ, ਸਿਰ, 0);
	ਵਾਪਸ errs;
}

ਸਥਿਰ ਖਾਲੀ show_local_changes(ਬਣਤਰ object *ਸਿਰ,
			       ਅਚਲ ਬਣਤਰ diff_options *opts)
{
	ਬਣਤਰ rev_info rev;
	/* I think we want full paths, even if we're in a subdirectory. */
	repo_init_revisions(the_repository, &rev, ਨਿਰਾ);
	rev.diffopt.flags = opts->flags;
	rev.diffopt.output_format |= DIFF_FORMAT_NAME_ਸਥਿਤੀ;
	rev.diffopt.flags.recursive = 1;
	diff_ਤਿਆਰੀ_ਪੂਰਾ(&rev.diffopt);
	ਜੋੜੋ_pending_object(&rev, ਸਿਰ, ਨਿਰਾ);
	ਲੜੀਵਾਰ_diff_ਸੂਚਕਾਂਕ(&rev, 0);
	release_revisions(&rev);
}

ਸਥਿਰ ਖਾਲੀ describe_detached_ਸਿਰ(ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ, ਬਣਤਰ commit *commit)
{
	ਬਣਤਰ strbuf sb = STRBUF_INIT;

	ਜੇ (!repo_parse_commit(the_repository, commit))
		pp_commit_easy(CMIT_FMT_ONELINE, commit, &sb);
	ਜੇ (print_sha1_ellipsis()) {
		fprintf(stderr, "%s %s... %s\n", ਸੁਨੇਹਾ,
			repo_ਲੱਭੋ_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV),
			sb.buf);
	} ਨਹੀਂਤਾਂ {
		fprintf(stderr, "%s %s %s\n", ਸੁਨੇਹਾ,
			repo_ਲੱਭੋ_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV),
			sb.buf);
	}
	strbuf_release(&sb);
}

ਸਥਿਰ ਪੂਰਨ reset_ਰੁੱਖ(ਬਣਤਰ ਰੁੱਖ *ਰੁੱਖ, ਅਚਲ ਬਣਤਰ checkout_opts *o,
		      ਪੂਰਨ worktree, ਪੂਰਨ *writeout_error,
		      ਬਣਤਰ branch_info *info)
{
	ਬਣਤਰ unpack_trees_options opts;
	ਬਣਤਰ ਰੁੱਖ_desc ਰੁੱਖ_desc;

	memset(&opts, 0, ਆਕਾਰ(opts));
	opts.ਸਿਰ_idx = -1;
	opts.update = worktree;
	opts.skip_unmerged = !worktree;
	opts.reset = o->force ? UNPACK_RESET_OVERWRITE_UNTRACKED :
				UNPACK_RESET_PROTECT_UNTRACKED;
	opts.preserve_ignored = (!o->force && !o->overwrite_ignore);
	opts.ਮਿਲਾਓ = 1;
	opts.fn = oneway_ਮਿਲਾਓ;
	opts.verbose_update = o->show_ਤਰੱਕੀ;
	opts.src_ਸੂਚਕਾਂਕ = the_repository->ਸੂਚਕਾਂਕ;
	opts.dst_ਸੂਚਕਾਂਕ = the_repository->ਸੂਚਕਾਂਕ;
	init_checkout_metadata(&opts.meta, info->refname,
			       info->commit ? &info->commit->object.oid : null_oid(the_hash_algo),
			       ਨਿਰਾ);
	ਜੇ (repo_parse_ਰੁੱਖ(the_repository, ਰੁੱਖ) < 0)
		ਵਾਪਸ 128;
	init_ਰੁੱਖ_desc(&ਰੁੱਖ_desc, &ਰੁੱਖ->object.oid, ਰੁੱਖ->ਬਫਰ, ਰੁੱਖ->ਮਾਪ);
	ਚੋਣ (unpack_trees(1, &ਰੁੱਖ_desc, &opts)) {
	ਹਾਲਤ -2:
		*writeout_error = 1;
		/*
		 * We return 0 nevertheless, as the index is all right
		 * and more importantly we have made best efforts to
		 * update paths in the work tree, and we cannot revert
		 * them.
		 */
		/* fallthrough */
	ਹਾਲਤ 0:
		ਵਾਪਸ 0;
	ਮੂਲ:
		ਵਾਪਸ 128;
	}
}

ਸਥਿਰ ਖਾਲੀ ਤਿਆਰੀ_branch_ਰਸਤਾ(ਬਣਤਰ branch_info *branch)
{
	ਬਣਤਰ strbuf buf = STRBUF_INIT;

	/*
	 * If this is a ref, resolve it; otherwise, look up the OID for our
	 * expression.  Failure here is okay.
	 */
	ਜੇ (!repo_dwim_ref(the_repository, branch->name, ਲੜੀ_ਲੰਬਾਈ(branch->name),
			   &branch->oid, &branch->refname, 0))
		repo_get_oid_committish(the_repository, branch->name, &branch->oid);

	copy_branchname(the_repository, &buf, branch->name, INTERPRET_BRANCH_LOCAL);
	ਜੇ (strcmp(buf.buf, branch->name)) {
		ਮੁਕਤ(branch->name);
		branch->name = xstrdup(buf.buf);
	}
	strbuf_splice(&buf, 0, 0, "refs/heads/", 11);
	ਮੁਕਤ(branch->ਰਸਤਾ);
	branch->ਰਸਤਾ = strbuf_detach(&buf, ਨਿਰਾ);
}

ਸਥਿਰ ਖਾਲੀ init_topts(ਬਣਤਰ unpack_trees_options *topts,
		       ਪੂਰਨ show_ਤਰੱਕੀ, ਪੂਰਨ overwrite_ignore,
		       bool quiet)
{
	memset(topts, 0, ਆਕਾਰ(*topts));
	topts->ਸਿਰ_idx = -1;
	topts->src_ਸੂਚਕਾਂਕ = the_repository->ਸੂਚਕਾਂਕ;
	topts->dst_ਸੂਚਕਾਂਕ = the_repository->ਸੂਚਕਾਂਕ;

	ਤਿਆਰੀ_unpack_trees_porcelain(topts, "checkout");

	topts->initial_checkout = is_ਸੂਚਕਾਂਕ_unborn(the_repository->ਸੂਚਕਾਂਕ);
	topts->update = 1;
	topts->ਮਿਲਾਓ = 1;
	topts->quiet = quiet;
	topts->verbose_update = show_ਤਰੱਕੀ;
	topts->fn = twoway_ਮਿਲਾਓ;
	topts->preserve_ignored = !overwrite_ignore;
}

ਸਥਿਰ ਪੂਰਨ ਮਿਲਾਓ_working_ਰੁੱਖ(ਅਚਲ ਬਣਤਰ checkout_opts *opts,
			      ਬਣਤਰ branch_info *old_branch_info,
			      ਬਣਤਰ branch_info *new_branch_info,
			      bool quiet,
			      ਪੂਰਨ *writeout_error)
{
	ਪੂਰਨ ret;
	ਬਣਤਰ ਤਾਲਾ_file ਤਾਲਾ_file = ਤਾਲਾ_INIT;
	ਬਣਤਰ ਰੁੱਖ *new_ਰੁੱਖ;

	repo_hold_locked_ਸੂਚਕਾਂਕ(the_repository, &ਤਾਲਾ_file, ਤਾਲਾ_DIE_ON_ERROR);
	ਜੇ (repo_read_ਸੂਚਕਾਂਕ_preload(the_repository, ਨਿਰਾ, 0) < 0) {
		rollback_ਤਾਲਾ_file(&ਤਾਲਾ_file);
		ਵਾਪਸ error(_("index file corrupt"));
	}

	resolve_undo_clear_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ);
	ਜੇ (opts->new_orphan_branch && opts->orphan_from_ਖਾਲੀ_ਥਾਵਾਂ_ਰੁੱਖ) {
		ਜੇ (new_branch_info->commit)
			BUG("'switch --orphan' should never accept a commit as starting point");
		new_ਰੁੱਖ = repo_parse_ਰੁੱਖ_ਅਸਿੱਧਾ(the_repository,
						    the_hash_algo->ਖਾਲੀ_ਥਾਵਾਂ_ਰੁੱਖ);
		ਜੇ (!new_ਰੁੱਖ)
			BUG("unable to read empty tree");
	} ਨਹੀਂਤਾਂ {
		new_ਰੁੱਖ = repo_get_commit_ਰੁੱਖ(the_repository,
						new_branch_info->commit);
		ਜੇ (!new_ਰੁੱਖ) {
			rollback_ਤਾਲਾ_file(&ਤਾਲਾ_file);
			ਵਾਪਸ error(_("unable to read tree (%s)"),
				     oid_ਮੰਜ਼ਿਲ_hex(&new_branch_info->commit->object.oid));
		}
	}
	ਜੇ (opts->discard_changes) {
		ret = reset_ਰੁੱਖ(new_ਰੁੱਖ, opts, 1, writeout_error, new_branch_info);
		ਜੇ (ret) {
			rollback_ਤਾਲਾ_file(&ਤਾਲਾ_file);
			ਵਾਪਸ ret;
		}
	} ਨਹੀਂਤਾਂ {
		ਬਣਤਰ ਰੁੱਖ_desc trees[2];
		ਬਣਤਰ ਰੁੱਖ *ਰੁੱਖ;
		ਬਣਤਰ unpack_trees_options topts;
		ਅਚਲ ਬਣਤਰ object_id *old_commit_oid;

		refresh_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ, REFRESH_QUIET, ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ);

		ਜੇ (unmerged_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ)) {
			rollback_ਤਾਲਾ_file(&ਤਾਲਾ_file);
			error(_("you need to resolve your current index first"));
			ਵਾਪਸ 1;
		}

		/* 2-way merge to the new branch */
		init_topts(&topts, opts->show_ਤਰੱਕੀ,
			   opts->overwrite_ignore, quiet);
		init_checkout_metadata(&topts.meta, new_branch_info->refname,
				       new_branch_info->commit ?
				       &new_branch_info->commit->object.oid :
				       &new_branch_info->oid, ਨਿਰਾ);

		old_commit_oid = old_branch_info->commit ?
			&old_branch_info->commit->object.oid :
			the_hash_algo->ਖਾਲੀ_ਥਾਵਾਂ_ਰੁੱਖ;
		ਰੁੱਖ = repo_parse_ਰੁੱਖ_ਅਸਿੱਧਾ(the_repository,
						old_commit_oid);
		ਜੇ (!ਰੁੱਖ)
			die(_("unable to parse commit %s"),
				oid_ਮੰਜ਼ਿਲ_hex(old_commit_oid));

		init_ਰੁੱਖ_desc(&trees[0], &ਰੁੱਖ->object.oid,
			       ਰੁੱਖ->ਬਫਰ, ਰੁੱਖ->ਮਾਪ);
		ਜੇ (repo_parse_ਰੁੱਖ(the_repository, new_ਰੁੱਖ) < 0)
			die(ਨਿਰਾ);
		ਰੁੱਖ = new_ਰੁੱਖ;
		init_ਰੁੱਖ_desc(&trees[1], &ਰੁੱਖ->object.oid,
			       ਰੁੱਖ->ਬਫਰ, ਰੁੱਖ->ਮਾਪ);

		ret = unpack_trees(2, trees, &topts);
		clear_unpack_trees_porcelain(&topts);
		ਜੇ (ret == -1) {
			rollback_ਤਾਲਾ_file(&ਤਾਲਾ_file);
			ਵਾਪਸ ਮਿਲਾਓ_WORKING_ਰੁੱਖ_UNPACK_FAILED;
		}
	}

	ਜੇ (!cache_ਰੁੱਖ_fully_valid(the_repository->ਸੂਚਕਾਂਕ))
		cache_ਰੁੱਖ_update(the_repository->ਸੂਚਕਾਂਕ, WRITE_ਰੁੱਖ_SILENT | WRITE_ਰੁੱਖ_REPAIR);

	ਜੇ (write_locked_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ, &ਤਾਲਾ_file, COMMIT_ਤਾਲਾ))
		die(_("unable to write new index file"));

	ਜੇ (!opts->discard_changes && !opts->quiet && new_branch_info->commit)
		show_local_changes(&new_branch_info->commit->object, &opts->diff_options);

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ report_tracking(ਬਣਤਰ branch_info *new_branch_info)
{
	ਬਣਤਰ strbuf sb = STRBUF_INIT;
	ਬਣਤਰ branch *branch = branch_get(new_branch_info->name);

	ਜੇ (!format_tracking_info(branch, &sb, AHEAD_BEHIND_ਭਰਿਆ, 1))
		ਵਾਪਸ;
	fputs(sb.buf, stdout);
	strbuf_release(&sb);
}

ਸਥਿਰ ਖਾਲੀ update_refs_for_switch(ਅਚਲ ਬਣਤਰ checkout_opts *opts,
				   ਬਣਤਰ branch_info *old_branch_info,
				   ਬਣਤਰ branch_info *new_branch_info)
{
	ਬਣਤਰ strbuf ਸੁਨੇਹਾ = STRBUF_INIT;
	ਅਚਲ ਅੱਖਰ *old_desc, *reflog_ਸੁਨੇਹਾ;
	ਜੇ (opts->new_branch) {
		ਜੇ (opts->new_orphan_branch) {
			ਗਿਣਤੀ log_refs_config log_all_ref_updates = LOG_REFS_UNSET;
			ਅਚਲ ਅੱਖਰ *ਮੁੱਲ;
			ਅੱਖਰ *refname;

			ਜੇ (!repo_config_get_string_tmp(the_repository, "core.logallrefupdates", &ਮੁੱਲ))
				log_all_ref_updates = refs_parse_log_all_ref_updates_config(ਮੁੱਲ);

			refname = mkpathdup("refs/heads/%s", opts->new_orphan_branch);
			ਜੇ (opts->new_branch_log &&
			    !should_autocreate_reflog(log_all_ref_updates, refname)) {
				ਪੂਰਨ ret;
				ਬਣਤਰ strbuf err = STRBUF_INIT;

				ret = refs_create_reflog(get_main_ref_store(the_repository),
							 refname, &err);
				ਜੇ (ret) {
					fprintf(stderr, _("Can not do reflog for '%s': %s\n"),
						opts->new_orphan_branch, err.buf);
					strbuf_release(&err);
					ਮੁਕਤ(refname);
					ਵਾਪਸ;
				}
				strbuf_release(&err);
			}
			ਮੁਕਤ(refname);
		}
		ਨਹੀਂਤਾਂ
			create_branch(the_repository,
				      opts->new_branch, new_branch_info->name,
				      opts->new_branch_force ? 1 : 0,
				      opts->new_branch_force ? 1 : 0,
				      opts->new_branch_log,
				      opts->quiet,
				      opts->track,
				      0);
		ਮੁਕਤ(new_branch_info->name);
		ਮੁਕਤ(new_branch_info->refname);
		new_branch_info->name = xstrdup(opts->new_branch);
		ਤਿਆਰੀ_branch_ਰਸਤਾ(new_branch_info);
	}

	old_desc = old_branch_info->name;
	ਜੇ (!old_desc && old_branch_info->commit)
		old_desc = oid_ਮੰਜ਼ਿਲ_hex(&old_branch_info->commit->object.oid);

	reflog_ਸੁਨੇਹਾ = getenv("GIT_REFLOG_ACTION");
	ਜੇ (!reflog_ਸੁਨੇਹਾ)
		strbuf_addf(&ਸੁਨੇਹਾ, "checkout: moving from %s to %s",
			old_desc ? old_desc : "(invalid)", new_branch_info->name);
	ਨਹੀਂਤਾਂ
		strbuf_insertstr(&ਸੁਨੇਹਾ, 0, reflog_ਸੁਨੇਹਾ);

	ਜੇ (!strcmp(new_branch_info->name, "HEAD") && !new_branch_info->ਰਸਤਾ && !opts->force_detach) {
		/* Nothing to do. */
	} ਨਹੀਂਤਾਂ ਜੇ (opts->force_detach || !new_branch_info->ਰਸਤਾ) {	/* No longer on any branch. */
		refs_update_ref(get_main_ref_store(the_repository), ਸੁਨੇਹਾ.buf,
				"HEAD", &new_branch_info->commit->object.oid,
				ਨਿਰਾ,
				REF_NO_DEREF, UPDATE_REFS_DIE_ON_ERR);
		ਜੇ (!opts->quiet) {
			ਜੇ (old_branch_info->ਰਸਤਾ &&
			    advice_enabled(ADVICE_DETACHED_ਸਿਰ) && !opts->force_detach)
				detach_advice(new_branch_info->name);
			describe_detached_ਸਿਰ(_("HEAD is now at"), new_branch_info->commit);
		}
	} ਨਹੀਂਤਾਂ ਜੇ (new_branch_info->ਰਸਤਾ) {	/* Switch branches. */
		ਜੇ (refs_update_symref(get_main_ref_store(the_repository), "HEAD", new_branch_info->ਰਸਤਾ, ਸੁਨੇਹਾ.buf) < 0)
			die(_("unable to update HEAD"));
		ਜੇ (!opts->quiet) {
			ਜੇ (old_branch_info->ਰਸਤਾ && !strcmp(new_branch_info->ਰਸਤਾ, old_branch_info->ਰਸਤਾ)) {
				ਜੇ (opts->new_branch_force)
					fprintf(stderr, _("Reset branch '%s'\n"),
						new_branch_info->name);
				ਨਹੀਂਤਾਂ
					fprintf(stderr, _("Already on '%s'\n"),
						new_branch_info->name);
			} ਨਹੀਂਤਾਂ ਜੇ (opts->new_branch) {
				ਜੇ (opts->branch_exists)
					fprintf(stderr, _("Switched to and reset branch '%s'\n"), new_branch_info->name);
				ਨਹੀਂਤਾਂ
					fprintf(stderr, _("Switched to a new branch '%s'\n"), new_branch_info->name);
			} ਨਹੀਂਤਾਂ {
				fprintf(stderr, _("Switched to branch '%s'\n"),
					new_branch_info->name);
			}
		}
		ਜੇ (old_branch_info->ਰਸਤਾ && old_branch_info->name) {
			ਜੇ (!refs_ref_exists(get_main_ref_store(the_repository), old_branch_info->ਰਸਤਾ) && refs_reflog_exists(get_main_ref_store(the_repository), old_branch_info->ਰਸਤਾ))
				refs_delete_reflog(get_main_ref_store(the_repository),
						   old_branch_info->ਰਸਤਾ);
		}
	}
	remove_branch_state(the_repository, !opts->quiet);
	strbuf_release(&ਸੁਨੇਹਾ);
	ਜੇ (!opts->quiet &&
	    !opts->force_detach &&
	    (new_branch_info->ਰਸਤਾ || !strcmp(new_branch_info->name, "HEAD")))
		report_tracking(new_branch_info);
}

ਸਥਿਰ ਪੂਰਨ ਜੋੜੋ_pending_uninteresting_ref(ਅਚਲ ਬਣਤਰ reference *ref, ਖਾਲੀ *cb_ਅੰਕੜੇ)
{
	ਜੋੜੋ_pending_oid(cb_ਅੰਕੜੇ, ref->name, ref->oid, UNINTERESTING);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ describe_one_orphan(ਬਣਤਰ strbuf *sb, ਬਣਤਰ commit *commit)
{
	strbuf_addstr(sb, "  ");
	strbuf_ਜੋੜੋ_unique_abbrev(sb, &commit->object.oid, DEFAULT_ABBREV);
	strbuf_addch(sb, ' ');
	ਜੇ (!repo_parse_commit(the_repository, commit))
		pp_commit_easy(CMIT_FMT_ONELINE, commit, sb);
	strbuf_addch(sb, '\n');
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ORPHAN_CUTOFF 4
ਸਥਿਰ ਖਾਲੀ suggest_reattach(ਬਣਤਰ commit *commit, ਬਣਤਰ rev_info *revs)
{
	ਬਣਤਰ commit *c, *ਆਖਰੀ = ਨਿਰਾ;
	ਬਣਤਰ strbuf sb = STRBUF_INIT;
	ਪੂਰਨ ਗੁਆਚਾ = 0;
	ਜਦੋਂ ((c = get_revision(revs)) != ਨਿਰਾ) {
		ਜੇ (ਗੁਆਚਾ < ORPHAN_CUTOFF)
			describe_one_orphan(&sb, c);
		ਆਖਰੀ = c;
		ਗੁਆਚਾ++;
	}
	ਜੇ (ORPHAN_CUTOFF < ਗੁਆਚਾ) {
		ਪੂਰਨ ਹੋਰ = ਗੁਆਚਾ - ORPHAN_CUTOFF;
		ਜੇ (ਹੋਰ == 1)
			describe_one_orphan(&sb, ਆਖਰੀ);
		ਨਹੀਂਤਾਂ
			strbuf_addf(&sb, _(" ... and %d more.\n"), ਹੋਰ);
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
		ਗੁਆਚਾ),
		ਗੁਆਚਾ,
		sb.buf);
	strbuf_release(&sb);

	ਜੇ (advice_enabled(ADVICE_DETACHED_ਸਿਰ))
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
			ਗੁਆਚਾ),
			repo_ਲੱਭੋ_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV));
}

/*
 * We are about to leave commit that was at the tip of a detached
 * HEAD.  If it is not reachable from any ref, this is the last chance
 * for the user to do so without resorting to reflog.
 */
ਸਥਿਰ ਖਾਲੀ orphaned_commit_warning(ਬਣਤਰ commit *old_commit, ਬਣਤਰ commit *new_commit)
{
	ਬਣਤਰ rev_info revs;
	ਬਣਤਰ object *object = &old_commit->object;

	repo_init_revisions(the_repository, &revs, ਨਿਰਾ);
	ਤਿਆਰੀ_revisions(0, ਨਿਰਾ, &revs, ਨਿਰਾ);

	object->flags &= ~UNINTERESTING;
	ਜੋੜੋ_pending_object(&revs, object, oid_ਮੰਜ਼ਿਲ_hex(&object->oid));

	refs_for_each_ref(get_main_ref_store(the_repository),
			  ਜੋੜੋ_pending_uninteresting_ref, &revs);
	ਜੇ (new_commit)
		ਜੋੜੋ_pending_oid(&revs, "HEAD",
				&new_commit->object.oid,
				UNINTERESTING);

	ਜੇ (prepare_revision_walk(&revs))
		die(_("internal error in revision walk"));
	ਜੇ (!(old_commit->object.flags & UNINTERESTING))
		suggest_reattach(old_commit, &revs);
	ਨਹੀਂਤਾਂ
		describe_detached_ਸਿਰ(_("Previous HEAD position was"), old_commit);

	/* Clean up objects used, as they will be reused. */
	repo_clear_commit_marks(the_repository, ALL_REV_FLAGS);
	release_revisions(&revs);
}

ਸਥਿਰ ਪੂਰਨ switch_branches(ਅਚਲ ਬਣਤਰ checkout_opts *opts,
			   ਬਣਤਰ branch_info *new_branch_info)
{
	ਪੂਰਨ ret = 0;
	ਬਣਤਰ branch_info old_branch_info = { 0 };
	ਬਣਤਰ object_id rev;
	ਪੂਰਨ flag, writeout_error = 0;
	ਪੂਰਨ do_ਮਿਲਾਓ = 1;
	ਪੂਰਨ created_autostash = 0;
	ਗਿਣਤੀ stash_apply_ਨਤੀਜਾ autostash_res = STASH_APPLY_CLEAN;
	ਬਣਤਰ strbuf old_commit_shortname = STRBUF_INIT;
	ਬਣਤਰ strbuf autostash_ਸੁਨੇਹਾ = STRBUF_INIT;
	ਅਚਲ ਅੱਖਰ *stash_label_ਅਧਾਰ = ਨਿਰਾ;

	trace2_cmd_ਬਹੁਲਕ("branch");

	memset(&old_branch_info, 0, ਆਕਾਰ(old_branch_info));
	old_branch_info.ਰਸਤਾ = refs_resolve_refdup(get_main_ref_store(the_repository),
						   "HEAD", 0, &rev, &flag);
	ਜੇ (old_branch_info.ਰਸਤਾ)
		old_branch_info.commit = lookup_commit_reference_gently(the_repository, &rev, 1);
	ਜੇ (!(flag & REF_ISSYMREF))
		FREE_AND_NULL(old_branch_info.ਰਸਤਾ);

	ਜੇ (old_branch_info.ਰਸਤਾ) {
		ਅਚਲ ਅੱਖਰ *ਅਚਲ ਅਗੇਤਰ = "refs/heads/";
		ਅਚਲ ਅੱਖਰ *p;
		ਜੇ (skip_ਅਗੇਤਰ(old_branch_info.ਰਸਤਾ, ਅਗੇਤਰ, &p))
			old_branch_info.name = xstrdup(p);
	}

	ਜੇ (opts->new_orphan_branch && opts->orphan_from_ਖਾਲੀ_ਥਾਵਾਂ_ਰੁੱਖ) {
		ਜੇ (new_branch_info->name)
			BUG("'switch --orphan' should never accept a commit as starting point");
		new_branch_info->commit = ਨਿਰਾ;
		new_branch_info->name = xstrdup("(empty)");
		do_ਮਿਲਾਓ = 1;
	}

	ਜੇ (!new_branch_info->name) {
		new_branch_info->name = xstrdup("HEAD");
		new_branch_info->commit = old_branch_info.commit;
		ਜੇ (!new_branch_info->commit)
			die(_("You are on a branch yet to be born"));
		parse_commit_or_die(new_branch_info->commit);

		ਜੇ (opts->only_ਮਿਲਾਓ_on_switching_branches)
			do_ਮਿਲਾਓ = 0;
	}

	ਜੇ (old_branch_info.name) {
		stash_label_ਅਧਾਰ = old_branch_info.name;
	} ਨਹੀਂਤਾਂ ਜੇ (old_branch_info.commit) {
		strbuf_ਜੋੜੋ_unique_abbrev(&old_commit_shortname,
					 &old_branch_info.commit->object.oid,
					 DEFAULT_ABBREV);
		stash_label_ਅਧਾਰ = old_commit_shortname.buf;
	}

	ਜੇ (do_ਮਿਲਾਓ) {
		ret = ਮਿਲਾਓ_working_ਰੁੱਖ(opts, &old_branch_info, new_branch_info,
					 opts->ਮਿਲਾਓ, &writeout_error);
		ਜੇ (ret == ਮਿਲਾਓ_WORKING_ਰੁੱਖ_UNPACK_FAILED && opts->ਮਿਲਾਓ) {
			strbuf_addf(&autostash_ਸੁਨੇਹਾ,
				    "autostash while switching to '%s'",
				    new_branch_info->name);
			create_autostash_ref(the_repository,
					     "CHECKOUT_AUTOSTASH_HEAD",
					     autostash_ਸੁਨੇਹਾ.buf, true);
			created_autostash = 1;
			ret = ਮਿਲਾਓ_working_ਰੁੱਖ(opts, &old_branch_info, new_branch_info,
						 false, &writeout_error);
		}
		ਜੇ (created_autostash) {
			ਜੇ (opts->conflict_style >= 0) {
				ਬਣਤਰ strbuf cfg = STRBUF_INIT;
				strbuf_addf(&cfg, "merge.conflictStyle=%s",
					    conflict_style_name(opts->conflict_style));
				git_config_ਧੱਕੋ_parameter(cfg.buf);
				strbuf_release(&cfg);
			}
			autostash_res = apply_autostash_ref(the_repository,
				    "CHECKOUT_AUTOSTASH_HEAD",
				    new_branch_info->name,
				    "local",
				    stash_label_ਅਧਾਰ,
				    autostash_ਸੁਨੇਹਾ.buf);
		}
		ਜੇ (ret) {
			branch_info_release(&old_branch_info);
			strbuf_release(&old_commit_shortname);
			strbuf_release(&autostash_ਸੁਨੇਹਾ);
			ਵਾਪਸ ret < 0 ? 1 : ret;
		}
	}

	ਜੇ (!opts->quiet && !old_branch_info.ਰਸਤਾ && old_branch_info.commit && new_branch_info->commit != old_branch_info.commit)
		orphaned_commit_warning(old_branch_info.commit, new_branch_info->commit);

	ਜੇ (autostash_res == STASH_APPLY_CONFLICT && !opts->quiet)
		fputc('\n', stderr);
	update_refs_for_switch(opts, &old_branch_info, new_branch_info);

	ਜੇ (created_autostash) {
		discard_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ);
		ਜੇ (repo_read_ਸੂਚਕਾਂਕ(the_repository) < 0)
			die(_("index file corrupt"));

		ਜੇ (!opts->quiet && new_branch_info->commit) {
			ਲਿਖੋ(_("The following paths have local changes:\n"));
			show_local_changes(&new_branch_info->commit->object,
					   &opts->diff_options);
		}
	}

	ret = post_checkout_hook(old_branch_info.commit, new_branch_info->commit, 1);
	branch_info_release(&old_branch_info);
	strbuf_release(&old_commit_shortname);
	strbuf_release(&autostash_ਸੁਨੇਹਾ);

	ਵਾਪਸ ret || writeout_error;
}

ਸਥਿਰ ਪੂਰਨ git_checkout_config(ਅਚਲ ਅੱਖਰ *var, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ,
			       ਅਚਲ ਬਣਤਰ config_context *ctx, ਖਾਲੀ *cb)
{
	ਬਣਤਰ checkout_opts *opts = cb;

	ਜੇ (!strcmp(var, "diff.ignoresubmodules")) {
		ਜੇ (!ਮੁੱਲ)
			ਵਾਪਸ config_error_nonbool(var);
		handle_ignore_submodules_arg(&opts->diff_options, ਮੁੱਲ);
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(var, "checkout.guess")) {
		opts->dwim_new_local_branch = git_config_bool(var, ਮੁੱਲ);
		ਵਾਪਸ 0;
	}

	ਜੇ (starts_with(var, "submodule."))
		ਵਾਪਸ git_default_submodule_config(var, ਮੁੱਲ, ਨਿਰਾ);

	ਵਾਪਸ git_xmerge_config(var, ਮੁੱਲ, ctx, ਨਿਰਾ);
}

ਸਥਿਰ ਖਾਲੀ ਤਿਆਰੀ_new_branch_info_and_ਸਰੋਤ_ਰੁੱਖ(
	ਬਣਤਰ branch_info *new_branch_info,
	ਬਣਤਰ checkout_opts *opts,
	ਬਣਤਰ object_id *rev,
	ਅਚਲ ਅੱਖਰ *arg)
{
	ਬਣਤਰ ਰੁੱਖ **ਸਰੋਤ_ਰੁੱਖ = &opts->ਸਰੋਤ_ਰੁੱਖ;
	ਬਣਤਰ object_id branch_rev;

	/* treat '@' as a shortcut for 'HEAD' */
	new_branch_info->name = !strcmp(arg, "@") ? xstrdup("HEAD") :
						    xstrdup(arg);
	ਤਿਆਰੀ_branch_ਰਸਤਾ(new_branch_info);

	ਜੇ (!check_refname_format(new_branch_info->ਰਸਤਾ, 0) &&
	    !refs_read_ref(get_main_ref_store(the_repository), new_branch_info->ਰਸਤਾ, &branch_rev))
		oidcpy(rev, &branch_rev);
	ਨਹੀਂਤਾਂ
		/* not an existing branch */
		FREE_AND_NULL(new_branch_info->ਰਸਤਾ);

	new_branch_info->commit = lookup_commit_reference_gently(the_repository, rev, 1);
	ਜੇ (!new_branch_info->commit) {
		/* not a commit */
		*ਸਰੋਤ_ਰੁੱਖ = repo_parse_ਰੁੱਖ_ਅਸਿੱਧਾ(the_repository, rev);
		ਜੇ (!*ਸਰੋਤ_ਰੁੱਖ)
			die(_("unable to read tree (%s)"), oid_ਮੰਜ਼ਿਲ_hex(rev));
	} ਨਹੀਂਤਾਂ {
		parse_commit_or_die(new_branch_info->commit);
		*ਸਰੋਤ_ਰੁੱਖ = repo_get_commit_ਰੁੱਖ(the_repository,
						    new_branch_info->commit);
		ਜੇ (!*ਸਰੋਤ_ਰੁੱਖ)
			die(_("unable to read tree (%s)"),
			    oid_ਮੰਜ਼ਿਲ_hex(&new_branch_info->commit->object.oid));
	}
}


ਗਿਣਤੀ checkout_command {
	CHECKOUT_CHECKOUT = 1,
	CHECKOUT_SWITCH = 2,
	CHECKOUT_RESTORE = 3,
};

ਸਥਿਰ ਖਾਲੀ advise_disambiguating_remotes(ਗਿਣਤੀ checkout_command which_command,
					  ਅਚਲ ਅੱਖਰ *branch,
					  ਅਚਲ ਬਣਤਰ string_list *matched_remote_ਨਾਂ)
{
	ਅਚਲ ਅੱਖਰ *cmdname;
	ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ;

	ਚੋਣ (which_command) {
	ਹਾਲਤ CHECKOUT_CHECKOUT:
		cmdname = "checkout";
		ਰੋਕੋ;
	ਹਾਲਤ CHECKOUT_SWITCH:
		cmdname = "switch";
		ਰੋਕੋ;
	ਮੂਲ:
		BUG("command <%d> should not reach advise_disambiguating_remotes",
		    which_command);
		ਰੋਕੋ;
	}

	advise(_("Branch name '%s' appears in multiple remotes:"), branch);
	for_each_string_list_ਵਸਤੂ(ਵਸਤੂ, matched_remote_ਨਾਂ) {
		advise(_("  %s"), ਵਸਤੂ->string);
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

ਸਥਿਰ ਅੱਖਰ *parse_remote_branch(ਅਚਲ ਅੱਖਰ *arg,
				 ਬਣਤਰ object_id *rev,
				 ਪੂਰਨ could_be_checkout_paths,
				 ਗਿਣਤੀ checkout_command which_command)
{
	ਪੂਰਨ ਅੰਕ_matches = 0;
	ਬਣਤਰ string_list matched_remote_ਨਾਂ = STRING_LIST_INIT_DUP;

	ਅੱਖਰ *remote = unique_tracking_name(arg, rev, &ਅੰਕ_matches,
					    &matched_remote_ਨਾਂ);

	ਜੇ (remote && could_be_checkout_paths) {
		die(_("'%s' could be both a local file and a tracking branch.\n"
			"Please use -- (and optionally --no-guess) to disambiguate"),
		    arg);
	}

	ਜੇ (!remote && ਅੰਕ_matches > 1) {
		ਜੇ (advice_enabled(ADVICE_CHECKOUT_AMBIGUOUS_REMOTE_BRANCH_NAME))
			advise_disambiguating_remotes(which_command, arg,
						      &matched_remote_ਨਾਂ);
		die(_("'%s' matched multiple (%d) remote tracking branches"),
		    arg, ਅੰਕ_matches);
	}

	string_list_clear(&matched_remote_ਨਾਂ, 0);

	ਵਾਪਸ remote;
}

ਸਥਿਰ ਪੂਰਨ parse_branchname_arg(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv,
				ਪੂਰਨ dwim_new_local_branch_ok,
				ਗਿਣਤੀ checkout_command which_command,
				ਬਣਤਰ branch_info *new_branch_info,
				ਬਣਤਰ checkout_opts *opts,
				ਬਣਤਰ object_id *rev)
{
	ਅਚਲ ਅੱਖਰ **new_branch = &opts->new_branch;
	ਪੂਰਨ argcount = 0;
	ਅਚਲ ਅੱਖਰ *arg;
	ਅੱਖਰ *remote = ਨਿਰਾ;
	ਪੂਰਨ dash_dash_pos;
	ਪੂਰਨ has_dash_dash = 0;
	ਪੂਰਨ i;

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
	ਜੇ (!argc)
		ਵਾਪਸ 0;

	ਜੇ (!opts->accept_pathspec) {
		ਜੇ (argc > 1)
			die(_("only one reference expected"));
		has_dash_dash = 1; /* helps disambiguate */
	}

	arg = argv[0];
	dash_dash_pos = -1;
	ਲਈ (i = 0; i < argc; i++) {
		ਜੇ (opts->accept_pathspec && !strcmp(argv[i], "--")) {
			dash_dash_pos = i;
			ਰੋਕੋ;
		}
	}
	ਜੇ (dash_dash_pos == 0)
		ਵਾਪਸ 1; /* case (2) */
	ਨਹੀਂਤਾਂ ਜੇ (dash_dash_pos == 1)
		has_dash_dash = 1; /* case (3) or (1) */
	ਨਹੀਂਤਾਂ ਜੇ (dash_dash_pos >= 2)
		die(_("only one reference expected, %d given."), dash_dash_pos);
	opts->ਗਿਣਤੀ_checkout_paths = !opts->quiet && !has_dash_dash;

	ਜੇ (!strcmp(arg, "-"))
		arg = "@{-1}";

	ਜੇ (repo_get_oid_mb(the_repository, arg, rev)) {
		/*
		 * Either case (3) or (4), with <something> not being
		 * a commit, or an attempt to use case (1) with an
		 * invalid ref.
		 *
		 * It's likely an error, but we need to find out if
		 * we should auto-create the branch, case (3).(b).
		 */
		ਪੂਰਨ recover_with_dwim = dwim_new_local_branch_ok;

		ਪੂਰਨ could_be_checkout_paths = !has_dash_dash &&
			check_filename(opts->ਅਗੇਤਰ, arg);

		ਜੇ (!has_dash_dash && !no_wildcard(arg))
			recover_with_dwim = 0;

		/*
		 * Accept "git checkout foo", "git checkout foo --"
		 * and "git switch foo" as candidates for dwim.
		 */
		ਜੇ (!(argc == 1 && !has_dash_dash) &&
		    !(argc == 2 && has_dash_dash) &&
		    opts->accept_pathspec)
			recover_with_dwim = 0;

		ਜੇ (recover_with_dwim) {
			remote = parse_remote_branch(arg, rev,
						     could_be_checkout_paths,
						     which_command);
			ਜੇ (remote) {
				*new_branch = arg;
				arg = remote;
				/* DWIMmed to create local branch, case (3).(b) */
			} ਨਹੀਂਤਾਂ {
				recover_with_dwim = 0;
			}
		}

		ਜੇ (!recover_with_dwim) {
			ਜੇ (has_dash_dash)
				die(_("invalid reference: %s"), arg);
			ਵਾਪਸ argcount;
		}
	}

	/* we can't end up being in (2) anymore, eat the argument */
	argcount++;
	argv++;
	argc--;

	ਤਿਆਰੀ_new_branch_info_and_ਸਰੋਤ_ਰੁੱਖ(new_branch_info, opts, rev, arg);

	ਜੇ (!opts->ਸਰੋਤ_ਰੁੱਖ)                   /* case (1): want a tree */
		die(_("reference is not a tree: %s"), arg);

	ਜੇ (!has_dash_dash) {	/* case (3).(d) -> (1) */
		/*
		 * Do not complain the most common case
		 *	git checkout branch
		 * even if there happen to be a file called 'branch';
		 * it would be extremely annoying.
		 */
		ਜੇ (argc)
			verify_non_filename(the_repository, opts->ਅਗੇਤਰ, arg);
	} ਨਹੀਂਤਾਂ ਜੇ (opts->accept_pathspec) {
		argcount++;
		argv++;
		argc--;
	}

	ਮੁਕਤ(remote);
	ਵਾਪਸ argcount;
}

ਸਥਿਰ ਪੂਰਨ switch_unborn_ਮੰਜ਼ਿਲ_new_branch(ਅਚਲ ਬਣਤਰ checkout_opts *opts)
{
	ਪੂਰਨ ਸਥਿਤੀ;
	ਬਣਤਰ strbuf branch_ref = STRBUF_INIT;

	trace2_cmd_ਬਹੁਲਕ("unborn");

	ਜੇ (!opts->new_branch)
		die(_("You are on a branch yet to be born"));
	strbuf_addf(&branch_ref, "refs/heads/%s", opts->new_branch);
	ਸਥਿਤੀ = refs_update_symref(get_main_ref_store(the_repository),
				    "HEAD", branch_ref.buf, "checkout -b");
	strbuf_release(&branch_ref);
	ਜੇ (!opts->quiet)
		fprintf(stderr, _("Switched to a new branch '%s'\n"),
			opts->new_branch);
	ਵਾਪਸ ਸਥਿਤੀ;
}

ਸਥਿਰ ਖਾਲੀ die_expecting_a_branch(ਅਚਲ ਬਣਤਰ branch_info *branch_info)
{
	ਬਣਤਰ object_id oid;
	ਅੱਖਰ *ਮੰਜ਼ਿਲ_free;
	ਪੂਰਨ ਕੋਡ;

	ਜੇ (repo_dwim_ref(the_repository, branch_info->name,
			  ਲੜੀ_ਲੰਬਾਈ(branch_info->name), &oid, &ਮੰਜ਼ਿਲ_free, 0) == 1) {
		ਅਚਲ ਅੱਖਰ *ref = ਮੰਜ਼ਿਲ_free;

		ਜੇ (skip_ਅਗੇਤਰ(ref, "refs/tags/", &ref))
			ਕੋਡ = die_ਸੁਨੇਹਾ(_("a branch is expected, got tag '%s'"), ref);
		ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(ref, "refs/remotes/", &ref))
			ਕੋਡ = die_ਸੁਨੇਹਾ(_("a branch is expected, got remote branch '%s'"), ref);
		ਨਹੀਂਤਾਂ
			ਕੋਡ = die_ਸੁਨੇਹਾ(_("a branch is expected, got '%s'"), ref);
	}
	ਨਹੀਂਤਾਂ ਜੇ (branch_info->commit)
		ਕੋਡ = die_ਸੁਨੇਹਾ(_("a branch is expected, got commit '%s'"), branch_info->name);
	ਨਹੀਂਤਾਂ
		/*
		 * This case should never happen because we already die() on
		 * non-commit, but just in case.
		 */
		ਕੋਡ = die_ਸੁਨੇਹਾ(_("a branch is expected, got '%s'"), branch_info->name);

	ਜੇ (advice_enabled(ADVICE_SUGGEST_DETACHING_ਸਿਰ))
		advise(_("If you want to detach HEAD at the commit, try again with the --detach option."));

	ਬਾਹਰ_ਨਿਕਲੋ(ਕੋਡ);
}

ਸਥਿਰ ਖਾਲੀ die_if_some_operation_ਵਿੱਚ_ਤਰੱਕੀ(ਖਾਲੀ)
{
	ਬਣਤਰ wt_ਸਥਿਤੀ_state state;

	memset(&state, 0, ਆਕਾਰ(state));
	wt_ਸਥਿਤੀ_get_state(the_repository, &state, 0);

	ਜੇ (state.ਮਿਲਾਓ_ਵਿੱਚ_ਤਰੱਕੀ)
		die(_("cannot switch branch while merging\n"
		      "Consider \"git merge --quit\" "
		      "or \"git worktree add\"."));
	ਜੇ (state.am_ਵਿੱਚ_ਤਰੱਕੀ)
		die(_("cannot switch branch in the middle of an am session\n"
		      "Consider \"git am --quit\" "
		      "or \"git worktree add\"."));
	ਜੇ (state.rebase_interactive_ਵਿੱਚ_ਤਰੱਕੀ || state.rebase_ਵਿੱਚ_ਤਰੱਕੀ)
		die(_("cannot switch branch while rebasing\n"
		      "Consider \"git rebase --quit\" "
		      "or \"git worktree add\"."));
	ਜੇ (state.cherry_pick_ਵਿੱਚ_ਤਰੱਕੀ)
		die(_("cannot switch branch while cherry-picking\n"
		      "Consider \"git cherry-pick --quit\" "
		      "or \"git worktree add\"."));
	ਜੇ (state.revert_ਵਿੱਚ_ਤਰੱਕੀ)
		die(_("cannot switch branch while reverting\n"
		      "Consider \"git revert --quit\" "
		      "or \"git worktree add\"."));
	ਜੇ (state.bisect_ਵਿੱਚ_ਤਰੱਕੀ)
		warning(_("you are switching branch while bisecting"));

	wt_ਸਥਿਤੀ_state_free_buffers(&state);
}

/*
 * die if attempting to checkout an existing branch that is in use
 * in another worktree, unless ignore-other-wortrees option is given.
 * The check is bypassed when the branch is already the current one,
 * as it will not make things any worse.
 */
ਸਥਿਰ ਖਾਲੀ die_if_switching_ਮੰਜ਼ਿਲ_a_branch_ਵਿੱਚ_use(ਬਣਤਰ checkout_opts *opts,
						ਅਚਲ ਅੱਖਰ *ਭਰਿਆ_ref)
{
	ਪੂਰਨ flags;
	ਅੱਖਰ *ਸਿਰ_ref;

	ਜੇ (opts->ignore_other_worktrees)
		ਵਾਪਸ;
	ਸਿਰ_ref = refs_resolve_refdup(get_main_ref_store(the_repository),
				       "HEAD", 0, ਨਿਰਾ, &flags);
	ਜੇ (ਸਿਰ_ref && (!(flags & REF_ISSYMREF) || strcmp(ਸਿਰ_ref, ਭਰਿਆ_ref)))
		die_if_checked_ਬਾਹਰਲਾ(ਭਰਿਆ_ref, 1);
	ਮੁਕਤ(ਸਿਰ_ref);
}

ਸਥਿਰ ਪੂਰਨ checkout_branch(ਬਣਤਰ checkout_opts *opts,
			   ਬਣਤਰ branch_info *new_branch_info)
{
	ਬਣਤਰ repo_config_ਮੁੱਲਾਂ *cfg = repo_config_ਮੁੱਲਾਂ(the_repository);
	ਪੂਰਨ noop_switch = (!new_branch_info->name &&
			   !opts->new_branch &&
			   !opts->force_detach);

	ਜੇ (opts->pathspec.nr)
		die(_("paths cannot be used with switching branches"));

	ਜੇ (opts->patch_ਬਹੁਲਕ)
		die(_("'%s' cannot be used with switching branches"),
		    "--patch");

	ਜੇ (opts->overlay_ਬਹੁਲਕ != -1)
		die(_("'%s' cannot be used with switching branches"),
		    "--[no]-overlay");

	ਜੇ (opts->writeout_stage) {
		ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ;
		ਜੇ (noop_switch)
			ਸੁਨੇਹਾ = _("'%s' needs the paths to check out");
		ਨਹੀਂਤਾਂ
			ਸੁਨੇਹਾ = _("'%s' cannot be used with switching branches");
		die(ਸੁਨੇਹਾ, "--ours/--theirs");
	}

	ਜੇ (opts->force && opts->ਮਿਲਾਓ)
		die(_("'%s' cannot be used with '%s'"), "-f", "-m");

	ਜੇ (opts->discard_changes && opts->ਮਿਲਾਓ)
		die(_("'%s' cannot be used with '%s'"), "--discard-changes", "--merge");

	ਜੇ (opts->force_detach && opts->new_branch)
		die(_("'%s' cannot be used with '%s'"),
		    "--detach", "-b/-B/--orphan");

	ਜੇ (opts->new_orphan_branch) {
		ਜੇ (opts->track != BRANCH_TRACK_UNSPECIFIED)
			die(_("'%s' cannot be used with '%s'"), "--orphan", "-t");
		ਜੇ (opts->orphan_from_ਖਾਲੀ_ਥਾਵਾਂ_ਰੁੱਖ && new_branch_info->name)
			die(_("'%s' cannot take <start-point>"), "--orphan");
	} ਨਹੀਂਤਾਂ ਜੇ (opts->force_detach) {
		ਜੇ (opts->track != BRANCH_TRACK_UNSPECIFIED)
			die(_("'%s' cannot be used with '%s'"), "--detach", "-t");
	} ਨਹੀਂਤਾਂ ਜੇ (opts->track == BRANCH_TRACK_UNSPECIFIED)
		opts->track = cfg->branch_track;

	ਜੇ (new_branch_info->name && !new_branch_info->commit)
		die(_("Cannot switch branch to a non-commit '%s'"),
		    new_branch_info->name);

	ਜੇ (noop_switch &&
	    !opts->switch_branch_doing_nothing_is_ok)
		die(_("missing branch or commit argument"));

	ਜੇ (!opts->implicit_detach &&
	    !opts->force_detach &&
	    !opts->new_branch &&
	    !opts->new_branch_force &&
	    new_branch_info->name &&
	    !new_branch_info->ਰਸਤਾ)
		die_expecting_a_branch(new_branch_info);

	ਜੇ (!opts->can_switch_when_ਵਿੱਚ_ਤਰੱਕੀ)
		die_if_some_operation_ਵਿੱਚ_ਤਰੱਕੀ();

	/* "git checkout <branch>" */
	ਜੇ (new_branch_info->ਰਸਤਾ && !opts->force_detach && !opts->new_branch)
		die_if_switching_ਮੰਜ਼ਿਲ_a_branch_ਵਿੱਚ_use(opts, new_branch_info->ਰਸਤਾ);

	/* "git checkout -B <branch>" */
	ਜੇ (opts->new_branch_force) {
		ਅੱਖਰ *ਭਰਿਆ_ref = xstrfmt("refs/heads/%s", opts->new_branch);
		die_if_switching_ਮੰਜ਼ਿਲ_a_branch_ਵਿੱਚ_use(opts, ਭਰਿਆ_ref);
		ਮੁਕਤ(ਭਰਿਆ_ref);
	}

	ਜੇ (!new_branch_info->commit && opts->new_branch) {
		ਬਣਤਰ object_id rev;
		ਪੂਰਨ flag;

		ਜੇ (!refs_read_ref_ਭਰਿਆ(get_main_ref_store(the_repository), "HEAD", 0, &rev, &flag) &&
		    (flag & REF_ISSYMREF) && is_null_oid(&rev))
			ਵਾਪਸ switch_unborn_ਮੰਜ਼ਿਲ_new_branch(opts);
	}
	ਵਾਪਸ switch_branches(opts, new_branch_info);
}

ਸਥਿਰ ਪੂਰਨ parse_opt_conflict(ਅਚਲ ਬਣਤਰ option *o, ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਬਣਤਰ checkout_opts *opts = o->ਮੁੱਲ;

	ਜੇ (unset) {
		opts->conflict_style = -1;
		ਵਾਪਸ 0;
	}
	opts->conflict_style = parse_conflict_style_name(arg);
	ਜੇ (opts->conflict_style < 0)
		ਵਾਪਸ error(_("unknown conflict style '%s'"), arg);
	/* --conflict overrides a previous --no-merge */
	ਜੇ (!opts->ਮਿਲਾਓ)
		opts->ਮਿਲਾਓ = -1;

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਬਣਤਰ option *ਜੋੜੋ_common_options(ਬਣਤਰ checkout_opts *opts,
					 ਬਣਤਰ option *prevopts)
{
	ਬਣਤਰ option options[] = {
		OPT__QUIET(&opts->quiet, N_("suppress progress reporting")),
		OPT_CALLBACK_F(0, "recurse-submodules", ਨਿਰਾ,
			    "checkout", "control recursive updating of submodules",
			    PARSE_OPT_OPTARG, option_parse_recurse_submodules_worktree_updater),
		OPT_BOOL(0, "progress", &opts->show_ਤਰੱਕੀ, N_("force progress reporting")),
		OPT_BOOL('m', "merge", &opts->ਮਿਲਾਓ, N_("perform a 3-way merge with the new branch")),
		OPT_CALLBACK(0, "conflict", opts, N_("style"),
			     N_("conflict style (merge, diff3, or zdiff3)"),
			     parse_opt_conflict),
		OPT_ਸਿਰਾ()
	};
	ਬਣਤਰ option *newopts = parse_options_concat(prevopts, options);
	ਮੁਕਤ(prevopts);
	ਵਾਪਸ newopts;
}

ਸਥਿਰ ਬਣਤਰ option *ਜੋੜੋ_common_switch_branch_options(
	ਬਣਤਰ checkout_opts *opts, ਬਣਤਰ option *prevopts)
{
	ਬਣਤਰ option options[] = {
		OPT_BOOL('d', "detach", &opts->force_detach, N_("detach HEAD at named commit")),
		OPT_CALLBACK_F('t', "track",  &opts->track, "(direct|inherit)",
			N_("set branch tracking configuration"),
			PARSE_OPT_OPTARG,
			parse_opt_tracking_ਬਹੁਲਕ),
		OPT__FORCE(&opts->force, N_("force checkout (throw away local modifications)"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_STRING(0, "orphan", &opts->new_orphan_branch, N_("new-branch"), N_("new unborn branch")),
		OPT_BOOL_F(0, "overwrite-ignore", &opts->overwrite_ignore,
			   N_("update ignored files (default)"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "ignore-other-worktrees", &opts->ignore_other_worktrees,
			 N_("do not check if another worktree is using this branch")),
		OPT_ਸਿਰਾ()
	};
	ਬਣਤਰ option *newopts = parse_options_concat(prevopts, options);
	ਮੁਕਤ(prevopts);
	ਵਾਪਸ newopts;
}

ਸਥਿਰ ਬਣਤਰ option *ਜੋੜੋ_checkout_ਰਸਤਾ_options(ਬਣਤਰ checkout_opts *opts,
						ਬਣਤਰ option *prevopts)
{
	ਬਣਤਰ option options[] = {
		OPT_ਸੈੱਟ_INT_F('2', "ours", &opts->writeout_stage,
			      N_("checkout our version for unmerged files"),
			      2, PARSE_OPT_NONEG),
		OPT_ਸੈੱਟ_INT_F('3', "theirs", &opts->writeout_stage,
			      N_("checkout their version for unmerged files"),
			      3, PARSE_OPT_NONEG),
		OPT_BOOL('p', "patch", &opts->patch_ਬਹੁਲਕ, N_("select hunks interactively")),
		OPT_DIFF_UNIFIED(&opts->patch_context),
		OPT_DIFF_INTERHUNK_CONTEXT(&opts->patch_interhunk_context),
		OPT_BOOL(0, "ignore-skip-worktree-bits", &opts->ignore_skipworktree,
			 N_("do not limit pathspecs to sparse entries only")),
		OPT_PATHSPEC_FROM_FILE(&opts->pathspec_from_file),
		OPT_PATHSPEC_FILE_NUL(&opts->pathspec_file_nul),
		OPT_ਸਿਰਾ()
	};
	ਬਣਤਰ option *newopts = parse_options_concat(prevopts, options);
	ਮੁਕਤ(prevopts);
	ਵਾਪਸ newopts;
}

/* create-branch option (either b or c) */
ਸਥਿਰ ਅੱਖਰ cb_option = 'b';

ਸਥਿਰ ਪੂਰਨ checkout_main(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			 ਬਣਤਰ checkout_opts *opts, ਬਣਤਰ option *options,
			 ਗਿਣਤੀ checkout_command which_command)
{
	ਪੂਰਨ parseopt_flags = 0;
	ਬਣਤਰ branch_info new_branch_info = { 0 };
	ਪੂਰਨ ret;

	ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ checkout_usage[] = {
		N_("git checkout [<options>] <branch>"),
		N_("git checkout [<options>] [<branch>] -- <file>..."),
		ਨਿਰਾ,
	};

	ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ switch_branch_usage[] = {
		N_("git switch [<options>] [<branch>]"),
		ਨਿਰਾ,
	};

	ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ restore_usage[] = {
		N_("git restore [<options>] [--source=<branch>] <file>..."),
		ਨਿਰਾ,
	};

	ਅਚਲ ਅੱਖਰ * ਅਚਲ *usagestr;

	ਚੋਣ (which_command) {
	ਹਾਲਤ CHECKOUT_CHECKOUT:
		usagestr = checkout_usage;
		ਰੋਕੋ;
	ਹਾਲਤ CHECKOUT_SWITCH:
		usagestr = switch_branch_usage;
		ਰੋਕੋ;
	ਹਾਲਤ CHECKOUT_RESTORE:
		usagestr = restore_usage;
		ਰੋਕੋ;
	ਮੂਲ:
		BUG("no such checkout variant %d", which_command);
	}

	opts->overwrite_ignore = 1;
	opts->ਅਗੇਤਰ = ਅਗੇਤਰ;
	opts->show_ਤਰੱਕੀ = -1;

	repo_config(the_repository, git_checkout_config, opts);
	ਜੇ (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_ਭਰਿਆ_ਸੂਚਕਾਂਕ = 0;
	}

	opts->track = BRANCH_TRACK_UNSPECIFIED;

	ਜੇ (!opts->accept_pathspec && !opts->accept_ref)
		BUG("make up your mind, you need to take _something_");
	ਜੇ (opts->accept_pathspec && opts->accept_ref)
		parseopt_flags = PARSE_OPT_KEEP_DASHDASH;

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options,
			     usagestr, parseopt_flags);

	ਜੇ (opts->patch_context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	ਜੇ (opts->patch_interhunk_context < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	ਜੇ (!opts->patch_ਬਹੁਲਕ) {
		ਜੇ (opts->patch_context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--patch");
		ਜੇ (opts->patch_interhunk_context != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--patch");
		ਜੇ (!opts->auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--patch");
	}

	ਜੇ (opts->show_ਤਰੱਕੀ < 0) {
		ਜੇ (opts->quiet)
			opts->show_ਤਰੱਕੀ = 0;
		ਨਹੀਂਤਾਂ
			opts->show_ਤਰੱਕੀ = isatty(2);
	}

	/* --conflicts implies --merge */
	ਜੇ (opts->ਮਿਲਾਓ == -1)
		opts->ਮਿਲਾਓ = opts->conflict_style >= 0;

	ਜੇ (opts->force) {
		opts->discard_changes = 1;
		opts->ignore_unmerged_opt = "--force";
		opts->ignore_unmerged = 1;
	}

	ਜੇ ((!!opts->new_branch + !!opts->new_branch_force + !!opts->new_orphan_branch) > 1)
		die(_("options '-%c', '-%c', and '%s' cannot be used together"),
			cb_option, ਵੱਡਾ_ਅੱਖਰ(cb_option), "--orphan");

	ਜੇ (opts->overlay_ਬਹੁਲਕ == 1 && opts->patch_ਬਹੁਲਕ)
		die(_("options '%s' and '%s' cannot be used together"), "-p", "--overlay");

	ਜੇ (opts->checkout_ਸੂਚਕਾਂਕ >= 0 || opts->checkout_worktree >= 0) {
		ਜੇ (opts->checkout_ਸੂਚਕਾਂਕ < 0)
			opts->checkout_ਸੂਚਕਾਂਕ = 0;
		ਜੇ (opts->checkout_worktree < 0)
			opts->checkout_worktree = 0;
	} ਨਹੀਂਤਾਂ {
		ਜੇ (opts->checkout_ਸੂਚਕਾਂਕ < 0)
			opts->checkout_ਸੂਚਕਾਂਕ = -opts->checkout_ਸੂਚਕਾਂਕ - 1;
		ਜੇ (opts->checkout_worktree < 0)
			opts->checkout_worktree = -opts->checkout_worktree - 1;
	}
	ਜੇ (opts->checkout_ਸੂਚਕਾਂਕ < 0 || opts->checkout_worktree < 0)
		BUG("these flags should be non-negative by now");
	/*
	 * convenient shortcut: "git restore --staged [--worktree]" equals
	 * "git restore --staged [--worktree] --source HEAD"
	 */
	ਜੇ (!opts->from_treeish && opts->checkout_ਸੂਚਕਾਂਕ)
		opts->from_treeish = "HEAD";

	/*
	 * From here on, new_branch will contain the branch to be checked out,
	 * and new_branch_force and new_orphan_branch will tell us which one of
	 * -b/-B/-c/-C/--orphan is being used.
	 */
	ਜੇ (opts->new_branch_force)
		opts->new_branch = opts->new_branch_force;

	ਜੇ (opts->new_orphan_branch)
		opts->new_branch = opts->new_orphan_branch;

	/* --track without -c/-C/-b/-B/--orphan should DWIM */
	ਜੇ (opts->track != BRANCH_TRACK_UNSPECIFIED && !opts->new_branch) {
		ਅਚਲ ਅੱਖਰ *argv0 = argv[0];
		ਜੇ (!argc || !strcmp(argv0, "--"))
			die(_("--track needs a branch name"));
		skip_ਅਗੇਤਰ(argv0, "refs/", &argv0);
		skip_ਅਗੇਤਰ(argv0, "remotes/", &argv0);
		argv0 = strchr(argv0, '/');
		ਜੇ (!argv0 || !argv0[1])
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
	ਜੇ (argc && opts->accept_ref) {
		ਬਣਤਰ object_id rev;
		ਪੂਰਨ dwim_ok =
			!opts->patch_ਬਹੁਲਕ &&
			opts->dwim_new_local_branch &&
			opts->track == BRANCH_TRACK_UNSPECIFIED &&
			!opts->new_branch;
		ਪੂਰਨ n = parse_branchname_arg(argc, argv, dwim_ok, which_command,
					     &new_branch_info, opts, &rev);
		argv += n;
		argc -= n;
	} ਨਹੀਂਤਾਂ ਜੇ (!opts->accept_ref && opts->from_treeish) {
		ਬਣਤਰ object_id rev;

		ਜੇ (repo_get_oid_mb(the_repository, opts->from_treeish, &rev))
			die(_("could not resolve '%s'"), opts->from_treeish);

		ਤਿਆਰੀ_new_branch_info_and_ਸਰੋਤ_ਰੁੱਖ(&new_branch_info,
						      opts, &rev,
						      opts->from_treeish);

		ਜੇ (!opts->ਸਰੋਤ_ਰੁੱਖ)
			die(_("reference is not a tree: %s"), opts->from_treeish);
	}

	ਜੇ (argc) {
		parse_pathspec(&opts->pathspec, 0,
			       opts->patch_ਬਹੁਲਕ ? PATHSPEC_ਅਗੇਤਰ_ORIGIN : 0,
			       ਅਗੇਤਰ, argv);

		ਜੇ (!opts->pathspec.nr)
			die(_("invalid path specification"));

		/*
		 * Try to give more helpful suggestion.
		 * new_branch && argc > 1 will be caught later.
		 */
		ਜੇ (opts->new_branch && argc == 1 && !new_branch_info.commit)
			die(_("'%s' is not a commit and a branch '%s' cannot be created from it"),
				argv[0], opts->new_branch);

		ਜੇ (opts->force_detach)
			die(_("git checkout: --detach does not take a path argument '%s'"),
			    argv[0]);
	}

	ਜੇ (opts->pathspec_from_file) {
		ਜੇ (opts->pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		ਜੇ (opts->force_detach)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--detach");

		ਜੇ (opts->patch_ਬਹੁਲਕ)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--patch");

		parse_pathspec_file(&opts->pathspec, 0,
				    0,
				    ਅਗੇਤਰ, opts->pathspec_from_file, opts->pathspec_file_nul);
	} ਨਹੀਂਤਾਂ ਜੇ (opts->pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	opts->pathspec.recursive = 1;

	ਜੇ (opts->pathspec.nr) {
		ਜੇ (1 < !!opts->writeout_stage + !!opts->force + !!opts->ਮਿਲਾਓ)
			die(_("git checkout: --ours/--theirs, --force and --merge are incompatible when\n"
			      "checking out of the index."));
	} ਨਹੀਂਤਾਂ {
		ਜੇ (opts->accept_pathspec && !opts->ਖਾਲੀ_ਥਾਵਾਂ_pathspec_ok &&
		    !opts->patch_ਬਹੁਲਕ)	/* patch mode is special */
			die(_("you must specify path(s) to restore"));
	}

	ਜੇ (opts->new_branch) {
		ਬਣਤਰ strbuf buf = STRBUF_INIT;

		ਜੇ (opts->new_branch_force)
			opts->branch_exists = validate_branchname(opts->new_branch, &buf);
		ਨਹੀਂਤਾਂ
			opts->branch_exists =
				validate_new_branchname(opts->new_branch, &buf, 0);
		strbuf_release(&buf);
	}

	ਜੇ (opts->patch_ਬਹੁਲਕ || opts->pathspec.nr)
		ret = checkout_paths(opts, &new_branch_info);
	ਨਹੀਂਤਾਂ
		ret = checkout_branch(opts, &new_branch_info);

	branch_info_release(&new_branch_info);
	clear_pathspec(&opts->pathspec);
	ਮੁਕਤ(opts->pathspec_from_file);
	ਮੁਕਤ(options);

	ਵਾਪਸ ret;
}

ਪੂਰਨ cmd_checkout(ਪੂਰਨ argc,
		 ਅਚਲ ਅੱਖਰ **argv,
		 ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		 ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ checkout_opts opts = CHECKOUT_OPTS_INIT;
	ਬਣਤਰ option *options;
	ਬਣਤਰ option checkout_options[] = {
		OPT_STRING('b', ਨਿਰਾ, &opts.new_branch, N_("branch"),
			   N_("create and checkout a new branch")),
		OPT_STRING('B', ਨਿਰਾ, &opts.new_branch_force, N_("branch"),
			   N_("create/reset and checkout a branch")),
		OPT_BOOL('l', ਨਿਰਾ, &opts.new_branch_log, N_("create reflog for new branch")),
		OPT_BOOL(0, "guess", &opts.dwim_new_local_branch,
			 N_("second guess 'git checkout <no-such-branch>' (default)")),
		OPT_BOOL(0, "overlay", &opts.overlay_ਬਹੁਲਕ, N_("use overlay mode (default)")),
		OPT_BOOL(0, "auto-advance", &opts.auto_advance,
			 N_("auto advance to the next file when selecting hunks interactively")),
		OPT_ਸਿਰਾ()
	};

	opts.dwim_new_local_branch = 1;
	opts.switch_branch_doing_nothing_is_ok = 1;
	opts.only_ਮਿਲਾਓ_on_switching_branches = 0;
	opts.accept_ref = 1;
	opts.accept_pathspec = 1;
	opts.implicit_detach = 1;
	opts.can_switch_when_ਵਿੱਚ_ਤਰੱਕੀ = 1;
	opts.orphan_from_ਖਾਲੀ_ਥਾਵਾਂ_ਰੁੱਖ = 0;
	opts.ਖਾਲੀ_ਥਾਵਾਂ_pathspec_ok = 1;
	opts.overlay_ਬਹੁਲਕ = -1;
	opts.checkout_ਸੂਚਕਾਂਕ = -2;    /* default on */
	opts.checkout_worktree = -2; /* default on */

	ਜੇ (argc == 3 && !strcmp(argv[1], "-b")) {
		/*
		 * User ran 'git checkout -b <branch>' and expects
		 * the same behavior as 'git switch -c <branch>'.
		 */
		opts.switch_branch_doing_nothing_is_ok = 0;
		opts.only_ਮਿਲਾਓ_on_switching_branches = 1;
	}

	options = parse_options_dup(checkout_options);
	options = ਜੋੜੋ_common_options(&opts, options);
	options = ਜੋੜੋ_common_switch_branch_options(&opts, options);
	options = ਜੋੜੋ_checkout_ਰਸਤਾ_options(&opts, options);

	ਵਾਪਸ checkout_main(argc, argv, ਅਗੇਤਰ, &opts, options,
			     CHECKOUT_CHECKOUT);
}

ਪੂਰਨ cmd_switch(ਪੂਰਨ argc,
	       ਅਚਲ ਅੱਖਰ **argv,
	       ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
	       ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ checkout_opts opts = CHECKOUT_OPTS_INIT;
	ਬਣਤਰ option *options = ਨਿਰਾ;
	ਬਣਤਰ option switch_options[] = {
		OPT_STRING('c', "create", &opts.new_branch, N_("branch"),
			   N_("create and switch to a new branch")),
		OPT_STRING('C', "force-create", &opts.new_branch_force, N_("branch"),
			   N_("create/reset and switch to a branch")),
		OPT_BOOL(0, "guess", &opts.dwim_new_local_branch,
			 N_("second guess 'git switch <no-such-branch>'")),
		OPT_BOOL(0, "discard-changes", &opts.discard_changes,
			 N_("throw away local modifications")),
		OPT_ਸਿਰਾ()
	};

	opts.dwim_new_local_branch = 1;
	opts.accept_ref = 1;
	opts.accept_pathspec = 0;
	opts.switch_branch_doing_nothing_is_ok = 0;
	opts.only_ਮਿਲਾਓ_on_switching_branches = 1;
	opts.implicit_detach = 0;
	opts.can_switch_when_ਵਿੱਚ_ਤਰੱਕੀ = 0;
	opts.orphan_from_ਖਾਲੀ_ਥਾਵਾਂ_ਰੁੱਖ = 1;
	opts.overlay_ਬਹੁਲਕ = -1;

	options = parse_options_dup(switch_options);
	options = ਜੋੜੋ_common_options(&opts, options);
	options = ਜੋੜੋ_common_switch_branch_options(&opts, options);

	cb_option = 'c';

	ਵਾਪਸ checkout_main(argc, argv, ਅਗੇਤਰ, &opts, options,
			     CHECKOUT_SWITCH);
}

ਪੂਰਨ cmd_restore(ਪੂਰਨ argc,
		ਅਚਲ ਅੱਖਰ **argv,
		ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ checkout_opts opts = CHECKOUT_OPTS_INIT;
	ਬਣਤਰ option *options;
	ਬਣਤਰ option restore_options[] = {
		OPT_STRING('s', "source", &opts.from_treeish, "<tree-ish>",
			   N_("which tree-ish to checkout from")),
		OPT_BOOL('S', "staged", &opts.checkout_ਸੂਚਕਾਂਕ,
			   N_("restore the index")),
		OPT_BOOL('W', "worktree", &opts.checkout_worktree,
			   N_("restore the working tree (default)")),
		OPT_BOOL(0, "ignore-unmerged", &opts.ignore_unmerged,
			 N_("ignore unmerged entries")),
		OPT_BOOL(0, "overlay", &opts.overlay_ਬਹੁਲਕ, N_("use overlay mode")),
		OPT_ਸਿਰਾ()
	};

	opts.accept_ref = 0;
	opts.accept_pathspec = 1;
	opts.ਖਾਲੀ_ਥਾਵਾਂ_pathspec_ok = 0;
	opts.overlay_ਬਹੁਲਕ = 0;
	opts.checkout_ਸੂਚਕਾਂਕ = -1;    /* default off */
	opts.checkout_worktree = -2; /* default on */
	opts.ignore_unmerged_opt = "--ignore-unmerged";

	options = parse_options_dup(restore_options);
	options = ਜੋੜੋ_common_options(&opts, options);
	options = ਜੋੜੋ_checkout_ਰਸਤਾ_options(&opts, options);

	ਵਾਪਸ checkout_main(argc, argv, ਅਗੇਤਰ, &opts, options,
			     CHECKOUT_RESTORE);
}
