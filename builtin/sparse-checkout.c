#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#شامل "builtin.h"
#شامل "abspath.h"
#شامل "config.h"
#شامل "dir.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "object-file.h"
#شامل "object-name.h"
#شامل "parse-options.h"
#شامل "path.h"
#شامل "pathspec.h"
#شامل "strbuf.h"
#شامل "string-list.h"
#شامل "lockfile.h"
#شامل "unpack-trees.h"
#شامل "quote.h"
#شامل "setup.h"
#شامل "sparse-index.h"
#شامل "worktree.h"

ایستا ثابت نویسه *empty_base = "";

ایستا نویسه ثابت * ثابت builtin_sparse_checkout_usage[] = {
	N_("git sparse-checkout (init | list | set | add | reapply | disable | check-rules | clean) [<options>]"),
	پوچ
};

ایستا تهی write_patterns_to_file(FILE *fp, ساختار pattern_list *pl)
{
	صحیح i;

	برای (i = 0; i < pl->nr; i++) {
		ساختار path_pattern *p = pl->patterns[i];

		اگر (p->flags & PATTERN_FLAG_NEGATIVE)
			fprintf(fp, "!");

		fprintf(fp, "%s", p->pattern);

		اگر (p->flags & PATTERN_FLAG_MUSTBEDIR)
			fprintf(fp, "/");

		fprintf(fp, "\n");
	}
}

ایستا نویسه ثابت * ثابت builtin_sparse_checkout_list_usage[] = {
	"git sparse-checkout list",
	پوچ
};

ایستا صحیح sparse_checkout_list(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
				ساختار repository *repo UNUSED)
{
	ایستا ساختار option builtin_sparse_checkout_list_options[] = {
		OPT_END(),
	};
	ساختار pattern_list pl;
	نویسه *sparse_filename;
	صحیح res;
	ساختار repo_config_مقادیر *cfg = repo_config_مقادیر(the_repository);

	setup_work_درخت(the_repository);
	اگر (!cfg->apply_sparse_checkout)
		die(_("this worktree is not sparse"));

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_list_options,
			     builtin_sparse_checkout_list_usage, 0);

	memset(&pl, 0, اندازه(pl));

	pl.use_cone_patterns = cfg->core_sparse_checkout_cone;

	sparse_filename = get_sparse_checkout_filename();
	res = add_patterns_from_file_to_list(sparse_filename, "", 0, &pl, پوچ, 0);
	آزاد(sparse_filename);

	اگر (res < 0) {
		warning(_("this worktree is not sparse (sparse-checkout file may not exist)"));
		بازگشت 0;
	}

	اگر (pl.use_cone_patterns) {
		صحیح i;
		ساختار pattern_entry *pe;
		ساختار hashmap_iter iter;
		ساختار string_list sl = STRING_LIST_INIT_DUP;

		hashmap_for_each_entry(&pl.recursive_hashmap, &iter, pe, ent) {
			/* pe->pattern starts with "/", skip it */
			string_list_append(&sl, pe->pattern + 1);
		}

		string_list_sort_u(&sl, 0);

		برای (i = 0; i < sl.nr; i++) {
			quote_c_style(sl.اقلام[i].string, پوچ, stdout, 0);
			printf("\n");
		}

		string_list_clear(&sl, 0);
	} وگرنه {
		write_patterns_to_file(stdout, &pl);
	}

	clear_pattern_list(&pl);

	بازگشت 0;
}

ایستا تهی clean_tracked_sparse_directories(ساختار repository *r)
{
	صحیح i, was_full = 0;
	ساختار strbuf path = STRBUF_INIT;
	size_t pathlen;
	ساختار string_list_item *item;
	ساختار string_list sparse_dirs = STRING_LIST_INIT_DUP;

	/*
	 * If we are not using cone mode patterns, then we cannot
	 * delete directories outside of the sparse cone.
	 */
	اگر (!r || !r->index || !r->worktree)
		بازگشت;
	اگر (init_sparse_checkout_patterns(r->index) ||
	    !r->index->sparse_checkout_patterns->use_cone_patterns)
		بازگشت;

	/*
	 * Use the sparse index as a data structure to assist finding
	 * directories that are safe to delete. This conversion to a
	 * sparse index will not delete directories that contain
	 * conflicted entries or submodules.
	 */
	اگر (r->index->sparse_index == INDEX_EXPANDED) {
		/*
		 * If something, such as a merge conflict or other concern,
		 * prevents us from converting to a sparse index, then do
		 * not try deleting files.
		 */
		اگر (convert_to_sparse(r->index, SPARSE_INDEX_MEMORY_ONLY))
			بازگشت;
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
	برای (i = 0; i < r->index->cache_nr; i++) {
		ساختار cache_entry *ce = r->index->cache[i];

		اگر (S_ISSPARSEDIR(ce->ce_mode) &&
		    repo_file_exists(r, ce->name))
			string_list_append(&sparse_dirs, ce->name);
	}

	for_each_string_list_item(item, &sparse_dirs) {
		ساختار dir_struct dir = DIR_INIT;
		ساختار pathspec p = { 0 };
		ساختار strvec s = STRVEC_INIT;

		strbuf_setlen(&path, pathlen);
		strbuf_addstr(&path, item->string);

		dir.flags |= DIR_SHOW_IGNORED_TOO;

		setup_standard_excludes(&dir);
		strvec_افزودن(&s, path.buf);

		parse_pathspec(&p, PATHSPEC_GLOB, 0, پوچ, s.v);
		fill_directory(&dir, r->index, &p);

		اگر (dir.nr) {
			warning(_("directory '%s' contains untracked files,"
				  " but is not in the sparse-checkout cone"),
				item->string);
		} وگرنه اگر (remove_dir_recursively(&path, 0)) {
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

	اگر (was_full)
		ensure_full_index(r->index);
}

ایستا صحیح update_working_directory(ساختار repository *r,
				    ساختار pattern_list *pl)
{
	شمارش update_sparsity_result result;
	ساختار unpack_trees_options o;
	ساختار lock_file lock_file = LOCK_INIT;
	ساختار pattern_list *old_pl;

	/* If no branch has been checked out, there are no updates to make. */
	اگر (is_index_unborn(r->index))
		بازگشت UPDATE_SPARSITY_SUCCESS;

	old_pl = r->index->sparse_checkout_patterns;
	r->index->sparse_checkout_patterns = pl;

	memset(&o, 0, اندازه(o));
	o.verbose_update = isatty(2);
	o.update = 1;
	o.head_idx = -1;
	o.src_index = r->index;
	o.dst_index = r->index;
	o.skip_sparse_checkout = 0;

	setup_work_درخت(the_repository);

	repo_hold_locked_index(r, &lock_file, LOCK_DIE_ON_ERROR);

	setup_unpack_trees_porcelain(&o, "sparse-checkout");
	result = update_sparsity(&o, pl);
	clear_unpack_trees_porcelain(&o);

	اگر (result == UPDATE_SPARSITY_WARNINGS)
		/*
		 * We don't do any special handling of warnings from untracked
		 * files in the way or dirty entries that can't be removed.
		 */
		result = UPDATE_SPARSITY_SUCCESS;
	اگر (result == UPDATE_SPARSITY_SUCCESS)
		write_locked_index(r->index, &lock_file, COMMIT_LOCK);
	وگرنه
		rollback_lock_file(&lock_file);

	clean_tracked_sparse_directories(r);

	اگر (r->index->sparse_checkout_patterns != pl) {
		clear_pattern_list(r->index->sparse_checkout_patterns);
		FREE_AND_NULL(r->index->sparse_checkout_patterns);
	}
	r->index->sparse_checkout_patterns = old_pl;

	بازگشت result;
}

ایستا نویسه *escaped_pattern(نویسه *pattern)
{
	نویسه *p = pattern;
	ساختار strbuf final = STRBUF_INIT;

	وقتی (*p) {
		اگر (is_glob_special(*p))
			strbuf_addch(&final, '\\');

		strbuf_addch(&final, *p);
		p++;
	}

	بازگشت strbuf_detach(&final, پوچ);
}

ایستا تهی write_cone_to_file(FILE *fp, ساختار pattern_list *pl)
{
	صحیح i;
	ساختار pattern_entry *pe;
	ساختار hashmap_iter iter;
	ساختار string_list sl = STRING_LIST_INIT_DUP;
	ساختار strbuf parent_pattern = STRBUF_INIT;

	hashmap_for_each_entry(&pl->parent_hashmap, &iter, pe, ent) {
		اگر (hashmap_get_entry(&pl->recursive_hashmap, pe, ent, پوچ))
			ادامه;

		اگر (!hashmap_contains_parent(&pl->recursive_hashmap,
					     pe->pattern,
					     &parent_pattern))
			string_list_append(&sl, pe->pattern);
	}

	string_list_sort_u(&sl, 0);

	fprintf(fp, "/*\n!/*/\n");

	برای (i = 0; i < sl.nr; i++) {
		نویسه *pattern = escaped_pattern(sl.اقلام[i].string);

		اگر (strlen(pattern))
			fprintf(fp, "%s/\n!%s/*/\n", pattern, pattern);
		آزاد(pattern);
	}

	string_list_clear(&sl, 0);

	hashmap_for_each_entry(&pl->recursive_hashmap, &iter, pe, ent) {
		اگر (!hashmap_contains_parent(&pl->recursive_hashmap,
					     pe->pattern,
					     &parent_pattern))
			string_list_append(&sl, pe->pattern);
	}

	strbuf_release(&parent_pattern);

	string_list_sort_u(&sl, 0);

	برای (i = 0; i < sl.nr; i++) {
		نویسه *pattern = escaped_pattern(sl.اقلام[i].string);
		fprintf(fp, "%s/\n", pattern);
		آزاد(pattern);
	}

	string_list_clear(&sl, 0);
}

ایستا صحیح write_patterns_and_update(ساختار repository *repo,
				     ساختار pattern_list *pl)
{
	نویسه *sparse_filename;
	FILE *fp;
	ساختار lock_file lk = LOCK_INIT;
	صحیح result;
	ساختار repo_config_مقادیر *cfg = repo_config_مقادیر(the_repository);

	sparse_filename = get_sparse_checkout_filename();

	اگر (safe_create_leading_directories(repo, sparse_filename))
		die(_("failed to create directory for sparse-checkout file"));

	repo_hold_lock_file_for_update(repo, &lk, sparse_filename,
				       LOCK_DIE_ON_ERROR);

	result = update_working_directory(repo, pl);
	اگر (result) {
		rollback_lock_file(&lk);
		update_working_directory(repo, پوچ);
		برو out;
	}

	fp = fdopen_lock_file(&lk, "w");
	اگر (!fp)
		die_errno(_("unable to fdopen %s"), get_lock_file_path(&lk));

	اگر (cfg->core_sparse_checkout_cone)
		write_cone_to_file(fp, pl);
	وگرنه
		write_patterns_to_file(fp, pl);

	اگر (commit_lock_file(&lk))
		die_errno(_("unable to write %s"), sparse_filename);

out:
	clear_pattern_list(pl);
	آزاد(sparse_filename);
	بازگشت result;
}

شمارش sparse_checkout_mode {
	MODE_NO_PATTERNS = 0,
	MODE_ALL_PATTERNS = 1,
	MODE_CONE_PATTERNS = 2,
};

ایستا صحیح set_config(ساختار repository *repo,
		      شمارش sparse_checkout_mode mode)
{
	/* Update to use worktree config, if not already. */
	اگر (init_worktree_config(repo)) {
		error(_("failed to initialize worktree config"));
		بازگشت 1;
	}

	اگر (repo_config_set_worktree_gently(repo,
					    "core.sparseCheckout",
					    mode ? "true" : "false") ||
	    repo_config_set_worktree_gently(repo,
					    "core.sparseCheckoutCone",
					    mode == MODE_CONE_PATTERNS ?
						"true" : "false"))
		بازگشت 1;

	اگر (mode == MODE_NO_PATTERNS)
		بازگشت set_sparse_index_config(repo, 0);

	بازگشت 0;
}

ایستا شمارش sparse_checkout_mode update_cone_mode(صحیح *cone_mode) {
	ساختار repo_config_مقادیر *cfg = repo_config_مقادیر(the_repository);

	/* If not specified, use previous definition of cone mode */
	اگر (*cone_mode == -1 && cfg->apply_sparse_checkout)
		*cone_mode = cfg->core_sparse_checkout_cone;

	/* Set cone/non-cone mode appropriately */
	cfg->apply_sparse_checkout = 1;
	اگر (*cone_mode == 1 || *cone_mode == -1) {
		cfg->core_sparse_checkout_cone = 1;
		بازگشت MODE_CONE_PATTERNS;
	}
	cfg->core_sparse_checkout_cone = 0;
	بازگشت MODE_ALL_PATTERNS;
}

ایستا صحیح update_modes(ساختار repository *repo, صحیح *cone_mode, صحیح *sparse_index)
{
	صحیح mode, record_mode;
	ساختار repo_config_مقادیر *cfg = repo_config_مقادیر(the_repository);

	/* Determine if we need to record the mode; ensure sparse checkout on */
	record_mode = (*cone_mode != -1) || !cfg->apply_sparse_checkout;

	mode = update_cone_mode(cone_mode);
	اگر (record_mode && set_config(repo, mode))
		بازگشت 1;

	/* Set sparse-index/non-sparse-index mode if specified */
	اگر (*sparse_index >= 0) {
		اگر (set_sparse_index_config(repo, *sparse_index) < 0)
			die(_("failed to modify sparse-index config"));

		/* force an index rewrite */
		repo_read_index(repo);
		repo->index->updated_workdir = 1;

		اگر (!*sparse_index)
			ensure_full_index(repo->index);
	}

	بازگشت 0;
}

ایستا نویسه ثابت * ثابت builtin_sparse_checkout_init_usage[] = {
	"git sparse-checkout init [--cone] [--[no-]sparse-index]",
	پوچ
};

ایستا ساختار sparse_checkout_init_opts {
	صحیح cone_mode;
	صحیح sparse_index;
} init_opts;

ایستا صحیح sparse_checkout_init(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
				ساختار repository *repo)
{
	ساختار pattern_list pl;
	نویسه *sparse_filename;
	صحیح res;
	ساختار object_id oid;

	ایستا ساختار option builtin_sparse_checkout_init_options[] = {
		OPT_BOOL(0, "cone", &init_opts.cone_mode,
			 N_("initialize the sparse-checkout in cone mode")),
		OPT_BOOL(0, "sparse-index", &init_opts.sparse_index,
			 N_("toggle the use of a sparse index")),
		OPT_END(),
	};

	setup_work_درخت(the_repository);
	repo_read_index(repo);

	init_opts.cone_mode = -1;
	init_opts.sparse_index = -1;

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_init_options,
			     builtin_sparse_checkout_init_usage, 0);

	اگر (update_modes(repo, &init_opts.cone_mode, &init_opts.sparse_index))
		بازگشت 1;

	memset(&pl, 0, اندازه(pl));

	sparse_filename = get_sparse_checkout_filename();
	res = add_patterns_from_file_to_list(sparse_filename, "", 0, &pl, پوچ, 0);

	/* If we already have a sparse-checkout file, use it. */
	اگر (res >= 0) {
		آزاد(sparse_filename);
		clear_pattern_list(&pl);
		بازگشت update_working_directory(repo, پوچ);
	}

	اگر (repo_get_oid(repo, "HEAD", &oid)) {
		FILE *fp;

		/* assume we are in a fresh repo, but update the sparse-checkout file */
		اگر (safe_create_leading_directories(repo, sparse_filename))
			die(_("unable to create leading directories of %s"),
			    sparse_filename);
		fp = xfopen(sparse_filename, "w");
		اگر (!fp)
			die(_("failed to open '%s'"), sparse_filename);

		آزاد(sparse_filename);
		fprintf(fp, "/*\n!/*/\n");
		fclose(fp);
		بازگشت 0;
	}

	آزاد(sparse_filename);

	add_pattern("/*", empty_base, 0, &pl, 0);
	add_pattern("!/*/", empty_base, 0, &pl, 0);
	pl.use_cone_patterns = init_opts.cone_mode;

	بازگشت write_patterns_and_update(repo, &pl);
}

ایستا تهی درج_recursive_pattern(ساختار pattern_list *pl, ساختار strbuf *path)
{
	ساختار pattern_entry *e = xmalloc(اندازه(*e));
	e->patternlen = path->len;
	e->pattern = strbuf_detach(path, پوچ);
	hashmap_entry_init(&e->ent, fspathhash(e->pattern));

	hashmap_add(&pl->recursive_hashmap, &e->ent);

	وقتی (e->patternlen) {
		نویسه *slash = strrchr(e->pattern, '/');
		نویسه *oldpattern = e->pattern;
		size_t newlen;
		ساختار pattern_entry *dup;

		اگر (!slash || slash == e->pattern)
			بشکن;

		newlen = slash - e->pattern;
		e = xmalloc(اندازه(ساختار pattern_entry));
		e->patternlen = newlen;
		e->pattern = xstrndup(oldpattern, newlen);
		hashmap_entry_init(&e->ent, fspathhash(e->pattern));

		dup = hashmap_get_entry(&pl->parent_hashmap, e, ent, پوچ);
		اگر (!dup) {
			hashmap_add(&pl->parent_hashmap, &e->ent);
		} وگرنه {
			آزاد(e->pattern);
			آزاد(e);
			e = dup;
		}
	}
}

ایستا تهی strbuf_to_cone_pattern(ساختار strbuf *سطر, ساختار pattern_list *pl)
{
	strbuf_trim(سطر);

	strbuf_trim_trailing_dir_sep(سطر);

	اگر (strbuf_normalize_path(سطر))
		die(_("could not normalize path %s"), سطر->buf);

	اگر (!سطر->len)
		بازگشت;

	اگر (سطر->buf[0] != '/')
		strbuf_insertstr(سطر, 0, "/");

	درج_recursive_pattern(pl, سطر);
}

ایستا تهی add_patterns_from_درون_داد(ساختار pattern_list *pl,
				    صحیح argc, ثابت نویسه **argv,
				    FILE *file)
{
	صحیح i;
	ساختار repo_config_مقادیر *cfg = repo_config_مقادیر(the_repository);

	اگر (cfg->core_sparse_checkout_cone) {
		ساختار strbuf سطر = STRBUF_INIT;

		hashmap_init(&pl->recursive_hashmap, pl_hashmap_cmp, پوچ, 0);
		hashmap_init(&pl->parent_hashmap, pl_hashmap_cmp, پوچ, 0);
		pl->use_cone_patterns = 1;

		اگر (file) {
			ساختار strbuf unquoted = STRBUF_INIT;
			وقتی (!strbuf_getline(&سطر, file)) {
				اگر (سطر.buf[0] == '"') {
					strbuf_reset(&unquoted);
					اگر (unquote_c_style(&unquoted, سطر.buf, پوچ))
						die(_("unable to unquote C-style string '%s'"),
						سطر.buf);

					strbuf_swap(&unquoted, &سطر);
				}

				strbuf_to_cone_pattern(&سطر, pl);
			}

			strbuf_release(&unquoted);
		} وگرنه {
			برای (i = 0; i < argc; i++) {
				strbuf_setlen(&سطر, 0);
				strbuf_addstr(&سطر, argv[i]);
				strbuf_to_cone_pattern(&سطر, pl);
			}
		}
		strbuf_release(&سطر);
	} وگرنه {
		اگر (file) {
			ساختار strbuf سطر = STRBUF_INIT;

			وقتی (!strbuf_getline(&سطر, file))
				add_pattern(سطر.buf, empty_base, 0, pl, 0);

			strbuf_release(&سطر);
		} وگرنه {
			برای (i = 0; i < argc; i++)
				add_pattern(argv[i], empty_base, 0, pl, 0);
		}
	}
}

شمارش modify_type {
	REPLACE,
	ADD,
};

ایستا تهی add_patterns_cone_mode(صحیح argc, ثابت نویسه **argv,
				   ساختار pattern_list *pl,
				   صحیح use_stdin)
{
	ساختار strbuf buffer = STRBUF_INIT;
	ساختار pattern_entry *pe;
	ساختار hashmap_iter iter;
	ساختار pattern_list existing;
	ساختار repo_config_مقادیر *cfg = repo_config_مقادیر(the_repository);
	نویسه *sparse_filename = get_sparse_checkout_filename();

	add_patterns_from_درون_داد(pl, argc, argv,
				use_stdin ? stdin : پوچ);

	memset(&existing, 0, اندازه(existing));
	existing.use_cone_patterns = cfg->core_sparse_checkout_cone;

	اگر (add_patterns_from_file_to_list(sparse_filename, "", 0,
					   &existing, پوچ, 0))
		die(_("unable to load existing sparse-checkout patterns"));
	آزاد(sparse_filename);

	اگر (!existing.use_cone_patterns)
		die(_("existing sparse-checkout patterns do not use cone mode"));

	hashmap_for_each_entry(&existing.recursive_hashmap, &iter, pe, ent) {
		اگر (!hashmap_contains_parent(&pl->recursive_hashmap,
					pe->pattern, &buffer) ||
		    !hashmap_contains_parent(&pl->parent_hashmap,
					pe->pattern, &buffer)) {
			strbuf_reset(&buffer);
			strbuf_addstr(&buffer, pe->pattern);
			درج_recursive_pattern(pl, &buffer);
		}
	}

	clear_pattern_list(&existing);
	strbuf_release(&buffer);
}

ایستا تهی add_patterns_literal(صحیح argc, ثابت نویسه **argv,
				 ساختار pattern_list *pl,
				 صحیح use_stdin)
{
	نویسه *sparse_filename = get_sparse_checkout_filename();
	اگر (add_patterns_from_file_to_list(sparse_filename, "", 0,
					   pl, پوچ, 0))
		die(_("unable to load existing sparse-checkout patterns"));
	آزاد(sparse_filename);
	add_patterns_from_درون_داد(pl, argc, argv, use_stdin ? stdin : پوچ);
}

ایستا صحیح modify_pattern_list(ساختار repository *repo,
			       ساختار strvec *args, صحیح use_stdin,
			       شمارش modify_type m)
{
	صحیح result;
	صحیح changed_config = 0;
	ساختار pattern_list *pl = xcalloc(1, اندازه(*pl));
	ساختار repo_config_مقادیر *cfg = repo_config_مقادیر(the_repository);

	انتخاب (m) {
	حالت ADD:
		اگر (cfg->core_sparse_checkout_cone)
			add_patterns_cone_mode(args->nr, args->v, pl, use_stdin);
		وگرنه
			add_patterns_literal(args->nr, args->v, pl, use_stdin);
		بشکن;

	حالت REPLACE:
		add_patterns_from_درون_داد(pl, args->nr, args->v,
					use_stdin ? stdin : پوچ);
		بشکن;
	}

	اگر (!cfg->apply_sparse_checkout) {
		set_config(repo, MODE_ALL_PATTERNS);
		cfg->apply_sparse_checkout = 1;
		changed_config = 1;
	}

	result = write_patterns_and_update(repo, pl);

	اگر (result && changed_config)
		set_config(repo, MODE_NO_PATTERNS);

	clear_pattern_list(pl);
	آزاد(pl);
	بازگشت result;
}

ایستا تهی sanitize_paths(ساختار repository *repo,
			   ساختار strvec *args,
			   ثابت نویسه *prefix, صحیح skip_checks)
{
	صحیح i;
	ساختار repo_config_مقادیر *cfg = repo_config_مقادیر(the_repository);

	اگر (!args->nr)
		بازگشت;

	اگر (prefix && *prefix && cfg->core_sparse_checkout_cone) {
		/*
		 * The args are not pathspecs, so unfortunately we
		 * cannot imitate how cmd_add() uses parse_pathspec().
		 */
		صحیح prefix_len = strlen(prefix);

		برای (i = 0; i < args->nr; i++) {
			نویسه *prefixed_path = prefix_path(the_repository, prefix,
							  prefix_len, args->v[i]);
			strvec_replace(args, i, prefixed_path);
			آزاد(prefixed_path);
		}
	}

	اگر (skip_checks)
		بازگشت;

	اگر (prefix && *prefix && !cfg->core_sparse_checkout_cone)
		die(_("please run from the toplevel directory in non-cone mode"));

	اگر (cfg->core_sparse_checkout_cone) {
		برای (i = 0; i < args->nr; i++) {
			اگر (args->v[i][0] == '/')
				die(_("specify directories rather than patterns (no leading slash)"));
			اگر (args->v[i][0] == '!')
				die(_("specify directories rather than patterns.  If your directory starts with a '!', pass --skip-checks"));
			اگر (strpbrk(args->v[i], "*?[]"))
				die(_("specify directories rather than patterns.  If your directory really has any of '*?[]\\' in it, pass --skip-checks"));
		}
	}

	برای (i = 0; i < args->nr; i++) {
		ساختار cache_entry *ce;
		ساختار index_state *index = repo->index;
		صحیح pos = index_name_pos(index, args->v[i], strlen(args->v[i]));

		اگر (pos < 0)
			ادامه;
		ce = index->cache[pos];
		اگر (S_ISSPARSEDIR(ce->ce_mode))
			ادامه;

		اگر (cfg->core_sparse_checkout_cone)
			die(_("'%s' is not a directory; to treat it as a directory anyway, rerun with --skip-checks"), args->v[i]);
		وگرنه
			warning(_("pass a leading slash before paths such as '%s' if you want a single file (see NON-CONE PROBLEMS in the git-sparse-checkout manual)."), args->v[i]);
	}
}

ایستا نویسه ثابت * ثابت builtin_sparse_checkout_add_usage[] = {
	N_("git sparse-checkout add [--skip-checks] (--stdin | <patterns>)"),
	پوچ
};

ایستا ساختار sparse_checkout_add_opts {
	صحیح skip_checks;
	صحیح use_stdin;
} add_opts;

ایستا صحیح sparse_checkout_add(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
			       ساختار repository *repo)
{
	ایستا ساختار option builtin_sparse_checkout_add_options[] = {
		OPT_BOOL_F(0, "skip-checks", &add_opts.skip_checks,
			   N_("skip some sanity checks on the given paths that might give false positives"),
			   PARSE_OPT_NONEG),
		OPT_BOOL(0, "stdin", &add_opts.use_stdin,
			 N_("read patterns from standard in")),
		OPT_END(),
	};
	ساختار strvec patterns = STRVEC_INIT;
	صحیح ret;
	ساختار repo_config_مقادیر *cfg = repo_config_مقادیر(the_repository);

	setup_work_درخت(the_repository);
	اگر (!cfg->apply_sparse_checkout)
		die(_("no sparse-checkout to add to"));

	repo_read_index(repo);

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_add_options,
			     builtin_sparse_checkout_add_usage, 0);

	برای (صحیح i = 0; i < argc; i++)
		strvec_افزودن(&patterns, argv[i]);
	sanitize_paths(repo, &patterns, prefix, add_opts.skip_checks);

	ret = modify_pattern_list(repo, &patterns, add_opts.use_stdin, ADD);

	strvec_clear(&patterns);
	بازگشت ret;
}

ایستا نویسه ثابت * ثابت builtin_sparse_checkout_set_usage[] = {
	N_("git sparse-checkout set [--[no-]cone] [--[no-]sparse-index] [--skip-checks] (--stdin | <patterns>)"),
	پوچ
};

ایستا ساختار sparse_checkout_set_opts {
	صحیح cone_mode;
	صحیح sparse_index;
	صحیح skip_checks;
	صحیح use_stdin;
} set_opts;

ایستا صحیح sparse_checkout_set(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
			       ساختار repository *repo)
{
	ساختار repo_config_مقادیر *cfg = repo_config_مقادیر(the_repository);
	صحیح default_patterns_nr = 2;
	ثابت نویسه *default_patterns[] = {"/*", "!/*/", پوچ};

	ایستا ساختار option builtin_sparse_checkout_set_options[] = {
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
	ساختار strvec patterns = STRVEC_INIT;
	صحیح ret;

	setup_work_درخت(the_repository);
	repo_read_index(repo);

	set_opts.cone_mode = -1;
	set_opts.sparse_index = -1;

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_set_options,
			     builtin_sparse_checkout_set_usage, 0);

	اگر (update_modes(repo, &set_opts.cone_mode, &set_opts.sparse_index))
		بازگشت 1;

	/*
	 * Cone mode automatically specifies the toplevel directory.  For
	 * non-cone mode, if nothing is specified, manually select just the
	 * top-level directory (much as 'init' would do).
	 */
	اگر (!cfg->core_sparse_checkout_cone && !set_opts.use_stdin && argc == 0) {
		برای (صحیح i = 0; i < default_patterns_nr; i++)
			strvec_افزودن(&patterns, default_patterns[i]);
	} وگرنه {
		برای (صحیح i = 0; i < argc; i++)
			strvec_افزودن(&patterns, argv[i]);
		sanitize_paths(repo, &patterns, prefix, set_opts.skip_checks);
	}

	ret = modify_pattern_list(repo, &patterns, set_opts.use_stdin, REPLACE);

	strvec_clear(&patterns);
	بازگشت ret;
}

ایستا نویسه ثابت * ثابت builtin_sparse_checkout_reapply_usage[] = {
	"git sparse-checkout reapply [--[no-]cone] [--[no-]sparse-index]",
	پوچ
};

ایستا ساختار sparse_checkout_reapply_opts {
	صحیح cone_mode;
	صحیح sparse_index;
} reapply_opts;

ایستا صحیح sparse_checkout_reapply(صحیح argc, ثابت نویسه **argv,
				   ثابت نویسه *prefix,
				   ساختار repository *repo)
{
	ایستا ساختار option builtin_sparse_checkout_reapply_options[] = {
		OPT_BOOL(0, "cone", &reapply_opts.cone_mode,
			 N_("initialize the sparse-checkout in cone mode")),
		OPT_BOOL(0, "sparse-index", &reapply_opts.sparse_index,
			 N_("toggle the use of a sparse index")),
		OPT_END(),
	};
	ساختار repo_config_مقادیر *cfg = repo_config_مقادیر(the_repository);

	setup_work_درخت(the_repository);
	اگر (!cfg->apply_sparse_checkout)
		die(_("must be in a sparse-checkout to reapply sparsity patterns"));

	reapply_opts.cone_mode = -1;
	reapply_opts.sparse_index = -1;

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_reapply_options,
			     builtin_sparse_checkout_reapply_usage, 0);

	repo_read_index(repo);

	اگر (update_modes(repo, &reapply_opts.cone_mode, &reapply_opts.sparse_index))
		بازگشت 1;

	بازگشت update_working_directory(repo, پوچ);
}

ایستا نویسه ثابت * ثابت builtin_sparse_checkout_clean_usage[] = {
	"git sparse-checkout clean [-n|--dry-run]",
	پوچ
};

ایستا صحیح list_file_iterator(ثابت نویسه *path, ثابت تهی *data)
{
	ثابت نویسه *پیام = data;

	printf(پیام, path);
	بازگشت 0;
}

ایستا تهی list_every_file_in_dir(ثابت نویسه *پیام,
				   ثابت نویسه *directory)
{
	ساختار strbuf path = STRBUF_INIT;

	strbuf_addstr(&path, directory);
	for_each_file_in_dir(&path, list_file_iterator, پیام);
	strbuf_release(&path);
}

ایستا ثابت نویسه *پیام_remove = N_("Removing %s\n");
ایستا ثابت نویسه *پیام_would_remove = N_("Would remove %s\n");

ایستا صحیح sparse_checkout_clean(صحیح argc, ثابت نویسه **argv,
				   ثابت نویسه *prefix,
				   ساختار repository *repo)
{
	ساختار strbuf full_path = STRBUF_INIT;
	ثابت نویسه *پیام = پیام_remove;
	size_t worktree_len;
	صحیح force = 0, dry_run = 0, verbose = 0;
	صحیح require_force = 1;
	ساختار repo_config_مقادیر *cfg = repo_config_مقادیر(the_repository);

	ساختار option builtin_sparse_checkout_clean_options[] = {
		OPT__DRY_RUN(&dry_run, N_("dry run")),
		OPT__FORCE(&force, N_("force"), PARSE_OPT_NOCOMPLETE),
		OPT__VERBOSE(&verbose, N_("report each affected file, not just directories")),
		OPT_END(),
	};

	setup_work_درخت(the_repository);
	اگر (!cfg->apply_sparse_checkout)
		die(_("must be in a sparse-checkout to clean directories"));
	اگر (!cfg->core_sparse_checkout_cone)
		die(_("must be in a cone-mode sparse-checkout to clean directories"));

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_clean_options,
			     builtin_sparse_checkout_clean_usage, 0);

	repo_config_get_bool(repo, "clean.requireforce", &require_force);
	اگر (require_force && !force && !dry_run)
		die(_("for safety, refusing to clean without one of --force or --dry-run"));

	اگر (dry_run)
		پیام = پیام_would_remove;

	اگر (repo_read_index(repo) < 0)
		die(_("failed to read index"));

	اگر (convert_to_sparse(repo->index, SPARSE_INDEX_MEMORY_ONLY) ||
	    repo->index->sparse_index == INDEX_EXPANDED)
		die(_("failed to convert index to a sparse index; resolve merge conflicts and try again"));

	strbuf_addstr(&full_path, repo->worktree);
	strbuf_addch(&full_path, '/');
	worktree_len = full_path.len;

	برای (size_t i = 0; i < repo->index->cache_nr; i++) {
		ساختار cache_entry *ce = repo->index->cache[i];
		اگر (!S_ISSPARSEDIR(ce->ce_mode))
			ادامه;
		strbuf_setlen(&full_path, worktree_len);
		strbuf_add(&full_path, ce->name, ce->ce_namelen);

		اگر (!is_directory(full_path.buf))
			ادامه;

		اگر (verbose)
			list_every_file_in_dir(پیام, ce->name);
		وگرنه
			printf(پیام, ce->name);

		اگر (dry_run <= 0 &&
		    remove_dir_recursively(&full_path, 0))
			warning_errno(_("failed to remove '%s'"), ce->name);
	}

	strbuf_release(&full_path);
	بازگشت 0;
}

ایستا نویسه ثابت * ثابت builtin_sparse_checkout_disable_usage[] = {
	"git sparse-checkout disable",
	پوچ
};

ایستا صحیح sparse_checkout_disable(صحیح argc, ثابت نویسه **argv,
				   ثابت نویسه *prefix,
				   ساختار repository *repo)
{
	ایستا ساختار option builtin_sparse_checkout_disable_options[] = {
		OPT_END(),
	};
	ساختار pattern_list pl;
	ساختار repo_config_مقادیر *cfg = repo_config_مقادیر(the_repository);

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

	setup_work_درخت(the_repository);
	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_disable_options,
			     builtin_sparse_checkout_disable_usage, 0);

	/*
	 * Disable the advice message for expanding a sparse index, as we
	 * are expecting to do that when disabling sparse-checkout.
	 */
	give_advice_on_expansion = 0;
	repo_read_index(repo);

	memset(&pl, 0, اندازه(pl));
	hashmap_init(&pl.recursive_hashmap, pl_hashmap_cmp, پوچ, 0);
	hashmap_init(&pl.parent_hashmap, pl_hashmap_cmp, پوچ, 0);
	pl.use_cone_patterns = 0;
	cfg->apply_sparse_checkout = 1;

	add_pattern("/*", empty_base, 0, &pl, 0);

	prepare_repo_settings(the_repository);
	repo->settings.sparse_index = 0;

	اگر (update_working_directory(repo, &pl))
		die(_("error while refreshing working directory"));

	clear_pattern_list(&pl);
	بازگشت set_config(repo, MODE_NO_PATTERNS);
}

ایستا نویسه ثابت * ثابت builtin_sparse_checkout_check_rules_usage[] = {
	N_("git sparse-checkout check-rules [-z] [--skip-checks]"
	   "[--[no-]cone] [--rules-file <file>]"),
	پوچ
};

ایستا ساختار sparse_checkout_check_rules_opts {
	صحیح cone_mode;
	صحیح null_termination;
	نویسه *rules_file;
} check_rules_opts;

ایستا صحیح check_rules(ساختار repository *repo,
		       ساختار pattern_list *pl,
		       صحیح null_terminated)
{
	ساختار strbuf سطر = STRBUF_INIT;
	ساختار strbuf unquoted = STRBUF_INIT;
	نویسه *path;
	صحیح سطر_terminator = null_terminated ? 0 : '\n';
	strbuf_getline_fn getline_fn = null_terminated ? strbuf_getline_nul
		: strbuf_getline;
	repo->index->sparse_checkout_patterns = pl;
	وقتی (!getline_fn(&سطر, stdin)) {
		path = سطر.buf;
		اگر (!null_terminated && سطر.buf[0] == '"') {
			strbuf_reset(&unquoted);
			اگر (unquote_c_style(&unquoted, سطر.buf, پوچ))
				die(_("unable to unquote C-style string '%s'"),
					سطر.buf);

			path = unquoted.buf;
		}

		اگر (path_in_sparse_checkout(path, repo->index))
			write_name_quoted(path, stdout, سطر_terminator);
	}
	strbuf_release(&سطر);
	strbuf_release(&unquoted);

	بازگشت 0;
}

ایستا صحیح sparse_checkout_check_rules(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
				       ساختار repository *repo)
{
	ایستا ساختار option builtin_sparse_checkout_check_rules_options[] = {
		OPT_BOOL('z', پوچ, &check_rules_opts.null_termination,
			 N_("terminate input and output files by a NUL character")),
		OPT_BOOL(0, "cone", &check_rules_opts.cone_mode,
			 N_("when used with --rules-file interpret patterns as cone mode patterns")),
		OPT_FILENAME(0, "rules-file", &check_rules_opts.rules_file,
			 N_("use patterns in <file> instead of the current ones.")),
		OPT_END(),
	};

	FILE *fp;
	صحیح ret;
	ساختار pattern_list pl = {0};
	ساختار repo_config_مقادیر *cfg = repo_config_مقادیر(the_repository);
	نویسه *sparse_filename;
	check_rules_opts.cone_mode = -1;

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_check_rules_options,
			     builtin_sparse_checkout_check_rules_usage, 0);

	اگر (check_rules_opts.rules_file && check_rules_opts.cone_mode < 0)
		check_rules_opts.cone_mode = 1;

	update_cone_mode(&check_rules_opts.cone_mode);
	pl.use_cone_patterns = cfg->core_sparse_checkout_cone;
	اگر (check_rules_opts.rules_file) {
		fp = xfopen(check_rules_opts.rules_file, "r");
		add_patterns_from_درون_داد(&pl, argc, argv, fp);
		fclose(fp);
	} وگرنه {
		sparse_filename = get_sparse_checkout_filename();
		اگر (add_patterns_from_file_to_list(sparse_filename, "", 0, &pl,
						   پوچ, 0))
			die(_("unable to load existing sparse-checkout patterns"));
		آزاد(sparse_filename);
	}

	ret = check_rules(repo, &pl, check_rules_opts.null_termination);
	clear_pattern_list(&pl);
	آزاد(check_rules_opts.rules_file);
	بازگشت ret;
}

صحیح cmd_sparse_checkout(صحیح argc,
			ثابت نویسه **argv,
			ثابت نویسه *prefix,
			ساختار repository *repo)
{
	parse_opt_subcommand_fn *fn = پوچ;
	ساختار option builtin_sparse_checkout_options[] = {
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

	repo_config(the_repository, git_default_config, پوچ);

	prepare_repo_settings(repo);
	repo->settings.command_requires_full_index = 0;

	بازگشت fn(argc, argv, prefix, repo);
}
