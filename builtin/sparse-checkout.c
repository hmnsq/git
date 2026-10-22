#घोषणा USE_THE_REPOSITORY_VARIABLE
#घोषणा DISABLE_SIGN_COMPARE_WARNINGS

#समावेश "builtin.h"
#समावेश "abspath.h"
#समावेश "config.h"
#समावेश "dir.h"
#समावेश "environment.h"
#समावेश "gettext.h"
#समावेश "object-file.h"
#समावेश "object-name.h"
#समावेश "parse-options.h"
#समावेश "path.h"
#समावेश "pathspec.h"
#समावेश "strbuf.h"
#समावेश "string-list.h"
#समावेश "lockfile.h"
#समावेश "unpack-trees.h"
#समावेश "quote.h"
#समावेश "setup.h"
#समावेश "sparse-index.h"
#समावेश "worktree.h"

अटल स्थिर अक्षर *empty_base = "";

अटल अक्षर स्थिर * स्थिर builtin_sparse_checkout_usage[] = {
	N_("git sparse-checkout (init | list | set | add | reapply | disable | check-rules | clean) [<options>]"),
	शून्य
};

अटल व्योम write_patterns_to_file(खाता *fp, काष्ठा pattern_list *pl)
{
	पूर्णांक i;

	क्रम (i = 0; i < pl->nr; i++) {
		काष्ठा path_pattern *p = pl->patterns[i];

		अगर (p->flags & PATTERN_FLAG_NEGATIVE)
			ख_लिखो(fp, "!");

		ख_लिखो(fp, "%s", p->pattern);

		अगर (p->flags & PATTERN_FLAG_MUSTBEDIR)
			ख_लिखो(fp, "/");

		ख_लिखो(fp, "\n");
	}
}

अटल अक्षर स्थिर * स्थिर builtin_sparse_checkout_list_usage[] = {
	"git sparse-checkout list",
	शून्य
};

अटल पूर्णांक sparse_checkout_list(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
				काष्ठा repository *repo UNUSED)
{
	अटल काष्ठा option builtin_sparse_checkout_list_options[] = {
		OPT_END(),
	};
	काष्ठा pattern_list pl;
	अक्षर *sparse_filename;
	पूर्णांक res;
	काष्ठा repo_config_मूल्यावली *cfg = repo_config_मूल्यावली(the_repository);

	setup_work_वृक्ष(the_repository);
	अगर (!cfg->apply_sparse_checkout)
		die(_("this worktree is not sparse"));

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_list_options,
			     builtin_sparse_checkout_list_usage, 0);

	स_रखो(&pl, 0, माप(pl));

	pl.use_cone_patterns = cfg->core_sparse_checkout_cone;

	sparse_filename = get_sparse_checkout_filename();
	res = add_patterns_from_file_to_list(sparse_filename, "", 0, &pl, शून्य, 0);
	मुक्त(sparse_filename);

	अगर (res < 0) {
		warning(_("this worktree is not sparse (sparse-checkout file may not exist)"));
		वापस 0;
	}

	अगर (pl.use_cone_patterns) {
		पूर्णांक i;
		काष्ठा pattern_entry *pe;
		काष्ठा hashmap_iter iter;
		काष्ठा string_list sl = STRING_LIST_INIT_DUP;

		hashmap_for_each_entry(&pl.recursive_hashmap, &iter, pe, ent) {
			/* pe->pattern starts with "/", skip it */
			string_list_append(&sl, pe->pattern + 1);
		}

		string_list_sort_u(&sl, 0);

		क्रम (i = 0; i < sl.nr; i++) {
			quote_c_style(sl.वस्तुएँ[i].string, शून्य, मानक_निकास, 0);
			म_लिखो("\n");
		}

		string_list_clear(&sl, 0);
	} अन्यथा {
		write_patterns_to_file(मानक_निकास, &pl);
	}

	clear_pattern_list(&pl);

	वापस 0;
}

अटल व्योम clean_tracked_sparse_directories(काष्ठा repository *r)
{
	पूर्णांक i, was_full = 0;
	काष्ठा strbuf path = STRBUF_INIT;
	माप_प्रकार pathlen;
	काष्ठा string_list_item *item;
	काष्ठा string_list sparse_dirs = STRING_LIST_INIT_DUP;

	/*
	 * If we are not using cone mode patterns, then we cannot
	 * delete directories outside of the sparse cone.
	 */
	अगर (!r || !r->सूचकांक || !r->worktree)
		वापस;
	अगर (init_sparse_checkout_patterns(r->सूचकांक) ||
	    !r->सूचकांक->sparse_checkout_patterns->use_cone_patterns)
		वापस;

	/*
	 * Use the sparse index as a data structure to assist finding
	 * directories that are safe to delete. This conversion to a
	 * sparse index will not delete directories that contain
	 * conflicted entries or submodules.
	 */
	अगर (r->सूचकांक->sparse_सूचकांक == सूचकांक_EXPANDED) {
		/*
		 * If something, such as a merge conflict or other concern,
		 * prevents us from converting to a sparse index, then do
		 * not try deleting files.
		 */
		अगर (convert_to_sparse(r->सूचकांक, SPARSE_सूचकांक_MEMORY_ONLY))
			वापस;
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
	क्रम (i = 0; i < r->सूचकांक->cache_nr; i++) {
		काष्ठा cache_entry *ce = r->सूचकांक->cache[i];

		अगर (S_ISSPARSEDIR(ce->ce_mode) &&
		    repo_file_exists(r, ce->name))
			string_list_append(&sparse_dirs, ce->name);
	}

	for_each_string_list_item(item, &sparse_dirs) {
		काष्ठा dir_struct dir = DIR_INIT;
		काष्ठा pathspec p = { 0 };
		काष्ठा strvec s = STRVEC_INIT;

		strbuf_setlen(&path, pathlen);
		strbuf_addstr(&path, item->string);

		dir.flags |= DIR_SHOW_IGNORED_TOO;

		setup_standard_excludes(&dir);
		strvec_धकेलो(&s, path.buf);

		parse_pathspec(&p, PATHSPEC_GLOB, 0, शून्य, s.v);
		fill_directory(&dir, r->सूचकांक, &p);

		अगर (dir.nr) {
			warning(_("directory '%s' contains untracked files,"
				  " but is not in the sparse-checkout cone"),
				item->string);
		} अन्यथा अगर (remove_dir_recursively(&path, 0)) {
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

	अगर (was_full)
		ensure_full_सूचकांक(r->सूचकांक);
}

अटल पूर्णांक update_working_directory(काष्ठा repository *r,
				    काष्ठा pattern_list *pl)
{
	क्रमागत update_sparsity_परिणाम परिणाम;
	काष्ठा unpack_trees_options o;
	काष्ठा lock_file lock_file = LOCK_INIT;
	काष्ठा pattern_list *old_pl;

	/* If no branch has been checked out, there are no updates to make. */
	अगर (is_सूचकांक_unborn(r->सूचकांक))
		वापस UPDATE_SPARSITY_SUCCESS;

	old_pl = r->सूचकांक->sparse_checkout_patterns;
	r->सूचकांक->sparse_checkout_patterns = pl;

	स_रखो(&o, 0, माप(o));
	o.verbose_update = isatty(2);
	o.update = 1;
	o.शीर्ष_idx = -1;
	o.src_सूचकांक = r->सूचकांक;
	o.dst_सूचकांक = r->सूचकांक;
	o.skip_sparse_checkout = 0;

	setup_work_वृक्ष(the_repository);

	repo_hold_locked_सूचकांक(r, &lock_file, LOCK_DIE_ON_ERROR);

	setup_unpack_trees_porcelain(&o, "sparse-checkout");
	परिणाम = update_sparsity(&o, pl);
	clear_unpack_trees_porcelain(&o);

	अगर (परिणाम == UPDATE_SPARSITY_WARNINGS)
		/*
		 * We don't do any special handling of warnings from untracked
		 * files in the way or dirty entries that can't be removed.
		 */
		परिणाम = UPDATE_SPARSITY_SUCCESS;
	अगर (परिणाम == UPDATE_SPARSITY_SUCCESS)
		write_locked_सूचकांक(r->सूचकांक, &lock_file, COMMIT_LOCK);
	अन्यथा
		rollback_lock_file(&lock_file);

	clean_tracked_sparse_directories(r);

	अगर (r->सूचकांक->sparse_checkout_patterns != pl) {
		clear_pattern_list(r->सूचकांक->sparse_checkout_patterns);
		FREE_AND_NULL(r->सूचकांक->sparse_checkout_patterns);
	}
	r->सूचकांक->sparse_checkout_patterns = old_pl;

	वापस परिणाम;
}

अटल अक्षर *escaped_pattern(अक्षर *pattern)
{
	अक्षर *p = pattern;
	काष्ठा strbuf final = STRBUF_INIT;

	जबतक (*p) {
		अगर (is_glob_special(*p))
			strbuf_addch(&final, '\\');

		strbuf_addch(&final, *p);
		p++;
	}

	वापस strbuf_detach(&final, शून्य);
}

अटल व्योम write_cone_to_file(खाता *fp, काष्ठा pattern_list *pl)
{
	पूर्णांक i;
	काष्ठा pattern_entry *pe;
	काष्ठा hashmap_iter iter;
	काष्ठा string_list sl = STRING_LIST_INIT_DUP;
	काष्ठा strbuf parent_pattern = STRBUF_INIT;

	hashmap_for_each_entry(&pl->parent_hashmap, &iter, pe, ent) {
		अगर (hashmap_get_entry(&pl->recursive_hashmap, pe, ent, शून्य))
			जारी;

		अगर (!hashmap_contains_parent(&pl->recursive_hashmap,
					     pe->pattern,
					     &parent_pattern))
			string_list_append(&sl, pe->pattern);
	}

	string_list_sort_u(&sl, 0);

	ख_लिखो(fp, "/*\n!/*/\n");

	क्रम (i = 0; i < sl.nr; i++) {
		अक्षर *pattern = escaped_pattern(sl.वस्तुएँ[i].string);

		अगर (म_माप(pattern))
			ख_लिखो(fp, "%s/\n!%s/*/\n", pattern, pattern);
		मुक्त(pattern);
	}

	string_list_clear(&sl, 0);

	hashmap_for_each_entry(&pl->recursive_hashmap, &iter, pe, ent) {
		अगर (!hashmap_contains_parent(&pl->recursive_hashmap,
					     pe->pattern,
					     &parent_pattern))
			string_list_append(&sl, pe->pattern);
	}

	strbuf_release(&parent_pattern);

	string_list_sort_u(&sl, 0);

	क्रम (i = 0; i < sl.nr; i++) {
		अक्षर *pattern = escaped_pattern(sl.वस्तुएँ[i].string);
		ख_लिखो(fp, "%s/\n", pattern);
		मुक्त(pattern);
	}

	string_list_clear(&sl, 0);
}

अटल पूर्णांक write_patterns_and_update(काष्ठा repository *repo,
				     काष्ठा pattern_list *pl)
{
	अक्षर *sparse_filename;
	खाता *fp;
	काष्ठा lock_file lk = LOCK_INIT;
	पूर्णांक परिणाम;
	काष्ठा repo_config_मूल्यावली *cfg = repo_config_मूल्यावली(the_repository);

	sparse_filename = get_sparse_checkout_filename();

	अगर (safe_create_leading_directories(repo, sparse_filename))
		die(_("failed to create directory for sparse-checkout file"));

	repo_hold_lock_file_for_update(repo, &lk, sparse_filename,
				       LOCK_DIE_ON_ERROR);

	परिणाम = update_working_directory(repo, pl);
	अगर (परिणाम) {
		rollback_lock_file(&lk);
		update_working_directory(repo, शून्य);
		जाओ out;
	}

	fp = fdopen_lock_file(&lk, "w");
	अगर (!fp)
		die_errno(_("unable to fdopen %s"), get_lock_file_path(&lk));

	अगर (cfg->core_sparse_checkout_cone)
		write_cone_to_file(fp, pl);
	अन्यथा
		write_patterns_to_file(fp, pl);

	अगर (commit_lock_file(&lk))
		die_errno(_("unable to write %s"), sparse_filename);

out:
	clear_pattern_list(pl);
	मुक्त(sparse_filename);
	वापस परिणाम;
}

क्रमागत sparse_checkout_mode {
	MODE_NO_PATTERNS = 0,
	MODE_ALL_PATTERNS = 1,
	MODE_CONE_PATTERNS = 2,
};

अटल पूर्णांक set_config(काष्ठा repository *repo,
		      क्रमागत sparse_checkout_mode mode)
{
	/* Update to use worktree config, if not already. */
	अगर (init_worktree_config(repo)) {
		error(_("failed to initialize worktree config"));
		वापस 1;
	}

	अगर (repo_config_set_worktree_gently(repo,
					    "core.sparseCheckout",
					    mode ? "true" : "false") ||
	    repo_config_set_worktree_gently(repo,
					    "core.sparseCheckoutCone",
					    mode == MODE_CONE_PATTERNS ?
						"true" : "false"))
		वापस 1;

	अगर (mode == MODE_NO_PATTERNS)
		वापस set_sparse_सूचकांक_config(repo, 0);

	वापस 0;
}

अटल क्रमागत sparse_checkout_mode update_cone_mode(पूर्णांक *cone_mode) {
	काष्ठा repo_config_मूल्यावली *cfg = repo_config_मूल्यावली(the_repository);

	/* If not specified, use previous definition of cone mode */
	अगर (*cone_mode == -1 && cfg->apply_sparse_checkout)
		*cone_mode = cfg->core_sparse_checkout_cone;

	/* Set cone/non-cone mode appropriately */
	cfg->apply_sparse_checkout = 1;
	अगर (*cone_mode == 1 || *cone_mode == -1) {
		cfg->core_sparse_checkout_cone = 1;
		वापस MODE_CONE_PATTERNS;
	}
	cfg->core_sparse_checkout_cone = 0;
	वापस MODE_ALL_PATTERNS;
}

अटल पूर्णांक update_modes(काष्ठा repository *repo, पूर्णांक *cone_mode, पूर्णांक *sparse_सूचकांक)
{
	पूर्णांक mode, record_mode;
	काष्ठा repo_config_मूल्यावली *cfg = repo_config_मूल्यावली(the_repository);

	/* Determine if we need to record the mode; ensure sparse checkout on */
	record_mode = (*cone_mode != -1) || !cfg->apply_sparse_checkout;

	mode = update_cone_mode(cone_mode);
	अगर (record_mode && set_config(repo, mode))
		वापस 1;

	/* Set sparse-index/non-sparse-index mode if specified */
	अगर (*sparse_सूचकांक >= 0) {
		अगर (set_sparse_सूचकांक_config(repo, *sparse_सूचकांक) < 0)
			die(_("failed to modify sparse-index config"));

		/* force an index rewrite */
		repo_read_सूचकांक(repo);
		repo->सूचकांक->updated_workdir = 1;

		अगर (!*sparse_सूचकांक)
			ensure_full_सूचकांक(repo->सूचकांक);
	}

	वापस 0;
}

अटल अक्षर स्थिर * स्थिर builtin_sparse_checkout_init_usage[] = {
	"git sparse-checkout init [--cone] [--[no-]sparse-index]",
	शून्य
};

अटल काष्ठा sparse_checkout_init_opts {
	पूर्णांक cone_mode;
	पूर्णांक sparse_सूचकांक;
} init_opts;

अटल पूर्णांक sparse_checkout_init(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
				काष्ठा repository *repo)
{
	काष्ठा pattern_list pl;
	अक्षर *sparse_filename;
	पूर्णांक res;
	काष्ठा object_id oid;

	अटल काष्ठा option builtin_sparse_checkout_init_options[] = {
		OPT_BOOL(0, "cone", &init_opts.cone_mode,
			 N_("initialize the sparse-checkout in cone mode")),
		OPT_BOOL(0, "sparse-index", &init_opts.sparse_सूचकांक,
			 N_("toggle the use of a sparse index")),
		OPT_END(),
	};

	setup_work_वृक्ष(the_repository);
	repo_read_सूचकांक(repo);

	init_opts.cone_mode = -1;
	init_opts.sparse_सूचकांक = -1;

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_init_options,
			     builtin_sparse_checkout_init_usage, 0);

	अगर (update_modes(repo, &init_opts.cone_mode, &init_opts.sparse_सूचकांक))
		वापस 1;

	स_रखो(&pl, 0, माप(pl));

	sparse_filename = get_sparse_checkout_filename();
	res = add_patterns_from_file_to_list(sparse_filename, "", 0, &pl, शून्य, 0);

	/* If we already have a sparse-checkout file, use it. */
	अगर (res >= 0) {
		मुक्त(sparse_filename);
		clear_pattern_list(&pl);
		वापस update_working_directory(repo, शून्य);
	}

	अगर (repo_get_oid(repo, "HEAD", &oid)) {
		खाता *fp;

		/* assume we are in a fresh repo, but update the sparse-checkout file */
		अगर (safe_create_leading_directories(repo, sparse_filename))
			die(_("unable to create leading directories of %s"),
			    sparse_filename);
		fp = xfopen(sparse_filename, "w");
		अगर (!fp)
			die(_("failed to open '%s'"), sparse_filename);

		मुक्त(sparse_filename);
		ख_लिखो(fp, "/*\n!/*/\n");
		ख_बंद(fp);
		वापस 0;
	}

	मुक्त(sparse_filename);

	add_pattern("/*", empty_base, 0, &pl, 0);
	add_pattern("!/*/", empty_base, 0, &pl, 0);
	pl.use_cone_patterns = init_opts.cone_mode;

	वापस write_patterns_and_update(repo, &pl);
}

अटल व्योम जोड़ो_recursive_pattern(काष्ठा pattern_list *pl, काष्ठा strbuf *path)
{
	काष्ठा pattern_entry *e = xmalloc(माप(*e));
	e->patternlen = path->len;
	e->pattern = strbuf_detach(path, शून्य);
	hashmap_entry_init(&e->ent, fspathhash(e->pattern));

	hashmap_add(&pl->recursive_hashmap, &e->ent);

	जबतक (e->patternlen) {
		अक्षर *slash = म_खोजप(e->pattern, '/');
		अक्षर *oldpattern = e->pattern;
		माप_प्रकार newlen;
		काष्ठा pattern_entry *dup;

		अगर (!slash || slash == e->pattern)
			अवरोध;

		newlen = slash - e->pattern;
		e = xmalloc(माप(काष्ठा pattern_entry));
		e->patternlen = newlen;
		e->pattern = xstrndup(oldpattern, newlen);
		hashmap_entry_init(&e->ent, fspathhash(e->pattern));

		dup = hashmap_get_entry(&pl->parent_hashmap, e, ent, शून्य);
		अगर (!dup) {
			hashmap_add(&pl->parent_hashmap, &e->ent);
		} अन्यथा {
			मुक्त(e->pattern);
			मुक्त(e);
			e = dup;
		}
	}
}

अटल व्योम strbuf_to_cone_pattern(काष्ठा strbuf *पंक्ति, काष्ठा pattern_list *pl)
{
	strbuf_trim(पंक्ति);

	strbuf_trim_trailing_dir_sep(पंक्ति);

	अगर (strbuf_normalize_path(पंक्ति))
		die(_("could not normalize path %s"), पंक्ति->buf);

	अगर (!पंक्ति->len)
		वापस;

	अगर (पंक्ति->buf[0] != '/')
		strbuf_insertstr(पंक्ति, 0, "/");

	जोड़ो_recursive_pattern(pl, पंक्ति);
}

अटल व्योम add_patterns_from_निवेश(काष्ठा pattern_list *pl,
				    पूर्णांक argc, स्थिर अक्षर **argv,
				    खाता *file)
{
	पूर्णांक i;
	काष्ठा repo_config_मूल्यावली *cfg = repo_config_मूल्यावली(the_repository);

	अगर (cfg->core_sparse_checkout_cone) {
		काष्ठा strbuf पंक्ति = STRBUF_INIT;

		hashmap_init(&pl->recursive_hashmap, pl_hashmap_cmp, शून्य, 0);
		hashmap_init(&pl->parent_hashmap, pl_hashmap_cmp, शून्य, 0);
		pl->use_cone_patterns = 1;

		अगर (file) {
			काष्ठा strbuf unquoted = STRBUF_INIT;
			जबतक (!strbuf_getline(&पंक्ति, file)) {
				अगर (पंक्ति.buf[0] == '"') {
					strbuf_reset(&unquoted);
					अगर (unquote_c_style(&unquoted, पंक्ति.buf, शून्य))
						die(_("unable to unquote C-style string '%s'"),
						पंक्ति.buf);

					strbuf_swap(&unquoted, &पंक्ति);
				}

				strbuf_to_cone_pattern(&पंक्ति, pl);
			}

			strbuf_release(&unquoted);
		} अन्यथा {
			क्रम (i = 0; i < argc; i++) {
				strbuf_setlen(&पंक्ति, 0);
				strbuf_addstr(&पंक्ति, argv[i]);
				strbuf_to_cone_pattern(&पंक्ति, pl);
			}
		}
		strbuf_release(&पंक्ति);
	} अन्यथा {
		अगर (file) {
			काष्ठा strbuf पंक्ति = STRBUF_INIT;

			जबतक (!strbuf_getline(&पंक्ति, file))
				add_pattern(पंक्ति.buf, empty_base, 0, pl, 0);

			strbuf_release(&पंक्ति);
		} अन्यथा {
			क्रम (i = 0; i < argc; i++)
				add_pattern(argv[i], empty_base, 0, pl, 0);
		}
	}
}

क्रमागत modify_type {
	REPLACE,
	ADD,
};

अटल व्योम add_patterns_cone_mode(पूर्णांक argc, स्थिर अक्षर **argv,
				   काष्ठा pattern_list *pl,
				   पूर्णांक use_stdin)
{
	काष्ठा strbuf बफर = STRBUF_INIT;
	काष्ठा pattern_entry *pe;
	काष्ठा hashmap_iter iter;
	काष्ठा pattern_list existing;
	काष्ठा repo_config_मूल्यावली *cfg = repo_config_मूल्यावली(the_repository);
	अक्षर *sparse_filename = get_sparse_checkout_filename();

	add_patterns_from_निवेश(pl, argc, argv,
				use_stdin ? मानक_निवेश : शून्य);

	स_रखो(&existing, 0, माप(existing));
	existing.use_cone_patterns = cfg->core_sparse_checkout_cone;

	अगर (add_patterns_from_file_to_list(sparse_filename, "", 0,
					   &existing, शून्य, 0))
		die(_("unable to load existing sparse-checkout patterns"));
	मुक्त(sparse_filename);

	अगर (!existing.use_cone_patterns)
		die(_("existing sparse-checkout patterns do not use cone mode"));

	hashmap_for_each_entry(&existing.recursive_hashmap, &iter, pe, ent) {
		अगर (!hashmap_contains_parent(&pl->recursive_hashmap,
					pe->pattern, &बफर) ||
		    !hashmap_contains_parent(&pl->parent_hashmap,
					pe->pattern, &बफर)) {
			strbuf_reset(&बफर);
			strbuf_addstr(&बफर, pe->pattern);
			जोड़ो_recursive_pattern(pl, &बफर);
		}
	}

	clear_pattern_list(&existing);
	strbuf_release(&बफर);
}

अटल व्योम add_patterns_literal(पूर्णांक argc, स्थिर अक्षर **argv,
				 काष्ठा pattern_list *pl,
				 पूर्णांक use_stdin)
{
	अक्षर *sparse_filename = get_sparse_checkout_filename();
	अगर (add_patterns_from_file_to_list(sparse_filename, "", 0,
					   pl, शून्य, 0))
		die(_("unable to load existing sparse-checkout patterns"));
	मुक्त(sparse_filename);
	add_patterns_from_निवेश(pl, argc, argv, use_stdin ? मानक_निवेश : शून्य);
}

अटल पूर्णांक modify_pattern_list(काष्ठा repository *repo,
			       काष्ठा strvec *args, पूर्णांक use_stdin,
			       क्रमागत modify_type m)
{
	पूर्णांक परिणाम;
	पूर्णांक changed_config = 0;
	काष्ठा pattern_list *pl = xcalloc(1, माप(*pl));
	काष्ठा repo_config_मूल्यावली *cfg = repo_config_मूल्यावली(the_repository);

	चयन (m) {
	हाल ADD:
		अगर (cfg->core_sparse_checkout_cone)
			add_patterns_cone_mode(args->nr, args->v, pl, use_stdin);
		अन्यथा
			add_patterns_literal(args->nr, args->v, pl, use_stdin);
		अवरोध;

	हाल REPLACE:
		add_patterns_from_निवेश(pl, args->nr, args->v,
					use_stdin ? मानक_निवेश : शून्य);
		अवरोध;
	}

	अगर (!cfg->apply_sparse_checkout) {
		set_config(repo, MODE_ALL_PATTERNS);
		cfg->apply_sparse_checkout = 1;
		changed_config = 1;
	}

	परिणाम = write_patterns_and_update(repo, pl);

	अगर (परिणाम && changed_config)
		set_config(repo, MODE_NO_PATTERNS);

	clear_pattern_list(pl);
	मुक्त(pl);
	वापस परिणाम;
}

अटल व्योम sanitize_paths(काष्ठा repository *repo,
			   काष्ठा strvec *args,
			   स्थिर अक्षर *prefix, पूर्णांक skip_checks)
{
	पूर्णांक i;
	काष्ठा repo_config_मूल्यावली *cfg = repo_config_मूल्यावली(the_repository);

	अगर (!args->nr)
		वापस;

	अगर (prefix && *prefix && cfg->core_sparse_checkout_cone) {
		/*
		 * The args are not pathspecs, so unfortunately we
		 * cannot imitate how cmd_add() uses parse_pathspec().
		 */
		पूर्णांक prefix_len = म_माप(prefix);

		क्रम (i = 0; i < args->nr; i++) {
			अक्षर *prefixed_path = prefix_path(the_repository, prefix,
							  prefix_len, args->v[i]);
			strvec_replace(args, i, prefixed_path);
			मुक्त(prefixed_path);
		}
	}

	अगर (skip_checks)
		वापस;

	अगर (prefix && *prefix && !cfg->core_sparse_checkout_cone)
		die(_("please run from the toplevel directory in non-cone mode"));

	अगर (cfg->core_sparse_checkout_cone) {
		क्रम (i = 0; i < args->nr; i++) {
			अगर (args->v[i][0] == '/')
				die(_("specify directories rather than patterns (no leading slash)"));
			अगर (args->v[i][0] == '!')
				die(_("specify directories rather than patterns.  If your directory starts with a '!', pass --skip-checks"));
			अगर (strpbrk(args->v[i], "*?[]"))
				die(_("specify directories rather than patterns.  If your directory really has any of '*?[]\\' in it, pass --skip-checks"));
		}
	}

	क्रम (i = 0; i < args->nr; i++) {
		काष्ठा cache_entry *ce;
		काष्ठा सूचकांक_state *सूचकांक = repo->सूचकांक;
		पूर्णांक pos = सूचकांक_name_pos(सूचकांक, args->v[i], म_माप(args->v[i]));

		अगर (pos < 0)
			जारी;
		ce = सूचकांक->cache[pos];
		अगर (S_ISSPARSEDIR(ce->ce_mode))
			जारी;

		अगर (cfg->core_sparse_checkout_cone)
			die(_("'%s' is not a directory; to treat it as a directory anyway, rerun with --skip-checks"), args->v[i]);
		अन्यथा
			warning(_("pass a leading slash before paths such as '%s' if you want a single file (see NON-CONE PROBLEMS in the git-sparse-checkout manual)."), args->v[i]);
	}
}

अटल अक्षर स्थिर * स्थिर builtin_sparse_checkout_add_usage[] = {
	N_("git sparse-checkout add [--skip-checks] (--stdin | <patterns>)"),
	शून्य
};

अटल काष्ठा sparse_checkout_add_opts {
	पूर्णांक skip_checks;
	पूर्णांक use_stdin;
} add_opts;

अटल पूर्णांक sparse_checkout_add(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
			       काष्ठा repository *repo)
{
	अटल काष्ठा option builtin_sparse_checkout_add_options[] = {
		OPT_BOOL_F(0, "skip-checks", &add_opts.skip_checks,
			   N_("skip some sanity checks on the given paths that might give false positives"),
			   PARSE_OPT_NONEG),
		OPT_BOOL(0, "stdin", &add_opts.use_stdin,
			 N_("read patterns from standard in")),
		OPT_END(),
	};
	काष्ठा strvec patterns = STRVEC_INIT;
	पूर्णांक ret;
	काष्ठा repo_config_मूल्यावली *cfg = repo_config_मूल्यावली(the_repository);

	setup_work_वृक्ष(the_repository);
	अगर (!cfg->apply_sparse_checkout)
		die(_("no sparse-checkout to add to"));

	repo_read_सूचकांक(repo);

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_add_options,
			     builtin_sparse_checkout_add_usage, 0);

	क्रम (पूर्णांक i = 0; i < argc; i++)
		strvec_धकेलो(&patterns, argv[i]);
	sanitize_paths(repo, &patterns, prefix, add_opts.skip_checks);

	ret = modify_pattern_list(repo, &patterns, add_opts.use_stdin, ADD);

	strvec_clear(&patterns);
	वापस ret;
}

अटल अक्षर स्थिर * स्थिर builtin_sparse_checkout_set_usage[] = {
	N_("git sparse-checkout set [--[no-]cone] [--[no-]sparse-index] [--skip-checks] (--stdin | <patterns>)"),
	शून्य
};

अटल काष्ठा sparse_checkout_set_opts {
	पूर्णांक cone_mode;
	पूर्णांक sparse_सूचकांक;
	पूर्णांक skip_checks;
	पूर्णांक use_stdin;
} set_opts;

अटल पूर्णांक sparse_checkout_set(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
			       काष्ठा repository *repo)
{
	काष्ठा repo_config_मूल्यावली *cfg = repo_config_मूल्यावली(the_repository);
	पूर्णांक default_patterns_nr = 2;
	स्थिर अक्षर *default_patterns[] = {"/*", "!/*/", शून्य};

	अटल काष्ठा option builtin_sparse_checkout_set_options[] = {
		OPT_BOOL(0, "cone", &set_opts.cone_mode,
			 N_("initialize the sparse-checkout in cone mode")),
		OPT_BOOL(0, "sparse-index", &set_opts.sparse_सूचकांक,
			 N_("toggle the use of a sparse index")),
		OPT_BOOL_F(0, "skip-checks", &set_opts.skip_checks,
			   N_("skip some sanity checks on the given paths that might give false positives"),
			   PARSE_OPT_NONEG),
		OPT_BOOL_F(0, "stdin", &set_opts.use_stdin,
			   N_("read patterns from standard in"),
			   PARSE_OPT_NONEG),
		OPT_END(),
	};
	काष्ठा strvec patterns = STRVEC_INIT;
	पूर्णांक ret;

	setup_work_वृक्ष(the_repository);
	repo_read_सूचकांक(repo);

	set_opts.cone_mode = -1;
	set_opts.sparse_सूचकांक = -1;

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_set_options,
			     builtin_sparse_checkout_set_usage, 0);

	अगर (update_modes(repo, &set_opts.cone_mode, &set_opts.sparse_सूचकांक))
		वापस 1;

	/*
	 * Cone mode automatically specifies the toplevel directory.  For
	 * non-cone mode, if nothing is specified, manually select just the
	 * top-level directory (much as 'init' would do).
	 */
	अगर (!cfg->core_sparse_checkout_cone && !set_opts.use_stdin && argc == 0) {
		क्रम (पूर्णांक i = 0; i < default_patterns_nr; i++)
			strvec_धकेलो(&patterns, default_patterns[i]);
	} अन्यथा {
		क्रम (पूर्णांक i = 0; i < argc; i++)
			strvec_धकेलो(&patterns, argv[i]);
		sanitize_paths(repo, &patterns, prefix, set_opts.skip_checks);
	}

	ret = modify_pattern_list(repo, &patterns, set_opts.use_stdin, REPLACE);

	strvec_clear(&patterns);
	वापस ret;
}

अटल अक्षर स्थिर * स्थिर builtin_sparse_checkout_reapply_usage[] = {
	"git sparse-checkout reapply [--[no-]cone] [--[no-]sparse-index]",
	शून्य
};

अटल काष्ठा sparse_checkout_reapply_opts {
	पूर्णांक cone_mode;
	पूर्णांक sparse_सूचकांक;
} reapply_opts;

अटल पूर्णांक sparse_checkout_reapply(पूर्णांक argc, स्थिर अक्षर **argv,
				   स्थिर अक्षर *prefix,
				   काष्ठा repository *repo)
{
	अटल काष्ठा option builtin_sparse_checkout_reapply_options[] = {
		OPT_BOOL(0, "cone", &reapply_opts.cone_mode,
			 N_("initialize the sparse-checkout in cone mode")),
		OPT_BOOL(0, "sparse-index", &reapply_opts.sparse_सूचकांक,
			 N_("toggle the use of a sparse index")),
		OPT_END(),
	};
	काष्ठा repo_config_मूल्यावली *cfg = repo_config_मूल्यावली(the_repository);

	setup_work_वृक्ष(the_repository);
	अगर (!cfg->apply_sparse_checkout)
		die(_("must be in a sparse-checkout to reapply sparsity patterns"));

	reapply_opts.cone_mode = -1;
	reapply_opts.sparse_सूचकांक = -1;

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_reapply_options,
			     builtin_sparse_checkout_reapply_usage, 0);

	repo_read_सूचकांक(repo);

	अगर (update_modes(repo, &reapply_opts.cone_mode, &reapply_opts.sparse_सूचकांक))
		वापस 1;

	वापस update_working_directory(repo, शून्य);
}

अटल अक्षर स्थिर * स्थिर builtin_sparse_checkout_clean_usage[] = {
	"git sparse-checkout clean [-n|--dry-run]",
	शून्य
};

अटल पूर्णांक list_file_iterator(स्थिर अक्षर *path, स्थिर व्योम *तथ्य)
{
	स्थिर अक्षर *संदेश = तथ्य;

	म_लिखो(संदेश, path);
	वापस 0;
}

अटल व्योम list_every_file_in_dir(स्थिर अक्षर *संदेश,
				   स्थिर अक्षर *directory)
{
	काष्ठा strbuf path = STRBUF_INIT;

	strbuf_addstr(&path, directory);
	for_each_file_in_dir(&path, list_file_iterator, संदेश);
	strbuf_release(&path);
}

अटल स्थिर अक्षर *संदेश_remove = N_("Removing %s\n");
अटल स्थिर अक्षर *संदेश_would_remove = N_("Would remove %s\n");

अटल पूर्णांक sparse_checkout_clean(पूर्णांक argc, स्थिर अक्षर **argv,
				   स्थिर अक्षर *prefix,
				   काष्ठा repository *repo)
{
	काष्ठा strbuf full_path = STRBUF_INIT;
	स्थिर अक्षर *संदेश = संदेश_remove;
	माप_प्रकार worktree_len;
	पूर्णांक force = 0, dry_run = 0, verbose = 0;
	पूर्णांक require_force = 1;
	काष्ठा repo_config_मूल्यावली *cfg = repo_config_मूल्यावली(the_repository);

	काष्ठा option builtin_sparse_checkout_clean_options[] = {
		OPT__DRY_RUN(&dry_run, N_("dry run")),
		OPT__FORCE(&force, N_("force"), PARSE_OPT_NOCOMPLETE),
		OPT__VERBOSE(&verbose, N_("report each affected file, not just directories")),
		OPT_END(),
	};

	setup_work_वृक्ष(the_repository);
	अगर (!cfg->apply_sparse_checkout)
		die(_("must be in a sparse-checkout to clean directories"));
	अगर (!cfg->core_sparse_checkout_cone)
		die(_("must be in a cone-mode sparse-checkout to clean directories"));

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_clean_options,
			     builtin_sparse_checkout_clean_usage, 0);

	repo_config_get_bool(repo, "clean.requireforce", &require_force);
	अगर (require_force && !force && !dry_run)
		die(_("for safety, refusing to clean without one of --force or --dry-run"));

	अगर (dry_run)
		संदेश = संदेश_would_remove;

	अगर (repo_read_सूचकांक(repo) < 0)
		die(_("failed to read index"));

	अगर (convert_to_sparse(repo->सूचकांक, SPARSE_सूचकांक_MEMORY_ONLY) ||
	    repo->सूचकांक->sparse_सूचकांक == सूचकांक_EXPANDED)
		die(_("failed to convert index to a sparse index; resolve merge conflicts and try again"));

	strbuf_addstr(&full_path, repo->worktree);
	strbuf_addch(&full_path, '/');
	worktree_len = full_path.len;

	क्रम (माप_प्रकार i = 0; i < repo->सूचकांक->cache_nr; i++) {
		काष्ठा cache_entry *ce = repo->सूचकांक->cache[i];
		अगर (!S_ISSPARSEDIR(ce->ce_mode))
			जारी;
		strbuf_setlen(&full_path, worktree_len);
		strbuf_add(&full_path, ce->name, ce->ce_namelen);

		अगर (!is_directory(full_path.buf))
			जारी;

		अगर (verbose)
			list_every_file_in_dir(संदेश, ce->name);
		अन्यथा
			म_लिखो(संदेश, ce->name);

		अगर (dry_run <= 0 &&
		    remove_dir_recursively(&full_path, 0))
			warning_errno(_("failed to remove '%s'"), ce->name);
	}

	strbuf_release(&full_path);
	वापस 0;
}

अटल अक्षर स्थिर * स्थिर builtin_sparse_checkout_disable_usage[] = {
	"git sparse-checkout disable",
	शून्य
};

अटल पूर्णांक sparse_checkout_disable(पूर्णांक argc, स्थिर अक्षर **argv,
				   स्थिर अक्षर *prefix,
				   काष्ठा repository *repo)
{
	अटल काष्ठा option builtin_sparse_checkout_disable_options[] = {
		OPT_END(),
	};
	काष्ठा pattern_list pl;
	काष्ठा repo_config_मूल्यावली *cfg = repo_config_मूल्यावली(the_repository);

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

	setup_work_वृक्ष(the_repository);
	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_disable_options,
			     builtin_sparse_checkout_disable_usage, 0);

	/*
	 * Disable the advice message for expanding a sparse index, as we
	 * are expecting to do that when disabling sparse-checkout.
	 */
	give_advice_on_expansion = 0;
	repo_read_सूचकांक(repo);

	स_रखो(&pl, 0, माप(pl));
	hashmap_init(&pl.recursive_hashmap, pl_hashmap_cmp, शून्य, 0);
	hashmap_init(&pl.parent_hashmap, pl_hashmap_cmp, शून्य, 0);
	pl.use_cone_patterns = 0;
	cfg->apply_sparse_checkout = 1;

	add_pattern("/*", empty_base, 0, &pl, 0);

	prepare_repo_settings(the_repository);
	repo->settings.sparse_सूचकांक = 0;

	अगर (update_working_directory(repo, &pl))
		die(_("error while refreshing working directory"));

	clear_pattern_list(&pl);
	वापस set_config(repo, MODE_NO_PATTERNS);
}

अटल अक्षर स्थिर * स्थिर builtin_sparse_checkout_check_rules_usage[] = {
	N_("git sparse-checkout check-rules [-z] [--skip-checks]"
	   "[--[no-]cone] [--rules-file <file>]"),
	शून्य
};

अटल काष्ठा sparse_checkout_check_rules_opts {
	पूर्णांक cone_mode;
	पूर्णांक null_termination;
	अक्षर *rules_file;
} check_rules_opts;

अटल पूर्णांक check_rules(काष्ठा repository *repo,
		       काष्ठा pattern_list *pl,
		       पूर्णांक null_terminated)
{
	काष्ठा strbuf पंक्ति = STRBUF_INIT;
	काष्ठा strbuf unquoted = STRBUF_INIT;
	अक्षर *path;
	पूर्णांक पंक्ति_terminator = null_terminated ? 0 : '\n';
	strbuf_getline_fn getline_fn = null_terminated ? strbuf_getline_nul
		: strbuf_getline;
	repo->सूचकांक->sparse_checkout_patterns = pl;
	जबतक (!getline_fn(&पंक्ति, मानक_निवेश)) {
		path = पंक्ति.buf;
		अगर (!null_terminated && पंक्ति.buf[0] == '"') {
			strbuf_reset(&unquoted);
			अगर (unquote_c_style(&unquoted, पंक्ति.buf, शून्य))
				die(_("unable to unquote C-style string '%s'"),
					पंक्ति.buf);

			path = unquoted.buf;
		}

		अगर (path_in_sparse_checkout(path, repo->सूचकांक))
			write_name_quoted(path, मानक_निकास, पंक्ति_terminator);
	}
	strbuf_release(&पंक्ति);
	strbuf_release(&unquoted);

	वापस 0;
}

अटल पूर्णांक sparse_checkout_check_rules(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
				       काष्ठा repository *repo)
{
	अटल काष्ठा option builtin_sparse_checkout_check_rules_options[] = {
		OPT_BOOL('z', शून्य, &check_rules_opts.null_termination,
			 N_("terminate input and output files by a NUL character")),
		OPT_BOOL(0, "cone", &check_rules_opts.cone_mode,
			 N_("when used with --rules-file interpret patterns as cone mode patterns")),
		OPT_FILENAME(0, "rules-file", &check_rules_opts.rules_file,
			 N_("use patterns in <file> instead of the current ones.")),
		OPT_END(),
	};

	खाता *fp;
	पूर्णांक ret;
	काष्ठा pattern_list pl = {0};
	काष्ठा repo_config_मूल्यावली *cfg = repo_config_मूल्यावली(the_repository);
	अक्षर *sparse_filename;
	check_rules_opts.cone_mode = -1;

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_check_rules_options,
			     builtin_sparse_checkout_check_rules_usage, 0);

	अगर (check_rules_opts.rules_file && check_rules_opts.cone_mode < 0)
		check_rules_opts.cone_mode = 1;

	update_cone_mode(&check_rules_opts.cone_mode);
	pl.use_cone_patterns = cfg->core_sparse_checkout_cone;
	अगर (check_rules_opts.rules_file) {
		fp = xfopen(check_rules_opts.rules_file, "r");
		add_patterns_from_निवेश(&pl, argc, argv, fp);
		ख_बंद(fp);
	} अन्यथा {
		sparse_filename = get_sparse_checkout_filename();
		अगर (add_patterns_from_file_to_list(sparse_filename, "", 0, &pl,
						   शून्य, 0))
			die(_("unable to load existing sparse-checkout patterns"));
		मुक्त(sparse_filename);
	}

	ret = check_rules(repo, &pl, check_rules_opts.null_termination);
	clear_pattern_list(&pl);
	मुक्त(check_rules_opts.rules_file);
	वापस ret;
}

पूर्णांक cmd_sparse_checkout(पूर्णांक argc,
			स्थिर अक्षर **argv,
			स्थिर अक्षर *prefix,
			काष्ठा repository *repo)
{
	parse_opt_subcommand_fn *fn = शून्य;
	काष्ठा option builtin_sparse_checkout_options[] = {
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

	repo_config(the_repository, git_default_config, शून्य);

	prepare_repo_settings(repo);
	repo->settings.command_requires_full_सूचकांक = 0;

	वापस fn(argc, argv, prefix, repo);
}
