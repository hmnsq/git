#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "abspath.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "dir.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "object-file.h"
#அந்தர்பாவ "object-name.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "path.h"
#அந்தர்பாவ "pathspec.h"
#அந்தர்பாவ "strbuf.h"
#அந்தர்பாவ "string-list.h"
#அந்தர்பாவ "lockfile.h"
#அந்தர்பாவ "unpack-trees.h"
#அந்தர்பாவ "quote.h"
#அந்தர்பாவ "setup.h"
#அந்தர்பாவ "sparse-index.h"
#அந்தர்பாவ "worktree.h"

நிலையான மாறிலி எழுத்து *empty_base = "";

நிலையான எழுத்து மாறிலி * மாறிலி builtin_sparse_checkout_usage[] = {
	N_("git sparse-checkout (init | list | set | add | reapply | disable | check-rules | clean) [<options>]"),
	வெற்று
};

நிலையான வெறுமை write_patterns_to_file(FILE *fp, கட்டமைப்பு pattern_list *pl)
{
	முழு i;

	க்கு (i = 0; i < pl->nr; i++) {
		கட்டமைப்பு path_pattern *p = pl->patterns[i];

		எனில் (p->flags & PATTERN_FLAG_NEGATIVE)
			fprintf(fp, "!");

		fprintf(fp, "%s", p->pattern);

		எனில் (p->flags & PATTERN_FLAG_MUSTBEDIR)
			fprintf(fp, "/");

		fprintf(fp, "\n");
	}
}

நிலையான எழுத்து மாறிலி * மாறிலி builtin_sparse_checkout_list_usage[] = {
	"git sparse-checkout list",
	வெற்று
};

நிலையான முழு sparse_checkout_list(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
				கட்டமைப்பு repository *repo UNUSED)
{
	நிலையான கட்டமைப்பு option builtin_sparse_checkout_list_options[] = {
		OPT_END(),
	};
	கட்டமைப்பு pattern_list pl;
	எழுத்து *sparse_filename;
	முழு res;
	கட்டமைப்பு repo_config_மதிப்புகள் *cfg = repo_config_மதிப்புகள்(the_repository);

	setup_work_மரம்(the_repository);
	எனில் (!cfg->apply_sparse_checkout)
		die(_("this worktree is not sparse"));

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_list_options,
			     builtin_sparse_checkout_list_usage, 0);

	memset(&pl, 0, அளவு(pl));

	pl.use_cone_patterns = cfg->core_sparse_checkout_cone;

	sparse_filename = get_sparse_checkout_filename();
	res = add_patterns_from_file_to_list(sparse_filename, "", 0, &pl, வெற்று, 0);
	விடுவி(sparse_filename);

	எனில் (res < 0) {
		warning(_("this worktree is not sparse (sparse-checkout file may not exist)"));
		திரும்பு 0;
	}

	எனில் (pl.use_cone_patterns) {
		முழு i;
		கட்டமைப்பு pattern_entry *pe;
		கட்டமைப்பு hashmap_iter iter;
		கட்டமைப்பு string_list sl = STRING_LIST_INIT_DUP;

		hashmap_for_each_entry(&pl.recursive_hashmap, &iter, pe, ent) {
			/* pe->pattern starts with "/", skip it */
			string_list_append(&sl, pe->pattern + 1);
		}

		string_list_sort_u(&sl, 0);

		க்கு (i = 0; i < sl.nr; i++) {
			quote_c_style(sl.பொருட்கள்[i].string, வெற்று, stdout, 0);
			printf("\n");
		}

		string_list_clear(&sl, 0);
	} இல்லையெனில் {
		write_patterns_to_file(stdout, &pl);
	}

	clear_pattern_list(&pl);

	திரும்பு 0;
}

நிலையான வெறுமை clean_tracked_sparse_directories(கட்டமைப்பு repository *r)
{
	முழு i, was_full = 0;
	கட்டமைப்பு strbuf path = STRBUF_INIT;
	size_t pathlen;
	கட்டமைப்பு string_list_item *item;
	கட்டமைப்பு string_list sparse_dirs = STRING_LIST_INIT_DUP;

	/*
	 * If we are not using cone mode patterns, then we cannot
	 * delete directories outside of the sparse cone.
	 */
	எனில் (!r || !r->index || !r->worktree)
		திரும்பு;
	எனில் (init_sparse_checkout_patterns(r->index) ||
	    !r->index->sparse_checkout_patterns->use_cone_patterns)
		திரும்பு;

	/*
	 * Use the sparse index as a data structure to assist finding
	 * directories that are safe to delete. This conversion to a
	 * sparse index will not delete directories that contain
	 * conflicted entries or submodules.
	 */
	எனில் (r->index->sparse_index == INDEX_EXPANDED) {
		/*
		 * If something, such as a merge conflict or other concern,
		 * prevents us from converting to a sparse index, then do
		 * not try deleting files.
		 */
		எனில் (convert_to_sparse(r->index, SPARSE_INDEX_MEMORY_ONLY))
			திரும்பு;
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
	க்கு (i = 0; i < r->index->cache_nr; i++) {
		கட்டமைப்பு cache_entry *ce = r->index->cache[i];

		எனில் (S_ISSPARSEDIR(ce->ce_mode) &&
		    repo_file_exists(r, ce->name))
			string_list_append(&sparse_dirs, ce->name);
	}

	for_each_string_list_item(item, &sparse_dirs) {
		கட்டமைப்பு dir_struct dir = DIR_INIT;
		கட்டமைப்பு pathspec p = { 0 };
		கட்டமைப்பு strvec s = STRVEC_INIT;

		strbuf_setlen(&path, pathlen);
		strbuf_addstr(&path, item->string);

		dir.flags |= DIR_SHOW_IGNORED_TOO;

		setup_standard_excludes(&dir);
		strvec_தள்ளு(&s, path.buf);

		parse_pathspec(&p, PATHSPEC_GLOB, 0, வெற்று, s.v);
		fill_directory(&dir, r->index, &p);

		எனில் (dir.nr) {
			warning(_("directory '%s' contains untracked files,"
				  " but is not in the sparse-checkout cone"),
				item->string);
		} இல்லையெனில் எனில் (remove_dir_recursively(&path, 0)) {
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

	எனில் (was_full)
		ensure_full_index(r->index);
}

நிலையான முழு update_working_directory(கட்டமைப்பு repository *r,
				    கட்டமைப்பு pattern_list *pl)
{
	எண்ணிக்கை update_sparsity_result result;
	கட்டமைப்பு unpack_trees_options o;
	கட்டமைப்பு lock_file lock_file = LOCK_INIT;
	கட்டமைப்பு pattern_list *old_pl;

	/* If no branch has been checked out, there are no updates to make. */
	எனில் (is_index_unborn(r->index))
		திரும்பு UPDATE_SPARSITY_SUCCESS;

	old_pl = r->index->sparse_checkout_patterns;
	r->index->sparse_checkout_patterns = pl;

	memset(&o, 0, அளவு(o));
	o.verbose_update = isatty(2);
	o.update = 1;
	o.head_idx = -1;
	o.src_index = r->index;
	o.dst_index = r->index;
	o.skip_sparse_checkout = 0;

	setup_work_மரம்(the_repository);

	repo_hold_locked_index(r, &lock_file, LOCK_DIE_ON_ERROR);

	setup_unpack_trees_porcelain(&o, "sparse-checkout");
	result = update_sparsity(&o, pl);
	clear_unpack_trees_porcelain(&o);

	எனில் (result == UPDATE_SPARSITY_WARNINGS)
		/*
		 * We don't do any special handling of warnings from untracked
		 * files in the way or dirty entries that can't be removed.
		 */
		result = UPDATE_SPARSITY_SUCCESS;
	எனில் (result == UPDATE_SPARSITY_SUCCESS)
		write_locked_index(r->index, &lock_file, COMMIT_LOCK);
	இல்லையெனில்
		rollback_lock_file(&lock_file);

	clean_tracked_sparse_directories(r);

	எனில் (r->index->sparse_checkout_patterns != pl) {
		clear_pattern_list(r->index->sparse_checkout_patterns);
		FREE_AND_NULL(r->index->sparse_checkout_patterns);
	}
	r->index->sparse_checkout_patterns = old_pl;

	திரும்பு result;
}

நிலையான எழுத்து *escaped_pattern(எழுத்து *pattern)
{
	எழுத்து *p = pattern;
	கட்டமைப்பு strbuf final = STRBUF_INIT;

	போது (*p) {
		எனில் (is_glob_special(*p))
			strbuf_addch(&final, '\\');

		strbuf_addch(&final, *p);
		p++;
	}

	திரும்பு strbuf_detach(&final, வெற்று);
}

நிலையான வெறுமை write_cone_to_file(FILE *fp, கட்டமைப்பு pattern_list *pl)
{
	முழு i;
	கட்டமைப்பு pattern_entry *pe;
	கட்டமைப்பு hashmap_iter iter;
	கட்டமைப்பு string_list sl = STRING_LIST_INIT_DUP;
	கட்டமைப்பு strbuf parent_pattern = STRBUF_INIT;

	hashmap_for_each_entry(&pl->parent_hashmap, &iter, pe, ent) {
		எனில் (hashmap_get_entry(&pl->recursive_hashmap, pe, ent, வெற்று))
			தொடர்;

		எனில் (!hashmap_contains_parent(&pl->recursive_hashmap,
					     pe->pattern,
					     &parent_pattern))
			string_list_append(&sl, pe->pattern);
	}

	string_list_sort_u(&sl, 0);

	fprintf(fp, "/*\n!/*/\n");

	க்கு (i = 0; i < sl.nr; i++) {
		எழுத்து *pattern = escaped_pattern(sl.பொருட்கள்[i].string);

		எனில் (strlen(pattern))
			fprintf(fp, "%s/\n!%s/*/\n", pattern, pattern);
		விடுவி(pattern);
	}

	string_list_clear(&sl, 0);

	hashmap_for_each_entry(&pl->recursive_hashmap, &iter, pe, ent) {
		எனில் (!hashmap_contains_parent(&pl->recursive_hashmap,
					     pe->pattern,
					     &parent_pattern))
			string_list_append(&sl, pe->pattern);
	}

	strbuf_release(&parent_pattern);

	string_list_sort_u(&sl, 0);

	க்கு (i = 0; i < sl.nr; i++) {
		எழுத்து *pattern = escaped_pattern(sl.பொருட்கள்[i].string);
		fprintf(fp, "%s/\n", pattern);
		விடுவி(pattern);
	}

	string_list_clear(&sl, 0);
}

நிலையான முழு write_patterns_and_update(கட்டமைப்பு repository *repo,
				     கட்டமைப்பு pattern_list *pl)
{
	எழுத்து *sparse_filename;
	FILE *fp;
	கட்டமைப்பு lock_file lk = LOCK_INIT;
	முழு result;
	கட்டமைப்பு repo_config_மதிப்புகள் *cfg = repo_config_மதிப்புகள்(the_repository);

	sparse_filename = get_sparse_checkout_filename();

	எனில் (safe_create_leading_directories(repo, sparse_filename))
		die(_("failed to create directory for sparse-checkout file"));

	repo_hold_lock_file_for_update(repo, &lk, sparse_filename,
				       LOCK_DIE_ON_ERROR);

	result = update_working_directory(repo, pl);
	எனில் (result) {
		rollback_lock_file(&lk);
		update_working_directory(repo, வெற்று);
		செல் out;
	}

	fp = fdopen_lock_file(&lk, "w");
	எனில் (!fp)
		die_errno(_("unable to fdopen %s"), get_lock_file_path(&lk));

	எனில் (cfg->core_sparse_checkout_cone)
		write_cone_to_file(fp, pl);
	இல்லையெனில்
		write_patterns_to_file(fp, pl);

	எனில் (commit_lock_file(&lk))
		die_errno(_("unable to write %s"), sparse_filename);

out:
	clear_pattern_list(pl);
	விடுவி(sparse_filename);
	திரும்பு result;
}

எண்ணிக்கை sparse_checkout_mode {
	MODE_NO_PATTERNS = 0,
	MODE_ALL_PATTERNS = 1,
	MODE_CONE_PATTERNS = 2,
};

நிலையான முழு set_config(கட்டமைப்பு repository *repo,
		      எண்ணிக்கை sparse_checkout_mode mode)
{
	/* Update to use worktree config, if not already. */
	எனில் (init_worktree_config(repo)) {
		error(_("failed to initialize worktree config"));
		திரும்பு 1;
	}

	எனில் (repo_config_set_worktree_gently(repo,
					    "core.sparseCheckout",
					    mode ? "true" : "false") ||
	    repo_config_set_worktree_gently(repo,
					    "core.sparseCheckoutCone",
					    mode == MODE_CONE_PATTERNS ?
						"true" : "false"))
		திரும்பு 1;

	எனில் (mode == MODE_NO_PATTERNS)
		திரும்பு set_sparse_index_config(repo, 0);

	திரும்பு 0;
}

நிலையான எண்ணிக்கை sparse_checkout_mode update_cone_mode(முழு *cone_mode) {
	கட்டமைப்பு repo_config_மதிப்புகள் *cfg = repo_config_மதிப்புகள்(the_repository);

	/* If not specified, use previous definition of cone mode */
	எனில் (*cone_mode == -1 && cfg->apply_sparse_checkout)
		*cone_mode = cfg->core_sparse_checkout_cone;

	/* Set cone/non-cone mode appropriately */
	cfg->apply_sparse_checkout = 1;
	எனில் (*cone_mode == 1 || *cone_mode == -1) {
		cfg->core_sparse_checkout_cone = 1;
		திரும்பு MODE_CONE_PATTERNS;
	}
	cfg->core_sparse_checkout_cone = 0;
	திரும்பு MODE_ALL_PATTERNS;
}

நிலையான முழு update_modes(கட்டமைப்பு repository *repo, முழு *cone_mode, முழு *sparse_index)
{
	முழு mode, record_mode;
	கட்டமைப்பு repo_config_மதிப்புகள் *cfg = repo_config_மதிப்புகள்(the_repository);

	/* Determine if we need to record the mode; ensure sparse checkout on */
	record_mode = (*cone_mode != -1) || !cfg->apply_sparse_checkout;

	mode = update_cone_mode(cone_mode);
	எனில் (record_mode && set_config(repo, mode))
		திரும்பு 1;

	/* Set sparse-index/non-sparse-index mode if specified */
	எனில் (*sparse_index >= 0) {
		எனில் (set_sparse_index_config(repo, *sparse_index) < 0)
			die(_("failed to modify sparse-index config"));

		/* force an index rewrite */
		repo_read_index(repo);
		repo->index->updated_workdir = 1;

		எனில் (!*sparse_index)
			ensure_full_index(repo->index);
	}

	திரும்பு 0;
}

நிலையான எழுத்து மாறிலி * மாறிலி builtin_sparse_checkout_init_usage[] = {
	"git sparse-checkout init [--cone] [--[no-]sparse-index]",
	வெற்று
};

நிலையான கட்டமைப்பு sparse_checkout_init_opts {
	முழு cone_mode;
	முழு sparse_index;
} init_opts;

நிலையான முழு sparse_checkout_init(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
				கட்டமைப்பு repository *repo)
{
	கட்டமைப்பு pattern_list pl;
	எழுத்து *sparse_filename;
	முழு res;
	கட்டமைப்பு object_id oid;

	நிலையான கட்டமைப்பு option builtin_sparse_checkout_init_options[] = {
		OPT_BOOL(0, "cone", &init_opts.cone_mode,
			 N_("initialize the sparse-checkout in cone mode")),
		OPT_BOOL(0, "sparse-index", &init_opts.sparse_index,
			 N_("toggle the use of a sparse index")),
		OPT_END(),
	};

	setup_work_மரம்(the_repository);
	repo_read_index(repo);

	init_opts.cone_mode = -1;
	init_opts.sparse_index = -1;

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_init_options,
			     builtin_sparse_checkout_init_usage, 0);

	எனில் (update_modes(repo, &init_opts.cone_mode, &init_opts.sparse_index))
		திரும்பு 1;

	memset(&pl, 0, அளவு(pl));

	sparse_filename = get_sparse_checkout_filename();
	res = add_patterns_from_file_to_list(sparse_filename, "", 0, &pl, வெற்று, 0);

	/* If we already have a sparse-checkout file, use it. */
	எனில் (res >= 0) {
		விடுவி(sparse_filename);
		clear_pattern_list(&pl);
		திரும்பு update_working_directory(repo, வெற்று);
	}

	எனில் (repo_get_oid(repo, "HEAD", &oid)) {
		FILE *fp;

		/* assume we are in a fresh repo, but update the sparse-checkout file */
		எனில் (safe_create_leading_directories(repo, sparse_filename))
			die(_("unable to create leading directories of %s"),
			    sparse_filename);
		fp = xfopen(sparse_filename, "w");
		எனில் (!fp)
			die(_("failed to open '%s'"), sparse_filename);

		விடுவி(sparse_filename);
		fprintf(fp, "/*\n!/*/\n");
		fclose(fp);
		திரும்பு 0;
	}

	விடுவி(sparse_filename);

	add_pattern("/*", empty_base, 0, &pl, 0);
	add_pattern("!/*/", empty_base, 0, &pl, 0);
	pl.use_cone_patterns = init_opts.cone_mode;

	திரும்பு write_patterns_and_update(repo, &pl);
}

நிலையான வெறுமை செருகு_recursive_pattern(கட்டமைப்பு pattern_list *pl, கட்டமைப்பு strbuf *path)
{
	கட்டமைப்பு pattern_entry *e = xmalloc(அளவு(*e));
	e->patternlen = path->len;
	e->pattern = strbuf_detach(path, வெற்று);
	hashmap_entry_init(&e->ent, fspathhash(e->pattern));

	hashmap_add(&pl->recursive_hashmap, &e->ent);

	போது (e->patternlen) {
		எழுத்து *slash = strrchr(e->pattern, '/');
		எழுத்து *oldpattern = e->pattern;
		size_t newlen;
		கட்டமைப்பு pattern_entry *dup;

		எனில் (!slash || slash == e->pattern)
			நிறுத்து;

		newlen = slash - e->pattern;
		e = xmalloc(அளவு(கட்டமைப்பு pattern_entry));
		e->patternlen = newlen;
		e->pattern = xstrndup(oldpattern, newlen);
		hashmap_entry_init(&e->ent, fspathhash(e->pattern));

		dup = hashmap_get_entry(&pl->parent_hashmap, e, ent, வெற்று);
		எனில் (!dup) {
			hashmap_add(&pl->parent_hashmap, &e->ent);
		} இல்லையெனில் {
			விடுவி(e->pattern);
			விடுவி(e);
			e = dup;
		}
	}
}

நிலையான வெறுமை strbuf_to_cone_pattern(கட்டமைப்பு strbuf *வரி, கட்டமைப்பு pattern_list *pl)
{
	strbuf_trim(வரி);

	strbuf_trim_trailing_dir_sep(வரி);

	எனில் (strbuf_normalize_path(வரி))
		die(_("could not normalize path %s"), வரி->buf);

	எனில் (!வரி->len)
		திரும்பு;

	எனில் (வரி->buf[0] != '/')
		strbuf_insertstr(வரி, 0, "/");

	செருகு_recursive_pattern(pl, வரி);
}

நிலையான வெறுமை add_patterns_from_உள்ளீடு(கட்டமைப்பு pattern_list *pl,
				    முழு argc, மாறிலி எழுத்து **argv,
				    FILE *file)
{
	முழு i;
	கட்டமைப்பு repo_config_மதிப்புகள் *cfg = repo_config_மதிப்புகள்(the_repository);

	எனில் (cfg->core_sparse_checkout_cone) {
		கட்டமைப்பு strbuf வரி = STRBUF_INIT;

		hashmap_init(&pl->recursive_hashmap, pl_hashmap_cmp, வெற்று, 0);
		hashmap_init(&pl->parent_hashmap, pl_hashmap_cmp, வெற்று, 0);
		pl->use_cone_patterns = 1;

		எனில் (file) {
			கட்டமைப்பு strbuf unquoted = STRBUF_INIT;
			போது (!strbuf_getline(&வரி, file)) {
				எனில் (வரி.buf[0] == '"') {
					strbuf_reset(&unquoted);
					எனில் (unquote_c_style(&unquoted, வரி.buf, வெற்று))
						die(_("unable to unquote C-style string '%s'"),
						வரி.buf);

					strbuf_swap(&unquoted, &வரி);
				}

				strbuf_to_cone_pattern(&வரி, pl);
			}

			strbuf_release(&unquoted);
		} இல்லையெனில் {
			க்கு (i = 0; i < argc; i++) {
				strbuf_setlen(&வரி, 0);
				strbuf_addstr(&வரி, argv[i]);
				strbuf_to_cone_pattern(&வரி, pl);
			}
		}
		strbuf_release(&வரி);
	} இல்லையெனில் {
		எனில் (file) {
			கட்டமைப்பு strbuf வரி = STRBUF_INIT;

			போது (!strbuf_getline(&வரி, file))
				add_pattern(வரி.buf, empty_base, 0, pl, 0);

			strbuf_release(&வரி);
		} இல்லையெனில் {
			க்கு (i = 0; i < argc; i++)
				add_pattern(argv[i], empty_base, 0, pl, 0);
		}
	}
}

எண்ணிக்கை modify_type {
	REPLACE,
	ADD,
};

நிலையான வெறுமை add_patterns_cone_mode(முழு argc, மாறிலி எழுத்து **argv,
				   கட்டமைப்பு pattern_list *pl,
				   முழு use_stdin)
{
	கட்டமைப்பு strbuf buffer = STRBUF_INIT;
	கட்டமைப்பு pattern_entry *pe;
	கட்டமைப்பு hashmap_iter iter;
	கட்டமைப்பு pattern_list existing;
	கட்டமைப்பு repo_config_மதிப்புகள் *cfg = repo_config_மதிப்புகள்(the_repository);
	எழுத்து *sparse_filename = get_sparse_checkout_filename();

	add_patterns_from_உள்ளீடு(pl, argc, argv,
				use_stdin ? stdin : வெற்று);

	memset(&existing, 0, அளவு(existing));
	existing.use_cone_patterns = cfg->core_sparse_checkout_cone;

	எனில் (add_patterns_from_file_to_list(sparse_filename, "", 0,
					   &existing, வெற்று, 0))
		die(_("unable to load existing sparse-checkout patterns"));
	விடுவி(sparse_filename);

	எனில் (!existing.use_cone_patterns)
		die(_("existing sparse-checkout patterns do not use cone mode"));

	hashmap_for_each_entry(&existing.recursive_hashmap, &iter, pe, ent) {
		எனில் (!hashmap_contains_parent(&pl->recursive_hashmap,
					pe->pattern, &buffer) ||
		    !hashmap_contains_parent(&pl->parent_hashmap,
					pe->pattern, &buffer)) {
			strbuf_reset(&buffer);
			strbuf_addstr(&buffer, pe->pattern);
			செருகு_recursive_pattern(pl, &buffer);
		}
	}

	clear_pattern_list(&existing);
	strbuf_release(&buffer);
}

நிலையான வெறுமை add_patterns_literal(முழு argc, மாறிலி எழுத்து **argv,
				 கட்டமைப்பு pattern_list *pl,
				 முழு use_stdin)
{
	எழுத்து *sparse_filename = get_sparse_checkout_filename();
	எனில் (add_patterns_from_file_to_list(sparse_filename, "", 0,
					   pl, வெற்று, 0))
		die(_("unable to load existing sparse-checkout patterns"));
	விடுவி(sparse_filename);
	add_patterns_from_உள்ளீடு(pl, argc, argv, use_stdin ? stdin : வெற்று);
}

நிலையான முழு modify_pattern_list(கட்டமைப்பு repository *repo,
			       கட்டமைப்பு strvec *args, முழு use_stdin,
			       எண்ணிக்கை modify_type m)
{
	முழு result;
	முழு changed_config = 0;
	கட்டமைப்பு pattern_list *pl = xcalloc(1, அளவு(*pl));
	கட்டமைப்பு repo_config_மதிப்புகள் *cfg = repo_config_மதிப்புகள்(the_repository);

	தேர்வு (m) {
	நிலை ADD:
		எனில் (cfg->core_sparse_checkout_cone)
			add_patterns_cone_mode(args->nr, args->v, pl, use_stdin);
		இல்லையெனில்
			add_patterns_literal(args->nr, args->v, pl, use_stdin);
		நிறுத்து;

	நிலை REPLACE:
		add_patterns_from_உள்ளீடு(pl, args->nr, args->v,
					use_stdin ? stdin : வெற்று);
		நிறுத்து;
	}

	எனில் (!cfg->apply_sparse_checkout) {
		set_config(repo, MODE_ALL_PATTERNS);
		cfg->apply_sparse_checkout = 1;
		changed_config = 1;
	}

	result = write_patterns_and_update(repo, pl);

	எனில் (result && changed_config)
		set_config(repo, MODE_NO_PATTERNS);

	clear_pattern_list(pl);
	விடுவி(pl);
	திரும்பு result;
}

நிலையான வெறுமை sanitize_paths(கட்டமைப்பு repository *repo,
			   கட்டமைப்பு strvec *args,
			   மாறிலி எழுத்து *prefix, முழு skip_checks)
{
	முழு i;
	கட்டமைப்பு repo_config_மதிப்புகள் *cfg = repo_config_மதிப்புகள்(the_repository);

	எனில் (!args->nr)
		திரும்பு;

	எனில் (prefix && *prefix && cfg->core_sparse_checkout_cone) {
		/*
		 * The args are not pathspecs, so unfortunately we
		 * cannot imitate how cmd_add() uses parse_pathspec().
		 */
		முழு prefix_len = strlen(prefix);

		க்கு (i = 0; i < args->nr; i++) {
			எழுத்து *prefixed_path = prefix_path(the_repository, prefix,
							  prefix_len, args->v[i]);
			strvec_replace(args, i, prefixed_path);
			விடுவி(prefixed_path);
		}
	}

	எனில் (skip_checks)
		திரும்பு;

	எனில் (prefix && *prefix && !cfg->core_sparse_checkout_cone)
		die(_("please run from the toplevel directory in non-cone mode"));

	எனில் (cfg->core_sparse_checkout_cone) {
		க்கு (i = 0; i < args->nr; i++) {
			எனில் (args->v[i][0] == '/')
				die(_("specify directories rather than patterns (no leading slash)"));
			எனில் (args->v[i][0] == '!')
				die(_("specify directories rather than patterns.  If your directory starts with a '!', pass --skip-checks"));
			எனில் (strpbrk(args->v[i], "*?[]"))
				die(_("specify directories rather than patterns.  If your directory really has any of '*?[]\\' in it, pass --skip-checks"));
		}
	}

	க்கு (i = 0; i < args->nr; i++) {
		கட்டமைப்பு cache_entry *ce;
		கட்டமைப்பு index_state *index = repo->index;
		முழு pos = index_name_pos(index, args->v[i], strlen(args->v[i]));

		எனில் (pos < 0)
			தொடர்;
		ce = index->cache[pos];
		எனில் (S_ISSPARSEDIR(ce->ce_mode))
			தொடர்;

		எனில் (cfg->core_sparse_checkout_cone)
			die(_("'%s' is not a directory; to treat it as a directory anyway, rerun with --skip-checks"), args->v[i]);
		இல்லையெனில்
			warning(_("pass a leading slash before paths such as '%s' if you want a single file (see NON-CONE PROBLEMS in the git-sparse-checkout manual)."), args->v[i]);
	}
}

நிலையான எழுத்து மாறிலி * மாறிலி builtin_sparse_checkout_add_usage[] = {
	N_("git sparse-checkout add [--skip-checks] (--stdin | <patterns>)"),
	வெற்று
};

நிலையான கட்டமைப்பு sparse_checkout_add_opts {
	முழு skip_checks;
	முழு use_stdin;
} add_opts;

நிலையான முழு sparse_checkout_add(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
			       கட்டமைப்பு repository *repo)
{
	நிலையான கட்டமைப்பு option builtin_sparse_checkout_add_options[] = {
		OPT_BOOL_F(0, "skip-checks", &add_opts.skip_checks,
			   N_("skip some sanity checks on the given paths that might give false positives"),
			   PARSE_OPT_NONEG),
		OPT_BOOL(0, "stdin", &add_opts.use_stdin,
			 N_("read patterns from standard in")),
		OPT_END(),
	};
	கட்டமைப்பு strvec patterns = STRVEC_INIT;
	முழு ret;
	கட்டமைப்பு repo_config_மதிப்புகள் *cfg = repo_config_மதிப்புகள்(the_repository);

	setup_work_மரம்(the_repository);
	எனில் (!cfg->apply_sparse_checkout)
		die(_("no sparse-checkout to add to"));

	repo_read_index(repo);

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_add_options,
			     builtin_sparse_checkout_add_usage, 0);

	க்கு (முழு i = 0; i < argc; i++)
		strvec_தள்ளு(&patterns, argv[i]);
	sanitize_paths(repo, &patterns, prefix, add_opts.skip_checks);

	ret = modify_pattern_list(repo, &patterns, add_opts.use_stdin, ADD);

	strvec_clear(&patterns);
	திரும்பு ret;
}

நிலையான எழுத்து மாறிலி * மாறிலி builtin_sparse_checkout_set_usage[] = {
	N_("git sparse-checkout set [--[no-]cone] [--[no-]sparse-index] [--skip-checks] (--stdin | <patterns>)"),
	வெற்று
};

நிலையான கட்டமைப்பு sparse_checkout_set_opts {
	முழு cone_mode;
	முழு sparse_index;
	முழு skip_checks;
	முழு use_stdin;
} set_opts;

நிலையான முழு sparse_checkout_set(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
			       கட்டமைப்பு repository *repo)
{
	கட்டமைப்பு repo_config_மதிப்புகள் *cfg = repo_config_மதிப்புகள்(the_repository);
	முழு default_patterns_nr = 2;
	மாறிலி எழுத்து *default_patterns[] = {"/*", "!/*/", வெற்று};

	நிலையான கட்டமைப்பு option builtin_sparse_checkout_set_options[] = {
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
	கட்டமைப்பு strvec patterns = STRVEC_INIT;
	முழு ret;

	setup_work_மரம்(the_repository);
	repo_read_index(repo);

	set_opts.cone_mode = -1;
	set_opts.sparse_index = -1;

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_set_options,
			     builtin_sparse_checkout_set_usage, 0);

	எனில் (update_modes(repo, &set_opts.cone_mode, &set_opts.sparse_index))
		திரும்பு 1;

	/*
	 * Cone mode automatically specifies the toplevel directory.  For
	 * non-cone mode, if nothing is specified, manually select just the
	 * top-level directory (much as 'init' would do).
	 */
	எனில் (!cfg->core_sparse_checkout_cone && !set_opts.use_stdin && argc == 0) {
		க்கு (முழு i = 0; i < default_patterns_nr; i++)
			strvec_தள்ளு(&patterns, default_patterns[i]);
	} இல்லையெனில் {
		க்கு (முழு i = 0; i < argc; i++)
			strvec_தள்ளு(&patterns, argv[i]);
		sanitize_paths(repo, &patterns, prefix, set_opts.skip_checks);
	}

	ret = modify_pattern_list(repo, &patterns, set_opts.use_stdin, REPLACE);

	strvec_clear(&patterns);
	திரும்பு ret;
}

நிலையான எழுத்து மாறிலி * மாறிலி builtin_sparse_checkout_reapply_usage[] = {
	"git sparse-checkout reapply [--[no-]cone] [--[no-]sparse-index]",
	வெற்று
};

நிலையான கட்டமைப்பு sparse_checkout_reapply_opts {
	முழு cone_mode;
	முழு sparse_index;
} reapply_opts;

நிலையான முழு sparse_checkout_reapply(முழு argc, மாறிலி எழுத்து **argv,
				   மாறிலி எழுத்து *prefix,
				   கட்டமைப்பு repository *repo)
{
	நிலையான கட்டமைப்பு option builtin_sparse_checkout_reapply_options[] = {
		OPT_BOOL(0, "cone", &reapply_opts.cone_mode,
			 N_("initialize the sparse-checkout in cone mode")),
		OPT_BOOL(0, "sparse-index", &reapply_opts.sparse_index,
			 N_("toggle the use of a sparse index")),
		OPT_END(),
	};
	கட்டமைப்பு repo_config_மதிப்புகள் *cfg = repo_config_மதிப்புகள்(the_repository);

	setup_work_மரம்(the_repository);
	எனில் (!cfg->apply_sparse_checkout)
		die(_("must be in a sparse-checkout to reapply sparsity patterns"));

	reapply_opts.cone_mode = -1;
	reapply_opts.sparse_index = -1;

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_reapply_options,
			     builtin_sparse_checkout_reapply_usage, 0);

	repo_read_index(repo);

	எனில் (update_modes(repo, &reapply_opts.cone_mode, &reapply_opts.sparse_index))
		திரும்பு 1;

	திரும்பு update_working_directory(repo, வெற்று);
}

நிலையான எழுத்து மாறிலி * மாறிலி builtin_sparse_checkout_clean_usage[] = {
	"git sparse-checkout clean [-n|--dry-run]",
	வெற்று
};

நிலையான முழு list_file_iterator(மாறிலி எழுத்து *path, மாறிலி வெறுமை *data)
{
	மாறிலி எழுத்து *செய்தி = data;

	printf(செய்தி, path);
	திரும்பு 0;
}

நிலையான வெறுமை list_every_file_in_dir(மாறிலி எழுத்து *செய்தி,
				   மாறிலி எழுத்து *directory)
{
	கட்டமைப்பு strbuf path = STRBUF_INIT;

	strbuf_addstr(&path, directory);
	for_each_file_in_dir(&path, list_file_iterator, செய்தி);
	strbuf_release(&path);
}

நிலையான மாறிலி எழுத்து *செய்தி_remove = N_("Removing %s\n");
நிலையான மாறிலி எழுத்து *செய்தி_would_remove = N_("Would remove %s\n");

நிலையான முழு sparse_checkout_clean(முழு argc, மாறிலி எழுத்து **argv,
				   மாறிலி எழுத்து *prefix,
				   கட்டமைப்பு repository *repo)
{
	கட்டமைப்பு strbuf full_path = STRBUF_INIT;
	மாறிலி எழுத்து *செய்தி = செய்தி_remove;
	size_t worktree_len;
	முழு force = 0, dry_run = 0, verbose = 0;
	முழு require_force = 1;
	கட்டமைப்பு repo_config_மதிப்புகள் *cfg = repo_config_மதிப்புகள்(the_repository);

	கட்டமைப்பு option builtin_sparse_checkout_clean_options[] = {
		OPT__DRY_RUN(&dry_run, N_("dry run")),
		OPT__FORCE(&force, N_("force"), PARSE_OPT_NOCOMPLETE),
		OPT__VERBOSE(&verbose, N_("report each affected file, not just directories")),
		OPT_END(),
	};

	setup_work_மரம்(the_repository);
	எனில் (!cfg->apply_sparse_checkout)
		die(_("must be in a sparse-checkout to clean directories"));
	எனில் (!cfg->core_sparse_checkout_cone)
		die(_("must be in a cone-mode sparse-checkout to clean directories"));

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_clean_options,
			     builtin_sparse_checkout_clean_usage, 0);

	repo_config_get_bool(repo, "clean.requireforce", &require_force);
	எனில் (require_force && !force && !dry_run)
		die(_("for safety, refusing to clean without one of --force or --dry-run"));

	எனில் (dry_run)
		செய்தி = செய்தி_would_remove;

	எனில் (repo_read_index(repo) < 0)
		die(_("failed to read index"));

	எனில் (convert_to_sparse(repo->index, SPARSE_INDEX_MEMORY_ONLY) ||
	    repo->index->sparse_index == INDEX_EXPANDED)
		die(_("failed to convert index to a sparse index; resolve merge conflicts and try again"));

	strbuf_addstr(&full_path, repo->worktree);
	strbuf_addch(&full_path, '/');
	worktree_len = full_path.len;

	க்கு (size_t i = 0; i < repo->index->cache_nr; i++) {
		கட்டமைப்பு cache_entry *ce = repo->index->cache[i];
		எனில் (!S_ISSPARSEDIR(ce->ce_mode))
			தொடர்;
		strbuf_setlen(&full_path, worktree_len);
		strbuf_add(&full_path, ce->name, ce->ce_namelen);

		எனில் (!is_directory(full_path.buf))
			தொடர்;

		எனில் (verbose)
			list_every_file_in_dir(செய்தி, ce->name);
		இல்லையெனில்
			printf(செய்தி, ce->name);

		எனில் (dry_run <= 0 &&
		    remove_dir_recursively(&full_path, 0))
			warning_errno(_("failed to remove '%s'"), ce->name);
	}

	strbuf_release(&full_path);
	திரும்பு 0;
}

நிலையான எழுத்து மாறிலி * மாறிலி builtin_sparse_checkout_disable_usage[] = {
	"git sparse-checkout disable",
	வெற்று
};

நிலையான முழு sparse_checkout_disable(முழு argc, மாறிலி எழுத்து **argv,
				   மாறிலி எழுத்து *prefix,
				   கட்டமைப்பு repository *repo)
{
	நிலையான கட்டமைப்பு option builtin_sparse_checkout_disable_options[] = {
		OPT_END(),
	};
	கட்டமைப்பு pattern_list pl;
	கட்டமைப்பு repo_config_மதிப்புகள் *cfg = repo_config_மதிப்புகள்(the_repository);

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

	setup_work_மரம்(the_repository);
	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_disable_options,
			     builtin_sparse_checkout_disable_usage, 0);

	/*
	 * Disable the advice message for expanding a sparse index, as we
	 * are expecting to do that when disabling sparse-checkout.
	 */
	give_advice_on_expansion = 0;
	repo_read_index(repo);

	memset(&pl, 0, அளவு(pl));
	hashmap_init(&pl.recursive_hashmap, pl_hashmap_cmp, வெற்று, 0);
	hashmap_init(&pl.parent_hashmap, pl_hashmap_cmp, வெற்று, 0);
	pl.use_cone_patterns = 0;
	cfg->apply_sparse_checkout = 1;

	add_pattern("/*", empty_base, 0, &pl, 0);

	prepare_repo_settings(the_repository);
	repo->settings.sparse_index = 0;

	எனில் (update_working_directory(repo, &pl))
		die(_("error while refreshing working directory"));

	clear_pattern_list(&pl);
	திரும்பு set_config(repo, MODE_NO_PATTERNS);
}

நிலையான எழுத்து மாறிலி * மாறிலி builtin_sparse_checkout_check_rules_usage[] = {
	N_("git sparse-checkout check-rules [-z] [--skip-checks]"
	   "[--[no-]cone] [--rules-file <file>]"),
	வெற்று
};

நிலையான கட்டமைப்பு sparse_checkout_check_rules_opts {
	முழு cone_mode;
	முழு null_termination;
	எழுத்து *rules_file;
} check_rules_opts;

நிலையான முழு check_rules(கட்டமைப்பு repository *repo,
		       கட்டமைப்பு pattern_list *pl,
		       முழு null_terminated)
{
	கட்டமைப்பு strbuf வரி = STRBUF_INIT;
	கட்டமைப்பு strbuf unquoted = STRBUF_INIT;
	எழுத்து *path;
	முழு வரி_terminator = null_terminated ? 0 : '\n';
	strbuf_getline_fn getline_fn = null_terminated ? strbuf_getline_nul
		: strbuf_getline;
	repo->index->sparse_checkout_patterns = pl;
	போது (!getline_fn(&வரி, stdin)) {
		path = வரி.buf;
		எனில் (!null_terminated && வரி.buf[0] == '"') {
			strbuf_reset(&unquoted);
			எனில் (unquote_c_style(&unquoted, வரி.buf, வெற்று))
				die(_("unable to unquote C-style string '%s'"),
					வரி.buf);

			path = unquoted.buf;
		}

		எனில் (path_in_sparse_checkout(path, repo->index))
			write_name_quoted(path, stdout, வரி_terminator);
	}
	strbuf_release(&வரி);
	strbuf_release(&unquoted);

	திரும்பு 0;
}

நிலையான முழு sparse_checkout_check_rules(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
				       கட்டமைப்பு repository *repo)
{
	நிலையான கட்டமைப்பு option builtin_sparse_checkout_check_rules_options[] = {
		OPT_BOOL('z', வெற்று, &check_rules_opts.null_termination,
			 N_("terminate input and output files by a NUL character")),
		OPT_BOOL(0, "cone", &check_rules_opts.cone_mode,
			 N_("when used with --rules-file interpret patterns as cone mode patterns")),
		OPT_FILENAME(0, "rules-file", &check_rules_opts.rules_file,
			 N_("use patterns in <file> instead of the current ones.")),
		OPT_END(),
	};

	FILE *fp;
	முழு ret;
	கட்டமைப்பு pattern_list pl = {0};
	கட்டமைப்பு repo_config_மதிப்புகள் *cfg = repo_config_மதிப்புகள்(the_repository);
	எழுத்து *sparse_filename;
	check_rules_opts.cone_mode = -1;

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_check_rules_options,
			     builtin_sparse_checkout_check_rules_usage, 0);

	எனில் (check_rules_opts.rules_file && check_rules_opts.cone_mode < 0)
		check_rules_opts.cone_mode = 1;

	update_cone_mode(&check_rules_opts.cone_mode);
	pl.use_cone_patterns = cfg->core_sparse_checkout_cone;
	எனில் (check_rules_opts.rules_file) {
		fp = xfopen(check_rules_opts.rules_file, "r");
		add_patterns_from_உள்ளீடு(&pl, argc, argv, fp);
		fclose(fp);
	} இல்லையெனில் {
		sparse_filename = get_sparse_checkout_filename();
		எனில் (add_patterns_from_file_to_list(sparse_filename, "", 0, &pl,
						   வெற்று, 0))
			die(_("unable to load existing sparse-checkout patterns"));
		விடுவி(sparse_filename);
	}

	ret = check_rules(repo, &pl, check_rules_opts.null_termination);
	clear_pattern_list(&pl);
	விடுவி(check_rules_opts.rules_file);
	திரும்பு ret;
}

முழு cmd_sparse_checkout(முழு argc,
			மாறிலி எழுத்து **argv,
			மாறிலி எழுத்து *prefix,
			கட்டமைப்பு repository *repo)
{
	parse_opt_subcommand_fn *fn = வெற்று;
	கட்டமைப்பு option builtin_sparse_checkout_options[] = {
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

	repo_config(the_repository, git_default_config, வெற்று);

	prepare_repo_settings(repo);
	repo->settings.command_requires_full_index = 0;

	திரும்பு fn(argc, argv, prefix, repo);
}
