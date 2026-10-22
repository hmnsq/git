#تعریف_کړه USE_THE_REPOSITORY_VARIABLE
#تعریف_کړه DISABLE_SIGN_COMPARE_WARNINGS

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

ساكن ثابت حرف *تشه_بنسټ = "";

ساكن حرف ثابت * ثابت builtin_sparse_checkout_usage[] = {
	N_("git sparse-checkout (init | list | set | add | reapply | disable | check-rules | clean) [<options>]"),
	عدم
};

ساكن فراغ write_patterns_منزل_file(FILE *fp, بنية نمونه_list *pl)
{
	صحيح i;

	لكل (i = 0; i < pl->nr; i++) {
		بنية لار_نمونه *p = pl->patterns[i];

		إذا (p->flags & نمونه_FLAG_NEGATIVE)
			fprintf(fp, "!");

		fprintf(fp, "%s", p->نمونه);

		إذا (p->flags & نمونه_FLAG_MUSTBEDIR)
			fprintf(fp, "/");

		fprintf(fp, "\n");
	}
}

ساكن حرف ثابت * ثابت builtin_sparse_checkout_list_usage[] = {
	"git sparse-checkout list",
	عدم
};

ساكن صحيح sparse_checkout_list(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی,
				بنية repository *repo ناکارول_شوی)
{
	ساكن بنية option builtin_sparse_checkout_list_options[] = {
		OPT_پای_ټکی(),
	};
	بنية نمونه_list pl;
	حرف *sparse_filename;
	صحيح res;
	بنية repo_config_ارزښتونه *cfg = repo_config_ارزښتونه(the_repository);

	تیاری_work_ونه(the_repository);
	إذا (!cfg->apply_sparse_checkout)
		die(_("this worktree is not sparse"));

	argc = parse_options(argc, argv, مختاړی,
			     builtin_sparse_checkout_list_options,
			     builtin_sparse_checkout_list_usage, 0);

	memset(&pl, 0, حجم(pl));

	pl.use_cone_patterns = cfg->core_sparse_checkout_cone;

	sparse_filename = get_sparse_checkout_filename();
	res = جمع_کړه_patterns_from_file_منزل_list(sparse_filename, "", 0, &pl, عدم, 0);
	حرر(sparse_filename);

	إذا (res < 0) {
		warning(_("this worktree is not sparse (sparse-checkout file may not exist)"));
		أرجع 0;
	}

	إذا (pl.use_cone_patterns) {
		صحيح i;
		بنية نمونه_ثبت *pe;
		بنية hashmap_iter iter;
		بنية string_list sl = STRING_LIST_INIT_DUP;

		hashmap_for_each_ثبت(&pl.recursive_hashmap, &iter, pe, ent) {
			/* pe->pattern starts with "/", skip it */
			string_list_append(&sl, pe->نمونه + 1);
		}

		string_list_ترتیبول_u(&sl, 0);

		لكل (i = 0; i < sl.nr; i++) {
			quote_c_style(sl.توکي[i].string, عدم, stdout, 0);
			چاپ_کړه("\n");
		}

		string_list_clear(&sl, 0);
	} وإلا {
		write_patterns_منزل_file(stdout, &pl);
	}

	clear_نمونه_list(&pl);

	أرجع 0;
}

ساكن فراغ clean_tracked_sparse_directories(بنية repository *r)
{
	صحيح i, was_ډک = 0;
	بنية strbuf لار = STRBUF_INIT;
	اندازه_t pathlen;
	بنية string_list_توکی *توکی;
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
		إذا (convert_منزل_sparse(r->شاخص, SPARSE_شاخص_حافظه_ONLY))
			أرجع;
		was_ډک = 1;
	}

	strbuf_addstr(&لار, r->worktree);
	strbuf_complete(&لار, '/');
	pathlen = لار.len;

	/*
	 * Collect directories that have gone out of scope but also
	 * exist on disk, so there is some work to be done. We need to
	 * store the entries in a list before exploring, since that might
	 * expand the sparse-index again.
	 */
	لكل (i = 0; i < r->شاخص->cache_nr; i++) {
		بنية cache_ثبت *ce = r->شاخص->cache[i];

		إذا (S_ISSPARSEDIR(ce->ce_ډېرپېښ) &&
		    repo_file_exists(r, ce->name))
			string_list_append(&sparse_dirs, ce->name);
	}

	for_each_string_list_توکی(توکی, &sparse_dirs) {
		بنية dir_struct dir = DIR_INIT;
		بنية pathspec p = { 0 };
		بنية strvec s = STRVEC_INIT;

		strbuf_setlen(&لار, pathlen);
		strbuf_addstr(&لار, توکی->string);

		dir.flags |= DIR_SHOW_IGNORED_TOO;

		تیاری_standard_excludes(&dir);
		strvec_ټېل(&s, لار.buf);

		parse_pathspec(&p, PATHSPEC_GLOB, 0, عدم, s.v);
		fill_directory(&dir, r->شاخص, &p);

		إذا (dir.nr) {
			warning(_("directory '%s' contains untracked files,"
				  " but is not in the sparse-checkout cone"),
				توکی->string);
		} وإلا إذا (remove_dir_recursively(&لار, 0)) {
			/*
			 * Removal is "best effort". If something blocks
			 * the deletion, then continue with a warning.
			 */
			warning(_("failed to remove directory '%s'"),
				توکی->string);
		}

		strvec_clear(&s);
		clear_pathspec(&p);
		dir_clear(&dir);
	}

	string_list_clear(&sparse_dirs, 0);
	strbuf_release(&لار);

	إذا (was_ډک)
		ensure_ډک_شاخص(r->شاخص);
}

ساكن صحيح update_working_directory(بنية repository *r,
				    بنية نمونه_list *pl)
{
	تعداد update_sparsity_پایله پایله;
	بنية unpack_trees_options o;
	بنية قلف_file قلف_file = قلف_INIT;
	بنية نمونه_list *old_pl;

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

	تیاری_work_ونه(the_repository);

	repo_hold_locked_شاخص(r, &قلف_file, قلف_DIE_ON_ERROR);

	تیاری_unpack_trees_porcelain(&o, "sparse-checkout");
	پایله = update_sparsity(&o, pl);
	clear_unpack_trees_porcelain(&o);

	إذا (پایله == UPDATE_SPARSITY_WARNINGS)
		/*
		 * We don't do any special handling of warnings from untracked
		 * files in the way or dirty entries that can't be removed.
		 */
		پایله = UPDATE_SPARSITY_SUCCESS;
	إذا (پایله == UPDATE_SPARSITY_SUCCESS)
		write_locked_شاخص(r->شاخص, &قلف_file, COMMIT_قلف);
	وإلا
		rollback_قلف_file(&قلف_file);

	clean_tracked_sparse_directories(r);

	إذا (r->شاخص->sparse_checkout_patterns != pl) {
		clear_نمونه_list(r->شاخص->sparse_checkout_patterns);
		FREE_AND_NULL(r->شاخص->sparse_checkout_patterns);
	}
	r->شاخص->sparse_checkout_patterns = old_pl;

	أرجع پایله;
}

ساكن حرف *escaped_نمونه(حرف *نمونه)
{
	حرف *p = نمونه;
	بنية strbuf final = STRBUF_INIT;

	طالما (*p) {
		إذا (is_glob_special(*p))
			strbuf_addch(&final, '\\');

		strbuf_addch(&final, *p);
		p++;
	}

	أرجع strbuf_detach(&final, عدم);
}

ساكن فراغ write_cone_منزل_file(FILE *fp, بنية نمونه_list *pl)
{
	صحيح i;
	بنية نمونه_ثبت *pe;
	بنية hashmap_iter iter;
	بنية string_list sl = STRING_LIST_INIT_DUP;
	بنية strbuf پلار_نمونه = STRBUF_INIT;

	hashmap_for_each_ثبت(&pl->پلار_hashmap, &iter, pe, ent) {
		إذا (hashmap_get_ثبت(&pl->recursive_hashmap, pe, ent, عدم))
			استمر;

		إذا (!hashmap_لري_پلار(&pl->recursive_hashmap,
					     pe->نمونه,
					     &پلار_نمونه))
			string_list_append(&sl, pe->نمونه);
	}

	string_list_ترتیبول_u(&sl, 0);

	fprintf(fp, "/*\n!/*/\n");

	لكل (i = 0; i < sl.nr; i++) {
		حرف *نمونه = escaped_نمونه(sl.توکي[i].string);

		إذا (مزي_اوږدوالی(نمونه))
			fprintf(fp, "%s/\n!%s/*/\n", نمونه, نمونه);
		حرر(نمونه);
	}

	string_list_clear(&sl, 0);

	hashmap_for_each_ثبت(&pl->recursive_hashmap, &iter, pe, ent) {
		إذا (!hashmap_لري_پلار(&pl->recursive_hashmap,
					     pe->نمونه,
					     &پلار_نمونه))
			string_list_append(&sl, pe->نمونه);
	}

	strbuf_release(&پلار_نمونه);

	string_list_ترتیبول_u(&sl, 0);

	لكل (i = 0; i < sl.nr; i++) {
		حرف *نمونه = escaped_نمونه(sl.توکي[i].string);
		fprintf(fp, "%s/\n", نمونه);
		حرر(نمونه);
	}

	string_list_clear(&sl, 0);
}

ساكن صحيح write_patterns_and_update(بنية repository *repo,
				     بنية نمونه_list *pl)
{
	حرف *sparse_filename;
	FILE *fp;
	بنية قلف_file lk = قلف_INIT;
	صحيح پایله;
	بنية repo_config_ارزښتونه *cfg = repo_config_ارزښتونه(the_repository);

	sparse_filename = get_sparse_checkout_filename();

	إذا (خوندي_create_leading_directories(repo, sparse_filename))
		die(_("failed to create directory for sparse-checkout file"));

	repo_hold_قلف_file_for_update(repo, &lk, sparse_filename,
				       قلف_DIE_ON_ERROR);

	پایله = update_working_directory(repo, pl);
	إذا (پایله) {
		rollback_قلف_file(&lk);
		update_working_directory(repo, عدم);
		اذهب بهر;
	}

	fp = fdopen_قلف_file(&lk, "w");
	إذا (!fp)
		die_errno(_("unable to fdopen %s"), get_قلف_file_لار(&lk));

	إذا (cfg->core_sparse_checkout_cone)
		write_cone_منزل_file(fp, pl);
	وإلا
		write_patterns_منزل_file(fp, pl);

	إذا (commit_قلف_file(&lk))
		die_errno(_("unable to write %s"), sparse_filename);

بهر:
	clear_نمونه_list(pl);
	حرر(sparse_filename);
	أرجع پایله;
}

تعداد sparse_checkout_ډېرپېښ {
	ډېرپېښ_NO_PATTERNS = 0,
	ډېرپېښ_ALL_PATTERNS = 1,
	ډېرپېښ_CONE_PATTERNS = 2,
};

ساكن صحيح سېټ_config(بنية repository *repo,
		      تعداد sparse_checkout_ډېرپېښ ډېرپېښ)
{
	/* Update to use worktree config, if not already. */
	إذا (init_worktree_config(repo)) {
		error(_("failed to initialize worktree config"));
		أرجع 1;
	}

	إذا (repo_config_سېټ_worktree_gently(repo,
					    "core.sparseCheckout",
					    ډېرپېښ ? "true" : "false") ||
	    repo_config_سېټ_worktree_gently(repo,
					    "core.sparseCheckoutCone",
					    ډېرپېښ == ډېرپېښ_CONE_PATTERNS ?
						"true" : "false"))
		أرجع 1;

	إذا (ډېرپېښ == ډېرپېښ_NO_PATTERNS)
		أرجع سېټ_sparse_شاخص_config(repo, 0);

	أرجع 0;
}

ساكن تعداد sparse_checkout_ډېرپېښ update_cone_ډېرپېښ(صحيح *cone_ډېرپېښ) {
	بنية repo_config_ارزښتونه *cfg = repo_config_ارزښتونه(the_repository);

	/* If not specified, use previous definition of cone mode */
	إذا (*cone_ډېرپېښ == -1 && cfg->apply_sparse_checkout)
		*cone_ډېرپېښ = cfg->core_sparse_checkout_cone;

	/* Set cone/non-cone mode appropriately */
	cfg->apply_sparse_checkout = 1;
	إذا (*cone_ډېرپېښ == 1 || *cone_ډېرپېښ == -1) {
		cfg->core_sparse_checkout_cone = 1;
		أرجع ډېرپېښ_CONE_PATTERNS;
	}
	cfg->core_sparse_checkout_cone = 0;
	أرجع ډېرپېښ_ALL_PATTERNS;
}

ساكن صحيح update_modes(بنية repository *repo, صحيح *cone_ډېرپېښ, صحيح *sparse_شاخص)
{
	صحيح ډېرپېښ, ریکارډ_ډېرپېښ;
	بنية repo_config_ارزښتونه *cfg = repo_config_ارزښتونه(the_repository);

	/* Determine if we need to record the mode; ensure sparse checkout on */
	ریکارډ_ډېرپېښ = (*cone_ډېرپېښ != -1) || !cfg->apply_sparse_checkout;

	ډېرپېښ = update_cone_ډېرپېښ(cone_ډېرپېښ);
	إذا (ریکارډ_ډېرپېښ && سېټ_config(repo, ډېرپېښ))
		أرجع 1;

	/* Set sparse-index/non-sparse-index mode if specified */
	إذا (*sparse_شاخص >= 0) {
		إذا (سېټ_sparse_شاخص_config(repo, *sparse_شاخص) < 0)
			die(_("failed to modify sparse-index config"));

		/* force an index rewrite */
		repo_read_شاخص(repo);
		repo->شاخص->updated_workdir = 1;

		إذا (!*sparse_شاخص)
			ensure_ډک_شاخص(repo->شاخص);
	}

	أرجع 0;
}

ساكن حرف ثابت * ثابت builtin_sparse_checkout_init_usage[] = {
	"git sparse-checkout init [--cone] [--[no-]sparse-index]",
	عدم
};

ساكن بنية sparse_checkout_init_opts {
	صحيح cone_ډېرپېښ;
	صحيح sparse_شاخص;
} init_opts;

ساكن صحيح sparse_checkout_init(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی,
				بنية repository *repo)
{
	بنية نمونه_list pl;
	حرف *sparse_filename;
	صحيح res;
	بنية object_id oid;

	ساكن بنية option builtin_sparse_checkout_init_options[] = {
		OPT_BOOL(0, "cone", &init_opts.cone_ډېرپېښ,
			 N_("initialize the sparse-checkout in cone mode")),
		OPT_BOOL(0, "sparse-index", &init_opts.sparse_شاخص,
			 N_("toggle the use of a sparse index")),
		OPT_پای_ټکی(),
	};

	تیاری_work_ونه(the_repository);
	repo_read_شاخص(repo);

	init_opts.cone_ډېرپېښ = -1;
	init_opts.sparse_شاخص = -1;

	argc = parse_options(argc, argv, مختاړی,
			     builtin_sparse_checkout_init_options,
			     builtin_sparse_checkout_init_usage, 0);

	إذا (update_modes(repo, &init_opts.cone_ډېرپېښ, &init_opts.sparse_شاخص))
		أرجع 1;

	memset(&pl, 0, حجم(pl));

	sparse_filename = get_sparse_checkout_filename();
	res = جمع_کړه_patterns_from_file_منزل_list(sparse_filename, "", 0, &pl, عدم, 0);

	/* If we already have a sparse-checkout file, use it. */
	إذا (res >= 0) {
		حرر(sparse_filename);
		clear_نمونه_list(&pl);
		أرجع update_working_directory(repo, عدم);
	}

	إذا (repo_get_oid(repo, "HEAD", &oid)) {
		FILE *fp;

		/* assume we are in a fresh repo, but update the sparse-checkout file */
		إذا (خوندي_create_leading_directories(repo, sparse_filename))
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

	جمع_کړه_نمونه("/*", تشه_بنسټ, 0, &pl, 0);
	جمع_کړه_نمونه("!/*/", تشه_بنسټ, 0, &pl, 0);
	pl.use_cone_patterns = init_opts.cone_ډېرپېښ;

	أرجع write_patterns_and_update(repo, &pl);
}

ساكن فراغ ورننباسه_recursive_نمونه(بنية نمونه_list *pl, بنية strbuf *لار)
{
	بنية نمونه_ثبت *e = xmalloc(حجم(*e));
	e->patternlen = لار->len;
	e->نمونه = strbuf_detach(لار, عدم);
	hashmap_ثبت_init(&e->ent, fspathhash(e->نمونه));

	hashmap_جمع_کړه(&pl->recursive_hashmap, &e->ent);

	طالما (e->patternlen) {
		حرف *slash = strrchr(e->نمونه, '/');
		حرف *oldpattern = e->نمونه;
		اندازه_t newlen;
		بنية نمونه_ثبت *dup;

		إذا (!slash || slash == e->نمونه)
			اكسر;

		newlen = slash - e->نمونه;
		e = xmalloc(حجم(بنية نمونه_ثبت));
		e->patternlen = newlen;
		e->نمونه = xstrndup(oldpattern, newlen);
		hashmap_ثبت_init(&e->ent, fspathhash(e->نمونه));

		dup = hashmap_get_ثبت(&pl->پلار_hashmap, e, ent, عدم);
		إذا (!dup) {
			hashmap_جمع_کړه(&pl->پلار_hashmap, &e->ent);
		} وإلا {
			حرر(e->نمونه);
			حرر(e);
			e = dup;
		}
	}
}

ساكن فراغ strbuf_منزل_cone_نمونه(بنية strbuf *کرښه, بنية نمونه_list *pl)
{
	strbuf_trim(کرښه);

	strbuf_trim_trailing_dir_sep(کرښه);

	إذا (strbuf_normalize_لار(کرښه))
		die(_("could not normalize path %s"), کرښه->buf);

	إذا (!کرښه->len)
		أرجع;

	إذا (کرښه->buf[0] != '/')
		strbuf_insertstr(کرښه, 0, "/");

	ورننباسه_recursive_نمونه(pl, کرښه);
}

ساكن فراغ جمع_کړه_patterns_from_ننوت(بنية نمونه_list *pl,
				    صحيح argc, ثابت حرف **argv,
				    FILE *file)
{
	صحيح i;
	بنية repo_config_ارزښتونه *cfg = repo_config_ارزښتونه(the_repository);

	إذا (cfg->core_sparse_checkout_cone) {
		بنية strbuf کرښه = STRBUF_INIT;

		hashmap_init(&pl->recursive_hashmap, pl_hashmap_cmp, عدم, 0);
		hashmap_init(&pl->پلار_hashmap, pl_hashmap_cmp, عدم, 0);
		pl->use_cone_patterns = 1;

		إذا (file) {
			بنية strbuf unquoted = STRBUF_INIT;
			طالما (!strbuf_getline(&کرښه, file)) {
				إذا (کرښه.buf[0] == '"') {
					strbuf_reset(&unquoted);
					إذا (unquote_c_style(&unquoted, کرښه.buf, عدم))
						die(_("unable to unquote C-style string '%s'"),
						کرښه.buf);

					strbuf_بدلون(&unquoted, &کرښه);
				}

				strbuf_منزل_cone_نمونه(&کرښه, pl);
			}

			strbuf_release(&unquoted);
		} وإلا {
			لكل (i = 0; i < argc; i++) {
				strbuf_setlen(&کرښه, 0);
				strbuf_addstr(&کرښه, argv[i]);
				strbuf_منزل_cone_نمونه(&کرښه, pl);
			}
		}
		strbuf_release(&کرښه);
	} وإلا {
		إذا (file) {
			بنية strbuf کرښه = STRBUF_INIT;

			طالما (!strbuf_getline(&کرښه, file))
				جمع_کړه_نمونه(کرښه.buf, تشه_بنسټ, 0, pl, 0);

			strbuf_release(&کرښه);
		} وإلا {
			لكل (i = 0; i < argc; i++)
				جمع_کړه_نمونه(argv[i], تشه_بنسټ, 0, pl, 0);
		}
	}
}

تعداد modify_type {
	REPLACE,
	جمع_کړه_2,
};

ساكن فراغ جمع_کړه_patterns_cone_ډېرپېښ(صحيح argc, ثابت حرف **argv,
				   بنية نمونه_list *pl,
				   صحيح use_stdin)
{
	بنية strbuf بفر = STRBUF_INIT;
	بنية نمونه_ثبت *pe;
	بنية hashmap_iter iter;
	بنية نمونه_list existing;
	بنية repo_config_ارزښتونه *cfg = repo_config_ارزښتونه(the_repository);
	حرف *sparse_filename = get_sparse_checkout_filename();

	جمع_کړه_patterns_from_ننوت(pl, argc, argv,
				use_stdin ? stdin : عدم);

	memset(&existing, 0, حجم(existing));
	existing.use_cone_patterns = cfg->core_sparse_checkout_cone;

	إذا (جمع_کړه_patterns_from_file_منزل_list(sparse_filename, "", 0,
					   &existing, عدم, 0))
		die(_("unable to load existing sparse-checkout patterns"));
	حرر(sparse_filename);

	إذا (!existing.use_cone_patterns)
		die(_("existing sparse-checkout patterns do not use cone mode"));

	hashmap_for_each_ثبت(&existing.recursive_hashmap, &iter, pe, ent) {
		إذا (!hashmap_لري_پلار(&pl->recursive_hashmap,
					pe->نمونه, &بفر) ||
		    !hashmap_لري_پلار(&pl->پلار_hashmap,
					pe->نمونه, &بفر)) {
			strbuf_reset(&بفر);
			strbuf_addstr(&بفر, pe->نمونه);
			ورننباسه_recursive_نمونه(pl, &بفر);
		}
	}

	clear_نمونه_list(&existing);
	strbuf_release(&بفر);
}

ساكن فراغ جمع_کړه_patterns_literal(صحيح argc, ثابت حرف **argv,
				 بنية نمونه_list *pl,
				 صحيح use_stdin)
{
	حرف *sparse_filename = get_sparse_checkout_filename();
	إذا (جمع_کړه_patterns_from_file_منزل_list(sparse_filename, "", 0,
					   pl, عدم, 0))
		die(_("unable to load existing sparse-checkout patterns"));
	حرر(sparse_filename);
	جمع_کړه_patterns_from_ننوت(pl, argc, argv, use_stdin ? stdin : عدم);
}

ساكن صحيح modify_نمونه_list(بنية repository *repo,
			       بنية strvec *دلیلونه, صحيح use_stdin,
			       تعداد modify_type m)
{
	صحيح پایله;
	صحيح بدل_شوی_config = 0;
	بنية نمونه_list *pl = xcalloc(1, حجم(*pl));
	بنية repo_config_ارزښتونه *cfg = repo_config_ارزښتونه(the_repository);

	بدل (m) {
	حالة جمع_کړه_2:
		إذا (cfg->core_sparse_checkout_cone)
			جمع_کړه_patterns_cone_ډېرپېښ(دلیلونه->nr, دلیلونه->v, pl, use_stdin);
		وإلا
			جمع_کړه_patterns_literal(دلیلونه->nr, دلیلونه->v, pl, use_stdin);
		اكسر;

	حالة REPLACE:
		جمع_کړه_patterns_from_ننوت(pl, دلیلونه->nr, دلیلونه->v,
					use_stdin ? stdin : عدم);
		اكسر;
	}

	إذا (!cfg->apply_sparse_checkout) {
		سېټ_config(repo, ډېرپېښ_ALL_PATTERNS);
		cfg->apply_sparse_checkout = 1;
		بدل_شوی_config = 1;
	}

	پایله = write_patterns_and_update(repo, pl);

	إذا (پایله && بدل_شوی_config)
		سېټ_config(repo, ډېرپېښ_NO_PATTERNS);

	clear_نمونه_list(pl);
	حرر(pl);
	أرجع پایله;
}

ساكن فراغ sanitize_paths(بنية repository *repo,
			   بنية strvec *دلیلونه,
			   ثابت حرف *مختاړی, صحيح skip_checks)
{
	صحيح i;
	بنية repo_config_ارزښتونه *cfg = repo_config_ارزښتونه(the_repository);

	إذا (!دلیلونه->nr)
		أرجع;

	إذا (مختاړی && *مختاړی && cfg->core_sparse_checkout_cone) {
		/*
		 * The args are not pathspecs, so unfortunately we
		 * cannot imitate how cmd_add() uses parse_pathspec().
		 */
		صحيح مختاړی_len = مزي_اوږدوالی(مختاړی);

		لكل (i = 0; i < دلیلونه->nr; i++) {
			حرف *prefixed_لار = مختاړی_لار(the_repository, مختاړی,
							  مختاړی_len, دلیلونه->v[i]);
			strvec_replace(دلیلونه, i, prefixed_لار);
			حرر(prefixed_لار);
		}
	}

	إذا (skip_checks)
		أرجع;

	إذا (مختاړی && *مختاړی && !cfg->core_sparse_checkout_cone)
		die(_("please run from the toplevel directory in non-cone mode"));

	إذا (cfg->core_sparse_checkout_cone) {
		لكل (i = 0; i < دلیلونه->nr; i++) {
			إذا (دلیلونه->v[i][0] == '/')
				die(_("specify directories rather than patterns (no leading slash)"));
			إذا (دلیلونه->v[i][0] == '!')
				die(_("specify directories rather than patterns.  If your directory starts with a '!', pass --skip-checks"));
			إذا (strpbrk(دلیلونه->v[i], "*?[]"))
				die(_("specify directories rather than patterns.  If your directory really has any of '*?[]\\' in it, pass --skip-checks"));
		}
	}

	لكل (i = 0; i < دلیلونه->nr; i++) {
		بنية cache_ثبت *ce;
		بنية شاخص_state *شاخص = repo->شاخص;
		صحيح pos = شاخص_name_pos(شاخص, دلیلونه->v[i], مزي_اوږدوالی(دلیلونه->v[i]));

		إذا (pos < 0)
			استمر;
		ce = شاخص->cache[pos];
		إذا (S_ISSPARSEDIR(ce->ce_ډېرپېښ))
			استمر;

		إذا (cfg->core_sparse_checkout_cone)
			die(_("'%s' is not a directory; to treat it as a directory anyway, rerun with --skip-checks"), دلیلونه->v[i]);
		وإلا
			warning(_("pass a leading slash before paths such as '%s' if you want a single file (see NON-CONE PROBLEMS in the git-sparse-checkout manual)."), دلیلونه->v[i]);
	}
}

ساكن حرف ثابت * ثابت builtin_sparse_checkout_جمع_کړه_usage[] = {
	N_("git sparse-checkout add [--skip-checks] (--stdin | <patterns>)"),
	عدم
};

ساكن بنية sparse_checkout_جمع_کړه_opts {
	صحيح skip_checks;
	صحيح use_stdin;
} جمع_کړه_opts;

ساكن صحيح sparse_checkout_جمع_کړه(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی,
			       بنية repository *repo)
{
	ساكن بنية option builtin_sparse_checkout_جمع_کړه_options[] = {
		OPT_BOOL_F(0, "skip-checks", &جمع_کړه_opts.skip_checks,
			   N_("skip some sanity checks on the given paths that might give false positives"),
			   PARSE_OPT_NONEG),
		OPT_BOOL(0, "stdin", &جمع_کړه_opts.use_stdin,
			 N_("read patterns from standard in")),
		OPT_پای_ټکی(),
	};
	بنية strvec patterns = STRVEC_INIT;
	صحيح ret;
	بنية repo_config_ارزښتونه *cfg = repo_config_ارزښتونه(the_repository);

	تیاری_work_ونه(the_repository);
	إذا (!cfg->apply_sparse_checkout)
		die(_("no sparse-checkout to add to"));

	repo_read_شاخص(repo);

	argc = parse_options(argc, argv, مختاړی,
			     builtin_sparse_checkout_جمع_کړه_options,
			     builtin_sparse_checkout_جمع_کړه_usage, 0);

	لكل (صحيح i = 0; i < argc; i++)
		strvec_ټېل(&patterns, argv[i]);
	sanitize_paths(repo, &patterns, مختاړی, جمع_کړه_opts.skip_checks);

	ret = modify_نمونه_list(repo, &patterns, جمع_کړه_opts.use_stdin, جمع_کړه_2);

	strvec_clear(&patterns);
	أرجع ret;
}

ساكن حرف ثابت * ثابت builtin_sparse_checkout_سېټ_usage[] = {
	N_("git sparse-checkout set [--[no-]cone] [--[no-]sparse-index] [--skip-checks] (--stdin | <patterns>)"),
	عدم
};

ساكن بنية sparse_checkout_سېټ_opts {
	صحيح cone_ډېرپېښ;
	صحيح sparse_شاخص;
	صحيح skip_checks;
	صحيح use_stdin;
} سېټ_opts;

ساكن صحيح sparse_checkout_سېټ(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی,
			       بنية repository *repo)
{
	بنية repo_config_ارزښتونه *cfg = repo_config_ارزښتونه(the_repository);
	صحيح default_patterns_nr = 2;
	ثابت حرف *default_patterns[] = {"/*", "!/*/", عدم};

	ساكن بنية option builtin_sparse_checkout_سېټ_options[] = {
		OPT_BOOL(0, "cone", &سېټ_opts.cone_ډېرپېښ,
			 N_("initialize the sparse-checkout in cone mode")),
		OPT_BOOL(0, "sparse-index", &سېټ_opts.sparse_شاخص,
			 N_("toggle the use of a sparse index")),
		OPT_BOOL_F(0, "skip-checks", &سېټ_opts.skip_checks,
			   N_("skip some sanity checks on the given paths that might give false positives"),
			   PARSE_OPT_NONEG),
		OPT_BOOL_F(0, "stdin", &سېټ_opts.use_stdin,
			   N_("read patterns from standard in"),
			   PARSE_OPT_NONEG),
		OPT_پای_ټکی(),
	};
	بنية strvec patterns = STRVEC_INIT;
	صحيح ret;

	تیاری_work_ونه(the_repository);
	repo_read_شاخص(repo);

	سېټ_opts.cone_ډېرپېښ = -1;
	سېټ_opts.sparse_شاخص = -1;

	argc = parse_options(argc, argv, مختاړی,
			     builtin_sparse_checkout_سېټ_options,
			     builtin_sparse_checkout_سېټ_usage, 0);

	إذا (update_modes(repo, &سېټ_opts.cone_ډېرپېښ, &سېټ_opts.sparse_شاخص))
		أرجع 1;

	/*
	 * Cone mode automatically specifies the toplevel directory.  For
	 * non-cone mode, if nothing is specified, manually select just the
	 * top-level directory (much as 'init' would do).
	 */
	إذا (!cfg->core_sparse_checkout_cone && !سېټ_opts.use_stdin && argc == 0) {
		لكل (صحيح i = 0; i < default_patterns_nr; i++)
			strvec_ټېل(&patterns, default_patterns[i]);
	} وإلا {
		لكل (صحيح i = 0; i < argc; i++)
			strvec_ټېل(&patterns, argv[i]);
		sanitize_paths(repo, &patterns, مختاړی, سېټ_opts.skip_checks);
	}

	ret = modify_نمونه_list(repo, &patterns, سېټ_opts.use_stdin, REPLACE);

	strvec_clear(&patterns);
	أرجع ret;
}

ساكن حرف ثابت * ثابت builtin_sparse_checkout_reapply_usage[] = {
	"git sparse-checkout reapply [--[no-]cone] [--[no-]sparse-index]",
	عدم
};

ساكن بنية sparse_checkout_reapply_opts {
	صحيح cone_ډېرپېښ;
	صحيح sparse_شاخص;
} reapply_opts;

ساكن صحيح sparse_checkout_reapply(صحيح argc, ثابت حرف **argv,
				   ثابت حرف *مختاړی,
				   بنية repository *repo)
{
	ساكن بنية option builtin_sparse_checkout_reapply_options[] = {
		OPT_BOOL(0, "cone", &reapply_opts.cone_ډېرپېښ,
			 N_("initialize the sparse-checkout in cone mode")),
		OPT_BOOL(0, "sparse-index", &reapply_opts.sparse_شاخص,
			 N_("toggle the use of a sparse index")),
		OPT_پای_ټکی(),
	};
	بنية repo_config_ارزښتونه *cfg = repo_config_ارزښتونه(the_repository);

	تیاری_work_ونه(the_repository);
	إذا (!cfg->apply_sparse_checkout)
		die(_("must be in a sparse-checkout to reapply sparsity patterns"));

	reapply_opts.cone_ډېرپېښ = -1;
	reapply_opts.sparse_شاخص = -1;

	argc = parse_options(argc, argv, مختاړی,
			     builtin_sparse_checkout_reapply_options,
			     builtin_sparse_checkout_reapply_usage, 0);

	repo_read_شاخص(repo);

	إذا (update_modes(repo, &reapply_opts.cone_ډېرپېښ, &reapply_opts.sparse_شاخص))
		أرجع 1;

	أرجع update_working_directory(repo, عدم);
}

ساكن حرف ثابت * ثابت builtin_sparse_checkout_clean_usage[] = {
	"git sparse-checkout clean [-n|--dry-run]",
	عدم
};

ساكن صحيح list_file_iterator(ثابت حرف *لار, ثابت فراغ *معلومات)
{
	ثابت حرف *پیغام = معلومات;

	چاپ_کړه(پیغام, لار);
	أرجع 0;
}

ساكن فراغ list_every_file_دننه_dir(ثابت حرف *پیغام,
				   ثابت حرف *directory)
{
	بنية strbuf لار = STRBUF_INIT;

	strbuf_addstr(&لار, directory);
	for_each_file_دننه_dir(&لار, list_file_iterator, پیغام);
	strbuf_release(&لار);
}

ساكن ثابت حرف *پیغام_remove = N_("Removing %s\n");
ساكن ثابت حرف *پیغام_would_remove = N_("Would remove %s\n");

ساكن صحيح sparse_checkout_clean(صحيح argc, ثابت حرف **argv,
				   ثابت حرف *مختاړی,
				   بنية repository *repo)
{
	بنية strbuf ډک_لار = STRBUF_INIT;
	ثابت حرف *پیغام = پیغام_remove;
	اندازه_t worktree_len;
	صحيح force = 0, dry_پرلپسې = 0, verbose = 0;
	صحيح require_force = 1;
	بنية repo_config_ارزښتونه *cfg = repo_config_ارزښتونه(the_repository);

	بنية option builtin_sparse_checkout_clean_options[] = {
		OPT_DRY_پرلپسې(&dry_پرلپسې, N_("dry run")),
		OPT__FORCE(&force, N_("force"), PARSE_OPT_NOCOMPLETE),
		OPT__VERBOSE(&verbose, N_("report each affected file, not just directories")),
		OPT_پای_ټکی(),
	};

	تیاری_work_ونه(the_repository);
	إذا (!cfg->apply_sparse_checkout)
		die(_("must be in a sparse-checkout to clean directories"));
	إذا (!cfg->core_sparse_checkout_cone)
		die(_("must be in a cone-mode sparse-checkout to clean directories"));

	argc = parse_options(argc, argv, مختاړی,
			     builtin_sparse_checkout_clean_options,
			     builtin_sparse_checkout_clean_usage, 0);

	repo_config_get_bool(repo, "clean.requireforce", &require_force);
	إذا (require_force && !force && !dry_پرلپسې)
		die(_("for safety, refusing to clean without one of --force or --dry-run"));

	إذا (dry_پرلپسې)
		پیغام = پیغام_would_remove;

	إذا (repo_read_شاخص(repo) < 0)
		die(_("failed to read index"));

	إذا (convert_منزل_sparse(repo->شاخص, SPARSE_شاخص_حافظه_ONLY) ||
	    repo->شاخص->sparse_شاخص == شاخص_EXPANDED)
		die(_("failed to convert index to a sparse index; resolve merge conflicts and try again"));

	strbuf_addstr(&ډک_لار, repo->worktree);
	strbuf_addch(&ډک_لار, '/');
	worktree_len = ډک_لار.len;

	لكل (اندازه_t i = 0; i < repo->شاخص->cache_nr; i++) {
		بنية cache_ثبت *ce = repo->شاخص->cache[i];
		إذا (!S_ISSPARSEDIR(ce->ce_ډېرپېښ))
			استمر;
		strbuf_setlen(&ډک_لار, worktree_len);
		strbuf_جمع_کړه(&ډک_لار, ce->name, ce->ce_namelen);

		إذا (!is_directory(ډک_لار.buf))
			استمر;

		إذا (verbose)
			list_every_file_دننه_dir(پیغام, ce->name);
		وإلا
			چاپ_کړه(پیغام, ce->name);

		إذا (dry_پرلپسې <= 0 &&
		    remove_dir_recursively(&ډک_لار, 0))
			warning_errno(_("failed to remove '%s'"), ce->name);
	}

	strbuf_release(&ډک_لار);
	أرجع 0;
}

ساكن حرف ثابت * ثابت builtin_sparse_checkout_disable_usage[] = {
	"git sparse-checkout disable",
	عدم
};

ساكن صحيح sparse_checkout_disable(صحيح argc, ثابت حرف **argv,
				   ثابت حرف *مختاړی,
				   بنية repository *repo)
{
	ساكن بنية option builtin_sparse_checkout_disable_options[] = {
		OPT_پای_ټکی(),
	};
	بنية نمونه_list pl;
	بنية repo_config_ارزښتونه *cfg = repo_config_ارزښتونه(the_repository);

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

	تیاری_work_ونه(the_repository);
	argc = parse_options(argc, argv, مختاړی,
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
	hashmap_init(&pl.پلار_hashmap, pl_hashmap_cmp, عدم, 0);
	pl.use_cone_patterns = 0;
	cfg->apply_sparse_checkout = 1;

	جمع_کړه_نمونه("/*", تشه_بنسټ, 0, &pl, 0);

	prepare_repo_settings(the_repository);
	repo->settings.sparse_شاخص = 0;

	إذا (update_working_directory(repo, &pl))
		die(_("error while refreshing working directory"));

	clear_نمونه_list(&pl);
	أرجع سېټ_config(repo, ډېرپېښ_NO_PATTERNS);
}

ساكن حرف ثابت * ثابت builtin_sparse_checkout_check_قواعد_usage[] = {
	N_("git sparse-checkout check-rules [-z] [--skip-checks]"
	   "[--[no-]cone] [--rules-file <file>]"),
	عدم
};

ساكن بنية sparse_checkout_check_قواعد_opts {
	صحيح cone_ډېرپېښ;
	صحيح null_termination;
	حرف *قواعد_file;
} check_قواعد_opts;

ساكن صحيح check_قواعد(بنية repository *repo,
		       بنية نمونه_list *pl,
		       صحيح null_terminated)
{
	بنية strbuf کرښه = STRBUF_INIT;
	بنية strbuf unquoted = STRBUF_INIT;
	حرف *لار;
	صحيح کرښه_terminator = null_terminated ? 0 : '\n';
	strbuf_getline_fn getline_fn = null_terminated ? strbuf_getline_nul
		: strbuf_getline;
	repo->شاخص->sparse_checkout_patterns = pl;
	طالما (!getline_fn(&کرښه, stdin)) {
		لار = کرښه.buf;
		إذا (!null_terminated && کرښه.buf[0] == '"') {
			strbuf_reset(&unquoted);
			إذا (unquote_c_style(&unquoted, کرښه.buf, عدم))
				die(_("unable to unquote C-style string '%s'"),
					کرښه.buf);

			لار = unquoted.buf;
		}

		إذا (لار_دننه_sparse_checkout(لار, repo->شاخص))
			write_name_quoted(لار, stdout, کرښه_terminator);
	}
	strbuf_release(&کرښه);
	strbuf_release(&unquoted);

	أرجع 0;
}

ساكن صحيح sparse_checkout_check_قواعد(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی,
				       بنية repository *repo)
{
	ساكن بنية option builtin_sparse_checkout_check_قواعد_options[] = {
		OPT_BOOL('z', عدم, &check_قواعد_opts.null_termination,
			 N_("terminate input and output files by a NUL character")),
		OPT_BOOL(0, "cone", &check_قواعد_opts.cone_ډېرپېښ,
			 N_("when used with --rules-file interpret patterns as cone mode patterns")),
		OPT_FILENAME(0, "rules-file", &check_قواعد_opts.قواعد_file,
			 N_("use patterns in <file> instead of the current ones.")),
		OPT_پای_ټکی(),
	};

	FILE *fp;
	صحيح ret;
	بنية نمونه_list pl = {0};
	بنية repo_config_ارزښتونه *cfg = repo_config_ارزښتونه(the_repository);
	حرف *sparse_filename;
	check_قواعد_opts.cone_ډېرپېښ = -1;

	argc = parse_options(argc, argv, مختاړی,
			     builtin_sparse_checkout_check_قواعد_options,
			     builtin_sparse_checkout_check_قواعد_usage, 0);

	إذا (check_قواعد_opts.قواعد_file && check_قواعد_opts.cone_ډېرپېښ < 0)
		check_قواعد_opts.cone_ډېرپېښ = 1;

	update_cone_ډېرپېښ(&check_قواعد_opts.cone_ډېرپېښ);
	pl.use_cone_patterns = cfg->core_sparse_checkout_cone;
	إذا (check_قواعد_opts.قواعد_file) {
		fp = xfopen(check_قواعد_opts.قواعد_file, "r");
		جمع_کړه_patterns_from_ننوت(&pl, argc, argv, fp);
		fclose(fp);
	} وإلا {
		sparse_filename = get_sparse_checkout_filename();
		إذا (جمع_کړه_patterns_from_file_منزل_list(sparse_filename, "", 0, &pl,
						   عدم, 0))
			die(_("unable to load existing sparse-checkout patterns"));
		حرر(sparse_filename);
	}

	ret = check_قواعد(repo, &pl, check_قواعد_opts.null_termination);
	clear_نمونه_list(&pl);
	حرر(check_قواعد_opts.قواعد_file);
	أرجع ret;
}

صحيح cmd_sparse_checkout(صحيح argc,
			ثابت حرف **argv,
			ثابت حرف *مختاړی,
			بنية repository *repo)
{
	parse_opt_subcommand_fn *fn = عدم;
	بنية option builtin_sparse_checkout_options[] = {
		OPT_SUBCOMMAND("list", &fn, sparse_checkout_list),
		OPT_SUBCOMMAND("init", &fn, sparse_checkout_init),
		OPT_SUBCOMMAND("set", &fn, sparse_checkout_سېټ),
		OPT_SUBCOMMAND("add", &fn, sparse_checkout_جمع_کړه),
		OPT_SUBCOMMAND("reapply", &fn, sparse_checkout_reapply),
		OPT_SUBCOMMAND("clean", &fn, sparse_checkout_clean),
		OPT_SUBCOMMAND("disable", &fn, sparse_checkout_disable),
		OPT_SUBCOMMAND("check-rules", &fn, sparse_checkout_check_قواعد),
		OPT_پای_ټکی(),
	};

	argc = parse_options(argc, argv, مختاړی,
			     builtin_sparse_checkout_options,
			     builtin_sparse_checkout_usage, 0);

	repo_config(the_repository, git_default_config, عدم);

	prepare_repo_settings(repo);
	repo->settings.command_requires_ډک_شاخص = 0;

	أرجع fn(argc, argv, مختاړی, repo);
}
