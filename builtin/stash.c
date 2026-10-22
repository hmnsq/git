#define USE_THE_REPOSITORY_VARIABLE

#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "abspath.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "hash.h"
#അന്തര്ഭാവ "hex.h"
#അന്തര്ഭാവ "object-name.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "refs.h"
#അന്തര്ഭാവ "stash.h"
#അന്തര്ഭാവ "lockfile.h"
#അന്തര്ഭാവ "cache-tree.h"
#അന്തര്ഭാവ "unpack-trees.h"
#അന്തര്ഭാവ "merge-ort-wrappers.h"
#അന്തര്ഭാവ "strvec.h"
#അന്തര്ഭാവ "run-command.h"
#അന്തര്ഭാവ "dir.h"
#അന്തര്ഭാവ "entry.h"
#അന്തര്ഭാവ "preload-index.h"
#അന്തര്ഭാവ "read-cache.h"
#അന്തര്ഭാവ "repository.h"
#അന്തര്ഭാവ "rerere.h"
#അന്തര്ഭാവ "revision.h"
#അന്തര്ഭാവ "setup.h"
#അന്തര്ഭാവ "sparse-index.h"
#അന്തര്ഭാവ "log-tree.h"
#അന്തര്ഭാവ "diffcore.h"
#അന്തര്ഭാവ "reflog.h"
#അന്തര്ഭാവ "reflog-walk.h"
#അന്തര്ഭാവ "add-interactive.h"
#അന്തര്ഭാവ "commit.h"

#define INCLUDE_ALL_FILES 2

#define BUILTIN_STASH_LIST_USAGE \
	N_("git stash list [<log-options>]")
#define BUILTIN_STASH_SHOW_USAGE \
	N_("git stash show [-u | --include-untracked | --only-untracked] [<diff-options>] [<stash>]")
#define BUILTIN_STASH_DROP_USAGE \
	N_("git stash drop [-q | --quiet] [<stash>]")
#define BUILTIN_STASH_എടുക്കുക_USAGE \
	N_("git stash pop [--index] [-q | --quiet] [<stash>]")
#define BUILTIN_STASH_APPLY_USAGE \
	N_("git stash apply [--index] [-q | --quiet] [--label-ours=<label>] [--label-theirs=<label>] [--label-base=<label>] [<stash>]")
#define BUILTIN_STASH_BRANCH_USAGE \
	N_("git stash branch <branchname> [<stash>]")
#define BUILTIN_STASH_STORE_USAGE \
	N_("git stash store [(-m | --message) <message>] [-q | --quiet] <commit>")
#define BUILTIN_STASH_തള്ളുക_USAGE \
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

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം git_stash_usage[] = {
	BUILTIN_STASH_LIST_USAGE,
	BUILTIN_STASH_SHOW_USAGE,
	BUILTIN_STASH_DROP_USAGE,
	BUILTIN_STASH_എടുക്കുക_USAGE,
	BUILTIN_STASH_APPLY_USAGE,
	BUILTIN_STASH_BRANCH_USAGE,
	BUILTIN_STASH_തള്ളുക_USAGE,
	BUILTIN_STASH_SAVE_USAGE,
	BUILTIN_STASH_CLEAR_USAGE,
	BUILTIN_STASH_CREATE_USAGE,
	BUILTIN_STASH_STORE_USAGE,
	BUILTIN_STASH_EXPORT_USAGE,
	BUILTIN_STASH_IMPORT_USAGE,
	ശൂന്യ
};

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം git_stash_list_usage[] = {
	BUILTIN_STASH_LIST_USAGE,
	ശൂന്യ
};

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം git_stash_show_usage[] = {
	BUILTIN_STASH_SHOW_USAGE,
	ശൂന്യ
};

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം git_stash_drop_usage[] = {
	BUILTIN_STASH_DROP_USAGE,
	ശൂന്യ
};

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം git_stash_എടുക്കുക_usage[] = {
	BUILTIN_STASH_എടുക്കുക_USAGE,
	ശൂന്യ
};

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം git_stash_apply_usage[] = {
	BUILTIN_STASH_APPLY_USAGE,
	ശൂന്യ
};

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം git_stash_branch_usage[] = {
	BUILTIN_STASH_BRANCH_USAGE,
	ശൂന്യ
};

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം git_stash_clear_usage[] = {
	BUILTIN_STASH_CLEAR_USAGE,
	ശൂന്യ
};

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം git_stash_store_usage[] = {
	BUILTIN_STASH_STORE_USAGE,
	ശൂന്യ
};

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം git_stash_തള്ളുക_usage[] = {
	BUILTIN_STASH_തള്ളുക_USAGE,
	ശൂന്യ
};

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം git_stash_save_usage[] = {
	BUILTIN_STASH_SAVE_USAGE,
	ശൂന്യ
};

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം git_stash_export_usage[] = {
	BUILTIN_STASH_EXPORT_USAGE,
	ശൂന്യ
};

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം git_stash_import_usage[] = {
	BUILTIN_STASH_IMPORT_USAGE,
	ശൂന്യ
};

സ്ഥിതം സ്ഥിരം അക്ഷരം ref_stash[] = "refs/stash";
സ്ഥിതം ഘടന strbuf stash_index_path = STRBUF_INIT;

സ്ഥിതം പൂർണ്ണസംഖ്യ show_stat = 1;
സ്ഥിതം പൂർണ്ണസംഖ്യ show_patch;
സ്ഥിതം പൂർണ്ണസംഖ്യ show_include_untracked;
സ്ഥിതം പൂർണ്ണസംഖ്യ use_index;

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
ഘടന stash_info {
	ഘടന object_id w_commit;
	ഘടന object_id b_commit;
	ഘടന object_id i_commit;
	ഘടന object_id u_commit;
	ഘടന object_id c_commit;
	ഘടന object_id w_വൃക്ഷം;
	ഘടന object_id b_വൃക്ഷം;
	ഘടന object_id i_വൃക്ഷം;
	ഘടന object_id u_വൃക്ഷം;
	ഘടന strbuf revision;
	പൂർണ്ണസംഖ്യ is_stash_ref;
	പൂർണ്ണസംഖ്യ has_u;
};

#define STASH_INFO_INIT { \
	.revision = STRBUF_INIT, \
}

സ്ഥിതം ശൂന്യം free_stash_info(ഘടന stash_info *info)
{
	strbuf_release(&info->revision);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ check_stash_topology(ഘടന repository *r, ഘടന commit *stash)
{
	ഘടന commit *p1, *p2, *p3 = ശൂന്യ;

	/* stash must have two or three parents */
	എങ്കിൽ (!stash->parents || !stash->parents->next ||
			(stash->parents->next->next && stash->parents->next->next->next))
		തിരികെ -1;
	p1 = stash->parents->item;
	p2 = stash->parents->next->item;
	എങ്കിൽ (stash->parents->next->next)
		p3 = stash->parents->next->next->item;
	എങ്കിൽ (repo_parse_commit(r, p1) || repo_parse_commit(r, p2) ||
			(p3 && repo_parse_commit(r, p3)))
		തിരികെ -1;
	/* p2 must have a single parent, p3 must have no parents */
	എങ്കിൽ (!p2->parents || p2->parents->next || (p3 && p3->parents))
		തിരികെ -1;
	എങ്കിൽ (repo_parse_commit(r, p2->parents->item))
		തിരികെ -1;
	/* p2^1 must equal p1 */
	എങ്കിൽ (!oideq(&p1->object.oid, &p2->parents->item->object.oid))
		തിരികെ -1;

	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം assert_stash_like(ഘടന stash_info *info, സ്ഥിരം അക്ഷരം *revision)
{
	എങ്കിൽ (get_oidf(&info->b_commit, "%s^1", revision) ||
	    get_oidf(&info->w_വൃക്ഷം, "%s:", revision) ||
	    get_oidf(&info->b_വൃക്ഷം, "%s^1:", revision) ||
	    get_oidf(&info->i_വൃക്ഷം, "%s^2:", revision))
		die(_("'%s' is not a stash-like commit"), revision);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ parse_stash_revision(ഘടന strbuf *revision, സ്ഥിരം അക്ഷരം *commit, പൂർണ്ണസംഖ്യ quiet)
{
	strbuf_reset(revision);
	എങ്കിൽ (!commit) {
		എങ്കിൽ (!refs_ref_exists(get_main_ref_store(the_repository), ref_stash)) {
			എങ്കിൽ (!quiet)
				fprintf_ln(stderr, _("No stash entries found."));
			തിരികെ -1;
		}

		strbuf_addf(revision, "%s@{0}", ref_stash);
	} അല്ലെങ്കിൽ എങ്കിൽ (strspn(commit, "0123456789") == strlen(commit)) {
		strbuf_addf(revision, "%s@{%s}", ref_stash, commit);
	} അല്ലെങ്കിൽ {
		strbuf_addstr(revision, commit);
	}
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ get_stash_info(ഘടന stash_info *info, പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv)
{
	പൂർണ്ണസംഖ്യ ret;
	അക്ഷരം *end_of_rev;
	അക്ഷരം *expanded_ref;
	സ്ഥിരം അക്ഷരം *revision;
	സ്ഥിരം അക്ഷരം *commit = ശൂന്യ;
	ഘടന object_id dummy;
	ഘടന strbuf symbolic = STRBUF_INIT;

	എങ്കിൽ (argc > 1) {
		പൂർണ്ണസംഖ്യ i;
		ഘടന strbuf refs_സന്ദേശം = STRBUF_INIT;

		വേണ്ടി (i = 0; i < argc; i++)
			strbuf_addf(&refs_സന്ദേശം, " '%s'", argv[i]);

		fprintf_ln(stderr, _("Too many revisions specified:%s"),
			   refs_സന്ദേശം.buf);
		strbuf_release(&refs_സന്ദേശം);

		തിരികെ -1;
	}

	എങ്കിൽ (argc == 1)
		commit = argv[0];

	strbuf_init(&info->revision, 0);
	എങ്കിൽ (parse_stash_revision(&info->revision, commit, 0)) {
		തിരികെ -1;
	}

	revision = info->revision.buf;

	എങ്കിൽ (repo_get_oid(the_repository, revision, &info->w_commit))
		തിരികെ error(_("%s is not a valid reference"), revision);

	assert_stash_like(info, revision);

	info->has_u = !get_oidf(&info->u_വൃക്ഷം, "%s^3:", revision);

	end_of_rev = strchrnul(revision, '@');
	strbuf_add(&symbolic, revision, end_of_rev - revision);

	ret = repo_dwim_ref(the_repository, symbolic.buf, symbolic.len,
			    &dummy, &expanded_ref, 0);
	strbuf_release(&symbolic);
	തിരഞ്ഞെടുപ്പ് (ret) {
	അവസ്ഥ 0: /* Not found, but valid ref */
		info->is_stash_ref = 0;
		നിർത്തുക;
	അവസ്ഥ 1:
		info->is_stash_ref = !strcmp(expanded_ref, ref_stash);
		നിർത്തുക;
	സ്ഥിരസ്ഥിതി: /* Invalid or ambiguous */
		നിർത്തുക;
	}

	സ്വതന്ത്രം(expanded_ref);
	തിരികെ !(ret == 0 || ret == 1);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ do_clear_stash(ശൂന്യം)
{
	ഘടന object_id obj;
	എങ്കിൽ (repo_get_oid(the_repository, ref_stash, &obj))
		തിരികെ 0;

	തിരികെ refs_delete_ref(get_main_ref_store(the_repository), ശൂന്യ,
			       ref_stash, &obj, 0);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ clear_stash(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
		       ഘടന repository *repo UNUSED)
{
	ഘടന option options[] = {
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_clear_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	എങ്കിൽ (argc)
		തിരികെ error(_("git stash clear with arguments is "
			       "unimplemented"));

	തിരികെ do_clear_stash();
}

സ്ഥിതം പൂർണ്ണസംഖ്യ reset_വൃക്ഷം(ഘടന object_id *i_വൃക്ഷം, പൂർണ്ണസംഖ്യ update, പൂർണ്ണസംഖ്യ reset)
{
	പൂർണ്ണസംഖ്യ nr_trees = 1;
	ഘടന unpack_trees_options opts;
	ഘടന വൃക്ഷം_desc t[MAX_UNPACK_TREES];
	ഘടന വൃക്ഷം *വൃക്ഷം;
	ഘടന lock_file lock_file = LOCK_INIT;

	repo_read_index_preload(the_repository, ശൂന്യ, 0);
	എങ്കിൽ (refresh_index(the_repository->index, REFRESH_QUIET, ശൂന്യ, ശൂന്യ, ശൂന്യ))
		തിരികെ -1;

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);

	memset(&opts, 0, വലുപ്പം(opts));

	വൃക്ഷം = repo_parse_വൃക്ഷം_indirect(the_repository, i_വൃക്ഷം);
	എങ്കിൽ (repo_parse_വൃക്ഷം(the_repository, വൃക്ഷം))
		തിരികെ -1;

	init_വൃക്ഷം_desc(t, &വൃക്ഷം->object.oid, വൃക്ഷം->buffer, വൃക്ഷം->size);

	opts.head_idx = 1;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;
	opts.merge = 1;
	opts.reset = reset ? UNPACK_RESET_PROTECT_UNTRACKED : 0;
	opts.update = update;
	എങ്കിൽ (update)
		opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
	opts.fn = oneway_merge;

	എങ്കിൽ (unpack_trees(nr_trees, t, &opts))
		തിരികെ -1;

	എങ്കിൽ (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
		തിരികെ error(_("unable to write new index file"));

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ create_index_from_വൃക്ഷം(സ്ഥിരം ഘടന object_id *വൃക്ഷം_id,
				  സ്ഥിരം അക്ഷരം *index_path)
{
	പൂർണ്ണസംഖ്യ nr_trees = 1;
	പൂർണ്ണസംഖ്യ ret = 0;
	ഘടന unpack_trees_options opts;
	ഘടന വൃക്ഷം_desc t[MAX_UNPACK_TREES];
	ഘടന വൃക്ഷം *വൃക്ഷം;
	ഘടന index_state dst_istate = INDEX_STATE_INIT(the_repository);
	ഘടന lock_file lock_file = LOCK_INIT;

	repo_read_index_preload(the_repository, ശൂന്യ, 0);
	refresh_index(the_repository->index, REFRESH_QUIET, ശൂന്യ, ശൂന്യ, ശൂന്യ);

	hold_lock_file_for_update(&lock_file, index_path, LOCK_DIE_ON_ERROR);

	memset(&opts, 0, വലുപ്പം(opts));

	വൃക്ഷം = repo_parse_വൃക്ഷം_indirect(the_repository, വൃക്ഷം_id);
	എങ്കിൽ (!വൃക്ഷം || repo_parse_വൃക്ഷം(the_repository, വൃക്ഷം)) {
		ret = -1;
		പോകുക done;
	}

	init_വൃക്ഷം_desc(t, &വൃക്ഷം->object.oid, വൃക്ഷം->buffer, വൃക്ഷം->size);

	opts.head_idx = 1;
	opts.src_index = the_repository->index;
	opts.dst_index = &dst_istate;
	opts.merge = 1;
	opts.reset = UNPACK_RESET_PROTECT_UNTRACKED;
	opts.fn = oneway_merge;

	എങ്കിൽ (unpack_trees(nr_trees, t, &opts)) {
		ret = -1;
		പോകുക done;
	}

	എങ്കിൽ (write_locked_index(&dst_istate, &lock_file, COMMIT_LOCK)) {
		ret = error(_("unable to write new index file"));
		പോകുക done;
	}

done:
	release_index(&dst_istate);
	എങ്കിൽ (ret)
		rollback_lock_file(&lock_file);
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ is_path_a_directory(സ്ഥിരം അക്ഷരം *path)
{
	/*
	 * This function differs from abspath.c:is_directory() in that
	 * here we use lstat() instead of stat(); we do not want to
	 * follow symbolic links here.
	 */
	ഘടന stat st;
	തിരികെ (!lstat(path, &st) && S_ISDIR(st.st_mode));
}

സ്ഥിതം ശൂന്യം add_diff_to_buf(ഘടന diff_queue_struct *q,
			    ഘടന diff_options *options UNUSED,
			    ശൂന്യം *data)
{
	പൂർണ്ണസംഖ്യ i;

	വേണ്ടി (i = 0; i < q->nr; i++) {
		എങ്കിൽ (is_path_a_directory(q->queue[i]->one->path))
			തുടരുക;

		strbuf_addstr(data, q->queue[i]->one->path);

		/* NUL-terminate: will be fed to update-index -z */
		strbuf_addch(data, '\0');
	}
}

സ്ഥിതം പൂർണ്ണസംഖ്യ restore_untracked(ഘടന object_id *u_വൃക്ഷം)
{
	പൂർണ്ണസംഖ്യ res;
	ഘടന child_process cp = CHILD_PROCESS_INIT;

	/*
	 * We need to run restore files from a given index, but without
	 * affecting the current index, so we use GIT_INDEX_FILE with
	 * run_command to fork processes that will not interfere.
	 */
	cp.git_cmd = 1;
	strvec_തള്ളുക(&cp.args, "read-tree");
	strvec_തള്ളുക(&cp.args, oid_to_hex(u_വൃക്ഷം));
	strvec_pushf(&cp.env, "GIT_INDEX_FILE=%s",
		     stash_index_path.buf);
	എങ്കിൽ (run_command(&cp)) {
		remove_path(stash_index_path.buf);
		തിരികെ -1;
	}

	child_process_init(&cp);
	cp.git_cmd = 1;
	strvec_pushl(&cp.args, "checkout-index", "--all", ശൂന്യ);
	strvec_pushf(&cp.env, "GIT_INDEX_FILE=%s",
		     stash_index_path.buf);

	res = run_command(&cp);
	remove_path(stash_index_path.buf);
	തിരികെ res;
}

സ്ഥിതം ശൂന്യം unstage_changes_unless_new(ഘടന object_id *orig_വൃക്ഷം)
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

	ഘടന checkout state = CHECKOUT_INIT;
	ഘടന diff_options diff_opts;
	ഘടന lock_file lock = LOCK_INIT;
	പൂർണ്ണസംഖ്യ i;

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

	do_diff_cache(orig_വൃക്ഷം, &diff_opts);
	diffcore_std(&diff_opts);

	/* Iterate over the paths that changed due to the merge... */
	വേണ്ടി (i = 0; i < diff_queued_diff.nr; i++) {
		ഘടന diff_filepair *p;
		ഘടന cache_entry *ce;
		പൂർണ്ണസംഖ്യ pos;

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
		എങ്കിൽ (pos >= 0 && ce_skip_worktree(the_repository->index->cache[pos])) {
			ഘടന stat st;

			ce = the_repository->index->cache[pos];
			എങ്കിൽ (!lstat(ce->name, &st)) {
				/* Conflicting path present; relocate it */
				ഘടന strbuf new_path = STRBUF_INIT;
				പൂർണ്ണസംഖ്യ fd;

				strbuf_addf(&new_path,
					    "%s.stash.XXXXXX", ce->name);
				fd = xmkstemp(new_path.buf);
				close(fd);
				printf(_("WARNING: Untracked file in way of "
					 "tracked file!  Renaming\n "
					 "           %s -> %s\n"
					 "         to make room.\n"),
				       ce->name, new_path.buf);
				എങ്കിൽ (rename(ce->name, new_path.buf))
					die("Failed to move %s to %s",
					    ce->name, new_path.buf);
				strbuf_release(&new_path);
			}
			checkout_entry(ce, &state, ശൂന്യ, ശൂന്യ);
			ce->ce_flags &= ~CE_SKIP_WORKTREE;
		}

		/*
		 * Step 3: "unstage" changes, as long as they are still tracked
		 */
		എങ്കിൽ (p->one->oid_valid) {
			/*
			 * Path existed in orig_tree; restore index entry
			 * from that tree in order to "unstage" the changes.
			 */
			പൂർണ്ണസംഖ്യ option = ADD_CACHE_OK_TO_REPLACE;
			എങ്കിൽ (pos < 0)
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
	എങ്കിൽ (write_locked_index(the_repository->index, &lock,
			       COMMIT_LOCK | SKIP_IF_UNCHANGED))
		die(_("could not write index"));
}

സ്ഥിതം എണ്ണൽ stash_apply_result do_apply_stash(സ്ഥിരം അക്ഷരം *prefix,
					      ഘടന stash_info *info,
					      പൂർണ്ണസംഖ്യ index, പൂർണ്ണസംഖ്യ quiet,
					      സ്ഥിരം അക്ഷരം *label_ours,
					      സ്ഥിരം അക്ഷരം *label_theirs,
					      സ്ഥിരം അക്ഷരം *label_base)
{
	പൂർണ്ണസംഖ്യ clean, ret;
	പൂർണ്ണസംഖ്യ has_index = index;
	ഘടന merge_options o;
	ഘടന object_id c_വൃക്ഷം;
	ഘടന object_id index_വൃക്ഷം;
	ഘടന വൃക്ഷം *head, *merge, *merge_base;
	ഘടന lock_file lock = LOCK_INIT;

	repo_read_index_preload(the_repository, ശൂന്യ, 0);
	എങ്കിൽ (repo_refresh_and_write_index(the_repository, REFRESH_QUIET, 0, 0,
					 ശൂന്യ, ശൂന്യ, ശൂന്യ))
		തിരികെ error(_("could not write index"));

	എങ്കിൽ (write_index_as_വൃക്ഷം(&c_വൃക്ഷം, the_repository->index,
				repo_get_index_file(the_repository), 0, ശൂന്യ))
		തിരികെ error(_("cannot apply a stash in the middle of a merge"));

	init_ui_merge_options(&o, the_repository);

	എങ്കിൽ (quiet)
		o.verbosity = 0;

	എങ്കിൽ (index) {
		എങ്കിൽ (oideq(&info->b_വൃക്ഷം, &info->i_വൃക്ഷം) ||
		    oideq(&c_വൃക്ഷം, &info->i_വൃക്ഷം)) {
			has_index = 0;
		} അല്ലെങ്കിൽ {
			ഘടന merge_result result = { 0 };

			o.branch1 = "Current index";
			o.branch2 = "Stashed index changes";
			o.ancestor = "Stash base";

			head = lookup_വൃക്ഷം(o.repo, &c_വൃക്ഷം);
			merge = lookup_വൃക്ഷം(o.repo, &info->i_വൃക്ഷം);
			merge_base = lookup_വൃക്ഷം(o.repo, &info->b_വൃക്ഷം);

			merge_incore_nonrecursive(&o, merge_base, head, merge,
						  &result);

			എങ്കിൽ (result.clean < 0) {
				merge_finalize(&o, &result);
				തിരികെ error(_("index merge failed"));
			} അല്ലെങ്കിൽ എങ്കിൽ (!result.clean) {
				merge_finalize(&o, &result);
				തിരികെ error(_("conflicts in index. "
					       "Try without --index."));
			} അല്ലെങ്കിൽ {
				oidcpy(&index_വൃക്ഷം, &result.വൃക്ഷം->object.oid);
				merge_finalize(&o, &result);
			}
		}
	}

	o.branch1 = label_ours ? label_ours : "Updated upstream";
	o.branch2 = label_theirs ? label_theirs : "Stashed changes";
	o.ancestor = label_base ? label_base : "Stash base";

	എങ്കിൽ (oideq(&info->b_വൃക്ഷം, &c_വൃക്ഷം))
		o.branch1 = "Version stash was based on";

	എങ്കിൽ (o.verbosity >= 3)
		printf_ln(_("Merging %s with %s"), o.branch1, o.branch2);

	head = lookup_വൃക്ഷം(o.repo, &c_വൃക്ഷം);
	merge = lookup_വൃക്ഷം(o.repo, &info->w_വൃക്ഷം);
	merge_base = lookup_വൃക്ഷം(o.repo, &info->b_വൃക്ഷം);

	repo_hold_locked_index(o.repo, &lock, LOCK_DIE_ON_ERROR);
	clean = merge_ort_nonrecursive(&o, head, merge, merge_base);

	/*
	 * If 'clean' >= 0, reverse the value for 'ret' so 'ret' is 0 when the
	 * merge was clean, and 1 if the merge was unclean or a negative value
	 * if it encountered an error.
	 */
	ret = clean >= 0 ? !clean : clean;

	എങ്കിൽ (ret < 0)
		rollback_lock_file(&lock);
	അല്ലെങ്കിൽ എങ്കിൽ (write_locked_index(o.repo->index, &lock,
				      COMMIT_LOCK | SKIP_IF_UNCHANGED))
		ret = error(_("could not write index"));

	എങ്കിൽ (ret) {
		repo_rerere(the_repository, 0);

		എങ്കിൽ (index)
			fprintf_ln(stderr, _("Index was not unstashed."));

		പോകുക restore_untracked;
	}

	എങ്കിൽ (has_index) {
		എങ്കിൽ (reset_വൃക്ഷം(&index_വൃക്ഷം, 0, 0))
			ret = -1;
	} അല്ലെങ്കിൽ {
		unstage_changes_unless_new(&c_വൃക്ഷം);
	}

restore_untracked:
	എങ്കിൽ (info->has_u && restore_untracked(&info->u_വൃക്ഷം))
		ret = error(_("could not restore untracked files from stash"));

	എങ്കിൽ (!quiet) {
		ഘടന child_process cp = CHILD_PROCESS_INIT;

		/*
		 * Status is quite simple and could be replaced with calls to
		 * wt_status in the future, but it adds complexities which may
		 * require more tests.
		 */
		cp.git_cmd = 1;
		cp.dir = prefix;
		strvec_pushf(&cp.env, GIT_WORK_വൃക്ഷം_ENVIRONMENT"=%s",
			     absolute_path(repo_get_work_വൃക്ഷം(the_repository)));
		strvec_pushf(&cp.env, GIT_DIR_ENVIRONMENT"=%s",
			     absolute_path(repo_get_git_dir(the_repository)));
		strvec_തള്ളുക(&cp.args, "status");
		run_command(&cp);
	}

	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ apply_stash(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
		       ഘടന repository *repo UNUSED)
{
	പൂർണ്ണസംഖ്യ ret = -1;
	പൂർണ്ണസംഖ്യ quiet = 0;
	പൂർണ്ണസംഖ്യ index = use_index;
	സ്ഥിരം അക്ഷരം *label_ours = ശൂന്യ, *label_theirs = ശൂന്യ, *label_base = ശൂന്യ;
	ഘടന stash_info info = STASH_INFO_INIT;
	ഘടന option options[] = {
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

	എങ്കിൽ (get_stash_info(&info, argc, argv))
		പോകുക cleanup;

	ret = do_apply_stash(prefix, &info, index, quiet,
			     label_ours, label_theirs, label_base);
cleanup:
	free_stash_info(&info);
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ reject_reflog_ent(സ്ഥിരം അക്ഷരം *refname UNUSED,
			     ഘടന object_id *ooid UNUSED,
			     ഘടന object_id *noid UNUSED,
			     സ്ഥിരം അക്ഷരം *email UNUSED,
			     timestamp_t timestamp UNUSED,
			     പൂർണ്ണസംഖ്യ tz UNUSED, സ്ഥിരം അക്ഷരം *message UNUSED,
			     ശൂന്യം *cb_data UNUSED)
{
	തിരികെ 1;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ reflog_is_empty(സ്ഥിരം അക്ഷരം *refname)
{
	തിരികെ !refs_for_each_reflog_ent(get_main_ref_store(the_repository),
					 refname, reject_reflog_ent, ശൂന്യ);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ do_drop_stash(ഘടന stash_info *info, പൂർണ്ണസംഖ്യ quiet)
{
	എങ്കിൽ (!reflog_delete(info->revision.buf,
			   EXPIRE_REFLOGS_REWRITE | EXPIRE_REFLOGS_UPDATE_REF,
			   0)) {
		എങ്കിൽ (!quiet)
			printf_ln(_("Dropped %s (%s)"), info->revision.buf,
				  oid_to_hex(&info->w_commit));
	} അല്ലെങ്കിൽ {
		തിരികെ error(_("%s: Could not drop stash entry"),
			     info->revision.buf);
	}

	എങ്കിൽ (reflog_is_empty(ref_stash))
		do_clear_stash();

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ get_stash_info_assert(ഘടന stash_info *info, പൂർണ്ണസംഖ്യ argc,
				 സ്ഥിരം അക്ഷരം **argv)
{
	പൂർണ്ണസംഖ്യ ret = get_stash_info(info, argc, argv);

	എങ്കിൽ (ret < 0)
		തിരികെ ret;

	എങ്കിൽ (!info->is_stash_ref)
		തിരികെ error(_("'%s' is not a stash reference"), info->revision.buf);

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ drop_stash(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
		      ഘടന repository *repo UNUSED)
{
	പൂർണ്ണസംഖ്യ ret = -1;
	പൂർണ്ണസംഖ്യ quiet = 0;
	ഘടന stash_info info = STASH_INFO_INIT;
	ഘടന option options[] = {
		OPT__QUIET(&quiet, N_("be quiet, only report errors")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_drop_usage, 0);

	എങ്കിൽ (get_stash_info_assert(&info, argc, argv))
		പോകുക cleanup;

	ret = do_drop_stash(&info, quiet);
cleanup:
	free_stash_info(&info);
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ എടുക്കുക_stash(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
		     ഘടന repository *repo UNUSED)
{
	പൂർണ്ണസംഖ്യ ret = -1;
	പൂർണ്ണസംഖ്യ index = use_index;
	പൂർണ്ണസംഖ്യ quiet = 0;
	ഘടന stash_info info = STASH_INFO_INIT;
	ഘടന option options[] = {
		OPT__QUIET(&quiet, N_("be quiet, only report errors")),
		OPT_BOOL(0, "index", &index,
			 N_("attempt to recreate the index")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_എടുക്കുക_usage, 0);

	എങ്കിൽ (get_stash_info_assert(&info, argc, argv))
		പോകുക cleanup;

	എങ്കിൽ ((ret = do_apply_stash(prefix, &info, index, quiet,
				  ശൂന്യ, ശൂന്യ, ശൂന്യ)))
		printf_ln(_("The stash entry is kept in case "
			    "you need it again."));
	അല്ലെങ്കിൽ
		ret = do_drop_stash(&info, quiet);

cleanup:
	free_stash_info(&info);
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ branch_stash(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
			ഘടന repository *repo UNUSED)
{
	പൂർണ്ണസംഖ്യ ret = -1;
	സ്ഥിരം അക്ഷരം *branch = ശൂന്യ;
	ഘടന stash_info info = STASH_INFO_INIT;
	ഘടന child_process cp = CHILD_PROCESS_INIT;
	ഘടന option options[] = {
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_branch_usage, 0);

	എങ്കിൽ (!argc) {
		fprintf_ln(stderr, _("No branch name specified"));
		തിരികെ -1;
	}

	branch = argv[0];

	എങ്കിൽ (get_stash_info(&info, argc - 1, argv + 1))
		പോകുക cleanup;

	cp.git_cmd = 1;
	strvec_pushl(&cp.args, "checkout", "-b", ശൂന്യ);
	strvec_തള്ളുക(&cp.args, branch);
	strvec_തള്ളുക(&cp.args, oid_to_hex(&info.b_commit));
	ret = run_command(&cp);
	എങ്കിൽ (!ret)
		ret = do_apply_stash(prefix, &info, 1, 0,
				     ശൂന്യ, ശൂന്യ, ശൂന്യ);
	എങ്കിൽ (!ret && info.is_stash_ref)
		ret = do_drop_stash(&info, 0);

cleanup:
	free_stash_info(&info);
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ list_stash(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
		      ഘടന repository *repo UNUSED)
{
	ഘടന child_process cp = CHILD_PROCESS_INIT;
	ഘടന option options[] = {
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_list_usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT);

	എങ്കിൽ (!refs_ref_exists(get_main_ref_store(the_repository), ref_stash))
		തിരികെ 0;

	cp.git_cmd = 1;
	strvec_pushl(&cp.args, "log", "--format=%gd: %gs", "-g",
		     "--first-parent", ശൂന്യ);
	strvec_pushv(&cp.args, argv);
	strvec_തള്ളുക(&cp.args, ref_stash);
	strvec_തള്ളുക(&cp.args, "--");
	തിരികെ run_command(&cp);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ git_stash_config(സ്ഥിരം അക്ഷരം *var, സ്ഥിരം അക്ഷരം *മൂല്യം,
			    സ്ഥിരം ഘടന config_context *ctx, ശൂന്യം *cb)
{
	എങ്കിൽ (!strcmp(var, "stash.showstat")) {
		show_stat = git_config_bool(var, മൂല്യം);
		തിരികെ 0;
	}
	എങ്കിൽ (!strcmp(var, "stash.showpatch")) {
		show_patch = git_config_bool(var, മൂല്യം);
		തിരികെ 0;
	}
	എങ്കിൽ (!strcmp(var, "stash.showincludeuntracked")) {
		show_include_untracked = git_config_bool(var, മൂല്യം);
		തിരികെ 0;
	}
	എങ്കിൽ (!strcmp(var, "stash.index")) {
		use_index = git_config_bool(var, മൂല്യം);
		തിരികെ 0;
	}
	തിരികെ git_diff_basic_config(var, മൂല്യം, ctx, cb);
}

സ്ഥിതം ശൂന്യം diff_include_untracked(സ്ഥിരം ഘടന stash_info *info, ഘടന diff_options *diff_opt)
{
	സ്ഥിരം ഘടന object_id *oid[] = { &info->w_commit, &info->u_വൃക്ഷം };
	ഘടന വൃക്ഷം *വൃക്ഷം[ARRAY_SIZE(oid)];
	ഘടന വൃക്ഷം_desc വൃക്ഷം_desc[ARRAY_SIZE(oid)];
	ഘടന unpack_trees_options unpack_വൃക്ഷം_opt = { 0 };

	വേണ്ടി (size_t i = 0; i < ARRAY_SIZE(oid); i++) {
		വൃക്ഷം[i] = repo_parse_വൃക്ഷം_indirect(the_repository, oid[i]);
		എങ്കിൽ (repo_parse_വൃക്ഷം(the_repository, വൃക്ഷം[i]) < 0)
			die(_("failed to parse tree"));
		init_വൃക്ഷം_desc(&വൃക്ഷം_desc[i], &വൃക്ഷം[i]->object.oid,
			       വൃക്ഷം[i]->buffer, വൃക്ഷം[i]->size);
	}

	unpack_വൃക്ഷം_opt.head_idx = -1;
	unpack_വൃക്ഷം_opt.src_index = the_repository->index;
	unpack_വൃക്ഷം_opt.dst_index = the_repository->index;
	unpack_വൃക്ഷം_opt.merge = 1;
	unpack_വൃക്ഷം_opt.fn = stash_worktree_untracked_merge;

	എങ്കിൽ (unpack_trees(ARRAY_SIZE(വൃക്ഷം_desc), വൃക്ഷം_desc, &unpack_വൃക്ഷം_opt))
		die(_("failed to unpack trees"));

	do_diff_cache(&info->b_commit, diff_opt);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ show_stash(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
		      ഘടന repository *repo UNUSED)
{
	പൂർണ്ണസംഖ്യ i;
	പൂർണ്ണസംഖ്യ ret = -1;
	ഘടന stash_info info = STASH_INFO_INIT;
	ഘടന rev_info rev;
	ഘടന strvec stash_args = STRVEC_INIT;
	ഘടന strvec revision_args = STRVEC_INIT;
	എണ്ണൽ {
		UNTRACKED_NONE,
		UNTRACKED_INCLUDE,
		UNTRACKED_ONLY
	} show_untracked = show_include_untracked ? UNTRACKED_INCLUDE : UNTRACKED_NONE;
	ഘടന option options[] = {
		OPT_SET_INT('u', "include-untracked", &show_untracked,
			    N_("include untracked files in the stash"),
			    UNTRACKED_INCLUDE),
		OPT_SET_INT_F(0, "only-untracked", &show_untracked,
			      N_("only show untracked files in the stash"),
			      UNTRACKED_ONLY, PARSE_OPT_NONEG),
		OPT_END()
	};
	പൂർണ്ണസംഖ്യ do_usage = 0;

	init_diff_ui_defaults();
	repo_config(the_repository, git_diff_ui_config, ശൂന്യ);
	repo_init_revisions(the_repository, &rev, prefix);

	argc = parse_options(argc, argv, prefix, options, git_stash_show_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	strvec_തള്ളുക(&revision_args, argv[0]);
	വേണ്ടി (i = 1; i < argc; i++) {
		എങ്കിൽ (argv[i][0] != '-')
			strvec_തള്ളുക(&stash_args, argv[i]);
		അല്ലെങ്കിൽ
			strvec_തള്ളുക(&revision_args, argv[i]);
	}

	എങ്കിൽ (get_stash_info(&info, stash_args.nr, stash_args.v))
		പോകുക cleanup;

	/*
	 * The config settings are applied only if there are not passed
	 * any options.
	 */
	എങ്കിൽ (revision_args.nr == 1) {
		എങ്കിൽ (show_stat)
			rev.diffopt.output_format = DIFF_FORMAT_DIFFSTAT;

		എങ്കിൽ (show_patch)
			rev.diffopt.output_format |= DIFF_FORMAT_PATCH;

		എങ്കിൽ (!show_stat && !show_patch) {
			ret = 0;
			പോകുക cleanup;
		}
	}

	setup_revisions_from_strvec(&revision_args, &rev, ശൂന്യ);
	എങ്കിൽ (revision_args.nr > 1)
		പോകുക usage;
	എങ്കിൽ (!rev.diffopt.output_format) {
		rev.diffopt.output_format = DIFF_FORMAT_PATCH;
		diff_setup_done(&rev.diffopt);
	}

	rev.diffopt.flags.recursive = 1;
	setup_diff_pager(&rev.diffopt);
	തിരഞ്ഞെടുപ്പ് (show_untracked) {
	അവസ്ഥ UNTRACKED_NONE:
		diff_വൃക്ഷം_oid(&info.b_commit, &info.w_commit, "", &rev.diffopt);
		നിർത്തുക;
	അവസ്ഥ UNTRACKED_ONLY:
		എങ്കിൽ (info.has_u)
			diff_വേര്_വൃക്ഷം_oid(&info.u_വൃക്ഷം, "", &rev.diffopt);
		നിർത്തുക;
	അവസ്ഥ UNTRACKED_INCLUDE:
		എങ്കിൽ (info.has_u)
			diff_include_untracked(&info, &rev.diffopt);
		അല്ലെങ്കിൽ
			diff_വൃക്ഷം_oid(&info.b_commit, &info.w_commit, "", &rev.diffopt);
		നിർത്തുക;
	}
	log_വൃക്ഷം_diff_flush(&rev);

	ret = diff_result_code(&rev);

cleanup:
	strvec_clear(&revision_args);
	strvec_clear(&stash_args);
	free_stash_info(&info);
	release_revisions(&rev);
	എങ്കിൽ (do_usage)
		usage_with_options(git_stash_show_usage, options);
	തിരികെ ret;
usage:
	do_usage = 1;
	പോകുക cleanup;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ do_store_stash(സ്ഥിരം ഘടന object_id *w_commit, സ്ഥിരം അക്ഷരം *stash_സന്ദേശം,
			  പൂർണ്ണസംഖ്യ quiet)
{
	ഘടന stash_info info;
	അക്ഷരം revision[GIT_MAX_HEXSZ];

	oid_to_hex_r(revision, w_commit);
	assert_stash_like(&info, revision);

	എങ്കിൽ (!stash_സന്ദേശം)
		stash_സന്ദേശം = "Created via \"git stash store\".";

	എങ്കിൽ (refs_update_ref(get_main_ref_store(the_repository), stash_സന്ദേശം, ref_stash, w_commit, ശൂന്യ,
			    REF_FORCE_CREATE_REFLOG,
			    quiet ? UPDATE_REFS_QUIET_ON_ERR :
			    UPDATE_REFS_സന്ദേശം_ON_ERR)) {
		എങ്കിൽ (!quiet) {
			fprintf_ln(stderr, _("Cannot update %s with %s"),
				   ref_stash, oid_to_hex(w_commit));
		}
		തിരികെ -1;
	}

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ store_stash(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
		       ഘടന repository *repo UNUSED)
{
	പൂർണ്ണസംഖ്യ quiet = 0;
	സ്ഥിരം അക്ഷരം *stash_സന്ദേശം = ശൂന്യ;
	ഘടന object_id obj;
	ഘടന option options[] = {
		OPT__QUIET(&quiet, N_("be quiet")),
		OPT_STRING('m', "message", &stash_സന്ദേശം, "message",
			   N_("stash message")),
		OPT_END()
	};
	പൂർണ്ണസംഖ്യ ret;

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_store_usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT);

	എങ്കിൽ (argc != 1) {
		എങ്കിൽ (!quiet)
			fprintf_ln(stderr, _("\"git stash store\" requires one "
					     "<commit> argument"));
		തിരികെ -1;
	}

	എങ്കിൽ (repo_get_oid_with_flags(the_repository, argv[0], &obj,
				    quiet ? GET_OID_QUIETLY : 0)) {
		എങ്കിൽ (!quiet)
			fprintf_ln(stderr, _("Cannot update %s with %s"),
					     ref_stash, argv[0]);
		ret = -1;
		പോകുക out;
	}

	ret = do_store_stash(&obj, stash_സന്ദേശം, quiet);

out:
	തിരികെ ret;
}

സ്ഥിതം ശൂന്യം add_pathspecs(ഘടന strvec *args,
			  സ്ഥിരം ഘടന pathspec *ps) {
	പൂർണ്ണസംഖ്യ i;

	വേണ്ടി (i = 0; i < ps->nr; i++)
		strvec_തള്ളുക(args, ps->ഇനങ്ങൾ[i].original);
}

/*
 * `untracked_files` will be filled with the names of untracked files.
 * The return value is:
 *
 * = 0 if there are not any untracked files
 * > 0 if there are untracked files
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ get_untracked_files(സ്ഥിരം ഘടന pathspec *ps, പൂർണ്ണസംഖ്യ include_untracked,
			       ഘടന strbuf *untracked_files)
{
	പൂർണ്ണസംഖ്യ i;
	പൂർണ്ണസംഖ്യ found = 0;
	ഘടന dir_struct dir = DIR_INIT;

	എങ്കിൽ (include_untracked != INCLUDE_ALL_FILES)
		setup_standard_excludes(&dir);

	fill_directory(&dir, the_repository->index, ps);
	വേണ്ടി (i = 0; i < dir.nr; i++) {
		ഘടന dir_entry *ent = dir.entries[i];
		found++;
		strbuf_addstr(untracked_files, ent->name);
		/* NUL-terminate: will be fed to update-index -z */
		strbuf_addch(untracked_files, '\0');
	}

	dir_clear(&dir);
	തിരികെ found;
}

/*
 * The return value of `check_changes_tracked_files()` can be:
 *
 * < 0 if there was an error
 * = 0 if there are no changes.
 * > 0 if there are changes.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ check_changes_tracked_files(സ്ഥിരം ഘടന pathspec *ps)
{
	ഘടന rev_info rev;
	ഘടന object_id dummy;
	പൂർണ്ണസംഖ്യ ret = 0;

	/* No initial commit. */
	എങ്കിൽ (repo_get_oid(the_repository, "HEAD", &dummy))
		തിരികെ -1;

	എങ്കിൽ (repo_read_index(the_repository) < 0)
		തിരികെ -1;

	repo_init_revisions(the_repository, &rev, ശൂന്യ);
	copy_pathspec(&rev.prune_data, ps);

	rev.diffopt.flags.quick = 1;
	rev.diffopt.flags.ignore_submodules = 1;
	rev.abbrev = 0;

	add_head_to_pending(&rev);
	diff_setup_done(&rev.diffopt);

	run_diff_index(&rev, DIFF_INDEX_CACHED);
	എങ്കിൽ (diff_result_code(&rev)) {
		ret = 1;
		പോകുക done;
	}

	run_diff_files(&rev, 0);
	എങ്കിൽ (diff_result_code(&rev)) {
		ret = 1;
		പോകുക done;
	}

done:
	release_revisions(&rev);
	തിരികെ ret;
}

/*
 * The function will fill `untracked_files` with the names of untracked files
 * It will return 1 if there were any changes and 0 if there were not.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ check_changes(സ്ഥിരം ഘടന pathspec *ps, പൂർണ്ണസംഖ്യ include_untracked,
			 ഘടന strbuf *untracked_files)
{
	പൂർണ്ണസംഖ്യ ret = 0;
	എങ്കിൽ (check_changes_tracked_files(ps))
		ret = 1;

	എങ്കിൽ (include_untracked && get_untracked_files(ps, include_untracked,
						     untracked_files))
		ret = 1;

	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ save_untracked_files(ഘടന stash_info *info, ഘടന strbuf *സന്ദേശം,
				ഘടന strbuf *files)
{
	പൂർണ്ണസംഖ്യ ret = 0;
	ഘടന strbuf untracked_സന്ദേശം = STRBUF_INIT;
	ഘടന child_process cp_upd_index = CHILD_PROCESS_INIT;
	ഘടന index_state istate = INDEX_STATE_INIT(the_repository);

	cp_upd_index.git_cmd = 1;
	strvec_pushl(&cp_upd_index.args, "update-index", "-z", "--add",
		     "--remove", "--stdin", ശൂന്യ);
	strvec_pushf(&cp_upd_index.env, "GIT_INDEX_FILE=%s",
			 stash_index_path.buf);

	strbuf_addf(&untracked_സന്ദേശം, "untracked files on %s\n", സന്ദേശം->buf);
	എങ്കിൽ (pipe_command(&cp_upd_index, files->buf, files->len, ശൂന്യ, 0,
			 ശൂന്യ, 0)) {
		ret = -1;
		പോകുക done;
	}

	എങ്കിൽ (write_index_as_വൃക്ഷം(&info->u_വൃക്ഷം, &istate, stash_index_path.buf, 0,
				ശൂന്യ)) {
		ret = -1;
		പോകുക done;
	}

	എങ്കിൽ (commit_വൃക്ഷം(untracked_സന്ദേശം.buf, untracked_സന്ദേശം.len,
			&info->u_വൃക്ഷം, ശൂന്യ, &info->u_commit, ശൂന്യ, ശൂന്യ)) {
		ret = -1;
		പോകുക done;
	}

done:
	release_index(&istate);
	strbuf_release(&untracked_സന്ദേശം);
	remove_path(stash_index_path.buf);
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ stash_staged(ഘടന stash_info *info, ഘടന strbuf *out_patch,
			പൂർണ്ണസംഖ്യ quiet)
{
	പൂർണ്ണസംഖ്യ ret = 0;
	ഘടന child_process cp_diff_വൃക്ഷം = CHILD_PROCESS_INIT;
	ഘടന index_state istate = INDEX_STATE_INIT(the_repository);

	എങ്കിൽ (write_index_as_വൃക്ഷം(&info->w_വൃക്ഷം, &istate, the_repository->index_file,
				0, ശൂന്യ)) {
		ret = -1;
		പോകുക done;
	}

	cp_diff_വൃക്ഷം.git_cmd = 1;
	strvec_pushl(&cp_diff_വൃക്ഷം.args, "diff-tree", "-p", "--binary",
		     "--no-color",
		     "-U1", "HEAD", oid_to_hex(&info->w_വൃക്ഷം), "--", ശൂന്യ);
	എങ്കിൽ (pipe_command(&cp_diff_വൃക്ഷം, ശൂന്യ, 0, out_patch, 0, ശൂന്യ, 0)) {
		ret = -1;
		പോകുക done;
	}

	എങ്കിൽ (!out_patch->len) {
		എങ്കിൽ (!quiet)
			fprintf_ln(stderr, _("No staged changes"));
		ret = 1;
	}

done:
	release_index(&istate);
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ stash_patch(ഘടന stash_info *info, സ്ഥിരം ഘടന pathspec *ps,
		       ഘടന strbuf *out_patch, പൂർണ്ണസംഖ്യ quiet,
		       ഘടന interactive_options *interactive_opts)
{
	പൂർണ്ണസംഖ്യ ret = 0;
	ഘടന child_process cp_diff_വൃക്ഷം = CHILD_PROCESS_INIT;
	ഘടന commit *head_commit;
	സ്ഥിരം ഘടന object_id *head_വൃക്ഷം;
	ഘടന index_state istate = INDEX_STATE_INIT(the_repository);
	അക്ഷരം *old_index_env = ശൂന്യ, *old_repo_index_file;

	remove_path(stash_index_path.buf);

	head_commit = lookup_commit(the_repository, &info->b_commit);
	എങ്കിൽ (!head_commit || repo_parse_commit(the_repository, head_commit)) {
		ret = -1;
		പോകുക done;
	}
	head_വൃക്ഷം = get_commit_വൃക്ഷം_oid(head_commit);
	എങ്കിൽ (!head_വൃക്ഷം) {
		ret = -1;
		പോകുക done;
	}

	എങ്കിൽ (create_index_from_വൃക്ഷം(head_വൃക്ഷം, stash_index_path.buf)) {
		ret = -1;
		പോകുക done;
	}

	/* Find out what the user wants. */
	old_repo_index_file = the_repository->index_file;
	the_repository->index_file = stash_index_path.buf;
	old_index_env = xstrdup_or_null(getenv(INDEX_ENVIRONMENT));
	setenv(INDEX_ENVIRONMENT, the_repository->index_file, 1);

	ret = !!run_add_p(the_repository, ADD_P_STASH, interactive_opts, ശൂന്യ, ps, 0);

	the_repository->index_file = old_repo_index_file;
	എങ്കിൽ (old_index_env && *old_index_env)
		setenv(INDEX_ENVIRONMENT, old_index_env, 1);
	അല്ലെങ്കിൽ
		unsetenv(INDEX_ENVIRONMENT);
	FREE_AND_NULL(old_index_env);

	/* State of the working tree. */
	എങ്കിൽ (write_index_as_വൃക്ഷം(&info->w_വൃക്ഷം, &istate, stash_index_path.buf, 0,
				ശൂന്യ)) {
		ret = -1;
		പോകുക done;
	}

	cp_diff_വൃക്ഷം.git_cmd = 1;
	strvec_pushl(&cp_diff_വൃക്ഷം.args, "diff-tree", "-p", "-U1", "HEAD",
		     "--no-color",
		     oid_to_hex(&info->w_വൃക്ഷം), "--", ശൂന്യ);
	എങ്കിൽ (pipe_command(&cp_diff_വൃക്ഷം, ശൂന്യ, 0, out_patch, 0, ശൂന്യ, 0)) {
		ret = -1;
		പോകുക done;
	}

	എങ്കിൽ (!out_patch->len) {
		എങ്കിൽ (!quiet)
			fprintf_ln(stderr, _("No changes selected"));
		ret = 1;
	}

done:
	release_index(&istate);
	remove_path(stash_index_path.buf);
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ stash_working_വൃക്ഷം(ഘടന stash_info *info, സ്ഥിരം ഘടന pathspec *ps)
{
	പൂർണ്ണസംഖ്യ ret = 0;
	ഘടന rev_info rev;
	ഘടന child_process cp_upd_index = CHILD_PROCESS_INIT;
	ഘടന strbuf diff_output = STRBUF_INIT;
	ഘടന index_state istate = INDEX_STATE_INIT(the_repository);

	repo_init_revisions(the_repository, &rev, ശൂന്യ);
	copy_pathspec(&rev.prune_data, ps);

	set_alternate_index_output(stash_index_path.buf);
	എങ്കിൽ (reset_വൃക്ഷം(&info->i_വൃക്ഷം, 0, 0)) {
		ret = -1;
		പോകുക done;
	}
	set_alternate_index_output(ശൂന്യ);

	rev.diffopt.output_format = DIFF_FORMAT_CALLBACK;
	rev.diffopt.format_callback = add_diff_to_buf;
	rev.diffopt.format_callback_data = &diff_output;

	എങ്കിൽ (repo_read_index_preload(the_repository, &rev.diffopt.pathspec, 0) < 0) {
		ret = -1;
		പോകുക done;
	}

	add_pending_object(&rev, parse_object(the_repository, &info->b_commit),
			   "");
	run_diff_index(&rev, 0);

	cp_upd_index.git_cmd = 1;
	strvec_pushl(&cp_upd_index.args, "update-index",
		     "--ignore-skip-worktree-entries",
		     "-z", "--add", "--remove", "--stdin", ശൂന്യ);
	strvec_pushf(&cp_upd_index.env, "GIT_INDEX_FILE=%s",
		     stash_index_path.buf);

	എങ്കിൽ (pipe_command(&cp_upd_index, diff_output.buf, diff_output.len,
			 ശൂന്യ, 0, ശൂന്യ, 0)) {
		ret = -1;
		പോകുക done;
	}

	എങ്കിൽ (write_index_as_വൃക്ഷം(&info->w_വൃക്ഷം, &istate, stash_index_path.buf, 0,
				ശൂന്യ)) {
		ret = -1;
		പോകുക done;
	}

done:
	release_index(&istate);
	release_revisions(&rev);
	strbuf_release(&diff_output);
	remove_path(stash_index_path.buf);
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ do_create_stash(സ്ഥിരം ഘടന pathspec *ps, ഘടന strbuf *stash_സന്ദേശം_buf,
			   പൂർണ്ണസംഖ്യ include_untracked, പൂർണ്ണസംഖ്യ patch_mode,
			   ഘടന interactive_options *interactive_opts,
			   പൂർണ്ണസംഖ്യ only_staged, ഘടന stash_info *info, ഘടന strbuf *patch,
			   പൂർണ്ണസംഖ്യ quiet)
{
	പൂർണ്ണസംഖ്യ ret = 0;
	പൂർണ്ണസംഖ്യ flags = 0;
	പൂർണ്ണസംഖ്യ untracked_commit_option = 0;
	സ്ഥിരം അക്ഷരം *head_short_sha1 = ശൂന്യ;
	സ്ഥിരം അക്ഷരം *branch_ref = ശൂന്യ;
	സ്ഥിരം അക്ഷരം *branch_name = "(no branch)";
	അക്ഷരം *branch_name_buf = ശൂന്യ;
	ഘടന commit *head_commit = ശൂന്യ;
	ഘടന commit_list *parents = ശൂന്യ;
	ഘടന strbuf സന്ദേശം = STRBUF_INIT;
	ഘടന strbuf commit_വൃക്ഷം_label = STRBUF_INIT;
	ഘടന strbuf untracked_files = STRBUF_INIT;

	prepare_fallback_ident("git stash", "git@stash");

	repo_read_index_preload(the_repository, ശൂന്യ, 0);
	എങ്കിൽ (repo_refresh_and_write_index(the_repository, REFRESH_QUIET, 0, 0,
					 ശൂന്യ, ശൂന്യ, ശൂന്യ) < 0) {
		ret = error(_("could not write index"));
		പോകുക done;
	}

	എങ്കിൽ (repo_get_oid(the_repository, "HEAD", &info->b_commit)) {
		എങ്കിൽ (!quiet)
			fprintf_ln(stderr, _("You do not have "
					     "the initial commit yet"));
		ret = -1;
		പോകുക done;
	} അല്ലെങ്കിൽ {
		head_commit = lookup_commit(the_repository, &info->b_commit);
	}

	എങ്കിൽ (!check_changes(ps, include_untracked, &untracked_files)) {
		ret = 1;
		പോകുക done;
	}

	branch_ref = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
					     "HEAD", 0, ശൂന്യ, &flags);

	എങ്കിൽ (flags & REF_ISSYMREF) {
		എങ്കിൽ (skip_prefix(branch_ref, "refs/heads/", &branch_name))
			branch_name = branch_name_buf = xstrdup(branch_name);
	}

	head_short_sha1 = repo_find_unique_abbrev(the_repository,
						  &head_commit->object.oid,
						  DEFAULT_ABBREV);
	strbuf_addf(&സന്ദേശം, "%s: %s ", branch_name, head_short_sha1);
	pp_commit_easy(CMIT_FMT_ONELINE, head_commit, &സന്ദേശം);

	strbuf_addf(&commit_വൃക്ഷം_label, "index on %s\n", സന്ദേശം.buf);
	commit_list_തിരുകുക(head_commit, &parents);
	എങ്കിൽ (write_index_as_വൃക്ഷം(&info->i_വൃക്ഷം, the_repository->index,
				repo_get_index_file(the_repository), 0, ശൂന്യ) ||
	    commit_വൃക്ഷം(commit_വൃക്ഷം_label.buf, commit_വൃക്ഷം_label.len,
			&info->i_വൃക്ഷം, parents, &info->i_commit, ശൂന്യ, ശൂന്യ)) {
		എങ്കിൽ (!quiet)
			fprintf_ln(stderr, _("Cannot save the current "
					     "index state"));
		ret = -1;
		പോകുക done;
	}

	commit_list_free(parents);
	parents = ശൂന്യ;

	എങ്കിൽ (include_untracked) {
		എങ്കിൽ (save_untracked_files(info, &സന്ദേശം, &untracked_files)) {
			എങ്കിൽ (!quiet)
				fprintf_ln(stderr, _("Cannot save "
						     "the untracked files"));
			ret = -1;
			പോകുക done;
		}
		untracked_commit_option = 1;
	}
	എങ്കിൽ (patch_mode) {
		ret = stash_patch(info, ps, patch, quiet, interactive_opts);
		എങ്കിൽ (ret < 0) {
			എങ്കിൽ (!quiet)
				fprintf_ln(stderr, _("Cannot save the current "
						     "worktree state"));
			പോകുക done;
		} അല്ലെങ്കിൽ എങ്കിൽ (ret > 0) {
			പോകുക done;
		}
	} അല്ലെങ്കിൽ എങ്കിൽ (only_staged) {
		ret = stash_staged(info, patch, quiet);
		എങ്കിൽ (ret < 0) {
			എങ്കിൽ (!quiet)
				fprintf_ln(stderr, _("Cannot save the current "
						     "staged state"));
			പോകുക done;
		} അല്ലെങ്കിൽ എങ്കിൽ (ret > 0) {
			പോകുക done;
		}
	} അല്ലെങ്കിൽ {
		എങ്കിൽ (stash_working_വൃക്ഷം(info, ps)) {
			എങ്കിൽ (!quiet)
				fprintf_ln(stderr, _("Cannot save the current "
						     "worktree state"));
			ret = -1;
			പോകുക done;
		}
	}

	എങ്കിൽ (!stash_സന്ദേശം_buf->len)
		strbuf_addf(stash_സന്ദേശം_buf, "WIP on %s", സന്ദേശം.buf);
	അല്ലെങ്കിൽ
		strbuf_insertf(stash_സന്ദേശം_buf, 0, "On %s: ", branch_name);

	എങ്കിൽ (untracked_commit_option)
		commit_list_തിരുകുക(lookup_commit(the_repository,
						 &info->u_commit),
				   &parents);
	commit_list_തിരുകുക(lookup_commit(the_repository, &info->i_commit),
			   &parents);
	commit_list_തിരുകുക(head_commit, &parents);

	എങ്കിൽ (commit_വൃക്ഷം(stash_സന്ദേശം_buf->buf, stash_സന്ദേശം_buf->len, &info->w_വൃക്ഷം,
			parents, &info->w_commit, ശൂന്യ, ശൂന്യ)) {
		എങ്കിൽ (!quiet)
			fprintf_ln(stderr, _("Cannot record "
					     "working tree state"));
		ret = -1;
		പോകുക done;
	}

done:
	strbuf_release(&commit_വൃക്ഷം_label);
	strbuf_release(&സന്ദേശം);
	strbuf_release(&untracked_files);
	commit_list_free(parents);
	സ്വതന്ത്രം(branch_name_buf);
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ create_stash(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix UNUSED,
			ഘടന repository *repo UNUSED)
{
	പൂർണ്ണസംഖ്യ ret;
	ഘടന strbuf stash_സന്ദേശം_buf = STRBUF_INIT;
	ഘടന stash_info info = STASH_INFO_INIT;
	ഘടന pathspec ps;

	/* Starting with argv[1], since argv[0] is "create" */
	strbuf_join_argv(&stash_സന്ദേശം_buf, argc - 1, ++argv, ' ');

	memset(&ps, 0, വലുപ്പം(ps));
	എങ്കിൽ (!check_changes_tracked_files(&ps))
		തിരികെ 0;

	ret = do_create_stash(&ps, &stash_സന്ദേശം_buf, 0, 0, ശൂന്യ, 0, &info,
			      ശൂന്യ, 0);
	എങ്കിൽ (!ret)
		printf_ln("%s", oid_to_hex(&info.w_commit));

	free_stash_info(&info);
	strbuf_release(&stash_സന്ദേശം_buf);
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ do_തള്ളുക_stash(സ്ഥിരം ഘടന pathspec *ps, സ്ഥിരം അക്ഷരം *stash_സന്ദേശം, പൂർണ്ണസംഖ്യ quiet,
			 പൂർണ്ണസംഖ്യ keep_index, പൂർണ്ണസംഖ്യ patch_mode,
			 ഘടന interactive_options *interactive_opts,
			 പൂർണ്ണസംഖ്യ include_untracked, പൂർണ്ണസംഖ്യ only_staged)
{
	പൂർണ്ണസംഖ്യ ret = 0;
	ഘടന stash_info info = STASH_INFO_INIT;
	ഘടന strbuf patch = STRBUF_INIT;
	ഘടന strbuf stash_സന്ദേശം_buf = STRBUF_INIT;
	ഘടന strbuf untracked_files = STRBUF_INIT;
	ഘടന strbuf out = STRBUF_INIT;

	എങ്കിൽ (patch_mode && keep_index == -1)
		keep_index = 1;

	എങ്കിൽ (patch_mode && include_untracked) {
		fprintf_ln(stderr, _("Can't use --patch and --include-untracked"
				     " or --all at the same time"));
		ret = -1;
		പോകുക done;
	}

	/* --patch overrides --staged */
	എങ്കിൽ (patch_mode)
		only_staged = 0;

	എങ്കിൽ (only_staged && include_untracked) {
		fprintf_ln(stderr, _("Can't use --staged and --include-untracked"
				     " or --all at the same time"));
		ret = -1;
		പോകുക done;
	}

	repo_read_index_preload(the_repository, ശൂന്യ, 0);
	എങ്കിൽ (!include_untracked && ps->nr) {
		അക്ഷരം *ps_matched = xcalloc(ps->nr, 1);

		എങ്കിൽ (pathspec_needs_expanded_index(the_repository->index, ps))
			ensure_full_index(the_repository->index);
		വേണ്ടി (size_t i = 0; i < the_repository->index->cache_nr; i++)
			ce_path_match(the_repository->index, the_repository->index->cache[i], ps,
				      ps_matched);

		എങ്കിൽ (report_path_error(ps_matched, ps)) {
			fprintf_ln(stderr, _("Did you forget to 'git add'?"));
			ret = -1;
			സ്വതന്ത്രം(ps_matched);
			പോകുക done;
		}
		സ്വതന്ത്രം(ps_matched);
	}

	എങ്കിൽ (repo_refresh_and_write_index(the_repository, REFRESH_QUIET, 0, 0,
					 ശൂന്യ, ശൂന്യ, ശൂന്യ)) {
		ret = error(_("could not write index"));
		പോകുക done;
	}

	എങ്കിൽ (!check_changes(ps, include_untracked, &untracked_files)) {
		എങ്കിൽ (!quiet)
			printf_ln(_("No local changes to save"));
		പോകുക done;
	}

	എങ്കിൽ (!refs_reflog_exists(get_main_ref_store(the_repository), ref_stash) && do_clear_stash()) {
		ret = -1;
		എങ്കിൽ (!quiet)
			fprintf_ln(stderr, _("Cannot initialize stash"));
		പോകുക done;
	}

	എങ്കിൽ (stash_സന്ദേശം)
		strbuf_addstr(&stash_സന്ദേശം_buf, stash_സന്ദേശം);
	എങ്കിൽ (do_create_stash(ps, &stash_സന്ദേശം_buf, include_untracked, patch_mode,
			    interactive_opts, only_staged, &info, &patch, quiet)) {
		ret = -1;
		പോകുക done;
	}

	എങ്കിൽ (do_store_stash(&info.w_commit, stash_സന്ദേശം_buf.buf, 1)) {
		ret = -1;
		എങ്കിൽ (!quiet)
			fprintf_ln(stderr, _("Cannot save the current status"));
		പോകുക done;
	}

	എങ്കിൽ (!quiet)
		printf_ln(_("Saved working directory and index state %s"),
			  stash_സന്ദേശം_buf.buf);

	എങ്കിൽ (!(patch_mode || only_staged)) {
		എങ്കിൽ (include_untracked && !ps->nr) {
			ഘടന child_process cp = CHILD_PROCESS_INIT;

			cp.git_cmd = 1;
			എങ്കിൽ (startup_info->original_cwd) {
				cp.dir = startup_info->original_cwd;
				strvec_pushf(&cp.env, "%s=%s",
					     GIT_WORK_വൃക്ഷം_ENVIRONMENT,
					     the_repository->worktree);
			}
			strvec_pushl(&cp.args, "clean", "--force",
				     "--quiet", "-d", ":/", ശൂന്യ);
			എങ്കിൽ (include_untracked == INCLUDE_ALL_FILES)
				strvec_തള്ളുക(&cp.args, "-x");
			എങ്കിൽ (run_command(&cp)) {
				ret = -1;
				പോകുക done;
			}
		}
		discard_index(the_repository->index);
		എങ്കിൽ (ps->nr) {
			ഘടന child_process cp_add = CHILD_PROCESS_INIT;
			ഘടന child_process cp_diff = CHILD_PROCESS_INIT;
			ഘടന child_process cp_apply = CHILD_PROCESS_INIT;

			cp_add.git_cmd = 1;
			strvec_തള്ളുക(&cp_add.args, "add");
			എങ്കിൽ (!include_untracked)
				strvec_തള്ളുക(&cp_add.args, "-u");
			എങ്കിൽ (include_untracked == INCLUDE_ALL_FILES)
				strvec_തള്ളുക(&cp_add.args, "--force");
			strvec_തള്ളുക(&cp_add.args, "--");
			add_pathspecs(&cp_add.args, ps);
			എങ്കിൽ (run_command(&cp_add)) {
				ret = -1;
				പോകുക done;
			}

			cp_diff.git_cmd = 1;
			strvec_pushl(&cp_diff.args, "diff-index", "-p",
				     "--no-color",
				     "--cached", "--binary", "HEAD", "--",
				     ശൂന്യ);
			add_pathspecs(&cp_diff.args, ps);
			എങ്കിൽ (pipe_command(&cp_diff, ശൂന്യ, 0, &out, 0, ശൂന്യ, 0)) {
				ret = -1;
				പോകുക done;
			}

			cp_apply.git_cmd = 1;
			strvec_pushl(&cp_apply.args, "apply", "--index",
				     "-R", ശൂന്യ);
			എങ്കിൽ (pipe_command(&cp_apply, out.buf, out.len, ശൂന്യ, 0,
					 ശൂന്യ, 0)) {
				ret = -1;
				പോകുക done;
			}
		} അല്ലെങ്കിൽ {
			ഘടന child_process cp = CHILD_PROCESS_INIT;
			cp.git_cmd = 1;
			/* BUG: this nukes untracked files in the way */
			strvec_pushl(&cp.args, "reset", "--hard", "-q",
				     "--no-recurse-submodules", ശൂന്യ);
			എങ്കിൽ (run_command(&cp)) {
				ret = -1;
				പോകുക done;
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
		എങ്കിൽ (keep_index == 1 && !is_null_oid(&info.i_വൃക്ഷം) &&
		    !is_empty_വൃക്ഷം_oid(&info.i_വൃക്ഷം, the_repository->hash_algo)) {
			ഘടന child_process cp = CHILD_PROCESS_INIT;

			cp.git_cmd = 1;
			strvec_pushl(&cp.args, "checkout", "--no-overlay",
				     oid_to_hex(&info.i_വൃക്ഷം), "--", ശൂന്യ);
			എങ്കിൽ (!ps->nr)
				strvec_തള്ളുക(&cp.args, ":/");
			അല്ലെങ്കിൽ
				add_pathspecs(&cp.args, ps);
			എങ്കിൽ (run_command(&cp)) {
				ret = -1;
				പോകുക done;
			}
		}
		പോകുക done;
	} അല്ലെങ്കിൽ {
		ഘടന child_process cp = CHILD_PROCESS_INIT;

		cp.git_cmd = 1;
		strvec_pushl(&cp.args, "apply", "-R", ശൂന്യ);

		എങ്കിൽ (pipe_command(&cp, patch.buf, patch.len, ശൂന്യ, 0, ശൂന്യ, 0)) {
			എങ്കിൽ (!quiet)
				fprintf_ln(stderr, _("Cannot remove "
						     "worktree changes"));
			ret = -1;
			പോകുക done;
		}

		എങ്കിൽ (keep_index < 1) {
			ഘടന child_process cp = CHILD_PROCESS_INIT;

			cp.git_cmd = 1;
			strvec_pushl(&cp.args, "reset", "-q", "--refresh", "--",
				     ശൂന്യ);
			add_pathspecs(&cp.args, ps);
			എങ്കിൽ (run_command(&cp)) {
				ret = -1;
				പോകുക done;
			}
		}
		പോകുക done;
	}

done:
	strbuf_release(&patch);
	strbuf_release(&out);
	free_stash_info(&info);
	strbuf_release(&stash_സന്ദേശം_buf);
	strbuf_release(&untracked_files);
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ തള്ളുക_stash(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
		      പൂർണ്ണസംഖ്യ തള്ളുക_assumed)
{
	പൂർണ്ണസംഖ്യ force_assume = 0;
	പൂർണ്ണസംഖ്യ keep_index = -1;
	പൂർണ്ണസംഖ്യ only_staged = 0;
	പൂർണ്ണസംഖ്യ patch_mode = 0;
	പൂർണ്ണസംഖ്യ include_untracked = 0;
	പൂർണ്ണസംഖ്യ quiet = 0;
	പൂർണ്ണസംഖ്യ pathspec_file_nul = 0;
	സ്ഥിരം അക്ഷരം *stash_സന്ദേശം = ശൂന്യ;
	അക്ഷരം *pathspec_from_file = ശൂന്യ;
	ഘടന pathspec ps;
	ഘടന interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	ഘടന option options[] = {
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
		OPT_STRING('m', "message", &stash_സന്ദേശം, N_("message"),
			   N_("stash message")),
		OPT_PATHSPEC_FROM_FILE(&pathspec_from_file),
		OPT_PATHSPEC_FILE_NUL(&pathspec_file_nul),
		OPT_END()
	};
	പൂർണ്ണസംഖ്യ ret;

	എങ്കിൽ (argc) {
		പൂർണ്ണസംഖ്യ flags = PARSE_OPT_KEEP_DASHDASH;

		എങ്കിൽ (തള്ളുക_assumed)
			flags |= PARSE_OPT_STOP_AT_NON_OPTION;

		argc = parse_options(argc, argv, prefix, options,
				     തള്ളുക_assumed ? git_stash_usage :
				     git_stash_തള്ളുക_usage, flags);
		force_assume |= patch_mode;
	}

	എങ്കിൽ (argc) {
		എങ്കിൽ (!strcmp(argv[0], "--")) {
			argc--;
			argv++;
		} അല്ലെങ്കിൽ എങ്കിൽ (തള്ളുക_assumed && !force_assume) {
			die("subcommand wasn't specified; 'push' can't be assumed due to unexpected token '%s'",
			    argv[0]);
		}
	}

	parse_pathspec(&ps, 0, PATHSPEC_PREFER_FULL | PATHSPEC_PREFIX_ORIGIN,
		       prefix, argv);

	എങ്കിൽ (pathspec_from_file) {
		എങ്കിൽ (patch_mode)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--patch");

		എങ്കിൽ (only_staged)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--staged");

		എങ്കിൽ (ps.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&ps, 0,
				    PATHSPEC_PREFER_FULL | PATHSPEC_PREFIX_ORIGIN,
				    prefix, pathspec_from_file, pathspec_file_nul);
	} അല്ലെങ്കിൽ എങ്കിൽ (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	എങ്കിൽ (!patch_mode) {
		എങ്കിൽ (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--patch");
		എങ്കിൽ (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--patch");
		എങ്കിൽ (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--patch");
	}

	എങ്കിൽ (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	എങ്കിൽ (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	ret = do_തള്ളുക_stash(&ps, stash_സന്ദേശം, quiet, keep_index, patch_mode,
			    &interactive_opts, include_untracked, only_staged);

	clear_pathspec(&ps);
	സ്വതന്ത്രം(pathspec_from_file);
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ തള്ളുക_stash_unassumed(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
				ഘടന repository *repo UNUSED)
{
	തിരികെ തള്ളുക_stash(argc, argv, prefix, 0);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ save_stash(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
		      ഘടന repository *repo UNUSED)
{
	പൂർണ്ണസംഖ്യ keep_index = -1;
	പൂർണ്ണസംഖ്യ only_staged = 0;
	പൂർണ്ണസംഖ്യ patch_mode = 0;
	പൂർണ്ണസംഖ്യ include_untracked = 0;
	പൂർണ്ണസംഖ്യ quiet = 0;
	പൂർണ്ണസംഖ്യ ret = 0;
	സ്ഥിരം അക്ഷരം *stash_സന്ദേശം = ശൂന്യ;
	ഘടന pathspec ps;
	ഘടന strbuf stash_സന്ദേശം_buf = STRBUF_INIT;
	ഘടന interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	ഘടന option options[] = {
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
		OPT_STRING('m', "message", &stash_സന്ദേശം, "message",
			   N_("stash message")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_save_usage,
			     PARSE_OPT_KEEP_DASHDASH);

	എങ്കിൽ (argc)
		stash_സന്ദേശം = strbuf_join_argv(&stash_സന്ദേശം_buf, argc, argv, ' ');

	memset(&ps, 0, വലുപ്പം(ps));

	എങ്കിൽ (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	എങ്കിൽ (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	എങ്കിൽ (!patch_mode) {
		എങ്കിൽ (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--patch");
		എങ്കിൽ (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--patch");
		എങ്കിൽ (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--patch");
	}

	ret = do_തള്ളുക_stash(&ps, stash_സന്ദേശം, quiet, keep_index,
			    patch_mode, &interactive_opts, include_untracked,
			    only_staged);

	strbuf_release(&stash_സന്ദേശം_buf);
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ write_commit_with_parents(ഘടന repository *r,
				     ഘടന object_id *out,
				     സ്ഥിരം ഘടന object_id *oid,
				     ഘടന commit_list *parents)
{
	size_t author_len, committer_len;
	ഘടന commit *this;
	സ്ഥിരം അക്ഷരം *orig_author, *orig_committer;
	അക്ഷരം *author = ശൂന്യ, *committer = ശൂന്യ;
	സ്ഥിരം അക്ഷരം *buffer;
	unsigned long bufsize;
	സ്ഥിരം അക്ഷരം *p;
	ഘടന strbuf സന്ദേശം = STRBUF_INIT;
	പൂർണ്ണസംഖ്യ ret = 0;
	ഘടന ident_split id;

	this = lookup_commit_reference(r, oid);
	buffer = repo_get_commit_buffer(r, this, &bufsize);
	orig_author = find_commit_header(buffer, "author", &author_len);
	orig_committer = find_commit_header(buffer, "committer", &committer_len);

	എങ്കിൽ (!orig_author || !orig_committer) {
		ret = error(_("cannot parse commit %s"), oid_to_hex(oid));
		പോകുക out;
	}

	എങ്കിൽ (split_ident_വരി(&id, orig_author, author_len) < 0 ||
	    split_ident_വരി(&id, orig_committer, committer_len) < 0) {
		ret = error(_("invalid author or committer for %s"), oid_to_hex(oid));
		പോകുക out;
	}

	p = strstr(buffer, "\n\n");
	strbuf_addstr(&സന്ദേശം, "git stash: ");

	എങ്കിൽ (p)
		strbuf_add(&സന്ദേശം, p + 2, bufsize - (p + 2 - buffer));
	strbuf_complete_വരി(&സന്ദേശം);

	author = xmemdupz(orig_author, author_len);
	committer = xmemdupz(orig_committer, committer_len);

	എങ്കിൽ (commit_വൃക്ഷം_extended(സന്ദേശം.buf, സന്ദേശം.len,
				 r->hash_algo->empty_വൃക്ഷം, parents,
				 out, author, committer,
				 ശൂന്യ, ശൂന്യ)) {
		ret = error(_("could not write commit"));
		പോകുക out;
	}
out:
	strbuf_release(&സന്ദേശം);
	repo_unuse_commit_buffer(r, this, buffer);
	സ്വതന്ത്രം(author);
	സ്വതന്ത്രം(committer);
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ do_import_stash(ഘടന repository *r, സ്ഥിരം അക്ഷരം *rev)
{
	ഘടന object_id chain;
	പൂർണ്ണസംഖ്യ res = 0;
	സ്ഥിരം അക്ഷരം *buffer = ശൂന്യ;
	unsigned long bufsize;
	ഘടന commit *this = ശൂന്യ;
	ഘടന commit_list *ഇനങ്ങൾ = ശൂന്യ, *cur;
	അക്ഷരം *സന്ദേശം = ശൂന്യ;

	എങ്കിൽ (repo_get_oid(r, rev, &chain))
		തിരികെ error(_("not a valid revision: %s"), rev);

	this = lookup_commit_reference(r, &chain);
	എങ്കിൽ (!this)
		തിരികെ error(_("not a commit: %s"), rev);

	/*
	 * Walk the commit history, finding each stash entry, and load data into
	 * the array.
	 */
	വേണ്ടി (;;) {
		സ്ഥിരം അക്ഷരം *author, *committer;
		size_t author_len, committer_len;
		സ്ഥിരം അക്ഷരം *p;
		സ്ഥിരം അക്ഷരം *expected = "git stash <git@stash> 1000684800 +0000";
		സ്ഥിരം അക്ഷരം *prefix = "git stash: ";
		ഘടന commit *stash;
		ഘടന വൃക്ഷം *വൃക്ഷം = repo_get_commit_വൃക്ഷം(r, this);

		എങ്കിൽ (!വൃക്ഷം ||
		    !oideq(&വൃക്ഷം->object.oid, r->hash_algo->empty_വൃക്ഷം) ||
		    (this->parents &&
		     (!this->parents->next || this->parents->next->next))) {
			res = error(_("%s is not a valid exported stash commit"),
					oid_to_hex(&this->object.oid));
			പോകുക out;
		}

		buffer = repo_get_commit_buffer(r, this, &bufsize);

		എങ്കിൽ (!this->parents) {
			/*
			 * We don't have any parents.  Make sure this is our
			 * root commit.
			 */
			author = find_commit_header(buffer, "author", &author_len);
			committer = find_commit_header(buffer, "committer", &committer_len);

			എങ്കിൽ (!author || !committer) {
				error(_("cannot parse commit %s"), oid_to_hex(&this->object.oid));
				പോകുക out;
			}

			എങ്കിൽ (author_len != strlen(expected) ||
			    committer_len != strlen(expected) ||
			    memcmp(author, expected, author_len) ||
			    memcmp(committer, expected, committer_len)) {
				res = error(_("found root commit %s with invalid data"), oid_to_hex(&this->object.oid));
				പോകുക out;
			}
			നിർത്തുക;
		}

		p = strstr(buffer, "\n\n");
		എങ്കിൽ (!p) {
			res = error(_("cannot parse commit %s"), oid_to_hex(&this->object.oid));
			പോകുക out;
		}

		p += 2;
		എങ്കിൽ (((size_t)(bufsize - (p - buffer)) < strlen(prefix)) ||
		    memcmp(prefix, p, strlen(prefix))) {
			res = error(_("found stash commit %s without expected prefix"), oid_to_hex(&this->object.oid));
			പോകുക out;
		}

		stash = this->parents->next->item;

		എങ്കിൽ (repo_parse_commit(r, this->parents->item) ||
		    repo_parse_commit(r, stash)) {
			res = error(_("cannot parse parents of commit: %s"),
					oid_to_hex(&this->object.oid));
			പോകുക out;
		}

		എങ്കിൽ (check_stash_topology(r, stash)) {
			res = error(_("%s does not look like a stash commit"),
					oid_to_hex(&stash->object.oid));
			പോകുക out;
		}

		repo_unuse_commit_buffer(r, this, buffer);
		buffer = ശൂന്യ;
		ഇനങ്ങൾ = commit_list_തിരുകുക(stash, &ഇനങ്ങൾ);
		this = this->parents->item;
	}

	/*
	 * Now, walk each entry, adding it to the stash as a normal stash
	 * commit.
	 */
	വേണ്ടി (cur = ഇനങ്ങൾ; cur; cur = cur->next) {
		സ്ഥിരം അക്ഷരം *p;
		ഘടന object_id *oid;

		this = cur->item;
		oid = &this->object.oid;
		buffer = repo_get_commit_buffer(r, this, &bufsize);
		എങ്കിൽ (!buffer) {
			res = error(_("cannot read commit buffer for %s"), oid_to_hex(oid));
			പോകുക out;
		}

		p = strstr(buffer, "\n\n");
		എങ്കിൽ (!p) {
			res = error(_("cannot parse commit %s"), oid_to_hex(oid));
			പോകുക out;
		}

		p += 2;
		സന്ദേശം = xmemdupz(p, bufsize - (p - buffer));
		repo_unuse_commit_buffer(r, this, buffer);
		buffer = ശൂന്യ;

		എങ്കിൽ (do_store_stash(oid, സന്ദേശം, 1)) {
			res = error(_("cannot save the stash for %s"), oid_to_hex(oid));
			പോകുക out;
		}
		FREE_AND_NULL(സന്ദേശം);
	}
out:
	എങ്കിൽ (this && buffer)
		repo_unuse_commit_buffer(r, this, buffer);
	commit_list_free(ഇനങ്ങൾ);
	സ്വതന്ത്രം(സന്ദേശം);

	തിരികെ res;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ import_stash(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
			ഘടന repository *repo)
{
	ഘടന option options[] = {
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_import_usage,
			     PARSE_OPT_KEEP_DASHDASH);

	എങ്കിൽ (argc != 1)
		usage_സന്ദേശം_opt("a revision is required", git_stash_import_usage, options);

	തിരികെ do_import_stash(repo, argv[0]);
}

ഘടന stash_entry_data {
	ഘടന repository *r;
	ഘടന commit_list **ഇനങ്ങൾ;
	size_t എണ്ണം;
};

സ്ഥിതം പൂർണ്ണസംഖ്യ collect_stash_entries(സ്ഥിരം അക്ഷരം *refname UNUSED,
				 ഘടന object_id *old_oid UNUSED,
				 ഘടന object_id *new_oid,
				 സ്ഥിരം അക്ഷരം *committer UNUSED,
				 timestamp_t timestamp UNUSED,
				 പൂർണ്ണസംഖ്യ tz UNUSED, സ്ഥിരം അക്ഷരം *സന്ദേശം UNUSED,
				 ശൂന്യം *cb_data)
{
	ഘടന stash_entry_data *data = cb_data;
	ഘടന commit *stash;

	data->എണ്ണം++;
	stash = lookup_commit_reference(data->r, new_oid);
	എങ്കിൽ (!stash || check_stash_topology(data->r, stash)) {
		തിരികെ error(_("%s does not look like a stash commit"),
			     oid_to_hex(new_oid));
	}
	data->ഇനങ്ങൾ = commit_list_append(stash, data->ഇനങ്ങൾ);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ do_export_stash(ഘടന repository *r,
			   സ്ഥിരം അക്ഷരം *ref,
			   പൂർണ്ണസംഖ്യ argc,
			   സ്ഥിരം അക്ഷരം **argv)
{
	ഘടന object_id base;
	ഘടന commit *prev;
	ഘടന commit_list *ഇനങ്ങൾ = ശൂന്യ, **iter = &ഇനങ്ങൾ, *cur;
	പൂർണ്ണസംഖ്യ res = 0;
	പൂർണ്ണസംഖ്യ i;
	ഘടന strbuf revision = STRBUF_INIT;
	സ്ഥിരം അക്ഷരം *author, *committer;

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
	എങ്കിൽ (commit_വൃക്ഷം_extended("", 0, r->hash_algo->empty_വൃക്ഷം, ശൂന്യ,
				 &base, author, committer, ശൂന്യ, ശൂന്യ))
		തിരികെ error(_("unable to write base commit"));

	prev = lookup_commit_reference(r, &base);

	എങ്കിൽ (argc) {
		/*
		 * Find each specified stash, and load data into the array.
		 */
		വേണ്ടി (i = 0; i < argc; i++) {
			ഘടന object_id oid;
			ഘടന commit *stash;

			എങ്കിൽ (parse_stash_revision(&revision, argv[i], 1) ||
			    repo_get_oid_with_flags(r, revision.buf, &oid,
						    GET_OID_QUIETLY |
						    GET_OID_GENTLY)) {
				res = error(_("unable to find stash entry %s"), argv[i]);
				പോകുക out;
			}

			stash = lookup_commit_reference(r, &oid);
			എങ്കിൽ (!stash || check_stash_topology(r, stash)) {
				res = error(_("%s does not look like a stash commit"),
						revision.buf);
				പോകുക out;
			}
			iter = commit_list_append(stash, iter);
		}
	} അല്ലെങ്കിൽ {
		/*
		 * Walk the reflog, finding each stash entry, and load data into the
		 * array.
		 */
		ഘടന stash_entry_data cb_data = {
			.r = r, .ഇനങ്ങൾ = iter,
		};
		എങ്കിൽ (refs_for_each_reflog_ent_reverse(get_main_ref_store(r),
						     "refs/stash",
						     collect_stash_entries,
						     &cb_data) && cb_data.എണ്ണം)
			പോകുക out;
	}

	/*
	 * Now, create a set of commits identical to the regular stash commits,
	 * but where their first parents form a chain to our original empty
	 * base commit.
	 */
	ഇനങ്ങൾ = commit_list_reverse(ഇനങ്ങൾ);
	വേണ്ടി (cur = ഇനങ്ങൾ; cur; cur = cur->next) {
		ഘടന commit_list *parents = ശൂന്യ;
		ഘടന commit_list **next = &parents;
		ഘടന object_id out;
		ഘടന commit *stash = cur->item;

		next = commit_list_append(prev, next);
		next = commit_list_append(stash, next);
		res = write_commit_with_parents(r, &out, &stash->object.oid, parents);
		commit_list_free(parents);
		എങ്കിൽ (res)
			പോകുക out;
		prev = lookup_commit_reference(r, &out);
	}
	എങ്കിൽ (ref)
		refs_update_ref(get_main_ref_store(r), ശൂന്യ, ref,
				&prev->object.oid, ശൂന്യ, 0, UPDATE_REFS_DIE_ON_ERR);
	അല്ലെങ്കിൽ
		puts(oid_to_hex(&prev->object.oid));
out:
	strbuf_release(&revision);
	commit_list_free(ഇനങ്ങൾ);

	തിരികെ res;
}

എണ്ണൽ export_action {
	ACTION_NONE,
	ACTION_PRINT,
	ACTION_TO_REF,
};

സ്ഥിതം പൂർണ്ണസംഖ്യ export_stash(പൂർണ്ണസംഖ്യ argc,
			സ്ഥിരം അക്ഷരം **argv,
			സ്ഥിരം അക്ഷരം *prefix,
			ഘടന repository *repo)
{
	സ്ഥിരം അക്ഷരം *ref = ശൂന്യ;
	എണ്ണൽ export_action action = ACTION_NONE;
	ഘടന option options[] = {
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

	എങ്കിൽ (ref && action == ACTION_NONE)
		action = ACTION_TO_REF;

	എങ്കിൽ (action == ACTION_NONE || (ref && action == ACTION_PRINT))
		തിരികെ error(_("exactly one of --print and --to-ref is required"));

	തിരികെ do_export_stash(repo, ref, argc, argv);
}

പൂർണ്ണസംഖ്യ cmd_stash(പൂർണ്ണസംഖ്യ argc,
	      സ്ഥിരം അക്ഷരം **argv,
	      സ്ഥിരം അക്ഷരം *prefix,
	      ഘടന repository *repo)
{
	pid_t pid = getpid();
	സ്ഥിരം അക്ഷരം *index_file;
	ഘടന strvec args = STRVEC_INIT;
	parse_opt_subcommand_fn *fn = ശൂന്യ;
	ഘടന option options[] = {
		OPT_SUBCOMMAND("apply", &fn, apply_stash),
		OPT_SUBCOMMAND("clear", &fn, clear_stash),
		OPT_SUBCOMMAND("drop", &fn, drop_stash),
		OPT_SUBCOMMAND("pop", &fn, എടുക്കുക_stash),
		OPT_SUBCOMMAND("branch", &fn, branch_stash),
		OPT_SUBCOMMAND("list", &fn, list_stash),
		OPT_SUBCOMMAND("show", &fn, show_stash),
		OPT_SUBCOMMAND("store", &fn, store_stash),
		OPT_SUBCOMMAND("create", &fn, create_stash),
		OPT_SUBCOMMAND("push", &fn, തള്ളുക_stash_unassumed),
		OPT_SUBCOMMAND("export", &fn, export_stash),
		OPT_SUBCOMMAND("import", &fn, import_stash),
		OPT_SUBCOMMAND_F("save", &fn, save_stash, ശൂന്യ, PARSE_OPT_NOCOMPLETE),
		OPT_END()
	};
	സ്ഥിരം അക്ഷരം **args_copy;
	പൂർണ്ണസംഖ്യ ret;

	repo_config(the_repository, git_stash_config, ശൂന്യ);

	argc = parse_options(argc, argv, prefix, options, git_stash_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL |
			     PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	index_file = repo_get_index_file(the_repository);
	strbuf_addf(&stash_index_path, "%s.stash.%" PRIuMAX, index_file,
		    (uintmax_t)pid);

	എങ്കിൽ (fn) {
		ret = fn(argc, argv, prefix, repo);

		/*
		 * The subcommand implementations return 0 on success, a
		 * negative value on failure, and STASH_APPLY_CONFLICT
		 * when applying a stash entry resulted in conflicts.
		 * Map failures to 128, the status die() uses, so that
		 * exit status 1 unambiguously indicates conflicts.
		 */
		എങ്കിൽ (ret < 0)
			തിരികെ 128;
		തിരികെ ret;
	} അല്ലെങ്കിൽ എങ്കിൽ (!argc) {
		തിരികെ !!തള്ളുക_stash_unassumed(0, ശൂന്യ, prefix, repo);
	}

	/* Assume 'stash push' */
	strvec_തള്ളുക(&args, "push");
	strvec_pushv(&args, argv);

	/*
	 * `push_stash()` ends up modifying the array, which causes memory
	 * leaks if we didn't copy the array here.
	 */
	DUP_ARRAY(args_copy, args.v, args.nr);

	ret = !!തള്ളുക_stash(args.nr, args_copy, prefix, 1);

	strvec_clear(&args);
	സ്വതന്ത്രം(args_copy);
	തിരികെ ret;
}
