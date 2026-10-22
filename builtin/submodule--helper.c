#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "abspath.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "hex.h"

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "path.h"
#ಅನ್ತರ್ಭಾವ "pathspec.h"
#ಅನ್ತರ್ಭಾವ "preload-index.h"
#ಅನ್ತರ್ಭಾವ "dir.h"
#ಅನ್ತರ್ಭಾವ "read-cache.h"
#ಅನ್ತರ್ಭಾವ "setup.h"
#ಅನ್ತರ್ಭಾವ "sparse-index.h"
#ಅನ್ತರ್ಭಾವ "submodule.h"
#ಅನ್ತರ್ಭಾವ "submodule-config.h"
#ಅನ್ತರ್ಭಾವ "string-list.h"
#ಅನ್ತರ್ಭಾವ "run-command.h"
#ಅನ್ತರ್ಭಾವ "remote.h"
#ಅನ್ತರ್ಭಾವ "refs.h"
#ಅನ್ತರ್ಭಾವ "refspec.h"
#ಅನ್ತರ್ಭಾವ "revision.h"
#ಅನ್ತರ್ಭಾವ "diffcore.h"
#ಅನ್ತರ್ಭಾವ "diff.h"
#ಅನ್ತರ್ಭಾವ "object-file.h"
#ಅನ್ತರ್ಭಾವ "object-name.h"
#ಅನ್ತರ್ಭಾವ "odb.h"
#ಅನ್ತರ್ಭಾವ "odb/source.h"
#ಅನ್ತರ್ಭಾವ "odb/source-files.h"
#ಅನ್ತರ್ಭಾವ "advice.h"
#ಅನ್ತರ್ಭಾವ "branch.h"
#ಅನ್ತರ್ಭಾವ "list-objects-filter-options.h"
#ಅನ್ತರ್ಭಾವ "wildmatch.h"
#ಅನ್ತರ್ಭಾವ "strbuf.h"
#ಅನ್ತರ್ಭಾವ "url.h"

#ವ್ಯಾಖ್ಯಾನಿಸು OPT_QUIET (1 << 0)
#ವ್ಯಾಖ್ಯಾನಿಸು OPT_CACHED (1 << 1)
#ವ್ಯಾಖ್ಯಾನಿಸು OPT_RECURSIVE (1 << 2)
#ವ್ಯಾಖ್ಯಾನಿಸು OPT_FORCE (1 << 3)

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (*each_submodule_fn)(ಸ್ಥಿರಾಂಕ ರಚನೆ cache_ನಮೂದು *list_ಅಂಶ,
				  ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ);

ಸ್ಥಿರ ಅಕ್ಷರ *get_default_remote(ಶೂನ್ಯ)
{
	ಹಿಂತಿರುಗಿ xstrdup(repo_default_remote(the_repository));
}

ಸ್ಥಿರ ಅಕ್ಷರ *resolve_relative_url(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rel_url, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೇಲೆ_ಮಾರ್ಗ, ಪೂರ್ಣಾಂಕ quiet)
{
	ಅಕ್ಷರ *remoteurl, *resolved_url;
	ಅಕ್ಷರ *remote = get_default_remote();
	ರಚನೆ strbuf remotesb = STRBUF_INIT;

	strbuf_addf(&remotesb, "remote.%s.url", remote);
	ಆದರೆ (repo_config_get_string(the_repository, remotesb.buf, &remoteurl)) {
		ಆದರೆ (!quiet)
			warning(_("could not look up configuration '%s'. "
				  "Assuming this repository is its own "
				  "authoritative upstream."),
				remotesb.buf);
		remoteurl = xgetcwd();
	}
	resolved_url = relative_url(remoteurl, rel_url, ಮೇಲೆ_ಮಾರ್ಗ);

	ಬಿಡುಗಡೆ(remote);
	ಬಿಡುಗಡೆ(remoteurl);
	strbuf_release(&remotesb);

	ಹಿಂತಿರುಗಿ resolved_url;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ get_default_remote_submodule(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *module_ಮಾರ್ಗ, ಅಕ್ಷರ **default_remote)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ submodule *sub;
	ರಚನೆ repository subrepo;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *remote_name = ಖಾಲಿ;
	ಅಕ್ಷರ *url = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ ret = 0;

	sub = submodule_from_ಮಾರ್ಗ(the_repository, null_oid(the_hash_algo), module_ಮಾರ್ಗ);
	ಆದರೆ (sub && sub->url) {
		url = xstrdup(sub->url);

		/* Possibly a url relative to parent */
		ಆದರೆ (starts_with_dot_dot_slash(url) ||
		    starts_with_dot_slash(url)) {
			ಅಕ್ಷರ *oldurl = url;

			url = resolve_relative_url(oldurl, ಖಾಲಿ, 1);
			ಬಿಡುಗಡೆ(oldurl);
		}
	}

	ಆದರೆ (repo_submodule_init(&subrepo, the_repository, module_ಮಾರ್ಗ,
				null_oid(the_hash_algo)) < 0) {
		ret = die_ಸಂದೇಶ(_("could not get a repository handle for submodule '%s'"),
				   module_ಮಾರ್ಗ);
		ಹೋಗು ಹೊರಹೋಗು;
	}

	/* Look up by URL first */
	ಆದರೆ (url)
		remote_name = repo_remote_from_url(&subrepo, url);
	ಆದರೆ (!remote_name)
		remote_name = repo_default_remote(&subrepo);

	*default_remote = xstrdup(remote_name);

ಹೊರಹೋಗು:
	repo_clear(&subrepo);
	ಬಿಡುಗಡೆ(url);

	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ module_get_default_remote(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
				     ರಚನೆ repository *repo ಬಳಸದ)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ;
	ಅಕ್ಷರ *resolved_ಮಾರ್ಗ = ಖಾಲಿ;
	ಅಕ್ಷರ *default_remote = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ ಸಂಕೇತ_2;
	ರಚನೆ option options[] = {
		OPT_ಕೊನೆ()
	};
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ usage[] = {
		N_("git submodule--helper get-default-remote <path>"),
		ಖಾಲಿ
	};

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options, usage, 0);
	ಆದರೆ (argc != 1)
		usage_with_options(usage, options);

	ಮಾರ್ಗ = argv[0];
	ಆದರೆ (ಪೂರ್ವಪ್ರತ್ಯಯ && *ಪೂರ್ವಪ್ರತ್ಯಯ && !is_absolute_ಮಾರ್ಗ(ಮಾರ್ಗ)) {
		resolved_ಮಾರ್ಗ = xstrfmt("%s%s", ಪೂರ್ವಪ್ರತ್ಯಯ, ಮಾರ್ಗ);
		ಮಾರ್ಗ = resolved_ಮಾರ್ಗ;
	}

	ಸಂಕೇತ_2 = get_default_remote_submodule(ಮಾರ್ಗ, &default_remote);
	ಆದರೆ (ಸಂಕೇತ_2) {
		ಬಿಡುಗಡೆ(resolved_ಮಾರ್ಗ);
		ಹಿಂತಿರುಗಿ ಸಂಕೇತ_2;
	}

	ಬರೆ("%s\n", default_remote);
	ಬಿಡುಗಡೆ(default_remote);
	ಬಿಡುಗಡೆ(resolved_ಮಾರ್ಗ);
	ಹಿಂತಿರುಗಿ 0;
}

/* the result should be freed by the caller. */
ಸ್ಥಿರ ಅಕ್ಷರ *get_submodule_displaypath(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
				       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *super_ಪೂರ್ವಪ್ರತ್ಯಯ)
{
	ಆದರೆ (ಪೂರ್ವಪ್ರತ್ಯಯ && super_ಪೂರ್ವಪ್ರತ್ಯಯ) {
		BUG("cannot have prefix '%s' and superprefix '%s'",
		    ಪೂರ್ವಪ್ರತ್ಯಯ, super_ಪೂರ್ವಪ್ರತ್ಯಯ);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಪೂರ್ವಪ್ರತ್ಯಯ) {
		ರಚನೆ strbuf sb = STRBUF_INIT;
		ಅಕ್ಷರ *displaypath = xstrdup(relative_ಮಾರ್ಗ(ಮಾರ್ಗ, ಪೂರ್ವಪ್ರತ್ಯಯ, &sb));
		strbuf_release(&sb);
		ಹಿಂತಿರುಗಿ displaypath;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (super_ಪೂರ್ವಪ್ರತ್ಯಯ) {
		ಹಿಂತಿರುಗಿ xstrfmt("%s%s", super_ಪೂರ್ವಪ್ರತ್ಯಯ, ಮಾರ್ಗ);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಹಿಂತಿರುಗಿ xstrdup(ಮಾರ್ಗ);
	}
}

ಸ್ಥಿರ ಅಕ್ಷರ *compute_rev_name(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *sub_ಮಾರ್ಗ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* object_id)
{
	ರಚನೆ strbuf sb = STRBUF_INIT;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ ***d;

	ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *describe_bare[] = { ಖಾಲಿ };

	ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *describe_ಟ್ಯಾಗುಗಳು[] = { "--tags", ಖಾಲಿ };

	ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *describe_ಒಳಗೊಂಡಿದೆ[] = { "--contains", ಖಾಲಿ };

	ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *describe_all_always[] = { "--all", "--always", ಖಾಲಿ };

	ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **describe_argv[] = { describe_bare, describe_ಟ್ಯಾಗುಗಳು,
						describe_ಒಳಗೊಂಡಿದೆ,
						describe_all_always, ಖಾಲಿ };

	ಗಾಗಿ (d = describe_argv; *d; d++) {
		ರಚನೆ ಮಗು_process cp = ಮಗು_PROCESS_INIT;
		prepare_submodule_repo_env(&cp.env);
		cp.dir = sub_ಮಾರ್ಗ;
		cp.git_cmd = 1;
		cp.no_stderr = 1;

		strvec_ತಳ್ಳು(&cp.ವಾದಗಳು, "describe");
		strvec_pushv(&cp.ವಾದಗಳು, *d);
		strvec_ತಳ್ಳು(&cp.ವಾದಗಳು, object_id);

		ಆದರೆ (!capture_command(&cp, &sb, 0)) {
			strbuf_strip_suffix(&sb, "\n");
			ಹಿಂತಿರುಗಿ strbuf_detach(&sb, ಖಾಲಿ);
		}
	}

	strbuf_release(&sb);
	ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

ರಚನೆ module_list {
	ಸ್ಥಿರಾಂಕ ರಚನೆ cache_ನಮೂದು **entries;
	ಪೂರ್ಣಾಂಕ alloc, nr;
};
#ವ್ಯಾಖ್ಯಾನಿಸು MODULE_LIST_INIT { 0 }

ಸ್ಥಿರ ಶೂನ್ಯ module_list_release(ರಚನೆ module_list *ml)
{
	ಬಿಡುಗಡೆ(ml->entries);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ module_list_compute(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
			       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			       ರಚನೆ pathspec *pathspec,
			       ರಚನೆ module_list *list)
{
	ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ = 0;
	ಅಕ್ಷರ *ps_matched = ಖಾಲಿ;

	parse_pathspec(pathspec, 0,
		       PATHSPEC_PREFER_ತುಂಬಿದ,
		       ಪೂರ್ವಪ್ರತ್ಯಯ, argv);

	ಆದರೆ (pathspec->nr)
		ps_matched = xcalloc(pathspec->nr, 1);

	ಆದರೆ (repo_read_ಸೂಚ್ಯಂಕ(the_repository) < 0)
		die(_("index file corrupt"));

	ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < the_repository->ಸೂಚ್ಯಂಕ->cache_nr; i++) {
		ಸ್ಥಿರಾಂಕ ರಚನೆ cache_ನಮೂದು *ce = the_repository->ಸೂಚ್ಯಂಕ->cache[i];

		ಆದರೆ (!ಹೊಂದಿಕೆ_pathspec(the_repository->ಸೂಚ್ಯಂಕ, pathspec, ce->name, ce_namelen(ce),
				    0, ps_matched, 1) ||
		    !S_ISGITLINK(ce->ce_ಬಹುಲಕ))
			ಮುಂದುವರಿ;

		ALLOC_GROW(list->entries, list->nr + 1, list->alloc);
		list->entries[list->nr++] = ce;
		ಆಗ (i + 1 < the_repository->ಸೂಚ್ಯಂಕ->cache_nr &&
		       !strcmp(ce->name, the_repository->ಸೂಚ್ಯಂಕ->cache[i + 1]->name))
			/*
			 * Skip entries with the same name in different stages
			 * to make sure an entry is returned only once.
			 */
			i++;
	}

	ಆದರೆ (ps_matched && report_ಮಾರ್ಗ_error(ps_matched, pathspec))
		ಫಲಿತಾಂಶ = -1;

	ಬಿಡುಗಡೆ(ps_matched);

	ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

ಸ್ಥಿರ ಶೂನ್ಯ module_list_ಸಕ್ರಿಯ(ರಚನೆ module_list *list)
{
	ಪೂರ್ಣಾಂಕ i;
	ರಚನೆ module_list ಸಕ್ರಿಯ_modules = MODULE_LIST_INIT;

	ಗಾಗಿ (i = 0; i < list->nr; i++) {
		ಸ್ಥಿರಾಂಕ ರಚನೆ cache_ನಮೂದು *ce = list->entries[i];

		ಆದರೆ (!is_submodule_ಸಕ್ರಿಯ(the_repository, ce->name))
			ಮುಂದುವರಿ;

		ALLOC_GROW(ಸಕ್ರಿಯ_modules.entries,
			   ಸಕ್ರಿಯ_modules.nr + 1,
			   ಸಕ್ರಿಯ_modules.alloc);
		ಸಕ್ರಿಯ_modules.entries[ಸಕ್ರಿಯ_modules.nr++] = ce;
	}

	module_list_release(list);
	*list = ಸಕ್ರಿಯ_modules;
}

ಸ್ಥಿರ ಅಕ್ಷರ *get_ಮೇಲೆ_ಮಾರ್ಗ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ)
{
	ರಚನೆ strbuf sb = STRBUF_INIT;

	strbuf_addstrings(&sb, "../", ಎಣಿಕೆ_slashes(ಮಾರ್ಗ));

	/*
	 * Check if 'path' ends with slash or not
	 * for having the same output for dir/sub_dir
	 * and dir/sub_dir/
	 */
	ಆದರೆ (!is_dir_sep(ಮಾರ್ಗ[ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ಮಾರ್ಗ) - 1]))
		strbuf_addstr(&sb, "../");

	ಹಿಂತಿರುಗಿ strbuf_detach(&sb, ಖಾಲಿ);
}

ಸ್ಥಿರ ಶೂನ್ಯ for_each_listed_submodule(ಸ್ಥಿರಾಂಕ ರಚನೆ module_list *list,
				      each_submodule_fn fn, ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ)
{
	ಪೂರ್ಣಾಂಕ i;

	ಗಾಗಿ (i = 0; i < list->nr; i++)
		fn(list->entries[i], cb_ದತ್ತಾಂಶ);
}

ರಚನೆ foreach_cb {
	ಪೂರ್ಣಾಂಕ argc;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *super_ಪೂರ್ವಪ್ರತ್ಯಯ;
	ಪೂರ್ಣಾಂಕ quiet;
	ಪೂರ್ಣಾಂಕ recursive;
};
#ವ್ಯಾಖ್ಯಾನಿಸು FOREACH_CB_INIT { 0 }

ಸ್ಥಿರ ಶೂನ್ಯ runcommand_ಒಳ_submodule_cb(ಸ್ಥಿರಾಂಕ ರಚನೆ cache_ನಮೂದು *list_ಅಂಶ,
				       ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ)
{
	ರಚನೆ foreach_cb *info = cb_ದತ್ತಾಂಶ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ = list_ಅಂಶ->name;
	ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *ce_oid = &list_ಅಂಶ->oid;
	ಸ್ಥಿರಾಂಕ ರಚನೆ submodule *sub;
	ರಚನೆ ಮಗು_process cp = ಮಗು_PROCESS_INIT;
	ಅಕ್ಷರ *displaypath;

	ಆದರೆ (validate_submodule_ಮಾರ್ಗ(ಮಾರ್ಗ) < 0)
		die(ಖಾಲಿ);

	displaypath = get_submodule_displaypath(ಮಾರ್ಗ, info->ಪೂರ್ವಪ್ರತ್ಯಯ,
						info->super_ಪೂರ್ವಪ್ರತ್ಯಯ);

	sub = submodule_from_ಮಾರ್ಗ(the_repository, null_oid(the_hash_algo), ಮಾರ್ಗ);

	ಆದರೆ (!sub)
		die(_("No url found for submodule path '%s' in .gitmodules"),
			displaypath);

	ಆದರೆ (!is_submodule_populated_gently(ಮಾರ್ಗ, ಖಾಲಿ))
		ಹೋಗು cleanup;

	prepare_submodule_repo_env(&cp.env);

	/*
	 * For the purpose of executing <command> in the submodule,
	 * separate shell is used for the purpose of running the
	 * child process.
	 */
	cp.use_shell = 1;
	cp.dir = ಮಾರ್ಗ;

	/*
	 * NEEDSWORK: the command currently has access to the variables $name,
	 * $sm_path, $displaypath, $sha1 and $toplevel only when the command
	 * contains a single argument. This is done for maintaining a faithful
	 * translation from shell script.
	 */
	ಆದರೆ (info->argc == 1) {
		ಅಕ್ಷರ *toplevel = xgetcwd();
		ರಚನೆ strbuf sb = STRBUF_INIT;

		strvec_pushf(&cp.env, "name=%s", sub->name);
		strvec_pushf(&cp.env, "sm_path=%s", ಮಾರ್ಗ);
		strvec_pushf(&cp.env, "displaypath=%s", displaypath);
		strvec_pushf(&cp.env, "sha1=%s",
			     oid_ಗಮ್ಯ_hex(ce_oid));
		strvec_pushf(&cp.env, "toplevel=%s", toplevel);

		/*
		 * Since the path variable was accessible from the script
		 * before porting, it is also made available after porting.
		 * The environment variable "PATH" has a very special purpose
		 * on windows. And since environment variables are
		 * case-insensitive in windows, it interferes with the
		 * existing PATH variable. Hence, to avoid that, we expose
		 * path via the args strvec and not via env.
		 */
		sq_quote_buf(&sb, ಮಾರ್ಗ);
		strvec_pushf(&cp.ವಾದಗಳು, "path=%s; %s",
			     sb.buf, info->argv[0]);
		strbuf_release(&sb);
		ಬಿಡುಗಡೆ(toplevel);
	} ಇಲ್ಲದಿದ್ದರೆ {
		strvec_pushv(&cp.ವಾದಗಳು, info->argv);
	}

	ಆದರೆ (!info->quiet)
		ಬರೆ(_("Entering '%s'\n"), displaypath);

	ಆದರೆ (info->argv[0]) {
		ಆದರೆ (ಸತತ_command(&cp))
			die(_("run_command returned non-zero status for %s\n."),
			    displaypath);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಮಗು_process_clear(&cp);
	}

	ಆದರೆ (info->recursive) {
		ರಚನೆ ಮಗು_process cpr = ಮಗು_PROCESS_INIT;

		cpr.git_cmd = 1;
		cpr.dir = ಮಾರ್ಗ;
		prepare_submodule_repo_env(&cpr.env);

		strvec_pushl(&cpr.ವಾದಗಳು, "submodule--helper", "foreach", "--recursive",
			     ಖಾಲಿ);
		strvec_pushf(&cpr.ವಾದಗಳು, "--super-prefix=%s/", displaypath);

		ಆದರೆ (info->quiet)
			strvec_ತಳ್ಳು(&cpr.ವಾದಗಳು, "--quiet");

		strvec_ತಳ್ಳು(&cpr.ವಾದಗಳು, "--");
		strvec_pushv(&cpr.ವಾದಗಳು, info->argv);

		ಆದರೆ (ಸತತ_command(&cpr))
			die(_("run_command returned non-zero status while "
				"recursing in the nested submodules of %s\n."),
				displaypath);
	}

cleanup:
	ಬಿಡುಗಡೆ(displaypath);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ module_foreach(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			  ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ foreach_cb info = FOREACH_CB_INIT;
	ರಚನೆ pathspec pathspec = { 0 };
	ರಚನೆ module_list list = MODULE_LIST_INIT;
	ರಚನೆ option module_foreach_options[] = {
		OPT_SUPER_ಪೂರ್ವಪ್ರತ್ಯಯ(&info.super_ಪೂರ್ವಪ್ರತ್ಯಯ),
		OPT__QUIET(&info.quiet, N_("suppress output of entering each submodule command")),
		OPT_BOOL(0, "recursive", &info.recursive,
			 N_("recurse into nested submodules")),
		OPT_ಕೊನೆ()
	};
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ git_submodule_helper_usage[] = {
		N_("git submodule foreach [--quiet] [--recursive] [--] <command>"),
		ಖಾಲಿ
	};
	ಪೂರ್ಣಾಂಕ ret = 1;

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, module_foreach_options,
			     git_submodule_helper_usage, 0);

	ಆದರೆ (module_list_compute(ಖಾಲಿ, ಪೂರ್ವಪ್ರತ್ಯಯ, &pathspec, &list) < 0)
		ಹೋಗು cleanup;

	info.argc = argc;
	info.argv = argv;
	info.ಪೂರ್ವಪ್ರತ್ಯಯ = ಪೂರ್ವಪ್ರತ್ಯಯ;

	for_each_listed_submodule(&list, runcommand_ಒಳ_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	ಹಿಂತಿರುಗಿ ret;
}

ರಚನೆ init_cb {
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *super_ಪೂರ್ವಪ್ರತ್ಯಯ;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags;
};
#ವ್ಯಾಖ್ಯಾನಿಸು INIT_CB_INIT { 0 }

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ validate_and_ಗಣ_submodule_gitdir(ರಚನೆ strbuf *gitdir_ಮಾರ್ಗ,
					     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *submodule_name)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ;
	ಅಕ್ಷರ *ಕೀಲಿ;

	ಆದರೆ (validate_submodule_git_dir(gitdir_ಮಾರ್ಗ->buf, submodule_name))
		ಹಿಂತಿರುಗಿ -1;

	ಕೀಲಿ = xstrfmt("submodule.%s.gitdir", submodule_name);

	/* Nothing to do if the config already exists. */
	ಆದರೆ (!repo_config_get_string_tmp(the_repository, ಕೀಲಿ, &ಮೌಲ್ಯ)) {
		ಬಿಡುಗಡೆ(ಕೀಲಿ);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (repo_config_ಗಣ_gently(the_repository, ಕೀಲಿ, gitdir_ಮಾರ್ಗ->buf)) {
		ಬಿಡುಗಡೆ(ಕೀಲಿ);
		ಹಿಂತಿರುಗಿ -1;
	}

	ಬಿಡುಗಡೆ(ಕೀಲಿ);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ create_default_gitdir_config(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *submodule_name)
{
	ರಚನೆ strbuf gitdir_ಮಾರ್ಗ = STRBUF_INIT;
	ರಚನೆ git_hash_ctx ctx;
	ಅಕ್ಷರ hex_name_hash[GIT_MAX_HEXSZ + 1], ಶೀರ್ಷಿಕೆ[128];
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ raw_name_hash[GIT_MAX_RAWSZ];
	ಪೂರ್ಣಾಂಕ ಶೀರ್ಷಿಕೆ_len;

	/* Case 1: try the plain module name */
	repo_git_ಮಾರ್ಗ_append(the_repository, &gitdir_ಮಾರ್ಗ, "modules/%s", submodule_name);
	ಆದರೆ (!validate_and_ಗಣ_submodule_gitdir(&gitdir_ಮಾರ್ಗ, submodule_name)) {
		strbuf_release(&gitdir_ಮಾರ್ಗ);
		ಹಿಂತಿರುಗಿ;
	}

	/* Case 2.1: Try URI-safe (RFC3986) encoding first, this fixes nested gitdirs */
	strbuf_reset(&gitdir_ಮಾರ್ಗ);
	repo_git_ಮಾರ್ಗ_append(the_repository, &gitdir_ಮಾರ್ಗ, "modules/");
	strbuf_addstr_urlencode(&gitdir_ಮಾರ್ಗ, submodule_name, is_rfc3986_unreserved);
	ಆದರೆ (!validate_and_ಗಣ_submodule_gitdir(&gitdir_ಮಾರ್ಗ, submodule_name)) {
		strbuf_release(&gitdir_ಮಾರ್ಗ);
		ಹಿಂತಿರುಗಿ;
	}

	/* Case 2.2: Try extended uppercase URI (RFC3986) encoding, to fix case-folding */
	strbuf_reset(&gitdir_ಮಾರ್ಗ);
	repo_git_ಮಾರ್ಗ_append(the_repository, &gitdir_ಮಾರ್ಗ, "modules/");
	strbuf_addstr_urlencode(&gitdir_ಮಾರ್ಗ, submodule_name, is_casefolding_rfc3986_unreserved);
	ಆದರೆ (!validate_and_ಗಣ_submodule_gitdir(&gitdir_ಮಾರ್ಗ, submodule_name))
		ಹಿಂತಿರುಗಿ;

	/* Case 2.3: Try some derived gitdir names, see if one sticks */
	ಗಾಗಿ (ಅಕ್ಷರ c = '0'; c <= '9'; c++) {
		strbuf_reset(&gitdir_ಮಾರ್ಗ);
		repo_git_ಮಾರ್ಗ_append(the_repository, &gitdir_ಮಾರ್ಗ, "modules/");
		strbuf_addstr_urlencode(&gitdir_ಮಾರ್ಗ, submodule_name, is_rfc3986_unreserved);
		strbuf_addch(&gitdir_ಮಾರ್ಗ, c);
		ಆದರೆ (!validate_and_ಗಣ_submodule_gitdir(&gitdir_ಮಾರ್ಗ, submodule_name))
			ಹಿಂತಿರುಗಿ;

		strbuf_reset(&gitdir_ಮಾರ್ಗ);
		repo_git_ಮಾರ್ಗ_append(the_repository, &gitdir_ಮಾರ್ಗ, "modules/");
		strbuf_addstr_urlencode(&gitdir_ಮಾರ್ಗ, submodule_name, is_casefolding_rfc3986_unreserved);
		strbuf_addch(&gitdir_ಮಾರ್ಗ, c);
		ಆದರೆ (!validate_and_ಗಣ_submodule_gitdir(&gitdir_ಮಾರ್ಗ, submodule_name))
			ಹಿಂತಿರುಗಿ;
	}

	/* Case 2.4: If all the above failed, try a hash of the name as a last resort */
	ಶೀರ್ಷಿಕೆ_len = snprintf(ಶೀರ್ಷಿಕೆ, ಗಾತ್ರ(ಶೀರ್ಷಿಕೆ),
			      "blob %"PRIuMAX, (uintmax_t)ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(submodule_name));
	git_hash_init(&ctx, the_hash_algo);
	git_hash_update(&ctx, ಶೀರ್ಷಿಕೆ, ಶೀರ್ಷಿಕೆ_len);
	git_hash_update(&ctx, "\0", 1);
	git_hash_update(&ctx, submodule_name, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(submodule_name));
	git_hash_final(raw_name_hash, &ctx);
	hash_ಗಮ್ಯ_hex_algop_r(hex_name_hash, raw_name_hash, the_hash_algo);
	strbuf_reset(&gitdir_ಮಾರ್ಗ);
	repo_git_ಮಾರ್ಗ_append(the_repository, &gitdir_ಮಾರ್ಗ, "modules/%s", hex_name_hash);
	ಆದರೆ (!validate_and_ಗಣ_submodule_gitdir(&gitdir_ಮಾರ್ಗ, submodule_name)) {
		strbuf_release(&gitdir_ಮಾರ್ಗ);
		ಹಿಂತಿರುಗಿ;
	}

	/* Case 3: nothing worked, error out */
	die(_("failed to set a valid default config for 'submodule.%s.gitdir'. "
	      "Please ensure it is set, for example by running something like: "
	      "'git config submodule.%s.gitdir .git/modules/%s'"),
	    submodule_name, submodule_name, submodule_name);
}

ಸ್ಥಿರ ಶೂನ್ಯ init_submodule(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *super_ಪೂರ್ವಪ್ರತ್ಯಯ,
			   ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ submodule *sub;
	ರಚನೆ strbuf sb = STRBUF_INIT;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *upd;
	ಅಕ್ಷರ *url = ಖಾಲಿ, *displaypath;

	displaypath = get_submodule_displaypath(ಮಾರ್ಗ, ಪೂರ್ವಪ್ರತ್ಯಯ, super_ಪೂರ್ವಪ್ರತ್ಯಯ);

	sub = submodule_from_ಮಾರ್ಗ(the_repository, null_oid(the_hash_algo), ಮಾರ್ಗ);

	ಆದರೆ (!sub)
		die(_("No url found for submodule path '%s' in .gitmodules"),
			displaypath);

	/*
	 * NEEDSWORK: In a multi-working-tree world, this needs to be
	 * set in the per-worktree config.
	 *
	 * Set active flag for the submodule being initialized
	 */
	ಆದರೆ (!is_submodule_ಸಕ್ರಿಯ(the_repository, ಮಾರ್ಗ)) {
		strbuf_addf(&sb, "submodule.%s.active", sub->name);
		repo_config_ಗಣ_gently(the_repository, sb.buf, "true");
		strbuf_reset(&sb);
	}

	/*
	 * Copy url setting when it is not set yet.
	 * To look up the url in .git/config, we must not fall back to
	 * .gitmodules, so look it up directly.
	 */
	strbuf_addf(&sb, "submodule.%s.url", sub->name);
	ಆದರೆ (repo_config_get_string(the_repository, sb.buf, &url)) {
		ಆದರೆ (!sub->url)
			die(_("No url found for submodule path '%s' in .gitmodules"),
				displaypath);

		url = xstrdup(sub->url);

		/* Possibly a url relative to parent */
		ಆದರೆ (starts_with_dot_dot_slash(url) ||
		    starts_with_dot_slash(url)) {
			ಅಕ್ಷರ *oldurl = url;

			url = resolve_relative_url(oldurl, ಖಾಲಿ, 0);
			ಬಿಡುಗಡೆ(oldurl);
		}

		ಆದರೆ (repo_config_ಗಣ_gently(the_repository, sb.buf, url))
			die(_("Failed to register url for submodule path '%s'"),
			    displaypath);
		ಆದರೆ (!(flags & OPT_QUIET))
			fprintf(stderr,
				_("Submodule '%s' (%s) registered for path '%s'\n"),
				sub->name, url, displaypath);
	}
	strbuf_reset(&sb);

	/* Copy "update" setting when it is not set yet */
	strbuf_addf(&sb, "submodule.%s.update", sub->name);
	ಆದರೆ (repo_config_get_string_tmp(the_repository, sb.buf, &upd) &&
	    sub->update_strategy.type != SM_UPDATE_UNSPECIFIED) {
		ಆದರೆ (sub->update_strategy.type == SM_UPDATE_COMMAND) {
			fprintf(stderr, _("warning: command update mode suggested for submodule '%s'\n"),
				sub->name);
			upd = "none";
		} ಇಲ್ಲದಿದ್ದರೆ {
			upd = submodule_update_type_ಗಮ್ಯ_string(sub->update_strategy.type);
		}

		ಆದರೆ (repo_config_ಗಣ_gently(the_repository, sb.buf, upd))
			die(_("Failed to register update mode for submodule path '%s'"), displaypath);
	}

	ಆದರೆ (the_repository->repository_format_submodule_ಮಾರ್ಗ_cfg)
		create_default_gitdir_config(sub->name);

	strbuf_release(&sb);
	ಬಿಡುಗಡೆ(displaypath);
	ಬಿಡುಗಡೆ(url);
}

ಸ್ಥಿರ ಶೂನ್ಯ init_submodule_cb(ಸ್ಥಿರಾಂಕ ರಚನೆ cache_ನಮೂದು *list_ಅಂಶ, ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ)
{
	ರಚನೆ init_cb *info = cb_ದತ್ತಾಂಶ;

	init_submodule(list_ಅಂಶ->name, info->ಪೂರ್ವಪ್ರತ್ಯಯ, info->super_ಪೂರ್ವಪ್ರತ್ಯಯ,
		       info->flags);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ module_init(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		       ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ init_cb info = INIT_CB_INIT;
	ರಚನೆ pathspec pathspec = { 0 };
	ರಚನೆ module_list list = MODULE_LIST_INIT;
	ಪೂರ್ಣಾಂಕ quiet = 0;
	ರಚನೆ option module_init_options[] = {
		OPT__QUIET(&quiet, N_("suppress output for initializing a submodule")),
		OPT_ಕೊನೆ()
	};
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ git_submodule_helper_usage[] = {
		N_("git submodule init [<options>] [<path>]"),
		ಖಾಲಿ
	};
	ಪೂರ್ಣಾಂಕ ret = 1;

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, module_init_options,
			     git_submodule_helper_usage, 0);

	ಆದರೆ (module_list_compute(argv, ಪೂರ್ವಪ್ರತ್ಯಯ, &pathspec, &list) < 0)
		ಹೋಗು cleanup;

	/*
	 * If there are no path args and submodule.active is set then,
	 * by default, only initialize 'active' modules.
	 */
	ಆದರೆ (!argc && !repo_config_get(the_repository, "submodule.active"))
		module_list_ಸಕ್ರಿಯ(&list);

	info.ಪೂರ್ವಪ್ರತ್ಯಯ = ಪೂರ್ವಪ್ರತ್ಯಯ;
	ಆದರೆ (quiet)
		info.flags |= OPT_QUIET;

	for_each_listed_submodule(&list, init_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	ಹಿಂತಿರುಗಿ ret;
}

ರಚನೆ ಸ್ಥಿತಿ_cb {
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *super_ಪೂರ್ವಪ್ರತ್ಯಯ;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags;
};
#ವ್ಯಾಖ್ಯಾನಿಸು ಸ್ಥಿತಿ_CB_INIT { 0 }

ಸ್ಥಿರ ಶೂನ್ಯ print_ಸ್ಥಿತಿ(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags, ಅಕ್ಷರ state, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ,
			 ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *displaypath)
{
	ಆದರೆ (flags & OPT_QUIET)
		ಹಿಂತಿರುಗಿ;

	ಬರೆ("%c%s %s", state, oid_ಗಮ್ಯ_hex(oid), displaypath);

	ಆದರೆ (state == ' ' || state == '+') {
		ಅಕ್ಷರ *name = compute_rev_name(ಮಾರ್ಗ, oid_ಗಮ್ಯ_hex(oid));

		ಆದರೆ (name)
			ಬರೆ(" (%s)", name);
		ಬಿಡುಗಡೆ(name);
	}

	ಬರೆ("\n");
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ handle_submodule_ತಲೆ_ref(ಸ್ಥಿರಾಂಕ ರಚನೆ reference *ref, ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ)
{
	ರಚನೆ object_id *output = cb_ದತ್ತಾಂಶ;

	ಆದರೆ (ref->oid)
		oidcpy(output, ref->oid);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ ಸ್ಥಿತಿ_submodule(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *ce_oid,
			     ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ce_flags, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *super_ಪೂರ್ವಪ್ರತ್ಯಯ, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags)
{
	ಅಕ್ಷರ *displaypath;
	ರಚನೆ strvec diff_files_ವಾದಗಳು = STRVEC_INIT;
	ರಚನೆ rev_info rev = REV_INFO_INIT;
	ರಚನೆ strbuf buf = STRBUF_INIT;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *git_dir;

	ಆದರೆ (validate_submodule_ಮಾರ್ಗ(ಮಾರ್ಗ) < 0)
		die(ಖಾಲಿ);

	ಆದರೆ (!submodule_from_ಮಾರ್ಗ(the_repository, null_oid(the_hash_algo), ಮಾರ್ಗ))
		die(_("no submodule mapping found in .gitmodules for path '%s'"),
		      ಮಾರ್ಗ);

	displaypath = get_submodule_displaypath(ಮಾರ್ಗ, ಪೂರ್ವಪ್ರತ್ಯಯ, super_ಪೂರ್ವಪ್ರತ್ಯಯ);

	ಆದರೆ ((CE_STAGEMASK & ce_flags) >> CE_STAGESHIFT) {
		print_ಸ್ಥಿತಿ(flags, 'U', ಮಾರ್ಗ, null_oid(the_hash_algo), displaypath);
		ಹೋಗು cleanup;
	}

	strbuf_addf(&buf, "%s/.git", ಮಾರ್ಗ);
	git_dir = read_gitfile(buf.buf);
	ಆದರೆ (!git_dir)
		git_dir = buf.buf;

	ಆದರೆ (!is_submodule_ಸಕ್ರಿಯ(the_repository, ಮಾರ್ಗ) ||
	    !is_git_directory(git_dir)) {
		print_ಸ್ಥಿತಿ(flags, '-', ಮಾರ್ಗ, ce_oid, displaypath);
		strbuf_release(&buf);
		ಹೋಗು cleanup;
	}
	strbuf_release(&buf);

	strvec_pushl(&diff_files_ವಾದಗಳು, "diff-files",
		     "--ignore-submodules=dirty", "--quiet", "--",
		     ಮಾರ್ಗ, ಖಾಲಿ);

	repo_config(the_repository, git_diff_basic_config, ಖಾಲಿ);

	repo_init_revisions(the_repository, &rev, ಖಾಲಿ);
	rev.abbrev = 0;
	ಸಿದ್ಧತೆ_revisions_from_strvec(&diff_files_ವಾದಗಳು, &rev, ಖಾಲಿ);
	ಸತತ_diff_files(&rev, 0);

	ಆದರೆ (!diff_ಫಲಿತಾಂಶ_ಸಂಕೇತ(&rev)) {
		print_ಸ್ಥಿತಿ(flags, ' ', ಮಾರ್ಗ, ce_oid,
			     displaypath);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!(flags & OPT_CACHED)) {
		ರಚನೆ object_id oid;
		ರಚನೆ ref_store *refs = repo_get_submodule_ref_store(the_repository,
								      ಮಾರ್ಗ);

		ಆದರೆ (!refs) {
			print_ಸ್ಥಿತಿ(flags, '-', ಮಾರ್ಗ, ce_oid, displaypath);
			ಹೋಗು cleanup;
		}
		ಆದರೆ (refs_ತಲೆ_ref(refs, handle_submodule_ತಲೆ_ref, &oid))
			die(_("could not resolve HEAD ref inside the "
			      "submodule '%s'"), ಮಾರ್ಗ);

		print_ಸ್ಥಿತಿ(flags, '+', ಮಾರ್ಗ, &oid, displaypath);
	} ಇಲ್ಲದಿದ್ದರೆ {
		print_ಸ್ಥಿತಿ(flags, '+', ಮಾರ್ಗ, ce_oid, displaypath);
	}

	ಆದರೆ (flags & OPT_RECURSIVE) {
		ರಚನೆ ಮಗು_process cpr = ಮಗು_PROCESS_INIT;
		ಪೂರ್ಣಾಂಕ res;

		cpr.git_cmd = 1;
		cpr.dir = ಮಾರ್ಗ;
		prepare_submodule_repo_env(&cpr.env);

		strvec_pushl(&cpr.ವಾದಗಳು, "submodule--helper", "status",
			     "--recursive", ಖಾಲಿ);
		strvec_pushf(&cpr.ವಾದಗಳು, "--super-prefix=%s/", displaypath);

		ಆದರೆ (flags & OPT_CACHED)
			strvec_ತಳ್ಳು(&cpr.ವಾದಗಳು, "--cached");

		ಆದರೆ (flags & OPT_QUIET)
			strvec_ತಳ್ಳು(&cpr.ವಾದಗಳು, "--quiet");

		res = ಸತತ_command(&cpr);
		ಆದರೆ (res == SIGPIPE + 128)
			ಎತ್ತು(SIGPIPE);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (res)
			die(_("failed to recurse into submodule '%s'"), ಮಾರ್ಗ);
	}

cleanup:
	strvec_clear(&diff_files_ವಾದಗಳು);
	ಬಿಡುಗಡೆ(displaypath);
	release_revisions(&rev);
}

ಸ್ಥಿರ ಶೂನ್ಯ ಸ್ಥಿತಿ_submodule_cb(ಸ್ಥಿರಾಂಕ ರಚನೆ cache_ನಮೂದು *list_ಅಂಶ,
				ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ)
{
	ರಚನೆ ಸ್ಥಿತಿ_cb *info = cb_ದತ್ತಾಂಶ;

	ಸ್ಥಿತಿ_submodule(list_ಅಂಶ->name, &list_ಅಂಶ->oid, list_ಅಂಶ->ce_flags,
			 info->ಪೂರ್ವಪ್ರತ್ಯಯ, info->super_ಪೂರ್ವಪ್ರತ್ಯಯ, info->flags);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ module_ಸ್ಥಿತಿ(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			 ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ ಸ್ಥಿತಿ_cb info = ಸ್ಥಿತಿ_CB_INIT;
	ರಚನೆ pathspec pathspec = { 0 };
	ರಚನೆ module_list list = MODULE_LIST_INIT;
	ಪೂರ್ಣಾಂಕ quiet = 0;
	ರಚನೆ option module_ಸ್ಥಿತಿ_options[] = {
		OPT_SUPER_ಪೂರ್ವಪ್ರತ್ಯಯ(&info.super_ಪೂರ್ವಪ್ರತ್ಯಯ),
		OPT__QUIET(&quiet, N_("suppress submodule status output")),
		OPT_ಬಿಟ್(0, "cached", &info.flags, N_("use commit stored in the index instead of the one stored in the submodule HEAD"), OPT_CACHED),
		OPT_ಬಿಟ್(0, "recursive", &info.flags, N_("recurse into nested submodules"), OPT_RECURSIVE),
		OPT_ಕೊನೆ()
	};
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ git_submodule_helper_usage[] = {
		N_("git submodule status [--quiet] [--cached] [--recursive] [<path>...]"),
		ಖಾಲಿ
	};
	ಪೂರ್ಣಾಂಕ ret = 1;

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, module_ಸ್ಥಿತಿ_options,
			     git_submodule_helper_usage, 0);

	ಆದರೆ (module_list_compute(argv, ಪೂರ್ವಪ್ರತ್ಯಯ, &pathspec, &list) < 0)
		ಹೋಗು cleanup;

	info.ಪೂರ್ವಪ್ರತ್ಯಯ = ಪೂರ್ವಪ್ರತ್ಯಯ;
	ಆದರೆ (quiet)
		info.flags |= OPT_QUIET;

	for_each_listed_submodule(&list, ಸ್ಥಿತಿ_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	ಹಿಂತಿರುಗಿ ret;
}

ರಚನೆ module_cb {
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ mod_src;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ mod_dst;
	ರಚನೆ object_id oid_src;
	ರಚನೆ object_id oid_dst;
	ಅಕ್ಷರ ಸ್ಥಿತಿ_2;
	ಅಕ್ಷರ *sm_ಮಾರ್ಗ;
};
#ವ್ಯಾಖ್ಯಾನಿಸು MODULE_CB_INIT { 0 }

ಸ್ಥಿರ ಶೂನ್ಯ module_cb_release(ರಚನೆ module_cb *mcb)
{
	ಬಿಡುಗಡೆ(mcb->sm_ಮಾರ್ಗ);
}

ರಚನೆ module_cb_list {
	ರಚನೆ module_cb **entries;
	ಪೂರ್ಣಾಂಕ alloc, nr;
};
#ವ್ಯಾಖ್ಯಾನಿಸು MODULE_CB_LIST_INIT { 0 }

ಸ್ಥಿರ ಶೂನ್ಯ module_cb_list_release(ರಚನೆ module_cb_list *mcbl)
{
	ಪೂರ್ಣಾಂಕ i;

	ಗಾಗಿ (i = 0; i < mcbl->nr; i++) {
		ರಚನೆ module_cb *mcb = mcbl->entries[i];

		module_cb_release(mcb);
		ಬಿಡುಗಡೆ(mcb);
	}
	ಬಿಡುಗಡೆ(mcbl->entries);
}

ರಚನೆ summary_cb {
	ಪೂರ್ಣಾಂಕ argc;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *super_ಪೂರ್ವಪ್ರತ್ಯಯ;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cached: 1;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ for_ಸ್ಥಿತಿ: 1;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ files: 1;
	ಪೂರ್ಣಾಂಕ summary_ಎಲ್ಲೆ;
};
#ವ್ಯಾಖ್ಯಾನಿಸು SUMMARY_CB_INIT { 0 }

ಎಣಿಕೆ diff_cmd {
	DIFF_ಸೂಚ್ಯಂಕ,
	DIFF_FILES
};

ಸ್ಥಿರ ಅಕ್ಷರ *verify_submodule_committish(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *sm_ಮಾರ್ಗ,
					 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *committish)
{
	ರಚನೆ ಮಗು_process cp_rev_parse = ಮಗು_PROCESS_INIT;
	ರಚನೆ strbuf ಫಲಿತಾಂಶ = STRBUF_INIT;

	cp_rev_parse.git_cmd = 1;
	cp_rev_parse.dir = sm_ಮಾರ್ಗ;
	prepare_submodule_repo_env(&cp_rev_parse.env);
	strvec_pushl(&cp_rev_parse.ವಾದಗಳು, "rev-parse", "-q", "--short", ಖಾಲಿ);
	strvec_pushf(&cp_rev_parse.ವಾದಗಳು, "%s^0", committish);
	strvec_ತಳ್ಳು(&cp_rev_parse.ವಾದಗಳು, "--");

	ಆದರೆ (capture_command(&cp_rev_parse, &ಫಲಿತಾಂಶ, 0))
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;

	strbuf_trim_trailing_newline(&ಫಲಿತಾಂಶ);
	ಹಿಂತಿರುಗಿ strbuf_detach(&ಫಲಿತಾಂಶ, ಖಾಲಿ);
}

ಸ್ಥಿರ ಶೂನ್ಯ print_submodule_summary(ರಚನೆ summary_cb *info, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *errmsg,
				    ಪೂರ್ಣಾಂಕ ಒಟ್ಟು_commits, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *displaypath,
				    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *src_abbrev, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *dst_abbrev,
				    ರಚನೆ module_cb *p)
{
	ಆದರೆ (p->ಸ್ಥಿತಿ_2 == 'T') {
		ಆದರೆ (S_ISGITLINK(p->mod_dst))
			ಬರೆ(_("* %s %s(blob)->%s(submodule)"),
				 displaypath, src_abbrev, dst_abbrev);
		ಇಲ್ಲದಿದ್ದರೆ
			ಬರೆ(_("* %s %s(submodule)->%s(blob)"),
				 displaypath, src_abbrev, dst_abbrev);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಬರೆ("* %s %s...%s",
			displaypath, src_abbrev, dst_abbrev);
	}

	ಆದರೆ (ಒಟ್ಟು_commits < 0)
		ಬರೆ(":\n");
	ಇಲ್ಲದಿದ್ದರೆ
		ಬರೆ(" (%d):\n", ಒಟ್ಟು_commits);

	ಆದರೆ (errmsg) {
		ಬರೆ(_("%s"), errmsg);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಒಟ್ಟು_commits > 0) {
		ರಚನೆ ಮಗು_process cp_log = ಮಗು_PROCESS_INIT;

		cp_log.git_cmd = 1;
		cp_log.dir = p->sm_ಮಾರ್ಗ;
		prepare_submodule_repo_env(&cp_log.env);
		strvec_pushl(&cp_log.ವಾದಗಳು, "log", ಖಾಲಿ);

		ಆದರೆ (S_ISGITLINK(p->mod_src) && S_ISGITLINK(p->mod_dst)) {
			ಆದರೆ (info->summary_ಎಲ್ಲೆ > 0)
				strvec_pushf(&cp_log.ವಾದಗಳು, "-%d",
					     info->summary_ಎಲ್ಲೆ);

			strvec_pushl(&cp_log.ವಾದಗಳು, "--pretty=  %m %s",
				     "--first-parent", ಖಾಲಿ);
			strvec_pushf(&cp_log.ವಾದಗಳು, "%s...%s",
				     src_abbrev, dst_abbrev);
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (S_ISGITLINK(p->mod_dst)) {
			strvec_pushl(&cp_log.ವಾದಗಳು, "--pretty=  > %s",
				     "-1", dst_abbrev, ಖಾಲಿ);
		} ಇಲ್ಲದಿದ್ದರೆ {
			strvec_pushl(&cp_log.ವಾದಗಳು, "--pretty=  < %s",
				     "-1", src_abbrev, ಖಾಲಿ);
		}
		ಸತತ_command(&cp_log);
	}
	ಬರೆ("\n");
}

ಸ್ಥಿರ ಶೂನ್ಯ generate_submodule_summary(ರಚನೆ summary_cb *info,
				       ರಚನೆ module_cb *p)
{
	ಅಕ್ಷರ *displaypath, *src_abbrev = ಖಾಲಿ, *dst_abbrev;
	ಪೂರ್ಣಾಂಕ missing_src = 0, missing_dst = 0;
	ರಚನೆ strbuf errmsg = STRBUF_INIT;
	ಪೂರ್ಣಾಂಕ ಒಟ್ಟು_commits = -1;

	ಆದರೆ (!info->cached && oideq(&p->oid_dst, null_oid(the_hash_algo))) {
		ಆದರೆ (S_ISGITLINK(p->mod_dst)) {
			ರಚನೆ ref_store *refs = repo_get_submodule_ref_store(the_repository,
									      p->sm_ಮಾರ್ಗ);

			ಆದರೆ (refs)
				refs_ತಲೆ_ref(refs, handle_submodule_ತಲೆ_ref, &p->oid_dst);
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (S_ISLNK(p->mod_dst) || S_ISREG(p->mod_dst)) {
			ರಚನೆ stat st;
			ಪೂರ್ಣಾಂಕ fd = open(p->sm_ಮಾರ್ಗ, O_RDONLY);

			ಆದರೆ (fd < 0 || fstat(fd, &st) < 0 ||
			    ಸೂಚ್ಯಂಕ_fd(the_repository->ಸೂಚ್ಯಂಕ, &p->oid_dst, fd, &st, OBJ_BLOB,
				     p->sm_ಮಾರ್ಗ, 0))
				error(_("couldn't hash object from '%s'"), p->sm_ಮಾರ್ಗ);
		} ಇಲ್ಲದಿದ್ದರೆ {
			/* for a submodule removal (mode:0000000), don't warn */
			ಆದರೆ (p->mod_dst)
				warning(_("unexpected mode %o"), p->mod_dst);
		}
	}

	ಆದರೆ (S_ISGITLINK(p->mod_src)) {
		ಆದರೆ (p->ಸ್ಥಿತಿ_2 != 'D')
			src_abbrev = verify_submodule_committish(p->sm_ಮಾರ್ಗ,
								 oid_ಗಮ್ಯ_hex(&p->oid_src));
		ಆದರೆ (!src_abbrev) {
			missing_src = 1;
			/*
			 * As `rev-parse` failed, we fallback to getting
			 * the abbreviated hash using oid_src. We do
			 * this as we might still need the abbreviated
			 * hash in cases like a submodule type change, etc.
			 */
			src_abbrev = xstrndup(oid_ಗಮ್ಯ_hex(&p->oid_src), 7);
		}
	} ಇಲ್ಲದಿದ್ದರೆ {
		/*
		 * The source does not point to a submodule.
		 * So, we fallback to getting the abbreviation using
		 * oid_src as we might still need the abbreviated
		 * hash in cases like submodule add, etc.
		 */
		src_abbrev = xstrndup(oid_ಗಮ್ಯ_hex(&p->oid_src), 7);
	}

	ಆದರೆ (S_ISGITLINK(p->mod_dst)) {
		dst_abbrev = verify_submodule_committish(p->sm_ಮಾರ್ಗ,
							 oid_ಗಮ್ಯ_hex(&p->oid_dst));
		ಆದರೆ (!dst_abbrev) {
			missing_dst = 1;
			/*
			 * As `rev-parse` failed, we fallback to getting
			 * the abbreviated hash using oid_dst. We do
			 * this as we might still need the abbreviated
			 * hash in cases like a submodule type change, etc.
			 */
			dst_abbrev = xstrndup(oid_ಗಮ್ಯ_hex(&p->oid_dst), 7);
		}
	} ಇಲ್ಲದಿದ್ದರೆ {
		/*
		 * The destination does not point to a submodule.
		 * So, we fallback to getting the abbreviation using
		 * oid_dst as we might still need the abbreviated
		 * hash in cases like a submodule removal, etc.
		 */
		dst_abbrev = xstrndup(oid_ಗಮ್ಯ_hex(&p->oid_dst), 7);
	}

	displaypath = get_submodule_displaypath(p->sm_ಮಾರ್ಗ, info->ಪೂರ್ವಪ್ರತ್ಯಯ,
						info->super_ಪೂರ್ವಪ್ರತ್ಯಯ);

	ಆದರೆ (!missing_src && !missing_dst) {
		ರಚನೆ ಮಗು_process cp_rev_list = ಮಗು_PROCESS_INIT;
		ರಚನೆ strbuf sb_rev_list = STRBUF_INIT;

		strvec_pushl(&cp_rev_list.ವಾದಗಳು, "rev-list",
			     "--first-parent", "--count", ಖಾಲಿ);
		ಆದರೆ (S_ISGITLINK(p->mod_src) && S_ISGITLINK(p->mod_dst))
			strvec_pushf(&cp_rev_list.ವಾದಗಳು, "%s...%s",
				     src_abbrev, dst_abbrev);
		ಇಲ್ಲದಿದ್ದರೆ
			strvec_ತಳ್ಳು(&cp_rev_list.ವಾದಗಳು, S_ISGITLINK(p->mod_src) ?
				    src_abbrev : dst_abbrev);
		strvec_ತಳ್ಳು(&cp_rev_list.ವಾದಗಳು, "--");

		cp_rev_list.git_cmd = 1;
		cp_rev_list.dir = p->sm_ಮಾರ್ಗ;
		prepare_submodule_repo_env(&cp_rev_list.env);

		ಆದರೆ (!capture_command(&cp_rev_list, &sb_rev_list, 0))
			ಒಟ್ಟು_commits = atoi(sb_rev_list.buf);

		strbuf_release(&sb_rev_list);
	} ಇಲ್ಲದಿದ್ದರೆ {
		/*
		 * Don't give error msg for modification whose dst is not
		 * submodule, i.e., deleted or changed to blob
		 */
		ಆದರೆ (S_ISGITLINK(p->mod_dst)) {
			ಆದರೆ (missing_src && missing_dst) {
				strbuf_addf(&errmsg, "  Warn: %s doesn't contain commits %s and %s\n",
					    displaypath, oid_ಗಮ್ಯ_hex(&p->oid_src),
					    oid_ಗಮ್ಯ_hex(&p->oid_dst));
			} ಇಲ್ಲದಿದ್ದರೆ {
				strbuf_addf(&errmsg, "  Warn: %s doesn't contain commit %s\n",
					    displaypath, missing_src ?
					    oid_ಗಮ್ಯ_hex(&p->oid_src) :
					    oid_ಗಮ್ಯ_hex(&p->oid_dst));
			}
		}
	}

	print_submodule_summary(info, errmsg.len ? errmsg.buf : ಖಾಲಿ,
				ಒಟ್ಟು_commits, displaypath, src_abbrev,
				dst_abbrev, p);

	ಬಿಡುಗಡೆ(displaypath);
	ಬಿಡುಗಡೆ(src_abbrev);
	ಬಿಡುಗಡೆ(dst_abbrev);
	strbuf_release(&errmsg);
}

ಸ್ಥಿರ ಶೂನ್ಯ prepare_submodule_summary(ರಚನೆ summary_cb *info,
				      ರಚನೆ module_cb_list *list)
{
	ಪೂರ್ಣಾಂಕ i;
	ಗಾಗಿ (i = 0; i < list->nr; i++) {
		ಸ್ಥಿರಾಂಕ ರಚನೆ submodule *sub;
		ರಚನೆ module_cb *p = list->entries[i];
		ರಚನೆ strbuf sm_gitdir = STRBUF_INIT;

		ಆದರೆ (p->ಸ್ಥಿತಿ_2 == 'D' || p->ಸ್ಥಿತಿ_2 == 'T') {
			generate_submodule_summary(info, p);
			ಮುಂದುವರಿ;
		}

		ಆದರೆ (info->for_ಸ್ಥಿತಿ && p->ಸ್ಥಿತಿ_2 != 'A' &&
		    (sub = submodule_from_ಮಾರ್ಗ(the_repository,
					       null_oid(the_hash_algo), p->sm_ಮಾರ್ಗ))) {
			ಅಕ್ಷರ *config_ಕೀಲಿ = ಖಾಲಿ;
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ;
			ಪೂರ್ಣಾಂಕ ignore_all = 0;

			config_ಕೀಲಿ = xstrfmt("submodule.%s.ignore",
					     sub->name);
			ಆದರೆ (!repo_config_get_string_tmp(the_repository, config_ಕೀಲಿ, &ಮೌಲ್ಯ))
				ignore_all = !strcmp(ಮೌಲ್ಯ, "all");
			ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (sub->ignore)
				ignore_all = !strcmp(sub->ignore, "all");

			ಬಿಡುಗಡೆ(config_ಕೀಲಿ);
			ಆದರೆ (ignore_all)
				ಮುಂದುವರಿ;
		}

		/* Also show added or modified modules which are checked out */
		strbuf_addstr(&sm_gitdir, p->sm_ಮಾರ್ಗ);
		ಆದರೆ (is_nonbare_repository_dir(&sm_gitdir))
			generate_submodule_summary(info, p);
		strbuf_release(&sm_gitdir);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ submodule_summary_callback(ರಚನೆ diff_ಸರತಿ_struct *q,
				       ರಚನೆ diff_options *options ಬಳಸದ,
				       ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
	ಪೂರ್ಣಾಂಕ i;
	ರಚನೆ module_cb_list *list = ದತ್ತಾಂಶ;
	ಗಾಗಿ (i = 0; i < q->nr; i++) {
		ರಚನೆ diff_filepair *p = q->ಸರತಿ[i];
		ರಚನೆ module_cb *ತಾತ್ಕಾಲಿಕ;

		ಆದರೆ (!S_ISGITLINK(p->one->ಬಹುಲಕ) && !S_ISGITLINK(p->two->ಬಹುಲಕ))
			ಮುಂದುವರಿ;
		ತಾತ್ಕಾಲಿಕ = xmalloc(ಗಾತ್ರ(*ತಾತ್ಕಾಲಿಕ));
		ತಾತ್ಕಾಲಿಕ->mod_src = p->one->ಬಹುಲಕ;
		ತಾತ್ಕಾಲಿಕ->mod_dst = p->two->ಬಹುಲಕ;
		ತಾತ್ಕಾಲಿಕ->oid_src = p->one->oid;
		ತಾತ್ಕಾಲಿಕ->oid_dst = p->two->oid;
		ತಾತ್ಕಾಲಿಕ->ಸ್ಥಿತಿ_2 = p->ಸ್ಥಿತಿ_2;
		ತಾತ್ಕಾಲಿಕ->sm_ಮಾರ್ಗ = xstrdup(p->one->ಮಾರ್ಗ);

		ALLOC_GROW(list->entries, list->nr + 1, list->alloc);
		list->entries[list->nr++] = ತಾತ್ಕಾಲಿಕ;
	}
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *get_diff_cmd(ಎಣಿಕೆ diff_cmd diff_cmd)
{
	ಆಯ್ಕೆ (diff_cmd) {
	ಸ್ಥಿತಿ DIFF_ಸೂಚ್ಯಂಕ: ಹಿಂತಿರುಗಿ "diff-index";
	ಸ್ಥಿತಿ DIFF_FILES: ಹಿಂತಿರುಗಿ "diff-files";
	ಪೂರ್ವನಿಯೋಜಿತ: BUG("bad diff_cmd value %d", diff_cmd);
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ compute_summary_module_list(ರಚನೆ object_id *ತಲೆ_oid,
				       ರಚನೆ summary_cb *info,
				       ಎಣಿಕೆ diff_cmd diff_cmd)
{
	ರಚನೆ strvec diff_ವಾದಗಳು = STRVEC_INIT;
	ರಚನೆ rev_info rev;
	ರಚನೆ module_cb_list list = MODULE_CB_LIST_INIT;
	ಪೂರ್ಣಾಂಕ ret = 0;

	strvec_ತಳ್ಳು(&diff_ವಾದಗಳು, get_diff_cmd(diff_cmd));
	ಆದರೆ (info->cached)
		strvec_ತಳ್ಳು(&diff_ವಾದಗಳು, "--cached");
	strvec_pushl(&diff_ವಾದಗಳು, "--ignore-submodules=dirty", "--raw", ಖಾಲಿ);
	ಆದರೆ (ತಲೆ_oid)
		strvec_ತಳ್ಳು(&diff_ವಾದಗಳು, oid_ಗಮ್ಯ_hex(ತಲೆ_oid));
	strvec_ತಳ್ಳು(&diff_ವಾದಗಳು, "--");
	ಆದರೆ (info->argc)
		strvec_pushv(&diff_ವಾದಗಳು, info->argv);

	repo_config(the_repository, git_diff_basic_config, ಖಾಲಿ);
	repo_init_revisions(the_repository, &rev, info->ಪೂರ್ವಪ್ರತ್ಯಯ);
	rev.abbrev = 0;
	precompose_argv_ಪೂರ್ವಪ್ರತ್ಯಯ(diff_ವಾದಗಳು.nr, diff_ವಾದಗಳು.v, ಖಾಲಿ);
	ಸಿದ್ಧತೆ_revisions_from_strvec(&diff_ವಾದಗಳು, &rev, ಖಾಲಿ);
	rev.diffopt.output_format = DIFF_FORMAT_NO_OUTPUT | DIFF_FORMAT_CALLBACK;
	rev.diffopt.format_callback = submodule_summary_callback;
	rev.diffopt.format_callback_ದತ್ತಾಂಶ = &list;

	ಆದರೆ (!info->cached) {
		ಆದರೆ (diff_cmd == DIFF_ಸೂಚ್ಯಂಕ)
			ಸಿದ್ಧತೆ_work_ವೃಕ್ಷ(the_repository);
		ಆದರೆ (repo_read_ಸೂಚ್ಯಂಕ_preload(the_repository, &rev.diffopt.pathspec, 0) < 0) {
			perror("repo_read_index_preload");
			ret = -1;
			ಹೋಗು cleanup;
		}
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (repo_read_ಸೂಚ್ಯಂಕ(the_repository) < 0) {
		perror("repo_read_cache");
		ret = -1;
		ಹೋಗು cleanup;
	}

	ಆದರೆ (diff_cmd == DIFF_ಸೂಚ್ಯಂಕ)
		ಸತತ_diff_ಸೂಚ್ಯಂಕ(&rev, info->cached ? DIFF_ಸೂಚ್ಯಂಕ_CACHED : 0);
	ಇಲ್ಲದಿದ್ದರೆ
		ಸತತ_diff_files(&rev, 0);
	prepare_submodule_summary(info, &list);
cleanup:
	strvec_clear(&diff_ವಾದಗಳು);
	release_revisions(&rev);
	module_cb_list_release(&list);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ module_summary(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			  ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ summary_cb info = SUMMARY_CB_INIT;
	ಪೂರ್ಣಾಂಕ cached = 0;
	ಪೂರ್ಣಾಂಕ for_ಸ್ಥಿತಿ = 0;
	ಪೂರ್ಣಾಂಕ files = 0;
	ಪೂರ್ಣಾಂಕ summary_ಎಲ್ಲೆ = -1;
	ಎಣಿಕೆ diff_cmd diff_cmd = DIFF_ಸೂಚ್ಯಂಕ;
	ರಚನೆ object_id ತಲೆ_oid;
	ಪೂರ್ಣಾಂಕ ret;
	ರಚನೆ option module_summary_options[] = {
		OPT_BOOL(0, "cached", &cached,
			 N_("use the commit stored in the index instead of the submodule HEAD")),
		OPT_BOOL(0, "files", &files,
			 N_("compare the commit in the index with that in the submodule HEAD")),
		OPT_BOOL(0, "for-status", &for_ಸ್ಥಿತಿ,
			 N_("skip submodules with 'ignore_config' value set to 'all'")),
		OPT_INTEGER('n', "summary-limit", &summary_ಎಲ್ಲೆ,
			     N_("limit the summary size")),
		OPT_ಕೊನೆ()
	};
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ git_submodule_helper_usage[] = {
		N_("git submodule summary [<options>] [<commit>] [--] [<path>]"),
		ಖಾಲಿ
	};

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, module_summary_options,
			     git_submodule_helper_usage, 0);

	ಆದರೆ (!summary_ಎಲ್ಲೆ)
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (!repo_get_oid(the_repository, argc ? argv[0] : "HEAD", &ತಲೆ_oid)) {
		ಆದರೆ (argc) {
			argv++;
			argc--;
		}
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!argc || !strcmp(argv[0], "HEAD")) {
		/* before the first commit: compare with an empty tree */
		oidcpy(&ತಲೆ_oid, the_hash_algo->ಖಾಲಿಗಳು_ವೃಕ್ಷ);
		ಆದರೆ (argc) {
			argv++;
			argc--;
		}
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಆದರೆ (repo_get_oid(the_repository, "HEAD", &ತಲೆ_oid))
			die(_("could not fetch a revision for HEAD"));
	}

	ಆದರೆ (files) {
		ಆದರೆ (cached)
			die(_("options '%s' and '%s' cannot be used together"), "--cached", "--files");
		diff_cmd = DIFF_FILES;
	}

	info.argc = argc;
	info.argv = argv;
	info.ಪೂರ್ವಪ್ರತ್ಯಯ = ಪೂರ್ವಪ್ರತ್ಯಯ;
	info.cached = !!cached;
	info.files = !!files;
	info.for_ಸ್ಥಿತಿ = !!for_ಸ್ಥಿತಿ;
	info.summary_ಎಲ್ಲೆ = summary_ಎಲ್ಲೆ;

	ret = compute_summary_module_list((diff_cmd == DIFF_ಸೂಚ್ಯಂಕ) ? &ತಲೆ_oid : ಖಾಲಿ,
					  &info, diff_cmd);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ module_gitdir(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ ಬಳಸದ,
			 ರಚನೆ repository *repo)
{
	ರಚನೆ strbuf gitdir = STRBUF_INIT;

	ಆದರೆ (argc != 2)
		usage(_("git submodule--helper gitdir <name>"));

	submodule_name_ಗಮ್ಯ_gitdir(&gitdir, repo, argv[1]);

	ಬರೆ("%s\n", gitdir.buf);

	strbuf_release(&gitdir);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ module_migrate(ಪೂರ್ಣಾಂಕ argc ಬಳಸದ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv ಬಳಸದ,
			  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ ಬಳಸದ, ರಚನೆ repository *repo)
{
	ರಚನೆ strbuf module_dir = STRBUF_INIT;
	DIR *dir;
	ರಚನೆ dirent *de;
	ಪೂರ್ಣಾಂಕ repo_ಆವೃತ್ತಿ = 0;

	repo_git_ಮಾರ್ಗ_append(repo, &module_dir, "modules/");

	dir = opendir(module_dir.buf);
	ಆದರೆ (!dir)
		die(_("could not open '%s'"), module_dir.buf);

	ಆಗ ((de = readdir(dir))) {
		ರಚನೆ strbuf gitdir_ಮಾರ್ಗ = STRBUF_INIT;
		ಅಕ್ಷರ *ಕೀಲಿ;
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ;

		ಆದರೆ (is_dot_or_dotdot(de->d_name))
			ಮುಂದುವರಿ;

		strbuf_addf(&gitdir_ಮಾರ್ಗ, "%s/%s", module_dir.buf, de->d_name);
		ಆದರೆ (!is_git_directory(gitdir_ಮಾರ್ಗ.buf)) {
			strbuf_release(&gitdir_ಮಾರ್ಗ);
			ಮುಂದುವರಿ;
		}
		strbuf_release(&gitdir_ಮಾರ್ಗ);

		ಕೀಲಿ = xstrfmt("submodule.%s.gitdir", de->d_name);
		ಆದರೆ (!repo_config_get_string_tmp(repo, ಕೀಲಿ, &ಮೌಲ್ಯ)) {
			/* Already has a gitdir config, nothing to do. */
			ಬಿಡುಗಡೆ(ಕೀಲಿ);
			ಮುಂದುವರಿ;
		}
		ಬಿಡುಗಡೆ(ಕೀಲಿ);

		create_default_gitdir_config(de->d_name);
	}

	closedir(dir);
	strbuf_release(&module_dir);

	repo_config_get_int(the_repository, "core.repositoryformatversion", &repo_ಆವೃತ್ತಿ);
	ಆದರೆ (repo_ಆವೃತ್ತಿ == 0 &&
	    repo_config_ಗಣ_gently(repo, "core.repositoryformatversion", "1"))
		die(_("could not set core.repositoryformatversion to 1.\n"
		      "Please set it for migration to work, for example:\n"
		      "git config core.repositoryformatversion 1"));

	ಆದರೆ (repo_config_ಗಣ_gently(repo, "extensions.submodulePathConfig", "true"))
		die(_("could not enable submodulePathConfig extension. It is required\n"
		      "for migration to work. Please enable it in the root repo:\n"
		      "git config extensions.submodulePathConfig true"));

	repo->repository_format_submodule_ಮಾರ್ಗ_cfg = 1;

	ಹಿಂತಿರುಗಿ 0;
}

ರಚನೆ sync_cb {
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *super_ಪೂರ್ವಪ್ರತ್ಯಯ;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags;
};
#ವ್ಯಾಖ್ಯಾನಿಸು SYNC_CB_INIT { 0 }

ಸ್ಥಿರ ಶೂನ್ಯ sync_submodule(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *super_ಪೂರ್ವಪ್ರತ್ಯಯ, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ submodule *sub;
	ಅಕ್ಷರ *remote_ಕೀಲಿ = ಖಾಲಿ;
	ಅಕ್ಷರ *sub_origin_url, *super_config_url, *displaypath, *default_remote;
	ರಚನೆ strbuf sb = STRBUF_INIT;
	ಅಕ್ಷರ *sub_config_ಮಾರ್ಗ = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ ಸಂಕೇತ_2;

	ಆದರೆ (!is_submodule_ಸಕ್ರಿಯ(the_repository, ಮಾರ್ಗ))
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (validate_submodule_ಮಾರ್ಗ(ಮಾರ್ಗ) < 0)
		die(ಖಾಲಿ);

	sub = submodule_from_ಮಾರ್ಗ(the_repository, null_oid(the_hash_algo), ಮಾರ್ಗ);

	ಆದರೆ (sub && sub->url) {
		ಆದರೆ (starts_with_dot_dot_slash(sub->url) ||
		    starts_with_dot_slash(sub->url)) {
			ಅಕ್ಷರ *ಮೇಲೆ_ಮಾರ್ಗ = get_ಮೇಲೆ_ಮಾರ್ಗ(ಮಾರ್ಗ);

			sub_origin_url = resolve_relative_url(sub->url, ಮೇಲೆ_ಮಾರ್ಗ, 1);
			super_config_url = resolve_relative_url(sub->url, ಖಾಲಿ, 1);
			ಬಿಡುಗಡೆ(ಮೇಲೆ_ಮಾರ್ಗ);
		} ಇಲ್ಲದಿದ್ದರೆ {
			sub_origin_url = xstrdup(sub->url);
			super_config_url = xstrdup(sub->url);
		}
	} ಇಲ್ಲದಿದ್ದರೆ {
		sub_origin_url = xstrdup("");
		super_config_url = xstrdup("");
	}

	displaypath = get_submodule_displaypath(ಮಾರ್ಗ, ಪೂರ್ವಪ್ರತ್ಯಯ, super_ಪೂರ್ವಪ್ರತ್ಯಯ);

	ಆದರೆ (!(flags & OPT_QUIET))
		ಬರೆ(_("Synchronizing submodule url for '%s'\n"),
			 displaypath);

	strbuf_reset(&sb);
	strbuf_addf(&sb, "submodule.%s.url", sub->name);
	ಆದರೆ (repo_config_ಗಣ_gently(the_repository, sb.buf, super_config_url))
		die(_("failed to register url for submodule path '%s'"),
		      displaypath);

	ಆದರೆ (!is_submodule_populated_gently(ಮಾರ್ಗ, ಖಾಲಿ))
		ಹೋಗು cleanup;

	strbuf_reset(&sb);
	ಸಂಕೇತ_2 = get_default_remote_submodule(ಮಾರ್ಗ, &default_remote);
	ಆದರೆ (ಸಂಕೇತ_2)
		ನಿರ್ಗಮಿಸು(ಸಂಕೇತ_2);

	remote_ಕೀಲಿ = xstrfmt("remote.%s.url", default_remote);
	ಬಿಡುಗಡೆ(default_remote);

	submodule_ಗಮ್ಯ_gitdir(the_repository, &sb, ಮಾರ್ಗ);
	strbuf_addstr(&sb, "/config");

	ಆದರೆ (repo_config_ಗಣ_ಒಳ_file_gently(the_repository, sb.buf, remote_ಕೀಲಿ, ಖಾಲಿ, sub_origin_url))
		die(_("failed to update remote for submodule '%s'"),
		      ಮಾರ್ಗ);

	ಆದರೆ (flags & OPT_RECURSIVE) {
		ರಚನೆ ಮಗು_process cpr = ಮಗು_PROCESS_INIT;

		cpr.git_cmd = 1;
		cpr.dir = ಮಾರ್ಗ;
		prepare_submodule_repo_env(&cpr.env);

		strvec_pushl(&cpr.ವಾದಗಳು, "submodule--helper", "sync",
			     "--recursive", ಖಾಲಿ);
		strvec_pushf(&cpr.ವಾದಗಳು, "--super-prefix=%s/", displaypath);

		ಆದರೆ (flags & OPT_QUIET)
			strvec_ತಳ್ಳು(&cpr.ವಾದಗಳು, "--quiet");

		ಆದರೆ (ಸತತ_command(&cpr))
			die(_("failed to recurse into submodule '%s'"),
			      ಮಾರ್ಗ);
	}

cleanup:
	ಬಿಡುಗಡೆ(super_config_url);
	ಬಿಡುಗಡೆ(sub_origin_url);
	strbuf_release(&sb);
	ಬಿಡುಗಡೆ(remote_ಕೀಲಿ);
	ಬಿಡುಗಡೆ(displaypath);
	ಬಿಡುಗಡೆ(sub_config_ಮಾರ್ಗ);
}

ಸ್ಥಿರ ಶೂನ್ಯ sync_submodule_cb(ಸ್ಥಿರಾಂಕ ರಚನೆ cache_ನಮೂದು *list_ಅಂಶ, ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ)
{
	ರಚನೆ sync_cb *info = cb_ದತ್ತಾಂಶ;

	sync_submodule(list_ಅಂಶ->name, info->ಪೂರ್ವಪ್ರತ್ಯಯ, info->super_ಪೂರ್ವಪ್ರತ್ಯಯ,
		       info->flags);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ module_sync(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		       ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ sync_cb info = SYNC_CB_INIT;
	ರಚನೆ pathspec pathspec = { 0 };
	ರಚನೆ module_list list = MODULE_LIST_INIT;
	ಪೂರ್ಣಾಂಕ quiet = 0;
	ಪೂರ್ಣಾಂಕ recursive = 0;
	ರಚನೆ option module_sync_options[] = {
		OPT_SUPER_ಪೂರ್ವಪ್ರತ್ಯಯ(&info.super_ಪೂರ್ವಪ್ರತ್ಯಯ),
		OPT__QUIET(&quiet, N_("suppress output of synchronizing submodule url")),
		OPT_BOOL(0, "recursive", &recursive,
			N_("recurse into nested submodules")),
		OPT_ಕೊನೆ()
	};
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ git_submodule_helper_usage[] = {
		N_("git submodule sync [--quiet] [--recursive] [<path>]"),
		ಖಾಲಿ
	};
	ಪೂರ್ಣಾಂಕ ret = 1;

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, module_sync_options,
			     git_submodule_helper_usage, 0);

	ಆದರೆ (module_list_compute(argv, ಪೂರ್ವಪ್ರತ್ಯಯ, &pathspec, &list) < 0)
		ಹೋಗು cleanup;

	info.ಪೂರ್ವಪ್ರತ್ಯಯ = ಪೂರ್ವಪ್ರತ್ಯಯ;
	ಆದರೆ (quiet)
		info.flags |= OPT_QUIET;
	ಆದರೆ (recursive)
		info.flags |= OPT_RECURSIVE;

	for_each_listed_submodule(&list, sync_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	ಹಿಂತಿರುಗಿ ret;
}

ರಚನೆ deinit_cb {
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags;
};
#ವ್ಯಾಖ್ಯಾನಿಸು DEINIT_CB_INIT { 0 }

ಸ್ಥಿರ ಶೂನ್ಯ deinit_submodule(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			     ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ submodule *sub;
	ಅಕ್ಷರ *displaypath = ಖಾಲಿ;
	ರಚನೆ ಮಗು_process cp_config = ಮಗು_PROCESS_INIT;
	ರಚನೆ strbuf sb_config = STRBUF_INIT;
	ಅಕ್ಷರ *sub_git_dir = xstrfmt("%s/.git", ಮಾರ್ಗ);

	ಆದರೆ (validate_submodule_ಮಾರ್ಗ(ಮಾರ್ಗ) < 0)
		die(ಖಾಲಿ);

	sub = submodule_from_ಮಾರ್ಗ(the_repository, null_oid(the_hash_algo), ಮಾರ್ಗ);

	ಆದರೆ (!sub || !sub->name)
		ಹೋಗು cleanup;

	displaypath = get_submodule_displaypath(ಮಾರ್ಗ, ಪೂರ್ವಪ್ರತ್ಯಯ, ಖಾಲಿ);

	/* remove the submodule work tree (unless the user already did it) */
	ಆದರೆ (is_directory(ಮಾರ್ಗ)) {
		ರಚನೆ strbuf sb_rm = STRBUF_INIT;
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *format;

		ಆದರೆ (is_directory(sub_git_dir)) {
			ಆದರೆ (!(flags & OPT_QUIET))
				warning(_("Submodule work tree '%s' contains a .git "
					  "directory. This will be replaced with a "
					  ".git file by using absorbgitdirs."),
					displaypath);

			absorb_git_dir_into_superproject(ಮಾರ್ಗ, ಖಾಲಿ);

		}

		ಆದರೆ (!(flags & OPT_FORCE)) {
			ರಚನೆ ಮಗು_process cp_rm = ಮಗು_PROCESS_INIT;

			cp_rm.git_cmd = 1;
			strvec_pushl(&cp_rm.ವಾದಗಳು, "rm", "-qn",
				     ಮಾರ್ಗ, ಖಾಲಿ);

			ಆದರೆ (ಸತತ_command(&cp_rm))
				die(_("Submodule work tree '%s' contains local "
				      "modifications; use '-f' to discard them"),
				      displaypath);
		}

		strbuf_addstr(&sb_rm, ಮಾರ್ಗ);

		ಆದರೆ (!remove_dir_recursively(&sb_rm, 0))
			format = _("Cleared directory '%s'\n");
		ಇಲ್ಲದಿದ್ದರೆ
			format = _("Could not remove submodule work tree '%s'\n");

		ಆದರೆ (!(flags & OPT_QUIET))
			ಬರೆ(format, displaypath);

		submodule_unset_core_worktree(sub);

		strbuf_release(&sb_rm);
	}

	ಆದರೆ (mkdir(ಮಾರ್ಗ, 0777))
		ಬರೆ(_("could not create empty submodule directory %s"),
		      displaypath);

	cp_config.git_cmd = 1;
	strvec_pushl(&cp_config.ವಾದಗಳು, "config", "--get-regexp", ಖಾಲಿ);
	strvec_pushf(&cp_config.ವಾದಗಳು, "submodule.%s\\.", sub->name);

	/* remove the .git/config entries (unless the user already did it) */
	ಆದರೆ (!capture_command(&cp_config, &sb_config, 0) && sb_config.len) {
		ಅಕ್ಷರ *sub_ಕೀಲಿ = xstrfmt("submodule.%s", sub->name);

		/*
		 * remove the whole section so we have a clean state when
		 * the user later decides to init this submodule again
		 */
		repo_config_rename_section_ಒಳ_file(the_repository, ಖಾಲಿ, sub_ಕೀಲಿ, ಖಾಲಿ);
		ಆದರೆ (!(flags & OPT_QUIET))
			ಬರೆ(_("Submodule '%s' (%s) unregistered for path '%s'\n"),
				 sub->name, sub->url, displaypath);
		ಬಿಡುಗಡೆ(sub_ಕೀಲಿ);
	}

cleanup:
	ಬಿಡುಗಡೆ(displaypath);
	ಬಿಡುಗಡೆ(sub_git_dir);
	strbuf_release(&sb_config);
}

ಸ್ಥಿರ ಶೂನ್ಯ deinit_submodule_cb(ಸ್ಥಿರಾಂಕ ರಚನೆ cache_ನಮೂದು *list_ಅಂಶ,
				ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ)
{
	ರಚನೆ deinit_cb *info = cb_ದತ್ತಾಂಶ;
	deinit_submodule(list_ಅಂಶ->name, info->ಪೂರ್ವಪ್ರತ್ಯಯ, info->flags);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ module_deinit(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			 ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ deinit_cb info = DEINIT_CB_INIT;
	ರಚನೆ pathspec pathspec = { 0 };
	ರಚನೆ module_list list = MODULE_LIST_INIT;
	ಪೂರ್ಣಾಂಕ quiet = 0;
	ಪೂರ್ಣಾಂಕ force = 0;
	ಪೂರ್ಣಾಂಕ all = 0;
	ರಚನೆ option module_deinit_options[] = {
		OPT__QUIET(&quiet, N_("suppress submodule status output")),
		OPT__FORCE(&force, N_("remove submodule working trees even if they contain local changes"), 0),
		OPT_BOOL(0, "all", &all, N_("unregister all submodules")),
		OPT_ಕೊನೆ()
	};
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ git_submodule_helper_usage[] = {
		N_("git submodule deinit [--quiet] [-f | --force] [--all | [--] [<path>...]]"),
		ಖಾಲಿ
	};
	ಪೂರ್ಣಾಂಕ ret = 1;

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, module_deinit_options,
			     git_submodule_helper_usage, 0);

	ಆದರೆ (all && argc) {
		error("pathspec and --all are incompatible");
		usage_with_options(git_submodule_helper_usage,
				   module_deinit_options);
	}

	ಆದರೆ (!argc && !all)
		die(_("Use '--all' if you really want to deinitialize all submodules"));

	ಆದರೆ (module_list_compute(argv, ಪೂರ್ವಪ್ರತ್ಯಯ, &pathspec, &list) < 0)
		ಹೋಗು cleanup;

	info.ಪೂರ್ವಪ್ರತ್ಯಯ = ಪೂರ್ವಪ್ರತ್ಯಯ;
	ಆದರೆ (quiet)
		info.flags |= OPT_QUIET;
	ಆದರೆ (force)
		info.flags |= OPT_FORCE;

	for_each_listed_submodule(&list, deinit_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	ಹಿಂತಿರುಗಿ ret;
}

ರಚನೆ module_clone_ದತ್ತಾಂಶ {
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *url;
	ಪೂರ್ಣಾಂಕ ಆಳ;
	ರಚನೆ list_objects_filter_options *filter_options;
	ಎಣಿಕೆ ref_storage_format ref_storage_format;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ quiet: 1;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಪ್ರಗತಿ: 1;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ dissociate: 1;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ require_init: 1;
	ಪೂರ್ಣಾಂಕ single_branch;
};
#ವ್ಯಾಖ್ಯಾನಿಸು MODULE_CLONE_ದತ್ತಾಂಶ_INIT { \
	.single_branch = -1, \
	.ref_storage_format = REF_STORAGE_FORMAT_UNKNOWN, \
}

ರಚನೆ submodule_alternate_ಸಿದ್ಧತೆ {
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *submodule_name;
	ಎಣಿಕೆ SUBMODULE_ALTERNATE_ERROR_ಬಹುಲಕ {
		SUBMODULE_ALTERNATE_ERROR_DIE,
		SUBMODULE_ALTERNATE_ERROR_INFO,
		SUBMODULE_ALTERNATE_ERROR_IGNORE
	} error_ಬಹುಲಕ;
	ರಚನೆ string_list *reference;
};
#ವ್ಯಾಖ್ಯಾನಿಸು SUBMODULE_ALTERNATE_ಸಿದ್ಧತೆ_INIT { \
	.error_ಬಹುಲಕ = SUBMODULE_ALTERNATE_ERROR_IGNORE, \
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ alternate_error_advice[] = N_(
"An alternate computed from a superproject's alternate is invalid.\n"
"To allow Git to clone without an alternate in such a case, set\n"
"submodule.alternateErrorStrategy to 'info' or, equivalently, clone with\n"
"'--reference-if-able' instead of '--reference'."
);

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಕೂಡಿಸು_possible_reference_from_superproject(
		ರಚನೆ odb_files_dir *alt_odb, ಶೂನ್ಯ *sas_cb)
{
	ರಚನೆ submodule_alternate_ಸಿದ್ಧತೆ *sas = sas_cb;
	ಗಾತ್ರ_t len;

	/*
	 * If the alternate object store is another repository, try the
	 * standard layout with .git/(modules/<name>)+/objects
	 */
	ಆದರೆ (strip_suffix(alt_odb->abspath, "/objects", &len)) {
		ರಚನೆ repository alternate;
		ಅಕ್ಷರ *sm_alternate;
		ರಚನೆ strbuf sb = STRBUF_INIT;
		ರಚನೆ strbuf err = STRBUF_INIT;
		strbuf_ಕೂಡಿಸು(&sb, alt_odb->abspath, len);

		ಆದರೆ (repo_init(&alternate, sb.buf, ಖಾಲಿ) < 0)
			die(_("could not get a repository handle for gitdir '%s'"),
			    sb.buf);

		/*
		 * We need to end the new path with '/' to mark it as a dir,
		 * otherwise a submodule name containing '/' will be broken
		 * as the last part of a missing submodule reference would
		 * be taken as a file name.
		 */
		strbuf_reset(&sb);
		submodule_name_ಗಮ್ಯ_gitdir(&sb, &alternate, sas->submodule_name);
		strbuf_addch(&sb, '/');
		repo_clear(&alternate);

		sm_alternate = compute_alternate_ಮಾರ್ಗ(sb.buf, &err);
		ಆದರೆ (sm_alternate) {
			ಅಕ್ಷರ *p = strbuf_detach(&sb, ಖಾಲಿ);

			string_list_append(sas->reference, p)->util = p;
			ಬಿಡುಗಡೆ(sm_alternate);
		} ಇಲ್ಲದಿದ್ದರೆ {
			ಆಯ್ಕೆ (sas->error_ಬಹುಲಕ) {
			ಸ್ಥಿತಿ SUBMODULE_ALTERNATE_ERROR_DIE:
				ಆದರೆ (advice_enabled(ADVICE_SUBMODULE_ALTERNATE_ERROR_STRATEGY_DIE))
					advise(_(alternate_error_advice));
				die(_("submodule '%s' cannot add alternate: %s"),
				    sas->submodule_name, err.buf);
			ಸ್ಥಿತಿ SUBMODULE_ALTERNATE_ERROR_INFO:
				fprintf_ln(stderr, _("submodule '%s' cannot add alternate: %s"),
					sas->submodule_name, err.buf);
			ಸ್ಥಿತಿ SUBMODULE_ALTERNATE_ERROR_IGNORE:
				; /* nothing */
			}
		}

		strbuf_release(&err);
		strbuf_release(&sb);
	}

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ prepare_possible_alternates(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *sm_name,
		ರಚನೆ string_list *reference)
{
	ಅಕ್ಷರ *sm_alternate = ಖಾಲಿ, *error_strategy = ಖಾಲಿ;
	ರಚನೆ submodule_alternate_ಸಿದ್ಧತೆ sas = SUBMODULE_ALTERNATE_ಸಿದ್ಧತೆ_INIT;

	repo_config_get_string(the_repository, "submodule.alternateLocation", &sm_alternate);
	ಆದರೆ (!sm_alternate)
		ಹಿಂತಿರುಗಿ;

	repo_config_get_string(the_repository, "submodule.alternateErrorStrategy", &error_strategy);

	ಆದರೆ (!error_strategy)
		error_strategy = xstrdup("die");

	sas.submodule_name = sm_name;
	sas.reference = reference;
	ಆದರೆ (!strcmp(error_strategy, "die"))
		sas.error_ಬಹುಲಕ = SUBMODULE_ALTERNATE_ERROR_DIE;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(error_strategy, "info"))
		sas.error_ಬಹುಲಕ = SUBMODULE_ALTERNATE_ERROR_INFO;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(error_strategy, "ignore"))
		sas.error_ಬಹುಲಕ = SUBMODULE_ALTERNATE_ERROR_IGNORE;
	ಇಲ್ಲದಿದ್ದರೆ
		die(_("Value '%s' for submodule.alternateErrorStrategy is not recognized"), error_strategy);

	ಆದರೆ (!strcmp(sm_alternate, "superproject"))
		odb_for_each_alternate(the_repository->objects,
				       ಕೂಡಿಸು_possible_reference_from_superproject, &sas);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(sm_alternate, "no"))
		; /* do nothing */
	ಇಲ್ಲದಿದ್ದರೆ
		die(_("Value '%s' for submodule.alternateLocation is not recognized"), sm_alternate);

	ಬಿಡುಗಡೆ(sm_alternate);
	ಬಿಡುಗಡೆ(error_strategy);
}

ಸ್ಥಿರ ಅಕ್ಷರ *clone_submodule_sm_gitdir(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name)
{
	ರಚನೆ strbuf sb = STRBUF_INIT;
	ಅಕ್ಷರ *sm_gitdir;

	submodule_name_ಗಮ್ಯ_gitdir(&sb, the_repository, name);
	sm_gitdir = absolute_pathdup(sb.buf);
	strbuf_release(&sb);

	ಹಿಂತಿರುಗಿ sm_gitdir;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ dir_ಒಳಗೊಂಡಿದೆ_only_dotgit(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ)
{
	DIR *dir = opendir(ಮಾರ್ಗ);
	ರಚನೆ dirent *e;
	ಪೂರ್ಣಾಂಕ ret = 1;

	ಆದರೆ (!dir)
		ಹಿಂತಿರುಗಿ 0;

	e = readdir_skip_dot_and_dotdot(dir);
	ಆದರೆ (!e)
		ret = 0;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (strcmp(DEFAULT_GIT_DIR_ENVIRONMENT, e->d_name) ||
		 (e = readdir_skip_dot_and_dotdot(dir))) {
		error("unexpected item '%s' in '%s'", e->d_name, ಮಾರ್ಗ);
		ret = 0;
	}

	closedir(dir);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ clone_submodule(ಸ್ಥಿರಾಂಕ ರಚನೆ module_clone_ದತ್ತಾಂಶ *clone_ದತ್ತಾಂಶ,
			   ರಚನೆ string_list *reference)
{
	ಅಕ್ಷರ *p;
	ಅಕ್ಷರ *sm_gitdir = clone_submodule_sm_gitdir(clone_ದತ್ತಾಂಶ->name);
	ಅಕ್ಷರ *sm_alternate = ಖಾಲಿ, *error_strategy = ಖಾಲಿ;
	ರಚನೆ stat st;
	ರಚನೆ ಮಗು_process cp = ಮಗು_PROCESS_INIT;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *clone_ದತ್ತಾಂಶ_ಮಾರ್ಗ = clone_ದತ್ತಾಂಶ->ಮಾರ್ಗ;
	ಅಕ್ಷರ *ಗಮ್ಯ_free = ಖಾಲಿ;

	ಆದರೆ (validate_submodule_ಮಾರ್ಗ(clone_ದತ್ತಾಂಶ_ಮಾರ್ಗ) < 0)
		die(ಖಾಲಿ);

	ಆದರೆ (!is_absolute_ಮಾರ್ಗ(clone_ದತ್ತಾಂಶ->ಮಾರ್ಗ))
		clone_ದತ್ತಾಂಶ_ಮಾರ್ಗ = ಗಮ್ಯ_free = xstrfmt("%s/%s", repo_get_work_ವೃಕ್ಷ(the_repository),
						    clone_ದತ್ತಾಂಶ->ಮಾರ್ಗ);

	ಆದರೆ (!file_exists(sm_gitdir)) {
		ಆದರೆ (clone_ದತ್ತಾಂಶ->require_init && !stat(clone_ದತ್ತಾಂಶ_ಮಾರ್ಗ, &st) &&
		    !is_ಖಾಲಿಗಳು_dir(clone_ದತ್ತಾಂಶ_ಮಾರ್ಗ))
			die(_("directory not empty: '%s'"), clone_ದತ್ತಾಂಶ_ಮಾರ್ಗ);

		ಆದರೆ (ಸುರಕ್ಷಿತ_create_leading_directories_const(the_repository, sm_gitdir) < 0)
			die(_("could not create directory '%s'"), sm_gitdir);

		prepare_possible_alternates(clone_ದತ್ತಾಂಶ->name, reference);

		strvec_ತಳ್ಳು(&cp.ವಾದಗಳು, "clone");
		strvec_ತಳ್ಳು(&cp.ವಾದಗಳು, "--no-checkout");
		ಆದರೆ (clone_ದತ್ತಾಂಶ->quiet)
			strvec_ತಳ್ಳು(&cp.ವಾದಗಳು, "--quiet");
		ಆದರೆ (clone_ದತ್ತಾಂಶ->ಪ್ರಗತಿ)
			strvec_ತಳ್ಳು(&cp.ವಾದಗಳು, "--progress");
		ಆದರೆ (clone_ದತ್ತಾಂಶ->ಆಳ > 0)
			strvec_pushf(&cp.ವಾದಗಳು, "--depth=%d", clone_ದತ್ತಾಂಶ->ಆಳ);
		ಆದರೆ (reference->nr) {
			ರಚನೆ string_list_ಅಂಶ *ಅಂಶ;

			for_each_string_list_ಅಂಶ(ಅಂಶ, reference)
				strvec_pushl(&cp.ವಾದಗಳು, "--reference",
					     ಅಂಶ->string, ಖಾಲಿ);
		}
		ಆದರೆ (clone_ದತ್ತಾಂಶ->ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
			strvec_pushf(&cp.ವಾದಗಳು, "--ref-storage-format=%s",
				     ref_storage_format_ಗಮ್ಯ_name(clone_ದತ್ತಾಂಶ->ref_storage_format));
		ಆದರೆ (clone_ದತ್ತಾಂಶ->dissociate)
			strvec_ತಳ್ಳು(&cp.ವಾದಗಳು, "--dissociate");
		ಆದರೆ (sm_gitdir && *sm_gitdir)
			strvec_pushl(&cp.ವಾದಗಳು, "--separate-git-dir", sm_gitdir, ಖಾಲಿ);
		ಆದರೆ (clone_ದತ್ತಾಂಶ->filter_options && clone_ದತ್ತಾಂಶ->filter_options->choice)
			strvec_pushf(&cp.ವಾದಗಳು, "--filter=%s",
				     expand_list_objects_filter_spec(
					     clone_ದತ್ತಾಂಶ->filter_options));
		ಆದರೆ (clone_ದತ್ತಾಂಶ->single_branch >= 0)
			strvec_ತಳ್ಳು(&cp.ವಾದಗಳು, clone_ದತ್ತಾಂಶ->single_branch ?
				    "--single-branch" :
				    "--no-single-branch");

		strvec_ತಳ್ಳು(&cp.ವಾದಗಳು, "--");
		strvec_ತಳ್ಳು(&cp.ವಾದಗಳು, clone_ದತ್ತಾಂಶ->url);
		strvec_ತಳ್ಳು(&cp.ವಾದಗಳು, clone_ದತ್ತಾಂಶ_ಮಾರ್ಗ);

		cp.git_cmd = 1;
		prepare_submodule_repo_env(&cp.env);
		cp.no_stdin = 1;

		ಆದರೆ(ಸತತ_command(&cp))
			die(_("clone of '%s' into submodule path '%s' failed"),
			    clone_ದತ್ತಾಂಶ->url, clone_ದತ್ತಾಂಶ_ಮಾರ್ಗ);

		ಆದರೆ (clone_ದತ್ತಾಂಶ->require_init && !stat(clone_ದತ್ತಾಂಶ_ಮಾರ್ಗ, &st) &&
		    !dir_ಒಳಗೊಂಡಿದೆ_only_dotgit(clone_ದತ್ತಾಂಶ_ಮಾರ್ಗ)) {
			ಅಕ್ಷರ *dot_git = xstrfmt("%s/.git", clone_ದತ್ತಾಂಶ_ಮಾರ್ಗ);
			unlink(dot_git);
			ಬಿಡುಗಡೆ(dot_git);
			die(_("directory not empty: '%s'"), clone_ದತ್ತಾಂಶ_ಮಾರ್ಗ);
		}
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಅಕ್ಷರ *ಮಾರ್ಗ;

		ಆದರೆ (clone_ದತ್ತಾಂಶ->require_init && !stat(clone_ದತ್ತಾಂಶ_ಮಾರ್ಗ, &st) &&
		    !is_ಖಾಲಿಗಳು_dir(clone_ದತ್ತಾಂಶ_ಮಾರ್ಗ))
			die(_("directory not empty: '%s'"), clone_ದತ್ತಾಂಶ_ಮಾರ್ಗ);
		ಆದರೆ (ಸುರಕ್ಷಿತ_create_leading_directories_const(the_repository, clone_ದತ್ತಾಂಶ_ಮಾರ್ಗ) < 0)
			die(_("could not create directory '%s'"), clone_ದತ್ತಾಂಶ_ಮಾರ್ಗ);
		ಮಾರ್ಗ = xstrfmt("%s/index", sm_gitdir);
		unlink_or_warn(ಮಾರ್ಗ);
		ಬಿಡುಗಡೆ(ಮಾರ್ಗ);
	}

	/*
	 * We already performed this check at the beginning of this function,
	 * before cloning the objects. This tries to detect racy behavior e.g.
	 * in parallel clones, where another process could easily have made the
	 * gitdir nested _after_ it was created.
	 *
	 * To prevent further harm coming from this unintentionally-nested
	 * gitdir, let's disable it by deleting the `HEAD` file.
	 */
	ಆದರೆ (validate_submodule_git_dir(sm_gitdir, clone_ದತ್ತಾಂಶ->name) < 0) {
		ಅಕ್ಷರ *ತಲೆ = xstrfmt("%s/HEAD", sm_gitdir);
		unlink(ತಲೆ);
		ಬಿಡುಗಡೆ(ತಲೆ);
		die(_("refusing to create/use '%s' in another submodule's git dir. "
		      "Enabling extensions.submodulePathConfig should fix this."),
		    sm_gitdir);
	}

	connect_work_ವೃಕ್ಷ_and_git_dir(clone_ದತ್ತಾಂಶ_ಮಾರ್ಗ, sm_gitdir, 0);

	p = repo_submodule_ಮಾರ್ಗ(the_repository, clone_ದತ್ತಾಂಶ_ಮಾರ್ಗ, "config");
	ಆದರೆ (!p)
		die(_("could not get submodule directory for '%s'"), clone_ದತ್ತಾಂಶ_ಮಾರ್ಗ);

	/* setup alternateLocation and alternateErrorStrategy in the cloned submodule if needed */
	repo_config_get_string(the_repository, "submodule.alternateLocation", &sm_alternate);
	ಆದರೆ (sm_alternate)
		repo_config_ಗಣ_ಒಳ_file(the_repository, p, "submodule.alternateLocation",
					sm_alternate);
	repo_config_get_string(the_repository, "submodule.alternateErrorStrategy", &error_strategy);
	ಆದರೆ (error_strategy)
		repo_config_ಗಣ_ಒಳ_file(the_repository, p, "submodule.alternateErrorStrategy",
					error_strategy);

	ಬಿಡುಗಡೆ(sm_alternate);
	ಬಿಡುಗಡೆ(error_strategy);

	ಬಿಡುಗಡೆ(sm_gitdir);
	ಬಿಡುಗಡೆ(p);
	ಬಿಡುಗಡೆ(ಗಮ್ಯ_free);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ module_clone(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			ರಚನೆ repository *repo ಬಳಸದ)
{
	ಪೂರ್ಣಾಂಕ dissociate = 0, quiet = 0, ಪ್ರಗತಿ = 0, require_init = 0;
	ರಚನೆ module_clone_ದತ್ತಾಂಶ clone_ದತ್ತಾಂಶ = MODULE_CLONE_ದತ್ತಾಂಶ_INIT;
	ರಚನೆ string_list reference = STRING_LIST_INIT_NODUP;
	ರಚನೆ list_objects_filter_options filter_options =
		LIST_OBJECTS_FILTER_INIT;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ref_storage_format = ಖಾಲಿ;

	ರಚನೆ option module_clone_options[] = {
		OPT_STRING(0, "prefix", &clone_ದತ್ತಾಂಶ.ಪೂರ್ವಪ್ರತ್ಯಯ,
			   N_("path"),
			   N_("alternative anchor for relative paths")),
		OPT_STRING(0, "path", &clone_ದತ್ತಾಂಶ.ಮಾರ್ಗ,
			   N_("path"),
			   N_("where the new submodule will be cloned to")),
		OPT_STRING(0, "name", &clone_ದತ್ತಾಂಶ.name,
			   N_("string"),
			   N_("name of the new submodule")),
		OPT_STRING(0, "url", &clone_ದತ್ತಾಂಶ.url,
			   N_("string"),
			   N_("url where to clone the submodule from")),
		OPT_STRING_LIST(0, "reference", &reference,
			   N_("repo"),
			   N_("reference repository")),
		OPT_STRING(0, "ref-storage-format", &ref_storage_format, N_("format"),
			   N_("specify the reference storage format to use")),
		OPT_ALIAS_F(0, "ref-format", "ref-storage-format", PARSE_OPT_HIDDEN),
		OPT_BOOL(0, "dissociate", &dissociate,
			   N_("use --reference only while cloning")),
		OPT_INTEGER(0, "depth", &clone_ದತ್ತಾಂಶ.ಆಳ,
			   N_("depth for shallow clones")),
		OPT__QUIET(&quiet, "suppress output for cloning a submodule"),
		OPT_BOOL(0, "progress", &ಪ್ರಗತಿ,
			   N_("force cloning progress")),
		OPT_BOOL(0, "require-init", &require_init,
			   N_("disallow cloning into non-empty directory")),
		OPT_BOOL(0, "single-branch", &clone_ದತ್ತಾಂಶ.single_branch,
			 N_("clone only one branch, HEAD or --branch")),
		OPT_PARSE_LIST_OBJECTS_FILTER(&filter_options),
		OPT_ಕೊನೆ()
	};
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ git_submodule_helper_usage[] = {
		N_("git submodule--helper clone [--prefix=<path>] [--quiet] "
		   "[--reference <repository>] [--name <name>] [--depth <depth>] "
		   "[--single-branch] [--filter <filter-spec>] "
		   "--url <url> --path <path>"),
		ಖಾಲಿ
	};

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, module_clone_options,
			     git_submodule_helper_usage, 0);

	ಆದರೆ (ref_storage_format) {
		clone_ದತ್ತಾಂಶ.ref_storage_format = ref_storage_format_by_name(ref_storage_format);
		ಆದರೆ (clone_ದತ್ತಾಂಶ.ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format);
	}
	clone_ದತ್ತಾಂಶ.dissociate = !!dissociate;
	clone_ದತ್ತಾಂಶ.quiet = !!quiet;
	clone_ದತ್ತಾಂಶ.ಪ್ರಗತಿ = !!ಪ್ರಗತಿ;
	clone_ದತ್ತಾಂಶ.require_init = !!require_init;
	clone_ದತ್ತಾಂಶ.filter_options = &filter_options;

	ಆದರೆ (argc || !clone_ದತ್ತಾಂಶ.url || !clone_ದತ್ತಾಂಶ.ಮಾರ್ಗ || !*(clone_ದತ್ತಾಂಶ.ಮಾರ್ಗ))
		usage_with_options(git_submodule_helper_usage,
				   module_clone_options);

	clone_submodule(&clone_ದತ್ತಾಂಶ, &reference);
	list_objects_filter_release(&filter_options);
	string_list_clear(&reference, 1);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ determine_submodule_update_strategy(ರಚನೆ repository *r,
					       ಪೂರ್ಣಾಂಕ just_cloned,
					       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ,
					       ಎಣಿಕೆ submodule_update_type update,
					       ರಚನೆ submodule_update_strategy *ಹೊರಹೋಗು)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ submodule *sub = submodule_from_ಮಾರ್ಗ(r, null_oid(the_hash_algo), ಮಾರ್ಗ);
	ಅಕ್ಷರ *ಕೀಲಿ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *val;
	ಪೂರ್ಣಾಂಕ ret;

	/*
	 * NEEDSWORK: audit and ensure that update_submodule() has right
	 * to assume that submodule_from_path() above will always succeed.
	 */
	ಆದರೆ (!sub)
		BUG("update_submodule assumes a submodule exists at path (%s)",
		    ಮಾರ್ಗ);
	ಕೀಲಿ = xstrfmt("submodule.%s.update", sub->name);

	ಆದರೆ (update) {
		ಹೊರಹೋಗು->type = update;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!repo_config_get_string_tmp(r, ಕೀಲಿ, &val)) {
		ಆದರೆ (parse_submodule_update_strategy(val, ಹೊರಹೋಗು) < 0) {
			ret = die_ಸಂದೇಶ(_("Invalid update mode '%s' configured for submodule path '%s'"),
					  val, ಮಾರ್ಗ);
			ಹೋಗು cleanup;
		}
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (sub->update_strategy.type != SM_UPDATE_UNSPECIFIED) {
		ಆದರೆ (sub->update_strategy.type == SM_UPDATE_COMMAND)
			BUG("how did we read update = !command from .gitmodules?");
		ಹೊರಹೋಗು->type = sub->update_strategy.type;
		ಹೊರಹೋಗು->command = sub->update_strategy.command;
	} ಇಲ್ಲದಿದ್ದರೆ
		ಹೊರಹೋಗು->type = SM_UPDATE_CHECKOUT;

	ಆದರೆ (just_cloned &&
	    (ಹೊರಹೋಗು->type == SM_UPDATE_ವಿಲೀನ ||
	     ಹೊರಹೋಗು->type == SM_UPDATE_REBASE ||
	     ಹೊರಹೋಗು->type == SM_UPDATE_NONE))
		ಹೊರಹೋಗು->type = SM_UPDATE_CHECKOUT;

	ret = 0;
cleanup:
	ಬಿಡುಗಡೆ(ಕೀಲಿ);
	ಹಿಂತಿರುಗಿ ret;
}

ರಚನೆ update_clone_ದತ್ತಾಂಶ {
	ಸ್ಥಿರಾಂಕ ರಚನೆ submodule *sub;
	ರಚನೆ object_id oid;
	ಚಿಹ್ನೆರಹಿತ just_cloned;
};

ರಚನೆ submodule_update_clone {
	/* index into 'update_data.list', the list of submodules to look into for cloning */
	ಪೂರ್ಣಾಂಕ current;

	/* configuration parameters which are passed on to the children */
	ಸ್ಥಿರಾಂಕ ರಚನೆ update_ದತ್ತಾಂಶ *update_ದತ್ತಾಂಶ;

	/* to be consumed by update_submodule() */
	ರಚನೆ update_clone_ದತ್ತಾಂಶ *update_clone;
	ಪೂರ್ಣಾಂಕ update_clone_nr; ಪೂರ್ಣಾಂಕ update_clone_alloc;

	/* If we want to stop as fast as possible and return an error */
	ಚಿಹ್ನೆರಹಿತ quickstop : 1;

	/* failed clones to be retried again */
	ಸ್ಥಿರಾಂಕ ರಚನೆ cache_ನಮೂದು **failed_clones;
	ಪೂರ್ಣಾಂಕ failed_clones_nr, failed_clones_alloc;
};
#ವ್ಯಾಖ್ಯಾನಿಸು SUBMODULE_UPDATE_CLONE_INIT { 0 }

ಸ್ಥಿರ ಶೂನ್ಯ submodule_update_clone_release(ರಚನೆ submodule_update_clone *suc)
{
	ಬಿಡುಗಡೆ(suc->update_clone);
	ಬಿಡುಗಡೆ(suc->failed_clones);
}

ರಚನೆ update_ದತ್ತಾಂಶ {
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *super_ಪೂರ್ವಪ್ರತ್ಯಯ;
	ಅಕ್ಷರ *displaypath;
	ಎಣಿಕೆ submodule_update_type update_default;
	ರಚನೆ object_id suboid;
	ರಚನೆ string_list references;
	ರಚನೆ submodule_update_strategy update_strategy;
	ರಚನೆ list_objects_filter_options *filter_options;
	ರಚನೆ module_list list;
	ಎಣಿಕೆ ref_storage_format ref_storage_format;
	ಪೂರ್ಣಾಂಕ ಆಳ;
	ಪೂರ್ಣಾಂಕ max_jobs;
	ಪೂರ್ಣಾಂಕ single_branch;
	ಪೂರ್ಣಾಂಕ recommend_shallow;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ require_init;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ force;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ quiet;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ nofetch;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ remote;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಪ್ರಗತಿ;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ dissociate;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ init;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ warn_if_uninitialized;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ recursive;

	/* copied over from update_clone_data */
	ರಚನೆ object_id oid;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ just_cloned;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *sm_ಮಾರ್ಗ;
};
#ವ್ಯಾಖ್ಯಾನಿಸು UPDATE_ದತ್ತಾಂಶ_INIT { \
	.update_strategy = SUBMODULE_UPDATE_STRATEGY_INIT, \
	.list = MODULE_LIST_INIT, \
	.ref_storage_format = REF_STORAGE_FORMAT_UNKNOWN, \
	.recommend_shallow = -1, \
	.references = STRING_LIST_INIT_DUP, \
	.single_branch = -1, \
	.max_jobs = 1, \
}

ಸ್ಥಿರ ಶೂನ್ಯ update_ದತ್ತಾಂಶ_release(ರಚನೆ update_ದತ್ತಾಂಶ *ud)
{
	ಬಿಡುಗಡೆ(ud->displaypath);
	submodule_update_strategy_release(&ud->update_strategy);
	module_list_release(&ud->list);
}

ಸ್ಥಿರ ಶೂನ್ಯ ಮುಂದಿನ_submodule_warn_missing(ರಚನೆ submodule_update_clone *suc,
		ರಚನೆ strbuf *ಹೊರಹೋಗು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *displaypath)
{
	/*
	 * Only mention uninitialized submodules when their
	 * paths have been specified.
	 */
	ಆದರೆ (suc->update_ದತ್ತಾಂಶ->warn_if_uninitialized) {
		strbuf_addf(ಹೊರಹೋಗು,
			_("Submodule path '%s' not initialized"),
			displaypath);
		strbuf_addch(ಹೊರಹೋಗು, '\n');
		strbuf_addstr(ಹೊರಹೋಗು,
			_("Maybe you want to use 'update --init'?"));
		strbuf_addch(ಹೊರಹೋಗು, '\n');
	}
}

/**
 * Determine whether 'ce' needs to be cloned. If so, prepare the 'child' to
 * run the clone. Returns 1 if 'ce' needs to be cloned, 0 otherwise.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ prepare_ಗಮ್ಯ_clone_ಮುಂದಿನ_submodule(ಸ್ಥಿರಾಂಕ ರಚನೆ cache_ನಮೂದು *ce,
					   ರಚನೆ ಮಗು_process *ಮಗು,
					   ರಚನೆ submodule_update_clone *suc,
					   ರಚನೆ strbuf *ಹೊರಹೋಗು)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ submodule *sub = ಖಾಲಿ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *url = ಖಾಲಿ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *update_string;
	ಎಣಿಕೆ submodule_update_type update_type;
	ಅಕ್ಷರ *ಕೀಲಿ;
	ಸ್ಥಿರಾಂಕ ರಚನೆ update_ದತ್ತಾಂಶ *ud = suc->update_ದತ್ತಾಂಶ;
	ಅಕ್ಷರ *displaypath = get_submodule_displaypath(ce->name, ud->ಪೂರ್ವಪ್ರತ್ಯಯ,
						      ud->super_ಪೂರ್ವಪ್ರತ್ಯಯ);
	ರಚನೆ strbuf sb = STRBUF_INIT;
	ಪೂರ್ಣಾಂಕ needs_cloning = 0;
	ಪೂರ್ಣಾಂಕ need_free_url = 0;

	ಆದರೆ (ce_stage(ce)) {
		strbuf_addf(ಹೊರಹೋಗು, _("Skipping unmerged submodule %s"), displaypath);
		strbuf_addch(ಹೊರಹೋಗು, '\n');
		ಹೋಗು cleanup;
	}

	sub = submodule_from_ಮಾರ್ಗ(the_repository, null_oid(the_hash_algo), ce->name);

	ಆದರೆ (!sub) {
		ಮುಂದಿನ_submodule_warn_missing(suc, ಹೊರಹೋಗು, displaypath);
		ಹೋಗು cleanup;
	}

	ಕೀಲಿ = xstrfmt("submodule.%s.update", sub->name);
	ಆದರೆ (!repo_config_get_string_tmp(the_repository, ಕೀಲಿ, &update_string)) {
		update_type = parse_submodule_update_type(update_string);
	} ಇಲ್ಲದಿದ್ದರೆ {
		update_type = sub->update_strategy.type;
	}
	ಬಿಡುಗಡೆ(ಕೀಲಿ);

	ಆದರೆ (suc->update_ದತ್ತಾಂಶ->update_strategy.type == SM_UPDATE_NONE
	    || (suc->update_ದತ್ತಾಂಶ->update_strategy.type == SM_UPDATE_UNSPECIFIED
		&& update_type == SM_UPDATE_NONE)) {
		strbuf_addf(ಹೊರಹೋಗು, _("Skipping submodule '%s'"), displaypath);
		strbuf_addch(ಹೊರಹೋಗು, '\n');
		ಹೋಗು cleanup;
	}

	/* Check if the submodule has been initialized. */
	ಆದರೆ (!is_submodule_ಸಕ್ರಿಯ(the_repository, ce->name)) {
		ಮುಂದಿನ_submodule_warn_missing(suc, ಹೊರಹೋಗು, displaypath);
		ಹೋಗು cleanup;
	}

	strbuf_reset(&sb);
	strbuf_addf(&sb, "submodule.%s.url", sub->name);
	ಆದರೆ (repo_config_get_string_tmp(the_repository, sb.buf, &url)) {
		ಆದರೆ (sub->url && (starts_with_dot_slash(sub->url) ||
				 starts_with_dot_dot_slash(sub->url))) {
			url = resolve_relative_url(sub->url, ಖಾಲಿ, 0);
			need_free_url = 1;
		} ಇಲ್ಲದಿದ್ದರೆ
			url = sub->url;
	}

	ಆದರೆ (!url)
		die(_("cannot clone submodule '%s' without a URL"), sub->name);

	strbuf_reset(&sb);
	strbuf_addf(&sb, "%s/.git", ce->name);
	needs_cloning = !file_exists(sb.buf);

	ALLOC_GROW(suc->update_clone, suc->update_clone_nr + 1,
		   suc->update_clone_alloc);
	oidcpy(&suc->update_clone[suc->update_clone_nr].oid, &ce->oid);
	suc->update_clone[suc->update_clone_nr].just_cloned = needs_cloning;
	suc->update_clone[suc->update_clone_nr].sub = sub;
	suc->update_clone_nr++;

	ಆದರೆ (!needs_cloning)
		ಹೋಗು cleanup;

	ಮಗು->git_cmd = 1;
	ಮಗು->no_stdin = 1;
	ಮಗು->stdout_ಗಮ್ಯ_stderr = 1;
	ಮಗು->err = -1;
	strvec_ತಳ್ಳು(&ಮಗು->ವಾದಗಳು, "submodule--helper");
	strvec_ತಳ್ಳು(&ಮಗು->ವಾದಗಳು, "clone");
	ಆದರೆ (suc->update_ದತ್ತಾಂಶ->ಪ್ರಗತಿ)
		strvec_ತಳ್ಳು(&ಮಗು->ವಾದಗಳು, "--progress");
	ಆದರೆ (suc->update_ದತ್ತಾಂಶ->quiet)
		strvec_ತಳ್ಳು(&ಮಗು->ವಾದಗಳು, "--quiet");
	ಆದರೆ (suc->update_ದತ್ತಾಂಶ->ಪೂರ್ವಪ್ರತ್ಯಯ)
		strvec_pushl(&ಮಗು->ವಾದಗಳು, "--prefix", suc->update_ದತ್ತಾಂಶ->ಪೂರ್ವಪ್ರತ್ಯಯ, ಖಾಲಿ);
	ಆದರೆ (suc->update_ದತ್ತಾಂಶ->recommend_shallow && sub->recommend_shallow == 1)
		strvec_ತಳ್ಳು(&ಮಗು->ವಾದಗಳು, "--depth=1");
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (suc->update_ದತ್ತಾಂಶ->ಆಳ)
		strvec_pushf(&ಮಗು->ವಾದಗಳು, "--depth=%d", suc->update_ದತ್ತಾಂಶ->ಆಳ);
	ಆದರೆ (suc->update_ದತ್ತಾಂಶ->filter_options && suc->update_ದತ್ತಾಂಶ->filter_options->choice)
		strvec_pushf(&ಮಗು->ವಾದಗಳು, "--filter=%s",
			     expand_list_objects_filter_spec(suc->update_ದತ್ತಾಂಶ->filter_options));
	ಆದರೆ (suc->update_ದತ್ತಾಂಶ->require_init)
		strvec_ತಳ್ಳು(&ಮಗು->ವಾದಗಳು, "--require-init");
	ಆದರೆ (suc->update_ದತ್ತಾಂಶ->ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
		strvec_pushf(&ಮಗು->ವಾದಗಳು, "--ref-storage-format=%s",
			     ref_storage_format_ಗಮ್ಯ_name(suc->update_ದತ್ತಾಂಶ->ref_storage_format));
	strvec_pushl(&ಮಗು->ವಾದಗಳು, "--path", sub->ಮಾರ್ಗ, ಖಾಲಿ);
	strvec_pushl(&ಮಗು->ವಾದಗಳು, "--name", sub->name, ಖಾಲಿ);
	strvec_pushl(&ಮಗು->ವಾದಗಳು, "--url", url, ಖಾಲಿ);
	ಆದರೆ (suc->update_ದತ್ತಾಂಶ->references.nr) {
		ರಚನೆ string_list_ಅಂಶ *ಅಂಶ;

		for_each_string_list_ಅಂಶ(ಅಂಶ, &suc->update_ದತ್ತಾಂಶ->references)
			strvec_pushl(&ಮಗು->ವಾದಗಳು, "--reference", ಅಂಶ->string, ಖಾಲಿ);
	}
	ಆದರೆ (suc->update_ದತ್ತಾಂಶ->dissociate)
		strvec_ತಳ್ಳು(&ಮಗು->ವಾದಗಳು, "--dissociate");
	ಆದರೆ (suc->update_ದತ್ತಾಂಶ->single_branch >= 0)
		strvec_ತಳ್ಳು(&ಮಗು->ವಾದಗಳು, suc->update_ದತ್ತಾಂಶ->single_branch ?
					      "--single-branch" :
					      "--no-single-branch");

cleanup:
	ಬಿಡುಗಡೆ(displaypath);
	strbuf_release(&sb);
	ಆದರೆ (need_free_url)
		ಬಿಡುಗಡೆ((ಶೂನ್ಯ*)url);

	ಹಿಂತಿರುಗಿ needs_cloning;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ update_clone_get_ಮುಂದಿನ_task(ರಚನೆ ಮಗು_process *ಮಗು,
				      ರಚನೆ strbuf *err,
				      ಶೂನ್ಯ *suc_cb,
				      ಶೂನ್ಯ **idx_task_cb)
{
	ರಚನೆ submodule_update_clone *suc = suc_cb;
	ಸ್ಥಿರಾಂಕ ರಚನೆ cache_ನಮೂದು *ce;
	ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ;

	ಗಾಗಿ (; suc->current < suc->update_ದತ್ತಾಂಶ->list.nr; suc->current++) {
		ce = suc->update_ದತ್ತಾಂಶ->list.entries[suc->current];
		ಆದರೆ (prepare_ಗಮ್ಯ_clone_ಮುಂದಿನ_submodule(ce, ಮಗು, suc, err)) {
			ಪೂರ್ಣಾಂಕ *p = xmalloc(ಗಾತ್ರ(*p));

			*p = suc->current;
			*idx_task_cb = p;
			suc->current++;
			ಹಿಂತಿರುಗಿ 1;
		}
	}

	/*
	 * The loop above tried cloning each submodule once, now try the
	 * stragglers again, which we can imagine as an extension of the
	 * entry list.
	 */
	ಸೂಚ್ಯಂಕ = suc->current - suc->update_ದತ್ತಾಂಶ->list.nr;
	ಆದರೆ (ಸೂಚ್ಯಂಕ < suc->failed_clones_nr) {
		ಪೂರ್ಣಾಂಕ *p;

		ce = suc->failed_clones[ಸೂಚ್ಯಂಕ];
		ಆದರೆ (!prepare_ಗಮ್ಯ_clone_ಮುಂದಿನ_submodule(ce, ಮಗು, suc, err)) {
			suc->current ++;
			strbuf_addstr(err, "BUG: submodule considered for "
					   "cloning, doesn't need cloning "
					   "any more?\n");
			ಹಿಂತಿರುಗಿ 0;
		}
		p = xmalloc(ಗಾತ್ರ(*p));
		*p = suc->current;
		*idx_task_cb = p;
		suc->current ++;
		ಹಿಂತಿರುಗಿ 1;
	}

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ update_clone_ಆರಂಭ_failure(ರಚನೆ strbuf *err ಬಳಸದ,
				      ಶೂನ್ಯ *suc_cb,
				      ಶೂನ್ಯ *idx_task_cb ಬಳಸದ)
{
	ರಚನೆ submodule_update_clone *suc = suc_cb;

	suc->quickstop = 1;
	ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ update_clone_task_ಪೂರ್ಣಗೊಂಡ(ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ,
				      ರಚನೆ strbuf *err,
				      ಶೂನ್ಯ *suc_cb,
				      ಶೂನ್ಯ *idx_task_cb)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ cache_ನಮೂದು *ce;
	ರಚನೆ submodule_update_clone *suc = suc_cb;
	ಪೂರ್ಣಾಂಕ *idxP = idx_task_cb;
	ಪೂರ್ಣಾಂಕ idx = *idxP;

	ಬಿಡುಗಡೆ(idxP);

	ಆದರೆ (!ಫಲಿತಾಂಶ)
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (idx < suc->update_ದತ್ತಾಂಶ->list.nr) {
		ce  = suc->update_ದತ್ತಾಂಶ->list.entries[idx];
		strbuf_addf(err, _("Failed to clone '%s'. Retry scheduled"),
			    ce->name);
		strbuf_addch(err, '\n');
		ALLOC_GROW(suc->failed_clones,
			   suc->failed_clones_nr + 1,
			   suc->failed_clones_alloc);
		suc->failed_clones[suc->failed_clones_nr++] = ce;
		ಹಿಂತಿರುಗಿ 0;
	} ಇಲ್ಲದಿದ್ದರೆ {
		idx -= suc->update_ದತ್ತಾಂಶ->list.nr;
		ce  = suc->failed_clones[idx];
		strbuf_addf(err, _("Failed to clone '%s' a second time, aborting"),
			    ce->name);
		strbuf_addch(err, '\n');
		suc->quickstop = 1;
		ಹಿಂತಿರುಗಿ 1;
	}

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ git_update_clone_config(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *var, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ,
				   ಸ್ಥಿರಾಂಕ ರಚನೆ config_context *ctx,
				   ಶೂನ್ಯ *cb)
{
	ಪೂರ್ಣಾಂಕ *max_jobs = cb;

	ಆದರೆ (!strcmp(var, "submodule.fetchjobs"))
		*max_jobs = parse_submodule_fetchjobs(var, ಮೌಲ್ಯ, ctx->kvi);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ is_tip_reachable(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid)
{
	ರಚನೆ ಮಗು_process cp = ಮಗು_PROCESS_INIT;
	ರಚನೆ strbuf rev = STRBUF_INIT;
	ಅಕ್ಷರ *hex = oid_ಗಮ್ಯ_hex(oid);
	ಪೂರ್ಣಾಂಕ reachable;

	cp.git_cmd = 1;
	cp.dir = ಮಾರ್ಗ;
	cp.no_stderr = 1;
	strvec_pushl(&cp.ವಾದಗಳು, "rev-list", "-n", "1", hex, "--not", "--all", ಖಾಲಿ);

	prepare_submodule_repo_env(&cp.env);

	ಆದರೆ (capture_command(&cp, &rev, GIT_MAX_HEXSZ + 1) || rev.len)
		reachable = 0;
	ಇಲ್ಲದಿದ್ದರೆ
		reachable = 1;

	strbuf_release(&rev);
	ಹಿಂತಿರುಗಿ reachable;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ fetch_ಒಳ_submodule(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *module_ಮಾರ್ಗ, ಪೂರ್ಣಾಂಕ ಆಳ, ಪೂರ್ಣಾಂಕ quiet,
			      ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid)
{
	ರಚನೆ ಮಗು_process cp = ಮಗು_PROCESS_INIT;

	prepare_submodule_repo_env(&cp.env);
	cp.git_cmd = 1;
	cp.dir = module_ಮಾರ್ಗ;

	strvec_ತಳ್ಳು(&cp.ವಾದಗಳು, "fetch");
	ಆದರೆ (quiet)
		strvec_ತಳ್ಳು(&cp.ವಾದಗಳು, "--quiet");
	ಆದರೆ (ಆಳ)
		strvec_pushf(&cp.ವಾದಗಳು, "--depth=%d", ಆಳ);
	ಆದರೆ (oid) {
		ಅಕ್ಷರ *hex = oid_ಗಮ್ಯ_hex(oid);
		ಅಕ್ಷರ *remote;
		ಪೂರ್ಣಾಂಕ ಸಂಕೇತ_2;

		ಸಂಕೇತ_2 = get_default_remote_submodule(module_ಮಾರ್ಗ, &remote);
		ಆದರೆ (ಸಂಕೇತ_2) {
			ಮಗು_process_clear(&cp);
			ಹಿಂತಿರುಗಿ ಸಂಕೇತ_2;
		}

		strvec_pushl(&cp.ವಾದಗಳು, remote, hex, ಖಾಲಿ);
		ಬಿಡುಗಡೆ(remote);
	}

	ಹಿಂತಿರುಗಿ ಸತತ_command(&cp);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಸತತ_update_command(ಸ್ಥಿರಾಂಕ ರಚನೆ update_ದತ್ತಾಂಶ *ud, ಪೂರ್ಣಾಂಕ subforce)
{
	ರಚನೆ ಮಗು_process cp = ಮಗು_PROCESS_INIT;
	ಅಕ್ಷರ *oid = oid_ಗಮ್ಯ_hex(&ud->oid);
	ಪೂರ್ಣಾಂಕ ret;

	ಆಯ್ಕೆ (ud->update_strategy.type) {
	ಸ್ಥಿತಿ SM_UPDATE_CHECKOUT:
		cp.git_cmd = 1;
		strvec_pushl(&cp.ವಾದಗಳು, "checkout", "-q", ಖಾಲಿ);
		ಆದರೆ (subforce)
			strvec_ತಳ್ಳು(&cp.ವಾದಗಳು, "-f");
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ SM_UPDATE_REBASE:
		cp.git_cmd = 1;
		strvec_ತಳ್ಳು(&cp.ವಾದಗಳು, "rebase");
		ಆದರೆ (ud->quiet)
			strvec_ತಳ್ಳು(&cp.ವಾದಗಳು, "--quiet");
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ SM_UPDATE_ವಿಲೀನ:
		cp.git_cmd = 1;
		strvec_ತಳ್ಳು(&cp.ವಾದಗಳು, "merge");
		ಆದರೆ (ud->quiet)
			strvec_ತಳ್ಳು(&cp.ವಾದಗಳು, "--quiet");
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ SM_UPDATE_COMMAND:
		cp.use_shell = 1;
		strvec_ತಳ್ಳು(&cp.ವಾದಗಳು, ud->update_strategy.command);
		ನಿಲ್ಲಿಸು;
	ಪೂರ್ವನಿಯೋಜಿತ:
		BUG("unexpected update strategy type: %d",
		    ud->update_strategy.type);
	}
	strvec_ತಳ್ಳು(&cp.ವಾದಗಳು, oid);

	cp.dir = ud->sm_ಮಾರ್ಗ;
	prepare_submodule_repo_env(&cp.env);
	ಆದರೆ ((ret = ಸತತ_command(&cp))) {
		ಆಯ್ಕೆ (ud->update_strategy.type) {
		ಸ್ಥಿತಿ SM_UPDATE_CHECKOUT:
			die_ಸಂದೇಶ(_("Unable to checkout '%s' in submodule path '%s'"),
				    oid, ud->displaypath);
			/* No "ret" assignment, use "git checkout"'s */
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ SM_UPDATE_REBASE:
			ret = die_ಸಂದೇಶ(_("Unable to rebase '%s' in submodule path '%s'"),
					  oid, ud->displaypath);
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ SM_UPDATE_ವಿಲೀನ:
			ret = die_ಸಂದೇಶ(_("Unable to merge '%s' in submodule path '%s'"),
					  oid, ud->displaypath);
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ SM_UPDATE_COMMAND:
			ret = die_ಸಂದೇಶ(_("Execution of '%s %s' failed in submodule path '%s'"),
					  ud->update_strategy.command, oid, ud->displaypath);
			ನಿಲ್ಲಿಸು;
		ಪೂರ್ವನಿಯೋಜಿತ:
			BUG("unexpected update strategy type: %d",
			    ud->update_strategy.type);
		}

		ಹಿಂತಿರುಗಿ ret;
	}

	ಆದರೆ (ud->quiet)
		ಹಿಂತಿರುಗಿ 0;

	ಆಯ್ಕೆ (ud->update_strategy.type) {
	ಸ್ಥಿತಿ SM_UPDATE_CHECKOUT:
		ಬರೆ(_("Submodule path '%s': checked out '%s'\n"),
		       ud->displaypath, oid);
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ SM_UPDATE_REBASE:
		ಬರೆ(_("Submodule path '%s': rebased into '%s'\n"),
		       ud->displaypath, oid);
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ SM_UPDATE_ವಿಲೀನ:
		ಬರೆ(_("Submodule path '%s': merged in '%s'\n"),
		       ud->displaypath, oid);
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ SM_UPDATE_COMMAND:
		ಬರೆ(_("Submodule path '%s': '%s %s'\n"),
		       ud->displaypath, ud->update_strategy.command, oid);
		ನಿಲ್ಲಿಸು;
	ಪೂರ್ವನಿಯೋಜಿತ:
		BUG("unexpected update strategy type: %d",
		    ud->update_strategy.type);
	}

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಸತತ_update_procedure(ಸ್ಥಿರಾಂಕ ರಚನೆ update_ದತ್ತಾಂಶ *ud)
{
	ಪೂರ್ಣಾಂಕ subforce = is_null_oid(&ud->suboid) || ud->force;

	ಆದರೆ (!ud->nofetch) {
		/*
		 * Run fetch only if `oid` isn't present or it
		 * is not reachable from a ref.
		 */
		ಆದರೆ (!is_tip_reachable(ud->sm_ಮಾರ್ಗ, &ud->oid) &&
		    fetch_ಒಳ_submodule(ud->sm_ಮಾರ್ಗ, ud->ಆಳ, ud->quiet, ಖಾಲಿ) &&
		    !ud->quiet)
			fprintf_ln(stderr,
				   _("Unable to fetch in submodule path '%s'; "
				     "trying to directly fetch %s:"),
				   ud->displaypath, oid_ಗಮ್ಯ_hex(&ud->oid));
		/*
		 * Now we tried the usual fetch, but `oid` may
		 * not be reachable from any of the refs.
		 */
		ಆದರೆ (!is_tip_reachable(ud->sm_ಮಾರ್ಗ, &ud->oid) &&
		    fetch_ಒಳ_submodule(ud->sm_ಮಾರ್ಗ, ud->ಆಳ, ud->quiet, &ud->oid))
			ಹಿಂತಿರುಗಿ die_ಸಂದೇಶ(_("Fetched in submodule path '%s', but it did not "
					     "contain %s. Direct fetching of that commit failed."),
					   ud->displaypath, oid_ಗಮ್ಯ_hex(&ud->oid));
	}

	ಹಿಂತಿರುಗಿ ಸತತ_update_command(ud, subforce);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ remote_submodule_branch(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **branch)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ submodule *sub;
	ಅಕ್ಷರ *ಕೀಲಿ;
	*branch = ಖಾಲಿ;

	sub = submodule_from_ಮಾರ್ಗ(the_repository, null_oid(the_hash_algo), ಮಾರ್ಗ);
	ಆದರೆ (!sub)
		ಹಿಂತಿರುಗಿ die_ಸಂದೇಶ(_("could not initialize submodule at path '%s'"),
				   ಮಾರ್ಗ);

	ಕೀಲಿ = xstrfmt("submodule.%s.branch", sub->name);
	ಆದರೆ (repo_config_get_string_tmp(the_repository, ಕೀಲಿ, branch))
		*branch = sub->branch;
	ಬಿಡುಗಡೆ(ಕೀಲಿ);

	ಆದರೆ (!*branch) {
		*branch = "HEAD";
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (!strcmp(*branch, ".")) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *refname = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
							      "HEAD", 0, ಖಾಲಿ,
							      ಖಾಲಿ);

		ಆದರೆ (!refname)
			ಹಿಂತಿರುಗಿ die_ಸಂದೇಶ(_("No such ref: %s"), "HEAD");

		/* detached HEAD */
		ಆದರೆ (!strcmp(refname, "HEAD"))
			ಹಿಂತಿರುಗಿ die_ಸಂದೇಶ(_("Submodule (%s) branch configured to inherit "
					     "branch from superproject, but the superproject "
					     "is not on any branch"), sub->name);

		ಆದರೆ (!skip_ಪೂರ್ವಪ್ರತ್ಯಯ(refname, "refs/heads/", &refname))
			ಹಿಂತಿರುಗಿ die_ಸಂದೇಶ(_("Expecting a full ref name, got %s"),
					   refname);

		*branch = refname;
		ಹಿಂತಿರುಗಿ 0;
	}

	/* Our "branch" is coming from repo_config_get_string_tmp() */
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ensure_core_worktree(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cw;
	ರಚನೆ repository subrepo;

	ಆದರೆ (repo_submodule_init(&subrepo, the_repository, ಮಾರ್ಗ, null_oid(the_hash_algo)))
		ಹಿಂತಿರುಗಿ die_ಸಂದೇಶ(_("could not get a repository handle for submodule '%s'"),
				   ಮಾರ್ಗ);

	ಆದರೆ (!repo_config_get_string_tmp(&subrepo, "core.worktree", &cw)) {
		ಅಕ್ಷರ *cfg_file, *abs_ಮಾರ್ಗ;
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rel_ಮಾರ್ಗ;
		ರಚನೆ strbuf sb = STRBUF_INIT;

		cfg_file = repo_git_ಮಾರ್ಗ(&subrepo, "config");

		abs_ಮಾರ್ಗ = absolute_pathdup(ಮಾರ್ಗ);
		rel_ಮಾರ್ಗ = relative_ಮಾರ್ಗ(abs_ಮಾರ್ಗ, subrepo.gitdir, &sb);

		repo_config_ಗಣ_ಒಳ_file(the_repository, cfg_file, "core.worktree", rel_ಮಾರ್ಗ);

		ಬಿಡುಗಡೆ(cfg_file);
		ಬಿಡುಗಡೆ(abs_ಮಾರ್ಗ);
		strbuf_release(&sb);
	}

	repo_clear(&subrepo);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *submodule_update_type_ಗಮ್ಯ_label(ಎಣಿಕೆ submodule_update_type type)
{
	ಆಯ್ಕೆ (type) {
	ಸ್ಥಿತಿ SM_UPDATE_CHECKOUT:
		ಹಿಂತಿರುಗಿ "checkout";
	ಸ್ಥಿತಿ SM_UPDATE_ವಿಲೀನ:
		ಹಿಂತಿರುಗಿ "merge";
	ಸ್ಥಿತಿ SM_UPDATE_REBASE:
		ಹಿಂತಿರುಗಿ "rebase";
	ಸ್ಥಿತಿ SM_UPDATE_UNSPECIFIED:
	ಸ್ಥಿತಿ SM_UPDATE_NONE:
	ಸ್ಥಿತಿ SM_UPDATE_COMMAND:
		ನಿಲ್ಲಿಸು;
	}
	BUG("unreachable with type %d", type);
}

ಸ್ಥಿರ ಶೂನ್ಯ update_ದತ್ತಾಂಶ_ಗಮ್ಯ_ವಾದಗಳು(ಸ್ಥಿರಾಂಕ ರಚನೆ update_ದತ್ತಾಂಶ *update_ದತ್ತಾಂಶ,
				ರಚನೆ strvec *ವಾದಗಳು)
{
	ಎಣಿಕೆ submodule_update_type update_type = update_ದತ್ತಾಂಶ->update_default;

	strvec_pushl(ವಾದಗಳು, "submodule--helper", "update", "--recursive", ಖಾಲಿ);
	ಆದರೆ (update_ದತ್ತಾಂಶ->displaypath)
		strvec_pushf(ವಾದಗಳು, "--super-prefix=%s/",
			     update_ದತ್ತಾಂಶ->displaypath);
	strvec_pushf(ವಾದಗಳು, "--jobs=%d", update_ದತ್ತಾಂಶ->max_jobs);
	ಆದರೆ (update_ದತ್ತಾಂಶ->quiet)
		strvec_ತಳ್ಳು(ವಾದಗಳು, "--quiet");
	ಆದರೆ (update_ದತ್ತಾಂಶ->force)
		strvec_ತಳ್ಳು(ವಾದಗಳು, "--force");
	ಆದರೆ (update_ದತ್ತಾಂಶ->init)
		strvec_ತಳ್ಳು(ವಾದಗಳು, "--init");
	ಆದರೆ (update_ದತ್ತಾಂಶ->remote)
		strvec_ತಳ್ಳು(ವಾದಗಳು, "--remote");
	ಆದರೆ (update_ದತ್ತಾಂಶ->nofetch)
		strvec_ತಳ್ಳು(ವಾದಗಳು, "--no-fetch");
	ಆದರೆ (update_ದತ್ತಾಂಶ->dissociate)
		strvec_ತಳ್ಳು(ವಾದಗಳು, "--dissociate");
	ಆದರೆ (update_ದತ್ತಾಂಶ->ಪ್ರಗತಿ)
		strvec_ತಳ್ಳು(ವಾದಗಳು, "--progress");
	ಆದರೆ (update_ದತ್ತಾಂಶ->require_init)
		strvec_ತಳ್ಳು(ವಾದಗಳು, "--require-init");
	ಆದರೆ (update_ದತ್ತಾಂಶ->ಆಳ)
		strvec_pushf(ವಾದಗಳು, "--depth=%d", update_ದತ್ತಾಂಶ->ಆಳ);
	ಆದರೆ (update_type != SM_UPDATE_UNSPECIFIED)
		strvec_pushf(ವಾದಗಳು, "--%s",
			     submodule_update_type_ಗಮ್ಯ_label(update_type));

	ಆದರೆ (update_ದತ್ತಾಂಶ->references.nr) {
		ರಚನೆ string_list_ಅಂಶ *ಅಂಶ;

		for_each_string_list_ಅಂಶ(ಅಂಶ, &update_ದತ್ತಾಂಶ->references)
			strvec_pushl(ವಾದಗಳು, "--reference", ಅಂಶ->string, ಖಾಲಿ);
	}
	ಆದರೆ (update_ದತ್ತಾಂಶ->ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
		strvec_pushf(ವಾದಗಳು, "--ref-storage-format=%s",
			     ref_storage_format_ಗಮ್ಯ_name(update_ದತ್ತಾಂಶ->ref_storage_format));
	ಆದರೆ (update_ದತ್ತಾಂಶ->filter_options && update_ದತ್ತಾಂಶ->filter_options->choice)
		strvec_pushf(ವಾದಗಳು, "--filter=%s",
				expand_list_objects_filter_spec(
					update_ದತ್ತಾಂಶ->filter_options));
	ಆದರೆ (update_ದತ್ತಾಂಶ->recommend_shallow == 0)
		strvec_ತಳ್ಳು(ವಾದಗಳು, "--no-recommend-shallow");
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (update_ದತ್ತಾಂಶ->recommend_shallow == 1)
		strvec_ತಳ್ಳು(ವಾದಗಳು, "--recommend-shallow");
	ಆದರೆ (update_ದತ್ತಾಂಶ->single_branch >= 0)
		strvec_ತಳ್ಳು(ವಾದಗಳು, update_ದತ್ತಾಂಶ->single_branch ?
				    "--single-branch" :
				    "--no-single-branch");
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ update_submodule(ರಚನೆ update_ದತ್ತಾಂಶ *update_ದತ್ತಾಂಶ)
{
	ಪೂರ್ಣಾಂಕ ret;

	ಆದರೆ (validate_submodule_ಮಾರ್ಗ(update_ದತ್ತಾಂಶ->sm_ಮಾರ್ಗ) < 0)
		ಹಿಂತಿರುಗಿ -1;

	ret = determine_submodule_update_strategy(the_repository,
						  update_ದತ್ತಾಂಶ->just_cloned,
						  update_ದತ್ತಾಂಶ->sm_ಮಾರ್ಗ,
						  update_ದತ್ತಾಂಶ->update_default,
						  &update_ದತ್ತಾಂಶ->update_strategy);
	ಆದರೆ (ret)
		ಹಿಂತಿರುಗಿ ret;

	ಆದರೆ (update_ದತ್ತಾಂಶ->just_cloned)
		oidcpy(&update_ದತ್ತಾಂಶ->suboid, null_oid(the_hash_algo));
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (repo_resolve_gitlink_ref(the_repository, update_ದತ್ತಾಂಶ->sm_ಮಾರ್ಗ,
					  "HEAD", &update_ದತ್ತಾಂಶ->suboid))
		ಹಿಂತಿರುಗಿ die_ಸಂದೇಶ(_("Unable to find current revision in submodule path '%s'"),
				   update_ದತ್ತಾಂಶ->displaypath);

	ಆದರೆ (update_ದತ್ತಾಂಶ->remote) {
		ಅಕ್ಷರ *remote_name;
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *branch;
		ಅಕ್ಷರ *remote_ref;
		ಪೂರ್ಣಾಂಕ ಸಂಕೇತ_2;

		ಸಂಕೇತ_2 = get_default_remote_submodule(update_ದತ್ತಾಂಶ->sm_ಮಾರ್ಗ, &remote_name);
		ಆದರೆ (ಸಂಕೇತ_2)
			ಹಿಂತಿರುಗಿ ಸಂಕೇತ_2;
		ಸಂಕೇತ_2 = remote_submodule_branch(update_ದತ್ತಾಂಶ->sm_ಮಾರ್ಗ, &branch);
		ಆದರೆ (ಸಂಕೇತ_2) {
			ಬಿಡುಗಡೆ(remote_name);
			ಹಿಂತಿರುಗಿ ಸಂಕೇತ_2;
		}
		remote_ref = xstrfmt("refs/remotes/%s/%s", remote_name, branch);

		ಬಿಡುಗಡೆ(remote_name);

		ಆದರೆ (!update_ದತ್ತಾಂಶ->nofetch) {
			ಆದರೆ (fetch_ಒಳ_submodule(update_ದತ್ತಾಂಶ->sm_ಮಾರ್ಗ, update_ದತ್ತಾಂಶ->ಆಳ,
					      0, ಖಾಲಿ)) {
				ಬಿಡುಗಡೆ(remote_ref);
				ಹಿಂತಿರುಗಿ die_ಸಂದೇಶ(_("Unable to fetch in submodule path '%s'"),
						   update_ದತ್ತಾಂಶ->sm_ಮಾರ್ಗ);
			}
		}

		ಆದರೆ (repo_resolve_gitlink_ref(the_repository, update_ದತ್ತಾಂಶ->sm_ಮಾರ್ಗ,
					     remote_ref, &update_ದತ್ತಾಂಶ->oid)) {
			ret = die_ಸಂದೇಶ(_("Unable to find %s revision in submodule path '%s'"),
					  remote_ref, update_ದತ್ತಾಂಶ->sm_ಮಾರ್ಗ);
			ಬಿಡುಗಡೆ(remote_ref);
			ಹಿಂತಿರುಗಿ ret;
		}

		ಬಿಡುಗಡೆ(remote_ref);
	}

	ಆದರೆ (!oideq(&update_ದತ್ತಾಂಶ->oid, &update_ದತ್ತಾಂಶ->suboid) || update_ದತ್ತಾಂಶ->force) {
		ret = ಸತತ_update_procedure(update_ದತ್ತಾಂಶ);
		ಆದರೆ (ret)
			ಹಿಂತಿರುಗಿ ret;
	}

	ಆದರೆ (update_ದತ್ತಾಂಶ->recursive) {
		ರಚನೆ ಮಗು_process cp = ಮಗು_PROCESS_INIT;
		ರಚನೆ update_ದತ್ತಾಂಶ ಮುಂದಿನ = *update_ದತ್ತಾಂಶ;

		ಮುಂದಿನ.ಪೂರ್ವಪ್ರತ್ಯಯ = ಖಾಲಿ;
		oidcpy(&ಮುಂದಿನ.oid, null_oid(the_hash_algo));
		oidcpy(&ಮುಂದಿನ.suboid, null_oid(the_hash_algo));

		cp.dir = update_ದತ್ತಾಂಶ->sm_ಮಾರ್ಗ;
		cp.git_cmd = 1;
		prepare_submodule_repo_env(&cp.env);
		update_ದತ್ತಾಂಶ_ಗಮ್ಯ_ವಾದಗಳು(&ಮುಂದಿನ, &cp.ವಾದಗಳು);

		ret = ಸತತ_command(&cp);
		ಆದರೆ (ret)
			die_ಸಂದೇಶ(_("Failed to recurse into submodule path '%s'"),
				    update_ದತ್ತಾಂಶ->displaypath);
		ಹಿಂತಿರುಗಿ ret;
	}

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ update_submodules(ರಚನೆ update_ದತ್ತಾಂಶ *update_ದತ್ತಾಂಶ)
{
	ಪೂರ್ಣಾಂಕ i, ret = 0;
	ರಚನೆ submodule_update_clone suc = SUBMODULE_UPDATE_CLONE_INIT;
	ಸ್ಥಿರಾಂಕ ರಚನೆ ಸತತ_process_parallel_opts opts = {
		.tr2_category = "submodule",
		.tr2_label = "parallel/update",

		.processes = update_ದತ್ತಾಂಶ->max_jobs,

		.get_ಮುಂದಿನ_task = update_clone_get_ಮುಂದಿನ_task,
		.ಆರಂಭ_failure = update_clone_ಆರಂಭ_failure,
		.task_ಪೂರ್ಣಗೊಂಡ = update_clone_task_ಪೂರ್ಣಗೊಂಡ,
		.ದತ್ತಾಂಶ = &suc,
	};

	suc.update_ದತ್ತಾಂಶ = update_ದತ್ತಾಂಶ;
	ಸತತ_processes_parallel(&opts);

	/*
	 * We saved the output and put it out all at once now.
	 * That means:
	 * - the listener does not have to interleave their (checkout)
	 *   work with our fetching.  The writes involved in a
	 *   checkout involve more straightforward sequential I/O.
	 * - the listener can avoid doing any work if fetching failed.
	 */
	ಆದರೆ (suc.quickstop) {
		ret = 1;
		ಹೋಗು cleanup;
	}

	ಗಾಗಿ (i = 0; i < suc.update_clone_nr; i++) {
		ರಚನೆ update_clone_ದತ್ತಾಂಶ ucd = suc.update_clone[i];
		ಪೂರ್ಣಾಂಕ ಸಂಕೇತ_2 = 128;

		oidcpy(&update_ದತ್ತಾಂಶ->oid, &ucd.oid);
		update_ದತ್ತಾಂಶ->just_cloned = ucd.just_cloned;
		update_ದತ್ತಾಂಶ->sm_ಮಾರ್ಗ = ucd.sub->ಮಾರ್ಗ;

		/*
		 * Verify that the submodule path does not contain any
		 * symlinks; if it does, it might have been tampered with.
		 * TODO: allow exempting it via
		 * `safe.submodule.path` or something
		 */
		ಆದರೆ (validate_submodule_ಮಾರ್ಗ(update_ದತ್ತಾಂಶ->sm_ಮಾರ್ಗ) < 0)
			ಹೋಗು fail;

		ಸಂಕೇತ_2 = ensure_core_worktree(update_ದತ್ತಾಂಶ->sm_ಮಾರ್ಗ);
		ಆದರೆ (ಸಂಕೇತ_2)
			ಹೋಗು fail;

		update_ದತ್ತಾಂಶ->displaypath = get_submodule_displaypath(
			update_ದತ್ತಾಂಶ->sm_ಮಾರ್ಗ, update_ದತ್ತಾಂಶ->ಪೂರ್ವಪ್ರತ್ಯಯ,
			update_ದತ್ತಾಂಶ->super_ಪೂರ್ವಪ್ರತ್ಯಯ);
		ಸಂಕೇತ_2 = update_submodule(update_ದತ್ತಾಂಶ);
		FREE_AND_NULL(update_ದತ್ತಾಂಶ->displaypath);
fail:
		ಆದರೆ (!ಸಂಕೇತ_2)
			ಮುಂದುವರಿ;
		ret = ಸಂಕೇತ_2;
		ಆದರೆ (ret == 128)
			ಹೋಗು cleanup;
	}

cleanup:
	submodule_update_clone_release(&suc);
	string_list_clear(&update_ದತ್ತಾಂಶ->references, 0);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ module_update(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			 ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ pathspec pathspec = { 0 };
	ರಚನೆ pathspec pathspec2 = { 0 };
	ರಚನೆ update_ದತ್ತಾಂಶ opt = UPDATE_ದತ್ತಾಂಶ_INIT;
	ರಚನೆ list_objects_filter_options filter_options =
		LIST_OBJECTS_FILTER_INIT;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ref_storage_format = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ ret;
	ರಚನೆ option module_update_options[] = {
		OPT_SUPER_ಪೂರ್ವಪ್ರತ್ಯಯ(&opt.super_ಪೂರ್ವಪ್ರತ್ಯಯ),
		OPT__FORCE(&opt.force, N_("force checkout updates"), 0),
		OPT_BOOL('i', "init", &opt.init,
			 N_("initialize uninitialized submodules before update")),
		OPT_BOOL(0, "remote", &opt.remote,
			 N_("use SHA-1 of submodule's remote tracking branch")),
		OPT_BOOL(0, "recursive", &opt.recursive,
			 N_("traverse submodules recursively")),
		OPT_BOOL('N', "no-fetch", &opt.nofetch,
			 N_("don't fetch new objects from the remote site")),
		OPT_ಗಣ_INT(0, "checkout", &opt.update_default,
			N_("use the 'checkout' update strategy (default)"),
			SM_UPDATE_CHECKOUT),
		OPT_ಗಣ_INT('m', "merge", &opt.update_default,
			N_("use the 'merge' update strategy"),
			SM_UPDATE_ವಿಲೀನ),
		OPT_ಗಣ_INT('r', "rebase", &opt.update_default,
			N_("use the 'rebase' update strategy"),
			SM_UPDATE_REBASE),
		OPT_STRING_LIST(0, "reference", &opt.references, N_("repo"),
			   N_("reference repository")),
		OPT_STRING(0, "ref-storage-format", &ref_storage_format, N_("format"),
			   N_("specify the reference storage format to use")),
		OPT_ALIAS_F(0, "ref-format", "ref-storage-format", PARSE_OPT_HIDDEN),
		OPT_BOOL(0, "dissociate", &opt.dissociate,
			   N_("use --reference only while cloning")),
		OPT_INTEGER(0, "depth", &opt.ಆಳ,
			   N_("create a shallow clone truncated to the "
			      "specified number of revisions")),
		OPT_INTEGER('j', "jobs", &opt.max_jobs,
			    N_("parallel jobs")),
		OPT_BOOL(0, "recommend-shallow", &opt.recommend_shallow,
			    N_("whether the initial clone should follow the shallow recommendation")),
		OPT__QUIET(&opt.quiet, N_("don't print cloning progress")),
		OPT_BOOL(0, "progress", &opt.ಪ್ರಗತಿ,
			    N_("force cloning progress")),
		OPT_BOOL(0, "require-init", &opt.require_init,
			   N_("disallow cloning into non-empty directory, implies --init")),
		OPT_BOOL(0, "single-branch", &opt.single_branch,
			 N_("clone only one branch, HEAD or --branch")),
		OPT_PARSE_LIST_OBJECTS_FILTER(&filter_options),
		OPT_ಕೊನೆ()
	};
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ git_submodule_helper_usage[] = {
		N_("git submodule [--quiet] update"
		" [--init [--filter=<filter-spec>]] [--remote]"
		" [-N|--no-fetch] [-f|--force]"
		" [--checkout|--merge|--rebase]"
		" [--[no-]recommend-shallow] [--reference <repository>]"
		" [--recursive] [--[no-]single-branch] [--] [<path>...]"),
		ಖಾಲಿ
	};

	update_clone_config_from_gitmodules(the_repository, &opt.max_jobs);
	repo_config(the_repository, git_update_clone_config, &opt.max_jobs);

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, module_update_options,
			     git_submodule_helper_usage, 0);

	ಆದರೆ (opt.require_init)
		opt.init = 1;

	ಆದರೆ (filter_options.choice && !opt.init) {
		usage_with_options(git_submodule_helper_usage,
				   module_update_options);
	}

	ಆದರೆ (ref_storage_format) {
		opt.ref_storage_format = ref_storage_format_by_name(ref_storage_format);
		ಆದರೆ (opt.ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format);
	}

	opt.filter_options = &filter_options;
	opt.ಪೂರ್ವಪ್ರತ್ಯಯ = ಪೂರ್ವಪ್ರತ್ಯಯ;

	ಆದರೆ (opt.update_default)
		opt.update_strategy.type = opt.update_default;

	ಆದರೆ (module_list_compute(argv, ಪೂರ್ವಪ್ರತ್ಯಯ, &pathspec, &opt.list) < 0) {
		ret = 1;
		ಹೋಗು cleanup;
	}

	ಆದರೆ (pathspec.nr)
		opt.warn_if_uninitialized = 1;

	ಆದರೆ (opt.init) {
		ರಚನೆ module_list list = MODULE_LIST_INIT;
		ರಚನೆ init_cb info = INIT_CB_INIT;

		ಆದರೆ (module_list_compute(argv, opt.ಪೂರ್ವಪ್ರತ್ಯಯ,
					&pathspec2, &list) < 0) {
			module_list_release(&list);
			ret = 1;
			ಹೋಗು cleanup;
		}

		/*
		 * If there are no path args and submodule.active is set then,
		 * by default, only initialize 'active' modules.
		 */
		ಆದರೆ (!argc && !repo_config_get(the_repository, "submodule.active"))
			module_list_ಸಕ್ರಿಯ(&list);

		info.ಪೂರ್ವಪ್ರತ್ಯಯ = opt.ಪೂರ್ವಪ್ರತ್ಯಯ;
		info.super_ಪೂರ್ವಪ್ರತ್ಯಯ = opt.super_ಪೂರ್ವಪ್ರತ್ಯಯ;
		ಆದರೆ (opt.quiet)
			info.flags |= OPT_QUIET;

		for_each_listed_submodule(&list, init_submodule_cb, &info);
		module_list_release(&list);
	}

	ret = update_submodules(&opt);
cleanup:
	update_ದತ್ತಾಂಶ_release(&opt);
	list_objects_filter_release(&filter_options);
	clear_pathspec(&pathspec);
	clear_pathspec(&pathspec2);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ತಳ್ಳು_check(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ ಬಳಸದ,
		      ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ remote *remote;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *superproject_ತಲೆ;
	ಅಕ್ಷರ *ತಲೆ;
	ಪೂರ್ಣಾಂಕ detached_ತಲೆ = 0;
	ರಚನೆ object_id ತಲೆ_oid;

	ಆದರೆ (argc < 3)
		die("submodule--helper push-check requires at least 2 arguments");

	/*
	 * superproject's resolved head ref.
	 * if HEAD then the superproject is in a detached head state, otherwise
	 * it will be the resolved head ref.
	 */
	superproject_ತಲೆ = argv[1];
	argv++;
	argc--;
	/* Get the submodule's head ref and determine if it is detached */
	ತಲೆ = refs_resolve_refdup(get_main_ref_store(the_repository), "HEAD",
				   0, &ತಲೆ_oid, ಖಾಲಿ);
	ಆದರೆ (!ತಲೆ)
		die(_("Failed to resolve HEAD as a valid ref."));
	ಆದರೆ (!strcmp(ತಲೆ, "HEAD"))
		detached_ತಲೆ = 1;

	/*
	 * The remote must be configured.
	 * This is to avoid pushing to the exact same URL as the parent.
	 */
	remote = pushremote_get(argv[1]);
	ಆದರೆ (!remote || remote->origin == REMOTE_UNCONFIGURED)
		die("remote '%s' not configured", argv[1]);

	/* Check the refspec */
	ಆದರೆ (argc > 2) {
		ಪೂರ್ಣಾಂಕ i;
		ರಚನೆ ref *local_refs = get_local_heads();
		ರಚನೆ refspec refspec = REFSPEC_INIT_ತಳ್ಳು(the_hash_algo);

		refspec_appendn(&refspec, argv + 2, argc - 2);

		ಗಾಗಿ (i = 0; i < refspec.nr; i++) {
			ಸ್ಥಿರಾಂಕ ರಚನೆ refspec_ಅಂಶ *rs = &refspec.ವಸ್ತುಗಳು[i];

			ಆದರೆ (rs->ನಮೂನೆ || rs->matching)
				ಮುಂದುವರಿ;

			/* LHS must match a single ref */
			ಆಯ್ಕೆ (ಎಣಿಕೆ_refspec_ಹೊಂದಿಕೆ(rs->src, local_refs, ಖಾಲಿ)) {
			ಸ್ಥಿತಿ 1:
				ನಿಲ್ಲಿಸು;
			ಸ್ಥಿತಿ 0:
				/*
				 * If LHS matches 'HEAD' then we need to ensure
				 * that it matches the same named branch
				 * checked out in the superproject.
				 */
				ಆದರೆ (!strcmp(rs->src, "HEAD")) {
					ಆದರೆ (!detached_ತಲೆ &&
					    !strcmp(ತಲೆ, superproject_ತಲೆ))
						ನಿಲ್ಲಿಸು;
					die("HEAD does not match the named branch in the superproject");
				}
				/* fallthrough */
			ಪೂರ್ವನಿಯೋಜಿತ:
				die("src refspec '%s' must name a ref",
				    rs->src);
			}
		}

		refspec_clear(&refspec);
		free_refs(local_refs);
	}
	ಬಿಡುಗಡೆ(ತಲೆ);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ absorb_git_dirs(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			   ರಚನೆ repository *repo ಬಳಸದ)
{
	ಪೂರ್ಣಾಂಕ i;
	ರಚನೆ pathspec pathspec = { 0 };
	ರಚನೆ module_list list = MODULE_LIST_INIT;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *super_ಪೂರ್ವಪ್ರತ್ಯಯ = ಖಾಲಿ;
	ರಚನೆ option embed_gitdir_options[] = {
		OPT_SUPER_ಪೂರ್ವಪ್ರತ್ಯಯ(&super_ಪೂರ್ವಪ್ರತ್ಯಯ),
		OPT_ಕೊನೆ()
	};
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ git_submodule_helper_usage[] = {
		N_("git submodule absorbgitdirs [<options>] [<path>...]"),
		ಖಾಲಿ
	};
	ಪೂರ್ಣಾಂಕ ret = 1;

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, embed_gitdir_options,
			     git_submodule_helper_usage, 0);

	ಆದರೆ (module_list_compute(argv, ಪೂರ್ವಪ್ರತ್ಯಯ, &pathspec, &list) < 0)
		ಹೋಗು cleanup;

	ಗಾಗಿ (i = 0; i < list.nr; i++)
		absorb_git_dir_into_superproject(list.entries[i]->name,
						 super_ಪೂರ್ವಪ್ರತ್ಯಯ);

	ret = 0;
cleanup:
	clear_pathspec(&pathspec);
	module_list_release(&list);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ module_ಗಣ_url(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			  ರಚನೆ repository *repo ಬಳಸದ)
{
	ಪೂರ್ಣಾಂಕ quiet = 0, ret;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *newurl;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ;
	ಅಕ್ಷರ *config_name;
	ರಚನೆ option options[] = {
		OPT__QUIET(&quiet, N_("suppress output for setting url of a submodule")),
		OPT_ಕೊನೆ()
	};
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ usage[] = {
		N_("git submodule set-url [--quiet] <path> <newurl>"),
		ಖಾಲಿ
	};
	ಸ್ಥಿರಾಂಕ ರಚನೆ submodule *sub;

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options, usage, 0);

	ಆದರೆ (argc != 2 || !(ಮಾರ್ಗ = argv[0]) || !(newurl = argv[1]))
		usage_with_options(usage, options);

	sub = submodule_from_ಮಾರ್ಗ(the_repository, null_oid(the_hash_algo), ಮಾರ್ಗ);

	ಆದರೆ (!sub)
		die(_("no submodule mapping found in .gitmodules for path '%s'"),
		    ಮಾರ್ಗ);

	config_name = xstrfmt("submodule.%s.url", sub->name);
	ret = config_ಗಣ_ಒಳ_gitmodules_file_gently(the_repository, config_name, newurl);

	ಆದರೆ (!ret) {
		repo_read_gitmodules(the_repository, 0);
		sync_submodule(sub->ಮಾರ್ಗ, ಪೂರ್ವಪ್ರತ್ಯಯ, ಖಾಲಿ, quiet ? OPT_QUIET : 0);
	}

	ಬಿಡುಗಡೆ(config_name);
	ಹಿಂತಿರುಗಿ !!ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ module_ಗಣ_branch(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			     ರಚನೆ repository *repo ಬಳಸದ)
{
	ಪೂರ್ಣಾಂಕ opt_default = 0, ret;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *opt_branch = ಖಾಲಿ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ;
	ಅಕ್ಷರ *config_name;
	ರಚನೆ option options[] = {
		/*
		 * We accept the `quiet` option for uniformity across subcommands,
		 * though there is nothing to make less verbose in this subcommand.
		 */
		OPT_NOOP_NOARG('q', "quiet"),

		OPT_BOOL('d', "default", &opt_default,
			N_("set the default tracking branch to master")),
		OPT_STRING('b', "branch", &opt_branch, N_("branch"),
			N_("set the default tracking branch")),
		OPT_ಕೊನೆ()
	};
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ usage[] = {
		N_("git submodule set-branch [-q|--quiet] (-d|--default) <path>"),
		N_("git submodule set-branch [-q|--quiet] (-b|--branch) <branch> <path>"),
		ಖಾಲಿ
	};
	ಸ್ಥಿರಾಂಕ ರಚನೆ submodule *sub;

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options, usage, 0);

	ಆದರೆ (!opt_branch && !opt_default)
		die(_("--branch or --default required"));

	ಆದರೆ (opt_branch && opt_default)
		die(_("options '%s' and '%s' cannot be used together"), "--branch", "--default");

	ಆದರೆ (argc != 1 || !(ಮಾರ್ಗ = argv[0]))
		usage_with_options(usage, options);

	sub = submodule_from_ಮಾರ್ಗ(the_repository, null_oid(the_hash_algo), ಮಾರ್ಗ);

	ಆದರೆ (!sub)
		die(_("no submodule mapping found in .gitmodules for path '%s'"),
		    ಮಾರ್ಗ);

	config_name = xstrfmt("submodule.%s.branch", sub->name);
	ret = config_ಗಣ_ಒಳ_gitmodules_file_gently(the_repository, config_name, opt_branch);

	ಬಿಡುಗಡೆ(config_name);
	ಹಿಂತಿರುಗಿ !!ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ module_create_branch(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
				ರಚನೆ repository *repo ಬಳಸದ)
{
	ಎಣಿಕೆ branch_track track;
	ಪೂರ್ಣಾಂಕ quiet = 0, force = 0, reflog = 0, dry_ಸತತ = 0;
	ರಚನೆ option options[] = {
		OPT__QUIET(&quiet, N_("print only error messages")),
		OPT__FORCE(&force, N_("force creation"), 0),
		OPT_BOOL(0, "create-reflog", &reflog,
			 N_("create the branch's reflog")),
		OPT_CALLBACK_F('t', "track",  &track, "(direct|inherit)",
			N_("set branch tracking configuration"),
			PARSE_OPT_OPTARG,
			parse_opt_tracking_ಬಹುಲಕ),
		OPT_DRY_ಸತತ(&dry_ಸತತ,
			     N_("show whether the branch would be created")),
		OPT_ಕೊನೆ()
	};
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ usage[] = {
		N_("git submodule--helper create-branch [-f|--force] [--create-reflog] [-q|--quiet] [-t|--track] [-n|--dry-run] <name> <start-oid> <start-name>"),
		ಖಾಲಿ
	};
	ರಚನೆ repo_config_ಮೌಲ್ಯಗಳು *cfg = repo_config_ಮೌಲ್ಯಗಳು(the_repository);

	repo_config(the_repository, git_default_config, ಖಾಲಿ);
	track = cfg->branch_track;
	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options, usage, 0);

	ಆದರೆ (argc != 3)
		usage_with_options(usage, options);

	ಆದರೆ (!quiet && !dry_ಸತತ)
		printf_ln(_("creating branch '%s'"), argv[0]);

	create_branches_recursively(the_repository, argv[0], argv[1], argv[2],
				    force, reflog, quiet, track, dry_ಸತತ);
	ಹಿಂತಿರುಗಿ 0;
}

ರಚನೆ ಕೂಡಿಸು_ದತ್ತಾಂಶ {
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *branch;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *reference_ಮಾರ್ಗ;
	ಅಕ್ಷರ *sm_ಮಾರ್ಗ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *sm_name;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *repo;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *realrepo;
	ಎಣಿಕೆ ref_storage_format ref_storage_format;
	ಪೂರ್ಣಾಂಕ ಆಳ;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ force: 1;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ quiet: 1;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಪ್ರಗತಿ: 1;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ dissociate: 1;
};
#ವ್ಯಾಖ್ಯಾನಿಸು ಕೂಡಿಸು_ದತ್ತಾಂಶ_INIT { \
	.ಆಳ = -1, \
	.ref_storage_format = REF_STORAGE_FORMAT_UNKNOWN, \
}

ಸ್ಥಿರ ಶೂನ್ಯ append_fetch_remotes(ರಚನೆ strbuf *ಸಂದೇಶ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *git_dir_ಮಾರ್ಗ)
{
	ರಚನೆ ಮಗು_process cp_remote = ಮಗು_PROCESS_INIT;
	ರಚನೆ strbuf sb_remote_ಹೊರಹೋಗು = STRBUF_INIT;

	cp_remote.git_cmd = 1;
	strvec_pushf(&cp_remote.env,
		     "GIT_DIR=%s", git_dir_ಮಾರ್ಗ);
	strvec_ತಳ್ಳು(&cp_remote.env, "GIT_WORK_TREE=.");
	strvec_pushl(&cp_remote.ವಾದಗಳು, "remote", "-v", ಖಾಲಿ);
	ಆದರೆ (!capture_command(&cp_remote, &sb_remote_ಹೊರಹೋಗು, 0)) {
		ಅಕ್ಷರ *ಮುಂದಿನ_ಸಾಲು;
		ಅಕ್ಷರ *ಸಾಲು = sb_remote_ಹೊರಹೋಗು.buf;

		ಆಗ ((ಮುಂದಿನ_ಸಾಲು = strchr(ಸಾಲು, '\n')) != ಖಾಲಿ) {
			ಗಾತ್ರ_t len = ಮುಂದಿನ_ಸಾಲು - ಸಾಲು;

			ಆದರೆ (strip_suffix_mem(ಸಾಲು, &len, " (fetch)"))
				strbuf_addf(ಸಂದೇಶ, "  %.*s\n", (ಪೂರ್ಣಾಂಕ)len, ಸಾಲು);
			ಸಾಲು = ಮುಂದಿನ_ಸಾಲು + 1;
		}
	}

	strbuf_release(&sb_remote_ಹೊರಹೋಗು);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಕೂಡಿಸು_submodule(ಸ್ಥಿರಾಂಕ ರಚನೆ ಕೂಡಿಸು_ದತ್ತಾಂಶ *ಕೂಡಿಸು_ದತ್ತಾಂಶ)
{
	ರಚನೆ module_clone_ದತ್ತಾಂಶ clone_ದತ್ತಾಂಶ = MODULE_CLONE_ದತ್ತಾಂಶ_INIT;
	ರಚನೆ string_list reference = STRING_LIST_INIT_NODUP;
	ಪೂರ್ಣಾಂಕ ret = -1;

	/* perhaps the path already exists and is already a git repo, else clone it */
	ಆದರೆ (is_directory(ಕೂಡಿಸು_ದತ್ತಾಂಶ->sm_ಮಾರ್ಗ)) {
		ಅಕ್ಷರ *submod_gitdir_ಮಾರ್ಗ;
		ರಚನೆ strbuf sm_ಮಾರ್ಗ = STRBUF_INIT;
		strbuf_addstr(&sm_ಮಾರ್ಗ, ಕೂಡಿಸು_ದತ್ತಾಂಶ->sm_ಮಾರ್ಗ);
		submod_gitdir_ಮಾರ್ಗ = xstrfmt("%s/.git", ಕೂಡಿಸು_ದತ್ತಾಂಶ->sm_ಮಾರ್ಗ);
		ಆದರೆ (is_nonbare_repository_dir(&sm_ಮಾರ್ಗ))
			ಬರೆ(_("Adding existing repo at '%s' to the index\n"),
			       ಕೂಡಿಸು_ದತ್ತಾಂಶ->sm_ಮಾರ್ಗ);
		ಇಲ್ಲದಿದ್ದರೆ
			die(_("'%s' already exists and is not a valid git repo"),
			    ಕೂಡಿಸು_ದತ್ತಾಂಶ->sm_ಮಾರ್ಗ);
		strbuf_release(&sm_ಮಾರ್ಗ);
		ಬಿಡುಗಡೆ(submod_gitdir_ಮಾರ್ಗ);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ರಚನೆ ಮಗು_process cp = ಮಗು_PROCESS_INIT;
		ರಚನೆ strbuf submod_gitdir = STRBUF_INIT;

		submodule_name_ಗಮ್ಯ_gitdir(&submod_gitdir, the_repository, ಕೂಡಿಸು_ದತ್ತಾಂಶ->sm_name);

		ಆದರೆ (is_directory(submod_gitdir.buf)) {
			ಆದರೆ (!ಕೂಡಿಸು_ದತ್ತಾಂಶ->force) {
				ರಚನೆ strbuf ಸಂದೇಶ = STRBUF_INIT;
				ಅಕ್ಷರ *die_ಸಂದೇಶ_2;

				strbuf_addf(&ಸಂದೇಶ, _("A git directory for '%s' is found "
						    "locally with remote(s):\n"),
					    ಕೂಡಿಸು_ದತ್ತಾಂಶ->sm_name);

				append_fetch_remotes(&ಸಂದೇಶ, submod_gitdir.buf);
				strbuf_release(&submod_gitdir);

				strbuf_addf(&ಸಂದೇಶ, _("If you want to reuse this local git "
						    "directory instead of cloning again from\n"
						    "  %s\n"
						    "use the '--force' option. If the local git "
						    "directory is not the correct repo\n"
						    "or you are unsure what this means choose "
						    "another name with the '--name' option."),
					    ಕೂಡಿಸು_ದತ್ತಾಂಶ->realrepo);

				die_ಸಂದೇಶ_2 = strbuf_detach(&ಸಂದೇಶ, ಖಾಲಿ);
				die("%s", die_ಸಂದೇಶ_2);
			} ಇಲ್ಲದಿದ್ದರೆ {
				ಬರೆ(_("Reactivating local git directory for "
					 "submodule '%s'\n"), ಕೂಡಿಸು_ದತ್ತಾಂಶ->sm_name);
			}
		}
		strbuf_release(&submod_gitdir);

		clone_ದತ್ತಾಂಶ.ಪೂರ್ವಪ್ರತ್ಯಯ = ಕೂಡಿಸು_ದತ್ತಾಂಶ->ಪೂರ್ವಪ್ರತ್ಯಯ;
		clone_ದತ್ತಾಂಶ.ಮಾರ್ಗ = ಕೂಡಿಸು_ದತ್ತಾಂಶ->sm_ಮಾರ್ಗ;
		clone_ದತ್ತಾಂಶ.name = ಕೂಡಿಸು_ದತ್ತಾಂಶ->sm_name;
		clone_ದತ್ತಾಂಶ.url = ಕೂಡಿಸು_ದತ್ತಾಂಶ->realrepo;
		clone_ದತ್ತಾಂಶ.quiet = ಕೂಡಿಸು_ದತ್ತಾಂಶ->quiet;
		clone_ದತ್ತಾಂಶ.ಪ್ರಗತಿ = ಕೂಡಿಸು_ದತ್ತಾಂಶ->ಪ್ರಗತಿ;
		ಆದರೆ (ಕೂಡಿಸು_ದತ್ತಾಂಶ->reference_ಮಾರ್ಗ) {
			ಅಕ್ಷರ *p = xstrdup(ಕೂಡಿಸು_ದತ್ತಾಂಶ->reference_ಮಾರ್ಗ);

			string_list_append(&reference, p)->util = p;
		}
		clone_ದತ್ತಾಂಶ.ref_storage_format = ಕೂಡಿಸು_ದತ್ತಾಂಶ->ref_storage_format;
		clone_ದತ್ತಾಂಶ.dissociate = ಕೂಡಿಸು_ದತ್ತಾಂಶ->dissociate;
		ಆದರೆ (ಕೂಡಿಸು_ದತ್ತಾಂಶ->ಆಳ >= 0)
			clone_ದತ್ತಾಂಶ.ಆಳ = ಕೂಡಿಸು_ದತ್ತಾಂಶ->ಆಳ;

		ಆದರೆ (clone_submodule(&clone_ದತ್ತಾಂಶ, &reference))
			ಹೋಗು cleanup;

		prepare_submodule_repo_env(&cp.env);
		cp.git_cmd = 1;
		cp.dir = ಕೂಡಿಸು_ದತ್ತಾಂಶ->sm_ಮಾರ್ಗ;
		/*
		 * NOTE: we only get here if add_data->force is true, so
		 * passing --force to checkout is reasonable.
		 */
		strvec_pushl(&cp.ವಾದಗಳು, "checkout", "-f", "-q", ಖಾಲಿ);

		ಆದರೆ (ಕೂಡಿಸು_ದತ್ತಾಂಶ->branch) {
			strvec_pushl(&cp.ವಾದಗಳು, "-B", ಕೂಡಿಸು_ದತ್ತಾಂಶ->branch, ಖಾಲಿ);
			strvec_pushf(&cp.ವಾದಗಳು, "origin/%s", ಕೂಡಿಸು_ದತ್ತಾಂಶ->branch);
		}

		ಆದರೆ (ಸತತ_command(&cp))
			die(_("unable to checkout submodule '%s'"), ಕೂಡಿಸು_ದತ್ತಾಂಶ->sm_ಮಾರ್ಗ);
	}
	ret = 0;

cleanup:
	string_list_clear(&reference, 1);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ config_submodule_ಒಳ_gitmodules(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *var, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ)
{
	ಅಕ್ಷರ *ಕೀಲಿ;
	ಪೂರ್ಣಾಂಕ ret;

	ಆದರೆ (!is_writing_gitmodules_ok())
		die(_("please make sure that the .gitmodules file is in the working tree"));

	ಕೀಲಿ = xstrfmt("submodule.%s.%s", name, var);
	ret = config_ಗಣ_ಒಳ_gitmodules_file_gently(the_repository, ಕೀಲಿ, ಮೌಲ್ಯ);
	ಬಿಡುಗಡೆ(ಕೀಲಿ);

	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಶೂನ್ಯ configure_added_submodule(ರಚನೆ ಕೂಡಿಸು_ದತ್ತಾಂಶ *ಕೂಡಿಸು_ದತ್ತಾಂಶ)
{
	ಅಕ್ಷರ *ಕೀಲಿ;
	ರಚನೆ ಮಗು_process ಕೂಡಿಸು_submod = ಮಗು_PROCESS_INIT;
	ರಚನೆ ಮಗು_process ಕೂಡಿಸು_gitmodules = ಮಗು_PROCESS_INIT;
	ಸ್ಥಿರಾಂಕ ರಚನೆ string_list *ಮೌಲ್ಯಗಳು;
	ಪೂರ್ಣಾಂಕ matched = 0;

	ಕೀಲಿ = xstrfmt("submodule.%s.url", ಕೂಡಿಸು_ದತ್ತಾಂಶ->sm_name);
	repo_config_ಗಣ_gently(the_repository, ಕೀಲಿ, ಕೂಡಿಸು_ದತ್ತಾಂಶ->realrepo);
	ಬಿಡುಗಡೆ(ಕೀಲಿ);

	ಕೂಡಿಸು_submod.git_cmd = 1;
	strvec_pushl(&ಕೂಡಿಸು_submod.ವಾದಗಳು, "add",
		     "--no-warn-embedded-repo", ಖಾಲಿ);
	ಆದರೆ (ಕೂಡಿಸು_ದತ್ತಾಂಶ->force)
		strvec_ತಳ್ಳು(&ಕೂಡಿಸು_submod.ವಾದಗಳು, "--force");
	strvec_pushl(&ಕೂಡಿಸು_submod.ವಾದಗಳು, "--", ಕೂಡಿಸು_ದತ್ತಾಂಶ->sm_ಮಾರ್ಗ, ಖಾಲಿ);

	ಆದರೆ (ಸತತ_command(&ಕೂಡಿಸು_submod))
		die(_("Failed to add submodule '%s'"), ಕೂಡಿಸು_ದತ್ತಾಂಶ->sm_ಮಾರ್ಗ);

	ಆದರೆ (config_submodule_ಒಳ_gitmodules(ಕೂಡಿಸು_ದತ್ತಾಂಶ->sm_name, "path", ಕೂಡಿಸು_ದತ್ತಾಂಶ->sm_ಮಾರ್ಗ) ||
	    config_submodule_ಒಳ_gitmodules(ಕೂಡಿಸು_ದತ್ತಾಂಶ->sm_name, "url", ಕೂಡಿಸು_ದತ್ತಾಂಶ->repo))
		die(_("Failed to register submodule '%s'"), ಕೂಡಿಸು_ದತ್ತಾಂಶ->sm_ಮಾರ್ಗ);

	ಆದರೆ (ಕೂಡಿಸು_ದತ್ತಾಂಶ->branch) {
		ಆದರೆ (config_submodule_ಒಳ_gitmodules(ಕೂಡಿಸು_ದತ್ತಾಂಶ->sm_name,
						   "branch", ಕೂಡಿಸು_ದತ್ತಾಂಶ->branch))
			die(_("Failed to register submodule '%s'"), ಕೂಡಿಸು_ದತ್ತಾಂಶ->sm_ಮಾರ್ಗ);
	}

	ಕೂಡಿಸು_gitmodules.git_cmd = 1;
	strvec_pushl(&ಕೂಡಿಸು_gitmodules.ವಾದಗಳು,
		     "add", "--force", "--", ".gitmodules", ಖಾಲಿ);

	ಆದರೆ (ಸತತ_command(&ಕೂಡಿಸು_gitmodules))
		die(_("Failed to register submodule '%s'"), ಕೂಡಿಸು_ದತ್ತಾಂಶ->sm_ಮಾರ್ಗ);

	/*
	 * NEEDSWORK: In a multi-working-tree world this needs to be
	 * set in the per-worktree config.
	 */
	/*
	 * NEEDSWORK: In the longer run, we need to get rid of this
	 * pattern of querying "submodule.active" before calling
	 * is_submodule_active(), since that function needs to find
	 * out the value of "submodule.active" again anyway.
	 */
	ಆದರೆ (repo_config_get(the_repository, "submodule.active") || /* key absent */
	    repo_config_get_string_multi(the_repository, "submodule.active", &ಮೌಲ್ಯಗಳು)) {
		/*
		 * If the submodule being added isn't already covered by the
		 * current configured pathspec, set the submodule's active flag
		 */
		ಕೀಲಿ = xstrfmt("submodule.%s.active", ಕೂಡಿಸು_ದತ್ತಾಂಶ->sm_name);
		repo_config_ಗಣ_gently(the_repository, ಕೀಲಿ, "true");
		ಬಿಡುಗಡೆ(ಕೀಲಿ);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < ಮೌಲ್ಯಗಳು->nr; i++) {
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *pat = ಮೌಲ್ಯಗಳು->ವಸ್ತುಗಳು[i].string;
			ಆದರೆ (!wildmatch(pat, ಕೂಡಿಸು_ದತ್ತಾಂಶ->sm_ಮಾರ್ಗ, 0)) { /* match found */
				matched = 1;
				ನಿಲ್ಲಿಸು;
			}
		}
		ಆದರೆ (!matched) { /* no pattern matched -> force-enable */
			ಕೀಲಿ = xstrfmt("submodule.%s.active", ಕೂಡಿಸು_ದತ್ತಾಂಶ->sm_name);
			repo_config_ಗಣ_gently(the_repository, ಕೀಲಿ, "true");
			ಬಿಡುಗಡೆ(ಕೀಲಿ);
		}
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ die_on_ಸೂಚ್ಯಂಕ_ಹೊಂದಿಕೆ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಪೂರ್ಣಾಂಕ force)
{
	ರಚನೆ pathspec ps;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ವಾದಗಳು[] = { ಮಾರ್ಗ, ಖಾಲಿ };
	parse_pathspec(&ps, 0, PATHSPEC_PREFER_CWD, ಖಾಲಿ, ವಾದಗಳು);

	ಆದರೆ (repo_read_ಸೂಚ್ಯಂಕ_preload(the_repository, ಖಾಲಿ, 0) < 0)
		die(_("index file corrupt"));

	ಆದರೆ (ps.nr) {
		ಅಕ್ಷರ *ps_matched = xcalloc(ps.nr, 1);

		/* TODO: audit for interaction with sparse-index. */
		ensure_ತುಂಬಿದ_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ);

		/*
		 * Since there is only one pathspec, we just need to
		 * check ps_matched[0] to know if a cache entry matched.
		 */
		ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < the_repository->ಸೂಚ್ಯಂಕ->cache_nr; i++) {
			ce_ಮಾರ್ಗ_ಹೊಂದಿಕೆ(the_repository->ಸೂಚ್ಯಂಕ, the_repository->ಸೂಚ್ಯಂಕ->cache[i], &ps,
				      ps_matched);

			ಆದರೆ (ps_matched[0]) {
				ಆದರೆ (!force)
					die(_("'%s' already exists in the index"),
					    ಮಾರ್ಗ);
				ಆದರೆ (!S_ISGITLINK(the_repository->ಸೂಚ್ಯಂಕ->cache[i]->ce_ಬಹುಲಕ))
					die(_("'%s' already exists in the index "
					      "and is not a submodule"), ಮಾರ್ಗ);
				ನಿಲ್ಲಿಸು;
			}
		}
		ಬಿಡುಗಡೆ(ps_matched);
	}
	clear_pathspec(&ps);
}

ಸ್ಥಿರ ಶೂನ್ಯ die_on_repo_without_commits(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ)
{
	ರಚನೆ strbuf sb = STRBUF_INIT;
	strbuf_addstr(&sb, ಮಾರ್ಗ);
	ಆದರೆ (is_nonbare_repository_dir(&sb)) {
		ರಚನೆ object_id oid;
		ಆದರೆ (repo_resolve_gitlink_ref(the_repository, ಮಾರ್ಗ, "HEAD", &oid) < 0)
			die(_("'%s' does not have a commit checked out"), ಮಾರ್ಗ);
	}
	strbuf_release(&sb);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ module_ಕೂಡಿಸು(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		      ರಚನೆ repository *repo ಬಳಸದ)
{
	ಪೂರ್ಣಾಂಕ force = 0, quiet = 0, ಪ್ರಗತಿ = 0, dissociate = 0;
	ರಚನೆ ಕೂಡಿಸು_ದತ್ತಾಂಶ ಕೂಡಿಸು_ದತ್ತಾಂಶ = ಕೂಡಿಸು_ದತ್ತಾಂಶ_INIT;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ref_storage_format = ಖಾಲಿ;
	ಅಕ್ಷರ *ಗಮ್ಯ_free = ಖಾಲಿ;
	ಸ್ಥಿರಾಂಕ ರಚನೆ submodule *existing;
	ರಚನೆ strbuf buf = STRBUF_INIT;
	ಅಕ್ಷರ *sm_name_ಗಮ್ಯ_free = ಖಾಲಿ;
	ರಚನೆ option options[] = {
		OPT_STRING('b', "branch", &ಕೂಡಿಸು_ದತ್ತಾಂಶ.branch, N_("branch"),
			   N_("branch of repository to add as submodule")),
		OPT__FORCE(&force, N_("allow adding an otherwise ignored submodule path"),
			   PARSE_OPT_NOCOMPLETE),
		OPT__QUIET(&quiet, N_("print only error messages")),
		OPT_BOOL(0, "progress", &ಪ್ರಗತಿ, N_("force cloning progress")),
		OPT_STRING(0, "reference", &ಕೂಡಿಸು_ದತ್ತಾಂಶ.reference_ಮಾರ್ಗ, N_("repository"),
			   N_("reference repository")),
		OPT_STRING(0, "ref-storage-format", &ref_storage_format, N_("format"),
			   N_("specify the reference storage format to use")),
		OPT_ALIAS_F(0, "ref-format", "ref-storage-format", PARSE_OPT_HIDDEN),
		OPT_BOOL(0, "dissociate", &dissociate, N_("borrow the objects from reference repositories")),
		OPT_STRING(0, "name", &ಕೂಡಿಸು_ದತ್ತಾಂಶ.sm_name, N_("name"),
			   N_("sets the submodule's name to the given string "
			      "instead of defaulting to its path")),
		OPT_INTEGER(0, "depth", &ಕೂಡಿಸು_ದತ್ತಾಂಶ.ಆಳ, N_("depth for shallow clones")),
		OPT_ಕೊನೆ()
	};
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ usage[] = {
		N_("git submodule add [<options>] [--] <repository> [<path>]"),
		ಖಾಲಿ
	};
	ರಚನೆ strbuf sb = STRBUF_INIT;
	ಪೂರ್ಣಾಂಕ ret = 1;

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options, usage, 0);

	ಆದರೆ (!is_writing_gitmodules_ok())
		die(_("please make sure that the .gitmodules file is in the working tree"));

	ಆದರೆ (ಪೂರ್ವಪ್ರತ್ಯಯ && *ಪೂರ್ವಪ್ರತ್ಯಯ &&
	    ಕೂಡಿಸು_ದತ್ತಾಂಶ.reference_ಮಾರ್ಗ && !is_absolute_ಮಾರ್ಗ(ಕೂಡಿಸು_ದತ್ತಾಂಶ.reference_ಮಾರ್ಗ))
		ಕೂಡಿಸು_ದತ್ತಾಂಶ.reference_ಮಾರ್ಗ = xstrfmt("%s%s", ಪೂರ್ವಪ್ರತ್ಯಯ, ಕೂಡಿಸು_ದತ್ತಾಂಶ.reference_ಮಾರ್ಗ);

	ಆದರೆ (argc == 0 || argc > 2)
		usage_with_options(usage, options);

	ಆದರೆ (ref_storage_format) {
		ಕೂಡಿಸು_ದತ್ತಾಂಶ.ref_storage_format = ref_storage_format_by_name(ref_storage_format);
		ಆದರೆ (ಕೂಡಿಸು_ದತ್ತಾಂಶ.ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format);
	}

	ಕೂಡಿಸು_ದತ್ತಾಂಶ.repo = argv[0];
	ಆದರೆ (argc == 1)
		ಕೂಡಿಸು_ದತ್ತಾಂಶ.sm_ಮಾರ್ಗ = git_url_basename(ಕೂಡಿಸು_ದತ್ತಾಂಶ.repo, 0, 0);
	ಇಲ್ಲದಿದ್ದರೆ
		ಕೂಡಿಸು_ದತ್ತಾಂಶ.sm_ಮಾರ್ಗ = xstrdup(argv[1]);

	ಆದರೆ (ಪೂರ್ವಪ್ರತ್ಯಯ && *ಪೂರ್ವಪ್ರತ್ಯಯ && !is_absolute_ಮಾರ್ಗ(ಕೂಡಿಸು_ದತ್ತಾಂಶ.sm_ಮಾರ್ಗ)) {
		ಅಕ್ಷರ *sm_ಮಾರ್ಗ = ಕೂಡಿಸು_ದತ್ತಾಂಶ.sm_ಮಾರ್ಗ;

		ಕೂಡಿಸು_ದತ್ತಾಂಶ.sm_ಮಾರ್ಗ = xstrfmt("%s%s", ಪೂರ್ವಪ್ರತ್ಯಯ, sm_ಮಾರ್ಗ);
		ಬಿಡುಗಡೆ(sm_ಮಾರ್ಗ);
	}

	ಆದರೆ (starts_with_dot_dot_slash(ಕೂಡಿಸು_ದತ್ತಾಂಶ.repo) ||
	    starts_with_dot_slash(ಕೂಡಿಸು_ದತ್ತಾಂಶ.repo)) {
		ಆದರೆ (ಪೂರ್ವಪ್ರತ್ಯಯ)
			die(_("Relative path can only be used from the toplevel "
			      "of the working tree"));

		/* dereference source url relative to parent's url */
		ಗಮ್ಯ_free = resolve_relative_url(ಕೂಡಿಸು_ದತ್ತಾಂಶ.repo, ಖಾಲಿ, 1);
		ಕೂಡಿಸು_ದತ್ತಾಂಶ.realrepo = ಗಮ್ಯ_free;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (is_dir_sep(ಕೂಡಿಸು_ದತ್ತಾಂಶ.repo[0]) || strchr(ಕೂಡಿಸು_ದತ್ತಾಂಶ.repo, ':')) {
		ಕೂಡಿಸು_ದತ್ತಾಂಶ.realrepo = ಕೂಡಿಸು_ದತ್ತಾಂಶ.repo;
	} ಇಲ್ಲದಿದ್ದರೆ {
		die(_("repo URL: '%s' must be absolute or begin with ./|../"),
		    ಕೂಡಿಸು_ದತ್ತಾಂಶ.repo);
	}

	/*
	 * normalize path:
	 * multiple //; leading ./; /./; /../;
	 */
	normalize_ಮಾರ್ಗ_copy(ಕೂಡಿಸು_ದತ್ತಾಂಶ.sm_ಮಾರ್ಗ, ಕೂಡಿಸು_ದತ್ತಾಂಶ.sm_ಮಾರ್ಗ);
	strip_dir_trailing_slashes(ಕೂಡಿಸು_ದತ್ತಾಂಶ.sm_ಮಾರ್ಗ);

	ಆದರೆ (validate_submodule_ಮಾರ್ಗ(ಕೂಡಿಸು_ದತ್ತಾಂಶ.sm_ಮಾರ್ಗ) < 0)
		die(ಖಾಲಿ);

	die_on_ಸೂಚ್ಯಂಕ_ಹೊಂದಿಕೆ(ಕೂಡಿಸು_ದತ್ತಾಂಶ.sm_ಮಾರ್ಗ, force);
	die_on_repo_without_commits(ಕೂಡಿಸು_ದತ್ತಾಂಶ.sm_ಮಾರ್ಗ);

	ಆದರೆ (!force) {
		ರಚನೆ ಮಗು_process cp = ಮಗು_PROCESS_INIT;

		cp.git_cmd = 1;
		cp.no_stdout = 1;
		strvec_pushl(&cp.ವಾದಗಳು, "add", "--dry-run", "--ignore-missing",
			     "--no-warn-embedded-repo", ಕೂಡಿಸು_ದತ್ತಾಂಶ.sm_ಮಾರ್ಗ, ಖಾಲಿ);
		ಆದರೆ ((ret = pipe_command(&cp, ಖಾಲಿ, 0, ಖಾಲಿ, 0, &sb, 0))) {
			strbuf_complete_ಸಾಲು(&sb);
			fputs(sb.buf, stderr);
			ಹೋಗು cleanup;
		}
	}

	ಆದರೆ (!ಕೂಡಿಸು_ದತ್ತಾಂಶ.sm_name)
		ಕೂಡಿಸು_ದತ್ತಾಂಶ.sm_name = ಕೂಡಿಸು_ದತ್ತಾಂಶ.sm_ಮಾರ್ಗ;

	existing = submodule_from_name(the_repository,
					null_oid(the_hash_algo),
					ಕೂಡಿಸು_ದತ್ತಾಂಶ.sm_name);

	ಆದರೆ (existing && existing->ಮಾರ್ಗ &&
	    strcmp(existing->ಮಾರ್ಗ, ಕೂಡಿಸು_ದತ್ತಾಂಶ.sm_ಮಾರ್ಗ)) {
		ಆದರೆ (!force) {
			die(_("submodule name '%s' already used for path '%s'"),
			    ಕೂಡಿಸು_ದತ್ತಾಂಶ.sm_name, existing->ಮಾರ್ಗ);
		}
		/* --force: build <name><n> until unique */
		ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 1; ; i++) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "%s%d", ಕೂಡಿಸು_ದತ್ತಾಂಶ.sm_name, i);
			ಆದರೆ (!submodule_from_name(the_repository,
						null_oid(the_hash_algo),
						buf.buf)) {
				ನಿಲ್ಲಿಸು;
			}
		}
		ಕೂಡಿಸು_ದತ್ತಾಂಶ.sm_name = sm_name_ಗಮ್ಯ_free = strbuf_detach(&buf, ಖಾಲಿ);
	}

	ಆದರೆ (check_submodule_name(ಕೂಡಿಸು_ದತ್ತಾಂಶ.sm_name))
		die(_("'%s' is not a valid submodule name"), ಕೂಡಿಸು_ದತ್ತಾಂಶ.sm_name);

	ಕೂಡಿಸು_ದತ್ತಾಂಶ.ಪೂರ್ವಪ್ರತ್ಯಯ = ಪೂರ್ವಪ್ರತ್ಯಯ;
	ಕೂಡಿಸು_ದತ್ತಾಂಶ.force = !!force;
	ಕೂಡಿಸು_ದತ್ತಾಂಶ.quiet = !!quiet;
	ಕೂಡಿಸು_ದತ್ತಾಂಶ.ಪ್ರಗತಿ = !!ಪ್ರಗತಿ;
	ಕೂಡಿಸು_ದತ್ತಾಂಶ.dissociate = !!dissociate;

	ಆದರೆ (the_repository->repository_format_submodule_ಮಾರ್ಗ_cfg)
		create_default_gitdir_config(ಕೂಡಿಸು_ದತ್ತಾಂಶ.sm_name);

	ಆದರೆ (ಕೂಡಿಸು_submodule(&ಕೂಡಿಸು_ದತ್ತಾಂಶ))
		ಹೋಗು cleanup;
	configure_added_submodule(&ಕೂಡಿಸು_ದತ್ತಾಂಶ);

	ret = 0;
cleanup:
	ಬಿಡುಗಡೆ(sm_name_ಗಮ್ಯ_free);
	ಬಿಡುಗಡೆ(ಕೂಡಿಸು_ದತ್ತಾಂಶ.sm_ಮಾರ್ಗ);
	ಬಿಡುಗಡೆ(ಗಮ್ಯ_free);
	strbuf_release(&sb);

	ಹಿಂತಿರುಗಿ ret;
}

ಪೂರ್ಣಾಂಕ cmd_submodule__helper(ಪೂರ್ಣಾಂಕ argc,
			  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
			  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			  ರಚನೆ repository *repo)
{
	parse_opt_subcommand_fn *fn = ಖಾಲಿ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ usage[] = {
		N_("git submodule--helper <command>"),
		ಖಾಲಿ
	};
	ರಚನೆ option options[] = {
		OPT_SUBCOMMAND("migrate-gitdir-configs", &fn, module_migrate),
		OPT_SUBCOMMAND("gitdir", &fn, module_gitdir),
		OPT_SUBCOMMAND("clone", &fn, module_clone),
		OPT_SUBCOMMAND("add", &fn, module_ಕೂಡಿಸು),
		OPT_SUBCOMMAND("update", &fn, module_update),
		OPT_SUBCOMMAND("foreach", &fn, module_foreach),
		OPT_SUBCOMMAND("init", &fn, module_init),
		OPT_SUBCOMMAND("status", &fn, module_ಸ್ಥಿತಿ),
		OPT_SUBCOMMAND("sync", &fn, module_sync),
		OPT_SUBCOMMAND("deinit", &fn, module_deinit),
		OPT_SUBCOMMAND("summary", &fn, module_summary),
		OPT_SUBCOMMAND("push-check", &fn, ತಳ್ಳು_check),
		OPT_SUBCOMMAND("absorbgitdirs", &fn, absorb_git_dirs),
		OPT_SUBCOMMAND("set-url", &fn, module_ಗಣ_url),
		OPT_SUBCOMMAND("set-branch", &fn, module_ಗಣ_branch),
		OPT_SUBCOMMAND("create-branch", &fn, module_create_branch),
		OPT_SUBCOMMAND("get-default-remote", &fn, module_get_default_remote),
		OPT_ಕೊನೆ()
	};
	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options, usage, 0);

	ಹಿಂತಿರುಗಿ fn(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, repo);
}
