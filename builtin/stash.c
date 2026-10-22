#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "abspath.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hash.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hex.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-name.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "refs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "stash.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "lockfile.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cache-tree.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "unpack-trees.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "merge-ort-wrappers.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "strvec.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "run-command.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "dir.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "entry.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "preload-index.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "read-cache.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "repository.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "rerere.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "revision.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "setup.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "sparse-index.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "log-tree.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "diffcore.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "reflog.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "reflog-walk.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "add-interactive.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "commit.h"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ INCLUDE_ALL_FILES 2

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BUILTIN_STASH_LIST_USAGE \
	N_("git stash list [<log-options>]")
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BUILTIN_STASH_SHOW_USAGE \
	N_("git stash show [-u | --include-untracked | --only-untracked] [<diff-options>] [<stash>]")
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BUILTIN_STASH_DROP_USAGE \
	N_("git stash drop [-q | --quiet] [<stash>]")
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BUILTIN_STASH_ਕੱਢੋ_USAGE \
	N_("git stash pop [--index] [-q | --quiet] [<stash>]")
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BUILTIN_STASH_APPLY_USAGE \
	N_("git stash apply [--index] [-q | --quiet] [--label-ours=<label>] [--label-theirs=<label>] [--label-base=<label>] [<stash>]")
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BUILTIN_STASH_BRANCH_USAGE \
	N_("git stash branch <branchname> [<stash>]")
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BUILTIN_STASH_STORE_USAGE \
	N_("git stash store [(-m | --message) <message>] [-q | --quiet] <commit>")
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BUILTIN_STASH_ਧੱਕੋ_USAGE \
	N_("git stash [push] [-p | --patch] [-S | --staged] [-k | --[no-]keep-index] [-q | --quiet]\n" \
	   "          [-u | --include-untracked] [-a | --all] [(-m | --message) <message>]\n" \
	   "          [--pathspec-from-file=<file> [--pathspec-file-nul]]\n" \
	   "          [--] [<pathspec>...]")
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BUILTIN_STASH_SAVE_USAGE \
	N_("git stash save [-p | --patch] [-S | --staged] [-k | --[no-]keep-index] [-q | --quiet]\n" \
	   "          [-u | --include-untracked] [-a | --all] [<message>]")
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BUILTIN_STASH_CREATE_USAGE \
	N_("git stash create [<message>]")
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BUILTIN_STASH_EXPORT_USAGE \
	N_("git stash export (--print | --to-ref <ref>) [<stash>...]")
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BUILTIN_STASH_IMPORT_USAGE \
	N_("git stash import <commit>")
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BUILTIN_STASH_CLEAR_USAGE \
	"git stash clear"

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ git_stash_usage[] = {
	BUILTIN_STASH_LIST_USAGE,
	BUILTIN_STASH_SHOW_USAGE,
	BUILTIN_STASH_DROP_USAGE,
	BUILTIN_STASH_ਕੱਢੋ_USAGE,
	BUILTIN_STASH_APPLY_USAGE,
	BUILTIN_STASH_BRANCH_USAGE,
	BUILTIN_STASH_ਧੱਕੋ_USAGE,
	BUILTIN_STASH_SAVE_USAGE,
	BUILTIN_STASH_CLEAR_USAGE,
	BUILTIN_STASH_CREATE_USAGE,
	BUILTIN_STASH_STORE_USAGE,
	BUILTIN_STASH_EXPORT_USAGE,
	BUILTIN_STASH_IMPORT_USAGE,
	ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ git_stash_list_usage[] = {
	BUILTIN_STASH_LIST_USAGE,
	ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ git_stash_show_usage[] = {
	BUILTIN_STASH_SHOW_USAGE,
	ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ git_stash_drop_usage[] = {
	BUILTIN_STASH_DROP_USAGE,
	ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ git_stash_ਕੱਢੋ_usage[] = {
	BUILTIN_STASH_ਕੱਢੋ_USAGE,
	ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ git_stash_apply_usage[] = {
	BUILTIN_STASH_APPLY_USAGE,
	ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ git_stash_branch_usage[] = {
	BUILTIN_STASH_BRANCH_USAGE,
	ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ git_stash_clear_usage[] = {
	BUILTIN_STASH_CLEAR_USAGE,
	ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ git_stash_store_usage[] = {
	BUILTIN_STASH_STORE_USAGE,
	ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ git_stash_ਧੱਕੋ_usage[] = {
	BUILTIN_STASH_ਧੱਕੋ_USAGE,
	ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ git_stash_save_usage[] = {
	BUILTIN_STASH_SAVE_USAGE,
	ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ git_stash_export_usage[] = {
	BUILTIN_STASH_EXPORT_USAGE,
	ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ git_stash_import_usage[] = {
	BUILTIN_STASH_IMPORT_USAGE,
	ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ ref_stash[] = "refs/stash";
ਸਥਿਰ ਬਣਤਰ strbuf stash_ਸੂਚਕਾਂਕ_ਰਸਤਾ = STRBUF_INIT;

ਸਥਿਰ ਪੂਰਨ show_stat = 1;
ਸਥਿਰ ਪੂਰਨ show_patch;
ਸਥਿਰ ਪੂਰਨ show_include_untracked;
ਸਥਿਰ ਪੂਰਨ use_ਸੂਚਕਾਂਕ;

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
ਬਣਤਰ stash_info {
	ਬਣਤਰ object_id w_commit;
	ਬਣਤਰ object_id b_commit;
	ਬਣਤਰ object_id i_commit;
	ਬਣਤਰ object_id u_commit;
	ਬਣਤਰ object_id c_commit;
	ਬਣਤਰ object_id w_ਰੁੱਖ;
	ਬਣਤਰ object_id b_ਰੁੱਖ;
	ਬਣਤਰ object_id i_ਰੁੱਖ;
	ਬਣਤਰ object_id u_ਰੁੱਖ;
	ਬਣਤਰ strbuf revision;
	ਪੂਰਨ is_stash_ref;
	ਪੂਰਨ has_u;
};

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ STASH_INFO_INIT { \
	.revision = STRBUF_INIT, \
}

ਸਥਿਰ ਖਾਲੀ free_stash_info(ਬਣਤਰ stash_info *info)
{
	strbuf_release(&info->revision);
}

ਸਥਿਰ ਪੂਰਨ check_stash_topology(ਬਣਤਰ repository *r, ਬਣਤਰ commit *stash)
{
	ਬਣਤਰ commit *p1, *p2, *p3 = ਨਿਰਾ;

	/* stash must have two or three parents */
	ਜੇ (!stash->parents || !stash->parents->ਅਗਲਾ ||
			(stash->parents->ਅਗਲਾ->ਅਗਲਾ && stash->parents->ਅਗਲਾ->ਅਗਲਾ->ਅਗਲਾ))
		ਵਾਪਸ -1;
	p1 = stash->parents->ਵਸਤੂ;
	p2 = stash->parents->ਅਗਲਾ->ਵਸਤੂ;
	ਜੇ (stash->parents->ਅਗਲਾ->ਅਗਲਾ)
		p3 = stash->parents->ਅਗਲਾ->ਅਗਲਾ->ਵਸਤੂ;
	ਜੇ (repo_parse_commit(r, p1) || repo_parse_commit(r, p2) ||
			(p3 && repo_parse_commit(r, p3)))
		ਵਾਪਸ -1;
	/* p2 must have a single parent, p3 must have no parents */
	ਜੇ (!p2->parents || p2->parents->ਅਗਲਾ || (p3 && p3->parents))
		ਵਾਪਸ -1;
	ਜੇ (repo_parse_commit(r, p2->parents->ਵਸਤੂ))
		ਵਾਪਸ -1;
	/* p2^1 must equal p1 */
	ਜੇ (!oideq(&p1->object.oid, &p2->parents->ਵਸਤੂ->object.oid))
		ਵਾਪਸ -1;

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ assert_stash_like(ਬਣਤਰ stash_info *info, ਅਚਲ ਅੱਖਰ *revision)
{
	ਜੇ (get_oidf(&info->b_commit, "%s^1", revision) ||
	    get_oidf(&info->w_ਰੁੱਖ, "%s:", revision) ||
	    get_oidf(&info->b_ਰੁੱਖ, "%s^1:", revision) ||
	    get_oidf(&info->i_ਰੁੱਖ, "%s^2:", revision))
		die(_("'%s' is not a stash-like commit"), revision);
}

ਸਥਿਰ ਪੂਰਨ parse_stash_revision(ਬਣਤਰ strbuf *revision, ਅਚਲ ਅੱਖਰ *commit, ਪੂਰਨ quiet)
{
	strbuf_reset(revision);
	ਜੇ (!commit) {
		ਜੇ (!refs_ref_exists(get_main_ref_store(the_repository), ref_stash)) {
			ਜੇ (!quiet)
				fprintf_ln(stderr, _("No stash entries found."));
			ਵਾਪਸ -1;
		}

		strbuf_addf(revision, "%s@{0}", ref_stash);
	} ਨਹੀਂਤਾਂ ਜੇ (strspn(commit, "0123456789") == ਲੜੀ_ਲੰਬਾਈ(commit)) {
		strbuf_addf(revision, "%s@{%s}", ref_stash, commit);
	} ਨਹੀਂਤਾਂ {
		strbuf_addstr(revision, commit);
	}
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ get_stash_info(ਬਣਤਰ stash_info *info, ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv)
{
	ਪੂਰਨ ret;
	ਅੱਖਰ *ਸਿਰਾ_ਦਾ_rev;
	ਅੱਖਰ *expanded_ref;
	ਅਚਲ ਅੱਖਰ *revision;
	ਅਚਲ ਅੱਖਰ *commit = ਨਿਰਾ;
	ਬਣਤਰ object_id dummy;
	ਬਣਤਰ strbuf symbolic = STRBUF_INIT;

	ਜੇ (argc > 1) {
		ਪੂਰਨ i;
		ਬਣਤਰ strbuf refs_ਸੁਨੇਹਾ = STRBUF_INIT;

		ਲਈ (i = 0; i < argc; i++)
			strbuf_addf(&refs_ਸੁਨੇਹਾ, " '%s'", argv[i]);

		fprintf_ln(stderr, _("Too many revisions specified:%s"),
			   refs_ਸੁਨੇਹਾ.buf);
		strbuf_release(&refs_ਸੁਨੇਹਾ);

		ਵਾਪਸ -1;
	}

	ਜੇ (argc == 1)
		commit = argv[0];

	strbuf_init(&info->revision, 0);
	ਜੇ (parse_stash_revision(&info->revision, commit, 0)) {
		ਵਾਪਸ -1;
	}

	revision = info->revision.buf;

	ਜੇ (repo_get_oid(the_repository, revision, &info->w_commit))
		ਵਾਪਸ error(_("%s is not a valid reference"), revision);

	assert_stash_like(info, revision);

	info->has_u = !get_oidf(&info->u_ਰੁੱਖ, "%s^3:", revision);

	ਸਿਰਾ_ਦਾ_rev = strchrnul(revision, '@');
	strbuf_ਜੋੜੋ(&symbolic, revision, ਸਿਰਾ_ਦਾ_rev - revision);

	ret = repo_dwim_ref(the_repository, symbolic.buf, symbolic.len,
			    &dummy, &expanded_ref, 0);
	strbuf_release(&symbolic);
	ਚੋਣ (ret) {
	ਹਾਲਤ 0: /* Not found, but valid ref */
		info->is_stash_ref = 0;
		ਰੋਕੋ;
	ਹਾਲਤ 1:
		info->is_stash_ref = !strcmp(expanded_ref, ref_stash);
		ਰੋਕੋ;
	ਮੂਲ: /* Invalid or ambiguous */
		ਰੋਕੋ;
	}

	ਮੁਕਤ(expanded_ref);
	ਵਾਪਸ !(ret == 0 || ret == 1);
}

ਸਥਿਰ ਪੂਰਨ do_clear_stash(ਖਾਲੀ)
{
	ਬਣਤਰ object_id obj;
	ਜੇ (repo_get_oid(the_repository, ref_stash, &obj))
		ਵਾਪਸ 0;

	ਵਾਪਸ refs_delete_ref(get_main_ref_store(the_repository), ਨਿਰਾ,
			       ref_stash, &obj, 0);
}

ਸਥਿਰ ਪੂਰਨ clear_stash(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		       ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ option options[] = {
		OPT_ਸਿਰਾ()
	};

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options,
			     git_stash_clear_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	ਜੇ (argc)
		ਵਾਪਸ error(_("git stash clear with arguments is "
			       "unimplemented"));

	ਵਾਪਸ do_clear_stash();
}

ਸਥਿਰ ਪੂਰਨ reset_ਰੁੱਖ(ਬਣਤਰ object_id *i_ਰੁੱਖ, ਪੂਰਨ update, ਪੂਰਨ reset)
{
	ਪੂਰਨ nr_trees = 1;
	ਬਣਤਰ unpack_trees_options opts;
	ਬਣਤਰ ਰੁੱਖ_desc t[MAX_UNPACK_TREES];
	ਬਣਤਰ ਰੁੱਖ *ਰੁੱਖ;
	ਬਣਤਰ ਤਾਲਾ_file ਤਾਲਾ_file = ਤਾਲਾ_INIT;

	repo_read_ਸੂਚਕਾਂਕ_preload(the_repository, ਨਿਰਾ, 0);
	ਜੇ (refresh_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ, REFRESH_QUIET, ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ))
		ਵਾਪਸ -1;

	repo_hold_locked_ਸੂਚਕਾਂਕ(the_repository, &ਤਾਲਾ_file, ਤਾਲਾ_DIE_ON_ERROR);

	memset(&opts, 0, ਆਕਾਰ(opts));

	ਰੁੱਖ = repo_parse_ਰੁੱਖ_ਅਸਿੱਧਾ(the_repository, i_ਰੁੱਖ);
	ਜੇ (repo_parse_ਰੁੱਖ(the_repository, ਰੁੱਖ))
		ਵਾਪਸ -1;

	init_ਰੁੱਖ_desc(t, &ਰੁੱਖ->object.oid, ਰੁੱਖ->ਬਫਰ, ਰੁੱਖ->ਮਾਪ);

	opts.ਸਿਰ_idx = 1;
	opts.src_ਸੂਚਕਾਂਕ = the_repository->ਸੂਚਕਾਂਕ;
	opts.dst_ਸੂਚਕਾਂਕ = the_repository->ਸੂਚਕਾਂਕ;
	opts.ਮਿਲਾਓ = 1;
	opts.reset = reset ? UNPACK_RESET_PROTECT_UNTRACKED : 0;
	opts.update = update;
	ਜੇ (update)
		opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
	opts.fn = oneway_ਮਿਲਾਓ;

	ਜੇ (unpack_trees(nr_trees, t, &opts))
		ਵਾਪਸ -1;

	ਜੇ (write_locked_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ, &ਤਾਲਾ_file, COMMIT_ਤਾਲਾ))
		ਵਾਪਸ error(_("unable to write new index file"));

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ create_ਸੂਚਕਾਂਕ_from_ਰੁੱਖ(ਅਚਲ ਬਣਤਰ object_id *ਰੁੱਖ_id,
				  ਅਚਲ ਅੱਖਰ *ਸੂਚਕਾਂਕ_ਰਸਤਾ)
{
	ਪੂਰਨ nr_trees = 1;
	ਪੂਰਨ ret = 0;
	ਬਣਤਰ unpack_trees_options opts;
	ਬਣਤਰ ਰੁੱਖ_desc t[MAX_UNPACK_TREES];
	ਬਣਤਰ ਰੁੱਖ *ਰੁੱਖ;
	ਬਣਤਰ ਸੂਚਕਾਂਕ_state dst_istate = ਸੂਚਕਾਂਕ_STATE_INIT(the_repository);
	ਬਣਤਰ ਤਾਲਾ_file ਤਾਲਾ_file = ਤਾਲਾ_INIT;

	repo_read_ਸੂਚਕਾਂਕ_preload(the_repository, ਨਿਰਾ, 0);
	refresh_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ, REFRESH_QUIET, ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ);

	hold_ਤਾਲਾ_file_for_update(&ਤਾਲਾ_file, ਸੂਚਕਾਂਕ_ਰਸਤਾ, ਤਾਲਾ_DIE_ON_ERROR);

	memset(&opts, 0, ਆਕਾਰ(opts));

	ਰੁੱਖ = repo_parse_ਰੁੱਖ_ਅਸਿੱਧਾ(the_repository, ਰੁੱਖ_id);
	ਜੇ (!ਰੁੱਖ || repo_parse_ਰੁੱਖ(the_repository, ਰੁੱਖ)) {
		ret = -1;
		ਜਾਓ ਪੂਰਾ;
	}

	init_ਰੁੱਖ_desc(t, &ਰੁੱਖ->object.oid, ਰੁੱਖ->ਬਫਰ, ਰੁੱਖ->ਮਾਪ);

	opts.ਸਿਰ_idx = 1;
	opts.src_ਸੂਚਕਾਂਕ = the_repository->ਸੂਚਕਾਂਕ;
	opts.dst_ਸੂਚਕਾਂਕ = &dst_istate;
	opts.ਮਿਲਾਓ = 1;
	opts.reset = UNPACK_RESET_PROTECT_UNTRACKED;
	opts.fn = oneway_ਮਿਲਾਓ;

	ਜੇ (unpack_trees(nr_trees, t, &opts)) {
		ret = -1;
		ਜਾਓ ਪੂਰਾ;
	}

	ਜੇ (write_locked_ਸੂਚਕਾਂਕ(&dst_istate, &ਤਾਲਾ_file, COMMIT_ਤਾਲਾ)) {
		ret = error(_("unable to write new index file"));
		ਜਾਓ ਪੂਰਾ;
	}

ਪੂਰਾ:
	release_ਸੂਚਕਾਂਕ(&dst_istate);
	ਜੇ (ret)
		rollback_ਤਾਲਾ_file(&ਤਾਲਾ_file);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ is_ਰਸਤਾ_a_directory(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ)
{
	/*
	 * This function differs from abspath.c:is_directory() in that
	 * here we use lstat() instead of stat(); we do not want to
	 * follow symbolic links here.
	 */
	ਬਣਤਰ stat st;
	ਵਾਪਸ (!lstat(ਰਸਤਾ, &st) && S_ISDIR(st.st_ਬਹੁਲਕ));
}

ਸਥਿਰ ਖਾਲੀ ਜੋੜੋ_diff_ਮੰਜ਼ਿਲ_buf(ਬਣਤਰ diff_ਕਤਾਰ_struct *q,
			    ਬਣਤਰ diff_options *options ਅਣਵਰਤਿਆ,
			    ਖਾਲੀ *ਅੰਕੜੇ)
{
	ਪੂਰਨ i;

	ਲਈ (i = 0; i < q->nr; i++) {
		ਜੇ (is_ਰਸਤਾ_a_directory(q->ਕਤਾਰ[i]->one->ਰਸਤਾ))
			ਜਾਰੀ;

		strbuf_addstr(ਅੰਕੜੇ, q->ਕਤਾਰ[i]->one->ਰਸਤਾ);

		/* NUL-terminate: will be fed to update-index -z */
		strbuf_addch(ਅੰਕੜੇ, '\0');
	}
}

ਸਥਿਰ ਪੂਰਨ restore_untracked(ਬਣਤਰ object_id *u_ਰੁੱਖ)
{
	ਪੂਰਨ res;
	ਬਣਤਰ ਬੱਚਾ_process cp = ਬੱਚਾ_PROCESS_INIT;

	/*
	 * We need to run restore files from a given index, but without
	 * affecting the current index, so we use GIT_INDEX_FILE with
	 * run_command to fork processes that will not interfere.
	 */
	cp.git_cmd = 1;
	strvec_ਧੱਕੋ(&cp.ਦਲੀਲਾਂ, "read-tree");
	strvec_ਧੱਕੋ(&cp.ਦਲੀਲਾਂ, oid_ਮੰਜ਼ਿਲ_hex(u_ਰੁੱਖ));
	strvec_pushf(&cp.env, "GIT_INDEX_FILE=%s",
		     stash_ਸੂਚਕਾਂਕ_ਰਸਤਾ.buf);
	ਜੇ (ਲੜੀਵਾਰ_command(&cp)) {
		remove_ਰਸਤਾ(stash_ਸੂਚਕਾਂਕ_ਰਸਤਾ.buf);
		ਵਾਪਸ -1;
	}

	ਬੱਚਾ_process_init(&cp);
	cp.git_cmd = 1;
	strvec_pushl(&cp.ਦਲੀਲਾਂ, "checkout-index", "--all", ਨਿਰਾ);
	strvec_pushf(&cp.env, "GIT_INDEX_FILE=%s",
		     stash_ਸੂਚਕਾਂਕ_ਰਸਤਾ.buf);

	res = ਲੜੀਵਾਰ_command(&cp);
	remove_ਰਸਤਾ(stash_ਸੂਚਕਾਂਕ_ਰਸਤਾ.buf);
	ਵਾਪਸ res;
}

ਸਥਿਰ ਖਾਲੀ unstage_changes_unless_new(ਬਣਤਰ object_id *orig_ਰੁੱਖ)
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

	ਬਣਤਰ checkout state = CHECKOUT_INIT;
	ਬਣਤਰ diff_options diff_opts;
	ਬਣਤਰ ਤਾਲਾ_file ਤਾਲਾ = ਤਾਲਾ_INIT;
	ਪੂਰਨ i;

	/* If any entries have skip_worktree set, we'll have to check 'em out */
	state.force = 1;
	state.quiet = 1;
	state.refresh_cache = 1;
	state.istate = the_repository->ਸੂਚਕਾਂਕ;

	/*
	 * Step 1: get a difference between orig_tree (which corresponding
	 * to the index before a merge was run) and the current index
	 * (reflecting the changes brought in by the merge).
	 */
	repo_diff_ਤਿਆਰੀ(the_repository, &diff_opts);
	diff_opts.flags.recursive = 1;
	diff_opts.detect_rename = 0;
	diff_opts.output_format = DIFF_FORMAT_NO_OUTPUT;
	diff_ਤਿਆਰੀ_ਪੂਰਾ(&diff_opts);

	do_diff_cache(orig_ਰੁੱਖ, &diff_opts);
	diffcore_std(&diff_opts);

	/* Iterate over the paths that changed due to the merge... */
	ਲਈ (i = 0; i < diff_queued_diff.nr; i++) {
		ਬਣਤਰ diff_filepair *p;
		ਬਣਤਰ cache_ਇੰਦਰਾਜ *ce;
		ਪੂਰਨ pos;

		/* Look up the path's position in the current index. */
		p = diff_queued_diff.ਕਤਾਰ[i];
		pos = ਸੂਚਕਾਂਕ_name_pos(the_repository->ਸੂਚਕਾਂਕ, p->two->ਰਸਤਾ,
				     ਲੜੀ_ਲੰਬਾਈ(p->two->ਰਸਤਾ));

		/*
		 * Step 2: Place changes in the working tree
		 *
		 * Stash is about restoring changes *to the working tree*.
		 * So if the merge successfully got a new version of some
		 * path, but left it out of the working tree, then clear the
		 * SKIP_WORKTREE bit and write it to the working tree.
		 */
		ਜੇ (pos >= 0 && ce_skip_worktree(the_repository->ਸੂਚਕਾਂਕ->cache[pos])) {
			ਬਣਤਰ stat st;

			ce = the_repository->ਸੂਚਕਾਂਕ->cache[pos];
			ਜੇ (!lstat(ce->name, &st)) {
				/* Conflicting path present; relocate it */
				ਬਣਤਰ strbuf new_ਰਸਤਾ = STRBUF_INIT;
				ਪੂਰਨ fd;

				strbuf_addf(&new_ਰਸਤਾ,
					    "%s.stash.XXXXXX", ce->name);
				fd = xmkstemp(new_ਰਸਤਾ.buf);
				close(fd);
				ਲਿਖੋ(_("WARNING: Untracked file in way of "
					 "tracked file!  Renaming\n "
					 "           %s -> %s\n"
					 "         to make room.\n"),
				       ce->name, new_ਰਸਤਾ.buf);
				ਜੇ (rename(ce->name, new_ਰਸਤਾ.buf))
					die("Failed to move %s to %s",
					    ce->name, new_ਰਸਤਾ.buf);
				strbuf_release(&new_ਰਸਤਾ);
			}
			checkout_ਇੰਦਰਾਜ(ce, &state, ਨਿਰਾ, ਨਿਰਾ);
			ce->ce_flags &= ~CE_SKIP_WORKTREE;
		}

		/*
		 * Step 3: "unstage" changes, as long as they are still tracked
		 */
		ਜੇ (p->one->oid_valid) {
			/*
			 * Path existed in orig_tree; restore index entry
			 * from that tree in order to "unstage" the changes.
			 */
			ਪੂਰਨ option = ਜੋੜੋ_CACHE_OK_ਮੰਜ਼ਿਲ_REPLACE;
			ਜੇ (pos < 0)
				option = ਜੋੜੋ_CACHE_OK_ਮੰਜ਼ਿਲ_ਜੋੜੋ;

			ce = make_cache_ਇੰਦਰਾਜ(the_repository->ਸੂਚਕਾਂਕ,
					      p->one->ਬਹੁਲਕ,
					      &p->one->oid,
					      p->one->ਰਸਤਾ,
					      0, 0);
			ਜੋੜੋ_ਸੂਚਕਾਂਕ_ਇੰਦਰਾਜ(the_repository->ਸੂਚਕਾਂਕ, ce, option);
		}
	}
	diff_flush(&diff_opts);

	/*
	 * Step 4: write the new index to disk
	 */
	repo_hold_locked_ਸੂਚਕਾਂਕ(the_repository, &ਤਾਲਾ, ਤਾਲਾ_DIE_ON_ERROR);
	ਜੇ (write_locked_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ, &ਤਾਲਾ,
			       COMMIT_ਤਾਲਾ | SKIP_IF_UNCHANGED))
		die(_("could not write index"));
}

ਸਥਿਰ ਗਿਣਤੀ stash_apply_ਨਤੀਜਾ do_apply_stash(ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
					      ਬਣਤਰ stash_info *info,
					      ਪੂਰਨ ਸੂਚਕਾਂਕ, ਪੂਰਨ quiet,
					      ਅਚਲ ਅੱਖਰ *label_ours,
					      ਅਚਲ ਅੱਖਰ *label_theirs,
					      ਅਚਲ ਅੱਖਰ *label_ਅਧਾਰ)
{
	ਪੂਰਨ clean, ret;
	ਪੂਰਨ has_ਸੂਚਕਾਂਕ = ਸੂਚਕਾਂਕ;
	ਬਣਤਰ ਮਿਲਾਓ_options o;
	ਬਣਤਰ object_id c_ਰੁੱਖ;
	ਬਣਤਰ object_id ਸੂਚਕਾਂਕ_ਰੁੱਖ;
	ਬਣਤਰ ਰੁੱਖ *ਸਿਰ, *ਮਿਲਾਓ, *ਮਿਲਾਓ_ਅਧਾਰ;
	ਬਣਤਰ ਤਾਲਾ_file ਤਾਲਾ = ਤਾਲਾ_INIT;

	repo_read_ਸੂਚਕਾਂਕ_preload(the_repository, ਨਿਰਾ, 0);
	ਜੇ (repo_refresh_and_write_ਸੂਚਕਾਂਕ(the_repository, REFRESH_QUIET, 0, 0,
					 ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ))
		ਵਾਪਸ error(_("could not write index"));

	ਜੇ (write_ਸੂਚਕਾਂਕ_as_ਰੁੱਖ(&c_ਰੁੱਖ, the_repository->ਸੂਚਕਾਂਕ,
				repo_get_ਸੂਚਕਾਂਕ_file(the_repository), 0, ਨਿਰਾ))
		ਵਾਪਸ error(_("cannot apply a stash in the middle of a merge"));

	init_ui_ਮਿਲਾਓ_options(&o, the_repository);

	ਜੇ (quiet)
		o.verbosity = 0;

	ਜੇ (ਸੂਚਕਾਂਕ) {
		ਜੇ (oideq(&info->b_ਰੁੱਖ, &info->i_ਰੁੱਖ) ||
		    oideq(&c_ਰੁੱਖ, &info->i_ਰੁੱਖ)) {
			has_ਸੂਚਕਾਂਕ = 0;
		} ਨਹੀਂਤਾਂ {
			ਬਣਤਰ ਮਿਲਾਓ_ਨਤੀਜਾ ਨਤੀਜਾ = { 0 };

			o.branch1 = "Current index";
			o.branch2 = "Stashed index changes";
			o.ancestor = "Stash base";

			ਸਿਰ = lookup_ਰੁੱਖ(o.repo, &c_ਰੁੱਖ);
			ਮਿਲਾਓ = lookup_ਰੁੱਖ(o.repo, &info->i_ਰੁੱਖ);
			ਮਿਲਾਓ_ਅਧਾਰ = lookup_ਰੁੱਖ(o.repo, &info->b_ਰੁੱਖ);

			ਮਿਲਾਓ_incore_nonrecursive(&o, ਮਿਲਾਓ_ਅਧਾਰ, ਸਿਰ, ਮਿਲਾਓ,
						  &ਨਤੀਜਾ);

			ਜੇ (ਨਤੀਜਾ.clean < 0) {
				ਮਿਲਾਓ_finalize(&o, &ਨਤੀਜਾ);
				ਵਾਪਸ error(_("index merge failed"));
			} ਨਹੀਂਤਾਂ ਜੇ (!ਨਤੀਜਾ.clean) {
				ਮਿਲਾਓ_finalize(&o, &ਨਤੀਜਾ);
				ਵਾਪਸ error(_("conflicts in index. "
					       "Try without --index."));
			} ਨਹੀਂਤਾਂ {
				oidcpy(&ਸੂਚਕਾਂਕ_ਰੁੱਖ, &ਨਤੀਜਾ.ਰੁੱਖ->object.oid);
				ਮਿਲਾਓ_finalize(&o, &ਨਤੀਜਾ);
			}
		}
	}

	o.branch1 = label_ours ? label_ours : "Updated upstream";
	o.branch2 = label_theirs ? label_theirs : "Stashed changes";
	o.ancestor = label_ਅਧਾਰ ? label_ਅਧਾਰ : "Stash base";

	ਜੇ (oideq(&info->b_ਰੁੱਖ, &c_ਰੁੱਖ))
		o.branch1 = "Version stash was based on";

	ਜੇ (o.verbosity >= 3)
		printf_ln(_("Merging %s with %s"), o.branch1, o.branch2);

	ਸਿਰ = lookup_ਰੁੱਖ(o.repo, &c_ਰੁੱਖ);
	ਮਿਲਾਓ = lookup_ਰੁੱਖ(o.repo, &info->w_ਰੁੱਖ);
	ਮਿਲਾਓ_ਅਧਾਰ = lookup_ਰੁੱਖ(o.repo, &info->b_ਰੁੱਖ);

	repo_hold_locked_ਸੂਚਕਾਂਕ(o.repo, &ਤਾਲਾ, ਤਾਲਾ_DIE_ON_ERROR);
	clean = ਮਿਲਾਓ_ort_nonrecursive(&o, ਸਿਰ, ਮਿਲਾਓ, ਮਿਲਾਓ_ਅਧਾਰ);

	/*
	 * If 'clean' >= 0, reverse the value for 'ret' so 'ret' is 0 when the
	 * merge was clean, and 1 if the merge was unclean or a negative value
	 * if it encountered an error.
	 */
	ret = clean >= 0 ? !clean : clean;

	ਜੇ (ret < 0)
		rollback_ਤਾਲਾ_file(&ਤਾਲਾ);
	ਨਹੀਂਤਾਂ ਜੇ (write_locked_ਸੂਚਕਾਂਕ(o.repo->ਸੂਚਕਾਂਕ, &ਤਾਲਾ,
				      COMMIT_ਤਾਲਾ | SKIP_IF_UNCHANGED))
		ret = error(_("could not write index"));

	ਜੇ (ret) {
		repo_rerere(the_repository, 0);

		ਜੇ (ਸੂਚਕਾਂਕ)
			fprintf_ln(stderr, _("Index was not unstashed."));

		ਜਾਓ restore_untracked;
	}

	ਜੇ (has_ਸੂਚਕਾਂਕ) {
		ਜੇ (reset_ਰੁੱਖ(&ਸੂਚਕਾਂਕ_ਰੁੱਖ, 0, 0))
			ret = -1;
	} ਨਹੀਂਤਾਂ {
		unstage_changes_unless_new(&c_ਰੁੱਖ);
	}

restore_untracked:
	ਜੇ (info->has_u && restore_untracked(&info->u_ਰੁੱਖ))
		ret = error(_("could not restore untracked files from stash"));

	ਜੇ (!quiet) {
		ਬਣਤਰ ਬੱਚਾ_process cp = ਬੱਚਾ_PROCESS_INIT;

		/*
		 * Status is quite simple and could be replaced with calls to
		 * wt_status in the future, but it adds complexities which may
		 * require more tests.
		 */
		cp.git_cmd = 1;
		cp.dir = ਅਗੇਤਰ;
		strvec_pushf(&cp.env, GIT_WORK_ਰੁੱਖ_ENVIRONMENT"=%s",
			     absolute_ਰਸਤਾ(repo_get_work_ਰੁੱਖ(the_repository)));
		strvec_pushf(&cp.env, GIT_DIR_ENVIRONMENT"=%s",
			     absolute_ਰਸਤਾ(repo_get_git_dir(the_repository)));
		strvec_ਧੱਕੋ(&cp.ਦਲੀਲਾਂ, "status");
		ਲੜੀਵਾਰ_command(&cp);
	}

	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ apply_stash(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		       ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ ret = -1;
	ਪੂਰਨ quiet = 0;
	ਪੂਰਨ ਸੂਚਕਾਂਕ = use_ਸੂਚਕਾਂਕ;
	ਅਚਲ ਅੱਖਰ *label_ours = ਨਿਰਾ, *label_theirs = ਨਿਰਾ, *label_ਅਧਾਰ = ਨਿਰਾ;
	ਬਣਤਰ stash_info info = STASH_INFO_INIT;
	ਬਣਤਰ option options[] = {
		OPT__QUIET(&quiet, N_("be quiet, only report errors")),
		OPT_BOOL(0, "index", &ਸੂਚਕਾਂਕ,
			 N_("attempt to recreate the index")),
		OPT_STRING(0, "label-ours", &label_ours, N_("label"),
			   N_("label for the upstream side in conflict markers")),
		OPT_STRING(0, "label-theirs", &label_theirs, N_("label"),
			   N_("label for the stashed side in conflict markers")),
		OPT_STRING(0, "label-base", &label_ਅਧਾਰ, N_("label"),
			   N_("label for the base in diff3 conflict markers")),
		OPT_ਸਿਰਾ()
	};

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options,
			     git_stash_apply_usage, 0);

	ਜੇ (get_stash_info(&info, argc, argv))
		ਜਾਓ cleanup;

	ret = do_apply_stash(ਅਗੇਤਰ, &info, ਸੂਚਕਾਂਕ, quiet,
			     label_ours, label_theirs, label_ਅਧਾਰ);
cleanup:
	free_stash_info(&info);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ reject_reflog_ent(ਅਚਲ ਅੱਖਰ *refname ਅਣਵਰਤਿਆ,
			     ਬਣਤਰ object_id *ooid ਅਣਵਰਤਿਆ,
			     ਬਣਤਰ object_id *noid ਅਣਵਰਤਿਆ,
			     ਅਚਲ ਅੱਖਰ *email ਅਣਵਰਤਿਆ,
			     timestamp_t timestamp ਅਣਵਰਤਿਆ,
			     ਪੂਰਨ tz ਅਣਵਰਤਿਆ, ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ_2 ਅਣਵਰਤਿਆ,
			     ਖਾਲੀ *cb_ਅੰਕੜੇ ਅਣਵਰਤਿਆ)
{
	ਵਾਪਸ 1;
}

ਸਥਿਰ ਪੂਰਨ reflog_is_ਖਾਲੀ_ਥਾਵਾਂ(ਅਚਲ ਅੱਖਰ *refname)
{
	ਵਾਪਸ !refs_for_each_reflog_ent(get_main_ref_store(the_repository),
					 refname, reject_reflog_ent, ਨਿਰਾ);
}

ਸਥਿਰ ਪੂਰਨ do_drop_stash(ਬਣਤਰ stash_info *info, ਪੂਰਨ quiet)
{
	ਜੇ (!reflog_delete(info->revision.buf,
			   EXPIRE_REFLOGS_REWRITE | EXPIRE_REFLOGS_UPDATE_REF,
			   0)) {
		ਜੇ (!quiet)
			printf_ln(_("Dropped %s (%s)"), info->revision.buf,
				  oid_ਮੰਜ਼ਿਲ_hex(&info->w_commit));
	} ਨਹੀਂਤਾਂ {
		ਵਾਪਸ error(_("%s: Could not drop stash entry"),
			     info->revision.buf);
	}

	ਜੇ (reflog_is_ਖਾਲੀ_ਥਾਵਾਂ(ref_stash))
		do_clear_stash();

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ get_stash_info_assert(ਬਣਤਰ stash_info *info, ਪੂਰਨ argc,
				 ਅਚਲ ਅੱਖਰ **argv)
{
	ਪੂਰਨ ret = get_stash_info(info, argc, argv);

	ਜੇ (ret < 0)
		ਵਾਪਸ ret;

	ਜੇ (!info->is_stash_ref)
		ਵਾਪਸ error(_("'%s' is not a stash reference"), info->revision.buf);

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ drop_stash(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		      ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ ret = -1;
	ਪੂਰਨ quiet = 0;
	ਬਣਤਰ stash_info info = STASH_INFO_INIT;
	ਬਣਤਰ option options[] = {
		OPT__QUIET(&quiet, N_("be quiet, only report errors")),
		OPT_ਸਿਰਾ()
	};

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options,
			     git_stash_drop_usage, 0);

	ਜੇ (get_stash_info_assert(&info, argc, argv))
		ਜਾਓ cleanup;

	ret = do_drop_stash(&info, quiet);
cleanup:
	free_stash_info(&info);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ ਕੱਢੋ_stash(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		     ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ ret = -1;
	ਪੂਰਨ ਸੂਚਕਾਂਕ = use_ਸੂਚਕਾਂਕ;
	ਪੂਰਨ quiet = 0;
	ਬਣਤਰ stash_info info = STASH_INFO_INIT;
	ਬਣਤਰ option options[] = {
		OPT__QUIET(&quiet, N_("be quiet, only report errors")),
		OPT_BOOL(0, "index", &ਸੂਚਕਾਂਕ,
			 N_("attempt to recreate the index")),
		OPT_ਸਿਰਾ()
	};

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options,
			     git_stash_ਕੱਢੋ_usage, 0);

	ਜੇ (get_stash_info_assert(&info, argc, argv))
		ਜਾਓ cleanup;

	ਜੇ ((ret = do_apply_stash(ਅਗੇਤਰ, &info, ਸੂਚਕਾਂਕ, quiet,
				  ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ)))
		printf_ln(_("The stash entry is kept in case "
			    "you need it again."));
	ਨਹੀਂਤਾਂ
		ret = do_drop_stash(&info, quiet);

cleanup:
	free_stash_info(&info);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ branch_stash(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ ret = -1;
	ਅਚਲ ਅੱਖਰ *branch = ਨਿਰਾ;
	ਬਣਤਰ stash_info info = STASH_INFO_INIT;
	ਬਣਤਰ ਬੱਚਾ_process cp = ਬੱਚਾ_PROCESS_INIT;
	ਬਣਤਰ option options[] = {
		OPT_ਸਿਰਾ()
	};

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options,
			     git_stash_branch_usage, 0);

	ਜੇ (!argc) {
		fprintf_ln(stderr, _("No branch name specified"));
		ਵਾਪਸ -1;
	}

	branch = argv[0];

	ਜੇ (get_stash_info(&info, argc - 1, argv + 1))
		ਜਾਓ cleanup;

	cp.git_cmd = 1;
	strvec_pushl(&cp.ਦਲੀਲਾਂ, "checkout", "-b", ਨਿਰਾ);
	strvec_ਧੱਕੋ(&cp.ਦਲੀਲਾਂ, branch);
	strvec_ਧੱਕੋ(&cp.ਦਲੀਲਾਂ, oid_ਮੰਜ਼ਿਲ_hex(&info.b_commit));
	ret = ਲੜੀਵਾਰ_command(&cp);
	ਜੇ (!ret)
		ret = do_apply_stash(ਅਗੇਤਰ, &info, 1, 0,
				     ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ);
	ਜੇ (!ret && info.is_stash_ref)
		ret = do_drop_stash(&info, 0);

cleanup:
	free_stash_info(&info);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ list_stash(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		      ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ ਬੱਚਾ_process cp = ਬੱਚਾ_PROCESS_INIT;
	ਬਣਤਰ option options[] = {
		OPT_ਸਿਰਾ()
	};

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options,
			     git_stash_list_usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT);

	ਜੇ (!refs_ref_exists(get_main_ref_store(the_repository), ref_stash))
		ਵਾਪਸ 0;

	cp.git_cmd = 1;
	strvec_pushl(&cp.ਦਲੀਲਾਂ, "log", "--format=%gd: %gs", "-g",
		     "--first-parent", ਨਿਰਾ);
	strvec_pushv(&cp.ਦਲੀਲਾਂ, argv);
	strvec_ਧੱਕੋ(&cp.ਦਲੀਲਾਂ, ref_stash);
	strvec_ਧੱਕੋ(&cp.ਦਲੀਲਾਂ, "--");
	ਵਾਪਸ ਲੜੀਵਾਰ_command(&cp);
}

ਸਥਿਰ ਪੂਰਨ git_stash_config(ਅਚਲ ਅੱਖਰ *var, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ,
			    ਅਚਲ ਬਣਤਰ config_context *ctx, ਖਾਲੀ *cb)
{
	ਜੇ (!strcmp(var, "stash.showstat")) {
		show_stat = git_config_bool(var, ਮੁੱਲ);
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(var, "stash.showpatch")) {
		show_patch = git_config_bool(var, ਮੁੱਲ);
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(var, "stash.showincludeuntracked")) {
		show_include_untracked = git_config_bool(var, ਮੁੱਲ);
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(var, "stash.index")) {
		use_ਸੂਚਕਾਂਕ = git_config_bool(var, ਮੁੱਲ);
		ਵਾਪਸ 0;
	}
	ਵਾਪਸ git_diff_basic_config(var, ਮੁੱਲ, ctx, cb);
}

ਸਥਿਰ ਖਾਲੀ diff_include_untracked(ਅਚਲ ਬਣਤਰ stash_info *info, ਬਣਤਰ diff_options *diff_opt)
{
	ਅਚਲ ਬਣਤਰ object_id *oid[] = { &info->w_commit, &info->u_ਰੁੱਖ };
	ਬਣਤਰ ਰੁੱਖ *ਰੁੱਖ[ARRAY_ਮਾਪ(oid)];
	ਬਣਤਰ ਰੁੱਖ_desc ਰੁੱਖ_desc[ARRAY_ਮਾਪ(oid)];
	ਬਣਤਰ unpack_trees_options unpack_ਰੁੱਖ_opt = { 0 };

	ਲਈ (ਮਾਪ_t i = 0; i < ARRAY_ਮਾਪ(oid); i++) {
		ਰੁੱਖ[i] = repo_parse_ਰੁੱਖ_ਅਸਿੱਧਾ(the_repository, oid[i]);
		ਜੇ (repo_parse_ਰੁੱਖ(the_repository, ਰੁੱਖ[i]) < 0)
			die(_("failed to parse tree"));
		init_ਰੁੱਖ_desc(&ਰੁੱਖ_desc[i], &ਰੁੱਖ[i]->object.oid,
			       ਰੁੱਖ[i]->ਬਫਰ, ਰੁੱਖ[i]->ਮਾਪ);
	}

	unpack_ਰੁੱਖ_opt.ਸਿਰ_idx = -1;
	unpack_ਰੁੱਖ_opt.src_ਸੂਚਕਾਂਕ = the_repository->ਸੂਚਕਾਂਕ;
	unpack_ਰੁੱਖ_opt.dst_ਸੂਚਕਾਂਕ = the_repository->ਸੂਚਕਾਂਕ;
	unpack_ਰੁੱਖ_opt.ਮਿਲਾਓ = 1;
	unpack_ਰੁੱਖ_opt.fn = stash_worktree_untracked_ਮਿਲਾਓ;

	ਜੇ (unpack_trees(ARRAY_ਮਾਪ(ਰੁੱਖ_desc), ਰੁੱਖ_desc, &unpack_ਰੁੱਖ_opt))
		die(_("failed to unpack trees"));

	do_diff_cache(&info->b_commit, diff_opt);
}

ਸਥਿਰ ਪੂਰਨ show_stash(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		      ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ i;
	ਪੂਰਨ ret = -1;
	ਬਣਤਰ stash_info info = STASH_INFO_INIT;
	ਬਣਤਰ rev_info rev;
	ਬਣਤਰ strvec stash_ਦਲੀਲਾਂ = STRVEC_INIT;
	ਬਣਤਰ strvec revision_ਦਲੀਲਾਂ = STRVEC_INIT;
	ਗਿਣਤੀ {
		UNTRACKED_NONE,
		UNTRACKED_INCLUDE,
		UNTRACKED_ONLY
	} show_untracked = show_include_untracked ? UNTRACKED_INCLUDE : UNTRACKED_NONE;
	ਬਣਤਰ option options[] = {
		OPT_ਸੈੱਟ_INT('u', "include-untracked", &show_untracked,
			    N_("include untracked files in the stash"),
			    UNTRACKED_INCLUDE),
		OPT_ਸੈੱਟ_INT_F(0, "only-untracked", &show_untracked,
			      N_("only show untracked files in the stash"),
			      UNTRACKED_ONLY, PARSE_OPT_NONEG),
		OPT_ਸਿਰਾ()
	};
	ਪੂਰਨ do_usage = 0;

	init_diff_ui_defaults();
	repo_config(the_repository, git_diff_ui_config, ਨਿਰਾ);
	repo_init_revisions(the_repository, &rev, ਅਗੇਤਰ);

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, git_stash_show_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	strvec_ਧੱਕੋ(&revision_ਦਲੀਲਾਂ, argv[0]);
	ਲਈ (i = 1; i < argc; i++) {
		ਜੇ (argv[i][0] != '-')
			strvec_ਧੱਕੋ(&stash_ਦਲੀਲਾਂ, argv[i]);
		ਨਹੀਂਤਾਂ
			strvec_ਧੱਕੋ(&revision_ਦਲੀਲਾਂ, argv[i]);
	}

	ਜੇ (get_stash_info(&info, stash_ਦਲੀਲਾਂ.nr, stash_ਦਲੀਲਾਂ.v))
		ਜਾਓ cleanup;

	/*
	 * The config settings are applied only if there are not passed
	 * any options.
	 */
	ਜੇ (revision_ਦਲੀਲਾਂ.nr == 1) {
		ਜੇ (show_stat)
			rev.diffopt.output_format = DIFF_FORMAT_DIFFSTAT;

		ਜੇ (show_patch)
			rev.diffopt.output_format |= DIFF_FORMAT_PATCH;

		ਜੇ (!show_stat && !show_patch) {
			ret = 0;
			ਜਾਓ cleanup;
		}
	}

	ਤਿਆਰੀ_revisions_from_strvec(&revision_ਦਲੀਲਾਂ, &rev, ਨਿਰਾ);
	ਜੇ (revision_ਦਲੀਲਾਂ.nr > 1)
		ਜਾਓ usage;
	ਜੇ (!rev.diffopt.output_format) {
		rev.diffopt.output_format = DIFF_FORMAT_PATCH;
		diff_ਤਿਆਰੀ_ਪੂਰਾ(&rev.diffopt);
	}

	rev.diffopt.flags.recursive = 1;
	ਤਿਆਰੀ_diff_pager(&rev.diffopt);
	ਚੋਣ (show_untracked) {
	ਹਾਲਤ UNTRACKED_NONE:
		diff_ਰੁੱਖ_oid(&info.b_commit, &info.w_commit, "", &rev.diffopt);
		ਰੋਕੋ;
	ਹਾਲਤ UNTRACKED_ONLY:
		ਜੇ (info.has_u)
			diff_ਜੜ੍ਹ_ਰੁੱਖ_oid(&info.u_ਰੁੱਖ, "", &rev.diffopt);
		ਰੋਕੋ;
	ਹਾਲਤ UNTRACKED_INCLUDE:
		ਜੇ (info.has_u)
			diff_include_untracked(&info, &rev.diffopt);
		ਨਹੀਂਤਾਂ
			diff_ਰੁੱਖ_oid(&info.b_commit, &info.w_commit, "", &rev.diffopt);
		ਰੋਕੋ;
	}
	log_ਰੁੱਖ_diff_flush(&rev);

	ret = diff_ਨਤੀਜਾ_ਕੋਡ(&rev);

cleanup:
	strvec_clear(&revision_ਦਲੀਲਾਂ);
	strvec_clear(&stash_ਦਲੀਲਾਂ);
	free_stash_info(&info);
	release_revisions(&rev);
	ਜੇ (do_usage)
		usage_with_options(git_stash_show_usage, options);
	ਵਾਪਸ ret;
usage:
	do_usage = 1;
	ਜਾਓ cleanup;
}

ਸਥਿਰ ਪੂਰਨ do_store_stash(ਅਚਲ ਬਣਤਰ object_id *w_commit, ਅਚਲ ਅੱਖਰ *stash_ਸੁਨੇਹਾ,
			  ਪੂਰਨ quiet)
{
	ਬਣਤਰ stash_info info;
	ਅੱਖਰ revision[GIT_MAX_HEXSZ];

	oid_ਮੰਜ਼ਿਲ_hex_r(revision, w_commit);
	assert_stash_like(&info, revision);

	ਜੇ (!stash_ਸੁਨੇਹਾ)
		stash_ਸੁਨੇਹਾ = "Created via \"git stash store\".";

	ਜੇ (refs_update_ref(get_main_ref_store(the_repository), stash_ਸੁਨੇਹਾ, ref_stash, w_commit, ਨਿਰਾ,
			    REF_FORCE_CREATE_REFLOG,
			    quiet ? UPDATE_REFS_QUIET_ON_ERR :
			    UPDATE_REFS_ਸੁਨੇਹਾ_ON_ERR)) {
		ਜੇ (!quiet) {
			fprintf_ln(stderr, _("Cannot update %s with %s"),
				   ref_stash, oid_ਮੰਜ਼ਿਲ_hex(w_commit));
		}
		ਵਾਪਸ -1;
	}

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ store_stash(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		       ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ quiet = 0;
	ਅਚਲ ਅੱਖਰ *stash_ਸੁਨੇਹਾ = ਨਿਰਾ;
	ਬਣਤਰ object_id obj;
	ਬਣਤਰ option options[] = {
		OPT__QUIET(&quiet, N_("be quiet")),
		OPT_STRING('m', "message", &stash_ਸੁਨੇਹਾ, "message",
			   N_("stash message")),
		OPT_ਸਿਰਾ()
	};
	ਪੂਰਨ ret;

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options,
			     git_stash_store_usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT);

	ਜੇ (argc != 1) {
		ਜੇ (!quiet)
			fprintf_ln(stderr, _("\"git stash store\" requires one "
					     "<commit> argument"));
		ਵਾਪਸ -1;
	}

	ਜੇ (repo_get_oid_with_flags(the_repository, argv[0], &obj,
				    quiet ? GET_OID_QUIETLY : 0)) {
		ਜੇ (!quiet)
			fprintf_ln(stderr, _("Cannot update %s with %s"),
					     ref_stash, argv[0]);
		ret = -1;
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	ret = do_store_stash(&obj, stash_ਸੁਨੇਹਾ, quiet);

ਬਾਹਰਲਾ:
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਖਾਲੀ ਜੋੜੋ_pathspecs(ਬਣਤਰ strvec *ਦਲੀਲਾਂ,
			  ਅਚਲ ਬਣਤਰ pathspec *ps) {
	ਪੂਰਨ i;

	ਲਈ (i = 0; i < ps->nr; i++)
		strvec_ਧੱਕੋ(ਦਲੀਲਾਂ, ps->ਚੀਜ਼ਾਂ[i].original);
}

/*
 * `untracked_files` will be filled with the names of untracked files.
 * The return value is:
 *
 * = 0 if there are not any untracked files
 * > 0 if there are untracked files
 */
ਸਥਿਰ ਪੂਰਨ get_untracked_files(ਅਚਲ ਬਣਤਰ pathspec *ps, ਪੂਰਨ include_untracked,
			       ਬਣਤਰ strbuf *untracked_files)
{
	ਪੂਰਨ i;
	ਪੂਰਨ ਮਿਲਿਆ_2 = 0;
	ਬਣਤਰ dir_struct dir = DIR_INIT;

	ਜੇ (include_untracked != INCLUDE_ALL_FILES)
		ਤਿਆਰੀ_standard_excludes(&dir);

	fill_directory(&dir, the_repository->ਸੂਚਕਾਂਕ, ps);
	ਲਈ (i = 0; i < dir.nr; i++) {
		ਬਣਤਰ dir_ਇੰਦਰਾਜ *ent = dir.entries[i];
		ਮਿਲਿਆ_2++;
		strbuf_addstr(untracked_files, ent->name);
		/* NUL-terminate: will be fed to update-index -z */
		strbuf_addch(untracked_files, '\0');
	}

	dir_clear(&dir);
	ਵਾਪਸ ਮਿਲਿਆ_2;
}

/*
 * The return value of `check_changes_tracked_files()` can be:
 *
 * < 0 if there was an error
 * = 0 if there are no changes.
 * > 0 if there are changes.
 */
ਸਥਿਰ ਪੂਰਨ check_changes_tracked_files(ਅਚਲ ਬਣਤਰ pathspec *ps)
{
	ਬਣਤਰ rev_info rev;
	ਬਣਤਰ object_id dummy;
	ਪੂਰਨ ret = 0;

	/* No initial commit. */
	ਜੇ (repo_get_oid(the_repository, "HEAD", &dummy))
		ਵਾਪਸ -1;

	ਜੇ (repo_read_ਸੂਚਕਾਂਕ(the_repository) < 0)
		ਵਾਪਸ -1;

	repo_init_revisions(the_repository, &rev, ਨਿਰਾ);
	copy_pathspec(&rev.prune_ਅੰਕੜੇ, ps);

	rev.diffopt.flags.ਤੇਜ਼ = 1;
	rev.diffopt.flags.ignore_submodules = 1;
	rev.abbrev = 0;

	ਜੋੜੋ_ਸਿਰ_ਮੰਜ਼ਿਲ_pending(&rev);
	diff_ਤਿਆਰੀ_ਪੂਰਾ(&rev.diffopt);

	ਲੜੀਵਾਰ_diff_ਸੂਚਕਾਂਕ(&rev, DIFF_ਸੂਚਕਾਂਕ_CACHED);
	ਜੇ (diff_ਨਤੀਜਾ_ਕੋਡ(&rev)) {
		ret = 1;
		ਜਾਓ ਪੂਰਾ;
	}

	ਲੜੀਵਾਰ_diff_files(&rev, 0);
	ਜੇ (diff_ਨਤੀਜਾ_ਕੋਡ(&rev)) {
		ret = 1;
		ਜਾਓ ਪੂਰਾ;
	}

ਪੂਰਾ:
	release_revisions(&rev);
	ਵਾਪਸ ret;
}

/*
 * The function will fill `untracked_files` with the names of untracked files
 * It will return 1 if there were any changes and 0 if there were not.
 */
ਸਥਿਰ ਪੂਰਨ check_changes(ਅਚਲ ਬਣਤਰ pathspec *ps, ਪੂਰਨ include_untracked,
			 ਬਣਤਰ strbuf *untracked_files)
{
	ਪੂਰਨ ret = 0;
	ਜੇ (check_changes_tracked_files(ps))
		ret = 1;

	ਜੇ (include_untracked && get_untracked_files(ps, include_untracked,
						     untracked_files))
		ret = 1;

	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ save_untracked_files(ਬਣਤਰ stash_info *info, ਬਣਤਰ strbuf *ਸੁਨੇਹਾ,
				ਬਣਤਰ strbuf *files)
{
	ਪੂਰਨ ret = 0;
	ਬਣਤਰ strbuf untracked_ਸੁਨੇਹਾ = STRBUF_INIT;
	ਬਣਤਰ ਬੱਚਾ_process cp_upd_ਸੂਚਕਾਂਕ = ਬੱਚਾ_PROCESS_INIT;
	ਬਣਤਰ ਸੂਚਕਾਂਕ_state istate = ਸੂਚਕਾਂਕ_STATE_INIT(the_repository);

	cp_upd_ਸੂਚਕਾਂਕ.git_cmd = 1;
	strvec_pushl(&cp_upd_ਸੂਚਕਾਂਕ.ਦਲੀਲਾਂ, "update-index", "-z", "--add",
		     "--remove", "--stdin", ਨਿਰਾ);
	strvec_pushf(&cp_upd_ਸੂਚਕਾਂਕ.env, "GIT_INDEX_FILE=%s",
			 stash_ਸੂਚਕਾਂਕ_ਰਸਤਾ.buf);

	strbuf_addf(&untracked_ਸੁਨੇਹਾ, "untracked files on %s\n", ਸੁਨੇਹਾ->buf);
	ਜੇ (pipe_command(&cp_upd_ਸੂਚਕਾਂਕ, files->buf, files->len, ਨਿਰਾ, 0,
			 ਨਿਰਾ, 0)) {
		ret = -1;
		ਜਾਓ ਪੂਰਾ;
	}

	ਜੇ (write_ਸੂਚਕਾਂਕ_as_ਰੁੱਖ(&info->u_ਰੁੱਖ, &istate, stash_ਸੂਚਕਾਂਕ_ਰਸਤਾ.buf, 0,
				ਨਿਰਾ)) {
		ret = -1;
		ਜਾਓ ਪੂਰਾ;
	}

	ਜੇ (commit_ਰੁੱਖ(untracked_ਸੁਨੇਹਾ.buf, untracked_ਸੁਨੇਹਾ.len,
			&info->u_ਰੁੱਖ, ਨਿਰਾ, &info->u_commit, ਨਿਰਾ, ਨਿਰਾ)) {
		ret = -1;
		ਜਾਓ ਪੂਰਾ;
	}

ਪੂਰਾ:
	release_ਸੂਚਕਾਂਕ(&istate);
	strbuf_release(&untracked_ਸੁਨੇਹਾ);
	remove_ਰਸਤਾ(stash_ਸੂਚਕਾਂਕ_ਰਸਤਾ.buf);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ stash_staged(ਬਣਤਰ stash_info *info, ਬਣਤਰ strbuf *ਬਾਹਰਲਾ_patch,
			ਪੂਰਨ quiet)
{
	ਪੂਰਨ ret = 0;
	ਬਣਤਰ ਬੱਚਾ_process cp_diff_ਰੁੱਖ = ਬੱਚਾ_PROCESS_INIT;
	ਬਣਤਰ ਸੂਚਕਾਂਕ_state istate = ਸੂਚਕਾਂਕ_STATE_INIT(the_repository);

	ਜੇ (write_ਸੂਚਕਾਂਕ_as_ਰੁੱਖ(&info->w_ਰੁੱਖ, &istate, the_repository->ਸੂਚਕਾਂਕ_file,
				0, ਨਿਰਾ)) {
		ret = -1;
		ਜਾਓ ਪੂਰਾ;
	}

	cp_diff_ਰੁੱਖ.git_cmd = 1;
	strvec_pushl(&cp_diff_ਰੁੱਖ.ਦਲੀਲਾਂ, "diff-tree", "-p", "--binary",
		     "--no-color",
		     "-U1", "HEAD", oid_ਮੰਜ਼ਿਲ_hex(&info->w_ਰੁੱਖ), "--", ਨਿਰਾ);
	ਜੇ (pipe_command(&cp_diff_ਰੁੱਖ, ਨਿਰਾ, 0, ਬਾਹਰਲਾ_patch, 0, ਨਿਰਾ, 0)) {
		ret = -1;
		ਜਾਓ ਪੂਰਾ;
	}

	ਜੇ (!ਬਾਹਰਲਾ_patch->len) {
		ਜੇ (!quiet)
			fprintf_ln(stderr, _("No staged changes"));
		ret = 1;
	}

ਪੂਰਾ:
	release_ਸੂਚਕਾਂਕ(&istate);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ stash_patch(ਬਣਤਰ stash_info *info, ਅਚਲ ਬਣਤਰ pathspec *ps,
		       ਬਣਤਰ strbuf *ਬਾਹਰਲਾ_patch, ਪੂਰਨ quiet,
		       ਬਣਤਰ interactive_options *interactive_opts)
{
	ਪੂਰਨ ret = 0;
	ਬਣਤਰ ਬੱਚਾ_process cp_diff_ਰੁੱਖ = ਬੱਚਾ_PROCESS_INIT;
	ਬਣਤਰ commit *ਸਿਰ_commit;
	ਅਚਲ ਬਣਤਰ object_id *ਸਿਰ_ਰੁੱਖ;
	ਬਣਤਰ ਸੂਚਕਾਂਕ_state istate = ਸੂਚਕਾਂਕ_STATE_INIT(the_repository);
	ਅੱਖਰ *old_ਸੂਚਕਾਂਕ_env = ਨਿਰਾ, *old_repo_ਸੂਚਕਾਂਕ_file;

	remove_ਰਸਤਾ(stash_ਸੂਚਕਾਂਕ_ਰਸਤਾ.buf);

	ਸਿਰ_commit = lookup_commit(the_repository, &info->b_commit);
	ਜੇ (!ਸਿਰ_commit || repo_parse_commit(the_repository, ਸਿਰ_commit)) {
		ret = -1;
		ਜਾਓ ਪੂਰਾ;
	}
	ਸਿਰ_ਰੁੱਖ = get_commit_ਰੁੱਖ_oid(ਸਿਰ_commit);
	ਜੇ (!ਸਿਰ_ਰੁੱਖ) {
		ret = -1;
		ਜਾਓ ਪੂਰਾ;
	}

	ਜੇ (create_ਸੂਚਕਾਂਕ_from_ਰੁੱਖ(ਸਿਰ_ਰੁੱਖ, stash_ਸੂਚਕਾਂਕ_ਰਸਤਾ.buf)) {
		ret = -1;
		ਜਾਓ ਪੂਰਾ;
	}

	/* Find out what the user wants. */
	old_repo_ਸੂਚਕਾਂਕ_file = the_repository->ਸੂਚਕਾਂਕ_file;
	the_repository->ਸੂਚਕਾਂਕ_file = stash_ਸੂਚਕਾਂਕ_ਰਸਤਾ.buf;
	old_ਸੂਚਕਾਂਕ_env = xstrdup_or_null(getenv(ਸੂਚਕਾਂਕ_ENVIRONMENT));
	setenv(ਸੂਚਕਾਂਕ_ENVIRONMENT, the_repository->ਸੂਚਕਾਂਕ_file, 1);

	ret = !!ਲੜੀਵਾਰ_ਜੋੜੋ_p(the_repository, ਜੋੜੋ_P_STASH, interactive_opts, ਨਿਰਾ, ps, 0);

	the_repository->ਸੂਚਕਾਂਕ_file = old_repo_ਸੂਚਕਾਂਕ_file;
	ਜੇ (old_ਸੂਚਕਾਂਕ_env && *old_ਸੂਚਕਾਂਕ_env)
		setenv(ਸੂਚਕਾਂਕ_ENVIRONMENT, old_ਸੂਚਕਾਂਕ_env, 1);
	ਨਹੀਂਤਾਂ
		unsetenv(ਸੂਚਕਾਂਕ_ENVIRONMENT);
	FREE_AND_NULL(old_ਸੂਚਕਾਂਕ_env);

	/* State of the working tree. */
	ਜੇ (write_ਸੂਚਕਾਂਕ_as_ਰੁੱਖ(&info->w_ਰੁੱਖ, &istate, stash_ਸੂਚਕਾਂਕ_ਰਸਤਾ.buf, 0,
				ਨਿਰਾ)) {
		ret = -1;
		ਜਾਓ ਪੂਰਾ;
	}

	cp_diff_ਰੁੱਖ.git_cmd = 1;
	strvec_pushl(&cp_diff_ਰੁੱਖ.ਦਲੀਲਾਂ, "diff-tree", "-p", "-U1", "HEAD",
		     "--no-color",
		     oid_ਮੰਜ਼ਿਲ_hex(&info->w_ਰੁੱਖ), "--", ਨਿਰਾ);
	ਜੇ (pipe_command(&cp_diff_ਰੁੱਖ, ਨਿਰਾ, 0, ਬਾਹਰਲਾ_patch, 0, ਨਿਰਾ, 0)) {
		ret = -1;
		ਜਾਓ ਪੂਰਾ;
	}

	ਜੇ (!ਬਾਹਰਲਾ_patch->len) {
		ਜੇ (!quiet)
			fprintf_ln(stderr, _("No changes selected"));
		ret = 1;
	}

ਪੂਰਾ:
	release_ਸੂਚਕਾਂਕ(&istate);
	remove_ਰਸਤਾ(stash_ਸੂਚਕਾਂਕ_ਰਸਤਾ.buf);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ stash_working_ਰੁੱਖ(ਬਣਤਰ stash_info *info, ਅਚਲ ਬਣਤਰ pathspec *ps)
{
	ਪੂਰਨ ret = 0;
	ਬਣਤਰ rev_info rev;
	ਬਣਤਰ ਬੱਚਾ_process cp_upd_ਸੂਚਕਾਂਕ = ਬੱਚਾ_PROCESS_INIT;
	ਬਣਤਰ strbuf diff_output = STRBUF_INIT;
	ਬਣਤਰ ਸੂਚਕਾਂਕ_state istate = ਸੂਚਕਾਂਕ_STATE_INIT(the_repository);

	repo_init_revisions(the_repository, &rev, ਨਿਰਾ);
	copy_pathspec(&rev.prune_ਅੰਕੜੇ, ps);

	ਸੈੱਟ_alternate_ਸੂਚਕਾਂਕ_output(stash_ਸੂਚਕਾਂਕ_ਰਸਤਾ.buf);
	ਜੇ (reset_ਰੁੱਖ(&info->i_ਰੁੱਖ, 0, 0)) {
		ret = -1;
		ਜਾਓ ਪੂਰਾ;
	}
	ਸੈੱਟ_alternate_ਸੂਚਕਾਂਕ_output(ਨਿਰਾ);

	rev.diffopt.output_format = DIFF_FORMAT_CALLBACK;
	rev.diffopt.format_callback = ਜੋੜੋ_diff_ਮੰਜ਼ਿਲ_buf;
	rev.diffopt.format_callback_ਅੰਕੜੇ = &diff_output;

	ਜੇ (repo_read_ਸੂਚਕਾਂਕ_preload(the_repository, &rev.diffopt.pathspec, 0) < 0) {
		ret = -1;
		ਜਾਓ ਪੂਰਾ;
	}

	ਜੋੜੋ_pending_object(&rev, parse_object(the_repository, &info->b_commit),
			   "");
	ਲੜੀਵਾਰ_diff_ਸੂਚਕਾਂਕ(&rev, 0);

	cp_upd_ਸੂਚਕਾਂਕ.git_cmd = 1;
	strvec_pushl(&cp_upd_ਸੂਚਕਾਂਕ.ਦਲੀਲਾਂ, "update-index",
		     "--ignore-skip-worktree-entries",
		     "-z", "--add", "--remove", "--stdin", ਨਿਰਾ);
	strvec_pushf(&cp_upd_ਸੂਚਕਾਂਕ.env, "GIT_INDEX_FILE=%s",
		     stash_ਸੂਚਕਾਂਕ_ਰਸਤਾ.buf);

	ਜੇ (pipe_command(&cp_upd_ਸੂਚਕਾਂਕ, diff_output.buf, diff_output.len,
			 ਨਿਰਾ, 0, ਨਿਰਾ, 0)) {
		ret = -1;
		ਜਾਓ ਪੂਰਾ;
	}

	ਜੇ (write_ਸੂਚਕਾਂਕ_as_ਰੁੱਖ(&info->w_ਰੁੱਖ, &istate, stash_ਸੂਚਕਾਂਕ_ਰਸਤਾ.buf, 0,
				ਨਿਰਾ)) {
		ret = -1;
		ਜਾਓ ਪੂਰਾ;
	}

ਪੂਰਾ:
	release_ਸੂਚਕਾਂਕ(&istate);
	release_revisions(&rev);
	strbuf_release(&diff_output);
	remove_ਰਸਤਾ(stash_ਸੂਚਕਾਂਕ_ਰਸਤਾ.buf);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ do_create_stash(ਅਚਲ ਬਣਤਰ pathspec *ps, ਬਣਤਰ strbuf *stash_ਸੁਨੇਹਾ_buf,
			   ਪੂਰਨ include_untracked, ਪੂਰਨ patch_ਬਹੁਲਕ,
			   ਬਣਤਰ interactive_options *interactive_opts,
			   ਪੂਰਨ only_staged, ਬਣਤਰ stash_info *info, ਬਣਤਰ strbuf *patch,
			   ਪੂਰਨ quiet)
{
	ਪੂਰਨ ret = 0;
	ਪੂਰਨ flags = 0;
	ਪੂਰਨ untracked_commit_option = 0;
	ਅਚਲ ਅੱਖਰ *ਸਿਰ_short_sha1 = ਨਿਰਾ;
	ਅਚਲ ਅੱਖਰ *branch_ref = ਨਿਰਾ;
	ਅਚਲ ਅੱਖਰ *branch_name = "(no branch)";
	ਅੱਖਰ *branch_name_buf = ਨਿਰਾ;
	ਬਣਤਰ commit *ਸਿਰ_commit = ਨਿਰਾ;
	ਬਣਤਰ commit_list *parents = ਨਿਰਾ;
	ਬਣਤਰ strbuf ਸੁਨੇਹਾ = STRBUF_INIT;
	ਬਣਤਰ strbuf commit_ਰੁੱਖ_label = STRBUF_INIT;
	ਬਣਤਰ strbuf untracked_files = STRBUF_INIT;

	prepare_fallback_ident("git stash", "git@stash");

	repo_read_ਸੂਚਕਾਂਕ_preload(the_repository, ਨਿਰਾ, 0);
	ਜੇ (repo_refresh_and_write_ਸੂਚਕਾਂਕ(the_repository, REFRESH_QUIET, 0, 0,
					 ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ) < 0) {
		ret = error(_("could not write index"));
		ਜਾਓ ਪੂਰਾ;
	}

	ਜੇ (repo_get_oid(the_repository, "HEAD", &info->b_commit)) {
		ਜੇ (!quiet)
			fprintf_ln(stderr, _("You do not have "
					     "the initial commit yet"));
		ret = -1;
		ਜਾਓ ਪੂਰਾ;
	} ਨਹੀਂਤਾਂ {
		ਸਿਰ_commit = lookup_commit(the_repository, &info->b_commit);
	}

	ਜੇ (!check_changes(ps, include_untracked, &untracked_files)) {
		ret = 1;
		ਜਾਓ ਪੂਰਾ;
	}

	branch_ref = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
					     "HEAD", 0, ਨਿਰਾ, &flags);

	ਜੇ (flags & REF_ISSYMREF) {
		ਜੇ (skip_ਅਗੇਤਰ(branch_ref, "refs/heads/", &branch_name))
			branch_name = branch_name_buf = xstrdup(branch_name);
	}

	ਸਿਰ_short_sha1 = repo_ਲੱਭੋ_unique_abbrev(the_repository,
						  &ਸਿਰ_commit->object.oid,
						  DEFAULT_ABBREV);
	strbuf_addf(&ਸੁਨੇਹਾ, "%s: %s ", branch_name, ਸਿਰ_short_sha1);
	pp_commit_easy(CMIT_FMT_ONELINE, ਸਿਰ_commit, &ਸੁਨੇਹਾ);

	strbuf_addf(&commit_ਰੁੱਖ_label, "index on %s\n", ਸੁਨੇਹਾ.buf);
	commit_list_ਪਾਓ(ਸਿਰ_commit, &parents);
	ਜੇ (write_ਸੂਚਕਾਂਕ_as_ਰੁੱਖ(&info->i_ਰੁੱਖ, the_repository->ਸੂਚਕਾਂਕ,
				repo_get_ਸੂਚਕਾਂਕ_file(the_repository), 0, ਨਿਰਾ) ||
	    commit_ਰੁੱਖ(commit_ਰੁੱਖ_label.buf, commit_ਰੁੱਖ_label.len,
			&info->i_ਰੁੱਖ, parents, &info->i_commit, ਨਿਰਾ, ਨਿਰਾ)) {
		ਜੇ (!quiet)
			fprintf_ln(stderr, _("Cannot save the current "
					     "index state"));
		ret = -1;
		ਜਾਓ ਪੂਰਾ;
	}

	commit_list_free(parents);
	parents = ਨਿਰਾ;

	ਜੇ (include_untracked) {
		ਜੇ (save_untracked_files(info, &ਸੁਨੇਹਾ, &untracked_files)) {
			ਜੇ (!quiet)
				fprintf_ln(stderr, _("Cannot save "
						     "the untracked files"));
			ret = -1;
			ਜਾਓ ਪੂਰਾ;
		}
		untracked_commit_option = 1;
	}
	ਜੇ (patch_ਬਹੁਲਕ) {
		ret = stash_patch(info, ps, patch, quiet, interactive_opts);
		ਜੇ (ret < 0) {
			ਜੇ (!quiet)
				fprintf_ln(stderr, _("Cannot save the current "
						     "worktree state"));
			ਜਾਓ ਪੂਰਾ;
		} ਨਹੀਂਤਾਂ ਜੇ (ret > 0) {
			ਜਾਓ ਪੂਰਾ;
		}
	} ਨਹੀਂਤਾਂ ਜੇ (only_staged) {
		ret = stash_staged(info, patch, quiet);
		ਜੇ (ret < 0) {
			ਜੇ (!quiet)
				fprintf_ln(stderr, _("Cannot save the current "
						     "staged state"));
			ਜਾਓ ਪੂਰਾ;
		} ਨਹੀਂਤਾਂ ਜੇ (ret > 0) {
			ਜਾਓ ਪੂਰਾ;
		}
	} ਨਹੀਂਤਾਂ {
		ਜੇ (stash_working_ਰੁੱਖ(info, ps)) {
			ਜੇ (!quiet)
				fprintf_ln(stderr, _("Cannot save the current "
						     "worktree state"));
			ret = -1;
			ਜਾਓ ਪੂਰਾ;
		}
	}

	ਜੇ (!stash_ਸੁਨੇਹਾ_buf->len)
		strbuf_addf(stash_ਸੁਨੇਹਾ_buf, "WIP on %s", ਸੁਨੇਹਾ.buf);
	ਨਹੀਂਤਾਂ
		strbuf_insertf(stash_ਸੁਨੇਹਾ_buf, 0, "On %s: ", branch_name);

	ਜੇ (untracked_commit_option)
		commit_list_ਪਾਓ(lookup_commit(the_repository,
						 &info->u_commit),
				   &parents);
	commit_list_ਪਾਓ(lookup_commit(the_repository, &info->i_commit),
			   &parents);
	commit_list_ਪਾਓ(ਸਿਰ_commit, &parents);

	ਜੇ (commit_ਰੁੱਖ(stash_ਸੁਨੇਹਾ_buf->buf, stash_ਸੁਨੇਹਾ_buf->len, &info->w_ਰੁੱਖ,
			parents, &info->w_commit, ਨਿਰਾ, ਨਿਰਾ)) {
		ਜੇ (!quiet)
			fprintf_ln(stderr, _("Cannot record "
					     "working tree state"));
		ret = -1;
		ਜਾਓ ਪੂਰਾ;
	}

ਪੂਰਾ:
	strbuf_release(&commit_ਰੁੱਖ_label);
	strbuf_release(&ਸੁਨੇਹਾ);
	strbuf_release(&untracked_files);
	commit_list_free(parents);
	ਮੁਕਤ(branch_name_buf);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ create_stash(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ ਅਣਵਰਤਿਆ,
			ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ ret;
	ਬਣਤਰ strbuf stash_ਸੁਨੇਹਾ_buf = STRBUF_INIT;
	ਬਣਤਰ stash_info info = STASH_INFO_INIT;
	ਬਣਤਰ pathspec ps;

	/* Starting with argv[1], since argv[0] is "create" */
	strbuf_join_argv(&stash_ਸੁਨੇਹਾ_buf, argc - 1, ++argv, ' ');

	memset(&ps, 0, ਆਕਾਰ(ps));
	ਜੇ (!check_changes_tracked_files(&ps))
		ਵਾਪਸ 0;

	ret = do_create_stash(&ps, &stash_ਸੁਨੇਹਾ_buf, 0, 0, ਨਿਰਾ, 0, &info,
			      ਨਿਰਾ, 0);
	ਜੇ (!ret)
		printf_ln("%s", oid_ਮੰਜ਼ਿਲ_hex(&info.w_commit));

	free_stash_info(&info);
	strbuf_release(&stash_ਸੁਨੇਹਾ_buf);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ do_ਧੱਕੋ_stash(ਅਚਲ ਬਣਤਰ pathspec *ps, ਅਚਲ ਅੱਖਰ *stash_ਸੁਨੇਹਾ, ਪੂਰਨ quiet,
			 ਪੂਰਨ keep_ਸੂਚਕਾਂਕ, ਪੂਰਨ patch_ਬਹੁਲਕ,
			 ਬਣਤਰ interactive_options *interactive_opts,
			 ਪੂਰਨ include_untracked, ਪੂਰਨ only_staged)
{
	ਪੂਰਨ ret = 0;
	ਬਣਤਰ stash_info info = STASH_INFO_INIT;
	ਬਣਤਰ strbuf patch = STRBUF_INIT;
	ਬਣਤਰ strbuf stash_ਸੁਨੇਹਾ_buf = STRBUF_INIT;
	ਬਣਤਰ strbuf untracked_files = STRBUF_INIT;
	ਬਣਤਰ strbuf ਬਾਹਰਲਾ = STRBUF_INIT;

	ਜੇ (patch_ਬਹੁਲਕ && keep_ਸੂਚਕਾਂਕ == -1)
		keep_ਸੂਚਕਾਂਕ = 1;

	ਜੇ (patch_ਬਹੁਲਕ && include_untracked) {
		fprintf_ln(stderr, _("Can't use --patch and --include-untracked"
				     " or --all at the same time"));
		ret = -1;
		ਜਾਓ ਪੂਰਾ;
	}

	/* --patch overrides --staged */
	ਜੇ (patch_ਬਹੁਲਕ)
		only_staged = 0;

	ਜੇ (only_staged && include_untracked) {
		fprintf_ln(stderr, _("Can't use --staged and --include-untracked"
				     " or --all at the same time"));
		ret = -1;
		ਜਾਓ ਪੂਰਾ;
	}

	repo_read_ਸੂਚਕਾਂਕ_preload(the_repository, ਨਿਰਾ, 0);
	ਜੇ (!include_untracked && ps->nr) {
		ਅੱਖਰ *ps_matched = xcalloc(ps->nr, 1);

		ਜੇ (pathspec_needs_expanded_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ, ps))
			ensure_ਭਰਿਆ_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ);
		ਲਈ (ਮਾਪ_t i = 0; i < the_repository->ਸੂਚਕਾਂਕ->cache_nr; i++)
			ce_ਰਸਤਾ_ਮੇਲ(the_repository->ਸੂਚਕਾਂਕ, the_repository->ਸੂਚਕਾਂਕ->cache[i], ps,
				      ps_matched);

		ਜੇ (report_ਰਸਤਾ_error(ps_matched, ps)) {
			fprintf_ln(stderr, _("Did you forget to 'git add'?"));
			ret = -1;
			ਮੁਕਤ(ps_matched);
			ਜਾਓ ਪੂਰਾ;
		}
		ਮੁਕਤ(ps_matched);
	}

	ਜੇ (repo_refresh_and_write_ਸੂਚਕਾਂਕ(the_repository, REFRESH_QUIET, 0, 0,
					 ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ)) {
		ret = error(_("could not write index"));
		ਜਾਓ ਪੂਰਾ;
	}

	ਜੇ (!check_changes(ps, include_untracked, &untracked_files)) {
		ਜੇ (!quiet)
			printf_ln(_("No local changes to save"));
		ਜਾਓ ਪੂਰਾ;
	}

	ਜੇ (!refs_reflog_exists(get_main_ref_store(the_repository), ref_stash) && do_clear_stash()) {
		ret = -1;
		ਜੇ (!quiet)
			fprintf_ln(stderr, _("Cannot initialize stash"));
		ਜਾਓ ਪੂਰਾ;
	}

	ਜੇ (stash_ਸੁਨੇਹਾ)
		strbuf_addstr(&stash_ਸੁਨੇਹਾ_buf, stash_ਸੁਨੇਹਾ);
	ਜੇ (do_create_stash(ps, &stash_ਸੁਨੇਹਾ_buf, include_untracked, patch_ਬਹੁਲਕ,
			    interactive_opts, only_staged, &info, &patch, quiet)) {
		ret = -1;
		ਜਾਓ ਪੂਰਾ;
	}

	ਜੇ (do_store_stash(&info.w_commit, stash_ਸੁਨੇਹਾ_buf.buf, 1)) {
		ret = -1;
		ਜੇ (!quiet)
			fprintf_ln(stderr, _("Cannot save the current status"));
		ਜਾਓ ਪੂਰਾ;
	}

	ਜੇ (!quiet)
		printf_ln(_("Saved working directory and index state %s"),
			  stash_ਸੁਨੇਹਾ_buf.buf);

	ਜੇ (!(patch_ਬਹੁਲਕ || only_staged)) {
		ਜੇ (include_untracked && !ps->nr) {
			ਬਣਤਰ ਬੱਚਾ_process cp = ਬੱਚਾ_PROCESS_INIT;

			cp.git_cmd = 1;
			ਜੇ (startup_info->original_cwd) {
				cp.dir = startup_info->original_cwd;
				strvec_pushf(&cp.env, "%s=%s",
					     GIT_WORK_ਰੁੱਖ_ENVIRONMENT,
					     the_repository->worktree);
			}
			strvec_pushl(&cp.ਦਲੀਲਾਂ, "clean", "--force",
				     "--quiet", "-d", ":/", ਨਿਰਾ);
			ਜੇ (include_untracked == INCLUDE_ALL_FILES)
				strvec_ਧੱਕੋ(&cp.ਦਲੀਲਾਂ, "-x");
			ਜੇ (ਲੜੀਵਾਰ_command(&cp)) {
				ret = -1;
				ਜਾਓ ਪੂਰਾ;
			}
		}
		discard_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ);
		ਜੇ (ps->nr) {
			ਬਣਤਰ ਬੱਚਾ_process cp_ਜੋੜੋ = ਬੱਚਾ_PROCESS_INIT;
			ਬਣਤਰ ਬੱਚਾ_process cp_diff = ਬੱਚਾ_PROCESS_INIT;
			ਬਣਤਰ ਬੱਚਾ_process cp_apply = ਬੱਚਾ_PROCESS_INIT;

			cp_ਜੋੜੋ.git_cmd = 1;
			strvec_ਧੱਕੋ(&cp_ਜੋੜੋ.ਦਲੀਲਾਂ, "add");
			ਜੇ (!include_untracked)
				strvec_ਧੱਕੋ(&cp_ਜੋੜੋ.ਦਲੀਲਾਂ, "-u");
			ਜੇ (include_untracked == INCLUDE_ALL_FILES)
				strvec_ਧੱਕੋ(&cp_ਜੋੜੋ.ਦਲੀਲਾਂ, "--force");
			strvec_ਧੱਕੋ(&cp_ਜੋੜੋ.ਦਲੀਲਾਂ, "--");
			ਜੋੜੋ_pathspecs(&cp_ਜੋੜੋ.ਦਲੀਲਾਂ, ps);
			ਜੇ (ਲੜੀਵਾਰ_command(&cp_ਜੋੜੋ)) {
				ret = -1;
				ਜਾਓ ਪੂਰਾ;
			}

			cp_diff.git_cmd = 1;
			strvec_pushl(&cp_diff.ਦਲੀਲਾਂ, "diff-index", "-p",
				     "--no-color",
				     "--cached", "--binary", "HEAD", "--",
				     ਨਿਰਾ);
			ਜੋੜੋ_pathspecs(&cp_diff.ਦਲੀਲਾਂ, ps);
			ਜੇ (pipe_command(&cp_diff, ਨਿਰਾ, 0, &ਬਾਹਰਲਾ, 0, ਨਿਰਾ, 0)) {
				ret = -1;
				ਜਾਓ ਪੂਰਾ;
			}

			cp_apply.git_cmd = 1;
			strvec_pushl(&cp_apply.ਦਲੀਲਾਂ, "apply", "--index",
				     "-R", ਨਿਰਾ);
			ਜੇ (pipe_command(&cp_apply, ਬਾਹਰਲਾ.buf, ਬਾਹਰਲਾ.len, ਨਿਰਾ, 0,
					 ਨਿਰਾ, 0)) {
				ret = -1;
				ਜਾਓ ਪੂਰਾ;
			}
		} ਨਹੀਂਤਾਂ {
			ਬਣਤਰ ਬੱਚਾ_process cp = ਬੱਚਾ_PROCESS_INIT;
			cp.git_cmd = 1;
			/* BUG: this nukes untracked files in the way */
			strvec_pushl(&cp.ਦਲੀਲਾਂ, "reset", "--hard", "-q",
				     "--no-recurse-submodules", ਨਿਰਾ);
			ਜੇ (ਲੜੀਵਾਰ_command(&cp)) {
				ret = -1;
				ਜਾਓ ਪੂਰਾ;
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
		ਜੇ (keep_ਸੂਚਕਾਂਕ == 1 && !is_null_oid(&info.i_ਰੁੱਖ) &&
		    !is_ਖਾਲੀ_ਥਾਵਾਂ_ਰੁੱਖ_oid(&info.i_ਰੁੱਖ, the_repository->hash_algo)) {
			ਬਣਤਰ ਬੱਚਾ_process cp = ਬੱਚਾ_PROCESS_INIT;

			cp.git_cmd = 1;
			strvec_pushl(&cp.ਦਲੀਲਾਂ, "checkout", "--no-overlay",
				     oid_ਮੰਜ਼ਿਲ_hex(&info.i_ਰੁੱਖ), "--", ਨਿਰਾ);
			ਜੇ (!ps->nr)
				strvec_ਧੱਕੋ(&cp.ਦਲੀਲਾਂ, ":/");
			ਨਹੀਂਤਾਂ
				ਜੋੜੋ_pathspecs(&cp.ਦਲੀਲਾਂ, ps);
			ਜੇ (ਲੜੀਵਾਰ_command(&cp)) {
				ret = -1;
				ਜਾਓ ਪੂਰਾ;
			}
		}
		ਜਾਓ ਪੂਰਾ;
	} ਨਹੀਂਤਾਂ {
		ਬਣਤਰ ਬੱਚਾ_process cp = ਬੱਚਾ_PROCESS_INIT;

		cp.git_cmd = 1;
		strvec_pushl(&cp.ਦਲੀਲਾਂ, "apply", "-R", ਨਿਰਾ);

		ਜੇ (pipe_command(&cp, patch.buf, patch.len, ਨਿਰਾ, 0, ਨਿਰਾ, 0)) {
			ਜੇ (!quiet)
				fprintf_ln(stderr, _("Cannot remove "
						     "worktree changes"));
			ret = -1;
			ਜਾਓ ਪੂਰਾ;
		}

		ਜੇ (keep_ਸੂਚਕਾਂਕ < 1) {
			ਬਣਤਰ ਬੱਚਾ_process cp = ਬੱਚਾ_PROCESS_INIT;

			cp.git_cmd = 1;
			strvec_pushl(&cp.ਦਲੀਲਾਂ, "reset", "-q", "--refresh", "--",
				     ਨਿਰਾ);
			ਜੋੜੋ_pathspecs(&cp.ਦਲੀਲਾਂ, ps);
			ਜੇ (ਲੜੀਵਾਰ_command(&cp)) {
				ret = -1;
				ਜਾਓ ਪੂਰਾ;
			}
		}
		ਜਾਓ ਪੂਰਾ;
	}

ਪੂਰਾ:
	strbuf_release(&patch);
	strbuf_release(&ਬਾਹਰਲਾ);
	free_stash_info(&info);
	strbuf_release(&stash_ਸੁਨੇਹਾ_buf);
	strbuf_release(&untracked_files);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ ਧੱਕੋ_stash(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		      ਪੂਰਨ ਧੱਕੋ_assumed)
{
	ਪੂਰਨ force_assume = 0;
	ਪੂਰਨ keep_ਸੂਚਕਾਂਕ = -1;
	ਪੂਰਨ only_staged = 0;
	ਪੂਰਨ patch_ਬਹੁਲਕ = 0;
	ਪੂਰਨ include_untracked = 0;
	ਪੂਰਨ quiet = 0;
	ਪੂਰਨ pathspec_file_nul = 0;
	ਅਚਲ ਅੱਖਰ *stash_ਸੁਨੇਹਾ = ਨਿਰਾ;
	ਅੱਖਰ *pathspec_from_file = ਨਿਰਾ;
	ਬਣਤਰ pathspec ps;
	ਬਣਤਰ interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	ਬਣਤਰ option options[] = {
		OPT_BOOL('k', "keep-index", &keep_ਸੂਚਕਾਂਕ,
			 N_("keep index")),
		OPT_BOOL('S', "staged", &only_staged,
			 N_("stash staged changes only")),
		OPT_BOOL('p', "patch", &patch_ਬਹੁਲਕ,
			 N_("stash in patch mode")),
		OPT_BOOL(0, "auto-advance", &interactive_opts.auto_advance,
			 N_("auto advance to the next file when selecting hunks interactively")),
		OPT_DIFF_UNIFIED(&interactive_opts.context),
		OPT_DIFF_INTERHUNK_CONTEXT(&interactive_opts.interhunkcontext),
		OPT__QUIET(&quiet, N_("quiet mode")),
		OPT_BOOL('u', "include-untracked", &include_untracked,
			 N_("include untracked files in stash")),
		OPT_ਸੈੱਟ_INT('a', "all", &include_untracked,
			    N_("include ignore files"), INCLUDE_ALL_FILES),
		OPT_STRING('m', "message", &stash_ਸੁਨੇਹਾ, N_("message"),
			   N_("stash message")),
		OPT_PATHSPEC_FROM_FILE(&pathspec_from_file),
		OPT_PATHSPEC_FILE_NUL(&pathspec_file_nul),
		OPT_ਸਿਰਾ()
	};
	ਪੂਰਨ ret;

	ਜੇ (argc) {
		ਪੂਰਨ flags = PARSE_OPT_KEEP_DASHDASH;

		ਜੇ (ਧੱਕੋ_assumed)
			flags |= PARSE_OPT_STOP_AT_NON_OPTION;

		argc = parse_options(argc, argv, ਅਗੇਤਰ, options,
				     ਧੱਕੋ_assumed ? git_stash_usage :
				     git_stash_ਧੱਕੋ_usage, flags);
		force_assume |= patch_ਬਹੁਲਕ;
	}

	ਜੇ (argc) {
		ਜੇ (!strcmp(argv[0], "--")) {
			argc--;
			argv++;
		} ਨਹੀਂਤਾਂ ਜੇ (ਧੱਕੋ_assumed && !force_assume) {
			die("subcommand wasn't specified; 'push' can't be assumed due to unexpected token '%s'",
			    argv[0]);
		}
	}

	parse_pathspec(&ps, 0, PATHSPEC_PREFER_ਭਰਿਆ | PATHSPEC_ਅਗੇਤਰ_ORIGIN,
		       ਅਗੇਤਰ, argv);

	ਜੇ (pathspec_from_file) {
		ਜੇ (patch_ਬਹੁਲਕ)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--patch");

		ਜੇ (only_staged)
			die(_("options '%s' and '%s' cannot be used together"), "--pathspec-from-file", "--staged");

		ਜੇ (ps.nr)
			die(_("'%s' and pathspec arguments cannot be used together"), "--pathspec-from-file");

		parse_pathspec_file(&ps, 0,
				    PATHSPEC_PREFER_ਭਰਿਆ | PATHSPEC_ਅਗੇਤਰ_ORIGIN,
				    ਅਗੇਤਰ, pathspec_from_file, pathspec_file_nul);
	} ਨਹੀਂਤਾਂ ਜੇ (pathspec_file_nul) {
		die(_("the option '%s' requires '%s'"), "--pathspec-file-nul", "--pathspec-from-file");
	}

	ਜੇ (!patch_ਬਹੁਲਕ) {
		ਜੇ (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--patch");
		ਜੇ (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--patch");
		ਜੇ (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--patch");
	}

	ਜੇ (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	ਜੇ (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	ret = do_ਧੱਕੋ_stash(&ps, stash_ਸੁਨੇਹਾ, quiet, keep_ਸੂਚਕਾਂਕ, patch_ਬਹੁਲਕ,
			    &interactive_opts, include_untracked, only_staged);

	clear_pathspec(&ps);
	ਮੁਕਤ(pathspec_from_file);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ ਧੱਕੋ_stash_unassumed(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
				ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਵਾਪਸ ਧੱਕੋ_stash(argc, argv, ਅਗੇਤਰ, 0);
}

ਸਥਿਰ ਪੂਰਨ save_stash(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		      ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ keep_ਸੂਚਕਾਂਕ = -1;
	ਪੂਰਨ only_staged = 0;
	ਪੂਰਨ patch_ਬਹੁਲਕ = 0;
	ਪੂਰਨ include_untracked = 0;
	ਪੂਰਨ quiet = 0;
	ਪੂਰਨ ret = 0;
	ਅਚਲ ਅੱਖਰ *stash_ਸੁਨੇਹਾ = ਨਿਰਾ;
	ਬਣਤਰ pathspec ps;
	ਬਣਤਰ strbuf stash_ਸੁਨੇਹਾ_buf = STRBUF_INIT;
	ਬਣਤਰ interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	ਬਣਤਰ option options[] = {
		OPT_BOOL('k', "keep-index", &keep_ਸੂਚਕਾਂਕ,
			 N_("keep index")),
		OPT_BOOL('S', "staged", &only_staged,
			 N_("stash staged changes only")),
		OPT_BOOL('p', "patch", &patch_ਬਹੁਲਕ,
			 N_("stash in patch mode")),
		OPT_BOOL(0, "auto-advance", &interactive_opts.auto_advance,
			 N_("auto advance to the next file when selecting hunks interactively")),
		OPT_DIFF_UNIFIED(&interactive_opts.context),
		OPT_DIFF_INTERHUNK_CONTEXT(&interactive_opts.interhunkcontext),
		OPT__QUIET(&quiet, N_("quiet mode")),
		OPT_BOOL('u', "include-untracked", &include_untracked,
			 N_("include untracked files in stash")),
		OPT_ਸੈੱਟ_INT('a', "all", &include_untracked,
			    N_("include ignore files"), INCLUDE_ALL_FILES),
		OPT_STRING('m', "message", &stash_ਸੁਨੇਹਾ, "message",
			   N_("stash message")),
		OPT_ਸਿਰਾ()
	};

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options,
			     git_stash_save_usage,
			     PARSE_OPT_KEEP_DASHDASH);

	ਜੇ (argc)
		stash_ਸੁਨੇਹਾ = strbuf_join_argv(&stash_ਸੁਨੇਹਾ_buf, argc, argv, ' ');

	memset(&ps, 0, ਆਕਾਰ(ps));

	ਜੇ (interactive_opts.context < -1)
		die(_("'%s' cannot be negative"), "--unified");
	ਜੇ (interactive_opts.interhunkcontext < -1)
		die(_("'%s' cannot be negative"), "--inter-hunk-context");

	ਜੇ (!patch_ਬਹੁਲਕ) {
		ਜੇ (interactive_opts.context != -1)
			die(_("the option '%s' requires '%s'"), "--unified", "--patch");
		ਜੇ (interactive_opts.interhunkcontext != -1)
			die(_("the option '%s' requires '%s'"), "--inter-hunk-context", "--patch");
		ਜੇ (!interactive_opts.auto_advance)
			die(_("the option '%s' requires '%s'"), "--no-auto-advance", "--patch");
	}

	ret = do_ਧੱਕੋ_stash(&ps, stash_ਸੁਨੇਹਾ, quiet, keep_ਸੂਚਕਾਂਕ,
			    patch_ਬਹੁਲਕ, &interactive_opts, include_untracked,
			    only_staged);

	strbuf_release(&stash_ਸੁਨੇਹਾ_buf);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ write_commit_with_parents(ਬਣਤਰ repository *r,
				     ਬਣਤਰ object_id *ਬਾਹਰਲਾ,
				     ਅਚਲ ਬਣਤਰ object_id *oid,
				     ਬਣਤਰ commit_list *parents)
{
	ਮਾਪ_t author_len, committer_len;
	ਬਣਤਰ commit *this;
	ਅਚਲ ਅੱਖਰ *orig_author, *orig_committer;
	ਅੱਖਰ *author = ਨਿਰਾ, *committer = ਨਿਰਾ;
	ਅਚਲ ਅੱਖਰ *ਬਫਰ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ bufsize;
	ਅਚਲ ਅੱਖਰ *p;
	ਬਣਤਰ strbuf ਸੁਨੇਹਾ = STRBUF_INIT;
	ਪੂਰਨ ret = 0;
	ਬਣਤਰ ident_ਵੰਡੋ id;

	this = lookup_commit_reference(r, oid);
	ਬਫਰ = repo_get_commit_ਬਫਰ(r, this, &bufsize);
	orig_author = ਲੱਭੋ_commit_ਸਿਰਲੇਖ(ਬਫਰ, "author", &author_len);
	orig_committer = ਲੱਭੋ_commit_ਸਿਰਲੇਖ(ਬਫਰ, "committer", &committer_len);

	ਜੇ (!orig_author || !orig_committer) {
		ret = error(_("cannot parse commit %s"), oid_ਮੰਜ਼ਿਲ_hex(oid));
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	ਜੇ (ਵੰਡੋ_ident_ਸਤਰ(&id, orig_author, author_len) < 0 ||
	    ਵੰਡੋ_ident_ਸਤਰ(&id, orig_committer, committer_len) < 0) {
		ret = error(_("invalid author or committer for %s"), oid_ਮੰਜ਼ਿਲ_hex(oid));
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	p = ਲੜੀ_ਖੋਜ(ਬਫਰ, "\n\n");
	strbuf_addstr(&ਸੁਨੇਹਾ, "git stash: ");

	ਜੇ (p)
		strbuf_ਜੋੜੋ(&ਸੁਨੇਹਾ, p + 2, bufsize - (p + 2 - ਬਫਰ));
	strbuf_complete_ਸਤਰ(&ਸੁਨੇਹਾ);

	author = xmemdupz(orig_author, author_len);
	committer = xmemdupz(orig_committer, committer_len);

	ਜੇ (commit_ਰੁੱਖ_extended(ਸੁਨੇਹਾ.buf, ਸੁਨੇਹਾ.len,
				 r->hash_algo->ਖਾਲੀ_ਥਾਵਾਂ_ਰੁੱਖ, parents,
				 ਬਾਹਰਲਾ, author, committer,
				 ਨਿਰਾ, ਨਿਰਾ)) {
		ret = error(_("could not write commit"));
		ਜਾਓ ਬਾਹਰਲਾ;
	}
ਬਾਹਰਲਾ:
	strbuf_release(&ਸੁਨੇਹਾ);
	repo_unuse_commit_ਬਫਰ(r, this, ਬਫਰ);
	ਮੁਕਤ(author);
	ਮੁਕਤ(committer);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ do_import_stash(ਬਣਤਰ repository *r, ਅਚਲ ਅੱਖਰ *rev)
{
	ਬਣਤਰ object_id chain;
	ਪੂਰਨ res = 0;
	ਅਚਲ ਅੱਖਰ *ਬਫਰ = ਨਿਰਾ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ bufsize;
	ਬਣਤਰ commit *this = ਨਿਰਾ;
	ਬਣਤਰ commit_list *ਚੀਜ਼ਾਂ = ਨਿਰਾ, *cur;
	ਅੱਖਰ *ਸੁਨੇਹਾ = ਨਿਰਾ;

	ਜੇ (repo_get_oid(r, rev, &chain))
		ਵਾਪਸ error(_("not a valid revision: %s"), rev);

	this = lookup_commit_reference(r, &chain);
	ਜੇ (!this)
		ਵਾਪਸ error(_("not a commit: %s"), rev);

	/*
	 * Walk the commit history, finding each stash entry, and load data into
	 * the array.
	 */
	ਲਈ (;;) {
		ਅਚਲ ਅੱਖਰ *author, *committer;
		ਮਾਪ_t author_len, committer_len;
		ਅਚਲ ਅੱਖਰ *p;
		ਅਚਲ ਅੱਖਰ *expected = "git stash <git@stash> 1000684800 +0000";
		ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ = "git stash: ";
		ਬਣਤਰ commit *stash;
		ਬਣਤਰ ਰੁੱਖ *ਰੁੱਖ = repo_get_commit_ਰੁੱਖ(r, this);

		ਜੇ (!ਰੁੱਖ ||
		    !oideq(&ਰੁੱਖ->object.oid, r->hash_algo->ਖਾਲੀ_ਥਾਵਾਂ_ਰੁੱਖ) ||
		    (this->parents &&
		     (!this->parents->ਅਗਲਾ || this->parents->ਅਗਲਾ->ਅਗਲਾ))) {
			res = error(_("%s is not a valid exported stash commit"),
					oid_ਮੰਜ਼ਿਲ_hex(&this->object.oid));
			ਜਾਓ ਬਾਹਰਲਾ;
		}

		ਬਫਰ = repo_get_commit_ਬਫਰ(r, this, &bufsize);

		ਜੇ (!this->parents) {
			/*
			 * We don't have any parents.  Make sure this is our
			 * root commit.
			 */
			author = ਲੱਭੋ_commit_ਸਿਰਲੇਖ(ਬਫਰ, "author", &author_len);
			committer = ਲੱਭੋ_commit_ਸਿਰਲੇਖ(ਬਫਰ, "committer", &committer_len);

			ਜੇ (!author || !committer) {
				error(_("cannot parse commit %s"), oid_ਮੰਜ਼ਿਲ_hex(&this->object.oid));
				ਜਾਓ ਬਾਹਰਲਾ;
			}

			ਜੇ (author_len != ਲੜੀ_ਲੰਬਾਈ(expected) ||
			    committer_len != ਲੜੀ_ਲੰਬਾਈ(expected) ||
			    memcmp(author, expected, author_len) ||
			    memcmp(committer, expected, committer_len)) {
				res = error(_("found root commit %s with invalid data"), oid_ਮੰਜ਼ਿਲ_hex(&this->object.oid));
				ਜਾਓ ਬਾਹਰਲਾ;
			}
			ਰੋਕੋ;
		}

		p = ਲੜੀ_ਖੋਜ(ਬਫਰ, "\n\n");
		ਜੇ (!p) {
			res = error(_("cannot parse commit %s"), oid_ਮੰਜ਼ਿਲ_hex(&this->object.oid));
			ਜਾਓ ਬਾਹਰਲਾ;
		}

		p += 2;
		ਜੇ (((ਮਾਪ_t)(bufsize - (p - ਬਫਰ)) < ਲੜੀ_ਲੰਬਾਈ(ਅਗੇਤਰ)) ||
		    memcmp(ਅਗੇਤਰ, p, ਲੜੀ_ਲੰਬਾਈ(ਅਗੇਤਰ))) {
			res = error(_("found stash commit %s without expected prefix"), oid_ਮੰਜ਼ਿਲ_hex(&this->object.oid));
			ਜਾਓ ਬਾਹਰਲਾ;
		}

		stash = this->parents->ਅਗਲਾ->ਵਸਤੂ;

		ਜੇ (repo_parse_commit(r, this->parents->ਵਸਤੂ) ||
		    repo_parse_commit(r, stash)) {
			res = error(_("cannot parse parents of commit: %s"),
					oid_ਮੰਜ਼ਿਲ_hex(&this->object.oid));
			ਜਾਓ ਬਾਹਰਲਾ;
		}

		ਜੇ (check_stash_topology(r, stash)) {
			res = error(_("%s does not look like a stash commit"),
					oid_ਮੰਜ਼ਿਲ_hex(&stash->object.oid));
			ਜਾਓ ਬਾਹਰਲਾ;
		}

		repo_unuse_commit_ਬਫਰ(r, this, ਬਫਰ);
		ਬਫਰ = ਨਿਰਾ;
		ਚੀਜ਼ਾਂ = commit_list_ਪਾਓ(stash, &ਚੀਜ਼ਾਂ);
		this = this->parents->ਵਸਤੂ;
	}

	/*
	 * Now, walk each entry, adding it to the stash as a normal stash
	 * commit.
	 */
	ਲਈ (cur = ਚੀਜ਼ਾਂ; cur; cur = cur->ਅਗਲਾ) {
		ਅਚਲ ਅੱਖਰ *p;
		ਬਣਤਰ object_id *oid;

		this = cur->ਵਸਤੂ;
		oid = &this->object.oid;
		ਬਫਰ = repo_get_commit_ਬਫਰ(r, this, &bufsize);
		ਜੇ (!ਬਫਰ) {
			res = error(_("cannot read commit buffer for %s"), oid_ਮੰਜ਼ਿਲ_hex(oid));
			ਜਾਓ ਬਾਹਰਲਾ;
		}

		p = ਲੜੀ_ਖੋਜ(ਬਫਰ, "\n\n");
		ਜੇ (!p) {
			res = error(_("cannot parse commit %s"), oid_ਮੰਜ਼ਿਲ_hex(oid));
			ਜਾਓ ਬਾਹਰਲਾ;
		}

		p += 2;
		ਸੁਨੇਹਾ = xmemdupz(p, bufsize - (p - ਬਫਰ));
		repo_unuse_commit_ਬਫਰ(r, this, ਬਫਰ);
		ਬਫਰ = ਨਿਰਾ;

		ਜੇ (do_store_stash(oid, ਸੁਨੇਹਾ, 1)) {
			res = error(_("cannot save the stash for %s"), oid_ਮੰਜ਼ਿਲ_hex(oid));
			ਜਾਓ ਬਾਹਰਲਾ;
		}
		FREE_AND_NULL(ਸੁਨੇਹਾ);
	}
ਬਾਹਰਲਾ:
	ਜੇ (this && ਬਫਰ)
		repo_unuse_commit_ਬਫਰ(r, this, ਬਫਰ);
	commit_list_free(ਚੀਜ਼ਾਂ);
	ਮੁਕਤ(ਸੁਨੇਹਾ);

	ਵਾਪਸ res;
}

ਸਥਿਰ ਪੂਰਨ import_stash(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			ਬਣਤਰ repository *repo)
{
	ਬਣਤਰ option options[] = {
		OPT_ਸਿਰਾ()
	};

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options,
			     git_stash_import_usage,
			     PARSE_OPT_KEEP_DASHDASH);

	ਜੇ (argc != 1)
		usage_ਸੁਨੇਹਾ_opt("a revision is required", git_stash_import_usage, options);

	ਵਾਪਸ do_import_stash(repo, argv[0]);
}

ਬਣਤਰ stash_ਇੰਦਰਾਜ_ਅੰਕੜੇ {
	ਬਣਤਰ repository *r;
	ਬਣਤਰ commit_list **ਚੀਜ਼ਾਂ;
	ਮਾਪ_t ਗਿਣਤੀ_2;
};

ਸਥਿਰ ਪੂਰਨ collect_stash_entries(ਅਚਲ ਅੱਖਰ *refname ਅਣਵਰਤਿਆ,
				 ਬਣਤਰ object_id *old_oid ਅਣਵਰਤਿਆ,
				 ਬਣਤਰ object_id *new_oid,
				 ਅਚਲ ਅੱਖਰ *committer ਅਣਵਰਤਿਆ,
				 timestamp_t timestamp ਅਣਵਰਤਿਆ,
				 ਪੂਰਨ tz ਅਣਵਰਤਿਆ, ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ ਅਣਵਰਤਿਆ,
				 ਖਾਲੀ *cb_ਅੰਕੜੇ)
{
	ਬਣਤਰ stash_ਇੰਦਰਾਜ_ਅੰਕੜੇ *ਅੰਕੜੇ = cb_ਅੰਕੜੇ;
	ਬਣਤਰ commit *stash;

	ਅੰਕੜੇ->ਗਿਣਤੀ_2++;
	stash = lookup_commit_reference(ਅੰਕੜੇ->r, new_oid);
	ਜੇ (!stash || check_stash_topology(ਅੰਕੜੇ->r, stash)) {
		ਵਾਪਸ error(_("%s does not look like a stash commit"),
			     oid_ਮੰਜ਼ਿਲ_hex(new_oid));
	}
	ਅੰਕੜੇ->ਚੀਜ਼ਾਂ = commit_list_append(stash, ਅੰਕੜੇ->ਚੀਜ਼ਾਂ);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ do_export_stash(ਬਣਤਰ repository *r,
			   ਅਚਲ ਅੱਖਰ *ref,
			   ਪੂਰਨ argc,
			   ਅਚਲ ਅੱਖਰ **argv)
{
	ਬਣਤਰ object_id ਅਧਾਰ;
	ਬਣਤਰ commit *prev;
	ਬਣਤਰ commit_list *ਚੀਜ਼ਾਂ = ਨਿਰਾ, **iter = &ਚੀਜ਼ਾਂ, *cur;
	ਪੂਰਨ res = 0;
	ਪੂਰਨ i;
	ਬਣਤਰ strbuf revision = STRBUF_INIT;
	ਅਚਲ ਅੱਖਰ *author, *committer;

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
	ਜੇ (commit_ਰੁੱਖ_extended("", 0, r->hash_algo->ਖਾਲੀ_ਥਾਵਾਂ_ਰੁੱਖ, ਨਿਰਾ,
				 &ਅਧਾਰ, author, committer, ਨਿਰਾ, ਨਿਰਾ))
		ਵਾਪਸ error(_("unable to write base commit"));

	prev = lookup_commit_reference(r, &ਅਧਾਰ);

	ਜੇ (argc) {
		/*
		 * Find each specified stash, and load data into the array.
		 */
		ਲਈ (i = 0; i < argc; i++) {
			ਬਣਤਰ object_id oid;
			ਬਣਤਰ commit *stash;

			ਜੇ (parse_stash_revision(&revision, argv[i], 1) ||
			    repo_get_oid_with_flags(r, revision.buf, &oid,
						    GET_OID_QUIETLY |
						    GET_OID_GENTLY)) {
				res = error(_("unable to find stash entry %s"), argv[i]);
				ਜਾਓ ਬਾਹਰਲਾ;
			}

			stash = lookup_commit_reference(r, &oid);
			ਜੇ (!stash || check_stash_topology(r, stash)) {
				res = error(_("%s does not look like a stash commit"),
						revision.buf);
				ਜਾਓ ਬਾਹਰਲਾ;
			}
			iter = commit_list_append(stash, iter);
		}
	} ਨਹੀਂਤਾਂ {
		/*
		 * Walk the reflog, finding each stash entry, and load data into the
		 * array.
		 */
		ਬਣਤਰ stash_ਇੰਦਰਾਜ_ਅੰਕੜੇ cb_ਅੰਕੜੇ = {
			.r = r, .ਚੀਜ਼ਾਂ = iter,
		};
		ਜੇ (refs_for_each_reflog_ent_ਉਲਟ(get_main_ref_store(r),
						     "refs/stash",
						     collect_stash_entries,
						     &cb_ਅੰਕੜੇ) && cb_ਅੰਕੜੇ.ਗਿਣਤੀ_2)
			ਜਾਓ ਬਾਹਰਲਾ;
	}

	/*
	 * Now, create a set of commits identical to the regular stash commits,
	 * but where their first parents form a chain to our original empty
	 * base commit.
	 */
	ਚੀਜ਼ਾਂ = commit_list_ਉਲਟ(ਚੀਜ਼ਾਂ);
	ਲਈ (cur = ਚੀਜ਼ਾਂ; cur; cur = cur->ਅਗਲਾ) {
		ਬਣਤਰ commit_list *parents = ਨਿਰਾ;
		ਬਣਤਰ commit_list **ਅਗਲਾ = &parents;
		ਬਣਤਰ object_id ਬਾਹਰਲਾ;
		ਬਣਤਰ commit *stash = cur->ਵਸਤੂ;

		ਅਗਲਾ = commit_list_append(prev, ਅਗਲਾ);
		ਅਗਲਾ = commit_list_append(stash, ਅਗਲਾ);
		res = write_commit_with_parents(r, &ਬਾਹਰਲਾ, &stash->object.oid, parents);
		commit_list_free(parents);
		ਜੇ (res)
			ਜਾਓ ਬਾਹਰਲਾ;
		prev = lookup_commit_reference(r, &ਬਾਹਰਲਾ);
	}
	ਜੇ (ref)
		refs_update_ref(get_main_ref_store(r), ਨਿਰਾ, ref,
				&prev->object.oid, ਨਿਰਾ, 0, UPDATE_REFS_DIE_ON_ERR);
	ਨਹੀਂਤਾਂ
		puts(oid_ਮੰਜ਼ਿਲ_hex(&prev->object.oid));
ਬਾਹਰਲਾ:
	strbuf_release(&revision);
	commit_list_free(ਚੀਜ਼ਾਂ);

	ਵਾਪਸ res;
}

ਗਿਣਤੀ export_action {
	ACTION_NONE,
	ACTION_PRINT,
	ACTION_ਮੰਜ਼ਿਲ_REF,
};

ਸਥਿਰ ਪੂਰਨ export_stash(ਪੂਰਨ argc,
			ਅਚਲ ਅੱਖਰ **argv,
			ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			ਬਣਤਰ repository *repo)
{
	ਅਚਲ ਅੱਖਰ *ref = ਨਿਰਾ;
	ਗਿਣਤੀ export_action action = ACTION_NONE;
	ਬਣਤਰ option options[] = {
		OPT_CMDMODE(0, "print", &action,
			    N_("print the object ID instead of writing it to a ref"),
			    ACTION_PRINT),
		OPT_STRING(0, "to-ref", &ref, "ref",
			    N_("save the data to the given ref")),
		OPT_ਸਿਰਾ()
	};

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options,
			     git_stash_export_usage,
			     PARSE_OPT_KEEP_DASHDASH);

	ਜੇ (ref && action == ACTION_NONE)
		action = ACTION_ਮੰਜ਼ਿਲ_REF;

	ਜੇ (action == ACTION_NONE || (ref && action == ACTION_PRINT))
		ਵਾਪਸ error(_("exactly one of --print and --to-ref is required"));

	ਵਾਪਸ do_export_stash(repo, ref, argc, argv);
}

ਪੂਰਨ cmd_stash(ਪੂਰਨ argc,
	      ਅਚਲ ਅੱਖਰ **argv,
	      ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
	      ਬਣਤਰ repository *repo)
{
	pid_t pid = getpid();
	ਅਚਲ ਅੱਖਰ *ਸੂਚਕਾਂਕ_file;
	ਬਣਤਰ strvec ਦਲੀਲਾਂ = STRVEC_INIT;
	parse_opt_subcommand_fn *fn = ਨਿਰਾ;
	ਬਣਤਰ option options[] = {
		OPT_SUBCOMMAND("apply", &fn, apply_stash),
		OPT_SUBCOMMAND("clear", &fn, clear_stash),
		OPT_SUBCOMMAND("drop", &fn, drop_stash),
		OPT_SUBCOMMAND("pop", &fn, ਕੱਢੋ_stash),
		OPT_SUBCOMMAND("branch", &fn, branch_stash),
		OPT_SUBCOMMAND("list", &fn, list_stash),
		OPT_SUBCOMMAND("show", &fn, show_stash),
		OPT_SUBCOMMAND("store", &fn, store_stash),
		OPT_SUBCOMMAND("create", &fn, create_stash),
		OPT_SUBCOMMAND("push", &fn, ਧੱਕੋ_stash_unassumed),
		OPT_SUBCOMMAND("export", &fn, export_stash),
		OPT_SUBCOMMAND("import", &fn, import_stash),
		OPT_SUBCOMMAND_F("save", &fn, save_stash, ਨਿਰਾ, PARSE_OPT_NOCOMPLETE),
		OPT_ਸਿਰਾ()
	};
	ਅਚਲ ਅੱਖਰ **ਦਲੀਲਾਂ_copy;
	ਪੂਰਨ ret;

	repo_config(the_repository, git_stash_config, ਨਿਰਾ);

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, git_stash_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL |
			     PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_ਭਰਿਆ_ਸੂਚਕਾਂਕ = 0;

	ਸੂਚਕਾਂਕ_file = repo_get_ਸੂਚਕਾਂਕ_file(the_repository);
	strbuf_addf(&stash_ਸੂਚਕਾਂਕ_ਰਸਤਾ, "%s.stash.%" PRIuMAX, ਸੂਚਕਾਂਕ_file,
		    (uintmax_t)pid);

	ਜੇ (fn) {
		ret = fn(argc, argv, ਅਗੇਤਰ, repo);

		/*
		 * The subcommand implementations return 0 on success, a
		 * negative value on failure, and STASH_APPLY_CONFLICT
		 * when applying a stash entry resulted in conflicts.
		 * Map failures to 128, the status die() uses, so that
		 * exit status 1 unambiguously indicates conflicts.
		 */
		ਜੇ (ret < 0)
			ਵਾਪਸ 128;
		ਵਾਪਸ ret;
	} ਨਹੀਂਤਾਂ ਜੇ (!argc) {
		ਵਾਪਸ !!ਧੱਕੋ_stash_unassumed(0, ਨਿਰਾ, ਅਗੇਤਰ, repo);
	}

	/* Assume 'stash push' */
	strvec_ਧੱਕੋ(&ਦਲੀਲਾਂ, "push");
	strvec_pushv(&ਦਲੀਲਾਂ, argv);

	/*
	 * `push_stash()` ends up modifying the array, which causes memory
	 * leaks if we didn't copy the array here.
	 */
	DUP_ARRAY(ਦਲੀਲਾਂ_copy, ਦਲੀਲਾਂ.v, ਦਲੀਲਾਂ.nr);

	ret = !!ਧੱਕੋ_stash(ਦਲੀਲਾਂ.nr, ਦਲੀਲਾਂ_copy, ਅਗੇਤਰ, 1);

	strvec_clear(&ਦਲੀਲਾਂ);
	ਮੁਕਤ(ਦਲੀਲਾਂ_copy);
	ਵਾਪਸ ret;
}
