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

#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "advice.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "hash.h"
#അന്തര്ഭാവ "hex.h"
#അന്തര്ഭാവ "lockfile.h"
#അന്തര്ഭാവ "object.h"
#അന്തര്ഭാവ "pretty.h"
#അന്തര്ഭാവ "refs.h"
#അന്തര്ഭാവ "diff.h"
#അന്തര്ഭാവ "diffcore.h"
#അന്തര്ഭാവ "tree.h"
#അന്തര്ഭാവ "branch.h"
#അന്തര്ഭാവ "object-name.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "path.h"
#അന്തര്ഭാവ "repository.h"
#അന്തര്ഭാവ "unpack-trees.h"
#അന്തര്ഭാവ "cache-tree.h"
#അന്തര്ഭാവ "setup.h"
#അന്തര്ഭാവ "sparse-index.h"
#അന്തര്ഭാവ "submodule.h"
#അന്തര്ഭാവ "trace.h"
#അന്തര്ഭാവ "trace2.h"
#അന്തര്ഭാവ "dir.h"
#അന്തര്ഭാവ "add-interactive.h"

#define REFRESH_INDEX_DELAY_WARNING_IN_MS (2 * 1000)

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം git_reset_usage[] = {
	N_("git reset [--mixed | --soft | --hard | --merge | --keep] [-q] [<commit>]"),
	N_("git reset [-q] [<tree-ish>] [--] <pathspec>..."),
	N_("git reset [-q] [--pathspec-from-file [--pathspec-file-nul]] [<tree-ish>]"),
	N_("git reset --patch [<tree-ish>] [--] [<pathspec>...]"),
	ശൂന്യ
};

എണ്ണൽ reset_type { MIXED, SOFT, HARD, MERGE, KEEP, NONE };
സ്ഥിതം സ്ഥിരം അക്ഷരം *reset_type_names[] = {
	N_("mixed"), N_("soft"), N_("hard"), N_("merge"), N_("keep"), ശൂന്യ
};

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ is_merge(ശൂന്യം)
{
	തിരികെ !access(git_path_merge_head(the_repository), F_OK);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ reset_index(സ്ഥിരം അക്ഷരം *ref, സ്ഥിരം ഘടന object_id *oid, പൂർണ്ണസംഖ്യ reset_type, പൂർണ്ണസംഖ്യ quiet)
{
	പൂർണ്ണസംഖ്യ i, nr = 0;
	ഘടന വൃക്ഷം_desc desc[2];
	ഘടന വൃക്ഷം *വൃക്ഷം;
	ഘടന unpack_trees_options opts;
	പൂർണ്ണസംഖ്യ ret = -1;

	memset(&opts, 0, വലുപ്പം(opts));
	opts.head_idx = 1;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;
	opts.fn = oneway_merge;
	opts.merge = 1;
	init_checkout_metadata(&opts.meta, ref, oid, ശൂന്യ);
	എങ്കിൽ (!quiet)
		opts.verbose_update = 1;
	തിരഞ്ഞെടുപ്പ് (reset_type) {
	അവസ്ഥ KEEP:
	അവസ്ഥ MERGE:
		opts.update = 1;
		opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
		നിർത്തുക;
	അവസ്ഥ HARD:
		opts.update = 1;
		opts.reset = UNPACK_RESET_OVERWRITE_UNTRACKED;
		opts.skip_cache_വൃക്ഷം_update = 1;
		നിർത്തുക;
	അവസ്ഥ MIXED:
		opts.reset = UNPACK_RESET_PROTECT_UNTRACKED;
		opts.skip_cache_വൃക്ഷം_update = 1;
		/* but opts.update=0, so working tree not updated */
		നിർത്തുക;
	സ്ഥിരസ്ഥിതി:
		BUG("invalid reset_type passed to reset_index");
	}

	repo_read_index_unmerged(the_repository);

	എങ്കിൽ (reset_type == KEEP) {
		ഘടന object_id head_oid;
		എങ്കിൽ (repo_get_oid(the_repository, "HEAD", &head_oid))
			തിരികെ error(_("You do not have a valid HEAD."));
		എങ്കിൽ (!fill_വൃക്ഷം_descriptor(the_repository, desc + nr, &head_oid))
			തിരികെ error(_("Failed to find tree of HEAD."));
		nr++;
		opts.fn = twoway_merge;
	}

	എങ്കിൽ (!fill_വൃക്ഷം_descriptor(the_repository, desc + nr, oid)) {
		error(_("Failed to find tree of %s."), oid_to_hex(oid));
		പോകുക out;
	}
	nr++;

	എങ്കിൽ (unpack_trees(nr, desc, &opts))
		പോകുക out;

	എങ്കിൽ (reset_type == MIXED || reset_type == HARD) {
		വൃക്ഷം = repo_parse_വൃക്ഷം_indirect(the_repository, oid);
		എങ്കിൽ (!വൃക്ഷം) {
			error(_("unable to read tree (%s)"), oid_to_hex(oid));
			പോകുക out;
		}
		അഭാജ്യം_cache_വൃക്ഷം(the_repository, the_repository->index, വൃക്ഷം);
	}

	ret = 0;

out:
	വേണ്ടി (i = 0; i < nr; i++)
		സ്വതന്ത്രം((ശൂന്യം *)desc[i].buffer);
	തിരികെ ret;
}

സ്ഥിതം ശൂന്യം print_new_head_വരി(ഘടന commit *commit)
{
	ഘടന strbuf buf = STRBUF_INIT;

	printf(_("HEAD is now at %s"),
		repo_find_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV));

	pp_commit_easy(CMIT_FMT_ONELINE, commit, &buf);
	എങ്കിൽ (buf.len > 0)
		printf(" %s", buf.buf);
	putchar('\n');
	strbuf_release(&buf);
}

സ്ഥിതം ശൂന്യം update_index_from_diff(ഘടന diff_queue_struct *q,
				   ഘടന diff_options *opt UNUSED,
				   ശൂന്യം *data)
{
	പൂർണ്ണസംഖ്യ i;
	പൂർണ്ണസംഖ്യ intent_to_add = *(പൂർണ്ണസംഖ്യ *)data;

	വേണ്ടി (i = 0; i < q->nr; i++) {
		പൂർണ്ണസംഖ്യ pos;
		ഘടന diff_filespec *one = q->queue[i]->one;
		പൂർണ്ണസംഖ്യ is_in_reset_വൃക്ഷം = one->mode && !is_null_oid(&one->oid);
		ഘടന cache_entry *ce;

		എങ്കിൽ (!is_in_reset_വൃക്ഷം && !intent_to_add) {
			remove_file_from_index(the_repository->index, one->path);
			തുടരുക;
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
		എങ്കിൽ ((pos >= 0 && ce_skip_worktree(the_repository->index->cache[pos])) ||
		    (pos < 0 && !path_in_sparse_checkout(one->path, the_repository->index)))
			ce->ce_flags |= CE_SKIP_WORKTREE;

		എങ്കിൽ (!ce)
			die(_("make_cache_entry failed for path '%s'"),
			    one->path);
		എങ്കിൽ (!is_in_reset_വൃക്ഷം) {
			ce->ce_flags |= CE_INTENT_TO_ADD;
			set_object_name_for_intent_to_add_entry(ce);
		}
		add_index_entry(the_repository->index, ce,
				ADD_CACHE_OK_TO_ADD | ADD_CACHE_OK_TO_REPLACE);
	}
}

സ്ഥിതം പൂർണ്ണസംഖ്യ read_from_വൃക്ഷം(സ്ഥിരം ഘടന pathspec *pathspec,
			  ഘടന object_id *വൃക്ഷം_oid,
			  പൂർണ്ണസംഖ്യ intent_to_add)
{
	ഘടന diff_options opt;

	memset(&opt, 0, വലുപ്പം(opt));
	copy_pathspec(&opt.pathspec, pathspec);
	opt.output_format = DIFF_FORMAT_CALLBACK;
	opt.format_callback = update_index_from_diff;
	opt.format_callback_data = &intent_to_add;
	opt.flags.override_submodule_config = 1;
	opt.flags.recursive = 1;
	opt.repo = the_repository;
	opt.change = diff_change;
	opt.add_remove = diff_addremove;

	എങ്കിൽ (pathspec->nr && pathspec_needs_expanded_index(the_repository->index, pathspec))
		ensure_full_index(the_repository->index);

	എങ്കിൽ (do_diff_cache(വൃക്ഷം_oid, &opt))
		തിരികെ 1;
	diffcore_std(&opt);
	diff_flush(&opt);

	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം set_reflog_message(ഘടന strbuf *sb, സ്ഥിരം അക്ഷരം *action,
			       സ്ഥിരം അക്ഷരം *rev)
{
	സ്ഥിരം അക്ഷരം *rla = getenv("GIT_REFLOG_ACTION");

	strbuf_reset(sb);
	എങ്കിൽ (rla)
		strbuf_addf(sb, "%s: %s", rla, action);
	അല്ലെങ്കിൽ എങ്കിൽ (rev)
		strbuf_addf(sb, "reset: moving to %s", rev);
	അല്ലെങ്കിൽ
		strbuf_addf(sb, "reset: %s", action);
}

സ്ഥിതം ശൂന്യം die_if_unmerged_cache(പൂർണ്ണസംഖ്യ reset_type)
{
	എങ്കിൽ (is_merge() || unmerged_index(the_repository->index))
		die(_("Cannot do a %s reset in the middle of a merge."),
		    _(reset_type_names[reset_type]));

}

സ്ഥിതം ശൂന്യം parse_args(ഘടന pathspec *pathspec,
		       സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
		       പൂർണ്ണസംഖ്യ patch_mode,
		       സ്ഥിരം അക്ഷരം **rev_ret)
{
	സ്ഥിരം അക്ഷരം *rev = "HEAD";
	ഘടന object_id unused;
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

	എങ്കിൽ (argv[0]) {
		എങ്കിൽ (!strcmp(argv[0], "--")) {
			argv++; /* reset to HEAD, possibly with paths */
		} അല്ലെങ്കിൽ എങ്കിൽ (argv[1] && !strcmp(argv[1], "--")) {
			rev = argv[0];
			argv += 2;
		}
		/*
		 * Otherwise, argv[0] could be either <rev> or <paths> and
		 * has to be unambiguous. If there is a single argument, it
		 * can not be a tree
		 */
		അല്ലെങ്കിൽ എങ്കിൽ ((!argv[1] && !repo_get_oid_committish(the_repository, argv[0], &unused)) ||
			 (argv[1] && !repo_get_oid_treeish(the_repository, argv[0], &unused))) {
			/*
			 * Ok, argv[0] looks like a commit/tree; it should not
			 * be a filename.
			 */
			verify_non_filename(the_repository, prefix, argv[0]);
			rev = *argv++;
		} അല്ലെങ്കിൽ {
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

സ്ഥിതം പൂർണ്ണസംഖ്യ reset_refs(സ്ഥിരം അക്ഷരം *rev, സ്ഥിരം ഘടന object_id *oid)
{
	പൂർണ്ണസംഖ്യ update_ref_status;
	ഘടന strbuf സന്ദേശം = STRBUF_INIT;
	ഘടന object_id *orig = ശൂന്യ, oid_orig,
		*old_orig = ശൂന്യ, oid_old_orig;

	എങ്കിൽ (!repo_get_oid(the_repository, "ORIG_HEAD", &oid_old_orig))
		old_orig = &oid_old_orig;
	എങ്കിൽ (!repo_get_oid(the_repository, "HEAD", &oid_orig)) {
		orig = &oid_orig;
		set_reflog_message(&സന്ദേശം, "updating ORIG_HEAD", ശൂന്യ);
		refs_update_ref(get_main_ref_store(the_repository), സന്ദേശം.buf,
				"ORIG_HEAD", orig, old_orig, 0,
				UPDATE_REFS_സന്ദേശം_ON_ERR);
	} അല്ലെങ്കിൽ എങ്കിൽ (old_orig)
		refs_delete_ref(get_main_ref_store(the_repository), ശൂന്യ,
				"ORIG_HEAD", old_orig, 0);
	set_reflog_message(&സന്ദേശം, "updating HEAD", rev);
	update_ref_status = refs_update_ref(get_main_ref_store(the_repository),
					    സന്ദേശം.buf, "HEAD", oid, orig, 0,
					    UPDATE_REFS_സന്ദേശം_ON_ERR);
	strbuf_release(&സന്ദേശം);
	തിരികെ update_ref_status;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ git_reset_config(സ്ഥിരം അക്ഷരം *var, സ്ഥിരം അക്ഷരം *മൂല്യം,
			    സ്ഥിരം ഘടന config_context *ctx, ശൂന്യം *cb)
{
	എങ്കിൽ (!strcmp(var, "submodule.recurse"))
		തിരികെ git_default_submodule_config(var, മൂല്യം, cb);

	തിരികെ git_default_config(var, മൂല്യം, ctx, cb);
}

പൂർണ്ണസംഖ്യ cmd_reset(പൂർണ്ണസംഖ്യ argc,
	      സ്ഥിരം അക്ഷരം **argv,
	      സ്ഥിരം അക്ഷരം *prefix,
	      ഘടന repository *repo UNUSED)
{
	പൂർണ്ണസംഖ്യ reset_type = NONE, update_ref_status = 0, quiet = 0;
	പൂർണ്ണസംഖ്യ no_refresh = 0;
	പൂർണ്ണസംഖ്യ patch_mode = 0, pathspec_file_nul = 0, unborn;
	സ്ഥിരം അക്ഷരം *rev;
	അക്ഷരം *pathspec_from_file = ശൂന്യ;
	ഘടന object_id oid;
	ഘടന pathspec pathspec;
	പൂർണ്ണസംഖ്യ intent_to_add = 0;
	ഘടന interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	സ്ഥിരം ഘടന option options[] = {
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
		OPT_CALLBACK_F(0, "recurse-submodules", ശൂന്യ,
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

	repo_config(the_repository, git_reset_config, ശൂന്യ);

	argc = parse_options(argc, argv, prefix, options, git_reset_usage,
						PARSE_OPT_KEEP_DASHDASH);
	parse_args(&pathspec, argv, prefix, patch_mode, &rev);

	എങ്കിൽ (pathspec_from_file) {
		എങ്കിൽ (patch_mode)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--patch");

		എങ്കിൽ (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_FULL,
				    prefix, pathspec_from_file, pathspec_file_nul);
	} അല്ലെങ്കിൽ എങ്കിൽ (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	unborn = !strcmp(rev, "HEAD") && repo_get_oid(the_repository, "HEAD",
						      &oid);
	എങ്കിൽ (unborn) {
		/* reset on unborn branch: treat as reset to empty tree */
		oidcpy(&oid, the_hash_algo->empty_വൃക്ഷം);
	} അല്ലെങ്കിൽ എങ്കിൽ (!pathspec.nr && !patch_mode) {
		ഘടന commit *commit;
		എങ്കിൽ (repo_get_oid_committish(the_repository, rev, &oid))
			die(_("Failed to resolve '%s' as a valid revision."), rev);
		commit = lookup_commit_reference(the_repository, &oid);
		എങ്കിൽ (!commit)
			die(_("Could not parse object '%s'."), rev);
		oidcpy(&oid, &commit->object.oid);
	} അല്ലെങ്കിൽ {
		ഘടന വൃക്ഷം *വൃക്ഷം;
		എങ്കിൽ (repo_get_oid_treeish(the_repository, rev, &oid))
			die(_("Failed to resolve '%s' as a valid tree."), rev);
		വൃക്ഷം = repo_parse_വൃക്ഷം_indirect(the_repository, &oid);
		എങ്കിൽ (!വൃക്ഷം)
			die(_("Could not parse object '%s'."), rev);
		oidcpy(&oid, &വൃക്ഷം->object.oid);
	}

	എങ്കിൽ (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	എങ്കിൽ (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	എങ്കിൽ (patch_mode) {
		എങ്കിൽ (reset_type != NONE)
			die(_("options '%s' and '%s' cannot be used together"), "--patch", "--{hard,mixed,soft}");
		trace2_cmd_mode("patch-interactive");
		update_ref_status = !!run_add_p(the_repository, ADD_P_RESET,
						&interactive_opts, rev, &pathspec, 0);
		പോകുക cleanup;
	} അല്ലെങ്കിൽ {
		എങ്കിൽ (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--patch");
		എങ്കിൽ (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--patch");
		എങ്കിൽ (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--patch");
	}

	/* git reset tree [--] paths... can be used to
	 * load chosen paths from the tree into the index without
	 * affecting the working tree nor HEAD. */
	എങ്കിൽ (pathspec.nr) {
		എങ്കിൽ (reset_type == MIXED)
			warning(_("--mixed with paths is deprecated; use 'git reset -- <paths>' instead."));
		അല്ലെങ്കിൽ എങ്കിൽ (reset_type != NONE)
			die(_("Cannot do %s reset with paths."),
					_(reset_type_names[reset_type]));
	}
	എങ്കിൽ (reset_type == NONE)
		reset_type = MIXED; /* by default */

	എങ്കിൽ (pathspec.nr)
		trace2_cmd_mode("path");
	അല്ലെങ്കിൽ
		trace2_cmd_mode(reset_type_names[reset_type]);

	എങ്കിൽ (reset_type != SOFT && (reset_type != MIXED || repo_get_work_വൃക്ഷം(the_repository)))
		setup_work_വൃക്ഷം(the_repository);

	എങ്കിൽ (reset_type == MIXED && is_bare_repository(the_repository))
		die(_("%s reset is not allowed in a bare repository"),
		    _(reset_type_names[reset_type]));

	എങ്കിൽ (intent_to_add && reset_type != MIXED)
		die(_("the option '%s' requires '%s'"), "-N", "--mixed");

	എങ്കിൽ (repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));

	/* Soft reset does not touch the index file nor the working tree
	 * at all, but requires them in a good order.  Other resets reset
	 * the index file to the tree object we are switching to. */
	എങ്കിൽ (reset_type == SOFT || reset_type == KEEP)
		die_if_unmerged_cache(reset_type);

	എങ്കിൽ (reset_type != SOFT) {
		ഘടന lock_file lock = LOCK_INIT;
		repo_hold_locked_index(the_repository, &lock,
				       LOCK_DIE_ON_ERROR);
		എങ്കിൽ (reset_type == MIXED) {
			പൂർണ്ണസംഖ്യ flags = quiet ? REFRESH_QUIET : REFRESH_IN_PORCELAIN;
			എങ്കിൽ (read_from_വൃക്ഷം(&pathspec, &oid, intent_to_add)) {
				update_ref_status = 1;
				പോകുക cleanup;
			}
			the_repository->index->updated_skipworktree = 1;
			എങ്കിൽ (!no_refresh && repo_get_work_വൃക്ഷം(the_repository)) {
				uint64_t t_begin, t_delta_in_ms;

				t_begin = getnanotime();
				refresh_index(the_repository->index, flags, ശൂന്യ, ശൂന്യ,
					      _("Unstaged changes after reset:"));
				t_delta_in_ms = (getnanotime() - t_begin) / 1000000;
				എങ്കിൽ (!quiet && advice_enabled(ADVICE_RESET_NO_REFRESH_WARNING) && t_delta_in_ms > REFRESH_INDEX_DELAY_WARNING_IN_MS) {
					advise(_("It took %.2f seconds to refresh the index after reset.  You can use\n"
						 "'--no-refresh' to avoid this."), t_delta_in_ms / 1000.0);
				}
			}
		} അല്ലെങ്കിൽ {
			ഘടന object_id dummy;
			അക്ഷരം *ref = ശൂന്യ;
			പൂർണ്ണസംഖ്യ err;

			repo_dwim_ref(the_repository, rev, strlen(rev),
				      &dummy, &ref, 0);
			എങ്കിൽ (ref && !starts_with(ref, "refs/"))
				FREE_AND_NULL(ref);

			err = reset_index(ref, &oid, reset_type, quiet);
			എങ്കിൽ (reset_type == KEEP && !err)
				err = reset_index(ref, &oid, MIXED, quiet);
			എങ്കിൽ (err)
				die(_("Could not reset index file to revision '%s'."), rev);
			സ്വതന്ത്രം(ref);
		}

		എങ്കിൽ (write_locked_index(the_repository->index, &lock, COMMIT_LOCK))
			die(_("Could not write new index file."));
	}

	എങ്കിൽ (!pathspec.nr && !unborn) {
		/* Any resets without paths update HEAD to the head being
		 * switched to, saving the previous head in ORIG_HEAD before. */
		update_ref_status = reset_refs(rev, &oid);

		എങ്കിൽ (reset_type == HARD && !update_ref_status && !quiet)
			print_new_head_വരി(lookup_commit_reference(the_repository, &oid));
	}
	എങ്കിൽ (!pathspec.nr)
		remove_branch_state(the_repository, 0);

	discard_index(the_repository->index);

cleanup:
	clear_pathspec(&pathspec);
	സ്വതന്ത്രം(pathspec_from_file);
	തിരികെ update_ref_status;
}
