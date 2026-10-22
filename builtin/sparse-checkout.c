#تعریف_کن USE_THE_REPOSITORY_VARIABLE
#تعریف_کن DISABLE_SIGN_COMPARE_WARNINGS

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

ساكن ثابت حرف *خالی_پایه = "";

ساكن حرف ثابت * ثابت builtin_sparse_checkout_usage[] = {
	N_("git sparse-checkout (init | list | set | add | reapply | disable | check-rules | clean) [<options>]"),
	عدم
};

ساكن فراغ write_patterns_مقصد_file(FILE *fp, بنية الگو_list *pl)
{
	صحيح i;

	لكل (i = 0; i < pl->nr; i++) {
		بنية مسیر_الگو *p = pl->patterns[i];

		إذا (p->flags & الگو_FLAG_NEGATIVE)
			fprintf(fp, "!");

		fprintf(fp, "%s", p->الگو);

		إذا (p->flags & الگو_FLAG_MUSTBEDIR)
			fprintf(fp, "/");

		fprintf(fp, "\n");
	}
}

ساكن حرف ثابت * ثابت builtin_sparse_checkout_list_usage[] = {
	"git sparse-checkout list",
	عدم
};

ساكن صحيح sparse_checkout_list(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
				بنية repository *repo استفادهنشده)
{
	ساكن بنية option builtin_sparse_checkout_list_options[] = {
		OPT_انتها(),
	};
	بنية الگو_list pl;
	حرف *sparse_filename;
	صحيح res;
	بنية repo_config_مقادیر *cfg = repo_config_مقادیر(the_repository);

	راهاندازی_work_درخت(the_repository);
	إذا (!cfg->apply_sparse_checkout)
		die(_("this worktree is not sparse"));

	argc = parse_options(argc, argv, پیشوند,
			     builtin_sparse_checkout_list_options,
			     builtin_sparse_checkout_list_usage, 0);

	memset(&pl, 0, حجم(pl));

	pl.use_cone_patterns = cfg->core_sparse_checkout_cone;

	sparse_filename = get_sparse_checkout_filename();
	res = جمع_کن_patterns_from_file_مقصد_list(sparse_filename, "", 0, &pl, عدم, 0);
	حرر(sparse_filename);

	إذا (res < 0) {
		warning(_("this worktree is not sparse (sparse-checkout file may not exist)"));
		أرجع 0;
	}

	إذا (pl.use_cone_patterns) {
		صحيح i;
		بنية الگو_مدخل *pe;
		بنية hashmap_iter iter;
		بنية string_list sl = STRING_LIST_INIT_DUP;

		hashmap_for_each_مدخل(&pl.recursive_hashmap, &iter, pe, ent) {
			/* pe->pattern starts with "/", skip it */
			string_list_append(&sl, pe->الگو + 1);
		}

		string_list_مرتب_سازی_u(&sl, 0);

		لكل (i = 0; i < sl.nr; i++) {
			quote_c_style(sl.اقلام[i].string, عدم, stdout, 0);
			چاپ_کن("\n");
		}

		string_list_clear(&sl, 0);
	} وإلا {
		write_patterns_مقصد_file(stdout, &pl);
	}

	clear_الگو_list(&pl);

	أرجع 0;
}

ساكن فراغ clean_tracked_sparse_directories(بنية repository *r)
{
	صحيح i, was_پر = 0;
	بنية strbuf مسیر = STRBUF_INIT;
	اندازه_t pathlen;
	بنية string_list_قلم *قلم;
	بنية string_list sparse_dirs = STRING_LIST_INIT_DUP;

	/*
	 * If we are not using cone mode patterns, then we cannot
	 * delete directories outside of the sparse cone.
	 */
	إذا (!r || !r->شاخص || !r->worktree)
		أرجع;
	إذا (init_sparse_checkout_patterns(r->شاخص) ||
	    !r->شاخص->sparse_checkout_patterns->use_cone_patterns)
		أرجع;

	/*
	 * Use the sparse index as a data structure to assist finding
	 * directories that are safe to delete. This conversion to a
	 * sparse index will not delete directories that contain
	 * conflicted entries or submodules.
	 */
	إذا (r->شاخص->sparse_شاخص == شاخص_EXPANDED) {
		/*
		 * If something, such as a merge conflict or other concern,
		 * prevents us from converting to a sparse index, then do
		 * not try deleting files.
		 */
		إذا (convert_مقصد_sparse(r->شاخص, SPARSE_شاخص_حافظه_ONLY))
			أرجع;
		was_پر = 1;
	}

	strbuf_addstr(&مسیر, r->worktree);
	strbuf_complete(&مسیر, '/');
	pathlen = مسیر.len;

	/*
	 * Collect directories that have gone out of scope but also
	 * exist on disk, so there is some work to be done. We need to
	 * store the entries in a list before exploring, since that might
	 * expand the sparse-index again.
	 */
	لكل (i = 0; i < r->شاخص->cache_nr; i++) {
		بنية cache_مدخل *ce = r->شاخص->cache[i];

		إذا (S_ISSPARSEDIR(ce->ce_نما) &&
		    repo_file_exists(r, ce->name))
			string_list_append(&sparse_dirs, ce->name);
	}

	for_each_string_list_قلم(قلم, &sparse_dirs) {
		بنية dir_struct dir = DIR_INIT;
		بنية pathspec p = { 0 };
		بنية strvec s = STRVEC_INIT;

		strbuf_setlen(&مسیر, pathlen);
		strbuf_addstr(&مسیر, قلم->string);

		dir.flags |= DIR_SHOW_IGNORED_TOO;

		راهاندازی_standard_excludes(&dir);
		strvec_افزودن(&s, مسیر.buf);

		parse_pathspec(&p, PATHSPEC_GLOB, 0, عدم, s.v);
		fill_directory(&dir, r->شاخص, &p);

		إذا (dir.nr) {
			warning(_("directory '%s' contains untracked files,"
				  " but is not in the sparse-checkout cone"),
				قلم->string);
		} وإلا إذا (remove_dir_recursively(&مسیر, 0)) {
			/*
			 * Removal is "best effort". If something blocks
			 * the deletion, then continue with a warning.
			 */
			warning(_("failed to remove directory '%s'"),
				قلم->string);
		}

		strvec_clear(&s);
		clear_pathspec(&p);
		dir_clear(&dir);
	}

	string_list_clear(&sparse_dirs, 0);
	strbuf_release(&مسیر);

	إذا (was_پر)
		ensure_پر_شاخص(r->شاخص);
}

ساكن صحيح update_working_directory(بنية repository *r,
				    بنية الگو_list *pl)
{
	تعداد update_sparsity_نتیجه نتیجه;
	بنية unpack_trees_options o;
	بنية قفل_file قفل_file = قفل_INIT;
	بنية الگو_list *old_pl;

	/* If no branch has been checked out, there are no updates to make. */
	إذا (is_شاخص_unborn(r->شاخص))
		أرجع UPDATE_SPARSITY_SUCCESS;

	old_pl = r->شاخص->sparse_checkout_patterns;
	r->شاخص->sparse_checkout_patterns = pl;

	memset(&o, 0, حجم(o));
	o.verbose_update = isatty(2);
	o.update = 1;
	o.سر_idx = -1;
	o.src_شاخص = r->شاخص;
	o.dst_شاخص = r->شاخص;
	o.skip_sparse_checkout = 0;

	راهاندازی_work_درخت(the_repository);

	repo_hold_locked_شاخص(r, &قفل_file, قفل_DIE_ON_ERROR);

	راهاندازی_unpack_trees_porcelain(&o, "sparse-checkout");
	نتیجه = update_sparsity(&o, pl);
	clear_unpack_trees_porcelain(&o);

	إذا (نتیجه == UPDATE_SPARSITY_WARNINGS)
		/*
		 * We don't do any special handling of warnings from untracked
		 * files in the way or dirty entries that can't be removed.
		 */
		نتیجه = UPDATE_SPARSITY_SUCCESS;
	إذا (نتیجه == UPDATE_SPARSITY_SUCCESS)
		write_locked_شاخص(r->شاخص, &قفل_file, COMMIT_قفل);
	وإلا
		rollback_قفل_file(&قفل_file);

	clean_tracked_sparse_directories(r);

	إذا (r->شاخص->sparse_checkout_patterns != pl) {
		clear_الگو_list(r->شاخص->sparse_checkout_patterns);
		FREE_AND_NULL(r->شاخص->sparse_checkout_patterns);
	}
	r->شاخص->sparse_checkout_patterns = old_pl;

	أرجع نتیجه;
}

ساكن حرف *escaped_الگو(حرف *الگو)
{
	حرف *p = الگو;
	بنية strbuf final = STRBUF_INIT;

	طالما (*p) {
		إذا (is_glob_special(*p))
			strbuf_addch(&final, '\\');

		strbuf_addch(&final, *p);
		p++;
	}

	أرجع strbuf_detach(&final, عدم);
}

ساكن فراغ write_cone_مقصد_file(FILE *fp, بنية الگو_list *pl)
{
	صحيح i;
	بنية الگو_مدخل *pe;
	بنية hashmap_iter iter;
	بنية string_list sl = STRING_LIST_INIT_DUP;
	بنية strbuf والد_الگو = STRBUF_INIT;

	hashmap_for_each_مدخل(&pl->والد_hashmap, &iter, pe, ent) {
		إذا (hashmap_get_مدخل(&pl->recursive_hashmap, pe, ent, عدم))
			استمر;

		إذا (!hashmap_شامل_است_والد(&pl->recursive_hashmap,
					     pe->الگو,
					     &والد_الگو))
			string_list_append(&sl, pe->الگو);
	}

	string_list_مرتب_سازی_u(&sl, 0);

	fprintf(fp, "/*\n!/*/\n");

	لكل (i = 0; i < sl.nr; i++) {
		حرف *الگو = escaped_الگو(sl.اقلام[i].string);

		إذا (طول_رشته(الگو))
			fprintf(fp, "%s/\n!%s/*/\n", الگو, الگو);
		حرر(الگو);
	}

	string_list_clear(&sl, 0);

	hashmap_for_each_مدخل(&pl->recursive_hashmap, &iter, pe, ent) {
		إذا (!hashmap_شامل_است_والد(&pl->recursive_hashmap,
					     pe->الگو,
					     &والد_الگو))
			string_list_append(&sl, pe->الگو);
	}

	strbuf_release(&والد_الگو);

	string_list_مرتب_سازی_u(&sl, 0);

	لكل (i = 0; i < sl.nr; i++) {
		حرف *الگو = escaped_الگو(sl.اقلام[i].string);
		fprintf(fp, "%s/\n", الگو);
		حرر(الگو);
	}

	string_list_clear(&sl, 0);
}

ساكن صحيح write_patterns_and_update(بنية repository *repo,
				     بنية الگو_list *pl)
{
	حرف *sparse_filename;
	FILE *fp;
	بنية قفل_file lk = قفل_INIT;
	صحيح نتیجه;
	بنية repo_config_مقادیر *cfg = repo_config_مقادیر(the_repository);

	sparse_filename = get_sparse_checkout_filename();

	إذا (امن_create_leading_directories(repo, sparse_filename))
		die(_("failed to create directory for sparse-checkout file"));

	repo_hold_قفل_file_for_update(repo, &lk, sparse_filename,
				       قفل_DIE_ON_ERROR);

	نتیجه = update_working_directory(repo, pl);
	إذا (نتیجه) {
		rollback_قفل_file(&lk);
		update_working_directory(repo, عدم);
		اذهب بیرون;
	}

	fp = fdopen_قفل_file(&lk, "w");
	إذا (!fp)
		die_errno(_("unable to fdopen %s"), get_قفل_file_مسیر(&lk));

	إذا (cfg->core_sparse_checkout_cone)
		write_cone_مقصد_file(fp, pl);
	وإلا
		write_patterns_مقصد_file(fp, pl);

	إذا (commit_قفل_file(&lk))
		die_errno(_("unable to write %s"), sparse_filename);

بیرون:
	clear_الگو_list(pl);
	حرر(sparse_filename);
	أرجع نتیجه;
}

تعداد sparse_checkout_نما {
	نما_NO_PATTERNS = 0,
	نما_ALL_PATTERNS = 1,
	نما_CONE_PATTERNS = 2,
};

ساكن صحيح مجموعه_config(بنية repository *repo,
		      تعداد sparse_checkout_نما نما)
{
	/* Update to use worktree config, if not already. */
	إذا (init_worktree_config(repo)) {
		error(_("failed to initialize worktree config"));
		أرجع 1;
	}

	إذا (repo_config_مجموعه_worktree_gently(repo,
					    "core.sparseCheckout",
					    نما ? "true" : "false") ||
	    repo_config_مجموعه_worktree_gently(repo,
					    "core.sparseCheckoutCone",
					    نما == نما_CONE_PATTERNS ?
						"true" : "false"))
		أرجع 1;

	إذا (نما == نما_NO_PATTERNS)
		أرجع مجموعه_sparse_شاخص_config(repo, 0);

	أرجع 0;
}

ساكن تعداد sparse_checkout_نما update_cone_نما(صحيح *cone_نما) {
	بنية repo_config_مقادیر *cfg = repo_config_مقادیر(the_repository);

	/* If not specified, use previous definition of cone mode */
	إذا (*cone_نما == -1 && cfg->apply_sparse_checkout)
		*cone_نما = cfg->core_sparse_checkout_cone;

	/* Set cone/non-cone mode appropriately */
	cfg->apply_sparse_checkout = 1;
	إذا (*cone_نما == 1 || *cone_نما == -1) {
		cfg->core_sparse_checkout_cone = 1;
		أرجع نما_CONE_PATTERNS;
	}
	cfg->core_sparse_checkout_cone = 0;
	أرجع نما_ALL_PATTERNS;
}

ساكن صحيح update_modes(بنية repository *repo, صحيح *cone_نما, صحيح *sparse_شاخص)
{
	صحيح نما, رکورد_نما;
	بنية repo_config_مقادیر *cfg = repo_config_مقادیر(the_repository);

	/* Determine if we need to record the mode; ensure sparse checkout on */
	رکورد_نما = (*cone_نما != -1) || !cfg->apply_sparse_checkout;

	نما = update_cone_نما(cone_نما);
	إذا (رکورد_نما && مجموعه_config(repo, نما))
		أرجع 1;

	/* Set sparse-index/non-sparse-index mode if specified */
	إذا (*sparse_شاخص >= 0) {
		إذا (مجموعه_sparse_شاخص_config(repo, *sparse_شاخص) < 0)
			die(_("failed to modify sparse-index config"));

		/* force an index rewrite */
		repo_read_شاخص(repo);
		repo->شاخص->updated_workdir = 1;

		إذا (!*sparse_شاخص)
			ensure_پر_شاخص(repo->شاخص);
	}

	أرجع 0;
}

ساكن حرف ثابت * ثابت builtin_sparse_checkout_init_usage[] = {
	"git sparse-checkout init [--cone] [--[no-]sparse-index]",
	عدم
};

ساكن بنية sparse_checkout_init_opts {
	صحيح cone_نما;
	صحيح sparse_شاخص;
} init_opts;

ساكن صحيح sparse_checkout_init(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
				بنية repository *repo)
{
	بنية الگو_list pl;
	حرف *sparse_filename;
	صحيح res;
	بنية object_id oid;

	ساكن بنية option builtin_sparse_checkout_init_options[] = {
		OPT_BOOL(0, "cone", &init_opts.cone_نما,
			 N_("initialize the sparse-checkout in cone mode")),
		OPT_BOOL(0, "sparse-index", &init_opts.sparse_شاخص,
			 N_("toggle the use of a sparse index")),
		OPT_انتها(),
	};

	راهاندازی_work_درخت(the_repository);
	repo_read_شاخص(repo);

	init_opts.cone_نما = -1;
	init_opts.sparse_شاخص = -1;

	argc = parse_options(argc, argv, پیشوند,
			     builtin_sparse_checkout_init_options,
			     builtin_sparse_checkout_init_usage, 0);

	إذا (update_modes(repo, &init_opts.cone_نما, &init_opts.sparse_شاخص))
		أرجع 1;

	memset(&pl, 0, حجم(pl));

	sparse_filename = get_sparse_checkout_filename();
	res = جمع_کن_patterns_from_file_مقصد_list(sparse_filename, "", 0, &pl, عدم, 0);

	/* If we already have a sparse-checkout file, use it. */
	إذا (res >= 0) {
		حرر(sparse_filename);
		clear_الگو_list(&pl);
		أرجع update_working_directory(repo, عدم);
	}

	إذا (repo_get_oid(repo, "HEAD", &oid)) {
		FILE *fp;

		/* assume we are in a fresh repo, but update the sparse-checkout file */
		إذا (امن_create_leading_directories(repo, sparse_filename))
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

	جمع_کن_الگو("/*", خالی_پایه, 0, &pl, 0);
	جمع_کن_الگو("!/*/", خالی_پایه, 0, &pl, 0);
	pl.use_cone_patterns = init_opts.cone_نما;

	أرجع write_patterns_and_update(repo, &pl);
}

ساكن فراغ درج_recursive_الگو(بنية الگو_list *pl, بنية strbuf *مسیر)
{
	بنية الگو_مدخل *e = xmalloc(حجم(*e));
	e->patternlen = مسیر->len;
	e->الگو = strbuf_detach(مسیر, عدم);
	hashmap_مدخل_init(&e->ent, fspathhash(e->الگو));

	hashmap_جمع_کن(&pl->recursive_hashmap, &e->ent);

	طالما (e->patternlen) {
		حرف *slash = strrchr(e->الگو, '/');
		حرف *oldpattern = e->الگو;
		اندازه_t newlen;
		بنية الگو_مدخل *dup;

		إذا (!slash || slash == e->الگو)
			اكسر;

		newlen = slash - e->الگو;
		e = xmalloc(حجم(بنية الگو_مدخل));
		e->patternlen = newlen;
		e->الگو = xstrndup(oldpattern, newlen);
		hashmap_مدخل_init(&e->ent, fspathhash(e->الگو));

		dup = hashmap_get_مدخل(&pl->والد_hashmap, e, ent, عدم);
		إذا (!dup) {
			hashmap_جمع_کن(&pl->والد_hashmap, &e->ent);
		} وإلا {
			حرر(e->الگو);
			حرر(e);
			e = dup;
		}
	}
}

ساكن فراغ strbuf_مقصد_cone_الگو(بنية strbuf *سطر, بنية الگو_list *pl)
{
	strbuf_trim(سطر);

	strbuf_trim_trailing_dir_sep(سطر);

	إذا (strbuf_normalize_مسیر(سطر))
		die(_("could not normalize path %s"), سطر->buf);

	إذا (!سطر->len)
		أرجع;

	إذا (سطر->buf[0] != '/')
		strbuf_insertstr(سطر, 0, "/");

	درج_recursive_الگو(pl, سطر);
}

ساكن فراغ جمع_کن_patterns_from_درون_داد(بنية الگو_list *pl,
				    صحيح argc, ثابت حرف **argv,
				    FILE *file)
{
	صحيح i;
	بنية repo_config_مقادیر *cfg = repo_config_مقادیر(the_repository);

	إذا (cfg->core_sparse_checkout_cone) {
		بنية strbuf سطر = STRBUF_INIT;

		hashmap_init(&pl->recursive_hashmap, pl_hashmap_cmp, عدم, 0);
		hashmap_init(&pl->والد_hashmap, pl_hashmap_cmp, عدم, 0);
		pl->use_cone_patterns = 1;

		إذا (file) {
			بنية strbuf unquoted = STRBUF_INIT;
			طالما (!strbuf_getline(&سطر, file)) {
				إذا (سطر.buf[0] == '"') {
					strbuf_reset(&unquoted);
					إذا (unquote_c_style(&unquoted, سطر.buf, عدم))
						die(_("unable to unquote C-style string '%s'"),
						سطر.buf);

					strbuf_جابجا(&unquoted, &سطر);
				}

				strbuf_مقصد_cone_الگو(&سطر, pl);
			}

			strbuf_release(&unquoted);
		} وإلا {
			لكل (i = 0; i < argc; i++) {
				strbuf_setlen(&سطر, 0);
				strbuf_addstr(&سطر, argv[i]);
				strbuf_مقصد_cone_الگو(&سطر, pl);
			}
		}
		strbuf_release(&سطر);
	} وإلا {
		إذا (file) {
			بنية strbuf سطر = STRBUF_INIT;

			طالما (!strbuf_getline(&سطر, file))
				جمع_کن_الگو(سطر.buf, خالی_پایه, 0, pl, 0);

			strbuf_release(&سطر);
		} وإلا {
			لكل (i = 0; i < argc; i++)
				جمع_کن_الگو(argv[i], خالی_پایه, 0, pl, 0);
		}
	}
}

تعداد modify_type {
	REPLACE,
	جمع_کن_2,
};

ساكن فراغ جمع_کن_patterns_cone_نما(صحيح argc, ثابت حرف **argv,
				   بنية الگو_list *pl,
				   صحيح use_stdin)
{
	بنية strbuf میانگیر = STRBUF_INIT;
	بنية الگو_مدخل *pe;
	بنية hashmap_iter iter;
	بنية الگو_list existing;
	بنية repo_config_مقادیر *cfg = repo_config_مقادیر(the_repository);
	حرف *sparse_filename = get_sparse_checkout_filename();

	جمع_کن_patterns_from_درون_داد(pl, argc, argv,
				use_stdin ? stdin : عدم);

	memset(&existing, 0, حجم(existing));
	existing.use_cone_patterns = cfg->core_sparse_checkout_cone;

	إذا (جمع_کن_patterns_from_file_مقصد_list(sparse_filename, "", 0,
					   &existing, عدم, 0))
		die(_("unable to load existing sparse-checkout patterns"));
	حرر(sparse_filename);

	إذا (!existing.use_cone_patterns)
		die(_("existing sparse-checkout patterns do not use cone mode"));

	hashmap_for_each_مدخل(&existing.recursive_hashmap, &iter, pe, ent) {
		إذا (!hashmap_شامل_است_والد(&pl->recursive_hashmap,
					pe->الگو, &میانگیر) ||
		    !hashmap_شامل_است_والد(&pl->والد_hashmap,
					pe->الگو, &میانگیر)) {
			strbuf_reset(&میانگیر);
			strbuf_addstr(&میانگیر, pe->الگو);
			درج_recursive_الگو(pl, &میانگیر);
		}
	}

	clear_الگو_list(&existing);
	strbuf_release(&میانگیر);
}

ساكن فراغ جمع_کن_patterns_literal(صحيح argc, ثابت حرف **argv,
				 بنية الگو_list *pl,
				 صحيح use_stdin)
{
	حرف *sparse_filename = get_sparse_checkout_filename();
	إذا (جمع_کن_patterns_from_file_مقصد_list(sparse_filename, "", 0,
					   pl, عدم, 0))
		die(_("unable to load existing sparse-checkout patterns"));
	حرر(sparse_filename);
	جمع_کن_patterns_from_درون_داد(pl, argc, argv, use_stdin ? stdin : عدم);
}

ساكن صحيح modify_الگو_list(بنية repository *repo,
			       بنية strvec *آرگومانها, صحيح use_stdin,
			       تعداد modify_type m)
{
	صحيح نتیجه;
	صحيح تغییریافته_config = 0;
	بنية الگو_list *pl = xcalloc(1, حجم(*pl));
	بنية repo_config_مقادیر *cfg = repo_config_مقادیر(the_repository);

	بدل (m) {
	حالة جمع_کن_2:
		إذا (cfg->core_sparse_checkout_cone)
			جمع_کن_patterns_cone_نما(آرگومانها->nr, آرگومانها->v, pl, use_stdin);
		وإلا
			جمع_کن_patterns_literal(آرگومانها->nr, آرگومانها->v, pl, use_stdin);
		اكسر;

	حالة REPLACE:
		جمع_کن_patterns_from_درون_داد(pl, آرگومانها->nr, آرگومانها->v,
					use_stdin ? stdin : عدم);
		اكسر;
	}

	إذا (!cfg->apply_sparse_checkout) {
		مجموعه_config(repo, نما_ALL_PATTERNS);
		cfg->apply_sparse_checkout = 1;
		تغییریافته_config = 1;
	}

	نتیجه = write_patterns_and_update(repo, pl);

	إذا (نتیجه && تغییریافته_config)
		مجموعه_config(repo, نما_NO_PATTERNS);

	clear_الگو_list(pl);
	حرر(pl);
	أرجع نتیجه;
}

ساكن فراغ sanitize_paths(بنية repository *repo,
			   بنية strvec *آرگومانها,
			   ثابت حرف *پیشوند, صحيح skip_checks)
{
	صحيح i;
	بنية repo_config_مقادیر *cfg = repo_config_مقادیر(the_repository);

	إذا (!آرگومانها->nr)
		أرجع;

	إذا (پیشوند && *پیشوند && cfg->core_sparse_checkout_cone) {
		/*
		 * The args are not pathspecs, so unfortunately we
		 * cannot imitate how cmd_add() uses parse_pathspec().
		 */
		صحيح پیشوند_len = طول_رشته(پیشوند);

		لكل (i = 0; i < آرگومانها->nr; i++) {
			حرف *prefixed_مسیر = پیشوند_مسیر(the_repository, پیشوند,
							  پیشوند_len, آرگومانها->v[i]);
			strvec_replace(آرگومانها, i, prefixed_مسیر);
			حرر(prefixed_مسیر);
		}
	}

	إذا (skip_checks)
		أرجع;

	إذا (پیشوند && *پیشوند && !cfg->core_sparse_checkout_cone)
		die(_("please run from the toplevel directory in non-cone mode"));

	إذا (cfg->core_sparse_checkout_cone) {
		لكل (i = 0; i < آرگومانها->nr; i++) {
			إذا (آرگومانها->v[i][0] == '/')
				die(_("specify directories rather than patterns (no leading slash)"));
			إذا (آرگومانها->v[i][0] == '!')
				die(_("specify directories rather than patterns.  If your directory starts with a '!', pass --skip-checks"));
			إذا (strpbrk(آرگومانها->v[i], "*?[]"))
				die(_("specify directories rather than patterns.  If your directory really has any of '*?[]\\' in it, pass --skip-checks"));
		}
	}

	لكل (i = 0; i < آرگومانها->nr; i++) {
		بنية cache_مدخل *ce;
		بنية شاخص_state *شاخص = repo->شاخص;
		صحيح pos = شاخص_name_pos(شاخص, آرگومانها->v[i], طول_رشته(آرگومانها->v[i]));

		إذا (pos < 0)
			استمر;
		ce = شاخص->cache[pos];
		إذا (S_ISSPARSEDIR(ce->ce_نما))
			استمر;

		إذا (cfg->core_sparse_checkout_cone)
			die(_("'%s' is not a directory; to treat it as a directory anyway, rerun with --skip-checks"), آرگومانها->v[i]);
		وإلا
			warning(_("pass a leading slash before paths such as '%s' if you want a single file (see NON-CONE PROBLEMS in the git-sparse-checkout manual)."), آرگومانها->v[i]);
	}
}

ساكن حرف ثابت * ثابت builtin_sparse_checkout_جمع_کن_usage[] = {
	N_("git sparse-checkout add [--skip-checks] (--stdin | <patterns>)"),
	عدم
};

ساكن بنية sparse_checkout_جمع_کن_opts {
	صحيح skip_checks;
	صحيح use_stdin;
} جمع_کن_opts;

ساكن صحيح sparse_checkout_جمع_کن(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
			       بنية repository *repo)
{
	ساكن بنية option builtin_sparse_checkout_جمع_کن_options[] = {
		OPT_BOOL_F(0, "skip-checks", &جمع_کن_opts.skip_checks,
			   N_("skip some sanity checks on the given paths that might give false positives"),
			   PARSE_OPT_NONEG),
		OPT_BOOL(0, "stdin", &جمع_کن_opts.use_stdin,
			 N_("read patterns from standard in")),
		OPT_انتها(),
	};
	بنية strvec patterns = STRVEC_INIT;
	صحيح ret;
	بنية repo_config_مقادیر *cfg = repo_config_مقادیر(the_repository);

	راهاندازی_work_درخت(the_repository);
	إذا (!cfg->apply_sparse_checkout)
		die(_("no sparse-checkout to add to"));

	repo_read_شاخص(repo);

	argc = parse_options(argc, argv, پیشوند,
			     builtin_sparse_checkout_جمع_کن_options,
			     builtin_sparse_checkout_جمع_کن_usage, 0);

	لكل (صحيح i = 0; i < argc; i++)
		strvec_افزودن(&patterns, argv[i]);
	sanitize_paths(repo, &patterns, پیشوند, جمع_کن_opts.skip_checks);

	ret = modify_الگو_list(repo, &patterns, جمع_کن_opts.use_stdin, جمع_کن_2);

	strvec_clear(&patterns);
	أرجع ret;
}

ساكن حرف ثابت * ثابت builtin_sparse_checkout_مجموعه_usage[] = {
	N_("git sparse-checkout set [--[no-]cone] [--[no-]sparse-index] [--skip-checks] (--stdin | <patterns>)"),
	عدم
};

ساكن بنية sparse_checkout_مجموعه_opts {
	صحيح cone_نما;
	صحيح sparse_شاخص;
	صحيح skip_checks;
	صحيح use_stdin;
} مجموعه_opts;

ساكن صحيح sparse_checkout_مجموعه(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
			       بنية repository *repo)
{
	بنية repo_config_مقادیر *cfg = repo_config_مقادیر(the_repository);
	صحيح default_patterns_nr = 2;
	ثابت حرف *default_patterns[] = {"/*", "!/*/", عدم};

	ساكن بنية option builtin_sparse_checkout_مجموعه_options[] = {
		OPT_BOOL(0, "cone", &مجموعه_opts.cone_نما,
			 N_("initialize the sparse-checkout in cone mode")),
		OPT_BOOL(0, "sparse-index", &مجموعه_opts.sparse_شاخص,
			 N_("toggle the use of a sparse index")),
		OPT_BOOL_F(0, "skip-checks", &مجموعه_opts.skip_checks,
			   N_("skip some sanity checks on the given paths that might give false positives"),
			   PARSE_OPT_NONEG),
		OPT_BOOL_F(0, "stdin", &مجموعه_opts.use_stdin,
			   N_("read patterns from standard in"),
			   PARSE_OPT_NONEG),
		OPT_انتها(),
	};
	بنية strvec patterns = STRVEC_INIT;
	صحيح ret;

	راهاندازی_work_درخت(the_repository);
	repo_read_شاخص(repo);

	مجموعه_opts.cone_نما = -1;
	مجموعه_opts.sparse_شاخص = -1;

	argc = parse_options(argc, argv, پیشوند,
			     builtin_sparse_checkout_مجموعه_options,
			     builtin_sparse_checkout_مجموعه_usage, 0);

	إذا (update_modes(repo, &مجموعه_opts.cone_نما, &مجموعه_opts.sparse_شاخص))
		أرجع 1;

	/*
	 * Cone mode automatically specifies the toplevel directory.  For
	 * non-cone mode, if nothing is specified, manually select just the
	 * top-level directory (much as 'init' would do).
	 */
	إذا (!cfg->core_sparse_checkout_cone && !مجموعه_opts.use_stdin && argc == 0) {
		لكل (صحيح i = 0; i < default_patterns_nr; i++)
			strvec_افزودن(&patterns, default_patterns[i]);
	} وإلا {
		لكل (صحيح i = 0; i < argc; i++)
			strvec_افزودن(&patterns, argv[i]);
		sanitize_paths(repo, &patterns, پیشوند, مجموعه_opts.skip_checks);
	}

	ret = modify_الگو_list(repo, &patterns, مجموعه_opts.use_stdin, REPLACE);

	strvec_clear(&patterns);
	أرجع ret;
}

ساكن حرف ثابت * ثابت builtin_sparse_checkout_reapply_usage[] = {
	"git sparse-checkout reapply [--[no-]cone] [--[no-]sparse-index]",
	عدم
};

ساكن بنية sparse_checkout_reapply_opts {
	صحيح cone_نما;
	صحيح sparse_شاخص;
} reapply_opts;

ساكن صحيح sparse_checkout_reapply(صحيح argc, ثابت حرف **argv,
				   ثابت حرف *پیشوند,
				   بنية repository *repo)
{
	ساكن بنية option builtin_sparse_checkout_reapply_options[] = {
		OPT_BOOL(0, "cone", &reapply_opts.cone_نما,
			 N_("initialize the sparse-checkout in cone mode")),
		OPT_BOOL(0, "sparse-index", &reapply_opts.sparse_شاخص,
			 N_("toggle the use of a sparse index")),
		OPT_انتها(),
	};
	بنية repo_config_مقادیر *cfg = repo_config_مقادیر(the_repository);

	راهاندازی_work_درخت(the_repository);
	إذا (!cfg->apply_sparse_checkout)
		die(_("must be in a sparse-checkout to reapply sparsity patterns"));

	reapply_opts.cone_نما = -1;
	reapply_opts.sparse_شاخص = -1;

	argc = parse_options(argc, argv, پیشوند,
			     builtin_sparse_checkout_reapply_options,
			     builtin_sparse_checkout_reapply_usage, 0);

	repo_read_شاخص(repo);

	إذا (update_modes(repo, &reapply_opts.cone_نما, &reapply_opts.sparse_شاخص))
		أرجع 1;

	أرجع update_working_directory(repo, عدم);
}

ساكن حرف ثابت * ثابت builtin_sparse_checkout_clean_usage[] = {
	"git sparse-checkout clean [-n|--dry-run]",
	عدم
};

ساكن صحيح list_file_iterator(ثابت حرف *مسیر, ثابت فراغ *داده)
{
	ثابت حرف *پیام = داده;

	چاپ_کن(پیام, مسیر);
	أرجع 0;
}

ساكن فراغ list_every_file_درون_dir(ثابت حرف *پیام,
				   ثابت حرف *directory)
{
	بنية strbuf مسیر = STRBUF_INIT;

	strbuf_addstr(&مسیر, directory);
	for_each_file_درون_dir(&مسیر, list_file_iterator, پیام);
	strbuf_release(&مسیر);
}

ساكن ثابت حرف *پیام_remove = N_("Removing %s\n");
ساكن ثابت حرف *پیام_would_remove = N_("Would remove %s\n");

ساكن صحيح sparse_checkout_clean(صحيح argc, ثابت حرف **argv,
				   ثابت حرف *پیشوند,
				   بنية repository *repo)
{
	بنية strbuf پر_مسیر = STRBUF_INIT;
	ثابت حرف *پیام = پیام_remove;
	اندازه_t worktree_len;
	صحيح force = 0, dry_پیاپی = 0, verbose = 0;
	صحيح require_force = 1;
	بنية repo_config_مقادیر *cfg = repo_config_مقادیر(the_repository);

	بنية option builtin_sparse_checkout_clean_options[] = {
		OPT_DRY_پیاپی(&dry_پیاپی, N_("dry run")),
		OPT__FORCE(&force, N_("force"), PARSE_OPT_NOCOMPLETE),
		OPT__VERBOSE(&verbose, N_("report each affected file, not just directories")),
		OPT_انتها(),
	};

	راهاندازی_work_درخت(the_repository);
	إذا (!cfg->apply_sparse_checkout)
		die(_("must be in a sparse-checkout to clean directories"));
	إذا (!cfg->core_sparse_checkout_cone)
		die(_("must be in a cone-mode sparse-checkout to clean directories"));

	argc = parse_options(argc, argv, پیشوند,
			     builtin_sparse_checkout_clean_options,
			     builtin_sparse_checkout_clean_usage, 0);

	repo_config_get_bool(repo, "clean.requireforce", &require_force);
	إذا (require_force && !force && !dry_پیاپی)
		die(_("for safety, refusing to clean without one of --force or --dry-run"));

	إذا (dry_پیاپی)
		پیام = پیام_would_remove;

	إذا (repo_read_شاخص(repo) < 0)
		die(_("failed to read index"));

	إذا (convert_مقصد_sparse(repo->شاخص, SPARSE_شاخص_حافظه_ONLY) ||
	    repo->شاخص->sparse_شاخص == شاخص_EXPANDED)
		die(_("failed to convert index to a sparse index; resolve merge conflicts and try again"));

	strbuf_addstr(&پر_مسیر, repo->worktree);
	strbuf_addch(&پر_مسیر, '/');
	worktree_len = پر_مسیر.len;

	لكل (اندازه_t i = 0; i < repo->شاخص->cache_nr; i++) {
		بنية cache_مدخل *ce = repo->شاخص->cache[i];
		إذا (!S_ISSPARSEDIR(ce->ce_نما))
			استمر;
		strbuf_setlen(&پر_مسیر, worktree_len);
		strbuf_جمع_کن(&پر_مسیر, ce->name, ce->ce_namelen);

		إذا (!is_directory(پر_مسیر.buf))
			استمر;

		إذا (verbose)
			list_every_file_درون_dir(پیام, ce->name);
		وإلا
			چاپ_کن(پیام, ce->name);

		إذا (dry_پیاپی <= 0 &&
		    remove_dir_recursively(&پر_مسیر, 0))
			warning_errno(_("failed to remove '%s'"), ce->name);
	}

	strbuf_release(&پر_مسیر);
	أرجع 0;
}

ساكن حرف ثابت * ثابت builtin_sparse_checkout_disable_usage[] = {
	"git sparse-checkout disable",
	عدم
};

ساكن صحيح sparse_checkout_disable(صحيح argc, ثابت حرف **argv,
				   ثابت حرف *پیشوند,
				   بنية repository *repo)
{
	ساكن بنية option builtin_sparse_checkout_disable_options[] = {
		OPT_انتها(),
	};
	بنية الگو_list pl;
	بنية repo_config_مقادیر *cfg = repo_config_مقادیر(the_repository);

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

	راهاندازی_work_درخت(the_repository);
	argc = parse_options(argc, argv, پیشوند,
			     builtin_sparse_checkout_disable_options,
			     builtin_sparse_checkout_disable_usage, 0);

	/*
	 * Disable the advice message for expanding a sparse index, as we
	 * are expecting to do that when disabling sparse-checkout.
	 */
	give_advice_on_expansion = 0;
	repo_read_شاخص(repo);

	memset(&pl, 0, حجم(pl));
	hashmap_init(&pl.recursive_hashmap, pl_hashmap_cmp, عدم, 0);
	hashmap_init(&pl.والد_hashmap, pl_hashmap_cmp, عدم, 0);
	pl.use_cone_patterns = 0;
	cfg->apply_sparse_checkout = 1;

	جمع_کن_الگو("/*", خالی_پایه, 0, &pl, 0);

	prepare_repo_settings(the_repository);
	repo->settings.sparse_شاخص = 0;

	إذا (update_working_directory(repo, &pl))
		die(_("error while refreshing working directory"));

	clear_الگو_list(&pl);
	أرجع مجموعه_config(repo, نما_NO_PATTERNS);
}

ساكن حرف ثابت * ثابت builtin_sparse_checkout_check_قواعد_usage[] = {
	N_("git sparse-checkout check-rules [-z] [--skip-checks]"
	   "[--[no-]cone] [--rules-file <file>]"),
	عدم
};

ساكن بنية sparse_checkout_check_قواعد_opts {
	صحيح cone_نما;
	صحيح null_termination;
	حرف *قواعد_file;
} check_قواعد_opts;

ساكن صحيح check_قواعد(بنية repository *repo,
		       بنية الگو_list *pl,
		       صحيح null_terminated)
{
	بنية strbuf سطر = STRBUF_INIT;
	بنية strbuf unquoted = STRBUF_INIT;
	حرف *مسیر;
	صحيح سطر_terminator = null_terminated ? 0 : '\n';
	strbuf_getline_fn getline_fn = null_terminated ? strbuf_getline_nul
		: strbuf_getline;
	repo->شاخص->sparse_checkout_patterns = pl;
	طالما (!getline_fn(&سطر, stdin)) {
		مسیر = سطر.buf;
		إذا (!null_terminated && سطر.buf[0] == '"') {
			strbuf_reset(&unquoted);
			إذا (unquote_c_style(&unquoted, سطر.buf, عدم))
				die(_("unable to unquote C-style string '%s'"),
					سطر.buf);

			مسیر = unquoted.buf;
		}

		إذا (مسیر_درون_sparse_checkout(مسیر, repo->شاخص))
			write_name_quoted(مسیر, stdout, سطر_terminator);
	}
	strbuf_release(&سطر);
	strbuf_release(&unquoted);

	أرجع 0;
}

ساكن صحيح sparse_checkout_check_قواعد(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
				       بنية repository *repo)
{
	ساكن بنية option builtin_sparse_checkout_check_قواعد_options[] = {
		OPT_BOOL('z', عدم, &check_قواعد_opts.null_termination,
			 N_("terminate input and output files by a NUL character")),
		OPT_BOOL(0, "cone", &check_قواعد_opts.cone_نما,
			 N_("when used with --rules-file interpret patterns as cone mode patterns")),
		OPT_FILENAME(0, "rules-file", &check_قواعد_opts.قواعد_file,
			 N_("use patterns in <file> instead of the current ones.")),
		OPT_انتها(),
	};

	FILE *fp;
	صحيح ret;
	بنية الگو_list pl = {0};
	بنية repo_config_مقادیر *cfg = repo_config_مقادیر(the_repository);
	حرف *sparse_filename;
	check_قواعد_opts.cone_نما = -1;

	argc = parse_options(argc, argv, پیشوند,
			     builtin_sparse_checkout_check_قواعد_options,
			     builtin_sparse_checkout_check_قواعد_usage, 0);

	إذا (check_قواعد_opts.قواعد_file && check_قواعد_opts.cone_نما < 0)
		check_قواعد_opts.cone_نما = 1;

	update_cone_نما(&check_قواعد_opts.cone_نما);
	pl.use_cone_patterns = cfg->core_sparse_checkout_cone;
	إذا (check_قواعد_opts.قواعد_file) {
		fp = xfopen(check_قواعد_opts.قواعد_file, "r");
		جمع_کن_patterns_from_درون_داد(&pl, argc, argv, fp);
		fclose(fp);
	} وإلا {
		sparse_filename = get_sparse_checkout_filename();
		إذا (جمع_کن_patterns_from_file_مقصد_list(sparse_filename, "", 0, &pl,
						   عدم, 0))
			die(_("unable to load existing sparse-checkout patterns"));
		حرر(sparse_filename);
	}

	ret = check_قواعد(repo, &pl, check_قواعد_opts.null_termination);
	clear_الگو_list(&pl);
	حرر(check_قواعد_opts.قواعد_file);
	أرجع ret;
}

صحيح cmd_sparse_checkout(صحيح argc,
			ثابت حرف **argv,
			ثابت حرف *پیشوند,
			بنية repository *repo)
{
	parse_opt_subcommand_fn *fn = عدم;
	بنية option builtin_sparse_checkout_options[] = {
		OPT_SUBCOMMAND("list", &fn, sparse_checkout_list),
		OPT_SUBCOMMAND("init", &fn, sparse_checkout_init),
		OPT_SUBCOMMAND("set", &fn, sparse_checkout_مجموعه),
		OPT_SUBCOMMAND("add", &fn, sparse_checkout_جمع_کن),
		OPT_SUBCOMMAND("reapply", &fn, sparse_checkout_reapply),
		OPT_SUBCOMMAND("clean", &fn, sparse_checkout_clean),
		OPT_SUBCOMMAND("disable", &fn, sparse_checkout_disable),
		OPT_SUBCOMMAND("check-rules", &fn, sparse_checkout_check_قواعد),
		OPT_انتها(),
	};

	argc = parse_options(argc, argv, پیشوند,
			     builtin_sparse_checkout_options,
			     builtin_sparse_checkout_usage, 0);

	repo_config(the_repository, git_default_config, عدم);

	prepare_repo_settings(repo);
	repo->settings.command_requires_پر_شاخص = 0;

	أرجع fn(argc, argv, پیشوند, repo);
}
