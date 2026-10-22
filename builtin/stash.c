#నిర్వచించు USE_THE_REPOSITORY_VARIABLE

#అన్తర్భావ "builtin.h"
#అన్తర్భావ "abspath.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "hash.h"
#అన్తర్భావ "hex.h"
#అన్తర్భావ "object-name.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "refs.h"
#అన్తర్భావ "stash.h"
#అన్తర్భావ "lockfile.h"
#అన్తర్భావ "cache-tree.h"
#అన్తర్భావ "unpack-trees.h"
#అన్తర్భావ "merge-ort-wrappers.h"
#అన్తర్భావ "strvec.h"
#అన్తర్భావ "run-command.h"
#అన్తర్భావ "dir.h"
#అన్తర్భావ "entry.h"
#అన్తర్భావ "preload-index.h"
#అన్తర్భావ "read-cache.h"
#అన్తర్భావ "repository.h"
#అన్తర్భావ "rerere.h"
#అన్తర్భావ "revision.h"
#అన్తర్భావ "setup.h"
#అన్తర్భావ "sparse-index.h"
#అన్తర్భావ "log-tree.h"
#అన్తర్భావ "diffcore.h"
#అన్తర్భావ "reflog.h"
#అన్తర్భావ "reflog-walk.h"
#అన్తర్భావ "add-interactive.h"
#అన్తర్భావ "commit.h"

#నిర్వచించు INCLUDE_ALL_FILES 2

#నిర్వచించు BUILTIN_STASH_LIST_USAGE \
	N_("git stash list [<log-options>]")
#నిర్వచించు BUILTIN_STASH_SHOW_USAGE \
	N_("git stash show [-u | --include-untracked | --only-untracked] [<diff-options>] [<stash>]")
#నిర్వచించు BUILTIN_STASH_DROP_USAGE \
	N_("git stash drop [-q | --quiet] [<stash>]")
#నిర్వచించు BUILTIN_STASH_తీయి_USAGE \
	N_("git stash pop [--index] [-q | --quiet] [<stash>]")
#నిర్వచించు BUILTIN_STASH_APPLY_USAGE \
	N_("git stash apply [--index] [-q | --quiet] [--label-ours=<label>] [--label-theirs=<label>] [--label-base=<label>] [<stash>]")
#నిర్వచించు BUILTIN_STASH_BRANCH_USAGE \
	N_("git stash branch <branchname> [<stash>]")
#నిర్వచించు BUILTIN_STASH_STORE_USAGE \
	N_("git stash store [(-m | --message) <message>] [-q | --quiet] <commit>")
#నిర్వచించు BUILTIN_STASH_నెట్టు_USAGE \
	N_("git stash [push] [-p | --patch] [-S | --staged] [-k | --[no-]keep-index] [-q | --quiet]\n" \
	   "          [-u | --include-untracked] [-a | --all] [(-m | --message) <message>]\n" \
	   "          [--pathspec-from-file=<file> [--pathspec-file-nul]]\n" \
	   "          [--] [<pathspec>...]")
#నిర్వచించు BUILTIN_STASH_SAVE_USAGE \
	N_("git stash save [-p | --patch] [-S | --staged] [-k | --[no-]keep-index] [-q | --quiet]\n" \
	   "          [-u | --include-untracked] [-a | --all] [<message>]")
#నిర్వచించు BUILTIN_STASH_CREATE_USAGE \
	N_("git stash create [<message>]")
#నిర్వచించు BUILTIN_STASH_EXPORT_USAGE \
	N_("git stash export (--print | --to-ref <ref>) [<stash>...]")
#నిర్వచించు BUILTIN_STASH_IMPORT_USAGE \
	N_("git stash import <commit>")
#నిర్వచించు BUILTIN_STASH_CLEAR_USAGE \
	"git stash clear"

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి git_stash_usage[] = {
	BUILTIN_STASH_LIST_USAGE,
	BUILTIN_STASH_SHOW_USAGE,
	BUILTIN_STASH_DROP_USAGE,
	BUILTIN_STASH_తీయి_USAGE,
	BUILTIN_STASH_APPLY_USAGE,
	BUILTIN_STASH_BRANCH_USAGE,
	BUILTIN_STASH_నెట్టు_USAGE,
	BUILTIN_STASH_SAVE_USAGE,
	BUILTIN_STASH_CLEAR_USAGE,
	BUILTIN_STASH_CREATE_USAGE,
	BUILTIN_STASH_STORE_USAGE,
	BUILTIN_STASH_EXPORT_USAGE,
	BUILTIN_STASH_IMPORT_USAGE,
	ఖాళీ
};

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి git_stash_list_usage[] = {
	BUILTIN_STASH_LIST_USAGE,
	ఖాళీ
};

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి git_stash_show_usage[] = {
	BUILTIN_STASH_SHOW_USAGE,
	ఖాళీ
};

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి git_stash_drop_usage[] = {
	BUILTIN_STASH_DROP_USAGE,
	ఖాళీ
};

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి git_stash_తీయి_usage[] = {
	BUILTIN_STASH_తీయి_USAGE,
	ఖాళీ
};

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి git_stash_apply_usage[] = {
	BUILTIN_STASH_APPLY_USAGE,
	ఖాళీ
};

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి git_stash_branch_usage[] = {
	BUILTIN_STASH_BRANCH_USAGE,
	ఖాళీ
};

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి git_stash_clear_usage[] = {
	BUILTIN_STASH_CLEAR_USAGE,
	ఖాళీ
};

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి git_stash_store_usage[] = {
	BUILTIN_STASH_STORE_USAGE,
	ఖాళీ
};

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి git_stash_నెట్టు_usage[] = {
	BUILTIN_STASH_నెట్టు_USAGE,
	ఖాళీ
};

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి git_stash_save_usage[] = {
	BUILTIN_STASH_SAVE_USAGE,
	ఖాళీ
};

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి git_stash_export_usage[] = {
	BUILTIN_STASH_EXPORT_USAGE,
	ఖాళీ
};

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి git_stash_import_usage[] = {
	BUILTIN_STASH_IMPORT_USAGE,
	ఖాళీ
};

స్థిర స్థిరరాశి అక్షరం ref_stash[] = "refs/stash";
స్థిర నిర్మాణం strbuf stash_సూచీ_మార్గం = STRBUF_INIT;

స్థిర పూర్ణాంకం show_stat = 1;
స్థిర పూర్ణాంకం show_patch;
స్థిర పూర్ణాంకం show_include_untracked;
స్థిర పూర్ణాంకం use_సూచీ;

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
నిర్మాణం stash_info {
	నిర్మాణం object_id w_commit;
	నిర్మాణం object_id b_commit;
	నిర్మాణం object_id i_commit;
	నిర్మాణం object_id u_commit;
	నిర్మాణం object_id c_commit;
	నిర్మాణం object_id w_వృక్షం;
	నిర్మాణం object_id b_వృక్షం;
	నిర్మాణం object_id i_వృక్షం;
	నిర్మాణం object_id u_వృక్షం;
	నిర్మాణం strbuf revision;
	పూర్ణాంకం is_stash_ref;
	పూర్ణాంకం has_u;
};

#నిర్వచించు STASH_INFO_INIT { \
	.revision = STRBUF_INIT, \
}

స్థిర శూన్య free_stash_info(నిర్మాణం stash_info *info)
{
	strbuf_release(&info->revision);
}

స్థిర పూర్ణాంకం check_stash_topology(నిర్మాణం repository *r, నిర్మాణం commit *stash)
{
	నిర్మాణం commit *p1, *p2, *p3 = ఖాళీ;

	/* stash must have two or three parents */
	ఒకవేళ (!stash->parents || !stash->parents->తదుపరి ||
			(stash->parents->తదుపరి->తదుపరి && stash->parents->తదుపరి->తదుపరి->తదుపరి))
		తిరిగి -1;
	p1 = stash->parents->అంశం;
	p2 = stash->parents->తదుపరి->అంశం;
	ఒకవేళ (stash->parents->తదుపరి->తదుపరి)
		p3 = stash->parents->తదుపరి->తదుపరి->అంశం;
	ఒకవేళ (repo_parse_commit(r, p1) || repo_parse_commit(r, p2) ||
			(p3 && repo_parse_commit(r, p3)))
		తిరిగి -1;
	/* p2 must have a single parent, p3 must have no parents */
	ఒకవేళ (!p2->parents || p2->parents->తదుపరి || (p3 && p3->parents))
		తిరిగి -1;
	ఒకవేళ (repo_parse_commit(r, p2->parents->అంశం))
		తిరిగి -1;
	/* p2^1 must equal p1 */
	ఒకవేళ (!oideq(&p1->object.oid, &p2->parents->అంశం->object.oid))
		తిరిగి -1;

	తిరిగి 0;
}

స్థిర శూన్య assert_stash_like(నిర్మాణం stash_info *info, స్థిరరాశి అక్షరం *revision)
{
	ఒకవేళ (get_oidf(&info->b_commit, "%s^1", revision) ||
	    get_oidf(&info->w_వృక్షం, "%s:", revision) ||
	    get_oidf(&info->b_వృక్షం, "%s^1:", revision) ||
	    get_oidf(&info->i_వృక్షం, "%s^2:", revision))
		die(_("'%s' is not a stash-like commit"), revision);
}

స్థిర పూర్ణాంకం parse_stash_revision(నిర్మాణం strbuf *revision, స్థిరరాశి అక్షరం *commit, పూర్ణాంకం quiet)
{
	strbuf_reset(revision);
	ఒకవేళ (!commit) {
		ఒకవేళ (!refs_ref_exists(get_main_ref_store(the_repository), ref_stash)) {
			ఒకవేళ (!quiet)
				fprintf_ln(stderr, _("No stash entries found."));
			తిరిగి -1;
		}

		strbuf_addf(revision, "%s@{0}", ref_stash);
	} లేకపోతే ఒకవేళ (strspn(commit, "0123456789") == అక్షరశ్రేణి_పొడవు(commit)) {
		strbuf_addf(revision, "%s@{%s}", ref_stash, commit);
	} లేకపోతే {
		strbuf_addstr(revision, commit);
	}
	తిరిగి 0;
}

స్థిర పూర్ణాంకం get_stash_info(నిర్మాణం stash_info *info, పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv)
{
	పూర్ణాంకం ret;
	అక్షరం *చివర_యొక్క_rev;
	అక్షరం *expanded_ref;
	స్థిరరాశి అక్షరం *revision;
	స్థిరరాశి అక్షరం *commit = ఖాళీ;
	నిర్మాణం object_id dummy;
	నిర్మాణం strbuf symbolic = STRBUF_INIT;

	ఒకవేళ (argc > 1) {
		పూర్ణాంకం i;
		నిర్మాణం strbuf refs_సందేశం = STRBUF_INIT;

		కొరకు (i = 0; i < argc; i++)
			strbuf_addf(&refs_సందేశం, " '%s'", argv[i]);

		fprintf_ln(stderr, _("Too many revisions specified:%s"),
			   refs_సందేశం.buf);
		strbuf_release(&refs_సందేశం);

		తిరిగి -1;
	}

	ఒకవేళ (argc == 1)
		commit = argv[0];

	strbuf_init(&info->revision, 0);
	ఒకవేళ (parse_stash_revision(&info->revision, commit, 0)) {
		తిరిగి -1;
	}

	revision = info->revision.buf;

	ఒకవేళ (repo_get_oid(the_repository, revision, &info->w_commit))
		తిరిగి error(_("%s is not a valid reference"), revision);

	assert_stash_like(info, revision);

	info->has_u = !get_oidf(&info->u_వృక్షం, "%s^3:", revision);

	చివర_యొక్క_rev = strchrnul(revision, '@');
	strbuf_కూడు(&symbolic, revision, చివర_యొక్క_rev - revision);

	ret = repo_dwim_ref(the_repository, symbolic.buf, symbolic.len,
			    &dummy, &expanded_ref, 0);
	strbuf_release(&symbolic);
	ఎంపిక (ret) {
	స్థితి 0: /* Not found, but valid ref */
		info->is_stash_ref = 0;
		ఆపు;
	స్థితి 1:
		info->is_stash_ref = !strcmp(expanded_ref, ref_stash);
		ఆపు;
	అప్రమేయ: /* Invalid or ambiguous */
		ఆపు;
	}

	విడుదల(expanded_ref);
	తిరిగి !(ret == 0 || ret == 1);
}

స్థిర పూర్ణాంకం do_clear_stash(శూన్య)
{
	నిర్మాణం object_id obj;
	ఒకవేళ (repo_get_oid(the_repository, ref_stash, &obj))
		తిరిగి 0;

	తిరిగి refs_delete_ref(get_main_ref_store(the_repository), ఖాళీ,
			       ref_stash, &obj, 0);
}

స్థిర పూర్ణాంకం clear_stash(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
		       నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం option options[] = {
		OPT_చివర()
	};

	argc = parse_options(argc, argv, ఉపసర్గ, options,
			     git_stash_clear_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	ఒకవేళ (argc)
		తిరిగి error(_("git stash clear with arguments is "
			       "unimplemented"));

	తిరిగి do_clear_stash();
}

స్థిర పూర్ణాంకం reset_వృక్షం(నిర్మాణం object_id *i_వృక్షం, పూర్ణాంకం update, పూర్ణాంకం reset)
{
	పూర్ణాంకం nr_trees = 1;
	నిర్మాణం unpack_trees_options opts;
	నిర్మాణం వృక్షం_desc t[MAX_UNPACK_TREES];
	నిర్మాణం వృక్షం *వృక్షం;
	నిర్మాణం తాళం_file తాళం_file = తాళం_INIT;

	repo_read_సూచీ_preload(the_repository, ఖాళీ, 0);
	ఒకవేళ (refresh_సూచీ(the_repository->సూచీ, REFRESH_QUIET, ఖాళీ, ఖాళీ, ఖాళీ))
		తిరిగి -1;

	repo_hold_locked_సూచీ(the_repository, &తాళం_file, తాళం_DIE_ON_ERROR);

	memset(&opts, 0, పరిమాణం(opts));

	వృక్షం = repo_parse_వృక్షం_పరోక్ష(the_repository, i_వృక్షం);
	ఒకవేళ (repo_parse_వృక్షం(the_repository, వృక్షం))
		తిరిగి -1;

	init_వృక్షం_desc(t, &వృక్షం->object.oid, వృక్షం->బఫరు, వృక్షం->కొలత);

	opts.తల_idx = 1;
	opts.src_సూచీ = the_repository->సూచీ;
	opts.dst_సూచీ = the_repository->సూచీ;
	opts.విలీనం = 1;
	opts.reset = reset ? UNPACK_RESET_PROTECT_UNTRACKED : 0;
	opts.update = update;
	ఒకవేళ (update)
		opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
	opts.fn = oneway_విలీనం;

	ఒకవేళ (unpack_trees(nr_trees, t, &opts))
		తిరిగి -1;

	ఒకవేళ (write_locked_సూచీ(the_repository->సూచీ, &తాళం_file, COMMIT_తాళం))
		తిరిగి error(_("unable to write new index file"));

	తిరిగి 0;
}

స్థిర పూర్ణాంకం create_సూచీ_from_వృక్షం(స్థిరరాశి నిర్మాణం object_id *వృక్షం_id,
				  స్థిరరాశి అక్షరం *సూచీ_మార్గం)
{
	పూర్ణాంకం nr_trees = 1;
	పూర్ణాంకం ret = 0;
	నిర్మాణం unpack_trees_options opts;
	నిర్మాణం వృక్షం_desc t[MAX_UNPACK_TREES];
	నిర్మాణం వృక్షం *వృక్షం;
	నిర్మాణం సూచీ_state dst_istate = సూచీ_STATE_INIT(the_repository);
	నిర్మాణం తాళం_file తాళం_file = తాళం_INIT;

	repo_read_సూచీ_preload(the_repository, ఖాళీ, 0);
	refresh_సూచీ(the_repository->సూచీ, REFRESH_QUIET, ఖాళీ, ఖాళీ, ఖాళీ);

	hold_తాళం_file_for_update(&తాళం_file, సూచీ_మార్గం, తాళం_DIE_ON_ERROR);

	memset(&opts, 0, పరిమాణం(opts));

	వృక్షం = repo_parse_వృక్షం_పరోక్ష(the_repository, వృక్షం_id);
	ఒకవేళ (!వృక్షం || repo_parse_వృక్షం(the_repository, వృక్షం)) {
		ret = -1;
		వెళ్ళు పూర్తి;
	}

	init_వృక్షం_desc(t, &వృక్షం->object.oid, వృక్షం->బఫరు, వృక్షం->కొలత);

	opts.తల_idx = 1;
	opts.src_సూచీ = the_repository->సూచీ;
	opts.dst_సూచీ = &dst_istate;
	opts.విలీనం = 1;
	opts.reset = UNPACK_RESET_PROTECT_UNTRACKED;
	opts.fn = oneway_విలీనం;

	ఒకవేళ (unpack_trees(nr_trees, t, &opts)) {
		ret = -1;
		వెళ్ళు పూర్తి;
	}

	ఒకవేళ (write_locked_సూచీ(&dst_istate, &తాళం_file, COMMIT_తాళం)) {
		ret = error(_("unable to write new index file"));
		వెళ్ళు పూర్తి;
	}

పూర్తి:
	release_సూచీ(&dst_istate);
	ఒకవేళ (ret)
		rollback_తాళం_file(&తాళం_file);
	తిరిగి ret;
}

స్థిర పూర్ణాంకం is_మార్గం_a_directory(స్థిరరాశి అక్షరం *మార్గం)
{
	/*
	 * This function differs from abspath.c:is_directory() in that
	 * here we use lstat() instead of stat(); we do not want to
	 * follow symbolic links here.
	 */
	నిర్మాణం stat st;
	తిరిగి (!lstat(మార్గం, &st) && S_ISDIR(st.st_బహుళకం));
}

స్థిర శూన్య కూడు_diff_గమ్యం_buf(నిర్మాణం diff_వరుస_struct *q,
			    నిర్మాణం diff_options *options వాడని,
			    శూన్య *దత్తాంశం)
{
	పూర్ణాంకం i;

	కొరకు (i = 0; i < q->nr; i++) {
		ఒకవేళ (is_మార్గం_a_directory(q->వరుస[i]->one->మార్గం))
			కొనసాగు;

		strbuf_addstr(దత్తాంశం, q->వరుస[i]->one->మార్గం);

		/* NUL-terminate: will be fed to update-index -z */
		strbuf_addch(దత్తాంశం, '\0');
	}
}

స్థిర పూర్ణాంకం restore_untracked(నిర్మాణం object_id *u_వృక్షం)
{
	పూర్ణాంకం res;
	నిర్మాణం బిడ్డ_process cp = బిడ్డ_PROCESS_INIT;

	/*
	 * We need to run restore files from a given index, but without
	 * affecting the current index, so we use GIT_INDEX_FILE with
	 * run_command to fork processes that will not interfere.
	 */
	cp.git_cmd = 1;
	strvec_నెట్టు(&cp.వాదనలు, "read-tree");
	strvec_నెట్టు(&cp.వాదనలు, oid_గమ్యం_hex(u_వృక్షం));
	strvec_pushf(&cp.env, "GIT_INDEX_FILE=%s",
		     stash_సూచీ_మార్గం.buf);
	ఒకవేళ (నిరంతరం_command(&cp)) {
		remove_మార్గం(stash_సూచీ_మార్గం.buf);
		తిరిగి -1;
	}

	బిడ్డ_process_init(&cp);
	cp.git_cmd = 1;
	strvec_pushl(&cp.వాదనలు, "checkout-index", "--all", ఖాళీ);
	strvec_pushf(&cp.env, "GIT_INDEX_FILE=%s",
		     stash_సూచీ_మార్గం.buf);

	res = నిరంతరం_command(&cp);
	remove_మార్గం(stash_సూచీ_మార్గం.buf);
	తిరిగి res;
}

స్థిర శూన్య unstage_changes_unless_new(నిర్మాణం object_id *orig_వృక్షం)
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

	నిర్మాణం checkout state = CHECKOUT_INIT;
	నిర్మాణం diff_options diff_opts;
	నిర్మాణం తాళం_file తాళం = తాళం_INIT;
	పూర్ణాంకం i;

	/* If any entries have skip_worktree set, we'll have to check 'em out */
	state.force = 1;
	state.quiet = 1;
	state.refresh_cache = 1;
	state.istate = the_repository->సూచీ;

	/*
	 * Step 1: get a difference between orig_tree (which corresponding
	 * to the index before a merge was run) and the current index
	 * (reflecting the changes brought in by the merge).
	 */
	repo_diff_అమరిక(the_repository, &diff_opts);
	diff_opts.flags.recursive = 1;
	diff_opts.detect_rename = 0;
	diff_opts.output_format = DIFF_FORMAT_NO_OUTPUT;
	diff_అమరిక_పూర్తి(&diff_opts);

	do_diff_cache(orig_వృక్షం, &diff_opts);
	diffcore_std(&diff_opts);

	/* Iterate over the paths that changed due to the merge... */
	కొరకు (i = 0; i < diff_queued_diff.nr; i++) {
		నిర్మాణం diff_filepair *p;
		నిర్మాణం cache_నమోదు *ce;
		పూర్ణాంకం pos;

		/* Look up the path's position in the current index. */
		p = diff_queued_diff.వరుస[i];
		pos = సూచీ_name_pos(the_repository->సూచీ, p->two->మార్గం,
				     అక్షరశ్రేణి_పొడవు(p->two->మార్గం));

		/*
		 * Step 2: Place changes in the working tree
		 *
		 * Stash is about restoring changes *to the working tree*.
		 * So if the merge successfully got a new version of some
		 * path, but left it out of the working tree, then clear the
		 * SKIP_WORKTREE bit and write it to the working tree.
		 */
		ఒకవేళ (pos >= 0 && ce_skip_worktree(the_repository->సూచీ->cache[pos])) {
			నిర్మాణం stat st;

			ce = the_repository->సూచీ->cache[pos];
			ఒకవేళ (!lstat(ce->name, &st)) {
				/* Conflicting path present; relocate it */
				నిర్మాణం strbuf new_మార్గం = STRBUF_INIT;
				పూర్ణాంకం fd;

				strbuf_addf(&new_మార్గం,
					    "%s.stash.XXXXXX", ce->name);
				fd = xmkstemp(new_మార్గం.buf);
				close(fd);
				రాయి(_("WARNING: Untracked file in way of "
					 "tracked file!  Renaming\n "
					 "           %s -> %s\n"
					 "         to make room.\n"),
				       ce->name, new_మార్గం.buf);
				ఒకవేళ (rename(ce->name, new_మార్గం.buf))
					die("Failed to move %s to %s",
					    ce->name, new_మార్గం.buf);
				strbuf_release(&new_మార్గం);
			}
			checkout_నమోదు(ce, &state, ఖాళీ, ఖాళీ);
			ce->ce_flags &= ~CE_SKIP_WORKTREE;
		}

		/*
		 * Step 3: "unstage" changes, as long as they are still tracked
		 */
		ఒకవేళ (p->one->oid_valid) {
			/*
			 * Path existed in orig_tree; restore index entry
			 * from that tree in order to "unstage" the changes.
			 */
			పూర్ణాంకం option = కూడు_CACHE_OK_గమ్యం_REPLACE;
			ఒకవేళ (pos < 0)
				option = కూడు_CACHE_OK_గమ్యం_కూడు;

			ce = make_cache_నమోదు(the_repository->సూచీ,
					      p->one->బహుళకం,
					      &p->one->oid,
					      p->one->మార్గం,
					      0, 0);
			కూడు_సూచీ_నమోదు(the_repository->సూచీ, ce, option);
		}
	}
	diff_flush(&diff_opts);

	/*
	 * Step 4: write the new index to disk
	 */
	repo_hold_locked_సూచీ(the_repository, &తాళం, తాళం_DIE_ON_ERROR);
	ఒకవేళ (write_locked_సూచీ(the_repository->సూచీ, &తాళం,
			       COMMIT_తాళం | SKIP_IF_UNCHANGED))
		die(_("could not write index"));
}

స్థిర గణన stash_apply_ఫలితం do_apply_stash(స్థిరరాశి అక్షరం *ఉపసర్గ,
					      నిర్మాణం stash_info *info,
					      పూర్ణాంకం సూచీ, పూర్ణాంకం quiet,
					      స్థిరరాశి అక్షరం *label_ours,
					      స్థిరరాశి అక్షరం *label_theirs,
					      స్థిరరాశి అక్షరం *label_పునాది)
{
	పూర్ణాంకం clean, ret;
	పూర్ణాంకం has_సూచీ = సూచీ;
	నిర్మాణం విలీనం_options o;
	నిర్మాణం object_id c_వృక్షం;
	నిర్మాణం object_id సూచీ_వృక్షం;
	నిర్మాణం వృక్షం *తల, *విలీనం, *విలీనం_పునాది;
	నిర్మాణం తాళం_file తాళం = తాళం_INIT;

	repo_read_సూచీ_preload(the_repository, ఖాళీ, 0);
	ఒకవేళ (repo_refresh_and_write_సూచీ(the_repository, REFRESH_QUIET, 0, 0,
					 ఖాళీ, ఖాళీ, ఖాళీ))
		తిరిగి error(_("could not write index"));

	ఒకవేళ (write_సూచీ_as_వృక్షం(&c_వృక్షం, the_repository->సూచీ,
				repo_get_సూచీ_file(the_repository), 0, ఖాళీ))
		తిరిగి error(_("cannot apply a stash in the middle of a merge"));

	init_ui_విలీనం_options(&o, the_repository);

	ఒకవేళ (quiet)
		o.verbosity = 0;

	ఒకవేళ (సూచీ) {
		ఒకవేళ (oideq(&info->b_వృక్షం, &info->i_వృక్షం) ||
		    oideq(&c_వృక్షం, &info->i_వృక్షం)) {
			has_సూచీ = 0;
		} లేకపోతే {
			నిర్మాణం విలీనం_ఫలితం ఫలితం = { 0 };

			o.branch1 = "Current index";
			o.branch2 = "Stashed index changes";
			o.ancestor = "Stash base";

			తల = lookup_వృక్షం(o.repo, &c_వృక్షం);
			విలీనం = lookup_వృక్షం(o.repo, &info->i_వృక్షం);
			విలీనం_పునాది = lookup_వృక్షం(o.repo, &info->b_వృక్షం);

			విలీనం_incore_nonrecursive(&o, విలీనం_పునాది, తల, విలీనం,
						  &ఫలితం);

			ఒకవేళ (ఫలితం.clean < 0) {
				విలీనం_finalize(&o, &ఫలితం);
				తిరిగి error(_("index merge failed"));
			} లేకపోతే ఒకవేళ (!ఫలితం.clean) {
				విలీనం_finalize(&o, &ఫలితం);
				తిరిగి error(_("conflicts in index. "
					       "Try without --index."));
			} లేకపోతే {
				oidcpy(&సూచీ_వృక్షం, &ఫలితం.వృక్షం->object.oid);
				విలీనం_finalize(&o, &ఫలితం);
			}
		}
	}

	o.branch1 = label_ours ? label_ours : "Updated upstream";
	o.branch2 = label_theirs ? label_theirs : "Stashed changes";
	o.ancestor = label_పునాది ? label_పునాది : "Stash base";

	ఒకవేళ (oideq(&info->b_వృక్షం, &c_వృక్షం))
		o.branch1 = "Version stash was based on";

	ఒకవేళ (o.verbosity >= 3)
		printf_ln(_("Merging %s with %s"), o.branch1, o.branch2);

	తల = lookup_వృక్షం(o.repo, &c_వృక్షం);
	విలీనం = lookup_వృక్షం(o.repo, &info->w_వృక్షం);
	విలీనం_పునాది = lookup_వృక్షం(o.repo, &info->b_వృక్షం);

	repo_hold_locked_సూచీ(o.repo, &తాళం, తాళం_DIE_ON_ERROR);
	clean = విలీనం_ort_nonrecursive(&o, తల, విలీనం, విలీనం_పునాది);

	/*
	 * If 'clean' >= 0, reverse the value for 'ret' so 'ret' is 0 when the
	 * merge was clean, and 1 if the merge was unclean or a negative value
	 * if it encountered an error.
	 */
	ret = clean >= 0 ? !clean : clean;

	ఒకవేళ (ret < 0)
		rollback_తాళం_file(&తాళం);
	లేకపోతే ఒకవేళ (write_locked_సూచీ(o.repo->సూచీ, &తాళం,
				      COMMIT_తాళం | SKIP_IF_UNCHANGED))
		ret = error(_("could not write index"));

	ఒకవేళ (ret) {
		repo_rerere(the_repository, 0);

		ఒకవేళ (సూచీ)
			fprintf_ln(stderr, _("Index was not unstashed."));

		వెళ్ళు restore_untracked;
	}

	ఒకవేళ (has_సూచీ) {
		ఒకవేళ (reset_వృక్షం(&సూచీ_వృక్షం, 0, 0))
			ret = -1;
	} లేకపోతే {
		unstage_changes_unless_new(&c_వృక్షం);
	}

restore_untracked:
	ఒకవేళ (info->has_u && restore_untracked(&info->u_వృక్షం))
		ret = error(_("could not restore untracked files from stash"));

	ఒకవేళ (!quiet) {
		నిర్మాణం బిడ్డ_process cp = బిడ్డ_PROCESS_INIT;

		/*
		 * Status is quite simple and could be replaced with calls to
		 * wt_status in the future, but it adds complexities which may
		 * require more tests.
		 */
		cp.git_cmd = 1;
		cp.dir = ఉపసర్గ;
		strvec_pushf(&cp.env, GIT_WORK_వృక్షం_ENVIRONMENT"=%s",
			     absolute_మార్గం(repo_get_work_వృక్షం(the_repository)));
		strvec_pushf(&cp.env, GIT_DIR_ENVIRONMENT"=%s",
			     absolute_మార్గం(repo_get_git_dir(the_repository)));
		strvec_నెట్టు(&cp.వాదనలు, "status");
		నిరంతరం_command(&cp);
	}

	తిరిగి ret;
}

స్థిర పూర్ణాంకం apply_stash(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
		       నిర్మాణం repository *repo వాడని)
{
	పూర్ణాంకం ret = -1;
	పూర్ణాంకం quiet = 0;
	పూర్ణాంకం సూచీ = use_సూచీ;
	స్థిరరాశి అక్షరం *label_ours = ఖాళీ, *label_theirs = ఖాళీ, *label_పునాది = ఖాళీ;
	నిర్మాణం stash_info info = STASH_INFO_INIT;
	నిర్మాణం option options[] = {
		OPT__QUIET(&quiet, N_("be quiet, only report errors")),
		OPT_BOOL(0, "index", &సూచీ,
			 N_("attempt to recreate the index")),
		OPT_STRING(0, "label-ours", &label_ours, N_("label"),
			   N_("label for the upstream side in conflict markers")),
		OPT_STRING(0, "label-theirs", &label_theirs, N_("label"),
			   N_("label for the stashed side in conflict markers")),
		OPT_STRING(0, "label-base", &label_పునాది, N_("label"),
			   N_("label for the base in diff3 conflict markers")),
		OPT_చివర()
	};

	argc = parse_options(argc, argv, ఉపసర్గ, options,
			     git_stash_apply_usage, 0);

	ఒకవేళ (get_stash_info(&info, argc, argv))
		వెళ్ళు cleanup;

	ret = do_apply_stash(ఉపసర్గ, &info, సూచీ, quiet,
			     label_ours, label_theirs, label_పునాది);
cleanup:
	free_stash_info(&info);
	తిరిగి ret;
}

స్థిర పూర్ణాంకం reject_reflog_ent(స్థిరరాశి అక్షరం *refname వాడని,
			     నిర్మాణం object_id *ooid వాడని,
			     నిర్మాణం object_id *noid వాడని,
			     స్థిరరాశి అక్షరం *email వాడని,
			     timestamp_t timestamp వాడని,
			     పూర్ణాంకం tz వాడని, స్థిరరాశి అక్షరం *సందేశం_2 వాడని,
			     శూన్య *cb_దత్తాంశం వాడని)
{
	తిరిగి 1;
}

స్థిర పూర్ణాంకం reflog_is_ఖాళీలు(స్థిరరాశి అక్షరం *refname)
{
	తిరిగి !refs_for_each_reflog_ent(get_main_ref_store(the_repository),
					 refname, reject_reflog_ent, ఖాళీ);
}

స్థిర పూర్ణాంకం do_drop_stash(నిర్మాణం stash_info *info, పూర్ణాంకం quiet)
{
	ఒకవేళ (!reflog_delete(info->revision.buf,
			   EXPIRE_REFLOGS_REWRITE | EXPIRE_REFLOGS_UPDATE_REF,
			   0)) {
		ఒకవేళ (!quiet)
			printf_ln(_("Dropped %s (%s)"), info->revision.buf,
				  oid_గమ్యం_hex(&info->w_commit));
	} లేకపోతే {
		తిరిగి error(_("%s: Could not drop stash entry"),
			     info->revision.buf);
	}

	ఒకవేళ (reflog_is_ఖాళీలు(ref_stash))
		do_clear_stash();

	తిరిగి 0;
}

స్థిర పూర్ణాంకం get_stash_info_assert(నిర్మాణం stash_info *info, పూర్ణాంకం argc,
				 స్థిరరాశి అక్షరం **argv)
{
	పూర్ణాంకం ret = get_stash_info(info, argc, argv);

	ఒకవేళ (ret < 0)
		తిరిగి ret;

	ఒకవేళ (!info->is_stash_ref)
		తిరిగి error(_("'%s' is not a stash reference"), info->revision.buf);

	తిరిగి 0;
}

స్థిర పూర్ణాంకం drop_stash(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
		      నిర్మాణం repository *repo వాడని)
{
	పూర్ణాంకం ret = -1;
	పూర్ణాంకం quiet = 0;
	నిర్మాణం stash_info info = STASH_INFO_INIT;
	నిర్మాణం option options[] = {
		OPT__QUIET(&quiet, N_("be quiet, only report errors")),
		OPT_చివర()
	};

	argc = parse_options(argc, argv, ఉపసర్గ, options,
			     git_stash_drop_usage, 0);

	ఒకవేళ (get_stash_info_assert(&info, argc, argv))
		వెళ్ళు cleanup;

	ret = do_drop_stash(&info, quiet);
cleanup:
	free_stash_info(&info);
	తిరిగి ret;
}

స్థిర పూర్ణాంకం తీయి_stash(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
		     నిర్మాణం repository *repo వాడని)
{
	పూర్ణాంకం ret = -1;
	పూర్ణాంకం సూచీ = use_సూచీ;
	పూర్ణాంకం quiet = 0;
	నిర్మాణం stash_info info = STASH_INFO_INIT;
	నిర్మాణం option options[] = {
		OPT__QUIET(&quiet, N_("be quiet, only report errors")),
		OPT_BOOL(0, "index", &సూచీ,
			 N_("attempt to recreate the index")),
		OPT_చివర()
	};

	argc = parse_options(argc, argv, ఉపసర్గ, options,
			     git_stash_తీయి_usage, 0);

	ఒకవేళ (get_stash_info_assert(&info, argc, argv))
		వెళ్ళు cleanup;

	ఒకవేళ ((ret = do_apply_stash(ఉపసర్గ, &info, సూచీ, quiet,
				  ఖాళీ, ఖాళీ, ఖాళీ)))
		printf_ln(_("The stash entry is kept in case "
			    "you need it again."));
	లేకపోతే
		ret = do_drop_stash(&info, quiet);

cleanup:
	free_stash_info(&info);
	తిరిగి ret;
}

స్థిర పూర్ణాంకం branch_stash(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
			నిర్మాణం repository *repo వాడని)
{
	పూర్ణాంకం ret = -1;
	స్థిరరాశి అక్షరం *branch = ఖాళీ;
	నిర్మాణం stash_info info = STASH_INFO_INIT;
	నిర్మాణం బిడ్డ_process cp = బిడ్డ_PROCESS_INIT;
	నిర్మాణం option options[] = {
		OPT_చివర()
	};

	argc = parse_options(argc, argv, ఉపసర్గ, options,
			     git_stash_branch_usage, 0);

	ఒకవేళ (!argc) {
		fprintf_ln(stderr, _("No branch name specified"));
		తిరిగి -1;
	}

	branch = argv[0];

	ఒకవేళ (get_stash_info(&info, argc - 1, argv + 1))
		వెళ్ళు cleanup;

	cp.git_cmd = 1;
	strvec_pushl(&cp.వాదనలు, "checkout", "-b", ఖాళీ);
	strvec_నెట్టు(&cp.వాదనలు, branch);
	strvec_నెట్టు(&cp.వాదనలు, oid_గమ్యం_hex(&info.b_commit));
	ret = నిరంతరం_command(&cp);
	ఒకవేళ (!ret)
		ret = do_apply_stash(ఉపసర్గ, &info, 1, 0,
				     ఖాళీ, ఖాళీ, ఖాళీ);
	ఒకవేళ (!ret && info.is_stash_ref)
		ret = do_drop_stash(&info, 0);

cleanup:
	free_stash_info(&info);
	తిరిగి ret;
}

స్థిర పూర్ణాంకం list_stash(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
		      నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం బిడ్డ_process cp = బిడ్డ_PROCESS_INIT;
	నిర్మాణం option options[] = {
		OPT_చివర()
	};

	argc = parse_options(argc, argv, ఉపసర్గ, options,
			     git_stash_list_usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT);

	ఒకవేళ (!refs_ref_exists(get_main_ref_store(the_repository), ref_stash))
		తిరిగి 0;

	cp.git_cmd = 1;
	strvec_pushl(&cp.వాదనలు, "log", "--format=%gd: %gs", "-g",
		     "--first-parent", ఖాళీ);
	strvec_pushv(&cp.వాదనలు, argv);
	strvec_నెట్టు(&cp.వాదనలు, ref_stash);
	strvec_నెట్టు(&cp.వాదనలు, "--");
	తిరిగి నిరంతరం_command(&cp);
}

స్థిర పూర్ణాంకం git_stash_config(స్థిరరాశి అక్షరం *var, స్థిరరాశి అక్షరం *విలువ,
			    స్థిరరాశి నిర్మాణం config_context *ctx, శూన్య *cb)
{
	ఒకవేళ (!strcmp(var, "stash.showstat")) {
		show_stat = git_config_bool(var, విలువ);
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(var, "stash.showpatch")) {
		show_patch = git_config_bool(var, విలువ);
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(var, "stash.showincludeuntracked")) {
		show_include_untracked = git_config_bool(var, విలువ);
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(var, "stash.index")) {
		use_సూచీ = git_config_bool(var, విలువ);
		తిరిగి 0;
	}
	తిరిగి git_diff_basic_config(var, విలువ, ctx, cb);
}

స్థిర శూన్య diff_include_untracked(స్థిరరాశి నిర్మాణం stash_info *info, నిర్మాణం diff_options *diff_opt)
{
	స్థిరరాశి నిర్మాణం object_id *oid[] = { &info->w_commit, &info->u_వృక్షం };
	నిర్మాణం వృక్షం *వృక్షం[ARRAY_కొలత(oid)];
	నిర్మాణం వృక్షం_desc వృక్షం_desc[ARRAY_కొలత(oid)];
	నిర్మాణం unpack_trees_options unpack_వృక్షం_opt = { 0 };

	కొరకు (కొలత_t i = 0; i < ARRAY_కొలత(oid); i++) {
		వృక్షం[i] = repo_parse_వృక్షం_పరోక్ష(the_repository, oid[i]);
		ఒకవేళ (repo_parse_వృక్షం(the_repository, వృక్షం[i]) < 0)
			die(_("failed to parse tree"));
		init_వృక్షం_desc(&వృక్షం_desc[i], &వృక్షం[i]->object.oid,
			       వృక్షం[i]->బఫరు, వృక్షం[i]->కొలత);
	}

	unpack_వృక్షం_opt.తల_idx = -1;
	unpack_వృక్షం_opt.src_సూచీ = the_repository->సూచీ;
	unpack_వృక్షం_opt.dst_సూచీ = the_repository->సూచీ;
	unpack_వృక్షం_opt.విలీనం = 1;
	unpack_వృక్షం_opt.fn = stash_worktree_untracked_విలీనం;

	ఒకవేళ (unpack_trees(ARRAY_కొలత(వృక్షం_desc), వృక్షం_desc, &unpack_వృక్షం_opt))
		die(_("failed to unpack trees"));

	do_diff_cache(&info->b_commit, diff_opt);
}

స్థిర పూర్ణాంకం show_stash(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
		      నిర్మాణం repository *repo వాడని)
{
	పూర్ణాంకం i;
	పూర్ణాంకం ret = -1;
	నిర్మాణం stash_info info = STASH_INFO_INIT;
	నిర్మాణం rev_info rev;
	నిర్మాణం strvec stash_వాదనలు = STRVEC_INIT;
	నిర్మాణం strvec revision_వాదనలు = STRVEC_INIT;
	గణన {
		UNTRACKED_NONE,
		UNTRACKED_INCLUDE,
		UNTRACKED_ONLY
	} show_untracked = show_include_untracked ? UNTRACKED_INCLUDE : UNTRACKED_NONE;
	నిర్మాణం option options[] = {
		OPT_సమితి_INT('u', "include-untracked", &show_untracked,
			    N_("include untracked files in the stash"),
			    UNTRACKED_INCLUDE),
		OPT_సమితి_INT_F(0, "only-untracked", &show_untracked,
			      N_("only show untracked files in the stash"),
			      UNTRACKED_ONLY, PARSE_OPT_NONEG),
		OPT_చివర()
	};
	పూర్ణాంకం do_usage = 0;

	init_diff_ui_defaults();
	repo_config(the_repository, git_diff_ui_config, ఖాళీ);
	repo_init_revisions(the_repository, &rev, ఉపసర్గ);

	argc = parse_options(argc, argv, ఉపసర్గ, options, git_stash_show_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	strvec_నెట్టు(&revision_వాదనలు, argv[0]);
	కొరకు (i = 1; i < argc; i++) {
		ఒకవేళ (argv[i][0] != '-')
			strvec_నెట్టు(&stash_వాదనలు, argv[i]);
		లేకపోతే
			strvec_నెట్టు(&revision_వాదనలు, argv[i]);
	}

	ఒకవేళ (get_stash_info(&info, stash_వాదనలు.nr, stash_వాదనలు.v))
		వెళ్ళు cleanup;

	/*
	 * The config settings are applied only if there are not passed
	 * any options.
	 */
	ఒకవేళ (revision_వాదనలు.nr == 1) {
		ఒకవేళ (show_stat)
			rev.diffopt.output_format = DIFF_FORMAT_DIFFSTAT;

		ఒకవేళ (show_patch)
			rev.diffopt.output_format |= DIFF_FORMAT_PATCH;

		ఒకవేళ (!show_stat && !show_patch) {
			ret = 0;
			వెళ్ళు cleanup;
		}
	}

	అమరిక_revisions_from_strvec(&revision_వాదనలు, &rev, ఖాళీ);
	ఒకవేళ (revision_వాదనలు.nr > 1)
		వెళ్ళు usage;
	ఒకవేళ (!rev.diffopt.output_format) {
		rev.diffopt.output_format = DIFF_FORMAT_PATCH;
		diff_అమరిక_పూర్తి(&rev.diffopt);
	}

	rev.diffopt.flags.recursive = 1;
	అమరిక_diff_pager(&rev.diffopt);
	ఎంపిక (show_untracked) {
	స్థితి UNTRACKED_NONE:
		diff_వృక్షం_oid(&info.b_commit, &info.w_commit, "", &rev.diffopt);
		ఆపు;
	స్థితి UNTRACKED_ONLY:
		ఒకవేళ (info.has_u)
			diff_మూలం_వృక్షం_oid(&info.u_వృక్షం, "", &rev.diffopt);
		ఆపు;
	స్థితి UNTRACKED_INCLUDE:
		ఒకవేళ (info.has_u)
			diff_include_untracked(&info, &rev.diffopt);
		లేకపోతే
			diff_వృక్షం_oid(&info.b_commit, &info.w_commit, "", &rev.diffopt);
		ఆపు;
	}
	log_వృక్షం_diff_flush(&rev);

	ret = diff_ఫలితం_సంకేతం(&rev);

cleanup:
	strvec_clear(&revision_వాదనలు);
	strvec_clear(&stash_వాదనలు);
	free_stash_info(&info);
	release_revisions(&rev);
	ఒకవేళ (do_usage)
		usage_with_options(git_stash_show_usage, options);
	తిరిగి ret;
usage:
	do_usage = 1;
	వెళ్ళు cleanup;
}

స్థిర పూర్ణాంకం do_store_stash(స్థిరరాశి నిర్మాణం object_id *w_commit, స్థిరరాశి అక్షరం *stash_సందేశం,
			  పూర్ణాంకం quiet)
{
	నిర్మాణం stash_info info;
	అక్షరం revision[GIT_MAX_HEXSZ];

	oid_గమ్యం_hex_r(revision, w_commit);
	assert_stash_like(&info, revision);

	ఒకవేళ (!stash_సందేశం)
		stash_సందేశం = "Created via \"git stash store\".";

	ఒకవేళ (refs_update_ref(get_main_ref_store(the_repository), stash_సందేశం, ref_stash, w_commit, ఖాళీ,
			    REF_FORCE_CREATE_REFLOG,
			    quiet ? UPDATE_REFS_QUIET_ON_ERR :
			    UPDATE_REFS_సందేశం_ON_ERR)) {
		ఒకవేళ (!quiet) {
			fprintf_ln(stderr, _("Cannot update %s with %s"),
				   ref_stash, oid_గమ్యం_hex(w_commit));
		}
		తిరిగి -1;
	}

	తిరిగి 0;
}

స్థిర పూర్ణాంకం store_stash(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
		       నిర్మాణం repository *repo వాడని)
{
	పూర్ణాంకం quiet = 0;
	స్థిరరాశి అక్షరం *stash_సందేశం = ఖాళీ;
	నిర్మాణం object_id obj;
	నిర్మాణం option options[] = {
		OPT__QUIET(&quiet, N_("be quiet")),
		OPT_STRING('m', "message", &stash_సందేశం, "message",
			   N_("stash message")),
		OPT_చివర()
	};
	పూర్ణాంకం ret;

	argc = parse_options(argc, argv, ఉపసర్గ, options,
			     git_stash_store_usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT);

	ఒకవేళ (argc != 1) {
		ఒకవేళ (!quiet)
			fprintf_ln(stderr, _("\"git stash store\" requires one "
					     "<commit> argument"));
		తిరిగి -1;
	}

	ఒకవేళ (repo_get_oid_with_flags(the_repository, argv[0], &obj,
				    quiet ? GET_OID_QUIETLY : 0)) {
		ఒకవేళ (!quiet)
			fprintf_ln(stderr, _("Cannot update %s with %s"),
					     ref_stash, argv[0]);
		ret = -1;
		వెళ్ళు బహిర్గతం;
	}

	ret = do_store_stash(&obj, stash_సందేశం, quiet);

బహిర్గతం:
	తిరిగి ret;
}

స్థిర శూన్య కూడు_pathspecs(నిర్మాణం strvec *వాదనలు,
			  స్థిరరాశి నిర్మాణం pathspec *ps) {
	పూర్ణాంకం i;

	కొరకు (i = 0; i < ps->nr; i++)
		strvec_నెట్టు(వాదనలు, ps->వస్తువులు[i].original);
}

/*
 * `untracked_files` will be filled with the names of untracked files.
 * The return value is:
 *
 * = 0 if there are not any untracked files
 * > 0 if there are untracked files
 */
స్థిర పూర్ణాంకం get_untracked_files(స్థిరరాశి నిర్మాణం pathspec *ps, పూర్ణాంకం include_untracked,
			       నిర్మాణం strbuf *untracked_files)
{
	పూర్ణాంకం i;
	పూర్ణాంకం దొరికింది_2 = 0;
	నిర్మాణం dir_struct dir = DIR_INIT;

	ఒకవేళ (include_untracked != INCLUDE_ALL_FILES)
		అమరిక_standard_excludes(&dir);

	fill_directory(&dir, the_repository->సూచీ, ps);
	కొరకు (i = 0; i < dir.nr; i++) {
		నిర్మాణం dir_నమోదు *ent = dir.entries[i];
		దొరికింది_2++;
		strbuf_addstr(untracked_files, ent->name);
		/* NUL-terminate: will be fed to update-index -z */
		strbuf_addch(untracked_files, '\0');
	}

	dir_clear(&dir);
	తిరిగి దొరికింది_2;
}

/*
 * The return value of `check_changes_tracked_files()` can be:
 *
 * < 0 if there was an error
 * = 0 if there are no changes.
 * > 0 if there are changes.
 */
స్థిర పూర్ణాంకం check_changes_tracked_files(స్థిరరాశి నిర్మాణం pathspec *ps)
{
	నిర్మాణం rev_info rev;
	నిర్మాణం object_id dummy;
	పూర్ణాంకం ret = 0;

	/* No initial commit. */
	ఒకవేళ (repo_get_oid(the_repository, "HEAD", &dummy))
		తిరిగి -1;

	ఒకవేళ (repo_read_సూచీ(the_repository) < 0)
		తిరిగి -1;

	repo_init_revisions(the_repository, &rev, ఖాళీ);
	copy_pathspec(&rev.prune_దత్తాంశం, ps);

	rev.diffopt.flags.శీఘ్ర = 1;
	rev.diffopt.flags.ignore_submodules = 1;
	rev.abbrev = 0;

	కూడు_తల_గమ్యం_pending(&rev);
	diff_అమరిక_పూర్తి(&rev.diffopt);

	నిరంతరం_diff_సూచీ(&rev, DIFF_సూచీ_CACHED);
	ఒకవేళ (diff_ఫలితం_సంకేతం(&rev)) {
		ret = 1;
		వెళ్ళు పూర్తి;
	}

	నిరంతరం_diff_files(&rev, 0);
	ఒకవేళ (diff_ఫలితం_సంకేతం(&rev)) {
		ret = 1;
		వెళ్ళు పూర్తి;
	}

పూర్తి:
	release_revisions(&rev);
	తిరిగి ret;
}

/*
 * The function will fill `untracked_files` with the names of untracked files
 * It will return 1 if there were any changes and 0 if there were not.
 */
స్థిర పూర్ణాంకం check_changes(స్థిరరాశి నిర్మాణం pathspec *ps, పూర్ణాంకం include_untracked,
			 నిర్మాణం strbuf *untracked_files)
{
	పూర్ణాంకం ret = 0;
	ఒకవేళ (check_changes_tracked_files(ps))
		ret = 1;

	ఒకవేళ (include_untracked && get_untracked_files(ps, include_untracked,
						     untracked_files))
		ret = 1;

	తిరిగి ret;
}

స్థిర పూర్ణాంకం save_untracked_files(నిర్మాణం stash_info *info, నిర్మాణం strbuf *సందేశం,
				నిర్మాణం strbuf *files)
{
	పూర్ణాంకం ret = 0;
	నిర్మాణం strbuf untracked_సందేశం = STRBUF_INIT;
	నిర్మాణం బిడ్డ_process cp_upd_సూచీ = బిడ్డ_PROCESS_INIT;
	నిర్మాణం సూచీ_state istate = సూచీ_STATE_INIT(the_repository);

	cp_upd_సూచీ.git_cmd = 1;
	strvec_pushl(&cp_upd_సూచీ.వాదనలు, "update-index", "-z", "--add",
		     "--remove", "--stdin", ఖాళీ);
	strvec_pushf(&cp_upd_సూచీ.env, "GIT_INDEX_FILE=%s",
			 stash_సూచీ_మార్గం.buf);

	strbuf_addf(&untracked_సందేశం, "untracked files on %s\n", సందేశం->buf);
	ఒకవేళ (pipe_command(&cp_upd_సూచీ, files->buf, files->len, ఖాళీ, 0,
			 ఖాళీ, 0)) {
		ret = -1;
		వెళ్ళు పూర్తి;
	}

	ఒకవేళ (write_సూచీ_as_వృక్షం(&info->u_వృక్షం, &istate, stash_సూచీ_మార్గం.buf, 0,
				ఖాళీ)) {
		ret = -1;
		వెళ్ళు పూర్తి;
	}

	ఒకవేళ (commit_వృక్షం(untracked_సందేశం.buf, untracked_సందేశం.len,
			&info->u_వృక్షం, ఖాళీ, &info->u_commit, ఖాళీ, ఖాళీ)) {
		ret = -1;
		వెళ్ళు పూర్తి;
	}

పూర్తి:
	release_సూచీ(&istate);
	strbuf_release(&untracked_సందేశం);
	remove_మార్గం(stash_సూచీ_మార్గం.buf);
	తిరిగి ret;
}

స్థిర పూర్ణాంకం stash_staged(నిర్మాణం stash_info *info, నిర్మాణం strbuf *బహిర్గతం_patch,
			పూర్ణాంకం quiet)
{
	పూర్ణాంకం ret = 0;
	నిర్మాణం బిడ్డ_process cp_diff_వృక్షం = బిడ్డ_PROCESS_INIT;
	నిర్మాణం సూచీ_state istate = సూచీ_STATE_INIT(the_repository);

	ఒకవేళ (write_సూచీ_as_వృక్షం(&info->w_వృక్షం, &istate, the_repository->సూచీ_file,
				0, ఖాళీ)) {
		ret = -1;
		వెళ్ళు పూర్తి;
	}

	cp_diff_వృక్షం.git_cmd = 1;
	strvec_pushl(&cp_diff_వృక్షం.వాదనలు, "diff-tree", "-p", "--binary",
		     "--no-color",
		     "-U1", "HEAD", oid_గమ్యం_hex(&info->w_వృక్షం), "--", ఖాళీ);
	ఒకవేళ (pipe_command(&cp_diff_వృక్షం, ఖాళీ, 0, బహిర్గతం_patch, 0, ఖాళీ, 0)) {
		ret = -1;
		వెళ్ళు పూర్తి;
	}

	ఒకవేళ (!బహిర్గతం_patch->len) {
		ఒకవేళ (!quiet)
			fprintf_ln(stderr, _("No staged changes"));
		ret = 1;
	}

పూర్తి:
	release_సూచీ(&istate);
	తిరిగి ret;
}

స్థిర పూర్ణాంకం stash_patch(నిర్మాణం stash_info *info, స్థిరరాశి నిర్మాణం pathspec *ps,
		       నిర్మాణం strbuf *బహిర్గతం_patch, పూర్ణాంకం quiet,
		       నిర్మాణం interactive_options *interactive_opts)
{
	పూర్ణాంకం ret = 0;
	నిర్మాణం బిడ్డ_process cp_diff_వృక్షం = బిడ్డ_PROCESS_INIT;
	నిర్మాణం commit *తల_commit;
	స్థిరరాశి నిర్మాణం object_id *తల_వృక్షం;
	నిర్మాణం సూచీ_state istate = సూచీ_STATE_INIT(the_repository);
	అక్షరం *old_సూచీ_env = ఖాళీ, *old_repo_సూచీ_file;

	remove_మార్గం(stash_సూచీ_మార్గం.buf);

	తల_commit = lookup_commit(the_repository, &info->b_commit);
	ఒకవేళ (!తల_commit || repo_parse_commit(the_repository, తల_commit)) {
		ret = -1;
		వెళ్ళు పూర్తి;
	}
	తల_వృక్షం = get_commit_వృక్షం_oid(తల_commit);
	ఒకవేళ (!తల_వృక్షం) {
		ret = -1;
		వెళ్ళు పూర్తి;
	}

	ఒకవేళ (create_సూచీ_from_వృక్షం(తల_వృక్షం, stash_సూచీ_మార్గం.buf)) {
		ret = -1;
		వెళ్ళు పూర్తి;
	}

	/* Find out what the user wants. */
	old_repo_సూచీ_file = the_repository->సూచీ_file;
	the_repository->సూచీ_file = stash_సూచీ_మార్గం.buf;
	old_సూచీ_env = xstrdup_or_null(getenv(సూచీ_ENVIRONMENT));
	setenv(సూచీ_ENVIRONMENT, the_repository->సూచీ_file, 1);

	ret = !!నిరంతరం_కూడు_p(the_repository, కూడు_P_STASH, interactive_opts, ఖాళీ, ps, 0);

	the_repository->సూచీ_file = old_repo_సూచీ_file;
	ఒకవేళ (old_సూచీ_env && *old_సూచీ_env)
		setenv(సూచీ_ENVIRONMENT, old_సూచీ_env, 1);
	లేకపోతే
		unsetenv(సూచీ_ENVIRONMENT);
	FREE_AND_NULL(old_సూచీ_env);

	/* State of the working tree. */
	ఒకవేళ (write_సూచీ_as_వృక్షం(&info->w_వృక్షం, &istate, stash_సూచీ_మార్గం.buf, 0,
				ఖాళీ)) {
		ret = -1;
		వెళ్ళు పూర్తి;
	}

	cp_diff_వృక్షం.git_cmd = 1;
	strvec_pushl(&cp_diff_వృక్షం.వాదనలు, "diff-tree", "-p", "-U1", "HEAD",
		     "--no-color",
		     oid_గమ్యం_hex(&info->w_వృక్షం), "--", ఖాళీ);
	ఒకవేళ (pipe_command(&cp_diff_వృక్షం, ఖాళీ, 0, బహిర్గతం_patch, 0, ఖాళీ, 0)) {
		ret = -1;
		వెళ్ళు పూర్తి;
	}

	ఒకవేళ (!బహిర్గతం_patch->len) {
		ఒకవేళ (!quiet)
			fprintf_ln(stderr, _("No changes selected"));
		ret = 1;
	}

పూర్తి:
	release_సూచీ(&istate);
	remove_మార్గం(stash_సూచీ_మార్గం.buf);
	తిరిగి ret;
}

స్థిర పూర్ణాంకం stash_working_వృక్షం(నిర్మాణం stash_info *info, స్థిరరాశి నిర్మాణం pathspec *ps)
{
	పూర్ణాంకం ret = 0;
	నిర్మాణం rev_info rev;
	నిర్మాణం బిడ్డ_process cp_upd_సూచీ = బిడ్డ_PROCESS_INIT;
	నిర్మాణం strbuf diff_output = STRBUF_INIT;
	నిర్మాణం సూచీ_state istate = సూచీ_STATE_INIT(the_repository);

	repo_init_revisions(the_repository, &rev, ఖాళీ);
	copy_pathspec(&rev.prune_దత్తాంశం, ps);

	సమితి_alternate_సూచీ_output(stash_సూచీ_మార్గం.buf);
	ఒకవేళ (reset_వృక్షం(&info->i_వృక్షం, 0, 0)) {
		ret = -1;
		వెళ్ళు పూర్తి;
	}
	సమితి_alternate_సూచీ_output(ఖాళీ);

	rev.diffopt.output_format = DIFF_FORMAT_CALLBACK;
	rev.diffopt.format_callback = కూడు_diff_గమ్యం_buf;
	rev.diffopt.format_callback_దత్తాంశం = &diff_output;

	ఒకవేళ (repo_read_సూచీ_preload(the_repository, &rev.diffopt.pathspec, 0) < 0) {
		ret = -1;
		వెళ్ళు పూర్తి;
	}

	కూడు_pending_object(&rev, parse_object(the_repository, &info->b_commit),
			   "");
	నిరంతరం_diff_సూచీ(&rev, 0);

	cp_upd_సూచీ.git_cmd = 1;
	strvec_pushl(&cp_upd_సూచీ.వాదనలు, "update-index",
		     "--ignore-skip-worktree-entries",
		     "-z", "--add", "--remove", "--stdin", ఖాళీ);
	strvec_pushf(&cp_upd_సూచీ.env, "GIT_INDEX_FILE=%s",
		     stash_సూచీ_మార్గం.buf);

	ఒకవేళ (pipe_command(&cp_upd_సూచీ, diff_output.buf, diff_output.len,
			 ఖాళీ, 0, ఖాళీ, 0)) {
		ret = -1;
		వెళ్ళు పూర్తి;
	}

	ఒకవేళ (write_సూచీ_as_వృక్షం(&info->w_వృక్షం, &istate, stash_సూచీ_మార్గం.buf, 0,
				ఖాళీ)) {
		ret = -1;
		వెళ్ళు పూర్తి;
	}

పూర్తి:
	release_సూచీ(&istate);
	release_revisions(&rev);
	strbuf_release(&diff_output);
	remove_మార్గం(stash_సూచీ_మార్గం.buf);
	తిరిగి ret;
}

స్థిర పూర్ణాంకం do_create_stash(స్థిరరాశి నిర్మాణం pathspec *ps, నిర్మాణం strbuf *stash_సందేశం_buf,
			   పూర్ణాంకం include_untracked, పూర్ణాంకం patch_బహుళకం,
			   నిర్మాణం interactive_options *interactive_opts,
			   పూర్ణాంకం only_staged, నిర్మాణం stash_info *info, నిర్మాణం strbuf *patch,
			   పూర్ణాంకం quiet)
{
	పూర్ణాంకం ret = 0;
	పూర్ణాంకం flags = 0;
	పూర్ణాంకం untracked_commit_option = 0;
	స్థిరరాశి అక్షరం *తల_short_sha1 = ఖాళీ;
	స్థిరరాశి అక్షరం *branch_ref = ఖాళీ;
	స్థిరరాశి అక్షరం *branch_name = "(no branch)";
	అక్షరం *branch_name_buf = ఖాళీ;
	నిర్మాణం commit *తల_commit = ఖాళీ;
	నిర్మాణం commit_list *parents = ఖాళీ;
	నిర్మాణం strbuf సందేశం = STRBUF_INIT;
	నిర్మాణం strbuf commit_వృక్షం_label = STRBUF_INIT;
	నిర్మాణం strbuf untracked_files = STRBUF_INIT;

	prepare_fallback_ident("git stash", "git@stash");

	repo_read_సూచీ_preload(the_repository, ఖాళీ, 0);
	ఒకవేళ (repo_refresh_and_write_సూచీ(the_repository, REFRESH_QUIET, 0, 0,
					 ఖాళీ, ఖాళీ, ఖాళీ) < 0) {
		ret = error(_("could not write index"));
		వెళ్ళు పూర్తి;
	}

	ఒకవేళ (repo_get_oid(the_repository, "HEAD", &info->b_commit)) {
		ఒకవేళ (!quiet)
			fprintf_ln(stderr, _("You do not have "
					     "the initial commit yet"));
		ret = -1;
		వెళ్ళు పూర్తి;
	} లేకపోతే {
		తల_commit = lookup_commit(the_repository, &info->b_commit);
	}

	ఒకవేళ (!check_changes(ps, include_untracked, &untracked_files)) {
		ret = 1;
		వెళ్ళు పూర్తి;
	}

	branch_ref = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
					     "HEAD", 0, ఖాళీ, &flags);

	ఒకవేళ (flags & REF_ISSYMREF) {
		ఒకవేళ (skip_ఉపసర్గ(branch_ref, "refs/heads/", &branch_name))
			branch_name = branch_name_buf = xstrdup(branch_name);
	}

	తల_short_sha1 = repo_కనుగొను_unique_abbrev(the_repository,
						  &తల_commit->object.oid,
						  DEFAULT_ABBREV);
	strbuf_addf(&సందేశం, "%s: %s ", branch_name, తల_short_sha1);
	pp_commit_easy(CMIT_FMT_ONELINE, తల_commit, &సందేశం);

	strbuf_addf(&commit_వృక్షం_label, "index on %s\n", సందేశం.buf);
	commit_list_చొప్పించు(తల_commit, &parents);
	ఒకవేళ (write_సూచీ_as_వృక్షం(&info->i_వృక్షం, the_repository->సూచీ,
				repo_get_సూచీ_file(the_repository), 0, ఖాళీ) ||
	    commit_వృక్షం(commit_వృక్షం_label.buf, commit_వృక్షం_label.len,
			&info->i_వృక్షం, parents, &info->i_commit, ఖాళీ, ఖాళీ)) {
		ఒకవేళ (!quiet)
			fprintf_ln(stderr, _("Cannot save the current "
					     "index state"));
		ret = -1;
		వెళ్ళు పూర్తి;
	}

	commit_list_free(parents);
	parents = ఖాళీ;

	ఒకవేళ (include_untracked) {
		ఒకవేళ (save_untracked_files(info, &సందేశం, &untracked_files)) {
			ఒకవేళ (!quiet)
				fprintf_ln(stderr, _("Cannot save "
						     "the untracked files"));
			ret = -1;
			వెళ్ళు పూర్తి;
		}
		untracked_commit_option = 1;
	}
	ఒకవేళ (patch_బహుళకం) {
		ret = stash_patch(info, ps, patch, quiet, interactive_opts);
		ఒకవేళ (ret < 0) {
			ఒకవేళ (!quiet)
				fprintf_ln(stderr, _("Cannot save the current "
						     "worktree state"));
			వెళ్ళు పూర్తి;
		} లేకపోతే ఒకవేళ (ret > 0) {
			వెళ్ళు పూర్తి;
		}
	} లేకపోతే ఒకవేళ (only_staged) {
		ret = stash_staged(info, patch, quiet);
		ఒకవేళ (ret < 0) {
			ఒకవేళ (!quiet)
				fprintf_ln(stderr, _("Cannot save the current "
						     "staged state"));
			వెళ్ళు పూర్తి;
		} లేకపోతే ఒకవేళ (ret > 0) {
			వెళ్ళు పూర్తి;
		}
	} లేకపోతే {
		ఒకవేళ (stash_working_వృక్షం(info, ps)) {
			ఒకవేళ (!quiet)
				fprintf_ln(stderr, _("Cannot save the current "
						     "worktree state"));
			ret = -1;
			వెళ్ళు పూర్తి;
		}
	}

	ఒకవేళ (!stash_సందేశం_buf->len)
		strbuf_addf(stash_సందేశం_buf, "WIP on %s", సందేశం.buf);
	లేకపోతే
		strbuf_insertf(stash_సందేశం_buf, 0, "On %s: ", branch_name);

	ఒకవేళ (untracked_commit_option)
		commit_list_చొప్పించు(lookup_commit(the_repository,
						 &info->u_commit),
				   &parents);
	commit_list_చొప్పించు(lookup_commit(the_repository, &info->i_commit),
			   &parents);
	commit_list_చొప్పించు(తల_commit, &parents);

	ఒకవేళ (commit_వృక్షం(stash_సందేశం_buf->buf, stash_సందేశం_buf->len, &info->w_వృక్షం,
			parents, &info->w_commit, ఖాళీ, ఖాళీ)) {
		ఒకవేళ (!quiet)
			fprintf_ln(stderr, _("Cannot record "
					     "working tree state"));
		ret = -1;
		వెళ్ళు పూర్తి;
	}

పూర్తి:
	strbuf_release(&commit_వృక్షం_label);
	strbuf_release(&సందేశం);
	strbuf_release(&untracked_files);
	commit_list_free(parents);
	విడుదల(branch_name_buf);
	తిరిగి ret;
}

స్థిర పూర్ణాంకం create_stash(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ వాడని,
			నిర్మాణం repository *repo వాడని)
{
	పూర్ణాంకం ret;
	నిర్మాణం strbuf stash_సందేశం_buf = STRBUF_INIT;
	నిర్మాణం stash_info info = STASH_INFO_INIT;
	నిర్మాణం pathspec ps;

	/* Starting with argv[1], since argv[0] is "create" */
	strbuf_join_argv(&stash_సందేశం_buf, argc - 1, ++argv, ' ');

	memset(&ps, 0, పరిమాణం(ps));
	ఒకవేళ (!check_changes_tracked_files(&ps))
		తిరిగి 0;

	ret = do_create_stash(&ps, &stash_సందేశం_buf, 0, 0, ఖాళీ, 0, &info,
			      ఖాళీ, 0);
	ఒకవేళ (!ret)
		printf_ln("%s", oid_గమ్యం_hex(&info.w_commit));

	free_stash_info(&info);
	strbuf_release(&stash_సందేశం_buf);
	తిరిగి ret;
}

స్థిర పూర్ణాంకం do_నెట్టు_stash(స్థిరరాశి నిర్మాణం pathspec *ps, స్థిరరాశి అక్షరం *stash_సందేశం, పూర్ణాంకం quiet,
			 పూర్ణాంకం keep_సూచీ, పూర్ణాంకం patch_బహుళకం,
			 నిర్మాణం interactive_options *interactive_opts,
			 పూర్ణాంకం include_untracked, పూర్ణాంకం only_staged)
{
	పూర్ణాంకం ret = 0;
	నిర్మాణం stash_info info = STASH_INFO_INIT;
	నిర్మాణం strbuf patch = STRBUF_INIT;
	నిర్మాణం strbuf stash_సందేశం_buf = STRBUF_INIT;
	నిర్మాణం strbuf untracked_files = STRBUF_INIT;
	నిర్మాణం strbuf బహిర్గతం = STRBUF_INIT;

	ఒకవేళ (patch_బహుళకం && keep_సూచీ == -1)
		keep_సూచీ = 1;

	ఒకవేళ (patch_బహుళకం && include_untracked) {
		fprintf_ln(stderr, _("Can't use --patch and --include-untracked"
				     " or --all at the same time"));
		ret = -1;
		వెళ్ళు పూర్తి;
	}

	/* --patch overrides --staged */
	ఒకవేళ (patch_బహుళకం)
		only_staged = 0;

	ఒకవేళ (only_staged && include_untracked) {
		fprintf_ln(stderr, _("Can't use --staged and --include-untracked"
				     " or --all at the same time"));
		ret = -1;
		వెళ్ళు పూర్తి;
	}

	repo_read_సూచీ_preload(the_repository, ఖాళీ, 0);
	ఒకవేళ (!include_untracked && ps->nr) {
		అక్షరం *ps_matched = xcalloc(ps->nr, 1);

		ఒకవేళ (pathspec_needs_expanded_సూచీ(the_repository->సూచీ, ps))
			ensure_నిండిన_సూచీ(the_repository->సూచీ);
		కొరకు (కొలత_t i = 0; i < the_repository->సూచీ->cache_nr; i++)
			ce_మార్గం_సరిపోలిక(the_repository->సూచీ, the_repository->సూచీ->cache[i], ps,
				      ps_matched);

		ఒకవేళ (report_మార్గం_error(ps_matched, ps)) {
			fprintf_ln(stderr, _("Did you forget to 'git add'?"));
			ret = -1;
			విడుదల(ps_matched);
			వెళ్ళు పూర్తి;
		}
		విడుదల(ps_matched);
	}

	ఒకవేళ (repo_refresh_and_write_సూచీ(the_repository, REFRESH_QUIET, 0, 0,
					 ఖాళీ, ఖాళీ, ఖాళీ)) {
		ret = error(_("could not write index"));
		వెళ్ళు పూర్తి;
	}

	ఒకవేళ (!check_changes(ps, include_untracked, &untracked_files)) {
		ఒకవేళ (!quiet)
			printf_ln(_("No local changes to save"));
		వెళ్ళు పూర్తి;
	}

	ఒకవేళ (!refs_reflog_exists(get_main_ref_store(the_repository), ref_stash) && do_clear_stash()) {
		ret = -1;
		ఒకవేళ (!quiet)
			fprintf_ln(stderr, _("Cannot initialize stash"));
		వెళ్ళు పూర్తి;
	}

	ఒకవేళ (stash_సందేశం)
		strbuf_addstr(&stash_సందేశం_buf, stash_సందేశం);
	ఒకవేళ (do_create_stash(ps, &stash_సందేశం_buf, include_untracked, patch_బహుళకం,
			    interactive_opts, only_staged, &info, &patch, quiet)) {
		ret = -1;
		వెళ్ళు పూర్తి;
	}

	ఒకవేళ (do_store_stash(&info.w_commit, stash_సందేశం_buf.buf, 1)) {
		ret = -1;
		ఒకవేళ (!quiet)
			fprintf_ln(stderr, _("Cannot save the current status"));
		వెళ్ళు పూర్తి;
	}

	ఒకవేళ (!quiet)
		printf_ln(_("Saved working directory and index state %s"),
			  stash_సందేశం_buf.buf);

	ఒకవేళ (!(patch_బహుళకం || only_staged)) {
		ఒకవేళ (include_untracked && !ps->nr) {
			నిర్మాణం బిడ్డ_process cp = బిడ్డ_PROCESS_INIT;

			cp.git_cmd = 1;
			ఒకవేళ (startup_info->original_cwd) {
				cp.dir = startup_info->original_cwd;
				strvec_pushf(&cp.env, "%s=%s",
					     GIT_WORK_వృక్షం_ENVIRONMENT,
					     the_repository->worktree);
			}
			strvec_pushl(&cp.వాదనలు, "clean", "--force",
				     "--quiet", "-d", ":/", ఖాళీ);
			ఒకవేళ (include_untracked == INCLUDE_ALL_FILES)
				strvec_నెట్టు(&cp.వాదనలు, "-x");
			ఒకవేళ (నిరంతరం_command(&cp)) {
				ret = -1;
				వెళ్ళు పూర్తి;
			}
		}
		discard_సూచీ(the_repository->సూచీ);
		ఒకవేళ (ps->nr) {
			నిర్మాణం బిడ్డ_process cp_కూడు = బిడ్డ_PROCESS_INIT;
			నిర్మాణం బిడ్డ_process cp_diff = బిడ్డ_PROCESS_INIT;
			నిర్మాణం బిడ్డ_process cp_apply = బిడ్డ_PROCESS_INIT;

			cp_కూడు.git_cmd = 1;
			strvec_నెట్టు(&cp_కూడు.వాదనలు, "add");
			ఒకవేళ (!include_untracked)
				strvec_నెట్టు(&cp_కూడు.వాదనలు, "-u");
			ఒకవేళ (include_untracked == INCLUDE_ALL_FILES)
				strvec_నెట్టు(&cp_కూడు.వాదనలు, "--force");
			strvec_నెట్టు(&cp_కూడు.వాదనలు, "--");
			కూడు_pathspecs(&cp_కూడు.వాదనలు, ps);
			ఒకవేళ (నిరంతరం_command(&cp_కూడు)) {
				ret = -1;
				వెళ్ళు పూర్తి;
			}

			cp_diff.git_cmd = 1;
			strvec_pushl(&cp_diff.వాదనలు, "diff-index", "-p",
				     "--no-color",
				     "--cached", "--binary", "HEAD", "--",
				     ఖాళీ);
			కూడు_pathspecs(&cp_diff.వాదనలు, ps);
			ఒకవేళ (pipe_command(&cp_diff, ఖాళీ, 0, &బహిర్గతం, 0, ఖాళీ, 0)) {
				ret = -1;
				వెళ్ళు పూర్తి;
			}

			cp_apply.git_cmd = 1;
			strvec_pushl(&cp_apply.వాదనలు, "apply", "--index",
				     "-R", ఖాళీ);
			ఒకవేళ (pipe_command(&cp_apply, బహిర్గతం.buf, బహిర్గతం.len, ఖాళీ, 0,
					 ఖాళీ, 0)) {
				ret = -1;
				వెళ్ళు పూర్తి;
			}
		} లేకపోతే {
			నిర్మాణం బిడ్డ_process cp = బిడ్డ_PROCESS_INIT;
			cp.git_cmd = 1;
			/* BUG: this nukes untracked files in the way */
			strvec_pushl(&cp.వాదనలు, "reset", "--hard", "-q",
				     "--no-recurse-submodules", ఖాళీ);
			ఒకవేళ (నిరంతరం_command(&cp)) {
				ret = -1;
				వెళ్ళు పూర్తి;
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
		ఒకవేళ (keep_సూచీ == 1 && !is_null_oid(&info.i_వృక్షం) &&
		    !is_ఖాళీలు_వృక్షం_oid(&info.i_వృక్షం, the_repository->hash_algo)) {
			నిర్మాణం బిడ్డ_process cp = బిడ్డ_PROCESS_INIT;

			cp.git_cmd = 1;
			strvec_pushl(&cp.వాదనలు, "checkout", "--no-overlay",
				     oid_గమ్యం_hex(&info.i_వృక్షం), "--", ఖాళీ);
			ఒకవేళ (!ps->nr)
				strvec_నెట్టు(&cp.వాదనలు, ":/");
			లేకపోతే
				కూడు_pathspecs(&cp.వాదనలు, ps);
			ఒకవేళ (నిరంతరం_command(&cp)) {
				ret = -1;
				వెళ్ళు పూర్తి;
			}
		}
		వెళ్ళు పూర్తి;
	} లేకపోతే {
		నిర్మాణం బిడ్డ_process cp = బిడ్డ_PROCESS_INIT;

		cp.git_cmd = 1;
		strvec_pushl(&cp.వాదనలు, "apply", "-R", ఖాళీ);

		ఒకవేళ (pipe_command(&cp, patch.buf, patch.len, ఖాళీ, 0, ఖాళీ, 0)) {
			ఒకవేళ (!quiet)
				fprintf_ln(stderr, _("Cannot remove "
						     "worktree changes"));
			ret = -1;
			వెళ్ళు పూర్తి;
		}

		ఒకవేళ (keep_సూచీ < 1) {
			నిర్మాణం బిడ్డ_process cp = బిడ్డ_PROCESS_INIT;

			cp.git_cmd = 1;
			strvec_pushl(&cp.వాదనలు, "reset", "-q", "--refresh", "--",
				     ఖాళీ);
			కూడు_pathspecs(&cp.వాదనలు, ps);
			ఒకవేళ (నిరంతరం_command(&cp)) {
				ret = -1;
				వెళ్ళు పూర్తి;
			}
		}
		వెళ్ళు పూర్తి;
	}

పూర్తి:
	strbuf_release(&patch);
	strbuf_release(&బహిర్గతం);
	free_stash_info(&info);
	strbuf_release(&stash_సందేశం_buf);
	strbuf_release(&untracked_files);
	తిరిగి ret;
}

స్థిర పూర్ణాంకం నెట్టు_stash(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
		      పూర్ణాంకం నెట్టు_assumed)
{
	పూర్ణాంకం force_assume = 0;
	పూర్ణాంకం keep_సూచీ = -1;
	పూర్ణాంకం only_staged = 0;
	పూర్ణాంకం patch_బహుళకం = 0;
	పూర్ణాంకం include_untracked = 0;
	పూర్ణాంకం quiet = 0;
	పూర్ణాంకం pathspec_file_nul = 0;
	స్థిరరాశి అక్షరం *stash_సందేశం = ఖాళీ;
	అక్షరం *pathspec_from_file = ఖాళీ;
	నిర్మాణం pathspec ps;
	నిర్మాణం interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	నిర్మాణం option options[] = {
		OPT_BOOL('k', "keep-index", &keep_సూచీ,
			 N_("keep index")),
		OPT_BOOL('S', "staged", &only_staged,
			 N_("stash staged changes only")),
		OPT_BOOL('p', "patch", &patch_బహుళకం,
			 N_("stash in patch mode")),
		OPT_BOOL(0, "auto-advance", &interactive_opts.auto_advance,
			 N_("auto advance to the next file when selecting hunks interactively")),
		OPT_DIFF_UNIFIED(&interactive_opts.context),
		OPT_DIFF_INTERHUNK_CONTEXT(&interactive_opts.interhunkcontext),
		OPT__QUIET(&quiet, N_("quiet mode")),
		OPT_BOOL('u', "include-untracked", &include_untracked,
			 N_("include untracked files in stash")),
		OPT_సమితి_INT('a', "all", &include_untracked,
			    N_("include ignore files"), INCLUDE_ALL_FILES),
		OPT_STRING('m', "message", &stash_సందేశం, N_("message"),
			   N_("stash message")),
		OPT_PATHSPEC_FROM_FILE(&pathspec_from_file),
		OPT_PATHSPEC_FILE_NUL(&pathspec_file_nul),
		OPT_చివర()
	};
	పూర్ణాంకం ret;

	ఒకవేళ (argc) {
		పూర్ణాంకం flags = PARSE_OPT_KEEP_DASHDASH;

		ఒకవేళ (నెట్టు_assumed)
			flags |= PARSE_OPT_STOP_AT_NON_OPTION;

		argc = parse_options(argc, argv, ఉపసర్గ, options,
				     నెట్టు_assumed ? git_stash_usage :
				     git_stash_నెట్టు_usage, flags);
		force_assume |= patch_బహుళకం;
	}

	ఒకవేళ (argc) {
		ఒకవేళ (!strcmp(argv[0], "--")) {
			argc--;
			argv++;
		} లేకపోతే ఒకవేళ (నెట్టు_assumed && !force_assume) {
			die("subcommand wasn't specified; 'push' can't be assumed due to unexpected token '%s'",
			    argv[0]);
		}
	}

	parse_pathspec(&ps, 0, PATHSPEC_PREFER_నిండిన | PATHSPEC_ఉపసర్గ_ORIGIN,
		       ఉపసర్గ, argv);

	ఒకవేళ (pathspec_from_file) {
		ఒకవేళ (patch_బహుళకం)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--patch");

		ఒకవేళ (only_staged)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--staged");

		ఒకవేళ (ps.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&ps, 0,
				    PATHSPEC_PREFER_నిండిన | PATHSPEC_ఉపసర్గ_ORIGIN,
				    ఉపసర్గ, pathspec_from_file, pathspec_file_nul);
	} లేకపోతే ఒకవేళ (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	ఒకవేళ (!patch_బహుళకం) {
		ఒకవేళ (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--patch");
		ఒకవేళ (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--patch");
		ఒకవేళ (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--patch");
	}

	ఒకవేళ (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	ఒకవేళ (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	ret = do_నెట్టు_stash(&ps, stash_సందేశం, quiet, keep_సూచీ, patch_బహుళకం,
			    &interactive_opts, include_untracked, only_staged);

	clear_pathspec(&ps);
	విడుదల(pathspec_from_file);
	తిరిగి ret;
}

స్థిర పూర్ణాంకం నెట్టు_stash_unassumed(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
				నిర్మాణం repository *repo వాడని)
{
	తిరిగి నెట్టు_stash(argc, argv, ఉపసర్గ, 0);
}

స్థిర పూర్ణాంకం save_stash(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
		      నిర్మాణం repository *repo వాడని)
{
	పూర్ణాంకం keep_సూచీ = -1;
	పూర్ణాంకం only_staged = 0;
	పూర్ణాంకం patch_బహుళకం = 0;
	పూర్ణాంకం include_untracked = 0;
	పూర్ణాంకం quiet = 0;
	పూర్ణాంకం ret = 0;
	స్థిరరాశి అక్షరం *stash_సందేశం = ఖాళీ;
	నిర్మాణం pathspec ps;
	నిర్మాణం strbuf stash_సందేశం_buf = STRBUF_INIT;
	నిర్మాణం interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	నిర్మాణం option options[] = {
		OPT_BOOL('k', "keep-index", &keep_సూచీ,
			 N_("keep index")),
		OPT_BOOL('S', "staged", &only_staged,
			 N_("stash staged changes only")),
		OPT_BOOL('p', "patch", &patch_బహుళకం,
			 N_("stash in patch mode")),
		OPT_BOOL(0, "auto-advance", &interactive_opts.auto_advance,
			 N_("auto advance to the next file when selecting hunks interactively")),
		OPT_DIFF_UNIFIED(&interactive_opts.context),
		OPT_DIFF_INTERHUNK_CONTEXT(&interactive_opts.interhunkcontext),
		OPT__QUIET(&quiet, N_("quiet mode")),
		OPT_BOOL('u', "include-untracked", &include_untracked,
			 N_("include untracked files in stash")),
		OPT_సమితి_INT('a', "all", &include_untracked,
			    N_("include ignore files"), INCLUDE_ALL_FILES),
		OPT_STRING('m', "message", &stash_సందేశం, "message",
			   N_("stash message")),
		OPT_చివర()
	};

	argc = parse_options(argc, argv, ఉపసర్గ, options,
			     git_stash_save_usage,
			     PARSE_OPT_KEEP_DASHDASH);

	ఒకవేళ (argc)
		stash_సందేశం = strbuf_join_argv(&stash_సందేశం_buf, argc, argv, ' ');

	memset(&ps, 0, పరిమాణం(ps));

	ఒకవేళ (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	ఒకవేళ (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	ఒకవేళ (!patch_బహుళకం) {
		ఒకవేళ (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--patch");
		ఒకవేళ (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--patch");
		ఒకవేళ (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--patch");
	}

	ret = do_నెట్టు_stash(&ps, stash_సందేశం, quiet, keep_సూచీ,
			    patch_బహుళకం, &interactive_opts, include_untracked,
			    only_staged);

	strbuf_release(&stash_సందేశం_buf);
	తిరిగి ret;
}

స్థిర పూర్ణాంకం write_commit_with_parents(నిర్మాణం repository *r,
				     నిర్మాణం object_id *బహిర్గతం,
				     స్థిరరాశి నిర్మాణం object_id *oid,
				     నిర్మాణం commit_list *parents)
{
	కొలత_t author_len, committer_len;
	నిర్మాణం commit *this;
	స్థిరరాశి అక్షరం *orig_author, *orig_committer;
	అక్షరం *author = ఖాళీ, *committer = ఖాళీ;
	స్థిరరాశి అక్షరం *బఫరు;
	గుర్తులేని దీర్ఘ bufsize;
	స్థిరరాశి అక్షరం *p;
	నిర్మాణం strbuf సందేశం = STRBUF_INIT;
	పూర్ణాంకం ret = 0;
	నిర్మాణం ident_విభజించు id;

	this = lookup_commit_reference(r, oid);
	బఫరు = repo_get_commit_బఫరు(r, this, &bufsize);
	orig_author = కనుగొను_commit_శీర్షిక(బఫరు, "author", &author_len);
	orig_committer = కనుగొను_commit_శీర్షిక(బఫరు, "committer", &committer_len);

	ఒకవేళ (!orig_author || !orig_committer) {
		ret = error(_("cannot parse commit %s"), oid_గమ్యం_hex(oid));
		వెళ్ళు బహిర్గతం;
	}

	ఒకవేళ (విభజించు_ident_పంక్తి(&id, orig_author, author_len) < 0 ||
	    విభజించు_ident_పంక్తి(&id, orig_committer, committer_len) < 0) {
		ret = error(_("invalid author or committer for %s"), oid_గమ్యం_hex(oid));
		వెళ్ళు బహిర్గతం;
	}

	p = అక్షరశ్రేణి_శోధన(బఫరు, "\n\n");
	strbuf_addstr(&సందేశం, "git stash: ");

	ఒకవేళ (p)
		strbuf_కూడు(&సందేశం, p + 2, bufsize - (p + 2 - బఫరు));
	strbuf_complete_పంక్తి(&సందేశం);

	author = xmemdupz(orig_author, author_len);
	committer = xmemdupz(orig_committer, committer_len);

	ఒకవేళ (commit_వృక్షం_extended(సందేశం.buf, సందేశం.len,
				 r->hash_algo->ఖాళీలు_వృక్షం, parents,
				 బహిర్గతం, author, committer,
				 ఖాళీ, ఖాళీ)) {
		ret = error(_("could not write commit"));
		వెళ్ళు బహిర్గతం;
	}
బహిర్గతం:
	strbuf_release(&సందేశం);
	repo_unuse_commit_బఫరు(r, this, బఫరు);
	విడుదల(author);
	విడుదల(committer);
	తిరిగి ret;
}

స్థిర పూర్ణాంకం do_import_stash(నిర్మాణం repository *r, స్థిరరాశి అక్షరం *rev)
{
	నిర్మాణం object_id chain;
	పూర్ణాంకం res = 0;
	స్థిరరాశి అక్షరం *బఫరు = ఖాళీ;
	గుర్తులేని దీర్ఘ bufsize;
	నిర్మాణం commit *this = ఖాళీ;
	నిర్మాణం commit_list *వస్తువులు = ఖాళీ, *cur;
	అక్షరం *సందేశం = ఖాళీ;

	ఒకవేళ (repo_get_oid(r, rev, &chain))
		తిరిగి error(_("not a valid revision: %s"), rev);

	this = lookup_commit_reference(r, &chain);
	ఒకవేళ (!this)
		తిరిగి error(_("not a commit: %s"), rev);

	/*
	 * Walk the commit history, finding each stash entry, and load data into
	 * the array.
	 */
	కొరకు (;;) {
		స్థిరరాశి అక్షరం *author, *committer;
		కొలత_t author_len, committer_len;
		స్థిరరాశి అక్షరం *p;
		స్థిరరాశి అక్షరం *expected = "git stash <git@stash> 1000684800 +0000";
		స్థిరరాశి అక్షరం *ఉపసర్గ = "git stash: ";
		నిర్మాణం commit *stash;
		నిర్మాణం వృక్షం *వృక్షం = repo_get_commit_వృక్షం(r, this);

		ఒకవేళ (!వృక్షం ||
		    !oideq(&వృక్షం->object.oid, r->hash_algo->ఖాళీలు_వృక్షం) ||
		    (this->parents &&
		     (!this->parents->తదుపరి || this->parents->తదుపరి->తదుపరి))) {
			res = error(_("%s is not a valid exported stash commit"),
					oid_గమ్యం_hex(&this->object.oid));
			వెళ్ళు బహిర్గతం;
		}

		బఫరు = repo_get_commit_బఫరు(r, this, &bufsize);

		ఒకవేళ (!this->parents) {
			/*
			 * We don't have any parents.  Make sure this is our
			 * root commit.
			 */
			author = కనుగొను_commit_శీర్షిక(బఫరు, "author", &author_len);
			committer = కనుగొను_commit_శీర్షిక(బఫరు, "committer", &committer_len);

			ఒకవేళ (!author || !committer) {
				error(_("cannot parse commit %s"), oid_గమ్యం_hex(&this->object.oid));
				వెళ్ళు బహిర్గతం;
			}

			ఒకవేళ (author_len != అక్షరశ్రేణి_పొడవు(expected) ||
			    committer_len != అక్షరశ్రేణి_పొడవు(expected) ||
			    memcmp(author, expected, author_len) ||
			    memcmp(committer, expected, committer_len)) {
				res = error(_("found root commit %s with invalid data"), oid_గమ్యం_hex(&this->object.oid));
				వెళ్ళు బహిర్గతం;
			}
			ఆపు;
		}

		p = అక్షరశ్రేణి_శోధన(బఫరు, "\n\n");
		ఒకవేళ (!p) {
			res = error(_("cannot parse commit %s"), oid_గమ్యం_hex(&this->object.oid));
			వెళ్ళు బహిర్గతం;
		}

		p += 2;
		ఒకవేళ (((కొలత_t)(bufsize - (p - బఫరు)) < అక్షరశ్రేణి_పొడవు(ఉపసర్గ)) ||
		    memcmp(ఉపసర్గ, p, అక్షరశ్రేణి_పొడవు(ఉపసర్గ))) {
			res = error(_("found stash commit %s without expected prefix"), oid_గమ్యం_hex(&this->object.oid));
			వెళ్ళు బహిర్గతం;
		}

		stash = this->parents->తదుపరి->అంశం;

		ఒకవేళ (repo_parse_commit(r, this->parents->అంశం) ||
		    repo_parse_commit(r, stash)) {
			res = error(_("cannot parse parents of commit: %s"),
					oid_గమ్యం_hex(&this->object.oid));
			వెళ్ళు బహిర్గతం;
		}

		ఒకవేళ (check_stash_topology(r, stash)) {
			res = error(_("%s does not look like a stash commit"),
					oid_గమ్యం_hex(&stash->object.oid));
			వెళ్ళు బహిర్గతం;
		}

		repo_unuse_commit_బఫరు(r, this, బఫరు);
		బఫరు = ఖాళీ;
		వస్తువులు = commit_list_చొప్పించు(stash, &వస్తువులు);
		this = this->parents->అంశం;
	}

	/*
	 * Now, walk each entry, adding it to the stash as a normal stash
	 * commit.
	 */
	కొరకు (cur = వస్తువులు; cur; cur = cur->తదుపరి) {
		స్థిరరాశి అక్షరం *p;
		నిర్మాణం object_id *oid;

		this = cur->అంశం;
		oid = &this->object.oid;
		బఫరు = repo_get_commit_బఫరు(r, this, &bufsize);
		ఒకవేళ (!బఫరు) {
			res = error(_("cannot read commit buffer for %s"), oid_గమ్యం_hex(oid));
			వెళ్ళు బహిర్గతం;
		}

		p = అక్షరశ్రేణి_శోధన(బఫరు, "\n\n");
		ఒకవేళ (!p) {
			res = error(_("cannot parse commit %s"), oid_గమ్యం_hex(oid));
			వెళ్ళు బహిర్గతం;
		}

		p += 2;
		సందేశం = xmemdupz(p, bufsize - (p - బఫరు));
		repo_unuse_commit_బఫరు(r, this, బఫరు);
		బఫరు = ఖాళీ;

		ఒకవేళ (do_store_stash(oid, సందేశం, 1)) {
			res = error(_("cannot save the stash for %s"), oid_గమ్యం_hex(oid));
			వెళ్ళు బహిర్గతం;
		}
		FREE_AND_NULL(సందేశం);
	}
బహిర్గతం:
	ఒకవేళ (this && బఫరు)
		repo_unuse_commit_బఫరు(r, this, బఫరు);
	commit_list_free(వస్తువులు);
	విడుదల(సందేశం);

	తిరిగి res;
}

స్థిర పూర్ణాంకం import_stash(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
			నిర్మాణం repository *repo)
{
	నిర్మాణం option options[] = {
		OPT_చివర()
	};

	argc = parse_options(argc, argv, ఉపసర్గ, options,
			     git_stash_import_usage,
			     PARSE_OPT_KEEP_DASHDASH);

	ఒకవేళ (argc != 1)
		usage_సందేశం_opt("a revision is required", git_stash_import_usage, options);

	తిరిగి do_import_stash(repo, argv[0]);
}

నిర్మాణం stash_నమోదు_దత్తాంశం {
	నిర్మాణం repository *r;
	నిర్మాణం commit_list **వస్తువులు;
	కొలత_t లెక్క;
};

స్థిర పూర్ణాంకం collect_stash_entries(స్థిరరాశి అక్షరం *refname వాడని,
				 నిర్మాణం object_id *old_oid వాడని,
				 నిర్మాణం object_id *new_oid,
				 స్థిరరాశి అక్షరం *committer వాడని,
				 timestamp_t timestamp వాడని,
				 పూర్ణాంకం tz వాడని, స్థిరరాశి అక్షరం *సందేశం వాడని,
				 శూన్య *cb_దత్తాంశం)
{
	నిర్మాణం stash_నమోదు_దత్తాంశం *దత్తాంశం = cb_దత్తాంశం;
	నిర్మాణం commit *stash;

	దత్తాంశం->లెక్క++;
	stash = lookup_commit_reference(దత్తాంశం->r, new_oid);
	ఒకవేళ (!stash || check_stash_topology(దత్తాంశం->r, stash)) {
		తిరిగి error(_("%s does not look like a stash commit"),
			     oid_గమ్యం_hex(new_oid));
	}
	దత్తాంశం->వస్తువులు = commit_list_append(stash, దత్తాంశం->వస్తువులు);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం do_export_stash(నిర్మాణం repository *r,
			   స్థిరరాశి అక్షరం *ref,
			   పూర్ణాంకం argc,
			   స్థిరరాశి అక్షరం **argv)
{
	నిర్మాణం object_id పునాది;
	నిర్మాణం commit *prev;
	నిర్మాణం commit_list *వస్తువులు = ఖాళీ, **iter = &వస్తువులు, *cur;
	పూర్ణాంకం res = 0;
	పూర్ణాంకం i;
	నిర్మాణం strbuf revision = STRBUF_INIT;
	స్థిరరాశి అక్షరం *author, *committer;

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
	ఒకవేళ (commit_వృక్షం_extended("", 0, r->hash_algo->ఖాళీలు_వృక్షం, ఖాళీ,
				 &పునాది, author, committer, ఖాళీ, ఖాళీ))
		తిరిగి error(_("unable to write base commit"));

	prev = lookup_commit_reference(r, &పునాది);

	ఒకవేళ (argc) {
		/*
		 * Find each specified stash, and load data into the array.
		 */
		కొరకు (i = 0; i < argc; i++) {
			నిర్మాణం object_id oid;
			నిర్మాణం commit *stash;

			ఒకవేళ (parse_stash_revision(&revision, argv[i], 1) ||
			    repo_get_oid_with_flags(r, revision.buf, &oid,
						    GET_OID_QUIETLY |
						    GET_OID_GENTLY)) {
				res = error(_("unable to find stash entry %s"), argv[i]);
				వెళ్ళు బహిర్గతం;
			}

			stash = lookup_commit_reference(r, &oid);
			ఒకవేళ (!stash || check_stash_topology(r, stash)) {
				res = error(_("%s does not look like a stash commit"),
						revision.buf);
				వెళ్ళు బహిర్గతం;
			}
			iter = commit_list_append(stash, iter);
		}
	} లేకపోతే {
		/*
		 * Walk the reflog, finding each stash entry, and load data into the
		 * array.
		 */
		నిర్మాణం stash_నమోదు_దత్తాంశం cb_దత్తాంశం = {
			.r = r, .వస్తువులు = iter,
		};
		ఒకవేళ (refs_for_each_reflog_ent_తిరగేయి(get_main_ref_store(r),
						     "refs/stash",
						     collect_stash_entries,
						     &cb_దత్తాంశం) && cb_దత్తాంశం.లెక్క)
			వెళ్ళు బహిర్గతం;
	}

	/*
	 * Now, create a set of commits identical to the regular stash commits,
	 * but where their first parents form a chain to our original empty
	 * base commit.
	 */
	వస్తువులు = commit_list_తిరగేయి(వస్తువులు);
	కొరకు (cur = వస్తువులు; cur; cur = cur->తదుపరి) {
		నిర్మాణం commit_list *parents = ఖాళీ;
		నిర్మాణం commit_list **తదుపరి = &parents;
		నిర్మాణం object_id బహిర్గతం;
		నిర్మాణం commit *stash = cur->అంశం;

		తదుపరి = commit_list_append(prev, తదుపరి);
		తదుపరి = commit_list_append(stash, తదుపరి);
		res = write_commit_with_parents(r, &బహిర్గతం, &stash->object.oid, parents);
		commit_list_free(parents);
		ఒకవేళ (res)
			వెళ్ళు బహిర్గతం;
		prev = lookup_commit_reference(r, &బహిర్గతం);
	}
	ఒకవేళ (ref)
		refs_update_ref(get_main_ref_store(r), ఖాళీ, ref,
				&prev->object.oid, ఖాళీ, 0, UPDATE_REFS_DIE_ON_ERR);
	లేకపోతే
		puts(oid_గమ్యం_hex(&prev->object.oid));
బహిర్గతం:
	strbuf_release(&revision);
	commit_list_free(వస్తువులు);

	తిరిగి res;
}

గణన export_action {
	ACTION_NONE,
	ACTION_PRINT,
	ACTION_గమ్యం_REF,
};

స్థిర పూర్ణాంకం export_stash(పూర్ణాంకం argc,
			స్థిరరాశి అక్షరం **argv,
			స్థిరరాశి అక్షరం *ఉపసర్గ,
			నిర్మాణం repository *repo)
{
	స్థిరరాశి అక్షరం *ref = ఖాళీ;
	గణన export_action action = ACTION_NONE;
	నిర్మాణం option options[] = {
		OPT_CMDMODE(0, "print", &action,
			    N_("print the object ID instead of writing it to a ref"),
			    ACTION_PRINT),
		OPT_STRING(0, "to-ref", &ref, "ref",
			    N_("save the data to the given ref")),
		OPT_చివర()
	};

	argc = parse_options(argc, argv, ఉపసర్గ, options,
			     git_stash_export_usage,
			     PARSE_OPT_KEEP_DASHDASH);

	ఒకవేళ (ref && action == ACTION_NONE)
		action = ACTION_గమ్యం_REF;

	ఒకవేళ (action == ACTION_NONE || (ref && action == ACTION_PRINT))
		తిరిగి error(_("exactly one of --print and --to-ref is required"));

	తిరిగి do_export_stash(repo, ref, argc, argv);
}

పూర్ణాంకం cmd_stash(పూర్ణాంకం argc,
	      స్థిరరాశి అక్షరం **argv,
	      స్థిరరాశి అక్షరం *ఉపసర్గ,
	      నిర్మాణం repository *repo)
{
	pid_t pid = getpid();
	స్థిరరాశి అక్షరం *సూచీ_file;
	నిర్మాణం strvec వాదనలు = STRVEC_INIT;
	parse_opt_subcommand_fn *fn = ఖాళీ;
	నిర్మాణం option options[] = {
		OPT_SUBCOMMAND("apply", &fn, apply_stash),
		OPT_SUBCOMMAND("clear", &fn, clear_stash),
		OPT_SUBCOMMAND("drop", &fn, drop_stash),
		OPT_SUBCOMMAND("pop", &fn, తీయి_stash),
		OPT_SUBCOMMAND("branch", &fn, branch_stash),
		OPT_SUBCOMMAND("list", &fn, list_stash),
		OPT_SUBCOMMAND("show", &fn, show_stash),
		OPT_SUBCOMMAND("store", &fn, store_stash),
		OPT_SUBCOMMAND("create", &fn, create_stash),
		OPT_SUBCOMMAND("push", &fn, నెట్టు_stash_unassumed),
		OPT_SUBCOMMAND("export", &fn, export_stash),
		OPT_SUBCOMMAND("import", &fn, import_stash),
		OPT_SUBCOMMAND_F("save", &fn, save_stash, ఖాళీ, PARSE_OPT_NOCOMPLETE),
		OPT_చివర()
	};
	స్థిరరాశి అక్షరం **వాదనలు_copy;
	పూర్ణాంకం ret;

	repo_config(the_repository, git_stash_config, ఖాళీ);

	argc = parse_options(argc, argv, ఉపసర్గ, options, git_stash_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL |
			     PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_నిండిన_సూచీ = 0;

	సూచీ_file = repo_get_సూచీ_file(the_repository);
	strbuf_addf(&stash_సూచీ_మార్గం, "%s.stash.%" PRIuMAX, సూచీ_file,
		    (uintmax_t)pid);

	ఒకవేళ (fn) {
		ret = fn(argc, argv, ఉపసర్గ, repo);

		/*
		 * The subcommand implementations return 0 on success, a
		 * negative value on failure, and STASH_APPLY_CONFLICT
		 * when applying a stash entry resulted in conflicts.
		 * Map failures to 128, the status die() uses, so that
		 * exit status 1 unambiguously indicates conflicts.
		 */
		ఒకవేళ (ret < 0)
			తిరిగి 128;
		తిరిగి ret;
	} లేకపోతే ఒకవేళ (!argc) {
		తిరిగి !!నెట్టు_stash_unassumed(0, ఖాళీ, ఉపసర్గ, repo);
	}

	/* Assume 'stash push' */
	strvec_నెట్టు(&వాదనలు, "push");
	strvec_pushv(&వాదనలు, argv);

	/*
	 * `push_stash()` ends up modifying the array, which causes memory
	 * leaks if we didn't copy the array here.
	 */
	DUP_ARRAY(వాదనలు_copy, వాదనలు.v, వాదనలు.nr);

	ret = !!నెట్టు_stash(వాదనలు.nr, వాదనలు_copy, ఉపసర్గ, 1);

	strvec_clear(&వాదనలు);
	విడుదల(వాదనలు_copy);
	తిరిగి ret;
}
