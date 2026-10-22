#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#include "builtin.h"
#include "abspath.h"
#include "config.h"
#include "dir.h"
#include "environment.h"
#include "gettext.h"
#include "object-file.h"
#include "object-name.h"
#include "parse-options.h"
#include "path.h"
#include "pathspec.h"
#include "strbuf.h"
#include "string-list.h"
#include "lockfile.h"
#include "unpack-trees.h"
#include "quote.h"
#include "setup.h"
#include "sparse-index.h"
#include "worktree.h"

static const var_na *empty_base = "";

static var_na const * const builtin_sparse_checkout_usage[] = {
	N_("git sparse-checkout (init | list | set | add | reapply | disable | check-rules | clean) [<options>]"),
	NULL
};

static void write_patterns_to_file(FILE *fp, sa_nracanaa pattern_list *pl)
{
	පූර්ණ_සංඛ්‍යා i;

	සඳහා (i = 0; i < pl->nr; i++) {
		sa_nracanaa path_pattern *p = pl->patterns[i];

		නම් (p->flags & PATTERN_FLAG_NEGATIVE)
			fprintf(fp, "!");

		fprintf(fp, "%s", p->pattern);

		නම් (p->flags & PATTERN_FLAG_MUSTBEDIR)
			fprintf(fp, "/");

		fprintf(fp, "\n");
	}
}

static var_na const * const builtin_sparse_checkout_list_usage[] = {
	"git sparse-checkout list",
	NULL
};

static පූර්ණ_සංඛ්‍යා sparse_checkout_list(පූර්ණ_සංඛ්‍යා argc, const var_na **argv, const var_na *prefix,
				sa_nracanaa repository *repo UNUSED)
{
	static sa_nracanaa option builtin_sparse_checkout_list_options[] = {
		OPT_END(),
	};
	sa_nracanaa pattern_list pl;
	var_na *sparse_filename;
	පූර්ණ_සංඛ්‍යා res;
	sa_nracanaa repo_config_values *cfg = repo_config_values(the_repository);

	setup_work_tree(the_repository);
	නම් (!cfg->apply_sparse_checkout)
		die(_("this worktree is not sparse"));

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_list_options,
			     builtin_sparse_checkout_list_usage, 0);

	memset(&pl, 0, parimaa_na(pl));

	pl.use_cone_patterns = cfg->core_sparse_checkout_cone;

	sparse_filename = get_sparse_checkout_filename();
	res = add_patterns_from_file_to_list(sparse_filename, "", 0, &pl, NULL, 0);
	free(sparse_filename);

	නම් (res < 0) {
		warning(_("this worktree is not sparse (sparse-checkout file may not exist)"));
		ආපසු 0;
	}

	නම් (pl.use_cone_patterns) {
		පූර්ණ_සංඛ්‍යා i;
		sa_nracanaa pattern_entry *pe;
		sa_nracanaa hashmap_iter iter;
		sa_nracanaa string_list sl = STRING_LIST_INIT_DUP;

		hashmap_for_each_entry(&pl.recursive_hashmap, &iter, pe, ent) {
			/* pe->pattern starts with "/", skip it */
			string_list_append(&sl, pe->pattern + 1);
		}

		string_list_sort_u(&sl, 0);

		සඳහා (i = 0; i < sl.nr; i++) {
			quote_c_style(sl.items[i].string, NULL, stdout, 0);
			printf("\n");
		}

		string_list_clear(&sl, 0);
	} නැතහොත් {
		write_patterns_to_file(stdout, &pl);
	}

	clear_pattern_list(&pl);

	ආපසු 0;
}

static void clean_tracked_sparse_directories(sa_nracanaa repository *r)
{
	පූර්ණ_සංඛ්‍යා i, was_full = 0;
	sa_nracanaa strbuf path = STRBUF_INIT;
	size_t pathlen;
	sa_nracanaa string_list_item *item;
	sa_nracanaa string_list sparse_dirs = STRING_LIST_INIT_DUP;

	/*
	 * If we are not using cone mode patterns, then we cannot
	 * delete directories outside of the sparse cone.
	 */
	නම් (!r || !r->index || !r->worktree)
		ආපසු;
	නම් (init_sparse_checkout_patterns(r->index) ||
	    !r->index->sparse_checkout_patterns->use_cone_patterns)
		ආපසු;

	/*
	 * Use the sparse index as a data structure to assist finding
	 * directories that are safe to delete. This conversion to a
	 * sparse index will not delete directories that contain
	 * conflicted entries or submodules.
	 */
	නම් (r->index->sparse_index == INDEX_EXPANDED) {
		/*
		 * If something, such as a merge conflict or other concern,
		 * prevents us from converting to a sparse index, then do
		 * not try deleting files.
		 */
		නම් (convert_to_sparse(r->index, SPARSE_INDEX_MEMORY_ONLY))
			ආපසු;
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
	සඳහා (i = 0; i < r->index->cache_nr; i++) {
		sa_nracanaa cache_entry *ce = r->index->cache[i];

		නම් (S_ISSPARSEDIR(ce->ce_mode) &&
		    repo_file_exists(r, ce->name))
			string_list_append(&sparse_dirs, ce->name);
	}

	for_each_string_list_item(item, &sparse_dirs) {
		sa_nracanaa dir_struct dir = DIR_INIT;
		sa_nracanaa pathspec p = { 0 };
		sa_nracanaa strvec s = STRVEC_INIT;

		strbuf_setlen(&path, pathlen);
		strbuf_addstr(&path, item->string);

		dir.flags |= DIR_SHOW_IGNORED_TOO;

		setup_standard_excludes(&dir);
		strvec_push(&s, path.buf);

		parse_pathspec(&p, PATHSPEC_GLOB, 0, NULL, s.v);
		fill_directory(&dir, r->index, &p);

		නම් (dir.nr) {
			warning(_("directory '%s' contains untracked files,"
				  " but is not in the sparse-checkout cone"),
				item->string);
		} නැතහොත් නම් (remove_dir_recursively(&path, 0)) {
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

	නම් (was_full)
		ensure_full_index(r->index);
}

static පූර්ණ_සංඛ්‍යා update_working_directory(sa_nracanaa repository *r,
				    sa_nracanaa pattern_list *pl)
{
	ga_nanaa update_sparsity_result result;
	sa_nracanaa unpack_trees_options o;
	sa_nracanaa lock_file lock_file = LOCK_INIT;
	sa_nracanaa pattern_list *old_pl;

	/* If no branch has been checked out, there are no updates to make. */
	නම් (is_index_unborn(r->index))
		ආපසු UPDATE_SPARSITY_SUCCESS;

	old_pl = r->index->sparse_checkout_patterns;
	r->index->sparse_checkout_patterns = pl;

	memset(&o, 0, parimaa_na(o));
	o.verbose_update = isatty(2);
	o.update = 1;
	o.head_idx = -1;
	o.src_index = r->index;
	o.dst_index = r->index;
	o.skip_sparse_checkout = 0;

	setup_work_tree(the_repository);

	repo_hold_locked_index(r, &lock_file, LOCK_DIE_ON_ERROR);

	setup_unpack_trees_porcelain(&o, "sparse-checkout");
	result = update_sparsity(&o, pl);
	clear_unpack_trees_porcelain(&o);

	නම් (result == UPDATE_SPARSITY_WARNINGS)
		/*
		 * We don't do any special handling of warnings from untracked
		 * files in the way or dirty entries that can't be removed.
		 */
		result = UPDATE_SPARSITY_SUCCESS;
	නම් (result == UPDATE_SPARSITY_SUCCESS)
		write_locked_index(r->index, &lock_file, COMMIT_LOCK);
	නැතහොත්
		rollback_lock_file(&lock_file);

	clean_tracked_sparse_directories(r);

	නම් (r->index->sparse_checkout_patterns != pl) {
		clear_pattern_list(r->index->sparse_checkout_patterns);
		FREE_AND_NULL(r->index->sparse_checkout_patterns);
	}
	r->index->sparse_checkout_patterns = old_pl;

	ආපසු result;
}

static var_na *escaped_pattern(var_na *pattern)
{
	var_na *p = pattern;
	sa_nracanaa strbuf final = STRBUF_INIT;

	අතරතුර (*p) {
		නම් (is_glob_special(*p))
			strbuf_addch(&final, '\\');

		strbuf_addch(&final, *p);
		p++;
	}

	ආපසු strbuf_detach(&final, NULL);
}

static void write_cone_to_file(FILE *fp, sa_nracanaa pattern_list *pl)
{
	පූර්ණ_සංඛ්‍යා i;
	sa_nracanaa pattern_entry *pe;
	sa_nracanaa hashmap_iter iter;
	sa_nracanaa string_list sl = STRING_LIST_INIT_DUP;
	sa_nracanaa strbuf parent_pattern = STRBUF_INIT;

	hashmap_for_each_entry(&pl->parent_hashmap, &iter, pe, ent) {
		නම් (hashmap_get_entry(&pl->recursive_hashmap, pe, ent, NULL))
			continue;

		නම් (!hashmap_contains_parent(&pl->recursive_hashmap,
					     pe->pattern,
					     &parent_pattern))
			string_list_append(&sl, pe->pattern);
	}

	string_list_sort_u(&sl, 0);

	fprintf(fp, "/*\n!/*/\n");

	සඳහා (i = 0; i < sl.nr; i++) {
		var_na *pattern = escaped_pattern(sl.items[i].string);

		නම් (strlen(pattern))
			fprintf(fp, "%s/\n!%s/*/\n", pattern, pattern);
		free(pattern);
	}

	string_list_clear(&sl, 0);

	hashmap_for_each_entry(&pl->recursive_hashmap, &iter, pe, ent) {
		නම් (!hashmap_contains_parent(&pl->recursive_hashmap,
					     pe->pattern,
					     &parent_pattern))
			string_list_append(&sl, pe->pattern);
	}

	strbuf_release(&parent_pattern);

	string_list_sort_u(&sl, 0);

	සඳහා (i = 0; i < sl.nr; i++) {
		var_na *pattern = escaped_pattern(sl.items[i].string);
		fprintf(fp, "%s/\n", pattern);
		free(pattern);
	}

	string_list_clear(&sl, 0);
}

static පූර්ණ_සංඛ්‍යා write_patterns_and_update(sa_nracanaa repository *repo,
				     sa_nracanaa pattern_list *pl)
{
	var_na *sparse_filename;
	FILE *fp;
	sa_nracanaa lock_file lk = LOCK_INIT;
	පූර්ණ_සංඛ්‍යා result;
	sa_nracanaa repo_config_values *cfg = repo_config_values(the_repository);

	sparse_filename = get_sparse_checkout_filename();

	නම් (safe_create_leading_directories(repo, sparse_filename))
		die(_("failed to create directory for sparse-checkout file"));

	repo_hold_lock_file_for_update(repo, &lk, sparse_filename,
				       LOCK_DIE_ON_ERROR);

	result = update_working_directory(repo, pl);
	නම් (result) {
		rollback_lock_file(&lk);
		update_working_directory(repo, NULL);
		goto out;
	}

	fp = fdopen_lock_file(&lk, "w");
	නම් (!fp)
		die_errno(_("unable to fdopen %s"), get_lock_file_path(&lk));

	නම් (cfg->core_sparse_checkout_cone)
		write_cone_to_file(fp, pl);
	නැතහොත්
		write_patterns_to_file(fp, pl);

	නම් (commit_lock_file(&lk))
		die_errno(_("unable to write %s"), sparse_filename);

out:
	clear_pattern_list(pl);
	free(sparse_filename);
	ආපසු result;
}

ga_nanaa sparse_checkout_mode {
	MODE_NO_PATTERNS = 0,
	MODE_ALL_PATTERNS = 1,
	MODE_CONE_PATTERNS = 2,
};

static පූර්ණ_සංඛ්‍යා set_config(sa_nracanaa repository *repo,
		      ga_nanaa sparse_checkout_mode mode)
{
	/* Update to use worktree config, if not already. */
	නම් (init_worktree_config(repo)) {
		error(_("failed to initialize worktree config"));
		ආපසු 1;
	}

	නම් (repo_config_set_worktree_gently(repo,
					    "core.sparseCheckout",
					    mode ? "true" : "false") ||
	    repo_config_set_worktree_gently(repo,
					    "core.sparseCheckoutCone",
					    mode == MODE_CONE_PATTERNS ?
						"true" : "false"))
		ආපසු 1;

	නම් (mode == MODE_NO_PATTERNS)
		ආපසු set_sparse_index_config(repo, 0);

	ආපසු 0;
}

static ga_nanaa sparse_checkout_mode update_cone_mode(පූර්ණ_සංඛ්‍යා *cone_mode) {
	sa_nracanaa repo_config_values *cfg = repo_config_values(the_repository);

	/* If not specified, use previous definition of cone mode */
	නම් (*cone_mode == -1 && cfg->apply_sparse_checkout)
		*cone_mode = cfg->core_sparse_checkout_cone;

	/* Set cone/non-cone mode appropriately */
	cfg->apply_sparse_checkout = 1;
	නම් (*cone_mode == 1 || *cone_mode == -1) {
		cfg->core_sparse_checkout_cone = 1;
		ආපසු MODE_CONE_PATTERNS;
	}
	cfg->core_sparse_checkout_cone = 0;
	ආපසු MODE_ALL_PATTERNS;
}

static පූර්ණ_සංඛ්‍යා update_modes(sa_nracanaa repository *repo, පූර්ණ_සංඛ්‍යා *cone_mode, පූර්ණ_සංඛ්‍යා *sparse_index)
{
	පූර්ණ_සංඛ්‍යා mode, record_mode;
	sa_nracanaa repo_config_values *cfg = repo_config_values(the_repository);

	/* Determine if we need to record the mode; ensure sparse checkout on */
	record_mode = (*cone_mode != -1) || !cfg->apply_sparse_checkout;

	mode = update_cone_mode(cone_mode);
	නම් (record_mode && set_config(repo, mode))
		ආපසු 1;

	/* Set sparse-index/non-sparse-index mode if specified */
	නම් (*sparse_index >= 0) {
		නම් (set_sparse_index_config(repo, *sparse_index) < 0)
			die(_("failed to modify sparse-index config"));

		/* force an index rewrite */
		repo_read_index(repo);
		repo->index->updated_workdir = 1;

		නම් (!*sparse_index)
			ensure_full_index(repo->index);
	}

	ආපසු 0;
}

static var_na const * const builtin_sparse_checkout_init_usage[] = {
	"git sparse-checkout init [--cone] [--[no-]sparse-index]",
	NULL
};

static sa_nracanaa sparse_checkout_init_opts {
	පූර්ණ_සංඛ්‍යා cone_mode;
	පූර්ණ_සංඛ්‍යා sparse_index;
} init_opts;

static පූර්ණ_සංඛ්‍යා sparse_checkout_init(පූර්ණ_සංඛ්‍යා argc, const var_na **argv, const var_na *prefix,
				sa_nracanaa repository *repo)
{
	sa_nracanaa pattern_list pl;
	var_na *sparse_filename;
	පූර්ණ_සංඛ්‍යා res;
	sa_nracanaa object_id oid;

	static sa_nracanaa option builtin_sparse_checkout_init_options[] = {
		OPT_BOOL(0, "cone", &init_opts.cone_mode,
			 N_("initialize the sparse-checkout in cone mode")),
		OPT_BOOL(0, "sparse-index", &init_opts.sparse_index,
			 N_("toggle the use of a sparse index")),
		OPT_END(),
	};

	setup_work_tree(the_repository);
	repo_read_index(repo);

	init_opts.cone_mode = -1;
	init_opts.sparse_index = -1;

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_init_options,
			     builtin_sparse_checkout_init_usage, 0);

	නම් (update_modes(repo, &init_opts.cone_mode, &init_opts.sparse_index))
		ආපසු 1;

	memset(&pl, 0, parimaa_na(pl));

	sparse_filename = get_sparse_checkout_filename();
	res = add_patterns_from_file_to_list(sparse_filename, "", 0, &pl, NULL, 0);

	/* If we already have a sparse-checkout file, use it. */
	නම් (res >= 0) {
		free(sparse_filename);
		clear_pattern_list(&pl);
		ආපසු update_working_directory(repo, NULL);
	}

	නම් (repo_get_oid(repo, "HEAD", &oid)) {
		FILE *fp;

		/* assume we are in a fresh repo, but update the sparse-checkout file */
		නම් (safe_create_leading_directories(repo, sparse_filename))
			die(_("unable to create leading directories of %s"),
			    sparse_filename);
		fp = xfopen(sparse_filename, "w");
		නම් (!fp)
			die(_("failed to open '%s'"), sparse_filename);

		free(sparse_filename);
		fprintf(fp, "/*\n!/*/\n");
		fclose(fp);
		ආපසු 0;
	}

	free(sparse_filename);

	add_pattern("/*", empty_base, 0, &pl, 0);
	add_pattern("!/*/", empty_base, 0, &pl, 0);
	pl.use_cone_patterns = init_opts.cone_mode;

	ආපසු write_patterns_and_update(repo, &pl);
}

static void insert_recursive_pattern(sa_nracanaa pattern_list *pl, sa_nracanaa strbuf *path)
{
	sa_nracanaa pattern_entry *e = xmalloc(parimaa_na(*e));
	e->patternlen = path->len;
	e->pattern = strbuf_detach(path, NULL);
	hashmap_entry_init(&e->ent, fspathhash(e->pattern));

	hashmap_add(&pl->recursive_hashmap, &e->ent);

	අතරතුර (e->patternlen) {
		var_na *slash = strrchr(e->pattern, '/');
		var_na *oldpattern = e->pattern;
		size_t newlen;
		sa_nracanaa pattern_entry *dup;

		නම් (!slash || slash == e->pattern)
			bha_nga;

		newlen = slash - e->pattern;
		e = xmalloc(parimaa_na(sa_nracanaa pattern_entry));
		e->patternlen = newlen;
		e->pattern = xstrndup(oldpattern, newlen);
		hashmap_entry_init(&e->ent, fspathhash(e->pattern));

		dup = hashmap_get_entry(&pl->parent_hashmap, e, ent, NULL);
		නම් (!dup) {
			hashmap_add(&pl->parent_hashmap, &e->ent);
		} නැතහොත් {
			free(e->pattern);
			free(e);
			e = dup;
		}
	}
}

static void strbuf_to_cone_pattern(sa_nracanaa strbuf *line, sa_nracanaa pattern_list *pl)
{
	strbuf_trim(line);

	strbuf_trim_trailing_dir_sep(line);

	නම් (strbuf_normalize_path(line))
		die(_("could not normalize path %s"), line->buf);

	නම් (!line->len)
		ආපසු;

	නම් (line->buf[0] != '/')
		strbuf_insertstr(line, 0, "/");

	insert_recursive_pattern(pl, line);
}

static void add_patterns_from_input(sa_nracanaa pattern_list *pl,
				    පූර්ණ_සංඛ්‍යා argc, const var_na **argv,
				    FILE *file)
{
	පූර්ණ_සංඛ්‍යා i;
	sa_nracanaa repo_config_values *cfg = repo_config_values(the_repository);

	නම් (cfg->core_sparse_checkout_cone) {
		sa_nracanaa strbuf line = STRBUF_INIT;

		hashmap_init(&pl->recursive_hashmap, pl_hashmap_cmp, NULL, 0);
		hashmap_init(&pl->parent_hashmap, pl_hashmap_cmp, NULL, 0);
		pl->use_cone_patterns = 1;

		නම් (file) {
			sa_nracanaa strbuf unquoted = STRBUF_INIT;
			අතරතුර (!strbuf_getline(&line, file)) {
				නම් (line.buf[0] == '"') {
					strbuf_reset(&unquoted);
					නම් (unquote_c_style(&unquoted, line.buf, NULL))
						die(_("unable to unquote C-style string '%s'"),
						line.buf);

					strbuf_swap(&unquoted, &line);
				}

				strbuf_to_cone_pattern(&line, pl);
			}

			strbuf_release(&unquoted);
		} නැතහොත් {
			සඳහා (i = 0; i < argc; i++) {
				strbuf_setlen(&line, 0);
				strbuf_addstr(&line, argv[i]);
				strbuf_to_cone_pattern(&line, pl);
			}
		}
		strbuf_release(&line);
	} නැතහොත් {
		නම් (file) {
			sa_nracanaa strbuf line = STRBUF_INIT;

			අතරතුර (!strbuf_getline(&line, file))
				add_pattern(line.buf, empty_base, 0, pl, 0);

			strbuf_release(&line);
		} නැතහොත් {
			සඳහා (i = 0; i < argc; i++)
				add_pattern(argv[i], empty_base, 0, pl, 0);
		}
	}
}

ga_nanaa modify_type {
	REPLACE,
	ADD,
};

static void add_patterns_cone_mode(පූර්ණ_සංඛ්‍යා argc, const var_na **argv,
				   sa_nracanaa pattern_list *pl,
				   පූර්ණ_සංඛ්‍යා use_stdin)
{
	sa_nracanaa strbuf buffer = STRBUF_INIT;
	sa_nracanaa pattern_entry *pe;
	sa_nracanaa hashmap_iter iter;
	sa_nracanaa pattern_list existing;
	sa_nracanaa repo_config_values *cfg = repo_config_values(the_repository);
	var_na *sparse_filename = get_sparse_checkout_filename();

	add_patterns_from_input(pl, argc, argv,
				use_stdin ? stdin : NULL);

	memset(&existing, 0, parimaa_na(existing));
	existing.use_cone_patterns = cfg->core_sparse_checkout_cone;

	නම් (add_patterns_from_file_to_list(sparse_filename, "", 0,
					   &existing, NULL, 0))
		die(_("unable to load existing sparse-checkout patterns"));
	free(sparse_filename);

	නම් (!existing.use_cone_patterns)
		die(_("existing sparse-checkout patterns do not use cone mode"));

	hashmap_for_each_entry(&existing.recursive_hashmap, &iter, pe, ent) {
		නම් (!hashmap_contains_parent(&pl->recursive_hashmap,
					pe->pattern, &buffer) ||
		    !hashmap_contains_parent(&pl->parent_hashmap,
					pe->pattern, &buffer)) {
			strbuf_reset(&buffer);
			strbuf_addstr(&buffer, pe->pattern);
			insert_recursive_pattern(pl, &buffer);
		}
	}

	clear_pattern_list(&existing);
	strbuf_release(&buffer);
}

static void add_patterns_literal(පූර්ණ_සංඛ්‍යා argc, const var_na **argv,
				 sa_nracanaa pattern_list *pl,
				 පූර්ණ_සංඛ්‍යා use_stdin)
{
	var_na *sparse_filename = get_sparse_checkout_filename();
	නම් (add_patterns_from_file_to_list(sparse_filename, "", 0,
					   pl, NULL, 0))
		die(_("unable to load existing sparse-checkout patterns"));
	free(sparse_filename);
	add_patterns_from_input(pl, argc, argv, use_stdin ? stdin : NULL);
}

static පූර්ණ_සංඛ්‍යා modify_pattern_list(sa_nracanaa repository *repo,
			       sa_nracanaa strvec *args, පූර්ණ_සංඛ්‍යා use_stdin,
			       ga_nanaa modify_type m)
{
	පූර්ණ_සංඛ්‍යා result;
	පූර්ණ_සංඛ්‍යා changed_config = 0;
	sa_nracanaa pattern_list *pl = xcalloc(1, parimaa_na(*pl));
	sa_nracanaa repo_config_values *cfg = repo_config_values(the_repository);

	switch (m) {
	pak_sa ADD:
		නම් (cfg->core_sparse_checkout_cone)
			add_patterns_cone_mode(args->nr, args->v, pl, use_stdin);
		නැතහොත්
			add_patterns_literal(args->nr, args->v, pl, use_stdin);
		bha_nga;

	pak_sa REPLACE:
		add_patterns_from_input(pl, args->nr, args->v,
					use_stdin ? stdin : NULL);
		bha_nga;
	}

	නම් (!cfg->apply_sparse_checkout) {
		set_config(repo, MODE_ALL_PATTERNS);
		cfg->apply_sparse_checkout = 1;
		changed_config = 1;
	}

	result = write_patterns_and_update(repo, pl);

	නම් (result && changed_config)
		set_config(repo, MODE_NO_PATTERNS);

	clear_pattern_list(pl);
	free(pl);
	ආපසු result;
}

static void sanitize_paths(sa_nracanaa repository *repo,
			   sa_nracanaa strvec *args,
			   const var_na *prefix, පූර්ණ_සංඛ්‍යා skip_checks)
{
	පූර්ණ_සංඛ්‍යා i;
	sa_nracanaa repo_config_values *cfg = repo_config_values(the_repository);

	නම් (!args->nr)
		ආපසු;

	නම් (prefix && *prefix && cfg->core_sparse_checkout_cone) {
		/*
		 * The args are not pathspecs, so unfortunately we
		 * cannot imitate how cmd_add() uses parse_pathspec().
		 */
		පූර්ණ_සංඛ්‍යා prefix_len = strlen(prefix);

		සඳහා (i = 0; i < args->nr; i++) {
			var_na *prefixed_path = prefix_path(the_repository, prefix,
							  prefix_len, args->v[i]);
			strvec_replace(args, i, prefixed_path);
			free(prefixed_path);
		}
	}

	නම් (skip_checks)
		ආපසු;

	නම් (prefix && *prefix && !cfg->core_sparse_checkout_cone)
		die(_("please run from the toplevel directory in non-cone mode"));

	නම් (cfg->core_sparse_checkout_cone) {
		සඳහා (i = 0; i < args->nr; i++) {
			නම් (args->v[i][0] == '/')
				die(_("specify directories rather than patterns (no leading slash)"));
			නම් (args->v[i][0] == '!')
				die(_("specify directories rather than patterns.  If your directory starts with a '!', pass --skip-checks"));
			නම් (strpbrk(args->v[i], "*?[]"))
				die(_("specify directories rather than patterns.  If your directory really has any of '*?[]\\' in it, pass --skip-checks"));
		}
	}

	සඳහා (i = 0; i < args->nr; i++) {
		sa_nracanaa cache_entry *ce;
		sa_nracanaa index_state *index = repo->index;
		පූර්ණ_සංඛ්‍යා pos = index_name_pos(index, args->v[i], strlen(args->v[i]));

		නම් (pos < 0)
			continue;
		ce = index->cache[pos];
		නම් (S_ISSPARSEDIR(ce->ce_mode))
			continue;

		නම් (cfg->core_sparse_checkout_cone)
			die(_("'%s' is not a directory; to treat it as a directory anyway, rerun with --skip-checks"), args->v[i]);
		නැතහොත්
			warning(_("pass a leading slash before paths such as '%s' if you want a single file (see NON-CONE PROBLEMS in the git-sparse-checkout manual)."), args->v[i]);
	}
}

static var_na const * const builtin_sparse_checkout_add_usage[] = {
	N_("git sparse-checkout add [--skip-checks] (--stdin | <patterns>)"),
	NULL
};

static sa_nracanaa sparse_checkout_add_opts {
	පූර්ණ_සංඛ්‍යා skip_checks;
	පූර්ණ_සංඛ්‍යා use_stdin;
} add_opts;

static පූර්ණ_සංඛ්‍යා sparse_checkout_add(පූර්ණ_සංඛ්‍යා argc, const var_na **argv, const var_na *prefix,
			       sa_nracanaa repository *repo)
{
	static sa_nracanaa option builtin_sparse_checkout_add_options[] = {
		OPT_BOOL_F(0, "skip-checks", &add_opts.skip_checks,
			   N_("skip some sanity checks on the given paths that might give false positives"),
			   PARSE_OPT_NONEG),
		OPT_BOOL(0, "stdin", &add_opts.use_stdin,
			 N_("read patterns from standard in")),
		OPT_END(),
	};
	sa_nracanaa strvec patterns = STRVEC_INIT;
	පූර්ණ_සංඛ්‍යා ret;
	sa_nracanaa repo_config_values *cfg = repo_config_values(the_repository);

	setup_work_tree(the_repository);
	නම් (!cfg->apply_sparse_checkout)
		die(_("no sparse-checkout to add to"));

	repo_read_index(repo);

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_add_options,
			     builtin_sparse_checkout_add_usage, 0);

	සඳහා (පූර්ණ_සංඛ්‍යා i = 0; i < argc; i++)
		strvec_push(&patterns, argv[i]);
	sanitize_paths(repo, &patterns, prefix, add_opts.skip_checks);

	ret = modify_pattern_list(repo, &patterns, add_opts.use_stdin, ADD);

	strvec_clear(&patterns);
	ආපසු ret;
}

static var_na const * const builtin_sparse_checkout_set_usage[] = {
	N_("git sparse-checkout set [--[no-]cone] [--[no-]sparse-index] [--skip-checks] (--stdin | <patterns>)"),
	NULL
};

static sa_nracanaa sparse_checkout_set_opts {
	පූර්ණ_සංඛ්‍යා cone_mode;
	පූර්ණ_සංඛ්‍යා sparse_index;
	පූර්ණ_සංඛ්‍යා skip_checks;
	පූර්ණ_සංඛ්‍යා use_stdin;
} set_opts;

static පූර්ණ_සංඛ්‍යා sparse_checkout_set(පූර්ණ_සංඛ්‍යා argc, const var_na **argv, const var_na *prefix,
			       sa_nracanaa repository *repo)
{
	sa_nracanaa repo_config_values *cfg = repo_config_values(the_repository);
	පූර්ණ_සංඛ්‍යා default_patterns_nr = 2;
	const var_na *default_patterns[] = {"/*", "!/*/", NULL};

	static sa_nracanaa option builtin_sparse_checkout_set_options[] = {
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
	sa_nracanaa strvec patterns = STRVEC_INIT;
	පූර්ණ_සංඛ්‍යා ret;

	setup_work_tree(the_repository);
	repo_read_index(repo);

	set_opts.cone_mode = -1;
	set_opts.sparse_index = -1;

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_set_options,
			     builtin_sparse_checkout_set_usage, 0);

	නම් (update_modes(repo, &set_opts.cone_mode, &set_opts.sparse_index))
		ආපසු 1;

	/*
	 * Cone mode automatically specifies the toplevel directory.  For
	 * non-cone mode, if nothing is specified, manually select just the
	 * top-level directory (much as 'init' would do).
	 */
	නම් (!cfg->core_sparse_checkout_cone && !set_opts.use_stdin && argc == 0) {
		සඳහා (පූර්ණ_සංඛ්‍යා i = 0; i < default_patterns_nr; i++)
			strvec_push(&patterns, default_patterns[i]);
	} නැතහොත් {
		සඳහා (පූර්ණ_සංඛ්‍යා i = 0; i < argc; i++)
			strvec_push(&patterns, argv[i]);
		sanitize_paths(repo, &patterns, prefix, set_opts.skip_checks);
	}

	ret = modify_pattern_list(repo, &patterns, set_opts.use_stdin, REPLACE);

	strvec_clear(&patterns);
	ආපසු ret;
}

static var_na const * const builtin_sparse_checkout_reapply_usage[] = {
	"git sparse-checkout reapply [--[no-]cone] [--[no-]sparse-index]",
	NULL
};

static sa_nracanaa sparse_checkout_reapply_opts {
	පූර්ණ_සංඛ්‍යා cone_mode;
	පූර්ණ_සංඛ්‍යා sparse_index;
} reapply_opts;

static පූර්ණ_සංඛ්‍යා sparse_checkout_reapply(පූර්ණ_සංඛ්‍යා argc, const var_na **argv,
				   const var_na *prefix,
				   sa_nracanaa repository *repo)
{
	static sa_nracanaa option builtin_sparse_checkout_reapply_options[] = {
		OPT_BOOL(0, "cone", &reapply_opts.cone_mode,
			 N_("initialize the sparse-checkout in cone mode")),
		OPT_BOOL(0, "sparse-index", &reapply_opts.sparse_index,
			 N_("toggle the use of a sparse index")),
		OPT_END(),
	};
	sa_nracanaa repo_config_values *cfg = repo_config_values(the_repository);

	setup_work_tree(the_repository);
	නම් (!cfg->apply_sparse_checkout)
		die(_("must be in a sparse-checkout to reapply sparsity patterns"));

	reapply_opts.cone_mode = -1;
	reapply_opts.sparse_index = -1;

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_reapply_options,
			     builtin_sparse_checkout_reapply_usage, 0);

	repo_read_index(repo);

	නම් (update_modes(repo, &reapply_opts.cone_mode, &reapply_opts.sparse_index))
		ආපසු 1;

	ආපසු update_working_directory(repo, NULL);
}

static var_na const * const builtin_sparse_checkout_clean_usage[] = {
	"git sparse-checkout clean [-n|--dry-run]",
	NULL
};

static පූර්ණ_සංඛ්‍යා list_file_iterator(const var_na *path, const void *data)
{
	const var_na *msg = data;

	printf(msg, path);
	ආපසු 0;
}

static void list_every_file_in_dir(const var_na *msg,
				   const var_na *directory)
{
	sa_nracanaa strbuf path = STRBUF_INIT;

	strbuf_addstr(&path, directory);
	for_each_file_in_dir(&path, list_file_iterator, msg);
	strbuf_release(&path);
}

static const var_na *msg_remove = N_("Removing %s\n");
static const var_na *msg_would_remove = N_("Would remove %s\n");

static පූර්ණ_සංඛ්‍යා sparse_checkout_clean(පූර්ණ_සංඛ්‍යා argc, const var_na **argv,
				   const var_na *prefix,
				   sa_nracanaa repository *repo)
{
	sa_nracanaa strbuf full_path = STRBUF_INIT;
	const var_na *msg = msg_remove;
	size_t worktree_len;
	පූර්ණ_සංඛ්‍යා force = 0, dry_run = 0, verbose = 0;
	පූර්ණ_සංඛ්‍යා require_force = 1;
	sa_nracanaa repo_config_values *cfg = repo_config_values(the_repository);

	sa_nracanaa option builtin_sparse_checkout_clean_options[] = {
		OPT__DRY_RUN(&dry_run, N_("dry run")),
		OPT__FORCE(&force, N_("force"), PARSE_OPT_NOCOMPLETE),
		OPT__VERBOSE(&verbose, N_("report each affected file, not just directories")),
		OPT_END(),
	};

	setup_work_tree(the_repository);
	නම් (!cfg->apply_sparse_checkout)
		die(_("must be in a sparse-checkout to clean directories"));
	නම් (!cfg->core_sparse_checkout_cone)
		die(_("must be in a cone-mode sparse-checkout to clean directories"));

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_clean_options,
			     builtin_sparse_checkout_clean_usage, 0);

	repo_config_get_bool(repo, "clean.requireforce", &require_force);
	නම් (require_force && !force && !dry_run)
		die(_("for safety, refusing to clean without one of --force or --dry-run"));

	නම් (dry_run)
		msg = msg_would_remove;

	නම් (repo_read_index(repo) < 0)
		die(_("failed to read index"));

	නම් (convert_to_sparse(repo->index, SPARSE_INDEX_MEMORY_ONLY) ||
	    repo->index->sparse_index == INDEX_EXPANDED)
		die(_("failed to convert index to a sparse index; resolve merge conflicts and try again"));

	strbuf_addstr(&full_path, repo->worktree);
	strbuf_addch(&full_path, '/');
	worktree_len = full_path.len;

	සඳහා (size_t i = 0; i < repo->index->cache_nr; i++) {
		sa_nracanaa cache_entry *ce = repo->index->cache[i];
		නම් (!S_ISSPARSEDIR(ce->ce_mode))
			continue;
		strbuf_setlen(&full_path, worktree_len);
		strbuf_add(&full_path, ce->name, ce->ce_namelen);

		නම් (!is_directory(full_path.buf))
			continue;

		නම් (verbose)
			list_every_file_in_dir(msg, ce->name);
		නැතහොත්
			printf(msg, ce->name);

		නම් (dry_run <= 0 &&
		    remove_dir_recursively(&full_path, 0))
			warning_errno(_("failed to remove '%s'"), ce->name);
	}

	strbuf_release(&full_path);
	ආපසු 0;
}

static var_na const * const builtin_sparse_checkout_disable_usage[] = {
	"git sparse-checkout disable",
	NULL
};

static පූර්ණ_සංඛ්‍යා sparse_checkout_disable(පූර්ණ_සංඛ්‍යා argc, const var_na **argv,
				   const var_na *prefix,
				   sa_nracanaa repository *repo)
{
	static sa_nracanaa option builtin_sparse_checkout_disable_options[] = {
		OPT_END(),
	};
	sa_nracanaa pattern_list pl;
	sa_nracanaa repo_config_values *cfg = repo_config_values(the_repository);

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

	setup_work_tree(the_repository);
	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_disable_options,
			     builtin_sparse_checkout_disable_usage, 0);

	/*
	 * Disable the advice message for expanding a sparse index, as we
	 * are expecting to do that when disabling sparse-checkout.
	 */
	give_advice_on_expansion = 0;
	repo_read_index(repo);

	memset(&pl, 0, parimaa_na(pl));
	hashmap_init(&pl.recursive_hashmap, pl_hashmap_cmp, NULL, 0);
	hashmap_init(&pl.parent_hashmap, pl_hashmap_cmp, NULL, 0);
	pl.use_cone_patterns = 0;
	cfg->apply_sparse_checkout = 1;

	add_pattern("/*", empty_base, 0, &pl, 0);

	prepare_repo_settings(the_repository);
	repo->settings.sparse_index = 0;

	නම් (update_working_directory(repo, &pl))
		die(_("error while refreshing working directory"));

	clear_pattern_list(&pl);
	ආපසු set_config(repo, MODE_NO_PATTERNS);
}

static var_na const * const builtin_sparse_checkout_check_rules_usage[] = {
	N_("git sparse-checkout check-rules [-z] [--skip-checks]"
	   "[--[no-]cone] [--rules-file <file>]"),
	NULL
};

static sa_nracanaa sparse_checkout_check_rules_opts {
	පූර්ණ_සංඛ්‍යා cone_mode;
	පූර්ණ_සංඛ්‍යා null_termination;
	var_na *rules_file;
} check_rules_opts;

static පූර්ණ_සංඛ්‍යා check_rules(sa_nracanaa repository *repo,
		       sa_nracanaa pattern_list *pl,
		       පූර්ණ_සංඛ්‍යා null_terminated)
{
	sa_nracanaa strbuf line = STRBUF_INIT;
	sa_nracanaa strbuf unquoted = STRBUF_INIT;
	var_na *path;
	පූර්ණ_සංඛ්‍යා line_terminator = null_terminated ? 0 : '\n';
	strbuf_getline_fn getline_fn = null_terminated ? strbuf_getline_nul
		: strbuf_getline;
	repo->index->sparse_checkout_patterns = pl;
	අතරතුර (!getline_fn(&line, stdin)) {
		path = line.buf;
		නම් (!null_terminated && line.buf[0] == '"') {
			strbuf_reset(&unquoted);
			නම් (unquote_c_style(&unquoted, line.buf, NULL))
				die(_("unable to unquote C-style string '%s'"),
					line.buf);

			path = unquoted.buf;
		}

		නම් (path_in_sparse_checkout(path, repo->index))
			write_name_quoted(path, stdout, line_terminator);
	}
	strbuf_release(&line);
	strbuf_release(&unquoted);

	ආපසු 0;
}

static පූර්ණ_සංඛ්‍යා sparse_checkout_check_rules(පූර්ණ_සංඛ්‍යා argc, const var_na **argv, const var_na *prefix,
				       sa_nracanaa repository *repo)
{
	static sa_nracanaa option builtin_sparse_checkout_check_rules_options[] = {
		OPT_BOOL('z', NULL, &check_rules_opts.null_termination,
			 N_("terminate input and output files by a NUL character")),
		OPT_BOOL(0, "cone", &check_rules_opts.cone_mode,
			 N_("when used with --rules-file interpret patterns as cone mode patterns")),
		OPT_FILENAME(0, "rules-file", &check_rules_opts.rules_file,
			 N_("use patterns in <file> instead of the current ones.")),
		OPT_END(),
	};

	FILE *fp;
	පූර්ණ_සංඛ්‍යා ret;
	sa_nracanaa pattern_list pl = {0};
	sa_nracanaa repo_config_values *cfg = repo_config_values(the_repository);
	var_na *sparse_filename;
	check_rules_opts.cone_mode = -1;

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_check_rules_options,
			     builtin_sparse_checkout_check_rules_usage, 0);

	නම් (check_rules_opts.rules_file && check_rules_opts.cone_mode < 0)
		check_rules_opts.cone_mode = 1;

	update_cone_mode(&check_rules_opts.cone_mode);
	pl.use_cone_patterns = cfg->core_sparse_checkout_cone;
	නම් (check_rules_opts.rules_file) {
		fp = xfopen(check_rules_opts.rules_file, "r");
		add_patterns_from_input(&pl, argc, argv, fp);
		fclose(fp);
	} නැතහොත් {
		sparse_filename = get_sparse_checkout_filename();
		නම් (add_patterns_from_file_to_list(sparse_filename, "", 0, &pl,
						   NULL, 0))
			die(_("unable to load existing sparse-checkout patterns"));
		free(sparse_filename);
	}

	ret = check_rules(repo, &pl, check_rules_opts.null_termination);
	clear_pattern_list(&pl);
	free(check_rules_opts.rules_file);
	ආපසු ret;
}

පූර්ණ_සංඛ්‍යා cmd_sparse_checkout(පූර්ණ_සංඛ්‍යා argc,
			const var_na **argv,
			const var_na *prefix,
			sa_nracanaa repository *repo)
{
	parse_opt_subcommand_fn *fn = NULL;
	sa_nracanaa option builtin_sparse_checkout_options[] = {
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

	repo_config(the_repository, git_default_config, NULL);

	prepare_repo_settings(repo);
	repo->settings.command_requires_full_index = 0;

	ආපසු fn(argc, argv, prefix, repo);
}
