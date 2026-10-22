/*
 * "git reset" builtin command
 *
 * Copyright (c) 2007 Carlos Rica
 *
 * Based on git-reset.sh, which is
 *
 * Copyright (c) 2005, 2006 Linus Torvalds and Junio C Hamano
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "advice.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hash.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hex.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "lockfile.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "pretty.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "refs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "diff.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "diffcore.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "tree.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "branch.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-name.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "path.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "repository.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "unpack-trees.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cache-tree.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "setup.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "sparse-index.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "submodule.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "trace.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "trace2.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "dir.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "add-interactive.h"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ REFRESH_ਸੂਚਕਾਂਕ_DELAY_WARNING_ਵਿੱਚ_MS (2 * 1000)

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ git_reset_usage[] = {
	N_("git reset [--mixed | --soft | --hard | --merge | --keep] [-q] [<commit>]"),
	N_("git reset [-q] [<tree-ish>] [--] <pathspec>..."),
	N_("git reset [-q] [--pathspec-from-file [--pathspec-file-nul]] [<tree-ish>]"),
	N_("git reset --patch [<tree-ish>] [--] [<pathspec>...]"),
	ਨਿਰਾ
};

ਗਿਣਤੀ reset_type { MIXED, SOFT, HARD, ਮਿਲਾਓ_2, KEEP, NONE };
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *reset_type_ਨਾਂ[] = {
	N_("mixed"), N_("soft"), N_("hard"), N_("merge"), N_("keep"), ਨਿਰਾ
};

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ is_ਮਿਲਾਓ(ਖਾਲੀ)
{
	ਵਾਪਸ !access(git_ਰਸਤਾ_ਮਿਲਾਓ_ਸਿਰ(the_repository), F_OK);
}

ਸਥਿਰ ਪੂਰਨ reset_ਸੂਚਕਾਂਕ(ਅਚਲ ਅੱਖਰ *ref, ਅਚਲ ਬਣਤਰ object_id *oid, ਪੂਰਨ reset_type, ਪੂਰਨ quiet)
{
	ਪੂਰਨ i, nr = 0;
	ਬਣਤਰ ਰੁੱਖ_desc desc[2];
	ਬਣਤਰ ਰੁੱਖ *ਰੁੱਖ;
	ਬਣਤਰ unpack_trees_options opts;
	ਪੂਰਨ ret = -1;

	memset(&opts, 0, ਆਕਾਰ(opts));
	opts.ਸਿਰ_idx = 1;
	opts.src_ਸੂਚਕਾਂਕ = the_repository->ਸੂਚਕਾਂਕ;
	opts.dst_ਸੂਚਕਾਂਕ = the_repository->ਸੂਚਕਾਂਕ;
	opts.fn = oneway_ਮਿਲਾਓ;
	opts.ਮਿਲਾਓ = 1;
	init_checkout_metadata(&opts.meta, ref, oid, ਨਿਰਾ);
	ਜੇ (!quiet)
		opts.verbose_update = 1;
	ਚੋਣ (reset_type) {
	ਹਾਲਤ KEEP:
	ਹਾਲਤ ਮਿਲਾਓ_2:
		opts.update = 1;
		opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
		ਰੋਕੋ;
	ਹਾਲਤ HARD:
		opts.update = 1;
		opts.reset = UNPACK_RESET_OVERWRITE_UNTRACKED;
		opts.skip_cache_ਰੁੱਖ_update = 1;
		ਰੋਕੋ;
	ਹਾਲਤ MIXED:
		opts.reset = UNPACK_RESET_PROTECT_UNTRACKED;
		opts.skip_cache_ਰੁੱਖ_update = 1;
		/* but opts.update=0, so working tree not updated */
		ਰੋਕੋ;
	ਮੂਲ:
		BUG("invalid reset_type passed to reset_index");
	}

	repo_read_ਸੂਚਕਾਂਕ_unmerged(the_repository);

	ਜੇ (reset_type == KEEP) {
		ਬਣਤਰ object_id ਸਿਰ_oid;
		ਜੇ (repo_get_oid(the_repository, "HEAD", &ਸਿਰ_oid))
			ਵਾਪਸ error(_("You do not have a valid HEAD."));
		ਜੇ (!fill_ਰੁੱਖ_descriptor(the_repository, desc + nr, &ਸਿਰ_oid))
			ਵਾਪਸ error(_("Failed to find tree of HEAD."));
		nr++;
		opts.fn = twoway_ਮਿਲਾਓ;
	}

	ਜੇ (!fill_ਰੁੱਖ_descriptor(the_repository, desc + nr, oid)) {
		error(_("Failed to find tree of %s."), oid_ਮੰਜ਼ਿਲ_hex(oid));
		ਜਾਓ ਬਾਹਰਲਾ;
	}
	nr++;

	ਜੇ (unpack_trees(nr, desc, &opts))
		ਜਾਓ ਬਾਹਰਲਾ;

	ਜੇ (reset_type == MIXED || reset_type == HARD) {
		ਰੁੱਖ = repo_parse_ਰੁੱਖ_ਅਸਿੱਧਾ(the_repository, oid);
		ਜੇ (!ਰੁੱਖ) {
			error(_("unable to read tree (%s)"), oid_ਮੰਜ਼ਿਲ_hex(oid));
			ਜਾਓ ਬਾਹਰਲਾ;
		}
		ਅਭਾਜ_cache_ਰੁੱਖ(the_repository, the_repository->ਸੂਚਕਾਂਕ, ਰੁੱਖ);
	}

	ret = 0;

ਬਾਹਰਲਾ:
	ਲਈ (i = 0; i < nr; i++)
		ਮੁਕਤ((ਖਾਲੀ *)desc[i].ਬਫਰ);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਖਾਲੀ print_new_ਸਿਰ_ਸਤਰ(ਬਣਤਰ commit *commit)
{
	ਬਣਤਰ strbuf buf = STRBUF_INIT;

	ਲਿਖੋ(_("HEAD is now at %s"),
		repo_ਲੱਭੋ_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV));

	pp_commit_easy(CMIT_FMT_ONELINE, commit, &buf);
	ਜੇ (buf.len > 0)
		ਲਿਖੋ(" %s", buf.buf);
	ਅੱਖਰ_ਲਿਖੋ('\n');
	strbuf_release(&buf);
}

ਸਥਿਰ ਖਾਲੀ update_ਸੂਚਕਾਂਕ_from_diff(ਬਣਤਰ diff_ਕਤਾਰ_struct *q,
				   ਬਣਤਰ diff_options *opt ਅਣਵਰਤਿਆ,
				   ਖਾਲੀ *ਅੰਕੜੇ)
{
	ਪੂਰਨ i;
	ਪੂਰਨ intent_ਮੰਜ਼ਿਲ_ਜੋੜੋ = *(ਪੂਰਨ *)ਅੰਕੜੇ;

	ਲਈ (i = 0; i < q->nr; i++) {
		ਪੂਰਨ pos;
		ਬਣਤਰ diff_filespec *one = q->ਕਤਾਰ[i]->one;
		ਪੂਰਨ is_ਵਿੱਚ_reset_ਰੁੱਖ = one->ਬਹੁਲਕ && !is_null_oid(&one->oid);
		ਬਣਤਰ cache_ਇੰਦਰਾਜ *ce;

		ਜੇ (!is_ਵਿੱਚ_reset_ਰੁੱਖ && !intent_ਮੰਜ਼ਿਲ_ਜੋੜੋ) {
			remove_file_from_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ, one->ਰਸਤਾ);
			ਜਾਰੀ;
		}

		ce = make_cache_ਇੰਦਰਾਜ(the_repository->ਸੂਚਕਾਂਕ, one->ਬਹੁਲਕ, &one->oid, one->ਰਸਤਾ,
				      0, 0);

		/*
		 * If the file 1) corresponds to an existing index entry with
		 * skip-worktree set, or 2) does not exist in the index but is
		 * outside the sparse checkout definition, add a skip-worktree bit
		 * to the new index entry. Note that a sparse index will be expanded
		 * if this entry is outside the sparse cone - this is necessary
		 * to properly construct the reset sparse directory.
		 */
		pos = ਸੂਚਕਾਂਕ_name_pos(the_repository->ਸੂਚਕਾਂਕ, one->ਰਸਤਾ, ਲੜੀ_ਲੰਬਾਈ(one->ਰਸਤਾ));
		ਜੇ ((pos >= 0 && ce_skip_worktree(the_repository->ਸੂਚਕਾਂਕ->cache[pos])) ||
		    (pos < 0 && !ਰਸਤਾ_ਵਿੱਚ_sparse_checkout(one->ਰਸਤਾ, the_repository->ਸੂਚਕਾਂਕ)))
			ce->ce_flags |= CE_SKIP_WORKTREE;

		ਜੇ (!ce)
			die(_("make_cache_entry failed for path '%s'"),
			    one->ਰਸਤਾ);
		ਜੇ (!is_ਵਿੱਚ_reset_ਰੁੱਖ) {
			ce->ce_flags |= CE_INTENT_ਮੰਜ਼ਿਲ_ਜੋੜੋ;
			ਸੈੱਟ_object_name_for_intent_ਮੰਜ਼ਿਲ_ਜੋੜੋ_ਇੰਦਰਾਜ(ce);
		}
		ਜੋੜੋ_ਸੂਚਕਾਂਕ_ਇੰਦਰਾਜ(the_repository->ਸੂਚਕਾਂਕ, ce,
				ਜੋੜੋ_CACHE_OK_ਮੰਜ਼ਿਲ_ਜੋੜੋ | ਜੋੜੋ_CACHE_OK_ਮੰਜ਼ਿਲ_REPLACE);
	}
}

ਸਥਿਰ ਪੂਰਨ read_from_ਰੁੱਖ(ਅਚਲ ਬਣਤਰ pathspec *pathspec,
			  ਬਣਤਰ object_id *ਰੁੱਖ_oid,
			  ਪੂਰਨ intent_ਮੰਜ਼ਿਲ_ਜੋੜੋ)
{
	ਬਣਤਰ diff_options opt;

	memset(&opt, 0, ਆਕਾਰ(opt));
	copy_pathspec(&opt.pathspec, pathspec);
	opt.output_format = DIFF_FORMAT_CALLBACK;
	opt.format_callback = update_ਸੂਚਕਾਂਕ_from_diff;
	opt.format_callback_ਅੰਕੜੇ = &intent_ਮੰਜ਼ਿਲ_ਜੋੜੋ;
	opt.flags.override_submodule_config = 1;
	opt.flags.recursive = 1;
	opt.repo = the_repository;
	opt.change = diff_change;
	opt.ਜੋੜੋ_remove = diff_addremove;

	ਜੇ (pathspec->nr && pathspec_needs_expanded_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ, pathspec))
		ensure_ਭਰਿਆ_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ);

	ਜੇ (do_diff_cache(ਰੁੱਖ_oid, &opt))
		ਵਾਪਸ 1;
	diffcore_std(&opt);
	diff_flush(&opt);

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ ਸੈੱਟ_reflog_ਸੁਨੇਹਾ(ਬਣਤਰ strbuf *sb, ਅਚਲ ਅੱਖਰ *action,
			       ਅਚਲ ਅੱਖਰ *rev)
{
	ਅਚਲ ਅੱਖਰ *rla = getenv("GIT_REFLOG_ACTION");

	strbuf_reset(sb);
	ਜੇ (rla)
		strbuf_addf(sb, "%s: %s", rla, action);
	ਨਹੀਂਤਾਂ ਜੇ (rev)
		strbuf_addf(sb, "reset: moving to %s", rev);
	ਨਹੀਂਤਾਂ
		strbuf_addf(sb, "reset: %s", action);
}

ਸਥਿਰ ਖਾਲੀ die_if_unmerged_cache(ਪੂਰਨ reset_type)
{
	ਜੇ (is_ਮਿਲਾਓ() || unmerged_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ))
		die(_("Cannot do a %s reset in the middle of a merge."),
		    _(reset_type_ਨਾਂ[reset_type]));

}

ਸਥਿਰ ਖਾਲੀ parse_ਦਲੀਲਾਂ(ਬਣਤਰ pathspec *pathspec,
		       ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		       ਪੂਰਨ patch_ਬਹੁਲਕ,
		       ਅਚਲ ਅੱਖਰ **rev_ret)
{
	ਅਚਲ ਅੱਖਰ *rev = "HEAD";
	ਬਣਤਰ object_id ਅਣਵਰਤਿਆ_2;
	/*
	 * Possible arguments are:
	 *
	 * git reset [-opts] [<rev>]
	 * git reset [-opts] <tree> [<paths>...]
	 * git reset [-opts] <tree> -- [<paths>...]
	 * git reset [-opts] -- [<paths>...]
	 * git reset [-opts] <paths>...
	 *
	 * At this point, argv points immediately after [-opts].
	 */

	ਜੇ (argv[0]) {
		ਜੇ (!strcmp(argv[0], "--")) {
			argv++; /* reset to HEAD, possibly with paths */
		} ਨਹੀਂਤਾਂ ਜੇ (argv[1] && !strcmp(argv[1], "--")) {
			rev = argv[0];
			argv += 2;
		}
		/*
		 * Otherwise, argv[0] could be either <rev> or <paths> and
		 * has to be unambiguous. If there is a single argument, it
		 * can not be a tree
		 */
		ਨਹੀਂਤਾਂ ਜੇ ((!argv[1] && !repo_get_oid_committish(the_repository, argv[0], &ਅਣਵਰਤਿਆ_2)) ||
			 (argv[1] && !repo_get_oid_treeish(the_repository, argv[0], &ਅਣਵਰਤਿਆ_2))) {
			/*
			 * Ok, argv[0] looks like a commit/tree; it should not
			 * be a filename.
			 */
			verify_non_filename(the_repository, ਅਗੇਤਰ, argv[0]);
			rev = *argv++;
		} ਨਹੀਂਤਾਂ {
			/* Otherwise we treat this as a filename */
			verify_filename(the_repository, ਅਗੇਤਰ, argv[0], 1);
		}
	}

	/* treat '@' as a shortcut for 'HEAD' */
	*rev_ret = !strcmp("@", rev) ? "HEAD" : rev;

	parse_pathspec(pathspec, 0,
		       PATHSPEC_PREFER_ਭਰਿਆ |
		       (patch_ਬਹੁਲਕ ? PATHSPEC_ਅਗੇਤਰ_ORIGIN : 0),
		       ਅਗੇਤਰ, argv);
}

ਸਥਿਰ ਪੂਰਨ reset_refs(ਅਚਲ ਅੱਖਰ *rev, ਅਚਲ ਬਣਤਰ object_id *oid)
{
	ਪੂਰਨ update_ref_ਸਥਿਤੀ;
	ਬਣਤਰ strbuf ਸੁਨੇਹਾ = STRBUF_INIT;
	ਬਣਤਰ object_id *orig = ਨਿਰਾ, oid_orig,
		*old_orig = ਨਿਰਾ, oid_old_orig;

	ਜੇ (!repo_get_oid(the_repository, "ORIG_HEAD", &oid_old_orig))
		old_orig = &oid_old_orig;
	ਜੇ (!repo_get_oid(the_repository, "HEAD", &oid_orig)) {
		orig = &oid_orig;
		ਸੈੱਟ_reflog_ਸੁਨੇਹਾ(&ਸੁਨੇਹਾ, "updating ORIG_HEAD", ਨਿਰਾ);
		refs_update_ref(get_main_ref_store(the_repository), ਸੁਨੇਹਾ.buf,
				"ORIG_HEAD", orig, old_orig, 0,
				UPDATE_REFS_ਸੁਨੇਹਾ_ON_ERR);
	} ਨਹੀਂਤਾਂ ਜੇ (old_orig)
		refs_delete_ref(get_main_ref_store(the_repository), ਨਿਰਾ,
				"ORIG_HEAD", old_orig, 0);
	ਸੈੱਟ_reflog_ਸੁਨੇਹਾ(&ਸੁਨੇਹਾ, "updating HEAD", rev);
	update_ref_ਸਥਿਤੀ = refs_update_ref(get_main_ref_store(the_repository),
					    ਸੁਨੇਹਾ.buf, "HEAD", oid, orig, 0,
					    UPDATE_REFS_ਸੁਨੇਹਾ_ON_ERR);
	strbuf_release(&ਸੁਨੇਹਾ);
	ਵਾਪਸ update_ref_ਸਥਿਤੀ;
}

ਸਥਿਰ ਪੂਰਨ git_reset_config(ਅਚਲ ਅੱਖਰ *var, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ,
			    ਅਚਲ ਬਣਤਰ config_context *ctx, ਖਾਲੀ *cb)
{
	ਜੇ (!strcmp(var, "submodule.recurse"))
		ਵਾਪਸ git_default_submodule_config(var, ਮੁੱਲ, cb);

	ਵਾਪਸ git_default_config(var, ਮੁੱਲ, ctx, cb);
}

ਪੂਰਨ cmd_reset(ਪੂਰਨ argc,
	      ਅਚਲ ਅੱਖਰ **argv,
	      ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
	      ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ reset_type = NONE, update_ref_ਸਥਿਤੀ = 0, quiet = 0;
	ਪੂਰਨ no_refresh = 0;
	ਪੂਰਨ patch_ਬਹੁਲਕ = 0, pathspec_file_nul = 0, unborn;
	ਅਚਲ ਅੱਖਰ *rev;
	ਅੱਖਰ *pathspec_from_file = ਨਿਰਾ;
	ਬਣਤਰ object_id oid;
	ਬਣਤਰ pathspec pathspec;
	ਪੂਰਨ intent_ਮੰਜ਼ਿਲ_ਜੋੜੋ = 0;
	ਬਣਤਰ interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	ਅਚਲ ਬਣਤਰ option options[] = {
		OPT__QUIET(&quiet, N_("be quiet, only report errors")),
		OPT_BOOL(0, "no-refresh", &no_refresh,
				N_("skip refreshing the index after reset")),
		OPT_ਸੈੱਟ_INT_F(0, "mixed", &reset_type,
			      N_("reset HEAD and index"),
			      MIXED, PARSE_OPT_NONEG),
		OPT_ਸੈੱਟ_INT_F(0, "soft", &reset_type,
			      N_("reset only HEAD"),
			      SOFT, PARSE_OPT_NONEG),
		OPT_ਸੈੱਟ_INT_F(0, "hard", &reset_type,
			      N_("reset HEAD, index and working tree"),
			      HARD, PARSE_OPT_NONEG),
		OPT_ਸੈੱਟ_INT_F(0, "merge", &reset_type,
			      N_("reset HEAD, index and working tree"),
			      ਮਿਲਾਓ_2, PARSE_OPT_NONEG),
		OPT_ਸੈੱਟ_INT_F(0, "keep", &reset_type,
			      N_("reset HEAD but keep local changes"),
			      KEEP, PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "recurse-submodules", ਨਿਰਾ,
			       "reset", "control recursive updating of submodules",
			       PARSE_OPT_OPTARG,
			       option_parse_recurse_submodules_worktree_updater),
		OPT_BOOL('p', "patch", &patch_ਬਹੁਲਕ, N_("select hunks interactively")),
		OPT_BOOL(0, "auto-advance", &interactive_opts.auto_advance,
			 N_("auto advance to the next file when selecting hunks interactively")),
		OPT_DIFF_UNIFIED(&interactive_opts.context),
		OPT_DIFF_INTERHUNK_CONTEXT(&interactive_opts.interhunkcontext),
		OPT_BOOL('N', "intent-to-add", &intent_ਮੰਜ਼ਿਲ_ਜੋੜੋ,
				N_("record only the fact that removed paths will be added later")),
		OPT_PATHSPEC_FROM_FILE(&pathspec_from_file),
		OPT_PATHSPEC_FILE_NUL(&pathspec_file_nul),
		OPT_ਸਿਰਾ()
	};

	repo_config(the_repository, git_reset_config, ਨਿਰਾ);

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, git_reset_usage,
						PARSE_OPT_KEEP_DASHDASH);
	parse_ਦਲੀਲਾਂ(&pathspec, argv, ਅਗੇਤਰ, patch_ਬਹੁਲਕ, &rev);

	ਜੇ (pathspec_from_file) {
		ਜੇ (patch_ਬਹੁਲਕ)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--patch");

		ਜੇ (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_ਭਰਿਆ,
				    ਅਗੇਤਰ, pathspec_from_file, pathspec_file_nul);
	} ਨਹੀਂਤਾਂ ਜੇ (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	unborn = !strcmp(rev, "HEAD") && repo_get_oid(the_repository, "HEAD",
						      &oid);
	ਜੇ (unborn) {
		/* reset on unborn branch: treat as reset to empty tree */
		oidcpy(&oid, the_hash_algo->ਖਾਲੀ_ਥਾਵਾਂ_ਰੁੱਖ);
	} ਨਹੀਂਤਾਂ ਜੇ (!pathspec.nr && !patch_ਬਹੁਲਕ) {
		ਬਣਤਰ commit *commit;
		ਜੇ (repo_get_oid_committish(the_repository, rev, &oid))
			die(_("Failed to resolve '%s' as a valid revision."), rev);
		commit = lookup_commit_reference(the_repository, &oid);
		ਜੇ (!commit)
			die(_("Could not parse object '%s'."), rev);
		oidcpy(&oid, &commit->object.oid);
	} ਨਹੀਂਤਾਂ {
		ਬਣਤਰ ਰੁੱਖ *ਰੁੱਖ;
		ਜੇ (repo_get_oid_treeish(the_repository, rev, &oid))
			die(_("Failed to resolve '%s' as a valid tree."), rev);
		ਰੁੱਖ = repo_parse_ਰੁੱਖ_ਅਸਿੱਧਾ(the_repository, &oid);
		ਜੇ (!ਰੁੱਖ)
			die(_("Could not parse object '%s'."), rev);
		oidcpy(&oid, &ਰੁੱਖ->object.oid);
	}

	ਜੇ (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	ਜੇ (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_ਭਰਿਆ_ਸੂਚਕਾਂਕ = 0;

	ਜੇ (patch_ਬਹੁਲਕ) {
		ਜੇ (reset_type != NONE)
			die(_("options '%s' and '%s' cannot be used together"), "--patch", "--{hard,mixed,soft}");
		trace2_cmd_ਬਹੁਲਕ("patch-interactive");
		update_ref_ਸਥਿਤੀ = !!ਲੜੀਵਾਰ_ਜੋੜੋ_p(the_repository, ਜੋੜੋ_P_RESET,
						&interactive_opts, rev, &pathspec, 0);
		ਜਾਓ cleanup;
	} ਨਹੀਂਤਾਂ {
		ਜੇ (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--patch");
		ਜੇ (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--patch");
		ਜੇ (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--patch");
	}

	/* git reset tree [--] paths... can be used to
	 * load chosen paths from the tree into the index without
	 * affecting the working tree nor HEAD. */
	ਜੇ (pathspec.nr) {
		ਜੇ (reset_type == MIXED)
			warning(_("--mixed with paths is deprecated; use 'git reset -- <paths>' instead."));
		ਨਹੀਂਤਾਂ ਜੇ (reset_type != NONE)
			die(_("Cannot do %s reset with paths."),
					_(reset_type_ਨਾਂ[reset_type]));
	}
	ਜੇ (reset_type == NONE)
		reset_type = MIXED; /* by default */

	ਜੇ (pathspec.nr)
		trace2_cmd_ਬਹੁਲਕ("path");
	ਨਹੀਂਤਾਂ
		trace2_cmd_ਬਹੁਲਕ(reset_type_ਨਾਂ[reset_type]);

	ਜੇ (reset_type != SOFT && (reset_type != MIXED || repo_get_work_ਰੁੱਖ(the_repository)))
		ਤਿਆਰੀ_work_ਰੁੱਖ(the_repository);

	ਜੇ (reset_type == MIXED && is_bare_repository(the_repository))
		die(_("%s reset is not allowed in a bare repository"),
		    _(reset_type_ਨਾਂ[reset_type]));

	ਜੇ (intent_ਮੰਜ਼ਿਲ_ਜੋੜੋ && reset_type != MIXED)
		die(_("the option '%s' requires '%s'"), "-N", "--mixed");

	ਜੇ (repo_read_ਸੂਚਕਾਂਕ(the_repository) < 0)
		die(_("index file corrupt"));

	/* Soft reset does not touch the index file nor the working tree
	 * at all, but requires them in a good order.  Other resets reset
	 * the index file to the tree object we are switching to. */
	ਜੇ (reset_type == SOFT || reset_type == KEEP)
		die_if_unmerged_cache(reset_type);

	ਜੇ (reset_type != SOFT) {
		ਬਣਤਰ ਤਾਲਾ_file ਤਾਲਾ = ਤਾਲਾ_INIT;
		repo_hold_locked_ਸੂਚਕਾਂਕ(the_repository, &ਤਾਲਾ,
				       ਤਾਲਾ_DIE_ON_ERROR);
		ਜੇ (reset_type == MIXED) {
			ਪੂਰਨ flags = quiet ? REFRESH_QUIET : REFRESH_ਵਿੱਚ_PORCELAIN;
			ਜੇ (read_from_ਰੁੱਖ(&pathspec, &oid, intent_ਮੰਜ਼ਿਲ_ਜੋੜੋ)) {
				update_ref_ਸਥਿਤੀ = 1;
				ਜਾਓ cleanup;
			}
			the_repository->ਸੂਚਕਾਂਕ->updated_skipworktree = 1;
			ਜੇ (!no_refresh && repo_get_work_ਰੁੱਖ(the_repository)) {
				uint64_t t_begin, t_delta_ਵਿੱਚ_ms;

				t_begin = getnanotime();
				refresh_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ, flags, ਨਿਰਾ, ਨਿਰਾ,
					      _("Unstaged changes after reset:"));
				t_delta_ਵਿੱਚ_ms = (getnanotime() - t_begin) / 1000000;
				ਜੇ (!quiet && advice_enabled(ADVICE_RESET_NO_REFRESH_WARNING) && t_delta_ਵਿੱਚ_ms > REFRESH_ਸੂਚਕਾਂਕ_DELAY_WARNING_ਵਿੱਚ_MS) {
					advise(_("It took %.2f seconds to refresh the index after reset.  You can use\n"
						 "'--no-refresh' to avoid this."), t_delta_ਵਿੱਚ_ms / 1000.0);
				}
			}
		} ਨਹੀਂਤਾਂ {
			ਬਣਤਰ object_id dummy;
			ਅੱਖਰ *ref = ਨਿਰਾ;
			ਪੂਰਨ err;

			repo_dwim_ref(the_repository, rev, ਲੜੀ_ਲੰਬਾਈ(rev),
				      &dummy, &ref, 0);
			ਜੇ (ref && !starts_with(ref, "refs/"))
				FREE_AND_NULL(ref);

			err = reset_ਸੂਚਕਾਂਕ(ref, &oid, reset_type, quiet);
			ਜੇ (reset_type == KEEP && !err)
				err = reset_ਸੂਚਕਾਂਕ(ref, &oid, MIXED, quiet);
			ਜੇ (err)
				die(_("Could not reset index file to revision '%s'."), rev);
			ਮੁਕਤ(ref);
		}

		ਜੇ (write_locked_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ, &ਤਾਲਾ, COMMIT_ਤਾਲਾ))
			die(_("Could not write new index file."));
	}

	ਜੇ (!pathspec.nr && !unborn) {
		/* Any resets without paths update HEAD to the head being
		 * switched to, saving the previous head in ORIG_HEAD before. */
		update_ref_ਸਥਿਤੀ = reset_refs(rev, &oid);

		ਜੇ (reset_type == HARD && !update_ref_ਸਥਿਤੀ && !quiet)
			print_new_ਸਿਰ_ਸਤਰ(lookup_commit_reference(the_repository, &oid));
	}
	ਜੇ (!pathspec.nr)
		remove_branch_state(the_repository, 0);

	discard_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ);

cleanup:
	clear_pathspec(&pathspec);
	ਮੁਕਤ(pathspec_from_file);
	ਵਾਪਸ update_ref_ਸਥਿਤੀ;
}
