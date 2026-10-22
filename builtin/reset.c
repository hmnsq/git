/*
 * "git reset" builtin command
 *
 * Copyright (c) 2007 Carlos Rica
 *
 * Based on git-reset.sh, which is
 *
 * Copyright (c) 2005, 2006 Linus Torvalds and Junio C Hamano
 */

#परिभाषय USE_THE_REPOSITORY_VARIABLE

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

#परिभाषय REFRESH_सूचकांक_DELAY_WARNING_अन्तर्_MS (2 * 1000)

स्थिर ध्रुव वर्ण * ध्रुव git_reset_usage[] = {
	N_("git reset [--mixed | --soft | --hard | --merge | --keep] [-q] [<commit>]"),
	N_("git reset [-q] [<tree-ish>] [--] <pathspec>..."),
	N_("git reset [-q] [--pathspec-from-file [--pathspec-file-nul]] [<tree-ish>]"),
	N_("git reset --patch [<tree-ish>] [--] [<pathspec>...]"),
	रिक्त
};

गणना reset_type { MIXED, SOFT, HARD, MERGE, KEEP, NONE };
स्थिर ध्रुव वर्ण *reset_type_नामानि[] = {
	N_("mixed"), N_("soft"), N_("hard"), N_("merge"), N_("keep"), रिक्त
};

स्थिर अन्तःस्थ पूर्णाङ्क is_merge(शून्य)
{
	प्रत्यागम !access(git_मार्गः_merge_शीर्ष(the_repository), F_OK);
}

स्थिर पूर्णाङ्क reset_सूचकांक(ध्रुव वर्ण *ref, ध्रुव संरचना object_id *oid, पूर्णाङ्क reset_type, पूर्णाङ्क quiet)
{
	पूर्णाङ्क i, nr = 0;
	संरचना वृक्षः_desc desc[2];
	संरचना वृक्षः *वृक्षः;
	संरचना unpack_trees_options opts;
	पूर्णाङ्क ret = -1;

	स्मृतिं_पूरय(&opts, 0, परिमाण(opts));
	opts.शीर्ष_idx = 1;
	opts.src_सूचकांक = the_repository->सूचकांक;
	opts.dst_सूचकांक = the_repository->सूचकांक;
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
		opts.skip_cache_वृक्षः_update = 1;
		भङ्ग;
	पक्ष MIXED:
		opts.reset = UNPACK_RESET_PROTECT_UNTRACKED;
		opts.skip_cache_वृक्षः_update = 1;
		/* but opts.update=0, so working tree not updated */
		भङ्ग;
	स्वभाव:
		BUG("invalid reset_type passed to reset_index");
	}

	repo_read_सूचकांक_unmerged(the_repository);

	यदि (reset_type == KEEP) {
		संरचना object_id शीर्ष_oid;
		यदि (repo_get_oid(the_repository, "HEAD", &शीर्ष_oid))
			प्रत्यागम error(_("You do not have a valid HEAD."));
		यदि (!fill_वृक्षः_descriptor(the_repository, desc + nr, &शीर्ष_oid))
			प्रत्यागम error(_("Failed to find tree of HEAD."));
		nr++;
		opts.fn = twoway_merge;
	}

	यदि (!fill_वृक्षः_descriptor(the_repository, desc + nr, oid)) {
		error(_("Failed to find tree of %s."), oid_गन्तव्यम्_hex(oid));
		गच्छ बहिर्गमनम्;
	}
	nr++;

	यदि (unpack_trees(nr, desc, &opts))
		गच्छ बहिर्गमनम्;

	यदि (reset_type == MIXED || reset_type == HARD) {
		वृक्षः = repo_parse_वृक्षः_परोक्षम्(the_repository, oid);
		यदि (!वृक्षः) {
			error(_("unable to read tree (%s)"), oid_गन्तव्यम्_hex(oid));
			गच्छ बहिर्गमनम्;
		}
		अविभाज्यः_cache_वृक्षः(the_repository, the_repository->सूचकांक, वृक्षः);
	}

	ret = 0;

बहिर्गमनम्:
	प्रति (i = 0; i < nr; i++)
		मुक्त((शून्य *)desc[i].बफर);
	प्रत्यागम ret;
}

स्थिर शून्य print_new_शीर्ष_पङ्क्तिः(संरचना commit *commit)
{
	संरचना strbuf buf = STRBUF_INIT;

	प्रारूप_लिख(_("HEAD is now at %s"),
		repo_find_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV));

	pp_commit_easy(CMIT_FMT_ONELINE, commit, &buf);
	यदि (buf.len > 0)
		प्रारूप_लिख(" %s", buf.buf);
	अक्षरं_लिख('\n');
	strbuf_release(&buf);
}

स्थिर शून्य update_सूचकांक_from_diff(संरचना diff_queue_struct *q,
				   संरचना diff_options *opt अप्रयुक्तम्,
				   शून्य *दत्त)
{
	पूर्णाङ्क i;
	पूर्णाङ्क intent_गन्तव्यम्_सङ्कलय = *(पूर्णाङ्क *)दत्त;

	प्रति (i = 0; i < q->nr; i++) {
		पूर्णाङ्क pos;
		संरचना diff_filespec *one = q->queue[i]->one;
		पूर्णाङ्क is_अन्तर्_reset_वृक्षः = one->बहुलकम् && !is_null_oid(&one->oid);
		संरचना cache_प्रविष्टिः *ce;

		यदि (!is_अन्तर्_reset_वृक्षः && !intent_गन्तव्यम्_सङ्कलय) {
			remove_file_from_सूचकांक(the_repository->सूचकांक, one->मार्गः);
			अनुवर्त;
		}

		ce = make_cache_प्रविष्टिः(the_repository->सूचकांक, one->बहुलकम्, &one->oid, one->मार्गः,
				      0, 0);

		/*
		 * If the file 1) corresponds to an existing index entry with
		 * skip-worktree set, or 2) does not exist in the index but is
		 * outside the sparse checkout definition, add a skip-worktree bit
		 * to the new index entry. Note that a sparse index will be expanded
		 * if this entry is outside the sparse cone - this is necessary
		 * to properly construct the reset sparse directory.
		 */
		pos = सूचकांक_name_pos(the_repository->सूचकांक, one->मार्गः, वाक्यदैर्घ्यम्(one->मार्गः));
		यदि ((pos >= 0 && ce_skip_worktree(the_repository->सूचकांक->cache[pos])) ||
		    (pos < 0 && !मार्गः_अन्तर्_sparse_checkout(one->मार्गः, the_repository->सूचकांक)))
			ce->ce_flags |= CE_SKIP_WORKTREE;

		यदि (!ce)
			die(_("make_cache_entry failed for path '%s'"),
			    one->मार्गः);
		यदि (!is_अन्तर्_reset_वृक्षः) {
			ce->ce_flags |= CE_INTENT_गन्तव्यम्_सङ्कलय;
			समुच्चयः_object_name_for_intent_गन्तव्यम्_सङ्कलय_प्रविष्टिः(ce);
		}
		सङ्कलय_सूचकांक_प्रविष्टिः(the_repository->सूचकांक, ce,
				सङ्कलय_CACHE_OK_गन्तव्यम्_सङ्कलय | सङ्कलय_CACHE_OK_गन्तव्यम्_REPLACE);
	}
}

स्थिर पूर्णाङ्क read_from_वृक्षः(ध्रुव संरचना pathspec *pathspec,
			  संरचना object_id *वृक्षः_oid,
			  पूर्णाङ्क intent_गन्तव्यम्_सङ्कलय)
{
	संरचना diff_options opt;

	स्मृतिं_पूरय(&opt, 0, परिमाण(opt));
	copy_pathspec(&opt.pathspec, pathspec);
	opt.output_format = DIFF_FORMAT_CALLBACK;
	opt.format_callback = update_सूचकांक_from_diff;
	opt.format_callback_दत्त = &intent_गन्तव्यम्_सङ्कलय;
	opt.flags.override_submodule_config = 1;
	opt.flags.recursive = 1;
	opt.repo = the_repository;
	opt.change = diff_change;
	opt.सङ्कलय_remove = diff_addremove;

	यदि (pathspec->nr && pathspec_needs_expanded_सूचकांक(the_repository->सूचकांक, pathspec))
		ensure_पूर्णम्_सूचकांक(the_repository->सूचकांक);

	यदि (do_diff_cache(वृक्षः_oid, &opt))
		प्रत्यागम 1;
	diffcore_std(&opt);
	diff_flush(&opt);

	प्रत्यागम 0;
}

स्थिर शून्य समुच्चयः_reflog_सन्देशः(संरचना strbuf *sb, ध्रुव वर्ण *action,
			       ध्रुव वर्ण *rev)
{
	ध्रुव वर्ण *rla = getenv("GIT_REFLOG_ACTION");

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
	यदि (is_merge() || unmerged_सूचकांक(the_repository->सूचकांक))
		die(_("Cannot do a %s reset in the middle of a merge."),
		    _(reset_type_नामानि[reset_type]));

}

स्थिर शून्य parse_तर्काः(संरचना pathspec *pathspec,
		       ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
		       पूर्णाङ्क patch_बहुलकम्,
		       ध्रुव वर्ण **rev_ret)
{
	ध्रुव वर्ण *rev = "HEAD";
	संरचना object_id अप्रयुक्तम्_2;
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
		यदि (!वाक्यं_तुलय(argv[0], "--")) {
			argv++; /* reset to HEAD, possibly with paths */
		} अन्यथा यदि (argv[1] && !वाक्यं_तुलय(argv[1], "--")) {
			rev = argv[0];
			argv += 2;
		}
		/*
		 * Otherwise, argv[0] could be either <rev> or <paths> and
		 * has to be unambiguous. If there is a single argument, it
		 * can not be a tree
		 */
		अन्यथा यदि ((!argv[1] && !repo_get_oid_committish(the_repository, argv[0], &अप्रयुक्तम्_2)) ||
			 (argv[1] && !repo_get_oid_treeish(the_repository, argv[0], &अप्रयुक्तम्_2))) {
			/*
			 * Ok, argv[0] looks like a commit/tree; it should not
			 * be a filename.
			 */
			verify_non_filename(the_repository, उपसर्गः, argv[0]);
			rev = *argv++;
		} अन्यथा {
			/* Otherwise we treat this as a filename */
			verify_filename(the_repository, उपसर्गः, argv[0], 1);
		}
	}

	/* treat '@' as a shortcut for 'HEAD' */
	*rev_ret = !वाक्यं_तुलय("@", rev) ? "HEAD" : rev;

	parse_pathspec(pathspec, 0,
		       PATHSPEC_PREFER_पूर्णम् |
		       (patch_बहुलकम् ? PATHSPEC_उपसर्गः_ORIGIN : 0),
		       उपसर्गः, argv);
}

स्थिर पूर्णाङ्क reset_refs(ध्रुव वर्ण *rev, ध्रुव संरचना object_id *oid)
{
	पूर्णाङ्क update_ref_स्थितिः;
	संरचना strbuf सन्देशः = STRBUF_INIT;
	संरचना object_id *orig = रिक्त, oid_orig,
		*old_orig = रिक्त, oid_old_orig;

	यदि (!repo_get_oid(the_repository, "ORIG_HEAD", &oid_old_orig))
		old_orig = &oid_old_orig;
	यदि (!repo_get_oid(the_repository, "HEAD", &oid_orig)) {
		orig = &oid_orig;
		समुच्चयः_reflog_सन्देशः(&सन्देशः, "updating ORIG_HEAD", रिक्त);
		refs_update_ref(get_main_ref_store(the_repository), सन्देशः.buf,
				"ORIG_HEAD", orig, old_orig, 0,
				UPDATE_REFS_सन्देशः_ON_ERR);
	} अन्यथा यदि (old_orig)
		refs_delete_ref(get_main_ref_store(the_repository), रिक्त,
				"ORIG_HEAD", old_orig, 0);
	समुच्चयः_reflog_सन्देशः(&सन्देशः, "updating HEAD", rev);
	update_ref_स्थितिः = refs_update_ref(get_main_ref_store(the_repository),
					    सन्देशः.buf, "HEAD", oid, orig, 0,
					    UPDATE_REFS_सन्देशः_ON_ERR);
	strbuf_release(&सन्देशः);
	प्रत्यागम update_ref_स्थितिः;
}

स्थिर पूर्णाङ्क git_reset_config(ध्रुव वर्ण *var, ध्रुव वर्ण *मानम्,
			    ध्रुव संरचना config_context *ctx, शून्य *cb)
{
	यदि (!वाक्यं_तुलय(var, "submodule.recurse"))
		प्रत्यागम git_default_submodule_config(var, मानम्, cb);

	प्रत्यागम git_default_config(var, मानम्, ctx, cb);
}

पूर्णाङ्क cmd_reset(पूर्णाङ्क argc,
	      ध्रुव वर्ण **argv,
	      ध्रुव वर्ण *उपसर्गः,
	      संरचना repository *repo अप्रयुक्तम्)
{
	पूर्णाङ्क reset_type = NONE, update_ref_स्थितिः = 0, quiet = 0;
	पूर्णाङ्क no_refresh = 0;
	पूर्णाङ्क patch_बहुलकम् = 0, pathspec_file_nul = 0, unborn;
	ध्रुव वर्ण *rev;
	वर्ण *pathspec_from_file = रिक्त;
	संरचना object_id oid;
	संरचना pathspec pathspec;
	पूर्णाङ्क intent_गन्तव्यम्_सङ्कलय = 0;
	संरचना interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	ध्रुव संरचना option options[] = {
		OPT__QUIET(&quiet, N_("be quiet, only report errors")),
		OPT_BOOL(0, "no-refresh", &no_refresh,
				N_("skip refreshing the index after reset")),
		OPT_समुच्चयः_INT_F(0, "mixed", &reset_type,
			      N_("reset HEAD and index"),
			      MIXED, PARSE_OPT_NONEG),
		OPT_समुच्चयः_INT_F(0, "soft", &reset_type,
			      N_("reset only HEAD"),
			      SOFT, PARSE_OPT_NONEG),
		OPT_समुच्चयः_INT_F(0, "hard", &reset_type,
			      N_("reset HEAD, index and working tree"),
			      HARD, PARSE_OPT_NONEG),
		OPT_समुच्चयः_INT_F(0, "merge", &reset_type,
			      N_("reset HEAD, index and working tree"),
			      MERGE, PARSE_OPT_NONEG),
		OPT_समुच्चयः_INT_F(0, "keep", &reset_type,
			      N_("reset HEAD but keep local changes"),
			      KEEP, PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "recurse-submodules", रिक्त,
			       "reset", "control recursive updating of submodules",
			       PARSE_OPT_OPTARG,
			       option_parse_recurse_submodules_worktree_updater),
		OPT_BOOL('p', "patch", &patch_बहुलकम्, N_("select hunks interactively")),
		OPT_BOOL(0, "auto-advance", &interactive_opts.auto_advance,
			 N_("auto advance to the next file when selecting hunks interactively")),
		OPT_DIFF_UNIFIED(&interactive_opts.context),
		OPT_DIFF_INTERHUNK_CONTEXT(&interactive_opts.interhunkcontext),
		OPT_BOOL('N', "intent-to-add", &intent_गन्तव्यम्_सङ्कलय,
				N_("record only the fact that removed paths will be added later")),
		OPT_PATHSPEC_FROM_FILE(&pathspec_from_file),
		OPT_PATHSPEC_FILE_NUL(&pathspec_file_nul),
		OPT_अन्तः()
	};

	repo_config(the_repository, git_reset_config, रिक्त);

	argc = parse_options(argc, argv, उपसर्गः, options, git_reset_usage,
						PARSE_OPT_KEEP_DASHDASH);
	parse_तर्काः(&pathspec, argv, उपसर्गः, patch_बहुलकम्, &rev);

	यदि (pathspec_from_file) {
		यदि (patch_बहुलकम्)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--patch");

		यदि (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_पूर्णम्,
				    उपसर्गः, pathspec_from_file, pathspec_file_nul);
	} अन्यथा यदि (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	unborn = !वाक्यं_तुलय(rev, "HEAD") && repo_get_oid(the_repository, "HEAD",
						      &oid);
	यदि (unborn) {
		/* reset on unborn branch: treat as reset to empty tree */
		oidcpy(&oid, the_hash_algo->रिक्तानि_वृक्षः);
	} अन्यथा यदि (!pathspec.nr && !patch_बहुलकम्) {
		संरचना commit *commit;
		यदि (repo_get_oid_committish(the_repository, rev, &oid))
			die(_("Failed to resolve '%s' as a valid revision."), rev);
		commit = lookup_commit_reference(the_repository, &oid);
		यदि (!commit)
			die(_("Could not parse object '%s'."), rev);
		oidcpy(&oid, &commit->object.oid);
	} अन्यथा {
		संरचना वृक्षः *वृक्षः;
		यदि (repo_get_oid_treeish(the_repository, rev, &oid))
			die(_("Failed to resolve '%s' as a valid tree."), rev);
		वृक्षः = repo_parse_वृक्षः_परोक्षम्(the_repository, &oid);
		यदि (!वृक्षः)
			die(_("Could not parse object '%s'."), rev);
		oidcpy(&oid, &वृक्षः->object.oid);
	}

	यदि (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	यदि (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_पूर्णम्_सूचकांक = 0;

	यदि (patch_बहुलकम्) {
		यदि (reset_type != NONE)
			die(_("options '%s' and '%s' cannot be used together"), "--patch", "--{hard,mixed,soft}");
		trace2_cmd_बहुलकम्("patch-interactive");
		update_ref_स्थितिः = !!सततम्_सङ्कलय_p(the_repository, सङ्कलय_P_RESET,
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
					_(reset_type_नामानि[reset_type]));
	}
	यदि (reset_type == NONE)
		reset_type = MIXED; /* by default */

	यदि (pathspec.nr)
		trace2_cmd_बहुलकम्("path");
	अन्यथा
		trace2_cmd_बहुलकम्(reset_type_नामानि[reset_type]);

	यदि (reset_type != SOFT && (reset_type != MIXED || repo_get_work_वृक्षः(the_repository)))
		सज्जता_work_वृक्षः(the_repository);

	यदि (reset_type == MIXED && is_bare_repository(the_repository))
		die(_("%s reset is not allowed in a bare repository"),
		    _(reset_type_नामानि[reset_type]));

	यदि (intent_गन्तव्यम्_सङ्कलय && reset_type != MIXED)
		die(_("the option '%s' requires '%s'"), "-N", "--mixed");

	यदि (repo_read_सूचकांक(the_repository) < 0)
		die(_("index file corrupt"));

	/* Soft reset does not touch the index file nor the working tree
	 * at all, but requires them in a good order.  Other resets reset
	 * the index file to the tree object we are switching to. */
	यदि (reset_type == SOFT || reset_type == KEEP)
		die_if_unmerged_cache(reset_type);

	यदि (reset_type != SOFT) {
		संरचना तालकम्_file तालकम् = तालकम्_INIT;
		repo_hold_locked_सूचकांक(the_repository, &तालकम्,
				       तालकम्_DIE_ON_ERROR);
		यदि (reset_type == MIXED) {
			पूर्णाङ्क flags = quiet ? REFRESH_QUIET : REFRESH_अन्तर्_PORCELAIN;
			यदि (read_from_वृक्षः(&pathspec, &oid, intent_गन्तव्यम्_सङ्कलय)) {
				update_ref_स्थितिः = 1;
				गच्छ cleanup;
			}
			the_repository->सूचकांक->updated_skipworktree = 1;
			यदि (!no_refresh && repo_get_work_वृक्षः(the_repository)) {
				uint64_t t_begin, t_delta_अन्तर्_ms;

				t_begin = getnanotime();
				refresh_सूचकांक(the_repository->सूचकांक, flags, रिक्त, रिक्त,
					      _("Unstaged changes after reset:"));
				t_delta_अन्तर्_ms = (getnanotime() - t_begin) / 1000000;
				यदि (!quiet && advice_enabled(ADVICE_RESET_NO_REFRESH_WARNING) && t_delta_अन्तर्_ms > REFRESH_सूचकांक_DELAY_WARNING_अन्तर्_MS) {
					advise(_("It took %.2f seconds to refresh the index after reset.  You can use\n"
						 "'--no-refresh' to avoid this."), t_delta_अन्तर्_ms / 1000.0);
				}
			}
		} अन्यथा {
			संरचना object_id dummy;
			वर्ण *ref = रिक्त;
			पूर्णाङ्क err;

			repo_dwim_ref(the_repository, rev, वाक्यदैर्घ्यम्(rev),
				      &dummy, &ref, 0);
			यदि (ref && !starts_with(ref, "refs/"))
				FREE_AND_NULL(ref);

			err = reset_सूचकांक(ref, &oid, reset_type, quiet);
			यदि (reset_type == KEEP && !err)
				err = reset_सूचकांक(ref, &oid, MIXED, quiet);
			यदि (err)
				die(_("Could not reset index file to revision '%s'."), rev);
			मुक्त(ref);
		}

		यदि (write_locked_सूचकांक(the_repository->सूचकांक, &तालकम्, COMMIT_तालकम्))
			die(_("Could not write new index file."));
	}

	यदि (!pathspec.nr && !unborn) {
		/* Any resets without paths update HEAD to the head being
		 * switched to, saving the previous head in ORIG_HEAD before. */
		update_ref_स्थितिः = reset_refs(rev, &oid);

		यदि (reset_type == HARD && !update_ref_स्थितिः && !quiet)
			print_new_शीर्ष_पङ्क्तिः(lookup_commit_reference(the_repository, &oid));
	}
	यदि (!pathspec.nr)
		remove_branch_state(the_repository, 0);

	discard_सूचकांक(the_repository->सूचकांक);

cleanup:
	clear_pathspec(&pathspec);
	मुक्त(pathspec_from_file);
	प्रत्यागम update_ref_स्थितिः;
}
