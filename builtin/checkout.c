#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE
#ವ್ಯಾಖ್ಯಾನಿಸು DISABLE_SIGN_COMPARE_WARNINGS

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "advice.h"
#ಅನ್ತರ್ಭಾವ "branch.h"
#ಅನ್ತರ್ಭಾವ "cache-tree.h"
#ಅನ್ತರ್ಭಾವ "checkout.h"
#ಅನ್ತರ್ಭಾವ "commit.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "diff.h"
#ಅನ್ತರ್ಭಾವ "dir.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "hook.h"
#ಅನ್ತರ್ಭಾವ "merge-ll.h"
#ಅನ್ತರ್ಭಾವ "lockfile.h"
#ಅನ್ತರ್ಭಾವ "mem-pool.h"
#ಅನ್ತರ್ಭಾವ "object-file.h"
#ಅನ್ತರ್ಭಾವ "object-name.h"
#ಅನ್ತರ್ಭಾವ "odb.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "path.h"
#ಅನ್ತರ್ಭಾವ "preload-index.h"
#ಅನ್ತರ್ಭಾವ "read-cache.h"
#ಅನ್ತರ್ಭಾವ "refs.h"
#ಅನ್ತರ್ಭಾವ "remote.h"
#ಅನ್ತರ್ಭಾವ "repo-settings.h"
#ಅನ್ತರ್ಭಾವ "resolve-undo.h"
#ಅನ್ತರ್ಭಾವ "revision.h"
#ಅನ್ತರ್ಭಾವ "sequencer.h"
#ಅನ್ತರ್ಭಾವ "setup.h"
#ಅನ್ತರ್ಭಾವ "sparse-index.h"
#ಅನ್ತರ್ಭಾವ "strvec.h"
#ಅನ್ತರ್ಭಾವ "submodule.h"
#ಅನ್ತರ್ಭಾವ "symlinks.h"
#ಅನ್ತರ್ಭಾವ "trace2.h"
#ಅನ್ತರ್ಭಾವ "tree.h"
#ಅನ್ತರ್ಭಾವ "tree-walk.h"
#ಅನ್ತರ್ಭಾವ "unpack-trees.h"
#ಅನ್ತರ್ಭಾವ "wt-status.h"
#ಅನ್ತರ್ಭಾವ "xdiff-interface.h"
#ಅನ್ತರ್ಭಾವ "entry.h"
#ಅನ್ತರ್ಭಾವ "parallel-checkout.h"
#ಅನ್ತರ್ಭಾವ "add-interactive.h"

ರಚನೆ checkout_opts {
	ಪೂರ್ಣಾಂಕ patch_ಬಹುಲಕ;
	ಪೂರ್ಣಾಂಕ patch_context;
	ಪೂರ್ಣಾಂಕ patch_interhunk_context;
	ಪೂರ್ಣಾಂಕ auto_advance;
	ಪೂರ್ಣಾಂಕ quiet;
	ಪೂರ್ಣಾಂಕ ವಿಲೀನ;
	ಪೂರ್ಣಾಂಕ force;
	ಪೂರ್ಣಾಂಕ force_detach;
	ಪೂರ್ಣಾಂಕ implicit_detach;
	ಪೂರ್ಣಾಂಕ writeout_stage;
	ಪೂರ್ಣಾಂಕ overwrite_ignore;
	ಪೂರ್ಣಾಂಕ ignore_skipworktree;
	ಪೂರ್ಣಾಂಕ ignore_other_worktrees;
	ಪೂರ್ಣಾಂಕ show_ಪ್ರಗತಿ;
	ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_checkout_paths;
	ಪೂರ್ಣಾಂಕ overlay_ಬಹುಲಕ;
	ಪೂರ್ಣಾಂಕ dwim_new_local_branch;
	ಪೂರ್ಣಾಂಕ discard_changes;
	ಪೂರ್ಣಾಂಕ accept_ref;
	ಪೂರ್ಣಾಂಕ accept_pathspec;
	ಪೂರ್ಣಾಂಕ switch_branch_doing_nothing_is_ok;
	ಪೂರ್ಣಾಂಕ only_ವಿಲೀನ_on_switching_branches;
	ಪೂರ್ಣಾಂಕ can_switch_when_ಒಳ_ಪ್ರಗತಿ;
	ಪೂರ್ಣಾಂಕ orphan_from_ಖಾಲಿಗಳು_ವೃಕ್ಷ;
	ಪೂರ್ಣಾಂಕ ಖಾಲಿಗಳು_pathspec_ok;
	ಪೂರ್ಣಾಂಕ checkout_ಸೂಚ್ಯಂಕ;
	ಪೂರ್ಣಾಂಕ checkout_worktree;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ignore_unmerged_opt;
	ಪೂರ್ಣಾಂಕ ignore_unmerged;
	ಪೂರ್ಣಾಂಕ pathspec_file_nul;
	ಅಕ್ಷರ *pathspec_from_file;

	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *new_branch;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *new_branch_force;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *new_orphan_branch;
	ಪೂರ್ಣಾಂಕ new_branch_log;
	ಎಣಿಕೆ branch_track track;
	ರಚನೆ diff_options diff_options;
	ಪೂರ್ಣಾಂಕ conflict_style;

	ಪೂರ್ಣಾಂಕ branch_exists;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ;
	ರಚನೆ pathspec pathspec;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *from_treeish;
	ರಚನೆ ವೃಕ್ಷ *ಮೂಲ_ವೃಕ್ಷ;
};

#ವ್ಯಾಖ್ಯಾನಿಸು CHECKOUT_OPTS_INIT { \
	.conflict_style = -1, \
	.ವಿಲೀನ = -1, \
	.patch_context = -1, \
	.patch_interhunk_context = -1, \
	.auto_advance = 1, \
}

#ವ್ಯಾಖ್ಯಾನಿಸು ವಿಲೀನ_WORKING_ವೃಕ್ಷ_UNPACK_FAILED (-2)

ರಚನೆ branch_info {
	ಅಕ್ಷರ *name; /* The short name used */
	ಅಕ್ಷರ *ಮಾರ್ಗ; /* The full name of a real branch */
	ರಚನೆ commit *commit; /* The named commit */
	ಅಕ್ಷರ *refname; /* The full name of the ref being checked out. */
	ರಚನೆ object_id oid; /* The object ID of the commit being checked out. */
	/*
	 * if not null the branch is detached because it's already
	 * checked out in this checkout
	 */
	ಅಕ್ಷರ *checkout;
};

ಸ್ಥಿರ ಶೂನ್ಯ branch_info_release(ರಚನೆ branch_info *info)
{
	ಬಿಡುಗಡೆ(info->name);
	ಬಿಡುಗಡೆ(info->ಮಾರ್ಗ);
	ಬಿಡುಗಡೆ(info->refname);
	ಬಿಡುಗಡೆ(info->checkout);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ post_checkout_hook(ರಚನೆ commit *old_commit, ರಚನೆ commit *new_commit,
			      ಪೂರ್ಣಾಂಕ ಬದಲಾಯಿತು)
{
	ರಚನೆ ಸತತ_hooks_opt opt = ಸತತ_HOOKS_OPT_INIT_FORCE_SERIAL;

	/*
	 * "new_commit" can be NULL when checking out from the index before
	 * a commit exists.
	 */
	strvec_pushl(&opt.ವಾದಗಳು,
		     oid_ಗಮ್ಯ_hex(old_commit ? &old_commit->object.oid : null_oid(the_hash_algo)),
		     oid_ಗಮ್ಯ_hex(new_commit ? &new_commit->object.oid : null_oid(the_hash_algo)),
		     ಬದಲಾಯಿತು ? "1" : "0",
		     ಖಾಲಿ);

	ಹಿಂತಿರುಗಿ ಸತತ_hooks_opt(the_repository, "post-checkout", &opt);
}

/*
 * Handle a tree object and determine if we need to recurse into the
 * tree (READ_TREE_RECURSIVE) or skip it (0).
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ try_update_sparse_directory(ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid,
				       ರಚನೆ strbuf *ತಳ,
				       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *pathname,
				       ಪೂರ್ಣಾಂಕ overlay_ಬಹುಲಕ)
{
	ರಚನೆ strbuf dirpath = STRBUF_INIT;
	ರಚನೆ cache_ನಮೂದು *old;
	ಪೂರ್ಣಾಂಕ pos, ಫಲಿತಾಂಶ = READ_ವೃಕ್ಷ_RECURSIVE;

	ಆದರೆ (!the_repository->ಸೂಚ್ಯಂಕ->sparse_ಸೂಚ್ಯಂಕ)
		ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;

	strbuf_addbuf(&dirpath, ತಳ);
	strbuf_addstr(&dirpath, pathname);
	strbuf_addch(&dirpath, '/');

	pos = ಸೂಚ್ಯಂಕ_name_pos_sparse(the_repository->ಸೂಚ್ಯಂಕ,
				    dirpath.buf, dirpath.len);
	ಆದರೆ (pos < 0)
		ಹೋಗು cleanup;

	old = the_repository->ಸೂಚ್ಯಂಕ->cache[pos];
	ಆದರೆ (!S_ISSPARSEDIR(old->ce_ಬಹುಲಕ))
		ಹೋಗು cleanup;

	ಆದರೆ (oideq(oid, &old->oid)) {
		/* Tree content already matches; no need to descend. */
		ಫಲಿತಾಂಶ = 0;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!overlay_ಬಹುಲಕ) {
		/*
		 * In non-overlay mode (e.g., restore --staged), replace the
		 * sparse directory OID directly since files not present in
		 * the source tree should be removed anyway.
		 */
		oidcpy(&old->oid, oid);
		old->ce_flags |= CE_UPDATE;
		ಫಲಿತಾಂಶ = 0;
	}

cleanup:
	strbuf_release(&dirpath);
	ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ update_some(ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid, ರಚನೆ strbuf *ತಳ,
		       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *pathname, ಚಿಹ್ನೆರಹಿತ ಬಹುಲಕ, ಶೂನ್ಯ *context)
{
	ಪೂರ್ಣಾಂಕ len;
	ರಚನೆ cache_ನಮೂದು *ce;
	ಪೂರ್ಣಾಂಕ pos;
	ಪೂರ್ಣಾಂಕ overlay_ಬಹುಲಕ = context ? *((ಪೂರ್ಣಾಂಕ *)context) : 1;

	ಆದರೆ (S_ISDIR(ಬಹುಲಕ))
		ಹಿಂತಿರುಗಿ try_update_sparse_directory(oid, ತಳ, pathname,
						   overlay_ಬಹುಲಕ);

	len = ತಳ->len + ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(pathname);
	ce = make_ಖಾಲಿಗಳು_cache_ನಮೂದು(the_repository->ಸೂಚ್ಯಂಕ, len);
	oidcpy(&ce->oid, oid);
	ಸ್ಮರಣೆ_ನಕಲು(ce->name, ತಳ->buf, ತಳ->len);
	ಸ್ಮರಣೆ_ನಕಲು(ce->name + ತಳ->len, pathname, len - ತಳ->len);
	ce->ce_flags = create_ce_flags(0) | CE_UPDATE;
	ce->ce_namelen = len;
	ce->ce_ಬಹುಲಕ = create_ce_ಬಹುಲಕ(ಬಹುಲಕ);

	/*
	 * If the entry is the same as the current index, we can leave the old
	 * entry in place. Whether it is UPTODATE or not, checkout_entry will
	 * do the right thing.
	 */
	pos = ಸೂಚ್ಯಂಕ_name_pos_sparse(the_repository->ಸೂಚ್ಯಂಕ, ce->name, ce->ce_namelen);
	ಆದರೆ (pos >= 0) {
		ರಚನೆ cache_ನಮೂದು *old = the_repository->ಸೂಚ್ಯಂಕ->cache[pos];
		ಆದರೆ (ce->ce_ಬಹುಲಕ == old->ce_ಬಹುಲಕ &&
		    !ce_intent_ಗಮ್ಯ_ಕೂಡಿಸು(old) &&
		    oideq(&ce->oid, &old->oid)) {
			old->ce_flags |= CE_UPDATE;
			discard_cache_ನಮೂದು(ce);
			ಹಿಂತಿರುಗಿ 0;
		}
	}

	ಕೂಡಿಸು_ಸೂಚ್ಯಂಕ_ನಮೂದು(the_repository->ಸೂಚ್ಯಂಕ, ce,
			ಕೂಡಿಸು_CACHE_OK_ಗಮ್ಯ_ಕೂಡಿಸು | ಕೂಡಿಸು_CACHE_OK_ಗಮ್ಯ_REPLACE);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ read_ವೃಕ್ಷ_some(ರಚನೆ ವೃಕ್ಷ *ವೃಕ್ಷ, ಸ್ಥಿರಾಂಕ ರಚನೆ pathspec *pathspec,
			  ಪೂರ್ಣಾಂಕ overlay_ಬಹುಲಕ)
{
	read_ವೃಕ್ಷ(the_repository, ವೃಕ್ಷ,
		  pathspec, update_some, &overlay_ಬಹುಲಕ);

	/* update the index with the given tree's info
	 * for all args, expanding wildcards, and exit
	 * with any non-zero return code.
	 */
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ skip_same_name(ಸ್ಥಿರಾಂಕ ರಚನೆ cache_ನಮೂದು *ce, ಪೂರ್ಣಾಂಕ pos)
{
	ಆಗ (++pos < the_repository->ಸೂಚ್ಯಂಕ->cache_nr &&
	       !strcmp(the_repository->ಸೂಚ್ಯಂಕ->cache[pos]->name, ce->name))
		; /* skip */
	ಹಿಂತಿರುಗಿ pos;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ check_stage(ಪೂರ್ಣಾಂಕ stage, ಸ್ಥಿರಾಂಕ ರಚನೆ cache_ನಮೂದು *ce, ಪೂರ್ಣಾಂಕ pos,
		       ಪೂರ್ಣಾಂಕ overlay_ಬಹುಲಕ)
{
	ಆಗ (pos < the_repository->ಸೂಚ್ಯಂಕ->cache_nr &&
	       !strcmp(the_repository->ಸೂಚ್ಯಂಕ->cache[pos]->name, ce->name)) {
		ಆದರೆ (ce_stage(the_repository->ಸೂಚ್ಯಂಕ->cache[pos]) == stage)
			ಹಿಂತಿರುಗಿ 0;
		pos++;
	}
	ಆದರೆ (!overlay_ಬಹುಲಕ)
		ಹಿಂತಿರುಗಿ 0;
	ಆದರೆ (stage == 2)
		ಹಿಂತಿರುಗಿ error(_("path '%s' does not have our version"), ce->name);
	ಇಲ್ಲದಿದ್ದರೆ
		ಹಿಂತಿರುಗಿ error(_("path '%s' does not have their version"), ce->name);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ check_stages(ಚಿಹ್ನೆರಹಿತ stages, ಸ್ಥಿರಾಂಕ ರಚನೆ cache_ನಮೂದು *ce, ಪೂರ್ಣಾಂಕ pos)
{
	ಚಿಹ್ನೆರಹಿತ ನೋಡಿದ = 0;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name = ce->name;

	ಆಗ (pos < the_repository->ಸೂಚ್ಯಂಕ->cache_nr) {
		ce = the_repository->ಸೂಚ್ಯಂಕ->cache[pos];
		ಆದರೆ (strcmp(name, ce->name))
			ನಿಲ್ಲಿಸು;
		ನೋಡಿದ |= (1 << ce_stage(ce));
		pos++;
	}
	ಆದರೆ ((stages & ನೋಡಿದ) != stages)
		ಹಿಂತಿರುಗಿ error(_("path '%s' does not have all necessary versions"),
			     name);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ checkout_stage(ಪೂರ್ಣಾಂಕ stage, ಸ್ಥಿರಾಂಕ ರಚನೆ cache_ನಮೂದು *ce, ಪೂರ್ಣಾಂಕ pos,
			  ಸ್ಥಿರಾಂಕ ರಚನೆ checkout *state, ಪೂರ್ಣಾಂಕ *nr_checkouts,
			  ಪೂರ್ಣಾಂಕ overlay_ಬಹುಲಕ)
{
	ಆಗ (pos < the_repository->ಸೂಚ್ಯಂಕ->cache_nr &&
	       !strcmp(the_repository->ಸೂಚ್ಯಂಕ->cache[pos]->name, ce->name)) {
		ಆದರೆ (ce_stage(the_repository->ಸೂಚ್ಯಂಕ->cache[pos]) == stage)
			ಹಿಂತಿರುಗಿ checkout_ನಮೂದು(the_repository->ಸೂಚ್ಯಂಕ->cache[pos], state,
					      ಖಾಲಿ, nr_checkouts);
		pos++;
	}
	ಆದರೆ (!overlay_ಬಹುಲಕ) {
		unlink_ನಮೂದು(ce, ಖಾಲಿ);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (stage == 2)
		ಹಿಂತಿರುಗಿ error(_("path '%s' does not have our version"), ce->name);
	ಇಲ್ಲದಿದ್ದರೆ
		ಹಿಂತಿರುಗಿ error(_("path '%s' does not have their version"), ce->name);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ checkout_merged(ಪೂರ್ಣಾಂಕ pos, ಸ್ಥಿರಾಂಕ ರಚನೆ checkout *state,
			   ಪೂರ್ಣಾಂಕ *nr_checkouts, ರಚನೆ mem_pool *ce_mem_pool,
			   ಪೂರ್ಣಾಂಕ conflict_style)
{
	ರಚನೆ cache_ನಮೂದು *ce = the_repository->ಸೂಚ್ಯಂಕ->cache[pos];
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ = ce->name;
	mmfile_t ancestor, ours, theirs;
	ಎಣಿಕೆ ll_ವಿಲೀನ_ಫಲಿತಾಂಶ ವಿಲೀನ_ಸ್ಥಿತಿ;
	ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2;
	ರಚನೆ object_id oid;
	mmfile_t ಫಲಿತಾಂಶ_buf;
	ರಚನೆ object_id threeway[3];
	ಚಿಹ್ನೆರಹಿತ ಬಹುಲಕ = 0;
	ರಚನೆ ll_ವಿಲೀನ_options ll_opts = LL_ವಿಲೀನ_OPTIONS_INIT;
	ಪೂರ್ಣಾಂಕ renormalize = 0;

	memset(threeway, 0, ಗಾತ್ರ(threeway));
	ಆಗ (pos < the_repository->ಸೂಚ್ಯಂಕ->cache_nr) {
		ಪೂರ್ಣಾಂಕ stage;
		stage = ce_stage(ce);
		ಆದರೆ (!stage || strcmp(ಮಾರ್ಗ, ce->name))
			ನಿಲ್ಲಿಸು;
		oidcpy(&threeway[stage - 1], &ce->oid);
		ಆದರೆ (stage == 2)
			ಬಹುಲಕ = create_ce_ಬಹುಲಕ(ce->ce_ಬಹುಲಕ);
		pos++;
		ce = the_repository->ಸೂಚ್ಯಂಕ->cache[pos];
	}
	ಆದರೆ (is_null_oid(&threeway[1]) || is_null_oid(&threeway[2]))
		ಹಿಂತಿರುಗಿ error(_("path '%s' does not have necessary versions"), ಮಾರ್ಗ);

	read_mmblob(&ancestor, the_repository->objects, &threeway[0]);
	read_mmblob(&ours, the_repository->objects, &threeway[1]);
	read_mmblob(&theirs, the_repository->objects, &threeway[2]);

	repo_config_get_bool(the_repository, "merge.renormalize", &renormalize);
	ll_opts.renormalize = renormalize;
	ll_opts.conflict_style = conflict_style;
	ವಿಲೀನ_ಸ್ಥಿತಿ = ll_ವಿಲೀನ(&ಫಲಿತಾಂಶ_buf, ಮಾರ್ಗ, &ancestor, "base",
				&ours, "ours", &theirs, "theirs",
				state->istate, &ll_opts);
	ಬಿಡುಗಡೆ(ancestor.ptr);
	ಬಿಡುಗಡೆ(ours.ptr);
	ಬಿಡುಗಡೆ(theirs.ptr);
	ಆದರೆ (ವಿಲೀನ_ಸ್ಥಿತಿ == LL_ವಿಲೀನ_ದ್ವಿಮಾನ_CONFLICT)
		warning("Cannot merge binary files: %s (%s vs. %s)",
			ಮಾರ್ಗ, "ours", "theirs");
	ಆದರೆ (ವಿಲೀನ_ಸ್ಥಿತಿ < 0 || !ಫಲಿತಾಂಶ_buf.ptr) {
		ಬಿಡುಗಡೆ(ಫಲಿತಾಂಶ_buf.ptr);
		ಹಿಂತಿರುಗಿ error(_("path '%s': cannot merge"), ಮಾರ್ಗ);
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
	ಆದರೆ (odb_write_object(the_repository->objects, ಫಲಿತಾಂಶ_buf.ptr, ಫಲಿತಾಂಶ_buf.ಗಾತ್ರ_2, OBJ_BLOB, &oid))
		die(_("Unable to add merge result for '%s'"), ಮಾರ್ಗ);
	ಬಿಡುಗಡೆ(ಫಲಿತಾಂಶ_buf.ptr);
	ce = make_transient_cache_ನಮೂದು(ಬಹುಲಕ, &oid, ಮಾರ್ಗ, 2, ce_mem_pool);
	ಆದರೆ (!ce)
		die(_("make_cache_entry failed for path '%s'"), ಮಾರ್ಗ);
	ಸ್ಥಿತಿ_2 = checkout_ನಮೂದು(ce, state, ಖಾಲಿ, nr_checkouts);
	ಹಿಂತಿರುಗಿ ಸ್ಥಿತಿ_2;
}

ಸ್ಥಿರ ಶೂನ್ಯ mark_ce_for_checkout_overlay(ರಚನೆ cache_ನಮೂದು *ce,
					 ಅಕ್ಷರ *ps_matched,
					 ಸ್ಥಿರಾಂಕ ರಚನೆ checkout_opts *opts)
{
	ce->ce_flags &= ~CE_MATCHED;
	ಆದರೆ (!opts->ignore_skipworktree && ce_skip_worktree(ce))
		ಹಿಂತಿರುಗಿ;
	ಆದರೆ (opts->ಮೂಲ_ವೃಕ್ಷ && !(ce->ce_flags & CE_UPDATE))
		/*
		 * "git checkout tree-ish -- path", but this entry
		 * is in the original index but is not in tree-ish
		 * or does not match the pathspec; it will not be
		 * checked out to the working tree.  We will not do
		 * anything to this entry at all.
		 */
		ಹಿಂತಿರುಗಿ;
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
	ಆದರೆ (ce_ಮಾರ್ಗ_ಹೊಂದಿಕೆ(the_repository->ಸೂಚ್ಯಂಕ, ce, &opts->pathspec, ps_matched))
		ce->ce_flags |= CE_MATCHED;
}

ಸ್ಥಿರ ಶೂನ್ಯ mark_ce_for_checkout_no_overlay(ರಚನೆ cache_ನಮೂದು *ce,
					    ಅಕ್ಷರ *ps_matched,
					    ಸ್ಥಿರಾಂಕ ರಚನೆ checkout_opts *opts)
{
	ce->ce_flags &= ~CE_MATCHED;
	ಆದರೆ (!opts->ignore_skipworktree && ce_skip_worktree(ce))
		ಹಿಂತಿರುಗಿ;
	ಆದರೆ (ce_ಮಾರ್ಗ_ಹೊಂದಿಕೆ(the_repository->ಸೂಚ್ಯಂಕ, ce, &opts->pathspec, ps_matched)) {
		ce->ce_flags |= CE_MATCHED;
		ಆದರೆ (opts->ಮೂಲ_ವೃಕ್ಷ && !(ce->ce_flags & CE_UPDATE))
			/*
			 * In overlay mode, but the path is not in
			 * tree-ish, which means we should remove it
			 * from the index and the working tree.
			 */
			ce->ce_flags |= CE_REMOVE | CE_WT_REMOVE;
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ checkout_worktree(ಸ್ಥಿರಾಂಕ ರಚನೆ checkout_opts *opts,
			     ಸ್ಥಿರಾಂಕ ರಚನೆ branch_info *info)
{
	ರಚನೆ checkout state = CHECKOUT_INIT;
	ಪೂರ್ಣಾಂಕ nr_checkouts = 0, nr_unmerged = 0;
	ಪೂರ್ಣಾಂಕ errs = 0;
	ಪೂರ್ಣಾಂಕ pos;
	ಪೂರ್ಣಾಂಕ pc_workers, pc_ಮಿತಿ;
	ರಚನೆ mem_pool ce_mem_pool;

	state.force = 1;
	state.refresh_cache = 1;
	state.istate = the_repository->ಸೂಚ್ಯಂಕ;

	mem_pool_init(&ce_mem_pool, 0);
	get_parallel_checkout_configs(&pc_workers, &pc_ಮಿತಿ);
	init_checkout_metadata(&state.meta, info->refname,
			       info->commit ? &info->commit->object.oid : &info->oid,
			       ಖಾಲಿ);

	enable_delayed_checkout(&state);

	ಆದರೆ (pc_workers > 1)
		init_parallel_checkout();

	ಗಾಗಿ (pos = 0; pos < the_repository->ಸೂಚ್ಯಂಕ->cache_nr; pos++) {
		ರಚನೆ cache_ನಮೂದು *ce = the_repository->ಸೂಚ್ಯಂಕ->cache[pos];
		ಆದರೆ (ce->ce_flags & CE_MATCHED) {
			ಆದರೆ (!ce_stage(ce)) {
				errs |= checkout_ನಮೂದು(ce, &state,
						       ಖಾಲಿ, &nr_checkouts);
				ಮುಂದುವರಿ;
			}
			ಆದರೆ (opts->writeout_stage)
				errs |= checkout_stage(opts->writeout_stage,
						       ce, pos,
						       &state,
						       &nr_checkouts, opts->overlay_ಬಹುಲಕ);
			ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (opts->ವಿಲೀನ)
				errs |= checkout_merged(pos, &state,
							&nr_unmerged,
							&ce_mem_pool,
							opts->conflict_style);
			pos = skip_same_name(ce, pos) - 1;
		}
	}
	ಆದರೆ (pc_workers > 1)
		errs |= ಸತತ_parallel_checkout(&state, pc_workers, pc_ಮಿತಿ,
					      ಖಾಲಿ, ಖಾಲಿ);
	mem_pool_discard(&ce_mem_pool, should_validate_cache_entries());
	remove_marked_cache_entries(the_repository->ಸೂಚ್ಯಂಕ, 1);
	remove_scheduled_dirs();
	errs |= ಮುಕ್ತಾಯ_delayed_checkout(&state, opts->show_ಪ್ರಗತಿ);

	ಆದರೆ (opts->ಎಣಿಕೆ_checkout_paths) {
		ಆದರೆ (nr_unmerged)
			fprintf_ln(stderr, Q_("Recreated %d merge conflict",
					      "Recreated %d merge conflicts",
					      nr_unmerged),
				   nr_unmerged);
		ಆದರೆ (opts->ಮೂಲ_ವೃಕ್ಷ)
			fprintf_ln(stderr, Q_("Updated %d path from %s",
					      "Updated %d paths from %s",
					      nr_checkouts),
				   nr_checkouts,
				   repo_ಕಂಡುಹಿಡಿ_unique_abbrev(the_repository, &opts->ಮೂಲ_ವೃಕ್ಷ->object.oid,
							   DEFAULT_ABBREV));
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!nr_unmerged || nr_checkouts)
			fprintf_ln(stderr, Q_("Updated %d path from the index",
					      "Updated %d paths from the index",
					      nr_checkouts),
				   nr_checkouts);
	}

	ಹಿಂತಿರುಗಿ errs;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ checkout_paths(ಸ್ಥಿರಾಂಕ ರಚನೆ checkout_opts *opts,
			  ಸ್ಥಿರಾಂಕ ರಚನೆ branch_info *new_branch_info)
{
	ಪೂರ್ಣಾಂಕ pos;
	ಸ್ಥಿರ ಅಕ್ಷರ *ps_matched;
	ರಚನೆ object_id rev;
	ರಚನೆ commit *ತಲೆ;
	ಪೂರ್ಣಾಂಕ errs = 0;
	ರಚನೆ ಬೀಗ_file ಬೀಗ_file = ಬೀಗ_INIT;
	ಪೂರ್ಣಾಂಕ checkout_ಸೂಚ್ಯಂಕ;

	trace2_cmd_ಬಹುಲಕ(opts->patch_ಬಹುಲಕ ? "patch" : "path");

	ಆದರೆ (opts->track != BRANCH_TRACK_UNSPECIFIED)
		die(_("'%s' cannot be used with updating paths"), "--track");

	ಆದರೆ (opts->new_branch_log)
		die(_("'%s' cannot be used with updating paths"), "-l");

	ಆದರೆ (opts->ignore_unmerged && opts->patch_ಬಹುಲಕ)
		die(_("'%s' cannot be used with updating paths"),
		    opts->ignore_unmerged_opt);

	ಆದರೆ (opts->force_detach)
		die(_("'%s' cannot be used with updating paths"), "--detach");

	ಆದರೆ (opts->ವಿಲೀನ && opts->patch_ಬಹುಲಕ)
		die(_("options '%s' and '%s' cannot be used together"), "--merge", "--patch");

	ಆದರೆ (opts->ignore_unmerged && opts->ವಿಲೀನ)
		die(_("options '%s' and '%s' cannot be used together"),
		    opts->ignore_unmerged_opt, "-m");

	ಆದರೆ (opts->new_branch)
		die(_("Cannot update paths and switch to branch '%s' at the same time."),
		    opts->new_branch);

	ಆದರೆ (!opts->checkout_worktree && !opts->checkout_ಸೂಚ್ಯಂಕ)
		die(_("neither '%s' or '%s' is specified"),
		    "--staged", "--worktree");

	ಆದರೆ (!opts->checkout_worktree && !opts->from_treeish)
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
	ಆದರೆ (!opts->accept_ref && opts->checkout_ಸೂಚ್ಯಂಕ) {
		ಆದರೆ (opts->writeout_stage)
			die(_("'%s' or '%s' cannot be used with %s"),
			    "--ours", "--theirs", "--staged");

		ಆದರೆ (opts->ವಿಲೀನ)
			die(_("'%s' or '%s' cannot be used with %s"),
			    "--merge", "--conflict", "--staged");
	}

	/*
	 * recreating unmerged index entries and writing out data from
	 * unmerged index entries would make no sense when checking out
	 * of a tree-ish.
	 */
	ಆದರೆ ((opts->ವಿಲೀನ || opts->writeout_stage) && opts->ಮೂಲ_ವೃಕ್ಷ)
		die(_("'%s', '%s', or '%s' cannot be used when checking out of a tree"),
		    "--merge", "--ours", "--theirs");

	ಆದರೆ (opts->patch_ಬಹುಲಕ) {
		ಎಣಿಕೆ ಕೂಡಿಸು_p_ಬಹುಲಕ patch_ಬಹುಲಕ;
		ರಚನೆ interactive_options interactive_opts = {
			.context = opts->patch_context,
			.interhunkcontext = opts->patch_interhunk_context,
			.auto_advance = opts->auto_advance
		};
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rev = new_branch_info->name;
		ಅಕ್ಷರ rev_oid[GIT_MAX_HEXSZ + 1];

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
		ಆದರೆ (rev && new_branch_info->commit && strcmp(rev, "HEAD"))
			rev = oid_ಗಮ್ಯ_hex_r(rev_oid, &new_branch_info->commit->object.oid);

		ಆದರೆ (opts->checkout_ಸೂಚ್ಯಂಕ && opts->checkout_worktree)
			patch_ಬಹುಲಕ = ಕೂಡಿಸು_P_CHECKOUT;
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (opts->checkout_ಸೂಚ್ಯಂಕ && !opts->checkout_worktree)
			patch_ಬಹುಲಕ = ಕೂಡಿಸು_P_RESET;
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!opts->checkout_ಸೂಚ್ಯಂಕ && opts->checkout_worktree)
			patch_ಬಹುಲಕ = ಕೂಡಿಸು_P_WORKTREE;
		ಇಲ್ಲದಿದ್ದರೆ
			BUG("either flag must have been set, worktree=%d, index=%d",
			    opts->checkout_worktree, opts->checkout_ಸೂಚ್ಯಂಕ);
		ಹಿಂತಿರುಗಿ !!ಸತತ_ಕೂಡಿಸು_p(the_repository, patch_ಬಹುಲಕ, &interactive_opts,
				   rev, &opts->pathspec, 0);
	}

	repo_hold_locked_ಸೂಚ್ಯಂಕ(the_repository, &ಬೀಗ_file, ಬೀಗ_DIE_ON_ERROR);
	ಆದರೆ (repo_read_ಸೂಚ್ಯಂಕ_preload(the_repository, &opts->pathspec, 0) < 0)
		ಹಿಂತಿರುಗಿ error(_("index file corrupt"));

	ಆದರೆ (opts->ಮೂಲ_ವೃಕ್ಷ)
		read_ವೃಕ್ಷ_some(opts->ಮೂಲ_ವೃಕ್ಷ, &opts->pathspec,
			       opts->overlay_ಬಹುಲಕ);
	ಆದರೆ (opts->ವಿಲೀನ)
		unmerge_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ, &opts->pathspec, CE_MATCHED);

	ps_matched = xcalloc(opts->pathspec.nr, 1);

	/*
	 * Make sure all pathspecs participated in locating the paths
	 * to be checked out.
	 */
	ಗಾಗಿ (pos = 0; pos < the_repository->ಸೂಚ್ಯಂಕ->cache_nr; pos++)
		ಆದರೆ (opts->overlay_ಬಹುಲಕ)
			mark_ce_for_checkout_overlay(the_repository->ಸೂಚ್ಯಂಕ->cache[pos],
						     ps_matched,
						     opts);
		ಇಲ್ಲದಿದ್ದರೆ
			mark_ce_for_checkout_no_overlay(the_repository->ಸೂಚ್ಯಂಕ->cache[pos],
							ps_matched,
							opts);

	ಆದರೆ (report_ಮಾರ್ಗ_error(ps_matched, &opts->pathspec)) {
		ಬಿಡುಗಡೆ(ps_matched);
		ಹಿಂತಿರುಗಿ 1;
	}
	ಬಿಡುಗಡೆ(ps_matched);

	/* Any unmerged paths? */
	ಗಾಗಿ (pos = 0; pos < the_repository->ಸೂಚ್ಯಂಕ->cache_nr; pos++) {
		ಸ್ಥಿರಾಂಕ ರಚನೆ cache_ನಮೂದು *ce = the_repository->ಸೂಚ್ಯಂಕ->cache[pos];
		ಆದರೆ (ce->ce_flags & CE_MATCHED) {
			ಆದರೆ (!ce_stage(ce))
				ಮುಂದುವರಿ;
			ಆದರೆ (opts->ignore_unmerged) {
				ಆದರೆ (!opts->quiet)
					warning(_("path '%s' is unmerged"), ce->name);
			} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (opts->writeout_stage) {
				errs |= check_stage(opts->writeout_stage, ce, pos, opts->overlay_ಬಹುಲಕ);
			} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (opts->ವಿಲೀನ) {
				errs |= check_stages((1<<2) | (1<<3), ce, pos);
			} ಇಲ್ಲದಿದ್ದರೆ {
				errs = 1;
				error(_("path '%s' is unmerged"), ce->name);
			}
			pos = skip_same_name(ce, pos) - 1;
		}
	}
	ಆದರೆ (errs)
		ಹಿಂತಿರುಗಿ 1;

	/* Now we are committed to check them out */
	ಆದರೆ (opts->checkout_worktree)
		errs |= checkout_worktree(opts, new_branch_info);
	ಇಲ್ಲದಿದ್ದರೆ
		remove_marked_cache_entries(the_repository->ಸೂಚ್ಯಂಕ, 1);

	/*
	 * Allow updating the index when checking out from the index.
	 * This is to save new stat info.
	 */
	ಆದರೆ (opts->checkout_worktree && !opts->checkout_ಸೂಚ್ಯಂಕ && !opts->ಮೂಲ_ವೃಕ್ಷ)
		checkout_ಸೂಚ್ಯಂಕ = 1;
	ಇಲ್ಲದಿದ್ದರೆ
		checkout_ಸೂಚ್ಯಂಕ = opts->checkout_ಸೂಚ್ಯಂಕ;

	ಆದರೆ (checkout_ಸೂಚ್ಯಂಕ) {
		ಆದರೆ (write_locked_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ, &ಬೀಗ_file, COMMIT_ಬೀಗ))
			die(_("unable to write new index file"));
	} ಇಲ್ಲದಿದ್ದರೆ {
		/*
		 * NEEDSWORK: if --worktree is not specified, we
		 * should save stat info of checked out files in the
		 * index to avoid the next (potentially costly)
		 * refresh. But it's a bit tricker to do...
		 */
		rollback_ಬೀಗ_file(&ಬೀಗ_file);
	}

	refs_read_ref_ತುಂಬಿದ(get_main_ref_store(the_repository), "HEAD", 0,
			   &rev, ಖಾಲಿ);
	ತಲೆ = lookup_commit_reference_gently(the_repository, &rev, 1);

	errs |= post_checkout_hook(ತಲೆ, ತಲೆ, 0);
	ಹಿಂತಿರುಗಿ errs;
}

ಸ್ಥಿರ ಶೂನ್ಯ show_local_changes(ರಚನೆ object *ತಲೆ,
			       ಸ್ಥಿರಾಂಕ ರಚನೆ diff_options *opts)
{
	ರಚನೆ rev_info rev;
	/* I think we want full paths, even if we're in a subdirectory. */
	repo_init_revisions(the_repository, &rev, ಖಾಲಿ);
	rev.diffopt.flags = opts->flags;
	rev.diffopt.output_format |= DIFF_FORMAT_NAME_ಸ್ಥಿತಿ;
	rev.diffopt.flags.recursive = 1;
	diff_ಸಿದ್ಧತೆ_ಮುಗಿದ(&rev.diffopt);
	ಕೂಡಿಸು_pending_object(&rev, ತಲೆ, ಖಾಲಿ);
	ಸತತ_diff_ಸೂಚ್ಯಂಕ(&rev, 0);
	release_revisions(&rev);
}

ಸ್ಥಿರ ಶೂನ್ಯ describe_detached_ತಲೆ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ, ರಚನೆ commit *commit)
{
	ರಚನೆ strbuf sb = STRBUF_INIT;

	ಆದರೆ (!repo_parse_commit(the_repository, commit))
		pp_commit_easy(CMIT_FMT_ONELINE, commit, &sb);
	ಆದರೆ (print_sha1_ellipsis()) {
		fprintf(stderr, "%s %s... %s\n", ಸಂದೇಶ,
			repo_ಕಂಡುಹಿಡಿ_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV),
			sb.buf);
	} ಇಲ್ಲದಿದ್ದರೆ {
		fprintf(stderr, "%s %s %s\n", ಸಂದೇಶ,
			repo_ಕಂಡುಹಿಡಿ_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV),
			sb.buf);
	}
	strbuf_release(&sb);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ reset_ವೃಕ್ಷ(ರಚನೆ ವೃಕ್ಷ *ವೃಕ್ಷ, ಸ್ಥಿರಾಂಕ ರಚನೆ checkout_opts *o,
		      ಪೂರ್ಣಾಂಕ worktree, ಪೂರ್ಣಾಂಕ *writeout_error,
		      ರಚನೆ branch_info *info)
{
	ರಚನೆ unpack_trees_options opts;
	ರಚನೆ ವೃಕ್ಷ_desc ವೃಕ್ಷ_desc;

	memset(&opts, 0, ಗಾತ್ರ(opts));
	opts.ತಲೆ_idx = -1;
	opts.update = worktree;
	opts.skip_unmerged = !worktree;
	opts.reset = o->force ? UNPACK_RESET_OVERWRITE_UNTRACKED :
				UNPACK_RESET_PROTECT_UNTRACKED;
	opts.preserve_ignored = (!o->force && !o->overwrite_ignore);
	opts.ವಿಲೀನ = 1;
	opts.fn = oneway_ವಿಲೀನ;
	opts.verbose_update = o->show_ಪ್ರಗತಿ;
	opts.src_ಸೂಚ್ಯಂಕ = the_repository->ಸೂಚ್ಯಂಕ;
	opts.dst_ಸೂಚ್ಯಂಕ = the_repository->ಸೂಚ್ಯಂಕ;
	init_checkout_metadata(&opts.meta, info->refname,
			       info->commit ? &info->commit->object.oid : null_oid(the_hash_algo),
			       ಖಾಲಿ);
	ಆದರೆ (repo_parse_ವೃಕ್ಷ(the_repository, ವೃಕ್ಷ) < 0)
		ಹಿಂತಿರುಗಿ 128;
	init_ವೃಕ್ಷ_desc(&ವೃಕ್ಷ_desc, &ವೃಕ್ಷ->object.oid, ವೃಕ್ಷ->ಬಫರ್, ವೃಕ್ಷ->ಗಾತ್ರ_2);
	ಆಯ್ಕೆ (unpack_trees(1, &ವೃಕ್ಷ_desc, &opts)) {
	ಸ್ಥಿತಿ -2:
		*writeout_error = 1;
		/*
		 * We return 0 nevertheless, as the index is all right
		 * and more importantly we have made best efforts to
		 * update paths in the work tree, and we cannot revert
		 * them.
		 */
		/* fallthrough */
	ಸ್ಥಿತಿ 0:
		ಹಿಂತಿರುಗಿ 0;
	ಪೂರ್ವನಿಯೋಜಿತ:
		ಹಿಂತಿರುಗಿ 128;
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ ಸಿದ್ಧತೆ_branch_ಮಾರ್ಗ(ರಚನೆ branch_info *branch)
{
	ರಚನೆ strbuf buf = STRBUF_INIT;

	/*
	 * If this is a ref, resolve it; otherwise, look up the OID for our
	 * expression.  Failure here is okay.
	 */
	ಆದರೆ (!repo_dwim_ref(the_repository, branch->name, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(branch->name),
			   &branch->oid, &branch->refname, 0))
		repo_get_oid_committish(the_repository, branch->name, &branch->oid);

	copy_branchname(the_repository, &buf, branch->name, INTERPRET_BRANCH_LOCAL);
	ಆದರೆ (strcmp(buf.buf, branch->name)) {
		ಬಿಡುಗಡೆ(branch->name);
		branch->name = xstrdup(buf.buf);
	}
	strbuf_splice(&buf, 0, 0, "refs/heads/", 11);
	ಬಿಡುಗಡೆ(branch->ಮಾರ್ಗ);
	branch->ಮಾರ್ಗ = strbuf_detach(&buf, ಖಾಲಿ);
}

ಸ್ಥಿರ ಶೂನ್ಯ init_topts(ರಚನೆ unpack_trees_options *topts,
		       ಪೂರ್ಣಾಂಕ show_ಪ್ರಗತಿ, ಪೂರ್ಣಾಂಕ overwrite_ignore,
		       bool quiet)
{
	memset(topts, 0, ಗಾತ್ರ(*topts));
	topts->ತಲೆ_idx = -1;
	topts->src_ಸೂಚ್ಯಂಕ = the_repository->ಸೂಚ್ಯಂಕ;
	topts->dst_ಸೂಚ್ಯಂಕ = the_repository->ಸೂಚ್ಯಂಕ;

	ಸಿದ್ಧತೆ_unpack_trees_porcelain(topts, "checkout");

	topts->initial_checkout = is_ಸೂಚ್ಯಂಕ_unborn(the_repository->ಸೂಚ್ಯಂಕ);
	topts->update = 1;
	topts->ವಿಲೀನ = 1;
	topts->quiet = quiet;
	topts->verbose_update = show_ಪ್ರಗತಿ;
	topts->fn = twoway_ವಿಲೀನ;
	topts->preserve_ignored = !overwrite_ignore;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ವಿಲೀನ_working_ವೃಕ್ಷ(ಸ್ಥಿರಾಂಕ ರಚನೆ checkout_opts *opts,
			      ರಚನೆ branch_info *old_branch_info,
			      ರಚನೆ branch_info *new_branch_info,
			      bool quiet,
			      ಪೂರ್ಣಾಂಕ *writeout_error)
{
	ಪೂರ್ಣಾಂಕ ret;
	ರಚನೆ ಬೀಗ_file ಬೀಗ_file = ಬೀಗ_INIT;
	ರಚನೆ ವೃಕ್ಷ *new_ವೃಕ್ಷ;

	repo_hold_locked_ಸೂಚ್ಯಂಕ(the_repository, &ಬೀಗ_file, ಬೀಗ_DIE_ON_ERROR);
	ಆದರೆ (repo_read_ಸೂಚ್ಯಂಕ_preload(the_repository, ಖಾಲಿ, 0) < 0) {
		rollback_ಬೀಗ_file(&ಬೀಗ_file);
		ಹಿಂತಿರುಗಿ error(_("index file corrupt"));
	}

	resolve_undo_clear_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ);
	ಆದರೆ (opts->new_orphan_branch && opts->orphan_from_ಖಾಲಿಗಳು_ವೃಕ್ಷ) {
		ಆದರೆ (new_branch_info->commit)
			BUG("'switch --orphan' should never accept a commit as starting point");
		new_ವೃಕ್ಷ = repo_parse_ವೃಕ್ಷ_ಪರೋಕ್ಷ(the_repository,
						    the_hash_algo->ಖಾಲಿಗಳು_ವೃಕ್ಷ);
		ಆದರೆ (!new_ವೃಕ್ಷ)
			BUG("unable to read empty tree");
	} ಇಲ್ಲದಿದ್ದರೆ {
		new_ವೃಕ್ಷ = repo_get_commit_ವೃಕ್ಷ(the_repository,
						new_branch_info->commit);
		ಆದರೆ (!new_ವೃಕ್ಷ) {
			rollback_ಬೀಗ_file(&ಬೀಗ_file);
			ಹಿಂತಿರುಗಿ error(_("unable to read tree (%s)"),
				     oid_ಗಮ್ಯ_hex(&new_branch_info->commit->object.oid));
		}
	}
	ಆದರೆ (opts->discard_changes) {
		ret = reset_ವೃಕ್ಷ(new_ವೃಕ್ಷ, opts, 1, writeout_error, new_branch_info);
		ಆದರೆ (ret) {
			rollback_ಬೀಗ_file(&ಬೀಗ_file);
			ಹಿಂತಿರುಗಿ ret;
		}
	} ಇಲ್ಲದಿದ್ದರೆ {
		ರಚನೆ ವೃಕ್ಷ_desc trees[2];
		ರಚನೆ ವೃಕ್ಷ *ವೃಕ್ಷ;
		ರಚನೆ unpack_trees_options topts;
		ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *old_commit_oid;

		refresh_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ, REFRESH_QUIET, ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ);

		ಆದರೆ (unmerged_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ)) {
			rollback_ಬೀಗ_file(&ಬೀಗ_file);
			error(_("you need to resolve your current index first"));
			ಹಿಂತಿರುಗಿ 1;
		}

		/* 2-way merge to the new branch */
		init_topts(&topts, opts->show_ಪ್ರಗತಿ,
			   opts->overwrite_ignore, quiet);
		init_checkout_metadata(&topts.meta, new_branch_info->refname,
				       new_branch_info->commit ?
				       &new_branch_info->commit->object.oid :
				       &new_branch_info->oid, ಖಾಲಿ);

		old_commit_oid = old_branch_info->commit ?
			&old_branch_info->commit->object.oid :
			the_hash_algo->ಖಾಲಿಗಳು_ವೃಕ್ಷ;
		ವೃಕ್ಷ = repo_parse_ವೃಕ್ಷ_ಪರೋಕ್ಷ(the_repository,
						old_commit_oid);
		ಆದರೆ (!ವೃಕ್ಷ)
			die(_("unable to parse commit %s"),
				oid_ಗಮ್ಯ_hex(old_commit_oid));

		init_ವೃಕ್ಷ_desc(&trees[0], &ವೃಕ್ಷ->object.oid,
			       ವೃಕ್ಷ->ಬಫರ್, ವೃಕ್ಷ->ಗಾತ್ರ_2);
		ಆದರೆ (repo_parse_ವೃಕ್ಷ(the_repository, new_ವೃಕ್ಷ) < 0)
			die(ಖಾಲಿ);
		ವೃಕ್ಷ = new_ವೃಕ್ಷ;
		init_ವೃಕ್ಷ_desc(&trees[1], &ವೃಕ್ಷ->object.oid,
			       ವೃಕ್ಷ->ಬಫರ್, ವೃಕ್ಷ->ಗಾತ್ರ_2);

		ret = unpack_trees(2, trees, &topts);
		clear_unpack_trees_porcelain(&topts);
		ಆದರೆ (ret == -1) {
			rollback_ಬೀಗ_file(&ಬೀಗ_file);
			ಹಿಂತಿರುಗಿ ವಿಲೀನ_WORKING_ವೃಕ್ಷ_UNPACK_FAILED;
		}
	}

	ಆದರೆ (!cache_ವೃಕ್ಷ_fully_valid(the_repository->ಸೂಚ್ಯಂಕ))
		cache_ವೃಕ್ಷ_update(the_repository->ಸೂಚ್ಯಂಕ, WRITE_ವೃಕ್ಷ_SILENT | WRITE_ವೃಕ್ಷ_REPAIR);

	ಆದರೆ (write_locked_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ, &ಬೀಗ_file, COMMIT_ಬೀಗ))
		die(_("unable to write new index file"));

	ಆದರೆ (!opts->discard_changes && !opts->quiet && new_branch_info->commit)
		show_local_changes(&new_branch_info->commit->object, &opts->diff_options);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ report_tracking(ರಚನೆ branch_info *new_branch_info)
{
	ರಚನೆ strbuf sb = STRBUF_INIT;
	ರಚನೆ branch *branch = branch_get(new_branch_info->name);

	ಆದರೆ (!format_tracking_info(branch, &sb, AHEAD_BEHIND_ತುಂಬಿದ, 1))
		ಹಿಂತಿರುಗಿ;
	fputs(sb.buf, stdout);
	strbuf_release(&sb);
}

ಸ್ಥಿರ ಶೂನ್ಯ update_refs_for_switch(ಸ್ಥಿರಾಂಕ ರಚನೆ checkout_opts *opts,
				   ರಚನೆ branch_info *old_branch_info,
				   ರಚನೆ branch_info *new_branch_info)
{
	ರಚನೆ strbuf ಸಂದೇಶ = STRBUF_INIT;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *old_desc, *reflog_ಸಂದೇಶ;
	ಆದರೆ (opts->new_branch) {
		ಆದರೆ (opts->new_orphan_branch) {
			ಎಣಿಕೆ log_refs_config log_all_ref_updates = LOG_REFS_UNSET;
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ;
			ಅಕ್ಷರ *refname;

			ಆದರೆ (!repo_config_get_string_tmp(the_repository, "core.logallrefupdates", &ಮೌಲ್ಯ))
				log_all_ref_updates = refs_parse_log_all_ref_updates_config(ಮೌಲ್ಯ);

			refname = mkpathdup("refs/heads/%s", opts->new_orphan_branch);
			ಆದರೆ (opts->new_branch_log &&
			    !should_autocreate_reflog(log_all_ref_updates, refname)) {
				ಪೂರ್ಣಾಂಕ ret;
				ರಚನೆ strbuf err = STRBUF_INIT;

				ret = refs_create_reflog(get_main_ref_store(the_repository),
							 refname, &err);
				ಆದರೆ (ret) {
					fprintf(stderr, _("Can not do reflog for '%s': %s\n"),
						opts->new_orphan_branch, err.buf);
					strbuf_release(&err);
					ಬಿಡುಗಡೆ(refname);
					ಹಿಂತಿರುಗಿ;
				}
				strbuf_release(&err);
			}
			ಬಿಡುಗಡೆ(refname);
		}
		ಇಲ್ಲದಿದ್ದರೆ
			create_branch(the_repository,
				      opts->new_branch, new_branch_info->name,
				      opts->new_branch_force ? 1 : 0,
				      opts->new_branch_force ? 1 : 0,
				      opts->new_branch_log,
				      opts->quiet,
				      opts->track,
				      0);
		ಬಿಡುಗಡೆ(new_branch_info->name);
		ಬಿಡುಗಡೆ(new_branch_info->refname);
		new_branch_info->name = xstrdup(opts->new_branch);
		ಸಿದ್ಧತೆ_branch_ಮಾರ್ಗ(new_branch_info);
	}

	old_desc = old_branch_info->name;
	ಆದರೆ (!old_desc && old_branch_info->commit)
		old_desc = oid_ಗಮ್ಯ_hex(&old_branch_info->commit->object.oid);

	reflog_ಸಂದೇಶ = getenv("GIT_REFLOG_ACTION");
	ಆದರೆ (!reflog_ಸಂದೇಶ)
		strbuf_addf(&ಸಂದೇಶ, "checkout: moving from %s to %s",
			old_desc ? old_desc : "(invalid)", new_branch_info->name);
	ಇಲ್ಲದಿದ್ದರೆ
		strbuf_insertstr(&ಸಂದೇಶ, 0, reflog_ಸಂದೇಶ);

	ಆದರೆ (!strcmp(new_branch_info->name, "HEAD") && !new_branch_info->ಮಾರ್ಗ && !opts->force_detach) {
		/* Nothing to do. */
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (opts->force_detach || !new_branch_info->ಮಾರ್ಗ) {	/* No longer on any branch. */
		refs_update_ref(get_main_ref_store(the_repository), ಸಂದೇಶ.buf,
				"HEAD", &new_branch_info->commit->object.oid,
				ಖಾಲಿ,
				REF_NO_DEREF, UPDATE_REFS_DIE_ON_ERR);
		ಆದರೆ (!opts->quiet) {
			ಆದರೆ (old_branch_info->ಮಾರ್ಗ &&
			    advice_enabled(ADVICE_DETACHED_ತಲೆ) && !opts->force_detach)
				detach_advice(new_branch_info->name);
			describe_detached_ತಲೆ(_("HEAD is now at"), new_branch_info->commit);
		}
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (new_branch_info->ಮಾರ್ಗ) {	/* Switch branches. */
		ಆದರೆ (refs_update_symref(get_main_ref_store(the_repository), "HEAD", new_branch_info->ಮಾರ್ಗ, ಸಂದೇಶ.buf) < 0)
			die(_("unable to update HEAD"));
		ಆದರೆ (!opts->quiet) {
			ಆದರೆ (old_branch_info->ಮಾರ್ಗ && !strcmp(new_branch_info->ಮಾರ್ಗ, old_branch_info->ಮಾರ್ಗ)) {
				ಆದರೆ (opts->new_branch_force)
					fprintf(stderr, _("Reset branch '%s'\n"),
						new_branch_info->name);
				ಇಲ್ಲದಿದ್ದರೆ
					fprintf(stderr, _("Already on '%s'\n"),
						new_branch_info->name);
			} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (opts->new_branch) {
				ಆದರೆ (opts->branch_exists)
					fprintf(stderr, _("Switched to and reset branch '%s'\n"), new_branch_info->name);
				ಇಲ್ಲದಿದ್ದರೆ
					fprintf(stderr, _("Switched to a new branch '%s'\n"), new_branch_info->name);
			} ಇಲ್ಲದಿದ್ದರೆ {
				fprintf(stderr, _("Switched to branch '%s'\n"),
					new_branch_info->name);
			}
		}
		ಆದರೆ (old_branch_info->ಮಾರ್ಗ && old_branch_info->name) {
			ಆದರೆ (!refs_ref_exists(get_main_ref_store(the_repository), old_branch_info->ಮಾರ್ಗ) && refs_reflog_exists(get_main_ref_store(the_repository), old_branch_info->ಮಾರ್ಗ))
				refs_delete_reflog(get_main_ref_store(the_repository),
						   old_branch_info->ಮಾರ್ಗ);
		}
	}
	remove_branch_state(the_repository, !opts->quiet);
	strbuf_release(&ಸಂದೇಶ);
	ಆದರೆ (!opts->quiet &&
	    !opts->force_detach &&
	    (new_branch_info->ಮಾರ್ಗ || !strcmp(new_branch_info->name, "HEAD")))
		report_tracking(new_branch_info);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಕೂಡಿಸು_pending_uninteresting_ref(ಸ್ಥಿರಾಂಕ ರಚನೆ reference *ref, ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ)
{
	ಕೂಡಿಸು_pending_oid(cb_ದತ್ತಾಂಶ, ref->name, ref->oid, UNINTERESTING);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ describe_one_orphan(ರಚನೆ strbuf *sb, ರಚನೆ commit *commit)
{
	strbuf_addstr(sb, "  ");
	strbuf_ಕೂಡಿಸು_unique_abbrev(sb, &commit->object.oid, DEFAULT_ABBREV);
	strbuf_addch(sb, ' ');
	ಆದರೆ (!repo_parse_commit(the_repository, commit))
		pp_commit_easy(CMIT_FMT_ONELINE, commit, sb);
	strbuf_addch(sb, '\n');
}

#ವ್ಯಾಖ್ಯಾನಿಸು ORPHAN_CUTOFF 4
ಸ್ಥಿರ ಶೂನ್ಯ suggest_reattach(ರಚನೆ commit *commit, ರಚನೆ rev_info *revs)
{
	ರಚನೆ commit *c, *ಕೊನೆಯ = ಖಾಲಿ;
	ರಚನೆ strbuf sb = STRBUF_INIT;
	ಪೂರ್ಣಾಂಕ ಕಳೆದುಹೋದ = 0;
	ಆಗ ((c = get_revision(revs)) != ಖಾಲಿ) {
		ಆದರೆ (ಕಳೆದುಹೋದ < ORPHAN_CUTOFF)
			describe_one_orphan(&sb, c);
		ಕೊನೆಯ = c;
		ಕಳೆದುಹೋದ++;
	}
	ಆದರೆ (ORPHAN_CUTOFF < ಕಳೆದುಹೋದ) {
		ಪೂರ್ಣಾಂಕ ಇನ್ನಷ್ಟು = ಕಳೆದುಹೋದ - ORPHAN_CUTOFF;
		ಆದರೆ (ಇನ್ನಷ್ಟು == 1)
			describe_one_orphan(&sb, ಕೊನೆಯ);
		ಇಲ್ಲದಿದ್ದರೆ
			strbuf_addf(&sb, _(" ... and %d more.\n"), ಇನ್ನಷ್ಟು);
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
		ಕಳೆದುಹೋದ),
		ಕಳೆದುಹೋದ,
		sb.buf);
	strbuf_release(&sb);

	ಆದರೆ (advice_enabled(ADVICE_DETACHED_ತಲೆ))
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
			ಕಳೆದುಹೋದ),
			repo_ಕಂಡುಹಿಡಿ_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV));
}

/*
 * We are about to leave commit that was at the tip of a detached
 * HEAD.  If it is not reachable from any ref, this is the last chance
 * for the user to do so without resorting to reflog.
 */
ಸ್ಥಿರ ಶೂನ್ಯ orphaned_commit_warning(ರಚನೆ commit *old_commit, ರಚನೆ commit *new_commit)
{
	ರಚನೆ rev_info revs;
	ರಚನೆ object *object = &old_commit->object;

	repo_init_revisions(the_repository, &revs, ಖಾಲಿ);
	ಸಿದ್ಧತೆ_revisions(0, ಖಾಲಿ, &revs, ಖಾಲಿ);

	object->flags &= ~UNINTERESTING;
	ಕೂಡಿಸು_pending_object(&revs, object, oid_ಗಮ್ಯ_hex(&object->oid));

	refs_for_each_ref(get_main_ref_store(the_repository),
			  ಕೂಡಿಸು_pending_uninteresting_ref, &revs);
	ಆದರೆ (new_commit)
		ಕೂಡಿಸು_pending_oid(&revs, "HEAD",
				&new_commit->object.oid,
				UNINTERESTING);

	ಆದರೆ (prepare_revision_walk(&revs))
		die(_("internal error in revision walk"));
	ಆದರೆ (!(old_commit->object.flags & UNINTERESTING))
		suggest_reattach(old_commit, &revs);
	ಇಲ್ಲದಿದ್ದರೆ
		describe_detached_ತಲೆ(_("Previous HEAD position was"), old_commit);

	/* Clean up objects used, as they will be reused. */
	repo_clear_commit_marks(the_repository, ALL_REV_FLAGS);
	release_revisions(&revs);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ switch_branches(ಸ್ಥಿರಾಂಕ ರಚನೆ checkout_opts *opts,
			   ರಚನೆ branch_info *new_branch_info)
{
	ಪೂರ್ಣಾಂಕ ret = 0;
	ರಚನೆ branch_info old_branch_info = { 0 };
	ರಚನೆ object_id rev;
	ಪೂರ್ಣಾಂಕ flag, writeout_error = 0;
	ಪೂರ್ಣಾಂಕ do_ವಿಲೀನ = 1;
	ಪೂರ್ಣಾಂಕ created_autostash = 0;
	ಎಣಿಕೆ stash_apply_ಫಲಿತಾಂಶ autostash_res = STASH_APPLY_CLEAN;
	ರಚನೆ strbuf old_commit_shortname = STRBUF_INIT;
	ರಚನೆ strbuf autostash_ಸಂದೇಶ = STRBUF_INIT;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *stash_label_ತಳ = ಖಾಲಿ;

	trace2_cmd_ಬಹುಲಕ("branch");

	memset(&old_branch_info, 0, ಗಾತ್ರ(old_branch_info));
	old_branch_info.ಮಾರ್ಗ = refs_resolve_refdup(get_main_ref_store(the_repository),
						   "HEAD", 0, &rev, &flag);
	ಆದರೆ (old_branch_info.ಮಾರ್ಗ)
		old_branch_info.commit = lookup_commit_reference_gently(the_repository, &rev, 1);
	ಆದರೆ (!(flag & REF_ISSYMREF))
		FREE_AND_NULL(old_branch_info.ಮಾರ್ಗ);

	ಆದರೆ (old_branch_info.ಮಾರ್ಗ) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ ಪೂರ್ವಪ್ರತ್ಯಯ = "refs/heads/";
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *p;
		ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(old_branch_info.ಮಾರ್ಗ, ಪೂರ್ವಪ್ರತ್ಯಯ, &p))
			old_branch_info.name = xstrdup(p);
	}

	ಆದರೆ (opts->new_orphan_branch && opts->orphan_from_ಖಾಲಿಗಳು_ವೃಕ್ಷ) {
		ಆದರೆ (new_branch_info->name)
			BUG("'switch --orphan' should never accept a commit as starting point");
		new_branch_info->commit = ಖಾಲಿ;
		new_branch_info->name = xstrdup("(empty)");
		do_ವಿಲೀನ = 1;
	}

	ಆದರೆ (!new_branch_info->name) {
		new_branch_info->name = xstrdup("HEAD");
		new_branch_info->commit = old_branch_info.commit;
		ಆದರೆ (!new_branch_info->commit)
			die(_("You are on a branch yet to be born"));
		parse_commit_or_die(new_branch_info->commit);

		ಆದರೆ (opts->only_ವಿಲೀನ_on_switching_branches)
			do_ವಿಲೀನ = 0;
	}

	ಆದರೆ (old_branch_info.name) {
		stash_label_ತಳ = old_branch_info.name;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (old_branch_info.commit) {
		strbuf_ಕೂಡಿಸು_unique_abbrev(&old_commit_shortname,
					 &old_branch_info.commit->object.oid,
					 DEFAULT_ABBREV);
		stash_label_ತಳ = old_commit_shortname.buf;
	}

	ಆದರೆ (do_ವಿಲೀನ) {
		ret = ವಿಲೀನ_working_ವೃಕ್ಷ(opts, &old_branch_info, new_branch_info,
					 opts->ವಿಲೀನ, &writeout_error);
		ಆದರೆ (ret == ವಿಲೀನ_WORKING_ವೃಕ್ಷ_UNPACK_FAILED && opts->ವಿಲೀನ) {
			strbuf_addf(&autostash_ಸಂದೇಶ,
				    "autostash while switching to '%s'",
				    new_branch_info->name);
			create_autostash_ref(the_repository,
					     "CHECKOUT_AUTOSTASH_HEAD",
					     autostash_ಸಂದೇಶ.buf, true);
			created_autostash = 1;
			ret = ವಿಲೀನ_working_ವೃಕ್ಷ(opts, &old_branch_info, new_branch_info,
						 false, &writeout_error);
		}
		ಆದರೆ (created_autostash) {
			ಆದರೆ (opts->conflict_style >= 0) {
				ರಚನೆ strbuf cfg = STRBUF_INIT;
				strbuf_addf(&cfg, "merge.conflictStyle=%s",
					    conflict_style_name(opts->conflict_style));
				git_config_ತಳ್ಳು_parameter(cfg.buf);
				strbuf_release(&cfg);
			}
			autostash_res = apply_autostash_ref(the_repository,
				    "CHECKOUT_AUTOSTASH_HEAD",
				    new_branch_info->name,
				    "local",
				    stash_label_ತಳ,
				    autostash_ಸಂದೇಶ.buf);
		}
		ಆದರೆ (ret) {
			branch_info_release(&old_branch_info);
			strbuf_release(&old_commit_shortname);
			strbuf_release(&autostash_ಸಂದೇಶ);
			ಹಿಂತಿರುಗಿ ret < 0 ? 1 : ret;
		}
	}

	ಆದರೆ (!opts->quiet && !old_branch_info.ಮಾರ್ಗ && old_branch_info.commit && new_branch_info->commit != old_branch_info.commit)
		orphaned_commit_warning(old_branch_info.commit, new_branch_info->commit);

	ಆದರೆ (autostash_res == STASH_APPLY_CONFLICT && !opts->quiet)
		fputc('\n', stderr);
	update_refs_for_switch(opts, &old_branch_info, new_branch_info);

	ಆದರೆ (created_autostash) {
		discard_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ);
		ಆದರೆ (repo_read_ಸೂಚ್ಯಂಕ(the_repository) < 0)
			die(_("index file corrupt"));

		ಆದರೆ (!opts->quiet && new_branch_info->commit) {
			ಬರೆ(_("The following paths have local changes:\n"));
			show_local_changes(&new_branch_info->commit->object,
					   &opts->diff_options);
		}
	}

	ret = post_checkout_hook(old_branch_info.commit, new_branch_info->commit, 1);
	branch_info_release(&old_branch_info);
	strbuf_release(&old_commit_shortname);
	strbuf_release(&autostash_ಸಂದೇಶ);

	ಹಿಂತಿರುಗಿ ret || writeout_error;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ git_checkout_config(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *var, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ,
			       ಸ್ಥಿರಾಂಕ ರಚನೆ config_context *ctx, ಶೂನ್ಯ *cb)
{
	ರಚನೆ checkout_opts *opts = cb;

	ಆದರೆ (!strcmp(var, "diff.ignoresubmodules")) {
		ಆದರೆ (!ಮೌಲ್ಯ)
			ಹಿಂತಿರುಗಿ config_error_nonbool(var);
		handle_ignore_submodules_arg(&opts->diff_options, ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(var, "checkout.guess")) {
		opts->dwim_new_local_branch = git_config_bool(var, ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (starts_with(var, "submodule."))
		ಹಿಂತಿರುಗಿ git_default_submodule_config(var, ಮೌಲ್ಯ, ಖಾಲಿ);

	ಹಿಂತಿರುಗಿ git_xmerge_config(var, ಮೌಲ್ಯ, ctx, ಖಾಲಿ);
}

ಸ್ಥಿರ ಶೂನ್ಯ ಸಿದ್ಧತೆ_new_branch_info_and_ಮೂಲ_ವೃಕ್ಷ(
	ರಚನೆ branch_info *new_branch_info,
	ರಚನೆ checkout_opts *opts,
	ರಚನೆ object_id *rev,
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg)
{
	ರಚನೆ ವೃಕ್ಷ **ಮೂಲ_ವೃಕ್ಷ = &opts->ಮೂಲ_ವೃಕ್ಷ;
	ರಚನೆ object_id branch_rev;

	/* treat '@' as a shortcut for 'HEAD' */
	new_branch_info->name = !strcmp(arg, "@") ? xstrdup("HEAD") :
						    xstrdup(arg);
	ಸಿದ್ಧತೆ_branch_ಮಾರ್ಗ(new_branch_info);

	ಆದರೆ (!check_refname_format(new_branch_info->ಮಾರ್ಗ, 0) &&
	    !refs_read_ref(get_main_ref_store(the_repository), new_branch_info->ಮಾರ್ಗ, &branch_rev))
		oidcpy(rev, &branch_rev);
	ಇಲ್ಲದಿದ್ದರೆ
		/* not an existing branch */
		FREE_AND_NULL(new_branch_info->ಮಾರ್ಗ);

	new_branch_info->commit = lookup_commit_reference_gently(the_repository, rev, 1);
	ಆದರೆ (!new_branch_info->commit) {
		/* not a commit */
		*ಮೂಲ_ವೃಕ್ಷ = repo_parse_ವೃಕ್ಷ_ಪರೋಕ್ಷ(the_repository, rev);
		ಆದರೆ (!*ಮೂಲ_ವೃಕ್ಷ)
			die(_("unable to read tree (%s)"), oid_ಗಮ್ಯ_hex(rev));
	} ಇಲ್ಲದಿದ್ದರೆ {
		parse_commit_or_die(new_branch_info->commit);
		*ಮೂಲ_ವೃಕ್ಷ = repo_get_commit_ವೃಕ್ಷ(the_repository,
						    new_branch_info->commit);
		ಆದರೆ (!*ಮೂಲ_ವೃಕ್ಷ)
			die(_("unable to read tree (%s)"),
			    oid_ಗಮ್ಯ_hex(&new_branch_info->commit->object.oid));
	}
}


ಎಣಿಕೆ checkout_command {
	CHECKOUT_CHECKOUT = 1,
	CHECKOUT_SWITCH = 2,
	CHECKOUT_RESTORE = 3,
};

ಸ್ಥಿರ ಶೂನ್ಯ advise_disambiguating_remotes(ಎಣಿಕೆ checkout_command which_command,
					  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *branch,
					  ಸ್ಥಿರಾಂಕ ರಚನೆ string_list *matched_remote_ಹೆಸರುಗಳು)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cmdname;
	ರಚನೆ string_list_ಅಂಶ *ಅಂಶ;

	ಆಯ್ಕೆ (which_command) {
	ಸ್ಥಿತಿ CHECKOUT_CHECKOUT:
		cmdname = "checkout";
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ CHECKOUT_SWITCH:
		cmdname = "switch";
		ನಿಲ್ಲಿಸು;
	ಪೂರ್ವನಿಯೋಜಿತ:
		BUG("command <%d> should not reach advise_disambiguating_remotes",
		    which_command);
		ನಿಲ್ಲಿಸು;
	}

	advise(_("Branch name '%s' appears in multiple remotes:"), branch);
	for_each_string_list_ಅಂಶ(ಅಂಶ, matched_remote_ಹೆಸರುಗಳು) {
		advise(_("  %s"), ಅಂಶ->string);
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

ಸ್ಥಿರ ಅಕ್ಷರ *parse_remote_branch(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg,
				 ರಚನೆ object_id *rev,
				 ಪೂರ್ಣಾಂಕ could_be_checkout_paths,
				 ಎಣಿಕೆ checkout_command which_command)
{
	ಪೂರ್ಣಾಂಕ ಅಂಕೆ_matches = 0;
	ರಚನೆ string_list matched_remote_ಹೆಸರುಗಳು = STRING_LIST_INIT_DUP;

	ಅಕ್ಷರ *remote = unique_tracking_name(arg, rev, &ಅಂಕೆ_matches,
					    &matched_remote_ಹೆಸರುಗಳು);

	ಆದರೆ (remote && could_be_checkout_paths) {
		die(_("'%s' could be both a local file and a tracking branch.\n"
			"Please use -- (and optionally --no-guess) to disambiguate"),
		    arg);
	}

	ಆದರೆ (!remote && ಅಂಕೆ_matches > 1) {
		ಆದರೆ (advice_enabled(ADVICE_CHECKOUT_AMBIGUOUS_REMOTE_BRANCH_NAME))
			advise_disambiguating_remotes(which_command, arg,
						      &matched_remote_ಹೆಸರುಗಳು);
		die(_("'%s' matched multiple (%d) remote tracking branches"),
		    arg, ಅಂಕೆ_matches);
	}

	string_list_clear(&matched_remote_ಹೆಸರುಗಳು, 0);

	ಹಿಂತಿರುಗಿ remote;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ parse_branchname_arg(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
				ಪೂರ್ಣಾಂಕ dwim_new_local_branch_ok,
				ಎಣಿಕೆ checkout_command which_command,
				ರಚನೆ branch_info *new_branch_info,
				ರಚನೆ checkout_opts *opts,
				ರಚನೆ object_id *rev)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **new_branch = &opts->new_branch;
	ಪೂರ್ಣಾಂಕ argcount = 0;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg;
	ಅಕ್ಷರ *remote = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ dash_dash_pos;
	ಪೂರ್ಣಾಂಕ has_dash_dash = 0;
	ಪೂರ್ಣಾಂಕ i;

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
	ಆದರೆ (!argc)
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (!opts->accept_pathspec) {
		ಆದರೆ (argc > 1)
			die(_("only one reference expected"));
		has_dash_dash = 1; /* helps disambiguate */
	}

	arg = argv[0];
	dash_dash_pos = -1;
	ಗಾಗಿ (i = 0; i < argc; i++) {
		ಆದರೆ (opts->accept_pathspec && !strcmp(argv[i], "--")) {
			dash_dash_pos = i;
			ನಿಲ್ಲಿಸು;
		}
	}
	ಆದರೆ (dash_dash_pos == 0)
		ಹಿಂತಿರುಗಿ 1; /* case (2) */
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (dash_dash_pos == 1)
		has_dash_dash = 1; /* case (3) or (1) */
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (dash_dash_pos >= 2)
		die(_("only one reference expected, %d given."), dash_dash_pos);
	opts->ಎಣಿಕೆ_checkout_paths = !opts->quiet && !has_dash_dash;

	ಆದರೆ (!strcmp(arg, "-"))
		arg = "@{-1}";

	ಆದರೆ (repo_get_oid_mb(the_repository, arg, rev)) {
		/*
		 * Either case (3) or (4), with <something> not being
		 * a commit, or an attempt to use case (1) with an
		 * invalid ref.
		 *
		 * It's likely an error, but we need to find out if
		 * we should auto-create the branch, case (3).(b).
		 */
		ಪೂರ್ಣಾಂಕ recover_with_dwim = dwim_new_local_branch_ok;

		ಪೂರ್ಣಾಂಕ could_be_checkout_paths = !has_dash_dash &&
			check_filename(opts->ಪೂರ್ವಪ್ರತ್ಯಯ, arg);

		ಆದರೆ (!has_dash_dash && !no_wildcard(arg))
			recover_with_dwim = 0;

		/*
		 * Accept "git checkout foo", "git checkout foo --"
		 * and "git switch foo" as candidates for dwim.
		 */
		ಆದರೆ (!(argc == 1 && !has_dash_dash) &&
		    !(argc == 2 && has_dash_dash) &&
		    opts->accept_pathspec)
			recover_with_dwim = 0;

		ಆದರೆ (recover_with_dwim) {
			remote = parse_remote_branch(arg, rev,
						     could_be_checkout_paths,
						     which_command);
			ಆದರೆ (remote) {
				*new_branch = arg;
				arg = remote;
				/* DWIMmed to create local branch, case (3).(b) */
			} ಇಲ್ಲದಿದ್ದರೆ {
				recover_with_dwim = 0;
			}
		}

		ಆದರೆ (!recover_with_dwim) {
			ಆದರೆ (has_dash_dash)
				die(_("invalid reference: %s"), arg);
			ಹಿಂತಿರುಗಿ argcount;
		}
	}

	/* we can't end up being in (2) anymore, eat the argument */
	argcount++;
	argv++;
	argc--;

	ಸಿದ್ಧತೆ_new_branch_info_and_ಮೂಲ_ವೃಕ್ಷ(new_branch_info, opts, rev, arg);

	ಆದರೆ (!opts->ಮೂಲ_ವೃಕ್ಷ)                   /* case (1): want a tree */
		die(_("reference is not a tree: %s"), arg);

	ಆದರೆ (!has_dash_dash) {	/* case (3).(d) -> (1) */
		/*
		 * Do not complain the most common case
		 *	git checkout branch
		 * even if there happen to be a file called 'branch';
		 * it would be extremely annoying.
		 */
		ಆದರೆ (argc)
			verify_non_filename(the_repository, opts->ಪೂರ್ವಪ್ರತ್ಯಯ, arg);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (opts->accept_pathspec) {
		argcount++;
		argv++;
		argc--;
	}

	ಬಿಡುಗಡೆ(remote);
	ಹಿಂತಿರುಗಿ argcount;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ switch_unborn_ಗಮ್ಯ_new_branch(ಸ್ಥಿರಾಂಕ ರಚನೆ checkout_opts *opts)
{
	ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2;
	ರಚನೆ strbuf branch_ref = STRBUF_INIT;

	trace2_cmd_ಬಹುಲಕ("unborn");

	ಆದರೆ (!opts->new_branch)
		die(_("You are on a branch yet to be born"));
	strbuf_addf(&branch_ref, "refs/heads/%s", opts->new_branch);
	ಸ್ಥಿತಿ_2 = refs_update_symref(get_main_ref_store(the_repository),
				    "HEAD", branch_ref.buf, "checkout -b");
	strbuf_release(&branch_ref);
	ಆದರೆ (!opts->quiet)
		fprintf(stderr, _("Switched to a new branch '%s'\n"),
			opts->new_branch);
	ಹಿಂತಿರುಗಿ ಸ್ಥಿತಿ_2;
}

ಸ್ಥಿರ ಶೂನ್ಯ die_expecting_a_branch(ಸ್ಥಿರಾಂಕ ರಚನೆ branch_info *branch_info)
{
	ರಚನೆ object_id oid;
	ಅಕ್ಷರ *ಗಮ್ಯ_free;
	ಪೂರ್ಣಾಂಕ ಸಂಕೇತ_2;

	ಆದರೆ (repo_dwim_ref(the_repository, branch_info->name,
			  ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(branch_info->name), &oid, &ಗಮ್ಯ_free, 0) == 1) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ref = ಗಮ್ಯ_free;

		ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(ref, "refs/tags/", &ref))
			ಸಂಕೇತ_2 = die_ಸಂದೇಶ(_("a branch is expected, got tag '%s'"), ref);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(ref, "refs/remotes/", &ref))
			ಸಂಕೇತ_2 = die_ಸಂದೇಶ(_("a branch is expected, got remote branch '%s'"), ref);
		ಇಲ್ಲದಿದ್ದರೆ
			ಸಂಕೇತ_2 = die_ಸಂದೇಶ(_("a branch is expected, got '%s'"), ref);
	}
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (branch_info->commit)
		ಸಂಕೇತ_2 = die_ಸಂದೇಶ(_("a branch is expected, got commit '%s'"), branch_info->name);
	ಇಲ್ಲದಿದ್ದರೆ
		/*
		 * This case should never happen because we already die() on
		 * non-commit, but just in case.
		 */
		ಸಂಕೇತ_2 = die_ಸಂದೇಶ(_("a branch is expected, got '%s'"), branch_info->name);

	ಆದರೆ (advice_enabled(ADVICE_SUGGEST_DETACHING_ತಲೆ))
		advise(_("If you want to detach HEAD at the commit, try again with the --detach option."));

	ನಿರ್ಗಮಿಸು(ಸಂಕೇತ_2);
}

ಸ್ಥಿರ ಶೂನ್ಯ die_if_some_operation_ಒಳ_ಪ್ರಗತಿ(ಶೂನ್ಯ)
{
	ರಚನೆ wt_ಸ್ಥಿತಿ_state state;

	memset(&state, 0, ಗಾತ್ರ(state));
	wt_ಸ್ಥಿತಿ_get_state(the_repository, &state, 0);

	ಆದರೆ (state.ವಿಲೀನ_ಒಳ_ಪ್ರಗತಿ)
		die(_("cannot switch branch while merging\n"
		      "Consider \"git merge --quit\" "
		      "or \"git worktree add\"."));
	ಆದರೆ (state.am_ಒಳ_ಪ್ರಗತಿ)
		die(_("cannot switch branch in the middle of an am session\n"
		      "Consider \"git am --quit\" "
		      "or \"git worktree add\"."));
	ಆದರೆ (state.rebase_interactive_ಒಳ_ಪ್ರಗತಿ || state.rebase_ಒಳ_ಪ್ರಗತಿ)
		die(_("cannot switch branch while rebasing\n"
		      "Consider \"git rebase --quit\" "
		      "or \"git worktree add\"."));
	ಆದರೆ (state.cherry_pick_ಒಳ_ಪ್ರಗತಿ)
		die(_("cannot switch branch while cherry-picking\n"
		      "Consider \"git cherry-pick --quit\" "
		      "or \"git worktree add\"."));
	ಆದರೆ (state.revert_ಒಳ_ಪ್ರಗತಿ)
		die(_("cannot switch branch while reverting\n"
		      "Consider \"git revert --quit\" "
		      "or \"git worktree add\"."));
	ಆದರೆ (state.bisect_ಒಳ_ಪ್ರಗತಿ)
		warning(_("you are switching branch while bisecting"));

	wt_ಸ್ಥಿತಿ_state_free_buffers(&state);
}

/*
 * die if attempting to checkout an existing branch that is in use
 * in another worktree, unless ignore-other-wortrees option is given.
 * The check is bypassed when the branch is already the current one,
 * as it will not make things any worse.
 */
ಸ್ಥಿರ ಶೂನ್ಯ die_if_switching_ಗಮ್ಯ_a_branch_ಒಳ_use(ರಚನೆ checkout_opts *opts,
						ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ತುಂಬಿದ_ref)
{
	ಪೂರ್ಣಾಂಕ flags;
	ಅಕ್ಷರ *ತಲೆ_ref;

	ಆದರೆ (opts->ignore_other_worktrees)
		ಹಿಂತಿರುಗಿ;
	ತಲೆ_ref = refs_resolve_refdup(get_main_ref_store(the_repository),
				       "HEAD", 0, ಖಾಲಿ, &flags);
	ಆದರೆ (ತಲೆ_ref && (!(flags & REF_ISSYMREF) || strcmp(ತಲೆ_ref, ತುಂಬಿದ_ref)))
		die_if_checked_ಹೊರಹೋಗು(ತುಂಬಿದ_ref, 1);
	ಬಿಡುಗಡೆ(ತಲೆ_ref);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ checkout_branch(ರಚನೆ checkout_opts *opts,
			   ರಚನೆ branch_info *new_branch_info)
{
	ರಚನೆ repo_config_ಮೌಲ್ಯಗಳು *cfg = repo_config_ಮೌಲ್ಯಗಳು(the_repository);
	ಪೂರ್ಣಾಂಕ noop_switch = (!new_branch_info->name &&
			   !opts->new_branch &&
			   !opts->force_detach);

	ಆದರೆ (opts->pathspec.nr)
		die(_("paths cannot be used with switching branches"));

	ಆದರೆ (opts->patch_ಬಹುಲಕ)
		die(_("'%s' cannot be used with switching branches"),
		    "--patch");

	ಆದರೆ (opts->overlay_ಬಹುಲಕ != -1)
		die(_("'%s' cannot be used with switching branches"),
		    "--[no]-overlay");

	ಆದರೆ (opts->writeout_stage) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ;
		ಆದರೆ (noop_switch)
			ಸಂದೇಶ = _("'%s' needs the paths to check out");
		ಇಲ್ಲದಿದ್ದರೆ
			ಸಂದೇಶ = _("'%s' cannot be used with switching branches");
		die(ಸಂದೇಶ, "--ours/--theirs");
	}

	ಆದರೆ (opts->force && opts->ವಿಲೀನ)
		die(_("'%s' cannot be used with '%s'"), "-f", "-m");

	ಆದರೆ (opts->discard_changes && opts->ವಿಲೀನ)
		die(_("'%s' cannot be used with '%s'"), "--discard-changes", "--merge");

	ಆದರೆ (opts->force_detach && opts->new_branch)
		die(_("'%s' cannot be used with '%s'"),
		    "--detach", "-b/-B/--orphan");

	ಆದರೆ (opts->new_orphan_branch) {
		ಆದರೆ (opts->track != BRANCH_TRACK_UNSPECIFIED)
			die(_("'%s' cannot be used with '%s'"), "--orphan", "-t");
		ಆದರೆ (opts->orphan_from_ಖಾಲಿಗಳು_ವೃಕ್ಷ && new_branch_info->name)
			die(_("'%s' cannot take <start-point>"), "--orphan");
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (opts->force_detach) {
		ಆದರೆ (opts->track != BRANCH_TRACK_UNSPECIFIED)
			die(_("'%s' cannot be used with '%s'"), "--detach", "-t");
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (opts->track == BRANCH_TRACK_UNSPECIFIED)
		opts->track = cfg->branch_track;

	ಆದರೆ (new_branch_info->name && !new_branch_info->commit)
		die(_("Cannot switch branch to a non-commit '%s'"),
		    new_branch_info->name);

	ಆದರೆ (noop_switch &&
	    !opts->switch_branch_doing_nothing_is_ok)
		die(_("missing branch or commit argument"));

	ಆದರೆ (!opts->implicit_detach &&
	    !opts->force_detach &&
	    !opts->new_branch &&
	    !opts->new_branch_force &&
	    new_branch_info->name &&
	    !new_branch_info->ಮಾರ್ಗ)
		die_expecting_a_branch(new_branch_info);

	ಆದರೆ (!opts->can_switch_when_ಒಳ_ಪ್ರಗತಿ)
		die_if_some_operation_ಒಳ_ಪ್ರಗತಿ();

	/* "git checkout <branch>" */
	ಆದರೆ (new_branch_info->ಮಾರ್ಗ && !opts->force_detach && !opts->new_branch)
		die_if_switching_ಗಮ್ಯ_a_branch_ಒಳ_use(opts, new_branch_info->ಮಾರ್ಗ);

	/* "git checkout -B <branch>" */
	ಆದರೆ (opts->new_branch_force) {
		ಅಕ್ಷರ *ತುಂಬಿದ_ref = xstrfmt("refs/heads/%s", opts->new_branch);
		die_if_switching_ಗಮ್ಯ_a_branch_ಒಳ_use(opts, ತುಂಬಿದ_ref);
		ಬಿಡುಗಡೆ(ತುಂಬಿದ_ref);
	}

	ಆದರೆ (!new_branch_info->commit && opts->new_branch) {
		ರಚನೆ object_id rev;
		ಪೂರ್ಣಾಂಕ flag;

		ಆದರೆ (!refs_read_ref_ತುಂಬಿದ(get_main_ref_store(the_repository), "HEAD", 0, &rev, &flag) &&
		    (flag & REF_ISSYMREF) && is_null_oid(&rev))
			ಹಿಂತಿರುಗಿ switch_unborn_ಗಮ್ಯ_new_branch(opts);
	}
	ಹಿಂತಿರುಗಿ switch_branches(opts, new_branch_info);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ parse_opt_conflict(ಸ್ಥಿರಾಂಕ ರಚನೆ option *o, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ರಚನೆ checkout_opts *opts = o->ಮೌಲ್ಯ;

	ಆದರೆ (unset) {
		opts->conflict_style = -1;
		ಹಿಂತಿರುಗಿ 0;
	}
	opts->conflict_style = parse_conflict_style_name(arg);
	ಆದರೆ (opts->conflict_style < 0)
		ಹಿಂತಿರುಗಿ error(_("unknown conflict style '%s'"), arg);
	/* --conflict overrides a previous --no-merge */
	ಆದರೆ (!opts->ವಿಲೀನ)
		opts->ವಿಲೀನ = -1;

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ರಚನೆ option *ಕೂಡಿಸು_common_options(ರಚನೆ checkout_opts *opts,
					 ರಚನೆ option *prevopts)
{
	ರಚನೆ option options[] = {
		OPT__QUIET(&opts->quiet, N_("suppress progress reporting")),
		OPT_CALLBACK_F(0, "recurse-submodules", ಖಾಲಿ,
			    "checkout", "control recursive updating of submodules",
			    PARSE_OPT_OPTARG, option_parse_recurse_submodules_worktree_updater),
		OPT_BOOL(0, "progress", &opts->show_ಪ್ರಗತಿ, N_("force progress reporting")),
		OPT_BOOL('m', "merge", &opts->ವಿಲೀನ, N_("perform a 3-way merge with the new branch")),
		OPT_CALLBACK(0, "conflict", opts, N_("style"),
			     N_("conflict style (merge, diff3, or zdiff3)"),
			     parse_opt_conflict),
		OPT_ಕೊನೆ()
	};
	ರಚನೆ option *newopts = parse_options_concat(prevopts, options);
	ಬಿಡುಗಡೆ(prevopts);
	ಹಿಂತಿರುಗಿ newopts;
}

ಸ್ಥಿರ ರಚನೆ option *ಕೂಡಿಸು_common_switch_branch_options(
	ರಚನೆ checkout_opts *opts, ರಚನೆ option *prevopts)
{
	ರಚನೆ option options[] = {
		OPT_BOOL('d', "detach", &opts->force_detach, N_("detach HEAD at named commit")),
		OPT_CALLBACK_F('t', "track",  &opts->track, "(direct|inherit)",
			N_("set branch tracking configuration"),
			PARSE_OPT_OPTARG,
			parse_opt_tracking_ಬಹುಲಕ),
		OPT__FORCE(&opts->force, N_("force checkout (throw away local modifications)"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_STRING(0, "orphan", &opts->new_orphan_branch, N_("new-branch"), N_("new unborn branch")),
		OPT_BOOL_F(0, "overwrite-ignore", &opts->overwrite_ignore,
			   N_("update ignored files (default)"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "ignore-other-worktrees", &opts->ignore_other_worktrees,
			 N_("do not check if another worktree is using this branch")),
		OPT_ಕೊನೆ()
	};
	ರಚನೆ option *newopts = parse_options_concat(prevopts, options);
	ಬಿಡುಗಡೆ(prevopts);
	ಹಿಂತಿರುಗಿ newopts;
}

ಸ್ಥಿರ ರಚನೆ option *ಕೂಡಿಸು_checkout_ಮಾರ್ಗ_options(ರಚನೆ checkout_opts *opts,
						ರಚನೆ option *prevopts)
{
	ರಚನೆ option options[] = {
		OPT_ಗಣ_INT_F('2', "ours", &opts->writeout_stage,
			      N_("checkout our version for unmerged files"),
			      2, PARSE_OPT_NONEG),
		OPT_ಗಣ_INT_F('3', "theirs", &opts->writeout_stage,
			      N_("checkout their version for unmerged files"),
			      3, PARSE_OPT_NONEG),
		OPT_BOOL('p', "patch", &opts->patch_ಬಹುಲಕ, N_("select hunks interactively")),
		OPT_DIFF_UNIFIED(&opts->patch_context),
		OPT_DIFF_INTERHUNK_CONTEXT(&opts->patch_interhunk_context),
		OPT_BOOL(0, "ignore-skip-worktree-bits", &opts->ignore_skipworktree,
			 N_("do not limit pathspecs to sparse entries only")),
		OPT_PATHSPEC_FROM_FILE(&opts->pathspec_from_file),
		OPT_PATHSPEC_FILE_NUL(&opts->pathspec_file_nul),
		OPT_ಕೊನೆ()
	};
	ರಚನೆ option *newopts = parse_options_concat(prevopts, options);
	ಬಿಡುಗಡೆ(prevopts);
	ಹಿಂತಿರುಗಿ newopts;
}

/* create-branch option (either b or c) */
ಸ್ಥಿರ ಅಕ್ಷರ cb_option = 'b';

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ checkout_main(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			 ರಚನೆ checkout_opts *opts, ರಚನೆ option *options,
			 ಎಣಿಕೆ checkout_command which_command)
{
	ಪೂರ್ಣಾಂಕ parseopt_flags = 0;
	ರಚನೆ branch_info new_branch_info = { 0 };
	ಪೂರ್ಣಾಂಕ ret;

	ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ checkout_usage[] = {
		N_("git checkout [<options>] <branch>"),
		N_("git checkout [<options>] [<branch>] -- <file>..."),
		ಖಾಲಿ,
	};

	ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ switch_branch_usage[] = {
		N_("git switch [<options>] [<branch>]"),
		ಖಾಲಿ,
	};

	ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ restore_usage[] = {
		N_("git restore [<options>] [--source=<branch>] <file>..."),
		ಖಾಲಿ,
	};

	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ *usagestr;

	ಆಯ್ಕೆ (which_command) {
	ಸ್ಥಿತಿ CHECKOUT_CHECKOUT:
		usagestr = checkout_usage;
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ CHECKOUT_SWITCH:
		usagestr = switch_branch_usage;
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ CHECKOUT_RESTORE:
		usagestr = restore_usage;
		ನಿಲ್ಲಿಸು;
	ಪೂರ್ವನಿಯೋಜಿತ:
		BUG("no such checkout variant %d", which_command);
	}

	opts->overwrite_ignore = 1;
	opts->ಪೂರ್ವಪ್ರತ್ಯಯ = ಪೂರ್ವಪ್ರತ್ಯಯ;
	opts->show_ಪ್ರಗತಿ = -1;

	repo_config(the_repository, git_checkout_config, opts);
	ಆದರೆ (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_ತುಂಬಿದ_ಸೂಚ್ಯಂಕ = 0;
	}

	opts->track = BRANCH_TRACK_UNSPECIFIED;

	ಆದರೆ (!opts->accept_pathspec && !opts->accept_ref)
		BUG("make up your mind, you need to take _something_");
	ಆದರೆ (opts->accept_pathspec && opts->accept_ref)
		parseopt_flags = PARSE_OPT_KEEP_DASHDASH;

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options,
			     usagestr, parseopt_flags);

	ಆದರೆ (opts->patch_context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	ಆದರೆ (opts->patch_interhunk_context < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	ಆದರೆ (!opts->patch_ಬಹುಲಕ) {
		ಆದರೆ (opts->patch_context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--patch");
		ಆದರೆ (opts->patch_interhunk_context != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--patch");
		ಆದರೆ (!opts->auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--patch");
	}

	ಆದರೆ (opts->show_ಪ್ರಗತಿ < 0) {
		ಆದರೆ (opts->quiet)
			opts->show_ಪ್ರಗತಿ = 0;
		ಇಲ್ಲದಿದ್ದರೆ
			opts->show_ಪ್ರಗತಿ = isatty(2);
	}

	/* --conflicts implies --merge */
	ಆದರೆ (opts->ವಿಲೀನ == -1)
		opts->ವಿಲೀನ = opts->conflict_style >= 0;

	ಆದರೆ (opts->force) {
		opts->discard_changes = 1;
		opts->ignore_unmerged_opt = "--force";
		opts->ignore_unmerged = 1;
	}

	ಆದರೆ ((!!opts->new_branch + !!opts->new_branch_force + !!opts->new_orphan_branch) > 1)
		die(_("options '-%c', '-%c', and '%s' cannot be used together"),
			cb_option, ದೊಡ್ಡ_ಅಕ್ಷರ(cb_option), "--orphan");

	ಆದರೆ (opts->overlay_ಬಹುಲಕ == 1 && opts->patch_ಬಹುಲಕ)
		die(_("options '%s' and '%s' cannot be used together"), "-p", "--overlay");

	ಆದರೆ (opts->checkout_ಸೂಚ್ಯಂಕ >= 0 || opts->checkout_worktree >= 0) {
		ಆದರೆ (opts->checkout_ಸೂಚ್ಯಂಕ < 0)
			opts->checkout_ಸೂಚ್ಯಂಕ = 0;
		ಆದರೆ (opts->checkout_worktree < 0)
			opts->checkout_worktree = 0;
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಆದರೆ (opts->checkout_ಸೂಚ್ಯಂಕ < 0)
			opts->checkout_ಸೂಚ್ಯಂಕ = -opts->checkout_ಸೂಚ್ಯಂಕ - 1;
		ಆದರೆ (opts->checkout_worktree < 0)
			opts->checkout_worktree = -opts->checkout_worktree - 1;
	}
	ಆದರೆ (opts->checkout_ಸೂಚ್ಯಂಕ < 0 || opts->checkout_worktree < 0)
		BUG("these flags should be non-negative by now");
	/*
	 * convenient shortcut: "git restore --staged [--worktree]" equals
	 * "git restore --staged [--worktree] --source HEAD"
	 */
	ಆದರೆ (!opts->from_treeish && opts->checkout_ಸೂಚ್ಯಂಕ)
		opts->from_treeish = "HEAD";

	/*
	 * From here on, new_branch will contain the branch to be checked out,
	 * and new_branch_force and new_orphan_branch will tell us which one of
	 * -b/-B/-c/-C/--orphan is being used.
	 */
	ಆದರೆ (opts->new_branch_force)
		opts->new_branch = opts->new_branch_force;

	ಆದರೆ (opts->new_orphan_branch)
		opts->new_branch = opts->new_orphan_branch;

	/* --track without -c/-C/-b/-B/--orphan should DWIM */
	ಆದರೆ (opts->track != BRANCH_TRACK_UNSPECIFIED && !opts->new_branch) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *argv0 = argv[0];
		ಆದರೆ (!argc || !strcmp(argv0, "--"))
			die(_("--track needs a branch name"));
		skip_ಪೂರ್ವಪ್ರತ್ಯಯ(argv0, "refs/", &argv0);
		skip_ಪೂರ್ವಪ್ರತ್ಯಯ(argv0, "remotes/", &argv0);
		argv0 = strchr(argv0, '/');
		ಆದರೆ (!argv0 || !argv0[1])
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
	ಆದರೆ (argc && opts->accept_ref) {
		ರಚನೆ object_id rev;
		ಪೂರ್ಣಾಂಕ dwim_ok =
			!opts->patch_ಬಹುಲಕ &&
			opts->dwim_new_local_branch &&
			opts->track == BRANCH_TRACK_UNSPECIFIED &&
			!opts->new_branch;
		ಪೂರ್ಣಾಂಕ n = parse_branchname_arg(argc, argv, dwim_ok, which_command,
					     &new_branch_info, opts, &rev);
		argv += n;
		argc -= n;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!opts->accept_ref && opts->from_treeish) {
		ರಚನೆ object_id rev;

		ಆದರೆ (repo_get_oid_mb(the_repository, opts->from_treeish, &rev))
			die(_("could not resolve '%s'"), opts->from_treeish);

		ಸಿದ್ಧತೆ_new_branch_info_and_ಮೂಲ_ವೃಕ್ಷ(&new_branch_info,
						      opts, &rev,
						      opts->from_treeish);

		ಆದರೆ (!opts->ಮೂಲ_ವೃಕ್ಷ)
			die(_("reference is not a tree: %s"), opts->from_treeish);
	}

	ಆದರೆ (argc) {
		parse_pathspec(&opts->pathspec, 0,
			       opts->patch_ಬಹುಲಕ ? PATHSPEC_ಪೂರ್ವಪ್ರತ್ಯಯ_ORIGIN : 0,
			       ಪೂರ್ವಪ್ರತ್ಯಯ, argv);

		ಆದರೆ (!opts->pathspec.nr)
			die(_("invalid path specification"));

		/*
		 * Try to give more helpful suggestion.
		 * new_branch && argc > 1 will be caught later.
		 */
		ಆದರೆ (opts->new_branch && argc == 1 && !new_branch_info.commit)
			die(_("'%s' is not a commit and a branch '%s' cannot be created from it"),
				argv[0], opts->new_branch);

		ಆದರೆ (opts->force_detach)
			die(_("git checkout: --detach does not take a path argument '%s'"),
			    argv[0]);
	}

	ಆದರೆ (opts->pathspec_from_file) {
		ಆದರೆ (opts->pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		ಆದರೆ (opts->force_detach)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--detach");

		ಆದರೆ (opts->patch_ಬಹುಲಕ)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--patch");

		parse_pathspec_file(&opts->pathspec, 0,
				    0,
				    ಪೂರ್ವಪ್ರತ್ಯಯ, opts->pathspec_from_file, opts->pathspec_file_nul);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (opts->pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	opts->pathspec.recursive = 1;

	ಆದರೆ (opts->pathspec.nr) {
		ಆದರೆ (1 < !!opts->writeout_stage + !!opts->force + !!opts->ವಿಲೀನ)
			die(_("git checkout: --ours/--theirs, --force and --merge are incompatible when\n"
			      "checking out of the index."));
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಆದರೆ (opts->accept_pathspec && !opts->ಖಾಲಿಗಳು_pathspec_ok &&
		    !opts->patch_ಬಹುಲಕ)	/* patch mode is special */
			die(_("you must specify path(s) to restore"));
	}

	ಆದರೆ (opts->new_branch) {
		ರಚನೆ strbuf buf = STRBUF_INIT;

		ಆದರೆ (opts->new_branch_force)
			opts->branch_exists = validate_branchname(opts->new_branch, &buf);
		ಇಲ್ಲದಿದ್ದರೆ
			opts->branch_exists =
				validate_new_branchname(opts->new_branch, &buf, 0);
		strbuf_release(&buf);
	}

	ಆದರೆ (opts->patch_ಬಹುಲಕ || opts->pathspec.nr)
		ret = checkout_paths(opts, &new_branch_info);
	ಇಲ್ಲದಿದ್ದರೆ
		ret = checkout_branch(opts, &new_branch_info);

	branch_info_release(&new_branch_info);
	clear_pathspec(&opts->pathspec);
	ಬಿಡುಗಡೆ(opts->pathspec_from_file);
	ಬಿಡುಗಡೆ(options);

	ಹಿಂತಿರುಗಿ ret;
}

ಪೂರ್ಣಾಂಕ cmd_checkout(ಪೂರ್ಣಾಂಕ argc,
		 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
		 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		 ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ checkout_opts opts = CHECKOUT_OPTS_INIT;
	ರಚನೆ option *options;
	ರಚನೆ option checkout_options[] = {
		OPT_STRING('b', ಖಾಲಿ, &opts.new_branch, N_("branch"),
			   N_("create and checkout a new branch")),
		OPT_STRING('B', ಖಾಲಿ, &opts.new_branch_force, N_("branch"),
			   N_("create/reset and checkout a branch")),
		OPT_BOOL('l', ಖಾಲಿ, &opts.new_branch_log, N_("create reflog for new branch")),
		OPT_BOOL(0, "guess", &opts.dwim_new_local_branch,
			 N_("second guess 'git checkout <no-such-branch>' (default)")),
		OPT_BOOL(0, "overlay", &opts.overlay_ಬಹುಲಕ, N_("use overlay mode (default)")),
		OPT_BOOL(0, "auto-advance", &opts.auto_advance,
			 N_("auto advance to the next file when selecting hunks interactively")),
		OPT_ಕೊನೆ()
	};

	opts.dwim_new_local_branch = 1;
	opts.switch_branch_doing_nothing_is_ok = 1;
	opts.only_ವಿಲೀನ_on_switching_branches = 0;
	opts.accept_ref = 1;
	opts.accept_pathspec = 1;
	opts.implicit_detach = 1;
	opts.can_switch_when_ಒಳ_ಪ್ರಗತಿ = 1;
	opts.orphan_from_ಖಾಲಿಗಳು_ವೃಕ್ಷ = 0;
	opts.ಖಾಲಿಗಳು_pathspec_ok = 1;
	opts.overlay_ಬಹುಲಕ = -1;
	opts.checkout_ಸೂಚ್ಯಂಕ = -2;    /* default on */
	opts.checkout_worktree = -2; /* default on */

	ಆದರೆ (argc == 3 && !strcmp(argv[1], "-b")) {
		/*
		 * User ran 'git checkout -b <branch>' and expects
		 * the same behavior as 'git switch -c <branch>'.
		 */
		opts.switch_branch_doing_nothing_is_ok = 0;
		opts.only_ವಿಲೀನ_on_switching_branches = 1;
	}

	options = parse_options_dup(checkout_options);
	options = ಕೂಡಿಸು_common_options(&opts, options);
	options = ಕೂಡಿಸು_common_switch_branch_options(&opts, options);
	options = ಕೂಡಿಸು_checkout_ಮಾರ್ಗ_options(&opts, options);

	ಹಿಂತಿರುಗಿ checkout_main(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, &opts, options,
			     CHECKOUT_CHECKOUT);
}

ಪೂರ್ಣಾಂಕ cmd_switch(ಪೂರ್ಣಾಂಕ argc,
	       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
	       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
	       ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ checkout_opts opts = CHECKOUT_OPTS_INIT;
	ರಚನೆ option *options = ಖಾಲಿ;
	ರಚನೆ option switch_options[] = {
		OPT_STRING('c', "create", &opts.new_branch, N_("branch"),
			   N_("create and switch to a new branch")),
		OPT_STRING('C', "force-create", &opts.new_branch_force, N_("branch"),
			   N_("create/reset and switch to a branch")),
		OPT_BOOL(0, "guess", &opts.dwim_new_local_branch,
			 N_("second guess 'git switch <no-such-branch>'")),
		OPT_BOOL(0, "discard-changes", &opts.discard_changes,
			 N_("throw away local modifications")),
		OPT_ಕೊನೆ()
	};

	opts.dwim_new_local_branch = 1;
	opts.accept_ref = 1;
	opts.accept_pathspec = 0;
	opts.switch_branch_doing_nothing_is_ok = 0;
	opts.only_ವಿಲೀನ_on_switching_branches = 1;
	opts.implicit_detach = 0;
	opts.can_switch_when_ಒಳ_ಪ್ರಗತಿ = 0;
	opts.orphan_from_ಖಾಲಿಗಳು_ವೃಕ್ಷ = 1;
	opts.overlay_ಬಹುಲಕ = -1;

	options = parse_options_dup(switch_options);
	options = ಕೂಡಿಸು_common_options(&opts, options);
	options = ಕೂಡಿಸು_common_switch_branch_options(&opts, options);

	cb_option = 'c';

	ಹಿಂತಿರುಗಿ checkout_main(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, &opts, options,
			     CHECKOUT_SWITCH);
}

ಪೂರ್ಣಾಂಕ cmd_restore(ಪೂರ್ಣಾಂಕ argc,
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ checkout_opts opts = CHECKOUT_OPTS_INIT;
	ರಚನೆ option *options;
	ರಚನೆ option restore_options[] = {
		OPT_STRING('s', "source", &opts.from_treeish, "<tree-ish>",
			   N_("which tree-ish to checkout from")),
		OPT_BOOL('S', "staged", &opts.checkout_ಸೂಚ್ಯಂಕ,
			   N_("restore the index")),
		OPT_BOOL('W', "worktree", &opts.checkout_worktree,
			   N_("restore the working tree (default)")),
		OPT_BOOL(0, "ignore-unmerged", &opts.ignore_unmerged,
			 N_("ignore unmerged entries")),
		OPT_BOOL(0, "overlay", &opts.overlay_ಬಹುಲಕ, N_("use overlay mode")),
		OPT_ಕೊನೆ()
	};

	opts.accept_ref = 0;
	opts.accept_pathspec = 1;
	opts.ಖಾಲಿಗಳು_pathspec_ok = 0;
	opts.overlay_ಬಹುಲಕ = 0;
	opts.checkout_ಸೂಚ್ಯಂಕ = -1;    /* default off */
	opts.checkout_worktree = -2; /* default on */
	opts.ignore_unmerged_opt = "--ignore-unmerged";

	options = parse_options_dup(restore_options);
	options = ಕೂಡಿಸು_common_options(&opts, options);
	options = ಕೂಡಿಸು_checkout_ಮಾರ್ಗ_options(&opts, options);

	ಹಿಂತಿರುಗಿ checkout_main(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, &opts, options,
			     CHECKOUT_RESTORE);
}
