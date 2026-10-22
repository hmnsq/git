#घोषणा USE_THE_REPOSITORY_VARIABLE
#घोषणा DISABLE_SIGN_COMPARE_WARNINGS

#समावेश "builtin.h"
#समावेश "advice.h"
#समावेश "branch.h"
#समावेश "cache-tree.h"
#समावेश "checkout.h"
#समावेश "commit.h"
#समावेश "config.h"
#समावेश "diff.h"
#समावेश "dir.h"
#समावेश "environment.h"
#समावेश "gettext.h"
#समावेश "hex.h"
#समावेश "hook.h"
#समावेश "merge-ll.h"
#समावेश "lockfile.h"
#समावेश "mem-pool.h"
#समावेश "object-file.h"
#समावेश "object-name.h"
#समावेश "odb.h"
#समावेश "parse-options.h"
#समावेश "path.h"
#समावेश "preload-index.h"
#समावेश "read-cache.h"
#समावेश "refs.h"
#समावेश "remote.h"
#समावेश "repo-settings.h"
#समावेश "resolve-undo.h"
#समावेश "revision.h"
#समावेश "sequencer.h"
#समावेश "setup.h"
#समावेश "sparse-index.h"
#समावेश "strvec.h"
#समावेश "submodule.h"
#समावेश "symlinks.h"
#समावेश "trace2.h"
#समावेश "tree.h"
#समावेश "tree-walk.h"
#समावेश "unpack-trees.h"
#समावेश "wt-status.h"
#समावेश "xdiff-interface.h"
#समावेश "entry.h"
#समावेश "parallel-checkout.h"
#समावेश "add-interactive.h"

काष्ठा checkout_opts {
	पूर्णांक patch_mode;
	पूर्णांक patch_context;
	पूर्णांक patch_interhunk_context;
	पूर्णांक auto_advance;
	पूर्णांक quiet;
	पूर्णांक merge;
	पूर्णांक force;
	पूर्णांक force_detach;
	पूर्णांक implicit_detach;
	पूर्णांक writeout_stage;
	पूर्णांक overwrite_ignore;
	पूर्णांक ignore_skipworktree;
	पूर्णांक ignore_other_worktrees;
	पूर्णांक show_progress;
	पूर्णांक गिनती_checkout_paths;
	पूर्णांक overlay_mode;
	पूर्णांक dwim_new_local_branch;
	पूर्णांक discard_changes;
	पूर्णांक accept_ref;
	पूर्णांक accept_pathspec;
	पूर्णांक switch_branch_doing_nothing_is_ok;
	पूर्णांक only_merge_on_switching_branches;
	पूर्णांक can_switch_when_in_progress;
	पूर्णांक orphan_from_empty_वृक्ष;
	पूर्णांक empty_pathspec_ok;
	पूर्णांक checkout_सूचकांक;
	पूर्णांक checkout_worktree;
	स्थिर अक्षर *ignore_unmerged_opt;
	पूर्णांक ignore_unmerged;
	पूर्णांक pathspec_file_nul;
	अक्षर *pathspec_from_file;

	स्थिर अक्षर *new_branch;
	स्थिर अक्षर *new_branch_force;
	स्थिर अक्षर *new_orphan_branch;
	पूर्णांक new_branch_log;
	क्रमागत branch_track track;
	काष्ठा diff_options diff_options;
	पूर्णांक conflict_style;

	पूर्णांक branch_exists;
	स्थिर अक्षर *prefix;
	काष्ठा pathspec pathspec;
	स्थिर अक्षर *from_treeish;
	काष्ठा वृक्ष *source_वृक्ष;
};

#घोषणा CHECKOUT_OPTS_INIT { \
	.conflict_style = -1, \
	.merge = -1, \
	.patch_context = -1, \
	.patch_interhunk_context = -1, \
	.auto_advance = 1, \
}

#घोषणा MERGE_WORKING_वृक्ष_UNPACK_FAILED (-2)

काष्ठा branch_info {
	अक्षर *name; /* The short name used */
	अक्षर *path; /* The full name of a real branch */
	काष्ठा commit *commit; /* The named commit */
	अक्षर *refname; /* The full name of the ref being checked out. */
	काष्ठा object_id oid; /* The object ID of the commit being checked out. */
	/*
	 * if not null the branch is detached because it's already
	 * checked out in this checkout
	 */
	अक्षर *checkout;
};

अटल व्योम branch_info_release(काष्ठा branch_info *info)
{
	मुक्त(info->name);
	मुक्त(info->path);
	मुक्त(info->refname);
	मुक्त(info->checkout);
}

अटल पूर्णांक post_checkout_hook(काष्ठा commit *old_commit, काष्ठा commit *new_commit,
			      पूर्णांक changed)
{
	काष्ठा run_hooks_opt opt = RUN_HOOKS_OPT_INIT_FORCE_SERIAL;

	/*
	 * "new_commit" can be NULL when checking out from the index before
	 * a commit exists.
	 */
	strvec_pushl(&opt.args,
		     oid_to_hex(old_commit ? &old_commit->object.oid : null_oid(the_hash_algo)),
		     oid_to_hex(new_commit ? &new_commit->object.oid : null_oid(the_hash_algo)),
		     changed ? "1" : "0",
		     शून्य);

	वापस run_hooks_opt(the_repository, "post-checkout", &opt);
}

/*
 * Handle a tree object and determine if we need to recurse into the
 * tree (READ_TREE_RECURSIVE) or skip it (0).
 */
अटल पूर्णांक try_update_sparse_directory(स्थिर काष्ठा object_id *oid,
				       काष्ठा strbuf *base,
				       स्थिर अक्षर *pathname,
				       पूर्णांक overlay_mode)
{
	काष्ठा strbuf dirpath = STRBUF_INIT;
	काष्ठा cache_entry *old;
	पूर्णांक pos, परिणाम = READ_वृक्ष_RECURSIVE;

	अगर (!the_repository->सूचकांक->sparse_सूचकांक)
		वापस परिणाम;

	strbuf_addbuf(&dirpath, base);
	strbuf_addstr(&dirpath, pathname);
	strbuf_addch(&dirpath, '/');

	pos = सूचकांक_name_pos_sparse(the_repository->सूचकांक,
				    dirpath.buf, dirpath.len);
	अगर (pos < 0)
		जाओ cleanup;

	old = the_repository->सूचकांक->cache[pos];
	अगर (!S_ISSPARSEDIR(old->ce_mode))
		जाओ cleanup;

	अगर (oideq(oid, &old->oid)) {
		/* Tree content already matches; no need to descend. */
		परिणाम = 0;
	} अन्यथा अगर (!overlay_mode) {
		/*
		 * In non-overlay mode (e.g., restore --staged), replace the
		 * sparse directory OID directly since files not present in
		 * the source tree should be removed anyway.
		 */
		oidcpy(&old->oid, oid);
		old->ce_flags |= CE_UPDATE;
		परिणाम = 0;
	}

cleanup:
	strbuf_release(&dirpath);
	वापस परिणाम;
}

अटल पूर्णांक update_some(स्थिर काष्ठा object_id *oid, काष्ठा strbuf *base,
		       स्थिर अक्षर *pathname, अचिन्हित mode, व्योम *context)
{
	पूर्णांक len;
	काष्ठा cache_entry *ce;
	पूर्णांक pos;
	पूर्णांक overlay_mode = context ? *((पूर्णांक *)context) : 1;

	अगर (S_ISDIR(mode))
		वापस try_update_sparse_directory(oid, base, pathname,
						   overlay_mode);

	len = base->len + म_माप(pathname);
	ce = make_empty_cache_entry(the_repository->सूचकांक, len);
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
	pos = सूचकांक_name_pos_sparse(the_repository->सूचकांक, ce->name, ce->ce_namelen);
	अगर (pos >= 0) {
		काष्ठा cache_entry *old = the_repository->सूचकांक->cache[pos];
		अगर (ce->ce_mode == old->ce_mode &&
		    !ce_intent_to_add(old) &&
		    oideq(&ce->oid, &old->oid)) {
			old->ce_flags |= CE_UPDATE;
			discard_cache_entry(ce);
			वापस 0;
		}
	}

	add_सूचकांक_entry(the_repository->सूचकांक, ce,
			ADD_CACHE_OK_TO_ADD | ADD_CACHE_OK_TO_REPLACE);
	वापस 0;
}

अटल पूर्णांक read_वृक्ष_some(काष्ठा वृक्ष *वृक्ष, स्थिर काष्ठा pathspec *pathspec,
			  पूर्णांक overlay_mode)
{
	read_वृक्ष(the_repository, वृक्ष,
		  pathspec, update_some, &overlay_mode);

	/* update the index with the given tree's info
	 * for all args, expanding wildcards, and exit
	 * with any non-zero return code.
	 */
	वापस 0;
}

अटल पूर्णांक skip_same_name(स्थिर काष्ठा cache_entry *ce, पूर्णांक pos)
{
	जबतक (++pos < the_repository->सूचकांक->cache_nr &&
	       !म_भेद(the_repository->सूचकांक->cache[pos]->name, ce->name))
		; /* skip */
	वापस pos;
}

अटल पूर्णांक check_stage(पूर्णांक stage, स्थिर काष्ठा cache_entry *ce, पूर्णांक pos,
		       पूर्णांक overlay_mode)
{
	जबतक (pos < the_repository->सूचकांक->cache_nr &&
	       !म_भेद(the_repository->सूचकांक->cache[pos]->name, ce->name)) {
		अगर (ce_stage(the_repository->सूचकांक->cache[pos]) == stage)
			वापस 0;
		pos++;
	}
	अगर (!overlay_mode)
		वापस 0;
	अगर (stage == 2)
		वापस error(_("path '%s' does not have our version"), ce->name);
	अन्यथा
		वापस error(_("path '%s' does not have their version"), ce->name);
}

अटल पूर्णांक check_stages(अचिन्हित stages, स्थिर काष्ठा cache_entry *ce, पूर्णांक pos)
{
	अचिन्हित seen = 0;
	स्थिर अक्षर *name = ce->name;

	जबतक (pos < the_repository->सूचकांक->cache_nr) {
		ce = the_repository->सूचकांक->cache[pos];
		अगर (म_भेद(name, ce->name))
			अवरोध;
		seen |= (1 << ce_stage(ce));
		pos++;
	}
	अगर ((stages & seen) != stages)
		वापस error(_("path '%s' does not have all necessary versions"),
			     name);
	वापस 0;
}

अटल पूर्णांक checkout_stage(पूर्णांक stage, स्थिर काष्ठा cache_entry *ce, पूर्णांक pos,
			  स्थिर काष्ठा checkout *state, पूर्णांक *nr_checkouts,
			  पूर्णांक overlay_mode)
{
	जबतक (pos < the_repository->सूचकांक->cache_nr &&
	       !म_भेद(the_repository->सूचकांक->cache[pos]->name, ce->name)) {
		अगर (ce_stage(the_repository->सूचकांक->cache[pos]) == stage)
			वापस checkout_entry(the_repository->सूचकांक->cache[pos], state,
					      शून्य, nr_checkouts);
		pos++;
	}
	अगर (!overlay_mode) {
		unlink_entry(ce, शून्य);
		वापस 0;
	}
	अगर (stage == 2)
		वापस error(_("path '%s' does not have our version"), ce->name);
	अन्यथा
		वापस error(_("path '%s' does not have their version"), ce->name);
}

अटल पूर्णांक checkout_merged(पूर्णांक pos, स्थिर काष्ठा checkout *state,
			   पूर्णांक *nr_checkouts, काष्ठा mem_pool *ce_mem_pool,
			   पूर्णांक conflict_style)
{
	काष्ठा cache_entry *ce = the_repository->सूचकांक->cache[pos];
	स्थिर अक्षर *path = ce->name;
	mmfile_t ancestor, ours, theirs;
	क्रमागत ll_merge_परिणाम merge_status;
	पूर्णांक status;
	काष्ठा object_id oid;
	mmfile_t परिणाम_buf;
	काष्ठा object_id threeway[3];
	अचिन्हित mode = 0;
	काष्ठा ll_merge_options ll_opts = LL_MERGE_OPTIONS_INIT;
	पूर्णांक renormalize = 0;

	स_रखो(threeway, 0, माप(threeway));
	जबतक (pos < the_repository->सूचकांक->cache_nr) {
		पूर्णांक stage;
		stage = ce_stage(ce);
		अगर (!stage || म_भेद(path, ce->name))
			अवरोध;
		oidcpy(&threeway[stage - 1], &ce->oid);
		अगर (stage == 2)
			mode = create_ce_mode(ce->ce_mode);
		pos++;
		ce = the_repository->सूचकांक->cache[pos];
	}
	अगर (is_null_oid(&threeway[1]) || is_null_oid(&threeway[2]))
		वापस error(_("path '%s' does not have necessary versions"), path);

	read_mmblob(&ancestor, the_repository->objects, &threeway[0]);
	read_mmblob(&ours, the_repository->objects, &threeway[1]);
	read_mmblob(&theirs, the_repository->objects, &threeway[2]);

	repo_config_get_bool(the_repository, "merge.renormalize", &renormalize);
	ll_opts.renormalize = renormalize;
	ll_opts.conflict_style = conflict_style;
	merge_status = ll_merge(&परिणाम_buf, path, &ancestor, "base",
				&ours, "ours", &theirs, "theirs",
				state->istate, &ll_opts);
	मुक्त(ancestor.ptr);
	मुक्त(ours.ptr);
	मुक्त(theirs.ptr);
	अगर (merge_status == LL_MERGE_द्विआधारी_CONFLICT)
		warning("Cannot merge binary files: %s (%s vs. %s)",
			path, "ours", "theirs");
	अगर (merge_status < 0 || !परिणाम_buf.ptr) {
		मुक्त(परिणाम_buf.ptr);
		वापस error(_("path '%s': cannot merge"), path);
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
	अगर (odb_write_object(the_repository->objects, परिणाम_buf.ptr, परिणाम_buf.आकार, OBJ_BLOB, &oid))
		die(_("Unable to add merge result for '%s'"), path);
	मुक्त(परिणाम_buf.ptr);
	ce = make_transient_cache_entry(mode, &oid, path, 2, ce_mem_pool);
	अगर (!ce)
		die(_("make_cache_entry failed for path '%s'"), path);
	status = checkout_entry(ce, state, शून्य, nr_checkouts);
	वापस status;
}

अटल व्योम mark_ce_for_checkout_overlay(काष्ठा cache_entry *ce,
					 अक्षर *ps_matched,
					 स्थिर काष्ठा checkout_opts *opts)
{
	ce->ce_flags &= ~CE_MATCHED;
	अगर (!opts->ignore_skipworktree && ce_skip_worktree(ce))
		वापस;
	अगर (opts->source_वृक्ष && !(ce->ce_flags & CE_UPDATE))
		/*
		 * "git checkout tree-ish -- path", but this entry
		 * is in the original index but is not in tree-ish
		 * or does not match the pathspec; it will not be
		 * checked out to the working tree.  We will not do
		 * anything to this entry at all.
		 */
		वापस;
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
	अगर (ce_path_match(the_repository->सूचकांक, ce, &opts->pathspec, ps_matched))
		ce->ce_flags |= CE_MATCHED;
}

अटल व्योम mark_ce_for_checkout_no_overlay(काष्ठा cache_entry *ce,
					    अक्षर *ps_matched,
					    स्थिर काष्ठा checkout_opts *opts)
{
	ce->ce_flags &= ~CE_MATCHED;
	अगर (!opts->ignore_skipworktree && ce_skip_worktree(ce))
		वापस;
	अगर (ce_path_match(the_repository->सूचकांक, ce, &opts->pathspec, ps_matched)) {
		ce->ce_flags |= CE_MATCHED;
		अगर (opts->source_वृक्ष && !(ce->ce_flags & CE_UPDATE))
			/*
			 * In overlay mode, but the path is not in
			 * tree-ish, which means we should remove it
			 * from the index and the working tree.
			 */
			ce->ce_flags |= CE_REMOVE | CE_WT_REMOVE;
	}
}

अटल पूर्णांक checkout_worktree(स्थिर काष्ठा checkout_opts *opts,
			     स्थिर काष्ठा branch_info *info)
{
	काष्ठा checkout state = CHECKOUT_INIT;
	पूर्णांक nr_checkouts = 0, nr_unmerged = 0;
	पूर्णांक errs = 0;
	पूर्णांक pos;
	पूर्णांक pc_workers, pc_threshold;
	काष्ठा mem_pool ce_mem_pool;

	state.force = 1;
	state.refresh_cache = 1;
	state.istate = the_repository->सूचकांक;

	mem_pool_init(&ce_mem_pool, 0);
	get_parallel_checkout_configs(&pc_workers, &pc_threshold);
	init_checkout_metadata(&state.meta, info->refname,
			       info->commit ? &info->commit->object.oid : &info->oid,
			       शून्य);

	enable_delayed_checkout(&state);

	अगर (pc_workers > 1)
		init_parallel_checkout();

	क्रम (pos = 0; pos < the_repository->सूचकांक->cache_nr; pos++) {
		काष्ठा cache_entry *ce = the_repository->सूचकांक->cache[pos];
		अगर (ce->ce_flags & CE_MATCHED) {
			अगर (!ce_stage(ce)) {
				errs |= checkout_entry(ce, &state,
						       शून्य, &nr_checkouts);
				जारी;
			}
			अगर (opts->writeout_stage)
				errs |= checkout_stage(opts->writeout_stage,
						       ce, pos,
						       &state,
						       &nr_checkouts, opts->overlay_mode);
			अन्यथा अगर (opts->merge)
				errs |= checkout_merged(pos, &state,
							&nr_unmerged,
							&ce_mem_pool,
							opts->conflict_style);
			pos = skip_same_name(ce, pos) - 1;
		}
	}
	अगर (pc_workers > 1)
		errs |= run_parallel_checkout(&state, pc_workers, pc_threshold,
					      शून्य, शून्य);
	mem_pool_discard(&ce_mem_pool, should_validate_cache_entries());
	remove_marked_cache_entries(the_repository->सूचकांक, 1);
	remove_scheduled_dirs();
	errs |= finish_delayed_checkout(&state, opts->show_progress);

	अगर (opts->गिनती_checkout_paths) {
		अगर (nr_unmerged)
			fprintf_ln(मानक_त्रुटि, Q_("Recreated %d merge conflict",
					      "Recreated %d merge conflicts",
					      nr_unmerged),
				   nr_unmerged);
		अगर (opts->source_वृक्ष)
			fprintf_ln(मानक_त्रुटि, Q_("Updated %d path from %s",
					      "Updated %d paths from %s",
					      nr_checkouts),
				   nr_checkouts,
				   repo_find_unique_abbrev(the_repository, &opts->source_वृक्ष->object.oid,
							   DEFAULT_ABBREV));
		अन्यथा अगर (!nr_unmerged || nr_checkouts)
			fprintf_ln(मानक_त्रुटि, Q_("Updated %d path from the index",
					      "Updated %d paths from the index",
					      nr_checkouts),
				   nr_checkouts);
	}

	वापस errs;
}

अटल पूर्णांक checkout_paths(स्थिर काष्ठा checkout_opts *opts,
			  स्थिर काष्ठा branch_info *new_branch_info)
{
	पूर्णांक pos;
	अटल अक्षर *ps_matched;
	काष्ठा object_id rev;
	काष्ठा commit *शीर्ष;
	पूर्णांक errs = 0;
	काष्ठा lock_file lock_file = LOCK_INIT;
	पूर्णांक checkout_सूचकांक;

	trace2_cmd_mode(opts->patch_mode ? "patch" : "path");

	अगर (opts->track != BRANCH_TRACK_UNSPECIFIED)
		die(_("'%s' cannot be used with updating paths"), "--track");

	अगर (opts->new_branch_log)
		die(_("'%s' cannot be used with updating paths"), "-l");

	अगर (opts->ignore_unmerged && opts->patch_mode)
		die(_("'%s' cannot be used with updating paths"),
		    opts->ignore_unmerged_opt);

	अगर (opts->force_detach)
		die(_("'%s' cannot be used with updating paths"), "--detach");

	अगर (opts->merge && opts->patch_mode)
		die(_("options '%s' and '%s' cannot be used together"), "--merge", "--patch");

	अगर (opts->ignore_unmerged && opts->merge)
		die(_("options '%s' and '%s' cannot be used together"),
		    opts->ignore_unmerged_opt, "-m");

	अगर (opts->new_branch)
		die(_("Cannot update paths and switch to branch '%s' at the same time."),
		    opts->new_branch);

	अगर (!opts->checkout_worktree && !opts->checkout_सूचकांक)
		die(_("neither '%s' or '%s' is specified"),
		    "--staged", "--worktree");

	अगर (!opts->checkout_worktree && !opts->from_treeish)
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
	अगर (!opts->accept_ref && opts->checkout_सूचकांक) {
		अगर (opts->writeout_stage)
			die(_("'%s' or '%s' cannot be used with %s"),
			    "--ours", "--theirs", "--staged");

		अगर (opts->merge)
			die(_("'%s' or '%s' cannot be used with %s"),
			    "--merge", "--conflict", "--staged");
	}

	/*
	 * recreating unmerged index entries and writing out data from
	 * unmerged index entries would make no sense when checking out
	 * of a tree-ish.
	 */
	अगर ((opts->merge || opts->writeout_stage) && opts->source_वृक्ष)
		die(_("'%s', '%s', or '%s' cannot be used when checking out of a tree"),
		    "--merge", "--ours", "--theirs");

	अगर (opts->patch_mode) {
		क्रमागत add_p_mode patch_mode;
		काष्ठा interactive_options interactive_opts = {
			.context = opts->patch_context,
			.interhunkcontext = opts->patch_interhunk_context,
			.auto_advance = opts->auto_advance
		};
		स्थिर अक्षर *rev = new_branch_info->name;
		अक्षर rev_oid[GIT_MAX_HEXSZ + 1];

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
		अगर (rev && new_branch_info->commit && म_भेद(rev, "HEAD"))
			rev = oid_to_hex_r(rev_oid, &new_branch_info->commit->object.oid);

		अगर (opts->checkout_सूचकांक && opts->checkout_worktree)
			patch_mode = ADD_P_CHECKOUT;
		अन्यथा अगर (opts->checkout_सूचकांक && !opts->checkout_worktree)
			patch_mode = ADD_P_RESET;
		अन्यथा अगर (!opts->checkout_सूचकांक && opts->checkout_worktree)
			patch_mode = ADD_P_WORKTREE;
		अन्यथा
			BUG("either flag must have been set, worktree=%d, index=%d",
			    opts->checkout_worktree, opts->checkout_सूचकांक);
		वापस !!run_add_p(the_repository, patch_mode, &interactive_opts,
				   rev, &opts->pathspec, 0);
	}

	repo_hold_locked_सूचकांक(the_repository, &lock_file, LOCK_DIE_ON_ERROR);
	अगर (repo_read_सूचकांक_preload(the_repository, &opts->pathspec, 0) < 0)
		वापस error(_("index file corrupt"));

	अगर (opts->source_वृक्ष)
		read_वृक्ष_some(opts->source_वृक्ष, &opts->pathspec,
			       opts->overlay_mode);
	अगर (opts->merge)
		unmerge_सूचकांक(the_repository->सूचकांक, &opts->pathspec, CE_MATCHED);

	ps_matched = xcalloc(opts->pathspec.nr, 1);

	/*
	 * Make sure all pathspecs participated in locating the paths
	 * to be checked out.
	 */
	क्रम (pos = 0; pos < the_repository->सूचकांक->cache_nr; pos++)
		अगर (opts->overlay_mode)
			mark_ce_for_checkout_overlay(the_repository->सूचकांक->cache[pos],
						     ps_matched,
						     opts);
		अन्यथा
			mark_ce_for_checkout_no_overlay(the_repository->सूचकांक->cache[pos],
							ps_matched,
							opts);

	अगर (report_path_error(ps_matched, &opts->pathspec)) {
		मुक्त(ps_matched);
		वापस 1;
	}
	मुक्त(ps_matched);

	/* Any unmerged paths? */
	क्रम (pos = 0; pos < the_repository->सूचकांक->cache_nr; pos++) {
		स्थिर काष्ठा cache_entry *ce = the_repository->सूचकांक->cache[pos];
		अगर (ce->ce_flags & CE_MATCHED) {
			अगर (!ce_stage(ce))
				जारी;
			अगर (opts->ignore_unmerged) {
				अगर (!opts->quiet)
					warning(_("path '%s' is unmerged"), ce->name);
			} अन्यथा अगर (opts->writeout_stage) {
				errs |= check_stage(opts->writeout_stage, ce, pos, opts->overlay_mode);
			} अन्यथा अगर (opts->merge) {
				errs |= check_stages((1<<2) | (1<<3), ce, pos);
			} अन्यथा {
				errs = 1;
				error(_("path '%s' is unmerged"), ce->name);
			}
			pos = skip_same_name(ce, pos) - 1;
		}
	}
	अगर (errs)
		वापस 1;

	/* Now we are committed to check them out */
	अगर (opts->checkout_worktree)
		errs |= checkout_worktree(opts, new_branch_info);
	अन्यथा
		remove_marked_cache_entries(the_repository->सूचकांक, 1);

	/*
	 * Allow updating the index when checking out from the index.
	 * This is to save new stat info.
	 */
	अगर (opts->checkout_worktree && !opts->checkout_सूचकांक && !opts->source_वृक्ष)
		checkout_सूचकांक = 1;
	अन्यथा
		checkout_सूचकांक = opts->checkout_सूचकांक;

	अगर (checkout_सूचकांक) {
		अगर (write_locked_सूचकांक(the_repository->सूचकांक, &lock_file, COMMIT_LOCK))
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
			   &rev, शून्य);
	शीर्ष = lookup_commit_reference_gently(the_repository, &rev, 1);

	errs |= post_checkout_hook(शीर्ष, शीर्ष, 0);
	वापस errs;
}

अटल व्योम show_local_changes(काष्ठा object *शीर्ष,
			       स्थिर काष्ठा diff_options *opts)
{
	काष्ठा rev_info rev;
	/* I think we want full paths, even if we're in a subdirectory. */
	repo_init_revisions(the_repository, &rev, शून्य);
	rev.diffopt.flags = opts->flags;
	rev.diffopt.output_format |= DIFF_FORMAT_NAME_STATUS;
	rev.diffopt.flags.recursive = 1;
	diff_setup_done(&rev.diffopt);
	add_pending_object(&rev, शीर्ष, शून्य);
	run_diff_सूचकांक(&rev, 0);
	release_revisions(&rev);
}

अटल व्योम describe_detached_शीर्ष(स्थिर अक्षर *संदेश, काष्ठा commit *commit)
{
	काष्ठा strbuf sb = STRBUF_INIT;

	अगर (!repo_parse_commit(the_repository, commit))
		pp_commit_easy(CMIT_FMT_ONELINE, commit, &sb);
	अगर (print_sha1_ellipsis()) {
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

अटल पूर्णांक reset_वृक्ष(काष्ठा वृक्ष *वृक्ष, स्थिर काष्ठा checkout_opts *o,
		      पूर्णांक worktree, पूर्णांक *writeout_error,
		      काष्ठा branch_info *info)
{
	काष्ठा unpack_trees_options opts;
	काष्ठा वृक्ष_desc वृक्ष_desc;

	स_रखो(&opts, 0, माप(opts));
	opts.शीर्ष_idx = -1;
	opts.update = worktree;
	opts.skip_unmerged = !worktree;
	opts.reset = o->force ? UNPACK_RESET_OVERWRITE_UNTRACKED :
				UNPACK_RESET_PROTECT_UNTRACKED;
	opts.preserve_ignored = (!o->force && !o->overwrite_ignore);
	opts.merge = 1;
	opts.fn = oneway_merge;
	opts.verbose_update = o->show_progress;
	opts.src_सूचकांक = the_repository->सूचकांक;
	opts.dst_सूचकांक = the_repository->सूचकांक;
	init_checkout_metadata(&opts.meta, info->refname,
			       info->commit ? &info->commit->object.oid : null_oid(the_hash_algo),
			       शून्य);
	अगर (repo_parse_वृक्ष(the_repository, वृक्ष) < 0)
		वापस 128;
	init_वृक्ष_desc(&वृक्ष_desc, &वृक्ष->object.oid, वृक्ष->बफर, वृक्ष->आकार);
	चयन (unpack_trees(1, &वृक्ष_desc, &opts)) {
	हाल -2:
		*writeout_error = 1;
		/*
		 * We return 0 nevertheless, as the index is all right
		 * and more importantly we have made best efforts to
		 * update paths in the work tree, and we cannot revert
		 * them.
		 */
		/* fallthrough */
	हाल 0:
		वापस 0;
	शेष:
		वापस 128;
	}
}

अटल व्योम setup_branch_path(काष्ठा branch_info *branch)
{
	काष्ठा strbuf buf = STRBUF_INIT;

	/*
	 * If this is a ref, resolve it; otherwise, look up the OID for our
	 * expression.  Failure here is okay.
	 */
	अगर (!repo_dwim_ref(the_repository, branch->name, म_माप(branch->name),
			   &branch->oid, &branch->refname, 0))
		repo_get_oid_committish(the_repository, branch->name, &branch->oid);

	copy_branchname(the_repository, &buf, branch->name, INTERPRET_BRANCH_LOCAL);
	अगर (म_भेद(buf.buf, branch->name)) {
		मुक्त(branch->name);
		branch->name = xstrdup(buf.buf);
	}
	strbuf_splice(&buf, 0, 0, "refs/heads/", 11);
	मुक्त(branch->path);
	branch->path = strbuf_detach(&buf, शून्य);
}

अटल व्योम init_topts(काष्ठा unpack_trees_options *topts,
		       पूर्णांक show_progress, पूर्णांक overwrite_ignore,
		       bool quiet)
{
	स_रखो(topts, 0, माप(*topts));
	topts->शीर्ष_idx = -1;
	topts->src_सूचकांक = the_repository->सूचकांक;
	topts->dst_सूचकांक = the_repository->सूचकांक;

	setup_unpack_trees_porcelain(topts, "checkout");

	topts->initial_checkout = is_सूचकांक_unborn(the_repository->सूचकांक);
	topts->update = 1;
	topts->merge = 1;
	topts->quiet = quiet;
	topts->verbose_update = show_progress;
	topts->fn = twoway_merge;
	topts->preserve_ignored = !overwrite_ignore;
}

अटल पूर्णांक merge_working_वृक्ष(स्थिर काष्ठा checkout_opts *opts,
			      काष्ठा branch_info *old_branch_info,
			      काष्ठा branch_info *new_branch_info,
			      bool quiet,
			      पूर्णांक *writeout_error)
{
	पूर्णांक ret;
	काष्ठा lock_file lock_file = LOCK_INIT;
	काष्ठा वृक्ष *new_वृक्ष;

	repo_hold_locked_सूचकांक(the_repository, &lock_file, LOCK_DIE_ON_ERROR);
	अगर (repo_read_सूचकांक_preload(the_repository, शून्य, 0) < 0) {
		rollback_lock_file(&lock_file);
		वापस error(_("index file corrupt"));
	}

	resolve_undo_clear_सूचकांक(the_repository->सूचकांक);
	अगर (opts->new_orphan_branch && opts->orphan_from_empty_वृक्ष) {
		अगर (new_branch_info->commit)
			BUG("'switch --orphan' should never accept a commit as starting point");
		new_वृक्ष = repo_parse_वृक्ष_indirect(the_repository,
						    the_hash_algo->empty_वृक्ष);
		अगर (!new_वृक्ष)
			BUG("unable to read empty tree");
	} अन्यथा {
		new_वृक्ष = repo_get_commit_वृक्ष(the_repository,
						new_branch_info->commit);
		अगर (!new_वृक्ष) {
			rollback_lock_file(&lock_file);
			वापस error(_("unable to read tree (%s)"),
				     oid_to_hex(&new_branch_info->commit->object.oid));
		}
	}
	अगर (opts->discard_changes) {
		ret = reset_वृक्ष(new_वृक्ष, opts, 1, writeout_error, new_branch_info);
		अगर (ret) {
			rollback_lock_file(&lock_file);
			वापस ret;
		}
	} अन्यथा {
		काष्ठा वृक्ष_desc trees[2];
		काष्ठा वृक्ष *वृक्ष;
		काष्ठा unpack_trees_options topts;
		स्थिर काष्ठा object_id *old_commit_oid;

		refresh_सूचकांक(the_repository->सूचकांक, REFRESH_QUIET, शून्य, शून्य, शून्य);

		अगर (unmerged_सूचकांक(the_repository->सूचकांक)) {
			rollback_lock_file(&lock_file);
			error(_("you need to resolve your current index first"));
			वापस 1;
		}

		/* 2-way merge to the new branch */
		init_topts(&topts, opts->show_progress,
			   opts->overwrite_ignore, quiet);
		init_checkout_metadata(&topts.meta, new_branch_info->refname,
				       new_branch_info->commit ?
				       &new_branch_info->commit->object.oid :
				       &new_branch_info->oid, शून्य);

		old_commit_oid = old_branch_info->commit ?
			&old_branch_info->commit->object.oid :
			the_hash_algo->empty_वृक्ष;
		वृक्ष = repo_parse_वृक्ष_indirect(the_repository,
						old_commit_oid);
		अगर (!वृक्ष)
			die(_("unable to parse commit %s"),
				oid_to_hex(old_commit_oid));

		init_वृक्ष_desc(&trees[0], &वृक्ष->object.oid,
			       वृक्ष->बफर, वृक्ष->आकार);
		अगर (repo_parse_वृक्ष(the_repository, new_वृक्ष) < 0)
			die(शून्य);
		वृक्ष = new_वृक्ष;
		init_वृक्ष_desc(&trees[1], &वृक्ष->object.oid,
			       वृक्ष->बफर, वृक्ष->आकार);

		ret = unpack_trees(2, trees, &topts);
		clear_unpack_trees_porcelain(&topts);
		अगर (ret == -1) {
			rollback_lock_file(&lock_file);
			वापस MERGE_WORKING_वृक्ष_UNPACK_FAILED;
		}
	}

	अगर (!cache_वृक्ष_fully_valid(the_repository->सूचकांक))
		cache_वृक्ष_update(the_repository->सूचकांक, WRITE_वृक्ष_SILENT | WRITE_वृक्ष_REPAIR);

	अगर (write_locked_सूचकांक(the_repository->सूचकांक, &lock_file, COMMIT_LOCK))
		die(_("unable to write new index file"));

	अगर (!opts->discard_changes && !opts->quiet && new_branch_info->commit)
		show_local_changes(&new_branch_info->commit->object, &opts->diff_options);

	वापस 0;
}

अटल व्योम report_tracking(काष्ठा branch_info *new_branch_info)
{
	काष्ठा strbuf sb = STRBUF_INIT;
	काष्ठा branch *branch = branch_get(new_branch_info->name);

	अगर (!format_tracking_info(branch, &sb, AHEAD_BEHIND_FULL, 1))
		वापस;
	ख_माला_दो(sb.buf, मानक_निकास);
	strbuf_release(&sb);
}

अटल व्योम update_refs_for_switch(स्थिर काष्ठा checkout_opts *opts,
				   काष्ठा branch_info *old_branch_info,
				   काष्ठा branch_info *new_branch_info)
{
	काष्ठा strbuf संदेश = STRBUF_INIT;
	स्थिर अक्षर *old_desc, *reflog_संदेश;
	अगर (opts->new_branch) {
		अगर (opts->new_orphan_branch) {
			क्रमागत log_refs_config log_all_ref_updates = LOG_REFS_UNSET;
			स्थिर अक्षर *मूल्य;
			अक्षर *refname;

			अगर (!repo_config_get_string_tmp(the_repository, "core.logallrefupdates", &मूल्य))
				log_all_ref_updates = refs_parse_log_all_ref_updates_config(मूल्य);

			refname = mkpathdup("refs/heads/%s", opts->new_orphan_branch);
			अगर (opts->new_branch_log &&
			    !should_autocreate_reflog(log_all_ref_updates, refname)) {
				पूर्णांक ret;
				काष्ठा strbuf err = STRBUF_INIT;

				ret = refs_create_reflog(get_main_ref_store(the_repository),
							 refname, &err);
				अगर (ret) {
					ख_लिखो(मानक_त्रुटि, _("Can not do reflog for '%s': %s\n"),
						opts->new_orphan_branch, err.buf);
					strbuf_release(&err);
					मुक्त(refname);
					वापस;
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
	अगर (!old_desc && old_branch_info->commit)
		old_desc = oid_to_hex(&old_branch_info->commit->object.oid);

	reflog_संदेश = दो_पर्या("GIT_REFLOG_ACTION");
	अगर (!reflog_संदेश)
		strbuf_addf(&संदेश, "checkout: moving from %s to %s",
			old_desc ? old_desc : "(invalid)", new_branch_info->name);
	अन्यथा
		strbuf_insertstr(&संदेश, 0, reflog_संदेश);

	अगर (!म_भेद(new_branch_info->name, "HEAD") && !new_branch_info->path && !opts->force_detach) {
		/* Nothing to do. */
	} अन्यथा अगर (opts->force_detach || !new_branch_info->path) {	/* No longer on any branch. */
		refs_update_ref(get_main_ref_store(the_repository), संदेश.buf,
				"HEAD", &new_branch_info->commit->object.oid,
				शून्य,
				REF_NO_DEREF, UPDATE_REFS_DIE_ON_ERR);
		अगर (!opts->quiet) {
			अगर (old_branch_info->path &&
			    advice_enabled(ADVICE_DETACHED_शीर्ष) && !opts->force_detach)
				detach_advice(new_branch_info->name);
			describe_detached_शीर्ष(_("HEAD is now at"), new_branch_info->commit);
		}
	} अन्यथा अगर (new_branch_info->path) {	/* Switch branches. */
		अगर (refs_update_symref(get_main_ref_store(the_repository), "HEAD", new_branch_info->path, संदेश.buf) < 0)
			die(_("unable to update HEAD"));
		अगर (!opts->quiet) {
			अगर (old_branch_info->path && !म_भेद(new_branch_info->path, old_branch_info->path)) {
				अगर (opts->new_branch_force)
					ख_लिखो(मानक_त्रुटि, _("Reset branch '%s'\n"),
						new_branch_info->name);
				अन्यथा
					ख_लिखो(मानक_त्रुटि, _("Already on '%s'\n"),
						new_branch_info->name);
			} अन्यथा अगर (opts->new_branch) {
				अगर (opts->branch_exists)
					ख_लिखो(मानक_त्रुटि, _("Switched to and reset branch '%s'\n"), new_branch_info->name);
				अन्यथा
					ख_लिखो(मानक_त्रुटि, _("Switched to a new branch '%s'\n"), new_branch_info->name);
			} अन्यथा {
				ख_लिखो(मानक_त्रुटि, _("Switched to branch '%s'\n"),
					new_branch_info->name);
			}
		}
		अगर (old_branch_info->path && old_branch_info->name) {
			अगर (!refs_ref_exists(get_main_ref_store(the_repository), old_branch_info->path) && refs_reflog_exists(get_main_ref_store(the_repository), old_branch_info->path))
				refs_delete_reflog(get_main_ref_store(the_repository),
						   old_branch_info->path);
		}
	}
	remove_branch_state(the_repository, !opts->quiet);
	strbuf_release(&संदेश);
	अगर (!opts->quiet &&
	    !opts->force_detach &&
	    (new_branch_info->path || !म_भेद(new_branch_info->name, "HEAD")))
		report_tracking(new_branch_info);
}

अटल पूर्णांक add_pending_uninteresting_ref(स्थिर काष्ठा reference *ref, व्योम *cb_तथ्य)
{
	add_pending_oid(cb_तथ्य, ref->name, ref->oid, UNINTERESTING);
	वापस 0;
}

अटल व्योम describe_one_orphan(काष्ठा strbuf *sb, काष्ठा commit *commit)
{
	strbuf_addstr(sb, "  ");
	strbuf_add_unique_abbrev(sb, &commit->object.oid, DEFAULT_ABBREV);
	strbuf_addch(sb, ' ');
	अगर (!repo_parse_commit(the_repository, commit))
		pp_commit_easy(CMIT_FMT_ONELINE, commit, sb);
	strbuf_addch(sb, '\n');
}

#घोषणा ORPHAN_CUTOFF 4
अटल व्योम suggest_reattach(काष्ठा commit *commit, काष्ठा rev_info *revs)
{
	काष्ठा commit *c, *last = शून्य;
	काष्ठा strbuf sb = STRBUF_INIT;
	पूर्णांक lost = 0;
	जबतक ((c = get_revision(revs)) != शून्य) {
		अगर (lost < ORPHAN_CUTOFF)
			describe_one_orphan(&sb, c);
		last = c;
		lost++;
	}
	अगर (ORPHAN_CUTOFF < lost) {
		पूर्णांक more = lost - ORPHAN_CUTOFF;
		अगर (more == 1)
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

	अगर (advice_enabled(ADVICE_DETACHED_शीर्ष))
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
अटल व्योम orphaned_commit_warning(काष्ठा commit *old_commit, काष्ठा commit *new_commit)
{
	काष्ठा rev_info revs;
	काष्ठा object *object = &old_commit->object;

	repo_init_revisions(the_repository, &revs, शून्य);
	setup_revisions(0, शून्य, &revs, शून्य);

	object->flags &= ~UNINTERESTING;
	add_pending_object(&revs, object, oid_to_hex(&object->oid));

	refs_for_each_ref(get_main_ref_store(the_repository),
			  add_pending_uninteresting_ref, &revs);
	अगर (new_commit)
		add_pending_oid(&revs, "HEAD",
				&new_commit->object.oid,
				UNINTERESTING);

	अगर (prepare_revision_walk(&revs))
		die(_("internal error in revision walk"));
	अगर (!(old_commit->object.flags & UNINTERESTING))
		suggest_reattach(old_commit, &revs);
	अन्यथा
		describe_detached_शीर्ष(_("Previous HEAD position was"), old_commit);

	/* Clean up objects used, as they will be reused. */
	repo_clear_commit_marks(the_repository, ALL_REV_FLAGS);
	release_revisions(&revs);
}

अटल पूर्णांक switch_branches(स्थिर काष्ठा checkout_opts *opts,
			   काष्ठा branch_info *new_branch_info)
{
	पूर्णांक ret = 0;
	काष्ठा branch_info old_branch_info = { 0 };
	काष्ठा object_id rev;
	पूर्णांक flag, writeout_error = 0;
	पूर्णांक do_merge = 1;
	पूर्णांक created_autostash = 0;
	क्रमागत stash_apply_परिणाम autostash_res = STASH_APPLY_CLEAN;
	काष्ठा strbuf old_commit_shortname = STRBUF_INIT;
	काष्ठा strbuf autostash_संदेश = STRBUF_INIT;
	स्थिर अक्षर *stash_label_base = शून्य;

	trace2_cmd_mode("branch");

	स_रखो(&old_branch_info, 0, माप(old_branch_info));
	old_branch_info.path = refs_resolve_refdup(get_main_ref_store(the_repository),
						   "HEAD", 0, &rev, &flag);
	अगर (old_branch_info.path)
		old_branch_info.commit = lookup_commit_reference_gently(the_repository, &rev, 1);
	अगर (!(flag & REF_ISSYMREF))
		FREE_AND_NULL(old_branch_info.path);

	अगर (old_branch_info.path) {
		स्थिर अक्षर *स्थिर prefix = "refs/heads/";
		स्थिर अक्षर *p;
		अगर (skip_prefix(old_branch_info.path, prefix, &p))
			old_branch_info.name = xstrdup(p);
	}

	अगर (opts->new_orphan_branch && opts->orphan_from_empty_वृक्ष) {
		अगर (new_branch_info->name)
			BUG("'switch --orphan' should never accept a commit as starting point");
		new_branch_info->commit = शून्य;
		new_branch_info->name = xstrdup("(empty)");
		do_merge = 1;
	}

	अगर (!new_branch_info->name) {
		new_branch_info->name = xstrdup("HEAD");
		new_branch_info->commit = old_branch_info.commit;
		अगर (!new_branch_info->commit)
			die(_("You are on a branch yet to be born"));
		parse_commit_or_die(new_branch_info->commit);

		अगर (opts->only_merge_on_switching_branches)
			do_merge = 0;
	}

	अगर (old_branch_info.name) {
		stash_label_base = old_branch_info.name;
	} अन्यथा अगर (old_branch_info.commit) {
		strbuf_add_unique_abbrev(&old_commit_shortname,
					 &old_branch_info.commit->object.oid,
					 DEFAULT_ABBREV);
		stash_label_base = old_commit_shortname.buf;
	}

	अगर (do_merge) {
		ret = merge_working_वृक्ष(opts, &old_branch_info, new_branch_info,
					 opts->merge, &writeout_error);
		अगर (ret == MERGE_WORKING_वृक्ष_UNPACK_FAILED && opts->merge) {
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
		अगर (created_autostash) {
			अगर (opts->conflict_style >= 0) {
				काष्ठा strbuf cfg = STRBUF_INIT;
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
		अगर (ret) {
			branch_info_release(&old_branch_info);
			strbuf_release(&old_commit_shortname);
			strbuf_release(&autostash_संदेश);
			वापस ret < 0 ? 1 : ret;
		}
	}

	अगर (!opts->quiet && !old_branch_info.path && old_branch_info.commit && new_branch_info->commit != old_branch_info.commit)
		orphaned_commit_warning(old_branch_info.commit, new_branch_info->commit);

	अगर (autostash_res == STASH_APPLY_CONFLICT && !opts->quiet)
		ख_अक्षर_दो('\n', मानक_त्रुटि);
	update_refs_for_switch(opts, &old_branch_info, new_branch_info);

	अगर (created_autostash) {
		discard_सूचकांक(the_repository->सूचकांक);
		अगर (repo_read_सूचकांक(the_repository) < 0)
			die(_("index file corrupt"));

		अगर (!opts->quiet && new_branch_info->commit) {
			म_लिखो(_("The following paths have local changes:\n"));
			show_local_changes(&new_branch_info->commit->object,
					   &opts->diff_options);
		}
	}

	ret = post_checkout_hook(old_branch_info.commit, new_branch_info->commit, 1);
	branch_info_release(&old_branch_info);
	strbuf_release(&old_commit_shortname);
	strbuf_release(&autostash_संदेश);

	वापस ret || writeout_error;
}

अटल पूर्णांक git_checkout_config(स्थिर अक्षर *var, स्थिर अक्षर *मूल्य,
			       स्थिर काष्ठा config_context *ctx, व्योम *cb)
{
	काष्ठा checkout_opts *opts = cb;

	अगर (!म_भेद(var, "diff.ignoresubmodules")) {
		अगर (!मूल्य)
			वापस config_error_nonbool(var);
		handle_ignore_submodules_arg(&opts->diff_options, मूल्य);
		वापस 0;
	}
	अगर (!म_भेद(var, "checkout.guess")) {
		opts->dwim_new_local_branch = git_config_bool(var, मूल्य);
		वापस 0;
	}

	अगर (starts_with(var, "submodule."))
		वापस git_default_submodule_config(var, मूल्य, शून्य);

	वापस git_xmerge_config(var, मूल्य, ctx, शून्य);
}

अटल व्योम setup_new_branch_info_and_source_वृक्ष(
	काष्ठा branch_info *new_branch_info,
	काष्ठा checkout_opts *opts,
	काष्ठा object_id *rev,
	स्थिर अक्षर *arg)
{
	काष्ठा वृक्ष **source_वृक्ष = &opts->source_वृक्ष;
	काष्ठा object_id branch_rev;

	/* treat '@' as a shortcut for 'HEAD' */
	new_branch_info->name = !म_भेद(arg, "@") ? xstrdup("HEAD") :
						    xstrdup(arg);
	setup_branch_path(new_branch_info);

	अगर (!check_refname_format(new_branch_info->path, 0) &&
	    !refs_read_ref(get_main_ref_store(the_repository), new_branch_info->path, &branch_rev))
		oidcpy(rev, &branch_rev);
	अन्यथा
		/* not an existing branch */
		FREE_AND_NULL(new_branch_info->path);

	new_branch_info->commit = lookup_commit_reference_gently(the_repository, rev, 1);
	अगर (!new_branch_info->commit) {
		/* not a commit */
		*source_वृक्ष = repo_parse_वृक्ष_indirect(the_repository, rev);
		अगर (!*source_वृक्ष)
			die(_("unable to read tree (%s)"), oid_to_hex(rev));
	} अन्यथा {
		parse_commit_or_die(new_branch_info->commit);
		*source_वृक्ष = repo_get_commit_वृक्ष(the_repository,
						    new_branch_info->commit);
		अगर (!*source_वृक्ष)
			die(_("unable to read tree (%s)"),
			    oid_to_hex(&new_branch_info->commit->object.oid));
	}
}


क्रमागत checkout_command {
	CHECKOUT_CHECKOUT = 1,
	CHECKOUT_SWITCH = 2,
	CHECKOUT_RESTORE = 3,
};

अटल व्योम advise_disambiguating_remotes(क्रमागत checkout_command which_command,
					  स्थिर अक्षर *branch,
					  स्थिर काष्ठा string_list *matched_remote_names)
{
	स्थिर अक्षर *cmdname;
	काष्ठा string_list_item *item;

	चयन (which_command) {
	हाल CHECKOUT_CHECKOUT:
		cmdname = "checkout";
		अवरोध;
	हाल CHECKOUT_SWITCH:
		cmdname = "switch";
		अवरोध;
	शेष:
		BUG("command <%d> should not reach advise_disambiguating_remotes",
		    which_command);
		अवरोध;
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

अटल अक्षर *parse_remote_branch(स्थिर अक्षर *arg,
				 काष्ठा object_id *rev,
				 पूर्णांक could_be_checkout_paths,
				 क्रमागत checkout_command which_command)
{
	पूर्णांक अंक_matches = 0;
	काष्ठा string_list matched_remote_names = STRING_LIST_INIT_DUP;

	अक्षर *remote = unique_tracking_name(arg, rev, &अंक_matches,
					    &matched_remote_names);

	अगर (remote && could_be_checkout_paths) {
		die(_("'%s' could be both a local file and a tracking branch.\n"
			"Please use -- (and optionally --no-guess) to disambiguate"),
		    arg);
	}

	अगर (!remote && अंक_matches > 1) {
		अगर (advice_enabled(ADVICE_CHECKOUT_AMBIGUOUS_REMOTE_BRANCH_NAME))
			advise_disambiguating_remotes(which_command, arg,
						      &matched_remote_names);
		die(_("'%s' matched multiple (%d) remote tracking branches"),
		    arg, अंक_matches);
	}

	string_list_clear(&matched_remote_names, 0);

	वापस remote;
}

अटल पूर्णांक parse_branchname_arg(पूर्णांक argc, स्थिर अक्षर **argv,
				पूर्णांक dwim_new_local_branch_ok,
				क्रमागत checkout_command which_command,
				काष्ठा branch_info *new_branch_info,
				काष्ठा checkout_opts *opts,
				काष्ठा object_id *rev)
{
	स्थिर अक्षर **new_branch = &opts->new_branch;
	पूर्णांक argcount = 0;
	स्थिर अक्षर *arg;
	अक्षर *remote = शून्य;
	पूर्णांक dash_dash_pos;
	पूर्णांक has_dash_dash = 0;
	पूर्णांक i;

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
	अगर (!argc)
		वापस 0;

	अगर (!opts->accept_pathspec) {
		अगर (argc > 1)
			die(_("only one reference expected"));
		has_dash_dash = 1; /* helps disambiguate */
	}

	arg = argv[0];
	dash_dash_pos = -1;
	क्रम (i = 0; i < argc; i++) {
		अगर (opts->accept_pathspec && !म_भेद(argv[i], "--")) {
			dash_dash_pos = i;
			अवरोध;
		}
	}
	अगर (dash_dash_pos == 0)
		वापस 1; /* case (2) */
	अन्यथा अगर (dash_dash_pos == 1)
		has_dash_dash = 1; /* case (3) or (1) */
	अन्यथा अगर (dash_dash_pos >= 2)
		die(_("only one reference expected, %d given."), dash_dash_pos);
	opts->गिनती_checkout_paths = !opts->quiet && !has_dash_dash;

	अगर (!म_भेद(arg, "-"))
		arg = "@{-1}";

	अगर (repo_get_oid_mb(the_repository, arg, rev)) {
		/*
		 * Either case (3) or (4), with <something> not being
		 * a commit, or an attempt to use case (1) with an
		 * invalid ref.
		 *
		 * It's likely an error, but we need to find out if
		 * we should auto-create the branch, case (3).(b).
		 */
		पूर्णांक recover_with_dwim = dwim_new_local_branch_ok;

		पूर्णांक could_be_checkout_paths = !has_dash_dash &&
			check_filename(opts->prefix, arg);

		अगर (!has_dash_dash && !no_wildcard(arg))
			recover_with_dwim = 0;

		/*
		 * Accept "git checkout foo", "git checkout foo --"
		 * and "git switch foo" as candidates for dwim.
		 */
		अगर (!(argc == 1 && !has_dash_dash) &&
		    !(argc == 2 && has_dash_dash) &&
		    opts->accept_pathspec)
			recover_with_dwim = 0;

		अगर (recover_with_dwim) {
			remote = parse_remote_branch(arg, rev,
						     could_be_checkout_paths,
						     which_command);
			अगर (remote) {
				*new_branch = arg;
				arg = remote;
				/* DWIMmed to create local branch, case (3).(b) */
			} अन्यथा {
				recover_with_dwim = 0;
			}
		}

		अगर (!recover_with_dwim) {
			अगर (has_dash_dash)
				die(_("invalid reference: %s"), arg);
			वापस argcount;
		}
	}

	/* we can't end up being in (2) anymore, eat the argument */
	argcount++;
	argv++;
	argc--;

	setup_new_branch_info_and_source_वृक्ष(new_branch_info, opts, rev, arg);

	अगर (!opts->source_वृक्ष)                   /* case (1): want a tree */
		die(_("reference is not a tree: %s"), arg);

	अगर (!has_dash_dash) {	/* case (3).(d) -> (1) */
		/*
		 * Do not complain the most common case
		 *	git checkout branch
		 * even if there happen to be a file called 'branch';
		 * it would be extremely annoying.
		 */
		अगर (argc)
			verify_non_filename(the_repository, opts->prefix, arg);
	} अन्यथा अगर (opts->accept_pathspec) {
		argcount++;
		argv++;
		argc--;
	}

	मुक्त(remote);
	वापस argcount;
}

अटल पूर्णांक switch_unborn_to_new_branch(स्थिर काष्ठा checkout_opts *opts)
{
	पूर्णांक status;
	काष्ठा strbuf branch_ref = STRBUF_INIT;

	trace2_cmd_mode("unborn");

	अगर (!opts->new_branch)
		die(_("You are on a branch yet to be born"));
	strbuf_addf(&branch_ref, "refs/heads/%s", opts->new_branch);
	status = refs_update_symref(get_main_ref_store(the_repository),
				    "HEAD", branch_ref.buf, "checkout -b");
	strbuf_release(&branch_ref);
	अगर (!opts->quiet)
		ख_लिखो(मानक_त्रुटि, _("Switched to a new branch '%s'\n"),
			opts->new_branch);
	वापस status;
}

अटल व्योम die_expecting_a_branch(स्थिर काष्ठा branch_info *branch_info)
{
	काष्ठा object_id oid;
	अक्षर *to_free;
	पूर्णांक code;

	अगर (repo_dwim_ref(the_repository, branch_info->name,
			  म_माप(branch_info->name), &oid, &to_free, 0) == 1) {
		स्थिर अक्षर *ref = to_free;

		अगर (skip_prefix(ref, "refs/tags/", &ref))
			code = die_message(_("a branch is expected, got tag '%s'"), ref);
		अन्यथा अगर (skip_prefix(ref, "refs/remotes/", &ref))
			code = die_message(_("a branch is expected, got remote branch '%s'"), ref);
		अन्यथा
			code = die_message(_("a branch is expected, got '%s'"), ref);
	}
	अन्यथा अगर (branch_info->commit)
		code = die_message(_("a branch is expected, got commit '%s'"), branch_info->name);
	अन्यथा
		/*
		 * This case should never happen because we already die() on
		 * non-commit, but just in case.
		 */
		code = die_message(_("a branch is expected, got '%s'"), branch_info->name);

	अगर (advice_enabled(ADVICE_SUGGEST_DETACHING_शीर्ष))
		advise(_("If you want to detach HEAD at the commit, try again with the --detach option."));

	निकास(code);
}

अटल व्योम die_if_some_operation_in_progress(व्योम)
{
	काष्ठा wt_status_state state;

	स_रखो(&state, 0, माप(state));
	wt_status_get_state(the_repository, &state, 0);

	अगर (state.merge_in_progress)
		die(_("cannot switch branch while merging\n"
		      "Consider \"git merge --quit\" "
		      "or \"git worktree add\"."));
	अगर (state.am_in_progress)
		die(_("cannot switch branch in the middle of an am session\n"
		      "Consider \"git am --quit\" "
		      "or \"git worktree add\"."));
	अगर (state.rebase_interactive_in_progress || state.rebase_in_progress)
		die(_("cannot switch branch while rebasing\n"
		      "Consider \"git rebase --quit\" "
		      "or \"git worktree add\"."));
	अगर (state.cherry_pick_in_progress)
		die(_("cannot switch branch while cherry-picking\n"
		      "Consider \"git cherry-pick --quit\" "
		      "or \"git worktree add\"."));
	अगर (state.revert_in_progress)
		die(_("cannot switch branch while reverting\n"
		      "Consider \"git revert --quit\" "
		      "or \"git worktree add\"."));
	अगर (state.bisect_in_progress)
		warning(_("you are switching branch while bisecting"));

	wt_status_state_free_buffers(&state);
}

/*
 * die if attempting to checkout an existing branch that is in use
 * in another worktree, unless ignore-other-wortrees option is given.
 * The check is bypassed when the branch is already the current one,
 * as it will not make things any worse.
 */
अटल व्योम die_if_switching_to_a_branch_in_use(काष्ठा checkout_opts *opts,
						स्थिर अक्षर *full_ref)
{
	पूर्णांक flags;
	अक्षर *शीर्ष_ref;

	अगर (opts->ignore_other_worktrees)
		वापस;
	शीर्ष_ref = refs_resolve_refdup(get_main_ref_store(the_repository),
				       "HEAD", 0, शून्य, &flags);
	अगर (शीर्ष_ref && (!(flags & REF_ISSYMREF) || म_भेद(शीर्ष_ref, full_ref)))
		die_if_checked_out(full_ref, 1);
	मुक्त(शीर्ष_ref);
}

अटल पूर्णांक checkout_branch(काष्ठा checkout_opts *opts,
			   काष्ठा branch_info *new_branch_info)
{
	काष्ठा repo_config_मूल्यावली *cfg = repo_config_मूल्यावली(the_repository);
	पूर्णांक noop_switch = (!new_branch_info->name &&
			   !opts->new_branch &&
			   !opts->force_detach);

	अगर (opts->pathspec.nr)
		die(_("paths cannot be used with switching branches"));

	अगर (opts->patch_mode)
		die(_("'%s' cannot be used with switching branches"),
		    "--patch");

	अगर (opts->overlay_mode != -1)
		die(_("'%s' cannot be used with switching branches"),
		    "--[no]-overlay");

	अगर (opts->writeout_stage) {
		स्थिर अक्षर *संदेश;
		अगर (noop_switch)
			संदेश = _("'%s' needs the paths to check out");
		अन्यथा
			संदेश = _("'%s' cannot be used with switching branches");
		die(संदेश, "--ours/--theirs");
	}

	अगर (opts->force && opts->merge)
		die(_("'%s' cannot be used with '%s'"), "-f", "-m");

	अगर (opts->discard_changes && opts->merge)
		die(_("'%s' cannot be used with '%s'"), "--discard-changes", "--merge");

	अगर (opts->force_detach && opts->new_branch)
		die(_("'%s' cannot be used with '%s'"),
		    "--detach", "-b/-B/--orphan");

	अगर (opts->new_orphan_branch) {
		अगर (opts->track != BRANCH_TRACK_UNSPECIFIED)
			die(_("'%s' cannot be used with '%s'"), "--orphan", "-t");
		अगर (opts->orphan_from_empty_वृक्ष && new_branch_info->name)
			die(_("'%s' cannot take <start-point>"), "--orphan");
	} अन्यथा अगर (opts->force_detach) {
		अगर (opts->track != BRANCH_TRACK_UNSPECIFIED)
			die(_("'%s' cannot be used with '%s'"), "--detach", "-t");
	} अन्यथा अगर (opts->track == BRANCH_TRACK_UNSPECIFIED)
		opts->track = cfg->branch_track;

	अगर (new_branch_info->name && !new_branch_info->commit)
		die(_("Cannot switch branch to a non-commit '%s'"),
		    new_branch_info->name);

	अगर (noop_switch &&
	    !opts->switch_branch_doing_nothing_is_ok)
		die(_("missing branch or commit argument"));

	अगर (!opts->implicit_detach &&
	    !opts->force_detach &&
	    !opts->new_branch &&
	    !opts->new_branch_force &&
	    new_branch_info->name &&
	    !new_branch_info->path)
		die_expecting_a_branch(new_branch_info);

	अगर (!opts->can_switch_when_in_progress)
		die_if_some_operation_in_progress();

	/* "git checkout <branch>" */
	अगर (new_branch_info->path && !opts->force_detach && !opts->new_branch)
		die_if_switching_to_a_branch_in_use(opts, new_branch_info->path);

	/* "git checkout -B <branch>" */
	अगर (opts->new_branch_force) {
		अक्षर *full_ref = xstrfmt("refs/heads/%s", opts->new_branch);
		die_if_switching_to_a_branch_in_use(opts, full_ref);
		मुक्त(full_ref);
	}

	अगर (!new_branch_info->commit && opts->new_branch) {
		काष्ठा object_id rev;
		पूर्णांक flag;

		अगर (!refs_read_ref_full(get_main_ref_store(the_repository), "HEAD", 0, &rev, &flag) &&
		    (flag & REF_ISSYMREF) && is_null_oid(&rev))
			वापस switch_unborn_to_new_branch(opts);
	}
	वापस switch_branches(opts, new_branch_info);
}

अटल पूर्णांक parse_opt_conflict(स्थिर काष्ठा option *o, स्थिर अक्षर *arg, पूर्णांक unset)
{
	काष्ठा checkout_opts *opts = o->मूल्य;

	अगर (unset) {
		opts->conflict_style = -1;
		वापस 0;
	}
	opts->conflict_style = parse_conflict_style_name(arg);
	अगर (opts->conflict_style < 0)
		वापस error(_("unknown conflict style '%s'"), arg);
	/* --conflict overrides a previous --no-merge */
	अगर (!opts->merge)
		opts->merge = -1;

	वापस 0;
}

अटल काष्ठा option *add_common_options(काष्ठा checkout_opts *opts,
					 काष्ठा option *prevopts)
{
	काष्ठा option options[] = {
		OPT__QUIET(&opts->quiet, N_("suppress progress reporting")),
		OPT_CALLBACK_F(0, "recurse-submodules", शून्य,
			    "checkout", "control recursive updating of submodules",
			    PARSE_OPT_OPTARG, option_parse_recurse_submodules_worktree_updater),
		OPT_BOOL(0, "progress", &opts->show_progress, N_("force progress reporting")),
		OPT_BOOL('m', "merge", &opts->merge, N_("perform a 3-way merge with the new branch")),
		OPT_CALLBACK(0, "conflict", opts, N_("style"),
			     N_("conflict style (merge, diff3, or zdiff3)"),
			     parse_opt_conflict),
		OPT_END()
	};
	काष्ठा option *newopts = parse_options_concat(prevopts, options);
	मुक्त(prevopts);
	वापस newopts;
}

अटल काष्ठा option *add_common_switch_branch_options(
	काष्ठा checkout_opts *opts, काष्ठा option *prevopts)
{
	काष्ठा option options[] = {
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
	काष्ठा option *newopts = parse_options_concat(prevopts, options);
	मुक्त(prevopts);
	वापस newopts;
}

अटल काष्ठा option *add_checkout_path_options(काष्ठा checkout_opts *opts,
						काष्ठा option *prevopts)
{
	काष्ठा option options[] = {
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
	काष्ठा option *newopts = parse_options_concat(prevopts, options);
	मुक्त(prevopts);
	वापस newopts;
}

/* create-branch option (either b or c) */
अटल अक्षर cb_option = 'b';

अटल पूर्णांक checkout_main(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
			 काष्ठा checkout_opts *opts, काष्ठा option *options,
			 क्रमागत checkout_command which_command)
{
	पूर्णांक parseopt_flags = 0;
	काष्ठा branch_info new_branch_info = { 0 };
	पूर्णांक ret;

	अटल स्थिर अक्षर * स्थिर checkout_usage[] = {
		N_("git checkout [<options>] <branch>"),
		N_("git checkout [<options>] [<branch>] -- <file>..."),
		शून्य,
	};

	अटल स्थिर अक्षर * स्थिर switch_branch_usage[] = {
		N_("git switch [<options>] [<branch>]"),
		शून्य,
	};

	अटल स्थिर अक्षर * स्थिर restore_usage[] = {
		N_("git restore [<options>] [--source=<branch>] <file>..."),
		शून्य,
	};

	स्थिर अक्षर * स्थिर *usagestr;

	चयन (which_command) {
	हाल CHECKOUT_CHECKOUT:
		usagestr = checkout_usage;
		अवरोध;
	हाल CHECKOUT_SWITCH:
		usagestr = switch_branch_usage;
		अवरोध;
	हाल CHECKOUT_RESTORE:
		usagestr = restore_usage;
		अवरोध;
	शेष:
		BUG("no such checkout variant %d", which_command);
	}

	opts->overwrite_ignore = 1;
	opts->prefix = prefix;
	opts->show_progress = -1;

	repo_config(the_repository, git_checkout_config, opts);
	अगर (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_full_सूचकांक = 0;
	}

	opts->track = BRANCH_TRACK_UNSPECIFIED;

	अगर (!opts->accept_pathspec && !opts->accept_ref)
		BUG("make up your mind, you need to take _something_");
	अगर (opts->accept_pathspec && opts->accept_ref)
		parseopt_flags = PARSE_OPT_KEEP_DASHDASH;

	argc = parse_options(argc, argv, prefix, options,
			     usagestr, parseopt_flags);

	अगर (opts->patch_context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	अगर (opts->patch_interhunk_context < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	अगर (!opts->patch_mode) {
		अगर (opts->patch_context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--patch");
		अगर (opts->patch_interhunk_context != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--patch");
		अगर (!opts->auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--patch");
	}

	अगर (opts->show_progress < 0) {
		अगर (opts->quiet)
			opts->show_progress = 0;
		अन्यथा
			opts->show_progress = isatty(2);
	}

	/* --conflicts implies --merge */
	अगर (opts->merge == -1)
		opts->merge = opts->conflict_style >= 0;

	अगर (opts->force) {
		opts->discard_changes = 1;
		opts->ignore_unmerged_opt = "--force";
		opts->ignore_unmerged = 1;
	}

	अगर ((!!opts->new_branch + !!opts->new_branch_force + !!opts->new_orphan_branch) > 1)
		die(_("options '-%c', '-%c', and '%s' cannot be used together"),
			cb_option, बड़े(cb_option), "--orphan");

	अगर (opts->overlay_mode == 1 && opts->patch_mode)
		die(_("options '%s' and '%s' cannot be used together"), "-p", "--overlay");

	अगर (opts->checkout_सूचकांक >= 0 || opts->checkout_worktree >= 0) {
		अगर (opts->checkout_सूचकांक < 0)
			opts->checkout_सूचकांक = 0;
		अगर (opts->checkout_worktree < 0)
			opts->checkout_worktree = 0;
	} अन्यथा {
		अगर (opts->checkout_सूचकांक < 0)
			opts->checkout_सूचकांक = -opts->checkout_सूचकांक - 1;
		अगर (opts->checkout_worktree < 0)
			opts->checkout_worktree = -opts->checkout_worktree - 1;
	}
	अगर (opts->checkout_सूचकांक < 0 || opts->checkout_worktree < 0)
		BUG("these flags should be non-negative by now");
	/*
	 * convenient shortcut: "git restore --staged [--worktree]" equals
	 * "git restore --staged [--worktree] --source HEAD"
	 */
	अगर (!opts->from_treeish && opts->checkout_सूचकांक)
		opts->from_treeish = "HEAD";

	/*
	 * From here on, new_branch will contain the branch to be checked out,
	 * and new_branch_force and new_orphan_branch will tell us which one of
	 * -b/-B/-c/-C/--orphan is being used.
	 */
	अगर (opts->new_branch_force)
		opts->new_branch = opts->new_branch_force;

	अगर (opts->new_orphan_branch)
		opts->new_branch = opts->new_orphan_branch;

	/* --track without -c/-C/-b/-B/--orphan should DWIM */
	अगर (opts->track != BRANCH_TRACK_UNSPECIFIED && !opts->new_branch) {
		स्थिर अक्षर *argv0 = argv[0];
		अगर (!argc || !म_भेद(argv0, "--"))
			die(_("--track needs a branch name"));
		skip_prefix(argv0, "refs/", &argv0);
		skip_prefix(argv0, "remotes/", &argv0);
		argv0 = म_अक्षर(argv0, '/');
		अगर (!argv0 || !argv0[1])
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
	अगर (argc && opts->accept_ref) {
		काष्ठा object_id rev;
		पूर्णांक dwim_ok =
			!opts->patch_mode &&
			opts->dwim_new_local_branch &&
			opts->track == BRANCH_TRACK_UNSPECIFIED &&
			!opts->new_branch;
		पूर्णांक n = parse_branchname_arg(argc, argv, dwim_ok, which_command,
					     &new_branch_info, opts, &rev);
		argv += n;
		argc -= n;
	} अन्यथा अगर (!opts->accept_ref && opts->from_treeish) {
		काष्ठा object_id rev;

		अगर (repo_get_oid_mb(the_repository, opts->from_treeish, &rev))
			die(_("could not resolve '%s'"), opts->from_treeish);

		setup_new_branch_info_and_source_वृक्ष(&new_branch_info,
						      opts, &rev,
						      opts->from_treeish);

		अगर (!opts->source_वृक्ष)
			die(_("reference is not a tree: %s"), opts->from_treeish);
	}

	अगर (argc) {
		parse_pathspec(&opts->pathspec, 0,
			       opts->patch_mode ? PATHSPEC_PREFIX_ORIGIN : 0,
			       prefix, argv);

		अगर (!opts->pathspec.nr)
			die(_("invalid path specification"));

		/*
		 * Try to give more helpful suggestion.
		 * new_branch && argc > 1 will be caught later.
		 */
		अगर (opts->new_branch && argc == 1 && !new_branch_info.commit)
			die(_("'%s' is not a commit and a branch '%s' cannot be created from it"),
				argv[0], opts->new_branch);

		अगर (opts->force_detach)
			die(_("git checkout: --detach does not take a path argument '%s'"),
			    argv[0]);
	}

	अगर (opts->pathspec_from_file) {
		अगर (opts->pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		अगर (opts->force_detach)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--detach");

		अगर (opts->patch_mode)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--patch");

		parse_pathspec_file(&opts->pathspec, 0,
				    0,
				    prefix, opts->pathspec_from_file, opts->pathspec_file_nul);
	} अन्यथा अगर (opts->pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	opts->pathspec.recursive = 1;

	अगर (opts->pathspec.nr) {
		अगर (1 < !!opts->writeout_stage + !!opts->force + !!opts->merge)
			die(_("git checkout: --ours/--theirs, --force and --merge are incompatible when\n"
			      "checking out of the index."));
	} अन्यथा {
		अगर (opts->accept_pathspec && !opts->empty_pathspec_ok &&
		    !opts->patch_mode)	/* patch mode is special */
			die(_("you must specify path(s) to restore"));
	}

	अगर (opts->new_branch) {
		काष्ठा strbuf buf = STRBUF_INIT;

		अगर (opts->new_branch_force)
			opts->branch_exists = validate_branchname(opts->new_branch, &buf);
		अन्यथा
			opts->branch_exists =
				validate_new_branchname(opts->new_branch, &buf, 0);
		strbuf_release(&buf);
	}

	अगर (opts->patch_mode || opts->pathspec.nr)
		ret = checkout_paths(opts, &new_branch_info);
	अन्यथा
		ret = checkout_branch(opts, &new_branch_info);

	branch_info_release(&new_branch_info);
	clear_pathspec(&opts->pathspec);
	मुक्त(opts->pathspec_from_file);
	मुक्त(options);

	वापस ret;
}

पूर्णांक cmd_checkout(पूर्णांक argc,
		 स्थिर अक्षर **argv,
		 स्थिर अक्षर *prefix,
		 काष्ठा repository *repo UNUSED)
{
	काष्ठा checkout_opts opts = CHECKOUT_OPTS_INIT;
	काष्ठा option *options;
	काष्ठा option checkout_options[] = {
		OPT_STRING('b', शून्य, &opts.new_branch, N_("branch"),
			   N_("create and checkout a new branch")),
		OPT_STRING('B', शून्य, &opts.new_branch_force, N_("branch"),
			   N_("create/reset and checkout a branch")),
		OPT_BOOL('l', शून्य, &opts.new_branch_log, N_("create reflog for new branch")),
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
	opts.checkout_सूचकांक = -2;    /* default on */
	opts.checkout_worktree = -2; /* default on */

	अगर (argc == 3 && !म_भेद(argv[1], "-b")) {
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

	वापस checkout_main(argc, argv, prefix, &opts, options,
			     CHECKOUT_CHECKOUT);
}

पूर्णांक cmd_switch(पूर्णांक argc,
	       स्थिर अक्षर **argv,
	       स्थिर अक्षर *prefix,
	       काष्ठा repository *repo UNUSED)
{
	काष्ठा checkout_opts opts = CHECKOUT_OPTS_INIT;
	काष्ठा option *options = शून्य;
	काष्ठा option switch_options[] = {
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

	वापस checkout_main(argc, argv, prefix, &opts, options,
			     CHECKOUT_SWITCH);
}

पूर्णांक cmd_restore(पूर्णांक argc,
		स्थिर अक्षर **argv,
		स्थिर अक्षर *prefix,
		काष्ठा repository *repo UNUSED)
{
	काष्ठा checkout_opts opts = CHECKOUT_OPTS_INIT;
	काष्ठा option *options;
	काष्ठा option restore_options[] = {
		OPT_STRING('s', "source", &opts.from_treeish, "<tree-ish>",
			   N_("which tree-ish to checkout from")),
		OPT_BOOL('S', "staged", &opts.checkout_सूचकांक,
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
	opts.checkout_सूचकांक = -1;    /* default off */
	opts.checkout_worktree = -2; /* default on */
	opts.ignore_unmerged_opt = "--ignore-unmerged";

	options = parse_options_dup(restore_options);
	options = add_common_options(&opts, options);
	options = add_checkout_path_options(&opts, options);

	वापस checkout_main(argc, argv, prefix, &opts, options,
			     CHECKOUT_RESTORE);
}
