#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "abspath.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "dir.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "object-file.h"
#അന്തര്ഭാവ "object-name.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "path.h"
#അന്തര്ഭാവ "pathspec.h"
#അന്തര്ഭാവ "strbuf.h"
#അന്തര്ഭാവ "string-list.h"
#അന്തര്ഭാവ "lockfile.h"
#അന്തര്ഭാവ "unpack-trees.h"
#അന്തര്ഭാവ "quote.h"
#അന്തര്ഭാവ "setup.h"
#അന്തര്ഭാവ "sparse-index.h"
#അന്തര്ഭാവ "worktree.h"

സ്ഥിതം സ്ഥിരം അക്ഷരം *empty_base = "";

സ്ഥിതം അക്ഷരം സ്ഥിരം * സ്ഥിരം builtin_sparse_checkout_usage[] = {
	N_("git sparse-checkout (init | list | set | add | reapply | disable | check-rules | clean) [<options>]"),
	ശൂന്യ
};

സ്ഥിതം ശൂന്യം write_patterns_to_file(FILE *fp, ഘടന pattern_list *pl)
{
	പൂർണ്ണസംഖ്യ i;

	വേണ്ടി (i = 0; i < pl->nr; i++) {
		ഘടന path_pattern *p = pl->patterns[i];

		എങ്കിൽ (p->flags & PATTERN_FLAG_NEGATIVE)
			fprintf(fp, "!");

		fprintf(fp, "%s", p->pattern);

		എങ്കിൽ (p->flags & PATTERN_FLAG_MUSTBEDIR)
			fprintf(fp, "/");

		fprintf(fp, "\n");
	}
}

സ്ഥിതം അക്ഷരം സ്ഥിരം * സ്ഥിരം builtin_sparse_checkout_list_usage[] = {
	"git sparse-checkout list",
	ശൂന്യ
};

സ്ഥിതം പൂർണ്ണസംഖ്യ sparse_checkout_list(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
				ഘടന repository *repo UNUSED)
{
	സ്ഥിതം ഘടന option builtin_sparse_checkout_list_options[] = {
		OPT_END(),
	};
	ഘടന pattern_list pl;
	അക്ഷരം *sparse_filename;
	പൂർണ്ണസംഖ്യ res;
	ഘടന repo_config_മൂല്യങ്ങൾ *cfg = repo_config_മൂല്യങ്ങൾ(the_repository);

	setup_work_വൃക്ഷം(the_repository);
	എങ്കിൽ (!cfg->apply_sparse_checkout)
		die(_("this worktree is not sparse"));

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_list_options,
			     builtin_sparse_checkout_list_usage, 0);

	memset(&pl, 0, വലുപ്പം(pl));

	pl.use_cone_patterns = cfg->core_sparse_checkout_cone;

	sparse_filename = get_sparse_checkout_filename();
	res = add_patterns_from_file_to_list(sparse_filename, "", 0, &pl, ശൂന്യ, 0);
	സ്വതന്ത്രം(sparse_filename);

	എങ്കിൽ (res < 0) {
		warning(_("this worktree is not sparse (sparse-checkout file may not exist)"));
		തിരികെ 0;
	}

	എങ്കിൽ (pl.use_cone_patterns) {
		പൂർണ്ണസംഖ്യ i;
		ഘടന pattern_entry *pe;
		ഘടന hashmap_iter iter;
		ഘടന string_list sl = STRING_LIST_INIT_DUP;

		hashmap_for_each_entry(&pl.recursive_hashmap, &iter, pe, ent) {
			/* pe->pattern starts with "/", skip it */
			string_list_append(&sl, pe->pattern + 1);
		}

		string_list_sort_u(&sl, 0);

		വേണ്ടി (i = 0; i < sl.nr; i++) {
			quote_c_style(sl.ഇനങ്ങൾ[i].string, ശൂന്യ, stdout, 0);
			printf("\n");
		}

		string_list_clear(&sl, 0);
	} അല്ലെങ്കിൽ {
		write_patterns_to_file(stdout, &pl);
	}

	clear_pattern_list(&pl);

	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം clean_tracked_sparse_directories(ഘടന repository *r)
{
	പൂർണ്ണസംഖ്യ i, was_full = 0;
	ഘടന strbuf path = STRBUF_INIT;
	size_t pathlen;
	ഘടന string_list_item *item;
	ഘടന string_list sparse_dirs = STRING_LIST_INIT_DUP;

	/*
	 * If we are not using cone mode patterns, then we cannot
	 * delete directories outside of the sparse cone.
	 */
	എങ്കിൽ (!r || !r->index || !r->worktree)
		തിരികെ;
	എങ്കിൽ (init_sparse_checkout_patterns(r->index) ||
	    !r->index->sparse_checkout_patterns->use_cone_patterns)
		തിരികെ;

	/*
	 * Use the sparse index as a data structure to assist finding
	 * directories that are safe to delete. This conversion to a
	 * sparse index will not delete directories that contain
	 * conflicted entries or submodules.
	 */
	എങ്കിൽ (r->index->sparse_index == INDEX_EXPANDED) {
		/*
		 * If something, such as a merge conflict or other concern,
		 * prevents us from converting to a sparse index, then do
		 * not try deleting files.
		 */
		എങ്കിൽ (convert_to_sparse(r->index, SPARSE_INDEX_MEMORY_ONLY))
			തിരികെ;
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
	വേണ്ടി (i = 0; i < r->index->cache_nr; i++) {
		ഘടന cache_entry *ce = r->index->cache[i];

		എങ്കിൽ (S_ISSPARSEDIR(ce->ce_mode) &&
		    repo_file_exists(r, ce->name))
			string_list_append(&sparse_dirs, ce->name);
	}

	for_each_string_list_item(item, &sparse_dirs) {
		ഘടന dir_struct dir = DIR_INIT;
		ഘടന pathspec p = { 0 };
		ഘടന strvec s = STRVEC_INIT;

		strbuf_setlen(&path, pathlen);
		strbuf_addstr(&path, item->string);

		dir.flags |= DIR_SHOW_IGNORED_TOO;

		setup_standard_excludes(&dir);
		strvec_തള്ളുക(&s, path.buf);

		parse_pathspec(&p, PATHSPEC_GLOB, 0, ശൂന്യ, s.v);
		fill_directory(&dir, r->index, &p);

		എങ്കിൽ (dir.nr) {
			warning(_("directory '%s' contains untracked files,"
				  " but is not in the sparse-checkout cone"),
				item->string);
		} അല്ലെങ്കിൽ എങ്കിൽ (remove_dir_recursively(&path, 0)) {
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

	എങ്കിൽ (was_full)
		ensure_full_index(r->index);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ update_working_directory(ഘടന repository *r,
				    ഘടന pattern_list *pl)
{
	എണ്ണൽ update_sparsity_result result;
	ഘടന unpack_trees_options o;
	ഘടന lock_file lock_file = LOCK_INIT;
	ഘടന pattern_list *old_pl;

	/* If no branch has been checked out, there are no updates to make. */
	എങ്കിൽ (is_index_unborn(r->index))
		തിരികെ UPDATE_SPARSITY_SUCCESS;

	old_pl = r->index->sparse_checkout_patterns;
	r->index->sparse_checkout_patterns = pl;

	memset(&o, 0, വലുപ്പം(o));
	o.verbose_update = isatty(2);
	o.update = 1;
	o.head_idx = -1;
	o.src_index = r->index;
	o.dst_index = r->index;
	o.skip_sparse_checkout = 0;

	setup_work_വൃക്ഷം(the_repository);

	repo_hold_locked_index(r, &lock_file, LOCK_DIE_ON_ERROR);

	setup_unpack_trees_porcelain(&o, "sparse-checkout");
	result = update_sparsity(&o, pl);
	clear_unpack_trees_porcelain(&o);

	എങ്കിൽ (result == UPDATE_SPARSITY_WARNINGS)
		/*
		 * We don't do any special handling of warnings from untracked
		 * files in the way or dirty entries that can't be removed.
		 */
		result = UPDATE_SPARSITY_SUCCESS;
	എങ്കിൽ (result == UPDATE_SPARSITY_SUCCESS)
		write_locked_index(r->index, &lock_file, COMMIT_LOCK);
	അല്ലെങ്കിൽ
		rollback_lock_file(&lock_file);

	clean_tracked_sparse_directories(r);

	എങ്കിൽ (r->index->sparse_checkout_patterns != pl) {
		clear_pattern_list(r->index->sparse_checkout_patterns);
		FREE_AND_NULL(r->index->sparse_checkout_patterns);
	}
	r->index->sparse_checkout_patterns = old_pl;

	തിരികെ result;
}

സ്ഥിതം അക്ഷരം *escaped_pattern(അക്ഷരം *pattern)
{
	അക്ഷരം *p = pattern;
	ഘടന strbuf final = STRBUF_INIT;

	ആകുമ്പോൾ (*p) {
		എങ്കിൽ (is_glob_special(*p))
			strbuf_addch(&final, '\\');

		strbuf_addch(&final, *p);
		p++;
	}

	തിരികെ strbuf_detach(&final, ശൂന്യ);
}

സ്ഥിതം ശൂന്യം write_cone_to_file(FILE *fp, ഘടന pattern_list *pl)
{
	പൂർണ്ണസംഖ്യ i;
	ഘടന pattern_entry *pe;
	ഘടന hashmap_iter iter;
	ഘടന string_list sl = STRING_LIST_INIT_DUP;
	ഘടന strbuf parent_pattern = STRBUF_INIT;

	hashmap_for_each_entry(&pl->parent_hashmap, &iter, pe, ent) {
		എങ്കിൽ (hashmap_get_entry(&pl->recursive_hashmap, pe, ent, ശൂന്യ))
			തുടരുക;

		എങ്കിൽ (!hashmap_contains_parent(&pl->recursive_hashmap,
					     pe->pattern,
					     &parent_pattern))
			string_list_append(&sl, pe->pattern);
	}

	string_list_sort_u(&sl, 0);

	fprintf(fp, "/*\n!/*/\n");

	വേണ്ടി (i = 0; i < sl.nr; i++) {
		അക്ഷരം *pattern = escaped_pattern(sl.ഇനങ്ങൾ[i].string);

		എങ്കിൽ (strlen(pattern))
			fprintf(fp, "%s/\n!%s/*/\n", pattern, pattern);
		സ്വതന്ത്രം(pattern);
	}

	string_list_clear(&sl, 0);

	hashmap_for_each_entry(&pl->recursive_hashmap, &iter, pe, ent) {
		എങ്കിൽ (!hashmap_contains_parent(&pl->recursive_hashmap,
					     pe->pattern,
					     &parent_pattern))
			string_list_append(&sl, pe->pattern);
	}

	strbuf_release(&parent_pattern);

	string_list_sort_u(&sl, 0);

	വേണ്ടി (i = 0; i < sl.nr; i++) {
		അക്ഷരം *pattern = escaped_pattern(sl.ഇനങ്ങൾ[i].string);
		fprintf(fp, "%s/\n", pattern);
		സ്വതന്ത്രം(pattern);
	}

	string_list_clear(&sl, 0);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ write_patterns_and_update(ഘടന repository *repo,
				     ഘടന pattern_list *pl)
{
	അക്ഷരം *sparse_filename;
	FILE *fp;
	ഘടന lock_file lk = LOCK_INIT;
	പൂർണ്ണസംഖ്യ result;
	ഘടന repo_config_മൂല്യങ്ങൾ *cfg = repo_config_മൂല്യങ്ങൾ(the_repository);

	sparse_filename = get_sparse_checkout_filename();

	എങ്കിൽ (safe_create_leading_directories(repo, sparse_filename))
		die(_("failed to create directory for sparse-checkout file"));

	repo_hold_lock_file_for_update(repo, &lk, sparse_filename,
				       LOCK_DIE_ON_ERROR);

	result = update_working_directory(repo, pl);
	എങ്കിൽ (result) {
		rollback_lock_file(&lk);
		update_working_directory(repo, ശൂന്യ);
		പോകുക out;
	}

	fp = fdopen_lock_file(&lk, "w");
	എങ്കിൽ (!fp)
		die_errno(_("unable to fdopen %s"), get_lock_file_path(&lk));

	എങ്കിൽ (cfg->core_sparse_checkout_cone)
		write_cone_to_file(fp, pl);
	അല്ലെങ്കിൽ
		write_patterns_to_file(fp, pl);

	എങ്കിൽ (commit_lock_file(&lk))
		die_errno(_("unable to write %s"), sparse_filename);

out:
	clear_pattern_list(pl);
	സ്വതന്ത്രം(sparse_filename);
	തിരികെ result;
}

എണ്ണൽ sparse_checkout_mode {
	MODE_NO_PATTERNS = 0,
	MODE_ALL_PATTERNS = 1,
	MODE_CONE_PATTERNS = 2,
};

സ്ഥിതം പൂർണ്ണസംഖ്യ set_config(ഘടന repository *repo,
		      എണ്ണൽ sparse_checkout_mode mode)
{
	/* Update to use worktree config, if not already. */
	എങ്കിൽ (init_worktree_config(repo)) {
		error(_("failed to initialize worktree config"));
		തിരികെ 1;
	}

	എങ്കിൽ (repo_config_set_worktree_gently(repo,
					    "core.sparseCheckout",
					    mode ? "true" : "false") ||
	    repo_config_set_worktree_gently(repo,
					    "core.sparseCheckoutCone",
					    mode == MODE_CONE_PATTERNS ?
						"true" : "false"))
		തിരികെ 1;

	എങ്കിൽ (mode == MODE_NO_PATTERNS)
		തിരികെ set_sparse_index_config(repo, 0);

	തിരികെ 0;
}

സ്ഥിതം എണ്ണൽ sparse_checkout_mode update_cone_mode(പൂർണ്ണസംഖ്യ *cone_mode) {
	ഘടന repo_config_മൂല്യങ്ങൾ *cfg = repo_config_മൂല്യങ്ങൾ(the_repository);

	/* If not specified, use previous definition of cone mode */
	എങ്കിൽ (*cone_mode == -1 && cfg->apply_sparse_checkout)
		*cone_mode = cfg->core_sparse_checkout_cone;

	/* Set cone/non-cone mode appropriately */
	cfg->apply_sparse_checkout = 1;
	എങ്കിൽ (*cone_mode == 1 || *cone_mode == -1) {
		cfg->core_sparse_checkout_cone = 1;
		തിരികെ MODE_CONE_PATTERNS;
	}
	cfg->core_sparse_checkout_cone = 0;
	തിരികെ MODE_ALL_PATTERNS;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ update_modes(ഘടന repository *repo, പൂർണ്ണസംഖ്യ *cone_mode, പൂർണ്ണസംഖ്യ *sparse_index)
{
	പൂർണ്ണസംഖ്യ mode, record_mode;
	ഘടന repo_config_മൂല്യങ്ങൾ *cfg = repo_config_മൂല്യങ്ങൾ(the_repository);

	/* Determine if we need to record the mode; ensure sparse checkout on */
	record_mode = (*cone_mode != -1) || !cfg->apply_sparse_checkout;

	mode = update_cone_mode(cone_mode);
	എങ്കിൽ (record_mode && set_config(repo, mode))
		തിരികെ 1;

	/* Set sparse-index/non-sparse-index mode if specified */
	എങ്കിൽ (*sparse_index >= 0) {
		എങ്കിൽ (set_sparse_index_config(repo, *sparse_index) < 0)
			die(_("failed to modify sparse-index config"));

		/* force an index rewrite */
		repo_read_index(repo);
		repo->index->updated_workdir = 1;

		എങ്കിൽ (!*sparse_index)
			ensure_full_index(repo->index);
	}

	തിരികെ 0;
}

സ്ഥിതം അക്ഷരം സ്ഥിരം * സ്ഥിരം builtin_sparse_checkout_init_usage[] = {
	"git sparse-checkout init [--cone] [--[no-]sparse-index]",
	ശൂന്യ
};

സ്ഥിതം ഘടന sparse_checkout_init_opts {
	പൂർണ്ണസംഖ്യ cone_mode;
	പൂർണ്ണസംഖ്യ sparse_index;
} init_opts;

സ്ഥിതം പൂർണ്ണസംഖ്യ sparse_checkout_init(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
				ഘടന repository *repo)
{
	ഘടന pattern_list pl;
	അക്ഷരം *sparse_filename;
	പൂർണ്ണസംഖ്യ res;
	ഘടന object_id oid;

	സ്ഥിതം ഘടന option builtin_sparse_checkout_init_options[] = {
		OPT_BOOL(0, "cone", &init_opts.cone_mode,
			 N_("initialize the sparse-checkout in cone mode")),
		OPT_BOOL(0, "sparse-index", &init_opts.sparse_index,
			 N_("toggle the use of a sparse index")),
		OPT_END(),
	};

	setup_work_വൃക്ഷം(the_repository);
	repo_read_index(repo);

	init_opts.cone_mode = -1;
	init_opts.sparse_index = -1;

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_init_options,
			     builtin_sparse_checkout_init_usage, 0);

	എങ്കിൽ (update_modes(repo, &init_opts.cone_mode, &init_opts.sparse_index))
		തിരികെ 1;

	memset(&pl, 0, വലുപ്പം(pl));

	sparse_filename = get_sparse_checkout_filename();
	res = add_patterns_from_file_to_list(sparse_filename, "", 0, &pl, ശൂന്യ, 0);

	/* If we already have a sparse-checkout file, use it. */
	എങ്കിൽ (res >= 0) {
		സ്വതന്ത്രം(sparse_filename);
		clear_pattern_list(&pl);
		തിരികെ update_working_directory(repo, ശൂന്യ);
	}

	എങ്കിൽ (repo_get_oid(repo, "HEAD", &oid)) {
		FILE *fp;

		/* assume we are in a fresh repo, but update the sparse-checkout file */
		എങ്കിൽ (safe_create_leading_directories(repo, sparse_filename))
			die(_("unable to create leading directories of %s"),
			    sparse_filename);
		fp = xfopen(sparse_filename, "w");
		എങ്കിൽ (!fp)
			die(_("failed to open '%s'"), sparse_filename);

		സ്വതന്ത്രം(sparse_filename);
		fprintf(fp, "/*\n!/*/\n");
		fclose(fp);
		തിരികെ 0;
	}

	സ്വതന്ത്രം(sparse_filename);

	add_pattern("/*", empty_base, 0, &pl, 0);
	add_pattern("!/*/", empty_base, 0, &pl, 0);
	pl.use_cone_patterns = init_opts.cone_mode;

	തിരികെ write_patterns_and_update(repo, &pl);
}

സ്ഥിതം ശൂന്യം തിരുകുക_recursive_pattern(ഘടന pattern_list *pl, ഘടന strbuf *path)
{
	ഘടന pattern_entry *e = xmalloc(വലുപ്പം(*e));
	e->patternlen = path->len;
	e->pattern = strbuf_detach(path, ശൂന്യ);
	hashmap_entry_init(&e->ent, fspathhash(e->pattern));

	hashmap_add(&pl->recursive_hashmap, &e->ent);

	ആകുമ്പോൾ (e->patternlen) {
		അക്ഷരം *slash = strrchr(e->pattern, '/');
		അക്ഷരം *oldpattern = e->pattern;
		size_t newlen;
		ഘടന pattern_entry *dup;

		എങ്കിൽ (!slash || slash == e->pattern)
			നിർത്തുക;

		newlen = slash - e->pattern;
		e = xmalloc(വലുപ്പം(ഘടന pattern_entry));
		e->patternlen = newlen;
		e->pattern = xstrndup(oldpattern, newlen);
		hashmap_entry_init(&e->ent, fspathhash(e->pattern));

		dup = hashmap_get_entry(&pl->parent_hashmap, e, ent, ശൂന്യ);
		എങ്കിൽ (!dup) {
			hashmap_add(&pl->parent_hashmap, &e->ent);
		} അല്ലെങ്കിൽ {
			സ്വതന്ത്രം(e->pattern);
			സ്വതന്ത്രം(e);
			e = dup;
		}
	}
}

സ്ഥിതം ശൂന്യം strbuf_to_cone_pattern(ഘടന strbuf *വരി, ഘടന pattern_list *pl)
{
	strbuf_trim(വരി);

	strbuf_trim_trailing_dir_sep(വരി);

	എങ്കിൽ (strbuf_normalize_path(വരി))
		die(_("could not normalize path %s"), വരി->buf);

	എങ്കിൽ (!വരി->len)
		തിരികെ;

	എങ്കിൽ (വരി->buf[0] != '/')
		strbuf_insertstr(വരി, 0, "/");

	തിരുകുക_recursive_pattern(pl, വരി);
}

സ്ഥിതം ശൂന്യം add_patterns_from_നിവേശം(ഘടന pattern_list *pl,
				    പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv,
				    FILE *file)
{
	പൂർണ്ണസംഖ്യ i;
	ഘടന repo_config_മൂല്യങ്ങൾ *cfg = repo_config_മൂല്യങ്ങൾ(the_repository);

	എങ്കിൽ (cfg->core_sparse_checkout_cone) {
		ഘടന strbuf വരി = STRBUF_INIT;

		hashmap_init(&pl->recursive_hashmap, pl_hashmap_cmp, ശൂന്യ, 0);
		hashmap_init(&pl->parent_hashmap, pl_hashmap_cmp, ശൂന്യ, 0);
		pl->use_cone_patterns = 1;

		എങ്കിൽ (file) {
			ഘടന strbuf unquoted = STRBUF_INIT;
			ആകുമ്പോൾ (!strbuf_getline(&വരി, file)) {
				എങ്കിൽ (വരി.buf[0] == '"') {
					strbuf_reset(&unquoted);
					എങ്കിൽ (unquote_c_style(&unquoted, വരി.buf, ശൂന്യ))
						die(_("unable to unquote C-style string '%s'"),
						വരി.buf);

					strbuf_swap(&unquoted, &വരി);
				}

				strbuf_to_cone_pattern(&വരി, pl);
			}

			strbuf_release(&unquoted);
		} അല്ലെങ്കിൽ {
			വേണ്ടി (i = 0; i < argc; i++) {
				strbuf_setlen(&വരി, 0);
				strbuf_addstr(&വരി, argv[i]);
				strbuf_to_cone_pattern(&വരി, pl);
			}
		}
		strbuf_release(&വരി);
	} അല്ലെങ്കിൽ {
		എങ്കിൽ (file) {
			ഘടന strbuf വരി = STRBUF_INIT;

			ആകുമ്പോൾ (!strbuf_getline(&വരി, file))
				add_pattern(വരി.buf, empty_base, 0, pl, 0);

			strbuf_release(&വരി);
		} അല്ലെങ്കിൽ {
			വേണ്ടി (i = 0; i < argc; i++)
				add_pattern(argv[i], empty_base, 0, pl, 0);
		}
	}
}

എണ്ണൽ modify_type {
	REPLACE,
	ADD,
};

സ്ഥിതം ശൂന്യം add_patterns_cone_mode(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv,
				   ഘടന pattern_list *pl,
				   പൂർണ്ണസംഖ്യ use_stdin)
{
	ഘടന strbuf buffer = STRBUF_INIT;
	ഘടന pattern_entry *pe;
	ഘടന hashmap_iter iter;
	ഘടന pattern_list existing;
	ഘടന repo_config_മൂല്യങ്ങൾ *cfg = repo_config_മൂല്യങ്ങൾ(the_repository);
	അക്ഷരം *sparse_filename = get_sparse_checkout_filename();

	add_patterns_from_നിവേശം(pl, argc, argv,
				use_stdin ? stdin : ശൂന്യ);

	memset(&existing, 0, വലുപ്പം(existing));
	existing.use_cone_patterns = cfg->core_sparse_checkout_cone;

	എങ്കിൽ (add_patterns_from_file_to_list(sparse_filename, "", 0,
					   &existing, ശൂന്യ, 0))
		die(_("unable to load existing sparse-checkout patterns"));
	സ്വതന്ത്രം(sparse_filename);

	എങ്കിൽ (!existing.use_cone_patterns)
		die(_("existing sparse-checkout patterns do not use cone mode"));

	hashmap_for_each_entry(&existing.recursive_hashmap, &iter, pe, ent) {
		എങ്കിൽ (!hashmap_contains_parent(&pl->recursive_hashmap,
					pe->pattern, &buffer) ||
		    !hashmap_contains_parent(&pl->parent_hashmap,
					pe->pattern, &buffer)) {
			strbuf_reset(&buffer);
			strbuf_addstr(&buffer, pe->pattern);
			തിരുകുക_recursive_pattern(pl, &buffer);
		}
	}

	clear_pattern_list(&existing);
	strbuf_release(&buffer);
}

സ്ഥിതം ശൂന്യം add_patterns_literal(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv,
				 ഘടന pattern_list *pl,
				 പൂർണ്ണസംഖ്യ use_stdin)
{
	അക്ഷരം *sparse_filename = get_sparse_checkout_filename();
	എങ്കിൽ (add_patterns_from_file_to_list(sparse_filename, "", 0,
					   pl, ശൂന്യ, 0))
		die(_("unable to load existing sparse-checkout patterns"));
	സ്വതന്ത്രം(sparse_filename);
	add_patterns_from_നിവേശം(pl, argc, argv, use_stdin ? stdin : ശൂന്യ);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ modify_pattern_list(ഘടന repository *repo,
			       ഘടന strvec *args, പൂർണ്ണസംഖ്യ use_stdin,
			       എണ്ണൽ modify_type m)
{
	പൂർണ്ണസംഖ്യ result;
	പൂർണ്ണസംഖ്യ changed_config = 0;
	ഘടന pattern_list *pl = xcalloc(1, വലുപ്പം(*pl));
	ഘടന repo_config_മൂല്യങ്ങൾ *cfg = repo_config_മൂല്യങ്ങൾ(the_repository);

	തിരഞ്ഞെടുപ്പ് (m) {
	അവസ്ഥ ADD:
		എങ്കിൽ (cfg->core_sparse_checkout_cone)
			add_patterns_cone_mode(args->nr, args->v, pl, use_stdin);
		അല്ലെങ്കിൽ
			add_patterns_literal(args->nr, args->v, pl, use_stdin);
		നിർത്തുക;

	അവസ്ഥ REPLACE:
		add_patterns_from_നിവേശം(pl, args->nr, args->v,
					use_stdin ? stdin : ശൂന്യ);
		നിർത്തുക;
	}

	എങ്കിൽ (!cfg->apply_sparse_checkout) {
		set_config(repo, MODE_ALL_PATTERNS);
		cfg->apply_sparse_checkout = 1;
		changed_config = 1;
	}

	result = write_patterns_and_update(repo, pl);

	എങ്കിൽ (result && changed_config)
		set_config(repo, MODE_NO_PATTERNS);

	clear_pattern_list(pl);
	സ്വതന്ത്രം(pl);
	തിരികെ result;
}

സ്ഥിതം ശൂന്യം sanitize_paths(ഘടന repository *repo,
			   ഘടന strvec *args,
			   സ്ഥിരം അക്ഷരം *prefix, പൂർണ്ണസംഖ്യ skip_checks)
{
	പൂർണ്ണസംഖ്യ i;
	ഘടന repo_config_മൂല്യങ്ങൾ *cfg = repo_config_മൂല്യങ്ങൾ(the_repository);

	എങ്കിൽ (!args->nr)
		തിരികെ;

	എങ്കിൽ (prefix && *prefix && cfg->core_sparse_checkout_cone) {
		/*
		 * The args are not pathspecs, so unfortunately we
		 * cannot imitate how cmd_add() uses parse_pathspec().
		 */
		പൂർണ്ണസംഖ്യ prefix_len = strlen(prefix);

		വേണ്ടി (i = 0; i < args->nr; i++) {
			അക്ഷരം *prefixed_path = prefix_path(the_repository, prefix,
							  prefix_len, args->v[i]);
			strvec_replace(args, i, prefixed_path);
			സ്വതന്ത്രം(prefixed_path);
		}
	}

	എങ്കിൽ (skip_checks)
		തിരികെ;

	എങ്കിൽ (prefix && *prefix && !cfg->core_sparse_checkout_cone)
		die(_("please run from the toplevel directory in non-cone mode"));

	എങ്കിൽ (cfg->core_sparse_checkout_cone) {
		വേണ്ടി (i = 0; i < args->nr; i++) {
			എങ്കിൽ (args->v[i][0] == '/')
				die(_("specify directories rather than patterns (no leading slash)"));
			എങ്കിൽ (args->v[i][0] == '!')
				die(_("specify directories rather than patterns.  If your directory starts with a '!', pass --skip-checks"));
			എങ്കിൽ (strpbrk(args->v[i], "*?[]"))
				die(_("specify directories rather than patterns.  If your directory really has any of '*?[]\\' in it, pass --skip-checks"));
		}
	}

	വേണ്ടി (i = 0; i < args->nr; i++) {
		ഘടന cache_entry *ce;
		ഘടന index_state *index = repo->index;
		പൂർണ്ണസംഖ്യ pos = index_name_pos(index, args->v[i], strlen(args->v[i]));

		എങ്കിൽ (pos < 0)
			തുടരുക;
		ce = index->cache[pos];
		എങ്കിൽ (S_ISSPARSEDIR(ce->ce_mode))
			തുടരുക;

		എങ്കിൽ (cfg->core_sparse_checkout_cone)
			die(_("'%s' is not a directory; to treat it as a directory anyway, rerun with --skip-checks"), args->v[i]);
		അല്ലെങ്കിൽ
			warning(_("pass a leading slash before paths such as '%s' if you want a single file (see NON-CONE PROBLEMS in the git-sparse-checkout manual)."), args->v[i]);
	}
}

സ്ഥിതം അക്ഷരം സ്ഥിരം * സ്ഥിരം builtin_sparse_checkout_add_usage[] = {
	N_("git sparse-checkout add [--skip-checks] (--stdin | <patterns>)"),
	ശൂന്യ
};

സ്ഥിതം ഘടന sparse_checkout_add_opts {
	പൂർണ്ണസംഖ്യ skip_checks;
	പൂർണ്ണസംഖ്യ use_stdin;
} add_opts;

സ്ഥിതം പൂർണ്ണസംഖ്യ sparse_checkout_add(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
			       ഘടന repository *repo)
{
	സ്ഥിതം ഘടന option builtin_sparse_checkout_add_options[] = {
		OPT_BOOL_F(0, "skip-checks", &add_opts.skip_checks,
			   N_("skip some sanity checks on the given paths that might give false positives"),
			   PARSE_OPT_NONEG),
		OPT_BOOL(0, "stdin", &add_opts.use_stdin,
			 N_("read patterns from standard in")),
		OPT_END(),
	};
	ഘടന strvec patterns = STRVEC_INIT;
	പൂർണ്ണസംഖ്യ ret;
	ഘടന repo_config_മൂല്യങ്ങൾ *cfg = repo_config_മൂല്യങ്ങൾ(the_repository);

	setup_work_വൃക്ഷം(the_repository);
	എങ്കിൽ (!cfg->apply_sparse_checkout)
		die(_("no sparse-checkout to add to"));

	repo_read_index(repo);

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_add_options,
			     builtin_sparse_checkout_add_usage, 0);

	വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < argc; i++)
		strvec_തള്ളുക(&patterns, argv[i]);
	sanitize_paths(repo, &patterns, prefix, add_opts.skip_checks);

	ret = modify_pattern_list(repo, &patterns, add_opts.use_stdin, ADD);

	strvec_clear(&patterns);
	തിരികെ ret;
}

സ്ഥിതം അക്ഷരം സ്ഥിരം * സ്ഥിരം builtin_sparse_checkout_set_usage[] = {
	N_("git sparse-checkout set [--[no-]cone] [--[no-]sparse-index] [--skip-checks] (--stdin | <patterns>)"),
	ശൂന്യ
};

സ്ഥിതം ഘടന sparse_checkout_set_opts {
	പൂർണ്ണസംഖ്യ cone_mode;
	പൂർണ്ണസംഖ്യ sparse_index;
	പൂർണ്ണസംഖ്യ skip_checks;
	പൂർണ്ണസംഖ്യ use_stdin;
} set_opts;

സ്ഥിതം പൂർണ്ണസംഖ്യ sparse_checkout_set(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
			       ഘടന repository *repo)
{
	ഘടന repo_config_മൂല്യങ്ങൾ *cfg = repo_config_മൂല്യങ്ങൾ(the_repository);
	പൂർണ്ണസംഖ്യ default_patterns_nr = 2;
	സ്ഥിരം അക്ഷരം *default_patterns[] = {"/*", "!/*/", ശൂന്യ};

	സ്ഥിതം ഘടന option builtin_sparse_checkout_set_options[] = {
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
	ഘടന strvec patterns = STRVEC_INIT;
	പൂർണ്ണസംഖ്യ ret;

	setup_work_വൃക്ഷം(the_repository);
	repo_read_index(repo);

	set_opts.cone_mode = -1;
	set_opts.sparse_index = -1;

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_set_options,
			     builtin_sparse_checkout_set_usage, 0);

	എങ്കിൽ (update_modes(repo, &set_opts.cone_mode, &set_opts.sparse_index))
		തിരികെ 1;

	/*
	 * Cone mode automatically specifies the toplevel directory.  For
	 * non-cone mode, if nothing is specified, manually select just the
	 * top-level directory (much as 'init' would do).
	 */
	എങ്കിൽ (!cfg->core_sparse_checkout_cone && !set_opts.use_stdin && argc == 0) {
		വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < default_patterns_nr; i++)
			strvec_തള്ളുക(&patterns, default_patterns[i]);
	} അല്ലെങ്കിൽ {
		വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < argc; i++)
			strvec_തള്ളുക(&patterns, argv[i]);
		sanitize_paths(repo, &patterns, prefix, set_opts.skip_checks);
	}

	ret = modify_pattern_list(repo, &patterns, set_opts.use_stdin, REPLACE);

	strvec_clear(&patterns);
	തിരികെ ret;
}

സ്ഥിതം അക്ഷരം സ്ഥിരം * സ്ഥിരം builtin_sparse_checkout_reapply_usage[] = {
	"git sparse-checkout reapply [--[no-]cone] [--[no-]sparse-index]",
	ശൂന്യ
};

സ്ഥിതം ഘടന sparse_checkout_reapply_opts {
	പൂർണ്ണസംഖ്യ cone_mode;
	പൂർണ്ണസംഖ്യ sparse_index;
} reapply_opts;

സ്ഥിതം പൂർണ്ണസംഖ്യ sparse_checkout_reapply(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv,
				   സ്ഥിരം അക്ഷരം *prefix,
				   ഘടന repository *repo)
{
	സ്ഥിതം ഘടന option builtin_sparse_checkout_reapply_options[] = {
		OPT_BOOL(0, "cone", &reapply_opts.cone_mode,
			 N_("initialize the sparse-checkout in cone mode")),
		OPT_BOOL(0, "sparse-index", &reapply_opts.sparse_index,
			 N_("toggle the use of a sparse index")),
		OPT_END(),
	};
	ഘടന repo_config_മൂല്യങ്ങൾ *cfg = repo_config_മൂല്യങ്ങൾ(the_repository);

	setup_work_വൃക്ഷം(the_repository);
	എങ്കിൽ (!cfg->apply_sparse_checkout)
		die(_("must be in a sparse-checkout to reapply sparsity patterns"));

	reapply_opts.cone_mode = -1;
	reapply_opts.sparse_index = -1;

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_reapply_options,
			     builtin_sparse_checkout_reapply_usage, 0);

	repo_read_index(repo);

	എങ്കിൽ (update_modes(repo, &reapply_opts.cone_mode, &reapply_opts.sparse_index))
		തിരികെ 1;

	തിരികെ update_working_directory(repo, ശൂന്യ);
}

സ്ഥിതം അക്ഷരം സ്ഥിരം * സ്ഥിരം builtin_sparse_checkout_clean_usage[] = {
	"git sparse-checkout clean [-n|--dry-run]",
	ശൂന്യ
};

സ്ഥിതം പൂർണ്ണസംഖ്യ list_file_iterator(സ്ഥിരം അക്ഷരം *path, സ്ഥിരം ശൂന്യം *data)
{
	സ്ഥിരം അക്ഷരം *സന്ദേശം = data;

	printf(സന്ദേശം, path);
	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം list_every_file_in_dir(സ്ഥിരം അക്ഷരം *സന്ദേശം,
				   സ്ഥിരം അക്ഷരം *directory)
{
	ഘടന strbuf path = STRBUF_INIT;

	strbuf_addstr(&path, directory);
	for_each_file_in_dir(&path, list_file_iterator, സന്ദേശം);
	strbuf_release(&path);
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *സന്ദേശം_remove = N_("Removing %s\n");
സ്ഥിതം സ്ഥിരം അക്ഷരം *സന്ദേശം_would_remove = N_("Would remove %s\n");

സ്ഥിതം പൂർണ്ണസംഖ്യ sparse_checkout_clean(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv,
				   സ്ഥിരം അക്ഷരം *prefix,
				   ഘടന repository *repo)
{
	ഘടന strbuf full_path = STRBUF_INIT;
	സ്ഥിരം അക്ഷരം *സന്ദേശം = സന്ദേശം_remove;
	size_t worktree_len;
	പൂർണ്ണസംഖ്യ force = 0, dry_run = 0, verbose = 0;
	പൂർണ്ണസംഖ്യ require_force = 1;
	ഘടന repo_config_മൂല്യങ്ങൾ *cfg = repo_config_മൂല്യങ്ങൾ(the_repository);

	ഘടന option builtin_sparse_checkout_clean_options[] = {
		OPT__DRY_RUN(&dry_run, N_("dry run")),
		OPT__FORCE(&force, N_("force"), PARSE_OPT_NOCOMPLETE),
		OPT__VERBOSE(&verbose, N_("report each affected file, not just directories")),
		OPT_END(),
	};

	setup_work_വൃക്ഷം(the_repository);
	എങ്കിൽ (!cfg->apply_sparse_checkout)
		die(_("must be in a sparse-checkout to clean directories"));
	എങ്കിൽ (!cfg->core_sparse_checkout_cone)
		die(_("must be in a cone-mode sparse-checkout to clean directories"));

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_clean_options,
			     builtin_sparse_checkout_clean_usage, 0);

	repo_config_get_bool(repo, "clean.requireforce", &require_force);
	എങ്കിൽ (require_force && !force && !dry_run)
		die(_("for safety, refusing to clean without one of --force or --dry-run"));

	എങ്കിൽ (dry_run)
		സന്ദേശം = സന്ദേശം_would_remove;

	എങ്കിൽ (repo_read_index(repo) < 0)
		die(_("failed to read index"));

	എങ്കിൽ (convert_to_sparse(repo->index, SPARSE_INDEX_MEMORY_ONLY) ||
	    repo->index->sparse_index == INDEX_EXPANDED)
		die(_("failed to convert index to a sparse index; resolve merge conflicts and try again"));

	strbuf_addstr(&full_path, repo->worktree);
	strbuf_addch(&full_path, '/');
	worktree_len = full_path.len;

	വേണ്ടി (size_t i = 0; i < repo->index->cache_nr; i++) {
		ഘടന cache_entry *ce = repo->index->cache[i];
		എങ്കിൽ (!S_ISSPARSEDIR(ce->ce_mode))
			തുടരുക;
		strbuf_setlen(&full_path, worktree_len);
		strbuf_add(&full_path, ce->name, ce->ce_namelen);

		എങ്കിൽ (!is_directory(full_path.buf))
			തുടരുക;

		എങ്കിൽ (verbose)
			list_every_file_in_dir(സന്ദേശം, ce->name);
		അല്ലെങ്കിൽ
			printf(സന്ദേശം, ce->name);

		എങ്കിൽ (dry_run <= 0 &&
		    remove_dir_recursively(&full_path, 0))
			warning_errno(_("failed to remove '%s'"), ce->name);
	}

	strbuf_release(&full_path);
	തിരികെ 0;
}

സ്ഥിതം അക്ഷരം സ്ഥിരം * സ്ഥിരം builtin_sparse_checkout_disable_usage[] = {
	"git sparse-checkout disable",
	ശൂന്യ
};

സ്ഥിതം പൂർണ്ണസംഖ്യ sparse_checkout_disable(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv,
				   സ്ഥിരം അക്ഷരം *prefix,
				   ഘടന repository *repo)
{
	സ്ഥിതം ഘടന option builtin_sparse_checkout_disable_options[] = {
		OPT_END(),
	};
	ഘടന pattern_list pl;
	ഘടന repo_config_മൂല്യങ്ങൾ *cfg = repo_config_മൂല്യങ്ങൾ(the_repository);

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

	setup_work_വൃക്ഷം(the_repository);
	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_disable_options,
			     builtin_sparse_checkout_disable_usage, 0);

	/*
	 * Disable the advice message for expanding a sparse index, as we
	 * are expecting to do that when disabling sparse-checkout.
	 */
	give_advice_on_expansion = 0;
	repo_read_index(repo);

	memset(&pl, 0, വലുപ്പം(pl));
	hashmap_init(&pl.recursive_hashmap, pl_hashmap_cmp, ശൂന്യ, 0);
	hashmap_init(&pl.parent_hashmap, pl_hashmap_cmp, ശൂന്യ, 0);
	pl.use_cone_patterns = 0;
	cfg->apply_sparse_checkout = 1;

	add_pattern("/*", empty_base, 0, &pl, 0);

	prepare_repo_settings(the_repository);
	repo->settings.sparse_index = 0;

	എങ്കിൽ (update_working_directory(repo, &pl))
		die(_("error while refreshing working directory"));

	clear_pattern_list(&pl);
	തിരികെ set_config(repo, MODE_NO_PATTERNS);
}

സ്ഥിതം അക്ഷരം സ്ഥിരം * സ്ഥിരം builtin_sparse_checkout_check_rules_usage[] = {
	N_("git sparse-checkout check-rules [-z] [--skip-checks]"
	   "[--[no-]cone] [--rules-file <file>]"),
	ശൂന്യ
};

സ്ഥിതം ഘടന sparse_checkout_check_rules_opts {
	പൂർണ്ണസംഖ്യ cone_mode;
	പൂർണ്ണസംഖ്യ null_termination;
	അക്ഷരം *rules_file;
} check_rules_opts;

സ്ഥിതം പൂർണ്ണസംഖ്യ check_rules(ഘടന repository *repo,
		       ഘടന pattern_list *pl,
		       പൂർണ്ണസംഖ്യ null_terminated)
{
	ഘടന strbuf വരി = STRBUF_INIT;
	ഘടന strbuf unquoted = STRBUF_INIT;
	അക്ഷരം *path;
	പൂർണ്ണസംഖ്യ വരി_terminator = null_terminated ? 0 : '\n';
	strbuf_getline_fn getline_fn = null_terminated ? strbuf_getline_nul
		: strbuf_getline;
	repo->index->sparse_checkout_patterns = pl;
	ആകുമ്പോൾ (!getline_fn(&വരി, stdin)) {
		path = വരി.buf;
		എങ്കിൽ (!null_terminated && വരി.buf[0] == '"') {
			strbuf_reset(&unquoted);
			എങ്കിൽ (unquote_c_style(&unquoted, വരി.buf, ശൂന്യ))
				die(_("unable to unquote C-style string '%s'"),
					വരി.buf);

			path = unquoted.buf;
		}

		എങ്കിൽ (path_in_sparse_checkout(path, repo->index))
			write_name_quoted(path, stdout, വരി_terminator);
	}
	strbuf_release(&വരി);
	strbuf_release(&unquoted);

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ sparse_checkout_check_rules(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
				       ഘടന repository *repo)
{
	സ്ഥിതം ഘടന option builtin_sparse_checkout_check_rules_options[] = {
		OPT_BOOL('z', ശൂന്യ, &check_rules_opts.null_termination,
			 N_("terminate input and output files by a NUL character")),
		OPT_BOOL(0, "cone", &check_rules_opts.cone_mode,
			 N_("when used with --rules-file interpret patterns as cone mode patterns")),
		OPT_FILENAME(0, "rules-file", &check_rules_opts.rules_file,
			 N_("use patterns in <file> instead of the current ones.")),
		OPT_END(),
	};

	FILE *fp;
	പൂർണ്ണസംഖ്യ ret;
	ഘടന pattern_list pl = {0};
	ഘടന repo_config_മൂല്യങ്ങൾ *cfg = repo_config_മൂല്യങ്ങൾ(the_repository);
	അക്ഷരം *sparse_filename;
	check_rules_opts.cone_mode = -1;

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_check_rules_options,
			     builtin_sparse_checkout_check_rules_usage, 0);

	എങ്കിൽ (check_rules_opts.rules_file && check_rules_opts.cone_mode < 0)
		check_rules_opts.cone_mode = 1;

	update_cone_mode(&check_rules_opts.cone_mode);
	pl.use_cone_patterns = cfg->core_sparse_checkout_cone;
	എങ്കിൽ (check_rules_opts.rules_file) {
		fp = xfopen(check_rules_opts.rules_file, "r");
		add_patterns_from_നിവേശം(&pl, argc, argv, fp);
		fclose(fp);
	} അല്ലെങ്കിൽ {
		sparse_filename = get_sparse_checkout_filename();
		എങ്കിൽ (add_patterns_from_file_to_list(sparse_filename, "", 0, &pl,
						   ശൂന്യ, 0))
			die(_("unable to load existing sparse-checkout patterns"));
		സ്വതന്ത്രം(sparse_filename);
	}

	ret = check_rules(repo, &pl, check_rules_opts.null_termination);
	clear_pattern_list(&pl);
	സ്വതന്ത്രം(check_rules_opts.rules_file);
	തിരികെ ret;
}

പൂർണ്ണസംഖ്യ cmd_sparse_checkout(പൂർണ്ണസംഖ്യ argc,
			സ്ഥിരം അക്ഷരം **argv,
			സ്ഥിരം അക്ഷരം *prefix,
			ഘടന repository *repo)
{
	parse_opt_subcommand_fn *fn = ശൂന്യ;
	ഘടന option builtin_sparse_checkout_options[] = {
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

	repo_config(the_repository, git_default_config, ശൂന്യ);

	prepare_repo_settings(repo);
	repo->settings.command_requires_full_index = 0;

	തിരികെ fn(argc, argv, prefix, repo);
}
