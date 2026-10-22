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

#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "advice.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "hash.h"
#ଅନ୍ତର୍ଭାଵ "hex.h"
#ଅନ୍ତର୍ଭାଵ "lockfile.h"
#ଅନ୍ତର୍ଭାଵ "object.h"
#ଅନ୍ତର୍ଭାଵ "pretty.h"
#ଅନ୍ତର୍ଭାଵ "refs.h"
#ଅନ୍ତର୍ଭାଵ "diff.h"
#ଅନ୍ତର୍ଭାଵ "diffcore.h"
#ଅନ୍ତର୍ଭାଵ "tree.h"
#ଅନ୍ତର୍ଭାଵ "branch.h"
#ଅନ୍ତର୍ଭାଵ "object-name.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "path.h"
#ଅନ୍ତର୍ଭାଵ "repository.h"
#ଅନ୍ତର୍ଭାଵ "unpack-trees.h"
#ଅନ୍ତର୍ଭାଵ "cache-tree.h"
#ଅନ୍ତର୍ଭାଵ "setup.h"
#ଅନ୍ତର୍ଭାଵ "sparse-index.h"
#ଅନ୍ତର୍ଭାଵ "submodule.h"
#ଅନ୍ତର୍ଭାଵ "trace.h"
#ଅନ୍ତର୍ଭାଵ "trace2.h"
#ଅନ୍ତର୍ଭାଵ "dir.h"
#ଅନ୍ତର୍ଭାଵ "add-interactive.h"

#define REFRESH_INDEX_DELAY_WARNING_IN_MS (2 * 1000)

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ git_reset_usage[] = {
	N_("git reset [--mixed | --soft | --hard | --merge | --keep] [-q] [<commit>]"),
	N_("git reset [-q] [<tree-ish>] [--] <pathspec>..."),
	N_("git reset [-q] [--pathspec-from-file [--pathspec-file-nul]] [<tree-ish>]"),
	N_("git reset --patch [<tree-ish>] [--] [<pathspec>...]"),
	ଖାଲି
};

ଗଣନା reset_type { MIXED, SOFT, HARD, MERGE, KEEP, NONE };
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *reset_type_names[] = {
	N_("mixed"), N_("soft"), N_("hard"), N_("merge"), N_("keep"), ଖାଲି
};

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା is_merge(ଶୂନ୍ୟ)
{
	ଫେରନ୍ତୁ !access(git_path_merge_head(the_repository), F_OK);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା reset_index(ଅଚଳ ଅକ୍ଷର *ref, ଅଚଳ ସଂରଚନା object_id *oid, ପୂର୍ଣ୍ଣସଂଖ୍ୟା reset_type, ପୂର୍ଣ୍ଣସଂଖ୍ୟା quiet)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, nr = 0;
	ସଂରଚନା ବୃକ୍ଷ_desc desc[2];
	ସଂରଚନା ବୃକ୍ଷ *ବୃକ୍ଷ;
	ସଂରଚନା unpack_trees_options opts;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = -1;

	memset(&opts, 0, ଆକାର(opts));
	opts.head_idx = 1;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;
	opts.fn = oneway_merge;
	opts.merge = 1;
	init_checkout_metadata(&opts.meta, ref, oid, ଖାଲି);
	ଯଦି (!quiet)
		opts.verbose_update = 1;
	ବାଛ (reset_type) {
	ସ୍ଥିତି KEEP:
	ସ୍ଥିତି MERGE:
		opts.update = 1;
		opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
		ଭାଙ୍ଗ;
	ସ୍ଥିତି HARD:
		opts.update = 1;
		opts.reset = UNPACK_RESET_OVERWRITE_UNTRACKED;
		opts.skip_cache_ବୃକ୍ଷ_update = 1;
		ଭାଙ୍ଗ;
	ସ୍ଥିତି MIXED:
		opts.reset = UNPACK_RESET_PROTECT_UNTRACKED;
		opts.skip_cache_ବୃକ୍ଷ_update = 1;
		/* but opts.update=0, so working tree not updated */
		ଭାଙ୍ଗ;
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
		BUG("invalid reset_type passed to reset_index");
	}

	repo_read_index_unmerged(the_repository);

	ଯଦି (reset_type == KEEP) {
		ସଂରଚନା object_id head_oid;
		ଯଦି (repo_get_oid(the_repository, "HEAD", &head_oid))
			ଫେରନ୍ତୁ error(_("You do not have a valid HEAD."));
		ଯଦି (!fill_ବୃକ୍ଷ_descriptor(the_repository, desc + nr, &head_oid))
			ଫେରନ୍ତୁ error(_("Failed to find tree of HEAD."));
		nr++;
		opts.fn = twoway_merge;
	}

	ଯଦି (!fill_ବୃକ୍ଷ_descriptor(the_repository, desc + nr, oid)) {
		error(_("Failed to find tree of %s."), oid_to_hex(oid));
		ଯାଅ out;
	}
	nr++;

	ଯଦି (unpack_trees(nr, desc, &opts))
		ଯାଅ out;

	ଯଦି (reset_type == MIXED || reset_type == HARD) {
		ବୃକ୍ଷ = repo_parse_ବୃକ୍ଷ_indirect(the_repository, oid);
		ଯଦି (!ବୃକ୍ଷ) {
			error(_("unable to read tree (%s)"), oid_to_hex(oid));
			ଯାଅ out;
		}
		ମୌଳିକ_cache_ବୃକ୍ଷ(the_repository, the_repository->index, ବୃକ୍ଷ);
	}

	ret = 0;

out:
	ପାଇଁ (i = 0; i < nr; i++)
		ମୁକ୍ତ((ଶୂନ୍ୟ *)desc[i].buffer);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ଶୂନ୍ୟ print_new_head_ଧାଡ଼ି(ସଂରଚନା commit *commit)
{
	ସଂରଚନା strbuf buf = STRBUF_INIT;

	printf(_("HEAD is now at %s"),
		repo_find_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV));

	pp_commit_easy(CMIT_FMT_ONELINE, commit, &buf);
	ଯଦି (buf.len > 0)
		printf(" %s", buf.buf);
	putchar('\n');
	strbuf_release(&buf);
}

ସ୍ଥିର ଶୂନ୍ୟ update_index_from_diff(ସଂରଚନା diff_queue_struct *q,
				   ସଂରଚନା diff_options *opt UNUSED,
				   ଶୂନ୍ୟ *data)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା intent_to_add = *(ପୂର୍ଣ୍ଣସଂଖ୍ୟା *)data;

	ପାଇଁ (i = 0; i < q->nr; i++) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା pos;
		ସଂରଚନା diff_filespec *one = q->queue[i]->one;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା is_in_reset_ବୃକ୍ଷ = one->mode && !is_null_oid(&one->oid);
		ସଂରଚନା cache_entry *ce;

		ଯଦି (!is_in_reset_ବୃକ୍ଷ && !intent_to_add) {
			remove_file_from_index(the_repository->index, one->path);
			ଜାରି;
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
		ଯଦି ((pos >= 0 && ce_skip_worktree(the_repository->index->cache[pos])) ||
		    (pos < 0 && !path_in_sparse_checkout(one->path, the_repository->index)))
			ce->ce_flags |= CE_SKIP_WORKTREE;

		ଯଦି (!ce)
			die(_("make_cache_entry failed for path '%s'"),
			    one->path);
		ଯଦି (!is_in_reset_ବୃକ୍ଷ) {
			ce->ce_flags |= CE_INTENT_TO_ADD;
			set_object_name_for_intent_to_add_entry(ce);
		}
		add_index_entry(the_repository->index, ce,
				ADD_CACHE_OK_TO_ADD | ADD_CACHE_OK_TO_REPLACE);
	}
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା read_from_ବୃକ୍ଷ(ଅଚଳ ସଂରଚନା pathspec *pathspec,
			  ସଂରଚନା object_id *ବୃକ୍ଷ_oid,
			  ପୂର୍ଣ୍ଣସଂଖ୍ୟା intent_to_add)
{
	ସଂରଚନା diff_options opt;

	memset(&opt, 0, ଆକାର(opt));
	copy_pathspec(&opt.pathspec, pathspec);
	opt.output_format = DIFF_FORMAT_CALLBACK;
	opt.format_callback = update_index_from_diff;
	opt.format_callback_data = &intent_to_add;
	opt.flags.override_submodule_config = 1;
	opt.flags.recursive = 1;
	opt.repo = the_repository;
	opt.change = diff_change;
	opt.add_remove = diff_addremove;

	ଯଦି (pathspec->nr && pathspec_needs_expanded_index(the_repository->index, pathspec))
		ensure_full_index(the_repository->index);

	ଯଦି (do_diff_cache(ବୃକ୍ଷ_oid, &opt))
		ଫେରନ୍ତୁ 1;
	diffcore_std(&opt);
	diff_flush(&opt);

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ set_reflog_message(ସଂରଚନା strbuf *sb, ଅଚଳ ଅକ୍ଷର *action,
			       ଅଚଳ ଅକ୍ଷର *rev)
{
	ଅଚଳ ଅକ୍ଷର *rla = getenv("GIT_REFLOG_ACTION");

	strbuf_reset(sb);
	ଯଦି (rla)
		strbuf_addf(sb, "%s: %s", rla, action);
	ଅନ୍ୟଥା ଯଦି (rev)
		strbuf_addf(sb, "reset: moving to %s", rev);
	ଅନ୍ୟଥା
		strbuf_addf(sb, "reset: %s", action);
}

ସ୍ଥିର ଶୂନ୍ୟ die_if_unmerged_cache(ପୂର୍ଣ୍ଣସଂଖ୍ୟା reset_type)
{
	ଯଦି (is_merge() || unmerged_index(the_repository->index))
		die(_("Cannot do a %s reset in the middle of a merge."),
		    _(reset_type_names[reset_type]));

}

ସ୍ଥିର ଶୂନ୍ୟ parse_args(ସଂରଚନା pathspec *pathspec,
		       ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
		       ପୂର୍ଣ୍ଣସଂଖ୍ୟା patch_mode,
		       ଅଚଳ ଅକ୍ଷର **rev_ret)
{
	ଅଚଳ ଅକ୍ଷର *rev = "HEAD";
	ସଂରଚନା object_id unused;
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

	ଯଦି (argv[0]) {
		ଯଦି (!strcmp(argv[0], "--")) {
			argv++; /* reset to HEAD, possibly with paths */
		} ଅନ୍ୟଥା ଯଦି (argv[1] && !strcmp(argv[1], "--")) {
			rev = argv[0];
			argv += 2;
		}
		/*
		 * Otherwise, argv[0] could be either <rev> or <paths> and
		 * has to be unambiguous. If there is a single argument, it
		 * can not be a tree
		 */
		ଅନ୍ୟଥା ଯଦି ((!argv[1] && !repo_get_oid_committish(the_repository, argv[0], &unused)) ||
			 (argv[1] && !repo_get_oid_treeish(the_repository, argv[0], &unused))) {
			/*
			 * Ok, argv[0] looks like a commit/tree; it should not
			 * be a filename.
			 */
			verify_non_filename(the_repository, prefix, argv[0]);
			rev = *argv++;
		} ଅନ୍ୟଥା {
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

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା reset_refs(ଅଚଳ ଅକ୍ଷର *rev, ଅଚଳ ସଂରଚନା object_id *oid)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା update_ref_status;
	ସଂରଚନା strbuf ସନ୍ଦେଶ = STRBUF_INIT;
	ସଂରଚନା object_id *orig = ଖାଲି, oid_orig,
		*old_orig = ଖାଲି, oid_old_orig;

	ଯଦି (!repo_get_oid(the_repository, "ORIG_HEAD", &oid_old_orig))
		old_orig = &oid_old_orig;
	ଯଦି (!repo_get_oid(the_repository, "HEAD", &oid_orig)) {
		orig = &oid_orig;
		set_reflog_message(&ସନ୍ଦେଶ, "updating ORIG_HEAD", ଖାଲି);
		refs_update_ref(get_main_ref_store(the_repository), ସନ୍ଦେଶ.buf,
				"ORIG_HEAD", orig, old_orig, 0,
				UPDATE_REFS_ସନ୍ଦେଶ_ON_ERR);
	} ଅନ୍ୟଥା ଯଦି (old_orig)
		refs_delete_ref(get_main_ref_store(the_repository), ଖାଲି,
				"ORIG_HEAD", old_orig, 0);
	set_reflog_message(&ସନ୍ଦେଶ, "updating HEAD", rev);
	update_ref_status = refs_update_ref(get_main_ref_store(the_repository),
					    ସନ୍ଦେଶ.buf, "HEAD", oid, orig, 0,
					    UPDATE_REFS_ସନ୍ଦେଶ_ON_ERR);
	strbuf_release(&ସନ୍ଦେଶ);
	ଫେରନ୍ତୁ update_ref_status;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା git_reset_config(ଅଚଳ ଅକ୍ଷର *var, ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ,
			    ଅଚଳ ସଂରଚନା config_context *ctx, ଶୂନ୍ୟ *cb)
{
	ଯଦି (!strcmp(var, "submodule.recurse"))
		ଫେରନ୍ତୁ git_default_submodule_config(var, ମୂଲ୍ୟ, cb);

	ଫେରନ୍ତୁ git_default_config(var, ମୂଲ୍ୟ, ctx, cb);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_reset(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
	      ଅଚଳ ଅକ୍ଷର **argv,
	      ଅଚଳ ଅକ୍ଷର *prefix,
	      ସଂରଚନା repository *repo UNUSED)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା reset_type = NONE, update_ref_status = 0, quiet = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା no_refresh = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା patch_mode = 0, pathspec_file_nul = 0, unborn;
	ଅଚଳ ଅକ୍ଷର *rev;
	ଅକ୍ଷର *pathspec_from_file = ଖାଲି;
	ସଂରଚନା object_id oid;
	ସଂରଚନା pathspec pathspec;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା intent_to_add = 0;
	ସଂରଚନା interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	ଅଚଳ ସଂରଚନା option options[] = {
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
		OPT_CALLBACK_F(0, "recurse-submodules", ଖାଲି,
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

	repo_config(the_repository, git_reset_config, ଖାଲି);

	argc = parse_options(argc, argv, prefix, options, git_reset_usage,
						PARSE_OPT_KEEP_DASHDASH);
	parse_args(&pathspec, argv, prefix, patch_mode, &rev);

	ଯଦି (pathspec_from_file) {
		ଯଦି (patch_mode)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--patch");

		ଯଦି (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_FULL,
				    prefix, pathspec_from_file, pathspec_file_nul);
	} ଅନ୍ୟଥା ଯଦି (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	unborn = !strcmp(rev, "HEAD") && repo_get_oid(the_repository, "HEAD",
						      &oid);
	ଯଦି (unborn) {
		/* reset on unborn branch: treat as reset to empty tree */
		oidcpy(&oid, the_hash_algo->empty_ବୃକ୍ଷ);
	} ଅନ୍ୟଥା ଯଦି (!pathspec.nr && !patch_mode) {
		ସଂରଚନା commit *commit;
		ଯଦି (repo_get_oid_committish(the_repository, rev, &oid))
			die(_("Failed to resolve '%s' as a valid revision."), rev);
		commit = lookup_commit_reference(the_repository, &oid);
		ଯଦି (!commit)
			die(_("Could not parse object '%s'."), rev);
		oidcpy(&oid, &commit->object.oid);
	} ଅନ୍ୟଥା {
		ସଂରଚନା ବୃକ୍ଷ *ବୃକ୍ଷ;
		ଯଦି (repo_get_oid_treeish(the_repository, rev, &oid))
			die(_("Failed to resolve '%s' as a valid tree."), rev);
		ବୃକ୍ଷ = repo_parse_ବୃକ୍ଷ_indirect(the_repository, &oid);
		ଯଦି (!ବୃକ୍ଷ)
			die(_("Could not parse object '%s'."), rev);
		oidcpy(&oid, &ବୃକ୍ଷ->object.oid);
	}

	ଯଦି (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	ଯଦି (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	ଯଦି (patch_mode) {
		ଯଦି (reset_type != NONE)
			die(_("options '%s' and '%s' cannot be used together"), "--patch", "--{hard,mixed,soft}");
		trace2_cmd_mode("patch-interactive");
		update_ref_status = !!run_add_p(the_repository, ADD_P_RESET,
						&interactive_opts, rev, &pathspec, 0);
		ଯାଅ cleanup;
	} ଅନ୍ୟଥା {
		ଯଦି (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--patch");
		ଯଦି (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--patch");
		ଯଦି (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--patch");
	}

	/* git reset tree [--] paths... can be used to
	 * load chosen paths from the tree into the index without
	 * affecting the working tree nor HEAD. */
	ଯଦି (pathspec.nr) {
		ଯଦି (reset_type == MIXED)
			warning(_("--mixed with paths is deprecated; use 'git reset -- <paths>' instead."));
		ଅନ୍ୟଥା ଯଦି (reset_type != NONE)
			die(_("Cannot do %s reset with paths."),
					_(reset_type_names[reset_type]));
	}
	ଯଦି (reset_type == NONE)
		reset_type = MIXED; /* by default */

	ଯଦି (pathspec.nr)
		trace2_cmd_mode("path");
	ଅନ୍ୟଥା
		trace2_cmd_mode(reset_type_names[reset_type]);

	ଯଦି (reset_type != SOFT && (reset_type != MIXED || repo_get_work_ବୃକ୍ଷ(the_repository)))
		setup_work_ବୃକ୍ଷ(the_repository);

	ଯଦି (reset_type == MIXED && is_bare_repository(the_repository))
		die(_("%s reset is not allowed in a bare repository"),
		    _(reset_type_names[reset_type]));

	ଯଦି (intent_to_add && reset_type != MIXED)
		die(_("the option '%s' requires '%s'"), "-N", "--mixed");

	ଯଦି (repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));

	/* Soft reset does not touch the index file nor the working tree
	 * at all, but requires them in a good order.  Other resets reset
	 * the index file to the tree object we are switching to. */
	ଯଦି (reset_type == SOFT || reset_type == KEEP)
		die_if_unmerged_cache(reset_type);

	ଯଦି (reset_type != SOFT) {
		ସଂରଚନା lock_file lock = LOCK_INIT;
		repo_hold_locked_index(the_repository, &lock,
				       LOCK_DIE_ON_ERROR);
		ଯଦି (reset_type == MIXED) {
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags = quiet ? REFRESH_QUIET : REFRESH_IN_PORCELAIN;
			ଯଦି (read_from_ବୃକ୍ଷ(&pathspec, &oid, intent_to_add)) {
				update_ref_status = 1;
				ଯାଅ cleanup;
			}
			the_repository->index->updated_skipworktree = 1;
			ଯଦି (!no_refresh && repo_get_work_ବୃକ୍ଷ(the_repository)) {
				uint64_t t_begin, t_delta_in_ms;

				t_begin = getnanotime();
				refresh_index(the_repository->index, flags, ଖାଲି, ଖାଲି,
					      _("Unstaged changes after reset:"));
				t_delta_in_ms = (getnanotime() - t_begin) / 1000000;
				ଯଦି (!quiet && advice_enabled(ADVICE_RESET_NO_REFRESH_WARNING) && t_delta_in_ms > REFRESH_INDEX_DELAY_WARNING_IN_MS) {
					advise(_("It took %.2f seconds to refresh the index after reset.  You can use\n"
						 "'--no-refresh' to avoid this."), t_delta_in_ms / 1000.0);
				}
			}
		} ଅନ୍ୟଥା {
			ସଂରଚନା object_id dummy;
			ଅକ୍ଷର *ref = ଖାଲି;
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା err;

			repo_dwim_ref(the_repository, rev, strlen(rev),
				      &dummy, &ref, 0);
			ଯଦି (ref && !starts_with(ref, "refs/"))
				FREE_AND_NULL(ref);

			err = reset_index(ref, &oid, reset_type, quiet);
			ଯଦି (reset_type == KEEP && !err)
				err = reset_index(ref, &oid, MIXED, quiet);
			ଯଦି (err)
				die(_("Could not reset index file to revision '%s'."), rev);
			ମୁକ୍ତ(ref);
		}

		ଯଦି (write_locked_index(the_repository->index, &lock, COMMIT_LOCK))
			die(_("Could not write new index file."));
	}

	ଯଦି (!pathspec.nr && !unborn) {
		/* Any resets without paths update HEAD to the head being
		 * switched to, saving the previous head in ORIG_HEAD before. */
		update_ref_status = reset_refs(rev, &oid);

		ଯଦି (reset_type == HARD && !update_ref_status && !quiet)
			print_new_head_ଧାଡ଼ି(lookup_commit_reference(the_repository, &oid));
	}
	ଯଦି (!pathspec.nr)
		remove_branch_state(the_repository, 0);

	discard_index(the_repository->index);

cleanup:
	clear_pathspec(&pathspec);
	ମୁକ୍ତ(pathspec_from_file);
	ଫେରନ୍ତୁ update_ref_status;
}
