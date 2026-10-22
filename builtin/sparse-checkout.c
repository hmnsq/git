#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ضمن "builtin.h"
#ضمن "abspath.h"
#ضمن "config.h"
#ضمن "dir.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "object-file.h"
#ضمن "object-name.h"
#ضمن "parse-options.h"
#ضمن "path.h"
#ضمن "pathspec.h"
#ضمن "strbuf.h"
#ضمن "string-list.h"
#ضمن "lockfile.h"
#ضمن "unpack-trees.h"
#ضمن "quote.h"
#ضمن "setup.h"
#ضمن "sparse-index.h"
#ضمن "worktree.h"

ساكن ثابت حرف *empty_base = "";

ساكن حرف ثابت * ثابت builtin_sparse_checkout_usage[] = {
	N_("git sparse-checkout (init | list | set | add | reapply | disable | check-rules | clean) [<options>]"),
	عدم
};

ساكن فراغ write_patterns_to_file(FILE *fp, بنية pattern_list *pl)
{
	صحيح i;

	لكل (i = 0; i < pl->nr; i++) {
		بنية path_pattern *p = pl->patterns[i];

		إذا (p->flags & PATTERN_FLAG_NEGATIVE)
			fprintf(fp, "!");

		fprintf(fp, "%s", p->pattern);

		إذا (p->flags & PATTERN_FLAG_MUSTBEDIR)
			fprintf(fp, "/");

		fprintf(fp, "\n");
	}
}

ساكن حرف ثابت * ثابت builtin_sparse_checkout_list_usage[] = {
	"git sparse-checkout list",
	عدم
};

ساكن صحيح sparse_checkout_list(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
				بنية repository *repo UNUSED)
{
	ساكن بنية option builtin_sparse_checkout_list_options[] = {
		OPT_END(),
	};
	بنية pattern_list pl;
	حرف *sparse_filename;
	صحيح res;
	بنية repo_config_قيم *cfg = repo_config_قيم(the_repository);

	setup_work_شجرة(the_repository);
	إذا (!cfg->apply_sparse_checkout)
		die(_("this worktree is not sparse"));

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_list_options,
			     builtin_sparse_checkout_list_usage, 0);

	memset(&pl, 0, حجم(pl));

	pl.use_cone_patterns = cfg->core_sparse_checkout_cone;

	sparse_filename = get_sparse_checkout_filename();
	res = add_patterns_from_file_to_list(sparse_filename, "", 0, &pl, عدم, 0);
	حرر(sparse_filename);

	إذا (res < 0) {
		warning(_("this worktree is not sparse (sparse-checkout file may not exist)"));
		أرجع 0;
	}

	إذا (pl.use_cone_patterns) {
		صحيح i;
		بنية pattern_entry *pe;
		بنية hashmap_iter iter;
		بنية string_list sl = STRING_LIST_INIT_DUP;

		hashmap_for_each_entry(&pl.recursive_hashmap, &iter, pe, ent) {
			/* pe->pattern starts with "/", skip it */
			string_list_append(&sl, pe->pattern + 1);
		}

		string_list_sort_u(&sl, 0);

		لكل (i = 0; i < sl.nr; i++) {
			quote_c_style(sl.عناصر[i].string, عدم, stdout, 0);
			printf("\n");
		}

		string_list_clear(&sl, 0);
	} وإلا {
		write_patterns_to_file(stdout, &pl);
	}

	clear_pattern_list(&pl);

	أرجع 0;
}

ساكن فراغ clean_tracked_sparse_directories(بنية repository *r)
{
	صحيح i, was_full = 0;
	بنية strbuf path = STRBUF_INIT;
	size_t pathlen;
	بنية string_list_item *item;
	بنية string_list sparse_dirs = STRING_LIST_INIT_DUP;

	/*
	 * If we are not using cone mode patterns, then we cannot
	 * delete directories outside of the sparse cone.
	 */
	إذا (!r || !r->index || !r->worktree)
		أرجع;
	إذا (init_sparse_checkout_patterns(r->index) ||
	    !r->index->sparse_checkout_patterns->use_cone_patterns)
		أرجع;

	/*
	 * Use the sparse index as a data structure to assist finding
	 * directories that are safe to delete. This conversion to a
	 * sparse index will not delete directories that contain
	 * conflicted entries or submodules.
	 */
	إذا (r->index->sparse_index == INDEX_EXPANDED) {
		/*
		 * If something, such as a merge conflict or other concern,
		 * prevents us from converting to a sparse index, then do
		 * not try deleting files.
		 */
		إذا (convert_to_sparse(r->index, SPARSE_INDEX_MEMORY_ONLY))
			أرجع;
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
	لكل (i = 0; i < r->index->cache_nr; i++) {
		بنية cache_entry *ce = r->index->cache[i];

		إذا (S_ISSPARSEDIR(ce->ce_mode) &&
		    repo_file_exists(r, ce->name))
			string_list_append(&sparse_dirs, ce->name);
	}

	for_each_string_list_item(item, &sparse_dirs) {
		بنية dir_struct dir = DIR_INIT;
		بنية pathspec p = { 0 };
		بنية strvec s = STRVEC_INIT;

		strbuf_setlen(&path, pathlen);
		strbuf_addstr(&path, item->string);

		dir.flags |= DIR_SHOW_IGNORED_TOO;

		setup_standard_excludes(&dir);
		strvec_ادفع(&s, path.buf);

		parse_pathspec(&p, PATHSPEC_GLOB, 0, عدم, s.v);
		fill_directory(&dir, r->index, &p);

		إذا (dir.nr) {
			warning(_("directory '%s' contains untracked files,"
				  " but is not in the sparse-checkout cone"),
				item->string);
		} وإلا إذا (remove_dir_recursively(&path, 0)) {
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

	إذا (was_full)
		ensure_full_index(r->index);
}

ساكن صحيح update_working_directory(بنية repository *r,
				    بنية pattern_list *pl)
{
	تعداد update_sparsity_result result;
	بنية unpack_trees_options o;
	بنية lock_file lock_file = LOCK_INIT;
	بنية pattern_list *old_pl;

	/* If no branch has been checked out, there are no updates to make. */
	إذا (is_index_unborn(r->index))
		أرجع UPDATE_SPARSITY_SUCCESS;

	old_pl = r->index->sparse_checkout_patterns;
	r->index->sparse_checkout_patterns = pl;

	memset(&o, 0, حجم(o));
	o.verbose_update = isatty(2);
	o.update = 1;
	o.head_idx = -1;
	o.src_index = r->index;
	o.dst_index = r->index;
	o.skip_sparse_checkout = 0;

	setup_work_شجرة(the_repository);

	repo_hold_locked_index(r, &lock_file, LOCK_DIE_ON_ERROR);

	setup_unpack_trees_porcelain(&o, "sparse-checkout");
	result = update_sparsity(&o, pl);
	clear_unpack_trees_porcelain(&o);

	إذا (result == UPDATE_SPARSITY_WARNINGS)
		/*
		 * We don't do any special handling of warnings from untracked
		 * files in the way or dirty entries that can't be removed.
		 */
		result = UPDATE_SPARSITY_SUCCESS;
	إذا (result == UPDATE_SPARSITY_SUCCESS)
		write_locked_index(r->index, &lock_file, COMMIT_LOCK);
	وإلا
		rollback_lock_file(&lock_file);

	clean_tracked_sparse_directories(r);

	إذا (r->index->sparse_checkout_patterns != pl) {
		clear_pattern_list(r->index->sparse_checkout_patterns);
		FREE_AND_NULL(r->index->sparse_checkout_patterns);
	}
	r->index->sparse_checkout_patterns = old_pl;

	أرجع result;
}

ساكن حرف *escaped_pattern(حرف *pattern)
{
	حرف *p = pattern;
	بنية strbuf final = STRBUF_INIT;

	طالما (*p) {
		إذا (is_glob_special(*p))
			strbuf_addch(&final, '\\');

		strbuf_addch(&final, *p);
		p++;
	}

	أرجع strbuf_detach(&final, عدم);
}

ساكن فراغ write_cone_to_file(FILE *fp, بنية pattern_list *pl)
{
	صحيح i;
	بنية pattern_entry *pe;
	بنية hashmap_iter iter;
	بنية string_list sl = STRING_LIST_INIT_DUP;
	بنية strbuf parent_pattern = STRBUF_INIT;

	hashmap_for_each_entry(&pl->parent_hashmap, &iter, pe, ent) {
		إذا (hashmap_get_entry(&pl->recursive_hashmap, pe, ent, عدم))
			استمر;

		إذا (!hashmap_contains_parent(&pl->recursive_hashmap,
					     pe->pattern,
					     &parent_pattern))
			string_list_append(&sl, pe->pattern);
	}

	string_list_sort_u(&sl, 0);

	fprintf(fp, "/*\n!/*/\n");

	لكل (i = 0; i < sl.nr; i++) {
		حرف *pattern = escaped_pattern(sl.عناصر[i].string);

		إذا (strlen(pattern))
			fprintf(fp, "%s/\n!%s/*/\n", pattern, pattern);
		حرر(pattern);
	}

	string_list_clear(&sl, 0);

	hashmap_for_each_entry(&pl->recursive_hashmap, &iter, pe, ent) {
		إذا (!hashmap_contains_parent(&pl->recursive_hashmap,
					     pe->pattern,
					     &parent_pattern))
			string_list_append(&sl, pe->pattern);
	}

	strbuf_release(&parent_pattern);

	string_list_sort_u(&sl, 0);

	لكل (i = 0; i < sl.nr; i++) {
		حرف *pattern = escaped_pattern(sl.عناصر[i].string);
		fprintf(fp, "%s/\n", pattern);
		حرر(pattern);
	}

	string_list_clear(&sl, 0);
}

ساكن صحيح write_patterns_and_update(بنية repository *repo,
				     بنية pattern_list *pl)
{
	حرف *sparse_filename;
	FILE *fp;
	بنية lock_file lk = LOCK_INIT;
	صحيح result;
	بنية repo_config_قيم *cfg = repo_config_قيم(the_repository);

	sparse_filename = get_sparse_checkout_filename();

	إذا (safe_create_leading_directories(repo, sparse_filename))
		die(_("failed to create directory for sparse-checkout file"));

	repo_hold_lock_file_for_update(repo, &lk, sparse_filename,
				       LOCK_DIE_ON_ERROR);

	result = update_working_directory(repo, pl);
	إذا (result) {
		rollback_lock_file(&lk);
		update_working_directory(repo, عدم);
		اذهب out;
	}

	fp = fdopen_lock_file(&lk, "w");
	إذا (!fp)
		die_errno(_("unable to fdopen %s"), get_lock_file_path(&lk));

	إذا (cfg->core_sparse_checkout_cone)
		write_cone_to_file(fp, pl);
	وإلا
		write_patterns_to_file(fp, pl);

	إذا (commit_lock_file(&lk))
		die_errno(_("unable to write %s"), sparse_filename);

out:
	clear_pattern_list(pl);
	حرر(sparse_filename);
	أرجع result;
}

تعداد sparse_checkout_mode {
	MODE_NO_PATTERNS = 0,
	MODE_ALL_PATTERNS = 1,
	MODE_CONE_PATTERNS = 2,
};

ساكن صحيح set_config(بنية repository *repo,
		      تعداد sparse_checkout_mode mode)
{
	/* Update to use worktree config, if not already. */
	إذا (init_worktree_config(repo)) {
		error(_("failed to initialize worktree config"));
		أرجع 1;
	}

	إذا (repo_config_set_worktree_gently(repo,
					    "core.sparseCheckout",
					    mode ? "true" : "false") ||
	    repo_config_set_worktree_gently(repo,
					    "core.sparseCheckoutCone",
					    mode == MODE_CONE_PATTERNS ?
						"true" : "false"))
		أرجع 1;

	إذا (mode == MODE_NO_PATTERNS)
		أرجع set_sparse_index_config(repo, 0);

	أرجع 0;
}

ساكن تعداد sparse_checkout_mode update_cone_mode(صحيح *cone_mode) {
	بنية repo_config_قيم *cfg = repo_config_قيم(the_repository);

	/* If not specified, use previous definition of cone mode */
	إذا (*cone_mode == -1 && cfg->apply_sparse_checkout)
		*cone_mode = cfg->core_sparse_checkout_cone;

	/* Set cone/non-cone mode appropriately */
	cfg->apply_sparse_checkout = 1;
	إذا (*cone_mode == 1 || *cone_mode == -1) {
		cfg->core_sparse_checkout_cone = 1;
		أرجع MODE_CONE_PATTERNS;
	}
	cfg->core_sparse_checkout_cone = 0;
	أرجع MODE_ALL_PATTERNS;
}

ساكن صحيح update_modes(بنية repository *repo, صحيح *cone_mode, صحيح *sparse_index)
{
	صحيح mode, record_mode;
	بنية repo_config_قيم *cfg = repo_config_قيم(the_repository);

	/* Determine if we need to record the mode; ensure sparse checkout on */
	record_mode = (*cone_mode != -1) || !cfg->apply_sparse_checkout;

	mode = update_cone_mode(cone_mode);
	إذا (record_mode && set_config(repo, mode))
		أرجع 1;

	/* Set sparse-index/non-sparse-index mode if specified */
	إذا (*sparse_index >= 0) {
		إذا (set_sparse_index_config(repo, *sparse_index) < 0)
			die(_("failed to modify sparse-index config"));

		/* force an index rewrite */
		repo_read_index(repo);
		repo->index->updated_workdir = 1;

		إذا (!*sparse_index)
			ensure_full_index(repo->index);
	}

	أرجع 0;
}

ساكن حرف ثابت * ثابت builtin_sparse_checkout_init_usage[] = {
	"git sparse-checkout init [--cone] [--[no-]sparse-index]",
	عدم
};

ساكن بنية sparse_checkout_init_opts {
	صحيح cone_mode;
	صحيح sparse_index;
} init_opts;

ساكن صحيح sparse_checkout_init(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
				بنية repository *repo)
{
	بنية pattern_list pl;
	حرف *sparse_filename;
	صحيح res;
	بنية object_id oid;

	ساكن بنية option builtin_sparse_checkout_init_options[] = {
		OPT_BOOL(0, "cone", &init_opts.cone_mode,
			 N_("initialize the sparse-checkout in cone mode")),
		OPT_BOOL(0, "sparse-index", &init_opts.sparse_index,
			 N_("toggle the use of a sparse index")),
		OPT_END(),
	};

	setup_work_شجرة(the_repository);
	repo_read_index(repo);

	init_opts.cone_mode = -1;
	init_opts.sparse_index = -1;

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_init_options,
			     builtin_sparse_checkout_init_usage, 0);

	إذا (update_modes(repo, &init_opts.cone_mode, &init_opts.sparse_index))
		أرجع 1;

	memset(&pl, 0, حجم(pl));

	sparse_filename = get_sparse_checkout_filename();
	res = add_patterns_from_file_to_list(sparse_filename, "", 0, &pl, عدم, 0);

	/* If we already have a sparse-checkout file, use it. */
	إذا (res >= 0) {
		حرر(sparse_filename);
		clear_pattern_list(&pl);
		أرجع update_working_directory(repo, عدم);
	}

	إذا (repo_get_oid(repo, "HEAD", &oid)) {
		FILE *fp;

		/* assume we are in a fresh repo, but update the sparse-checkout file */
		إذا (safe_create_leading_directories(repo, sparse_filename))
			die(_("unable to create leading directories of %s"),
			    sparse_filename);
		fp = xfopen(sparse_filename, "w");
		إذا (!fp)
			die(_("failed to open '%s'"), sparse_filename);

		حرر(sparse_filename);
		fprintf(fp, "/*\n!/*/\n");
		fclose(fp);
		أرجع 0;
	}

	حرر(sparse_filename);

	add_pattern("/*", empty_base, 0, &pl, 0);
	add_pattern("!/*/", empty_base, 0, &pl, 0);
	pl.use_cone_patterns = init_opts.cone_mode;

	أرجع write_patterns_and_update(repo, &pl);
}

ساكن فراغ أدرج_recursive_pattern(بنية pattern_list *pl, بنية strbuf *path)
{
	بنية pattern_entry *e = xmalloc(حجم(*e));
	e->patternlen = path->len;
	e->pattern = strbuf_detach(path, عدم);
	hashmap_entry_init(&e->ent, fspathhash(e->pattern));

	hashmap_add(&pl->recursive_hashmap, &e->ent);

	طالما (e->patternlen) {
		حرف *slash = strrchr(e->pattern, '/');
		حرف *oldpattern = e->pattern;
		size_t newlen;
		بنية pattern_entry *dup;

		إذا (!slash || slash == e->pattern)
			اكسر;

		newlen = slash - e->pattern;
		e = xmalloc(حجم(بنية pattern_entry));
		e->patternlen = newlen;
		e->pattern = xstrndup(oldpattern, newlen);
		hashmap_entry_init(&e->ent, fspathhash(e->pattern));

		dup = hashmap_get_entry(&pl->parent_hashmap, e, ent, عدم);
		إذا (!dup) {
			hashmap_add(&pl->parent_hashmap, &e->ent);
		} وإلا {
			حرر(e->pattern);
			حرر(e);
			e = dup;
		}
	}
}

ساكن فراغ strbuf_to_cone_pattern(بنية strbuf *سطر, بنية pattern_list *pl)
{
	strbuf_trim(سطر);

	strbuf_trim_trailing_dir_sep(سطر);

	إذا (strbuf_normalize_path(سطر))
		die(_("could not normalize path %s"), سطر->buf);

	إذا (!سطر->len)
		أرجع;

	إذا (سطر->buf[0] != '/')
		strbuf_insertstr(سطر, 0, "/");

	أدرج_recursive_pattern(pl, سطر);
}

ساكن فراغ add_patterns_from_مدخل(بنية pattern_list *pl,
				    صحيح argc, ثابت حرف **argv,
				    FILE *file)
{
	صحيح i;
	بنية repo_config_قيم *cfg = repo_config_قيم(the_repository);

	إذا (cfg->core_sparse_checkout_cone) {
		بنية strbuf سطر = STRBUF_INIT;

		hashmap_init(&pl->recursive_hashmap, pl_hashmap_cmp, عدم, 0);
		hashmap_init(&pl->parent_hashmap, pl_hashmap_cmp, عدم, 0);
		pl->use_cone_patterns = 1;

		إذا (file) {
			بنية strbuf unquoted = STRBUF_INIT;
			طالما (!strbuf_getline(&سطر, file)) {
				إذا (سطر.buf[0] == '"') {
					strbuf_reset(&unquoted);
					إذا (unquote_c_style(&unquoted, سطر.buf, عدم))
						die(_("unable to unquote C-style string '%s'"),
						سطر.buf);

					strbuf_swap(&unquoted, &سطر);
				}

				strbuf_to_cone_pattern(&سطر, pl);
			}

			strbuf_release(&unquoted);
		} وإلا {
			لكل (i = 0; i < argc; i++) {
				strbuf_setlen(&سطر, 0);
				strbuf_addstr(&سطر, argv[i]);
				strbuf_to_cone_pattern(&سطر, pl);
			}
		}
		strbuf_release(&سطر);
	} وإلا {
		إذا (file) {
			بنية strbuf سطر = STRBUF_INIT;

			طالما (!strbuf_getline(&سطر, file))
				add_pattern(سطر.buf, empty_base, 0, pl, 0);

			strbuf_release(&سطر);
		} وإلا {
			لكل (i = 0; i < argc; i++)
				add_pattern(argv[i], empty_base, 0, pl, 0);
		}
	}
}

تعداد modify_type {
	REPLACE,
	ADD,
};

ساكن فراغ add_patterns_cone_mode(صحيح argc, ثابت حرف **argv,
				   بنية pattern_list *pl,
				   صحيح use_stdin)
{
	بنية strbuf buffer = STRBUF_INIT;
	بنية pattern_entry *pe;
	بنية hashmap_iter iter;
	بنية pattern_list existing;
	بنية repo_config_قيم *cfg = repo_config_قيم(the_repository);
	حرف *sparse_filename = get_sparse_checkout_filename();

	add_patterns_from_مدخل(pl, argc, argv,
				use_stdin ? stdin : عدم);

	memset(&existing, 0, حجم(existing));
	existing.use_cone_patterns = cfg->core_sparse_checkout_cone;

	إذا (add_patterns_from_file_to_list(sparse_filename, "", 0,
					   &existing, عدم, 0))
		die(_("unable to load existing sparse-checkout patterns"));
	حرر(sparse_filename);

	إذا (!existing.use_cone_patterns)
		die(_("existing sparse-checkout patterns do not use cone mode"));

	hashmap_for_each_entry(&existing.recursive_hashmap, &iter, pe, ent) {
		إذا (!hashmap_contains_parent(&pl->recursive_hashmap,
					pe->pattern, &buffer) ||
		    !hashmap_contains_parent(&pl->parent_hashmap,
					pe->pattern, &buffer)) {
			strbuf_reset(&buffer);
			strbuf_addstr(&buffer, pe->pattern);
			أدرج_recursive_pattern(pl, &buffer);
		}
	}

	clear_pattern_list(&existing);
	strbuf_release(&buffer);
}

ساكن فراغ add_patterns_literal(صحيح argc, ثابت حرف **argv,
				 بنية pattern_list *pl,
				 صحيح use_stdin)
{
	حرف *sparse_filename = get_sparse_checkout_filename();
	إذا (add_patterns_from_file_to_list(sparse_filename, "", 0,
					   pl, عدم, 0))
		die(_("unable to load existing sparse-checkout patterns"));
	حرر(sparse_filename);
	add_patterns_from_مدخل(pl, argc, argv, use_stdin ? stdin : عدم);
}

ساكن صحيح modify_pattern_list(بنية repository *repo,
			       بنية strvec *وسائط, صحيح use_stdin,
			       تعداد modify_type m)
{
	صحيح result;
	صحيح changed_config = 0;
	بنية pattern_list *pl = xcalloc(1, حجم(*pl));
	بنية repo_config_قيم *cfg = repo_config_قيم(the_repository);

	بدل (m) {
	حالة ADD:
		إذا (cfg->core_sparse_checkout_cone)
			add_patterns_cone_mode(وسائط->nr, وسائط->v, pl, use_stdin);
		وإلا
			add_patterns_literal(وسائط->nr, وسائط->v, pl, use_stdin);
		اكسر;

	حالة REPLACE:
		add_patterns_from_مدخل(pl, وسائط->nr, وسائط->v,
					use_stdin ? stdin : عدم);
		اكسر;
	}

	إذا (!cfg->apply_sparse_checkout) {
		set_config(repo, MODE_ALL_PATTERNS);
		cfg->apply_sparse_checkout = 1;
		changed_config = 1;
	}

	result = write_patterns_and_update(repo, pl);

	إذا (result && changed_config)
		set_config(repo, MODE_NO_PATTERNS);

	clear_pattern_list(pl);
	حرر(pl);
	أرجع result;
}

ساكن فراغ sanitize_paths(بنية repository *repo,
			   بنية strvec *وسائط,
			   ثابت حرف *prefix, صحيح skip_checks)
{
	صحيح i;
	بنية repo_config_قيم *cfg = repo_config_قيم(the_repository);

	إذا (!وسائط->nr)
		أرجع;

	إذا (prefix && *prefix && cfg->core_sparse_checkout_cone) {
		/*
		 * The args are not pathspecs, so unfortunately we
		 * cannot imitate how cmd_add() uses parse_pathspec().
		 */
		صحيح prefix_len = strlen(prefix);

		لكل (i = 0; i < وسائط->nr; i++) {
			حرف *prefixed_path = prefix_path(the_repository, prefix,
							  prefix_len, وسائط->v[i]);
			strvec_replace(وسائط, i, prefixed_path);
			حرر(prefixed_path);
		}
	}

	إذا (skip_checks)
		أرجع;

	إذا (prefix && *prefix && !cfg->core_sparse_checkout_cone)
		die(_("please run from the toplevel directory in non-cone mode"));

	إذا (cfg->core_sparse_checkout_cone) {
		لكل (i = 0; i < وسائط->nr; i++) {
			إذا (وسائط->v[i][0] == '/')
				die(_("specify directories rather than patterns (no leading slash)"));
			إذا (وسائط->v[i][0] == '!')
				die(_("specify directories rather than patterns.  If your directory starts with a '!', pass --skip-checks"));
			إذا (strpbrk(وسائط->v[i], "*?[]"))
				die(_("specify directories rather than patterns.  If your directory really has any of '*?[]\\' in it, pass --skip-checks"));
		}
	}

	لكل (i = 0; i < وسائط->nr; i++) {
		بنية cache_entry *ce;
		بنية index_state *index = repo->index;
		صحيح pos = index_name_pos(index, وسائط->v[i], strlen(وسائط->v[i]));

		إذا (pos < 0)
			استمر;
		ce = index->cache[pos];
		إذا (S_ISSPARSEDIR(ce->ce_mode))
			استمر;

		إذا (cfg->core_sparse_checkout_cone)
			die(_("'%s' is not a directory; to treat it as a directory anyway, rerun with --skip-checks"), وسائط->v[i]);
		وإلا
			warning(_("pass a leading slash before paths such as '%s' if you want a single file (see NON-CONE PROBLEMS in the git-sparse-checkout manual)."), وسائط->v[i]);
	}
}

ساكن حرف ثابت * ثابت builtin_sparse_checkout_add_usage[] = {
	N_("git sparse-checkout add [--skip-checks] (--stdin | <patterns>)"),
	عدم
};

ساكن بنية sparse_checkout_add_opts {
	صحيح skip_checks;
	صحيح use_stdin;
} add_opts;

ساكن صحيح sparse_checkout_add(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
			       بنية repository *repo)
{
	ساكن بنية option builtin_sparse_checkout_add_options[] = {
		OPT_BOOL_F(0, "skip-checks", &add_opts.skip_checks,
			   N_("skip some sanity checks on the given paths that might give false positives"),
			   PARSE_OPT_NONEG),
		OPT_BOOL(0, "stdin", &add_opts.use_stdin,
			 N_("read patterns from standard in")),
		OPT_END(),
	};
	بنية strvec patterns = STRVEC_INIT;
	صحيح ret;
	بنية repo_config_قيم *cfg = repo_config_قيم(the_repository);

	setup_work_شجرة(the_repository);
	إذا (!cfg->apply_sparse_checkout)
		die(_("no sparse-checkout to add to"));

	repo_read_index(repo);

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_add_options,
			     builtin_sparse_checkout_add_usage, 0);

	لكل (صحيح i = 0; i < argc; i++)
		strvec_ادفع(&patterns, argv[i]);
	sanitize_paths(repo, &patterns, prefix, add_opts.skip_checks);

	ret = modify_pattern_list(repo, &patterns, add_opts.use_stdin, ADD);

	strvec_clear(&patterns);
	أرجع ret;
}

ساكن حرف ثابت * ثابت builtin_sparse_checkout_set_usage[] = {
	N_("git sparse-checkout set [--[no-]cone] [--[no-]sparse-index] [--skip-checks] (--stdin | <patterns>)"),
	عدم
};

ساكن بنية sparse_checkout_set_opts {
	صحيح cone_mode;
	صحيح sparse_index;
	صحيح skip_checks;
	صحيح use_stdin;
} set_opts;

ساكن صحيح sparse_checkout_set(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
			       بنية repository *repo)
{
	بنية repo_config_قيم *cfg = repo_config_قيم(the_repository);
	صحيح default_patterns_nr = 2;
	ثابت حرف *default_patterns[] = {"/*", "!/*/", عدم};

	ساكن بنية option builtin_sparse_checkout_set_options[] = {
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
	بنية strvec patterns = STRVEC_INIT;
	صحيح ret;

	setup_work_شجرة(the_repository);
	repo_read_index(repo);

	set_opts.cone_mode = -1;
	set_opts.sparse_index = -1;

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_set_options,
			     builtin_sparse_checkout_set_usage, 0);

	إذا (update_modes(repo, &set_opts.cone_mode, &set_opts.sparse_index))
		أرجع 1;

	/*
	 * Cone mode automatically specifies the toplevel directory.  For
	 * non-cone mode, if nothing is specified, manually select just the
	 * top-level directory (much as 'init' would do).
	 */
	إذا (!cfg->core_sparse_checkout_cone && !set_opts.use_stdin && argc == 0) {
		لكل (صحيح i = 0; i < default_patterns_nr; i++)
			strvec_ادفع(&patterns, default_patterns[i]);
	} وإلا {
		لكل (صحيح i = 0; i < argc; i++)
			strvec_ادفع(&patterns, argv[i]);
		sanitize_paths(repo, &patterns, prefix, set_opts.skip_checks);
	}

	ret = modify_pattern_list(repo, &patterns, set_opts.use_stdin, REPLACE);

	strvec_clear(&patterns);
	أرجع ret;
}

ساكن حرف ثابت * ثابت builtin_sparse_checkout_reapply_usage[] = {
	"git sparse-checkout reapply [--[no-]cone] [--[no-]sparse-index]",
	عدم
};

ساكن بنية sparse_checkout_reapply_opts {
	صحيح cone_mode;
	صحيح sparse_index;
} reapply_opts;

ساكن صحيح sparse_checkout_reapply(صحيح argc, ثابت حرف **argv,
				   ثابت حرف *prefix,
				   بنية repository *repo)
{
	ساكن بنية option builtin_sparse_checkout_reapply_options[] = {
		OPT_BOOL(0, "cone", &reapply_opts.cone_mode,
			 N_("initialize the sparse-checkout in cone mode")),
		OPT_BOOL(0, "sparse-index", &reapply_opts.sparse_index,
			 N_("toggle the use of a sparse index")),
		OPT_END(),
	};
	بنية repo_config_قيم *cfg = repo_config_قيم(the_repository);

	setup_work_شجرة(the_repository);
	إذا (!cfg->apply_sparse_checkout)
		die(_("must be in a sparse-checkout to reapply sparsity patterns"));

	reapply_opts.cone_mode = -1;
	reapply_opts.sparse_index = -1;

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_reapply_options,
			     builtin_sparse_checkout_reapply_usage, 0);

	repo_read_index(repo);

	إذا (update_modes(repo, &reapply_opts.cone_mode, &reapply_opts.sparse_index))
		أرجع 1;

	أرجع update_working_directory(repo, عدم);
}

ساكن حرف ثابت * ثابت builtin_sparse_checkout_clean_usage[] = {
	"git sparse-checkout clean [-n|--dry-run]",
	عدم
};

ساكن صحيح list_file_iterator(ثابت حرف *path, ثابت فراغ *data)
{
	ثابت حرف *رسالة = data;

	printf(رسالة, path);
	أرجع 0;
}

ساكن فراغ list_every_file_in_dir(ثابت حرف *رسالة,
				   ثابت حرف *directory)
{
	بنية strbuf path = STRBUF_INIT;

	strbuf_addstr(&path, directory);
	for_each_file_in_dir(&path, list_file_iterator, رسالة);
	strbuf_release(&path);
}

ساكن ثابت حرف *رسالة_remove = N_("Removing %s\n");
ساكن ثابت حرف *رسالة_would_remove = N_("Would remove %s\n");

ساكن صحيح sparse_checkout_clean(صحيح argc, ثابت حرف **argv,
				   ثابت حرف *prefix,
				   بنية repository *repo)
{
	بنية strbuf full_path = STRBUF_INIT;
	ثابت حرف *رسالة = رسالة_remove;
	size_t worktree_len;
	صحيح force = 0, dry_run = 0, verbose = 0;
	صحيح require_force = 1;
	بنية repo_config_قيم *cfg = repo_config_قيم(the_repository);

	بنية option builtin_sparse_checkout_clean_options[] = {
		OPT__DRY_RUN(&dry_run, N_("dry run")),
		OPT__FORCE(&force, N_("force"), PARSE_OPT_NOCOMPLETE),
		OPT__VERBOSE(&verbose, N_("report each affected file, not just directories")),
		OPT_END(),
	};

	setup_work_شجرة(the_repository);
	إذا (!cfg->apply_sparse_checkout)
		die(_("must be in a sparse-checkout to clean directories"));
	إذا (!cfg->core_sparse_checkout_cone)
		die(_("must be in a cone-mode sparse-checkout to clean directories"));

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_clean_options,
			     builtin_sparse_checkout_clean_usage, 0);

	repo_config_get_bool(repo, "clean.requireforce", &require_force);
	إذا (require_force && !force && !dry_run)
		die(_("for safety, refusing to clean without one of --force or --dry-run"));

	إذا (dry_run)
		رسالة = رسالة_would_remove;

	إذا (repo_read_index(repo) < 0)
		die(_("failed to read index"));

	إذا (convert_to_sparse(repo->index, SPARSE_INDEX_MEMORY_ONLY) ||
	    repo->index->sparse_index == INDEX_EXPANDED)
		die(_("failed to convert index to a sparse index; resolve merge conflicts and try again"));

	strbuf_addstr(&full_path, repo->worktree);
	strbuf_addch(&full_path, '/');
	worktree_len = full_path.len;

	لكل (size_t i = 0; i < repo->index->cache_nr; i++) {
		بنية cache_entry *ce = repo->index->cache[i];
		إذا (!S_ISSPARSEDIR(ce->ce_mode))
			استمر;
		strbuf_setlen(&full_path, worktree_len);
		strbuf_add(&full_path, ce->name, ce->ce_namelen);

		إذا (!is_directory(full_path.buf))
			استمر;

		إذا (verbose)
			list_every_file_in_dir(رسالة, ce->name);
		وإلا
			printf(رسالة, ce->name);

		إذا (dry_run <= 0 &&
		    remove_dir_recursively(&full_path, 0))
			warning_errno(_("failed to remove '%s'"), ce->name);
	}

	strbuf_release(&full_path);
	أرجع 0;
}

ساكن حرف ثابت * ثابت builtin_sparse_checkout_disable_usage[] = {
	"git sparse-checkout disable",
	عدم
};

ساكن صحيح sparse_checkout_disable(صحيح argc, ثابت حرف **argv,
				   ثابت حرف *prefix,
				   بنية repository *repo)
{
	ساكن بنية option builtin_sparse_checkout_disable_options[] = {
		OPT_END(),
	};
	بنية pattern_list pl;
	بنية repo_config_قيم *cfg = repo_config_قيم(the_repository);

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

	setup_work_شجرة(the_repository);
	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_disable_options,
			     builtin_sparse_checkout_disable_usage, 0);

	/*
	 * Disable the advice message for expanding a sparse index, as we
	 * are expecting to do that when disabling sparse-checkout.
	 */
	give_advice_on_expansion = 0;
	repo_read_index(repo);

	memset(&pl, 0, حجم(pl));
	hashmap_init(&pl.recursive_hashmap, pl_hashmap_cmp, عدم, 0);
	hashmap_init(&pl.parent_hashmap, pl_hashmap_cmp, عدم, 0);
	pl.use_cone_patterns = 0;
	cfg->apply_sparse_checkout = 1;

	add_pattern("/*", empty_base, 0, &pl, 0);

	prepare_repo_settings(the_repository);
	repo->settings.sparse_index = 0;

	إذا (update_working_directory(repo, &pl))
		die(_("error while refreshing working directory"));

	clear_pattern_list(&pl);
	أرجع set_config(repo, MODE_NO_PATTERNS);
}

ساكن حرف ثابت * ثابت builtin_sparse_checkout_check_rules_usage[] = {
	N_("git sparse-checkout check-rules [-z] [--skip-checks]"
	   "[--[no-]cone] [--rules-file <file>]"),
	عدم
};

ساكن بنية sparse_checkout_check_rules_opts {
	صحيح cone_mode;
	صحيح null_termination;
	حرف *rules_file;
} check_rules_opts;

ساكن صحيح check_rules(بنية repository *repo,
		       بنية pattern_list *pl,
		       صحيح null_terminated)
{
	بنية strbuf سطر = STRBUF_INIT;
	بنية strbuf unquoted = STRBUF_INIT;
	حرف *path;
	صحيح سطر_terminator = null_terminated ? 0 : '\n';
	strbuf_getline_fn getline_fn = null_terminated ? strbuf_getline_nul
		: strbuf_getline;
	repo->index->sparse_checkout_patterns = pl;
	طالما (!getline_fn(&سطر, stdin)) {
		path = سطر.buf;
		إذا (!null_terminated && سطر.buf[0] == '"') {
			strbuf_reset(&unquoted);
			إذا (unquote_c_style(&unquoted, سطر.buf, عدم))
				die(_("unable to unquote C-style string '%s'"),
					سطر.buf);

			path = unquoted.buf;
		}

		إذا (path_in_sparse_checkout(path, repo->index))
			write_name_quoted(path, stdout, سطر_terminator);
	}
	strbuf_release(&سطر);
	strbuf_release(&unquoted);

	أرجع 0;
}

ساكن صحيح sparse_checkout_check_rules(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
				       بنية repository *repo)
{
	ساكن بنية option builtin_sparse_checkout_check_rules_options[] = {
		OPT_BOOL('z', عدم, &check_rules_opts.null_termination,
			 N_("terminate input and output files by a NUL character")),
		OPT_BOOL(0, "cone", &check_rules_opts.cone_mode,
			 N_("when used with --rules-file interpret patterns as cone mode patterns")),
		OPT_FILENAME(0, "rules-file", &check_rules_opts.rules_file,
			 N_("use patterns in <file> instead of the current ones.")),
		OPT_END(),
	};

	FILE *fp;
	صحيح ret;
	بنية pattern_list pl = {0};
	بنية repo_config_قيم *cfg = repo_config_قيم(the_repository);
	حرف *sparse_filename;
	check_rules_opts.cone_mode = -1;

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_check_rules_options,
			     builtin_sparse_checkout_check_rules_usage, 0);

	إذا (check_rules_opts.rules_file && check_rules_opts.cone_mode < 0)
		check_rules_opts.cone_mode = 1;

	update_cone_mode(&check_rules_opts.cone_mode);
	pl.use_cone_patterns = cfg->core_sparse_checkout_cone;
	إذا (check_rules_opts.rules_file) {
		fp = xfopen(check_rules_opts.rules_file, "r");
		add_patterns_from_مدخل(&pl, argc, argv, fp);
		fclose(fp);
	} وإلا {
		sparse_filename = get_sparse_checkout_filename();
		إذا (add_patterns_from_file_to_list(sparse_filename, "", 0, &pl,
						   عدم, 0))
			die(_("unable to load existing sparse-checkout patterns"));
		حرر(sparse_filename);
	}

	ret = check_rules(repo, &pl, check_rules_opts.null_termination);
	clear_pattern_list(&pl);
	حرر(check_rules_opts.rules_file);
	أرجع ret;
}

صحيح cmd_sparse_checkout(صحيح argc,
			ثابت حرف **argv,
			ثابت حرف *prefix,
			بنية repository *repo)
{
	parse_opt_subcommand_fn *fn = عدم;
	بنية option builtin_sparse_checkout_options[] = {
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

	repo_config(the_repository, git_default_config, عدم);

	prepare_repo_settings(repo);
	repo->settings.command_requires_full_index = 0;

	أرجع fn(argc, argv, prefix, repo);
}
