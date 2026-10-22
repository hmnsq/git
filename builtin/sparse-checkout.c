#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "abspath.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "dir.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "object-file.h"
#অন্তর্ভাব "object-name.h"
#অন্তর্ভাব "parse-options.h"
#অন্তর্ভাব "path.h"
#অন্তর্ভাব "pathspec.h"
#অন্তর্ভাব "strbuf.h"
#অন্তর্ভাব "string-list.h"
#অন্তর্ভাব "lockfile.h"
#অন্তর্ভাব "unpack-trees.h"
#অন্তর্ভাব "quote.h"
#অন্তর্ভাব "setup.h"
#অন্তর্ভাব "sparse-index.h"
#অন্তর্ভাব "worktree.h"

স্থির ধ্রুবক অক্ষর *empty_base = "";

স্থির অক্ষর ধ্রুবক * ধ্রুবক builtin_sparse_checkout_usage[] = {
	N_("git sparse-checkout (init | list | set | add | reapply | disable | check-rules | clean) [<options>]"),
	ফাঁকা
};

স্থির শূন্য write_patterns_to_file(FILE *fp, কাঠামো pattern_list *pl)
{
	পূর্ণসংখ্যা i;

	জন্য (i = 0; i < pl->nr; i++) {
		কাঠামো path_pattern *p = pl->patterns[i];

		যদি (p->flags & PATTERN_FLAG_NEGATIVE)
			fprintf(fp, "!");

		fprintf(fp, "%s", p->pattern);

		যদি (p->flags & PATTERN_FLAG_MUSTBEDIR)
			fprintf(fp, "/");

		fprintf(fp, "\n");
	}
}

স্থির অক্ষর ধ্রুবক * ধ্রুবক builtin_sparse_checkout_list_usage[] = {
	"git sparse-checkout list",
	ফাঁকা
};

স্থির পূর্ণসংখ্যা sparse_checkout_list(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix,
				কাঠামো repository *repo UNUSED)
{
	স্থির কাঠামো option builtin_sparse_checkout_list_options[] = {
		OPT_END(),
	};
	কাঠামো pattern_list pl;
	অক্ষর *sparse_filename;
	পূর্ণসংখ্যা res;
	কাঠামো repo_config_মানসমূহ *cfg = repo_config_মানসমূহ(the_repository);

	setup_work_বৃক্ষ(the_repository);
	যদি (!cfg->apply_sparse_checkout)
		die(_("this worktree is not sparse"));

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_list_options,
			     builtin_sparse_checkout_list_usage, 0);

	memset(&pl, 0, আকার(pl));

	pl.use_cone_patterns = cfg->core_sparse_checkout_cone;

	sparse_filename = get_sparse_checkout_filename();
	res = add_patterns_from_file_to_list(sparse_filename, "", 0, &pl, ফাঁকা, 0);
	মুক্ত(sparse_filename);

	যদি (res < 0) {
		warning(_("this worktree is not sparse (sparse-checkout file may not exist)"));
		ফেরত 0;
	}

	যদি (pl.use_cone_patterns) {
		পূর্ণসংখ্যা i;
		কাঠামো pattern_entry *pe;
		কাঠামো hashmap_iter iter;
		কাঠামো string_list sl = STRING_LIST_INIT_DUP;

		hashmap_for_each_entry(&pl.recursive_hashmap, &iter, pe, ent) {
			/* pe->pattern starts with "/", skip it */
			string_list_append(&sl, pe->pattern + 1);
		}

		string_list_sort_u(&sl, 0);

		জন্য (i = 0; i < sl.nr; i++) {
			quote_c_style(sl.বস্তুসমূহ[i].string, ফাঁকা, stdout, 0);
			printf("\n");
		}

		string_list_clear(&sl, 0);
	} অন্যথা {
		write_patterns_to_file(stdout, &pl);
	}

	clear_pattern_list(&pl);

	ফেরত 0;
}

স্থির শূন্য clean_tracked_sparse_directories(কাঠামো repository *r)
{
	পূর্ণসংখ্যা i, was_full = 0;
	কাঠামো strbuf path = STRBUF_INIT;
	আকার_t pathlen;
	কাঠামো string_list_item *item;
	কাঠামো string_list sparse_dirs = STRING_LIST_INIT_DUP;

	/*
	 * If we are not using cone mode patterns, then we cannot
	 * delete directories outside of the sparse cone.
	 */
	যদি (!r || !r->সূচক || !r->worktree)
		ফেরত;
	যদি (init_sparse_checkout_patterns(r->সূচক) ||
	    !r->সূচক->sparse_checkout_patterns->use_cone_patterns)
		ফেরত;

	/*
	 * Use the sparse index as a data structure to assist finding
	 * directories that are safe to delete. This conversion to a
	 * sparse index will not delete directories that contain
	 * conflicted entries or submodules.
	 */
	যদি (r->সূচক->sparse_সূচক == সূচক_EXPANDED) {
		/*
		 * If something, such as a merge conflict or other concern,
		 * prevents us from converting to a sparse index, then do
		 * not try deleting files.
		 */
		যদি (convert_to_sparse(r->সূচক, SPARSE_সূচক_MEMORY_ONLY))
			ফেরত;
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
	জন্য (i = 0; i < r->সূচক->cache_nr; i++) {
		কাঠামো cache_entry *ce = r->সূচক->cache[i];

		যদি (S_ISSPARSEDIR(ce->ce_mode) &&
		    repo_file_exists(r, ce->name))
			string_list_append(&sparse_dirs, ce->name);
	}

	for_each_string_list_item(item, &sparse_dirs) {
		কাঠামো dir_struct dir = DIR_INIT;
		কাঠামো pathspec p = { 0 };
		কাঠামো strvec s = STRVEC_INIT;

		strbuf_setlen(&path, pathlen);
		strbuf_addstr(&path, item->string);

		dir.flags |= DIR_SHOW_IGNORED_TOO;

		setup_standard_excludes(&dir);
		strvec_ঠেলো(&s, path.buf);

		parse_pathspec(&p, PATHSPEC_GLOB, 0, ফাঁকা, s.v);
		fill_directory(&dir, r->সূচক, &p);

		যদি (dir.nr) {
			warning(_("directory '%s' contains untracked files,"
				  " but is not in the sparse-checkout cone"),
				item->string);
		} অন্যথা যদি (remove_dir_recursively(&path, 0)) {
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

	যদি (was_full)
		ensure_full_সূচক(r->সূচক);
}

স্থির পূর্ণসংখ্যা update_working_directory(কাঠামো repository *r,
				    কাঠামো pattern_list *pl)
{
	গণনা update_sparsity_ফলাফল ফলাফল;
	কাঠামো unpack_trees_options o;
	কাঠামো lock_file lock_file = LOCK_INIT;
	কাঠামো pattern_list *old_pl;

	/* If no branch has been checked out, there are no updates to make. */
	যদি (is_সূচক_unborn(r->সূচক))
		ফেরত UPDATE_SPARSITY_SUCCESS;

	old_pl = r->সূচক->sparse_checkout_patterns;
	r->সূচক->sparse_checkout_patterns = pl;

	memset(&o, 0, আকার(o));
	o.verbose_update = isatty(2);
	o.update = 1;
	o.শীর্ষ_idx = -1;
	o.src_সূচক = r->সূচক;
	o.dst_সূচক = r->সূচক;
	o.skip_sparse_checkout = 0;

	setup_work_বৃক্ষ(the_repository);

	repo_hold_locked_সূচক(r, &lock_file, LOCK_DIE_ON_ERROR);

	setup_unpack_trees_porcelain(&o, "sparse-checkout");
	ফলাফল = update_sparsity(&o, pl);
	clear_unpack_trees_porcelain(&o);

	যদি (ফলাফল == UPDATE_SPARSITY_WARNINGS)
		/*
		 * We don't do any special handling of warnings from untracked
		 * files in the way or dirty entries that can't be removed.
		 */
		ফলাফল = UPDATE_SPARSITY_SUCCESS;
	যদি (ফলাফল == UPDATE_SPARSITY_SUCCESS)
		write_locked_সূচক(r->সূচক, &lock_file, COMMIT_LOCK);
	অন্যথা
		rollback_lock_file(&lock_file);

	clean_tracked_sparse_directories(r);

	যদি (r->সূচক->sparse_checkout_patterns != pl) {
		clear_pattern_list(r->সূচক->sparse_checkout_patterns);
		FREE_AND_NULL(r->সূচক->sparse_checkout_patterns);
	}
	r->সূচক->sparse_checkout_patterns = old_pl;

	ফেরত ফলাফল;
}

স্থির অক্ষর *escaped_pattern(অক্ষর *pattern)
{
	অক্ষর *p = pattern;
	কাঠামো strbuf final = STRBUF_INIT;

	যখন (*p) {
		যদি (is_glob_special(*p))
			strbuf_addch(&final, '\\');

		strbuf_addch(&final, *p);
		p++;
	}

	ফেরত strbuf_detach(&final, ফাঁকা);
}

স্থির শূন্য write_cone_to_file(FILE *fp, কাঠামো pattern_list *pl)
{
	পূর্ণসংখ্যা i;
	কাঠামো pattern_entry *pe;
	কাঠামো hashmap_iter iter;
	কাঠামো string_list sl = STRING_LIST_INIT_DUP;
	কাঠামো strbuf parent_pattern = STRBUF_INIT;

	hashmap_for_each_entry(&pl->parent_hashmap, &iter, pe, ent) {
		যদি (hashmap_get_entry(&pl->recursive_hashmap, pe, ent, ফাঁকা))
			চালু;

		যদি (!hashmap_contains_parent(&pl->recursive_hashmap,
					     pe->pattern,
					     &parent_pattern))
			string_list_append(&sl, pe->pattern);
	}

	string_list_sort_u(&sl, 0);

	fprintf(fp, "/*\n!/*/\n");

	জন্য (i = 0; i < sl.nr; i++) {
		অক্ষর *pattern = escaped_pattern(sl.বস্তুসমূহ[i].string);

		যদি (strlen(pattern))
			fprintf(fp, "%s/\n!%s/*/\n", pattern, pattern);
		মুক্ত(pattern);
	}

	string_list_clear(&sl, 0);

	hashmap_for_each_entry(&pl->recursive_hashmap, &iter, pe, ent) {
		যদি (!hashmap_contains_parent(&pl->recursive_hashmap,
					     pe->pattern,
					     &parent_pattern))
			string_list_append(&sl, pe->pattern);
	}

	strbuf_release(&parent_pattern);

	string_list_sort_u(&sl, 0);

	জন্য (i = 0; i < sl.nr; i++) {
		অক্ষর *pattern = escaped_pattern(sl.বস্তুসমূহ[i].string);
		fprintf(fp, "%s/\n", pattern);
		মুক্ত(pattern);
	}

	string_list_clear(&sl, 0);
}

স্থির পূর্ণসংখ্যা write_patterns_and_update(কাঠামো repository *repo,
				     কাঠামো pattern_list *pl)
{
	অক্ষর *sparse_filename;
	FILE *fp;
	কাঠামো lock_file lk = LOCK_INIT;
	পূর্ণসংখ্যা ফলাফল;
	কাঠামো repo_config_মানসমূহ *cfg = repo_config_মানসমূহ(the_repository);

	sparse_filename = get_sparse_checkout_filename();

	যদি (safe_create_leading_directories(repo, sparse_filename))
		die(_("failed to create directory for sparse-checkout file"));

	repo_hold_lock_file_for_update(repo, &lk, sparse_filename,
				       LOCK_DIE_ON_ERROR);

	ফলাফল = update_working_directory(repo, pl);
	যদি (ফলাফল) {
		rollback_lock_file(&lk);
		update_working_directory(repo, ফাঁকা);
		যাও out;
	}

	fp = fdopen_lock_file(&lk, "w");
	যদি (!fp)
		die_errno(_("unable to fdopen %s"), get_lock_file_path(&lk));

	যদি (cfg->core_sparse_checkout_cone)
		write_cone_to_file(fp, pl);
	অন্যথা
		write_patterns_to_file(fp, pl);

	যদি (commit_lock_file(&lk))
		die_errno(_("unable to write %s"), sparse_filename);

out:
	clear_pattern_list(pl);
	মুক্ত(sparse_filename);
	ফেরত ফলাফল;
}

গণনা sparse_checkout_mode {
	MODE_NO_PATTERNS = 0,
	MODE_ALL_PATTERNS = 1,
	MODE_CONE_PATTERNS = 2,
};

স্থির পূর্ণসংখ্যা set_config(কাঠামো repository *repo,
		      গণনা sparse_checkout_mode mode)
{
	/* Update to use worktree config, if not already. */
	যদি (init_worktree_config(repo)) {
		error(_("failed to initialize worktree config"));
		ফেরত 1;
	}

	যদি (repo_config_set_worktree_gently(repo,
					    "core.sparseCheckout",
					    mode ? "true" : "false") ||
	    repo_config_set_worktree_gently(repo,
					    "core.sparseCheckoutCone",
					    mode == MODE_CONE_PATTERNS ?
						"true" : "false"))
		ফেরত 1;

	যদি (mode == MODE_NO_PATTERNS)
		ফেরত set_sparse_সূচক_config(repo, 0);

	ফেরত 0;
}

স্থির গণনা sparse_checkout_mode update_cone_mode(পূর্ণসংখ্যা *cone_mode) {
	কাঠামো repo_config_মানসমূহ *cfg = repo_config_মানসমূহ(the_repository);

	/* If not specified, use previous definition of cone mode */
	যদি (*cone_mode == -1 && cfg->apply_sparse_checkout)
		*cone_mode = cfg->core_sparse_checkout_cone;

	/* Set cone/non-cone mode appropriately */
	cfg->apply_sparse_checkout = 1;
	যদি (*cone_mode == 1 || *cone_mode == -1) {
		cfg->core_sparse_checkout_cone = 1;
		ফেরত MODE_CONE_PATTERNS;
	}
	cfg->core_sparse_checkout_cone = 0;
	ফেরত MODE_ALL_PATTERNS;
}

স্থির পূর্ণসংখ্যা update_modes(কাঠামো repository *repo, পূর্ণসংখ্যা *cone_mode, পূর্ণসংখ্যা *sparse_সূচক)
{
	পূর্ণসংখ্যা mode, record_mode;
	কাঠামো repo_config_মানসমূহ *cfg = repo_config_মানসমূহ(the_repository);

	/* Determine if we need to record the mode; ensure sparse checkout on */
	record_mode = (*cone_mode != -1) || !cfg->apply_sparse_checkout;

	mode = update_cone_mode(cone_mode);
	যদি (record_mode && set_config(repo, mode))
		ফেরত 1;

	/* Set sparse-index/non-sparse-index mode if specified */
	যদি (*sparse_সূচক >= 0) {
		যদি (set_sparse_সূচক_config(repo, *sparse_সূচক) < 0)
			die(_("failed to modify sparse-index config"));

		/* force an index rewrite */
		repo_read_সূচক(repo);
		repo->সূচক->updated_workdir = 1;

		যদি (!*sparse_সূচক)
			ensure_full_সূচক(repo->সূচক);
	}

	ফেরত 0;
}

স্থির অক্ষর ধ্রুবক * ধ্রুবক builtin_sparse_checkout_init_usage[] = {
	"git sparse-checkout init [--cone] [--[no-]sparse-index]",
	ফাঁকা
};

স্থির কাঠামো sparse_checkout_init_opts {
	পূর্ণসংখ্যা cone_mode;
	পূর্ণসংখ্যা sparse_সূচক;
} init_opts;

স্থির পূর্ণসংখ্যা sparse_checkout_init(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix,
				কাঠামো repository *repo)
{
	কাঠামো pattern_list pl;
	অক্ষর *sparse_filename;
	পূর্ণসংখ্যা res;
	কাঠামো object_id oid;

	স্থির কাঠামো option builtin_sparse_checkout_init_options[] = {
		OPT_BOOL(0, "cone", &init_opts.cone_mode,
			 N_("initialize the sparse-checkout in cone mode")),
		OPT_BOOL(0, "sparse-index", &init_opts.sparse_সূচক,
			 N_("toggle the use of a sparse index")),
		OPT_END(),
	};

	setup_work_বৃক্ষ(the_repository);
	repo_read_সূচক(repo);

	init_opts.cone_mode = -1;
	init_opts.sparse_সূচক = -1;

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_init_options,
			     builtin_sparse_checkout_init_usage, 0);

	যদি (update_modes(repo, &init_opts.cone_mode, &init_opts.sparse_সূচক))
		ফেরত 1;

	memset(&pl, 0, আকার(pl));

	sparse_filename = get_sparse_checkout_filename();
	res = add_patterns_from_file_to_list(sparse_filename, "", 0, &pl, ফাঁকা, 0);

	/* If we already have a sparse-checkout file, use it. */
	যদি (res >= 0) {
		মুক্ত(sparse_filename);
		clear_pattern_list(&pl);
		ফেরত update_working_directory(repo, ফাঁকা);
	}

	যদি (repo_get_oid(repo, "HEAD", &oid)) {
		FILE *fp;

		/* assume we are in a fresh repo, but update the sparse-checkout file */
		যদি (safe_create_leading_directories(repo, sparse_filename))
			die(_("unable to create leading directories of %s"),
			    sparse_filename);
		fp = xfopen(sparse_filename, "w");
		যদি (!fp)
			die(_("failed to open '%s'"), sparse_filename);

		মুক্ত(sparse_filename);
		fprintf(fp, "/*\n!/*/\n");
		fclose(fp);
		ফেরত 0;
	}

	মুক্ত(sparse_filename);

	add_pattern("/*", empty_base, 0, &pl, 0);
	add_pattern("!/*/", empty_base, 0, &pl, 0);
	pl.use_cone_patterns = init_opts.cone_mode;

	ফেরত write_patterns_and_update(repo, &pl);
}

স্থির শূন্য ঢোকাও_recursive_pattern(কাঠামো pattern_list *pl, কাঠামো strbuf *path)
{
	কাঠামো pattern_entry *e = xmalloc(আকার(*e));
	e->patternlen = path->len;
	e->pattern = strbuf_detach(path, ফাঁকা);
	hashmap_entry_init(&e->ent, fspathhash(e->pattern));

	hashmap_add(&pl->recursive_hashmap, &e->ent);

	যখন (e->patternlen) {
		অক্ষর *slash = strrchr(e->pattern, '/');
		অক্ষর *oldpattern = e->pattern;
		আকার_t newlen;
		কাঠামো pattern_entry *dup;

		যদি (!slash || slash == e->pattern)
			বিরতি;

		newlen = slash - e->pattern;
		e = xmalloc(আকার(কাঠামো pattern_entry));
		e->patternlen = newlen;
		e->pattern = xstrndup(oldpattern, newlen);
		hashmap_entry_init(&e->ent, fspathhash(e->pattern));

		dup = hashmap_get_entry(&pl->parent_hashmap, e, ent, ফাঁকা);
		যদি (!dup) {
			hashmap_add(&pl->parent_hashmap, &e->ent);
		} অন্যথা {
			মুক্ত(e->pattern);
			মুক্ত(e);
			e = dup;
		}
	}
}

স্থির শূন্য strbuf_to_cone_pattern(কাঠামো strbuf *সারি, কাঠামো pattern_list *pl)
{
	strbuf_trim(সারি);

	strbuf_trim_trailing_dir_sep(সারি);

	যদি (strbuf_normalize_path(সারি))
		die(_("could not normalize path %s"), সারি->buf);

	যদি (!সারি->len)
		ফেরত;

	যদি (সারি->buf[0] != '/')
		strbuf_insertstr(সারি, 0, "/");

	ঢোকাও_recursive_pattern(pl, সারি);
}

স্থির শূন্য add_patterns_from_প্রবেশ(কাঠামো pattern_list *pl,
				    পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv,
				    FILE *file)
{
	পূর্ণসংখ্যা i;
	কাঠামো repo_config_মানসমূহ *cfg = repo_config_মানসমূহ(the_repository);

	যদি (cfg->core_sparse_checkout_cone) {
		কাঠামো strbuf সারি = STRBUF_INIT;

		hashmap_init(&pl->recursive_hashmap, pl_hashmap_cmp, ফাঁকা, 0);
		hashmap_init(&pl->parent_hashmap, pl_hashmap_cmp, ফাঁকা, 0);
		pl->use_cone_patterns = 1;

		যদি (file) {
			কাঠামো strbuf unquoted = STRBUF_INIT;
			যখন (!strbuf_getline(&সারি, file)) {
				যদি (সারি.buf[0] == '"') {
					strbuf_reset(&unquoted);
					যদি (unquote_c_style(&unquoted, সারি.buf, ফাঁকা))
						die(_("unable to unquote C-style string '%s'"),
						সারি.buf);

					strbuf_swap(&unquoted, &সারি);
				}

				strbuf_to_cone_pattern(&সারি, pl);
			}

			strbuf_release(&unquoted);
		} অন্যথা {
			জন্য (i = 0; i < argc; i++) {
				strbuf_setlen(&সারি, 0);
				strbuf_addstr(&সারি, argv[i]);
				strbuf_to_cone_pattern(&সারি, pl);
			}
		}
		strbuf_release(&সারি);
	} অন্যথা {
		যদি (file) {
			কাঠামো strbuf সারি = STRBUF_INIT;

			যখন (!strbuf_getline(&সারি, file))
				add_pattern(সারি.buf, empty_base, 0, pl, 0);

			strbuf_release(&সারি);
		} অন্যথা {
			জন্য (i = 0; i < argc; i++)
				add_pattern(argv[i], empty_base, 0, pl, 0);
		}
	}
}

গণনা modify_type {
	REPLACE,
	ADD,
};

স্থির শূন্য add_patterns_cone_mode(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv,
				   কাঠামো pattern_list *pl,
				   পূর্ণসংখ্যা use_stdin)
{
	কাঠামো strbuf বাফার = STRBUF_INIT;
	কাঠামো pattern_entry *pe;
	কাঠামো hashmap_iter iter;
	কাঠামো pattern_list existing;
	কাঠামো repo_config_মানসমূহ *cfg = repo_config_মানসমূহ(the_repository);
	অক্ষর *sparse_filename = get_sparse_checkout_filename();

	add_patterns_from_প্রবেশ(pl, argc, argv,
				use_stdin ? stdin : ফাঁকা);

	memset(&existing, 0, আকার(existing));
	existing.use_cone_patterns = cfg->core_sparse_checkout_cone;

	যদি (add_patterns_from_file_to_list(sparse_filename, "", 0,
					   &existing, ফাঁকা, 0))
		die(_("unable to load existing sparse-checkout patterns"));
	মুক্ত(sparse_filename);

	যদি (!existing.use_cone_patterns)
		die(_("existing sparse-checkout patterns do not use cone mode"));

	hashmap_for_each_entry(&existing.recursive_hashmap, &iter, pe, ent) {
		যদি (!hashmap_contains_parent(&pl->recursive_hashmap,
					pe->pattern, &বাফার) ||
		    !hashmap_contains_parent(&pl->parent_hashmap,
					pe->pattern, &বাফার)) {
			strbuf_reset(&বাফার);
			strbuf_addstr(&বাফার, pe->pattern);
			ঢোকাও_recursive_pattern(pl, &বাফার);
		}
	}

	clear_pattern_list(&existing);
	strbuf_release(&বাফার);
}

স্থির শূন্য add_patterns_literal(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv,
				 কাঠামো pattern_list *pl,
				 পূর্ণসংখ্যা use_stdin)
{
	অক্ষর *sparse_filename = get_sparse_checkout_filename();
	যদি (add_patterns_from_file_to_list(sparse_filename, "", 0,
					   pl, ফাঁকা, 0))
		die(_("unable to load existing sparse-checkout patterns"));
	মুক্ত(sparse_filename);
	add_patterns_from_প্রবেশ(pl, argc, argv, use_stdin ? stdin : ফাঁকা);
}

স্থির পূর্ণসংখ্যা modify_pattern_list(কাঠামো repository *repo,
			       কাঠামো strvec *args, পূর্ণসংখ্যা use_stdin,
			       গণনা modify_type m)
{
	পূর্ণসংখ্যা ফলাফল;
	পূর্ণসংখ্যা changed_config = 0;
	কাঠামো pattern_list *pl = xcalloc(1, আকার(*pl));
	কাঠামো repo_config_মানসমূহ *cfg = repo_config_মানসমূহ(the_repository);

	নির্বাচন (m) {
	ক্ষেত্র ADD:
		যদি (cfg->core_sparse_checkout_cone)
			add_patterns_cone_mode(args->nr, args->v, pl, use_stdin);
		অন্যথা
			add_patterns_literal(args->nr, args->v, pl, use_stdin);
		বিরতি;

	ক্ষেত্র REPLACE:
		add_patterns_from_প্রবেশ(pl, args->nr, args->v,
					use_stdin ? stdin : ফাঁকা);
		বিরতি;
	}

	যদি (!cfg->apply_sparse_checkout) {
		set_config(repo, MODE_ALL_PATTERNS);
		cfg->apply_sparse_checkout = 1;
		changed_config = 1;
	}

	ফলাফল = write_patterns_and_update(repo, pl);

	যদি (ফলাফল && changed_config)
		set_config(repo, MODE_NO_PATTERNS);

	clear_pattern_list(pl);
	মুক্ত(pl);
	ফেরত ফলাফল;
}

স্থির শূন্য sanitize_paths(কাঠামো repository *repo,
			   কাঠামো strvec *args,
			   ধ্রুবক অক্ষর *prefix, পূর্ণসংখ্যা skip_checks)
{
	পূর্ণসংখ্যা i;
	কাঠামো repo_config_মানসমূহ *cfg = repo_config_মানসমূহ(the_repository);

	যদি (!args->nr)
		ফেরত;

	যদি (prefix && *prefix && cfg->core_sparse_checkout_cone) {
		/*
		 * The args are not pathspecs, so unfortunately we
		 * cannot imitate how cmd_add() uses parse_pathspec().
		 */
		পূর্ণসংখ্যা prefix_len = strlen(prefix);

		জন্য (i = 0; i < args->nr; i++) {
			অক্ষর *prefixed_path = prefix_path(the_repository, prefix,
							  prefix_len, args->v[i]);
			strvec_replace(args, i, prefixed_path);
			মুক্ত(prefixed_path);
		}
	}

	যদি (skip_checks)
		ফেরত;

	যদি (prefix && *prefix && !cfg->core_sparse_checkout_cone)
		die(_("please run from the toplevel directory in non-cone mode"));

	যদি (cfg->core_sparse_checkout_cone) {
		জন্য (i = 0; i < args->nr; i++) {
			যদি (args->v[i][0] == '/')
				die(_("specify directories rather than patterns (no leading slash)"));
			যদি (args->v[i][0] == '!')
				die(_("specify directories rather than patterns.  If your directory starts with a '!', pass --skip-checks"));
			যদি (strpbrk(args->v[i], "*?[]"))
				die(_("specify directories rather than patterns.  If your directory really has any of '*?[]\\' in it, pass --skip-checks"));
		}
	}

	জন্য (i = 0; i < args->nr; i++) {
		কাঠামো cache_entry *ce;
		কাঠামো সূচক_state *সূচক = repo->সূচক;
		পূর্ণসংখ্যা pos = সূচক_name_pos(সূচক, args->v[i], strlen(args->v[i]));

		যদি (pos < 0)
			চালু;
		ce = সূচক->cache[pos];
		যদি (S_ISSPARSEDIR(ce->ce_mode))
			চালু;

		যদি (cfg->core_sparse_checkout_cone)
			die(_("'%s' is not a directory; to treat it as a directory anyway, rerun with --skip-checks"), args->v[i]);
		অন্যথা
			warning(_("pass a leading slash before paths such as '%s' if you want a single file (see NON-CONE PROBLEMS in the git-sparse-checkout manual)."), args->v[i]);
	}
}

স্থির অক্ষর ধ্রুবক * ধ্রুবক builtin_sparse_checkout_add_usage[] = {
	N_("git sparse-checkout add [--skip-checks] (--stdin | <patterns>)"),
	ফাঁকা
};

স্থির কাঠামো sparse_checkout_add_opts {
	পূর্ণসংখ্যা skip_checks;
	পূর্ণসংখ্যা use_stdin;
} add_opts;

স্থির পূর্ণসংখ্যা sparse_checkout_add(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix,
			       কাঠামো repository *repo)
{
	স্থির কাঠামো option builtin_sparse_checkout_add_options[] = {
		OPT_BOOL_F(0, "skip-checks", &add_opts.skip_checks,
			   N_("skip some sanity checks on the given paths that might give false positives"),
			   PARSE_OPT_NONEG),
		OPT_BOOL(0, "stdin", &add_opts.use_stdin,
			 N_("read patterns from standard in")),
		OPT_END(),
	};
	কাঠামো strvec patterns = STRVEC_INIT;
	পূর্ণসংখ্যা ret;
	কাঠামো repo_config_মানসমূহ *cfg = repo_config_মানসমূহ(the_repository);

	setup_work_বৃক্ষ(the_repository);
	যদি (!cfg->apply_sparse_checkout)
		die(_("no sparse-checkout to add to"));

	repo_read_সূচক(repo);

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_add_options,
			     builtin_sparse_checkout_add_usage, 0);

	জন্য (পূর্ণসংখ্যা i = 0; i < argc; i++)
		strvec_ঠেলো(&patterns, argv[i]);
	sanitize_paths(repo, &patterns, prefix, add_opts.skip_checks);

	ret = modify_pattern_list(repo, &patterns, add_opts.use_stdin, ADD);

	strvec_clear(&patterns);
	ফেরত ret;
}

স্থির অক্ষর ধ্রুবক * ধ্রুবক builtin_sparse_checkout_set_usage[] = {
	N_("git sparse-checkout set [--[no-]cone] [--[no-]sparse-index] [--skip-checks] (--stdin | <patterns>)"),
	ফাঁকা
};

স্থির কাঠামো sparse_checkout_set_opts {
	পূর্ণসংখ্যা cone_mode;
	পূর্ণসংখ্যা sparse_সূচক;
	পূর্ণসংখ্যা skip_checks;
	পূর্ণসংখ্যা use_stdin;
} set_opts;

স্থির পূর্ণসংখ্যা sparse_checkout_set(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix,
			       কাঠামো repository *repo)
{
	কাঠামো repo_config_মানসমূহ *cfg = repo_config_মানসমূহ(the_repository);
	পূর্ণসংখ্যা default_patterns_nr = 2;
	ধ্রুবক অক্ষর *default_patterns[] = {"/*", "!/*/", ফাঁকা};

	স্থির কাঠামো option builtin_sparse_checkout_set_options[] = {
		OPT_BOOL(0, "cone", &set_opts.cone_mode,
			 N_("initialize the sparse-checkout in cone mode")),
		OPT_BOOL(0, "sparse-index", &set_opts.sparse_সূচক,
			 N_("toggle the use of a sparse index")),
		OPT_BOOL_F(0, "skip-checks", &set_opts.skip_checks,
			   N_("skip some sanity checks on the given paths that might give false positives"),
			   PARSE_OPT_NONEG),
		OPT_BOOL_F(0, "stdin", &set_opts.use_stdin,
			   N_("read patterns from standard in"),
			   PARSE_OPT_NONEG),
		OPT_END(),
	};
	কাঠামো strvec patterns = STRVEC_INIT;
	পূর্ণসংখ্যা ret;

	setup_work_বৃক্ষ(the_repository);
	repo_read_সূচক(repo);

	set_opts.cone_mode = -1;
	set_opts.sparse_সূচক = -1;

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_set_options,
			     builtin_sparse_checkout_set_usage, 0);

	যদি (update_modes(repo, &set_opts.cone_mode, &set_opts.sparse_সূচক))
		ফেরত 1;

	/*
	 * Cone mode automatically specifies the toplevel directory.  For
	 * non-cone mode, if nothing is specified, manually select just the
	 * top-level directory (much as 'init' would do).
	 */
	যদি (!cfg->core_sparse_checkout_cone && !set_opts.use_stdin && argc == 0) {
		জন্য (পূর্ণসংখ্যা i = 0; i < default_patterns_nr; i++)
			strvec_ঠেলো(&patterns, default_patterns[i]);
	} অন্যথা {
		জন্য (পূর্ণসংখ্যা i = 0; i < argc; i++)
			strvec_ঠেলো(&patterns, argv[i]);
		sanitize_paths(repo, &patterns, prefix, set_opts.skip_checks);
	}

	ret = modify_pattern_list(repo, &patterns, set_opts.use_stdin, REPLACE);

	strvec_clear(&patterns);
	ফেরত ret;
}

স্থির অক্ষর ধ্রুবক * ধ্রুবক builtin_sparse_checkout_reapply_usage[] = {
	"git sparse-checkout reapply [--[no-]cone] [--[no-]sparse-index]",
	ফাঁকা
};

স্থির কাঠামো sparse_checkout_reapply_opts {
	পূর্ণসংখ্যা cone_mode;
	পূর্ণসংখ্যা sparse_সূচক;
} reapply_opts;

স্থির পূর্ণসংখ্যা sparse_checkout_reapply(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv,
				   ধ্রুবক অক্ষর *prefix,
				   কাঠামো repository *repo)
{
	স্থির কাঠামো option builtin_sparse_checkout_reapply_options[] = {
		OPT_BOOL(0, "cone", &reapply_opts.cone_mode,
			 N_("initialize the sparse-checkout in cone mode")),
		OPT_BOOL(0, "sparse-index", &reapply_opts.sparse_সূচক,
			 N_("toggle the use of a sparse index")),
		OPT_END(),
	};
	কাঠামো repo_config_মানসমূহ *cfg = repo_config_মানসমূহ(the_repository);

	setup_work_বৃক্ষ(the_repository);
	যদি (!cfg->apply_sparse_checkout)
		die(_("must be in a sparse-checkout to reapply sparsity patterns"));

	reapply_opts.cone_mode = -1;
	reapply_opts.sparse_সূচক = -1;

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_reapply_options,
			     builtin_sparse_checkout_reapply_usage, 0);

	repo_read_সূচক(repo);

	যদি (update_modes(repo, &reapply_opts.cone_mode, &reapply_opts.sparse_সূচক))
		ফেরত 1;

	ফেরত update_working_directory(repo, ফাঁকা);
}

স্থির অক্ষর ধ্রুবক * ধ্রুবক builtin_sparse_checkout_clean_usage[] = {
	"git sparse-checkout clean [-n|--dry-run]",
	ফাঁকা
};

স্থির পূর্ণসংখ্যা list_file_iterator(ধ্রুবক অক্ষর *path, ধ্রুবক শূন্য *তথ্য)
{
	ধ্রুবক অক্ষর *বার্তা = তথ্য;

	printf(বার্তা, path);
	ফেরত 0;
}

স্থির শূন্য list_every_file_in_dir(ধ্রুবক অক্ষর *বার্তা,
				   ধ্রুবক অক্ষর *directory)
{
	কাঠামো strbuf path = STRBUF_INIT;

	strbuf_addstr(&path, directory);
	for_each_file_in_dir(&path, list_file_iterator, বার্তা);
	strbuf_release(&path);
}

স্থির ধ্রুবক অক্ষর *বার্তা_remove = N_("Removing %s\n");
স্থির ধ্রুবক অক্ষর *বার্তা_would_remove = N_("Would remove %s\n");

স্থির পূর্ণসংখ্যা sparse_checkout_clean(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv,
				   ধ্রুবক অক্ষর *prefix,
				   কাঠামো repository *repo)
{
	কাঠামো strbuf full_path = STRBUF_INIT;
	ধ্রুবক অক্ষর *বার্তা = বার্তা_remove;
	আকার_t worktree_len;
	পূর্ণসংখ্যা force = 0, dry_run = 0, verbose = 0;
	পূর্ণসংখ্যা require_force = 1;
	কাঠামো repo_config_মানসমূহ *cfg = repo_config_মানসমূহ(the_repository);

	কাঠামো option builtin_sparse_checkout_clean_options[] = {
		OPT__DRY_RUN(&dry_run, N_("dry run")),
		OPT__FORCE(&force, N_("force"), PARSE_OPT_NOCOMPLETE),
		OPT__VERBOSE(&verbose, N_("report each affected file, not just directories")),
		OPT_END(),
	};

	setup_work_বৃক্ষ(the_repository);
	যদি (!cfg->apply_sparse_checkout)
		die(_("must be in a sparse-checkout to clean directories"));
	যদি (!cfg->core_sparse_checkout_cone)
		die(_("must be in a cone-mode sparse-checkout to clean directories"));

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_clean_options,
			     builtin_sparse_checkout_clean_usage, 0);

	repo_config_get_bool(repo, "clean.requireforce", &require_force);
	যদি (require_force && !force && !dry_run)
		die(_("for safety, refusing to clean without one of --force or --dry-run"));

	যদি (dry_run)
		বার্তা = বার্তা_would_remove;

	যদি (repo_read_সূচক(repo) < 0)
		die(_("failed to read index"));

	যদি (convert_to_sparse(repo->সূচক, SPARSE_সূচক_MEMORY_ONLY) ||
	    repo->সূচক->sparse_সূচক == সূচক_EXPANDED)
		die(_("failed to convert index to a sparse index; resolve merge conflicts and try again"));

	strbuf_addstr(&full_path, repo->worktree);
	strbuf_addch(&full_path, '/');
	worktree_len = full_path.len;

	জন্য (আকার_t i = 0; i < repo->সূচক->cache_nr; i++) {
		কাঠামো cache_entry *ce = repo->সূচক->cache[i];
		যদি (!S_ISSPARSEDIR(ce->ce_mode))
			চালু;
		strbuf_setlen(&full_path, worktree_len);
		strbuf_add(&full_path, ce->name, ce->ce_namelen);

		যদি (!is_directory(full_path.buf))
			চালু;

		যদি (verbose)
			list_every_file_in_dir(বার্তা, ce->name);
		অন্যথা
			printf(বার্তা, ce->name);

		যদি (dry_run <= 0 &&
		    remove_dir_recursively(&full_path, 0))
			warning_errno(_("failed to remove '%s'"), ce->name);
	}

	strbuf_release(&full_path);
	ফেরত 0;
}

স্থির অক্ষর ধ্রুবক * ধ্রুবক builtin_sparse_checkout_disable_usage[] = {
	"git sparse-checkout disable",
	ফাঁকা
};

স্থির পূর্ণসংখ্যা sparse_checkout_disable(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv,
				   ধ্রুবক অক্ষর *prefix,
				   কাঠামো repository *repo)
{
	স্থির কাঠামো option builtin_sparse_checkout_disable_options[] = {
		OPT_END(),
	};
	কাঠামো pattern_list pl;
	কাঠামো repo_config_মানসমূহ *cfg = repo_config_মানসমূহ(the_repository);

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

	setup_work_বৃক্ষ(the_repository);
	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_disable_options,
			     builtin_sparse_checkout_disable_usage, 0);

	/*
	 * Disable the advice message for expanding a sparse index, as we
	 * are expecting to do that when disabling sparse-checkout.
	 */
	give_advice_on_expansion = 0;
	repo_read_সূচক(repo);

	memset(&pl, 0, আকার(pl));
	hashmap_init(&pl.recursive_hashmap, pl_hashmap_cmp, ফাঁকা, 0);
	hashmap_init(&pl.parent_hashmap, pl_hashmap_cmp, ফাঁকা, 0);
	pl.use_cone_patterns = 0;
	cfg->apply_sparse_checkout = 1;

	add_pattern("/*", empty_base, 0, &pl, 0);

	prepare_repo_settings(the_repository);
	repo->settings.sparse_সূচক = 0;

	যদি (update_working_directory(repo, &pl))
		die(_("error while refreshing working directory"));

	clear_pattern_list(&pl);
	ফেরত set_config(repo, MODE_NO_PATTERNS);
}

স্থির অক্ষর ধ্রুবক * ধ্রুবক builtin_sparse_checkout_check_rules_usage[] = {
	N_("git sparse-checkout check-rules [-z] [--skip-checks]"
	   "[--[no-]cone] [--rules-file <file>]"),
	ফাঁকা
};

স্থির কাঠামো sparse_checkout_check_rules_opts {
	পূর্ণসংখ্যা cone_mode;
	পূর্ণসংখ্যা null_termination;
	অক্ষর *rules_file;
} check_rules_opts;

স্থির পূর্ণসংখ্যা check_rules(কাঠামো repository *repo,
		       কাঠামো pattern_list *pl,
		       পূর্ণসংখ্যা null_terminated)
{
	কাঠামো strbuf সারি = STRBUF_INIT;
	কাঠামো strbuf unquoted = STRBUF_INIT;
	অক্ষর *path;
	পূর্ণসংখ্যা সারি_terminator = null_terminated ? 0 : '\n';
	strbuf_getline_fn getline_fn = null_terminated ? strbuf_getline_nul
		: strbuf_getline;
	repo->সূচক->sparse_checkout_patterns = pl;
	যখন (!getline_fn(&সারি, stdin)) {
		path = সারি.buf;
		যদি (!null_terminated && সারি.buf[0] == '"') {
			strbuf_reset(&unquoted);
			যদি (unquote_c_style(&unquoted, সারি.buf, ফাঁকা))
				die(_("unable to unquote C-style string '%s'"),
					সারি.buf);

			path = unquoted.buf;
		}

		যদি (path_in_sparse_checkout(path, repo->সূচক))
			write_name_quoted(path, stdout, সারি_terminator);
	}
	strbuf_release(&সারি);
	strbuf_release(&unquoted);

	ফেরত 0;
}

স্থির পূর্ণসংখ্যা sparse_checkout_check_rules(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix,
				       কাঠামো repository *repo)
{
	স্থির কাঠামো option builtin_sparse_checkout_check_rules_options[] = {
		OPT_BOOL('z', ফাঁকা, &check_rules_opts.null_termination,
			 N_("terminate input and output files by a NUL character")),
		OPT_BOOL(0, "cone", &check_rules_opts.cone_mode,
			 N_("when used with --rules-file interpret patterns as cone mode patterns")),
		OPT_FILENAME(0, "rules-file", &check_rules_opts.rules_file,
			 N_("use patterns in <file> instead of the current ones.")),
		OPT_END(),
	};

	FILE *fp;
	পূর্ণসংখ্যা ret;
	কাঠামো pattern_list pl = {0};
	কাঠামো repo_config_মানসমূহ *cfg = repo_config_মানসমূহ(the_repository);
	অক্ষর *sparse_filename;
	check_rules_opts.cone_mode = -1;

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_check_rules_options,
			     builtin_sparse_checkout_check_rules_usage, 0);

	যদি (check_rules_opts.rules_file && check_rules_opts.cone_mode < 0)
		check_rules_opts.cone_mode = 1;

	update_cone_mode(&check_rules_opts.cone_mode);
	pl.use_cone_patterns = cfg->core_sparse_checkout_cone;
	যদি (check_rules_opts.rules_file) {
		fp = xfopen(check_rules_opts.rules_file, "r");
		add_patterns_from_প্রবেশ(&pl, argc, argv, fp);
		fclose(fp);
	} অন্যথা {
		sparse_filename = get_sparse_checkout_filename();
		যদি (add_patterns_from_file_to_list(sparse_filename, "", 0, &pl,
						   ফাঁকা, 0))
			die(_("unable to load existing sparse-checkout patterns"));
		মুক্ত(sparse_filename);
	}

	ret = check_rules(repo, &pl, check_rules_opts.null_termination);
	clear_pattern_list(&pl);
	মুক্ত(check_rules_opts.rules_file);
	ফেরত ret;
}

পূর্ণসংখ্যা cmd_sparse_checkout(পূর্ণসংখ্যা argc,
			ধ্রুবক অক্ষর **argv,
			ধ্রুবক অক্ষর *prefix,
			কাঠামো repository *repo)
{
	parse_opt_subcommand_fn *fn = ফাঁকা;
	কাঠামো option builtin_sparse_checkout_options[] = {
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

	repo_config(the_repository, git_default_config, ফাঁকা);

	prepare_repo_settings(repo);
	repo->settings.command_requires_full_সূচক = 0;

	ফেরত fn(argc, argv, prefix, repo);
}
