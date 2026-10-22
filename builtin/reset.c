/*
 * "git reset" builtin command
 *
 * Copyright (c) 2007 Carlos Rica
 *
 * Based on git-reset.sh, which is
 *
 * Copyright (c) 2005, 2006 Linus Torvalds and Junio C Hamano
 */

#تعریف_کړه USE_THE_REPOSITORY_VARIABLE

#ضمن "builtin.h"
#ضمن "advice.h"
#ضمن "config.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "hash.h"
#ضمن "hex.h"
#ضمن "lockfile.h"
#ضمن "object.h"
#ضمن "pretty.h"
#ضمن "refs.h"
#ضمن "diff.h"
#ضمن "diffcore.h"
#ضمن "tree.h"
#ضمن "branch.h"
#ضمن "object-name.h"
#ضمن "parse-options.h"
#ضمن "path.h"
#ضمن "repository.h"
#ضمن "unpack-trees.h"
#ضمن "cache-tree.h"
#ضمن "setup.h"
#ضمن "sparse-index.h"
#ضمن "submodule.h"
#ضمن "trace.h"
#ضمن "trace2.h"
#ضمن "dir.h"
#ضمن "add-interactive.h"

#تعریف_کړه REFRESH_شاخص_DELAY_WARNING_دننه_MS (2 * 1000)

ساكن ثابت حرف * ثابت git_reset_usage[] = {
	N_("git reset [--mixed | --soft | --hard | --merge | --keep] [-q] [<commit>]"),
	N_("git reset [-q] [<tree-ish>] [--] <pathspec>..."),
	N_("git reset [-q] [--pathspec-from-file [--pathspec-file-nul]] [<tree-ish>]"),
	N_("git reset --patch [<tree-ish>] [--] [<pathspec>...]"),
	عدم
};

تعداد reset_type { MIXED, SOFT, HARD, یوځای_2, KEEP, NONE };
ساكن ثابت حرف *reset_type_نومونه[] = {
	N_("mixed"), N_("soft"), N_("hard"), N_("merge"), N_("keep"), عدم
};

ساكن مضمن صحيح is_یوځای(فراغ)
{
	أرجع !access(git_لار_یوځای_سر(the_repository), F_OK);
}

ساكن صحيح reset_شاخص(ثابت حرف *ref, ثابت بنية object_id *oid, صحيح reset_type, صحيح quiet)
{
	صحيح i, nr = 0;
	بنية ونه_desc desc[2];
	بنية ونه *ونه;
	بنية unpack_trees_options opts;
	صحيح ret = -1;

	memset(&opts, 0, حجم(opts));
	opts.سر_idx = 1;
	opts.src_شاخص = the_repository->شاخص;
	opts.dst_شاخص = the_repository->شاخص;
	opts.fn = oneway_یوځای;
	opts.یوځای = 1;
	init_checkout_metadata(&opts.meta, ref, oid, عدم);
	إذا (!quiet)
		opts.verbose_update = 1;
	بدل (reset_type) {
	حالة KEEP:
	حالة یوځای_2:
		opts.update = 1;
		opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
		اكسر;
	حالة HARD:
		opts.update = 1;
		opts.reset = UNPACK_RESET_OVERWRITE_UNTRACKED;
		opts.skip_cache_ونه_update = 1;
		اكسر;
	حالة MIXED:
		opts.reset = UNPACK_RESET_PROTECT_UNTRACKED;
		opts.skip_cache_ونه_update = 1;
		/* but opts.update=0, so working tree not updated */
		اكسر;
	افتراضي:
		BUG("invalid reset_type passed to reset_index");
	}

	repo_read_شاخص_unmerged(the_repository);

	إذا (reset_type == KEEP) {
		بنية object_id سر_oid;
		إذا (repo_get_oid(the_repository, "HEAD", &سر_oid))
			أرجع error(_("You do not have a valid HEAD."));
		إذا (!fill_ونه_descriptor(the_repository, desc + nr, &سر_oid))
			أرجع error(_("Failed to find tree of HEAD."));
		nr++;
		opts.fn = twoway_یوځای;
	}

	إذا (!fill_ونه_descriptor(the_repository, desc + nr, oid)) {
		error(_("Failed to find tree of %s."), oid_منزل_hex(oid));
		اذهب بهر;
	}
	nr++;

	إذا (unpack_trees(nr, desc, &opts))
		اذهب بهر;

	إذا (reset_type == MIXED || reset_type == HARD) {
		ونه = repo_parse_ونه_غیرمستقیم(the_repository, oid);
		إذا (!ونه) {
			error(_("unable to read tree (%s)"), oid_منزل_hex(oid));
			اذهب بهر;
		}
		اولي_cache_ونه(the_repository, the_repository->شاخص, ونه);
	}

	ret = 0;

بهر:
	لكل (i = 0; i < nr; i++)
		حرر((فراغ *)desc[i].بفر);
	أرجع ret;
}

ساكن فراغ print_new_سر_کرښه(بنية commit *commit)
{
	بنية strbuf buf = STRBUF_INIT;

	چاپ_کړه(_("HEAD is now at %s"),
		repo_پیدا_کړه_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV));

	pp_commit_easy(CMIT_FMT_ONELINE, commit, &buf);
	إذا (buf.len > 0)
		چاپ_کړه(" %s", buf.buf);
	توری_چاپ_کړه('\n');
	strbuf_release(&buf);
}

ساكن فراغ update_شاخص_from_diff(بنية diff_کتار_struct *q,
				   بنية diff_options *opt ناکارول_شوی,
				   فراغ *معلومات)
{
	صحيح i;
	صحيح intent_منزل_جمع_کړه = *(صحيح *)معلومات;

	لكل (i = 0; i < q->nr; i++) {
		صحيح pos;
		بنية diff_filespec *one = q->کتار[i]->one;
		صحيح is_دننه_reset_ونه = one->ډېرپېښ && !is_null_oid(&one->oid);
		بنية cache_ثبت *ce;

		إذا (!is_دننه_reset_ونه && !intent_منزل_جمع_کړه) {
			remove_file_from_شاخص(the_repository->شاخص, one->لار);
			استمر;
		}

		ce = make_cache_ثبت(the_repository->شاخص, one->ډېرپېښ, &one->oid, one->لار,
				      0, 0);

		/*
		 * If the file 1) corresponds to an existing index entry with
		 * skip-worktree set, or 2) does not exist in the index but is
		 * outside the sparse checkout definition, add a skip-worktree bit
		 * to the new index entry. Note that a sparse index will be expanded
		 * if this entry is outside the sparse cone - this is necessary
		 * to properly construct the reset sparse directory.
		 */
		pos = شاخص_name_pos(the_repository->شاخص, one->لار, مزي_اوږدوالی(one->لار));
		إذا ((pos >= 0 && ce_skip_worktree(the_repository->شاخص->cache[pos])) ||
		    (pos < 0 && !لار_دننه_sparse_checkout(one->لار, the_repository->شاخص)))
			ce->ce_flags |= CE_SKIP_WORKTREE;

		إذا (!ce)
			die(_("make_cache_entry failed for path '%s'"),
			    one->لار);
		إذا (!is_دننه_reset_ونه) {
			ce->ce_flags |= CE_INTENT_منزل_جمع_کړه;
			سېټ_object_name_for_intent_منزل_جمع_کړه_ثبت(ce);
		}
		جمع_کړه_شاخص_ثبت(the_repository->شاخص, ce,
				جمع_کړه_CACHE_OK_منزل_جمع_کړه | جمع_کړه_CACHE_OK_منزل_REPLACE);
	}
}

ساكن صحيح read_from_ونه(ثابت بنية pathspec *pathspec,
			  بنية object_id *ونه_oid,
			  صحيح intent_منزل_جمع_کړه)
{
	بنية diff_options opt;

	memset(&opt, 0, حجم(opt));
	copy_pathspec(&opt.pathspec, pathspec);
	opt.output_format = DIFF_FORMAT_CALLBACK;
	opt.format_callback = update_شاخص_from_diff;
	opt.format_callback_معلومات = &intent_منزل_جمع_کړه;
	opt.flags.override_submodule_config = 1;
	opt.flags.recursive = 1;
	opt.repo = the_repository;
	opt.change = diff_change;
	opt.جمع_کړه_remove = diff_addremove;

	إذا (pathspec->nr && pathspec_needs_expanded_شاخص(the_repository->شاخص, pathspec))
		ensure_ډک_شاخص(the_repository->شاخص);

	إذا (do_diff_cache(ونه_oid, &opt))
		أرجع 1;
	diffcore_std(&opt);
	diff_flush(&opt);

	أرجع 0;
}

ساكن فراغ سېټ_reflog_پیغام(بنية strbuf *sb, ثابت حرف *action,
			       ثابت حرف *rev)
{
	ثابت حرف *rla = getenv("GIT_REFLOG_ACTION");

	strbuf_reset(sb);
	إذا (rla)
		strbuf_addf(sb, "%s: %s", rla, action);
	وإلا إذا (rev)
		strbuf_addf(sb, "reset: moving to %s", rev);
	وإلا
		strbuf_addf(sb, "reset: %s", action);
}

ساكن فراغ die_if_unmerged_cache(صحيح reset_type)
{
	إذا (is_یوځای() || unmerged_شاخص(the_repository->شاخص))
		die(_("Cannot do a %s reset in the middle of a merge."),
		    _(reset_type_نومونه[reset_type]));

}

ساكن فراغ parse_دلیلونه(بنية pathspec *pathspec,
		       ثابت حرف **argv, ثابت حرف *مختاړی,
		       صحيح patch_ډېرپېښ,
		       ثابت حرف **rev_ret)
{
	ثابت حرف *rev = "HEAD";
	بنية object_id ناکارول_شوی_2;
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

	إذا (argv[0]) {
		إذا (!strcmp(argv[0], "--")) {
			argv++; /* reset to HEAD, possibly with paths */
		} وإلا إذا (argv[1] && !strcmp(argv[1], "--")) {
			rev = argv[0];
			argv += 2;
		}
		/*
		 * Otherwise, argv[0] could be either <rev> or <paths> and
		 * has to be unambiguous. If there is a single argument, it
		 * can not be a tree
		 */
		وإلا إذا ((!argv[1] && !repo_get_oid_committish(the_repository, argv[0], &ناکارول_شوی_2)) ||
			 (argv[1] && !repo_get_oid_treeish(the_repository, argv[0], &ناکارول_شوی_2))) {
			/*
			 * Ok, argv[0] looks like a commit/tree; it should not
			 * be a filename.
			 */
			verify_non_filename(the_repository, مختاړی, argv[0]);
			rev = *argv++;
		} وإلا {
			/* Otherwise we treat this as a filename */
			verify_filename(the_repository, مختاړی, argv[0], 1);
		}
	}

	/* treat '@' as a shortcut for 'HEAD' */
	*rev_ret = !strcmp("@", rev) ? "HEAD" : rev;

	parse_pathspec(pathspec, 0,
		       PATHSPEC_PREFER_ډک |
		       (patch_ډېرپېښ ? PATHSPEC_مختاړی_ORIGIN : 0),
		       مختاړی, argv);
}

ساكن صحيح reset_refs(ثابت حرف *rev, ثابت بنية object_id *oid)
{
	صحيح update_ref_حالت;
	بنية strbuf پیغام = STRBUF_INIT;
	بنية object_id *orig = عدم, oid_orig,
		*old_orig = عدم, oid_old_orig;

	إذا (!repo_get_oid(the_repository, "ORIG_HEAD", &oid_old_orig))
		old_orig = &oid_old_orig;
	إذا (!repo_get_oid(the_repository, "HEAD", &oid_orig)) {
		orig = &oid_orig;
		سېټ_reflog_پیغام(&پیغام, "updating ORIG_HEAD", عدم);
		refs_update_ref(get_main_ref_store(the_repository), پیغام.buf,
				"ORIG_HEAD", orig, old_orig, 0,
				UPDATE_REFS_پیغام_ON_ERR);
	} وإلا إذا (old_orig)
		refs_delete_ref(get_main_ref_store(the_repository), عدم,
				"ORIG_HEAD", old_orig, 0);
	سېټ_reflog_پیغام(&پیغام, "updating HEAD", rev);
	update_ref_حالت = refs_update_ref(get_main_ref_store(the_repository),
					    پیغام.buf, "HEAD", oid, orig, 0,
					    UPDATE_REFS_پیغام_ON_ERR);
	strbuf_release(&پیغام);
	أرجع update_ref_حالت;
}

ساكن صحيح git_reset_config(ثابت حرف *var, ثابت حرف *ارزښت,
			    ثابت بنية config_context *ctx, فراغ *cb)
{
	إذا (!strcmp(var, "submodule.recurse"))
		أرجع git_default_submodule_config(var, ارزښت, cb);

	أرجع git_default_config(var, ارزښت, ctx, cb);
}

صحيح cmd_reset(صحيح argc,
	      ثابت حرف **argv,
	      ثابت حرف *مختاړی,
	      بنية repository *repo ناکارول_شوی)
{
	صحيح reset_type = NONE, update_ref_حالت = 0, quiet = 0;
	صحيح no_refresh = 0;
	صحيح patch_ډېرپېښ = 0, pathspec_file_nul = 0, unborn;
	ثابت حرف *rev;
	حرف *pathspec_from_file = عدم;
	بنية object_id oid;
	بنية pathspec pathspec;
	صحيح intent_منزل_جمع_کړه = 0;
	بنية interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	ثابت بنية option options[] = {
		OPT__QUIET(&quiet, N_("be quiet, only report errors")),
		OPT_BOOL(0, "no-refresh", &no_refresh,
				N_("skip refreshing the index after reset")),
		OPT_سېټ_INT_F(0, "mixed", &reset_type,
			      N_("reset HEAD and index"),
			      MIXED, PARSE_OPT_NONEG),
		OPT_سېټ_INT_F(0, "soft", &reset_type,
			      N_("reset only HEAD"),
			      SOFT, PARSE_OPT_NONEG),
		OPT_سېټ_INT_F(0, "hard", &reset_type,
			      N_("reset HEAD, index and working tree"),
			      HARD, PARSE_OPT_NONEG),
		OPT_سېټ_INT_F(0, "merge", &reset_type,
			      N_("reset HEAD, index and working tree"),
			      یوځای_2, PARSE_OPT_NONEG),
		OPT_سېټ_INT_F(0, "keep", &reset_type,
			      N_("reset HEAD but keep local changes"),
			      KEEP, PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "recurse-submodules", عدم,
			       "reset", "control recursive updating of submodules",
			       PARSE_OPT_OPTARG,
			       option_parse_recurse_submodules_worktree_updater),
		OPT_BOOL('p', "patch", &patch_ډېرپېښ, N_("select hunks interactively")),
		OPT_BOOL(0, "auto-advance", &interactive_opts.auto_advance,
			 N_("auto advance to the next file when selecting hunks interactively")),
		OPT_DIFF_UNIFIED(&interactive_opts.context),
		OPT_DIFF_INTERHUNK_CONTEXT(&interactive_opts.interhunkcontext),
		OPT_BOOL('N', "intent-to-add", &intent_منزل_جمع_کړه,
				N_("record only the fact that removed paths will be added later")),
		OPT_PATHSPEC_FROM_FILE(&pathspec_from_file),
		OPT_PATHSPEC_FILE_NUL(&pathspec_file_nul),
		OPT_پای_ټکی()
	};

	repo_config(the_repository, git_reset_config, عدم);

	argc = parse_options(argc, argv, مختاړی, options, git_reset_usage,
						PARSE_OPT_KEEP_DASHDASH);
	parse_دلیلونه(&pathspec, argv, مختاړی, patch_ډېرپېښ, &rev);

	إذا (pathspec_from_file) {
		إذا (patch_ډېرپېښ)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--patch");

		إذا (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_ډک,
				    مختاړی, pathspec_from_file, pathspec_file_nul);
	} وإلا إذا (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	unborn = !strcmp(rev, "HEAD") && repo_get_oid(the_repository, "HEAD",
						      &oid);
	إذا (unborn) {
		/* reset on unborn branch: treat as reset to empty tree */
		oidcpy(&oid, the_hash_algo->تشه_ونه);
	} وإلا إذا (!pathspec.nr && !patch_ډېرپېښ) {
		بنية commit *commit;
		إذا (repo_get_oid_committish(the_repository, rev, &oid))
			die(_("Failed to resolve '%s' as a valid revision."), rev);
		commit = lookup_commit_reference(the_repository, &oid);
		إذا (!commit)
			die(_("Could not parse object '%s'."), rev);
		oidcpy(&oid, &commit->object.oid);
	} وإلا {
		بنية ونه *ونه;
		إذا (repo_get_oid_treeish(the_repository, rev, &oid))
			die(_("Failed to resolve '%s' as a valid tree."), rev);
		ونه = repo_parse_ونه_غیرمستقیم(the_repository, &oid);
		إذا (!ونه)
			die(_("Could not parse object '%s'."), rev);
		oidcpy(&oid, &ونه->object.oid);
	}

	إذا (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	إذا (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_ډک_شاخص = 0;

	إذا (patch_ډېرپېښ) {
		إذا (reset_type != NONE)
			die(_("options '%s' and '%s' cannot be used together"), "--patch", "--{hard,mixed,soft}");
		trace2_cmd_ډېرپېښ("patch-interactive");
		update_ref_حالت = !!پرلپسې_جمع_کړه_p(the_repository, جمع_کړه_P_RESET,
						&interactive_opts, rev, &pathspec, 0);
		اذهب cleanup;
	} وإلا {
		إذا (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--patch");
		إذا (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--patch");
		إذا (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--patch");
	}

	/* git reset tree [--] paths... can be used to
	 * load chosen paths from the tree into the index without
	 * affecting the working tree nor HEAD. */
	إذا (pathspec.nr) {
		إذا (reset_type == MIXED)
			warning(_("--mixed with paths is deprecated; use 'git reset -- <paths>' instead."));
		وإلا إذا (reset_type != NONE)
			die(_("Cannot do %s reset with paths."),
					_(reset_type_نومونه[reset_type]));
	}
	إذا (reset_type == NONE)
		reset_type = MIXED; /* by default */

	إذا (pathspec.nr)
		trace2_cmd_ډېرپېښ("path");
	وإلا
		trace2_cmd_ډېرپېښ(reset_type_نومونه[reset_type]);

	إذا (reset_type != SOFT && (reset_type != MIXED || repo_get_work_ونه(the_repository)))
		تیاری_work_ونه(the_repository);

	إذا (reset_type == MIXED && is_bare_repository(the_repository))
		die(_("%s reset is not allowed in a bare repository"),
		    _(reset_type_نومونه[reset_type]));

	إذا (intent_منزل_جمع_کړه && reset_type != MIXED)
		die(_("the option '%s' requires '%s'"), "-N", "--mixed");

	إذا (repo_read_شاخص(the_repository) < 0)
		die(_("index file corrupt"));

	/* Soft reset does not touch the index file nor the working tree
	 * at all, but requires them in a good order.  Other resets reset
	 * the index file to the tree object we are switching to. */
	إذا (reset_type == SOFT || reset_type == KEEP)
		die_if_unmerged_cache(reset_type);

	إذا (reset_type != SOFT) {
		بنية قلف_file قلف = قلف_INIT;
		repo_hold_locked_شاخص(the_repository, &قلف,
				       قلف_DIE_ON_ERROR);
		إذا (reset_type == MIXED) {
			صحيح flags = quiet ? REFRESH_QUIET : REFRESH_دننه_PORCELAIN;
			إذا (read_from_ونه(&pathspec, &oid, intent_منزل_جمع_کړه)) {
				update_ref_حالت = 1;
				اذهب cleanup;
			}
			the_repository->شاخص->updated_skipworktree = 1;
			إذا (!no_refresh && repo_get_work_ونه(the_repository)) {
				uint64_t t_begin, t_delta_دننه_ms;

				t_begin = getnanotime();
				refresh_شاخص(the_repository->شاخص, flags, عدم, عدم,
					      _("Unstaged changes after reset:"));
				t_delta_دننه_ms = (getnanotime() - t_begin) / 1000000;
				إذا (!quiet && advice_enabled(ADVICE_RESET_NO_REFRESH_WARNING) && t_delta_دننه_ms > REFRESH_شاخص_DELAY_WARNING_دننه_MS) {
					advise(_("It took %.2f seconds to refresh the index after reset.  You can use\n"
						 "'--no-refresh' to avoid this."), t_delta_دننه_ms / 1000.0);
				}
			}
		} وإلا {
			بنية object_id dummy;
			حرف *ref = عدم;
			صحيح err;

			repo_dwim_ref(the_repository, rev, مزي_اوږدوالی(rev),
				      &dummy, &ref, 0);
			إذا (ref && !starts_with(ref, "refs/"))
				FREE_AND_NULL(ref);

			err = reset_شاخص(ref, &oid, reset_type, quiet);
			إذا (reset_type == KEEP && !err)
				err = reset_شاخص(ref, &oid, MIXED, quiet);
			إذا (err)
				die(_("Could not reset index file to revision '%s'."), rev);
			حرر(ref);
		}

		إذا (write_locked_شاخص(the_repository->شاخص, &قلف, COMMIT_قلف))
			die(_("Could not write new index file."));
	}

	إذا (!pathspec.nr && !unborn) {
		/* Any resets without paths update HEAD to the head being
		 * switched to, saving the previous head in ORIG_HEAD before. */
		update_ref_حالت = reset_refs(rev, &oid);

		إذا (reset_type == HARD && !update_ref_حالت && !quiet)
			print_new_سر_کرښه(lookup_commit_reference(the_repository, &oid));
	}
	إذا (!pathspec.nr)
		remove_branch_state(the_repository, 0);

	discard_شاخص(the_repository->شاخص);

cleanup:
	clear_pathspec(&pathspec);
	حرر(pathspec_from_file);
	أرجع update_ref_حالت;
}
