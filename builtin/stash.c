#define USE_THE_REPOSITORY_VARIABLE

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "abspath.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "hash.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "object-name.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "refs.h"
#ಅನ್ತರ್ಭಾವ "stash.h"
#ಅನ್ತರ್ಭಾವ "lockfile.h"
#ಅನ್ತರ್ಭಾವ "cache-tree.h"
#ಅನ್ತರ್ಭಾವ "unpack-trees.h"
#ಅನ್ತರ್ಭಾವ "merge-ort-wrappers.h"
#ಅನ್ತರ್ಭಾವ "strvec.h"
#ಅನ್ತರ್ಭಾವ "run-command.h"
#ಅನ್ತರ್ಭಾವ "dir.h"
#ಅನ್ತರ್ಭಾವ "entry.h"
#ಅನ್ತರ್ಭಾವ "preload-index.h"
#ಅನ್ತರ್ಭಾವ "read-cache.h"
#ಅನ್ತರ್ಭಾವ "repository.h"
#ಅನ್ತರ್ಭಾವ "rerere.h"
#ಅನ್ತರ್ಭಾವ "revision.h"
#ಅನ್ತರ್ಭಾವ "setup.h"
#ಅನ್ತರ್ಭಾವ "sparse-index.h"
#ಅನ್ತರ್ಭಾವ "log-tree.h"
#ಅನ್ತರ್ಭಾವ "diffcore.h"
#ಅನ್ತರ್ಭಾವ "reflog.h"
#ಅನ್ತರ್ಭಾವ "reflog-walk.h"
#ಅನ್ತರ್ಭಾವ "add-interactive.h"
#ಅನ್ತರ್ಭಾವ "commit.h"

#define INCLUDE_ALL_FILES 2

#define BUILTIN_STASH_LIST_USAGE \
	N_("git stash list [<log-options>]")
#define BUILTIN_STASH_SHOW_USAGE \
	N_("git stash show [-u | --include-untracked | --only-untracked] [<diff-options>] [<stash>]")
#define BUILTIN_STASH_DROP_USAGE \
	N_("git stash drop [-q | --quiet] [<stash>]")
#define BUILTIN_STASH_ತೆಗೆ_USAGE \
	N_("git stash pop [--index] [-q | --quiet] [<stash>]")
#define BUILTIN_STASH_APPLY_USAGE \
	N_("git stash apply [--index] [-q | --quiet] [--label-ours=<label>] [--label-theirs=<label>] [--label-base=<label>] [<stash>]")
#define BUILTIN_STASH_BRANCH_USAGE \
	N_("git stash branch <branchname> [<stash>]")
#define BUILTIN_STASH_STORE_USAGE \
	N_("git stash store [(-m | --message) <message>] [-q | --quiet] <commit>")
#define BUILTIN_STASH_ತಳ್ಳು_USAGE \
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

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ git_stash_usage[] = {
	BUILTIN_STASH_LIST_USAGE,
	BUILTIN_STASH_SHOW_USAGE,
	BUILTIN_STASH_DROP_USAGE,
	BUILTIN_STASH_ತೆಗೆ_USAGE,
	BUILTIN_STASH_APPLY_USAGE,
	BUILTIN_STASH_BRANCH_USAGE,
	BUILTIN_STASH_ತಳ್ಳು_USAGE,
	BUILTIN_STASH_SAVE_USAGE,
	BUILTIN_STASH_CLEAR_USAGE,
	BUILTIN_STASH_CREATE_USAGE,
	BUILTIN_STASH_STORE_USAGE,
	BUILTIN_STASH_EXPORT_USAGE,
	BUILTIN_STASH_IMPORT_USAGE,
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ git_stash_list_usage[] = {
	BUILTIN_STASH_LIST_USAGE,
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ git_stash_show_usage[] = {
	BUILTIN_STASH_SHOW_USAGE,
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ git_stash_drop_usage[] = {
	BUILTIN_STASH_DROP_USAGE,
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ git_stash_ತೆಗೆ_usage[] = {
	BUILTIN_STASH_ತೆಗೆ_USAGE,
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ git_stash_apply_usage[] = {
	BUILTIN_STASH_APPLY_USAGE,
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ git_stash_branch_usage[] = {
	BUILTIN_STASH_BRANCH_USAGE,
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ git_stash_clear_usage[] = {
	BUILTIN_STASH_CLEAR_USAGE,
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ git_stash_store_usage[] = {
	BUILTIN_STASH_STORE_USAGE,
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ git_stash_ತಳ್ಳು_usage[] = {
	BUILTIN_STASH_ತಳ್ಳು_USAGE,
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ git_stash_save_usage[] = {
	BUILTIN_STASH_SAVE_USAGE,
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ git_stash_export_usage[] = {
	BUILTIN_STASH_EXPORT_USAGE,
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ git_stash_import_usage[] = {
	BUILTIN_STASH_IMPORT_USAGE,
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ ref_stash[] = "refs/stash";
ಸ್ಥಿರ ಸಂರಚನಾ strbuf stash_index_path = STRBUF_INIT;

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_stat = 1;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_patch;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_include_untracked;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ use_index;

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
ಸಂರಚನಾ stash_info {
	ಸಂರಚನಾ object_id w_commit;
	ಸಂರಚನಾ object_id b_commit;
	ಸಂರಚನಾ object_id i_commit;
	ಸಂರಚನಾ object_id u_commit;
	ಸಂರಚನಾ object_id c_commit;
	ಸಂರಚನಾ object_id w_ವೃಕ್ಷ;
	ಸಂರಚನಾ object_id b_ವೃಕ್ಷ;
	ಸಂರಚನಾ object_id i_ವೃಕ್ಷ;
	ಸಂರಚನಾ object_id u_ವೃಕ್ಷ;
	ಸಂರಚನಾ strbuf revision;
	ಪೂರ್ಣಾಙ್ಕ is_stash_ref;
	ಪೂರ್ಣಾಙ್ಕ has_u;
};

#define STASH_INFO_INIT { \
	.revision = STRBUF_INIT, \
}

ಸ್ಥಿರ ಶೂನ್ಯ free_stash_info(ಸಂರಚನಾ stash_info *info)
{
	strbuf_release(&info->revision);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ check_stash_topology(ಸಂರಚನಾ repository *r, ಸಂರಚನಾ commit *stash)
{
	ಸಂರಚನಾ commit *p1, *p2, *p3 = ರಿಕ್ತ;

	/* stash must have two or three parents */
	ಯದಿ (!stash->parents || !stash->parents->next ||
			(stash->parents->next->next && stash->parents->next->next->next))
		ಪ್ರತ್ಯಾಗಮ -1;
	p1 = stash->parents->item;
	p2 = stash->parents->next->item;
	ಯದಿ (stash->parents->next->next)
		p3 = stash->parents->next->next->item;
	ಯದಿ (repo_parse_commit(r, p1) || repo_parse_commit(r, p2) ||
			(p3 && repo_parse_commit(r, p3)))
		ಪ್ರತ್ಯಾಗಮ -1;
	/* p2 must have a single parent, p3 must have no parents */
	ಯದಿ (!p2->parents || p2->parents->next || (p3 && p3->parents))
		ಪ್ರತ್ಯಾಗಮ -1;
	ಯದಿ (repo_parse_commit(r, p2->parents->item))
		ಪ್ರತ್ಯಾಗಮ -1;
	/* p2^1 must equal p1 */
	ಯದಿ (!oideq(&p1->object.oid, &p2->parents->item->object.oid))
		ಪ್ರತ್ಯಾಗಮ -1;

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ assert_stash_like(ಸಂರಚನಾ stash_info *info, ಧ್ರುವ ವರ್ಣ *revision)
{
	ಯದಿ (get_oidf(&info->b_commit, "%s^1", revision) ||
	    get_oidf(&info->w_ವೃಕ್ಷ, "%s:", revision) ||
	    get_oidf(&info->b_ವೃಕ್ಷ, "%s^1:", revision) ||
	    get_oidf(&info->i_ವೃಕ್ಷ, "%s^2:", revision))
		die(_("'%s' is not a stash-like commit"), revision);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ parse_stash_revision(ಸಂರಚನಾ strbuf *revision, ಧ್ರುವ ವರ್ಣ *commit, ಪೂರ್ಣಾಙ್ಕ quiet)
{
	strbuf_reset(revision);
	ಯದಿ (!commit) {
		ಯದಿ (!refs_ref_exists(get_main_ref_store(the_repository), ref_stash)) {
			ಯದಿ (!quiet)
				fprintf_ln(stderr, _("No stash entries found."));
			ಪ್ರತ್ಯಾಗಮ -1;
		}

		strbuf_addf(revision, "%s@{0}", ref_stash);
	} ಅನ್ಯಥಾ ಯದಿ (strspn(commit, "0123456789") == strlen(commit)) {
		strbuf_addf(revision, "%s@{%s}", ref_stash, commit);
	} ಅನ್ಯಥಾ {
		strbuf_addstr(revision, commit);
	}
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ get_stash_info(ಸಂರಚನಾ stash_info *info, ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv)
{
	ಪೂರ್ಣಾಙ್ಕ ret;
	ವರ್ಣ *end_of_rev;
	ವರ್ಣ *expanded_ref;
	ಧ್ರುವ ವರ್ಣ *revision;
	ಧ್ರುವ ವರ್ಣ *commit = ರಿಕ್ತ;
	ಸಂರಚನಾ object_id dummy;
	ಸಂರಚನಾ strbuf symbolic = STRBUF_INIT;

	ಯದಿ (argc > 1) {
		ಪೂರ್ಣಾಙ್ಕ i;
		ಸಂರಚನಾ strbuf refs_ಸಂದೇಶ = STRBUF_INIT;

		ಪ್ರತಿ (i = 0; i < argc; i++)
			strbuf_addf(&refs_ಸಂದೇಶ, " '%s'", argv[i]);

		fprintf_ln(stderr, _("Too many revisions specified:%s"),
			   refs_ಸಂದೇಶ.buf);
		strbuf_release(&refs_ಸಂದೇಶ);

		ಪ್ರತ್ಯಾಗಮ -1;
	}

	ಯದಿ (argc == 1)
		commit = argv[0];

	strbuf_init(&info->revision, 0);
	ಯದಿ (parse_stash_revision(&info->revision, commit, 0)) {
		ಪ್ರತ್ಯಾಗಮ -1;
	}

	revision = info->revision.buf;

	ಯದಿ (repo_get_oid(the_repository, revision, &info->w_commit))
		ಪ್ರತ್ಯಾಗಮ error(_("%s is not a valid reference"), revision);

	assert_stash_like(info, revision);

	info->has_u = !get_oidf(&info->u_ವೃಕ್ಷ, "%s^3:", revision);

	end_of_rev = strchrnul(revision, '@');
	strbuf_add(&symbolic, revision, end_of_rev - revision);

	ret = repo_dwim_ref(the_repository, symbolic.buf, symbolic.len,
			    &dummy, &expanded_ref, 0);
	strbuf_release(&symbolic);
	ವಿಕಲ್ಪ (ret) {
	ಪಕ್ಷ 0: /* Not found, but valid ref */
		info->is_stash_ref = 0;
		ಭಙ್ಗ;
	ಪಕ್ಷ 1:
		info->is_stash_ref = !strcmp(expanded_ref, ref_stash);
		ಭಙ್ಗ;
	ಸ್ವಭಾವ: /* Invalid or ambiguous */
		ಭಙ್ಗ;
	}

	ಮುಕ್ತ(expanded_ref);
	ಪ್ರತ್ಯಾಗಮ !(ret == 0 || ret == 1);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ do_clear_stash(ಶೂನ್ಯ)
{
	ಸಂರಚನಾ object_id obj;
	ಯದಿ (repo_get_oid(the_repository, ref_stash, &obj))
		ಪ್ರತ್ಯಾಗಮ 0;

	ಪ್ರತ್ಯಾಗಮ refs_delete_ref(get_main_ref_store(the_repository), ರಿಕ್ತ,
			       ref_stash, &obj, 0);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ clear_stash(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
		       ಸಂರಚನಾ repository *repo UNUSED)
{
	ಸಂರಚನಾ option options[] = {
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_clear_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	ಯದಿ (argc)
		ಪ್ರತ್ಯಾಗಮ error(_("git stash clear with arguments is "
			       "unimplemented"));

	ಪ್ರತ್ಯಾಗಮ do_clear_stash();
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ reset_ವೃಕ್ಷ(ಸಂರಚನಾ object_id *i_ವೃಕ್ಷ, ಪೂರ್ಣಾಙ್ಕ update, ಪೂರ್ಣಾಙ್ಕ reset)
{
	ಪೂರ್ಣಾಙ್ಕ nr_trees = 1;
	ಸಂರಚನಾ unpack_trees_options opts;
	ಸಂರಚನಾ ವೃಕ್ಷ_desc t[MAX_UNPACK_TREES];
	ಸಂರಚನಾ ವೃಕ್ಷ *ವೃಕ್ಷ;
	ಸಂರಚನಾ lock_file lock_file = LOCK_INIT;

	repo_read_index_preload(the_repository, ರಿಕ್ತ, 0);
	ಯದಿ (refresh_index(the_repository->index, REFRESH_QUIET, ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ))
		ಪ್ರತ್ಯಾಗಮ -1;

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);

	memset(&opts, 0, ಪರಿಮಾಣ(opts));

	ವೃಕ್ಷ = repo_parse_ವೃಕ್ಷ_indirect(the_repository, i_ವೃಕ್ಷ);
	ಯದಿ (repo_parse_ವೃಕ್ಷ(the_repository, ವೃಕ್ಷ))
		ಪ್ರತ್ಯಾಗಮ -1;

	init_ವೃಕ್ಷ_desc(t, &ವೃಕ್ಷ->object.oid, ವೃಕ್ಷ->buffer, ವೃಕ್ಷ->size);

	opts.head_idx = 1;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;
	opts.merge = 1;
	opts.reset = reset ? UNPACK_RESET_PROTECT_UNTRACKED : 0;
	opts.update = update;
	ಯದಿ (update)
		opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
	opts.fn = oneway_merge;

	ಯದಿ (unpack_trees(nr_trees, t, &opts))
		ಪ್ರತ್ಯಾಗಮ -1;

	ಯದಿ (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
		ಪ್ರತ್ಯಾಗಮ error(_("unable to write new index file"));

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ create_index_from_ವೃಕ್ಷ(ಧ್ರುವ ಸಂರಚನಾ object_id *ವೃಕ್ಷ_id,
				  ಧ್ರುವ ವರ್ಣ *index_path)
{
	ಪೂರ್ಣಾಙ್ಕ nr_trees = 1;
	ಪೂರ್ಣಾಙ್ಕ ret = 0;
	ಸಂರಚನಾ unpack_trees_options opts;
	ಸಂರಚನಾ ವೃಕ್ಷ_desc t[MAX_UNPACK_TREES];
	ಸಂರಚನಾ ವೃಕ್ಷ *ವೃಕ್ಷ;
	ಸಂರಚನಾ index_state dst_istate = INDEX_STATE_INIT(the_repository);
	ಸಂರಚನಾ lock_file lock_file = LOCK_INIT;

	repo_read_index_preload(the_repository, ರಿಕ್ತ, 0);
	refresh_index(the_repository->index, REFRESH_QUIET, ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ);

	hold_lock_file_for_update(&lock_file, index_path, LOCK_DIE_ON_ERROR);

	memset(&opts, 0, ಪರಿಮಾಣ(opts));

	ವೃಕ್ಷ = repo_parse_ವೃಕ್ಷ_indirect(the_repository, ವೃಕ್ಷ_id);
	ಯದಿ (!ವೃಕ್ಷ || repo_parse_ವೃಕ್ಷ(the_repository, ವೃಕ್ಷ)) {
		ret = -1;
		ಗಚ್ಛ done;
	}

	init_ವೃಕ್ಷ_desc(t, &ವೃಕ್ಷ->object.oid, ವೃಕ್ಷ->buffer, ವೃಕ್ಷ->size);

	opts.head_idx = 1;
	opts.src_index = the_repository->index;
	opts.dst_index = &dst_istate;
	opts.merge = 1;
	opts.reset = UNPACK_RESET_PROTECT_UNTRACKED;
	opts.fn = oneway_merge;

	ಯದಿ (unpack_trees(nr_trees, t, &opts)) {
		ret = -1;
		ಗಚ್ಛ done;
	}

	ಯದಿ (write_locked_index(&dst_istate, &lock_file, COMMIT_LOCK)) {
		ret = error(_("unable to write new index file"));
		ಗಚ್ಛ done;
	}

done:
	release_index(&dst_istate);
	ಯದಿ (ret)
		rollback_lock_file(&lock_file);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ is_path_a_directory(ಧ್ರುವ ವರ್ಣ *path)
{
	/*
	 * This function differs from abspath.c:is_directory() in that
	 * here we use lstat() instead of stat(); we do not want to
	 * follow symbolic links here.
	 */
	ಸಂರಚನಾ stat st;
	ಪ್ರತ್ಯಾಗಮ (!lstat(path, &st) && S_ISDIR(st.st_mode));
}

ಸ್ಥಿರ ಶೂನ್ಯ add_diff_to_buf(ಸಂರಚನಾ diff_queue_struct *q,
			    ಸಂರಚನಾ diff_options *options UNUSED,
			    ಶೂನ್ಯ *data)
{
	ಪೂರ್ಣಾಙ್ಕ i;

	ಪ್ರತಿ (i = 0; i < q->nr; i++) {
		ಯದಿ (is_path_a_directory(q->queue[i]->one->path))
			ಅನುವರ್ತ;

		strbuf_addstr(data, q->queue[i]->one->path);

		/* NUL-terminate: will be fed to update-index -z */
		strbuf_addch(data, '\0');
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ restore_untracked(ಸಂರಚನಾ object_id *u_ವೃಕ್ಷ)
{
	ಪೂರ್ಣಾಙ್ಕ res;
	ಸಂರಚನಾ child_process cp = CHILD_PROCESS_INIT;

	/*
	 * We need to run restore files from a given index, but without
	 * affecting the current index, so we use GIT_INDEX_FILE with
	 * run_command to fork processes that will not interfere.
	 */
	cp.git_cmd = 1;
	strvec_ತಳ್ಳು(&cp.args, "read-tree");
	strvec_ತಳ್ಳು(&cp.args, oid_to_hex(u_ವೃಕ್ಷ));
	strvec_pushf(&cp.env, "GIT_INDEX_FILE=%s",
		     stash_index_path.buf);
	ಯದಿ (run_command(&cp)) {
		remove_path(stash_index_path.buf);
		ಪ್ರತ್ಯಾಗಮ -1;
	}

	child_process_init(&cp);
	cp.git_cmd = 1;
	strvec_pushl(&cp.args, "checkout-index", "--all", ರಿಕ್ತ);
	strvec_pushf(&cp.env, "GIT_INDEX_FILE=%s",
		     stash_index_path.buf);

	res = run_command(&cp);
	remove_path(stash_index_path.buf);
	ಪ್ರತ್ಯಾಗಮ res;
}

ಸ್ಥಿರ ಶೂನ್ಯ unstage_changes_unless_new(ಸಂರಚನಾ object_id *orig_ವೃಕ್ಷ)
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

	ಸಂರಚನಾ checkout state = CHECKOUT_INIT;
	ಸಂರಚನಾ diff_options diff_opts;
	ಸಂರಚನಾ lock_file lock = LOCK_INIT;
	ಪೂರ್ಣಾಙ್ಕ i;

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

	do_diff_cache(orig_ವೃಕ್ಷ, &diff_opts);
	diffcore_std(&diff_opts);

	/* Iterate over the paths that changed due to the merge... */
	ಪ್ರತಿ (i = 0; i < diff_queued_diff.nr; i++) {
		ಸಂರಚನಾ diff_filepair *p;
		ಸಂರಚನಾ cache_entry *ce;
		ಪೂರ್ಣಾಙ್ಕ pos;

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
		ಯದಿ (pos >= 0 && ce_skip_worktree(the_repository->index->cache[pos])) {
			ಸಂರಚನಾ stat st;

			ce = the_repository->index->cache[pos];
			ಯದಿ (!lstat(ce->name, &st)) {
				/* Conflicting path present; relocate it */
				ಸಂರಚನಾ strbuf new_path = STRBUF_INIT;
				ಪೂರ್ಣಾಙ್ಕ fd;

				strbuf_addf(&new_path,
					    "%s.stash.XXXXXX", ce->name);
				fd = xmkstemp(new_path.buf);
				close(fd);
				printf(_("WARNING: Untracked file in way of "
					 "tracked file!  Renaming\n "
					 "           %s -> %s\n"
					 "         to make room.\n"),
				       ce->name, new_path.buf);
				ಯದಿ (rename(ce->name, new_path.buf))
					die("Failed to move %s to %s",
					    ce->name, new_path.buf);
				strbuf_release(&new_path);
			}
			checkout_entry(ce, &state, ರಿಕ್ತ, ರಿಕ್ತ);
			ce->ce_flags &= ~CE_SKIP_WORKTREE;
		}

		/*
		 * Step 3: "unstage" changes, as long as they are still tracked
		 */
		ಯದಿ (p->one->oid_valid) {
			/*
			 * Path existed in orig_tree; restore index entry
			 * from that tree in order to "unstage" the changes.
			 */
			ಪೂರ್ಣಾಙ್ಕ option = ADD_CACHE_OK_TO_REPLACE;
			ಯದಿ (pos < 0)
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
	ಯದಿ (write_locked_index(the_repository->index, &lock,
			       COMMIT_LOCK | SKIP_IF_UNCHANGED))
		die(_("could not write index"));
}

ಸ್ಥಿರ ಗಣನಾ stash_apply_result do_apply_stash(ಧ್ರುವ ವರ್ಣ *prefix,
					      ಸಂರಚನಾ stash_info *info,
					      ಪೂರ್ಣಾಙ್ಕ index, ಪೂರ್ಣಾಙ್ಕ quiet,
					      ಧ್ರುವ ವರ್ಣ *label_ours,
					      ಧ್ರುವ ವರ್ಣ *label_theirs,
					      ಧ್ರುವ ವರ್ಣ *label_base)
{
	ಪೂರ್ಣಾಙ್ಕ clean, ret;
	ಪೂರ್ಣಾಙ್ಕ has_index = index;
	ಸಂರಚನಾ merge_options o;
	ಸಂರಚನಾ object_id c_ವೃಕ್ಷ;
	ಸಂರಚನಾ object_id index_ವೃಕ್ಷ;
	ಸಂರಚನಾ ವೃಕ್ಷ *head, *merge, *merge_base;
	ಸಂರಚನಾ lock_file lock = LOCK_INIT;

	repo_read_index_preload(the_repository, ರಿಕ್ತ, 0);
	ಯದಿ (repo_refresh_and_write_index(the_repository, REFRESH_QUIET, 0, 0,
					 ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ))
		ಪ್ರತ್ಯಾಗಮ error(_("could not write index"));

	ಯದಿ (write_index_as_ವೃಕ್ಷ(&c_ವೃಕ್ಷ, the_repository->index,
				repo_get_index_file(the_repository), 0, ರಿಕ್ತ))
		ಪ್ರತ್ಯಾಗಮ error(_("cannot apply a stash in the middle of a merge"));

	init_ui_merge_options(&o, the_repository);

	ಯದಿ (quiet)
		o.verbosity = 0;

	ಯದಿ (index) {
		ಯದಿ (oideq(&info->b_ವೃಕ್ಷ, &info->i_ವೃಕ್ಷ) ||
		    oideq(&c_ವೃಕ್ಷ, &info->i_ವೃಕ್ಷ)) {
			has_index = 0;
		} ಅನ್ಯಥಾ {
			ಸಂರಚನಾ merge_result result = { 0 };

			o.branch1 = "Current index";
			o.branch2 = "Stashed index changes";
			o.ancestor = "Stash base";

			head = lookup_ವೃಕ್ಷ(o.repo, &c_ವೃಕ್ಷ);
			merge = lookup_ವೃಕ್ಷ(o.repo, &info->i_ವೃಕ್ಷ);
			merge_base = lookup_ವೃಕ್ಷ(o.repo, &info->b_ವೃಕ್ಷ);

			merge_incore_nonrecursive(&o, merge_base, head, merge,
						  &result);

			ಯದಿ (result.clean < 0) {
				merge_finalize(&o, &result);
				ಪ್ರತ್ಯಾಗಮ error(_("index merge failed"));
			} ಅನ್ಯಥಾ ಯದಿ (!result.clean) {
				merge_finalize(&o, &result);
				ಪ್ರತ್ಯಾಗಮ error(_("conflicts in index. "
					       "Try without --index."));
			} ಅನ್ಯಥಾ {
				oidcpy(&index_ವೃಕ್ಷ, &result.ವೃಕ್ಷ->object.oid);
				merge_finalize(&o, &result);
			}
		}
	}

	o.branch1 = label_ours ? label_ours : "Updated upstream";
	o.branch2 = label_theirs ? label_theirs : "Stashed changes";
	o.ancestor = label_base ? label_base : "Stash base";

	ಯದಿ (oideq(&info->b_ವೃಕ್ಷ, &c_ವೃಕ್ಷ))
		o.branch1 = "Version stash was based on";

	ಯದಿ (o.verbosity >= 3)
		printf_ln(_("Merging %s with %s"), o.branch1, o.branch2);

	head = lookup_ವೃಕ್ಷ(o.repo, &c_ವೃಕ್ಷ);
	merge = lookup_ವೃಕ್ಷ(o.repo, &info->w_ವೃಕ್ಷ);
	merge_base = lookup_ವೃಕ್ಷ(o.repo, &info->b_ವೃಕ್ಷ);

	repo_hold_locked_index(o.repo, &lock, LOCK_DIE_ON_ERROR);
	clean = merge_ort_nonrecursive(&o, head, merge, merge_base);

	/*
	 * If 'clean' >= 0, reverse the value for 'ret' so 'ret' is 0 when the
	 * merge was clean, and 1 if the merge was unclean or a negative value
	 * if it encountered an error.
	 */
	ret = clean >= 0 ? !clean : clean;

	ಯದಿ (ret < 0)
		rollback_lock_file(&lock);
	ಅನ್ಯಥಾ ಯದಿ (write_locked_index(o.repo->index, &lock,
				      COMMIT_LOCK | SKIP_IF_UNCHANGED))
		ret = error(_("could not write index"));

	ಯದಿ (ret) {
		repo_rerere(the_repository, 0);

		ಯದಿ (index)
			fprintf_ln(stderr, _("Index was not unstashed."));

		ಗಚ್ಛ restore_untracked;
	}

	ಯದಿ (has_index) {
		ಯದಿ (reset_ವೃಕ್ಷ(&index_ವೃಕ್ಷ, 0, 0))
			ret = -1;
	} ಅನ್ಯಥಾ {
		unstage_changes_unless_new(&c_ವೃಕ್ಷ);
	}

restore_untracked:
	ಯದಿ (info->has_u && restore_untracked(&info->u_ವೃಕ್ಷ))
		ret = error(_("could not restore untracked files from stash"));

	ಯದಿ (!quiet) {
		ಸಂರಚನಾ child_process cp = CHILD_PROCESS_INIT;

		/*
		 * Status is quite simple and could be replaced with calls to
		 * wt_status in the future, but it adds complexities which may
		 * require more tests.
		 */
		cp.git_cmd = 1;
		cp.dir = prefix;
		strvec_pushf(&cp.env, GIT_WORK_ವೃಕ್ಷ_ENVIRONMENT"=%s",
			     absolute_path(repo_get_work_ವೃಕ್ಷ(the_repository)));
		strvec_pushf(&cp.env, GIT_DIR_ENVIRONMENT"=%s",
			     absolute_path(repo_get_git_dir(the_repository)));
		strvec_ತಳ್ಳು(&cp.args, "status");
		run_command(&cp);
	}

	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ apply_stash(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
		       ಸಂರಚನಾ repository *repo UNUSED)
{
	ಪೂರ್ಣಾಙ್ಕ ret = -1;
	ಪೂರ್ಣಾಙ್ಕ quiet = 0;
	ಪೂರ್ಣಾಙ್ಕ index = use_index;
	ಧ್ರುವ ವರ್ಣ *label_ours = ರಿಕ್ತ, *label_theirs = ರಿಕ್ತ, *label_base = ರಿಕ್ತ;
	ಸಂರಚನಾ stash_info info = STASH_INFO_INIT;
	ಸಂರಚನಾ option options[] = {
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

	ಯದಿ (get_stash_info(&info, argc, argv))
		ಗಚ್ಛ cleanup;

	ret = do_apply_stash(prefix, &info, index, quiet,
			     label_ours, label_theirs, label_base);
cleanup:
	free_stash_info(&info);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ reject_reflog_ent(ಧ್ರುವ ವರ್ಣ *refname UNUSED,
			     ಸಂರಚನಾ object_id *ooid UNUSED,
			     ಸಂರಚನಾ object_id *noid UNUSED,
			     ಧ್ರುವ ವರ್ಣ *email UNUSED,
			     timestamp_t timestamp UNUSED,
			     ಪೂರ್ಣಾಙ್ಕ tz UNUSED, ಧ್ರುವ ವರ್ಣ *message UNUSED,
			     ಶೂನ್ಯ *cb_data UNUSED)
{
	ಪ್ರತ್ಯಾಗಮ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ reflog_is_empty(ಧ್ರುವ ವರ್ಣ *refname)
{
	ಪ್ರತ್ಯಾಗಮ !refs_for_each_reflog_ent(get_main_ref_store(the_repository),
					 refname, reject_reflog_ent, ರಿಕ್ತ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ do_drop_stash(ಸಂರಚನಾ stash_info *info, ಪೂರ್ಣಾಙ್ಕ quiet)
{
	ಯದಿ (!reflog_delete(info->revision.buf,
			   EXPIRE_REFLOGS_REWRITE | EXPIRE_REFLOGS_UPDATE_REF,
			   0)) {
		ಯದಿ (!quiet)
			printf_ln(_("Dropped %s (%s)"), info->revision.buf,
				  oid_to_hex(&info->w_commit));
	} ಅನ್ಯಥಾ {
		ಪ್ರತ್ಯಾಗಮ error(_("%s: Could not drop stash entry"),
			     info->revision.buf);
	}

	ಯದಿ (reflog_is_empty(ref_stash))
		do_clear_stash();

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ get_stash_info_assert(ಸಂರಚನಾ stash_info *info, ಪೂರ್ಣಾಙ್ಕ argc,
				 ಧ್ರುವ ವರ್ಣ **argv)
{
	ಪೂರ್ಣಾಙ್ಕ ret = get_stash_info(info, argc, argv);

	ಯದಿ (ret < 0)
		ಪ್ರತ್ಯಾಗಮ ret;

	ಯದಿ (!info->is_stash_ref)
		ಪ್ರತ್ಯಾಗಮ error(_("'%s' is not a stash reference"), info->revision.buf);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ drop_stash(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
		      ಸಂರಚನಾ repository *repo UNUSED)
{
	ಪೂರ್ಣಾಙ್ಕ ret = -1;
	ಪೂರ್ಣಾಙ್ಕ quiet = 0;
	ಸಂರಚನಾ stash_info info = STASH_INFO_INIT;
	ಸಂರಚನಾ option options[] = {
		OPT__QUIET(&quiet, N_("be quiet, only report errors")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_drop_usage, 0);

	ಯದಿ (get_stash_info_assert(&info, argc, argv))
		ಗಚ್ಛ cleanup;

	ret = do_drop_stash(&info, quiet);
cleanup:
	free_stash_info(&info);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ ತೆಗೆ_stash(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
		     ಸಂರಚನಾ repository *repo UNUSED)
{
	ಪೂರ್ಣಾಙ್ಕ ret = -1;
	ಪೂರ್ಣಾಙ್ಕ index = use_index;
	ಪೂರ್ಣಾಙ್ಕ quiet = 0;
	ಸಂರಚನಾ stash_info info = STASH_INFO_INIT;
	ಸಂರಚನಾ option options[] = {
		OPT__QUIET(&quiet, N_("be quiet, only report errors")),
		OPT_BOOL(0, "index", &index,
			 N_("attempt to recreate the index")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_ತೆಗೆ_usage, 0);

	ಯದಿ (get_stash_info_assert(&info, argc, argv))
		ಗಚ್ಛ cleanup;

	ಯದಿ ((ret = do_apply_stash(prefix, &info, index, quiet,
				  ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ)))
		printf_ln(_("The stash entry is kept in case "
			    "you need it again."));
	ಅನ್ಯಥಾ
		ret = do_drop_stash(&info, quiet);

cleanup:
	free_stash_info(&info);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ branch_stash(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
			ಸಂರಚನಾ repository *repo UNUSED)
{
	ಪೂರ್ಣಾಙ್ಕ ret = -1;
	ಧ್ರುವ ವರ್ಣ *branch = ರಿಕ್ತ;
	ಸಂರಚನಾ stash_info info = STASH_INFO_INIT;
	ಸಂರಚನಾ child_process cp = CHILD_PROCESS_INIT;
	ಸಂರಚನಾ option options[] = {
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_branch_usage, 0);

	ಯದಿ (!argc) {
		fprintf_ln(stderr, _("No branch name specified"));
		ಪ್ರತ್ಯಾಗಮ -1;
	}

	branch = argv[0];

	ಯದಿ (get_stash_info(&info, argc - 1, argv + 1))
		ಗಚ್ಛ cleanup;

	cp.git_cmd = 1;
	strvec_pushl(&cp.args, "checkout", "-b", ರಿಕ್ತ);
	strvec_ತಳ್ಳು(&cp.args, branch);
	strvec_ತಳ್ಳು(&cp.args, oid_to_hex(&info.b_commit));
	ret = run_command(&cp);
	ಯದಿ (!ret)
		ret = do_apply_stash(prefix, &info, 1, 0,
				     ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ);
	ಯದಿ (!ret && info.is_stash_ref)
		ret = do_drop_stash(&info, 0);

cleanup:
	free_stash_info(&info);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ list_stash(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
		      ಸಂರಚನಾ repository *repo UNUSED)
{
	ಸಂರಚನಾ child_process cp = CHILD_PROCESS_INIT;
	ಸಂರಚನಾ option options[] = {
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_list_usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT);

	ಯದಿ (!refs_ref_exists(get_main_ref_store(the_repository), ref_stash))
		ಪ್ರತ್ಯಾಗಮ 0;

	cp.git_cmd = 1;
	strvec_pushl(&cp.args, "log", "--format=%gd: %gs", "-g",
		     "--first-parent", ರಿಕ್ತ);
	strvec_pushv(&cp.args, argv);
	strvec_ತಳ್ಳು(&cp.args, ref_stash);
	strvec_ತಳ್ಳು(&cp.args, "--");
	ಪ್ರತ್ಯಾಗಮ run_command(&cp);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ git_stash_config(ಧ್ರುವ ವರ್ಣ *var, ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ,
			    ಧ್ರುವ ಸಂರಚನಾ config_context *ctx, ಶೂನ್ಯ *cb)
{
	ಯದಿ (!strcmp(var, "stash.showstat")) {
		show_stat = git_config_bool(var, ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!strcmp(var, "stash.showpatch")) {
		show_patch = git_config_bool(var, ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!strcmp(var, "stash.showincludeuntracked")) {
		show_include_untracked = git_config_bool(var, ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!strcmp(var, "stash.index")) {
		use_index = git_config_bool(var, ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಪ್ರತ್ಯಾಗಮ git_diff_basic_config(var, ಮೌಲ್ಯ, ctx, cb);
}

ಸ್ಥಿರ ಶೂನ್ಯ diff_include_untracked(ಧ್ರುವ ಸಂರಚನಾ stash_info *info, ಸಂರಚನಾ diff_options *diff_opt)
{
	ಧ್ರುವ ಸಂರಚನಾ object_id *oid[] = { &info->w_commit, &info->u_ವೃಕ್ಷ };
	ಸಂರಚನಾ ವೃಕ್ಷ *ವೃಕ್ಷ[ARRAY_SIZE(oid)];
	ಸಂರಚನಾ ವೃಕ್ಷ_desc ವೃಕ್ಷ_desc[ARRAY_SIZE(oid)];
	ಸಂರಚನಾ unpack_trees_options unpack_ವೃಕ್ಷ_opt = { 0 };

	ಪ್ರತಿ (size_t i = 0; i < ARRAY_SIZE(oid); i++) {
		ವೃಕ್ಷ[i] = repo_parse_ವೃಕ್ಷ_indirect(the_repository, oid[i]);
		ಯದಿ (repo_parse_ವೃಕ್ಷ(the_repository, ವೃಕ್ಷ[i]) < 0)
			die(_("failed to parse tree"));
		init_ವೃಕ್ಷ_desc(&ವೃಕ್ಷ_desc[i], &ವೃಕ್ಷ[i]->object.oid,
			       ವೃಕ್ಷ[i]->buffer, ವೃಕ್ಷ[i]->size);
	}

	unpack_ವೃಕ್ಷ_opt.head_idx = -1;
	unpack_ವೃಕ್ಷ_opt.src_index = the_repository->index;
	unpack_ವೃಕ್ಷ_opt.dst_index = the_repository->index;
	unpack_ವೃಕ್ಷ_opt.merge = 1;
	unpack_ವೃಕ್ಷ_opt.fn = stash_worktree_untracked_merge;

	ಯದಿ (unpack_trees(ARRAY_SIZE(ವೃಕ್ಷ_desc), ವೃಕ್ಷ_desc, &unpack_ವೃಕ್ಷ_opt))
		die(_("failed to unpack trees"));

	do_diff_cache(&info->b_commit, diff_opt);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_stash(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
		      ಸಂರಚನಾ repository *repo UNUSED)
{
	ಪೂರ್ಣಾಙ್ಕ i;
	ಪೂರ್ಣಾಙ್ಕ ret = -1;
	ಸಂರಚನಾ stash_info info = STASH_INFO_INIT;
	ಸಂರಚನಾ rev_info rev;
	ಸಂರಚನಾ strvec stash_args = STRVEC_INIT;
	ಸಂರಚನಾ strvec revision_args = STRVEC_INIT;
	ಗಣನಾ {
		UNTRACKED_NONE,
		UNTRACKED_INCLUDE,
		UNTRACKED_ONLY
	} show_untracked = show_include_untracked ? UNTRACKED_INCLUDE : UNTRACKED_NONE;
	ಸಂರಚನಾ option options[] = {
		OPT_SET_INT('u', "include-untracked", &show_untracked,
			    N_("include untracked files in the stash"),
			    UNTRACKED_INCLUDE),
		OPT_SET_INT_F(0, "only-untracked", &show_untracked,
			      N_("only show untracked files in the stash"),
			      UNTRACKED_ONLY, PARSE_OPT_NONEG),
		OPT_END()
	};
	ಪೂರ್ಣಾಙ್ಕ do_usage = 0;

	init_diff_ui_defaults();
	repo_config(the_repository, git_diff_ui_config, ರಿಕ್ತ);
	repo_init_revisions(the_repository, &rev, prefix);

	argc = parse_options(argc, argv, prefix, options, git_stash_show_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	strvec_ತಳ್ಳು(&revision_args, argv[0]);
	ಪ್ರತಿ (i = 1; i < argc; i++) {
		ಯದಿ (argv[i][0] != '-')
			strvec_ತಳ್ಳು(&stash_args, argv[i]);
		ಅನ್ಯಥಾ
			strvec_ತಳ್ಳು(&revision_args, argv[i]);
	}

	ಯದಿ (get_stash_info(&info, stash_args.nr, stash_args.v))
		ಗಚ್ಛ cleanup;

	/*
	 * The config settings are applied only if there are not passed
	 * any options.
	 */
	ಯದಿ (revision_args.nr == 1) {
		ಯದಿ (show_stat)
			rev.diffopt.output_format = DIFF_FORMAT_DIFFSTAT;

		ಯದಿ (show_patch)
			rev.diffopt.output_format |= DIFF_FORMAT_PATCH;

		ಯದಿ (!show_stat && !show_patch) {
			ret = 0;
			ಗಚ್ಛ cleanup;
		}
	}

	setup_revisions_from_strvec(&revision_args, &rev, ರಿಕ್ತ);
	ಯದಿ (revision_args.nr > 1)
		ಗಚ್ಛ usage;
	ಯದಿ (!rev.diffopt.output_format) {
		rev.diffopt.output_format = DIFF_FORMAT_PATCH;
		diff_setup_done(&rev.diffopt);
	}

	rev.diffopt.flags.recursive = 1;
	setup_diff_pager(&rev.diffopt);
	ವಿಕಲ್ಪ (show_untracked) {
	ಪಕ್ಷ UNTRACKED_NONE:
		diff_ವೃಕ್ಷ_oid(&info.b_commit, &info.w_commit, "", &rev.diffopt);
		ಭಙ್ಗ;
	ಪಕ್ಷ UNTRACKED_ONLY:
		ಯದಿ (info.has_u)
			diff_ಬೇರು_ವೃಕ್ಷ_oid(&info.u_ವೃಕ್ಷ, "", &rev.diffopt);
		ಭಙ್ಗ;
	ಪಕ್ಷ UNTRACKED_INCLUDE:
		ಯದಿ (info.has_u)
			diff_include_untracked(&info, &rev.diffopt);
		ಅನ್ಯಥಾ
			diff_ವೃಕ್ಷ_oid(&info.b_commit, &info.w_commit, "", &rev.diffopt);
		ಭಙ್ಗ;
	}
	log_ವೃಕ್ಷ_diff_flush(&rev);

	ret = diff_result_code(&rev);

cleanup:
	strvec_clear(&revision_args);
	strvec_clear(&stash_args);
	free_stash_info(&info);
	release_revisions(&rev);
	ಯದಿ (do_usage)
		usage_with_options(git_stash_show_usage, options);
	ಪ್ರತ್ಯಾಗಮ ret;
usage:
	do_usage = 1;
	ಗಚ್ಛ cleanup;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ do_store_stash(ಧ್ರುವ ಸಂರಚನಾ object_id *w_commit, ಧ್ರುವ ವರ್ಣ *stash_ಸಂದೇಶ,
			  ಪೂರ್ಣಾಙ್ಕ quiet)
{
	ಸಂರಚನಾ stash_info info;
	ವರ್ಣ revision[GIT_MAX_HEXSZ];

	oid_to_hex_r(revision, w_commit);
	assert_stash_like(&info, revision);

	ಯದಿ (!stash_ಸಂದೇಶ)
		stash_ಸಂದೇಶ = "Created via \"git stash store\".";

	ಯದಿ (refs_update_ref(get_main_ref_store(the_repository), stash_ಸಂದೇಶ, ref_stash, w_commit, ರಿಕ್ತ,
			    REF_FORCE_CREATE_REFLOG,
			    quiet ? UPDATE_REFS_QUIET_ON_ERR :
			    UPDATE_REFS_ಸಂದೇಶ_ON_ERR)) {
		ಯದಿ (!quiet) {
			fprintf_ln(stderr, _("Cannot update %s with %s"),
				   ref_stash, oid_to_hex(w_commit));
		}
		ಪ್ರತ್ಯಾಗಮ -1;
	}

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ store_stash(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
		       ಸಂರಚನಾ repository *repo UNUSED)
{
	ಪೂರ್ಣಾಙ್ಕ quiet = 0;
	ಧ್ರುವ ವರ್ಣ *stash_ಸಂದೇಶ = ರಿಕ್ತ;
	ಸಂರಚನಾ object_id obj;
	ಸಂರಚನಾ option options[] = {
		OPT__QUIET(&quiet, N_("be quiet")),
		OPT_STRING('m', "message", &stash_ಸಂದೇಶ, "message",
			   N_("stash message")),
		OPT_END()
	};
	ಪೂರ್ಣಾಙ್ಕ ret;

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_store_usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT);

	ಯದಿ (argc != 1) {
		ಯದಿ (!quiet)
			fprintf_ln(stderr, _("\"git stash store\" requires one "
					     "<commit> argument"));
		ಪ್ರತ್ಯಾಗಮ -1;
	}

	ಯದಿ (repo_get_oid_with_flags(the_repository, argv[0], &obj,
				    quiet ? GET_OID_QUIETLY : 0)) {
		ಯದಿ (!quiet)
			fprintf_ln(stderr, _("Cannot update %s with %s"),
					     ref_stash, argv[0]);
		ret = -1;
		ಗಚ್ಛ out;
	}

	ret = do_store_stash(&obj, stash_ಸಂದೇಶ, quiet);

out:
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಶೂನ್ಯ add_pathspecs(ಸಂರಚನಾ strvec *args,
			  ಧ್ರುವ ಸಂರಚನಾ pathspec *ps) {
	ಪೂರ್ಣಾಙ್ಕ i;

	ಪ್ರತಿ (i = 0; i < ps->nr; i++)
		strvec_ತಳ್ಳು(args, ps->ವಸ್ತುಗಳು[i].original);
}

/*
 * `untracked_files` will be filled with the names of untracked files.
 * The return value is:
 *
 * = 0 if there are not any untracked files
 * > 0 if there are untracked files
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ get_untracked_files(ಧ್ರುವ ಸಂರಚನಾ pathspec *ps, ಪೂರ್ಣಾಙ್ಕ include_untracked,
			       ಸಂರಚನಾ strbuf *untracked_files)
{
	ಪೂರ್ಣಾಙ್ಕ i;
	ಪೂರ್ಣಾಙ್ಕ found = 0;
	ಸಂರಚನಾ dir_struct dir = DIR_INIT;

	ಯದಿ (include_untracked != INCLUDE_ALL_FILES)
		setup_standard_excludes(&dir);

	fill_directory(&dir, the_repository->index, ps);
	ಪ್ರತಿ (i = 0; i < dir.nr; i++) {
		ಸಂರಚನಾ dir_entry *ent = dir.entries[i];
		found++;
		strbuf_addstr(untracked_files, ent->name);
		/* NUL-terminate: will be fed to update-index -z */
		strbuf_addch(untracked_files, '\0');
	}

	dir_clear(&dir);
	ಪ್ರತ್ಯಾಗಮ found;
}

/*
 * The return value of `check_changes_tracked_files()` can be:
 *
 * < 0 if there was an error
 * = 0 if there are no changes.
 * > 0 if there are changes.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ check_changes_tracked_files(ಧ್ರುವ ಸಂರಚನಾ pathspec *ps)
{
	ಸಂರಚನಾ rev_info rev;
	ಸಂರಚನಾ object_id dummy;
	ಪೂರ್ಣಾಙ್ಕ ret = 0;

	/* No initial commit. */
	ಯದಿ (repo_get_oid(the_repository, "HEAD", &dummy))
		ಪ್ರತ್ಯಾಗಮ -1;

	ಯದಿ (repo_read_index(the_repository) < 0)
		ಪ್ರತ್ಯಾಗಮ -1;

	repo_init_revisions(the_repository, &rev, ರಿಕ್ತ);
	copy_pathspec(&rev.prune_data, ps);

	rev.diffopt.flags.quick = 1;
	rev.diffopt.flags.ignore_submodules = 1;
	rev.abbrev = 0;

	add_head_to_pending(&rev);
	diff_setup_done(&rev.diffopt);

	run_diff_index(&rev, DIFF_INDEX_CACHED);
	ಯದಿ (diff_result_code(&rev)) {
		ret = 1;
		ಗಚ್ಛ done;
	}

	run_diff_files(&rev, 0);
	ಯದಿ (diff_result_code(&rev)) {
		ret = 1;
		ಗಚ್ಛ done;
	}

done:
	release_revisions(&rev);
	ಪ್ರತ್ಯಾಗಮ ret;
}

/*
 * The function will fill `untracked_files` with the names of untracked files
 * It will return 1 if there were any changes and 0 if there were not.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ check_changes(ಧ್ರುವ ಸಂರಚನಾ pathspec *ps, ಪೂರ್ಣಾಙ್ಕ include_untracked,
			 ಸಂರಚನಾ strbuf *untracked_files)
{
	ಪೂರ್ಣಾಙ್ಕ ret = 0;
	ಯದಿ (check_changes_tracked_files(ps))
		ret = 1;

	ಯದಿ (include_untracked && get_untracked_files(ps, include_untracked,
						     untracked_files))
		ret = 1;

	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ save_untracked_files(ಸಂರಚನಾ stash_info *info, ಸಂರಚನಾ strbuf *ಸಂದೇಶ,
				ಸಂರಚನಾ strbuf *files)
{
	ಪೂರ್ಣಾಙ್ಕ ret = 0;
	ಸಂರಚನಾ strbuf untracked_ಸಂದೇಶ = STRBUF_INIT;
	ಸಂರಚನಾ child_process cp_upd_index = CHILD_PROCESS_INIT;
	ಸಂರಚನಾ index_state istate = INDEX_STATE_INIT(the_repository);

	cp_upd_index.git_cmd = 1;
	strvec_pushl(&cp_upd_index.args, "update-index", "-z", "--add",
		     "--remove", "--stdin", ರಿಕ್ತ);
	strvec_pushf(&cp_upd_index.env, "GIT_INDEX_FILE=%s",
			 stash_index_path.buf);

	strbuf_addf(&untracked_ಸಂದೇಶ, "untracked files on %s\n", ಸಂದೇಶ->buf);
	ಯದಿ (pipe_command(&cp_upd_index, files->buf, files->len, ರಿಕ್ತ, 0,
			 ರಿಕ್ತ, 0)) {
		ret = -1;
		ಗಚ್ಛ done;
	}

	ಯದಿ (write_index_as_ವೃಕ್ಷ(&info->u_ವೃಕ್ಷ, &istate, stash_index_path.buf, 0,
				ರಿಕ್ತ)) {
		ret = -1;
		ಗಚ್ಛ done;
	}

	ಯದಿ (commit_ವೃಕ್ಷ(untracked_ಸಂದೇಶ.buf, untracked_ಸಂದೇಶ.len,
			&info->u_ವೃಕ್ಷ, ರಿಕ್ತ, &info->u_commit, ರಿಕ್ತ, ರಿಕ್ತ)) {
		ret = -1;
		ಗಚ್ಛ done;
	}

done:
	release_index(&istate);
	strbuf_release(&untracked_ಸಂದೇಶ);
	remove_path(stash_index_path.buf);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ stash_staged(ಸಂರಚನಾ stash_info *info, ಸಂರಚನಾ strbuf *out_patch,
			ಪೂರ್ಣಾಙ್ಕ quiet)
{
	ಪೂರ್ಣಾಙ್ಕ ret = 0;
	ಸಂರಚನಾ child_process cp_diff_ವೃಕ್ಷ = CHILD_PROCESS_INIT;
	ಸಂರಚನಾ index_state istate = INDEX_STATE_INIT(the_repository);

	ಯದಿ (write_index_as_ವೃಕ್ಷ(&info->w_ವೃಕ್ಷ, &istate, the_repository->index_file,
				0, ರಿಕ್ತ)) {
		ret = -1;
		ಗಚ್ಛ done;
	}

	cp_diff_ವೃಕ್ಷ.git_cmd = 1;
	strvec_pushl(&cp_diff_ವೃಕ್ಷ.args, "diff-tree", "-p", "--binary",
		     "--no-color",
		     "-U1", "HEAD", oid_to_hex(&info->w_ವೃಕ್ಷ), "--", ರಿಕ್ತ);
	ಯದಿ (pipe_command(&cp_diff_ವೃಕ್ಷ, ರಿಕ್ತ, 0, out_patch, 0, ರಿಕ್ತ, 0)) {
		ret = -1;
		ಗಚ್ಛ done;
	}

	ಯದಿ (!out_patch->len) {
		ಯದಿ (!quiet)
			fprintf_ln(stderr, _("No staged changes"));
		ret = 1;
	}

done:
	release_index(&istate);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ stash_patch(ಸಂರಚನಾ stash_info *info, ಧ್ರುವ ಸಂರಚನಾ pathspec *ps,
		       ಸಂರಚನಾ strbuf *out_patch, ಪೂರ್ಣಾಙ್ಕ quiet,
		       ಸಂರಚನಾ interactive_options *interactive_opts)
{
	ಪೂರ್ಣಾಙ್ಕ ret = 0;
	ಸಂರಚನಾ child_process cp_diff_ವೃಕ್ಷ = CHILD_PROCESS_INIT;
	ಸಂರಚನಾ commit *head_commit;
	ಧ್ರುವ ಸಂರಚನಾ object_id *head_ವೃಕ್ಷ;
	ಸಂರಚನಾ index_state istate = INDEX_STATE_INIT(the_repository);
	ವರ್ಣ *old_index_env = ರಿಕ್ತ, *old_repo_index_file;

	remove_path(stash_index_path.buf);

	head_commit = lookup_commit(the_repository, &info->b_commit);
	ಯದಿ (!head_commit || repo_parse_commit(the_repository, head_commit)) {
		ret = -1;
		ಗಚ್ಛ done;
	}
	head_ವೃಕ್ಷ = get_commit_ವೃಕ್ಷ_oid(head_commit);
	ಯದಿ (!head_ವೃಕ್ಷ) {
		ret = -1;
		ಗಚ್ಛ done;
	}

	ಯದಿ (create_index_from_ವೃಕ್ಷ(head_ವೃಕ್ಷ, stash_index_path.buf)) {
		ret = -1;
		ಗಚ್ಛ done;
	}

	/* Find out what the user wants. */
	old_repo_index_file = the_repository->index_file;
	the_repository->index_file = stash_index_path.buf;
	old_index_env = xstrdup_or_null(getenv(INDEX_ENVIRONMENT));
	setenv(INDEX_ENVIRONMENT, the_repository->index_file, 1);

	ret = !!run_add_p(the_repository, ADD_P_STASH, interactive_opts, ರಿಕ್ತ, ps, 0);

	the_repository->index_file = old_repo_index_file;
	ಯದಿ (old_index_env && *old_index_env)
		setenv(INDEX_ENVIRONMENT, old_index_env, 1);
	ಅನ್ಯಥಾ
		unsetenv(INDEX_ENVIRONMENT);
	FREE_AND_NULL(old_index_env);

	/* State of the working tree. */
	ಯದಿ (write_index_as_ವೃಕ್ಷ(&info->w_ವೃಕ್ಷ, &istate, stash_index_path.buf, 0,
				ರಿಕ್ತ)) {
		ret = -1;
		ಗಚ್ಛ done;
	}

	cp_diff_ವೃಕ್ಷ.git_cmd = 1;
	strvec_pushl(&cp_diff_ವೃಕ್ಷ.args, "diff-tree", "-p", "-U1", "HEAD",
		     "--no-color",
		     oid_to_hex(&info->w_ವೃಕ್ಷ), "--", ರಿಕ್ತ);
	ಯದಿ (pipe_command(&cp_diff_ವೃಕ್ಷ, ರಿಕ್ತ, 0, out_patch, 0, ರಿಕ್ತ, 0)) {
		ret = -1;
		ಗಚ್ಛ done;
	}

	ಯದಿ (!out_patch->len) {
		ಯದಿ (!quiet)
			fprintf_ln(stderr, _("No changes selected"));
		ret = 1;
	}

done:
	release_index(&istate);
	remove_path(stash_index_path.buf);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ stash_working_ವೃಕ್ಷ(ಸಂರಚನಾ stash_info *info, ಧ್ರುವ ಸಂರಚನಾ pathspec *ps)
{
	ಪೂರ್ಣಾಙ್ಕ ret = 0;
	ಸಂರಚನಾ rev_info rev;
	ಸಂರಚನಾ child_process cp_upd_index = CHILD_PROCESS_INIT;
	ಸಂರಚನಾ strbuf diff_output = STRBUF_INIT;
	ಸಂರಚನಾ index_state istate = INDEX_STATE_INIT(the_repository);

	repo_init_revisions(the_repository, &rev, ರಿಕ್ತ);
	copy_pathspec(&rev.prune_data, ps);

	set_alternate_index_output(stash_index_path.buf);
	ಯದಿ (reset_ವೃಕ್ಷ(&info->i_ವೃಕ್ಷ, 0, 0)) {
		ret = -1;
		ಗಚ್ಛ done;
	}
	set_alternate_index_output(ರಿಕ್ತ);

	rev.diffopt.output_format = DIFF_FORMAT_CALLBACK;
	rev.diffopt.format_callback = add_diff_to_buf;
	rev.diffopt.format_callback_data = &diff_output;

	ಯದಿ (repo_read_index_preload(the_repository, &rev.diffopt.pathspec, 0) < 0) {
		ret = -1;
		ಗಚ್ಛ done;
	}

	add_pending_object(&rev, parse_object(the_repository, &info->b_commit),
			   "");
	run_diff_index(&rev, 0);

	cp_upd_index.git_cmd = 1;
	strvec_pushl(&cp_upd_index.args, "update-index",
		     "--ignore-skip-worktree-entries",
		     "-z", "--add", "--remove", "--stdin", ರಿಕ್ತ);
	strvec_pushf(&cp_upd_index.env, "GIT_INDEX_FILE=%s",
		     stash_index_path.buf);

	ಯದಿ (pipe_command(&cp_upd_index, diff_output.buf, diff_output.len,
			 ರಿಕ್ತ, 0, ರಿಕ್ತ, 0)) {
		ret = -1;
		ಗಚ್ಛ done;
	}

	ಯದಿ (write_index_as_ವೃಕ್ಷ(&info->w_ವೃಕ್ಷ, &istate, stash_index_path.buf, 0,
				ರಿಕ್ತ)) {
		ret = -1;
		ಗಚ್ಛ done;
	}

done:
	release_index(&istate);
	release_revisions(&rev);
	strbuf_release(&diff_output);
	remove_path(stash_index_path.buf);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ do_create_stash(ಧ್ರುವ ಸಂರಚನಾ pathspec *ps, ಸಂರಚನಾ strbuf *stash_ಸಂದೇಶ_buf,
			   ಪೂರ್ಣಾಙ್ಕ include_untracked, ಪೂರ್ಣಾಙ್ಕ patch_mode,
			   ಸಂರಚನಾ interactive_options *interactive_opts,
			   ಪೂರ್ಣಾಙ್ಕ only_staged, ಸಂರಚನಾ stash_info *info, ಸಂರಚನಾ strbuf *patch,
			   ಪೂರ್ಣಾಙ್ಕ quiet)
{
	ಪೂರ್ಣಾಙ್ಕ ret = 0;
	ಪೂರ್ಣಾಙ್ಕ flags = 0;
	ಪೂರ್ಣಾಙ್ಕ untracked_commit_option = 0;
	ಧ್ರುವ ವರ್ಣ *head_short_sha1 = ರಿಕ್ತ;
	ಧ್ರುವ ವರ್ಣ *branch_ref = ರಿಕ್ತ;
	ಧ್ರುವ ವರ್ಣ *branch_name = "(no branch)";
	ವರ್ಣ *branch_name_buf = ರಿಕ್ತ;
	ಸಂರಚನಾ commit *head_commit = ರಿಕ್ತ;
	ಸಂರಚನಾ commit_list *parents = ರಿಕ್ತ;
	ಸಂರಚನಾ strbuf ಸಂದೇಶ = STRBUF_INIT;
	ಸಂರಚನಾ strbuf commit_ವೃಕ್ಷ_label = STRBUF_INIT;
	ಸಂರಚನಾ strbuf untracked_files = STRBUF_INIT;

	prepare_fallback_ident("git stash", "git@stash");

	repo_read_index_preload(the_repository, ರಿಕ್ತ, 0);
	ಯದಿ (repo_refresh_and_write_index(the_repository, REFRESH_QUIET, 0, 0,
					 ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ) < 0) {
		ret = error(_("could not write index"));
		ಗಚ್ಛ done;
	}

	ಯದಿ (repo_get_oid(the_repository, "HEAD", &info->b_commit)) {
		ಯದಿ (!quiet)
			fprintf_ln(stderr, _("You do not have "
					     "the initial commit yet"));
		ret = -1;
		ಗಚ್ಛ done;
	} ಅನ್ಯಥಾ {
		head_commit = lookup_commit(the_repository, &info->b_commit);
	}

	ಯದಿ (!check_changes(ps, include_untracked, &untracked_files)) {
		ret = 1;
		ಗಚ್ಛ done;
	}

	branch_ref = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
					     "HEAD", 0, ರಿಕ್ತ, &flags);

	ಯದಿ (flags & REF_ISSYMREF) {
		ಯದಿ (skip_prefix(branch_ref, "refs/heads/", &branch_name))
			branch_name = branch_name_buf = xstrdup(branch_name);
	}

	head_short_sha1 = repo_find_unique_abbrev(the_repository,
						  &head_commit->object.oid,
						  DEFAULT_ABBREV);
	strbuf_addf(&ಸಂದೇಶ, "%s: %s ", branch_name, head_short_sha1);
	pp_commit_easy(CMIT_FMT_ONELINE, head_commit, &ಸಂದೇಶ);

	strbuf_addf(&commit_ವೃಕ್ಷ_label, "index on %s\n", ಸಂದೇಶ.buf);
	commit_list_ಸೇರಿಸು(head_commit, &parents);
	ಯದಿ (write_index_as_ವೃಕ್ಷ(&info->i_ವೃಕ್ಷ, the_repository->index,
				repo_get_index_file(the_repository), 0, ರಿಕ್ತ) ||
	    commit_ವೃಕ್ಷ(commit_ವೃಕ್ಷ_label.buf, commit_ವೃಕ್ಷ_label.len,
			&info->i_ವೃಕ್ಷ, parents, &info->i_commit, ರಿಕ್ತ, ರಿಕ್ತ)) {
		ಯದಿ (!quiet)
			fprintf_ln(stderr, _("Cannot save the current "
					     "index state"));
		ret = -1;
		ಗಚ್ಛ done;
	}

	commit_list_free(parents);
	parents = ರಿಕ್ತ;

	ಯದಿ (include_untracked) {
		ಯದಿ (save_untracked_files(info, &ಸಂದೇಶ, &untracked_files)) {
			ಯದಿ (!quiet)
				fprintf_ln(stderr, _("Cannot save "
						     "the untracked files"));
			ret = -1;
			ಗಚ್ಛ done;
		}
		untracked_commit_option = 1;
	}
	ಯದಿ (patch_mode) {
		ret = stash_patch(info, ps, patch, quiet, interactive_opts);
		ಯದಿ (ret < 0) {
			ಯದಿ (!quiet)
				fprintf_ln(stderr, _("Cannot save the current "
						     "worktree state"));
			ಗಚ್ಛ done;
		} ಅನ್ಯಥಾ ಯದಿ (ret > 0) {
			ಗಚ್ಛ done;
		}
	} ಅನ್ಯಥಾ ಯದಿ (only_staged) {
		ret = stash_staged(info, patch, quiet);
		ಯದಿ (ret < 0) {
			ಯದಿ (!quiet)
				fprintf_ln(stderr, _("Cannot save the current "
						     "staged state"));
			ಗಚ್ಛ done;
		} ಅನ್ಯಥಾ ಯದಿ (ret > 0) {
			ಗಚ್ಛ done;
		}
	} ಅನ್ಯಥಾ {
		ಯದಿ (stash_working_ವೃಕ್ಷ(info, ps)) {
			ಯದಿ (!quiet)
				fprintf_ln(stderr, _("Cannot save the current "
						     "worktree state"));
			ret = -1;
			ಗಚ್ಛ done;
		}
	}

	ಯದಿ (!stash_ಸಂದೇಶ_buf->len)
		strbuf_addf(stash_ಸಂದೇಶ_buf, "WIP on %s", ಸಂದೇಶ.buf);
	ಅನ್ಯಥಾ
		strbuf_insertf(stash_ಸಂದೇಶ_buf, 0, "On %s: ", branch_name);

	ಯದಿ (untracked_commit_option)
		commit_list_ಸೇರಿಸು(lookup_commit(the_repository,
						 &info->u_commit),
				   &parents);
	commit_list_ಸೇರಿಸು(lookup_commit(the_repository, &info->i_commit),
			   &parents);
	commit_list_ಸೇರಿಸು(head_commit, &parents);

	ಯದಿ (commit_ವೃಕ್ಷ(stash_ಸಂದೇಶ_buf->buf, stash_ಸಂದೇಶ_buf->len, &info->w_ವೃಕ್ಷ,
			parents, &info->w_commit, ರಿಕ್ತ, ರಿಕ್ತ)) {
		ಯದಿ (!quiet)
			fprintf_ln(stderr, _("Cannot record "
					     "working tree state"));
		ret = -1;
		ಗಚ್ಛ done;
	}

done:
	strbuf_release(&commit_ವೃಕ್ಷ_label);
	strbuf_release(&ಸಂದೇಶ);
	strbuf_release(&untracked_files);
	commit_list_free(parents);
	ಮುಕ್ತ(branch_name_buf);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ create_stash(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix UNUSED,
			ಸಂರಚನಾ repository *repo UNUSED)
{
	ಪೂರ್ಣಾಙ್ಕ ret;
	ಸಂರಚನಾ strbuf stash_ಸಂದೇಶ_buf = STRBUF_INIT;
	ಸಂರಚನಾ stash_info info = STASH_INFO_INIT;
	ಸಂರಚನಾ pathspec ps;

	/* Starting with argv[1], since argv[0] is "create" */
	strbuf_join_argv(&stash_ಸಂದೇಶ_buf, argc - 1, ++argv, ' ');

	memset(&ps, 0, ಪರಿಮಾಣ(ps));
	ಯದಿ (!check_changes_tracked_files(&ps))
		ಪ್ರತ್ಯಾಗಮ 0;

	ret = do_create_stash(&ps, &stash_ಸಂದೇಶ_buf, 0, 0, ರಿಕ್ತ, 0, &info,
			      ರಿಕ್ತ, 0);
	ಯದಿ (!ret)
		printf_ln("%s", oid_to_hex(&info.w_commit));

	free_stash_info(&info);
	strbuf_release(&stash_ಸಂದೇಶ_buf);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ do_ತಳ್ಳು_stash(ಧ್ರುವ ಸಂರಚನಾ pathspec *ps, ಧ್ರುವ ವರ್ಣ *stash_ಸಂದೇಶ, ಪೂರ್ಣಾಙ್ಕ quiet,
			 ಪೂರ್ಣಾಙ್ಕ keep_index, ಪೂರ್ಣಾಙ್ಕ patch_mode,
			 ಸಂರಚನಾ interactive_options *interactive_opts,
			 ಪೂರ್ಣಾಙ್ಕ include_untracked, ಪೂರ್ಣಾಙ್ಕ only_staged)
{
	ಪೂರ್ಣಾಙ್ಕ ret = 0;
	ಸಂರಚನಾ stash_info info = STASH_INFO_INIT;
	ಸಂರಚನಾ strbuf patch = STRBUF_INIT;
	ಸಂರಚನಾ strbuf stash_ಸಂದೇಶ_buf = STRBUF_INIT;
	ಸಂರಚನಾ strbuf untracked_files = STRBUF_INIT;
	ಸಂರಚನಾ strbuf out = STRBUF_INIT;

	ಯದಿ (patch_mode && keep_index == -1)
		keep_index = 1;

	ಯದಿ (patch_mode && include_untracked) {
		fprintf_ln(stderr, _("Can't use --patch and --include-untracked"
				     " or --all at the same time"));
		ret = -1;
		ಗಚ್ಛ done;
	}

	/* --patch overrides --staged */
	ಯದಿ (patch_mode)
		only_staged = 0;

	ಯದಿ (only_staged && include_untracked) {
		fprintf_ln(stderr, _("Can't use --staged and --include-untracked"
				     " or --all at the same time"));
		ret = -1;
		ಗಚ್ಛ done;
	}

	repo_read_index_preload(the_repository, ರಿಕ್ತ, 0);
	ಯದಿ (!include_untracked && ps->nr) {
		ವರ್ಣ *ps_matched = xcalloc(ps->nr, 1);

		ಯದಿ (pathspec_needs_expanded_index(the_repository->index, ps))
			ensure_full_index(the_repository->index);
		ಪ್ರತಿ (size_t i = 0; i < the_repository->index->cache_nr; i++)
			ce_path_match(the_repository->index, the_repository->index->cache[i], ps,
				      ps_matched);

		ಯದಿ (report_path_error(ps_matched, ps)) {
			fprintf_ln(stderr, _("Did you forget to 'git add'?"));
			ret = -1;
			ಮುಕ್ತ(ps_matched);
			ಗಚ್ಛ done;
		}
		ಮುಕ್ತ(ps_matched);
	}

	ಯದಿ (repo_refresh_and_write_index(the_repository, REFRESH_QUIET, 0, 0,
					 ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ)) {
		ret = error(_("could not write index"));
		ಗಚ್ಛ done;
	}

	ಯದಿ (!check_changes(ps, include_untracked, &untracked_files)) {
		ಯದಿ (!quiet)
			printf_ln(_("No local changes to save"));
		ಗಚ್ಛ done;
	}

	ಯದಿ (!refs_reflog_exists(get_main_ref_store(the_repository), ref_stash) && do_clear_stash()) {
		ret = -1;
		ಯದಿ (!quiet)
			fprintf_ln(stderr, _("Cannot initialize stash"));
		ಗಚ್ಛ done;
	}

	ಯದಿ (stash_ಸಂದೇಶ)
		strbuf_addstr(&stash_ಸಂದೇಶ_buf, stash_ಸಂದೇಶ);
	ಯದಿ (do_create_stash(ps, &stash_ಸಂದೇಶ_buf, include_untracked, patch_mode,
			    interactive_opts, only_staged, &info, &patch, quiet)) {
		ret = -1;
		ಗಚ್ಛ done;
	}

	ಯದಿ (do_store_stash(&info.w_commit, stash_ಸಂದೇಶ_buf.buf, 1)) {
		ret = -1;
		ಯದಿ (!quiet)
			fprintf_ln(stderr, _("Cannot save the current status"));
		ಗಚ್ಛ done;
	}

	ಯದಿ (!quiet)
		printf_ln(_("Saved working directory and index state %s"),
			  stash_ಸಂದೇಶ_buf.buf);

	ಯದಿ (!(patch_mode || only_staged)) {
		ಯದಿ (include_untracked && !ps->nr) {
			ಸಂರಚನಾ child_process cp = CHILD_PROCESS_INIT;

			cp.git_cmd = 1;
			ಯದಿ (startup_info->original_cwd) {
				cp.dir = startup_info->original_cwd;
				strvec_pushf(&cp.env, "%s=%s",
					     GIT_WORK_ವೃಕ್ಷ_ENVIRONMENT,
					     the_repository->worktree);
			}
			strvec_pushl(&cp.args, "clean", "--force",
				     "--quiet", "-d", ":/", ರಿಕ್ತ);
			ಯದಿ (include_untracked == INCLUDE_ALL_FILES)
				strvec_ತಳ್ಳು(&cp.args, "-x");
			ಯದಿ (run_command(&cp)) {
				ret = -1;
				ಗಚ್ಛ done;
			}
		}
		discard_index(the_repository->index);
		ಯದಿ (ps->nr) {
			ಸಂರಚನಾ child_process cp_add = CHILD_PROCESS_INIT;
			ಸಂರಚನಾ child_process cp_diff = CHILD_PROCESS_INIT;
			ಸಂರಚನಾ child_process cp_apply = CHILD_PROCESS_INIT;

			cp_add.git_cmd = 1;
			strvec_ತಳ್ಳು(&cp_add.args, "add");
			ಯದಿ (!include_untracked)
				strvec_ತಳ್ಳು(&cp_add.args, "-u");
			ಯದಿ (include_untracked == INCLUDE_ALL_FILES)
				strvec_ತಳ್ಳು(&cp_add.args, "--force");
			strvec_ತಳ್ಳು(&cp_add.args, "--");
			add_pathspecs(&cp_add.args, ps);
			ಯದಿ (run_command(&cp_add)) {
				ret = -1;
				ಗಚ್ಛ done;
			}

			cp_diff.git_cmd = 1;
			strvec_pushl(&cp_diff.args, "diff-index", "-p",
				     "--no-color",
				     "--cached", "--binary", "HEAD", "--",
				     ರಿಕ್ತ);
			add_pathspecs(&cp_diff.args, ps);
			ಯದಿ (pipe_command(&cp_diff, ರಿಕ್ತ, 0, &out, 0, ರಿಕ್ತ, 0)) {
				ret = -1;
				ಗಚ್ಛ done;
			}

			cp_apply.git_cmd = 1;
			strvec_pushl(&cp_apply.args, "apply", "--index",
				     "-R", ರಿಕ್ತ);
			ಯದಿ (pipe_command(&cp_apply, out.buf, out.len, ರಿಕ್ತ, 0,
					 ರಿಕ್ತ, 0)) {
				ret = -1;
				ಗಚ್ಛ done;
			}
		} ಅನ್ಯಥಾ {
			ಸಂರಚನಾ child_process cp = CHILD_PROCESS_INIT;
			cp.git_cmd = 1;
			/* BUG: this nukes untracked files in the way */
			strvec_pushl(&cp.args, "reset", "--hard", "-q",
				     "--no-recurse-submodules", ರಿಕ್ತ);
			ಯದಿ (run_command(&cp)) {
				ret = -1;
				ಗಚ್ಛ done;
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
		ಯದಿ (keep_index == 1 && !is_null_oid(&info.i_ವೃಕ್ಷ) &&
		    !is_empty_ವೃಕ್ಷ_oid(&info.i_ವೃಕ್ಷ, the_repository->hash_algo)) {
			ಸಂರಚನಾ child_process cp = CHILD_PROCESS_INIT;

			cp.git_cmd = 1;
			strvec_pushl(&cp.args, "checkout", "--no-overlay",
				     oid_to_hex(&info.i_ವೃಕ್ಷ), "--", ರಿಕ್ತ);
			ಯದಿ (!ps->nr)
				strvec_ತಳ್ಳು(&cp.args, ":/");
			ಅನ್ಯಥಾ
				add_pathspecs(&cp.args, ps);
			ಯದಿ (run_command(&cp)) {
				ret = -1;
				ಗಚ್ಛ done;
			}
		}
		ಗಚ್ಛ done;
	} ಅನ್ಯಥಾ {
		ಸಂರಚನಾ child_process cp = CHILD_PROCESS_INIT;

		cp.git_cmd = 1;
		strvec_pushl(&cp.args, "apply", "-R", ರಿಕ್ತ);

		ಯದಿ (pipe_command(&cp, patch.buf, patch.len, ರಿಕ್ತ, 0, ರಿಕ್ತ, 0)) {
			ಯದಿ (!quiet)
				fprintf_ln(stderr, _("Cannot remove "
						     "worktree changes"));
			ret = -1;
			ಗಚ್ಛ done;
		}

		ಯದಿ (keep_index < 1) {
			ಸಂರಚನಾ child_process cp = CHILD_PROCESS_INIT;

			cp.git_cmd = 1;
			strvec_pushl(&cp.args, "reset", "-q", "--refresh", "--",
				     ರಿಕ್ತ);
			add_pathspecs(&cp.args, ps);
			ಯದಿ (run_command(&cp)) {
				ret = -1;
				ಗಚ್ಛ done;
			}
		}
		ಗಚ್ಛ done;
	}

done:
	strbuf_release(&patch);
	strbuf_release(&out);
	free_stash_info(&info);
	strbuf_release(&stash_ಸಂದೇಶ_buf);
	strbuf_release(&untracked_files);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ ತಳ್ಳು_stash(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
		      ಪೂರ್ಣಾಙ್ಕ ತಳ್ಳು_assumed)
{
	ಪೂರ್ಣಾಙ್ಕ force_assume = 0;
	ಪೂರ್ಣಾಙ್ಕ keep_index = -1;
	ಪೂರ್ಣಾಙ್ಕ only_staged = 0;
	ಪೂರ್ಣಾಙ್ಕ patch_mode = 0;
	ಪೂರ್ಣಾಙ್ಕ include_untracked = 0;
	ಪೂರ್ಣಾಙ್ಕ quiet = 0;
	ಪೂರ್ಣಾಙ್ಕ pathspec_file_nul = 0;
	ಧ್ರುವ ವರ್ಣ *stash_ಸಂದೇಶ = ರಿಕ್ತ;
	ವರ್ಣ *pathspec_from_file = ರಿಕ್ತ;
	ಸಂರಚನಾ pathspec ps;
	ಸಂರಚನಾ interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	ಸಂರಚನಾ option options[] = {
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
		OPT_STRING('m', "message", &stash_ಸಂದೇಶ, N_("message"),
			   N_("stash message")),
		OPT_PATHSPEC_FROM_FILE(&pathspec_from_file),
		OPT_PATHSPEC_FILE_NUL(&pathspec_file_nul),
		OPT_END()
	};
	ಪೂರ್ಣಾಙ್ಕ ret;

	ಯದಿ (argc) {
		ಪೂರ್ಣಾಙ್ಕ flags = PARSE_OPT_KEEP_DASHDASH;

		ಯದಿ (ತಳ್ಳು_assumed)
			flags |= PARSE_OPT_STOP_AT_NON_OPTION;

		argc = parse_options(argc, argv, prefix, options,
				     ತಳ್ಳು_assumed ? git_stash_usage :
				     git_stash_ತಳ್ಳು_usage, flags);
		force_assume |= patch_mode;
	}

	ಯದಿ (argc) {
		ಯದಿ (!strcmp(argv[0], "--")) {
			argc--;
			argv++;
		} ಅನ್ಯಥಾ ಯದಿ (ತಳ್ಳು_assumed && !force_assume) {
			die("subcommand wasn't specified; 'push' can't be assumed due to unexpected token '%s'",
			    argv[0]);
		}
	}

	parse_pathspec(&ps, 0, PATHSPEC_PREFER_FULL | PATHSPEC_PREFIX_ORIGIN,
		       prefix, argv);

	ಯದಿ (pathspec_from_file) {
		ಯದಿ (patch_mode)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--patch");

		ಯದಿ (only_staged)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--staged");

		ಯದಿ (ps.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&ps, 0,
				    PATHSPEC_PREFER_FULL | PATHSPEC_PREFIX_ORIGIN,
				    prefix, pathspec_from_file, pathspec_file_nul);
	} ಅನ್ಯಥಾ ಯದಿ (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	ಯದಿ (!patch_mode) {
		ಯದಿ (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--patch");
		ಯದಿ (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--patch");
		ಯದಿ (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--patch");
	}

	ಯದಿ (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	ಯದಿ (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	ret = do_ತಳ್ಳು_stash(&ps, stash_ಸಂದೇಶ, quiet, keep_index, patch_mode,
			    &interactive_opts, include_untracked, only_staged);

	clear_pathspec(&ps);
	ಮುಕ್ತ(pathspec_from_file);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ ತಳ್ಳು_stash_unassumed(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
				ಸಂರಚನಾ repository *repo UNUSED)
{
	ಪ್ರತ್ಯಾಗಮ ತಳ್ಳು_stash(argc, argv, prefix, 0);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ save_stash(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
		      ಸಂರಚನಾ repository *repo UNUSED)
{
	ಪೂರ್ಣಾಙ್ಕ keep_index = -1;
	ಪೂರ್ಣಾಙ್ಕ only_staged = 0;
	ಪೂರ್ಣಾಙ್ಕ patch_mode = 0;
	ಪೂರ್ಣಾಙ್ಕ include_untracked = 0;
	ಪೂರ್ಣಾಙ್ಕ quiet = 0;
	ಪೂರ್ಣಾಙ್ಕ ret = 0;
	ಧ್ರುವ ವರ್ಣ *stash_ಸಂದೇಶ = ರಿಕ್ತ;
	ಸಂರಚನಾ pathspec ps;
	ಸಂರಚನಾ strbuf stash_ಸಂದೇಶ_buf = STRBUF_INIT;
	ಸಂರಚನಾ interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	ಸಂರಚನಾ option options[] = {
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
		OPT_STRING('m', "message", &stash_ಸಂದೇಶ, "message",
			   N_("stash message")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_save_usage,
			     PARSE_OPT_KEEP_DASHDASH);

	ಯದಿ (argc)
		stash_ಸಂದೇಶ = strbuf_join_argv(&stash_ಸಂದೇಶ_buf, argc, argv, ' ');

	memset(&ps, 0, ಪರಿಮಾಣ(ps));

	ಯದಿ (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	ಯದಿ (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	ಯದಿ (!patch_mode) {
		ಯದಿ (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--patch");
		ಯದಿ (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--patch");
		ಯದಿ (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--patch");
	}

	ret = do_ತಳ್ಳು_stash(&ps, stash_ಸಂದೇಶ, quiet, keep_index,
			    patch_mode, &interactive_opts, include_untracked,
			    only_staged);

	strbuf_release(&stash_ಸಂದೇಶ_buf);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ write_commit_with_parents(ಸಂರಚನಾ repository *r,
				     ಸಂರಚನಾ object_id *out,
				     ಧ್ರುವ ಸಂರಚನಾ object_id *oid,
				     ಸಂರಚನಾ commit_list *parents)
{
	size_t author_len, committer_len;
	ಸಂರಚನಾ commit *this;
	ಧ್ರುವ ವರ್ಣ *orig_author, *orig_committer;
	ವರ್ಣ *author = ರಿಕ್ತ, *committer = ರಿಕ್ತ;
	ಧ್ರುವ ವರ್ಣ *buffer;
	unsigned long bufsize;
	ಧ್ರುವ ವರ್ಣ *p;
	ಸಂರಚನಾ strbuf ಸಂದೇಶ = STRBUF_INIT;
	ಪೂರ್ಣಾಙ್ಕ ret = 0;
	ಸಂರಚನಾ ident_split id;

	this = lookup_commit_reference(r, oid);
	buffer = repo_get_commit_buffer(r, this, &bufsize);
	orig_author = find_commit_header(buffer, "author", &author_len);
	orig_committer = find_commit_header(buffer, "committer", &committer_len);

	ಯದಿ (!orig_author || !orig_committer) {
		ret = error(_("cannot parse commit %s"), oid_to_hex(oid));
		ಗಚ್ಛ out;
	}

	ಯದಿ (split_ident_ಸಾಲು(&id, orig_author, author_len) < 0 ||
	    split_ident_ಸಾಲು(&id, orig_committer, committer_len) < 0) {
		ret = error(_("invalid author or committer for %s"), oid_to_hex(oid));
		ಗಚ್ಛ out;
	}

	p = strstr(buffer, "\n\n");
	strbuf_addstr(&ಸಂದೇಶ, "git stash: ");

	ಯದಿ (p)
		strbuf_add(&ಸಂದೇಶ, p + 2, bufsize - (p + 2 - buffer));
	strbuf_complete_ಸಾಲು(&ಸಂದೇಶ);

	author = xmemdupz(orig_author, author_len);
	committer = xmemdupz(orig_committer, committer_len);

	ಯದಿ (commit_ವೃಕ್ಷ_extended(ಸಂದೇಶ.buf, ಸಂದೇಶ.len,
				 r->hash_algo->empty_ವೃಕ್ಷ, parents,
				 out, author, committer,
				 ರಿಕ್ತ, ರಿಕ್ತ)) {
		ret = error(_("could not write commit"));
		ಗಚ್ಛ out;
	}
out:
	strbuf_release(&ಸಂದೇಶ);
	repo_unuse_commit_buffer(r, this, buffer);
	ಮುಕ್ತ(author);
	ಮುಕ್ತ(committer);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ do_import_stash(ಸಂರಚನಾ repository *r, ಧ್ರುವ ವರ್ಣ *rev)
{
	ಸಂರಚನಾ object_id chain;
	ಪೂರ್ಣಾಙ್ಕ res = 0;
	ಧ್ರುವ ವರ್ಣ *buffer = ರಿಕ್ತ;
	unsigned long bufsize;
	ಸಂರಚನಾ commit *this = ರಿಕ್ತ;
	ಸಂರಚನಾ commit_list *ವಸ್ತುಗಳು = ರಿಕ್ತ, *cur;
	ವರ್ಣ *ಸಂದೇಶ = ರಿಕ್ತ;

	ಯದಿ (repo_get_oid(r, rev, &chain))
		ಪ್ರತ್ಯಾಗಮ error(_("not a valid revision: %s"), rev);

	this = lookup_commit_reference(r, &chain);
	ಯದಿ (!this)
		ಪ್ರತ್ಯಾಗಮ error(_("not a commit: %s"), rev);

	/*
	 * Walk the commit history, finding each stash entry, and load data into
	 * the array.
	 */
	ಪ್ರತಿ (;;) {
		ಧ್ರುವ ವರ್ಣ *author, *committer;
		size_t author_len, committer_len;
		ಧ್ರುವ ವರ್ಣ *p;
		ಧ್ರುವ ವರ್ಣ *expected = "git stash <git@stash> 1000684800 +0000";
		ಧ್ರುವ ವರ್ಣ *prefix = "git stash: ";
		ಸಂರಚನಾ commit *stash;
		ಸಂರಚನಾ ವೃಕ್ಷ *ವೃಕ್ಷ = repo_get_commit_ವೃಕ್ಷ(r, this);

		ಯದಿ (!ವೃಕ್ಷ ||
		    !oideq(&ವೃಕ್ಷ->object.oid, r->hash_algo->empty_ವೃಕ್ಷ) ||
		    (this->parents &&
		     (!this->parents->next || this->parents->next->next))) {
			res = error(_("%s is not a valid exported stash commit"),
					oid_to_hex(&this->object.oid));
			ಗಚ್ಛ out;
		}

		buffer = repo_get_commit_buffer(r, this, &bufsize);

		ಯದಿ (!this->parents) {
			/*
			 * We don't have any parents.  Make sure this is our
			 * root commit.
			 */
			author = find_commit_header(buffer, "author", &author_len);
			committer = find_commit_header(buffer, "committer", &committer_len);

			ಯದಿ (!author || !committer) {
				error(_("cannot parse commit %s"), oid_to_hex(&this->object.oid));
				ಗಚ್ಛ out;
			}

			ಯದಿ (author_len != strlen(expected) ||
			    committer_len != strlen(expected) ||
			    memcmp(author, expected, author_len) ||
			    memcmp(committer, expected, committer_len)) {
				res = error(_("found root commit %s with invalid data"), oid_to_hex(&this->object.oid));
				ಗಚ್ಛ out;
			}
			ಭಙ್ಗ;
		}

		p = strstr(buffer, "\n\n");
		ಯದಿ (!p) {
			res = error(_("cannot parse commit %s"), oid_to_hex(&this->object.oid));
			ಗಚ್ಛ out;
		}

		p += 2;
		ಯದಿ (((size_t)(bufsize - (p - buffer)) < strlen(prefix)) ||
		    memcmp(prefix, p, strlen(prefix))) {
			res = error(_("found stash commit %s without expected prefix"), oid_to_hex(&this->object.oid));
			ಗಚ್ಛ out;
		}

		stash = this->parents->next->item;

		ಯದಿ (repo_parse_commit(r, this->parents->item) ||
		    repo_parse_commit(r, stash)) {
			res = error(_("cannot parse parents of commit: %s"),
					oid_to_hex(&this->object.oid));
			ಗಚ್ಛ out;
		}

		ಯದಿ (check_stash_topology(r, stash)) {
			res = error(_("%s does not look like a stash commit"),
					oid_to_hex(&stash->object.oid));
			ಗಚ್ಛ out;
		}

		repo_unuse_commit_buffer(r, this, buffer);
		buffer = ರಿಕ್ತ;
		ವಸ್ತುಗಳು = commit_list_ಸೇರಿಸು(stash, &ವಸ್ತುಗಳು);
		this = this->parents->item;
	}

	/*
	 * Now, walk each entry, adding it to the stash as a normal stash
	 * commit.
	 */
	ಪ್ರತಿ (cur = ವಸ್ತುಗಳು; cur; cur = cur->next) {
		ಧ್ರುವ ವರ್ಣ *p;
		ಸಂರಚನಾ object_id *oid;

		this = cur->item;
		oid = &this->object.oid;
		buffer = repo_get_commit_buffer(r, this, &bufsize);
		ಯದಿ (!buffer) {
			res = error(_("cannot read commit buffer for %s"), oid_to_hex(oid));
			ಗಚ್ಛ out;
		}

		p = strstr(buffer, "\n\n");
		ಯದಿ (!p) {
			res = error(_("cannot parse commit %s"), oid_to_hex(oid));
			ಗಚ್ಛ out;
		}

		p += 2;
		ಸಂದೇಶ = xmemdupz(p, bufsize - (p - buffer));
		repo_unuse_commit_buffer(r, this, buffer);
		buffer = ರಿಕ್ತ;

		ಯದಿ (do_store_stash(oid, ಸಂದೇಶ, 1)) {
			res = error(_("cannot save the stash for %s"), oid_to_hex(oid));
			ಗಚ್ಛ out;
		}
		FREE_AND_NULL(ಸಂದೇಶ);
	}
out:
	ಯದಿ (this && buffer)
		repo_unuse_commit_buffer(r, this, buffer);
	commit_list_free(ವಸ್ತುಗಳು);
	ಮುಕ್ತ(ಸಂದೇಶ);

	ಪ್ರತ್ಯಾಗಮ res;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ import_stash(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
			ಸಂರಚನಾ repository *repo)
{
	ಸಂರಚನಾ option options[] = {
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     git_stash_import_usage,
			     PARSE_OPT_KEEP_DASHDASH);

	ಯದಿ (argc != 1)
		usage_ಸಂದೇಶ_opt("a revision is required", git_stash_import_usage, options);

	ಪ್ರತ್ಯಾಗಮ do_import_stash(repo, argv[0]);
}

ಸಂರಚನಾ stash_entry_data {
	ಸಂರಚನಾ repository *r;
	ಸಂರಚನಾ commit_list **ವಸ್ತುಗಳು;
	size_t ಎಣಿಕೆ;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ collect_stash_entries(ಧ್ರುವ ವರ್ಣ *refname UNUSED,
				 ಸಂರಚನಾ object_id *old_oid UNUSED,
				 ಸಂರಚನಾ object_id *new_oid,
				 ಧ್ರುವ ವರ್ಣ *committer UNUSED,
				 timestamp_t timestamp UNUSED,
				 ಪೂರ್ಣಾಙ್ಕ tz UNUSED, ಧ್ರುವ ವರ್ಣ *ಸಂದೇಶ UNUSED,
				 ಶೂನ್ಯ *cb_data)
{
	ಸಂರಚನಾ stash_entry_data *data = cb_data;
	ಸಂರಚನಾ commit *stash;

	data->ಎಣಿಕೆ++;
	stash = lookup_commit_reference(data->r, new_oid);
	ಯದಿ (!stash || check_stash_topology(data->r, stash)) {
		ಪ್ರತ್ಯಾಗಮ error(_("%s does not look like a stash commit"),
			     oid_to_hex(new_oid));
	}
	data->ವಸ್ತುಗಳು = commit_list_append(stash, data->ವಸ್ತುಗಳು);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ do_export_stash(ಸಂರಚನಾ repository *r,
			   ಧ್ರುವ ವರ್ಣ *ref,
			   ಪೂರ್ಣಾಙ್ಕ argc,
			   ಧ್ರುವ ವರ್ಣ **argv)
{
	ಸಂರಚನಾ object_id base;
	ಸಂರಚನಾ commit *prev;
	ಸಂರಚನಾ commit_list *ವಸ್ತುಗಳು = ರಿಕ್ತ, **iter = &ವಸ್ತುಗಳು, *cur;
	ಪೂರ್ಣಾಙ್ಕ res = 0;
	ಪೂರ್ಣಾಙ್ಕ i;
	ಸಂರಚನಾ strbuf revision = STRBUF_INIT;
	ಧ್ರುವ ವರ್ಣ *author, *committer;

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
	ಯದಿ (commit_ವೃಕ್ಷ_extended("", 0, r->hash_algo->empty_ವೃಕ್ಷ, ರಿಕ್ತ,
				 &base, author, committer, ರಿಕ್ತ, ರಿಕ್ತ))
		ಪ್ರತ್ಯಾಗಮ error(_("unable to write base commit"));

	prev = lookup_commit_reference(r, &base);

	ಯದಿ (argc) {
		/*
		 * Find each specified stash, and load data into the array.
		 */
		ಪ್ರತಿ (i = 0; i < argc; i++) {
			ಸಂರಚನಾ object_id oid;
			ಸಂರಚನಾ commit *stash;

			ಯದಿ (parse_stash_revision(&revision, argv[i], 1) ||
			    repo_get_oid_with_flags(r, revision.buf, &oid,
						    GET_OID_QUIETLY |
						    GET_OID_GENTLY)) {
				res = error(_("unable to find stash entry %s"), argv[i]);
				ಗಚ್ಛ out;
			}

			stash = lookup_commit_reference(r, &oid);
			ಯದಿ (!stash || check_stash_topology(r, stash)) {
				res = error(_("%s does not look like a stash commit"),
						revision.buf);
				ಗಚ್ಛ out;
			}
			iter = commit_list_append(stash, iter);
		}
	} ಅನ್ಯಥಾ {
		/*
		 * Walk the reflog, finding each stash entry, and load data into the
		 * array.
		 */
		ಸಂರಚನಾ stash_entry_data cb_data = {
			.r = r, .ವಸ್ತುಗಳು = iter,
		};
		ಯದಿ (refs_for_each_reflog_ent_reverse(get_main_ref_store(r),
						     "refs/stash",
						     collect_stash_entries,
						     &cb_data) && cb_data.ಎಣಿಕೆ)
			ಗಚ್ಛ out;
	}

	/*
	 * Now, create a set of commits identical to the regular stash commits,
	 * but where their first parents form a chain to our original empty
	 * base commit.
	 */
	ವಸ್ತುಗಳು = commit_list_reverse(ವಸ್ತುಗಳು);
	ಪ್ರತಿ (cur = ವಸ್ತುಗಳು; cur; cur = cur->next) {
		ಸಂರಚನಾ commit_list *parents = ರಿಕ್ತ;
		ಸಂರಚನಾ commit_list **next = &parents;
		ಸಂರಚನಾ object_id out;
		ಸಂರಚನಾ commit *stash = cur->item;

		next = commit_list_append(prev, next);
		next = commit_list_append(stash, next);
		res = write_commit_with_parents(r, &out, &stash->object.oid, parents);
		commit_list_free(parents);
		ಯದಿ (res)
			ಗಚ್ಛ out;
		prev = lookup_commit_reference(r, &out);
	}
	ಯದಿ (ref)
		refs_update_ref(get_main_ref_store(r), ರಿಕ್ತ, ref,
				&prev->object.oid, ರಿಕ್ತ, 0, UPDATE_REFS_DIE_ON_ERR);
	ಅನ್ಯಥಾ
		puts(oid_to_hex(&prev->object.oid));
out:
	strbuf_release(&revision);
	commit_list_free(ವಸ್ತುಗಳು);

	ಪ್ರತ್ಯಾಗಮ res;
}

ಗಣನಾ export_action {
	ACTION_NONE,
	ACTION_PRINT,
	ACTION_TO_REF,
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ export_stash(ಪೂರ್ಣಾಙ್ಕ argc,
			ಧ್ರುವ ವರ್ಣ **argv,
			ಧ್ರುವ ವರ್ಣ *prefix,
			ಸಂರಚನಾ repository *repo)
{
	ಧ್ರುವ ವರ್ಣ *ref = ರಿಕ್ತ;
	ಗಣನಾ export_action action = ACTION_NONE;
	ಸಂರಚನಾ option options[] = {
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

	ಯದಿ (ref && action == ACTION_NONE)
		action = ACTION_TO_REF;

	ಯದಿ (action == ACTION_NONE || (ref && action == ACTION_PRINT))
		ಪ್ರತ್ಯಾಗಮ error(_("exactly one of --print and --to-ref is required"));

	ಪ್ರತ್ಯಾಗಮ do_export_stash(repo, ref, argc, argv);
}

ಪೂರ್ಣಾಙ್ಕ cmd_stash(ಪೂರ್ಣಾಙ್ಕ argc,
	      ಧ್ರುವ ವರ್ಣ **argv,
	      ಧ್ರುವ ವರ್ಣ *prefix,
	      ಸಂರಚನಾ repository *repo)
{
	pid_t pid = getpid();
	ಧ್ರುವ ವರ್ಣ *index_file;
	ಸಂರಚನಾ strvec args = STRVEC_INIT;
	parse_opt_subcommand_fn *fn = ರಿಕ್ತ;
	ಸಂರಚನಾ option options[] = {
		OPT_SUBCOMMAND("apply", &fn, apply_stash),
		OPT_SUBCOMMAND("clear", &fn, clear_stash),
		OPT_SUBCOMMAND("drop", &fn, drop_stash),
		OPT_SUBCOMMAND("pop", &fn, ತೆಗೆ_stash),
		OPT_SUBCOMMAND("branch", &fn, branch_stash),
		OPT_SUBCOMMAND("list", &fn, list_stash),
		OPT_SUBCOMMAND("show", &fn, show_stash),
		OPT_SUBCOMMAND("store", &fn, store_stash),
		OPT_SUBCOMMAND("create", &fn, create_stash),
		OPT_SUBCOMMAND("push", &fn, ತಳ್ಳು_stash_unassumed),
		OPT_SUBCOMMAND("export", &fn, export_stash),
		OPT_SUBCOMMAND("import", &fn, import_stash),
		OPT_SUBCOMMAND_F("save", &fn, save_stash, ರಿಕ್ತ, PARSE_OPT_NOCOMPLETE),
		OPT_END()
	};
	ಧ್ರುವ ವರ್ಣ **args_copy;
	ಪೂರ್ಣಾಙ್ಕ ret;

	repo_config(the_repository, git_stash_config, ರಿಕ್ತ);

	argc = parse_options(argc, argv, prefix, options, git_stash_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL |
			     PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	index_file = repo_get_index_file(the_repository);
	strbuf_addf(&stash_index_path, "%s.stash.%" PRIuMAX, index_file,
		    (uintmax_t)pid);

	ಯದಿ (fn) {
		ret = fn(argc, argv, prefix, repo);

		/*
		 * The subcommand implementations return 0 on success, a
		 * negative value on failure, and STASH_APPLY_CONFLICT
		 * when applying a stash entry resulted in conflicts.
		 * Map failures to 128, the status die() uses, so that
		 * exit status 1 unambiguously indicates conflicts.
		 */
		ಯದಿ (ret < 0)
			ಪ್ರತ್ಯಾಗಮ 128;
		ಪ್ರತ್ಯಾಗಮ ret;
	} ಅನ್ಯಥಾ ಯದಿ (!argc) {
		ಪ್ರತ್ಯಾಗಮ !!ತಳ್ಳು_stash_unassumed(0, ರಿಕ್ತ, prefix, repo);
	}

	/* Assume 'stash push' */
	strvec_ತಳ್ಳು(&args, "push");
	strvec_pushv(&args, argv);

	/*
	 * `push_stash()` ends up modifying the array, which causes memory
	 * leaks if we didn't copy the array here.
	 */
	DUP_ARRAY(args_copy, args.v, args.nr);

	ret = !!ತಳ್ಳು_stash(args.nr, args_copy, prefix, 1);

	strvec_clear(&args);
	ಮುಕ್ತ(args_copy);
	ಪ್ರತ್ಯಾಗಮ ret;
}
