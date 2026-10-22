#define USE_THE_REPOSITORY_VARIABLE

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "abspath.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hash.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "object-name.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "refs.h"
#अन्तर्भाव "stash.h"
#अन्तर्भाव "lockfile.h"
#अन्तर्भाव "cache-tree.h"
#अन्तर्भाव "unpack-trees.h"
#अन्तर्भाव "merge-ort-wrappers.h"
#अन्तर्भाव "strvec.h"
#अन्तर्भाव "run-command.h"
#अन्तर्भाव "dir.h"
#अन्तर्भाव "entry.h"
#अन्तर्भाव "preload-index.h"
#अन्तर्भाव "read-cache.h"
#अन्तर्भाव "repository.h"
#अन्तर्भाव "rerere.h"
#अन्तर्भाव "revision.h"
#अन्तर्भाव "setup.h"
#अन्तर्भाव "sparse-index.h"
#अन्तर्भाव "log-tree.h"
#अन्तर्भाव "diffcore.h"
#अन्तर्भाव "reflog.h"
#अन्तर्भाव "reflog-walk.h"
#अन्तर्भाव "add-interactive.h"
#अन्तर्भाव "commit.h"

#define INCLUDE_ALL_FILES 2

#define BUILTIN_STASH_LIST_USAGE \
	N_("git stash list [<log-options>]")
#define BUILTIN_STASH_SHOW_USAGE \
	N_("git stash show [-u | --include-untracked | --only-untracked] [<diff-options>] [<stash>]")
#define BUILTIN_STASH_DROP_USAGE \
	N_("git stash drop [-q | --quiet] [<stash>]")
#define BUILTIN_STASH_झिक_USAGE \
	N_("git stash pop [--index] [-q | --quiet] [<stash>]")
#define BUILTIN_STASH_APPLY_USAGE \
	N_("git stash apply [--index] [-q | --quiet] [--label-ours=<label>] [--label-theirs=<label>] [--label-base=<label>] [<stash>]")
#define BUILTIN_STASH_BRANCH_USAGE \
	N_("git stash branch <branchname> [<stash>]")
#define BUILTIN_STASH_STORE_USAGE \
	N_("git stash store [(-m | --message) <message>] [-q | --quiet] <commit>")
#define BUILTIN_STASH_थप_USAGE \
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

स्थिर अचर वर्ण * अचर git_stash_usage[] = {
	BUILTIN_STASH_LIST_USAGE,
	BUILTIN_STASH_SHOW_USAGE,
	BUILTIN_STASH_DROP_USAGE,
	BUILTIN_STASH_झिक_USAGE,
	BUILTIN_STASH_APPLY_USAGE,
	BUILTIN_STASH_BRANCH_USAGE,
	BUILTIN_STASH_थप_USAGE,
	BUILTIN_STASH_SAVE_USAGE,
	BUILTIN_STASH_CLEAR_USAGE,
	BUILTIN_STASH_CREATE_USAGE,
	BUILTIN_STASH_STORE_USAGE,
	BUILTIN_STASH_EXPORT_USAGE,
	BUILTIN_STASH_IMPORT_USAGE,
	रिक्त
};

स्थिर अचर वर्ण * अचर git_stash_list_usage[] = {
	BUILTIN_STASH_LIST_USAGE,
	रिक्त
};

स्थिर अचर वर्ण * अचर git_stash_show_usage[] = {
	BUILTIN_STASH_SHOW_USAGE,
	रिक्त
};

स्थिर अचर वर्ण * अचर git_stash_drop_usage[] = {
	BUILTIN_STASH_DROP_USAGE,
	रिक्त
};

स्थिर अचर वर्ण * अचर git_stash_झिक_usage[] = {
	BUILTIN_STASH_झिक_USAGE,
	रिक्त
};

स्थिर अचर वर्ण * अचर git_stash_apply_usage[] = {
	BUILTIN_STASH_APPLY_USAGE,
	रिक्त
};

स्थिर अचर वर्ण * अचर git_stash_branch_usage[] = {
	BUILTIN_STASH_BRANCH_USAGE,
	रिक्त
};

स्थिर अचर वर्ण * अचर git_stash_clear_usage[] = {
	BUILTIN_STASH_CLEAR_USAGE,
	रिक्त
};

स्थिर अचर वर्ण * अचर git_stash_store_usage[] = {
	BUILTIN_STASH_STORE_USAGE,
	रिक्त
};

स्थिर अचर वर्ण * अचर git_stash_थप_usage[] = {
	BUILTIN_STASH_थप_USAGE,
	रिक्त
};

स्थिर अचर वर्ण * अचर git_stash_save_usage[] = {
	BUILTIN_STASH_SAVE_USAGE,
	रिक्त
};

स्थिर अचर वर्ण * अचर git_stash_export_usage[] = {
	BUILTIN_STASH_EXPORT_USAGE,
	रिक्त
};

स्थिर अचर वर्ण * अचर git_stash_import_usage[] = {
	BUILTIN_STASH_IMPORT_USAGE,
	रिक्त
};

स्थिर अचर वर्ण ref_stash[] = "refs/stash";
स्थिर संरचना strbuf stash_index_path = STRBUF_INIT;

स्थिर पूर्णाङ्क show_stat = 1;
स्थिर पूर्णाङ्क show_patch;
स्थिर पूर्णाङ्क show_include_untracked;
स्थिर पूर्णाङ्क use_index;

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
संरचना stash_info {
	संरचना object_id w_commit;
	संरचना object_id b_commit;
	संरचना object_id i_commit;
	संरचना object_id u_commit;
	संरचना object_id c_commit;
	संरचना object_id w_रूख;
	संरचना object_id b_रूख;
	संरचना object_id i_रूख;
	संरचना object_id u_रूख;
	संरचना strbuf revision;
	पूर्णाङ्क is_stash_ref;
	पूर्णाङ्क has_u;
};

#define STASH_INFO_INIT { \
	.revision = STRBUF_INIT, \
}

स्थिर शून्य free_stash_info(संरचना stash_info *info)
{
	strbuf_release(&info->revision);
}

स्थिर पूर्णाङ्क check_stash_topology(संरचना repository *r, संरचना commit *stash)
{
	संरचना commit *p1, *p2, *p3 = रिक्त;

	/* stash must have two or three parents */
	यदि (!stash->parents || !stash->parents->next ||
			(stash->parents->next->next && stash->parents->next->next->next))
		फर्काऊ -1;
	p1 = stash->parents->item;
	p2 = stash->parents->next->item;
	यदि (stash->parents->next->next)
		p3 = stash->parents->next->next->item;
	यदि (repo_parse_commit(r, p1) || repo_parse_commit(r, p2) ||
			(p3 && repo_parse_commit(r, p3)))
		फर्काऊ -1;
	/* p2 must have a single parent, p3 must have no parents */
	यदि (!p2->parents || p2->parents->next || (p3 && p3->parents))
		फर्काऊ -1;
	यदि (repo_parse_commit(r, p2->parents->item))
		फर्काऊ -1;
	/* p2^1 must equal p1 */
	यदि (!oideq(&p1->object.oid, &p2->parents->item->object.oid))
		फर्काऊ -1;

	फर्काऊ 0;
}

स्थिर शून्य assert_stash_like(संरचना stash_info *info, अचर वर्ण *revision)
{
	यदि (get_oidf(&info->b_commit, "%s^1", revision) ||
	    get_oidf(&info->w_रूख, "%s:", revision) ||
	    get_oidf(&info->b_रूख, "%s^1:", revision) ||
	    get_oidf(&info->i_रूख, "%s^2:", revision))
		die(_("'%s' is not a stash-like commit"), revision);
}

स्थिर पूर्णाङ्क parse_stash_revision(संरचना strbuf *revision, अचर वर्ण *commit, पूर्णाङ्क quiet)
{
	strbuf_reset(revision);
	यदि (!commit) {
		यदि (!refs_ref_exists(get_main_ref_store(the_repository), ref_stash)) {
			यदि (!quiet)
				fprintf_ln(मानक_त्रुटि, _("No stash entries found."));
			फर्काऊ -1;
		}

		strbuf_addf(revision, "%s@{0}", ref_stash);
	} अन्यथा यदि (म_अखोज(commit, "0123456789") == म_माप(commit)) {
		strbuf_addf(revision, "%s@{%s}", ref_stash, commit);
	} अन्यथा {
		strbuf_addstr(revision, commit);
	}
	फर्काऊ 0;
}

स्थिर पूर्णाङ्क get_stash_info(संरचना stash_info *info, पूर्णाङ्क argc, अचर वर्ण **argv)
{
	पूर्णाङ्क ret;
	वर्ण *end_of_rev;
	वर्ण *expanded_ref;
	अचर वर्ण *revision;
	अचर वर्ण *commit = रिक्त;
	संरचना object_id dummy;
	संरचना strbuf symbolic = STRBUF_INIT;

	यदि (argc > 1) {
		पूर्णाङ्क i;
		संरचना strbuf refs_सन्देश = STRBUF_INIT;

		लागि (i = 0; i < argc; i++)
			strbuf_addf(&refs_सन्देश, " '%s'", argv[i]);

		fprintf_ln(मानक_त्रुटि, _("Too many revisions specified:%s"),
			   refs_सन्देश.buf);
		strbuf_release(&refs_सन्देश);

		फर्काऊ -1;
	}

	यदि (argc == 1)
		commit = argv[0];

	strbuf_init(&info->revision, 0);
	यदि (parse_stash_revision(&info->revision, commit, 0)) {
		फर्काऊ -1;
	}

	revision = info->revision.buf;

	यदि (repo_get_oid(the_repository, revision, &info->w_commit))
		फर्काऊ error(_("%s is not a valid reference"), revision);

	assert_stash_like(info, revision);

	info->has_u = !get_oidf(&info->u_रूख, "%s^3:", revision);

	end_of_rev = strchrnul(revision, '@');
	strbuf_add(&symbolic, revision, end_of_rev - revision);

	ret = repo_dwim_ref(the_repository, symbolic.buf, symbolic.len,
			    &dummy, &expanded_ref, 0);
	strbuf_release(&symbolic);
	विकल्प (ret) {
	पक्ष 0: /* Not found, but valid ref */
		info->is_stash_ref = 0;
		रोक;
	पक्ष 1:
		info->is_stash_ref = !म_भेद(expanded_ref, ref_stash);
		रोक;
	स्वभाव: /* Invalid or ambiguous */
		रोक;
	}

	मुक्त(expanded_ref);
	फर्काऊ !(ret == 0 || ret == 1);
}

स्थिर पूर्णाङ्क do_clear_stash(शून्य)
{
	संरचना object_id obj;
	यदि (repo_get_oid(the_repository, ref_stash, &obj))
		फर्काऊ 0;

	फर्काऊ refs_delete_ref(get_main_ref_store(the_repository), रिक्त,
			       ref_stash, &obj, 0);
}

स्थिर पूर्णाङ्क clear_stash(पूर्णाङ्क argc, अचर वर्ण **argv, अचर वर्ण *prefix,
		       संरचना repository *repo UNUSED)
{
	संरचना option options[] = {
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_clear_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	यदि (argc)
		फर्काऊ error(_("git stash clear with arguments is "
			       "unimplemented"));

	फर्काऊ do_clear_stash();
}

स्थिर पूर्णाङ्क reset_रूख(संरचना object_id *i_रूख, पूर्णाङ्क update, पूर्णाङ्क reset)
{
	पूर्णाङ्क nr_trees = 1;
	संरचना unpack_trees_options opts;
	संरचना रूख_desc t[MAX_UNPACK_TREES];
	संरचना रूख *रूख;
	संरचना lock_file lock_file = LOCK_INIT;

	repo_read_index_preload(the_repository, रिक्त, 0);
	यदि (refresh_index(the_repository->index, REFRESH_QUIET, रिक्त, रिक्त, रिक्त))
		फर्काऊ -1;

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);

	स_रखो(&opts, 0, परिमाण(opts));

	रूख = repo_parse_रूख_indirect(the_repository, i_रूख);
	यदि (repo_parse_रूख(the_repository, रूख))
		फर्काऊ -1;

	init_रूख_desc(t, &रूख->object.oid, रूख->buffer, रूख->size);

	opts.head_idx = 1;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;
	opts.merge = 1;
	opts.reset = reset ? UNPACK_RESET_PROTECT_UNTRACKED : 0;
	opts.update = update;
	यदि (update)
		opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
	opts.fn = oneway_merge;

	यदि (unpack_trees(nr_trees, t, &opts))
		फर्काऊ -1;

	यदि (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
		फर्काऊ error(_("unable to write new index file"));

	फर्काऊ 0;
}

स्थिर पूर्णाङ्क create_index_from_रूख(अचर संरचना object_id *रूख_id,
				  अचर वर्ण *index_path)
{
	पूर्णाङ्क nr_trees = 1;
	पूर्णाङ्क ret = 0;
	संरचना unpack_trees_options opts;
	संरचना रूख_desc t[MAX_UNPACK_TREES];
	संरचना रूख *रूख;
	संरचना index_state dst_istate = INDEX_STATE_INIT(the_repository);
	संरचना lock_file lock_file = LOCK_INIT;

	repo_read_index_preload(the_repository, रिक्त, 0);
	refresh_index(the_repository->index, REFRESH_QUIET, रिक्त, रिक्त, रिक्त);

	hold_lock_file_for_update(&lock_file, index_path, LOCK_DIE_ON_ERROR);

	स_रखो(&opts, 0, परिमाण(opts));

	रूख = repo_parse_रूख_indirect(the_repository, रूख_id);
	यदि (!रूख || repo_parse_रूख(the_repository, रूख)) {
		ret = -1;
		गच्छ done;
	}

	init_रूख_desc(t, &रूख->object.oid, रूख->buffer, रूख->size);

	opts.head_idx = 1;
	opts.src_index = the_repository->index;
	opts.dst_index = &dst_istate;
	opts.merge = 1;
	opts.reset = UNPACK_RESET_PROTECT_UNTRACKED;
	opts.fn = oneway_merge;

	यदि (unpack_trees(nr_trees, t, &opts)) {
		ret = -1;
		गच्छ done;
	}

	यदि (write_locked_index(&dst_istate, &lock_file, COMMIT_LOCK)) {
		ret = error(_("unable to write new index file"));
		गच्छ done;
	}

done:
	release_index(&dst_istate);
	यदि (ret)
		rollback_lock_file(&lock_file);
	फर्काऊ ret;
}

स्थिर पूर्णाङ्क is_path_a_directory(अचर वर्ण *path)
{
	/*
	 * This function differs from abspath.c:is_directory() in that
	 * here we use lstat() instead of stat(); we do not want to
	 * follow symbolic links here.
	 */
	संरचना stat st;
	फर्काऊ (!lstat(path, &st) && S_ISDIR(st.st_mode));
}

स्थिर शून्य add_diff_to_buf(संरचना diff_queue_struct *q,
			    संरचना diff_options *options UNUSED,
			    शून्य *data)
{
	पूर्णाङ्क i;

	लागि (i = 0; i < q->nr; i++) {
		यदि (is_path_a_directory(q->queue[i]->one->path))
			जारी;

		strbuf_addstr(data, q->queue[i]->one->path);

		/* NUL-terminate: will be fed to update-index -z */
		strbuf_addch(data, '\0');
	}
}

स्थिर पूर्णाङ्क restore_untracked(संरचना object_id *u_रूख)
{
	पूर्णाङ्क res;
	संरचना child_process cp = CHILD_PROCESS_INIT;

	/*
	 * We need to run restore files from a given index, but without
	 * affecting the current index, so we use GIT_INDEX_FILE with
	 * run_command to fork processes that will not interfere.
	 */
	cp.git_cmd = 1;
	strvec_थप(&cp.args, "read-tree");
	strvec_थप(&cp.args, oid_to_hex(u_रूख));
	strvec_pushf(&cp.env, "GIT_INDEX_FILE=%s",
		     stash_index_path.buf);
	यदि (run_command(&cp)) {
		remove_path(stash_index_path.buf);
		फर्काऊ -1;
	}

	child_process_init(&cp);
	cp.git_cmd = 1;
	strvec_pushl(&cp.args, "checkout-index", "--all", रिक्त);
	strvec_pushf(&cp.env, "GIT_INDEX_FILE=%s",
		     stash_index_path.buf);

	res = run_command(&cp);
	remove_path(stash_index_path.buf);
	फर्काऊ res;
}

स्थिर शून्य unstage_changes_unless_new(संरचना object_id *orig_रूख)
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

	संरचना checkout state = CHECKOUT_INIT;
	संरचना diff_options diff_opts;
	संरचना lock_file lock = LOCK_INIT;
	पूर्णाङ्क i;

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

	do_diff_cache(orig_रूख, &diff_opts);
	diffcore_std(&diff_opts);

	/* Iterate over the paths that changed due to the merge... */
	लागि (i = 0; i < diff_queued_diff.nr; i++) {
		संरचना diff_filepair *p;
		संरचना cache_entry *ce;
		पूर्णाङ्क pos;

		/* Look up the path's position in the current index. */
		p = diff_queued_diff.queue[i];
		pos = index_name_pos(the_repository->index, p->two->path,
				     म_माप(p->two->path));

		/*
		 * Step 2: Place changes in the working tree
		 *
		 * Stash is about restoring changes *to the working tree*.
		 * So if the merge successfully got a new version of some
		 * path, but left it out of the working tree, then clear the
		 * SKIP_WORKTREE bit and write it to the working tree.
		 */
		यदि (pos >= 0 && ce_skip_worktree(the_repository->index->cache[pos])) {
			संरचना stat st;

			ce = the_repository->index->cache[pos];
			यदि (!lstat(ce->name, &st)) {
				/* Conflicting path present; relocate it */
				संरचना strbuf new_path = STRBUF_INIT;
				पूर्णाङ्क fd;

				strbuf_addf(&new_path,
					    "%s.stash.XXXXXX", ce->name);
				fd = xmkstemp(new_path.buf);
				बंद(fd);
				म_लिखो(_("WARNING: Untracked file in way of "
					 "tracked file!  Renaming\n "
					 "           %s -> %s\n"
					 "         to make room.\n"),
				       ce->name, new_path.buf);
				यदि (नाम(ce->name, new_path.buf))
					die("Failed to move %s to %s",
					    ce->name, new_path.buf);
				strbuf_release(&new_path);
			}
			checkout_entry(ce, &state, रिक्त, रिक्त);
			ce->ce_flags &= ~CE_SKIP_WORKTREE;
		}

		/*
		 * Step 3: "unstage" changes, as long as they are still tracked
		 */
		यदि (p->one->oid_valid) {
			/*
			 * Path existed in orig_tree; restore index entry
			 * from that tree in order to "unstage" the changes.
			 */
			पूर्णाङ्क option = ADD_CACHE_OK_TO_REPLACE;
			यदि (pos < 0)
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
	यदि (write_locked_index(the_repository->index, &lock,
			       COMMIT_LOCK | SKIP_IF_UNCHANGED))
		die(_("could not write index"));
}

स्थिर गणना stash_apply_result do_apply_stash(अचर वर्ण *prefix,
					      संरचना stash_info *info,
					      पूर्णाङ्क index, पूर्णाङ्क quiet,
					      अचर वर्ण *label_ours,
					      अचर वर्ण *label_theirs,
					      अचर वर्ण *label_base)
{
	पूर्णाङ्क clean, ret;
	पूर्णाङ्क has_index = index;
	संरचना merge_options o;
	संरचना object_id c_रूख;
	संरचना object_id index_रूख;
	संरचना रूख *head, *merge, *merge_base;
	संरचना lock_file lock = LOCK_INIT;

	repo_read_index_preload(the_repository, रिक्त, 0);
	यदि (repo_refresh_and_write_index(the_repository, REFRESH_QUIET, 0, 0,
					 रिक्त, रिक्त, रिक्त))
		फर्काऊ error(_("could not write index"));

	यदि (write_index_as_रूख(&c_रूख, the_repository->index,
				repo_get_index_file(the_repository), 0, रिक्त))
		फर्काऊ error(_("cannot apply a stash in the middle of a merge"));

	init_ui_merge_options(&o, the_repository);

	यदि (quiet)
		o.verbosity = 0;

	यदि (index) {
		यदि (oideq(&info->b_रूख, &info->i_रूख) ||
		    oideq(&c_रूख, &info->i_रूख)) {
			has_index = 0;
		} अन्यथा {
			संरचना merge_result result = { 0 };

			o.branch1 = "Current index";
			o.branch2 = "Stashed index changes";
			o.ancestor = "Stash base";

			head = lookup_रूख(o.repo, &c_रूख);
			merge = lookup_रूख(o.repo, &info->i_रूख);
			merge_base = lookup_रूख(o.repo, &info->b_रूख);

			merge_incore_nonrecursive(&o, merge_base, head, merge,
						  &result);

			यदि (result.clean < 0) {
				merge_finalize(&o, &result);
				फर्काऊ error(_("index merge failed"));
			} अन्यथा यदि (!result.clean) {
				merge_finalize(&o, &result);
				फर्काऊ error(_("conflicts in index. "
					       "Try without --index."));
			} अन्यथा {
				oidcpy(&index_रूख, &result.रूख->object.oid);
				merge_finalize(&o, &result);
			}
		}
	}

	o.branch1 = label_ours ? label_ours : "Updated upstream";
	o.branch2 = label_theirs ? label_theirs : "Stashed changes";
	o.ancestor = label_base ? label_base : "Stash base";

	यदि (oideq(&info->b_रूख, &c_रूख))
		o.branch1 = "Version stash was based on";

	यदि (o.verbosity >= 3)
		printf_ln(_("Merging %s with %s"), o.branch1, o.branch2);

	head = lookup_रूख(o.repo, &c_रूख);
	merge = lookup_रूख(o.repo, &info->w_रूख);
	merge_base = lookup_रूख(o.repo, &info->b_रूख);

	repo_hold_locked_index(o.repo, &lock, LOCK_DIE_ON_ERROR);
	clean = merge_ort_nonrecursive(&o, head, merge, merge_base);

	/*
	 * If 'clean' >= 0, reverse the value for 'ret' so 'ret' is 0 when the
	 * merge was clean, and 1 if the merge was unclean or a negative value
	 * if it encountered an error.
	 */
	ret = clean >= 0 ? !clean : clean;

	यदि (ret < 0)
		rollback_lock_file(&lock);
	अन्यथा यदि (write_locked_index(o.repo->index, &lock,
				      COMMIT_LOCK | SKIP_IF_UNCHANGED))
		ret = error(_("could not write index"));

	यदि (ret) {
		repo_rerere(the_repository, 0);

		यदि (index)
			fprintf_ln(मानक_त्रुटि, _("Index was not unstashed."));

		गच्छ restore_untracked;
	}

	यदि (has_index) {
		यदि (reset_रूख(&index_रूख, 0, 0))
			ret = -1;
	} अन्यथा {
		unstage_changes_unless_new(&c_रूख);
	}

restore_untracked:
	यदि (info->has_u && restore_untracked(&info->u_रूख))
		ret = error(_("could not restore untracked files from stash"));

	यदि (!quiet) {
		संरचना child_process cp = CHILD_PROCESS_INIT;

		/*
		 * Status is quite simple and could be replaced with calls to
		 * wt_status in the future, but it adds complexities which may
		 * require more tests.
		 */
		cp.git_cmd = 1;
		cp.dir = prefix;
		strvec_pushf(&cp.env, GIT_WORK_रूख_ENVIRONMENT"=%s",
			     absolute_path(repo_get_work_रूख(the_repository)));
		strvec_pushf(&cp.env, GIT_DIR_ENVIRONMENT"=%s",
			     absolute_path(repo_get_git_dir(the_repository)));
		strvec_थप(&cp.args, "status");
		run_command(&cp);
	}

	फर्काऊ ret;
}

स्थिर पूर्णाङ्क apply_stash(पूर्णाङ्क argc, अचर वर्ण **argv, अचर वर्ण *prefix,
		       संरचना repository *repo UNUSED)
{
	पूर्णाङ्क ret = -1;
	पूर्णाङ्क quiet = 0;
	पूर्णाङ्क index = use_index;
	अचर वर्ण *label_ours = रिक्त, *label_theirs = रिक्त, *label_base = रिक्त;
	संरचना stash_info info = STASH_INFO_INIT;
	संरचना option options[] = {
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

	यदि (get_stash_info(&info, argc, argv))
		गच्छ cleanup;

	ret = do_apply_stash(prefix, &info, index, quiet,
			     label_ours, label_theirs, label_base);
cleanup:
	free_stash_info(&info);
	फर्काऊ ret;
}

स्थिर पूर्णाङ्क reject_reflog_ent(अचर वर्ण *refname UNUSED,
			     संरचना object_id *ooid UNUSED,
			     संरचना object_id *noid UNUSED,
			     अचर वर्ण *email UNUSED,
			     timestamp_t timestamp UNUSED,
			     पूर्णाङ्क tz UNUSED, अचर वर्ण *message UNUSED,
			     शून्य *cb_data UNUSED)
{
	फर्काऊ 1;
}

स्थिर पूर्णाङ्क reflog_is_empty(अचर वर्ण *refname)
{
	फर्काऊ !refs_for_each_reflog_ent(get_main_ref_store(the_repository),
					 refname, reject_reflog_ent, रिक्त);
}

स्थिर पूर्णाङ्क do_drop_stash(संरचना stash_info *info, पूर्णाङ्क quiet)
{
	यदि (!reflog_delete(info->revision.buf,
			   EXPIRE_REFLOGS_REWRITE | EXPIRE_REFLOGS_UPDATE_REF,
			   0)) {
		यदि (!quiet)
			printf_ln(_("Dropped %s (%s)"), info->revision.buf,
				  oid_to_hex(&info->w_commit));
	} अन्यथा {
		फर्काऊ error(_("%s: Could not drop stash entry"),
			     info->revision.buf);
	}

	यदि (reflog_is_empty(ref_stash))
		do_clear_stash();

	फर्काऊ 0;
}

स्थिर पूर्णाङ्क get_stash_info_assert(संरचना stash_info *info, पूर्णाङ्क argc,
				 अचर वर्ण **argv)
{
	पूर्णाङ्क ret = get_stash_info(info, argc, argv);

	यदि (ret < 0)
		फर्काऊ ret;

	यदि (!info->is_stash_ref)
		फर्काऊ error(_("'%s' is not a stash reference"), info->revision.buf);

	फर्काऊ 0;
}

स्थिर पूर्णाङ्क drop_stash(पूर्णाङ्क argc, अचर वर्ण **argv, अचर वर्ण *prefix,
		      संरचना repository *repo UNUSED)
{
	पूर्णाङ्क ret = -1;
	पूर्णाङ्क quiet = 0;
	संरचना stash_info info = STASH_INFO_INIT;
	संरचना option options[] = {
		OPT__QUIET(&quiet, N_("be quiet, only report errors")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_drop_usage, 0);

	यदि (get_stash_info_assert(&info, argc, argv))
		गच्छ cleanup;

	ret = do_drop_stash(&info, quiet);
cleanup:
	free_stash_info(&info);
	फर्काऊ ret;
}

स्थिर पूर्णाङ्क झिक_stash(पूर्णाङ्क argc, अचर वर्ण **argv, अचर वर्ण *prefix,
		     संरचना repository *repo UNUSED)
{
	पूर्णाङ्क ret = -1;
	पूर्णाङ्क index = use_index;
	पूर्णाङ्क quiet = 0;
	संरचना stash_info info = STASH_INFO_INIT;
	संरचना option options[] = {
		OPT__QUIET(&quiet, N_("be quiet, only report errors")),
		OPT_BOOL(0, "index", &index,
			 N_("attempt to recreate the index")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_झिक_usage, 0);

	यदि (get_stash_info_assert(&info, argc, argv))
		गच्छ cleanup;

	यदि ((ret = do_apply_stash(prefix, &info, index, quiet,
				  रिक्त, रिक्त, रिक्त)))
		printf_ln(_("The stash entry is kept in case "
			    "you need it again."));
	अन्यथा
		ret = do_drop_stash(&info, quiet);

cleanup:
	free_stash_info(&info);
	फर्काऊ ret;
}

स्थिर पूर्णाङ्क branch_stash(पूर्णाङ्क argc, अचर वर्ण **argv, अचर वर्ण *prefix,
			संरचना repository *repo UNUSED)
{
	पूर्णाङ्क ret = -1;
	अचर वर्ण *branch = रिक्त;
	संरचना stash_info info = STASH_INFO_INIT;
	संरचना child_process cp = CHILD_PROCESS_INIT;
	संरचना option options[] = {
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_branch_usage, 0);

	यदि (!argc) {
		fprintf_ln(मानक_त्रुटि, _("No branch name specified"));
		फर्काऊ -1;
	}

	branch = argv[0];

	यदि (get_stash_info(&info, argc - 1, argv + 1))
		गच्छ cleanup;

	cp.git_cmd = 1;
	strvec_pushl(&cp.args, "checkout", "-b", रिक्त);
	strvec_थप(&cp.args, branch);
	strvec_थप(&cp.args, oid_to_hex(&info.b_commit));
	ret = run_command(&cp);
	यदि (!ret)
		ret = do_apply_stash(prefix, &info, 1, 0,
				     रिक्त, रिक्त, रिक्त);
	यदि (!ret && info.is_stash_ref)
		ret = do_drop_stash(&info, 0);

cleanup:
	free_stash_info(&info);
	फर्काऊ ret;
}

स्थिर पूर्णाङ्क list_stash(पूर्णाङ्क argc, अचर वर्ण **argv, अचर वर्ण *prefix,
		      संरचना repository *repo UNUSED)
{
	संरचना child_process cp = CHILD_PROCESS_INIT;
	संरचना option options[] = {
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_list_usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT);

	यदि (!refs_ref_exists(get_main_ref_store(the_repository), ref_stash))
		फर्काऊ 0;

	cp.git_cmd = 1;
	strvec_pushl(&cp.args, "log", "--format=%gd: %gs", "-g",
		     "--first-parent", रिक्त);
	strvec_pushv(&cp.args, argv);
	strvec_थप(&cp.args, ref_stash);
	strvec_थप(&cp.args, "--");
	फर्काऊ run_command(&cp);
}

स्थिर पूर्णाङ्क git_stash_config(अचर वर्ण *var, अचर वर्ण *मान,
			    अचर संरचना config_context *ctx, शून्य *cb)
{
	यदि (!म_भेद(var, "stash.showstat")) {
		show_stat = git_config_bool(var, मान);
		फर्काऊ 0;
	}
	यदि (!म_भेद(var, "stash.showpatch")) {
		show_patch = git_config_bool(var, मान);
		फर्काऊ 0;
	}
	यदि (!म_भेद(var, "stash.showincludeuntracked")) {
		show_include_untracked = git_config_bool(var, मान);
		फर्काऊ 0;
	}
	यदि (!म_भेद(var, "stash.index")) {
		use_index = git_config_bool(var, मान);
		फर्काऊ 0;
	}
	फर्काऊ git_diff_basic_config(var, मान, ctx, cb);
}

स्थिर शून्य diff_include_untracked(अचर संरचना stash_info *info, संरचना diff_options *diff_opt)
{
	अचर संरचना object_id *oid[] = { &info->w_commit, &info->u_रूख };
	संरचना रूख *रूख[ARRAY_SIZE(oid)];
	संरचना रूख_desc रूख_desc[ARRAY_SIZE(oid)];
	संरचना unpack_trees_options unpack_रूख_opt = { 0 };

	लागि (माप_प्रकार i = 0; i < ARRAY_SIZE(oid); i++) {
		रूख[i] = repo_parse_रूख_indirect(the_repository, oid[i]);
		यदि (repo_parse_रूख(the_repository, रूख[i]) < 0)
			die(_("failed to parse tree"));
		init_रूख_desc(&रूख_desc[i], &रूख[i]->object.oid,
			       रूख[i]->buffer, रूख[i]->size);
	}

	unpack_रूख_opt.head_idx = -1;
	unpack_रूख_opt.src_index = the_repository->index;
	unpack_रूख_opt.dst_index = the_repository->index;
	unpack_रूख_opt.merge = 1;
	unpack_रूख_opt.fn = stash_worktree_untracked_merge;

	यदि (unpack_trees(ARRAY_SIZE(रूख_desc), रूख_desc, &unpack_रूख_opt))
		die(_("failed to unpack trees"));

	do_diff_cache(&info->b_commit, diff_opt);
}

स्थिर पूर्णाङ्क show_stash(पूर्णाङ्क argc, अचर वर्ण **argv, अचर वर्ण *prefix,
		      संरचना repository *repo UNUSED)
{
	पूर्णाङ्क i;
	पूर्णाङ्क ret = -1;
	संरचना stash_info info = STASH_INFO_INIT;
	संरचना rev_info rev;
	संरचना strvec stash_args = STRVEC_INIT;
	संरचना strvec revision_args = STRVEC_INIT;
	गणना {
		UNTRACKED_NONE,
		UNTRACKED_INCLUDE,
		UNTRACKED_ONLY
	} show_untracked = show_include_untracked ? UNTRACKED_INCLUDE : UNTRACKED_NONE;
	संरचना option options[] = {
		OPT_SET_INT('u', "include-untracked", &show_untracked,
			    N_("include untracked files in the stash"),
			    UNTRACKED_INCLUDE),
		OPT_SET_INT_F(0, "only-untracked", &show_untracked,
			      N_("only show untracked files in the stash"),
			      UNTRACKED_ONLY, PARSE_OPT_NONEG),
		OPT_END()
	};
	पूर्णाङ्क do_usage = 0;

	init_diff_ui_defaults();
	repo_config(the_repository, git_diff_ui_config, रिक्त);
	repo_init_revisions(the_repository, &rev, prefix);

	argc = parse_options(argc, argv, prefix, options, git_stash_show_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	strvec_थप(&revision_args, argv[0]);
	लागि (i = 1; i < argc; i++) {
		यदि (argv[i][0] != '-')
			strvec_थप(&stash_args, argv[i]);
		अन्यथा
			strvec_थप(&revision_args, argv[i]);
	}

	यदि (get_stash_info(&info, stash_args.nr, stash_args.v))
		गच्छ cleanup;

	/*
	 * The config settings are applied only if there are not passed
	 * any options.
	 */
	यदि (revision_args.nr == 1) {
		यदि (show_stat)
			rev.diffopt.output_format = DIFF_FORMAT_DIFFSTAT;

		यदि (show_patch)
			rev.diffopt.output_format |= DIFF_FORMAT_PATCH;

		यदि (!show_stat && !show_patch) {
			ret = 0;
			गच्छ cleanup;
		}
	}

	setup_revisions_from_strvec(&revision_args, &rev, रिक्त);
	यदि (revision_args.nr > 1)
		गच्छ usage;
	यदि (!rev.diffopt.output_format) {
		rev.diffopt.output_format = DIFF_FORMAT_PATCH;
		diff_setup_done(&rev.diffopt);
	}

	rev.diffopt.flags.recursive = 1;
	setup_diff_pager(&rev.diffopt);
	विकल्प (show_untracked) {
	पक्ष UNTRACKED_NONE:
		diff_रूख_oid(&info.b_commit, &info.w_commit, "", &rev.diffopt);
		रोक;
	पक्ष UNTRACKED_ONLY:
		यदि (info.has_u)
			diff_जरा_रूख_oid(&info.u_रूख, "", &rev.diffopt);
		रोक;
	पक्ष UNTRACKED_INCLUDE:
		यदि (info.has_u)
			diff_include_untracked(&info, &rev.diffopt);
		अन्यथा
			diff_रूख_oid(&info.b_commit, &info.w_commit, "", &rev.diffopt);
		रोक;
	}
	log_रूख_diff_flush(&rev);

	ret = diff_result_code(&rev);

cleanup:
	strvec_clear(&revision_args);
	strvec_clear(&stash_args);
	free_stash_info(&info);
	release_revisions(&rev);
	यदि (do_usage)
		usage_with_options(git_stash_show_usage, options);
	फर्काऊ ret;
usage:
	do_usage = 1;
	गच्छ cleanup;
}

स्थिर पूर्णाङ्क do_store_stash(अचर संरचना object_id *w_commit, अचर वर्ण *stash_सन्देश,
			  पूर्णाङ्क quiet)
{
	संरचना stash_info info;
	वर्ण revision[GIT_MAX_HEXSZ];

	oid_to_hex_r(revision, w_commit);
	assert_stash_like(&info, revision);

	यदि (!stash_सन्देश)
		stash_सन्देश = "Created via \"git stash store\".";

	यदि (refs_update_ref(get_main_ref_store(the_repository), stash_सन्देश, ref_stash, w_commit, रिक्त,
			    REF_FORCE_CREATE_REFLOG,
			    quiet ? UPDATE_REFS_QUIET_ON_ERR :
			    UPDATE_REFS_सन्देश_ON_ERR)) {
		यदि (!quiet) {
			fprintf_ln(मानक_त्रुटि, _("Cannot update %s with %s"),
				   ref_stash, oid_to_hex(w_commit));
		}
		फर्काऊ -1;
	}

	फर्काऊ 0;
}

स्थिर पूर्णाङ्क store_stash(पूर्णाङ्क argc, अचर वर्ण **argv, अचर वर्ण *prefix,
		       संरचना repository *repo UNUSED)
{
	पूर्णाङ्क quiet = 0;
	अचर वर्ण *stash_सन्देश = रिक्त;
	संरचना object_id obj;
	संरचना option options[] = {
		OPT__QUIET(&quiet, N_("be quiet")),
		OPT_STRING('m', "message", &stash_सन्देश, "message",
			   N_("stash message")),
		OPT_END()
	};
	पूर्णाङ्क ret;

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_store_usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT);

	यदि (argc != 1) {
		यदि (!quiet)
			fprintf_ln(मानक_त्रुटि, _("\"git stash store\" requires one "
					     "<commit> argument"));
		फर्काऊ -1;
	}

	यदि (repo_get_oid_with_flags(the_repository, argv[0], &obj,
				    quiet ? GET_OID_QUIETLY : 0)) {
		यदि (!quiet)
			fprintf_ln(मानक_त्रुटि, _("Cannot update %s with %s"),
					     ref_stash, argv[0]);
		ret = -1;
		गच्छ out;
	}

	ret = do_store_stash(&obj, stash_सन्देश, quiet);

out:
	फर्काऊ ret;
}

स्थिर शून्य add_pathspecs(संरचना strvec *args,
			  अचर संरचना pathspec *ps) {
	पूर्णाङ्क i;

	लागि (i = 0; i < ps->nr; i++)
		strvec_थप(args, ps->वस्तुहरू[i].original);
}

/*
 * `untracked_files` will be filled with the names of untracked files.
 * The return value is:
 *
 * = 0 if there are not any untracked files
 * > 0 if there are untracked files
 */
स्थिर पूर्णाङ्क get_untracked_files(अचर संरचना pathspec *ps, पूर्णाङ्क include_untracked,
			       संरचना strbuf *untracked_files)
{
	पूर्णाङ्क i;
	पूर्णाङ्क found = 0;
	संरचना dir_struct dir = DIR_INIT;

	यदि (include_untracked != INCLUDE_ALL_FILES)
		setup_standard_excludes(&dir);

	fill_directory(&dir, the_repository->index, ps);
	लागि (i = 0; i < dir.nr; i++) {
		संरचना dir_entry *ent = dir.entries[i];
		found++;
		strbuf_addstr(untracked_files, ent->name);
		/* NUL-terminate: will be fed to update-index -z */
		strbuf_addch(untracked_files, '\0');
	}

	dir_clear(&dir);
	फर्काऊ found;
}

/*
 * The return value of `check_changes_tracked_files()` can be:
 *
 * < 0 if there was an error
 * = 0 if there are no changes.
 * > 0 if there are changes.
 */
स्थिर पूर्णाङ्क check_changes_tracked_files(अचर संरचना pathspec *ps)
{
	संरचना rev_info rev;
	संरचना object_id dummy;
	पूर्णाङ्क ret = 0;

	/* No initial commit. */
	यदि (repo_get_oid(the_repository, "HEAD", &dummy))
		फर्काऊ -1;

	यदि (repo_read_index(the_repository) < 0)
		फर्काऊ -1;

	repo_init_revisions(the_repository, &rev, रिक्त);
	copy_pathspec(&rev.prune_data, ps);

	rev.diffopt.flags.quick = 1;
	rev.diffopt.flags.ignore_submodules = 1;
	rev.abbrev = 0;

	add_head_to_pending(&rev);
	diff_setup_done(&rev.diffopt);

	run_diff_index(&rev, DIFF_INDEX_CACHED);
	यदि (diff_result_code(&rev)) {
		ret = 1;
		गच्छ done;
	}

	run_diff_files(&rev, 0);
	यदि (diff_result_code(&rev)) {
		ret = 1;
		गच्छ done;
	}

done:
	release_revisions(&rev);
	फर्काऊ ret;
}

/*
 * The function will fill `untracked_files` with the names of untracked files
 * It will return 1 if there were any changes and 0 if there were not.
 */
स्थिर पूर्णाङ्क check_changes(अचर संरचना pathspec *ps, पूर्णाङ्क include_untracked,
			 संरचना strbuf *untracked_files)
{
	पूर्णाङ्क ret = 0;
	यदि (check_changes_tracked_files(ps))
		ret = 1;

	यदि (include_untracked && get_untracked_files(ps, include_untracked,
						     untracked_files))
		ret = 1;

	फर्काऊ ret;
}

स्थिर पूर्णाङ्क save_untracked_files(संरचना stash_info *info, संरचना strbuf *सन्देश,
				संरचना strbuf *files)
{
	पूर्णाङ्क ret = 0;
	संरचना strbuf untracked_सन्देश = STRBUF_INIT;
	संरचना child_process cp_upd_index = CHILD_PROCESS_INIT;
	संरचना index_state istate = INDEX_STATE_INIT(the_repository);

	cp_upd_index.git_cmd = 1;
	strvec_pushl(&cp_upd_index.args, "update-index", "-z", "--add",
		     "--remove", "--stdin", रिक्त);
	strvec_pushf(&cp_upd_index.env, "GIT_INDEX_FILE=%s",
			 stash_index_path.buf);

	strbuf_addf(&untracked_सन्देश, "untracked files on %s\n", सन्देश->buf);
	यदि (pipe_command(&cp_upd_index, files->buf, files->len, रिक्त, 0,
			 रिक्त, 0)) {
		ret = -1;
		गच्छ done;
	}

	यदि (write_index_as_रूख(&info->u_रूख, &istate, stash_index_path.buf, 0,
				रिक्त)) {
		ret = -1;
		गच्छ done;
	}

	यदि (commit_रूख(untracked_सन्देश.buf, untracked_सन्देश.len,
			&info->u_रूख, रिक्त, &info->u_commit, रिक्त, रिक्त)) {
		ret = -1;
		गच्छ done;
	}

done:
	release_index(&istate);
	strbuf_release(&untracked_सन्देश);
	remove_path(stash_index_path.buf);
	फर्काऊ ret;
}

स्थिर पूर्णाङ्क stash_staged(संरचना stash_info *info, संरचना strbuf *out_patch,
			पूर्णाङ्क quiet)
{
	पूर्णाङ्क ret = 0;
	संरचना child_process cp_diff_रूख = CHILD_PROCESS_INIT;
	संरचना index_state istate = INDEX_STATE_INIT(the_repository);

	यदि (write_index_as_रूख(&info->w_रूख, &istate, the_repository->index_file,
				0, रिक्त)) {
		ret = -1;
		गच्छ done;
	}

	cp_diff_रूख.git_cmd = 1;
	strvec_pushl(&cp_diff_रूख.args, "diff-tree", "-p", "--binary",
		     "--no-color",
		     "-U1", "HEAD", oid_to_hex(&info->w_रूख), "--", रिक्त);
	यदि (pipe_command(&cp_diff_रूख, रिक्त, 0, out_patch, 0, रिक्त, 0)) {
		ret = -1;
		गच्छ done;
	}

	यदि (!out_patch->len) {
		यदि (!quiet)
			fprintf_ln(मानक_त्रुटि, _("No staged changes"));
		ret = 1;
	}

done:
	release_index(&istate);
	फर्काऊ ret;
}

स्थिर पूर्णाङ्क stash_patch(संरचना stash_info *info, अचर संरचना pathspec *ps,
		       संरचना strbuf *out_patch, पूर्णाङ्क quiet,
		       संरचना interactive_options *interactive_opts)
{
	पूर्णाङ्क ret = 0;
	संरचना child_process cp_diff_रूख = CHILD_PROCESS_INIT;
	संरचना commit *head_commit;
	अचर संरचना object_id *head_रूख;
	संरचना index_state istate = INDEX_STATE_INIT(the_repository);
	वर्ण *old_index_env = रिक्त, *old_repo_index_file;

	remove_path(stash_index_path.buf);

	head_commit = lookup_commit(the_repository, &info->b_commit);
	यदि (!head_commit || repo_parse_commit(the_repository, head_commit)) {
		ret = -1;
		गच्छ done;
	}
	head_रूख = get_commit_रूख_oid(head_commit);
	यदि (!head_रूख) {
		ret = -1;
		गच्छ done;
	}

	यदि (create_index_from_रूख(head_रूख, stash_index_path.buf)) {
		ret = -1;
		गच्छ done;
	}

	/* Find out what the user wants. */
	old_repo_index_file = the_repository->index_file;
	the_repository->index_file = stash_index_path.buf;
	old_index_env = xstrdup_or_null(दो_पर्या(INDEX_ENVIRONMENT));
	setenv(INDEX_ENVIRONMENT, the_repository->index_file, 1);

	ret = !!run_add_p(the_repository, ADD_P_STASH, interactive_opts, रिक्त, ps, 0);

	the_repository->index_file = old_repo_index_file;
	यदि (old_index_env && *old_index_env)
		setenv(INDEX_ENVIRONMENT, old_index_env, 1);
	अन्यथा
		unsetenv(INDEX_ENVIRONMENT);
	FREE_AND_NULL(old_index_env);

	/* State of the working tree. */
	यदि (write_index_as_रूख(&info->w_रूख, &istate, stash_index_path.buf, 0,
				रिक्त)) {
		ret = -1;
		गच्छ done;
	}

	cp_diff_रूख.git_cmd = 1;
	strvec_pushl(&cp_diff_रूख.args, "diff-tree", "-p", "-U1", "HEAD",
		     "--no-color",
		     oid_to_hex(&info->w_रूख), "--", रिक्त);
	यदि (pipe_command(&cp_diff_रूख, रिक्त, 0, out_patch, 0, रिक्त, 0)) {
		ret = -1;
		गच्छ done;
	}

	यदि (!out_patch->len) {
		यदि (!quiet)
			fprintf_ln(मानक_त्रुटि, _("No changes selected"));
		ret = 1;
	}

done:
	release_index(&istate);
	remove_path(stash_index_path.buf);
	फर्काऊ ret;
}

स्थिर पूर्णाङ्क stash_working_रूख(संरचना stash_info *info, अचर संरचना pathspec *ps)
{
	पूर्णाङ्क ret = 0;
	संरचना rev_info rev;
	संरचना child_process cp_upd_index = CHILD_PROCESS_INIT;
	संरचना strbuf diff_output = STRBUF_INIT;
	संरचना index_state istate = INDEX_STATE_INIT(the_repository);

	repo_init_revisions(the_repository, &rev, रिक्त);
	copy_pathspec(&rev.prune_data, ps);

	set_alternate_index_output(stash_index_path.buf);
	यदि (reset_रूख(&info->i_रूख, 0, 0)) {
		ret = -1;
		गच्छ done;
	}
	set_alternate_index_output(रिक्त);

	rev.diffopt.output_format = DIFF_FORMAT_CALLBACK;
	rev.diffopt.format_callback = add_diff_to_buf;
	rev.diffopt.format_callback_data = &diff_output;

	यदि (repo_read_index_preload(the_repository, &rev.diffopt.pathspec, 0) < 0) {
		ret = -1;
		गच्छ done;
	}

	add_pending_object(&rev, parse_object(the_repository, &info->b_commit),
			   "");
	run_diff_index(&rev, 0);

	cp_upd_index.git_cmd = 1;
	strvec_pushl(&cp_upd_index.args, "update-index",
		     "--ignore-skip-worktree-entries",
		     "-z", "--add", "--remove", "--stdin", रिक्त);
	strvec_pushf(&cp_upd_index.env, "GIT_INDEX_FILE=%s",
		     stash_index_path.buf);

	यदि (pipe_command(&cp_upd_index, diff_output.buf, diff_output.len,
			 रिक्त, 0, रिक्त, 0)) {
		ret = -1;
		गच्छ done;
	}

	यदि (write_index_as_रूख(&info->w_रूख, &istate, stash_index_path.buf, 0,
				रिक्त)) {
		ret = -1;
		गच्छ done;
	}

done:
	release_index(&istate);
	release_revisions(&rev);
	strbuf_release(&diff_output);
	remove_path(stash_index_path.buf);
	फर्काऊ ret;
}

स्थिर पूर्णाङ्क do_create_stash(अचर संरचना pathspec *ps, संरचना strbuf *stash_सन्देश_buf,
			   पूर्णाङ्क include_untracked, पूर्णाङ्क patch_mode,
			   संरचना interactive_options *interactive_opts,
			   पूर्णाङ्क only_staged, संरचना stash_info *info, संरचना strbuf *patch,
			   पूर्णाङ्क quiet)
{
	पूर्णाङ्क ret = 0;
	पूर्णाङ्क flags = 0;
	पूर्णाङ्क untracked_commit_option = 0;
	अचर वर्ण *head_short_sha1 = रिक्त;
	अचर वर्ण *branch_ref = रिक्त;
	अचर वर्ण *branch_name = "(no branch)";
	वर्ण *branch_name_buf = रिक्त;
	संरचना commit *head_commit = रिक्त;
	संरचना commit_list *parents = रिक्त;
	संरचना strbuf सन्देश = STRBUF_INIT;
	संरचना strbuf commit_रूख_label = STRBUF_INIT;
	संरचना strbuf untracked_files = STRBUF_INIT;

	prepare_fallback_ident("git stash", "git@stash");

	repo_read_index_preload(the_repository, रिक्त, 0);
	यदि (repo_refresh_and_write_index(the_repository, REFRESH_QUIET, 0, 0,
					 रिक्त, रिक्त, रिक्त) < 0) {
		ret = error(_("could not write index"));
		गच्छ done;
	}

	यदि (repo_get_oid(the_repository, "HEAD", &info->b_commit)) {
		यदि (!quiet)
			fprintf_ln(मानक_त्रुटि, _("You do not have "
					     "the initial commit yet"));
		ret = -1;
		गच्छ done;
	} अन्यथा {
		head_commit = lookup_commit(the_repository, &info->b_commit);
	}

	यदि (!check_changes(ps, include_untracked, &untracked_files)) {
		ret = 1;
		गच्छ done;
	}

	branch_ref = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
					     "HEAD", 0, रिक्त, &flags);

	यदि (flags & REF_ISSYMREF) {
		यदि (skip_prefix(branch_ref, "refs/heads/", &branch_name))
			branch_name = branch_name_buf = xstrdup(branch_name);
	}

	head_short_sha1 = repo_find_unique_abbrev(the_repository,
						  &head_commit->object.oid,
						  DEFAULT_ABBREV);
	strbuf_addf(&सन्देश, "%s: %s ", branch_name, head_short_sha1);
	pp_commit_easy(CMIT_FMT_ONELINE, head_commit, &सन्देश);

	strbuf_addf(&commit_रूख_label, "index on %s\n", सन्देश.buf);
	commit_list_घुसाउ(head_commit, &parents);
	यदि (write_index_as_रूख(&info->i_रूख, the_repository->index,
				repo_get_index_file(the_repository), 0, रिक्त) ||
	    commit_रूख(commit_रूख_label.buf, commit_रूख_label.len,
			&info->i_रूख, parents, &info->i_commit, रिक्त, रिक्त)) {
		यदि (!quiet)
			fprintf_ln(मानक_त्रुटि, _("Cannot save the current "
					     "index state"));
		ret = -1;
		गच्छ done;
	}

	commit_list_free(parents);
	parents = रिक्त;

	यदि (include_untracked) {
		यदि (save_untracked_files(info, &सन्देश, &untracked_files)) {
			यदि (!quiet)
				fprintf_ln(मानक_त्रुटि, _("Cannot save "
						     "the untracked files"));
			ret = -1;
			गच्छ done;
		}
		untracked_commit_option = 1;
	}
	यदि (patch_mode) {
		ret = stash_patch(info, ps, patch, quiet, interactive_opts);
		यदि (ret < 0) {
			यदि (!quiet)
				fprintf_ln(मानक_त्रुटि, _("Cannot save the current "
						     "worktree state"));
			गच्छ done;
		} अन्यथा यदि (ret > 0) {
			गच्छ done;
		}
	} अन्यथा यदि (only_staged) {
		ret = stash_staged(info, patch, quiet);
		यदि (ret < 0) {
			यदि (!quiet)
				fprintf_ln(मानक_त्रुटि, _("Cannot save the current "
						     "staged state"));
			गच्छ done;
		} अन्यथा यदि (ret > 0) {
			गच्छ done;
		}
	} अन्यथा {
		यदि (stash_working_रूख(info, ps)) {
			यदि (!quiet)
				fprintf_ln(मानक_त्रुटि, _("Cannot save the current "
						     "worktree state"));
			ret = -1;
			गच्छ done;
		}
	}

	यदि (!stash_सन्देश_buf->len)
		strbuf_addf(stash_सन्देश_buf, "WIP on %s", सन्देश.buf);
	अन्यथा
		strbuf_insertf(stash_सन्देश_buf, 0, "On %s: ", branch_name);

	यदि (untracked_commit_option)
		commit_list_घुसाउ(lookup_commit(the_repository,
						 &info->u_commit),
				   &parents);
	commit_list_घुसाउ(lookup_commit(the_repository, &info->i_commit),
			   &parents);
	commit_list_घुसाउ(head_commit, &parents);

	यदि (commit_रूख(stash_सन्देश_buf->buf, stash_सन्देश_buf->len, &info->w_रूख,
			parents, &info->w_commit, रिक्त, रिक्त)) {
		यदि (!quiet)
			fprintf_ln(मानक_त्रुटि, _("Cannot record "
					     "working tree state"));
		ret = -1;
		गच्छ done;
	}

done:
	strbuf_release(&commit_रूख_label);
	strbuf_release(&सन्देश);
	strbuf_release(&untracked_files);
	commit_list_free(parents);
	मुक्त(branch_name_buf);
	फर्काऊ ret;
}

स्थिर पूर्णाङ्क create_stash(पूर्णाङ्क argc, अचर वर्ण **argv, अचर वर्ण *prefix UNUSED,
			संरचना repository *repo UNUSED)
{
	पूर्णाङ्क ret;
	संरचना strbuf stash_सन्देश_buf = STRBUF_INIT;
	संरचना stash_info info = STASH_INFO_INIT;
	संरचना pathspec ps;

	/* Starting with argv[1], since argv[0] is "create" */
	strbuf_join_argv(&stash_सन्देश_buf, argc - 1, ++argv, ' ');

	स_रखो(&ps, 0, परिमाण(ps));
	यदि (!check_changes_tracked_files(&ps))
		फर्काऊ 0;

	ret = do_create_stash(&ps, &stash_सन्देश_buf, 0, 0, रिक्त, 0, &info,
			      रिक्त, 0);
	यदि (!ret)
		printf_ln("%s", oid_to_hex(&info.w_commit));

	free_stash_info(&info);
	strbuf_release(&stash_सन्देश_buf);
	फर्काऊ ret;
}

स्थिर पूर्णाङ्क do_थप_stash(अचर संरचना pathspec *ps, अचर वर्ण *stash_सन्देश, पूर्णाङ्क quiet,
			 पूर्णाङ्क keep_index, पूर्णाङ्क patch_mode,
			 संरचना interactive_options *interactive_opts,
			 पूर्णाङ्क include_untracked, पूर्णाङ्क only_staged)
{
	पूर्णाङ्क ret = 0;
	संरचना stash_info info = STASH_INFO_INIT;
	संरचना strbuf patch = STRBUF_INIT;
	संरचना strbuf stash_सन्देश_buf = STRBUF_INIT;
	संरचना strbuf untracked_files = STRBUF_INIT;
	संरचना strbuf out = STRBUF_INIT;

	यदि (patch_mode && keep_index == -1)
		keep_index = 1;

	यदि (patch_mode && include_untracked) {
		fprintf_ln(मानक_त्रुटि, _("Can't use --patch and --include-untracked"
				     " or --all at the same time"));
		ret = -1;
		गच्छ done;
	}

	/* --patch overrides --staged */
	यदि (patch_mode)
		only_staged = 0;

	यदि (only_staged && include_untracked) {
		fprintf_ln(मानक_त्रुटि, _("Can't use --staged and --include-untracked"
				     " or --all at the same time"));
		ret = -1;
		गच्छ done;
	}

	repo_read_index_preload(the_repository, रिक्त, 0);
	यदि (!include_untracked && ps->nr) {
		वर्ण *ps_matched = xcalloc(ps->nr, 1);

		यदि (pathspec_needs_expanded_index(the_repository->index, ps))
			ensure_full_index(the_repository->index);
		लागि (माप_प्रकार i = 0; i < the_repository->index->cache_nr; i++)
			ce_path_match(the_repository->index, the_repository->index->cache[i], ps,
				      ps_matched);

		यदि (report_path_error(ps_matched, ps)) {
			fprintf_ln(मानक_त्रुटि, _("Did you forget to 'git add'?"));
			ret = -1;
			मुक्त(ps_matched);
			गच्छ done;
		}
		मुक्त(ps_matched);
	}

	यदि (repo_refresh_and_write_index(the_repository, REFRESH_QUIET, 0, 0,
					 रिक्त, रिक्त, रिक्त)) {
		ret = error(_("could not write index"));
		गच्छ done;
	}

	यदि (!check_changes(ps, include_untracked, &untracked_files)) {
		यदि (!quiet)
			printf_ln(_("No local changes to save"));
		गच्छ done;
	}

	यदि (!refs_reflog_exists(get_main_ref_store(the_repository), ref_stash) && do_clear_stash()) {
		ret = -1;
		यदि (!quiet)
			fprintf_ln(मानक_त्रुटि, _("Cannot initialize stash"));
		गच्छ done;
	}

	यदि (stash_सन्देश)
		strbuf_addstr(&stash_सन्देश_buf, stash_सन्देश);
	यदि (do_create_stash(ps, &stash_सन्देश_buf, include_untracked, patch_mode,
			    interactive_opts, only_staged, &info, &patch, quiet)) {
		ret = -1;
		गच्छ done;
	}

	यदि (do_store_stash(&info.w_commit, stash_सन्देश_buf.buf, 1)) {
		ret = -1;
		यदि (!quiet)
			fprintf_ln(मानक_त्रुटि, _("Cannot save the current status"));
		गच्छ done;
	}

	यदि (!quiet)
		printf_ln(_("Saved working directory and index state %s"),
			  stash_सन्देश_buf.buf);

	यदि (!(patch_mode || only_staged)) {
		यदि (include_untracked && !ps->nr) {
			संरचना child_process cp = CHILD_PROCESS_INIT;

			cp.git_cmd = 1;
			यदि (startup_info->original_cwd) {
				cp.dir = startup_info->original_cwd;
				strvec_pushf(&cp.env, "%s=%s",
					     GIT_WORK_रूख_ENVIRONMENT,
					     the_repository->worktree);
			}
			strvec_pushl(&cp.args, "clean", "--force",
				     "--quiet", "-d", ":/", रिक्त);
			यदि (include_untracked == INCLUDE_ALL_FILES)
				strvec_थप(&cp.args, "-x");
			यदि (run_command(&cp)) {
				ret = -1;
				गच्छ done;
			}
		}
		discard_index(the_repository->index);
		यदि (ps->nr) {
			संरचना child_process cp_add = CHILD_PROCESS_INIT;
			संरचना child_process cp_diff = CHILD_PROCESS_INIT;
			संरचना child_process cp_apply = CHILD_PROCESS_INIT;

			cp_add.git_cmd = 1;
			strvec_थप(&cp_add.args, "add");
			यदि (!include_untracked)
				strvec_थप(&cp_add.args, "-u");
			यदि (include_untracked == INCLUDE_ALL_FILES)
				strvec_थप(&cp_add.args, "--force");
			strvec_थप(&cp_add.args, "--");
			add_pathspecs(&cp_add.args, ps);
			यदि (run_command(&cp_add)) {
				ret = -1;
				गच्छ done;
			}

			cp_diff.git_cmd = 1;
			strvec_pushl(&cp_diff.args, "diff-index", "-p",
				     "--no-color",
				     "--cached", "--binary", "HEAD", "--",
				     रिक्त);
			add_pathspecs(&cp_diff.args, ps);
			यदि (pipe_command(&cp_diff, रिक्त, 0, &out, 0, रिक्त, 0)) {
				ret = -1;
				गच्छ done;
			}

			cp_apply.git_cmd = 1;
			strvec_pushl(&cp_apply.args, "apply", "--index",
				     "-R", रिक्त);
			यदि (pipe_command(&cp_apply, out.buf, out.len, रिक्त, 0,
					 रिक्त, 0)) {
				ret = -1;
				गच्छ done;
			}
		} अन्यथा {
			संरचना child_process cp = CHILD_PROCESS_INIT;
			cp.git_cmd = 1;
			/* BUG: this nukes untracked files in the way */
			strvec_pushl(&cp.args, "reset", "--hard", "-q",
				     "--no-recurse-submodules", रिक्त);
			यदि (run_command(&cp)) {
				ret = -1;
				गच्छ done;
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
		यदि (keep_index == 1 && !is_null_oid(&info.i_रूख) &&
		    !is_empty_रूख_oid(&info.i_रूख, the_repository->hash_algo)) {
			संरचना child_process cp = CHILD_PROCESS_INIT;

			cp.git_cmd = 1;
			strvec_pushl(&cp.args, "checkout", "--no-overlay",
				     oid_to_hex(&info.i_रूख), "--", रिक्त);
			यदि (!ps->nr)
				strvec_थप(&cp.args, ":/");
			अन्यथा
				add_pathspecs(&cp.args, ps);
			यदि (run_command(&cp)) {
				ret = -1;
				गच्छ done;
			}
		}
		गच्छ done;
	} अन्यथा {
		संरचना child_process cp = CHILD_PROCESS_INIT;

		cp.git_cmd = 1;
		strvec_pushl(&cp.args, "apply", "-R", रिक्त);

		यदि (pipe_command(&cp, patch.buf, patch.len, रिक्त, 0, रिक्त, 0)) {
			यदि (!quiet)
				fprintf_ln(मानक_त्रुटि, _("Cannot remove "
						     "worktree changes"));
			ret = -1;
			गच्छ done;
		}

		यदि (keep_index < 1) {
			संरचना child_process cp = CHILD_PROCESS_INIT;

			cp.git_cmd = 1;
			strvec_pushl(&cp.args, "reset", "-q", "--refresh", "--",
				     रिक्त);
			add_pathspecs(&cp.args, ps);
			यदि (run_command(&cp)) {
				ret = -1;
				गच्छ done;
			}
		}
		गच्छ done;
	}

done:
	strbuf_release(&patch);
	strbuf_release(&out);
	free_stash_info(&info);
	strbuf_release(&stash_सन्देश_buf);
	strbuf_release(&untracked_files);
	फर्काऊ ret;
}

स्थिर पूर्णाङ्क थप_stash(पूर्णाङ्क argc, अचर वर्ण **argv, अचर वर्ण *prefix,
		      पूर्णाङ्क थप_assumed)
{
	पूर्णाङ्क force_assume = 0;
	पूर्णाङ्क keep_index = -1;
	पूर्णाङ्क only_staged = 0;
	पूर्णाङ्क patch_mode = 0;
	पूर्णाङ्क include_untracked = 0;
	पूर्णाङ्क quiet = 0;
	पूर्णाङ्क pathspec_file_nul = 0;
	अचर वर्ण *stash_सन्देश = रिक्त;
	वर्ण *pathspec_from_file = रिक्त;
	संरचना pathspec ps;
	संरचना interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	संरचना option options[] = {
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
		OPT_STRING('m', "message", &stash_सन्देश, N_("message"),
			   N_("stash message")),
		OPT_PATHSPEC_FROM_FILE(&pathspec_from_file),
		OPT_PATHSPEC_FILE_NUL(&pathspec_file_nul),
		OPT_END()
	};
	पूर्णाङ्क ret;

	यदि (argc) {
		पूर्णाङ्क flags = PARSE_OPT_KEEP_DASHDASH;

		यदि (थप_assumed)
			flags |= PARSE_OPT_STOP_AT_NON_OPTION;

		argc = parse_options(argc, argv, prefix, options,
				     थप_assumed ? git_stash_usage :
				     git_stash_थप_usage, flags);
		force_assume |= patch_mode;
	}

	यदि (argc) {
		यदि (!म_भेद(argv[0], "--")) {
			argc--;
			argv++;
		} अन्यथा यदि (थप_assumed && !force_assume) {
			die("subcommand wasn't specified; 'push' can't be assumed due to unexpected token '%s'",
			    argv[0]);
		}
	}

	parse_pathspec(&ps, 0, PATHSPEC_PREFER_FULL | PATHSPEC_PREFIX_ORIGIN,
		       prefix, argv);

	यदि (pathspec_from_file) {
		यदि (patch_mode)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--patch");

		यदि (only_staged)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--staged");

		यदि (ps.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&ps, 0,
				    PATHSPEC_PREFER_FULL | PATHSPEC_PREFIX_ORIGIN,
				    prefix, pathspec_from_file, pathspec_file_nul);
	} अन्यथा यदि (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	यदि (!patch_mode) {
		यदि (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--patch");
		यदि (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--patch");
		यदि (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--patch");
	}

	यदि (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	यदि (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	ret = do_थप_stash(&ps, stash_सन्देश, quiet, keep_index, patch_mode,
			    &interactive_opts, include_untracked, only_staged);

	clear_pathspec(&ps);
	मुक्त(pathspec_from_file);
	फर्काऊ ret;
}

स्थिर पूर्णाङ्क थप_stash_unassumed(पूर्णाङ्क argc, अचर वर्ण **argv, अचर वर्ण *prefix,
				संरचना repository *repo UNUSED)
{
	फर्काऊ थप_stash(argc, argv, prefix, 0);
}

स्थिर पूर्णाङ्क save_stash(पूर्णाङ्क argc, अचर वर्ण **argv, अचर वर्ण *prefix,
		      संरचना repository *repo UNUSED)
{
	पूर्णाङ्क keep_index = -1;
	पूर्णाङ्क only_staged = 0;
	पूर्णाङ्क patch_mode = 0;
	पूर्णाङ्क include_untracked = 0;
	पूर्णाङ्क quiet = 0;
	पूर्णाङ्क ret = 0;
	अचर वर्ण *stash_सन्देश = रिक्त;
	संरचना pathspec ps;
	संरचना strbuf stash_सन्देश_buf = STRBUF_INIT;
	संरचना interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	संरचना option options[] = {
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
		OPT_STRING('m', "message", &stash_सन्देश, "message",
			   N_("stash message")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_save_usage,
			     PARSE_OPT_KEEP_DASHDASH);

	यदि (argc)
		stash_सन्देश = strbuf_join_argv(&stash_सन्देश_buf, argc, argv, ' ');

	स_रखो(&ps, 0, परिमाण(ps));

	यदि (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	यदि (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	यदि (!patch_mode) {
		यदि (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--patch");
		यदि (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--patch");
		यदि (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--patch");
	}

	ret = do_थप_stash(&ps, stash_सन्देश, quiet, keep_index,
			    patch_mode, &interactive_opts, include_untracked,
			    only_staged);

	strbuf_release(&stash_सन्देश_buf);
	फर्काऊ ret;
}

स्थिर पूर्णाङ्क write_commit_with_parents(संरचना repository *r,
				     संरचना object_id *out,
				     अचर संरचना object_id *oid,
				     संरचना commit_list *parents)
{
	माप_प्रकार author_len, committer_len;
	संरचना commit *this;
	अचर वर्ण *orig_author, *orig_committer;
	वर्ण *author = रिक्त, *committer = रिक्त;
	अचर वर्ण *buffer;
	अचिन्हित दीर्घ bufsize;
	अचर वर्ण *p;
	संरचना strbuf सन्देश = STRBUF_INIT;
	पूर्णाङ्क ret = 0;
	संरचना ident_split id;

	this = lookup_commit_reference(r, oid);
	buffer = repo_get_commit_buffer(r, this, &bufsize);
	orig_author = find_commit_header(buffer, "author", &author_len);
	orig_committer = find_commit_header(buffer, "committer", &committer_len);

	यदि (!orig_author || !orig_committer) {
		ret = error(_("cannot parse commit %s"), oid_to_hex(oid));
		गच्छ out;
	}

	यदि (split_ident_पङ्क्ति(&id, orig_author, author_len) < 0 ||
	    split_ident_पङ्क्ति(&id, orig_committer, committer_len) < 0) {
		ret = error(_("invalid author or committer for %s"), oid_to_hex(oid));
		गच्छ out;
	}

	p = म_माला(buffer, "\n\n");
	strbuf_addstr(&सन्देश, "git stash: ");

	यदि (p)
		strbuf_add(&सन्देश, p + 2, bufsize - (p + 2 - buffer));
	strbuf_complete_पङ्क्ति(&सन्देश);

	author = xmemdupz(orig_author, author_len);
	committer = xmemdupz(orig_committer, committer_len);

	यदि (commit_रूख_extended(सन्देश.buf, सन्देश.len,
				 r->hash_algo->empty_रूख, parents,
				 out, author, committer,
				 रिक्त, रिक्त)) {
		ret = error(_("could not write commit"));
		गच्छ out;
	}
out:
	strbuf_release(&सन्देश);
	repo_unuse_commit_buffer(r, this, buffer);
	मुक्त(author);
	मुक्त(committer);
	फर्काऊ ret;
}

स्थिर पूर्णाङ्क do_import_stash(संरचना repository *r, अचर वर्ण *rev)
{
	संरचना object_id chain;
	पूर्णाङ्क res = 0;
	अचर वर्ण *buffer = रिक्त;
	अचिन्हित दीर्घ bufsize;
	संरचना commit *this = रिक्त;
	संरचना commit_list *वस्तुहरू = रिक्त, *cur;
	वर्ण *सन्देश = रिक्त;

	यदि (repo_get_oid(r, rev, &chain))
		फर्काऊ error(_("not a valid revision: %s"), rev);

	this = lookup_commit_reference(r, &chain);
	यदि (!this)
		फर्काऊ error(_("not a commit: %s"), rev);

	/*
	 * Walk the commit history, finding each stash entry, and load data into
	 * the array.
	 */
	लागि (;;) {
		अचर वर्ण *author, *committer;
		माप_प्रकार author_len, committer_len;
		अचर वर्ण *p;
		अचर वर्ण *expected = "git stash <git@stash> 1000684800 +0000";
		अचर वर्ण *prefix = "git stash: ";
		संरचना commit *stash;
		संरचना रूख *रूख = repo_get_commit_रूख(r, this);

		यदि (!रूख ||
		    !oideq(&रूख->object.oid, r->hash_algo->empty_रूख) ||
		    (this->parents &&
		     (!this->parents->next || this->parents->next->next))) {
			res = error(_("%s is not a valid exported stash commit"),
					oid_to_hex(&this->object.oid));
			गच्छ out;
		}

		buffer = repo_get_commit_buffer(r, this, &bufsize);

		यदि (!this->parents) {
			/*
			 * We don't have any parents.  Make sure this is our
			 * root commit.
			 */
			author = find_commit_header(buffer, "author", &author_len);
			committer = find_commit_header(buffer, "committer", &committer_len);

			यदि (!author || !committer) {
				error(_("cannot parse commit %s"), oid_to_hex(&this->object.oid));
				गच्छ out;
			}

			यदि (author_len != म_माप(expected) ||
			    committer_len != म_माप(expected) ||
			    स_भेद(author, expected, author_len) ||
			    स_भेद(committer, expected, committer_len)) {
				res = error(_("found root commit %s with invalid data"), oid_to_hex(&this->object.oid));
				गच्छ out;
			}
			रोक;
		}

		p = म_माला(buffer, "\n\n");
		यदि (!p) {
			res = error(_("cannot parse commit %s"), oid_to_hex(&this->object.oid));
			गच्छ out;
		}

		p += 2;
		यदि (((माप_प्रकार)(bufsize - (p - buffer)) < म_माप(prefix)) ||
		    स_भेद(prefix, p, म_माप(prefix))) {
			res = error(_("found stash commit %s without expected prefix"), oid_to_hex(&this->object.oid));
			गच्छ out;
		}

		stash = this->parents->next->item;

		यदि (repo_parse_commit(r, this->parents->item) ||
		    repo_parse_commit(r, stash)) {
			res = error(_("cannot parse parents of commit: %s"),
					oid_to_hex(&this->object.oid));
			गच्छ out;
		}

		यदि (check_stash_topology(r, stash)) {
			res = error(_("%s does not look like a stash commit"),
					oid_to_hex(&stash->object.oid));
			गच्छ out;
		}

		repo_unuse_commit_buffer(r, this, buffer);
		buffer = रिक्त;
		वस्तुहरू = commit_list_घुसाउ(stash, &वस्तुहरू);
		this = this->parents->item;
	}

	/*
	 * Now, walk each entry, adding it to the stash as a normal stash
	 * commit.
	 */
	लागि (cur = वस्तुहरू; cur; cur = cur->next) {
		अचर वर्ण *p;
		संरचना object_id *oid;

		this = cur->item;
		oid = &this->object.oid;
		buffer = repo_get_commit_buffer(r, this, &bufsize);
		यदि (!buffer) {
			res = error(_("cannot read commit buffer for %s"), oid_to_hex(oid));
			गच्छ out;
		}

		p = म_माला(buffer, "\n\n");
		यदि (!p) {
			res = error(_("cannot parse commit %s"), oid_to_hex(oid));
			गच्छ out;
		}

		p += 2;
		सन्देश = xmemdupz(p, bufsize - (p - buffer));
		repo_unuse_commit_buffer(r, this, buffer);
		buffer = रिक्त;

		यदि (do_store_stash(oid, सन्देश, 1)) {
			res = error(_("cannot save the stash for %s"), oid_to_hex(oid));
			गच्छ out;
		}
		FREE_AND_NULL(सन्देश);
	}
out:
	यदि (this && buffer)
		repo_unuse_commit_buffer(r, this, buffer);
	commit_list_free(वस्तुहरू);
	मुक्त(सन्देश);

	फर्काऊ res;
}

स्थिर पूर्णाङ्क import_stash(पूर्णाङ्क argc, अचर वर्ण **argv, अचर वर्ण *prefix,
			संरचना repository *repo)
{
	संरचना option options[] = {
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_import_usage,
			     PARSE_OPT_KEEP_DASHDASH);

	यदि (argc != 1)
		usage_सन्देश_opt("a revision is required", git_stash_import_usage, options);

	फर्काऊ do_import_stash(repo, argv[0]);
}

संरचना stash_entry_data {
	संरचना repository *r;
	संरचना commit_list **वस्तुहरू;
	माप_प्रकार गन्ती;
};

स्थिर पूर्णाङ्क collect_stash_entries(अचर वर्ण *refname UNUSED,
				 संरचना object_id *old_oid UNUSED,
				 संरचना object_id *new_oid,
				 अचर वर्ण *committer UNUSED,
				 timestamp_t timestamp UNUSED,
				 पूर्णाङ्क tz UNUSED, अचर वर्ण *सन्देश UNUSED,
				 शून्य *cb_data)
{
	संरचना stash_entry_data *data = cb_data;
	संरचना commit *stash;

	data->गन्ती++;
	stash = lookup_commit_reference(data->r, new_oid);
	यदि (!stash || check_stash_topology(data->r, stash)) {
		फर्काऊ error(_("%s does not look like a stash commit"),
			     oid_to_hex(new_oid));
	}
	data->वस्तुहरू = commit_list_append(stash, data->वस्तुहरू);
	फर्काऊ 0;
}

स्थिर पूर्णाङ्क do_export_stash(संरचना repository *r,
			   अचर वर्ण *ref,
			   पूर्णाङ्क argc,
			   अचर वर्ण **argv)
{
	संरचना object_id base;
	संरचना commit *prev;
	संरचना commit_list *वस्तुहरू = रिक्त, **iter = &वस्तुहरू, *cur;
	पूर्णाङ्क res = 0;
	पूर्णाङ्क i;
	संरचना strbuf revision = STRBUF_INIT;
	अचर वर्ण *author, *committer;

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
	यदि (commit_रूख_extended("", 0, r->hash_algo->empty_रूख, रिक्त,
				 &base, author, committer, रिक्त, रिक्त))
		फर्काऊ error(_("unable to write base commit"));

	prev = lookup_commit_reference(r, &base);

	यदि (argc) {
		/*
		 * Find each specified stash, and load data into the array.
		 */
		लागि (i = 0; i < argc; i++) {
			संरचना object_id oid;
			संरचना commit *stash;

			यदि (parse_stash_revision(&revision, argv[i], 1) ||
			    repo_get_oid_with_flags(r, revision.buf, &oid,
						    GET_OID_QUIETLY |
						    GET_OID_GENTLY)) {
				res = error(_("unable to find stash entry %s"), argv[i]);
				गच्छ out;
			}

			stash = lookup_commit_reference(r, &oid);
			यदि (!stash || check_stash_topology(r, stash)) {
				res = error(_("%s does not look like a stash commit"),
						revision.buf);
				गच्छ out;
			}
			iter = commit_list_append(stash, iter);
		}
	} अन्यथा {
		/*
		 * Walk the reflog, finding each stash entry, and load data into the
		 * array.
		 */
		संरचना stash_entry_data cb_data = {
			.r = r, .वस्तुहरू = iter,
		};
		यदि (refs_for_each_reflog_ent_reverse(get_main_ref_store(r),
						     "refs/stash",
						     collect_stash_entries,
						     &cb_data) && cb_data.गन्ती)
			गच्छ out;
	}

	/*
	 * Now, create a set of commits identical to the regular stash commits,
	 * but where their first parents form a chain to our original empty
	 * base commit.
	 */
	वस्तुहरू = commit_list_reverse(वस्तुहरू);
	लागि (cur = वस्तुहरू; cur; cur = cur->next) {
		संरचना commit_list *parents = रिक्त;
		संरचना commit_list **next = &parents;
		संरचना object_id out;
		संरचना commit *stash = cur->item;

		next = commit_list_append(prev, next);
		next = commit_list_append(stash, next);
		res = write_commit_with_parents(r, &out, &stash->object.oid, parents);
		commit_list_free(parents);
		यदि (res)
			गच्छ out;
		prev = lookup_commit_reference(r, &out);
	}
	यदि (ref)
		refs_update_ref(get_main_ref_store(r), रिक्त, ref,
				&prev->object.oid, रिक्त, 0, UPDATE_REFS_DIE_ON_ERR);
	अन्यथा
		माला_दो(oid_to_hex(&prev->object.oid));
out:
	strbuf_release(&revision);
	commit_list_free(वस्तुहरू);

	फर्काऊ res;
}

गणना export_action {
	ACTION_NONE,
	ACTION_PRINT,
	ACTION_TO_REF,
};

स्थिर पूर्णाङ्क export_stash(पूर्णाङ्क argc,
			अचर वर्ण **argv,
			अचर वर्ण *prefix,
			संरचना repository *repo)
{
	अचर वर्ण *ref = रिक्त;
	गणना export_action action = ACTION_NONE;
	संरचना option options[] = {
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

	यदि (ref && action == ACTION_NONE)
		action = ACTION_TO_REF;

	यदि (action == ACTION_NONE || (ref && action == ACTION_PRINT))
		फर्काऊ error(_("exactly one of --print and --to-ref is required"));

	फर्काऊ do_export_stash(repo, ref, argc, argv);
}

पूर्णाङ्क cmd_stash(पूर्णाङ्क argc,
	      अचर वर्ण **argv,
	      अचर वर्ण *prefix,
	      संरचना repository *repo)
{
	pid_t pid = getpid();
	अचर वर्ण *index_file;
	संरचना strvec args = STRVEC_INIT;
	parse_opt_subcommand_fn *fn = रिक्त;
	संरचना option options[] = {
		OPT_SUBCOMMAND("apply", &fn, apply_stash),
		OPT_SUBCOMMAND("clear", &fn, clear_stash),
		OPT_SUBCOMMAND("drop", &fn, drop_stash),
		OPT_SUBCOMMAND("pop", &fn, झिक_stash),
		OPT_SUBCOMMAND("branch", &fn, branch_stash),
		OPT_SUBCOMMAND("list", &fn, list_stash),
		OPT_SUBCOMMAND("show", &fn, show_stash),
		OPT_SUBCOMMAND("store", &fn, store_stash),
		OPT_SUBCOMMAND("create", &fn, create_stash),
		OPT_SUBCOMMAND("push", &fn, थप_stash_unassumed),
		OPT_SUBCOMMAND("export", &fn, export_stash),
		OPT_SUBCOMMAND("import", &fn, import_stash),
		OPT_SUBCOMMAND_F("save", &fn, save_stash, रिक्त, PARSE_OPT_NOCOMPLETE),
		OPT_END()
	};
	अचर वर्ण **args_copy;
	पूर्णाङ्क ret;

	repo_config(the_repository, git_stash_config, रिक्त);

	argc = parse_options(argc, argv, prefix, options, git_stash_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL |
			     PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	index_file = repo_get_index_file(the_repository);
	strbuf_addf(&stash_index_path, "%s.stash.%" PRIuMAX, index_file,
		    (uintmax_t)pid);

	यदि (fn) {
		ret = fn(argc, argv, prefix, repo);

		/*
		 * The subcommand implementations return 0 on success, a
		 * negative value on failure, and STASH_APPLY_CONFLICT
		 * when applying a stash entry resulted in conflicts.
		 * Map failures to 128, the status die() uses, so that
		 * exit status 1 unambiguously indicates conflicts.
		 */
		यदि (ret < 0)
			फर्काऊ 128;
		फर्काऊ ret;
	} अन्यथा यदि (!argc) {
		फर्काऊ !!थप_stash_unassumed(0, रिक्त, prefix, repo);
	}

	/* Assume 'stash push' */
	strvec_थप(&args, "push");
	strvec_pushv(&args, argv);

	/*
	 * `push_stash()` ends up modifying the array, which causes memory
	 * leaks if we didn't copy the array here.
	 */
	DUP_ARRAY(args_copy, args.v, args.nr);

	ret = !!थप_stash(args.nr, args_copy, prefix, 1);

	strvec_clear(&args);
	मुक्त(args_copy);
	फर्काऊ ret;
}
