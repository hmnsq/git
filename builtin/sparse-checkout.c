#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "abspath.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "dir.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "object-file.h"
#અન્તર્ભાવ "object-name.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "path.h"
#અન્તર્ભાવ "pathspec.h"
#અન્તર્ભાવ "strbuf.h"
#અન્તર્ભાવ "string-list.h"
#અન્તર્ભાવ "lockfile.h"
#અન્તર્ભાવ "unpack-trees.h"
#અન્તર્ભાવ "quote.h"
#અન્તર્ભાવ "setup.h"
#અન્તર્ભાવ "sparse-index.h"
#અન્તર્ભાવ "worktree.h"

સ્થિર અચલ અક્ષર *empty_base = "";

સ્થિર અક્ષર અચલ * અચલ builtin_sparse_checkout_usage[] = {
	N_("git sparse-checkout (init | list | set | add | reapply | disable | check-rules | clean) [<options>]"),
	ખાલી
};

સ્થિર શૂન્ય write_patterns_to_file(FILE *fp, સંરચના pattern_list *pl)
{
	પૂર્ણાંક i;

	માટે (i = 0; i < pl->nr; i++) {
		સંરચના path_pattern *p = pl->patterns[i];

		જો (p->flags & PATTERN_FLAG_NEGATIVE)
			fprintf(fp, "!");

		fprintf(fp, "%s", p->pattern);

		જો (p->flags & PATTERN_FLAG_MUSTBEDIR)
			fprintf(fp, "/");

		fprintf(fp, "\n");
	}
}

સ્થિર અક્ષર અચલ * અચલ builtin_sparse_checkout_list_usage[] = {
	"git sparse-checkout list",
	ખાલી
};

સ્થિર પૂર્ણાંક sparse_checkout_list(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
				સંરચના repository *repo UNUSED)
{
	સ્થિર સંરચના option builtin_sparse_checkout_list_options[] = {
		OPT_END(),
	};
	સંરચના pattern_list pl;
	અક્ષર *sparse_filename;
	પૂર્ણાંક res;
	સંરચના repo_config_કિંમતો *cfg = repo_config_કિંમતો(the_repository);

	setup_work_વૃક્ષ(the_repository);
	જો (!cfg->apply_sparse_checkout)
		die(_("this worktree is not sparse"));

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_list_options,
			     builtin_sparse_checkout_list_usage, 0);

	memset(&pl, 0, કદ(pl));

	pl.use_cone_patterns = cfg->core_sparse_checkout_cone;

	sparse_filename = get_sparse_checkout_filename();
	res = add_patterns_from_file_to_list(sparse_filename, "", 0, &pl, ખાલી, 0);
	મુક્ત(sparse_filename);

	જો (res < 0) {
		warning(_("this worktree is not sparse (sparse-checkout file may not exist)"));
		પાછા 0;
	}

	જો (pl.use_cone_patterns) {
		પૂર્ણાંક i;
		સંરચના pattern_entry *pe;
		સંરચના hashmap_iter iter;
		સંરચના string_list sl = STRING_LIST_INIT_DUP;

		hashmap_for_each_entry(&pl.recursive_hashmap, &iter, pe, ent) {
			/* pe->pattern starts with "/", skip it */
			string_list_append(&sl, pe->pattern + 1);
		}

		string_list_sort_u(&sl, 0);

		માટે (i = 0; i < sl.nr; i++) {
			quote_c_style(sl.વસ્તુઓ[i].string, ખાલી, stdout, 0);
			printf("\n");
		}

		string_list_clear(&sl, 0);
	} નહીંતર {
		write_patterns_to_file(stdout, &pl);
	}

	clear_pattern_list(&pl);

	પાછા 0;
}

સ્થિર શૂન્ય clean_tracked_sparse_directories(સંરચના repository *r)
{
	પૂર્ણાંક i, was_full = 0;
	સંરચના strbuf path = STRBUF_INIT;
	size_t pathlen;
	સંરચના string_list_item *item;
	સંરચના string_list sparse_dirs = STRING_LIST_INIT_DUP;

	/*
	 * If we are not using cone mode patterns, then we cannot
	 * delete directories outside of the sparse cone.
	 */
	જો (!r || !r->index || !r->worktree)
		પાછા;
	જો (init_sparse_checkout_patterns(r->index) ||
	    !r->index->sparse_checkout_patterns->use_cone_patterns)
		પાછા;

	/*
	 * Use the sparse index as a data structure to assist finding
	 * directories that are safe to delete. This conversion to a
	 * sparse index will not delete directories that contain
	 * conflicted entries or submodules.
	 */
	જો (r->index->sparse_index == INDEX_EXPANDED) {
		/*
		 * If something, such as a merge conflict or other concern,
		 * prevents us from converting to a sparse index, then do
		 * not try deleting files.
		 */
		જો (convert_to_sparse(r->index, SPARSE_INDEX_MEMORY_ONLY))
			પાછા;
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
	માટે (i = 0; i < r->index->cache_nr; i++) {
		સંરચના cache_entry *ce = r->index->cache[i];

		જો (S_ISSPARSEDIR(ce->ce_mode) &&
		    repo_file_exists(r, ce->name))
			string_list_append(&sparse_dirs, ce->name);
	}

	for_each_string_list_item(item, &sparse_dirs) {
		સંરચના dir_struct dir = DIR_INIT;
		સંરચના pathspec p = { 0 };
		સંરચના strvec s = STRVEC_INIT;

		strbuf_setlen(&path, pathlen);
		strbuf_addstr(&path, item->string);

		dir.flags |= DIR_SHOW_IGNORED_TOO;

		setup_standard_excludes(&dir);
		strvec_ધકેલો(&s, path.buf);

		parse_pathspec(&p, PATHSPEC_GLOB, 0, ખાલી, s.v);
		fill_directory(&dir, r->index, &p);

		જો (dir.nr) {
			warning(_("directory '%s' contains untracked files,"
				  " but is not in the sparse-checkout cone"),
				item->string);
		} નહીંતર જો (remove_dir_recursively(&path, 0)) {
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

	જો (was_full)
		ensure_full_index(r->index);
}

સ્થિર પૂર્ણાંક update_working_directory(સંરચના repository *r,
				    સંરચના pattern_list *pl)
{
	ગણના update_sparsity_result result;
	સંરચના unpack_trees_options o;
	સંરચના lock_file lock_file = LOCK_INIT;
	સંરચના pattern_list *old_pl;

	/* If no branch has been checked out, there are no updates to make. */
	જો (is_index_unborn(r->index))
		પાછા UPDATE_SPARSITY_SUCCESS;

	old_pl = r->index->sparse_checkout_patterns;
	r->index->sparse_checkout_patterns = pl;

	memset(&o, 0, કદ(o));
	o.verbose_update = isatty(2);
	o.update = 1;
	o.head_idx = -1;
	o.src_index = r->index;
	o.dst_index = r->index;
	o.skip_sparse_checkout = 0;

	setup_work_વૃક્ષ(the_repository);

	repo_hold_locked_index(r, &lock_file, LOCK_DIE_ON_ERROR);

	setup_unpack_trees_porcelain(&o, "sparse-checkout");
	result = update_sparsity(&o, pl);
	clear_unpack_trees_porcelain(&o);

	જો (result == UPDATE_SPARSITY_WARNINGS)
		/*
		 * We don't do any special handling of warnings from untracked
		 * files in the way or dirty entries that can't be removed.
		 */
		result = UPDATE_SPARSITY_SUCCESS;
	જો (result == UPDATE_SPARSITY_SUCCESS)
		write_locked_index(r->index, &lock_file, COMMIT_LOCK);
	નહીંતર
		rollback_lock_file(&lock_file);

	clean_tracked_sparse_directories(r);

	જો (r->index->sparse_checkout_patterns != pl) {
		clear_pattern_list(r->index->sparse_checkout_patterns);
		FREE_AND_NULL(r->index->sparse_checkout_patterns);
	}
	r->index->sparse_checkout_patterns = old_pl;

	પાછા result;
}

સ્થિર અક્ષર *escaped_pattern(અક્ષર *pattern)
{
	અક્ષર *p = pattern;
	સંરચના strbuf final = STRBUF_INIT;

	જ્યારે (*p) {
		જો (is_glob_special(*p))
			strbuf_addch(&final, '\\');

		strbuf_addch(&final, *p);
		p++;
	}

	પાછા strbuf_detach(&final, ખાલી);
}

સ્થિર શૂન્ય write_cone_to_file(FILE *fp, સંરચના pattern_list *pl)
{
	પૂર્ણાંક i;
	સંરચના pattern_entry *pe;
	સંરચના hashmap_iter iter;
	સંરચના string_list sl = STRING_LIST_INIT_DUP;
	સંરચના strbuf parent_pattern = STRBUF_INIT;

	hashmap_for_each_entry(&pl->parent_hashmap, &iter, pe, ent) {
		જો (hashmap_get_entry(&pl->recursive_hashmap, pe, ent, ખાલી))
			ચાલુ;

		જો (!hashmap_contains_parent(&pl->recursive_hashmap,
					     pe->pattern,
					     &parent_pattern))
			string_list_append(&sl, pe->pattern);
	}

	string_list_sort_u(&sl, 0);

	fprintf(fp, "/*\n!/*/\n");

	માટે (i = 0; i < sl.nr; i++) {
		અક્ષર *pattern = escaped_pattern(sl.વસ્તુઓ[i].string);

		જો (strlen(pattern))
			fprintf(fp, "%s/\n!%s/*/\n", pattern, pattern);
		મુક્ત(pattern);
	}

	string_list_clear(&sl, 0);

	hashmap_for_each_entry(&pl->recursive_hashmap, &iter, pe, ent) {
		જો (!hashmap_contains_parent(&pl->recursive_hashmap,
					     pe->pattern,
					     &parent_pattern))
			string_list_append(&sl, pe->pattern);
	}

	strbuf_release(&parent_pattern);

	string_list_sort_u(&sl, 0);

	માટે (i = 0; i < sl.nr; i++) {
		અક્ષર *pattern = escaped_pattern(sl.વસ્તુઓ[i].string);
		fprintf(fp, "%s/\n", pattern);
		મુક્ત(pattern);
	}

	string_list_clear(&sl, 0);
}

સ્થિર પૂર્ણાંક write_patterns_and_update(સંરચના repository *repo,
				     સંરચના pattern_list *pl)
{
	અક્ષર *sparse_filename;
	FILE *fp;
	સંરચના lock_file lk = LOCK_INIT;
	પૂર્ણાંક result;
	સંરચના repo_config_કિંમતો *cfg = repo_config_કિંમતો(the_repository);

	sparse_filename = get_sparse_checkout_filename();

	જો (safe_create_leading_directories(repo, sparse_filename))
		die(_("failed to create directory for sparse-checkout file"));

	repo_hold_lock_file_for_update(repo, &lk, sparse_filename,
				       LOCK_DIE_ON_ERROR);

	result = update_working_directory(repo, pl);
	જો (result) {
		rollback_lock_file(&lk);
		update_working_directory(repo, ખાલી);
		જાઓ out;
	}

	fp = fdopen_lock_file(&lk, "w");
	જો (!fp)
		die_errno(_("unable to fdopen %s"), get_lock_file_path(&lk));

	જો (cfg->core_sparse_checkout_cone)
		write_cone_to_file(fp, pl);
	નહીંતર
		write_patterns_to_file(fp, pl);

	જો (commit_lock_file(&lk))
		die_errno(_("unable to write %s"), sparse_filename);

out:
	clear_pattern_list(pl);
	મુક્ત(sparse_filename);
	પાછા result;
}

ગણના sparse_checkout_mode {
	MODE_NO_PATTERNS = 0,
	MODE_ALL_PATTERNS = 1,
	MODE_CONE_PATTERNS = 2,
};

સ્થિર પૂર્ણાંક set_config(સંરચના repository *repo,
		      ગણના sparse_checkout_mode mode)
{
	/* Update to use worktree config, if not already. */
	જો (init_worktree_config(repo)) {
		error(_("failed to initialize worktree config"));
		પાછા 1;
	}

	જો (repo_config_set_worktree_gently(repo,
					    "core.sparseCheckout",
					    mode ? "true" : "false") ||
	    repo_config_set_worktree_gently(repo,
					    "core.sparseCheckoutCone",
					    mode == MODE_CONE_PATTERNS ?
						"true" : "false"))
		પાછા 1;

	જો (mode == MODE_NO_PATTERNS)
		પાછા set_sparse_index_config(repo, 0);

	પાછા 0;
}

સ્થિર ગણના sparse_checkout_mode update_cone_mode(પૂર્ણાંક *cone_mode) {
	સંરચના repo_config_કિંમતો *cfg = repo_config_કિંમતો(the_repository);

	/* If not specified, use previous definition of cone mode */
	જો (*cone_mode == -1 && cfg->apply_sparse_checkout)
		*cone_mode = cfg->core_sparse_checkout_cone;

	/* Set cone/non-cone mode appropriately */
	cfg->apply_sparse_checkout = 1;
	જો (*cone_mode == 1 || *cone_mode == -1) {
		cfg->core_sparse_checkout_cone = 1;
		પાછા MODE_CONE_PATTERNS;
	}
	cfg->core_sparse_checkout_cone = 0;
	પાછા MODE_ALL_PATTERNS;
}

સ્થિર પૂર્ણાંક update_modes(સંરચના repository *repo, પૂર્ણાંક *cone_mode, પૂર્ણાંક *sparse_index)
{
	પૂર્ણાંક mode, record_mode;
	સંરચના repo_config_કિંમતો *cfg = repo_config_કિંમતો(the_repository);

	/* Determine if we need to record the mode; ensure sparse checkout on */
	record_mode = (*cone_mode != -1) || !cfg->apply_sparse_checkout;

	mode = update_cone_mode(cone_mode);
	જો (record_mode && set_config(repo, mode))
		પાછા 1;

	/* Set sparse-index/non-sparse-index mode if specified */
	જો (*sparse_index >= 0) {
		જો (set_sparse_index_config(repo, *sparse_index) < 0)
			die(_("failed to modify sparse-index config"));

		/* force an index rewrite */
		repo_read_index(repo);
		repo->index->updated_workdir = 1;

		જો (!*sparse_index)
			ensure_full_index(repo->index);
	}

	પાછા 0;
}

સ્થિર અક્ષર અચલ * અચલ builtin_sparse_checkout_init_usage[] = {
	"git sparse-checkout init [--cone] [--[no-]sparse-index]",
	ખાલી
};

સ્થિર સંરચના sparse_checkout_init_opts {
	પૂર્ણાંક cone_mode;
	પૂર્ણાંક sparse_index;
} init_opts;

સ્થિર પૂર્ણાંક sparse_checkout_init(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
				સંરચના repository *repo)
{
	સંરચના pattern_list pl;
	અક્ષર *sparse_filename;
	પૂર્ણાંક res;
	સંરચના object_id oid;

	સ્થિર સંરચના option builtin_sparse_checkout_init_options[] = {
		OPT_BOOL(0, "cone", &init_opts.cone_mode,
			 N_("initialize the sparse-checkout in cone mode")),
		OPT_BOOL(0, "sparse-index", &init_opts.sparse_index,
			 N_("toggle the use of a sparse index")),
		OPT_END(),
	};

	setup_work_વૃક્ષ(the_repository);
	repo_read_index(repo);

	init_opts.cone_mode = -1;
	init_opts.sparse_index = -1;

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_init_options,
			     builtin_sparse_checkout_init_usage, 0);

	જો (update_modes(repo, &init_opts.cone_mode, &init_opts.sparse_index))
		પાછા 1;

	memset(&pl, 0, કદ(pl));

	sparse_filename = get_sparse_checkout_filename();
	res = add_patterns_from_file_to_list(sparse_filename, "", 0, &pl, ખાલી, 0);

	/* If we already have a sparse-checkout file, use it. */
	જો (res >= 0) {
		મુક્ત(sparse_filename);
		clear_pattern_list(&pl);
		પાછા update_working_directory(repo, ખાલી);
	}

	જો (repo_get_oid(repo, "HEAD", &oid)) {
		FILE *fp;

		/* assume we are in a fresh repo, but update the sparse-checkout file */
		જો (safe_create_leading_directories(repo, sparse_filename))
			die(_("unable to create leading directories of %s"),
			    sparse_filename);
		fp = xfopen(sparse_filename, "w");
		જો (!fp)
			die(_("failed to open '%s'"), sparse_filename);

		મુક્ત(sparse_filename);
		fprintf(fp, "/*\n!/*/\n");
		fclose(fp);
		પાછા 0;
	}

	મુક્ત(sparse_filename);

	add_pattern("/*", empty_base, 0, &pl, 0);
	add_pattern("!/*/", empty_base, 0, &pl, 0);
	pl.use_cone_patterns = init_opts.cone_mode;

	પાછા write_patterns_and_update(repo, &pl);
}

સ્થિર શૂન્ય ઉમેરો_recursive_pattern(સંરચના pattern_list *pl, સંરચના strbuf *path)
{
	સંરચના pattern_entry *e = xmalloc(કદ(*e));
	e->patternlen = path->len;
	e->pattern = strbuf_detach(path, ખાલી);
	hashmap_entry_init(&e->ent, fspathhash(e->pattern));

	hashmap_add(&pl->recursive_hashmap, &e->ent);

	જ્યારે (e->patternlen) {
		અક્ષર *slash = strrchr(e->pattern, '/');
		અક્ષર *oldpattern = e->pattern;
		size_t newlen;
		સંરચના pattern_entry *dup;

		જો (!slash || slash == e->pattern)
			તોડો;

		newlen = slash - e->pattern;
		e = xmalloc(કદ(સંરચના pattern_entry));
		e->patternlen = newlen;
		e->pattern = xstrndup(oldpattern, newlen);
		hashmap_entry_init(&e->ent, fspathhash(e->pattern));

		dup = hashmap_get_entry(&pl->parent_hashmap, e, ent, ખાલી);
		જો (!dup) {
			hashmap_add(&pl->parent_hashmap, &e->ent);
		} નહીંતર {
			મુક્ત(e->pattern);
			મુક્ત(e);
			e = dup;
		}
	}
}

સ્થિર શૂન્ય strbuf_to_cone_pattern(સંરચના strbuf *લીટી, સંરચના pattern_list *pl)
{
	strbuf_trim(લીટી);

	strbuf_trim_trailing_dir_sep(લીટી);

	જો (strbuf_normalize_path(લીટી))
		die(_("could not normalize path %s"), લીટી->buf);

	જો (!લીટી->len)
		પાછા;

	જો (લીટી->buf[0] != '/')
		strbuf_insertstr(લીટી, 0, "/");

	ઉમેરો_recursive_pattern(pl, લીટી);
}

સ્થિર શૂન્ય add_patterns_from_આગત(સંરચના pattern_list *pl,
				    પૂર્ણાંક argc, અચલ અક્ષર **argv,
				    FILE *file)
{
	પૂર્ણાંક i;
	સંરચના repo_config_કિંમતો *cfg = repo_config_કિંમતો(the_repository);

	જો (cfg->core_sparse_checkout_cone) {
		સંરચના strbuf લીટી = STRBUF_INIT;

		hashmap_init(&pl->recursive_hashmap, pl_hashmap_cmp, ખાલી, 0);
		hashmap_init(&pl->parent_hashmap, pl_hashmap_cmp, ખાલી, 0);
		pl->use_cone_patterns = 1;

		જો (file) {
			સંરચના strbuf unquoted = STRBUF_INIT;
			જ્યારે (!strbuf_getline(&લીટી, file)) {
				જો (લીટી.buf[0] == '"') {
					strbuf_reset(&unquoted);
					જો (unquote_c_style(&unquoted, લીટી.buf, ખાલી))
						die(_("unable to unquote C-style string '%s'"),
						લીટી.buf);

					strbuf_swap(&unquoted, &લીટી);
				}

				strbuf_to_cone_pattern(&લીટી, pl);
			}

			strbuf_release(&unquoted);
		} નહીંતર {
			માટે (i = 0; i < argc; i++) {
				strbuf_setlen(&લીટી, 0);
				strbuf_addstr(&લીટી, argv[i]);
				strbuf_to_cone_pattern(&લીટી, pl);
			}
		}
		strbuf_release(&લીટી);
	} નહીંતર {
		જો (file) {
			સંરચના strbuf લીટી = STRBUF_INIT;

			જ્યારે (!strbuf_getline(&લીટી, file))
				add_pattern(લીટી.buf, empty_base, 0, pl, 0);

			strbuf_release(&લીટી);
		} નહીંતર {
			માટે (i = 0; i < argc; i++)
				add_pattern(argv[i], empty_base, 0, pl, 0);
		}
	}
}

ગણના modify_type {
	REPLACE,
	ADD,
};

સ્થિર શૂન્ય add_patterns_cone_mode(પૂર્ણાંક argc, અચલ અક્ષર **argv,
				   સંરચના pattern_list *pl,
				   પૂર્ણાંક use_stdin)
{
	સંરચના strbuf buffer = STRBUF_INIT;
	સંરચના pattern_entry *pe;
	સંરચના hashmap_iter iter;
	સંરચના pattern_list existing;
	સંરચના repo_config_કિંમતો *cfg = repo_config_કિંમતો(the_repository);
	અક્ષર *sparse_filename = get_sparse_checkout_filename();

	add_patterns_from_આગત(pl, argc, argv,
				use_stdin ? stdin : ખાલી);

	memset(&existing, 0, કદ(existing));
	existing.use_cone_patterns = cfg->core_sparse_checkout_cone;

	જો (add_patterns_from_file_to_list(sparse_filename, "", 0,
					   &existing, ખાલી, 0))
		die(_("unable to load existing sparse-checkout patterns"));
	મુક્ત(sparse_filename);

	જો (!existing.use_cone_patterns)
		die(_("existing sparse-checkout patterns do not use cone mode"));

	hashmap_for_each_entry(&existing.recursive_hashmap, &iter, pe, ent) {
		જો (!hashmap_contains_parent(&pl->recursive_hashmap,
					pe->pattern, &buffer) ||
		    !hashmap_contains_parent(&pl->parent_hashmap,
					pe->pattern, &buffer)) {
			strbuf_reset(&buffer);
			strbuf_addstr(&buffer, pe->pattern);
			ઉમેરો_recursive_pattern(pl, &buffer);
		}
	}

	clear_pattern_list(&existing);
	strbuf_release(&buffer);
}

સ્થિર શૂન્ય add_patterns_literal(પૂર્ણાંક argc, અચલ અક્ષર **argv,
				 સંરચના pattern_list *pl,
				 પૂર્ણાંક use_stdin)
{
	અક્ષર *sparse_filename = get_sparse_checkout_filename();
	જો (add_patterns_from_file_to_list(sparse_filename, "", 0,
					   pl, ખાલી, 0))
		die(_("unable to load existing sparse-checkout patterns"));
	મુક્ત(sparse_filename);
	add_patterns_from_આગત(pl, argc, argv, use_stdin ? stdin : ખાલી);
}

સ્થિર પૂર્ણાંક modify_pattern_list(સંરચના repository *repo,
			       સંરચના strvec *args, પૂર્ણાંક use_stdin,
			       ગણના modify_type m)
{
	પૂર્ણાંક result;
	પૂર્ણાંક changed_config = 0;
	સંરચના pattern_list *pl = xcalloc(1, કદ(*pl));
	સંરચના repo_config_કિંમતો *cfg = repo_config_કિંમતો(the_repository);

	પસંદગી (m) {
	સ્થિતિ ADD:
		જો (cfg->core_sparse_checkout_cone)
			add_patterns_cone_mode(args->nr, args->v, pl, use_stdin);
		નહીંતર
			add_patterns_literal(args->nr, args->v, pl, use_stdin);
		તોડો;

	સ્થિતિ REPLACE:
		add_patterns_from_આગત(pl, args->nr, args->v,
					use_stdin ? stdin : ખાલી);
		તોડો;
	}

	જો (!cfg->apply_sparse_checkout) {
		set_config(repo, MODE_ALL_PATTERNS);
		cfg->apply_sparse_checkout = 1;
		changed_config = 1;
	}

	result = write_patterns_and_update(repo, pl);

	જો (result && changed_config)
		set_config(repo, MODE_NO_PATTERNS);

	clear_pattern_list(pl);
	મુક્ત(pl);
	પાછા result;
}

સ્થિર શૂન્ય sanitize_paths(સંરચના repository *repo,
			   સંરચના strvec *args,
			   અચલ અક્ષર *prefix, પૂર્ણાંક skip_checks)
{
	પૂર્ણાંક i;
	સંરચના repo_config_કિંમતો *cfg = repo_config_કિંમતો(the_repository);

	જો (!args->nr)
		પાછા;

	જો (prefix && *prefix && cfg->core_sparse_checkout_cone) {
		/*
		 * The args are not pathspecs, so unfortunately we
		 * cannot imitate how cmd_add() uses parse_pathspec().
		 */
		પૂર્ણાંક prefix_len = strlen(prefix);

		માટે (i = 0; i < args->nr; i++) {
			અક્ષર *prefixed_path = prefix_path(the_repository, prefix,
							  prefix_len, args->v[i]);
			strvec_replace(args, i, prefixed_path);
			મુક્ત(prefixed_path);
		}
	}

	જો (skip_checks)
		પાછા;

	જો (prefix && *prefix && !cfg->core_sparse_checkout_cone)
		die(_("please run from the toplevel directory in non-cone mode"));

	જો (cfg->core_sparse_checkout_cone) {
		માટે (i = 0; i < args->nr; i++) {
			જો (args->v[i][0] == '/')
				die(_("specify directories rather than patterns (no leading slash)"));
			જો (args->v[i][0] == '!')
				die(_("specify directories rather than patterns.  If your directory starts with a '!', pass --skip-checks"));
			જો (strpbrk(args->v[i], "*?[]"))
				die(_("specify directories rather than patterns.  If your directory really has any of '*?[]\\' in it, pass --skip-checks"));
		}
	}

	માટે (i = 0; i < args->nr; i++) {
		સંરચના cache_entry *ce;
		સંરચના index_state *index = repo->index;
		પૂર્ણાંક pos = index_name_pos(index, args->v[i], strlen(args->v[i]));

		જો (pos < 0)
			ચાલુ;
		ce = index->cache[pos];
		જો (S_ISSPARSEDIR(ce->ce_mode))
			ચાલુ;

		જો (cfg->core_sparse_checkout_cone)
			die(_("'%s' is not a directory; to treat it as a directory anyway, rerun with --skip-checks"), args->v[i]);
		નહીંતર
			warning(_("pass a leading slash before paths such as '%s' if you want a single file (see NON-CONE PROBLEMS in the git-sparse-checkout manual)."), args->v[i]);
	}
}

સ્થિર અક્ષર અચલ * અચલ builtin_sparse_checkout_add_usage[] = {
	N_("git sparse-checkout add [--skip-checks] (--stdin | <patterns>)"),
	ખાલી
};

સ્થિર સંરચના sparse_checkout_add_opts {
	પૂર્ણાંક skip_checks;
	પૂર્ણાંક use_stdin;
} add_opts;

સ્થિર પૂર્ણાંક sparse_checkout_add(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
			       સંરચના repository *repo)
{
	સ્થિર સંરચના option builtin_sparse_checkout_add_options[] = {
		OPT_BOOL_F(0, "skip-checks", &add_opts.skip_checks,
			   N_("skip some sanity checks on the given paths that might give false positives"),
			   PARSE_OPT_NONEG),
		OPT_BOOL(0, "stdin", &add_opts.use_stdin,
			 N_("read patterns from standard in")),
		OPT_END(),
	};
	સંરચના strvec patterns = STRVEC_INIT;
	પૂર્ણાંક ret;
	સંરચના repo_config_કિંમતો *cfg = repo_config_કિંમતો(the_repository);

	setup_work_વૃક્ષ(the_repository);
	જો (!cfg->apply_sparse_checkout)
		die(_("no sparse-checkout to add to"));

	repo_read_index(repo);

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_add_options,
			     builtin_sparse_checkout_add_usage, 0);

	માટે (પૂર્ણાંક i = 0; i < argc; i++)
		strvec_ધકેલો(&patterns, argv[i]);
	sanitize_paths(repo, &patterns, prefix, add_opts.skip_checks);

	ret = modify_pattern_list(repo, &patterns, add_opts.use_stdin, ADD);

	strvec_clear(&patterns);
	પાછા ret;
}

સ્થિર અક્ષર અચલ * અચલ builtin_sparse_checkout_set_usage[] = {
	N_("git sparse-checkout set [--[no-]cone] [--[no-]sparse-index] [--skip-checks] (--stdin | <patterns>)"),
	ખાલી
};

સ્થિર સંરચના sparse_checkout_set_opts {
	પૂર્ણાંક cone_mode;
	પૂર્ણાંક sparse_index;
	પૂર્ણાંક skip_checks;
	પૂર્ણાંક use_stdin;
} set_opts;

સ્થિર પૂર્ણાંક sparse_checkout_set(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
			       સંરચના repository *repo)
{
	સંરચના repo_config_કિંમતો *cfg = repo_config_કિંમતો(the_repository);
	પૂર્ણાંક default_patterns_nr = 2;
	અચલ અક્ષર *default_patterns[] = {"/*", "!/*/", ખાલી};

	સ્થિર સંરચના option builtin_sparse_checkout_set_options[] = {
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
	સંરચના strvec patterns = STRVEC_INIT;
	પૂર્ણાંક ret;

	setup_work_વૃક્ષ(the_repository);
	repo_read_index(repo);

	set_opts.cone_mode = -1;
	set_opts.sparse_index = -1;

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_set_options,
			     builtin_sparse_checkout_set_usage, 0);

	જો (update_modes(repo, &set_opts.cone_mode, &set_opts.sparse_index))
		પાછા 1;

	/*
	 * Cone mode automatically specifies the toplevel directory.  For
	 * non-cone mode, if nothing is specified, manually select just the
	 * top-level directory (much as 'init' would do).
	 */
	જો (!cfg->core_sparse_checkout_cone && !set_opts.use_stdin && argc == 0) {
		માટે (પૂર્ણાંક i = 0; i < default_patterns_nr; i++)
			strvec_ધકેલો(&patterns, default_patterns[i]);
	} નહીંતર {
		માટે (પૂર્ણાંક i = 0; i < argc; i++)
			strvec_ધકેલો(&patterns, argv[i]);
		sanitize_paths(repo, &patterns, prefix, set_opts.skip_checks);
	}

	ret = modify_pattern_list(repo, &patterns, set_opts.use_stdin, REPLACE);

	strvec_clear(&patterns);
	પાછા ret;
}

સ્થિર અક્ષર અચલ * અચલ builtin_sparse_checkout_reapply_usage[] = {
	"git sparse-checkout reapply [--[no-]cone] [--[no-]sparse-index]",
	ખાલી
};

સ્થિર સંરચના sparse_checkout_reapply_opts {
	પૂર્ણાંક cone_mode;
	પૂર્ણાંક sparse_index;
} reapply_opts;

સ્થિર પૂર્ણાંક sparse_checkout_reapply(પૂર્ણાંક argc, અચલ અક્ષર **argv,
				   અચલ અક્ષર *prefix,
				   સંરચના repository *repo)
{
	સ્થિર સંરચના option builtin_sparse_checkout_reapply_options[] = {
		OPT_BOOL(0, "cone", &reapply_opts.cone_mode,
			 N_("initialize the sparse-checkout in cone mode")),
		OPT_BOOL(0, "sparse-index", &reapply_opts.sparse_index,
			 N_("toggle the use of a sparse index")),
		OPT_END(),
	};
	સંરચના repo_config_કિંમતો *cfg = repo_config_કિંમતો(the_repository);

	setup_work_વૃક્ષ(the_repository);
	જો (!cfg->apply_sparse_checkout)
		die(_("must be in a sparse-checkout to reapply sparsity patterns"));

	reapply_opts.cone_mode = -1;
	reapply_opts.sparse_index = -1;

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_reapply_options,
			     builtin_sparse_checkout_reapply_usage, 0);

	repo_read_index(repo);

	જો (update_modes(repo, &reapply_opts.cone_mode, &reapply_opts.sparse_index))
		પાછા 1;

	પાછા update_working_directory(repo, ખાલી);
}

સ્થિર અક્ષર અચલ * અચલ builtin_sparse_checkout_clean_usage[] = {
	"git sparse-checkout clean [-n|--dry-run]",
	ખાલી
};

સ્થિર પૂર્ણાંક list_file_iterator(અચલ અક્ષર *path, અચલ શૂન્ય *data)
{
	અચલ અક્ષર *સંદેશ = data;

	printf(સંદેશ, path);
	પાછા 0;
}

સ્થિર શૂન્ય list_every_file_in_dir(અચલ અક્ષર *સંદેશ,
				   અચલ અક્ષર *directory)
{
	સંરચના strbuf path = STRBUF_INIT;

	strbuf_addstr(&path, directory);
	for_each_file_in_dir(&path, list_file_iterator, સંદેશ);
	strbuf_release(&path);
}

સ્થિર અચલ અક્ષર *સંદેશ_remove = N_("Removing %s\n");
સ્થિર અચલ અક્ષર *સંદેશ_would_remove = N_("Would remove %s\n");

સ્થિર પૂર્ણાંક sparse_checkout_clean(પૂર્ણાંક argc, અચલ અક્ષર **argv,
				   અચલ અક્ષર *prefix,
				   સંરચના repository *repo)
{
	સંરચના strbuf full_path = STRBUF_INIT;
	અચલ અક્ષર *સંદેશ = સંદેશ_remove;
	size_t worktree_len;
	પૂર્ણાંક force = 0, dry_run = 0, verbose = 0;
	પૂર્ણાંક require_force = 1;
	સંરચના repo_config_કિંમતો *cfg = repo_config_કિંમતો(the_repository);

	સંરચના option builtin_sparse_checkout_clean_options[] = {
		OPT__DRY_RUN(&dry_run, N_("dry run")),
		OPT__FORCE(&force, N_("force"), PARSE_OPT_NOCOMPLETE),
		OPT__VERBOSE(&verbose, N_("report each affected file, not just directories")),
		OPT_END(),
	};

	setup_work_વૃક્ષ(the_repository);
	જો (!cfg->apply_sparse_checkout)
		die(_("must be in a sparse-checkout to clean directories"));
	જો (!cfg->core_sparse_checkout_cone)
		die(_("must be in a cone-mode sparse-checkout to clean directories"));

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_clean_options,
			     builtin_sparse_checkout_clean_usage, 0);

	repo_config_get_bool(repo, "clean.requireforce", &require_force);
	જો (require_force && !force && !dry_run)
		die(_("for safety, refusing to clean without one of --force or --dry-run"));

	જો (dry_run)
		સંદેશ = સંદેશ_would_remove;

	જો (repo_read_index(repo) < 0)
		die(_("failed to read index"));

	જો (convert_to_sparse(repo->index, SPARSE_INDEX_MEMORY_ONLY) ||
	    repo->index->sparse_index == INDEX_EXPANDED)
		die(_("failed to convert index to a sparse index; resolve merge conflicts and try again"));

	strbuf_addstr(&full_path, repo->worktree);
	strbuf_addch(&full_path, '/');
	worktree_len = full_path.len;

	માટે (size_t i = 0; i < repo->index->cache_nr; i++) {
		સંરચના cache_entry *ce = repo->index->cache[i];
		જો (!S_ISSPARSEDIR(ce->ce_mode))
			ચાલુ;
		strbuf_setlen(&full_path, worktree_len);
		strbuf_add(&full_path, ce->name, ce->ce_namelen);

		જો (!is_directory(full_path.buf))
			ચાલુ;

		જો (verbose)
			list_every_file_in_dir(સંદેશ, ce->name);
		નહીંતર
			printf(સંદેશ, ce->name);

		જો (dry_run <= 0 &&
		    remove_dir_recursively(&full_path, 0))
			warning_errno(_("failed to remove '%s'"), ce->name);
	}

	strbuf_release(&full_path);
	પાછા 0;
}

સ્થિર અક્ષર અચલ * અચલ builtin_sparse_checkout_disable_usage[] = {
	"git sparse-checkout disable",
	ખાલી
};

સ્થિર પૂર્ણાંક sparse_checkout_disable(પૂર્ણાંક argc, અચલ અક્ષર **argv,
				   અચલ અક્ષર *prefix,
				   સંરચના repository *repo)
{
	સ્થિર સંરચના option builtin_sparse_checkout_disable_options[] = {
		OPT_END(),
	};
	સંરચના pattern_list pl;
	સંરચના repo_config_કિંમતો *cfg = repo_config_કિંમતો(the_repository);

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

	setup_work_વૃક્ષ(the_repository);
	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_disable_options,
			     builtin_sparse_checkout_disable_usage, 0);

	/*
	 * Disable the advice message for expanding a sparse index, as we
	 * are expecting to do that when disabling sparse-checkout.
	 */
	give_advice_on_expansion = 0;
	repo_read_index(repo);

	memset(&pl, 0, કદ(pl));
	hashmap_init(&pl.recursive_hashmap, pl_hashmap_cmp, ખાલી, 0);
	hashmap_init(&pl.parent_hashmap, pl_hashmap_cmp, ખાલી, 0);
	pl.use_cone_patterns = 0;
	cfg->apply_sparse_checkout = 1;

	add_pattern("/*", empty_base, 0, &pl, 0);

	prepare_repo_settings(the_repository);
	repo->settings.sparse_index = 0;

	જો (update_working_directory(repo, &pl))
		die(_("error while refreshing working directory"));

	clear_pattern_list(&pl);
	પાછા set_config(repo, MODE_NO_PATTERNS);
}

સ્થિર અક્ષર અચલ * અચલ builtin_sparse_checkout_check_rules_usage[] = {
	N_("git sparse-checkout check-rules [-z] [--skip-checks]"
	   "[--[no-]cone] [--rules-file <file>]"),
	ખાલી
};

સ્થિર સંરચના sparse_checkout_check_rules_opts {
	પૂર્ણાંક cone_mode;
	પૂર્ણાંક null_termination;
	અક્ષર *rules_file;
} check_rules_opts;

સ્થિર પૂર્ણાંક check_rules(સંરચના repository *repo,
		       સંરચના pattern_list *pl,
		       પૂર્ણાંક null_terminated)
{
	સંરચના strbuf લીટી = STRBUF_INIT;
	સંરચના strbuf unquoted = STRBUF_INIT;
	અક્ષર *path;
	પૂર્ણાંક લીટી_terminator = null_terminated ? 0 : '\n';
	strbuf_getline_fn getline_fn = null_terminated ? strbuf_getline_nul
		: strbuf_getline;
	repo->index->sparse_checkout_patterns = pl;
	જ્યારે (!getline_fn(&લીટી, stdin)) {
		path = લીટી.buf;
		જો (!null_terminated && લીટી.buf[0] == '"') {
			strbuf_reset(&unquoted);
			જો (unquote_c_style(&unquoted, લીટી.buf, ખાલી))
				die(_("unable to unquote C-style string '%s'"),
					લીટી.buf);

			path = unquoted.buf;
		}

		જો (path_in_sparse_checkout(path, repo->index))
			write_name_quoted(path, stdout, લીટી_terminator);
	}
	strbuf_release(&લીટી);
	strbuf_release(&unquoted);

	પાછા 0;
}

સ્થિર પૂર્ણાંક sparse_checkout_check_rules(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
				       સંરચના repository *repo)
{
	સ્થિર સંરચના option builtin_sparse_checkout_check_rules_options[] = {
		OPT_BOOL('z', ખાલી, &check_rules_opts.null_termination,
			 N_("terminate input and output files by a NUL character")),
		OPT_BOOL(0, "cone", &check_rules_opts.cone_mode,
			 N_("when used with --rules-file interpret patterns as cone mode patterns")),
		OPT_FILENAME(0, "rules-file", &check_rules_opts.rules_file,
			 N_("use patterns in <file> instead of the current ones.")),
		OPT_END(),
	};

	FILE *fp;
	પૂર્ણાંક ret;
	સંરચના pattern_list pl = {0};
	સંરચના repo_config_કિંમતો *cfg = repo_config_કિંમતો(the_repository);
	અક્ષર *sparse_filename;
	check_rules_opts.cone_mode = -1;

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_check_rules_options,
			     builtin_sparse_checkout_check_rules_usage, 0);

	જો (check_rules_opts.rules_file && check_rules_opts.cone_mode < 0)
		check_rules_opts.cone_mode = 1;

	update_cone_mode(&check_rules_opts.cone_mode);
	pl.use_cone_patterns = cfg->core_sparse_checkout_cone;
	જો (check_rules_opts.rules_file) {
		fp = xfopen(check_rules_opts.rules_file, "r");
		add_patterns_from_આગત(&pl, argc, argv, fp);
		fclose(fp);
	} નહીંતર {
		sparse_filename = get_sparse_checkout_filename();
		જો (add_patterns_from_file_to_list(sparse_filename, "", 0, &pl,
						   ખાલી, 0))
			die(_("unable to load existing sparse-checkout patterns"));
		મુક્ત(sparse_filename);
	}

	ret = check_rules(repo, &pl, check_rules_opts.null_termination);
	clear_pattern_list(&pl);
	મુક્ત(check_rules_opts.rules_file);
	પાછા ret;
}

પૂર્ણાંક cmd_sparse_checkout(પૂર્ણાંક argc,
			અચલ અક્ષર **argv,
			અચલ અક્ષર *prefix,
			સંરચના repository *repo)
{
	parse_opt_subcommand_fn *fn = ખાલી;
	સંરચના option builtin_sparse_checkout_options[] = {
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

	repo_config(the_repository, git_default_config, ખાલી);

	prepare_repo_settings(repo);
	repo->settings.command_requires_full_index = 0;

	પાછા fn(argc, argv, prefix, repo);
}
