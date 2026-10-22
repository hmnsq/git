#وصف_ڪريو USE_THE_REPOSITORY_VARIABLE

#ضمن "builtin.h"
#ضمن "abspath.h"
#ضمن "config.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "hash.h"
#ضمن "hex.h"
#ضمن "object-name.h"
#ضمن "parse-options.h"
#ضمن "refs.h"
#ضمن "stash.h"
#ضمن "lockfile.h"
#ضمن "cache-tree.h"
#ضمن "unpack-trees.h"
#ضمن "merge-ort-wrappers.h"
#ضمن "strvec.h"
#ضمن "run-command.h"
#ضمن "dir.h"
#ضمن "entry.h"
#ضمن "preload-index.h"
#ضمن "read-cache.h"
#ضمن "repository.h"
#ضمن "rerere.h"
#ضمن "revision.h"
#ضمن "setup.h"
#ضمن "sparse-index.h"
#ضمن "log-tree.h"
#ضمن "diffcore.h"
#ضمن "reflog.h"
#ضمن "reflog-walk.h"
#ضمن "add-interactive.h"
#ضمن "commit.h"

#وصف_ڪريو INCLUDE_ALL_FILES 2

#وصف_ڪريو BUILTIN_STASH_LIST_USAGE \
	N_("git stash list [<log-options>]")
#وصف_ڪريو BUILTIN_STASH_SHOW_USAGE \
	N_("git stash show [-u | --include-untracked | --only-untracked] [<diff-options>] [<stash>]")
#وصف_ڪريو BUILTIN_STASH_DROP_USAGE \
	N_("git stash drop [-q | --quiet] [<stash>]")
#وصف_ڪريو BUILTIN_STASH_ڪڍو_USAGE \
	N_("git stash pop [--index] [-q | --quiet] [<stash>]")
#وصف_ڪريو BUILTIN_STASH_APPLY_USAGE \
	N_("git stash apply [--index] [-q | --quiet] [--label-ours=<label>] [--label-theirs=<label>] [--label-base=<label>] [<stash>]")
#وصف_ڪريو BUILTIN_STASH_BRANCH_USAGE \
	N_("git stash branch <branchname> [<stash>]")
#وصف_ڪريو BUILTIN_STASH_STORE_USAGE \
	N_("git stash store [(-m | --message) <message>] [-q | --quiet] <commit>")
#وصف_ڪريو BUILTIN_STASH_ڌڪو_USAGE \
	N_("git stash [push] [-p | --patch] [-S | --staged] [-k | --[no-]keep-index] [-q | --quiet]\n" \
	   "          [-u | --include-untracked] [-a | --all] [(-m | --message) <message>]\n" \
	   "          [--pathspec-from-file=<file> [--pathspec-file-nul]]\n" \
	   "          [--] [<pathspec>...]")
#وصف_ڪريو BUILTIN_STASH_SAVE_USAGE \
	N_("git stash save [-p | --patch] [-S | --staged] [-k | --[no-]keep-index] [-q | --quiet]\n" \
	   "          [-u | --include-untracked] [-a | --all] [<message>]")
#وصف_ڪريو BUILTIN_STASH_CREATE_USAGE \
	N_("git stash create [<message>]")
#وصف_ڪريو BUILTIN_STASH_EXPORT_USAGE \
	N_("git stash export (--print | --to-ref <ref>) [<stash>...]")
#وصف_ڪريو BUILTIN_STASH_IMPORT_USAGE \
	N_("git stash import <commit>")
#وصف_ڪريو BUILTIN_STASH_CLEAR_USAGE \
	"git stash clear"

ساكن ثابت حرف * ثابت git_stash_usage[] = {
	BUILTIN_STASH_LIST_USAGE,
	BUILTIN_STASH_SHOW_USAGE,
	BUILTIN_STASH_DROP_USAGE,
	BUILTIN_STASH_ڪڍو_USAGE,
	BUILTIN_STASH_APPLY_USAGE,
	BUILTIN_STASH_BRANCH_USAGE,
	BUILTIN_STASH_ڌڪو_USAGE,
	BUILTIN_STASH_SAVE_USAGE,
	BUILTIN_STASH_CLEAR_USAGE,
	BUILTIN_STASH_CREATE_USAGE,
	BUILTIN_STASH_STORE_USAGE,
	BUILTIN_STASH_EXPORT_USAGE,
	BUILTIN_STASH_IMPORT_USAGE,
	عدم
};

ساكن ثابت حرف * ثابت git_stash_list_usage[] = {
	BUILTIN_STASH_LIST_USAGE,
	عدم
};

ساكن ثابت حرف * ثابت git_stash_show_usage[] = {
	BUILTIN_STASH_SHOW_USAGE,
	عدم
};

ساكن ثابت حرف * ثابت git_stash_drop_usage[] = {
	BUILTIN_STASH_DROP_USAGE,
	عدم
};

ساكن ثابت حرف * ثابت git_stash_ڪڍو_usage[] = {
	BUILTIN_STASH_ڪڍو_USAGE,
	عدم
};

ساكن ثابت حرف * ثابت git_stash_apply_usage[] = {
	BUILTIN_STASH_APPLY_USAGE,
	عدم
};

ساكن ثابت حرف * ثابت git_stash_branch_usage[] = {
	BUILTIN_STASH_BRANCH_USAGE,
	عدم
};

ساكن ثابت حرف * ثابت git_stash_clear_usage[] = {
	BUILTIN_STASH_CLEAR_USAGE,
	عدم
};

ساكن ثابت حرف * ثابت git_stash_store_usage[] = {
	BUILTIN_STASH_STORE_USAGE,
	عدم
};

ساكن ثابت حرف * ثابت git_stash_ڌڪو_usage[] = {
	BUILTIN_STASH_ڌڪو_USAGE,
	عدم
};

ساكن ثابت حرف * ثابت git_stash_save_usage[] = {
	BUILTIN_STASH_SAVE_USAGE,
	عدم
};

ساكن ثابت حرف * ثابت git_stash_export_usage[] = {
	BUILTIN_STASH_EXPORT_USAGE,
	عدم
};

ساكن ثابت حرف * ثابت git_stash_import_usage[] = {
	BUILTIN_STASH_IMPORT_USAGE,
	عدم
};

ساكن ثابت حرف ref_stash[] = "refs/stash";
ساكن بنية strbuf stash_اشاريو_رستو = STRBUF_INIT;

ساكن صحيح show_stat = 1;
ساكن صحيح show_patch;
ساكن صحيح show_include_untracked;
ساكن صحيح use_اشاريو;

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
بنية stash_info {
	بنية object_id w_commit;
	بنية object_id b_commit;
	بنية object_id i_commit;
	بنية object_id u_commit;
	بنية object_id c_commit;
	بنية object_id w_وڻ;
	بنية object_id b_وڻ;
	بنية object_id i_وڻ;
	بنية object_id u_وڻ;
	بنية strbuf revision;
	صحيح is_stash_ref;
	صحيح has_u;
};

#وصف_ڪريو STASH_INFO_INIT { \
	.revision = STRBUF_INIT, \
}

ساكن فراغ free_stash_info(بنية stash_info *info)
{
	strbuf_release(&info->revision);
}

ساكن صحيح check_stash_topology(بنية repository *r, بنية commit *stash)
{
	بنية commit *p1, *p2, *p3 = عدم;

	/* stash must have two or three parents */
	إذا (!stash->parents || !stash->parents->ايندڙ ||
			(stash->parents->ايندڙ->ايندڙ && stash->parents->ايندڙ->ايندڙ->ايندڙ))
		أرجع -1;
	p1 = stash->parents->شي;
	p2 = stash->parents->ايندڙ->شي;
	إذا (stash->parents->ايندڙ->ايندڙ)
		p3 = stash->parents->ايندڙ->ايندڙ->شي;
	إذا (repo_parse_commit(r, p1) || repo_parse_commit(r, p2) ||
			(p3 && repo_parse_commit(r, p3)))
		أرجع -1;
	/* p2 must have a single parent, p3 must have no parents */
	إذا (!p2->parents || p2->parents->ايندڙ || (p3 && p3->parents))
		أرجع -1;
	إذا (repo_parse_commit(r, p2->parents->شي))
		أرجع -1;
	/* p2^1 must equal p1 */
	إذا (!oideq(&p1->object.oid, &p2->parents->شي->object.oid))
		أرجع -1;

	أرجع 0;
}

ساكن فراغ assert_stash_like(بنية stash_info *info, ثابت حرف *revision)
{
	إذا (get_oidf(&info->b_commit, "%s^1", revision) ||
	    get_oidf(&info->w_وڻ, "%s:", revision) ||
	    get_oidf(&info->b_وڻ, "%s^1:", revision) ||
	    get_oidf(&info->i_وڻ, "%s^2:", revision))
		die(_("'%s' is not a stash-like commit"), revision);
}

ساكن صحيح parse_stash_revision(بنية strbuf *revision, ثابت حرف *commit, صحيح quiet)
{
	strbuf_reset(revision);
	إذا (!commit) {
		إذا (!refs_ref_exists(get_main_ref_store(the_repository), ref_stash)) {
			إذا (!quiet)
				fprintf_ln(stderr, _("No stash entries found."));
			أرجع -1;
		}

		strbuf_addf(revision, "%s@{0}", ref_stash);
	} وإلا إذا (strspn(commit, "0123456789") == لڙي_ڊيگهه(commit)) {
		strbuf_addf(revision, "%s@{%s}", ref_stash, commit);
	} وإلا {
		strbuf_addstr(revision, commit);
	}
	أرجع 0;
}

ساكن صحيح get_stash_info(بنية stash_info *info, صحيح argc, ثابت حرف **argv)
{
	صحيح ret;
	حرف *پڇاڙي_جو_rev;
	حرف *expanded_ref;
	ثابت حرف *revision;
	ثابت حرف *commit = عدم;
	بنية object_id dummy;
	بنية strbuf symbolic = STRBUF_INIT;

	إذا (argc > 1) {
		صحيح i;
		بنية strbuf refs_نياپو = STRBUF_INIT;

		لكل (i = 0; i < argc; i++)
			strbuf_addf(&refs_نياپو, " '%s'", argv[i]);

		fprintf_ln(stderr, _("Too many revisions specified:%s"),
			   refs_نياپو.buf);
		strbuf_release(&refs_نياپو);

		أرجع -1;
	}

	إذا (argc == 1)
		commit = argv[0];

	strbuf_init(&info->revision, 0);
	إذا (parse_stash_revision(&info->revision, commit, 0)) {
		أرجع -1;
	}

	revision = info->revision.buf;

	إذا (repo_get_oid(the_repository, revision, &info->w_commit))
		أرجع error(_("%s is not a valid reference"), revision);

	assert_stash_like(info, revision);

	info->has_u = !get_oidf(&info->u_وڻ, "%s^3:", revision);

	پڇاڙي_جو_rev = strchrnul(revision, '@');
	strbuf_جوڙيو(&symbolic, revision, پڇاڙي_جو_rev - revision);

	ret = repo_dwim_ref(the_repository, symbolic.buf, symbolic.len,
			    &dummy, &expanded_ref, 0);
	strbuf_release(&symbolic);
	بدل (ret) {
	حالة 0: /* Not found, but valid ref */
		info->is_stash_ref = 0;
		اكسر;
	حالة 1:
		info->is_stash_ref = !strcmp(expanded_ref, ref_stash);
		اكسر;
	افتراضي: /* Invalid or ambiguous */
		اكسر;
	}

	حرر(expanded_ref);
	أرجع !(ret == 0 || ret == 1);
}

ساكن صحيح do_clear_stash(فراغ)
{
	بنية object_id obj;
	إذا (repo_get_oid(the_repository, ref_stash, &obj))
		أرجع 0;

	أرجع refs_delete_ref(get_main_ref_store(the_repository), عدم,
			       ref_stash, &obj, 0);
}

ساكن صحيح clear_stash(صحيح argc, ثابت حرف **argv, ثابت حرف *اڳياڙي,
		       بنية repository *repo اڻ_استعمال_ٿيل)
{
	بنية option options[] = {
		OPT_پڇاڙي()
	};

	argc = parse_options(argc, argv, اڳياڙي, options,
			     git_stash_clear_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	إذا (argc)
		أرجع error(_("git stash clear with arguments is "
			       "unimplemented"));

	أرجع do_clear_stash();
}

ساكن صحيح reset_وڻ(بنية object_id *i_وڻ, صحيح update, صحيح reset)
{
	صحيح nr_trees = 1;
	بنية unpack_trees_options opts;
	بنية وڻ_desc t[MAX_UNPACK_TREES];
	بنية وڻ *وڻ;
	بنية تالو_file تالو_file = تالو_INIT;

	repo_read_اشاريو_preload(the_repository, عدم, 0);
	إذا (refresh_اشاريو(the_repository->اشاريو, REFRESH_QUIET, عدم, عدم, عدم))
		أرجع -1;

	repo_hold_locked_اشاريو(the_repository, &تالو_file, تالو_DIE_ON_ERROR);

	memset(&opts, 0, حجم(opts));

	وڻ = repo_parse_وڻ_اڻسڌو(the_repository, i_وڻ);
	إذا (repo_parse_وڻ(the_repository, وڻ))
		أرجع -1;

	init_وڻ_desc(t, &وڻ->object.oid, وڻ->بفر, وڻ->سائيز);

	opts.سر_idx = 1;
	opts.src_اشاريو = the_repository->اشاريو;
	opts.dst_اشاريو = the_repository->اشاريو;
	opts.ملايو = 1;
	opts.reset = reset ? UNPACK_RESET_PROTECT_UNTRACKED : 0;
	opts.update = update;
	إذا (update)
		opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
	opts.fn = oneway_ملايو;

	إذا (unpack_trees(nr_trees, t, &opts))
		أرجع -1;

	إذا (write_locked_اشاريو(the_repository->اشاريو, &تالو_file, COMMIT_تالو))
		أرجع error(_("unable to write new index file"));

	أرجع 0;
}

ساكن صحيح create_اشاريو_from_وڻ(ثابت بنية object_id *وڻ_id,
				  ثابت حرف *اشاريو_رستو)
{
	صحيح nr_trees = 1;
	صحيح ret = 0;
	بنية unpack_trees_options opts;
	بنية وڻ_desc t[MAX_UNPACK_TREES];
	بنية وڻ *وڻ;
	بنية اشاريو_state dst_istate = اشاريو_STATE_INIT(the_repository);
	بنية تالو_file تالو_file = تالو_INIT;

	repo_read_اشاريو_preload(the_repository, عدم, 0);
	refresh_اشاريو(the_repository->اشاريو, REFRESH_QUIET, عدم, عدم, عدم);

	hold_تالو_file_for_update(&تالو_file, اشاريو_رستو, تالو_DIE_ON_ERROR);

	memset(&opts, 0, حجم(opts));

	وڻ = repo_parse_وڻ_اڻسڌو(the_repository, وڻ_id);
	إذا (!وڻ || repo_parse_وڻ(the_repository, وڻ)) {
		ret = -1;
		اذهب پورو;
	}

	init_وڻ_desc(t, &وڻ->object.oid, وڻ->بفر, وڻ->سائيز);

	opts.سر_idx = 1;
	opts.src_اشاريو = the_repository->اشاريو;
	opts.dst_اشاريو = &dst_istate;
	opts.ملايو = 1;
	opts.reset = UNPACK_RESET_PROTECT_UNTRACKED;
	opts.fn = oneway_ملايو;

	إذا (unpack_trees(nr_trees, t, &opts)) {
		ret = -1;
		اذهب پورو;
	}

	إذا (write_locked_اشاريو(&dst_istate, &تالو_file, COMMIT_تالو)) {
		ret = error(_("unable to write new index file"));
		اذهب پورو;
	}

پورو:
	release_اشاريو(&dst_istate);
	إذا (ret)
		rollback_تالو_file(&تالو_file);
	أرجع ret;
}

ساكن صحيح is_رستو_a_directory(ثابت حرف *رستو)
{
	/*
	 * This function differs from abspath.c:is_directory() in that
	 * here we use lstat() instead of stat(); we do not want to
	 * follow symbolic links here.
	 */
	بنية stat st;
	أرجع (!lstat(رستو, &st) && S_ISDIR(st.st_گهڻائي));
}

ساكن فراغ جوڙيو_diff_منزل_buf(بنية diff_قطار_struct *q,
			    بنية diff_options *options اڻ_استعمال_ٿيل,
			    فراغ *معلومات)
{
	صحيح i;

	لكل (i = 0; i < q->nr; i++) {
		إذا (is_رستو_a_directory(q->قطار[i]->one->رستو))
			استمر;

		strbuf_addstr(معلومات, q->قطار[i]->one->رستو);

		/* NUL-terminate: will be fed to update-index -z */
		strbuf_addch(معلومات, '\0');
	}
}

ساكن صحيح restore_untracked(بنية object_id *u_وڻ)
{
	صحيح res;
	بنية ٻار_process cp = ٻار_PROCESS_INIT;

	/*
	 * We need to run restore files from a given index, but without
	 * affecting the current index, so we use GIT_INDEX_FILE with
	 * run_command to fork processes that will not interfere.
	 */
	cp.git_cmd = 1;
	strvec_ڌڪو(&cp.دليلون, "read-tree");
	strvec_ڌڪو(&cp.دليلون, oid_منزل_hex(u_وڻ));
	strvec_pushf(&cp.env, "GIT_INDEX_FILE=%s",
		     stash_اشاريو_رستو.buf);
	إذا (لاڳيتو_command(&cp)) {
		remove_رستو(stash_اشاريو_رستو.buf);
		أرجع -1;
	}

	ٻار_process_init(&cp);
	cp.git_cmd = 1;
	strvec_pushl(&cp.دليلون, "checkout-index", "--all", عدم);
	strvec_pushf(&cp.env, "GIT_INDEX_FILE=%s",
		     stash_اشاريو_رستو.buf);

	res = لاڳيتو_command(&cp);
	remove_رستو(stash_اشاريو_رستو.buf);
	أرجع res;
}

ساكن فراغ unstage_changes_unless_new(بنية object_id *orig_وڻ)
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

	بنية checkout state = CHECKOUT_INIT;
	بنية diff_options diff_opts;
	بنية تالو_file تالو = تالو_INIT;
	صحيح i;

	/* If any entries have skip_worktree set, we'll have to check 'em out */
	state.force = 1;
	state.quiet = 1;
	state.refresh_cache = 1;
	state.istate = the_repository->اشاريو;

	/*
	 * Step 1: get a difference between orig_tree (which corresponding
	 * to the index before a merge was run) and the current index
	 * (reflecting the changes brought in by the merge).
	 */
	repo_diff_تياري(the_repository, &diff_opts);
	diff_opts.flags.recursive = 1;
	diff_opts.detect_rename = 0;
	diff_opts.output_format = DIFF_FORMAT_NO_OUTPUT;
	diff_تياري_پورو(&diff_opts);

	do_diff_cache(orig_وڻ, &diff_opts);
	diffcore_std(&diff_opts);

	/* Iterate over the paths that changed due to the merge... */
	لكل (i = 0; i < diff_queued_diff.nr; i++) {
		بنية diff_filepair *p;
		بنية cache_داخلا *ce;
		صحيح pos;

		/* Look up the path's position in the current index. */
		p = diff_queued_diff.قطار[i];
		pos = اشاريو_name_pos(the_repository->اشاريو, p->two->رستو,
				     لڙي_ڊيگهه(p->two->رستو));

		/*
		 * Step 2: Place changes in the working tree
		 *
		 * Stash is about restoring changes *to the working tree*.
		 * So if the merge successfully got a new version of some
		 * path, but left it out of the working tree, then clear the
		 * SKIP_WORKTREE bit and write it to the working tree.
		 */
		إذا (pos >= 0 && ce_skip_worktree(the_repository->اشاريو->cache[pos])) {
			بنية stat st;

			ce = the_repository->اشاريو->cache[pos];
			إذا (!lstat(ce->name, &st)) {
				/* Conflicting path present; relocate it */
				بنية strbuf new_رستو = STRBUF_INIT;
				صحيح fd;

				strbuf_addf(&new_رستو,
					    "%s.stash.XXXXXX", ce->name);
				fd = xmkstemp(new_رستو.buf);
				close(fd);
				لکو(_("WARNING: Untracked file in way of "
					 "tracked file!  Renaming\n "
					 "           %s -> %s\n"
					 "         to make room.\n"),
				       ce->name, new_رستو.buf);
				إذا (rename(ce->name, new_رستو.buf))
					die("Failed to move %s to %s",
					    ce->name, new_رستو.buf);
				strbuf_release(&new_رستو);
			}
			checkout_داخلا(ce, &state, عدم, عدم);
			ce->ce_flags &= ~CE_SKIP_WORKTREE;
		}

		/*
		 * Step 3: "unstage" changes, as long as they are still tracked
		 */
		إذا (p->one->oid_valid) {
			/*
			 * Path existed in orig_tree; restore index entry
			 * from that tree in order to "unstage" the changes.
			 */
			صحيح option = جوڙيو_CACHE_OK_منزل_REPLACE;
			إذا (pos < 0)
				option = جوڙيو_CACHE_OK_منزل_جوڙيو;

			ce = make_cache_داخلا(the_repository->اشاريو,
					      p->one->گهڻائي,
					      &p->one->oid,
					      p->one->رستو,
					      0, 0);
			جوڙيو_اشاريو_داخلا(the_repository->اشاريو, ce, option);
		}
	}
	diff_flush(&diff_opts);

	/*
	 * Step 4: write the new index to disk
	 */
	repo_hold_locked_اشاريو(the_repository, &تالو, تالو_DIE_ON_ERROR);
	إذا (write_locked_اشاريو(the_repository->اشاريو, &تالو,
			       COMMIT_تالو | SKIP_IF_UNCHANGED))
		die(_("could not write index"));
}

ساكن تعداد stash_apply_نتيجو do_apply_stash(ثابت حرف *اڳياڙي,
					      بنية stash_info *info,
					      صحيح اشاريو, صحيح quiet,
					      ثابت حرف *label_ours,
					      ثابت حرف *label_theirs,
					      ثابت حرف *label_بنياد)
{
	صحيح clean, ret;
	صحيح has_اشاريو = اشاريو;
	بنية ملايو_options o;
	بنية object_id c_وڻ;
	بنية object_id اشاريو_وڻ;
	بنية وڻ *سر, *ملايو, *ملايو_بنياد;
	بنية تالو_file تالو = تالو_INIT;

	repo_read_اشاريو_preload(the_repository, عدم, 0);
	إذا (repo_refresh_and_write_اشاريو(the_repository, REFRESH_QUIET, 0, 0,
					 عدم, عدم, عدم))
		أرجع error(_("could not write index"));

	إذا (write_اشاريو_as_وڻ(&c_وڻ, the_repository->اشاريو,
				repo_get_اشاريو_file(the_repository), 0, عدم))
		أرجع error(_("cannot apply a stash in the middle of a merge"));

	init_ui_ملايو_options(&o, the_repository);

	إذا (quiet)
		o.verbosity = 0;

	إذا (اشاريو) {
		إذا (oideq(&info->b_وڻ, &info->i_وڻ) ||
		    oideq(&c_وڻ, &info->i_وڻ)) {
			has_اشاريو = 0;
		} وإلا {
			بنية ملايو_نتيجو نتيجو = { 0 };

			o.branch1 = "Current index";
			o.branch2 = "Stashed index changes";
			o.ancestor = "Stash base";

			سر = lookup_وڻ(o.repo, &c_وڻ);
			ملايو = lookup_وڻ(o.repo, &info->i_وڻ);
			ملايو_بنياد = lookup_وڻ(o.repo, &info->b_وڻ);

			ملايو_incore_nonrecursive(&o, ملايو_بنياد, سر, ملايو,
						  &نتيجو);

			إذا (نتيجو.clean < 0) {
				ملايو_finalize(&o, &نتيجو);
				أرجع error(_("index merge failed"));
			} وإلا إذا (!نتيجو.clean) {
				ملايو_finalize(&o, &نتيجو);
				أرجع error(_("conflicts in index. "
					       "Try without --index."));
			} وإلا {
				oidcpy(&اشاريو_وڻ, &نتيجو.وڻ->object.oid);
				ملايو_finalize(&o, &نتيجو);
			}
		}
	}

	o.branch1 = label_ours ? label_ours : "Updated upstream";
	o.branch2 = label_theirs ? label_theirs : "Stashed changes";
	o.ancestor = label_بنياد ? label_بنياد : "Stash base";

	إذا (oideq(&info->b_وڻ, &c_وڻ))
		o.branch1 = "Version stash was based on";

	إذا (o.verbosity >= 3)
		printf_ln(_("Merging %s with %s"), o.branch1, o.branch2);

	سر = lookup_وڻ(o.repo, &c_وڻ);
	ملايو = lookup_وڻ(o.repo, &info->w_وڻ);
	ملايو_بنياد = lookup_وڻ(o.repo, &info->b_وڻ);

	repo_hold_locked_اشاريو(o.repo, &تالو, تالو_DIE_ON_ERROR);
	clean = ملايو_ort_nonrecursive(&o, سر, ملايو, ملايو_بنياد);

	/*
	 * If 'clean' >= 0, reverse the value for 'ret' so 'ret' is 0 when the
	 * merge was clean, and 1 if the merge was unclean or a negative value
	 * if it encountered an error.
	 */
	ret = clean >= 0 ? !clean : clean;

	إذا (ret < 0)
		rollback_تالو_file(&تالو);
	وإلا إذا (write_locked_اشاريو(o.repo->اشاريو, &تالو,
				      COMMIT_تالو | SKIP_IF_UNCHANGED))
		ret = error(_("could not write index"));

	إذا (ret) {
		repo_rerere(the_repository, 0);

		إذا (اشاريو)
			fprintf_ln(stderr, _("Index was not unstashed."));

		اذهب restore_untracked;
	}

	إذا (has_اشاريو) {
		إذا (reset_وڻ(&اشاريو_وڻ, 0, 0))
			ret = -1;
	} وإلا {
		unstage_changes_unless_new(&c_وڻ);
	}

restore_untracked:
	إذا (info->has_u && restore_untracked(&info->u_وڻ))
		ret = error(_("could not restore untracked files from stash"));

	إذا (!quiet) {
		بنية ٻار_process cp = ٻار_PROCESS_INIT;

		/*
		 * Status is quite simple and could be replaced with calls to
		 * wt_status in the future, but it adds complexities which may
		 * require more tests.
		 */
		cp.git_cmd = 1;
		cp.dir = اڳياڙي;
		strvec_pushf(&cp.env, GIT_WORK_وڻ_ENVIRONMENT"=%s",
			     absolute_رستو(repo_get_work_وڻ(the_repository)));
		strvec_pushf(&cp.env, GIT_DIR_ENVIRONMENT"=%s",
			     absolute_رستو(repo_get_git_dir(the_repository)));
		strvec_ڌڪو(&cp.دليلون, "status");
		لاڳيتو_command(&cp);
	}

	أرجع ret;
}

ساكن صحيح apply_stash(صحيح argc, ثابت حرف **argv, ثابت حرف *اڳياڙي,
		       بنية repository *repo اڻ_استعمال_ٿيل)
{
	صحيح ret = -1;
	صحيح quiet = 0;
	صحيح اشاريو = use_اشاريو;
	ثابت حرف *label_ours = عدم, *label_theirs = عدم, *label_بنياد = عدم;
	بنية stash_info info = STASH_INFO_INIT;
	بنية option options[] = {
		OPT__QUIET(&quiet, N_("be quiet, only report errors")),
		OPT_BOOL(0, "index", &اشاريو,
			 N_("attempt to recreate the index")),
		OPT_STRING(0, "label-ours", &label_ours, N_("label"),
			   N_("label for the upstream side in conflict markers")),
		OPT_STRING(0, "label-theirs", &label_theirs, N_("label"),
			   N_("label for the stashed side in conflict markers")),
		OPT_STRING(0, "label-base", &label_بنياد, N_("label"),
			   N_("label for the base in diff3 conflict markers")),
		OPT_پڇاڙي()
	};

	argc = parse_options(argc, argv, اڳياڙي, options,
			     git_stash_apply_usage, 0);

	إذا (get_stash_info(&info, argc, argv))
		اذهب cleanup;

	ret = do_apply_stash(اڳياڙي, &info, اشاريو, quiet,
			     label_ours, label_theirs, label_بنياد);
cleanup:
	free_stash_info(&info);
	أرجع ret;
}

ساكن صحيح reject_reflog_ent(ثابت حرف *refname اڻ_استعمال_ٿيل,
			     بنية object_id *ooid اڻ_استعمال_ٿيل,
			     بنية object_id *noid اڻ_استعمال_ٿيل,
			     ثابت حرف *email اڻ_استعمال_ٿيل,
			     timestamp_t timestamp اڻ_استعمال_ٿيل,
			     صحيح tz اڻ_استعمال_ٿيل, ثابت حرف *پيغام اڻ_استعمال_ٿيل,
			     فراغ *cb_معلومات اڻ_استعمال_ٿيل)
{
	أرجع 1;
}

ساكن صحيح reflog_is_خالي_جايون(ثابت حرف *refname)
{
	أرجع !refs_for_each_reflog_ent(get_main_ref_store(the_repository),
					 refname, reject_reflog_ent, عدم);
}

ساكن صحيح do_drop_stash(بنية stash_info *info, صحيح quiet)
{
	إذا (!reflog_delete(info->revision.buf,
			   EXPIRE_REFLOGS_REWRITE | EXPIRE_REFLOGS_UPDATE_REF,
			   0)) {
		إذا (!quiet)
			printf_ln(_("Dropped %s (%s)"), info->revision.buf,
				  oid_منزل_hex(&info->w_commit));
	} وإلا {
		أرجع error(_("%s: Could not drop stash entry"),
			     info->revision.buf);
	}

	إذا (reflog_is_خالي_جايون(ref_stash))
		do_clear_stash();

	أرجع 0;
}

ساكن صحيح get_stash_info_assert(بنية stash_info *info, صحيح argc,
				 ثابت حرف **argv)
{
	صحيح ret = get_stash_info(info, argc, argv);

	إذا (ret < 0)
		أرجع ret;

	إذا (!info->is_stash_ref)
		أرجع error(_("'%s' is not a stash reference"), info->revision.buf);

	أرجع 0;
}

ساكن صحيح drop_stash(صحيح argc, ثابت حرف **argv, ثابت حرف *اڳياڙي,
		      بنية repository *repo اڻ_استعمال_ٿيل)
{
	صحيح ret = -1;
	صحيح quiet = 0;
	بنية stash_info info = STASH_INFO_INIT;
	بنية option options[] = {
		OPT__QUIET(&quiet, N_("be quiet, only report errors")),
		OPT_پڇاڙي()
	};

	argc = parse_options(argc, argv, اڳياڙي, options,
			     git_stash_drop_usage, 0);

	إذا (get_stash_info_assert(&info, argc, argv))
		اذهب cleanup;

	ret = do_drop_stash(&info, quiet);
cleanup:
	free_stash_info(&info);
	أرجع ret;
}

ساكن صحيح ڪڍو_stash(صحيح argc, ثابت حرف **argv, ثابت حرف *اڳياڙي,
		     بنية repository *repo اڻ_استعمال_ٿيل)
{
	صحيح ret = -1;
	صحيح اشاريو = use_اشاريو;
	صحيح quiet = 0;
	بنية stash_info info = STASH_INFO_INIT;
	بنية option options[] = {
		OPT__QUIET(&quiet, N_("be quiet, only report errors")),
		OPT_BOOL(0, "index", &اشاريو,
			 N_("attempt to recreate the index")),
		OPT_پڇاڙي()
	};

	argc = parse_options(argc, argv, اڳياڙي, options,
			     git_stash_ڪڍو_usage, 0);

	إذا (get_stash_info_assert(&info, argc, argv))
		اذهب cleanup;

	إذا ((ret = do_apply_stash(اڳياڙي, &info, اشاريو, quiet,
				  عدم, عدم, عدم)))
		printf_ln(_("The stash entry is kept in case "
			    "you need it again."));
	وإلا
		ret = do_drop_stash(&info, quiet);

cleanup:
	free_stash_info(&info);
	أرجع ret;
}

ساكن صحيح branch_stash(صحيح argc, ثابت حرف **argv, ثابت حرف *اڳياڙي,
			بنية repository *repo اڻ_استعمال_ٿيل)
{
	صحيح ret = -1;
	ثابت حرف *branch = عدم;
	بنية stash_info info = STASH_INFO_INIT;
	بنية ٻار_process cp = ٻار_PROCESS_INIT;
	بنية option options[] = {
		OPT_پڇاڙي()
	};

	argc = parse_options(argc, argv, اڳياڙي, options,
			     git_stash_branch_usage, 0);

	إذا (!argc) {
		fprintf_ln(stderr, _("No branch name specified"));
		أرجع -1;
	}

	branch = argv[0];

	إذا (get_stash_info(&info, argc - 1, argv + 1))
		اذهب cleanup;

	cp.git_cmd = 1;
	strvec_pushl(&cp.دليلون, "checkout", "-b", عدم);
	strvec_ڌڪو(&cp.دليلون, branch);
	strvec_ڌڪو(&cp.دليلون, oid_منزل_hex(&info.b_commit));
	ret = لاڳيتو_command(&cp);
	إذا (!ret)
		ret = do_apply_stash(اڳياڙي, &info, 1, 0,
				     عدم, عدم, عدم);
	إذا (!ret && info.is_stash_ref)
		ret = do_drop_stash(&info, 0);

cleanup:
	free_stash_info(&info);
	أرجع ret;
}

ساكن صحيح list_stash(صحيح argc, ثابت حرف **argv, ثابت حرف *اڳياڙي,
		      بنية repository *repo اڻ_استعمال_ٿيل)
{
	بنية ٻار_process cp = ٻار_PROCESS_INIT;
	بنية option options[] = {
		OPT_پڇاڙي()
	};

	argc = parse_options(argc, argv, اڳياڙي, options,
			     git_stash_list_usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT);

	إذا (!refs_ref_exists(get_main_ref_store(the_repository), ref_stash))
		أرجع 0;

	cp.git_cmd = 1;
	strvec_pushl(&cp.دليلون, "log", "--format=%gd: %gs", "-g",
		     "--first-parent", عدم);
	strvec_pushv(&cp.دليلون, argv);
	strvec_ڌڪو(&cp.دليلون, ref_stash);
	strvec_ڌڪو(&cp.دليلون, "--");
	أرجع لاڳيتو_command(&cp);
}

ساكن صحيح git_stash_config(ثابت حرف *var, ثابت حرف *قدر,
			    ثابت بنية config_context *ctx, فراغ *cb)
{
	إذا (!strcmp(var, "stash.showstat")) {
		show_stat = git_config_bool(var, قدر);
		أرجع 0;
	}
	إذا (!strcmp(var, "stash.showpatch")) {
		show_patch = git_config_bool(var, قدر);
		أرجع 0;
	}
	إذا (!strcmp(var, "stash.showincludeuntracked")) {
		show_include_untracked = git_config_bool(var, قدر);
		أرجع 0;
	}
	إذا (!strcmp(var, "stash.index")) {
		use_اشاريو = git_config_bool(var, قدر);
		أرجع 0;
	}
	أرجع git_diff_basic_config(var, قدر, ctx, cb);
}

ساكن فراغ diff_include_untracked(ثابت بنية stash_info *info, بنية diff_options *diff_opt)
{
	ثابت بنية object_id *oid[] = { &info->w_commit, &info->u_وڻ };
	بنية وڻ *وڻ[ARRAY_سائيز(oid)];
	بنية وڻ_desc وڻ_desc[ARRAY_سائيز(oid)];
	بنية unpack_trees_options unpack_وڻ_opt = { 0 };

	لكل (سائيز_t i = 0; i < ARRAY_سائيز(oid); i++) {
		وڻ[i] = repo_parse_وڻ_اڻسڌو(the_repository, oid[i]);
		إذا (repo_parse_وڻ(the_repository, وڻ[i]) < 0)
			die(_("failed to parse tree"));
		init_وڻ_desc(&وڻ_desc[i], &وڻ[i]->object.oid,
			       وڻ[i]->بفر, وڻ[i]->سائيز);
	}

	unpack_وڻ_opt.سر_idx = -1;
	unpack_وڻ_opt.src_اشاريو = the_repository->اشاريو;
	unpack_وڻ_opt.dst_اشاريو = the_repository->اشاريو;
	unpack_وڻ_opt.ملايو = 1;
	unpack_وڻ_opt.fn = stash_worktree_untracked_ملايو;

	إذا (unpack_trees(ARRAY_سائيز(وڻ_desc), وڻ_desc, &unpack_وڻ_opt))
		die(_("failed to unpack trees"));

	do_diff_cache(&info->b_commit, diff_opt);
}

ساكن صحيح show_stash(صحيح argc, ثابت حرف **argv, ثابت حرف *اڳياڙي,
		      بنية repository *repo اڻ_استعمال_ٿيل)
{
	صحيح i;
	صحيح ret = -1;
	بنية stash_info info = STASH_INFO_INIT;
	بنية rev_info rev;
	بنية strvec stash_دليلون = STRVEC_INIT;
	بنية strvec revision_دليلون = STRVEC_INIT;
	تعداد {
		UNTRACKED_NONE,
		UNTRACKED_INCLUDE,
		UNTRACKED_ONLY
	} show_untracked = show_include_untracked ? UNTRACKED_INCLUDE : UNTRACKED_NONE;
	بنية option options[] = {
		OPT_سيٽ_INT('u', "include-untracked", &show_untracked,
			    N_("include untracked files in the stash"),
			    UNTRACKED_INCLUDE),
		OPT_سيٽ_INT_F(0, "only-untracked", &show_untracked,
			      N_("only show untracked files in the stash"),
			      UNTRACKED_ONLY, PARSE_OPT_NONEG),
		OPT_پڇاڙي()
	};
	صحيح do_usage = 0;

	init_diff_ui_defaults();
	repo_config(the_repository, git_diff_ui_config, عدم);
	repo_init_revisions(the_repository, &rev, اڳياڙي);

	argc = parse_options(argc, argv, اڳياڙي, options, git_stash_show_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	strvec_ڌڪو(&revision_دليلون, argv[0]);
	لكل (i = 1; i < argc; i++) {
		إذا (argv[i][0] != '-')
			strvec_ڌڪو(&stash_دليلون, argv[i]);
		وإلا
			strvec_ڌڪو(&revision_دليلون, argv[i]);
	}

	إذا (get_stash_info(&info, stash_دليلون.nr, stash_دليلون.v))
		اذهب cleanup;

	/*
	 * The config settings are applied only if there are not passed
	 * any options.
	 */
	إذا (revision_دليلون.nr == 1) {
		إذا (show_stat)
			rev.diffopt.output_format = DIFF_FORMAT_DIFFSTAT;

		إذا (show_patch)
			rev.diffopt.output_format |= DIFF_FORMAT_PATCH;

		إذا (!show_stat && !show_patch) {
			ret = 0;
			اذهب cleanup;
		}
	}

	تياري_revisions_from_strvec(&revision_دليلون, &rev, عدم);
	إذا (revision_دليلون.nr > 1)
		اذهب usage;
	إذا (!rev.diffopt.output_format) {
		rev.diffopt.output_format = DIFF_FORMAT_PATCH;
		diff_تياري_پورو(&rev.diffopt);
	}

	rev.diffopt.flags.recursive = 1;
	تياري_diff_pager(&rev.diffopt);
	بدل (show_untracked) {
	حالة UNTRACKED_NONE:
		diff_وڻ_oid(&info.b_commit, &info.w_commit, "", &rev.diffopt);
		اكسر;
	حالة UNTRACKED_ONLY:
		إذا (info.has_u)
			diff_پاڙ_وڻ_oid(&info.u_وڻ, "", &rev.diffopt);
		اكسر;
	حالة UNTRACKED_INCLUDE:
		إذا (info.has_u)
			diff_include_untracked(&info, &rev.diffopt);
		وإلا
			diff_وڻ_oid(&info.b_commit, &info.w_commit, "", &rev.diffopt);
		اكسر;
	}
	log_وڻ_diff_flush(&rev);

	ret = diff_نتيجو_ڪوڊ(&rev);

cleanup:
	strvec_clear(&revision_دليلون);
	strvec_clear(&stash_دليلون);
	free_stash_info(&info);
	release_revisions(&rev);
	إذا (do_usage)
		usage_with_options(git_stash_show_usage, options);
	أرجع ret;
usage:
	do_usage = 1;
	اذهب cleanup;
}

ساكن صحيح do_store_stash(ثابت بنية object_id *w_commit, ثابت حرف *stash_نياپو,
			  صحيح quiet)
{
	بنية stash_info info;
	حرف revision[GIT_MAX_HEXSZ];

	oid_منزل_hex_r(revision, w_commit);
	assert_stash_like(&info, revision);

	إذا (!stash_نياپو)
		stash_نياپو = "Created via \"git stash store\".";

	إذا (refs_update_ref(get_main_ref_store(the_repository), stash_نياپو, ref_stash, w_commit, عدم,
			    REF_FORCE_CREATE_REFLOG,
			    quiet ? UPDATE_REFS_QUIET_ON_ERR :
			    UPDATE_REFS_نياپو_ON_ERR)) {
		إذا (!quiet) {
			fprintf_ln(stderr, _("Cannot update %s with %s"),
				   ref_stash, oid_منزل_hex(w_commit));
		}
		أرجع -1;
	}

	أرجع 0;
}

ساكن صحيح store_stash(صحيح argc, ثابت حرف **argv, ثابت حرف *اڳياڙي,
		       بنية repository *repo اڻ_استعمال_ٿيل)
{
	صحيح quiet = 0;
	ثابت حرف *stash_نياپو = عدم;
	بنية object_id obj;
	بنية option options[] = {
		OPT__QUIET(&quiet, N_("be quiet")),
		OPT_STRING('m', "message", &stash_نياپو, "message",
			   N_("stash message")),
		OPT_پڇاڙي()
	};
	صحيح ret;

	argc = parse_options(argc, argv, اڳياڙي, options,
			     git_stash_store_usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT);

	إذا (argc != 1) {
		إذا (!quiet)
			fprintf_ln(stderr, _("\"git stash store\" requires one "
					     "<commit> argument"));
		أرجع -1;
	}

	إذا (repo_get_oid_with_flags(the_repository, argv[0], &obj,
				    quiet ? GET_OID_QUIETLY : 0)) {
		إذا (!quiet)
			fprintf_ln(stderr, _("Cannot update %s with %s"),
					     ref_stash, argv[0]);
		ret = -1;
		اذهب ٻاهريون;
	}

	ret = do_store_stash(&obj, stash_نياپو, quiet);

ٻاهريون:
	أرجع ret;
}

ساكن فراغ جوڙيو_pathspecs(بنية strvec *دليلون,
			  ثابت بنية pathspec *ps) {
	صحيح i;

	لكل (i = 0; i < ps->nr; i++)
		strvec_ڌڪو(دليلون, ps->شيون[i].original);
}

/*
 * `untracked_files` will be filled with the names of untracked files.
 * The return value is:
 *
 * = 0 if there are not any untracked files
 * > 0 if there are untracked files
 */
ساكن صحيح get_untracked_files(ثابت بنية pathspec *ps, صحيح include_untracked,
			       بنية strbuf *untracked_files)
{
	صحيح i;
	صحيح مليو_2 = 0;
	بنية dir_struct dir = DIR_INIT;

	إذا (include_untracked != INCLUDE_ALL_FILES)
		تياري_standard_excludes(&dir);

	fill_directory(&dir, the_repository->اشاريو, ps);
	لكل (i = 0; i < dir.nr; i++) {
		بنية dir_داخلا *ent = dir.entries[i];
		مليو_2++;
		strbuf_addstr(untracked_files, ent->name);
		/* NUL-terminate: will be fed to update-index -z */
		strbuf_addch(untracked_files, '\0');
	}

	dir_clear(&dir);
	أرجع مليو_2;
}

/*
 * The return value of `check_changes_tracked_files()` can be:
 *
 * < 0 if there was an error
 * = 0 if there are no changes.
 * > 0 if there are changes.
 */
ساكن صحيح check_changes_tracked_files(ثابت بنية pathspec *ps)
{
	بنية rev_info rev;
	بنية object_id dummy;
	صحيح ret = 0;

	/* No initial commit. */
	إذا (repo_get_oid(the_repository, "HEAD", &dummy))
		أرجع -1;

	إذا (repo_read_اشاريو(the_repository) < 0)
		أرجع -1;

	repo_init_revisions(the_repository, &rev, عدم);
	copy_pathspec(&rev.prune_معلومات, ps);

	rev.diffopt.flags.تيز = 1;
	rev.diffopt.flags.ignore_submodules = 1;
	rev.abbrev = 0;

	جوڙيو_سر_منزل_pending(&rev);
	diff_تياري_پورو(&rev.diffopt);

	لاڳيتو_diff_اشاريو(&rev, DIFF_اشاريو_CACHED);
	إذا (diff_نتيجو_ڪوڊ(&rev)) {
		ret = 1;
		اذهب پورو;
	}

	لاڳيتو_diff_files(&rev, 0);
	إذا (diff_نتيجو_ڪوڊ(&rev)) {
		ret = 1;
		اذهب پورو;
	}

پورو:
	release_revisions(&rev);
	أرجع ret;
}

/*
 * The function will fill `untracked_files` with the names of untracked files
 * It will return 1 if there were any changes and 0 if there were not.
 */
ساكن صحيح check_changes(ثابت بنية pathspec *ps, صحيح include_untracked,
			 بنية strbuf *untracked_files)
{
	صحيح ret = 0;
	إذا (check_changes_tracked_files(ps))
		ret = 1;

	إذا (include_untracked && get_untracked_files(ps, include_untracked,
						     untracked_files))
		ret = 1;

	أرجع ret;
}

ساكن صحيح save_untracked_files(بنية stash_info *info, بنية strbuf *نياپو,
				بنية strbuf *files)
{
	صحيح ret = 0;
	بنية strbuf untracked_نياپو = STRBUF_INIT;
	بنية ٻار_process cp_upd_اشاريو = ٻار_PROCESS_INIT;
	بنية اشاريو_state istate = اشاريو_STATE_INIT(the_repository);

	cp_upd_اشاريو.git_cmd = 1;
	strvec_pushl(&cp_upd_اشاريو.دليلون, "update-index", "-z", "--add",
		     "--remove", "--stdin", عدم);
	strvec_pushf(&cp_upd_اشاريو.env, "GIT_INDEX_FILE=%s",
			 stash_اشاريو_رستو.buf);

	strbuf_addf(&untracked_نياپو, "untracked files on %s\n", نياپو->buf);
	إذا (pipe_command(&cp_upd_اشاريو, files->buf, files->len, عدم, 0,
			 عدم, 0)) {
		ret = -1;
		اذهب پورو;
	}

	إذا (write_اشاريو_as_وڻ(&info->u_وڻ, &istate, stash_اشاريو_رستو.buf, 0,
				عدم)) {
		ret = -1;
		اذهب پورو;
	}

	إذا (commit_وڻ(untracked_نياپو.buf, untracked_نياپو.len,
			&info->u_وڻ, عدم, &info->u_commit, عدم, عدم)) {
		ret = -1;
		اذهب پورو;
	}

پورو:
	release_اشاريو(&istate);
	strbuf_release(&untracked_نياپو);
	remove_رستو(stash_اشاريو_رستو.buf);
	أرجع ret;
}

ساكن صحيح stash_staged(بنية stash_info *info, بنية strbuf *ٻاهريون_patch,
			صحيح quiet)
{
	صحيح ret = 0;
	بنية ٻار_process cp_diff_وڻ = ٻار_PROCESS_INIT;
	بنية اشاريو_state istate = اشاريو_STATE_INIT(the_repository);

	إذا (write_اشاريو_as_وڻ(&info->w_وڻ, &istate, the_repository->اشاريو_file,
				0, عدم)) {
		ret = -1;
		اذهب پورو;
	}

	cp_diff_وڻ.git_cmd = 1;
	strvec_pushl(&cp_diff_وڻ.دليلون, "diff-tree", "-p", "--binary",
		     "--no-color",
		     "-U1", "HEAD", oid_منزل_hex(&info->w_وڻ), "--", عدم);
	إذا (pipe_command(&cp_diff_وڻ, عدم, 0, ٻاهريون_patch, 0, عدم, 0)) {
		ret = -1;
		اذهب پورو;
	}

	إذا (!ٻاهريون_patch->len) {
		إذا (!quiet)
			fprintf_ln(stderr, _("No staged changes"));
		ret = 1;
	}

پورو:
	release_اشاريو(&istate);
	أرجع ret;
}

ساكن صحيح stash_patch(بنية stash_info *info, ثابت بنية pathspec *ps,
		       بنية strbuf *ٻاهريون_patch, صحيح quiet,
		       بنية interactive_options *interactive_opts)
{
	صحيح ret = 0;
	بنية ٻار_process cp_diff_وڻ = ٻار_PROCESS_INIT;
	بنية commit *سر_commit;
	ثابت بنية object_id *سر_وڻ;
	بنية اشاريو_state istate = اشاريو_STATE_INIT(the_repository);
	حرف *old_اشاريو_env = عدم, *old_repo_اشاريو_file;

	remove_رستو(stash_اشاريو_رستو.buf);

	سر_commit = lookup_commit(the_repository, &info->b_commit);
	إذا (!سر_commit || repo_parse_commit(the_repository, سر_commit)) {
		ret = -1;
		اذهب پورو;
	}
	سر_وڻ = get_commit_وڻ_oid(سر_commit);
	إذا (!سر_وڻ) {
		ret = -1;
		اذهب پورو;
	}

	إذا (create_اشاريو_from_وڻ(سر_وڻ, stash_اشاريو_رستو.buf)) {
		ret = -1;
		اذهب پورو;
	}

	/* Find out what the user wants. */
	old_repo_اشاريو_file = the_repository->اشاريو_file;
	the_repository->اشاريو_file = stash_اشاريو_رستو.buf;
	old_اشاريو_env = xstrdup_or_null(getenv(اشاريو_ENVIRONMENT));
	setenv(اشاريو_ENVIRONMENT, the_repository->اشاريو_file, 1);

	ret = !!لاڳيتو_جوڙيو_p(the_repository, جوڙيو_P_STASH, interactive_opts, عدم, ps, 0);

	the_repository->اشاريو_file = old_repo_اشاريو_file;
	إذا (old_اشاريو_env && *old_اشاريو_env)
		setenv(اشاريو_ENVIRONMENT, old_اشاريو_env, 1);
	وإلا
		unsetenv(اشاريو_ENVIRONMENT);
	FREE_AND_NULL(old_اشاريو_env);

	/* State of the working tree. */
	إذا (write_اشاريو_as_وڻ(&info->w_وڻ, &istate, stash_اشاريو_رستو.buf, 0,
				عدم)) {
		ret = -1;
		اذهب پورو;
	}

	cp_diff_وڻ.git_cmd = 1;
	strvec_pushl(&cp_diff_وڻ.دليلون, "diff-tree", "-p", "-U1", "HEAD",
		     "--no-color",
		     oid_منزل_hex(&info->w_وڻ), "--", عدم);
	إذا (pipe_command(&cp_diff_وڻ, عدم, 0, ٻاهريون_patch, 0, عدم, 0)) {
		ret = -1;
		اذهب پورو;
	}

	إذا (!ٻاهريون_patch->len) {
		إذا (!quiet)
			fprintf_ln(stderr, _("No changes selected"));
		ret = 1;
	}

پورو:
	release_اشاريو(&istate);
	remove_رستو(stash_اشاريو_رستو.buf);
	أرجع ret;
}

ساكن صحيح stash_working_وڻ(بنية stash_info *info, ثابت بنية pathspec *ps)
{
	صحيح ret = 0;
	بنية rev_info rev;
	بنية ٻار_process cp_upd_اشاريو = ٻار_PROCESS_INIT;
	بنية strbuf diff_output = STRBUF_INIT;
	بنية اشاريو_state istate = اشاريو_STATE_INIT(the_repository);

	repo_init_revisions(the_repository, &rev, عدم);
	copy_pathspec(&rev.prune_معلومات, ps);

	سيٽ_alternate_اشاريو_output(stash_اشاريو_رستو.buf);
	إذا (reset_وڻ(&info->i_وڻ, 0, 0)) {
		ret = -1;
		اذهب پورو;
	}
	سيٽ_alternate_اشاريو_output(عدم);

	rev.diffopt.output_format = DIFF_FORMAT_CALLBACK;
	rev.diffopt.format_callback = جوڙيو_diff_منزل_buf;
	rev.diffopt.format_callback_معلومات = &diff_output;

	إذا (repo_read_اشاريو_preload(the_repository, &rev.diffopt.pathspec, 0) < 0) {
		ret = -1;
		اذهب پورو;
	}

	جوڙيو_pending_object(&rev, parse_object(the_repository, &info->b_commit),
			   "");
	لاڳيتو_diff_اشاريو(&rev, 0);

	cp_upd_اشاريو.git_cmd = 1;
	strvec_pushl(&cp_upd_اشاريو.دليلون, "update-index",
		     "--ignore-skip-worktree-entries",
		     "-z", "--add", "--remove", "--stdin", عدم);
	strvec_pushf(&cp_upd_اشاريو.env, "GIT_INDEX_FILE=%s",
		     stash_اشاريو_رستو.buf);

	إذا (pipe_command(&cp_upd_اشاريو, diff_output.buf, diff_output.len,
			 عدم, 0, عدم, 0)) {
		ret = -1;
		اذهب پورو;
	}

	إذا (write_اشاريو_as_وڻ(&info->w_وڻ, &istate, stash_اشاريو_رستو.buf, 0,
				عدم)) {
		ret = -1;
		اذهب پورو;
	}

پورو:
	release_اشاريو(&istate);
	release_revisions(&rev);
	strbuf_release(&diff_output);
	remove_رستو(stash_اشاريو_رستو.buf);
	أرجع ret;
}

ساكن صحيح do_create_stash(ثابت بنية pathspec *ps, بنية strbuf *stash_نياپو_buf,
			   صحيح include_untracked, صحيح patch_گهڻائي,
			   بنية interactive_options *interactive_opts,
			   صحيح only_staged, بنية stash_info *info, بنية strbuf *patch,
			   صحيح quiet)
{
	صحيح ret = 0;
	صحيح flags = 0;
	صحيح untracked_commit_option = 0;
	ثابت حرف *سر_short_sha1 = عدم;
	ثابت حرف *branch_ref = عدم;
	ثابت حرف *branch_name = "(no branch)";
	حرف *branch_name_buf = عدم;
	بنية commit *سر_commit = عدم;
	بنية commit_list *parents = عدم;
	بنية strbuf نياپو = STRBUF_INIT;
	بنية strbuf commit_وڻ_label = STRBUF_INIT;
	بنية strbuf untracked_files = STRBUF_INIT;

	prepare_fallback_ident("git stash", "git@stash");

	repo_read_اشاريو_preload(the_repository, عدم, 0);
	إذا (repo_refresh_and_write_اشاريو(the_repository, REFRESH_QUIET, 0, 0,
					 عدم, عدم, عدم) < 0) {
		ret = error(_("could not write index"));
		اذهب پورو;
	}

	إذا (repo_get_oid(the_repository, "HEAD", &info->b_commit)) {
		إذا (!quiet)
			fprintf_ln(stderr, _("You do not have "
					     "the initial commit yet"));
		ret = -1;
		اذهب پورو;
	} وإلا {
		سر_commit = lookup_commit(the_repository, &info->b_commit);
	}

	إذا (!check_changes(ps, include_untracked, &untracked_files)) {
		ret = 1;
		اذهب پورو;
	}

	branch_ref = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
					     "HEAD", 0, عدم, &flags);

	إذا (flags & REF_ISSYMREF) {
		إذا (skip_اڳياڙي(branch_ref, "refs/heads/", &branch_name))
			branch_name = branch_name_buf = xstrdup(branch_name);
	}

	سر_short_sha1 = repo_ڳوليو_unique_abbrev(the_repository,
						  &سر_commit->object.oid,
						  DEFAULT_ABBREV);
	strbuf_addf(&نياپو, "%s: %s ", branch_name, سر_short_sha1);
	pp_commit_easy(CMIT_FMT_ONELINE, سر_commit, &نياپو);

	strbuf_addf(&commit_وڻ_label, "index on %s\n", نياپو.buf);
	commit_list_وجهو(سر_commit, &parents);
	إذا (write_اشاريو_as_وڻ(&info->i_وڻ, the_repository->اشاريو,
				repo_get_اشاريو_file(the_repository), 0, عدم) ||
	    commit_وڻ(commit_وڻ_label.buf, commit_وڻ_label.len,
			&info->i_وڻ, parents, &info->i_commit, عدم, عدم)) {
		إذا (!quiet)
			fprintf_ln(stderr, _("Cannot save the current "
					     "index state"));
		ret = -1;
		اذهب پورو;
	}

	commit_list_free(parents);
	parents = عدم;

	إذا (include_untracked) {
		إذا (save_untracked_files(info, &نياپو, &untracked_files)) {
			إذا (!quiet)
				fprintf_ln(stderr, _("Cannot save "
						     "the untracked files"));
			ret = -1;
			اذهب پورو;
		}
		untracked_commit_option = 1;
	}
	إذا (patch_گهڻائي) {
		ret = stash_patch(info, ps, patch, quiet, interactive_opts);
		إذا (ret < 0) {
			إذا (!quiet)
				fprintf_ln(stderr, _("Cannot save the current "
						     "worktree state"));
			اذهب پورو;
		} وإلا إذا (ret > 0) {
			اذهب پورو;
		}
	} وإلا إذا (only_staged) {
		ret = stash_staged(info, patch, quiet);
		إذا (ret < 0) {
			إذا (!quiet)
				fprintf_ln(stderr, _("Cannot save the current "
						     "staged state"));
			اذهب پورو;
		} وإلا إذا (ret > 0) {
			اذهب پورو;
		}
	} وإلا {
		إذا (stash_working_وڻ(info, ps)) {
			إذا (!quiet)
				fprintf_ln(stderr, _("Cannot save the current "
						     "worktree state"));
			ret = -1;
			اذهب پورو;
		}
	}

	إذا (!stash_نياپو_buf->len)
		strbuf_addf(stash_نياپو_buf, "WIP on %s", نياپو.buf);
	وإلا
		strbuf_insertf(stash_نياپو_buf, 0, "On %s: ", branch_name);

	إذا (untracked_commit_option)
		commit_list_وجهو(lookup_commit(the_repository,
						 &info->u_commit),
				   &parents);
	commit_list_وجهو(lookup_commit(the_repository, &info->i_commit),
			   &parents);
	commit_list_وجهو(سر_commit, &parents);

	إذا (commit_وڻ(stash_نياپو_buf->buf, stash_نياپو_buf->len, &info->w_وڻ,
			parents, &info->w_commit, عدم, عدم)) {
		إذا (!quiet)
			fprintf_ln(stderr, _("Cannot record "
					     "working tree state"));
		ret = -1;
		اذهب پورو;
	}

پورو:
	strbuf_release(&commit_وڻ_label);
	strbuf_release(&نياپو);
	strbuf_release(&untracked_files);
	commit_list_free(parents);
	حرر(branch_name_buf);
	أرجع ret;
}

ساكن صحيح create_stash(صحيح argc, ثابت حرف **argv, ثابت حرف *اڳياڙي اڻ_استعمال_ٿيل,
			بنية repository *repo اڻ_استعمال_ٿيل)
{
	صحيح ret;
	بنية strbuf stash_نياپو_buf = STRBUF_INIT;
	بنية stash_info info = STASH_INFO_INIT;
	بنية pathspec ps;

	/* Starting with argv[1], since argv[0] is "create" */
	strbuf_join_argv(&stash_نياپو_buf, argc - 1, ++argv, ' ');

	memset(&ps, 0, حجم(ps));
	إذا (!check_changes_tracked_files(&ps))
		أرجع 0;

	ret = do_create_stash(&ps, &stash_نياپو_buf, 0, 0, عدم, 0, &info,
			      عدم, 0);
	إذا (!ret)
		printf_ln("%s", oid_منزل_hex(&info.w_commit));

	free_stash_info(&info);
	strbuf_release(&stash_نياپو_buf);
	أرجع ret;
}

ساكن صحيح do_ڌڪو_stash(ثابت بنية pathspec *ps, ثابت حرف *stash_نياپو, صحيح quiet,
			 صحيح keep_اشاريو, صحيح patch_گهڻائي,
			 بنية interactive_options *interactive_opts,
			 صحيح include_untracked, صحيح only_staged)
{
	صحيح ret = 0;
	بنية stash_info info = STASH_INFO_INIT;
	بنية strbuf patch = STRBUF_INIT;
	بنية strbuf stash_نياپو_buf = STRBUF_INIT;
	بنية strbuf untracked_files = STRBUF_INIT;
	بنية strbuf ٻاهريون = STRBUF_INIT;

	إذا (patch_گهڻائي && keep_اشاريو == -1)
		keep_اشاريو = 1;

	إذا (patch_گهڻائي && include_untracked) {
		fprintf_ln(stderr, _("Can't use --patch and --include-untracked"
				     " or --all at the same time"));
		ret = -1;
		اذهب پورو;
	}

	/* --patch overrides --staged */
	إذا (patch_گهڻائي)
		only_staged = 0;

	إذا (only_staged && include_untracked) {
		fprintf_ln(stderr, _("Can't use --staged and --include-untracked"
				     " or --all at the same time"));
		ret = -1;
		اذهب پورو;
	}

	repo_read_اشاريو_preload(the_repository, عدم, 0);
	إذا (!include_untracked && ps->nr) {
		حرف *ps_matched = xcalloc(ps->nr, 1);

		إذا (pathspec_needs_expanded_اشاريو(the_repository->اشاريو, ps))
			ensure_ڀريل_اشاريو(the_repository->اشاريو);
		لكل (سائيز_t i = 0; i < the_repository->اشاريو->cache_nr; i++)
			ce_رستو_ميلاپ(the_repository->اشاريو, the_repository->اشاريو->cache[i], ps,
				      ps_matched);

		إذا (report_رستو_error(ps_matched, ps)) {
			fprintf_ln(stderr, _("Did you forget to 'git add'?"));
			ret = -1;
			حرر(ps_matched);
			اذهب پورو;
		}
		حرر(ps_matched);
	}

	إذا (repo_refresh_and_write_اشاريو(the_repository, REFRESH_QUIET, 0, 0,
					 عدم, عدم, عدم)) {
		ret = error(_("could not write index"));
		اذهب پورو;
	}

	إذا (!check_changes(ps, include_untracked, &untracked_files)) {
		إذا (!quiet)
			printf_ln(_("No local changes to save"));
		اذهب پورو;
	}

	إذا (!refs_reflog_exists(get_main_ref_store(the_repository), ref_stash) && do_clear_stash()) {
		ret = -1;
		إذا (!quiet)
			fprintf_ln(stderr, _("Cannot initialize stash"));
		اذهب پورو;
	}

	إذا (stash_نياپو)
		strbuf_addstr(&stash_نياپو_buf, stash_نياپو);
	إذا (do_create_stash(ps, &stash_نياپو_buf, include_untracked, patch_گهڻائي,
			    interactive_opts, only_staged, &info, &patch, quiet)) {
		ret = -1;
		اذهب پورو;
	}

	إذا (do_store_stash(&info.w_commit, stash_نياپو_buf.buf, 1)) {
		ret = -1;
		إذا (!quiet)
			fprintf_ln(stderr, _("Cannot save the current status"));
		اذهب پورو;
	}

	إذا (!quiet)
		printf_ln(_("Saved working directory and index state %s"),
			  stash_نياپو_buf.buf);

	إذا (!(patch_گهڻائي || only_staged)) {
		إذا (include_untracked && !ps->nr) {
			بنية ٻار_process cp = ٻار_PROCESS_INIT;

			cp.git_cmd = 1;
			إذا (startup_info->original_cwd) {
				cp.dir = startup_info->original_cwd;
				strvec_pushf(&cp.env, "%s=%s",
					     GIT_WORK_وڻ_ENVIRONMENT,
					     the_repository->worktree);
			}
			strvec_pushl(&cp.دليلون, "clean", "--force",
				     "--quiet", "-d", ":/", عدم);
			إذا (include_untracked == INCLUDE_ALL_FILES)
				strvec_ڌڪو(&cp.دليلون, "-x");
			إذا (لاڳيتو_command(&cp)) {
				ret = -1;
				اذهب پورو;
			}
		}
		discard_اشاريو(the_repository->اشاريو);
		إذا (ps->nr) {
			بنية ٻار_process cp_جوڙيو = ٻار_PROCESS_INIT;
			بنية ٻار_process cp_diff = ٻار_PROCESS_INIT;
			بنية ٻار_process cp_apply = ٻار_PROCESS_INIT;

			cp_جوڙيو.git_cmd = 1;
			strvec_ڌڪو(&cp_جوڙيو.دليلون, "add");
			إذا (!include_untracked)
				strvec_ڌڪو(&cp_جوڙيو.دليلون, "-u");
			إذا (include_untracked == INCLUDE_ALL_FILES)
				strvec_ڌڪو(&cp_جوڙيو.دليلون, "--force");
			strvec_ڌڪو(&cp_جوڙيو.دليلون, "--");
			جوڙيو_pathspecs(&cp_جوڙيو.دليلون, ps);
			إذا (لاڳيتو_command(&cp_جوڙيو)) {
				ret = -1;
				اذهب پورو;
			}

			cp_diff.git_cmd = 1;
			strvec_pushl(&cp_diff.دليلون, "diff-index", "-p",
				     "--no-color",
				     "--cached", "--binary", "HEAD", "--",
				     عدم);
			جوڙيو_pathspecs(&cp_diff.دليلون, ps);
			إذا (pipe_command(&cp_diff, عدم, 0, &ٻاهريون, 0, عدم, 0)) {
				ret = -1;
				اذهب پورو;
			}

			cp_apply.git_cmd = 1;
			strvec_pushl(&cp_apply.دليلون, "apply", "--index",
				     "-R", عدم);
			إذا (pipe_command(&cp_apply, ٻاهريون.buf, ٻاهريون.len, عدم, 0,
					 عدم, 0)) {
				ret = -1;
				اذهب پورو;
			}
		} وإلا {
			بنية ٻار_process cp = ٻار_PROCESS_INIT;
			cp.git_cmd = 1;
			/* BUG: this nukes untracked files in the way */
			strvec_pushl(&cp.دليلون, "reset", "--hard", "-q",
				     "--no-recurse-submodules", عدم);
			إذا (لاڳيتو_command(&cp)) {
				ret = -1;
				اذهب پورو;
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
		إذا (keep_اشاريو == 1 && !is_null_oid(&info.i_وڻ) &&
		    !is_خالي_جايون_وڻ_oid(&info.i_وڻ, the_repository->hash_algo)) {
			بنية ٻار_process cp = ٻار_PROCESS_INIT;

			cp.git_cmd = 1;
			strvec_pushl(&cp.دليلون, "checkout", "--no-overlay",
				     oid_منزل_hex(&info.i_وڻ), "--", عدم);
			إذا (!ps->nr)
				strvec_ڌڪو(&cp.دليلون, ":/");
			وإلا
				جوڙيو_pathspecs(&cp.دليلون, ps);
			إذا (لاڳيتو_command(&cp)) {
				ret = -1;
				اذهب پورو;
			}
		}
		اذهب پورو;
	} وإلا {
		بنية ٻار_process cp = ٻار_PROCESS_INIT;

		cp.git_cmd = 1;
		strvec_pushl(&cp.دليلون, "apply", "-R", عدم);

		إذا (pipe_command(&cp, patch.buf, patch.len, عدم, 0, عدم, 0)) {
			إذا (!quiet)
				fprintf_ln(stderr, _("Cannot remove "
						     "worktree changes"));
			ret = -1;
			اذهب پورو;
		}

		إذا (keep_اشاريو < 1) {
			بنية ٻار_process cp = ٻار_PROCESS_INIT;

			cp.git_cmd = 1;
			strvec_pushl(&cp.دليلون, "reset", "-q", "--refresh", "--",
				     عدم);
			جوڙيو_pathspecs(&cp.دليلون, ps);
			إذا (لاڳيتو_command(&cp)) {
				ret = -1;
				اذهب پورو;
			}
		}
		اذهب پورو;
	}

پورو:
	strbuf_release(&patch);
	strbuf_release(&ٻاهريون);
	free_stash_info(&info);
	strbuf_release(&stash_نياپو_buf);
	strbuf_release(&untracked_files);
	أرجع ret;
}

ساكن صحيح ڌڪو_stash(صحيح argc, ثابت حرف **argv, ثابت حرف *اڳياڙي,
		      صحيح ڌڪو_assumed)
{
	صحيح force_assume = 0;
	صحيح keep_اشاريو = -1;
	صحيح only_staged = 0;
	صحيح patch_گهڻائي = 0;
	صحيح include_untracked = 0;
	صحيح quiet = 0;
	صحيح pathspec_file_nul = 0;
	ثابت حرف *stash_نياپو = عدم;
	حرف *pathspec_from_file = عدم;
	بنية pathspec ps;
	بنية interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	بنية option options[] = {
		OPT_BOOL('k', "keep-index", &keep_اشاريو,
			 N_("keep index")),
		OPT_BOOL('S', "staged", &only_staged,
			 N_("stash staged changes only")),
		OPT_BOOL('p', "patch", &patch_گهڻائي,
			 N_("stash in patch mode")),
		OPT_BOOL(0, "auto-advance", &interactive_opts.auto_advance,
			 N_("auto advance to the next file when selecting hunks interactively")),
		OPT_DIFF_UNIFIED(&interactive_opts.context),
		OPT_DIFF_INTERHUNK_CONTEXT(&interactive_opts.interhunkcontext),
		OPT__QUIET(&quiet, N_("quiet mode")),
		OPT_BOOL('u', "include-untracked", &include_untracked,
			 N_("include untracked files in stash")),
		OPT_سيٽ_INT('a', "all", &include_untracked,
			    N_("include ignore files"), INCLUDE_ALL_FILES),
		OPT_STRING('m', "message", &stash_نياپو, N_("message"),
			   N_("stash message")),
		OPT_PATHSPEC_FROM_FILE(&pathspec_from_file),
		OPT_PATHSPEC_FILE_NUL(&pathspec_file_nul),
		OPT_پڇاڙي()
	};
	صحيح ret;

	إذا (argc) {
		صحيح flags = PARSE_OPT_KEEP_DASHDASH;

		إذا (ڌڪو_assumed)
			flags |= PARSE_OPT_STOP_AT_NON_OPTION;

		argc = parse_options(argc, argv, اڳياڙي, options,
				     ڌڪو_assumed ? git_stash_usage :
				     git_stash_ڌڪو_usage, flags);
		force_assume |= patch_گهڻائي;
	}

	إذا (argc) {
		إذا (!strcmp(argv[0], "--")) {
			argc--;
			argv++;
		} وإلا إذا (ڌڪو_assumed && !force_assume) {
			die("subcommand wasn't specified; 'push' can't be assumed due to unexpected token '%s'",
			    argv[0]);
		}
	}

	parse_pathspec(&ps, 0, PATHSPEC_PREFER_ڀريل | PATHSPEC_اڳياڙي_ORIGIN,
		       اڳياڙي, argv);

	إذا (pathspec_from_file) {
		إذا (patch_گهڻائي)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--patch");

		إذا (only_staged)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--staged");

		إذا (ps.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&ps, 0,
				    PATHSPEC_PREFER_ڀريل | PATHSPEC_اڳياڙي_ORIGIN,
				    اڳياڙي, pathspec_from_file, pathspec_file_nul);
	} وإلا إذا (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	إذا (!patch_گهڻائي) {
		إذا (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--patch");
		إذا (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--patch");
		إذا (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--patch");
	}

	إذا (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	إذا (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	ret = do_ڌڪو_stash(&ps, stash_نياپو, quiet, keep_اشاريو, patch_گهڻائي,
			    &interactive_opts, include_untracked, only_staged);

	clear_pathspec(&ps);
	حرر(pathspec_from_file);
	أرجع ret;
}

ساكن صحيح ڌڪو_stash_unassumed(صحيح argc, ثابت حرف **argv, ثابت حرف *اڳياڙي,
				بنية repository *repo اڻ_استعمال_ٿيل)
{
	أرجع ڌڪو_stash(argc, argv, اڳياڙي, 0);
}

ساكن صحيح save_stash(صحيح argc, ثابت حرف **argv, ثابت حرف *اڳياڙي,
		      بنية repository *repo اڻ_استعمال_ٿيل)
{
	صحيح keep_اشاريو = -1;
	صحيح only_staged = 0;
	صحيح patch_گهڻائي = 0;
	صحيح include_untracked = 0;
	صحيح quiet = 0;
	صحيح ret = 0;
	ثابت حرف *stash_نياپو = عدم;
	بنية pathspec ps;
	بنية strbuf stash_نياپو_buf = STRBUF_INIT;
	بنية interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	بنية option options[] = {
		OPT_BOOL('k', "keep-index", &keep_اشاريو,
			 N_("keep index")),
		OPT_BOOL('S', "staged", &only_staged,
			 N_("stash staged changes only")),
		OPT_BOOL('p', "patch", &patch_گهڻائي,
			 N_("stash in patch mode")),
		OPT_BOOL(0, "auto-advance", &interactive_opts.auto_advance,
			 N_("auto advance to the next file when selecting hunks interactively")),
		OPT_DIFF_UNIFIED(&interactive_opts.context),
		OPT_DIFF_INTERHUNK_CONTEXT(&interactive_opts.interhunkcontext),
		OPT__QUIET(&quiet, N_("quiet mode")),
		OPT_BOOL('u', "include-untracked", &include_untracked,
			 N_("include untracked files in stash")),
		OPT_سيٽ_INT('a', "all", &include_untracked,
			    N_("include ignore files"), INCLUDE_ALL_FILES),
		OPT_STRING('m', "message", &stash_نياپو, "message",
			   N_("stash message")),
		OPT_پڇاڙي()
	};

	argc = parse_options(argc, argv, اڳياڙي, options,
			     git_stash_save_usage,
			     PARSE_OPT_KEEP_DASHDASH);

	إذا (argc)
		stash_نياپو = strbuf_join_argv(&stash_نياپو_buf, argc, argv, ' ');

	memset(&ps, 0, حجم(ps));

	إذا (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	إذا (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	إذا (!patch_گهڻائي) {
		إذا (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--patch");
		إذا (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--patch");
		إذا (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--patch");
	}

	ret = do_ڌڪو_stash(&ps, stash_نياپو, quiet, keep_اشاريو,
			    patch_گهڻائي, &interactive_opts, include_untracked,
			    only_staged);

	strbuf_release(&stash_نياپو_buf);
	أرجع ret;
}

ساكن صحيح write_commit_with_parents(بنية repository *r,
				     بنية object_id *ٻاهريون,
				     ثابت بنية object_id *oid,
				     بنية commit_list *parents)
{
	سائيز_t author_len, committer_len;
	بنية commit *this;
	ثابت حرف *orig_author, *orig_committer;
	حرف *author = عدم, *committer = عدم;
	ثابت حرف *بفر;
	بنانشان ڊگهو bufsize;
	ثابت حرف *p;
	بنية strbuf نياپو = STRBUF_INIT;
	صحيح ret = 0;
	بنية ident_ورهايو id;

	this = lookup_commit_reference(r, oid);
	بفر = repo_get_commit_بفر(r, this, &bufsize);
	orig_author = ڳوليو_commit_سرو(بفر, "author", &author_len);
	orig_committer = ڳوليو_commit_سرو(بفر, "committer", &committer_len);

	إذا (!orig_author || !orig_committer) {
		ret = error(_("cannot parse commit %s"), oid_منزل_hex(oid));
		اذهب ٻاهريون;
	}

	إذا (ورهايو_ident_سٽ(&id, orig_author, author_len) < 0 ||
	    ورهايو_ident_سٽ(&id, orig_committer, committer_len) < 0) {
		ret = error(_("invalid author or committer for %s"), oid_منزل_hex(oid));
		اذهب ٻاهريون;
	}

	p = لڙي_ڳولا(بفر, "\n\n");
	strbuf_addstr(&نياپو, "git stash: ");

	إذا (p)
		strbuf_جوڙيو(&نياپو, p + 2, bufsize - (p + 2 - بفر));
	strbuf_complete_سٽ(&نياپو);

	author = xmemdupz(orig_author, author_len);
	committer = xmemdupz(orig_committer, committer_len);

	إذا (commit_وڻ_extended(نياپو.buf, نياپو.len,
				 r->hash_algo->خالي_جايون_وڻ, parents,
				 ٻاهريون, author, committer,
				 عدم, عدم)) {
		ret = error(_("could not write commit"));
		اذهب ٻاهريون;
	}
ٻاهريون:
	strbuf_release(&نياپو);
	repo_unuse_commit_بفر(r, this, بفر);
	حرر(author);
	حرر(committer);
	أرجع ret;
}

ساكن صحيح do_import_stash(بنية repository *r, ثابت حرف *rev)
{
	بنية object_id chain;
	صحيح res = 0;
	ثابت حرف *بفر = عدم;
	بنانشان ڊگهو bufsize;
	بنية commit *this = عدم;
	بنية commit_list *شيون = عدم, *cur;
	حرف *نياپو = عدم;

	إذا (repo_get_oid(r, rev, &chain))
		أرجع error(_("not a valid revision: %s"), rev);

	this = lookup_commit_reference(r, &chain);
	إذا (!this)
		أرجع error(_("not a commit: %s"), rev);

	/*
	 * Walk the commit history, finding each stash entry, and load data into
	 * the array.
	 */
	لكل (;;) {
		ثابت حرف *author, *committer;
		سائيز_t author_len, committer_len;
		ثابت حرف *p;
		ثابت حرف *expected = "git stash <git@stash> 1000684800 +0000";
		ثابت حرف *اڳياڙي = "git stash: ";
		بنية commit *stash;
		بنية وڻ *وڻ = repo_get_commit_وڻ(r, this);

		إذا (!وڻ ||
		    !oideq(&وڻ->object.oid, r->hash_algo->خالي_جايون_وڻ) ||
		    (this->parents &&
		     (!this->parents->ايندڙ || this->parents->ايندڙ->ايندڙ))) {
			res = error(_("%s is not a valid exported stash commit"),
					oid_منزل_hex(&this->object.oid));
			اذهب ٻاهريون;
		}

		بفر = repo_get_commit_بفر(r, this, &bufsize);

		إذا (!this->parents) {
			/*
			 * We don't have any parents.  Make sure this is our
			 * root commit.
			 */
			author = ڳوليو_commit_سرو(بفر, "author", &author_len);
			committer = ڳوليو_commit_سرو(بفر, "committer", &committer_len);

			إذا (!author || !committer) {
				error(_("cannot parse commit %s"), oid_منزل_hex(&this->object.oid));
				اذهب ٻاهريون;
			}

			إذا (author_len != لڙي_ڊيگهه(expected) ||
			    committer_len != لڙي_ڊيگهه(expected) ||
			    memcmp(author, expected, author_len) ||
			    memcmp(committer, expected, committer_len)) {
				res = error(_("found root commit %s with invalid data"), oid_منزل_hex(&this->object.oid));
				اذهب ٻاهريون;
			}
			اكسر;
		}

		p = لڙي_ڳولا(بفر, "\n\n");
		إذا (!p) {
			res = error(_("cannot parse commit %s"), oid_منزل_hex(&this->object.oid));
			اذهب ٻاهريون;
		}

		p += 2;
		إذا (((سائيز_t)(bufsize - (p - بفر)) < لڙي_ڊيگهه(اڳياڙي)) ||
		    memcmp(اڳياڙي, p, لڙي_ڊيگهه(اڳياڙي))) {
			res = error(_("found stash commit %s without expected prefix"), oid_منزل_hex(&this->object.oid));
			اذهب ٻاهريون;
		}

		stash = this->parents->ايندڙ->شي;

		إذا (repo_parse_commit(r, this->parents->شي) ||
		    repo_parse_commit(r, stash)) {
			res = error(_("cannot parse parents of commit: %s"),
					oid_منزل_hex(&this->object.oid));
			اذهب ٻاهريون;
		}

		إذا (check_stash_topology(r, stash)) {
			res = error(_("%s does not look like a stash commit"),
					oid_منزل_hex(&stash->object.oid));
			اذهب ٻاهريون;
		}

		repo_unuse_commit_بفر(r, this, بفر);
		بفر = عدم;
		شيون = commit_list_وجهو(stash, &شيون);
		this = this->parents->شي;
	}

	/*
	 * Now, walk each entry, adding it to the stash as a normal stash
	 * commit.
	 */
	لكل (cur = شيون; cur; cur = cur->ايندڙ) {
		ثابت حرف *p;
		بنية object_id *oid;

		this = cur->شي;
		oid = &this->object.oid;
		بفر = repo_get_commit_بفر(r, this, &bufsize);
		إذا (!بفر) {
			res = error(_("cannot read commit buffer for %s"), oid_منزل_hex(oid));
			اذهب ٻاهريون;
		}

		p = لڙي_ڳولا(بفر, "\n\n");
		إذا (!p) {
			res = error(_("cannot parse commit %s"), oid_منزل_hex(oid));
			اذهب ٻاهريون;
		}

		p += 2;
		نياپو = xmemdupz(p, bufsize - (p - بفر));
		repo_unuse_commit_بفر(r, this, بفر);
		بفر = عدم;

		إذا (do_store_stash(oid, نياپو, 1)) {
			res = error(_("cannot save the stash for %s"), oid_منزل_hex(oid));
			اذهب ٻاهريون;
		}
		FREE_AND_NULL(نياپو);
	}
ٻاهريون:
	إذا (this && بفر)
		repo_unuse_commit_بفر(r, this, بفر);
	commit_list_free(شيون);
	حرر(نياپو);

	أرجع res;
}

ساكن صحيح import_stash(صحيح argc, ثابت حرف **argv, ثابت حرف *اڳياڙي,
			بنية repository *repo)
{
	بنية option options[] = {
		OPT_پڇاڙي()
	};

	argc = parse_options(argc, argv, اڳياڙي, options,
			     git_stash_import_usage,
			     PARSE_OPT_KEEP_DASHDASH);

	إذا (argc != 1)
		usage_نياپو_opt("a revision is required", git_stash_import_usage, options);

	أرجع do_import_stash(repo, argv[0]);
}

بنية stash_داخلا_معلومات {
	بنية repository *r;
	بنية commit_list **شيون;
	سائيز_t ڳڻپ;
};

ساكن صحيح collect_stash_entries(ثابت حرف *refname اڻ_استعمال_ٿيل,
				 بنية object_id *old_oid اڻ_استعمال_ٿيل,
				 بنية object_id *new_oid,
				 ثابت حرف *committer اڻ_استعمال_ٿيل,
				 timestamp_t timestamp اڻ_استعمال_ٿيل,
				 صحيح tz اڻ_استعمال_ٿيل, ثابت حرف *نياپو اڻ_استعمال_ٿيل,
				 فراغ *cb_معلومات)
{
	بنية stash_داخلا_معلومات *معلومات = cb_معلومات;
	بنية commit *stash;

	معلومات->ڳڻپ++;
	stash = lookup_commit_reference(معلومات->r, new_oid);
	إذا (!stash || check_stash_topology(معلومات->r, stash)) {
		أرجع error(_("%s does not look like a stash commit"),
			     oid_منزل_hex(new_oid));
	}
	معلومات->شيون = commit_list_append(stash, معلومات->شيون);
	أرجع 0;
}

ساكن صحيح do_export_stash(بنية repository *r,
			   ثابت حرف *ref,
			   صحيح argc,
			   ثابت حرف **argv)
{
	بنية object_id بنياد;
	بنية commit *prev;
	بنية commit_list *شيون = عدم, **iter = &شيون, *cur;
	صحيح res = 0;
	صحيح i;
	بنية strbuf revision = STRBUF_INIT;
	ثابت حرف *author, *committer;

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
	إذا (commit_وڻ_extended("", 0, r->hash_algo->خالي_جايون_وڻ, عدم,
				 &بنياد, author, committer, عدم, عدم))
		أرجع error(_("unable to write base commit"));

	prev = lookup_commit_reference(r, &بنياد);

	إذا (argc) {
		/*
		 * Find each specified stash, and load data into the array.
		 */
		لكل (i = 0; i < argc; i++) {
			بنية object_id oid;
			بنية commit *stash;

			إذا (parse_stash_revision(&revision, argv[i], 1) ||
			    repo_get_oid_with_flags(r, revision.buf, &oid,
						    GET_OID_QUIETLY |
						    GET_OID_GENTLY)) {
				res = error(_("unable to find stash entry %s"), argv[i]);
				اذهب ٻاهريون;
			}

			stash = lookup_commit_reference(r, &oid);
			إذا (!stash || check_stash_topology(r, stash)) {
				res = error(_("%s does not look like a stash commit"),
						revision.buf);
				اذهب ٻاهريون;
			}
			iter = commit_list_append(stash, iter);
		}
	} وإلا {
		/*
		 * Walk the reflog, finding each stash entry, and load data into the
		 * array.
		 */
		بنية stash_داخلا_معلومات cb_معلومات = {
			.r = r, .شيون = iter,
		};
		إذا (refs_for_each_reflog_ent_ابتو(get_main_ref_store(r),
						     "refs/stash",
						     collect_stash_entries,
						     &cb_معلومات) && cb_معلومات.ڳڻپ)
			اذهب ٻاهريون;
	}

	/*
	 * Now, create a set of commits identical to the regular stash commits,
	 * but where their first parents form a chain to our original empty
	 * base commit.
	 */
	شيون = commit_list_ابتو(شيون);
	لكل (cur = شيون; cur; cur = cur->ايندڙ) {
		بنية commit_list *parents = عدم;
		بنية commit_list **ايندڙ = &parents;
		بنية object_id ٻاهريون;
		بنية commit *stash = cur->شي;

		ايندڙ = commit_list_append(prev, ايندڙ);
		ايندڙ = commit_list_append(stash, ايندڙ);
		res = write_commit_with_parents(r, &ٻاهريون, &stash->object.oid, parents);
		commit_list_free(parents);
		إذا (res)
			اذهب ٻاهريون;
		prev = lookup_commit_reference(r, &ٻاهريون);
	}
	إذا (ref)
		refs_update_ref(get_main_ref_store(r), عدم, ref,
				&prev->object.oid, عدم, 0, UPDATE_REFS_DIE_ON_ERR);
	وإلا
		puts(oid_منزل_hex(&prev->object.oid));
ٻاهريون:
	strbuf_release(&revision);
	commit_list_free(شيون);

	أرجع res;
}

تعداد export_action {
	ACTION_NONE,
	ACTION_PRINT,
	ACTION_منزل_REF,
};

ساكن صحيح export_stash(صحيح argc,
			ثابت حرف **argv,
			ثابت حرف *اڳياڙي,
			بنية repository *repo)
{
	ثابت حرف *ref = عدم;
	تعداد export_action action = ACTION_NONE;
	بنية option options[] = {
		OPT_CMDMODE(0, "print", &action,
			    N_("print the object ID instead of writing it to a ref"),
			    ACTION_PRINT),
		OPT_STRING(0, "to-ref", &ref, "ref",
			    N_("save the data to the given ref")),
		OPT_پڇاڙي()
	};

	argc = parse_options(argc, argv, اڳياڙي, options,
			     git_stash_export_usage,
			     PARSE_OPT_KEEP_DASHDASH);

	إذا (ref && action == ACTION_NONE)
		action = ACTION_منزل_REF;

	إذا (action == ACTION_NONE || (ref && action == ACTION_PRINT))
		أرجع error(_("exactly one of --print and --to-ref is required"));

	أرجع do_export_stash(repo, ref, argc, argv);
}

صحيح cmd_stash(صحيح argc,
	      ثابت حرف **argv,
	      ثابت حرف *اڳياڙي,
	      بنية repository *repo)
{
	pid_t pid = getpid();
	ثابت حرف *اشاريو_file;
	بنية strvec دليلون = STRVEC_INIT;
	parse_opt_subcommand_fn *fn = عدم;
	بنية option options[] = {
		OPT_SUBCOMMAND("apply", &fn, apply_stash),
		OPT_SUBCOMMAND("clear", &fn, clear_stash),
		OPT_SUBCOMMAND("drop", &fn, drop_stash),
		OPT_SUBCOMMAND("pop", &fn, ڪڍو_stash),
		OPT_SUBCOMMAND("branch", &fn, branch_stash),
		OPT_SUBCOMMAND("list", &fn, list_stash),
		OPT_SUBCOMMAND("show", &fn, show_stash),
		OPT_SUBCOMMAND("store", &fn, store_stash),
		OPT_SUBCOMMAND("create", &fn, create_stash),
		OPT_SUBCOMMAND("push", &fn, ڌڪو_stash_unassumed),
		OPT_SUBCOMMAND("export", &fn, export_stash),
		OPT_SUBCOMMAND("import", &fn, import_stash),
		OPT_SUBCOMMAND_F("save", &fn, save_stash, عدم, PARSE_OPT_NOCOMPLETE),
		OPT_پڇاڙي()
	};
	ثابت حرف **دليلون_copy;
	صحيح ret;

	repo_config(the_repository, git_stash_config, عدم);

	argc = parse_options(argc, argv, اڳياڙي, options, git_stash_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL |
			     PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_ڀريل_اشاريو = 0;

	اشاريو_file = repo_get_اشاريو_file(the_repository);
	strbuf_addf(&stash_اشاريو_رستو, "%s.stash.%" PRIuMAX, اشاريو_file,
		    (uintmax_t)pid);

	إذا (fn) {
		ret = fn(argc, argv, اڳياڙي, repo);

		/*
		 * The subcommand implementations return 0 on success, a
		 * negative value on failure, and STASH_APPLY_CONFLICT
		 * when applying a stash entry resulted in conflicts.
		 * Map failures to 128, the status die() uses, so that
		 * exit status 1 unambiguously indicates conflicts.
		 */
		إذا (ret < 0)
			أرجع 128;
		أرجع ret;
	} وإلا إذا (!argc) {
		أرجع !!ڌڪو_stash_unassumed(0, عدم, اڳياڙي, repo);
	}

	/* Assume 'stash push' */
	strvec_ڌڪو(&دليلون, "push");
	strvec_pushv(&دليلون, argv);

	/*
	 * `push_stash()` ends up modifying the array, which causes memory
	 * leaks if we didn't copy the array here.
	 */
	DUP_ARRAY(دليلون_copy, دليلون.v, دليلون.nr);

	ret = !!ڌڪو_stash(دليلون.nr, دليلون_copy, اڳياڙي, 1);

	strvec_clear(&دليلون);
	حرر(دليلون_copy);
	أرجع ret;
}
