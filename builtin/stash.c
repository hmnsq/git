#define USE_THE_REPOSITORY_VARIABLE

#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "abspath.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "hash.h"
#અન્તર્ભાવ "hex.h"
#અન્તર્ભાવ "object-name.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "refs.h"
#અન્તર્ભાવ "stash.h"
#અન્તર્ભાવ "lockfile.h"
#અન્તર્ભાવ "cache-tree.h"
#અન્તર્ભાવ "unpack-trees.h"
#અન્તર્ભાવ "merge-ort-wrappers.h"
#અન્તર્ભાવ "strvec.h"
#અન્તર્ભાવ "run-command.h"
#અન્તર્ભાવ "dir.h"
#અન્તર્ભાવ "entry.h"
#અન્તર્ભાવ "preload-index.h"
#અન્તર્ભાવ "read-cache.h"
#અન્તર્ભાવ "repository.h"
#અન્તર્ભાવ "rerere.h"
#અન્તર્ભાવ "revision.h"
#અન્તર્ભાવ "setup.h"
#અન્તર્ભાવ "sparse-index.h"
#અન્તર્ભાવ "log-tree.h"
#અન્તર્ભાવ "diffcore.h"
#અન્તર્ભાવ "reflog.h"
#અન્તર્ભાવ "reflog-walk.h"
#અન્તર્ભાવ "add-interactive.h"
#અન્તર્ભાવ "commit.h"

#define INCLUDE_ALL_FILES 2

#define BUILTIN_STASH_LIST_USAGE \
	N_("git stash list [<log-options>]")
#define BUILTIN_STASH_SHOW_USAGE \
	N_("git stash show [-u | --include-untracked | --only-untracked] [<diff-options>] [<stash>]")
#define BUILTIN_STASH_DROP_USAGE \
	N_("git stash drop [-q | --quiet] [<stash>]")
#define BUILTIN_STASH_કાઢો_USAGE \
	N_("git stash pop [--index] [-q | --quiet] [<stash>]")
#define BUILTIN_STASH_APPLY_USAGE \
	N_("git stash apply [--index] [-q | --quiet] [--label-ours=<label>] [--label-theirs=<label>] [--label-base=<label>] [<stash>]")
#define BUILTIN_STASH_BRANCH_USAGE \
	N_("git stash branch <branchname> [<stash>]")
#define BUILTIN_STASH_STORE_USAGE \
	N_("git stash store [(-m | --message) <message>] [-q | --quiet] <commit>")
#define BUILTIN_STASH_ધકેલો_USAGE \
	N_("git stash [push] [-p | --patch] [-S | --staged] [-k | --[no-]keep-index] [-q | --quiet]\n" \
	   "          [-u | --include-untracked] [-a | --all] [(-m | --message) <message>]\n" \
	   "          [--pathspec-from-file=<file> [--pathspec-file-nul]]\n" \
	   "          [--] [<pathspec>...]")
#define BUILTIN_STASH_SAVE_USAGE \
	N_("git stash save [-p | --patch] [-S | --staged] [-k | --[no-]keep-index] [-q | --quiet]\n" \
	   "          [-u | --include-untracked] [-a | --all] [<message>]")
#define BUILTIN_STASH_CREATE_USAGE \
	N_("git stash create [<message>]")
#define BUILTIN_STASH_EXPORT_USAGE \
	N_("git stash export (--print | --to-ref <ref>) [<stash>...]")
#define BUILTIN_STASH_IMPORT_USAGE \
	N_("git stash import <commit>")
#define BUILTIN_STASH_CLEAR_USAGE \
	"git stash clear"

સ્થિર અચલ અક્ષર * અચલ git_stash_usage[] = {
	BUILTIN_STASH_LIST_USAGE,
	BUILTIN_STASH_SHOW_USAGE,
	BUILTIN_STASH_DROP_USAGE,
	BUILTIN_STASH_કાઢો_USAGE,
	BUILTIN_STASH_APPLY_USAGE,
	BUILTIN_STASH_BRANCH_USAGE,
	BUILTIN_STASH_ધકેલો_USAGE,
	BUILTIN_STASH_SAVE_USAGE,
	BUILTIN_STASH_CLEAR_USAGE,
	BUILTIN_STASH_CREATE_USAGE,
	BUILTIN_STASH_STORE_USAGE,
	BUILTIN_STASH_EXPORT_USAGE,
	BUILTIN_STASH_IMPORT_USAGE,
	ખાલી
};

સ્થિર અચલ અક્ષર * અચલ git_stash_list_usage[] = {
	BUILTIN_STASH_LIST_USAGE,
	ખાલી
};

સ્થિર અચલ અક્ષર * અચલ git_stash_show_usage[] = {
	BUILTIN_STASH_SHOW_USAGE,
	ખાલી
};

સ્થિર અચલ અક્ષર * અચલ git_stash_drop_usage[] = {
	BUILTIN_STASH_DROP_USAGE,
	ખાલી
};

સ્થિર અચલ અક્ષર * અચલ git_stash_કાઢો_usage[] = {
	BUILTIN_STASH_કાઢો_USAGE,
	ખાલી
};

સ્થિર અચલ અક્ષર * અચલ git_stash_apply_usage[] = {
	BUILTIN_STASH_APPLY_USAGE,
	ખાલી
};

સ્થિર અચલ અક્ષર * અચલ git_stash_branch_usage[] = {
	BUILTIN_STASH_BRANCH_USAGE,
	ખાલી
};

સ્થિર અચલ અક્ષર * અચલ git_stash_clear_usage[] = {
	BUILTIN_STASH_CLEAR_USAGE,
	ખાલી
};

સ્થિર અચલ અક્ષર * અચલ git_stash_store_usage[] = {
	BUILTIN_STASH_STORE_USAGE,
	ખાલી
};

સ્થિર અચલ અક્ષર * અચલ git_stash_ધકેલો_usage[] = {
	BUILTIN_STASH_ધકેલો_USAGE,
	ખાલી
};

સ્થિર અચલ અક્ષર * અચલ git_stash_save_usage[] = {
	BUILTIN_STASH_SAVE_USAGE,
	ખાલી
};

સ્થિર અચલ અક્ષર * અચલ git_stash_export_usage[] = {
	BUILTIN_STASH_EXPORT_USAGE,
	ખાલી
};

સ્થિર અચલ અક્ષર * અચલ git_stash_import_usage[] = {
	BUILTIN_STASH_IMPORT_USAGE,
	ખાલી
};

સ્થિર અચલ અક્ષર ref_stash[] = "refs/stash";
સ્થિર સંરચના strbuf stash_index_path = STRBUF_INIT;

સ્થિર પૂર્ણાંક show_stat = 1;
સ્થિર પૂર્ણાંક show_patch;
સ્થિર પૂર્ણાંક show_include_untracked;
સ્થિર પૂર્ણાંક use_index;

/*
 * w_commit is set to the commit containing the working tree
 * b_commit is set to the base commit
 * i_commit is set to the commit containing the index tree
 * u_commit is set to the commit containing the untracked files tree
 * c_commit is set to the first parent (chain commit) when importing and is otherwise unset
 * w_tree is set to the working tree
 * b_tree is set to the base tree
 * i_tree is set to the index tree
 * u_tree is set to the untracked files tree
 */
સંરચના stash_info {
	સંરચના object_id w_commit;
	સંરચના object_id b_commit;
	સંરચના object_id i_commit;
	સંરચના object_id u_commit;
	સંરચના object_id c_commit;
	સંરચના object_id w_વૃક્ષ;
	સંરચના object_id b_વૃક્ષ;
	સંરચના object_id i_વૃક્ષ;
	સંરચના object_id u_વૃક્ષ;
	સંરચના strbuf revision;
	પૂર્ણાંક is_stash_ref;
	પૂર્ણાંક has_u;
};

#define STASH_INFO_INIT { \
	.revision = STRBUF_INIT, \
}

સ્થિર શૂન્ય free_stash_info(સંરચના stash_info *info)
{
	strbuf_release(&info->revision);
}

સ્થિર પૂર્ણાંક check_stash_topology(સંરચના repository *r, સંરચના commit *stash)
{
	સંરચના commit *p1, *p2, *p3 = ખાલી;

	/* stash must have two or three parents */
	જો (!stash->parents || !stash->parents->next ||
			(stash->parents->next->next && stash->parents->next->next->next))
		પાછા -1;
	p1 = stash->parents->item;
	p2 = stash->parents->next->item;
	જો (stash->parents->next->next)
		p3 = stash->parents->next->next->item;
	જો (repo_parse_commit(r, p1) || repo_parse_commit(r, p2) ||
			(p3 && repo_parse_commit(r, p3)))
		પાછા -1;
	/* p2 must have a single parent, p3 must have no parents */
	જો (!p2->parents || p2->parents->next || (p3 && p3->parents))
		પાછા -1;
	જો (repo_parse_commit(r, p2->parents->item))
		પાછા -1;
	/* p2^1 must equal p1 */
	જો (!oideq(&p1->object.oid, &p2->parents->item->object.oid))
		પાછા -1;

	પાછા 0;
}

સ્થિર શૂન્ય assert_stash_like(સંરચના stash_info *info, અચલ અક્ષર *revision)
{
	જો (get_oidf(&info->b_commit, "%s^1", revision) ||
	    get_oidf(&info->w_વૃક્ષ, "%s:", revision) ||
	    get_oidf(&info->b_વૃક્ષ, "%s^1:", revision) ||
	    get_oidf(&info->i_વૃક્ષ, "%s^2:", revision))
		die(_("'%s' is not a stash-like commit"), revision);
}

સ્થિર પૂર્ણાંક parse_stash_revision(સંરચના strbuf *revision, અચલ અક્ષર *commit, પૂર્ણાંક quiet)
{
	strbuf_reset(revision);
	જો (!commit) {
		જો (!refs_ref_exists(get_main_ref_store(the_repository), ref_stash)) {
			જો (!quiet)
				fprintf_ln(stderr, _("No stash entries found."));
			પાછા -1;
		}

		strbuf_addf(revision, "%s@{0}", ref_stash);
	} નહીંતર જો (strspn(commit, "0123456789") == strlen(commit)) {
		strbuf_addf(revision, "%s@{%s}", ref_stash, commit);
	} નહીંતર {
		strbuf_addstr(revision, commit);
	}
	પાછા 0;
}

સ્થિર પૂર્ણાંક get_stash_info(સંરચના stash_info *info, પૂર્ણાંક argc, અચલ અક્ષર **argv)
{
	પૂર્ણાંક ret;
	અક્ષર *end_of_rev;
	અક્ષર *expanded_ref;
	અચલ અક્ષર *revision;
	અચલ અક્ષર *commit = ખાલી;
	સંરચના object_id dummy;
	સંરચના strbuf symbolic = STRBUF_INIT;

	જો (argc > 1) {
		પૂર્ણાંક i;
		સંરચના strbuf refs_સંદેશ = STRBUF_INIT;

		માટે (i = 0; i < argc; i++)
			strbuf_addf(&refs_સંદેશ, " '%s'", argv[i]);

		fprintf_ln(stderr, _("Too many revisions specified:%s"),
			   refs_સંદેશ.buf);
		strbuf_release(&refs_સંદેશ);

		પાછા -1;
	}

	જો (argc == 1)
		commit = argv[0];

	strbuf_init(&info->revision, 0);
	જો (parse_stash_revision(&info->revision, commit, 0)) {
		પાછા -1;
	}

	revision = info->revision.buf;

	જો (repo_get_oid(the_repository, revision, &info->w_commit))
		પાછા error(_("%s is not a valid reference"), revision);

	assert_stash_like(info, revision);

	info->has_u = !get_oidf(&info->u_વૃક્ષ, "%s^3:", revision);

	end_of_rev = strchrnul(revision, '@');
	strbuf_add(&symbolic, revision, end_of_rev - revision);

	ret = repo_dwim_ref(the_repository, symbolic.buf, symbolic.len,
			    &dummy, &expanded_ref, 0);
	strbuf_release(&symbolic);
	પસંદગી (ret) {
	સ્થિતિ 0: /* Not found, but valid ref */
		info->is_stash_ref = 0;
		તોડો;
	સ્થિતિ 1:
		info->is_stash_ref = !strcmp(expanded_ref, ref_stash);
		તોડો;
	મૂળભૂત: /* Invalid or ambiguous */
		તોડો;
	}

	મુક્ત(expanded_ref);
	પાછા !(ret == 0 || ret == 1);
}

સ્થિર પૂર્ણાંક do_clear_stash(શૂન્ય)
{
	સંરચના object_id obj;
	જો (repo_get_oid(the_repository, ref_stash, &obj))
		પાછા 0;

	પાછા refs_delete_ref(get_main_ref_store(the_repository), ખાલી,
			       ref_stash, &obj, 0);
}

સ્થિર પૂર્ણાંક clear_stash(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
		       સંરચના repository *repo UNUSED)
{
	સંરચના option options[] = {
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_clear_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	જો (argc)
		પાછા error(_("git stash clear with arguments is "
			       "unimplemented"));

	પાછા do_clear_stash();
}

સ્થિર પૂર્ણાંક reset_વૃક્ષ(સંરચના object_id *i_વૃક્ષ, પૂર્ણાંક update, પૂર્ણાંક reset)
{
	પૂર્ણાંક nr_trees = 1;
	સંરચના unpack_trees_options opts;
	સંરચના વૃક્ષ_desc t[MAX_UNPACK_TREES];
	સંરચના વૃક્ષ *વૃક્ષ;
	સંરચના lock_file lock_file = LOCK_INIT;

	repo_read_index_preload(the_repository, ખાલી, 0);
	જો (refresh_index(the_repository->index, REFRESH_QUIET, ખાલી, ખાલી, ખાલી))
		પાછા -1;

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);

	memset(&opts, 0, કદ(opts));

	વૃક્ષ = repo_parse_વૃક્ષ_indirect(the_repository, i_વૃક્ષ);
	જો (repo_parse_વૃક્ષ(the_repository, વૃક્ષ))
		પાછા -1;

	init_વૃક્ષ_desc(t, &વૃક્ષ->object.oid, વૃક્ષ->buffer, વૃક્ષ->size);

	opts.head_idx = 1;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;
	opts.merge = 1;
	opts.reset = reset ? UNPACK_RESET_PROTECT_UNTRACKED : 0;
	opts.update = update;
	જો (update)
		opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
	opts.fn = oneway_merge;

	જો (unpack_trees(nr_trees, t, &opts))
		પાછા -1;

	જો (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
		પાછા error(_("unable to write new index file"));

	પાછા 0;
}

સ્થિર પૂર્ણાંક create_index_from_વૃક્ષ(અચલ સંરચના object_id *વૃક્ષ_id,
				  અચલ અક્ષર *index_path)
{
	પૂર્ણાંક nr_trees = 1;
	પૂર્ણાંક ret = 0;
	સંરચના unpack_trees_options opts;
	સંરચના વૃક્ષ_desc t[MAX_UNPACK_TREES];
	સંરચના વૃક્ષ *વૃક્ષ;
	સંરચના index_state dst_istate = INDEX_STATE_INIT(the_repository);
	સંરચના lock_file lock_file = LOCK_INIT;

	repo_read_index_preload(the_repository, ખાલી, 0);
	refresh_index(the_repository->index, REFRESH_QUIET, ખાલી, ખાલી, ખાલી);

	hold_lock_file_for_update(&lock_file, index_path, LOCK_DIE_ON_ERROR);

	memset(&opts, 0, કદ(opts));

	વૃક્ષ = repo_parse_વૃક્ષ_indirect(the_repository, વૃક્ષ_id);
	જો (!વૃક્ષ || repo_parse_વૃક્ષ(the_repository, વૃક્ષ)) {
		ret = -1;
		જાઓ done;
	}

	init_વૃક્ષ_desc(t, &વૃક્ષ->object.oid, વૃક્ષ->buffer, વૃક્ષ->size);

	opts.head_idx = 1;
	opts.src_index = the_repository->index;
	opts.dst_index = &dst_istate;
	opts.merge = 1;
	opts.reset = UNPACK_RESET_PROTECT_UNTRACKED;
	opts.fn = oneway_merge;

	જો (unpack_trees(nr_trees, t, &opts)) {
		ret = -1;
		જાઓ done;
	}

	જો (write_locked_index(&dst_istate, &lock_file, COMMIT_LOCK)) {
		ret = error(_("unable to write new index file"));
		જાઓ done;
	}

done:
	release_index(&dst_istate);
	જો (ret)
		rollback_lock_file(&lock_file);
	પાછા ret;
}

સ્થિર પૂર્ણાંક is_path_a_directory(અચલ અક્ષર *path)
{
	/*
	 * This function differs from abspath.c:is_directory() in that
	 * here we use lstat() instead of stat(); we do not want to
	 * follow symbolic links here.
	 */
	સંરચના stat st;
	પાછા (!lstat(path, &st) && S_ISDIR(st.st_mode));
}

સ્થિર શૂન્ય add_diff_to_buf(સંરચના diff_queue_struct *q,
			    સંરચના diff_options *options UNUSED,
			    શૂન્ય *data)
{
	પૂર્ણાંક i;

	માટે (i = 0; i < q->nr; i++) {
		જો (is_path_a_directory(q->queue[i]->one->path))
			ચાલુ;

		strbuf_addstr(data, q->queue[i]->one->path);

		/* NUL-terminate: will be fed to update-index -z */
		strbuf_addch(data, '\0');
	}
}

સ્થિર પૂર્ણાંક restore_untracked(સંરચના object_id *u_વૃક્ષ)
{
	પૂર્ણાંક res;
	સંરચના child_process cp = CHILD_PROCESS_INIT;

	/*
	 * We need to run restore files from a given index, but without
	 * affecting the current index, so we use GIT_INDEX_FILE with
	 * run_command to fork processes that will not interfere.
	 */
	cp.git_cmd = 1;
	strvec_ધકેલો(&cp.args, "read-tree");
	strvec_ધકેલો(&cp.args, oid_to_hex(u_વૃક્ષ));
	strvec_pushf(&cp.env, "GIT_INDEX_FILE=%s",
		     stash_index_path.buf);
	જો (run_command(&cp)) {
		remove_path(stash_index_path.buf);
		પાછા -1;
	}

	child_process_init(&cp);
	cp.git_cmd = 1;
	strvec_pushl(&cp.args, "checkout-index", "--all", ખાલી);
	strvec_pushf(&cp.env, "GIT_INDEX_FILE=%s",
		     stash_index_path.buf);

	res = run_command(&cp);
	remove_path(stash_index_path.buf);
	પાછા res;
}

સ્થિર શૂન્ય unstage_changes_unless_new(સંરચના object_id *orig_વૃક્ષ)
{
	/*
	 * When we enter this function, there has been a clean merge of
	 * relevant trees, and the merge logic always stages whatever merges
	 * cleanly.  We want to unstage those changes, unless it corresponds
	 * to a file that didn't exist as of orig_tree.
	 *
	 * However, if any SKIP_WORKTREE path is modified relative to
	 * orig_tree, then we want to clear the SKIP_WORKTREE bit and write
	 * it to the worktree before unstaging.
	 */

	સંરચના checkout state = CHECKOUT_INIT;
	સંરચના diff_options diff_opts;
	સંરચના lock_file lock = LOCK_INIT;
	પૂર્ણાંક i;

	/* If any entries have skip_worktree set, we'll have to check 'em out */
	state.force = 1;
	state.quiet = 1;
	state.refresh_cache = 1;
	state.istate = the_repository->index;

	/*
	 * Step 1: get a difference between orig_tree (which corresponding
	 * to the index before a merge was run) and the current index
	 * (reflecting the changes brought in by the merge).
	 */
	repo_diff_setup(the_repository, &diff_opts);
	diff_opts.flags.recursive = 1;
	diff_opts.detect_rename = 0;
	diff_opts.output_format = DIFF_FORMAT_NO_OUTPUT;
	diff_setup_done(&diff_opts);

	do_diff_cache(orig_વૃક્ષ, &diff_opts);
	diffcore_std(&diff_opts);

	/* Iterate over the paths that changed due to the merge... */
	માટે (i = 0; i < diff_queued_diff.nr; i++) {
		સંરચના diff_filepair *p;
		સંરચના cache_entry *ce;
		પૂર્ણાંક pos;

		/* Look up the path's position in the current index. */
		p = diff_queued_diff.queue[i];
		pos = index_name_pos(the_repository->index, p->two->path,
				     strlen(p->two->path));

		/*
		 * Step 2: Place changes in the working tree
		 *
		 * Stash is about restoring changes *to the working tree*.
		 * So if the merge successfully got a new version of some
		 * path, but left it out of the working tree, then clear the
		 * SKIP_WORKTREE bit and write it to the working tree.
		 */
		જો (pos >= 0 && ce_skip_worktree(the_repository->index->cache[pos])) {
			સંરચના stat st;

			ce = the_repository->index->cache[pos];
			જો (!lstat(ce->name, &st)) {
				/* Conflicting path present; relocate it */
				સંરચના strbuf new_path = STRBUF_INIT;
				પૂર્ણાંક fd;

				strbuf_addf(&new_path,
					    "%s.stash.XXXXXX", ce->name);
				fd = xmkstemp(new_path.buf);
				close(fd);
				printf(_("WARNING: Untracked file in way of "
					 "tracked file!  Renaming\n "
					 "           %s -> %s\n"
					 "         to make room.\n"),
				       ce->name, new_path.buf);
				જો (rename(ce->name, new_path.buf))
					die("Failed to move %s to %s",
					    ce->name, new_path.buf);
				strbuf_release(&new_path);
			}
			checkout_entry(ce, &state, ખાલી, ખાલી);
			ce->ce_flags &= ~CE_SKIP_WORKTREE;
		}

		/*
		 * Step 3: "unstage" changes, as long as they are still tracked
		 */
		જો (p->one->oid_valid) {
			/*
			 * Path existed in orig_tree; restore index entry
			 * from that tree in order to "unstage" the changes.
			 */
			પૂર્ણાંક option = ADD_CACHE_OK_TO_REPLACE;
			જો (pos < 0)
				option = ADD_CACHE_OK_TO_ADD;

			ce = make_cache_entry(the_repository->index,
					      p->one->mode,
					      &p->one->oid,
					      p->one->path,
					      0, 0);
			add_index_entry(the_repository->index, ce, option);
		}
	}
	diff_flush(&diff_opts);

	/*
	 * Step 4: write the new index to disk
	 */
	repo_hold_locked_index(the_repository, &lock, LOCK_DIE_ON_ERROR);
	જો (write_locked_index(the_repository->index, &lock,
			       COMMIT_LOCK | SKIP_IF_UNCHANGED))
		die(_("could not write index"));
}

સ્થિર ગણના stash_apply_result do_apply_stash(અચલ અક્ષર *prefix,
					      સંરચના stash_info *info,
					      પૂર્ણાંક index, પૂર્ણાંક quiet,
					      અચલ અક્ષર *label_ours,
					      અચલ અક્ષર *label_theirs,
					      અચલ અક્ષર *label_base)
{
	પૂર્ણાંક clean, ret;
	પૂર્ણાંક has_index = index;
	સંરચના merge_options o;
	સંરચના object_id c_વૃક્ષ;
	સંરચના object_id index_વૃક્ષ;
	સંરચના વૃક્ષ *head, *merge, *merge_base;
	સંરચના lock_file lock = LOCK_INIT;

	repo_read_index_preload(the_repository, ખાલી, 0);
	જો (repo_refresh_and_write_index(the_repository, REFRESH_QUIET, 0, 0,
					 ખાલી, ખાલી, ખાલી))
		પાછા error(_("could not write index"));

	જો (write_index_as_વૃક્ષ(&c_વૃક્ષ, the_repository->index,
				repo_get_index_file(the_repository), 0, ખાલી))
		પાછા error(_("cannot apply a stash in the middle of a merge"));

	init_ui_merge_options(&o, the_repository);

	જો (quiet)
		o.verbosity = 0;

	જો (index) {
		જો (oideq(&info->b_વૃક્ષ, &info->i_વૃક્ષ) ||
		    oideq(&c_વૃક્ષ, &info->i_વૃક્ષ)) {
			has_index = 0;
		} નહીંતર {
			સંરચના merge_result result = { 0 };

			o.branch1 = "Current index";
			o.branch2 = "Stashed index changes";
			o.ancestor = "Stash base";

			head = lookup_વૃક્ષ(o.repo, &c_વૃક્ષ);
			merge = lookup_વૃક્ષ(o.repo, &info->i_વૃક્ષ);
			merge_base = lookup_વૃક્ષ(o.repo, &info->b_વૃક્ષ);

			merge_incore_nonrecursive(&o, merge_base, head, merge,
						  &result);

			જો (result.clean < 0) {
				merge_finalize(&o, &result);
				પાછા error(_("index merge failed"));
			} નહીંતર જો (!result.clean) {
				merge_finalize(&o, &result);
				પાછા error(_("conflicts in index. "
					       "Try without --index."));
			} નહીંતર {
				oidcpy(&index_વૃક્ષ, &result.વૃક્ષ->object.oid);
				merge_finalize(&o, &result);
			}
		}
	}

	o.branch1 = label_ours ? label_ours : "Updated upstream";
	o.branch2 = label_theirs ? label_theirs : "Stashed changes";
	o.ancestor = label_base ? label_base : "Stash base";

	જો (oideq(&info->b_વૃક્ષ, &c_વૃક્ષ))
		o.branch1 = "Version stash was based on";

	જો (o.verbosity >= 3)
		printf_ln(_("Merging %s with %s"), o.branch1, o.branch2);

	head = lookup_વૃક્ષ(o.repo, &c_વૃક્ષ);
	merge = lookup_વૃક્ષ(o.repo, &info->w_વૃક્ષ);
	merge_base = lookup_વૃક્ષ(o.repo, &info->b_વૃક્ષ);

	repo_hold_locked_index(o.repo, &lock, LOCK_DIE_ON_ERROR);
	clean = merge_ort_nonrecursive(&o, head, merge, merge_base);

	/*
	 * If 'clean' >= 0, reverse the value for 'ret' so 'ret' is 0 when the
	 * merge was clean, and 1 if the merge was unclean or a negative value
	 * if it encountered an error.
	 */
	ret = clean >= 0 ? !clean : clean;

	જો (ret < 0)
		rollback_lock_file(&lock);
	નહીંતર જો (write_locked_index(o.repo->index, &lock,
				      COMMIT_LOCK | SKIP_IF_UNCHANGED))
		ret = error(_("could not write index"));

	જો (ret) {
		repo_rerere(the_repository, 0);

		જો (index)
			fprintf_ln(stderr, _("Index was not unstashed."));

		જાઓ restore_untracked;
	}

	જો (has_index) {
		જો (reset_વૃક્ષ(&index_વૃક્ષ, 0, 0))
			ret = -1;
	} નહીંતર {
		unstage_changes_unless_new(&c_વૃક્ષ);
	}

restore_untracked:
	જો (info->has_u && restore_untracked(&info->u_વૃક્ષ))
		ret = error(_("could not restore untracked files from stash"));

	જો (!quiet) {
		સંરચના child_process cp = CHILD_PROCESS_INIT;

		/*
		 * Status is quite simple and could be replaced with calls to
		 * wt_status in the future, but it adds complexities which may
		 * require more tests.
		 */
		cp.git_cmd = 1;
		cp.dir = prefix;
		strvec_pushf(&cp.env, GIT_WORK_વૃક્ષ_ENVIRONMENT"=%s",
			     absolute_path(repo_get_work_વૃક્ષ(the_repository)));
		strvec_pushf(&cp.env, GIT_DIR_ENVIRONMENT"=%s",
			     absolute_path(repo_get_git_dir(the_repository)));
		strvec_ધકેલો(&cp.args, "status");
		run_command(&cp);
	}

	પાછા ret;
}

સ્થિર પૂર્ણાંક apply_stash(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
		       સંરચના repository *repo UNUSED)
{
	પૂર્ણાંક ret = -1;
	પૂર્ણાંક quiet = 0;
	પૂર્ણાંક index = use_index;
	અચલ અક્ષર *label_ours = ખાલી, *label_theirs = ખાલી, *label_base = ખાલી;
	સંરચના stash_info info = STASH_INFO_INIT;
	સંરચના option options[] = {
		OPT__QUIET(&quiet, N_("be quiet, only report errors")),
		OPT_BOOL(0, "index", &index,
			 N_("attempt to recreate the index")),
		OPT_STRING(0, "label-ours", &label_ours, N_("label"),
			   N_("label for the upstream side in conflict markers")),
		OPT_STRING(0, "label-theirs", &label_theirs, N_("label"),
			   N_("label for the stashed side in conflict markers")),
		OPT_STRING(0, "label-base", &label_base, N_("label"),
			   N_("label for the base in diff3 conflict markers")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_apply_usage, 0);

	જો (get_stash_info(&info, argc, argv))
		જાઓ cleanup;

	ret = do_apply_stash(prefix, &info, index, quiet,
			     label_ours, label_theirs, label_base);
cleanup:
	free_stash_info(&info);
	પાછા ret;
}

સ્થિર પૂર્ણાંક reject_reflog_ent(અચલ અક્ષર *refname UNUSED,
			     સંરચના object_id *ooid UNUSED,
			     સંરચના object_id *noid UNUSED,
			     અચલ અક્ષર *email UNUSED,
			     timestamp_t timestamp UNUSED,
			     પૂર્ણાંક tz UNUSED, અચલ અક્ષર *message UNUSED,
			     શૂન્ય *cb_data UNUSED)
{
	પાછા 1;
}

સ્થિર પૂર્ણાંક reflog_is_empty(અચલ અક્ષર *refname)
{
	પાછા !refs_for_each_reflog_ent(get_main_ref_store(the_repository),
					 refname, reject_reflog_ent, ખાલી);
}

સ્થિર પૂર્ણાંક do_drop_stash(સંરચના stash_info *info, પૂર્ણાંક quiet)
{
	જો (!reflog_delete(info->revision.buf,
			   EXPIRE_REFLOGS_REWRITE | EXPIRE_REFLOGS_UPDATE_REF,
			   0)) {
		જો (!quiet)
			printf_ln(_("Dropped %s (%s)"), info->revision.buf,
				  oid_to_hex(&info->w_commit));
	} નહીંતર {
		પાછા error(_("%s: Could not drop stash entry"),
			     info->revision.buf);
	}

	જો (reflog_is_empty(ref_stash))
		do_clear_stash();

	પાછા 0;
}

સ્થિર પૂર્ણાંક get_stash_info_assert(સંરચના stash_info *info, પૂર્ણાંક argc,
				 અચલ અક્ષર **argv)
{
	પૂર્ણાંક ret = get_stash_info(info, argc, argv);

	જો (ret < 0)
		પાછા ret;

	જો (!info->is_stash_ref)
		પાછા error(_("'%s' is not a stash reference"), info->revision.buf);

	પાછા 0;
}

સ્થિર પૂર્ણાંક drop_stash(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
		      સંરચના repository *repo UNUSED)
{
	પૂર્ણાંક ret = -1;
	પૂર્ણાંક quiet = 0;
	સંરચના stash_info info = STASH_INFO_INIT;
	સંરચના option options[] = {
		OPT__QUIET(&quiet, N_("be quiet, only report errors")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_drop_usage, 0);

	જો (get_stash_info_assert(&info, argc, argv))
		જાઓ cleanup;

	ret = do_drop_stash(&info, quiet);
cleanup:
	free_stash_info(&info);
	પાછા ret;
}

સ્થિર પૂર્ણાંક કાઢો_stash(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
		     સંરચના repository *repo UNUSED)
{
	પૂર્ણાંક ret = -1;
	પૂર્ણાંક index = use_index;
	પૂર્ણાંક quiet = 0;
	સંરચના stash_info info = STASH_INFO_INIT;
	સંરચના option options[] = {
		OPT__QUIET(&quiet, N_("be quiet, only report errors")),
		OPT_BOOL(0, "index", &index,
			 N_("attempt to recreate the index")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_કાઢો_usage, 0);

	જો (get_stash_info_assert(&info, argc, argv))
		જાઓ cleanup;

	જો ((ret = do_apply_stash(prefix, &info, index, quiet,
				  ખાલી, ખાલી, ખાલી)))
		printf_ln(_("The stash entry is kept in case "
			    "you need it again."));
	નહીંતર
		ret = do_drop_stash(&info, quiet);

cleanup:
	free_stash_info(&info);
	પાછા ret;
}

સ્થિર પૂર્ણાંક branch_stash(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
			સંરચના repository *repo UNUSED)
{
	પૂર્ણાંક ret = -1;
	અચલ અક્ષર *branch = ખાલી;
	સંરચના stash_info info = STASH_INFO_INIT;
	સંરચના child_process cp = CHILD_PROCESS_INIT;
	સંરચના option options[] = {
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_branch_usage, 0);

	જો (!argc) {
		fprintf_ln(stderr, _("No branch name specified"));
		પાછા -1;
	}

	branch = argv[0];

	જો (get_stash_info(&info, argc - 1, argv + 1))
		જાઓ cleanup;

	cp.git_cmd = 1;
	strvec_pushl(&cp.args, "checkout", "-b", ખાલી);
	strvec_ધકેલો(&cp.args, branch);
	strvec_ધકેલો(&cp.args, oid_to_hex(&info.b_commit));
	ret = run_command(&cp);
	જો (!ret)
		ret = do_apply_stash(prefix, &info, 1, 0,
				     ખાલી, ખાલી, ખાલી);
	જો (!ret && info.is_stash_ref)
		ret = do_drop_stash(&info, 0);

cleanup:
	free_stash_info(&info);
	પાછા ret;
}

સ્થિર પૂર્ણાંક list_stash(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
		      સંરચના repository *repo UNUSED)
{
	સંરચના child_process cp = CHILD_PROCESS_INIT;
	સંરચના option options[] = {
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_list_usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT);

	જો (!refs_ref_exists(get_main_ref_store(the_repository), ref_stash))
		પાછા 0;

	cp.git_cmd = 1;
	strvec_pushl(&cp.args, "log", "--format=%gd: %gs", "-g",
		     "--first-parent", ખાલી);
	strvec_pushv(&cp.args, argv);
	strvec_ધકેલો(&cp.args, ref_stash);
	strvec_ધકેલો(&cp.args, "--");
	પાછા run_command(&cp);
}

સ્થિર પૂર્ણાંક git_stash_config(અચલ અક્ષર *var, અચલ અક્ષર *કિંમત,
			    અચલ સંરચના config_context *ctx, શૂન્ય *cb)
{
	જો (!strcmp(var, "stash.showstat")) {
		show_stat = git_config_bool(var, કિંમત);
		પાછા 0;
	}
	જો (!strcmp(var, "stash.showpatch")) {
		show_patch = git_config_bool(var, કિંમત);
		પાછા 0;
	}
	જો (!strcmp(var, "stash.showincludeuntracked")) {
		show_include_untracked = git_config_bool(var, કિંમત);
		પાછા 0;
	}
	જો (!strcmp(var, "stash.index")) {
		use_index = git_config_bool(var, કિંમત);
		પાછા 0;
	}
	પાછા git_diff_basic_config(var, કિંમત, ctx, cb);
}

સ્થિર શૂન્ય diff_include_untracked(અચલ સંરચના stash_info *info, સંરચના diff_options *diff_opt)
{
	અચલ સંરચના object_id *oid[] = { &info->w_commit, &info->u_વૃક્ષ };
	સંરચના વૃક્ષ *વૃક્ષ[ARRAY_SIZE(oid)];
	સંરચના વૃક્ષ_desc વૃક્ષ_desc[ARRAY_SIZE(oid)];
	સંરચના unpack_trees_options unpack_વૃક્ષ_opt = { 0 };

	માટે (size_t i = 0; i < ARRAY_SIZE(oid); i++) {
		વૃક્ષ[i] = repo_parse_વૃક્ષ_indirect(the_repository, oid[i]);
		જો (repo_parse_વૃક્ષ(the_repository, વૃક્ષ[i]) < 0)
			die(_("failed to parse tree"));
		init_વૃક્ષ_desc(&વૃક્ષ_desc[i], &વૃક્ષ[i]->object.oid,
			       વૃક્ષ[i]->buffer, વૃક્ષ[i]->size);
	}

	unpack_વૃક્ષ_opt.head_idx = -1;
	unpack_વૃક્ષ_opt.src_index = the_repository->index;
	unpack_વૃક્ષ_opt.dst_index = the_repository->index;
	unpack_વૃક્ષ_opt.merge = 1;
	unpack_વૃક્ષ_opt.fn = stash_worktree_untracked_merge;

	જો (unpack_trees(ARRAY_SIZE(વૃક્ષ_desc), વૃક્ષ_desc, &unpack_વૃક્ષ_opt))
		die(_("failed to unpack trees"));

	do_diff_cache(&info->b_commit, diff_opt);
}

સ્થિર પૂર્ણાંક show_stash(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
		      સંરચના repository *repo UNUSED)
{
	પૂર્ણાંક i;
	પૂર્ણાંક ret = -1;
	સંરચના stash_info info = STASH_INFO_INIT;
	સંરચના rev_info rev;
	સંરચના strvec stash_args = STRVEC_INIT;
	સંરચના strvec revision_args = STRVEC_INIT;
	ગણના {
		UNTRACKED_NONE,
		UNTRACKED_INCLUDE,
		UNTRACKED_ONLY
	} show_untracked = show_include_untracked ? UNTRACKED_INCLUDE : UNTRACKED_NONE;
	સંરચના option options[] = {
		OPT_SET_INT('u', "include-untracked", &show_untracked,
			    N_("include untracked files in the stash"),
			    UNTRACKED_INCLUDE),
		OPT_SET_INT_F(0, "only-untracked", &show_untracked,
			      N_("only show untracked files in the stash"),
			      UNTRACKED_ONLY, PARSE_OPT_NONEG),
		OPT_END()
	};
	પૂર્ણાંક do_usage = 0;

	init_diff_ui_defaults();
	repo_config(the_repository, git_diff_ui_config, ખાલી);
	repo_init_revisions(the_repository, &rev, prefix);

	argc = parse_options(argc, argv, prefix, options, git_stash_show_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	strvec_ધકેલો(&revision_args, argv[0]);
	માટે (i = 1; i < argc; i++) {
		જો (argv[i][0] != '-')
			strvec_ધકેલો(&stash_args, argv[i]);
		નહીંતર
			strvec_ધકેલો(&revision_args, argv[i]);
	}

	જો (get_stash_info(&info, stash_args.nr, stash_args.v))
		જાઓ cleanup;

	/*
	 * The config settings are applied only if there are not passed
	 * any options.
	 */
	જો (revision_args.nr == 1) {
		જો (show_stat)
			rev.diffopt.output_format = DIFF_FORMAT_DIFFSTAT;

		જો (show_patch)
			rev.diffopt.output_format |= DIFF_FORMAT_PATCH;

		જો (!show_stat && !show_patch) {
			ret = 0;
			જાઓ cleanup;
		}
	}

	setup_revisions_from_strvec(&revision_args, &rev, ખાલી);
	જો (revision_args.nr > 1)
		જાઓ usage;
	જો (!rev.diffopt.output_format) {
		rev.diffopt.output_format = DIFF_FORMAT_PATCH;
		diff_setup_done(&rev.diffopt);
	}

	rev.diffopt.flags.recursive = 1;
	setup_diff_pager(&rev.diffopt);
	પસંદગી (show_untracked) {
	સ્થિતિ UNTRACKED_NONE:
		diff_વૃક્ષ_oid(&info.b_commit, &info.w_commit, "", &rev.diffopt);
		તોડો;
	સ્થિતિ UNTRACKED_ONLY:
		જો (info.has_u)
			diff_મૂળ_વૃક્ષ_oid(&info.u_વૃક્ષ, "", &rev.diffopt);
		તોડો;
	સ્થિતિ UNTRACKED_INCLUDE:
		જો (info.has_u)
			diff_include_untracked(&info, &rev.diffopt);
		નહીંતર
			diff_વૃક્ષ_oid(&info.b_commit, &info.w_commit, "", &rev.diffopt);
		તોડો;
	}
	log_વૃક્ષ_diff_flush(&rev);

	ret = diff_result_code(&rev);

cleanup:
	strvec_clear(&revision_args);
	strvec_clear(&stash_args);
	free_stash_info(&info);
	release_revisions(&rev);
	જો (do_usage)
		usage_with_options(git_stash_show_usage, options);
	પાછા ret;
usage:
	do_usage = 1;
	જાઓ cleanup;
}

સ્થિર પૂર્ણાંક do_store_stash(અચલ સંરચના object_id *w_commit, અચલ અક્ષર *stash_સંદેશ,
			  પૂર્ણાંક quiet)
{
	સંરચના stash_info info;
	અક્ષર revision[GIT_MAX_HEXSZ];

	oid_to_hex_r(revision, w_commit);
	assert_stash_like(&info, revision);

	જો (!stash_સંદેશ)
		stash_સંદેશ = "Created via \"git stash store\".";

	જો (refs_update_ref(get_main_ref_store(the_repository), stash_સંદેશ, ref_stash, w_commit, ખાલી,
			    REF_FORCE_CREATE_REFLOG,
			    quiet ? UPDATE_REFS_QUIET_ON_ERR :
			    UPDATE_REFS_સંદેશ_ON_ERR)) {
		જો (!quiet) {
			fprintf_ln(stderr, _("Cannot update %s with %s"),
				   ref_stash, oid_to_hex(w_commit));
		}
		પાછા -1;
	}

	પાછા 0;
}

સ્થિર પૂર્ણાંક store_stash(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
		       સંરચના repository *repo UNUSED)
{
	પૂર્ણાંક quiet = 0;
	અચલ અક્ષર *stash_સંદેશ = ખાલી;
	સંરચના object_id obj;
	સંરચના option options[] = {
		OPT__QUIET(&quiet, N_("be quiet")),
		OPT_STRING('m', "message", &stash_સંદેશ, "message",
			   N_("stash message")),
		OPT_END()
	};
	પૂર્ણાંક ret;

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_store_usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT);

	જો (argc != 1) {
		જો (!quiet)
			fprintf_ln(stderr, _("\"git stash store\" requires one "
					     "<commit> argument"));
		પાછા -1;
	}

	જો (repo_get_oid_with_flags(the_repository, argv[0], &obj,
				    quiet ? GET_OID_QUIETLY : 0)) {
		જો (!quiet)
			fprintf_ln(stderr, _("Cannot update %s with %s"),
					     ref_stash, argv[0]);
		ret = -1;
		જાઓ out;
	}

	ret = do_store_stash(&obj, stash_સંદેશ, quiet);

out:
	પાછા ret;
}

સ્થિર શૂન્ય add_pathspecs(સંરચના strvec *args,
			  અચલ સંરચના pathspec *ps) {
	પૂર્ણાંક i;

	માટે (i = 0; i < ps->nr; i++)
		strvec_ધકેલો(args, ps->વસ્તુઓ[i].original);
}

/*
 * `untracked_files` will be filled with the names of untracked files.
 * The return value is:
 *
 * = 0 if there are not any untracked files
 * > 0 if there are untracked files
 */
સ્થિર પૂર્ણાંક get_untracked_files(અચલ સંરચના pathspec *ps, પૂર્ણાંક include_untracked,
			       સંરચના strbuf *untracked_files)
{
	પૂર્ણાંક i;
	પૂર્ણાંક found = 0;
	સંરચના dir_struct dir = DIR_INIT;

	જો (include_untracked != INCLUDE_ALL_FILES)
		setup_standard_excludes(&dir);

	fill_directory(&dir, the_repository->index, ps);
	માટે (i = 0; i < dir.nr; i++) {
		સંરચના dir_entry *ent = dir.entries[i];
		found++;
		strbuf_addstr(untracked_files, ent->name);
		/* NUL-terminate: will be fed to update-index -z */
		strbuf_addch(untracked_files, '\0');
	}

	dir_clear(&dir);
	પાછા found;
}

/*
 * The return value of `check_changes_tracked_files()` can be:
 *
 * < 0 if there was an error
 * = 0 if there are no changes.
 * > 0 if there are changes.
 */
સ્થિર પૂર્ણાંક check_changes_tracked_files(અચલ સંરચના pathspec *ps)
{
	સંરચના rev_info rev;
	સંરચના object_id dummy;
	પૂર્ણાંક ret = 0;

	/* No initial commit. */
	જો (repo_get_oid(the_repository, "HEAD", &dummy))
		પાછા -1;

	જો (repo_read_index(the_repository) < 0)
		પાછા -1;

	repo_init_revisions(the_repository, &rev, ખાલી);
	copy_pathspec(&rev.prune_data, ps);

	rev.diffopt.flags.quick = 1;
	rev.diffopt.flags.ignore_submodules = 1;
	rev.abbrev = 0;

	add_head_to_pending(&rev);
	diff_setup_done(&rev.diffopt);

	run_diff_index(&rev, DIFF_INDEX_CACHED);
	જો (diff_result_code(&rev)) {
		ret = 1;
		જાઓ done;
	}

	run_diff_files(&rev, 0);
	જો (diff_result_code(&rev)) {
		ret = 1;
		જાઓ done;
	}

done:
	release_revisions(&rev);
	પાછા ret;
}

/*
 * The function will fill `untracked_files` with the names of untracked files
 * It will return 1 if there were any changes and 0 if there were not.
 */
સ્થિર પૂર્ણાંક check_changes(અચલ સંરચના pathspec *ps, પૂર્ણાંક include_untracked,
			 સંરચના strbuf *untracked_files)
{
	પૂર્ણાંક ret = 0;
	જો (check_changes_tracked_files(ps))
		ret = 1;

	જો (include_untracked && get_untracked_files(ps, include_untracked,
						     untracked_files))
		ret = 1;

	પાછા ret;
}

સ્થિર પૂર્ણાંક save_untracked_files(સંરચના stash_info *info, સંરચના strbuf *સંદેશ,
				સંરચના strbuf *files)
{
	પૂર્ણાંક ret = 0;
	સંરચના strbuf untracked_સંદેશ = STRBUF_INIT;
	સંરચના child_process cp_upd_index = CHILD_PROCESS_INIT;
	સંરચના index_state istate = INDEX_STATE_INIT(the_repository);

	cp_upd_index.git_cmd = 1;
	strvec_pushl(&cp_upd_index.args, "update-index", "-z", "--add",
		     "--remove", "--stdin", ખાલી);
	strvec_pushf(&cp_upd_index.env, "GIT_INDEX_FILE=%s",
			 stash_index_path.buf);

	strbuf_addf(&untracked_સંદેશ, "untracked files on %s\n", સંદેશ->buf);
	જો (pipe_command(&cp_upd_index, files->buf, files->len, ખાલી, 0,
			 ખાલી, 0)) {
		ret = -1;
		જાઓ done;
	}

	જો (write_index_as_વૃક્ષ(&info->u_વૃક્ષ, &istate, stash_index_path.buf, 0,
				ખાલી)) {
		ret = -1;
		જાઓ done;
	}

	જો (commit_વૃક્ષ(untracked_સંદેશ.buf, untracked_સંદેશ.len,
			&info->u_વૃક્ષ, ખાલી, &info->u_commit, ખાલી, ખાલી)) {
		ret = -1;
		જાઓ done;
	}

done:
	release_index(&istate);
	strbuf_release(&untracked_સંદેશ);
	remove_path(stash_index_path.buf);
	પાછા ret;
}

સ્થિર પૂર્ણાંક stash_staged(સંરચના stash_info *info, સંરચના strbuf *out_patch,
			પૂર્ણાંક quiet)
{
	પૂર્ણાંક ret = 0;
	સંરચના child_process cp_diff_વૃક્ષ = CHILD_PROCESS_INIT;
	સંરચના index_state istate = INDEX_STATE_INIT(the_repository);

	જો (write_index_as_વૃક્ષ(&info->w_વૃક્ષ, &istate, the_repository->index_file,
				0, ખાલી)) {
		ret = -1;
		જાઓ done;
	}

	cp_diff_વૃક્ષ.git_cmd = 1;
	strvec_pushl(&cp_diff_વૃક્ષ.args, "diff-tree", "-p", "--binary",
		     "--no-color",
		     "-U1", "HEAD", oid_to_hex(&info->w_વૃક્ષ), "--", ખાલી);
	જો (pipe_command(&cp_diff_વૃક્ષ, ખાલી, 0, out_patch, 0, ખાલી, 0)) {
		ret = -1;
		જાઓ done;
	}

	જો (!out_patch->len) {
		જો (!quiet)
			fprintf_ln(stderr, _("No staged changes"));
		ret = 1;
	}

done:
	release_index(&istate);
	પાછા ret;
}

સ્થિર પૂર્ણાંક stash_patch(સંરચના stash_info *info, અચલ સંરચના pathspec *ps,
		       સંરચના strbuf *out_patch, પૂર્ણાંક quiet,
		       સંરચના interactive_options *interactive_opts)
{
	પૂર્ણાંક ret = 0;
	સંરચના child_process cp_diff_વૃક્ષ = CHILD_PROCESS_INIT;
	સંરચના commit *head_commit;
	અચલ સંરચના object_id *head_વૃક્ષ;
	સંરચના index_state istate = INDEX_STATE_INIT(the_repository);
	અક્ષર *old_index_env = ખાલી, *old_repo_index_file;

	remove_path(stash_index_path.buf);

	head_commit = lookup_commit(the_repository, &info->b_commit);
	જો (!head_commit || repo_parse_commit(the_repository, head_commit)) {
		ret = -1;
		જાઓ done;
	}
	head_વૃક્ષ = get_commit_વૃક્ષ_oid(head_commit);
	જો (!head_વૃક્ષ) {
		ret = -1;
		જાઓ done;
	}

	જો (create_index_from_વૃક્ષ(head_વૃક્ષ, stash_index_path.buf)) {
		ret = -1;
		જાઓ done;
	}

	/* Find out what the user wants. */
	old_repo_index_file = the_repository->index_file;
	the_repository->index_file = stash_index_path.buf;
	old_index_env = xstrdup_or_null(getenv(INDEX_ENVIRONMENT));
	setenv(INDEX_ENVIRONMENT, the_repository->index_file, 1);

	ret = !!run_add_p(the_repository, ADD_P_STASH, interactive_opts, ખાલી, ps, 0);

	the_repository->index_file = old_repo_index_file;
	જો (old_index_env && *old_index_env)
		setenv(INDEX_ENVIRONMENT, old_index_env, 1);
	નહીંતર
		unsetenv(INDEX_ENVIRONMENT);
	FREE_AND_NULL(old_index_env);

	/* State of the working tree. */
	જો (write_index_as_વૃક્ષ(&info->w_વૃક્ષ, &istate, stash_index_path.buf, 0,
				ખાલી)) {
		ret = -1;
		જાઓ done;
	}

	cp_diff_વૃક્ષ.git_cmd = 1;
	strvec_pushl(&cp_diff_વૃક્ષ.args, "diff-tree", "-p", "-U1", "HEAD",
		     "--no-color",
		     oid_to_hex(&info->w_વૃક્ષ), "--", ખાલી);
	જો (pipe_command(&cp_diff_વૃક્ષ, ખાલી, 0, out_patch, 0, ખાલી, 0)) {
		ret = -1;
		જાઓ done;
	}

	જો (!out_patch->len) {
		જો (!quiet)
			fprintf_ln(stderr, _("No changes selected"));
		ret = 1;
	}

done:
	release_index(&istate);
	remove_path(stash_index_path.buf);
	પાછા ret;
}

સ્થિર પૂર્ણાંક stash_working_વૃક્ષ(સંરચના stash_info *info, અચલ સંરચના pathspec *ps)
{
	પૂર્ણાંક ret = 0;
	સંરચના rev_info rev;
	સંરચના child_process cp_upd_index = CHILD_PROCESS_INIT;
	સંરચના strbuf diff_output = STRBUF_INIT;
	સંરચના index_state istate = INDEX_STATE_INIT(the_repository);

	repo_init_revisions(the_repository, &rev, ખાલી);
	copy_pathspec(&rev.prune_data, ps);

	set_alternate_index_output(stash_index_path.buf);
	જો (reset_વૃક્ષ(&info->i_વૃક્ષ, 0, 0)) {
		ret = -1;
		જાઓ done;
	}
	set_alternate_index_output(ખાલી);

	rev.diffopt.output_format = DIFF_FORMAT_CALLBACK;
	rev.diffopt.format_callback = add_diff_to_buf;
	rev.diffopt.format_callback_data = &diff_output;

	જો (repo_read_index_preload(the_repository, &rev.diffopt.pathspec, 0) < 0) {
		ret = -1;
		જાઓ done;
	}

	add_pending_object(&rev, parse_object(the_repository, &info->b_commit),
			   "");
	run_diff_index(&rev, 0);

	cp_upd_index.git_cmd = 1;
	strvec_pushl(&cp_upd_index.args, "update-index",
		     "--ignore-skip-worktree-entries",
		     "-z", "--add", "--remove", "--stdin", ખાલી);
	strvec_pushf(&cp_upd_index.env, "GIT_INDEX_FILE=%s",
		     stash_index_path.buf);

	જો (pipe_command(&cp_upd_index, diff_output.buf, diff_output.len,
			 ખાલી, 0, ખાલી, 0)) {
		ret = -1;
		જાઓ done;
	}

	જો (write_index_as_વૃક્ષ(&info->w_વૃક્ષ, &istate, stash_index_path.buf, 0,
				ખાલી)) {
		ret = -1;
		જાઓ done;
	}

done:
	release_index(&istate);
	release_revisions(&rev);
	strbuf_release(&diff_output);
	remove_path(stash_index_path.buf);
	પાછા ret;
}

સ્થિર પૂર્ણાંક do_create_stash(અચલ સંરચના pathspec *ps, સંરચના strbuf *stash_સંદેશ_buf,
			   પૂર્ણાંક include_untracked, પૂર્ણાંક patch_mode,
			   સંરચના interactive_options *interactive_opts,
			   પૂર્ણાંક only_staged, સંરચના stash_info *info, સંરચના strbuf *patch,
			   પૂર્ણાંક quiet)
{
	પૂર્ણાંક ret = 0;
	પૂર્ણાંક flags = 0;
	પૂર્ણાંક untracked_commit_option = 0;
	અચલ અક્ષર *head_short_sha1 = ખાલી;
	અચલ અક્ષર *branch_ref = ખાલી;
	અચલ અક્ષર *branch_name = "(no branch)";
	અક્ષર *branch_name_buf = ખાલી;
	સંરચના commit *head_commit = ખાલી;
	સંરચના commit_list *parents = ખાલી;
	સંરચના strbuf સંદેશ = STRBUF_INIT;
	સંરચના strbuf commit_વૃક્ષ_label = STRBUF_INIT;
	સંરચના strbuf untracked_files = STRBUF_INIT;

	prepare_fallback_ident("git stash", "git@stash");

	repo_read_index_preload(the_repository, ખાલી, 0);
	જો (repo_refresh_and_write_index(the_repository, REFRESH_QUIET, 0, 0,
					 ખાલી, ખાલી, ખાલી) < 0) {
		ret = error(_("could not write index"));
		જાઓ done;
	}

	જો (repo_get_oid(the_repository, "HEAD", &info->b_commit)) {
		જો (!quiet)
			fprintf_ln(stderr, _("You do not have "
					     "the initial commit yet"));
		ret = -1;
		જાઓ done;
	} નહીંતર {
		head_commit = lookup_commit(the_repository, &info->b_commit);
	}

	જો (!check_changes(ps, include_untracked, &untracked_files)) {
		ret = 1;
		જાઓ done;
	}

	branch_ref = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
					     "HEAD", 0, ખાલી, &flags);

	જો (flags & REF_ISSYMREF) {
		જો (skip_prefix(branch_ref, "refs/heads/", &branch_name))
			branch_name = branch_name_buf = xstrdup(branch_name);
	}

	head_short_sha1 = repo_find_unique_abbrev(the_repository,
						  &head_commit->object.oid,
						  DEFAULT_ABBREV);
	strbuf_addf(&સંદેશ, "%s: %s ", branch_name, head_short_sha1);
	pp_commit_easy(CMIT_FMT_ONELINE, head_commit, &સંદેશ);

	strbuf_addf(&commit_વૃક્ષ_label, "index on %s\n", સંદેશ.buf);
	commit_list_ઉમેરો(head_commit, &parents);
	જો (write_index_as_વૃક્ષ(&info->i_વૃક્ષ, the_repository->index,
				repo_get_index_file(the_repository), 0, ખાલી) ||
	    commit_વૃક્ષ(commit_વૃક્ષ_label.buf, commit_વૃક્ષ_label.len,
			&info->i_વૃક્ષ, parents, &info->i_commit, ખાલી, ખાલી)) {
		જો (!quiet)
			fprintf_ln(stderr, _("Cannot save the current "
					     "index state"));
		ret = -1;
		જાઓ done;
	}

	commit_list_free(parents);
	parents = ખાલી;

	જો (include_untracked) {
		જો (save_untracked_files(info, &સંદેશ, &untracked_files)) {
			જો (!quiet)
				fprintf_ln(stderr, _("Cannot save "
						     "the untracked files"));
			ret = -1;
			જાઓ done;
		}
		untracked_commit_option = 1;
	}
	જો (patch_mode) {
		ret = stash_patch(info, ps, patch, quiet, interactive_opts);
		જો (ret < 0) {
			જો (!quiet)
				fprintf_ln(stderr, _("Cannot save the current "
						     "worktree state"));
			જાઓ done;
		} નહીંતર જો (ret > 0) {
			જાઓ done;
		}
	} નહીંતર જો (only_staged) {
		ret = stash_staged(info, patch, quiet);
		જો (ret < 0) {
			જો (!quiet)
				fprintf_ln(stderr, _("Cannot save the current "
						     "staged state"));
			જાઓ done;
		} નહીંતર જો (ret > 0) {
			જાઓ done;
		}
	} નહીંતર {
		જો (stash_working_વૃક્ષ(info, ps)) {
			જો (!quiet)
				fprintf_ln(stderr, _("Cannot save the current "
						     "worktree state"));
			ret = -1;
			જાઓ done;
		}
	}

	જો (!stash_સંદેશ_buf->len)
		strbuf_addf(stash_સંદેશ_buf, "WIP on %s", સંદેશ.buf);
	નહીંતર
		strbuf_insertf(stash_સંદેશ_buf, 0, "On %s: ", branch_name);

	જો (untracked_commit_option)
		commit_list_ઉમેરો(lookup_commit(the_repository,
						 &info->u_commit),
				   &parents);
	commit_list_ઉમેરો(lookup_commit(the_repository, &info->i_commit),
			   &parents);
	commit_list_ઉમેરો(head_commit, &parents);

	જો (commit_વૃક્ષ(stash_સંદેશ_buf->buf, stash_સંદેશ_buf->len, &info->w_વૃક્ષ,
			parents, &info->w_commit, ખાલી, ખાલી)) {
		જો (!quiet)
			fprintf_ln(stderr, _("Cannot record "
					     "working tree state"));
		ret = -1;
		જાઓ done;
	}

done:
	strbuf_release(&commit_વૃક્ષ_label);
	strbuf_release(&સંદેશ);
	strbuf_release(&untracked_files);
	commit_list_free(parents);
	મુક્ત(branch_name_buf);
	પાછા ret;
}

સ્થિર પૂર્ણાંક create_stash(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix UNUSED,
			સંરચના repository *repo UNUSED)
{
	પૂર્ણાંક ret;
	સંરચના strbuf stash_સંદેશ_buf = STRBUF_INIT;
	સંરચના stash_info info = STASH_INFO_INIT;
	સંરચના pathspec ps;

	/* Starting with argv[1], since argv[0] is "create" */
	strbuf_join_argv(&stash_સંદેશ_buf, argc - 1, ++argv, ' ');

	memset(&ps, 0, કદ(ps));
	જો (!check_changes_tracked_files(&ps))
		પાછા 0;

	ret = do_create_stash(&ps, &stash_સંદેશ_buf, 0, 0, ખાલી, 0, &info,
			      ખાલી, 0);
	જો (!ret)
		printf_ln("%s", oid_to_hex(&info.w_commit));

	free_stash_info(&info);
	strbuf_release(&stash_સંદેશ_buf);
	પાછા ret;
}

સ્થિર પૂર્ણાંક do_ધકેલો_stash(અચલ સંરચના pathspec *ps, અચલ અક્ષર *stash_સંદેશ, પૂર્ણાંક quiet,
			 પૂર્ણાંક keep_index, પૂર્ણાંક patch_mode,
			 સંરચના interactive_options *interactive_opts,
			 પૂર્ણાંક include_untracked, પૂર્ણાંક only_staged)
{
	પૂર્ણાંક ret = 0;
	સંરચના stash_info info = STASH_INFO_INIT;
	સંરચના strbuf patch = STRBUF_INIT;
	સંરચના strbuf stash_સંદેશ_buf = STRBUF_INIT;
	સંરચના strbuf untracked_files = STRBUF_INIT;
	સંરચના strbuf out = STRBUF_INIT;

	જો (patch_mode && keep_index == -1)
		keep_index = 1;

	જો (patch_mode && include_untracked) {
		fprintf_ln(stderr, _("Can't use --patch and --include-untracked"
				     " or --all at the same time"));
		ret = -1;
		જાઓ done;
	}

	/* --patch overrides --staged */
	જો (patch_mode)
		only_staged = 0;

	જો (only_staged && include_untracked) {
		fprintf_ln(stderr, _("Can't use --staged and --include-untracked"
				     " or --all at the same time"));
		ret = -1;
		જાઓ done;
	}

	repo_read_index_preload(the_repository, ખાલી, 0);
	જો (!include_untracked && ps->nr) {
		અક્ષર *ps_matched = xcalloc(ps->nr, 1);

		જો (pathspec_needs_expanded_index(the_repository->index, ps))
			ensure_full_index(the_repository->index);
		માટે (size_t i = 0; i < the_repository->index->cache_nr; i++)
			ce_path_match(the_repository->index, the_repository->index->cache[i], ps,
				      ps_matched);

		જો (report_path_error(ps_matched, ps)) {
			fprintf_ln(stderr, _("Did you forget to 'git add'?"));
			ret = -1;
			મુક્ત(ps_matched);
			જાઓ done;
		}
		મુક્ત(ps_matched);
	}

	જો (repo_refresh_and_write_index(the_repository, REFRESH_QUIET, 0, 0,
					 ખાલી, ખાલી, ખાલી)) {
		ret = error(_("could not write index"));
		જાઓ done;
	}

	જો (!check_changes(ps, include_untracked, &untracked_files)) {
		જો (!quiet)
			printf_ln(_("No local changes to save"));
		જાઓ done;
	}

	જો (!refs_reflog_exists(get_main_ref_store(the_repository), ref_stash) && do_clear_stash()) {
		ret = -1;
		જો (!quiet)
			fprintf_ln(stderr, _("Cannot initialize stash"));
		જાઓ done;
	}

	જો (stash_સંદેશ)
		strbuf_addstr(&stash_સંદેશ_buf, stash_સંદેશ);
	જો (do_create_stash(ps, &stash_સંદેશ_buf, include_untracked, patch_mode,
			    interactive_opts, only_staged, &info, &patch, quiet)) {
		ret = -1;
		જાઓ done;
	}

	જો (do_store_stash(&info.w_commit, stash_સંદેશ_buf.buf, 1)) {
		ret = -1;
		જો (!quiet)
			fprintf_ln(stderr, _("Cannot save the current status"));
		જાઓ done;
	}

	જો (!quiet)
		printf_ln(_("Saved working directory and index state %s"),
			  stash_સંદેશ_buf.buf);

	જો (!(patch_mode || only_staged)) {
		જો (include_untracked && !ps->nr) {
			સંરચના child_process cp = CHILD_PROCESS_INIT;

			cp.git_cmd = 1;
			જો (startup_info->original_cwd) {
				cp.dir = startup_info->original_cwd;
				strvec_pushf(&cp.env, "%s=%s",
					     GIT_WORK_વૃક્ષ_ENVIRONMENT,
					     the_repository->worktree);
			}
			strvec_pushl(&cp.args, "clean", "--force",
				     "--quiet", "-d", ":/", ખાલી);
			જો (include_untracked == INCLUDE_ALL_FILES)
				strvec_ધકેલો(&cp.args, "-x");
			જો (run_command(&cp)) {
				ret = -1;
				જાઓ done;
			}
		}
		discard_index(the_repository->index);
		જો (ps->nr) {
			સંરચના child_process cp_add = CHILD_PROCESS_INIT;
			સંરચના child_process cp_diff = CHILD_PROCESS_INIT;
			સંરચના child_process cp_apply = CHILD_PROCESS_INIT;

			cp_add.git_cmd = 1;
			strvec_ધકેલો(&cp_add.args, "add");
			જો (!include_untracked)
				strvec_ધકેલો(&cp_add.args, "-u");
			જો (include_untracked == INCLUDE_ALL_FILES)
				strvec_ધકેલો(&cp_add.args, "--force");
			strvec_ધકેલો(&cp_add.args, "--");
			add_pathspecs(&cp_add.args, ps);
			જો (run_command(&cp_add)) {
				ret = -1;
				જાઓ done;
			}

			cp_diff.git_cmd = 1;
			strvec_pushl(&cp_diff.args, "diff-index", "-p",
				     "--no-color",
				     "--cached", "--binary", "HEAD", "--",
				     ખાલી);
			add_pathspecs(&cp_diff.args, ps);
			જો (pipe_command(&cp_diff, ખાલી, 0, &out, 0, ખાલી, 0)) {
				ret = -1;
				જાઓ done;
			}

			cp_apply.git_cmd = 1;
			strvec_pushl(&cp_apply.args, "apply", "--index",
				     "-R", ખાલી);
			જો (pipe_command(&cp_apply, out.buf, out.len, ખાલી, 0,
					 ખાલી, 0)) {
				ret = -1;
				જાઓ done;
			}
		} નહીંતર {
			સંરચના child_process cp = CHILD_PROCESS_INIT;
			cp.git_cmd = 1;
			/* BUG: this nukes untracked files in the way */
			strvec_pushl(&cp.args, "reset", "--hard", "-q",
				     "--no-recurse-submodules", ખાલી);
			જો (run_command(&cp)) {
				ret = -1;
				જાઓ done;
			}
		}

		/*
		 * When keeping staged entries, we need to reset the working
		 * directory to match the state of our index. This can be
		 * skipped when the index is the empty tree, because there is
		 * nothing to reset in that case:
		 *
		 *   - When the index has any file, regardless of whether
		 *     staged or not, the tree cannot be empty by definition
		 *     and thus we enter the condition.
		 *
		 *   - When the index has no files, the only thing we need to
		 *     care about is untracked files when `--include-untracked`
		 *     is given. But as we already execute git-clean(1) further
		 *     up to delete such untracked files we don't have to do
		 *     anything here, either.
		 *
		 * We thus skip calling git-checkout(1) in this case, also
		 * because running it on an empty tree will cause it to fail
		 * due to the pathspec not matching anything.
		 */
		જો (keep_index == 1 && !is_null_oid(&info.i_વૃક્ષ) &&
		    !is_empty_વૃક્ષ_oid(&info.i_વૃક્ષ, the_repository->hash_algo)) {
			સંરચના child_process cp = CHILD_PROCESS_INIT;

			cp.git_cmd = 1;
			strvec_pushl(&cp.args, "checkout", "--no-overlay",
				     oid_to_hex(&info.i_વૃક્ષ), "--", ખાલી);
			જો (!ps->nr)
				strvec_ધકેલો(&cp.args, ":/");
			નહીંતર
				add_pathspecs(&cp.args, ps);
			જો (run_command(&cp)) {
				ret = -1;
				જાઓ done;
			}
		}
		જાઓ done;
	} નહીંતર {
		સંરચના child_process cp = CHILD_PROCESS_INIT;

		cp.git_cmd = 1;
		strvec_pushl(&cp.args, "apply", "-R", ખાલી);

		જો (pipe_command(&cp, patch.buf, patch.len, ખાલી, 0, ખાલી, 0)) {
			જો (!quiet)
				fprintf_ln(stderr, _("Cannot remove "
						     "worktree changes"));
			ret = -1;
			જાઓ done;
		}

		જો (keep_index < 1) {
			સંરચના child_process cp = CHILD_PROCESS_INIT;

			cp.git_cmd = 1;
			strvec_pushl(&cp.args, "reset", "-q", "--refresh", "--",
				     ખાલી);
			add_pathspecs(&cp.args, ps);
			જો (run_command(&cp)) {
				ret = -1;
				જાઓ done;
			}
		}
		જાઓ done;
	}

done:
	strbuf_release(&patch);
	strbuf_release(&out);
	free_stash_info(&info);
	strbuf_release(&stash_સંદેશ_buf);
	strbuf_release(&untracked_files);
	પાછા ret;
}

સ્થિર પૂર્ણાંક ધકેલો_stash(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
		      પૂર્ણાંક ધકેલો_assumed)
{
	પૂર્ણાંક force_assume = 0;
	પૂર્ણાંક keep_index = -1;
	પૂર્ણાંક only_staged = 0;
	પૂર્ણાંક patch_mode = 0;
	પૂર્ણાંક include_untracked = 0;
	પૂર્ણાંક quiet = 0;
	પૂર્ણાંક pathspec_file_nul = 0;
	અચલ અક્ષર *stash_સંદેશ = ખાલી;
	અક્ષર *pathspec_from_file = ખાલી;
	સંરચના pathspec ps;
	સંરચના interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	સંરચના option options[] = {
		OPT_BOOL('k', "keep-index", &keep_index,
			 N_("keep index")),
		OPT_BOOL('S', "staged", &only_staged,
			 N_("stash staged changes only")),
		OPT_BOOL('p', "patch", &patch_mode,
			 N_("stash in patch mode")),
		OPT_BOOL(0, "auto-advance", &interactive_opts.auto_advance,
			 N_("auto advance to the next file when selecting hunks interactively")),
		OPT_DIFF_UNIFIED(&interactive_opts.context),
		OPT_DIFF_INTERHUNK_CONTEXT(&interactive_opts.interhunkcontext),
		OPT__QUIET(&quiet, N_("quiet mode")),
		OPT_BOOL('u', "include-untracked", &include_untracked,
			 N_("include untracked files in stash")),
		OPT_SET_INT('a', "all", &include_untracked,
			    N_("include ignore files"), INCLUDE_ALL_FILES),
		OPT_STRING('m', "message", &stash_સંદેશ, N_("message"),
			   N_("stash message")),
		OPT_PATHSPEC_FROM_FILE(&pathspec_from_file),
		OPT_PATHSPEC_FILE_NUL(&pathspec_file_nul),
		OPT_END()
	};
	પૂર્ણાંક ret;

	જો (argc) {
		પૂર્ણાંક flags = PARSE_OPT_KEEP_DASHDASH;

		જો (ધકેલો_assumed)
			flags |= PARSE_OPT_STOP_AT_NON_OPTION;

		argc = parse_options(argc, argv, prefix, options,
				     ધકેલો_assumed ? git_stash_usage :
				     git_stash_ધકેલો_usage, flags);
		force_assume |= patch_mode;
	}

	જો (argc) {
		જો (!strcmp(argv[0], "--")) {
			argc--;
			argv++;
		} નહીંતર જો (ધકેલો_assumed && !force_assume) {
			die("subcommand wasn't specified; 'push' can't be assumed due to unexpected token '%s'",
			    argv[0]);
		}
	}

	parse_pathspec(&ps, 0, PATHSPEC_PREFER_FULL | PATHSPEC_PREFIX_ORIGIN,
		       prefix, argv);

	જો (pathspec_from_file) {
		જો (patch_mode)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--patch");

		જો (only_staged)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--staged");

		જો (ps.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&ps, 0,
				    PATHSPEC_PREFER_FULL | PATHSPEC_PREFIX_ORIGIN,
				    prefix, pathspec_from_file, pathspec_file_nul);
	} નહીંતર જો (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	જો (!patch_mode) {
		જો (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--patch");
		જો (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--patch");
		જો (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--patch");
	}

	જો (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	જો (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	ret = do_ધકેલો_stash(&ps, stash_સંદેશ, quiet, keep_index, patch_mode,
			    &interactive_opts, include_untracked, only_staged);

	clear_pathspec(&ps);
	મુક્ત(pathspec_from_file);
	પાછા ret;
}

સ્થિર પૂર્ણાંક ધકેલો_stash_unassumed(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
				સંરચના repository *repo UNUSED)
{
	પાછા ધકેલો_stash(argc, argv, prefix, 0);
}

સ્થિર પૂર્ણાંક save_stash(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
		      સંરચના repository *repo UNUSED)
{
	પૂર્ણાંક keep_index = -1;
	પૂર્ણાંક only_staged = 0;
	પૂર્ણાંક patch_mode = 0;
	પૂર્ણાંક include_untracked = 0;
	પૂર્ણાંક quiet = 0;
	પૂર્ણાંક ret = 0;
	અચલ અક્ષર *stash_સંદેશ = ખાલી;
	સંરચના pathspec ps;
	સંરચના strbuf stash_સંદેશ_buf = STRBUF_INIT;
	સંરચના interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	સંરચના option options[] = {
		OPT_BOOL('k', "keep-index", &keep_index,
			 N_("keep index")),
		OPT_BOOL('S', "staged", &only_staged,
			 N_("stash staged changes only")),
		OPT_BOOL('p', "patch", &patch_mode,
			 N_("stash in patch mode")),
		OPT_BOOL(0, "auto-advance", &interactive_opts.auto_advance,
			 N_("auto advance to the next file when selecting hunks interactively")),
		OPT_DIFF_UNIFIED(&interactive_opts.context),
		OPT_DIFF_INTERHUNK_CONTEXT(&interactive_opts.interhunkcontext),
		OPT__QUIET(&quiet, N_("quiet mode")),
		OPT_BOOL('u', "include-untracked", &include_untracked,
			 N_("include untracked files in stash")),
		OPT_SET_INT('a', "all", &include_untracked,
			    N_("include ignore files"), INCLUDE_ALL_FILES),
		OPT_STRING('m', "message", &stash_સંદેશ, "message",
			   N_("stash message")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_save_usage,
			     PARSE_OPT_KEEP_DASHDASH);

	જો (argc)
		stash_સંદેશ = strbuf_join_argv(&stash_સંદેશ_buf, argc, argv, ' ');

	memset(&ps, 0, કદ(ps));

	જો (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	જો (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	જો (!patch_mode) {
		જો (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--patch");
		જો (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--patch");
		જો (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--patch");
	}

	ret = do_ધકેલો_stash(&ps, stash_સંદેશ, quiet, keep_index,
			    patch_mode, &interactive_opts, include_untracked,
			    only_staged);

	strbuf_release(&stash_સંદેશ_buf);
	પાછા ret;
}

સ્થિર પૂર્ણાંક write_commit_with_parents(સંરચના repository *r,
				     સંરચના object_id *out,
				     અચલ સંરચના object_id *oid,
				     સંરચના commit_list *parents)
{
	size_t author_len, committer_len;
	સંરચના commit *this;
	અચલ અક્ષર *orig_author, *orig_committer;
	અક્ષર *author = ખાલી, *committer = ખાલી;
	અચલ અક્ષર *buffer;
	unsigned long bufsize;
	અચલ અક્ષર *p;
	સંરચના strbuf સંદેશ = STRBUF_INIT;
	પૂર્ણાંક ret = 0;
	સંરચના ident_split id;

	this = lookup_commit_reference(r, oid);
	buffer = repo_get_commit_buffer(r, this, &bufsize);
	orig_author = find_commit_header(buffer, "author", &author_len);
	orig_committer = find_commit_header(buffer, "committer", &committer_len);

	જો (!orig_author || !orig_committer) {
		ret = error(_("cannot parse commit %s"), oid_to_hex(oid));
		જાઓ out;
	}

	જો (split_ident_લીટી(&id, orig_author, author_len) < 0 ||
	    split_ident_લીટી(&id, orig_committer, committer_len) < 0) {
		ret = error(_("invalid author or committer for %s"), oid_to_hex(oid));
		જાઓ out;
	}

	p = strstr(buffer, "\n\n");
	strbuf_addstr(&સંદેશ, "git stash: ");

	જો (p)
		strbuf_add(&સંદેશ, p + 2, bufsize - (p + 2 - buffer));
	strbuf_complete_લીટી(&સંદેશ);

	author = xmemdupz(orig_author, author_len);
	committer = xmemdupz(orig_committer, committer_len);

	જો (commit_વૃક્ષ_extended(સંદેશ.buf, સંદેશ.len,
				 r->hash_algo->empty_વૃક્ષ, parents,
				 out, author, committer,
				 ખાલી, ખાલી)) {
		ret = error(_("could not write commit"));
		જાઓ out;
	}
out:
	strbuf_release(&સંદેશ);
	repo_unuse_commit_buffer(r, this, buffer);
	મુક્ત(author);
	મુક્ત(committer);
	પાછા ret;
}

સ્થિર પૂર્ણાંક do_import_stash(સંરચના repository *r, અચલ અક્ષર *rev)
{
	સંરચના object_id chain;
	પૂર્ણાંક res = 0;
	અચલ અક્ષર *buffer = ખાલી;
	unsigned long bufsize;
	સંરચના commit *this = ખાલી;
	સંરચના commit_list *વસ્તુઓ = ખાલી, *cur;
	અક્ષર *સંદેશ = ખાલી;

	જો (repo_get_oid(r, rev, &chain))
		પાછા error(_("not a valid revision: %s"), rev);

	this = lookup_commit_reference(r, &chain);
	જો (!this)
		પાછા error(_("not a commit: %s"), rev);

	/*
	 * Walk the commit history, finding each stash entry, and load data into
	 * the array.
	 */
	માટે (;;) {
		અચલ અક્ષર *author, *committer;
		size_t author_len, committer_len;
		અચલ અક્ષર *p;
		અચલ અક્ષર *expected = "git stash <git@stash> 1000684800 +0000";
		અચલ અક્ષર *prefix = "git stash: ";
		સંરચના commit *stash;
		સંરચના વૃક્ષ *વૃક્ષ = repo_get_commit_વૃક્ષ(r, this);

		જો (!વૃક્ષ ||
		    !oideq(&વૃક્ષ->object.oid, r->hash_algo->empty_વૃક્ષ) ||
		    (this->parents &&
		     (!this->parents->next || this->parents->next->next))) {
			res = error(_("%s is not a valid exported stash commit"),
					oid_to_hex(&this->object.oid));
			જાઓ out;
		}

		buffer = repo_get_commit_buffer(r, this, &bufsize);

		જો (!this->parents) {
			/*
			 * We don't have any parents.  Make sure this is our
			 * root commit.
			 */
			author = find_commit_header(buffer, "author", &author_len);
			committer = find_commit_header(buffer, "committer", &committer_len);

			જો (!author || !committer) {
				error(_("cannot parse commit %s"), oid_to_hex(&this->object.oid));
				જાઓ out;
			}

			જો (author_len != strlen(expected) ||
			    committer_len != strlen(expected) ||
			    memcmp(author, expected, author_len) ||
			    memcmp(committer, expected, committer_len)) {
				res = error(_("found root commit %s with invalid data"), oid_to_hex(&this->object.oid));
				જાઓ out;
			}
			તોડો;
		}

		p = strstr(buffer, "\n\n");
		જો (!p) {
			res = error(_("cannot parse commit %s"), oid_to_hex(&this->object.oid));
			જાઓ out;
		}

		p += 2;
		જો (((size_t)(bufsize - (p - buffer)) < strlen(prefix)) ||
		    memcmp(prefix, p, strlen(prefix))) {
			res = error(_("found stash commit %s without expected prefix"), oid_to_hex(&this->object.oid));
			જાઓ out;
		}

		stash = this->parents->next->item;

		જો (repo_parse_commit(r, this->parents->item) ||
		    repo_parse_commit(r, stash)) {
			res = error(_("cannot parse parents of commit: %s"),
					oid_to_hex(&this->object.oid));
			જાઓ out;
		}

		જો (check_stash_topology(r, stash)) {
			res = error(_("%s does not look like a stash commit"),
					oid_to_hex(&stash->object.oid));
			જાઓ out;
		}

		repo_unuse_commit_buffer(r, this, buffer);
		buffer = ખાલી;
		વસ્તુઓ = commit_list_ઉમેરો(stash, &વસ્તુઓ);
		this = this->parents->item;
	}

	/*
	 * Now, walk each entry, adding it to the stash as a normal stash
	 * commit.
	 */
	માટે (cur = વસ્તુઓ; cur; cur = cur->next) {
		અચલ અક્ષર *p;
		સંરચના object_id *oid;

		this = cur->item;
		oid = &this->object.oid;
		buffer = repo_get_commit_buffer(r, this, &bufsize);
		જો (!buffer) {
			res = error(_("cannot read commit buffer for %s"), oid_to_hex(oid));
			જાઓ out;
		}

		p = strstr(buffer, "\n\n");
		જો (!p) {
			res = error(_("cannot parse commit %s"), oid_to_hex(oid));
			જાઓ out;
		}

		p += 2;
		સંદેશ = xmemdupz(p, bufsize - (p - buffer));
		repo_unuse_commit_buffer(r, this, buffer);
		buffer = ખાલી;

		જો (do_store_stash(oid, સંદેશ, 1)) {
			res = error(_("cannot save the stash for %s"), oid_to_hex(oid));
			જાઓ out;
		}
		FREE_AND_NULL(સંદેશ);
	}
out:
	જો (this && buffer)
		repo_unuse_commit_buffer(r, this, buffer);
	commit_list_free(વસ્તુઓ);
	મુક્ત(સંદેશ);

	પાછા res;
}

સ્થિર પૂર્ણાંક import_stash(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
			સંરચના repository *repo)
{
	સંરચના option options[] = {
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_import_usage,
			     PARSE_OPT_KEEP_DASHDASH);

	જો (argc != 1)
		usage_સંદેશ_opt("a revision is required", git_stash_import_usage, options);

	પાછા do_import_stash(repo, argv[0]);
}

સંરચના stash_entry_data {
	સંરચના repository *r;
	સંરચના commit_list **વસ્તુઓ;
	size_t ગણતરી;
};

સ્થિર પૂર્ણાંક collect_stash_entries(અચલ અક્ષર *refname UNUSED,
				 સંરચના object_id *old_oid UNUSED,
				 સંરચના object_id *new_oid,
				 અચલ અક્ષર *committer UNUSED,
				 timestamp_t timestamp UNUSED,
				 પૂર્ણાંક tz UNUSED, અચલ અક્ષર *સંદેશ UNUSED,
				 શૂન્ય *cb_data)
{
	સંરચના stash_entry_data *data = cb_data;
	સંરચના commit *stash;

	data->ગણતરી++;
	stash = lookup_commit_reference(data->r, new_oid);
	જો (!stash || check_stash_topology(data->r, stash)) {
		પાછા error(_("%s does not look like a stash commit"),
			     oid_to_hex(new_oid));
	}
	data->વસ્તુઓ = commit_list_append(stash, data->વસ્તુઓ);
	પાછા 0;
}

સ્થિર પૂર્ણાંક do_export_stash(સંરચના repository *r,
			   અચલ અક્ષર *ref,
			   પૂર્ણાંક argc,
			   અચલ અક્ષર **argv)
{
	સંરચના object_id base;
	સંરચના commit *prev;
	સંરચના commit_list *વસ્તુઓ = ખાલી, **iter = &વસ્તુઓ, *cur;
	પૂર્ણાંક res = 0;
	પૂર્ણાંક i;
	સંરચના strbuf revision = STRBUF_INIT;
	અચલ અક્ષર *author, *committer;

	/*
	 * This is an arbitrary, fixed date, specifically the one used by git
	 * format-patch.  The goal is merely to produce reproducible output.
	 */
	prepare_fallback_ident("git stash", "git@stash");
	author = fmt_ident("git stash", "git@stash", WANT_BLANK_IDENT,
			   "2001-09-17T00:00:00Z", 0);
	committer = fmt_ident("git stash", "git@stash", WANT_BLANK_IDENT,
			      "2001-09-17T00:00:00Z", 0);

	/* First, we create a single empty commit. */
	જો (commit_વૃક્ષ_extended("", 0, r->hash_algo->empty_વૃક્ષ, ખાલી,
				 &base, author, committer, ખાલી, ખાલી))
		પાછા error(_("unable to write base commit"));

	prev = lookup_commit_reference(r, &base);

	જો (argc) {
		/*
		 * Find each specified stash, and load data into the array.
		 */
		માટે (i = 0; i < argc; i++) {
			સંરચના object_id oid;
			સંરચના commit *stash;

			જો (parse_stash_revision(&revision, argv[i], 1) ||
			    repo_get_oid_with_flags(r, revision.buf, &oid,
						    GET_OID_QUIETLY |
						    GET_OID_GENTLY)) {
				res = error(_("unable to find stash entry %s"), argv[i]);
				જાઓ out;
			}

			stash = lookup_commit_reference(r, &oid);
			જો (!stash || check_stash_topology(r, stash)) {
				res = error(_("%s does not look like a stash commit"),
						revision.buf);
				જાઓ out;
			}
			iter = commit_list_append(stash, iter);
		}
	} નહીંતર {
		/*
		 * Walk the reflog, finding each stash entry, and load data into the
		 * array.
		 */
		સંરચના stash_entry_data cb_data = {
			.r = r, .વસ્તુઓ = iter,
		};
		જો (refs_for_each_reflog_ent_reverse(get_main_ref_store(r),
						     "refs/stash",
						     collect_stash_entries,
						     &cb_data) && cb_data.ગણતરી)
			જાઓ out;
	}

	/*
	 * Now, create a set of commits identical to the regular stash commits,
	 * but where their first parents form a chain to our original empty
	 * base commit.
	 */
	વસ્તુઓ = commit_list_reverse(વસ્તુઓ);
	માટે (cur = વસ્તુઓ; cur; cur = cur->next) {
		સંરચના commit_list *parents = ખાલી;
		સંરચના commit_list **next = &parents;
		સંરચના object_id out;
		સંરચના commit *stash = cur->item;

		next = commit_list_append(prev, next);
		next = commit_list_append(stash, next);
		res = write_commit_with_parents(r, &out, &stash->object.oid, parents);
		commit_list_free(parents);
		જો (res)
			જાઓ out;
		prev = lookup_commit_reference(r, &out);
	}
	જો (ref)
		refs_update_ref(get_main_ref_store(r), ખાલી, ref,
				&prev->object.oid, ખાલી, 0, UPDATE_REFS_DIE_ON_ERR);
	નહીંતર
		puts(oid_to_hex(&prev->object.oid));
out:
	strbuf_release(&revision);
	commit_list_free(વસ્તુઓ);

	પાછા res;
}

ગણના export_action {
	ACTION_NONE,
	ACTION_PRINT,
	ACTION_TO_REF,
};

સ્થિર પૂર્ણાંક export_stash(પૂર્ણાંક argc,
			અચલ અક્ષર **argv,
			અચલ અક્ષર *prefix,
			સંરચના repository *repo)
{
	અચલ અક્ષર *ref = ખાલી;
	ગણના export_action action = ACTION_NONE;
	સંરચના option options[] = {
		OPT_CMDMODE(0, "print", &action,
			    N_("print the object ID instead of writing it to a ref"),
			    ACTION_PRINT),
		OPT_STRING(0, "to-ref", &ref, "ref",
			    N_("save the data to the given ref")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_export_usage,
			     PARSE_OPT_KEEP_DASHDASH);

	જો (ref && action == ACTION_NONE)
		action = ACTION_TO_REF;

	જો (action == ACTION_NONE || (ref && action == ACTION_PRINT))
		પાછા error(_("exactly one of --print and --to-ref is required"));

	પાછા do_export_stash(repo, ref, argc, argv);
}

પૂર્ણાંક cmd_stash(પૂર્ણાંક argc,
	      અચલ અક્ષર **argv,
	      અચલ અક્ષર *prefix,
	      સંરચના repository *repo)
{
	pid_t pid = getpid();
	અચલ અક્ષર *index_file;
	સંરચના strvec args = STRVEC_INIT;
	parse_opt_subcommand_fn *fn = ખાલી;
	સંરચના option options[] = {
		OPT_SUBCOMMAND("apply", &fn, apply_stash),
		OPT_SUBCOMMAND("clear", &fn, clear_stash),
		OPT_SUBCOMMAND("drop", &fn, drop_stash),
		OPT_SUBCOMMAND("pop", &fn, કાઢો_stash),
		OPT_SUBCOMMAND("branch", &fn, branch_stash),
		OPT_SUBCOMMAND("list", &fn, list_stash),
		OPT_SUBCOMMAND("show", &fn, show_stash),
		OPT_SUBCOMMAND("store", &fn, store_stash),
		OPT_SUBCOMMAND("create", &fn, create_stash),
		OPT_SUBCOMMAND("push", &fn, ધકેલો_stash_unassumed),
		OPT_SUBCOMMAND("export", &fn, export_stash),
		OPT_SUBCOMMAND("import", &fn, import_stash),
		OPT_SUBCOMMAND_F("save", &fn, save_stash, ખાલી, PARSE_OPT_NOCOMPLETE),
		OPT_END()
	};
	અચલ અક્ષર **args_copy;
	પૂર્ણાંક ret;

	repo_config(the_repository, git_stash_config, ખાલી);

	argc = parse_options(argc, argv, prefix, options, git_stash_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL |
			     PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	index_file = repo_get_index_file(the_repository);
	strbuf_addf(&stash_index_path, "%s.stash.%" PRIuMAX, index_file,
		    (uintmax_t)pid);

	જો (fn) {
		ret = fn(argc, argv, prefix, repo);

		/*
		 * The subcommand implementations return 0 on success, a
		 * negative value on failure, and STASH_APPLY_CONFLICT
		 * when applying a stash entry resulted in conflicts.
		 * Map failures to 128, the status die() uses, so that
		 * exit status 1 unambiguously indicates conflicts.
		 */
		જો (ret < 0)
			પાછા 128;
		પાછા ret;
	} નહીંતર જો (!argc) {
		પાછા !!ધકેલો_stash_unassumed(0, ખાલી, prefix, repo);
	}

	/* Assume 'stash push' */
	strvec_ધકેલો(&args, "push");
	strvec_pushv(&args, argv);

	/*
	 * `push_stash()` ends up modifying the array, which causes memory
	 * leaks if we didn't copy the array here.
	 */
	DUP_ARRAY(args_copy, args.v, args.nr);

	ret = !!ધકેલો_stash(args.nr, args_copy, prefix, 1);

	strvec_clear(&args);
	મુક્ત(args_copy);
	પાછા ret;
}
