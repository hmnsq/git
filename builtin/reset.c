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

#شامل "builtin.h"
#شامل "advice.h"
#شامل "config.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "hash.h"
#شامل "hex.h"
#شامل "lockfile.h"
#شامل "object.h"
#شامل "pretty.h"
#شامل "refs.h"
#شامل "diff.h"
#شامل "diffcore.h"
#شامل "tree.h"
#شامل "branch.h"
#شامل "object-name.h"
#شامل "parse-options.h"
#شامل "path.h"
#شامل "repository.h"
#شامل "unpack-trees.h"
#شامل "cache-tree.h"
#شامل "setup.h"
#شامل "sparse-index.h"
#شامل "submodule.h"
#شامل "trace.h"
#شامل "trace2.h"
#شامل "dir.h"
#شامل "add-interactive.h"

#define REFRESH_INDEX_DELAY_WARNING_IN_MS (2 * 1000)

ایستا ثابت نویسه * ثابت git_reset_usage[] = {
	N_("git reset [--mixed | --soft | --hard | --merge | --keep] [-q] [<commit>]"),
	N_("git reset [-q] [<tree-ish>] [--] <pathspec>..."),
	N_("git reset [-q] [--pathspec-from-file [--pathspec-file-nul]] [<tree-ish>]"),
	N_("git reset --patch [<tree-ish>] [--] [<pathspec>...]"),
	پوچ
};

شمارش reset_type { MIXED, SOFT, HARD, MERGE, KEEP, NONE };
ایستا ثابت نویسه *reset_type_names[] = {
	N_("mixed"), N_("soft"), N_("hard"), N_("merge"), N_("keep"), پوچ
};

ایستا درونخطی صحیح is_merge(تهی)
{
	بازگشت !access(git_path_merge_head(the_repository), F_OK);
}

ایستا صحیح reset_index(ثابت نویسه *ref, ثابت ساختار object_id *oid, صحیح reset_type, صحیح quiet)
{
	صحیح i, nr = 0;
	ساختار درخت_desc desc[2];
	ساختار درخت *درخت;
	ساختار unpack_trees_options opts;
	صحیح ret = -1;

	memset(&opts, 0, اندازه(opts));
	opts.head_idx = 1;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;
	opts.fn = oneway_merge;
	opts.merge = 1;
	init_checkout_metadata(&opts.meta, ref, oid, پوچ);
	اگر (!quiet)
		opts.verbose_update = 1;
	انتخاب (reset_type) {
	حالت KEEP:
	حالت MERGE:
		opts.update = 1;
		opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
		بشکن;
	حالت HARD:
		opts.update = 1;
		opts.reset = UNPACK_RESET_OVERWRITE_UNTRACKED;
		opts.skip_cache_درخت_update = 1;
		بشکن;
	حالت MIXED:
		opts.reset = UNPACK_RESET_PROTECT_UNTRACKED;
		opts.skip_cache_درخت_update = 1;
		/* but opts.update=0, so working tree not updated */
		بشکن;
	پیشفرض:
		BUG("invalid reset_type passed to reset_index");
	}

	repo_read_index_unmerged(the_repository);

	اگر (reset_type == KEEP) {
		ساختار object_id head_oid;
		اگر (repo_get_oid(the_repository, "HEAD", &head_oid))
			بازگشت error(_("You do not have a valid HEAD."));
		اگر (!fill_درخت_descriptor(the_repository, desc + nr, &head_oid))
			بازگشت error(_("Failed to find tree of HEAD."));
		nr++;
		opts.fn = twoway_merge;
	}

	اگر (!fill_درخت_descriptor(the_repository, desc + nr, oid)) {
		error(_("Failed to find tree of %s."), oid_to_hex(oid));
		برو out;
	}
	nr++;

	اگر (unpack_trees(nr, desc, &opts))
		برو out;

	اگر (reset_type == MIXED || reset_type == HARD) {
		درخت = repo_parse_درخت_indirect(the_repository, oid);
		اگر (!درخت) {
			error(_("unable to read tree (%s)"), oid_to_hex(oid));
			برو out;
		}
		اول_cache_درخت(the_repository, the_repository->index, درخت);
	}

	ret = 0;

out:
	برای (i = 0; i < nr; i++)
		آزاد((تهی *)desc[i].buffer);
	بازگشت ret;
}

ایستا تهی print_new_head_سطر(ساختار commit *commit)
{
	ساختار strbuf buf = STRBUF_INIT;

	printf(_("HEAD is now at %s"),
		repo_find_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV));

	pp_commit_easy(CMIT_FMT_ONELINE, commit, &buf);
	اگر (buf.len > 0)
		printf(" %s", buf.buf);
	putchar('\n');
	strbuf_release(&buf);
}

ایستا تهی update_index_from_diff(ساختار diff_queue_struct *q,
				   ساختار diff_options *opt UNUSED,
				   تهی *data)
{
	صحیح i;
	صحیح intent_to_add = *(صحیح *)data;

	برای (i = 0; i < q->nr; i++) {
		صحیح pos;
		ساختار diff_filespec *one = q->queue[i]->one;
		صحیح is_in_reset_درخت = one->mode && !is_null_oid(&one->oid);
		ساختار cache_entry *ce;

		اگر (!is_in_reset_درخت && !intent_to_add) {
			remove_file_from_index(the_repository->index, one->path);
			ادامه;
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
		اگر ((pos >= 0 && ce_skip_worktree(the_repository->index->cache[pos])) ||
		    (pos < 0 && !path_in_sparse_checkout(one->path, the_repository->index)))
			ce->ce_flags |= CE_SKIP_WORKTREE;

		اگر (!ce)
			die(_("make_cache_entry failed for path '%s'"),
			    one->path);
		اگر (!is_in_reset_درخت) {
			ce->ce_flags |= CE_INTENT_TO_ADD;
			set_object_name_for_intent_to_add_entry(ce);
		}
		add_index_entry(the_repository->index, ce,
				ADD_CACHE_OK_TO_ADD | ADD_CACHE_OK_TO_REPLACE);
	}
}

ایستا صحیح read_from_درخت(ثابت ساختار pathspec *pathspec,
			  ساختار object_id *درخت_oid,
			  صحیح intent_to_add)
{
	ساختار diff_options opt;

	memset(&opt, 0, اندازه(opt));
	copy_pathspec(&opt.pathspec, pathspec);
	opt.output_format = DIFF_FORMAT_CALLBACK;
	opt.format_callback = update_index_from_diff;
	opt.format_callback_data = &intent_to_add;
	opt.flags.override_submodule_config = 1;
	opt.flags.recursive = 1;
	opt.repo = the_repository;
	opt.change = diff_change;
	opt.add_remove = diff_addremove;

	اگر (pathspec->nr && pathspec_needs_expanded_index(the_repository->index, pathspec))
		ensure_full_index(the_repository->index);

	اگر (do_diff_cache(درخت_oid, &opt))
		بازگشت 1;
	diffcore_std(&opt);
	diff_flush(&opt);

	بازگشت 0;
}

ایستا تهی set_reflog_message(ساختار strbuf *sb, ثابت نویسه *action,
			       ثابت نویسه *rev)
{
	ثابت نویسه *rla = getenv("GIT_REFLOG_ACTION");

	strbuf_reset(sb);
	اگر (rla)
		strbuf_addf(sb, "%s: %s", rla, action);
	وگرنه اگر (rev)
		strbuf_addf(sb, "reset: moving to %s", rev);
	وگرنه
		strbuf_addf(sb, "reset: %s", action);
}

ایستا تهی die_if_unmerged_cache(صحیح reset_type)
{
	اگر (is_merge() || unmerged_index(the_repository->index))
		die(_("Cannot do a %s reset in the middle of a merge."),
		    _(reset_type_names[reset_type]));

}

ایستا تهی parse_args(ساختار pathspec *pathspec,
		       ثابت نویسه **argv, ثابت نویسه *prefix,
		       صحیح patch_mode,
		       ثابت نویسه **rev_ret)
{
	ثابت نویسه *rev = "HEAD";
	ساختار object_id unused;
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

	اگر (argv[0]) {
		اگر (!strcmp(argv[0], "--")) {
			argv++; /* reset to HEAD, possibly with paths */
		} وگرنه اگر (argv[1] && !strcmp(argv[1], "--")) {
			rev = argv[0];
			argv += 2;
		}
		/*
		 * Otherwise, argv[0] could be either <rev> or <paths> and
		 * has to be unambiguous. If there is a single argument, it
		 * can not be a tree
		 */
		وگرنه اگر ((!argv[1] && !repo_get_oid_committish(the_repository, argv[0], &unused)) ||
			 (argv[1] && !repo_get_oid_treeish(the_repository, argv[0], &unused))) {
			/*
			 * Ok, argv[0] looks like a commit/tree; it should not
			 * be a filename.
			 */
			verify_non_filename(the_repository, prefix, argv[0]);
			rev = *argv++;
		} وگرنه {
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

ایستا صحیح reset_refs(ثابت نویسه *rev, ثابت ساختار object_id *oid)
{
	صحیح update_ref_status;
	ساختار strbuf پیام = STRBUF_INIT;
	ساختار object_id *orig = پوچ, oid_orig,
		*old_orig = پوچ, oid_old_orig;

	اگر (!repo_get_oid(the_repository, "ORIG_HEAD", &oid_old_orig))
		old_orig = &oid_old_orig;
	اگر (!repo_get_oid(the_repository, "HEAD", &oid_orig)) {
		orig = &oid_orig;
		set_reflog_message(&پیام, "updating ORIG_HEAD", پوچ);
		refs_update_ref(get_main_ref_store(the_repository), پیام.buf,
				"ORIG_HEAD", orig, old_orig, 0,
				UPDATE_REFS_پیام_ON_ERR);
	} وگرنه اگر (old_orig)
		refs_delete_ref(get_main_ref_store(the_repository), پوچ,
				"ORIG_HEAD", old_orig, 0);
	set_reflog_message(&پیام, "updating HEAD", rev);
	update_ref_status = refs_update_ref(get_main_ref_store(the_repository),
					    پیام.buf, "HEAD", oid, orig, 0,
					    UPDATE_REFS_پیام_ON_ERR);
	strbuf_release(&پیام);
	بازگشت update_ref_status;
}

ایستا صحیح git_reset_config(ثابت نویسه *var, ثابت نویسه *مقدار,
			    ثابت ساختار config_context *ctx, تهی *cb)
{
	اگر (!strcmp(var, "submodule.recurse"))
		بازگشت git_default_submodule_config(var, مقدار, cb);

	بازگشت git_default_config(var, مقدار, ctx, cb);
}

صحیح cmd_reset(صحیح argc,
	      ثابت نویسه **argv,
	      ثابت نویسه *prefix,
	      ساختار repository *repo UNUSED)
{
	صحیح reset_type = NONE, update_ref_status = 0, quiet = 0;
	صحیح no_refresh = 0;
	صحیح patch_mode = 0, pathspec_file_nul = 0, unborn;
	ثابت نویسه *rev;
	نویسه *pathspec_from_file = پوچ;
	ساختار object_id oid;
	ساختار pathspec pathspec;
	صحیح intent_to_add = 0;
	ساختار interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	ثابت ساختار option options[] = {
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
		OPT_CALLBACK_F(0, "recurse-submodules", پوچ,
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

	repo_config(the_repository, git_reset_config, پوچ);

	argc = parse_options(argc, argv, prefix, options, git_reset_usage,
						PARSE_OPT_KEEP_DASHDASH);
	parse_args(&pathspec, argv, prefix, patch_mode, &rev);

	اگر (pathspec_from_file) {
		اگر (patch_mode)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--patch");

		اگر (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_FULL,
				    prefix, pathspec_from_file, pathspec_file_nul);
	} وگرنه اگر (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	unborn = !strcmp(rev, "HEAD") && repo_get_oid(the_repository, "HEAD",
						      &oid);
	اگر (unborn) {
		/* reset on unborn branch: treat as reset to empty tree */
		oidcpy(&oid, the_hash_algo->empty_درخت);
	} وگرنه اگر (!pathspec.nr && !patch_mode) {
		ساختار commit *commit;
		اگر (repo_get_oid_committish(the_repository, rev, &oid))
			die(_("Failed to resolve '%s' as a valid revision."), rev);
		commit = lookup_commit_reference(the_repository, &oid);
		اگر (!commit)
			die(_("Could not parse object '%s'."), rev);
		oidcpy(&oid, &commit->object.oid);
	} وگرنه {
		ساختار درخت *درخت;
		اگر (repo_get_oid_treeish(the_repository, rev, &oid))
			die(_("Failed to resolve '%s' as a valid tree."), rev);
		درخت = repo_parse_درخت_indirect(the_repository, &oid);
		اگر (!درخت)
			die(_("Could not parse object '%s'."), rev);
		oidcpy(&oid, &درخت->object.oid);
	}

	اگر (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	اگر (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	اگر (patch_mode) {
		اگر (reset_type != NONE)
			die(_("options '%s' and '%s' cannot be used together"), "--patch", "--{hard,mixed,soft}");
		trace2_cmd_mode("patch-interactive");
		update_ref_status = !!run_add_p(the_repository, ADD_P_RESET,
						&interactive_opts, rev, &pathspec, 0);
		برو cleanup;
	} وگرنه {
		اگر (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--patch");
		اگر (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--patch");
		اگر (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--patch");
	}

	/* git reset tree [--] paths... can be used to
	 * load chosen paths from the tree into the index without
	 * affecting the working tree nor HEAD. */
	اگر (pathspec.nr) {
		اگر (reset_type == MIXED)
			warning(_("--mixed with paths is deprecated; use 'git reset -- <paths>' instead."));
		وگرنه اگر (reset_type != NONE)
			die(_("Cannot do %s reset with paths."),
					_(reset_type_names[reset_type]));
	}
	اگر (reset_type == NONE)
		reset_type = MIXED; /* by default */

	اگر (pathspec.nr)
		trace2_cmd_mode("path");
	وگرنه
		trace2_cmd_mode(reset_type_names[reset_type]);

	اگر (reset_type != SOFT && (reset_type != MIXED || repo_get_work_درخت(the_repository)))
		setup_work_درخت(the_repository);

	اگر (reset_type == MIXED && is_bare_repository(the_repository))
		die(_("%s reset is not allowed in a bare repository"),
		    _(reset_type_names[reset_type]));

	اگر (intent_to_add && reset_type != MIXED)
		die(_("the option '%s' requires '%s'"), "-N", "--mixed");

	اگر (repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));

	/* Soft reset does not touch the index file nor the working tree
	 * at all, but requires them in a good order.  Other resets reset
	 * the index file to the tree object we are switching to. */
	اگر (reset_type == SOFT || reset_type == KEEP)
		die_if_unmerged_cache(reset_type);

	اگر (reset_type != SOFT) {
		ساختار lock_file lock = LOCK_INIT;
		repo_hold_locked_index(the_repository, &lock,
				       LOCK_DIE_ON_ERROR);
		اگر (reset_type == MIXED) {
			صحیح flags = quiet ? REFRESH_QUIET : REFRESH_IN_PORCELAIN;
			اگر (read_from_درخت(&pathspec, &oid, intent_to_add)) {
				update_ref_status = 1;
				برو cleanup;
			}
			the_repository->index->updated_skipworktree = 1;
			اگر (!no_refresh && repo_get_work_درخت(the_repository)) {
				uint64_t t_begin, t_delta_in_ms;

				t_begin = getnanotime();
				refresh_index(the_repository->index, flags, پوچ, پوچ,
					      _("Unstaged changes after reset:"));
				t_delta_in_ms = (getnanotime() - t_begin) / 1000000;
				اگر (!quiet && advice_enabled(ADVICE_RESET_NO_REFRESH_WARNING) && t_delta_in_ms > REFRESH_INDEX_DELAY_WARNING_IN_MS) {
					advise(_("It took %.2f seconds to refresh the index after reset.  You can use\n"
						 "'--no-refresh' to avoid this."), t_delta_in_ms / 1000.0);
				}
			}
		} وگرنه {
			ساختار object_id dummy;
			نویسه *ref = پوچ;
			صحیح err;

			repo_dwim_ref(the_repository, rev, strlen(rev),
				      &dummy, &ref, 0);
			اگر (ref && !starts_with(ref, "refs/"))
				FREE_AND_NULL(ref);

			err = reset_index(ref, &oid, reset_type, quiet);
			اگر (reset_type == KEEP && !err)
				err = reset_index(ref, &oid, MIXED, quiet);
			اگر (err)
				die(_("Could not reset index file to revision '%s'."), rev);
			آزاد(ref);
		}

		اگر (write_locked_index(the_repository->index, &lock, COMMIT_LOCK))
			die(_("Could not write new index file."));
	}

	اگر (!pathspec.nr && !unborn) {
		/* Any resets without paths update HEAD to the head being
		 * switched to, saving the previous head in ORIG_HEAD before. */
		update_ref_status = reset_refs(rev, &oid);

		اگر (reset_type == HARD && !update_ref_status && !quiet)
			print_new_head_سطر(lookup_commit_reference(the_repository, &oid));
	}
	اگر (!pathspec.nr)
		remove_branch_state(the_repository, 0);

	discard_index(the_repository->index);

cleanup:
	clear_pathspec(&pathspec);
	آزاد(pathspec_from_file);
	بازگشت update_ref_status;
}
