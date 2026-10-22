#నిర్వచించు USE_THE_REPOSITORY_VARIABLE
#నిర్వచించు DISABLE_SIGN_COMPARE_WARNINGS

#అన్తర్భావ "builtin.h"
#అన్తర్భావ "abspath.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "dir.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "object-file.h"
#అన్తర్భావ "object-name.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "path.h"
#అన్తర్భావ "pathspec.h"
#అన్తర్భావ "strbuf.h"
#అన్తర్భావ "string-list.h"
#అన్తర్భావ "lockfile.h"
#అన్తర్భావ "unpack-trees.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "setup.h"
#అన్తర్భావ "sparse-index.h"
#అన్తర్భావ "worktree.h"

స్థిర స్థిరరాశి అక్షరం *ఖాళీలు_పునాది = "";

స్థిర అక్షరం స్థిరరాశి * స్థిరరాశి builtin_sparse_checkout_usage[] = {
	N_("git sparse-checkout (init | list | set | add | reapply | disable | check-rules | clean) [<options>]"),
	ఖాళీ
};

స్థిర శూన్య write_patterns_గమ్యం_file(FILE *fp, నిర్మాణం నమూనా_list *pl)
{
	పూర్ణాంకం i;

	కొరకు (i = 0; i < pl->nr; i++) {
		నిర్మాణం మార్గం_నమూనా *p = pl->patterns[i];

		ఒకవేళ (p->flags & నమూనా_FLAG_NEGATIVE)
			fprintf(fp, "!");

		fprintf(fp, "%s", p->నమూనా);

		ఒకవేళ (p->flags & నమూనా_FLAG_MUSTBEDIR)
			fprintf(fp, "/");

		fprintf(fp, "\n");
	}
}

స్థిర అక్షరం స్థిరరాశి * స్థిరరాశి builtin_sparse_checkout_list_usage[] = {
	"git sparse-checkout list",
	ఖాళీ
};

స్థిర పూర్ణాంకం sparse_checkout_list(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
				నిర్మాణం repository *repo వాడని)
{
	స్థిర నిర్మాణం option builtin_sparse_checkout_list_options[] = {
		OPT_చివర(),
	};
	నిర్మాణం నమూనా_list pl;
	అక్షరం *sparse_filename;
	పూర్ణాంకం res;
	నిర్మాణం repo_config_విలువలు *cfg = repo_config_విలువలు(the_repository);

	అమరిక_work_వృక్షం(the_repository);
	ఒకవేళ (!cfg->apply_sparse_checkout)
		die(_("this worktree is not sparse"));

	argc = parse_options(argc, argv, ఉపసర్గ,
			     builtin_sparse_checkout_list_options,
			     builtin_sparse_checkout_list_usage, 0);

	memset(&pl, 0, పరిమాణం(pl));

	pl.use_cone_patterns = cfg->core_sparse_checkout_cone;

	sparse_filename = get_sparse_checkout_filename();
	res = కూడు_patterns_from_file_గమ్యం_list(sparse_filename, "", 0, &pl, ఖాళీ, 0);
	విడుదల(sparse_filename);

	ఒకవేళ (res < 0) {
		warning(_("this worktree is not sparse (sparse-checkout file may not exist)"));
		తిరిగి 0;
	}

	ఒకవేళ (pl.use_cone_patterns) {
		పూర్ణాంకం i;
		నిర్మాణం నమూనా_నమోదు *pe;
		నిర్మాణం hashmap_iter iter;
		నిర్మాణం string_list sl = STRING_LIST_INIT_DUP;

		hashmap_for_each_నమోదు(&pl.recursive_hashmap, &iter, pe, ent) {
			/* pe->pattern starts with "/", skip it */
			string_list_append(&sl, pe->నమూనా + 1);
		}

		string_list_క్రమీకరించు_u(&sl, 0);

		కొరకు (i = 0; i < sl.nr; i++) {
			quote_c_style(sl.వస్తువులు[i].string, ఖాళీ, stdout, 0);
			రాయి("\n");
		}

		string_list_clear(&sl, 0);
	} లేకపోతే {
		write_patterns_గమ్యం_file(stdout, &pl);
	}

	clear_నమూనా_list(&pl);

	తిరిగి 0;
}

స్థిర శూన్య clean_tracked_sparse_directories(నిర్మాణం repository *r)
{
	పూర్ణాంకం i, was_నిండిన = 0;
	నిర్మాణం strbuf మార్గం = STRBUF_INIT;
	కొలత_t pathlen;
	నిర్మాణం string_list_అంశం *అంశం;
	నిర్మాణం string_list sparse_dirs = STRING_LIST_INIT_DUP;

	/*
	 * If we are not using cone mode patterns, then we cannot
	 * delete directories outside of the sparse cone.
	 */
	ఒకవేళ (!r || !r->సూచీ || !r->worktree)
		తిరిగి;
	ఒకవేళ (init_sparse_checkout_patterns(r->సూచీ) ||
	    !r->సూచీ->sparse_checkout_patterns->use_cone_patterns)
		తిరిగి;

	/*
	 * Use the sparse index as a data structure to assist finding
	 * directories that are safe to delete. This conversion to a
	 * sparse index will not delete directories that contain
	 * conflicted entries or submodules.
	 */
	ఒకవేళ (r->సూచీ->sparse_సూచీ == సూచీ_EXPANDED) {
		/*
		 * If something, such as a merge conflict or other concern,
		 * prevents us from converting to a sparse index, then do
		 * not try deleting files.
		 */
		ఒకవేళ (convert_గమ్యం_sparse(r->సూచీ, SPARSE_సూచీ_జ్ఞాపకం_ONLY))
			తిరిగి;
		was_నిండిన = 1;
	}

	strbuf_addstr(&మార్గం, r->worktree);
	strbuf_complete(&మార్గం, '/');
	pathlen = మార్గం.len;

	/*
	 * Collect directories that have gone out of scope but also
	 * exist on disk, so there is some work to be done. We need to
	 * store the entries in a list before exploring, since that might
	 * expand the sparse-index again.
	 */
	కొరకు (i = 0; i < r->సూచీ->cache_nr; i++) {
		నిర్మాణం cache_నమోదు *ce = r->సూచీ->cache[i];

		ఒకవేళ (S_ISSPARSEDIR(ce->ce_బహుళకం) &&
		    repo_file_exists(r, ce->name))
			string_list_append(&sparse_dirs, ce->name);
	}

	for_each_string_list_అంశం(అంశం, &sparse_dirs) {
		నిర్మాణం dir_struct dir = DIR_INIT;
		నిర్మాణం pathspec p = { 0 };
		నిర్మాణం strvec s = STRVEC_INIT;

		strbuf_setlen(&మార్గం, pathlen);
		strbuf_addstr(&మార్గం, అంశం->string);

		dir.flags |= DIR_SHOW_IGNORED_TOO;

		అమరిక_standard_excludes(&dir);
		strvec_నెట్టు(&s, మార్గం.buf);

		parse_pathspec(&p, PATHSPEC_GLOB, 0, ఖాళీ, s.v);
		fill_directory(&dir, r->సూచీ, &p);

		ఒకవేళ (dir.nr) {
			warning(_("directory '%s' contains untracked files,"
				  " but is not in the sparse-checkout cone"),
				అంశం->string);
		} లేకపోతే ఒకవేళ (remove_dir_recursively(&మార్గం, 0)) {
			/*
			 * Removal is "best effort". If something blocks
			 * the deletion, then continue with a warning.
			 */
			warning(_("failed to remove directory '%s'"),
				అంశం->string);
		}

		strvec_clear(&s);
		clear_pathspec(&p);
		dir_clear(&dir);
	}

	string_list_clear(&sparse_dirs, 0);
	strbuf_release(&మార్గం);

	ఒకవేళ (was_నిండిన)
		ensure_నిండిన_సూచీ(r->సూచీ);
}

స్థిర పూర్ణాంకం update_working_directory(నిర్మాణం repository *r,
				    నిర్మాణం నమూనా_list *pl)
{
	గణన update_sparsity_ఫలితం ఫలితం;
	నిర్మాణం unpack_trees_options o;
	నిర్మాణం తాళం_file తాళం_file = తాళం_INIT;
	నిర్మాణం నమూనా_list *old_pl;

	/* If no branch has been checked out, there are no updates to make. */
	ఒకవేళ (is_సూచీ_unborn(r->సూచీ))
		తిరిగి UPDATE_SPARSITY_SUCCESS;

	old_pl = r->సూచీ->sparse_checkout_patterns;
	r->సూచీ->sparse_checkout_patterns = pl;

	memset(&o, 0, పరిమాణం(o));
	o.verbose_update = isatty(2);
	o.update = 1;
	o.తల_idx = -1;
	o.src_సూచీ = r->సూచీ;
	o.dst_సూచీ = r->సూచీ;
	o.skip_sparse_checkout = 0;

	అమరిక_work_వృక్షం(the_repository);

	repo_hold_locked_సూచీ(r, &తాళం_file, తాళం_DIE_ON_ERROR);

	అమరిక_unpack_trees_porcelain(&o, "sparse-checkout");
	ఫలితం = update_sparsity(&o, pl);
	clear_unpack_trees_porcelain(&o);

	ఒకవేళ (ఫలితం == UPDATE_SPARSITY_WARNINGS)
		/*
		 * We don't do any special handling of warnings from untracked
		 * files in the way or dirty entries that can't be removed.
		 */
		ఫలితం = UPDATE_SPARSITY_SUCCESS;
	ఒకవేళ (ఫలితం == UPDATE_SPARSITY_SUCCESS)
		write_locked_సూచీ(r->సూచీ, &తాళం_file, COMMIT_తాళం);
	లేకపోతే
		rollback_తాళం_file(&తాళం_file);

	clean_tracked_sparse_directories(r);

	ఒకవేళ (r->సూచీ->sparse_checkout_patterns != pl) {
		clear_నమూనా_list(r->సూచీ->sparse_checkout_patterns);
		FREE_AND_NULL(r->సూచీ->sparse_checkout_patterns);
	}
	r->సూచీ->sparse_checkout_patterns = old_pl;

	తిరిగి ఫలితం;
}

స్థిర అక్షరం *escaped_నమూనా(అక్షరం *నమూనా)
{
	అక్షరం *p = నమూనా;
	నిర్మాణం strbuf final = STRBUF_INIT;

	అయితే (*p) {
		ఒకవేళ (is_glob_special(*p))
			strbuf_addch(&final, '\\');

		strbuf_addch(&final, *p);
		p++;
	}

	తిరిగి strbuf_detach(&final, ఖాళీ);
}

స్థిర శూన్య write_cone_గమ్యం_file(FILE *fp, నిర్మాణం నమూనా_list *pl)
{
	పూర్ణాంకం i;
	నిర్మాణం నమూనా_నమోదు *pe;
	నిర్మాణం hashmap_iter iter;
	నిర్మాణం string_list sl = STRING_LIST_INIT_DUP;
	నిర్మాణం strbuf మాతృ_నమూనా = STRBUF_INIT;

	hashmap_for_each_నమోదు(&pl->మాతృ_hashmap, &iter, pe, ent) {
		ఒకవేళ (hashmap_get_నమోదు(&pl->recursive_hashmap, pe, ent, ఖాళీ))
			కొనసాగు;

		ఒకవేళ (!hashmap_కలిగియుంది_మాతృ(&pl->recursive_hashmap,
					     pe->నమూనా,
					     &మాతృ_నమూనా))
			string_list_append(&sl, pe->నమూనా);
	}

	string_list_క్రమీకరించు_u(&sl, 0);

	fprintf(fp, "/*\n!/*/\n");

	కొరకు (i = 0; i < sl.nr; i++) {
		అక్షరం *నమూనా = escaped_నమూనా(sl.వస్తువులు[i].string);

		ఒకవేళ (అక్షరశ్రేణి_పొడవు(నమూనా))
			fprintf(fp, "%s/\n!%s/*/\n", నమూనా, నమూనా);
		విడుదల(నమూనా);
	}

	string_list_clear(&sl, 0);

	hashmap_for_each_నమోదు(&pl->recursive_hashmap, &iter, pe, ent) {
		ఒకవేళ (!hashmap_కలిగియుంది_మాతృ(&pl->recursive_hashmap,
					     pe->నమూనా,
					     &మాతృ_నమూనా))
			string_list_append(&sl, pe->నమూనా);
	}

	strbuf_release(&మాతృ_నమూనా);

	string_list_క్రమీకరించు_u(&sl, 0);

	కొరకు (i = 0; i < sl.nr; i++) {
		అక్షరం *నమూనా = escaped_నమూనా(sl.వస్తువులు[i].string);
		fprintf(fp, "%s/\n", నమూనా);
		విడుదల(నమూనా);
	}

	string_list_clear(&sl, 0);
}

స్థిర పూర్ణాంకం write_patterns_and_update(నిర్మాణం repository *repo,
				     నిర్మాణం నమూనా_list *pl)
{
	అక్షరం *sparse_filename;
	FILE *fp;
	నిర్మాణం తాళం_file lk = తాళం_INIT;
	పూర్ణాంకం ఫలితం;
	నిర్మాణం repo_config_విలువలు *cfg = repo_config_విలువలు(the_repository);

	sparse_filename = get_sparse_checkout_filename();

	ఒకవేళ (సురక్షిత_create_leading_directories(repo, sparse_filename))
		die(_("failed to create directory for sparse-checkout file"));

	repo_hold_తాళం_file_for_update(repo, &lk, sparse_filename,
				       తాళం_DIE_ON_ERROR);

	ఫలితం = update_working_directory(repo, pl);
	ఒకవేళ (ఫలితం) {
		rollback_తాళం_file(&lk);
		update_working_directory(repo, ఖాళీ);
		వెళ్ళు బహిర్గతం;
	}

	fp = fdopen_తాళం_file(&lk, "w");
	ఒకవేళ (!fp)
		die_errno(_("unable to fdopen %s"), get_తాళం_file_మార్గం(&lk));

	ఒకవేళ (cfg->core_sparse_checkout_cone)
		write_cone_గమ్యం_file(fp, pl);
	లేకపోతే
		write_patterns_గమ్యం_file(fp, pl);

	ఒకవేళ (commit_తాళం_file(&lk))
		die_errno(_("unable to write %s"), sparse_filename);

బహిర్గతం:
	clear_నమూనా_list(pl);
	విడుదల(sparse_filename);
	తిరిగి ఫలితం;
}

గణన sparse_checkout_బహుళకం {
	బహుళకం_NO_PATTERNS = 0,
	బహుళకం_ALL_PATTERNS = 1,
	బహుళకం_CONE_PATTERNS = 2,
};

స్థిర పూర్ణాంకం సమితి_config(నిర్మాణం repository *repo,
		      గణన sparse_checkout_బహుళకం బహుళకం)
{
	/* Update to use worktree config, if not already. */
	ఒకవేళ (init_worktree_config(repo)) {
		error(_("failed to initialize worktree config"));
		తిరిగి 1;
	}

	ఒకవేళ (repo_config_సమితి_worktree_gently(repo,
					    "core.sparseCheckout",
					    బహుళకం ? "true" : "false") ||
	    repo_config_సమితి_worktree_gently(repo,
					    "core.sparseCheckoutCone",
					    బహుళకం == బహుళకం_CONE_PATTERNS ?
						"true" : "false"))
		తిరిగి 1;

	ఒకవేళ (బహుళకం == బహుళకం_NO_PATTERNS)
		తిరిగి సమితి_sparse_సూచీ_config(repo, 0);

	తిరిగి 0;
}

స్థిర గణన sparse_checkout_బహుళకం update_cone_బహుళకం(పూర్ణాంకం *cone_బహుళకం) {
	నిర్మాణం repo_config_విలువలు *cfg = repo_config_విలువలు(the_repository);

	/* If not specified, use previous definition of cone mode */
	ఒకవేళ (*cone_బహుళకం == -1 && cfg->apply_sparse_checkout)
		*cone_బహుళకం = cfg->core_sparse_checkout_cone;

	/* Set cone/non-cone mode appropriately */
	cfg->apply_sparse_checkout = 1;
	ఒకవేళ (*cone_బహుళకం == 1 || *cone_బహుళకం == -1) {
		cfg->core_sparse_checkout_cone = 1;
		తిరిగి బహుళకం_CONE_PATTERNS;
	}
	cfg->core_sparse_checkout_cone = 0;
	తిరిగి బహుళకం_ALL_PATTERNS;
}

స్థిర పూర్ణాంకం update_modes(నిర్మాణం repository *repo, పూర్ణాంకం *cone_బహుళకం, పూర్ణాంకం *sparse_సూచీ)
{
	పూర్ణాంకం బహుళకం, రికార్డు_బహుళకం;
	నిర్మాణం repo_config_విలువలు *cfg = repo_config_విలువలు(the_repository);

	/* Determine if we need to record the mode; ensure sparse checkout on */
	రికార్డు_బహుళకం = (*cone_బహుళకం != -1) || !cfg->apply_sparse_checkout;

	బహుళకం = update_cone_బహుళకం(cone_బహుళకం);
	ఒకవేళ (రికార్డు_బహుళకం && సమితి_config(repo, బహుళకం))
		తిరిగి 1;

	/* Set sparse-index/non-sparse-index mode if specified */
	ఒకవేళ (*sparse_సూచీ >= 0) {
		ఒకవేళ (సమితి_sparse_సూచీ_config(repo, *sparse_సూచీ) < 0)
			die(_("failed to modify sparse-index config"));

		/* force an index rewrite */
		repo_read_సూచీ(repo);
		repo->సూచీ->updated_workdir = 1;

		ఒకవేళ (!*sparse_సూచీ)
			ensure_నిండిన_సూచీ(repo->సూచీ);
	}

	తిరిగి 0;
}

స్థిర అక్షరం స్థిరరాశి * స్థిరరాశి builtin_sparse_checkout_init_usage[] = {
	"git sparse-checkout init [--cone] [--[no-]sparse-index]",
	ఖాళీ
};

స్థిర నిర్మాణం sparse_checkout_init_opts {
	పూర్ణాంకం cone_బహుళకం;
	పూర్ణాంకం sparse_సూచీ;
} init_opts;

స్థిర పూర్ణాంకం sparse_checkout_init(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
				నిర్మాణం repository *repo)
{
	నిర్మాణం నమూనా_list pl;
	అక్షరం *sparse_filename;
	పూర్ణాంకం res;
	నిర్మాణం object_id oid;

	స్థిర నిర్మాణం option builtin_sparse_checkout_init_options[] = {
		OPT_BOOL(0, "cone", &init_opts.cone_బహుళకం,
			 N_("initialize the sparse-checkout in cone mode")),
		OPT_BOOL(0, "sparse-index", &init_opts.sparse_సూచీ,
			 N_("toggle the use of a sparse index")),
		OPT_చివర(),
	};

	అమరిక_work_వృక్షం(the_repository);
	repo_read_సూచీ(repo);

	init_opts.cone_బహుళకం = -1;
	init_opts.sparse_సూచీ = -1;

	argc = parse_options(argc, argv, ఉపసర్గ,
			     builtin_sparse_checkout_init_options,
			     builtin_sparse_checkout_init_usage, 0);

	ఒకవేళ (update_modes(repo, &init_opts.cone_బహుళకం, &init_opts.sparse_సూచీ))
		తిరిగి 1;

	memset(&pl, 0, పరిమాణం(pl));

	sparse_filename = get_sparse_checkout_filename();
	res = కూడు_patterns_from_file_గమ్యం_list(sparse_filename, "", 0, &pl, ఖాళీ, 0);

	/* If we already have a sparse-checkout file, use it. */
	ఒకవేళ (res >= 0) {
		విడుదల(sparse_filename);
		clear_నమూనా_list(&pl);
		తిరిగి update_working_directory(repo, ఖాళీ);
	}

	ఒకవేళ (repo_get_oid(repo, "HEAD", &oid)) {
		FILE *fp;

		/* assume we are in a fresh repo, but update the sparse-checkout file */
		ఒకవేళ (సురక్షిత_create_leading_directories(repo, sparse_filename))
			die(_("unable to create leading directories of %s"),
			    sparse_filename);
		fp = xfopen(sparse_filename, "w");
		ఒకవేళ (!fp)
			die(_("failed to open '%s'"), sparse_filename);

		విడుదల(sparse_filename);
		fprintf(fp, "/*\n!/*/\n");
		fclose(fp);
		తిరిగి 0;
	}

	విడుదల(sparse_filename);

	కూడు_నమూనా("/*", ఖాళీలు_పునాది, 0, &pl, 0);
	కూడు_నమూనా("!/*/", ఖాళీలు_పునాది, 0, &pl, 0);
	pl.use_cone_patterns = init_opts.cone_బహుళకం;

	తిరిగి write_patterns_and_update(repo, &pl);
}

స్థిర శూన్య చొప్పించు_recursive_నమూనా(నిర్మాణం నమూనా_list *pl, నిర్మాణం strbuf *మార్గం)
{
	నిర్మాణం నమూనా_నమోదు *e = xmalloc(పరిమాణం(*e));
	e->patternlen = మార్గం->len;
	e->నమూనా = strbuf_detach(మార్గం, ఖాళీ);
	hashmap_నమోదు_init(&e->ent, fspathhash(e->నమూనా));

	hashmap_కూడు(&pl->recursive_hashmap, &e->ent);

	అయితే (e->patternlen) {
		అక్షరం *slash = strrchr(e->నమూనా, '/');
		అక్షరం *oldpattern = e->నమూనా;
		కొలత_t newlen;
		నిర్మాణం నమూనా_నమోదు *dup;

		ఒకవేళ (!slash || slash == e->నమూనా)
			ఆపు;

		newlen = slash - e->నమూనా;
		e = xmalloc(పరిమాణం(నిర్మాణం నమూనా_నమోదు));
		e->patternlen = newlen;
		e->నమూనా = xstrndup(oldpattern, newlen);
		hashmap_నమోదు_init(&e->ent, fspathhash(e->నమూనా));

		dup = hashmap_get_నమోదు(&pl->మాతృ_hashmap, e, ent, ఖాళీ);
		ఒకవేళ (!dup) {
			hashmap_కూడు(&pl->మాతృ_hashmap, &e->ent);
		} లేకపోతే {
			విడుదల(e->నమూనా);
			విడుదల(e);
			e = dup;
		}
	}
}

స్థిర శూన్య strbuf_గమ్యం_cone_నమూనా(నిర్మాణం strbuf *పంక్తి, నిర్మాణం నమూనా_list *pl)
{
	strbuf_trim(పంక్తి);

	strbuf_trim_trailing_dir_sep(పంక్తి);

	ఒకవేళ (strbuf_normalize_మార్గం(పంక్తి))
		die(_("could not normalize path %s"), పంక్తి->buf);

	ఒకవేళ (!పంక్తి->len)
		తిరిగి;

	ఒకవేళ (పంక్తి->buf[0] != '/')
		strbuf_insertstr(పంక్తి, 0, "/");

	చొప్పించు_recursive_నమూనా(pl, పంక్తి);
}

స్థిర శూన్య కూడు_patterns_from_ఆదానం(నిర్మాణం నమూనా_list *pl,
				    పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv,
				    FILE *file)
{
	పూర్ణాంకం i;
	నిర్మాణం repo_config_విలువలు *cfg = repo_config_విలువలు(the_repository);

	ఒకవేళ (cfg->core_sparse_checkout_cone) {
		నిర్మాణం strbuf పంక్తి = STRBUF_INIT;

		hashmap_init(&pl->recursive_hashmap, pl_hashmap_cmp, ఖాళీ, 0);
		hashmap_init(&pl->మాతృ_hashmap, pl_hashmap_cmp, ఖాళీ, 0);
		pl->use_cone_patterns = 1;

		ఒకవేళ (file) {
			నిర్మాణం strbuf unquoted = STRBUF_INIT;
			అయితే (!strbuf_getline(&పంక్తి, file)) {
				ఒకవేళ (పంక్తి.buf[0] == '"') {
					strbuf_reset(&unquoted);
					ఒకవేళ (unquote_c_style(&unquoted, పంక్తి.buf, ఖాళీ))
						die(_("unable to unquote C-style string '%s'"),
						పంక్తి.buf);

					strbuf_మార్పిడి(&unquoted, &పంక్తి);
				}

				strbuf_గమ్యం_cone_నమూనా(&పంక్తి, pl);
			}

			strbuf_release(&unquoted);
		} లేకపోతే {
			కొరకు (i = 0; i < argc; i++) {
				strbuf_setlen(&పంక్తి, 0);
				strbuf_addstr(&పంక్తి, argv[i]);
				strbuf_గమ్యం_cone_నమూనా(&పంక్తి, pl);
			}
		}
		strbuf_release(&పంక్తి);
	} లేకపోతే {
		ఒకవేళ (file) {
			నిర్మాణం strbuf పంక్తి = STRBUF_INIT;

			అయితే (!strbuf_getline(&పంక్తి, file))
				కూడు_నమూనా(పంక్తి.buf, ఖాళీలు_పునాది, 0, pl, 0);

			strbuf_release(&పంక్తి);
		} లేకపోతే {
			కొరకు (i = 0; i < argc; i++)
				కూడు_నమూనా(argv[i], ఖాళీలు_పునాది, 0, pl, 0);
		}
	}
}

గణన modify_type {
	REPLACE,
	కూడు_2,
};

స్థిర శూన్య కూడు_patterns_cone_బహుళకం(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv,
				   నిర్మాణం నమూనా_list *pl,
				   పూర్ణాంకం use_stdin)
{
	నిర్మాణం strbuf బఫరు = STRBUF_INIT;
	నిర్మాణం నమూనా_నమోదు *pe;
	నిర్మాణం hashmap_iter iter;
	నిర్మాణం నమూనా_list existing;
	నిర్మాణం repo_config_విలువలు *cfg = repo_config_విలువలు(the_repository);
	అక్షరం *sparse_filename = get_sparse_checkout_filename();

	కూడు_patterns_from_ఆదానం(pl, argc, argv,
				use_stdin ? stdin : ఖాళీ);

	memset(&existing, 0, పరిమాణం(existing));
	existing.use_cone_patterns = cfg->core_sparse_checkout_cone;

	ఒకవేళ (కూడు_patterns_from_file_గమ్యం_list(sparse_filename, "", 0,
					   &existing, ఖాళీ, 0))
		die(_("unable to load existing sparse-checkout patterns"));
	విడుదల(sparse_filename);

	ఒకవేళ (!existing.use_cone_patterns)
		die(_("existing sparse-checkout patterns do not use cone mode"));

	hashmap_for_each_నమోదు(&existing.recursive_hashmap, &iter, pe, ent) {
		ఒకవేళ (!hashmap_కలిగియుంది_మాతృ(&pl->recursive_hashmap,
					pe->నమూనా, &బఫరు) ||
		    !hashmap_కలిగియుంది_మాతృ(&pl->మాతృ_hashmap,
					pe->నమూనా, &బఫరు)) {
			strbuf_reset(&బఫరు);
			strbuf_addstr(&బఫరు, pe->నమూనా);
			చొప్పించు_recursive_నమూనా(pl, &బఫరు);
		}
	}

	clear_నమూనా_list(&existing);
	strbuf_release(&బఫరు);
}

స్థిర శూన్య కూడు_patterns_literal(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv,
				 నిర్మాణం నమూనా_list *pl,
				 పూర్ణాంకం use_stdin)
{
	అక్షరం *sparse_filename = get_sparse_checkout_filename();
	ఒకవేళ (కూడు_patterns_from_file_గమ్యం_list(sparse_filename, "", 0,
					   pl, ఖాళీ, 0))
		die(_("unable to load existing sparse-checkout patterns"));
	విడుదల(sparse_filename);
	కూడు_patterns_from_ఆదానం(pl, argc, argv, use_stdin ? stdin : ఖాళీ);
}

స్థిర పూర్ణాంకం modify_నమూనా_list(నిర్మాణం repository *repo,
			       నిర్మాణం strvec *వాదనలు, పూర్ణాంకం use_stdin,
			       గణన modify_type m)
{
	పూర్ణాంకం ఫలితం;
	పూర్ణాంకం మారింది_config = 0;
	నిర్మాణం నమూనా_list *pl = xcalloc(1, పరిమాణం(*pl));
	నిర్మాణం repo_config_విలువలు *cfg = repo_config_విలువలు(the_repository);

	ఎంపిక (m) {
	స్థితి కూడు_2:
		ఒకవేళ (cfg->core_sparse_checkout_cone)
			కూడు_patterns_cone_బహుళకం(వాదనలు->nr, వాదనలు->v, pl, use_stdin);
		లేకపోతే
			కూడు_patterns_literal(వాదనలు->nr, వాదనలు->v, pl, use_stdin);
		ఆపు;

	స్థితి REPLACE:
		కూడు_patterns_from_ఆదానం(pl, వాదనలు->nr, వాదనలు->v,
					use_stdin ? stdin : ఖాళీ);
		ఆపు;
	}

	ఒకవేళ (!cfg->apply_sparse_checkout) {
		సమితి_config(repo, బహుళకం_ALL_PATTERNS);
		cfg->apply_sparse_checkout = 1;
		మారింది_config = 1;
	}

	ఫలితం = write_patterns_and_update(repo, pl);

	ఒకవేళ (ఫలితం && మారింది_config)
		సమితి_config(repo, బహుళకం_NO_PATTERNS);

	clear_నమూనా_list(pl);
	విడుదల(pl);
	తిరిగి ఫలితం;
}

స్థిర శూన్య sanitize_paths(నిర్మాణం repository *repo,
			   నిర్మాణం strvec *వాదనలు,
			   స్థిరరాశి అక్షరం *ఉపసర్గ, పూర్ణాంకం skip_checks)
{
	పూర్ణాంకం i;
	నిర్మాణం repo_config_విలువలు *cfg = repo_config_విలువలు(the_repository);

	ఒకవేళ (!వాదనలు->nr)
		తిరిగి;

	ఒకవేళ (ఉపసర్గ && *ఉపసర్గ && cfg->core_sparse_checkout_cone) {
		/*
		 * The args are not pathspecs, so unfortunately we
		 * cannot imitate how cmd_add() uses parse_pathspec().
		 */
		పూర్ణాంకం ఉపసర్గ_len = అక్షరశ్రేణి_పొడవు(ఉపసర్గ);

		కొరకు (i = 0; i < వాదనలు->nr; i++) {
			అక్షరం *prefixed_మార్గం = ఉపసర్గ_మార్గం(the_repository, ఉపసర్గ,
							  ఉపసర్గ_len, వాదనలు->v[i]);
			strvec_replace(వాదనలు, i, prefixed_మార్గం);
			విడుదల(prefixed_మార్గం);
		}
	}

	ఒకవేళ (skip_checks)
		తిరిగి;

	ఒకవేళ (ఉపసర్గ && *ఉపసర్గ && !cfg->core_sparse_checkout_cone)
		die(_("please run from the toplevel directory in non-cone mode"));

	ఒకవేళ (cfg->core_sparse_checkout_cone) {
		కొరకు (i = 0; i < వాదనలు->nr; i++) {
			ఒకవేళ (వాదనలు->v[i][0] == '/')
				die(_("specify directories rather than patterns (no leading slash)"));
			ఒకవేళ (వాదనలు->v[i][0] == '!')
				die(_("specify directories rather than patterns.  If your directory starts with a '!', pass --skip-checks"));
			ఒకవేళ (strpbrk(వాదనలు->v[i], "*?[]"))
				die(_("specify directories rather than patterns.  If your directory really has any of '*?[]\\' in it, pass --skip-checks"));
		}
	}

	కొరకు (i = 0; i < వాదనలు->nr; i++) {
		నిర్మాణం cache_నమోదు *ce;
		నిర్మాణం సూచీ_state *సూచీ = repo->సూచీ;
		పూర్ణాంకం pos = సూచీ_name_pos(సూచీ, వాదనలు->v[i], అక్షరశ్రేణి_పొడవు(వాదనలు->v[i]));

		ఒకవేళ (pos < 0)
			కొనసాగు;
		ce = సూచీ->cache[pos];
		ఒకవేళ (S_ISSPARSEDIR(ce->ce_బహుళకం))
			కొనసాగు;

		ఒకవేళ (cfg->core_sparse_checkout_cone)
			die(_("'%s' is not a directory; to treat it as a directory anyway, rerun with --skip-checks"), వాదనలు->v[i]);
		లేకపోతే
			warning(_("pass a leading slash before paths such as '%s' if you want a single file (see NON-CONE PROBLEMS in the git-sparse-checkout manual)."), వాదనలు->v[i]);
	}
}

స్థిర అక్షరం స్థిరరాశి * స్థిరరాశి builtin_sparse_checkout_కూడు_usage[] = {
	N_("git sparse-checkout add [--skip-checks] (--stdin | <patterns>)"),
	ఖాళీ
};

స్థిర నిర్మాణం sparse_checkout_కూడు_opts {
	పూర్ణాంకం skip_checks;
	పూర్ణాంకం use_stdin;
} కూడు_opts;

స్థిర పూర్ణాంకం sparse_checkout_కూడు(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
			       నిర్మాణం repository *repo)
{
	స్థిర నిర్మాణం option builtin_sparse_checkout_కూడు_options[] = {
		OPT_BOOL_F(0, "skip-checks", &కూడు_opts.skip_checks,
			   N_("skip some sanity checks on the given paths that might give false positives"),
			   PARSE_OPT_NONEG),
		OPT_BOOL(0, "stdin", &కూడు_opts.use_stdin,
			 N_("read patterns from standard in")),
		OPT_చివర(),
	};
	నిర్మాణం strvec patterns = STRVEC_INIT;
	పూర్ణాంకం ret;
	నిర్మాణం repo_config_విలువలు *cfg = repo_config_విలువలు(the_repository);

	అమరిక_work_వృక్షం(the_repository);
	ఒకవేళ (!cfg->apply_sparse_checkout)
		die(_("no sparse-checkout to add to"));

	repo_read_సూచీ(repo);

	argc = parse_options(argc, argv, ఉపసర్గ,
			     builtin_sparse_checkout_కూడు_options,
			     builtin_sparse_checkout_కూడు_usage, 0);

	కొరకు (పూర్ణాంకం i = 0; i < argc; i++)
		strvec_నెట్టు(&patterns, argv[i]);
	sanitize_paths(repo, &patterns, ఉపసర్గ, కూడు_opts.skip_checks);

	ret = modify_నమూనా_list(repo, &patterns, కూడు_opts.use_stdin, కూడు_2);

	strvec_clear(&patterns);
	తిరిగి ret;
}

స్థిర అక్షరం స్థిరరాశి * స్థిరరాశి builtin_sparse_checkout_సమితి_usage[] = {
	N_("git sparse-checkout set [--[no-]cone] [--[no-]sparse-index] [--skip-checks] (--stdin | <patterns>)"),
	ఖాళీ
};

స్థిర నిర్మాణం sparse_checkout_సమితి_opts {
	పూర్ణాంకం cone_బహుళకం;
	పూర్ణాంకం sparse_సూచీ;
	పూర్ణాంకం skip_checks;
	పూర్ణాంకం use_stdin;
} సమితి_opts;

స్థిర పూర్ణాంకం sparse_checkout_సమితి(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
			       నిర్మాణం repository *repo)
{
	నిర్మాణం repo_config_విలువలు *cfg = repo_config_విలువలు(the_repository);
	పూర్ణాంకం default_patterns_nr = 2;
	స్థిరరాశి అక్షరం *default_patterns[] = {"/*", "!/*/", ఖాళీ};

	స్థిర నిర్మాణం option builtin_sparse_checkout_సమితి_options[] = {
		OPT_BOOL(0, "cone", &సమితి_opts.cone_బహుళకం,
			 N_("initialize the sparse-checkout in cone mode")),
		OPT_BOOL(0, "sparse-index", &సమితి_opts.sparse_సూచీ,
			 N_("toggle the use of a sparse index")),
		OPT_BOOL_F(0, "skip-checks", &సమితి_opts.skip_checks,
			   N_("skip some sanity checks on the given paths that might give false positives"),
			   PARSE_OPT_NONEG),
		OPT_BOOL_F(0, "stdin", &సమితి_opts.use_stdin,
			   N_("read patterns from standard in"),
			   PARSE_OPT_NONEG),
		OPT_చివర(),
	};
	నిర్మాణం strvec patterns = STRVEC_INIT;
	పూర్ణాంకం ret;

	అమరిక_work_వృక్షం(the_repository);
	repo_read_సూచీ(repo);

	సమితి_opts.cone_బహుళకం = -1;
	సమితి_opts.sparse_సూచీ = -1;

	argc = parse_options(argc, argv, ఉపసర్గ,
			     builtin_sparse_checkout_సమితి_options,
			     builtin_sparse_checkout_సమితి_usage, 0);

	ఒకవేళ (update_modes(repo, &సమితి_opts.cone_బహుళకం, &సమితి_opts.sparse_సూచీ))
		తిరిగి 1;

	/*
	 * Cone mode automatically specifies the toplevel directory.  For
	 * non-cone mode, if nothing is specified, manually select just the
	 * top-level directory (much as 'init' would do).
	 */
	ఒకవేళ (!cfg->core_sparse_checkout_cone && !సమితి_opts.use_stdin && argc == 0) {
		కొరకు (పూర్ణాంకం i = 0; i < default_patterns_nr; i++)
			strvec_నెట్టు(&patterns, default_patterns[i]);
	} లేకపోతే {
		కొరకు (పూర్ణాంకం i = 0; i < argc; i++)
			strvec_నెట్టు(&patterns, argv[i]);
		sanitize_paths(repo, &patterns, ఉపసర్గ, సమితి_opts.skip_checks);
	}

	ret = modify_నమూనా_list(repo, &patterns, సమితి_opts.use_stdin, REPLACE);

	strvec_clear(&patterns);
	తిరిగి ret;
}

స్థిర అక్షరం స్థిరరాశి * స్థిరరాశి builtin_sparse_checkout_reapply_usage[] = {
	"git sparse-checkout reapply [--[no-]cone] [--[no-]sparse-index]",
	ఖాళీ
};

స్థిర నిర్మాణం sparse_checkout_reapply_opts {
	పూర్ణాంకం cone_బహుళకం;
	పూర్ణాంకం sparse_సూచీ;
} reapply_opts;

స్థిర పూర్ణాంకం sparse_checkout_reapply(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv,
				   స్థిరరాశి అక్షరం *ఉపసర్గ,
				   నిర్మాణం repository *repo)
{
	స్థిర నిర్మాణం option builtin_sparse_checkout_reapply_options[] = {
		OPT_BOOL(0, "cone", &reapply_opts.cone_బహుళకం,
			 N_("initialize the sparse-checkout in cone mode")),
		OPT_BOOL(0, "sparse-index", &reapply_opts.sparse_సూచీ,
			 N_("toggle the use of a sparse index")),
		OPT_చివర(),
	};
	నిర్మాణం repo_config_విలువలు *cfg = repo_config_విలువలు(the_repository);

	అమరిక_work_వృక్షం(the_repository);
	ఒకవేళ (!cfg->apply_sparse_checkout)
		die(_("must be in a sparse-checkout to reapply sparsity patterns"));

	reapply_opts.cone_బహుళకం = -1;
	reapply_opts.sparse_సూచీ = -1;

	argc = parse_options(argc, argv, ఉపసర్గ,
			     builtin_sparse_checkout_reapply_options,
			     builtin_sparse_checkout_reapply_usage, 0);

	repo_read_సూచీ(repo);

	ఒకవేళ (update_modes(repo, &reapply_opts.cone_బహుళకం, &reapply_opts.sparse_సూచీ))
		తిరిగి 1;

	తిరిగి update_working_directory(repo, ఖాళీ);
}

స్థిర అక్షరం స్థిరరాశి * స్థిరరాశి builtin_sparse_checkout_clean_usage[] = {
	"git sparse-checkout clean [-n|--dry-run]",
	ఖాళీ
};

స్థిర పూర్ణాంకం list_file_iterator(స్థిరరాశి అక్షరం *మార్గం, స్థిరరాశి శూన్య *దత్తాంశం)
{
	స్థిరరాశి అక్షరం *సందేశం = దత్తాంశం;

	రాయి(సందేశం, మార్గం);
	తిరిగి 0;
}

స్థిర శూన్య list_every_file_లోపల_dir(స్థిరరాశి అక్షరం *సందేశం,
				   స్థిరరాశి అక్షరం *directory)
{
	నిర్మాణం strbuf మార్గం = STRBUF_INIT;

	strbuf_addstr(&మార్గం, directory);
	for_each_file_లోపల_dir(&మార్గం, list_file_iterator, సందేశం);
	strbuf_release(&మార్గం);
}

స్థిర స్థిరరాశి అక్షరం *సందేశం_remove = N_("Removing %s\n");
స్థిర స్థిరరాశి అక్షరం *సందేశం_would_remove = N_("Would remove %s\n");

స్థిర పూర్ణాంకం sparse_checkout_clean(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv,
				   స్థిరరాశి అక్షరం *ఉపసర్గ,
				   నిర్మాణం repository *repo)
{
	నిర్మాణం strbuf నిండిన_మార్గం = STRBUF_INIT;
	స్థిరరాశి అక్షరం *సందేశం = సందేశం_remove;
	కొలత_t worktree_len;
	పూర్ణాంకం force = 0, dry_నిరంతరం = 0, verbose = 0;
	పూర్ణాంకం require_force = 1;
	నిర్మాణం repo_config_విలువలు *cfg = repo_config_విలువలు(the_repository);

	నిర్మాణం option builtin_sparse_checkout_clean_options[] = {
		OPT_DRY_నిరంతరం(&dry_నిరంతరం, N_("dry run")),
		OPT__FORCE(&force, N_("force"), PARSE_OPT_NOCOMPLETE),
		OPT__VERBOSE(&verbose, N_("report each affected file, not just directories")),
		OPT_చివర(),
	};

	అమరిక_work_వృక్షం(the_repository);
	ఒకవేళ (!cfg->apply_sparse_checkout)
		die(_("must be in a sparse-checkout to clean directories"));
	ఒకవేళ (!cfg->core_sparse_checkout_cone)
		die(_("must be in a cone-mode sparse-checkout to clean directories"));

	argc = parse_options(argc, argv, ఉపసర్గ,
			     builtin_sparse_checkout_clean_options,
			     builtin_sparse_checkout_clean_usage, 0);

	repo_config_get_bool(repo, "clean.requireforce", &require_force);
	ఒకవేళ (require_force && !force && !dry_నిరంతరం)
		die(_("for safety, refusing to clean without one of --force or --dry-run"));

	ఒకవేళ (dry_నిరంతరం)
		సందేశం = సందేశం_would_remove;

	ఒకవేళ (repo_read_సూచీ(repo) < 0)
		die(_("failed to read index"));

	ఒకవేళ (convert_గమ్యం_sparse(repo->సూచీ, SPARSE_సూచీ_జ్ఞాపకం_ONLY) ||
	    repo->సూచీ->sparse_సూచీ == సూచీ_EXPANDED)
		die(_("failed to convert index to a sparse index; resolve merge conflicts and try again"));

	strbuf_addstr(&నిండిన_మార్గం, repo->worktree);
	strbuf_addch(&నిండిన_మార్గం, '/');
	worktree_len = నిండిన_మార్గం.len;

	కొరకు (కొలత_t i = 0; i < repo->సూచీ->cache_nr; i++) {
		నిర్మాణం cache_నమోదు *ce = repo->సూచీ->cache[i];
		ఒకవేళ (!S_ISSPARSEDIR(ce->ce_బహుళకం))
			కొనసాగు;
		strbuf_setlen(&నిండిన_మార్గం, worktree_len);
		strbuf_కూడు(&నిండిన_మార్గం, ce->name, ce->ce_namelen);

		ఒకవేళ (!is_directory(నిండిన_మార్గం.buf))
			కొనసాగు;

		ఒకవేళ (verbose)
			list_every_file_లోపల_dir(సందేశం, ce->name);
		లేకపోతే
			రాయి(సందేశం, ce->name);

		ఒకవేళ (dry_నిరంతరం <= 0 &&
		    remove_dir_recursively(&నిండిన_మార్గం, 0))
			warning_errno(_("failed to remove '%s'"), ce->name);
	}

	strbuf_release(&నిండిన_మార్గం);
	తిరిగి 0;
}

స్థిర అక్షరం స్థిరరాశి * స్థిరరాశి builtin_sparse_checkout_disable_usage[] = {
	"git sparse-checkout disable",
	ఖాళీ
};

స్థిర పూర్ణాంకం sparse_checkout_disable(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv,
				   స్థిరరాశి అక్షరం *ఉపసర్గ,
				   నిర్మాణం repository *repo)
{
	స్థిర నిర్మాణం option builtin_sparse_checkout_disable_options[] = {
		OPT_చివర(),
	};
	నిర్మాణం నమూనా_list pl;
	నిర్మాణం repo_config_విలువలు *cfg = repo_config_విలువలు(the_repository);

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

	అమరిక_work_వృక్షం(the_repository);
	argc = parse_options(argc, argv, ఉపసర్గ,
			     builtin_sparse_checkout_disable_options,
			     builtin_sparse_checkout_disable_usage, 0);

	/*
	 * Disable the advice message for expanding a sparse index, as we
	 * are expecting to do that when disabling sparse-checkout.
	 */
	give_advice_on_expansion = 0;
	repo_read_సూచీ(repo);

	memset(&pl, 0, పరిమాణం(pl));
	hashmap_init(&pl.recursive_hashmap, pl_hashmap_cmp, ఖాళీ, 0);
	hashmap_init(&pl.మాతృ_hashmap, pl_hashmap_cmp, ఖాళీ, 0);
	pl.use_cone_patterns = 0;
	cfg->apply_sparse_checkout = 1;

	కూడు_నమూనా("/*", ఖాళీలు_పునాది, 0, &pl, 0);

	prepare_repo_settings(the_repository);
	repo->settings.sparse_సూచీ = 0;

	ఒకవేళ (update_working_directory(repo, &pl))
		die(_("error while refreshing working directory"));

	clear_నమూనా_list(&pl);
	తిరిగి సమితి_config(repo, బహుళకం_NO_PATTERNS);
}

స్థిర అక్షరం స్థిరరాశి * స్థిరరాశి builtin_sparse_checkout_check_నియమాలు_usage[] = {
	N_("git sparse-checkout check-rules [-z] [--skip-checks]"
	   "[--[no-]cone] [--rules-file <file>]"),
	ఖాళీ
};

స్థిర నిర్మాణం sparse_checkout_check_నియమాలు_opts {
	పూర్ణాంకం cone_బహుళకం;
	పూర్ణాంకం null_termination;
	అక్షరం *నియమాలు_file;
} check_నియమాలు_opts;

స్థిర పూర్ణాంకం check_నియమాలు(నిర్మాణం repository *repo,
		       నిర్మాణం నమూనా_list *pl,
		       పూర్ణాంకం null_terminated)
{
	నిర్మాణం strbuf పంక్తి = STRBUF_INIT;
	నిర్మాణం strbuf unquoted = STRBUF_INIT;
	అక్షరం *మార్గం;
	పూర్ణాంకం పంక్తి_terminator = null_terminated ? 0 : '\n';
	strbuf_getline_fn getline_fn = null_terminated ? strbuf_getline_nul
		: strbuf_getline;
	repo->సూచీ->sparse_checkout_patterns = pl;
	అయితే (!getline_fn(&పంక్తి, stdin)) {
		మార్గం = పంక్తి.buf;
		ఒకవేళ (!null_terminated && పంక్తి.buf[0] == '"') {
			strbuf_reset(&unquoted);
			ఒకవేళ (unquote_c_style(&unquoted, పంక్తి.buf, ఖాళీ))
				die(_("unable to unquote C-style string '%s'"),
					పంక్తి.buf);

			మార్గం = unquoted.buf;
		}

		ఒకవేళ (మార్గం_లోపల_sparse_checkout(మార్గం, repo->సూచీ))
			write_name_quoted(మార్గం, stdout, పంక్తి_terminator);
	}
	strbuf_release(&పంక్తి);
	strbuf_release(&unquoted);

	తిరిగి 0;
}

స్థిర పూర్ణాంకం sparse_checkout_check_నియమాలు(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
				       నిర్మాణం repository *repo)
{
	స్థిర నిర్మాణం option builtin_sparse_checkout_check_నియమాలు_options[] = {
		OPT_BOOL('z', ఖాళీ, &check_నియమాలు_opts.null_termination,
			 N_("terminate input and output files by a NUL character")),
		OPT_BOOL(0, "cone", &check_నియమాలు_opts.cone_బహుళకం,
			 N_("when used with --rules-file interpret patterns as cone mode patterns")),
		OPT_FILENAME(0, "rules-file", &check_నియమాలు_opts.నియమాలు_file,
			 N_("use patterns in <file> instead of the current ones.")),
		OPT_చివర(),
	};

	FILE *fp;
	పూర్ణాంకం ret;
	నిర్మాణం నమూనా_list pl = {0};
	నిర్మాణం repo_config_విలువలు *cfg = repo_config_విలువలు(the_repository);
	అక్షరం *sparse_filename;
	check_నియమాలు_opts.cone_బహుళకం = -1;

	argc = parse_options(argc, argv, ఉపసర్గ,
			     builtin_sparse_checkout_check_నియమాలు_options,
			     builtin_sparse_checkout_check_నియమాలు_usage, 0);

	ఒకవేళ (check_నియమాలు_opts.నియమాలు_file && check_నియమాలు_opts.cone_బహుళకం < 0)
		check_నియమాలు_opts.cone_బహుళకం = 1;

	update_cone_బహుళకం(&check_నియమాలు_opts.cone_బహుళకం);
	pl.use_cone_patterns = cfg->core_sparse_checkout_cone;
	ఒకవేళ (check_నియమాలు_opts.నియమాలు_file) {
		fp = xfopen(check_నియమాలు_opts.నియమాలు_file, "r");
		కూడు_patterns_from_ఆదానం(&pl, argc, argv, fp);
		fclose(fp);
	} లేకపోతే {
		sparse_filename = get_sparse_checkout_filename();
		ఒకవేళ (కూడు_patterns_from_file_గమ్యం_list(sparse_filename, "", 0, &pl,
						   ఖాళీ, 0))
			die(_("unable to load existing sparse-checkout patterns"));
		విడుదల(sparse_filename);
	}

	ret = check_నియమాలు(repo, &pl, check_నియమాలు_opts.null_termination);
	clear_నమూనా_list(&pl);
	విడుదల(check_నియమాలు_opts.నియమాలు_file);
	తిరిగి ret;
}

పూర్ణాంకం cmd_sparse_checkout(పూర్ణాంకం argc,
			స్థిరరాశి అక్షరం **argv,
			స్థిరరాశి అక్షరం *ఉపసర్గ,
			నిర్మాణం repository *repo)
{
	parse_opt_subcommand_fn *fn = ఖాళీ;
	నిర్మాణం option builtin_sparse_checkout_options[] = {
		OPT_SUBCOMMAND("list", &fn, sparse_checkout_list),
		OPT_SUBCOMMAND("init", &fn, sparse_checkout_init),
		OPT_SUBCOMMAND("set", &fn, sparse_checkout_సమితి),
		OPT_SUBCOMMAND("add", &fn, sparse_checkout_కూడు),
		OPT_SUBCOMMAND("reapply", &fn, sparse_checkout_reapply),
		OPT_SUBCOMMAND("clean", &fn, sparse_checkout_clean),
		OPT_SUBCOMMAND("disable", &fn, sparse_checkout_disable),
		OPT_SUBCOMMAND("check-rules", &fn, sparse_checkout_check_నియమాలు),
		OPT_చివర(),
	};

	argc = parse_options(argc, argv, ఉపసర్గ,
			     builtin_sparse_checkout_options,
			     builtin_sparse_checkout_usage, 0);

	repo_config(the_repository, git_default_config, ఖాళీ);

	prepare_repo_settings(repo);
	repo->settings.command_requires_నిండిన_సూచీ = 0;

	తిరిగి fn(argc, argv, ఉపసర్గ, repo);
}
