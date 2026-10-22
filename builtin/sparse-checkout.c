#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#تعریف_کرو DISABLE_SIGN_COMPARE_WARNINGS

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

ساکن ثابت حرف *خالی_خانے_بنیاد = "";

ساکن حرف ثابت * ثابت builtin_sparse_checkout_usage[] = {
	N_("git sparse-checkout (init | list | set | add | reapply | disable | check-rules | clean) [<options>]"),
	تہی
};

ساکن خالی write_patterns_مقصد_file(FILE *fp, ساخت نمونہ_list *pl)
{
	صحیح i;

	برائے (i = 0; i < pl->nr; i++) {
		ساخت راستہ_نمونہ *p = pl->patterns[i];

		اگر (p->flags & نمونہ_FLAG_NEGATIVE)
			fprintf(fp, "!");

		fprintf(fp, "%s", p->نمونہ);

		اگر (p->flags & نمونہ_FLAG_MUSTBEDIR)
			fprintf(fp, "/");

		fprintf(fp, "\n");
	}
}

ساکن حرف ثابت * ثابت builtin_sparse_checkout_list_usage[] = {
	"git sparse-checkout list",
	تہی
};

ساکن صحیح sparse_checkout_list(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
				ساخت repository *repo غیر_مستعمل)
{
	ساکن ساخت option builtin_sparse_checkout_list_options[] = {
		OPT_انتہا(),
	};
	ساخت نمونہ_list pl;
	حرف *sparse_filename;
	صحیح res;
	ساخت repo_config_اقدار *cfg = repo_config_اقدار(the_repository);

	تیاری_work_درخت(the_repository);
	اگر (!cfg->apply_sparse_checkout)
		die(_("this worktree is not sparse"));

	argc = parse_options(argc, argv, سابقہ,
			     builtin_sparse_checkout_list_options,
			     builtin_sparse_checkout_list_usage, 0);

	memset(&pl, 0, حجم(pl));

	pl.use_cone_patterns = cfg->core_sparse_checkout_cone;

	sparse_filename = get_sparse_checkout_filename();
	res = جوڑو_patterns_from_file_مقصد_list(sparse_filename, "", 0, &pl, تہی, 0);
	آزاد(sparse_filename);

	اگر (res < 0) {
		warning(_("this worktree is not sparse (sparse-checkout file may not exist)"));
		واپس 0;
	}

	اگر (pl.use_cone_patterns) {
		صحیح i;
		ساخت نمونہ_اندراج *pe;
		ساخت hashmap_iter iter;
		ساخت string_list sl = STRING_LIST_INIT_DUP;

		hashmap_for_each_اندراج(&pl.recursive_hashmap, &iter, pe, ent) {
			/* pe->pattern starts with "/", skip it */
			string_list_append(&sl, pe->نمونہ + 1);
		}

		string_list_ترتیب_دو_u(&sl, 0);

		برائے (i = 0; i < sl.nr; i++) {
			quote_c_style(sl.اشیاء[i].string, تہی, stdout, 0);
			لکھو("\n");
		}

		string_list_clear(&sl, 0);
	} ورنہ {
		write_patterns_مقصد_file(stdout, &pl);
	}

	clear_نمونہ_list(&pl);

	واپس 0;
}

ساکن خالی clean_tracked_sparse_directories(ساخت repository *r)
{
	صحیح i, was_بھرا = 0;
	ساخت strbuf راستہ = STRBUF_INIT;
	سائز_t pathlen;
	ساخت string_list_شے *شے;
	ساخت string_list sparse_dirs = STRING_LIST_INIT_DUP;

	/*
	 * If we are not using cone mode patterns, then we cannot
	 * delete directories outside of the sparse cone.
	 */
	اگر (!r || !r->اشاریہ || !r->worktree)
		واپس;
	اگر (init_sparse_checkout_patterns(r->اشاریہ) ||
	    !r->اشاریہ->sparse_checkout_patterns->use_cone_patterns)
		واپس;

	/*
	 * Use the sparse index as a data structure to assist finding
	 * directories that are safe to delete. This conversion to a
	 * sparse index will not delete directories that contain
	 * conflicted entries or submodules.
	 */
	اگر (r->اشاریہ->sparse_اشاریہ == اشاریہ_EXPANDED) {
		/*
		 * If something, such as a merge conflict or other concern,
		 * prevents us from converting to a sparse index, then do
		 * not try deleting files.
		 */
		اگر (convert_مقصد_sparse(r->اشاریہ, SPARSE_اشاریہ_حافظہ_ONLY))
			واپس;
		was_بھرا = 1;
	}

	strbuf_addstr(&راستہ, r->worktree);
	strbuf_complete(&راستہ, '/');
	pathlen = راستہ.len;

	/*
	 * Collect directories that have gone out of scope but also
	 * exist on disk, so there is some work to be done. We need to
	 * store the entries in a list before exploring, since that might
	 * expand the sparse-index again.
	 */
	برائے (i = 0; i < r->اشاریہ->cache_nr; i++) {
		ساخت cache_اندراج *ce = r->اشاریہ->cache[i];

		اگر (S_ISSPARSEDIR(ce->ce_منوال) &&
		    repo_file_exists(r, ce->name))
			string_list_append(&sparse_dirs, ce->name);
	}

	for_each_string_list_شے(شے, &sparse_dirs) {
		ساخت dir_struct dir = DIR_INIT;
		ساخت pathspec p = { 0 };
		ساخت strvec s = STRVEC_INIT;

		strbuf_setlen(&راستہ, pathlen);
		strbuf_addstr(&راستہ, شے->string);

		dir.flags |= DIR_SHOW_IGNORED_TOO;

		تیاری_standard_excludes(&dir);
		strvec_ڈالو(&s, راستہ.buf);

		parse_pathspec(&p, PATHSPEC_GLOB, 0, تہی, s.v);
		fill_directory(&dir, r->اشاریہ, &p);

		اگر (dir.nr) {
			warning(_("directory '%s' contains untracked files,"
				  " but is not in the sparse-checkout cone"),
				شے->string);
		} ورنہ اگر (remove_dir_recursively(&راستہ, 0)) {
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
	strbuf_release(&راستہ);

	اگر (was_بھرا)
		ensure_بھرا_اشاریہ(r->اشاریہ);
}

ساکن صحیح update_working_directory(ساخت repository *r,
				    ساخت نمونہ_list *pl)
{
	شمار update_sparsity_نتیجہ نتیجہ;
	ساخت unpack_trees_options o;
	ساخت تالا_file تالا_file = تالا_INIT;
	ساخت نمونہ_list *old_pl;

	/* If no branch has been checked out, there are no updates to make. */
	اگر (is_اشاریہ_unborn(r->اشاریہ))
		واپس UPDATE_SPARSITY_SUCCESS;

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

	repo_hold_locked_اشاریہ(r, &تالا_file, تالا_DIE_ON_ERROR);

	تیاری_unpack_trees_porcelain(&o, "sparse-checkout");
	نتیجہ = update_sparsity(&o, pl);
	clear_unpack_trees_porcelain(&o);

	اگر (نتیجہ == UPDATE_SPARSITY_WARNINGS)
		/*
		 * We don't do any special handling of warnings from untracked
		 * files in the way or dirty entries that can't be removed.
		 */
		نتیجہ = UPDATE_SPARSITY_SUCCESS;
	اگر (نتیجہ == UPDATE_SPARSITY_SUCCESS)
		write_locked_اشاریہ(r->اشاریہ, &تالا_file, COMMIT_تالا);
	ورنہ
		rollback_تالا_file(&تالا_file);

	clean_tracked_sparse_directories(r);

	اگر (r->اشاریہ->sparse_checkout_patterns != pl) {
		clear_نمونہ_list(r->اشاریہ->sparse_checkout_patterns);
		FREE_AND_NULL(r->اشاریہ->sparse_checkout_patterns);
	}
	r->اشاریہ->sparse_checkout_patterns = old_pl;

	واپس نتیجہ;
}

ساکن حرف *escaped_نمونہ(حرف *نمونہ)
{
	حرف *p = نمونہ;
	ساخت strbuf final = STRBUF_INIT;

	جب (*p) {
		اگر (is_glob_special(*p))
			strbuf_addch(&final, '\\');

		strbuf_addch(&final, *p);
		p++;
	}

	واپس strbuf_detach(&final, تہی);
}

ساکن خالی write_cone_مقصد_file(FILE *fp, ساخت نمونہ_list *pl)
{
	صحیح i;
	ساخت نمونہ_اندراج *pe;
	ساخت hashmap_iter iter;
	ساخت string_list sl = STRING_LIST_INIT_DUP;
	ساخت strbuf والد_نمونہ = STRBUF_INIT;

	hashmap_for_each_اندراج(&pl->والد_hashmap, &iter, pe, ent) {
		اگر (hashmap_get_اندراج(&pl->recursive_hashmap, pe, ent, تہی))
			جاری;

		اگر (!hashmap_شامل_ہے_والد(&pl->recursive_hashmap,
					     pe->نمونہ,
					     &والد_نمونہ))
			string_list_append(&sl, pe->نمونہ);
	}

	string_list_ترتیب_دو_u(&sl, 0);

	fprintf(fp, "/*\n!/*/\n");

	برائے (i = 0; i < sl.nr; i++) {
		حرف *نمونہ = escaped_نمونہ(sl.اشیاء[i].string);

		اگر (لڑی_لمبائی(نمونہ))
			fprintf(fp, "%s/\n!%s/*/\n", نمونہ, نمونہ);
		آزاد(نمونہ);
	}

	string_list_clear(&sl, 0);

	hashmap_for_each_اندراج(&pl->recursive_hashmap, &iter, pe, ent) {
		اگر (!hashmap_شامل_ہے_والد(&pl->recursive_hashmap,
					     pe->نمونہ,
					     &والد_نمونہ))
			string_list_append(&sl, pe->نمونہ);
	}

	strbuf_release(&والد_نمونہ);

	string_list_ترتیب_دو_u(&sl, 0);

	برائے (i = 0; i < sl.nr; i++) {
		حرف *نمونہ = escaped_نمونہ(sl.اشیاء[i].string);
		fprintf(fp, "%s/\n", نمونہ);
		آزاد(نمونہ);
	}

	string_list_clear(&sl, 0);
}

ساکن صحیح write_patterns_and_update(ساخت repository *repo,
				     ساخت نمونہ_list *pl)
{
	حرف *sparse_filename;
	FILE *fp;
	ساخت تالا_file lk = تالا_INIT;
	صحیح نتیجہ;
	ساخت repo_config_اقدار *cfg = repo_config_اقدار(the_repository);

	sparse_filename = get_sparse_checkout_filename();

	اگر (محفوظ_create_leading_directories(repo, sparse_filename))
		die(_("failed to create directory for sparse-checkout file"));

	repo_hold_تالا_file_for_update(repo, &lk, sparse_filename,
				       تالا_DIE_ON_ERROR);

	نتیجہ = update_working_directory(repo, pl);
	اگر (نتیجہ) {
		rollback_تالا_file(&lk);
		update_working_directory(repo, تہی);
		جاؤ برآمد;
	}

	fp = fdopen_تالا_file(&lk, "w");
	اگر (!fp)
		die_errno(_("unable to fdopen %s"), get_تالا_file_راستہ(&lk));

	اگر (cfg->core_sparse_checkout_cone)
		write_cone_مقصد_file(fp, pl);
	ورنہ
		write_patterns_مقصد_file(fp, pl);

	اگر (commit_تالا_file(&lk))
		die_errno(_("unable to write %s"), sparse_filename);

برآمد:
	clear_نمونہ_list(pl);
	آزاد(sparse_filename);
	واپس نتیجہ;
}

شمار sparse_checkout_منوال {
	منوال_NO_PATTERNS = 0,
	منوال_ALL_PATTERNS = 1,
	منوال_CONE_PATTERNS = 2,
};

ساکن صحیح سیٹ_config(ساخت repository *repo,
		      شمار sparse_checkout_منوال منوال)
{
	/* Update to use worktree config, if not already. */
	اگر (init_worktree_config(repo)) {
		error(_("failed to initialize worktree config"));
		واپس 1;
	}

	اگر (repo_config_سیٹ_worktree_gently(repo,
					    "core.sparseCheckout",
					    منوال ? "true" : "false") ||
	    repo_config_سیٹ_worktree_gently(repo,
					    "core.sparseCheckoutCone",
					    منوال == منوال_CONE_PATTERNS ?
						"true" : "false"))
		واپس 1;

	اگر (منوال == منوال_NO_PATTERNS)
		واپس سیٹ_sparse_اشاریہ_config(repo, 0);

	واپس 0;
}

ساکن شمار sparse_checkout_منوال update_cone_منوال(صحیح *cone_منوال) {
	ساخت repo_config_اقدار *cfg = repo_config_اقدار(the_repository);

	/* If not specified, use previous definition of cone mode */
	اگر (*cone_منوال == -1 && cfg->apply_sparse_checkout)
		*cone_منوال = cfg->core_sparse_checkout_cone;

	/* Set cone/non-cone mode appropriately */
	cfg->apply_sparse_checkout = 1;
	اگر (*cone_منوال == 1 || *cone_منوال == -1) {
		cfg->core_sparse_checkout_cone = 1;
		واپس منوال_CONE_PATTERNS;
	}
	cfg->core_sparse_checkout_cone = 0;
	واپس منوال_ALL_PATTERNS;
}

ساکن صحیح update_modes(ساخت repository *repo, صحیح *cone_منوال, صحیح *sparse_اشاریہ)
{
	صحیح منوال, ریکارڈ_منوال;
	ساخت repo_config_اقدار *cfg = repo_config_اقدار(the_repository);

	/* Determine if we need to record the mode; ensure sparse checkout on */
	ریکارڈ_منوال = (*cone_منوال != -1) || !cfg->apply_sparse_checkout;

	منوال = update_cone_منوال(cone_منوال);
	اگر (ریکارڈ_منوال && سیٹ_config(repo, منوال))
		واپس 1;

	/* Set sparse-index/non-sparse-index mode if specified */
	اگر (*sparse_اشاریہ >= 0) {
		اگر (سیٹ_sparse_اشاریہ_config(repo, *sparse_اشاریہ) < 0)
			die(_("failed to modify sparse-index config"));

		/* force an index rewrite */
		repo_read_اشاریہ(repo);
		repo->اشاریہ->updated_workdir = 1;

		اگر (!*sparse_اشاریہ)
			ensure_بھرا_اشاریہ(repo->اشاریہ);
	}

	واپس 0;
}

ساکن حرف ثابت * ثابت builtin_sparse_checkout_init_usage[] = {
	"git sparse-checkout init [--cone] [--[no-]sparse-index]",
	تہی
};

ساکن ساخت sparse_checkout_init_opts {
	صحیح cone_منوال;
	صحیح sparse_اشاریہ;
} init_opts;

ساکن صحیح sparse_checkout_init(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
				ساخت repository *repo)
{
	ساخت نمونہ_list pl;
	حرف *sparse_filename;
	صحیح res;
	ساخت object_id oid;

	ساکن ساخت option builtin_sparse_checkout_init_options[] = {
		OPT_BOOL(0, "cone", &init_opts.cone_منوال,
			 N_("initialize the sparse-checkout in cone mode")),
		OPT_BOOL(0, "sparse-index", &init_opts.sparse_اشاریہ,
			 N_("toggle the use of a sparse index")),
		OPT_انتہا(),
	};

	تیاری_work_درخت(the_repository);
	repo_read_اشاریہ(repo);

	init_opts.cone_منوال = -1;
	init_opts.sparse_اشاریہ = -1;

	argc = parse_options(argc, argv, سابقہ,
			     builtin_sparse_checkout_init_options,
			     builtin_sparse_checkout_init_usage, 0);

	اگر (update_modes(repo, &init_opts.cone_منوال, &init_opts.sparse_اشاریہ))
		واپس 1;

	memset(&pl, 0, حجم(pl));

	sparse_filename = get_sparse_checkout_filename();
	res = جوڑو_patterns_from_file_مقصد_list(sparse_filename, "", 0, &pl, تہی, 0);

	/* If we already have a sparse-checkout file, use it. */
	اگر (res >= 0) {
		آزاد(sparse_filename);
		clear_نمونہ_list(&pl);
		واپس update_working_directory(repo, تہی);
	}

	اگر (repo_get_oid(repo, "HEAD", &oid)) {
		FILE *fp;

		/* assume we are in a fresh repo, but update the sparse-checkout file */
		اگر (محفوظ_create_leading_directories(repo, sparse_filename))
			die(_("unable to create leading directories of %s"),
			    sparse_filename);
		fp = xfopen(sparse_filename, "w");
		اگر (!fp)
			die(_("failed to open '%s'"), sparse_filename);

		آزاد(sparse_filename);
		fprintf(fp, "/*\n!/*/\n");
		fclose(fp);
		واپس 0;
	}

	آزاد(sparse_filename);

	جوڑو_نمونہ("/*", خالی_خانے_بنیاد, 0, &pl, 0);
	جوڑو_نمونہ("!/*/", خالی_خانے_بنیاد, 0, &pl, 0);
	pl.use_cone_patterns = init_opts.cone_منوال;

	واپس write_patterns_and_update(repo, &pl);
}

ساکن خالی داخل_کرو_recursive_نمونہ(ساخت نمونہ_list *pl, ساخت strbuf *راستہ)
{
	ساخت نمونہ_اندراج *e = xmalloc(حجم(*e));
	e->patternlen = راستہ->len;
	e->نمونہ = strbuf_detach(راستہ, تہی);
	hashmap_اندراج_init(&e->ent, fspathhash(e->نمونہ));

	hashmap_جوڑو(&pl->recursive_hashmap, &e->ent);

	جب (e->patternlen) {
		حرف *slash = strrchr(e->نمونہ, '/');
		حرف *oldpattern = e->نمونہ;
		سائز_t newlen;
		ساخت نمونہ_اندراج *dup;

		اگر (!slash || slash == e->نمونہ)
			توڑو;

		newlen = slash - e->نمونہ;
		e = xmalloc(حجم(ساخت نمونہ_اندراج));
		e->patternlen = newlen;
		e->نمونہ = xstrndup(oldpattern, newlen);
		hashmap_اندراج_init(&e->ent, fspathhash(e->نمونہ));

		dup = hashmap_get_اندراج(&pl->والد_hashmap, e, ent, تہی);
		اگر (!dup) {
			hashmap_جوڑو(&pl->والد_hashmap, &e->ent);
		} ورنہ {
			آزاد(e->نمونہ);
			آزاد(e);
			e = dup;
		}
	}
}

ساکن خالی strbuf_مقصد_cone_نمونہ(ساخت strbuf *سطر, ساخت نمونہ_list *pl)
{
	strbuf_trim(سطر);

	strbuf_trim_trailing_dir_sep(سطر);

	اگر (strbuf_normalize_راستہ(سطر))
		die(_("could not normalize path %s"), سطر->buf);

	اگر (!سطر->len)
		واپس;

	اگر (سطر->buf[0] != '/')
		strbuf_insertstr(سطر, 0, "/");

	داخل_کرو_recursive_نمونہ(pl, سطر);
}

ساکن خالی جوڑو_patterns_from_داخلہ(ساخت نمونہ_list *pl,
				    صحیح argc, ثابت حرف **argv,
				    FILE *file)
{
	صحیح i;
	ساخت repo_config_اقدار *cfg = repo_config_اقدار(the_repository);

	اگر (cfg->core_sparse_checkout_cone) {
		ساخت strbuf سطر = STRBUF_INIT;

		hashmap_init(&pl->recursive_hashmap, pl_hashmap_cmp, تہی, 0);
		hashmap_init(&pl->والد_hashmap, pl_hashmap_cmp, تہی, 0);
		pl->use_cone_patterns = 1;

		اگر (file) {
			ساخت strbuf unquoted = STRBUF_INIT;
			جب (!strbuf_getline(&سطر, file)) {
				اگر (سطر.buf[0] == '"') {
					strbuf_reset(&unquoted);
					اگر (unquote_c_style(&unquoted, سطر.buf, تہی))
						die(_("unable to unquote C-style string '%s'"),
						سطر.buf);

					strbuf_تبادلہ(&unquoted, &سطر);
				}

				strbuf_مقصد_cone_نمونہ(&سطر, pl);
			}

			strbuf_release(&unquoted);
		} ورنہ {
			برائے (i = 0; i < argc; i++) {
				strbuf_setlen(&سطر, 0);
				strbuf_addstr(&سطر, argv[i]);
				strbuf_مقصد_cone_نمونہ(&سطر, pl);
			}
		}
		strbuf_release(&سطر);
	} ورنہ {
		اگر (file) {
			ساخت strbuf سطر = STRBUF_INIT;

			جب (!strbuf_getline(&سطر, file))
				جوڑو_نمونہ(سطر.buf, خالی_خانے_بنیاد, 0, pl, 0);

			strbuf_release(&سطر);
		} ورنہ {
			برائے (i = 0; i < argc; i++)
				جوڑو_نمونہ(argv[i], خالی_خانے_بنیاد, 0, pl, 0);
		}
	}
}

شمار modify_type {
	REPLACE,
	جوڑو_2,
};

ساکن خالی جوڑو_patterns_cone_منوال(صحیح argc, ثابت حرف **argv,
				   ساخت نمونہ_list *pl,
				   صحیح use_stdin)
{
	ساخت strbuf بفر = STRBUF_INIT;
	ساخت نمونہ_اندراج *pe;
	ساخت hashmap_iter iter;
	ساخت نمونہ_list existing;
	ساخت repo_config_اقدار *cfg = repo_config_اقدار(the_repository);
	حرف *sparse_filename = get_sparse_checkout_filename();

	جوڑو_patterns_from_داخلہ(pl, argc, argv,
				use_stdin ? stdin : تہی);

	memset(&existing, 0, حجم(existing));
	existing.use_cone_patterns = cfg->core_sparse_checkout_cone;

	اگر (جوڑو_patterns_from_file_مقصد_list(sparse_filename, "", 0,
					   &existing, تہی, 0))
		die(_("unable to load existing sparse-checkout patterns"));
	آزاد(sparse_filename);

	اگر (!existing.use_cone_patterns)
		die(_("existing sparse-checkout patterns do not use cone mode"));

	hashmap_for_each_اندراج(&existing.recursive_hashmap, &iter, pe, ent) {
		اگر (!hashmap_شامل_ہے_والد(&pl->recursive_hashmap,
					pe->نمونہ, &بفر) ||
		    !hashmap_شامل_ہے_والد(&pl->والد_hashmap,
					pe->نمونہ, &بفر)) {
			strbuf_reset(&بفر);
			strbuf_addstr(&بفر, pe->نمونہ);
			داخل_کرو_recursive_نمونہ(pl, &بفر);
		}
	}

	clear_نمونہ_list(&existing);
	strbuf_release(&بفر);
}

ساکن خالی جوڑو_patterns_literal(صحیح argc, ثابت حرف **argv,
				 ساخت نمونہ_list *pl,
				 صحیح use_stdin)
{
	حرف *sparse_filename = get_sparse_checkout_filename();
	اگر (جوڑو_patterns_from_file_مقصد_list(sparse_filename, "", 0,
					   pl, تہی, 0))
		die(_("unable to load existing sparse-checkout patterns"));
	آزاد(sparse_filename);
	جوڑو_patterns_from_داخلہ(pl, argc, argv, use_stdin ? stdin : تہی);
}

ساکن صحیح modify_نمونہ_list(ساخت repository *repo,
			       ساخت strvec *دلائل, صحیح use_stdin,
			       شمار modify_type m)
{
	صحیح نتیجہ;
	صحیح بدلا_config = 0;
	ساخت نمونہ_list *pl = xcalloc(1, حجم(*pl));
	ساخت repo_config_اقدار *cfg = repo_config_اقدار(the_repository);

	انتخاب (m) {
	حالت جوڑو_2:
		اگر (cfg->core_sparse_checkout_cone)
			جوڑو_patterns_cone_منوال(دلائل->nr, دلائل->v, pl, use_stdin);
		ورنہ
			جوڑو_patterns_literal(دلائل->nr, دلائل->v, pl, use_stdin);
		توڑو;

	حالت REPLACE:
		جوڑو_patterns_from_داخلہ(pl, دلائل->nr, دلائل->v,
					use_stdin ? stdin : تہی);
		توڑو;
	}

	اگر (!cfg->apply_sparse_checkout) {
		سیٹ_config(repo, منوال_ALL_PATTERNS);
		cfg->apply_sparse_checkout = 1;
		بدلا_config = 1;
	}

	نتیجہ = write_patterns_and_update(repo, pl);

	اگر (نتیجہ && بدلا_config)
		سیٹ_config(repo, منوال_NO_PATTERNS);

	clear_نمونہ_list(pl);
	آزاد(pl);
	واپس نتیجہ;
}

ساکن خالی sanitize_paths(ساخت repository *repo,
			   ساخت strvec *دلائل,
			   ثابت حرف *سابقہ, صحیح skip_checks)
{
	صحیح i;
	ساخت repo_config_اقدار *cfg = repo_config_اقدار(the_repository);

	اگر (!دلائل->nr)
		واپس;

	اگر (سابقہ && *سابقہ && cfg->core_sparse_checkout_cone) {
		/*
		 * The args are not pathspecs, so unfortunately we
		 * cannot imitate how cmd_add() uses parse_pathspec().
		 */
		صحیح سابقہ_len = لڑی_لمبائی(سابقہ);

		برائے (i = 0; i < دلائل->nr; i++) {
			حرف *prefixed_راستہ = سابقہ_راستہ(the_repository, سابقہ,
							  سابقہ_len, دلائل->v[i]);
			strvec_replace(دلائل, i, prefixed_راستہ);
			آزاد(prefixed_راستہ);
		}
	}

	اگر (skip_checks)
		واپس;

	اگر (سابقہ && *سابقہ && !cfg->core_sparse_checkout_cone)
		die(_("please run from the toplevel directory in non-cone mode"));

	اگر (cfg->core_sparse_checkout_cone) {
		برائے (i = 0; i < دلائل->nr; i++) {
			اگر (دلائل->v[i][0] == '/')
				die(_("specify directories rather than patterns (no leading slash)"));
			اگر (دلائل->v[i][0] == '!')
				die(_("specify directories rather than patterns.  If your directory starts with a '!', pass --skip-checks"));
			اگر (strpbrk(دلائل->v[i], "*?[]"))
				die(_("specify directories rather than patterns.  If your directory really has any of '*?[]\\' in it, pass --skip-checks"));
		}
	}

	برائے (i = 0; i < دلائل->nr; i++) {
		ساخت cache_اندراج *ce;
		ساخت اشاریہ_state *اشاریہ = repo->اشاریہ;
		صحیح pos = اشاریہ_name_pos(اشاریہ, دلائل->v[i], لڑی_لمبائی(دلائل->v[i]));

		اگر (pos < 0)
			جاری;
		ce = اشاریہ->cache[pos];
		اگر (S_ISSPARSEDIR(ce->ce_منوال))
			جاری;

		اگر (cfg->core_sparse_checkout_cone)
			die(_("'%s' is not a directory; to treat it as a directory anyway, rerun with --skip-checks"), دلائل->v[i]);
		ورنہ
			warning(_("pass a leading slash before paths such as '%s' if you want a single file (see NON-CONE PROBLEMS in the git-sparse-checkout manual)."), دلائل->v[i]);
	}
}

ساکن حرف ثابت * ثابت builtin_sparse_checkout_جوڑو_usage[] = {
	N_("git sparse-checkout add [--skip-checks] (--stdin | <patterns>)"),
	تہی
};

ساکن ساخت sparse_checkout_جوڑو_opts {
	صحیح skip_checks;
	صحیح use_stdin;
} جوڑو_opts;

ساکن صحیح sparse_checkout_جوڑو(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
			       ساخت repository *repo)
{
	ساکن ساخت option builtin_sparse_checkout_جوڑو_options[] = {
		OPT_BOOL_F(0, "skip-checks", &جوڑو_opts.skip_checks,
			   N_("skip some sanity checks on the given paths that might give false positives"),
			   PARSE_OPT_NONEG),
		OPT_BOOL(0, "stdin", &جوڑو_opts.use_stdin,
			 N_("read patterns from standard in")),
		OPT_انتہا(),
	};
	ساخت strvec patterns = STRVEC_INIT;
	صحیح ret;
	ساخت repo_config_اقدار *cfg = repo_config_اقدار(the_repository);

	تیاری_work_درخت(the_repository);
	اگر (!cfg->apply_sparse_checkout)
		die(_("no sparse-checkout to add to"));

	repo_read_اشاریہ(repo);

	argc = parse_options(argc, argv, سابقہ,
			     builtin_sparse_checkout_جوڑو_options,
			     builtin_sparse_checkout_جوڑو_usage, 0);

	برائے (صحیح i = 0; i < argc; i++)
		strvec_ڈالو(&patterns, argv[i]);
	sanitize_paths(repo, &patterns, سابقہ, جوڑو_opts.skip_checks);

	ret = modify_نمونہ_list(repo, &patterns, جوڑو_opts.use_stdin, جوڑو_2);

	strvec_clear(&patterns);
	واپس ret;
}

ساکن حرف ثابت * ثابت builtin_sparse_checkout_سیٹ_usage[] = {
	N_("git sparse-checkout set [--[no-]cone] [--[no-]sparse-index] [--skip-checks] (--stdin | <patterns>)"),
	تہی
};

ساکن ساخت sparse_checkout_سیٹ_opts {
	صحیح cone_منوال;
	صحیح sparse_اشاریہ;
	صحیح skip_checks;
	صحیح use_stdin;
} سیٹ_opts;

ساکن صحیح sparse_checkout_سیٹ(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
			       ساخت repository *repo)
{
	ساخت repo_config_اقدار *cfg = repo_config_اقدار(the_repository);
	صحیح default_patterns_nr = 2;
	ثابت حرف *default_patterns[] = {"/*", "!/*/", تہی};

	ساکن ساخت option builtin_sparse_checkout_سیٹ_options[] = {
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
		OPT_انتہا(),
	};
	ساخت strvec patterns = STRVEC_INIT;
	صحیح ret;

	تیاری_work_درخت(the_repository);
	repo_read_اشاریہ(repo);

	سیٹ_opts.cone_منوال = -1;
	سیٹ_opts.sparse_اشاریہ = -1;

	argc = parse_options(argc, argv, سابقہ,
			     builtin_sparse_checkout_سیٹ_options,
			     builtin_sparse_checkout_سیٹ_usage, 0);

	اگر (update_modes(repo, &سیٹ_opts.cone_منوال, &سیٹ_opts.sparse_اشاریہ))
		واپس 1;

	/*
	 * Cone mode automatically specifies the toplevel directory.  For
	 * non-cone mode, if nothing is specified, manually select just the
	 * top-level directory (much as 'init' would do).
	 */
	اگر (!cfg->core_sparse_checkout_cone && !سیٹ_opts.use_stdin && argc == 0) {
		برائے (صحیح i = 0; i < default_patterns_nr; i++)
			strvec_ڈالو(&patterns, default_patterns[i]);
	} ورنہ {
		برائے (صحیح i = 0; i < argc; i++)
			strvec_ڈالو(&patterns, argv[i]);
		sanitize_paths(repo, &patterns, سابقہ, سیٹ_opts.skip_checks);
	}

	ret = modify_نمونہ_list(repo, &patterns, سیٹ_opts.use_stdin, REPLACE);

	strvec_clear(&patterns);
	واپس ret;
}

ساکن حرف ثابت * ثابت builtin_sparse_checkout_reapply_usage[] = {
	"git sparse-checkout reapply [--[no-]cone] [--[no-]sparse-index]",
	تہی
};

ساکن ساخت sparse_checkout_reapply_opts {
	صحیح cone_منوال;
	صحیح sparse_اشاریہ;
} reapply_opts;

ساکن صحیح sparse_checkout_reapply(صحیح argc, ثابت حرف **argv,
				   ثابت حرف *سابقہ,
				   ساخت repository *repo)
{
	ساکن ساخت option builtin_sparse_checkout_reapply_options[] = {
		OPT_BOOL(0, "cone", &reapply_opts.cone_منوال,
			 N_("initialize the sparse-checkout in cone mode")),
		OPT_BOOL(0, "sparse-index", &reapply_opts.sparse_اشاریہ,
			 N_("toggle the use of a sparse index")),
		OPT_انتہا(),
	};
	ساخت repo_config_اقدار *cfg = repo_config_اقدار(the_repository);

	تیاری_work_درخت(the_repository);
	اگر (!cfg->apply_sparse_checkout)
		die(_("must be in a sparse-checkout to reapply sparsity patterns"));

	reapply_opts.cone_منوال = -1;
	reapply_opts.sparse_اشاریہ = -1;

	argc = parse_options(argc, argv, سابقہ,
			     builtin_sparse_checkout_reapply_options,
			     builtin_sparse_checkout_reapply_usage, 0);

	repo_read_اشاریہ(repo);

	اگر (update_modes(repo, &reapply_opts.cone_منوال, &reapply_opts.sparse_اشاریہ))
		واپس 1;

	واپس update_working_directory(repo, تہی);
}

ساکن حرف ثابت * ثابت builtin_sparse_checkout_clean_usage[] = {
	"git sparse-checkout clean [-n|--dry-run]",
	تہی
};

ساکن صحیح list_file_iterator(ثابت حرف *راستہ, ثابت خالی *ڈیٹا)
{
	ثابت حرف *پیغام = ڈیٹا;

	لکھو(پیغام, راستہ);
	واپس 0;
}

ساکن خالی list_every_file_اندر_dir(ثابت حرف *پیغام,
				   ثابت حرف *directory)
{
	ساخت strbuf راستہ = STRBUF_INIT;

	strbuf_addstr(&راستہ, directory);
	for_each_file_اندر_dir(&راستہ, list_file_iterator, پیغام);
	strbuf_release(&راستہ);
}

ساکن ثابت حرف *پیغام_remove = N_("Removing %s\n");
ساکن ثابت حرف *پیغام_would_remove = N_("Would remove %s\n");

ساکن صحیح sparse_checkout_clean(صحیح argc, ثابت حرف **argv,
				   ثابت حرف *سابقہ,
				   ساخت repository *repo)
{
	ساخت strbuf بھرا_راستہ = STRBUF_INIT;
	ثابت حرف *پیغام = پیغام_remove;
	سائز_t worktree_len;
	صحیح force = 0, dry_تسلسل = 0, verbose = 0;
	صحیح require_force = 1;
	ساخت repo_config_اقدار *cfg = repo_config_اقدار(the_repository);

	ساخت option builtin_sparse_checkout_clean_options[] = {
		OPT_DRY_تسلسل(&dry_تسلسل, N_("dry run")),
		OPT__FORCE(&force, N_("force"), PARSE_OPT_NOCOMPLETE),
		OPT__VERBOSE(&verbose, N_("report each affected file, not just directories")),
		OPT_انتہا(),
	};

	تیاری_work_درخت(the_repository);
	اگر (!cfg->apply_sparse_checkout)
		die(_("must be in a sparse-checkout to clean directories"));
	اگر (!cfg->core_sparse_checkout_cone)
		die(_("must be in a cone-mode sparse-checkout to clean directories"));

	argc = parse_options(argc, argv, سابقہ,
			     builtin_sparse_checkout_clean_options,
			     builtin_sparse_checkout_clean_usage, 0);

	repo_config_get_bool(repo, "clean.requireforce", &require_force);
	اگر (require_force && !force && !dry_تسلسل)
		die(_("for safety, refusing to clean without one of --force or --dry-run"));

	اگر (dry_تسلسل)
		پیغام = پیغام_would_remove;

	اگر (repo_read_اشاریہ(repo) < 0)
		die(_("failed to read index"));

	اگر (convert_مقصد_sparse(repo->اشاریہ, SPARSE_اشاریہ_حافظہ_ONLY) ||
	    repo->اشاریہ->sparse_اشاریہ == اشاریہ_EXPANDED)
		die(_("failed to convert index to a sparse index; resolve merge conflicts and try again"));

	strbuf_addstr(&بھرا_راستہ, repo->worktree);
	strbuf_addch(&بھرا_راستہ, '/');
	worktree_len = بھرا_راستہ.len;

	برائے (سائز_t i = 0; i < repo->اشاریہ->cache_nr; i++) {
		ساخت cache_اندراج *ce = repo->اشاریہ->cache[i];
		اگر (!S_ISSPARSEDIR(ce->ce_منوال))
			جاری;
		strbuf_setlen(&بھرا_راستہ, worktree_len);
		strbuf_جوڑو(&بھرا_راستہ, ce->name, ce->ce_namelen);

		اگر (!is_directory(بھرا_راستہ.buf))
			جاری;

		اگر (verbose)
			list_every_file_اندر_dir(پیغام, ce->name);
		ورنہ
			لکھو(پیغام, ce->name);

		اگر (dry_تسلسل <= 0 &&
		    remove_dir_recursively(&بھرا_راستہ, 0))
			warning_errno(_("failed to remove '%s'"), ce->name);
	}

	strbuf_release(&بھرا_راستہ);
	واپس 0;
}

ساکن حرف ثابت * ثابت builtin_sparse_checkout_disable_usage[] = {
	"git sparse-checkout disable",
	تہی
};

ساکن صحیح sparse_checkout_disable(صحیح argc, ثابت حرف **argv,
				   ثابت حرف *سابقہ,
				   ساخت repository *repo)
{
	ساکن ساخت option builtin_sparse_checkout_disable_options[] = {
		OPT_انتہا(),
	};
	ساخت نمونہ_list pl;
	ساخت repo_config_اقدار *cfg = repo_config_اقدار(the_repository);

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
	argc = parse_options(argc, argv, سابقہ,
			     builtin_sparse_checkout_disable_options,
			     builtin_sparse_checkout_disable_usage, 0);

	/*
	 * Disable the advice message for expanding a sparse index, as we
	 * are expecting to do that when disabling sparse-checkout.
	 */
	give_advice_on_expansion = 0;
	repo_read_اشاریہ(repo);

	memset(&pl, 0, حجم(pl));
	hashmap_init(&pl.recursive_hashmap, pl_hashmap_cmp, تہی, 0);
	hashmap_init(&pl.والد_hashmap, pl_hashmap_cmp, تہی, 0);
	pl.use_cone_patterns = 0;
	cfg->apply_sparse_checkout = 1;

	جوڑو_نمونہ("/*", خالی_خانے_بنیاد, 0, &pl, 0);

	prepare_repo_settings(the_repository);
	repo->settings.sparse_اشاریہ = 0;

	اگر (update_working_directory(repo, &pl))
		die(_("error while refreshing working directory"));

	clear_نمونہ_list(&pl);
	واپس سیٹ_config(repo, منوال_NO_PATTERNS);
}

ساکن حرف ثابت * ثابت builtin_sparse_checkout_check_قواعد_usage[] = {
	N_("git sparse-checkout check-rules [-z] [--skip-checks]"
	   "[--[no-]cone] [--rules-file <file>]"),
	تہی
};

ساکن ساخت sparse_checkout_check_قواعد_opts {
	صحیح cone_منوال;
	صحیح null_termination;
	حرف *قواعد_file;
} check_قواعد_opts;

ساکن صحیح check_قواعد(ساخت repository *repo,
		       ساخت نمونہ_list *pl,
		       صحیح null_terminated)
{
	ساخت strbuf سطر = STRBUF_INIT;
	ساخت strbuf unquoted = STRBUF_INIT;
	حرف *راستہ;
	صحیح سطر_terminator = null_terminated ? 0 : '\n';
	strbuf_getline_fn getline_fn = null_terminated ? strbuf_getline_nul
		: strbuf_getline;
	repo->اشاریہ->sparse_checkout_patterns = pl;
	جب (!getline_fn(&سطر, stdin)) {
		راستہ = سطر.buf;
		اگر (!null_terminated && سطر.buf[0] == '"') {
			strbuf_reset(&unquoted);
			اگر (unquote_c_style(&unquoted, سطر.buf, تہی))
				die(_("unable to unquote C-style string '%s'"),
					سطر.buf);

			راستہ = unquoted.buf;
		}

		اگر (راستہ_اندر_sparse_checkout(راستہ, repo->اشاریہ))
			write_name_quoted(راستہ, stdout, سطر_terminator);
	}
	strbuf_release(&سطر);
	strbuf_release(&unquoted);

	واپس 0;
}

ساکن صحیح sparse_checkout_check_قواعد(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
				       ساخت repository *repo)
{
	ساکن ساخت option builtin_sparse_checkout_check_قواعد_options[] = {
		OPT_BOOL('z', تہی, &check_قواعد_opts.null_termination,
			 N_("terminate input and output files by a NUL character")),
		OPT_BOOL(0, "cone", &check_قواعد_opts.cone_منوال,
			 N_("when used with --rules-file interpret patterns as cone mode patterns")),
		OPT_FILENAME(0, "rules-file", &check_قواعد_opts.قواعد_file,
			 N_("use patterns in <file> instead of the current ones.")),
		OPT_انتہا(),
	};

	FILE *fp;
	صحیح ret;
	ساخت نمونہ_list pl = {0};
	ساخت repo_config_اقدار *cfg = repo_config_اقدار(the_repository);
	حرف *sparse_filename;
	check_قواعد_opts.cone_منوال = -1;

	argc = parse_options(argc, argv, سابقہ,
			     builtin_sparse_checkout_check_قواعد_options,
			     builtin_sparse_checkout_check_قواعد_usage, 0);

	اگر (check_قواعد_opts.قواعد_file && check_قواعد_opts.cone_منوال < 0)
		check_قواعد_opts.cone_منوال = 1;

	update_cone_منوال(&check_قواعد_opts.cone_منوال);
	pl.use_cone_patterns = cfg->core_sparse_checkout_cone;
	اگر (check_قواعد_opts.قواعد_file) {
		fp = xfopen(check_قواعد_opts.قواعد_file, "r");
		جوڑو_patterns_from_داخلہ(&pl, argc, argv, fp);
		fclose(fp);
	} ورنہ {
		sparse_filename = get_sparse_checkout_filename();
		اگر (جوڑو_patterns_from_file_مقصد_list(sparse_filename, "", 0, &pl,
						   تہی, 0))
			die(_("unable to load existing sparse-checkout patterns"));
		آزاد(sparse_filename);
	}

	ret = check_قواعد(repo, &pl, check_قواعد_opts.null_termination);
	clear_نمونہ_list(&pl);
	آزاد(check_قواعد_opts.قواعد_file);
	واپس ret;
}

صحیح cmd_sparse_checkout(صحیح argc,
			ثابت حرف **argv,
			ثابت حرف *سابقہ,
			ساخت repository *repo)
{
	parse_opt_subcommand_fn *fn = تہی;
	ساخت option builtin_sparse_checkout_options[] = {
		OPT_SUBCOMMAND("list", &fn, sparse_checkout_list),
		OPT_SUBCOMMAND("init", &fn, sparse_checkout_init),
		OPT_SUBCOMMAND("set", &fn, sparse_checkout_سیٹ),
		OPT_SUBCOMMAND("add", &fn, sparse_checkout_جوڑو),
		OPT_SUBCOMMAND("reapply", &fn, sparse_checkout_reapply),
		OPT_SUBCOMMAND("clean", &fn, sparse_checkout_clean),
		OPT_SUBCOMMAND("disable", &fn, sparse_checkout_disable),
		OPT_SUBCOMMAND("check-rules", &fn, sparse_checkout_check_قواعد),
		OPT_انتہا(),
	};

	argc = parse_options(argc, argv, سابقہ,
			     builtin_sparse_checkout_options,
			     builtin_sparse_checkout_usage, 0);

	repo_config(the_repository, git_default_config, تہی);

	prepare_repo_settings(repo);
	repo->settings.command_requires_بھرا_اشاریہ = 0;

	واپس fn(argc, argv, سابقہ, repo);
}
