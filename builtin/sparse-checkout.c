#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "abspath.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "dir.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "object-file.h"
#ଅନ୍ତର୍ଭାଵ "object-name.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "path.h"
#ଅନ୍ତର୍ଭାଵ "pathspec.h"
#ଅନ୍ତର୍ଭାଵ "strbuf.h"
#ଅନ୍ତର୍ଭାଵ "string-list.h"
#ଅନ୍ତର୍ଭାଵ "lockfile.h"
#ଅନ୍ତର୍ଭାଵ "unpack-trees.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"
#ଅନ୍ତର୍ଭାଵ "setup.h"
#ଅନ୍ତର୍ଭାଵ "sparse-index.h"
#ଅନ୍ତର୍ଭାଵ "worktree.h"

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *empty_base = "";

ସ୍ଥିର ଅକ୍ଷର ଅଚଳ * ଅଚଳ builtin_sparse_checkout_usage[] = {
	N_("git sparse-checkout (init | list | set | add | reapply | disable | check-rules | clean) [<options>]"),
	ଖାଲି
};

ସ୍ଥିର ଶୂନ୍ୟ write_patterns_to_file(FILE *fp, ସଂରଚନା pattern_list *pl)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	ପାଇଁ (i = 0; i < pl->nr; i++) {
		ସଂରଚନା path_pattern *p = pl->patterns[i];

		ଯଦି (p->flags & PATTERN_FLAG_NEGATIVE)
			fprintf(fp, "!");

		fprintf(fp, "%s", p->pattern);

		ଯଦି (p->flags & PATTERN_FLAG_MUSTBEDIR)
			fprintf(fp, "/");

		fprintf(fp, "\n");
	}
}

ସ୍ଥିର ଅକ୍ଷର ଅଚଳ * ଅଚଳ builtin_sparse_checkout_list_usage[] = {
	"git sparse-checkout list",
	ଖାଲି
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା sparse_checkout_list(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
				ସଂରଚନା repository *repo UNUSED)
{
	ସ୍ଥିର ସଂରଚନା option builtin_sparse_checkout_list_options[] = {
		OPT_END(),
	};
	ସଂରଚନା pattern_list pl;
	ଅକ୍ଷର *sparse_filename;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା res;
	ସଂରଚନା repo_config_ମୂଲ୍ୟଗୁଡ଼ିକ *cfg = repo_config_ମୂଲ୍ୟଗୁଡ଼ିକ(the_repository);

	setup_work_ବୃକ୍ଷ(the_repository);
	ଯଦି (!cfg->apply_sparse_checkout)
		die(_("this worktree is not sparse"));

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_list_options,
			     builtin_sparse_checkout_list_usage, 0);

	memset(&pl, 0, ଆକାର(pl));

	pl.use_cone_patterns = cfg->core_sparse_checkout_cone;

	sparse_filename = get_sparse_checkout_filename();
	res = add_patterns_from_file_to_list(sparse_filename, "", 0, &pl, ଖାଲି, 0);
	ମୁକ୍ତ(sparse_filename);

	ଯଦି (res < 0) {
		warning(_("this worktree is not sparse (sparse-checkout file may not exist)"));
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (pl.use_cone_patterns) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
		ସଂରଚନା pattern_entry *pe;
		ସଂରଚନା hashmap_iter iter;
		ସଂରଚନା string_list sl = STRING_LIST_INIT_DUP;

		hashmap_for_each_entry(&pl.recursive_hashmap, &iter, pe, ent) {
			/* pe->pattern starts with "/", skip it */
			string_list_append(&sl, pe->pattern + 1);
		}

		string_list_sort_u(&sl, 0);

		ପାଇଁ (i = 0; i < sl.nr; i++) {
			quote_c_style(sl.ବସ୍ତୁଗୁଡ଼ିକ[i].string, ଖାଲି, stdout, 0);
			printf("\n");
		}

		string_list_clear(&sl, 0);
	} ଅନ୍ୟଥା {
		write_patterns_to_file(stdout, &pl);
	}

	clear_pattern_list(&pl);

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ clean_tracked_sparse_directories(ସଂରଚନା repository *r)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, was_full = 0;
	ସଂରଚନା strbuf path = STRBUF_INIT;
	size_t pathlen;
	ସଂରଚନା string_list_item *item;
	ସଂରଚନା string_list sparse_dirs = STRING_LIST_INIT_DUP;

	/*
	 * If we are not using cone mode patterns, then we cannot
	 * delete directories outside of the sparse cone.
	 */
	ଯଦି (!r || !r->index || !r->worktree)
		ଫେରନ୍ତୁ;
	ଯଦି (init_sparse_checkout_patterns(r->index) ||
	    !r->index->sparse_checkout_patterns->use_cone_patterns)
		ଫେରନ୍ତୁ;

	/*
	 * Use the sparse index as a data structure to assist finding
	 * directories that are safe to delete. This conversion to a
	 * sparse index will not delete directories that contain
	 * conflicted entries or submodules.
	 */
	ଯଦି (r->index->sparse_index == INDEX_EXPANDED) {
		/*
		 * If something, such as a merge conflict or other concern,
		 * prevents us from converting to a sparse index, then do
		 * not try deleting files.
		 */
		ଯଦି (convert_to_sparse(r->index, SPARSE_INDEX_MEMORY_ONLY))
			ଫେରନ୍ତୁ;
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
	ପାଇଁ (i = 0; i < r->index->cache_nr; i++) {
		ସଂରଚନା cache_entry *ce = r->index->cache[i];

		ଯଦି (S_ISSPARSEDIR(ce->ce_mode) &&
		    repo_file_exists(r, ce->name))
			string_list_append(&sparse_dirs, ce->name);
	}

	for_each_string_list_item(item, &sparse_dirs) {
		ସଂରଚନା dir_struct dir = DIR_INIT;
		ସଂରଚନା pathspec p = { 0 };
		ସଂରଚନା strvec s = STRVEC_INIT;

		strbuf_setlen(&path, pathlen);
		strbuf_addstr(&path, item->string);

		dir.flags |= DIR_SHOW_IGNORED_TOO;

		setup_standard_excludes(&dir);
		strvec_ଠେଲ(&s, path.buf);

		parse_pathspec(&p, PATHSPEC_GLOB, 0, ଖାଲି, s.v);
		fill_directory(&dir, r->index, &p);

		ଯଦି (dir.nr) {
			warning(_("directory '%s' contains untracked files,"
				  " but is not in the sparse-checkout cone"),
				item->string);
		} ଅନ୍ୟଥା ଯଦି (remove_dir_recursively(&path, 0)) {
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

	ଯଦି (was_full)
		ensure_full_index(r->index);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା update_working_directory(ସଂରଚନା repository *r,
				    ସଂରଚନା pattern_list *pl)
{
	ଗଣନା update_sparsity_result result;
	ସଂରଚନା unpack_trees_options o;
	ସଂରଚନା lock_file lock_file = LOCK_INIT;
	ସଂରଚନା pattern_list *old_pl;

	/* If no branch has been checked out, there are no updates to make. */
	ଯଦି (is_index_unborn(r->index))
		ଫେରନ୍ତୁ UPDATE_SPARSITY_SUCCESS;

	old_pl = r->index->sparse_checkout_patterns;
	r->index->sparse_checkout_patterns = pl;

	memset(&o, 0, ଆକାର(o));
	o.verbose_update = isatty(2);
	o.update = 1;
	o.head_idx = -1;
	o.src_index = r->index;
	o.dst_index = r->index;
	o.skip_sparse_checkout = 0;

	setup_work_ବୃକ୍ଷ(the_repository);

	repo_hold_locked_index(r, &lock_file, LOCK_DIE_ON_ERROR);

	setup_unpack_trees_porcelain(&o, "sparse-checkout");
	result = update_sparsity(&o, pl);
	clear_unpack_trees_porcelain(&o);

	ଯଦି (result == UPDATE_SPARSITY_WARNINGS)
		/*
		 * We don't do any special handling of warnings from untracked
		 * files in the way or dirty entries that can't be removed.
		 */
		result = UPDATE_SPARSITY_SUCCESS;
	ଯଦି (result == UPDATE_SPARSITY_SUCCESS)
		write_locked_index(r->index, &lock_file, COMMIT_LOCK);
	ଅନ୍ୟଥା
		rollback_lock_file(&lock_file);

	clean_tracked_sparse_directories(r);

	ଯଦି (r->index->sparse_checkout_patterns != pl) {
		clear_pattern_list(r->index->sparse_checkout_patterns);
		FREE_AND_NULL(r->index->sparse_checkout_patterns);
	}
	r->index->sparse_checkout_patterns = old_pl;

	ଫେରନ୍ତୁ result;
}

ସ୍ଥିର ଅକ୍ଷର *escaped_pattern(ଅକ୍ଷର *pattern)
{
	ଅକ୍ଷର *p = pattern;
	ସଂରଚନା strbuf final = STRBUF_INIT;

	ଯେତେବେଳେ (*p) {
		ଯଦି (is_glob_special(*p))
			strbuf_addch(&final, '\\');

		strbuf_addch(&final, *p);
		p++;
	}

	ଫେରନ୍ତୁ strbuf_detach(&final, ଖାଲି);
}

ସ୍ଥିର ଶୂନ୍ୟ write_cone_to_file(FILE *fp, ସଂରଚନା pattern_list *pl)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	ସଂରଚନା pattern_entry *pe;
	ସଂରଚନା hashmap_iter iter;
	ସଂରଚନା string_list sl = STRING_LIST_INIT_DUP;
	ସଂରଚନା strbuf parent_pattern = STRBUF_INIT;

	hashmap_for_each_entry(&pl->parent_hashmap, &iter, pe, ent) {
		ଯଦି (hashmap_get_entry(&pl->recursive_hashmap, pe, ent, ଖାଲି))
			ଜାରି;

		ଯଦି (!hashmap_contains_parent(&pl->recursive_hashmap,
					     pe->pattern,
					     &parent_pattern))
			string_list_append(&sl, pe->pattern);
	}

	string_list_sort_u(&sl, 0);

	fprintf(fp, "/*\n!/*/\n");

	ପାଇଁ (i = 0; i < sl.nr; i++) {
		ଅକ୍ଷର *pattern = escaped_pattern(sl.ବସ୍ତୁଗୁଡ଼ିକ[i].string);

		ଯଦି (strlen(pattern))
			fprintf(fp, "%s/\n!%s/*/\n", pattern, pattern);
		ମୁକ୍ତ(pattern);
	}

	string_list_clear(&sl, 0);

	hashmap_for_each_entry(&pl->recursive_hashmap, &iter, pe, ent) {
		ଯଦି (!hashmap_contains_parent(&pl->recursive_hashmap,
					     pe->pattern,
					     &parent_pattern))
			string_list_append(&sl, pe->pattern);
	}

	strbuf_release(&parent_pattern);

	string_list_sort_u(&sl, 0);

	ପାଇଁ (i = 0; i < sl.nr; i++) {
		ଅକ୍ଷର *pattern = escaped_pattern(sl.ବସ୍ତୁଗୁଡ଼ିକ[i].string);
		fprintf(fp, "%s/\n", pattern);
		ମୁକ୍ତ(pattern);
	}

	string_list_clear(&sl, 0);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା write_patterns_and_update(ସଂରଚନା repository *repo,
				     ସଂରଚନା pattern_list *pl)
{
	ଅକ୍ଷର *sparse_filename;
	FILE *fp;
	ସଂରଚନା lock_file lk = LOCK_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା result;
	ସଂରଚନା repo_config_ମୂଲ୍ୟଗୁଡ଼ିକ *cfg = repo_config_ମୂଲ୍ୟଗୁଡ଼ିକ(the_repository);

	sparse_filename = get_sparse_checkout_filename();

	ଯଦି (safe_create_leading_directories(repo, sparse_filename))
		die(_("failed to create directory for sparse-checkout file"));

	repo_hold_lock_file_for_update(repo, &lk, sparse_filename,
				       LOCK_DIE_ON_ERROR);

	result = update_working_directory(repo, pl);
	ଯଦି (result) {
		rollback_lock_file(&lk);
		update_working_directory(repo, ଖାଲି);
		ଯାଅ out;
	}

	fp = fdopen_lock_file(&lk, "w");
	ଯଦି (!fp)
		die_errno(_("unable to fdopen %s"), get_lock_file_path(&lk));

	ଯଦି (cfg->core_sparse_checkout_cone)
		write_cone_to_file(fp, pl);
	ଅନ୍ୟଥା
		write_patterns_to_file(fp, pl);

	ଯଦି (commit_lock_file(&lk))
		die_errno(_("unable to write %s"), sparse_filename);

out:
	clear_pattern_list(pl);
	ମୁକ୍ତ(sparse_filename);
	ଫେରନ୍ତୁ result;
}

ଗଣନା sparse_checkout_mode {
	MODE_NO_PATTERNS = 0,
	MODE_ALL_PATTERNS = 1,
	MODE_CONE_PATTERNS = 2,
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା set_config(ସଂରଚନା repository *repo,
		      ଗଣନା sparse_checkout_mode mode)
{
	/* Update to use worktree config, if not already. */
	ଯଦି (init_worktree_config(repo)) {
		error(_("failed to initialize worktree config"));
		ଫେରନ୍ତୁ 1;
	}

	ଯଦି (repo_config_set_worktree_gently(repo,
					    "core.sparseCheckout",
					    mode ? "true" : "false") ||
	    repo_config_set_worktree_gently(repo,
					    "core.sparseCheckoutCone",
					    mode == MODE_CONE_PATTERNS ?
						"true" : "false"))
		ଫେରନ୍ତୁ 1;

	ଯଦି (mode == MODE_NO_PATTERNS)
		ଫେରନ୍ତୁ set_sparse_index_config(repo, 0);

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଗଣନା sparse_checkout_mode update_cone_mode(ପୂର୍ଣ୍ଣସଂଖ୍ୟା *cone_mode) {
	ସଂରଚନା repo_config_ମୂଲ୍ୟଗୁଡ଼ିକ *cfg = repo_config_ମୂଲ୍ୟଗୁଡ଼ିକ(the_repository);

	/* If not specified, use previous definition of cone mode */
	ଯଦି (*cone_mode == -1 && cfg->apply_sparse_checkout)
		*cone_mode = cfg->core_sparse_checkout_cone;

	/* Set cone/non-cone mode appropriately */
	cfg->apply_sparse_checkout = 1;
	ଯଦି (*cone_mode == 1 || *cone_mode == -1) {
		cfg->core_sparse_checkout_cone = 1;
		ଫେରନ୍ତୁ MODE_CONE_PATTERNS;
	}
	cfg->core_sparse_checkout_cone = 0;
	ଫେରନ୍ତୁ MODE_ALL_PATTERNS;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା update_modes(ସଂରଚନା repository *repo, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *cone_mode, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *sparse_index)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା mode, record_mode;
	ସଂରଚନା repo_config_ମୂଲ୍ୟଗୁଡ଼ିକ *cfg = repo_config_ମୂଲ୍ୟଗୁଡ଼ିକ(the_repository);

	/* Determine if we need to record the mode; ensure sparse checkout on */
	record_mode = (*cone_mode != -1) || !cfg->apply_sparse_checkout;

	mode = update_cone_mode(cone_mode);
	ଯଦି (record_mode && set_config(repo, mode))
		ଫେରନ୍ତୁ 1;

	/* Set sparse-index/non-sparse-index mode if specified */
	ଯଦି (*sparse_index >= 0) {
		ଯଦି (set_sparse_index_config(repo, *sparse_index) < 0)
			die(_("failed to modify sparse-index config"));

		/* force an index rewrite */
		repo_read_index(repo);
		repo->index->updated_workdir = 1;

		ଯଦି (!*sparse_index)
			ensure_full_index(repo->index);
	}

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଅକ୍ଷର ଅଚଳ * ଅଚଳ builtin_sparse_checkout_init_usage[] = {
	"git sparse-checkout init [--cone] [--[no-]sparse-index]",
	ଖାଲି
};

ସ୍ଥିର ସଂରଚନା sparse_checkout_init_opts {
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cone_mode;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା sparse_index;
} init_opts;

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା sparse_checkout_init(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
				ସଂରଚନା repository *repo)
{
	ସଂରଚନା pattern_list pl;
	ଅକ୍ଷର *sparse_filename;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା res;
	ସଂରଚନା object_id oid;

	ସ୍ଥିର ସଂରଚନା option builtin_sparse_checkout_init_options[] = {
		OPT_BOOL(0, "cone", &init_opts.cone_mode,
			 N_("initialize the sparse-checkout in cone mode")),
		OPT_BOOL(0, "sparse-index", &init_opts.sparse_index,
			 N_("toggle the use of a sparse index")),
		OPT_END(),
	};

	setup_work_ବୃକ୍ଷ(the_repository);
	repo_read_index(repo);

	init_opts.cone_mode = -1;
	init_opts.sparse_index = -1;

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_init_options,
			     builtin_sparse_checkout_init_usage, 0);

	ଯଦି (update_modes(repo, &init_opts.cone_mode, &init_opts.sparse_index))
		ଫେରନ୍ତୁ 1;

	memset(&pl, 0, ଆକାର(pl));

	sparse_filename = get_sparse_checkout_filename();
	res = add_patterns_from_file_to_list(sparse_filename, "", 0, &pl, ଖାଲି, 0);

	/* If we already have a sparse-checkout file, use it. */
	ଯଦି (res >= 0) {
		ମୁକ୍ତ(sparse_filename);
		clear_pattern_list(&pl);
		ଫେରନ୍ତୁ update_working_directory(repo, ଖାଲି);
	}

	ଯଦି (repo_get_oid(repo, "HEAD", &oid)) {
		FILE *fp;

		/* assume we are in a fresh repo, but update the sparse-checkout file */
		ଯଦି (safe_create_leading_directories(repo, sparse_filename))
			die(_("unable to create leading directories of %s"),
			    sparse_filename);
		fp = xfopen(sparse_filename, "w");
		ଯଦି (!fp)
			die(_("failed to open '%s'"), sparse_filename);

		ମୁକ୍ତ(sparse_filename);
		fprintf(fp, "/*\n!/*/\n");
		fclose(fp);
		ଫେରନ୍ତୁ 0;
	}

	ମୁକ୍ତ(sparse_filename);

	add_pattern("/*", empty_base, 0, &pl, 0);
	add_pattern("!/*/", empty_base, 0, &pl, 0);
	pl.use_cone_patterns = init_opts.cone_mode;

	ଫେରନ୍ତୁ write_patterns_and_update(repo, &pl);
}

ସ୍ଥିର ଶୂନ୍ୟ ଭର୍ତ୍ତି_recursive_pattern(ସଂରଚନା pattern_list *pl, ସଂରଚନା strbuf *path)
{
	ସଂରଚନା pattern_entry *e = xmalloc(ଆକାର(*e));
	e->patternlen = path->len;
	e->pattern = strbuf_detach(path, ଖାଲି);
	hashmap_entry_init(&e->ent, fspathhash(e->pattern));

	hashmap_add(&pl->recursive_hashmap, &e->ent);

	ଯେତେବେଳେ (e->patternlen) {
		ଅକ୍ଷର *slash = strrchr(e->pattern, '/');
		ଅକ୍ଷର *oldpattern = e->pattern;
		size_t newlen;
		ସଂରଚନା pattern_entry *dup;

		ଯଦି (!slash || slash == e->pattern)
			ଭାଙ୍ଗ;

		newlen = slash - e->pattern;
		e = xmalloc(ଆକାର(ସଂରଚନା pattern_entry));
		e->patternlen = newlen;
		e->pattern = xstrndup(oldpattern, newlen);
		hashmap_entry_init(&e->ent, fspathhash(e->pattern));

		dup = hashmap_get_entry(&pl->parent_hashmap, e, ent, ଖାଲି);
		ଯଦି (!dup) {
			hashmap_add(&pl->parent_hashmap, &e->ent);
		} ଅନ୍ୟଥା {
			ମୁକ୍ତ(e->pattern);
			ମୁକ୍ତ(e);
			e = dup;
		}
	}
}

ସ୍ଥିର ଶୂନ୍ୟ strbuf_to_cone_pattern(ସଂରଚନା strbuf *ଧାଡ଼ି, ସଂରଚନା pattern_list *pl)
{
	strbuf_trim(ଧାଡ଼ି);

	strbuf_trim_trailing_dir_sep(ଧାଡ଼ି);

	ଯଦି (strbuf_normalize_path(ଧାଡ଼ି))
		die(_("could not normalize path %s"), ଧାଡ଼ି->buf);

	ଯଦି (!ଧାଡ଼ି->len)
		ଫେରନ୍ତୁ;

	ଯଦି (ଧାଡ଼ି->buf[0] != '/')
		strbuf_insertstr(ଧାଡ଼ି, 0, "/");

	ଭର୍ତ୍ତି_recursive_pattern(pl, ଧାଡ଼ି);
}

ସ୍ଥିର ଶୂନ୍ୟ add_patterns_from_ନିବେଶ(ସଂରଚନା pattern_list *pl,
				    ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv,
				    FILE *file)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	ସଂରଚନା repo_config_ମୂଲ୍ୟଗୁଡ଼ିକ *cfg = repo_config_ମୂଲ୍ୟଗୁଡ଼ିକ(the_repository);

	ଯଦି (cfg->core_sparse_checkout_cone) {
		ସଂରଚନା strbuf ଧାଡ଼ି = STRBUF_INIT;

		hashmap_init(&pl->recursive_hashmap, pl_hashmap_cmp, ଖାଲି, 0);
		hashmap_init(&pl->parent_hashmap, pl_hashmap_cmp, ଖାଲି, 0);
		pl->use_cone_patterns = 1;

		ଯଦି (file) {
			ସଂରଚନା strbuf unquoted = STRBUF_INIT;
			ଯେତେବେଳେ (!strbuf_getline(&ଧାଡ଼ି, file)) {
				ଯଦି (ଧାଡ଼ି.buf[0] == '"') {
					strbuf_reset(&unquoted);
					ଯଦି (unquote_c_style(&unquoted, ଧାଡ଼ି.buf, ଖାଲି))
						die(_("unable to unquote C-style string '%s'"),
						ଧାଡ଼ି.buf);

					strbuf_swap(&unquoted, &ଧାଡ଼ି);
				}

				strbuf_to_cone_pattern(&ଧାଡ଼ି, pl);
			}

			strbuf_release(&unquoted);
		} ଅନ୍ୟଥା {
			ପାଇଁ (i = 0; i < argc; i++) {
				strbuf_setlen(&ଧାଡ଼ି, 0);
				strbuf_addstr(&ଧାଡ଼ି, argv[i]);
				strbuf_to_cone_pattern(&ଧାଡ଼ି, pl);
			}
		}
		strbuf_release(&ଧାଡ଼ି);
	} ଅନ୍ୟଥା {
		ଯଦି (file) {
			ସଂରଚନା strbuf ଧାଡ଼ି = STRBUF_INIT;

			ଯେତେବେଳେ (!strbuf_getline(&ଧାଡ଼ି, file))
				add_pattern(ଧାଡ଼ି.buf, empty_base, 0, pl, 0);

			strbuf_release(&ଧାଡ଼ି);
		} ଅନ୍ୟଥା {
			ପାଇଁ (i = 0; i < argc; i++)
				add_pattern(argv[i], empty_base, 0, pl, 0);
		}
	}
}

ଗଣନା modify_type {
	REPLACE,
	ADD,
};

ସ୍ଥିର ଶୂନ୍ୟ add_patterns_cone_mode(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv,
				   ସଂରଚନା pattern_list *pl,
				   ପୂର୍ଣ୍ଣସଂଖ୍ୟା use_stdin)
{
	ସଂରଚନା strbuf buffer = STRBUF_INIT;
	ସଂରଚନା pattern_entry *pe;
	ସଂରଚନା hashmap_iter iter;
	ସଂରଚନା pattern_list existing;
	ସଂରଚନା repo_config_ମୂଲ୍ୟଗୁଡ଼ିକ *cfg = repo_config_ମୂଲ୍ୟଗୁଡ଼ିକ(the_repository);
	ଅକ୍ଷର *sparse_filename = get_sparse_checkout_filename();

	add_patterns_from_ନିବେଶ(pl, argc, argv,
				use_stdin ? stdin : ଖାଲି);

	memset(&existing, 0, ଆକାର(existing));
	existing.use_cone_patterns = cfg->core_sparse_checkout_cone;

	ଯଦି (add_patterns_from_file_to_list(sparse_filename, "", 0,
					   &existing, ଖାଲି, 0))
		die(_("unable to load existing sparse-checkout patterns"));
	ମୁକ୍ତ(sparse_filename);

	ଯଦି (!existing.use_cone_patterns)
		die(_("existing sparse-checkout patterns do not use cone mode"));

	hashmap_for_each_entry(&existing.recursive_hashmap, &iter, pe, ent) {
		ଯଦି (!hashmap_contains_parent(&pl->recursive_hashmap,
					pe->pattern, &buffer) ||
		    !hashmap_contains_parent(&pl->parent_hashmap,
					pe->pattern, &buffer)) {
			strbuf_reset(&buffer);
			strbuf_addstr(&buffer, pe->pattern);
			ଭର୍ତ୍ତି_recursive_pattern(pl, &buffer);
		}
	}

	clear_pattern_list(&existing);
	strbuf_release(&buffer);
}

ସ୍ଥିର ଶୂନ୍ୟ add_patterns_literal(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv,
				 ସଂରଚନା pattern_list *pl,
				 ପୂର୍ଣ୍ଣସଂଖ୍ୟା use_stdin)
{
	ଅକ୍ଷର *sparse_filename = get_sparse_checkout_filename();
	ଯଦି (add_patterns_from_file_to_list(sparse_filename, "", 0,
					   pl, ଖାଲି, 0))
		die(_("unable to load existing sparse-checkout patterns"));
	ମୁକ୍ତ(sparse_filename);
	add_patterns_from_ନିବେଶ(pl, argc, argv, use_stdin ? stdin : ଖାଲି);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା modify_pattern_list(ସଂରଚନା repository *repo,
			       ସଂରଚନା strvec *args, ପୂର୍ଣ୍ଣସଂଖ୍ୟା use_stdin,
			       ଗଣନା modify_type m)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା result;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା changed_config = 0;
	ସଂରଚନା pattern_list *pl = xcalloc(1, ଆକାର(*pl));
	ସଂରଚନା repo_config_ମୂଲ୍ୟଗୁଡ଼ିକ *cfg = repo_config_ମୂଲ୍ୟଗୁଡ଼ିକ(the_repository);

	ବାଛ (m) {
	ସ୍ଥିତି ADD:
		ଯଦି (cfg->core_sparse_checkout_cone)
			add_patterns_cone_mode(args->nr, args->v, pl, use_stdin);
		ଅନ୍ୟଥା
			add_patterns_literal(args->nr, args->v, pl, use_stdin);
		ଭାଙ୍ଗ;

	ସ୍ଥିତି REPLACE:
		add_patterns_from_ନିବେଶ(pl, args->nr, args->v,
					use_stdin ? stdin : ଖାଲି);
		ଭାଙ୍ଗ;
	}

	ଯଦି (!cfg->apply_sparse_checkout) {
		set_config(repo, MODE_ALL_PATTERNS);
		cfg->apply_sparse_checkout = 1;
		changed_config = 1;
	}

	result = write_patterns_and_update(repo, pl);

	ଯଦି (result && changed_config)
		set_config(repo, MODE_NO_PATTERNS);

	clear_pattern_list(pl);
	ମୁକ୍ତ(pl);
	ଫେରନ୍ତୁ result;
}

ସ୍ଥିର ଶୂନ୍ୟ sanitize_paths(ସଂରଚନା repository *repo,
			   ସଂରଚନା strvec *args,
			   ଅଚଳ ଅକ୍ଷର *prefix, ପୂର୍ଣ୍ଣସଂଖ୍ୟା skip_checks)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	ସଂରଚନା repo_config_ମୂଲ୍ୟଗୁଡ଼ିକ *cfg = repo_config_ମୂଲ୍ୟଗୁଡ଼ିକ(the_repository);

	ଯଦି (!args->nr)
		ଫେରନ୍ତୁ;

	ଯଦି (prefix && *prefix && cfg->core_sparse_checkout_cone) {
		/*
		 * The args are not pathspecs, so unfortunately we
		 * cannot imitate how cmd_add() uses parse_pathspec().
		 */
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା prefix_len = strlen(prefix);

		ପାଇଁ (i = 0; i < args->nr; i++) {
			ଅକ୍ଷର *prefixed_path = prefix_path(the_repository, prefix,
							  prefix_len, args->v[i]);
			strvec_replace(args, i, prefixed_path);
			ମୁକ୍ତ(prefixed_path);
		}
	}

	ଯଦି (skip_checks)
		ଫେରନ୍ତୁ;

	ଯଦି (prefix && *prefix && !cfg->core_sparse_checkout_cone)
		die(_("please run from the toplevel directory in non-cone mode"));

	ଯଦି (cfg->core_sparse_checkout_cone) {
		ପାଇଁ (i = 0; i < args->nr; i++) {
			ଯଦି (args->v[i][0] == '/')
				die(_("specify directories rather than patterns (no leading slash)"));
			ଯଦି (args->v[i][0] == '!')
				die(_("specify directories rather than patterns.  If your directory starts with a '!', pass --skip-checks"));
			ଯଦି (strpbrk(args->v[i], "*?[]"))
				die(_("specify directories rather than patterns.  If your directory really has any of '*?[]\\' in it, pass --skip-checks"));
		}
	}

	ପାଇଁ (i = 0; i < args->nr; i++) {
		ସଂରଚନା cache_entry *ce;
		ସଂରଚନା index_state *index = repo->index;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା pos = index_name_pos(index, args->v[i], strlen(args->v[i]));

		ଯଦି (pos < 0)
			ଜାରି;
		ce = index->cache[pos];
		ଯଦି (S_ISSPARSEDIR(ce->ce_mode))
			ଜାରି;

		ଯଦି (cfg->core_sparse_checkout_cone)
			die(_("'%s' is not a directory; to treat it as a directory anyway, rerun with --skip-checks"), args->v[i]);
		ଅନ୍ୟଥା
			warning(_("pass a leading slash before paths such as '%s' if you want a single file (see NON-CONE PROBLEMS in the git-sparse-checkout manual)."), args->v[i]);
	}
}

ସ୍ଥିର ଅକ୍ଷର ଅଚଳ * ଅଚଳ builtin_sparse_checkout_add_usage[] = {
	N_("git sparse-checkout add [--skip-checks] (--stdin | <patterns>)"),
	ଖାଲି
};

ସ୍ଥିର ସଂରଚନା sparse_checkout_add_opts {
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା skip_checks;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା use_stdin;
} add_opts;

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା sparse_checkout_add(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
			       ସଂରଚନା repository *repo)
{
	ସ୍ଥିର ସଂରଚନା option builtin_sparse_checkout_add_options[] = {
		OPT_BOOL_F(0, "skip-checks", &add_opts.skip_checks,
			   N_("skip some sanity checks on the given paths that might give false positives"),
			   PARSE_OPT_NONEG),
		OPT_BOOL(0, "stdin", &add_opts.use_stdin,
			 N_("read patterns from standard in")),
		OPT_END(),
	};
	ସଂରଚନା strvec patterns = STRVEC_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
	ସଂରଚନା repo_config_ମୂଲ୍ୟଗୁଡ଼ିକ *cfg = repo_config_ମୂଲ୍ୟଗୁଡ଼ିକ(the_repository);

	setup_work_ବୃକ୍ଷ(the_repository);
	ଯଦି (!cfg->apply_sparse_checkout)
		die(_("no sparse-checkout to add to"));

	repo_read_index(repo);

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_add_options,
			     builtin_sparse_checkout_add_usage, 0);

	ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < argc; i++)
		strvec_ଠେଲ(&patterns, argv[i]);
	sanitize_paths(repo, &patterns, prefix, add_opts.skip_checks);

	ret = modify_pattern_list(repo, &patterns, add_opts.use_stdin, ADD);

	strvec_clear(&patterns);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ଅକ୍ଷର ଅଚଳ * ଅଚଳ builtin_sparse_checkout_set_usage[] = {
	N_("git sparse-checkout set [--[no-]cone] [--[no-]sparse-index] [--skip-checks] (--stdin | <patterns>)"),
	ଖାଲି
};

ସ୍ଥିର ସଂରଚନା sparse_checkout_set_opts {
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cone_mode;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା sparse_index;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା skip_checks;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା use_stdin;
} set_opts;

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା sparse_checkout_set(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
			       ସଂରଚନା repository *repo)
{
	ସଂରଚନା repo_config_ମୂଲ୍ୟଗୁଡ଼ିକ *cfg = repo_config_ମୂଲ୍ୟଗୁଡ଼ିକ(the_repository);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା default_patterns_nr = 2;
	ଅଚଳ ଅକ୍ଷର *default_patterns[] = {"/*", "!/*/", ଖାଲି};

	ସ୍ଥିର ସଂରଚନା option builtin_sparse_checkout_set_options[] = {
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
	ସଂରଚନା strvec patterns = STRVEC_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	setup_work_ବୃକ୍ଷ(the_repository);
	repo_read_index(repo);

	set_opts.cone_mode = -1;
	set_opts.sparse_index = -1;

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_set_options,
			     builtin_sparse_checkout_set_usage, 0);

	ଯଦି (update_modes(repo, &set_opts.cone_mode, &set_opts.sparse_index))
		ଫେରନ୍ତୁ 1;

	/*
	 * Cone mode automatically specifies the toplevel directory.  For
	 * non-cone mode, if nothing is specified, manually select just the
	 * top-level directory (much as 'init' would do).
	 */
	ଯଦି (!cfg->core_sparse_checkout_cone && !set_opts.use_stdin && argc == 0) {
		ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < default_patterns_nr; i++)
			strvec_ଠେଲ(&patterns, default_patterns[i]);
	} ଅନ୍ୟଥା {
		ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < argc; i++)
			strvec_ଠେଲ(&patterns, argv[i]);
		sanitize_paths(repo, &patterns, prefix, set_opts.skip_checks);
	}

	ret = modify_pattern_list(repo, &patterns, set_opts.use_stdin, REPLACE);

	strvec_clear(&patterns);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ଅକ୍ଷର ଅଚଳ * ଅଚଳ builtin_sparse_checkout_reapply_usage[] = {
	"git sparse-checkout reapply [--[no-]cone] [--[no-]sparse-index]",
	ଖାଲି
};

ସ୍ଥିର ସଂରଚନା sparse_checkout_reapply_opts {
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cone_mode;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା sparse_index;
} reapply_opts;

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା sparse_checkout_reapply(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv,
				   ଅଚଳ ଅକ୍ଷର *prefix,
				   ସଂରଚନା repository *repo)
{
	ସ୍ଥିର ସଂରଚନା option builtin_sparse_checkout_reapply_options[] = {
		OPT_BOOL(0, "cone", &reapply_opts.cone_mode,
			 N_("initialize the sparse-checkout in cone mode")),
		OPT_BOOL(0, "sparse-index", &reapply_opts.sparse_index,
			 N_("toggle the use of a sparse index")),
		OPT_END(),
	};
	ସଂରଚନା repo_config_ମୂଲ୍ୟଗୁଡ଼ିକ *cfg = repo_config_ମୂଲ୍ୟଗୁଡ଼ିକ(the_repository);

	setup_work_ବୃକ୍ଷ(the_repository);
	ଯଦି (!cfg->apply_sparse_checkout)
		die(_("must be in a sparse-checkout to reapply sparsity patterns"));

	reapply_opts.cone_mode = -1;
	reapply_opts.sparse_index = -1;

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_reapply_options,
			     builtin_sparse_checkout_reapply_usage, 0);

	repo_read_index(repo);

	ଯଦି (update_modes(repo, &reapply_opts.cone_mode, &reapply_opts.sparse_index))
		ଫେରନ୍ତୁ 1;

	ଫେରନ୍ତୁ update_working_directory(repo, ଖାଲି);
}

ସ୍ଥିର ଅକ୍ଷର ଅଚଳ * ଅଚଳ builtin_sparse_checkout_clean_usage[] = {
	"git sparse-checkout clean [-n|--dry-run]",
	ଖାଲି
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା list_file_iterator(ଅଚଳ ଅକ୍ଷର *path, ଅଚଳ ଶୂନ୍ୟ *data)
{
	ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ = data;

	printf(ସନ୍ଦେଶ, path);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ list_every_file_in_dir(ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ,
				   ଅଚଳ ଅକ୍ଷର *directory)
{
	ସଂରଚନା strbuf path = STRBUF_INIT;

	strbuf_addstr(&path, directory);
	for_each_file_in_dir(&path, list_file_iterator, ସନ୍ଦେଶ);
	strbuf_release(&path);
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ_remove = N_("Removing %s\n");
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ_would_remove = N_("Would remove %s\n");

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା sparse_checkout_clean(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv,
				   ଅଚଳ ଅକ୍ଷର *prefix,
				   ସଂରଚନା repository *repo)
{
	ସଂରଚନା strbuf full_path = STRBUF_INIT;
	ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ = ସନ୍ଦେଶ_remove;
	size_t worktree_len;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା force = 0, dry_run = 0, verbose = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା require_force = 1;
	ସଂରଚନା repo_config_ମୂଲ୍ୟଗୁଡ଼ିକ *cfg = repo_config_ମୂଲ୍ୟଗୁଡ଼ିକ(the_repository);

	ସଂରଚନା option builtin_sparse_checkout_clean_options[] = {
		OPT__DRY_RUN(&dry_run, N_("dry run")),
		OPT__FORCE(&force, N_("force"), PARSE_OPT_NOCOMPLETE),
		OPT__VERBOSE(&verbose, N_("report each affected file, not just directories")),
		OPT_END(),
	};

	setup_work_ବୃକ୍ଷ(the_repository);
	ଯଦି (!cfg->apply_sparse_checkout)
		die(_("must be in a sparse-checkout to clean directories"));
	ଯଦି (!cfg->core_sparse_checkout_cone)
		die(_("must be in a cone-mode sparse-checkout to clean directories"));

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_clean_options,
			     builtin_sparse_checkout_clean_usage, 0);

	repo_config_get_bool(repo, "clean.requireforce", &require_force);
	ଯଦି (require_force && !force && !dry_run)
		die(_("for safety, refusing to clean without one of --force or --dry-run"));

	ଯଦି (dry_run)
		ସନ୍ଦେଶ = ସନ୍ଦେଶ_would_remove;

	ଯଦି (repo_read_index(repo) < 0)
		die(_("failed to read index"));

	ଯଦି (convert_to_sparse(repo->index, SPARSE_INDEX_MEMORY_ONLY) ||
	    repo->index->sparse_index == INDEX_EXPANDED)
		die(_("failed to convert index to a sparse index; resolve merge conflicts and try again"));

	strbuf_addstr(&full_path, repo->worktree);
	strbuf_addch(&full_path, '/');
	worktree_len = full_path.len;

	ପାଇଁ (size_t i = 0; i < repo->index->cache_nr; i++) {
		ସଂରଚନା cache_entry *ce = repo->index->cache[i];
		ଯଦି (!S_ISSPARSEDIR(ce->ce_mode))
			ଜାରି;
		strbuf_setlen(&full_path, worktree_len);
		strbuf_add(&full_path, ce->name, ce->ce_namelen);

		ଯଦି (!is_directory(full_path.buf))
			ଜାରି;

		ଯଦି (verbose)
			list_every_file_in_dir(ସନ୍ଦେଶ, ce->name);
		ଅନ୍ୟଥା
			printf(ସନ୍ଦେଶ, ce->name);

		ଯଦି (dry_run <= 0 &&
		    remove_dir_recursively(&full_path, 0))
			warning_errno(_("failed to remove '%s'"), ce->name);
	}

	strbuf_release(&full_path);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଅକ୍ଷର ଅଚଳ * ଅଚଳ builtin_sparse_checkout_disable_usage[] = {
	"git sparse-checkout disable",
	ଖାଲି
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା sparse_checkout_disable(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv,
				   ଅଚଳ ଅକ୍ଷର *prefix,
				   ସଂରଚନା repository *repo)
{
	ସ୍ଥିର ସଂରଚନା option builtin_sparse_checkout_disable_options[] = {
		OPT_END(),
	};
	ସଂରଚନା pattern_list pl;
	ସଂରଚନା repo_config_ମୂଲ୍ୟଗୁଡ଼ିକ *cfg = repo_config_ମୂଲ୍ୟଗୁଡ଼ିକ(the_repository);

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

	setup_work_ବୃକ୍ଷ(the_repository);
	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_disable_options,
			     builtin_sparse_checkout_disable_usage, 0);

	/*
	 * Disable the advice message for expanding a sparse index, as we
	 * are expecting to do that when disabling sparse-checkout.
	 */
	give_advice_on_expansion = 0;
	repo_read_index(repo);

	memset(&pl, 0, ଆକାର(pl));
	hashmap_init(&pl.recursive_hashmap, pl_hashmap_cmp, ଖାଲି, 0);
	hashmap_init(&pl.parent_hashmap, pl_hashmap_cmp, ଖାଲି, 0);
	pl.use_cone_patterns = 0;
	cfg->apply_sparse_checkout = 1;

	add_pattern("/*", empty_base, 0, &pl, 0);

	prepare_repo_settings(the_repository);
	repo->settings.sparse_index = 0;

	ଯଦି (update_working_directory(repo, &pl))
		die(_("error while refreshing working directory"));

	clear_pattern_list(&pl);
	ଫେରନ୍ତୁ set_config(repo, MODE_NO_PATTERNS);
}

ସ୍ଥିର ଅକ୍ଷର ଅଚଳ * ଅଚଳ builtin_sparse_checkout_check_rules_usage[] = {
	N_("git sparse-checkout check-rules [-z] [--skip-checks]"
	   "[--[no-]cone] [--rules-file <file>]"),
	ଖାଲି
};

ସ୍ଥିର ସଂରଚନା sparse_checkout_check_rules_opts {
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cone_mode;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା null_termination;
	ଅକ୍ଷର *rules_file;
} check_rules_opts;

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା check_rules(ସଂରଚନା repository *repo,
		       ସଂରଚନା pattern_list *pl,
		       ପୂର୍ଣ୍ଣସଂଖ୍ୟା null_terminated)
{
	ସଂରଚନା strbuf ଧାଡ଼ି = STRBUF_INIT;
	ସଂରଚନା strbuf unquoted = STRBUF_INIT;
	ଅକ୍ଷର *path;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଧାଡ଼ି_terminator = null_terminated ? 0 : '\n';
	strbuf_getline_fn getline_fn = null_terminated ? strbuf_getline_nul
		: strbuf_getline;
	repo->index->sparse_checkout_patterns = pl;
	ଯେତେବେଳେ (!getline_fn(&ଧାଡ଼ି, stdin)) {
		path = ଧାଡ଼ି.buf;
		ଯଦି (!null_terminated && ଧାଡ଼ି.buf[0] == '"') {
			strbuf_reset(&unquoted);
			ଯଦି (unquote_c_style(&unquoted, ଧାଡ଼ି.buf, ଖାଲି))
				die(_("unable to unquote C-style string '%s'"),
					ଧାଡ଼ି.buf);

			path = unquoted.buf;
		}

		ଯଦି (path_in_sparse_checkout(path, repo->index))
			write_name_quoted(path, stdout, ଧାଡ଼ି_terminator);
	}
	strbuf_release(&ଧାଡ଼ି);
	strbuf_release(&unquoted);

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା sparse_checkout_check_rules(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
				       ସଂରଚନା repository *repo)
{
	ସ୍ଥିର ସଂରଚନା option builtin_sparse_checkout_check_rules_options[] = {
		OPT_BOOL('z', ଖାଲି, &check_rules_opts.null_termination,
			 N_("terminate input and output files by a NUL character")),
		OPT_BOOL(0, "cone", &check_rules_opts.cone_mode,
			 N_("when used with --rules-file interpret patterns as cone mode patterns")),
		OPT_FILENAME(0, "rules-file", &check_rules_opts.rules_file,
			 N_("use patterns in <file> instead of the current ones.")),
		OPT_END(),
	};

	FILE *fp;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
	ସଂରଚନା pattern_list pl = {0};
	ସଂରଚନା repo_config_ମୂଲ୍ୟଗୁଡ଼ିକ *cfg = repo_config_ମୂଲ୍ୟଗୁଡ଼ିକ(the_repository);
	ଅକ୍ଷର *sparse_filename;
	check_rules_opts.cone_mode = -1;

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_check_rules_options,
			     builtin_sparse_checkout_check_rules_usage, 0);

	ଯଦି (check_rules_opts.rules_file && check_rules_opts.cone_mode < 0)
		check_rules_opts.cone_mode = 1;

	update_cone_mode(&check_rules_opts.cone_mode);
	pl.use_cone_patterns = cfg->core_sparse_checkout_cone;
	ଯଦି (check_rules_opts.rules_file) {
		fp = xfopen(check_rules_opts.rules_file, "r");
		add_patterns_from_ନିବେଶ(&pl, argc, argv, fp);
		fclose(fp);
	} ଅନ୍ୟଥା {
		sparse_filename = get_sparse_checkout_filename();
		ଯଦି (add_patterns_from_file_to_list(sparse_filename, "", 0, &pl,
						   ଖାଲି, 0))
			die(_("unable to load existing sparse-checkout patterns"));
		ମୁକ୍ତ(sparse_filename);
	}

	ret = check_rules(repo, &pl, check_rules_opts.null_termination);
	clear_pattern_list(&pl);
	ମୁକ୍ତ(check_rules_opts.rules_file);
	ଫେରନ୍ତୁ ret;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_sparse_checkout(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
			ଅଚଳ ଅକ୍ଷର **argv,
			ଅଚଳ ଅକ୍ଷର *prefix,
			ସଂରଚନା repository *repo)
{
	parse_opt_subcommand_fn *fn = ଖାଲି;
	ସଂରଚନା option builtin_sparse_checkout_options[] = {
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

	repo_config(the_repository, git_default_config, ଖାଲି);

	prepare_repo_settings(repo);
	repo->settings.command_requires_full_index = 0;

	ଫେରନ୍ତୁ fn(argc, argv, prefix, repo);
}
