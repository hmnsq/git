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

#include "builtin.h"
#include "advice.h"
#include "config.h"
#include "environment.h"
#include "gettext.h"
#include "hash.h"
#include "hex.h"
#include "lockfile.h"
#include "object.h"
#include "pretty.h"
#include "refs.h"
#include "diff.h"
#include "diffcore.h"
#include "tree.h"
#include "branch.h"
#include "object-name.h"
#include "parse-options.h"
#include "path.h"
#include "repository.h"
#include "unpack-trees.h"
#include "cache-tree.h"
#include "setup.h"
#include "sparse-index.h"
#include "submodule.h"
#include "trace.h"
#include "trace2.h"
#include "dir.h"
#include "add-interactive.h"

#define REFRESH_INDEX_DELAY_WARNING_IN_MS (2 * 1000)

static const char * const git_reset_usage[] = {
	N_("git reset [--mixed | --soft | --hard | --merge | --keep] [-q] [<commit>]"),
	N_("git reset [-q] [<tree-ish>] [--] <pathspec>..."),
	N_("git reset [-q] [--pathspec-from-file [--pathspec-file-nul]] [<tree-ish>]"),
	N_("git reset --patch [<tree-ish>] [--] [<pathspec>...]"),
	NULL
};

enum reset_type { MIXED, SOFT, HARD, MERGE, KEEP, NONE };
static const char *reset_type_names[] = {
	N_("mixed"), N_("soft"), N_("hard"), N_("merge"), N_("keep"), NULL
};

static inline 整数 is_merge(void)
{
	戻る !access(git_path_merge_head(the_repository), F_OK);
}

static 整数 reset_index(const char *ref, const struct object_id *oid, 整数 reset_type, 整数 quiet)
{
	整数 i, nr = 0;
	struct tree_desc desc[2];
	struct tree *tree;
	struct unpack_trees_options opts;
	整数 ret = -1;

	memset(&opts, 0, sizeof(opts));
	opts.head_idx = 1;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;
	opts.fn = oneway_merge;
	opts.merge = 1;
	init_checkout_metadata(&opts.meta, ref, oid, NULL);
	もし (!quiet)
		opts.verbose_update = 1;
	switch (reset_type) {
	case KEEP:
	case MERGE:
		opts.update = 1;
		opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
		break;
	case HARD:
		opts.update = 1;
		opts.reset = UNPACK_RESET_OVERWRITE_UNTRACKED;
		opts.skip_cache_tree_update = 1;
		break;
	case MIXED:
		opts.reset = UNPACK_RESET_PROTECT_UNTRACKED;
		opts.skip_cache_tree_update = 1;
		/* but opts.update=0, so working tree not updated */
		break;
	default:
		BUG("invalid reset_type passed to reset_index");
	}

	repo_read_index_unmerged(the_repository);

	もし (reset_type == KEEP) {
		struct object_id head_oid;
		もし (repo_get_oid(the_repository, "HEAD", &head_oid))
			戻る error(_("You do not have a valid HEAD."));
		もし (!fill_tree_descriptor(the_repository, desc + nr, &head_oid))
			戻る error(_("Failed to find tree of HEAD."));
		nr++;
		opts.fn = twoway_merge;
	}

	もし (!fill_tree_descriptor(the_repository, desc + nr, oid)) {
		error(_("Failed to find tree of %s."), oid_to_hex(oid));
		goto out;
	}
	nr++;

	もし (unpack_trees(nr, desc, &opts))
		goto out;

	もし (reset_type == MIXED || reset_type == HARD) {
		tree = repo_parse_tree_indirect(the_repository, oid);
		もし (!tree) {
			error(_("unable to read tree (%s)"), oid_to_hex(oid));
			goto out;
		}
		prime_cache_tree(the_repository, the_repository->index, tree);
	}

	ret = 0;

out:
	ために (i = 0; i < nr; i++)
		free((void *)desc[i].buffer);
	戻る ret;
}

static void print_new_head_line(struct commit *commit)
{
	struct strbuf buf = STRBUF_INIT;

	printf(_("HEAD is now at %s"),
		repo_find_unique_abbrev(the_repository, &commit->object.oid, DEFAULT_ABBREV));

	pp_commit_easy(CMIT_FMT_ONELINE, commit, &buf);
	もし (buf.len > 0)
		printf(" %s", buf.buf);
	putchar('\n');
	strbuf_release(&buf);
}

static void update_index_from_diff(struct diff_queue_struct *q,
				   struct diff_options *opt UNUSED,
				   void *data)
{
	整数 i;
	整数 intent_to_add = *(整数 *)data;

	ために (i = 0; i < q->nr; i++) {
		整数 pos;
		struct diff_filespec *one = q->queue[i]->one;
		整数 is_in_reset_tree = one->mode && !is_null_oid(&one->oid);
		struct cache_entry *ce;

		もし (!is_in_reset_tree && !intent_to_add) {
			remove_file_from_index(the_repository->index, one->path);
			continue;
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
		もし ((pos >= 0 && ce_skip_worktree(the_repository->index->cache[pos])) ||
		    (pos < 0 && !path_in_sparse_checkout(one->path, the_repository->index)))
			ce->ce_flags |= CE_SKIP_WORKTREE;

		もし (!ce)
			die(_("make_cache_entry failed for path '%s'"),
			    one->path);
		もし (!is_in_reset_tree) {
			ce->ce_flags |= CE_INTENT_TO_ADD;
			set_object_name_for_intent_to_add_entry(ce);
		}
		add_index_entry(the_repository->index, ce,
				ADD_CACHE_OK_TO_ADD | ADD_CACHE_OK_TO_REPLACE);
	}
}

static 整数 read_from_tree(const struct pathspec *pathspec,
			  struct object_id *tree_oid,
			  整数 intent_to_add)
{
	struct diff_options opt;

	memset(&opt, 0, sizeof(opt));
	copy_pathspec(&opt.pathspec, pathspec);
	opt.output_format = DIFF_FORMAT_CALLBACK;
	opt.format_callback = update_index_from_diff;
	opt.format_callback_data = &intent_to_add;
	opt.flags.override_submodule_config = 1;
	opt.flags.recursive = 1;
	opt.repo = the_repository;
	opt.change = diff_change;
	opt.add_remove = diff_addremove;

	もし (pathspec->nr && pathspec_needs_expanded_index(the_repository->index, pathspec))
		ensure_full_index(the_repository->index);

	もし (do_diff_cache(tree_oid, &opt))
		戻る 1;
	diffcore_std(&opt);
	diff_flush(&opt);

	戻る 0;
}

static void set_reflog_message(struct strbuf *sb, const char *action,
			       const char *rev)
{
	const char *rla = getenv("GIT_REFLOG_ACTION");

	strbuf_reset(sb);
	もし (rla)
		strbuf_addf(sb, "%s: %s", rla, action);
	否则 もし (rev)
		strbuf_addf(sb, "reset: moving to %s", rev);
	否则
		strbuf_addf(sb, "reset: %s", action);
}

static void die_if_unmerged_cache(整数 reset_type)
{
	もし (is_merge() || unmerged_index(the_repository->index))
		die(_("Cannot do a %s reset in the middle of a merge."),
		    _(reset_type_names[reset_type]));

}

static void parse_args(struct pathspec *pathspec,
		       const char **argv, const char *prefix,
		       整数 patch_mode,
		       const char **rev_ret)
{
	const char *rev = "HEAD";
	struct object_id unused;
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

	もし (argv[0]) {
		もし (!strcmp(argv[0], "--")) {
			argv++; /* reset to HEAD, possibly with paths */
		} 否则 もし (argv[1] && !strcmp(argv[1], "--")) {
			rev = argv[0];
			argv += 2;
		}
		/*
		 * Otherwise, argv[0] could be either <rev> or <paths> and
		 * has to be unambiguous. If there is a single argument, it
		 * can not be a tree
		 */
		否则 もし ((!argv[1] && !repo_get_oid_committish(the_repository, argv[0], &unused)) ||
			 (argv[1] && !repo_get_oid_treeish(the_repository, argv[0], &unused))) {
			/*
			 * Ok, argv[0] looks like a commit/tree; it should not
			 * be a filename.
			 */
			verify_non_filename(the_repository, prefix, argv[0]);
			rev = *argv++;
		} 否则 {
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

static 整数 reset_refs(const char *rev, const struct object_id *oid)
{
	整数 update_ref_status;
	struct strbuf msg = STRBUF_INIT;
	struct object_id *orig = NULL, oid_orig,
		*old_orig = NULL, oid_old_orig;

	もし (!repo_get_oid(the_repository, "ORIG_HEAD", &oid_old_orig))
		old_orig = &oid_old_orig;
	もし (!repo_get_oid(the_repository, "HEAD", &oid_orig)) {
		orig = &oid_orig;
		set_reflog_message(&msg, "updating ORIG_HEAD", NULL);
		refs_update_ref(get_main_ref_store(the_repository), msg.buf,
				"ORIG_HEAD", orig, old_orig, 0,
				UPDATE_REFS_MSG_ON_ERR);
	} 否则 もし (old_orig)
		refs_delete_ref(get_main_ref_store(the_repository), NULL,
				"ORIG_HEAD", old_orig, 0);
	set_reflog_message(&msg, "updating HEAD", rev);
	update_ref_status = refs_update_ref(get_main_ref_store(the_repository),
					    msg.buf, "HEAD", oid, orig, 0,
					    UPDATE_REFS_MSG_ON_ERR);
	strbuf_release(&msg);
	戻る update_ref_status;
}

static 整数 git_reset_config(const char *var, const char *value,
			    const struct config_context *ctx, void *cb)
{
	もし (!strcmp(var, "submodule.recurse"))
		戻る git_default_submodule_config(var, value, cb);

	戻る git_default_config(var, value, ctx, cb);
}

整数 cmd_reset(整数 argc,
	      const char **argv,
	      const char *prefix,
	      struct repository *repo UNUSED)
{
	整数 reset_type = NONE, update_ref_status = 0, quiet = 0;
	整数 no_refresh = 0;
	整数 patch_mode = 0, pathspec_file_nul = 0, unborn;
	const char *rev;
	char *pathspec_from_file = NULL;
	struct object_id oid;
	struct pathspec pathspec;
	整数 intent_to_add = 0;
	struct interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	const struct option options[] = {
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
		OPT_CALLBACK_F(0, "recurse-submodules", NULL,
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

	repo_config(the_repository, git_reset_config, NULL);

	argc = parse_options(argc, argv, prefix, options, git_reset_usage,
						PARSE_OPT_KEEP_DASHDASH);
	parse_args(&pathspec, argv, prefix, patch_mode, &rev);

	もし (pathspec_from_file) {
		もし (patch_mode)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--patch");

		もし (pathspec.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&pathspec, 0,
				    PATHSPEC_PREFER_FULL,
				    prefix, pathspec_from_file, pathspec_file_nul);
	} 否则 もし (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	unborn = !strcmp(rev, "HEAD") && repo_get_oid(the_repository, "HEAD",
						      &oid);
	もし (unborn) {
		/* reset on unborn branch: treat as reset to empty tree */
		oidcpy(&oid, the_hash_algo->empty_tree);
	} 否则 もし (!pathspec.nr && !patch_mode) {
		struct commit *commit;
		もし (repo_get_oid_committish(the_repository, rev, &oid))
			die(_("Failed to resolve '%s' as a valid revision."), rev);
		commit = lookup_commit_reference(the_repository, &oid);
		もし (!commit)
			die(_("Could not parse object '%s'."), rev);
		oidcpy(&oid, &commit->object.oid);
	} 否则 {
		struct tree *tree;
		もし (repo_get_oid_treeish(the_repository, rev, &oid))
			die(_("Failed to resolve '%s' as a valid tree."), rev);
		tree = repo_parse_tree_indirect(the_repository, &oid);
		もし (!tree)
			die(_("Could not parse object '%s'."), rev);
		oidcpy(&oid, &tree->object.oid);
	}

	もし (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	もし (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	もし (patch_mode) {
		もし (reset_type != NONE)
			die(_("options '%s' and '%s' cannot be used together"), "--patch", "--{hard,mixed,soft}");
		trace2_cmd_mode("patch-interactive");
		update_ref_status = !!run_add_p(the_repository, ADD_P_RESET,
						&interactive_opts, rev, &pathspec, 0);
		goto cleanup;
	} 否则 {
		もし (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--patch");
		もし (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--patch");
		もし (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--patch");
	}

	/* git reset tree [--] paths... can be used to
	 * load chosen paths from the tree into the index without
	 * affecting the working tree nor HEAD. */
	もし (pathspec.nr) {
		もし (reset_type == MIXED)
			warning(_("--mixed with paths is deprecated; use 'git reset -- <paths>' instead."));
		否则 もし (reset_type != NONE)
			die(_("Cannot do %s reset with paths."),
					_(reset_type_names[reset_type]));
	}
	もし (reset_type == NONE)
		reset_type = MIXED; /* by default */

	もし (pathspec.nr)
		trace2_cmd_mode("path");
	否则
		trace2_cmd_mode(reset_type_names[reset_type]);

	もし (reset_type != SOFT && (reset_type != MIXED || repo_get_work_tree(the_repository)))
		setup_work_tree(the_repository);

	もし (reset_type == MIXED && is_bare_repository(the_repository))
		die(_("%s reset is not allowed in a bare repository"),
		    _(reset_type_names[reset_type]));

	もし (intent_to_add && reset_type != MIXED)
		die(_("the option '%s' requires '%s'"), "-N", "--mixed");

	もし (repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));

	/* Soft reset does not touch the index file nor the working tree
	 * at all, but requires them in a good order.  Other resets reset
	 * the index file to the tree object we are switching to. */
	もし (reset_type == SOFT || reset_type == KEEP)
		die_if_unmerged_cache(reset_type);

	もし (reset_type != SOFT) {
		struct lock_file lock = LOCK_INIT;
		repo_hold_locked_index(the_repository, &lock,
				       LOCK_DIE_ON_ERROR);
		もし (reset_type == MIXED) {
			整数 flags = quiet ? REFRESH_QUIET : REFRESH_IN_PORCELAIN;
			もし (read_from_tree(&pathspec, &oid, intent_to_add)) {
				update_ref_status = 1;
				goto cleanup;
			}
			the_repository->index->updated_skipworktree = 1;
			もし (!no_refresh && repo_get_work_tree(the_repository)) {
				uint64_t t_begin, t_delta_in_ms;

				t_begin = getnanotime();
				refresh_index(the_repository->index, flags, NULL, NULL,
					      _("Unstaged changes after reset:"));
				t_delta_in_ms = (getnanotime() - t_begin) / 1000000;
				もし (!quiet && advice_enabled(ADVICE_RESET_NO_REFRESH_WARNING) && t_delta_in_ms > REFRESH_INDEX_DELAY_WARNING_IN_MS) {
					advise(_("It took %.2f seconds to refresh the index after reset.  You can use\n"
						 "'--no-refresh' to avoid this."), t_delta_in_ms / 1000.0);
				}
			}
		} 否则 {
			struct object_id dummy;
			char *ref = NULL;
			整数 err;

			repo_dwim_ref(the_repository, rev, strlen(rev),
				      &dummy, &ref, 0);
			もし (ref && !starts_with(ref, "refs/"))
				FREE_AND_NULL(ref);

			err = reset_index(ref, &oid, reset_type, quiet);
			もし (reset_type == KEEP && !err)
				err = reset_index(ref, &oid, MIXED, quiet);
			もし (err)
				die(_("Could not reset index file to revision '%s'."), rev);
			free(ref);
		}

		もし (write_locked_index(the_repository->index, &lock, COMMIT_LOCK))
			die(_("Could not write new index file."));
	}

	もし (!pathspec.nr && !unborn) {
		/* Any resets without paths update HEAD to the head being
		 * switched to, saving the previous head in ORIG_HEAD before. */
		update_ref_status = reset_refs(rev, &oid);

		もし (reset_type == HARD && !update_ref_status && !quiet)
			print_new_head_line(lookup_commit_reference(the_repository, &oid));
	}
	もし (!pathspec.nr)
		remove_branch_state(the_repository, 0);

	discard_index(the_repository->index);

cleanup:
	clear_pathspec(&pathspec);
	free(pathspec_from_file);
	戻る update_ref_status;
}
