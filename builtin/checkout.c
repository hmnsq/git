#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "advice.h"
#ଅନ୍ତର୍ଭାଵ "branch.h"
#ଅନ୍ତର୍ଭାଵ "cache-tree.h"
#ଅନ୍ତର୍ଭାଵ "checkout.h"
#ଅନ୍ତର୍ଭାଵ "commit.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "diff.h"
#ଅନ୍ତର୍ଭାଵ "dir.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "hex.h"
#ଅନ୍ତର୍ଭାଵ "hook.h"
#ଅନ୍ତର୍ଭାଵ "merge-ll.h"
#ଅନ୍ତର୍ଭାଵ "lockfile.h"
#ଅନ୍ତର୍ଭାଵ "mem-pool.h"
#ଅନ୍ତର୍ଭାଵ "object-file.h"
#ଅନ୍ତର୍ଭାଵ "object-name.h"
#ଅନ୍ତର୍ଭାଵ "odb.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "path.h"
#ଅନ୍ତର୍ଭାଵ "preload-index.h"
#ଅନ୍ତର୍ଭାଵ "read-cache.h"
#ଅନ୍ତର୍ଭାଵ "refs.h"
#ଅନ୍ତର୍ଭାଵ "remote.h"
#ଅନ୍ତର୍ଭାଵ "repo-settings.h"
#ଅନ୍ତର୍ଭାଵ "resolve-undo.h"
#ଅନ୍ତର୍ଭାଵ "revision.h"
#ଅନ୍ତର୍ଭାଵ "sequencer.h"
#ଅନ୍ତର୍ଭାଵ "setup.h"
#ଅନ୍ତର୍ଭାଵ "sparse-index.h"
#ଅନ୍ତର୍ଭାଵ "strvec.h"
#ଅନ୍ତର୍ଭାଵ "submodule.h"
#ଅନ୍ତର୍ଭାଵ "symlinks.h"
#ଅନ୍ତର୍ଭାଵ "trace2.h"
#ଅନ୍ତର୍ଭାଵ "tree.h"
#ଅନ୍ତର୍ଭାଵ "tree-walk.h"
#ଅନ୍ତର୍ଭାଵ "unpack-trees.h"
#ଅନ୍ତର୍ଭାଵ "wt-status.h"
#ଅନ୍ତର୍ଭାଵ "xdiff-interface.h"
#ଅନ୍ତର୍ଭାଵ "entry.h"
#ଅନ୍ତର୍ଭାଵ "parallel-checkout.h"
#ଅନ୍ତର୍ଭାଵ "add-interactive.h"

ସଂରଚନା checkout_opts {
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା patch_mode;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା patch_context;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା patch_interhunk_context;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା auto_advance;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା quiet;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା merge;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା force;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା force_detach;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା implicit_detach;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା writeout_stage;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା overwrite_ignore;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ignore_skipworktree;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ignore_other_worktrees;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_progress;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_checkout_paths;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା overlay_mode;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା dwim_new_local_branch;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା discard_changes;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା accept_ref;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା accept_pathspec;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା switch_branch_doing_nothing_is_ok;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା only_merge_on_switching_branches;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା can_switch_when_in_progress;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା orphan_from_empty_ବୃକ୍ଷ;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା empty_pathspec_ok;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା checkout_index;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା checkout_worktree;
	ଅଚଳ ଅକ୍ଷର *ignore_unmerged_opt;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ignore_unmerged;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା pathspec_file_nul;
	ଅକ୍ଷର *pathspec_from_file;

	ଅଚଳ ଅକ୍ଷର *new_branch;
	ଅଚଳ ଅକ୍ଷର *new_branch_force;
	ଅଚଳ ଅକ୍ଷର *new_orphan_branch;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା new_branch_log;
	ଗଣନା branch_track track;
	ସଂରଚନା diff_options diff_options;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା conflict_style;

	ପୂର୍ଣ୍ଣସଂଖ୍ୟା branch_exists;
	ଅଚଳ ଅକ୍ଷର *prefix;
	ସଂରଚନା pathspec pathspec;
	ଅଚଳ ଅକ୍ଷର *from_treeish;
	ସଂରଚନା ବୃକ୍ଷ *source_ବୃକ୍ଷ;
};

#define CHECKOUT_OPTS_INIT { \
	.conflict_style = -1, \
	.merge = -1, \
	.patch_context = -1, \
	.patch_interhunk_context = -1, \
	.auto_advance = 1, \
}

#define MERGE_WORKING_ବୃକ୍ଷ_UNPACK_FAILED (-2)

ସଂରଚନା branch_info {
	ଅକ୍ଷର *name; /* The short name used */
	ଅକ୍ଷର *path; /* The full name of a real branch */
	ସଂରଚନା commit *commit; /* The named commit */
	ଅକ୍ଷର *refname; /* The full name of the ref being checked out. */
	ସଂରଚନା object_id oid; /* The object ID of the commit being checked out. */
	/*
	 * if not null the branch is detached because it's already
	 * checked out in this checkout
	 */
	ଅକ୍ଷର *checkout;
};

ସ୍ଥିର ଶୂନ୍ୟ branch_info_release(ସଂରଚନା branch_info *info)
{
	ମୁକ୍ତ(info->name);
	ମୁକ୍ତ(info->path);
	ମୁକ୍ତ(info->refname);
	ମୁକ୍ତ(info->checkout);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା post_checkout_hook(ସଂରଚନା commit *old_commit, ସଂରଚନା commit *new_commit,
			      ପୂର୍ଣ୍ଣସଂଖ୍ୟା changed)
{
	ସଂରଚନା run_hooks_opt opt = RUN_HOOKS_OPT_INIT_FORCE_SERIAL;

	/*
	 * "new_commit" can be NULL when checking out from the index before
	 * a commit exists.
	 */
	strvec_pushl(&opt.args,
		     oid_to_hex(old_commit ? &old_commit->object.oid : null_oid(the_hash_algo)),
		     oid_to_hex(new_commit ? &new_commit->object.oid : null_oid(the_hash_algo)),
		     changed ? "1" : "0",
		     ଖାଲି);

	ଫେରନ୍ତୁ run_hooks_opt(the_repository, "post-checkout", &opt);
}

/*
 * Handle a tree object and determine if we need to recurse into the
 * tree (READ_TREE_RECURSIVE) or skip it (0).
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା try_update_sparse_directory(ଅଚଳ ସଂରଚନା object_id *oid,
				       ସଂରଚନା strbuf *base,
				       ଅଚଳ ଅକ୍ଷର *pathname,
				       ପୂର୍ଣ୍ଣସଂଖ୍ୟା overlay_mode)
{
	ସଂରଚନା strbuf dirpath = STRBUF_INIT;
	ସଂରଚନା cache_entry *old;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା pos, result = READ_ବୃକ୍ଷ_RECURSIVE;

	ଯଦି (!the_repository->index->sparse_index)
		ଫେରନ୍ତୁ result;

	strbuf_addbuf(&dirpath, base);
	strbuf_addstr(&dirpath, pathname);
	strbuf_addch(&dirpath, '/');

	pos = index_name_pos_sparse(the_repository->index,
				    dirpath.buf, dirpath.len);
	ଯଦି (pos < 0)
		ଯାଅ cleanup;

	old = the_repository->index->cache[pos];
	ଯଦି (!S_ISSPARSEDIR(old->ce_mode))
		ଯାଅ cleanup;

	ଯଦି (oideq(oid, &old->oid)) {
		/* Tree content already matches; no need to descend. */
		result = 0;
	} ଅନ୍ୟଥା ଯଦି (!overlay_mode) {
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
	ଫେରନ୍ତୁ result;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା update_some(ଅଚଳ ସଂରଚନା object_id *oid, ସଂରଚନା strbuf *base,
		       ଅଚଳ ଅକ୍ଷର *pathname, unsigned mode, ଶୂନ୍ୟ *context)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା len;
	ସଂରଚନା cache_entry *ce;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା pos;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା overlay_mode = context ? *((ପୂର୍ଣ୍ଣସଂଖ୍ୟା *)context) : 1;

	ଯଦି (S_ISDIR(mode))
		ଫେରନ୍ତୁ try_update_sparse_directory(oid, base, pathname,
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
	ଯଦି (pos >= 0) {
		ସଂରଚନା cache_entry *old = the_repository->index->cache[pos];
		ଯଦି (ce->ce_mode == old->ce_mode &&
		    !ce_intent_to_add(old) &&
		    oideq(&ce->oid, &old->oid)) {
			old->ce_flags |= CE_UPDATE;
			discard_cache_entry(ce);
			ଫେରନ୍ତୁ 0;
		}
	}

	add_index_entry(the_repository->index, ce,
			ADD_CACHE_OK_TO_ADD | ADD_CACHE_OK_TO_REPLACE);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା read_ବୃକ୍ଷ_some(ସଂରଚନା ବୃକ୍ଷ *ବୃକ୍ଷ, ଅଚଳ ସଂରଚନା pathspec *pathspec,
			  ପୂର୍ଣ୍ଣସଂଖ୍ୟା overlay_mode)
{
	read_ବୃକ୍ଷ(the_repository, ବୃକ୍ଷ,
		  pathspec, update_some, &overlay_mode);

	/* update the index with the given tree's info
	 * for all args, expanding wildcards, and exit
	 * with any non-zero return code.
	 */
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା skip_same_name(ଅଚଳ ସଂରଚନା cache_entry *ce, ପୂର୍ଣ୍ଣସଂଖ୍ୟା pos)
{
	ଯେତେବେଳେ (++pos < the_repository->index->cache_nr &&
	       !strcmp(the_repository->index->cache[pos]->name, ce->name))
		; /* skip */
	ଫେରନ୍ତୁ pos;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା check_stage(ପୂର୍ଣ୍ଣସଂଖ୍ୟା stage, ଅଚଳ ସଂରଚନା cache_entry *ce, ପୂର୍ଣ୍ଣସଂଖ୍ୟା pos,
		       ପୂର୍ଣ୍ଣସଂଖ୍ୟା overlay_mode)
{
	ଯେତେବେଳେ (pos < the_repository->index->cache_nr &&
	       !strcmp(the_repository->index->cache[pos]->name, ce->name)) {
		ଯଦି (ce_stage(the_repository->index->cache[pos]) == stage)
			ଫେରନ୍ତୁ 0;
		pos++;
	}
	ଯଦି (!overlay_mode)
		ଫେରନ୍ତୁ 0;
	ଯଦି (stage == 2)
		ଫେରନ୍ତୁ error(_("path '%s' does not have our version"), ce->name);
	ଅନ୍ୟଥା
		ଫେରନ୍ତୁ error(_("path '%s' does not have their version"), ce->name);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା check_stages(unsigned stages, ଅଚଳ ସଂରଚନା cache_entry *ce, ପୂର୍ଣ୍ଣସଂଖ୍ୟା pos)
{
	unsigned seen = 0;
	ଅଚଳ ଅକ୍ଷର *name = ce->name;

	ଯେତେବେଳେ (pos < the_repository->index->cache_nr) {
		ce = the_repository->index->cache[pos];
		ଯଦି (strcmp(name, ce->name))
			ଭାଙ୍ଗ;
		seen |= (1 << ce_stage(ce));
		pos++;
	}
	ଯଦି ((stages & seen) != stages)
		ଫେରନ୍ତୁ error(_("path '%s' does not have all necessary versions"),
			     name);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା checkout_stage(ପୂର୍ଣ୍ଣସଂଖ୍ୟା stage, ଅଚଳ ସଂରଚନା cache_entry *ce, ପୂର୍ଣ୍ଣସଂଖ୍ୟା pos,
			  ଅଚଳ ସଂରଚନା checkout *state, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *nr_checkouts,
			  ପୂର୍ଣ୍ଣସଂଖ୍ୟା overlay_mode)
{
	ଯେତେବେଳେ (pos < the_repository->index->cache_nr &&
	       !strcmp(the_repository->index->cache[pos]->name, ce->name)) {
		ଯଦି (ce_stage(the_repository->index->cache[pos]) == stage)
			ଫେରନ୍ତୁ checkout_entry(the_repository->index->cache[pos], state,
					      ଖାଲି, nr_checkouts);
		pos++;
	}
	ଯଦି (!overlay_mode) {
		unlink_entry(ce, ଖାଲି);
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (stage == 2)
		ଫେରନ୍ତୁ error(_("path '%s' does not have our version"), ce->name);
	ଅନ୍ୟଥା
		ଫେରନ୍ତୁ error(_("path '%s' does not have their version"), ce->name);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା checkout_merged(ପୂର୍ଣ୍ଣସଂଖ୍ୟା pos, ଅଚଳ ସଂରଚନା checkout *state,
			   ପୂର୍ଣ୍ଣସଂଖ୍ୟା *nr_checkouts, ସଂରଚନା mem_pool *ce_mem_pool,
			   ପୂର୍ଣ୍ଣସଂଖ୍ୟା conflict_style)
{
	ସଂରଚନା cache_entry *ce = the_repository->index->cache[pos];
	ଅଚଳ ଅକ୍ଷର *path = ce->name;
	mmfile_t ancestor, ours, theirs;
	ଗଣନା ll_merge_result merge_status;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା status;
	ସଂରଚନା object_id oid;
	mmfile_t result_buf;
	ସଂରଚନା object_id threeway[3];
	unsigned mode = 0;
	ସଂରଚନା ll_merge_options ll_opts = LL_MERGE_OPTIONS_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା renormalize = 0;

	memset(threeway, 0, ଆକାର(threeway));
	ଯେତେବେଳେ (pos < the_repository->index->cache_nr) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା stage;
		stage = ce_stage(ce);
		ଯଦି (!stage || strcmp(path, ce->name))
			ଭାଙ୍ଗ;
		oidcpy(&threeway[stage - 1], &ce->oid);
		ଯଦି (stage == 2)
			mode = create_ce_mode(ce->ce_mode);
		pos++;
		ce = the_repository->index->cache[pos];
	}
	ଯଦି (is_null_oid(&threeway[1]) || is_null_oid(&threeway[2]))
		ଫେରନ୍ତୁ error(_("path '%s' does not have necessary versions"), path);

	read_mmblob(&ancestor, the_repository->objects, &threeway[0]);
	read_mmblob(&ours, the_repository->objects, &threeway[1]);
	read_mmblob(&theirs, the_repository->objects, &threeway[2]);

	repo_config_get_bool(the_repository, "merge.renormalize", &renormalize);
	ll_opts.renormalize = renormalize;
	ll_opts.conflict_style = conflict_style;
	merge_status = ll_merge(&result_buf, path, &ancestor, "base",
				&ours, "ours", &theirs, "theirs",
				state->istate, &ll_opts);
	ମୁକ୍ତ(ancestor.ptr);
	ମୁକ୍ତ(ours.ptr);
	ମୁକ୍ତ(theirs.ptr);
	ଯଦି (merge_status == LL_MERGE_ଦ୍ୱିମିକ_CONFLICT)
		warning("Cannot merge binary files: %s (%s vs. %s)",
			path, "ours", "theirs");
	ଯଦି (merge_status < 0 || !result_buf.ptr) {
		ମୁକ୍ତ(result_buf.ptr);
		ଫେରନ୍ତୁ error(_("path '%s': cannot merge"), path);
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
	ଯଦି (odb_write_object(the_repository->objects, result_buf.ptr, result_buf.size, OBJ_BLOB, &oid))
		die(_("Unable to add merge result for '%s'"), path);
	ମୁକ୍ତ(result_buf.ptr);
	ce = make_transient_cache_entry(mode, &oid, path, 2, ce_mem_pool);
	ଯଦି (!ce)
		die(_("make_cache_entry failed for path '%s'"), path);
	status = checkout_entry(ce, state, ଖାଲି, nr_checkouts);
	ଫେରନ୍ତୁ status;
}

ସ୍ଥିର ଶୂନ୍ୟ mark_ce_for_checkout_overlay(ସଂରଚନା cache_entry *ce,
					 ଅକ୍ଷର *ps_matched,
					 ଅଚଳ ସଂରଚନା checkout_opts *opts)
{
	ce->ce_flags &= ~CE_MATCHED;
	ଯଦି (!opts->ignore_skipworktree && ce_skip_worktree(ce))
		ଫେରନ୍ତୁ;
	ଯଦି (opts->source_ବୃକ୍ଷ && !(ce->ce_flags & CE_UPDATE))
		/*
		 * "git checkout tree-ish -- path", but this entry
		 * is in the original index but is not in tree-ish
		 * or does not match the pathspec; it will not be
		 * checked out to the working tree.  We will not do
		 * anything to this entry at all.
		 */
		ଫେରନ୍ତୁ;
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
	ଯଦି (ce_path_match(the_repository->index, ce, &opts->pathspec, ps_matched))
		ce->ce_flags |= CE_MATCHED;
}

ସ୍ଥିର ଶୂନ୍ୟ mark_ce_for_checkout_no_overlay(ସଂରଚନା cache_entry *ce,
					    ଅକ୍ଷର *ps_matched,
					    ଅଚଳ ସଂରଚନା checkout_opts *opts)
{
	ce->ce_flags &= ~CE_MATCHED;
	ଯଦି (!opts->ignore_skipworktree && ce_skip_worktree(ce))
		ଫେରନ୍ତୁ;
	ଯଦି (ce_path_match(the_repository->index, ce, &opts->pathspec, ps_matched)) {
		ce->ce_flags |= CE_MATCHED;
		ଯଦି (opts->source_ବୃକ୍ଷ && !(ce->ce_flags & CE_UPDATE))
			/*
			 * In overlay mode, but the path is not in
			 * tree-ish, which means we should remove it
			 * from the index and the working tree.
			 */
			ce->ce_flags |= CE_REMOVE | CE_WT_REMOVE;
	}
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା checkout_worktree(ଅଚଳ ସଂରଚନା checkout_opts *opts,
			     ଅଚଳ ସଂରଚନା branch_info *info)
{
	ସଂରଚନା checkout state = CHECKOUT_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା nr_checkouts = 0, nr_unmerged = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା errs = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା pos;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା pc_workers, pc_threshold;
	ସଂରଚନା mem_pool ce_mem_pool;

	state.force = 1;
	state.refresh_cache = 1;
	state.istate = the_repository->index;

	mem_pool_init(&ce_mem_pool, 0);
	get_parallel_checkout_configs(&pc_workers, &pc_threshold);
	init_checkout_metadata(&state.meta, info->refname,
			       info->commit ? &info->commit->object.oid : &info->oid,
			       ଖାଲି);

	enable_delayed_checkout(&state);

	ଯଦି (pc_workers > 1)
		init_parallel_checkout();

	ପାଇଁ (pos = 0; pos < the_repository->index->cache_nr; pos++) {
		ସଂରଚନା cache_entry *ce = the_repository->index->cache[pos];
		ଯଦି (ce->ce_flags & CE_MATCHED) {
			ଯଦି (!ce_stage(ce)) {
				errs |= checkout_entry(ce, &state,
						       ଖାଲି, &nr_checkouts);
				ଜାରି;
			}
			ଯଦି (opts->writeout_stage)
				errs |= checkout_stage(opts->writeout_stage,
						       ce, pos,
						       &state,
						       &nr_checkouts, opts->overlay_mode);
			ଅନ୍ୟଥା ଯଦି (opts->merge)
				errs |= checkout_merged(pos, &state,
							&nr_unmerged,
							&ce_mem_pool,
							opts->conflict_style);
			pos = skip_same_name(ce, pos) - 1;
		}
	}
	ଯଦି (pc_workers > 1)
		errs |= run_parallel_checkout(&state, pc_workers, pc_threshold,
					      ଖାଲି, ଖାଲି);
	mem_pool_discard(&ce_mem_pool, should_validate_cache_entries());
	remove_marked_cache_entries(the_repository->index, 1);
	remove_scheduled_dirs();
	errs |= finish_delayed_checkout(&state, opts->show_progress);

	ଯଦି (opts->ଗଣନା_checkout_paths) {
		ଯଦି (nr_unmerged)
			fprintf_ln(stderr, Q_("Recreated %d merge conflict",
					      "Recreated %d merge conflicts",
					      nr_unmerged),
				   nr_unmerged);
		ଯଦି (opts->source_ବୃକ୍ଷ)
			fprintf_ln(stderr, Q_("Updated %d path from %s",
					      "Updated %d paths from %s",
					      nr_checkouts),
				   nr_checkouts,
				   repo_find_unique_abbrev(the_repository, &opts->source_ବୃକ୍ଷ->object.oid,
							   DEFAULT_ABBREV));
		ଅନ୍ୟଥା ଯଦି (!nr_unmerged || nr_checkouts)
			fprintf_ln(stderr, Q_("Updated %d path from the index",
					      "Updated %d paths from the index",
					      nr_checkouts),
				   nr_checkouts);
	}

	ଫେରନ୍ତୁ errs;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା checkout_paths(ଅଚଳ ସଂରଚନା checkout_opts *opts,
			  ଅଚଳ ସଂରଚନା branch_info *new_branch_info)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା pos;
	ସ୍ଥିର ଅକ୍ଷର *ps_matched;
	ସଂରଚନା object_id rev;
	ସଂରଚନା commit *head;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା errs = 0;
	ସଂରଚନା lock_file lock_file = LOCK_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା checkout_index;

	trace2_cmd_mode(opts->patch_mode ? "patch" : "path");

	ଯଦି (opts->track != BRANCH_TRACK_UNSPECIFIED)
		die(_("'%s' cannot be used with updating paths"), "--track");

	ଯଦି (opts->new_branch_log)
		die(_("'%s' cannot be used with updating paths"), "-l");

	ଯଦି (opts->ignore_unmerged && opts->patch_mode)
		die(_("'%s' cannot be used with updating paths"),
		    opts->ignore_unmerged_opt);

	ଯଦି (opts->force_detach)
		die(_("'%s' cannot be used with updating paths"), "--detach");

	ଯଦି (opts->merge && opts->patch_mode)
		die(_("options '%s' and '%s' cannot be used together"), "--merge", "--patch");

	ଯଦି (opts->ignore_unmerged && opts->merge)
		die(_("options '%s' and '%s' cannot be used together"),
		    opts->ignore_unmerged_opt, "-m");

	ଯଦି (opts->new_branch)
		die(_("Cannot update paths and switch to branch '%s' at the same time."),
		    opts->new_branch);

	ଯଦି (!opts->checkout_worktree && !opts->checkout_index)
		die(_("neither '%s' or '%s' is specified"),
		    "--staged", "--worktree");

	ଯଦି (!opts->checkout_worktree && !opts->from_treeish)
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
	ଯଦି (!opts->accept_ref && opts->checkout_index) {
		ଯଦି (opts->writeout_stage)
			die(_("'%s' or '%s' cannot be used with %s"),
			    "--ours", "--theirs", "--staged");

		ଯଦି (opts->merge)
			die(_("'%s' or '%s' cannot be used with %s"),
			    "--merge", "--conflict", "--staged");
	}

	/*
	 * recreating unmerged index entries and writing out data from
	 * unmerged index entries would make no sense when checking out
	 * of a tree-ish.
	 */
	ଯଦି ((opts->merge || opts->writeout_stage) && opts->source_ବୃକ୍ଷ)
		die(_("'%s', '%s', or '%s' cannot be used when checking out of a tree"),
		    "--merge", "--ours", "--theirs");

	ଯଦି (opts->patch_mode) {
		ଗଣନା add_p_mode patch_mode;
		ସଂରଚନା interactive_options interactive_opts = {
			.context = opts->patch_context,
			.interhunkcontext = opts->patch_interhunk_context,
			.auto_advance = opts->auto_advance
		};
		ଅଚଳ ଅକ୍ଷର *rev = new_branch_info->name;
		ଅକ୍ଷର rev_oid[GIT_MAX_HEXSZ + 1];

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
		ଯଦି (rev && new_branch_info->commit && strcmp(rev, "HEAD"))
			rev = oid_to_hex_r(rev_oid, &new_branch_info->commit->object.oid);

		ଯଦି (opts->checkout_index && opts->checkout_worktree)
			patch_mode = ADD_P_CHECKOUT;
		ଅନ୍ୟଥା ଯଦି (opts->checkout_index && !opts->checkout_worktree)
			patch_mode = ADD_P_RESET;
		ଅନ୍ୟଥା ଯଦି (!opts->checkout_index && opts->checkout_worktree)
			patch_mode = ADD_P_WORKTREE;
		ଅନ୍ୟଥା
			BUG("either flag must have been set, worktree=%d, index=%d",
			    opts->checkout_worktree, opts->checkout_index);
		ଫେରନ୍ତୁ !!run_add_p(the_repository, patch_mode, &interactive_opts,
				   rev, &opts->pathspec, 0);
	}

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);
	ଯଦି (repo_read_index_preload(the_repository, &opts->pathspec, 0) < 0)
		ଫେରନ୍ତୁ error(_("index file corrupt"));

	ଯଦି (opts->source_ବୃକ୍ଷ)
		read_ବୃକ୍ଷ_some(opts->source_ବୃକ୍ଷ, &opts->pathspec,
			       opts->overlay_mode);
	ଯଦି (opts->merge)
		unmerge_index(the_repository->index, &opts->pathspec, CE_MATCHED);

	ps_matched = xcalloc(opts->pathspec.nr, 1);

	/*
	 * Make sure all pathspecs participated in locating the paths
	 * to be checked out.
	 */
	ପାଇଁ (pos = 0; pos < the_repository->index->cache_nr; pos++)
		ଯଦି (opts->overlay_mode)
			mark_ce_for_checkout_overlay(the_repository->index->cache[pos],
						     ps_matched,
						     opts);
		ଅନ୍ୟଥା
			mark_ce_for_checkout_no_overlay(the_repository->index->cache[pos],
							ps_matched,
							opts);

	ଯଦି (report_path_error(ps_matched, &opts->pathspec)) {
		ମୁକ୍ତ(ps_matched);
		ଫେରନ୍ତୁ 1;
	}
	ମୁକ୍ତ(ps_matched);

	/* Any unmerged paths? */
	ପାଇଁ (pos = 0; pos < the_repository->index->cache_nr; pos++) {
		ଅଚଳ ସଂରଚନା cache_entry *ce = the_repository->index->cache[pos];
		ଯଦି (ce->ce_flags & CE_MATCHED) {
			ଯଦି (!ce_stage(ce))
				ଜାରି;
			ଯଦି (opts->ignore_unmerged) {
				ଯଦି (!opts->quiet)
					warning(_("path '%s' is unmerged"), ce->name);
			} ଅନ୍ୟଥା ଯଦି (opts->writeout_stage) {
				errs |= check_stage(opts->writeout_stage, ce, pos, opts->overlay_mode);
			} ଅନ୍ୟଥା ଯଦି (opts->merge) {
				errs |= check_stages((1<<2) | (1<<3), ce, pos);
			} ଅନ୍ୟଥା {
				errs = 1;
				error(_("path '%s' is unmerged"), ce->name);
			}
			pos = skip_same_name(ce, pos) - 1;
		}
	}
	ଯଦି (errs)
		ଫେରନ୍ତୁ 1;

	/* Now we are committed to check them out */
	ଯଦି (opts->checkout_worktree)
		errs |= checkout_worktree(opts, new_branch_info);
	ଅନ୍ୟଥା
		remove_marked_cache_entries(the_repository->index, 1);

	/*
	 * Allow updating the index when checking out from the index.
	 * This is to save new stat info.
	 */
	ଯଦି (opts->checkout_worktree && !opts->checkout_index && !opts->source_ବୃକ୍ଷ)
		checkout_index = 1;
	ଅନ୍ୟଥା
		checkout_index = opts->checkout_index;

	ଯଦି (checkout_index) {
		ଯଦି (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
			die(_("unable to write new index file"));
	} ଅନ୍ୟଥା {
		/*
		 * NEEDSWORK: if --worktree is not specified, we
		 * should save stat info of checked out files in the
		 * index to avoid the next (potentially costly)
		 * refresh. But it's a bit tricker to do...
		 */
		rollback_lock_file(&lock_file);
	}

	refs_read_ref_full(get_main_ref_store(the_repository), "HEAD", 0,
			   &rev, ଖାଲି);
	head = lookup_commit_reference_gently(the_repository, &rev, 1);

	errs |= post_checkout_hook(head, head, 0);
	ଫେରନ୍ତୁ errs;
}

ସ୍ଥିର ଶୂନ୍ୟ show_local_changes(ସଂରଚନା object *head,
			       ଅଚଳ ସଂରଚନା diff_options *opts)
{
	ସଂରଚନା rev_info rev;
	/* I think we want full paths, even if we're in a subdirectory. */
	repo_init_revisions(the_repository, &rev, ଖାଲି);
	rev.diffopt.flags = opts->flags;
	rev.diffopt.output_format |= DIFF_FORMAT_NAME_STATUS;
	rev.diffopt.flags.recursive = 1;
	diff_setup_done(&rev.diffopt);
	add_pending_object(&rev, head, ଖାଲି);
	run_diff_index(&rev, 0);
	release_revisions(&rev);
}

ସ୍ଥିର ଶୂନ୍ୟ describe_detached_head(ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ, ସଂରଚନା commit *commit)
{
	ସଂରଚନା strbuf sb = STRBUF_INIT;

	ଯଦି (!repo_parse_commit(the_repository, commit))
		pp_commit_easy(CMIT_FMT_ONELINE, commit, &sb);
	ଯଦି (print_sha1_ellipsis()) {
		fprintf(stderr, "%s %s... %s\n", ସନ୍ଦେଶ,
			repo_find_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV),
			sb.buf);
	} ଅନ୍ୟଥା {
		fprintf(stderr, "%s %s %s\n", ସନ୍ଦେଶ,
			repo_find_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV),
			sb.buf);
	}
	strbuf_release(&sb);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା reset_ବୃକ୍ଷ(ସଂରଚନା ବୃକ୍ଷ *ବୃକ୍ଷ, ଅଚଳ ସଂରଚନା checkout_opts *o,
		      ପୂର୍ଣ୍ଣସଂଖ୍ୟା worktree, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *writeout_error,
		      ସଂରଚନା branch_info *info)
{
	ସଂରଚନା unpack_trees_options opts;
	ସଂରଚନା ବୃକ୍ଷ_desc ବୃକ୍ଷ_desc;

	memset(&opts, 0, ଆକାର(opts));
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
			       ଖାଲି);
	ଯଦି (repo_parse_ବୃକ୍ଷ(the_repository, ବୃକ୍ଷ) < 0)
		ଫେରନ୍ତୁ 128;
	init_ବୃକ୍ଷ_desc(&ବୃକ୍ଷ_desc, &ବୃକ୍ଷ->object.oid, ବୃକ୍ଷ->buffer, ବୃକ୍ଷ->size);
	ବାଛ (unpack_trees(1, &ବୃକ୍ଷ_desc, &opts)) {
	ସ୍ଥିତି -2:
		*writeout_error = 1;
		/*
		 * We return 0 nevertheless, as the index is all right
		 * and more importantly we have made best efforts to
		 * update paths in the work tree, and we cannot revert
		 * them.
		 */
		/* fallthrough */
	ସ୍ଥିତି 0:
		ଫେରନ୍ତୁ 0;
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
		ଫେରନ୍ତୁ 128;
	}
}

ସ୍ଥିର ଶୂନ୍ୟ setup_branch_path(ସଂରଚନା branch_info *branch)
{
	ସଂରଚନା strbuf buf = STRBUF_INIT;

	/*
	 * If this is a ref, resolve it; otherwise, look up the OID for our
	 * expression.  Failure here is okay.
	 */
	ଯଦି (!repo_dwim_ref(the_repository, branch->name, strlen(branch->name),
			   &branch->oid, &branch->refname, 0))
		repo_get_oid_committish(the_repository, branch->name, &branch->oid);

	copy_branchname(the_repository, &buf, branch->name, INTERPRET_BRANCH_LOCAL);
	ଯଦି (strcmp(buf.buf, branch->name)) {
		ମୁକ୍ତ(branch->name);
		branch->name = xstrdup(buf.buf);
	}
	strbuf_splice(&buf, 0, 0, "refs/heads/", 11);
	ମୁକ୍ତ(branch->path);
	branch->path = strbuf_detach(&buf, ଖାଲି);
}

ସ୍ଥିର ଶୂନ୍ୟ init_topts(ସଂରଚନା unpack_trees_options *topts,
		       ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_progress, ପୂର୍ଣ୍ଣସଂଖ୍ୟା overwrite_ignore,
		       bool quiet)
{
	memset(topts, 0, ଆକାର(*topts));
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

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା merge_working_ବୃକ୍ଷ(ଅଚଳ ସଂରଚନା checkout_opts *opts,
			      ସଂରଚନା branch_info *old_branch_info,
			      ସଂରଚନା branch_info *new_branch_info,
			      bool quiet,
			      ପୂର୍ଣ୍ଣସଂଖ୍ୟା *writeout_error)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
	ସଂରଚନା lock_file lock_file = LOCK_INIT;
	ସଂରଚନା ବୃକ୍ଷ *new_ବୃକ୍ଷ;

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);
	ଯଦି (repo_read_index_preload(the_repository, ଖାଲି, 0) < 0) {
		rollback_lock_file(&lock_file);
		ଫେରନ୍ତୁ error(_("index file corrupt"));
	}

	resolve_undo_clear_index(the_repository->index);
	ଯଦି (opts->new_orphan_branch && opts->orphan_from_empty_ବୃକ୍ଷ) {
		ଯଦି (new_branch_info->commit)
			BUG("'switch --orphan' should never accept a commit as starting point");
		new_ବୃକ୍ଷ = repo_parse_ବୃକ୍ଷ_indirect(the_repository,
						    the_hash_algo->empty_ବୃକ୍ଷ);
		ଯଦି (!new_ବୃକ୍ଷ)
			BUG("unable to read empty tree");
	} ଅନ୍ୟଥା {
		new_ବୃକ୍ଷ = repo_get_commit_ବୃକ୍ଷ(the_repository,
						new_branch_info->commit);
		ଯଦି (!new_ବୃକ୍ଷ) {
			rollback_lock_file(&lock_file);
			ଫେରନ୍ତୁ error(_("unable to read tree (%s)"),
				     oid_to_hex(&new_branch_info->commit->object.oid));
		}
	}
	ଯଦି (opts->discard_changes) {
		ret = reset_ବୃକ୍ଷ(new_ବୃକ୍ଷ, opts, 1, writeout_error, new_branch_info);
		ଯଦି (ret) {
			rollback_lock_file(&lock_file);
			ଫେରନ୍ତୁ ret;
		}
	} ଅନ୍ୟଥା {
		ସଂରଚନା ବୃକ୍ଷ_desc trees[2];
		ସଂରଚନା ବୃକ୍ଷ *ବୃକ୍ଷ;
		ସଂରଚନା unpack_trees_options topts;
		ଅଚଳ ସଂରଚନା object_id *old_commit_oid;

		refresh_index(the_repository->index, REFRESH_QUIET, ଖାଲି, ଖାଲି, ଖାଲି);

		ଯଦି (unmerged_index(the_repository->index)) {
			rollback_lock_file(&lock_file);
			error(_("you need to resolve your current index first"));
			ଫେରନ୍ତୁ 1;
		}

		/* 2-way merge to the new branch */
		init_topts(&topts, opts->show_progress,
			   opts->overwrite_ignore, quiet);
		init_checkout_metadata(&topts.meta, new_branch_info->refname,
				       new_branch_info->commit ?
				       &new_branch_info->commit->object.oid :
				       &new_branch_info->oid, ଖାଲି);

		old_commit_oid = old_branch_info->commit ?
			&old_branch_info->commit->object.oid :
			the_hash_algo->empty_ବୃକ୍ଷ;
		ବୃକ୍ଷ = repo_parse_ବୃକ୍ଷ_indirect(the_repository,
						old_commit_oid);
		ଯଦି (!ବୃକ୍ଷ)
			die(_("unable to parse commit %s"),
				oid_to_hex(old_commit_oid));

		init_ବୃକ୍ଷ_desc(&trees[0], &ବୃକ୍ଷ->object.oid,
			       ବୃକ୍ଷ->buffer, ବୃକ୍ଷ->size);
		ଯଦି (repo_parse_ବୃକ୍ଷ(the_repository, new_ବୃକ୍ଷ) < 0)
			die(ଖାଲି);
		ବୃକ୍ଷ = new_ବୃକ୍ଷ;
		init_ବୃକ୍ଷ_desc(&trees[1], &ବୃକ୍ଷ->object.oid,
			       ବୃକ୍ଷ->buffer, ବୃକ୍ଷ->size);

		ret = unpack_trees(2, trees, &topts);
		clear_unpack_trees_porcelain(&topts);
		ଯଦି (ret == -1) {
			rollback_lock_file(&lock_file);
			ଫେରନ୍ତୁ MERGE_WORKING_ବୃକ୍ଷ_UNPACK_FAILED;
		}
	}

	ଯଦି (!cache_ବୃକ୍ଷ_fully_valid(the_repository->index))
		cache_ବୃକ୍ଷ_update(the_repository->index, WRITE_ବୃକ୍ଷ_SILENT | WRITE_ବୃକ୍ଷ_REPAIR);

	ଯଦି (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
		die(_("unable to write new index file"));

	ଯଦି (!opts->discard_changes && !opts->quiet && new_branch_info->commit)
		show_local_changes(&new_branch_info->commit->object, &opts->diff_options);

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ report_tracking(ସଂରଚନା branch_info *new_branch_info)
{
	ସଂରଚନା strbuf sb = STRBUF_INIT;
	ସଂରଚନା branch *branch = branch_get(new_branch_info->name);

	ଯଦି (!format_tracking_info(branch, &sb, AHEAD_BEHIND_FULL, 1))
		ଫେରନ୍ତୁ;
	fputs(sb.buf, stdout);
	strbuf_release(&sb);
}

ସ୍ଥିର ଶୂନ୍ୟ update_refs_for_switch(ଅଚଳ ସଂରଚନା checkout_opts *opts,
				   ସଂରଚନା branch_info *old_branch_info,
				   ସଂରଚନା branch_info *new_branch_info)
{
	ସଂରଚନା strbuf ସନ୍ଦେଶ = STRBUF_INIT;
	ଅଚଳ ଅକ୍ଷର *old_desc, *reflog_ସନ୍ଦେଶ;
	ଯଦି (opts->new_branch) {
		ଯଦି (opts->new_orphan_branch) {
			ଗଣନା log_refs_config log_all_ref_updates = LOG_REFS_UNSET;
			ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ;
			ଅକ୍ଷର *refname;

			ଯଦି (!repo_config_get_string_tmp(the_repository, "core.logallrefupdates", &ମୂଲ୍ୟ))
				log_all_ref_updates = refs_parse_log_all_ref_updates_config(ମୂଲ୍ୟ);

			refname = mkpathdup("refs/heads/%s", opts->new_orphan_branch);
			ଯଦି (opts->new_branch_log &&
			    !should_autocreate_reflog(log_all_ref_updates, refname)) {
				ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
				ସଂରଚନା strbuf err = STRBUF_INIT;

				ret = refs_create_reflog(get_main_ref_store(the_repository),
							 refname, &err);
				ଯଦି (ret) {
					fprintf(stderr, _("Can not do reflog for '%s': %s\n"),
						opts->new_orphan_branch, err.buf);
					strbuf_release(&err);
					ମୁକ୍ତ(refname);
					ଫେରନ୍ତୁ;
				}
				strbuf_release(&err);
			}
			ମୁକ୍ତ(refname);
		}
		ଅନ୍ୟଥା
			create_branch(the_repository,
				      opts->new_branch, new_branch_info->name,
				      opts->new_branch_force ? 1 : 0,
				      opts->new_branch_force ? 1 : 0,
				      opts->new_branch_log,
				      opts->quiet,
				      opts->track,
				      0);
		ମୁକ୍ତ(new_branch_info->name);
		ମୁକ୍ତ(new_branch_info->refname);
		new_branch_info->name = xstrdup(opts->new_branch);
		setup_branch_path(new_branch_info);
	}

	old_desc = old_branch_info->name;
	ଯଦି (!old_desc && old_branch_info->commit)
		old_desc = oid_to_hex(&old_branch_info->commit->object.oid);

	reflog_ସନ୍ଦେଶ = getenv("GIT_REFLOG_ACTION");
	ଯଦି (!reflog_ସନ୍ଦେଶ)
		strbuf_addf(&ସନ୍ଦେଶ, "checkout: moving from %s to %s",
			old_desc ? old_desc : "(invalid)", new_branch_info->name);
	ଅନ୍ୟଥା
		strbuf_insertstr(&ସନ୍ଦେଶ, 0, reflog_ସନ୍ଦେଶ);

	ଯଦି (!strcmp(new_branch_info->name, "HEAD") && !new_branch_info->path && !opts->force_detach) {
		/* Nothing to do. */
	} ଅନ୍ୟଥା ଯଦି (opts->force_detach || !new_branch_info->path) {	/* No longer on any branch. */
		refs_update_ref(get_main_ref_store(the_repository), ସନ୍ଦେଶ.buf,
				"HEAD", &new_branch_info->commit->object.oid,
				ଖାଲି,
				REF_NO_DEREF, UPDATE_REFS_DIE_ON_ERR);
		ଯଦି (!opts->quiet) {
			ଯଦି (old_branch_info->path &&
			    advice_enabled(ADVICE_DETACHED_HEAD) && !opts->force_detach)
				detach_advice(new_branch_info->name);
			describe_detached_head(_("HEAD is now at"), new_branch_info->commit);
		}
	} ଅନ୍ୟଥା ଯଦି (new_branch_info->path) {	/* Switch branches. */
		ଯଦି (refs_update_symref(get_main_ref_store(the_repository), "HEAD", new_branch_info->path, ସନ୍ଦେଶ.buf) < 0)
			die(_("unable to update HEAD"));
		ଯଦି (!opts->quiet) {
			ଯଦି (old_branch_info->path && !strcmp(new_branch_info->path, old_branch_info->path)) {
				ଯଦି (opts->new_branch_force)
					fprintf(stderr, _("Reset branch '%s'\n"),
						new_branch_info->name);
				ଅନ୍ୟଥା
					fprintf(stderr, _("Already on '%s'\n"),
						new_branch_info->name);
			} ଅନ୍ୟଥା ଯଦି (opts->new_branch) {
				ଯଦି (opts->branch_exists)
					fprintf(stderr, _("Switched to and reset branch '%s'\n"), new_branch_info->name);
				ଅନ୍ୟଥା
					fprintf(stderr, _("Switched to a new branch '%s'\n"), new_branch_info->name);
			} ଅନ୍ୟଥା {
				fprintf(stderr, _("Switched to branch '%s'\n"),
					new_branch_info->name);
			}
		}
		ଯଦି (old_branch_info->path && old_branch_info->name) {
			ଯଦି (!refs_ref_exists(get_main_ref_store(the_repository), old_branch_info->path) && refs_reflog_exists(get_main_ref_store(the_repository), old_branch_info->path))
				refs_delete_reflog(get_main_ref_store(the_repository),
						   old_branch_info->path);
		}
	}
	remove_branch_state(the_repository, !opts->quiet);
	strbuf_release(&ସନ୍ଦେଶ);
	ଯଦି (!opts->quiet &&
	    !opts->force_detach &&
	    (new_branch_info->path || !strcmp(new_branch_info->name, "HEAD")))
		report_tracking(new_branch_info);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା add_pending_uninteresting_ref(ଅଚଳ ସଂରଚନା reference *ref, ଶୂନ୍ୟ *cb_data)
{
	add_pending_oid(cb_data, ref->name, ref->oid, UNINTERESTING);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ describe_one_orphan(ସଂରଚନା strbuf *sb, ସଂରଚନା commit *commit)
{
	strbuf_addstr(sb, "  ");
	strbuf_add_unique_abbrev(sb, &commit->object.oid, DEFAULT_ABBREV);
	strbuf_addch(sb, ' ');
	ଯଦି (!repo_parse_commit(the_repository, commit))
		pp_commit_easy(CMIT_FMT_ONELINE, commit, sb);
	strbuf_addch(sb, '\n');
}

#define ORPHAN_CUTOFF 4
ସ୍ଥିର ଶୂନ୍ୟ suggest_reattach(ସଂରଚନା commit *commit, ସଂରଚନା rev_info *revs)
{
	ସଂରଚନା commit *c, *last = ଖାଲି;
	ସଂରଚନା strbuf sb = STRBUF_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା lost = 0;
	ଯେତେବେଳେ ((c = get_revision(revs)) != ଖାଲି) {
		ଯଦି (lost < ORPHAN_CUTOFF)
			describe_one_orphan(&sb, c);
		last = c;
		lost++;
	}
	ଯଦି (ORPHAN_CUTOFF < lost) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା more = lost - ORPHAN_CUTOFF;
		ଯଦି (more == 1)
			describe_one_orphan(&sb, last);
		ଅନ୍ୟଥା
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

	ଯଦି (advice_enabled(ADVICE_DETACHED_HEAD))
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
ସ୍ଥିର ଶୂନ୍ୟ orphaned_commit_warning(ସଂରଚନା commit *old_commit, ସଂରଚନା commit *new_commit)
{
	ସଂରଚନା rev_info revs;
	ସଂରଚନା object *object = &old_commit->object;

	repo_init_revisions(the_repository, &revs, ଖାଲି);
	setup_revisions(0, ଖାଲି, &revs, ଖାଲି);

	object->flags &= ~UNINTERESTING;
	add_pending_object(&revs, object, oid_to_hex(&object->oid));

	refs_for_each_ref(get_main_ref_store(the_repository),
			  add_pending_uninteresting_ref, &revs);
	ଯଦି (new_commit)
		add_pending_oid(&revs, "HEAD",
				&new_commit->object.oid,
				UNINTERESTING);

	ଯଦି (prepare_revision_walk(&revs))
		die(_("internal error in revision walk"));
	ଯଦି (!(old_commit->object.flags & UNINTERESTING))
		suggest_reattach(old_commit, &revs);
	ଅନ୍ୟଥା
		describe_detached_head(_("Previous HEAD position was"), old_commit);

	/* Clean up objects used, as they will be reused. */
	repo_clear_commit_marks(the_repository, ALL_REV_FLAGS);
	release_revisions(&revs);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା switch_branches(ଅଚଳ ସଂରଚନା checkout_opts *opts,
			   ସଂରଚନା branch_info *new_branch_info)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;
	ସଂରଚନା branch_info old_branch_info = { 0 };
	ସଂରଚନା object_id rev;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା flag, writeout_error = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା do_merge = 1;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା created_autostash = 0;
	ଗଣନା stash_apply_result autostash_res = STASH_APPLY_CLEAN;
	ସଂରଚନା strbuf old_commit_shortname = STRBUF_INIT;
	ସଂରଚନା strbuf autostash_ସନ୍ଦେଶ = STRBUF_INIT;
	ଅଚଳ ଅକ୍ଷର *stash_label_base = ଖାଲି;

	trace2_cmd_mode("branch");

	memset(&old_branch_info, 0, ଆକାର(old_branch_info));
	old_branch_info.path = refs_resolve_refdup(get_main_ref_store(the_repository),
						   "HEAD", 0, &rev, &flag);
	ଯଦି (old_branch_info.path)
		old_branch_info.commit = lookup_commit_reference_gently(the_repository, &rev, 1);
	ଯଦି (!(flag & REF_ISSYMREF))
		FREE_AND_NULL(old_branch_info.path);

	ଯଦି (old_branch_info.path) {
		ଅଚଳ ଅକ୍ଷର *ଅଚଳ prefix = "refs/heads/";
		ଅଚଳ ଅକ୍ଷର *p;
		ଯଦି (skip_prefix(old_branch_info.path, prefix, &p))
			old_branch_info.name = xstrdup(p);
	}

	ଯଦି (opts->new_orphan_branch && opts->orphan_from_empty_ବୃକ୍ଷ) {
		ଯଦି (new_branch_info->name)
			BUG("'switch --orphan' should never accept a commit as starting point");
		new_branch_info->commit = ଖାଲି;
		new_branch_info->name = xstrdup("(empty)");
		do_merge = 1;
	}

	ଯଦି (!new_branch_info->name) {
		new_branch_info->name = xstrdup("HEAD");
		new_branch_info->commit = old_branch_info.commit;
		ଯଦି (!new_branch_info->commit)
			die(_("You are on a branch yet to be born"));
		parse_commit_or_die(new_branch_info->commit);

		ଯଦି (opts->only_merge_on_switching_branches)
			do_merge = 0;
	}

	ଯଦି (old_branch_info.name) {
		stash_label_base = old_branch_info.name;
	} ଅନ୍ୟଥା ଯଦି (old_branch_info.commit) {
		strbuf_add_unique_abbrev(&old_commit_shortname,
					 &old_branch_info.commit->object.oid,
					 DEFAULT_ABBREV);
		stash_label_base = old_commit_shortname.buf;
	}

	ଯଦି (do_merge) {
		ret = merge_working_ବୃକ୍ଷ(opts, &old_branch_info, new_branch_info,
					 opts->merge, &writeout_error);
		ଯଦି (ret == MERGE_WORKING_ବୃକ୍ଷ_UNPACK_FAILED && opts->merge) {
			strbuf_addf(&autostash_ସନ୍ଦେଶ,
				    "autostash while switching to '%s'",
				    new_branch_info->name);
			create_autostash_ref(the_repository,
					     "CHECKOUT_AUTOSTASH_HEAD",
					     autostash_ସନ୍ଦେଶ.buf, true);
			created_autostash = 1;
			ret = merge_working_ବୃକ୍ଷ(opts, &old_branch_info, new_branch_info,
						 false, &writeout_error);
		}
		ଯଦି (created_autostash) {
			ଯଦି (opts->conflict_style >= 0) {
				ସଂରଚନା strbuf cfg = STRBUF_INIT;
				strbuf_addf(&cfg, "merge.conflictStyle=%s",
					    conflict_style_name(opts->conflict_style));
				git_config_ଠେଲ_parameter(cfg.buf);
				strbuf_release(&cfg);
			}
			autostash_res = apply_autostash_ref(the_repository,
				    "CHECKOUT_AUTOSTASH_HEAD",
				    new_branch_info->name,
				    "local",
				    stash_label_base,
				    autostash_ସନ୍ଦେଶ.buf);
		}
		ଯଦି (ret) {
			branch_info_release(&old_branch_info);
			strbuf_release(&old_commit_shortname);
			strbuf_release(&autostash_ସନ୍ଦେଶ);
			ଫେରନ୍ତୁ ret < 0 ? 1 : ret;
		}
	}

	ଯଦି (!opts->quiet && !old_branch_info.path && old_branch_info.commit && new_branch_info->commit != old_branch_info.commit)
		orphaned_commit_warning(old_branch_info.commit, new_branch_info->commit);

	ଯଦି (autostash_res == STASH_APPLY_CONFLICT && !opts->quiet)
		fputc('\n', stderr);
	update_refs_for_switch(opts, &old_branch_info, new_branch_info);

	ଯଦି (created_autostash) {
		discard_index(the_repository->index);
		ଯଦି (repo_read_index(the_repository) < 0)
			die(_("index file corrupt"));

		ଯଦି (!opts->quiet && new_branch_info->commit) {
			printf(_("The following paths have local changes:\n"));
			show_local_changes(&new_branch_info->commit->object,
					   &opts->diff_options);
		}
	}

	ret = post_checkout_hook(old_branch_info.commit, new_branch_info->commit, 1);
	branch_info_release(&old_branch_info);
	strbuf_release(&old_commit_shortname);
	strbuf_release(&autostash_ସନ୍ଦେଶ);

	ଫେରନ୍ତୁ ret || writeout_error;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା git_checkout_config(ଅଚଳ ଅକ୍ଷର *var, ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ,
			       ଅଚଳ ସଂରଚନା config_context *ctx, ଶୂନ୍ୟ *cb)
{
	ସଂରଚନା checkout_opts *opts = cb;

	ଯଦି (!strcmp(var, "diff.ignoresubmodules")) {
		ଯଦି (!ମୂଲ୍ୟ)
			ଫେରନ୍ତୁ config_error_nonbool(var);
		handle_ignore_submodules_arg(&opts->diff_options, ମୂଲ୍ୟ);
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (!strcmp(var, "checkout.guess")) {
		opts->dwim_new_local_branch = git_config_bool(var, ମୂଲ୍ୟ);
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (starts_with(var, "submodule."))
		ଫେରନ୍ତୁ git_default_submodule_config(var, ମୂଲ୍ୟ, ଖାଲି);

	ଫେରନ୍ତୁ git_xmerge_config(var, ମୂଲ୍ୟ, ctx, ଖାଲି);
}

ସ୍ଥିର ଶୂନ୍ୟ setup_new_branch_info_and_source_ବୃକ୍ଷ(
	ସଂରଚନା branch_info *new_branch_info,
	ସଂରଚନା checkout_opts *opts,
	ସଂରଚନା object_id *rev,
	ଅଚଳ ଅକ୍ଷର *arg)
{
	ସଂରଚନା ବୃକ୍ଷ **source_ବୃକ୍ଷ = &opts->source_ବୃକ୍ଷ;
	ସଂରଚନା object_id branch_rev;

	/* treat '@' as a shortcut for 'HEAD' */
	new_branch_info->name = !strcmp(arg, "@") ? xstrdup("HEAD") :
						    xstrdup(arg);
	setup_branch_path(new_branch_info);

	ଯଦି (!check_refname_format(new_branch_info->path, 0) &&
	    !refs_read_ref(get_main_ref_store(the_repository), new_branch_info->path, &branch_rev))
		oidcpy(rev, &branch_rev);
	ଅନ୍ୟଥା
		/* not an existing branch */
		FREE_AND_NULL(new_branch_info->path);

	new_branch_info->commit = lookup_commit_reference_gently(the_repository, rev, 1);
	ଯଦି (!new_branch_info->commit) {
		/* not a commit */
		*source_ବୃକ୍ଷ = repo_parse_ବୃକ୍ଷ_indirect(the_repository, rev);
		ଯଦି (!*source_ବୃକ୍ଷ)
			die(_("unable to read tree (%s)"), oid_to_hex(rev));
	} ଅନ୍ୟଥା {
		parse_commit_or_die(new_branch_info->commit);
		*source_ବୃକ୍ଷ = repo_get_commit_ବୃକ୍ଷ(the_repository,
						    new_branch_info->commit);
		ଯଦି (!*source_ବୃକ୍ଷ)
			die(_("unable to read tree (%s)"),
			    oid_to_hex(&new_branch_info->commit->object.oid));
	}
}


ଗଣନା checkout_command {
	CHECKOUT_CHECKOUT = 1,
	CHECKOUT_SWITCH = 2,
	CHECKOUT_RESTORE = 3,
};

ସ୍ଥିର ଶୂନ୍ୟ advise_disambiguating_remotes(ଗଣନା checkout_command which_command,
					  ଅଚଳ ଅକ୍ଷର *branch,
					  ଅଚଳ ସଂରଚନା string_list *matched_remote_names)
{
	ଅଚଳ ଅକ୍ଷର *cmdname;
	ସଂରଚନା string_list_item *item;

	ବାଛ (which_command) {
	ସ୍ଥିତି CHECKOUT_CHECKOUT:
		cmdname = "checkout";
		ଭାଙ୍ଗ;
	ସ୍ଥିତି CHECKOUT_SWITCH:
		cmdname = "switch";
		ଭାଙ୍ଗ;
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
		BUG("command <%d> should not reach advise_disambiguating_remotes",
		    which_command);
		ଭାଙ୍ଗ;
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

ସ୍ଥିର ଅକ୍ଷର *parse_remote_branch(ଅଚଳ ଅକ୍ଷର *arg,
				 ସଂରଚନା object_id *rev,
				 ପୂର୍ଣ୍ଣସଂଖ୍ୟା could_be_checkout_paths,
				 ଗଣନା checkout_command which_command)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_matches = 0;
	ସଂରଚନା string_list matched_remote_names = STRING_LIST_INIT_DUP;

	ଅକ୍ଷର *remote = unique_tracking_name(arg, rev, &ଅଙ୍କ_matches,
					    &matched_remote_names);

	ଯଦି (remote && could_be_checkout_paths) {
		die(_("'%s' could be both a local file and a tracking branch.\n"
			"Please use -- (and optionally --no-guess) to disambiguate"),
		    arg);
	}

	ଯଦି (!remote && ଅଙ୍କ_matches > 1) {
		ଯଦି (advice_enabled(ADVICE_CHECKOUT_AMBIGUOUS_REMOTE_BRANCH_NAME))
			advise_disambiguating_remotes(which_command, arg,
						      &matched_remote_names);
		die(_("'%s' matched multiple (%d) remote tracking branches"),
		    arg, ଅଙ୍କ_matches);
	}

	string_list_clear(&matched_remote_names, 0);

	ଫେରନ୍ତୁ remote;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା parse_branchname_arg(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv,
				ପୂର୍ଣ୍ଣସଂଖ୍ୟା dwim_new_local_branch_ok,
				ଗଣନା checkout_command which_command,
				ସଂରଚନା branch_info *new_branch_info,
				ସଂରଚନା checkout_opts *opts,
				ସଂରଚନା object_id *rev)
{
	ଅଚଳ ଅକ୍ଷର **new_branch = &opts->new_branch;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା argcount = 0;
	ଅଚଳ ଅକ୍ଷର *arg;
	ଅକ୍ଷର *remote = ଖାଲି;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା dash_dash_pos;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା has_dash_dash = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

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
	ଯଦି (!argc)
		ଫେରନ୍ତୁ 0;

	ଯଦି (!opts->accept_pathspec) {
		ଯଦି (argc > 1)
			die(_("only one reference expected"));
		has_dash_dash = 1; /* helps disambiguate */
	}

	arg = argv[0];
	dash_dash_pos = -1;
	ପାଇଁ (i = 0; i < argc; i++) {
		ଯଦି (opts->accept_pathspec && !strcmp(argv[i], "--")) {
			dash_dash_pos = i;
			ଭାଙ୍ଗ;
		}
	}
	ଯଦି (dash_dash_pos == 0)
		ଫେରନ୍ତୁ 1; /* case (2) */
	ଅନ୍ୟଥା ଯଦି (dash_dash_pos == 1)
		has_dash_dash = 1; /* case (3) or (1) */
	ଅନ୍ୟଥା ଯଦି (dash_dash_pos >= 2)
		die(_("only one reference expected, %d given."), dash_dash_pos);
	opts->ଗଣନା_checkout_paths = !opts->quiet && !has_dash_dash;

	ଯଦି (!strcmp(arg, "-"))
		arg = "@{-1}";

	ଯଦି (repo_get_oid_mb(the_repository, arg, rev)) {
		/*
		 * Either case (3) or (4), with <something> not being
		 * a commit, or an attempt to use case (1) with an
		 * invalid ref.
		 *
		 * It's likely an error, but we need to find out if
		 * we should auto-create the branch, case (3).(b).
		 */
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା recover_with_dwim = dwim_new_local_branch_ok;

		ପୂର୍ଣ୍ଣସଂଖ୍ୟା could_be_checkout_paths = !has_dash_dash &&
			check_filename(opts->prefix, arg);

		ଯଦି (!has_dash_dash && !no_wildcard(arg))
			recover_with_dwim = 0;

		/*
		 * Accept "git checkout foo", "git checkout foo --"
		 * and "git switch foo" as candidates for dwim.
		 */
		ଯଦି (!(argc == 1 && !has_dash_dash) &&
		    !(argc == 2 && has_dash_dash) &&
		    opts->accept_pathspec)
			recover_with_dwim = 0;

		ଯଦି (recover_with_dwim) {
			remote = parse_remote_branch(arg, rev,
						     could_be_checkout_paths,
						     which_command);
			ଯଦି (remote) {
				*new_branch = arg;
				arg = remote;
				/* DWIMmed to create local branch, case (3).(b) */
			} ଅନ୍ୟଥା {
				recover_with_dwim = 0;
			}
		}

		ଯଦି (!recover_with_dwim) {
			ଯଦି (has_dash_dash)
				die(_("invalid reference: %s"), arg);
			ଫେରନ୍ତୁ argcount;
		}
	}

	/* we can't end up being in (2) anymore, eat the argument */
	argcount++;
	argv++;
	argc--;

	setup_new_branch_info_and_source_ବୃକ୍ଷ(new_branch_info, opts, rev, arg);

	ଯଦି (!opts->source_ବୃକ୍ଷ)                   /* case (1): want a tree */
		die(_("reference is not a tree: %s"), arg);

	ଯଦି (!has_dash_dash) {	/* case (3).(d) -> (1) */
		/*
		 * Do not complain the most common case
		 *	git checkout branch
		 * even if there happen to be a file called 'branch';
		 * it would be extremely annoying.
		 */
		ଯଦି (argc)
			verify_non_filename(the_repository, opts->prefix, arg);
	} ଅନ୍ୟଥା ଯଦି (opts->accept_pathspec) {
		argcount++;
		argv++;
		argc--;
	}

	ମୁକ୍ତ(remote);
	ଫେରନ୍ତୁ argcount;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା switch_unborn_to_new_branch(ଅଚଳ ସଂରଚନା checkout_opts *opts)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା status;
	ସଂରଚନା strbuf branch_ref = STRBUF_INIT;

	trace2_cmd_mode("unborn");

	ଯଦି (!opts->new_branch)
		die(_("You are on a branch yet to be born"));
	strbuf_addf(&branch_ref, "refs/heads/%s", opts->new_branch);
	status = refs_update_symref(get_main_ref_store(the_repository),
				    "HEAD", branch_ref.buf, "checkout -b");
	strbuf_release(&branch_ref);
	ଯଦି (!opts->quiet)
		fprintf(stderr, _("Switched to a new branch '%s'\n"),
			opts->new_branch);
	ଫେରନ୍ତୁ status;
}

ସ୍ଥିର ଶୂନ୍ୟ die_expecting_a_branch(ଅଚଳ ସଂରଚନା branch_info *branch_info)
{
	ସଂରଚନା object_id oid;
	ଅକ୍ଷର *to_free;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା code;

	ଯଦି (repo_dwim_ref(the_repository, branch_info->name,
			  strlen(branch_info->name), &oid, &to_free, 0) == 1) {
		ଅଚଳ ଅକ୍ଷର *ref = to_free;

		ଯଦି (skip_prefix(ref, "refs/tags/", &ref))
			code = die_message(_("a branch is expected, got tag '%s'"), ref);
		ଅନ୍ୟଥା ଯଦି (skip_prefix(ref, "refs/remotes/", &ref))
			code = die_message(_("a branch is expected, got remote branch '%s'"), ref);
		ଅନ୍ୟଥା
			code = die_message(_("a branch is expected, got '%s'"), ref);
	}
	ଅନ୍ୟଥା ଯଦି (branch_info->commit)
		code = die_message(_("a branch is expected, got commit '%s'"), branch_info->name);
	ଅନ୍ୟଥା
		/*
		 * This case should never happen because we already die() on
		 * non-commit, but just in case.
		 */
		code = die_message(_("a branch is expected, got '%s'"), branch_info->name);

	ଯଦି (advice_enabled(ADVICE_SUGGEST_DETACHING_HEAD))
		advise(_("If you want to detach HEAD at the commit, try again with the --detach option."));

	exit(code);
}

ସ୍ଥିର ଶୂନ୍ୟ die_if_some_operation_in_progress(ଶୂନ୍ୟ)
{
	ସଂରଚନା wt_status_state state;

	memset(&state, 0, ଆକାର(state));
	wt_status_get_state(the_repository, &state, 0);

	ଯଦି (state.merge_in_progress)
		die(_("cannot switch branch while merging\n"
		      "Consider \"git merge --quit\" "
		      "or \"git worktree add\"."));
	ଯଦି (state.am_in_progress)
		die(_("cannot switch branch in the middle of an am session\n"
		      "Consider \"git am --quit\" "
		      "or \"git worktree add\"."));
	ଯଦି (state.rebase_interactive_in_progress || state.rebase_in_progress)
		die(_("cannot switch branch while rebasing\n"
		      "Consider \"git rebase --quit\" "
		      "or \"git worktree add\"."));
	ଯଦି (state.cherry_pick_in_progress)
		die(_("cannot switch branch while cherry-picking\n"
		      "Consider \"git cherry-pick --quit\" "
		      "or \"git worktree add\"."));
	ଯଦି (state.revert_in_progress)
		die(_("cannot switch branch while reverting\n"
		      "Consider \"git revert --quit\" "
		      "or \"git worktree add\"."));
	ଯଦି (state.bisect_in_progress)
		warning(_("you are switching branch while bisecting"));

	wt_status_state_free_buffers(&state);
}

/*
 * die if attempting to checkout an existing branch that is in use
 * in another worktree, unless ignore-other-wortrees option is given.
 * The check is bypassed when the branch is already the current one,
 * as it will not make things any worse.
 */
ସ୍ଥିର ଶୂନ୍ୟ die_if_switching_to_a_branch_in_use(ସଂରଚନା checkout_opts *opts,
						ଅଚଳ ଅକ୍ଷର *full_ref)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags;
	ଅକ୍ଷର *head_ref;

	ଯଦି (opts->ignore_other_worktrees)
		ଫେରନ୍ତୁ;
	head_ref = refs_resolve_refdup(get_main_ref_store(the_repository),
				       "HEAD", 0, ଖାଲି, &flags);
	ଯଦି (head_ref && (!(flags & REF_ISSYMREF) || strcmp(head_ref, full_ref)))
		die_if_checked_out(full_ref, 1);
	ମୁକ୍ତ(head_ref);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା checkout_branch(ସଂରଚନା checkout_opts *opts,
			   ସଂରଚନା branch_info *new_branch_info)
{
	ସଂରଚନା repo_config_ମୂଲ୍ୟଗୁଡ଼ିକ *cfg = repo_config_ମୂଲ୍ୟଗୁଡ଼ିକ(the_repository);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା noop_switch = (!new_branch_info->name &&
			   !opts->new_branch &&
			   !opts->force_detach);

	ଯଦି (opts->pathspec.nr)
		die(_("paths cannot be used with switching branches"));

	ଯଦି (opts->patch_mode)
		die(_("'%s' cannot be used with switching branches"),
		    "--patch");

	ଯଦି (opts->overlay_mode != -1)
		die(_("'%s' cannot be used with switching branches"),
		    "--[no]-overlay");

	ଯଦି (opts->writeout_stage) {
		ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ;
		ଯଦି (noop_switch)
			ସନ୍ଦେଶ = _("'%s' needs the paths to check out");
		ଅନ୍ୟଥା
			ସନ୍ଦେଶ = _("'%s' cannot be used with switching branches");
		die(ସନ୍ଦେଶ, "--ours/--theirs");
	}

	ଯଦି (opts->force && opts->merge)
		die(_("'%s' cannot be used with '%s'"), "-f", "-m");

	ଯଦି (opts->discard_changes && opts->merge)
		die(_("'%s' cannot be used with '%s'"), "--discard-changes", "--merge");

	ଯଦି (opts->force_detach && opts->new_branch)
		die(_("'%s' cannot be used with '%s'"),
		    "--detach", "-b/-B/--orphan");

	ଯଦି (opts->new_orphan_branch) {
		ଯଦି (opts->track != BRANCH_TRACK_UNSPECIFIED)
			die(_("'%s' cannot be used with '%s'"), "--orphan", "-t");
		ଯଦି (opts->orphan_from_empty_ବୃକ୍ଷ && new_branch_info->name)
			die(_("'%s' cannot take <start-point>"), "--orphan");
	} ଅନ୍ୟଥା ଯଦି (opts->force_detach) {
		ଯଦି (opts->track != BRANCH_TRACK_UNSPECIFIED)
			die(_("'%s' cannot be used with '%s'"), "--detach", "-t");
	} ଅନ୍ୟଥା ଯଦି (opts->track == BRANCH_TRACK_UNSPECIFIED)
		opts->track = cfg->branch_track;

	ଯଦି (new_branch_info->name && !new_branch_info->commit)
		die(_("Cannot switch branch to a non-commit '%s'"),
		    new_branch_info->name);

	ଯଦି (noop_switch &&
	    !opts->switch_branch_doing_nothing_is_ok)
		die(_("missing branch or commit argument"));

	ଯଦି (!opts->implicit_detach &&
	    !opts->force_detach &&
	    !opts->new_branch &&
	    !opts->new_branch_force &&
	    new_branch_info->name &&
	    !new_branch_info->path)
		die_expecting_a_branch(new_branch_info);

	ଯଦି (!opts->can_switch_when_in_progress)
		die_if_some_operation_in_progress();

	/* "git checkout <branch>" */
	ଯଦି (new_branch_info->path && !opts->force_detach && !opts->new_branch)
		die_if_switching_to_a_branch_in_use(opts, new_branch_info->path);

	/* "git checkout -B <branch>" */
	ଯଦି (opts->new_branch_force) {
		ଅକ୍ଷର *full_ref = xstrfmt("refs/heads/%s", opts->new_branch);
		die_if_switching_to_a_branch_in_use(opts, full_ref);
		ମୁକ୍ତ(full_ref);
	}

	ଯଦି (!new_branch_info->commit && opts->new_branch) {
		ସଂରଚନା object_id rev;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା flag;

		ଯଦି (!refs_read_ref_full(get_main_ref_store(the_repository), "HEAD", 0, &rev, &flag) &&
		    (flag & REF_ISSYMREF) && is_null_oid(&rev))
			ଫେରନ୍ତୁ switch_unborn_to_new_branch(opts);
	}
	ଫେରନ୍ତୁ switch_branches(opts, new_branch_info);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା parse_opt_conflict(ଅଚଳ ସଂରଚନା option *o, ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ସଂରଚନା checkout_opts *opts = o->ମୂଲ୍ୟ;

	ଯଦି (unset) {
		opts->conflict_style = -1;
		ଫେରନ୍ତୁ 0;
	}
	opts->conflict_style = parse_conflict_style_name(arg);
	ଯଦି (opts->conflict_style < 0)
		ଫେରନ୍ତୁ error(_("unknown conflict style '%s'"), arg);
	/* --conflict overrides a previous --no-merge */
	ଯଦି (!opts->merge)
		opts->merge = -1;

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ସଂରଚନା option *add_common_options(ସଂରଚନା checkout_opts *opts,
					 ସଂରଚନା option *prevopts)
{
	ସଂରଚନା option options[] = {
		OPT__QUIET(&opts->quiet, N_("suppress progress reporting")),
		OPT_CALLBACK_F(0, "recurse-submodules", ଖାଲି,
			    "checkout", "control recursive updating of submodules",
			    PARSE_OPT_OPTARG, option_parse_recurse_submodules_worktree_updater),
		OPT_BOOL(0, "progress", &opts->show_progress, N_("force progress reporting")),
		OPT_BOOL('m', "merge", &opts->merge, N_("perform a 3-way merge with the new branch")),
		OPT_CALLBACK(0, "conflict", opts, N_("style"),
			     N_("conflict style (merge, diff3, or zdiff3)"),
			     parse_opt_conflict),
		OPT_END()
	};
	ସଂରଚନା option *newopts = parse_options_concat(prevopts, options);
	ମୁକ୍ତ(prevopts);
	ଫେରନ୍ତୁ newopts;
}

ସ୍ଥିର ସଂରଚନା option *add_common_switch_branch_options(
	ସଂରଚନା checkout_opts *opts, ସଂରଚନା option *prevopts)
{
	ସଂରଚନା option options[] = {
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
	ସଂରଚନା option *newopts = parse_options_concat(prevopts, options);
	ମୁକ୍ତ(prevopts);
	ଫେରନ୍ତୁ newopts;
}

ସ୍ଥିର ସଂରଚନା option *add_checkout_path_options(ସଂରଚନା checkout_opts *opts,
						ସଂରଚନା option *prevopts)
{
	ସଂରଚନା option options[] = {
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
	ସଂରଚନା option *newopts = parse_options_concat(prevopts, options);
	ମୁକ୍ତ(prevopts);
	ଫେରନ୍ତୁ newopts;
}

/* create-branch option (either b or c) */
ସ୍ଥିର ଅକ୍ଷର cb_option = 'b';

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା checkout_main(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
			 ସଂରଚନା checkout_opts *opts, ସଂରଚନା option *options,
			 ଗଣନା checkout_command which_command)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା parseopt_flags = 0;
	ସଂରଚନା branch_info new_branch_info = { 0 };
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ checkout_usage[] = {
		N_("git checkout [<options>] <branch>"),
		N_("git checkout [<options>] [<branch>] -- <file>..."),
		ଖାଲି,
	};

	ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ switch_branch_usage[] = {
		N_("git switch [<options>] [<branch>]"),
		ଖାଲି,
	};

	ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ restore_usage[] = {
		N_("git restore [<options>] [--source=<branch>] <file>..."),
		ଖାଲି,
	};

	ଅଚଳ ଅକ୍ଷର * ଅଚଳ *usagestr;

	ବାଛ (which_command) {
	ସ୍ଥିତି CHECKOUT_CHECKOUT:
		usagestr = checkout_usage;
		ଭାଙ୍ଗ;
	ସ୍ଥିତି CHECKOUT_SWITCH:
		usagestr = switch_branch_usage;
		ଭାଙ୍ଗ;
	ସ୍ଥିତି CHECKOUT_RESTORE:
		usagestr = restore_usage;
		ଭାଙ୍ଗ;
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
		BUG("no such checkout variant %d", which_command);
	}

	opts->overwrite_ignore = 1;
	opts->prefix = prefix;
	opts->show_progress = -1;

	repo_config(the_repository, git_checkout_config, opts);
	ଯଦି (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_full_index = 0;
	}

	opts->track = BRANCH_TRACK_UNSPECIFIED;

	ଯଦି (!opts->accept_pathspec && !opts->accept_ref)
		BUG("make up your mind, you need to take _something_");
	ଯଦି (opts->accept_pathspec && opts->accept_ref)
		parseopt_flags = PARSE_OPT_KEEP_DASHDASH;

	argc = parse_options(argc, argv, prefix, options,
			     usagestr, parseopt_flags);

	ଯଦି (opts->patch_context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	ଯଦି (opts->patch_interhunk_context < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	ଯଦି (!opts->patch_mode) {
		ଯଦି (opts->patch_context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--patch");
		ଯଦି (opts->patch_interhunk_context != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--patch");
		ଯଦି (!opts->auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--patch");
	}

	ଯଦି (opts->show_progress < 0) {
		ଯଦି (opts->quiet)
			opts->show_progress = 0;
		ଅନ୍ୟଥା
			opts->show_progress = isatty(2);
	}

	/* --conflicts implies --merge */
	ଯଦି (opts->merge == -1)
		opts->merge = opts->conflict_style >= 0;

	ଯଦି (opts->force) {
		opts->discard_changes = 1;
		opts->ignore_unmerged_opt = "--force";
		opts->ignore_unmerged = 1;
	}

	ଯଦି ((!!opts->new_branch + !!opts->new_branch_force + !!opts->new_orphan_branch) > 1)
		die(_("options '-%c', '-%c', and '%s' cannot be used together"),
			cb_option, toupper(cb_option), "--orphan");

	ଯଦି (opts->overlay_mode == 1 && opts->patch_mode)
		die(_("options '%s' and '%s' cannot be used together"), "-p", "--overlay");

	ଯଦି (opts->checkout_index >= 0 || opts->checkout_worktree >= 0) {
		ଯଦି (opts->checkout_index < 0)
			opts->checkout_index = 0;
		ଯଦି (opts->checkout_worktree < 0)
			opts->checkout_worktree = 0;
	} ଅନ୍ୟଥା {
		ଯଦି (opts->checkout_index < 0)
			opts->checkout_index = -opts->checkout_index - 1;
		ଯଦି (opts->checkout_worktree < 0)
			opts->checkout_worktree = -opts->checkout_worktree - 1;
	}
	ଯଦି (opts->checkout_index < 0 || opts->checkout_worktree < 0)
		BUG("these flags should be non-negative by now");
	/*
	 * convenient shortcut: "git restore --staged [--worktree]" equals
	 * "git restore --staged [--worktree] --source HEAD"
	 */
	ଯଦି (!opts->from_treeish && opts->checkout_index)
		opts->from_treeish = "HEAD";

	/*
	 * From here on, new_branch will contain the branch to be checked out,
	 * and new_branch_force and new_orphan_branch will tell us which one of
	 * -b/-B/-c/-C/--orphan is being used.
	 */
	ଯଦି (opts->new_branch_force)
		opts->new_branch = opts->new_branch_force;

	ଯଦି (opts->new_orphan_branch)
		opts->new_branch = opts->new_orphan_branch;

	/* --track without -c/-C/-b/-B/--orphan should DWIM */
	ଯଦି (opts->track != BRANCH_TRACK_UNSPECIFIED && !opts->new_branch) {
		ଅଚଳ ଅକ୍ଷର *argv0 = argv[0];
		ଯଦି (!argc || !strcmp(argv0, "--"))
			die(_("--track needs a branch name"));
		skip_prefix(argv0, "refs/", &argv0);
		skip_prefix(argv0, "remotes/", &argv0);
		argv0 = strchr(argv0, '/');
		ଯଦି (!argv0 || !argv0[1])
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
	ଯଦି (argc && opts->accept_ref) {
		ସଂରଚନା object_id rev;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା dwim_ok =
			!opts->patch_mode &&
			opts->dwim_new_local_branch &&
			opts->track == BRANCH_TRACK_UNSPECIFIED &&
			!opts->new_branch;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = parse_branchname_arg(argc, argv, dwim_ok, which_command,
					     &new_branch_info, opts, &rev);
		argv += n;
		argc -= n;
	} ଅନ୍ୟଥା ଯଦି (!opts->accept_ref && opts->from_treeish) {
		ସଂରଚନା object_id rev;

		ଯଦି (repo_get_oid_mb(the_repository, opts->from_treeish, &rev))
			die(_("could not resolve '%s'"), opts->from_treeish);

		setup_new_branch_info_and_source_ବୃକ୍ଷ(&new_branch_info,
						      opts, &rev,
						      opts->from_treeish);

		ଯଦି (!opts->source_ବୃକ୍ଷ)
			die(_("reference is not a tree: %s"), opts->from_treeish);
	}

	ଯଦି (argc) {
		parse_pathspec(&opts->pathspec, 0,
			       opts->patch_mode ? PATHSPEC_PREFIX_ORIGIN : 0,
			       prefix, argv);

		ଯଦି (!opts->pathspec.nr)
			die(_("invalid path specification"));

		/*
		 * Try to give more helpful suggestion.
		 * new_branch && argc > 1 will be caught later.
		 */
		ଯଦି (opts->new_branch && argc == 1 && !new_branch_info.commit)
			die(_("'%s' is not a commit and a branch '%s' cannot be created from it"),
				argv[0], opts->new_branch);

		ଯଦି (opts->force_detach)
			die(_("git checkout: --detach does not take a path argument '%s'"),
			    argv[0]);
	}

	ଯଦି (opts->pathspec_from_file) {
		ଯଦି (opts->pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		ଯଦି (opts->force_detach)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--detach");

		ଯଦି (opts->patch_mode)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--patch");

		parse_pathspec_file(&opts->pathspec, 0,
				    0,
				    prefix, opts->pathspec_from_file, opts->pathspec_file_nul);
	} ଅନ୍ୟଥା ଯଦି (opts->pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	opts->pathspec.recursive = 1;

	ଯଦି (opts->pathspec.nr) {
		ଯଦି (1 < !!opts->writeout_stage + !!opts->force + !!opts->merge)
			die(_("git checkout: --ours/--theirs, --force and --merge are incompatible when\n"
			      "checking out of the index."));
	} ଅନ୍ୟଥା {
		ଯଦି (opts->accept_pathspec && !opts->empty_pathspec_ok &&
		    !opts->patch_mode)	/* patch mode is special */
			die(_("you must specify path(s) to restore"));
	}

	ଯଦି (opts->new_branch) {
		ସଂରଚନା strbuf buf = STRBUF_INIT;

		ଯଦି (opts->new_branch_force)
			opts->branch_exists = validate_branchname(opts->new_branch, &buf);
		ଅନ୍ୟଥା
			opts->branch_exists =
				validate_new_branchname(opts->new_branch, &buf, 0);
		strbuf_release(&buf);
	}

	ଯଦି (opts->patch_mode || opts->pathspec.nr)
		ret = checkout_paths(opts, &new_branch_info);
	ଅନ୍ୟଥା
		ret = checkout_branch(opts, &new_branch_info);

	branch_info_release(&new_branch_info);
	clear_pathspec(&opts->pathspec);
	ମୁକ୍ତ(opts->pathspec_from_file);
	ମୁକ୍ତ(options);

	ଫେରନ୍ତୁ ret;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_checkout(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
		 ଅଚଳ ଅକ୍ଷର **argv,
		 ଅଚଳ ଅକ୍ଷର *prefix,
		 ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା checkout_opts opts = CHECKOUT_OPTS_INIT;
	ସଂରଚନା option *options;
	ସଂରଚନା option checkout_options[] = {
		OPT_STRING('b', ଖାଲି, &opts.new_branch, N_("branch"),
			   N_("create and checkout a new branch")),
		OPT_STRING('B', ଖାଲି, &opts.new_branch_force, N_("branch"),
			   N_("create/reset and checkout a branch")),
		OPT_BOOL('l', ଖାଲି, &opts.new_branch_log, N_("create reflog for new branch")),
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
	opts.orphan_from_empty_ବୃକ୍ଷ = 0;
	opts.empty_pathspec_ok = 1;
	opts.overlay_mode = -1;
	opts.checkout_index = -2;    /* default on */
	opts.checkout_worktree = -2; /* default on */

	ଯଦି (argc == 3 && !strcmp(argv[1], "-b")) {
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

	ଫେରନ୍ତୁ checkout_main(argc, argv, prefix, &opts, options,
			     CHECKOUT_CHECKOUT);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_switch(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
	       ଅଚଳ ଅକ୍ଷର **argv,
	       ଅଚଳ ଅକ୍ଷର *prefix,
	       ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା checkout_opts opts = CHECKOUT_OPTS_INIT;
	ସଂରଚନା option *options = ଖାଲି;
	ସଂରଚନା option switch_options[] = {
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
	opts.orphan_from_empty_ବୃକ୍ଷ = 1;
	opts.overlay_mode = -1;

	options = parse_options_dup(switch_options);
	options = add_common_options(&opts, options);
	options = add_common_switch_branch_options(&opts, options);

	cb_option = 'c';

	ଫେରନ୍ତୁ checkout_main(argc, argv, prefix, &opts, options,
			     CHECKOUT_SWITCH);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_restore(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
		ଅଚଳ ଅକ୍ଷର **argv,
		ଅଚଳ ଅକ୍ଷର *prefix,
		ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା checkout_opts opts = CHECKOUT_OPTS_INIT;
	ସଂରଚନା option *options;
	ସଂରଚନା option restore_options[] = {
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

	ଫେରନ୍ତୁ checkout_main(argc, argv, prefix, &opts, options,
			     CHECKOUT_RESTORE);
}
