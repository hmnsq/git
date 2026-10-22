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

স্থির ধ্রুব বর্ণ *empty_base = "";

স্থির বর্ণ ধ্রুব * ধ্রুব builtin_sparse_checkout_usage[] = {
	N_("git sparse-checkout (init | list | set | add | reapply | disable | check-rules | clean) [<options>]"),
	রিক্ত
};

স্থির শূন্য write_patterns_to_file(FILE *fp, সংরচনা pattern_list *pl)
{
	পূর্ণাঙ্ক i;

	প্রতি (i = 0; i < pl->nr; i++) {
		সংরচনা path_pattern *p = pl->patterns[i];

		যদি (p->flags & PATTERN_FLAG_NEGATIVE)
			fprintf(fp, "!");

		fprintf(fp, "%s", p->pattern);

		যদি (p->flags & PATTERN_FLAG_MUSTBEDIR)
			fprintf(fp, "/");

		fprintf(fp, "\n");
	}
}

স্থির বর্ণ ধ্রুব * ধ্রুব builtin_sparse_checkout_list_usage[] = {
	"git sparse-checkout list",
	রিক্ত
};

স্থির পূর্ণাঙ্ক sparse_checkout_list(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix,
				সংরচনা repository *repo UNUSED)
{
	স্থির সংরচনা option builtin_sparse_checkout_list_options[] = {
		OPT_END(),
	};
	সংরচনা pattern_list pl;
	বর্ণ *sparse_filename;
	পূর্ণাঙ্ক res;
	সংরচনা repo_config_মানসমূহ *cfg = repo_config_মানসমূহ(the_repository);

	setup_work_বৃক্ষ(the_repository);
	যদি (!cfg->apply_sparse_checkout)
		die(_("this worktree is not sparse"));

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_list_options,
			     builtin_sparse_checkout_list_usage, 0);

	memset(&pl, 0, পরিমাণ(pl));

	pl.use_cone_patterns = cfg->core_sparse_checkout_cone;

	sparse_filename = get_sparse_checkout_filename();
	res = add_patterns_from_file_to_list(sparse_filename, "", 0, &pl, রিক্ত, 0);
	মুক্ত(sparse_filename);

	যদি (res < 0) {
		warning(_("this worktree is not sparse (sparse-checkout file may not exist)"));
		প্রত্যাগম 0;
	}

	যদি (pl.use_cone_patterns) {
		পূর্ণাঙ্ক i;
		সংরচনা pattern_entry *pe;
		সংরচনা hashmap_iter iter;
		সংরচনা string_list sl = STRING_LIST_INIT_DUP;

		hashmap_for_each_entry(&pl.recursive_hashmap, &iter, pe, ent) {
			/* pe->pattern starts with "/", skip it */
			string_list_append(&sl, pe->pattern + 1);
		}

		string_list_sort_u(&sl, 0);

		প্রতি (i = 0; i < sl.nr; i++) {
			quote_c_style(sl.বস্তুসমূহ[i].string, রিক্ত, stdout, 0);
			printf("\n");
		}

		string_list_clear(&sl, 0);
	} অন্যথা {
		write_patterns_to_file(stdout, &pl);
	}

	clear_pattern_list(&pl);

	প্রত্যাগম 0;
}

স্থির শূন্য clean_tracked_sparse_directories(সংরচনা repository *r)
{
	পূর্ণাঙ্ক i, was_full = 0;
	সংরচনা strbuf path = STRBUF_INIT;
	size_t pathlen;
	সংরচনা string_list_item *item;
	সংরচনা string_list sparse_dirs = STRING_LIST_INIT_DUP;

	/*
	 * If we are not using cone mode patterns, then we cannot
	 * delete directories outside of the sparse cone.
	 */
	যদি (!r || !r->index || !r->worktree)
		প্রত্যাগম;
	যদি (init_sparse_checkout_patterns(r->index) ||
	    !r->index->sparse_checkout_patterns->use_cone_patterns)
		প্রত্যাগম;

	/*
	 * Use the sparse index as a data structure to assist finding
	 * directories that are safe to delete. This conversion to a
	 * sparse index will not delete directories that contain
	 * conflicted entries or submodules.
	 */
	যদি (r->index->sparse_index == INDEX_EXPANDED) {
		/*
		 * If something, such as a merge conflict or other concern,
		 * prevents us from converting to a sparse index, then do
		 * not try deleting files.
		 */
		যদি (convert_to_sparse(r->index, SPARSE_INDEX_MEMORY_ONLY))
			প্রত্যাগম;
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
	প্রতি (i = 0; i < r->index->cache_nr; i++) {
		সংরচনা cache_entry *ce = r->index->cache[i];

		যদি (S_ISSPARSEDIR(ce->ce_mode) &&
		    repo_file_exists(r, ce->name))
			string_list_append(&sparse_dirs, ce->name);
	}

	for_each_string_list_item(item, &sparse_dirs) {
		সংরচনা dir_struct dir = DIR_INIT;
		সংরচনা pathspec p = { 0 };
		সংরচনা strvec s = STRVEC_INIT;

		strbuf_setlen(&path, pathlen);
		strbuf_addstr(&path, item->string);

		dir.flags |= DIR_SHOW_IGNORED_TOO;

		setup_standard_excludes(&dir);
		strvec_ঠেলো(&s, path.buf);

		parse_pathspec(&p, PATHSPEC_GLOB, 0, রিক্ত, s.v);
		fill_directory(&dir, r->index, &p);

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
		ensure_full_index(r->index);
}

স্থির পূর্ণাঙ্ক update_working_directory(সংরচনা repository *r,
				    সংরচনা pattern_list *pl)
{
	গণনা update_sparsity_result result;
	সংরচনা unpack_trees_options o;
	সংরচনা lock_file lock_file = LOCK_INIT;
	সংরচনা pattern_list *old_pl;

	/* If no branch has been checked out, there are no updates to make. */
	যদি (is_index_unborn(r->index))
		প্রত্যাগম UPDATE_SPARSITY_SUCCESS;

	old_pl = r->index->sparse_checkout_patterns;
	r->index->sparse_checkout_patterns = pl;

	memset(&o, 0, পরিমাণ(o));
	o.verbose_update = isatty(2);
	o.update = 1;
	o.head_idx = -1;
	o.src_index = r->index;
	o.dst_index = r->index;
	o.skip_sparse_checkout = 0;

	setup_work_বৃক্ষ(the_repository);

	repo_hold_locked_index(r, &lock_file, LOCK_DIE_ON_ERROR);

	setup_unpack_trees_porcelain(&o, "sparse-checkout");
	result = update_sparsity(&o, pl);
	clear_unpack_trees_porcelain(&o);

	যদি (result == UPDATE_SPARSITY_WARNINGS)
		/*
		 * We don't do any special handling of warnings from untracked
		 * files in the way or dirty entries that can't be removed.
		 */
		result = UPDATE_SPARSITY_SUCCESS;
	যদি (result == UPDATE_SPARSITY_SUCCESS)
		write_locked_index(r->index, &lock_file, COMMIT_LOCK);
	অন্যথা
		rollback_lock_file(&lock_file);

	clean_tracked_sparse_directories(r);

	যদি (r->index->sparse_checkout_patterns != pl) {
		clear_pattern_list(r->index->sparse_checkout_patterns);
		FREE_AND_NULL(r->index->sparse_checkout_patterns);
	}
	r->index->sparse_checkout_patterns = old_pl;

	প্রত্যাগম result;
}

স্থির বর্ণ *escaped_pattern(বর্ণ *pattern)
{
	বর্ণ *p = pattern;
	সংরচনা strbuf final = STRBUF_INIT;

	যাবত্ (*p) {
		যদি (is_glob_special(*p))
			strbuf_addch(&final, '\\');

		strbuf_addch(&final, *p);
		p++;
	}

	প্রত্যাগম strbuf_detach(&final, রিক্ত);
}

স্থির শূন্য write_cone_to_file(FILE *fp, সংরচনা pattern_list *pl)
{
	পূর্ণাঙ্ক i;
	সংরচনা pattern_entry *pe;
	সংরচনা hashmap_iter iter;
	সংরচনা string_list sl = STRING_LIST_INIT_DUP;
	সংরচনা strbuf parent_pattern = STRBUF_INIT;

	hashmap_for_each_entry(&pl->parent_hashmap, &iter, pe, ent) {
		যদি (hashmap_get_entry(&pl->recursive_hashmap, pe, ent, রিক্ত))
			অনুবর্ত;

		যদি (!hashmap_contains_parent(&pl->recursive_hashmap,
					     pe->pattern,
					     &parent_pattern))
			string_list_append(&sl, pe->pattern);
	}

	string_list_sort_u(&sl, 0);

	fprintf(fp, "/*\n!/*/\n");

	প্রতি (i = 0; i < sl.nr; i++) {
		বর্ণ *pattern = escaped_pattern(sl.বস্তুসমূহ[i].string);

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

	প্রতি (i = 0; i < sl.nr; i++) {
		বর্ণ *pattern = escaped_pattern(sl.বস্তুসমূহ[i].string);
		fprintf(fp, "%s/\n", pattern);
		মুক্ত(pattern);
	}

	string_list_clear(&sl, 0);
}

স্থির পূর্ণাঙ্ক write_patterns_and_update(সংরচনা repository *repo,
				     সংরচনা pattern_list *pl)
{
	বর্ণ *sparse_filename;
	FILE *fp;
	সংরচনা lock_file lk = LOCK_INIT;
	পূর্ণাঙ্ক result;
	সংরচনা repo_config_মানসমূহ *cfg = repo_config_মানসমূহ(the_repository);

	sparse_filename = get_sparse_checkout_filename();

	যদি (safe_create_leading_directories(repo, sparse_filename))
		die(_("failed to create directory for sparse-checkout file"));

	repo_hold_lock_file_for_update(repo, &lk, sparse_filename,
				       LOCK_DIE_ON_ERROR);

	result = update_working_directory(repo, pl);
	যদি (result) {
		rollback_lock_file(&lk);
		update_working_directory(repo, রিক্ত);
		গচ্ছ out;
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
	প্রত্যাগম result;
}

গণনা sparse_checkout_mode {
	MODE_NO_PATTERNS = 0,
	MODE_ALL_PATTERNS = 1,
	MODE_CONE_PATTERNS = 2,
};

স্থির পূর্ণাঙ্ক set_config(সংরচনা repository *repo,
		      গণনা sparse_checkout_mode mode)
{
	/* Update to use worktree config, if not already. */
	যদি (init_worktree_config(repo)) {
		error(_("failed to initialize worktree config"));
		প্রত্যাগম 1;
	}

	যদি (repo_config_set_worktree_gently(repo,
					    "core.sparseCheckout",
					    mode ? "true" : "false") ||
	    repo_config_set_worktree_gently(repo,
					    "core.sparseCheckoutCone",
					    mode == MODE_CONE_PATTERNS ?
						"true" : "false"))
		প্রত্যাগম 1;

	যদি (mode == MODE_NO_PATTERNS)
		প্রত্যাগম set_sparse_index_config(repo, 0);

	প্রত্যাগম 0;
}

স্থির গণনা sparse_checkout_mode update_cone_mode(পূর্ণাঙ্ক *cone_mode) {
	সংরচনা repo_config_মানসমূহ *cfg = repo_config_মানসমূহ(the_repository);

	/* If not specified, use previous definition of cone mode */
	যদি (*cone_mode == -1 && cfg->apply_sparse_checkout)
		*cone_mode = cfg->core_sparse_checkout_cone;

	/* Set cone/non-cone mode appropriately */
	cfg->apply_sparse_checkout = 1;
	যদি (*cone_mode == 1 || *cone_mode == -1) {
		cfg->core_sparse_checkout_cone = 1;
		প্রত্যাগম MODE_CONE_PATTERNS;
	}
	cfg->core_sparse_checkout_cone = 0;
	প্রত্যাগম MODE_ALL_PATTERNS;
}

স্থির পূর্ণাঙ্ক update_modes(সংরচনা repository *repo, পূর্ণাঙ্ক *cone_mode, পূর্ণাঙ্ক *sparse_index)
{
	পূর্ণাঙ্ক mode, record_mode;
	সংরচনা repo_config_মানসমূহ *cfg = repo_config_মানসমূহ(the_repository);

	/* Determine if we need to record the mode; ensure sparse checkout on */
	record_mode = (*cone_mode != -1) || !cfg->apply_sparse_checkout;

	mode = update_cone_mode(cone_mode);
	যদি (record_mode && set_config(repo, mode))
		প্রত্যাগম 1;

	/* Set sparse-index/non-sparse-index mode if specified */
	যদি (*sparse_index >= 0) {
		যদি (set_sparse_index_config(repo, *sparse_index) < 0)
			die(_("failed to modify sparse-index config"));

		/* force an index rewrite */
		repo_read_index(repo);
		repo->index->updated_workdir = 1;

		যদি (!*sparse_index)
			ensure_full_index(repo->index);
	}

	প্রত্যাগম 0;
}

স্থির বর্ণ ধ্রুব * ধ্রুব builtin_sparse_checkout_init_usage[] = {
	"git sparse-checkout init [--cone] [--[no-]sparse-index]",
	রিক্ত
};

স্থির সংরচনা sparse_checkout_init_opts {
	পূর্ণাঙ্ক cone_mode;
	পূর্ণাঙ্ক sparse_index;
} init_opts;

স্থির পূর্ণাঙ্ক sparse_checkout_init(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix,
				সংরচনা repository *repo)
{
	সংরচনা pattern_list pl;
	বর্ণ *sparse_filename;
	পূর্ণাঙ্ক res;
	সংরচনা object_id oid;

	স্থির সংরচনা option builtin_sparse_checkout_init_options[] = {
		OPT_BOOL(0, "cone", &init_opts.cone_mode,
			 N_("initialize the sparse-checkout in cone mode")),
		OPT_BOOL(0, "sparse-index", &init_opts.sparse_index,
			 N_("toggle the use of a sparse index")),
		OPT_END(),
	};

	setup_work_বৃক্ষ(the_repository);
	repo_read_index(repo);

	init_opts.cone_mode = -1;
	init_opts.sparse_index = -1;

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_init_options,
			     builtin_sparse_checkout_init_usage, 0);

	যদি (update_modes(repo, &init_opts.cone_mode, &init_opts.sparse_index))
		প্রত্যাগম 1;

	memset(&pl, 0, পরিমাণ(pl));

	sparse_filename = get_sparse_checkout_filename();
	res = add_patterns_from_file_to_list(sparse_filename, "", 0, &pl, রিক্ত, 0);

	/* If we already have a sparse-checkout file, use it. */
	যদি (res >= 0) {
		মুক্ত(sparse_filename);
		clear_pattern_list(&pl);
		প্রত্যাগম update_working_directory(repo, রিক্ত);
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
		প্রত্যাগম 0;
	}

	মুক্ত(sparse_filename);

	add_pattern("/*", empty_base, 0, &pl, 0);
	add_pattern("!/*/", empty_base, 0, &pl, 0);
	pl.use_cone_patterns = init_opts.cone_mode;

	প্রত্যাগম write_patterns_and_update(repo, &pl);
}

স্থির শূন্য ঢোকাও_recursive_pattern(সংরচনা pattern_list *pl, সংরচনা strbuf *path)
{
	সংরচনা pattern_entry *e = xmalloc(পরিমাণ(*e));
	e->patternlen = path->len;
	e->pattern = strbuf_detach(path, রিক্ত);
	hashmap_entry_init(&e->ent, fspathhash(e->pattern));

	hashmap_add(&pl->recursive_hashmap, &e->ent);

	যাবত্ (e->patternlen) {
		বর্ণ *slash = strrchr(e->pattern, '/');
		বর্ণ *oldpattern = e->pattern;
		size_t newlen;
		সংরচনা pattern_entry *dup;

		যদি (!slash || slash == e->pattern)
			ভঙ্গ;

		newlen = slash - e->pattern;
		e = xmalloc(পরিমাণ(সংরচনা pattern_entry));
		e->patternlen = newlen;
		e->pattern = xstrndup(oldpattern, newlen);
		hashmap_entry_init(&e->ent, fspathhash(e->pattern));

		dup = hashmap_get_entry(&pl->parent_hashmap, e, ent, রিক্ত);
		যদি (!dup) {
			hashmap_add(&pl->parent_hashmap, &e->ent);
		} অন্যথা {
			মুক্ত(e->pattern);
			মুক্ত(e);
			e = dup;
		}
	}
}

স্থির শূন্য strbuf_to_cone_pattern(সংরচনা strbuf *সারি, সংরচনা pattern_list *pl)
{
	strbuf_trim(সারি);

	strbuf_trim_trailing_dir_sep(সারি);

	যদি (strbuf_normalize_path(সারি))
		die(_("could not normalize path %s"), সারি->buf);

	যদি (!সারি->len)
		প্রত্যাগম;

	যদি (সারি->buf[0] != '/')
		strbuf_insertstr(সারি, 0, "/");

	ঢোকাও_recursive_pattern(pl, সারি);
}

স্থির শূন্য add_patterns_from_প্রবেশ(সংরচনা pattern_list *pl,
				    পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv,
				    FILE *file)
{
	পূর্ণাঙ্ক i;
	সংরচনা repo_config_মানসমূহ *cfg = repo_config_মানসমূহ(the_repository);

	যদি (cfg->core_sparse_checkout_cone) {
		সংরচনা strbuf সারি = STRBUF_INIT;

		hashmap_init(&pl->recursive_hashmap, pl_hashmap_cmp, রিক্ত, 0);
		hashmap_init(&pl->parent_hashmap, pl_hashmap_cmp, রিক্ত, 0);
		pl->use_cone_patterns = 1;

		যদি (file) {
			সংরচনা strbuf unquoted = STRBUF_INIT;
			যাবত্ (!strbuf_getline(&সারি, file)) {
				যদি (সারি.buf[0] == '"') {
					strbuf_reset(&unquoted);
					যদি (unquote_c_style(&unquoted, সারি.buf, রিক্ত))
						die(_("unable to unquote C-style string '%s'"),
						সারি.buf);

					strbuf_swap(&unquoted, &সারি);
				}

				strbuf_to_cone_pattern(&সারি, pl);
			}

			strbuf_release(&unquoted);
		} অন্যথা {
			প্রতি (i = 0; i < argc; i++) {
				strbuf_setlen(&সারি, 0);
				strbuf_addstr(&সারি, argv[i]);
				strbuf_to_cone_pattern(&সারি, pl);
			}
		}
		strbuf_release(&সারি);
	} অন্যথা {
		যদি (file) {
			সংরচনা strbuf সারি = STRBUF_INIT;

			যাবত্ (!strbuf_getline(&সারি, file))
				add_pattern(সারি.buf, empty_base, 0, pl, 0);

			strbuf_release(&সারি);
		} অন্যথা {
			প্রতি (i = 0; i < argc; i++)
				add_pattern(argv[i], empty_base, 0, pl, 0);
		}
	}
}

গণনা modify_type {
	REPLACE,
	ADD,
};

স্থির শূন্য add_patterns_cone_mode(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv,
				   সংরচনা pattern_list *pl,
				   পূর্ণাঙ্ক use_stdin)
{
	সংরচনা strbuf buffer = STRBUF_INIT;
	সংরচনা pattern_entry *pe;
	সংরচনা hashmap_iter iter;
	সংরচনা pattern_list existing;
	সংরচনা repo_config_মানসমূহ *cfg = repo_config_মানসমূহ(the_repository);
	বর্ণ *sparse_filename = get_sparse_checkout_filename();

	add_patterns_from_প্রবেশ(pl, argc, argv,
				use_stdin ? stdin : রিক্ত);

	memset(&existing, 0, পরিমাণ(existing));
	existing.use_cone_patterns = cfg->core_sparse_checkout_cone;

	যদি (add_patterns_from_file_to_list(sparse_filename, "", 0,
					   &existing, রিক্ত, 0))
		die(_("unable to load existing sparse-checkout patterns"));
	মুক্ত(sparse_filename);

	যদি (!existing.use_cone_patterns)
		die(_("existing sparse-checkout patterns do not use cone mode"));

	hashmap_for_each_entry(&existing.recursive_hashmap, &iter, pe, ent) {
		যদি (!hashmap_contains_parent(&pl->recursive_hashmap,
					pe->pattern, &buffer) ||
		    !hashmap_contains_parent(&pl->parent_hashmap,
					pe->pattern, &buffer)) {
			strbuf_reset(&buffer);
			strbuf_addstr(&buffer, pe->pattern);
			ঢোকাও_recursive_pattern(pl, &buffer);
		}
	}

	clear_pattern_list(&existing);
	strbuf_release(&buffer);
}

স্থির শূন্য add_patterns_literal(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv,
				 সংরচনা pattern_list *pl,
				 পূর্ণাঙ্ক use_stdin)
{
	বর্ণ *sparse_filename = get_sparse_checkout_filename();
	যদি (add_patterns_from_file_to_list(sparse_filename, "", 0,
					   pl, রিক্ত, 0))
		die(_("unable to load existing sparse-checkout patterns"));
	মুক্ত(sparse_filename);
	add_patterns_from_প্রবেশ(pl, argc, argv, use_stdin ? stdin : রিক্ত);
}

স্থির পূর্ণাঙ্ক modify_pattern_list(সংরচনা repository *repo,
			       সংরচনা strvec *args, পূর্ণাঙ্ক use_stdin,
			       গণনা modify_type m)
{
	পূর্ণাঙ্ক result;
	পূর্ণাঙ্ক changed_config = 0;
	সংরচনা pattern_list *pl = xcalloc(1, পরিমাণ(*pl));
	সংরচনা repo_config_মানসমূহ *cfg = repo_config_মানসমূহ(the_repository);

	বিকল্প (m) {
	পক্ষ ADD:
		যদি (cfg->core_sparse_checkout_cone)
			add_patterns_cone_mode(args->nr, args->v, pl, use_stdin);
		অন্যথা
			add_patterns_literal(args->nr, args->v, pl, use_stdin);
		ভঙ্গ;

	পক্ষ REPLACE:
		add_patterns_from_প্রবেশ(pl, args->nr, args->v,
					use_stdin ? stdin : রিক্ত);
		ভঙ্গ;
	}

	যদি (!cfg->apply_sparse_checkout) {
		set_config(repo, MODE_ALL_PATTERNS);
		cfg->apply_sparse_checkout = 1;
		changed_config = 1;
	}

	result = write_patterns_and_update(repo, pl);

	যদি (result && changed_config)
		set_config(repo, MODE_NO_PATTERNS);

	clear_pattern_list(pl);
	মুক্ত(pl);
	প্রত্যাগম result;
}

স্থির শূন্য sanitize_paths(সংরচনা repository *repo,
			   সংরচনা strvec *args,
			   ধ্রুব বর্ণ *prefix, পূর্ণাঙ্ক skip_checks)
{
	পূর্ণাঙ্ক i;
	সংরচনা repo_config_মানসমূহ *cfg = repo_config_মানসমূহ(the_repository);

	যদি (!args->nr)
		প্রত্যাগম;

	যদি (prefix && *prefix && cfg->core_sparse_checkout_cone) {
		/*
		 * The args are not pathspecs, so unfortunately we
		 * cannot imitate how cmd_add() uses parse_pathspec().
		 */
		পূর্ণাঙ্ক prefix_len = strlen(prefix);

		প্রতি (i = 0; i < args->nr; i++) {
			বর্ণ *prefixed_path = prefix_path(the_repository, prefix,
							  prefix_len, args->v[i]);
			strvec_replace(args, i, prefixed_path);
			মুক্ত(prefixed_path);
		}
	}

	যদি (skip_checks)
		প্রত্যাগম;

	যদি (prefix && *prefix && !cfg->core_sparse_checkout_cone)
		die(_("please run from the toplevel directory in non-cone mode"));

	যদি (cfg->core_sparse_checkout_cone) {
		প্রতি (i = 0; i < args->nr; i++) {
			যদি (args->v[i][0] == '/')
				die(_("specify directories rather than patterns (no leading slash)"));
			যদি (args->v[i][0] == '!')
				die(_("specify directories rather than patterns.  If your directory starts with a '!', pass --skip-checks"));
			যদি (strpbrk(args->v[i], "*?[]"))
				die(_("specify directories rather than patterns.  If your directory really has any of '*?[]\\' in it, pass --skip-checks"));
		}
	}

	প্রতি (i = 0; i < args->nr; i++) {
		সংরচনা cache_entry *ce;
		সংরচনা index_state *index = repo->index;
		পূর্ণাঙ্ক pos = index_name_pos(index, args->v[i], strlen(args->v[i]));

		যদি (pos < 0)
			অনুবর্ত;
		ce = index->cache[pos];
		যদি (S_ISSPARSEDIR(ce->ce_mode))
			অনুবর্ত;

		যদি (cfg->core_sparse_checkout_cone)
			die(_("'%s' is not a directory; to treat it as a directory anyway, rerun with --skip-checks"), args->v[i]);
		অন্যথা
			warning(_("pass a leading slash before paths such as '%s' if you want a single file (see NON-CONE PROBLEMS in the git-sparse-checkout manual)."), args->v[i]);
	}
}

স্থির বর্ণ ধ্রুব * ধ্রুব builtin_sparse_checkout_add_usage[] = {
	N_("git sparse-checkout add [--skip-checks] (--stdin | <patterns>)"),
	রিক্ত
};

স্থির সংরচনা sparse_checkout_add_opts {
	পূর্ণাঙ্ক skip_checks;
	পূর্ণাঙ্ক use_stdin;
} add_opts;

স্থির পূর্ণাঙ্ক sparse_checkout_add(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix,
			       সংরচনা repository *repo)
{
	স্থির সংরচনা option builtin_sparse_checkout_add_options[] = {
		OPT_BOOL_F(0, "skip-checks", &add_opts.skip_checks,
			   N_("skip some sanity checks on the given paths that might give false positives"),
			   PARSE_OPT_NONEG),
		OPT_BOOL(0, "stdin", &add_opts.use_stdin,
			 N_("read patterns from standard in")),
		OPT_END(),
	};
	সংরচনা strvec patterns = STRVEC_INIT;
	পূর্ণাঙ্ক ret;
	সংরচনা repo_config_মানসমূহ *cfg = repo_config_মানসমূহ(the_repository);

	setup_work_বৃক্ষ(the_repository);
	যদি (!cfg->apply_sparse_checkout)
		die(_("no sparse-checkout to add to"));

	repo_read_index(repo);

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_add_options,
			     builtin_sparse_checkout_add_usage, 0);

	প্রতি (পূর্ণাঙ্ক i = 0; i < argc; i++)
		strvec_ঠেলো(&patterns, argv[i]);
	sanitize_paths(repo, &patterns, prefix, add_opts.skip_checks);

	ret = modify_pattern_list(repo, &patterns, add_opts.use_stdin, ADD);

	strvec_clear(&patterns);
	প্রত্যাগম ret;
}

স্থির বর্ণ ধ্রুব * ধ্রুব builtin_sparse_checkout_set_usage[] = {
	N_("git sparse-checkout set [--[no-]cone] [--[no-]sparse-index] [--skip-checks] (--stdin | <patterns>)"),
	রিক্ত
};

স্থির সংরচনা sparse_checkout_set_opts {
	পূর্ণাঙ্ক cone_mode;
	পূর্ণাঙ্ক sparse_index;
	পূর্ণাঙ্ক skip_checks;
	পূর্ণাঙ্ক use_stdin;
} set_opts;

স্থির পূর্ণাঙ্ক sparse_checkout_set(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix,
			       সংরচনা repository *repo)
{
	সংরচনা repo_config_মানসমূহ *cfg = repo_config_মানসমূহ(the_repository);
	পূর্ণাঙ্ক default_patterns_nr = 2;
	ধ্রুব বর্ণ *default_patterns[] = {"/*", "!/*/", রিক্ত};

	স্থির সংরচনা option builtin_sparse_checkout_set_options[] = {
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
	সংরচনা strvec patterns = STRVEC_INIT;
	পূর্ণাঙ্ক ret;

	setup_work_বৃক্ষ(the_repository);
	repo_read_index(repo);

	set_opts.cone_mode = -1;
	set_opts.sparse_index = -1;

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_set_options,
			     builtin_sparse_checkout_set_usage, 0);

	যদি (update_modes(repo, &set_opts.cone_mode, &set_opts.sparse_index))
		প্রত্যাগম 1;

	/*
	 * Cone mode automatically specifies the toplevel directory.  For
	 * non-cone mode, if nothing is specified, manually select just the
	 * top-level directory (much as 'init' would do).
	 */
	যদি (!cfg->core_sparse_checkout_cone && !set_opts.use_stdin && argc == 0) {
		প্রতি (পূর্ণাঙ্ক i = 0; i < default_patterns_nr; i++)
			strvec_ঠেলো(&patterns, default_patterns[i]);
	} অন্যথা {
		প্রতি (পূর্ণাঙ্ক i = 0; i < argc; i++)
			strvec_ঠেলো(&patterns, argv[i]);
		sanitize_paths(repo, &patterns, prefix, set_opts.skip_checks);
	}

	ret = modify_pattern_list(repo, &patterns, set_opts.use_stdin, REPLACE);

	strvec_clear(&patterns);
	প্রত্যাগম ret;
}

স্থির বর্ণ ধ্রুব * ধ্রুব builtin_sparse_checkout_reapply_usage[] = {
	"git sparse-checkout reapply [--[no-]cone] [--[no-]sparse-index]",
	রিক্ত
};

স্থির সংরচনা sparse_checkout_reapply_opts {
	পূর্ণাঙ্ক cone_mode;
	পূর্ণাঙ্ক sparse_index;
} reapply_opts;

স্থির পূর্ণাঙ্ক sparse_checkout_reapply(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv,
				   ধ্রুব বর্ণ *prefix,
				   সংরচনা repository *repo)
{
	স্থির সংরচনা option builtin_sparse_checkout_reapply_options[] = {
		OPT_BOOL(0, "cone", &reapply_opts.cone_mode,
			 N_("initialize the sparse-checkout in cone mode")),
		OPT_BOOL(0, "sparse-index", &reapply_opts.sparse_index,
			 N_("toggle the use of a sparse index")),
		OPT_END(),
	};
	সংরচনা repo_config_মানসমূহ *cfg = repo_config_মানসমূহ(the_repository);

	setup_work_বৃক্ষ(the_repository);
	যদি (!cfg->apply_sparse_checkout)
		die(_("must be in a sparse-checkout to reapply sparsity patterns"));

	reapply_opts.cone_mode = -1;
	reapply_opts.sparse_index = -1;

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_reapply_options,
			     builtin_sparse_checkout_reapply_usage, 0);

	repo_read_index(repo);

	যদি (update_modes(repo, &reapply_opts.cone_mode, &reapply_opts.sparse_index))
		প্রত্যাগম 1;

	প্রত্যাগম update_working_directory(repo, রিক্ত);
}

স্থির বর্ণ ধ্রুব * ধ্রুব builtin_sparse_checkout_clean_usage[] = {
	"git sparse-checkout clean [-n|--dry-run]",
	রিক্ত
};

স্থির পূর্ণাঙ্ক list_file_iterator(ধ্রুব বর্ণ *path, ধ্রুব শূন্য *data)
{
	ধ্রুব বর্ণ *বার্তা = data;

	printf(বার্তা, path);
	প্রত্যাগম 0;
}

স্থির শূন্য list_every_file_in_dir(ধ্রুব বর্ণ *বার্তা,
				   ধ্রুব বর্ণ *directory)
{
	সংরচনা strbuf path = STRBUF_INIT;

	strbuf_addstr(&path, directory);
	for_each_file_in_dir(&path, list_file_iterator, বার্তা);
	strbuf_release(&path);
}

স্থির ধ্রুব বর্ণ *বার্তা_remove = N_("Removing %s\n");
স্থির ধ্রুব বর্ণ *বার্তা_would_remove = N_("Would remove %s\n");

স্থির পূর্ণাঙ্ক sparse_checkout_clean(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv,
				   ধ্রুব বর্ণ *prefix,
				   সংরচনা repository *repo)
{
	সংরচনা strbuf full_path = STRBUF_INIT;
	ধ্রুব বর্ণ *বার্তা = বার্তা_remove;
	size_t worktree_len;
	পূর্ণাঙ্ক force = 0, dry_run = 0, verbose = 0;
	পূর্ণাঙ্ক require_force = 1;
	সংরচনা repo_config_মানসমূহ *cfg = repo_config_মানসমূহ(the_repository);

	সংরচনা option builtin_sparse_checkout_clean_options[] = {
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

	যদি (repo_read_index(repo) < 0)
		die(_("failed to read index"));

	যদি (convert_to_sparse(repo->index, SPARSE_INDEX_MEMORY_ONLY) ||
	    repo->index->sparse_index == INDEX_EXPANDED)
		die(_("failed to convert index to a sparse index; resolve merge conflicts and try again"));

	strbuf_addstr(&full_path, repo->worktree);
	strbuf_addch(&full_path, '/');
	worktree_len = full_path.len;

	প্রতি (size_t i = 0; i < repo->index->cache_nr; i++) {
		সংরচনা cache_entry *ce = repo->index->cache[i];
		যদি (!S_ISSPARSEDIR(ce->ce_mode))
			অনুবর্ত;
		strbuf_setlen(&full_path, worktree_len);
		strbuf_add(&full_path, ce->name, ce->ce_namelen);

		যদি (!is_directory(full_path.buf))
			অনুবর্ত;

		যদি (verbose)
			list_every_file_in_dir(বার্তা, ce->name);
		অন্যথা
			printf(বার্তা, ce->name);

		যদি (dry_run <= 0 &&
		    remove_dir_recursively(&full_path, 0))
			warning_errno(_("failed to remove '%s'"), ce->name);
	}

	strbuf_release(&full_path);
	প্রত্যাগম 0;
}

স্থির বর্ণ ধ্রুব * ধ্রুব builtin_sparse_checkout_disable_usage[] = {
	"git sparse-checkout disable",
	রিক্ত
};

স্থির পূর্ণাঙ্ক sparse_checkout_disable(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv,
				   ধ্রুব বর্ণ *prefix,
				   সংরচনা repository *repo)
{
	স্থির সংরচনা option builtin_sparse_checkout_disable_options[] = {
		OPT_END(),
	};
	সংরচনা pattern_list pl;
	সংরচনা repo_config_মানসমূহ *cfg = repo_config_মানসমূহ(the_repository);

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
	repo_read_index(repo);

	memset(&pl, 0, পরিমাণ(pl));
	hashmap_init(&pl.recursive_hashmap, pl_hashmap_cmp, রিক্ত, 0);
	hashmap_init(&pl.parent_hashmap, pl_hashmap_cmp, রিক্ত, 0);
	pl.use_cone_patterns = 0;
	cfg->apply_sparse_checkout = 1;

	add_pattern("/*", empty_base, 0, &pl, 0);

	prepare_repo_settings(the_repository);
	repo->settings.sparse_index = 0;

	যদি (update_working_directory(repo, &pl))
		die(_("error while refreshing working directory"));

	clear_pattern_list(&pl);
	প্রত্যাগম set_config(repo, MODE_NO_PATTERNS);
}

স্থির বর্ণ ধ্রুব * ধ্রুব builtin_sparse_checkout_check_rules_usage[] = {
	N_("git sparse-checkout check-rules [-z] [--skip-checks]"
	   "[--[no-]cone] [--rules-file <file>]"),
	রিক্ত
};

স্থির সংরচনা sparse_checkout_check_rules_opts {
	পূর্ণাঙ্ক cone_mode;
	পূর্ণাঙ্ক null_termination;
	বর্ণ *rules_file;
} check_rules_opts;

স্থির পূর্ণাঙ্ক check_rules(সংরচনা repository *repo,
		       সংরচনা pattern_list *pl,
		       পূর্ণাঙ্ক null_terminated)
{
	সংরচনা strbuf সারি = STRBUF_INIT;
	সংরচনা strbuf unquoted = STRBUF_INIT;
	বর্ণ *path;
	পূর্ণাঙ্ক সারি_terminator = null_terminated ? 0 : '\n';
	strbuf_getline_fn getline_fn = null_terminated ? strbuf_getline_nul
		: strbuf_getline;
	repo->index->sparse_checkout_patterns = pl;
	যাবত্ (!getline_fn(&সারি, stdin)) {
		path = সারি.buf;
		যদি (!null_terminated && সারি.buf[0] == '"') {
			strbuf_reset(&unquoted);
			যদি (unquote_c_style(&unquoted, সারি.buf, রিক্ত))
				die(_("unable to unquote C-style string '%s'"),
					সারি.buf);

			path = unquoted.buf;
		}

		যদি (path_in_sparse_checkout(path, repo->index))
			write_name_quoted(path, stdout, সারি_terminator);
	}
	strbuf_release(&সারি);
	strbuf_release(&unquoted);

	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক sparse_checkout_check_rules(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix,
				       সংরচনা repository *repo)
{
	স্থির সংরচনা option builtin_sparse_checkout_check_rules_options[] = {
		OPT_BOOL('z', রিক্ত, &check_rules_opts.null_termination,
			 N_("terminate input and output files by a NUL character")),
		OPT_BOOL(0, "cone", &check_rules_opts.cone_mode,
			 N_("when used with --rules-file interpret patterns as cone mode patterns")),
		OPT_FILENAME(0, "rules-file", &check_rules_opts.rules_file,
			 N_("use patterns in <file> instead of the current ones.")),
		OPT_END(),
	};

	FILE *fp;
	পূর্ণাঙ্ক ret;
	সংরচনা pattern_list pl = {0};
	সংরচনা repo_config_মানসমূহ *cfg = repo_config_মানসমূহ(the_repository);
	বর্ণ *sparse_filename;
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
						   রিক্ত, 0))
			die(_("unable to load existing sparse-checkout patterns"));
		মুক্ত(sparse_filename);
	}

	ret = check_rules(repo, &pl, check_rules_opts.null_termination);
	clear_pattern_list(&pl);
	মুক্ত(check_rules_opts.rules_file);
	প্রত্যাগম ret;
}

পূর্ণাঙ্ক cmd_sparse_checkout(পূর্ণাঙ্ক argc,
			ধ্রুব বর্ণ **argv,
			ধ্রুব বর্ণ *prefix,
			সংরচনা repository *repo)
{
	parse_opt_subcommand_fn *fn = রিক্ত;
	সংরচনা option builtin_sparse_checkout_options[] = {
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

	repo_config(the_repository, git_default_config, রিক্ত);

	prepare_repo_settings(repo);
	repo->settings.command_requires_full_index = 0;

	প্রত্যাগম fn(argc, argv, prefix, repo);
}
