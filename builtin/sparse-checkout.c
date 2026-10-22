#परिभाषय USE_THE_REPOSITORY_VARIABLE
#परिभाषय DISABLE_SIGN_COMPARE_WARNINGS

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "abspath.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "dir.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "object-file.h"
#अन्तर्भाव "object-name.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "path.h"
#अन्तर्भाव "pathspec.h"
#अन्तर्भाव "strbuf.h"
#अन्तर्भाव "string-list.h"
#अन्तर्भाव "lockfile.h"
#अन्तर्भाव "unpack-trees.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "setup.h"
#अन्तर्भाव "sparse-index.h"
#अन्तर्भाव "worktree.h"

स्थिर ध्रुव वर्ण *रिक्तानि_आधारः = "";

स्थिर वर्ण ध्रुव * ध्रुव builtin_sparse_checkout_usage[] = {
	N_("git sparse-checkout (init | list | set | add | reapply | disable | check-rules | clean) [<options>]"),
	रिक्त
};

स्थिर शून्य write_patterns_गन्तव्यम्_file(FILE *fp, संरचना आदर्शः_list *pl)
{
	पूर्णाङ्क i;

	प्रति (i = 0; i < pl->nr; i++) {
		संरचना मार्गः_आदर्शः *p = pl->patterns[i];

		यदि (p->flags & आदर्शः_FLAG_NEGATIVE)
			fprintf(fp, "!");

		fprintf(fp, "%s", p->आदर्शः);

		यदि (p->flags & आदर्शः_FLAG_MUSTBEDIR)
			fprintf(fp, "/");

		fprintf(fp, "\n");
	}
}

स्थिर वर्ण ध्रुव * ध्रुव builtin_sparse_checkout_list_usage[] = {
	"git sparse-checkout list",
	रिक्त
};

स्थिर पूर्णाङ्क sparse_checkout_list(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
				संरचना repository *repo अप्रयुक्तम्)
{
	स्थिर संरचना option builtin_sparse_checkout_list_options[] = {
		OPT_अन्तः(),
	};
	संरचना आदर्शः_list pl;
	वर्ण *sparse_filename;
	पूर्णाङ्क res;
	संरचना repo_config_मानानि *cfg = repo_config_मानानि(the_repository);

	सज्जता_work_वृक्षः(the_repository);
	यदि (!cfg->apply_sparse_checkout)
		die(_("this worktree is not sparse"));

	argc = parse_options(argc, argv, उपसर्गः,
			     builtin_sparse_checkout_list_options,
			     builtin_sparse_checkout_list_usage, 0);

	स्मृतिं_पूरय(&pl, 0, परिमाण(pl));

	pl.use_cone_patterns = cfg->core_sparse_checkout_cone;

	sparse_filename = get_sparse_checkout_filename();
	res = सङ्कलय_patterns_from_file_गन्तव्यम्_list(sparse_filename, "", 0, &pl, रिक्त, 0);
	मुक्त(sparse_filename);

	यदि (res < 0) {
		warning(_("this worktree is not sparse (sparse-checkout file may not exist)"));
		प्रत्यागम 0;
	}

	यदि (pl.use_cone_patterns) {
		पूर्णाङ्क i;
		संरचना आदर्शः_प्रविष्टिः *pe;
		संरचना hashmap_iter iter;
		संरचना string_list sl = STRING_LIST_INIT_DUP;

		hashmap_for_each_प्रविष्टिः(&pl.recursive_hashmap, &iter, pe, ent) {
			/* pe->pattern starts with "/", skip it */
			string_list_append(&sl, pe->आदर्शः + 1);
		}

		string_list_क्रमय_u(&sl, 0);

		प्रति (i = 0; i < sl.nr; i++) {
			quote_c_style(sl.वस्तूनि[i].string, रिक्त, stdout, 0);
			प्रारूप_लिख("\n");
		}

		string_list_clear(&sl, 0);
	} अन्यथा {
		write_patterns_गन्तव्यम्_file(stdout, &pl);
	}

	clear_आदर्शः_list(&pl);

	प्रत्यागम 0;
}

स्थिर शून्य clean_tracked_sparse_directories(संरचना repository *r)
{
	पूर्णाङ्क i, was_पूर्णम् = 0;
	संरचना strbuf मार्गः = STRBUF_INIT;
	आकार_t pathlen;
	संरचना string_list_item *item;
	संरचना string_list sparse_dirs = STRING_LIST_INIT_DUP;

	/*
	 * If we are not using cone mode patterns, then we cannot
	 * delete directories outside of the sparse cone.
	 */
	यदि (!r || !r->सूचकांक || !r->worktree)
		प्रत्यागम;
	यदि (init_sparse_checkout_patterns(r->सूचकांक) ||
	    !r->सूचकांक->sparse_checkout_patterns->use_cone_patterns)
		प्रत्यागम;

	/*
	 * Use the sparse index as a data structure to assist finding
	 * directories that are safe to delete. This conversion to a
	 * sparse index will not delete directories that contain
	 * conflicted entries or submodules.
	 */
	यदि (r->सूचकांक->sparse_सूचकांक == सूचकांक_EXPANDED) {
		/*
		 * If something, such as a merge conflict or other concern,
		 * prevents us from converting to a sparse index, then do
		 * not try deleting files.
		 */
		यदि (convert_गन्तव्यम्_sparse(r->सूचकांक, SPARSE_सूचकांक_स्मृतिः_ONLY))
			प्रत्यागम;
		was_पूर्णम् = 1;
	}

	strbuf_addstr(&मार्गः, r->worktree);
	strbuf_complete(&मार्गः, '/');
	pathlen = मार्गः.len;

	/*
	 * Collect directories that have gone out of scope but also
	 * exist on disk, so there is some work to be done. We need to
	 * store the entries in a list before exploring, since that might
	 * expand the sparse-index again.
	 */
	प्रति (i = 0; i < r->सूचकांक->cache_nr; i++) {
		संरचना cache_प्रविष्टिः *ce = r->सूचकांक->cache[i];

		यदि (S_ISSPARSEDIR(ce->ce_बहुलकम्) &&
		    repo_file_exists(r, ce->name))
			string_list_append(&sparse_dirs, ce->name);
	}

	for_each_string_list_item(item, &sparse_dirs) {
		संरचना dir_struct dir = DIR_INIT;
		संरचना pathspec p = { 0 };
		संरचना strvec s = STRVEC_INIT;

		strbuf_setlen(&मार्गः, pathlen);
		strbuf_addstr(&मार्गः, item->string);

		dir.flags |= DIR_SHOW_IGNORED_TOO;

		सज्जता_standard_excludes(&dir);
		strvec_प्रक्षिप(&s, मार्गः.buf);

		parse_pathspec(&p, PATHSPEC_GLOB, 0, रिक्त, s.v);
		fill_directory(&dir, r->सूचकांक, &p);

		यदि (dir.nr) {
			warning(_("directory '%s' contains untracked files,"
				  " but is not in the sparse-checkout cone"),
				item->string);
		} अन्यथा यदि (remove_dir_recursively(&मार्गः, 0)) {
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
	strbuf_release(&मार्गः);

	यदि (was_पूर्णम्)
		ensure_पूर्णम्_सूचकांक(r->सूचकांक);
}

स्थिर पूर्णाङ्क update_working_directory(संरचना repository *r,
				    संरचना आदर्शः_list *pl)
{
	गणना update_sparsity_परिणाम परिणाम;
	संरचना unpack_trees_options o;
	संरचना तालकम्_file तालकम्_file = तालकम्_INIT;
	संरचना आदर्शः_list *old_pl;

	/* If no branch has been checked out, there are no updates to make. */
	यदि (is_सूचकांक_unborn(r->सूचकांक))
		प्रत्यागम UPDATE_SPARSITY_SUCCESS;

	old_pl = r->सूचकांक->sparse_checkout_patterns;
	r->सूचकांक->sparse_checkout_patterns = pl;

	स्मृतिं_पूरय(&o, 0, परिमाण(o));
	o.verbose_update = isatty(2);
	o.update = 1;
	o.शीर्ष_idx = -1;
	o.src_सूचकांक = r->सूचकांक;
	o.dst_सूचकांक = r->सूचकांक;
	o.skip_sparse_checkout = 0;

	सज्जता_work_वृक्षः(the_repository);

	repo_hold_locked_सूचकांक(r, &तालकम्_file, तालकम्_DIE_ON_ERROR);

	सज्जता_unpack_trees_porcelain(&o, "sparse-checkout");
	परिणाम = update_sparsity(&o, pl);
	clear_unpack_trees_porcelain(&o);

	यदि (परिणाम == UPDATE_SPARSITY_WARNINGS)
		/*
		 * We don't do any special handling of warnings from untracked
		 * files in the way or dirty entries that can't be removed.
		 */
		परिणाम = UPDATE_SPARSITY_SUCCESS;
	यदि (परिणाम == UPDATE_SPARSITY_SUCCESS)
		write_locked_सूचकांक(r->सूचकांक, &तालकम्_file, COMMIT_तालकम्);
	अन्यथा
		rollback_तालकम्_file(&तालकम्_file);

	clean_tracked_sparse_directories(r);

	यदि (r->सूचकांक->sparse_checkout_patterns != pl) {
		clear_आदर्शः_list(r->सूचकांक->sparse_checkout_patterns);
		FREE_AND_NULL(r->सूचकांक->sparse_checkout_patterns);
	}
	r->सूचकांक->sparse_checkout_patterns = old_pl;

	प्रत्यागम परिणाम;
}

स्थिर वर्ण *escaped_आदर्शः(वर्ण *आदर्शः)
{
	वर्ण *p = आदर्शः;
	संरचना strbuf final = STRBUF_INIT;

	यावत् (*p) {
		यदि (is_glob_special(*p))
			strbuf_addch(&final, '\\');

		strbuf_addch(&final, *p);
		p++;
	}

	प्रत्यागम strbuf_detach(&final, रिक्त);
}

स्थिर शून्य write_cone_गन्तव्यम्_file(FILE *fp, संरचना आदर्शः_list *pl)
{
	पूर्णाङ्क i;
	संरचना आदर्शः_प्रविष्टिः *pe;
	संरचना hashmap_iter iter;
	संरचना string_list sl = STRING_LIST_INIT_DUP;
	संरचना strbuf parent_आदर्शः = STRBUF_INIT;

	hashmap_for_each_प्रविष्टिः(&pl->parent_hashmap, &iter, pe, ent) {
		यदि (hashmap_get_प्रविष्टिः(&pl->recursive_hashmap, pe, ent, रिक्त))
			अनुवर्त;

		यदि (!hashmap_अन्तर्भवति_parent(&pl->recursive_hashmap,
					     pe->आदर्शः,
					     &parent_आदर्शः))
			string_list_append(&sl, pe->आदर्शः);
	}

	string_list_क्रमय_u(&sl, 0);

	fprintf(fp, "/*\n!/*/\n");

	प्रति (i = 0; i < sl.nr; i++) {
		वर्ण *आदर्शः = escaped_आदर्शः(sl.वस्तूनि[i].string);

		यदि (वाक्यदैर्घ्यम्(आदर्शः))
			fprintf(fp, "%s/\n!%s/*/\n", आदर्शः, आदर्शः);
		मुक्त(आदर्शः);
	}

	string_list_clear(&sl, 0);

	hashmap_for_each_प्रविष्टिः(&pl->recursive_hashmap, &iter, pe, ent) {
		यदि (!hashmap_अन्तर्भवति_parent(&pl->recursive_hashmap,
					     pe->आदर्शः,
					     &parent_आदर्शः))
			string_list_append(&sl, pe->आदर्शः);
	}

	strbuf_release(&parent_आदर्शः);

	string_list_क्रमय_u(&sl, 0);

	प्रति (i = 0; i < sl.nr; i++) {
		वर्ण *आदर्शः = escaped_आदर्शः(sl.वस्तूनि[i].string);
		fprintf(fp, "%s/\n", आदर्शः);
		मुक्त(आदर्शः);
	}

	string_list_clear(&sl, 0);
}

स्थिर पूर्णाङ्क write_patterns_and_update(संरचना repository *repo,
				     संरचना आदर्शः_list *pl)
{
	वर्ण *sparse_filename;
	FILE *fp;
	संरचना तालकम्_file lk = तालकम्_INIT;
	पूर्णाङ्क परिणाम;
	संरचना repo_config_मानानि *cfg = repo_config_मानानि(the_repository);

	sparse_filename = get_sparse_checkout_filename();

	यदि (safe_create_leading_directories(repo, sparse_filename))
		die(_("failed to create directory for sparse-checkout file"));

	repo_hold_तालकम्_file_for_update(repo, &lk, sparse_filename,
				       तालकम्_DIE_ON_ERROR);

	परिणाम = update_working_directory(repo, pl);
	यदि (परिणाम) {
		rollback_तालकम्_file(&lk);
		update_working_directory(repo, रिक्त);
		गच्छ बहिर्गमनम्;
	}

	fp = fdopen_तालकम्_file(&lk, "w");
	यदि (!fp)
		die_errno(_("unable to fdopen %s"), get_तालकम्_file_मार्गः(&lk));

	यदि (cfg->core_sparse_checkout_cone)
		write_cone_गन्तव्यम्_file(fp, pl);
	अन्यथा
		write_patterns_गन्तव्यम्_file(fp, pl);

	यदि (commit_तालकम्_file(&lk))
		die_errno(_("unable to write %s"), sparse_filename);

बहिर्गमनम्:
	clear_आदर्शः_list(pl);
	मुक्त(sparse_filename);
	प्रत्यागम परिणाम;
}

गणना sparse_checkout_बहुलकम् {
	बहुलकम्_NO_PATTERNS = 0,
	बहुलकम्_ALL_PATTERNS = 1,
	बहुलकम्_CONE_PATTERNS = 2,
};

स्थिर पूर्णाङ्क समुच्चयः_config(संरचना repository *repo,
		      गणना sparse_checkout_बहुलकम् बहुलकम्)
{
	/* Update to use worktree config, if not already. */
	यदि (init_worktree_config(repo)) {
		error(_("failed to initialize worktree config"));
		प्रत्यागम 1;
	}

	यदि (repo_config_समुच्चयः_worktree_gently(repo,
					    "core.sparseCheckout",
					    बहुलकम् ? "true" : "false") ||
	    repo_config_समुच्चयः_worktree_gently(repo,
					    "core.sparseCheckoutCone",
					    बहुलकम् == बहुलकम्_CONE_PATTERNS ?
						"true" : "false"))
		प्रत्यागम 1;

	यदि (बहुलकम् == बहुलकम्_NO_PATTERNS)
		प्रत्यागम समुच्चयः_sparse_सूचकांक_config(repo, 0);

	प्रत्यागम 0;
}

स्थिर गणना sparse_checkout_बहुलकम् update_cone_बहुलकम्(पूर्णाङ्क *cone_बहुलकम्) {
	संरचना repo_config_मानानि *cfg = repo_config_मानानि(the_repository);

	/* If not specified, use previous definition of cone mode */
	यदि (*cone_बहुलकम् == -1 && cfg->apply_sparse_checkout)
		*cone_बहुलकम् = cfg->core_sparse_checkout_cone;

	/* Set cone/non-cone mode appropriately */
	cfg->apply_sparse_checkout = 1;
	यदि (*cone_बहुलकम् == 1 || *cone_बहुलकम् == -1) {
		cfg->core_sparse_checkout_cone = 1;
		प्रत्यागम बहुलकम्_CONE_PATTERNS;
	}
	cfg->core_sparse_checkout_cone = 0;
	प्रत्यागम बहुलकम्_ALL_PATTERNS;
}

स्थिर पूर्णाङ्क update_modes(संरचना repository *repo, पूर्णाङ्क *cone_बहुलकम्, पूर्णाङ्क *sparse_सूचकांक)
{
	पूर्णाङ्क बहुलकम्, लेखः_बहुलकम्;
	संरचना repo_config_मानानि *cfg = repo_config_मानानि(the_repository);

	/* Determine if we need to record the mode; ensure sparse checkout on */
	लेखः_बहुलकम् = (*cone_बहुलकम् != -1) || !cfg->apply_sparse_checkout;

	बहुलकम् = update_cone_बहुलकम्(cone_बहुलकम्);
	यदि (लेखः_बहुलकम् && समुच्चयः_config(repo, बहुलकम्))
		प्रत्यागम 1;

	/* Set sparse-index/non-sparse-index mode if specified */
	यदि (*sparse_सूचकांक >= 0) {
		यदि (समुच्चयः_sparse_सूचकांक_config(repo, *sparse_सूचकांक) < 0)
			die(_("failed to modify sparse-index config"));

		/* force an index rewrite */
		repo_read_सूचकांक(repo);
		repo->सूचकांक->updated_workdir = 1;

		यदि (!*sparse_सूचकांक)
			ensure_पूर्णम्_सूचकांक(repo->सूचकांक);
	}

	प्रत्यागम 0;
}

स्थिर वर्ण ध्रुव * ध्रुव builtin_sparse_checkout_init_usage[] = {
	"git sparse-checkout init [--cone] [--[no-]sparse-index]",
	रिक्त
};

स्थिर संरचना sparse_checkout_init_opts {
	पूर्णाङ्क cone_बहुलकम्;
	पूर्णाङ्क sparse_सूचकांक;
} init_opts;

स्थिर पूर्णाङ्क sparse_checkout_init(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
				संरचना repository *repo)
{
	संरचना आदर्शः_list pl;
	वर्ण *sparse_filename;
	पूर्णाङ्क res;
	संरचना object_id oid;

	स्थिर संरचना option builtin_sparse_checkout_init_options[] = {
		OPT_BOOL(0, "cone", &init_opts.cone_बहुलकम्,
			 N_("initialize the sparse-checkout in cone mode")),
		OPT_BOOL(0, "sparse-index", &init_opts.sparse_सूचकांक,
			 N_("toggle the use of a sparse index")),
		OPT_अन्तः(),
	};

	सज्जता_work_वृक्षः(the_repository);
	repo_read_सूचकांक(repo);

	init_opts.cone_बहुलकम् = -1;
	init_opts.sparse_सूचकांक = -1;

	argc = parse_options(argc, argv, उपसर्गः,
			     builtin_sparse_checkout_init_options,
			     builtin_sparse_checkout_init_usage, 0);

	यदि (update_modes(repo, &init_opts.cone_बहुलकम्, &init_opts.sparse_सूचकांक))
		प्रत्यागम 1;

	स्मृतिं_पूरय(&pl, 0, परिमाण(pl));

	sparse_filename = get_sparse_checkout_filename();
	res = सङ्कलय_patterns_from_file_गन्तव्यम्_list(sparse_filename, "", 0, &pl, रिक्त, 0);

	/* If we already have a sparse-checkout file, use it. */
	यदि (res >= 0) {
		मुक्त(sparse_filename);
		clear_आदर्शः_list(&pl);
		प्रत्यागम update_working_directory(repo, रिक्त);
	}

	यदि (repo_get_oid(repo, "HEAD", &oid)) {
		FILE *fp;

		/* assume we are in a fresh repo, but update the sparse-checkout file */
		यदि (safe_create_leading_directories(repo, sparse_filename))
			die(_("unable to create leading directories of %s"),
			    sparse_filename);
		fp = xfopen(sparse_filename, "w");
		यदि (!fp)
			die(_("failed to open '%s'"), sparse_filename);

		मुक्त(sparse_filename);
		fprintf(fp, "/*\n!/*/\n");
		fclose(fp);
		प्रत्यागम 0;
	}

	मुक्त(sparse_filename);

	सङ्कलय_आदर्शः("/*", रिक्तानि_आधारः, 0, &pl, 0);
	सङ्कलय_आदर्शः("!/*/", रिक्तानि_आधारः, 0, &pl, 0);
	pl.use_cone_patterns = init_opts.cone_बहुलकम्;

	प्रत्यागम write_patterns_and_update(repo, &pl);
}

स्थिर शून्य निवेशय_recursive_आदर्शः(संरचना आदर्शः_list *pl, संरचना strbuf *मार्गः)
{
	संरचना आदर्शः_प्रविष्टिः *e = xmalloc(परिमाण(*e));
	e->patternlen = मार्गः->len;
	e->आदर्शः = strbuf_detach(मार्गः, रिक्त);
	hashmap_प्रविष्टिः_init(&e->ent, fspathhash(e->आदर्शः));

	hashmap_सङ्कलय(&pl->recursive_hashmap, &e->ent);

	यावत् (e->patternlen) {
		वर्ण *slash = strrchr(e->आदर्शः, '/');
		वर्ण *oldpattern = e->आदर्शः;
		आकार_t newlen;
		संरचना आदर्शः_प्रविष्टिः *dup;

		यदि (!slash || slash == e->आदर्शः)
			भङ्ग;

		newlen = slash - e->आदर्शः;
		e = xmalloc(परिमाण(संरचना आदर्शः_प्रविष्टिः));
		e->patternlen = newlen;
		e->आदर्शः = xstrndup(oldpattern, newlen);
		hashmap_प्रविष्टिः_init(&e->ent, fspathhash(e->आदर्शः));

		dup = hashmap_get_प्रविष्टिः(&pl->parent_hashmap, e, ent, रिक्त);
		यदि (!dup) {
			hashmap_सङ्कलय(&pl->parent_hashmap, &e->ent);
		} अन्यथा {
			मुक्त(e->आदर्शः);
			मुक्त(e);
			e = dup;
		}
	}
}

स्थिर शून्य strbuf_गन्तव्यम्_cone_आदर्शः(संरचना strbuf *पङ्क्तिः, संरचना आदर्शः_list *pl)
{
	strbuf_trim(पङ्क्तिः);

	strbuf_trim_trailing_dir_sep(पङ्क्तिः);

	यदि (strbuf_normalize_मार्गः(पङ्क्तिः))
		die(_("could not normalize path %s"), पङ्क्तिः->buf);

	यदि (!पङ्क्तिः->len)
		प्रत्यागम;

	यदि (पङ्क्तिः->buf[0] != '/')
		strbuf_insertstr(पङ्क्तिः, 0, "/");

	निवेशय_recursive_आदर्शः(pl, पङ्क्तिः);
}

स्थिर शून्य सङ्कलय_patterns_from_निवेशः(संरचना आदर्शः_list *pl,
				    पूर्णाङ्क argc, ध्रुव वर्ण **argv,
				    FILE *file)
{
	पूर्णाङ्क i;
	संरचना repo_config_मानानि *cfg = repo_config_मानानि(the_repository);

	यदि (cfg->core_sparse_checkout_cone) {
		संरचना strbuf पङ्क्तिः = STRBUF_INIT;

		hashmap_init(&pl->recursive_hashmap, pl_hashmap_cmp, रिक्त, 0);
		hashmap_init(&pl->parent_hashmap, pl_hashmap_cmp, रिक्त, 0);
		pl->use_cone_patterns = 1;

		यदि (file) {
			संरचना strbuf unquoted = STRBUF_INIT;
			यावत् (!strbuf_getline(&पङ्क्तिः, file)) {
				यदि (पङ्क्तिः.buf[0] == '"') {
					strbuf_reset(&unquoted);
					यदि (unquote_c_style(&unquoted, पङ्क्तिः.buf, रिक्त))
						die(_("unable to unquote C-style string '%s'"),
						पङ्क्तिः.buf);

					strbuf_swap(&unquoted, &पङ्क्तिः);
				}

				strbuf_गन्तव्यम्_cone_आदर्शः(&पङ्क्तिः, pl);
			}

			strbuf_release(&unquoted);
		} अन्यथा {
			प्रति (i = 0; i < argc; i++) {
				strbuf_setlen(&पङ्क्तिः, 0);
				strbuf_addstr(&पङ्क्तिः, argv[i]);
				strbuf_गन्तव्यम्_cone_आदर्शः(&पङ्क्तिः, pl);
			}
		}
		strbuf_release(&पङ्क्तिः);
	} अन्यथा {
		यदि (file) {
			संरचना strbuf पङ्क्तिः = STRBUF_INIT;

			यावत् (!strbuf_getline(&पङ्क्तिः, file))
				सङ्कलय_आदर्शः(पङ्क्तिः.buf, रिक्तानि_आधारः, 0, pl, 0);

			strbuf_release(&पङ्क्तिः);
		} अन्यथा {
			प्रति (i = 0; i < argc; i++)
				सङ्कलय_आदर्शः(argv[i], रिक्तानि_आधारः, 0, pl, 0);
		}
	}
}

गणना modify_type {
	REPLACE,
	सङ्कलय_2,
};

स्थिर शून्य सङ्कलय_patterns_cone_बहुलकम्(पूर्णाङ्क argc, ध्रुव वर्ण **argv,
				   संरचना आदर्शः_list *pl,
				   पूर्णाङ्क use_stdin)
{
	संरचना strbuf बफर = STRBUF_INIT;
	संरचना आदर्शः_प्रविष्टिः *pe;
	संरचना hashmap_iter iter;
	संरचना आदर्शः_list existing;
	संरचना repo_config_मानानि *cfg = repo_config_मानानि(the_repository);
	वर्ण *sparse_filename = get_sparse_checkout_filename();

	सङ्कलय_patterns_from_निवेशः(pl, argc, argv,
				use_stdin ? stdin : रिक्त);

	स्मृतिं_पूरय(&existing, 0, परिमाण(existing));
	existing.use_cone_patterns = cfg->core_sparse_checkout_cone;

	यदि (सङ्कलय_patterns_from_file_गन्तव्यम्_list(sparse_filename, "", 0,
					   &existing, रिक्त, 0))
		die(_("unable to load existing sparse-checkout patterns"));
	मुक्त(sparse_filename);

	यदि (!existing.use_cone_patterns)
		die(_("existing sparse-checkout patterns do not use cone mode"));

	hashmap_for_each_प्रविष्टिः(&existing.recursive_hashmap, &iter, pe, ent) {
		यदि (!hashmap_अन्तर्भवति_parent(&pl->recursive_hashmap,
					pe->आदर्शः, &बफर) ||
		    !hashmap_अन्तर्भवति_parent(&pl->parent_hashmap,
					pe->आदर्शः, &बफर)) {
			strbuf_reset(&बफर);
			strbuf_addstr(&बफर, pe->आदर्शः);
			निवेशय_recursive_आदर्शः(pl, &बफर);
		}
	}

	clear_आदर्शः_list(&existing);
	strbuf_release(&बफर);
}

स्थिर शून्य सङ्कलय_patterns_literal(पूर्णाङ्क argc, ध्रुव वर्ण **argv,
				 संरचना आदर्शः_list *pl,
				 पूर्णाङ्क use_stdin)
{
	वर्ण *sparse_filename = get_sparse_checkout_filename();
	यदि (सङ्कलय_patterns_from_file_गन्तव्यम्_list(sparse_filename, "", 0,
					   pl, रिक्त, 0))
		die(_("unable to load existing sparse-checkout patterns"));
	मुक्त(sparse_filename);
	सङ्कलय_patterns_from_निवेशः(pl, argc, argv, use_stdin ? stdin : रिक्त);
}

स्थिर पूर्णाङ्क modify_आदर्शः_list(संरचना repository *repo,
			       संरचना strvec *तर्काः, पूर्णाङ्क use_stdin,
			       गणना modify_type m)
{
	पूर्णाङ्क परिणाम;
	पूर्णाङ्क परिवर्तितम्_config = 0;
	संरचना आदर्शः_list *pl = xcalloc(1, परिमाण(*pl));
	संरचना repo_config_मानानि *cfg = repo_config_मानानि(the_repository);

	विकल्प (m) {
	पक्ष सङ्कलय_2:
		यदि (cfg->core_sparse_checkout_cone)
			सङ्कलय_patterns_cone_बहुलकम्(तर्काः->nr, तर्काः->v, pl, use_stdin);
		अन्यथा
			सङ्कलय_patterns_literal(तर्काः->nr, तर्काः->v, pl, use_stdin);
		भङ्ग;

	पक्ष REPLACE:
		सङ्कलय_patterns_from_निवेशः(pl, तर्काः->nr, तर्काः->v,
					use_stdin ? stdin : रिक्त);
		भङ्ग;
	}

	यदि (!cfg->apply_sparse_checkout) {
		समुच्चयः_config(repo, बहुलकम्_ALL_PATTERNS);
		cfg->apply_sparse_checkout = 1;
		परिवर्तितम्_config = 1;
	}

	परिणाम = write_patterns_and_update(repo, pl);

	यदि (परिणाम && परिवर्तितम्_config)
		समुच्चयः_config(repo, बहुलकम्_NO_PATTERNS);

	clear_आदर्शः_list(pl);
	मुक्त(pl);
	प्रत्यागम परिणाम;
}

स्थिर शून्य sanitize_paths(संरचना repository *repo,
			   संरचना strvec *तर्काः,
			   ध्रुव वर्ण *उपसर्गः, पूर्णाङ्क skip_checks)
{
	पूर्णाङ्क i;
	संरचना repo_config_मानानि *cfg = repo_config_मानानि(the_repository);

	यदि (!तर्काः->nr)
		प्रत्यागम;

	यदि (उपसर्गः && *उपसर्गः && cfg->core_sparse_checkout_cone) {
		/*
		 * The args are not pathspecs, so unfortunately we
		 * cannot imitate how cmd_add() uses parse_pathspec().
		 */
		पूर्णाङ्क उपसर्गः_len = वाक्यदैर्घ्यम्(उपसर्गः);

		प्रति (i = 0; i < तर्काः->nr; i++) {
			वर्ण *prefixed_मार्गः = उपसर्गः_मार्गः(the_repository, उपसर्गः,
							  उपसर्गः_len, तर्काः->v[i]);
			strvec_replace(तर्काः, i, prefixed_मार्गः);
			मुक्त(prefixed_मार्गः);
		}
	}

	यदि (skip_checks)
		प्रत्यागम;

	यदि (उपसर्गः && *उपसर्गः && !cfg->core_sparse_checkout_cone)
		die(_("please run from the toplevel directory in non-cone mode"));

	यदि (cfg->core_sparse_checkout_cone) {
		प्रति (i = 0; i < तर्काः->nr; i++) {
			यदि (तर्काः->v[i][0] == '/')
				die(_("specify directories rather than patterns (no leading slash)"));
			यदि (तर्काः->v[i][0] == '!')
				die(_("specify directories rather than patterns.  If your directory starts with a '!', pass --skip-checks"));
			यदि (strpbrk(तर्काः->v[i], "*?[]"))
				die(_("specify directories rather than patterns.  If your directory really has any of '*?[]\\' in it, pass --skip-checks"));
		}
	}

	प्रति (i = 0; i < तर्काः->nr; i++) {
		संरचना cache_प्रविष्टिः *ce;
		संरचना सूचकांक_state *सूचकांक = repo->सूचकांक;
		पूर्णाङ्क pos = सूचकांक_name_pos(सूचकांक, तर्काः->v[i], वाक्यदैर्घ्यम्(तर्काः->v[i]));

		यदि (pos < 0)
			अनुवर्त;
		ce = सूचकांक->cache[pos];
		यदि (S_ISSPARSEDIR(ce->ce_बहुलकम्))
			अनुवर्त;

		यदि (cfg->core_sparse_checkout_cone)
			die(_("'%s' is not a directory; to treat it as a directory anyway, rerun with --skip-checks"), तर्काः->v[i]);
		अन्यथा
			warning(_("pass a leading slash before paths such as '%s' if you want a single file (see NON-CONE PROBLEMS in the git-sparse-checkout manual)."), तर्काः->v[i]);
	}
}

स्थिर वर्ण ध्रुव * ध्रुव builtin_sparse_checkout_सङ्कलय_usage[] = {
	N_("git sparse-checkout add [--skip-checks] (--stdin | <patterns>)"),
	रिक्त
};

स्थिर संरचना sparse_checkout_सङ्कलय_opts {
	पूर्णाङ्क skip_checks;
	पूर्णाङ्क use_stdin;
} सङ्कलय_opts;

स्थिर पूर्णाङ्क sparse_checkout_सङ्कलय(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
			       संरचना repository *repo)
{
	स्थिर संरचना option builtin_sparse_checkout_सङ्कलय_options[] = {
		OPT_BOOL_F(0, "skip-checks", &सङ्कलय_opts.skip_checks,
			   N_("skip some sanity checks on the given paths that might give false positives"),
			   PARSE_OPT_NONEG),
		OPT_BOOL(0, "stdin", &सङ्कलय_opts.use_stdin,
			 N_("read patterns from standard in")),
		OPT_अन्तः(),
	};
	संरचना strvec patterns = STRVEC_INIT;
	पूर्णाङ्क ret;
	संरचना repo_config_मानानि *cfg = repo_config_मानानि(the_repository);

	सज्जता_work_वृक्षः(the_repository);
	यदि (!cfg->apply_sparse_checkout)
		die(_("no sparse-checkout to add to"));

	repo_read_सूचकांक(repo);

	argc = parse_options(argc, argv, उपसर्गः,
			     builtin_sparse_checkout_सङ्कलय_options,
			     builtin_sparse_checkout_सङ्कलय_usage, 0);

	प्रति (पूर्णाङ्क i = 0; i < argc; i++)
		strvec_प्रक्षिप(&patterns, argv[i]);
	sanitize_paths(repo, &patterns, उपसर्गः, सङ्कलय_opts.skip_checks);

	ret = modify_आदर्शः_list(repo, &patterns, सङ्कलय_opts.use_stdin, सङ्कलय_2);

	strvec_clear(&patterns);
	प्रत्यागम ret;
}

स्थिर वर्ण ध्रुव * ध्रुव builtin_sparse_checkout_समुच्चयः_usage[] = {
	N_("git sparse-checkout set [--[no-]cone] [--[no-]sparse-index] [--skip-checks] (--stdin | <patterns>)"),
	रिक्त
};

स्थिर संरचना sparse_checkout_समुच्चयः_opts {
	पूर्णाङ्क cone_बहुलकम्;
	पूर्णाङ्क sparse_सूचकांक;
	पूर्णाङ्क skip_checks;
	पूर्णाङ्क use_stdin;
} समुच्चयः_opts;

स्थिर पूर्णाङ्क sparse_checkout_समुच्चयः(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
			       संरचना repository *repo)
{
	संरचना repo_config_मानानि *cfg = repo_config_मानानि(the_repository);
	पूर्णाङ्क default_patterns_nr = 2;
	ध्रुव वर्ण *default_patterns[] = {"/*", "!/*/", रिक्त};

	स्थिर संरचना option builtin_sparse_checkout_समुच्चयः_options[] = {
		OPT_BOOL(0, "cone", &समुच्चयः_opts.cone_बहुलकम्,
			 N_("initialize the sparse-checkout in cone mode")),
		OPT_BOOL(0, "sparse-index", &समुच्चयः_opts.sparse_सूचकांक,
			 N_("toggle the use of a sparse index")),
		OPT_BOOL_F(0, "skip-checks", &समुच्चयः_opts.skip_checks,
			   N_("skip some sanity checks on the given paths that might give false positives"),
			   PARSE_OPT_NONEG),
		OPT_BOOL_F(0, "stdin", &समुच्चयः_opts.use_stdin,
			   N_("read patterns from standard in"),
			   PARSE_OPT_NONEG),
		OPT_अन्तः(),
	};
	संरचना strvec patterns = STRVEC_INIT;
	पूर्णाङ्क ret;

	सज्जता_work_वृक्षः(the_repository);
	repo_read_सूचकांक(repo);

	समुच्चयः_opts.cone_बहुलकम् = -1;
	समुच्चयः_opts.sparse_सूचकांक = -1;

	argc = parse_options(argc, argv, उपसर्गः,
			     builtin_sparse_checkout_समुच्चयः_options,
			     builtin_sparse_checkout_समुच्चयः_usage, 0);

	यदि (update_modes(repo, &समुच्चयः_opts.cone_बहुलकम्, &समुच्चयः_opts.sparse_सूचकांक))
		प्रत्यागम 1;

	/*
	 * Cone mode automatically specifies the toplevel directory.  For
	 * non-cone mode, if nothing is specified, manually select just the
	 * top-level directory (much as 'init' would do).
	 */
	यदि (!cfg->core_sparse_checkout_cone && !समुच्चयः_opts.use_stdin && argc == 0) {
		प्रति (पूर्णाङ्क i = 0; i < default_patterns_nr; i++)
			strvec_प्रक्षिप(&patterns, default_patterns[i]);
	} अन्यथा {
		प्रति (पूर्णाङ्क i = 0; i < argc; i++)
			strvec_प्रक्षिप(&patterns, argv[i]);
		sanitize_paths(repo, &patterns, उपसर्गः, समुच्चयः_opts.skip_checks);
	}

	ret = modify_आदर्शः_list(repo, &patterns, समुच्चयः_opts.use_stdin, REPLACE);

	strvec_clear(&patterns);
	प्रत्यागम ret;
}

स्थिर वर्ण ध्रुव * ध्रुव builtin_sparse_checkout_reapply_usage[] = {
	"git sparse-checkout reapply [--[no-]cone] [--[no-]sparse-index]",
	रिक्त
};

स्थिर संरचना sparse_checkout_reapply_opts {
	पूर्णाङ्क cone_बहुलकम्;
	पूर्णाङ्क sparse_सूचकांक;
} reapply_opts;

स्थिर पूर्णाङ्क sparse_checkout_reapply(पूर्णाङ्क argc, ध्रुव वर्ण **argv,
				   ध्रुव वर्ण *उपसर्गः,
				   संरचना repository *repo)
{
	स्थिर संरचना option builtin_sparse_checkout_reapply_options[] = {
		OPT_BOOL(0, "cone", &reapply_opts.cone_बहुलकम्,
			 N_("initialize the sparse-checkout in cone mode")),
		OPT_BOOL(0, "sparse-index", &reapply_opts.sparse_सूचकांक,
			 N_("toggle the use of a sparse index")),
		OPT_अन्तः(),
	};
	संरचना repo_config_मानानि *cfg = repo_config_मानानि(the_repository);

	सज्जता_work_वृक्षः(the_repository);
	यदि (!cfg->apply_sparse_checkout)
		die(_("must be in a sparse-checkout to reapply sparsity patterns"));

	reapply_opts.cone_बहुलकम् = -1;
	reapply_opts.sparse_सूचकांक = -1;

	argc = parse_options(argc, argv, उपसर्गः,
			     builtin_sparse_checkout_reapply_options,
			     builtin_sparse_checkout_reapply_usage, 0);

	repo_read_सूचकांक(repo);

	यदि (update_modes(repo, &reapply_opts.cone_बहुलकम्, &reapply_opts.sparse_सूचकांक))
		प्रत्यागम 1;

	प्रत्यागम update_working_directory(repo, रिक्त);
}

स्थिर वर्ण ध्रुव * ध्रुव builtin_sparse_checkout_clean_usage[] = {
	"git sparse-checkout clean [-n|--dry-run]",
	रिक्त
};

स्थिर पूर्णाङ्क list_file_iterator(ध्रुव वर्ण *मार्गः, ध्रुव शून्य *दत्त)
{
	ध्रुव वर्ण *सन्देशः = दत्त;

	प्रारूप_लिख(सन्देशः, मार्गः);
	प्रत्यागम 0;
}

स्थिर शून्य list_every_file_अन्तर्_dir(ध्रुव वर्ण *सन्देशः,
				   ध्रुव वर्ण *directory)
{
	संरचना strbuf मार्गः = STRBUF_INIT;

	strbuf_addstr(&मार्गः, directory);
	for_each_file_अन्तर्_dir(&मार्गः, list_file_iterator, सन्देशः);
	strbuf_release(&मार्गः);
}

स्थिर ध्रुव वर्ण *सन्देशः_remove = N_("Removing %s\n");
स्थिर ध्रुव वर्ण *सन्देशः_would_remove = N_("Would remove %s\n");

स्थिर पूर्णाङ्क sparse_checkout_clean(पूर्णाङ्क argc, ध्रुव वर्ण **argv,
				   ध्रुव वर्ण *उपसर्गः,
				   संरचना repository *repo)
{
	संरचना strbuf पूर्णम्_मार्गः = STRBUF_INIT;
	ध्रुव वर्ण *सन्देशः = सन्देशः_remove;
	आकार_t worktree_len;
	पूर्णाङ्क force = 0, dry_सततम् = 0, verbose = 0;
	पूर्णाङ्क require_force = 1;
	संरचना repo_config_मानानि *cfg = repo_config_मानानि(the_repository);

	संरचना option builtin_sparse_checkout_clean_options[] = {
		OPT_DRY_सततम्(&dry_सततम्, N_("dry run")),
		OPT__FORCE(&force, N_("force"), PARSE_OPT_NOCOMPLETE),
		OPT__VERBOSE(&verbose, N_("report each affected file, not just directories")),
		OPT_अन्तः(),
	};

	सज्जता_work_वृक्षः(the_repository);
	यदि (!cfg->apply_sparse_checkout)
		die(_("must be in a sparse-checkout to clean directories"));
	यदि (!cfg->core_sparse_checkout_cone)
		die(_("must be in a cone-mode sparse-checkout to clean directories"));

	argc = parse_options(argc, argv, उपसर्गः,
			     builtin_sparse_checkout_clean_options,
			     builtin_sparse_checkout_clean_usage, 0);

	repo_config_get_bool(repo, "clean.requireforce", &require_force);
	यदि (require_force && !force && !dry_सततम्)
		die(_("for safety, refusing to clean without one of --force or --dry-run"));

	यदि (dry_सततम्)
		सन्देशः = सन्देशः_would_remove;

	यदि (repo_read_सूचकांक(repo) < 0)
		die(_("failed to read index"));

	यदि (convert_गन्तव्यम्_sparse(repo->सूचकांक, SPARSE_सूचकांक_स्मृतिः_ONLY) ||
	    repo->सूचकांक->sparse_सूचकांक == सूचकांक_EXPANDED)
		die(_("failed to convert index to a sparse index; resolve merge conflicts and try again"));

	strbuf_addstr(&पूर्णम्_मार्गः, repo->worktree);
	strbuf_addch(&पूर्णम्_मार्गः, '/');
	worktree_len = पूर्णम्_मार्गः.len;

	प्रति (आकार_t i = 0; i < repo->सूचकांक->cache_nr; i++) {
		संरचना cache_प्रविष्टिः *ce = repo->सूचकांक->cache[i];
		यदि (!S_ISSPARSEDIR(ce->ce_बहुलकम्))
			अनुवर्त;
		strbuf_setlen(&पूर्णम्_मार्गः, worktree_len);
		strbuf_सङ्कलय(&पूर्णम्_मार्गः, ce->name, ce->ce_namelen);

		यदि (!is_directory(पूर्णम्_मार्गः.buf))
			अनुवर्त;

		यदि (verbose)
			list_every_file_अन्तर्_dir(सन्देशः, ce->name);
		अन्यथा
			प्रारूप_लिख(सन्देशः, ce->name);

		यदि (dry_सततम् <= 0 &&
		    remove_dir_recursively(&पूर्णम्_मार्गः, 0))
			warning_errno(_("failed to remove '%s'"), ce->name);
	}

	strbuf_release(&पूर्णम्_मार्गः);
	प्रत्यागम 0;
}

स्थिर वर्ण ध्रुव * ध्रुव builtin_sparse_checkout_disable_usage[] = {
	"git sparse-checkout disable",
	रिक्त
};

स्थिर पूर्णाङ्क sparse_checkout_disable(पूर्णाङ्क argc, ध्रुव वर्ण **argv,
				   ध्रुव वर्ण *उपसर्गः,
				   संरचना repository *repo)
{
	स्थिर संरचना option builtin_sparse_checkout_disable_options[] = {
		OPT_अन्तः(),
	};
	संरचना आदर्शः_list pl;
	संरचना repo_config_मानानि *cfg = repo_config_मानानि(the_repository);

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

	सज्जता_work_वृक्षः(the_repository);
	argc = parse_options(argc, argv, उपसर्गः,
			     builtin_sparse_checkout_disable_options,
			     builtin_sparse_checkout_disable_usage, 0);

	/*
	 * Disable the advice message for expanding a sparse index, as we
	 * are expecting to do that when disabling sparse-checkout.
	 */
	give_advice_on_expansion = 0;
	repo_read_सूचकांक(repo);

	स्मृतिं_पूरय(&pl, 0, परिमाण(pl));
	hashmap_init(&pl.recursive_hashmap, pl_hashmap_cmp, रिक्त, 0);
	hashmap_init(&pl.parent_hashmap, pl_hashmap_cmp, रिक्त, 0);
	pl.use_cone_patterns = 0;
	cfg->apply_sparse_checkout = 1;

	सङ्कलय_आदर्शः("/*", रिक्तानि_आधारः, 0, &pl, 0);

	prepare_repo_settings(the_repository);
	repo->settings.sparse_सूचकांक = 0;

	यदि (update_working_directory(repo, &pl))
		die(_("error while refreshing working directory"));

	clear_आदर्शः_list(&pl);
	प्रत्यागम समुच्चयः_config(repo, बहुलकम्_NO_PATTERNS);
}

स्थिर वर्ण ध्रुव * ध्रुव builtin_sparse_checkout_check_नियमाः_usage[] = {
	N_("git sparse-checkout check-rules [-z] [--skip-checks]"
	   "[--[no-]cone] [--rules-file <file>]"),
	रिक्त
};

स्थिर संरचना sparse_checkout_check_नियमाः_opts {
	पूर्णाङ्क cone_बहुलकम्;
	पूर्णाङ्क null_termination;
	वर्ण *नियमाः_file;
} check_नियमाः_opts;

स्थिर पूर्णाङ्क check_नियमाः(संरचना repository *repo,
		       संरचना आदर्शः_list *pl,
		       पूर्णाङ्क null_terminated)
{
	संरचना strbuf पङ्क्तिः = STRBUF_INIT;
	संरचना strbuf unquoted = STRBUF_INIT;
	वर्ण *मार्गः;
	पूर्णाङ्क पङ्क्तिः_terminator = null_terminated ? 0 : '\n';
	strbuf_getline_fn getline_fn = null_terminated ? strbuf_getline_nul
		: strbuf_getline;
	repo->सूचकांक->sparse_checkout_patterns = pl;
	यावत् (!getline_fn(&पङ्क्तिः, stdin)) {
		मार्गः = पङ्क्तिः.buf;
		यदि (!null_terminated && पङ्क्तिः.buf[0] == '"') {
			strbuf_reset(&unquoted);
			यदि (unquote_c_style(&unquoted, पङ्क्तिः.buf, रिक्त))
				die(_("unable to unquote C-style string '%s'"),
					पङ्क्तिः.buf);

			मार्गः = unquoted.buf;
		}

		यदि (मार्गः_अन्तर्_sparse_checkout(मार्गः, repo->सूचकांक))
			write_name_quoted(मार्गः, stdout, पङ्क्तिः_terminator);
	}
	strbuf_release(&पङ्क्तिः);
	strbuf_release(&unquoted);

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क sparse_checkout_check_नियमाः(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
				       संरचना repository *repo)
{
	स्थिर संरचना option builtin_sparse_checkout_check_नियमाः_options[] = {
		OPT_BOOL('z', रिक्त, &check_नियमाः_opts.null_termination,
			 N_("terminate input and output files by a NUL character")),
		OPT_BOOL(0, "cone", &check_नियमाः_opts.cone_बहुलकम्,
			 N_("when used with --rules-file interpret patterns as cone mode patterns")),
		OPT_FILENAME(0, "rules-file", &check_नियमाः_opts.नियमाः_file,
			 N_("use patterns in <file> instead of the current ones.")),
		OPT_अन्तः(),
	};

	FILE *fp;
	पूर्णाङ्क ret;
	संरचना आदर्शः_list pl = {0};
	संरचना repo_config_मानानि *cfg = repo_config_मानानि(the_repository);
	वर्ण *sparse_filename;
	check_नियमाः_opts.cone_बहुलकम् = -1;

	argc = parse_options(argc, argv, उपसर्गः,
			     builtin_sparse_checkout_check_नियमाः_options,
			     builtin_sparse_checkout_check_नियमाः_usage, 0);

	यदि (check_नियमाः_opts.नियमाः_file && check_नियमाः_opts.cone_बहुलकम् < 0)
		check_नियमाः_opts.cone_बहुलकम् = 1;

	update_cone_बहुलकम्(&check_नियमाः_opts.cone_बहुलकम्);
	pl.use_cone_patterns = cfg->core_sparse_checkout_cone;
	यदि (check_नियमाः_opts.नियमाः_file) {
		fp = xfopen(check_नियमाः_opts.नियमाः_file, "r");
		सङ्कलय_patterns_from_निवेशः(&pl, argc, argv, fp);
		fclose(fp);
	} अन्यथा {
		sparse_filename = get_sparse_checkout_filename();
		यदि (सङ्कलय_patterns_from_file_गन्तव्यम्_list(sparse_filename, "", 0, &pl,
						   रिक्त, 0))
			die(_("unable to load existing sparse-checkout patterns"));
		मुक्त(sparse_filename);
	}

	ret = check_नियमाः(repo, &pl, check_नियमाः_opts.null_termination);
	clear_आदर्शः_list(&pl);
	मुक्त(check_नियमाः_opts.नियमाः_file);
	प्रत्यागम ret;
}

पूर्णाङ्क cmd_sparse_checkout(पूर्णाङ्क argc,
			ध्रुव वर्ण **argv,
			ध्रुव वर्ण *उपसर्गः,
			संरचना repository *repo)
{
	parse_opt_subcommand_fn *fn = रिक्त;
	संरचना option builtin_sparse_checkout_options[] = {
		OPT_SUBCOMMAND("list", &fn, sparse_checkout_list),
		OPT_SUBCOMMAND("init", &fn, sparse_checkout_init),
		OPT_SUBCOMMAND("set", &fn, sparse_checkout_समुच्चयः),
		OPT_SUBCOMMAND("add", &fn, sparse_checkout_सङ्कलय),
		OPT_SUBCOMMAND("reapply", &fn, sparse_checkout_reapply),
		OPT_SUBCOMMAND("clean", &fn, sparse_checkout_clean),
		OPT_SUBCOMMAND("disable", &fn, sparse_checkout_disable),
		OPT_SUBCOMMAND("check-rules", &fn, sparse_checkout_check_नियमाः),
		OPT_अन्तः(),
	};

	argc = parse_options(argc, argv, उपसर्गः,
			     builtin_sparse_checkout_options,
			     builtin_sparse_checkout_usage, 0);

	repo_config(the_repository, git_default_config, रिक्त);

	prepare_repo_settings(repo);
	repo->settings.command_requires_पूर्णम्_सूचकांक = 0;

	प्रत्यागम fn(argc, argv, उपसर्गः, repo);
}
