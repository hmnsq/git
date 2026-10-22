#وصف_ڪريو USE_THE_REPOSITORY_VARIABLE
#وصف_ڪريو DISABLE_SIGN_COMPARE_WARNINGS

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

ساكن ثابت حرف *خالي_جايون_بنياد = "";

ساكن حرف ثابت * ثابت builtin_sparse_checkout_usage[] = {
	N_("git sparse-checkout (init | list | set | add | reapply | disable | check-rules | clean) [<options>]"),
	عدم
};

ساكن فراغ write_patterns_منزل_file(FILE *fp, بنية نمونو_list *pl)
{
	صحيح i;

	لكل (i = 0; i < pl->nr; i++) {
		بنية رستو_نمونو *p = pl->patterns[i];

		إذا (p->flags & نمونو_FLAG_NEGATIVE)
			fprintf(fp, "!");

		fprintf(fp, "%s", p->نمونو);

		إذا (p->flags & نمونو_FLAG_MUSTBEDIR)
			fprintf(fp, "/");

		fprintf(fp, "\n");
	}
}

ساكن حرف ثابت * ثابت builtin_sparse_checkout_list_usage[] = {
	"git sparse-checkout list",
	عدم
};

ساكن صحيح sparse_checkout_list(صحيح argc, ثابت حرف **argv, ثابت حرف *اڳياڙي,
				بنية repository *repo اڻ_استعمال_ٿيل)
{
	ساكن بنية option builtin_sparse_checkout_list_options[] = {
		OPT_پڇاڙي(),
	};
	بنية نمونو_list pl;
	حرف *sparse_filename;
	صحيح res;
	بنية repo_config_قدرون *cfg = repo_config_قدرون(the_repository);

	تياري_work_وڻ(the_repository);
	إذا (!cfg->apply_sparse_checkout)
		die(_("this worktree is not sparse"));

	argc = parse_options(argc, argv, اڳياڙي,
			     builtin_sparse_checkout_list_options,
			     builtin_sparse_checkout_list_usage, 0);

	memset(&pl, 0, حجم(pl));

	pl.use_cone_patterns = cfg->core_sparse_checkout_cone;

	sparse_filename = get_sparse_checkout_filename();
	res = جوڙيو_patterns_from_file_منزل_list(sparse_filename, "", 0, &pl, عدم, 0);
	حرر(sparse_filename);

	إذا (res < 0) {
		warning(_("this worktree is not sparse (sparse-checkout file may not exist)"));
		أرجع 0;
	}

	إذا (pl.use_cone_patterns) {
		صحيح i;
		بنية نمونو_داخلا *pe;
		بنية hashmap_iter iter;
		بنية string_list sl = STRING_LIST_INIT_DUP;

		hashmap_for_each_داخلا(&pl.recursive_hashmap, &iter, pe, ent) {
			/* pe->pattern starts with "/", skip it */
			string_list_append(&sl, pe->نمونو + 1);
		}

		string_list_ترتيب_ڏيو_u(&sl, 0);

		لكل (i = 0; i < sl.nr; i++) {
			quote_c_style(sl.شيون[i].string, عدم, stdout, 0);
			لکو("\n");
		}

		string_list_clear(&sl, 0);
	} وإلا {
		write_patterns_منزل_file(stdout, &pl);
	}

	clear_نمونو_list(&pl);

	أرجع 0;
}

ساكن فراغ clean_tracked_sparse_directories(بنية repository *r)
{
	صحيح i, was_ڀريل = 0;
	بنية strbuf رستو = STRBUF_INIT;
	سائيز_t pathlen;
	بنية string_list_شي *شي;
	بنية string_list sparse_dirs = STRING_LIST_INIT_DUP;

	/*
	 * If we are not using cone mode patterns, then we cannot
	 * delete directories outside of the sparse cone.
	 */
	إذا (!r || !r->اشاريو || !r->worktree)
		أرجع;
	إذا (init_sparse_checkout_patterns(r->اشاريو) ||
	    !r->اشاريو->sparse_checkout_patterns->use_cone_patterns)
		أرجع;

	/*
	 * Use the sparse index as a data structure to assist finding
	 * directories that are safe to delete. This conversion to a
	 * sparse index will not delete directories that contain
	 * conflicted entries or submodules.
	 */
	إذا (r->اشاريو->sparse_اشاريو == اشاريو_EXPANDED) {
		/*
		 * If something, such as a merge conflict or other concern,
		 * prevents us from converting to a sparse index, then do
		 * not try deleting files.
		 */
		إذا (convert_منزل_sparse(r->اشاريو, SPARSE_اشاريو_يادگيري_ONLY))
			أرجع;
		was_ڀريل = 1;
	}

	strbuf_addstr(&رستو, r->worktree);
	strbuf_complete(&رستو, '/');
	pathlen = رستو.len;

	/*
	 * Collect directories that have gone out of scope but also
	 * exist on disk, so there is some work to be done. We need to
	 * store the entries in a list before exploring, since that might
	 * expand the sparse-index again.
	 */
	لكل (i = 0; i < r->اشاريو->cache_nr; i++) {
		بنية cache_داخلا *ce = r->اشاريو->cache[i];

		إذا (S_ISSPARSEDIR(ce->ce_گهڻائي) &&
		    repo_file_exists(r, ce->name))
			string_list_append(&sparse_dirs, ce->name);
	}

	for_each_string_list_شي(شي, &sparse_dirs) {
		بنية dir_struct dir = DIR_INIT;
		بنية pathspec p = { 0 };
		بنية strvec s = STRVEC_INIT;

		strbuf_setlen(&رستو, pathlen);
		strbuf_addstr(&رستو, شي->string);

		dir.flags |= DIR_SHOW_IGNORED_TOO;

		تياري_standard_excludes(&dir);
		strvec_ڌڪو(&s, رستو.buf);

		parse_pathspec(&p, PATHSPEC_GLOB, 0, عدم, s.v);
		fill_directory(&dir, r->اشاريو, &p);

		إذا (dir.nr) {
			warning(_("directory '%s' contains untracked files,"
				  " but is not in the sparse-checkout cone"),
				شي->string);
		} وإلا إذا (remove_dir_recursively(&رستو, 0)) {
			/*
			 * Removal is "best effort". If something blocks
			 * the deletion, then continue with a warning.
			 */
			warning(_("failed to remove directory '%s'"),
				شي->string);
		}

		strvec_clear(&s);
		clear_pathspec(&p);
		dir_clear(&dir);
	}

	string_list_clear(&sparse_dirs, 0);
	strbuf_release(&رستو);

	إذا (was_ڀريل)
		ensure_ڀريل_اشاريو(r->اشاريو);
}

ساكن صحيح update_working_directory(بنية repository *r,
				    بنية نمونو_list *pl)
{
	تعداد update_sparsity_نتيجو نتيجو;
	بنية unpack_trees_options o;
	بنية تالو_file تالو_file = تالو_INIT;
	بنية نمونو_list *old_pl;

	/* If no branch has been checked out, there are no updates to make. */
	إذا (is_اشاريو_unborn(r->اشاريو))
		أرجع UPDATE_SPARSITY_SUCCESS;

	old_pl = r->اشاريو->sparse_checkout_patterns;
	r->اشاريو->sparse_checkout_patterns = pl;

	memset(&o, 0, حجم(o));
	o.verbose_update = isatty(2);
	o.update = 1;
	o.سر_idx = -1;
	o.src_اشاريو = r->اشاريو;
	o.dst_اشاريو = r->اشاريو;
	o.skip_sparse_checkout = 0;

	تياري_work_وڻ(the_repository);

	repo_hold_locked_اشاريو(r, &تالو_file, تالو_DIE_ON_ERROR);

	تياري_unpack_trees_porcelain(&o, "sparse-checkout");
	نتيجو = update_sparsity(&o, pl);
	clear_unpack_trees_porcelain(&o);

	إذا (نتيجو == UPDATE_SPARSITY_WARNINGS)
		/*
		 * We don't do any special handling of warnings from untracked
		 * files in the way or dirty entries that can't be removed.
		 */
		نتيجو = UPDATE_SPARSITY_SUCCESS;
	إذا (نتيجو == UPDATE_SPARSITY_SUCCESS)
		write_locked_اشاريو(r->اشاريو, &تالو_file, COMMIT_تالو);
	وإلا
		rollback_تالو_file(&تالو_file);

	clean_tracked_sparse_directories(r);

	إذا (r->اشاريو->sparse_checkout_patterns != pl) {
		clear_نمونو_list(r->اشاريو->sparse_checkout_patterns);
		FREE_AND_NULL(r->اشاريو->sparse_checkout_patterns);
	}
	r->اشاريو->sparse_checkout_patterns = old_pl;

	أرجع نتيجو;
}

ساكن حرف *escaped_نمونو(حرف *نمونو)
{
	حرف *p = نمونو;
	بنية strbuf final = STRBUF_INIT;

	طالما (*p) {
		إذا (is_glob_special(*p))
			strbuf_addch(&final, '\\');

		strbuf_addch(&final, *p);
		p++;
	}

	أرجع strbuf_detach(&final, عدم);
}

ساكن فراغ write_cone_منزل_file(FILE *fp, بنية نمونو_list *pl)
{
	صحيح i;
	بنية نمونو_داخلا *pe;
	بنية hashmap_iter iter;
	بنية string_list sl = STRING_LIST_INIT_DUP;
	بنية strbuf والد_نمونو = STRBUF_INIT;

	hashmap_for_each_داخلا(&pl->والد_hashmap, &iter, pe, ent) {
		إذا (hashmap_get_داخلا(&pl->recursive_hashmap, pe, ent, عدم))
			استمر;

		إذا (!hashmap_شامل_آهي_والد(&pl->recursive_hashmap,
					     pe->نمونو,
					     &والد_نمونو))
			string_list_append(&sl, pe->نمونو);
	}

	string_list_ترتيب_ڏيو_u(&sl, 0);

	fprintf(fp, "/*\n!/*/\n");

	لكل (i = 0; i < sl.nr; i++) {
		حرف *نمونو = escaped_نمونو(sl.شيون[i].string);

		إذا (لڙي_ڊيگهه(نمونو))
			fprintf(fp, "%s/\n!%s/*/\n", نمونو, نمونو);
		حرر(نمونو);
	}

	string_list_clear(&sl, 0);

	hashmap_for_each_داخلا(&pl->recursive_hashmap, &iter, pe, ent) {
		إذا (!hashmap_شامل_آهي_والد(&pl->recursive_hashmap,
					     pe->نمونو,
					     &والد_نمونو))
			string_list_append(&sl, pe->نمونو);
	}

	strbuf_release(&والد_نمونو);

	string_list_ترتيب_ڏيو_u(&sl, 0);

	لكل (i = 0; i < sl.nr; i++) {
		حرف *نمونو = escaped_نمونو(sl.شيون[i].string);
		fprintf(fp, "%s/\n", نمونو);
		حرر(نمونو);
	}

	string_list_clear(&sl, 0);
}

ساكن صحيح write_patterns_and_update(بنية repository *repo,
				     بنية نمونو_list *pl)
{
	حرف *sparse_filename;
	FILE *fp;
	بنية تالو_file lk = تالو_INIT;
	صحيح نتيجو;
	بنية repo_config_قدرون *cfg = repo_config_قدرون(the_repository);

	sparse_filename = get_sparse_checkout_filename();

	إذا (محفوظ_create_leading_directories(repo, sparse_filename))
		die(_("failed to create directory for sparse-checkout file"));

	repo_hold_تالو_file_for_update(repo, &lk, sparse_filename,
				       تالو_DIE_ON_ERROR);

	نتيجو = update_working_directory(repo, pl);
	إذا (نتيجو) {
		rollback_تالو_file(&lk);
		update_working_directory(repo, عدم);
		اذهب ٻاهريون;
	}

	fp = fdopen_تالو_file(&lk, "w");
	إذا (!fp)
		die_errno(_("unable to fdopen %s"), get_تالو_file_رستو(&lk));

	إذا (cfg->core_sparse_checkout_cone)
		write_cone_منزل_file(fp, pl);
	وإلا
		write_patterns_منزل_file(fp, pl);

	إذا (commit_تالو_file(&lk))
		die_errno(_("unable to write %s"), sparse_filename);

ٻاهريون:
	clear_نمونو_list(pl);
	حرر(sparse_filename);
	أرجع نتيجو;
}

تعداد sparse_checkout_گهڻائي {
	گهڻائي_NO_PATTERNS = 0,
	گهڻائي_ALL_PATTERNS = 1,
	گهڻائي_CONE_PATTERNS = 2,
};

ساكن صحيح سيٽ_config(بنية repository *repo,
		      تعداد sparse_checkout_گهڻائي گهڻائي)
{
	/* Update to use worktree config, if not already. */
	إذا (init_worktree_config(repo)) {
		error(_("failed to initialize worktree config"));
		أرجع 1;
	}

	إذا (repo_config_سيٽ_worktree_gently(repo,
					    "core.sparseCheckout",
					    گهڻائي ? "true" : "false") ||
	    repo_config_سيٽ_worktree_gently(repo,
					    "core.sparseCheckoutCone",
					    گهڻائي == گهڻائي_CONE_PATTERNS ?
						"true" : "false"))
		أرجع 1;

	إذا (گهڻائي == گهڻائي_NO_PATTERNS)
		أرجع سيٽ_sparse_اشاريو_config(repo, 0);

	أرجع 0;
}

ساكن تعداد sparse_checkout_گهڻائي update_cone_گهڻائي(صحيح *cone_گهڻائي) {
	بنية repo_config_قدرون *cfg = repo_config_قدرون(the_repository);

	/* If not specified, use previous definition of cone mode */
	إذا (*cone_گهڻائي == -1 && cfg->apply_sparse_checkout)
		*cone_گهڻائي = cfg->core_sparse_checkout_cone;

	/* Set cone/non-cone mode appropriately */
	cfg->apply_sparse_checkout = 1;
	إذا (*cone_گهڻائي == 1 || *cone_گهڻائي == -1) {
		cfg->core_sparse_checkout_cone = 1;
		أرجع گهڻائي_CONE_PATTERNS;
	}
	cfg->core_sparse_checkout_cone = 0;
	أرجع گهڻائي_ALL_PATTERNS;
}

ساكن صحيح update_modes(بنية repository *repo, صحيح *cone_گهڻائي, صحيح *sparse_اشاريو)
{
	صحيح گهڻائي, رڪارڊ_گهڻائي;
	بنية repo_config_قدرون *cfg = repo_config_قدرون(the_repository);

	/* Determine if we need to record the mode; ensure sparse checkout on */
	رڪارڊ_گهڻائي = (*cone_گهڻائي != -1) || !cfg->apply_sparse_checkout;

	گهڻائي = update_cone_گهڻائي(cone_گهڻائي);
	إذا (رڪارڊ_گهڻائي && سيٽ_config(repo, گهڻائي))
		أرجع 1;

	/* Set sparse-index/non-sparse-index mode if specified */
	إذا (*sparse_اشاريو >= 0) {
		إذا (سيٽ_sparse_اشاريو_config(repo, *sparse_اشاريو) < 0)
			die(_("failed to modify sparse-index config"));

		/* force an index rewrite */
		repo_read_اشاريو(repo);
		repo->اشاريو->updated_workdir = 1;

		إذا (!*sparse_اشاريو)
			ensure_ڀريل_اشاريو(repo->اشاريو);
	}

	أرجع 0;
}

ساكن حرف ثابت * ثابت builtin_sparse_checkout_init_usage[] = {
	"git sparse-checkout init [--cone] [--[no-]sparse-index]",
	عدم
};

ساكن بنية sparse_checkout_init_opts {
	صحيح cone_گهڻائي;
	صحيح sparse_اشاريو;
} init_opts;

ساكن صحيح sparse_checkout_init(صحيح argc, ثابت حرف **argv, ثابت حرف *اڳياڙي,
				بنية repository *repo)
{
	بنية نمونو_list pl;
	حرف *sparse_filename;
	صحيح res;
	بنية object_id oid;

	ساكن بنية option builtin_sparse_checkout_init_options[] = {
		OPT_BOOL(0, "cone", &init_opts.cone_گهڻائي,
			 N_("initialize the sparse-checkout in cone mode")),
		OPT_BOOL(0, "sparse-index", &init_opts.sparse_اشاريو,
			 N_("toggle the use of a sparse index")),
		OPT_پڇاڙي(),
	};

	تياري_work_وڻ(the_repository);
	repo_read_اشاريو(repo);

	init_opts.cone_گهڻائي = -1;
	init_opts.sparse_اشاريو = -1;

	argc = parse_options(argc, argv, اڳياڙي,
			     builtin_sparse_checkout_init_options,
			     builtin_sparse_checkout_init_usage, 0);

	إذا (update_modes(repo, &init_opts.cone_گهڻائي, &init_opts.sparse_اشاريو))
		أرجع 1;

	memset(&pl, 0, حجم(pl));

	sparse_filename = get_sparse_checkout_filename();
	res = جوڙيو_patterns_from_file_منزل_list(sparse_filename, "", 0, &pl, عدم, 0);

	/* If we already have a sparse-checkout file, use it. */
	إذا (res >= 0) {
		حرر(sparse_filename);
		clear_نمونو_list(&pl);
		أرجع update_working_directory(repo, عدم);
	}

	إذا (repo_get_oid(repo, "HEAD", &oid)) {
		FILE *fp;

		/* assume we are in a fresh repo, but update the sparse-checkout file */
		إذا (محفوظ_create_leading_directories(repo, sparse_filename))
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

	جوڙيو_نمونو("/*", خالي_جايون_بنياد, 0, &pl, 0);
	جوڙيو_نمونو("!/*/", خالي_جايون_بنياد, 0, &pl, 0);
	pl.use_cone_patterns = init_opts.cone_گهڻائي;

	أرجع write_patterns_and_update(repo, &pl);
}

ساكن فراغ وجهو_recursive_نمونو(بنية نمونو_list *pl, بنية strbuf *رستو)
{
	بنية نمونو_داخلا *e = xmalloc(حجم(*e));
	e->patternlen = رستو->len;
	e->نمونو = strbuf_detach(رستو, عدم);
	hashmap_داخلا_init(&e->ent, fspathhash(e->نمونو));

	hashmap_جوڙيو(&pl->recursive_hashmap, &e->ent);

	طالما (e->patternlen) {
		حرف *slash = strrchr(e->نمونو, '/');
		حرف *oldpattern = e->نمونو;
		سائيز_t newlen;
		بنية نمونو_داخلا *dup;

		إذا (!slash || slash == e->نمونو)
			اكسر;

		newlen = slash - e->نمونو;
		e = xmalloc(حجم(بنية نمونو_داخلا));
		e->patternlen = newlen;
		e->نمونو = xstrndup(oldpattern, newlen);
		hashmap_داخلا_init(&e->ent, fspathhash(e->نمونو));

		dup = hashmap_get_داخلا(&pl->والد_hashmap, e, ent, عدم);
		إذا (!dup) {
			hashmap_جوڙيو(&pl->والد_hashmap, &e->ent);
		} وإلا {
			حرر(e->نمونو);
			حرر(e);
			e = dup;
		}
	}
}

ساكن فراغ strbuf_منزل_cone_نمونو(بنية strbuf *سٽ, بنية نمونو_list *pl)
{
	strbuf_trim(سٽ);

	strbuf_trim_trailing_dir_sep(سٽ);

	إذا (strbuf_normalize_رستو(سٽ))
		die(_("could not normalize path %s"), سٽ->buf);

	إذا (!سٽ->len)
		أرجع;

	إذا (سٽ->buf[0] != '/')
		strbuf_insertstr(سٽ, 0, "/");

	وجهو_recursive_نمونو(pl, سٽ);
}

ساكن فراغ جوڙيو_patterns_from_داخلا(بنية نمونو_list *pl,
				    صحيح argc, ثابت حرف **argv,
				    FILE *file)
{
	صحيح i;
	بنية repo_config_قدرون *cfg = repo_config_قدرون(the_repository);

	إذا (cfg->core_sparse_checkout_cone) {
		بنية strbuf سٽ = STRBUF_INIT;

		hashmap_init(&pl->recursive_hashmap, pl_hashmap_cmp, عدم, 0);
		hashmap_init(&pl->والد_hashmap, pl_hashmap_cmp, عدم, 0);
		pl->use_cone_patterns = 1;

		إذا (file) {
			بنية strbuf unquoted = STRBUF_INIT;
			طالما (!strbuf_getline(&سٽ, file)) {
				إذا (سٽ.buf[0] == '"') {
					strbuf_reset(&unquoted);
					إذا (unquote_c_style(&unquoted, سٽ.buf, عدم))
						die(_("unable to unquote C-style string '%s'"),
						سٽ.buf);

					strbuf_مٽا_سٽا(&unquoted, &سٽ);
				}

				strbuf_منزل_cone_نمونو(&سٽ, pl);
			}

			strbuf_release(&unquoted);
		} وإلا {
			لكل (i = 0; i < argc; i++) {
				strbuf_setlen(&سٽ, 0);
				strbuf_addstr(&سٽ, argv[i]);
				strbuf_منزل_cone_نمونو(&سٽ, pl);
			}
		}
		strbuf_release(&سٽ);
	} وإلا {
		إذا (file) {
			بنية strbuf سٽ = STRBUF_INIT;

			طالما (!strbuf_getline(&سٽ, file))
				جوڙيو_نمونو(سٽ.buf, خالي_جايون_بنياد, 0, pl, 0);

			strbuf_release(&سٽ);
		} وإلا {
			لكل (i = 0; i < argc; i++)
				جوڙيو_نمونو(argv[i], خالي_جايون_بنياد, 0, pl, 0);
		}
	}
}

تعداد modify_type {
	REPLACE,
	جوڙيو_2,
};

ساكن فراغ جوڙيو_patterns_cone_گهڻائي(صحيح argc, ثابت حرف **argv,
				   بنية نمونو_list *pl,
				   صحيح use_stdin)
{
	بنية strbuf بفر = STRBUF_INIT;
	بنية نمونو_داخلا *pe;
	بنية hashmap_iter iter;
	بنية نمونو_list existing;
	بنية repo_config_قدرون *cfg = repo_config_قدرون(the_repository);
	حرف *sparse_filename = get_sparse_checkout_filename();

	جوڙيو_patterns_from_داخلا(pl, argc, argv,
				use_stdin ? stdin : عدم);

	memset(&existing, 0, حجم(existing));
	existing.use_cone_patterns = cfg->core_sparse_checkout_cone;

	إذا (جوڙيو_patterns_from_file_منزل_list(sparse_filename, "", 0,
					   &existing, عدم, 0))
		die(_("unable to load existing sparse-checkout patterns"));
	حرر(sparse_filename);

	إذا (!existing.use_cone_patterns)
		die(_("existing sparse-checkout patterns do not use cone mode"));

	hashmap_for_each_داخلا(&existing.recursive_hashmap, &iter, pe, ent) {
		إذا (!hashmap_شامل_آهي_والد(&pl->recursive_hashmap,
					pe->نمونو, &بفر) ||
		    !hashmap_شامل_آهي_والد(&pl->والد_hashmap,
					pe->نمونو, &بفر)) {
			strbuf_reset(&بفر);
			strbuf_addstr(&بفر, pe->نمونو);
			وجهو_recursive_نمونو(pl, &بفر);
		}
	}

	clear_نمونو_list(&existing);
	strbuf_release(&بفر);
}

ساكن فراغ جوڙيو_patterns_literal(صحيح argc, ثابت حرف **argv,
				 بنية نمونو_list *pl,
				 صحيح use_stdin)
{
	حرف *sparse_filename = get_sparse_checkout_filename();
	إذا (جوڙيو_patterns_from_file_منزل_list(sparse_filename, "", 0,
					   pl, عدم, 0))
		die(_("unable to load existing sparse-checkout patterns"));
	حرر(sparse_filename);
	جوڙيو_patterns_from_داخلا(pl, argc, argv, use_stdin ? stdin : عدم);
}

ساكن صحيح modify_نمونو_list(بنية repository *repo,
			       بنية strvec *دليلون, صحيح use_stdin,
			       تعداد modify_type m)
{
	صحيح نتيجو;
	صحيح بدليو_config = 0;
	بنية نمونو_list *pl = xcalloc(1, حجم(*pl));
	بنية repo_config_قدرون *cfg = repo_config_قدرون(the_repository);

	بدل (m) {
	حالة جوڙيو_2:
		إذا (cfg->core_sparse_checkout_cone)
			جوڙيو_patterns_cone_گهڻائي(دليلون->nr, دليلون->v, pl, use_stdin);
		وإلا
			جوڙيو_patterns_literal(دليلون->nr, دليلون->v, pl, use_stdin);
		اكسر;

	حالة REPLACE:
		جوڙيو_patterns_from_داخلا(pl, دليلون->nr, دليلون->v,
					use_stdin ? stdin : عدم);
		اكسر;
	}

	إذا (!cfg->apply_sparse_checkout) {
		سيٽ_config(repo, گهڻائي_ALL_PATTERNS);
		cfg->apply_sparse_checkout = 1;
		بدليو_config = 1;
	}

	نتيجو = write_patterns_and_update(repo, pl);

	إذا (نتيجو && بدليو_config)
		سيٽ_config(repo, گهڻائي_NO_PATTERNS);

	clear_نمونو_list(pl);
	حرر(pl);
	أرجع نتيجو;
}

ساكن فراغ sanitize_paths(بنية repository *repo,
			   بنية strvec *دليلون,
			   ثابت حرف *اڳياڙي, صحيح skip_checks)
{
	صحيح i;
	بنية repo_config_قدرون *cfg = repo_config_قدرون(the_repository);

	إذا (!دليلون->nr)
		أرجع;

	إذا (اڳياڙي && *اڳياڙي && cfg->core_sparse_checkout_cone) {
		/*
		 * The args are not pathspecs, so unfortunately we
		 * cannot imitate how cmd_add() uses parse_pathspec().
		 */
		صحيح اڳياڙي_len = لڙي_ڊيگهه(اڳياڙي);

		لكل (i = 0; i < دليلون->nr; i++) {
			حرف *prefixed_رستو = اڳياڙي_رستو(the_repository, اڳياڙي,
							  اڳياڙي_len, دليلون->v[i]);
			strvec_replace(دليلون, i, prefixed_رستو);
			حرر(prefixed_رستو);
		}
	}

	إذا (skip_checks)
		أرجع;

	إذا (اڳياڙي && *اڳياڙي && !cfg->core_sparse_checkout_cone)
		die(_("please run from the toplevel directory in non-cone mode"));

	إذا (cfg->core_sparse_checkout_cone) {
		لكل (i = 0; i < دليلون->nr; i++) {
			إذا (دليلون->v[i][0] == '/')
				die(_("specify directories rather than patterns (no leading slash)"));
			إذا (دليلون->v[i][0] == '!')
				die(_("specify directories rather than patterns.  If your directory starts with a '!', pass --skip-checks"));
			إذا (strpbrk(دليلون->v[i], "*?[]"))
				die(_("specify directories rather than patterns.  If your directory really has any of '*?[]\\' in it, pass --skip-checks"));
		}
	}

	لكل (i = 0; i < دليلون->nr; i++) {
		بنية cache_داخلا *ce;
		بنية اشاريو_state *اشاريو = repo->اشاريو;
		صحيح pos = اشاريو_name_pos(اشاريو, دليلون->v[i], لڙي_ڊيگهه(دليلون->v[i]));

		إذا (pos < 0)
			استمر;
		ce = اشاريو->cache[pos];
		إذا (S_ISSPARSEDIR(ce->ce_گهڻائي))
			استمر;

		إذا (cfg->core_sparse_checkout_cone)
			die(_("'%s' is not a directory; to treat it as a directory anyway, rerun with --skip-checks"), دليلون->v[i]);
		وإلا
			warning(_("pass a leading slash before paths such as '%s' if you want a single file (see NON-CONE PROBLEMS in the git-sparse-checkout manual)."), دليلون->v[i]);
	}
}

ساكن حرف ثابت * ثابت builtin_sparse_checkout_جوڙيو_usage[] = {
	N_("git sparse-checkout add [--skip-checks] (--stdin | <patterns>)"),
	عدم
};

ساكن بنية sparse_checkout_جوڙيو_opts {
	صحيح skip_checks;
	صحيح use_stdin;
} جوڙيو_opts;

ساكن صحيح sparse_checkout_جوڙيو(صحيح argc, ثابت حرف **argv, ثابت حرف *اڳياڙي,
			       بنية repository *repo)
{
	ساكن بنية option builtin_sparse_checkout_جوڙيو_options[] = {
		OPT_BOOL_F(0, "skip-checks", &جوڙيو_opts.skip_checks,
			   N_("skip some sanity checks on the given paths that might give false positives"),
			   PARSE_OPT_NONEG),
		OPT_BOOL(0, "stdin", &جوڙيو_opts.use_stdin,
			 N_("read patterns from standard in")),
		OPT_پڇاڙي(),
	};
	بنية strvec patterns = STRVEC_INIT;
	صحيح ret;
	بنية repo_config_قدرون *cfg = repo_config_قدرون(the_repository);

	تياري_work_وڻ(the_repository);
	إذا (!cfg->apply_sparse_checkout)
		die(_("no sparse-checkout to add to"));

	repo_read_اشاريو(repo);

	argc = parse_options(argc, argv, اڳياڙي,
			     builtin_sparse_checkout_جوڙيو_options,
			     builtin_sparse_checkout_جوڙيو_usage, 0);

	لكل (صحيح i = 0; i < argc; i++)
		strvec_ڌڪو(&patterns, argv[i]);
	sanitize_paths(repo, &patterns, اڳياڙي, جوڙيو_opts.skip_checks);

	ret = modify_نمونو_list(repo, &patterns, جوڙيو_opts.use_stdin, جوڙيو_2);

	strvec_clear(&patterns);
	أرجع ret;
}

ساكن حرف ثابت * ثابت builtin_sparse_checkout_سيٽ_usage[] = {
	N_("git sparse-checkout set [--[no-]cone] [--[no-]sparse-index] [--skip-checks] (--stdin | <patterns>)"),
	عدم
};

ساكن بنية sparse_checkout_سيٽ_opts {
	صحيح cone_گهڻائي;
	صحيح sparse_اشاريو;
	صحيح skip_checks;
	صحيح use_stdin;
} سيٽ_opts;

ساكن صحيح sparse_checkout_سيٽ(صحيح argc, ثابت حرف **argv, ثابت حرف *اڳياڙي,
			       بنية repository *repo)
{
	بنية repo_config_قدرون *cfg = repo_config_قدرون(the_repository);
	صحيح default_patterns_nr = 2;
	ثابت حرف *default_patterns[] = {"/*", "!/*/", عدم};

	ساكن بنية option builtin_sparse_checkout_سيٽ_options[] = {
		OPT_BOOL(0, "cone", &سيٽ_opts.cone_گهڻائي,
			 N_("initialize the sparse-checkout in cone mode")),
		OPT_BOOL(0, "sparse-index", &سيٽ_opts.sparse_اشاريو,
			 N_("toggle the use of a sparse index")),
		OPT_BOOL_F(0, "skip-checks", &سيٽ_opts.skip_checks,
			   N_("skip some sanity checks on the given paths that might give false positives"),
			   PARSE_OPT_NONEG),
		OPT_BOOL_F(0, "stdin", &سيٽ_opts.use_stdin,
			   N_("read patterns from standard in"),
			   PARSE_OPT_NONEG),
		OPT_پڇاڙي(),
	};
	بنية strvec patterns = STRVEC_INIT;
	صحيح ret;

	تياري_work_وڻ(the_repository);
	repo_read_اشاريو(repo);

	سيٽ_opts.cone_گهڻائي = -1;
	سيٽ_opts.sparse_اشاريو = -1;

	argc = parse_options(argc, argv, اڳياڙي,
			     builtin_sparse_checkout_سيٽ_options,
			     builtin_sparse_checkout_سيٽ_usage, 0);

	إذا (update_modes(repo, &سيٽ_opts.cone_گهڻائي, &سيٽ_opts.sparse_اشاريو))
		أرجع 1;

	/*
	 * Cone mode automatically specifies the toplevel directory.  For
	 * non-cone mode, if nothing is specified, manually select just the
	 * top-level directory (much as 'init' would do).
	 */
	إذا (!cfg->core_sparse_checkout_cone && !سيٽ_opts.use_stdin && argc == 0) {
		لكل (صحيح i = 0; i < default_patterns_nr; i++)
			strvec_ڌڪو(&patterns, default_patterns[i]);
	} وإلا {
		لكل (صحيح i = 0; i < argc; i++)
			strvec_ڌڪو(&patterns, argv[i]);
		sanitize_paths(repo, &patterns, اڳياڙي, سيٽ_opts.skip_checks);
	}

	ret = modify_نمونو_list(repo, &patterns, سيٽ_opts.use_stdin, REPLACE);

	strvec_clear(&patterns);
	أرجع ret;
}

ساكن حرف ثابت * ثابت builtin_sparse_checkout_reapply_usage[] = {
	"git sparse-checkout reapply [--[no-]cone] [--[no-]sparse-index]",
	عدم
};

ساكن بنية sparse_checkout_reapply_opts {
	صحيح cone_گهڻائي;
	صحيح sparse_اشاريو;
} reapply_opts;

ساكن صحيح sparse_checkout_reapply(صحيح argc, ثابت حرف **argv,
				   ثابت حرف *اڳياڙي,
				   بنية repository *repo)
{
	ساكن بنية option builtin_sparse_checkout_reapply_options[] = {
		OPT_BOOL(0, "cone", &reapply_opts.cone_گهڻائي,
			 N_("initialize the sparse-checkout in cone mode")),
		OPT_BOOL(0, "sparse-index", &reapply_opts.sparse_اشاريو,
			 N_("toggle the use of a sparse index")),
		OPT_پڇاڙي(),
	};
	بنية repo_config_قدرون *cfg = repo_config_قدرون(the_repository);

	تياري_work_وڻ(the_repository);
	إذا (!cfg->apply_sparse_checkout)
		die(_("must be in a sparse-checkout to reapply sparsity patterns"));

	reapply_opts.cone_گهڻائي = -1;
	reapply_opts.sparse_اشاريو = -1;

	argc = parse_options(argc, argv, اڳياڙي,
			     builtin_sparse_checkout_reapply_options,
			     builtin_sparse_checkout_reapply_usage, 0);

	repo_read_اشاريو(repo);

	إذا (update_modes(repo, &reapply_opts.cone_گهڻائي, &reapply_opts.sparse_اشاريو))
		أرجع 1;

	أرجع update_working_directory(repo, عدم);
}

ساكن حرف ثابت * ثابت builtin_sparse_checkout_clean_usage[] = {
	"git sparse-checkout clean [-n|--dry-run]",
	عدم
};

ساكن صحيح list_file_iterator(ثابت حرف *رستو, ثابت فراغ *معلومات)
{
	ثابت حرف *نياپو = معلومات;

	لکو(نياپو, رستو);
	أرجع 0;
}

ساكن فراغ list_every_file_اندريون_dir(ثابت حرف *نياپو,
				   ثابت حرف *directory)
{
	بنية strbuf رستو = STRBUF_INIT;

	strbuf_addstr(&رستو, directory);
	for_each_file_اندريون_dir(&رستو, list_file_iterator, نياپو);
	strbuf_release(&رستو);
}

ساكن ثابت حرف *نياپو_remove = N_("Removing %s\n");
ساكن ثابت حرف *نياپو_would_remove = N_("Would remove %s\n");

ساكن صحيح sparse_checkout_clean(صحيح argc, ثابت حرف **argv,
				   ثابت حرف *اڳياڙي,
				   بنية repository *repo)
{
	بنية strbuf ڀريل_رستو = STRBUF_INIT;
	ثابت حرف *نياپو = نياپو_remove;
	سائيز_t worktree_len;
	صحيح force = 0, dry_لاڳيتو = 0, verbose = 0;
	صحيح require_force = 1;
	بنية repo_config_قدرون *cfg = repo_config_قدرون(the_repository);

	بنية option builtin_sparse_checkout_clean_options[] = {
		OPT_DRY_لاڳيتو(&dry_لاڳيتو, N_("dry run")),
		OPT__FORCE(&force, N_("force"), PARSE_OPT_NOCOMPLETE),
		OPT__VERBOSE(&verbose, N_("report each affected file, not just directories")),
		OPT_پڇاڙي(),
	};

	تياري_work_وڻ(the_repository);
	إذا (!cfg->apply_sparse_checkout)
		die(_("must be in a sparse-checkout to clean directories"));
	إذا (!cfg->core_sparse_checkout_cone)
		die(_("must be in a cone-mode sparse-checkout to clean directories"));

	argc = parse_options(argc, argv, اڳياڙي,
			     builtin_sparse_checkout_clean_options,
			     builtin_sparse_checkout_clean_usage, 0);

	repo_config_get_bool(repo, "clean.requireforce", &require_force);
	إذا (require_force && !force && !dry_لاڳيتو)
		die(_("for safety, refusing to clean without one of --force or --dry-run"));

	إذا (dry_لاڳيتو)
		نياپو = نياپو_would_remove;

	إذا (repo_read_اشاريو(repo) < 0)
		die(_("failed to read index"));

	إذا (convert_منزل_sparse(repo->اشاريو, SPARSE_اشاريو_يادگيري_ONLY) ||
	    repo->اشاريو->sparse_اشاريو == اشاريو_EXPANDED)
		die(_("failed to convert index to a sparse index; resolve merge conflicts and try again"));

	strbuf_addstr(&ڀريل_رستو, repo->worktree);
	strbuf_addch(&ڀريل_رستو, '/');
	worktree_len = ڀريل_رستو.len;

	لكل (سائيز_t i = 0; i < repo->اشاريو->cache_nr; i++) {
		بنية cache_داخلا *ce = repo->اشاريو->cache[i];
		إذا (!S_ISSPARSEDIR(ce->ce_گهڻائي))
			استمر;
		strbuf_setlen(&ڀريل_رستو, worktree_len);
		strbuf_جوڙيو(&ڀريل_رستو, ce->name, ce->ce_namelen);

		إذا (!is_directory(ڀريل_رستو.buf))
			استمر;

		إذا (verbose)
			list_every_file_اندريون_dir(نياپو, ce->name);
		وإلا
			لکو(نياپو, ce->name);

		إذا (dry_لاڳيتو <= 0 &&
		    remove_dir_recursively(&ڀريل_رستو, 0))
			warning_errno(_("failed to remove '%s'"), ce->name);
	}

	strbuf_release(&ڀريل_رستو);
	أرجع 0;
}

ساكن حرف ثابت * ثابت builtin_sparse_checkout_disable_usage[] = {
	"git sparse-checkout disable",
	عدم
};

ساكن صحيح sparse_checkout_disable(صحيح argc, ثابت حرف **argv,
				   ثابت حرف *اڳياڙي,
				   بنية repository *repo)
{
	ساكن بنية option builtin_sparse_checkout_disable_options[] = {
		OPT_پڇاڙي(),
	};
	بنية نمونو_list pl;
	بنية repo_config_قدرون *cfg = repo_config_قدرون(the_repository);

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

	تياري_work_وڻ(the_repository);
	argc = parse_options(argc, argv, اڳياڙي,
			     builtin_sparse_checkout_disable_options,
			     builtin_sparse_checkout_disable_usage, 0);

	/*
	 * Disable the advice message for expanding a sparse index, as we
	 * are expecting to do that when disabling sparse-checkout.
	 */
	give_advice_on_expansion = 0;
	repo_read_اشاريو(repo);

	memset(&pl, 0, حجم(pl));
	hashmap_init(&pl.recursive_hashmap, pl_hashmap_cmp, عدم, 0);
	hashmap_init(&pl.والد_hashmap, pl_hashmap_cmp, عدم, 0);
	pl.use_cone_patterns = 0;
	cfg->apply_sparse_checkout = 1;

	جوڙيو_نمونو("/*", خالي_جايون_بنياد, 0, &pl, 0);

	prepare_repo_settings(the_repository);
	repo->settings.sparse_اشاريو = 0;

	إذا (update_working_directory(repo, &pl))
		die(_("error while refreshing working directory"));

	clear_نمونو_list(&pl);
	أرجع سيٽ_config(repo, گهڻائي_NO_PATTERNS);
}

ساكن حرف ثابت * ثابت builtin_sparse_checkout_check_قاعدا_usage[] = {
	N_("git sparse-checkout check-rules [-z] [--skip-checks]"
	   "[--[no-]cone] [--rules-file <file>]"),
	عدم
};

ساكن بنية sparse_checkout_check_قاعدا_opts {
	صحيح cone_گهڻائي;
	صحيح null_termination;
	حرف *قاعدا_file;
} check_قاعدا_opts;

ساكن صحيح check_قاعدا(بنية repository *repo,
		       بنية نمونو_list *pl,
		       صحيح null_terminated)
{
	بنية strbuf سٽ = STRBUF_INIT;
	بنية strbuf unquoted = STRBUF_INIT;
	حرف *رستو;
	صحيح سٽ_terminator = null_terminated ? 0 : '\n';
	strbuf_getline_fn getline_fn = null_terminated ? strbuf_getline_nul
		: strbuf_getline;
	repo->اشاريو->sparse_checkout_patterns = pl;
	طالما (!getline_fn(&سٽ, stdin)) {
		رستو = سٽ.buf;
		إذا (!null_terminated && سٽ.buf[0] == '"') {
			strbuf_reset(&unquoted);
			إذا (unquote_c_style(&unquoted, سٽ.buf, عدم))
				die(_("unable to unquote C-style string '%s'"),
					سٽ.buf);

			رستو = unquoted.buf;
		}

		إذا (رستو_اندريون_sparse_checkout(رستو, repo->اشاريو))
			write_name_quoted(رستو, stdout, سٽ_terminator);
	}
	strbuf_release(&سٽ);
	strbuf_release(&unquoted);

	أرجع 0;
}

ساكن صحيح sparse_checkout_check_قاعدا(صحيح argc, ثابت حرف **argv, ثابت حرف *اڳياڙي,
				       بنية repository *repo)
{
	ساكن بنية option builtin_sparse_checkout_check_قاعدا_options[] = {
		OPT_BOOL('z', عدم, &check_قاعدا_opts.null_termination,
			 N_("terminate input and output files by a NUL character")),
		OPT_BOOL(0, "cone", &check_قاعدا_opts.cone_گهڻائي,
			 N_("when used with --rules-file interpret patterns as cone mode patterns")),
		OPT_FILENAME(0, "rules-file", &check_قاعدا_opts.قاعدا_file,
			 N_("use patterns in <file> instead of the current ones.")),
		OPT_پڇاڙي(),
	};

	FILE *fp;
	صحيح ret;
	بنية نمونو_list pl = {0};
	بنية repo_config_قدرون *cfg = repo_config_قدرون(the_repository);
	حرف *sparse_filename;
	check_قاعدا_opts.cone_گهڻائي = -1;

	argc = parse_options(argc, argv, اڳياڙي,
			     builtin_sparse_checkout_check_قاعدا_options,
			     builtin_sparse_checkout_check_قاعدا_usage, 0);

	إذا (check_قاعدا_opts.قاعدا_file && check_قاعدا_opts.cone_گهڻائي < 0)
		check_قاعدا_opts.cone_گهڻائي = 1;

	update_cone_گهڻائي(&check_قاعدا_opts.cone_گهڻائي);
	pl.use_cone_patterns = cfg->core_sparse_checkout_cone;
	إذا (check_قاعدا_opts.قاعدا_file) {
		fp = xfopen(check_قاعدا_opts.قاعدا_file, "r");
		جوڙيو_patterns_from_داخلا(&pl, argc, argv, fp);
		fclose(fp);
	} وإلا {
		sparse_filename = get_sparse_checkout_filename();
		إذا (جوڙيو_patterns_from_file_منزل_list(sparse_filename, "", 0, &pl,
						   عدم, 0))
			die(_("unable to load existing sparse-checkout patterns"));
		حرر(sparse_filename);
	}

	ret = check_قاعدا(repo, &pl, check_قاعدا_opts.null_termination);
	clear_نمونو_list(&pl);
	حرر(check_قاعدا_opts.قاعدا_file);
	أرجع ret;
}

صحيح cmd_sparse_checkout(صحيح argc,
			ثابت حرف **argv,
			ثابت حرف *اڳياڙي,
			بنية repository *repo)
{
	parse_opt_subcommand_fn *fn = عدم;
	بنية option builtin_sparse_checkout_options[] = {
		OPT_SUBCOMMAND("list", &fn, sparse_checkout_list),
		OPT_SUBCOMMAND("init", &fn, sparse_checkout_init),
		OPT_SUBCOMMAND("set", &fn, sparse_checkout_سيٽ),
		OPT_SUBCOMMAND("add", &fn, sparse_checkout_جوڙيو),
		OPT_SUBCOMMAND("reapply", &fn, sparse_checkout_reapply),
		OPT_SUBCOMMAND("clean", &fn, sparse_checkout_clean),
		OPT_SUBCOMMAND("disable", &fn, sparse_checkout_disable),
		OPT_SUBCOMMAND("check-rules", &fn, sparse_checkout_check_قاعدا),
		OPT_پڇاڙي(),
	};

	argc = parse_options(argc, argv, اڳياڙي,
			     builtin_sparse_checkout_options,
			     builtin_sparse_checkout_usage, 0);

	repo_config(the_repository, git_default_config, عدم);

	prepare_repo_settings(repo);
	repo->settings.command_requires_ڀريل_اشاريو = 0;

	أرجع fn(argc, argv, اڳياڙي, repo);
}
