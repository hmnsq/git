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

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "advice.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hash.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "lockfile.h"
#अन्तर्भाव "object.h"
#अन्तर्भाव "pretty.h"
#अन्तर्भाव "refs.h"
#अन्तर्भाव "diff.h"
#अन्तर्भाव "diffcore.h"
#अन्तर्भाव "tree.h"
#अन्तर्भाव "branch.h"
#अन्तर्भाव "object-name.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "path.h"
#अन्तर्भाव "repository.h"
#अन्तर्भाव "unpack-trees.h"
#अन्तर्भाव "cache-tree.h"
#अन्तर्भाव "setup.h"
#अन्तर्भाव "sparse-index.h"
#अन्तर्भाव "submodule.h"
#अन्तर्भाव "trace.h"
#अन्तर्भाव "trace2.h"
#अन्तर्भाव "dir.h"
#अन्तर्भाव "add-interactive.h"

#define REFRESH_INDEX_DELAY_WARNING_IN_MS (2 * 1000)

स्थिर ध्रुव वर्ण * ध्रुव git_reset_usage[] = {
	N_("git reset [--mixed | --soft | --hard | --merge | --keep] [-q] [<commit>]"),
	N_("git reset [-q] [<tree-ish>] [--] <pathspec>..."),
	N_("git reset [-q] [--pathspec-from-file [--pathspec-file-nul]] [<tree-ish>]"),
	N_("git reset --patch [<tree-ish>] [--] [<pathspec>...]"),
	रिक्त
};

गणना reset_type { MIXED, SOFT, HARD, MERGE, KEEP, NONE };
स्थिर ध्रुव वर्ण *reset_type_names[] = {
	N_("mixed"), N_("soft"), N_("hard"), N_("merge"), N_("keep"), रिक्त
};

स्थिर अन्तःस्थ पूर्णाङ्क is_merge(शून्य)
{
	प्रत्यागम !access(git_path_merge_head(the_repository), F_OK);
}

स्थिर पूर्णाङ्क reset_index(ध्रुव वर्ण *ref, ध्रुव संरचना object_id *oid, पूर्णाङ्क reset_type, पूर्णाङ्क quiet)
{
	पूर्णाङ्क i, nr = 0;
	संरचना वृक्ष_desc desc[2];
	संरचना वृक्ष *वृक्ष;
	संरचना unpack_trees_options opts;
	पूर्णाङ्क ret = -1;

	स_रखो(&opts, 0, परिमाण(opts));
	opts.head_idx = 1;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;
	opts.fn = oneway_merge;
	opts.merge = 1;
	init_checkout_metadata(&opts.meta, ref, oid, रिक्त);
	यदि (!quiet)
		opts.verbose_update = 1;
	विकल्प (reset_type) {
	पक्ष KEEP:
	पक्ष MERGE:
		opts.update = 1;
		opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
		भङ्ग;
	पक्ष HARD:
		opts.update = 1;
		opts.reset = UNPACK_RESET_OVERWRITE_UNTRACKED;
		opts.skip_cache_वृक्ष_update = 1;
		भङ्ग;
	पक्ष MIXED:
		opts.reset = UNPACK_RESET_PROTECT_UNTRACKED;
		opts.skip_cache_वृक्ष_update = 1;
		/* but opts.update=0, so working tree not updated */
		भङ्ग;
	स्वभाव:
		BUG("invalid reset_type passed to reset_index");
	}

	repo_read_index_unmerged(the_repository);

	यदि (reset_type == KEEP) {
		संरचना object_id head_oid;
		यदि (repo_get_oid(the_repository, "HEAD", &head_oid))
			प्रत्यागम error(_("You do not have a valid HEAD."));
		यदि (!fill_वृक्ष_descriptor(the_repository, desc + nr, &head_oid))
			प्रत्यागम error(_("Failed to find tree of HEAD."));
		nr++;
		opts.fn = twoway_merge;
	}

	यदि (!fill_वृक्ष_descriptor(the_repository, desc + nr, oid)) {
		error(_("Failed to find tree of %s."), oid_to_hex(oid));
		गच्छ out;
	}
	nr++;

	यदि (unpack_trees(nr, desc, &opts))
		गच्छ out;

	यदि (reset_type == MIXED || reset_type == HARD) {
		वृक्ष = repo_parse_वृक्ष_indirect(the_repository, oid);
		यदि (!वृक्ष) {
			error(_("unable to read tree (%s)"), oid_to_hex(oid));
			गच्छ out;
		}
		अभाज्य_cache_वृक्ष(the_repository, the_repository->index, वृक्ष);
	}

	ret = 0;

out:
	प्रति (i = 0; i < nr; i++)
		मुक्त((शून्य *)desc[i].buffer);
	प्रत्यागम ret;
}

स्थिर शून्य print_new_head_पंक्ति(संरचना commit *commit)
{
	संरचना strbuf buf = STRBUF_INIT;

	म_लिखो(_("HEAD is now at %s"),
		repo_find_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV));

	pp_commit_easy(CMIT_FMT_ONELINE, commit, &buf);
	यदि (buf.len > 0)
		म_लिखो(" %s", buf.buf);
	अक्षर_दो('\n');
	strbuf_release(&buf);
}

स्थिर शून्य update_index_from_diff(संरचना diff_queue_struct *q,
				   संरचना diff_options *opt UNUSED,
				   शून्य *data)
{
	पूर्णाङ्क i;
	पूर्णाङ्क intent_to_add = *(पूर्णाङ्क *)data;

	प्रति (i = 0; i < q->nr; i++) {
		पूर्णाङ्क pos;
		संरचना diff_filespec *one = q->queue[i]->one;
		पूर्णाङ्क is_in_reset_वृक्ष = one->mode && !is_null_oid(&one->oid);
		संरचना cache_entry *ce;

		यदि (!is_in_reset_वृक्ष && !intent_to_add) {
			remove_file_from_index(the_repository->index, one->path);
			अनुवर्त;
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
		pos = index_name_pos(the_repository->index, one->path, म_माप(one->path));
		यदि ((pos >= 0 && ce_skip_worktree(the_repository->index->cache[pos])) ||
		    (pos < 0 && !path_in_sparse_checkout(one->path, the_repository->index)))
			ce->ce_flags |= CE_SKIP_WORKTREE;

		यदि (!ce)
			die(_("make_cache_entry failed for path '%s'"),
			    one->path);
		यदि (!is_in_reset_वृक्ष) {
			ce->ce_flags |= CE_INTENT_TO_ADD;
			set_object_name_for_intent_to_add_entry(ce);
		}
		add_index_entry(the_repository->index, ce,
				ADD_CACHE_OK_TO_ADD | ADD_CACHE_OK_TO_REPLACE);
	}
}

स्थिर पूर्णाङ्क read_from_वृक्ष(ध्रुव संरचना pathspec *pathspec,
			  संरचना object_id *वृक्ष_oid,
			  पूर्णाङ्क intent_to_add)
{
	संरचना diff_options opt;

	स_रखो(&opt, 0, परिमाण(opt));
	copy_pathspec(&opt.pathspec, pathspec);
	opt.output_format = DIFF_FORMAT_CALLBACK;
	opt.format_callback = update_index_from_diff;
	opt.format_callback_data = &intent_to_add;
	opt.flags.override_submodule_config = 1;
	opt.flags.recursive = 1;
	opt.repo = the_repository;
	opt.change = diff_change;
	opt.add_remove = diff_addremove;

	यदि (pathspec->nr && pathspec_needs_expanded_index(the_repository->index, pathspec))
		ensure_full_index(the_repository->index);

	यदि (do_diff_cache(वृक्ष_oid, &opt))
		प्रत्यागम 1;
	diffcore_std(&opt);
	diff_flush(&opt);

	प्रत्यागम 0;
}

स्थिर शून्य set_reflog_message(संरचना strbuf *sb, ध्रुव वर्ण *action,
			       ध्रुव वर्ण *rev)
{
	ध्रुव वर्ण *rla = दो_पर्या("GIT_REFLOG_ACTION");

	strbuf_reset(sb);
	यदि (rla)
		strbuf_addf(sb, "%s: %s", rla, action);
	अन्यथा यदि (rev)
		strbuf_addf(sb, "reset: moving to %s", rev);
	अन्यथा
		strbuf_addf(sb, "reset: %s", action);
}

स्थिर शून्य die_if_unmerged_cache(पूर्णाङ्क reset_type)
{
	यदि (is_merge() || unmerged_index(the_repository->index))
		die(_("Cannot do a %s reset in the middle of a merge."),
		    _(reset_type_names[reset_type]));

}

स्थिर शून्य parse_args(संरचना pathspec *pathspec,
		       ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix,
		       पूर्णाङ्क patch_mode,
		       ध्रुव वर्ण **rev_ret)
{
	ध्रुव वर्ण *rev = "HEAD";
	संरचना object_id unused;
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

	यदि (argv[0]) {
		यदि (!म_भेद(argv[0], "--")) {
			argv++; /* reset to HEAD, possibly with paths */
		} अन्यथा यदि (argv[1] && !म_भेद(argv[1], "--")) {
			rev = argv[0];
			argv += 2;
		}
		/*
		 * Otherwise, argv[0] could be either <rev> or <paths> and
		 * has to be unambiguous. If there is a single argument, it
		 * can not be a tree
		 */
		अन्यथा यदि ((!argv[1] && !repo_get_oid_committish(the_repository, argv[0], &unused)) ||
			 (argv[1] && !repo_get_oid_treeish(the_repository, argv[0], &unused))) {
			/*
			 * Ok, argv[0] looks like a commit/tree; it should not
			 * be a filename.
			 */
			verify_non_filename(the_repository, prefix, argv[0]);
			rev = *argv++;
		} अन्यथा {
			/* Otherwise we treat this as a filename */
			verify_filename(the_repository, prefix, argv[0], 1);
		}
	}

	/* treat '@' as a shortcut for 'HEAD' */
	*rev_ret = !म_भेद("@", rev) ? "HEAD" : rev;

	parse_pathspec(pathspec, 0,
		       PATHSPEC_PREFER_FULL |
		       (patch_mode ? PATHSPEC_PREFIX_ORIGIN : 0),
		       prefix, argv);
}

स्थिर पूर्णाङ्क reset_refs(ध्रुव वर्ण *rev, ध्रुव संरचना object_id *oid)
{
	पूर्णाङ्क update_ref_status;
	संरचना strbuf संदेश = STRBUF_INIT;
	संरचना object_id *orig = रिक्त, oid_orig,
		*old_orig = रिक्त, oid_old_orig;

	यदि (!repo_get_oid(the_repository, "ORIG_HEAD", &oid_old_orig))
		old_orig = &oid_old_orig;
	यदि (!repo_get_oid(the_repository, "HEAD", &oid_orig)) {
		orig = &oid_orig;
		set_reflog_message(&संदेश, "updating ORIG_HEAD", रिक्त);
		refs_update_ref(get_main_ref_store(the_repository), संदेश.buf,
				"ORIG_HEAD", orig, old_orig, 0,
				UPDATE_REFS_संदेश_ON_ERR);
	} अन्यथा यदि (old_orig)
		refs_delete_ref(get_main_ref_store(the_repository), रिक्त,
				"ORIG_HEAD", old_orig, 0);
	set_reflog_message(&संदेश, "updating HEAD", rev);
	update_ref_status = refs_update_ref(get_main_ref_store(the_repository),
					    संदेश.buf, "HEAD", oid, orig, 0,
					    UPDATE_REFS_संदेश_ON_ERR);
	strbuf_release(&संदेश);
	प्रत्यागम update_ref_status;
}

स्थिर पूर्णाङ्क git_reset_config(ध्रुव वर्ण *var, ध्रुव वर्ण *मूल्य,
			    ध्रुव संरचना config_context *ctx, शून्य *cb)
{
	यदि (!म_भेद(var, "submodule.recurse"))
		प्रत्यागम git_default_submodule_config(var, मूल्य, cb);

	प्रत्यागम git_default_config(var, मूल्य, ctx, cb);
}

पूर्णाङ्क cmd_reset(पूर्णाङ्क argc,
	      ध्रुव वर्ण **argv,
	      ध्रुव वर्ण *prefix,
	      संरचना repository *repo UNUSED)
{
	पूर्णाङ्क reset_type = NONE, update_ref_status = 0, quiet = 0;
	पूर्णाङ्क no_refresh = 0;
	पूर्णाङ्क patch_mode = 0, pathspec_file_nul = 0, unborn;
	ध्रुव वर्ण *rev;
	वर्ण *pathspec_from_file = रिक्त;
	संरचना object_id oid;
	संरचना pathspec pathspec;
	पूर्णाङ्क intent_to_add = 0;
	संरचना interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	ध्रुव संरचना option options[] = {
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
		OPT_CALLBACK_F(0, "recurse-submodules", रिक्त,
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

	repo_config(the_repository, git_reset_config, रिक्त);

	argc = parse_options(argc, argv, prefix, options, git_reset_usage,
						PARSE_OPT_KEEP_DASHDASH);
	parse_args(&pathspec, argv, prefix, patch_mode, &rev);

	यदि (pathspec_from_file) {
		यदि (patch_mode)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--patch");

		यदि (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_FULL,
				    prefix, pathspec_from_file, pathspec_file_nul);
	} अन्यथा यदि (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	unborn = !म_भेद(rev, "HEAD") && repo_get_oid(the_repository, "HEAD",
						      &oid);
	यदि (unborn) {
		/* reset on unborn branch: treat as reset to empty tree */
		oidcpy(&oid, the_hash_algo->empty_वृक्ष);
	} अन्यथा यदि (!pathspec.nr && !patch_mode) {
		संरचना commit *commit;
		यदि (repo_get_oid_committish(the_repository, rev, &oid))
			die(_("Failed to resolve '%s' as a valid revision."), rev);
		commit = lookup_commit_reference(the_repository, &oid);
		यदि (!commit)
			die(_("Could not parse object '%s'."), rev);
		oidcpy(&oid, &commit->object.oid);
	} अन्यथा {
		संरचना वृक्ष *वृक्ष;
		यदि (repo_get_oid_treeish(the_repository, rev, &oid))
			die(_("Failed to resolve '%s' as a valid tree."), rev);
		वृक्ष = repo_parse_वृक्ष_indirect(the_repository, &oid);
		यदि (!वृक्ष)
			die(_("Could not parse object '%s'."), rev);
		oidcpy(&oid, &वृक्ष->object.oid);
	}

	यदि (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	यदि (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	यदि (patch_mode) {
		यदि (reset_type != NONE)
			die(_("options '%s' and '%s' cannot be used together"), "--patch", "--{hard,mixed,soft}");
		trace2_cmd_mode("patch-interactive");
		update_ref_status = !!run_add_p(the_repository, ADD_P_RESET,
						&interactive_opts, rev, &pathspec, 0);
		गच्छ cleanup;
	} अन्यथा {
		यदि (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--patch");
		यदि (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--patch");
		यदि (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--patch");
	}

	/* git reset tree [--] paths... can be used to
	 * load chosen paths from the tree into the index without
	 * affecting the working tree nor HEAD. */
	यदि (pathspec.nr) {
		यदि (reset_type == MIXED)
			warning(_("--mixed with paths is deprecated; use 'git reset -- <paths>' instead."));
		अन्यथा यदि (reset_type != NONE)
			die(_("Cannot do %s reset with paths."),
					_(reset_type_names[reset_type]));
	}
	यदि (reset_type == NONE)
		reset_type = MIXED; /* by default */

	यदि (pathspec.nr)
		trace2_cmd_mode("path");
	अन्यथा
		trace2_cmd_mode(reset_type_names[reset_type]);

	यदि (reset_type != SOFT && (reset_type != MIXED || repo_get_work_वृक्ष(the_repository)))
		setup_work_वृक्ष(the_repository);

	यदि (reset_type == MIXED && is_bare_repository(the_repository))
		die(_("%s reset is not allowed in a bare repository"),
		    _(reset_type_names[reset_type]));

	यदि (intent_to_add && reset_type != MIXED)
		die(_("the option '%s' requires '%s'"), "-N", "--mixed");

	यदि (repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));

	/* Soft reset does not touch the index file nor the working tree
	 * at all, but requires them in a good order.  Other resets reset
	 * the index file to the tree object we are switching to. */
	यदि (reset_type == SOFT || reset_type == KEEP)
		die_if_unmerged_cache(reset_type);

	यदि (reset_type != SOFT) {
		संरचना lock_file lock = LOCK_INIT;
		repo_hold_locked_index(the_repository, &lock,
				       LOCK_DIE_ON_ERROR);
		यदि (reset_type == MIXED) {
			पूर्णाङ्क flags = quiet ? REFRESH_QUIET : REFRESH_IN_PORCELAIN;
			यदि (read_from_वृक्ष(&pathspec, &oid, intent_to_add)) {
				update_ref_status = 1;
				गच्छ cleanup;
			}
			the_repository->index->updated_skipworktree = 1;
			यदि (!no_refresh && repo_get_work_वृक्ष(the_repository)) {
				uint64_t t_begin, t_delta_in_ms;

				t_begin = getnanotime();
				refresh_index(the_repository->index, flags, रिक्त, रिक्त,
					      _("Unstaged changes after reset:"));
				t_delta_in_ms = (getnanotime() - t_begin) / 1000000;
				यदि (!quiet && advice_enabled(ADVICE_RESET_NO_REFRESH_WARNING) && t_delta_in_ms > REFRESH_INDEX_DELAY_WARNING_IN_MS) {
					advise(_("It took %.2f seconds to refresh the index after reset.  You can use\n"
						 "'--no-refresh' to avoid this."), t_delta_in_ms / 1000.0);
				}
			}
		} अन्यथा {
			संरचना object_id dummy;
			वर्ण *ref = रिक्त;
			पूर्णाङ्क err;

			repo_dwim_ref(the_repository, rev, म_माप(rev),
				      &dummy, &ref, 0);
			यदि (ref && !starts_with(ref, "refs/"))
				FREE_AND_NULL(ref);

			err = reset_index(ref, &oid, reset_type, quiet);
			यदि (reset_type == KEEP && !err)
				err = reset_index(ref, &oid, MIXED, quiet);
			यदि (err)
				die(_("Could not reset index file to revision '%s'."), rev);
			मुक्त(ref);
		}

		यदि (write_locked_index(the_repository->index, &lock, COMMIT_LOCK))
			die(_("Could not write new index file."));
	}

	यदि (!pathspec.nr && !unborn) {
		/* Any resets without paths update HEAD to the head being
		 * switched to, saving the previous head in ORIG_HEAD before. */
		update_ref_status = reset_refs(rev, &oid);

		यदि (reset_type == HARD && !update_ref_status && !quiet)
			print_new_head_पंक्ति(lookup_commit_reference(the_repository, &oid));
	}
	यदि (!pathspec.nr)
		remove_branch_state(the_repository, 0);

	discard_index(the_repository->index);

cleanup:
	clear_pathspec(&pathspec);
	मुक्त(pathspec_from_file);
	प्रत्यागम update_ref_status;
}
