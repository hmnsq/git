#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

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

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *empty_base = "";

ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ * ಧ್ರುವ builtin_sparse_checkout_usage[] = {
	N_("git sparse-checkout (init | list | set | add | reapply | disable | check-rules | clean) [<options>]"),
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಶೂನ್ಯ write_patterns_to_file(FILE *fp, ಸಂರಚನಾ pattern_list *pl)
{
	ಪೂರ್ಣಾಙ್ಕ i;

	ಪ್ರತಿ (i = 0; i < pl->nr; i++) {
		ಸಂರಚನಾ path_pattern *p = pl->patterns[i];

		ಯದಿ (p->flags & PATTERN_FLAG_NEGATIVE)
			fprintf(fp, "!");

		fprintf(fp, "%s", p->pattern);

		ಯದಿ (p->flags & PATTERN_FLAG_MUSTBEDIR)
			fprintf(fp, "/");

		fprintf(fp, "\n");
	}
}

ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ * ಧ್ರುವ builtin_sparse_checkout_list_usage[] = {
	"git sparse-checkout list",
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ sparse_checkout_list(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
				ಸಂರಚನಾ repository *repo UNUSED)
{
	ಸ್ಥಿರ ಸಂರಚನಾ option builtin_sparse_checkout_list_options[] = {
		OPT_END(),
	};
	ಸಂರಚನಾ pattern_list pl;
	ವರ್ಣ *sparse_filename;
	ಪೂರ್ಣಾಙ್ಕ res;
	ಸಂರಚನಾ repo_config_ಮೌಲ್ಯಗಳು *cfg = repo_config_ಮೌಲ್ಯಗಳು(the_repository);

	setup_work_ವೃಕ್ಷ(the_repository);
	ಯದಿ (!cfg->apply_sparse_checkout)
		die(_("this worktree is not sparse"));

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_list_options,
			     builtin_sparse_checkout_list_usage, 0);

	memset(&pl, 0, ಪರಿಮಾಣ(pl));

	pl.use_cone_patterns = cfg->core_sparse_checkout_cone;

	sparse_filename = get_sparse_checkout_filename();
	res = add_patterns_from_file_to_list(sparse_filename, "", 0, &pl, ರಿಕ್ತ, 0);
	ಮುಕ್ತ(sparse_filename);

	ಯದಿ (res < 0) {
		warning(_("this worktree is not sparse (sparse-checkout file may not exist)"));
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (pl.use_cone_patterns) {
		ಪೂರ್ಣಾಙ್ಕ i;
		ಸಂರಚನಾ pattern_entry *pe;
		ಸಂರಚನಾ hashmap_iter iter;
		ಸಂರಚನಾ string_list sl = STRING_LIST_INIT_DUP;

		hashmap_for_each_entry(&pl.recursive_hashmap, &iter, pe, ent) {
			/* pe->pattern starts with "/", skip it */
			string_list_append(&sl, pe->pattern + 1);
		}

		string_list_sort_u(&sl, 0);

		ಪ್ರತಿ (i = 0; i < sl.nr; i++) {
			quote_c_style(sl.ವಸ್ತುಗಳು[i].string, ರಿಕ್ತ, stdout, 0);
			printf("\n");
		}

		string_list_clear(&sl, 0);
	} ಅನ್ಯಥಾ {
		write_patterns_to_file(stdout, &pl);
	}

	clear_pattern_list(&pl);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ clean_tracked_sparse_directories(ಸಂರಚನಾ repository *r)
{
	ಪೂರ್ಣಾಙ್ಕ i, was_full = 0;
	ಸಂರಚನಾ strbuf path = STRBUF_INIT;
	size_t pathlen;
	ಸಂರಚನಾ string_list_item *item;
	ಸಂರಚನಾ string_list sparse_dirs = STRING_LIST_INIT_DUP;

	/*
	 * If we are not using cone mode patterns, then we cannot
	 * delete directories outside of the sparse cone.
	 */
	ಯದಿ (!r || !r->index || !r->worktree)
		ಪ್ರತ್ಯಾಗಮ;
	ಯದಿ (init_sparse_checkout_patterns(r->index) ||
	    !r->index->sparse_checkout_patterns->use_cone_patterns)
		ಪ್ರತ್ಯಾಗಮ;

	/*
	 * Use the sparse index as a data structure to assist finding
	 * directories that are safe to delete. This conversion to a
	 * sparse index will not delete directories that contain
	 * conflicted entries or submodules.
	 */
	ಯದಿ (r->index->sparse_index == INDEX_EXPANDED) {
		/*
		 * If something, such as a merge conflict or other concern,
		 * prevents us from converting to a sparse index, then do
		 * not try deleting files.
		 */
		ಯದಿ (convert_to_sparse(r->index, SPARSE_INDEX_MEMORY_ONLY))
			ಪ್ರತ್ಯಾಗಮ;
		was_full = 1;
	}

	strbuf_addstr(&path, r->worktree);
	strbuf_complete(&path, '/');
	pathlen = path.len;

	/*
	 * Collect directories that have gone out of scope but also
	 * exist on disk, so there is some work to be done. We need to
	 * store the entries in a list before exploring, since that might
	 * expand the sparse-index again.
	 */
	ಪ್ರತಿ (i = 0; i < r->index->cache_nr; i++) {
		ಸಂರಚನಾ cache_entry *ce = r->index->cache[i];

		ಯದಿ (S_ISSPARSEDIR(ce->ce_mode) &&
		    repo_file_exists(r, ce->name))
			string_list_append(&sparse_dirs, ce->name);
	}

	for_each_string_list_item(item, &sparse_dirs) {
		ಸಂರಚನಾ dir_struct dir = DIR_INIT;
		ಸಂರಚನಾ pathspec p = { 0 };
		ಸಂರಚನಾ strvec s = STRVEC_INIT;

		strbuf_setlen(&path, pathlen);
		strbuf_addstr(&path, item->string);

		dir.flags |= DIR_SHOW_IGNORED_TOO;

		setup_standard_excludes(&dir);
		strvec_ತಳ್ಳು(&s, path.buf);

		parse_pathspec(&p, PATHSPEC_GLOB, 0, ರಿಕ್ತ, s.v);
		fill_directory(&dir, r->index, &p);

		ಯದಿ (dir.nr) {
			warning(_("directory '%s' contains untracked files,"
				  " but is not in the sparse-checkout cone"),
				item->string);
		} ಅನ್ಯಥಾ ಯದಿ (remove_dir_recursively(&path, 0)) {
			/*
			 * Removal is "best effort". If something blocks
			 * the deletion, then continue with a warning.
			 */
			warning(_("failed to remove directory '%s'"),
				item->string);
		}

		strvec_clear(&s);
		clear_pathspec(&p);
		dir_clear(&dir);
	}

	string_list_clear(&sparse_dirs, 0);
	strbuf_release(&path);

	ಯದಿ (was_full)
		ensure_full_index(r->index);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ update_working_directory(ಸಂರಚನಾ repository *r,
				    ಸಂರಚನಾ pattern_list *pl)
{
	ಗಣನಾ update_sparsity_result result;
	ಸಂರಚನಾ unpack_trees_options o;
	ಸಂರಚನಾ lock_file lock_file = LOCK_INIT;
	ಸಂರಚನಾ pattern_list *old_pl;

	/* If no branch has been checked out, there are no updates to make. */
	ಯದಿ (is_index_unborn(r->index))
		ಪ್ರತ್ಯಾಗಮ UPDATE_SPARSITY_SUCCESS;

	old_pl = r->index->sparse_checkout_patterns;
	r->index->sparse_checkout_patterns = pl;

	memset(&o, 0, ಪರಿಮಾಣ(o));
	o.verbose_update = isatty(2);
	o.update = 1;
	o.head_idx = -1;
	o.src_index = r->index;
	o.dst_index = r->index;
	o.skip_sparse_checkout = 0;

	setup_work_ವೃಕ್ಷ(the_repository);

	repo_hold_locked_index(r, &lock_file, LOCK_DIE_ON_ERROR);

	setup_unpack_trees_porcelain(&o, "sparse-checkout");
	result = update_sparsity(&o, pl);
	clear_unpack_trees_porcelain(&o);

	ಯದಿ (result == UPDATE_SPARSITY_WARNINGS)
		/*
		 * We don't do any special handling of warnings from untracked
		 * files in the way or dirty entries that can't be removed.
		 */
		result = UPDATE_SPARSITY_SUCCESS;
	ಯದಿ (result == UPDATE_SPARSITY_SUCCESS)
		write_locked_index(r->index, &lock_file, COMMIT_LOCK);
	ಅನ್ಯಥಾ
		rollback_lock_file(&lock_file);

	clean_tracked_sparse_directories(r);

	ಯದಿ (r->index->sparse_checkout_patterns != pl) {
		clear_pattern_list(r->index->sparse_checkout_patterns);
		FREE_AND_NULL(r->index->sparse_checkout_patterns);
	}
	r->index->sparse_checkout_patterns = old_pl;

	ಪ್ರತ್ಯಾಗಮ result;
}

ಸ್ಥಿರ ವರ್ಣ *escaped_pattern(ವರ್ಣ *pattern)
{
	ವರ್ಣ *p = pattern;
	ಸಂರಚನಾ strbuf final = STRBUF_INIT;

	ಯಾವತ್ (*p) {
		ಯದಿ (is_glob_special(*p))
			strbuf_addch(&final, '\\');

		strbuf_addch(&final, *p);
		p++;
	}

	ಪ್ರತ್ಯಾಗಮ strbuf_detach(&final, ರಿಕ್ತ);
}

ಸ್ಥಿರ ಶೂನ್ಯ write_cone_to_file(FILE *fp, ಸಂರಚನಾ pattern_list *pl)
{
	ಪೂರ್ಣಾಙ್ಕ i;
	ಸಂರಚನಾ pattern_entry *pe;
	ಸಂರಚನಾ hashmap_iter iter;
	ಸಂರಚನಾ string_list sl = STRING_LIST_INIT_DUP;
	ಸಂರಚನಾ strbuf parent_pattern = STRBUF_INIT;

	hashmap_for_each_entry(&pl->parent_hashmap, &iter, pe, ent) {
		ಯದಿ (hashmap_get_entry(&pl->recursive_hashmap, pe, ent, ರಿಕ್ತ))
			ಅನುವರ್ತ;

		ಯದಿ (!hashmap_contains_parent(&pl->recursive_hashmap,
					     pe->pattern,
					     &parent_pattern))
			string_list_append(&sl, pe->pattern);
	}

	string_list_sort_u(&sl, 0);

	fprintf(fp, "/*\n!/*/\n");

	ಪ್ರತಿ (i = 0; i < sl.nr; i++) {
		ವರ್ಣ *pattern = escaped_pattern(sl.ವಸ್ತುಗಳು[i].string);

		ಯದಿ (strlen(pattern))
			fprintf(fp, "%s/\n!%s/*/\n", pattern, pattern);
		ಮುಕ್ತ(pattern);
	}

	string_list_clear(&sl, 0);

	hashmap_for_each_entry(&pl->recursive_hashmap, &iter, pe, ent) {
		ಯದಿ (!hashmap_contains_parent(&pl->recursive_hashmap,
					     pe->pattern,
					     &parent_pattern))
			string_list_append(&sl, pe->pattern);
	}

	strbuf_release(&parent_pattern);

	string_list_sort_u(&sl, 0);

	ಪ್ರತಿ (i = 0; i < sl.nr; i++) {
		ವರ್ಣ *pattern = escaped_pattern(sl.ವಸ್ತುಗಳು[i].string);
		fprintf(fp, "%s/\n", pattern);
		ಮುಕ್ತ(pattern);
	}

	string_list_clear(&sl, 0);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ write_patterns_and_update(ಸಂರಚನಾ repository *repo,
				     ಸಂರಚನಾ pattern_list *pl)
{
	ವರ್ಣ *sparse_filename;
	FILE *fp;
	ಸಂರಚನಾ lock_file lk = LOCK_INIT;
	ಪೂರ್ಣಾಙ್ಕ result;
	ಸಂರಚನಾ repo_config_ಮೌಲ್ಯಗಳು *cfg = repo_config_ಮೌಲ್ಯಗಳು(the_repository);

	sparse_filename = get_sparse_checkout_filename();

	ಯದಿ (safe_create_leading_directories(repo, sparse_filename))
		die(_("failed to create directory for sparse-checkout file"));

	repo_hold_lock_file_for_update(repo, &lk, sparse_filename,
				       LOCK_DIE_ON_ERROR);

	result = update_working_directory(repo, pl);
	ಯದಿ (result) {
		rollback_lock_file(&lk);
		update_working_directory(repo, ರಿಕ್ತ);
		ಗಚ್ಛ out;
	}

	fp = fdopen_lock_file(&lk, "w");
	ಯದಿ (!fp)
		die_errno(_("unable to fdopen %s"), get_lock_file_path(&lk));

	ಯದಿ (cfg->core_sparse_checkout_cone)
		write_cone_to_file(fp, pl);
	ಅನ್ಯಥಾ
		write_patterns_to_file(fp, pl);

	ಯದಿ (commit_lock_file(&lk))
		die_errno(_("unable to write %s"), sparse_filename);

out:
	clear_pattern_list(pl);
	ಮುಕ್ತ(sparse_filename);
	ಪ್ರತ್ಯಾಗಮ result;
}

ಗಣನಾ sparse_checkout_mode {
	MODE_NO_PATTERNS = 0,
	MODE_ALL_PATTERNS = 1,
	MODE_CONE_PATTERNS = 2,
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ set_config(ಸಂರಚನಾ repository *repo,
		      ಗಣನಾ sparse_checkout_mode mode)
{
	/* Update to use worktree config, if not already. */
	ಯದಿ (init_worktree_config(repo)) {
		error(_("failed to initialize worktree config"));
		ಪ್ರತ್ಯಾಗಮ 1;
	}

	ಯದಿ (repo_config_set_worktree_gently(repo,
					    "core.sparseCheckout",
					    mode ? "true" : "false") ||
	    repo_config_set_worktree_gently(repo,
					    "core.sparseCheckoutCone",
					    mode == MODE_CONE_PATTERNS ?
						"true" : "false"))
		ಪ್ರತ್ಯಾಗಮ 1;

	ಯದಿ (mode == MODE_NO_PATTERNS)
		ಪ್ರತ್ಯಾಗಮ set_sparse_index_config(repo, 0);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಗಣನಾ sparse_checkout_mode update_cone_mode(ಪೂರ್ಣಾಙ್ಕ *cone_mode) {
	ಸಂರಚನಾ repo_config_ಮೌಲ್ಯಗಳು *cfg = repo_config_ಮೌಲ್ಯಗಳು(the_repository);

	/* If not specified, use previous definition of cone mode */
	ಯದಿ (*cone_mode == -1 && cfg->apply_sparse_checkout)
		*cone_mode = cfg->core_sparse_checkout_cone;

	/* Set cone/non-cone mode appropriately */
	cfg->apply_sparse_checkout = 1;
	ಯದಿ (*cone_mode == 1 || *cone_mode == -1) {
		cfg->core_sparse_checkout_cone = 1;
		ಪ್ರತ್ಯಾಗಮ MODE_CONE_PATTERNS;
	}
	cfg->core_sparse_checkout_cone = 0;
	ಪ್ರತ್ಯಾಗಮ MODE_ALL_PATTERNS;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ update_modes(ಸಂರಚನಾ repository *repo, ಪೂರ್ಣಾಙ್ಕ *cone_mode, ಪೂರ್ಣಾಙ್ಕ *sparse_index)
{
	ಪೂರ್ಣಾಙ್ಕ mode, record_mode;
	ಸಂರಚನಾ repo_config_ಮೌಲ್ಯಗಳು *cfg = repo_config_ಮೌಲ್ಯಗಳು(the_repository);

	/* Determine if we need to record the mode; ensure sparse checkout on */
	record_mode = (*cone_mode != -1) || !cfg->apply_sparse_checkout;

	mode = update_cone_mode(cone_mode);
	ಯದಿ (record_mode && set_config(repo, mode))
		ಪ್ರತ್ಯಾಗಮ 1;

	/* Set sparse-index/non-sparse-index mode if specified */
	ಯದಿ (*sparse_index >= 0) {
		ಯದಿ (set_sparse_index_config(repo, *sparse_index) < 0)
			die(_("failed to modify sparse-index config"));

		/* force an index rewrite */
		repo_read_index(repo);
		repo->index->updated_workdir = 1;

		ಯದಿ (!*sparse_index)
			ensure_full_index(repo->index);
	}

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ * ಧ್ರುವ builtin_sparse_checkout_init_usage[] = {
	"git sparse-checkout init [--cone] [--[no-]sparse-index]",
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಸಂರಚನಾ sparse_checkout_init_opts {
	ಪೂರ್ಣಾಙ್ಕ cone_mode;
	ಪೂರ್ಣಾಙ್ಕ sparse_index;
} init_opts;

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ sparse_checkout_init(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
				ಸಂರಚನಾ repository *repo)
{
	ಸಂರಚನಾ pattern_list pl;
	ವರ್ಣ *sparse_filename;
	ಪೂರ್ಣಾಙ್ಕ res;
	ಸಂರಚನಾ object_id oid;

	ಸ್ಥಿರ ಸಂರಚನಾ option builtin_sparse_checkout_init_options[] = {
		OPT_BOOL(0, "cone", &init_opts.cone_mode,
			 N_("initialize the sparse-checkout in cone mode")),
		OPT_BOOL(0, "sparse-index", &init_opts.sparse_index,
			 N_("toggle the use of a sparse index")),
		OPT_END(),
	};

	setup_work_ವೃಕ್ಷ(the_repository);
	repo_read_index(repo);

	init_opts.cone_mode = -1;
	init_opts.sparse_index = -1;

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_init_options,
			     builtin_sparse_checkout_init_usage, 0);

	ಯದಿ (update_modes(repo, &init_opts.cone_mode, &init_opts.sparse_index))
		ಪ್ರತ್ಯಾಗಮ 1;

	memset(&pl, 0, ಪರಿಮಾಣ(pl));

	sparse_filename = get_sparse_checkout_filename();
	res = add_patterns_from_file_to_list(sparse_filename, "", 0, &pl, ರಿಕ್ತ, 0);

	/* If we already have a sparse-checkout file, use it. */
	ಯದಿ (res >= 0) {
		ಮುಕ್ತ(sparse_filename);
		clear_pattern_list(&pl);
		ಪ್ರತ್ಯಾಗಮ update_working_directory(repo, ರಿಕ್ತ);
	}

	ಯದಿ (repo_get_oid(repo, "HEAD", &oid)) {
		FILE *fp;

		/* assume we are in a fresh repo, but update the sparse-checkout file */
		ಯದಿ (safe_create_leading_directories(repo, sparse_filename))
			die(_("unable to create leading directories of %s"),
			    sparse_filename);
		fp = xfopen(sparse_filename, "w");
		ಯದಿ (!fp)
			die(_("failed to open '%s'"), sparse_filename);

		ಮುಕ್ತ(sparse_filename);
		fprintf(fp, "/*\n!/*/\n");
		fclose(fp);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಮುಕ್ತ(sparse_filename);

	add_pattern("/*", empty_base, 0, &pl, 0);
	add_pattern("!/*/", empty_base, 0, &pl, 0);
	pl.use_cone_patterns = init_opts.cone_mode;

	ಪ್ರತ್ಯಾಗಮ write_patterns_and_update(repo, &pl);
}

ಸ್ಥಿರ ಶೂನ್ಯ ಸೇರಿಸು_recursive_pattern(ಸಂರಚನಾ pattern_list *pl, ಸಂರಚನಾ strbuf *path)
{
	ಸಂರಚನಾ pattern_entry *e = xmalloc(ಪರಿಮಾಣ(*e));
	e->patternlen = path->len;
	e->pattern = strbuf_detach(path, ರಿಕ್ತ);
	hashmap_entry_init(&e->ent, fspathhash(e->pattern));

	hashmap_add(&pl->recursive_hashmap, &e->ent);

	ಯಾವತ್ (e->patternlen) {
		ವರ್ಣ *slash = strrchr(e->pattern, '/');
		ವರ್ಣ *oldpattern = e->pattern;
		size_t newlen;
		ಸಂರಚನಾ pattern_entry *dup;

		ಯದಿ (!slash || slash == e->pattern)
			ಭಙ್ಗ;

		newlen = slash - e->pattern;
		e = xmalloc(ಪರಿಮಾಣ(ಸಂರಚನಾ pattern_entry));
		e->patternlen = newlen;
		e->pattern = xstrndup(oldpattern, newlen);
		hashmap_entry_init(&e->ent, fspathhash(e->pattern));

		dup = hashmap_get_entry(&pl->parent_hashmap, e, ent, ರಿಕ್ತ);
		ಯದಿ (!dup) {
			hashmap_add(&pl->parent_hashmap, &e->ent);
		} ಅನ್ಯಥಾ {
			ಮುಕ್ತ(e->pattern);
			ಮುಕ್ತ(e);
			e = dup;
		}
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ strbuf_to_cone_pattern(ಸಂರಚನಾ strbuf *ಸಾಲು, ಸಂರಚನಾ pattern_list *pl)
{
	strbuf_trim(ಸಾಲು);

	strbuf_trim_trailing_dir_sep(ಸಾಲು);

	ಯದಿ (strbuf_normalize_path(ಸಾಲು))
		die(_("could not normalize path %s"), ಸಾಲು->buf);

	ಯದಿ (!ಸಾಲು->len)
		ಪ್ರತ್ಯಾಗಮ;

	ಯದಿ (ಸಾಲು->buf[0] != '/')
		strbuf_insertstr(ಸಾಲು, 0, "/");

	ಸೇರಿಸು_recursive_pattern(pl, ಸಾಲು);
}

ಸ್ಥಿರ ಶೂನ್ಯ add_patterns_from_ಒಳಹರಿವು(ಸಂರಚನಾ pattern_list *pl,
				    ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv,
				    FILE *file)
{
	ಪೂರ್ಣಾಙ್ಕ i;
	ಸಂರಚನಾ repo_config_ಮೌಲ್ಯಗಳು *cfg = repo_config_ಮೌಲ್ಯಗಳು(the_repository);

	ಯದಿ (cfg->core_sparse_checkout_cone) {
		ಸಂರಚನಾ strbuf ಸಾಲು = STRBUF_INIT;

		hashmap_init(&pl->recursive_hashmap, pl_hashmap_cmp, ರಿಕ್ತ, 0);
		hashmap_init(&pl->parent_hashmap, pl_hashmap_cmp, ರಿಕ್ತ, 0);
		pl->use_cone_patterns = 1;

		ಯದಿ (file) {
			ಸಂರಚನಾ strbuf unquoted = STRBUF_INIT;
			ಯಾವತ್ (!strbuf_getline(&ಸಾಲು, file)) {
				ಯದಿ (ಸಾಲು.buf[0] == '"') {
					strbuf_reset(&unquoted);
					ಯದಿ (unquote_c_style(&unquoted, ಸಾಲು.buf, ರಿಕ್ತ))
						die(_("unable to unquote C-style string '%s'"),
						ಸಾಲು.buf);

					strbuf_swap(&unquoted, &ಸಾಲು);
				}

				strbuf_to_cone_pattern(&ಸಾಲು, pl);
			}

			strbuf_release(&unquoted);
		} ಅನ್ಯಥಾ {
			ಪ್ರತಿ (i = 0; i < argc; i++) {
				strbuf_setlen(&ಸಾಲು, 0);
				strbuf_addstr(&ಸಾಲು, argv[i]);
				strbuf_to_cone_pattern(&ಸಾಲು, pl);
			}
		}
		strbuf_release(&ಸಾಲು);
	} ಅನ್ಯಥಾ {
		ಯದಿ (file) {
			ಸಂರಚನಾ strbuf ಸಾಲು = STRBUF_INIT;

			ಯಾವತ್ (!strbuf_getline(&ಸಾಲು, file))
				add_pattern(ಸಾಲು.buf, empty_base, 0, pl, 0);

			strbuf_release(&ಸಾಲು);
		} ಅನ್ಯಥಾ {
			ಪ್ರತಿ (i = 0; i < argc; i++)
				add_pattern(argv[i], empty_base, 0, pl, 0);
		}
	}
}

ಗಣನಾ modify_type {
	REPLACE,
	ADD,
};

ಸ್ಥಿರ ಶೂನ್ಯ add_patterns_cone_mode(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv,
				   ಸಂರಚನಾ pattern_list *pl,
				   ಪೂರ್ಣಾಙ್ಕ use_stdin)
{
	ಸಂರಚನಾ strbuf buffer = STRBUF_INIT;
	ಸಂರಚನಾ pattern_entry *pe;
	ಸಂರಚನಾ hashmap_iter iter;
	ಸಂರಚನಾ pattern_list existing;
	ಸಂರಚನಾ repo_config_ಮೌಲ್ಯಗಳು *cfg = repo_config_ಮೌಲ್ಯಗಳು(the_repository);
	ವರ್ಣ *sparse_filename = get_sparse_checkout_filename();

	add_patterns_from_ಒಳಹರಿವು(pl, argc, argv,
				use_stdin ? stdin : ರಿಕ್ತ);

	memset(&existing, 0, ಪರಿಮಾಣ(existing));
	existing.use_cone_patterns = cfg->core_sparse_checkout_cone;

	ಯದಿ (add_patterns_from_file_to_list(sparse_filename, "", 0,
					   &existing, ರಿಕ್ತ, 0))
		die(_("unable to load existing sparse-checkout patterns"));
	ಮುಕ್ತ(sparse_filename);

	ಯದಿ (!existing.use_cone_patterns)
		die(_("existing sparse-checkout patterns do not use cone mode"));

	hashmap_for_each_entry(&existing.recursive_hashmap, &iter, pe, ent) {
		ಯದಿ (!hashmap_contains_parent(&pl->recursive_hashmap,
					pe->pattern, &buffer) ||
		    !hashmap_contains_parent(&pl->parent_hashmap,
					pe->pattern, &buffer)) {
			strbuf_reset(&buffer);
			strbuf_addstr(&buffer, pe->pattern);
			ಸೇರಿಸು_recursive_pattern(pl, &buffer);
		}
	}

	clear_pattern_list(&existing);
	strbuf_release(&buffer);
}

ಸ್ಥಿರ ಶೂನ್ಯ add_patterns_literal(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv,
				 ಸಂರಚನಾ pattern_list *pl,
				 ಪೂರ್ಣಾಙ್ಕ use_stdin)
{
	ವರ್ಣ *sparse_filename = get_sparse_checkout_filename();
	ಯದಿ (add_patterns_from_file_to_list(sparse_filename, "", 0,
					   pl, ರಿಕ್ತ, 0))
		die(_("unable to load existing sparse-checkout patterns"));
	ಮುಕ್ತ(sparse_filename);
	add_patterns_from_ಒಳಹರಿವು(pl, argc, argv, use_stdin ? stdin : ರಿಕ್ತ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ modify_pattern_list(ಸಂರಚನಾ repository *repo,
			       ಸಂರಚನಾ strvec *args, ಪೂರ್ಣಾಙ್ಕ use_stdin,
			       ಗಣನಾ modify_type m)
{
	ಪೂರ್ಣಾಙ್ಕ result;
	ಪೂರ್ಣಾಙ್ಕ changed_config = 0;
	ಸಂರಚನಾ pattern_list *pl = xcalloc(1, ಪರಿಮಾಣ(*pl));
	ಸಂರಚನಾ repo_config_ಮೌಲ್ಯಗಳು *cfg = repo_config_ಮೌಲ್ಯಗಳು(the_repository);

	ವಿಕಲ್ಪ (m) {
	ಪಕ್ಷ ADD:
		ಯದಿ (cfg->core_sparse_checkout_cone)
			add_patterns_cone_mode(args->nr, args->v, pl, use_stdin);
		ಅನ್ಯಥಾ
			add_patterns_literal(args->nr, args->v, pl, use_stdin);
		ಭಙ್ಗ;

	ಪಕ್ಷ REPLACE:
		add_patterns_from_ಒಳಹರಿವು(pl, args->nr, args->v,
					use_stdin ? stdin : ರಿಕ್ತ);
		ಭಙ್ಗ;
	}

	ಯದಿ (!cfg->apply_sparse_checkout) {
		set_config(repo, MODE_ALL_PATTERNS);
		cfg->apply_sparse_checkout = 1;
		changed_config = 1;
	}

	result = write_patterns_and_update(repo, pl);

	ಯದಿ (result && changed_config)
		set_config(repo, MODE_NO_PATTERNS);

	clear_pattern_list(pl);
	ಮುಕ್ತ(pl);
	ಪ್ರತ್ಯಾಗಮ result;
}

ಸ್ಥಿರ ಶೂನ್ಯ sanitize_paths(ಸಂರಚನಾ repository *repo,
			   ಸಂರಚನಾ strvec *args,
			   ಧ್ರುವ ವರ್ಣ *prefix, ಪೂರ್ಣಾಙ್ಕ skip_checks)
{
	ಪೂರ್ಣಾಙ್ಕ i;
	ಸಂರಚನಾ repo_config_ಮೌಲ್ಯಗಳು *cfg = repo_config_ಮೌಲ್ಯಗಳು(the_repository);

	ಯದಿ (!args->nr)
		ಪ್ರತ್ಯಾಗಮ;

	ಯದಿ (prefix && *prefix && cfg->core_sparse_checkout_cone) {
		/*
		 * The args are not pathspecs, so unfortunately we
		 * cannot imitate how cmd_add() uses parse_pathspec().
		 */
		ಪೂರ್ಣಾಙ್ಕ prefix_len = strlen(prefix);

		ಪ್ರತಿ (i = 0; i < args->nr; i++) {
			ವರ್ಣ *prefixed_path = prefix_path(the_repository, prefix,
							  prefix_len, args->v[i]);
			strvec_replace(args, i, prefixed_path);
			ಮುಕ್ತ(prefixed_path);
		}
	}

	ಯದಿ (skip_checks)
		ಪ್ರತ್ಯಾಗಮ;

	ಯದಿ (prefix && *prefix && !cfg->core_sparse_checkout_cone)
		die(_("please run from the toplevel directory in non-cone mode"));

	ಯದಿ (cfg->core_sparse_checkout_cone) {
		ಪ್ರತಿ (i = 0; i < args->nr; i++) {
			ಯದಿ (args->v[i][0] == '/')
				die(_("specify directories rather than patterns (no leading slash)"));
			ಯದಿ (args->v[i][0] == '!')
				die(_("specify directories rather than patterns.  If your directory starts with a '!', pass --skip-checks"));
			ಯದಿ (strpbrk(args->v[i], "*?[]"))
				die(_("specify directories rather than patterns.  If your directory really has any of '*?[]\\' in it, pass --skip-checks"));
		}
	}

	ಪ್ರತಿ (i = 0; i < args->nr; i++) {
		ಸಂರಚನಾ cache_entry *ce;
		ಸಂರಚನಾ index_state *index = repo->index;
		ಪೂರ್ಣಾಙ್ಕ pos = index_name_pos(index, args->v[i], strlen(args->v[i]));

		ಯದಿ (pos < 0)
			ಅನುವರ್ತ;
		ce = index->cache[pos];
		ಯದಿ (S_ISSPARSEDIR(ce->ce_mode))
			ಅನುವರ್ತ;

		ಯದಿ (cfg->core_sparse_checkout_cone)
			die(_("'%s' is not a directory; to treat it as a directory anyway, rerun with --skip-checks"), args->v[i]);
		ಅನ್ಯಥಾ
			warning(_("pass a leading slash before paths such as '%s' if you want a single file (see NON-CONE PROBLEMS in the git-sparse-checkout manual)."), args->v[i]);
	}
}

ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ * ಧ್ರುವ builtin_sparse_checkout_add_usage[] = {
	N_("git sparse-checkout add [--skip-checks] (--stdin | <patterns>)"),
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಸಂರಚನಾ sparse_checkout_add_opts {
	ಪೂರ್ಣಾಙ್ಕ skip_checks;
	ಪೂರ್ಣಾಙ್ಕ use_stdin;
} add_opts;

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ sparse_checkout_add(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
			       ಸಂರಚನಾ repository *repo)
{
	ಸ್ಥಿರ ಸಂರಚನಾ option builtin_sparse_checkout_add_options[] = {
		OPT_BOOL_F(0, "skip-checks", &add_opts.skip_checks,
			   N_("skip some sanity checks on the given paths that might give false positives"),
			   PARSE_OPT_NONEG),
		OPT_BOOL(0, "stdin", &add_opts.use_stdin,
			 N_("read patterns from standard in")),
		OPT_END(),
	};
	ಸಂರಚನಾ strvec patterns = STRVEC_INIT;
	ಪೂರ್ಣಾಙ್ಕ ret;
	ಸಂರಚನಾ repo_config_ಮೌಲ್ಯಗಳು *cfg = repo_config_ಮೌಲ್ಯಗಳು(the_repository);

	setup_work_ವೃಕ್ಷ(the_repository);
	ಯದಿ (!cfg->apply_sparse_checkout)
		die(_("no sparse-checkout to add to"));

	repo_read_index(repo);

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_add_options,
			     builtin_sparse_checkout_add_usage, 0);

	ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < argc; i++)
		strvec_ತಳ್ಳು(&patterns, argv[i]);
	sanitize_paths(repo, &patterns, prefix, add_opts.skip_checks);

	ret = modify_pattern_list(repo, &patterns, add_opts.use_stdin, ADD);

	strvec_clear(&patterns);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ * ಧ್ರುವ builtin_sparse_checkout_set_usage[] = {
	N_("git sparse-checkout set [--[no-]cone] [--[no-]sparse-index] [--skip-checks] (--stdin | <patterns>)"),
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಸಂರಚನಾ sparse_checkout_set_opts {
	ಪೂರ್ಣಾಙ್ಕ cone_mode;
	ಪೂರ್ಣಾಙ್ಕ sparse_index;
	ಪೂರ್ಣಾಙ್ಕ skip_checks;
	ಪೂರ್ಣಾಙ್ಕ use_stdin;
} set_opts;

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ sparse_checkout_set(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
			       ಸಂರಚನಾ repository *repo)
{
	ಸಂರಚನಾ repo_config_ಮೌಲ್ಯಗಳು *cfg = repo_config_ಮೌಲ್ಯಗಳು(the_repository);
	ಪೂರ್ಣಾಙ್ಕ default_patterns_nr = 2;
	ಧ್ರುವ ವರ್ಣ *default_patterns[] = {"/*", "!/*/", ರಿಕ್ತ};

	ಸ್ಥಿರ ಸಂರಚನಾ option builtin_sparse_checkout_set_options[] = {
		OPT_BOOL(0, "cone", &set_opts.cone_mode,
			 N_("initialize the sparse-checkout in cone mode")),
		OPT_BOOL(0, "sparse-index", &set_opts.sparse_index,
			 N_("toggle the use of a sparse index")),
		OPT_BOOL_F(0, "skip-checks", &set_opts.skip_checks,
			   N_("skip some sanity checks on the given paths that might give false positives"),
			   PARSE_OPT_NONEG),
		OPT_BOOL_F(0, "stdin", &set_opts.use_stdin,
			   N_("read patterns from standard in"),
			   PARSE_OPT_NONEG),
		OPT_END(),
	};
	ಸಂರಚನಾ strvec patterns = STRVEC_INIT;
	ಪೂರ್ಣಾಙ್ಕ ret;

	setup_work_ವೃಕ್ಷ(the_repository);
	repo_read_index(repo);

	set_opts.cone_mode = -1;
	set_opts.sparse_index = -1;

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_set_options,
			     builtin_sparse_checkout_set_usage, 0);

	ಯದಿ (update_modes(repo, &set_opts.cone_mode, &set_opts.sparse_index))
		ಪ್ರತ್ಯಾಗಮ 1;

	/*
	 * Cone mode automatically specifies the toplevel directory.  For
	 * non-cone mode, if nothing is specified, manually select just the
	 * top-level directory (much as 'init' would do).
	 */
	ಯದಿ (!cfg->core_sparse_checkout_cone && !set_opts.use_stdin && argc == 0) {
		ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < default_patterns_nr; i++)
			strvec_ತಳ್ಳು(&patterns, default_patterns[i]);
	} ಅನ್ಯಥಾ {
		ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < argc; i++)
			strvec_ತಳ್ಳು(&patterns, argv[i]);
		sanitize_paths(repo, &patterns, prefix, set_opts.skip_checks);
	}

	ret = modify_pattern_list(repo, &patterns, set_opts.use_stdin, REPLACE);

	strvec_clear(&patterns);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ * ಧ್ರುವ builtin_sparse_checkout_reapply_usage[] = {
	"git sparse-checkout reapply [--[no-]cone] [--[no-]sparse-index]",
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಸಂರಚನಾ sparse_checkout_reapply_opts {
	ಪೂರ್ಣಾಙ್ಕ cone_mode;
	ಪೂರ್ಣಾಙ್ಕ sparse_index;
} reapply_opts;

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ sparse_checkout_reapply(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv,
				   ಧ್ರುವ ವರ್ಣ *prefix,
				   ಸಂರಚನಾ repository *repo)
{
	ಸ್ಥಿರ ಸಂರಚನಾ option builtin_sparse_checkout_reapply_options[] = {
		OPT_BOOL(0, "cone", &reapply_opts.cone_mode,
			 N_("initialize the sparse-checkout in cone mode")),
		OPT_BOOL(0, "sparse-index", &reapply_opts.sparse_index,
			 N_("toggle the use of a sparse index")),
		OPT_END(),
	};
	ಸಂರಚನಾ repo_config_ಮೌಲ್ಯಗಳು *cfg = repo_config_ಮೌಲ್ಯಗಳು(the_repository);

	setup_work_ವೃಕ್ಷ(the_repository);
	ಯದಿ (!cfg->apply_sparse_checkout)
		die(_("must be in a sparse-checkout to reapply sparsity patterns"));

	reapply_opts.cone_mode = -1;
	reapply_opts.sparse_index = -1;

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_reapply_options,
			     builtin_sparse_checkout_reapply_usage, 0);

	repo_read_index(repo);

	ಯದಿ (update_modes(repo, &reapply_opts.cone_mode, &reapply_opts.sparse_index))
		ಪ್ರತ್ಯಾಗಮ 1;

	ಪ್ರತ್ಯಾಗಮ update_working_directory(repo, ರಿಕ್ತ);
}

ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ * ಧ್ರುವ builtin_sparse_checkout_clean_usage[] = {
	"git sparse-checkout clean [-n|--dry-run]",
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ list_file_iterator(ಧ್ರುವ ವರ್ಣ *path, ಧ್ರುವ ಶೂನ್ಯ *data)
{
	ಧ್ರುವ ವರ್ಣ *ಸಂದೇಶ = data;

	printf(ಸಂದೇಶ, path);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ list_every_file_in_dir(ಧ್ರುವ ವರ್ಣ *ಸಂದೇಶ,
				   ಧ್ರುವ ವರ್ಣ *directory)
{
	ಸಂರಚನಾ strbuf path = STRBUF_INIT;

	strbuf_addstr(&path, directory);
	for_each_file_in_dir(&path, list_file_iterator, ಸಂದೇಶ);
	strbuf_release(&path);
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *ಸಂದೇಶ_remove = N_("Removing %s\n");
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *ಸಂದೇಶ_would_remove = N_("Would remove %s\n");

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ sparse_checkout_clean(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv,
				   ಧ್ರುವ ವರ್ಣ *prefix,
				   ಸಂರಚನಾ repository *repo)
{
	ಸಂರಚನಾ strbuf full_path = STRBUF_INIT;
	ಧ್ರುವ ವರ್ಣ *ಸಂದೇಶ = ಸಂದೇಶ_remove;
	size_t worktree_len;
	ಪೂರ್ಣಾಙ್ಕ force = 0, dry_run = 0, verbose = 0;
	ಪೂರ್ಣಾಙ್ಕ require_force = 1;
	ಸಂರಚನಾ repo_config_ಮೌಲ್ಯಗಳು *cfg = repo_config_ಮೌಲ್ಯಗಳು(the_repository);

	ಸಂರಚನಾ option builtin_sparse_checkout_clean_options[] = {
		OPT__DRY_RUN(&dry_run, N_("dry run")),
		OPT__FORCE(&force, N_("force"), PARSE_OPT_NOCOMPLETE),
		OPT__VERBOSE(&verbose, N_("report each affected file, not just directories")),
		OPT_END(),
	};

	setup_work_ವೃಕ್ಷ(the_repository);
	ಯದಿ (!cfg->apply_sparse_checkout)
		die(_("must be in a sparse-checkout to clean directories"));
	ಯದಿ (!cfg->core_sparse_checkout_cone)
		die(_("must be in a cone-mode sparse-checkout to clean directories"));

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_clean_options,
			     builtin_sparse_checkout_clean_usage, 0);

	repo_config_get_bool(repo, "clean.requireforce", &require_force);
	ಯದಿ (require_force && !force && !dry_run)
		die(_("for safety, refusing to clean without one of --force or --dry-run"));

	ಯದಿ (dry_run)
		ಸಂದೇಶ = ಸಂದೇಶ_would_remove;

	ಯದಿ (repo_read_index(repo) < 0)
		die(_("failed to read index"));

	ಯದಿ (convert_to_sparse(repo->index, SPARSE_INDEX_MEMORY_ONLY) ||
	    repo->index->sparse_index == INDEX_EXPANDED)
		die(_("failed to convert index to a sparse index; resolve merge conflicts and try again"));

	strbuf_addstr(&full_path, repo->worktree);
	strbuf_addch(&full_path, '/');
	worktree_len = full_path.len;

	ಪ್ರತಿ (size_t i = 0; i < repo->index->cache_nr; i++) {
		ಸಂರಚನಾ cache_entry *ce = repo->index->cache[i];
		ಯದಿ (!S_ISSPARSEDIR(ce->ce_mode))
			ಅನುವರ್ತ;
		strbuf_setlen(&full_path, worktree_len);
		strbuf_add(&full_path, ce->name, ce->ce_namelen);

		ಯದಿ (!is_directory(full_path.buf))
			ಅನುವರ್ತ;

		ಯದಿ (verbose)
			list_every_file_in_dir(ಸಂದೇಶ, ce->name);
		ಅನ್ಯಥಾ
			printf(ಸಂದೇಶ, ce->name);

		ಯದಿ (dry_run <= 0 &&
		    remove_dir_recursively(&full_path, 0))
			warning_errno(_("failed to remove '%s'"), ce->name);
	}

	strbuf_release(&full_path);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ * ಧ್ರುವ builtin_sparse_checkout_disable_usage[] = {
	"git sparse-checkout disable",
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ sparse_checkout_disable(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv,
				   ಧ್ರುವ ವರ್ಣ *prefix,
				   ಸಂರಚನಾ repository *repo)
{
	ಸ್ಥಿರ ಸಂರಚನಾ option builtin_sparse_checkout_disable_options[] = {
		OPT_END(),
	};
	ಸಂರಚನಾ pattern_list pl;
	ಸಂರಚನಾ repo_config_ಮೌಲ್ಯಗಳು *cfg = repo_config_ಮೌಲ್ಯಗಳು(the_repository);

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

	setup_work_ವೃಕ್ಷ(the_repository);
	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_disable_options,
			     builtin_sparse_checkout_disable_usage, 0);

	/*
	 * Disable the advice message for expanding a sparse index, as we
	 * are expecting to do that when disabling sparse-checkout.
	 */
	give_advice_on_expansion = 0;
	repo_read_index(repo);

	memset(&pl, 0, ಪರಿಮಾಣ(pl));
	hashmap_init(&pl.recursive_hashmap, pl_hashmap_cmp, ರಿಕ್ತ, 0);
	hashmap_init(&pl.parent_hashmap, pl_hashmap_cmp, ರಿಕ್ತ, 0);
	pl.use_cone_patterns = 0;
	cfg->apply_sparse_checkout = 1;

	add_pattern("/*", empty_base, 0, &pl, 0);

	prepare_repo_settings(the_repository);
	repo->settings.sparse_index = 0;

	ಯದಿ (update_working_directory(repo, &pl))
		die(_("error while refreshing working directory"));

	clear_pattern_list(&pl);
	ಪ್ರತ್ಯಾಗಮ set_config(repo, MODE_NO_PATTERNS);
}

ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ * ಧ್ರುವ builtin_sparse_checkout_check_rules_usage[] = {
	N_("git sparse-checkout check-rules [-z] [--skip-checks]"
	   "[--[no-]cone] [--rules-file <file>]"),
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಸಂರಚನಾ sparse_checkout_check_rules_opts {
	ಪೂರ್ಣಾಙ್ಕ cone_mode;
	ಪೂರ್ಣಾಙ್ಕ null_termination;
	ವರ್ಣ *rules_file;
} check_rules_opts;

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ check_rules(ಸಂರಚನಾ repository *repo,
		       ಸಂರಚನಾ pattern_list *pl,
		       ಪೂರ್ಣಾಙ್ಕ null_terminated)
{
	ಸಂರಚನಾ strbuf ಸಾಲು = STRBUF_INIT;
	ಸಂರಚನಾ strbuf unquoted = STRBUF_INIT;
	ವರ್ಣ *path;
	ಪೂರ್ಣಾಙ್ಕ ಸಾಲು_terminator = null_terminated ? 0 : '\n';
	strbuf_getline_fn getline_fn = null_terminated ? strbuf_getline_nul
		: strbuf_getline;
	repo->index->sparse_checkout_patterns = pl;
	ಯಾವತ್ (!getline_fn(&ಸಾಲು, stdin)) {
		path = ಸಾಲು.buf;
		ಯದಿ (!null_terminated && ಸಾಲು.buf[0] == '"') {
			strbuf_reset(&unquoted);
			ಯದಿ (unquote_c_style(&unquoted, ಸಾಲು.buf, ರಿಕ್ತ))
				die(_("unable to unquote C-style string '%s'"),
					ಸಾಲು.buf);

			path = unquoted.buf;
		}

		ಯದಿ (path_in_sparse_checkout(path, repo->index))
			write_name_quoted(path, stdout, ಸಾಲು_terminator);
	}
	strbuf_release(&ಸಾಲು);
	strbuf_release(&unquoted);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ sparse_checkout_check_rules(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
				       ಸಂರಚನಾ repository *repo)
{
	ಸ್ಥಿರ ಸಂರಚನಾ option builtin_sparse_checkout_check_rules_options[] = {
		OPT_BOOL('z', ರಿಕ್ತ, &check_rules_opts.null_termination,
			 N_("terminate input and output files by a NUL character")),
		OPT_BOOL(0, "cone", &check_rules_opts.cone_mode,
			 N_("when used with --rules-file interpret patterns as cone mode patterns")),
		OPT_FILENAME(0, "rules-file", &check_rules_opts.rules_file,
			 N_("use patterns in <file> instead of the current ones.")),
		OPT_END(),
	};

	FILE *fp;
	ಪೂರ್ಣಾಙ್ಕ ret;
	ಸಂರಚನಾ pattern_list pl = {0};
	ಸಂರಚನಾ repo_config_ಮೌಲ್ಯಗಳು *cfg = repo_config_ಮೌಲ್ಯಗಳು(the_repository);
	ವರ್ಣ *sparse_filename;
	check_rules_opts.cone_mode = -1;

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_check_rules_options,
			     builtin_sparse_checkout_check_rules_usage, 0);

	ಯದಿ (check_rules_opts.rules_file && check_rules_opts.cone_mode < 0)
		check_rules_opts.cone_mode = 1;

	update_cone_mode(&check_rules_opts.cone_mode);
	pl.use_cone_patterns = cfg->core_sparse_checkout_cone;
	ಯದಿ (check_rules_opts.rules_file) {
		fp = xfopen(check_rules_opts.rules_file, "r");
		add_patterns_from_ಒಳಹರಿವು(&pl, argc, argv, fp);
		fclose(fp);
	} ಅನ್ಯಥಾ {
		sparse_filename = get_sparse_checkout_filename();
		ಯದಿ (add_patterns_from_file_to_list(sparse_filename, "", 0, &pl,
						   ರಿಕ್ತ, 0))
			die(_("unable to load existing sparse-checkout patterns"));
		ಮುಕ್ತ(sparse_filename);
	}

	ret = check_rules(repo, &pl, check_rules_opts.null_termination);
	clear_pattern_list(&pl);
	ಮುಕ್ತ(check_rules_opts.rules_file);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಪೂರ್ಣಾಙ್ಕ cmd_sparse_checkout(ಪೂರ್ಣಾಙ್ಕ argc,
			ಧ್ರುವ ವರ್ಣ **argv,
			ಧ್ರುವ ವರ್ಣ *prefix,
			ಸಂರಚನಾ repository *repo)
{
	parse_opt_subcommand_fn *fn = ರಿಕ್ತ;
	ಸಂರಚನಾ option builtin_sparse_checkout_options[] = {
		OPT_SUBCOMMAND("list", &fn, sparse_checkout_list),
		OPT_SUBCOMMAND("init", &fn, sparse_checkout_init),
		OPT_SUBCOMMAND("set", &fn, sparse_checkout_set),
		OPT_SUBCOMMAND("add", &fn, sparse_checkout_add),
		OPT_SUBCOMMAND("reapply", &fn, sparse_checkout_reapply),
		OPT_SUBCOMMAND("clean", &fn, sparse_checkout_clean),
		OPT_SUBCOMMAND("disable", &fn, sparse_checkout_disable),
		OPT_SUBCOMMAND("check-rules", &fn, sparse_checkout_check_rules),
		OPT_END(),
	};

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_options,
			     builtin_sparse_checkout_usage, 0);

	repo_config(the_repository, git_default_config, ರಿಕ್ತ);

	prepare_repo_settings(repo);
	repo->settings.command_requires_full_index = 0;

	ಪ್ರತ್ಯಾಗಮ fn(argc, argv, prefix, repo);
}
