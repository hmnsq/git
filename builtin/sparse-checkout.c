#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#تعریف_کرو DISABLE_SIGN_COMPARE_WARNINGS

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

ساكن ثابت حرف *خالی_تھاواں_ادھار = "";

ساكن حرف ثابت * ثابت builtin_sparse_checkout_usage[] = {
	N_("git sparse-checkout (init | list | set | add | reapply | disable | check-rules | clean) [<options>]"),
	عدم
};

ساكن فراغ write_patterns_منزل_file(FILE *fp, بنية نمونہ_list *pl)
{
	صحيح i;

	لكل (i = 0; i < pl->nr; i++) {
		بنية رستہ_نمونہ *p = pl->patterns[i];

		إذا (p->flags & نمونہ_FLAG_NEGATIVE)
			fprintf(fp, "!");

		fprintf(fp, "%s", p->نمونہ);

		إذا (p->flags & نمونہ_FLAG_MUSTBEDIR)
			fprintf(fp, "/");

		fprintf(fp, "\n");
	}
}

ساكن حرف ثابت * ثابت builtin_sparse_checkout_list_usage[] = {
	"git sparse-checkout list",
	عدم
};

ساكن صحيح sparse_checkout_list(صحيح argc, ثابت حرف **argv, ثابت حرف *اگیتر,
				بنية repository *repo بن_ورتیا)
{
	ساكن بنية option builtin_sparse_checkout_list_options[] = {
		OPT_سرا(),
	};
	بنية نمونہ_list pl;
	حرف *sparse_filename;
	صحيح res;
	بنية repo_config_اقدار *cfg = repo_config_اقدار(the_repository);

	تیاری_work_درخت(the_repository);
	إذا (!cfg->apply_sparse_checkout)
		die(_("this worktree is not sparse"));

	argc = parse_options(argc, argv, اگیتر,
			     builtin_sparse_checkout_list_options,
			     builtin_sparse_checkout_list_usage, 0);

	memset(&pl, 0, حجم(pl));

	pl.use_cone_patterns = cfg->core_sparse_checkout_cone;

	sparse_filename = get_sparse_checkout_filename();
	res = جوڑو_patterns_from_file_منزل_list(sparse_filename, "", 0, &pl, عدم, 0);
	حرر(sparse_filename);

	إذا (res < 0) {
		warning(_("this worktree is not sparse (sparse-checkout file may not exist)"));
		أرجع 0;
	}

	إذا (pl.use_cone_patterns) {
		صحيح i;
		بنية نمونہ_اندراج *pe;
		بنية hashmap_iter iter;
		بنية string_list sl = STRING_LIST_INIT_DUP;

		hashmap_for_each_اندراج(&pl.recursive_hashmap, &iter, pe, ent) {
			/* pe->pattern starts with "/", skip it */
			string_list_append(&sl, pe->نمونہ + 1);
		}

		string_list_ترتیب_دیو_u(&sl, 0);

		لكل (i = 0; i < sl.nr; i++) {
			quote_c_style(sl.اشیاء[i].string, عدم, stdout, 0);
			لکھو("\n");
		}

		string_list_clear(&sl, 0);
	} وإلا {
		write_patterns_منزل_file(stdout, &pl);
	}

	clear_نمونہ_list(&pl);

	أرجع 0;
}

ساكن فراغ clean_tracked_sparse_directories(بنية repository *r)
{
	صحيح i, was_بھریا = 0;
	بنية strbuf رستہ = STRBUF_INIT;
	ناپ_t pathlen;
	بنية string_list_شے *شے;
	بنية string_list sparse_dirs = STRING_LIST_INIT_DUP;

	/*
	 * If we are not using cone mode patterns, then we cannot
	 * delete directories outside of the sparse cone.
	 */
	إذا (!r || !r->اشاریہ || !r->worktree)
		أرجع;
	إذا (init_sparse_checkout_patterns(r->اشاریہ) ||
	    !r->اشاریہ->sparse_checkout_patterns->use_cone_patterns)
		أرجع;

	/*
	 * Use the sparse index as a data structure to assist finding
	 * directories that are safe to delete. This conversion to a
	 * sparse index will not delete directories that contain
	 * conflicted entries or submodules.
	 */
	إذا (r->اشاریہ->sparse_اشاریہ == اشاریہ_EXPANDED) {
		/*
		 * If something, such as a merge conflict or other concern,
		 * prevents us from converting to a sparse index, then do
		 * not try deleting files.
		 */
		إذا (convert_منزل_sparse(r->اشاریہ, SPARSE_اشاریہ_حافظہ_ONLY))
			أرجع;
		was_بھریا = 1;
	}

	strbuf_addstr(&رستہ, r->worktree);
	strbuf_complete(&رستہ, '/');
	pathlen = رستہ.len;

	/*
	 * Collect directories that have gone out of scope but also
	 * exist on disk, so there is some work to be done. We need to
	 * store the entries in a list before exploring, since that might
	 * expand the sparse-index again.
	 */
	لكل (i = 0; i < r->اشاریہ->cache_nr; i++) {
		بنية cache_اندراج *ce = r->اشاریہ->cache[i];

		إذا (S_ISSPARSEDIR(ce->ce_منوال) &&
		    repo_file_exists(r, ce->name))
			string_list_append(&sparse_dirs, ce->name);
	}

	for_each_string_list_شے(شے, &sparse_dirs) {
		بنية dir_struct dir = DIR_INIT;
		بنية pathspec p = { 0 };
		بنية strvec s = STRVEC_INIT;

		strbuf_setlen(&رستہ, pathlen);
		strbuf_addstr(&رستہ, شے->string);

		dir.flags |= DIR_SHOW_IGNORED_TOO;

		تیاری_standard_excludes(&dir);
		strvec_ڈالو(&s, رستہ.buf);

		parse_pathspec(&p, PATHSPEC_GLOB, 0, عدم, s.v);
		fill_directory(&dir, r->اشاریہ, &p);

		إذا (dir.nr) {
			warning(_("directory '%s' contains untracked files,"
				  " but is not in the sparse-checkout cone"),
				شے->string);
		} وإلا إذا (remove_dir_recursively(&رستہ, 0)) {
			/*
			 * Removal is "best effort". If something blocks
			 * the deletion, then continue with a warning.
			 */
			warning(_("failed to remove directory '%s'"),
				شے->string);
		}

		strvec_clear(&s);
		clear_pathspec(&p);
		dir_clear(&dir);
	}

	string_list_clear(&sparse_dirs, 0);
	strbuf_release(&رستہ);

	إذا (was_بھریا)
		ensure_بھریا_اشاریہ(r->اشاریہ);
}

ساكن صحيح update_working_directory(بنية repository *r,
				    بنية نمونہ_list *pl)
{
	تعداد update_sparsity_نتیجہ نتیجہ;
	بنية unpack_trees_options o;
	بنية جندرا_file جندرا_file = جندرا_INIT;
	بنية نمونہ_list *old_pl;

	/* If no branch has been checked out, there are no updates to make. */
	إذا (is_اشاریہ_unborn(r->اشاریہ))
		أرجع UPDATE_SPARSITY_SUCCESS;

	old_pl = r->اشاریہ->sparse_checkout_patterns;
	r->اشاریہ->sparse_checkout_patterns = pl;

	memset(&o, 0, حجم(o));
	o.verbose_update = isatty(2);
	o.update = 1;
	o.سر_idx = -1;
	o.src_اشاریہ = r->اشاریہ;
	o.dst_اشاریہ = r->اشاریہ;
	o.skip_sparse_checkout = 0;

	تیاری_work_درخت(the_repository);

	repo_hold_locked_اشاریہ(r, &جندرا_file, جندرا_DIE_ON_ERROR);

	تیاری_unpack_trees_porcelain(&o, "sparse-checkout");
	نتیجہ = update_sparsity(&o, pl);
	clear_unpack_trees_porcelain(&o);

	إذا (نتیجہ == UPDATE_SPARSITY_WARNINGS)
		/*
		 * We don't do any special handling of warnings from untracked
		 * files in the way or dirty entries that can't be removed.
		 */
		نتیجہ = UPDATE_SPARSITY_SUCCESS;
	إذا (نتیجہ == UPDATE_SPARSITY_SUCCESS)
		write_locked_اشاریہ(r->اشاریہ, &جندرا_file, COMMIT_جندرا);
	وإلا
		rollback_جندرا_file(&جندرا_file);

	clean_tracked_sparse_directories(r);

	إذا (r->اشاریہ->sparse_checkout_patterns != pl) {
		clear_نمونہ_list(r->اشاریہ->sparse_checkout_patterns);
		FREE_AND_NULL(r->اشاریہ->sparse_checkout_patterns);
	}
	r->اشاریہ->sparse_checkout_patterns = old_pl;

	أرجع نتیجہ;
}

ساكن حرف *escaped_نمونہ(حرف *نمونہ)
{
	حرف *p = نمونہ;
	بنية strbuf final = STRBUF_INIT;

	طالما (*p) {
		إذا (is_glob_special(*p))
			strbuf_addch(&final, '\\');

		strbuf_addch(&final, *p);
		p++;
	}

	أرجع strbuf_detach(&final, عدم);
}

ساكن فراغ write_cone_منزل_file(FILE *fp, بنية نمونہ_list *pl)
{
	صحيح i;
	بنية نمونہ_اندراج *pe;
	بنية hashmap_iter iter;
	بنية string_list sl = STRING_LIST_INIT_DUP;
	بنية strbuf ماپا_نمونہ = STRBUF_INIT;

	hashmap_for_each_اندراج(&pl->ماپا_hashmap, &iter, pe, ent) {
		إذا (hashmap_get_اندراج(&pl->recursive_hashmap, pe, ent, عدم))
			استمر;

		إذا (!hashmap_شامل_اے_ماپا(&pl->recursive_hashmap,
					     pe->نمونہ,
					     &ماپا_نمونہ))
			string_list_append(&sl, pe->نمونہ);
	}

	string_list_ترتیب_دیو_u(&sl, 0);

	fprintf(fp, "/*\n!/*/\n");

	لكل (i = 0; i < sl.nr; i++) {
		حرف *نمونہ = escaped_نمونہ(sl.اشیاء[i].string);

		إذا (لڑی_لمبائی(نمونہ))
			fprintf(fp, "%s/\n!%s/*/\n", نمونہ, نمونہ);
		حرر(نمونہ);
	}

	string_list_clear(&sl, 0);

	hashmap_for_each_اندراج(&pl->recursive_hashmap, &iter, pe, ent) {
		إذا (!hashmap_شامل_اے_ماپا(&pl->recursive_hashmap,
					     pe->نمونہ,
					     &ماپا_نمونہ))
			string_list_append(&sl, pe->نمونہ);
	}

	strbuf_release(&ماپا_نمونہ);

	string_list_ترتیب_دیو_u(&sl, 0);

	لكل (i = 0; i < sl.nr; i++) {
		حرف *نمونہ = escaped_نمونہ(sl.اشیاء[i].string);
		fprintf(fp, "%s/\n", نمونہ);
		حرر(نمونہ);
	}

	string_list_clear(&sl, 0);
}

ساكن صحيح write_patterns_and_update(بنية repository *repo,
				     بنية نمونہ_list *pl)
{
	حرف *sparse_filename;
	FILE *fp;
	بنية جندرا_file lk = جندرا_INIT;
	صحيح نتیجہ;
	بنية repo_config_اقدار *cfg = repo_config_اقدار(the_repository);

	sparse_filename = get_sparse_checkout_filename();

	إذا (محفوظ_create_leading_directories(repo, sparse_filename))
		die(_("failed to create directory for sparse-checkout file"));

	repo_hold_جندرا_file_for_update(repo, &lk, sparse_filename,
				       جندرا_DIE_ON_ERROR);

	نتیجہ = update_working_directory(repo, pl);
	إذا (نتیجہ) {
		rollback_جندرا_file(&lk);
		update_working_directory(repo, عدم);
		اذهب باہرلا;
	}

	fp = fdopen_جندرا_file(&lk, "w");
	إذا (!fp)
		die_errno(_("unable to fdopen %s"), get_جندرا_file_رستہ(&lk));

	إذا (cfg->core_sparse_checkout_cone)
		write_cone_منزل_file(fp, pl);
	وإلا
		write_patterns_منزل_file(fp, pl);

	إذا (commit_جندرا_file(&lk))
		die_errno(_("unable to write %s"), sparse_filename);

باہرلا:
	clear_نمونہ_list(pl);
	حرر(sparse_filename);
	أرجع نتیجہ;
}

تعداد sparse_checkout_منوال {
	منوال_NO_PATTERNS = 0,
	منوال_ALL_PATTERNS = 1,
	منوال_CONE_PATTERNS = 2,
};

ساكن صحيح سیٹ_config(بنية repository *repo,
		      تعداد sparse_checkout_منوال منوال)
{
	/* Update to use worktree config, if not already. */
	إذا (init_worktree_config(repo)) {
		error(_("failed to initialize worktree config"));
		أرجع 1;
	}

	إذا (repo_config_سیٹ_worktree_gently(repo,
					    "core.sparseCheckout",
					    منوال ? "true" : "false") ||
	    repo_config_سیٹ_worktree_gently(repo,
					    "core.sparseCheckoutCone",
					    منوال == منوال_CONE_PATTERNS ?
						"true" : "false"))
		أرجع 1;

	إذا (منوال == منوال_NO_PATTERNS)
		أرجع سیٹ_sparse_اشاریہ_config(repo, 0);

	أرجع 0;
}

ساكن تعداد sparse_checkout_منوال update_cone_منوال(صحيح *cone_منوال) {
	بنية repo_config_اقدار *cfg = repo_config_اقدار(the_repository);

	/* If not specified, use previous definition of cone mode */
	إذا (*cone_منوال == -1 && cfg->apply_sparse_checkout)
		*cone_منوال = cfg->core_sparse_checkout_cone;

	/* Set cone/non-cone mode appropriately */
	cfg->apply_sparse_checkout = 1;
	إذا (*cone_منوال == 1 || *cone_منوال == -1) {
		cfg->core_sparse_checkout_cone = 1;
		أرجع منوال_CONE_PATTERNS;
	}
	cfg->core_sparse_checkout_cone = 0;
	أرجع منوال_ALL_PATTERNS;
}

ساكن صحيح update_modes(بنية repository *repo, صحيح *cone_منوال, صحيح *sparse_اشاریہ)
{
	صحيح منوال, ریکارڈ_منوال;
	بنية repo_config_اقدار *cfg = repo_config_اقدار(the_repository);

	/* Determine if we need to record the mode; ensure sparse checkout on */
	ریکارڈ_منوال = (*cone_منوال != -1) || !cfg->apply_sparse_checkout;

	منوال = update_cone_منوال(cone_منوال);
	إذا (ریکارڈ_منوال && سیٹ_config(repo, منوال))
		أرجع 1;

	/* Set sparse-index/non-sparse-index mode if specified */
	إذا (*sparse_اشاریہ >= 0) {
		إذا (سیٹ_sparse_اشاریہ_config(repo, *sparse_اشاریہ) < 0)
			die(_("failed to modify sparse-index config"));

		/* force an index rewrite */
		repo_read_اشاریہ(repo);
		repo->اشاریہ->updated_workdir = 1;

		إذا (!*sparse_اشاریہ)
			ensure_بھریا_اشاریہ(repo->اشاریہ);
	}

	أرجع 0;
}

ساكن حرف ثابت * ثابت builtin_sparse_checkout_init_usage[] = {
	"git sparse-checkout init [--cone] [--[no-]sparse-index]",
	عدم
};

ساكن بنية sparse_checkout_init_opts {
	صحيح cone_منوال;
	صحيح sparse_اشاریہ;
} init_opts;

ساكن صحيح sparse_checkout_init(صحيح argc, ثابت حرف **argv, ثابت حرف *اگیتر,
				بنية repository *repo)
{
	بنية نمونہ_list pl;
	حرف *sparse_filename;
	صحيح res;
	بنية object_id oid;

	ساكن بنية option builtin_sparse_checkout_init_options[] = {
		OPT_BOOL(0, "cone", &init_opts.cone_منوال,
			 N_("initialize the sparse-checkout in cone mode")),
		OPT_BOOL(0, "sparse-index", &init_opts.sparse_اشاریہ,
			 N_("toggle the use of a sparse index")),
		OPT_سرا(),
	};

	تیاری_work_درخت(the_repository);
	repo_read_اشاریہ(repo);

	init_opts.cone_منوال = -1;
	init_opts.sparse_اشاریہ = -1;

	argc = parse_options(argc, argv, اگیتر,
			     builtin_sparse_checkout_init_options,
			     builtin_sparse_checkout_init_usage, 0);

	إذا (update_modes(repo, &init_opts.cone_منوال, &init_opts.sparse_اشاریہ))
		أرجع 1;

	memset(&pl, 0, حجم(pl));

	sparse_filename = get_sparse_checkout_filename();
	res = جوڑو_patterns_from_file_منزل_list(sparse_filename, "", 0, &pl, عدم, 0);

	/* If we already have a sparse-checkout file, use it. */
	إذا (res >= 0) {
		حرر(sparse_filename);
		clear_نمونہ_list(&pl);
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

	جوڑو_نمونہ("/*", خالی_تھاواں_ادھار, 0, &pl, 0);
	جوڑو_نمونہ("!/*/", خالی_تھاواں_ادھار, 0, &pl, 0);
	pl.use_cone_patterns = init_opts.cone_منوال;

	أرجع write_patterns_and_update(repo, &pl);
}

ساكن فراغ داخل_کرو_recursive_نمونہ(بنية نمونہ_list *pl, بنية strbuf *رستہ)
{
	بنية نمونہ_اندراج *e = xmalloc(حجم(*e));
	e->patternlen = رستہ->len;
	e->نمونہ = strbuf_detach(رستہ, عدم);
	hashmap_اندراج_init(&e->ent, fspathhash(e->نمونہ));

	hashmap_جوڑو(&pl->recursive_hashmap, &e->ent);

	طالما (e->patternlen) {
		حرف *slash = strrchr(e->نمونہ, '/');
		حرف *oldpattern = e->نمونہ;
		ناپ_t newlen;
		بنية نمونہ_اندراج *dup;

		إذا (!slash || slash == e->نمونہ)
			اكسر;

		newlen = slash - e->نمونہ;
		e = xmalloc(حجم(بنية نمونہ_اندراج));
		e->patternlen = newlen;
		e->نمونہ = xstrndup(oldpattern, newlen);
		hashmap_اندراج_init(&e->ent, fspathhash(e->نمونہ));

		dup = hashmap_get_اندراج(&pl->ماپا_hashmap, e, ent, عدم);
		إذا (!dup) {
			hashmap_جوڑو(&pl->ماپا_hashmap, &e->ent);
		} وإلا {
			حرر(e->نمونہ);
			حرر(e);
			e = dup;
		}
	}
}

ساكن فراغ strbuf_منزل_cone_نمونہ(بنية strbuf *سطر, بنية نمونہ_list *pl)
{
	strbuf_trim(سطر);

	strbuf_trim_trailing_dir_sep(سطر);

	إذا (strbuf_normalize_رستہ(سطر))
		die(_("could not normalize path %s"), سطر->buf);

	إذا (!سطر->len)
		أرجع;

	إذا (سطر->buf[0] != '/')
		strbuf_insertstr(سطر, 0, "/");

	داخل_کرو_recursive_نمونہ(pl, سطر);
}

ساكن فراغ جوڑو_patterns_from_داخلہ(بنية نمونہ_list *pl,
				    صحيح argc, ثابت حرف **argv,
				    FILE *file)
{
	صحيح i;
	بنية repo_config_اقدار *cfg = repo_config_اقدار(the_repository);

	إذا (cfg->core_sparse_checkout_cone) {
		بنية strbuf سطر = STRBUF_INIT;

		hashmap_init(&pl->recursive_hashmap, pl_hashmap_cmp, عدم, 0);
		hashmap_init(&pl->ماپا_hashmap, pl_hashmap_cmp, عدم, 0);
		pl->use_cone_patterns = 1;

		إذا (file) {
			بنية strbuf unquoted = STRBUF_INIT;
			طالما (!strbuf_getline(&سطر, file)) {
				إذا (سطر.buf[0] == '"') {
					strbuf_reset(&unquoted);
					إذا (unquote_c_style(&unquoted, سطر.buf, عدم))
						die(_("unable to unquote C-style string '%s'"),
						سطر.buf);

					strbuf_وٹاندرا(&unquoted, &سطر);
				}

				strbuf_منزل_cone_نمونہ(&سطر, pl);
			}

			strbuf_release(&unquoted);
		} وإلا {
			لكل (i = 0; i < argc; i++) {
				strbuf_setlen(&سطر, 0);
				strbuf_addstr(&سطر, argv[i]);
				strbuf_منزل_cone_نمونہ(&سطر, pl);
			}
		}
		strbuf_release(&سطر);
	} وإلا {
		إذا (file) {
			بنية strbuf سطر = STRBUF_INIT;

			طالما (!strbuf_getline(&سطر, file))
				جوڑو_نمونہ(سطر.buf, خالی_تھاواں_ادھار, 0, pl, 0);

			strbuf_release(&سطر);
		} وإلا {
			لكل (i = 0; i < argc; i++)
				جوڑو_نمونہ(argv[i], خالی_تھاواں_ادھار, 0, pl, 0);
		}
	}
}

تعداد modify_type {
	REPLACE,
	جوڑو_2,
};

ساكن فراغ جوڑو_patterns_cone_منوال(صحيح argc, ثابت حرف **argv,
				   بنية نمونہ_list *pl,
				   صحيح use_stdin)
{
	بنية strbuf بفر = STRBUF_INIT;
	بنية نمونہ_اندراج *pe;
	بنية hashmap_iter iter;
	بنية نمونہ_list existing;
	بنية repo_config_اقدار *cfg = repo_config_اقدار(the_repository);
	حرف *sparse_filename = get_sparse_checkout_filename();

	جوڑو_patterns_from_داخلہ(pl, argc, argv,
				use_stdin ? stdin : عدم);

	memset(&existing, 0, حجم(existing));
	existing.use_cone_patterns = cfg->core_sparse_checkout_cone;

	إذا (جوڑو_patterns_from_file_منزل_list(sparse_filename, "", 0,
					   &existing, عدم, 0))
		die(_("unable to load existing sparse-checkout patterns"));
	حرر(sparse_filename);

	إذا (!existing.use_cone_patterns)
		die(_("existing sparse-checkout patterns do not use cone mode"));

	hashmap_for_each_اندراج(&existing.recursive_hashmap, &iter, pe, ent) {
		إذا (!hashmap_شامل_اے_ماپا(&pl->recursive_hashmap,
					pe->نمونہ, &بفر) ||
		    !hashmap_شامل_اے_ماپا(&pl->ماپا_hashmap,
					pe->نمونہ, &بفر)) {
			strbuf_reset(&بفر);
			strbuf_addstr(&بفر, pe->نمونہ);
			داخل_کرو_recursive_نمونہ(pl, &بفر);
		}
	}

	clear_نمونہ_list(&existing);
	strbuf_release(&بفر);
}

ساكن فراغ جوڑو_patterns_literal(صحيح argc, ثابت حرف **argv,
				 بنية نمونہ_list *pl,
				 صحيح use_stdin)
{
	حرف *sparse_filename = get_sparse_checkout_filename();
	إذا (جوڑو_patterns_from_file_منزل_list(sparse_filename, "", 0,
					   pl, عدم, 0))
		die(_("unable to load existing sparse-checkout patterns"));
	حرر(sparse_filename);
	جوڑو_patterns_from_داخلہ(pl, argc, argv, use_stdin ? stdin : عدم);
}

ساكن صحيح modify_نمونہ_list(بنية repository *repo,
			       بنية strvec *دلیلاں, صحيح use_stdin,
			       تعداد modify_type m)
{
	صحيح نتیجہ;
	صحيح بدلیا_config = 0;
	بنية نمونہ_list *pl = xcalloc(1, حجم(*pl));
	بنية repo_config_اقدار *cfg = repo_config_اقدار(the_repository);

	بدل (m) {
	حالة جوڑو_2:
		إذا (cfg->core_sparse_checkout_cone)
			جوڑو_patterns_cone_منوال(دلیلاں->nr, دلیلاں->v, pl, use_stdin);
		وإلا
			جوڑو_patterns_literal(دلیلاں->nr, دلیلاں->v, pl, use_stdin);
		اكسر;

	حالة REPLACE:
		جوڑو_patterns_from_داخلہ(pl, دلیلاں->nr, دلیلاں->v,
					use_stdin ? stdin : عدم);
		اكسر;
	}

	إذا (!cfg->apply_sparse_checkout) {
		سیٹ_config(repo, منوال_ALL_PATTERNS);
		cfg->apply_sparse_checkout = 1;
		بدلیا_config = 1;
	}

	نتیجہ = write_patterns_and_update(repo, pl);

	إذا (نتیجہ && بدلیا_config)
		سیٹ_config(repo, منوال_NO_PATTERNS);

	clear_نمونہ_list(pl);
	حرر(pl);
	أرجع نتیجہ;
}

ساكن فراغ sanitize_paths(بنية repository *repo,
			   بنية strvec *دلیلاں,
			   ثابت حرف *اگیتر, صحيح skip_checks)
{
	صحيح i;
	بنية repo_config_اقدار *cfg = repo_config_اقدار(the_repository);

	إذا (!دلیلاں->nr)
		أرجع;

	إذا (اگیتر && *اگیتر && cfg->core_sparse_checkout_cone) {
		/*
		 * The args are not pathspecs, so unfortunately we
		 * cannot imitate how cmd_add() uses parse_pathspec().
		 */
		صحيح اگیتر_len = لڑی_لمبائی(اگیتر);

		لكل (i = 0; i < دلیلاں->nr; i++) {
			حرف *prefixed_رستہ = اگیتر_رستہ(the_repository, اگیتر,
							  اگیتر_len, دلیلاں->v[i]);
			strvec_replace(دلیلاں, i, prefixed_رستہ);
			حرر(prefixed_رستہ);
		}
	}

	إذا (skip_checks)
		أرجع;

	إذا (اگیتر && *اگیتر && !cfg->core_sparse_checkout_cone)
		die(_("please run from the toplevel directory in non-cone mode"));

	إذا (cfg->core_sparse_checkout_cone) {
		لكل (i = 0; i < دلیلاں->nr; i++) {
			إذا (دلیلاں->v[i][0] == '/')
				die(_("specify directories rather than patterns (no leading slash)"));
			إذا (دلیلاں->v[i][0] == '!')
				die(_("specify directories rather than patterns.  If your directory starts with a '!', pass --skip-checks"));
			إذا (strpbrk(دلیلاں->v[i], "*?[]"))
				die(_("specify directories rather than patterns.  If your directory really has any of '*?[]\\' in it, pass --skip-checks"));
		}
	}

	لكل (i = 0; i < دلیلاں->nr; i++) {
		بنية cache_اندراج *ce;
		بنية اشاریہ_state *اشاریہ = repo->اشاریہ;
		صحيح pos = اشاریہ_name_pos(اشاریہ, دلیلاں->v[i], لڑی_لمبائی(دلیلاں->v[i]));

		إذا (pos < 0)
			استمر;
		ce = اشاریہ->cache[pos];
		إذا (S_ISSPARSEDIR(ce->ce_منوال))
			استمر;

		إذا (cfg->core_sparse_checkout_cone)
			die(_("'%s' is not a directory; to treat it as a directory anyway, rerun with --skip-checks"), دلیلاں->v[i]);
		وإلا
			warning(_("pass a leading slash before paths such as '%s' if you want a single file (see NON-CONE PROBLEMS in the git-sparse-checkout manual)."), دلیلاں->v[i]);
	}
}

ساكن حرف ثابت * ثابت builtin_sparse_checkout_جوڑو_usage[] = {
	N_("git sparse-checkout add [--skip-checks] (--stdin | <patterns>)"),
	عدم
};

ساكن بنية sparse_checkout_جوڑو_opts {
	صحيح skip_checks;
	صحيح use_stdin;
} جوڑو_opts;

ساكن صحيح sparse_checkout_جوڑو(صحيح argc, ثابت حرف **argv, ثابت حرف *اگیتر,
			       بنية repository *repo)
{
	ساكن بنية option builtin_sparse_checkout_جوڑو_options[] = {
		OPT_BOOL_F(0, "skip-checks", &جوڑو_opts.skip_checks,
			   N_("skip some sanity checks on the given paths that might give false positives"),
			   PARSE_OPT_NONEG),
		OPT_BOOL(0, "stdin", &جوڑو_opts.use_stdin,
			 N_("read patterns from standard in")),
		OPT_سرا(),
	};
	بنية strvec patterns = STRVEC_INIT;
	صحيح ret;
	بنية repo_config_اقدار *cfg = repo_config_اقدار(the_repository);

	تیاری_work_درخت(the_repository);
	إذا (!cfg->apply_sparse_checkout)
		die(_("no sparse-checkout to add to"));

	repo_read_اشاریہ(repo);

	argc = parse_options(argc, argv, اگیتر,
			     builtin_sparse_checkout_جوڑو_options,
			     builtin_sparse_checkout_جوڑو_usage, 0);

	لكل (صحيح i = 0; i < argc; i++)
		strvec_ڈالو(&patterns, argv[i]);
	sanitize_paths(repo, &patterns, اگیتر, جوڑو_opts.skip_checks);

	ret = modify_نمونہ_list(repo, &patterns, جوڑو_opts.use_stdin, جوڑو_2);

	strvec_clear(&patterns);
	أرجع ret;
}

ساكن حرف ثابت * ثابت builtin_sparse_checkout_سیٹ_usage[] = {
	N_("git sparse-checkout set [--[no-]cone] [--[no-]sparse-index] [--skip-checks] (--stdin | <patterns>)"),
	عدم
};

ساكن بنية sparse_checkout_سیٹ_opts {
	صحيح cone_منوال;
	صحيح sparse_اشاریہ;
	صحيح skip_checks;
	صحيح use_stdin;
} سیٹ_opts;

ساكن صحيح sparse_checkout_سیٹ(صحيح argc, ثابت حرف **argv, ثابت حرف *اگیتر,
			       بنية repository *repo)
{
	بنية repo_config_اقدار *cfg = repo_config_اقدار(the_repository);
	صحيح default_patterns_nr = 2;
	ثابت حرف *default_patterns[] = {"/*", "!/*/", عدم};

	ساكن بنية option builtin_sparse_checkout_سیٹ_options[] = {
		OPT_BOOL(0, "cone", &سیٹ_opts.cone_منوال,
			 N_("initialize the sparse-checkout in cone mode")),
		OPT_BOOL(0, "sparse-index", &سیٹ_opts.sparse_اشاریہ,
			 N_("toggle the use of a sparse index")),
		OPT_BOOL_F(0, "skip-checks", &سیٹ_opts.skip_checks,
			   N_("skip some sanity checks on the given paths that might give false positives"),
			   PARSE_OPT_NONEG),
		OPT_BOOL_F(0, "stdin", &سیٹ_opts.use_stdin,
			   N_("read patterns from standard in"),
			   PARSE_OPT_NONEG),
		OPT_سرا(),
	};
	بنية strvec patterns = STRVEC_INIT;
	صحيح ret;

	تیاری_work_درخت(the_repository);
	repo_read_اشاریہ(repo);

	سیٹ_opts.cone_منوال = -1;
	سیٹ_opts.sparse_اشاریہ = -1;

	argc = parse_options(argc, argv, اگیتر,
			     builtin_sparse_checkout_سیٹ_options,
			     builtin_sparse_checkout_سیٹ_usage, 0);

	إذا (update_modes(repo, &سیٹ_opts.cone_منوال, &سیٹ_opts.sparse_اشاریہ))
		أرجع 1;

	/*
	 * Cone mode automatically specifies the toplevel directory.  For
	 * non-cone mode, if nothing is specified, manually select just the
	 * top-level directory (much as 'init' would do).
	 */
	إذا (!cfg->core_sparse_checkout_cone && !سیٹ_opts.use_stdin && argc == 0) {
		لكل (صحيح i = 0; i < default_patterns_nr; i++)
			strvec_ڈالو(&patterns, default_patterns[i]);
	} وإلا {
		لكل (صحيح i = 0; i < argc; i++)
			strvec_ڈالو(&patterns, argv[i]);
		sanitize_paths(repo, &patterns, اگیتر, سیٹ_opts.skip_checks);
	}

	ret = modify_نمونہ_list(repo, &patterns, سیٹ_opts.use_stdin, REPLACE);

	strvec_clear(&patterns);
	أرجع ret;
}

ساكن حرف ثابت * ثابت builtin_sparse_checkout_reapply_usage[] = {
	"git sparse-checkout reapply [--[no-]cone] [--[no-]sparse-index]",
	عدم
};

ساكن بنية sparse_checkout_reapply_opts {
	صحيح cone_منوال;
	صحيح sparse_اشاریہ;
} reapply_opts;

ساكن صحيح sparse_checkout_reapply(صحيح argc, ثابت حرف **argv,
				   ثابت حرف *اگیتر,
				   بنية repository *repo)
{
	ساكن بنية option builtin_sparse_checkout_reapply_options[] = {
		OPT_BOOL(0, "cone", &reapply_opts.cone_منوال,
			 N_("initialize the sparse-checkout in cone mode")),
		OPT_BOOL(0, "sparse-index", &reapply_opts.sparse_اشاریہ,
			 N_("toggle the use of a sparse index")),
		OPT_سرا(),
	};
	بنية repo_config_اقدار *cfg = repo_config_اقدار(the_repository);

	تیاری_work_درخت(the_repository);
	إذا (!cfg->apply_sparse_checkout)
		die(_("must be in a sparse-checkout to reapply sparsity patterns"));

	reapply_opts.cone_منوال = -1;
	reapply_opts.sparse_اشاریہ = -1;

	argc = parse_options(argc, argv, اگیتر,
			     builtin_sparse_checkout_reapply_options,
			     builtin_sparse_checkout_reapply_usage, 0);

	repo_read_اشاریہ(repo);

	إذا (update_modes(repo, &reapply_opts.cone_منوال, &reapply_opts.sparse_اشاریہ))
		أرجع 1;

	أرجع update_working_directory(repo, عدم);
}

ساكن حرف ثابت * ثابت builtin_sparse_checkout_clean_usage[] = {
	"git sparse-checkout clean [-n|--dry-run]",
	عدم
};

ساكن صحيح list_file_iterator(ثابت حرف *رستہ, ثابت فراغ *انکڑے)
{
	ثابت حرف *پیغام = انکڑے;

	لکھو(پیغام, رستہ);
	أرجع 0;
}

ساكن فراغ list_every_file_وچ_dir(ثابت حرف *پیغام,
				   ثابت حرف *directory)
{
	بنية strbuf رستہ = STRBUF_INIT;

	strbuf_addstr(&رستہ, directory);
	for_each_file_وچ_dir(&رستہ, list_file_iterator, پیغام);
	strbuf_release(&رستہ);
}

ساكن ثابت حرف *پیغام_remove = N_("Removing %s\n");
ساكن ثابت حرف *پیغام_would_remove = N_("Would remove %s\n");

ساكن صحيح sparse_checkout_clean(صحيح argc, ثابت حرف **argv,
				   ثابت حرف *اگیتر,
				   بنية repository *repo)
{
	بنية strbuf بھریا_رستہ = STRBUF_INIT;
	ثابت حرف *پیغام = پیغام_remove;
	ناپ_t worktree_len;
	صحيح force = 0, dry_لگاتار = 0, verbose = 0;
	صحيح require_force = 1;
	بنية repo_config_اقدار *cfg = repo_config_اقدار(the_repository);

	بنية option builtin_sparse_checkout_clean_options[] = {
		OPT_DRY_لگاتار(&dry_لگاتار, N_("dry run")),
		OPT__FORCE(&force, N_("force"), PARSE_OPT_NOCOMPLETE),
		OPT__VERBOSE(&verbose, N_("report each affected file, not just directories")),
		OPT_سرا(),
	};

	تیاری_work_درخت(the_repository);
	إذا (!cfg->apply_sparse_checkout)
		die(_("must be in a sparse-checkout to clean directories"));
	إذا (!cfg->core_sparse_checkout_cone)
		die(_("must be in a cone-mode sparse-checkout to clean directories"));

	argc = parse_options(argc, argv, اگیتر,
			     builtin_sparse_checkout_clean_options,
			     builtin_sparse_checkout_clean_usage, 0);

	repo_config_get_bool(repo, "clean.requireforce", &require_force);
	إذا (require_force && !force && !dry_لگاتار)
		die(_("for safety, refusing to clean without one of --force or --dry-run"));

	إذا (dry_لگاتار)
		پیغام = پیغام_would_remove;

	إذا (repo_read_اشاریہ(repo) < 0)
		die(_("failed to read index"));

	إذا (convert_منزل_sparse(repo->اشاریہ, SPARSE_اشاریہ_حافظہ_ONLY) ||
	    repo->اشاریہ->sparse_اشاریہ == اشاریہ_EXPANDED)
		die(_("failed to convert index to a sparse index; resolve merge conflicts and try again"));

	strbuf_addstr(&بھریا_رستہ, repo->worktree);
	strbuf_addch(&بھریا_رستہ, '/');
	worktree_len = بھریا_رستہ.len;

	لكل (ناپ_t i = 0; i < repo->اشاریہ->cache_nr; i++) {
		بنية cache_اندراج *ce = repo->اشاریہ->cache[i];
		إذا (!S_ISSPARSEDIR(ce->ce_منوال))
			استمر;
		strbuf_setlen(&بھریا_رستہ, worktree_len);
		strbuf_جوڑو(&بھریا_رستہ, ce->name, ce->ce_namelen);

		إذا (!is_directory(بھریا_رستہ.buf))
			استمر;

		إذا (verbose)
			list_every_file_وچ_dir(پیغام, ce->name);
		وإلا
			لکھو(پیغام, ce->name);

		إذا (dry_لگاتار <= 0 &&
		    remove_dir_recursively(&بھریا_رستہ, 0))
			warning_errno(_("failed to remove '%s'"), ce->name);
	}

	strbuf_release(&بھریا_رستہ);
	أرجع 0;
}

ساكن حرف ثابت * ثابت builtin_sparse_checkout_disable_usage[] = {
	"git sparse-checkout disable",
	عدم
};

ساكن صحيح sparse_checkout_disable(صحيح argc, ثابت حرف **argv,
				   ثابت حرف *اگیتر,
				   بنية repository *repo)
{
	ساكن بنية option builtin_sparse_checkout_disable_options[] = {
		OPT_سرا(),
	};
	بنية نمونہ_list pl;
	بنية repo_config_اقدار *cfg = repo_config_اقدار(the_repository);

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

	تیاری_work_درخت(the_repository);
	argc = parse_options(argc, argv, اگیتر,
			     builtin_sparse_checkout_disable_options,
			     builtin_sparse_checkout_disable_usage, 0);

	/*
	 * Disable the advice message for expanding a sparse index, as we
	 * are expecting to do that when disabling sparse-checkout.
	 */
	give_advice_on_expansion = 0;
	repo_read_اشاریہ(repo);

	memset(&pl, 0, حجم(pl));
	hashmap_init(&pl.recursive_hashmap, pl_hashmap_cmp, عدم, 0);
	hashmap_init(&pl.ماپا_hashmap, pl_hashmap_cmp, عدم, 0);
	pl.use_cone_patterns = 0;
	cfg->apply_sparse_checkout = 1;

	جوڑو_نمونہ("/*", خالی_تھاواں_ادھار, 0, &pl, 0);

	prepare_repo_settings(the_repository);
	repo->settings.sparse_اشاریہ = 0;

	إذا (update_working_directory(repo, &pl))
		die(_("error while refreshing working directory"));

	clear_نمونہ_list(&pl);
	أرجع سیٹ_config(repo, منوال_NO_PATTERNS);
}

ساكن حرف ثابت * ثابت builtin_sparse_checkout_check_قاعدے_usage[] = {
	N_("git sparse-checkout check-rules [-z] [--skip-checks]"
	   "[--[no-]cone] [--rules-file <file>]"),
	عدم
};

ساكن بنية sparse_checkout_check_قاعدے_opts {
	صحيح cone_منوال;
	صحيح null_termination;
	حرف *قاعدے_file;
} check_قاعدے_opts;

ساكن صحيح check_قاعدے(بنية repository *repo,
		       بنية نمونہ_list *pl,
		       صحيح null_terminated)
{
	بنية strbuf سطر = STRBUF_INIT;
	بنية strbuf unquoted = STRBUF_INIT;
	حرف *رستہ;
	صحيح سطر_terminator = null_terminated ? 0 : '\n';
	strbuf_getline_fn getline_fn = null_terminated ? strbuf_getline_nul
		: strbuf_getline;
	repo->اشاریہ->sparse_checkout_patterns = pl;
	طالما (!getline_fn(&سطر, stdin)) {
		رستہ = سطر.buf;
		إذا (!null_terminated && سطر.buf[0] == '"') {
			strbuf_reset(&unquoted);
			إذا (unquote_c_style(&unquoted, سطر.buf, عدم))
				die(_("unable to unquote C-style string '%s'"),
					سطر.buf);

			رستہ = unquoted.buf;
		}

		إذا (رستہ_وچ_sparse_checkout(رستہ, repo->اشاریہ))
			write_name_quoted(رستہ, stdout, سطر_terminator);
	}
	strbuf_release(&سطر);
	strbuf_release(&unquoted);

	أرجع 0;
}

ساكن صحيح sparse_checkout_check_قاعدے(صحيح argc, ثابت حرف **argv, ثابت حرف *اگیتر,
				       بنية repository *repo)
{
	ساكن بنية option builtin_sparse_checkout_check_قاعدے_options[] = {
		OPT_BOOL('z', عدم, &check_قاعدے_opts.null_termination,
			 N_("terminate input and output files by a NUL character")),
		OPT_BOOL(0, "cone", &check_قاعدے_opts.cone_منوال,
			 N_("when used with --rules-file interpret patterns as cone mode patterns")),
		OPT_FILENAME(0, "rules-file", &check_قاعدے_opts.قاعدے_file,
			 N_("use patterns in <file> instead of the current ones.")),
		OPT_سرا(),
	};

	FILE *fp;
	صحيح ret;
	بنية نمونہ_list pl = {0};
	بنية repo_config_اقدار *cfg = repo_config_اقدار(the_repository);
	حرف *sparse_filename;
	check_قاعدے_opts.cone_منوال = -1;

	argc = parse_options(argc, argv, اگیتر,
			     builtin_sparse_checkout_check_قاعدے_options,
			     builtin_sparse_checkout_check_قاعدے_usage, 0);

	إذا (check_قاعدے_opts.قاعدے_file && check_قاعدے_opts.cone_منوال < 0)
		check_قاعدے_opts.cone_منوال = 1;

	update_cone_منوال(&check_قاعدے_opts.cone_منوال);
	pl.use_cone_patterns = cfg->core_sparse_checkout_cone;
	إذا (check_قاعدے_opts.قاعدے_file) {
		fp = xfopen(check_قاعدے_opts.قاعدے_file, "r");
		جوڑو_patterns_from_داخلہ(&pl, argc, argv, fp);
		fclose(fp);
	} وإلا {
		sparse_filename = get_sparse_checkout_filename();
		إذا (جوڑو_patterns_from_file_منزل_list(sparse_filename, "", 0, &pl,
						   عدم, 0))
			die(_("unable to load existing sparse-checkout patterns"));
		حرر(sparse_filename);
	}

	ret = check_قاعدے(repo, &pl, check_قاعدے_opts.null_termination);
	clear_نمونہ_list(&pl);
	حرر(check_قاعدے_opts.قاعدے_file);
	أرجع ret;
}

صحيح cmd_sparse_checkout(صحيح argc,
			ثابت حرف **argv,
			ثابت حرف *اگیتر,
			بنية repository *repo)
{
	parse_opt_subcommand_fn *fn = عدم;
	بنية option builtin_sparse_checkout_options[] = {
		OPT_SUBCOMMAND("list", &fn, sparse_checkout_list),
		OPT_SUBCOMMAND("init", &fn, sparse_checkout_init),
		OPT_SUBCOMMAND("set", &fn, sparse_checkout_سیٹ),
		OPT_SUBCOMMAND("add", &fn, sparse_checkout_جوڑو),
		OPT_SUBCOMMAND("reapply", &fn, sparse_checkout_reapply),
		OPT_SUBCOMMAND("clean", &fn, sparse_checkout_clean),
		OPT_SUBCOMMAND("disable", &fn, sparse_checkout_disable),
		OPT_SUBCOMMAND("check-rules", &fn, sparse_checkout_check_قاعدے),
		OPT_سرا(),
	};

	argc = parse_options(argc, argv, اگیتر,
			     builtin_sparse_checkout_options,
			     builtin_sparse_checkout_usage, 0);

	repo_config(the_repository, git_default_config, عدم);

	prepare_repo_settings(repo);
	repo->settings.command_requires_بھریا_اشاریہ = 0;

	أرجع fn(argc, argv, اگیتر, repo);
}
