#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

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

स्थिर ध्रुव वर्ण *empty_base = "";

स्थिर वर्ण ध्रुव * ध्रुव builtin_sparse_checkout_usage[] = {
	N_("git sparse-checkout (init | list | set | add | reapply | disable | check-rules | clean) [<options>]"),
	रिक्त
};

स्थिर शून्य write_patterns_to_file(खाता *fp, संरचना pattern_list *pl)
{
	पूर्णाङ्क i;

	प्रति (i = 0; i < pl->nr; i++) {
		संरचना path_pattern *p = pl->patterns[i];

		यदि (p->flags & PATTERN_FLAG_NEGATIVE)
			ख_लिखो(fp, "!");

		ख_लिखो(fp, "%s", p->pattern);

		यदि (p->flags & PATTERN_FLAG_MUSTBEDIR)
			ख_लिखो(fp, "/");

		ख_लिखो(fp, "\n");
	}
}

स्थिर वर्ण ध्रुव * ध्रुव builtin_sparse_checkout_list_usage[] = {
	"git sparse-checkout list",
	रिक्त
};

स्थिर पूर्णाङ्क sparse_checkout_list(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix,
				संरचना repository *repo UNUSED)
{
	स्थिर संरचना option builtin_sparse_checkout_list_options[] = {
		OPT_END(),
	};
	संरचना pattern_list pl;
	वर्ण *sparse_filename;
	पूर्णाङ्क res;
	संरचना repo_config_मूल्यावली *cfg = repo_config_मूल्यावली(the_repository);

	setup_work_वृक्ष(the_repository);
	यदि (!cfg->apply_sparse_checkout)
		die(_("this worktree is not sparse"));

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_list_options,
			     builtin_sparse_checkout_list_usage, 0);

	स_रखो(&pl, 0, परिमाण(pl));

	pl.use_cone_patterns = cfg->core_sparse_checkout_cone;

	sparse_filename = get_sparse_checkout_filename();
	res = add_patterns_from_file_to_list(sparse_filename, "", 0, &pl, रिक्त, 0);
	मुक्त(sparse_filename);

	यदि (res < 0) {
		warning(_("this worktree is not sparse (sparse-checkout file may not exist)"));
		प्रत्यागम 0;
	}

	यदि (pl.use_cone_patterns) {
		पूर्णाङ्क i;
		संरचना pattern_entry *pe;
		संरचना hashmap_iter iter;
		संरचना string_list sl = STRING_LIST_INIT_DUP;

		hashmap_for_each_entry(&pl.recursive_hashmap, &iter, pe, ent) {
			/* pe->pattern starts with "/", skip it */
			string_list_append(&sl, pe->pattern + 1);
		}

		string_list_sort_u(&sl, 0);

		प्रति (i = 0; i < sl.nr; i++) {
			quote_c_style(sl.वस्तुएँ[i].string, रिक्त, मानक_निकास, 0);
			म_लिखो("\n");
		}

		string_list_clear(&sl, 0);
	} अन्यथा {
		write_patterns_to_file(मानक_निकास, &pl);
	}

	clear_pattern_list(&pl);

	प्रत्यागम 0;
}

स्थिर शून्य clean_tracked_sparse_directories(संरचना repository *r)
{
	पूर्णाङ्क i, was_full = 0;
	संरचना strbuf path = STRBUF_INIT;
	माप_प्रकार pathlen;
	संरचना string_list_item *item;
	संरचना string_list sparse_dirs = STRING_LIST_INIT_DUP;

	/*
	 * If we are not using cone mode patterns, then we cannot
	 * delete directories outside of the sparse cone.
	 */
	यदि (!r || !r->index || !r->worktree)
		प्रत्यागम;
	यदि (init_sparse_checkout_patterns(r->index) ||
	    !r->index->sparse_checkout_patterns->use_cone_patterns)
		प्रत्यागम;

	/*
	 * Use the sparse index as a data structure to assist finding
	 * directories that are safe to delete. This conversion to a
	 * sparse index will not delete directories that contain
	 * conflicted entries or submodules.
	 */
	यदि (r->index->sparse_index == INDEX_EXPANDED) {
		/*
		 * If something, such as a merge conflict or other concern,
		 * prevents us from converting to a sparse index, then do
		 * not try deleting files.
		 */
		यदि (convert_to_sparse(r->index, SPARSE_INDEX_MEMORY_ONLY))
			प्रत्यागम;
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
	प्रति (i = 0; i < r->index->cache_nr; i++) {
		संरचना cache_entry *ce = r->index->cache[i];

		यदि (S_ISSPARSEDIR(ce->ce_mode) &&
		    repo_file_exists(r, ce->name))
			string_list_append(&sparse_dirs, ce->name);
	}

	for_each_string_list_item(item, &sparse_dirs) {
		संरचना dir_struct dir = DIR_INIT;
		संरचना pathspec p = { 0 };
		संरचना strvec s = STRVEC_INIT;

		strbuf_setlen(&path, pathlen);
		strbuf_addstr(&path, item->string);

		dir.flags |= DIR_SHOW_IGNORED_TOO;

		setup_standard_excludes(&dir);
		strvec_धकेलो(&s, path.buf);

		parse_pathspec(&p, PATHSPEC_GLOB, 0, रिक्त, s.v);
		fill_directory(&dir, r->index, &p);

		यदि (dir.nr) {
			warning(_("directory '%s' contains untracked files,"
				  " but is not in the sparse-checkout cone"),
				item->string);
		} अन्यथा यदि (remove_dir_recursively(&path, 0)) {
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

	यदि (was_full)
		ensure_full_index(r->index);
}

स्थिर पूर्णाङ्क update_working_directory(संरचना repository *r,
				    संरचना pattern_list *pl)
{
	गणना update_sparsity_result result;
	संरचना unpack_trees_options o;
	संरचना lock_file lock_file = LOCK_INIT;
	संरचना pattern_list *old_pl;

	/* If no branch has been checked out, there are no updates to make. */
	यदि (is_index_unborn(r->index))
		प्रत्यागम UPDATE_SPARSITY_SUCCESS;

	old_pl = r->index->sparse_checkout_patterns;
	r->index->sparse_checkout_patterns = pl;

	स_रखो(&o, 0, परिमाण(o));
	o.verbose_update = isatty(2);
	o.update = 1;
	o.head_idx = -1;
	o.src_index = r->index;
	o.dst_index = r->index;
	o.skip_sparse_checkout = 0;

	setup_work_वृक्ष(the_repository);

	repo_hold_locked_index(r, &lock_file, LOCK_DIE_ON_ERROR);

	setup_unpack_trees_porcelain(&o, "sparse-checkout");
	result = update_sparsity(&o, pl);
	clear_unpack_trees_porcelain(&o);

	यदि (result == UPDATE_SPARSITY_WARNINGS)
		/*
		 * We don't do any special handling of warnings from untracked
		 * files in the way or dirty entries that can't be removed.
		 */
		result = UPDATE_SPARSITY_SUCCESS;
	यदि (result == UPDATE_SPARSITY_SUCCESS)
		write_locked_index(r->index, &lock_file, COMMIT_LOCK);
	अन्यथा
		rollback_lock_file(&lock_file);

	clean_tracked_sparse_directories(r);

	यदि (r->index->sparse_checkout_patterns != pl) {
		clear_pattern_list(r->index->sparse_checkout_patterns);
		FREE_AND_NULL(r->index->sparse_checkout_patterns);
	}
	r->index->sparse_checkout_patterns = old_pl;

	प्रत्यागम result;
}

स्थिर वर्ण *escaped_pattern(वर्ण *pattern)
{
	वर्ण *p = pattern;
	संरचना strbuf final = STRBUF_INIT;

	यावत् (*p) {
		यदि (is_glob_special(*p))
			strbuf_addch(&final, '\\');

		strbuf_addch(&final, *p);
		p++;
	}

	प्रत्यागम strbuf_detach(&final, रिक्त);
}

स्थिर शून्य write_cone_to_file(खाता *fp, संरचना pattern_list *pl)
{
	पूर्णाङ्क i;
	संरचना pattern_entry *pe;
	संरचना hashmap_iter iter;
	संरचना string_list sl = STRING_LIST_INIT_DUP;
	संरचना strbuf parent_pattern = STRBUF_INIT;

	hashmap_for_each_entry(&pl->parent_hashmap, &iter, pe, ent) {
		यदि (hashmap_get_entry(&pl->recursive_hashmap, pe, ent, रिक्त))
			अनुवर्त;

		यदि (!hashmap_contains_parent(&pl->recursive_hashmap,
					     pe->pattern,
					     &parent_pattern))
			string_list_append(&sl, pe->pattern);
	}

	string_list_sort_u(&sl, 0);

	ख_लिखो(fp, "/*\n!/*/\n");

	प्रति (i = 0; i < sl.nr; i++) {
		वर्ण *pattern = escaped_pattern(sl.वस्तुएँ[i].string);

		यदि (म_माप(pattern))
			ख_लिखो(fp, "%s/\n!%s/*/\n", pattern, pattern);
		मुक्त(pattern);
	}

	string_list_clear(&sl, 0);

	hashmap_for_each_entry(&pl->recursive_hashmap, &iter, pe, ent) {
		यदि (!hashmap_contains_parent(&pl->recursive_hashmap,
					     pe->pattern,
					     &parent_pattern))
			string_list_append(&sl, pe->pattern);
	}

	strbuf_release(&parent_pattern);

	string_list_sort_u(&sl, 0);

	प्रति (i = 0; i < sl.nr; i++) {
		वर्ण *pattern = escaped_pattern(sl.वस्तुएँ[i].string);
		ख_लिखो(fp, "%s/\n", pattern);
		मुक्त(pattern);
	}

	string_list_clear(&sl, 0);
}

स्थिर पूर्णाङ्क write_patterns_and_update(संरचना repository *repo,
				     संरचना pattern_list *pl)
{
	वर्ण *sparse_filename;
	खाता *fp;
	संरचना lock_file lk = LOCK_INIT;
	पूर्णाङ्क result;
	संरचना repo_config_मूल्यावली *cfg = repo_config_मूल्यावली(the_repository);

	sparse_filename = get_sparse_checkout_filename();

	यदि (safe_create_leading_directories(repo, sparse_filename))
		die(_("failed to create directory for sparse-checkout file"));

	repo_hold_lock_file_for_update(repo, &lk, sparse_filename,
				       LOCK_DIE_ON_ERROR);

	result = update_working_directory(repo, pl);
	यदि (result) {
		rollback_lock_file(&lk);
		update_working_directory(repo, रिक्त);
		गच्छ out;
	}

	fp = fdopen_lock_file(&lk, "w");
	यदि (!fp)
		die_errno(_("unable to fdopen %s"), get_lock_file_path(&lk));

	यदि (cfg->core_sparse_checkout_cone)
		write_cone_to_file(fp, pl);
	अन्यथा
		write_patterns_to_file(fp, pl);

	यदि (commit_lock_file(&lk))
		die_errno(_("unable to write %s"), sparse_filename);

out:
	clear_pattern_list(pl);
	मुक्त(sparse_filename);
	प्रत्यागम result;
}

गणना sparse_checkout_mode {
	MODE_NO_PATTERNS = 0,
	MODE_ALL_PATTERNS = 1,
	MODE_CONE_PATTERNS = 2,
};

स्थिर पूर्णाङ्क set_config(संरचना repository *repo,
		      गणना sparse_checkout_mode mode)
{
	/* Update to use worktree config, if not already. */
	यदि (init_worktree_config(repo)) {
		error(_("failed to initialize worktree config"));
		प्रत्यागम 1;
	}

	यदि (repo_config_set_worktree_gently(repo,
					    "core.sparseCheckout",
					    mode ? "true" : "false") ||
	    repo_config_set_worktree_gently(repo,
					    "core.sparseCheckoutCone",
					    mode == MODE_CONE_PATTERNS ?
						"true" : "false"))
		प्रत्यागम 1;

	यदि (mode == MODE_NO_PATTERNS)
		प्रत्यागम set_sparse_index_config(repo, 0);

	प्रत्यागम 0;
}

स्थिर गणना sparse_checkout_mode update_cone_mode(पूर्णाङ्क *cone_mode) {
	संरचना repo_config_मूल्यावली *cfg = repo_config_मूल्यावली(the_repository);

	/* If not specified, use previous definition of cone mode */
	यदि (*cone_mode == -1 && cfg->apply_sparse_checkout)
		*cone_mode = cfg->core_sparse_checkout_cone;

	/* Set cone/non-cone mode appropriately */
	cfg->apply_sparse_checkout = 1;
	यदि (*cone_mode == 1 || *cone_mode == -1) {
		cfg->core_sparse_checkout_cone = 1;
		प्रत्यागम MODE_CONE_PATTERNS;
	}
	cfg->core_sparse_checkout_cone = 0;
	प्रत्यागम MODE_ALL_PATTERNS;
}

स्थिर पूर्णाङ्क update_modes(संरचना repository *repo, पूर्णाङ्क *cone_mode, पूर्णाङ्क *sparse_index)
{
	पूर्णाङ्क mode, record_mode;
	संरचना repo_config_मूल्यावली *cfg = repo_config_मूल्यावली(the_repository);

	/* Determine if we need to record the mode; ensure sparse checkout on */
	record_mode = (*cone_mode != -1) || !cfg->apply_sparse_checkout;

	mode = update_cone_mode(cone_mode);
	यदि (record_mode && set_config(repo, mode))
		प्रत्यागम 1;

	/* Set sparse-index/non-sparse-index mode if specified */
	यदि (*sparse_index >= 0) {
		यदि (set_sparse_index_config(repo, *sparse_index) < 0)
			die(_("failed to modify sparse-index config"));

		/* force an index rewrite */
		repo_read_index(repo);
		repo->index->updated_workdir = 1;

		यदि (!*sparse_index)
			ensure_full_index(repo->index);
	}

	प्रत्यागम 0;
}

स्थिर वर्ण ध्रुव * ध्रुव builtin_sparse_checkout_init_usage[] = {
	"git sparse-checkout init [--cone] [--[no-]sparse-index]",
	रिक्त
};

स्थिर संरचना sparse_checkout_init_opts {
	पूर्णाङ्क cone_mode;
	पूर्णाङ्क sparse_index;
} init_opts;

स्थिर पूर्णाङ्क sparse_checkout_init(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix,
				संरचना repository *repo)
{
	संरचना pattern_list pl;
	वर्ण *sparse_filename;
	पूर्णाङ्क res;
	संरचना object_id oid;

	स्थिर संरचना option builtin_sparse_checkout_init_options[] = {
		OPT_BOOL(0, "cone", &init_opts.cone_mode,
			 N_("initialize the sparse-checkout in cone mode")),
		OPT_BOOL(0, "sparse-index", &init_opts.sparse_index,
			 N_("toggle the use of a sparse index")),
		OPT_END(),
	};

	setup_work_वृक्ष(the_repository);
	repo_read_index(repo);

	init_opts.cone_mode = -1;
	init_opts.sparse_index = -1;

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_init_options,
			     builtin_sparse_checkout_init_usage, 0);

	यदि (update_modes(repo, &init_opts.cone_mode, &init_opts.sparse_index))
		प्रत्यागम 1;

	स_रखो(&pl, 0, परिमाण(pl));

	sparse_filename = get_sparse_checkout_filename();
	res = add_patterns_from_file_to_list(sparse_filename, "", 0, &pl, रिक्त, 0);

	/* If we already have a sparse-checkout file, use it. */
	यदि (res >= 0) {
		मुक्त(sparse_filename);
		clear_pattern_list(&pl);
		प्रत्यागम update_working_directory(repo, रिक्त);
	}

	यदि (repo_get_oid(repo, "HEAD", &oid)) {
		खाता *fp;

		/* assume we are in a fresh repo, but update the sparse-checkout file */
		यदि (safe_create_leading_directories(repo, sparse_filename))
			die(_("unable to create leading directories of %s"),
			    sparse_filename);
		fp = xfopen(sparse_filename, "w");
		यदि (!fp)
			die(_("failed to open '%s'"), sparse_filename);

		मुक्त(sparse_filename);
		ख_लिखो(fp, "/*\n!/*/\n");
		ख_बंद(fp);
		प्रत्यागम 0;
	}

	मुक्त(sparse_filename);

	add_pattern("/*", empty_base, 0, &pl, 0);
	add_pattern("!/*/", empty_base, 0, &pl, 0);
	pl.use_cone_patterns = init_opts.cone_mode;

	प्रत्यागम write_patterns_and_update(repo, &pl);
}

स्थिर शून्य जोड़ो_recursive_pattern(संरचना pattern_list *pl, संरचना strbuf *path)
{
	संरचना pattern_entry *e = xmalloc(परिमाण(*e));
	e->patternlen = path->len;
	e->pattern = strbuf_detach(path, रिक्त);
	hashmap_entry_init(&e->ent, fspathhash(e->pattern));

	hashmap_add(&pl->recursive_hashmap, &e->ent);

	यावत् (e->patternlen) {
		वर्ण *slash = म_खोजप(e->pattern, '/');
		वर्ण *oldpattern = e->pattern;
		माप_प्रकार newlen;
		संरचना pattern_entry *dup;

		यदि (!slash || slash == e->pattern)
			भङ्ग;

		newlen = slash - e->pattern;
		e = xmalloc(परिमाण(संरचना pattern_entry));
		e->patternlen = newlen;
		e->pattern = xstrndup(oldpattern, newlen);
		hashmap_entry_init(&e->ent, fspathhash(e->pattern));

		dup = hashmap_get_entry(&pl->parent_hashmap, e, ent, रिक्त);
		यदि (!dup) {
			hashmap_add(&pl->parent_hashmap, &e->ent);
		} अन्यथा {
			मुक्त(e->pattern);
			मुक्त(e);
			e = dup;
		}
	}
}

स्थिर शून्य strbuf_to_cone_pattern(संरचना strbuf *पंक्ति, संरचना pattern_list *pl)
{
	strbuf_trim(पंक्ति);

	strbuf_trim_trailing_dir_sep(पंक्ति);

	यदि (strbuf_normalize_path(पंक्ति))
		die(_("could not normalize path %s"), पंक्ति->buf);

	यदि (!पंक्ति->len)
		प्रत्यागम;

	यदि (पंक्ति->buf[0] != '/')
		strbuf_insertstr(पंक्ति, 0, "/");

	जोड़ो_recursive_pattern(pl, पंक्ति);
}

स्थिर शून्य add_patterns_from_निवेश(संरचना pattern_list *pl,
				    पूर्णाङ्क argc, ध्रुव वर्ण **argv,
				    खाता *file)
{
	पूर्णाङ्क i;
	संरचना repo_config_मूल्यावली *cfg = repo_config_मूल्यावली(the_repository);

	यदि (cfg->core_sparse_checkout_cone) {
		संरचना strbuf पंक्ति = STRBUF_INIT;

		hashmap_init(&pl->recursive_hashmap, pl_hashmap_cmp, रिक्त, 0);
		hashmap_init(&pl->parent_hashmap, pl_hashmap_cmp, रिक्त, 0);
		pl->use_cone_patterns = 1;

		यदि (file) {
			संरचना strbuf unquoted = STRBUF_INIT;
			यावत् (!strbuf_getline(&पंक्ति, file)) {
				यदि (पंक्ति.buf[0] == '"') {
					strbuf_reset(&unquoted);
					यदि (unquote_c_style(&unquoted, पंक्ति.buf, रिक्त))
						die(_("unable to unquote C-style string '%s'"),
						पंक्ति.buf);

					strbuf_swap(&unquoted, &पंक्ति);
				}

				strbuf_to_cone_pattern(&पंक्ति, pl);
			}

			strbuf_release(&unquoted);
		} अन्यथा {
			प्रति (i = 0; i < argc; i++) {
				strbuf_setlen(&पंक्ति, 0);
				strbuf_addstr(&पंक्ति, argv[i]);
				strbuf_to_cone_pattern(&पंक्ति, pl);
			}
		}
		strbuf_release(&पंक्ति);
	} अन्यथा {
		यदि (file) {
			संरचना strbuf पंक्ति = STRBUF_INIT;

			यावत् (!strbuf_getline(&पंक्ति, file))
				add_pattern(पंक्ति.buf, empty_base, 0, pl, 0);

			strbuf_release(&पंक्ति);
		} अन्यथा {
			प्रति (i = 0; i < argc; i++)
				add_pattern(argv[i], empty_base, 0, pl, 0);
		}
	}
}

गणना modify_type {
	REPLACE,
	ADD,
};

स्थिर शून्य add_patterns_cone_mode(पूर्णाङ्क argc, ध्रुव वर्ण **argv,
				   संरचना pattern_list *pl,
				   पूर्णाङ्क use_stdin)
{
	संरचना strbuf buffer = STRBUF_INIT;
	संरचना pattern_entry *pe;
	संरचना hashmap_iter iter;
	संरचना pattern_list existing;
	संरचना repo_config_मूल्यावली *cfg = repo_config_मूल्यावली(the_repository);
	वर्ण *sparse_filename = get_sparse_checkout_filename();

	add_patterns_from_निवेश(pl, argc, argv,
				use_stdin ? मानक_निवेश : रिक्त);

	स_रखो(&existing, 0, परिमाण(existing));
	existing.use_cone_patterns = cfg->core_sparse_checkout_cone;

	यदि (add_patterns_from_file_to_list(sparse_filename, "", 0,
					   &existing, रिक्त, 0))
		die(_("unable to load existing sparse-checkout patterns"));
	मुक्त(sparse_filename);

	यदि (!existing.use_cone_patterns)
		die(_("existing sparse-checkout patterns do not use cone mode"));

	hashmap_for_each_entry(&existing.recursive_hashmap, &iter, pe, ent) {
		यदि (!hashmap_contains_parent(&pl->recursive_hashmap,
					pe->pattern, &buffer) ||
		    !hashmap_contains_parent(&pl->parent_hashmap,
					pe->pattern, &buffer)) {
			strbuf_reset(&buffer);
			strbuf_addstr(&buffer, pe->pattern);
			जोड़ो_recursive_pattern(pl, &buffer);
		}
	}

	clear_pattern_list(&existing);
	strbuf_release(&buffer);
}

स्थिर शून्य add_patterns_literal(पूर्णाङ्क argc, ध्रुव वर्ण **argv,
				 संरचना pattern_list *pl,
				 पूर्णाङ्क use_stdin)
{
	वर्ण *sparse_filename = get_sparse_checkout_filename();
	यदि (add_patterns_from_file_to_list(sparse_filename, "", 0,
					   pl, रिक्त, 0))
		die(_("unable to load existing sparse-checkout patterns"));
	मुक्त(sparse_filename);
	add_patterns_from_निवेश(pl, argc, argv, use_stdin ? मानक_निवेश : रिक्त);
}

स्थिर पूर्णाङ्क modify_pattern_list(संरचना repository *repo,
			       संरचना strvec *args, पूर्णाङ्क use_stdin,
			       गणना modify_type m)
{
	पूर्णाङ्क result;
	पूर्णाङ्क changed_config = 0;
	संरचना pattern_list *pl = xcalloc(1, परिमाण(*pl));
	संरचना repo_config_मूल्यावली *cfg = repo_config_मूल्यावली(the_repository);

	विकल्प (m) {
	पक्ष ADD:
		यदि (cfg->core_sparse_checkout_cone)
			add_patterns_cone_mode(args->nr, args->v, pl, use_stdin);
		अन्यथा
			add_patterns_literal(args->nr, args->v, pl, use_stdin);
		भङ्ग;

	पक्ष REPLACE:
		add_patterns_from_निवेश(pl, args->nr, args->v,
					use_stdin ? मानक_निवेश : रिक्त);
		भङ्ग;
	}

	यदि (!cfg->apply_sparse_checkout) {
		set_config(repo, MODE_ALL_PATTERNS);
		cfg->apply_sparse_checkout = 1;
		changed_config = 1;
	}

	result = write_patterns_and_update(repo, pl);

	यदि (result && changed_config)
		set_config(repo, MODE_NO_PATTERNS);

	clear_pattern_list(pl);
	मुक्त(pl);
	प्रत्यागम result;
}

स्थिर शून्य sanitize_paths(संरचना repository *repo,
			   संरचना strvec *args,
			   ध्रुव वर्ण *prefix, पूर्णाङ्क skip_checks)
{
	पूर्णाङ्क i;
	संरचना repo_config_मूल्यावली *cfg = repo_config_मूल्यावली(the_repository);

	यदि (!args->nr)
		प्रत्यागम;

	यदि (prefix && *prefix && cfg->core_sparse_checkout_cone) {
		/*
		 * The args are not pathspecs, so unfortunately we
		 * cannot imitate how cmd_add() uses parse_pathspec().
		 */
		पूर्णाङ्क prefix_len = म_माप(prefix);

		प्रति (i = 0; i < args->nr; i++) {
			वर्ण *prefixed_path = prefix_path(the_repository, prefix,
							  prefix_len, args->v[i]);
			strvec_replace(args, i, prefixed_path);
			मुक्त(prefixed_path);
		}
	}

	यदि (skip_checks)
		प्रत्यागम;

	यदि (prefix && *prefix && !cfg->core_sparse_checkout_cone)
		die(_("please run from the toplevel directory in non-cone mode"));

	यदि (cfg->core_sparse_checkout_cone) {
		प्रति (i = 0; i < args->nr; i++) {
			यदि (args->v[i][0] == '/')
				die(_("specify directories rather than patterns (no leading slash)"));
			यदि (args->v[i][0] == '!')
				die(_("specify directories rather than patterns.  If your directory starts with a '!', pass --skip-checks"));
			यदि (strpbrk(args->v[i], "*?[]"))
				die(_("specify directories rather than patterns.  If your directory really has any of '*?[]\\' in it, pass --skip-checks"));
		}
	}

	प्रति (i = 0; i < args->nr; i++) {
		संरचना cache_entry *ce;
		संरचना index_state *index = repo->index;
		पूर्णाङ्क pos = index_name_pos(index, args->v[i], म_माप(args->v[i]));

		यदि (pos < 0)
			अनुवर्त;
		ce = index->cache[pos];
		यदि (S_ISSPARSEDIR(ce->ce_mode))
			अनुवर्त;

		यदि (cfg->core_sparse_checkout_cone)
			die(_("'%s' is not a directory; to treat it as a directory anyway, rerun with --skip-checks"), args->v[i]);
		अन्यथा
			warning(_("pass a leading slash before paths such as '%s' if you want a single file (see NON-CONE PROBLEMS in the git-sparse-checkout manual)."), args->v[i]);
	}
}

स्थिर वर्ण ध्रुव * ध्रुव builtin_sparse_checkout_add_usage[] = {
	N_("git sparse-checkout add [--skip-checks] (--stdin | <patterns>)"),
	रिक्त
};

स्थिर संरचना sparse_checkout_add_opts {
	पूर्णाङ्क skip_checks;
	पूर्णाङ्क use_stdin;
} add_opts;

स्थिर पूर्णाङ्क sparse_checkout_add(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix,
			       संरचना repository *repo)
{
	स्थिर संरचना option builtin_sparse_checkout_add_options[] = {
		OPT_BOOL_F(0, "skip-checks", &add_opts.skip_checks,
			   N_("skip some sanity checks on the given paths that might give false positives"),
			   PARSE_OPT_NONEG),
		OPT_BOOL(0, "stdin", &add_opts.use_stdin,
			 N_("read patterns from standard in")),
		OPT_END(),
	};
	संरचना strvec patterns = STRVEC_INIT;
	पूर्णाङ्क ret;
	संरचना repo_config_मूल्यावली *cfg = repo_config_मूल्यावली(the_repository);

	setup_work_वृक्ष(the_repository);
	यदि (!cfg->apply_sparse_checkout)
		die(_("no sparse-checkout to add to"));

	repo_read_index(repo);

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_add_options,
			     builtin_sparse_checkout_add_usage, 0);

	प्रति (पूर्णाङ्क i = 0; i < argc; i++)
		strvec_धकेलो(&patterns, argv[i]);
	sanitize_paths(repo, &patterns, prefix, add_opts.skip_checks);

	ret = modify_pattern_list(repo, &patterns, add_opts.use_stdin, ADD);

	strvec_clear(&patterns);
	प्रत्यागम ret;
}

स्थिर वर्ण ध्रुव * ध्रुव builtin_sparse_checkout_set_usage[] = {
	N_("git sparse-checkout set [--[no-]cone] [--[no-]sparse-index] [--skip-checks] (--stdin | <patterns>)"),
	रिक्त
};

स्थिर संरचना sparse_checkout_set_opts {
	पूर्णाङ्क cone_mode;
	पूर्णाङ्क sparse_index;
	पूर्णाङ्क skip_checks;
	पूर्णाङ्क use_stdin;
} set_opts;

स्थिर पूर्णाङ्क sparse_checkout_set(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix,
			       संरचना repository *repo)
{
	संरचना repo_config_मूल्यावली *cfg = repo_config_मूल्यावली(the_repository);
	पूर्णाङ्क default_patterns_nr = 2;
	ध्रुव वर्ण *default_patterns[] = {"/*", "!/*/", रिक्त};

	स्थिर संरचना option builtin_sparse_checkout_set_options[] = {
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
	संरचना strvec patterns = STRVEC_INIT;
	पूर्णाङ्क ret;

	setup_work_वृक्ष(the_repository);
	repo_read_index(repo);

	set_opts.cone_mode = -1;
	set_opts.sparse_index = -1;

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_set_options,
			     builtin_sparse_checkout_set_usage, 0);

	यदि (update_modes(repo, &set_opts.cone_mode, &set_opts.sparse_index))
		प्रत्यागम 1;

	/*
	 * Cone mode automatically specifies the toplevel directory.  For
	 * non-cone mode, if nothing is specified, manually select just the
	 * top-level directory (much as 'init' would do).
	 */
	यदि (!cfg->core_sparse_checkout_cone && !set_opts.use_stdin && argc == 0) {
		प्रति (पूर्णाङ्क i = 0; i < default_patterns_nr; i++)
			strvec_धकेलो(&patterns, default_patterns[i]);
	} अन्यथा {
		प्रति (पूर्णाङ्क i = 0; i < argc; i++)
			strvec_धकेलो(&patterns, argv[i]);
		sanitize_paths(repo, &patterns, prefix, set_opts.skip_checks);
	}

	ret = modify_pattern_list(repo, &patterns, set_opts.use_stdin, REPLACE);

	strvec_clear(&patterns);
	प्रत्यागम ret;
}

स्थिर वर्ण ध्रुव * ध्रुव builtin_sparse_checkout_reapply_usage[] = {
	"git sparse-checkout reapply [--[no-]cone] [--[no-]sparse-index]",
	रिक्त
};

स्थिर संरचना sparse_checkout_reapply_opts {
	पूर्णाङ्क cone_mode;
	पूर्णाङ्क sparse_index;
} reapply_opts;

स्थिर पूर्णाङ्क sparse_checkout_reapply(पूर्णाङ्क argc, ध्रुव वर्ण **argv,
				   ध्रुव वर्ण *prefix,
				   संरचना repository *repo)
{
	स्थिर संरचना option builtin_sparse_checkout_reapply_options[] = {
		OPT_BOOL(0, "cone", &reapply_opts.cone_mode,
			 N_("initialize the sparse-checkout in cone mode")),
		OPT_BOOL(0, "sparse-index", &reapply_opts.sparse_index,
			 N_("toggle the use of a sparse index")),
		OPT_END(),
	};
	संरचना repo_config_मूल्यावली *cfg = repo_config_मूल्यावली(the_repository);

	setup_work_वृक्ष(the_repository);
	यदि (!cfg->apply_sparse_checkout)
		die(_("must be in a sparse-checkout to reapply sparsity patterns"));

	reapply_opts.cone_mode = -1;
	reapply_opts.sparse_index = -1;

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_reapply_options,
			     builtin_sparse_checkout_reapply_usage, 0);

	repo_read_index(repo);

	यदि (update_modes(repo, &reapply_opts.cone_mode, &reapply_opts.sparse_index))
		प्रत्यागम 1;

	प्रत्यागम update_working_directory(repo, रिक्त);
}

स्थिर वर्ण ध्रुव * ध्रुव builtin_sparse_checkout_clean_usage[] = {
	"git sparse-checkout clean [-n|--dry-run]",
	रिक्त
};

स्थिर पूर्णाङ्क list_file_iterator(ध्रुव वर्ण *path, ध्रुव शून्य *data)
{
	ध्रुव वर्ण *संदेश = data;

	म_लिखो(संदेश, path);
	प्रत्यागम 0;
}

स्थिर शून्य list_every_file_in_dir(ध्रुव वर्ण *संदेश,
				   ध्रुव वर्ण *directory)
{
	संरचना strbuf path = STRBUF_INIT;

	strbuf_addstr(&path, directory);
	for_each_file_in_dir(&path, list_file_iterator, संदेश);
	strbuf_release(&path);
}

स्थिर ध्रुव वर्ण *संदेश_remove = N_("Removing %s\n");
स्थिर ध्रुव वर्ण *संदेश_would_remove = N_("Would remove %s\n");

स्थिर पूर्णाङ्क sparse_checkout_clean(पूर्णाङ्क argc, ध्रुव वर्ण **argv,
				   ध्रुव वर्ण *prefix,
				   संरचना repository *repo)
{
	संरचना strbuf full_path = STRBUF_INIT;
	ध्रुव वर्ण *संदेश = संदेश_remove;
	माप_प्रकार worktree_len;
	पूर्णाङ्क force = 0, dry_run = 0, verbose = 0;
	पूर्णाङ्क require_force = 1;
	संरचना repo_config_मूल्यावली *cfg = repo_config_मूल्यावली(the_repository);

	संरचना option builtin_sparse_checkout_clean_options[] = {
		OPT__DRY_RUN(&dry_run, N_("dry run")),
		OPT__FORCE(&force, N_("force"), PARSE_OPT_NOCOMPLETE),
		OPT__VERBOSE(&verbose, N_("report each affected file, not just directories")),
		OPT_END(),
	};

	setup_work_वृक्ष(the_repository);
	यदि (!cfg->apply_sparse_checkout)
		die(_("must be in a sparse-checkout to clean directories"));
	यदि (!cfg->core_sparse_checkout_cone)
		die(_("must be in a cone-mode sparse-checkout to clean directories"));

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_clean_options,
			     builtin_sparse_checkout_clean_usage, 0);

	repo_config_get_bool(repo, "clean.requireforce", &require_force);
	यदि (require_force && !force && !dry_run)
		die(_("for safety, refusing to clean without one of --force or --dry-run"));

	यदि (dry_run)
		संदेश = संदेश_would_remove;

	यदि (repo_read_index(repo) < 0)
		die(_("failed to read index"));

	यदि (convert_to_sparse(repo->index, SPARSE_INDEX_MEMORY_ONLY) ||
	    repo->index->sparse_index == INDEX_EXPANDED)
		die(_("failed to convert index to a sparse index; resolve merge conflicts and try again"));

	strbuf_addstr(&full_path, repo->worktree);
	strbuf_addch(&full_path, '/');
	worktree_len = full_path.len;

	प्रति (माप_प्रकार i = 0; i < repo->index->cache_nr; i++) {
		संरचना cache_entry *ce = repo->index->cache[i];
		यदि (!S_ISSPARSEDIR(ce->ce_mode))
			अनुवर्त;
		strbuf_setlen(&full_path, worktree_len);
		strbuf_add(&full_path, ce->name, ce->ce_namelen);

		यदि (!is_directory(full_path.buf))
			अनुवर्त;

		यदि (verbose)
			list_every_file_in_dir(संदेश, ce->name);
		अन्यथा
			म_लिखो(संदेश, ce->name);

		यदि (dry_run <= 0 &&
		    remove_dir_recursively(&full_path, 0))
			warning_errno(_("failed to remove '%s'"), ce->name);
	}

	strbuf_release(&full_path);
	प्रत्यागम 0;
}

स्थिर वर्ण ध्रुव * ध्रुव builtin_sparse_checkout_disable_usage[] = {
	"git sparse-checkout disable",
	रिक्त
};

स्थिर पूर्णाङ्क sparse_checkout_disable(पूर्णाङ्क argc, ध्रुव वर्ण **argv,
				   ध्रुव वर्ण *prefix,
				   संरचना repository *repo)
{
	स्थिर संरचना option builtin_sparse_checkout_disable_options[] = {
		OPT_END(),
	};
	संरचना pattern_list pl;
	संरचना repo_config_मूल्यावली *cfg = repo_config_मूल्यावली(the_repository);

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
	repo_read_index(repo);

	स_रखो(&pl, 0, परिमाण(pl));
	hashmap_init(&pl.recursive_hashmap, pl_hashmap_cmp, रिक्त, 0);
	hashmap_init(&pl.parent_hashmap, pl_hashmap_cmp, रिक्त, 0);
	pl.use_cone_patterns = 0;
	cfg->apply_sparse_checkout = 1;

	add_pattern("/*", empty_base, 0, &pl, 0);

	prepare_repo_settings(the_repository);
	repo->settings.sparse_index = 0;

	यदि (update_working_directory(repo, &pl))
		die(_("error while refreshing working directory"));

	clear_pattern_list(&pl);
	प्रत्यागम set_config(repo, MODE_NO_PATTERNS);
}

स्थिर वर्ण ध्रुव * ध्रुव builtin_sparse_checkout_check_rules_usage[] = {
	N_("git sparse-checkout check-rules [-z] [--skip-checks]"
	   "[--[no-]cone] [--rules-file <file>]"),
	रिक्त
};

स्थिर संरचना sparse_checkout_check_rules_opts {
	पूर्णाङ्क cone_mode;
	पूर्णाङ्क null_termination;
	वर्ण *rules_file;
} check_rules_opts;

स्थिर पूर्णाङ्क check_rules(संरचना repository *repo,
		       संरचना pattern_list *pl,
		       पूर्णाङ्क null_terminated)
{
	संरचना strbuf पंक्ति = STRBUF_INIT;
	संरचना strbuf unquoted = STRBUF_INIT;
	वर्ण *path;
	पूर्णाङ्क पंक्ति_terminator = null_terminated ? 0 : '\n';
	strbuf_getline_fn getline_fn = null_terminated ? strbuf_getline_nul
		: strbuf_getline;
	repo->index->sparse_checkout_patterns = pl;
	यावत् (!getline_fn(&पंक्ति, मानक_निवेश)) {
		path = पंक्ति.buf;
		यदि (!null_terminated && पंक्ति.buf[0] == '"') {
			strbuf_reset(&unquoted);
			यदि (unquote_c_style(&unquoted, पंक्ति.buf, रिक्त))
				die(_("unable to unquote C-style string '%s'"),
					पंक्ति.buf);

			path = unquoted.buf;
		}

		यदि (path_in_sparse_checkout(path, repo->index))
			write_name_quoted(path, मानक_निकास, पंक्ति_terminator);
	}
	strbuf_release(&पंक्ति);
	strbuf_release(&unquoted);

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क sparse_checkout_check_rules(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix,
				       संरचना repository *repo)
{
	स्थिर संरचना option builtin_sparse_checkout_check_rules_options[] = {
		OPT_BOOL('z', रिक्त, &check_rules_opts.null_termination,
			 N_("terminate input and output files by a NUL character")),
		OPT_BOOL(0, "cone", &check_rules_opts.cone_mode,
			 N_("when used with --rules-file interpret patterns as cone mode patterns")),
		OPT_FILENAME(0, "rules-file", &check_rules_opts.rules_file,
			 N_("use patterns in <file> instead of the current ones.")),
		OPT_END(),
	};

	खाता *fp;
	पूर्णाङ्क ret;
	संरचना pattern_list pl = {0};
	संरचना repo_config_मूल्यावली *cfg = repo_config_मूल्यावली(the_repository);
	वर्ण *sparse_filename;
	check_rules_opts.cone_mode = -1;

	argc = parse_options(argc, argv, prefix,
			     builtin_sparse_checkout_check_rules_options,
			     builtin_sparse_checkout_check_rules_usage, 0);

	यदि (check_rules_opts.rules_file && check_rules_opts.cone_mode < 0)
		check_rules_opts.cone_mode = 1;

	update_cone_mode(&check_rules_opts.cone_mode);
	pl.use_cone_patterns = cfg->core_sparse_checkout_cone;
	यदि (check_rules_opts.rules_file) {
		fp = xfopen(check_rules_opts.rules_file, "r");
		add_patterns_from_निवेश(&pl, argc, argv, fp);
		ख_बंद(fp);
	} अन्यथा {
		sparse_filename = get_sparse_checkout_filename();
		यदि (add_patterns_from_file_to_list(sparse_filename, "", 0, &pl,
						   रिक्त, 0))
			die(_("unable to load existing sparse-checkout patterns"));
		मुक्त(sparse_filename);
	}

	ret = check_rules(repo, &pl, check_rules_opts.null_termination);
	clear_pattern_list(&pl);
	मुक्त(check_rules_opts.rules_file);
	प्रत्यागम ret;
}

पूर्णाङ्क cmd_sparse_checkout(पूर्णाङ्क argc,
			ध्रुव वर्ण **argv,
			ध्रुव वर्ण *prefix,
			संरचना repository *repo)
{
	parse_opt_subcommand_fn *fn = रिक्त;
	संरचना option builtin_sparse_checkout_options[] = {
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

	repo_config(the_repository, git_default_config, रिक्त);

	prepare_repo_settings(repo);
	repo->settings.command_requires_full_index = 0;

	प्रत्यागम fn(argc, argv, prefix, repo);
}
