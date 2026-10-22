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

#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "advice.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "hash.h"
#અન્તર્ભાવ "hex.h"
#અન્તર્ભાવ "lockfile.h"
#અન્તર્ભાવ "object.h"
#અન્તર્ભાવ "pretty.h"
#અન્તર્ભાવ "refs.h"
#અન્તર્ભાવ "diff.h"
#અન્તર્ભાવ "diffcore.h"
#અન્તર્ભાવ "tree.h"
#અન્તર્ભાવ "branch.h"
#અન્તર્ભાવ "object-name.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "path.h"
#અન્તર્ભાવ "repository.h"
#અન્તર્ભાવ "unpack-trees.h"
#અન્તર્ભાવ "cache-tree.h"
#અન્તર્ભાવ "setup.h"
#અન્તર્ભાવ "sparse-index.h"
#અન્તર્ભાવ "submodule.h"
#અન્તર્ભાવ "trace.h"
#અન્તર્ભાવ "trace2.h"
#અન્તર્ભાવ "dir.h"
#અન્તર્ભાવ "add-interactive.h"

#define REFRESH_INDEX_DELAY_WARNING_IN_MS (2 * 1000)

સ્થિર અચલ અક્ષર * અચલ git_reset_usage[] = {
	N_("git reset [--mixed | --soft | --hard | --merge | --keep] [-q] [<commit>]"),
	N_("git reset [-q] [<tree-ish>] [--] <pathspec>..."),
	N_("git reset [-q] [--pathspec-from-file [--pathspec-file-nul]] [<tree-ish>]"),
	N_("git reset --patch [<tree-ish>] [--] [<pathspec>...]"),
	ખાલી
};

ગણના reset_type { MIXED, SOFT, HARD, MERGE, KEEP, NONE };
સ્થિર અચલ અક્ષર *reset_type_names[] = {
	N_("mixed"), N_("soft"), N_("hard"), N_("merge"), N_("keep"), ખાલી
};

સ્થિર અંતઃરેખ પૂર્ણાંક is_merge(શૂન્ય)
{
	પાછા !access(git_path_merge_head(the_repository), F_OK);
}

સ્થિર પૂર્ણાંક reset_index(અચલ અક્ષર *ref, અચલ સંરચના object_id *oid, પૂર્ણાંક reset_type, પૂર્ણાંક quiet)
{
	પૂર્ણાંક i, nr = 0;
	સંરચના વૃક્ષ_desc desc[2];
	સંરચના વૃક્ષ *વૃક્ષ;
	સંરચના unpack_trees_options opts;
	પૂર્ણાંક ret = -1;

	memset(&opts, 0, કદ(opts));
	opts.head_idx = 1;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;
	opts.fn = oneway_merge;
	opts.merge = 1;
	init_checkout_metadata(&opts.meta, ref, oid, ખાલી);
	જો (!quiet)
		opts.verbose_update = 1;
	પસંદગી (reset_type) {
	સ્થિતિ KEEP:
	સ્થિતિ MERGE:
		opts.update = 1;
		opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
		તોડો;
	સ્થિતિ HARD:
		opts.update = 1;
		opts.reset = UNPACK_RESET_OVERWRITE_UNTRACKED;
		opts.skip_cache_વૃક્ષ_update = 1;
		તોડો;
	સ્થિતિ MIXED:
		opts.reset = UNPACK_RESET_PROTECT_UNTRACKED;
		opts.skip_cache_વૃક્ષ_update = 1;
		/* but opts.update=0, so working tree not updated */
		તોડો;
	મૂળભૂત:
		BUG("invalid reset_type passed to reset_index");
	}

	repo_read_index_unmerged(the_repository);

	જો (reset_type == KEEP) {
		સંરચના object_id head_oid;
		જો (repo_get_oid(the_repository, "HEAD", &head_oid))
			પાછા error(_("You do not have a valid HEAD."));
		જો (!fill_વૃક્ષ_descriptor(the_repository, desc + nr, &head_oid))
			પાછા error(_("Failed to find tree of HEAD."));
		nr++;
		opts.fn = twoway_merge;
	}

	જો (!fill_વૃક્ષ_descriptor(the_repository, desc + nr, oid)) {
		error(_("Failed to find tree of %s."), oid_to_hex(oid));
		જાઓ out;
	}
	nr++;

	જો (unpack_trees(nr, desc, &opts))
		જાઓ out;

	જો (reset_type == MIXED || reset_type == HARD) {
		વૃક્ષ = repo_parse_વૃક્ષ_indirect(the_repository, oid);
		જો (!વૃક્ષ) {
			error(_("unable to read tree (%s)"), oid_to_hex(oid));
			જાઓ out;
		}
		અવિભાજ્ય_cache_વૃક્ષ(the_repository, the_repository->index, વૃક્ષ);
	}

	ret = 0;

out:
	માટે (i = 0; i < nr; i++)
		મુક્ત((શૂન્ય *)desc[i].buffer);
	પાછા ret;
}

સ્થિર શૂન્ય print_new_head_લીટી(સંરચના commit *commit)
{
	સંરચના strbuf buf = STRBUF_INIT;

	printf(_("HEAD is now at %s"),
		repo_find_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV));

	pp_commit_easy(CMIT_FMT_ONELINE, commit, &buf);
	જો (buf.len > 0)
		printf(" %s", buf.buf);
	putchar('\n');
	strbuf_release(&buf);
}

સ્થિર શૂન્ય update_index_from_diff(સંરચના diff_queue_struct *q,
				   સંરચના diff_options *opt UNUSED,
				   શૂન્ય *data)
{
	પૂર્ણાંક i;
	પૂર્ણાંક intent_to_add = *(પૂર્ણાંક *)data;

	માટે (i = 0; i < q->nr; i++) {
		પૂર્ણાંક pos;
		સંરચના diff_filespec *one = q->queue[i]->one;
		પૂર્ણાંક is_in_reset_વૃક્ષ = one->mode && !is_null_oid(&one->oid);
		સંરચના cache_entry *ce;

		જો (!is_in_reset_વૃક્ષ && !intent_to_add) {
			remove_file_from_index(the_repository->index, one->path);
			ચાલુ;
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
		જો ((pos >= 0 && ce_skip_worktree(the_repository->index->cache[pos])) ||
		    (pos < 0 && !path_in_sparse_checkout(one->path, the_repository->index)))
			ce->ce_flags |= CE_SKIP_WORKTREE;

		જો (!ce)
			die(_("make_cache_entry failed for path '%s'"),
			    one->path);
		જો (!is_in_reset_વૃક્ષ) {
			ce->ce_flags |= CE_INTENT_TO_ADD;
			set_object_name_for_intent_to_add_entry(ce);
		}
		add_index_entry(the_repository->index, ce,
				ADD_CACHE_OK_TO_ADD | ADD_CACHE_OK_TO_REPLACE);
	}
}

સ્થિર પૂર્ણાંક read_from_વૃક્ષ(અચલ સંરચના pathspec *pathspec,
			  સંરચના object_id *વૃક્ષ_oid,
			  પૂર્ણાંક intent_to_add)
{
	સંરચના diff_options opt;

	memset(&opt, 0, કદ(opt));
	copy_pathspec(&opt.pathspec, pathspec);
	opt.output_format = DIFF_FORMAT_CALLBACK;
	opt.format_callback = update_index_from_diff;
	opt.format_callback_data = &intent_to_add;
	opt.flags.override_submodule_config = 1;
	opt.flags.recursive = 1;
	opt.repo = the_repository;
	opt.change = diff_change;
	opt.add_remove = diff_addremove;

	જો (pathspec->nr && pathspec_needs_expanded_index(the_repository->index, pathspec))
		ensure_full_index(the_repository->index);

	જો (do_diff_cache(વૃક્ષ_oid, &opt))
		પાછા 1;
	diffcore_std(&opt);
	diff_flush(&opt);

	પાછા 0;
}

સ્થિર શૂન્ય set_reflog_message(સંરચના strbuf *sb, અચલ અક્ષર *action,
			       અચલ અક્ષર *rev)
{
	અચલ અક્ષર *rla = getenv("GIT_REFLOG_ACTION");

	strbuf_reset(sb);
	જો (rla)
		strbuf_addf(sb, "%s: %s", rla, action);
	નહીંતર જો (rev)
		strbuf_addf(sb, "reset: moving to %s", rev);
	નહીંતર
		strbuf_addf(sb, "reset: %s", action);
}

સ્થિર શૂન્ય die_if_unmerged_cache(પૂર્ણાંક reset_type)
{
	જો (is_merge() || unmerged_index(the_repository->index))
		die(_("Cannot do a %s reset in the middle of a merge."),
		    _(reset_type_names[reset_type]));

}

સ્થિર શૂન્ય parse_args(સંરચના pathspec *pathspec,
		       અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
		       પૂર્ણાંક patch_mode,
		       અચલ અક્ષર **rev_ret)
{
	અચલ અક્ષર *rev = "HEAD";
	સંરચના object_id unused;
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

	જો (argv[0]) {
		જો (!strcmp(argv[0], "--")) {
			argv++; /* reset to HEAD, possibly with paths */
		} નહીંતર જો (argv[1] && !strcmp(argv[1], "--")) {
			rev = argv[0];
			argv += 2;
		}
		/*
		 * Otherwise, argv[0] could be either <rev> or <paths> and
		 * has to be unambiguous. If there is a single argument, it
		 * can not be a tree
		 */
		નહીંતર જો ((!argv[1] && !repo_get_oid_committish(the_repository, argv[0], &unused)) ||
			 (argv[1] && !repo_get_oid_treeish(the_repository, argv[0], &unused))) {
			/*
			 * Ok, argv[0] looks like a commit/tree; it should not
			 * be a filename.
			 */
			verify_non_filename(the_repository, prefix, argv[0]);
			rev = *argv++;
		} નહીંતર {
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

સ્થિર પૂર્ણાંક reset_refs(અચલ અક્ષર *rev, અચલ સંરચના object_id *oid)
{
	પૂર્ણાંક update_ref_status;
	સંરચના strbuf સંદેશ = STRBUF_INIT;
	સંરચના object_id *orig = ખાલી, oid_orig,
		*old_orig = ખાલી, oid_old_orig;

	જો (!repo_get_oid(the_repository, "ORIG_HEAD", &oid_old_orig))
		old_orig = &oid_old_orig;
	જો (!repo_get_oid(the_repository, "HEAD", &oid_orig)) {
		orig = &oid_orig;
		set_reflog_message(&સંદેશ, "updating ORIG_HEAD", ખાલી);
		refs_update_ref(get_main_ref_store(the_repository), સંદેશ.buf,
				"ORIG_HEAD", orig, old_orig, 0,
				UPDATE_REFS_સંદેશ_ON_ERR);
	} નહીંતર જો (old_orig)
		refs_delete_ref(get_main_ref_store(the_repository), ખાલી,
				"ORIG_HEAD", old_orig, 0);
	set_reflog_message(&સંદેશ, "updating HEAD", rev);
	update_ref_status = refs_update_ref(get_main_ref_store(the_repository),
					    સંદેશ.buf, "HEAD", oid, orig, 0,
					    UPDATE_REFS_સંદેશ_ON_ERR);
	strbuf_release(&સંદેશ);
	પાછા update_ref_status;
}

સ્થિર પૂર્ણાંક git_reset_config(અચલ અક્ષર *var, અચલ અક્ષર *કિંમત,
			    અચલ સંરચના config_context *ctx, શૂન્ય *cb)
{
	જો (!strcmp(var, "submodule.recurse"))
		પાછા git_default_submodule_config(var, કિંમત, cb);

	પાછા git_default_config(var, કિંમત, ctx, cb);
}

પૂર્ણાંક cmd_reset(પૂર્ણાંક argc,
	      અચલ અક્ષર **argv,
	      અચલ અક્ષર *prefix,
	      સંરચના repository *repo UNUSED)
{
	પૂર્ણાંક reset_type = NONE, update_ref_status = 0, quiet = 0;
	પૂર્ણાંક no_refresh = 0;
	પૂર્ણાંક patch_mode = 0, pathspec_file_nul = 0, unborn;
	અચલ અક્ષર *rev;
	અક્ષર *pathspec_from_file = ખાલી;
	સંરચના object_id oid;
	સંરચના pathspec pathspec;
	પૂર્ણાંક intent_to_add = 0;
	સંરચના interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	અચલ સંરચના option options[] = {
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
		OPT_CALLBACK_F(0, "recurse-submodules", ખાલી,
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

	repo_config(the_repository, git_reset_config, ખાલી);

	argc = parse_options(argc, argv, prefix, options, git_reset_usage,
						PARSE_OPT_KEEP_DASHDASH);
	parse_args(&pathspec, argv, prefix, patch_mode, &rev);

	જો (pathspec_from_file) {
		જો (patch_mode)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--patch");

		જો (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_FULL,
				    prefix, pathspec_from_file, pathspec_file_nul);
	} નહીંતર જો (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	unborn = !strcmp(rev, "HEAD") && repo_get_oid(the_repository, "HEAD",
						      &oid);
	જો (unborn) {
		/* reset on unborn branch: treat as reset to empty tree */
		oidcpy(&oid, the_hash_algo->empty_વૃક્ષ);
	} નહીંતર જો (!pathspec.nr && !patch_mode) {
		સંરચના commit *commit;
		જો (repo_get_oid_committish(the_repository, rev, &oid))
			die(_("Failed to resolve '%s' as a valid revision."), rev);
		commit = lookup_commit_reference(the_repository, &oid);
		જો (!commit)
			die(_("Could not parse object '%s'."), rev);
		oidcpy(&oid, &commit->object.oid);
	} નહીંતર {
		સંરચના વૃક્ષ *વૃક્ષ;
		જો (repo_get_oid_treeish(the_repository, rev, &oid))
			die(_("Failed to resolve '%s' as a valid tree."), rev);
		વૃક્ષ = repo_parse_વૃક્ષ_indirect(the_repository, &oid);
		જો (!વૃક્ષ)
			die(_("Could not parse object '%s'."), rev);
		oidcpy(&oid, &વૃક્ષ->object.oid);
	}

	જો (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	જો (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	જો (patch_mode) {
		જો (reset_type != NONE)
			die(_("options '%s' and '%s' cannot be used together"), "--patch", "--{hard,mixed,soft}");
		trace2_cmd_mode("patch-interactive");
		update_ref_status = !!run_add_p(the_repository, ADD_P_RESET,
						&interactive_opts, rev, &pathspec, 0);
		જાઓ cleanup;
	} નહીંતર {
		જો (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--patch");
		જો (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--patch");
		જો (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--patch");
	}

	/* git reset tree [--] paths... can be used to
	 * load chosen paths from the tree into the index without
	 * affecting the working tree nor HEAD. */
	જો (pathspec.nr) {
		જો (reset_type == MIXED)
			warning(_("--mixed with paths is deprecated; use 'git reset -- <paths>' instead."));
		નહીંતર જો (reset_type != NONE)
			die(_("Cannot do %s reset with paths."),
					_(reset_type_names[reset_type]));
	}
	જો (reset_type == NONE)
		reset_type = MIXED; /* by default */

	જો (pathspec.nr)
		trace2_cmd_mode("path");
	નહીંતર
		trace2_cmd_mode(reset_type_names[reset_type]);

	જો (reset_type != SOFT && (reset_type != MIXED || repo_get_work_વૃક્ષ(the_repository)))
		setup_work_વૃક્ષ(the_repository);

	જો (reset_type == MIXED && is_bare_repository(the_repository))
		die(_("%s reset is not allowed in a bare repository"),
		    _(reset_type_names[reset_type]));

	જો (intent_to_add && reset_type != MIXED)
		die(_("the option '%s' requires '%s'"), "-N", "--mixed");

	જો (repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));

	/* Soft reset does not touch the index file nor the working tree
	 * at all, but requires them in a good order.  Other resets reset
	 * the index file to the tree object we are switching to. */
	જો (reset_type == SOFT || reset_type == KEEP)
		die_if_unmerged_cache(reset_type);

	જો (reset_type != SOFT) {
		સંરચના lock_file lock = LOCK_INIT;
		repo_hold_locked_index(the_repository, &lock,
				       LOCK_DIE_ON_ERROR);
		જો (reset_type == MIXED) {
			પૂર્ણાંક flags = quiet ? REFRESH_QUIET : REFRESH_IN_PORCELAIN;
			જો (read_from_વૃક્ષ(&pathspec, &oid, intent_to_add)) {
				update_ref_status = 1;
				જાઓ cleanup;
			}
			the_repository->index->updated_skipworktree = 1;
			જો (!no_refresh && repo_get_work_વૃક્ષ(the_repository)) {
				uint64_t t_begin, t_delta_in_ms;

				t_begin = getnanotime();
				refresh_index(the_repository->index, flags, ખાલી, ખાલી,
					      _("Unstaged changes after reset:"));
				t_delta_in_ms = (getnanotime() - t_begin) / 1000000;
				જો (!quiet && advice_enabled(ADVICE_RESET_NO_REFRESH_WARNING) && t_delta_in_ms > REFRESH_INDEX_DELAY_WARNING_IN_MS) {
					advise(_("It took %.2f seconds to refresh the index after reset.  You can use\n"
						 "'--no-refresh' to avoid this."), t_delta_in_ms / 1000.0);
				}
			}
		} નહીંતર {
			સંરચના object_id dummy;
			અક્ષર *ref = ખાલી;
			પૂર્ણાંક err;

			repo_dwim_ref(the_repository, rev, strlen(rev),
				      &dummy, &ref, 0);
			જો (ref && !starts_with(ref, "refs/"))
				FREE_AND_NULL(ref);

			err = reset_index(ref, &oid, reset_type, quiet);
			જો (reset_type == KEEP && !err)
				err = reset_index(ref, &oid, MIXED, quiet);
			જો (err)
				die(_("Could not reset index file to revision '%s'."), rev);
			મુક્ત(ref);
		}

		જો (write_locked_index(the_repository->index, &lock, COMMIT_LOCK))
			die(_("Could not write new index file."));
	}

	જો (!pathspec.nr && !unborn) {
		/* Any resets without paths update HEAD to the head being
		 * switched to, saving the previous head in ORIG_HEAD before. */
		update_ref_status = reset_refs(rev, &oid);

		જો (reset_type == HARD && !update_ref_status && !quiet)
			print_new_head_લીટી(lookup_commit_reference(the_repository, &oid));
	}
	જો (!pathspec.nr)
		remove_branch_state(the_repository, 0);

	discard_index(the_repository->index);

cleanup:
	clear_pathspec(&pathspec);
	મુક્ત(pathspec_from_file);
	પાછા update_ref_status;
}
