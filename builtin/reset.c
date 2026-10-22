/*
 * "git reset" builtin command
 *
 * Copyright (c) 2007 Carlos Rica
 *
 * Based on git-reset.sh, which is
 *
 * Copyright (c) 2005, 2006 Linus Torvalds and Junio C Hamano
 */

#define USE_THE_REPOSITORY_VARIABLE

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

#define REFRESH_INDEX_DELAY_WARNING_IN_MS (2 * 1000)

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ git_reset_usage[] = {
	N_("git reset [--mixed | --soft | --hard | --merge | --keep] [-q] [<commit>]"),
	N_("git reset [-q] [<tree-ish>] [--] <pathspec>..."),
	N_("git reset [-q] [--pathspec-from-file [--pathspec-file-nul]] [<tree-ish>]"),
	N_("git reset --patch [<tree-ish>] [--] [<pathspec>...]"),
	ರಿಕ್ತ
};

ಗಣನಾ reset_type { MIXED, SOFT, HARD, MERGE, KEEP, NONE };
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *reset_type_names[] = {
	N_("mixed"), N_("soft"), N_("hard"), N_("merge"), N_("keep"), ರಿಕ್ತ
};

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ is_merge(ಶೂನ್ಯ)
{
	ಪ್ರತ್ಯಾಗಮ !access(git_path_merge_head(the_repository), F_OK);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ reset_index(ಧ್ರುವ ವರ್ಣ *ref, ಧ್ರುವ ಸಂರಚನಾ object_id *oid, ಪೂರ್ಣಾಙ್ಕ reset_type, ಪೂರ್ಣಾಙ್ಕ quiet)
{
	ಪೂರ್ಣಾಙ್ಕ i, nr = 0;
	ಸಂರಚನಾ ವೃಕ್ಷ_desc desc[2];
	ಸಂರಚನಾ ವೃಕ್ಷ *ವೃಕ್ಷ;
	ಸಂರಚನಾ unpack_trees_options opts;
	ಪೂರ್ಣಾಙ್ಕ ret = -1;

	memset(&opts, 0, ಪರಿಮಾಣ(opts));
	opts.head_idx = 1;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;
	opts.fn = oneway_merge;
	opts.merge = 1;
	init_checkout_metadata(&opts.meta, ref, oid, ರಿಕ್ತ);
	ಯದಿ (!quiet)
		opts.verbose_update = 1;
	ವಿಕಲ್ಪ (reset_type) {
	ಪಕ್ಷ KEEP:
	ಪಕ್ಷ MERGE:
		opts.update = 1;
		opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
		ಭಙ್ಗ;
	ಪಕ್ಷ HARD:
		opts.update = 1;
		opts.reset = UNPACK_RESET_OVERWRITE_UNTRACKED;
		opts.skip_cache_ವೃಕ್ಷ_update = 1;
		ಭಙ್ಗ;
	ಪಕ್ಷ MIXED:
		opts.reset = UNPACK_RESET_PROTECT_UNTRACKED;
		opts.skip_cache_ವೃಕ್ಷ_update = 1;
		/* but opts.update=0, so working tree not updated */
		ಭಙ್ಗ;
	ಸ್ವಭಾವ:
		BUG("invalid reset_type passed to reset_index");
	}

	repo_read_index_unmerged(the_repository);

	ಯದಿ (reset_type == KEEP) {
		ಸಂರಚನಾ object_id head_oid;
		ಯದಿ (repo_get_oid(the_repository, "HEAD", &head_oid))
			ಪ್ರತ್ಯಾಗಮ error(_("You do not have a valid HEAD."));
		ಯದಿ (!fill_ವೃಕ್ಷ_descriptor(the_repository, desc + nr, &head_oid))
			ಪ್ರತ್ಯಾಗಮ error(_("Failed to find tree of HEAD."));
		nr++;
		opts.fn = twoway_merge;
	}

	ಯದಿ (!fill_ವೃಕ್ಷ_descriptor(the_repository, desc + nr, oid)) {
		error(_("Failed to find tree of %s."), oid_to_hex(oid));
		ಗಚ್ಛ out;
	}
	nr++;

	ಯದಿ (unpack_trees(nr, desc, &opts))
		ಗಚ್ಛ out;

	ಯದಿ (reset_type == MIXED || reset_type == HARD) {
		ವೃಕ್ಷ = repo_parse_ವೃಕ್ಷ_indirect(the_repository, oid);
		ಯದಿ (!ವೃಕ್ಷ) {
			error(_("unable to read tree (%s)"), oid_to_hex(oid));
			ಗಚ್ಛ out;
		}
		ಅವಿಭಾಜ್ಯ_cache_ವೃಕ್ಷ(the_repository, the_repository->index, ವೃಕ್ಷ);
	}

	ret = 0;

out:
	ಪ್ರತಿ (i = 0; i < nr; i++)
		ಮುಕ್ತ((ಶೂನ್ಯ *)desc[i].buffer);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಶೂನ್ಯ print_new_head_ಸಾಲು(ಸಂರಚನಾ commit *commit)
{
	ಸಂರಚನಾ strbuf buf = STRBUF_INIT;

	printf(_("HEAD is now at %s"),
		repo_find_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV));

	pp_commit_easy(CMIT_FMT_ONELINE, commit, &buf);
	ಯದಿ (buf.len > 0)
		printf(" %s", buf.buf);
	putchar('\n');
	strbuf_release(&buf);
}

ಸ್ಥಿರ ಶೂನ್ಯ update_index_from_diff(ಸಂರಚನಾ diff_queue_struct *q,
				   ಸಂರಚನಾ diff_options *opt UNUSED,
				   ಶೂನ್ಯ *data)
{
	ಪೂರ್ಣಾಙ್ಕ i;
	ಪೂರ್ಣಾಙ್ಕ intent_to_add = *(ಪೂರ್ಣಾಙ್ಕ *)data;

	ಪ್ರತಿ (i = 0; i < q->nr; i++) {
		ಪೂರ್ಣಾಙ್ಕ pos;
		ಸಂರಚನಾ diff_filespec *one = q->queue[i]->one;
		ಪೂರ್ಣಾಙ್ಕ is_in_reset_ವೃಕ್ಷ = one->mode && !is_null_oid(&one->oid);
		ಸಂರಚನಾ cache_entry *ce;

		ಯದಿ (!is_in_reset_ವೃಕ್ಷ && !intent_to_add) {
			remove_file_from_index(the_repository->index, one->path);
			ಅನುವರ್ತ;
		}

		ce = make_cache_entry(the_repository->index, one->mode, &one->oid, one->path,
				      0, 0);

		/*
		 * If the file 1) corresponds to an existing index entry with
		 * skip-worktree set, or 2) does not exist in the index but is
		 * outside the sparse checkout definition, add a skip-worktree bit
		 * to the new index entry. Note that a sparse index will be expanded
		 * if this entry is outside the sparse cone - this is necessary
		 * to properly construct the reset sparse directory.
		 */
		pos = index_name_pos(the_repository->index, one->path, strlen(one->path));
		ಯದಿ ((pos >= 0 && ce_skip_worktree(the_repository->index->cache[pos])) ||
		    (pos < 0 && !path_in_sparse_checkout(one->path, the_repository->index)))
			ce->ce_flags |= CE_SKIP_WORKTREE;

		ಯದಿ (!ce)
			die(_("make_cache_entry failed for path '%s'"),
			    one->path);
		ಯದಿ (!is_in_reset_ವೃಕ್ಷ) {
			ce->ce_flags |= CE_INTENT_TO_ADD;
			set_object_name_for_intent_to_add_entry(ce);
		}
		add_index_entry(the_repository->index, ce,
				ADD_CACHE_OK_TO_ADD | ADD_CACHE_OK_TO_REPLACE);
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ read_from_ವೃಕ್ಷ(ಧ್ರುವ ಸಂರಚನಾ pathspec *pathspec,
			  ಸಂರಚನಾ object_id *ವೃಕ್ಷ_oid,
			  ಪೂರ್ಣಾಙ್ಕ intent_to_add)
{
	ಸಂರಚನಾ diff_options opt;

	memset(&opt, 0, ಪರಿಮಾಣ(opt));
	copy_pathspec(&opt.pathspec, pathspec);
	opt.output_format = DIFF_FORMAT_CALLBACK;
	opt.format_callback = update_index_from_diff;
	opt.format_callback_data = &intent_to_add;
	opt.flags.override_submodule_config = 1;
	opt.flags.recursive = 1;
	opt.repo = the_repository;
	opt.change = diff_change;
	opt.add_remove = diff_addremove;

	ಯದಿ (pathspec->nr && pathspec_needs_expanded_index(the_repository->index, pathspec))
		ensure_full_index(the_repository->index);

	ಯದಿ (do_diff_cache(ವೃಕ್ಷ_oid, &opt))
		ಪ್ರತ್ಯಾಗಮ 1;
	diffcore_std(&opt);
	diff_flush(&opt);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ set_reflog_message(ಸಂರಚನಾ strbuf *sb, ಧ್ರುವ ವರ್ಣ *action,
			       ಧ್ರುವ ವರ್ಣ *rev)
{
	ಧ್ರುವ ವರ್ಣ *rla = getenv("GIT_REFLOG_ACTION");

	strbuf_reset(sb);
	ಯದಿ (rla)
		strbuf_addf(sb, "%s: %s", rla, action);
	ಅನ್ಯಥಾ ಯದಿ (rev)
		strbuf_addf(sb, "reset: moving to %s", rev);
	ಅನ್ಯಥಾ
		strbuf_addf(sb, "reset: %s", action);
}

ಸ್ಥಿರ ಶೂನ್ಯ die_if_unmerged_cache(ಪೂರ್ಣಾಙ್ಕ reset_type)
{
	ಯದಿ (is_merge() || unmerged_index(the_repository->index))
		die(_("Cannot do a %s reset in the middle of a merge."),
		    _(reset_type_names[reset_type]));

}

ಸ್ಥಿರ ಶೂನ್ಯ parse_args(ಸಂರಚನಾ pathspec *pathspec,
		       ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
		       ಪೂರ್ಣಾಙ್ಕ patch_mode,
		       ಧ್ರುವ ವರ್ಣ **rev_ret)
{
	ಧ್ರುವ ವರ್ಣ *rev = "HEAD";
	ಸಂರಚನಾ object_id unused;
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

	ಯದಿ (argv[0]) {
		ಯದಿ (!strcmp(argv[0], "--")) {
			argv++; /* reset to HEAD, possibly with paths */
		} ಅನ್ಯಥಾ ಯದಿ (argv[1] && !strcmp(argv[1], "--")) {
			rev = argv[0];
			argv += 2;
		}
		/*
		 * Otherwise, argv[0] could be either <rev> or <paths> and
		 * has to be unambiguous. If there is a single argument, it
		 * can not be a tree
		 */
		ಅನ್ಯಥಾ ಯದಿ ((!argv[1] && !repo_get_oid_committish(the_repository, argv[0], &unused)) ||
			 (argv[1] && !repo_get_oid_treeish(the_repository, argv[0], &unused))) {
			/*
			 * Ok, argv[0] looks like a commit/tree; it should not
			 * be a filename.
			 */
			verify_non_filename(the_repository, prefix, argv[0]);
			rev = *argv++;
		} ಅನ್ಯಥಾ {
			/* Otherwise we treat this as a filename */
			verify_filename(the_repository, prefix, argv[0], 1);
		}
	}

	/* treat '@' as a shortcut for 'HEAD' */
	*rev_ret = !strcmp("@", rev) ? "HEAD" : rev;

	parse_pathspec(pathspec, 0,
		       PATHSPEC_PREFER_FULL |
		       (patch_mode ? PATHSPEC_PREFIX_ORIGIN : 0),
		       prefix, argv);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ reset_refs(ಧ್ರುವ ವರ್ಣ *rev, ಧ್ರುವ ಸಂರಚನಾ object_id *oid)
{
	ಪೂರ್ಣಾಙ್ಕ update_ref_status;
	ಸಂರಚನಾ strbuf ಸಂದೇಶ = STRBUF_INIT;
	ಸಂರಚನಾ object_id *orig = ರಿಕ್ತ, oid_orig,
		*old_orig = ರಿಕ್ತ, oid_old_orig;

	ಯದಿ (!repo_get_oid(the_repository, "ORIG_HEAD", &oid_old_orig))
		old_orig = &oid_old_orig;
	ಯದಿ (!repo_get_oid(the_repository, "HEAD", &oid_orig)) {
		orig = &oid_orig;
		set_reflog_message(&ಸಂದೇಶ, "updating ORIG_HEAD", ರಿಕ್ತ);
		refs_update_ref(get_main_ref_store(the_repository), ಸಂದೇಶ.buf,
				"ORIG_HEAD", orig, old_orig, 0,
				UPDATE_REFS_ಸಂದೇಶ_ON_ERR);
	} ಅನ್ಯಥಾ ಯದಿ (old_orig)
		refs_delete_ref(get_main_ref_store(the_repository), ರಿಕ್ತ,
				"ORIG_HEAD", old_orig, 0);
	set_reflog_message(&ಸಂದೇಶ, "updating HEAD", rev);
	update_ref_status = refs_update_ref(get_main_ref_store(the_repository),
					    ಸಂದೇಶ.buf, "HEAD", oid, orig, 0,
					    UPDATE_REFS_ಸಂದೇಶ_ON_ERR);
	strbuf_release(&ಸಂದೇಶ);
	ಪ್ರತ್ಯಾಗಮ update_ref_status;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ git_reset_config(ಧ್ರುವ ವರ್ಣ *var, ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ,
			    ಧ್ರುವ ಸಂರಚನಾ config_context *ctx, ಶೂನ್ಯ *cb)
{
	ಯದಿ (!strcmp(var, "submodule.recurse"))
		ಪ್ರತ್ಯಾಗಮ git_default_submodule_config(var, ಮೌಲ್ಯ, cb);

	ಪ್ರತ್ಯಾಗಮ git_default_config(var, ಮೌಲ್ಯ, ctx, cb);
}

ಪೂರ್ಣಾಙ್ಕ cmd_reset(ಪೂರ್ಣಾಙ್ಕ argc,
	      ಧ್ರುವ ವರ್ಣ **argv,
	      ಧ್ರುವ ವರ್ಣ *prefix,
	      ಸಂರಚನಾ repository *repo UNUSED)
{
	ಪೂರ್ಣಾಙ್ಕ reset_type = NONE, update_ref_status = 0, quiet = 0;
	ಪೂರ್ಣಾಙ್ಕ no_refresh = 0;
	ಪೂರ್ಣಾಙ್ಕ patch_mode = 0, pathspec_file_nul = 0, unborn;
	ಧ್ರುವ ವರ್ಣ *rev;
	ವರ್ಣ *pathspec_from_file = ರಿಕ್ತ;
	ಸಂರಚನಾ object_id oid;
	ಸಂರಚನಾ pathspec pathspec;
	ಪೂರ್ಣಾಙ್ಕ intent_to_add = 0;
	ಸಂರಚನಾ interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	ಧ್ರುವ ಸಂರಚನಾ option options[] = {
		OPT__QUIET(&quiet, N_("be quiet, only report errors")),
		OPT_BOOL(0, "no-refresh", &no_refresh,
				N_("skip refreshing the index after reset")),
		OPT_SET_INT_F(0, "mixed", &reset_type,
			      N_("reset HEAD and index"),
			      MIXED, PARSE_OPT_NONEG),
		OPT_SET_INT_F(0, "soft", &reset_type,
			      N_("reset only HEAD"),
			      SOFT, PARSE_OPT_NONEG),
		OPT_SET_INT_F(0, "hard", &reset_type,
			      N_("reset HEAD, index and working tree"),
			      HARD, PARSE_OPT_NONEG),
		OPT_SET_INT_F(0, "merge", &reset_type,
			      N_("reset HEAD, index and working tree"),
			      MERGE, PARSE_OPT_NONEG),
		OPT_SET_INT_F(0, "keep", &reset_type,
			      N_("reset HEAD but keep local changes"),
			      KEEP, PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "recurse-submodules", ರಿಕ್ತ,
			       "reset", "control recursive updating of submodules",
			       PARSE_OPT_OPTARG,
			       option_parse_recurse_submodules_worktree_updater),
		OPT_BOOL('p', "patch", &patch_mode, N_("select hunks interactively")),
		OPT_BOOL(0, "auto-advance", &interactive_opts.auto_advance,
			 N_("auto advance to the next file when selecting hunks interactively")),
		OPT_DIFF_UNIFIED(&interactive_opts.context),
		OPT_DIFF_INTERHUNK_CONTEXT(&interactive_opts.interhunkcontext),
		OPT_BOOL('N', "intent-to-add", &intent_to_add,
				N_("record only the fact that removed paths will be added later")),
		OPT_PATHSPEC_FROM_FILE(&pathspec_from_file),
		OPT_PATHSPEC_FILE_NUL(&pathspec_file_nul),
		OPT_END()
	};

	repo_config(the_repository, git_reset_config, ರಿಕ್ತ);

	argc = parse_options(argc, argv, prefix, options, git_reset_usage,
						PARSE_OPT_KEEP_DASHDASH);
	parse_args(&pathspec, argv, prefix, patch_mode, &rev);

	ಯದಿ (pathspec_from_file) {
		ಯದಿ (patch_mode)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--patch");

		ಯದಿ (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_FULL,
				    prefix, pathspec_from_file, pathspec_file_nul);
	} ಅನ್ಯಥಾ ಯದಿ (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	unborn = !strcmp(rev, "HEAD") && repo_get_oid(the_repository, "HEAD",
						      &oid);
	ಯದಿ (unborn) {
		/* reset on unborn branch: treat as reset to empty tree */
		oidcpy(&oid, the_hash_algo->empty_ವೃಕ್ಷ);
	} ಅನ್ಯಥಾ ಯದಿ (!pathspec.nr && !patch_mode) {
		ಸಂರಚನಾ commit *commit;
		ಯದಿ (repo_get_oid_committish(the_repository, rev, &oid))
			die(_("Failed to resolve '%s' as a valid revision."), rev);
		commit = lookup_commit_reference(the_repository, &oid);
		ಯದಿ (!commit)
			die(_("Could not parse object '%s'."), rev);
		oidcpy(&oid, &commit->object.oid);
	} ಅನ್ಯಥಾ {
		ಸಂರಚನಾ ವೃಕ್ಷ *ವೃಕ್ಷ;
		ಯದಿ (repo_get_oid_treeish(the_repository, rev, &oid))
			die(_("Failed to resolve '%s' as a valid tree."), rev);
		ವೃಕ್ಷ = repo_parse_ವೃಕ್ಷ_indirect(the_repository, &oid);
		ಯದಿ (!ವೃಕ್ಷ)
			die(_("Could not parse object '%s'."), rev);
		oidcpy(&oid, &ವೃಕ್ಷ->object.oid);
	}

	ಯದಿ (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	ಯದಿ (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	ಯದಿ (patch_mode) {
		ಯದಿ (reset_type != NONE)
			die(_("options '%s' and '%s' cannot be used together"), "--patch", "--{hard,mixed,soft}");
		trace2_cmd_mode("patch-interactive");
		update_ref_status = !!run_add_p(the_repository, ADD_P_RESET,
						&interactive_opts, rev, &pathspec, 0);
		ಗಚ್ಛ cleanup;
	} ಅನ್ಯಥಾ {
		ಯದಿ (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--patch");
		ಯದಿ (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--patch");
		ಯದಿ (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--patch");
	}

	/* git reset tree [--] paths... can be used to
	 * load chosen paths from the tree into the index without
	 * affecting the working tree nor HEAD. */
	ಯದಿ (pathspec.nr) {
		ಯದಿ (reset_type == MIXED)
			warning(_("--mixed with paths is deprecated; use 'git reset -- <paths>' instead."));
		ಅನ್ಯಥಾ ಯದಿ (reset_type != NONE)
			die(_("Cannot do %s reset with paths."),
					_(reset_type_names[reset_type]));
	}
	ಯದಿ (reset_type == NONE)
		reset_type = MIXED; /* by default */

	ಯದಿ (pathspec.nr)
		trace2_cmd_mode("path");
	ಅನ್ಯಥಾ
		trace2_cmd_mode(reset_type_names[reset_type]);

	ಯದಿ (reset_type != SOFT && (reset_type != MIXED || repo_get_work_ವೃಕ್ಷ(the_repository)))
		setup_work_ವೃಕ್ಷ(the_repository);

	ಯದಿ (reset_type == MIXED && is_bare_repository(the_repository))
		die(_("%s reset is not allowed in a bare repository"),
		    _(reset_type_names[reset_type]));

	ಯದಿ (intent_to_add && reset_type != MIXED)
		die(_("the option '%s' requires '%s'"), "-N", "--mixed");

	ಯದಿ (repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));

	/* Soft reset does not touch the index file nor the working tree
	 * at all, but requires them in a good order.  Other resets reset
	 * the index file to the tree object we are switching to. */
	ಯದಿ (reset_type == SOFT || reset_type == KEEP)
		die_if_unmerged_cache(reset_type);

	ಯದಿ (reset_type != SOFT) {
		ಸಂರಚನಾ lock_file lock = LOCK_INIT;
		repo_hold_locked_index(the_repository, &lock,
				       LOCK_DIE_ON_ERROR);
		ಯದಿ (reset_type == MIXED) {
			ಪೂರ್ಣಾಙ್ಕ flags = quiet ? REFRESH_QUIET : REFRESH_IN_PORCELAIN;
			ಯದಿ (read_from_ವೃಕ್ಷ(&pathspec, &oid, intent_to_add)) {
				update_ref_status = 1;
				ಗಚ್ಛ cleanup;
			}
			the_repository->index->updated_skipworktree = 1;
			ಯದಿ (!no_refresh && repo_get_work_ವೃಕ್ಷ(the_repository)) {
				uint64_t t_begin, t_delta_in_ms;

				t_begin = getnanotime();
				refresh_index(the_repository->index, flags, ರಿಕ್ತ, ರಿಕ್ತ,
					      _("Unstaged changes after reset:"));
				t_delta_in_ms = (getnanotime() - t_begin) / 1000000;
				ಯದಿ (!quiet && advice_enabled(ADVICE_RESET_NO_REFRESH_WARNING) && t_delta_in_ms > REFRESH_INDEX_DELAY_WARNING_IN_MS) {
					advise(_("It took %.2f seconds to refresh the index after reset.  You can use\n"
						 "'--no-refresh' to avoid this."), t_delta_in_ms / 1000.0);
				}
			}
		} ಅನ್ಯಥಾ {
			ಸಂರಚನಾ object_id dummy;
			ವರ್ಣ *ref = ರಿಕ್ತ;
			ಪೂರ್ಣಾಙ್ಕ err;

			repo_dwim_ref(the_repository, rev, strlen(rev),
				      &dummy, &ref, 0);
			ಯದಿ (ref && !starts_with(ref, "refs/"))
				FREE_AND_NULL(ref);

			err = reset_index(ref, &oid, reset_type, quiet);
			ಯದಿ (reset_type == KEEP && !err)
				err = reset_index(ref, &oid, MIXED, quiet);
			ಯದಿ (err)
				die(_("Could not reset index file to revision '%s'."), rev);
			ಮುಕ್ತ(ref);
		}

		ಯದಿ (write_locked_index(the_repository->index, &lock, COMMIT_LOCK))
			die(_("Could not write new index file."));
	}

	ಯದಿ (!pathspec.nr && !unborn) {
		/* Any resets without paths update HEAD to the head being
		 * switched to, saving the previous head in ORIG_HEAD before. */
		update_ref_status = reset_refs(rev, &oid);

		ಯದಿ (reset_type == HARD && !update_ref_status && !quiet)
			print_new_head_ಸಾಲು(lookup_commit_reference(the_repository, &oid));
	}
	ಯದಿ (!pathspec.nr)
		remove_branch_state(the_repository, 0);

	discard_index(the_repository->index);

cleanup:
	clear_pathspec(&pathspec);
	ಮುಕ್ತ(pathspec_from_file);
	ಪ್ರತ್ಯಾಗಮ update_ref_status;
}
