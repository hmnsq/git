#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE
#ವ್ಯಾಖ್ಯಾನಿಸು DISABLE_SIGN_COMPARE_WARNINGS

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "abspath.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "dir.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "object-file.h"
#ಅನ್ತರ್ಭಾವ "object-name.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "path.h"
#ಅನ್ತರ್ಭಾವ "pathspec.h"
#ಅನ್ತರ್ಭಾವ "strbuf.h"
#ಅನ್ತರ್ಭಾವ "string-list.h"
#ಅನ್ತರ್ಭಾವ "lockfile.h"
#ಅನ್ತರ್ಭಾವ "unpack-trees.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "setup.h"
#ಅನ್ತರ್ಭಾವ "sparse-index.h"
#ಅನ್ತರ್ಭಾವ "worktree.h"

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಖಾಲಿಗಳು_ತಳ = "";

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ * ಸ್ಥಿರಾಂಕ builtin_sparse_checkout_usage[] = {
	N_("git sparse-checkout (init | list | set | add | reapply | disable | check-rules | clean) [<options>]"),
	ಖಾಲಿ
};

ಸ್ಥಿರ ಶೂನ್ಯ write_patterns_ಗಮ್ಯ_file(FILE *fp, ರಚನೆ ನಮೂನೆ_list *pl)
{
	ಪೂರ್ಣಾಂಕ i;

	ಗಾಗಿ (i = 0; i < pl->nr; i++) {
		ರಚನೆ ಮಾರ್ಗ_ನಮೂನೆ *p = pl->patterns[i];

		ಆದರೆ (p->flags & ನಮೂನೆ_FLAG_NEGATIVE)
			fprintf(fp, "!");

		fprintf(fp, "%s", p->ನಮೂನೆ);

		ಆದರೆ (p->flags & ನಮೂನೆ_FLAG_MUSTBEDIR)
			fprintf(fp, "/");

		fprintf(fp, "\n");
	}
}

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ * ಸ್ಥಿರಾಂಕ builtin_sparse_checkout_list_usage[] = {
	"git sparse-checkout list",
	ಖಾಲಿ
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sparse_checkout_list(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
				ರಚನೆ repository *repo ಬಳಸದ)
{
	ಸ್ಥಿರ ರಚನೆ option builtin_sparse_checkout_list_options[] = {
		OPT_ಕೊನೆ(),
	};
	ರಚನೆ ನಮೂನೆ_list pl;
	ಅಕ್ಷರ *sparse_filename;
	ಪೂರ್ಣಾಂಕ res;
	ರಚನೆ repo_config_ಮೌಲ್ಯಗಳು *cfg = repo_config_ಮೌಲ್ಯಗಳು(the_repository);

	ಸಿದ್ಧತೆ_work_ವೃಕ್ಷ(the_repository);
	ಆದರೆ (!cfg->apply_sparse_checkout)
		die(_("this worktree is not sparse"));

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ,
			     builtin_sparse_checkout_list_options,
			     builtin_sparse_checkout_list_usage, 0);

	memset(&pl, 0, ಗಾತ್ರ(pl));

	pl.use_cone_patterns = cfg->core_sparse_checkout_cone;

	sparse_filename = get_sparse_checkout_filename();
	res = ಕೂಡಿಸು_patterns_from_file_ಗಮ್ಯ_list(sparse_filename, "", 0, &pl, ಖಾಲಿ, 0);
	ಬಿಡುಗಡೆ(sparse_filename);

	ಆದರೆ (res < 0) {
		warning(_("this worktree is not sparse (sparse-checkout file may not exist)"));
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (pl.use_cone_patterns) {
		ಪೂರ್ಣಾಂಕ i;
		ರಚನೆ ನಮೂನೆ_ನಮೂದು *pe;
		ರಚನೆ hashmap_iter iter;
		ರಚನೆ string_list sl = STRING_LIST_INIT_DUP;

		hashmap_for_each_ನಮೂದು(&pl.recursive_hashmap, &iter, pe, ent) {
			/* pe->pattern starts with "/", skip it */
			string_list_append(&sl, pe->ನಮೂನೆ + 1);
		}

		string_list_ವಿಂಗಡಿಸು_u(&sl, 0);

		ಗಾಗಿ (i = 0; i < sl.nr; i++) {
			quote_c_style(sl.ವಸ್ತುಗಳು[i].string, ಖಾಲಿ, stdout, 0);
			ಬರೆ("\n");
		}

		string_list_clear(&sl, 0);
	} ಇಲ್ಲದಿದ್ದರೆ {
		write_patterns_ಗಮ್ಯ_file(stdout, &pl);
	}

	clear_ನಮೂನೆ_list(&pl);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ clean_tracked_sparse_directories(ರಚನೆ repository *r)
{
	ಪೂರ್ಣಾಂಕ i, was_ತುಂಬಿದ = 0;
	ರಚನೆ strbuf ಮಾರ್ಗ = STRBUF_INIT;
	ಗಾತ್ರ_t pathlen;
	ರಚನೆ string_list_ಅಂಶ *ಅಂಶ;
	ರಚನೆ string_list sparse_dirs = STRING_LIST_INIT_DUP;

	/*
	 * If we are not using cone mode patterns, then we cannot
	 * delete directories outside of the sparse cone.
	 */
	ಆದರೆ (!r || !r->ಸೂಚ್ಯಂಕ || !r->worktree)
		ಹಿಂತಿರುಗಿ;
	ಆದರೆ (init_sparse_checkout_patterns(r->ಸೂಚ್ಯಂಕ) ||
	    !r->ಸೂಚ್ಯಂಕ->sparse_checkout_patterns->use_cone_patterns)
		ಹಿಂತಿರುಗಿ;

	/*
	 * Use the sparse index as a data structure to assist finding
	 * directories that are safe to delete. This conversion to a
	 * sparse index will not delete directories that contain
	 * conflicted entries or submodules.
	 */
	ಆದರೆ (r->ಸೂಚ್ಯಂಕ->sparse_ಸೂಚ್ಯಂಕ == ಸೂಚ್ಯಂಕ_EXPANDED) {
		/*
		 * If something, such as a merge conflict or other concern,
		 * prevents us from converting to a sparse index, then do
		 * not try deleting files.
		 */
		ಆದರೆ (convert_ಗಮ್ಯ_sparse(r->ಸೂಚ್ಯಂಕ, SPARSE_ಸೂಚ್ಯಂಕ_ಸ್ಮರಣೆ_ONLY))
			ಹಿಂತಿರುಗಿ;
		was_ತುಂಬಿದ = 1;
	}

	strbuf_addstr(&ಮಾರ್ಗ, r->worktree);
	strbuf_complete(&ಮಾರ್ಗ, '/');
	pathlen = ಮಾರ್ಗ.len;

	/*
	 * Collect directories that have gone out of scope but also
	 * exist on disk, so there is some work to be done. We need to
	 * store the entries in a list before exploring, since that might
	 * expand the sparse-index again.
	 */
	ಗಾಗಿ (i = 0; i < r->ಸೂಚ್ಯಂಕ->cache_nr; i++) {
		ರಚನೆ cache_ನಮೂದು *ce = r->ಸೂಚ್ಯಂಕ->cache[i];

		ಆದರೆ (S_ISSPARSEDIR(ce->ce_ಬಹುಲಕ) &&
		    repo_file_exists(r, ce->name))
			string_list_append(&sparse_dirs, ce->name);
	}

	for_each_string_list_ಅಂಶ(ಅಂಶ, &sparse_dirs) {
		ರಚನೆ dir_struct dir = DIR_INIT;
		ರಚನೆ pathspec p = { 0 };
		ರಚನೆ strvec s = STRVEC_INIT;

		strbuf_setlen(&ಮಾರ್ಗ, pathlen);
		strbuf_addstr(&ಮಾರ್ಗ, ಅಂಶ->string);

		dir.flags |= DIR_SHOW_IGNORED_TOO;

		ಸಿದ್ಧತೆ_standard_excludes(&dir);
		strvec_ತಳ್ಳು(&s, ಮಾರ್ಗ.buf);

		parse_pathspec(&p, PATHSPEC_GLOB, 0, ಖಾಲಿ, s.v);
		fill_directory(&dir, r->ಸೂಚ್ಯಂಕ, &p);

		ಆದರೆ (dir.nr) {
			warning(_("directory '%s' contains untracked files,"
				  " but is not in the sparse-checkout cone"),
				ಅಂಶ->string);
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (remove_dir_recursively(&ಮಾರ್ಗ, 0)) {
			/*
			 * Removal is "best effort". If something blocks
			 * the deletion, then continue with a warning.
			 */
			warning(_("failed to remove directory '%s'"),
				ಅಂಶ->string);
		}

		strvec_clear(&s);
		clear_pathspec(&p);
		dir_clear(&dir);
	}

	string_list_clear(&sparse_dirs, 0);
	strbuf_release(&ಮಾರ್ಗ);

	ಆದರೆ (was_ತುಂಬಿದ)
		ensure_ತುಂಬಿದ_ಸೂಚ್ಯಂಕ(r->ಸೂಚ್ಯಂಕ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ update_working_directory(ರಚನೆ repository *r,
				    ರಚನೆ ನಮೂನೆ_list *pl)
{
	ಎಣಿಕೆ update_sparsity_ಫಲಿತಾಂಶ ಫಲಿತಾಂಶ;
	ರಚನೆ unpack_trees_options o;
	ರಚನೆ ಬೀಗ_file ಬೀಗ_file = ಬೀಗ_INIT;
	ರಚನೆ ನಮೂನೆ_list *old_pl;

	/* If no branch has been checked out, there are no updates to make. */
	ಆದರೆ (is_ಸೂಚ್ಯಂಕ_unborn(r->ಸೂಚ್ಯಂಕ))
		ಹಿಂತಿರುಗಿ UPDATE_SPARSITY_SUCCESS;

	old_pl = r->ಸೂಚ್ಯಂಕ->sparse_checkout_patterns;
	r->ಸೂಚ್ಯಂಕ->sparse_checkout_patterns = pl;

	memset(&o, 0, ಗಾತ್ರ(o));
	o.verbose_update = isatty(2);
	o.update = 1;
	o.ತಲೆ_idx = -1;
	o.src_ಸೂಚ್ಯಂಕ = r->ಸೂಚ್ಯಂಕ;
	o.dst_ಸೂಚ್ಯಂಕ = r->ಸೂಚ್ಯಂಕ;
	o.skip_sparse_checkout = 0;

	ಸಿದ್ಧತೆ_work_ವೃಕ್ಷ(the_repository);

	repo_hold_locked_ಸೂಚ್ಯಂಕ(r, &ಬೀಗ_file, ಬೀಗ_DIE_ON_ERROR);

	ಸಿದ್ಧತೆ_unpack_trees_porcelain(&o, "sparse-checkout");
	ಫಲಿತಾಂಶ = update_sparsity(&o, pl);
	clear_unpack_trees_porcelain(&o);

	ಆದರೆ (ಫಲಿತಾಂಶ == UPDATE_SPARSITY_WARNINGS)
		/*
		 * We don't do any special handling of warnings from untracked
		 * files in the way or dirty entries that can't be removed.
		 */
		ಫಲಿತಾಂಶ = UPDATE_SPARSITY_SUCCESS;
	ಆದರೆ (ಫಲಿತಾಂಶ == UPDATE_SPARSITY_SUCCESS)
		write_locked_ಸೂಚ್ಯಂಕ(r->ಸೂಚ್ಯಂಕ, &ಬೀಗ_file, COMMIT_ಬೀಗ);
	ಇಲ್ಲದಿದ್ದರೆ
		rollback_ಬೀಗ_file(&ಬೀಗ_file);

	clean_tracked_sparse_directories(r);

	ಆದರೆ (r->ಸೂಚ್ಯಂಕ->sparse_checkout_patterns != pl) {
		clear_ನಮೂನೆ_list(r->ಸೂಚ್ಯಂಕ->sparse_checkout_patterns);
		FREE_AND_NULL(r->ಸೂಚ್ಯಂಕ->sparse_checkout_patterns);
	}
	r->ಸೂಚ್ಯಂಕ->sparse_checkout_patterns = old_pl;

	ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

ಸ್ಥಿರ ಅಕ್ಷರ *escaped_ನಮೂನೆ(ಅಕ್ಷರ *ನಮೂನೆ)
{
	ಅಕ್ಷರ *p = ನಮೂನೆ;
	ರಚನೆ strbuf final = STRBUF_INIT;

	ಆಗ (*p) {
		ಆದರೆ (is_glob_special(*p))
			strbuf_addch(&final, '\\');

		strbuf_addch(&final, *p);
		p++;
	}

	ಹಿಂತಿರುಗಿ strbuf_detach(&final, ಖಾಲಿ);
}

ಸ್ಥಿರ ಶೂನ್ಯ write_cone_ಗಮ್ಯ_file(FILE *fp, ರಚನೆ ನಮೂನೆ_list *pl)
{
	ಪೂರ್ಣಾಂಕ i;
	ರಚನೆ ನಮೂನೆ_ನಮೂದು *pe;
	ರಚನೆ hashmap_iter iter;
	ರಚನೆ string_list sl = STRING_LIST_INIT_DUP;
	ರಚನೆ strbuf ಪೋಷಕ_ನಮೂನೆ = STRBUF_INIT;

	hashmap_for_each_ನಮೂದು(&pl->ಪೋಷಕ_hashmap, &iter, pe, ent) {
		ಆದರೆ (hashmap_get_ನಮೂದು(&pl->recursive_hashmap, pe, ent, ಖಾಲಿ))
			ಮುಂದುವರಿ;

		ಆದರೆ (!hashmap_ಒಳಗೊಂಡಿದೆ_ಪೋಷಕ(&pl->recursive_hashmap,
					     pe->ನಮೂನೆ,
					     &ಪೋಷಕ_ನಮೂನೆ))
			string_list_append(&sl, pe->ನಮೂನೆ);
	}

	string_list_ವಿಂಗಡಿಸು_u(&sl, 0);

	fprintf(fp, "/*\n!/*/\n");

	ಗಾಗಿ (i = 0; i < sl.nr; i++) {
		ಅಕ್ಷರ *ನಮೂನೆ = escaped_ನಮೂನೆ(sl.ವಸ್ತುಗಳು[i].string);

		ಆದರೆ (ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ನಮೂನೆ))
			fprintf(fp, "%s/\n!%s/*/\n", ನಮೂನೆ, ನಮೂನೆ);
		ಬಿಡುಗಡೆ(ನಮೂನೆ);
	}

	string_list_clear(&sl, 0);

	hashmap_for_each_ನಮೂದು(&pl->recursive_hashmap, &iter, pe, ent) {
		ಆದರೆ (!hashmap_ಒಳಗೊಂಡಿದೆ_ಪೋಷಕ(&pl->recursive_hashmap,
					     pe->ನಮೂನೆ,
					     &ಪೋಷಕ_ನಮೂನೆ))
			string_list_append(&sl, pe->ನಮೂನೆ);
	}

	strbuf_release(&ಪೋಷಕ_ನಮೂನೆ);

	string_list_ವಿಂಗಡಿಸು_u(&sl, 0);

	ಗಾಗಿ (i = 0; i < sl.nr; i++) {
		ಅಕ್ಷರ *ನಮೂನೆ = escaped_ನಮೂನೆ(sl.ವಸ್ತುಗಳು[i].string);
		fprintf(fp, "%s/\n", ನಮೂನೆ);
		ಬಿಡುಗಡೆ(ನಮೂನೆ);
	}

	string_list_clear(&sl, 0);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ write_patterns_and_update(ರಚನೆ repository *repo,
				     ರಚನೆ ನಮೂನೆ_list *pl)
{
	ಅಕ್ಷರ *sparse_filename;
	FILE *fp;
	ರಚನೆ ಬೀಗ_file lk = ಬೀಗ_INIT;
	ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ;
	ರಚನೆ repo_config_ಮೌಲ್ಯಗಳು *cfg = repo_config_ಮೌಲ್ಯಗಳು(the_repository);

	sparse_filename = get_sparse_checkout_filename();

	ಆದರೆ (ಸುರಕ್ಷಿತ_create_leading_directories(repo, sparse_filename))
		die(_("failed to create directory for sparse-checkout file"));

	repo_hold_ಬೀಗ_file_for_update(repo, &lk, sparse_filename,
				       ಬೀಗ_DIE_ON_ERROR);

	ಫಲಿತಾಂಶ = update_working_directory(repo, pl);
	ಆದರೆ (ಫಲಿತಾಂಶ) {
		rollback_ಬೀಗ_file(&lk);
		update_working_directory(repo, ಖಾಲಿ);
		ಹೋಗು ಹೊರಹೋಗು;
	}

	fp = fdopen_ಬೀಗ_file(&lk, "w");
	ಆದರೆ (!fp)
		die_errno(_("unable to fdopen %s"), get_ಬೀಗ_file_ಮಾರ್ಗ(&lk));

	ಆದರೆ (cfg->core_sparse_checkout_cone)
		write_cone_ಗಮ್ಯ_file(fp, pl);
	ಇಲ್ಲದಿದ್ದರೆ
		write_patterns_ಗಮ್ಯ_file(fp, pl);

	ಆದರೆ (commit_ಬೀಗ_file(&lk))
		die_errno(_("unable to write %s"), sparse_filename);

ಹೊರಹೋಗು:
	clear_ನಮೂನೆ_list(pl);
	ಬಿಡುಗಡೆ(sparse_filename);
	ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

ಎಣಿಕೆ sparse_checkout_ಬಹುಲಕ {
	ಬಹುಲಕ_NO_PATTERNS = 0,
	ಬಹುಲಕ_ALL_PATTERNS = 1,
	ಬಹುಲಕ_CONE_PATTERNS = 2,
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಗಣ_config(ರಚನೆ repository *repo,
		      ಎಣಿಕೆ sparse_checkout_ಬಹುಲಕ ಬಹುಲಕ)
{
	/* Update to use worktree config, if not already. */
	ಆದರೆ (init_worktree_config(repo)) {
		error(_("failed to initialize worktree config"));
		ಹಿಂತಿರುಗಿ 1;
	}

	ಆದರೆ (repo_config_ಗಣ_worktree_gently(repo,
					    "core.sparseCheckout",
					    ಬಹುಲಕ ? "true" : "false") ||
	    repo_config_ಗಣ_worktree_gently(repo,
					    "core.sparseCheckoutCone",
					    ಬಹುಲಕ == ಬಹುಲಕ_CONE_PATTERNS ?
						"true" : "false"))
		ಹಿಂತಿರುಗಿ 1;

	ಆದರೆ (ಬಹುಲಕ == ಬಹುಲಕ_NO_PATTERNS)
		ಹಿಂತಿರುಗಿ ಗಣ_sparse_ಸೂಚ್ಯಂಕ_config(repo, 0);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಎಣಿಕೆ sparse_checkout_ಬಹುಲಕ update_cone_ಬಹುಲಕ(ಪೂರ್ಣಾಂಕ *cone_ಬಹುಲಕ) {
	ರಚನೆ repo_config_ಮೌಲ್ಯಗಳು *cfg = repo_config_ಮೌಲ್ಯಗಳು(the_repository);

	/* If not specified, use previous definition of cone mode */
	ಆದರೆ (*cone_ಬಹುಲಕ == -1 && cfg->apply_sparse_checkout)
		*cone_ಬಹುಲಕ = cfg->core_sparse_checkout_cone;

	/* Set cone/non-cone mode appropriately */
	cfg->apply_sparse_checkout = 1;
	ಆದರೆ (*cone_ಬಹುಲಕ == 1 || *cone_ಬಹುಲಕ == -1) {
		cfg->core_sparse_checkout_cone = 1;
		ಹಿಂತಿರುಗಿ ಬಹುಲಕ_CONE_PATTERNS;
	}
	cfg->core_sparse_checkout_cone = 0;
	ಹಿಂತಿರುಗಿ ಬಹುಲಕ_ALL_PATTERNS;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ update_modes(ರಚನೆ repository *repo, ಪೂರ್ಣಾಂಕ *cone_ಬಹುಲಕ, ಪೂರ್ಣಾಂಕ *sparse_ಸೂಚ್ಯಂಕ)
{
	ಪೂರ್ಣಾಂಕ ಬಹುಲಕ, ದಾಖಲೆ_ಬಹುಲಕ;
	ರಚನೆ repo_config_ಮೌಲ್ಯಗಳು *cfg = repo_config_ಮೌಲ್ಯಗಳು(the_repository);

	/* Determine if we need to record the mode; ensure sparse checkout on */
	ದಾಖಲೆ_ಬಹುಲಕ = (*cone_ಬಹುಲಕ != -1) || !cfg->apply_sparse_checkout;

	ಬಹುಲಕ = update_cone_ಬಹುಲಕ(cone_ಬಹುಲಕ);
	ಆದರೆ (ದಾಖಲೆ_ಬಹುಲಕ && ಗಣ_config(repo, ಬಹುಲಕ))
		ಹಿಂತಿರುಗಿ 1;

	/* Set sparse-index/non-sparse-index mode if specified */
	ಆದರೆ (*sparse_ಸೂಚ್ಯಂಕ >= 0) {
		ಆದರೆ (ಗಣ_sparse_ಸೂಚ್ಯಂಕ_config(repo, *sparse_ಸೂಚ್ಯಂಕ) < 0)
			die(_("failed to modify sparse-index config"));

		/* force an index rewrite */
		repo_read_ಸೂಚ್ಯಂಕ(repo);
		repo->ಸೂಚ್ಯಂಕ->updated_workdir = 1;

		ಆದರೆ (!*sparse_ಸೂಚ್ಯಂಕ)
			ensure_ತುಂಬಿದ_ಸೂಚ್ಯಂಕ(repo->ಸೂಚ್ಯಂಕ);
	}

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ * ಸ್ಥಿರಾಂಕ builtin_sparse_checkout_init_usage[] = {
	"git sparse-checkout init [--cone] [--[no-]sparse-index]",
	ಖಾಲಿ
};

ಸ್ಥಿರ ರಚನೆ sparse_checkout_init_opts {
	ಪೂರ್ಣಾಂಕ cone_ಬಹುಲಕ;
	ಪೂರ್ಣಾಂಕ sparse_ಸೂಚ್ಯಂಕ;
} init_opts;

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sparse_checkout_init(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
				ರಚನೆ repository *repo)
{
	ರಚನೆ ನಮೂನೆ_list pl;
	ಅಕ್ಷರ *sparse_filename;
	ಪೂರ್ಣಾಂಕ res;
	ರಚನೆ object_id oid;

	ಸ್ಥಿರ ರಚನೆ option builtin_sparse_checkout_init_options[] = {
		OPT_BOOL(0, "cone", &init_opts.cone_ಬಹುಲಕ,
			 N_("initialize the sparse-checkout in cone mode")),
		OPT_BOOL(0, "sparse-index", &init_opts.sparse_ಸೂಚ್ಯಂಕ,
			 N_("toggle the use of a sparse index")),
		OPT_ಕೊನೆ(),
	};

	ಸಿದ್ಧತೆ_work_ವೃಕ್ಷ(the_repository);
	repo_read_ಸೂಚ್ಯಂಕ(repo);

	init_opts.cone_ಬಹುಲಕ = -1;
	init_opts.sparse_ಸೂಚ್ಯಂಕ = -1;

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ,
			     builtin_sparse_checkout_init_options,
			     builtin_sparse_checkout_init_usage, 0);

	ಆದರೆ (update_modes(repo, &init_opts.cone_ಬಹುಲಕ, &init_opts.sparse_ಸೂಚ್ಯಂಕ))
		ಹಿಂತಿರುಗಿ 1;

	memset(&pl, 0, ಗಾತ್ರ(pl));

	sparse_filename = get_sparse_checkout_filename();
	res = ಕೂಡಿಸು_patterns_from_file_ಗಮ್ಯ_list(sparse_filename, "", 0, &pl, ಖಾಲಿ, 0);

	/* If we already have a sparse-checkout file, use it. */
	ಆದರೆ (res >= 0) {
		ಬಿಡುಗಡೆ(sparse_filename);
		clear_ನಮೂನೆ_list(&pl);
		ಹಿಂತಿರುಗಿ update_working_directory(repo, ಖಾಲಿ);
	}

	ಆದರೆ (repo_get_oid(repo, "HEAD", &oid)) {
		FILE *fp;

		/* assume we are in a fresh repo, but update the sparse-checkout file */
		ಆದರೆ (ಸುರಕ್ಷಿತ_create_leading_directories(repo, sparse_filename))
			die(_("unable to create leading directories of %s"),
			    sparse_filename);
		fp = xfopen(sparse_filename, "w");
		ಆದರೆ (!fp)
			die(_("failed to open '%s'"), sparse_filename);

		ಬಿಡುಗಡೆ(sparse_filename);
		fprintf(fp, "/*\n!/*/\n");
		fclose(fp);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಬಿಡುಗಡೆ(sparse_filename);

	ಕೂಡಿಸು_ನಮೂನೆ("/*", ಖಾಲಿಗಳು_ತಳ, 0, &pl, 0);
	ಕೂಡಿಸು_ನಮೂನೆ("!/*/", ಖಾಲಿಗಳು_ತಳ, 0, &pl, 0);
	pl.use_cone_patterns = init_opts.cone_ಬಹುಲಕ;

	ಹಿಂತಿರುಗಿ write_patterns_and_update(repo, &pl);
}

ಸ್ಥಿರ ಶೂನ್ಯ ಸೇರಿಸು_recursive_ನಮೂನೆ(ರಚನೆ ನಮೂನೆ_list *pl, ರಚನೆ strbuf *ಮಾರ್ಗ)
{
	ರಚನೆ ನಮೂನೆ_ನಮೂದು *e = xmalloc(ಗಾತ್ರ(*e));
	e->patternlen = ಮಾರ್ಗ->len;
	e->ನಮೂನೆ = strbuf_detach(ಮಾರ್ಗ, ಖಾಲಿ);
	hashmap_ನಮೂದು_init(&e->ent, fspathhash(e->ನಮೂನೆ));

	hashmap_ಕೂಡಿಸು(&pl->recursive_hashmap, &e->ent);

	ಆಗ (e->patternlen) {
		ಅಕ್ಷರ *slash = strrchr(e->ನಮೂನೆ, '/');
		ಅಕ್ಷರ *oldpattern = e->ನಮೂನೆ;
		ಗಾತ್ರ_t newlen;
		ರಚನೆ ನಮೂನೆ_ನಮೂದು *dup;

		ಆದರೆ (!slash || slash == e->ನಮೂನೆ)
			ನಿಲ್ಲಿಸು;

		newlen = slash - e->ನಮೂನೆ;
		e = xmalloc(ಗಾತ್ರ(ರಚನೆ ನಮೂನೆ_ನಮೂದು));
		e->patternlen = newlen;
		e->ನಮೂನೆ = xstrndup(oldpattern, newlen);
		hashmap_ನಮೂದು_init(&e->ent, fspathhash(e->ನಮೂನೆ));

		dup = hashmap_get_ನಮೂದು(&pl->ಪೋಷಕ_hashmap, e, ent, ಖಾಲಿ);
		ಆದರೆ (!dup) {
			hashmap_ಕೂಡಿಸು(&pl->ಪೋಷಕ_hashmap, &e->ent);
		} ಇಲ್ಲದಿದ್ದರೆ {
			ಬಿಡುಗಡೆ(e->ನಮೂನೆ);
			ಬಿಡುಗಡೆ(e);
			e = dup;
		}
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ strbuf_ಗಮ್ಯ_cone_ನಮೂನೆ(ರಚನೆ strbuf *ಸಾಲು, ರಚನೆ ನಮೂನೆ_list *pl)
{
	strbuf_trim(ಸಾಲು);

	strbuf_trim_trailing_dir_sep(ಸಾಲು);

	ಆದರೆ (strbuf_normalize_ಮಾರ್ಗ(ಸಾಲು))
		die(_("could not normalize path %s"), ಸಾಲು->buf);

	ಆದರೆ (!ಸಾಲು->len)
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (ಸಾಲು->buf[0] != '/')
		strbuf_insertstr(ಸಾಲು, 0, "/");

	ಸೇರಿಸು_recursive_ನಮೂನೆ(pl, ಸಾಲು);
}

ಸ್ಥಿರ ಶೂನ್ಯ ಕೂಡಿಸು_patterns_from_ಒಳಹರಿವು(ರಚನೆ ನಮೂನೆ_list *pl,
				    ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
				    FILE *file)
{
	ಪೂರ್ಣಾಂಕ i;
	ರಚನೆ repo_config_ಮೌಲ್ಯಗಳು *cfg = repo_config_ಮೌಲ್ಯಗಳು(the_repository);

	ಆದರೆ (cfg->core_sparse_checkout_cone) {
		ರಚನೆ strbuf ಸಾಲು = STRBUF_INIT;

		hashmap_init(&pl->recursive_hashmap, pl_hashmap_cmp, ಖಾಲಿ, 0);
		hashmap_init(&pl->ಪೋಷಕ_hashmap, pl_hashmap_cmp, ಖಾಲಿ, 0);
		pl->use_cone_patterns = 1;

		ಆದರೆ (file) {
			ರಚನೆ strbuf unquoted = STRBUF_INIT;
			ಆಗ (!strbuf_getline(&ಸಾಲು, file)) {
				ಆದರೆ (ಸಾಲು.buf[0] == '"') {
					strbuf_reset(&unquoted);
					ಆದರೆ (unquote_c_style(&unquoted, ಸಾಲು.buf, ಖಾಲಿ))
						die(_("unable to unquote C-style string '%s'"),
						ಸಾಲು.buf);

					strbuf_ಅದಲುಬದಲು(&unquoted, &ಸಾಲು);
				}

				strbuf_ಗಮ್ಯ_cone_ನಮೂನೆ(&ಸಾಲು, pl);
			}

			strbuf_release(&unquoted);
		} ಇಲ್ಲದಿದ್ದರೆ {
			ಗಾಗಿ (i = 0; i < argc; i++) {
				strbuf_setlen(&ಸಾಲು, 0);
				strbuf_addstr(&ಸಾಲು, argv[i]);
				strbuf_ಗಮ್ಯ_cone_ನಮೂನೆ(&ಸಾಲು, pl);
			}
		}
		strbuf_release(&ಸಾಲು);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಆದರೆ (file) {
			ರಚನೆ strbuf ಸಾಲು = STRBUF_INIT;

			ಆಗ (!strbuf_getline(&ಸಾಲು, file))
				ಕೂಡಿಸು_ನಮೂನೆ(ಸಾಲು.buf, ಖಾಲಿಗಳು_ತಳ, 0, pl, 0);

			strbuf_release(&ಸಾಲು);
		} ಇಲ್ಲದಿದ್ದರೆ {
			ಗಾಗಿ (i = 0; i < argc; i++)
				ಕೂಡಿಸು_ನಮೂನೆ(argv[i], ಖಾಲಿಗಳು_ತಳ, 0, pl, 0);
		}
	}
}

ಎಣಿಕೆ modify_type {
	REPLACE,
	ಕೂಡಿಸು_2,
};

ಸ್ಥಿರ ಶೂನ್ಯ ಕೂಡಿಸು_patterns_cone_ಬಹುಲಕ(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
				   ರಚನೆ ನಮೂನೆ_list *pl,
				   ಪೂರ್ಣಾಂಕ use_stdin)
{
	ರಚನೆ strbuf ಬಫರ್ = STRBUF_INIT;
	ರಚನೆ ನಮೂನೆ_ನಮೂದು *pe;
	ರಚನೆ hashmap_iter iter;
	ರಚನೆ ನಮೂನೆ_list existing;
	ರಚನೆ repo_config_ಮೌಲ್ಯಗಳು *cfg = repo_config_ಮೌಲ್ಯಗಳು(the_repository);
	ಅಕ್ಷರ *sparse_filename = get_sparse_checkout_filename();

	ಕೂಡಿಸು_patterns_from_ಒಳಹರಿವು(pl, argc, argv,
				use_stdin ? stdin : ಖಾಲಿ);

	memset(&existing, 0, ಗಾತ್ರ(existing));
	existing.use_cone_patterns = cfg->core_sparse_checkout_cone;

	ಆದರೆ (ಕೂಡಿಸು_patterns_from_file_ಗಮ್ಯ_list(sparse_filename, "", 0,
					   &existing, ಖಾಲಿ, 0))
		die(_("unable to load existing sparse-checkout patterns"));
	ಬಿಡುಗಡೆ(sparse_filename);

	ಆದರೆ (!existing.use_cone_patterns)
		die(_("existing sparse-checkout patterns do not use cone mode"));

	hashmap_for_each_ನಮೂದು(&existing.recursive_hashmap, &iter, pe, ent) {
		ಆದರೆ (!hashmap_ಒಳಗೊಂಡಿದೆ_ಪೋಷಕ(&pl->recursive_hashmap,
					pe->ನಮೂನೆ, &ಬಫರ್) ||
		    !hashmap_ಒಳಗೊಂಡಿದೆ_ಪೋಷಕ(&pl->ಪೋಷಕ_hashmap,
					pe->ನಮೂನೆ, &ಬಫರ್)) {
			strbuf_reset(&ಬಫರ್);
			strbuf_addstr(&ಬಫರ್, pe->ನಮೂನೆ);
			ಸೇರಿಸು_recursive_ನಮೂನೆ(pl, &ಬಫರ್);
		}
	}

	clear_ನಮೂನೆ_list(&existing);
	strbuf_release(&ಬಫರ್);
}

ಸ್ಥಿರ ಶೂನ್ಯ ಕೂಡಿಸು_patterns_literal(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
				 ರಚನೆ ನಮೂನೆ_list *pl,
				 ಪೂರ್ಣಾಂಕ use_stdin)
{
	ಅಕ್ಷರ *sparse_filename = get_sparse_checkout_filename();
	ಆದರೆ (ಕೂಡಿಸು_patterns_from_file_ಗಮ್ಯ_list(sparse_filename, "", 0,
					   pl, ಖಾಲಿ, 0))
		die(_("unable to load existing sparse-checkout patterns"));
	ಬಿಡುಗಡೆ(sparse_filename);
	ಕೂಡಿಸು_patterns_from_ಒಳಹರಿವು(pl, argc, argv, use_stdin ? stdin : ಖಾಲಿ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ modify_ನಮೂನೆ_list(ರಚನೆ repository *repo,
			       ರಚನೆ strvec *ವಾದಗಳು, ಪೂರ್ಣಾಂಕ use_stdin,
			       ಎಣಿಕೆ modify_type m)
{
	ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ;
	ಪೂರ್ಣಾಂಕ ಬದಲಾಯಿತು_config = 0;
	ರಚನೆ ನಮೂನೆ_list *pl = xcalloc(1, ಗಾತ್ರ(*pl));
	ರಚನೆ repo_config_ಮೌಲ್ಯಗಳು *cfg = repo_config_ಮೌಲ್ಯಗಳು(the_repository);

	ಆಯ್ಕೆ (m) {
	ಸ್ಥಿತಿ ಕೂಡಿಸು_2:
		ಆದರೆ (cfg->core_sparse_checkout_cone)
			ಕೂಡಿಸು_patterns_cone_ಬಹುಲಕ(ವಾದಗಳು->nr, ವಾದಗಳು->v, pl, use_stdin);
		ಇಲ್ಲದಿದ್ದರೆ
			ಕೂಡಿಸು_patterns_literal(ವಾದಗಳು->nr, ವಾದಗಳು->v, pl, use_stdin);
		ನಿಲ್ಲಿಸು;

	ಸ್ಥಿತಿ REPLACE:
		ಕೂಡಿಸು_patterns_from_ಒಳಹರಿವು(pl, ವಾದಗಳು->nr, ವಾದಗಳು->v,
					use_stdin ? stdin : ಖಾಲಿ);
		ನಿಲ್ಲಿಸು;
	}

	ಆದರೆ (!cfg->apply_sparse_checkout) {
		ಗಣ_config(repo, ಬಹುಲಕ_ALL_PATTERNS);
		cfg->apply_sparse_checkout = 1;
		ಬದಲಾಯಿತು_config = 1;
	}

	ಫಲಿತಾಂಶ = write_patterns_and_update(repo, pl);

	ಆದರೆ (ಫಲಿತಾಂಶ && ಬದಲಾಯಿತು_config)
		ಗಣ_config(repo, ಬಹುಲಕ_NO_PATTERNS);

	clear_ನಮೂನೆ_list(pl);
	ಬಿಡುಗಡೆ(pl);
	ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

ಸ್ಥಿರ ಶೂನ್ಯ sanitize_paths(ರಚನೆ repository *repo,
			   ರಚನೆ strvec *ವಾದಗಳು,
			   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ, ಪೂರ್ಣಾಂಕ skip_checks)
{
	ಪೂರ್ಣಾಂಕ i;
	ರಚನೆ repo_config_ಮೌಲ್ಯಗಳು *cfg = repo_config_ಮೌಲ್ಯಗಳು(the_repository);

	ಆದರೆ (!ವಾದಗಳು->nr)
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (ಪೂರ್ವಪ್ರತ್ಯಯ && *ಪೂರ್ವಪ್ರತ್ಯಯ && cfg->core_sparse_checkout_cone) {
		/*
		 * The args are not pathspecs, so unfortunately we
		 * cannot imitate how cmd_add() uses parse_pathspec().
		 */
		ಪೂರ್ಣಾಂಕ ಪೂರ್ವಪ್ರತ್ಯಯ_len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ಪೂರ್ವಪ್ರತ್ಯಯ);

		ಗಾಗಿ (i = 0; i < ವಾದಗಳು->nr; i++) {
			ಅಕ್ಷರ *prefixed_ಮಾರ್ಗ = ಪೂರ್ವಪ್ರತ್ಯಯ_ಮಾರ್ಗ(the_repository, ಪೂರ್ವಪ್ರತ್ಯಯ,
							  ಪೂರ್ವಪ್ರತ್ಯಯ_len, ವಾದಗಳು->v[i]);
			strvec_replace(ವಾದಗಳು, i, prefixed_ಮಾರ್ಗ);
			ಬಿಡುಗಡೆ(prefixed_ಮಾರ್ಗ);
		}
	}

	ಆದರೆ (skip_checks)
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (ಪೂರ್ವಪ್ರತ್ಯಯ && *ಪೂರ್ವಪ್ರತ್ಯಯ && !cfg->core_sparse_checkout_cone)
		die(_("please run from the toplevel directory in non-cone mode"));

	ಆದರೆ (cfg->core_sparse_checkout_cone) {
		ಗಾಗಿ (i = 0; i < ವಾದಗಳು->nr; i++) {
			ಆದರೆ (ವಾದಗಳು->v[i][0] == '/')
				die(_("specify directories rather than patterns (no leading slash)"));
			ಆದರೆ (ವಾದಗಳು->v[i][0] == '!')
				die(_("specify directories rather than patterns.  If your directory starts with a '!', pass --skip-checks"));
			ಆದರೆ (strpbrk(ವಾದಗಳು->v[i], "*?[]"))
				die(_("specify directories rather than patterns.  If your directory really has any of '*?[]\\' in it, pass --skip-checks"));
		}
	}

	ಗಾಗಿ (i = 0; i < ವಾದಗಳು->nr; i++) {
		ರಚನೆ cache_ನಮೂದು *ce;
		ರಚನೆ ಸೂಚ್ಯಂಕ_state *ಸೂಚ್ಯಂಕ = repo->ಸೂಚ್ಯಂಕ;
		ಪೂರ್ಣಾಂಕ pos = ಸೂಚ್ಯಂಕ_name_pos(ಸೂಚ್ಯಂಕ, ವಾದಗಳು->v[i], ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ವಾದಗಳು->v[i]));

		ಆದರೆ (pos < 0)
			ಮುಂದುವರಿ;
		ce = ಸೂಚ್ಯಂಕ->cache[pos];
		ಆದರೆ (S_ISSPARSEDIR(ce->ce_ಬಹುಲಕ))
			ಮುಂದುವರಿ;

		ಆದರೆ (cfg->core_sparse_checkout_cone)
			die(_("'%s' is not a directory; to treat it as a directory anyway, rerun with --skip-checks"), ವಾದಗಳು->v[i]);
		ಇಲ್ಲದಿದ್ದರೆ
			warning(_("pass a leading slash before paths such as '%s' if you want a single file (see NON-CONE PROBLEMS in the git-sparse-checkout manual)."), ವಾದಗಳು->v[i]);
	}
}

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ * ಸ್ಥಿರಾಂಕ builtin_sparse_checkout_ಕೂಡಿಸು_usage[] = {
	N_("git sparse-checkout add [--skip-checks] (--stdin | <patterns>)"),
	ಖಾಲಿ
};

ಸ್ಥಿರ ರಚನೆ sparse_checkout_ಕೂಡಿಸು_opts {
	ಪೂರ್ಣಾಂಕ skip_checks;
	ಪೂರ್ಣಾಂಕ use_stdin;
} ಕೂಡಿಸು_opts;

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sparse_checkout_ಕೂಡಿಸು(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			       ರಚನೆ repository *repo)
{
	ಸ್ಥಿರ ರಚನೆ option builtin_sparse_checkout_ಕೂಡಿಸು_options[] = {
		OPT_BOOL_F(0, "skip-checks", &ಕೂಡಿಸು_opts.skip_checks,
			   N_("skip some sanity checks on the given paths that might give false positives"),
			   PARSE_OPT_NONEG),
		OPT_BOOL(0, "stdin", &ಕೂಡಿಸು_opts.use_stdin,
			 N_("read patterns from standard in")),
		OPT_ಕೊನೆ(),
	};
	ರಚನೆ strvec patterns = STRVEC_INIT;
	ಪೂರ್ಣಾಂಕ ret;
	ರಚನೆ repo_config_ಮೌಲ್ಯಗಳು *cfg = repo_config_ಮೌಲ್ಯಗಳು(the_repository);

	ಸಿದ್ಧತೆ_work_ವೃಕ್ಷ(the_repository);
	ಆದರೆ (!cfg->apply_sparse_checkout)
		die(_("no sparse-checkout to add to"));

	repo_read_ಸೂಚ್ಯಂಕ(repo);

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ,
			     builtin_sparse_checkout_ಕೂಡಿಸು_options,
			     builtin_sparse_checkout_ಕೂಡಿಸು_usage, 0);

	ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < argc; i++)
		strvec_ತಳ್ಳು(&patterns, argv[i]);
	sanitize_paths(repo, &patterns, ಪೂರ್ವಪ್ರತ್ಯಯ, ಕೂಡಿಸು_opts.skip_checks);

	ret = modify_ನಮೂನೆ_list(repo, &patterns, ಕೂಡಿಸು_opts.use_stdin, ಕೂಡಿಸು_2);

	strvec_clear(&patterns);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ * ಸ್ಥಿರಾಂಕ builtin_sparse_checkout_ಗಣ_usage[] = {
	N_("git sparse-checkout set [--[no-]cone] [--[no-]sparse-index] [--skip-checks] (--stdin | <patterns>)"),
	ಖಾಲಿ
};

ಸ್ಥಿರ ರಚನೆ sparse_checkout_ಗಣ_opts {
	ಪೂರ್ಣಾಂಕ cone_ಬಹುಲಕ;
	ಪೂರ್ಣಾಂಕ sparse_ಸೂಚ್ಯಂಕ;
	ಪೂರ್ಣಾಂಕ skip_checks;
	ಪೂರ್ಣಾಂಕ use_stdin;
} ಗಣ_opts;

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sparse_checkout_ಗಣ(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			       ರಚನೆ repository *repo)
{
	ರಚನೆ repo_config_ಮೌಲ್ಯಗಳು *cfg = repo_config_ಮೌಲ್ಯಗಳು(the_repository);
	ಪೂರ್ಣಾಂಕ default_patterns_nr = 2;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *default_patterns[] = {"/*", "!/*/", ಖಾಲಿ};

	ಸ್ಥಿರ ರಚನೆ option builtin_sparse_checkout_ಗಣ_options[] = {
		OPT_BOOL(0, "cone", &ಗಣ_opts.cone_ಬಹುಲಕ,
			 N_("initialize the sparse-checkout in cone mode")),
		OPT_BOOL(0, "sparse-index", &ಗಣ_opts.sparse_ಸೂಚ್ಯಂಕ,
			 N_("toggle the use of a sparse index")),
		OPT_BOOL_F(0, "skip-checks", &ಗಣ_opts.skip_checks,
			   N_("skip some sanity checks on the given paths that might give false positives"),
			   PARSE_OPT_NONEG),
		OPT_BOOL_F(0, "stdin", &ಗಣ_opts.use_stdin,
			   N_("read patterns from standard in"),
			   PARSE_OPT_NONEG),
		OPT_ಕೊನೆ(),
	};
	ರಚನೆ strvec patterns = STRVEC_INIT;
	ಪೂರ್ಣಾಂಕ ret;

	ಸಿದ್ಧತೆ_work_ವೃಕ್ಷ(the_repository);
	repo_read_ಸೂಚ್ಯಂಕ(repo);

	ಗಣ_opts.cone_ಬಹುಲಕ = -1;
	ಗಣ_opts.sparse_ಸೂಚ್ಯಂಕ = -1;

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ,
			     builtin_sparse_checkout_ಗಣ_options,
			     builtin_sparse_checkout_ಗಣ_usage, 0);

	ಆದರೆ (update_modes(repo, &ಗಣ_opts.cone_ಬಹುಲಕ, &ಗಣ_opts.sparse_ಸೂಚ್ಯಂಕ))
		ಹಿಂತಿರುಗಿ 1;

	/*
	 * Cone mode automatically specifies the toplevel directory.  For
	 * non-cone mode, if nothing is specified, manually select just the
	 * top-level directory (much as 'init' would do).
	 */
	ಆದರೆ (!cfg->core_sparse_checkout_cone && !ಗಣ_opts.use_stdin && argc == 0) {
		ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < default_patterns_nr; i++)
			strvec_ತಳ್ಳು(&patterns, default_patterns[i]);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < argc; i++)
			strvec_ತಳ್ಳು(&patterns, argv[i]);
		sanitize_paths(repo, &patterns, ಪೂರ್ವಪ್ರತ್ಯಯ, ಗಣ_opts.skip_checks);
	}

	ret = modify_ನಮೂನೆ_list(repo, &patterns, ಗಣ_opts.use_stdin, REPLACE);

	strvec_clear(&patterns);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ * ಸ್ಥಿರಾಂಕ builtin_sparse_checkout_reapply_usage[] = {
	"git sparse-checkout reapply [--[no-]cone] [--[no-]sparse-index]",
	ಖಾಲಿ
};

ಸ್ಥಿರ ರಚನೆ sparse_checkout_reapply_opts {
	ಪೂರ್ಣಾಂಕ cone_ಬಹುಲಕ;
	ಪೂರ್ಣಾಂಕ sparse_ಸೂಚ್ಯಂಕ;
} reapply_opts;

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sparse_checkout_reapply(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
				   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
				   ರಚನೆ repository *repo)
{
	ಸ್ಥಿರ ರಚನೆ option builtin_sparse_checkout_reapply_options[] = {
		OPT_BOOL(0, "cone", &reapply_opts.cone_ಬಹುಲಕ,
			 N_("initialize the sparse-checkout in cone mode")),
		OPT_BOOL(0, "sparse-index", &reapply_opts.sparse_ಸೂಚ್ಯಂಕ,
			 N_("toggle the use of a sparse index")),
		OPT_ಕೊನೆ(),
	};
	ರಚನೆ repo_config_ಮೌಲ್ಯಗಳು *cfg = repo_config_ಮೌಲ್ಯಗಳು(the_repository);

	ಸಿದ್ಧತೆ_work_ವೃಕ್ಷ(the_repository);
	ಆದರೆ (!cfg->apply_sparse_checkout)
		die(_("must be in a sparse-checkout to reapply sparsity patterns"));

	reapply_opts.cone_ಬಹುಲಕ = -1;
	reapply_opts.sparse_ಸೂಚ್ಯಂಕ = -1;

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ,
			     builtin_sparse_checkout_reapply_options,
			     builtin_sparse_checkout_reapply_usage, 0);

	repo_read_ಸೂಚ್ಯಂಕ(repo);

	ಆದರೆ (update_modes(repo, &reapply_opts.cone_ಬಹುಲಕ, &reapply_opts.sparse_ಸೂಚ್ಯಂಕ))
		ಹಿಂತಿರುಗಿ 1;

	ಹಿಂತಿರುಗಿ update_working_directory(repo, ಖಾಲಿ);
}

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ * ಸ್ಥಿರಾಂಕ builtin_sparse_checkout_clean_usage[] = {
	"git sparse-checkout clean [-n|--dry-run]",
	ಖಾಲಿ
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ list_file_iterator(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ = ದತ್ತಾಂಶ;

	ಬರೆ(ಸಂದೇಶ, ಮಾರ್ಗ);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ list_every_file_ಒಳ_dir(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ,
				   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *directory)
{
	ರಚನೆ strbuf ಮಾರ್ಗ = STRBUF_INIT;

	strbuf_addstr(&ಮಾರ್ಗ, directory);
	for_each_file_ಒಳ_dir(&ಮಾರ್ಗ, list_file_iterator, ಸಂದೇಶ);
	strbuf_release(&ಮಾರ್ಗ);
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ_remove = N_("Removing %s\n");
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ_would_remove = N_("Would remove %s\n");

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sparse_checkout_clean(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
				   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
				   ರಚನೆ repository *repo)
{
	ರಚನೆ strbuf ತುಂಬಿದ_ಮಾರ್ಗ = STRBUF_INIT;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ = ಸಂದೇಶ_remove;
	ಗಾತ್ರ_t worktree_len;
	ಪೂರ್ಣಾಂಕ force = 0, dry_ಸತತ = 0, verbose = 0;
	ಪೂರ್ಣಾಂಕ require_force = 1;
	ರಚನೆ repo_config_ಮೌಲ್ಯಗಳು *cfg = repo_config_ಮೌಲ್ಯಗಳು(the_repository);

	ರಚನೆ option builtin_sparse_checkout_clean_options[] = {
		OPT_DRY_ಸತತ(&dry_ಸತತ, N_("dry run")),
		OPT__FORCE(&force, N_("force"), PARSE_OPT_NOCOMPLETE),
		OPT__VERBOSE(&verbose, N_("report each affected file, not just directories")),
		OPT_ಕೊನೆ(),
	};

	ಸಿದ್ಧತೆ_work_ವೃಕ್ಷ(the_repository);
	ಆದರೆ (!cfg->apply_sparse_checkout)
		die(_("must be in a sparse-checkout to clean directories"));
	ಆದರೆ (!cfg->core_sparse_checkout_cone)
		die(_("must be in a cone-mode sparse-checkout to clean directories"));

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ,
			     builtin_sparse_checkout_clean_options,
			     builtin_sparse_checkout_clean_usage, 0);

	repo_config_get_bool(repo, "clean.requireforce", &require_force);
	ಆದರೆ (require_force && !force && !dry_ಸತತ)
		die(_("for safety, refusing to clean without one of --force or --dry-run"));

	ಆದರೆ (dry_ಸತತ)
		ಸಂದೇಶ = ಸಂದೇಶ_would_remove;

	ಆದರೆ (repo_read_ಸೂಚ್ಯಂಕ(repo) < 0)
		die(_("failed to read index"));

	ಆದರೆ (convert_ಗಮ್ಯ_sparse(repo->ಸೂಚ್ಯಂಕ, SPARSE_ಸೂಚ್ಯಂಕ_ಸ್ಮರಣೆ_ONLY) ||
	    repo->ಸೂಚ್ಯಂಕ->sparse_ಸೂಚ್ಯಂಕ == ಸೂಚ್ಯಂಕ_EXPANDED)
		die(_("failed to convert index to a sparse index; resolve merge conflicts and try again"));

	strbuf_addstr(&ತುಂಬಿದ_ಮಾರ್ಗ, repo->worktree);
	strbuf_addch(&ತುಂಬಿದ_ಮಾರ್ಗ, '/');
	worktree_len = ತುಂಬಿದ_ಮಾರ್ಗ.len;

	ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < repo->ಸೂಚ್ಯಂಕ->cache_nr; i++) {
		ರಚನೆ cache_ನಮೂದು *ce = repo->ಸೂಚ್ಯಂಕ->cache[i];
		ಆದರೆ (!S_ISSPARSEDIR(ce->ce_ಬಹುಲಕ))
			ಮುಂದುವರಿ;
		strbuf_setlen(&ತುಂಬಿದ_ಮಾರ್ಗ, worktree_len);
		strbuf_ಕೂಡಿಸು(&ತುಂಬಿದ_ಮಾರ್ಗ, ce->name, ce->ce_namelen);

		ಆದರೆ (!is_directory(ತುಂಬಿದ_ಮಾರ್ಗ.buf))
			ಮುಂದುವರಿ;

		ಆದರೆ (verbose)
			list_every_file_ಒಳ_dir(ಸಂದೇಶ, ce->name);
		ಇಲ್ಲದಿದ್ದರೆ
			ಬರೆ(ಸಂದೇಶ, ce->name);

		ಆದರೆ (dry_ಸತತ <= 0 &&
		    remove_dir_recursively(&ತುಂಬಿದ_ಮಾರ್ಗ, 0))
			warning_errno(_("failed to remove '%s'"), ce->name);
	}

	strbuf_release(&ತುಂಬಿದ_ಮಾರ್ಗ);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ * ಸ್ಥಿರಾಂಕ builtin_sparse_checkout_disable_usage[] = {
	"git sparse-checkout disable",
	ಖಾಲಿ
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sparse_checkout_disable(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
				   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
				   ರಚನೆ repository *repo)
{
	ಸ್ಥಿರ ರಚನೆ option builtin_sparse_checkout_disable_options[] = {
		OPT_ಕೊನೆ(),
	};
	ರಚನೆ ನಮೂನೆ_list pl;
	ರಚನೆ repo_config_ಮೌಲ್ಯಗಳು *cfg = repo_config_ಮೌಲ್ಯಗಳು(the_repository);

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

	ಸಿದ್ಧತೆ_work_ವೃಕ್ಷ(the_repository);
	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ,
			     builtin_sparse_checkout_disable_options,
			     builtin_sparse_checkout_disable_usage, 0);

	/*
	 * Disable the advice message for expanding a sparse index, as we
	 * are expecting to do that when disabling sparse-checkout.
	 */
	give_advice_on_expansion = 0;
	repo_read_ಸೂಚ್ಯಂಕ(repo);

	memset(&pl, 0, ಗಾತ್ರ(pl));
	hashmap_init(&pl.recursive_hashmap, pl_hashmap_cmp, ಖಾಲಿ, 0);
	hashmap_init(&pl.ಪೋಷಕ_hashmap, pl_hashmap_cmp, ಖಾಲಿ, 0);
	pl.use_cone_patterns = 0;
	cfg->apply_sparse_checkout = 1;

	ಕೂಡಿಸು_ನಮೂನೆ("/*", ಖಾಲಿಗಳು_ತಳ, 0, &pl, 0);

	prepare_repo_settings(the_repository);
	repo->settings.sparse_ಸೂಚ್ಯಂಕ = 0;

	ಆದರೆ (update_working_directory(repo, &pl))
		die(_("error while refreshing working directory"));

	clear_ನಮೂನೆ_list(&pl);
	ಹಿಂತಿರುಗಿ ಗಣ_config(repo, ಬಹುಲಕ_NO_PATTERNS);
}

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ * ಸ್ಥಿರಾಂಕ builtin_sparse_checkout_check_ನಿಯಮಗಳು_usage[] = {
	N_("git sparse-checkout check-rules [-z] [--skip-checks]"
	   "[--[no-]cone] [--rules-file <file>]"),
	ಖಾಲಿ
};

ಸ್ಥಿರ ರಚನೆ sparse_checkout_check_ನಿಯಮಗಳು_opts {
	ಪೂರ್ಣಾಂಕ cone_ಬಹುಲಕ;
	ಪೂರ್ಣಾಂಕ null_termination;
	ಅಕ್ಷರ *ನಿಯಮಗಳು_file;
} check_ನಿಯಮಗಳು_opts;

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ check_ನಿಯಮಗಳು(ರಚನೆ repository *repo,
		       ರಚನೆ ನಮೂನೆ_list *pl,
		       ಪೂರ್ಣಾಂಕ null_terminated)
{
	ರಚನೆ strbuf ಸಾಲು = STRBUF_INIT;
	ರಚನೆ strbuf unquoted = STRBUF_INIT;
	ಅಕ್ಷರ *ಮಾರ್ಗ;
	ಪೂರ್ಣಾಂಕ ಸಾಲು_terminator = null_terminated ? 0 : '\n';
	strbuf_getline_fn getline_fn = null_terminated ? strbuf_getline_nul
		: strbuf_getline;
	repo->ಸೂಚ್ಯಂಕ->sparse_checkout_patterns = pl;
	ಆಗ (!getline_fn(&ಸಾಲು, stdin)) {
		ಮಾರ್ಗ = ಸಾಲು.buf;
		ಆದರೆ (!null_terminated && ಸಾಲು.buf[0] == '"') {
			strbuf_reset(&unquoted);
			ಆದರೆ (unquote_c_style(&unquoted, ಸಾಲು.buf, ಖಾಲಿ))
				die(_("unable to unquote C-style string '%s'"),
					ಸಾಲು.buf);

			ಮಾರ್ಗ = unquoted.buf;
		}

		ಆದರೆ (ಮಾರ್ಗ_ಒಳ_sparse_checkout(ಮಾರ್ಗ, repo->ಸೂಚ್ಯಂಕ))
			write_name_quoted(ಮಾರ್ಗ, stdout, ಸಾಲು_terminator);
	}
	strbuf_release(&ಸಾಲು);
	strbuf_release(&unquoted);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sparse_checkout_check_ನಿಯಮಗಳು(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
				       ರಚನೆ repository *repo)
{
	ಸ್ಥಿರ ರಚನೆ option builtin_sparse_checkout_check_ನಿಯಮಗಳು_options[] = {
		OPT_BOOL('z', ಖಾಲಿ, &check_ನಿಯಮಗಳು_opts.null_termination,
			 N_("terminate input and output files by a NUL character")),
		OPT_BOOL(0, "cone", &check_ನಿಯಮಗಳು_opts.cone_ಬಹುಲಕ,
			 N_("when used with --rules-file interpret patterns as cone mode patterns")),
		OPT_FILENAME(0, "rules-file", &check_ನಿಯಮಗಳು_opts.ನಿಯಮಗಳು_file,
			 N_("use patterns in <file> instead of the current ones.")),
		OPT_ಕೊನೆ(),
	};

	FILE *fp;
	ಪೂರ್ಣಾಂಕ ret;
	ರಚನೆ ನಮೂನೆ_list pl = {0};
	ರಚನೆ repo_config_ಮೌಲ್ಯಗಳು *cfg = repo_config_ಮೌಲ್ಯಗಳು(the_repository);
	ಅಕ್ಷರ *sparse_filename;
	check_ನಿಯಮಗಳು_opts.cone_ಬಹುಲಕ = -1;

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ,
			     builtin_sparse_checkout_check_ನಿಯಮಗಳು_options,
			     builtin_sparse_checkout_check_ನಿಯಮಗಳು_usage, 0);

	ಆದರೆ (check_ನಿಯಮಗಳು_opts.ನಿಯಮಗಳು_file && check_ನಿಯಮಗಳು_opts.cone_ಬಹುಲಕ < 0)
		check_ನಿಯಮಗಳು_opts.cone_ಬಹುಲಕ = 1;

	update_cone_ಬಹುಲಕ(&check_ನಿಯಮಗಳು_opts.cone_ಬಹುಲಕ);
	pl.use_cone_patterns = cfg->core_sparse_checkout_cone;
	ಆದರೆ (check_ನಿಯಮಗಳು_opts.ನಿಯಮಗಳು_file) {
		fp = xfopen(check_ನಿಯಮಗಳು_opts.ನಿಯಮಗಳು_file, "r");
		ಕೂಡಿಸು_patterns_from_ಒಳಹರಿವು(&pl, argc, argv, fp);
		fclose(fp);
	} ಇಲ್ಲದಿದ್ದರೆ {
		sparse_filename = get_sparse_checkout_filename();
		ಆದರೆ (ಕೂಡಿಸು_patterns_from_file_ಗಮ್ಯ_list(sparse_filename, "", 0, &pl,
						   ಖಾಲಿ, 0))
			die(_("unable to load existing sparse-checkout patterns"));
		ಬಿಡುಗಡೆ(sparse_filename);
	}

	ret = check_ನಿಯಮಗಳು(repo, &pl, check_ನಿಯಮಗಳು_opts.null_termination);
	clear_ನಮೂನೆ_list(&pl);
	ಬಿಡುಗಡೆ(check_ನಿಯಮಗಳು_opts.ನಿಯಮಗಳು_file);
	ಹಿಂತಿರುಗಿ ret;
}

ಪೂರ್ಣಾಂಕ cmd_sparse_checkout(ಪೂರ್ಣಾಂಕ argc,
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			ರಚನೆ repository *repo)
{
	parse_opt_subcommand_fn *fn = ಖಾಲಿ;
	ರಚನೆ option builtin_sparse_checkout_options[] = {
		OPT_SUBCOMMAND("list", &fn, sparse_checkout_list),
		OPT_SUBCOMMAND("init", &fn, sparse_checkout_init),
		OPT_SUBCOMMAND("set", &fn, sparse_checkout_ಗಣ),
		OPT_SUBCOMMAND("add", &fn, sparse_checkout_ಕೂಡಿಸು),
		OPT_SUBCOMMAND("reapply", &fn, sparse_checkout_reapply),
		OPT_SUBCOMMAND("clean", &fn, sparse_checkout_clean),
		OPT_SUBCOMMAND("disable", &fn, sparse_checkout_disable),
		OPT_SUBCOMMAND("check-rules", &fn, sparse_checkout_check_ನಿಯಮಗಳು),
		OPT_ಕೊನೆ(),
	};

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ,
			     builtin_sparse_checkout_options,
			     builtin_sparse_checkout_usage, 0);

	repo_config(the_repository, git_default_config, ಖಾಲಿ);

	prepare_repo_settings(repo);
	repo->settings.command_requires_ತುಂಬಿದ_ಸೂಚ್ಯಂಕ = 0;

	ಹಿಂತಿರುಗಿ fn(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, repo);
}
