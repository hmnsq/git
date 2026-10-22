#define USE_THE_REPOSITORY_VARIABLE

#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "abspath.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "hash.h"
#ଅନ୍ତର୍ଭାଵ "hex.h"
#ଅନ୍ତର୍ଭାଵ "object-name.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "refs.h"
#ଅନ୍ତର୍ଭାଵ "stash.h"
#ଅନ୍ତର୍ଭାଵ "lockfile.h"
#ଅନ୍ତର୍ଭାଵ "cache-tree.h"
#ଅନ୍ତର୍ଭାଵ "unpack-trees.h"
#ଅନ୍ତର୍ଭାଵ "merge-ort-wrappers.h"
#ଅନ୍ତର୍ଭାଵ "strvec.h"
#ଅନ୍ତର୍ଭାଵ "run-command.h"
#ଅନ୍ତର୍ଭାଵ "dir.h"
#ଅନ୍ତର୍ଭାଵ "entry.h"
#ଅନ୍ତର୍ଭାଵ "preload-index.h"
#ଅନ୍ତର୍ଭାଵ "read-cache.h"
#ଅନ୍ତର୍ଭାଵ "repository.h"
#ଅନ୍ତର୍ଭାଵ "rerere.h"
#ଅନ୍ତର୍ଭାଵ "revision.h"
#ଅନ୍ତର୍ଭାଵ "setup.h"
#ଅନ୍ତର୍ଭାଵ "sparse-index.h"
#ଅନ୍ତର୍ଭାଵ "log-tree.h"
#ଅନ୍ତର୍ଭାଵ "diffcore.h"
#ଅନ୍ତର୍ଭାଵ "reflog.h"
#ଅନ୍ତର୍ଭାଵ "reflog-walk.h"
#ଅନ୍ତର୍ଭାଵ "add-interactive.h"
#ଅନ୍ତର୍ଭାଵ "commit.h"

#define INCLUDE_ALL_FILES 2

#define BUILTIN_STASH_LIST_USAGE \
	N_("git stash list [<log-options>]")
#define BUILTIN_STASH_SHOW_USAGE \
	N_("git stash show [-u | --include-untracked | --only-untracked] [<diff-options>] [<stash>]")
#define BUILTIN_STASH_DROP_USAGE \
	N_("git stash drop [-q | --quiet] [<stash>]")
#define BUILTIN_STASH_କାଢ଼_USAGE \
	N_("git stash pop [--index] [-q | --quiet] [<stash>]")
#define BUILTIN_STASH_APPLY_USAGE \
	N_("git stash apply [--index] [-q | --quiet] [--label-ours=<label>] [--label-theirs=<label>] [--label-base=<label>] [<stash>]")
#define BUILTIN_STASH_BRANCH_USAGE \
	N_("git stash branch <branchname> [<stash>]")
#define BUILTIN_STASH_STORE_USAGE \
	N_("git stash store [(-m | --message) <message>] [-q | --quiet] <commit>")
#define BUILTIN_STASH_ଠେଲ_USAGE \
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

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ git_stash_usage[] = {
	BUILTIN_STASH_LIST_USAGE,
	BUILTIN_STASH_SHOW_USAGE,
	BUILTIN_STASH_DROP_USAGE,
	BUILTIN_STASH_କାଢ଼_USAGE,
	BUILTIN_STASH_APPLY_USAGE,
	BUILTIN_STASH_BRANCH_USAGE,
	BUILTIN_STASH_ଠେଲ_USAGE,
	BUILTIN_STASH_SAVE_USAGE,
	BUILTIN_STASH_CLEAR_USAGE,
	BUILTIN_STASH_CREATE_USAGE,
	BUILTIN_STASH_STORE_USAGE,
	BUILTIN_STASH_EXPORT_USAGE,
	BUILTIN_STASH_IMPORT_USAGE,
	ଖାଲି
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ git_stash_list_usage[] = {
	BUILTIN_STASH_LIST_USAGE,
	ଖାଲି
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ git_stash_show_usage[] = {
	BUILTIN_STASH_SHOW_USAGE,
	ଖାଲି
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ git_stash_drop_usage[] = {
	BUILTIN_STASH_DROP_USAGE,
	ଖାଲି
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ git_stash_କାଢ଼_usage[] = {
	BUILTIN_STASH_କାଢ଼_USAGE,
	ଖାଲି
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ git_stash_apply_usage[] = {
	BUILTIN_STASH_APPLY_USAGE,
	ଖାଲି
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ git_stash_branch_usage[] = {
	BUILTIN_STASH_BRANCH_USAGE,
	ଖାଲି
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ git_stash_clear_usage[] = {
	BUILTIN_STASH_CLEAR_USAGE,
	ଖାଲି
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ git_stash_store_usage[] = {
	BUILTIN_STASH_STORE_USAGE,
	ଖାଲି
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ git_stash_ଠେଲ_usage[] = {
	BUILTIN_STASH_ଠେଲ_USAGE,
	ଖାଲି
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ git_stash_save_usage[] = {
	BUILTIN_STASH_SAVE_USAGE,
	ଖାଲି
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ git_stash_export_usage[] = {
	BUILTIN_STASH_EXPORT_USAGE,
	ଖାଲି
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ git_stash_import_usage[] = {
	BUILTIN_STASH_IMPORT_USAGE,
	ଖାଲି
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର ref_stash[] = "refs/stash";
ସ୍ଥିର ସଂରଚନା strbuf stash_index_path = STRBUF_INIT;

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_stat = 1;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_patch;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_include_untracked;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା use_index;

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
ସଂରଚନା stash_info {
	ସଂରଚନା object_id w_commit;
	ସଂରଚନା object_id b_commit;
	ସଂରଚନା object_id i_commit;
	ସଂରଚନା object_id u_commit;
	ସଂରଚନା object_id c_commit;
	ସଂରଚନା object_id w_ବୃକ୍ଷ;
	ସଂରଚନା object_id b_ବୃକ୍ଷ;
	ସଂରଚନା object_id i_ବୃକ୍ଷ;
	ସଂରଚନା object_id u_ବୃକ୍ଷ;
	ସଂରଚନା strbuf revision;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା is_stash_ref;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା has_u;
};

#define STASH_INFO_INIT { \
	.revision = STRBUF_INIT, \
}

ସ୍ଥିର ଶୂନ୍ୟ free_stash_info(ସଂରଚନା stash_info *info)
{
	strbuf_release(&info->revision);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା check_stash_topology(ସଂରଚନା repository *r, ସଂରଚନା commit *stash)
{
	ସଂରଚନା commit *p1, *p2, *p3 = ଖାଲି;

	/* stash must have two or three parents */
	ଯଦି (!stash->parents || !stash->parents->next ||
			(stash->parents->next->next && stash->parents->next->next->next))
		ଫେରନ୍ତୁ -1;
	p1 = stash->parents->item;
	p2 = stash->parents->next->item;
	ଯଦି (stash->parents->next->next)
		p3 = stash->parents->next->next->item;
	ଯଦି (repo_parse_commit(r, p1) || repo_parse_commit(r, p2) ||
			(p3 && repo_parse_commit(r, p3)))
		ଫେରନ୍ତୁ -1;
	/* p2 must have a single parent, p3 must have no parents */
	ଯଦି (!p2->parents || p2->parents->next || (p3 && p3->parents))
		ଫେରନ୍ତୁ -1;
	ଯଦି (repo_parse_commit(r, p2->parents->item))
		ଫେରନ୍ତୁ -1;
	/* p2^1 must equal p1 */
	ଯଦି (!oideq(&p1->object.oid, &p2->parents->item->object.oid))
		ଫେରନ୍ତୁ -1;

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ assert_stash_like(ସଂରଚନା stash_info *info, ଅଚଳ ଅକ୍ଷର *revision)
{
	ଯଦି (get_oidf(&info->b_commit, "%s^1", revision) ||
	    get_oidf(&info->w_ବୃକ୍ଷ, "%s:", revision) ||
	    get_oidf(&info->b_ବୃକ୍ଷ, "%s^1:", revision) ||
	    get_oidf(&info->i_ବୃକ୍ଷ, "%s^2:", revision))
		die(_("'%s' is not a stash-like commit"), revision);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା parse_stash_revision(ସଂରଚନା strbuf *revision, ଅଚଳ ଅକ୍ଷର *commit, ପୂର୍ଣ୍ଣସଂଖ୍ୟା quiet)
{
	strbuf_reset(revision);
	ଯଦି (!commit) {
		ଯଦି (!refs_ref_exists(get_main_ref_store(the_repository), ref_stash)) {
			ଯଦି (!quiet)
				fprintf_ln(stderr, _("No stash entries found."));
			ଫେରନ୍ତୁ -1;
		}

		strbuf_addf(revision, "%s@{0}", ref_stash);
	} ଅନ୍ୟଥା ଯଦି (strspn(commit, "0123456789") == strlen(commit)) {
		strbuf_addf(revision, "%s@{%s}", ref_stash, commit);
	} ଅନ୍ୟଥା {
		strbuf_addstr(revision, commit);
	}
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_stash_info(ସଂରଚନା stash_info *info, ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
	ଅକ୍ଷର *end_of_rev;
	ଅକ୍ଷର *expanded_ref;
	ଅଚଳ ଅକ୍ଷର *revision;
	ଅଚଳ ଅକ୍ଷର *commit = ଖାଲି;
	ସଂରଚନା object_id dummy;
	ସଂରଚନା strbuf symbolic = STRBUF_INIT;

	ଯଦି (argc > 1) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
		ସଂରଚନା strbuf refs_ସନ୍ଦେଶ = STRBUF_INIT;

		ପାଇଁ (i = 0; i < argc; i++)
			strbuf_addf(&refs_ସନ୍ଦେଶ, " '%s'", argv[i]);

		fprintf_ln(stderr, _("Too many revisions specified:%s"),
			   refs_ସନ୍ଦେଶ.buf);
		strbuf_release(&refs_ସନ୍ଦେଶ);

		ଫେରନ୍ତୁ -1;
	}

	ଯଦି (argc == 1)
		commit = argv[0];

	strbuf_init(&info->revision, 0);
	ଯଦି (parse_stash_revision(&info->revision, commit, 0)) {
		ଫେରନ୍ତୁ -1;
	}

	revision = info->revision.buf;

	ଯଦି (repo_get_oid(the_repository, revision, &info->w_commit))
		ଫେରନ୍ତୁ error(_("%s is not a valid reference"), revision);

	assert_stash_like(info, revision);

	info->has_u = !get_oidf(&info->u_ବୃକ୍ଷ, "%s^3:", revision);

	end_of_rev = strchrnul(revision, '@');
	strbuf_add(&symbolic, revision, end_of_rev - revision);

	ret = repo_dwim_ref(the_repository, symbolic.buf, symbolic.len,
			    &dummy, &expanded_ref, 0);
	strbuf_release(&symbolic);
	ବାଛ (ret) {
	ସ୍ଥିତି 0: /* Not found, but valid ref */
		info->is_stash_ref = 0;
		ଭାଙ୍ଗ;
	ସ୍ଥିତି 1:
		info->is_stash_ref = !strcmp(expanded_ref, ref_stash);
		ଭାଙ୍ଗ;
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ: /* Invalid or ambiguous */
		ଭାଙ୍ଗ;
	}

	ମୁକ୍ତ(expanded_ref);
	ଫେରନ୍ତୁ !(ret == 0 || ret == 1);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା do_clear_stash(ଶୂନ୍ୟ)
{
	ସଂରଚନା object_id obj;
	ଯଦି (repo_get_oid(the_repository, ref_stash, &obj))
		ଫେରନ୍ତୁ 0;

	ଫେରନ୍ତୁ refs_delete_ref(get_main_ref_store(the_repository), ଖାଲି,
			       ref_stash, &obj, 0);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା clear_stash(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
		       ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା option options[] = {
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_clear_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	ଯଦି (argc)
		ଫେରନ୍ତୁ error(_("git stash clear with arguments is "
			       "unimplemented"));

	ଫେରନ୍ତୁ do_clear_stash();
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା reset_ବୃକ୍ଷ(ସଂରଚନା object_id *i_ବୃକ୍ଷ, ପୂର୍ଣ୍ଣସଂଖ୍ୟା update, ପୂର୍ଣ୍ଣସଂଖ୍ୟା reset)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା nr_trees = 1;
	ସଂରଚନା unpack_trees_options opts;
	ସଂରଚନା ବୃକ୍ଷ_desc t[MAX_UNPACK_TREES];
	ସଂରଚନା ବୃକ୍ଷ *ବୃକ୍ଷ;
	ସଂରଚନା lock_file lock_file = LOCK_INIT;

	repo_read_index_preload(the_repository, ଖାଲି, 0);
	ଯଦି (refresh_index(the_repository->index, REFRESH_QUIET, ଖାଲି, ଖାଲି, ଖାଲି))
		ଫେରନ୍ତୁ -1;

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);

	memset(&opts, 0, ଆକାର(opts));

	ବୃକ୍ଷ = repo_parse_ବୃକ୍ଷ_indirect(the_repository, i_ବୃକ୍ଷ);
	ଯଦି (repo_parse_ବୃକ୍ଷ(the_repository, ବୃକ୍ଷ))
		ଫେରନ୍ତୁ -1;

	init_ବୃକ୍ଷ_desc(t, &ବୃକ୍ଷ->object.oid, ବୃକ୍ଷ->buffer, ବୃକ୍ଷ->size);

	opts.head_idx = 1;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;
	opts.merge = 1;
	opts.reset = reset ? UNPACK_RESET_PROTECT_UNTRACKED : 0;
	opts.update = update;
	ଯଦି (update)
		opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
	opts.fn = oneway_merge;

	ଯଦି (unpack_trees(nr_trees, t, &opts))
		ଫେରନ୍ତୁ -1;

	ଯଦି (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
		ଫେରନ୍ତୁ error(_("unable to write new index file"));

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା create_index_from_ବୃକ୍ଷ(ଅଚଳ ସଂରଚନା object_id *ବୃକ୍ଷ_id,
				  ଅଚଳ ଅକ୍ଷର *index_path)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା nr_trees = 1;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;
	ସଂରଚନା unpack_trees_options opts;
	ସଂରଚନା ବୃକ୍ଷ_desc t[MAX_UNPACK_TREES];
	ସଂରଚନା ବୃକ୍ଷ *ବୃକ୍ଷ;
	ସଂରଚନା index_state dst_istate = INDEX_STATE_INIT(the_repository);
	ସଂରଚନା lock_file lock_file = LOCK_INIT;

	repo_read_index_preload(the_repository, ଖାଲି, 0);
	refresh_index(the_repository->index, REFRESH_QUIET, ଖାଲି, ଖାଲି, ଖାଲି);

	hold_lock_file_for_update(&lock_file, index_path, LOCK_DIE_ON_ERROR);

	memset(&opts, 0, ଆକାର(opts));

	ବୃକ୍ଷ = repo_parse_ବୃକ୍ଷ_indirect(the_repository, ବୃକ୍ଷ_id);
	ଯଦି (!ବୃକ୍ଷ || repo_parse_ବୃକ୍ଷ(the_repository, ବୃକ୍ଷ)) {
		ret = -1;
		ଯାଅ done;
	}

	init_ବୃକ୍ଷ_desc(t, &ବୃକ୍ଷ->object.oid, ବୃକ୍ଷ->buffer, ବୃକ୍ଷ->size);

	opts.head_idx = 1;
	opts.src_index = the_repository->index;
	opts.dst_index = &dst_istate;
	opts.merge = 1;
	opts.reset = UNPACK_RESET_PROTECT_UNTRACKED;
	opts.fn = oneway_merge;

	ଯଦି (unpack_trees(nr_trees, t, &opts)) {
		ret = -1;
		ଯାଅ done;
	}

	ଯଦି (write_locked_index(&dst_istate, &lock_file, COMMIT_LOCK)) {
		ret = error(_("unable to write new index file"));
		ଯାଅ done;
	}

done:
	release_index(&dst_istate);
	ଯଦି (ret)
		rollback_lock_file(&lock_file);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା is_path_a_directory(ଅଚଳ ଅକ୍ଷର *path)
{
	/*
	 * This function differs from abspath.c:is_directory() in that
	 * here we use lstat() instead of stat(); we do not want to
	 * follow symbolic links here.
	 */
	ସଂରଚନା stat st;
	ଫେରନ୍ତୁ (!lstat(path, &st) && S_ISDIR(st.st_mode));
}

ସ୍ଥିର ଶୂନ୍ୟ add_diff_to_buf(ସଂରଚନା diff_queue_struct *q,
			    ସଂରଚନା diff_options *options UNUSED,
			    ଶୂନ୍ୟ *data)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	ପାଇଁ (i = 0; i < q->nr; i++) {
		ଯଦି (is_path_a_directory(q->queue[i]->one->path))
			ଜାରି;

		strbuf_addstr(data, q->queue[i]->one->path);

		/* NUL-terminate: will be fed to update-index -z */
		strbuf_addch(data, '\0');
	}
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା restore_untracked(ସଂରଚନା object_id *u_ବୃକ୍ଷ)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା res;
	ସଂରଚନା child_process cp = CHILD_PROCESS_INIT;

	/*
	 * We need to run restore files from a given index, but without
	 * affecting the current index, so we use GIT_INDEX_FILE with
	 * run_command to fork processes that will not interfere.
	 */
	cp.git_cmd = 1;
	strvec_ଠେଲ(&cp.args, "read-tree");
	strvec_ଠେଲ(&cp.args, oid_to_hex(u_ବୃକ୍ଷ));
	strvec_pushf(&cp.env, "GIT_INDEX_FILE=%s",
		     stash_index_path.buf);
	ଯଦି (run_command(&cp)) {
		remove_path(stash_index_path.buf);
		ଫେରନ୍ତୁ -1;
	}

	child_process_init(&cp);
	cp.git_cmd = 1;
	strvec_pushl(&cp.args, "checkout-index", "--all", ଖାଲି);
	strvec_pushf(&cp.env, "GIT_INDEX_FILE=%s",
		     stash_index_path.buf);

	res = run_command(&cp);
	remove_path(stash_index_path.buf);
	ଫେରନ୍ତୁ res;
}

ସ୍ଥିର ଶୂନ୍ୟ unstage_changes_unless_new(ସଂରଚନା object_id *orig_ବୃକ୍ଷ)
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

	ସଂରଚନା checkout state = CHECKOUT_INIT;
	ସଂରଚନା diff_options diff_opts;
	ସଂରଚନା lock_file lock = LOCK_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

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

	do_diff_cache(orig_ବୃକ୍ଷ, &diff_opts);
	diffcore_std(&diff_opts);

	/* Iterate over the paths that changed due to the merge... */
	ପାଇଁ (i = 0; i < diff_queued_diff.nr; i++) {
		ସଂରଚନା diff_filepair *p;
		ସଂରଚନା cache_entry *ce;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା pos;

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
		ଯଦି (pos >= 0 && ce_skip_worktree(the_repository->index->cache[pos])) {
			ସଂରଚନା stat st;

			ce = the_repository->index->cache[pos];
			ଯଦି (!lstat(ce->name, &st)) {
				/* Conflicting path present; relocate it */
				ସଂରଚନା strbuf new_path = STRBUF_INIT;
				ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd;

				strbuf_addf(&new_path,
					    "%s.stash.XXXXXX", ce->name);
				fd = xmkstemp(new_path.buf);
				close(fd);
				printf(_("WARNING: Untracked file in way of "
					 "tracked file!  Renaming\n "
					 "           %s -> %s\n"
					 "         to make room.\n"),
				       ce->name, new_path.buf);
				ଯଦି (rename(ce->name, new_path.buf))
					die("Failed to move %s to %s",
					    ce->name, new_path.buf);
				strbuf_release(&new_path);
			}
			checkout_entry(ce, &state, ଖାଲି, ଖାଲି);
			ce->ce_flags &= ~CE_SKIP_WORKTREE;
		}

		/*
		 * Step 3: "unstage" changes, as long as they are still tracked
		 */
		ଯଦି (p->one->oid_valid) {
			/*
			 * Path existed in orig_tree; restore index entry
			 * from that tree in order to "unstage" the changes.
			 */
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା option = ADD_CACHE_OK_TO_REPLACE;
			ଯଦି (pos < 0)
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
	ଯଦି (write_locked_index(the_repository->index, &lock,
			       COMMIT_LOCK | SKIP_IF_UNCHANGED))
		die(_("could not write index"));
}

ସ୍ଥିର ଗଣନା stash_apply_result do_apply_stash(ଅଚଳ ଅକ୍ଷର *prefix,
					      ସଂରଚନା stash_info *info,
					      ପୂର୍ଣ୍ଣସଂଖ୍ୟା index, ପୂର୍ଣ୍ଣସଂଖ୍ୟା quiet,
					      ଅଚଳ ଅକ୍ଷର *label_ours,
					      ଅଚଳ ଅକ୍ଷର *label_theirs,
					      ଅଚଳ ଅକ୍ଷର *label_base)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା clean, ret;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା has_index = index;
	ସଂରଚନା merge_options o;
	ସଂରଚନା object_id c_ବୃକ୍ଷ;
	ସଂରଚନା object_id index_ବୃକ୍ଷ;
	ସଂରଚନା ବୃକ୍ଷ *head, *merge, *merge_base;
	ସଂରଚନା lock_file lock = LOCK_INIT;

	repo_read_index_preload(the_repository, ଖାଲି, 0);
	ଯଦି (repo_refresh_and_write_index(the_repository, REFRESH_QUIET, 0, 0,
					 ଖାଲି, ଖାଲି, ଖାଲି))
		ଫେରନ୍ତୁ error(_("could not write index"));

	ଯଦି (write_index_as_ବୃକ୍ଷ(&c_ବୃକ୍ଷ, the_repository->index,
				repo_get_index_file(the_repository), 0, ଖାଲି))
		ଫେରନ୍ତୁ error(_("cannot apply a stash in the middle of a merge"));

	init_ui_merge_options(&o, the_repository);

	ଯଦି (quiet)
		o.verbosity = 0;

	ଯଦି (index) {
		ଯଦି (oideq(&info->b_ବୃକ୍ଷ, &info->i_ବୃକ୍ଷ) ||
		    oideq(&c_ବୃକ୍ଷ, &info->i_ବୃକ୍ଷ)) {
			has_index = 0;
		} ଅନ୍ୟଥା {
			ସଂରଚନା merge_result result = { 0 };

			o.branch1 = "Current index";
			o.branch2 = "Stashed index changes";
			o.ancestor = "Stash base";

			head = lookup_ବୃକ୍ଷ(o.repo, &c_ବୃକ୍ଷ);
			merge = lookup_ବୃକ୍ଷ(o.repo, &info->i_ବୃକ୍ଷ);
			merge_base = lookup_ବୃକ୍ଷ(o.repo, &info->b_ବୃକ୍ଷ);

			merge_incore_nonrecursive(&o, merge_base, head, merge,
						  &result);

			ଯଦି (result.clean < 0) {
				merge_finalize(&o, &result);
				ଫେରନ୍ତୁ error(_("index merge failed"));
			} ଅନ୍ୟଥା ଯଦି (!result.clean) {
				merge_finalize(&o, &result);
				ଫେରନ୍ତୁ error(_("conflicts in index. "
					       "Try without --index."));
			} ଅନ୍ୟଥା {
				oidcpy(&index_ବୃକ୍ଷ, &result.ବୃକ୍ଷ->object.oid);
				merge_finalize(&o, &result);
			}
		}
	}

	o.branch1 = label_ours ? label_ours : "Updated upstream";
	o.branch2 = label_theirs ? label_theirs : "Stashed changes";
	o.ancestor = label_base ? label_base : "Stash base";

	ଯଦି (oideq(&info->b_ବୃକ୍ଷ, &c_ବୃକ୍ଷ))
		o.branch1 = "Version stash was based on";

	ଯଦି (o.verbosity >= 3)
		printf_ln(_("Merging %s with %s"), o.branch1, o.branch2);

	head = lookup_ବୃକ୍ଷ(o.repo, &c_ବୃକ୍ଷ);
	merge = lookup_ବୃକ୍ଷ(o.repo, &info->w_ବୃକ୍ଷ);
	merge_base = lookup_ବୃକ୍ଷ(o.repo, &info->b_ବୃକ୍ଷ);

	repo_hold_locked_index(o.repo, &lock, LOCK_DIE_ON_ERROR);
	clean = merge_ort_nonrecursive(&o, head, merge, merge_base);

	/*
	 * If 'clean' >= 0, reverse the value for 'ret' so 'ret' is 0 when the
	 * merge was clean, and 1 if the merge was unclean or a negative value
	 * if it encountered an error.
	 */
	ret = clean >= 0 ? !clean : clean;

	ଯଦି (ret < 0)
		rollback_lock_file(&lock);
	ଅନ୍ୟଥା ଯଦି (write_locked_index(o.repo->index, &lock,
				      COMMIT_LOCK | SKIP_IF_UNCHANGED))
		ret = error(_("could not write index"));

	ଯଦି (ret) {
		repo_rerere(the_repository, 0);

		ଯଦି (index)
			fprintf_ln(stderr, _("Index was not unstashed."));

		ଯାଅ restore_untracked;
	}

	ଯଦି (has_index) {
		ଯଦି (reset_ବୃକ୍ଷ(&index_ବୃକ୍ଷ, 0, 0))
			ret = -1;
	} ଅନ୍ୟଥା {
		unstage_changes_unless_new(&c_ବୃକ୍ଷ);
	}

restore_untracked:
	ଯଦି (info->has_u && restore_untracked(&info->u_ବୃକ୍ଷ))
		ret = error(_("could not restore untracked files from stash"));

	ଯଦି (!quiet) {
		ସଂରଚନା child_process cp = CHILD_PROCESS_INIT;

		/*
		 * Status is quite simple and could be replaced with calls to
		 * wt_status in the future, but it adds complexities which may
		 * require more tests.
		 */
		cp.git_cmd = 1;
		cp.dir = prefix;
		strvec_pushf(&cp.env, GIT_WORK_ବୃକ୍ଷ_ENVIRONMENT"=%s",
			     absolute_path(repo_get_work_ବୃକ୍ଷ(the_repository)));
		strvec_pushf(&cp.env, GIT_DIR_ENVIRONMENT"=%s",
			     absolute_path(repo_get_git_dir(the_repository)));
		strvec_ଠେଲ(&cp.args, "status");
		run_command(&cp);
	}

	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା apply_stash(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
		       ସଂରଚନା repository *repo UNUSED)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = -1;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା quiet = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = use_index;
	ଅଚଳ ଅକ୍ଷର *label_ours = ଖାଲି, *label_theirs = ଖାଲି, *label_base = ଖାଲି;
	ସଂରଚନା stash_info info = STASH_INFO_INIT;
	ସଂରଚନା option options[] = {
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

	ଯଦି (get_stash_info(&info, argc, argv))
		ଯାଅ cleanup;

	ret = do_apply_stash(prefix, &info, index, quiet,
			     label_ours, label_theirs, label_base);
cleanup:
	free_stash_info(&info);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା reject_reflog_ent(ଅଚଳ ଅକ୍ଷର *refname UNUSED,
			     ସଂରଚନା object_id *ooid UNUSED,
			     ସଂରଚନା object_id *noid UNUSED,
			     ଅଚଳ ଅକ୍ଷର *email UNUSED,
			     timestamp_t timestamp UNUSED,
			     ପୂର୍ଣ୍ଣସଂଖ୍ୟା tz UNUSED, ଅଚଳ ଅକ୍ଷର *message UNUSED,
			     ଶୂନ୍ୟ *cb_data UNUSED)
{
	ଫେରନ୍ତୁ 1;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା reflog_is_empty(ଅଚଳ ଅକ୍ଷର *refname)
{
	ଫେରନ୍ତୁ !refs_for_each_reflog_ent(get_main_ref_store(the_repository),
					 refname, reject_reflog_ent, ଖାଲି);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା do_drop_stash(ସଂରଚନା stash_info *info, ପୂର୍ଣ୍ଣସଂଖ୍ୟା quiet)
{
	ଯଦି (!reflog_delete(info->revision.buf,
			   EXPIRE_REFLOGS_REWRITE | EXPIRE_REFLOGS_UPDATE_REF,
			   0)) {
		ଯଦି (!quiet)
			printf_ln(_("Dropped %s (%s)"), info->revision.buf,
				  oid_to_hex(&info->w_commit));
	} ଅନ୍ୟଥା {
		ଫେରନ୍ତୁ error(_("%s: Could not drop stash entry"),
			     info->revision.buf);
	}

	ଯଦି (reflog_is_empty(ref_stash))
		do_clear_stash();

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_stash_info_assert(ସଂରଚନା stash_info *info, ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
				 ଅଚଳ ଅକ୍ଷର **argv)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = get_stash_info(info, argc, argv);

	ଯଦି (ret < 0)
		ଫେରନ୍ତୁ ret;

	ଯଦି (!info->is_stash_ref)
		ଫେରନ୍ତୁ error(_("'%s' is not a stash reference"), info->revision.buf);

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା drop_stash(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
		      ସଂରଚନା repository *repo UNUSED)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = -1;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା quiet = 0;
	ସଂରଚନା stash_info info = STASH_INFO_INIT;
	ସଂରଚନା option options[] = {
		OPT__QUIET(&quiet, N_("be quiet, only report errors")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_drop_usage, 0);

	ଯଦି (get_stash_info_assert(&info, argc, argv))
		ଯାଅ cleanup;

	ret = do_drop_stash(&info, quiet);
cleanup:
	free_stash_info(&info);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା କାଢ଼_stash(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
		     ସଂରଚନା repository *repo UNUSED)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = -1;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = use_index;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା quiet = 0;
	ସଂରଚନା stash_info info = STASH_INFO_INIT;
	ସଂରଚନା option options[] = {
		OPT__QUIET(&quiet, N_("be quiet, only report errors")),
		OPT_BOOL(0, "index", &index,
			 N_("attempt to recreate the index")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_କାଢ଼_usage, 0);

	ଯଦି (get_stash_info_assert(&info, argc, argv))
		ଯାଅ cleanup;

	ଯଦି ((ret = do_apply_stash(prefix, &info, index, quiet,
				  ଖାଲି, ଖାଲି, ଖାଲି)))
		printf_ln(_("The stash entry is kept in case "
			    "you need it again."));
	ଅନ୍ୟଥା
		ret = do_drop_stash(&info, quiet);

cleanup:
	free_stash_info(&info);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା branch_stash(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
			ସଂରଚନା repository *repo UNUSED)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = -1;
	ଅଚଳ ଅକ୍ଷର *branch = ଖାଲି;
	ସଂରଚନା stash_info info = STASH_INFO_INIT;
	ସଂରଚନା child_process cp = CHILD_PROCESS_INIT;
	ସଂରଚନା option options[] = {
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_branch_usage, 0);

	ଯଦି (!argc) {
		fprintf_ln(stderr, _("No branch name specified"));
		ଫେରନ୍ତୁ -1;
	}

	branch = argv[0];

	ଯଦି (get_stash_info(&info, argc - 1, argv + 1))
		ଯାଅ cleanup;

	cp.git_cmd = 1;
	strvec_pushl(&cp.args, "checkout", "-b", ଖାଲି);
	strvec_ଠେଲ(&cp.args, branch);
	strvec_ଠେଲ(&cp.args, oid_to_hex(&info.b_commit));
	ret = run_command(&cp);
	ଯଦି (!ret)
		ret = do_apply_stash(prefix, &info, 1, 0,
				     ଖାଲି, ଖାଲି, ଖାଲି);
	ଯଦି (!ret && info.is_stash_ref)
		ret = do_drop_stash(&info, 0);

cleanup:
	free_stash_info(&info);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା list_stash(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
		      ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା child_process cp = CHILD_PROCESS_INIT;
	ସଂରଚନା option options[] = {
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_list_usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT);

	ଯଦି (!refs_ref_exists(get_main_ref_store(the_repository), ref_stash))
		ଫେରନ୍ତୁ 0;

	cp.git_cmd = 1;
	strvec_pushl(&cp.args, "log", "--format=%gd: %gs", "-g",
		     "--first-parent", ଖାଲି);
	strvec_pushv(&cp.args, argv);
	strvec_ଠେଲ(&cp.args, ref_stash);
	strvec_ଠେଲ(&cp.args, "--");
	ଫେରନ୍ତୁ run_command(&cp);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା git_stash_config(ଅଚଳ ଅକ୍ଷର *var, ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ,
			    ଅଚଳ ସଂରଚନା config_context *ctx, ଶୂନ୍ୟ *cb)
{
	ଯଦି (!strcmp(var, "stash.showstat")) {
		show_stat = git_config_bool(var, ମୂଲ୍ୟ);
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (!strcmp(var, "stash.showpatch")) {
		show_patch = git_config_bool(var, ମୂଲ୍ୟ);
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (!strcmp(var, "stash.showincludeuntracked")) {
		show_include_untracked = git_config_bool(var, ମୂଲ୍ୟ);
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (!strcmp(var, "stash.index")) {
		use_index = git_config_bool(var, ମୂଲ୍ୟ);
		ଫେରନ୍ତୁ 0;
	}
	ଫେରନ୍ତୁ git_diff_basic_config(var, ମୂଲ୍ୟ, ctx, cb);
}

ସ୍ଥିର ଶୂନ୍ୟ diff_include_untracked(ଅଚଳ ସଂରଚନା stash_info *info, ସଂରଚନା diff_options *diff_opt)
{
	ଅଚଳ ସଂରଚନା object_id *oid[] = { &info->w_commit, &info->u_ବୃକ୍ଷ };
	ସଂରଚନା ବୃକ୍ଷ *ବୃକ୍ଷ[ARRAY_SIZE(oid)];
	ସଂରଚନା ବୃକ୍ଷ_desc ବୃକ୍ଷ_desc[ARRAY_SIZE(oid)];
	ସଂରଚନା unpack_trees_options unpack_ବୃକ୍ଷ_opt = { 0 };

	ପାଇଁ (size_t i = 0; i < ARRAY_SIZE(oid); i++) {
		ବୃକ୍ଷ[i] = repo_parse_ବୃକ୍ଷ_indirect(the_repository, oid[i]);
		ଯଦି (repo_parse_ବୃକ୍ଷ(the_repository, ବୃକ୍ଷ[i]) < 0)
			die(_("failed to parse tree"));
		init_ବୃକ୍ଷ_desc(&ବୃକ୍ଷ_desc[i], &ବୃକ୍ଷ[i]->object.oid,
			       ବୃକ୍ଷ[i]->buffer, ବୃକ୍ଷ[i]->size);
	}

	unpack_ବୃକ୍ଷ_opt.head_idx = -1;
	unpack_ବୃକ୍ଷ_opt.src_index = the_repository->index;
	unpack_ବୃକ୍ଷ_opt.dst_index = the_repository->index;
	unpack_ବୃକ୍ଷ_opt.merge = 1;
	unpack_ବୃକ୍ଷ_opt.fn = stash_worktree_untracked_merge;

	ଯଦି (unpack_trees(ARRAY_SIZE(ବୃକ୍ଷ_desc), ବୃକ୍ଷ_desc, &unpack_ବୃକ୍ଷ_opt))
		die(_("failed to unpack trees"));

	do_diff_cache(&info->b_commit, diff_opt);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_stash(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
		      ସଂରଚନା repository *repo UNUSED)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = -1;
	ସଂରଚନା stash_info info = STASH_INFO_INIT;
	ସଂରଚନା rev_info rev;
	ସଂରଚନା strvec stash_args = STRVEC_INIT;
	ସଂରଚନା strvec revision_args = STRVEC_INIT;
	ଗଣନା {
		UNTRACKED_NONE,
		UNTRACKED_INCLUDE,
		UNTRACKED_ONLY
	} show_untracked = show_include_untracked ? UNTRACKED_INCLUDE : UNTRACKED_NONE;
	ସଂରଚନା option options[] = {
		OPT_SET_INT('u', "include-untracked", &show_untracked,
			    N_("include untracked files in the stash"),
			    UNTRACKED_INCLUDE),
		OPT_SET_INT_F(0, "only-untracked", &show_untracked,
			      N_("only show untracked files in the stash"),
			      UNTRACKED_ONLY, PARSE_OPT_NONEG),
		OPT_END()
	};
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା do_usage = 0;

	init_diff_ui_defaults();
	repo_config(the_repository, git_diff_ui_config, ଖାଲି);
	repo_init_revisions(the_repository, &rev, prefix);

	argc = parse_options(argc, argv, prefix, options, git_stash_show_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	strvec_ଠେଲ(&revision_args, argv[0]);
	ପାଇଁ (i = 1; i < argc; i++) {
		ଯଦି (argv[i][0] != '-')
			strvec_ଠେଲ(&stash_args, argv[i]);
		ଅନ୍ୟଥା
			strvec_ଠେଲ(&revision_args, argv[i]);
	}

	ଯଦି (get_stash_info(&info, stash_args.nr, stash_args.v))
		ଯାଅ cleanup;

	/*
	 * The config settings are applied only if there are not passed
	 * any options.
	 */
	ଯଦି (revision_args.nr == 1) {
		ଯଦି (show_stat)
			rev.diffopt.output_format = DIFF_FORMAT_DIFFSTAT;

		ଯଦି (show_patch)
			rev.diffopt.output_format |= DIFF_FORMAT_PATCH;

		ଯଦି (!show_stat && !show_patch) {
			ret = 0;
			ଯାଅ cleanup;
		}
	}

	setup_revisions_from_strvec(&revision_args, &rev, ଖାଲି);
	ଯଦି (revision_args.nr > 1)
		ଯାଅ usage;
	ଯଦି (!rev.diffopt.output_format) {
		rev.diffopt.output_format = DIFF_FORMAT_PATCH;
		diff_setup_done(&rev.diffopt);
	}

	rev.diffopt.flags.recursive = 1;
	setup_diff_pager(&rev.diffopt);
	ବାଛ (show_untracked) {
	ସ୍ଥିତି UNTRACKED_NONE:
		diff_ବୃକ୍ଷ_oid(&info.b_commit, &info.w_commit, "", &rev.diffopt);
		ଭାଙ୍ଗ;
	ସ୍ଥିତି UNTRACKED_ONLY:
		ଯଦି (info.has_u)
			diff_ମୂଳ_ବୃକ୍ଷ_oid(&info.u_ବୃକ୍ଷ, "", &rev.diffopt);
		ଭାଙ୍ଗ;
	ସ୍ଥିତି UNTRACKED_INCLUDE:
		ଯଦି (info.has_u)
			diff_include_untracked(&info, &rev.diffopt);
		ଅନ୍ୟଥା
			diff_ବୃକ୍ଷ_oid(&info.b_commit, &info.w_commit, "", &rev.diffopt);
		ଭାଙ୍ଗ;
	}
	log_ବୃକ୍ଷ_diff_flush(&rev);

	ret = diff_result_code(&rev);

cleanup:
	strvec_clear(&revision_args);
	strvec_clear(&stash_args);
	free_stash_info(&info);
	release_revisions(&rev);
	ଯଦି (do_usage)
		usage_with_options(git_stash_show_usage, options);
	ଫେରନ୍ତୁ ret;
usage:
	do_usage = 1;
	ଯାଅ cleanup;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା do_store_stash(ଅଚଳ ସଂରଚନା object_id *w_commit, ଅଚଳ ଅକ୍ଷର *stash_ସନ୍ଦେଶ,
			  ପୂର୍ଣ୍ଣସଂଖ୍ୟା quiet)
{
	ସଂରଚନା stash_info info;
	ଅକ୍ଷର revision[GIT_MAX_HEXSZ];

	oid_to_hex_r(revision, w_commit);
	assert_stash_like(&info, revision);

	ଯଦି (!stash_ସନ୍ଦେଶ)
		stash_ସନ୍ଦେଶ = "Created via \"git stash store\".";

	ଯଦି (refs_update_ref(get_main_ref_store(the_repository), stash_ସନ୍ଦେଶ, ref_stash, w_commit, ଖାଲି,
			    REF_FORCE_CREATE_REFLOG,
			    quiet ? UPDATE_REFS_QUIET_ON_ERR :
			    UPDATE_REFS_ସନ୍ଦେଶ_ON_ERR)) {
		ଯଦି (!quiet) {
			fprintf_ln(stderr, _("Cannot update %s with %s"),
				   ref_stash, oid_to_hex(w_commit));
		}
		ଫେରନ୍ତୁ -1;
	}

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା store_stash(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
		       ସଂରଚନା repository *repo UNUSED)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା quiet = 0;
	ଅଚଳ ଅକ୍ଷର *stash_ସନ୍ଦେଶ = ଖାଲି;
	ସଂରଚନା object_id obj;
	ସଂରଚନା option options[] = {
		OPT__QUIET(&quiet, N_("be quiet")),
		OPT_STRING('m', "message", &stash_ସନ୍ଦେଶ, "message",
			   N_("stash message")),
		OPT_END()
	};
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_store_usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT);

	ଯଦି (argc != 1) {
		ଯଦି (!quiet)
			fprintf_ln(stderr, _("\"git stash store\" requires one "
					     "<commit> argument"));
		ଫେରନ୍ତୁ -1;
	}

	ଯଦି (repo_get_oid_with_flags(the_repository, argv[0], &obj,
				    quiet ? GET_OID_QUIETLY : 0)) {
		ଯଦି (!quiet)
			fprintf_ln(stderr, _("Cannot update %s with %s"),
					     ref_stash, argv[0]);
		ret = -1;
		ଯାଅ out;
	}

	ret = do_store_stash(&obj, stash_ସନ୍ଦେଶ, quiet);

out:
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ଶୂନ୍ୟ add_pathspecs(ସଂରଚନା strvec *args,
			  ଅଚଳ ସଂରଚନା pathspec *ps) {
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	ପାଇଁ (i = 0; i < ps->nr; i++)
		strvec_ଠେଲ(args, ps->ବସ୍ତୁଗୁଡ଼ିକ[i].original);
}

/*
 * `untracked_files` will be filled with the names of untracked files.
 * The return value is:
 *
 * = 0 if there are not any untracked files
 * > 0 if there are untracked files
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_untracked_files(ଅଚଳ ସଂରଚନା pathspec *ps, ପୂର୍ଣ୍ଣସଂଖ୍ୟା include_untracked,
			       ସଂରଚନା strbuf *untracked_files)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା found = 0;
	ସଂରଚନା dir_struct dir = DIR_INIT;

	ଯଦି (include_untracked != INCLUDE_ALL_FILES)
		setup_standard_excludes(&dir);

	fill_directory(&dir, the_repository->index, ps);
	ପାଇଁ (i = 0; i < dir.nr; i++) {
		ସଂରଚନା dir_entry *ent = dir.entries[i];
		found++;
		strbuf_addstr(untracked_files, ent->name);
		/* NUL-terminate: will be fed to update-index -z */
		strbuf_addch(untracked_files, '\0');
	}

	dir_clear(&dir);
	ଫେରନ୍ତୁ found;
}

/*
 * The return value of `check_changes_tracked_files()` can be:
 *
 * < 0 if there was an error
 * = 0 if there are no changes.
 * > 0 if there are changes.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା check_changes_tracked_files(ଅଚଳ ସଂରଚନା pathspec *ps)
{
	ସଂରଚନା rev_info rev;
	ସଂରଚନା object_id dummy;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;

	/* No initial commit. */
	ଯଦି (repo_get_oid(the_repository, "HEAD", &dummy))
		ଫେରନ୍ତୁ -1;

	ଯଦି (repo_read_index(the_repository) < 0)
		ଫେରନ୍ତୁ -1;

	repo_init_revisions(the_repository, &rev, ଖାଲି);
	copy_pathspec(&rev.prune_data, ps);

	rev.diffopt.flags.quick = 1;
	rev.diffopt.flags.ignore_submodules = 1;
	rev.abbrev = 0;

	add_head_to_pending(&rev);
	diff_setup_done(&rev.diffopt);

	run_diff_index(&rev, DIFF_INDEX_CACHED);
	ଯଦି (diff_result_code(&rev)) {
		ret = 1;
		ଯାଅ done;
	}

	run_diff_files(&rev, 0);
	ଯଦି (diff_result_code(&rev)) {
		ret = 1;
		ଯାଅ done;
	}

done:
	release_revisions(&rev);
	ଫେରନ୍ତୁ ret;
}

/*
 * The function will fill `untracked_files` with the names of untracked files
 * It will return 1 if there were any changes and 0 if there were not.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା check_changes(ଅଚଳ ସଂରଚନା pathspec *ps, ପୂର୍ଣ୍ଣସଂଖ୍ୟା include_untracked,
			 ସଂରଚନା strbuf *untracked_files)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;
	ଯଦି (check_changes_tracked_files(ps))
		ret = 1;

	ଯଦି (include_untracked && get_untracked_files(ps, include_untracked,
						     untracked_files))
		ret = 1;

	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା save_untracked_files(ସଂରଚନା stash_info *info, ସଂରଚନା strbuf *ସନ୍ଦେଶ,
				ସଂରଚନା strbuf *files)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;
	ସଂରଚନା strbuf untracked_ସନ୍ଦେଶ = STRBUF_INIT;
	ସଂରଚନା child_process cp_upd_index = CHILD_PROCESS_INIT;
	ସଂରଚନା index_state istate = INDEX_STATE_INIT(the_repository);

	cp_upd_index.git_cmd = 1;
	strvec_pushl(&cp_upd_index.args, "update-index", "-z", "--add",
		     "--remove", "--stdin", ଖାଲି);
	strvec_pushf(&cp_upd_index.env, "GIT_INDEX_FILE=%s",
			 stash_index_path.buf);

	strbuf_addf(&untracked_ସନ୍ଦେଶ, "untracked files on %s\n", ସନ୍ଦେଶ->buf);
	ଯଦି (pipe_command(&cp_upd_index, files->buf, files->len, ଖାଲି, 0,
			 ଖାଲି, 0)) {
		ret = -1;
		ଯାଅ done;
	}

	ଯଦି (write_index_as_ବୃକ୍ଷ(&info->u_ବୃକ୍ଷ, &istate, stash_index_path.buf, 0,
				ଖାଲି)) {
		ret = -1;
		ଯାଅ done;
	}

	ଯଦି (commit_ବୃକ୍ଷ(untracked_ସନ୍ଦେଶ.buf, untracked_ସନ୍ଦେଶ.len,
			&info->u_ବୃକ୍ଷ, ଖାଲି, &info->u_commit, ଖାଲି, ଖାଲି)) {
		ret = -1;
		ଯାଅ done;
	}

done:
	release_index(&istate);
	strbuf_release(&untracked_ସନ୍ଦେଶ);
	remove_path(stash_index_path.buf);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା stash_staged(ସଂରଚନା stash_info *info, ସଂରଚନା strbuf *out_patch,
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା quiet)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;
	ସଂରଚନା child_process cp_diff_ବୃକ୍ଷ = CHILD_PROCESS_INIT;
	ସଂରଚନା index_state istate = INDEX_STATE_INIT(the_repository);

	ଯଦି (write_index_as_ବୃକ୍ଷ(&info->w_ବୃକ୍ଷ, &istate, the_repository->index_file,
				0, ଖାଲି)) {
		ret = -1;
		ଯାଅ done;
	}

	cp_diff_ବୃକ୍ଷ.git_cmd = 1;
	strvec_pushl(&cp_diff_ବୃକ୍ଷ.args, "diff-tree", "-p", "--binary",
		     "--no-color",
		     "-U1", "HEAD", oid_to_hex(&info->w_ବୃକ୍ଷ), "--", ଖାଲି);
	ଯଦି (pipe_command(&cp_diff_ବୃକ୍ଷ, ଖାଲି, 0, out_patch, 0, ଖାଲି, 0)) {
		ret = -1;
		ଯାଅ done;
	}

	ଯଦି (!out_patch->len) {
		ଯଦି (!quiet)
			fprintf_ln(stderr, _("No staged changes"));
		ret = 1;
	}

done:
	release_index(&istate);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା stash_patch(ସଂରଚନା stash_info *info, ଅଚଳ ସଂରଚନା pathspec *ps,
		       ସଂରଚନା strbuf *out_patch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା quiet,
		       ସଂରଚନା interactive_options *interactive_opts)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;
	ସଂରଚନା child_process cp_diff_ବୃକ୍ଷ = CHILD_PROCESS_INIT;
	ସଂରଚନା commit *head_commit;
	ଅଚଳ ସଂରଚନା object_id *head_ବୃକ୍ଷ;
	ସଂରଚନା index_state istate = INDEX_STATE_INIT(the_repository);
	ଅକ୍ଷର *old_index_env = ଖାଲି, *old_repo_index_file;

	remove_path(stash_index_path.buf);

	head_commit = lookup_commit(the_repository, &info->b_commit);
	ଯଦି (!head_commit || repo_parse_commit(the_repository, head_commit)) {
		ret = -1;
		ଯାଅ done;
	}
	head_ବୃକ୍ଷ = get_commit_ବୃକ୍ଷ_oid(head_commit);
	ଯଦି (!head_ବୃକ୍ଷ) {
		ret = -1;
		ଯାଅ done;
	}

	ଯଦି (create_index_from_ବୃକ୍ଷ(head_ବୃକ୍ଷ, stash_index_path.buf)) {
		ret = -1;
		ଯାଅ done;
	}

	/* Find out what the user wants. */
	old_repo_index_file = the_repository->index_file;
	the_repository->index_file = stash_index_path.buf;
	old_index_env = xstrdup_or_null(getenv(INDEX_ENVIRONMENT));
	setenv(INDEX_ENVIRONMENT, the_repository->index_file, 1);

	ret = !!run_add_p(the_repository, ADD_P_STASH, interactive_opts, ଖାଲି, ps, 0);

	the_repository->index_file = old_repo_index_file;
	ଯଦି (old_index_env && *old_index_env)
		setenv(INDEX_ENVIRONMENT, old_index_env, 1);
	ଅନ୍ୟଥା
		unsetenv(INDEX_ENVIRONMENT);
	FREE_AND_NULL(old_index_env);

	/* State of the working tree. */
	ଯଦି (write_index_as_ବୃକ୍ଷ(&info->w_ବୃକ୍ଷ, &istate, stash_index_path.buf, 0,
				ଖାଲି)) {
		ret = -1;
		ଯାଅ done;
	}

	cp_diff_ବୃକ୍ଷ.git_cmd = 1;
	strvec_pushl(&cp_diff_ବୃକ୍ଷ.args, "diff-tree", "-p", "-U1", "HEAD",
		     "--no-color",
		     oid_to_hex(&info->w_ବୃକ୍ଷ), "--", ଖାଲି);
	ଯଦି (pipe_command(&cp_diff_ବୃକ୍ଷ, ଖାଲି, 0, out_patch, 0, ଖାଲି, 0)) {
		ret = -1;
		ଯାଅ done;
	}

	ଯଦି (!out_patch->len) {
		ଯଦି (!quiet)
			fprintf_ln(stderr, _("No changes selected"));
		ret = 1;
	}

done:
	release_index(&istate);
	remove_path(stash_index_path.buf);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା stash_working_ବୃକ୍ଷ(ସଂରଚନା stash_info *info, ଅଚଳ ସଂରଚନା pathspec *ps)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;
	ସଂରଚନା rev_info rev;
	ସଂରଚନା child_process cp_upd_index = CHILD_PROCESS_INIT;
	ସଂରଚନା strbuf diff_output = STRBUF_INIT;
	ସଂରଚନା index_state istate = INDEX_STATE_INIT(the_repository);

	repo_init_revisions(the_repository, &rev, ଖାଲି);
	copy_pathspec(&rev.prune_data, ps);

	set_alternate_index_output(stash_index_path.buf);
	ଯଦି (reset_ବୃକ୍ଷ(&info->i_ବୃକ୍ଷ, 0, 0)) {
		ret = -1;
		ଯାଅ done;
	}
	set_alternate_index_output(ଖାଲି);

	rev.diffopt.output_format = DIFF_FORMAT_CALLBACK;
	rev.diffopt.format_callback = add_diff_to_buf;
	rev.diffopt.format_callback_data = &diff_output;

	ଯଦି (repo_read_index_preload(the_repository, &rev.diffopt.pathspec, 0) < 0) {
		ret = -1;
		ଯାଅ done;
	}

	add_pending_object(&rev, parse_object(the_repository, &info->b_commit),
			   "");
	run_diff_index(&rev, 0);

	cp_upd_index.git_cmd = 1;
	strvec_pushl(&cp_upd_index.args, "update-index",
		     "--ignore-skip-worktree-entries",
		     "-z", "--add", "--remove", "--stdin", ଖାଲି);
	strvec_pushf(&cp_upd_index.env, "GIT_INDEX_FILE=%s",
		     stash_index_path.buf);

	ଯଦି (pipe_command(&cp_upd_index, diff_output.buf, diff_output.len,
			 ଖାଲି, 0, ଖାଲି, 0)) {
		ret = -1;
		ଯାଅ done;
	}

	ଯଦି (write_index_as_ବୃକ୍ଷ(&info->w_ବୃକ୍ଷ, &istate, stash_index_path.buf, 0,
				ଖାଲି)) {
		ret = -1;
		ଯାଅ done;
	}

done:
	release_index(&istate);
	release_revisions(&rev);
	strbuf_release(&diff_output);
	remove_path(stash_index_path.buf);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା do_create_stash(ଅଚଳ ସଂରଚନା pathspec *ps, ସଂରଚନା strbuf *stash_ସନ୍ଦେଶ_buf,
			   ପୂର୍ଣ୍ଣସଂଖ୍ୟା include_untracked, ପୂର୍ଣ୍ଣସଂଖ୍ୟା patch_mode,
			   ସଂରଚନା interactive_options *interactive_opts,
			   ପୂର୍ଣ୍ଣସଂଖ୍ୟା only_staged, ସଂରଚନା stash_info *info, ସଂରଚନା strbuf *patch,
			   ପୂର୍ଣ୍ଣସଂଖ୍ୟା quiet)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା untracked_commit_option = 0;
	ଅଚଳ ଅକ୍ଷର *head_short_sha1 = ଖାଲି;
	ଅଚଳ ଅକ୍ଷର *branch_ref = ଖାଲି;
	ଅଚଳ ଅକ୍ଷର *branch_name = "(no branch)";
	ଅକ୍ଷର *branch_name_buf = ଖାଲି;
	ସଂରଚନା commit *head_commit = ଖାଲି;
	ସଂରଚନା commit_list *parents = ଖାଲି;
	ସଂରଚନା strbuf ସନ୍ଦେଶ = STRBUF_INIT;
	ସଂରଚନା strbuf commit_ବୃକ୍ଷ_label = STRBUF_INIT;
	ସଂରଚନା strbuf untracked_files = STRBUF_INIT;

	prepare_fallback_ident("git stash", "git@stash");

	repo_read_index_preload(the_repository, ଖାଲି, 0);
	ଯଦି (repo_refresh_and_write_index(the_repository, REFRESH_QUIET, 0, 0,
					 ଖାଲି, ଖାଲି, ଖାଲି) < 0) {
		ret = error(_("could not write index"));
		ଯାଅ done;
	}

	ଯଦି (repo_get_oid(the_repository, "HEAD", &info->b_commit)) {
		ଯଦି (!quiet)
			fprintf_ln(stderr, _("You do not have "
					     "the initial commit yet"));
		ret = -1;
		ଯାଅ done;
	} ଅନ୍ୟଥା {
		head_commit = lookup_commit(the_repository, &info->b_commit);
	}

	ଯଦି (!check_changes(ps, include_untracked, &untracked_files)) {
		ret = 1;
		ଯାଅ done;
	}

	branch_ref = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
					     "HEAD", 0, ଖାଲି, &flags);

	ଯଦି (flags & REF_ISSYMREF) {
		ଯଦି (skip_prefix(branch_ref, "refs/heads/", &branch_name))
			branch_name = branch_name_buf = xstrdup(branch_name);
	}

	head_short_sha1 = repo_find_unique_abbrev(the_repository,
						  &head_commit->object.oid,
						  DEFAULT_ABBREV);
	strbuf_addf(&ସନ୍ଦେଶ, "%s: %s ", branch_name, head_short_sha1);
	pp_commit_easy(CMIT_FMT_ONELINE, head_commit, &ସନ୍ଦେଶ);

	strbuf_addf(&commit_ବୃକ୍ଷ_label, "index on %s\n", ସନ୍ଦେଶ.buf);
	commit_list_ଭର୍ତ୍ତି(head_commit, &parents);
	ଯଦି (write_index_as_ବୃକ୍ଷ(&info->i_ବୃକ୍ଷ, the_repository->index,
				repo_get_index_file(the_repository), 0, ଖାଲି) ||
	    commit_ବୃକ୍ଷ(commit_ବୃକ୍ଷ_label.buf, commit_ବୃକ୍ଷ_label.len,
			&info->i_ବୃକ୍ଷ, parents, &info->i_commit, ଖାଲି, ଖାଲି)) {
		ଯଦି (!quiet)
			fprintf_ln(stderr, _("Cannot save the current "
					     "index state"));
		ret = -1;
		ଯାଅ done;
	}

	commit_list_free(parents);
	parents = ଖାଲି;

	ଯଦି (include_untracked) {
		ଯଦି (save_untracked_files(info, &ସନ୍ଦେଶ, &untracked_files)) {
			ଯଦି (!quiet)
				fprintf_ln(stderr, _("Cannot save "
						     "the untracked files"));
			ret = -1;
			ଯାଅ done;
		}
		untracked_commit_option = 1;
	}
	ଯଦି (patch_mode) {
		ret = stash_patch(info, ps, patch, quiet, interactive_opts);
		ଯଦି (ret < 0) {
			ଯଦି (!quiet)
				fprintf_ln(stderr, _("Cannot save the current "
						     "worktree state"));
			ଯାଅ done;
		} ଅନ୍ୟଥା ଯଦି (ret > 0) {
			ଯାଅ done;
		}
	} ଅନ୍ୟଥା ଯଦି (only_staged) {
		ret = stash_staged(info, patch, quiet);
		ଯଦି (ret < 0) {
			ଯଦି (!quiet)
				fprintf_ln(stderr, _("Cannot save the current "
						     "staged state"));
			ଯାଅ done;
		} ଅନ୍ୟଥା ଯଦି (ret > 0) {
			ଯାଅ done;
		}
	} ଅନ୍ୟଥା {
		ଯଦି (stash_working_ବୃକ୍ଷ(info, ps)) {
			ଯଦି (!quiet)
				fprintf_ln(stderr, _("Cannot save the current "
						     "worktree state"));
			ret = -1;
			ଯାଅ done;
		}
	}

	ଯଦି (!stash_ସନ୍ଦେଶ_buf->len)
		strbuf_addf(stash_ସନ୍ଦେଶ_buf, "WIP on %s", ସନ୍ଦେଶ.buf);
	ଅନ୍ୟଥା
		strbuf_insertf(stash_ସନ୍ଦେଶ_buf, 0, "On %s: ", branch_name);

	ଯଦି (untracked_commit_option)
		commit_list_ଭର୍ତ୍ତି(lookup_commit(the_repository,
						 &info->u_commit),
				   &parents);
	commit_list_ଭର୍ତ୍ତି(lookup_commit(the_repository, &info->i_commit),
			   &parents);
	commit_list_ଭର୍ତ୍ତି(head_commit, &parents);

	ଯଦି (commit_ବୃକ୍ଷ(stash_ସନ୍ଦେଶ_buf->buf, stash_ସନ୍ଦେଶ_buf->len, &info->w_ବୃକ୍ଷ,
			parents, &info->w_commit, ଖାଲି, ଖାଲି)) {
		ଯଦି (!quiet)
			fprintf_ln(stderr, _("Cannot record "
					     "working tree state"));
		ret = -1;
		ଯାଅ done;
	}

done:
	strbuf_release(&commit_ବୃକ୍ଷ_label);
	strbuf_release(&ସନ୍ଦେଶ);
	strbuf_release(&untracked_files);
	commit_list_free(parents);
	ମୁକ୍ତ(branch_name_buf);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା create_stash(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix UNUSED,
			ସଂରଚନା repository *repo UNUSED)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
	ସଂରଚନା strbuf stash_ସନ୍ଦେଶ_buf = STRBUF_INIT;
	ସଂରଚନା stash_info info = STASH_INFO_INIT;
	ସଂରଚନା pathspec ps;

	/* Starting with argv[1], since argv[0] is "create" */
	strbuf_join_argv(&stash_ସନ୍ଦେଶ_buf, argc - 1, ++argv, ' ');

	memset(&ps, 0, ଆକାର(ps));
	ଯଦି (!check_changes_tracked_files(&ps))
		ଫେରନ୍ତୁ 0;

	ret = do_create_stash(&ps, &stash_ସନ୍ଦେଶ_buf, 0, 0, ଖାଲି, 0, &info,
			      ଖାଲି, 0);
	ଯଦି (!ret)
		printf_ln("%s", oid_to_hex(&info.w_commit));

	free_stash_info(&info);
	strbuf_release(&stash_ସନ୍ଦେଶ_buf);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା do_ଠେଲ_stash(ଅଚଳ ସଂରଚନା pathspec *ps, ଅଚଳ ଅକ୍ଷର *stash_ସନ୍ଦେଶ, ପୂର୍ଣ୍ଣସଂଖ୍ୟା quiet,
			 ପୂର୍ଣ୍ଣସଂଖ୍ୟା keep_index, ପୂର୍ଣ୍ଣସଂଖ୍ୟା patch_mode,
			 ସଂରଚନା interactive_options *interactive_opts,
			 ପୂର୍ଣ୍ଣସଂଖ୍ୟା include_untracked, ପୂର୍ଣ୍ଣସଂଖ୍ୟା only_staged)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;
	ସଂରଚନା stash_info info = STASH_INFO_INIT;
	ସଂରଚନା strbuf patch = STRBUF_INIT;
	ସଂରଚନା strbuf stash_ସନ୍ଦେଶ_buf = STRBUF_INIT;
	ସଂରଚନା strbuf untracked_files = STRBUF_INIT;
	ସଂରଚନା strbuf out = STRBUF_INIT;

	ଯଦି (patch_mode && keep_index == -1)
		keep_index = 1;

	ଯଦି (patch_mode && include_untracked) {
		fprintf_ln(stderr, _("Can't use --patch and --include-untracked"
				     " or --all at the same time"));
		ret = -1;
		ଯାଅ done;
	}

	/* --patch overrides --staged */
	ଯଦି (patch_mode)
		only_staged = 0;

	ଯଦି (only_staged && include_untracked) {
		fprintf_ln(stderr, _("Can't use --staged and --include-untracked"
				     " or --all at the same time"));
		ret = -1;
		ଯାଅ done;
	}

	repo_read_index_preload(the_repository, ଖାଲି, 0);
	ଯଦି (!include_untracked && ps->nr) {
		ଅକ୍ଷର *ps_matched = xcalloc(ps->nr, 1);

		ଯଦି (pathspec_needs_expanded_index(the_repository->index, ps))
			ensure_full_index(the_repository->index);
		ପାଇଁ (size_t i = 0; i < the_repository->index->cache_nr; i++)
			ce_path_match(the_repository->index, the_repository->index->cache[i], ps,
				      ps_matched);

		ଯଦି (report_path_error(ps_matched, ps)) {
			fprintf_ln(stderr, _("Did you forget to 'git add'?"));
			ret = -1;
			ମୁକ୍ତ(ps_matched);
			ଯାଅ done;
		}
		ମୁକ୍ତ(ps_matched);
	}

	ଯଦି (repo_refresh_and_write_index(the_repository, REFRESH_QUIET, 0, 0,
					 ଖାଲି, ଖାଲି, ଖାଲି)) {
		ret = error(_("could not write index"));
		ଯାଅ done;
	}

	ଯଦି (!check_changes(ps, include_untracked, &untracked_files)) {
		ଯଦି (!quiet)
			printf_ln(_("No local changes to save"));
		ଯାଅ done;
	}

	ଯଦି (!refs_reflog_exists(get_main_ref_store(the_repository), ref_stash) && do_clear_stash()) {
		ret = -1;
		ଯଦି (!quiet)
			fprintf_ln(stderr, _("Cannot initialize stash"));
		ଯାଅ done;
	}

	ଯଦି (stash_ସନ୍ଦେଶ)
		strbuf_addstr(&stash_ସନ୍ଦେଶ_buf, stash_ସନ୍ଦେଶ);
	ଯଦି (do_create_stash(ps, &stash_ସନ୍ଦେଶ_buf, include_untracked, patch_mode,
			    interactive_opts, only_staged, &info, &patch, quiet)) {
		ret = -1;
		ଯାଅ done;
	}

	ଯଦି (do_store_stash(&info.w_commit, stash_ସନ୍ଦେଶ_buf.buf, 1)) {
		ret = -1;
		ଯଦି (!quiet)
			fprintf_ln(stderr, _("Cannot save the current status"));
		ଯାଅ done;
	}

	ଯଦି (!quiet)
		printf_ln(_("Saved working directory and index state %s"),
			  stash_ସନ୍ଦେଶ_buf.buf);

	ଯଦି (!(patch_mode || only_staged)) {
		ଯଦି (include_untracked && !ps->nr) {
			ସଂରଚନା child_process cp = CHILD_PROCESS_INIT;

			cp.git_cmd = 1;
			ଯଦି (startup_info->original_cwd) {
				cp.dir = startup_info->original_cwd;
				strvec_pushf(&cp.env, "%s=%s",
					     GIT_WORK_ବୃକ୍ଷ_ENVIRONMENT,
					     the_repository->worktree);
			}
			strvec_pushl(&cp.args, "clean", "--force",
				     "--quiet", "-d", ":/", ଖାଲି);
			ଯଦି (include_untracked == INCLUDE_ALL_FILES)
				strvec_ଠେଲ(&cp.args, "-x");
			ଯଦି (run_command(&cp)) {
				ret = -1;
				ଯାଅ done;
			}
		}
		discard_index(the_repository->index);
		ଯଦି (ps->nr) {
			ସଂରଚନା child_process cp_add = CHILD_PROCESS_INIT;
			ସଂରଚନା child_process cp_diff = CHILD_PROCESS_INIT;
			ସଂରଚନା child_process cp_apply = CHILD_PROCESS_INIT;

			cp_add.git_cmd = 1;
			strvec_ଠେଲ(&cp_add.args, "add");
			ଯଦି (!include_untracked)
				strvec_ଠେଲ(&cp_add.args, "-u");
			ଯଦି (include_untracked == INCLUDE_ALL_FILES)
				strvec_ଠେଲ(&cp_add.args, "--force");
			strvec_ଠେଲ(&cp_add.args, "--");
			add_pathspecs(&cp_add.args, ps);
			ଯଦି (run_command(&cp_add)) {
				ret = -1;
				ଯାଅ done;
			}

			cp_diff.git_cmd = 1;
			strvec_pushl(&cp_diff.args, "diff-index", "-p",
				     "--no-color",
				     "--cached", "--binary", "HEAD", "--",
				     ଖାଲି);
			add_pathspecs(&cp_diff.args, ps);
			ଯଦି (pipe_command(&cp_diff, ଖାଲି, 0, &out, 0, ଖାଲି, 0)) {
				ret = -1;
				ଯାଅ done;
			}

			cp_apply.git_cmd = 1;
			strvec_pushl(&cp_apply.args, "apply", "--index",
				     "-R", ଖାଲି);
			ଯଦି (pipe_command(&cp_apply, out.buf, out.len, ଖାଲି, 0,
					 ଖାଲି, 0)) {
				ret = -1;
				ଯାଅ done;
			}
		} ଅନ୍ୟଥା {
			ସଂରଚନା child_process cp = CHILD_PROCESS_INIT;
			cp.git_cmd = 1;
			/* BUG: this nukes untracked files in the way */
			strvec_pushl(&cp.args, "reset", "--hard", "-q",
				     "--no-recurse-submodules", ଖାଲି);
			ଯଦି (run_command(&cp)) {
				ret = -1;
				ଯାଅ done;
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
		ଯଦି (keep_index == 1 && !is_null_oid(&info.i_ବୃକ୍ଷ) &&
		    !is_empty_ବୃକ୍ଷ_oid(&info.i_ବୃକ୍ଷ, the_repository->hash_algo)) {
			ସଂରଚନା child_process cp = CHILD_PROCESS_INIT;

			cp.git_cmd = 1;
			strvec_pushl(&cp.args, "checkout", "--no-overlay",
				     oid_to_hex(&info.i_ବୃକ୍ଷ), "--", ଖାଲି);
			ଯଦି (!ps->nr)
				strvec_ଠେଲ(&cp.args, ":/");
			ଅନ୍ୟଥା
				add_pathspecs(&cp.args, ps);
			ଯଦି (run_command(&cp)) {
				ret = -1;
				ଯାଅ done;
			}
		}
		ଯାଅ done;
	} ଅନ୍ୟଥା {
		ସଂରଚନା child_process cp = CHILD_PROCESS_INIT;

		cp.git_cmd = 1;
		strvec_pushl(&cp.args, "apply", "-R", ଖାଲି);

		ଯଦି (pipe_command(&cp, patch.buf, patch.len, ଖାଲି, 0, ଖାଲି, 0)) {
			ଯଦି (!quiet)
				fprintf_ln(stderr, _("Cannot remove "
						     "worktree changes"));
			ret = -1;
			ଯାଅ done;
		}

		ଯଦି (keep_index < 1) {
			ସଂରଚନା child_process cp = CHILD_PROCESS_INIT;

			cp.git_cmd = 1;
			strvec_pushl(&cp.args, "reset", "-q", "--refresh", "--",
				     ଖାଲି);
			add_pathspecs(&cp.args, ps);
			ଯଦି (run_command(&cp)) {
				ret = -1;
				ଯାଅ done;
			}
		}
		ଯାଅ done;
	}

done:
	strbuf_release(&patch);
	strbuf_release(&out);
	free_stash_info(&info);
	strbuf_release(&stash_ସନ୍ଦେଶ_buf);
	strbuf_release(&untracked_files);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଠେଲ_stash(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
		      ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଠେଲ_assumed)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା force_assume = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା keep_index = -1;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା only_staged = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା patch_mode = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା include_untracked = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା quiet = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା pathspec_file_nul = 0;
	ଅଚଳ ଅକ୍ଷର *stash_ସନ୍ଦେଶ = ଖାଲି;
	ଅକ୍ଷର *pathspec_from_file = ଖାଲି;
	ସଂରଚନା pathspec ps;
	ସଂରଚନା interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	ସଂରଚନା option options[] = {
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
		OPT_STRING('m', "message", &stash_ସନ୍ଦେଶ, N_("message"),
			   N_("stash message")),
		OPT_PATHSPEC_FROM_FILE(&pathspec_from_file),
		OPT_PATHSPEC_FILE_NUL(&pathspec_file_nul),
		OPT_END()
	};
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	ଯଦି (argc) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags = PARSE_OPT_KEEP_DASHDASH;

		ଯଦି (ଠେଲ_assumed)
			flags |= PARSE_OPT_STOP_AT_NON_OPTION;

		argc = parse_options(argc, argv, prefix, options,
				     ଠେଲ_assumed ? git_stash_usage :
				     git_stash_ଠେଲ_usage, flags);
		force_assume |= patch_mode;
	}

	ଯଦି (argc) {
		ଯଦି (!strcmp(argv[0], "--")) {
			argc--;
			argv++;
		} ଅନ୍ୟଥା ଯଦି (ଠେଲ_assumed && !force_assume) {
			die("subcommand wasn't specified; 'push' can't be assumed due to unexpected token '%s'",
			    argv[0]);
		}
	}

	parse_pathspec(&ps, 0, PATHSPEC_PREFER_FULL | PATHSPEC_PREFIX_ORIGIN,
		       prefix, argv);

	ଯଦି (pathspec_from_file) {
		ଯଦି (patch_mode)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--patch");

		ଯଦି (only_staged)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--staged");

		ଯଦି (ps.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&ps, 0,
				    PATHSPEC_PREFER_FULL | PATHSPEC_PREFIX_ORIGIN,
				    prefix, pathspec_from_file, pathspec_file_nul);
	} ଅନ୍ୟଥା ଯଦି (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	ଯଦି (!patch_mode) {
		ଯଦି (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--patch");
		ଯଦି (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--patch");
		ଯଦି (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--patch");
	}

	ଯଦି (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	ଯଦି (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	ret = do_ଠେଲ_stash(&ps, stash_ସନ୍ଦେଶ, quiet, keep_index, patch_mode,
			    &interactive_opts, include_untracked, only_staged);

	clear_pathspec(&ps);
	ମୁକ୍ତ(pathspec_from_file);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଠେଲ_stash_unassumed(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
				ସଂରଚନା repository *repo UNUSED)
{
	ଫେରନ୍ତୁ ଠେଲ_stash(argc, argv, prefix, 0);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା save_stash(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
		      ସଂରଚନା repository *repo UNUSED)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା keep_index = -1;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା only_staged = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା patch_mode = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା include_untracked = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା quiet = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;
	ଅଚଳ ଅକ୍ଷର *stash_ସନ୍ଦେଶ = ଖାଲି;
	ସଂରଚନା pathspec ps;
	ସଂରଚନା strbuf stash_ସନ୍ଦେଶ_buf = STRBUF_INIT;
	ସଂରଚନା interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	ସଂରଚନା option options[] = {
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
		OPT_STRING('m', "message", &stash_ସନ୍ଦେଶ, "message",
			   N_("stash message")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_save_usage,
			     PARSE_OPT_KEEP_DASHDASH);

	ଯଦି (argc)
		stash_ସନ୍ଦେଶ = strbuf_join_argv(&stash_ସନ୍ଦେଶ_buf, argc, argv, ' ');

	memset(&ps, 0, ଆକାର(ps));

	ଯଦି (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	ଯଦି (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	ଯଦି (!patch_mode) {
		ଯଦି (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--patch");
		ଯଦି (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--patch");
		ଯଦି (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--patch");
	}

	ret = do_ଠେଲ_stash(&ps, stash_ସନ୍ଦେଶ, quiet, keep_index,
			    patch_mode, &interactive_opts, include_untracked,
			    only_staged);

	strbuf_release(&stash_ସନ୍ଦେଶ_buf);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା write_commit_with_parents(ସଂରଚନା repository *r,
				     ସଂରଚନା object_id *out,
				     ଅଚଳ ସଂରଚନା object_id *oid,
				     ସଂରଚନା commit_list *parents)
{
	size_t author_len, committer_len;
	ସଂରଚନା commit *this;
	ଅଚଳ ଅକ୍ଷର *orig_author, *orig_committer;
	ଅକ୍ଷର *author = ଖାଲି, *committer = ଖାଲି;
	ଅଚଳ ଅକ୍ଷର *buffer;
	unsigned long bufsize;
	ଅଚଳ ଅକ୍ଷର *p;
	ସଂରଚନା strbuf ସନ୍ଦେଶ = STRBUF_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;
	ସଂରଚନା ident_split id;

	this = lookup_commit_reference(r, oid);
	buffer = repo_get_commit_buffer(r, this, &bufsize);
	orig_author = find_commit_header(buffer, "author", &author_len);
	orig_committer = find_commit_header(buffer, "committer", &committer_len);

	ଯଦି (!orig_author || !orig_committer) {
		ret = error(_("cannot parse commit %s"), oid_to_hex(oid));
		ଯାଅ out;
	}

	ଯଦି (split_ident_ଧାଡ଼ି(&id, orig_author, author_len) < 0 ||
	    split_ident_ଧାଡ଼ି(&id, orig_committer, committer_len) < 0) {
		ret = error(_("invalid author or committer for %s"), oid_to_hex(oid));
		ଯାଅ out;
	}

	p = strstr(buffer, "\n\n");
	strbuf_addstr(&ସନ୍ଦେଶ, "git stash: ");

	ଯଦି (p)
		strbuf_add(&ସନ୍ଦେଶ, p + 2, bufsize - (p + 2 - buffer));
	strbuf_complete_ଧାଡ଼ି(&ସନ୍ଦେଶ);

	author = xmemdupz(orig_author, author_len);
	committer = xmemdupz(orig_committer, committer_len);

	ଯଦି (commit_ବୃକ୍ଷ_extended(ସନ୍ଦେଶ.buf, ସନ୍ଦେଶ.len,
				 r->hash_algo->empty_ବୃକ୍ଷ, parents,
				 out, author, committer,
				 ଖାଲି, ଖାଲି)) {
		ret = error(_("could not write commit"));
		ଯାଅ out;
	}
out:
	strbuf_release(&ସନ୍ଦେଶ);
	repo_unuse_commit_buffer(r, this, buffer);
	ମୁକ୍ତ(author);
	ମୁକ୍ତ(committer);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା do_import_stash(ସଂରଚନା repository *r, ଅଚଳ ଅକ୍ଷର *rev)
{
	ସଂରଚନା object_id chain;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା res = 0;
	ଅଚଳ ଅକ୍ଷର *buffer = ଖାଲି;
	unsigned long bufsize;
	ସଂରଚନା commit *this = ଖାଲି;
	ସଂରଚନା commit_list *ବସ୍ତୁଗୁଡ଼ିକ = ଖାଲି, *cur;
	ଅକ୍ଷର *ସନ୍ଦେଶ = ଖାଲି;

	ଯଦି (repo_get_oid(r, rev, &chain))
		ଫେରନ୍ତୁ error(_("not a valid revision: %s"), rev);

	this = lookup_commit_reference(r, &chain);
	ଯଦି (!this)
		ଫେରନ୍ତୁ error(_("not a commit: %s"), rev);

	/*
	 * Walk the commit history, finding each stash entry, and load data into
	 * the array.
	 */
	ପାଇଁ (;;) {
		ଅଚଳ ଅକ୍ଷର *author, *committer;
		size_t author_len, committer_len;
		ଅଚଳ ଅକ୍ଷର *p;
		ଅଚଳ ଅକ୍ଷର *expected = "git stash <git@stash> 1000684800 +0000";
		ଅଚଳ ଅକ୍ଷର *prefix = "git stash: ";
		ସଂରଚନା commit *stash;
		ସଂରଚନା ବୃକ୍ଷ *ବୃକ୍ଷ = repo_get_commit_ବୃକ୍ଷ(r, this);

		ଯଦି (!ବୃକ୍ଷ ||
		    !oideq(&ବୃକ୍ଷ->object.oid, r->hash_algo->empty_ବୃକ୍ଷ) ||
		    (this->parents &&
		     (!this->parents->next || this->parents->next->next))) {
			res = error(_("%s is not a valid exported stash commit"),
					oid_to_hex(&this->object.oid));
			ଯାଅ out;
		}

		buffer = repo_get_commit_buffer(r, this, &bufsize);

		ଯଦି (!this->parents) {
			/*
			 * We don't have any parents.  Make sure this is our
			 * root commit.
			 */
			author = find_commit_header(buffer, "author", &author_len);
			committer = find_commit_header(buffer, "committer", &committer_len);

			ଯଦି (!author || !committer) {
				error(_("cannot parse commit %s"), oid_to_hex(&this->object.oid));
				ଯାଅ out;
			}

			ଯଦି (author_len != strlen(expected) ||
			    committer_len != strlen(expected) ||
			    memcmp(author, expected, author_len) ||
			    memcmp(committer, expected, committer_len)) {
				res = error(_("found root commit %s with invalid data"), oid_to_hex(&this->object.oid));
				ଯାଅ out;
			}
			ଭାଙ୍ଗ;
		}

		p = strstr(buffer, "\n\n");
		ଯଦି (!p) {
			res = error(_("cannot parse commit %s"), oid_to_hex(&this->object.oid));
			ଯାଅ out;
		}

		p += 2;
		ଯଦି (((size_t)(bufsize - (p - buffer)) < strlen(prefix)) ||
		    memcmp(prefix, p, strlen(prefix))) {
			res = error(_("found stash commit %s without expected prefix"), oid_to_hex(&this->object.oid));
			ଯାଅ out;
		}

		stash = this->parents->next->item;

		ଯଦି (repo_parse_commit(r, this->parents->item) ||
		    repo_parse_commit(r, stash)) {
			res = error(_("cannot parse parents of commit: %s"),
					oid_to_hex(&this->object.oid));
			ଯାଅ out;
		}

		ଯଦି (check_stash_topology(r, stash)) {
			res = error(_("%s does not look like a stash commit"),
					oid_to_hex(&stash->object.oid));
			ଯାଅ out;
		}

		repo_unuse_commit_buffer(r, this, buffer);
		buffer = ଖାଲି;
		ବସ୍ତୁଗୁଡ଼ିକ = commit_list_ଭର୍ତ୍ତି(stash, &ବସ୍ତୁଗୁଡ଼ିକ);
		this = this->parents->item;
	}

	/*
	 * Now, walk each entry, adding it to the stash as a normal stash
	 * commit.
	 */
	ପାଇଁ (cur = ବସ୍ତୁଗୁଡ଼ିକ; cur; cur = cur->next) {
		ଅଚଳ ଅକ୍ଷର *p;
		ସଂରଚନା object_id *oid;

		this = cur->item;
		oid = &this->object.oid;
		buffer = repo_get_commit_buffer(r, this, &bufsize);
		ଯଦି (!buffer) {
			res = error(_("cannot read commit buffer for %s"), oid_to_hex(oid));
			ଯାଅ out;
		}

		p = strstr(buffer, "\n\n");
		ଯଦି (!p) {
			res = error(_("cannot parse commit %s"), oid_to_hex(oid));
			ଯାଅ out;
		}

		p += 2;
		ସନ୍ଦେଶ = xmemdupz(p, bufsize - (p - buffer));
		repo_unuse_commit_buffer(r, this, buffer);
		buffer = ଖାଲି;

		ଯଦି (do_store_stash(oid, ସନ୍ଦେଶ, 1)) {
			res = error(_("cannot save the stash for %s"), oid_to_hex(oid));
			ଯାଅ out;
		}
		FREE_AND_NULL(ସନ୍ଦେଶ);
	}
out:
	ଯଦି (this && buffer)
		repo_unuse_commit_buffer(r, this, buffer);
	commit_list_free(ବସ୍ତୁଗୁଡ଼ିକ);
	ମୁକ୍ତ(ସନ୍ଦେଶ);

	ଫେରନ୍ତୁ res;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା import_stash(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
			ସଂରଚନା repository *repo)
{
	ସଂରଚନା option options[] = {
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_import_usage,
			     PARSE_OPT_KEEP_DASHDASH);

	ଯଦି (argc != 1)
		usage_ସନ୍ଦେଶ_opt("a revision is required", git_stash_import_usage, options);

	ଫେରନ୍ତୁ do_import_stash(repo, argv[0]);
}

ସଂରଚନା stash_entry_data {
	ସଂରଚନା repository *r;
	ସଂରଚନା commit_list **ବସ୍ତୁଗୁଡ଼ିକ;
	size_t ଗଣନା_2;
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା collect_stash_entries(ଅଚଳ ଅକ୍ଷର *refname UNUSED,
				 ସଂରଚନା object_id *old_oid UNUSED,
				 ସଂରଚନା object_id *new_oid,
				 ଅଚଳ ଅକ୍ଷର *committer UNUSED,
				 timestamp_t timestamp UNUSED,
				 ପୂର୍ଣ୍ଣସଂଖ୍ୟା tz UNUSED, ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ UNUSED,
				 ଶୂନ୍ୟ *cb_data)
{
	ସଂରଚନା stash_entry_data *data = cb_data;
	ସଂରଚନା commit *stash;

	data->ଗଣନା_2++;
	stash = lookup_commit_reference(data->r, new_oid);
	ଯଦି (!stash || check_stash_topology(data->r, stash)) {
		ଫେରନ୍ତୁ error(_("%s does not look like a stash commit"),
			     oid_to_hex(new_oid));
	}
	data->ବସ୍ତୁଗୁଡ଼ିକ = commit_list_append(stash, data->ବସ୍ତୁଗୁଡ଼ିକ);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା do_export_stash(ସଂରଚନା repository *r,
			   ଅଚଳ ଅକ୍ଷର *ref,
			   ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
			   ଅଚଳ ଅକ୍ଷର **argv)
{
	ସଂରଚନା object_id base;
	ସଂରଚନା commit *prev;
	ସଂରଚନା commit_list *ବସ୍ତୁଗୁଡ଼ିକ = ଖାଲି, **iter = &ବସ୍ତୁଗୁଡ଼ିକ, *cur;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା res = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	ସଂରଚନା strbuf revision = STRBUF_INIT;
	ଅଚଳ ଅକ୍ଷର *author, *committer;

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
	ଯଦି (commit_ବୃକ୍ଷ_extended("", 0, r->hash_algo->empty_ବୃକ୍ଷ, ଖାଲି,
				 &base, author, committer, ଖାଲି, ଖାଲି))
		ଫେରନ୍ତୁ error(_("unable to write base commit"));

	prev = lookup_commit_reference(r, &base);

	ଯଦି (argc) {
		/*
		 * Find each specified stash, and load data into the array.
		 */
		ପାଇଁ (i = 0; i < argc; i++) {
			ସଂରଚନା object_id oid;
			ସଂରଚନା commit *stash;

			ଯଦି (parse_stash_revision(&revision, argv[i], 1) ||
			    repo_get_oid_with_flags(r, revision.buf, &oid,
						    GET_OID_QUIETLY |
						    GET_OID_GENTLY)) {
				res = error(_("unable to find stash entry %s"), argv[i]);
				ଯାଅ out;
			}

			stash = lookup_commit_reference(r, &oid);
			ଯଦି (!stash || check_stash_topology(r, stash)) {
				res = error(_("%s does not look like a stash commit"),
						revision.buf);
				ଯାଅ out;
			}
			iter = commit_list_append(stash, iter);
		}
	} ଅନ୍ୟଥା {
		/*
		 * Walk the reflog, finding each stash entry, and load data into the
		 * array.
		 */
		ସଂରଚନା stash_entry_data cb_data = {
			.r = r, .ବସ୍ତୁଗୁଡ଼ିକ = iter,
		};
		ଯଦି (refs_for_each_reflog_ent_reverse(get_main_ref_store(r),
						     "refs/stash",
						     collect_stash_entries,
						     &cb_data) && cb_data.ଗଣନା_2)
			ଯାଅ out;
	}

	/*
	 * Now, create a set of commits identical to the regular stash commits,
	 * but where their first parents form a chain to our original empty
	 * base commit.
	 */
	ବସ୍ତୁଗୁଡ଼ିକ = commit_list_reverse(ବସ୍ତୁଗୁଡ଼ିକ);
	ପାଇଁ (cur = ବସ୍ତୁଗୁଡ଼ିକ; cur; cur = cur->next) {
		ସଂରଚନା commit_list *parents = ଖାଲି;
		ସଂରଚନା commit_list **next = &parents;
		ସଂରଚନା object_id out;
		ସଂରଚନା commit *stash = cur->item;

		next = commit_list_append(prev, next);
		next = commit_list_append(stash, next);
		res = write_commit_with_parents(r, &out, &stash->object.oid, parents);
		commit_list_free(parents);
		ଯଦି (res)
			ଯାଅ out;
		prev = lookup_commit_reference(r, &out);
	}
	ଯଦି (ref)
		refs_update_ref(get_main_ref_store(r), ଖାଲି, ref,
				&prev->object.oid, ଖାଲି, 0, UPDATE_REFS_DIE_ON_ERR);
	ଅନ୍ୟଥା
		puts(oid_to_hex(&prev->object.oid));
out:
	strbuf_release(&revision);
	commit_list_free(ବସ୍ତୁଗୁଡ଼ିକ);

	ଫେରନ୍ତୁ res;
}

ଗଣନା export_action {
	ACTION_NONE,
	ACTION_PRINT,
	ACTION_TO_REF,
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା export_stash(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
			ଅଚଳ ଅକ୍ଷର **argv,
			ଅଚଳ ଅକ୍ଷର *prefix,
			ସଂରଚନା repository *repo)
{
	ଅଚଳ ଅକ୍ଷର *ref = ଖାଲି;
	ଗଣନା export_action action = ACTION_NONE;
	ସଂରଚନା option options[] = {
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

	ଯଦି (ref && action == ACTION_NONE)
		action = ACTION_TO_REF;

	ଯଦି (action == ACTION_NONE || (ref && action == ACTION_PRINT))
		ଫେରନ୍ତୁ error(_("exactly one of --print and --to-ref is required"));

	ଫେରନ୍ତୁ do_export_stash(repo, ref, argc, argv);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_stash(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
	      ଅଚଳ ଅକ୍ଷର **argv,
	      ଅଚଳ ଅକ୍ଷର *prefix,
	      ସଂରଚନା repository *repo)
{
	pid_t pid = getpid();
	ଅଚଳ ଅକ୍ଷର *index_file;
	ସଂରଚନା strvec args = STRVEC_INIT;
	parse_opt_subcommand_fn *fn = ଖାଲି;
	ସଂରଚନା option options[] = {
		OPT_SUBCOMMAND("apply", &fn, apply_stash),
		OPT_SUBCOMMAND("clear", &fn, clear_stash),
		OPT_SUBCOMMAND("drop", &fn, drop_stash),
		OPT_SUBCOMMAND("pop", &fn, କାଢ଼_stash),
		OPT_SUBCOMMAND("branch", &fn, branch_stash),
		OPT_SUBCOMMAND("list", &fn, list_stash),
		OPT_SUBCOMMAND("show", &fn, show_stash),
		OPT_SUBCOMMAND("store", &fn, store_stash),
		OPT_SUBCOMMAND("create", &fn, create_stash),
		OPT_SUBCOMMAND("push", &fn, ଠେଲ_stash_unassumed),
		OPT_SUBCOMMAND("export", &fn, export_stash),
		OPT_SUBCOMMAND("import", &fn, import_stash),
		OPT_SUBCOMMAND_F("save", &fn, save_stash, ଖାଲି, PARSE_OPT_NOCOMPLETE),
		OPT_END()
	};
	ଅଚଳ ଅକ୍ଷର **args_copy;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	repo_config(the_repository, git_stash_config, ଖାଲି);

	argc = parse_options(argc, argv, prefix, options, git_stash_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL |
			     PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	index_file = repo_get_index_file(the_repository);
	strbuf_addf(&stash_index_path, "%s.stash.%" PRIuMAX, index_file,
		    (uintmax_t)pid);

	ଯଦି (fn) {
		ret = fn(argc, argv, prefix, repo);

		/*
		 * The subcommand implementations return 0 on success, a
		 * negative value on failure, and STASH_APPLY_CONFLICT
		 * when applying a stash entry resulted in conflicts.
		 * Map failures to 128, the status die() uses, so that
		 * exit status 1 unambiguously indicates conflicts.
		 */
		ଯଦି (ret < 0)
			ଫେରନ୍ତୁ 128;
		ଫେରନ୍ତୁ ret;
	} ଅନ୍ୟଥା ଯଦି (!argc) {
		ଫେରନ୍ତୁ !!ଠେଲ_stash_unassumed(0, ଖାଲି, prefix, repo);
	}

	/* Assume 'stash push' */
	strvec_ଠେଲ(&args, "push");
	strvec_pushv(&args, argv);

	/*
	 * `push_stash()` ends up modifying the array, which causes memory
	 * leaks if we didn't copy the array here.
	 */
	DUP_ARRAY(args_copy, args.v, args.nr);

	ret = !!ଠେଲ_stash(args.nr, args_copy, prefix, 1);

	strvec_clear(&args);
	ମୁକ୍ତ(args_copy);
	ଫେରନ୍ତୁ ret;
}
