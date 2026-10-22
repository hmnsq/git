#تعریف_کرو USE_THE_REPOSITORY_VARIABLE

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

#تعریف_کرو INCLUDE_ALL_FILES 2

#تعریف_کرو BUILTIN_STASH_LIST_USAGE \
	N_("git stash list [<log-options>]")
#تعریف_کرو BUILTIN_STASH_SHOW_USAGE \
	N_("git stash show [-u | --include-untracked | --only-untracked] [<diff-options>] [<stash>]")
#تعریف_کرو BUILTIN_STASH_DROP_USAGE \
	N_("git stash drop [-q | --quiet] [<stash>]")
#تعریف_کرو BUILTIN_STASH_نکالو_USAGE \
	N_("git stash pop [--index] [-q | --quiet] [<stash>]")
#تعریف_کرو BUILTIN_STASH_APPLY_USAGE \
	N_("git stash apply [--index] [-q | --quiet] [--label-ours=<label>] [--label-theirs=<label>] [--label-base=<label>] [<stash>]")
#تعریف_کرو BUILTIN_STASH_BRANCH_USAGE \
	N_("git stash branch <branchname> [<stash>]")
#تعریف_کرو BUILTIN_STASH_STORE_USAGE \
	N_("git stash store [(-m | --message) <message>] [-q | --quiet] <commit>")
#تعریف_کرو BUILTIN_STASH_ڈالو_USAGE \
	N_("git stash [push] [-p | --patch] [-S | --staged] [-k | --[no-]keep-index] [-q | --quiet]\n" \
	   "          [-u | --include-untracked] [-a | --all] [(-m | --message) <message>]\n" \
	   "          [--pathspec-from-file=<file> [--pathspec-file-nul]]\n" \
	   "          [--] [<pathspec>...]")
#تعریف_کرو BUILTIN_STASH_SAVE_USAGE \
	N_("git stash save [-p | --patch] [-S | --staged] [-k | --[no-]keep-index] [-q | --quiet]\n" \
	   "          [-u | --include-untracked] [-a | --all] [<message>]")
#تعریف_کرو BUILTIN_STASH_CREATE_USAGE \
	N_("git stash create [<message>]")
#تعریف_کرو BUILTIN_STASH_EXPORT_USAGE \
	N_("git stash export (--print | --to-ref <ref>) [<stash>...]")
#تعریف_کرو BUILTIN_STASH_IMPORT_USAGE \
	N_("git stash import <commit>")
#تعریف_کرو BUILTIN_STASH_CLEAR_USAGE \
	"git stash clear"

ساكن ثابت حرف * ثابت git_stash_usage[] = {
	BUILTIN_STASH_LIST_USAGE,
	BUILTIN_STASH_SHOW_USAGE,
	BUILTIN_STASH_DROP_USAGE,
	BUILTIN_STASH_نکالو_USAGE,
	BUILTIN_STASH_APPLY_USAGE,
	BUILTIN_STASH_BRANCH_USAGE,
	BUILTIN_STASH_ڈالو_USAGE,
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

ساكن ثابت حرف * ثابت git_stash_نکالو_usage[] = {
	BUILTIN_STASH_نکالو_USAGE,
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

ساكن ثابت حرف * ثابت git_stash_ڈالو_usage[] = {
	BUILTIN_STASH_ڈالو_USAGE,
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
ساكن بنية strbuf stash_اشاریہ_رستہ = STRBUF_INIT;

ساكن صحيح show_stat = 1;
ساكن صحيح show_patch;
ساكن صحيح show_include_untracked;
ساكن صحيح use_اشاریہ;

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
	بنية object_id w_درخت;
	بنية object_id b_درخت;
	بنية object_id i_درخت;
	بنية object_id u_درخت;
	بنية strbuf revision;
	صحيح is_stash_ref;
	صحيح has_u;
};

#تعریف_کرو STASH_INFO_INIT { \
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
	إذا (!stash->parents || !stash->parents->اگلا ||
			(stash->parents->اگلا->اگلا && stash->parents->اگلا->اگلا->اگلا))
		أرجع -1;
	p1 = stash->parents->شے;
	p2 = stash->parents->اگلا->شے;
	إذا (stash->parents->اگلا->اگلا)
		p3 = stash->parents->اگلا->اگلا->شے;
	إذا (repo_parse_commit(r, p1) || repo_parse_commit(r, p2) ||
			(p3 && repo_parse_commit(r, p3)))
		أرجع -1;
	/* p2 must have a single parent, p3 must have no parents */
	إذا (!p2->parents || p2->parents->اگلا || (p3 && p3->parents))
		أرجع -1;
	إذا (repo_parse_commit(r, p2->parents->شے))
		أرجع -1;
	/* p2^1 must equal p1 */
	إذا (!oideq(&p1->object.oid, &p2->parents->شے->object.oid))
		أرجع -1;

	أرجع 0;
}

ساكن فراغ assert_stash_like(بنية stash_info *info, ثابت حرف *revision)
{
	إذا (get_oidf(&info->b_commit, "%s^1", revision) ||
	    get_oidf(&info->w_درخت, "%s:", revision) ||
	    get_oidf(&info->b_درخت, "%s^1:", revision) ||
	    get_oidf(&info->i_درخت, "%s^2:", revision))
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
	} وإلا إذا (strspn(commit, "0123456789") == لڑی_لمبائی(commit)) {
		strbuf_addf(revision, "%s@{%s}", ref_stash, commit);
	} وإلا {
		strbuf_addstr(revision, commit);
	}
	أرجع 0;
}

ساكن صحيح get_stash_info(بنية stash_info *info, صحيح argc, ثابت حرف **argv)
{
	صحيح ret;
	حرف *سرا_دا_rev;
	حرف *expanded_ref;
	ثابت حرف *revision;
	ثابت حرف *commit = عدم;
	بنية object_id dummy;
	بنية strbuf symbolic = STRBUF_INIT;

	إذا (argc > 1) {
		صحيح i;
		بنية strbuf refs_پیغام = STRBUF_INIT;

		لكل (i = 0; i < argc; i++)
			strbuf_addf(&refs_پیغام, " '%s'", argv[i]);

		fprintf_ln(stderr, _("Too many revisions specified:%s"),
			   refs_پیغام.buf);
		strbuf_release(&refs_پیغام);

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

	info->has_u = !get_oidf(&info->u_درخت, "%s^3:", revision);

	سرا_دا_rev = strchrnul(revision, '@');
	strbuf_جوڑو(&symbolic, revision, سرا_دا_rev - revision);

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

ساكن صحيح clear_stash(صحيح argc, ثابت حرف **argv, ثابت حرف *اگیتر,
		       بنية repository *repo بن_ورتیا)
{
	بنية option options[] = {
		OPT_سرا()
	};

	argc = parse_options(argc, argv, اگیتر, options,
			     git_stash_clear_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	إذا (argc)
		أرجع error(_("git stash clear with arguments is "
			       "unimplemented"));

	أرجع do_clear_stash();
}

ساكن صحيح reset_درخت(بنية object_id *i_درخت, صحيح update, صحيح reset)
{
	صحيح nr_trees = 1;
	بنية unpack_trees_options opts;
	بنية درخت_desc t[MAX_UNPACK_TREES];
	بنية درخت *درخت;
	بنية جندرا_file جندرا_file = جندرا_INIT;

	repo_read_اشاریہ_preload(the_repository, عدم, 0);
	إذا (refresh_اشاریہ(the_repository->اشاریہ, REFRESH_QUIET, عدم, عدم, عدم))
		أرجع -1;

	repo_hold_locked_اشاریہ(the_repository, &جندرا_file, جندرا_DIE_ON_ERROR);

	memset(&opts, 0, حجم(opts));

	درخت = repo_parse_درخت_اسدھا(the_repository, i_درخت);
	إذا (repo_parse_درخت(the_repository, درخت))
		أرجع -1;

	init_درخت_desc(t, &درخت->object.oid, درخت->بفر, درخت->ناپ);

	opts.سر_idx = 1;
	opts.src_اشاریہ = the_repository->اشاریہ;
	opts.dst_اشاریہ = the_repository->اشاریہ;
	opts.ملاؤ = 1;
	opts.reset = reset ? UNPACK_RESET_PROTECT_UNTRACKED : 0;
	opts.update = update;
	إذا (update)
		opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
	opts.fn = oneway_ملاؤ;

	إذا (unpack_trees(nr_trees, t, &opts))
		أرجع -1;

	إذا (write_locked_اشاریہ(the_repository->اشاریہ, &جندرا_file, COMMIT_جندرا))
		أرجع error(_("unable to write new index file"));

	أرجع 0;
}

ساكن صحيح create_اشاریہ_from_درخت(ثابت بنية object_id *درخت_id,
				  ثابت حرف *اشاریہ_رستہ)
{
	صحيح nr_trees = 1;
	صحيح ret = 0;
	بنية unpack_trees_options opts;
	بنية درخت_desc t[MAX_UNPACK_TREES];
	بنية درخت *درخت;
	بنية اشاریہ_state dst_istate = اشاریہ_STATE_INIT(the_repository);
	بنية جندرا_file جندرا_file = جندرا_INIT;

	repo_read_اشاریہ_preload(the_repository, عدم, 0);
	refresh_اشاریہ(the_repository->اشاریہ, REFRESH_QUIET, عدم, عدم, عدم);

	hold_جندرا_file_for_update(&جندرا_file, اشاریہ_رستہ, جندرا_DIE_ON_ERROR);

	memset(&opts, 0, حجم(opts));

	درخت = repo_parse_درخت_اسدھا(the_repository, درخت_id);
	إذا (!درخت || repo_parse_درخت(the_repository, درخت)) {
		ret = -1;
		اذهب پورا;
	}

	init_درخت_desc(t, &درخت->object.oid, درخت->بفر, درخت->ناپ);

	opts.سر_idx = 1;
	opts.src_اشاریہ = the_repository->اشاریہ;
	opts.dst_اشاریہ = &dst_istate;
	opts.ملاؤ = 1;
	opts.reset = UNPACK_RESET_PROTECT_UNTRACKED;
	opts.fn = oneway_ملاؤ;

	إذا (unpack_trees(nr_trees, t, &opts)) {
		ret = -1;
		اذهب پورا;
	}

	إذا (write_locked_اشاریہ(&dst_istate, &جندرا_file, COMMIT_جندرا)) {
		ret = error(_("unable to write new index file"));
		اذهب پورا;
	}

پورا:
	release_اشاریہ(&dst_istate);
	إذا (ret)
		rollback_جندرا_file(&جندرا_file);
	أرجع ret;
}

ساكن صحيح is_رستہ_a_directory(ثابت حرف *رستہ)
{
	/*
	 * This function differs from abspath.c:is_directory() in that
	 * here we use lstat() instead of stat(); we do not want to
	 * follow symbolic links here.
	 */
	بنية stat st;
	أرجع (!lstat(رستہ, &st) && S_ISDIR(st.st_منوال));
}

ساكن فراغ جوڑو_diff_منزل_buf(بنية diff_قطار_struct *q,
			    بنية diff_options *options بن_ورتیا,
			    فراغ *انکڑے)
{
	صحيح i;

	لكل (i = 0; i < q->nr; i++) {
		إذا (is_رستہ_a_directory(q->قطار[i]->one->رستہ))
			استمر;

		strbuf_addstr(انکڑے, q->قطار[i]->one->رستہ);

		/* NUL-terminate: will be fed to update-index -z */
		strbuf_addch(انکڑے, '\0');
	}
}

ساكن صحيح restore_untracked(بنية object_id *u_درخت)
{
	صحيح res;
	بنية بچہ_process cp = بچہ_PROCESS_INIT;

	/*
	 * We need to run restore files from a given index, but without
	 * affecting the current index, so we use GIT_INDEX_FILE with
	 * run_command to fork processes that will not interfere.
	 */
	cp.git_cmd = 1;
	strvec_ڈالو(&cp.دلیلاں, "read-tree");
	strvec_ڈالو(&cp.دلیلاں, oid_منزل_hex(u_درخت));
	strvec_pushf(&cp.env, "GIT_INDEX_FILE=%s",
		     stash_اشاریہ_رستہ.buf);
	إذا (لگاتار_command(&cp)) {
		remove_رستہ(stash_اشاریہ_رستہ.buf);
		أرجع -1;
	}

	بچہ_process_init(&cp);
	cp.git_cmd = 1;
	strvec_pushl(&cp.دلیلاں, "checkout-index", "--all", عدم);
	strvec_pushf(&cp.env, "GIT_INDEX_FILE=%s",
		     stash_اشاریہ_رستہ.buf);

	res = لگاتار_command(&cp);
	remove_رستہ(stash_اشاریہ_رستہ.buf);
	أرجع res;
}

ساكن فراغ unstage_changes_unless_new(بنية object_id *orig_درخت)
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
	بنية جندرا_file جندرا = جندرا_INIT;
	صحيح i;

	/* If any entries have skip_worktree set, we'll have to check 'em out */
	state.force = 1;
	state.quiet = 1;
	state.refresh_cache = 1;
	state.istate = the_repository->اشاریہ;

	/*
	 * Step 1: get a difference between orig_tree (which corresponding
	 * to the index before a merge was run) and the current index
	 * (reflecting the changes brought in by the merge).
	 */
	repo_diff_تیاری(the_repository, &diff_opts);
	diff_opts.flags.recursive = 1;
	diff_opts.detect_rename = 0;
	diff_opts.output_format = DIFF_FORMAT_NO_OUTPUT;
	diff_تیاری_پورا(&diff_opts);

	do_diff_cache(orig_درخت, &diff_opts);
	diffcore_std(&diff_opts);

	/* Iterate over the paths that changed due to the merge... */
	لكل (i = 0; i < diff_queued_diff.nr; i++) {
		بنية diff_filepair *p;
		بنية cache_اندراج *ce;
		صحيح pos;

		/* Look up the path's position in the current index. */
		p = diff_queued_diff.قطار[i];
		pos = اشاریہ_name_pos(the_repository->اشاریہ, p->two->رستہ,
				     لڑی_لمبائی(p->two->رستہ));

		/*
		 * Step 2: Place changes in the working tree
		 *
		 * Stash is about restoring changes *to the working tree*.
		 * So if the merge successfully got a new version of some
		 * path, but left it out of the working tree, then clear the
		 * SKIP_WORKTREE bit and write it to the working tree.
		 */
		إذا (pos >= 0 && ce_skip_worktree(the_repository->اشاریہ->cache[pos])) {
			بنية stat st;

			ce = the_repository->اشاریہ->cache[pos];
			إذا (!lstat(ce->name, &st)) {
				/* Conflicting path present; relocate it */
				بنية strbuf new_رستہ = STRBUF_INIT;
				صحيح fd;

				strbuf_addf(&new_رستہ,
					    "%s.stash.XXXXXX", ce->name);
				fd = xmkstemp(new_رستہ.buf);
				close(fd);
				لکھو(_("WARNING: Untracked file in way of "
					 "tracked file!  Renaming\n "
					 "           %s -> %s\n"
					 "         to make room.\n"),
				       ce->name, new_رستہ.buf);
				إذا (rename(ce->name, new_رستہ.buf))
					die("Failed to move %s to %s",
					    ce->name, new_رستہ.buf);
				strbuf_release(&new_رستہ);
			}
			checkout_اندراج(ce, &state, عدم, عدم);
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
			صحيح option = جوڑو_CACHE_OK_منزل_REPLACE;
			إذا (pos < 0)
				option = جوڑو_CACHE_OK_منزل_جوڑو;

			ce = make_cache_اندراج(the_repository->اشاریہ,
					      p->one->منوال,
					      &p->one->oid,
					      p->one->رستہ,
					      0, 0);
			جوڑو_اشاریہ_اندراج(the_repository->اشاریہ, ce, option);
		}
	}
	diff_flush(&diff_opts);

	/*
	 * Step 4: write the new index to disk
	 */
	repo_hold_locked_اشاریہ(the_repository, &جندرا, جندرا_DIE_ON_ERROR);
	إذا (write_locked_اشاریہ(the_repository->اشاریہ, &جندرا,
			       COMMIT_جندرا | SKIP_IF_UNCHANGED))
		die(_("could not write index"));
}

ساكن تعداد stash_apply_نتیجہ do_apply_stash(ثابت حرف *اگیتر,
					      بنية stash_info *info,
					      صحيح اشاریہ, صحيح quiet,
					      ثابت حرف *label_ours,
					      ثابت حرف *label_theirs,
					      ثابت حرف *label_ادھار)
{
	صحيح clean, ret;
	صحيح has_اشاریہ = اشاریہ;
	بنية ملاؤ_options o;
	بنية object_id c_درخت;
	بنية object_id اشاریہ_درخت;
	بنية درخت *سر, *ملاؤ, *ملاؤ_ادھار;
	بنية جندرا_file جندرا = جندرا_INIT;

	repo_read_اشاریہ_preload(the_repository, عدم, 0);
	إذا (repo_refresh_and_write_اشاریہ(the_repository, REFRESH_QUIET, 0, 0,
					 عدم, عدم, عدم))
		أرجع error(_("could not write index"));

	إذا (write_اشاریہ_as_درخت(&c_درخت, the_repository->اشاریہ,
				repo_get_اشاریہ_file(the_repository), 0, عدم))
		أرجع error(_("cannot apply a stash in the middle of a merge"));

	init_ui_ملاؤ_options(&o, the_repository);

	إذا (quiet)
		o.verbosity = 0;

	إذا (اشاریہ) {
		إذا (oideq(&info->b_درخت, &info->i_درخت) ||
		    oideq(&c_درخت, &info->i_درخت)) {
			has_اشاریہ = 0;
		} وإلا {
			بنية ملاؤ_نتیجہ نتیجہ = { 0 };

			o.branch1 = "Current index";
			o.branch2 = "Stashed index changes";
			o.ancestor = "Stash base";

			سر = lookup_درخت(o.repo, &c_درخت);
			ملاؤ = lookup_درخت(o.repo, &info->i_درخت);
			ملاؤ_ادھار = lookup_درخت(o.repo, &info->b_درخت);

			ملاؤ_incore_nonrecursive(&o, ملاؤ_ادھار, سر, ملاؤ,
						  &نتیجہ);

			إذا (نتیجہ.clean < 0) {
				ملاؤ_finalize(&o, &نتیجہ);
				أرجع error(_("index merge failed"));
			} وإلا إذا (!نتیجہ.clean) {
				ملاؤ_finalize(&o, &نتیجہ);
				أرجع error(_("conflicts in index. "
					       "Try without --index."));
			} وإلا {
				oidcpy(&اشاریہ_درخت, &نتیجہ.درخت->object.oid);
				ملاؤ_finalize(&o, &نتیجہ);
			}
		}
	}

	o.branch1 = label_ours ? label_ours : "Updated upstream";
	o.branch2 = label_theirs ? label_theirs : "Stashed changes";
	o.ancestor = label_ادھار ? label_ادھار : "Stash base";

	إذا (oideq(&info->b_درخت, &c_درخت))
		o.branch1 = "Version stash was based on";

	إذا (o.verbosity >= 3)
		printf_ln(_("Merging %s with %s"), o.branch1, o.branch2);

	سر = lookup_درخت(o.repo, &c_درخت);
	ملاؤ = lookup_درخت(o.repo, &info->w_درخت);
	ملاؤ_ادھار = lookup_درخت(o.repo, &info->b_درخت);

	repo_hold_locked_اشاریہ(o.repo, &جندرا, جندرا_DIE_ON_ERROR);
	clean = ملاؤ_ort_nonrecursive(&o, سر, ملاؤ, ملاؤ_ادھار);

	/*
	 * If 'clean' >= 0, reverse the value for 'ret' so 'ret' is 0 when the
	 * merge was clean, and 1 if the merge was unclean or a negative value
	 * if it encountered an error.
	 */
	ret = clean >= 0 ? !clean : clean;

	إذا (ret < 0)
		rollback_جندرا_file(&جندرا);
	وإلا إذا (write_locked_اشاریہ(o.repo->اشاریہ, &جندرا,
				      COMMIT_جندرا | SKIP_IF_UNCHANGED))
		ret = error(_("could not write index"));

	إذا (ret) {
		repo_rerere(the_repository, 0);

		إذا (اشاریہ)
			fprintf_ln(stderr, _("Index was not unstashed."));

		اذهب restore_untracked;
	}

	إذا (has_اشاریہ) {
		إذا (reset_درخت(&اشاریہ_درخت, 0, 0))
			ret = -1;
	} وإلا {
		unstage_changes_unless_new(&c_درخت);
	}

restore_untracked:
	إذا (info->has_u && restore_untracked(&info->u_درخت))
		ret = error(_("could not restore untracked files from stash"));

	إذا (!quiet) {
		بنية بچہ_process cp = بچہ_PROCESS_INIT;

		/*
		 * Status is quite simple and could be replaced with calls to
		 * wt_status in the future, but it adds complexities which may
		 * require more tests.
		 */
		cp.git_cmd = 1;
		cp.dir = اگیتر;
		strvec_pushf(&cp.env, GIT_WORK_درخت_ENVIRONMENT"=%s",
			     absolute_رستہ(repo_get_work_درخت(the_repository)));
		strvec_pushf(&cp.env, GIT_DIR_ENVIRONMENT"=%s",
			     absolute_رستہ(repo_get_git_dir(the_repository)));
		strvec_ڈالو(&cp.دلیلاں, "status");
		لگاتار_command(&cp);
	}

	أرجع ret;
}

ساكن صحيح apply_stash(صحيح argc, ثابت حرف **argv, ثابت حرف *اگیتر,
		       بنية repository *repo بن_ورتیا)
{
	صحيح ret = -1;
	صحيح quiet = 0;
	صحيح اشاریہ = use_اشاریہ;
	ثابت حرف *label_ours = عدم, *label_theirs = عدم, *label_ادھار = عدم;
	بنية stash_info info = STASH_INFO_INIT;
	بنية option options[] = {
		OPT__QUIET(&quiet, N_("be quiet, only report errors")),
		OPT_BOOL(0, "index", &اشاریہ,
			 N_("attempt to recreate the index")),
		OPT_STRING(0, "label-ours", &label_ours, N_("label"),
			   N_("label for the upstream side in conflict markers")),
		OPT_STRING(0, "label-theirs", &label_theirs, N_("label"),
			   N_("label for the stashed side in conflict markers")),
		OPT_STRING(0, "label-base", &label_ادھار, N_("label"),
			   N_("label for the base in diff3 conflict markers")),
		OPT_سرا()
	};

	argc = parse_options(argc, argv, اگیتر, options,
			     git_stash_apply_usage, 0);

	إذا (get_stash_info(&info, argc, argv))
		اذهب cleanup;

	ret = do_apply_stash(اگیتر, &info, اشاریہ, quiet,
			     label_ours, label_theirs, label_ادھار);
cleanup:
	free_stash_info(&info);
	أرجع ret;
}

ساكن صحيح reject_reflog_ent(ثابت حرف *refname بن_ورتیا,
			     بنية object_id *ooid بن_ورتیا,
			     بنية object_id *noid بن_ورتیا,
			     ثابت حرف *email بن_ورتیا,
			     timestamp_t timestamp بن_ورتیا,
			     صحيح tz بن_ورتیا, ثابت حرف *سنیہا بن_ورتیا,
			     فراغ *cb_انکڑے بن_ورتیا)
{
	أرجع 1;
}

ساكن صحيح reflog_is_خالی_تھاواں(ثابت حرف *refname)
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

	إذا (reflog_is_خالی_تھاواں(ref_stash))
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

ساكن صحيح drop_stash(صحيح argc, ثابت حرف **argv, ثابت حرف *اگیتر,
		      بنية repository *repo بن_ورتیا)
{
	صحيح ret = -1;
	صحيح quiet = 0;
	بنية stash_info info = STASH_INFO_INIT;
	بنية option options[] = {
		OPT__QUIET(&quiet, N_("be quiet, only report errors")),
		OPT_سرا()
	};

	argc = parse_options(argc, argv, اگیتر, options,
			     git_stash_drop_usage, 0);

	إذا (get_stash_info_assert(&info, argc, argv))
		اذهب cleanup;

	ret = do_drop_stash(&info, quiet);
cleanup:
	free_stash_info(&info);
	أرجع ret;
}

ساكن صحيح نکالو_stash(صحيح argc, ثابت حرف **argv, ثابت حرف *اگیتر,
		     بنية repository *repo بن_ورتیا)
{
	صحيح ret = -1;
	صحيح اشاریہ = use_اشاریہ;
	صحيح quiet = 0;
	بنية stash_info info = STASH_INFO_INIT;
	بنية option options[] = {
		OPT__QUIET(&quiet, N_("be quiet, only report errors")),
		OPT_BOOL(0, "index", &اشاریہ,
			 N_("attempt to recreate the index")),
		OPT_سرا()
	};

	argc = parse_options(argc, argv, اگیتر, options,
			     git_stash_نکالو_usage, 0);

	إذا (get_stash_info_assert(&info, argc, argv))
		اذهب cleanup;

	إذا ((ret = do_apply_stash(اگیتر, &info, اشاریہ, quiet,
				  عدم, عدم, عدم)))
		printf_ln(_("The stash entry is kept in case "
			    "you need it again."));
	وإلا
		ret = do_drop_stash(&info, quiet);

cleanup:
	free_stash_info(&info);
	أرجع ret;
}

ساكن صحيح branch_stash(صحيح argc, ثابت حرف **argv, ثابت حرف *اگیتر,
			بنية repository *repo بن_ورتیا)
{
	صحيح ret = -1;
	ثابت حرف *branch = عدم;
	بنية stash_info info = STASH_INFO_INIT;
	بنية بچہ_process cp = بچہ_PROCESS_INIT;
	بنية option options[] = {
		OPT_سرا()
	};

	argc = parse_options(argc, argv, اگیتر, options,
			     git_stash_branch_usage, 0);

	إذا (!argc) {
		fprintf_ln(stderr, _("No branch name specified"));
		أرجع -1;
	}

	branch = argv[0];

	إذا (get_stash_info(&info, argc - 1, argv + 1))
		اذهب cleanup;

	cp.git_cmd = 1;
	strvec_pushl(&cp.دلیلاں, "checkout", "-b", عدم);
	strvec_ڈالو(&cp.دلیلاں, branch);
	strvec_ڈالو(&cp.دلیلاں, oid_منزل_hex(&info.b_commit));
	ret = لگاتار_command(&cp);
	إذا (!ret)
		ret = do_apply_stash(اگیتر, &info, 1, 0,
				     عدم, عدم, عدم);
	إذا (!ret && info.is_stash_ref)
		ret = do_drop_stash(&info, 0);

cleanup:
	free_stash_info(&info);
	أرجع ret;
}

ساكن صحيح list_stash(صحيح argc, ثابت حرف **argv, ثابت حرف *اگیتر,
		      بنية repository *repo بن_ورتیا)
{
	بنية بچہ_process cp = بچہ_PROCESS_INIT;
	بنية option options[] = {
		OPT_سرا()
	};

	argc = parse_options(argc, argv, اگیتر, options,
			     git_stash_list_usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT);

	إذا (!refs_ref_exists(get_main_ref_store(the_repository), ref_stash))
		أرجع 0;

	cp.git_cmd = 1;
	strvec_pushl(&cp.دلیلاں, "log", "--format=%gd: %gs", "-g",
		     "--first-parent", عدم);
	strvec_pushv(&cp.دلیلاں, argv);
	strvec_ڈالو(&cp.دلیلاں, ref_stash);
	strvec_ڈالو(&cp.دلیلاں, "--");
	أرجع لگاتار_command(&cp);
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
		use_اشاریہ = git_config_bool(var, قدر);
		أرجع 0;
	}
	أرجع git_diff_basic_config(var, قدر, ctx, cb);
}

ساكن فراغ diff_include_untracked(ثابت بنية stash_info *info, بنية diff_options *diff_opt)
{
	ثابت بنية object_id *oid[] = { &info->w_commit, &info->u_درخت };
	بنية درخت *درخت[ARRAY_ناپ(oid)];
	بنية درخت_desc درخت_desc[ARRAY_ناپ(oid)];
	بنية unpack_trees_options unpack_درخت_opt = { 0 };

	لكل (ناپ_t i = 0; i < ARRAY_ناپ(oid); i++) {
		درخت[i] = repo_parse_درخت_اسدھا(the_repository, oid[i]);
		إذا (repo_parse_درخت(the_repository, درخت[i]) < 0)
			die(_("failed to parse tree"));
		init_درخت_desc(&درخت_desc[i], &درخت[i]->object.oid,
			       درخت[i]->بفر, درخت[i]->ناپ);
	}

	unpack_درخت_opt.سر_idx = -1;
	unpack_درخت_opt.src_اشاریہ = the_repository->اشاریہ;
	unpack_درخت_opt.dst_اشاریہ = the_repository->اشاریہ;
	unpack_درخت_opt.ملاؤ = 1;
	unpack_درخت_opt.fn = stash_worktree_untracked_ملاؤ;

	إذا (unpack_trees(ARRAY_ناپ(درخت_desc), درخت_desc, &unpack_درخت_opt))
		die(_("failed to unpack trees"));

	do_diff_cache(&info->b_commit, diff_opt);
}

ساكن صحيح show_stash(صحيح argc, ثابت حرف **argv, ثابت حرف *اگیتر,
		      بنية repository *repo بن_ورتیا)
{
	صحيح i;
	صحيح ret = -1;
	بنية stash_info info = STASH_INFO_INIT;
	بنية rev_info rev;
	بنية strvec stash_دلیلاں = STRVEC_INIT;
	بنية strvec revision_دلیلاں = STRVEC_INIT;
	تعداد {
		UNTRACKED_NONE,
		UNTRACKED_INCLUDE,
		UNTRACKED_ONLY
	} show_untracked = show_include_untracked ? UNTRACKED_INCLUDE : UNTRACKED_NONE;
	بنية option options[] = {
		OPT_سیٹ_INT('u', "include-untracked", &show_untracked,
			    N_("include untracked files in the stash"),
			    UNTRACKED_INCLUDE),
		OPT_سیٹ_INT_F(0, "only-untracked", &show_untracked,
			      N_("only show untracked files in the stash"),
			      UNTRACKED_ONLY, PARSE_OPT_NONEG),
		OPT_سرا()
	};
	صحيح do_usage = 0;

	init_diff_ui_defaults();
	repo_config(the_repository, git_diff_ui_config, عدم);
	repo_init_revisions(the_repository, &rev, اگیتر);

	argc = parse_options(argc, argv, اگیتر, options, git_stash_show_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	strvec_ڈالو(&revision_دلیلاں, argv[0]);
	لكل (i = 1; i < argc; i++) {
		إذا (argv[i][0] != '-')
			strvec_ڈالو(&stash_دلیلاں, argv[i]);
		وإلا
			strvec_ڈالو(&revision_دلیلاں, argv[i]);
	}

	إذا (get_stash_info(&info, stash_دلیلاں.nr, stash_دلیلاں.v))
		اذهب cleanup;

	/*
	 * The config settings are applied only if there are not passed
	 * any options.
	 */
	إذا (revision_دلیلاں.nr == 1) {
		إذا (show_stat)
			rev.diffopt.output_format = DIFF_FORMAT_DIFFSTAT;

		إذا (show_patch)
			rev.diffopt.output_format |= DIFF_FORMAT_PATCH;

		إذا (!show_stat && !show_patch) {
			ret = 0;
			اذهب cleanup;
		}
	}

	تیاری_revisions_from_strvec(&revision_دلیلاں, &rev, عدم);
	إذا (revision_دلیلاں.nr > 1)
		اذهب usage;
	إذا (!rev.diffopt.output_format) {
		rev.diffopt.output_format = DIFF_FORMAT_PATCH;
		diff_تیاری_پورا(&rev.diffopt);
	}

	rev.diffopt.flags.recursive = 1;
	تیاری_diff_pager(&rev.diffopt);
	بدل (show_untracked) {
	حالة UNTRACKED_NONE:
		diff_درخت_oid(&info.b_commit, &info.w_commit, "", &rev.diffopt);
		اكسر;
	حالة UNTRACKED_ONLY:
		إذا (info.has_u)
			diff_جڑ_درخت_oid(&info.u_درخت, "", &rev.diffopt);
		اكسر;
	حالة UNTRACKED_INCLUDE:
		إذا (info.has_u)
			diff_include_untracked(&info, &rev.diffopt);
		وإلا
			diff_درخت_oid(&info.b_commit, &info.w_commit, "", &rev.diffopt);
		اكسر;
	}
	log_درخت_diff_flush(&rev);

	ret = diff_نتیجہ_کوڈ(&rev);

cleanup:
	strvec_clear(&revision_دلیلاں);
	strvec_clear(&stash_دلیلاں);
	free_stash_info(&info);
	release_revisions(&rev);
	إذا (do_usage)
		usage_with_options(git_stash_show_usage, options);
	أرجع ret;
usage:
	do_usage = 1;
	اذهب cleanup;
}

ساكن صحيح do_store_stash(ثابت بنية object_id *w_commit, ثابت حرف *stash_پیغام,
			  صحيح quiet)
{
	بنية stash_info info;
	حرف revision[GIT_MAX_HEXSZ];

	oid_منزل_hex_r(revision, w_commit);
	assert_stash_like(&info, revision);

	إذا (!stash_پیغام)
		stash_پیغام = "Created via \"git stash store\".";

	إذا (refs_update_ref(get_main_ref_store(the_repository), stash_پیغام, ref_stash, w_commit, عدم,
			    REF_FORCE_CREATE_REFLOG,
			    quiet ? UPDATE_REFS_QUIET_ON_ERR :
			    UPDATE_REFS_پیغام_ON_ERR)) {
		إذا (!quiet) {
			fprintf_ln(stderr, _("Cannot update %s with %s"),
				   ref_stash, oid_منزل_hex(w_commit));
		}
		أرجع -1;
	}

	أرجع 0;
}

ساكن صحيح store_stash(صحيح argc, ثابت حرف **argv, ثابت حرف *اگیتر,
		       بنية repository *repo بن_ورتیا)
{
	صحيح quiet = 0;
	ثابت حرف *stash_پیغام = عدم;
	بنية object_id obj;
	بنية option options[] = {
		OPT__QUIET(&quiet, N_("be quiet")),
		OPT_STRING('m', "message", &stash_پیغام, "message",
			   N_("stash message")),
		OPT_سرا()
	};
	صحيح ret;

	argc = parse_options(argc, argv, اگیتر, options,
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
		اذهب باہرلا;
	}

	ret = do_store_stash(&obj, stash_پیغام, quiet);

باہرلا:
	أرجع ret;
}

ساكن فراغ جوڑو_pathspecs(بنية strvec *دلیلاں,
			  ثابت بنية pathspec *ps) {
	صحيح i;

	لكل (i = 0; i < ps->nr; i++)
		strvec_ڈالو(دلیلاں, ps->اشیاء[i].original);
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
	صحيح لبھیا_2 = 0;
	بنية dir_struct dir = DIR_INIT;

	إذا (include_untracked != INCLUDE_ALL_FILES)
		تیاری_standard_excludes(&dir);

	fill_directory(&dir, the_repository->اشاریہ, ps);
	لكل (i = 0; i < dir.nr; i++) {
		بنية dir_اندراج *ent = dir.entries[i];
		لبھیا_2++;
		strbuf_addstr(untracked_files, ent->name);
		/* NUL-terminate: will be fed to update-index -z */
		strbuf_addch(untracked_files, '\0');
	}

	dir_clear(&dir);
	أرجع لبھیا_2;
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

	إذا (repo_read_اشاریہ(the_repository) < 0)
		أرجع -1;

	repo_init_revisions(the_repository, &rev, عدم);
	copy_pathspec(&rev.prune_انکڑے, ps);

	rev.diffopt.flags.تیز = 1;
	rev.diffopt.flags.ignore_submodules = 1;
	rev.abbrev = 0;

	جوڑو_سر_منزل_pending(&rev);
	diff_تیاری_پورا(&rev.diffopt);

	لگاتار_diff_اشاریہ(&rev, DIFF_اشاریہ_CACHED);
	إذا (diff_نتیجہ_کوڈ(&rev)) {
		ret = 1;
		اذهب پورا;
	}

	لگاتار_diff_files(&rev, 0);
	إذا (diff_نتیجہ_کوڈ(&rev)) {
		ret = 1;
		اذهب پورا;
	}

پورا:
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

ساكن صحيح save_untracked_files(بنية stash_info *info, بنية strbuf *پیغام,
				بنية strbuf *files)
{
	صحيح ret = 0;
	بنية strbuf untracked_پیغام = STRBUF_INIT;
	بنية بچہ_process cp_upd_اشاریہ = بچہ_PROCESS_INIT;
	بنية اشاریہ_state istate = اشاریہ_STATE_INIT(the_repository);

	cp_upd_اشاریہ.git_cmd = 1;
	strvec_pushl(&cp_upd_اشاریہ.دلیلاں, "update-index", "-z", "--add",
		     "--remove", "--stdin", عدم);
	strvec_pushf(&cp_upd_اشاریہ.env, "GIT_INDEX_FILE=%s",
			 stash_اشاریہ_رستہ.buf);

	strbuf_addf(&untracked_پیغام, "untracked files on %s\n", پیغام->buf);
	إذا (pipe_command(&cp_upd_اشاریہ, files->buf, files->len, عدم, 0,
			 عدم, 0)) {
		ret = -1;
		اذهب پورا;
	}

	إذا (write_اشاریہ_as_درخت(&info->u_درخت, &istate, stash_اشاریہ_رستہ.buf, 0,
				عدم)) {
		ret = -1;
		اذهب پورا;
	}

	إذا (commit_درخت(untracked_پیغام.buf, untracked_پیغام.len,
			&info->u_درخت, عدم, &info->u_commit, عدم, عدم)) {
		ret = -1;
		اذهب پورا;
	}

پورا:
	release_اشاریہ(&istate);
	strbuf_release(&untracked_پیغام);
	remove_رستہ(stash_اشاریہ_رستہ.buf);
	أرجع ret;
}

ساكن صحيح stash_staged(بنية stash_info *info, بنية strbuf *باہرلا_patch,
			صحيح quiet)
{
	صحيح ret = 0;
	بنية بچہ_process cp_diff_درخت = بچہ_PROCESS_INIT;
	بنية اشاریہ_state istate = اشاریہ_STATE_INIT(the_repository);

	إذا (write_اشاریہ_as_درخت(&info->w_درخت, &istate, the_repository->اشاریہ_file,
				0, عدم)) {
		ret = -1;
		اذهب پورا;
	}

	cp_diff_درخت.git_cmd = 1;
	strvec_pushl(&cp_diff_درخت.دلیلاں, "diff-tree", "-p", "--binary",
		     "--no-color",
		     "-U1", "HEAD", oid_منزل_hex(&info->w_درخت), "--", عدم);
	إذا (pipe_command(&cp_diff_درخت, عدم, 0, باہرلا_patch, 0, عدم, 0)) {
		ret = -1;
		اذهب پورا;
	}

	إذا (!باہرلا_patch->len) {
		إذا (!quiet)
			fprintf_ln(stderr, _("No staged changes"));
		ret = 1;
	}

پورا:
	release_اشاریہ(&istate);
	أرجع ret;
}

ساكن صحيح stash_patch(بنية stash_info *info, ثابت بنية pathspec *ps,
		       بنية strbuf *باہرلا_patch, صحيح quiet,
		       بنية interactive_options *interactive_opts)
{
	صحيح ret = 0;
	بنية بچہ_process cp_diff_درخت = بچہ_PROCESS_INIT;
	بنية commit *سر_commit;
	ثابت بنية object_id *سر_درخت;
	بنية اشاریہ_state istate = اشاریہ_STATE_INIT(the_repository);
	حرف *old_اشاریہ_env = عدم, *old_repo_اشاریہ_file;

	remove_رستہ(stash_اشاریہ_رستہ.buf);

	سر_commit = lookup_commit(the_repository, &info->b_commit);
	إذا (!سر_commit || repo_parse_commit(the_repository, سر_commit)) {
		ret = -1;
		اذهب پورا;
	}
	سر_درخت = get_commit_درخت_oid(سر_commit);
	إذا (!سر_درخت) {
		ret = -1;
		اذهب پورا;
	}

	إذا (create_اشاریہ_from_درخت(سر_درخت, stash_اشاریہ_رستہ.buf)) {
		ret = -1;
		اذهب پورا;
	}

	/* Find out what the user wants. */
	old_repo_اشاریہ_file = the_repository->اشاریہ_file;
	the_repository->اشاریہ_file = stash_اشاریہ_رستہ.buf;
	old_اشاریہ_env = xstrdup_or_null(getenv(اشاریہ_ENVIRONMENT));
	setenv(اشاریہ_ENVIRONMENT, the_repository->اشاریہ_file, 1);

	ret = !!لگاتار_جوڑو_p(the_repository, جوڑو_P_STASH, interactive_opts, عدم, ps, 0);

	the_repository->اشاریہ_file = old_repo_اشاریہ_file;
	إذا (old_اشاریہ_env && *old_اشاریہ_env)
		setenv(اشاریہ_ENVIRONMENT, old_اشاریہ_env, 1);
	وإلا
		unsetenv(اشاریہ_ENVIRONMENT);
	FREE_AND_NULL(old_اشاریہ_env);

	/* State of the working tree. */
	إذا (write_اشاریہ_as_درخت(&info->w_درخت, &istate, stash_اشاریہ_رستہ.buf, 0,
				عدم)) {
		ret = -1;
		اذهب پورا;
	}

	cp_diff_درخت.git_cmd = 1;
	strvec_pushl(&cp_diff_درخت.دلیلاں, "diff-tree", "-p", "-U1", "HEAD",
		     "--no-color",
		     oid_منزل_hex(&info->w_درخت), "--", عدم);
	إذا (pipe_command(&cp_diff_درخت, عدم, 0, باہرلا_patch, 0, عدم, 0)) {
		ret = -1;
		اذهب پورا;
	}

	إذا (!باہرلا_patch->len) {
		إذا (!quiet)
			fprintf_ln(stderr, _("No changes selected"));
		ret = 1;
	}

پورا:
	release_اشاریہ(&istate);
	remove_رستہ(stash_اشاریہ_رستہ.buf);
	أرجع ret;
}

ساكن صحيح stash_working_درخت(بنية stash_info *info, ثابت بنية pathspec *ps)
{
	صحيح ret = 0;
	بنية rev_info rev;
	بنية بچہ_process cp_upd_اشاریہ = بچہ_PROCESS_INIT;
	بنية strbuf diff_output = STRBUF_INIT;
	بنية اشاریہ_state istate = اشاریہ_STATE_INIT(the_repository);

	repo_init_revisions(the_repository, &rev, عدم);
	copy_pathspec(&rev.prune_انکڑے, ps);

	سیٹ_alternate_اشاریہ_output(stash_اشاریہ_رستہ.buf);
	إذا (reset_درخت(&info->i_درخت, 0, 0)) {
		ret = -1;
		اذهب پورا;
	}
	سیٹ_alternate_اشاریہ_output(عدم);

	rev.diffopt.output_format = DIFF_FORMAT_CALLBACK;
	rev.diffopt.format_callback = جوڑو_diff_منزل_buf;
	rev.diffopt.format_callback_انکڑے = &diff_output;

	إذا (repo_read_اشاریہ_preload(the_repository, &rev.diffopt.pathspec, 0) < 0) {
		ret = -1;
		اذهب پورا;
	}

	جوڑو_pending_object(&rev, parse_object(the_repository, &info->b_commit),
			   "");
	لگاتار_diff_اشاریہ(&rev, 0);

	cp_upd_اشاریہ.git_cmd = 1;
	strvec_pushl(&cp_upd_اشاریہ.دلیلاں, "update-index",
		     "--ignore-skip-worktree-entries",
		     "-z", "--add", "--remove", "--stdin", عدم);
	strvec_pushf(&cp_upd_اشاریہ.env, "GIT_INDEX_FILE=%s",
		     stash_اشاریہ_رستہ.buf);

	إذا (pipe_command(&cp_upd_اشاریہ, diff_output.buf, diff_output.len,
			 عدم, 0, عدم, 0)) {
		ret = -1;
		اذهب پورا;
	}

	إذا (write_اشاریہ_as_درخت(&info->w_درخت, &istate, stash_اشاریہ_رستہ.buf, 0,
				عدم)) {
		ret = -1;
		اذهب پورا;
	}

پورا:
	release_اشاریہ(&istate);
	release_revisions(&rev);
	strbuf_release(&diff_output);
	remove_رستہ(stash_اشاریہ_رستہ.buf);
	أرجع ret;
}

ساكن صحيح do_create_stash(ثابت بنية pathspec *ps, بنية strbuf *stash_پیغام_buf,
			   صحيح include_untracked, صحيح patch_منوال,
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
	بنية strbuf پیغام = STRBUF_INIT;
	بنية strbuf commit_درخت_label = STRBUF_INIT;
	بنية strbuf untracked_files = STRBUF_INIT;

	prepare_fallback_ident("git stash", "git@stash");

	repo_read_اشاریہ_preload(the_repository, عدم, 0);
	إذا (repo_refresh_and_write_اشاریہ(the_repository, REFRESH_QUIET, 0, 0,
					 عدم, عدم, عدم) < 0) {
		ret = error(_("could not write index"));
		اذهب پورا;
	}

	إذا (repo_get_oid(the_repository, "HEAD", &info->b_commit)) {
		إذا (!quiet)
			fprintf_ln(stderr, _("You do not have "
					     "the initial commit yet"));
		ret = -1;
		اذهب پورا;
	} وإلا {
		سر_commit = lookup_commit(the_repository, &info->b_commit);
	}

	إذا (!check_changes(ps, include_untracked, &untracked_files)) {
		ret = 1;
		اذهب پورا;
	}

	branch_ref = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
					     "HEAD", 0, عدم, &flags);

	إذا (flags & REF_ISSYMREF) {
		إذا (skip_اگیتر(branch_ref, "refs/heads/", &branch_name))
			branch_name = branch_name_buf = xstrdup(branch_name);
	}

	سر_short_sha1 = repo_لبھو_unique_abbrev(the_repository,
						  &سر_commit->object.oid,
						  DEFAULT_ABBREV);
	strbuf_addf(&پیغام, "%s: %s ", branch_name, سر_short_sha1);
	pp_commit_easy(CMIT_FMT_ONELINE, سر_commit, &پیغام);

	strbuf_addf(&commit_درخت_label, "index on %s\n", پیغام.buf);
	commit_list_داخل_کرو(سر_commit, &parents);
	إذا (write_اشاریہ_as_درخت(&info->i_درخت, the_repository->اشاریہ,
				repo_get_اشاریہ_file(the_repository), 0, عدم) ||
	    commit_درخت(commit_درخت_label.buf, commit_درخت_label.len,
			&info->i_درخت, parents, &info->i_commit, عدم, عدم)) {
		إذا (!quiet)
			fprintf_ln(stderr, _("Cannot save the current "
					     "index state"));
		ret = -1;
		اذهب پورا;
	}

	commit_list_free(parents);
	parents = عدم;

	إذا (include_untracked) {
		إذا (save_untracked_files(info, &پیغام, &untracked_files)) {
			إذا (!quiet)
				fprintf_ln(stderr, _("Cannot save "
						     "the untracked files"));
			ret = -1;
			اذهب پورا;
		}
		untracked_commit_option = 1;
	}
	إذا (patch_منوال) {
		ret = stash_patch(info, ps, patch, quiet, interactive_opts);
		إذا (ret < 0) {
			إذا (!quiet)
				fprintf_ln(stderr, _("Cannot save the current "
						     "worktree state"));
			اذهب پورا;
		} وإلا إذا (ret > 0) {
			اذهب پورا;
		}
	} وإلا إذا (only_staged) {
		ret = stash_staged(info, patch, quiet);
		إذا (ret < 0) {
			إذا (!quiet)
				fprintf_ln(stderr, _("Cannot save the current "
						     "staged state"));
			اذهب پورا;
		} وإلا إذا (ret > 0) {
			اذهب پورا;
		}
	} وإلا {
		إذا (stash_working_درخت(info, ps)) {
			إذا (!quiet)
				fprintf_ln(stderr, _("Cannot save the current "
						     "worktree state"));
			ret = -1;
			اذهب پورا;
		}
	}

	إذا (!stash_پیغام_buf->len)
		strbuf_addf(stash_پیغام_buf, "WIP on %s", پیغام.buf);
	وإلا
		strbuf_insertf(stash_پیغام_buf, 0, "On %s: ", branch_name);

	إذا (untracked_commit_option)
		commit_list_داخل_کرو(lookup_commit(the_repository,
						 &info->u_commit),
				   &parents);
	commit_list_داخل_کرو(lookup_commit(the_repository, &info->i_commit),
			   &parents);
	commit_list_داخل_کرو(سر_commit, &parents);

	إذا (commit_درخت(stash_پیغام_buf->buf, stash_پیغام_buf->len, &info->w_درخت,
			parents, &info->w_commit, عدم, عدم)) {
		إذا (!quiet)
			fprintf_ln(stderr, _("Cannot record "
					     "working tree state"));
		ret = -1;
		اذهب پورا;
	}

پورا:
	strbuf_release(&commit_درخت_label);
	strbuf_release(&پیغام);
	strbuf_release(&untracked_files);
	commit_list_free(parents);
	حرر(branch_name_buf);
	أرجع ret;
}

ساكن صحيح create_stash(صحيح argc, ثابت حرف **argv, ثابت حرف *اگیتر بن_ورتیا,
			بنية repository *repo بن_ورتیا)
{
	صحيح ret;
	بنية strbuf stash_پیغام_buf = STRBUF_INIT;
	بنية stash_info info = STASH_INFO_INIT;
	بنية pathspec ps;

	/* Starting with argv[1], since argv[0] is "create" */
	strbuf_join_argv(&stash_پیغام_buf, argc - 1, ++argv, ' ');

	memset(&ps, 0, حجم(ps));
	إذا (!check_changes_tracked_files(&ps))
		أرجع 0;

	ret = do_create_stash(&ps, &stash_پیغام_buf, 0, 0, عدم, 0, &info,
			      عدم, 0);
	إذا (!ret)
		printf_ln("%s", oid_منزل_hex(&info.w_commit));

	free_stash_info(&info);
	strbuf_release(&stash_پیغام_buf);
	أرجع ret;
}

ساكن صحيح do_ڈالو_stash(ثابت بنية pathspec *ps, ثابت حرف *stash_پیغام, صحيح quiet,
			 صحيح keep_اشاریہ, صحيح patch_منوال,
			 بنية interactive_options *interactive_opts,
			 صحيح include_untracked, صحيح only_staged)
{
	صحيح ret = 0;
	بنية stash_info info = STASH_INFO_INIT;
	بنية strbuf patch = STRBUF_INIT;
	بنية strbuf stash_پیغام_buf = STRBUF_INIT;
	بنية strbuf untracked_files = STRBUF_INIT;
	بنية strbuf باہرلا = STRBUF_INIT;

	إذا (patch_منوال && keep_اشاریہ == -1)
		keep_اشاریہ = 1;

	إذا (patch_منوال && include_untracked) {
		fprintf_ln(stderr, _("Can't use --patch and --include-untracked"
				     " or --all at the same time"));
		ret = -1;
		اذهب پورا;
	}

	/* --patch overrides --staged */
	إذا (patch_منوال)
		only_staged = 0;

	إذا (only_staged && include_untracked) {
		fprintf_ln(stderr, _("Can't use --staged and --include-untracked"
				     " or --all at the same time"));
		ret = -1;
		اذهب پورا;
	}

	repo_read_اشاریہ_preload(the_repository, عدم, 0);
	إذا (!include_untracked && ps->nr) {
		حرف *ps_matched = xcalloc(ps->nr, 1);

		إذا (pathspec_needs_expanded_اشاریہ(the_repository->اشاریہ, ps))
			ensure_بھریا_اشاریہ(the_repository->اشاریہ);
		لكل (ناپ_t i = 0; i < the_repository->اشاریہ->cache_nr; i++)
			ce_رستہ_میل(the_repository->اشاریہ, the_repository->اشاریہ->cache[i], ps,
				      ps_matched);

		إذا (report_رستہ_error(ps_matched, ps)) {
			fprintf_ln(stderr, _("Did you forget to 'git add'?"));
			ret = -1;
			حرر(ps_matched);
			اذهب پورا;
		}
		حرر(ps_matched);
	}

	إذا (repo_refresh_and_write_اشاریہ(the_repository, REFRESH_QUIET, 0, 0,
					 عدم, عدم, عدم)) {
		ret = error(_("could not write index"));
		اذهب پورا;
	}

	إذا (!check_changes(ps, include_untracked, &untracked_files)) {
		إذا (!quiet)
			printf_ln(_("No local changes to save"));
		اذهب پورا;
	}

	إذا (!refs_reflog_exists(get_main_ref_store(the_repository), ref_stash) && do_clear_stash()) {
		ret = -1;
		إذا (!quiet)
			fprintf_ln(stderr, _("Cannot initialize stash"));
		اذهب پورا;
	}

	إذا (stash_پیغام)
		strbuf_addstr(&stash_پیغام_buf, stash_پیغام);
	إذا (do_create_stash(ps, &stash_پیغام_buf, include_untracked, patch_منوال,
			    interactive_opts, only_staged, &info, &patch, quiet)) {
		ret = -1;
		اذهب پورا;
	}

	إذا (do_store_stash(&info.w_commit, stash_پیغام_buf.buf, 1)) {
		ret = -1;
		إذا (!quiet)
			fprintf_ln(stderr, _("Cannot save the current status"));
		اذهب پورا;
	}

	إذا (!quiet)
		printf_ln(_("Saved working directory and index state %s"),
			  stash_پیغام_buf.buf);

	إذا (!(patch_منوال || only_staged)) {
		إذا (include_untracked && !ps->nr) {
			بنية بچہ_process cp = بچہ_PROCESS_INIT;

			cp.git_cmd = 1;
			إذا (startup_info->original_cwd) {
				cp.dir = startup_info->original_cwd;
				strvec_pushf(&cp.env, "%s=%s",
					     GIT_WORK_درخت_ENVIRONMENT,
					     the_repository->worktree);
			}
			strvec_pushl(&cp.دلیلاں, "clean", "--force",
				     "--quiet", "-d", ":/", عدم);
			إذا (include_untracked == INCLUDE_ALL_FILES)
				strvec_ڈالو(&cp.دلیلاں, "-x");
			إذا (لگاتار_command(&cp)) {
				ret = -1;
				اذهب پورا;
			}
		}
		discard_اشاریہ(the_repository->اشاریہ);
		إذا (ps->nr) {
			بنية بچہ_process cp_جوڑو = بچہ_PROCESS_INIT;
			بنية بچہ_process cp_diff = بچہ_PROCESS_INIT;
			بنية بچہ_process cp_apply = بچہ_PROCESS_INIT;

			cp_جوڑو.git_cmd = 1;
			strvec_ڈالو(&cp_جوڑو.دلیلاں, "add");
			إذا (!include_untracked)
				strvec_ڈالو(&cp_جوڑو.دلیلاں, "-u");
			إذا (include_untracked == INCLUDE_ALL_FILES)
				strvec_ڈالو(&cp_جوڑو.دلیلاں, "--force");
			strvec_ڈالو(&cp_جوڑو.دلیلاں, "--");
			جوڑو_pathspecs(&cp_جوڑو.دلیلاں, ps);
			إذا (لگاتار_command(&cp_جوڑو)) {
				ret = -1;
				اذهب پورا;
			}

			cp_diff.git_cmd = 1;
			strvec_pushl(&cp_diff.دلیلاں, "diff-index", "-p",
				     "--no-color",
				     "--cached", "--binary", "HEAD", "--",
				     عدم);
			جوڑو_pathspecs(&cp_diff.دلیلاں, ps);
			إذا (pipe_command(&cp_diff, عدم, 0, &باہرلا, 0, عدم, 0)) {
				ret = -1;
				اذهب پورا;
			}

			cp_apply.git_cmd = 1;
			strvec_pushl(&cp_apply.دلیلاں, "apply", "--index",
				     "-R", عدم);
			إذا (pipe_command(&cp_apply, باہرلا.buf, باہرلا.len, عدم, 0,
					 عدم, 0)) {
				ret = -1;
				اذهب پورا;
			}
		} وإلا {
			بنية بچہ_process cp = بچہ_PROCESS_INIT;
			cp.git_cmd = 1;
			/* BUG: this nukes untracked files in the way */
			strvec_pushl(&cp.دلیلاں, "reset", "--hard", "-q",
				     "--no-recurse-submodules", عدم);
			إذا (لگاتار_command(&cp)) {
				ret = -1;
				اذهب پورا;
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
		إذا (keep_اشاریہ == 1 && !is_null_oid(&info.i_درخت) &&
		    !is_خالی_تھاواں_درخت_oid(&info.i_درخت, the_repository->hash_algo)) {
			بنية بچہ_process cp = بچہ_PROCESS_INIT;

			cp.git_cmd = 1;
			strvec_pushl(&cp.دلیلاں, "checkout", "--no-overlay",
				     oid_منزل_hex(&info.i_درخت), "--", عدم);
			إذا (!ps->nr)
				strvec_ڈالو(&cp.دلیلاں, ":/");
			وإلا
				جوڑو_pathspecs(&cp.دلیلاں, ps);
			إذا (لگاتار_command(&cp)) {
				ret = -1;
				اذهب پورا;
			}
		}
		اذهب پورا;
	} وإلا {
		بنية بچہ_process cp = بچہ_PROCESS_INIT;

		cp.git_cmd = 1;
		strvec_pushl(&cp.دلیلاں, "apply", "-R", عدم);

		إذا (pipe_command(&cp, patch.buf, patch.len, عدم, 0, عدم, 0)) {
			إذا (!quiet)
				fprintf_ln(stderr, _("Cannot remove "
						     "worktree changes"));
			ret = -1;
			اذهب پورا;
		}

		إذا (keep_اشاریہ < 1) {
			بنية بچہ_process cp = بچہ_PROCESS_INIT;

			cp.git_cmd = 1;
			strvec_pushl(&cp.دلیلاں, "reset", "-q", "--refresh", "--",
				     عدم);
			جوڑو_pathspecs(&cp.دلیلاں, ps);
			إذا (لگاتار_command(&cp)) {
				ret = -1;
				اذهب پورا;
			}
		}
		اذهب پورا;
	}

پورا:
	strbuf_release(&patch);
	strbuf_release(&باہرلا);
	free_stash_info(&info);
	strbuf_release(&stash_پیغام_buf);
	strbuf_release(&untracked_files);
	أرجع ret;
}

ساكن صحيح ڈالو_stash(صحيح argc, ثابت حرف **argv, ثابت حرف *اگیتر,
		      صحيح ڈالو_assumed)
{
	صحيح force_assume = 0;
	صحيح keep_اشاریہ = -1;
	صحيح only_staged = 0;
	صحيح patch_منوال = 0;
	صحيح include_untracked = 0;
	صحيح quiet = 0;
	صحيح pathspec_file_nul = 0;
	ثابت حرف *stash_پیغام = عدم;
	حرف *pathspec_from_file = عدم;
	بنية pathspec ps;
	بنية interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	بنية option options[] = {
		OPT_BOOL('k', "keep-index", &keep_اشاریہ,
			 N_("keep index")),
		OPT_BOOL('S', "staged", &only_staged,
			 N_("stash staged changes only")),
		OPT_BOOL('p', "patch", &patch_منوال,
			 N_("stash in patch mode")),
		OPT_BOOL(0, "auto-advance", &interactive_opts.auto_advance,
			 N_("auto advance to the next file when selecting hunks interactively")),
		OPT_DIFF_UNIFIED(&interactive_opts.context),
		OPT_DIFF_INTERHUNK_CONTEXT(&interactive_opts.interhunkcontext),
		OPT__QUIET(&quiet, N_("quiet mode")),
		OPT_BOOL('u', "include-untracked", &include_untracked,
			 N_("include untracked files in stash")),
		OPT_سیٹ_INT('a', "all", &include_untracked,
			    N_("include ignore files"), INCLUDE_ALL_FILES),
		OPT_STRING('m', "message", &stash_پیغام, N_("message"),
			   N_("stash message")),
		OPT_PATHSPEC_FROM_FILE(&pathspec_from_file),
		OPT_PATHSPEC_FILE_NUL(&pathspec_file_nul),
		OPT_سرا()
	};
	صحيح ret;

	إذا (argc) {
		صحيح flags = PARSE_OPT_KEEP_DASHDASH;

		إذا (ڈالو_assumed)
			flags |= PARSE_OPT_STOP_AT_NON_OPTION;

		argc = parse_options(argc, argv, اگیتر, options,
				     ڈالو_assumed ? git_stash_usage :
				     git_stash_ڈالو_usage, flags);
		force_assume |= patch_منوال;
	}

	إذا (argc) {
		إذا (!strcmp(argv[0], "--")) {
			argc--;
			argv++;
		} وإلا إذا (ڈالو_assumed && !force_assume) {
			die("subcommand wasn't specified; 'push' can't be assumed due to unexpected token '%s'",
			    argv[0]);
		}
	}

	parse_pathspec(&ps, 0, PATHSPEC_PREFER_بھریا | PATHSPEC_اگیتر_ORIGIN,
		       اگیتر, argv);

	إذا (pathspec_from_file) {
		إذا (patch_منوال)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--patch");

		إذا (only_staged)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--staged");

		إذا (ps.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&ps, 0,
				    PATHSPEC_PREFER_بھریا | PATHSPEC_اگیتر_ORIGIN,
				    اگیتر, pathspec_from_file, pathspec_file_nul);
	} وإلا إذا (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	إذا (!patch_منوال) {
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

	ret = do_ڈالو_stash(&ps, stash_پیغام, quiet, keep_اشاریہ, patch_منوال,
			    &interactive_opts, include_untracked, only_staged);

	clear_pathspec(&ps);
	حرر(pathspec_from_file);
	أرجع ret;
}

ساكن صحيح ڈالو_stash_unassumed(صحيح argc, ثابت حرف **argv, ثابت حرف *اگیتر,
				بنية repository *repo بن_ورتیا)
{
	أرجع ڈالو_stash(argc, argv, اگیتر, 0);
}

ساكن صحيح save_stash(صحيح argc, ثابت حرف **argv, ثابت حرف *اگیتر,
		      بنية repository *repo بن_ورتیا)
{
	صحيح keep_اشاریہ = -1;
	صحيح only_staged = 0;
	صحيح patch_منوال = 0;
	صحيح include_untracked = 0;
	صحيح quiet = 0;
	صحيح ret = 0;
	ثابت حرف *stash_پیغام = عدم;
	بنية pathspec ps;
	بنية strbuf stash_پیغام_buf = STRBUF_INIT;
	بنية interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	بنية option options[] = {
		OPT_BOOL('k', "keep-index", &keep_اشاریہ,
			 N_("keep index")),
		OPT_BOOL('S', "staged", &only_staged,
			 N_("stash staged changes only")),
		OPT_BOOL('p', "patch", &patch_منوال,
			 N_("stash in patch mode")),
		OPT_BOOL(0, "auto-advance", &interactive_opts.auto_advance,
			 N_("auto advance to the next file when selecting hunks interactively")),
		OPT_DIFF_UNIFIED(&interactive_opts.context),
		OPT_DIFF_INTERHUNK_CONTEXT(&interactive_opts.interhunkcontext),
		OPT__QUIET(&quiet, N_("quiet mode")),
		OPT_BOOL('u', "include-untracked", &include_untracked,
			 N_("include untracked files in stash")),
		OPT_سیٹ_INT('a', "all", &include_untracked,
			    N_("include ignore files"), INCLUDE_ALL_FILES),
		OPT_STRING('m', "message", &stash_پیغام, "message",
			   N_("stash message")),
		OPT_سرا()
	};

	argc = parse_options(argc, argv, اگیتر, options,
			     git_stash_save_usage,
			     PARSE_OPT_KEEP_DASHDASH);

	إذا (argc)
		stash_پیغام = strbuf_join_argv(&stash_پیغام_buf, argc, argv, ' ');

	memset(&ps, 0, حجم(ps));

	إذا (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	إذا (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	إذا (!patch_منوال) {
		إذا (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--patch");
		إذا (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--patch");
		إذا (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--patch");
	}

	ret = do_ڈالو_stash(&ps, stash_پیغام, quiet, keep_اشاریہ,
			    patch_منوال, &interactive_opts, include_untracked,
			    only_staged);

	strbuf_release(&stash_پیغام_buf);
	أرجع ret;
}

ساكن صحيح write_commit_with_parents(بنية repository *r,
				     بنية object_id *باہرلا,
				     ثابت بنية object_id *oid,
				     بنية commit_list *parents)
{
	ناپ_t author_len, committer_len;
	بنية commit *this;
	ثابت حرف *orig_author, *orig_committer;
	حرف *author = عدم, *committer = عدم;
	ثابت حرف *بفر;
	بنانشانی لما bufsize;
	ثابت حرف *p;
	بنية strbuf پیغام = STRBUF_INIT;
	صحيح ret = 0;
	بنية ident_ونڈو id;

	this = lookup_commit_reference(r, oid);
	بفر = repo_get_commit_بفر(r, this, &bufsize);
	orig_author = لبھو_commit_سرخی(بفر, "author", &author_len);
	orig_committer = لبھو_commit_سرخی(بفر, "committer", &committer_len);

	إذا (!orig_author || !orig_committer) {
		ret = error(_("cannot parse commit %s"), oid_منزل_hex(oid));
		اذهب باہرلا;
	}

	إذا (ونڈو_ident_سطر(&id, orig_author, author_len) < 0 ||
	    ونڈو_ident_سطر(&id, orig_committer, committer_len) < 0) {
		ret = error(_("invalid author or committer for %s"), oid_منزل_hex(oid));
		اذهب باہرلا;
	}

	p = لڑی_کھوج(بفر, "\n\n");
	strbuf_addstr(&پیغام, "git stash: ");

	إذا (p)
		strbuf_جوڑو(&پیغام, p + 2, bufsize - (p + 2 - بفر));
	strbuf_complete_سطر(&پیغام);

	author = xmemdupz(orig_author, author_len);
	committer = xmemdupz(orig_committer, committer_len);

	إذا (commit_درخت_extended(پیغام.buf, پیغام.len,
				 r->hash_algo->خالی_تھاواں_درخت, parents,
				 باہرلا, author, committer,
				 عدم, عدم)) {
		ret = error(_("could not write commit"));
		اذهب باہرلا;
	}
باہرلا:
	strbuf_release(&پیغام);
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
	بنانشانی لما bufsize;
	بنية commit *this = عدم;
	بنية commit_list *اشیاء = عدم, *cur;
	حرف *پیغام = عدم;

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
		ناپ_t author_len, committer_len;
		ثابت حرف *p;
		ثابت حرف *expected = "git stash <git@stash> 1000684800 +0000";
		ثابت حرف *اگیتر = "git stash: ";
		بنية commit *stash;
		بنية درخت *درخت = repo_get_commit_درخت(r, this);

		إذا (!درخت ||
		    !oideq(&درخت->object.oid, r->hash_algo->خالی_تھاواں_درخت) ||
		    (this->parents &&
		     (!this->parents->اگلا || this->parents->اگلا->اگلا))) {
			res = error(_("%s is not a valid exported stash commit"),
					oid_منزل_hex(&this->object.oid));
			اذهب باہرلا;
		}

		بفر = repo_get_commit_بفر(r, this, &bufsize);

		إذا (!this->parents) {
			/*
			 * We don't have any parents.  Make sure this is our
			 * root commit.
			 */
			author = لبھو_commit_سرخی(بفر, "author", &author_len);
			committer = لبھو_commit_سرخی(بفر, "committer", &committer_len);

			إذا (!author || !committer) {
				error(_("cannot parse commit %s"), oid_منزل_hex(&this->object.oid));
				اذهب باہرلا;
			}

			إذا (author_len != لڑی_لمبائی(expected) ||
			    committer_len != لڑی_لمبائی(expected) ||
			    memcmp(author, expected, author_len) ||
			    memcmp(committer, expected, committer_len)) {
				res = error(_("found root commit %s with invalid data"), oid_منزل_hex(&this->object.oid));
				اذهب باہرلا;
			}
			اكسر;
		}

		p = لڑی_کھوج(بفر, "\n\n");
		إذا (!p) {
			res = error(_("cannot parse commit %s"), oid_منزل_hex(&this->object.oid));
			اذهب باہرلا;
		}

		p += 2;
		إذا (((ناپ_t)(bufsize - (p - بفر)) < لڑی_لمبائی(اگیتر)) ||
		    memcmp(اگیتر, p, لڑی_لمبائی(اگیتر))) {
			res = error(_("found stash commit %s without expected prefix"), oid_منزل_hex(&this->object.oid));
			اذهب باہرلا;
		}

		stash = this->parents->اگلا->شے;

		إذا (repo_parse_commit(r, this->parents->شے) ||
		    repo_parse_commit(r, stash)) {
			res = error(_("cannot parse parents of commit: %s"),
					oid_منزل_hex(&this->object.oid));
			اذهب باہرلا;
		}

		إذا (check_stash_topology(r, stash)) {
			res = error(_("%s does not look like a stash commit"),
					oid_منزل_hex(&stash->object.oid));
			اذهب باہرلا;
		}

		repo_unuse_commit_بفر(r, this, بفر);
		بفر = عدم;
		اشیاء = commit_list_داخل_کرو(stash, &اشیاء);
		this = this->parents->شے;
	}

	/*
	 * Now, walk each entry, adding it to the stash as a normal stash
	 * commit.
	 */
	لكل (cur = اشیاء; cur; cur = cur->اگلا) {
		ثابت حرف *p;
		بنية object_id *oid;

		this = cur->شے;
		oid = &this->object.oid;
		بفر = repo_get_commit_بفر(r, this, &bufsize);
		إذا (!بفر) {
			res = error(_("cannot read commit buffer for %s"), oid_منزل_hex(oid));
			اذهب باہرلا;
		}

		p = لڑی_کھوج(بفر, "\n\n");
		إذا (!p) {
			res = error(_("cannot parse commit %s"), oid_منزل_hex(oid));
			اذهب باہرلا;
		}

		p += 2;
		پیغام = xmemdupz(p, bufsize - (p - بفر));
		repo_unuse_commit_بفر(r, this, بفر);
		بفر = عدم;

		إذا (do_store_stash(oid, پیغام, 1)) {
			res = error(_("cannot save the stash for %s"), oid_منزل_hex(oid));
			اذهب باہرلا;
		}
		FREE_AND_NULL(پیغام);
	}
باہرلا:
	إذا (this && بفر)
		repo_unuse_commit_بفر(r, this, بفر);
	commit_list_free(اشیاء);
	حرر(پیغام);

	أرجع res;
}

ساكن صحيح import_stash(صحيح argc, ثابت حرف **argv, ثابت حرف *اگیتر,
			بنية repository *repo)
{
	بنية option options[] = {
		OPT_سرا()
	};

	argc = parse_options(argc, argv, اگیتر, options,
			     git_stash_import_usage,
			     PARSE_OPT_KEEP_DASHDASH);

	إذا (argc != 1)
		usage_پیغام_opt("a revision is required", git_stash_import_usage, options);

	أرجع do_import_stash(repo, argv[0]);
}

بنية stash_اندراج_انکڑے {
	بنية repository *r;
	بنية commit_list **اشیاء;
	ناپ_t گنتی;
};

ساكن صحيح collect_stash_entries(ثابت حرف *refname بن_ورتیا,
				 بنية object_id *old_oid بن_ورتیا,
				 بنية object_id *new_oid,
				 ثابت حرف *committer بن_ورتیا,
				 timestamp_t timestamp بن_ورتیا,
				 صحيح tz بن_ورتیا, ثابت حرف *پیغام بن_ورتیا,
				 فراغ *cb_انکڑے)
{
	بنية stash_اندراج_انکڑے *انکڑے = cb_انکڑے;
	بنية commit *stash;

	انکڑے->گنتی++;
	stash = lookup_commit_reference(انکڑے->r, new_oid);
	إذا (!stash || check_stash_topology(انکڑے->r, stash)) {
		أرجع error(_("%s does not look like a stash commit"),
			     oid_منزل_hex(new_oid));
	}
	انکڑے->اشیاء = commit_list_append(stash, انکڑے->اشیاء);
	أرجع 0;
}

ساكن صحيح do_export_stash(بنية repository *r,
			   ثابت حرف *ref,
			   صحيح argc,
			   ثابت حرف **argv)
{
	بنية object_id ادھار;
	بنية commit *prev;
	بنية commit_list *اشیاء = عدم, **iter = &اشیاء, *cur;
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
	إذا (commit_درخت_extended("", 0, r->hash_algo->خالی_تھاواں_درخت, عدم,
				 &ادھار, author, committer, عدم, عدم))
		أرجع error(_("unable to write base commit"));

	prev = lookup_commit_reference(r, &ادھار);

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
				اذهب باہرلا;
			}

			stash = lookup_commit_reference(r, &oid);
			إذا (!stash || check_stash_topology(r, stash)) {
				res = error(_("%s does not look like a stash commit"),
						revision.buf);
				اذهب باہرلا;
			}
			iter = commit_list_append(stash, iter);
		}
	} وإلا {
		/*
		 * Walk the reflog, finding each stash entry, and load data into the
		 * array.
		 */
		بنية stash_اندراج_انکڑے cb_انکڑے = {
			.r = r, .اشیاء = iter,
		};
		إذا (refs_for_each_reflog_ent_الٹ(get_main_ref_store(r),
						     "refs/stash",
						     collect_stash_entries,
						     &cb_انکڑے) && cb_انکڑے.گنتی)
			اذهب باہرلا;
	}

	/*
	 * Now, create a set of commits identical to the regular stash commits,
	 * but where their first parents form a chain to our original empty
	 * base commit.
	 */
	اشیاء = commit_list_الٹ(اشیاء);
	لكل (cur = اشیاء; cur; cur = cur->اگلا) {
		بنية commit_list *parents = عدم;
		بنية commit_list **اگلا = &parents;
		بنية object_id باہرلا;
		بنية commit *stash = cur->شے;

		اگلا = commit_list_append(prev, اگلا);
		اگلا = commit_list_append(stash, اگلا);
		res = write_commit_with_parents(r, &باہرلا, &stash->object.oid, parents);
		commit_list_free(parents);
		إذا (res)
			اذهب باہرلا;
		prev = lookup_commit_reference(r, &باہرلا);
	}
	إذا (ref)
		refs_update_ref(get_main_ref_store(r), عدم, ref,
				&prev->object.oid, عدم, 0, UPDATE_REFS_DIE_ON_ERR);
	وإلا
		puts(oid_منزل_hex(&prev->object.oid));
باہرلا:
	strbuf_release(&revision);
	commit_list_free(اشیاء);

	أرجع res;
}

تعداد export_action {
	ACTION_NONE,
	ACTION_PRINT,
	ACTION_منزل_REF,
};

ساكن صحيح export_stash(صحيح argc,
			ثابت حرف **argv,
			ثابت حرف *اگیتر,
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
		OPT_سرا()
	};

	argc = parse_options(argc, argv, اگیتر, options,
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
	      ثابت حرف *اگیتر,
	      بنية repository *repo)
{
	pid_t pid = getpid();
	ثابت حرف *اشاریہ_file;
	بنية strvec دلیلاں = STRVEC_INIT;
	parse_opt_subcommand_fn *fn = عدم;
	بنية option options[] = {
		OPT_SUBCOMMAND("apply", &fn, apply_stash),
		OPT_SUBCOMMAND("clear", &fn, clear_stash),
		OPT_SUBCOMMAND("drop", &fn, drop_stash),
		OPT_SUBCOMMAND("pop", &fn, نکالو_stash),
		OPT_SUBCOMMAND("branch", &fn, branch_stash),
		OPT_SUBCOMMAND("list", &fn, list_stash),
		OPT_SUBCOMMAND("show", &fn, show_stash),
		OPT_SUBCOMMAND("store", &fn, store_stash),
		OPT_SUBCOMMAND("create", &fn, create_stash),
		OPT_SUBCOMMAND("push", &fn, ڈالو_stash_unassumed),
		OPT_SUBCOMMAND("export", &fn, export_stash),
		OPT_SUBCOMMAND("import", &fn, import_stash),
		OPT_SUBCOMMAND_F("save", &fn, save_stash, عدم, PARSE_OPT_NOCOMPLETE),
		OPT_سرا()
	};
	ثابت حرف **دلیلاں_copy;
	صحيح ret;

	repo_config(the_repository, git_stash_config, عدم);

	argc = parse_options(argc, argv, اگیتر, options, git_stash_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL |
			     PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_بھریا_اشاریہ = 0;

	اشاریہ_file = repo_get_اشاریہ_file(the_repository);
	strbuf_addf(&stash_اشاریہ_رستہ, "%s.stash.%" PRIuMAX, اشاریہ_file,
		    (uintmax_t)pid);

	إذا (fn) {
		ret = fn(argc, argv, اگیتر, repo);

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
		أرجع !!ڈالو_stash_unassumed(0, عدم, اگیتر, repo);
	}

	/* Assume 'stash push' */
	strvec_ڈالو(&دلیلاں, "push");
	strvec_pushv(&دلیلاں, argv);

	/*
	 * `push_stash()` ends up modifying the array, which causes memory
	 * leaks if we didn't copy the array here.
	 */
	DUP_ARRAY(دلیلاں_copy, دلیلاں.v, دلیلاں.nr);

	ret = !!ڈالو_stash(دلیلاں.nr, دلیلاں_copy, اگیتر, 1);

	strvec_clear(&دلیلاں);
	حرر(دلیلاں_copy);
	أرجع ret;
}
