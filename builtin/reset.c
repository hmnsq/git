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

#অন্তর্ভাৱ "builtin.h"
#অন্তর্ভাৱ "advice.h"
#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ "environment.h"
#অন্তর্ভাৱ "gettext.h"
#অন্তর্ভাৱ "hash.h"
#অন্তর্ভাৱ "hex.h"
#অন্তর্ভাৱ "lockfile.h"
#অন্তর্ভাৱ "object.h"
#অন্তর্ভাৱ "pretty.h"
#অন্তর্ভাৱ "refs.h"
#অন্তর্ভাৱ "diff.h"
#অন্তর্ভাৱ "diffcore.h"
#অন্তর্ভাৱ "tree.h"
#অন্তর্ভাৱ "branch.h"
#অন্তর্ভাৱ "object-name.h"
#অন্তর্ভাৱ "parse-options.h"
#অন্তর্ভাৱ "path.h"
#অন্তর্ভাৱ "repository.h"
#অন্তর্ভাৱ "unpack-trees.h"
#অন্তর্ভাৱ "cache-tree.h"
#অন্তর্ভাৱ "setup.h"
#অন্তর্ভাৱ "sparse-index.h"
#অন্তর্ভাৱ "submodule.h"
#অন্তর্ভাৱ "trace.h"
#অন্তর্ভাৱ "trace2.h"
#অন্তর্ভাৱ "dir.h"
#অন্তর্ভাৱ "add-interactive.h"

#define REFRESH_INDEX_DELAY_WARNING_IN_MS (2 * 1000)

স্থির ধ্রুৱ ৱর্ণ * ধ্রুৱ git_reset_usage[] = {
	N_("git reset [--mixed | --soft | --hard | --merge | --keep] [-q] [<commit>]"),
	N_("git reset [-q] [<tree-ish>] [--] <pathspec>..."),
	N_("git reset [-q] [--pathspec-from-file [--pathspec-file-nul]] [<tree-ish>]"),
	N_("git reset --patch [<tree-ish>] [--] [<pathspec>...]"),
	রিক্ত
};

গণনা reset_type { MIXED, SOFT, HARD, MERGE, KEEP, NONE };
স্থির ধ্রুৱ ৱর্ণ *reset_type_names[] = {
	N_("mixed"), N_("soft"), N_("hard"), N_("merge"), N_("keep"), রিক্ত
};

স্থির অন্তঃস্থ পূর্ণাঙ্ক is_merge(শূন্য)
{
	ঘূৰাই !access(git_path_merge_head(the_repository), F_OK);
}

স্থির পূর্ণাঙ্ক reset_index(ধ্রুৱ ৱর্ণ *ref, ধ্রুৱ সংরচনা object_id *oid, পূর্ণাঙ্ক reset_type, পূর্ণাঙ্ক quiet)
{
	পূর্ণাঙ্ক i, nr = 0;
	সংরচনা বৃক্ষ_desc desc[2];
	সংরচনা বৃক্ষ *বৃক্ষ;
	সংরচনা unpack_trees_options opts;
	পূর্ণাঙ্ক ret = -1;

	memset(&opts, 0, পরিমাণ(opts));
	opts.head_idx = 1;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;
	opts.fn = oneway_merge;
	opts.merge = 1;
	init_checkout_metadata(&opts.meta, ref, oid, রিক্ত);
	যদি (!quiet)
		opts.verbose_update = 1;
	ৱিকল্প (reset_type) {
	পক্ষ KEEP:
	পক্ষ MERGE:
		opts.update = 1;
		opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
		ভঙ্গ;
	পক্ষ HARD:
		opts.update = 1;
		opts.reset = UNPACK_RESET_OVERWRITE_UNTRACKED;
		opts.skip_cache_বৃক্ষ_update = 1;
		ভঙ্গ;
	পক্ষ MIXED:
		opts.reset = UNPACK_RESET_PROTECT_UNTRACKED;
		opts.skip_cache_বৃক্ষ_update = 1;
		/* but opts.update=0, so working tree not updated */
		ভঙ্গ;
	স্ৱভাৱ:
		BUG("invalid reset_type passed to reset_index");
	}

	repo_read_index_unmerged(the_repository);

	যদি (reset_type == KEEP) {
		সংরচনা object_id head_oid;
		যদি (repo_get_oid(the_repository, "HEAD", &head_oid))
			ঘূৰাই error(_("You do not have a valid HEAD."));
		যদি (!fill_বৃক্ষ_descriptor(the_repository, desc + nr, &head_oid))
			ঘূৰাই error(_("Failed to find tree of HEAD."));
		nr++;
		opts.fn = twoway_merge;
	}

	যদি (!fill_বৃক্ষ_descriptor(the_repository, desc + nr, oid)) {
		error(_("Failed to find tree of %s."), oid_to_hex(oid));
		গচ্ছ out;
	}
	nr++;

	যদি (unpack_trees(nr, desc, &opts))
		গচ্ছ out;

	যদি (reset_type == MIXED || reset_type == HARD) {
		বৃক্ষ = repo_parse_বৃক্ষ_indirect(the_repository, oid);
		যদি (!বৃক্ষ) {
			error(_("unable to read tree (%s)"), oid_to_hex(oid));
			গচ্ছ out;
		}
		মৌলিক_cache_বৃক্ষ(the_repository, the_repository->index, বৃক্ষ);
	}

	ret = 0;

out:
	বাবে (i = 0; i < nr; i++)
		মুক্ত((শূন্য *)desc[i].buffer);
	ঘূৰাই ret;
}

স্থির শূন্য print_new_head_সাৰি(সংরচনা commit *commit)
{
	সংরচনা strbuf buf = STRBUF_INIT;

	printf(_("HEAD is now at %s"),
		repo_find_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV));

	pp_commit_easy(CMIT_FMT_ONELINE, commit, &buf);
	যদি (buf.len > 0)
		printf(" %s", buf.buf);
	putchar('\n');
	strbuf_release(&buf);
}

স্থির শূন্য update_index_from_diff(সংরচনা diff_queue_struct *q,
				   সংরচনা diff_options *opt UNUSED,
				   শূন্য *data)
{
	পূর্ণাঙ্ক i;
	পূর্ণাঙ্ক intent_to_add = *(পূর্ণাঙ্ক *)data;

	বাবে (i = 0; i < q->nr; i++) {
		পূর্ণাঙ্ক pos;
		সংরচনা diff_filespec *one = q->queue[i]->one;
		পূর্ণাঙ্ক is_in_reset_বৃক্ষ = one->mode && !is_null_oid(&one->oid);
		সংরচনা cache_entry *ce;

		যদি (!is_in_reset_বৃক্ষ && !intent_to_add) {
			remove_file_from_index(the_repository->index, one->path);
			অনুৱর্ত;
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
		যদি ((pos >= 0 && ce_skip_worktree(the_repository->index->cache[pos])) ||
		    (pos < 0 && !path_in_sparse_checkout(one->path, the_repository->index)))
			ce->ce_flags |= CE_SKIP_WORKTREE;

		যদি (!ce)
			die(_("make_cache_entry failed for path '%s'"),
			    one->path);
		যদি (!is_in_reset_বৃক্ষ) {
			ce->ce_flags |= CE_INTENT_TO_ADD;
			set_object_name_for_intent_to_add_entry(ce);
		}
		add_index_entry(the_repository->index, ce,
				ADD_CACHE_OK_TO_ADD | ADD_CACHE_OK_TO_REPLACE);
	}
}

স্থির পূর্ণাঙ্ক read_from_বৃক্ষ(ধ্রুৱ সংরচনা pathspec *pathspec,
			  সংরচনা object_id *বৃক্ষ_oid,
			  পূর্ণাঙ্ক intent_to_add)
{
	সংরচনা diff_options opt;

	memset(&opt, 0, পরিমাণ(opt));
	copy_pathspec(&opt.pathspec, pathspec);
	opt.output_format = DIFF_FORMAT_CALLBACK;
	opt.format_callback = update_index_from_diff;
	opt.format_callback_data = &intent_to_add;
	opt.flags.override_submodule_config = 1;
	opt.flags.recursive = 1;
	opt.repo = the_repository;
	opt.change = diff_change;
	opt.add_remove = diff_addremove;

	যদি (pathspec->nr && pathspec_needs_expanded_index(the_repository->index, pathspec))
		ensure_full_index(the_repository->index);

	যদি (do_diff_cache(বৃক্ষ_oid, &opt))
		ঘূৰাই 1;
	diffcore_std(&opt);
	diff_flush(&opt);

	ঘূৰাই 0;
}

স্থির শূন্য set_reflog_message(সংরচনা strbuf *sb, ধ্রুৱ ৱর্ণ *action,
			       ধ্রুৱ ৱর্ণ *rev)
{
	ধ্রুৱ ৱর্ণ *rla = getenv("GIT_REFLOG_ACTION");

	strbuf_reset(sb);
	যদি (rla)
		strbuf_addf(sb, "%s: %s", rla, action);
	অন্যথা যদি (rev)
		strbuf_addf(sb, "reset: moving to %s", rev);
	অন্যথা
		strbuf_addf(sb, "reset: %s", action);
}

স্থির শূন্য die_if_unmerged_cache(পূর্ণাঙ্ক reset_type)
{
	যদি (is_merge() || unmerged_index(the_repository->index))
		die(_("Cannot do a %s reset in the middle of a merge."),
		    _(reset_type_names[reset_type]));

}

স্থির শূন্য parse_args(সংরচনা pathspec *pathspec,
		       ধ্রুৱ ৱর্ণ **argv, ধ্রুৱ ৱর্ণ *prefix,
		       পূর্ণাঙ্ক patch_mode,
		       ধ্রুৱ ৱর্ণ **rev_ret)
{
	ধ্রুৱ ৱর্ণ *rev = "HEAD";
	সংরচনা object_id unused;
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

	যদি (argv[0]) {
		যদি (!strcmp(argv[0], "--")) {
			argv++; /* reset to HEAD, possibly with paths */
		} অন্যথা যদি (argv[1] && !strcmp(argv[1], "--")) {
			rev = argv[0];
			argv += 2;
		}
		/*
		 * Otherwise, argv[0] could be either <rev> or <paths> and
		 * has to be unambiguous. If there is a single argument, it
		 * can not be a tree
		 */
		অন্যথা যদি ((!argv[1] && !repo_get_oid_committish(the_repository, argv[0], &unused)) ||
			 (argv[1] && !repo_get_oid_treeish(the_repository, argv[0], &unused))) {
			/*
			 * Ok, argv[0] looks like a commit/tree; it should not
			 * be a filename.
			 */
			verify_non_filename(the_repository, prefix, argv[0]);
			rev = *argv++;
		} অন্যথা {
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

স্থির পূর্ণাঙ্ক reset_refs(ধ্রুৱ ৱর্ণ *rev, ধ্রুৱ সংরচনা object_id *oid)
{
	পূর্ণাঙ্ক update_ref_status;
	সংরচনা strbuf বাৰ্তা = STRBUF_INIT;
	সংরচনা object_id *orig = রিক্ত, oid_orig,
		*old_orig = রিক্ত, oid_old_orig;

	যদি (!repo_get_oid(the_repository, "ORIG_HEAD", &oid_old_orig))
		old_orig = &oid_old_orig;
	যদি (!repo_get_oid(the_repository, "HEAD", &oid_orig)) {
		orig = &oid_orig;
		set_reflog_message(&বাৰ্তা, "updating ORIG_HEAD", রিক্ত);
		refs_update_ref(get_main_ref_store(the_repository), বাৰ্তা.buf,
				"ORIG_HEAD", orig, old_orig, 0,
				UPDATE_REFS_বাৰ্তা_ON_ERR);
	} অন্যথা যদি (old_orig)
		refs_delete_ref(get_main_ref_store(the_repository), রিক্ত,
				"ORIG_HEAD", old_orig, 0);
	set_reflog_message(&বাৰ্তা, "updating HEAD", rev);
	update_ref_status = refs_update_ref(get_main_ref_store(the_repository),
					    বাৰ্তা.buf, "HEAD", oid, orig, 0,
					    UPDATE_REFS_বাৰ্তা_ON_ERR);
	strbuf_release(&বাৰ্তা);
	ঘূৰাই update_ref_status;
}

স্থির পূর্ণাঙ্ক git_reset_config(ধ্রুৱ ৱর্ণ *var, ধ্রুৱ ৱর্ণ *মান,
			    ধ্রুৱ সংরচনা config_context *ctx, শূন্য *cb)
{
	যদি (!strcmp(var, "submodule.recurse"))
		ঘূৰাই git_default_submodule_config(var, মান, cb);

	ঘূৰাই git_default_config(var, মান, ctx, cb);
}

পূর্ণাঙ্ক cmd_reset(পূর্ণাঙ্ক argc,
	      ধ্রুৱ ৱর্ণ **argv,
	      ধ্রুৱ ৱর্ণ *prefix,
	      সংরচনা repository *repo UNUSED)
{
	পূর্ণাঙ্ক reset_type = NONE, update_ref_status = 0, quiet = 0;
	পূর্ণাঙ্ক no_refresh = 0;
	পূর্ণাঙ্ক patch_mode = 0, pathspec_file_nul = 0, unborn;
	ধ্রুৱ ৱর্ণ *rev;
	ৱর্ণ *pathspec_from_file = রিক্ত;
	সংরচনা object_id oid;
	সংরচনা pathspec pathspec;
	পূর্ণাঙ্ক intent_to_add = 0;
	সংরচনা interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	ধ্রুৱ সংরচনা option options[] = {
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
		OPT_CALLBACK_F(0, "recurse-submodules", রিক্ত,
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

	repo_config(the_repository, git_reset_config, রিক্ত);

	argc = parse_options(argc, argv, prefix, options, git_reset_usage,
						PARSE_OPT_KEEP_DASHDASH);
	parse_args(&pathspec, argv, prefix, patch_mode, &rev);

	যদি (pathspec_from_file) {
		যদি (patch_mode)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--patch");

		যদি (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_FULL,
				    prefix, pathspec_from_file, pathspec_file_nul);
	} অন্যথা যদি (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	unborn = !strcmp(rev, "HEAD") && repo_get_oid(the_repository, "HEAD",
						      &oid);
	যদি (unborn) {
		/* reset on unborn branch: treat as reset to empty tree */
		oidcpy(&oid, the_hash_algo->empty_বৃক্ষ);
	} অন্যথা যদি (!pathspec.nr && !patch_mode) {
		সংরচনা commit *commit;
		যদি (repo_get_oid_committish(the_repository, rev, &oid))
			die(_("Failed to resolve '%s' as a valid revision."), rev);
		commit = lookup_commit_reference(the_repository, &oid);
		যদি (!commit)
			die(_("Could not parse object '%s'."), rev);
		oidcpy(&oid, &commit->object.oid);
	} অন্যথা {
		সংরচনা বৃক্ষ *বৃক্ষ;
		যদি (repo_get_oid_treeish(the_repository, rev, &oid))
			die(_("Failed to resolve '%s' as a valid tree."), rev);
		বৃক্ষ = repo_parse_বৃক্ষ_indirect(the_repository, &oid);
		যদি (!বৃক্ষ)
			die(_("Could not parse object '%s'."), rev);
		oidcpy(&oid, &বৃক্ষ->object.oid);
	}

	যদি (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	যদি (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	যদি (patch_mode) {
		যদি (reset_type != NONE)
			die(_("options '%s' and '%s' cannot be used together"), "--patch", "--{hard,mixed,soft}");
		trace2_cmd_mode("patch-interactive");
		update_ref_status = !!run_add_p(the_repository, ADD_P_RESET,
						&interactive_opts, rev, &pathspec, 0);
		গচ্ছ cleanup;
	} অন্যথা {
		যদি (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--patch");
		যদি (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--patch");
		যদি (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--patch");
	}

	/* git reset tree [--] paths... can be used to
	 * load chosen paths from the tree into the index without
	 * affecting the working tree nor HEAD. */
	যদি (pathspec.nr) {
		যদি (reset_type == MIXED)
			warning(_("--mixed with paths is deprecated; use 'git reset -- <paths>' instead."));
		অন্যথা যদি (reset_type != NONE)
			die(_("Cannot do %s reset with paths."),
					_(reset_type_names[reset_type]));
	}
	যদি (reset_type == NONE)
		reset_type = MIXED; /* by default */

	যদি (pathspec.nr)
		trace2_cmd_mode("path");
	অন্যথা
		trace2_cmd_mode(reset_type_names[reset_type]);

	যদি (reset_type != SOFT && (reset_type != MIXED || repo_get_work_বৃক্ষ(the_repository)))
		setup_work_বৃক্ষ(the_repository);

	যদি (reset_type == MIXED && is_bare_repository(the_repository))
		die(_("%s reset is not allowed in a bare repository"),
		    _(reset_type_names[reset_type]));

	যদি (intent_to_add && reset_type != MIXED)
		die(_("the option '%s' requires '%s'"), "-N", "--mixed");

	যদি (repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));

	/* Soft reset does not touch the index file nor the working tree
	 * at all, but requires them in a good order.  Other resets reset
	 * the index file to the tree object we are switching to. */
	যদি (reset_type == SOFT || reset_type == KEEP)
		die_if_unmerged_cache(reset_type);

	যদি (reset_type != SOFT) {
		সংরচনা lock_file lock = LOCK_INIT;
		repo_hold_locked_index(the_repository, &lock,
				       LOCK_DIE_ON_ERROR);
		যদি (reset_type == MIXED) {
			পূর্ণাঙ্ক flags = quiet ? REFRESH_QUIET : REFRESH_IN_PORCELAIN;
			যদি (read_from_বৃক্ষ(&pathspec, &oid, intent_to_add)) {
				update_ref_status = 1;
				গচ্ছ cleanup;
			}
			the_repository->index->updated_skipworktree = 1;
			যদি (!no_refresh && repo_get_work_বৃক্ষ(the_repository)) {
				uint64_t t_begin, t_delta_in_ms;

				t_begin = getnanotime();
				refresh_index(the_repository->index, flags, রিক্ত, রিক্ত,
					      _("Unstaged changes after reset:"));
				t_delta_in_ms = (getnanotime() - t_begin) / 1000000;
				যদি (!quiet && advice_enabled(ADVICE_RESET_NO_REFRESH_WARNING) && t_delta_in_ms > REFRESH_INDEX_DELAY_WARNING_IN_MS) {
					advise(_("It took %.2f seconds to refresh the index after reset.  You can use\n"
						 "'--no-refresh' to avoid this."), t_delta_in_ms / 1000.0);
				}
			}
		} অন্যথা {
			সংরচনা object_id dummy;
			ৱর্ণ *ref = রিক্ত;
			পূর্ণাঙ্ক err;

			repo_dwim_ref(the_repository, rev, strlen(rev),
				      &dummy, &ref, 0);
			যদি (ref && !starts_with(ref, "refs/"))
				FREE_AND_NULL(ref);

			err = reset_index(ref, &oid, reset_type, quiet);
			যদি (reset_type == KEEP && !err)
				err = reset_index(ref, &oid, MIXED, quiet);
			যদি (err)
				die(_("Could not reset index file to revision '%s'."), rev);
			মুক্ত(ref);
		}

		যদি (write_locked_index(the_repository->index, &lock, COMMIT_LOCK))
			die(_("Could not write new index file."));
	}

	যদি (!pathspec.nr && !unborn) {
		/* Any resets without paths update HEAD to the head being
		 * switched to, saving the previous head in ORIG_HEAD before. */
		update_ref_status = reset_refs(rev, &oid);

		যদি (reset_type == HARD && !update_ref_status && !quiet)
			print_new_head_সাৰি(lookup_commit_reference(the_repository, &oid));
	}
	যদি (!pathspec.nr)
		remove_branch_state(the_repository, 0);

	discard_index(the_repository->index);

cleanup:
	clear_pathspec(&pathspec);
	মুক্ত(pathspec_from_file);
	ঘূৰাই update_ref_status;
}
