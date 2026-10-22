/*
 * "git reset" builtin command
 *
 * Copyright (c) 2007 Carlos Rica
 *
 * Based on git-reset.sh, which is
 *
 * Copyright (c) 2005, 2006 Linus Torvalds and Junio C Hamano
 */

#घोषणा USE_THE_REPOSITORY_VARIABLE

#समावेश "builtin.h"
#समावेश "advice.h"
#समावेश "config.h"
#समावेश "environment.h"
#समावेश "gettext.h"
#समावेश "hash.h"
#समावेश "hex.h"
#समावेश "lockfile.h"
#समावेश "object.h"
#समावेश "pretty.h"
#समावेश "refs.h"
#समावेश "diff.h"
#समावेश "diffcore.h"
#समावेश "tree.h"
#समावेश "branch.h"
#समावेश "object-name.h"
#समावेश "parse-options.h"
#समावेश "path.h"
#समावेश "repository.h"
#समावेश "unpack-trees.h"
#समावेश "cache-tree.h"
#समावेश "setup.h"
#समावेश "sparse-index.h"
#समावेश "submodule.h"
#समावेश "trace.h"
#समावेश "trace2.h"
#समावेश "dir.h"
#समावेश "add-interactive.h"

#घोषणा REFRESH_सूचकांक_DELAY_WARNING_IN_MS (2 * 1000)

अटल स्थिर अक्षर * स्थिर git_reset_usage[] = {
	N_("git reset [--mixed | --soft | --hard | --merge | --keep] [-q] [<commit>]"),
	N_("git reset [-q] [<tree-ish>] [--] <pathspec>..."),
	N_("git reset [-q] [--pathspec-from-file [--pathspec-file-nul]] [<tree-ish>]"),
	N_("git reset --patch [<tree-ish>] [--] [<pathspec>...]"),
	शून्य
};

क्रमागत reset_type { MIXED, SOFT, HARD, MERGE, KEEP, NONE };
अटल स्थिर अक्षर *reset_type_names[] = {
	N_("mixed"), N_("soft"), N_("hard"), N_("merge"), N_("keep"), शून्य
};

अटल अंतरभूत पूर्णांक is_merge(व्योम)
{
	वापस !access(git_path_merge_शीर्ष(the_repository), F_OK);
}

अटल पूर्णांक reset_सूचकांक(स्थिर अक्षर *ref, स्थिर काष्ठा object_id *oid, पूर्णांक reset_type, पूर्णांक quiet)
{
	पूर्णांक i, nr = 0;
	काष्ठा वृक्ष_desc desc[2];
	काष्ठा वृक्ष *वृक्ष;
	काष्ठा unpack_trees_options opts;
	पूर्णांक ret = -1;

	स_रखो(&opts, 0, माप(opts));
	opts.शीर्ष_idx = 1;
	opts.src_सूचकांक = the_repository->सूचकांक;
	opts.dst_सूचकांक = the_repository->सूचकांक;
	opts.fn = oneway_merge;
	opts.merge = 1;
	init_checkout_metadata(&opts.meta, ref, oid, शून्य);
	अगर (!quiet)
		opts.verbose_update = 1;
	चयन (reset_type) {
	हाल KEEP:
	हाल MERGE:
		opts.update = 1;
		opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
		अवरोध;
	हाल HARD:
		opts.update = 1;
		opts.reset = UNPACK_RESET_OVERWRITE_UNTRACKED;
		opts.skip_cache_वृक्ष_update = 1;
		अवरोध;
	हाल MIXED:
		opts.reset = UNPACK_RESET_PROTECT_UNTRACKED;
		opts.skip_cache_वृक्ष_update = 1;
		/* but opts.update=0, so working tree not updated */
		अवरोध;
	शेष:
		BUG("invalid reset_type passed to reset_index");
	}

	repo_read_सूचकांक_unmerged(the_repository);

	अगर (reset_type == KEEP) {
		काष्ठा object_id शीर्ष_oid;
		अगर (repo_get_oid(the_repository, "HEAD", &शीर्ष_oid))
			वापस error(_("You do not have a valid HEAD."));
		अगर (!fill_वृक्ष_descriptor(the_repository, desc + nr, &शीर्ष_oid))
			वापस error(_("Failed to find tree of HEAD."));
		nr++;
		opts.fn = twoway_merge;
	}

	अगर (!fill_वृक्ष_descriptor(the_repository, desc + nr, oid)) {
		error(_("Failed to find tree of %s."), oid_to_hex(oid));
		जाओ out;
	}
	nr++;

	अगर (unpack_trees(nr, desc, &opts))
		जाओ out;

	अगर (reset_type == MIXED || reset_type == HARD) {
		वृक्ष = repo_parse_वृक्ष_indirect(the_repository, oid);
		अगर (!वृक्ष) {
			error(_("unable to read tree (%s)"), oid_to_hex(oid));
			जाओ out;
		}
		अभाज्य_cache_वृक्ष(the_repository, the_repository->सूचकांक, वृक्ष);
	}

	ret = 0;

out:
	क्रम (i = 0; i < nr; i++)
		मुक्त((व्योम *)desc[i].बफर);
	वापस ret;
}

अटल व्योम print_new_शीर्ष_पंक्ति(काष्ठा commit *commit)
{
	काष्ठा strbuf buf = STRBUF_INIT;

	म_लिखो(_("HEAD is now at %s"),
		repo_find_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV));

	pp_commit_easy(CMIT_FMT_ONELINE, commit, &buf);
	अगर (buf.len > 0)
		म_लिखो(" %s", buf.buf);
	अक्षर_दो('\n');
	strbuf_release(&buf);
}

अटल व्योम update_सूचकांक_from_diff(काष्ठा diff_queue_struct *q,
				   काष्ठा diff_options *opt UNUSED,
				   व्योम *तथ्य)
{
	पूर्णांक i;
	पूर्णांक intent_to_add = *(पूर्णांक *)तथ्य;

	क्रम (i = 0; i < q->nr; i++) {
		पूर्णांक pos;
		काष्ठा diff_filespec *one = q->queue[i]->one;
		पूर्णांक is_in_reset_वृक्ष = one->mode && !is_null_oid(&one->oid);
		काष्ठा cache_entry *ce;

		अगर (!is_in_reset_वृक्ष && !intent_to_add) {
			remove_file_from_सूचकांक(the_repository->सूचकांक, one->path);
			जारी;
		}

		ce = make_cache_entry(the_repository->सूचकांक, one->mode, &one->oid, one->path,
				      0, 0);

		/*
		 * If the file 1) corresponds to an existing index entry with
		 * skip-worktree set, or 2) does not exist in the index but is
		 * outside the sparse checkout definition, add a skip-worktree bit
		 * to the new index entry. Note that a sparse index will be expanded
		 * if this entry is outside the sparse cone - this is necessary
		 * to properly construct the reset sparse directory.
		 */
		pos = सूचकांक_name_pos(the_repository->सूचकांक, one->path, म_माप(one->path));
		अगर ((pos >= 0 && ce_skip_worktree(the_repository->सूचकांक->cache[pos])) ||
		    (pos < 0 && !path_in_sparse_checkout(one->path, the_repository->सूचकांक)))
			ce->ce_flags |= CE_SKIP_WORKTREE;

		अगर (!ce)
			die(_("make_cache_entry failed for path '%s'"),
			    one->path);
		अगर (!is_in_reset_वृक्ष) {
			ce->ce_flags |= CE_INTENT_TO_ADD;
			set_object_name_for_intent_to_add_entry(ce);
		}
		add_सूचकांक_entry(the_repository->सूचकांक, ce,
				ADD_CACHE_OK_TO_ADD | ADD_CACHE_OK_TO_REPLACE);
	}
}

अटल पूर्णांक read_from_वृक्ष(स्थिर काष्ठा pathspec *pathspec,
			  काष्ठा object_id *वृक्ष_oid,
			  पूर्णांक intent_to_add)
{
	काष्ठा diff_options opt;

	स_रखो(&opt, 0, माप(opt));
	copy_pathspec(&opt.pathspec, pathspec);
	opt.output_format = DIFF_FORMAT_CALLBACK;
	opt.format_callback = update_सूचकांक_from_diff;
	opt.format_callback_तथ्य = &intent_to_add;
	opt.flags.override_submodule_config = 1;
	opt.flags.recursive = 1;
	opt.repo = the_repository;
	opt.change = diff_change;
	opt.add_remove = diff_addremove;

	अगर (pathspec->nr && pathspec_needs_expanded_सूचकांक(the_repository->सूचकांक, pathspec))
		ensure_full_सूचकांक(the_repository->सूचकांक);

	अगर (do_diff_cache(वृक्ष_oid, &opt))
		वापस 1;
	diffcore_std(&opt);
	diff_flush(&opt);

	वापस 0;
}

अटल व्योम set_reflog_message(काष्ठा strbuf *sb, स्थिर अक्षर *action,
			       स्थिर अक्षर *rev)
{
	स्थिर अक्षर *rla = दो_पर्या("GIT_REFLOG_ACTION");

	strbuf_reset(sb);
	अगर (rla)
		strbuf_addf(sb, "%s: %s", rla, action);
	अन्यथा अगर (rev)
		strbuf_addf(sb, "reset: moving to %s", rev);
	अन्यथा
		strbuf_addf(sb, "reset: %s", action);
}

अटल व्योम die_if_unmerged_cache(पूर्णांक reset_type)
{
	अगर (is_merge() || unmerged_सूचकांक(the_repository->सूचकांक))
		die(_("Cannot do a %s reset in the middle of a merge."),
		    _(reset_type_names[reset_type]));

}

अटल व्योम parse_args(काष्ठा pathspec *pathspec,
		       स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
		       पूर्णांक patch_mode,
		       स्थिर अक्षर **rev_ret)
{
	स्थिर अक्षर *rev = "HEAD";
	काष्ठा object_id unused;
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

	अगर (argv[0]) {
		अगर (!म_भेद(argv[0], "--")) {
			argv++; /* reset to HEAD, possibly with paths */
		} अन्यथा अगर (argv[1] && !म_भेद(argv[1], "--")) {
			rev = argv[0];
			argv += 2;
		}
		/*
		 * Otherwise, argv[0] could be either <rev> or <paths> and
		 * has to be unambiguous. If there is a single argument, it
		 * can not be a tree
		 */
		अन्यथा अगर ((!argv[1] && !repo_get_oid_committish(the_repository, argv[0], &unused)) ||
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

अटल पूर्णांक reset_refs(स्थिर अक्षर *rev, स्थिर काष्ठा object_id *oid)
{
	पूर्णांक update_ref_status;
	काष्ठा strbuf संदेश = STRBUF_INIT;
	काष्ठा object_id *orig = शून्य, oid_orig,
		*old_orig = शून्य, oid_old_orig;

	अगर (!repo_get_oid(the_repository, "ORIG_HEAD", &oid_old_orig))
		old_orig = &oid_old_orig;
	अगर (!repo_get_oid(the_repository, "HEAD", &oid_orig)) {
		orig = &oid_orig;
		set_reflog_message(&संदेश, "updating ORIG_HEAD", शून्य);
		refs_update_ref(get_main_ref_store(the_repository), संदेश.buf,
				"ORIG_HEAD", orig, old_orig, 0,
				UPDATE_REFS_संदेश_ON_ERR);
	} अन्यथा अगर (old_orig)
		refs_delete_ref(get_main_ref_store(the_repository), शून्य,
				"ORIG_HEAD", old_orig, 0);
	set_reflog_message(&संदेश, "updating HEAD", rev);
	update_ref_status = refs_update_ref(get_main_ref_store(the_repository),
					    संदेश.buf, "HEAD", oid, orig, 0,
					    UPDATE_REFS_संदेश_ON_ERR);
	strbuf_release(&संदेश);
	वापस update_ref_status;
}

अटल पूर्णांक git_reset_config(स्थिर अक्षर *var, स्थिर अक्षर *मूल्य,
			    स्थिर काष्ठा config_context *ctx, व्योम *cb)
{
	अगर (!म_भेद(var, "submodule.recurse"))
		वापस git_default_submodule_config(var, मूल्य, cb);

	वापस git_default_config(var, मूल्य, ctx, cb);
}

पूर्णांक cmd_reset(पूर्णांक argc,
	      स्थिर अक्षर **argv,
	      स्थिर अक्षर *prefix,
	      काष्ठा repository *repo UNUSED)
{
	पूर्णांक reset_type = NONE, update_ref_status = 0, quiet = 0;
	पूर्णांक no_refresh = 0;
	पूर्णांक patch_mode = 0, pathspec_file_nul = 0, unborn;
	स्थिर अक्षर *rev;
	अक्षर *pathspec_from_file = शून्य;
	काष्ठा object_id oid;
	काष्ठा pathspec pathspec;
	पूर्णांक intent_to_add = 0;
	काष्ठा interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	स्थिर काष्ठा option options[] = {
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
		OPT_CALLBACK_F(0, "recurse-submodules", शून्य,
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

	repo_config(the_repository, git_reset_config, शून्य);

	argc = parse_options(argc, argv, prefix, options, git_reset_usage,
						PARSE_OPT_KEEP_DASHDASH);
	parse_args(&pathspec, argv, prefix, patch_mode, &rev);

	अगर (pathspec_from_file) {
		अगर (patch_mode)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--patch");

		अगर (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_FULL,
				    prefix, pathspec_from_file, pathspec_file_nul);
	} अन्यथा अगर (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	unborn = !म_भेद(rev, "HEAD") && repo_get_oid(the_repository, "HEAD",
						      &oid);
	अगर (unborn) {
		/* reset on unborn branch: treat as reset to empty tree */
		oidcpy(&oid, the_hash_algo->empty_वृक्ष);
	} अन्यथा अगर (!pathspec.nr && !patch_mode) {
		काष्ठा commit *commit;
		अगर (repo_get_oid_committish(the_repository, rev, &oid))
			die(_("Failed to resolve '%s' as a valid revision."), rev);
		commit = lookup_commit_reference(the_repository, &oid);
		अगर (!commit)
			die(_("Could not parse object '%s'."), rev);
		oidcpy(&oid, &commit->object.oid);
	} अन्यथा {
		काष्ठा वृक्ष *वृक्ष;
		अगर (repo_get_oid_treeish(the_repository, rev, &oid))
			die(_("Failed to resolve '%s' as a valid tree."), rev);
		वृक्ष = repo_parse_वृक्ष_indirect(the_repository, &oid);
		अगर (!वृक्ष)
			die(_("Could not parse object '%s'."), rev);
		oidcpy(&oid, &वृक्ष->object.oid);
	}

	अगर (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	अगर (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_सूचकांक = 0;

	अगर (patch_mode) {
		अगर (reset_type != NONE)
			die(_("options '%s' and '%s' cannot be used together"), "--patch", "--{hard,mixed,soft}");
		trace2_cmd_mode("patch-interactive");
		update_ref_status = !!run_add_p(the_repository, ADD_P_RESET,
						&interactive_opts, rev, &pathspec, 0);
		जाओ cleanup;
	} अन्यथा {
		अगर (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--patch");
		अगर (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--patch");
		अगर (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--patch");
	}

	/* git reset tree [--] paths... can be used to
	 * load chosen paths from the tree into the index without
	 * affecting the working tree nor HEAD. */
	अगर (pathspec.nr) {
		अगर (reset_type == MIXED)
			warning(_("--mixed with paths is deprecated; use 'git reset -- <paths>' instead."));
		अन्यथा अगर (reset_type != NONE)
			die(_("Cannot do %s reset with paths."),
					_(reset_type_names[reset_type]));
	}
	अगर (reset_type == NONE)
		reset_type = MIXED; /* by default */

	अगर (pathspec.nr)
		trace2_cmd_mode("path");
	अन्यथा
		trace2_cmd_mode(reset_type_names[reset_type]);

	अगर (reset_type != SOFT && (reset_type != MIXED || repo_get_work_वृक्ष(the_repository)))
		setup_work_वृक्ष(the_repository);

	अगर (reset_type == MIXED && is_bare_repository(the_repository))
		die(_("%s reset is not allowed in a bare repository"),
		    _(reset_type_names[reset_type]));

	अगर (intent_to_add && reset_type != MIXED)
		die(_("the option '%s' requires '%s'"), "-N", "--mixed");

	अगर (repo_read_सूचकांक(the_repository) < 0)
		die(_("index file corrupt"));

	/* Soft reset does not touch the index file nor the working tree
	 * at all, but requires them in a good order.  Other resets reset
	 * the index file to the tree object we are switching to. */
	अगर (reset_type == SOFT || reset_type == KEEP)
		die_if_unmerged_cache(reset_type);

	अगर (reset_type != SOFT) {
		काष्ठा lock_file lock = LOCK_INIT;
		repo_hold_locked_सूचकांक(the_repository, &lock,
				       LOCK_DIE_ON_ERROR);
		अगर (reset_type == MIXED) {
			पूर्णांक flags = quiet ? REFRESH_QUIET : REFRESH_IN_PORCELAIN;
			अगर (read_from_वृक्ष(&pathspec, &oid, intent_to_add)) {
				update_ref_status = 1;
				जाओ cleanup;
			}
			the_repository->सूचकांक->updated_skipworktree = 1;
			अगर (!no_refresh && repo_get_work_वृक्ष(the_repository)) {
				uint64_t t_begin, t_delta_in_ms;

				t_begin = getnanotime();
				refresh_सूचकांक(the_repository->सूचकांक, flags, शून्य, शून्य,
					      _("Unstaged changes after reset:"));
				t_delta_in_ms = (getnanotime() - t_begin) / 1000000;
				अगर (!quiet && advice_enabled(ADVICE_RESET_NO_REFRESH_WARNING) && t_delta_in_ms > REFRESH_सूचकांक_DELAY_WARNING_IN_MS) {
					advise(_("It took %.2f seconds to refresh the index after reset.  You can use\n"
						 "'--no-refresh' to avoid this."), t_delta_in_ms / 1000.0);
				}
			}
		} अन्यथा {
			काष्ठा object_id dummy;
			अक्षर *ref = शून्य;
			पूर्णांक err;

			repo_dwim_ref(the_repository, rev, म_माप(rev),
				      &dummy, &ref, 0);
			अगर (ref && !starts_with(ref, "refs/"))
				FREE_AND_NULL(ref);

			err = reset_सूचकांक(ref, &oid, reset_type, quiet);
			अगर (reset_type == KEEP && !err)
				err = reset_सूचकांक(ref, &oid, MIXED, quiet);
			अगर (err)
				die(_("Could not reset index file to revision '%s'."), rev);
			मुक्त(ref);
		}

		अगर (write_locked_सूचकांक(the_repository->सूचकांक, &lock, COMMIT_LOCK))
			die(_("Could not write new index file."));
	}

	अगर (!pathspec.nr && !unborn) {
		/* Any resets without paths update HEAD to the head being
		 * switched to, saving the previous head in ORIG_HEAD before. */
		update_ref_status = reset_refs(rev, &oid);

		अगर (reset_type == HARD && !update_ref_status && !quiet)
			print_new_शीर्ष_पंक्ति(lookup_commit_reference(the_repository, &oid));
	}
	अगर (!pathspec.nr)
		remove_branch_state(the_repository, 0);

	discard_सूचकांक(the_repository->सूचकांक);

cleanup:
	clear_pathspec(&pathspec);
	मुक्त(pathspec_from_file);
	वापस update_ref_status;
}
