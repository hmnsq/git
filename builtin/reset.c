/*
 * "git reset" builtin command
 *
 * Copyright (c) 2007 Carlos Rica
 *
 * Based on git-reset.sh, which is
 *
 * Copyright (c) 2005, 2006 Linus Torvalds and Junio C Hamano
 */

#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "advice.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "hash.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "lockfile.h"
#ಅನ್ತರ್ಭಾವ "object.h"
#ಅನ್ತರ್ಭಾವ "pretty.h"
#ಅನ್ತರ್ಭಾವ "refs.h"
#ಅನ್ತರ್ಭಾವ "diff.h"
#ಅನ್ತರ್ಭಾವ "diffcore.h"
#ಅನ್ತರ್ಭಾವ "tree.h"
#ಅನ್ತರ್ಭಾವ "branch.h"
#ಅನ್ತರ್ಭಾವ "object-name.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "path.h"
#ಅನ್ತರ್ಭಾವ "repository.h"
#ಅನ್ತರ್ಭಾವ "unpack-trees.h"
#ಅನ್ತರ್ಭಾವ "cache-tree.h"
#ಅನ್ತರ್ಭಾವ "setup.h"
#ಅನ್ತರ್ಭಾವ "sparse-index.h"
#ಅನ್ತರ್ಭಾವ "submodule.h"
#ಅನ್ತರ್ಭಾವ "trace.h"
#ಅನ್ತರ್ಭಾವ "trace2.h"
#ಅನ್ತರ್ಭಾವ "dir.h"
#ಅನ್ತರ್ಭಾವ "add-interactive.h"

#ವ್ಯಾಖ್ಯಾನಿಸು REFRESH_ಸೂಚ್ಯಂಕ_DELAY_WARNING_ಒಳ_MS (2 * 1000)

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ git_reset_usage[] = {
	N_("git reset [--mixed | --soft | --hard | --merge | --keep] [-q] [<commit>]"),
	N_("git reset [-q] [<tree-ish>] [--] <pathspec>..."),
	N_("git reset [-q] [--pathspec-from-file [--pathspec-file-nul]] [<tree-ish>]"),
	N_("git reset --patch [<tree-ish>] [--] [<pathspec>...]"),
	ಖಾಲಿ
};

ಎಣಿಕೆ reset_type { MIXED, SOFT, HARD, ವಿಲೀನ_2, KEEP, NONE };
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *reset_type_ಹೆಸರುಗಳು[] = {
	N_("mixed"), N_("soft"), N_("hard"), N_("merge"), N_("keep"), ಖಾಲಿ
};

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ is_ವಿಲೀನ(ಶೂನ್ಯ)
{
	ಹಿಂತಿರುಗಿ !access(git_ಮಾರ್ಗ_ವಿಲೀನ_ತಲೆ(the_repository), F_OK);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ reset_ಸೂಚ್ಯಂಕ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ref, ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid, ಪೂರ್ಣಾಂಕ reset_type, ಪೂರ್ಣಾಂಕ quiet)
{
	ಪೂರ್ಣಾಂಕ i, nr = 0;
	ರಚನೆ ವೃಕ್ಷ_desc desc[2];
	ರಚನೆ ವೃಕ್ಷ *ವೃಕ್ಷ;
	ರಚನೆ unpack_trees_options opts;
	ಪೂರ್ಣಾಂಕ ret = -1;

	memset(&opts, 0, ಗಾತ್ರ(opts));
	opts.ತಲೆ_idx = 1;
	opts.src_ಸೂಚ್ಯಂಕ = the_repository->ಸೂಚ್ಯಂಕ;
	opts.dst_ಸೂಚ್ಯಂಕ = the_repository->ಸೂಚ್ಯಂಕ;
	opts.fn = oneway_ವಿಲೀನ;
	opts.ವಿಲೀನ = 1;
	init_checkout_metadata(&opts.meta, ref, oid, ಖಾಲಿ);
	ಆದರೆ (!quiet)
		opts.verbose_update = 1;
	ಆಯ್ಕೆ (reset_type) {
	ಸ್ಥಿತಿ KEEP:
	ಸ್ಥಿತಿ ವಿಲೀನ_2:
		opts.update = 1;
		opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ HARD:
		opts.update = 1;
		opts.reset = UNPACK_RESET_OVERWRITE_UNTRACKED;
		opts.skip_cache_ವೃಕ್ಷ_update = 1;
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ MIXED:
		opts.reset = UNPACK_RESET_PROTECT_UNTRACKED;
		opts.skip_cache_ವೃಕ್ಷ_update = 1;
		/* but opts.update=0, so working tree not updated */
		ನಿಲ್ಲಿಸು;
	ಪೂರ್ವನಿಯೋಜಿತ:
		BUG("invalid reset_type passed to reset_index");
	}

	repo_read_ಸೂಚ್ಯಂಕ_unmerged(the_repository);

	ಆದರೆ (reset_type == KEEP) {
		ರಚನೆ object_id ತಲೆ_oid;
		ಆದರೆ (repo_get_oid(the_repository, "HEAD", &ತಲೆ_oid))
			ಹಿಂತಿರುಗಿ error(_("You do not have a valid HEAD."));
		ಆದರೆ (!fill_ವೃಕ್ಷ_descriptor(the_repository, desc + nr, &ತಲೆ_oid))
			ಹಿಂತಿರುಗಿ error(_("Failed to find tree of HEAD."));
		nr++;
		opts.fn = twoway_ವಿಲೀನ;
	}

	ಆದರೆ (!fill_ವೃಕ್ಷ_descriptor(the_repository, desc + nr, oid)) {
		error(_("Failed to find tree of %s."), oid_ಗಮ್ಯ_hex(oid));
		ಹೋಗು ಹೊರಹೋಗು;
	}
	nr++;

	ಆದರೆ (unpack_trees(nr, desc, &opts))
		ಹೋಗು ಹೊರಹೋಗು;

	ಆದರೆ (reset_type == MIXED || reset_type == HARD) {
		ವೃಕ್ಷ = repo_parse_ವೃಕ್ಷ_ಪರೋಕ್ಷ(the_repository, oid);
		ಆದರೆ (!ವೃಕ್ಷ) {
			error(_("unable to read tree (%s)"), oid_ಗಮ್ಯ_hex(oid));
			ಹೋಗು ಹೊರಹೋಗು;
		}
		ಅವಿಭಾಜ್ಯ_cache_ವೃಕ್ಷ(the_repository, the_repository->ಸೂಚ್ಯಂಕ, ವೃಕ್ಷ);
	}

	ret = 0;

ಹೊರಹೋಗು:
	ಗಾಗಿ (i = 0; i < nr; i++)
		ಬಿಡುಗಡೆ((ಶೂನ್ಯ *)desc[i].ಬಫರ್);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಶೂನ್ಯ print_new_ತಲೆ_ಸಾಲು(ರಚನೆ commit *commit)
{
	ರಚನೆ strbuf buf = STRBUF_INIT;

	ಬರೆ(_("HEAD is now at %s"),
		repo_ಕಂಡುಹಿಡಿ_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV));

	pp_commit_easy(CMIT_FMT_ONELINE, commit, &buf);
	ಆದರೆ (buf.len > 0)
		ಬರೆ(" %s", buf.buf);
	ಅಕ್ಷರ_ಬರೆ('\n');
	strbuf_release(&buf);
}

ಸ್ಥಿರ ಶೂನ್ಯ update_ಸೂಚ್ಯಂಕ_from_diff(ರಚನೆ diff_ಸರತಿ_struct *q,
				   ರಚನೆ diff_options *opt ಬಳಸದ,
				   ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
	ಪೂರ್ಣಾಂಕ i;
	ಪೂರ್ಣಾಂಕ intent_ಗಮ್ಯ_ಕೂಡಿಸು = *(ಪೂರ್ಣಾಂಕ *)ದತ್ತಾಂಶ;

	ಗಾಗಿ (i = 0; i < q->nr; i++) {
		ಪೂರ್ಣಾಂಕ pos;
		ರಚನೆ diff_filespec *one = q->ಸರತಿ[i]->one;
		ಪೂರ್ಣಾಂಕ is_ಒಳ_reset_ವೃಕ್ಷ = one->ಬಹುಲಕ && !is_null_oid(&one->oid);
		ರಚನೆ cache_ನಮೂದು *ce;

		ಆದರೆ (!is_ಒಳ_reset_ವೃಕ್ಷ && !intent_ಗಮ್ಯ_ಕೂಡಿಸು) {
			remove_file_from_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ, one->ಮಾರ್ಗ);
			ಮುಂದುವರಿ;
		}

		ce = make_cache_ನಮೂದು(the_repository->ಸೂಚ್ಯಂಕ, one->ಬಹುಲಕ, &one->oid, one->ಮಾರ್ಗ,
				      0, 0);

		/*
		 * If the file 1) corresponds to an existing index entry with
		 * skip-worktree set, or 2) does not exist in the index but is
		 * outside the sparse checkout definition, add a skip-worktree bit
		 * to the new index entry. Note that a sparse index will be expanded
		 * if this entry is outside the sparse cone - this is necessary
		 * to properly construct the reset sparse directory.
		 */
		pos = ಸೂಚ್ಯಂಕ_name_pos(the_repository->ಸೂಚ್ಯಂಕ, one->ಮಾರ್ಗ, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(one->ಮಾರ್ಗ));
		ಆದರೆ ((pos >= 0 && ce_skip_worktree(the_repository->ಸೂಚ್ಯಂಕ->cache[pos])) ||
		    (pos < 0 && !ಮಾರ್ಗ_ಒಳ_sparse_checkout(one->ಮಾರ್ಗ, the_repository->ಸೂಚ್ಯಂಕ)))
			ce->ce_flags |= CE_SKIP_WORKTREE;

		ಆದರೆ (!ce)
			die(_("make_cache_entry failed for path '%s'"),
			    one->ಮಾರ್ಗ);
		ಆದರೆ (!is_ಒಳ_reset_ವೃಕ್ಷ) {
			ce->ce_flags |= CE_INTENT_ಗಮ್ಯ_ಕೂಡಿಸು;
			ಗಣ_object_name_for_intent_ಗಮ್ಯ_ಕೂಡಿಸು_ನಮೂದು(ce);
		}
		ಕೂಡಿಸು_ಸೂಚ್ಯಂಕ_ನಮೂದು(the_repository->ಸೂಚ್ಯಂಕ, ce,
				ಕೂಡಿಸು_CACHE_OK_ಗಮ್ಯ_ಕೂಡಿಸು | ಕೂಡಿಸು_CACHE_OK_ಗಮ್ಯ_REPLACE);
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ read_from_ವೃಕ್ಷ(ಸ್ಥಿರಾಂಕ ರಚನೆ pathspec *pathspec,
			  ರಚನೆ object_id *ವೃಕ್ಷ_oid,
			  ಪೂರ್ಣಾಂಕ intent_ಗಮ್ಯ_ಕೂಡಿಸು)
{
	ರಚನೆ diff_options opt;

	memset(&opt, 0, ಗಾತ್ರ(opt));
	copy_pathspec(&opt.pathspec, pathspec);
	opt.output_format = DIFF_FORMAT_CALLBACK;
	opt.format_callback = update_ಸೂಚ್ಯಂಕ_from_diff;
	opt.format_callback_ದತ್ತಾಂಶ = &intent_ಗಮ್ಯ_ಕೂಡಿಸು;
	opt.flags.override_submodule_config = 1;
	opt.flags.recursive = 1;
	opt.repo = the_repository;
	opt.change = diff_change;
	opt.ಕೂಡಿಸು_remove = diff_addremove;

	ಆದರೆ (pathspec->nr && pathspec_needs_expanded_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ, pathspec))
		ensure_ತುಂಬಿದ_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ);

	ಆದರೆ (do_diff_cache(ವೃಕ್ಷ_oid, &opt))
		ಹಿಂತಿರುಗಿ 1;
	diffcore_std(&opt);
	diff_flush(&opt);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ ಗಣ_reflog_ಸಂದೇಶ(ರಚನೆ strbuf *sb, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *action,
			       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rev)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rla = getenv("GIT_REFLOG_ACTION");

	strbuf_reset(sb);
	ಆದರೆ (rla)
		strbuf_addf(sb, "%s: %s", rla, action);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (rev)
		strbuf_addf(sb, "reset: moving to %s", rev);
	ಇಲ್ಲದಿದ್ದರೆ
		strbuf_addf(sb, "reset: %s", action);
}

ಸ್ಥಿರ ಶೂನ್ಯ die_if_unmerged_cache(ಪೂರ್ಣಾಂಕ reset_type)
{
	ಆದರೆ (is_ವಿಲೀನ() || unmerged_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ))
		die(_("Cannot do a %s reset in the middle of a merge."),
		    _(reset_type_ಹೆಸರುಗಳು[reset_type]));

}

ಸ್ಥಿರ ಶೂನ್ಯ parse_ವಾದಗಳು(ರಚನೆ pathspec *pathspec,
		       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		       ಪೂರ್ಣಾಂಕ patch_ಬಹುಲಕ,
		       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **rev_ret)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rev = "HEAD";
	ರಚನೆ object_id ಬಳಸದ_2;
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

	ಆದರೆ (argv[0]) {
		ಆದರೆ (!strcmp(argv[0], "--")) {
			argv++; /* reset to HEAD, possibly with paths */
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (argv[1] && !strcmp(argv[1], "--")) {
			rev = argv[0];
			argv += 2;
		}
		/*
		 * Otherwise, argv[0] could be either <rev> or <paths> and
		 * has to be unambiguous. If there is a single argument, it
		 * can not be a tree
		 */
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ((!argv[1] && !repo_get_oid_committish(the_repository, argv[0], &ಬಳಸದ_2)) ||
			 (argv[1] && !repo_get_oid_treeish(the_repository, argv[0], &ಬಳಸದ_2))) {
			/*
			 * Ok, argv[0] looks like a commit/tree; it should not
			 * be a filename.
			 */
			verify_non_filename(the_repository, ಪೂರ್ವಪ್ರತ್ಯಯ, argv[0]);
			rev = *argv++;
		} ಇಲ್ಲದಿದ್ದರೆ {
			/* Otherwise we treat this as a filename */
			verify_filename(the_repository, ಪೂರ್ವಪ್ರತ್ಯಯ, argv[0], 1);
		}
	}

	/* treat '@' as a shortcut for 'HEAD' */
	*rev_ret = !strcmp("@", rev) ? "HEAD" : rev;

	parse_pathspec(pathspec, 0,
		       PATHSPEC_PREFER_ತುಂಬಿದ |
		       (patch_ಬಹುಲಕ ? PATHSPEC_ಪೂರ್ವಪ್ರತ್ಯಯ_ORIGIN : 0),
		       ಪೂರ್ವಪ್ರತ್ಯಯ, argv);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ reset_refs(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rev, ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid)
{
	ಪೂರ್ಣಾಂಕ update_ref_ಸ್ಥಿತಿ;
	ರಚನೆ strbuf ಸಂದೇಶ = STRBUF_INIT;
	ರಚನೆ object_id *orig = ಖಾಲಿ, oid_orig,
		*old_orig = ಖಾಲಿ, oid_old_orig;

	ಆದರೆ (!repo_get_oid(the_repository, "ORIG_HEAD", &oid_old_orig))
		old_orig = &oid_old_orig;
	ಆದರೆ (!repo_get_oid(the_repository, "HEAD", &oid_orig)) {
		orig = &oid_orig;
		ಗಣ_reflog_ಸಂದೇಶ(&ಸಂದೇಶ, "updating ORIG_HEAD", ಖಾಲಿ);
		refs_update_ref(get_main_ref_store(the_repository), ಸಂದೇಶ.buf,
				"ORIG_HEAD", orig, old_orig, 0,
				UPDATE_REFS_ಸಂದೇಶ_ON_ERR);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (old_orig)
		refs_delete_ref(get_main_ref_store(the_repository), ಖಾಲಿ,
				"ORIG_HEAD", old_orig, 0);
	ಗಣ_reflog_ಸಂದೇಶ(&ಸಂದೇಶ, "updating HEAD", rev);
	update_ref_ಸ್ಥಿತಿ = refs_update_ref(get_main_ref_store(the_repository),
					    ಸಂದೇಶ.buf, "HEAD", oid, orig, 0,
					    UPDATE_REFS_ಸಂದೇಶ_ON_ERR);
	strbuf_release(&ಸಂದೇಶ);
	ಹಿಂತಿರುಗಿ update_ref_ಸ್ಥಿತಿ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ git_reset_config(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *var, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ,
			    ಸ್ಥಿರಾಂಕ ರಚನೆ config_context *ctx, ಶೂನ್ಯ *cb)
{
	ಆದರೆ (!strcmp(var, "submodule.recurse"))
		ಹಿಂತಿರುಗಿ git_default_submodule_config(var, ಮೌಲ್ಯ, cb);

	ಹಿಂತಿರುಗಿ git_default_config(var, ಮೌಲ್ಯ, ctx, cb);
}

ಪೂರ್ಣಾಂಕ cmd_reset(ಪೂರ್ಣಾಂಕ argc,
	      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
	      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
	      ರಚನೆ repository *repo ಬಳಸದ)
{
	ಪೂರ್ಣಾಂಕ reset_type = NONE, update_ref_ಸ್ಥಿತಿ = 0, quiet = 0;
	ಪೂರ್ಣಾಂಕ no_refresh = 0;
	ಪೂರ್ಣಾಂಕ patch_ಬಹುಲಕ = 0, pathspec_file_nul = 0, unborn;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rev;
	ಅಕ್ಷರ *pathspec_from_file = ಖಾಲಿ;
	ರಚನೆ object_id oid;
	ರಚನೆ pathspec pathspec;
	ಪೂರ್ಣಾಂಕ intent_ಗಮ್ಯ_ಕೂಡಿಸು = 0;
	ರಚನೆ interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	ಸ್ಥಿರಾಂಕ ರಚನೆ option options[] = {
		OPT__QUIET(&quiet, N_("be quiet, only report errors")),
		OPT_BOOL(0, "no-refresh", &no_refresh,
				N_("skip refreshing the index after reset")),
		OPT_ಗಣ_INT_F(0, "mixed", &reset_type,
			      N_("reset HEAD and index"),
			      MIXED, PARSE_OPT_NONEG),
		OPT_ಗಣ_INT_F(0, "soft", &reset_type,
			      N_("reset only HEAD"),
			      SOFT, PARSE_OPT_NONEG),
		OPT_ಗಣ_INT_F(0, "hard", &reset_type,
			      N_("reset HEAD, index and working tree"),
			      HARD, PARSE_OPT_NONEG),
		OPT_ಗಣ_INT_F(0, "merge", &reset_type,
			      N_("reset HEAD, index and working tree"),
			      ವಿಲೀನ_2, PARSE_OPT_NONEG),
		OPT_ಗಣ_INT_F(0, "keep", &reset_type,
			      N_("reset HEAD but keep local changes"),
			      KEEP, PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "recurse-submodules", ಖಾಲಿ,
			       "reset", "control recursive updating of submodules",
			       PARSE_OPT_OPTARG,
			       option_parse_recurse_submodules_worktree_updater),
		OPT_BOOL('p', "patch", &patch_ಬಹುಲಕ, N_("select hunks interactively")),
		OPT_BOOL(0, "auto-advance", &interactive_opts.auto_advance,
			 N_("auto advance to the next file when selecting hunks interactively")),
		OPT_DIFF_UNIFIED(&interactive_opts.context),
		OPT_DIFF_INTERHUNK_CONTEXT(&interactive_opts.interhunkcontext),
		OPT_BOOL('N', "intent-to-add", &intent_ಗಮ್ಯ_ಕೂಡಿಸು,
				N_("record only the fact that removed paths will be added later")),
		OPT_PATHSPEC_FROM_FILE(&pathspec_from_file),
		OPT_PATHSPEC_FILE_NUL(&pathspec_file_nul),
		OPT_ಕೊನೆ()
	};

	repo_config(the_repository, git_reset_config, ಖಾಲಿ);

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options, git_reset_usage,
						PARSE_OPT_KEEP_DASHDASH);
	parse_ವಾದಗಳು(&pathspec, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, patch_ಬಹುಲಕ, &rev);

	ಆದರೆ (pathspec_from_file) {
		ಆದರೆ (patch_ಬಹುಲಕ)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--patch");

		ಆದರೆ (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_ತುಂಬಿದ,
				    ಪೂರ್ವಪ್ರತ್ಯಯ, pathspec_from_file, pathspec_file_nul);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	unborn = !strcmp(rev, "HEAD") && repo_get_oid(the_repository, "HEAD",
						      &oid);
	ಆದರೆ (unborn) {
		/* reset on unborn branch: treat as reset to empty tree */
		oidcpy(&oid, the_hash_algo->ಖಾಲಿಗಳು_ವೃಕ್ಷ);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!pathspec.nr && !patch_ಬಹುಲಕ) {
		ರಚನೆ commit *commit;
		ಆದರೆ (repo_get_oid_committish(the_repository, rev, &oid))
			die(_("Failed to resolve '%s' as a valid revision."), rev);
		commit = lookup_commit_reference(the_repository, &oid);
		ಆದರೆ (!commit)
			die(_("Could not parse object '%s'."), rev);
		oidcpy(&oid, &commit->object.oid);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ರಚನೆ ವೃಕ್ಷ *ವೃಕ್ಷ;
		ಆದರೆ (repo_get_oid_treeish(the_repository, rev, &oid))
			die(_("Failed to resolve '%s' as a valid tree."), rev);
		ವೃಕ್ಷ = repo_parse_ವೃಕ್ಷ_ಪರೋಕ್ಷ(the_repository, &oid);
		ಆದರೆ (!ವೃಕ್ಷ)
			die(_("Could not parse object '%s'."), rev);
		oidcpy(&oid, &ವೃಕ್ಷ->object.oid);
	}

	ಆದರೆ (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	ಆದರೆ (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_ತುಂಬಿದ_ಸೂಚ್ಯಂಕ = 0;

	ಆದರೆ (patch_ಬಹುಲಕ) {
		ಆದರೆ (reset_type != NONE)
			die(_("options '%s' and '%s' cannot be used together"), "--patch", "--{hard,mixed,soft}");
		trace2_cmd_ಬಹುಲಕ("patch-interactive");
		update_ref_ಸ್ಥಿತಿ = !!ಸತತ_ಕೂಡಿಸು_p(the_repository, ಕೂಡಿಸು_P_RESET,
						&interactive_opts, rev, &pathspec, 0);
		ಹೋಗು cleanup;
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಆದರೆ (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--patch");
		ಆದರೆ (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--patch");
		ಆದರೆ (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--patch");
	}

	/* git reset tree [--] paths... can be used to
	 * load chosen paths from the tree into the index without
	 * affecting the working tree nor HEAD. */
	ಆದರೆ (pathspec.nr) {
		ಆದರೆ (reset_type == MIXED)
			warning(_("--mixed with paths is deprecated; use 'git reset -- <paths>' instead."));
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (reset_type != NONE)
			die(_("Cannot do %s reset with paths."),
					_(reset_type_ಹೆಸರುಗಳು[reset_type]));
	}
	ಆದರೆ (reset_type == NONE)
		reset_type = MIXED; /* by default */

	ಆದರೆ (pathspec.nr)
		trace2_cmd_ಬಹುಲಕ("path");
	ಇಲ್ಲದಿದ್ದರೆ
		trace2_cmd_ಬಹುಲಕ(reset_type_ಹೆಸರುಗಳು[reset_type]);

	ಆದರೆ (reset_type != SOFT && (reset_type != MIXED || repo_get_work_ವೃಕ್ಷ(the_repository)))
		ಸಿದ್ಧತೆ_work_ವೃಕ್ಷ(the_repository);

	ಆದರೆ (reset_type == MIXED && is_bare_repository(the_repository))
		die(_("%s reset is not allowed in a bare repository"),
		    _(reset_type_ಹೆಸರುಗಳು[reset_type]));

	ಆದರೆ (intent_ಗಮ್ಯ_ಕೂಡಿಸು && reset_type != MIXED)
		die(_("the option '%s' requires '%s'"), "-N", "--mixed");

	ಆದರೆ (repo_read_ಸೂಚ್ಯಂಕ(the_repository) < 0)
		die(_("index file corrupt"));

	/* Soft reset does not touch the index file nor the working tree
	 * at all, but requires them in a good order.  Other resets reset
	 * the index file to the tree object we are switching to. */
	ಆದರೆ (reset_type == SOFT || reset_type == KEEP)
		die_if_unmerged_cache(reset_type);

	ಆದರೆ (reset_type != SOFT) {
		ರಚನೆ ಬೀಗ_file ಬೀಗ = ಬೀಗ_INIT;
		repo_hold_locked_ಸೂಚ್ಯಂಕ(the_repository, &ಬೀಗ,
				       ಬೀಗ_DIE_ON_ERROR);
		ಆದರೆ (reset_type == MIXED) {
			ಪೂರ್ಣಾಂಕ flags = quiet ? REFRESH_QUIET : REFRESH_ಒಳ_PORCELAIN;
			ಆದರೆ (read_from_ವೃಕ್ಷ(&pathspec, &oid, intent_ಗಮ್ಯ_ಕೂಡಿಸು)) {
				update_ref_ಸ್ಥಿತಿ = 1;
				ಹೋಗು cleanup;
			}
			the_repository->ಸೂಚ್ಯಂಕ->updated_skipworktree = 1;
			ಆದರೆ (!no_refresh && repo_get_work_ವೃಕ್ಷ(the_repository)) {
				uint64_t t_begin, t_delta_ಒಳ_ms;

				t_begin = getnanotime();
				refresh_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ, flags, ಖಾಲಿ, ಖಾಲಿ,
					      _("Unstaged changes after reset:"));
				t_delta_ಒಳ_ms = (getnanotime() - t_begin) / 1000000;
				ಆದರೆ (!quiet && advice_enabled(ADVICE_RESET_NO_REFRESH_WARNING) && t_delta_ಒಳ_ms > REFRESH_ಸೂಚ್ಯಂಕ_DELAY_WARNING_ಒಳ_MS) {
					advise(_("It took %.2f seconds to refresh the index after reset.  You can use\n"
						 "'--no-refresh' to avoid this."), t_delta_ಒಳ_ms / 1000.0);
				}
			}
		} ಇಲ್ಲದಿದ್ದರೆ {
			ರಚನೆ object_id dummy;
			ಅಕ್ಷರ *ref = ಖಾಲಿ;
			ಪೂರ್ಣಾಂಕ err;

			repo_dwim_ref(the_repository, rev, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(rev),
				      &dummy, &ref, 0);
			ಆದರೆ (ref && !starts_with(ref, "refs/"))
				FREE_AND_NULL(ref);

			err = reset_ಸೂಚ್ಯಂಕ(ref, &oid, reset_type, quiet);
			ಆದರೆ (reset_type == KEEP && !err)
				err = reset_ಸೂಚ್ಯಂಕ(ref, &oid, MIXED, quiet);
			ಆದರೆ (err)
				die(_("Could not reset index file to revision '%s'."), rev);
			ಬಿಡುಗಡೆ(ref);
		}

		ಆದರೆ (write_locked_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ, &ಬೀಗ, COMMIT_ಬೀಗ))
			die(_("Could not write new index file."));
	}

	ಆದರೆ (!pathspec.nr && !unborn) {
		/* Any resets without paths update HEAD to the head being
		 * switched to, saving the previous head in ORIG_HEAD before. */
		update_ref_ಸ್ಥಿತಿ = reset_refs(rev, &oid);

		ಆದರೆ (reset_type == HARD && !update_ref_ಸ್ಥಿತಿ && !quiet)
			print_new_ತಲೆ_ಸಾಲು(lookup_commit_reference(the_repository, &oid));
	}
	ಆದರೆ (!pathspec.nr)
		remove_branch_state(the_repository, 0);

	discard_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ);

cleanup:
	clear_pathspec(&pathspec);
	ಬಿಡುಗಡೆ(pathspec_from_file);
	ಹಿಂತಿರುಗಿ update_ref_ಸ್ಥಿತಿ;
}
