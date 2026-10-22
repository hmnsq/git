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

#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "advice.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "hash.h"
#அந்தர்பாவ "hex.h"
#அந்தர்பாவ "lockfile.h"
#அந்தர்பாவ "object.h"
#அந்தர்பாவ "pretty.h"
#அந்தர்பாவ "refs.h"
#அந்தர்பாவ "diff.h"
#அந்தர்பாவ "diffcore.h"
#அந்தர்பாவ "tree.h"
#அந்தர்பாவ "branch.h"
#அந்தர்பாவ "object-name.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "path.h"
#அந்தர்பாவ "repository.h"
#அந்தர்பாவ "unpack-trees.h"
#அந்தர்பாவ "cache-tree.h"
#அந்தர்பாவ "setup.h"
#அந்தர்பாவ "sparse-index.h"
#அந்தர்பாவ "submodule.h"
#அந்தர்பாவ "trace.h"
#அந்தர்பாவ "trace2.h"
#அந்தர்பாவ "dir.h"
#அந்தர்பாவ "add-interactive.h"

#define REFRESH_INDEX_DELAY_WARNING_IN_MS (2 * 1000)

நிலையான மாறிலி எழுத்து * மாறிலி git_reset_usage[] = {
	N_("git reset [--mixed | --soft | --hard | --merge | --keep] [-q] [<commit>]"),
	N_("git reset [-q] [<tree-ish>] [--] <pathspec>..."),
	N_("git reset [-q] [--pathspec-from-file [--pathspec-file-nul]] [<tree-ish>]"),
	N_("git reset --patch [<tree-ish>] [--] [<pathspec>...]"),
	வெற்று
};

எண்ணிக்கை reset_type { MIXED, SOFT, HARD, MERGE, KEEP, NONE };
நிலையான மாறிலி எழுத்து *reset_type_names[] = {
	N_("mixed"), N_("soft"), N_("hard"), N_("merge"), N_("keep"), வெற்று
};

நிலையான உள்வரி முழு is_merge(வெறுமை)
{
	திரும்பு !access(git_path_merge_head(the_repository), F_OK);
}

நிலையான முழு reset_index(மாறிலி எழுத்து *ref, மாறிலி கட்டமைப்பு object_id *oid, முழு reset_type, முழு quiet)
{
	முழு i, nr = 0;
	கட்டமைப்பு மரம்_desc desc[2];
	கட்டமைப்பு மரம் *மரம்;
	கட்டமைப்பு unpack_trees_options opts;
	முழு ret = -1;

	memset(&opts, 0, அளவு(opts));
	opts.head_idx = 1;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;
	opts.fn = oneway_merge;
	opts.merge = 1;
	init_checkout_metadata(&opts.meta, ref, oid, வெற்று);
	எனில் (!quiet)
		opts.verbose_update = 1;
	தேர்வு (reset_type) {
	நிலை KEEP:
	நிலை MERGE:
		opts.update = 1;
		opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
		நிறுத்து;
	நிலை HARD:
		opts.update = 1;
		opts.reset = UNPACK_RESET_OVERWRITE_UNTRACKED;
		opts.skip_cache_மரம்_update = 1;
		நிறுத்து;
	நிலை MIXED:
		opts.reset = UNPACK_RESET_PROTECT_UNTRACKED;
		opts.skip_cache_மரம்_update = 1;
		/* but opts.update=0, so working tree not updated */
		நிறுத்து;
	இயல்பு:
		BUG("invalid reset_type passed to reset_index");
	}

	repo_read_index_unmerged(the_repository);

	எனில் (reset_type == KEEP) {
		கட்டமைப்பு object_id head_oid;
		எனில் (repo_get_oid(the_repository, "HEAD", &head_oid))
			திரும்பு error(_("You do not have a valid HEAD."));
		எனில் (!fill_மரம்_descriptor(the_repository, desc + nr, &head_oid))
			திரும்பு error(_("Failed to find tree of HEAD."));
		nr++;
		opts.fn = twoway_merge;
	}

	எனில் (!fill_மரம்_descriptor(the_repository, desc + nr, oid)) {
		error(_("Failed to find tree of %s."), oid_to_hex(oid));
		செல் out;
	}
	nr++;

	எனில் (unpack_trees(nr, desc, &opts))
		செல் out;

	எனில் (reset_type == MIXED || reset_type == HARD) {
		மரம் = repo_parse_மரம்_indirect(the_repository, oid);
		எனில் (!மரம்) {
			error(_("unable to read tree (%s)"), oid_to_hex(oid));
			செல் out;
		}
		பகாஎண்_cache_மரம்(the_repository, the_repository->index, மரம்);
	}

	ret = 0;

out:
	க்கு (i = 0; i < nr; i++)
		விடுவி((வெறுமை *)desc[i].buffer);
	திரும்பு ret;
}

நிலையான வெறுமை print_new_head_வரி(கட்டமைப்பு commit *commit)
{
	கட்டமைப்பு strbuf buf = STRBUF_INIT;

	printf(_("HEAD is now at %s"),
		repo_find_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV));

	pp_commit_easy(CMIT_FMT_ONELINE, commit, &buf);
	எனில் (buf.len > 0)
		printf(" %s", buf.buf);
	putchar('\n');
	strbuf_release(&buf);
}

நிலையான வெறுமை update_index_from_diff(கட்டமைப்பு diff_queue_struct *q,
				   கட்டமைப்பு diff_options *opt UNUSED,
				   வெறுமை *data)
{
	முழு i;
	முழு intent_to_add = *(முழு *)data;

	க்கு (i = 0; i < q->nr; i++) {
		முழு pos;
		கட்டமைப்பு diff_filespec *one = q->queue[i]->one;
		முழு is_in_reset_மரம் = one->mode && !is_null_oid(&one->oid);
		கட்டமைப்பு cache_entry *ce;

		எனில் (!is_in_reset_மரம் && !intent_to_add) {
			remove_file_from_index(the_repository->index, one->path);
			தொடர்;
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
		எனில் ((pos >= 0 && ce_skip_worktree(the_repository->index->cache[pos])) ||
		    (pos < 0 && !path_in_sparse_checkout(one->path, the_repository->index)))
			ce->ce_flags |= CE_SKIP_WORKTREE;

		எனில் (!ce)
			die(_("make_cache_entry failed for path '%s'"),
			    one->path);
		எனில் (!is_in_reset_மரம்) {
			ce->ce_flags |= CE_INTENT_TO_ADD;
			set_object_name_for_intent_to_add_entry(ce);
		}
		add_index_entry(the_repository->index, ce,
				ADD_CACHE_OK_TO_ADD | ADD_CACHE_OK_TO_REPLACE);
	}
}

நிலையான முழு read_from_மரம்(மாறிலி கட்டமைப்பு pathspec *pathspec,
			  கட்டமைப்பு object_id *மரம்_oid,
			  முழு intent_to_add)
{
	கட்டமைப்பு diff_options opt;

	memset(&opt, 0, அளவு(opt));
	copy_pathspec(&opt.pathspec, pathspec);
	opt.output_format = DIFF_FORMAT_CALLBACK;
	opt.format_callback = update_index_from_diff;
	opt.format_callback_data = &intent_to_add;
	opt.flags.override_submodule_config = 1;
	opt.flags.recursive = 1;
	opt.repo = the_repository;
	opt.change = diff_change;
	opt.add_remove = diff_addremove;

	எனில் (pathspec->nr && pathspec_needs_expanded_index(the_repository->index, pathspec))
		ensure_full_index(the_repository->index);

	எனில் (do_diff_cache(மரம்_oid, &opt))
		திரும்பு 1;
	diffcore_std(&opt);
	diff_flush(&opt);

	திரும்பு 0;
}

நிலையான வெறுமை set_reflog_message(கட்டமைப்பு strbuf *sb, மாறிலி எழுத்து *action,
			       மாறிலி எழுத்து *rev)
{
	மாறிலி எழுத்து *rla = getenv("GIT_REFLOG_ACTION");

	strbuf_reset(sb);
	எனில் (rla)
		strbuf_addf(sb, "%s: %s", rla, action);
	இல்லையெனில் எனில் (rev)
		strbuf_addf(sb, "reset: moving to %s", rev);
	இல்லையெனில்
		strbuf_addf(sb, "reset: %s", action);
}

நிலையான வெறுமை die_if_unmerged_cache(முழு reset_type)
{
	எனில் (is_merge() || unmerged_index(the_repository->index))
		die(_("Cannot do a %s reset in the middle of a merge."),
		    _(reset_type_names[reset_type]));

}

நிலையான வெறுமை parse_args(கட்டமைப்பு pathspec *pathspec,
		       மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
		       முழு patch_mode,
		       மாறிலி எழுத்து **rev_ret)
{
	மாறிலி எழுத்து *rev = "HEAD";
	கட்டமைப்பு object_id unused;
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

	எனில் (argv[0]) {
		எனில் (!strcmp(argv[0], "--")) {
			argv++; /* reset to HEAD, possibly with paths */
		} இல்லையெனில் எனில் (argv[1] && !strcmp(argv[1], "--")) {
			rev = argv[0];
			argv += 2;
		}
		/*
		 * Otherwise, argv[0] could be either <rev> or <paths> and
		 * has to be unambiguous. If there is a single argument, it
		 * can not be a tree
		 */
		இல்லையெனில் எனில் ((!argv[1] && !repo_get_oid_committish(the_repository, argv[0], &unused)) ||
			 (argv[1] && !repo_get_oid_treeish(the_repository, argv[0], &unused))) {
			/*
			 * Ok, argv[0] looks like a commit/tree; it should not
			 * be a filename.
			 */
			verify_non_filename(the_repository, prefix, argv[0]);
			rev = *argv++;
		} இல்லையெனில் {
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

நிலையான முழு reset_refs(மாறிலி எழுத்து *rev, மாறிலி கட்டமைப்பு object_id *oid)
{
	முழு update_ref_status;
	கட்டமைப்பு strbuf செய்தி = STRBUF_INIT;
	கட்டமைப்பு object_id *orig = வெற்று, oid_orig,
		*old_orig = வெற்று, oid_old_orig;

	எனில் (!repo_get_oid(the_repository, "ORIG_HEAD", &oid_old_orig))
		old_orig = &oid_old_orig;
	எனில் (!repo_get_oid(the_repository, "HEAD", &oid_orig)) {
		orig = &oid_orig;
		set_reflog_message(&செய்தி, "updating ORIG_HEAD", வெற்று);
		refs_update_ref(get_main_ref_store(the_repository), செய்தி.buf,
				"ORIG_HEAD", orig, old_orig, 0,
				UPDATE_REFS_செய்தி_ON_ERR);
	} இல்லையெனில் எனில் (old_orig)
		refs_delete_ref(get_main_ref_store(the_repository), வெற்று,
				"ORIG_HEAD", old_orig, 0);
	set_reflog_message(&செய்தி, "updating HEAD", rev);
	update_ref_status = refs_update_ref(get_main_ref_store(the_repository),
					    செய்தி.buf, "HEAD", oid, orig, 0,
					    UPDATE_REFS_செய்தி_ON_ERR);
	strbuf_release(&செய்தி);
	திரும்பு update_ref_status;
}

நிலையான முழு git_reset_config(மாறிலி எழுத்து *var, மாறிலி எழுத்து *மதிப்பு,
			    மாறிலி கட்டமைப்பு config_context *ctx, வெறுமை *cb)
{
	எனில் (!strcmp(var, "submodule.recurse"))
		திரும்பு git_default_submodule_config(var, மதிப்பு, cb);

	திரும்பு git_default_config(var, மதிப்பு, ctx, cb);
}

முழு cmd_reset(முழு argc,
	      மாறிலி எழுத்து **argv,
	      மாறிலி எழுத்து *prefix,
	      கட்டமைப்பு repository *repo UNUSED)
{
	முழு reset_type = NONE, update_ref_status = 0, quiet = 0;
	முழு no_refresh = 0;
	முழு patch_mode = 0, pathspec_file_nul = 0, unborn;
	மாறிலி எழுத்து *rev;
	எழுத்து *pathspec_from_file = வெற்று;
	கட்டமைப்பு object_id oid;
	கட்டமைப்பு pathspec pathspec;
	முழு intent_to_add = 0;
	கட்டமைப்பு interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	மாறிலி கட்டமைப்பு option options[] = {
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
		OPT_CALLBACK_F(0, "recurse-submodules", வெற்று,
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

	repo_config(the_repository, git_reset_config, வெற்று);

	argc = parse_options(argc, argv, prefix, options, git_reset_usage,
						PARSE_OPT_KEEP_DASHDASH);
	parse_args(&pathspec, argv, prefix, patch_mode, &rev);

	எனில் (pathspec_from_file) {
		எனில் (patch_mode)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--patch");

		எனில் (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_FULL,
				    prefix, pathspec_from_file, pathspec_file_nul);
	} இல்லையெனில் எனில் (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	unborn = !strcmp(rev, "HEAD") && repo_get_oid(the_repository, "HEAD",
						      &oid);
	எனில் (unborn) {
		/* reset on unborn branch: treat as reset to empty tree */
		oidcpy(&oid, the_hash_algo->empty_மரம்);
	} இல்லையெனில் எனில் (!pathspec.nr && !patch_mode) {
		கட்டமைப்பு commit *commit;
		எனில் (repo_get_oid_committish(the_repository, rev, &oid))
			die(_("Failed to resolve '%s' as a valid revision."), rev);
		commit = lookup_commit_reference(the_repository, &oid);
		எனில் (!commit)
			die(_("Could not parse object '%s'."), rev);
		oidcpy(&oid, &commit->object.oid);
	} இல்லையெனில் {
		கட்டமைப்பு மரம் *மரம்;
		எனில் (repo_get_oid_treeish(the_repository, rev, &oid))
			die(_("Failed to resolve '%s' as a valid tree."), rev);
		மரம் = repo_parse_மரம்_indirect(the_repository, &oid);
		எனில் (!மரம்)
			die(_("Could not parse object '%s'."), rev);
		oidcpy(&oid, &மரம்->object.oid);
	}

	எனில் (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	எனில் (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	எனில் (patch_mode) {
		எனில் (reset_type != NONE)
			die(_("options '%s' and '%s' cannot be used together"), "--patch", "--{hard,mixed,soft}");
		trace2_cmd_mode("patch-interactive");
		update_ref_status = !!run_add_p(the_repository, ADD_P_RESET,
						&interactive_opts, rev, &pathspec, 0);
		செல் cleanup;
	} இல்லையெனில் {
		எனில் (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--patch");
		எனில் (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--patch");
		எனில் (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--patch");
	}

	/* git reset tree [--] paths... can be used to
	 * load chosen paths from the tree into the index without
	 * affecting the working tree nor HEAD. */
	எனில் (pathspec.nr) {
		எனில் (reset_type == MIXED)
			warning(_("--mixed with paths is deprecated; use 'git reset -- <paths>' instead."));
		இல்லையெனில் எனில் (reset_type != NONE)
			die(_("Cannot do %s reset with paths."),
					_(reset_type_names[reset_type]));
	}
	எனில் (reset_type == NONE)
		reset_type = MIXED; /* by default */

	எனில் (pathspec.nr)
		trace2_cmd_mode("path");
	இல்லையெனில்
		trace2_cmd_mode(reset_type_names[reset_type]);

	எனில் (reset_type != SOFT && (reset_type != MIXED || repo_get_work_மரம்(the_repository)))
		setup_work_மரம்(the_repository);

	எனில் (reset_type == MIXED && is_bare_repository(the_repository))
		die(_("%s reset is not allowed in a bare repository"),
		    _(reset_type_names[reset_type]));

	எனில் (intent_to_add && reset_type != MIXED)
		die(_("the option '%s' requires '%s'"), "-N", "--mixed");

	எனில் (repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));

	/* Soft reset does not touch the index file nor the working tree
	 * at all, but requires them in a good order.  Other resets reset
	 * the index file to the tree object we are switching to. */
	எனில் (reset_type == SOFT || reset_type == KEEP)
		die_if_unmerged_cache(reset_type);

	எனில் (reset_type != SOFT) {
		கட்டமைப்பு lock_file lock = LOCK_INIT;
		repo_hold_locked_index(the_repository, &lock,
				       LOCK_DIE_ON_ERROR);
		எனில் (reset_type == MIXED) {
			முழு flags = quiet ? REFRESH_QUIET : REFRESH_IN_PORCELAIN;
			எனில் (read_from_மரம்(&pathspec, &oid, intent_to_add)) {
				update_ref_status = 1;
				செல் cleanup;
			}
			the_repository->index->updated_skipworktree = 1;
			எனில் (!no_refresh && repo_get_work_மரம்(the_repository)) {
				uint64_t t_begin, t_delta_in_ms;

				t_begin = getnanotime();
				refresh_index(the_repository->index, flags, வெற்று, வெற்று,
					      _("Unstaged changes after reset:"));
				t_delta_in_ms = (getnanotime() - t_begin) / 1000000;
				எனில் (!quiet && advice_enabled(ADVICE_RESET_NO_REFRESH_WARNING) && t_delta_in_ms > REFRESH_INDEX_DELAY_WARNING_IN_MS) {
					advise(_("It took %.2f seconds to refresh the index after reset.  You can use\n"
						 "'--no-refresh' to avoid this."), t_delta_in_ms / 1000.0);
				}
			}
		} இல்லையெனில் {
			கட்டமைப்பு object_id dummy;
			எழுத்து *ref = வெற்று;
			முழு err;

			repo_dwim_ref(the_repository, rev, strlen(rev),
				      &dummy, &ref, 0);
			எனில் (ref && !starts_with(ref, "refs/"))
				FREE_AND_NULL(ref);

			err = reset_index(ref, &oid, reset_type, quiet);
			எனில் (reset_type == KEEP && !err)
				err = reset_index(ref, &oid, MIXED, quiet);
			எனில் (err)
				die(_("Could not reset index file to revision '%s'."), rev);
			விடுவி(ref);
		}

		எனில் (write_locked_index(the_repository->index, &lock, COMMIT_LOCK))
			die(_("Could not write new index file."));
	}

	எனில் (!pathspec.nr && !unborn) {
		/* Any resets without paths update HEAD to the head being
		 * switched to, saving the previous head in ORIG_HEAD before. */
		update_ref_status = reset_refs(rev, &oid);

		எனில் (reset_type == HARD && !update_ref_status && !quiet)
			print_new_head_வரி(lookup_commit_reference(the_repository, &oid));
	}
	எனில் (!pathspec.nr)
		remove_branch_state(the_repository, 0);

	discard_index(the_repository->index);

cleanup:
	clear_pathspec(&pathspec);
	விடுவி(pathspec_from_file);
	திரும்பு update_ref_status;
}
