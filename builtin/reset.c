/*
 * "git reset" builtin command
 *
 * Copyright (c) 2007 Carlos Rica
 *
 * Based on git-reset.sh, which is
 *
 * Copyright (c) 2005, 2006 Linus Torvalds and Junio C Hamano
 */

#تعریف_کرو USE_THE_REPOSITORY_VARIABLE

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

#تعریف_کرو REFRESH_اشاریہ_DELAY_WARNING_اندر_MS (2 * 1000)

ساکن ثابت حرف * ثابت git_reset_usage[] = {
	N_("git reset [--mixed | --soft | --hard | --merge | --keep] [-q] [<commit>]"),
	N_("git reset [-q] [<tree-ish>] [--] <pathspec>..."),
	N_("git reset [-q] [--pathspec-from-file [--pathspec-file-nul]] [<tree-ish>]"),
	N_("git reset --patch [<tree-ish>] [--] [<pathspec>...]"),
	تہی
};

شمار reset_type { MIXED, SOFT, HARD, ملاؤ_2, KEEP, NONE };
ساکن ثابت حرف *reset_type_نام[] = {
	N_("mixed"), N_("soft"), N_("hard"), N_("merge"), N_("keep"), تہی
};

ساکن اندرونی صحیح is_ملاؤ(خالی)
{
	واپس !access(git_راستہ_ملاؤ_سر(the_repository), F_OK);
}

ساکن صحیح reset_اشاریہ(ثابت حرف *ref, ثابت ساخت object_id *oid, صحیح reset_type, صحیح quiet)
{
	صحیح i, nr = 0;
	ساخت درخت_desc desc[2];
	ساخت درخت *درخت;
	ساخت unpack_trees_options opts;
	صحیح ret = -1;

	memset(&opts, 0, حجم(opts));
	opts.سر_idx = 1;
	opts.src_اشاریہ = the_repository->اشاریہ;
	opts.dst_اشاریہ = the_repository->اشاریہ;
	opts.fn = oneway_ملاؤ;
	opts.ملاؤ = 1;
	init_checkout_metadata(&opts.meta, ref, oid, تہی);
	اگر (!quiet)
		opts.verbose_update = 1;
	انتخاب (reset_type) {
	حالت KEEP:
	حالت ملاؤ_2:
		opts.update = 1;
		opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
		توڑو;
	حالت HARD:
		opts.update = 1;
		opts.reset = UNPACK_RESET_OVERWRITE_UNTRACKED;
		opts.skip_cache_درخت_update = 1;
		توڑو;
	حالت MIXED:
		opts.reset = UNPACK_RESET_PROTECT_UNTRACKED;
		opts.skip_cache_درخت_update = 1;
		/* but opts.update=0, so working tree not updated */
		توڑو;
	طےشدہ:
		BUG("invalid reset_type passed to reset_index");
	}

	repo_read_اشاریہ_unmerged(the_repository);

	اگر (reset_type == KEEP) {
		ساخت object_id سر_oid;
		اگر (repo_get_oid(the_repository, "HEAD", &سر_oid))
			واپس error(_("You do not have a valid HEAD."));
		اگر (!fill_درخت_descriptor(the_repository, desc + nr, &سر_oid))
			واپس error(_("Failed to find tree of HEAD."));
		nr++;
		opts.fn = twoway_ملاؤ;
	}

	اگر (!fill_درخت_descriptor(the_repository, desc + nr, oid)) {
		error(_("Failed to find tree of %s."), oid_مقصد_hex(oid));
		جاؤ برآمد;
	}
	nr++;

	اگر (unpack_trees(nr, desc, &opts))
		جاؤ برآمد;

	اگر (reset_type == MIXED || reset_type == HARD) {
		درخت = repo_parse_درخت_بالواسطہ(the_repository, oid);
		اگر (!درخت) {
			error(_("unable to read tree (%s)"), oid_مقصد_hex(oid));
			جاؤ برآمد;
		}
		مفرد_cache_درخت(the_repository, the_repository->اشاریہ, درخت);
	}

	ret = 0;

برآمد:
	برائے (i = 0; i < nr; i++)
		آزاد((خالی *)desc[i].بفر);
	واپس ret;
}

ساکن خالی print_new_سر_سطر(ساخت commit *commit)
{
	ساخت strbuf buf = STRBUF_INIT;

	لکھو(_("HEAD is now at %s"),
		repo_ڈھونڈو_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV));

	pp_commit_easy(CMIT_FMT_ONELINE, commit, &buf);
	اگر (buf.len > 0)
		لکھو(" %s", buf.buf);
	حرف_لکھو('\n');
	strbuf_release(&buf);
}

ساکن خالی update_اشاریہ_from_diff(ساخت diff_قطار_struct *q,
				   ساخت diff_options *opt غیر_مستعمل,
				   خالی *ڈیٹا)
{
	صحیح i;
	صحیح intent_مقصد_جوڑو = *(صحیح *)ڈیٹا;

	برائے (i = 0; i < q->nr; i++) {
		صحیح pos;
		ساخت diff_filespec *one = q->قطار[i]->one;
		صحیح is_اندر_reset_درخت = one->منوال && !is_null_oid(&one->oid);
		ساخت cache_اندراج *ce;

		اگر (!is_اندر_reset_درخت && !intent_مقصد_جوڑو) {
			remove_file_from_اشاریہ(the_repository->اشاریہ, one->راستہ);
			جاری;
		}

		ce = make_cache_اندراج(the_repository->اشاریہ, one->منوال, &one->oid, one->راستہ,
				      0, 0);

		/*
		 * If the file 1) corresponds to an existing index entry with
		 * skip-worktree set, or 2) does not exist in the index but is
		 * outside the sparse checkout definition, add a skip-worktree bit
		 * to the new index entry. Note that a sparse index will be expanded
		 * if this entry is outside the sparse cone - this is necessary
		 * to properly construct the reset sparse directory.
		 */
		pos = اشاریہ_name_pos(the_repository->اشاریہ, one->راستہ, لڑی_لمبائی(one->راستہ));
		اگر ((pos >= 0 && ce_skip_worktree(the_repository->اشاریہ->cache[pos])) ||
		    (pos < 0 && !راستہ_اندر_sparse_checkout(one->راستہ, the_repository->اشاریہ)))
			ce->ce_flags |= CE_SKIP_WORKTREE;

		اگر (!ce)
			die(_("make_cache_entry failed for path '%s'"),
			    one->راستہ);
		اگر (!is_اندر_reset_درخت) {
			ce->ce_flags |= CE_INTENT_مقصد_جوڑو;
			سیٹ_object_name_for_intent_مقصد_جوڑو_اندراج(ce);
		}
		جوڑو_اشاریہ_اندراج(the_repository->اشاریہ, ce,
				جوڑو_CACHE_OK_مقصد_جوڑو | جوڑو_CACHE_OK_مقصد_REPLACE);
	}
}

ساکن صحیح read_from_درخت(ثابت ساخت pathspec *pathspec,
			  ساخت object_id *درخت_oid,
			  صحیح intent_مقصد_جوڑو)
{
	ساخت diff_options opt;

	memset(&opt, 0, حجم(opt));
	copy_pathspec(&opt.pathspec, pathspec);
	opt.output_format = DIFF_FORMAT_CALLBACK;
	opt.format_callback = update_اشاریہ_from_diff;
	opt.format_callback_ڈیٹا = &intent_مقصد_جوڑو;
	opt.flags.override_submodule_config = 1;
	opt.flags.recursive = 1;
	opt.repo = the_repository;
	opt.change = diff_change;
	opt.جوڑو_remove = diff_addremove;

	اگر (pathspec->nr && pathspec_needs_expanded_اشاریہ(the_repository->اشاریہ, pathspec))
		ensure_بھرا_اشاریہ(the_repository->اشاریہ);

	اگر (do_diff_cache(درخت_oid, &opt))
		واپس 1;
	diffcore_std(&opt);
	diff_flush(&opt);

	واپس 0;
}

ساکن خالی سیٹ_reflog_پیغام(ساخت strbuf *sb, ثابت حرف *action,
			       ثابت حرف *rev)
{
	ثابت حرف *rla = getenv("GIT_REFLOG_ACTION");

	strbuf_reset(sb);
	اگر (rla)
		strbuf_addf(sb, "%s: %s", rla, action);
	ورنہ اگر (rev)
		strbuf_addf(sb, "reset: moving to %s", rev);
	ورنہ
		strbuf_addf(sb, "reset: %s", action);
}

ساکن خالی die_if_unmerged_cache(صحیح reset_type)
{
	اگر (is_ملاؤ() || unmerged_اشاریہ(the_repository->اشاریہ))
		die(_("Cannot do a %s reset in the middle of a merge."),
		    _(reset_type_نام[reset_type]));

}

ساکن خالی parse_دلائل(ساخت pathspec *pathspec,
		       ثابت حرف **argv, ثابت حرف *سابقہ,
		       صحیح patch_منوال,
		       ثابت حرف **rev_ret)
{
	ثابت حرف *rev = "HEAD";
	ساخت object_id غیر_مستعمل_2;
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
		} ورنہ اگر (argv[1] && !strcmp(argv[1], "--")) {
			rev = argv[0];
			argv += 2;
		}
		/*
		 * Otherwise, argv[0] could be either <rev> or <paths> and
		 * has to be unambiguous. If there is a single argument, it
		 * can not be a tree
		 */
		ورنہ اگر ((!argv[1] && !repo_get_oid_committish(the_repository, argv[0], &غیر_مستعمل_2)) ||
			 (argv[1] && !repo_get_oid_treeish(the_repository, argv[0], &غیر_مستعمل_2))) {
			/*
			 * Ok, argv[0] looks like a commit/tree; it should not
			 * be a filename.
			 */
			verify_non_filename(the_repository, سابقہ, argv[0]);
			rev = *argv++;
		} ورنہ {
			/* Otherwise we treat this as a filename */
			verify_filename(the_repository, سابقہ, argv[0], 1);
		}
	}

	/* treat '@' as a shortcut for 'HEAD' */
	*rev_ret = !strcmp("@", rev) ? "HEAD" : rev;

	parse_pathspec(pathspec, 0,
		       PATHSPEC_PREFER_بھرا |
		       (patch_منوال ? PATHSPEC_سابقہ_ORIGIN : 0),
		       سابقہ, argv);
}

ساکن صحیح reset_refs(ثابت حرف *rev, ثابت ساخت object_id *oid)
{
	صحیح update_ref_حالت;
	ساخت strbuf پیغام = STRBUF_INIT;
	ساخت object_id *orig = تہی, oid_orig,
		*old_orig = تہی, oid_old_orig;

	اگر (!repo_get_oid(the_repository, "ORIG_HEAD", &oid_old_orig))
		old_orig = &oid_old_orig;
	اگر (!repo_get_oid(the_repository, "HEAD", &oid_orig)) {
		orig = &oid_orig;
		سیٹ_reflog_پیغام(&پیغام, "updating ORIG_HEAD", تہی);
		refs_update_ref(get_main_ref_store(the_repository), پیغام.buf,
				"ORIG_HEAD", orig, old_orig, 0,
				UPDATE_REFS_پیغام_ON_ERR);
	} ورنہ اگر (old_orig)
		refs_delete_ref(get_main_ref_store(the_repository), تہی,
				"ORIG_HEAD", old_orig, 0);
	سیٹ_reflog_پیغام(&پیغام, "updating HEAD", rev);
	update_ref_حالت = refs_update_ref(get_main_ref_store(the_repository),
					    پیغام.buf, "HEAD", oid, orig, 0,
					    UPDATE_REFS_پیغام_ON_ERR);
	strbuf_release(&پیغام);
	واپس update_ref_حالت;
}

ساکن صحیح git_reset_config(ثابت حرف *var, ثابت حرف *قدر,
			    ثابت ساخت config_context *ctx, خالی *cb)
{
	اگر (!strcmp(var, "submodule.recurse"))
		واپس git_default_submodule_config(var, قدر, cb);

	واپس git_default_config(var, قدر, ctx, cb);
}

صحیح cmd_reset(صحیح argc,
	      ثابت حرف **argv,
	      ثابت حرف *سابقہ,
	      ساخت repository *repo غیر_مستعمل)
{
	صحیح reset_type = NONE, update_ref_حالت = 0, quiet = 0;
	صحیح no_refresh = 0;
	صحیح patch_منوال = 0, pathspec_file_nul = 0, unborn;
	ثابت حرف *rev;
	حرف *pathspec_from_file = تہی;
	ساخت object_id oid;
	ساخت pathspec pathspec;
	صحیح intent_مقصد_جوڑو = 0;
	ساخت interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	ثابت ساخت option options[] = {
		OPT__QUIET(&quiet, N_("be quiet, only report errors")),
		OPT_BOOL(0, "no-refresh", &no_refresh,
				N_("skip refreshing the index after reset")),
		OPT_سیٹ_INT_F(0, "mixed", &reset_type,
			      N_("reset HEAD and index"),
			      MIXED, PARSE_OPT_NONEG),
		OPT_سیٹ_INT_F(0, "soft", &reset_type,
			      N_("reset only HEAD"),
			      SOFT, PARSE_OPT_NONEG),
		OPT_سیٹ_INT_F(0, "hard", &reset_type,
			      N_("reset HEAD, index and working tree"),
			      HARD, PARSE_OPT_NONEG),
		OPT_سیٹ_INT_F(0, "merge", &reset_type,
			      N_("reset HEAD, index and working tree"),
			      ملاؤ_2, PARSE_OPT_NONEG),
		OPT_سیٹ_INT_F(0, "keep", &reset_type,
			      N_("reset HEAD but keep local changes"),
			      KEEP, PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "recurse-submodules", تہی,
			       "reset", "control recursive updating of submodules",
			       PARSE_OPT_OPTARG,
			       option_parse_recurse_submodules_worktree_updater),
		OPT_BOOL('p', "patch", &patch_منوال, N_("select hunks interactively")),
		OPT_BOOL(0, "auto-advance", &interactive_opts.auto_advance,
			 N_("auto advance to the next file when selecting hunks interactively")),
		OPT_DIFF_UNIFIED(&interactive_opts.context),
		OPT_DIFF_INTERHUNK_CONTEXT(&interactive_opts.interhunkcontext),
		OPT_BOOL('N', "intent-to-add", &intent_مقصد_جوڑو,
				N_("record only the fact that removed paths will be added later")),
		OPT_PATHSPEC_FROM_FILE(&pathspec_from_file),
		OPT_PATHSPEC_FILE_NUL(&pathspec_file_nul),
		OPT_انتہا()
	};

	repo_config(the_repository, git_reset_config, تہی);

	argc = parse_options(argc, argv, سابقہ, options, git_reset_usage,
						PARSE_OPT_KEEP_DASHDASH);
	parse_دلائل(&pathspec, argv, سابقہ, patch_منوال, &rev);

	اگر (pathspec_from_file) {
		اگر (patch_منوال)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--patch");

		اگر (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_بھرا,
				    سابقہ, pathspec_from_file, pathspec_file_nul);
	} ورنہ اگر (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	unborn = !strcmp(rev, "HEAD") && repo_get_oid(the_repository, "HEAD",
						      &oid);
	اگر (unborn) {
		/* reset on unborn branch: treat as reset to empty tree */
		oidcpy(&oid, the_hash_algo->خالی_خانے_درخت);
	} ورنہ اگر (!pathspec.nr && !patch_منوال) {
		ساخت commit *commit;
		اگر (repo_get_oid_committish(the_repository, rev, &oid))
			die(_("Failed to resolve '%s' as a valid revision."), rev);
		commit = lookup_commit_reference(the_repository, &oid);
		اگر (!commit)
			die(_("Could not parse object '%s'."), rev);
		oidcpy(&oid, &commit->object.oid);
	} ورنہ {
		ساخت درخت *درخت;
		اگر (repo_get_oid_treeish(the_repository, rev, &oid))
			die(_("Failed to resolve '%s' as a valid tree."), rev);
		درخت = repo_parse_درخت_بالواسطہ(the_repository, &oid);
		اگر (!درخت)
			die(_("Could not parse object '%s'."), rev);
		oidcpy(&oid, &درخت->object.oid);
	}

	اگر (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	اگر (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_بھرا_اشاریہ = 0;

	اگر (patch_منوال) {
		اگر (reset_type != NONE)
			die(_("options '%s' and '%s' cannot be used together"), "--patch", "--{hard,mixed,soft}");
		trace2_cmd_منوال("patch-interactive");
		update_ref_حالت = !!تسلسل_جوڑو_p(the_repository, جوڑو_P_RESET,
						&interactive_opts, rev, &pathspec, 0);
		جاؤ cleanup;
	} ورنہ {
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
		ورنہ اگر (reset_type != NONE)
			die(_("Cannot do %s reset with paths."),
					_(reset_type_نام[reset_type]));
	}
	اگر (reset_type == NONE)
		reset_type = MIXED; /* by default */

	اگر (pathspec.nr)
		trace2_cmd_منوال("path");
	ورنہ
		trace2_cmd_منوال(reset_type_نام[reset_type]);

	اگر (reset_type != SOFT && (reset_type != MIXED || repo_get_work_درخت(the_repository)))
		تیاری_work_درخت(the_repository);

	اگر (reset_type == MIXED && is_bare_repository(the_repository))
		die(_("%s reset is not allowed in a bare repository"),
		    _(reset_type_نام[reset_type]));

	اگر (intent_مقصد_جوڑو && reset_type != MIXED)
		die(_("the option '%s' requires '%s'"), "-N", "--mixed");

	اگر (repo_read_اشاریہ(the_repository) < 0)
		die(_("index file corrupt"));

	/* Soft reset does not touch the index file nor the working tree
	 * at all, but requires them in a good order.  Other resets reset
	 * the index file to the tree object we are switching to. */
	اگر (reset_type == SOFT || reset_type == KEEP)
		die_if_unmerged_cache(reset_type);

	اگر (reset_type != SOFT) {
		ساخت تالا_file تالا = تالا_INIT;
		repo_hold_locked_اشاریہ(the_repository, &تالا,
				       تالا_DIE_ON_ERROR);
		اگر (reset_type == MIXED) {
			صحیح flags = quiet ? REFRESH_QUIET : REFRESH_اندر_PORCELAIN;
			اگر (read_from_درخت(&pathspec, &oid, intent_مقصد_جوڑو)) {
				update_ref_حالت = 1;
				جاؤ cleanup;
			}
			the_repository->اشاریہ->updated_skipworktree = 1;
			اگر (!no_refresh && repo_get_work_درخت(the_repository)) {
				uint64_t t_begin, t_delta_اندر_ms;

				t_begin = getnanotime();
				refresh_اشاریہ(the_repository->اشاریہ, flags, تہی, تہی,
					      _("Unstaged changes after reset:"));
				t_delta_اندر_ms = (getnanotime() - t_begin) / 1000000;
				اگر (!quiet && advice_enabled(ADVICE_RESET_NO_REFRESH_WARNING) && t_delta_اندر_ms > REFRESH_اشاریہ_DELAY_WARNING_اندر_MS) {
					advise(_("It took %.2f seconds to refresh the index after reset.  You can use\n"
						 "'--no-refresh' to avoid this."), t_delta_اندر_ms / 1000.0);
				}
			}
		} ورنہ {
			ساخت object_id dummy;
			حرف *ref = تہی;
			صحیح err;

			repo_dwim_ref(the_repository, rev, لڑی_لمبائی(rev),
				      &dummy, &ref, 0);
			اگر (ref && !starts_with(ref, "refs/"))
				FREE_AND_NULL(ref);

			err = reset_اشاریہ(ref, &oid, reset_type, quiet);
			اگر (reset_type == KEEP && !err)
				err = reset_اشاریہ(ref, &oid, MIXED, quiet);
			اگر (err)
				die(_("Could not reset index file to revision '%s'."), rev);
			آزاد(ref);
		}

		اگر (write_locked_اشاریہ(the_repository->اشاریہ, &تالا, COMMIT_تالا))
			die(_("Could not write new index file."));
	}

	اگر (!pathspec.nr && !unborn) {
		/* Any resets without paths update HEAD to the head being
		 * switched to, saving the previous head in ORIG_HEAD before. */
		update_ref_حالت = reset_refs(rev, &oid);

		اگر (reset_type == HARD && !update_ref_حالت && !quiet)
			print_new_سر_سطر(lookup_commit_reference(the_repository, &oid));
	}
	اگر (!pathspec.nr)
		remove_branch_state(the_repository, 0);

	discard_اشاریہ(the_repository->اشاریہ);

cleanup:
	clear_pathspec(&pathspec);
	آزاد(pathspec_from_file);
	واپس update_ref_حالت;
}
