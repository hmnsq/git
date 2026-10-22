#परिभाषय USE_THE_REPOSITORY_VARIABLE

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

#परिभाषय INCLUDE_ALL_FILES 2

#परिभाषय BUILTIN_STASH_LIST_USAGE \
	N_("git stash list [<log-options>]")
#परिभाषय BUILTIN_STASH_SHOW_USAGE \
	N_("git stash show [-u | --include-untracked | --only-untracked] [<diff-options>] [<stash>]")
#परिभाषय BUILTIN_STASH_DROP_USAGE \
	N_("git stash drop [-q | --quiet] [<stash>]")
#परिभाषय BUILTIN_STASH_निष्कासय_USAGE \
	N_("git stash pop [--index] [-q | --quiet] [<stash>]")
#परिभाषय BUILTIN_STASH_APPLY_USAGE \
	N_("git stash apply [--index] [-q | --quiet] [--label-ours=<label>] [--label-theirs=<label>] [--label-base=<label>] [<stash>]")
#परिभाषय BUILTIN_STASH_BRANCH_USAGE \
	N_("git stash branch <branchname> [<stash>]")
#परिभाषय BUILTIN_STASH_STORE_USAGE \
	N_("git stash store [(-m | --message) <message>] [-q | --quiet] <commit>")
#परिभाषय BUILTIN_STASH_प्रक्षिप_USAGE \
	N_("git stash [push] [-p | --patch] [-S | --staged] [-k | --[no-]keep-index] [-q | --quiet]\n" \
	   "          [-u | --include-untracked] [-a | --all] [(-m | --message) <message>]\n" \
	   "          [--pathspec-from-file=<file> [--pathspec-file-nul]]\n" \
	   "          [--] [<pathspec>...]")
#परिभाषय BUILTIN_STASH_SAVE_USAGE \
	N_("git stash save [-p | --patch] [-S | --staged] [-k | --[no-]keep-index] [-q | --quiet]\n" \
	   "          [-u | --include-untracked] [-a | --all] [<message>]")
#परिभाषय BUILTIN_STASH_CREATE_USAGE \
	N_("git stash create [<message>]")
#परिभाषय BUILTIN_STASH_EXPORT_USAGE \
	N_("git stash export (--print | --to-ref <ref>) [<stash>...]")
#परिभाषय BUILTIN_STASH_IMPORT_USAGE \
	N_("git stash import <commit>")
#परिभाषय BUILTIN_STASH_CLEAR_USAGE \
	"git stash clear"

स्थिर ध्रुव वर्ण * ध्रुव git_stash_usage[] = {
	BUILTIN_STASH_LIST_USAGE,
	BUILTIN_STASH_SHOW_USAGE,
	BUILTIN_STASH_DROP_USAGE,
	BUILTIN_STASH_निष्कासय_USAGE,
	BUILTIN_STASH_APPLY_USAGE,
	BUILTIN_STASH_BRANCH_USAGE,
	BUILTIN_STASH_प्रक्षिप_USAGE,
	BUILTIN_STASH_SAVE_USAGE,
	BUILTIN_STASH_CLEAR_USAGE,
	BUILTIN_STASH_CREATE_USAGE,
	BUILTIN_STASH_STORE_USAGE,
	BUILTIN_STASH_EXPORT_USAGE,
	BUILTIN_STASH_IMPORT_USAGE,
	रिक्त
};

स्थिर ध्रुव वर्ण * ध्रुव git_stash_list_usage[] = {
	BUILTIN_STASH_LIST_USAGE,
	रिक्त
};

स्थिर ध्रुव वर्ण * ध्रुव git_stash_show_usage[] = {
	BUILTIN_STASH_SHOW_USAGE,
	रिक्त
};

स्थिर ध्रुव वर्ण * ध्रुव git_stash_drop_usage[] = {
	BUILTIN_STASH_DROP_USAGE,
	रिक्त
};

स्थिर ध्रुव वर्ण * ध्रुव git_stash_निष्कासय_usage[] = {
	BUILTIN_STASH_निष्कासय_USAGE,
	रिक्त
};

स्थिर ध्रुव वर्ण * ध्रुव git_stash_apply_usage[] = {
	BUILTIN_STASH_APPLY_USAGE,
	रिक्त
};

स्थिर ध्रुव वर्ण * ध्रुव git_stash_branch_usage[] = {
	BUILTIN_STASH_BRANCH_USAGE,
	रिक्त
};

स्थिर ध्रुव वर्ण * ध्रुव git_stash_clear_usage[] = {
	BUILTIN_STASH_CLEAR_USAGE,
	रिक्त
};

स्थिर ध्रुव वर्ण * ध्रुव git_stash_store_usage[] = {
	BUILTIN_STASH_STORE_USAGE,
	रिक्त
};

स्थिर ध्रुव वर्ण * ध्रुव git_stash_प्रक्षिप_usage[] = {
	BUILTIN_STASH_प्रक्षिप_USAGE,
	रिक्त
};

स्थिर ध्रुव वर्ण * ध्रुव git_stash_save_usage[] = {
	BUILTIN_STASH_SAVE_USAGE,
	रिक्त
};

स्थिर ध्रुव वर्ण * ध्रुव git_stash_export_usage[] = {
	BUILTIN_STASH_EXPORT_USAGE,
	रिक्त
};

स्थिर ध्रुव वर्ण * ध्रुव git_stash_import_usage[] = {
	BUILTIN_STASH_IMPORT_USAGE,
	रिक्त
};

स्थिर ध्रुव वर्ण ref_stash[] = "refs/stash";
स्थिर संरचना strbuf stash_सूचकांक_मार्गः = STRBUF_INIT;

स्थिर पूर्णाङ्क show_stat = 1;
स्थिर पूर्णाङ्क show_patch;
स्थिर पूर्णाङ्क show_include_untracked;
स्थिर पूर्णाङ्क use_सूचकांक;

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
	संरचना object_id w_वृक्षः;
	संरचना object_id b_वृक्षः;
	संरचना object_id i_वृक्षः;
	संरचना object_id u_वृक्षः;
	संरचना strbuf revision;
	पूर्णाङ्क is_stash_ref;
	पूर्णाङ्क has_u;
};

#परिभाषय STASH_INFO_INIT { \
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
	यदि (!stash->parents || !stash->parents->अग्र ||
			(stash->parents->अग्र->अग्र && stash->parents->अग्र->अग्र->अग्र))
		प्रत्यागम -1;
	p1 = stash->parents->item;
	p2 = stash->parents->अग्र->item;
	यदि (stash->parents->अग्र->अग्र)
		p3 = stash->parents->अग्र->अग्र->item;
	यदि (repo_parse_commit(r, p1) || repo_parse_commit(r, p2) ||
			(p3 && repo_parse_commit(r, p3)))
		प्रत्यागम -1;
	/* p2 must have a single parent, p3 must have no parents */
	यदि (!p2->parents || p2->parents->अग्र || (p3 && p3->parents))
		प्रत्यागम -1;
	यदि (repo_parse_commit(r, p2->parents->item))
		प्रत्यागम -1;
	/* p2^1 must equal p1 */
	यदि (!oideq(&p1->object.oid, &p2->parents->item->object.oid))
		प्रत्यागम -1;

	प्रत्यागम 0;
}

स्थिर शून्य assert_stash_like(संरचना stash_info *info, ध्रुव वर्ण *revision)
{
	यदि (get_oidf(&info->b_commit, "%s^1", revision) ||
	    get_oidf(&info->w_वृक्षः, "%s:", revision) ||
	    get_oidf(&info->b_वृक्षः, "%s^1:", revision) ||
	    get_oidf(&info->i_वृक्षः, "%s^2:", revision))
		die(_("'%s' is not a stash-like commit"), revision);
}

स्थिर पूर्णाङ्क parse_stash_revision(संरचना strbuf *revision, ध्रुव वर्ण *commit, पूर्णाङ्क quiet)
{
	strbuf_reset(revision);
	यदि (!commit) {
		यदि (!refs_ref_exists(get_main_ref_store(the_repository), ref_stash)) {
			यदि (!quiet)
				fprintf_ln(stderr, _("No stash entries found."));
			प्रत्यागम -1;
		}

		strbuf_addf(revision, "%s@{0}", ref_stash);
	} अन्यथा यदि (strspn(commit, "0123456789") == वाक्यदैर्घ्यम्(commit)) {
		strbuf_addf(revision, "%s@{%s}", ref_stash, commit);
	} अन्यथा {
		strbuf_addstr(revision, commit);
	}
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क get_stash_info(संरचना stash_info *info, पूर्णाङ्क argc, ध्रुव वर्ण **argv)
{
	पूर्णाङ्क ret;
	वर्ण *अन्तः_सम्बन्धि_rev;
	वर्ण *expanded_ref;
	ध्रुव वर्ण *revision;
	ध्रुव वर्ण *commit = रिक्त;
	संरचना object_id dummy;
	संरचना strbuf symbolic = STRBUF_INIT;

	यदि (argc > 1) {
		पूर्णाङ्क i;
		संरचना strbuf refs_सन्देशः = STRBUF_INIT;

		प्रति (i = 0; i < argc; i++)
			strbuf_addf(&refs_सन्देशः, " '%s'", argv[i]);

		fprintf_ln(stderr, _("Too many revisions specified:%s"),
			   refs_सन्देशः.buf);
		strbuf_release(&refs_सन्देशः);

		प्रत्यागम -1;
	}

	यदि (argc == 1)
		commit = argv[0];

	strbuf_init(&info->revision, 0);
	यदि (parse_stash_revision(&info->revision, commit, 0)) {
		प्रत्यागम -1;
	}

	revision = info->revision.buf;

	यदि (repo_get_oid(the_repository, revision, &info->w_commit))
		प्रत्यागम error(_("%s is not a valid reference"), revision);

	assert_stash_like(info, revision);

	info->has_u = !get_oidf(&info->u_वृक्षः, "%s^3:", revision);

	अन्तः_सम्बन्धि_rev = strchrnul(revision, '@');
	strbuf_सङ्कलय(&symbolic, revision, अन्तः_सम्बन्धि_rev - revision);

	ret = repo_dwim_ref(the_repository, symbolic.buf, symbolic.len,
			    &dummy, &expanded_ref, 0);
	strbuf_release(&symbolic);
	विकल्प (ret) {
	पक्ष 0: /* Not found, but valid ref */
		info->is_stash_ref = 0;
		भङ्ग;
	पक्ष 1:
		info->is_stash_ref = !वाक्यं_तुलय(expanded_ref, ref_stash);
		भङ्ग;
	स्वभाव: /* Invalid or ambiguous */
		भङ्ग;
	}

	मुक्त(expanded_ref);
	प्रत्यागम !(ret == 0 || ret == 1);
}

स्थिर पूर्णाङ्क do_clear_stash(शून्य)
{
	संरचना object_id obj;
	यदि (repo_get_oid(the_repository, ref_stash, &obj))
		प्रत्यागम 0;

	प्रत्यागम refs_delete_ref(get_main_ref_store(the_repository), रिक्त,
			       ref_stash, &obj, 0);
}

स्थिर पूर्णाङ्क clear_stash(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
		       संरचना repository *repo अप्रयुक्तम्)
{
	संरचना option options[] = {
		OPT_अन्तः()
	};

	argc = parse_options(argc, argv, उपसर्गः, options,
			     git_stash_clear_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	यदि (argc)
		प्रत्यागम error(_("git stash clear with arguments is "
			       "unimplemented"));

	प्रत्यागम do_clear_stash();
}

स्थिर पूर्णाङ्क reset_वृक्षः(संरचना object_id *i_वृक्षः, पूर्णाङ्क update, पूर्णाङ्क reset)
{
	पूर्णाङ्क nr_trees = 1;
	संरचना unpack_trees_options opts;
	संरचना वृक्षः_desc t[MAX_UNPACK_TREES];
	संरचना वृक्षः *वृक्षः;
	संरचना तालकम्_file तालकम्_file = तालकम्_INIT;

	repo_read_सूचकांक_preload(the_repository, रिक्त, 0);
	यदि (refresh_सूचकांक(the_repository->सूचकांक, REFRESH_QUIET, रिक्त, रिक्त, रिक्त))
		प्रत्यागम -1;

	repo_hold_locked_सूचकांक(the_repository, &तालकम्_file, तालकम्_DIE_ON_ERROR);

	स्मृतिं_पूरय(&opts, 0, परिमाण(opts));

	वृक्षः = repo_parse_वृक्षः_परोक्षम्(the_repository, i_वृक्षः);
	यदि (repo_parse_वृक्षः(the_repository, वृक्षः))
		प्रत्यागम -1;

	init_वृक्षः_desc(t, &वृक्षः->object.oid, वृक्षः->बफर, वृक्षः->आकार);

	opts.शीर्ष_idx = 1;
	opts.src_सूचकांक = the_repository->सूचकांक;
	opts.dst_सूचकांक = the_repository->सूचकांक;
	opts.merge = 1;
	opts.reset = reset ? UNPACK_RESET_PROTECT_UNTRACKED : 0;
	opts.update = update;
	यदि (update)
		opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
	opts.fn = oneway_merge;

	यदि (unpack_trees(nr_trees, t, &opts))
		प्रत्यागम -1;

	यदि (write_locked_सूचकांक(the_repository->सूचकांक, &तालकम्_file, COMMIT_तालकम्))
		प्रत्यागम error(_("unable to write new index file"));

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क create_सूचकांक_from_वृक्षः(ध्रुव संरचना object_id *वृक्षः_id,
				  ध्रुव वर्ण *सूचकांक_मार्गः)
{
	पूर्णाङ्क nr_trees = 1;
	पूर्णाङ्क ret = 0;
	संरचना unpack_trees_options opts;
	संरचना वृक्षः_desc t[MAX_UNPACK_TREES];
	संरचना वृक्षः *वृक्षः;
	संरचना सूचकांक_state dst_istate = सूचकांक_STATE_INIT(the_repository);
	संरचना तालकम्_file तालकम्_file = तालकम्_INIT;

	repo_read_सूचकांक_preload(the_repository, रिक्त, 0);
	refresh_सूचकांक(the_repository->सूचकांक, REFRESH_QUIET, रिक्त, रिक्त, रिक्त);

	hold_तालकम्_file_for_update(&तालकम्_file, सूचकांक_मार्गः, तालकम्_DIE_ON_ERROR);

	स्मृतिं_पूरय(&opts, 0, परिमाण(opts));

	वृक्षः = repo_parse_वृक्षः_परोक्षम्(the_repository, वृक्षः_id);
	यदि (!वृक्षः || repo_parse_वृक्षः(the_repository, वृक्षः)) {
		ret = -1;
		गच्छ समाप्तम्;
	}

	init_वृक्षः_desc(t, &वृक्षः->object.oid, वृक्षः->बफर, वृक्षः->आकार);

	opts.शीर्ष_idx = 1;
	opts.src_सूचकांक = the_repository->सूचकांक;
	opts.dst_सूचकांक = &dst_istate;
	opts.merge = 1;
	opts.reset = UNPACK_RESET_PROTECT_UNTRACKED;
	opts.fn = oneway_merge;

	यदि (unpack_trees(nr_trees, t, &opts)) {
		ret = -1;
		गच्छ समाप्तम्;
	}

	यदि (write_locked_सूचकांक(&dst_istate, &तालकम्_file, COMMIT_तालकम्)) {
		ret = error(_("unable to write new index file"));
		गच्छ समाप्तम्;
	}

समाप्तम्:
	release_सूचकांक(&dst_istate);
	यदि (ret)
		rollback_तालकम्_file(&तालकम्_file);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क is_मार्गः_a_directory(ध्रुव वर्ण *मार्गः)
{
	/*
	 * This function differs from abspath.c:is_directory() in that
	 * here we use lstat() instead of stat(); we do not want to
	 * follow symbolic links here.
	 */
	संरचना stat st;
	प्रत्यागम (!lstat(मार्गः, &st) && S_ISDIR(st.st_बहुलकम्));
}

स्थिर शून्य सङ्कलय_diff_गन्तव्यम्_buf(संरचना diff_queue_struct *q,
			    संरचना diff_options *options अप्रयुक्तम्,
			    शून्य *दत्त)
{
	पूर्णाङ्क i;

	प्रति (i = 0; i < q->nr; i++) {
		यदि (is_मार्गः_a_directory(q->queue[i]->one->मार्गः))
			अनुवर्त;

		strbuf_addstr(दत्त, q->queue[i]->one->मार्गः);

		/* NUL-terminate: will be fed to update-index -z */
		strbuf_addch(दत्त, '\0');
	}
}

स्थिर पूर्णाङ्क restore_untracked(संरचना object_id *u_वृक्षः)
{
	पूर्णाङ्क res;
	संरचना child_process cp = CHILD_PROCESS_INIT;

	/*
	 * We need to run restore files from a given index, but without
	 * affecting the current index, so we use GIT_INDEX_FILE with
	 * run_command to fork processes that will not interfere.
	 */
	cp.git_cmd = 1;
	strvec_प्रक्षिप(&cp.तर्काः, "read-tree");
	strvec_प्रक्षिप(&cp.तर्काः, oid_गन्तव्यम्_hex(u_वृक्षः));
	strvec_pushf(&cp.env, "GIT_INDEX_FILE=%s",
		     stash_सूचकांक_मार्गः.buf);
	यदि (सततम्_command(&cp)) {
		remove_मार्गः(stash_सूचकांक_मार्गः.buf);
		प्रत्यागम -1;
	}

	child_process_init(&cp);
	cp.git_cmd = 1;
	strvec_pushl(&cp.तर्काः, "checkout-index", "--all", रिक्त);
	strvec_pushf(&cp.env, "GIT_INDEX_FILE=%s",
		     stash_सूचकांक_मार्गः.buf);

	res = सततम्_command(&cp);
	remove_मार्गः(stash_सूचकांक_मार्गः.buf);
	प्रत्यागम res;
}

स्थिर शून्य unstage_changes_unless_new(संरचना object_id *orig_वृक्षः)
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
	संरचना तालकम्_file तालकम् = तालकम्_INIT;
	पूर्णाङ्क i;

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
	repo_diff_सज्जता(the_repository, &diff_opts);
	diff_opts.flags.recursive = 1;
	diff_opts.detect_rename = 0;
	diff_opts.output_format = DIFF_FORMAT_NO_OUTPUT;
	diff_सज्जता_समाप्तम्(&diff_opts);

	do_diff_cache(orig_वृक्षः, &diff_opts);
	diffcore_std(&diff_opts);

	/* Iterate over the paths that changed due to the merge... */
	प्रति (i = 0; i < diff_queued_diff.nr; i++) {
		संरचना diff_filepair *p;
		संरचना cache_प्रविष्टिः *ce;
		पूर्णाङ्क pos;

		/* Look up the path's position in the current index. */
		p = diff_queued_diff.queue[i];
		pos = सूचकांक_name_pos(the_repository->सूचकांक, p->two->मार्गः,
				     वाक्यदैर्घ्यम्(p->two->मार्गः));

		/*
		 * Step 2: Place changes in the working tree
		 *
		 * Stash is about restoring changes *to the working tree*.
		 * So if the merge successfully got a new version of some
		 * path, but left it out of the working tree, then clear the
		 * SKIP_WORKTREE bit and write it to the working tree.
		 */
		यदि (pos >= 0 && ce_skip_worktree(the_repository->सूचकांक->cache[pos])) {
			संरचना stat st;

			ce = the_repository->सूचकांक->cache[pos];
			यदि (!lstat(ce->name, &st)) {
				/* Conflicting path present; relocate it */
				संरचना strbuf new_मार्गः = STRBUF_INIT;
				पूर्णाङ्क fd;

				strbuf_addf(&new_मार्गः,
					    "%s.stash.XXXXXX", ce->name);
				fd = xmkstemp(new_मार्गः.buf);
				close(fd);
				प्रारूप_लिख(_("WARNING: Untracked file in way of "
					 "tracked file!  Renaming\n "
					 "           %s -> %s\n"
					 "         to make room.\n"),
				       ce->name, new_मार्गः.buf);
				यदि (rename(ce->name, new_मार्गः.buf))
					die("Failed to move %s to %s",
					    ce->name, new_मार्गः.buf);
				strbuf_release(&new_मार्गः);
			}
			checkout_प्रविष्टिः(ce, &state, रिक्त, रिक्त);
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
			पूर्णाङ्क option = सङ्कलय_CACHE_OK_गन्तव्यम्_REPLACE;
			यदि (pos < 0)
				option = सङ्कलय_CACHE_OK_गन्तव्यम्_सङ्कलय;

			ce = make_cache_प्रविष्टिः(the_repository->सूचकांक,
					      p->one->बहुलकम्,
					      &p->one->oid,
					      p->one->मार्गः,
					      0, 0);
			सङ्कलय_सूचकांक_प्रविष्टिः(the_repository->सूचकांक, ce, option);
		}
	}
	diff_flush(&diff_opts);

	/*
	 * Step 4: write the new index to disk
	 */
	repo_hold_locked_सूचकांक(the_repository, &तालकम्, तालकम्_DIE_ON_ERROR);
	यदि (write_locked_सूचकांक(the_repository->सूचकांक, &तालकम्,
			       COMMIT_तालकम् | SKIP_IF_UNCHANGED))
		die(_("could not write index"));
}

स्थिर गणना stash_apply_परिणाम do_apply_stash(ध्रुव वर्ण *उपसर्गः,
					      संरचना stash_info *info,
					      पूर्णाङ्क सूचकांक, पूर्णाङ्क quiet,
					      ध्रुव वर्ण *label_ours,
					      ध्रुव वर्ण *label_theirs,
					      ध्रुव वर्ण *label_आधारः)
{
	पूर्णाङ्क clean, ret;
	पूर्णाङ्क has_सूचकांक = सूचकांक;
	संरचना merge_options o;
	संरचना object_id c_वृक्षः;
	संरचना object_id सूचकांक_वृक्षः;
	संरचना वृक्षः *शीर्ष, *merge, *merge_आधारः;
	संरचना तालकम्_file तालकम् = तालकम्_INIT;

	repo_read_सूचकांक_preload(the_repository, रिक्त, 0);
	यदि (repo_refresh_and_write_सूचकांक(the_repository, REFRESH_QUIET, 0, 0,
					 रिक्त, रिक्त, रिक्त))
		प्रत्यागम error(_("could not write index"));

	यदि (write_सूचकांक_as_वृक्षः(&c_वृक्षः, the_repository->सूचकांक,
				repo_get_सूचकांक_file(the_repository), 0, रिक्त))
		प्रत्यागम error(_("cannot apply a stash in the middle of a merge"));

	init_ui_merge_options(&o, the_repository);

	यदि (quiet)
		o.verbosity = 0;

	यदि (सूचकांक) {
		यदि (oideq(&info->b_वृक्षः, &info->i_वृक्षः) ||
		    oideq(&c_वृक्षः, &info->i_वृक्षः)) {
			has_सूचकांक = 0;
		} अन्यथा {
			संरचना merge_परिणाम परिणाम = { 0 };

			o.branch1 = "Current index";
			o.branch2 = "Stashed index changes";
			o.ancestor = "Stash base";

			शीर्ष = lookup_वृक्षः(o.repo, &c_वृक्षः);
			merge = lookup_वृक्षः(o.repo, &info->i_वृक्षः);
			merge_आधारः = lookup_वृक्षः(o.repo, &info->b_वृक्षः);

			merge_incore_nonrecursive(&o, merge_आधारः, शीर्ष, merge,
						  &परिणाम);

			यदि (परिणाम.clean < 0) {
				merge_finalize(&o, &परिणाम);
				प्रत्यागम error(_("index merge failed"));
			} अन्यथा यदि (!परिणाम.clean) {
				merge_finalize(&o, &परिणाम);
				प्रत्यागम error(_("conflicts in index. "
					       "Try without --index."));
			} अन्यथा {
				oidcpy(&सूचकांक_वृक्षः, &परिणाम.वृक्षः->object.oid);
				merge_finalize(&o, &परिणाम);
			}
		}
	}

	o.branch1 = label_ours ? label_ours : "Updated upstream";
	o.branch2 = label_theirs ? label_theirs : "Stashed changes";
	o.ancestor = label_आधारः ? label_आधारः : "Stash base";

	यदि (oideq(&info->b_वृक्षः, &c_वृक्षः))
		o.branch1 = "Version stash was based on";

	यदि (o.verbosity >= 3)
		printf_ln(_("Merging %s with %s"), o.branch1, o.branch2);

	शीर्ष = lookup_वृक्षः(o.repo, &c_वृक्षः);
	merge = lookup_वृक्षः(o.repo, &info->w_वृक्षः);
	merge_आधारः = lookup_वृक्षः(o.repo, &info->b_वृक्षः);

	repo_hold_locked_सूचकांक(o.repo, &तालकम्, तालकम्_DIE_ON_ERROR);
	clean = merge_ort_nonrecursive(&o, शीर्ष, merge, merge_आधारः);

	/*
	 * If 'clean' >= 0, reverse the value for 'ret' so 'ret' is 0 when the
	 * merge was clean, and 1 if the merge was unclean or a negative value
	 * if it encountered an error.
	 */
	ret = clean >= 0 ? !clean : clean;

	यदि (ret < 0)
		rollback_तालकम्_file(&तालकम्);
	अन्यथा यदि (write_locked_सूचकांक(o.repo->सूचकांक, &तालकम्,
				      COMMIT_तालकम् | SKIP_IF_UNCHANGED))
		ret = error(_("could not write index"));

	यदि (ret) {
		repo_rerere(the_repository, 0);

		यदि (सूचकांक)
			fprintf_ln(stderr, _("Index was not unstashed."));

		गच्छ restore_untracked;
	}

	यदि (has_सूचकांक) {
		यदि (reset_वृक्षः(&सूचकांक_वृक्षः, 0, 0))
			ret = -1;
	} अन्यथा {
		unstage_changes_unless_new(&c_वृक्षः);
	}

restore_untracked:
	यदि (info->has_u && restore_untracked(&info->u_वृक्षः))
		ret = error(_("could not restore untracked files from stash"));

	यदि (!quiet) {
		संरचना child_process cp = CHILD_PROCESS_INIT;

		/*
		 * Status is quite simple and could be replaced with calls to
		 * wt_status in the future, but it adds complexities which may
		 * require more tests.
		 */
		cp.git_cmd = 1;
		cp.dir = उपसर्गः;
		strvec_pushf(&cp.env, GIT_WORK_वृक्षः_ENVIRONMENT"=%s",
			     absolute_मार्गः(repo_get_work_वृक्षः(the_repository)));
		strvec_pushf(&cp.env, GIT_DIR_ENVIRONMENT"=%s",
			     absolute_मार्गः(repo_get_git_dir(the_repository)));
		strvec_प्रक्षिप(&cp.तर्काः, "status");
		सततम्_command(&cp);
	}

	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क apply_stash(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
		       संरचना repository *repo अप्रयुक्तम्)
{
	पूर्णाङ्क ret = -1;
	पूर्णाङ्क quiet = 0;
	पूर्णाङ्क सूचकांक = use_सूचकांक;
	ध्रुव वर्ण *label_ours = रिक्त, *label_theirs = रिक्त, *label_आधारः = रिक्त;
	संरचना stash_info info = STASH_INFO_INIT;
	संरचना option options[] = {
		OPT__QUIET(&quiet, N_("be quiet, only report errors")),
		OPT_BOOL(0, "index", &सूचकांक,
			 N_("attempt to recreate the index")),
		OPT_STRING(0, "label-ours", &label_ours, N_("label"),
			   N_("label for the upstream side in conflict markers")),
		OPT_STRING(0, "label-theirs", &label_theirs, N_("label"),
			   N_("label for the stashed side in conflict markers")),
		OPT_STRING(0, "label-base", &label_आधारः, N_("label"),
			   N_("label for the base in diff3 conflict markers")),
		OPT_अन्तः()
	};

	argc = parse_options(argc, argv, उपसर्गः, options,
			     git_stash_apply_usage, 0);

	यदि (get_stash_info(&info, argc, argv))
		गच्छ cleanup;

	ret = do_apply_stash(उपसर्गः, &info, सूचकांक, quiet,
			     label_ours, label_theirs, label_आधारः);
cleanup:
	free_stash_info(&info);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क reject_reflog_ent(ध्रुव वर्ण *refname अप्रयुक्तम्,
			     संरचना object_id *ooid अप्रयुक्तम्,
			     संरचना object_id *noid अप्रयुक्तम्,
			     ध्रुव वर्ण *email अप्रयुक्तम्,
			     timestamp_t timestamp अप्रयुक्तम्,
			     पूर्णाङ्क tz अप्रयुक्तम्, ध्रुव वर्ण *सन्देशः_2 अप्रयुक्तम्,
			     शून्य *cb_दत्त अप्रयुक्तम्)
{
	प्रत्यागम 1;
}

स्थिर पूर्णाङ्क reflog_is_रिक्तानि(ध्रुव वर्ण *refname)
{
	प्रत्यागम !refs_for_each_reflog_ent(get_main_ref_store(the_repository),
					 refname, reject_reflog_ent, रिक्त);
}

स्थिर पूर्णाङ्क do_drop_stash(संरचना stash_info *info, पूर्णाङ्क quiet)
{
	यदि (!reflog_delete(info->revision.buf,
			   EXPIRE_REFLOGS_REWRITE | EXPIRE_REFLOGS_UPDATE_REF,
			   0)) {
		यदि (!quiet)
			printf_ln(_("Dropped %s (%s)"), info->revision.buf,
				  oid_गन्तव्यम्_hex(&info->w_commit));
	} अन्यथा {
		प्रत्यागम error(_("%s: Could not drop stash entry"),
			     info->revision.buf);
	}

	यदि (reflog_is_रिक्तानि(ref_stash))
		do_clear_stash();

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क get_stash_info_assert(संरचना stash_info *info, पूर्णाङ्क argc,
				 ध्रुव वर्ण **argv)
{
	पूर्णाङ्क ret = get_stash_info(info, argc, argv);

	यदि (ret < 0)
		प्रत्यागम ret;

	यदि (!info->is_stash_ref)
		प्रत्यागम error(_("'%s' is not a stash reference"), info->revision.buf);

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क drop_stash(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
		      संरचना repository *repo अप्रयुक्तम्)
{
	पूर्णाङ्क ret = -1;
	पूर्णाङ्क quiet = 0;
	संरचना stash_info info = STASH_INFO_INIT;
	संरचना option options[] = {
		OPT__QUIET(&quiet, N_("be quiet, only report errors")),
		OPT_अन्तः()
	};

	argc = parse_options(argc, argv, उपसर्गः, options,
			     git_stash_drop_usage, 0);

	यदि (get_stash_info_assert(&info, argc, argv))
		गच्छ cleanup;

	ret = do_drop_stash(&info, quiet);
cleanup:
	free_stash_info(&info);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क निष्कासय_stash(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
		     संरचना repository *repo अप्रयुक्तम्)
{
	पूर्णाङ्क ret = -1;
	पूर्णाङ्क सूचकांक = use_सूचकांक;
	पूर्णाङ्क quiet = 0;
	संरचना stash_info info = STASH_INFO_INIT;
	संरचना option options[] = {
		OPT__QUIET(&quiet, N_("be quiet, only report errors")),
		OPT_BOOL(0, "index", &सूचकांक,
			 N_("attempt to recreate the index")),
		OPT_अन्तः()
	};

	argc = parse_options(argc, argv, उपसर्गः, options,
			     git_stash_निष्कासय_usage, 0);

	यदि (get_stash_info_assert(&info, argc, argv))
		गच्छ cleanup;

	यदि ((ret = do_apply_stash(उपसर्गः, &info, सूचकांक, quiet,
				  रिक्त, रिक्त, रिक्त)))
		printf_ln(_("The stash entry is kept in case "
			    "you need it again."));
	अन्यथा
		ret = do_drop_stash(&info, quiet);

cleanup:
	free_stash_info(&info);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क branch_stash(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
			संरचना repository *repo अप्रयुक्तम्)
{
	पूर्णाङ्क ret = -1;
	ध्रुव वर्ण *branch = रिक्त;
	संरचना stash_info info = STASH_INFO_INIT;
	संरचना child_process cp = CHILD_PROCESS_INIT;
	संरचना option options[] = {
		OPT_अन्तः()
	};

	argc = parse_options(argc, argv, उपसर्गः, options,
			     git_stash_branch_usage, 0);

	यदि (!argc) {
		fprintf_ln(stderr, _("No branch name specified"));
		प्रत्यागम -1;
	}

	branch = argv[0];

	यदि (get_stash_info(&info, argc - 1, argv + 1))
		गच्छ cleanup;

	cp.git_cmd = 1;
	strvec_pushl(&cp.तर्काः, "checkout", "-b", रिक्त);
	strvec_प्रक्षिप(&cp.तर्काः, branch);
	strvec_प्रक्षिप(&cp.तर्काः, oid_गन्तव्यम्_hex(&info.b_commit));
	ret = सततम्_command(&cp);
	यदि (!ret)
		ret = do_apply_stash(उपसर्गः, &info, 1, 0,
				     रिक्त, रिक्त, रिक्त);
	यदि (!ret && info.is_stash_ref)
		ret = do_drop_stash(&info, 0);

cleanup:
	free_stash_info(&info);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क list_stash(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
		      संरचना repository *repo अप्रयुक्तम्)
{
	संरचना child_process cp = CHILD_PROCESS_INIT;
	संरचना option options[] = {
		OPT_अन्तः()
	};

	argc = parse_options(argc, argv, उपसर्गः, options,
			     git_stash_list_usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT);

	यदि (!refs_ref_exists(get_main_ref_store(the_repository), ref_stash))
		प्रत्यागम 0;

	cp.git_cmd = 1;
	strvec_pushl(&cp.तर्काः, "log", "--format=%gd: %gs", "-g",
		     "--first-parent", रिक्त);
	strvec_pushv(&cp.तर्काः, argv);
	strvec_प्रक्षिप(&cp.तर्काः, ref_stash);
	strvec_प्रक्षिप(&cp.तर्काः, "--");
	प्रत्यागम सततम्_command(&cp);
}

स्थिर पूर्णाङ्क git_stash_config(ध्रुव वर्ण *var, ध्रुव वर्ण *मानम्,
			    ध्रुव संरचना config_context *ctx, शून्य *cb)
{
	यदि (!वाक्यं_तुलय(var, "stash.showstat")) {
		show_stat = git_config_bool(var, मानम्);
		प्रत्यागम 0;
	}
	यदि (!वाक्यं_तुलय(var, "stash.showpatch")) {
		show_patch = git_config_bool(var, मानम्);
		प्रत्यागम 0;
	}
	यदि (!वाक्यं_तुलय(var, "stash.showincludeuntracked")) {
		show_include_untracked = git_config_bool(var, मानम्);
		प्रत्यागम 0;
	}
	यदि (!वाक्यं_तुलय(var, "stash.index")) {
		use_सूचकांक = git_config_bool(var, मानम्);
		प्रत्यागम 0;
	}
	प्रत्यागम git_diff_basic_config(var, मानम्, ctx, cb);
}

स्थिर शून्य diff_include_untracked(ध्रुव संरचना stash_info *info, संरचना diff_options *diff_opt)
{
	ध्रुव संरचना object_id *oid[] = { &info->w_commit, &info->u_वृक्षः };
	संरचना वृक्षः *वृक्षः[ARRAY_आकार(oid)];
	संरचना वृक्षः_desc वृक्षः_desc[ARRAY_आकार(oid)];
	संरचना unpack_trees_options unpack_वृक्षः_opt = { 0 };

	प्रति (आकार_t i = 0; i < ARRAY_आकार(oid); i++) {
		वृक्षः[i] = repo_parse_वृक्षः_परोक्षम्(the_repository, oid[i]);
		यदि (repo_parse_वृक्षः(the_repository, वृक्षः[i]) < 0)
			die(_("failed to parse tree"));
		init_वृक्षः_desc(&वृक्षः_desc[i], &वृक्षः[i]->object.oid,
			       वृक्षः[i]->बफर, वृक्षः[i]->आकार);
	}

	unpack_वृक्षः_opt.शीर्ष_idx = -1;
	unpack_वृक्षः_opt.src_सूचकांक = the_repository->सूचकांक;
	unpack_वृक्षः_opt.dst_सूचकांक = the_repository->सूचकांक;
	unpack_वृक्षः_opt.merge = 1;
	unpack_वृक्षः_opt.fn = stash_worktree_untracked_merge;

	यदि (unpack_trees(ARRAY_आकार(वृक्षः_desc), वृक्षः_desc, &unpack_वृक्षः_opt))
		die(_("failed to unpack trees"));

	do_diff_cache(&info->b_commit, diff_opt);
}

स्थिर पूर्णाङ्क show_stash(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
		      संरचना repository *repo अप्रयुक्तम्)
{
	पूर्णाङ्क i;
	पूर्णाङ्क ret = -1;
	संरचना stash_info info = STASH_INFO_INIT;
	संरचना rev_info rev;
	संरचना strvec stash_तर्काः = STRVEC_INIT;
	संरचना strvec revision_तर्काः = STRVEC_INIT;
	गणना {
		UNTRACKED_NONE,
		UNTRACKED_INCLUDE,
		UNTRACKED_ONLY
	} show_untracked = show_include_untracked ? UNTRACKED_INCLUDE : UNTRACKED_NONE;
	संरचना option options[] = {
		OPT_समुच्चयः_INT('u', "include-untracked", &show_untracked,
			    N_("include untracked files in the stash"),
			    UNTRACKED_INCLUDE),
		OPT_समुच्चयः_INT_F(0, "only-untracked", &show_untracked,
			      N_("only show untracked files in the stash"),
			      UNTRACKED_ONLY, PARSE_OPT_NONEG),
		OPT_अन्तः()
	};
	पूर्णाङ्क do_usage = 0;

	init_diff_ui_defaults();
	repo_config(the_repository, git_diff_ui_config, रिक्त);
	repo_init_revisions(the_repository, &rev, उपसर्गः);

	argc = parse_options(argc, argv, उपसर्गः, options, git_stash_show_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	strvec_प्रक्षिप(&revision_तर्काः, argv[0]);
	प्रति (i = 1; i < argc; i++) {
		यदि (argv[i][0] != '-')
			strvec_प्रक्षिप(&stash_तर्काः, argv[i]);
		अन्यथा
			strvec_प्रक्षिप(&revision_तर्काः, argv[i]);
	}

	यदि (get_stash_info(&info, stash_तर्काः.nr, stash_तर्काः.v))
		गच्छ cleanup;

	/*
	 * The config settings are applied only if there are not passed
	 * any options.
	 */
	यदि (revision_तर्काः.nr == 1) {
		यदि (show_stat)
			rev.diffopt.output_format = DIFF_FORMAT_DIFFSTAT;

		यदि (show_patch)
			rev.diffopt.output_format |= DIFF_FORMAT_PATCH;

		यदि (!show_stat && !show_patch) {
			ret = 0;
			गच्छ cleanup;
		}
	}

	सज्जता_revisions_from_strvec(&revision_तर्काः, &rev, रिक्त);
	यदि (revision_तर्काः.nr > 1)
		गच्छ usage;
	यदि (!rev.diffopt.output_format) {
		rev.diffopt.output_format = DIFF_FORMAT_PATCH;
		diff_सज्जता_समाप्तम्(&rev.diffopt);
	}

	rev.diffopt.flags.recursive = 1;
	सज्जता_diff_pager(&rev.diffopt);
	विकल्प (show_untracked) {
	पक्ष UNTRACKED_NONE:
		diff_वृक्षः_oid(&info.b_commit, &info.w_commit, "", &rev.diffopt);
		भङ्ग;
	पक्ष UNTRACKED_ONLY:
		यदि (info.has_u)
			diff_मूलम्_वृक्षः_oid(&info.u_वृक्षः, "", &rev.diffopt);
		भङ्ग;
	पक्ष UNTRACKED_INCLUDE:
		यदि (info.has_u)
			diff_include_untracked(&info, &rev.diffopt);
		अन्यथा
			diff_वृक्षः_oid(&info.b_commit, &info.w_commit, "", &rev.diffopt);
		भङ्ग;
	}
	log_वृक्षः_diff_flush(&rev);

	ret = diff_परिणाम_संहिता(&rev);

cleanup:
	strvec_clear(&revision_तर्काः);
	strvec_clear(&stash_तर्काः);
	free_stash_info(&info);
	release_revisions(&rev);
	यदि (do_usage)
		usage_with_options(git_stash_show_usage, options);
	प्रत्यागम ret;
usage:
	do_usage = 1;
	गच्छ cleanup;
}

स्थिर पूर्णाङ्क do_store_stash(ध्रुव संरचना object_id *w_commit, ध्रुव वर्ण *stash_सन्देशः,
			  पूर्णाङ्क quiet)
{
	संरचना stash_info info;
	वर्ण revision[GIT_MAX_HEXSZ];

	oid_गन्तव्यम्_hex_r(revision, w_commit);
	assert_stash_like(&info, revision);

	यदि (!stash_सन्देशः)
		stash_सन्देशः = "Created via \"git stash store\".";

	यदि (refs_update_ref(get_main_ref_store(the_repository), stash_सन्देशः, ref_stash, w_commit, रिक्त,
			    REF_FORCE_CREATE_REFLOG,
			    quiet ? UPDATE_REFS_QUIET_ON_ERR :
			    UPDATE_REFS_सन्देशः_ON_ERR)) {
		यदि (!quiet) {
			fprintf_ln(stderr, _("Cannot update %s with %s"),
				   ref_stash, oid_गन्तव्यम्_hex(w_commit));
		}
		प्रत्यागम -1;
	}

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क store_stash(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
		       संरचना repository *repo अप्रयुक्तम्)
{
	पूर्णाङ्क quiet = 0;
	ध्रुव वर्ण *stash_सन्देशः = रिक्त;
	संरचना object_id obj;
	संरचना option options[] = {
		OPT__QUIET(&quiet, N_("be quiet")),
		OPT_STRING('m', "message", &stash_सन्देशः, "message",
			   N_("stash message")),
		OPT_अन्तः()
	};
	पूर्णाङ्क ret;

	argc = parse_options(argc, argv, उपसर्गः, options,
			     git_stash_store_usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT);

	यदि (argc != 1) {
		यदि (!quiet)
			fprintf_ln(stderr, _("\"git stash store\" requires one "
					     "<commit> argument"));
		प्रत्यागम -1;
	}

	यदि (repo_get_oid_with_flags(the_repository, argv[0], &obj,
				    quiet ? GET_OID_QUIETLY : 0)) {
		यदि (!quiet)
			fprintf_ln(stderr, _("Cannot update %s with %s"),
					     ref_stash, argv[0]);
		ret = -1;
		गच्छ बहिर्गमनम्;
	}

	ret = do_store_stash(&obj, stash_सन्देशः, quiet);

बहिर्गमनम्:
	प्रत्यागम ret;
}

स्थिर शून्य सङ्कलय_pathspecs(संरचना strvec *तर्काः,
			  ध्रुव संरचना pathspec *ps) {
	पूर्णाङ्क i;

	प्रति (i = 0; i < ps->nr; i++)
		strvec_प्रक्षिप(तर्काः, ps->वस्तूनि[i].original);
}

/*
 * `untracked_files` will be filled with the names of untracked files.
 * The return value is:
 *
 * = 0 if there are not any untracked files
 * > 0 if there are untracked files
 */
स्थिर पूर्णाङ्क get_untracked_files(ध्रुव संरचना pathspec *ps, पूर्णाङ्क include_untracked,
			       संरचना strbuf *untracked_files)
{
	पूर्णाङ्क i;
	पूर्णाङ्क प्राप्तम्_2 = 0;
	संरचना dir_struct dir = DIR_INIT;

	यदि (include_untracked != INCLUDE_ALL_FILES)
		सज्जता_standard_excludes(&dir);

	fill_directory(&dir, the_repository->सूचकांक, ps);
	प्रति (i = 0; i < dir.nr; i++) {
		संरचना dir_प्रविष्टिः *ent = dir.entries[i];
		प्राप्तम्_2++;
		strbuf_addstr(untracked_files, ent->name);
		/* NUL-terminate: will be fed to update-index -z */
		strbuf_addch(untracked_files, '\0');
	}

	dir_clear(&dir);
	प्रत्यागम प्राप्तम्_2;
}

/*
 * The return value of `check_changes_tracked_files()` can be:
 *
 * < 0 if there was an error
 * = 0 if there are no changes.
 * > 0 if there are changes.
 */
स्थिर पूर्णाङ्क check_changes_tracked_files(ध्रुव संरचना pathspec *ps)
{
	संरचना rev_info rev;
	संरचना object_id dummy;
	पूर्णाङ्क ret = 0;

	/* No initial commit. */
	यदि (repo_get_oid(the_repository, "HEAD", &dummy))
		प्रत्यागम -1;

	यदि (repo_read_सूचकांक(the_repository) < 0)
		प्रत्यागम -1;

	repo_init_revisions(the_repository, &rev, रिक्त);
	copy_pathspec(&rev.prune_दत्त, ps);

	rev.diffopt.flags.शीघ्र = 1;
	rev.diffopt.flags.ignore_submodules = 1;
	rev.abbrev = 0;

	सङ्कलय_शीर्ष_गन्तव्यम्_pending(&rev);
	diff_सज्जता_समाप्तम्(&rev.diffopt);

	सततम्_diff_सूचकांक(&rev, DIFF_सूचकांक_CACHED);
	यदि (diff_परिणाम_संहिता(&rev)) {
		ret = 1;
		गच्छ समाप्तम्;
	}

	सततम्_diff_files(&rev, 0);
	यदि (diff_परिणाम_संहिता(&rev)) {
		ret = 1;
		गच्छ समाप्तम्;
	}

समाप्तम्:
	release_revisions(&rev);
	प्रत्यागम ret;
}

/*
 * The function will fill `untracked_files` with the names of untracked files
 * It will return 1 if there were any changes and 0 if there were not.
 */
स्थिर पूर्णाङ्क check_changes(ध्रुव संरचना pathspec *ps, पूर्णाङ्क include_untracked,
			 संरचना strbuf *untracked_files)
{
	पूर्णाङ्क ret = 0;
	यदि (check_changes_tracked_files(ps))
		ret = 1;

	यदि (include_untracked && get_untracked_files(ps, include_untracked,
						     untracked_files))
		ret = 1;

	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क save_untracked_files(संरचना stash_info *info, संरचना strbuf *सन्देशः,
				संरचना strbuf *files)
{
	पूर्णाङ्क ret = 0;
	संरचना strbuf untracked_सन्देशः = STRBUF_INIT;
	संरचना child_process cp_upd_सूचकांक = CHILD_PROCESS_INIT;
	संरचना सूचकांक_state istate = सूचकांक_STATE_INIT(the_repository);

	cp_upd_सूचकांक.git_cmd = 1;
	strvec_pushl(&cp_upd_सूचकांक.तर्काः, "update-index", "-z", "--add",
		     "--remove", "--stdin", रिक्त);
	strvec_pushf(&cp_upd_सूचकांक.env, "GIT_INDEX_FILE=%s",
			 stash_सूचकांक_मार्गः.buf);

	strbuf_addf(&untracked_सन्देशः, "untracked files on %s\n", सन्देशः->buf);
	यदि (pipe_command(&cp_upd_सूचकांक, files->buf, files->len, रिक्त, 0,
			 रिक्त, 0)) {
		ret = -1;
		गच्छ समाप्तम्;
	}

	यदि (write_सूचकांक_as_वृक्षः(&info->u_वृक्षः, &istate, stash_सूचकांक_मार्गः.buf, 0,
				रिक्त)) {
		ret = -1;
		गच्छ समाप्तम्;
	}

	यदि (commit_वृक्षः(untracked_सन्देशः.buf, untracked_सन्देशः.len,
			&info->u_वृक्षः, रिक्त, &info->u_commit, रिक्त, रिक्त)) {
		ret = -1;
		गच्छ समाप्तम्;
	}

समाप्तम्:
	release_सूचकांक(&istate);
	strbuf_release(&untracked_सन्देशः);
	remove_मार्गः(stash_सूचकांक_मार्गः.buf);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क stash_staged(संरचना stash_info *info, संरचना strbuf *बहिर्गमनम्_patch,
			पूर्णाङ्क quiet)
{
	पूर्णाङ्क ret = 0;
	संरचना child_process cp_diff_वृक्षः = CHILD_PROCESS_INIT;
	संरचना सूचकांक_state istate = सूचकांक_STATE_INIT(the_repository);

	यदि (write_सूचकांक_as_वृक्षः(&info->w_वृक्षः, &istate, the_repository->सूचकांक_file,
				0, रिक्त)) {
		ret = -1;
		गच्छ समाप्तम्;
	}

	cp_diff_वृक्षः.git_cmd = 1;
	strvec_pushl(&cp_diff_वृक्षः.तर्काः, "diff-tree", "-p", "--binary",
		     "--no-color",
		     "-U1", "HEAD", oid_गन्तव्यम्_hex(&info->w_वृक्षः), "--", रिक्त);
	यदि (pipe_command(&cp_diff_वृक्षः, रिक्त, 0, बहिर्गमनम्_patch, 0, रिक्त, 0)) {
		ret = -1;
		गच्छ समाप्तम्;
	}

	यदि (!बहिर्गमनम्_patch->len) {
		यदि (!quiet)
			fprintf_ln(stderr, _("No staged changes"));
		ret = 1;
	}

समाप्तम्:
	release_सूचकांक(&istate);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क stash_patch(संरचना stash_info *info, ध्रुव संरचना pathspec *ps,
		       संरचना strbuf *बहिर्गमनम्_patch, पूर्णाङ्क quiet,
		       संरचना interactive_options *interactive_opts)
{
	पूर्णाङ्क ret = 0;
	संरचना child_process cp_diff_वृक्षः = CHILD_PROCESS_INIT;
	संरचना commit *शीर्ष_commit;
	ध्रुव संरचना object_id *शीर्ष_वृक्षः;
	संरचना सूचकांक_state istate = सूचकांक_STATE_INIT(the_repository);
	वर्ण *old_सूचकांक_env = रिक्त, *old_repo_सूचकांक_file;

	remove_मार्गः(stash_सूचकांक_मार्गः.buf);

	शीर्ष_commit = lookup_commit(the_repository, &info->b_commit);
	यदि (!शीर्ष_commit || repo_parse_commit(the_repository, शीर्ष_commit)) {
		ret = -1;
		गच्छ समाप्तम्;
	}
	शीर्ष_वृक्षः = get_commit_वृक्षः_oid(शीर्ष_commit);
	यदि (!शीर्ष_वृक्षः) {
		ret = -1;
		गच्छ समाप्तम्;
	}

	यदि (create_सूचकांक_from_वृक्षः(शीर्ष_वृक्षः, stash_सूचकांक_मार्गः.buf)) {
		ret = -1;
		गच्छ समाप्तम्;
	}

	/* Find out what the user wants. */
	old_repo_सूचकांक_file = the_repository->सूचकांक_file;
	the_repository->सूचकांक_file = stash_सूचकांक_मार्गः.buf;
	old_सूचकांक_env = xstrdup_or_null(getenv(सूचकांक_ENVIRONMENT));
	setenv(सूचकांक_ENVIRONMENT, the_repository->सूचकांक_file, 1);

	ret = !!सततम्_सङ्कलय_p(the_repository, सङ्कलय_P_STASH, interactive_opts, रिक्त, ps, 0);

	the_repository->सूचकांक_file = old_repo_सूचकांक_file;
	यदि (old_सूचकांक_env && *old_सूचकांक_env)
		setenv(सूचकांक_ENVIRONMENT, old_सूचकांक_env, 1);
	अन्यथा
		unsetenv(सूचकांक_ENVIRONMENT);
	FREE_AND_NULL(old_सूचकांक_env);

	/* State of the working tree. */
	यदि (write_सूचकांक_as_वृक्षः(&info->w_वृक्षः, &istate, stash_सूचकांक_मार्गः.buf, 0,
				रिक्त)) {
		ret = -1;
		गच्छ समाप्तम्;
	}

	cp_diff_वृक्षः.git_cmd = 1;
	strvec_pushl(&cp_diff_वृक्षः.तर्काः, "diff-tree", "-p", "-U1", "HEAD",
		     "--no-color",
		     oid_गन्तव्यम्_hex(&info->w_वृक्षः), "--", रिक्त);
	यदि (pipe_command(&cp_diff_वृक्षः, रिक्त, 0, बहिर्गमनम्_patch, 0, रिक्त, 0)) {
		ret = -1;
		गच्छ समाप्तम्;
	}

	यदि (!बहिर्गमनम्_patch->len) {
		यदि (!quiet)
			fprintf_ln(stderr, _("No changes selected"));
		ret = 1;
	}

समाप्तम्:
	release_सूचकांक(&istate);
	remove_मार्गः(stash_सूचकांक_मार्गः.buf);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क stash_working_वृक्षः(संरचना stash_info *info, ध्रुव संरचना pathspec *ps)
{
	पूर्णाङ्क ret = 0;
	संरचना rev_info rev;
	संरचना child_process cp_upd_सूचकांक = CHILD_PROCESS_INIT;
	संरचना strbuf diff_output = STRBUF_INIT;
	संरचना सूचकांक_state istate = सूचकांक_STATE_INIT(the_repository);

	repo_init_revisions(the_repository, &rev, रिक्त);
	copy_pathspec(&rev.prune_दत्त, ps);

	समुच्चयः_alternate_सूचकांक_output(stash_सूचकांक_मार्गः.buf);
	यदि (reset_वृक्षः(&info->i_वृक्षः, 0, 0)) {
		ret = -1;
		गच्छ समाप्तम्;
	}
	समुच्चयः_alternate_सूचकांक_output(रिक्त);

	rev.diffopt.output_format = DIFF_FORMAT_CALLBACK;
	rev.diffopt.format_callback = सङ्कलय_diff_गन्तव्यम्_buf;
	rev.diffopt.format_callback_दत्त = &diff_output;

	यदि (repo_read_सूचकांक_preload(the_repository, &rev.diffopt.pathspec, 0) < 0) {
		ret = -1;
		गच्छ समाप्तम्;
	}

	सङ्कलय_pending_object(&rev, parse_object(the_repository, &info->b_commit),
			   "");
	सततम्_diff_सूचकांक(&rev, 0);

	cp_upd_सूचकांक.git_cmd = 1;
	strvec_pushl(&cp_upd_सूचकांक.तर्काः, "update-index",
		     "--ignore-skip-worktree-entries",
		     "-z", "--add", "--remove", "--stdin", रिक्त);
	strvec_pushf(&cp_upd_सूचकांक.env, "GIT_INDEX_FILE=%s",
		     stash_सूचकांक_मार्गः.buf);

	यदि (pipe_command(&cp_upd_सूचकांक, diff_output.buf, diff_output.len,
			 रिक्त, 0, रिक्त, 0)) {
		ret = -1;
		गच्छ समाप्तम्;
	}

	यदि (write_सूचकांक_as_वृक्षः(&info->w_वृक्षः, &istate, stash_सूचकांक_मार्गः.buf, 0,
				रिक्त)) {
		ret = -1;
		गच्छ समाप्तम्;
	}

समाप्तम्:
	release_सूचकांक(&istate);
	release_revisions(&rev);
	strbuf_release(&diff_output);
	remove_मार्गः(stash_सूचकांक_मार्गः.buf);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क do_create_stash(ध्रुव संरचना pathspec *ps, संरचना strbuf *stash_सन्देशः_buf,
			   पूर्णाङ्क include_untracked, पूर्णाङ्क patch_बहुलकम्,
			   संरचना interactive_options *interactive_opts,
			   पूर्णाङ्क only_staged, संरचना stash_info *info, संरचना strbuf *patch,
			   पूर्णाङ्क quiet)
{
	पूर्णाङ्क ret = 0;
	पूर्णाङ्क flags = 0;
	पूर्णाङ्क untracked_commit_option = 0;
	ध्रुव वर्ण *शीर्ष_short_sha1 = रिक्त;
	ध्रुव वर्ण *branch_ref = रिक्त;
	ध्रुव वर्ण *branch_name = "(no branch)";
	वर्ण *branch_name_buf = रिक्त;
	संरचना commit *शीर्ष_commit = रिक्त;
	संरचना commit_list *parents = रिक्त;
	संरचना strbuf सन्देशः = STRBUF_INIT;
	संरचना strbuf commit_वृक्षः_label = STRBUF_INIT;
	संरचना strbuf untracked_files = STRBUF_INIT;

	prepare_fallback_ident("git stash", "git@stash");

	repo_read_सूचकांक_preload(the_repository, रिक्त, 0);
	यदि (repo_refresh_and_write_सूचकांक(the_repository, REFRESH_QUIET, 0, 0,
					 रिक्त, रिक्त, रिक्त) < 0) {
		ret = error(_("could not write index"));
		गच्छ समाप्तम्;
	}

	यदि (repo_get_oid(the_repository, "HEAD", &info->b_commit)) {
		यदि (!quiet)
			fprintf_ln(stderr, _("You do not have "
					     "the initial commit yet"));
		ret = -1;
		गच्छ समाप्तम्;
	} अन्यथा {
		शीर्ष_commit = lookup_commit(the_repository, &info->b_commit);
	}

	यदि (!check_changes(ps, include_untracked, &untracked_files)) {
		ret = 1;
		गच्छ समाप्तम्;
	}

	branch_ref = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
					     "HEAD", 0, रिक्त, &flags);

	यदि (flags & REF_ISSYMREF) {
		यदि (skip_उपसर्गः(branch_ref, "refs/heads/", &branch_name))
			branch_name = branch_name_buf = xstrdup(branch_name);
	}

	शीर्ष_short_sha1 = repo_find_unique_abbrev(the_repository,
						  &शीर्ष_commit->object.oid,
						  DEFAULT_ABBREV);
	strbuf_addf(&सन्देशः, "%s: %s ", branch_name, शीर्ष_short_sha1);
	pp_commit_easy(CMIT_FMT_ONELINE, शीर्ष_commit, &सन्देशः);

	strbuf_addf(&commit_वृक्षः_label, "index on %s\n", सन्देशः.buf);
	commit_list_निवेशय(शीर्ष_commit, &parents);
	यदि (write_सूचकांक_as_वृक्षः(&info->i_वृक्षः, the_repository->सूचकांक,
				repo_get_सूचकांक_file(the_repository), 0, रिक्त) ||
	    commit_वृक्षः(commit_वृक्षः_label.buf, commit_वृक्षः_label.len,
			&info->i_वृक्षः, parents, &info->i_commit, रिक्त, रिक्त)) {
		यदि (!quiet)
			fprintf_ln(stderr, _("Cannot save the current "
					     "index state"));
		ret = -1;
		गच्छ समाप्तम्;
	}

	commit_list_free(parents);
	parents = रिक्त;

	यदि (include_untracked) {
		यदि (save_untracked_files(info, &सन्देशः, &untracked_files)) {
			यदि (!quiet)
				fprintf_ln(stderr, _("Cannot save "
						     "the untracked files"));
			ret = -1;
			गच्छ समाप्तम्;
		}
		untracked_commit_option = 1;
	}
	यदि (patch_बहुलकम्) {
		ret = stash_patch(info, ps, patch, quiet, interactive_opts);
		यदि (ret < 0) {
			यदि (!quiet)
				fprintf_ln(stderr, _("Cannot save the current "
						     "worktree state"));
			गच्छ समाप्तम्;
		} अन्यथा यदि (ret > 0) {
			गच्छ समाप्तम्;
		}
	} अन्यथा यदि (only_staged) {
		ret = stash_staged(info, patch, quiet);
		यदि (ret < 0) {
			यदि (!quiet)
				fprintf_ln(stderr, _("Cannot save the current "
						     "staged state"));
			गच्छ समाप्तम्;
		} अन्यथा यदि (ret > 0) {
			गच्छ समाप्तम्;
		}
	} अन्यथा {
		यदि (stash_working_वृक्षः(info, ps)) {
			यदि (!quiet)
				fprintf_ln(stderr, _("Cannot save the current "
						     "worktree state"));
			ret = -1;
			गच्छ समाप्तम्;
		}
	}

	यदि (!stash_सन्देशः_buf->len)
		strbuf_addf(stash_सन्देशः_buf, "WIP on %s", सन्देशः.buf);
	अन्यथा
		strbuf_insertf(stash_सन्देशः_buf, 0, "On %s: ", branch_name);

	यदि (untracked_commit_option)
		commit_list_निवेशय(lookup_commit(the_repository,
						 &info->u_commit),
				   &parents);
	commit_list_निवेशय(lookup_commit(the_repository, &info->i_commit),
			   &parents);
	commit_list_निवेशय(शीर्ष_commit, &parents);

	यदि (commit_वृक्षः(stash_सन्देशः_buf->buf, stash_सन्देशः_buf->len, &info->w_वृक्षः,
			parents, &info->w_commit, रिक्त, रिक्त)) {
		यदि (!quiet)
			fprintf_ln(stderr, _("Cannot record "
					     "working tree state"));
		ret = -1;
		गच्छ समाप्तम्;
	}

समाप्तम्:
	strbuf_release(&commit_वृक्षः_label);
	strbuf_release(&सन्देशः);
	strbuf_release(&untracked_files);
	commit_list_free(parents);
	मुक्त(branch_name_buf);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क create_stash(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः अप्रयुक्तम्,
			संरचना repository *repo अप्रयुक्तम्)
{
	पूर्णाङ्क ret;
	संरचना strbuf stash_सन्देशः_buf = STRBUF_INIT;
	संरचना stash_info info = STASH_INFO_INIT;
	संरचना pathspec ps;

	/* Starting with argv[1], since argv[0] is "create" */
	strbuf_join_argv(&stash_सन्देशः_buf, argc - 1, ++argv, ' ');

	स्मृतिं_पूरय(&ps, 0, परिमाण(ps));
	यदि (!check_changes_tracked_files(&ps))
		प्रत्यागम 0;

	ret = do_create_stash(&ps, &stash_सन्देशः_buf, 0, 0, रिक्त, 0, &info,
			      रिक्त, 0);
	यदि (!ret)
		printf_ln("%s", oid_गन्तव्यम्_hex(&info.w_commit));

	free_stash_info(&info);
	strbuf_release(&stash_सन्देशः_buf);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क do_प्रक्षिप_stash(ध्रुव संरचना pathspec *ps, ध्रुव वर्ण *stash_सन्देशः, पूर्णाङ्क quiet,
			 पूर्णाङ्क keep_सूचकांक, पूर्णाङ्क patch_बहुलकम्,
			 संरचना interactive_options *interactive_opts,
			 पूर्णाङ्क include_untracked, पूर्णाङ्क only_staged)
{
	पूर्णाङ्क ret = 0;
	संरचना stash_info info = STASH_INFO_INIT;
	संरचना strbuf patch = STRBUF_INIT;
	संरचना strbuf stash_सन्देशः_buf = STRBUF_INIT;
	संरचना strbuf untracked_files = STRBUF_INIT;
	संरचना strbuf बहिर्गमनम् = STRBUF_INIT;

	यदि (patch_बहुलकम् && keep_सूचकांक == -1)
		keep_सूचकांक = 1;

	यदि (patch_बहुलकम् && include_untracked) {
		fprintf_ln(stderr, _("Can't use --patch and --include-untracked"
				     " or --all at the same time"));
		ret = -1;
		गच्छ समाप्तम्;
	}

	/* --patch overrides --staged */
	यदि (patch_बहुलकम्)
		only_staged = 0;

	यदि (only_staged && include_untracked) {
		fprintf_ln(stderr, _("Can't use --staged and --include-untracked"
				     " or --all at the same time"));
		ret = -1;
		गच्छ समाप्तम्;
	}

	repo_read_सूचकांक_preload(the_repository, रिक्त, 0);
	यदि (!include_untracked && ps->nr) {
		वर्ण *ps_matched = xcalloc(ps->nr, 1);

		यदि (pathspec_needs_expanded_सूचकांक(the_repository->सूचकांक, ps))
			ensure_पूर्णम्_सूचकांक(the_repository->सूचकांक);
		प्रति (आकार_t i = 0; i < the_repository->सूचकांक->cache_nr; i++)
			ce_मार्गः_मेलनम्(the_repository->सूचकांक, the_repository->सूचकांक->cache[i], ps,
				      ps_matched);

		यदि (report_मार्गः_error(ps_matched, ps)) {
			fprintf_ln(stderr, _("Did you forget to 'git add'?"));
			ret = -1;
			मुक्त(ps_matched);
			गच्छ समाप्तम्;
		}
		मुक्त(ps_matched);
	}

	यदि (repo_refresh_and_write_सूचकांक(the_repository, REFRESH_QUIET, 0, 0,
					 रिक्त, रिक्त, रिक्त)) {
		ret = error(_("could not write index"));
		गच्छ समाप्तम्;
	}

	यदि (!check_changes(ps, include_untracked, &untracked_files)) {
		यदि (!quiet)
			printf_ln(_("No local changes to save"));
		गच्छ समाप्तम्;
	}

	यदि (!refs_reflog_exists(get_main_ref_store(the_repository), ref_stash) && do_clear_stash()) {
		ret = -1;
		यदि (!quiet)
			fprintf_ln(stderr, _("Cannot initialize stash"));
		गच्छ समाप्तम्;
	}

	यदि (stash_सन्देशः)
		strbuf_addstr(&stash_सन्देशः_buf, stash_सन्देशः);
	यदि (do_create_stash(ps, &stash_सन्देशः_buf, include_untracked, patch_बहुलकम्,
			    interactive_opts, only_staged, &info, &patch, quiet)) {
		ret = -1;
		गच्छ समाप्तम्;
	}

	यदि (do_store_stash(&info.w_commit, stash_सन्देशः_buf.buf, 1)) {
		ret = -1;
		यदि (!quiet)
			fprintf_ln(stderr, _("Cannot save the current status"));
		गच्छ समाप्तम्;
	}

	यदि (!quiet)
		printf_ln(_("Saved working directory and index state %s"),
			  stash_सन्देशः_buf.buf);

	यदि (!(patch_बहुलकम् || only_staged)) {
		यदि (include_untracked && !ps->nr) {
			संरचना child_process cp = CHILD_PROCESS_INIT;

			cp.git_cmd = 1;
			यदि (startup_info->original_cwd) {
				cp.dir = startup_info->original_cwd;
				strvec_pushf(&cp.env, "%s=%s",
					     GIT_WORK_वृक्षः_ENVIRONMENT,
					     the_repository->worktree);
			}
			strvec_pushl(&cp.तर्काः, "clean", "--force",
				     "--quiet", "-d", ":/", रिक्त);
			यदि (include_untracked == INCLUDE_ALL_FILES)
				strvec_प्रक्षिप(&cp.तर्काः, "-x");
			यदि (सततम्_command(&cp)) {
				ret = -1;
				गच्छ समाप्तम्;
			}
		}
		discard_सूचकांक(the_repository->सूचकांक);
		यदि (ps->nr) {
			संरचना child_process cp_सङ्कलय = CHILD_PROCESS_INIT;
			संरचना child_process cp_diff = CHILD_PROCESS_INIT;
			संरचना child_process cp_apply = CHILD_PROCESS_INIT;

			cp_सङ्कलय.git_cmd = 1;
			strvec_प्रक्षिप(&cp_सङ्कलय.तर्काः, "add");
			यदि (!include_untracked)
				strvec_प्रक्षिप(&cp_सङ्कलय.तर्काः, "-u");
			यदि (include_untracked == INCLUDE_ALL_FILES)
				strvec_प्रक्षिप(&cp_सङ्कलय.तर्काः, "--force");
			strvec_प्रक्षिप(&cp_सङ्कलय.तर्काः, "--");
			सङ्कलय_pathspecs(&cp_सङ्कलय.तर्काः, ps);
			यदि (सततम्_command(&cp_सङ्कलय)) {
				ret = -1;
				गच्छ समाप्तम्;
			}

			cp_diff.git_cmd = 1;
			strvec_pushl(&cp_diff.तर्काः, "diff-index", "-p",
				     "--no-color",
				     "--cached", "--binary", "HEAD", "--",
				     रिक्त);
			सङ्कलय_pathspecs(&cp_diff.तर्काः, ps);
			यदि (pipe_command(&cp_diff, रिक्त, 0, &बहिर्गमनम्, 0, रिक्त, 0)) {
				ret = -1;
				गच्छ समाप्तम्;
			}

			cp_apply.git_cmd = 1;
			strvec_pushl(&cp_apply.तर्काः, "apply", "--index",
				     "-R", रिक्त);
			यदि (pipe_command(&cp_apply, बहिर्गमनम्.buf, बहिर्गमनम्.len, रिक्त, 0,
					 रिक्त, 0)) {
				ret = -1;
				गच्छ समाप्तम्;
			}
		} अन्यथा {
			संरचना child_process cp = CHILD_PROCESS_INIT;
			cp.git_cmd = 1;
			/* BUG: this nukes untracked files in the way */
			strvec_pushl(&cp.तर्काः, "reset", "--hard", "-q",
				     "--no-recurse-submodules", रिक्त);
			यदि (सततम्_command(&cp)) {
				ret = -1;
				गच्छ समाप्तम्;
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
		यदि (keep_सूचकांक == 1 && !is_null_oid(&info.i_वृक्षः) &&
		    !is_रिक्तानि_वृक्षः_oid(&info.i_वृक्षः, the_repository->hash_algo)) {
			संरचना child_process cp = CHILD_PROCESS_INIT;

			cp.git_cmd = 1;
			strvec_pushl(&cp.तर्काः, "checkout", "--no-overlay",
				     oid_गन्तव्यम्_hex(&info.i_वृक्षः), "--", रिक्त);
			यदि (!ps->nr)
				strvec_प्रक्षिप(&cp.तर्काः, ":/");
			अन्यथा
				सङ्कलय_pathspecs(&cp.तर्काः, ps);
			यदि (सततम्_command(&cp)) {
				ret = -1;
				गच्छ समाप्तम्;
			}
		}
		गच्छ समाप्तम्;
	} अन्यथा {
		संरचना child_process cp = CHILD_PROCESS_INIT;

		cp.git_cmd = 1;
		strvec_pushl(&cp.तर्काः, "apply", "-R", रिक्त);

		यदि (pipe_command(&cp, patch.buf, patch.len, रिक्त, 0, रिक्त, 0)) {
			यदि (!quiet)
				fprintf_ln(stderr, _("Cannot remove "
						     "worktree changes"));
			ret = -1;
			गच्छ समाप्तम्;
		}

		यदि (keep_सूचकांक < 1) {
			संरचना child_process cp = CHILD_PROCESS_INIT;

			cp.git_cmd = 1;
			strvec_pushl(&cp.तर्काः, "reset", "-q", "--refresh", "--",
				     रिक्त);
			सङ्कलय_pathspecs(&cp.तर्काः, ps);
			यदि (सततम्_command(&cp)) {
				ret = -1;
				गच्छ समाप्तम्;
			}
		}
		गच्छ समाप्तम्;
	}

समाप्तम्:
	strbuf_release(&patch);
	strbuf_release(&बहिर्गमनम्);
	free_stash_info(&info);
	strbuf_release(&stash_सन्देशः_buf);
	strbuf_release(&untracked_files);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क प्रक्षिप_stash(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
		      पूर्णाङ्क प्रक्षिप_assumed)
{
	पूर्णाङ्क force_assume = 0;
	पूर्णाङ्क keep_सूचकांक = -1;
	पूर्णाङ्क only_staged = 0;
	पूर्णाङ्क patch_बहुलकम् = 0;
	पूर्णाङ्क include_untracked = 0;
	पूर्णाङ्क quiet = 0;
	पूर्णाङ्क pathspec_file_nul = 0;
	ध्रुव वर्ण *stash_सन्देशः = रिक्त;
	वर्ण *pathspec_from_file = रिक्त;
	संरचना pathspec ps;
	संरचना interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	संरचना option options[] = {
		OPT_BOOL('k', "keep-index", &keep_सूचकांक,
			 N_("keep index")),
		OPT_BOOL('S', "staged", &only_staged,
			 N_("stash staged changes only")),
		OPT_BOOL('p', "patch", &patch_बहुलकम्,
			 N_("stash in patch mode")),
		OPT_BOOL(0, "auto-advance", &interactive_opts.auto_advance,
			 N_("auto advance to the next file when selecting hunks interactively")),
		OPT_DIFF_UNIFIED(&interactive_opts.context),
		OPT_DIFF_INTERHUNK_CONTEXT(&interactive_opts.interhunkcontext),
		OPT__QUIET(&quiet, N_("quiet mode")),
		OPT_BOOL('u', "include-untracked", &include_untracked,
			 N_("include untracked files in stash")),
		OPT_समुच्चयः_INT('a', "all", &include_untracked,
			    N_("include ignore files"), INCLUDE_ALL_FILES),
		OPT_STRING('m', "message", &stash_सन्देशः, N_("message"),
			   N_("stash message")),
		OPT_PATHSPEC_FROM_FILE(&pathspec_from_file),
		OPT_PATHSPEC_FILE_NUL(&pathspec_file_nul),
		OPT_अन्तः()
	};
	पूर्णाङ्क ret;

	यदि (argc) {
		पूर्णाङ्क flags = PARSE_OPT_KEEP_DASHDASH;

		यदि (प्रक्षिप_assumed)
			flags |= PARSE_OPT_STOP_AT_NON_OPTION;

		argc = parse_options(argc, argv, उपसर्गः, options,
				     प्रक्षिप_assumed ? git_stash_usage :
				     git_stash_प्रक्षिप_usage, flags);
		force_assume |= patch_बहुलकम्;
	}

	यदि (argc) {
		यदि (!वाक्यं_तुलय(argv[0], "--")) {
			argc--;
			argv++;
		} अन्यथा यदि (प्रक्षिप_assumed && !force_assume) {
			die("subcommand wasn't specified; 'push' can't be assumed due to unexpected token '%s'",
			    argv[0]);
		}
	}

	parse_pathspec(&ps, 0, PATHSPEC_PREFER_पूर्णम् | PATHSPEC_उपसर्गः_ORIGIN,
		       उपसर्गः, argv);

	यदि (pathspec_from_file) {
		यदि (patch_बहुलकम्)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--patch");

		यदि (only_staged)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--staged");

		यदि (ps.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&ps, 0,
				    PATHSPEC_PREFER_पूर्णम् | PATHSPEC_उपसर्गः_ORIGIN,
				    उपसर्गः, pathspec_from_file, pathspec_file_nul);
	} अन्यथा यदि (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	यदि (!patch_बहुलकम्) {
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

	ret = do_प्रक्षिप_stash(&ps, stash_सन्देशः, quiet, keep_सूचकांक, patch_बहुलकम्,
			    &interactive_opts, include_untracked, only_staged);

	clear_pathspec(&ps);
	मुक्त(pathspec_from_file);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क प्रक्षिप_stash_unassumed(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
				संरचना repository *repo अप्रयुक्तम्)
{
	प्रत्यागम प्रक्षिप_stash(argc, argv, उपसर्गः, 0);
}

स्थिर पूर्णाङ्क save_stash(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
		      संरचना repository *repo अप्रयुक्तम्)
{
	पूर्णाङ्क keep_सूचकांक = -1;
	पूर्णाङ्क only_staged = 0;
	पूर्णाङ्क patch_बहुलकम् = 0;
	पूर्णाङ्क include_untracked = 0;
	पूर्णाङ्क quiet = 0;
	पूर्णाङ्क ret = 0;
	ध्रुव वर्ण *stash_सन्देशः = रिक्त;
	संरचना pathspec ps;
	संरचना strbuf stash_सन्देशः_buf = STRBUF_INIT;
	संरचना interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	संरचना option options[] = {
		OPT_BOOL('k', "keep-index", &keep_सूचकांक,
			 N_("keep index")),
		OPT_BOOL('S', "staged", &only_staged,
			 N_("stash staged changes only")),
		OPT_BOOL('p', "patch", &patch_बहुलकम्,
			 N_("stash in patch mode")),
		OPT_BOOL(0, "auto-advance", &interactive_opts.auto_advance,
			 N_("auto advance to the next file when selecting hunks interactively")),
		OPT_DIFF_UNIFIED(&interactive_opts.context),
		OPT_DIFF_INTERHUNK_CONTEXT(&interactive_opts.interhunkcontext),
		OPT__QUIET(&quiet, N_("quiet mode")),
		OPT_BOOL('u', "include-untracked", &include_untracked,
			 N_("include untracked files in stash")),
		OPT_समुच्चयः_INT('a', "all", &include_untracked,
			    N_("include ignore files"), INCLUDE_ALL_FILES),
		OPT_STRING('m', "message", &stash_सन्देशः, "message",
			   N_("stash message")),
		OPT_अन्तः()
	};

	argc = parse_options(argc, argv, उपसर्गः, options,
			     git_stash_save_usage,
			     PARSE_OPT_KEEP_DASHDASH);

	यदि (argc)
		stash_सन्देशः = strbuf_join_argv(&stash_सन्देशः_buf, argc, argv, ' ');

	स्मृतिं_पूरय(&ps, 0, परिमाण(ps));

	यदि (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	यदि (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	यदि (!patch_बहुलकम्) {
		यदि (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--patch");
		यदि (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--patch");
		यदि (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--patch");
	}

	ret = do_प्रक्षिप_stash(&ps, stash_सन्देशः, quiet, keep_सूचकांक,
			    patch_बहुलकम्, &interactive_opts, include_untracked,
			    only_staged);

	strbuf_release(&stash_सन्देशः_buf);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क write_commit_with_parents(संरचना repository *r,
				     संरचना object_id *बहिर्गमनम्,
				     ध्रुव संरचना object_id *oid,
				     संरचना commit_list *parents)
{
	आकार_t author_len, committer_len;
	संरचना commit *this;
	ध्रुव वर्ण *orig_author, *orig_committer;
	वर्ण *author = रिक्त, *committer = रिक्त;
	ध्रुव वर्ण *बफर;
	अचिह्नित दीर्घ bufsize;
	ध्रुव वर्ण *p;
	संरचना strbuf सन्देशः = STRBUF_INIT;
	पूर्णाङ्क ret = 0;
	संरचना ident_विभज id;

	this = lookup_commit_reference(r, oid);
	बफर = repo_get_commit_बफर(r, this, &bufsize);
	orig_author = find_commit_शीर्षकम्(बफर, "author", &author_len);
	orig_committer = find_commit_शीर्षकम्(बफर, "committer", &committer_len);

	यदि (!orig_author || !orig_committer) {
		ret = error(_("cannot parse commit %s"), oid_गन्तव्यम्_hex(oid));
		गच्छ बहिर्गमनम्;
	}

	यदि (विभज_ident_पङ्क्तिः(&id, orig_author, author_len) < 0 ||
	    विभज_ident_पङ्क्तिः(&id, orig_committer, committer_len) < 0) {
		ret = error(_("invalid author or committer for %s"), oid_गन्तव्यम्_hex(oid));
		गच्छ बहिर्गमनम्;
	}

	p = वाक्ये_अन्विष(बफर, "\n\n");
	strbuf_addstr(&सन्देशः, "git stash: ");

	यदि (p)
		strbuf_सङ्कलय(&सन्देशः, p + 2, bufsize - (p + 2 - बफर));
	strbuf_complete_पङ्क्तिः(&सन्देशः);

	author = xmemdupz(orig_author, author_len);
	committer = xmemdupz(orig_committer, committer_len);

	यदि (commit_वृक्षः_extended(सन्देशः.buf, सन्देशः.len,
				 r->hash_algo->रिक्तानि_वृक्षः, parents,
				 बहिर्गमनम्, author, committer,
				 रिक्त, रिक्त)) {
		ret = error(_("could not write commit"));
		गच्छ बहिर्गमनम्;
	}
बहिर्गमनम्:
	strbuf_release(&सन्देशः);
	repo_unuse_commit_बफर(r, this, बफर);
	मुक्त(author);
	मुक्त(committer);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क do_import_stash(संरचना repository *r, ध्रुव वर्ण *rev)
{
	संरचना object_id chain;
	पूर्णाङ्क res = 0;
	ध्रुव वर्ण *बफर = रिक्त;
	अचिह्नित दीर्घ bufsize;
	संरचना commit *this = रिक्त;
	संरचना commit_list *वस्तूनि = रिक्त, *cur;
	वर्ण *सन्देशः = रिक्त;

	यदि (repo_get_oid(r, rev, &chain))
		प्रत्यागम error(_("not a valid revision: %s"), rev);

	this = lookup_commit_reference(r, &chain);
	यदि (!this)
		प्रत्यागम error(_("not a commit: %s"), rev);

	/*
	 * Walk the commit history, finding each stash entry, and load data into
	 * the array.
	 */
	प्रति (;;) {
		ध्रुव वर्ण *author, *committer;
		आकार_t author_len, committer_len;
		ध्रुव वर्ण *p;
		ध्रुव वर्ण *expected = "git stash <git@stash> 1000684800 +0000";
		ध्रुव वर्ण *उपसर्गः = "git stash: ";
		संरचना commit *stash;
		संरचना वृक्षः *वृक्षः = repo_get_commit_वृक्षः(r, this);

		यदि (!वृक्षः ||
		    !oideq(&वृक्षः->object.oid, r->hash_algo->रिक्तानि_वृक्षः) ||
		    (this->parents &&
		     (!this->parents->अग्र || this->parents->अग्र->अग्र))) {
			res = error(_("%s is not a valid exported stash commit"),
					oid_गन्तव्यम्_hex(&this->object.oid));
			गच्छ बहिर्गमनम्;
		}

		बफर = repo_get_commit_बफर(r, this, &bufsize);

		यदि (!this->parents) {
			/*
			 * We don't have any parents.  Make sure this is our
			 * root commit.
			 */
			author = find_commit_शीर्षकम्(बफर, "author", &author_len);
			committer = find_commit_शीर्षकम्(बफर, "committer", &committer_len);

			यदि (!author || !committer) {
				error(_("cannot parse commit %s"), oid_गन्तव्यम्_hex(&this->object.oid));
				गच्छ बहिर्गमनम्;
			}

			यदि (author_len != वाक्यदैर्घ्यम्(expected) ||
			    committer_len != वाक्यदैर्घ्यम्(expected) ||
			    memcmp(author, expected, author_len) ||
			    memcmp(committer, expected, committer_len)) {
				res = error(_("found root commit %s with invalid data"), oid_गन्तव्यम्_hex(&this->object.oid));
				गच्छ बहिर्गमनम्;
			}
			भङ्ग;
		}

		p = वाक्ये_अन्विष(बफर, "\n\n");
		यदि (!p) {
			res = error(_("cannot parse commit %s"), oid_गन्तव्यम्_hex(&this->object.oid));
			गच्छ बहिर्गमनम्;
		}

		p += 2;
		यदि (((आकार_t)(bufsize - (p - बफर)) < वाक्यदैर्घ्यम्(उपसर्गः)) ||
		    memcmp(उपसर्गः, p, वाक्यदैर्घ्यम्(उपसर्गः))) {
			res = error(_("found stash commit %s without expected prefix"), oid_गन्तव्यम्_hex(&this->object.oid));
			गच्छ बहिर्गमनम्;
		}

		stash = this->parents->अग्र->item;

		यदि (repo_parse_commit(r, this->parents->item) ||
		    repo_parse_commit(r, stash)) {
			res = error(_("cannot parse parents of commit: %s"),
					oid_गन्तव्यम्_hex(&this->object.oid));
			गच्छ बहिर्गमनम्;
		}

		यदि (check_stash_topology(r, stash)) {
			res = error(_("%s does not look like a stash commit"),
					oid_गन्तव्यम्_hex(&stash->object.oid));
			गच्छ बहिर्गमनम्;
		}

		repo_unuse_commit_बफर(r, this, बफर);
		बफर = रिक्त;
		वस्तूनि = commit_list_निवेशय(stash, &वस्तूनि);
		this = this->parents->item;
	}

	/*
	 * Now, walk each entry, adding it to the stash as a normal stash
	 * commit.
	 */
	प्रति (cur = वस्तूनि; cur; cur = cur->अग्र) {
		ध्रुव वर्ण *p;
		संरचना object_id *oid;

		this = cur->item;
		oid = &this->object.oid;
		बफर = repo_get_commit_बफर(r, this, &bufsize);
		यदि (!बफर) {
			res = error(_("cannot read commit buffer for %s"), oid_गन्तव्यम्_hex(oid));
			गच्छ बहिर्गमनम्;
		}

		p = वाक्ये_अन्विष(बफर, "\n\n");
		यदि (!p) {
			res = error(_("cannot parse commit %s"), oid_गन्तव्यम्_hex(oid));
			गच्छ बहिर्गमनम्;
		}

		p += 2;
		सन्देशः = xmemdupz(p, bufsize - (p - बफर));
		repo_unuse_commit_बफर(r, this, बफर);
		बफर = रिक्त;

		यदि (do_store_stash(oid, सन्देशः, 1)) {
			res = error(_("cannot save the stash for %s"), oid_गन्तव्यम्_hex(oid));
			गच्छ बहिर्गमनम्;
		}
		FREE_AND_NULL(सन्देशः);
	}
बहिर्गमनम्:
	यदि (this && बफर)
		repo_unuse_commit_बफर(r, this, बफर);
	commit_list_free(वस्तूनि);
	मुक्त(सन्देशः);

	प्रत्यागम res;
}

स्थिर पूर्णाङ्क import_stash(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
			संरचना repository *repo)
{
	संरचना option options[] = {
		OPT_अन्तः()
	};

	argc = parse_options(argc, argv, उपसर्गः, options,
			     git_stash_import_usage,
			     PARSE_OPT_KEEP_DASHDASH);

	यदि (argc != 1)
		usage_सन्देशः_opt("a revision is required", git_stash_import_usage, options);

	प्रत्यागम do_import_stash(repo, argv[0]);
}

संरचना stash_प्रविष्टिः_दत्त {
	संरचना repository *r;
	संरचना commit_list **वस्तूनि;
	आकार_t गणनम्;
};

स्थिर पूर्णाङ्क collect_stash_entries(ध्रुव वर्ण *refname अप्रयुक्तम्,
				 संरचना object_id *old_oid अप्रयुक्तम्,
				 संरचना object_id *new_oid,
				 ध्रुव वर्ण *committer अप्रयुक्तम्,
				 timestamp_t timestamp अप्रयुक्तम्,
				 पूर्णाङ्क tz अप्रयुक्तम्, ध्रुव वर्ण *सन्देशः अप्रयुक्तम्,
				 शून्य *cb_दत्त)
{
	संरचना stash_प्रविष्टिः_दत्त *दत्त = cb_दत्त;
	संरचना commit *stash;

	दत्त->गणनम्++;
	stash = lookup_commit_reference(दत्त->r, new_oid);
	यदि (!stash || check_stash_topology(दत्त->r, stash)) {
		प्रत्यागम error(_("%s does not look like a stash commit"),
			     oid_गन्तव्यम्_hex(new_oid));
	}
	दत्त->वस्तूनि = commit_list_append(stash, दत्त->वस्तूनि);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क do_export_stash(संरचना repository *r,
			   ध्रुव वर्ण *ref,
			   पूर्णाङ्क argc,
			   ध्रुव वर्ण **argv)
{
	संरचना object_id आधारः;
	संरचना commit *पूर्व;
	संरचना commit_list *वस्तूनि = रिक्त, **iter = &वस्तूनि, *cur;
	पूर्णाङ्क res = 0;
	पूर्णाङ्क i;
	संरचना strbuf revision = STRBUF_INIT;
	ध्रुव वर्ण *author, *committer;

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
	यदि (commit_वृक्षः_extended("", 0, r->hash_algo->रिक्तानि_वृक्षः, रिक्त,
				 &आधारः, author, committer, रिक्त, रिक्त))
		प्रत्यागम error(_("unable to write base commit"));

	पूर्व = lookup_commit_reference(r, &आधारः);

	यदि (argc) {
		/*
		 * Find each specified stash, and load data into the array.
		 */
		प्रति (i = 0; i < argc; i++) {
			संरचना object_id oid;
			संरचना commit *stash;

			यदि (parse_stash_revision(&revision, argv[i], 1) ||
			    repo_get_oid_with_flags(r, revision.buf, &oid,
						    GET_OID_QUIETLY |
						    GET_OID_GENTLY)) {
				res = error(_("unable to find stash entry %s"), argv[i]);
				गच्छ बहिर्गमनम्;
			}

			stash = lookup_commit_reference(r, &oid);
			यदि (!stash || check_stash_topology(r, stash)) {
				res = error(_("%s does not look like a stash commit"),
						revision.buf);
				गच्छ बहिर्गमनम्;
			}
			iter = commit_list_append(stash, iter);
		}
	} अन्यथा {
		/*
		 * Walk the reflog, finding each stash entry, and load data into the
		 * array.
		 */
		संरचना stash_प्रविष्टिः_दत्त cb_दत्त = {
			.r = r, .वस्तूनि = iter,
		};
		यदि (refs_for_each_reflog_ent_reverse(get_main_ref_store(r),
						     "refs/stash",
						     collect_stash_entries,
						     &cb_दत्त) && cb_दत्त.गणनम्)
			गच्छ बहिर्गमनम्;
	}

	/*
	 * Now, create a set of commits identical to the regular stash commits,
	 * but where their first parents form a chain to our original empty
	 * base commit.
	 */
	वस्तूनि = commit_list_reverse(वस्तूनि);
	प्रति (cur = वस्तूनि; cur; cur = cur->अग्र) {
		संरचना commit_list *parents = रिक्त;
		संरचना commit_list **अग्र = &parents;
		संरचना object_id बहिर्गमनम्;
		संरचना commit *stash = cur->item;

		अग्र = commit_list_append(पूर्व, अग्र);
		अग्र = commit_list_append(stash, अग्र);
		res = write_commit_with_parents(r, &बहिर्गमनम्, &stash->object.oid, parents);
		commit_list_free(parents);
		यदि (res)
			गच्छ बहिर्गमनम्;
		पूर्व = lookup_commit_reference(r, &बहिर्गमनम्);
	}
	यदि (ref)
		refs_update_ref(get_main_ref_store(r), रिक्त, ref,
				&पूर्व->object.oid, रिक्त, 0, UPDATE_REFS_DIE_ON_ERR);
	अन्यथा
		पङ्क्तिं_लिख(oid_गन्तव्यम्_hex(&पूर्व->object.oid));
बहिर्गमनम्:
	strbuf_release(&revision);
	commit_list_free(वस्तूनि);

	प्रत्यागम res;
}

गणना export_action {
	ACTION_NONE,
	ACTION_PRINT,
	ACTION_गन्तव्यम्_REF,
};

स्थिर पूर्णाङ्क export_stash(पूर्णाङ्क argc,
			ध्रुव वर्ण **argv,
			ध्रुव वर्ण *उपसर्गः,
			संरचना repository *repo)
{
	ध्रुव वर्ण *ref = रिक्त;
	गणना export_action action = ACTION_NONE;
	संरचना option options[] = {
		OPT_CMDMODE(0, "print", &action,
			    N_("print the object ID instead of writing it to a ref"),
			    ACTION_PRINT),
		OPT_STRING(0, "to-ref", &ref, "ref",
			    N_("save the data to the given ref")),
		OPT_अन्तः()
	};

	argc = parse_options(argc, argv, उपसर्गः, options,
			     git_stash_export_usage,
			     PARSE_OPT_KEEP_DASHDASH);

	यदि (ref && action == ACTION_NONE)
		action = ACTION_गन्तव्यम्_REF;

	यदि (action == ACTION_NONE || (ref && action == ACTION_PRINT))
		प्रत्यागम error(_("exactly one of --print and --to-ref is required"));

	प्रत्यागम do_export_stash(repo, ref, argc, argv);
}

पूर्णाङ्क cmd_stash(पूर्णाङ्क argc,
	      ध्रुव वर्ण **argv,
	      ध्रुव वर्ण *उपसर्गः,
	      संरचना repository *repo)
{
	pid_t pid = getpid();
	ध्रुव वर्ण *सूचकांक_file;
	संरचना strvec तर्काः = STRVEC_INIT;
	parse_opt_subcommand_fn *fn = रिक्त;
	संरचना option options[] = {
		OPT_SUBCOMMAND("apply", &fn, apply_stash),
		OPT_SUBCOMMAND("clear", &fn, clear_stash),
		OPT_SUBCOMMAND("drop", &fn, drop_stash),
		OPT_SUBCOMMAND("pop", &fn, निष्कासय_stash),
		OPT_SUBCOMMAND("branch", &fn, branch_stash),
		OPT_SUBCOMMAND("list", &fn, list_stash),
		OPT_SUBCOMMAND("show", &fn, show_stash),
		OPT_SUBCOMMAND("store", &fn, store_stash),
		OPT_SUBCOMMAND("create", &fn, create_stash),
		OPT_SUBCOMMAND("push", &fn, प्रक्षिप_stash_unassumed),
		OPT_SUBCOMMAND("export", &fn, export_stash),
		OPT_SUBCOMMAND("import", &fn, import_stash),
		OPT_SUBCOMMAND_F("save", &fn, save_stash, रिक्त, PARSE_OPT_NOCOMPLETE),
		OPT_अन्तः()
	};
	ध्रुव वर्ण **तर्काः_copy;
	पूर्णाङ्क ret;

	repo_config(the_repository, git_stash_config, रिक्त);

	argc = parse_options(argc, argv, उपसर्गः, options, git_stash_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL |
			     PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_पूर्णम्_सूचकांक = 0;

	सूचकांक_file = repo_get_सूचकांक_file(the_repository);
	strbuf_addf(&stash_सूचकांक_मार्गः, "%s.stash.%" PRIuMAX, सूचकांक_file,
		    (uintmax_t)pid);

	यदि (fn) {
		ret = fn(argc, argv, उपसर्गः, repo);

		/*
		 * The subcommand implementations return 0 on success, a
		 * negative value on failure, and STASH_APPLY_CONFLICT
		 * when applying a stash entry resulted in conflicts.
		 * Map failures to 128, the status die() uses, so that
		 * exit status 1 unambiguously indicates conflicts.
		 */
		यदि (ret < 0)
			प्रत्यागम 128;
		प्रत्यागम ret;
	} अन्यथा यदि (!argc) {
		प्रत्यागम !!प्रक्षिप_stash_unassumed(0, रिक्त, उपसर्गः, repo);
	}

	/* Assume 'stash push' */
	strvec_प्रक्षिप(&तर्काः, "push");
	strvec_pushv(&तर्काः, argv);

	/*
	 * `push_stash()` ends up modifying the array, which causes memory
	 * leaks if we didn't copy the array here.
	 */
	DUP_ARRAY(तर्काः_copy, तर्काः.v, तर्काः.nr);

	ret = !!प्रक्षिप_stash(तर्काः.nr, तर्काः_copy, उपसर्गः, 1);

	strvec_clear(&तर्काः);
	मुक्त(तर्काः_copy);
	प्रत्यागम ret;
}
