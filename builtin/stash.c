#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE

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

#ವ್ಯಾಖ್ಯಾನಿಸು INCLUDE_ALL_FILES 2

#ವ್ಯಾಖ್ಯಾನಿಸು BUILTIN_STASH_LIST_USAGE \
	N_("git stash list [<log-options>]")
#ವ್ಯಾಖ್ಯಾನಿಸು BUILTIN_STASH_SHOW_USAGE \
	N_("git stash show [-u | --include-untracked | --only-untracked] [<diff-options>] [<stash>]")
#ವ್ಯಾಖ್ಯಾನಿಸು BUILTIN_STASH_DROP_USAGE \
	N_("git stash drop [-q | --quiet] [<stash>]")
#ವ್ಯಾಖ್ಯಾನಿಸು BUILTIN_STASH_ತೆಗೆ_USAGE \
	N_("git stash pop [--index] [-q | --quiet] [<stash>]")
#ವ್ಯಾಖ್ಯಾನಿಸು BUILTIN_STASH_APPLY_USAGE \
	N_("git stash apply [--index] [-q | --quiet] [--label-ours=<label>] [--label-theirs=<label>] [--label-base=<label>] [<stash>]")
#ವ್ಯಾಖ್ಯಾನಿಸು BUILTIN_STASH_BRANCH_USAGE \
	N_("git stash branch <branchname> [<stash>]")
#ವ್ಯಾಖ್ಯಾನಿಸು BUILTIN_STASH_STORE_USAGE \
	N_("git stash store [(-m | --message) <message>] [-q | --quiet] <commit>")
#ವ್ಯಾಖ್ಯಾನಿಸು BUILTIN_STASH_ತಳ್ಳು_USAGE \
	N_("git stash [push] [-p | --patch] [-S | --staged] [-k | --[no-]keep-index] [-q | --quiet]\n" \
	   "          [-u | --include-untracked] [-a | --all] [(-m | --message) <message>]\n" \
	   "          [--pathspec-from-file=<file> [--pathspec-file-nul]]\n" \
	   "          [--] [<pathspec>...]")
#ವ್ಯಾಖ್ಯಾನಿಸು BUILTIN_STASH_SAVE_USAGE \
	N_("git stash save [-p | --patch] [-S | --staged] [-k | --[no-]keep-index] [-q | --quiet]\n" \
	   "          [-u | --include-untracked] [-a | --all] [<message>]")
#ವ್ಯಾಖ್ಯಾನಿಸು BUILTIN_STASH_CREATE_USAGE \
	N_("git stash create [<message>]")
#ವ್ಯಾಖ್ಯಾನಿಸು BUILTIN_STASH_EXPORT_USAGE \
	N_("git stash export (--print | --to-ref <ref>) [<stash>...]")
#ವ್ಯಾಖ್ಯಾನಿಸು BUILTIN_STASH_IMPORT_USAGE \
	N_("git stash import <commit>")
#ವ್ಯಾಖ್ಯಾನಿಸು BUILTIN_STASH_CLEAR_USAGE \
	"git stash clear"

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ git_stash_usage[] = {
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
	ಖಾಲಿ
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ git_stash_list_usage[] = {
	BUILTIN_STASH_LIST_USAGE,
	ಖಾಲಿ
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ git_stash_show_usage[] = {
	BUILTIN_STASH_SHOW_USAGE,
	ಖಾಲಿ
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ git_stash_drop_usage[] = {
	BUILTIN_STASH_DROP_USAGE,
	ಖಾಲಿ
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ git_stash_ತೆಗೆ_usage[] = {
	BUILTIN_STASH_ತೆಗೆ_USAGE,
	ಖಾಲಿ
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ git_stash_apply_usage[] = {
	BUILTIN_STASH_APPLY_USAGE,
	ಖಾಲಿ
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ git_stash_branch_usage[] = {
	BUILTIN_STASH_BRANCH_USAGE,
	ಖಾಲಿ
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ git_stash_clear_usage[] = {
	BUILTIN_STASH_CLEAR_USAGE,
	ಖಾಲಿ
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ git_stash_store_usage[] = {
	BUILTIN_STASH_STORE_USAGE,
	ಖಾಲಿ
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ git_stash_ತಳ್ಳು_usage[] = {
	BUILTIN_STASH_ತಳ್ಳು_USAGE,
	ಖಾಲಿ
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ git_stash_save_usage[] = {
	BUILTIN_STASH_SAVE_USAGE,
	ಖಾಲಿ
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ git_stash_export_usage[] = {
	BUILTIN_STASH_EXPORT_USAGE,
	ಖಾಲಿ
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ git_stash_import_usage[] = {
	BUILTIN_STASH_IMPORT_USAGE,
	ಖಾಲಿ
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ ref_stash[] = "refs/stash";
ಸ್ಥಿರ ರಚನೆ strbuf stash_ಸೂಚ್ಯಂಕ_ಮಾರ್ಗ = STRBUF_INIT;

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_stat = 1;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_patch;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_include_untracked;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ use_ಸೂಚ್ಯಂಕ;

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
ರಚನೆ stash_info {
	ರಚನೆ object_id w_commit;
	ರಚನೆ object_id b_commit;
	ರಚನೆ object_id i_commit;
	ರಚನೆ object_id u_commit;
	ರಚನೆ object_id c_commit;
	ರಚನೆ object_id w_ವೃಕ್ಷ;
	ರಚನೆ object_id b_ವೃಕ್ಷ;
	ರಚನೆ object_id i_ವೃಕ್ಷ;
	ರಚನೆ object_id u_ವೃಕ್ಷ;
	ರಚನೆ strbuf revision;
	ಪೂರ್ಣಾಂಕ is_stash_ref;
	ಪೂರ್ಣಾಂಕ has_u;
};

#ವ್ಯಾಖ್ಯಾನಿಸು STASH_INFO_INIT { \
	.revision = STRBUF_INIT, \
}

ಸ್ಥಿರ ಶೂನ್ಯ free_stash_info(ರಚನೆ stash_info *info)
{
	strbuf_release(&info->revision);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ check_stash_topology(ರಚನೆ repository *r, ರಚನೆ commit *stash)
{
	ರಚನೆ commit *p1, *p2, *p3 = ಖಾಲಿ;

	/* stash must have two or three parents */
	ಆದರೆ (!stash->parents || !stash->parents->ಮುಂದಿನ ||
			(stash->parents->ಮುಂದಿನ->ಮುಂದಿನ && stash->parents->ಮುಂದಿನ->ಮುಂದಿನ->ಮುಂದಿನ))
		ಹಿಂತಿರುಗಿ -1;
	p1 = stash->parents->ಅಂಶ;
	p2 = stash->parents->ಮುಂದಿನ->ಅಂಶ;
	ಆದರೆ (stash->parents->ಮುಂದಿನ->ಮುಂದಿನ)
		p3 = stash->parents->ಮುಂದಿನ->ಮುಂದಿನ->ಅಂಶ;
	ಆದರೆ (repo_parse_commit(r, p1) || repo_parse_commit(r, p2) ||
			(p3 && repo_parse_commit(r, p3)))
		ಹಿಂತಿರುಗಿ -1;
	/* p2 must have a single parent, p3 must have no parents */
	ಆದರೆ (!p2->parents || p2->parents->ಮುಂದಿನ || (p3 && p3->parents))
		ಹಿಂತಿರುಗಿ -1;
	ಆದರೆ (repo_parse_commit(r, p2->parents->ಅಂಶ))
		ಹಿಂತಿರುಗಿ -1;
	/* p2^1 must equal p1 */
	ಆದರೆ (!oideq(&p1->object.oid, &p2->parents->ಅಂಶ->object.oid))
		ಹಿಂತಿರುಗಿ -1;

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ assert_stash_like(ರಚನೆ stash_info *info, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *revision)
{
	ಆದರೆ (get_oidf(&info->b_commit, "%s^1", revision) ||
	    get_oidf(&info->w_ವೃಕ್ಷ, "%s:", revision) ||
	    get_oidf(&info->b_ವೃಕ್ಷ, "%s^1:", revision) ||
	    get_oidf(&info->i_ವೃಕ್ಷ, "%s^2:", revision))
		die(_("'%s' is not a stash-like commit"), revision);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ parse_stash_revision(ರಚನೆ strbuf *revision, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *commit, ಪೂರ್ಣಾಂಕ quiet)
{
	strbuf_reset(revision);
	ಆದರೆ (!commit) {
		ಆದರೆ (!refs_ref_exists(get_main_ref_store(the_repository), ref_stash)) {
			ಆದರೆ (!quiet)
				fprintf_ln(stderr, _("No stash entries found."));
			ಹಿಂತಿರುಗಿ -1;
		}

		strbuf_addf(revision, "%s@{0}", ref_stash);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (strspn(commit, "0123456789") == ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(commit)) {
		strbuf_addf(revision, "%s@{%s}", ref_stash, commit);
	} ಇಲ್ಲದಿದ್ದರೆ {
		strbuf_addstr(revision, commit);
	}
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ get_stash_info(ರಚನೆ stash_info *info, ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv)
{
	ಪೂರ್ಣಾಂಕ ret;
	ಅಕ್ಷರ *ಕೊನೆ_ದ_rev;
	ಅಕ್ಷರ *expanded_ref;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *revision;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *commit = ಖಾಲಿ;
	ರಚನೆ object_id dummy;
	ರಚನೆ strbuf symbolic = STRBUF_INIT;

	ಆದರೆ (argc > 1) {
		ಪೂರ್ಣಾಂಕ i;
		ರಚನೆ strbuf refs_ಸಂದೇಶ = STRBUF_INIT;

		ಗಾಗಿ (i = 0; i < argc; i++)
			strbuf_addf(&refs_ಸಂದೇಶ, " '%s'", argv[i]);

		fprintf_ln(stderr, _("Too many revisions specified:%s"),
			   refs_ಸಂದೇಶ.buf);
		strbuf_release(&refs_ಸಂದೇಶ);

		ಹಿಂತಿರುಗಿ -1;
	}

	ಆದರೆ (argc == 1)
		commit = argv[0];

	strbuf_init(&info->revision, 0);
	ಆದರೆ (parse_stash_revision(&info->revision, commit, 0)) {
		ಹಿಂತಿರುಗಿ -1;
	}

	revision = info->revision.buf;

	ಆದರೆ (repo_get_oid(the_repository, revision, &info->w_commit))
		ಹಿಂತಿರುಗಿ error(_("%s is not a valid reference"), revision);

	assert_stash_like(info, revision);

	info->has_u = !get_oidf(&info->u_ವೃಕ್ಷ, "%s^3:", revision);

	ಕೊನೆ_ದ_rev = strchrnul(revision, '@');
	strbuf_ಕೂಡಿಸು(&symbolic, revision, ಕೊನೆ_ದ_rev - revision);

	ret = repo_dwim_ref(the_repository, symbolic.buf, symbolic.len,
			    &dummy, &expanded_ref, 0);
	strbuf_release(&symbolic);
	ಆಯ್ಕೆ (ret) {
	ಸ್ಥಿತಿ 0: /* Not found, but valid ref */
		info->is_stash_ref = 0;
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ 1:
		info->is_stash_ref = !strcmp(expanded_ref, ref_stash);
		ನಿಲ್ಲಿಸು;
	ಪೂರ್ವನಿಯೋಜಿತ: /* Invalid or ambiguous */
		ನಿಲ್ಲಿಸು;
	}

	ಬಿಡುಗಡೆ(expanded_ref);
	ಹಿಂತಿರುಗಿ !(ret == 0 || ret == 1);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ do_clear_stash(ಶೂನ್ಯ)
{
	ರಚನೆ object_id obj;
	ಆದರೆ (repo_get_oid(the_repository, ref_stash, &obj))
		ಹಿಂತಿರುಗಿ 0;

	ಹಿಂತಿರುಗಿ refs_delete_ref(get_main_ref_store(the_repository), ಖಾಲಿ,
			       ref_stash, &obj, 0);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ clear_stash(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		       ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ option options[] = {
		OPT_ಕೊನೆ()
	};

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options,
			     git_stash_clear_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	ಆದರೆ (argc)
		ಹಿಂತಿರುಗಿ error(_("git stash clear with arguments is "
			       "unimplemented"));

	ಹಿಂತಿರುಗಿ do_clear_stash();
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ reset_ವೃಕ್ಷ(ರಚನೆ object_id *i_ವೃಕ್ಷ, ಪೂರ್ಣಾಂಕ update, ಪೂರ್ಣಾಂಕ reset)
{
	ಪೂರ್ಣಾಂಕ nr_trees = 1;
	ರಚನೆ unpack_trees_options opts;
	ರಚನೆ ವೃಕ್ಷ_desc t[MAX_UNPACK_TREES];
	ರಚನೆ ವೃಕ್ಷ *ವೃಕ್ಷ;
	ರಚನೆ ಬೀಗ_file ಬೀಗ_file = ಬೀಗ_INIT;

	repo_read_ಸೂಚ್ಯಂಕ_preload(the_repository, ಖಾಲಿ, 0);
	ಆದರೆ (refresh_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ, REFRESH_QUIET, ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ))
		ಹಿಂತಿರುಗಿ -1;

	repo_hold_locked_ಸೂಚ್ಯಂಕ(the_repository, &ಬೀಗ_file, ಬೀಗ_DIE_ON_ERROR);

	memset(&opts, 0, ಗಾತ್ರ(opts));

	ವೃಕ್ಷ = repo_parse_ವೃಕ್ಷ_ಪರೋಕ್ಷ(the_repository, i_ವೃಕ್ಷ);
	ಆದರೆ (repo_parse_ವೃಕ್ಷ(the_repository, ವೃಕ್ಷ))
		ಹಿಂತಿರುಗಿ -1;

	init_ವೃಕ್ಷ_desc(t, &ವೃಕ್ಷ->object.oid, ವೃಕ್ಷ->ಬಫರ್, ವೃಕ್ಷ->ಗಾತ್ರ_2);

	opts.ತಲೆ_idx = 1;
	opts.src_ಸೂಚ್ಯಂಕ = the_repository->ಸೂಚ್ಯಂಕ;
	opts.dst_ಸೂಚ್ಯಂಕ = the_repository->ಸೂಚ್ಯಂಕ;
	opts.ವಿಲೀನ = 1;
	opts.reset = reset ? UNPACK_RESET_PROTECT_UNTRACKED : 0;
	opts.update = update;
	ಆದರೆ (update)
		opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
	opts.fn = oneway_ವಿಲೀನ;

	ಆದರೆ (unpack_trees(nr_trees, t, &opts))
		ಹಿಂತಿರುಗಿ -1;

	ಆದರೆ (write_locked_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ, &ಬೀಗ_file, COMMIT_ಬೀಗ))
		ಹಿಂತಿರುಗಿ error(_("unable to write new index file"));

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ create_ಸೂಚ್ಯಂಕ_from_ವೃಕ್ಷ(ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *ವೃಕ್ಷ_id,
				  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸೂಚ್ಯಂಕ_ಮಾರ್ಗ)
{
	ಪೂರ್ಣಾಂಕ nr_trees = 1;
	ಪೂರ್ಣಾಂಕ ret = 0;
	ರಚನೆ unpack_trees_options opts;
	ರಚನೆ ವೃಕ್ಷ_desc t[MAX_UNPACK_TREES];
	ರಚನೆ ವೃಕ್ಷ *ವೃಕ್ಷ;
	ರಚನೆ ಸೂಚ್ಯಂಕ_state dst_istate = ಸೂಚ್ಯಂಕ_STATE_INIT(the_repository);
	ರಚನೆ ಬೀಗ_file ಬೀಗ_file = ಬೀಗ_INIT;

	repo_read_ಸೂಚ್ಯಂಕ_preload(the_repository, ಖಾಲಿ, 0);
	refresh_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ, REFRESH_QUIET, ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ);

	hold_ಬೀಗ_file_for_update(&ಬೀಗ_file, ಸೂಚ್ಯಂಕ_ಮಾರ್ಗ, ಬೀಗ_DIE_ON_ERROR);

	memset(&opts, 0, ಗಾತ್ರ(opts));

	ವೃಕ್ಷ = repo_parse_ವೃಕ್ಷ_ಪರೋಕ್ಷ(the_repository, ವೃಕ್ಷ_id);
	ಆದರೆ (!ವೃಕ್ಷ || repo_parse_ವೃಕ್ಷ(the_repository, ವೃಕ್ಷ)) {
		ret = -1;
		ಹೋಗು ಮುಗಿದ;
	}

	init_ವೃಕ್ಷ_desc(t, &ವೃಕ್ಷ->object.oid, ವೃಕ್ಷ->ಬಫರ್, ವೃಕ್ಷ->ಗಾತ್ರ_2);

	opts.ತಲೆ_idx = 1;
	opts.src_ಸೂಚ್ಯಂಕ = the_repository->ಸೂಚ್ಯಂಕ;
	opts.dst_ಸೂಚ್ಯಂಕ = &dst_istate;
	opts.ವಿಲೀನ = 1;
	opts.reset = UNPACK_RESET_PROTECT_UNTRACKED;
	opts.fn = oneway_ವಿಲೀನ;

	ಆದರೆ (unpack_trees(nr_trees, t, &opts)) {
		ret = -1;
		ಹೋಗು ಮುಗಿದ;
	}

	ಆದರೆ (write_locked_ಸೂಚ್ಯಂಕ(&dst_istate, &ಬೀಗ_file, COMMIT_ಬೀಗ)) {
		ret = error(_("unable to write new index file"));
		ಹೋಗು ಮುಗಿದ;
	}

ಮುಗಿದ:
	release_ಸೂಚ್ಯಂಕ(&dst_istate);
	ಆದರೆ (ret)
		rollback_ಬೀಗ_file(&ಬೀಗ_file);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ is_ಮಾರ್ಗ_a_directory(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ)
{
	/*
	 * This function differs from abspath.c:is_directory() in that
	 * here we use lstat() instead of stat(); we do not want to
	 * follow symbolic links here.
	 */
	ರಚನೆ stat st;
	ಹಿಂತಿರುಗಿ (!lstat(ಮಾರ್ಗ, &st) && S_ISDIR(st.st_ಬಹುಲಕ));
}

ಸ್ಥಿರ ಶೂನ್ಯ ಕೂಡಿಸು_diff_ಗಮ್ಯ_buf(ರಚನೆ diff_ಸರತಿ_struct *q,
			    ರಚನೆ diff_options *options ಬಳಸದ,
			    ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
	ಪೂರ್ಣಾಂಕ i;

	ಗಾಗಿ (i = 0; i < q->nr; i++) {
		ಆದರೆ (is_ಮಾರ್ಗ_a_directory(q->ಸರತಿ[i]->one->ಮಾರ್ಗ))
			ಮುಂದುವರಿ;

		strbuf_addstr(ದತ್ತಾಂಶ, q->ಸರತಿ[i]->one->ಮಾರ್ಗ);

		/* NUL-terminate: will be fed to update-index -z */
		strbuf_addch(ದತ್ತಾಂಶ, '\0');
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ restore_untracked(ರಚನೆ object_id *u_ವೃಕ್ಷ)
{
	ಪೂರ್ಣಾಂಕ res;
	ರಚನೆ ಮಗು_process cp = ಮಗು_PROCESS_INIT;

	/*
	 * We need to run restore files from a given index, but without
	 * affecting the current index, so we use GIT_INDEX_FILE with
	 * run_command to fork processes that will not interfere.
	 */
	cp.git_cmd = 1;
	strvec_ತಳ್ಳು(&cp.ವಾದಗಳು, "read-tree");
	strvec_ತಳ್ಳು(&cp.ವಾದಗಳು, oid_ಗಮ್ಯ_hex(u_ವೃಕ್ಷ));
	strvec_pushf(&cp.env, "GIT_INDEX_FILE=%s",
		     stash_ಸೂಚ್ಯಂಕ_ಮಾರ್ಗ.buf);
	ಆದರೆ (ಸತತ_command(&cp)) {
		remove_ಮಾರ್ಗ(stash_ಸೂಚ್ಯಂಕ_ಮಾರ್ಗ.buf);
		ಹಿಂತಿರುಗಿ -1;
	}

	ಮಗು_process_init(&cp);
	cp.git_cmd = 1;
	strvec_pushl(&cp.ವಾದಗಳು, "checkout-index", "--all", ಖಾಲಿ);
	strvec_pushf(&cp.env, "GIT_INDEX_FILE=%s",
		     stash_ಸೂಚ್ಯಂಕ_ಮಾರ್ಗ.buf);

	res = ಸತತ_command(&cp);
	remove_ಮಾರ್ಗ(stash_ಸೂಚ್ಯಂಕ_ಮಾರ್ಗ.buf);
	ಹಿಂತಿರುಗಿ res;
}

ಸ್ಥಿರ ಶೂನ್ಯ unstage_changes_unless_new(ರಚನೆ object_id *orig_ವೃಕ್ಷ)
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

	ರಚನೆ checkout state = CHECKOUT_INIT;
	ರಚನೆ diff_options diff_opts;
	ರಚನೆ ಬೀಗ_file ಬೀಗ = ಬೀಗ_INIT;
	ಪೂರ್ಣಾಂಕ i;

	/* If any entries have skip_worktree set, we'll have to check 'em out */
	state.force = 1;
	state.quiet = 1;
	state.refresh_cache = 1;
	state.istate = the_repository->ಸೂಚ್ಯಂಕ;

	/*
	 * Step 1: get a difference between orig_tree (which corresponding
	 * to the index before a merge was run) and the current index
	 * (reflecting the changes brought in by the merge).
	 */
	repo_diff_ಸಿದ್ಧತೆ(the_repository, &diff_opts);
	diff_opts.flags.recursive = 1;
	diff_opts.detect_rename = 0;
	diff_opts.output_format = DIFF_FORMAT_NO_OUTPUT;
	diff_ಸಿದ್ಧತೆ_ಮುಗಿದ(&diff_opts);

	do_diff_cache(orig_ವೃಕ್ಷ, &diff_opts);
	diffcore_std(&diff_opts);

	/* Iterate over the paths that changed due to the merge... */
	ಗಾಗಿ (i = 0; i < diff_queued_diff.nr; i++) {
		ರಚನೆ diff_filepair *p;
		ರಚನೆ cache_ನಮೂದು *ce;
		ಪೂರ್ಣಾಂಕ pos;

		/* Look up the path's position in the current index. */
		p = diff_queued_diff.ಸರತಿ[i];
		pos = ಸೂಚ್ಯಂಕ_name_pos(the_repository->ಸೂಚ್ಯಂಕ, p->two->ಮಾರ್ಗ,
				     ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(p->two->ಮಾರ್ಗ));

		/*
		 * Step 2: Place changes in the working tree
		 *
		 * Stash is about restoring changes *to the working tree*.
		 * So if the merge successfully got a new version of some
		 * path, but left it out of the working tree, then clear the
		 * SKIP_WORKTREE bit and write it to the working tree.
		 */
		ಆದರೆ (pos >= 0 && ce_skip_worktree(the_repository->ಸೂಚ್ಯಂಕ->cache[pos])) {
			ರಚನೆ stat st;

			ce = the_repository->ಸೂಚ್ಯಂಕ->cache[pos];
			ಆದರೆ (!lstat(ce->name, &st)) {
				/* Conflicting path present; relocate it */
				ರಚನೆ strbuf new_ಮಾರ್ಗ = STRBUF_INIT;
				ಪೂರ್ಣಾಂಕ fd;

				strbuf_addf(&new_ಮಾರ್ಗ,
					    "%s.stash.XXXXXX", ce->name);
				fd = xmkstemp(new_ಮಾರ್ಗ.buf);
				close(fd);
				ಬರೆ(_("WARNING: Untracked file in way of "
					 "tracked file!  Renaming\n "
					 "           %s -> %s\n"
					 "         to make room.\n"),
				       ce->name, new_ಮಾರ್ಗ.buf);
				ಆದರೆ (rename(ce->name, new_ಮಾರ್ಗ.buf))
					die("Failed to move %s to %s",
					    ce->name, new_ಮಾರ್ಗ.buf);
				strbuf_release(&new_ಮಾರ್ಗ);
			}
			checkout_ನಮೂದು(ce, &state, ಖಾಲಿ, ಖಾಲಿ);
			ce->ce_flags &= ~CE_SKIP_WORKTREE;
		}

		/*
		 * Step 3: "unstage" changes, as long as they are still tracked
		 */
		ಆದರೆ (p->one->oid_valid) {
			/*
			 * Path existed in orig_tree; restore index entry
			 * from that tree in order to "unstage" the changes.
			 */
			ಪೂರ್ಣಾಂಕ option = ಕೂಡಿಸು_CACHE_OK_ಗಮ್ಯ_REPLACE;
			ಆದರೆ (pos < 0)
				option = ಕೂಡಿಸು_CACHE_OK_ಗಮ್ಯ_ಕೂಡಿಸು;

			ce = make_cache_ನಮೂದು(the_repository->ಸೂಚ್ಯಂಕ,
					      p->one->ಬಹುಲಕ,
					      &p->one->oid,
					      p->one->ಮಾರ್ಗ,
					      0, 0);
			ಕೂಡಿಸು_ಸೂಚ್ಯಂಕ_ನಮೂದು(the_repository->ಸೂಚ್ಯಂಕ, ce, option);
		}
	}
	diff_flush(&diff_opts);

	/*
	 * Step 4: write the new index to disk
	 */
	repo_hold_locked_ಸೂಚ್ಯಂಕ(the_repository, &ಬೀಗ, ಬೀಗ_DIE_ON_ERROR);
	ಆದರೆ (write_locked_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ, &ಬೀಗ,
			       COMMIT_ಬೀಗ | SKIP_IF_UNCHANGED))
		die(_("could not write index"));
}

ಸ್ಥಿರ ಎಣಿಕೆ stash_apply_ಫಲಿತಾಂಶ do_apply_stash(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
					      ರಚನೆ stash_info *info,
					      ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ, ಪೂರ್ಣಾಂಕ quiet,
					      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *label_ours,
					      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *label_theirs,
					      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *label_ತಳ)
{
	ಪೂರ್ಣಾಂಕ clean, ret;
	ಪೂರ್ಣಾಂಕ has_ಸೂಚ್ಯಂಕ = ಸೂಚ್ಯಂಕ;
	ರಚನೆ ವಿಲೀನ_options o;
	ರಚನೆ object_id c_ವೃಕ್ಷ;
	ರಚನೆ object_id ಸೂಚ್ಯಂಕ_ವೃಕ್ಷ;
	ರಚನೆ ವೃಕ್ಷ *ತಲೆ, *ವಿಲೀನ, *ವಿಲೀನ_ತಳ;
	ರಚನೆ ಬೀಗ_file ಬೀಗ = ಬೀಗ_INIT;

	repo_read_ಸೂಚ್ಯಂಕ_preload(the_repository, ಖಾಲಿ, 0);
	ಆದರೆ (repo_refresh_and_write_ಸೂಚ್ಯಂಕ(the_repository, REFRESH_QUIET, 0, 0,
					 ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ))
		ಹಿಂತಿರುಗಿ error(_("could not write index"));

	ಆದರೆ (write_ಸೂಚ್ಯಂಕ_as_ವೃಕ್ಷ(&c_ವೃಕ್ಷ, the_repository->ಸೂಚ್ಯಂಕ,
				repo_get_ಸೂಚ್ಯಂಕ_file(the_repository), 0, ಖಾಲಿ))
		ಹಿಂತಿರುಗಿ error(_("cannot apply a stash in the middle of a merge"));

	init_ui_ವಿಲೀನ_options(&o, the_repository);

	ಆದರೆ (quiet)
		o.verbosity = 0;

	ಆದರೆ (ಸೂಚ್ಯಂಕ) {
		ಆದರೆ (oideq(&info->b_ವೃಕ್ಷ, &info->i_ವೃಕ್ಷ) ||
		    oideq(&c_ವೃಕ್ಷ, &info->i_ವೃಕ್ಷ)) {
			has_ಸೂಚ್ಯಂಕ = 0;
		} ಇಲ್ಲದಿದ್ದರೆ {
			ರಚನೆ ವಿಲೀನ_ಫಲಿತಾಂಶ ಫಲಿತಾಂಶ = { 0 };

			o.branch1 = "Current index";
			o.branch2 = "Stashed index changes";
			o.ancestor = "Stash base";

			ತಲೆ = lookup_ವೃಕ್ಷ(o.repo, &c_ವೃಕ್ಷ);
			ವಿಲೀನ = lookup_ವೃಕ್ಷ(o.repo, &info->i_ವೃಕ್ಷ);
			ವಿಲೀನ_ತಳ = lookup_ವೃಕ್ಷ(o.repo, &info->b_ವೃಕ್ಷ);

			ವಿಲೀನ_incore_nonrecursive(&o, ವಿಲೀನ_ತಳ, ತಲೆ, ವಿಲೀನ,
						  &ಫಲಿತಾಂಶ);

			ಆದರೆ (ಫಲಿತಾಂಶ.clean < 0) {
				ವಿಲೀನ_finalize(&o, &ಫಲಿತಾಂಶ);
				ಹಿಂತಿರುಗಿ error(_("index merge failed"));
			} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!ಫಲಿತಾಂಶ.clean) {
				ವಿಲೀನ_finalize(&o, &ಫಲಿತಾಂಶ);
				ಹಿಂತಿರುಗಿ error(_("conflicts in index. "
					       "Try without --index."));
			} ಇಲ್ಲದಿದ್ದರೆ {
				oidcpy(&ಸೂಚ್ಯಂಕ_ವೃಕ್ಷ, &ಫಲಿತಾಂಶ.ವೃಕ್ಷ->object.oid);
				ವಿಲೀನ_finalize(&o, &ಫಲಿತಾಂಶ);
			}
		}
	}

	o.branch1 = label_ours ? label_ours : "Updated upstream";
	o.branch2 = label_theirs ? label_theirs : "Stashed changes";
	o.ancestor = label_ತಳ ? label_ತಳ : "Stash base";

	ಆದರೆ (oideq(&info->b_ವೃಕ್ಷ, &c_ವೃಕ್ಷ))
		o.branch1 = "Version stash was based on";

	ಆದರೆ (o.verbosity >= 3)
		printf_ln(_("Merging %s with %s"), o.branch1, o.branch2);

	ತಲೆ = lookup_ವೃಕ್ಷ(o.repo, &c_ವೃಕ್ಷ);
	ವಿಲೀನ = lookup_ವೃಕ್ಷ(o.repo, &info->w_ವೃಕ್ಷ);
	ವಿಲೀನ_ತಳ = lookup_ವೃಕ್ಷ(o.repo, &info->b_ವೃಕ್ಷ);

	repo_hold_locked_ಸೂಚ್ಯಂಕ(o.repo, &ಬೀಗ, ಬೀಗ_DIE_ON_ERROR);
	clean = ವಿಲೀನ_ort_nonrecursive(&o, ತಲೆ, ವಿಲೀನ, ವಿಲೀನ_ತಳ);

	/*
	 * If 'clean' >= 0, reverse the value for 'ret' so 'ret' is 0 when the
	 * merge was clean, and 1 if the merge was unclean or a negative value
	 * if it encountered an error.
	 */
	ret = clean >= 0 ? !clean : clean;

	ಆದರೆ (ret < 0)
		rollback_ಬೀಗ_file(&ಬೀಗ);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (write_locked_ಸೂಚ್ಯಂಕ(o.repo->ಸೂಚ್ಯಂಕ, &ಬೀಗ,
				      COMMIT_ಬೀಗ | SKIP_IF_UNCHANGED))
		ret = error(_("could not write index"));

	ಆದರೆ (ret) {
		repo_rerere(the_repository, 0);

		ಆದರೆ (ಸೂಚ್ಯಂಕ)
			fprintf_ln(stderr, _("Index was not unstashed."));

		ಹೋಗು restore_untracked;
	}

	ಆದರೆ (has_ಸೂಚ್ಯಂಕ) {
		ಆದರೆ (reset_ವೃಕ್ಷ(&ಸೂಚ್ಯಂಕ_ವೃಕ್ಷ, 0, 0))
			ret = -1;
	} ಇಲ್ಲದಿದ್ದರೆ {
		unstage_changes_unless_new(&c_ವೃಕ್ಷ);
	}

restore_untracked:
	ಆದರೆ (info->has_u && restore_untracked(&info->u_ವೃಕ್ಷ))
		ret = error(_("could not restore untracked files from stash"));

	ಆದರೆ (!quiet) {
		ರಚನೆ ಮಗು_process cp = ಮಗು_PROCESS_INIT;

		/*
		 * Status is quite simple and could be replaced with calls to
		 * wt_status in the future, but it adds complexities which may
		 * require more tests.
		 */
		cp.git_cmd = 1;
		cp.dir = ಪೂರ್ವಪ್ರತ್ಯಯ;
		strvec_pushf(&cp.env, GIT_WORK_ವೃಕ್ಷ_ENVIRONMENT"=%s",
			     absolute_ಮಾರ್ಗ(repo_get_work_ವೃಕ್ಷ(the_repository)));
		strvec_pushf(&cp.env, GIT_DIR_ENVIRONMENT"=%s",
			     absolute_ಮಾರ್ಗ(repo_get_git_dir(the_repository)));
		strvec_ತಳ್ಳು(&cp.ವಾದಗಳು, "status");
		ಸತತ_command(&cp);
	}

	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ apply_stash(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		       ರಚನೆ repository *repo ಬಳಸದ)
{
	ಪೂರ್ಣಾಂಕ ret = -1;
	ಪೂರ್ಣಾಂಕ quiet = 0;
	ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ = use_ಸೂಚ್ಯಂಕ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *label_ours = ಖಾಲಿ, *label_theirs = ಖಾಲಿ, *label_ತಳ = ಖಾಲಿ;
	ರಚನೆ stash_info info = STASH_INFO_INIT;
	ರಚನೆ option options[] = {
		OPT__QUIET(&quiet, N_("be quiet, only report errors")),
		OPT_BOOL(0, "index", &ಸೂಚ್ಯಂಕ,
			 N_("attempt to recreate the index")),
		OPT_STRING(0, "label-ours", &label_ours, N_("label"),
			   N_("label for the upstream side in conflict markers")),
		OPT_STRING(0, "label-theirs", &label_theirs, N_("label"),
			   N_("label for the stashed side in conflict markers")),
		OPT_STRING(0, "label-base", &label_ತಳ, N_("label"),
			   N_("label for the base in diff3 conflict markers")),
		OPT_ಕೊನೆ()
	};

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options,
			     git_stash_apply_usage, 0);

	ಆದರೆ (get_stash_info(&info, argc, argv))
		ಹೋಗು cleanup;

	ret = do_apply_stash(ಪೂರ್ವಪ್ರತ್ಯಯ, &info, ಸೂಚ್ಯಂಕ, quiet,
			     label_ours, label_theirs, label_ತಳ);
cleanup:
	free_stash_info(&info);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ reject_reflog_ent(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *refname ಬಳಸದ,
			     ರಚನೆ object_id *ooid ಬಳಸದ,
			     ರಚನೆ object_id *noid ಬಳಸದ,
			     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *email ಬಳಸದ,
			     timestamp_t timestamp ಬಳಸದ,
			     ಪೂರ್ಣಾಂಕ tz ಬಳಸದ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ_2 ಬಳಸದ,
			     ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ ಬಳಸದ)
{
	ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ reflog_is_ಖಾಲಿಗಳು(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *refname)
{
	ಹಿಂತಿರುಗಿ !refs_for_each_reflog_ent(get_main_ref_store(the_repository),
					 refname, reject_reflog_ent, ಖಾಲಿ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ do_drop_stash(ರಚನೆ stash_info *info, ಪೂರ್ಣಾಂಕ quiet)
{
	ಆದರೆ (!reflog_delete(info->revision.buf,
			   EXPIRE_REFLOGS_REWRITE | EXPIRE_REFLOGS_UPDATE_REF,
			   0)) {
		ಆದರೆ (!quiet)
			printf_ln(_("Dropped %s (%s)"), info->revision.buf,
				  oid_ಗಮ್ಯ_hex(&info->w_commit));
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಹಿಂತಿರುಗಿ error(_("%s: Could not drop stash entry"),
			     info->revision.buf);
	}

	ಆದರೆ (reflog_is_ಖಾಲಿಗಳು(ref_stash))
		do_clear_stash();

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ get_stash_info_assert(ರಚನೆ stash_info *info, ಪೂರ್ಣಾಂಕ argc,
				 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv)
{
	ಪೂರ್ಣಾಂಕ ret = get_stash_info(info, argc, argv);

	ಆದರೆ (ret < 0)
		ಹಿಂತಿರುಗಿ ret;

	ಆದರೆ (!info->is_stash_ref)
		ಹಿಂತಿರುಗಿ error(_("'%s' is not a stash reference"), info->revision.buf);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ drop_stash(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		      ರಚನೆ repository *repo ಬಳಸದ)
{
	ಪೂರ್ಣಾಂಕ ret = -1;
	ಪೂರ್ಣಾಂಕ quiet = 0;
	ರಚನೆ stash_info info = STASH_INFO_INIT;
	ರಚನೆ option options[] = {
		OPT__QUIET(&quiet, N_("be quiet, only report errors")),
		OPT_ಕೊನೆ()
	};

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options,
			     git_stash_drop_usage, 0);

	ಆದರೆ (get_stash_info_assert(&info, argc, argv))
		ಹೋಗು cleanup;

	ret = do_drop_stash(&info, quiet);
cleanup:
	free_stash_info(&info);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ತೆಗೆ_stash(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		     ರಚನೆ repository *repo ಬಳಸದ)
{
	ಪೂರ್ಣಾಂಕ ret = -1;
	ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ = use_ಸೂಚ್ಯಂಕ;
	ಪೂರ್ಣಾಂಕ quiet = 0;
	ರಚನೆ stash_info info = STASH_INFO_INIT;
	ರಚನೆ option options[] = {
		OPT__QUIET(&quiet, N_("be quiet, only report errors")),
		OPT_BOOL(0, "index", &ಸೂಚ್ಯಂಕ,
			 N_("attempt to recreate the index")),
		OPT_ಕೊನೆ()
	};

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options,
			     git_stash_ತೆಗೆ_usage, 0);

	ಆದರೆ (get_stash_info_assert(&info, argc, argv))
		ಹೋಗು cleanup;

	ಆದರೆ ((ret = do_apply_stash(ಪೂರ್ವಪ್ರತ್ಯಯ, &info, ಸೂಚ್ಯಂಕ, quiet,
				  ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ)))
		printf_ln(_("The stash entry is kept in case "
			    "you need it again."));
	ಇಲ್ಲದಿದ್ದರೆ
		ret = do_drop_stash(&info, quiet);

cleanup:
	free_stash_info(&info);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ branch_stash(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			ರಚನೆ repository *repo ಬಳಸದ)
{
	ಪೂರ್ಣಾಂಕ ret = -1;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *branch = ಖಾಲಿ;
	ರಚನೆ stash_info info = STASH_INFO_INIT;
	ರಚನೆ ಮಗು_process cp = ಮಗು_PROCESS_INIT;
	ರಚನೆ option options[] = {
		OPT_ಕೊನೆ()
	};

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options,
			     git_stash_branch_usage, 0);

	ಆದರೆ (!argc) {
		fprintf_ln(stderr, _("No branch name specified"));
		ಹಿಂತಿರುಗಿ -1;
	}

	branch = argv[0];

	ಆದರೆ (get_stash_info(&info, argc - 1, argv + 1))
		ಹೋಗು cleanup;

	cp.git_cmd = 1;
	strvec_pushl(&cp.ವಾದಗಳು, "checkout", "-b", ಖಾಲಿ);
	strvec_ತಳ್ಳು(&cp.ವಾದಗಳು, branch);
	strvec_ತಳ್ಳು(&cp.ವಾದಗಳು, oid_ಗಮ್ಯ_hex(&info.b_commit));
	ret = ಸತತ_command(&cp);
	ಆದರೆ (!ret)
		ret = do_apply_stash(ಪೂರ್ವಪ್ರತ್ಯಯ, &info, 1, 0,
				     ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ);
	ಆದರೆ (!ret && info.is_stash_ref)
		ret = do_drop_stash(&info, 0);

cleanup:
	free_stash_info(&info);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ list_stash(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		      ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ ಮಗು_process cp = ಮಗು_PROCESS_INIT;
	ರಚನೆ option options[] = {
		OPT_ಕೊನೆ()
	};

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options,
			     git_stash_list_usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT);

	ಆದರೆ (!refs_ref_exists(get_main_ref_store(the_repository), ref_stash))
		ಹಿಂತಿರುಗಿ 0;

	cp.git_cmd = 1;
	strvec_pushl(&cp.ವಾದಗಳು, "log", "--format=%gd: %gs", "-g",
		     "--first-parent", ಖಾಲಿ);
	strvec_pushv(&cp.ವಾದಗಳು, argv);
	strvec_ತಳ್ಳು(&cp.ವಾದಗಳು, ref_stash);
	strvec_ತಳ್ಳು(&cp.ವಾದಗಳು, "--");
	ಹಿಂತಿರುಗಿ ಸತತ_command(&cp);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ git_stash_config(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *var, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ,
			    ಸ್ಥಿರಾಂಕ ರಚನೆ config_context *ctx, ಶೂನ್ಯ *cb)
{
	ಆದರೆ (!strcmp(var, "stash.showstat")) {
		show_stat = git_config_bool(var, ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(var, "stash.showpatch")) {
		show_patch = git_config_bool(var, ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(var, "stash.showincludeuntracked")) {
		show_include_untracked = git_config_bool(var, ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(var, "stash.index")) {
		use_ಸೂಚ್ಯಂಕ = git_config_bool(var, ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಹಿಂತಿರುಗಿ git_diff_basic_config(var, ಮೌಲ್ಯ, ctx, cb);
}

ಸ್ಥಿರ ಶೂನ್ಯ diff_include_untracked(ಸ್ಥಿರಾಂಕ ರಚನೆ stash_info *info, ರಚನೆ diff_options *diff_opt)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid[] = { &info->w_commit, &info->u_ವೃಕ್ಷ };
	ರಚನೆ ವೃಕ್ಷ *ವೃಕ್ಷ[ARRAY_ಗಾತ್ರ(oid)];
	ರಚನೆ ವೃಕ್ಷ_desc ವೃಕ್ಷ_desc[ARRAY_ಗಾತ್ರ(oid)];
	ರಚನೆ unpack_trees_options unpack_ವೃಕ್ಷ_opt = { 0 };

	ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < ARRAY_ಗಾತ್ರ(oid); i++) {
		ವೃಕ್ಷ[i] = repo_parse_ವೃಕ್ಷ_ಪರೋಕ್ಷ(the_repository, oid[i]);
		ಆದರೆ (repo_parse_ವೃಕ್ಷ(the_repository, ವೃಕ್ಷ[i]) < 0)
			die(_("failed to parse tree"));
		init_ವೃಕ್ಷ_desc(&ವೃಕ್ಷ_desc[i], &ವೃಕ್ಷ[i]->object.oid,
			       ವೃಕ್ಷ[i]->ಬಫರ್, ವೃಕ್ಷ[i]->ಗಾತ್ರ_2);
	}

	unpack_ವೃಕ್ಷ_opt.ತಲೆ_idx = -1;
	unpack_ವೃಕ್ಷ_opt.src_ಸೂಚ್ಯಂಕ = the_repository->ಸೂಚ್ಯಂಕ;
	unpack_ವೃಕ್ಷ_opt.dst_ಸೂಚ್ಯಂಕ = the_repository->ಸೂಚ್ಯಂಕ;
	unpack_ವೃಕ್ಷ_opt.ವಿಲೀನ = 1;
	unpack_ವೃಕ್ಷ_opt.fn = stash_worktree_untracked_ವಿಲೀನ;

	ಆದರೆ (unpack_trees(ARRAY_ಗಾತ್ರ(ವೃಕ್ಷ_desc), ವೃಕ್ಷ_desc, &unpack_ವೃಕ್ಷ_opt))
		die(_("failed to unpack trees"));

	do_diff_cache(&info->b_commit, diff_opt);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_stash(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		      ರಚನೆ repository *repo ಬಳಸದ)
{
	ಪೂರ್ಣಾಂಕ i;
	ಪೂರ್ಣಾಂಕ ret = -1;
	ರಚನೆ stash_info info = STASH_INFO_INIT;
	ರಚನೆ rev_info rev;
	ರಚನೆ strvec stash_ವಾದಗಳು = STRVEC_INIT;
	ರಚನೆ strvec revision_ವಾದಗಳು = STRVEC_INIT;
	ಎಣಿಕೆ {
		UNTRACKED_NONE,
		UNTRACKED_INCLUDE,
		UNTRACKED_ONLY
	} show_untracked = show_include_untracked ? UNTRACKED_INCLUDE : UNTRACKED_NONE;
	ರಚನೆ option options[] = {
		OPT_ಗಣ_INT('u', "include-untracked", &show_untracked,
			    N_("include untracked files in the stash"),
			    UNTRACKED_INCLUDE),
		OPT_ಗಣ_INT_F(0, "only-untracked", &show_untracked,
			      N_("only show untracked files in the stash"),
			      UNTRACKED_ONLY, PARSE_OPT_NONEG),
		OPT_ಕೊನೆ()
	};
	ಪೂರ್ಣಾಂಕ do_usage = 0;

	init_diff_ui_defaults();
	repo_config(the_repository, git_diff_ui_config, ಖಾಲಿ);
	repo_init_revisions(the_repository, &rev, ಪೂರ್ವಪ್ರತ್ಯಯ);

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options, git_stash_show_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	strvec_ತಳ್ಳು(&revision_ವಾದಗಳು, argv[0]);
	ಗಾಗಿ (i = 1; i < argc; i++) {
		ಆದರೆ (argv[i][0] != '-')
			strvec_ತಳ್ಳು(&stash_ವಾದಗಳು, argv[i]);
		ಇಲ್ಲದಿದ್ದರೆ
			strvec_ತಳ್ಳು(&revision_ವಾದಗಳು, argv[i]);
	}

	ಆದರೆ (get_stash_info(&info, stash_ವಾದಗಳು.nr, stash_ವಾದಗಳು.v))
		ಹೋಗು cleanup;

	/*
	 * The config settings are applied only if there are not passed
	 * any options.
	 */
	ಆದರೆ (revision_ವಾದಗಳು.nr == 1) {
		ಆದರೆ (show_stat)
			rev.diffopt.output_format = DIFF_FORMAT_DIFFSTAT;

		ಆದರೆ (show_patch)
			rev.diffopt.output_format |= DIFF_FORMAT_PATCH;

		ಆದರೆ (!show_stat && !show_patch) {
			ret = 0;
			ಹೋಗು cleanup;
		}
	}

	ಸಿದ್ಧತೆ_revisions_from_strvec(&revision_ವಾದಗಳು, &rev, ಖಾಲಿ);
	ಆದರೆ (revision_ವಾದಗಳು.nr > 1)
		ಹೋಗು usage;
	ಆದರೆ (!rev.diffopt.output_format) {
		rev.diffopt.output_format = DIFF_FORMAT_PATCH;
		diff_ಸಿದ್ಧತೆ_ಮುಗಿದ(&rev.diffopt);
	}

	rev.diffopt.flags.recursive = 1;
	ಸಿದ್ಧತೆ_diff_pager(&rev.diffopt);
	ಆಯ್ಕೆ (show_untracked) {
	ಸ್ಥಿತಿ UNTRACKED_NONE:
		diff_ವೃಕ್ಷ_oid(&info.b_commit, &info.w_commit, "", &rev.diffopt);
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ UNTRACKED_ONLY:
		ಆದರೆ (info.has_u)
			diff_ಬೇರು_ವೃಕ್ಷ_oid(&info.u_ವೃಕ್ಷ, "", &rev.diffopt);
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ UNTRACKED_INCLUDE:
		ಆದರೆ (info.has_u)
			diff_include_untracked(&info, &rev.diffopt);
		ಇಲ್ಲದಿದ್ದರೆ
			diff_ವೃಕ್ಷ_oid(&info.b_commit, &info.w_commit, "", &rev.diffopt);
		ನಿಲ್ಲಿಸು;
	}
	log_ವೃಕ್ಷ_diff_flush(&rev);

	ret = diff_ಫಲಿತಾಂಶ_ಸಂಕೇತ(&rev);

cleanup:
	strvec_clear(&revision_ವಾದಗಳು);
	strvec_clear(&stash_ವಾದಗಳು);
	free_stash_info(&info);
	release_revisions(&rev);
	ಆದರೆ (do_usage)
		usage_with_options(git_stash_show_usage, options);
	ಹಿಂತಿರುಗಿ ret;
usage:
	do_usage = 1;
	ಹೋಗು cleanup;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ do_store_stash(ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *w_commit, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *stash_ಸಂದೇಶ,
			  ಪೂರ್ಣಾಂಕ quiet)
{
	ರಚನೆ stash_info info;
	ಅಕ್ಷರ revision[GIT_MAX_HEXSZ];

	oid_ಗಮ್ಯ_hex_r(revision, w_commit);
	assert_stash_like(&info, revision);

	ಆದರೆ (!stash_ಸಂದೇಶ)
		stash_ಸಂದೇಶ = "Created via \"git stash store\".";

	ಆದರೆ (refs_update_ref(get_main_ref_store(the_repository), stash_ಸಂದೇಶ, ref_stash, w_commit, ಖಾಲಿ,
			    REF_FORCE_CREATE_REFLOG,
			    quiet ? UPDATE_REFS_QUIET_ON_ERR :
			    UPDATE_REFS_ಸಂದೇಶ_ON_ERR)) {
		ಆದರೆ (!quiet) {
			fprintf_ln(stderr, _("Cannot update %s with %s"),
				   ref_stash, oid_ಗಮ್ಯ_hex(w_commit));
		}
		ಹಿಂತಿರುಗಿ -1;
	}

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ store_stash(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		       ರಚನೆ repository *repo ಬಳಸದ)
{
	ಪೂರ್ಣಾಂಕ quiet = 0;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *stash_ಸಂದೇಶ = ಖಾಲಿ;
	ರಚನೆ object_id obj;
	ರಚನೆ option options[] = {
		OPT__QUIET(&quiet, N_("be quiet")),
		OPT_STRING('m', "message", &stash_ಸಂದೇಶ, "message",
			   N_("stash message")),
		OPT_ಕೊನೆ()
	};
	ಪೂರ್ಣಾಂಕ ret;

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options,
			     git_stash_store_usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT);

	ಆದರೆ (argc != 1) {
		ಆದರೆ (!quiet)
			fprintf_ln(stderr, _("\"git stash store\" requires one "
					     "<commit> argument"));
		ಹಿಂತಿರುಗಿ -1;
	}

	ಆದರೆ (repo_get_oid_with_flags(the_repository, argv[0], &obj,
				    quiet ? GET_OID_QUIETLY : 0)) {
		ಆದರೆ (!quiet)
			fprintf_ln(stderr, _("Cannot update %s with %s"),
					     ref_stash, argv[0]);
		ret = -1;
		ಹೋಗು ಹೊರಹೋಗು;
	}

	ret = do_store_stash(&obj, stash_ಸಂದೇಶ, quiet);

ಹೊರಹೋಗು:
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಶೂನ್ಯ ಕೂಡಿಸು_pathspecs(ರಚನೆ strvec *ವಾದಗಳು,
			  ಸ್ಥಿರಾಂಕ ರಚನೆ pathspec *ps) {
	ಪೂರ್ಣಾಂಕ i;

	ಗಾಗಿ (i = 0; i < ps->nr; i++)
		strvec_ತಳ್ಳು(ವಾದಗಳು, ps->ವಸ್ತುಗಳು[i].original);
}

/*
 * `untracked_files` will be filled with the names of untracked files.
 * The return value is:
 *
 * = 0 if there are not any untracked files
 * > 0 if there are untracked files
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ get_untracked_files(ಸ್ಥಿರಾಂಕ ರಚನೆ pathspec *ps, ಪೂರ್ಣಾಂಕ include_untracked,
			       ರಚನೆ strbuf *untracked_files)
{
	ಪೂರ್ಣಾಂಕ i;
	ಪೂರ್ಣಾಂಕ ಸಿಕ್ಕಿತು_2 = 0;
	ರಚನೆ dir_struct dir = DIR_INIT;

	ಆದರೆ (include_untracked != INCLUDE_ALL_FILES)
		ಸಿದ್ಧತೆ_standard_excludes(&dir);

	fill_directory(&dir, the_repository->ಸೂಚ್ಯಂಕ, ps);
	ಗಾಗಿ (i = 0; i < dir.nr; i++) {
		ರಚನೆ dir_ನಮೂದು *ent = dir.entries[i];
		ಸಿಕ್ಕಿತು_2++;
		strbuf_addstr(untracked_files, ent->name);
		/* NUL-terminate: will be fed to update-index -z */
		strbuf_addch(untracked_files, '\0');
	}

	dir_clear(&dir);
	ಹಿಂತಿರುಗಿ ಸಿಕ್ಕಿತು_2;
}

/*
 * The return value of `check_changes_tracked_files()` can be:
 *
 * < 0 if there was an error
 * = 0 if there are no changes.
 * > 0 if there are changes.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ check_changes_tracked_files(ಸ್ಥಿರಾಂಕ ರಚನೆ pathspec *ps)
{
	ರಚನೆ rev_info rev;
	ರಚನೆ object_id dummy;
	ಪೂರ್ಣಾಂಕ ret = 0;

	/* No initial commit. */
	ಆದರೆ (repo_get_oid(the_repository, "HEAD", &dummy))
		ಹಿಂತಿರುಗಿ -1;

	ಆದರೆ (repo_read_ಸೂಚ್ಯಂಕ(the_repository) < 0)
		ಹಿಂತಿರುಗಿ -1;

	repo_init_revisions(the_repository, &rev, ಖಾಲಿ);
	copy_pathspec(&rev.prune_ದತ್ತಾಂಶ, ps);

	rev.diffopt.flags.ಶೀಘ್ರ = 1;
	rev.diffopt.flags.ignore_submodules = 1;
	rev.abbrev = 0;

	ಕೂಡಿಸು_ತಲೆ_ಗಮ್ಯ_pending(&rev);
	diff_ಸಿದ್ಧತೆ_ಮುಗಿದ(&rev.diffopt);

	ಸತತ_diff_ಸೂಚ್ಯಂಕ(&rev, DIFF_ಸೂಚ್ಯಂಕ_CACHED);
	ಆದರೆ (diff_ಫಲಿತಾಂಶ_ಸಂಕೇತ(&rev)) {
		ret = 1;
		ಹೋಗು ಮುಗಿದ;
	}

	ಸತತ_diff_files(&rev, 0);
	ಆದರೆ (diff_ಫಲಿತಾಂಶ_ಸಂಕೇತ(&rev)) {
		ret = 1;
		ಹೋಗು ಮುಗಿದ;
	}

ಮುಗಿದ:
	release_revisions(&rev);
	ಹಿಂತಿರುಗಿ ret;
}

/*
 * The function will fill `untracked_files` with the names of untracked files
 * It will return 1 if there were any changes and 0 if there were not.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ check_changes(ಸ್ಥಿರಾಂಕ ರಚನೆ pathspec *ps, ಪೂರ್ಣಾಂಕ include_untracked,
			 ರಚನೆ strbuf *untracked_files)
{
	ಪೂರ್ಣಾಂಕ ret = 0;
	ಆದರೆ (check_changes_tracked_files(ps))
		ret = 1;

	ಆದರೆ (include_untracked && get_untracked_files(ps, include_untracked,
						     untracked_files))
		ret = 1;

	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ save_untracked_files(ರಚನೆ stash_info *info, ರಚನೆ strbuf *ಸಂದೇಶ,
				ರಚನೆ strbuf *files)
{
	ಪೂರ್ಣಾಂಕ ret = 0;
	ರಚನೆ strbuf untracked_ಸಂದೇಶ = STRBUF_INIT;
	ರಚನೆ ಮಗು_process cp_upd_ಸೂಚ್ಯಂಕ = ಮಗು_PROCESS_INIT;
	ರಚನೆ ಸೂಚ್ಯಂಕ_state istate = ಸೂಚ್ಯಂಕ_STATE_INIT(the_repository);

	cp_upd_ಸೂಚ್ಯಂಕ.git_cmd = 1;
	strvec_pushl(&cp_upd_ಸೂಚ್ಯಂಕ.ವಾದಗಳು, "update-index", "-z", "--add",
		     "--remove", "--stdin", ಖಾಲಿ);
	strvec_pushf(&cp_upd_ಸೂಚ್ಯಂಕ.env, "GIT_INDEX_FILE=%s",
			 stash_ಸೂಚ್ಯಂಕ_ಮಾರ್ಗ.buf);

	strbuf_addf(&untracked_ಸಂದೇಶ, "untracked files on %s\n", ಸಂದೇಶ->buf);
	ಆದರೆ (pipe_command(&cp_upd_ಸೂಚ್ಯಂಕ, files->buf, files->len, ಖಾಲಿ, 0,
			 ಖಾಲಿ, 0)) {
		ret = -1;
		ಹೋಗು ಮುಗಿದ;
	}

	ಆದರೆ (write_ಸೂಚ್ಯಂಕ_as_ವೃಕ್ಷ(&info->u_ವೃಕ್ಷ, &istate, stash_ಸೂಚ್ಯಂಕ_ಮಾರ್ಗ.buf, 0,
				ಖಾಲಿ)) {
		ret = -1;
		ಹೋಗು ಮುಗಿದ;
	}

	ಆದರೆ (commit_ವೃಕ್ಷ(untracked_ಸಂದೇಶ.buf, untracked_ಸಂದೇಶ.len,
			&info->u_ವೃಕ್ಷ, ಖಾಲಿ, &info->u_commit, ಖಾಲಿ, ಖಾಲಿ)) {
		ret = -1;
		ಹೋಗು ಮುಗಿದ;
	}

ಮುಗಿದ:
	release_ಸೂಚ್ಯಂಕ(&istate);
	strbuf_release(&untracked_ಸಂದೇಶ);
	remove_ಮಾರ್ಗ(stash_ಸೂಚ್ಯಂಕ_ಮಾರ್ಗ.buf);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stash_staged(ರಚನೆ stash_info *info, ರಚನೆ strbuf *ಹೊರಹೋಗು_patch,
			ಪೂರ್ಣಾಂಕ quiet)
{
	ಪೂರ್ಣಾಂಕ ret = 0;
	ರಚನೆ ಮಗು_process cp_diff_ವೃಕ್ಷ = ಮಗು_PROCESS_INIT;
	ರಚನೆ ಸೂಚ್ಯಂಕ_state istate = ಸೂಚ್ಯಂಕ_STATE_INIT(the_repository);

	ಆದರೆ (write_ಸೂಚ್ಯಂಕ_as_ವೃಕ್ಷ(&info->w_ವೃಕ್ಷ, &istate, the_repository->ಸೂಚ್ಯಂಕ_file,
				0, ಖಾಲಿ)) {
		ret = -1;
		ಹೋಗು ಮುಗಿದ;
	}

	cp_diff_ವೃಕ್ಷ.git_cmd = 1;
	strvec_pushl(&cp_diff_ವೃಕ್ಷ.ವಾದಗಳು, "diff-tree", "-p", "--binary",
		     "--no-color",
		     "-U1", "HEAD", oid_ಗಮ್ಯ_hex(&info->w_ವೃಕ್ಷ), "--", ಖಾಲಿ);
	ಆದರೆ (pipe_command(&cp_diff_ವೃಕ್ಷ, ಖಾಲಿ, 0, ಹೊರಹೋಗು_patch, 0, ಖಾಲಿ, 0)) {
		ret = -1;
		ಹೋಗು ಮುಗಿದ;
	}

	ಆದರೆ (!ಹೊರಹೋಗು_patch->len) {
		ಆದರೆ (!quiet)
			fprintf_ln(stderr, _("No staged changes"));
		ret = 1;
	}

ಮುಗಿದ:
	release_ಸೂಚ್ಯಂಕ(&istate);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stash_patch(ರಚನೆ stash_info *info, ಸ್ಥಿರಾಂಕ ರಚನೆ pathspec *ps,
		       ರಚನೆ strbuf *ಹೊರಹೋಗು_patch, ಪೂರ್ಣಾಂಕ quiet,
		       ರಚನೆ interactive_options *interactive_opts)
{
	ಪೂರ್ಣಾಂಕ ret = 0;
	ರಚನೆ ಮಗು_process cp_diff_ವೃಕ್ಷ = ಮಗು_PROCESS_INIT;
	ರಚನೆ commit *ತಲೆ_commit;
	ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *ತಲೆ_ವೃಕ್ಷ;
	ರಚನೆ ಸೂಚ್ಯಂಕ_state istate = ಸೂಚ್ಯಂಕ_STATE_INIT(the_repository);
	ಅಕ್ಷರ *old_ಸೂಚ್ಯಂಕ_env = ಖಾಲಿ, *old_repo_ಸೂಚ್ಯಂಕ_file;

	remove_ಮಾರ್ಗ(stash_ಸೂಚ್ಯಂಕ_ಮಾರ್ಗ.buf);

	ತಲೆ_commit = lookup_commit(the_repository, &info->b_commit);
	ಆದರೆ (!ತಲೆ_commit || repo_parse_commit(the_repository, ತಲೆ_commit)) {
		ret = -1;
		ಹೋಗು ಮುಗಿದ;
	}
	ತಲೆ_ವೃಕ್ಷ = get_commit_ವೃಕ್ಷ_oid(ತಲೆ_commit);
	ಆದರೆ (!ತಲೆ_ವೃಕ್ಷ) {
		ret = -1;
		ಹೋಗು ಮುಗಿದ;
	}

	ಆದರೆ (create_ಸೂಚ್ಯಂಕ_from_ವೃಕ್ಷ(ತಲೆ_ವೃಕ್ಷ, stash_ಸೂಚ್ಯಂಕ_ಮಾರ್ಗ.buf)) {
		ret = -1;
		ಹೋಗು ಮುಗಿದ;
	}

	/* Find out what the user wants. */
	old_repo_ಸೂಚ್ಯಂಕ_file = the_repository->ಸೂಚ್ಯಂಕ_file;
	the_repository->ಸೂಚ್ಯಂಕ_file = stash_ಸೂಚ್ಯಂಕ_ಮಾರ್ಗ.buf;
	old_ಸೂಚ್ಯಂಕ_env = xstrdup_or_null(getenv(ಸೂಚ್ಯಂಕ_ENVIRONMENT));
	setenv(ಸೂಚ್ಯಂಕ_ENVIRONMENT, the_repository->ಸೂಚ್ಯಂಕ_file, 1);

	ret = !!ಸತತ_ಕೂಡಿಸು_p(the_repository, ಕೂಡಿಸು_P_STASH, interactive_opts, ಖಾಲಿ, ps, 0);

	the_repository->ಸೂಚ್ಯಂಕ_file = old_repo_ಸೂಚ್ಯಂಕ_file;
	ಆದರೆ (old_ಸೂಚ್ಯಂಕ_env && *old_ಸೂಚ್ಯಂಕ_env)
		setenv(ಸೂಚ್ಯಂಕ_ENVIRONMENT, old_ಸೂಚ್ಯಂಕ_env, 1);
	ಇಲ್ಲದಿದ್ದರೆ
		unsetenv(ಸೂಚ್ಯಂಕ_ENVIRONMENT);
	FREE_AND_NULL(old_ಸೂಚ್ಯಂಕ_env);

	/* State of the working tree. */
	ಆದರೆ (write_ಸೂಚ್ಯಂಕ_as_ವೃಕ್ಷ(&info->w_ವೃಕ್ಷ, &istate, stash_ಸೂಚ್ಯಂಕ_ಮಾರ್ಗ.buf, 0,
				ಖಾಲಿ)) {
		ret = -1;
		ಹೋಗು ಮುಗಿದ;
	}

	cp_diff_ವೃಕ್ಷ.git_cmd = 1;
	strvec_pushl(&cp_diff_ವೃಕ್ಷ.ವಾದಗಳು, "diff-tree", "-p", "-U1", "HEAD",
		     "--no-color",
		     oid_ಗಮ್ಯ_hex(&info->w_ವೃಕ್ಷ), "--", ಖಾಲಿ);
	ಆದರೆ (pipe_command(&cp_diff_ವೃಕ್ಷ, ಖಾಲಿ, 0, ಹೊರಹೋಗು_patch, 0, ಖಾಲಿ, 0)) {
		ret = -1;
		ಹೋಗು ಮುಗಿದ;
	}

	ಆದರೆ (!ಹೊರಹೋಗು_patch->len) {
		ಆದರೆ (!quiet)
			fprintf_ln(stderr, _("No changes selected"));
		ret = 1;
	}

ಮುಗಿದ:
	release_ಸೂಚ್ಯಂಕ(&istate);
	remove_ಮಾರ್ಗ(stash_ಸೂಚ್ಯಂಕ_ಮಾರ್ಗ.buf);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stash_working_ವೃಕ್ಷ(ರಚನೆ stash_info *info, ಸ್ಥಿರಾಂಕ ರಚನೆ pathspec *ps)
{
	ಪೂರ್ಣಾಂಕ ret = 0;
	ರಚನೆ rev_info rev;
	ರಚನೆ ಮಗು_process cp_upd_ಸೂಚ್ಯಂಕ = ಮಗು_PROCESS_INIT;
	ರಚನೆ strbuf diff_output = STRBUF_INIT;
	ರಚನೆ ಸೂಚ್ಯಂಕ_state istate = ಸೂಚ್ಯಂಕ_STATE_INIT(the_repository);

	repo_init_revisions(the_repository, &rev, ಖಾಲಿ);
	copy_pathspec(&rev.prune_ದತ್ತಾಂಶ, ps);

	ಗಣ_alternate_ಸೂಚ್ಯಂಕ_output(stash_ಸೂಚ್ಯಂಕ_ಮಾರ್ಗ.buf);
	ಆದರೆ (reset_ವೃಕ್ಷ(&info->i_ವೃಕ್ಷ, 0, 0)) {
		ret = -1;
		ಹೋಗು ಮುಗಿದ;
	}
	ಗಣ_alternate_ಸೂಚ್ಯಂಕ_output(ಖಾಲಿ);

	rev.diffopt.output_format = DIFF_FORMAT_CALLBACK;
	rev.diffopt.format_callback = ಕೂಡಿಸು_diff_ಗಮ್ಯ_buf;
	rev.diffopt.format_callback_ದತ್ತಾಂಶ = &diff_output;

	ಆದರೆ (repo_read_ಸೂಚ್ಯಂಕ_preload(the_repository, &rev.diffopt.pathspec, 0) < 0) {
		ret = -1;
		ಹೋಗು ಮುಗಿದ;
	}

	ಕೂಡಿಸು_pending_object(&rev, parse_object(the_repository, &info->b_commit),
			   "");
	ಸತತ_diff_ಸೂಚ್ಯಂಕ(&rev, 0);

	cp_upd_ಸೂಚ್ಯಂಕ.git_cmd = 1;
	strvec_pushl(&cp_upd_ಸೂಚ್ಯಂಕ.ವಾದಗಳು, "update-index",
		     "--ignore-skip-worktree-entries",
		     "-z", "--add", "--remove", "--stdin", ಖಾಲಿ);
	strvec_pushf(&cp_upd_ಸೂಚ್ಯಂಕ.env, "GIT_INDEX_FILE=%s",
		     stash_ಸೂಚ್ಯಂಕ_ಮಾರ್ಗ.buf);

	ಆದರೆ (pipe_command(&cp_upd_ಸೂಚ್ಯಂಕ, diff_output.buf, diff_output.len,
			 ಖಾಲಿ, 0, ಖಾಲಿ, 0)) {
		ret = -1;
		ಹೋಗು ಮುಗಿದ;
	}

	ಆದರೆ (write_ಸೂಚ್ಯಂಕ_as_ವೃಕ್ಷ(&info->w_ವೃಕ್ಷ, &istate, stash_ಸೂಚ್ಯಂಕ_ಮಾರ್ಗ.buf, 0,
				ಖಾಲಿ)) {
		ret = -1;
		ಹೋಗು ಮುಗಿದ;
	}

ಮುಗಿದ:
	release_ಸೂಚ್ಯಂಕ(&istate);
	release_revisions(&rev);
	strbuf_release(&diff_output);
	remove_ಮಾರ್ಗ(stash_ಸೂಚ್ಯಂಕ_ಮಾರ್ಗ.buf);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ do_create_stash(ಸ್ಥಿರಾಂಕ ರಚನೆ pathspec *ps, ರಚನೆ strbuf *stash_ಸಂದೇಶ_buf,
			   ಪೂರ್ಣಾಂಕ include_untracked, ಪೂರ್ಣಾಂಕ patch_ಬಹುಲಕ,
			   ರಚನೆ interactive_options *interactive_opts,
			   ಪೂರ್ಣಾಂಕ only_staged, ರಚನೆ stash_info *info, ರಚನೆ strbuf *patch,
			   ಪೂರ್ಣಾಂಕ quiet)
{
	ಪೂರ್ಣಾಂಕ ret = 0;
	ಪೂರ್ಣಾಂಕ flags = 0;
	ಪೂರ್ಣಾಂಕ untracked_commit_option = 0;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ತಲೆ_short_sha1 = ಖಾಲಿ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *branch_ref = ಖಾಲಿ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *branch_name = "(no branch)";
	ಅಕ್ಷರ *branch_name_buf = ಖಾಲಿ;
	ರಚನೆ commit *ತಲೆ_commit = ಖಾಲಿ;
	ರಚನೆ commit_list *parents = ಖಾಲಿ;
	ರಚನೆ strbuf ಸಂದೇಶ = STRBUF_INIT;
	ರಚನೆ strbuf commit_ವೃಕ್ಷ_label = STRBUF_INIT;
	ರಚನೆ strbuf untracked_files = STRBUF_INIT;

	prepare_fallback_ident("git stash", "git@stash");

	repo_read_ಸೂಚ್ಯಂಕ_preload(the_repository, ಖಾಲಿ, 0);
	ಆದರೆ (repo_refresh_and_write_ಸೂಚ್ಯಂಕ(the_repository, REFRESH_QUIET, 0, 0,
					 ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ) < 0) {
		ret = error(_("could not write index"));
		ಹೋಗು ಮುಗಿದ;
	}

	ಆದರೆ (repo_get_oid(the_repository, "HEAD", &info->b_commit)) {
		ಆದರೆ (!quiet)
			fprintf_ln(stderr, _("You do not have "
					     "the initial commit yet"));
		ret = -1;
		ಹೋಗು ಮುಗಿದ;
	} ಇಲ್ಲದಿದ್ದರೆ {
		ತಲೆ_commit = lookup_commit(the_repository, &info->b_commit);
	}

	ಆದರೆ (!check_changes(ps, include_untracked, &untracked_files)) {
		ret = 1;
		ಹೋಗು ಮುಗಿದ;
	}

	branch_ref = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
					     "HEAD", 0, ಖಾಲಿ, &flags);

	ಆದರೆ (flags & REF_ISSYMREF) {
		ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(branch_ref, "refs/heads/", &branch_name))
			branch_name = branch_name_buf = xstrdup(branch_name);
	}

	ತಲೆ_short_sha1 = repo_ಕಂಡುಹಿಡಿ_unique_abbrev(the_repository,
						  &ತಲೆ_commit->object.oid,
						  DEFAULT_ABBREV);
	strbuf_addf(&ಸಂದೇಶ, "%s: %s ", branch_name, ತಲೆ_short_sha1);
	pp_commit_easy(CMIT_FMT_ONELINE, ತಲೆ_commit, &ಸಂದೇಶ);

	strbuf_addf(&commit_ವೃಕ್ಷ_label, "index on %s\n", ಸಂದೇಶ.buf);
	commit_list_ಸೇರಿಸು(ತಲೆ_commit, &parents);
	ಆದರೆ (write_ಸೂಚ್ಯಂಕ_as_ವೃಕ್ಷ(&info->i_ವೃಕ್ಷ, the_repository->ಸೂಚ್ಯಂಕ,
				repo_get_ಸೂಚ್ಯಂಕ_file(the_repository), 0, ಖಾಲಿ) ||
	    commit_ವೃಕ್ಷ(commit_ವೃಕ್ಷ_label.buf, commit_ವೃಕ್ಷ_label.len,
			&info->i_ವೃಕ್ಷ, parents, &info->i_commit, ಖಾಲಿ, ಖಾಲಿ)) {
		ಆದರೆ (!quiet)
			fprintf_ln(stderr, _("Cannot save the current "
					     "index state"));
		ret = -1;
		ಹೋಗು ಮುಗಿದ;
	}

	commit_list_free(parents);
	parents = ಖಾಲಿ;

	ಆದರೆ (include_untracked) {
		ಆದರೆ (save_untracked_files(info, &ಸಂದೇಶ, &untracked_files)) {
			ಆದರೆ (!quiet)
				fprintf_ln(stderr, _("Cannot save "
						     "the untracked files"));
			ret = -1;
			ಹೋಗು ಮುಗಿದ;
		}
		untracked_commit_option = 1;
	}
	ಆದರೆ (patch_ಬಹುಲಕ) {
		ret = stash_patch(info, ps, patch, quiet, interactive_opts);
		ಆದರೆ (ret < 0) {
			ಆದರೆ (!quiet)
				fprintf_ln(stderr, _("Cannot save the current "
						     "worktree state"));
			ಹೋಗು ಮುಗಿದ;
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ret > 0) {
			ಹೋಗು ಮುಗಿದ;
		}
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (only_staged) {
		ret = stash_staged(info, patch, quiet);
		ಆದರೆ (ret < 0) {
			ಆದರೆ (!quiet)
				fprintf_ln(stderr, _("Cannot save the current "
						     "staged state"));
			ಹೋಗು ಮುಗಿದ;
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ret > 0) {
			ಹೋಗು ಮುಗಿದ;
		}
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಆದರೆ (stash_working_ವೃಕ್ಷ(info, ps)) {
			ಆದರೆ (!quiet)
				fprintf_ln(stderr, _("Cannot save the current "
						     "worktree state"));
			ret = -1;
			ಹೋಗು ಮುಗಿದ;
		}
	}

	ಆದರೆ (!stash_ಸಂದೇಶ_buf->len)
		strbuf_addf(stash_ಸಂದೇಶ_buf, "WIP on %s", ಸಂದೇಶ.buf);
	ಇಲ್ಲದಿದ್ದರೆ
		strbuf_insertf(stash_ಸಂದೇಶ_buf, 0, "On %s: ", branch_name);

	ಆದರೆ (untracked_commit_option)
		commit_list_ಸೇರಿಸು(lookup_commit(the_repository,
						 &info->u_commit),
				   &parents);
	commit_list_ಸೇರಿಸು(lookup_commit(the_repository, &info->i_commit),
			   &parents);
	commit_list_ಸೇರಿಸು(ತಲೆ_commit, &parents);

	ಆದರೆ (commit_ವೃಕ್ಷ(stash_ಸಂದೇಶ_buf->buf, stash_ಸಂದೇಶ_buf->len, &info->w_ವೃಕ್ಷ,
			parents, &info->w_commit, ಖಾಲಿ, ಖಾಲಿ)) {
		ಆದರೆ (!quiet)
			fprintf_ln(stderr, _("Cannot record "
					     "working tree state"));
		ret = -1;
		ಹೋಗು ಮುಗಿದ;
	}

ಮುಗಿದ:
	strbuf_release(&commit_ವೃಕ್ಷ_label);
	strbuf_release(&ಸಂದೇಶ);
	strbuf_release(&untracked_files);
	commit_list_free(parents);
	ಬಿಡುಗಡೆ(branch_name_buf);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ create_stash(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ ಬಳಸದ,
			ರಚನೆ repository *repo ಬಳಸದ)
{
	ಪೂರ್ಣಾಂಕ ret;
	ರಚನೆ strbuf stash_ಸಂದೇಶ_buf = STRBUF_INIT;
	ರಚನೆ stash_info info = STASH_INFO_INIT;
	ರಚನೆ pathspec ps;

	/* Starting with argv[1], since argv[0] is "create" */
	strbuf_join_argv(&stash_ಸಂದೇಶ_buf, argc - 1, ++argv, ' ');

	memset(&ps, 0, ಗಾತ್ರ(ps));
	ಆದರೆ (!check_changes_tracked_files(&ps))
		ಹಿಂತಿರುಗಿ 0;

	ret = do_create_stash(&ps, &stash_ಸಂದೇಶ_buf, 0, 0, ಖಾಲಿ, 0, &info,
			      ಖಾಲಿ, 0);
	ಆದರೆ (!ret)
		printf_ln("%s", oid_ಗಮ್ಯ_hex(&info.w_commit));

	free_stash_info(&info);
	strbuf_release(&stash_ಸಂದೇಶ_buf);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ do_ತಳ್ಳು_stash(ಸ್ಥಿರಾಂಕ ರಚನೆ pathspec *ps, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *stash_ಸಂದೇಶ, ಪೂರ್ಣಾಂಕ quiet,
			 ಪೂರ್ಣಾಂಕ keep_ಸೂಚ್ಯಂಕ, ಪೂರ್ಣಾಂಕ patch_ಬಹುಲಕ,
			 ರಚನೆ interactive_options *interactive_opts,
			 ಪೂರ್ಣಾಂಕ include_untracked, ಪೂರ್ಣಾಂಕ only_staged)
{
	ಪೂರ್ಣಾಂಕ ret = 0;
	ರಚನೆ stash_info info = STASH_INFO_INIT;
	ರಚನೆ strbuf patch = STRBUF_INIT;
	ರಚನೆ strbuf stash_ಸಂದೇಶ_buf = STRBUF_INIT;
	ರಚನೆ strbuf untracked_files = STRBUF_INIT;
	ರಚನೆ strbuf ಹೊರಹೋಗು = STRBUF_INIT;

	ಆದರೆ (patch_ಬಹುಲಕ && keep_ಸೂಚ್ಯಂಕ == -1)
		keep_ಸೂಚ್ಯಂಕ = 1;

	ಆದರೆ (patch_ಬಹುಲಕ && include_untracked) {
		fprintf_ln(stderr, _("Can't use --patch and --include-untracked"
				     " or --all at the same time"));
		ret = -1;
		ಹೋಗು ಮುಗಿದ;
	}

	/* --patch overrides --staged */
	ಆದರೆ (patch_ಬಹುಲಕ)
		only_staged = 0;

	ಆದರೆ (only_staged && include_untracked) {
		fprintf_ln(stderr, _("Can't use --staged and --include-untracked"
				     " or --all at the same time"));
		ret = -1;
		ಹೋಗು ಮುಗಿದ;
	}

	repo_read_ಸೂಚ್ಯಂಕ_preload(the_repository, ಖಾಲಿ, 0);
	ಆದರೆ (!include_untracked && ps->nr) {
		ಅಕ್ಷರ *ps_matched = xcalloc(ps->nr, 1);

		ಆದರೆ (pathspec_needs_expanded_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ, ps))
			ensure_ತುಂಬಿದ_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ);
		ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < the_repository->ಸೂಚ್ಯಂಕ->cache_nr; i++)
			ce_ಮಾರ್ಗ_ಹೊಂದಿಕೆ(the_repository->ಸೂಚ್ಯಂಕ, the_repository->ಸೂಚ್ಯಂಕ->cache[i], ps,
				      ps_matched);

		ಆದರೆ (report_ಮಾರ್ಗ_error(ps_matched, ps)) {
			fprintf_ln(stderr, _("Did you forget to 'git add'?"));
			ret = -1;
			ಬಿಡುಗಡೆ(ps_matched);
			ಹೋಗು ಮುಗಿದ;
		}
		ಬಿಡುಗಡೆ(ps_matched);
	}

	ಆದರೆ (repo_refresh_and_write_ಸೂಚ್ಯಂಕ(the_repository, REFRESH_QUIET, 0, 0,
					 ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ)) {
		ret = error(_("could not write index"));
		ಹೋಗು ಮುಗಿದ;
	}

	ಆದರೆ (!check_changes(ps, include_untracked, &untracked_files)) {
		ಆದರೆ (!quiet)
			printf_ln(_("No local changes to save"));
		ಹೋಗು ಮುಗಿದ;
	}

	ಆದರೆ (!refs_reflog_exists(get_main_ref_store(the_repository), ref_stash) && do_clear_stash()) {
		ret = -1;
		ಆದರೆ (!quiet)
			fprintf_ln(stderr, _("Cannot initialize stash"));
		ಹೋಗು ಮುಗಿದ;
	}

	ಆದರೆ (stash_ಸಂದೇಶ)
		strbuf_addstr(&stash_ಸಂದೇಶ_buf, stash_ಸಂದೇಶ);
	ಆದರೆ (do_create_stash(ps, &stash_ಸಂದೇಶ_buf, include_untracked, patch_ಬಹುಲಕ,
			    interactive_opts, only_staged, &info, &patch, quiet)) {
		ret = -1;
		ಹೋಗು ಮುಗಿದ;
	}

	ಆದರೆ (do_store_stash(&info.w_commit, stash_ಸಂದೇಶ_buf.buf, 1)) {
		ret = -1;
		ಆದರೆ (!quiet)
			fprintf_ln(stderr, _("Cannot save the current status"));
		ಹೋಗು ಮುಗಿದ;
	}

	ಆದರೆ (!quiet)
		printf_ln(_("Saved working directory and index state %s"),
			  stash_ಸಂದೇಶ_buf.buf);

	ಆದರೆ (!(patch_ಬಹುಲಕ || only_staged)) {
		ಆದರೆ (include_untracked && !ps->nr) {
			ರಚನೆ ಮಗು_process cp = ಮಗು_PROCESS_INIT;

			cp.git_cmd = 1;
			ಆದರೆ (startup_info->original_cwd) {
				cp.dir = startup_info->original_cwd;
				strvec_pushf(&cp.env, "%s=%s",
					     GIT_WORK_ವೃಕ್ಷ_ENVIRONMENT,
					     the_repository->worktree);
			}
			strvec_pushl(&cp.ವಾದಗಳು, "clean", "--force",
				     "--quiet", "-d", ":/", ಖಾಲಿ);
			ಆದರೆ (include_untracked == INCLUDE_ALL_FILES)
				strvec_ತಳ್ಳು(&cp.ವಾದಗಳು, "-x");
			ಆದರೆ (ಸತತ_command(&cp)) {
				ret = -1;
				ಹೋಗು ಮುಗಿದ;
			}
		}
		discard_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ);
		ಆದರೆ (ps->nr) {
			ರಚನೆ ಮಗು_process cp_ಕೂಡಿಸು = ಮಗು_PROCESS_INIT;
			ರಚನೆ ಮಗು_process cp_diff = ಮಗು_PROCESS_INIT;
			ರಚನೆ ಮಗು_process cp_apply = ಮಗು_PROCESS_INIT;

			cp_ಕೂಡಿಸು.git_cmd = 1;
			strvec_ತಳ್ಳು(&cp_ಕೂಡಿಸು.ವಾದಗಳು, "add");
			ಆದರೆ (!include_untracked)
				strvec_ತಳ್ಳು(&cp_ಕೂಡಿಸು.ವಾದಗಳು, "-u");
			ಆದರೆ (include_untracked == INCLUDE_ALL_FILES)
				strvec_ತಳ್ಳು(&cp_ಕೂಡಿಸು.ವಾದಗಳು, "--force");
			strvec_ತಳ್ಳು(&cp_ಕೂಡಿಸು.ವಾದಗಳು, "--");
			ಕೂಡಿಸು_pathspecs(&cp_ಕೂಡಿಸು.ವಾದಗಳು, ps);
			ಆದರೆ (ಸತತ_command(&cp_ಕೂಡಿಸು)) {
				ret = -1;
				ಹೋಗು ಮುಗಿದ;
			}

			cp_diff.git_cmd = 1;
			strvec_pushl(&cp_diff.ವಾದಗಳು, "diff-index", "-p",
				     "--no-color",
				     "--cached", "--binary", "HEAD", "--",
				     ಖಾಲಿ);
			ಕೂಡಿಸು_pathspecs(&cp_diff.ವಾದಗಳು, ps);
			ಆದರೆ (pipe_command(&cp_diff, ಖಾಲಿ, 0, &ಹೊರಹೋಗು, 0, ಖಾಲಿ, 0)) {
				ret = -1;
				ಹೋಗು ಮುಗಿದ;
			}

			cp_apply.git_cmd = 1;
			strvec_pushl(&cp_apply.ವಾದಗಳು, "apply", "--index",
				     "-R", ಖಾಲಿ);
			ಆದರೆ (pipe_command(&cp_apply, ಹೊರಹೋಗು.buf, ಹೊರಹೋಗು.len, ಖಾಲಿ, 0,
					 ಖಾಲಿ, 0)) {
				ret = -1;
				ಹೋಗು ಮುಗಿದ;
			}
		} ಇಲ್ಲದಿದ್ದರೆ {
			ರಚನೆ ಮಗು_process cp = ಮಗು_PROCESS_INIT;
			cp.git_cmd = 1;
			/* BUG: this nukes untracked files in the way */
			strvec_pushl(&cp.ವಾದಗಳು, "reset", "--hard", "-q",
				     "--no-recurse-submodules", ಖಾಲಿ);
			ಆದರೆ (ಸತತ_command(&cp)) {
				ret = -1;
				ಹೋಗು ಮುಗಿದ;
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
		ಆದರೆ (keep_ಸೂಚ್ಯಂಕ == 1 && !is_null_oid(&info.i_ವೃಕ್ಷ) &&
		    !is_ಖಾಲಿಗಳು_ವೃಕ್ಷ_oid(&info.i_ವೃಕ್ಷ, the_repository->hash_algo)) {
			ರಚನೆ ಮಗು_process cp = ಮಗು_PROCESS_INIT;

			cp.git_cmd = 1;
			strvec_pushl(&cp.ವಾದಗಳು, "checkout", "--no-overlay",
				     oid_ಗಮ್ಯ_hex(&info.i_ವೃಕ್ಷ), "--", ಖಾಲಿ);
			ಆದರೆ (!ps->nr)
				strvec_ತಳ್ಳು(&cp.ವಾದಗಳು, ":/");
			ಇಲ್ಲದಿದ್ದರೆ
				ಕೂಡಿಸು_pathspecs(&cp.ವಾದಗಳು, ps);
			ಆದರೆ (ಸತತ_command(&cp)) {
				ret = -1;
				ಹೋಗು ಮುಗಿದ;
			}
		}
		ಹೋಗು ಮುಗಿದ;
	} ಇಲ್ಲದಿದ್ದರೆ {
		ರಚನೆ ಮಗು_process cp = ಮಗು_PROCESS_INIT;

		cp.git_cmd = 1;
		strvec_pushl(&cp.ವಾದಗಳು, "apply", "-R", ಖಾಲಿ);

		ಆದರೆ (pipe_command(&cp, patch.buf, patch.len, ಖಾಲಿ, 0, ಖಾಲಿ, 0)) {
			ಆದರೆ (!quiet)
				fprintf_ln(stderr, _("Cannot remove "
						     "worktree changes"));
			ret = -1;
			ಹೋಗು ಮುಗಿದ;
		}

		ಆದರೆ (keep_ಸೂಚ್ಯಂಕ < 1) {
			ರಚನೆ ಮಗು_process cp = ಮಗು_PROCESS_INIT;

			cp.git_cmd = 1;
			strvec_pushl(&cp.ವಾದಗಳು, "reset", "-q", "--refresh", "--",
				     ಖಾಲಿ);
			ಕೂಡಿಸು_pathspecs(&cp.ವಾದಗಳು, ps);
			ಆದರೆ (ಸತತ_command(&cp)) {
				ret = -1;
				ಹೋಗು ಮುಗಿದ;
			}
		}
		ಹೋಗು ಮುಗಿದ;
	}

ಮುಗಿದ:
	strbuf_release(&patch);
	strbuf_release(&ಹೊರಹೋಗು);
	free_stash_info(&info);
	strbuf_release(&stash_ಸಂದೇಶ_buf);
	strbuf_release(&untracked_files);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ತಳ್ಳು_stash(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		      ಪೂರ್ಣಾಂಕ ತಳ್ಳು_assumed)
{
	ಪೂರ್ಣಾಂಕ force_assume = 0;
	ಪೂರ್ಣಾಂಕ keep_ಸೂಚ್ಯಂಕ = -1;
	ಪೂರ್ಣಾಂಕ only_staged = 0;
	ಪೂರ್ಣಾಂಕ patch_ಬಹುಲಕ = 0;
	ಪೂರ್ಣಾಂಕ include_untracked = 0;
	ಪೂರ್ಣಾಂಕ quiet = 0;
	ಪೂರ್ಣಾಂಕ pathspec_file_nul = 0;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *stash_ಸಂದೇಶ = ಖಾಲಿ;
	ಅಕ್ಷರ *pathspec_from_file = ಖಾಲಿ;
	ರಚನೆ pathspec ps;
	ರಚನೆ interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	ರಚನೆ option options[] = {
		OPT_BOOL('k', "keep-index", &keep_ಸೂಚ್ಯಂಕ,
			 N_("keep index")),
		OPT_BOOL('S', "staged", &only_staged,
			 N_("stash staged changes only")),
		OPT_BOOL('p', "patch", &patch_ಬಹುಲಕ,
			 N_("stash in patch mode")),
		OPT_BOOL(0, "auto-advance", &interactive_opts.auto_advance,
			 N_("auto advance to the next file when selecting hunks interactively")),
		OPT_DIFF_UNIFIED(&interactive_opts.context),
		OPT_DIFF_INTERHUNK_CONTEXT(&interactive_opts.interhunkcontext),
		OPT__QUIET(&quiet, N_("quiet mode")),
		OPT_BOOL('u', "include-untracked", &include_untracked,
			 N_("include untracked files in stash")),
		OPT_ಗಣ_INT('a', "all", &include_untracked,
			    N_("include ignore files"), INCLUDE_ALL_FILES),
		OPT_STRING('m', "message", &stash_ಸಂದೇಶ, N_("message"),
			   N_("stash message")),
		OPT_PATHSPEC_FROM_FILE(&pathspec_from_file),
		OPT_PATHSPEC_FILE_NUL(&pathspec_file_nul),
		OPT_ಕೊನೆ()
	};
	ಪೂರ್ಣಾಂಕ ret;

	ಆದರೆ (argc) {
		ಪೂರ್ಣಾಂಕ flags = PARSE_OPT_KEEP_DASHDASH;

		ಆದರೆ (ತಳ್ಳು_assumed)
			flags |= PARSE_OPT_STOP_AT_NON_OPTION;

		argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options,
				     ತಳ್ಳು_assumed ? git_stash_usage :
				     git_stash_ತಳ್ಳು_usage, flags);
		force_assume |= patch_ಬಹುಲಕ;
	}

	ಆದರೆ (argc) {
		ಆದರೆ (!strcmp(argv[0], "--")) {
			argc--;
			argv++;
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ತಳ್ಳು_assumed && !force_assume) {
			die("subcommand wasn't specified; 'push' can't be assumed due to unexpected token '%s'",
			    argv[0]);
		}
	}

	parse_pathspec(&ps, 0, PATHSPEC_PREFER_ತುಂಬಿದ | PATHSPEC_ಪೂರ್ವಪ್ರತ್ಯಯ_ORIGIN,
		       ಪೂರ್ವಪ್ರತ್ಯಯ, argv);

	ಆದರೆ (pathspec_from_file) {
		ಆದರೆ (patch_ಬಹುಲಕ)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--patch");

		ಆದರೆ (only_staged)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--staged");

		ಆದರೆ (ps.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&ps, 0,
				    PATHSPEC_PREFER_ತುಂಬಿದ | PATHSPEC_ಪೂರ್ವಪ್ರತ್ಯಯ_ORIGIN,
				    ಪೂರ್ವಪ್ರತ್ಯಯ, pathspec_from_file, pathspec_file_nul);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	ಆದರೆ (!patch_ಬಹುಲಕ) {
		ಆದರೆ (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--patch");
		ಆದರೆ (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--patch");
		ಆದರೆ (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--patch");
	}

	ಆದರೆ (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	ಆದರೆ (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	ret = do_ತಳ್ಳು_stash(&ps, stash_ಸಂದೇಶ, quiet, keep_ಸೂಚ್ಯಂಕ, patch_ಬಹುಲಕ,
			    &interactive_opts, include_untracked, only_staged);

	clear_pathspec(&ps);
	ಬಿಡುಗಡೆ(pathspec_from_file);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ತಳ್ಳು_stash_unassumed(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
				ರಚನೆ repository *repo ಬಳಸದ)
{
	ಹಿಂತಿರುಗಿ ತಳ್ಳು_stash(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, 0);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ save_stash(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		      ರಚನೆ repository *repo ಬಳಸದ)
{
	ಪೂರ್ಣಾಂಕ keep_ಸೂಚ್ಯಂಕ = -1;
	ಪೂರ್ಣಾಂಕ only_staged = 0;
	ಪೂರ್ಣಾಂಕ patch_ಬಹುಲಕ = 0;
	ಪೂರ್ಣಾಂಕ include_untracked = 0;
	ಪೂರ್ಣಾಂಕ quiet = 0;
	ಪೂರ್ಣಾಂಕ ret = 0;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *stash_ಸಂದೇಶ = ಖಾಲಿ;
	ರಚನೆ pathspec ps;
	ರಚನೆ strbuf stash_ಸಂದೇಶ_buf = STRBUF_INIT;
	ರಚನೆ interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	ರಚನೆ option options[] = {
		OPT_BOOL('k', "keep-index", &keep_ಸೂಚ್ಯಂಕ,
			 N_("keep index")),
		OPT_BOOL('S', "staged", &only_staged,
			 N_("stash staged changes only")),
		OPT_BOOL('p', "patch", &patch_ಬಹುಲಕ,
			 N_("stash in patch mode")),
		OPT_BOOL(0, "auto-advance", &interactive_opts.auto_advance,
			 N_("auto advance to the next file when selecting hunks interactively")),
		OPT_DIFF_UNIFIED(&interactive_opts.context),
		OPT_DIFF_INTERHUNK_CONTEXT(&interactive_opts.interhunkcontext),
		OPT__QUIET(&quiet, N_("quiet mode")),
		OPT_BOOL('u', "include-untracked", &include_untracked,
			 N_("include untracked files in stash")),
		OPT_ಗಣ_INT('a', "all", &include_untracked,
			    N_("include ignore files"), INCLUDE_ALL_FILES),
		OPT_STRING('m', "message", &stash_ಸಂದೇಶ, "message",
			   N_("stash message")),
		OPT_ಕೊನೆ()
	};

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options,
			     git_stash_save_usage,
			     PARSE_OPT_KEEP_DASHDASH);

	ಆದರೆ (argc)
		stash_ಸಂದೇಶ = strbuf_join_argv(&stash_ಸಂದೇಶ_buf, argc, argv, ' ');

	memset(&ps, 0, ಗಾತ್ರ(ps));

	ಆದರೆ (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	ಆದರೆ (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	ಆದರೆ (!patch_ಬಹುಲಕ) {
		ಆದರೆ (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--patch");
		ಆದರೆ (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--patch");
		ಆದರೆ (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--patch");
	}

	ret = do_ತಳ್ಳು_stash(&ps, stash_ಸಂದೇಶ, quiet, keep_ಸೂಚ್ಯಂಕ,
			    patch_ಬಹುಲಕ, &interactive_opts, include_untracked,
			    only_staged);

	strbuf_release(&stash_ಸಂದೇಶ_buf);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ write_commit_with_parents(ರಚನೆ repository *r,
				     ರಚನೆ object_id *ಹೊರಹೋಗು,
				     ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid,
				     ರಚನೆ commit_list *parents)
{
	ಗಾತ್ರ_t author_len, committer_len;
	ರಚನೆ commit *this;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *orig_author, *orig_committer;
	ಅಕ್ಷರ *author = ಖಾಲಿ, *committer = ಖಾಲಿ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಬಫರ್;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ bufsize;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *p;
	ರಚನೆ strbuf ಸಂದೇಶ = STRBUF_INIT;
	ಪೂರ್ಣಾಂಕ ret = 0;
	ರಚನೆ ident_ವಿಭಜಿಸು id;

	this = lookup_commit_reference(r, oid);
	ಬಫರ್ = repo_get_commit_ಬಫರ್(r, this, &bufsize);
	orig_author = ಕಂಡುಹಿಡಿ_commit_ಶೀರ್ಷಿಕೆ(ಬಫರ್, "author", &author_len);
	orig_committer = ಕಂಡುಹಿಡಿ_commit_ಶೀರ್ಷಿಕೆ(ಬಫರ್, "committer", &committer_len);

	ಆದರೆ (!orig_author || !orig_committer) {
		ret = error(_("cannot parse commit %s"), oid_ಗಮ್ಯ_hex(oid));
		ಹೋಗು ಹೊರಹೋಗು;
	}

	ಆದರೆ (ವಿಭಜಿಸು_ident_ಸಾಲು(&id, orig_author, author_len) < 0 ||
	    ವಿಭಜಿಸು_ident_ಸಾಲು(&id, orig_committer, committer_len) < 0) {
		ret = error(_("invalid author or committer for %s"), oid_ಗಮ್ಯ_hex(oid));
		ಹೋಗು ಹೊರಹೋಗು;
	}

	p = ಅಕ್ಷರಸರಣಿ_ಹುಡುಕು(ಬಫರ್, "\n\n");
	strbuf_addstr(&ಸಂದೇಶ, "git stash: ");

	ಆದರೆ (p)
		strbuf_ಕೂಡಿಸು(&ಸಂದೇಶ, p + 2, bufsize - (p + 2 - ಬಫರ್));
	strbuf_complete_ಸಾಲು(&ಸಂದೇಶ);

	author = xmemdupz(orig_author, author_len);
	committer = xmemdupz(orig_committer, committer_len);

	ಆದರೆ (commit_ವೃಕ್ಷ_extended(ಸಂದೇಶ.buf, ಸಂದೇಶ.len,
				 r->hash_algo->ಖಾಲಿಗಳು_ವೃಕ್ಷ, parents,
				 ಹೊರಹೋಗು, author, committer,
				 ಖಾಲಿ, ಖಾಲಿ)) {
		ret = error(_("could not write commit"));
		ಹೋಗು ಹೊರಹೋಗು;
	}
ಹೊರಹೋಗು:
	strbuf_release(&ಸಂದೇಶ);
	repo_unuse_commit_ಬಫರ್(r, this, ಬಫರ್);
	ಬಿಡುಗಡೆ(author);
	ಬಿಡುಗಡೆ(committer);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ do_import_stash(ರಚನೆ repository *r, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rev)
{
	ರಚನೆ object_id chain;
	ಪೂರ್ಣಾಂಕ res = 0;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಬಫರ್ = ಖಾಲಿ;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ bufsize;
	ರಚನೆ commit *this = ಖಾಲಿ;
	ರಚನೆ commit_list *ವಸ್ತುಗಳು = ಖಾಲಿ, *cur;
	ಅಕ್ಷರ *ಸಂದೇಶ = ಖಾಲಿ;

	ಆದರೆ (repo_get_oid(r, rev, &chain))
		ಹಿಂತಿರುಗಿ error(_("not a valid revision: %s"), rev);

	this = lookup_commit_reference(r, &chain);
	ಆದರೆ (!this)
		ಹಿಂತಿರುಗಿ error(_("not a commit: %s"), rev);

	/*
	 * Walk the commit history, finding each stash entry, and load data into
	 * the array.
	 */
	ಗಾಗಿ (;;) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *author, *committer;
		ಗಾತ್ರ_t author_len, committer_len;
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *p;
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *expected = "git stash <git@stash> 1000684800 +0000";
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ = "git stash: ";
		ರಚನೆ commit *stash;
		ರಚನೆ ವೃಕ್ಷ *ವೃಕ್ಷ = repo_get_commit_ವೃಕ್ಷ(r, this);

		ಆದರೆ (!ವೃಕ್ಷ ||
		    !oideq(&ವೃಕ್ಷ->object.oid, r->hash_algo->ಖಾಲಿಗಳು_ವೃಕ್ಷ) ||
		    (this->parents &&
		     (!this->parents->ಮುಂದಿನ || this->parents->ಮುಂದಿನ->ಮುಂದಿನ))) {
			res = error(_("%s is not a valid exported stash commit"),
					oid_ಗಮ್ಯ_hex(&this->object.oid));
			ಹೋಗು ಹೊರಹೋಗು;
		}

		ಬಫರ್ = repo_get_commit_ಬಫರ್(r, this, &bufsize);

		ಆದರೆ (!this->parents) {
			/*
			 * We don't have any parents.  Make sure this is our
			 * root commit.
			 */
			author = ಕಂಡುಹಿಡಿ_commit_ಶೀರ್ಷಿಕೆ(ಬಫರ್, "author", &author_len);
			committer = ಕಂಡುಹಿಡಿ_commit_ಶೀರ್ಷಿಕೆ(ಬಫರ್, "committer", &committer_len);

			ಆದರೆ (!author || !committer) {
				error(_("cannot parse commit %s"), oid_ಗಮ್ಯ_hex(&this->object.oid));
				ಹೋಗು ಹೊರಹೋಗು;
			}

			ಆದರೆ (author_len != ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(expected) ||
			    committer_len != ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(expected) ||
			    memcmp(author, expected, author_len) ||
			    memcmp(committer, expected, committer_len)) {
				res = error(_("found root commit %s with invalid data"), oid_ಗಮ್ಯ_hex(&this->object.oid));
				ಹೋಗು ಹೊರಹೋಗು;
			}
			ನಿಲ್ಲಿಸು;
		}

		p = ಅಕ್ಷರಸರಣಿ_ಹುಡುಕು(ಬಫರ್, "\n\n");
		ಆದರೆ (!p) {
			res = error(_("cannot parse commit %s"), oid_ಗಮ್ಯ_hex(&this->object.oid));
			ಹೋಗು ಹೊರಹೋಗು;
		}

		p += 2;
		ಆದರೆ (((ಗಾತ್ರ_t)(bufsize - (p - ಬಫರ್)) < ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ಪೂರ್ವಪ್ರತ್ಯಯ)) ||
		    memcmp(ಪೂರ್ವಪ್ರತ್ಯಯ, p, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ಪೂರ್ವಪ್ರತ್ಯಯ))) {
			res = error(_("found stash commit %s without expected prefix"), oid_ಗಮ್ಯ_hex(&this->object.oid));
			ಹೋಗು ಹೊರಹೋಗು;
		}

		stash = this->parents->ಮುಂದಿನ->ಅಂಶ;

		ಆದರೆ (repo_parse_commit(r, this->parents->ಅಂಶ) ||
		    repo_parse_commit(r, stash)) {
			res = error(_("cannot parse parents of commit: %s"),
					oid_ಗಮ್ಯ_hex(&this->object.oid));
			ಹೋಗು ಹೊರಹೋಗು;
		}

		ಆದರೆ (check_stash_topology(r, stash)) {
			res = error(_("%s does not look like a stash commit"),
					oid_ಗಮ್ಯ_hex(&stash->object.oid));
			ಹೋಗು ಹೊರಹೋಗು;
		}

		repo_unuse_commit_ಬಫರ್(r, this, ಬಫರ್);
		ಬಫರ್ = ಖಾಲಿ;
		ವಸ್ತುಗಳು = commit_list_ಸೇರಿಸು(stash, &ವಸ್ತುಗಳು);
		this = this->parents->ಅಂಶ;
	}

	/*
	 * Now, walk each entry, adding it to the stash as a normal stash
	 * commit.
	 */
	ಗಾಗಿ (cur = ವಸ್ತುಗಳು; cur; cur = cur->ಮುಂದಿನ) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *p;
		ರಚನೆ object_id *oid;

		this = cur->ಅಂಶ;
		oid = &this->object.oid;
		ಬಫರ್ = repo_get_commit_ಬಫರ್(r, this, &bufsize);
		ಆದರೆ (!ಬಫರ್) {
			res = error(_("cannot read commit buffer for %s"), oid_ಗಮ್ಯ_hex(oid));
			ಹೋಗು ಹೊರಹೋಗು;
		}

		p = ಅಕ್ಷರಸರಣಿ_ಹುಡುಕು(ಬಫರ್, "\n\n");
		ಆದರೆ (!p) {
			res = error(_("cannot parse commit %s"), oid_ಗಮ್ಯ_hex(oid));
			ಹೋಗು ಹೊರಹೋಗು;
		}

		p += 2;
		ಸಂದೇಶ = xmemdupz(p, bufsize - (p - ಬಫರ್));
		repo_unuse_commit_ಬಫರ್(r, this, ಬಫರ್);
		ಬಫರ್ = ಖಾಲಿ;

		ಆದರೆ (do_store_stash(oid, ಸಂದೇಶ, 1)) {
			res = error(_("cannot save the stash for %s"), oid_ಗಮ್ಯ_hex(oid));
			ಹೋಗು ಹೊರಹೋಗು;
		}
		FREE_AND_NULL(ಸಂದೇಶ);
	}
ಹೊರಹೋಗು:
	ಆದರೆ (this && ಬಫರ್)
		repo_unuse_commit_ಬಫರ್(r, this, ಬಫರ್);
	commit_list_free(ವಸ್ತುಗಳು);
	ಬಿಡುಗಡೆ(ಸಂದೇಶ);

	ಹಿಂತಿರುಗಿ res;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ import_stash(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			ರಚನೆ repository *repo)
{
	ರಚನೆ option options[] = {
		OPT_ಕೊನೆ()
	};

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options,
			     git_stash_import_usage,
			     PARSE_OPT_KEEP_DASHDASH);

	ಆದರೆ (argc != 1)
		usage_ಸಂದೇಶ_opt("a revision is required", git_stash_import_usage, options);

	ಹಿಂತಿರುಗಿ do_import_stash(repo, argv[0]);
}

ರಚನೆ stash_ನಮೂದು_ದತ್ತಾಂಶ {
	ರಚನೆ repository *r;
	ರಚನೆ commit_list **ವಸ್ತುಗಳು;
	ಗಾತ್ರ_t ಎಣಿಕೆ_2;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ collect_stash_entries(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *refname ಬಳಸದ,
				 ರಚನೆ object_id *old_oid ಬಳಸದ,
				 ರಚನೆ object_id *new_oid,
				 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *committer ಬಳಸದ,
				 timestamp_t timestamp ಬಳಸದ,
				 ಪೂರ್ಣಾಂಕ tz ಬಳಸದ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ ಬಳಸದ,
				 ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ)
{
	ರಚನೆ stash_ನಮೂದು_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ = cb_ದತ್ತಾಂಶ;
	ರಚನೆ commit *stash;

	ದತ್ತಾಂಶ->ಎಣಿಕೆ_2++;
	stash = lookup_commit_reference(ದತ್ತಾಂಶ->r, new_oid);
	ಆದರೆ (!stash || check_stash_topology(ದತ್ತಾಂಶ->r, stash)) {
		ಹಿಂತಿರುಗಿ error(_("%s does not look like a stash commit"),
			     oid_ಗಮ್ಯ_hex(new_oid));
	}
	ದತ್ತಾಂಶ->ವಸ್ತುಗಳು = commit_list_append(stash, ದತ್ತಾಂಶ->ವಸ್ತುಗಳು);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ do_export_stash(ರಚನೆ repository *r,
			   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ref,
			   ಪೂರ್ಣಾಂಕ argc,
			   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv)
{
	ರಚನೆ object_id ತಳ;
	ರಚನೆ commit *prev;
	ರಚನೆ commit_list *ವಸ್ತುಗಳು = ಖಾಲಿ, **iter = &ವಸ್ತುಗಳು, *cur;
	ಪೂರ್ಣಾಂಕ res = 0;
	ಪೂರ್ಣಾಂಕ i;
	ರಚನೆ strbuf revision = STRBUF_INIT;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *author, *committer;

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
	ಆದರೆ (commit_ವೃಕ್ಷ_extended("", 0, r->hash_algo->ಖಾಲಿಗಳು_ವೃಕ್ಷ, ಖಾಲಿ,
				 &ತಳ, author, committer, ಖಾಲಿ, ಖಾಲಿ))
		ಹಿಂತಿರುಗಿ error(_("unable to write base commit"));

	prev = lookup_commit_reference(r, &ತಳ);

	ಆದರೆ (argc) {
		/*
		 * Find each specified stash, and load data into the array.
		 */
		ಗಾಗಿ (i = 0; i < argc; i++) {
			ರಚನೆ object_id oid;
			ರಚನೆ commit *stash;

			ಆದರೆ (parse_stash_revision(&revision, argv[i], 1) ||
			    repo_get_oid_with_flags(r, revision.buf, &oid,
						    GET_OID_QUIETLY |
						    GET_OID_GENTLY)) {
				res = error(_("unable to find stash entry %s"), argv[i]);
				ಹೋಗು ಹೊರಹೋಗು;
			}

			stash = lookup_commit_reference(r, &oid);
			ಆದರೆ (!stash || check_stash_topology(r, stash)) {
				res = error(_("%s does not look like a stash commit"),
						revision.buf);
				ಹೋಗು ಹೊರಹೋಗು;
			}
			iter = commit_list_append(stash, iter);
		}
	} ಇಲ್ಲದಿದ್ದರೆ {
		/*
		 * Walk the reflog, finding each stash entry, and load data into the
		 * array.
		 */
		ರಚನೆ stash_ನಮೂದು_ದತ್ತಾಂಶ cb_ದತ್ತಾಂಶ = {
			.r = r, .ವಸ್ತುಗಳು = iter,
		};
		ಆದರೆ (refs_for_each_reflog_ent_ಹಿಮ್ಮುಖ(get_main_ref_store(r),
						     "refs/stash",
						     collect_stash_entries,
						     &cb_ದತ್ತಾಂಶ) && cb_ದತ್ತಾಂಶ.ಎಣಿಕೆ_2)
			ಹೋಗು ಹೊರಹೋಗು;
	}

	/*
	 * Now, create a set of commits identical to the regular stash commits,
	 * but where their first parents form a chain to our original empty
	 * base commit.
	 */
	ವಸ್ತುಗಳು = commit_list_ಹಿಮ್ಮುಖ(ವಸ್ತುಗಳು);
	ಗಾಗಿ (cur = ವಸ್ತುಗಳು; cur; cur = cur->ಮುಂದಿನ) {
		ರಚನೆ commit_list *parents = ಖಾಲಿ;
		ರಚನೆ commit_list **ಮುಂದಿನ = &parents;
		ರಚನೆ object_id ಹೊರಹೋಗು;
		ರಚನೆ commit *stash = cur->ಅಂಶ;

		ಮುಂದಿನ = commit_list_append(prev, ಮುಂದಿನ);
		ಮುಂದಿನ = commit_list_append(stash, ಮುಂದಿನ);
		res = write_commit_with_parents(r, &ಹೊರಹೋಗು, &stash->object.oid, parents);
		commit_list_free(parents);
		ಆದರೆ (res)
			ಹೋಗು ಹೊರಹೋಗು;
		prev = lookup_commit_reference(r, &ಹೊರಹೋಗು);
	}
	ಆದರೆ (ref)
		refs_update_ref(get_main_ref_store(r), ಖಾಲಿ, ref,
				&prev->object.oid, ಖಾಲಿ, 0, UPDATE_REFS_DIE_ON_ERR);
	ಇಲ್ಲದಿದ್ದರೆ
		puts(oid_ಗಮ್ಯ_hex(&prev->object.oid));
ಹೊರಹೋಗು:
	strbuf_release(&revision);
	commit_list_free(ವಸ್ತುಗಳು);

	ಹಿಂತಿರುಗಿ res;
}

ಎಣಿಕೆ export_action {
	ACTION_NONE,
	ACTION_PRINT,
	ACTION_ಗಮ್ಯ_REF,
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ export_stash(ಪೂರ್ಣಾಂಕ argc,
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			ರಚನೆ repository *repo)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ref = ಖಾಲಿ;
	ಎಣಿಕೆ export_action action = ACTION_NONE;
	ರಚನೆ option options[] = {
		OPT_CMDMODE(0, "print", &action,
			    N_("print the object ID instead of writing it to a ref"),
			    ACTION_PRINT),
		OPT_STRING(0, "to-ref", &ref, "ref",
			    N_("save the data to the given ref")),
		OPT_ಕೊನೆ()
	};

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options,
			     git_stash_export_usage,
			     PARSE_OPT_KEEP_DASHDASH);

	ಆದರೆ (ref && action == ACTION_NONE)
		action = ACTION_ಗಮ್ಯ_REF;

	ಆದರೆ (action == ACTION_NONE || (ref && action == ACTION_PRINT))
		ಹಿಂತಿರುಗಿ error(_("exactly one of --print and --to-ref is required"));

	ಹಿಂತಿರುಗಿ do_export_stash(repo, ref, argc, argv);
}

ಪೂರ್ಣಾಂಕ cmd_stash(ಪೂರ್ಣಾಂಕ argc,
	      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
	      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
	      ರಚನೆ repository *repo)
{
	pid_t pid = getpid();
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸೂಚ್ಯಂಕ_file;
	ರಚನೆ strvec ವಾದಗಳು = STRVEC_INIT;
	parse_opt_subcommand_fn *fn = ಖಾಲಿ;
	ರಚನೆ option options[] = {
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
		OPT_SUBCOMMAND_F("save", &fn, save_stash, ಖಾಲಿ, PARSE_OPT_NOCOMPLETE),
		OPT_ಕೊನೆ()
	};
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **ವಾದಗಳು_copy;
	ಪೂರ್ಣಾಂಕ ret;

	repo_config(the_repository, git_stash_config, ಖಾಲಿ);

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options, git_stash_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL |
			     PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_ತುಂಬಿದ_ಸೂಚ್ಯಂಕ = 0;

	ಸೂಚ್ಯಂಕ_file = repo_get_ಸೂಚ್ಯಂಕ_file(the_repository);
	strbuf_addf(&stash_ಸೂಚ್ಯಂಕ_ಮಾರ್ಗ, "%s.stash.%" PRIuMAX, ಸೂಚ್ಯಂಕ_file,
		    (uintmax_t)pid);

	ಆದರೆ (fn) {
		ret = fn(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, repo);

		/*
		 * The subcommand implementations return 0 on success, a
		 * negative value on failure, and STASH_APPLY_CONFLICT
		 * when applying a stash entry resulted in conflicts.
		 * Map failures to 128, the status die() uses, so that
		 * exit status 1 unambiguously indicates conflicts.
		 */
		ಆದರೆ (ret < 0)
			ಹಿಂತಿರುಗಿ 128;
		ಹಿಂತಿರುಗಿ ret;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!argc) {
		ಹಿಂತಿರುಗಿ !!ತಳ್ಳು_stash_unassumed(0, ಖಾಲಿ, ಪೂರ್ವಪ್ರತ್ಯಯ, repo);
	}

	/* Assume 'stash push' */
	strvec_ತಳ್ಳು(&ವಾದಗಳು, "push");
	strvec_pushv(&ವಾದಗಳು, argv);

	/*
	 * `push_stash()` ends up modifying the array, which causes memory
	 * leaks if we didn't copy the array here.
	 */
	DUP_ARRAY(ವಾದಗಳು_copy, ವಾದಗಳು.v, ವಾದಗಳು.nr);

	ret = !!ತಳ್ಳು_stash(ವಾದಗಳು.nr, ವಾದಗಳು_copy, ಪೂರ್ವಪ್ರತ್ಯಯ, 1);

	strvec_clear(&ವಾದಗಳು);
	ಬಿಡುಗಡೆ(ವಾದಗಳು_copy);
	ಹಿಂತಿರುಗಿ ret;
}
