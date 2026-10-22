#घोषणा USE_THE_REPOSITORY_VARIABLE

#समावेश "builtin.h"
#समावेश "abspath.h"
#समावेश "config.h"
#समावेश "environment.h"
#समावेश "gettext.h"
#समावेश "hash.h"
#समावेश "hex.h"
#समावेश "object-name.h"
#समावेश "parse-options.h"
#समावेश "refs.h"
#समावेश "stash.h"
#समावेश "lockfile.h"
#समावेश "cache-tree.h"
#समावेश "unpack-trees.h"
#समावेश "merge-ort-wrappers.h"
#समावेश "strvec.h"
#समावेश "run-command.h"
#समावेश "dir.h"
#समावेश "entry.h"
#समावेश "preload-index.h"
#समावेश "read-cache.h"
#समावेश "repository.h"
#समावेश "rerere.h"
#समावेश "revision.h"
#समावेश "setup.h"
#समावेश "sparse-index.h"
#समावेश "log-tree.h"
#समावेश "diffcore.h"
#समावेश "reflog.h"
#समावेश "reflog-walk.h"
#समावेश "add-interactive.h"
#समावेश "commit.h"

#घोषणा INCLUDE_ALL_FILES 2

#घोषणा BUILTIN_STASH_LIST_USAGE \
	N_("git stash list [<log-options>]")
#घोषणा BUILTIN_STASH_SHOW_USAGE \
	N_("git stash show [-u | --include-untracked | --only-untracked] [<diff-options>] [<stash>]")
#घोषणा BUILTIN_STASH_DROP_USAGE \
	N_("git stash drop [-q | --quiet] [<stash>]")
#घोषणा BUILTIN_STASH_निकालो_USAGE \
	N_("git stash pop [--index] [-q | --quiet] [<stash>]")
#घोषणा BUILTIN_STASH_APPLY_USAGE \
	N_("git stash apply [--index] [-q | --quiet] [--label-ours=<label>] [--label-theirs=<label>] [--label-base=<label>] [<stash>]")
#घोषणा BUILTIN_STASH_BRANCH_USAGE \
	N_("git stash branch <branchname> [<stash>]")
#घोषणा BUILTIN_STASH_STORE_USAGE \
	N_("git stash store [(-m | --message) <message>] [-q | --quiet] <commit>")
#घोषणा BUILTIN_STASH_धकेलो_USAGE \
	N_("git stash [push] [-p | --patch] [-S | --staged] [-k | --[no-]keep-index] [-q | --quiet]\n" \
	   "          [-u | --include-untracked] [-a | --all] [(-m | --message) <message>]\n" \
	   "          [--pathspec-from-file=<file> [--pathspec-file-nul]]\n" \
	   "          [--] [<pathspec>...]")
#घोषणा BUILTIN_STASH_SAVE_USAGE \
	N_("git stash save [-p | --patch] [-S | --staged] [-k | --[no-]keep-index] [-q | --quiet]\n" \
	   "          [-u | --include-untracked] [-a | --all] [<message>]")
#घोषणा BUILTIN_STASH_CREATE_USAGE \
	N_("git stash create [<message>]")
#घोषणा BUILTIN_STASH_EXPORT_USAGE \
	N_("git stash export (--print | --to-ref <ref>) [<stash>...]")
#घोषणा BUILTIN_STASH_IMPORT_USAGE \
	N_("git stash import <commit>")
#घोषणा BUILTIN_STASH_CLEAR_USAGE \
	"git stash clear"

अटल स्थिर अक्षर * स्थिर git_stash_usage[] = {
	BUILTIN_STASH_LIST_USAGE,
	BUILTIN_STASH_SHOW_USAGE,
	BUILTIN_STASH_DROP_USAGE,
	BUILTIN_STASH_निकालो_USAGE,
	BUILTIN_STASH_APPLY_USAGE,
	BUILTIN_STASH_BRANCH_USAGE,
	BUILTIN_STASH_धकेलो_USAGE,
	BUILTIN_STASH_SAVE_USAGE,
	BUILTIN_STASH_CLEAR_USAGE,
	BUILTIN_STASH_CREATE_USAGE,
	BUILTIN_STASH_STORE_USAGE,
	BUILTIN_STASH_EXPORT_USAGE,
	BUILTIN_STASH_IMPORT_USAGE,
	शून्य
};

अटल स्थिर अक्षर * स्थिर git_stash_list_usage[] = {
	BUILTIN_STASH_LIST_USAGE,
	शून्य
};

अटल स्थिर अक्षर * स्थिर git_stash_show_usage[] = {
	BUILTIN_STASH_SHOW_USAGE,
	शून्य
};

अटल स्थिर अक्षर * स्थिर git_stash_drop_usage[] = {
	BUILTIN_STASH_DROP_USAGE,
	शून्य
};

अटल स्थिर अक्षर * स्थिर git_stash_निकालो_usage[] = {
	BUILTIN_STASH_निकालो_USAGE,
	शून्य
};

अटल स्थिर अक्षर * स्थिर git_stash_apply_usage[] = {
	BUILTIN_STASH_APPLY_USAGE,
	शून्य
};

अटल स्थिर अक्षर * स्थिर git_stash_branch_usage[] = {
	BUILTIN_STASH_BRANCH_USAGE,
	शून्य
};

अटल स्थिर अक्षर * स्थिर git_stash_clear_usage[] = {
	BUILTIN_STASH_CLEAR_USAGE,
	शून्य
};

अटल स्थिर अक्षर * स्थिर git_stash_store_usage[] = {
	BUILTIN_STASH_STORE_USAGE,
	शून्य
};

अटल स्थिर अक्षर * स्थिर git_stash_धकेलो_usage[] = {
	BUILTIN_STASH_धकेलो_USAGE,
	शून्य
};

अटल स्थिर अक्षर * स्थिर git_stash_save_usage[] = {
	BUILTIN_STASH_SAVE_USAGE,
	शून्य
};

अटल स्थिर अक्षर * स्थिर git_stash_export_usage[] = {
	BUILTIN_STASH_EXPORT_USAGE,
	शून्य
};

अटल स्थिर अक्षर * स्थिर git_stash_import_usage[] = {
	BUILTIN_STASH_IMPORT_USAGE,
	शून्य
};

अटल स्थिर अक्षर ref_stash[] = "refs/stash";
अटल काष्ठा strbuf stash_सूचकांक_path = STRBUF_INIT;

अटल पूर्णांक show_stat = 1;
अटल पूर्णांक show_patch;
अटल पूर्णांक show_include_untracked;
अटल पूर्णांक use_सूचकांक;

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
काष्ठा stash_info {
	काष्ठा object_id w_commit;
	काष्ठा object_id b_commit;
	काष्ठा object_id i_commit;
	काष्ठा object_id u_commit;
	काष्ठा object_id c_commit;
	काष्ठा object_id w_वृक्ष;
	काष्ठा object_id b_वृक्ष;
	काष्ठा object_id i_वृक्ष;
	काष्ठा object_id u_वृक्ष;
	काष्ठा strbuf revision;
	पूर्णांक is_stash_ref;
	पूर्णांक has_u;
};

#घोषणा STASH_INFO_INIT { \
	.revision = STRBUF_INIT, \
}

अटल व्योम free_stash_info(काष्ठा stash_info *info)
{
	strbuf_release(&info->revision);
}

अटल पूर्णांक check_stash_topology(काष्ठा repository *r, काष्ठा commit *stash)
{
	काष्ठा commit *p1, *p2, *p3 = शून्य;

	/* stash must have two or three parents */
	अगर (!stash->parents || !stash->parents->अग्र ||
			(stash->parents->अग्र->अग्र && stash->parents->अग्र->अग्र->अग्र))
		वापस -1;
	p1 = stash->parents->item;
	p2 = stash->parents->अग्र->item;
	अगर (stash->parents->अग्र->अग्र)
		p3 = stash->parents->अग्र->अग्र->item;
	अगर (repo_parse_commit(r, p1) || repo_parse_commit(r, p2) ||
			(p3 && repo_parse_commit(r, p3)))
		वापस -1;
	/* p2 must have a single parent, p3 must have no parents */
	अगर (!p2->parents || p2->parents->अग्र || (p3 && p3->parents))
		वापस -1;
	अगर (repo_parse_commit(r, p2->parents->item))
		वापस -1;
	/* p2^1 must equal p1 */
	अगर (!oideq(&p1->object.oid, &p2->parents->item->object.oid))
		वापस -1;

	वापस 0;
}

अटल व्योम assert_stash_like(काष्ठा stash_info *info, स्थिर अक्षर *revision)
{
	अगर (get_oidf(&info->b_commit, "%s^1", revision) ||
	    get_oidf(&info->w_वृक्ष, "%s:", revision) ||
	    get_oidf(&info->b_वृक्ष, "%s^1:", revision) ||
	    get_oidf(&info->i_वृक्ष, "%s^2:", revision))
		die(_("'%s' is not a stash-like commit"), revision);
}

अटल पूर्णांक parse_stash_revision(काष्ठा strbuf *revision, स्थिर अक्षर *commit, पूर्णांक quiet)
{
	strbuf_reset(revision);
	अगर (!commit) {
		अगर (!refs_ref_exists(get_main_ref_store(the_repository), ref_stash)) {
			अगर (!quiet)
				fprintf_ln(मानक_त्रुटि, _("No stash entries found."));
			वापस -1;
		}

		strbuf_addf(revision, "%s@{0}", ref_stash);
	} अन्यथा अगर (म_अखोज(commit, "0123456789") == म_माप(commit)) {
		strbuf_addf(revision, "%s@{%s}", ref_stash, commit);
	} अन्यथा {
		strbuf_addstr(revision, commit);
	}
	वापस 0;
}

अटल पूर्णांक get_stash_info(काष्ठा stash_info *info, पूर्णांक argc, स्थिर अक्षर **argv)
{
	पूर्णांक ret;
	अक्षर *end_of_rev;
	अक्षर *expanded_ref;
	स्थिर अक्षर *revision;
	स्थिर अक्षर *commit = शून्य;
	काष्ठा object_id dummy;
	काष्ठा strbuf symbolic = STRBUF_INIT;

	अगर (argc > 1) {
		पूर्णांक i;
		काष्ठा strbuf refs_संदेश = STRBUF_INIT;

		क्रम (i = 0; i < argc; i++)
			strbuf_addf(&refs_संदेश, " '%s'", argv[i]);

		fprintf_ln(मानक_त्रुटि, _("Too many revisions specified:%s"),
			   refs_संदेश.buf);
		strbuf_release(&refs_संदेश);

		वापस -1;
	}

	अगर (argc == 1)
		commit = argv[0];

	strbuf_init(&info->revision, 0);
	अगर (parse_stash_revision(&info->revision, commit, 0)) {
		वापस -1;
	}

	revision = info->revision.buf;

	अगर (repo_get_oid(the_repository, revision, &info->w_commit))
		वापस error(_("%s is not a valid reference"), revision);

	assert_stash_like(info, revision);

	info->has_u = !get_oidf(&info->u_वृक्ष, "%s^3:", revision);

	end_of_rev = strchrnul(revision, '@');
	strbuf_add(&symbolic, revision, end_of_rev - revision);

	ret = repo_dwim_ref(the_repository, symbolic.buf, symbolic.len,
			    &dummy, &expanded_ref, 0);
	strbuf_release(&symbolic);
	चयन (ret) {
	हाल 0: /* Not found, but valid ref */
		info->is_stash_ref = 0;
		अवरोध;
	हाल 1:
		info->is_stash_ref = !म_भेद(expanded_ref, ref_stash);
		अवरोध;
	शेष: /* Invalid or ambiguous */
		अवरोध;
	}

	मुक्त(expanded_ref);
	वापस !(ret == 0 || ret == 1);
}

अटल पूर्णांक do_clear_stash(व्योम)
{
	काष्ठा object_id obj;
	अगर (repo_get_oid(the_repository, ref_stash, &obj))
		वापस 0;

	वापस refs_delete_ref(get_main_ref_store(the_repository), शून्य,
			       ref_stash, &obj, 0);
}

अटल पूर्णांक clear_stash(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
		       काष्ठा repository *repo UNUSED)
{
	काष्ठा option options[] = {
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_clear_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	अगर (argc)
		वापस error(_("git stash clear with arguments is "
			       "unimplemented"));

	वापस do_clear_stash();
}

अटल पूर्णांक reset_वृक्ष(काष्ठा object_id *i_वृक्ष, पूर्णांक update, पूर्णांक reset)
{
	पूर्णांक nr_trees = 1;
	काष्ठा unpack_trees_options opts;
	काष्ठा वृक्ष_desc t[MAX_UNPACK_TREES];
	काष्ठा वृक्ष *वृक्ष;
	काष्ठा lock_file lock_file = LOCK_INIT;

	repo_read_सूचकांक_preload(the_repository, शून्य, 0);
	अगर (refresh_सूचकांक(the_repository->सूचकांक, REFRESH_QUIET, शून्य, शून्य, शून्य))
		वापस -1;

	repo_hold_locked_सूचकांक(the_repository, &lock_file, LOCK_DIE_ON_ERROR);

	स_रखो(&opts, 0, माप(opts));

	वृक्ष = repo_parse_वृक्ष_indirect(the_repository, i_वृक्ष);
	अगर (repo_parse_वृक्ष(the_repository, वृक्ष))
		वापस -1;

	init_वृक्ष_desc(t, &वृक्ष->object.oid, वृक्ष->बफर, वृक्ष->आकार);

	opts.शीर्ष_idx = 1;
	opts.src_सूचकांक = the_repository->सूचकांक;
	opts.dst_सूचकांक = the_repository->सूचकांक;
	opts.merge = 1;
	opts.reset = reset ? UNPACK_RESET_PROTECT_UNTRACKED : 0;
	opts.update = update;
	अगर (update)
		opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
	opts.fn = oneway_merge;

	अगर (unpack_trees(nr_trees, t, &opts))
		वापस -1;

	अगर (write_locked_सूचकांक(the_repository->सूचकांक, &lock_file, COMMIT_LOCK))
		वापस error(_("unable to write new index file"));

	वापस 0;
}

अटल पूर्णांक create_सूचकांक_from_वृक्ष(स्थिर काष्ठा object_id *वृक्ष_id,
				  स्थिर अक्षर *सूचकांक_path)
{
	पूर्णांक nr_trees = 1;
	पूर्णांक ret = 0;
	काष्ठा unpack_trees_options opts;
	काष्ठा वृक्ष_desc t[MAX_UNPACK_TREES];
	काष्ठा वृक्ष *वृक्ष;
	काष्ठा सूचकांक_state dst_istate = सूचकांक_STATE_INIT(the_repository);
	काष्ठा lock_file lock_file = LOCK_INIT;

	repo_read_सूचकांक_preload(the_repository, शून्य, 0);
	refresh_सूचकांक(the_repository->सूचकांक, REFRESH_QUIET, शून्य, शून्य, शून्य);

	hold_lock_file_for_update(&lock_file, सूचकांक_path, LOCK_DIE_ON_ERROR);

	स_रखो(&opts, 0, माप(opts));

	वृक्ष = repo_parse_वृक्ष_indirect(the_repository, वृक्ष_id);
	अगर (!वृक्ष || repo_parse_वृक्ष(the_repository, वृक्ष)) {
		ret = -1;
		जाओ done;
	}

	init_वृक्ष_desc(t, &वृक्ष->object.oid, वृक्ष->बफर, वृक्ष->आकार);

	opts.शीर्ष_idx = 1;
	opts.src_सूचकांक = the_repository->सूचकांक;
	opts.dst_सूचकांक = &dst_istate;
	opts.merge = 1;
	opts.reset = UNPACK_RESET_PROTECT_UNTRACKED;
	opts.fn = oneway_merge;

	अगर (unpack_trees(nr_trees, t, &opts)) {
		ret = -1;
		जाओ done;
	}

	अगर (write_locked_सूचकांक(&dst_istate, &lock_file, COMMIT_LOCK)) {
		ret = error(_("unable to write new index file"));
		जाओ done;
	}

done:
	release_सूचकांक(&dst_istate);
	अगर (ret)
		rollback_lock_file(&lock_file);
	वापस ret;
}

अटल पूर्णांक is_path_a_directory(स्थिर अक्षर *path)
{
	/*
	 * This function differs from abspath.c:is_directory() in that
	 * here we use lstat() instead of stat(); we do not want to
	 * follow symbolic links here.
	 */
	काष्ठा stat st;
	वापस (!lstat(path, &st) && S_ISDIR(st.st_mode));
}

अटल व्योम add_diff_to_buf(काष्ठा diff_queue_struct *q,
			    काष्ठा diff_options *options UNUSED,
			    व्योम *तथ्य)
{
	पूर्णांक i;

	क्रम (i = 0; i < q->nr; i++) {
		अगर (is_path_a_directory(q->queue[i]->one->path))
			जारी;

		strbuf_addstr(तथ्य, q->queue[i]->one->path);

		/* NUL-terminate: will be fed to update-index -z */
		strbuf_addch(तथ्य, '\0');
	}
}

अटल पूर्णांक restore_untracked(काष्ठा object_id *u_वृक्ष)
{
	पूर्णांक res;
	काष्ठा child_process cp = CHILD_PROCESS_INIT;

	/*
	 * We need to run restore files from a given index, but without
	 * affecting the current index, so we use GIT_INDEX_FILE with
	 * run_command to fork processes that will not interfere.
	 */
	cp.git_cmd = 1;
	strvec_धकेलो(&cp.args, "read-tree");
	strvec_धकेलो(&cp.args, oid_to_hex(u_वृक्ष));
	strvec_pushf(&cp.env, "GIT_INDEX_FILE=%s",
		     stash_सूचकांक_path.buf);
	अगर (run_command(&cp)) {
		remove_path(stash_सूचकांक_path.buf);
		वापस -1;
	}

	child_process_init(&cp);
	cp.git_cmd = 1;
	strvec_pushl(&cp.args, "checkout-index", "--all", शून्य);
	strvec_pushf(&cp.env, "GIT_INDEX_FILE=%s",
		     stash_सूचकांक_path.buf);

	res = run_command(&cp);
	remove_path(stash_सूचकांक_path.buf);
	वापस res;
}

अटल व्योम unstage_changes_unless_new(काष्ठा object_id *orig_वृक्ष)
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

	काष्ठा checkout state = CHECKOUT_INIT;
	काष्ठा diff_options diff_opts;
	काष्ठा lock_file lock = LOCK_INIT;
	पूर्णांक i;

	/* If any entries have skip_worktree set, we'll have to check 'em out */
	state.force = 1;
	state.quiet = 1;
	state.refresh_cache = 1;
	state.istate = the_repository->सूचकांक;

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

	do_diff_cache(orig_वृक्ष, &diff_opts);
	diffcore_std(&diff_opts);

	/* Iterate over the paths that changed due to the merge... */
	क्रम (i = 0; i < diff_queued_diff.nr; i++) {
		काष्ठा diff_filepair *p;
		काष्ठा cache_entry *ce;
		पूर्णांक pos;

		/* Look up the path's position in the current index. */
		p = diff_queued_diff.queue[i];
		pos = सूचकांक_name_pos(the_repository->सूचकांक, p->two->path,
				     म_माप(p->two->path));

		/*
		 * Step 2: Place changes in the working tree
		 *
		 * Stash is about restoring changes *to the working tree*.
		 * So if the merge successfully got a new version of some
		 * path, but left it out of the working tree, then clear the
		 * SKIP_WORKTREE bit and write it to the working tree.
		 */
		अगर (pos >= 0 && ce_skip_worktree(the_repository->सूचकांक->cache[pos])) {
			काष्ठा stat st;

			ce = the_repository->सूचकांक->cache[pos];
			अगर (!lstat(ce->name, &st)) {
				/* Conflicting path present; relocate it */
				काष्ठा strbuf new_path = STRBUF_INIT;
				पूर्णांक fd;

				strbuf_addf(&new_path,
					    "%s.stash.XXXXXX", ce->name);
				fd = xmkstemp(new_path.buf);
				बंद(fd);
				म_लिखो(_("WARNING: Untracked file in way of "
					 "tracked file!  Renaming\n "
					 "           %s -> %s\n"
					 "         to make room.\n"),
				       ce->name, new_path.buf);
				अगर (नाम(ce->name, new_path.buf))
					die("Failed to move %s to %s",
					    ce->name, new_path.buf);
				strbuf_release(&new_path);
			}
			checkout_entry(ce, &state, शून्य, शून्य);
			ce->ce_flags &= ~CE_SKIP_WORKTREE;
		}

		/*
		 * Step 3: "unstage" changes, as long as they are still tracked
		 */
		अगर (p->one->oid_valid) {
			/*
			 * Path existed in orig_tree; restore index entry
			 * from that tree in order to "unstage" the changes.
			 */
			पूर्णांक option = ADD_CACHE_OK_TO_REPLACE;
			अगर (pos < 0)
				option = ADD_CACHE_OK_TO_ADD;

			ce = make_cache_entry(the_repository->सूचकांक,
					      p->one->mode,
					      &p->one->oid,
					      p->one->path,
					      0, 0);
			add_सूचकांक_entry(the_repository->सूचकांक, ce, option);
		}
	}
	diff_flush(&diff_opts);

	/*
	 * Step 4: write the new index to disk
	 */
	repo_hold_locked_सूचकांक(the_repository, &lock, LOCK_DIE_ON_ERROR);
	अगर (write_locked_सूचकांक(the_repository->सूचकांक, &lock,
			       COMMIT_LOCK | SKIP_IF_UNCHANGED))
		die(_("could not write index"));
}

अटल क्रमागत stash_apply_परिणाम do_apply_stash(स्थिर अक्षर *prefix,
					      काष्ठा stash_info *info,
					      पूर्णांक सूचकांक, पूर्णांक quiet,
					      स्थिर अक्षर *label_ours,
					      स्थिर अक्षर *label_theirs,
					      स्थिर अक्षर *label_base)
{
	पूर्णांक clean, ret;
	पूर्णांक has_सूचकांक = सूचकांक;
	काष्ठा merge_options o;
	काष्ठा object_id c_वृक्ष;
	काष्ठा object_id सूचकांक_वृक्ष;
	काष्ठा वृक्ष *शीर्ष, *merge, *merge_base;
	काष्ठा lock_file lock = LOCK_INIT;

	repo_read_सूचकांक_preload(the_repository, शून्य, 0);
	अगर (repo_refresh_and_write_सूचकांक(the_repository, REFRESH_QUIET, 0, 0,
					 शून्य, शून्य, शून्य))
		वापस error(_("could not write index"));

	अगर (write_सूचकांक_as_वृक्ष(&c_वृक्ष, the_repository->सूचकांक,
				repo_get_सूचकांक_file(the_repository), 0, शून्य))
		वापस error(_("cannot apply a stash in the middle of a merge"));

	init_ui_merge_options(&o, the_repository);

	अगर (quiet)
		o.verbosity = 0;

	अगर (सूचकांक) {
		अगर (oideq(&info->b_वृक्ष, &info->i_वृक्ष) ||
		    oideq(&c_वृक्ष, &info->i_वृक्ष)) {
			has_सूचकांक = 0;
		} अन्यथा {
			काष्ठा merge_परिणाम परिणाम = { 0 };

			o.branch1 = "Current index";
			o.branch2 = "Stashed index changes";
			o.ancestor = "Stash base";

			शीर्ष = lookup_वृक्ष(o.repo, &c_वृक्ष);
			merge = lookup_वृक्ष(o.repo, &info->i_वृक्ष);
			merge_base = lookup_वृक्ष(o.repo, &info->b_वृक्ष);

			merge_incore_nonrecursive(&o, merge_base, शीर्ष, merge,
						  &परिणाम);

			अगर (परिणाम.clean < 0) {
				merge_finalize(&o, &परिणाम);
				वापस error(_("index merge failed"));
			} अन्यथा अगर (!परिणाम.clean) {
				merge_finalize(&o, &परिणाम);
				वापस error(_("conflicts in index. "
					       "Try without --index."));
			} अन्यथा {
				oidcpy(&सूचकांक_वृक्ष, &परिणाम.वृक्ष->object.oid);
				merge_finalize(&o, &परिणाम);
			}
		}
	}

	o.branch1 = label_ours ? label_ours : "Updated upstream";
	o.branch2 = label_theirs ? label_theirs : "Stashed changes";
	o.ancestor = label_base ? label_base : "Stash base";

	अगर (oideq(&info->b_वृक्ष, &c_वृक्ष))
		o.branch1 = "Version stash was based on";

	अगर (o.verbosity >= 3)
		printf_ln(_("Merging %s with %s"), o.branch1, o.branch2);

	शीर्ष = lookup_वृक्ष(o.repo, &c_वृक्ष);
	merge = lookup_वृक्ष(o.repo, &info->w_वृक्ष);
	merge_base = lookup_वृक्ष(o.repo, &info->b_वृक्ष);

	repo_hold_locked_सूचकांक(o.repo, &lock, LOCK_DIE_ON_ERROR);
	clean = merge_ort_nonrecursive(&o, शीर्ष, merge, merge_base);

	/*
	 * If 'clean' >= 0, reverse the value for 'ret' so 'ret' is 0 when the
	 * merge was clean, and 1 if the merge was unclean or a negative value
	 * if it encountered an error.
	 */
	ret = clean >= 0 ? !clean : clean;

	अगर (ret < 0)
		rollback_lock_file(&lock);
	अन्यथा अगर (write_locked_सूचकांक(o.repo->सूचकांक, &lock,
				      COMMIT_LOCK | SKIP_IF_UNCHANGED))
		ret = error(_("could not write index"));

	अगर (ret) {
		repo_rerere(the_repository, 0);

		अगर (सूचकांक)
			fprintf_ln(मानक_त्रुटि, _("Index was not unstashed."));

		जाओ restore_untracked;
	}

	अगर (has_सूचकांक) {
		अगर (reset_वृक्ष(&सूचकांक_वृक्ष, 0, 0))
			ret = -1;
	} अन्यथा {
		unstage_changes_unless_new(&c_वृक्ष);
	}

restore_untracked:
	अगर (info->has_u && restore_untracked(&info->u_वृक्ष))
		ret = error(_("could not restore untracked files from stash"));

	अगर (!quiet) {
		काष्ठा child_process cp = CHILD_PROCESS_INIT;

		/*
		 * Status is quite simple and could be replaced with calls to
		 * wt_status in the future, but it adds complexities which may
		 * require more tests.
		 */
		cp.git_cmd = 1;
		cp.dir = prefix;
		strvec_pushf(&cp.env, GIT_WORK_वृक्ष_ENVIRONMENT"=%s",
			     absolute_path(repo_get_work_वृक्ष(the_repository)));
		strvec_pushf(&cp.env, GIT_DIR_ENVIRONMENT"=%s",
			     absolute_path(repo_get_git_dir(the_repository)));
		strvec_धकेलो(&cp.args, "status");
		run_command(&cp);
	}

	वापस ret;
}

अटल पूर्णांक apply_stash(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
		       काष्ठा repository *repo UNUSED)
{
	पूर्णांक ret = -1;
	पूर्णांक quiet = 0;
	पूर्णांक सूचकांक = use_सूचकांक;
	स्थिर अक्षर *label_ours = शून्य, *label_theirs = शून्य, *label_base = शून्य;
	काष्ठा stash_info info = STASH_INFO_INIT;
	काष्ठा option options[] = {
		OPT__QUIET(&quiet, N_("be quiet, only report errors")),
		OPT_BOOL(0, "index", &सूचकांक,
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

	अगर (get_stash_info(&info, argc, argv))
		जाओ cleanup;

	ret = do_apply_stash(prefix, &info, सूचकांक, quiet,
			     label_ours, label_theirs, label_base);
cleanup:
	free_stash_info(&info);
	वापस ret;
}

अटल पूर्णांक reject_reflog_ent(स्थिर अक्षर *refname UNUSED,
			     काष्ठा object_id *ooid UNUSED,
			     काष्ठा object_id *noid UNUSED,
			     स्थिर अक्षर *email UNUSED,
			     timestamp_t timestamp UNUSED,
			     पूर्णांक tz UNUSED, स्थिर अक्षर *message UNUSED,
			     व्योम *cb_तथ्य UNUSED)
{
	वापस 1;
}

अटल पूर्णांक reflog_is_empty(स्थिर अक्षर *refname)
{
	वापस !refs_for_each_reflog_ent(get_main_ref_store(the_repository),
					 refname, reject_reflog_ent, शून्य);
}

अटल पूर्णांक do_drop_stash(काष्ठा stash_info *info, पूर्णांक quiet)
{
	अगर (!reflog_delete(info->revision.buf,
			   EXPIRE_REFLOGS_REWRITE | EXPIRE_REFLOGS_UPDATE_REF,
			   0)) {
		अगर (!quiet)
			printf_ln(_("Dropped %s (%s)"), info->revision.buf,
				  oid_to_hex(&info->w_commit));
	} अन्यथा {
		वापस error(_("%s: Could not drop stash entry"),
			     info->revision.buf);
	}

	अगर (reflog_is_empty(ref_stash))
		do_clear_stash();

	वापस 0;
}

अटल पूर्णांक get_stash_info_assert(काष्ठा stash_info *info, पूर्णांक argc,
				 स्थिर अक्षर **argv)
{
	पूर्णांक ret = get_stash_info(info, argc, argv);

	अगर (ret < 0)
		वापस ret;

	अगर (!info->is_stash_ref)
		वापस error(_("'%s' is not a stash reference"), info->revision.buf);

	वापस 0;
}

अटल पूर्णांक drop_stash(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
		      काष्ठा repository *repo UNUSED)
{
	पूर्णांक ret = -1;
	पूर्णांक quiet = 0;
	काष्ठा stash_info info = STASH_INFO_INIT;
	काष्ठा option options[] = {
		OPT__QUIET(&quiet, N_("be quiet, only report errors")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_drop_usage, 0);

	अगर (get_stash_info_assert(&info, argc, argv))
		जाओ cleanup;

	ret = do_drop_stash(&info, quiet);
cleanup:
	free_stash_info(&info);
	वापस ret;
}

अटल पूर्णांक निकालो_stash(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
		     काष्ठा repository *repo UNUSED)
{
	पूर्णांक ret = -1;
	पूर्णांक सूचकांक = use_सूचकांक;
	पूर्णांक quiet = 0;
	काष्ठा stash_info info = STASH_INFO_INIT;
	काष्ठा option options[] = {
		OPT__QUIET(&quiet, N_("be quiet, only report errors")),
		OPT_BOOL(0, "index", &सूचकांक,
			 N_("attempt to recreate the index")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_निकालो_usage, 0);

	अगर (get_stash_info_assert(&info, argc, argv))
		जाओ cleanup;

	अगर ((ret = do_apply_stash(prefix, &info, सूचकांक, quiet,
				  शून्य, शून्य, शून्य)))
		printf_ln(_("The stash entry is kept in case "
			    "you need it again."));
	अन्यथा
		ret = do_drop_stash(&info, quiet);

cleanup:
	free_stash_info(&info);
	वापस ret;
}

अटल पूर्णांक branch_stash(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
			काष्ठा repository *repo UNUSED)
{
	पूर्णांक ret = -1;
	स्थिर अक्षर *branch = शून्य;
	काष्ठा stash_info info = STASH_INFO_INIT;
	काष्ठा child_process cp = CHILD_PROCESS_INIT;
	काष्ठा option options[] = {
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_branch_usage, 0);

	अगर (!argc) {
		fprintf_ln(मानक_त्रुटि, _("No branch name specified"));
		वापस -1;
	}

	branch = argv[0];

	अगर (get_stash_info(&info, argc - 1, argv + 1))
		जाओ cleanup;

	cp.git_cmd = 1;
	strvec_pushl(&cp.args, "checkout", "-b", शून्य);
	strvec_धकेलो(&cp.args, branch);
	strvec_धकेलो(&cp.args, oid_to_hex(&info.b_commit));
	ret = run_command(&cp);
	अगर (!ret)
		ret = do_apply_stash(prefix, &info, 1, 0,
				     शून्य, शून्य, शून्य);
	अगर (!ret && info.is_stash_ref)
		ret = do_drop_stash(&info, 0);

cleanup:
	free_stash_info(&info);
	वापस ret;
}

अटल पूर्णांक list_stash(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
		      काष्ठा repository *repo UNUSED)
{
	काष्ठा child_process cp = CHILD_PROCESS_INIT;
	काष्ठा option options[] = {
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_list_usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT);

	अगर (!refs_ref_exists(get_main_ref_store(the_repository), ref_stash))
		वापस 0;

	cp.git_cmd = 1;
	strvec_pushl(&cp.args, "log", "--format=%gd: %gs", "-g",
		     "--first-parent", शून्य);
	strvec_pushv(&cp.args, argv);
	strvec_धकेलो(&cp.args, ref_stash);
	strvec_धकेलो(&cp.args, "--");
	वापस run_command(&cp);
}

अटल पूर्णांक git_stash_config(स्थिर अक्षर *var, स्थिर अक्षर *मूल्य,
			    स्थिर काष्ठा config_context *ctx, व्योम *cb)
{
	अगर (!म_भेद(var, "stash.showstat")) {
		show_stat = git_config_bool(var, मूल्य);
		वापस 0;
	}
	अगर (!म_भेद(var, "stash.showpatch")) {
		show_patch = git_config_bool(var, मूल्य);
		वापस 0;
	}
	अगर (!म_भेद(var, "stash.showincludeuntracked")) {
		show_include_untracked = git_config_bool(var, मूल्य);
		वापस 0;
	}
	अगर (!म_भेद(var, "stash.index")) {
		use_सूचकांक = git_config_bool(var, मूल्य);
		वापस 0;
	}
	वापस git_diff_basic_config(var, मूल्य, ctx, cb);
}

अटल व्योम diff_include_untracked(स्थिर काष्ठा stash_info *info, काष्ठा diff_options *diff_opt)
{
	स्थिर काष्ठा object_id *oid[] = { &info->w_commit, &info->u_वृक्ष };
	काष्ठा वृक्ष *वृक्ष[ARRAY_आकार(oid)];
	काष्ठा वृक्ष_desc वृक्ष_desc[ARRAY_आकार(oid)];
	काष्ठा unpack_trees_options unpack_वृक्ष_opt = { 0 };

	क्रम (माप_प्रकार i = 0; i < ARRAY_आकार(oid); i++) {
		वृक्ष[i] = repo_parse_वृक्ष_indirect(the_repository, oid[i]);
		अगर (repo_parse_वृक्ष(the_repository, वृक्ष[i]) < 0)
			die(_("failed to parse tree"));
		init_वृक्ष_desc(&वृक्ष_desc[i], &वृक्ष[i]->object.oid,
			       वृक्ष[i]->बफर, वृक्ष[i]->आकार);
	}

	unpack_वृक्ष_opt.शीर्ष_idx = -1;
	unpack_वृक्ष_opt.src_सूचकांक = the_repository->सूचकांक;
	unpack_वृक्ष_opt.dst_सूचकांक = the_repository->सूचकांक;
	unpack_वृक्ष_opt.merge = 1;
	unpack_वृक्ष_opt.fn = stash_worktree_untracked_merge;

	अगर (unpack_trees(ARRAY_आकार(वृक्ष_desc), वृक्ष_desc, &unpack_वृक्ष_opt))
		die(_("failed to unpack trees"));

	do_diff_cache(&info->b_commit, diff_opt);
}

अटल पूर्णांक show_stash(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
		      काष्ठा repository *repo UNUSED)
{
	पूर्णांक i;
	पूर्णांक ret = -1;
	काष्ठा stash_info info = STASH_INFO_INIT;
	काष्ठा rev_info rev;
	काष्ठा strvec stash_args = STRVEC_INIT;
	काष्ठा strvec revision_args = STRVEC_INIT;
	क्रमागत {
		UNTRACKED_NONE,
		UNTRACKED_INCLUDE,
		UNTRACKED_ONLY
	} show_untracked = show_include_untracked ? UNTRACKED_INCLUDE : UNTRACKED_NONE;
	काष्ठा option options[] = {
		OPT_SET_INT('u', "include-untracked", &show_untracked,
			    N_("include untracked files in the stash"),
			    UNTRACKED_INCLUDE),
		OPT_SET_INT_F(0, "only-untracked", &show_untracked,
			      N_("only show untracked files in the stash"),
			      UNTRACKED_ONLY, PARSE_OPT_NONEG),
		OPT_END()
	};
	पूर्णांक do_usage = 0;

	init_diff_ui_defaults();
	repo_config(the_repository, git_diff_ui_config, शून्य);
	repo_init_revisions(the_repository, &rev, prefix);

	argc = parse_options(argc, argv, prefix, options, git_stash_show_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	strvec_धकेलो(&revision_args, argv[0]);
	क्रम (i = 1; i < argc; i++) {
		अगर (argv[i][0] != '-')
			strvec_धकेलो(&stash_args, argv[i]);
		अन्यथा
			strvec_धकेलो(&revision_args, argv[i]);
	}

	अगर (get_stash_info(&info, stash_args.nr, stash_args.v))
		जाओ cleanup;

	/*
	 * The config settings are applied only if there are not passed
	 * any options.
	 */
	अगर (revision_args.nr == 1) {
		अगर (show_stat)
			rev.diffopt.output_format = DIFF_FORMAT_DIFFSTAT;

		अगर (show_patch)
			rev.diffopt.output_format |= DIFF_FORMAT_PATCH;

		अगर (!show_stat && !show_patch) {
			ret = 0;
			जाओ cleanup;
		}
	}

	setup_revisions_from_strvec(&revision_args, &rev, शून्य);
	अगर (revision_args.nr > 1)
		जाओ usage;
	अगर (!rev.diffopt.output_format) {
		rev.diffopt.output_format = DIFF_FORMAT_PATCH;
		diff_setup_done(&rev.diffopt);
	}

	rev.diffopt.flags.recursive = 1;
	setup_diff_pager(&rev.diffopt);
	चयन (show_untracked) {
	हाल UNTRACKED_NONE:
		diff_वृक्ष_oid(&info.b_commit, &info.w_commit, "", &rev.diffopt);
		अवरोध;
	हाल UNTRACKED_ONLY:
		अगर (info.has_u)
			diff_मूल_वृक्ष_oid(&info.u_वृक्ष, "", &rev.diffopt);
		अवरोध;
	हाल UNTRACKED_INCLUDE:
		अगर (info.has_u)
			diff_include_untracked(&info, &rev.diffopt);
		अन्यथा
			diff_वृक्ष_oid(&info.b_commit, &info.w_commit, "", &rev.diffopt);
		अवरोध;
	}
	log_वृक्ष_diff_flush(&rev);

	ret = diff_परिणाम_code(&rev);

cleanup:
	strvec_clear(&revision_args);
	strvec_clear(&stash_args);
	free_stash_info(&info);
	release_revisions(&rev);
	अगर (do_usage)
		usage_with_options(git_stash_show_usage, options);
	वापस ret;
usage:
	do_usage = 1;
	जाओ cleanup;
}

अटल पूर्णांक do_store_stash(स्थिर काष्ठा object_id *w_commit, स्थिर अक्षर *stash_संदेश,
			  पूर्णांक quiet)
{
	काष्ठा stash_info info;
	अक्षर revision[GIT_MAX_HEXSZ];

	oid_to_hex_r(revision, w_commit);
	assert_stash_like(&info, revision);

	अगर (!stash_संदेश)
		stash_संदेश = "Created via \"git stash store\".";

	अगर (refs_update_ref(get_main_ref_store(the_repository), stash_संदेश, ref_stash, w_commit, शून्य,
			    REF_FORCE_CREATE_REFLOG,
			    quiet ? UPDATE_REFS_QUIET_ON_ERR :
			    UPDATE_REFS_संदेश_ON_ERR)) {
		अगर (!quiet) {
			fprintf_ln(मानक_त्रुटि, _("Cannot update %s with %s"),
				   ref_stash, oid_to_hex(w_commit));
		}
		वापस -1;
	}

	वापस 0;
}

अटल पूर्णांक store_stash(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
		       काष्ठा repository *repo UNUSED)
{
	पूर्णांक quiet = 0;
	स्थिर अक्षर *stash_संदेश = शून्य;
	काष्ठा object_id obj;
	काष्ठा option options[] = {
		OPT__QUIET(&quiet, N_("be quiet")),
		OPT_STRING('m', "message", &stash_संदेश, "message",
			   N_("stash message")),
		OPT_END()
	};
	पूर्णांक ret;

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_store_usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT);

	अगर (argc != 1) {
		अगर (!quiet)
			fprintf_ln(मानक_त्रुटि, _("\"git stash store\" requires one "
					     "<commit> argument"));
		वापस -1;
	}

	अगर (repo_get_oid_with_flags(the_repository, argv[0], &obj,
				    quiet ? GET_OID_QUIETLY : 0)) {
		अगर (!quiet)
			fprintf_ln(मानक_त्रुटि, _("Cannot update %s with %s"),
					     ref_stash, argv[0]);
		ret = -1;
		जाओ out;
	}

	ret = do_store_stash(&obj, stash_संदेश, quiet);

out:
	वापस ret;
}

अटल व्योम add_pathspecs(काष्ठा strvec *args,
			  स्थिर काष्ठा pathspec *ps) {
	पूर्णांक i;

	क्रम (i = 0; i < ps->nr; i++)
		strvec_धकेलो(args, ps->वस्तुएँ[i].original);
}

/*
 * `untracked_files` will be filled with the names of untracked files.
 * The return value is:
 *
 * = 0 if there are not any untracked files
 * > 0 if there are untracked files
 */
अटल पूर्णांक get_untracked_files(स्थिर काष्ठा pathspec *ps, पूर्णांक include_untracked,
			       काष्ठा strbuf *untracked_files)
{
	पूर्णांक i;
	पूर्णांक found = 0;
	काष्ठा dir_struct dir = DIR_INIT;

	अगर (include_untracked != INCLUDE_ALL_FILES)
		setup_standard_excludes(&dir);

	fill_directory(&dir, the_repository->सूचकांक, ps);
	क्रम (i = 0; i < dir.nr; i++) {
		काष्ठा dir_entry *ent = dir.entries[i];
		found++;
		strbuf_addstr(untracked_files, ent->name);
		/* NUL-terminate: will be fed to update-index -z */
		strbuf_addch(untracked_files, '\0');
	}

	dir_clear(&dir);
	वापस found;
}

/*
 * The return value of `check_changes_tracked_files()` can be:
 *
 * < 0 if there was an error
 * = 0 if there are no changes.
 * > 0 if there are changes.
 */
अटल पूर्णांक check_changes_tracked_files(स्थिर काष्ठा pathspec *ps)
{
	काष्ठा rev_info rev;
	काष्ठा object_id dummy;
	पूर्णांक ret = 0;

	/* No initial commit. */
	अगर (repo_get_oid(the_repository, "HEAD", &dummy))
		वापस -1;

	अगर (repo_read_सूचकांक(the_repository) < 0)
		वापस -1;

	repo_init_revisions(the_repository, &rev, शून्य);
	copy_pathspec(&rev.prune_तथ्य, ps);

	rev.diffopt.flags.quick = 1;
	rev.diffopt.flags.ignore_submodules = 1;
	rev.abbrev = 0;

	add_शीर्ष_to_pending(&rev);
	diff_setup_done(&rev.diffopt);

	run_diff_सूचकांक(&rev, DIFF_सूचकांक_CACHED);
	अगर (diff_परिणाम_code(&rev)) {
		ret = 1;
		जाओ done;
	}

	run_diff_files(&rev, 0);
	अगर (diff_परिणाम_code(&rev)) {
		ret = 1;
		जाओ done;
	}

done:
	release_revisions(&rev);
	वापस ret;
}

/*
 * The function will fill `untracked_files` with the names of untracked files
 * It will return 1 if there were any changes and 0 if there were not.
 */
अटल पूर्णांक check_changes(स्थिर काष्ठा pathspec *ps, पूर्णांक include_untracked,
			 काष्ठा strbuf *untracked_files)
{
	पूर्णांक ret = 0;
	अगर (check_changes_tracked_files(ps))
		ret = 1;

	अगर (include_untracked && get_untracked_files(ps, include_untracked,
						     untracked_files))
		ret = 1;

	वापस ret;
}

अटल पूर्णांक save_untracked_files(काष्ठा stash_info *info, काष्ठा strbuf *संदेश,
				काष्ठा strbuf *files)
{
	पूर्णांक ret = 0;
	काष्ठा strbuf untracked_संदेश = STRBUF_INIT;
	काष्ठा child_process cp_upd_सूचकांक = CHILD_PROCESS_INIT;
	काष्ठा सूचकांक_state istate = सूचकांक_STATE_INIT(the_repository);

	cp_upd_सूचकांक.git_cmd = 1;
	strvec_pushl(&cp_upd_सूचकांक.args, "update-index", "-z", "--add",
		     "--remove", "--stdin", शून्य);
	strvec_pushf(&cp_upd_सूचकांक.env, "GIT_INDEX_FILE=%s",
			 stash_सूचकांक_path.buf);

	strbuf_addf(&untracked_संदेश, "untracked files on %s\n", संदेश->buf);
	अगर (pipe_command(&cp_upd_सूचकांक, files->buf, files->len, शून्य, 0,
			 शून्य, 0)) {
		ret = -1;
		जाओ done;
	}

	अगर (write_सूचकांक_as_वृक्ष(&info->u_वृक्ष, &istate, stash_सूचकांक_path.buf, 0,
				शून्य)) {
		ret = -1;
		जाओ done;
	}

	अगर (commit_वृक्ष(untracked_संदेश.buf, untracked_संदेश.len,
			&info->u_वृक्ष, शून्य, &info->u_commit, शून्य, शून्य)) {
		ret = -1;
		जाओ done;
	}

done:
	release_सूचकांक(&istate);
	strbuf_release(&untracked_संदेश);
	remove_path(stash_सूचकांक_path.buf);
	वापस ret;
}

अटल पूर्णांक stash_staged(काष्ठा stash_info *info, काष्ठा strbuf *out_patch,
			पूर्णांक quiet)
{
	पूर्णांक ret = 0;
	काष्ठा child_process cp_diff_वृक्ष = CHILD_PROCESS_INIT;
	काष्ठा सूचकांक_state istate = सूचकांक_STATE_INIT(the_repository);

	अगर (write_सूचकांक_as_वृक्ष(&info->w_वृक्ष, &istate, the_repository->सूचकांक_file,
				0, शून्य)) {
		ret = -1;
		जाओ done;
	}

	cp_diff_वृक्ष.git_cmd = 1;
	strvec_pushl(&cp_diff_वृक्ष.args, "diff-tree", "-p", "--binary",
		     "--no-color",
		     "-U1", "HEAD", oid_to_hex(&info->w_वृक्ष), "--", शून्य);
	अगर (pipe_command(&cp_diff_वृक्ष, शून्य, 0, out_patch, 0, शून्य, 0)) {
		ret = -1;
		जाओ done;
	}

	अगर (!out_patch->len) {
		अगर (!quiet)
			fprintf_ln(मानक_त्रुटि, _("No staged changes"));
		ret = 1;
	}

done:
	release_सूचकांक(&istate);
	वापस ret;
}

अटल पूर्णांक stash_patch(काष्ठा stash_info *info, स्थिर काष्ठा pathspec *ps,
		       काष्ठा strbuf *out_patch, पूर्णांक quiet,
		       काष्ठा interactive_options *interactive_opts)
{
	पूर्णांक ret = 0;
	काष्ठा child_process cp_diff_वृक्ष = CHILD_PROCESS_INIT;
	काष्ठा commit *शीर्ष_commit;
	स्थिर काष्ठा object_id *शीर्ष_वृक्ष;
	काष्ठा सूचकांक_state istate = सूचकांक_STATE_INIT(the_repository);
	अक्षर *old_सूचकांक_env = शून्य, *old_repo_सूचकांक_file;

	remove_path(stash_सूचकांक_path.buf);

	शीर्ष_commit = lookup_commit(the_repository, &info->b_commit);
	अगर (!शीर्ष_commit || repo_parse_commit(the_repository, शीर्ष_commit)) {
		ret = -1;
		जाओ done;
	}
	शीर्ष_वृक्ष = get_commit_वृक्ष_oid(शीर्ष_commit);
	अगर (!शीर्ष_वृक्ष) {
		ret = -1;
		जाओ done;
	}

	अगर (create_सूचकांक_from_वृक्ष(शीर्ष_वृक्ष, stash_सूचकांक_path.buf)) {
		ret = -1;
		जाओ done;
	}

	/* Find out what the user wants. */
	old_repo_सूचकांक_file = the_repository->सूचकांक_file;
	the_repository->सूचकांक_file = stash_सूचकांक_path.buf;
	old_सूचकांक_env = xstrdup_or_null(दो_पर्या(सूचकांक_ENVIRONMENT));
	setenv(सूचकांक_ENVIRONMENT, the_repository->सूचकांक_file, 1);

	ret = !!run_add_p(the_repository, ADD_P_STASH, interactive_opts, शून्य, ps, 0);

	the_repository->सूचकांक_file = old_repo_सूचकांक_file;
	अगर (old_सूचकांक_env && *old_सूचकांक_env)
		setenv(सूचकांक_ENVIRONMENT, old_सूचकांक_env, 1);
	अन्यथा
		unsetenv(सूचकांक_ENVIRONMENT);
	FREE_AND_NULL(old_सूचकांक_env);

	/* State of the working tree. */
	अगर (write_सूचकांक_as_वृक्ष(&info->w_वृक्ष, &istate, stash_सूचकांक_path.buf, 0,
				शून्य)) {
		ret = -1;
		जाओ done;
	}

	cp_diff_वृक्ष.git_cmd = 1;
	strvec_pushl(&cp_diff_वृक्ष.args, "diff-tree", "-p", "-U1", "HEAD",
		     "--no-color",
		     oid_to_hex(&info->w_वृक्ष), "--", शून्य);
	अगर (pipe_command(&cp_diff_वृक्ष, शून्य, 0, out_patch, 0, शून्य, 0)) {
		ret = -1;
		जाओ done;
	}

	अगर (!out_patch->len) {
		अगर (!quiet)
			fprintf_ln(मानक_त्रुटि, _("No changes selected"));
		ret = 1;
	}

done:
	release_सूचकांक(&istate);
	remove_path(stash_सूचकांक_path.buf);
	वापस ret;
}

अटल पूर्णांक stash_working_वृक्ष(काष्ठा stash_info *info, स्थिर काष्ठा pathspec *ps)
{
	पूर्णांक ret = 0;
	काष्ठा rev_info rev;
	काष्ठा child_process cp_upd_सूचकांक = CHILD_PROCESS_INIT;
	काष्ठा strbuf diff_output = STRBUF_INIT;
	काष्ठा सूचकांक_state istate = सूचकांक_STATE_INIT(the_repository);

	repo_init_revisions(the_repository, &rev, शून्य);
	copy_pathspec(&rev.prune_तथ्य, ps);

	set_alternate_सूचकांक_output(stash_सूचकांक_path.buf);
	अगर (reset_वृक्ष(&info->i_वृक्ष, 0, 0)) {
		ret = -1;
		जाओ done;
	}
	set_alternate_सूचकांक_output(शून्य);

	rev.diffopt.output_format = DIFF_FORMAT_CALLBACK;
	rev.diffopt.format_callback = add_diff_to_buf;
	rev.diffopt.format_callback_तथ्य = &diff_output;

	अगर (repo_read_सूचकांक_preload(the_repository, &rev.diffopt.pathspec, 0) < 0) {
		ret = -1;
		जाओ done;
	}

	add_pending_object(&rev, parse_object(the_repository, &info->b_commit),
			   "");
	run_diff_सूचकांक(&rev, 0);

	cp_upd_सूचकांक.git_cmd = 1;
	strvec_pushl(&cp_upd_सूचकांक.args, "update-index",
		     "--ignore-skip-worktree-entries",
		     "-z", "--add", "--remove", "--stdin", शून्य);
	strvec_pushf(&cp_upd_सूचकांक.env, "GIT_INDEX_FILE=%s",
		     stash_सूचकांक_path.buf);

	अगर (pipe_command(&cp_upd_सूचकांक, diff_output.buf, diff_output.len,
			 शून्य, 0, शून्य, 0)) {
		ret = -1;
		जाओ done;
	}

	अगर (write_सूचकांक_as_वृक्ष(&info->w_वृक्ष, &istate, stash_सूचकांक_path.buf, 0,
				शून्य)) {
		ret = -1;
		जाओ done;
	}

done:
	release_सूचकांक(&istate);
	release_revisions(&rev);
	strbuf_release(&diff_output);
	remove_path(stash_सूचकांक_path.buf);
	वापस ret;
}

अटल पूर्णांक do_create_stash(स्थिर काष्ठा pathspec *ps, काष्ठा strbuf *stash_संदेश_buf,
			   पूर्णांक include_untracked, पूर्णांक patch_mode,
			   काष्ठा interactive_options *interactive_opts,
			   पूर्णांक only_staged, काष्ठा stash_info *info, काष्ठा strbuf *patch,
			   पूर्णांक quiet)
{
	पूर्णांक ret = 0;
	पूर्णांक flags = 0;
	पूर्णांक untracked_commit_option = 0;
	स्थिर अक्षर *शीर्ष_short_sha1 = शून्य;
	स्थिर अक्षर *branch_ref = शून्य;
	स्थिर अक्षर *branch_name = "(no branch)";
	अक्षर *branch_name_buf = शून्य;
	काष्ठा commit *शीर्ष_commit = शून्य;
	काष्ठा commit_list *parents = शून्य;
	काष्ठा strbuf संदेश = STRBUF_INIT;
	काष्ठा strbuf commit_वृक्ष_label = STRBUF_INIT;
	काष्ठा strbuf untracked_files = STRBUF_INIT;

	prepare_fallback_ident("git stash", "git@stash");

	repo_read_सूचकांक_preload(the_repository, शून्य, 0);
	अगर (repo_refresh_and_write_सूचकांक(the_repository, REFRESH_QUIET, 0, 0,
					 शून्य, शून्य, शून्य) < 0) {
		ret = error(_("could not write index"));
		जाओ done;
	}

	अगर (repo_get_oid(the_repository, "HEAD", &info->b_commit)) {
		अगर (!quiet)
			fprintf_ln(मानक_त्रुटि, _("You do not have "
					     "the initial commit yet"));
		ret = -1;
		जाओ done;
	} अन्यथा {
		शीर्ष_commit = lookup_commit(the_repository, &info->b_commit);
	}

	अगर (!check_changes(ps, include_untracked, &untracked_files)) {
		ret = 1;
		जाओ done;
	}

	branch_ref = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
					     "HEAD", 0, शून्य, &flags);

	अगर (flags & REF_ISSYMREF) {
		अगर (skip_prefix(branch_ref, "refs/heads/", &branch_name))
			branch_name = branch_name_buf = xstrdup(branch_name);
	}

	शीर्ष_short_sha1 = repo_find_unique_abbrev(the_repository,
						  &शीर्ष_commit->object.oid,
						  DEFAULT_ABBREV);
	strbuf_addf(&संदेश, "%s: %s ", branch_name, शीर्ष_short_sha1);
	pp_commit_easy(CMIT_FMT_ONELINE, शीर्ष_commit, &संदेश);

	strbuf_addf(&commit_वृक्ष_label, "index on %s\n", संदेश.buf);
	commit_list_जोड़ो(शीर्ष_commit, &parents);
	अगर (write_सूचकांक_as_वृक्ष(&info->i_वृक्ष, the_repository->सूचकांक,
				repo_get_सूचकांक_file(the_repository), 0, शून्य) ||
	    commit_वृक्ष(commit_वृक्ष_label.buf, commit_वृक्ष_label.len,
			&info->i_वृक्ष, parents, &info->i_commit, शून्य, शून्य)) {
		अगर (!quiet)
			fprintf_ln(मानक_त्रुटि, _("Cannot save the current "
					     "index state"));
		ret = -1;
		जाओ done;
	}

	commit_list_free(parents);
	parents = शून्य;

	अगर (include_untracked) {
		अगर (save_untracked_files(info, &संदेश, &untracked_files)) {
			अगर (!quiet)
				fprintf_ln(मानक_त्रुटि, _("Cannot save "
						     "the untracked files"));
			ret = -1;
			जाओ done;
		}
		untracked_commit_option = 1;
	}
	अगर (patch_mode) {
		ret = stash_patch(info, ps, patch, quiet, interactive_opts);
		अगर (ret < 0) {
			अगर (!quiet)
				fprintf_ln(मानक_त्रुटि, _("Cannot save the current "
						     "worktree state"));
			जाओ done;
		} अन्यथा अगर (ret > 0) {
			जाओ done;
		}
	} अन्यथा अगर (only_staged) {
		ret = stash_staged(info, patch, quiet);
		अगर (ret < 0) {
			अगर (!quiet)
				fprintf_ln(मानक_त्रुटि, _("Cannot save the current "
						     "staged state"));
			जाओ done;
		} अन्यथा अगर (ret > 0) {
			जाओ done;
		}
	} अन्यथा {
		अगर (stash_working_वृक्ष(info, ps)) {
			अगर (!quiet)
				fprintf_ln(मानक_त्रुटि, _("Cannot save the current "
						     "worktree state"));
			ret = -1;
			जाओ done;
		}
	}

	अगर (!stash_संदेश_buf->len)
		strbuf_addf(stash_संदेश_buf, "WIP on %s", संदेश.buf);
	अन्यथा
		strbuf_insertf(stash_संदेश_buf, 0, "On %s: ", branch_name);

	अगर (untracked_commit_option)
		commit_list_जोड़ो(lookup_commit(the_repository,
						 &info->u_commit),
				   &parents);
	commit_list_जोड़ो(lookup_commit(the_repository, &info->i_commit),
			   &parents);
	commit_list_जोड़ो(शीर्ष_commit, &parents);

	अगर (commit_वृक्ष(stash_संदेश_buf->buf, stash_संदेश_buf->len, &info->w_वृक्ष,
			parents, &info->w_commit, शून्य, शून्य)) {
		अगर (!quiet)
			fprintf_ln(मानक_त्रुटि, _("Cannot record "
					     "working tree state"));
		ret = -1;
		जाओ done;
	}

done:
	strbuf_release(&commit_वृक्ष_label);
	strbuf_release(&संदेश);
	strbuf_release(&untracked_files);
	commit_list_free(parents);
	मुक्त(branch_name_buf);
	वापस ret;
}

अटल पूर्णांक create_stash(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix UNUSED,
			काष्ठा repository *repo UNUSED)
{
	पूर्णांक ret;
	काष्ठा strbuf stash_संदेश_buf = STRBUF_INIT;
	काष्ठा stash_info info = STASH_INFO_INIT;
	काष्ठा pathspec ps;

	/* Starting with argv[1], since argv[0] is "create" */
	strbuf_join_argv(&stash_संदेश_buf, argc - 1, ++argv, ' ');

	स_रखो(&ps, 0, माप(ps));
	अगर (!check_changes_tracked_files(&ps))
		वापस 0;

	ret = do_create_stash(&ps, &stash_संदेश_buf, 0, 0, शून्य, 0, &info,
			      शून्य, 0);
	अगर (!ret)
		printf_ln("%s", oid_to_hex(&info.w_commit));

	free_stash_info(&info);
	strbuf_release(&stash_संदेश_buf);
	वापस ret;
}

अटल पूर्णांक do_धकेलो_stash(स्थिर काष्ठा pathspec *ps, स्थिर अक्षर *stash_संदेश, पूर्णांक quiet,
			 पूर्णांक keep_सूचकांक, पूर्णांक patch_mode,
			 काष्ठा interactive_options *interactive_opts,
			 पूर्णांक include_untracked, पूर्णांक only_staged)
{
	पूर्णांक ret = 0;
	काष्ठा stash_info info = STASH_INFO_INIT;
	काष्ठा strbuf patch = STRBUF_INIT;
	काष्ठा strbuf stash_संदेश_buf = STRBUF_INIT;
	काष्ठा strbuf untracked_files = STRBUF_INIT;
	काष्ठा strbuf out = STRBUF_INIT;

	अगर (patch_mode && keep_सूचकांक == -1)
		keep_सूचकांक = 1;

	अगर (patch_mode && include_untracked) {
		fprintf_ln(मानक_त्रुटि, _("Can't use --patch and --include-untracked"
				     " or --all at the same time"));
		ret = -1;
		जाओ done;
	}

	/* --patch overrides --staged */
	अगर (patch_mode)
		only_staged = 0;

	अगर (only_staged && include_untracked) {
		fprintf_ln(मानक_त्रुटि, _("Can't use --staged and --include-untracked"
				     " or --all at the same time"));
		ret = -1;
		जाओ done;
	}

	repo_read_सूचकांक_preload(the_repository, शून्य, 0);
	अगर (!include_untracked && ps->nr) {
		अक्षर *ps_matched = xcalloc(ps->nr, 1);

		अगर (pathspec_needs_expanded_सूचकांक(the_repository->सूचकांक, ps))
			ensure_full_सूचकांक(the_repository->सूचकांक);
		क्रम (माप_प्रकार i = 0; i < the_repository->सूचकांक->cache_nr; i++)
			ce_path_match(the_repository->सूचकांक, the_repository->सूचकांक->cache[i], ps,
				      ps_matched);

		अगर (report_path_error(ps_matched, ps)) {
			fprintf_ln(मानक_त्रुटि, _("Did you forget to 'git add'?"));
			ret = -1;
			मुक्त(ps_matched);
			जाओ done;
		}
		मुक्त(ps_matched);
	}

	अगर (repo_refresh_and_write_सूचकांक(the_repository, REFRESH_QUIET, 0, 0,
					 शून्य, शून्य, शून्य)) {
		ret = error(_("could not write index"));
		जाओ done;
	}

	अगर (!check_changes(ps, include_untracked, &untracked_files)) {
		अगर (!quiet)
			printf_ln(_("No local changes to save"));
		जाओ done;
	}

	अगर (!refs_reflog_exists(get_main_ref_store(the_repository), ref_stash) && do_clear_stash()) {
		ret = -1;
		अगर (!quiet)
			fprintf_ln(मानक_त्रुटि, _("Cannot initialize stash"));
		जाओ done;
	}

	अगर (stash_संदेश)
		strbuf_addstr(&stash_संदेश_buf, stash_संदेश);
	अगर (do_create_stash(ps, &stash_संदेश_buf, include_untracked, patch_mode,
			    interactive_opts, only_staged, &info, &patch, quiet)) {
		ret = -1;
		जाओ done;
	}

	अगर (do_store_stash(&info.w_commit, stash_संदेश_buf.buf, 1)) {
		ret = -1;
		अगर (!quiet)
			fprintf_ln(मानक_त्रुटि, _("Cannot save the current status"));
		जाओ done;
	}

	अगर (!quiet)
		printf_ln(_("Saved working directory and index state %s"),
			  stash_संदेश_buf.buf);

	अगर (!(patch_mode || only_staged)) {
		अगर (include_untracked && !ps->nr) {
			काष्ठा child_process cp = CHILD_PROCESS_INIT;

			cp.git_cmd = 1;
			अगर (startup_info->original_cwd) {
				cp.dir = startup_info->original_cwd;
				strvec_pushf(&cp.env, "%s=%s",
					     GIT_WORK_वृक्ष_ENVIRONMENT,
					     the_repository->worktree);
			}
			strvec_pushl(&cp.args, "clean", "--force",
				     "--quiet", "-d", ":/", शून्य);
			अगर (include_untracked == INCLUDE_ALL_FILES)
				strvec_धकेलो(&cp.args, "-x");
			अगर (run_command(&cp)) {
				ret = -1;
				जाओ done;
			}
		}
		discard_सूचकांक(the_repository->सूचकांक);
		अगर (ps->nr) {
			काष्ठा child_process cp_add = CHILD_PROCESS_INIT;
			काष्ठा child_process cp_diff = CHILD_PROCESS_INIT;
			काष्ठा child_process cp_apply = CHILD_PROCESS_INIT;

			cp_add.git_cmd = 1;
			strvec_धकेलो(&cp_add.args, "add");
			अगर (!include_untracked)
				strvec_धकेलो(&cp_add.args, "-u");
			अगर (include_untracked == INCLUDE_ALL_FILES)
				strvec_धकेलो(&cp_add.args, "--force");
			strvec_धकेलो(&cp_add.args, "--");
			add_pathspecs(&cp_add.args, ps);
			अगर (run_command(&cp_add)) {
				ret = -1;
				जाओ done;
			}

			cp_diff.git_cmd = 1;
			strvec_pushl(&cp_diff.args, "diff-index", "-p",
				     "--no-color",
				     "--cached", "--binary", "HEAD", "--",
				     शून्य);
			add_pathspecs(&cp_diff.args, ps);
			अगर (pipe_command(&cp_diff, शून्य, 0, &out, 0, शून्य, 0)) {
				ret = -1;
				जाओ done;
			}

			cp_apply.git_cmd = 1;
			strvec_pushl(&cp_apply.args, "apply", "--index",
				     "-R", शून्य);
			अगर (pipe_command(&cp_apply, out.buf, out.len, शून्य, 0,
					 शून्य, 0)) {
				ret = -1;
				जाओ done;
			}
		} अन्यथा {
			काष्ठा child_process cp = CHILD_PROCESS_INIT;
			cp.git_cmd = 1;
			/* BUG: this nukes untracked files in the way */
			strvec_pushl(&cp.args, "reset", "--hard", "-q",
				     "--no-recurse-submodules", शून्य);
			अगर (run_command(&cp)) {
				ret = -1;
				जाओ done;
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
		अगर (keep_सूचकांक == 1 && !is_null_oid(&info.i_वृक्ष) &&
		    !is_empty_वृक्ष_oid(&info.i_वृक्ष, the_repository->hash_algo)) {
			काष्ठा child_process cp = CHILD_PROCESS_INIT;

			cp.git_cmd = 1;
			strvec_pushl(&cp.args, "checkout", "--no-overlay",
				     oid_to_hex(&info.i_वृक्ष), "--", शून्य);
			अगर (!ps->nr)
				strvec_धकेलो(&cp.args, ":/");
			अन्यथा
				add_pathspecs(&cp.args, ps);
			अगर (run_command(&cp)) {
				ret = -1;
				जाओ done;
			}
		}
		जाओ done;
	} अन्यथा {
		काष्ठा child_process cp = CHILD_PROCESS_INIT;

		cp.git_cmd = 1;
		strvec_pushl(&cp.args, "apply", "-R", शून्य);

		अगर (pipe_command(&cp, patch.buf, patch.len, शून्य, 0, शून्य, 0)) {
			अगर (!quiet)
				fprintf_ln(मानक_त्रुटि, _("Cannot remove "
						     "worktree changes"));
			ret = -1;
			जाओ done;
		}

		अगर (keep_सूचकांक < 1) {
			काष्ठा child_process cp = CHILD_PROCESS_INIT;

			cp.git_cmd = 1;
			strvec_pushl(&cp.args, "reset", "-q", "--refresh", "--",
				     शून्य);
			add_pathspecs(&cp.args, ps);
			अगर (run_command(&cp)) {
				ret = -1;
				जाओ done;
			}
		}
		जाओ done;
	}

done:
	strbuf_release(&patch);
	strbuf_release(&out);
	free_stash_info(&info);
	strbuf_release(&stash_संदेश_buf);
	strbuf_release(&untracked_files);
	वापस ret;
}

अटल पूर्णांक धकेलो_stash(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
		      पूर्णांक धकेलो_assumed)
{
	पूर्णांक force_assume = 0;
	पूर्णांक keep_सूचकांक = -1;
	पूर्णांक only_staged = 0;
	पूर्णांक patch_mode = 0;
	पूर्णांक include_untracked = 0;
	पूर्णांक quiet = 0;
	पूर्णांक pathspec_file_nul = 0;
	स्थिर अक्षर *stash_संदेश = शून्य;
	अक्षर *pathspec_from_file = शून्य;
	काष्ठा pathspec ps;
	काष्ठा interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	काष्ठा option options[] = {
		OPT_BOOL('k', "keep-index", &keep_सूचकांक,
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
		OPT_STRING('m', "message", &stash_संदेश, N_("message"),
			   N_("stash message")),
		OPT_PATHSPEC_FROM_FILE(&pathspec_from_file),
		OPT_PATHSPEC_FILE_NUL(&pathspec_file_nul),
		OPT_END()
	};
	पूर्णांक ret;

	अगर (argc) {
		पूर्णांक flags = PARSE_OPT_KEEP_DASHDASH;

		अगर (धकेलो_assumed)
			flags |= PARSE_OPT_STOP_AT_NON_OPTION;

		argc = parse_options(argc, argv, prefix, options,
				     धकेलो_assumed ? git_stash_usage :
				     git_stash_धकेलो_usage, flags);
		force_assume |= patch_mode;
	}

	अगर (argc) {
		अगर (!म_भेद(argv[0], "--")) {
			argc--;
			argv++;
		} अन्यथा अगर (धकेलो_assumed && !force_assume) {
			die("subcommand wasn't specified; 'push' can't be assumed due to unexpected token '%s'",
			    argv[0]);
		}
	}

	parse_pathspec(&ps, 0, PATHSPEC_PREFER_FULL | PATHSPEC_PREFIX_ORIGIN,
		       prefix, argv);

	अगर (pathspec_from_file) {
		अगर (patch_mode)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--patch");

		अगर (only_staged)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--staged");

		अगर (ps.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&ps, 0,
				    PATHSPEC_PREFER_FULL | PATHSPEC_PREFIX_ORIGIN,
				    prefix, pathspec_from_file, pathspec_file_nul);
	} अन्यथा अगर (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	अगर (!patch_mode) {
		अगर (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--patch");
		अगर (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--patch");
		अगर (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--patch");
	}

	अगर (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	अगर (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	ret = do_धकेलो_stash(&ps, stash_संदेश, quiet, keep_सूचकांक, patch_mode,
			    &interactive_opts, include_untracked, only_staged);

	clear_pathspec(&ps);
	मुक्त(pathspec_from_file);
	वापस ret;
}

अटल पूर्णांक धकेलो_stash_unassumed(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
				काष्ठा repository *repo UNUSED)
{
	वापस धकेलो_stash(argc, argv, prefix, 0);
}

अटल पूर्णांक save_stash(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
		      काष्ठा repository *repo UNUSED)
{
	पूर्णांक keep_सूचकांक = -1;
	पूर्णांक only_staged = 0;
	पूर्णांक patch_mode = 0;
	पूर्णांक include_untracked = 0;
	पूर्णांक quiet = 0;
	पूर्णांक ret = 0;
	स्थिर अक्षर *stash_संदेश = शून्य;
	काष्ठा pathspec ps;
	काष्ठा strbuf stash_संदेश_buf = STRBUF_INIT;
	काष्ठा interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	काष्ठा option options[] = {
		OPT_BOOL('k', "keep-index", &keep_सूचकांक,
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
		OPT_STRING('m', "message", &stash_संदेश, "message",
			   N_("stash message")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_save_usage,
			     PARSE_OPT_KEEP_DASHDASH);

	अगर (argc)
		stash_संदेश = strbuf_join_argv(&stash_संदेश_buf, argc, argv, ' ');

	स_रखो(&ps, 0, माप(ps));

	अगर (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	अगर (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	अगर (!patch_mode) {
		अगर (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--patch");
		अगर (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--patch");
		अगर (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--patch");
	}

	ret = do_धकेलो_stash(&ps, stash_संदेश, quiet, keep_सूचकांक,
			    patch_mode, &interactive_opts, include_untracked,
			    only_staged);

	strbuf_release(&stash_संदेश_buf);
	वापस ret;
}

अटल पूर्णांक write_commit_with_parents(काष्ठा repository *r,
				     काष्ठा object_id *out,
				     स्थिर काष्ठा object_id *oid,
				     काष्ठा commit_list *parents)
{
	माप_प्रकार author_len, committer_len;
	काष्ठा commit *this;
	स्थिर अक्षर *orig_author, *orig_committer;
	अक्षर *author = शून्य, *committer = शून्य;
	स्थिर अक्षर *बफर;
	अचिन्हित दीर्घ bufsize;
	स्थिर अक्षर *p;
	काष्ठा strbuf संदेश = STRBUF_INIT;
	पूर्णांक ret = 0;
	काष्ठा ident_split id;

	this = lookup_commit_reference(r, oid);
	बफर = repo_get_commit_बफर(r, this, &bufsize);
	orig_author = find_commit_header(बफर, "author", &author_len);
	orig_committer = find_commit_header(बफर, "committer", &committer_len);

	अगर (!orig_author || !orig_committer) {
		ret = error(_("cannot parse commit %s"), oid_to_hex(oid));
		जाओ out;
	}

	अगर (split_ident_पंक्ति(&id, orig_author, author_len) < 0 ||
	    split_ident_पंक्ति(&id, orig_committer, committer_len) < 0) {
		ret = error(_("invalid author or committer for %s"), oid_to_hex(oid));
		जाओ out;
	}

	p = म_माला(बफर, "\n\n");
	strbuf_addstr(&संदेश, "git stash: ");

	अगर (p)
		strbuf_add(&संदेश, p + 2, bufsize - (p + 2 - बफर));
	strbuf_complete_पंक्ति(&संदेश);

	author = xmemdupz(orig_author, author_len);
	committer = xmemdupz(orig_committer, committer_len);

	अगर (commit_वृक्ष_extended(संदेश.buf, संदेश.len,
				 r->hash_algo->empty_वृक्ष, parents,
				 out, author, committer,
				 शून्य, शून्य)) {
		ret = error(_("could not write commit"));
		जाओ out;
	}
out:
	strbuf_release(&संदेश);
	repo_unuse_commit_बफर(r, this, बफर);
	मुक्त(author);
	मुक्त(committer);
	वापस ret;
}

अटल पूर्णांक do_import_stash(काष्ठा repository *r, स्थिर अक्षर *rev)
{
	काष्ठा object_id chain;
	पूर्णांक res = 0;
	स्थिर अक्षर *बफर = शून्य;
	अचिन्हित दीर्घ bufsize;
	काष्ठा commit *this = शून्य;
	काष्ठा commit_list *वस्तुएँ = शून्य, *cur;
	अक्षर *संदेश = शून्य;

	अगर (repo_get_oid(r, rev, &chain))
		वापस error(_("not a valid revision: %s"), rev);

	this = lookup_commit_reference(r, &chain);
	अगर (!this)
		वापस error(_("not a commit: %s"), rev);

	/*
	 * Walk the commit history, finding each stash entry, and load data into
	 * the array.
	 */
	क्रम (;;) {
		स्थिर अक्षर *author, *committer;
		माप_प्रकार author_len, committer_len;
		स्थिर अक्षर *p;
		स्थिर अक्षर *expected = "git stash <git@stash> 1000684800 +0000";
		स्थिर अक्षर *prefix = "git stash: ";
		काष्ठा commit *stash;
		काष्ठा वृक्ष *वृक्ष = repo_get_commit_वृक्ष(r, this);

		अगर (!वृक्ष ||
		    !oideq(&वृक्ष->object.oid, r->hash_algo->empty_वृक्ष) ||
		    (this->parents &&
		     (!this->parents->अग्र || this->parents->अग्र->अग्र))) {
			res = error(_("%s is not a valid exported stash commit"),
					oid_to_hex(&this->object.oid));
			जाओ out;
		}

		बफर = repo_get_commit_बफर(r, this, &bufsize);

		अगर (!this->parents) {
			/*
			 * We don't have any parents.  Make sure this is our
			 * root commit.
			 */
			author = find_commit_header(बफर, "author", &author_len);
			committer = find_commit_header(बफर, "committer", &committer_len);

			अगर (!author || !committer) {
				error(_("cannot parse commit %s"), oid_to_hex(&this->object.oid));
				जाओ out;
			}

			अगर (author_len != म_माप(expected) ||
			    committer_len != म_माप(expected) ||
			    स_भेद(author, expected, author_len) ||
			    स_भेद(committer, expected, committer_len)) {
				res = error(_("found root commit %s with invalid data"), oid_to_hex(&this->object.oid));
				जाओ out;
			}
			अवरोध;
		}

		p = म_माला(बफर, "\n\n");
		अगर (!p) {
			res = error(_("cannot parse commit %s"), oid_to_hex(&this->object.oid));
			जाओ out;
		}

		p += 2;
		अगर (((माप_प्रकार)(bufsize - (p - बफर)) < म_माप(prefix)) ||
		    स_भेद(prefix, p, म_माप(prefix))) {
			res = error(_("found stash commit %s without expected prefix"), oid_to_hex(&this->object.oid));
			जाओ out;
		}

		stash = this->parents->अग्र->item;

		अगर (repo_parse_commit(r, this->parents->item) ||
		    repo_parse_commit(r, stash)) {
			res = error(_("cannot parse parents of commit: %s"),
					oid_to_hex(&this->object.oid));
			जाओ out;
		}

		अगर (check_stash_topology(r, stash)) {
			res = error(_("%s does not look like a stash commit"),
					oid_to_hex(&stash->object.oid));
			जाओ out;
		}

		repo_unuse_commit_बफर(r, this, बफर);
		बफर = शून्य;
		वस्तुएँ = commit_list_जोड़ो(stash, &वस्तुएँ);
		this = this->parents->item;
	}

	/*
	 * Now, walk each entry, adding it to the stash as a normal stash
	 * commit.
	 */
	क्रम (cur = वस्तुएँ; cur; cur = cur->अग्र) {
		स्थिर अक्षर *p;
		काष्ठा object_id *oid;

		this = cur->item;
		oid = &this->object.oid;
		बफर = repo_get_commit_बफर(r, this, &bufsize);
		अगर (!बफर) {
			res = error(_("cannot read commit buffer for %s"), oid_to_hex(oid));
			जाओ out;
		}

		p = म_माला(बफर, "\n\n");
		अगर (!p) {
			res = error(_("cannot parse commit %s"), oid_to_hex(oid));
			जाओ out;
		}

		p += 2;
		संदेश = xmemdupz(p, bufsize - (p - बफर));
		repo_unuse_commit_बफर(r, this, बफर);
		बफर = शून्य;

		अगर (do_store_stash(oid, संदेश, 1)) {
			res = error(_("cannot save the stash for %s"), oid_to_hex(oid));
			जाओ out;
		}
		FREE_AND_NULL(संदेश);
	}
out:
	अगर (this && बफर)
		repo_unuse_commit_बफर(r, this, बफर);
	commit_list_free(वस्तुएँ);
	मुक्त(संदेश);

	वापस res;
}

अटल पूर्णांक import_stash(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
			काष्ठा repository *repo)
{
	काष्ठा option options[] = {
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_import_usage,
			     PARSE_OPT_KEEP_DASHDASH);

	अगर (argc != 1)
		usage_संदेश_opt("a revision is required", git_stash_import_usage, options);

	वापस do_import_stash(repo, argv[0]);
}

काष्ठा stash_entry_तथ्य {
	काष्ठा repository *r;
	काष्ठा commit_list **वस्तुएँ;
	माप_प्रकार गिनती;
};

अटल पूर्णांक collect_stash_entries(स्थिर अक्षर *refname UNUSED,
				 काष्ठा object_id *old_oid UNUSED,
				 काष्ठा object_id *new_oid,
				 स्थिर अक्षर *committer UNUSED,
				 timestamp_t timestamp UNUSED,
				 पूर्णांक tz UNUSED, स्थिर अक्षर *संदेश UNUSED,
				 व्योम *cb_तथ्य)
{
	काष्ठा stash_entry_तथ्य *तथ्य = cb_तथ्य;
	काष्ठा commit *stash;

	तथ्य->गिनती++;
	stash = lookup_commit_reference(तथ्य->r, new_oid);
	अगर (!stash || check_stash_topology(तथ्य->r, stash)) {
		वापस error(_("%s does not look like a stash commit"),
			     oid_to_hex(new_oid));
	}
	तथ्य->वस्तुएँ = commit_list_append(stash, तथ्य->वस्तुएँ);
	वापस 0;
}

अटल पूर्णांक do_export_stash(काष्ठा repository *r,
			   स्थिर अक्षर *ref,
			   पूर्णांक argc,
			   स्थिर अक्षर **argv)
{
	काष्ठा object_id base;
	काष्ठा commit *पूर्व;
	काष्ठा commit_list *वस्तुएँ = शून्य, **iter = &वस्तुएँ, *cur;
	पूर्णांक res = 0;
	पूर्णांक i;
	काष्ठा strbuf revision = STRBUF_INIT;
	स्थिर अक्षर *author, *committer;

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
	अगर (commit_वृक्ष_extended("", 0, r->hash_algo->empty_वृक्ष, शून्य,
				 &base, author, committer, शून्य, शून्य))
		वापस error(_("unable to write base commit"));

	पूर्व = lookup_commit_reference(r, &base);

	अगर (argc) {
		/*
		 * Find each specified stash, and load data into the array.
		 */
		क्रम (i = 0; i < argc; i++) {
			काष्ठा object_id oid;
			काष्ठा commit *stash;

			अगर (parse_stash_revision(&revision, argv[i], 1) ||
			    repo_get_oid_with_flags(r, revision.buf, &oid,
						    GET_OID_QUIETLY |
						    GET_OID_GENTLY)) {
				res = error(_("unable to find stash entry %s"), argv[i]);
				जाओ out;
			}

			stash = lookup_commit_reference(r, &oid);
			अगर (!stash || check_stash_topology(r, stash)) {
				res = error(_("%s does not look like a stash commit"),
						revision.buf);
				जाओ out;
			}
			iter = commit_list_append(stash, iter);
		}
	} अन्यथा {
		/*
		 * Walk the reflog, finding each stash entry, and load data into the
		 * array.
		 */
		काष्ठा stash_entry_तथ्य cb_तथ्य = {
			.r = r, .वस्तुएँ = iter,
		};
		अगर (refs_for_each_reflog_ent_reverse(get_main_ref_store(r),
						     "refs/stash",
						     collect_stash_entries,
						     &cb_तथ्य) && cb_तथ्य.गिनती)
			जाओ out;
	}

	/*
	 * Now, create a set of commits identical to the regular stash commits,
	 * but where their first parents form a chain to our original empty
	 * base commit.
	 */
	वस्तुएँ = commit_list_reverse(वस्तुएँ);
	क्रम (cur = वस्तुएँ; cur; cur = cur->अग्र) {
		काष्ठा commit_list *parents = शून्य;
		काष्ठा commit_list **अग्र = &parents;
		काष्ठा object_id out;
		काष्ठा commit *stash = cur->item;

		अग्र = commit_list_append(पूर्व, अग्र);
		अग्र = commit_list_append(stash, अग्र);
		res = write_commit_with_parents(r, &out, &stash->object.oid, parents);
		commit_list_free(parents);
		अगर (res)
			जाओ out;
		पूर्व = lookup_commit_reference(r, &out);
	}
	अगर (ref)
		refs_update_ref(get_main_ref_store(r), शून्य, ref,
				&पूर्व->object.oid, शून्य, 0, UPDATE_REFS_DIE_ON_ERR);
	अन्यथा
		माला_दो(oid_to_hex(&पूर्व->object.oid));
out:
	strbuf_release(&revision);
	commit_list_free(वस्तुएँ);

	वापस res;
}

क्रमागत export_action {
	ACTION_NONE,
	ACTION_PRINT,
	ACTION_TO_REF,
};

अटल पूर्णांक export_stash(पूर्णांक argc,
			स्थिर अक्षर **argv,
			स्थिर अक्षर *prefix,
			काष्ठा repository *repo)
{
	स्थिर अक्षर *ref = शून्य;
	क्रमागत export_action action = ACTION_NONE;
	काष्ठा option options[] = {
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

	अगर (ref && action == ACTION_NONE)
		action = ACTION_TO_REF;

	अगर (action == ACTION_NONE || (ref && action == ACTION_PRINT))
		वापस error(_("exactly one of --print and --to-ref is required"));

	वापस do_export_stash(repo, ref, argc, argv);
}

पूर्णांक cmd_stash(पूर्णांक argc,
	      स्थिर अक्षर **argv,
	      स्थिर अक्षर *prefix,
	      काष्ठा repository *repo)
{
	pid_t pid = getpid();
	स्थिर अक्षर *सूचकांक_file;
	काष्ठा strvec args = STRVEC_INIT;
	parse_opt_subcommand_fn *fn = शून्य;
	काष्ठा option options[] = {
		OPT_SUBCOMMAND("apply", &fn, apply_stash),
		OPT_SUBCOMMAND("clear", &fn, clear_stash),
		OPT_SUBCOMMAND("drop", &fn, drop_stash),
		OPT_SUBCOMMAND("pop", &fn, निकालो_stash),
		OPT_SUBCOMMAND("branch", &fn, branch_stash),
		OPT_SUBCOMMAND("list", &fn, list_stash),
		OPT_SUBCOMMAND("show", &fn, show_stash),
		OPT_SUBCOMMAND("store", &fn, store_stash),
		OPT_SUBCOMMAND("create", &fn, create_stash),
		OPT_SUBCOMMAND("push", &fn, धकेलो_stash_unassumed),
		OPT_SUBCOMMAND("export", &fn, export_stash),
		OPT_SUBCOMMAND("import", &fn, import_stash),
		OPT_SUBCOMMAND_F("save", &fn, save_stash, शून्य, PARSE_OPT_NOCOMPLETE),
		OPT_END()
	};
	स्थिर अक्षर **args_copy;
	पूर्णांक ret;

	repo_config(the_repository, git_stash_config, शून्य);

	argc = parse_options(argc, argv, prefix, options, git_stash_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL |
			     PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_सूचकांक = 0;

	सूचकांक_file = repo_get_सूचकांक_file(the_repository);
	strbuf_addf(&stash_सूचकांक_path, "%s.stash.%" PRIuMAX, सूचकांक_file,
		    (uintmax_t)pid);

	अगर (fn) {
		ret = fn(argc, argv, prefix, repo);

		/*
		 * The subcommand implementations return 0 on success, a
		 * negative value on failure, and STASH_APPLY_CONFLICT
		 * when applying a stash entry resulted in conflicts.
		 * Map failures to 128, the status die() uses, so that
		 * exit status 1 unambiguously indicates conflicts.
		 */
		अगर (ret < 0)
			वापस 128;
		वापस ret;
	} अन्यथा अगर (!argc) {
		वापस !!धकेलो_stash_unassumed(0, शून्य, prefix, repo);
	}

	/* Assume 'stash push' */
	strvec_धकेलो(&args, "push");
	strvec_pushv(&args, argv);

	/*
	 * `push_stash()` ends up modifying the array, which causes memory
	 * leaks if we didn't copy the array here.
	 */
	DUP_ARRAY(args_copy, args.v, args.nr);

	ret = !!धकेलो_stash(args.nr, args_copy, prefix, 1);

	strvec_clear(&args);
	मुक्त(args_copy);
	वापस ret;
}
