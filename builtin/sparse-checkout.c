#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DISABLE_SIGN_COMPARE_WARNINGS

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "abspath.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "dir.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-file.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-name.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "path.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "pathspec.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "strbuf.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "string-list.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "lockfile.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "unpack-trees.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "setup.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "sparse-index.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "worktree.h"

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਖਾਲੀ_ਥਾਵਾਂ_ਅਧਾਰ = "";

ਸਥਿਰ ਅੱਖਰ ਅਚਲ * ਅਚਲ builtin_sparse_checkout_usage[] = {
	N_("git sparse-checkout (init | list | set | add | reapply | disable | check-rules | clean) [<options>]"),
	ਨਿਰਾ
};

ਸਥਿਰ ਖਾਲੀ write_patterns_ਮੰਜ਼ਿਲ_file(FILE *fp, ਬਣਤਰ ਨਮੂਨਾ_list *pl)
{
	ਪੂਰਨ i;

	ਲਈ (i = 0; i < pl->nr; i++) {
		ਬਣਤਰ ਰਸਤਾ_ਨਮੂਨਾ *p = pl->patterns[i];

		ਜੇ (p->flags & ਨਮੂਨਾ_FLAG_NEGATIVE)
			fprintf(fp, "!");

		fprintf(fp, "%s", p->ਨਮੂਨਾ);

		ਜੇ (p->flags & ਨਮੂਨਾ_FLAG_MUSTBEDIR)
			fprintf(fp, "/");

		fprintf(fp, "\n");
	}
}

ਸਥਿਰ ਅੱਖਰ ਅਚਲ * ਅਚਲ builtin_sparse_checkout_list_usage[] = {
	"git sparse-checkout list",
	ਨਿਰਾ
};

ਸਥਿਰ ਪੂਰਨ sparse_checkout_list(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
				ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਸਥਿਰ ਬਣਤਰ option builtin_sparse_checkout_list_options[] = {
		OPT_ਸਿਰਾ(),
	};
	ਬਣਤਰ ਨਮੂਨਾ_list pl;
	ਅੱਖਰ *sparse_filename;
	ਪੂਰਨ res;
	ਬਣਤਰ repo_config_ਮੁੱਲਾਂ *cfg = repo_config_ਮੁੱਲਾਂ(the_repository);

	ਤਿਆਰੀ_work_ਰੁੱਖ(the_repository);
	ਜੇ (!cfg->apply_sparse_checkout)
		die(_("this worktree is not sparse"));

	argc = parse_options(argc, argv, ਅਗੇਤਰ,
			     builtin_sparse_checkout_list_options,
			     builtin_sparse_checkout_list_usage, 0);

	memset(&pl, 0, ਆਕਾਰ(pl));

	pl.use_cone_patterns = cfg->core_sparse_checkout_cone;

	sparse_filename = get_sparse_checkout_filename();
	res = ਜੋੜੋ_patterns_from_file_ਮੰਜ਼ਿਲ_list(sparse_filename, "", 0, &pl, ਨਿਰਾ, 0);
	ਮੁਕਤ(sparse_filename);

	ਜੇ (res < 0) {
		warning(_("this worktree is not sparse (sparse-checkout file may not exist)"));
		ਵਾਪਸ 0;
	}

	ਜੇ (pl.use_cone_patterns) {
		ਪੂਰਨ i;
		ਬਣਤਰ ਨਮੂਨਾ_ਇੰਦਰਾਜ *pe;
		ਬਣਤਰ hashmap_iter iter;
		ਬਣਤਰ string_list sl = STRING_LIST_INIT_DUP;

		hashmap_for_each_ਇੰਦਰਾਜ(&pl.recursive_hashmap, &iter, pe, ent) {
			/* pe->pattern starts with "/", skip it */
			string_list_append(&sl, pe->ਨਮੂਨਾ + 1);
		}

		string_list_ਛਾਂਟੋ_u(&sl, 0);

		ਲਈ (i = 0; i < sl.nr; i++) {
			quote_c_style(sl.ਚੀਜ਼ਾਂ[i].string, ਨਿਰਾ, stdout, 0);
			ਲਿਖੋ("\n");
		}

		string_list_clear(&sl, 0);
	} ਨਹੀਂਤਾਂ {
		write_patterns_ਮੰਜ਼ਿਲ_file(stdout, &pl);
	}

	clear_ਨਮੂਨਾ_list(&pl);

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ clean_tracked_sparse_directories(ਬਣਤਰ repository *r)
{
	ਪੂਰਨ i, was_ਭਰਿਆ = 0;
	ਬਣਤਰ strbuf ਰਸਤਾ = STRBUF_INIT;
	ਮਾਪ_t pathlen;
	ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ;
	ਬਣਤਰ string_list sparse_dirs = STRING_LIST_INIT_DUP;

	/*
	 * If we are not using cone mode patterns, then we cannot
	 * delete directories outside of the sparse cone.
	 */
	ਜੇ (!r || !r->ਸੂਚਕਾਂਕ || !r->worktree)
		ਵਾਪਸ;
	ਜੇ (init_sparse_checkout_patterns(r->ਸੂਚਕਾਂਕ) ||
	    !r->ਸੂਚਕਾਂਕ->sparse_checkout_patterns->use_cone_patterns)
		ਵਾਪਸ;

	/*
	 * Use the sparse index as a data structure to assist finding
	 * directories that are safe to delete. This conversion to a
	 * sparse index will not delete directories that contain
	 * conflicted entries or submodules.
	 */
	ਜੇ (r->ਸੂਚਕਾਂਕ->sparse_ਸੂਚਕਾਂਕ == ਸੂਚਕਾਂਕ_EXPANDED) {
		/*
		 * If something, such as a merge conflict or other concern,
		 * prevents us from converting to a sparse index, then do
		 * not try deleting files.
		 */
		ਜੇ (convert_ਮੰਜ਼ਿਲ_sparse(r->ਸੂਚਕਾਂਕ, SPARSE_ਸੂਚਕਾਂਕ_ਸਮ੍ਰਿਤੀ_ONLY))
			ਵਾਪਸ;
		was_ਭਰਿਆ = 1;
	}

	strbuf_addstr(&ਰਸਤਾ, r->worktree);
	strbuf_complete(&ਰਸਤਾ, '/');
	pathlen = ਰਸਤਾ.len;

	/*
	 * Collect directories that have gone out of scope but also
	 * exist on disk, so there is some work to be done. We need to
	 * store the entries in a list before exploring, since that might
	 * expand the sparse-index again.
	 */
	ਲਈ (i = 0; i < r->ਸੂਚਕਾਂਕ->cache_nr; i++) {
		ਬਣਤਰ cache_ਇੰਦਰਾਜ *ce = r->ਸੂਚਕਾਂਕ->cache[i];

		ਜੇ (S_ISSPARSEDIR(ce->ce_ਬਹੁਲਕ) &&
		    repo_file_exists(r, ce->name))
			string_list_append(&sparse_dirs, ce->name);
	}

	for_each_string_list_ਵਸਤੂ(ਵਸਤੂ, &sparse_dirs) {
		ਬਣਤਰ dir_struct dir = DIR_INIT;
		ਬਣਤਰ pathspec p = { 0 };
		ਬਣਤਰ strvec s = STRVEC_INIT;

		strbuf_setlen(&ਰਸਤਾ, pathlen);
		strbuf_addstr(&ਰਸਤਾ, ਵਸਤੂ->string);

		dir.flags |= DIR_SHOW_IGNORED_TOO;

		ਤਿਆਰੀ_standard_excludes(&dir);
		strvec_ਧੱਕੋ(&s, ਰਸਤਾ.buf);

		parse_pathspec(&p, PATHSPEC_GLOB, 0, ਨਿਰਾ, s.v);
		fill_directory(&dir, r->ਸੂਚਕਾਂਕ, &p);

		ਜੇ (dir.nr) {
			warning(_("directory '%s' contains untracked files,"
				  " but is not in the sparse-checkout cone"),
				ਵਸਤੂ->string);
		} ਨਹੀਂਤਾਂ ਜੇ (remove_dir_recursively(&ਰਸਤਾ, 0)) {
			/*
			 * Removal is "best effort". If something blocks
			 * the deletion, then continue with a warning.
			 */
			warning(_("failed to remove directory '%s'"),
				ਵਸਤੂ->string);
		}

		strvec_clear(&s);
		clear_pathspec(&p);
		dir_clear(&dir);
	}

	string_list_clear(&sparse_dirs, 0);
	strbuf_release(&ਰਸਤਾ);

	ਜੇ (was_ਭਰਿਆ)
		ensure_ਭਰਿਆ_ਸੂਚਕਾਂਕ(r->ਸੂਚਕਾਂਕ);
}

ਸਥਿਰ ਪੂਰਨ update_working_directory(ਬਣਤਰ repository *r,
				    ਬਣਤਰ ਨਮੂਨਾ_list *pl)
{
	ਗਿਣਤੀ update_sparsity_ਨਤੀਜਾ ਨਤੀਜਾ;
	ਬਣਤਰ unpack_trees_options o;
	ਬਣਤਰ ਤਾਲਾ_file ਤਾਲਾ_file = ਤਾਲਾ_INIT;
	ਬਣਤਰ ਨਮੂਨਾ_list *old_pl;

	/* If no branch has been checked out, there are no updates to make. */
	ਜੇ (is_ਸੂਚਕਾਂਕ_unborn(r->ਸੂਚਕਾਂਕ))
		ਵਾਪਸ UPDATE_SPARSITY_SUCCESS;

	old_pl = r->ਸੂਚਕਾਂਕ->sparse_checkout_patterns;
	r->ਸੂਚਕਾਂਕ->sparse_checkout_patterns = pl;

	memset(&o, 0, ਆਕਾਰ(o));
	o.verbose_update = isatty(2);
	o.update = 1;
	o.ਸਿਰ_idx = -1;
	o.src_ਸੂਚਕਾਂਕ = r->ਸੂਚਕਾਂਕ;
	o.dst_ਸੂਚਕਾਂਕ = r->ਸੂਚਕਾਂਕ;
	o.skip_sparse_checkout = 0;

	ਤਿਆਰੀ_work_ਰੁੱਖ(the_repository);

	repo_hold_locked_ਸੂਚਕਾਂਕ(r, &ਤਾਲਾ_file, ਤਾਲਾ_DIE_ON_ERROR);

	ਤਿਆਰੀ_unpack_trees_porcelain(&o, "sparse-checkout");
	ਨਤੀਜਾ = update_sparsity(&o, pl);
	clear_unpack_trees_porcelain(&o);

	ਜੇ (ਨਤੀਜਾ == UPDATE_SPARSITY_WARNINGS)
		/*
		 * We don't do any special handling of warnings from untracked
		 * files in the way or dirty entries that can't be removed.
		 */
		ਨਤੀਜਾ = UPDATE_SPARSITY_SUCCESS;
	ਜੇ (ਨਤੀਜਾ == UPDATE_SPARSITY_SUCCESS)
		write_locked_ਸੂਚਕਾਂਕ(r->ਸੂਚਕਾਂਕ, &ਤਾਲਾ_file, COMMIT_ਤਾਲਾ);
	ਨਹੀਂਤਾਂ
		rollback_ਤਾਲਾ_file(&ਤਾਲਾ_file);

	clean_tracked_sparse_directories(r);

	ਜੇ (r->ਸੂਚਕਾਂਕ->sparse_checkout_patterns != pl) {
		clear_ਨਮੂਨਾ_list(r->ਸੂਚਕਾਂਕ->sparse_checkout_patterns);
		FREE_AND_NULL(r->ਸੂਚਕਾਂਕ->sparse_checkout_patterns);
	}
	r->ਸੂਚਕਾਂਕ->sparse_checkout_patterns = old_pl;

	ਵਾਪਸ ਨਤੀਜਾ;
}

ਸਥਿਰ ਅੱਖਰ *escaped_ਨਮੂਨਾ(ਅੱਖਰ *ਨਮੂਨਾ)
{
	ਅੱਖਰ *p = ਨਮੂਨਾ;
	ਬਣਤਰ strbuf final = STRBUF_INIT;

	ਜਦੋਂ (*p) {
		ਜੇ (is_glob_special(*p))
			strbuf_addch(&final, '\\');

		strbuf_addch(&final, *p);
		p++;
	}

	ਵਾਪਸ strbuf_detach(&final, ਨਿਰਾ);
}

ਸਥਿਰ ਖਾਲੀ write_cone_ਮੰਜ਼ਿਲ_file(FILE *fp, ਬਣਤਰ ਨਮੂਨਾ_list *pl)
{
	ਪੂਰਨ i;
	ਬਣਤਰ ਨਮੂਨਾ_ਇੰਦਰਾਜ *pe;
	ਬਣਤਰ hashmap_iter iter;
	ਬਣਤਰ string_list sl = STRING_LIST_INIT_DUP;
	ਬਣਤਰ strbuf ਮਾਪਾ_ਨਮੂਨਾ = STRBUF_INIT;

	hashmap_for_each_ਇੰਦਰਾਜ(&pl->ਮਾਪਾ_hashmap, &iter, pe, ent) {
		ਜੇ (hashmap_get_ਇੰਦਰਾਜ(&pl->recursive_hashmap, pe, ent, ਨਿਰਾ))
			ਜਾਰੀ;

		ਜੇ (!hashmap_ਸ਼ਾਮਲ_ਹੈ_ਮਾਪਾ(&pl->recursive_hashmap,
					     pe->ਨਮੂਨਾ,
					     &ਮਾਪਾ_ਨਮੂਨਾ))
			string_list_append(&sl, pe->ਨਮੂਨਾ);
	}

	string_list_ਛਾਂਟੋ_u(&sl, 0);

	fprintf(fp, "/*\n!/*/\n");

	ਲਈ (i = 0; i < sl.nr; i++) {
		ਅੱਖਰ *ਨਮੂਨਾ = escaped_ਨਮੂਨਾ(sl.ਚੀਜ਼ਾਂ[i].string);

		ਜੇ (ਲੜੀ_ਲੰਬਾਈ(ਨਮੂਨਾ))
			fprintf(fp, "%s/\n!%s/*/\n", ਨਮੂਨਾ, ਨਮੂਨਾ);
		ਮੁਕਤ(ਨਮੂਨਾ);
	}

	string_list_clear(&sl, 0);

	hashmap_for_each_ਇੰਦਰਾਜ(&pl->recursive_hashmap, &iter, pe, ent) {
		ਜੇ (!hashmap_ਸ਼ਾਮਲ_ਹੈ_ਮਾਪਾ(&pl->recursive_hashmap,
					     pe->ਨਮੂਨਾ,
					     &ਮਾਪਾ_ਨਮੂਨਾ))
			string_list_append(&sl, pe->ਨਮੂਨਾ);
	}

	strbuf_release(&ਮਾਪਾ_ਨਮੂਨਾ);

	string_list_ਛਾਂਟੋ_u(&sl, 0);

	ਲਈ (i = 0; i < sl.nr; i++) {
		ਅੱਖਰ *ਨਮੂਨਾ = escaped_ਨਮੂਨਾ(sl.ਚੀਜ਼ਾਂ[i].string);
		fprintf(fp, "%s/\n", ਨਮੂਨਾ);
		ਮੁਕਤ(ਨਮੂਨਾ);
	}

	string_list_clear(&sl, 0);
}

ਸਥਿਰ ਪੂਰਨ write_patterns_and_update(ਬਣਤਰ repository *repo,
				     ਬਣਤਰ ਨਮੂਨਾ_list *pl)
{
	ਅੱਖਰ *sparse_filename;
	FILE *fp;
	ਬਣਤਰ ਤਾਲਾ_file lk = ਤਾਲਾ_INIT;
	ਪੂਰਨ ਨਤੀਜਾ;
	ਬਣਤਰ repo_config_ਮੁੱਲਾਂ *cfg = repo_config_ਮੁੱਲਾਂ(the_repository);

	sparse_filename = get_sparse_checkout_filename();

	ਜੇ (ਸੁਰੱਖਿਅਤ_create_leading_directories(repo, sparse_filename))
		die(_("failed to create directory for sparse-checkout file"));

	repo_hold_ਤਾਲਾ_file_for_update(repo, &lk, sparse_filename,
				       ਤਾਲਾ_DIE_ON_ERROR);

	ਨਤੀਜਾ = update_working_directory(repo, pl);
	ਜੇ (ਨਤੀਜਾ) {
		rollback_ਤਾਲਾ_file(&lk);
		update_working_directory(repo, ਨਿਰਾ);
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	fp = fdopen_ਤਾਲਾ_file(&lk, "w");
	ਜੇ (!fp)
		die_errno(_("unable to fdopen %s"), get_ਤਾਲਾ_file_ਰਸਤਾ(&lk));

	ਜੇ (cfg->core_sparse_checkout_cone)
		write_cone_ਮੰਜ਼ਿਲ_file(fp, pl);
	ਨਹੀਂਤਾਂ
		write_patterns_ਮੰਜ਼ਿਲ_file(fp, pl);

	ਜੇ (commit_ਤਾਲਾ_file(&lk))
		die_errno(_("unable to write %s"), sparse_filename);

ਬਾਹਰਲਾ:
	clear_ਨਮੂਨਾ_list(pl);
	ਮੁਕਤ(sparse_filename);
	ਵਾਪਸ ਨਤੀਜਾ;
}

ਗਿਣਤੀ sparse_checkout_ਬਹੁਲਕ {
	ਬਹੁਲਕ_NO_PATTERNS = 0,
	ਬਹੁਲਕ_ALL_PATTERNS = 1,
	ਬਹੁਲਕ_CONE_PATTERNS = 2,
};

ਸਥਿਰ ਪੂਰਨ ਸੈੱਟ_config(ਬਣਤਰ repository *repo,
		      ਗਿਣਤੀ sparse_checkout_ਬਹੁਲਕ ਬਹੁਲਕ)
{
	/* Update to use worktree config, if not already. */
	ਜੇ (init_worktree_config(repo)) {
		error(_("failed to initialize worktree config"));
		ਵਾਪਸ 1;
	}

	ਜੇ (repo_config_ਸੈੱਟ_worktree_gently(repo,
					    "core.sparseCheckout",
					    ਬਹੁਲਕ ? "true" : "false") ||
	    repo_config_ਸੈੱਟ_worktree_gently(repo,
					    "core.sparseCheckoutCone",
					    ਬਹੁਲਕ == ਬਹੁਲਕ_CONE_PATTERNS ?
						"true" : "false"))
		ਵਾਪਸ 1;

	ਜੇ (ਬਹੁਲਕ == ਬਹੁਲਕ_NO_PATTERNS)
		ਵਾਪਸ ਸੈੱਟ_sparse_ਸੂਚਕਾਂਕ_config(repo, 0);

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਗਿਣਤੀ sparse_checkout_ਬਹੁਲਕ update_cone_ਬਹੁਲਕ(ਪੂਰਨ *cone_ਬਹੁਲਕ) {
	ਬਣਤਰ repo_config_ਮੁੱਲਾਂ *cfg = repo_config_ਮੁੱਲਾਂ(the_repository);

	/* If not specified, use previous definition of cone mode */
	ਜੇ (*cone_ਬਹੁਲਕ == -1 && cfg->apply_sparse_checkout)
		*cone_ਬਹੁਲਕ = cfg->core_sparse_checkout_cone;

	/* Set cone/non-cone mode appropriately */
	cfg->apply_sparse_checkout = 1;
	ਜੇ (*cone_ਬਹੁਲਕ == 1 || *cone_ਬਹੁਲਕ == -1) {
		cfg->core_sparse_checkout_cone = 1;
		ਵਾਪਸ ਬਹੁਲਕ_CONE_PATTERNS;
	}
	cfg->core_sparse_checkout_cone = 0;
	ਵਾਪਸ ਬਹੁਲਕ_ALL_PATTERNS;
}

ਸਥਿਰ ਪੂਰਨ update_modes(ਬਣਤਰ repository *repo, ਪੂਰਨ *cone_ਬਹੁਲਕ, ਪੂਰਨ *sparse_ਸੂਚਕਾਂਕ)
{
	ਪੂਰਨ ਬਹੁਲਕ, ਰਿਕਾਰਡ_ਬਹੁਲਕ;
	ਬਣਤਰ repo_config_ਮੁੱਲਾਂ *cfg = repo_config_ਮੁੱਲਾਂ(the_repository);

	/* Determine if we need to record the mode; ensure sparse checkout on */
	ਰਿਕਾਰਡ_ਬਹੁਲਕ = (*cone_ਬਹੁਲਕ != -1) || !cfg->apply_sparse_checkout;

	ਬਹੁਲਕ = update_cone_ਬਹੁਲਕ(cone_ਬਹੁਲਕ);
	ਜੇ (ਰਿਕਾਰਡ_ਬਹੁਲਕ && ਸੈੱਟ_config(repo, ਬਹੁਲਕ))
		ਵਾਪਸ 1;

	/* Set sparse-index/non-sparse-index mode if specified */
	ਜੇ (*sparse_ਸੂਚਕਾਂਕ >= 0) {
		ਜੇ (ਸੈੱਟ_sparse_ਸੂਚਕਾਂਕ_config(repo, *sparse_ਸੂਚਕਾਂਕ) < 0)
			die(_("failed to modify sparse-index config"));

		/* force an index rewrite */
		repo_read_ਸੂਚਕਾਂਕ(repo);
		repo->ਸੂਚਕਾਂਕ->updated_workdir = 1;

		ਜੇ (!*sparse_ਸੂਚਕਾਂਕ)
			ensure_ਭਰਿਆ_ਸੂਚਕਾਂਕ(repo->ਸੂਚਕਾਂਕ);
	}

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਅੱਖਰ ਅਚਲ * ਅਚਲ builtin_sparse_checkout_init_usage[] = {
	"git sparse-checkout init [--cone] [--[no-]sparse-index]",
	ਨਿਰਾ
};

ਸਥਿਰ ਬਣਤਰ sparse_checkout_init_opts {
	ਪੂਰਨ cone_ਬਹੁਲਕ;
	ਪੂਰਨ sparse_ਸੂਚਕਾਂਕ;
} init_opts;

ਸਥਿਰ ਪੂਰਨ sparse_checkout_init(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
				ਬਣਤਰ repository *repo)
{
	ਬਣਤਰ ਨਮੂਨਾ_list pl;
	ਅੱਖਰ *sparse_filename;
	ਪੂਰਨ res;
	ਬਣਤਰ object_id oid;

	ਸਥਿਰ ਬਣਤਰ option builtin_sparse_checkout_init_options[] = {
		OPT_BOOL(0, "cone", &init_opts.cone_ਬਹੁਲਕ,
			 N_("initialize the sparse-checkout in cone mode")),
		OPT_BOOL(0, "sparse-index", &init_opts.sparse_ਸੂਚਕਾਂਕ,
			 N_("toggle the use of a sparse index")),
		OPT_ਸਿਰਾ(),
	};

	ਤਿਆਰੀ_work_ਰੁੱਖ(the_repository);
	repo_read_ਸੂਚਕਾਂਕ(repo);

	init_opts.cone_ਬਹੁਲਕ = -1;
	init_opts.sparse_ਸੂਚਕਾਂਕ = -1;

	argc = parse_options(argc, argv, ਅਗੇਤਰ,
			     builtin_sparse_checkout_init_options,
			     builtin_sparse_checkout_init_usage, 0);

	ਜੇ (update_modes(repo, &init_opts.cone_ਬਹੁਲਕ, &init_opts.sparse_ਸੂਚਕਾਂਕ))
		ਵਾਪਸ 1;

	memset(&pl, 0, ਆਕਾਰ(pl));

	sparse_filename = get_sparse_checkout_filename();
	res = ਜੋੜੋ_patterns_from_file_ਮੰਜ਼ਿਲ_list(sparse_filename, "", 0, &pl, ਨਿਰਾ, 0);

	/* If we already have a sparse-checkout file, use it. */
	ਜੇ (res >= 0) {
		ਮੁਕਤ(sparse_filename);
		clear_ਨਮੂਨਾ_list(&pl);
		ਵਾਪਸ update_working_directory(repo, ਨਿਰਾ);
	}

	ਜੇ (repo_get_oid(repo, "HEAD", &oid)) {
		FILE *fp;

		/* assume we are in a fresh repo, but update the sparse-checkout file */
		ਜੇ (ਸੁਰੱਖਿਅਤ_create_leading_directories(repo, sparse_filename))
			die(_("unable to create leading directories of %s"),
			    sparse_filename);
		fp = xfopen(sparse_filename, "w");
		ਜੇ (!fp)
			die(_("failed to open '%s'"), sparse_filename);

		ਮੁਕਤ(sparse_filename);
		fprintf(fp, "/*\n!/*/\n");
		fclose(fp);
		ਵਾਪਸ 0;
	}

	ਮੁਕਤ(sparse_filename);

	ਜੋੜੋ_ਨਮੂਨਾ("/*", ਖਾਲੀ_ਥਾਵਾਂ_ਅਧਾਰ, 0, &pl, 0);
	ਜੋੜੋ_ਨਮੂਨਾ("!/*/", ਖਾਲੀ_ਥਾਵਾਂ_ਅਧਾਰ, 0, &pl, 0);
	pl.use_cone_patterns = init_opts.cone_ਬਹੁਲਕ;

	ਵਾਪਸ write_patterns_and_update(repo, &pl);
}

ਸਥਿਰ ਖਾਲੀ ਪਾਓ_recursive_ਨਮੂਨਾ(ਬਣਤਰ ਨਮੂਨਾ_list *pl, ਬਣਤਰ strbuf *ਰਸਤਾ)
{
	ਬਣਤਰ ਨਮੂਨਾ_ਇੰਦਰਾਜ *e = xmalloc(ਆਕਾਰ(*e));
	e->patternlen = ਰਸਤਾ->len;
	e->ਨਮੂਨਾ = strbuf_detach(ਰਸਤਾ, ਨਿਰਾ);
	hashmap_ਇੰਦਰਾਜ_init(&e->ent, fspathhash(e->ਨਮੂਨਾ));

	hashmap_ਜੋੜੋ(&pl->recursive_hashmap, &e->ent);

	ਜਦੋਂ (e->patternlen) {
		ਅੱਖਰ *slash = strrchr(e->ਨਮੂਨਾ, '/');
		ਅੱਖਰ *oldpattern = e->ਨਮੂਨਾ;
		ਮਾਪ_t newlen;
		ਬਣਤਰ ਨਮੂਨਾ_ਇੰਦਰਾਜ *dup;

		ਜੇ (!slash || slash == e->ਨਮੂਨਾ)
			ਰੋਕੋ;

		newlen = slash - e->ਨਮੂਨਾ;
		e = xmalloc(ਆਕਾਰ(ਬਣਤਰ ਨਮੂਨਾ_ਇੰਦਰਾਜ));
		e->patternlen = newlen;
		e->ਨਮੂਨਾ = xstrndup(oldpattern, newlen);
		hashmap_ਇੰਦਰਾਜ_init(&e->ent, fspathhash(e->ਨਮੂਨਾ));

		dup = hashmap_get_ਇੰਦਰਾਜ(&pl->ਮਾਪਾ_hashmap, e, ent, ਨਿਰਾ);
		ਜੇ (!dup) {
			hashmap_ਜੋੜੋ(&pl->ਮਾਪਾ_hashmap, &e->ent);
		} ਨਹੀਂਤਾਂ {
			ਮੁਕਤ(e->ਨਮੂਨਾ);
			ਮੁਕਤ(e);
			e = dup;
		}
	}
}

ਸਥਿਰ ਖਾਲੀ strbuf_ਮੰਜ਼ਿਲ_cone_ਨਮੂਨਾ(ਬਣਤਰ strbuf *ਸਤਰ, ਬਣਤਰ ਨਮੂਨਾ_list *pl)
{
	strbuf_trim(ਸਤਰ);

	strbuf_trim_trailing_dir_sep(ਸਤਰ);

	ਜੇ (strbuf_normalize_ਰਸਤਾ(ਸਤਰ))
		die(_("could not normalize path %s"), ਸਤਰ->buf);

	ਜੇ (!ਸਤਰ->len)
		ਵਾਪਸ;

	ਜੇ (ਸਤਰ->buf[0] != '/')
		strbuf_insertstr(ਸਤਰ, 0, "/");

	ਪਾਓ_recursive_ਨਮੂਨਾ(pl, ਸਤਰ);
}

ਸਥਿਰ ਖਾਲੀ ਜੋੜੋ_patterns_from_ਨਿਵੇਸ਼(ਬਣਤਰ ਨਮੂਨਾ_list *pl,
				    ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv,
				    FILE *file)
{
	ਪੂਰਨ i;
	ਬਣਤਰ repo_config_ਮੁੱਲਾਂ *cfg = repo_config_ਮੁੱਲਾਂ(the_repository);

	ਜੇ (cfg->core_sparse_checkout_cone) {
		ਬਣਤਰ strbuf ਸਤਰ = STRBUF_INIT;

		hashmap_init(&pl->recursive_hashmap, pl_hashmap_cmp, ਨਿਰਾ, 0);
		hashmap_init(&pl->ਮਾਪਾ_hashmap, pl_hashmap_cmp, ਨਿਰਾ, 0);
		pl->use_cone_patterns = 1;

		ਜੇ (file) {
			ਬਣਤਰ strbuf unquoted = STRBUF_INIT;
			ਜਦੋਂ (!strbuf_getline(&ਸਤਰ, file)) {
				ਜੇ (ਸਤਰ.buf[0] == '"') {
					strbuf_reset(&unquoted);
					ਜੇ (unquote_c_style(&unquoted, ਸਤਰ.buf, ਨਿਰਾ))
						die(_("unable to unquote C-style string '%s'"),
						ਸਤਰ.buf);

					strbuf_ਅਦਲਾ_ਬਦਲੀ(&unquoted, &ਸਤਰ);
				}

				strbuf_ਮੰਜ਼ਿਲ_cone_ਨਮੂਨਾ(&ਸਤਰ, pl);
			}

			strbuf_release(&unquoted);
		} ਨਹੀਂਤਾਂ {
			ਲਈ (i = 0; i < argc; i++) {
				strbuf_setlen(&ਸਤਰ, 0);
				strbuf_addstr(&ਸਤਰ, argv[i]);
				strbuf_ਮੰਜ਼ਿਲ_cone_ਨਮੂਨਾ(&ਸਤਰ, pl);
			}
		}
		strbuf_release(&ਸਤਰ);
	} ਨਹੀਂਤਾਂ {
		ਜੇ (file) {
			ਬਣਤਰ strbuf ਸਤਰ = STRBUF_INIT;

			ਜਦੋਂ (!strbuf_getline(&ਸਤਰ, file))
				ਜੋੜੋ_ਨਮੂਨਾ(ਸਤਰ.buf, ਖਾਲੀ_ਥਾਵਾਂ_ਅਧਾਰ, 0, pl, 0);

			strbuf_release(&ਸਤਰ);
		} ਨਹੀਂਤਾਂ {
			ਲਈ (i = 0; i < argc; i++)
				ਜੋੜੋ_ਨਮੂਨਾ(argv[i], ਖਾਲੀ_ਥਾਵਾਂ_ਅਧਾਰ, 0, pl, 0);
		}
	}
}

ਗਿਣਤੀ modify_type {
	REPLACE,
	ਜੋੜੋ_2,
};

ਸਥਿਰ ਖਾਲੀ ਜੋੜੋ_patterns_cone_ਬਹੁਲਕ(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv,
				   ਬਣਤਰ ਨਮੂਨਾ_list *pl,
				   ਪੂਰਨ use_stdin)
{
	ਬਣਤਰ strbuf ਬਫਰ = STRBUF_INIT;
	ਬਣਤਰ ਨਮੂਨਾ_ਇੰਦਰਾਜ *pe;
	ਬਣਤਰ hashmap_iter iter;
	ਬਣਤਰ ਨਮੂਨਾ_list existing;
	ਬਣਤਰ repo_config_ਮੁੱਲਾਂ *cfg = repo_config_ਮੁੱਲਾਂ(the_repository);
	ਅੱਖਰ *sparse_filename = get_sparse_checkout_filename();

	ਜੋੜੋ_patterns_from_ਨਿਵੇਸ਼(pl, argc, argv,
				use_stdin ? stdin : ਨਿਰਾ);

	memset(&existing, 0, ਆਕਾਰ(existing));
	existing.use_cone_patterns = cfg->core_sparse_checkout_cone;

	ਜੇ (ਜੋੜੋ_patterns_from_file_ਮੰਜ਼ਿਲ_list(sparse_filename, "", 0,
					   &existing, ਨਿਰਾ, 0))
		die(_("unable to load existing sparse-checkout patterns"));
	ਮੁਕਤ(sparse_filename);

	ਜੇ (!existing.use_cone_patterns)
		die(_("existing sparse-checkout patterns do not use cone mode"));

	hashmap_for_each_ਇੰਦਰਾਜ(&existing.recursive_hashmap, &iter, pe, ent) {
		ਜੇ (!hashmap_ਸ਼ਾਮਲ_ਹੈ_ਮਾਪਾ(&pl->recursive_hashmap,
					pe->ਨਮੂਨਾ, &ਬਫਰ) ||
		    !hashmap_ਸ਼ਾਮਲ_ਹੈ_ਮਾਪਾ(&pl->ਮਾਪਾ_hashmap,
					pe->ਨਮੂਨਾ, &ਬਫਰ)) {
			strbuf_reset(&ਬਫਰ);
			strbuf_addstr(&ਬਫਰ, pe->ਨਮੂਨਾ);
			ਪਾਓ_recursive_ਨਮੂਨਾ(pl, &ਬਫਰ);
		}
	}

	clear_ਨਮੂਨਾ_list(&existing);
	strbuf_release(&ਬਫਰ);
}

ਸਥਿਰ ਖਾਲੀ ਜੋੜੋ_patterns_literal(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv,
				 ਬਣਤਰ ਨਮੂਨਾ_list *pl,
				 ਪੂਰਨ use_stdin)
{
	ਅੱਖਰ *sparse_filename = get_sparse_checkout_filename();
	ਜੇ (ਜੋੜੋ_patterns_from_file_ਮੰਜ਼ਿਲ_list(sparse_filename, "", 0,
					   pl, ਨਿਰਾ, 0))
		die(_("unable to load existing sparse-checkout patterns"));
	ਮੁਕਤ(sparse_filename);
	ਜੋੜੋ_patterns_from_ਨਿਵੇਸ਼(pl, argc, argv, use_stdin ? stdin : ਨਿਰਾ);
}

ਸਥਿਰ ਪੂਰਨ modify_ਨਮੂਨਾ_list(ਬਣਤਰ repository *repo,
			       ਬਣਤਰ strvec *ਦਲੀਲਾਂ, ਪੂਰਨ use_stdin,
			       ਗਿਣਤੀ modify_type m)
{
	ਪੂਰਨ ਨਤੀਜਾ;
	ਪੂਰਨ ਬਦਲਿਆ_config = 0;
	ਬਣਤਰ ਨਮੂਨਾ_list *pl = xcalloc(1, ਆਕਾਰ(*pl));
	ਬਣਤਰ repo_config_ਮੁੱਲਾਂ *cfg = repo_config_ਮੁੱਲਾਂ(the_repository);

	ਚੋਣ (m) {
	ਹਾਲਤ ਜੋੜੋ_2:
		ਜੇ (cfg->core_sparse_checkout_cone)
			ਜੋੜੋ_patterns_cone_ਬਹੁਲਕ(ਦਲੀਲਾਂ->nr, ਦਲੀਲਾਂ->v, pl, use_stdin);
		ਨਹੀਂਤਾਂ
			ਜੋੜੋ_patterns_literal(ਦਲੀਲਾਂ->nr, ਦਲੀਲਾਂ->v, pl, use_stdin);
		ਰੋਕੋ;

	ਹਾਲਤ REPLACE:
		ਜੋੜੋ_patterns_from_ਨਿਵੇਸ਼(pl, ਦਲੀਲਾਂ->nr, ਦਲੀਲਾਂ->v,
					use_stdin ? stdin : ਨਿਰਾ);
		ਰੋਕੋ;
	}

	ਜੇ (!cfg->apply_sparse_checkout) {
		ਸੈੱਟ_config(repo, ਬਹੁਲਕ_ALL_PATTERNS);
		cfg->apply_sparse_checkout = 1;
		ਬਦਲਿਆ_config = 1;
	}

	ਨਤੀਜਾ = write_patterns_and_update(repo, pl);

	ਜੇ (ਨਤੀਜਾ && ਬਦਲਿਆ_config)
		ਸੈੱਟ_config(repo, ਬਹੁਲਕ_NO_PATTERNS);

	clear_ਨਮੂਨਾ_list(pl);
	ਮੁਕਤ(pl);
	ਵਾਪਸ ਨਤੀਜਾ;
}

ਸਥਿਰ ਖਾਲੀ sanitize_paths(ਬਣਤਰ repository *repo,
			   ਬਣਤਰ strvec *ਦਲੀਲਾਂ,
			   ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ, ਪੂਰਨ skip_checks)
{
	ਪੂਰਨ i;
	ਬਣਤਰ repo_config_ਮੁੱਲਾਂ *cfg = repo_config_ਮੁੱਲਾਂ(the_repository);

	ਜੇ (!ਦਲੀਲਾਂ->nr)
		ਵਾਪਸ;

	ਜੇ (ਅਗੇਤਰ && *ਅਗੇਤਰ && cfg->core_sparse_checkout_cone) {
		/*
		 * The args are not pathspecs, so unfortunately we
		 * cannot imitate how cmd_add() uses parse_pathspec().
		 */
		ਪੂਰਨ ਅਗੇਤਰ_len = ਲੜੀ_ਲੰਬਾਈ(ਅਗੇਤਰ);

		ਲਈ (i = 0; i < ਦਲੀਲਾਂ->nr; i++) {
			ਅੱਖਰ *prefixed_ਰਸਤਾ = ਅਗੇਤਰ_ਰਸਤਾ(the_repository, ਅਗੇਤਰ,
							  ਅਗੇਤਰ_len, ਦਲੀਲਾਂ->v[i]);
			strvec_replace(ਦਲੀਲਾਂ, i, prefixed_ਰਸਤਾ);
			ਮੁਕਤ(prefixed_ਰਸਤਾ);
		}
	}

	ਜੇ (skip_checks)
		ਵਾਪਸ;

	ਜੇ (ਅਗੇਤਰ && *ਅਗੇਤਰ && !cfg->core_sparse_checkout_cone)
		die(_("please run from the toplevel directory in non-cone mode"));

	ਜੇ (cfg->core_sparse_checkout_cone) {
		ਲਈ (i = 0; i < ਦਲੀਲਾਂ->nr; i++) {
			ਜੇ (ਦਲੀਲਾਂ->v[i][0] == '/')
				die(_("specify directories rather than patterns (no leading slash)"));
			ਜੇ (ਦਲੀਲਾਂ->v[i][0] == '!')
				die(_("specify directories rather than patterns.  If your directory starts with a '!', pass --skip-checks"));
			ਜੇ (strpbrk(ਦਲੀਲਾਂ->v[i], "*?[]"))
				die(_("specify directories rather than patterns.  If your directory really has any of '*?[]\\' in it, pass --skip-checks"));
		}
	}

	ਲਈ (i = 0; i < ਦਲੀਲਾਂ->nr; i++) {
		ਬਣਤਰ cache_ਇੰਦਰਾਜ *ce;
		ਬਣਤਰ ਸੂਚਕਾਂਕ_state *ਸੂਚਕਾਂਕ = repo->ਸੂਚਕਾਂਕ;
		ਪੂਰਨ pos = ਸੂਚਕਾਂਕ_name_pos(ਸੂਚਕਾਂਕ, ਦਲੀਲਾਂ->v[i], ਲੜੀ_ਲੰਬਾਈ(ਦਲੀਲਾਂ->v[i]));

		ਜੇ (pos < 0)
			ਜਾਰੀ;
		ce = ਸੂਚਕਾਂਕ->cache[pos];
		ਜੇ (S_ISSPARSEDIR(ce->ce_ਬਹੁਲਕ))
			ਜਾਰੀ;

		ਜੇ (cfg->core_sparse_checkout_cone)
			die(_("'%s' is not a directory; to treat it as a directory anyway, rerun with --skip-checks"), ਦਲੀਲਾਂ->v[i]);
		ਨਹੀਂਤਾਂ
			warning(_("pass a leading slash before paths such as '%s' if you want a single file (see NON-CONE PROBLEMS in the git-sparse-checkout manual)."), ਦਲੀਲਾਂ->v[i]);
	}
}

ਸਥਿਰ ਅੱਖਰ ਅਚਲ * ਅਚਲ builtin_sparse_checkout_ਜੋੜੋ_usage[] = {
	N_("git sparse-checkout add [--skip-checks] (--stdin | <patterns>)"),
	ਨਿਰਾ
};

ਸਥਿਰ ਬਣਤਰ sparse_checkout_ਜੋੜੋ_opts {
	ਪੂਰਨ skip_checks;
	ਪੂਰਨ use_stdin;
} ਜੋੜੋ_opts;

ਸਥਿਰ ਪੂਰਨ sparse_checkout_ਜੋੜੋ(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			       ਬਣਤਰ repository *repo)
{
	ਸਥਿਰ ਬਣਤਰ option builtin_sparse_checkout_ਜੋੜੋ_options[] = {
		OPT_BOOL_F(0, "skip-checks", &ਜੋੜੋ_opts.skip_checks,
			   N_("skip some sanity checks on the given paths that might give false positives"),
			   PARSE_OPT_NONEG),
		OPT_BOOL(0, "stdin", &ਜੋੜੋ_opts.use_stdin,
			 N_("read patterns from standard in")),
		OPT_ਸਿਰਾ(),
	};
	ਬਣਤਰ strvec patterns = STRVEC_INIT;
	ਪੂਰਨ ret;
	ਬਣਤਰ repo_config_ਮੁੱਲਾਂ *cfg = repo_config_ਮੁੱਲਾਂ(the_repository);

	ਤਿਆਰੀ_work_ਰੁੱਖ(the_repository);
	ਜੇ (!cfg->apply_sparse_checkout)
		die(_("no sparse-checkout to add to"));

	repo_read_ਸੂਚਕਾਂਕ(repo);

	argc = parse_options(argc, argv, ਅਗੇਤਰ,
			     builtin_sparse_checkout_ਜੋੜੋ_options,
			     builtin_sparse_checkout_ਜੋੜੋ_usage, 0);

	ਲਈ (ਪੂਰਨ i = 0; i < argc; i++)
		strvec_ਧੱਕੋ(&patterns, argv[i]);
	sanitize_paths(repo, &patterns, ਅਗੇਤਰ, ਜੋੜੋ_opts.skip_checks);

	ret = modify_ਨਮੂਨਾ_list(repo, &patterns, ਜੋੜੋ_opts.use_stdin, ਜੋੜੋ_2);

	strvec_clear(&patterns);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਅੱਖਰ ਅਚਲ * ਅਚਲ builtin_sparse_checkout_ਸੈੱਟ_usage[] = {
	N_("git sparse-checkout set [--[no-]cone] [--[no-]sparse-index] [--skip-checks] (--stdin | <patterns>)"),
	ਨਿਰਾ
};

ਸਥਿਰ ਬਣਤਰ sparse_checkout_ਸੈੱਟ_opts {
	ਪੂਰਨ cone_ਬਹੁਲਕ;
	ਪੂਰਨ sparse_ਸੂਚਕਾਂਕ;
	ਪੂਰਨ skip_checks;
	ਪੂਰਨ use_stdin;
} ਸੈੱਟ_opts;

ਸਥਿਰ ਪੂਰਨ sparse_checkout_ਸੈੱਟ(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			       ਬਣਤਰ repository *repo)
{
	ਬਣਤਰ repo_config_ਮੁੱਲਾਂ *cfg = repo_config_ਮੁੱਲਾਂ(the_repository);
	ਪੂਰਨ default_patterns_nr = 2;
	ਅਚਲ ਅੱਖਰ *default_patterns[] = {"/*", "!/*/", ਨਿਰਾ};

	ਸਥਿਰ ਬਣਤਰ option builtin_sparse_checkout_ਸੈੱਟ_options[] = {
		OPT_BOOL(0, "cone", &ਸੈੱਟ_opts.cone_ਬਹੁਲਕ,
			 N_("initialize the sparse-checkout in cone mode")),
		OPT_BOOL(0, "sparse-index", &ਸੈੱਟ_opts.sparse_ਸੂਚਕਾਂਕ,
			 N_("toggle the use of a sparse index")),
		OPT_BOOL_F(0, "skip-checks", &ਸੈੱਟ_opts.skip_checks,
			   N_("skip some sanity checks on the given paths that might give false positives"),
			   PARSE_OPT_NONEG),
		OPT_BOOL_F(0, "stdin", &ਸੈੱਟ_opts.use_stdin,
			   N_("read patterns from standard in"),
			   PARSE_OPT_NONEG),
		OPT_ਸਿਰਾ(),
	};
	ਬਣਤਰ strvec patterns = STRVEC_INIT;
	ਪੂਰਨ ret;

	ਤਿਆਰੀ_work_ਰੁੱਖ(the_repository);
	repo_read_ਸੂਚਕਾਂਕ(repo);

	ਸੈੱਟ_opts.cone_ਬਹੁਲਕ = -1;
	ਸੈੱਟ_opts.sparse_ਸੂਚਕਾਂਕ = -1;

	argc = parse_options(argc, argv, ਅਗੇਤਰ,
			     builtin_sparse_checkout_ਸੈੱਟ_options,
			     builtin_sparse_checkout_ਸੈੱਟ_usage, 0);

	ਜੇ (update_modes(repo, &ਸੈੱਟ_opts.cone_ਬਹੁਲਕ, &ਸੈੱਟ_opts.sparse_ਸੂਚਕਾਂਕ))
		ਵਾਪਸ 1;

	/*
	 * Cone mode automatically specifies the toplevel directory.  For
	 * non-cone mode, if nothing is specified, manually select just the
	 * top-level directory (much as 'init' would do).
	 */
	ਜੇ (!cfg->core_sparse_checkout_cone && !ਸੈੱਟ_opts.use_stdin && argc == 0) {
		ਲਈ (ਪੂਰਨ i = 0; i < default_patterns_nr; i++)
			strvec_ਧੱਕੋ(&patterns, default_patterns[i]);
	} ਨਹੀਂਤਾਂ {
		ਲਈ (ਪੂਰਨ i = 0; i < argc; i++)
			strvec_ਧੱਕੋ(&patterns, argv[i]);
		sanitize_paths(repo, &patterns, ਅਗੇਤਰ, ਸੈੱਟ_opts.skip_checks);
	}

	ret = modify_ਨਮੂਨਾ_list(repo, &patterns, ਸੈੱਟ_opts.use_stdin, REPLACE);

	strvec_clear(&patterns);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਅੱਖਰ ਅਚਲ * ਅਚਲ builtin_sparse_checkout_reapply_usage[] = {
	"git sparse-checkout reapply [--[no-]cone] [--[no-]sparse-index]",
	ਨਿਰਾ
};

ਸਥਿਰ ਬਣਤਰ sparse_checkout_reapply_opts {
	ਪੂਰਨ cone_ਬਹੁਲਕ;
	ਪੂਰਨ sparse_ਸੂਚਕਾਂਕ;
} reapply_opts;

ਸਥਿਰ ਪੂਰਨ sparse_checkout_reapply(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv,
				   ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
				   ਬਣਤਰ repository *repo)
{
	ਸਥਿਰ ਬਣਤਰ option builtin_sparse_checkout_reapply_options[] = {
		OPT_BOOL(0, "cone", &reapply_opts.cone_ਬਹੁਲਕ,
			 N_("initialize the sparse-checkout in cone mode")),
		OPT_BOOL(0, "sparse-index", &reapply_opts.sparse_ਸੂਚਕਾਂਕ,
			 N_("toggle the use of a sparse index")),
		OPT_ਸਿਰਾ(),
	};
	ਬਣਤਰ repo_config_ਮੁੱਲਾਂ *cfg = repo_config_ਮੁੱਲਾਂ(the_repository);

	ਤਿਆਰੀ_work_ਰੁੱਖ(the_repository);
	ਜੇ (!cfg->apply_sparse_checkout)
		die(_("must be in a sparse-checkout to reapply sparsity patterns"));

	reapply_opts.cone_ਬਹੁਲਕ = -1;
	reapply_opts.sparse_ਸੂਚਕਾਂਕ = -1;

	argc = parse_options(argc, argv, ਅਗੇਤਰ,
			     builtin_sparse_checkout_reapply_options,
			     builtin_sparse_checkout_reapply_usage, 0);

	repo_read_ਸੂਚਕਾਂਕ(repo);

	ਜੇ (update_modes(repo, &reapply_opts.cone_ਬਹੁਲਕ, &reapply_opts.sparse_ਸੂਚਕਾਂਕ))
		ਵਾਪਸ 1;

	ਵਾਪਸ update_working_directory(repo, ਨਿਰਾ);
}

ਸਥਿਰ ਅੱਖਰ ਅਚਲ * ਅਚਲ builtin_sparse_checkout_clean_usage[] = {
	"git sparse-checkout clean [-n|--dry-run]",
	ਨਿਰਾ
};

ਸਥਿਰ ਪੂਰਨ list_file_iterator(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ)
{
	ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ = ਅੰਕੜੇ;

	ਲਿਖੋ(ਸੁਨੇਹਾ, ਰਸਤਾ);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ list_every_file_ਵਿੱਚ_dir(ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ,
				   ਅਚਲ ਅੱਖਰ *directory)
{
	ਬਣਤਰ strbuf ਰਸਤਾ = STRBUF_INIT;

	strbuf_addstr(&ਰਸਤਾ, directory);
	for_each_file_ਵਿੱਚ_dir(&ਰਸਤਾ, list_file_iterator, ਸੁਨੇਹਾ);
	strbuf_release(&ਰਸਤਾ);
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ_remove = N_("Removing %s\n");
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ_would_remove = N_("Would remove %s\n");

ਸਥਿਰ ਪੂਰਨ sparse_checkout_clean(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv,
				   ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
				   ਬਣਤਰ repository *repo)
{
	ਬਣਤਰ strbuf ਭਰਿਆ_ਰਸਤਾ = STRBUF_INIT;
	ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ = ਸੁਨੇਹਾ_remove;
	ਮਾਪ_t worktree_len;
	ਪੂਰਨ force = 0, dry_ਲੜੀਵਾਰ = 0, verbose = 0;
	ਪੂਰਨ require_force = 1;
	ਬਣਤਰ repo_config_ਮੁੱਲਾਂ *cfg = repo_config_ਮੁੱਲਾਂ(the_repository);

	ਬਣਤਰ option builtin_sparse_checkout_clean_options[] = {
		OPT_DRY_ਲੜੀਵਾਰ(&dry_ਲੜੀਵਾਰ, N_("dry run")),
		OPT__FORCE(&force, N_("force"), PARSE_OPT_NOCOMPLETE),
		OPT__VERBOSE(&verbose, N_("report each affected file, not just directories")),
		OPT_ਸਿਰਾ(),
	};

	ਤਿਆਰੀ_work_ਰੁੱਖ(the_repository);
	ਜੇ (!cfg->apply_sparse_checkout)
		die(_("must be in a sparse-checkout to clean directories"));
	ਜੇ (!cfg->core_sparse_checkout_cone)
		die(_("must be in a cone-mode sparse-checkout to clean directories"));

	argc = parse_options(argc, argv, ਅਗੇਤਰ,
			     builtin_sparse_checkout_clean_options,
			     builtin_sparse_checkout_clean_usage, 0);

	repo_config_get_bool(repo, "clean.requireforce", &require_force);
	ਜੇ (require_force && !force && !dry_ਲੜੀਵਾਰ)
		die(_("for safety, refusing to clean without one of --force or --dry-run"));

	ਜੇ (dry_ਲੜੀਵਾਰ)
		ਸੁਨੇਹਾ = ਸੁਨੇਹਾ_would_remove;

	ਜੇ (repo_read_ਸੂਚਕਾਂਕ(repo) < 0)
		die(_("failed to read index"));

	ਜੇ (convert_ਮੰਜ਼ਿਲ_sparse(repo->ਸੂਚਕਾਂਕ, SPARSE_ਸੂਚਕਾਂਕ_ਸਮ੍ਰਿਤੀ_ONLY) ||
	    repo->ਸੂਚਕਾਂਕ->sparse_ਸੂਚਕਾਂਕ == ਸੂਚਕਾਂਕ_EXPANDED)
		die(_("failed to convert index to a sparse index; resolve merge conflicts and try again"));

	strbuf_addstr(&ਭਰਿਆ_ਰਸਤਾ, repo->worktree);
	strbuf_addch(&ਭਰਿਆ_ਰਸਤਾ, '/');
	worktree_len = ਭਰਿਆ_ਰਸਤਾ.len;

	ਲਈ (ਮਾਪ_t i = 0; i < repo->ਸੂਚਕਾਂਕ->cache_nr; i++) {
		ਬਣਤਰ cache_ਇੰਦਰਾਜ *ce = repo->ਸੂਚਕਾਂਕ->cache[i];
		ਜੇ (!S_ISSPARSEDIR(ce->ce_ਬਹੁਲਕ))
			ਜਾਰੀ;
		strbuf_setlen(&ਭਰਿਆ_ਰਸਤਾ, worktree_len);
		strbuf_ਜੋੜੋ(&ਭਰਿਆ_ਰਸਤਾ, ce->name, ce->ce_namelen);

		ਜੇ (!is_directory(ਭਰਿਆ_ਰਸਤਾ.buf))
			ਜਾਰੀ;

		ਜੇ (verbose)
			list_every_file_ਵਿੱਚ_dir(ਸੁਨੇਹਾ, ce->name);
		ਨਹੀਂਤਾਂ
			ਲਿਖੋ(ਸੁਨੇਹਾ, ce->name);

		ਜੇ (dry_ਲੜੀਵਾਰ <= 0 &&
		    remove_dir_recursively(&ਭਰਿਆ_ਰਸਤਾ, 0))
			warning_errno(_("failed to remove '%s'"), ce->name);
	}

	strbuf_release(&ਭਰਿਆ_ਰਸਤਾ);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਅੱਖਰ ਅਚਲ * ਅਚਲ builtin_sparse_checkout_disable_usage[] = {
	"git sparse-checkout disable",
	ਨਿਰਾ
};

ਸਥਿਰ ਪੂਰਨ sparse_checkout_disable(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv,
				   ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
				   ਬਣਤਰ repository *repo)
{
	ਸਥਿਰ ਬਣਤਰ option builtin_sparse_checkout_disable_options[] = {
		OPT_ਸਿਰਾ(),
	};
	ਬਣਤਰ ਨਮੂਨਾ_list pl;
	ਬਣਤਰ repo_config_ਮੁੱਲਾਂ *cfg = repo_config_ਮੁੱਲਾਂ(the_repository);

	/*
	 * We do not exit early if !repo->config_values.apply_sparse_checkout; due to the
	 * ability for users to manually muck things up between
	 *   direct editing of .git/info/sparse-checkout
	 *   running read-tree -m u HEAD or update-index --skip-worktree
	 *   direct toggling of config options
	 * users might end up with an index with SKIP_WORKTREE bit set on
	 * some files and not know how to undo it.  So, here we just
	 * forcibly return to a dense checkout regardless of initial state.
	 */

	ਤਿਆਰੀ_work_ਰੁੱਖ(the_repository);
	argc = parse_options(argc, argv, ਅਗੇਤਰ,
			     builtin_sparse_checkout_disable_options,
			     builtin_sparse_checkout_disable_usage, 0);

	/*
	 * Disable the advice message for expanding a sparse index, as we
	 * are expecting to do that when disabling sparse-checkout.
	 */
	give_advice_on_expansion = 0;
	repo_read_ਸੂਚਕਾਂਕ(repo);

	memset(&pl, 0, ਆਕਾਰ(pl));
	hashmap_init(&pl.recursive_hashmap, pl_hashmap_cmp, ਨਿਰਾ, 0);
	hashmap_init(&pl.ਮਾਪਾ_hashmap, pl_hashmap_cmp, ਨਿਰਾ, 0);
	pl.use_cone_patterns = 0;
	cfg->apply_sparse_checkout = 1;

	ਜੋੜੋ_ਨਮੂਨਾ("/*", ਖਾਲੀ_ਥਾਵਾਂ_ਅਧਾਰ, 0, &pl, 0);

	prepare_repo_settings(the_repository);
	repo->settings.sparse_ਸੂਚਕਾਂਕ = 0;

	ਜੇ (update_working_directory(repo, &pl))
		die(_("error while refreshing working directory"));

	clear_ਨਮੂਨਾ_list(&pl);
	ਵਾਪਸ ਸੈੱਟ_config(repo, ਬਹੁਲਕ_NO_PATTERNS);
}

ਸਥਿਰ ਅੱਖਰ ਅਚਲ * ਅਚਲ builtin_sparse_checkout_check_ਨਿਯਮਾਂ_usage[] = {
	N_("git sparse-checkout check-rules [-z] [--skip-checks]"
	   "[--[no-]cone] [--rules-file <file>]"),
	ਨਿਰਾ
};

ਸਥਿਰ ਬਣਤਰ sparse_checkout_check_ਨਿਯਮਾਂ_opts {
	ਪੂਰਨ cone_ਬਹੁਲਕ;
	ਪੂਰਨ null_termination;
	ਅੱਖਰ *ਨਿਯਮਾਂ_file;
} check_ਨਿਯਮਾਂ_opts;

ਸਥਿਰ ਪੂਰਨ check_ਨਿਯਮਾਂ(ਬਣਤਰ repository *repo,
		       ਬਣਤਰ ਨਮੂਨਾ_list *pl,
		       ਪੂਰਨ null_terminated)
{
	ਬਣਤਰ strbuf ਸਤਰ = STRBUF_INIT;
	ਬਣਤਰ strbuf unquoted = STRBUF_INIT;
	ਅੱਖਰ *ਰਸਤਾ;
	ਪੂਰਨ ਸਤਰ_terminator = null_terminated ? 0 : '\n';
	strbuf_getline_fn getline_fn = null_terminated ? strbuf_getline_nul
		: strbuf_getline;
	repo->ਸੂਚਕਾਂਕ->sparse_checkout_patterns = pl;
	ਜਦੋਂ (!getline_fn(&ਸਤਰ, stdin)) {
		ਰਸਤਾ = ਸਤਰ.buf;
		ਜੇ (!null_terminated && ਸਤਰ.buf[0] == '"') {
			strbuf_reset(&unquoted);
			ਜੇ (unquote_c_style(&unquoted, ਸਤਰ.buf, ਨਿਰਾ))
				die(_("unable to unquote C-style string '%s'"),
					ਸਤਰ.buf);

			ਰਸਤਾ = unquoted.buf;
		}

		ਜੇ (ਰਸਤਾ_ਵਿੱਚ_sparse_checkout(ਰਸਤਾ, repo->ਸੂਚਕਾਂਕ))
			write_name_quoted(ਰਸਤਾ, stdout, ਸਤਰ_terminator);
	}
	strbuf_release(&ਸਤਰ);
	strbuf_release(&unquoted);

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ sparse_checkout_check_ਨਿਯਮਾਂ(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
				       ਬਣਤਰ repository *repo)
{
	ਸਥਿਰ ਬਣਤਰ option builtin_sparse_checkout_check_ਨਿਯਮਾਂ_options[] = {
		OPT_BOOL('z', ਨਿਰਾ, &check_ਨਿਯਮਾਂ_opts.null_termination,
			 N_("terminate input and output files by a NUL character")),
		OPT_BOOL(0, "cone", &check_ਨਿਯਮਾਂ_opts.cone_ਬਹੁਲਕ,
			 N_("when used with --rules-file interpret patterns as cone mode patterns")),
		OPT_FILENAME(0, "rules-file", &check_ਨਿਯਮਾਂ_opts.ਨਿਯਮਾਂ_file,
			 N_("use patterns in <file> instead of the current ones.")),
		OPT_ਸਿਰਾ(),
	};

	FILE *fp;
	ਪੂਰਨ ret;
	ਬਣਤਰ ਨਮੂਨਾ_list pl = {0};
	ਬਣਤਰ repo_config_ਮੁੱਲਾਂ *cfg = repo_config_ਮੁੱਲਾਂ(the_repository);
	ਅੱਖਰ *sparse_filename;
	check_ਨਿਯਮਾਂ_opts.cone_ਬਹੁਲਕ = -1;

	argc = parse_options(argc, argv, ਅਗੇਤਰ,
			     builtin_sparse_checkout_check_ਨਿਯਮਾਂ_options,
			     builtin_sparse_checkout_check_ਨਿਯਮਾਂ_usage, 0);

	ਜੇ (check_ਨਿਯਮਾਂ_opts.ਨਿਯਮਾਂ_file && check_ਨਿਯਮਾਂ_opts.cone_ਬਹੁਲਕ < 0)
		check_ਨਿਯਮਾਂ_opts.cone_ਬਹੁਲਕ = 1;

	update_cone_ਬਹੁਲਕ(&check_ਨਿਯਮਾਂ_opts.cone_ਬਹੁਲਕ);
	pl.use_cone_patterns = cfg->core_sparse_checkout_cone;
	ਜੇ (check_ਨਿਯਮਾਂ_opts.ਨਿਯਮਾਂ_file) {
		fp = xfopen(check_ਨਿਯਮਾਂ_opts.ਨਿਯਮਾਂ_file, "r");
		ਜੋੜੋ_patterns_from_ਨਿਵੇਸ਼(&pl, argc, argv, fp);
		fclose(fp);
	} ਨਹੀਂਤਾਂ {
		sparse_filename = get_sparse_checkout_filename();
		ਜੇ (ਜੋੜੋ_patterns_from_file_ਮੰਜ਼ਿਲ_list(sparse_filename, "", 0, &pl,
						   ਨਿਰਾ, 0))
			die(_("unable to load existing sparse-checkout patterns"));
		ਮੁਕਤ(sparse_filename);
	}

	ret = check_ਨਿਯਮਾਂ(repo, &pl, check_ਨਿਯਮਾਂ_opts.null_termination);
	clear_ਨਮੂਨਾ_list(&pl);
	ਮੁਕਤ(check_ਨਿਯਮਾਂ_opts.ਨਿਯਮਾਂ_file);
	ਵਾਪਸ ret;
}

ਪੂਰਨ cmd_sparse_checkout(ਪੂਰਨ argc,
			ਅਚਲ ਅੱਖਰ **argv,
			ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			ਬਣਤਰ repository *repo)
{
	parse_opt_subcommand_fn *fn = ਨਿਰਾ;
	ਬਣਤਰ option builtin_sparse_checkout_options[] = {
		OPT_SUBCOMMAND("list", &fn, sparse_checkout_list),
		OPT_SUBCOMMAND("init", &fn, sparse_checkout_init),
		OPT_SUBCOMMAND("set", &fn, sparse_checkout_ਸੈੱਟ),
		OPT_SUBCOMMAND("add", &fn, sparse_checkout_ਜੋੜੋ),
		OPT_SUBCOMMAND("reapply", &fn, sparse_checkout_reapply),
		OPT_SUBCOMMAND("clean", &fn, sparse_checkout_clean),
		OPT_SUBCOMMAND("disable", &fn, sparse_checkout_disable),
		OPT_SUBCOMMAND("check-rules", &fn, sparse_checkout_check_ਨਿਯਮਾਂ),
		OPT_ਸਿਰਾ(),
	};

	argc = parse_options(argc, argv, ਅਗੇਤਰ,
			     builtin_sparse_checkout_options,
			     builtin_sparse_checkout_usage, 0);

	repo_config(the_repository, git_default_config, ਨਿਰਾ);

	prepare_repo_settings(repo);
	repo->settings.command_requires_ਭਰਿਆ_ਸੂਚਕਾਂਕ = 0;

	ਵਾਪਸ fn(argc, argv, ਅਗੇਤਰ, repo);
}
