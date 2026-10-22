#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "abspath.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hex.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "path.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "pathspec.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "preload-index.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "dir.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "read-cache.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "setup.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "sparse-index.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "submodule.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "submodule-config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "string-list.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "run-command.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "remote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "refs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "refspec.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "revision.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "diffcore.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "diff.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-file.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-name.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "odb.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "odb/source.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "odb/source-files.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "advice.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "branch.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "list-objects-filter-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "wildmatch.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "strbuf.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "url.h"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OPT_QUIET (1 << 0)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OPT_CACHED (1 << 1)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OPT_RECURSIVE (1 << 2)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OPT_FORCE (1 << 3)

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (*each_submodule_fn)(ਅਚਲ ਬਣਤਰ cache_ਇੰਦਰਾਜ *list_ਵਸਤੂ,
				  ਖਾਲੀ *cb_ਅੰਕੜੇ);

ਸਥਿਰ ਅੱਖਰ *get_default_remote(ਖਾਲੀ)
{
	ਵਾਪਸ xstrdup(repo_default_remote(the_repository));
}

ਸਥਿਰ ਅੱਖਰ *resolve_relative_url(ਅਚਲ ਅੱਖਰ *rel_url, ਅਚਲ ਅੱਖਰ *ਉੱਪਰ_ਰਸਤਾ, ਪੂਰਨ quiet)
{
	ਅੱਖਰ *remoteurl, *resolved_url;
	ਅੱਖਰ *remote = get_default_remote();
	ਬਣਤਰ strbuf remotesb = STRBUF_INIT;

	strbuf_addf(&remotesb, "remote.%s.url", remote);
	ਜੇ (repo_config_get_string(the_repository, remotesb.buf, &remoteurl)) {
		ਜੇ (!quiet)
			warning(_("could not look up configuration '%s'. "
				  "Assuming this repository is its own "
				  "authoritative upstream."),
				remotesb.buf);
		remoteurl = xgetcwd();
	}
	resolved_url = relative_url(remoteurl, rel_url, ਉੱਪਰ_ਰਸਤਾ);

	ਮੁਕਤ(remote);
	ਮੁਕਤ(remoteurl);
	strbuf_release(&remotesb);

	ਵਾਪਸ resolved_url;
}

ਸਥਿਰ ਪੂਰਨ get_default_remote_submodule(ਅਚਲ ਅੱਖਰ *module_ਰਸਤਾ, ਅੱਖਰ **default_remote)
{
	ਅਚਲ ਬਣਤਰ submodule *sub;
	ਬਣਤਰ repository subrepo;
	ਅਚਲ ਅੱਖਰ *remote_name = ਨਿਰਾ;
	ਅੱਖਰ *url = ਨਿਰਾ;
	ਪੂਰਨ ret = 0;

	sub = submodule_from_ਰਸਤਾ(the_repository, null_oid(the_hash_algo), module_ਰਸਤਾ);
	ਜੇ (sub && sub->url) {
		url = xstrdup(sub->url);

		/* Possibly a url relative to parent */
		ਜੇ (starts_with_dot_dot_slash(url) ||
		    starts_with_dot_slash(url)) {
			ਅੱਖਰ *oldurl = url;

			url = resolve_relative_url(oldurl, ਨਿਰਾ, 1);
			ਮੁਕਤ(oldurl);
		}
	}

	ਜੇ (repo_submodule_init(&subrepo, the_repository, module_ਰਸਤਾ,
				null_oid(the_hash_algo)) < 0) {
		ret = die_ਸੁਨੇਹਾ(_("could not get a repository handle for submodule '%s'"),
				   module_ਰਸਤਾ);
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	/* Look up by URL first */
	ਜੇ (url)
		remote_name = repo_remote_from_url(&subrepo, url);
	ਜੇ (!remote_name)
		remote_name = repo_default_remote(&subrepo);

	*default_remote = xstrdup(remote_name);

ਬਾਹਰਲਾ:
	repo_clear(&subrepo);
	ਮੁਕਤ(url);

	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ module_get_default_remote(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
				     ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਅਚਲ ਅੱਖਰ *ਰਸਤਾ;
	ਅੱਖਰ *resolved_ਰਸਤਾ = ਨਿਰਾ;
	ਅੱਖਰ *default_remote = ਨਿਰਾ;
	ਪੂਰਨ ਕੋਡ;
	ਬਣਤਰ option options[] = {
		OPT_ਸਿਰਾ()
	};
	ਅਚਲ ਅੱਖਰ *ਅਚਲ usage[] = {
		N_("git submodule--helper get-default-remote <path>"),
		ਨਿਰਾ
	};

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, usage, 0);
	ਜੇ (argc != 1)
		usage_with_options(usage, options);

	ਰਸਤਾ = argv[0];
	ਜੇ (ਅਗੇਤਰ && *ਅਗੇਤਰ && !is_absolute_ਰਸਤਾ(ਰਸਤਾ)) {
		resolved_ਰਸਤਾ = xstrfmt("%s%s", ਅਗੇਤਰ, ਰਸਤਾ);
		ਰਸਤਾ = resolved_ਰਸਤਾ;
	}

	ਕੋਡ = get_default_remote_submodule(ਰਸਤਾ, &default_remote);
	ਜੇ (ਕੋਡ) {
		ਮੁਕਤ(resolved_ਰਸਤਾ);
		ਵਾਪਸ ਕੋਡ;
	}

	ਲਿਖੋ("%s\n", default_remote);
	ਮੁਕਤ(default_remote);
	ਮੁਕਤ(resolved_ਰਸਤਾ);
	ਵਾਪਸ 0;
}

/* the result should be freed by the caller. */
ਸਥਿਰ ਅੱਖਰ *get_submodule_displaypath(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
				       ਅਚਲ ਅੱਖਰ *super_ਅਗੇਤਰ)
{
	ਜੇ (ਅਗੇਤਰ && super_ਅਗੇਤਰ) {
		BUG("cannot have prefix '%s' and superprefix '%s'",
		    ਅਗੇਤਰ, super_ਅਗੇਤਰ);
	} ਨਹੀਂਤਾਂ ਜੇ (ਅਗੇਤਰ) {
		ਬਣਤਰ strbuf sb = STRBUF_INIT;
		ਅੱਖਰ *displaypath = xstrdup(relative_ਰਸਤਾ(ਰਸਤਾ, ਅਗੇਤਰ, &sb));
		strbuf_release(&sb);
		ਵਾਪਸ displaypath;
	} ਨਹੀਂਤਾਂ ਜੇ (super_ਅਗੇਤਰ) {
		ਵਾਪਸ xstrfmt("%s%s", super_ਅਗੇਤਰ, ਰਸਤਾ);
	} ਨਹੀਂਤਾਂ {
		ਵਾਪਸ xstrdup(ਰਸਤਾ);
	}
}

ਸਥਿਰ ਅੱਖਰ *compute_rev_name(ਅਚਲ ਅੱਖਰ *sub_ਰਸਤਾ, ਅਚਲ ਅੱਖਰ* object_id)
{
	ਬਣਤਰ strbuf sb = STRBUF_INIT;
	ਅਚਲ ਅੱਖਰ ***d;

	ਸਥਿਰ ਅਚਲ ਅੱਖਰ *describe_bare[] = { ਨਿਰਾ };

	ਸਥਿਰ ਅਚਲ ਅੱਖਰ *describe_ਟੈਗ_ਸੂਚੀ[] = { "--tags", ਨਿਰਾ };

	ਸਥਿਰ ਅਚਲ ਅੱਖਰ *describe_ਸ਼ਾਮਲ_ਹੈ[] = { "--contains", ਨਿਰਾ };

	ਸਥਿਰ ਅਚਲ ਅੱਖਰ *describe_all_always[] = { "--all", "--always", ਨਿਰਾ };

	ਸਥਿਰ ਅਚਲ ਅੱਖਰ **describe_argv[] = { describe_bare, describe_ਟੈਗ_ਸੂਚੀ,
						describe_ਸ਼ਾਮਲ_ਹੈ,
						describe_all_always, ਨਿਰਾ };

	ਲਈ (d = describe_argv; *d; d++) {
		ਬਣਤਰ ਬੱਚਾ_process cp = ਬੱਚਾ_PROCESS_INIT;
		prepare_submodule_repo_env(&cp.env);
		cp.dir = sub_ਰਸਤਾ;
		cp.git_cmd = 1;
		cp.no_stderr = 1;

		strvec_ਧੱਕੋ(&cp.ਦਲੀਲਾਂ, "describe");
		strvec_pushv(&cp.ਦਲੀਲਾਂ, *d);
		strvec_ਧੱਕੋ(&cp.ਦਲੀਲਾਂ, object_id);

		ਜੇ (!capture_command(&cp, &sb, 0)) {
			strbuf_strip_suffix(&sb, "\n");
			ਵਾਪਸ strbuf_detach(&sb, ਨਿਰਾ);
		}
	}

	strbuf_release(&sb);
	ਵਾਪਸ ਨਿਰਾ;
}

ਬਣਤਰ module_list {
	ਅਚਲ ਬਣਤਰ cache_ਇੰਦਰਾਜ **entries;
	ਪੂਰਨ alloc, nr;
};
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MODULE_LIST_INIT { 0 }

ਸਥਿਰ ਖਾਲੀ module_list_release(ਬਣਤਰ module_list *ml)
{
	ਮੁਕਤ(ml->entries);
}

ਸਥਿਰ ਪੂਰਨ module_list_compute(ਅਚਲ ਅੱਖਰ **argv,
			       ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			       ਬਣਤਰ pathspec *pathspec,
			       ਬਣਤਰ module_list *list)
{
	ਪੂਰਨ ਨਤੀਜਾ = 0;
	ਅੱਖਰ *ps_matched = ਨਿਰਾ;

	parse_pathspec(pathspec, 0,
		       PATHSPEC_PREFER_ਭਰਿਆ,
		       ਅਗੇਤਰ, argv);

	ਜੇ (pathspec->nr)
		ps_matched = xcalloc(pathspec->nr, 1);

	ਜੇ (repo_read_ਸੂਚਕਾਂਕ(the_repository) < 0)
		die(_("index file corrupt"));

	ਲਈ (ਮਾਪ_t i = 0; i < the_repository->ਸੂਚਕਾਂਕ->cache_nr; i++) {
		ਅਚਲ ਬਣਤਰ cache_ਇੰਦਰਾਜ *ce = the_repository->ਸੂਚਕਾਂਕ->cache[i];

		ਜੇ (!ਮੇਲ_pathspec(the_repository->ਸੂਚਕਾਂਕ, pathspec, ce->name, ce_namelen(ce),
				    0, ps_matched, 1) ||
		    !S_ISGITLINK(ce->ce_ਬਹੁਲਕ))
			ਜਾਰੀ;

		ALLOC_GROW(list->entries, list->nr + 1, list->alloc);
		list->entries[list->nr++] = ce;
		ਜਦੋਂ (i + 1 < the_repository->ਸੂਚਕਾਂਕ->cache_nr &&
		       !strcmp(ce->name, the_repository->ਸੂਚਕਾਂਕ->cache[i + 1]->name))
			/*
			 * Skip entries with the same name in different stages
			 * to make sure an entry is returned only once.
			 */
			i++;
	}

	ਜੇ (ps_matched && report_ਰਸਤਾ_error(ps_matched, pathspec))
		ਨਤੀਜਾ = -1;

	ਮੁਕਤ(ps_matched);

	ਵਾਪਸ ਨਤੀਜਾ;
}

ਸਥਿਰ ਖਾਲੀ module_list_ਸਰਗਰਮ(ਬਣਤਰ module_list *list)
{
	ਪੂਰਨ i;
	ਬਣਤਰ module_list ਸਰਗਰਮ_modules = MODULE_LIST_INIT;

	ਲਈ (i = 0; i < list->nr; i++) {
		ਅਚਲ ਬਣਤਰ cache_ਇੰਦਰਾਜ *ce = list->entries[i];

		ਜੇ (!is_submodule_ਸਰਗਰਮ(the_repository, ce->name))
			ਜਾਰੀ;

		ALLOC_GROW(ਸਰਗਰਮ_modules.entries,
			   ਸਰਗਰਮ_modules.nr + 1,
			   ਸਰਗਰਮ_modules.alloc);
		ਸਰਗਰਮ_modules.entries[ਸਰਗਰਮ_modules.nr++] = ce;
	}

	module_list_release(list);
	*list = ਸਰਗਰਮ_modules;
}

ਸਥਿਰ ਅੱਖਰ *get_ਉੱਪਰ_ਰਸਤਾ(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ)
{
	ਬਣਤਰ strbuf sb = STRBUF_INIT;

	strbuf_addstrings(&sb, "../", ਗਿਣਤੀ_slashes(ਰਸਤਾ));

	/*
	 * Check if 'path' ends with slash or not
	 * for having the same output for dir/sub_dir
	 * and dir/sub_dir/
	 */
	ਜੇ (!is_dir_sep(ਰਸਤਾ[ਲੜੀ_ਲੰਬਾਈ(ਰਸਤਾ) - 1]))
		strbuf_addstr(&sb, "../");

	ਵਾਪਸ strbuf_detach(&sb, ਨਿਰਾ);
}

ਸਥਿਰ ਖਾਲੀ for_each_listed_submodule(ਅਚਲ ਬਣਤਰ module_list *list,
				      each_submodule_fn fn, ਖਾਲੀ *cb_ਅੰਕੜੇ)
{
	ਪੂਰਨ i;

	ਲਈ (i = 0; i < list->nr; i++)
		fn(list->entries[i], cb_ਅੰਕੜੇ);
}

ਬਣਤਰ foreach_cb {
	ਪੂਰਨ argc;
	ਅਚਲ ਅੱਖਰ **argv;
	ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ;
	ਅਚਲ ਅੱਖਰ *super_ਅਗੇਤਰ;
	ਪੂਰਨ quiet;
	ਪੂਰਨ recursive;
};
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FOREACH_CB_INIT { 0 }

ਸਥਿਰ ਖਾਲੀ runcommand_ਵਿੱਚ_submodule_cb(ਅਚਲ ਬਣਤਰ cache_ਇੰਦਰਾਜ *list_ਵਸਤੂ,
				       ਖਾਲੀ *cb_ਅੰਕੜੇ)
{
	ਬਣਤਰ foreach_cb *info = cb_ਅੰਕੜੇ;
	ਅਚਲ ਅੱਖਰ *ਰਸਤਾ = list_ਵਸਤੂ->name;
	ਅਚਲ ਬਣਤਰ object_id *ce_oid = &list_ਵਸਤੂ->oid;
	ਅਚਲ ਬਣਤਰ submodule *sub;
	ਬਣਤਰ ਬੱਚਾ_process cp = ਬੱਚਾ_PROCESS_INIT;
	ਅੱਖਰ *displaypath;

	ਜੇ (validate_submodule_ਰਸਤਾ(ਰਸਤਾ) < 0)
		die(ਨਿਰਾ);

	displaypath = get_submodule_displaypath(ਰਸਤਾ, info->ਅਗੇਤਰ,
						info->super_ਅਗੇਤਰ);

	sub = submodule_from_ਰਸਤਾ(the_repository, null_oid(the_hash_algo), ਰਸਤਾ);

	ਜੇ (!sub)
		die(_("No url found for submodule path '%s' in .gitmodules"),
			displaypath);

	ਜੇ (!is_submodule_populated_gently(ਰਸਤਾ, ਨਿਰਾ))
		ਜਾਓ cleanup;

	prepare_submodule_repo_env(&cp.env);

	/*
	 * For the purpose of executing <command> in the submodule,
	 * separate shell is used for the purpose of running the
	 * child process.
	 */
	cp.use_shell = 1;
	cp.dir = ਰਸਤਾ;

	/*
	 * NEEDSWORK: the command currently has access to the variables $name,
	 * $sm_path, $displaypath, $sha1 and $toplevel only when the command
	 * contains a single argument. This is done for maintaining a faithful
	 * translation from shell script.
	 */
	ਜੇ (info->argc == 1) {
		ਅੱਖਰ *toplevel = xgetcwd();
		ਬਣਤਰ strbuf sb = STRBUF_INIT;

		strvec_pushf(&cp.env, "name=%s", sub->name);
		strvec_pushf(&cp.env, "sm_path=%s", ਰਸਤਾ);
		strvec_pushf(&cp.env, "displaypath=%s", displaypath);
		strvec_pushf(&cp.env, "sha1=%s",
			     oid_ਮੰਜ਼ਿਲ_hex(ce_oid));
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
		sq_quote_buf(&sb, ਰਸਤਾ);
		strvec_pushf(&cp.ਦਲੀਲਾਂ, "path=%s; %s",
			     sb.buf, info->argv[0]);
		strbuf_release(&sb);
		ਮੁਕਤ(toplevel);
	} ਨਹੀਂਤਾਂ {
		strvec_pushv(&cp.ਦਲੀਲਾਂ, info->argv);
	}

	ਜੇ (!info->quiet)
		ਲਿਖੋ(_("Entering '%s'\n"), displaypath);

	ਜੇ (info->argv[0]) {
		ਜੇ (ਲੜੀਵਾਰ_command(&cp))
			die(_("run_command returned non-zero status for %s\n."),
			    displaypath);
	} ਨਹੀਂਤਾਂ {
		ਬੱਚਾ_process_clear(&cp);
	}

	ਜੇ (info->recursive) {
		ਬਣਤਰ ਬੱਚਾ_process cpr = ਬੱਚਾ_PROCESS_INIT;

		cpr.git_cmd = 1;
		cpr.dir = ਰਸਤਾ;
		prepare_submodule_repo_env(&cpr.env);

		strvec_pushl(&cpr.ਦਲੀਲਾਂ, "submodule--helper", "foreach", "--recursive",
			     ਨਿਰਾ);
		strvec_pushf(&cpr.ਦਲੀਲਾਂ, "--super-prefix=%s/", displaypath);

		ਜੇ (info->quiet)
			strvec_ਧੱਕੋ(&cpr.ਦਲੀਲਾਂ, "--quiet");

		strvec_ਧੱਕੋ(&cpr.ਦਲੀਲਾਂ, "--");
		strvec_pushv(&cpr.ਦਲੀਲਾਂ, info->argv);

		ਜੇ (ਲੜੀਵਾਰ_command(&cpr))
			die(_("run_command returned non-zero status while "
				"recursing in the nested submodules of %s\n."),
				displaypath);
	}

cleanup:
	ਮੁਕਤ(displaypath);
}

ਸਥਿਰ ਪੂਰਨ module_foreach(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			  ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ foreach_cb info = FOREACH_CB_INIT;
	ਬਣਤਰ pathspec pathspec = { 0 };
	ਬਣਤਰ module_list list = MODULE_LIST_INIT;
	ਬਣਤਰ option module_foreach_options[] = {
		OPT_SUPER_ਅਗੇਤਰ(&info.super_ਅਗੇਤਰ),
		OPT__QUIET(&info.quiet, N_("suppress output of entering each submodule command")),
		OPT_BOOL(0, "recursive", &info.recursive,
			 N_("recurse into nested submodules")),
		OPT_ਸਿਰਾ()
	};
	ਅਚਲ ਅੱਖਰ *ਅਚਲ git_submodule_helper_usage[] = {
		N_("git submodule foreach [--quiet] [--recursive] [--] <command>"),
		ਨਿਰਾ
	};
	ਪੂਰਨ ret = 1;

	argc = parse_options(argc, argv, ਅਗੇਤਰ, module_foreach_options,
			     git_submodule_helper_usage, 0);

	ਜੇ (module_list_compute(ਨਿਰਾ, ਅਗੇਤਰ, &pathspec, &list) < 0)
		ਜਾਓ cleanup;

	info.argc = argc;
	info.argv = argv;
	info.ਅਗੇਤਰ = ਅਗੇਤਰ;

	for_each_listed_submodule(&list, runcommand_ਵਿੱਚ_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	ਵਾਪਸ ret;
}

ਬਣਤਰ init_cb {
	ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ;
	ਅਚਲ ਅੱਖਰ *super_ਅਗੇਤਰ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags;
};
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ INIT_CB_INIT { 0 }

ਸਥਿਰ ਪੂਰਨ validate_and_ਸੈੱਟ_submodule_gitdir(ਬਣਤਰ strbuf *gitdir_ਰਸਤਾ,
					     ਅਚਲ ਅੱਖਰ *submodule_name)
{
	ਅਚਲ ਅੱਖਰ *ਮੁੱਲ;
	ਅੱਖਰ *ਕੁੰਜੀ;

	ਜੇ (validate_submodule_git_dir(gitdir_ਰਸਤਾ->buf, submodule_name))
		ਵਾਪਸ -1;

	ਕੁੰਜੀ = xstrfmt("submodule.%s.gitdir", submodule_name);

	/* Nothing to do if the config already exists. */
	ਜੇ (!repo_config_get_string_tmp(the_repository, ਕੁੰਜੀ, &ਮੁੱਲ)) {
		ਮੁਕਤ(ਕੁੰਜੀ);
		ਵਾਪਸ 0;
	}

	ਜੇ (repo_config_ਸੈੱਟ_gently(the_repository, ਕੁੰਜੀ, gitdir_ਰਸਤਾ->buf)) {
		ਮੁਕਤ(ਕੁੰਜੀ);
		ਵਾਪਸ -1;
	}

	ਮੁਕਤ(ਕੁੰਜੀ);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ create_default_gitdir_config(ਅਚਲ ਅੱਖਰ *submodule_name)
{
	ਬਣਤਰ strbuf gitdir_ਰਸਤਾ = STRBUF_INIT;
	ਬਣਤਰ git_hash_ctx ctx;
	ਅੱਖਰ hex_name_hash[GIT_MAX_HEXSZ + 1], ਸਿਰਲੇਖ[128];
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ raw_name_hash[GIT_MAX_RAWSZ];
	ਪੂਰਨ ਸਿਰਲੇਖ_len;

	/* Case 1: try the plain module name */
	repo_git_ਰਸਤਾ_append(the_repository, &gitdir_ਰਸਤਾ, "modules/%s", submodule_name);
	ਜੇ (!validate_and_ਸੈੱਟ_submodule_gitdir(&gitdir_ਰਸਤਾ, submodule_name)) {
		strbuf_release(&gitdir_ਰਸਤਾ);
		ਵਾਪਸ;
	}

	/* Case 2.1: Try URI-safe (RFC3986) encoding first, this fixes nested gitdirs */
	strbuf_reset(&gitdir_ਰਸਤਾ);
	repo_git_ਰਸਤਾ_append(the_repository, &gitdir_ਰਸਤਾ, "modules/");
	strbuf_addstr_urlencode(&gitdir_ਰਸਤਾ, submodule_name, is_rfc3986_unreserved);
	ਜੇ (!validate_and_ਸੈੱਟ_submodule_gitdir(&gitdir_ਰਸਤਾ, submodule_name)) {
		strbuf_release(&gitdir_ਰਸਤਾ);
		ਵਾਪਸ;
	}

	/* Case 2.2: Try extended uppercase URI (RFC3986) encoding, to fix case-folding */
	strbuf_reset(&gitdir_ਰਸਤਾ);
	repo_git_ਰਸਤਾ_append(the_repository, &gitdir_ਰਸਤਾ, "modules/");
	strbuf_addstr_urlencode(&gitdir_ਰਸਤਾ, submodule_name, is_casefolding_rfc3986_unreserved);
	ਜੇ (!validate_and_ਸੈੱਟ_submodule_gitdir(&gitdir_ਰਸਤਾ, submodule_name))
		ਵਾਪਸ;

	/* Case 2.3: Try some derived gitdir names, see if one sticks */
	ਲਈ (ਅੱਖਰ c = '0'; c <= '9'; c++) {
		strbuf_reset(&gitdir_ਰਸਤਾ);
		repo_git_ਰਸਤਾ_append(the_repository, &gitdir_ਰਸਤਾ, "modules/");
		strbuf_addstr_urlencode(&gitdir_ਰਸਤਾ, submodule_name, is_rfc3986_unreserved);
		strbuf_addch(&gitdir_ਰਸਤਾ, c);
		ਜੇ (!validate_and_ਸੈੱਟ_submodule_gitdir(&gitdir_ਰਸਤਾ, submodule_name))
			ਵਾਪਸ;

		strbuf_reset(&gitdir_ਰਸਤਾ);
		repo_git_ਰਸਤਾ_append(the_repository, &gitdir_ਰਸਤਾ, "modules/");
		strbuf_addstr_urlencode(&gitdir_ਰਸਤਾ, submodule_name, is_casefolding_rfc3986_unreserved);
		strbuf_addch(&gitdir_ਰਸਤਾ, c);
		ਜੇ (!validate_and_ਸੈੱਟ_submodule_gitdir(&gitdir_ਰਸਤਾ, submodule_name))
			ਵਾਪਸ;
	}

	/* Case 2.4: If all the above failed, try a hash of the name as a last resort */
	ਸਿਰਲੇਖ_len = snprintf(ਸਿਰਲੇਖ, ਆਕਾਰ(ਸਿਰਲੇਖ),
			      "blob %"PRIuMAX, (uintmax_t)ਲੜੀ_ਲੰਬਾਈ(submodule_name));
	git_hash_init(&ctx, the_hash_algo);
	git_hash_update(&ctx, ਸਿਰਲੇਖ, ਸਿਰਲੇਖ_len);
	git_hash_update(&ctx, "\0", 1);
	git_hash_update(&ctx, submodule_name, ਲੜੀ_ਲੰਬਾਈ(submodule_name));
	git_hash_final(raw_name_hash, &ctx);
	hash_ਮੰਜ਼ਿਲ_hex_algop_r(hex_name_hash, raw_name_hash, the_hash_algo);
	strbuf_reset(&gitdir_ਰਸਤਾ);
	repo_git_ਰਸਤਾ_append(the_repository, &gitdir_ਰਸਤਾ, "modules/%s", hex_name_hash);
	ਜੇ (!validate_and_ਸੈੱਟ_submodule_gitdir(&gitdir_ਰਸਤਾ, submodule_name)) {
		strbuf_release(&gitdir_ਰਸਤਾ);
		ਵਾਪਸ;
	}

	/* Case 3: nothing worked, error out */
	die(_("failed to set a valid default config for 'submodule.%s.gitdir'. "
	      "Please ensure it is set, for example by running something like: "
	      "'git config submodule.%s.gitdir .git/modules/%s'"),
	    submodule_name, submodule_name, submodule_name);
}

ਸਥਿਰ ਖਾਲੀ init_submodule(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			   ਅਚਲ ਅੱਖਰ *super_ਅਗੇਤਰ,
			   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags)
{
	ਅਚਲ ਬਣਤਰ submodule *sub;
	ਬਣਤਰ strbuf sb = STRBUF_INIT;
	ਅਚਲ ਅੱਖਰ *upd;
	ਅੱਖਰ *url = ਨਿਰਾ, *displaypath;

	displaypath = get_submodule_displaypath(ਰਸਤਾ, ਅਗੇਤਰ, super_ਅਗੇਤਰ);

	sub = submodule_from_ਰਸਤਾ(the_repository, null_oid(the_hash_algo), ਰਸਤਾ);

	ਜੇ (!sub)
		die(_("No url found for submodule path '%s' in .gitmodules"),
			displaypath);

	/*
	 * NEEDSWORK: In a multi-working-tree world, this needs to be
	 * set in the per-worktree config.
	 *
	 * Set active flag for the submodule being initialized
	 */
	ਜੇ (!is_submodule_ਸਰਗਰਮ(the_repository, ਰਸਤਾ)) {
		strbuf_addf(&sb, "submodule.%s.active", sub->name);
		repo_config_ਸੈੱਟ_gently(the_repository, sb.buf, "true");
		strbuf_reset(&sb);
	}

	/*
	 * Copy url setting when it is not set yet.
	 * To look up the url in .git/config, we must not fall back to
	 * .gitmodules, so look it up directly.
	 */
	strbuf_addf(&sb, "submodule.%s.url", sub->name);
	ਜੇ (repo_config_get_string(the_repository, sb.buf, &url)) {
		ਜੇ (!sub->url)
			die(_("No url found for submodule path '%s' in .gitmodules"),
				displaypath);

		url = xstrdup(sub->url);

		/* Possibly a url relative to parent */
		ਜੇ (starts_with_dot_dot_slash(url) ||
		    starts_with_dot_slash(url)) {
			ਅੱਖਰ *oldurl = url;

			url = resolve_relative_url(oldurl, ਨਿਰਾ, 0);
			ਮੁਕਤ(oldurl);
		}

		ਜੇ (repo_config_ਸੈੱਟ_gently(the_repository, sb.buf, url))
			die(_("Failed to register url for submodule path '%s'"),
			    displaypath);
		ਜੇ (!(flags & OPT_QUIET))
			fprintf(stderr,
				_("Submodule '%s' (%s) registered for path '%s'\n"),
				sub->name, url, displaypath);
	}
	strbuf_reset(&sb);

	/* Copy "update" setting when it is not set yet */
	strbuf_addf(&sb, "submodule.%s.update", sub->name);
	ਜੇ (repo_config_get_string_tmp(the_repository, sb.buf, &upd) &&
	    sub->update_strategy.type != SM_UPDATE_UNSPECIFIED) {
		ਜੇ (sub->update_strategy.type == SM_UPDATE_COMMAND) {
			fprintf(stderr, _("warning: command update mode suggested for submodule '%s'\n"),
				sub->name);
			upd = "none";
		} ਨਹੀਂਤਾਂ {
			upd = submodule_update_type_ਮੰਜ਼ਿਲ_string(sub->update_strategy.type);
		}

		ਜੇ (repo_config_ਸੈੱਟ_gently(the_repository, sb.buf, upd))
			die(_("Failed to register update mode for submodule path '%s'"), displaypath);
	}

	ਜੇ (the_repository->repository_format_submodule_ਰਸਤਾ_cfg)
		create_default_gitdir_config(sub->name);

	strbuf_release(&sb);
	ਮੁਕਤ(displaypath);
	ਮੁਕਤ(url);
}

ਸਥਿਰ ਖਾਲੀ init_submodule_cb(ਅਚਲ ਬਣਤਰ cache_ਇੰਦਰਾਜ *list_ਵਸਤੂ, ਖਾਲੀ *cb_ਅੰਕੜੇ)
{
	ਬਣਤਰ init_cb *info = cb_ਅੰਕੜੇ;

	init_submodule(list_ਵਸਤੂ->name, info->ਅਗੇਤਰ, info->super_ਅਗੇਤਰ,
		       info->flags);
}

ਸਥਿਰ ਪੂਰਨ module_init(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		       ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ init_cb info = INIT_CB_INIT;
	ਬਣਤਰ pathspec pathspec = { 0 };
	ਬਣਤਰ module_list list = MODULE_LIST_INIT;
	ਪੂਰਨ quiet = 0;
	ਬਣਤਰ option module_init_options[] = {
		OPT__QUIET(&quiet, N_("suppress output for initializing a submodule")),
		OPT_ਸਿਰਾ()
	};
	ਅਚਲ ਅੱਖਰ *ਅਚਲ git_submodule_helper_usage[] = {
		N_("git submodule init [<options>] [<path>]"),
		ਨਿਰਾ
	};
	ਪੂਰਨ ret = 1;

	argc = parse_options(argc, argv, ਅਗੇਤਰ, module_init_options,
			     git_submodule_helper_usage, 0);

	ਜੇ (module_list_compute(argv, ਅਗੇਤਰ, &pathspec, &list) < 0)
		ਜਾਓ cleanup;

	/*
	 * If there are no path args and submodule.active is set then,
	 * by default, only initialize 'active' modules.
	 */
	ਜੇ (!argc && !repo_config_get(the_repository, "submodule.active"))
		module_list_ਸਰਗਰਮ(&list);

	info.ਅਗੇਤਰ = ਅਗੇਤਰ;
	ਜੇ (quiet)
		info.flags |= OPT_QUIET;

	for_each_listed_submodule(&list, init_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	ਵਾਪਸ ret;
}

ਬਣਤਰ ਸਥਿਤੀ_cb {
	ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ;
	ਅਚਲ ਅੱਖਰ *super_ਅਗੇਤਰ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags;
};
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਸਥਿਤੀ_CB_INIT { 0 }

ਸਥਿਰ ਖਾਲੀ print_ਸਥਿਤੀ(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags, ਅੱਖਰ state, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ,
			 ਅਚਲ ਬਣਤਰ object_id *oid, ਅਚਲ ਅੱਖਰ *displaypath)
{
	ਜੇ (flags & OPT_QUIET)
		ਵਾਪਸ;

	ਲਿਖੋ("%c%s %s", state, oid_ਮੰਜ਼ਿਲ_hex(oid), displaypath);

	ਜੇ (state == ' ' || state == '+') {
		ਅੱਖਰ *name = compute_rev_name(ਰਸਤਾ, oid_ਮੰਜ਼ਿਲ_hex(oid));

		ਜੇ (name)
			ਲਿਖੋ(" (%s)", name);
		ਮੁਕਤ(name);
	}

	ਲਿਖੋ("\n");
}

ਸਥਿਰ ਪੂਰਨ handle_submodule_ਸਿਰ_ref(ਅਚਲ ਬਣਤਰ reference *ref, ਖਾਲੀ *cb_ਅੰਕੜੇ)
{
	ਬਣਤਰ object_id *output = cb_ਅੰਕੜੇ;

	ਜੇ (ref->oid)
		oidcpy(output, ref->oid);

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ ਸਥਿਤੀ_submodule(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਅਚਲ ਬਣਤਰ object_id *ce_oid,
			     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ce_flags, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			     ਅਚਲ ਅੱਖਰ *super_ਅਗੇਤਰ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags)
{
	ਅੱਖਰ *displaypath;
	ਬਣਤਰ strvec diff_files_ਦਲੀਲਾਂ = STRVEC_INIT;
	ਬਣਤਰ rev_info rev = REV_INFO_INIT;
	ਬਣਤਰ strbuf buf = STRBUF_INIT;
	ਅਚਲ ਅੱਖਰ *git_dir;

	ਜੇ (validate_submodule_ਰਸਤਾ(ਰਸਤਾ) < 0)
		die(ਨਿਰਾ);

	ਜੇ (!submodule_from_ਰਸਤਾ(the_repository, null_oid(the_hash_algo), ਰਸਤਾ))
		die(_("no submodule mapping found in .gitmodules for path '%s'"),
		      ਰਸਤਾ);

	displaypath = get_submodule_displaypath(ਰਸਤਾ, ਅਗੇਤਰ, super_ਅਗੇਤਰ);

	ਜੇ ((CE_STAGEMASK & ce_flags) >> CE_STAGESHIFT) {
		print_ਸਥਿਤੀ(flags, 'U', ਰਸਤਾ, null_oid(the_hash_algo), displaypath);
		ਜਾਓ cleanup;
	}

	strbuf_addf(&buf, "%s/.git", ਰਸਤਾ);
	git_dir = read_gitfile(buf.buf);
	ਜੇ (!git_dir)
		git_dir = buf.buf;

	ਜੇ (!is_submodule_ਸਰਗਰਮ(the_repository, ਰਸਤਾ) ||
	    !is_git_directory(git_dir)) {
		print_ਸਥਿਤੀ(flags, '-', ਰਸਤਾ, ce_oid, displaypath);
		strbuf_release(&buf);
		ਜਾਓ cleanup;
	}
	strbuf_release(&buf);

	strvec_pushl(&diff_files_ਦਲੀਲਾਂ, "diff-files",
		     "--ignore-submodules=dirty", "--quiet", "--",
		     ਰਸਤਾ, ਨਿਰਾ);

	repo_config(the_repository, git_diff_basic_config, ਨਿਰਾ);

	repo_init_revisions(the_repository, &rev, ਨਿਰਾ);
	rev.abbrev = 0;
	ਤਿਆਰੀ_revisions_from_strvec(&diff_files_ਦਲੀਲਾਂ, &rev, ਨਿਰਾ);
	ਲੜੀਵਾਰ_diff_files(&rev, 0);

	ਜੇ (!diff_ਨਤੀਜਾ_ਕੋਡ(&rev)) {
		print_ਸਥਿਤੀ(flags, ' ', ਰਸਤਾ, ce_oid,
			     displaypath);
	} ਨਹੀਂਤਾਂ ਜੇ (!(flags & OPT_CACHED)) {
		ਬਣਤਰ object_id oid;
		ਬਣਤਰ ref_store *refs = repo_get_submodule_ref_store(the_repository,
								      ਰਸਤਾ);

		ਜੇ (!refs) {
			print_ਸਥਿਤੀ(flags, '-', ਰਸਤਾ, ce_oid, displaypath);
			ਜਾਓ cleanup;
		}
		ਜੇ (refs_ਸਿਰ_ref(refs, handle_submodule_ਸਿਰ_ref, &oid))
			die(_("could not resolve HEAD ref inside the "
			      "submodule '%s'"), ਰਸਤਾ);

		print_ਸਥਿਤੀ(flags, '+', ਰਸਤਾ, &oid, displaypath);
	} ਨਹੀਂਤਾਂ {
		print_ਸਥਿਤੀ(flags, '+', ਰਸਤਾ, ce_oid, displaypath);
	}

	ਜੇ (flags & OPT_RECURSIVE) {
		ਬਣਤਰ ਬੱਚਾ_process cpr = ਬੱਚਾ_PROCESS_INIT;
		ਪੂਰਨ res;

		cpr.git_cmd = 1;
		cpr.dir = ਰਸਤਾ;
		prepare_submodule_repo_env(&cpr.env);

		strvec_pushl(&cpr.ਦਲੀਲਾਂ, "submodule--helper", "status",
			     "--recursive", ਨਿਰਾ);
		strvec_pushf(&cpr.ਦਲੀਲਾਂ, "--super-prefix=%s/", displaypath);

		ਜੇ (flags & OPT_CACHED)
			strvec_ਧੱਕੋ(&cpr.ਦਲੀਲਾਂ, "--cached");

		ਜੇ (flags & OPT_QUIET)
			strvec_ਧੱਕੋ(&cpr.ਦਲੀਲਾਂ, "--quiet");

		res = ਲੜੀਵਾਰ_command(&cpr);
		ਜੇ (res == SIGPIPE + 128)
			ਉਠਾਓ(SIGPIPE);
		ਨਹੀਂਤਾਂ ਜੇ (res)
			die(_("failed to recurse into submodule '%s'"), ਰਸਤਾ);
	}

cleanup:
	strvec_clear(&diff_files_ਦਲੀਲਾਂ);
	ਮੁਕਤ(displaypath);
	release_revisions(&rev);
}

ਸਥਿਰ ਖਾਲੀ ਸਥਿਤੀ_submodule_cb(ਅਚਲ ਬਣਤਰ cache_ਇੰਦਰਾਜ *list_ਵਸਤੂ,
				ਖਾਲੀ *cb_ਅੰਕੜੇ)
{
	ਬਣਤਰ ਸਥਿਤੀ_cb *info = cb_ਅੰਕੜੇ;

	ਸਥਿਤੀ_submodule(list_ਵਸਤੂ->name, &list_ਵਸਤੂ->oid, list_ਵਸਤੂ->ce_flags,
			 info->ਅਗੇਤਰ, info->super_ਅਗੇਤਰ, info->flags);
}

ਸਥਿਰ ਪੂਰਨ module_ਸਥਿਤੀ(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			 ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ ਸਥਿਤੀ_cb info = ਸਥਿਤੀ_CB_INIT;
	ਬਣਤਰ pathspec pathspec = { 0 };
	ਬਣਤਰ module_list list = MODULE_LIST_INIT;
	ਪੂਰਨ quiet = 0;
	ਬਣਤਰ option module_ਸਥਿਤੀ_options[] = {
		OPT_SUPER_ਅਗੇਤਰ(&info.super_ਅਗੇਤਰ),
		OPT__QUIET(&quiet, N_("suppress submodule status output")),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ(0, "cached", &info.flags, N_("use commit stored in the index instead of the one stored in the submodule HEAD"), OPT_CACHED),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ(0, "recursive", &info.flags, N_("recurse into nested submodules"), OPT_RECURSIVE),
		OPT_ਸਿਰਾ()
	};
	ਅਚਲ ਅੱਖਰ *ਅਚਲ git_submodule_helper_usage[] = {
		N_("git submodule status [--quiet] [--cached] [--recursive] [<path>...]"),
		ਨਿਰਾ
	};
	ਪੂਰਨ ret = 1;

	argc = parse_options(argc, argv, ਅਗੇਤਰ, module_ਸਥਿਤੀ_options,
			     git_submodule_helper_usage, 0);

	ਜੇ (module_list_compute(argv, ਅਗੇਤਰ, &pathspec, &list) < 0)
		ਜਾਓ cleanup;

	info.ਅਗੇਤਰ = ਅਗੇਤਰ;
	ਜੇ (quiet)
		info.flags |= OPT_QUIET;

	for_each_listed_submodule(&list, ਸਥਿਤੀ_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	ਵਾਪਸ ret;
}

ਬਣਤਰ module_cb {
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ mod_src;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ mod_dst;
	ਬਣਤਰ object_id oid_src;
	ਬਣਤਰ object_id oid_dst;
	ਅੱਖਰ ਸਥਿਤੀ;
	ਅੱਖਰ *sm_ਰਸਤਾ;
};
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MODULE_CB_INIT { 0 }

ਸਥਿਰ ਖਾਲੀ module_cb_release(ਬਣਤਰ module_cb *mcb)
{
	ਮੁਕਤ(mcb->sm_ਰਸਤਾ);
}

ਬਣਤਰ module_cb_list {
	ਬਣਤਰ module_cb **entries;
	ਪੂਰਨ alloc, nr;
};
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MODULE_CB_LIST_INIT { 0 }

ਸਥਿਰ ਖਾਲੀ module_cb_list_release(ਬਣਤਰ module_cb_list *mcbl)
{
	ਪੂਰਨ i;

	ਲਈ (i = 0; i < mcbl->nr; i++) {
		ਬਣਤਰ module_cb *mcb = mcbl->entries[i];

		module_cb_release(mcb);
		ਮੁਕਤ(mcb);
	}
	ਮੁਕਤ(mcbl->entries);
}

ਬਣਤਰ summary_cb {
	ਪੂਰਨ argc;
	ਅਚਲ ਅੱਖਰ **argv;
	ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ;
	ਅਚਲ ਅੱਖਰ *super_ਅਗੇਤਰ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cached: 1;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ for_ਸਥਿਤੀ: 1;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ files: 1;
	ਪੂਰਨ summary_ਹੱਦਬੰਦੀ;
};
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SUMMARY_CB_INIT { 0 }

ਗਿਣਤੀ diff_cmd {
	DIFF_ਸੂਚਕਾਂਕ,
	DIFF_FILES
};

ਸਥਿਰ ਅੱਖਰ *verify_submodule_committish(ਅਚਲ ਅੱਖਰ *sm_ਰਸਤਾ,
					 ਅਚਲ ਅੱਖਰ *committish)
{
	ਬਣਤਰ ਬੱਚਾ_process cp_rev_parse = ਬੱਚਾ_PROCESS_INIT;
	ਬਣਤਰ strbuf ਨਤੀਜਾ = STRBUF_INIT;

	cp_rev_parse.git_cmd = 1;
	cp_rev_parse.dir = sm_ਰਸਤਾ;
	prepare_submodule_repo_env(&cp_rev_parse.env);
	strvec_pushl(&cp_rev_parse.ਦਲੀਲਾਂ, "rev-parse", "-q", "--short", ਨਿਰਾ);
	strvec_pushf(&cp_rev_parse.ਦਲੀਲਾਂ, "%s^0", committish);
	strvec_ਧੱਕੋ(&cp_rev_parse.ਦਲੀਲਾਂ, "--");

	ਜੇ (capture_command(&cp_rev_parse, &ਨਤੀਜਾ, 0))
		ਵਾਪਸ ਨਿਰਾ;

	strbuf_trim_trailing_newline(&ਨਤੀਜਾ);
	ਵਾਪਸ strbuf_detach(&ਨਤੀਜਾ, ਨਿਰਾ);
}

ਸਥਿਰ ਖਾਲੀ print_submodule_summary(ਬਣਤਰ summary_cb *info, ਅਚਲ ਅੱਖਰ *errmsg,
				    ਪੂਰਨ ਕੁੱਲ_commits, ਅਚਲ ਅੱਖਰ *displaypath,
				    ਅਚਲ ਅੱਖਰ *src_abbrev, ਅਚਲ ਅੱਖਰ *dst_abbrev,
				    ਬਣਤਰ module_cb *p)
{
	ਜੇ (p->ਸਥਿਤੀ == 'T') {
		ਜੇ (S_ISGITLINK(p->mod_dst))
			ਲਿਖੋ(_("* %s %s(blob)->%s(submodule)"),
				 displaypath, src_abbrev, dst_abbrev);
		ਨਹੀਂਤਾਂ
			ਲਿਖੋ(_("* %s %s(submodule)->%s(blob)"),
				 displaypath, src_abbrev, dst_abbrev);
	} ਨਹੀਂਤਾਂ {
		ਲਿਖੋ("* %s %s...%s",
			displaypath, src_abbrev, dst_abbrev);
	}

	ਜੇ (ਕੁੱਲ_commits < 0)
		ਲਿਖੋ(":\n");
	ਨਹੀਂਤਾਂ
		ਲਿਖੋ(" (%d):\n", ਕੁੱਲ_commits);

	ਜੇ (errmsg) {
		ਲਿਖੋ(_("%s"), errmsg);
	} ਨਹੀਂਤਾਂ ਜੇ (ਕੁੱਲ_commits > 0) {
		ਬਣਤਰ ਬੱਚਾ_process cp_log = ਬੱਚਾ_PROCESS_INIT;

		cp_log.git_cmd = 1;
		cp_log.dir = p->sm_ਰਸਤਾ;
		prepare_submodule_repo_env(&cp_log.env);
		strvec_pushl(&cp_log.ਦਲੀਲਾਂ, "log", ਨਿਰਾ);

		ਜੇ (S_ISGITLINK(p->mod_src) && S_ISGITLINK(p->mod_dst)) {
			ਜੇ (info->summary_ਹੱਦਬੰਦੀ > 0)
				strvec_pushf(&cp_log.ਦਲੀਲਾਂ, "-%d",
					     info->summary_ਹੱਦਬੰਦੀ);

			strvec_pushl(&cp_log.ਦਲੀਲਾਂ, "--pretty=  %m %s",
				     "--first-parent", ਨਿਰਾ);
			strvec_pushf(&cp_log.ਦਲੀਲਾਂ, "%s...%s",
				     src_abbrev, dst_abbrev);
		} ਨਹੀਂਤਾਂ ਜੇ (S_ISGITLINK(p->mod_dst)) {
			strvec_pushl(&cp_log.ਦਲੀਲਾਂ, "--pretty=  > %s",
				     "-1", dst_abbrev, ਨਿਰਾ);
		} ਨਹੀਂਤਾਂ {
			strvec_pushl(&cp_log.ਦਲੀਲਾਂ, "--pretty=  < %s",
				     "-1", src_abbrev, ਨਿਰਾ);
		}
		ਲੜੀਵਾਰ_command(&cp_log);
	}
	ਲਿਖੋ("\n");
}

ਸਥਿਰ ਖਾਲੀ generate_submodule_summary(ਬਣਤਰ summary_cb *info,
				       ਬਣਤਰ module_cb *p)
{
	ਅੱਖਰ *displaypath, *src_abbrev = ਨਿਰਾ, *dst_abbrev;
	ਪੂਰਨ missing_src = 0, missing_dst = 0;
	ਬਣਤਰ strbuf errmsg = STRBUF_INIT;
	ਪੂਰਨ ਕੁੱਲ_commits = -1;

	ਜੇ (!info->cached && oideq(&p->oid_dst, null_oid(the_hash_algo))) {
		ਜੇ (S_ISGITLINK(p->mod_dst)) {
			ਬਣਤਰ ref_store *refs = repo_get_submodule_ref_store(the_repository,
									      p->sm_ਰਸਤਾ);

			ਜੇ (refs)
				refs_ਸਿਰ_ref(refs, handle_submodule_ਸਿਰ_ref, &p->oid_dst);
		} ਨਹੀਂਤਾਂ ਜੇ (S_ISLNK(p->mod_dst) || S_ISREG(p->mod_dst)) {
			ਬਣਤਰ stat st;
			ਪੂਰਨ fd = open(p->sm_ਰਸਤਾ, O_RDONLY);

			ਜੇ (fd < 0 || fstat(fd, &st) < 0 ||
			    ਸੂਚਕਾਂਕ_fd(the_repository->ਸੂਚਕਾਂਕ, &p->oid_dst, fd, &st, OBJ_BLOB,
				     p->sm_ਰਸਤਾ, 0))
				error(_("couldn't hash object from '%s'"), p->sm_ਰਸਤਾ);
		} ਨਹੀਂਤਾਂ {
			/* for a submodule removal (mode:0000000), don't warn */
			ਜੇ (p->mod_dst)
				warning(_("unexpected mode %o"), p->mod_dst);
		}
	}

	ਜੇ (S_ISGITLINK(p->mod_src)) {
		ਜੇ (p->ਸਥਿਤੀ != 'D')
			src_abbrev = verify_submodule_committish(p->sm_ਰਸਤਾ,
								 oid_ਮੰਜ਼ਿਲ_hex(&p->oid_src));
		ਜੇ (!src_abbrev) {
			missing_src = 1;
			/*
			 * As `rev-parse` failed, we fallback to getting
			 * the abbreviated hash using oid_src. We do
			 * this as we might still need the abbreviated
			 * hash in cases like a submodule type change, etc.
			 */
			src_abbrev = xstrndup(oid_ਮੰਜ਼ਿਲ_hex(&p->oid_src), 7);
		}
	} ਨਹੀਂਤਾਂ {
		/*
		 * The source does not point to a submodule.
		 * So, we fallback to getting the abbreviation using
		 * oid_src as we might still need the abbreviated
		 * hash in cases like submodule add, etc.
		 */
		src_abbrev = xstrndup(oid_ਮੰਜ਼ਿਲ_hex(&p->oid_src), 7);
	}

	ਜੇ (S_ISGITLINK(p->mod_dst)) {
		dst_abbrev = verify_submodule_committish(p->sm_ਰਸਤਾ,
							 oid_ਮੰਜ਼ਿਲ_hex(&p->oid_dst));
		ਜੇ (!dst_abbrev) {
			missing_dst = 1;
			/*
			 * As `rev-parse` failed, we fallback to getting
			 * the abbreviated hash using oid_dst. We do
			 * this as we might still need the abbreviated
			 * hash in cases like a submodule type change, etc.
			 */
			dst_abbrev = xstrndup(oid_ਮੰਜ਼ਿਲ_hex(&p->oid_dst), 7);
		}
	} ਨਹੀਂਤਾਂ {
		/*
		 * The destination does not point to a submodule.
		 * So, we fallback to getting the abbreviation using
		 * oid_dst as we might still need the abbreviated
		 * hash in cases like a submodule removal, etc.
		 */
		dst_abbrev = xstrndup(oid_ਮੰਜ਼ਿਲ_hex(&p->oid_dst), 7);
	}

	displaypath = get_submodule_displaypath(p->sm_ਰਸਤਾ, info->ਅਗੇਤਰ,
						info->super_ਅਗੇਤਰ);

	ਜੇ (!missing_src && !missing_dst) {
		ਬਣਤਰ ਬੱਚਾ_process cp_rev_list = ਬੱਚਾ_PROCESS_INIT;
		ਬਣਤਰ strbuf sb_rev_list = STRBUF_INIT;

		strvec_pushl(&cp_rev_list.ਦਲੀਲਾਂ, "rev-list",
			     "--first-parent", "--count", ਨਿਰਾ);
		ਜੇ (S_ISGITLINK(p->mod_src) && S_ISGITLINK(p->mod_dst))
			strvec_pushf(&cp_rev_list.ਦਲੀਲਾਂ, "%s...%s",
				     src_abbrev, dst_abbrev);
		ਨਹੀਂਤਾਂ
			strvec_ਧੱਕੋ(&cp_rev_list.ਦਲੀਲਾਂ, S_ISGITLINK(p->mod_src) ?
				    src_abbrev : dst_abbrev);
		strvec_ਧੱਕੋ(&cp_rev_list.ਦਲੀਲਾਂ, "--");

		cp_rev_list.git_cmd = 1;
		cp_rev_list.dir = p->sm_ਰਸਤਾ;
		prepare_submodule_repo_env(&cp_rev_list.env);

		ਜੇ (!capture_command(&cp_rev_list, &sb_rev_list, 0))
			ਕੁੱਲ_commits = atoi(sb_rev_list.buf);

		strbuf_release(&sb_rev_list);
	} ਨਹੀਂਤਾਂ {
		/*
		 * Don't give error msg for modification whose dst is not
		 * submodule, i.e., deleted or changed to blob
		 */
		ਜੇ (S_ISGITLINK(p->mod_dst)) {
			ਜੇ (missing_src && missing_dst) {
				strbuf_addf(&errmsg, "  Warn: %s doesn't contain commits %s and %s\n",
					    displaypath, oid_ਮੰਜ਼ਿਲ_hex(&p->oid_src),
					    oid_ਮੰਜ਼ਿਲ_hex(&p->oid_dst));
			} ਨਹੀਂਤਾਂ {
				strbuf_addf(&errmsg, "  Warn: %s doesn't contain commit %s\n",
					    displaypath, missing_src ?
					    oid_ਮੰਜ਼ਿਲ_hex(&p->oid_src) :
					    oid_ਮੰਜ਼ਿਲ_hex(&p->oid_dst));
			}
		}
	}

	print_submodule_summary(info, errmsg.len ? errmsg.buf : ਨਿਰਾ,
				ਕੁੱਲ_commits, displaypath, src_abbrev,
				dst_abbrev, p);

	ਮੁਕਤ(displaypath);
	ਮੁਕਤ(src_abbrev);
	ਮੁਕਤ(dst_abbrev);
	strbuf_release(&errmsg);
}

ਸਥਿਰ ਖਾਲੀ prepare_submodule_summary(ਬਣਤਰ summary_cb *info,
				      ਬਣਤਰ module_cb_list *list)
{
	ਪੂਰਨ i;
	ਲਈ (i = 0; i < list->nr; i++) {
		ਅਚਲ ਬਣਤਰ submodule *sub;
		ਬਣਤਰ module_cb *p = list->entries[i];
		ਬਣਤਰ strbuf sm_gitdir = STRBUF_INIT;

		ਜੇ (p->ਸਥਿਤੀ == 'D' || p->ਸਥਿਤੀ == 'T') {
			generate_submodule_summary(info, p);
			ਜਾਰੀ;
		}

		ਜੇ (info->for_ਸਥਿਤੀ && p->ਸਥਿਤੀ != 'A' &&
		    (sub = submodule_from_ਰਸਤਾ(the_repository,
					       null_oid(the_hash_algo), p->sm_ਰਸਤਾ))) {
			ਅੱਖਰ *config_ਕੁੰਜੀ = ਨਿਰਾ;
			ਅਚਲ ਅੱਖਰ *ਮੁੱਲ;
			ਪੂਰਨ ignore_all = 0;

			config_ਕੁੰਜੀ = xstrfmt("submodule.%s.ignore",
					     sub->name);
			ਜੇ (!repo_config_get_string_tmp(the_repository, config_ਕੁੰਜੀ, &ਮੁੱਲ))
				ignore_all = !strcmp(ਮੁੱਲ, "all");
			ਨਹੀਂਤਾਂ ਜੇ (sub->ignore)
				ignore_all = !strcmp(sub->ignore, "all");

			ਮੁਕਤ(config_ਕੁੰਜੀ);
			ਜੇ (ignore_all)
				ਜਾਰੀ;
		}

		/* Also show added or modified modules which are checked out */
		strbuf_addstr(&sm_gitdir, p->sm_ਰਸਤਾ);
		ਜੇ (is_nonbare_repository_dir(&sm_gitdir))
			generate_submodule_summary(info, p);
		strbuf_release(&sm_gitdir);
	}
}

ਸਥਿਰ ਖਾਲੀ submodule_summary_callback(ਬਣਤਰ diff_ਕਤਾਰ_struct *q,
				       ਬਣਤਰ diff_options *options ਅਣਵਰਤਿਆ,
				       ਖਾਲੀ *ਅੰਕੜੇ)
{
	ਪੂਰਨ i;
	ਬਣਤਰ module_cb_list *list = ਅੰਕੜੇ;
	ਲਈ (i = 0; i < q->nr; i++) {
		ਬਣਤਰ diff_filepair *p = q->ਕਤਾਰ[i];
		ਬਣਤਰ module_cb *ਆਰਜ਼ੀ;

		ਜੇ (!S_ISGITLINK(p->one->ਬਹੁਲਕ) && !S_ISGITLINK(p->two->ਬਹੁਲਕ))
			ਜਾਰੀ;
		ਆਰਜ਼ੀ = xmalloc(ਆਕਾਰ(*ਆਰਜ਼ੀ));
		ਆਰਜ਼ੀ->mod_src = p->one->ਬਹੁਲਕ;
		ਆਰਜ਼ੀ->mod_dst = p->two->ਬਹੁਲਕ;
		ਆਰਜ਼ੀ->oid_src = p->one->oid;
		ਆਰਜ਼ੀ->oid_dst = p->two->oid;
		ਆਰਜ਼ੀ->ਸਥਿਤੀ = p->ਸਥਿਤੀ;
		ਆਰਜ਼ੀ->sm_ਰਸਤਾ = xstrdup(p->one->ਰਸਤਾ);

		ALLOC_GROW(list->entries, list->nr + 1, list->alloc);
		list->entries[list->nr++] = ਆਰਜ਼ੀ;
	}
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *get_diff_cmd(ਗਿਣਤੀ diff_cmd diff_cmd)
{
	ਚੋਣ (diff_cmd) {
	ਹਾਲਤ DIFF_ਸੂਚਕਾਂਕ: ਵਾਪਸ "diff-index";
	ਹਾਲਤ DIFF_FILES: ਵਾਪਸ "diff-files";
	ਮੂਲ: BUG("bad diff_cmd value %d", diff_cmd);
	}
}

ਸਥਿਰ ਪੂਰਨ compute_summary_module_list(ਬਣਤਰ object_id *ਸਿਰ_oid,
				       ਬਣਤਰ summary_cb *info,
				       ਗਿਣਤੀ diff_cmd diff_cmd)
{
	ਬਣਤਰ strvec diff_ਦਲੀਲਾਂ = STRVEC_INIT;
	ਬਣਤਰ rev_info rev;
	ਬਣਤਰ module_cb_list list = MODULE_CB_LIST_INIT;
	ਪੂਰਨ ret = 0;

	strvec_ਧੱਕੋ(&diff_ਦਲੀਲਾਂ, get_diff_cmd(diff_cmd));
	ਜੇ (info->cached)
		strvec_ਧੱਕੋ(&diff_ਦਲੀਲਾਂ, "--cached");
	strvec_pushl(&diff_ਦਲੀਲਾਂ, "--ignore-submodules=dirty", "--raw", ਨਿਰਾ);
	ਜੇ (ਸਿਰ_oid)
		strvec_ਧੱਕੋ(&diff_ਦਲੀਲਾਂ, oid_ਮੰਜ਼ਿਲ_hex(ਸਿਰ_oid));
	strvec_ਧੱਕੋ(&diff_ਦਲੀਲਾਂ, "--");
	ਜੇ (info->argc)
		strvec_pushv(&diff_ਦਲੀਲਾਂ, info->argv);

	repo_config(the_repository, git_diff_basic_config, ਨਿਰਾ);
	repo_init_revisions(the_repository, &rev, info->ਅਗੇਤਰ);
	rev.abbrev = 0;
	precompose_argv_ਅਗੇਤਰ(diff_ਦਲੀਲਾਂ.nr, diff_ਦਲੀਲਾਂ.v, ਨਿਰਾ);
	ਤਿਆਰੀ_revisions_from_strvec(&diff_ਦਲੀਲਾਂ, &rev, ਨਿਰਾ);
	rev.diffopt.output_format = DIFF_FORMAT_NO_OUTPUT | DIFF_FORMAT_CALLBACK;
	rev.diffopt.format_callback = submodule_summary_callback;
	rev.diffopt.format_callback_ਅੰਕੜੇ = &list;

	ਜੇ (!info->cached) {
		ਜੇ (diff_cmd == DIFF_ਸੂਚਕਾਂਕ)
			ਤਿਆਰੀ_work_ਰੁੱਖ(the_repository);
		ਜੇ (repo_read_ਸੂਚਕਾਂਕ_preload(the_repository, &rev.diffopt.pathspec, 0) < 0) {
			perror("repo_read_index_preload");
			ret = -1;
			ਜਾਓ cleanup;
		}
	} ਨਹੀਂਤਾਂ ਜੇ (repo_read_ਸੂਚਕਾਂਕ(the_repository) < 0) {
		perror("repo_read_cache");
		ret = -1;
		ਜਾਓ cleanup;
	}

	ਜੇ (diff_cmd == DIFF_ਸੂਚਕਾਂਕ)
		ਲੜੀਵਾਰ_diff_ਸੂਚਕਾਂਕ(&rev, info->cached ? DIFF_ਸੂਚਕਾਂਕ_CACHED : 0);
	ਨਹੀਂਤਾਂ
		ਲੜੀਵਾਰ_diff_files(&rev, 0);
	prepare_submodule_summary(info, &list);
cleanup:
	strvec_clear(&diff_ਦਲੀਲਾਂ);
	release_revisions(&rev);
	module_cb_list_release(&list);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ module_summary(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			  ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ summary_cb info = SUMMARY_CB_INIT;
	ਪੂਰਨ cached = 0;
	ਪੂਰਨ for_ਸਥਿਤੀ = 0;
	ਪੂਰਨ files = 0;
	ਪੂਰਨ summary_ਹੱਦਬੰਦੀ = -1;
	ਗਿਣਤੀ diff_cmd diff_cmd = DIFF_ਸੂਚਕਾਂਕ;
	ਬਣਤਰ object_id ਸਿਰ_oid;
	ਪੂਰਨ ret;
	ਬਣਤਰ option module_summary_options[] = {
		OPT_BOOL(0, "cached", &cached,
			 N_("use the commit stored in the index instead of the submodule HEAD")),
		OPT_BOOL(0, "files", &files,
			 N_("compare the commit in the index with that in the submodule HEAD")),
		OPT_BOOL(0, "for-status", &for_ਸਥਿਤੀ,
			 N_("skip submodules with 'ignore_config' value set to 'all'")),
		OPT_INTEGER('n', "summary-limit", &summary_ਹੱਦਬੰਦੀ,
			     N_("limit the summary size")),
		OPT_ਸਿਰਾ()
	};
	ਅਚਲ ਅੱਖਰ *ਅਚਲ git_submodule_helper_usage[] = {
		N_("git submodule summary [<options>] [<commit>] [--] [<path>]"),
		ਨਿਰਾ
	};

	argc = parse_options(argc, argv, ਅਗੇਤਰ, module_summary_options,
			     git_submodule_helper_usage, 0);

	ਜੇ (!summary_ਹੱਦਬੰਦੀ)
		ਵਾਪਸ 0;

	ਜੇ (!repo_get_oid(the_repository, argc ? argv[0] : "HEAD", &ਸਿਰ_oid)) {
		ਜੇ (argc) {
			argv++;
			argc--;
		}
	} ਨਹੀਂਤਾਂ ਜੇ (!argc || !strcmp(argv[0], "HEAD")) {
		/* before the first commit: compare with an empty tree */
		oidcpy(&ਸਿਰ_oid, the_hash_algo->ਖਾਲੀ_ਥਾਵਾਂ_ਰੁੱਖ);
		ਜੇ (argc) {
			argv++;
			argc--;
		}
	} ਨਹੀਂਤਾਂ {
		ਜੇ (repo_get_oid(the_repository, "HEAD", &ਸਿਰ_oid))
			die(_("could not fetch a revision for HEAD"));
	}

	ਜੇ (files) {
		ਜੇ (cached)
			die(_("options '%s' and '%s' cannot be used together"), "--cached", "--files");
		diff_cmd = DIFF_FILES;
	}

	info.argc = argc;
	info.argv = argv;
	info.ਅਗੇਤਰ = ਅਗੇਤਰ;
	info.cached = !!cached;
	info.files = !!files;
	info.for_ਸਥਿਤੀ = !!for_ਸਥਿਤੀ;
	info.summary_ਹੱਦਬੰਦੀ = summary_ਹੱਦਬੰਦੀ;

	ret = compute_summary_module_list((diff_cmd == DIFF_ਸੂਚਕਾਂਕ) ? &ਸਿਰ_oid : ਨਿਰਾ,
					  &info, diff_cmd);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ module_gitdir(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ ਅਣਵਰਤਿਆ,
			 ਬਣਤਰ repository *repo)
{
	ਬਣਤਰ strbuf gitdir = STRBUF_INIT;

	ਜੇ (argc != 2)
		usage(_("git submodule--helper gitdir <name>"));

	submodule_name_ਮੰਜ਼ਿਲ_gitdir(&gitdir, repo, argv[1]);

	ਲਿਖੋ("%s\n", gitdir.buf);

	strbuf_release(&gitdir);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ module_migrate(ਪੂਰਨ argc ਅਣਵਰਤਿਆ, ਅਚਲ ਅੱਖਰ **argv ਅਣਵਰਤਿਆ,
			  ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ ਅਣਵਰਤਿਆ, ਬਣਤਰ repository *repo)
{
	ਬਣਤਰ strbuf module_dir = STRBUF_INIT;
	DIR *dir;
	ਬਣਤਰ dirent *de;
	ਪੂਰਨ repo_ਸੰਸਕਰਣ = 0;

	repo_git_ਰਸਤਾ_append(repo, &module_dir, "modules/");

	dir = opendir(module_dir.buf);
	ਜੇ (!dir)
		die(_("could not open '%s'"), module_dir.buf);

	ਜਦੋਂ ((de = readdir(dir))) {
		ਬਣਤਰ strbuf gitdir_ਰਸਤਾ = STRBUF_INIT;
		ਅੱਖਰ *ਕੁੰਜੀ;
		ਅਚਲ ਅੱਖਰ *ਮੁੱਲ;

		ਜੇ (is_dot_or_dotdot(de->d_name))
			ਜਾਰੀ;

		strbuf_addf(&gitdir_ਰਸਤਾ, "%s/%s", module_dir.buf, de->d_name);
		ਜੇ (!is_git_directory(gitdir_ਰਸਤਾ.buf)) {
			strbuf_release(&gitdir_ਰਸਤਾ);
			ਜਾਰੀ;
		}
		strbuf_release(&gitdir_ਰਸਤਾ);

		ਕੁੰਜੀ = xstrfmt("submodule.%s.gitdir", de->d_name);
		ਜੇ (!repo_config_get_string_tmp(repo, ਕੁੰਜੀ, &ਮੁੱਲ)) {
			/* Already has a gitdir config, nothing to do. */
			ਮੁਕਤ(ਕੁੰਜੀ);
			ਜਾਰੀ;
		}
		ਮੁਕਤ(ਕੁੰਜੀ);

		create_default_gitdir_config(de->d_name);
	}

	closedir(dir);
	strbuf_release(&module_dir);

	repo_config_get_int(the_repository, "core.repositoryformatversion", &repo_ਸੰਸਕਰਣ);
	ਜੇ (repo_ਸੰਸਕਰਣ == 0 &&
	    repo_config_ਸੈੱਟ_gently(repo, "core.repositoryformatversion", "1"))
		die(_("could not set core.repositoryformatversion to 1.\n"
		      "Please set it for migration to work, for example:\n"
		      "git config core.repositoryformatversion 1"));

	ਜੇ (repo_config_ਸੈੱਟ_gently(repo, "extensions.submodulePathConfig", "true"))
		die(_("could not enable submodulePathConfig extension. It is required\n"
		      "for migration to work. Please enable it in the root repo:\n"
		      "git config extensions.submodulePathConfig true"));

	repo->repository_format_submodule_ਰਸਤਾ_cfg = 1;

	ਵਾਪਸ 0;
}

ਬਣਤਰ sync_cb {
	ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ;
	ਅਚਲ ਅੱਖਰ *super_ਅਗੇਤਰ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags;
};
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYNC_CB_INIT { 0 }

ਸਥਿਰ ਖਾਲੀ sync_submodule(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			   ਅਚਲ ਅੱਖਰ *super_ਅਗੇਤਰ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags)
{
	ਅਚਲ ਬਣਤਰ submodule *sub;
	ਅੱਖਰ *remote_ਕੁੰਜੀ = ਨਿਰਾ;
	ਅੱਖਰ *sub_origin_url, *super_config_url, *displaypath, *default_remote;
	ਬਣਤਰ strbuf sb = STRBUF_INIT;
	ਅੱਖਰ *sub_config_ਰਸਤਾ = ਨਿਰਾ;
	ਪੂਰਨ ਕੋਡ;

	ਜੇ (!is_submodule_ਸਰਗਰਮ(the_repository, ਰਸਤਾ))
		ਵਾਪਸ;

	ਜੇ (validate_submodule_ਰਸਤਾ(ਰਸਤਾ) < 0)
		die(ਨਿਰਾ);

	sub = submodule_from_ਰਸਤਾ(the_repository, null_oid(the_hash_algo), ਰਸਤਾ);

	ਜੇ (sub && sub->url) {
		ਜੇ (starts_with_dot_dot_slash(sub->url) ||
		    starts_with_dot_slash(sub->url)) {
			ਅੱਖਰ *ਉੱਪਰ_ਰਸਤਾ = get_ਉੱਪਰ_ਰਸਤਾ(ਰਸਤਾ);

			sub_origin_url = resolve_relative_url(sub->url, ਉੱਪਰ_ਰਸਤਾ, 1);
			super_config_url = resolve_relative_url(sub->url, ਨਿਰਾ, 1);
			ਮੁਕਤ(ਉੱਪਰ_ਰਸਤਾ);
		} ਨਹੀਂਤਾਂ {
			sub_origin_url = xstrdup(sub->url);
			super_config_url = xstrdup(sub->url);
		}
	} ਨਹੀਂਤਾਂ {
		sub_origin_url = xstrdup("");
		super_config_url = xstrdup("");
	}

	displaypath = get_submodule_displaypath(ਰਸਤਾ, ਅਗੇਤਰ, super_ਅਗੇਤਰ);

	ਜੇ (!(flags & OPT_QUIET))
		ਲਿਖੋ(_("Synchronizing submodule url for '%s'\n"),
			 displaypath);

	strbuf_reset(&sb);
	strbuf_addf(&sb, "submodule.%s.url", sub->name);
	ਜੇ (repo_config_ਸੈੱਟ_gently(the_repository, sb.buf, super_config_url))
		die(_("failed to register url for submodule path '%s'"),
		      displaypath);

	ਜੇ (!is_submodule_populated_gently(ਰਸਤਾ, ਨਿਰਾ))
		ਜਾਓ cleanup;

	strbuf_reset(&sb);
	ਕੋਡ = get_default_remote_submodule(ਰਸਤਾ, &default_remote);
	ਜੇ (ਕੋਡ)
		ਬਾਹਰ_ਨਿਕਲੋ(ਕੋਡ);

	remote_ਕੁੰਜੀ = xstrfmt("remote.%s.url", default_remote);
	ਮੁਕਤ(default_remote);

	submodule_ਮੰਜ਼ਿਲ_gitdir(the_repository, &sb, ਰਸਤਾ);
	strbuf_addstr(&sb, "/config");

	ਜੇ (repo_config_ਸੈੱਟ_ਵਿੱਚ_file_gently(the_repository, sb.buf, remote_ਕੁੰਜੀ, ਨਿਰਾ, sub_origin_url))
		die(_("failed to update remote for submodule '%s'"),
		      ਰਸਤਾ);

	ਜੇ (flags & OPT_RECURSIVE) {
		ਬਣਤਰ ਬੱਚਾ_process cpr = ਬੱਚਾ_PROCESS_INIT;

		cpr.git_cmd = 1;
		cpr.dir = ਰਸਤਾ;
		prepare_submodule_repo_env(&cpr.env);

		strvec_pushl(&cpr.ਦਲੀਲਾਂ, "submodule--helper", "sync",
			     "--recursive", ਨਿਰਾ);
		strvec_pushf(&cpr.ਦਲੀਲਾਂ, "--super-prefix=%s/", displaypath);

		ਜੇ (flags & OPT_QUIET)
			strvec_ਧੱਕੋ(&cpr.ਦਲੀਲਾਂ, "--quiet");

		ਜੇ (ਲੜੀਵਾਰ_command(&cpr))
			die(_("failed to recurse into submodule '%s'"),
			      ਰਸਤਾ);
	}

cleanup:
	ਮੁਕਤ(super_config_url);
	ਮੁਕਤ(sub_origin_url);
	strbuf_release(&sb);
	ਮੁਕਤ(remote_ਕੁੰਜੀ);
	ਮੁਕਤ(displaypath);
	ਮੁਕਤ(sub_config_ਰਸਤਾ);
}

ਸਥਿਰ ਖਾਲੀ sync_submodule_cb(ਅਚਲ ਬਣਤਰ cache_ਇੰਦਰਾਜ *list_ਵਸਤੂ, ਖਾਲੀ *cb_ਅੰਕੜੇ)
{
	ਬਣਤਰ sync_cb *info = cb_ਅੰਕੜੇ;

	sync_submodule(list_ਵਸਤੂ->name, info->ਅਗੇਤਰ, info->super_ਅਗੇਤਰ,
		       info->flags);
}

ਸਥਿਰ ਪੂਰਨ module_sync(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		       ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ sync_cb info = SYNC_CB_INIT;
	ਬਣਤਰ pathspec pathspec = { 0 };
	ਬਣਤਰ module_list list = MODULE_LIST_INIT;
	ਪੂਰਨ quiet = 0;
	ਪੂਰਨ recursive = 0;
	ਬਣਤਰ option module_sync_options[] = {
		OPT_SUPER_ਅਗੇਤਰ(&info.super_ਅਗੇਤਰ),
		OPT__QUIET(&quiet, N_("suppress output of synchronizing submodule url")),
		OPT_BOOL(0, "recursive", &recursive,
			N_("recurse into nested submodules")),
		OPT_ਸਿਰਾ()
	};
	ਅਚਲ ਅੱਖਰ *ਅਚਲ git_submodule_helper_usage[] = {
		N_("git submodule sync [--quiet] [--recursive] [<path>]"),
		ਨਿਰਾ
	};
	ਪੂਰਨ ret = 1;

	argc = parse_options(argc, argv, ਅਗੇਤਰ, module_sync_options,
			     git_submodule_helper_usage, 0);

	ਜੇ (module_list_compute(argv, ਅਗੇਤਰ, &pathspec, &list) < 0)
		ਜਾਓ cleanup;

	info.ਅਗੇਤਰ = ਅਗੇਤਰ;
	ਜੇ (quiet)
		info.flags |= OPT_QUIET;
	ਜੇ (recursive)
		info.flags |= OPT_RECURSIVE;

	for_each_listed_submodule(&list, sync_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	ਵਾਪਸ ret;
}

ਬਣਤਰ deinit_cb {
	ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags;
};
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEINIT_CB_INIT { 0 }

ਸਥਿਰ ਖਾਲੀ deinit_submodule(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags)
{
	ਅਚਲ ਬਣਤਰ submodule *sub;
	ਅੱਖਰ *displaypath = ਨਿਰਾ;
	ਬਣਤਰ ਬੱਚਾ_process cp_config = ਬੱਚਾ_PROCESS_INIT;
	ਬਣਤਰ strbuf sb_config = STRBUF_INIT;
	ਅੱਖਰ *sub_git_dir = xstrfmt("%s/.git", ਰਸਤਾ);

	ਜੇ (validate_submodule_ਰਸਤਾ(ਰਸਤਾ) < 0)
		die(ਨਿਰਾ);

	sub = submodule_from_ਰਸਤਾ(the_repository, null_oid(the_hash_algo), ਰਸਤਾ);

	ਜੇ (!sub || !sub->name)
		ਜਾਓ cleanup;

	displaypath = get_submodule_displaypath(ਰਸਤਾ, ਅਗੇਤਰ, ਨਿਰਾ);

	/* remove the submodule work tree (unless the user already did it) */
	ਜੇ (is_directory(ਰਸਤਾ)) {
		ਬਣਤਰ strbuf sb_rm = STRBUF_INIT;
		ਅਚਲ ਅੱਖਰ *format;

		ਜੇ (is_directory(sub_git_dir)) {
			ਜੇ (!(flags & OPT_QUIET))
				warning(_("Submodule work tree '%s' contains a .git "
					  "directory. This will be replaced with a "
					  ".git file by using absorbgitdirs."),
					displaypath);

			absorb_git_dir_into_superproject(ਰਸਤਾ, ਨਿਰਾ);

		}

		ਜੇ (!(flags & OPT_FORCE)) {
			ਬਣਤਰ ਬੱਚਾ_process cp_rm = ਬੱਚਾ_PROCESS_INIT;

			cp_rm.git_cmd = 1;
			strvec_pushl(&cp_rm.ਦਲੀਲਾਂ, "rm", "-qn",
				     ਰਸਤਾ, ਨਿਰਾ);

			ਜੇ (ਲੜੀਵਾਰ_command(&cp_rm))
				die(_("Submodule work tree '%s' contains local "
				      "modifications; use '-f' to discard them"),
				      displaypath);
		}

		strbuf_addstr(&sb_rm, ਰਸਤਾ);

		ਜੇ (!remove_dir_recursively(&sb_rm, 0))
			format = _("Cleared directory '%s'\n");
		ਨਹੀਂਤਾਂ
			format = _("Could not remove submodule work tree '%s'\n");

		ਜੇ (!(flags & OPT_QUIET))
			ਲਿਖੋ(format, displaypath);

		submodule_unset_core_worktree(sub);

		strbuf_release(&sb_rm);
	}

	ਜੇ (mkdir(ਰਸਤਾ, 0777))
		ਲਿਖੋ(_("could not create empty submodule directory %s"),
		      displaypath);

	cp_config.git_cmd = 1;
	strvec_pushl(&cp_config.ਦਲੀਲਾਂ, "config", "--get-regexp", ਨਿਰਾ);
	strvec_pushf(&cp_config.ਦਲੀਲਾਂ, "submodule.%s\\.", sub->name);

	/* remove the .git/config entries (unless the user already did it) */
	ਜੇ (!capture_command(&cp_config, &sb_config, 0) && sb_config.len) {
		ਅੱਖਰ *sub_ਕੁੰਜੀ = xstrfmt("submodule.%s", sub->name);

		/*
		 * remove the whole section so we have a clean state when
		 * the user later decides to init this submodule again
		 */
		repo_config_rename_section_ਵਿੱਚ_file(the_repository, ਨਿਰਾ, sub_ਕੁੰਜੀ, ਨਿਰਾ);
		ਜੇ (!(flags & OPT_QUIET))
			ਲਿਖੋ(_("Submodule '%s' (%s) unregistered for path '%s'\n"),
				 sub->name, sub->url, displaypath);
		ਮੁਕਤ(sub_ਕੁੰਜੀ);
	}

cleanup:
	ਮੁਕਤ(displaypath);
	ਮੁਕਤ(sub_git_dir);
	strbuf_release(&sb_config);
}

ਸਥਿਰ ਖਾਲੀ deinit_submodule_cb(ਅਚਲ ਬਣਤਰ cache_ਇੰਦਰਾਜ *list_ਵਸਤੂ,
				ਖਾਲੀ *cb_ਅੰਕੜੇ)
{
	ਬਣਤਰ deinit_cb *info = cb_ਅੰਕੜੇ;
	deinit_submodule(list_ਵਸਤੂ->name, info->ਅਗੇਤਰ, info->flags);
}

ਸਥਿਰ ਪੂਰਨ module_deinit(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			 ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ deinit_cb info = DEINIT_CB_INIT;
	ਬਣਤਰ pathspec pathspec = { 0 };
	ਬਣਤਰ module_list list = MODULE_LIST_INIT;
	ਪੂਰਨ quiet = 0;
	ਪੂਰਨ force = 0;
	ਪੂਰਨ all = 0;
	ਬਣਤਰ option module_deinit_options[] = {
		OPT__QUIET(&quiet, N_("suppress submodule status output")),
		OPT__FORCE(&force, N_("remove submodule working trees even if they contain local changes"), 0),
		OPT_BOOL(0, "all", &all, N_("unregister all submodules")),
		OPT_ਸਿਰਾ()
	};
	ਅਚਲ ਅੱਖਰ *ਅਚਲ git_submodule_helper_usage[] = {
		N_("git submodule deinit [--quiet] [-f | --force] [--all | [--] [<path>...]]"),
		ਨਿਰਾ
	};
	ਪੂਰਨ ret = 1;

	argc = parse_options(argc, argv, ਅਗੇਤਰ, module_deinit_options,
			     git_submodule_helper_usage, 0);

	ਜੇ (all && argc) {
		error("pathspec and --all are incompatible");
		usage_with_options(git_submodule_helper_usage,
				   module_deinit_options);
	}

	ਜੇ (!argc && !all)
		die(_("Use '--all' if you really want to deinitialize all submodules"));

	ਜੇ (module_list_compute(argv, ਅਗੇਤਰ, &pathspec, &list) < 0)
		ਜਾਓ cleanup;

	info.ਅਗੇਤਰ = ਅਗੇਤਰ;
	ਜੇ (quiet)
		info.flags |= OPT_QUIET;
	ਜੇ (force)
		info.flags |= OPT_FORCE;

	for_each_listed_submodule(&list, deinit_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	ਵਾਪਸ ret;
}

ਬਣਤਰ module_clone_ਅੰਕੜੇ {
	ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ;
	ਅਚਲ ਅੱਖਰ *ਰਸਤਾ;
	ਅਚਲ ਅੱਖਰ *name;
	ਅਚਲ ਅੱਖਰ *url;
	ਪੂਰਨ ਡੂੰਘਾਈ;
	ਬਣਤਰ list_objects_filter_options *filter_options;
	ਗਿਣਤੀ ref_storage_format ref_storage_format;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ quiet: 1;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਤਰੱਕੀ: 1;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ dissociate: 1;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ require_init: 1;
	ਪੂਰਨ single_branch;
};
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MODULE_CLONE_ਅੰਕੜੇ_INIT { \
	.single_branch = -1, \
	.ref_storage_format = REF_STORAGE_FORMAT_UNKNOWN, \
}

ਬਣਤਰ submodule_alternate_ਤਿਆਰੀ {
	ਅਚਲ ਅੱਖਰ *submodule_name;
	ਗਿਣਤੀ SUBMODULE_ALTERNATE_ERROR_ਬਹੁਲਕ {
		SUBMODULE_ALTERNATE_ERROR_DIE,
		SUBMODULE_ALTERNATE_ERROR_INFO,
		SUBMODULE_ALTERNATE_ERROR_IGNORE
	} error_ਬਹੁਲਕ;
	ਬਣਤਰ string_list *reference;
};
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SUBMODULE_ALTERNATE_ਤਿਆਰੀ_INIT { \
	.error_ਬਹੁਲਕ = SUBMODULE_ALTERNATE_ERROR_IGNORE, \
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ alternate_error_advice[] = N_(
"An alternate computed from a superproject's alternate is invalid.\n"
"To allow Git to clone without an alternate in such a case, set\n"
"submodule.alternateErrorStrategy to 'info' or, equivalently, clone with\n"
"'--reference-if-able' instead of '--reference'."
);

ਸਥਿਰ ਪੂਰਨ ਜੋੜੋ_possible_reference_from_superproject(
		ਬਣਤਰ odb_files_dir *alt_odb, ਖਾਲੀ *sas_cb)
{
	ਬਣਤਰ submodule_alternate_ਤਿਆਰੀ *sas = sas_cb;
	ਮਾਪ_t len;

	/*
	 * If the alternate object store is another repository, try the
	 * standard layout with .git/(modules/<name>)+/objects
	 */
	ਜੇ (strip_suffix(alt_odb->abspath, "/objects", &len)) {
		ਬਣਤਰ repository alternate;
		ਅੱਖਰ *sm_alternate;
		ਬਣਤਰ strbuf sb = STRBUF_INIT;
		ਬਣਤਰ strbuf err = STRBUF_INIT;
		strbuf_ਜੋੜੋ(&sb, alt_odb->abspath, len);

		ਜੇ (repo_init(&alternate, sb.buf, ਨਿਰਾ) < 0)
			die(_("could not get a repository handle for gitdir '%s'"),
			    sb.buf);

		/*
		 * We need to end the new path with '/' to mark it as a dir,
		 * otherwise a submodule name containing '/' will be broken
		 * as the last part of a missing submodule reference would
		 * be taken as a file name.
		 */
		strbuf_reset(&sb);
		submodule_name_ਮੰਜ਼ਿਲ_gitdir(&sb, &alternate, sas->submodule_name);
		strbuf_addch(&sb, '/');
		repo_clear(&alternate);

		sm_alternate = compute_alternate_ਰਸਤਾ(sb.buf, &err);
		ਜੇ (sm_alternate) {
			ਅੱਖਰ *p = strbuf_detach(&sb, ਨਿਰਾ);

			string_list_append(sas->reference, p)->util = p;
			ਮੁਕਤ(sm_alternate);
		} ਨਹੀਂਤਾਂ {
			ਚੋਣ (sas->error_ਬਹੁਲਕ) {
			ਹਾਲਤ SUBMODULE_ALTERNATE_ERROR_DIE:
				ਜੇ (advice_enabled(ADVICE_SUBMODULE_ALTERNATE_ERROR_STRATEGY_DIE))
					advise(_(alternate_error_advice));
				die(_("submodule '%s' cannot add alternate: %s"),
				    sas->submodule_name, err.buf);
			ਹਾਲਤ SUBMODULE_ALTERNATE_ERROR_INFO:
				fprintf_ln(stderr, _("submodule '%s' cannot add alternate: %s"),
					sas->submodule_name, err.buf);
			ਹਾਲਤ SUBMODULE_ALTERNATE_ERROR_IGNORE:
				; /* nothing */
			}
		}

		strbuf_release(&err);
		strbuf_release(&sb);
	}

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ prepare_possible_alternates(ਅਚਲ ਅੱਖਰ *sm_name,
		ਬਣਤਰ string_list *reference)
{
	ਅੱਖਰ *sm_alternate = ਨਿਰਾ, *error_strategy = ਨਿਰਾ;
	ਬਣਤਰ submodule_alternate_ਤਿਆਰੀ sas = SUBMODULE_ALTERNATE_ਤਿਆਰੀ_INIT;

	repo_config_get_string(the_repository, "submodule.alternateLocation", &sm_alternate);
	ਜੇ (!sm_alternate)
		ਵਾਪਸ;

	repo_config_get_string(the_repository, "submodule.alternateErrorStrategy", &error_strategy);

	ਜੇ (!error_strategy)
		error_strategy = xstrdup("die");

	sas.submodule_name = sm_name;
	sas.reference = reference;
	ਜੇ (!strcmp(error_strategy, "die"))
		sas.error_ਬਹੁਲਕ = SUBMODULE_ALTERNATE_ERROR_DIE;
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(error_strategy, "info"))
		sas.error_ਬਹੁਲਕ = SUBMODULE_ALTERNATE_ERROR_INFO;
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(error_strategy, "ignore"))
		sas.error_ਬਹੁਲਕ = SUBMODULE_ALTERNATE_ERROR_IGNORE;
	ਨਹੀਂਤਾਂ
		die(_("Value '%s' for submodule.alternateErrorStrategy is not recognized"), error_strategy);

	ਜੇ (!strcmp(sm_alternate, "superproject"))
		odb_for_each_alternate(the_repository->objects,
				       ਜੋੜੋ_possible_reference_from_superproject, &sas);
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(sm_alternate, "no"))
		; /* do nothing */
	ਨਹੀਂਤਾਂ
		die(_("Value '%s' for submodule.alternateLocation is not recognized"), sm_alternate);

	ਮੁਕਤ(sm_alternate);
	ਮੁਕਤ(error_strategy);
}

ਸਥਿਰ ਅੱਖਰ *clone_submodule_sm_gitdir(ਅਚਲ ਅੱਖਰ *name)
{
	ਬਣਤਰ strbuf sb = STRBUF_INIT;
	ਅੱਖਰ *sm_gitdir;

	submodule_name_ਮੰਜ਼ਿਲ_gitdir(&sb, the_repository, name);
	sm_gitdir = absolute_pathdup(sb.buf);
	strbuf_release(&sb);

	ਵਾਪਸ sm_gitdir;
}

ਸਥਿਰ ਪੂਰਨ dir_ਸ਼ਾਮਲ_ਹੈ_only_dotgit(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ)
{
	DIR *dir = opendir(ਰਸਤਾ);
	ਬਣਤਰ dirent *e;
	ਪੂਰਨ ret = 1;

	ਜੇ (!dir)
		ਵਾਪਸ 0;

	e = readdir_skip_dot_and_dotdot(dir);
	ਜੇ (!e)
		ret = 0;
	ਨਹੀਂਤਾਂ ਜੇ (strcmp(DEFAULT_GIT_DIR_ENVIRONMENT, e->d_name) ||
		 (e = readdir_skip_dot_and_dotdot(dir))) {
		error("unexpected item '%s' in '%s'", e->d_name, ਰਸਤਾ);
		ret = 0;
	}

	closedir(dir);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ clone_submodule(ਅਚਲ ਬਣਤਰ module_clone_ਅੰਕੜੇ *clone_ਅੰਕੜੇ,
			   ਬਣਤਰ string_list *reference)
{
	ਅੱਖਰ *p;
	ਅੱਖਰ *sm_gitdir = clone_submodule_sm_gitdir(clone_ਅੰਕੜੇ->name);
	ਅੱਖਰ *sm_alternate = ਨਿਰਾ, *error_strategy = ਨਿਰਾ;
	ਬਣਤਰ stat st;
	ਬਣਤਰ ਬੱਚਾ_process cp = ਬੱਚਾ_PROCESS_INIT;
	ਅਚਲ ਅੱਖਰ *clone_ਅੰਕੜੇ_ਰਸਤਾ = clone_ਅੰਕੜੇ->ਰਸਤਾ;
	ਅੱਖਰ *ਮੰਜ਼ਿਲ_free = ਨਿਰਾ;

	ਜੇ (validate_submodule_ਰਸਤਾ(clone_ਅੰਕੜੇ_ਰਸਤਾ) < 0)
		die(ਨਿਰਾ);

	ਜੇ (!is_absolute_ਰਸਤਾ(clone_ਅੰਕੜੇ->ਰਸਤਾ))
		clone_ਅੰਕੜੇ_ਰਸਤਾ = ਮੰਜ਼ਿਲ_free = xstrfmt("%s/%s", repo_get_work_ਰੁੱਖ(the_repository),
						    clone_ਅੰਕੜੇ->ਰਸਤਾ);

	ਜੇ (!file_exists(sm_gitdir)) {
		ਜੇ (clone_ਅੰਕੜੇ->require_init && !stat(clone_ਅੰਕੜੇ_ਰਸਤਾ, &st) &&
		    !is_ਖਾਲੀ_ਥਾਵਾਂ_dir(clone_ਅੰਕੜੇ_ਰਸਤਾ))
			die(_("directory not empty: '%s'"), clone_ਅੰਕੜੇ_ਰਸਤਾ);

		ਜੇ (ਸੁਰੱਖਿਅਤ_create_leading_directories_const(the_repository, sm_gitdir) < 0)
			die(_("could not create directory '%s'"), sm_gitdir);

		prepare_possible_alternates(clone_ਅੰਕੜੇ->name, reference);

		strvec_ਧੱਕੋ(&cp.ਦਲੀਲਾਂ, "clone");
		strvec_ਧੱਕੋ(&cp.ਦਲੀਲਾਂ, "--no-checkout");
		ਜੇ (clone_ਅੰਕੜੇ->quiet)
			strvec_ਧੱਕੋ(&cp.ਦਲੀਲਾਂ, "--quiet");
		ਜੇ (clone_ਅੰਕੜੇ->ਤਰੱਕੀ)
			strvec_ਧੱਕੋ(&cp.ਦਲੀਲਾਂ, "--progress");
		ਜੇ (clone_ਅੰਕੜੇ->ਡੂੰਘਾਈ > 0)
			strvec_pushf(&cp.ਦਲੀਲਾਂ, "--depth=%d", clone_ਅੰਕੜੇ->ਡੂੰਘਾਈ);
		ਜੇ (reference->nr) {
			ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ;

			for_each_string_list_ਵਸਤੂ(ਵਸਤੂ, reference)
				strvec_pushl(&cp.ਦਲੀਲਾਂ, "--reference",
					     ਵਸਤੂ->string, ਨਿਰਾ);
		}
		ਜੇ (clone_ਅੰਕੜੇ->ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
			strvec_pushf(&cp.ਦਲੀਲਾਂ, "--ref-storage-format=%s",
				     ref_storage_format_ਮੰਜ਼ਿਲ_name(clone_ਅੰਕੜੇ->ref_storage_format));
		ਜੇ (clone_ਅੰਕੜੇ->dissociate)
			strvec_ਧੱਕੋ(&cp.ਦਲੀਲਾਂ, "--dissociate");
		ਜੇ (sm_gitdir && *sm_gitdir)
			strvec_pushl(&cp.ਦਲੀਲਾਂ, "--separate-git-dir", sm_gitdir, ਨਿਰਾ);
		ਜੇ (clone_ਅੰਕੜੇ->filter_options && clone_ਅੰਕੜੇ->filter_options->choice)
			strvec_pushf(&cp.ਦਲੀਲਾਂ, "--filter=%s",
				     expand_list_objects_filter_spec(
					     clone_ਅੰਕੜੇ->filter_options));
		ਜੇ (clone_ਅੰਕੜੇ->single_branch >= 0)
			strvec_ਧੱਕੋ(&cp.ਦਲੀਲਾਂ, clone_ਅੰਕੜੇ->single_branch ?
				    "--single-branch" :
				    "--no-single-branch");

		strvec_ਧੱਕੋ(&cp.ਦਲੀਲਾਂ, "--");
		strvec_ਧੱਕੋ(&cp.ਦਲੀਲਾਂ, clone_ਅੰਕੜੇ->url);
		strvec_ਧੱਕੋ(&cp.ਦਲੀਲਾਂ, clone_ਅੰਕੜੇ_ਰਸਤਾ);

		cp.git_cmd = 1;
		prepare_submodule_repo_env(&cp.env);
		cp.no_stdin = 1;

		ਜੇ(ਲੜੀਵਾਰ_command(&cp))
			die(_("clone of '%s' into submodule path '%s' failed"),
			    clone_ਅੰਕੜੇ->url, clone_ਅੰਕੜੇ_ਰਸਤਾ);

		ਜੇ (clone_ਅੰਕੜੇ->require_init && !stat(clone_ਅੰਕੜੇ_ਰਸਤਾ, &st) &&
		    !dir_ਸ਼ਾਮਲ_ਹੈ_only_dotgit(clone_ਅੰਕੜੇ_ਰਸਤਾ)) {
			ਅੱਖਰ *dot_git = xstrfmt("%s/.git", clone_ਅੰਕੜੇ_ਰਸਤਾ);
			unlink(dot_git);
			ਮੁਕਤ(dot_git);
			die(_("directory not empty: '%s'"), clone_ਅੰਕੜੇ_ਰਸਤਾ);
		}
	} ਨਹੀਂਤਾਂ {
		ਅੱਖਰ *ਰਸਤਾ;

		ਜੇ (clone_ਅੰਕੜੇ->require_init && !stat(clone_ਅੰਕੜੇ_ਰਸਤਾ, &st) &&
		    !is_ਖਾਲੀ_ਥਾਵਾਂ_dir(clone_ਅੰਕੜੇ_ਰਸਤਾ))
			die(_("directory not empty: '%s'"), clone_ਅੰਕੜੇ_ਰਸਤਾ);
		ਜੇ (ਸੁਰੱਖਿਅਤ_create_leading_directories_const(the_repository, clone_ਅੰਕੜੇ_ਰਸਤਾ) < 0)
			die(_("could not create directory '%s'"), clone_ਅੰਕੜੇ_ਰਸਤਾ);
		ਰਸਤਾ = xstrfmt("%s/index", sm_gitdir);
		unlink_or_warn(ਰਸਤਾ);
		ਮੁਕਤ(ਰਸਤਾ);
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
	ਜੇ (validate_submodule_git_dir(sm_gitdir, clone_ਅੰਕੜੇ->name) < 0) {
		ਅੱਖਰ *ਸਿਰ = xstrfmt("%s/HEAD", sm_gitdir);
		unlink(ਸਿਰ);
		ਮੁਕਤ(ਸਿਰ);
		die(_("refusing to create/use '%s' in another submodule's git dir. "
		      "Enabling extensions.submodulePathConfig should fix this."),
		    sm_gitdir);
	}

	connect_work_ਰੁੱਖ_and_git_dir(clone_ਅੰਕੜੇ_ਰਸਤਾ, sm_gitdir, 0);

	p = repo_submodule_ਰਸਤਾ(the_repository, clone_ਅੰਕੜੇ_ਰਸਤਾ, "config");
	ਜੇ (!p)
		die(_("could not get submodule directory for '%s'"), clone_ਅੰਕੜੇ_ਰਸਤਾ);

	/* setup alternateLocation and alternateErrorStrategy in the cloned submodule if needed */
	repo_config_get_string(the_repository, "submodule.alternateLocation", &sm_alternate);
	ਜੇ (sm_alternate)
		repo_config_ਸੈੱਟ_ਵਿੱਚ_file(the_repository, p, "submodule.alternateLocation",
					sm_alternate);
	repo_config_get_string(the_repository, "submodule.alternateErrorStrategy", &error_strategy);
	ਜੇ (error_strategy)
		repo_config_ਸੈੱਟ_ਵਿੱਚ_file(the_repository, p, "submodule.alternateErrorStrategy",
					error_strategy);

	ਮੁਕਤ(sm_alternate);
	ਮੁਕਤ(error_strategy);

	ਮੁਕਤ(sm_gitdir);
	ਮੁਕਤ(p);
	ਮੁਕਤ(ਮੰਜ਼ਿਲ_free);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ module_clone(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ dissociate = 0, quiet = 0, ਤਰੱਕੀ = 0, require_init = 0;
	ਬਣਤਰ module_clone_ਅੰਕੜੇ clone_ਅੰਕੜੇ = MODULE_CLONE_ਅੰਕੜੇ_INIT;
	ਬਣਤਰ string_list reference = STRING_LIST_INIT_NODUP;
	ਬਣਤਰ list_objects_filter_options filter_options =
		LIST_OBJECTS_FILTER_INIT;
	ਅਚਲ ਅੱਖਰ *ref_storage_format = ਨਿਰਾ;

	ਬਣਤਰ option module_clone_options[] = {
		OPT_STRING(0, "prefix", &clone_ਅੰਕੜੇ.ਅਗੇਤਰ,
			   N_("path"),
			   N_("alternative anchor for relative paths")),
		OPT_STRING(0, "path", &clone_ਅੰਕੜੇ.ਰਸਤਾ,
			   N_("path"),
			   N_("where the new submodule will be cloned to")),
		OPT_STRING(0, "name", &clone_ਅੰਕੜੇ.name,
			   N_("string"),
			   N_("name of the new submodule")),
		OPT_STRING(0, "url", &clone_ਅੰਕੜੇ.url,
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
		OPT_INTEGER(0, "depth", &clone_ਅੰਕੜੇ.ਡੂੰਘਾਈ,
			   N_("depth for shallow clones")),
		OPT__QUIET(&quiet, "suppress output for cloning a submodule"),
		OPT_BOOL(0, "progress", &ਤਰੱਕੀ,
			   N_("force cloning progress")),
		OPT_BOOL(0, "require-init", &require_init,
			   N_("disallow cloning into non-empty directory")),
		OPT_BOOL(0, "single-branch", &clone_ਅੰਕੜੇ.single_branch,
			 N_("clone only one branch, HEAD or --branch")),
		OPT_PARSE_LIST_OBJECTS_FILTER(&filter_options),
		OPT_ਸਿਰਾ()
	};
	ਅਚਲ ਅੱਖਰ *ਅਚਲ git_submodule_helper_usage[] = {
		N_("git submodule--helper clone [--prefix=<path>] [--quiet] "
		   "[--reference <repository>] [--name <name>] [--depth <depth>] "
		   "[--single-branch] [--filter <filter-spec>] "
		   "--url <url> --path <path>"),
		ਨਿਰਾ
	};

	argc = parse_options(argc, argv, ਅਗੇਤਰ, module_clone_options,
			     git_submodule_helper_usage, 0);

	ਜੇ (ref_storage_format) {
		clone_ਅੰਕੜੇ.ref_storage_format = ref_storage_format_by_name(ref_storage_format);
		ਜੇ (clone_ਅੰਕੜੇ.ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format);
	}
	clone_ਅੰਕੜੇ.dissociate = !!dissociate;
	clone_ਅੰਕੜੇ.quiet = !!quiet;
	clone_ਅੰਕੜੇ.ਤਰੱਕੀ = !!ਤਰੱਕੀ;
	clone_ਅੰਕੜੇ.require_init = !!require_init;
	clone_ਅੰਕੜੇ.filter_options = &filter_options;

	ਜੇ (argc || !clone_ਅੰਕੜੇ.url || !clone_ਅੰਕੜੇ.ਰਸਤਾ || !*(clone_ਅੰਕੜੇ.ਰਸਤਾ))
		usage_with_options(git_submodule_helper_usage,
				   module_clone_options);

	clone_submodule(&clone_ਅੰਕੜੇ, &reference);
	list_objects_filter_release(&filter_options);
	string_list_clear(&reference, 1);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ determine_submodule_update_strategy(ਬਣਤਰ repository *r,
					       ਪੂਰਨ just_cloned,
					       ਅਚਲ ਅੱਖਰ *ਰਸਤਾ,
					       ਗਿਣਤੀ submodule_update_type update,
					       ਬਣਤਰ submodule_update_strategy *ਬਾਹਰਲਾ)
{
	ਅਚਲ ਬਣਤਰ submodule *sub = submodule_from_ਰਸਤਾ(r, null_oid(the_hash_algo), ਰਸਤਾ);
	ਅੱਖਰ *ਕੁੰਜੀ;
	ਅਚਲ ਅੱਖਰ *val;
	ਪੂਰਨ ret;

	/*
	 * NEEDSWORK: audit and ensure that update_submodule() has right
	 * to assume that submodule_from_path() above will always succeed.
	 */
	ਜੇ (!sub)
		BUG("update_submodule assumes a submodule exists at path (%s)",
		    ਰਸਤਾ);
	ਕੁੰਜੀ = xstrfmt("submodule.%s.update", sub->name);

	ਜੇ (update) {
		ਬਾਹਰਲਾ->type = update;
	} ਨਹੀਂਤਾਂ ਜੇ (!repo_config_get_string_tmp(r, ਕੁੰਜੀ, &val)) {
		ਜੇ (parse_submodule_update_strategy(val, ਬਾਹਰਲਾ) < 0) {
			ret = die_ਸੁਨੇਹਾ(_("Invalid update mode '%s' configured for submodule path '%s'"),
					  val, ਰਸਤਾ);
			ਜਾਓ cleanup;
		}
	} ਨਹੀਂਤਾਂ ਜੇ (sub->update_strategy.type != SM_UPDATE_UNSPECIFIED) {
		ਜੇ (sub->update_strategy.type == SM_UPDATE_COMMAND)
			BUG("how did we read update = !command from .gitmodules?");
		ਬਾਹਰਲਾ->type = sub->update_strategy.type;
		ਬਾਹਰਲਾ->command = sub->update_strategy.command;
	} ਨਹੀਂਤਾਂ
		ਬਾਹਰਲਾ->type = SM_UPDATE_CHECKOUT;

	ਜੇ (just_cloned &&
	    (ਬਾਹਰਲਾ->type == SM_UPDATE_ਮਿਲਾਓ ||
	     ਬਾਹਰਲਾ->type == SM_UPDATE_REBASE ||
	     ਬਾਹਰਲਾ->type == SM_UPDATE_NONE))
		ਬਾਹਰਲਾ->type = SM_UPDATE_CHECKOUT;

	ret = 0;
cleanup:
	ਮੁਕਤ(ਕੁੰਜੀ);
	ਵਾਪਸ ret;
}

ਬਣਤਰ update_clone_ਅੰਕੜੇ {
	ਅਚਲ ਬਣਤਰ submodule *sub;
	ਬਣਤਰ object_id oid;
	ਬਿਨਾਨਿਸ਼ਾਨੀ just_cloned;
};

ਬਣਤਰ submodule_update_clone {
	/* index into 'update_data.list', the list of submodules to look into for cloning */
	ਪੂਰਨ current;

	/* configuration parameters which are passed on to the children */
	ਅਚਲ ਬਣਤਰ update_ਅੰਕੜੇ *update_ਅੰਕੜੇ;

	/* to be consumed by update_submodule() */
	ਬਣਤਰ update_clone_ਅੰਕੜੇ *update_clone;
	ਪੂਰਨ update_clone_nr; ਪੂਰਨ update_clone_alloc;

	/* If we want to stop as fast as possible and return an error */
	ਬਿਨਾਨਿਸ਼ਾਨੀ quickstop : 1;

	/* failed clones to be retried again */
	ਅਚਲ ਬਣਤਰ cache_ਇੰਦਰਾਜ **failed_clones;
	ਪੂਰਨ failed_clones_nr, failed_clones_alloc;
};
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SUBMODULE_UPDATE_CLONE_INIT { 0 }

ਸਥਿਰ ਖਾਲੀ submodule_update_clone_release(ਬਣਤਰ submodule_update_clone *suc)
{
	ਮੁਕਤ(suc->update_clone);
	ਮੁਕਤ(suc->failed_clones);
}

ਬਣਤਰ update_ਅੰਕੜੇ {
	ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ;
	ਅਚਲ ਅੱਖਰ *super_ਅਗੇਤਰ;
	ਅੱਖਰ *displaypath;
	ਗਿਣਤੀ submodule_update_type update_default;
	ਬਣਤਰ object_id suboid;
	ਬਣਤਰ string_list references;
	ਬਣਤਰ submodule_update_strategy update_strategy;
	ਬਣਤਰ list_objects_filter_options *filter_options;
	ਬਣਤਰ module_list list;
	ਗਿਣਤੀ ref_storage_format ref_storage_format;
	ਪੂਰਨ ਡੂੰਘਾਈ;
	ਪੂਰਨ max_jobs;
	ਪੂਰਨ single_branch;
	ਪੂਰਨ recommend_shallow;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ require_init;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ force;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ quiet;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ nofetch;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ remote;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਤਰੱਕੀ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ dissociate;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ init;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ warn_if_uninitialized;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ recursive;

	/* copied over from update_clone_data */
	ਬਣਤਰ object_id oid;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ just_cloned;
	ਅਚਲ ਅੱਖਰ *sm_ਰਸਤਾ;
};
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ UPDATE_ਅੰਕੜੇ_INIT { \
	.update_strategy = SUBMODULE_UPDATE_STRATEGY_INIT, \
	.list = MODULE_LIST_INIT, \
	.ref_storage_format = REF_STORAGE_FORMAT_UNKNOWN, \
	.recommend_shallow = -1, \
	.references = STRING_LIST_INIT_DUP, \
	.single_branch = -1, \
	.max_jobs = 1, \
}

ਸਥਿਰ ਖਾਲੀ update_ਅੰਕੜੇ_release(ਬਣਤਰ update_ਅੰਕੜੇ *ud)
{
	ਮੁਕਤ(ud->displaypath);
	submodule_update_strategy_release(&ud->update_strategy);
	module_list_release(&ud->list);
}

ਸਥਿਰ ਖਾਲੀ ਅਗਲਾ_submodule_warn_missing(ਬਣਤਰ submodule_update_clone *suc,
		ਬਣਤਰ strbuf *ਬਾਹਰਲਾ, ਅਚਲ ਅੱਖਰ *displaypath)
{
	/*
	 * Only mention uninitialized submodules when their
	 * paths have been specified.
	 */
	ਜੇ (suc->update_ਅੰਕੜੇ->warn_if_uninitialized) {
		strbuf_addf(ਬਾਹਰਲਾ,
			_("Submodule path '%s' not initialized"),
			displaypath);
		strbuf_addch(ਬਾਹਰਲਾ, '\n');
		strbuf_addstr(ਬਾਹਰਲਾ,
			_("Maybe you want to use 'update --init'?"));
		strbuf_addch(ਬਾਹਰਲਾ, '\n');
	}
}

/**
 * Determine whether 'ce' needs to be cloned. If so, prepare the 'child' to
 * run the clone. Returns 1 if 'ce' needs to be cloned, 0 otherwise.
 */
ਸਥਿਰ ਪੂਰਨ prepare_ਮੰਜ਼ਿਲ_clone_ਅਗਲਾ_submodule(ਅਚਲ ਬਣਤਰ cache_ਇੰਦਰਾਜ *ce,
					   ਬਣਤਰ ਬੱਚਾ_process *ਬੱਚਾ,
					   ਬਣਤਰ submodule_update_clone *suc,
					   ਬਣਤਰ strbuf *ਬਾਹਰਲਾ)
{
	ਅਚਲ ਬਣਤਰ submodule *sub = ਨਿਰਾ;
	ਅਚਲ ਅੱਖਰ *url = ਨਿਰਾ;
	ਅਚਲ ਅੱਖਰ *update_string;
	ਗਿਣਤੀ submodule_update_type update_type;
	ਅੱਖਰ *ਕੁੰਜੀ;
	ਅਚਲ ਬਣਤਰ update_ਅੰਕੜੇ *ud = suc->update_ਅੰਕੜੇ;
	ਅੱਖਰ *displaypath = get_submodule_displaypath(ce->name, ud->ਅਗੇਤਰ,
						      ud->super_ਅਗੇਤਰ);
	ਬਣਤਰ strbuf sb = STRBUF_INIT;
	ਪੂਰਨ needs_cloning = 0;
	ਪੂਰਨ need_free_url = 0;

	ਜੇ (ce_stage(ce)) {
		strbuf_addf(ਬਾਹਰਲਾ, _("Skipping unmerged submodule %s"), displaypath);
		strbuf_addch(ਬਾਹਰਲਾ, '\n');
		ਜਾਓ cleanup;
	}

	sub = submodule_from_ਰਸਤਾ(the_repository, null_oid(the_hash_algo), ce->name);

	ਜੇ (!sub) {
		ਅਗਲਾ_submodule_warn_missing(suc, ਬਾਹਰਲਾ, displaypath);
		ਜਾਓ cleanup;
	}

	ਕੁੰਜੀ = xstrfmt("submodule.%s.update", sub->name);
	ਜੇ (!repo_config_get_string_tmp(the_repository, ਕੁੰਜੀ, &update_string)) {
		update_type = parse_submodule_update_type(update_string);
	} ਨਹੀਂਤਾਂ {
		update_type = sub->update_strategy.type;
	}
	ਮੁਕਤ(ਕੁੰਜੀ);

	ਜੇ (suc->update_ਅੰਕੜੇ->update_strategy.type == SM_UPDATE_NONE
	    || (suc->update_ਅੰਕੜੇ->update_strategy.type == SM_UPDATE_UNSPECIFIED
		&& update_type == SM_UPDATE_NONE)) {
		strbuf_addf(ਬਾਹਰਲਾ, _("Skipping submodule '%s'"), displaypath);
		strbuf_addch(ਬਾਹਰਲਾ, '\n');
		ਜਾਓ cleanup;
	}

	/* Check if the submodule has been initialized. */
	ਜੇ (!is_submodule_ਸਰਗਰਮ(the_repository, ce->name)) {
		ਅਗਲਾ_submodule_warn_missing(suc, ਬਾਹਰਲਾ, displaypath);
		ਜਾਓ cleanup;
	}

	strbuf_reset(&sb);
	strbuf_addf(&sb, "submodule.%s.url", sub->name);
	ਜੇ (repo_config_get_string_tmp(the_repository, sb.buf, &url)) {
		ਜੇ (sub->url && (starts_with_dot_slash(sub->url) ||
				 starts_with_dot_dot_slash(sub->url))) {
			url = resolve_relative_url(sub->url, ਨਿਰਾ, 0);
			need_free_url = 1;
		} ਨਹੀਂਤਾਂ
			url = sub->url;
	}

	ਜੇ (!url)
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

	ਜੇ (!needs_cloning)
		ਜਾਓ cleanup;

	ਬੱਚਾ->git_cmd = 1;
	ਬੱਚਾ->no_stdin = 1;
	ਬੱਚਾ->stdout_ਮੰਜ਼ਿਲ_stderr = 1;
	ਬੱਚਾ->err = -1;
	strvec_ਧੱਕੋ(&ਬੱਚਾ->ਦਲੀਲਾਂ, "submodule--helper");
	strvec_ਧੱਕੋ(&ਬੱਚਾ->ਦਲੀਲਾਂ, "clone");
	ਜੇ (suc->update_ਅੰਕੜੇ->ਤਰੱਕੀ)
		strvec_ਧੱਕੋ(&ਬੱਚਾ->ਦਲੀਲਾਂ, "--progress");
	ਜੇ (suc->update_ਅੰਕੜੇ->quiet)
		strvec_ਧੱਕੋ(&ਬੱਚਾ->ਦਲੀਲਾਂ, "--quiet");
	ਜੇ (suc->update_ਅੰਕੜੇ->ਅਗੇਤਰ)
		strvec_pushl(&ਬੱਚਾ->ਦਲੀਲਾਂ, "--prefix", suc->update_ਅੰਕੜੇ->ਅਗੇਤਰ, ਨਿਰਾ);
	ਜੇ (suc->update_ਅੰਕੜੇ->recommend_shallow && sub->recommend_shallow == 1)
		strvec_ਧੱਕੋ(&ਬੱਚਾ->ਦਲੀਲਾਂ, "--depth=1");
	ਨਹੀਂਤਾਂ ਜੇ (suc->update_ਅੰਕੜੇ->ਡੂੰਘਾਈ)
		strvec_pushf(&ਬੱਚਾ->ਦਲੀਲਾਂ, "--depth=%d", suc->update_ਅੰਕੜੇ->ਡੂੰਘਾਈ);
	ਜੇ (suc->update_ਅੰਕੜੇ->filter_options && suc->update_ਅੰਕੜੇ->filter_options->choice)
		strvec_pushf(&ਬੱਚਾ->ਦਲੀਲਾਂ, "--filter=%s",
			     expand_list_objects_filter_spec(suc->update_ਅੰਕੜੇ->filter_options));
	ਜੇ (suc->update_ਅੰਕੜੇ->require_init)
		strvec_ਧੱਕੋ(&ਬੱਚਾ->ਦਲੀਲਾਂ, "--require-init");
	ਜੇ (suc->update_ਅੰਕੜੇ->ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
		strvec_pushf(&ਬੱਚਾ->ਦਲੀਲਾਂ, "--ref-storage-format=%s",
			     ref_storage_format_ਮੰਜ਼ਿਲ_name(suc->update_ਅੰਕੜੇ->ref_storage_format));
	strvec_pushl(&ਬੱਚਾ->ਦਲੀਲਾਂ, "--path", sub->ਰਸਤਾ, ਨਿਰਾ);
	strvec_pushl(&ਬੱਚਾ->ਦਲੀਲਾਂ, "--name", sub->name, ਨਿਰਾ);
	strvec_pushl(&ਬੱਚਾ->ਦਲੀਲਾਂ, "--url", url, ਨਿਰਾ);
	ਜੇ (suc->update_ਅੰਕੜੇ->references.nr) {
		ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ;

		for_each_string_list_ਵਸਤੂ(ਵਸਤੂ, &suc->update_ਅੰਕੜੇ->references)
			strvec_pushl(&ਬੱਚਾ->ਦਲੀਲਾਂ, "--reference", ਵਸਤੂ->string, ਨਿਰਾ);
	}
	ਜੇ (suc->update_ਅੰਕੜੇ->dissociate)
		strvec_ਧੱਕੋ(&ਬੱਚਾ->ਦਲੀਲਾਂ, "--dissociate");
	ਜੇ (suc->update_ਅੰਕੜੇ->single_branch >= 0)
		strvec_ਧੱਕੋ(&ਬੱਚਾ->ਦਲੀਲਾਂ, suc->update_ਅੰਕੜੇ->single_branch ?
					      "--single-branch" :
					      "--no-single-branch");

cleanup:
	ਮੁਕਤ(displaypath);
	strbuf_release(&sb);
	ਜੇ (need_free_url)
		ਮੁਕਤ((ਖਾਲੀ*)url);

	ਵਾਪਸ needs_cloning;
}

ਸਥਿਰ ਪੂਰਨ update_clone_get_ਅਗਲਾ_task(ਬਣਤਰ ਬੱਚਾ_process *ਬੱਚਾ,
				      ਬਣਤਰ strbuf *err,
				      ਖਾਲੀ *suc_cb,
				      ਖਾਲੀ **idx_task_cb)
{
	ਬਣਤਰ submodule_update_clone *suc = suc_cb;
	ਅਚਲ ਬਣਤਰ cache_ਇੰਦਰਾਜ *ce;
	ਪੂਰਨ ਸੂਚਕਾਂਕ;

	ਲਈ (; suc->current < suc->update_ਅੰਕੜੇ->list.nr; suc->current++) {
		ce = suc->update_ਅੰਕੜੇ->list.entries[suc->current];
		ਜੇ (prepare_ਮੰਜ਼ਿਲ_clone_ਅਗਲਾ_submodule(ce, ਬੱਚਾ, suc, err)) {
			ਪੂਰਨ *p = xmalloc(ਆਕਾਰ(*p));

			*p = suc->current;
			*idx_task_cb = p;
			suc->current++;
			ਵਾਪਸ 1;
		}
	}

	/*
	 * The loop above tried cloning each submodule once, now try the
	 * stragglers again, which we can imagine as an extension of the
	 * entry list.
	 */
	ਸੂਚਕਾਂਕ = suc->current - suc->update_ਅੰਕੜੇ->list.nr;
	ਜੇ (ਸੂਚਕਾਂਕ < suc->failed_clones_nr) {
		ਪੂਰਨ *p;

		ce = suc->failed_clones[ਸੂਚਕਾਂਕ];
		ਜੇ (!prepare_ਮੰਜ਼ਿਲ_clone_ਅਗਲਾ_submodule(ce, ਬੱਚਾ, suc, err)) {
			suc->current ++;
			strbuf_addstr(err, "BUG: submodule considered for "
					   "cloning, doesn't need cloning "
					   "any more?\n");
			ਵਾਪਸ 0;
		}
		p = xmalloc(ਆਕਾਰ(*p));
		*p = suc->current;
		*idx_task_cb = p;
		suc->current ++;
		ਵਾਪਸ 1;
	}

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ update_clone_ਸ਼ੁਰੂ_failure(ਬਣਤਰ strbuf *err ਅਣਵਰਤਿਆ,
				      ਖਾਲੀ *suc_cb,
				      ਖਾਲੀ *idx_task_cb ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ submodule_update_clone *suc = suc_cb;

	suc->quickstop = 1;
	ਵਾਪਸ 1;
}

ਸਥਿਰ ਪੂਰਨ update_clone_task_ਮੁਕੰਮਲ(ਪੂਰਨ ਨਤੀਜਾ,
				      ਬਣਤਰ strbuf *err,
				      ਖਾਲੀ *suc_cb,
				      ਖਾਲੀ *idx_task_cb)
{
	ਅਚਲ ਬਣਤਰ cache_ਇੰਦਰਾਜ *ce;
	ਬਣਤਰ submodule_update_clone *suc = suc_cb;
	ਪੂਰਨ *idxP = idx_task_cb;
	ਪੂਰਨ idx = *idxP;

	ਮੁਕਤ(idxP);

	ਜੇ (!ਨਤੀਜਾ)
		ਵਾਪਸ 0;

	ਜੇ (idx < suc->update_ਅੰਕੜੇ->list.nr) {
		ce  = suc->update_ਅੰਕੜੇ->list.entries[idx];
		strbuf_addf(err, _("Failed to clone '%s'. Retry scheduled"),
			    ce->name);
		strbuf_addch(err, '\n');
		ALLOC_GROW(suc->failed_clones,
			   suc->failed_clones_nr + 1,
			   suc->failed_clones_alloc);
		suc->failed_clones[suc->failed_clones_nr++] = ce;
		ਵਾਪਸ 0;
	} ਨਹੀਂਤਾਂ {
		idx -= suc->update_ਅੰਕੜੇ->list.nr;
		ce  = suc->failed_clones[idx];
		strbuf_addf(err, _("Failed to clone '%s' a second time, aborting"),
			    ce->name);
		strbuf_addch(err, '\n');
		suc->quickstop = 1;
		ਵਾਪਸ 1;
	}

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ git_update_clone_config(ਅਚਲ ਅੱਖਰ *var, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ,
				   ਅਚਲ ਬਣਤਰ config_context *ctx,
				   ਖਾਲੀ *cb)
{
	ਪੂਰਨ *max_jobs = cb;

	ਜੇ (!strcmp(var, "submodule.fetchjobs"))
		*max_jobs = parse_submodule_fetchjobs(var, ਮੁੱਲ, ctx->kvi);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ is_tip_reachable(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਅਚਲ ਬਣਤਰ object_id *oid)
{
	ਬਣਤਰ ਬੱਚਾ_process cp = ਬੱਚਾ_PROCESS_INIT;
	ਬਣਤਰ strbuf rev = STRBUF_INIT;
	ਅੱਖਰ *hex = oid_ਮੰਜ਼ਿਲ_hex(oid);
	ਪੂਰਨ reachable;

	cp.git_cmd = 1;
	cp.dir = ਰਸਤਾ;
	cp.no_stderr = 1;
	strvec_pushl(&cp.ਦਲੀਲਾਂ, "rev-list", "-n", "1", hex, "--not", "--all", ਨਿਰਾ);

	prepare_submodule_repo_env(&cp.env);

	ਜੇ (capture_command(&cp, &rev, GIT_MAX_HEXSZ + 1) || rev.len)
		reachable = 0;
	ਨਹੀਂਤਾਂ
		reachable = 1;

	strbuf_release(&rev);
	ਵਾਪਸ reachable;
}

ਸਥਿਰ ਪੂਰਨ fetch_ਵਿੱਚ_submodule(ਅਚਲ ਅੱਖਰ *module_ਰਸਤਾ, ਪੂਰਨ ਡੂੰਘਾਈ, ਪੂਰਨ quiet,
			      ਅਚਲ ਬਣਤਰ object_id *oid)
{
	ਬਣਤਰ ਬੱਚਾ_process cp = ਬੱਚਾ_PROCESS_INIT;

	prepare_submodule_repo_env(&cp.env);
	cp.git_cmd = 1;
	cp.dir = module_ਰਸਤਾ;

	strvec_ਧੱਕੋ(&cp.ਦਲੀਲਾਂ, "fetch");
	ਜੇ (quiet)
		strvec_ਧੱਕੋ(&cp.ਦਲੀਲਾਂ, "--quiet");
	ਜੇ (ਡੂੰਘਾਈ)
		strvec_pushf(&cp.ਦਲੀਲਾਂ, "--depth=%d", ਡੂੰਘਾਈ);
	ਜੇ (oid) {
		ਅੱਖਰ *hex = oid_ਮੰਜ਼ਿਲ_hex(oid);
		ਅੱਖਰ *remote;
		ਪੂਰਨ ਕੋਡ;

		ਕੋਡ = get_default_remote_submodule(module_ਰਸਤਾ, &remote);
		ਜੇ (ਕੋਡ) {
			ਬੱਚਾ_process_clear(&cp);
			ਵਾਪਸ ਕੋਡ;
		}

		strvec_pushl(&cp.ਦਲੀਲਾਂ, remote, hex, ਨਿਰਾ);
		ਮੁਕਤ(remote);
	}

	ਵਾਪਸ ਲੜੀਵਾਰ_command(&cp);
}

ਸਥਿਰ ਪੂਰਨ ਲੜੀਵਾਰ_update_command(ਅਚਲ ਬਣਤਰ update_ਅੰਕੜੇ *ud, ਪੂਰਨ subforce)
{
	ਬਣਤਰ ਬੱਚਾ_process cp = ਬੱਚਾ_PROCESS_INIT;
	ਅੱਖਰ *oid = oid_ਮੰਜ਼ਿਲ_hex(&ud->oid);
	ਪੂਰਨ ret;

	ਚੋਣ (ud->update_strategy.type) {
	ਹਾਲਤ SM_UPDATE_CHECKOUT:
		cp.git_cmd = 1;
		strvec_pushl(&cp.ਦਲੀਲਾਂ, "checkout", "-q", ਨਿਰਾ);
		ਜੇ (subforce)
			strvec_ਧੱਕੋ(&cp.ਦਲੀਲਾਂ, "-f");
		ਰੋਕੋ;
	ਹਾਲਤ SM_UPDATE_REBASE:
		cp.git_cmd = 1;
		strvec_ਧੱਕੋ(&cp.ਦਲੀਲਾਂ, "rebase");
		ਜੇ (ud->quiet)
			strvec_ਧੱਕੋ(&cp.ਦਲੀਲਾਂ, "--quiet");
		ਰੋਕੋ;
	ਹਾਲਤ SM_UPDATE_ਮਿਲਾਓ:
		cp.git_cmd = 1;
		strvec_ਧੱਕੋ(&cp.ਦਲੀਲਾਂ, "merge");
		ਜੇ (ud->quiet)
			strvec_ਧੱਕੋ(&cp.ਦਲੀਲਾਂ, "--quiet");
		ਰੋਕੋ;
	ਹਾਲਤ SM_UPDATE_COMMAND:
		cp.use_shell = 1;
		strvec_ਧੱਕੋ(&cp.ਦਲੀਲਾਂ, ud->update_strategy.command);
		ਰੋਕੋ;
	ਮੂਲ:
		BUG("unexpected update strategy type: %d",
		    ud->update_strategy.type);
	}
	strvec_ਧੱਕੋ(&cp.ਦਲੀਲਾਂ, oid);

	cp.dir = ud->sm_ਰਸਤਾ;
	prepare_submodule_repo_env(&cp.env);
	ਜੇ ((ret = ਲੜੀਵਾਰ_command(&cp))) {
		ਚੋਣ (ud->update_strategy.type) {
		ਹਾਲਤ SM_UPDATE_CHECKOUT:
			die_ਸੁਨੇਹਾ(_("Unable to checkout '%s' in submodule path '%s'"),
				    oid, ud->displaypath);
			/* No "ret" assignment, use "git checkout"'s */
			ਰੋਕੋ;
		ਹਾਲਤ SM_UPDATE_REBASE:
			ret = die_ਸੁਨੇਹਾ(_("Unable to rebase '%s' in submodule path '%s'"),
					  oid, ud->displaypath);
			ਰੋਕੋ;
		ਹਾਲਤ SM_UPDATE_ਮਿਲਾਓ:
			ret = die_ਸੁਨੇਹਾ(_("Unable to merge '%s' in submodule path '%s'"),
					  oid, ud->displaypath);
			ਰੋਕੋ;
		ਹਾਲਤ SM_UPDATE_COMMAND:
			ret = die_ਸੁਨੇਹਾ(_("Execution of '%s %s' failed in submodule path '%s'"),
					  ud->update_strategy.command, oid, ud->displaypath);
			ਰੋਕੋ;
		ਮੂਲ:
			BUG("unexpected update strategy type: %d",
			    ud->update_strategy.type);
		}

		ਵਾਪਸ ret;
	}

	ਜੇ (ud->quiet)
		ਵਾਪਸ 0;

	ਚੋਣ (ud->update_strategy.type) {
	ਹਾਲਤ SM_UPDATE_CHECKOUT:
		ਲਿਖੋ(_("Submodule path '%s': checked out '%s'\n"),
		       ud->displaypath, oid);
		ਰੋਕੋ;
	ਹਾਲਤ SM_UPDATE_REBASE:
		ਲਿਖੋ(_("Submodule path '%s': rebased into '%s'\n"),
		       ud->displaypath, oid);
		ਰੋਕੋ;
	ਹਾਲਤ SM_UPDATE_ਮਿਲਾਓ:
		ਲਿਖੋ(_("Submodule path '%s': merged in '%s'\n"),
		       ud->displaypath, oid);
		ਰੋਕੋ;
	ਹਾਲਤ SM_UPDATE_COMMAND:
		ਲਿਖੋ(_("Submodule path '%s': '%s %s'\n"),
		       ud->displaypath, ud->update_strategy.command, oid);
		ਰੋਕੋ;
	ਮੂਲ:
		BUG("unexpected update strategy type: %d",
		    ud->update_strategy.type);
	}

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ ਲੜੀਵਾਰ_update_procedure(ਅਚਲ ਬਣਤਰ update_ਅੰਕੜੇ *ud)
{
	ਪੂਰਨ subforce = is_null_oid(&ud->suboid) || ud->force;

	ਜੇ (!ud->nofetch) {
		/*
		 * Run fetch only if `oid` isn't present or it
		 * is not reachable from a ref.
		 */
		ਜੇ (!is_tip_reachable(ud->sm_ਰਸਤਾ, &ud->oid) &&
		    fetch_ਵਿੱਚ_submodule(ud->sm_ਰਸਤਾ, ud->ਡੂੰਘਾਈ, ud->quiet, ਨਿਰਾ) &&
		    !ud->quiet)
			fprintf_ln(stderr,
				   _("Unable to fetch in submodule path '%s'; "
				     "trying to directly fetch %s:"),
				   ud->displaypath, oid_ਮੰਜ਼ਿਲ_hex(&ud->oid));
		/*
		 * Now we tried the usual fetch, but `oid` may
		 * not be reachable from any of the refs.
		 */
		ਜੇ (!is_tip_reachable(ud->sm_ਰਸਤਾ, &ud->oid) &&
		    fetch_ਵਿੱਚ_submodule(ud->sm_ਰਸਤਾ, ud->ਡੂੰਘਾਈ, ud->quiet, &ud->oid))
			ਵਾਪਸ die_ਸੁਨੇਹਾ(_("Fetched in submodule path '%s', but it did not "
					     "contain %s. Direct fetching of that commit failed."),
					   ud->displaypath, oid_ਮੰਜ਼ਿਲ_hex(&ud->oid));
	}

	ਵਾਪਸ ਲੜੀਵਾਰ_update_command(ud, subforce);
}

ਸਥਿਰ ਪੂਰਨ remote_submodule_branch(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਅਚਲ ਅੱਖਰ **branch)
{
	ਅਚਲ ਬਣਤਰ submodule *sub;
	ਅੱਖਰ *ਕੁੰਜੀ;
	*branch = ਨਿਰਾ;

	sub = submodule_from_ਰਸਤਾ(the_repository, null_oid(the_hash_algo), ਰਸਤਾ);
	ਜੇ (!sub)
		ਵਾਪਸ die_ਸੁਨੇਹਾ(_("could not initialize submodule at path '%s'"),
				   ਰਸਤਾ);

	ਕੁੰਜੀ = xstrfmt("submodule.%s.branch", sub->name);
	ਜੇ (repo_config_get_string_tmp(the_repository, ਕੁੰਜੀ, branch))
		*branch = sub->branch;
	ਮੁਕਤ(ਕੁੰਜੀ);

	ਜੇ (!*branch) {
		*branch = "HEAD";
		ਵਾਪਸ 0;
	}

	ਜੇ (!strcmp(*branch, ".")) {
		ਅਚਲ ਅੱਖਰ *refname = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
							      "HEAD", 0, ਨਿਰਾ,
							      ਨਿਰਾ);

		ਜੇ (!refname)
			ਵਾਪਸ die_ਸੁਨੇਹਾ(_("No such ref: %s"), "HEAD");

		/* detached HEAD */
		ਜੇ (!strcmp(refname, "HEAD"))
			ਵਾਪਸ die_ਸੁਨੇਹਾ(_("Submodule (%s) branch configured to inherit "
					     "branch from superproject, but the superproject "
					     "is not on any branch"), sub->name);

		ਜੇ (!skip_ਅਗੇਤਰ(refname, "refs/heads/", &refname))
			ਵਾਪਸ die_ਸੁਨੇਹਾ(_("Expecting a full ref name, got %s"),
					   refname);

		*branch = refname;
		ਵਾਪਸ 0;
	}

	/* Our "branch" is coming from repo_config_get_string_tmp() */
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ ensure_core_worktree(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ)
{
	ਅਚਲ ਅੱਖਰ *cw;
	ਬਣਤਰ repository subrepo;

	ਜੇ (repo_submodule_init(&subrepo, the_repository, ਰਸਤਾ, null_oid(the_hash_algo)))
		ਵਾਪਸ die_ਸੁਨੇਹਾ(_("could not get a repository handle for submodule '%s'"),
				   ਰਸਤਾ);

	ਜੇ (!repo_config_get_string_tmp(&subrepo, "core.worktree", &cw)) {
		ਅੱਖਰ *cfg_file, *abs_ਰਸਤਾ;
		ਅਚਲ ਅੱਖਰ *rel_ਰਸਤਾ;
		ਬਣਤਰ strbuf sb = STRBUF_INIT;

		cfg_file = repo_git_ਰਸਤਾ(&subrepo, "config");

		abs_ਰਸਤਾ = absolute_pathdup(ਰਸਤਾ);
		rel_ਰਸਤਾ = relative_ਰਸਤਾ(abs_ਰਸਤਾ, subrepo.gitdir, &sb);

		repo_config_ਸੈੱਟ_ਵਿੱਚ_file(the_repository, cfg_file, "core.worktree", rel_ਰਸਤਾ);

		ਮੁਕਤ(cfg_file);
		ਮੁਕਤ(abs_ਰਸਤਾ);
		strbuf_release(&sb);
	}

	repo_clear(&subrepo);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *submodule_update_type_ਮੰਜ਼ਿਲ_label(ਗਿਣਤੀ submodule_update_type type)
{
	ਚੋਣ (type) {
	ਹਾਲਤ SM_UPDATE_CHECKOUT:
		ਵਾਪਸ "checkout";
	ਹਾਲਤ SM_UPDATE_ਮਿਲਾਓ:
		ਵਾਪਸ "merge";
	ਹਾਲਤ SM_UPDATE_REBASE:
		ਵਾਪਸ "rebase";
	ਹਾਲਤ SM_UPDATE_UNSPECIFIED:
	ਹਾਲਤ SM_UPDATE_NONE:
	ਹਾਲਤ SM_UPDATE_COMMAND:
		ਰੋਕੋ;
	}
	BUG("unreachable with type %d", type);
}

ਸਥਿਰ ਖਾਲੀ update_ਅੰਕੜੇ_ਮੰਜ਼ਿਲ_ਦਲੀਲਾਂ(ਅਚਲ ਬਣਤਰ update_ਅੰਕੜੇ *update_ਅੰਕੜੇ,
				ਬਣਤਰ strvec *ਦਲੀਲਾਂ)
{
	ਗਿਣਤੀ submodule_update_type update_type = update_ਅੰਕੜੇ->update_default;

	strvec_pushl(ਦਲੀਲਾਂ, "submodule--helper", "update", "--recursive", ਨਿਰਾ);
	ਜੇ (update_ਅੰਕੜੇ->displaypath)
		strvec_pushf(ਦਲੀਲਾਂ, "--super-prefix=%s/",
			     update_ਅੰਕੜੇ->displaypath);
	strvec_pushf(ਦਲੀਲਾਂ, "--jobs=%d", update_ਅੰਕੜੇ->max_jobs);
	ਜੇ (update_ਅੰਕੜੇ->quiet)
		strvec_ਧੱਕੋ(ਦਲੀਲਾਂ, "--quiet");
	ਜੇ (update_ਅੰਕੜੇ->force)
		strvec_ਧੱਕੋ(ਦਲੀਲਾਂ, "--force");
	ਜੇ (update_ਅੰਕੜੇ->init)
		strvec_ਧੱਕੋ(ਦਲੀਲਾਂ, "--init");
	ਜੇ (update_ਅੰਕੜੇ->remote)
		strvec_ਧੱਕੋ(ਦਲੀਲਾਂ, "--remote");
	ਜੇ (update_ਅੰਕੜੇ->nofetch)
		strvec_ਧੱਕੋ(ਦਲੀਲਾਂ, "--no-fetch");
	ਜੇ (update_ਅੰਕੜੇ->dissociate)
		strvec_ਧੱਕੋ(ਦਲੀਲਾਂ, "--dissociate");
	ਜੇ (update_ਅੰਕੜੇ->ਤਰੱਕੀ)
		strvec_ਧੱਕੋ(ਦਲੀਲਾਂ, "--progress");
	ਜੇ (update_ਅੰਕੜੇ->require_init)
		strvec_ਧੱਕੋ(ਦਲੀਲਾਂ, "--require-init");
	ਜੇ (update_ਅੰਕੜੇ->ਡੂੰਘਾਈ)
		strvec_pushf(ਦਲੀਲਾਂ, "--depth=%d", update_ਅੰਕੜੇ->ਡੂੰਘਾਈ);
	ਜੇ (update_type != SM_UPDATE_UNSPECIFIED)
		strvec_pushf(ਦਲੀਲਾਂ, "--%s",
			     submodule_update_type_ਮੰਜ਼ਿਲ_label(update_type));

	ਜੇ (update_ਅੰਕੜੇ->references.nr) {
		ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ;

		for_each_string_list_ਵਸਤੂ(ਵਸਤੂ, &update_ਅੰਕੜੇ->references)
			strvec_pushl(ਦਲੀਲਾਂ, "--reference", ਵਸਤੂ->string, ਨਿਰਾ);
	}
	ਜੇ (update_ਅੰਕੜੇ->ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
		strvec_pushf(ਦਲੀਲਾਂ, "--ref-storage-format=%s",
			     ref_storage_format_ਮੰਜ਼ਿਲ_name(update_ਅੰਕੜੇ->ref_storage_format));
	ਜੇ (update_ਅੰਕੜੇ->filter_options && update_ਅੰਕੜੇ->filter_options->choice)
		strvec_pushf(ਦਲੀਲਾਂ, "--filter=%s",
				expand_list_objects_filter_spec(
					update_ਅੰਕੜੇ->filter_options));
	ਜੇ (update_ਅੰਕੜੇ->recommend_shallow == 0)
		strvec_ਧੱਕੋ(ਦਲੀਲਾਂ, "--no-recommend-shallow");
	ਨਹੀਂਤਾਂ ਜੇ (update_ਅੰਕੜੇ->recommend_shallow == 1)
		strvec_ਧੱਕੋ(ਦਲੀਲਾਂ, "--recommend-shallow");
	ਜੇ (update_ਅੰਕੜੇ->single_branch >= 0)
		strvec_ਧੱਕੋ(ਦਲੀਲਾਂ, update_ਅੰਕੜੇ->single_branch ?
				    "--single-branch" :
				    "--no-single-branch");
}

ਸਥਿਰ ਪੂਰਨ update_submodule(ਬਣਤਰ update_ਅੰਕੜੇ *update_ਅੰਕੜੇ)
{
	ਪੂਰਨ ret;

	ਜੇ (validate_submodule_ਰਸਤਾ(update_ਅੰਕੜੇ->sm_ਰਸਤਾ) < 0)
		ਵਾਪਸ -1;

	ret = determine_submodule_update_strategy(the_repository,
						  update_ਅੰਕੜੇ->just_cloned,
						  update_ਅੰਕੜੇ->sm_ਰਸਤਾ,
						  update_ਅੰਕੜੇ->update_default,
						  &update_ਅੰਕੜੇ->update_strategy);
	ਜੇ (ret)
		ਵਾਪਸ ret;

	ਜੇ (update_ਅੰਕੜੇ->just_cloned)
		oidcpy(&update_ਅੰਕੜੇ->suboid, null_oid(the_hash_algo));
	ਨਹੀਂਤਾਂ ਜੇ (repo_resolve_gitlink_ref(the_repository, update_ਅੰਕੜੇ->sm_ਰਸਤਾ,
					  "HEAD", &update_ਅੰਕੜੇ->suboid))
		ਵਾਪਸ die_ਸੁਨੇਹਾ(_("Unable to find current revision in submodule path '%s'"),
				   update_ਅੰਕੜੇ->displaypath);

	ਜੇ (update_ਅੰਕੜੇ->remote) {
		ਅੱਖਰ *remote_name;
		ਅਚਲ ਅੱਖਰ *branch;
		ਅੱਖਰ *remote_ref;
		ਪੂਰਨ ਕੋਡ;

		ਕੋਡ = get_default_remote_submodule(update_ਅੰਕੜੇ->sm_ਰਸਤਾ, &remote_name);
		ਜੇ (ਕੋਡ)
			ਵਾਪਸ ਕੋਡ;
		ਕੋਡ = remote_submodule_branch(update_ਅੰਕੜੇ->sm_ਰਸਤਾ, &branch);
		ਜੇ (ਕੋਡ) {
			ਮੁਕਤ(remote_name);
			ਵਾਪਸ ਕੋਡ;
		}
		remote_ref = xstrfmt("refs/remotes/%s/%s", remote_name, branch);

		ਮੁਕਤ(remote_name);

		ਜੇ (!update_ਅੰਕੜੇ->nofetch) {
			ਜੇ (fetch_ਵਿੱਚ_submodule(update_ਅੰਕੜੇ->sm_ਰਸਤਾ, update_ਅੰਕੜੇ->ਡੂੰਘਾਈ,
					      0, ਨਿਰਾ)) {
				ਮੁਕਤ(remote_ref);
				ਵਾਪਸ die_ਸੁਨੇਹਾ(_("Unable to fetch in submodule path '%s'"),
						   update_ਅੰਕੜੇ->sm_ਰਸਤਾ);
			}
		}

		ਜੇ (repo_resolve_gitlink_ref(the_repository, update_ਅੰਕੜੇ->sm_ਰਸਤਾ,
					     remote_ref, &update_ਅੰਕੜੇ->oid)) {
			ret = die_ਸੁਨੇਹਾ(_("Unable to find %s revision in submodule path '%s'"),
					  remote_ref, update_ਅੰਕੜੇ->sm_ਰਸਤਾ);
			ਮੁਕਤ(remote_ref);
			ਵਾਪਸ ret;
		}

		ਮੁਕਤ(remote_ref);
	}

	ਜੇ (!oideq(&update_ਅੰਕੜੇ->oid, &update_ਅੰਕੜੇ->suboid) || update_ਅੰਕੜੇ->force) {
		ret = ਲੜੀਵਾਰ_update_procedure(update_ਅੰਕੜੇ);
		ਜੇ (ret)
			ਵਾਪਸ ret;
	}

	ਜੇ (update_ਅੰਕੜੇ->recursive) {
		ਬਣਤਰ ਬੱਚਾ_process cp = ਬੱਚਾ_PROCESS_INIT;
		ਬਣਤਰ update_ਅੰਕੜੇ ਅਗਲਾ = *update_ਅੰਕੜੇ;

		ਅਗਲਾ.ਅਗੇਤਰ = ਨਿਰਾ;
		oidcpy(&ਅਗਲਾ.oid, null_oid(the_hash_algo));
		oidcpy(&ਅਗਲਾ.suboid, null_oid(the_hash_algo));

		cp.dir = update_ਅੰਕੜੇ->sm_ਰਸਤਾ;
		cp.git_cmd = 1;
		prepare_submodule_repo_env(&cp.env);
		update_ਅੰਕੜੇ_ਮੰਜ਼ਿਲ_ਦਲੀਲਾਂ(&ਅਗਲਾ, &cp.ਦਲੀਲਾਂ);

		ret = ਲੜੀਵਾਰ_command(&cp);
		ਜੇ (ret)
			die_ਸੁਨੇਹਾ(_("Failed to recurse into submodule path '%s'"),
				    update_ਅੰਕੜੇ->displaypath);
		ਵਾਪਸ ret;
	}

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ update_submodules(ਬਣਤਰ update_ਅੰਕੜੇ *update_ਅੰਕੜੇ)
{
	ਪੂਰਨ i, ret = 0;
	ਬਣਤਰ submodule_update_clone suc = SUBMODULE_UPDATE_CLONE_INIT;
	ਅਚਲ ਬਣਤਰ ਲੜੀਵਾਰ_process_parallel_opts opts = {
		.tr2_category = "submodule",
		.tr2_label = "parallel/update",

		.processes = update_ਅੰਕੜੇ->max_jobs,

		.get_ਅਗਲਾ_task = update_clone_get_ਅਗਲਾ_task,
		.ਸ਼ੁਰੂ_failure = update_clone_ਸ਼ੁਰੂ_failure,
		.task_ਮੁਕੰਮਲ = update_clone_task_ਮੁਕੰਮਲ,
		.ਅੰਕੜੇ = &suc,
	};

	suc.update_ਅੰਕੜੇ = update_ਅੰਕੜੇ;
	ਲੜੀਵਾਰ_processes_parallel(&opts);

	/*
	 * We saved the output and put it out all at once now.
	 * That means:
	 * - the listener does not have to interleave their (checkout)
	 *   work with our fetching.  The writes involved in a
	 *   checkout involve more straightforward sequential I/O.
	 * - the listener can avoid doing any work if fetching failed.
	 */
	ਜੇ (suc.quickstop) {
		ret = 1;
		ਜਾਓ cleanup;
	}

	ਲਈ (i = 0; i < suc.update_clone_nr; i++) {
		ਬਣਤਰ update_clone_ਅੰਕੜੇ ucd = suc.update_clone[i];
		ਪੂਰਨ ਕੋਡ = 128;

		oidcpy(&update_ਅੰਕੜੇ->oid, &ucd.oid);
		update_ਅੰਕੜੇ->just_cloned = ucd.just_cloned;
		update_ਅੰਕੜੇ->sm_ਰਸਤਾ = ucd.sub->ਰਸਤਾ;

		/*
		 * Verify that the submodule path does not contain any
		 * symlinks; if it does, it might have been tampered with.
		 * TODO: allow exempting it via
		 * `safe.submodule.path` or something
		 */
		ਜੇ (validate_submodule_ਰਸਤਾ(update_ਅੰਕੜੇ->sm_ਰਸਤਾ) < 0)
			ਜਾਓ fail;

		ਕੋਡ = ensure_core_worktree(update_ਅੰਕੜੇ->sm_ਰਸਤਾ);
		ਜੇ (ਕੋਡ)
			ਜਾਓ fail;

		update_ਅੰਕੜੇ->displaypath = get_submodule_displaypath(
			update_ਅੰਕੜੇ->sm_ਰਸਤਾ, update_ਅੰਕੜੇ->ਅਗੇਤਰ,
			update_ਅੰਕੜੇ->super_ਅਗੇਤਰ);
		ਕੋਡ = update_submodule(update_ਅੰਕੜੇ);
		FREE_AND_NULL(update_ਅੰਕੜੇ->displaypath);
fail:
		ਜੇ (!ਕੋਡ)
			ਜਾਰੀ;
		ret = ਕੋਡ;
		ਜੇ (ret == 128)
			ਜਾਓ cleanup;
	}

cleanup:
	submodule_update_clone_release(&suc);
	string_list_clear(&update_ਅੰਕੜੇ->references, 0);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ module_update(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			 ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ pathspec pathspec = { 0 };
	ਬਣਤਰ pathspec pathspec2 = { 0 };
	ਬਣਤਰ update_ਅੰਕੜੇ opt = UPDATE_ਅੰਕੜੇ_INIT;
	ਬਣਤਰ list_objects_filter_options filter_options =
		LIST_OBJECTS_FILTER_INIT;
	ਅਚਲ ਅੱਖਰ *ref_storage_format = ਨਿਰਾ;
	ਪੂਰਨ ret;
	ਬਣਤਰ option module_update_options[] = {
		OPT_SUPER_ਅਗੇਤਰ(&opt.super_ਅਗੇਤਰ),
		OPT__FORCE(&opt.force, N_("force checkout updates"), 0),
		OPT_BOOL('i', "init", &opt.init,
			 N_("initialize uninitialized submodules before update")),
		OPT_BOOL(0, "remote", &opt.remote,
			 N_("use SHA-1 of submodule's remote tracking branch")),
		OPT_BOOL(0, "recursive", &opt.recursive,
			 N_("traverse submodules recursively")),
		OPT_BOOL('N', "no-fetch", &opt.nofetch,
			 N_("don't fetch new objects from the remote site")),
		OPT_ਸੈੱਟ_INT(0, "checkout", &opt.update_default,
			N_("use the 'checkout' update strategy (default)"),
			SM_UPDATE_CHECKOUT),
		OPT_ਸੈੱਟ_INT('m', "merge", &opt.update_default,
			N_("use the 'merge' update strategy"),
			SM_UPDATE_ਮਿਲਾਓ),
		OPT_ਸੈੱਟ_INT('r', "rebase", &opt.update_default,
			N_("use the 'rebase' update strategy"),
			SM_UPDATE_REBASE),
		OPT_STRING_LIST(0, "reference", &opt.references, N_("repo"),
			   N_("reference repository")),
		OPT_STRING(0, "ref-storage-format", &ref_storage_format, N_("format"),
			   N_("specify the reference storage format to use")),
		OPT_ALIAS_F(0, "ref-format", "ref-storage-format", PARSE_OPT_HIDDEN),
		OPT_BOOL(0, "dissociate", &opt.dissociate,
			   N_("use --reference only while cloning")),
		OPT_INTEGER(0, "depth", &opt.ਡੂੰਘਾਈ,
			   N_("create a shallow clone truncated to the "
			      "specified number of revisions")),
		OPT_INTEGER('j', "jobs", &opt.max_jobs,
			    N_("parallel jobs")),
		OPT_BOOL(0, "recommend-shallow", &opt.recommend_shallow,
			    N_("whether the initial clone should follow the shallow recommendation")),
		OPT__QUIET(&opt.quiet, N_("don't print cloning progress")),
		OPT_BOOL(0, "progress", &opt.ਤਰੱਕੀ,
			    N_("force cloning progress")),
		OPT_BOOL(0, "require-init", &opt.require_init,
			   N_("disallow cloning into non-empty directory, implies --init")),
		OPT_BOOL(0, "single-branch", &opt.single_branch,
			 N_("clone only one branch, HEAD or --branch")),
		OPT_PARSE_LIST_OBJECTS_FILTER(&filter_options),
		OPT_ਸਿਰਾ()
	};
	ਅਚਲ ਅੱਖਰ *ਅਚਲ git_submodule_helper_usage[] = {
		N_("git submodule [--quiet] update"
		" [--init [--filter=<filter-spec>]] [--remote]"
		" [-N|--no-fetch] [-f|--force]"
		" [--checkout|--merge|--rebase]"
		" [--[no-]recommend-shallow] [--reference <repository>]"
		" [--recursive] [--[no-]single-branch] [--] [<path>...]"),
		ਨਿਰਾ
	};

	update_clone_config_from_gitmodules(the_repository, &opt.max_jobs);
	repo_config(the_repository, git_update_clone_config, &opt.max_jobs);

	argc = parse_options(argc, argv, ਅਗੇਤਰ, module_update_options,
			     git_submodule_helper_usage, 0);

	ਜੇ (opt.require_init)
		opt.init = 1;

	ਜੇ (filter_options.choice && !opt.init) {
		usage_with_options(git_submodule_helper_usage,
				   module_update_options);
	}

	ਜੇ (ref_storage_format) {
		opt.ref_storage_format = ref_storage_format_by_name(ref_storage_format);
		ਜੇ (opt.ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format);
	}

	opt.filter_options = &filter_options;
	opt.ਅਗੇਤਰ = ਅਗੇਤਰ;

	ਜੇ (opt.update_default)
		opt.update_strategy.type = opt.update_default;

	ਜੇ (module_list_compute(argv, ਅਗੇਤਰ, &pathspec, &opt.list) < 0) {
		ret = 1;
		ਜਾਓ cleanup;
	}

	ਜੇ (pathspec.nr)
		opt.warn_if_uninitialized = 1;

	ਜੇ (opt.init) {
		ਬਣਤਰ module_list list = MODULE_LIST_INIT;
		ਬਣਤਰ init_cb info = INIT_CB_INIT;

		ਜੇ (module_list_compute(argv, opt.ਅਗੇਤਰ,
					&pathspec2, &list) < 0) {
			module_list_release(&list);
			ret = 1;
			ਜਾਓ cleanup;
		}

		/*
		 * If there are no path args and submodule.active is set then,
		 * by default, only initialize 'active' modules.
		 */
		ਜੇ (!argc && !repo_config_get(the_repository, "submodule.active"))
			module_list_ਸਰਗਰਮ(&list);

		info.ਅਗੇਤਰ = opt.ਅਗੇਤਰ;
		info.super_ਅਗੇਤਰ = opt.super_ਅਗੇਤਰ;
		ਜੇ (opt.quiet)
			info.flags |= OPT_QUIET;

		for_each_listed_submodule(&list, init_submodule_cb, &info);
		module_list_release(&list);
	}

	ret = update_submodules(&opt);
cleanup:
	update_ਅੰਕੜੇ_release(&opt);
	list_objects_filter_release(&filter_options);
	clear_pathspec(&pathspec);
	clear_pathspec(&pathspec2);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ ਧੱਕੋ_check(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ ਅਣਵਰਤਿਆ,
		      ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ remote *remote;
	ਅਚਲ ਅੱਖਰ *superproject_ਸਿਰ;
	ਅੱਖਰ *ਸਿਰ;
	ਪੂਰਨ detached_ਸਿਰ = 0;
	ਬਣਤਰ object_id ਸਿਰ_oid;

	ਜੇ (argc < 3)
		die("submodule--helper push-check requires at least 2 arguments");

	/*
	 * superproject's resolved head ref.
	 * if HEAD then the superproject is in a detached head state, otherwise
	 * it will be the resolved head ref.
	 */
	superproject_ਸਿਰ = argv[1];
	argv++;
	argc--;
	/* Get the submodule's head ref and determine if it is detached */
	ਸਿਰ = refs_resolve_refdup(get_main_ref_store(the_repository), "HEAD",
				   0, &ਸਿਰ_oid, ਨਿਰਾ);
	ਜੇ (!ਸਿਰ)
		die(_("Failed to resolve HEAD as a valid ref."));
	ਜੇ (!strcmp(ਸਿਰ, "HEAD"))
		detached_ਸਿਰ = 1;

	/*
	 * The remote must be configured.
	 * This is to avoid pushing to the exact same URL as the parent.
	 */
	remote = pushremote_get(argv[1]);
	ਜੇ (!remote || remote->origin == REMOTE_UNCONFIGURED)
		die("remote '%s' not configured", argv[1]);

	/* Check the refspec */
	ਜੇ (argc > 2) {
		ਪੂਰਨ i;
		ਬਣਤਰ ref *local_refs = get_local_heads();
		ਬਣਤਰ refspec refspec = REFSPEC_INIT_ਧੱਕੋ(the_hash_algo);

		refspec_appendn(&refspec, argv + 2, argc - 2);

		ਲਈ (i = 0; i < refspec.nr; i++) {
			ਅਚਲ ਬਣਤਰ refspec_ਵਸਤੂ *rs = &refspec.ਚੀਜ਼ਾਂ[i];

			ਜੇ (rs->ਨਮੂਨਾ || rs->matching)
				ਜਾਰੀ;

			/* LHS must match a single ref */
			ਚੋਣ (ਗਿਣਤੀ_refspec_ਮੇਲ(rs->src, local_refs, ਨਿਰਾ)) {
			ਹਾਲਤ 1:
				ਰੋਕੋ;
			ਹਾਲਤ 0:
				/*
				 * If LHS matches 'HEAD' then we need to ensure
				 * that it matches the same named branch
				 * checked out in the superproject.
				 */
				ਜੇ (!strcmp(rs->src, "HEAD")) {
					ਜੇ (!detached_ਸਿਰ &&
					    !strcmp(ਸਿਰ, superproject_ਸਿਰ))
						ਰੋਕੋ;
					die("HEAD does not match the named branch in the superproject");
				}
				/* fallthrough */
			ਮੂਲ:
				die("src refspec '%s' must name a ref",
				    rs->src);
			}
		}

		refspec_clear(&refspec);
		free_refs(local_refs);
	}
	ਮੁਕਤ(ਸਿਰ);

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ absorb_git_dirs(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			   ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ i;
	ਬਣਤਰ pathspec pathspec = { 0 };
	ਬਣਤਰ module_list list = MODULE_LIST_INIT;
	ਅਚਲ ਅੱਖਰ *super_ਅਗੇਤਰ = ਨਿਰਾ;
	ਬਣਤਰ option embed_gitdir_options[] = {
		OPT_SUPER_ਅਗੇਤਰ(&super_ਅਗੇਤਰ),
		OPT_ਸਿਰਾ()
	};
	ਅਚਲ ਅੱਖਰ *ਅਚਲ git_submodule_helper_usage[] = {
		N_("git submodule absorbgitdirs [<options>] [<path>...]"),
		ਨਿਰਾ
	};
	ਪੂਰਨ ret = 1;

	argc = parse_options(argc, argv, ਅਗੇਤਰ, embed_gitdir_options,
			     git_submodule_helper_usage, 0);

	ਜੇ (module_list_compute(argv, ਅਗੇਤਰ, &pathspec, &list) < 0)
		ਜਾਓ cleanup;

	ਲਈ (i = 0; i < list.nr; i++)
		absorb_git_dir_into_superproject(list.entries[i]->name,
						 super_ਅਗੇਤਰ);

	ret = 0;
cleanup:
	clear_pathspec(&pathspec);
	module_list_release(&list);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ module_ਸੈੱਟ_url(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			  ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ quiet = 0, ret;
	ਅਚਲ ਅੱਖਰ *newurl;
	ਅਚਲ ਅੱਖਰ *ਰਸਤਾ;
	ਅੱਖਰ *config_name;
	ਬਣਤਰ option options[] = {
		OPT__QUIET(&quiet, N_("suppress output for setting url of a submodule")),
		OPT_ਸਿਰਾ()
	};
	ਅਚਲ ਅੱਖਰ *ਅਚਲ usage[] = {
		N_("git submodule set-url [--quiet] <path> <newurl>"),
		ਨਿਰਾ
	};
	ਅਚਲ ਬਣਤਰ submodule *sub;

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, usage, 0);

	ਜੇ (argc != 2 || !(ਰਸਤਾ = argv[0]) || !(newurl = argv[1]))
		usage_with_options(usage, options);

	sub = submodule_from_ਰਸਤਾ(the_repository, null_oid(the_hash_algo), ਰਸਤਾ);

	ਜੇ (!sub)
		die(_("no submodule mapping found in .gitmodules for path '%s'"),
		    ਰਸਤਾ);

	config_name = xstrfmt("submodule.%s.url", sub->name);
	ret = config_ਸੈੱਟ_ਵਿੱਚ_gitmodules_file_gently(the_repository, config_name, newurl);

	ਜੇ (!ret) {
		repo_read_gitmodules(the_repository, 0);
		sync_submodule(sub->ਰਸਤਾ, ਅਗੇਤਰ, ਨਿਰਾ, quiet ? OPT_QUIET : 0);
	}

	ਮੁਕਤ(config_name);
	ਵਾਪਸ !!ret;
}

ਸਥਿਰ ਪੂਰਨ module_ਸੈੱਟ_branch(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			     ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ opt_default = 0, ret;
	ਅਚਲ ਅੱਖਰ *opt_branch = ਨਿਰਾ;
	ਅਚਲ ਅੱਖਰ *ਰਸਤਾ;
	ਅੱਖਰ *config_name;
	ਬਣਤਰ option options[] = {
		/*
		 * We accept the `quiet` option for uniformity across subcommands,
		 * though there is nothing to make less verbose in this subcommand.
		 */
		OPT_NOOP_NOARG('q', "quiet"),

		OPT_BOOL('d', "default", &opt_default,
			N_("set the default tracking branch to master")),
		OPT_STRING('b', "branch", &opt_branch, N_("branch"),
			N_("set the default tracking branch")),
		OPT_ਸਿਰਾ()
	};
	ਅਚਲ ਅੱਖਰ *ਅਚਲ usage[] = {
		N_("git submodule set-branch [-q|--quiet] (-d|--default) <path>"),
		N_("git submodule set-branch [-q|--quiet] (-b|--branch) <branch> <path>"),
		ਨਿਰਾ
	};
	ਅਚਲ ਬਣਤਰ submodule *sub;

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, usage, 0);

	ਜੇ (!opt_branch && !opt_default)
		die(_("--branch or --default required"));

	ਜੇ (opt_branch && opt_default)
		die(_("options '%s' and '%s' cannot be used together"), "--branch", "--default");

	ਜੇ (argc != 1 || !(ਰਸਤਾ = argv[0]))
		usage_with_options(usage, options);

	sub = submodule_from_ਰਸਤਾ(the_repository, null_oid(the_hash_algo), ਰਸਤਾ);

	ਜੇ (!sub)
		die(_("no submodule mapping found in .gitmodules for path '%s'"),
		    ਰਸਤਾ);

	config_name = xstrfmt("submodule.%s.branch", sub->name);
	ret = config_ਸੈੱਟ_ਵਿੱਚ_gitmodules_file_gently(the_repository, config_name, opt_branch);

	ਮੁਕਤ(config_name);
	ਵਾਪਸ !!ret;
}

ਸਥਿਰ ਪੂਰਨ module_create_branch(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
				ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਗਿਣਤੀ branch_track track;
	ਪੂਰਨ quiet = 0, force = 0, reflog = 0, dry_ਲੜੀਵਾਰ = 0;
	ਬਣਤਰ option options[] = {
		OPT__QUIET(&quiet, N_("print only error messages")),
		OPT__FORCE(&force, N_("force creation"), 0),
		OPT_BOOL(0, "create-reflog", &reflog,
			 N_("create the branch's reflog")),
		OPT_CALLBACK_F('t', "track",  &track, "(direct|inherit)",
			N_("set branch tracking configuration"),
			PARSE_OPT_OPTARG,
			parse_opt_tracking_ਬਹੁਲਕ),
		OPT_DRY_ਲੜੀਵਾਰ(&dry_ਲੜੀਵਾਰ,
			     N_("show whether the branch would be created")),
		OPT_ਸਿਰਾ()
	};
	ਅਚਲ ਅੱਖਰ *ਅਚਲ usage[] = {
		N_("git submodule--helper create-branch [-f|--force] [--create-reflog] [-q|--quiet] [-t|--track] [-n|--dry-run] <name> <start-oid> <start-name>"),
		ਨਿਰਾ
	};
	ਬਣਤਰ repo_config_ਮੁੱਲਾਂ *cfg = repo_config_ਮੁੱਲਾਂ(the_repository);

	repo_config(the_repository, git_default_config, ਨਿਰਾ);
	track = cfg->branch_track;
	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, usage, 0);

	ਜੇ (argc != 3)
		usage_with_options(usage, options);

	ਜੇ (!quiet && !dry_ਲੜੀਵਾਰ)
		printf_ln(_("creating branch '%s'"), argv[0]);

	create_branches_recursively(the_repository, argv[0], argv[1], argv[2],
				    force, reflog, quiet, track, dry_ਲੜੀਵਾਰ);
	ਵਾਪਸ 0;
}

ਬਣਤਰ ਜੋੜੋ_ਅੰਕੜੇ {
	ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ;
	ਅਚਲ ਅੱਖਰ *branch;
	ਅਚਲ ਅੱਖਰ *reference_ਰਸਤਾ;
	ਅੱਖਰ *sm_ਰਸਤਾ;
	ਅਚਲ ਅੱਖਰ *sm_name;
	ਅਚਲ ਅੱਖਰ *repo;
	ਅਚਲ ਅੱਖਰ *realrepo;
	ਗਿਣਤੀ ref_storage_format ref_storage_format;
	ਪੂਰਨ ਡੂੰਘਾਈ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ force: 1;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ quiet: 1;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਤਰੱਕੀ: 1;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ dissociate: 1;
};
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਜੋੜੋ_ਅੰਕੜੇ_INIT { \
	.ਡੂੰਘਾਈ = -1, \
	.ref_storage_format = REF_STORAGE_FORMAT_UNKNOWN, \
}

ਸਥਿਰ ਖਾਲੀ append_fetch_remotes(ਬਣਤਰ strbuf *ਸੁਨੇਹਾ, ਅਚਲ ਅੱਖਰ *git_dir_ਰਸਤਾ)
{
	ਬਣਤਰ ਬੱਚਾ_process cp_remote = ਬੱਚਾ_PROCESS_INIT;
	ਬਣਤਰ strbuf sb_remote_ਬਾਹਰਲਾ = STRBUF_INIT;

	cp_remote.git_cmd = 1;
	strvec_pushf(&cp_remote.env,
		     "GIT_DIR=%s", git_dir_ਰਸਤਾ);
	strvec_ਧੱਕੋ(&cp_remote.env, "GIT_WORK_TREE=.");
	strvec_pushl(&cp_remote.ਦਲੀਲਾਂ, "remote", "-v", ਨਿਰਾ);
	ਜੇ (!capture_command(&cp_remote, &sb_remote_ਬਾਹਰਲਾ, 0)) {
		ਅੱਖਰ *ਅਗਲਾ_ਸਤਰ;
		ਅੱਖਰ *ਸਤਰ = sb_remote_ਬਾਹਰਲਾ.buf;

		ਜਦੋਂ ((ਅਗਲਾ_ਸਤਰ = strchr(ਸਤਰ, '\n')) != ਨਿਰਾ) {
			ਮਾਪ_t len = ਅਗਲਾ_ਸਤਰ - ਸਤਰ;

			ਜੇ (strip_suffix_mem(ਸਤਰ, &len, " (fetch)"))
				strbuf_addf(ਸੁਨੇਹਾ, "  %.*s\n", (ਪੂਰਨ)len, ਸਤਰ);
			ਸਤਰ = ਅਗਲਾ_ਸਤਰ + 1;
		}
	}

	strbuf_release(&sb_remote_ਬਾਹਰਲਾ);
}

ਸਥਿਰ ਪੂਰਨ ਜੋੜੋ_submodule(ਅਚਲ ਬਣਤਰ ਜੋੜੋ_ਅੰਕੜੇ *ਜੋੜੋ_ਅੰਕੜੇ)
{
	ਬਣਤਰ module_clone_ਅੰਕੜੇ clone_ਅੰਕੜੇ = MODULE_CLONE_ਅੰਕੜੇ_INIT;
	ਬਣਤਰ string_list reference = STRING_LIST_INIT_NODUP;
	ਪੂਰਨ ret = -1;

	/* perhaps the path already exists and is already a git repo, else clone it */
	ਜੇ (is_directory(ਜੋੜੋ_ਅੰਕੜੇ->sm_ਰਸਤਾ)) {
		ਅੱਖਰ *submod_gitdir_ਰਸਤਾ;
		ਬਣਤਰ strbuf sm_ਰਸਤਾ = STRBUF_INIT;
		strbuf_addstr(&sm_ਰਸਤਾ, ਜੋੜੋ_ਅੰਕੜੇ->sm_ਰਸਤਾ);
		submod_gitdir_ਰਸਤਾ = xstrfmt("%s/.git", ਜੋੜੋ_ਅੰਕੜੇ->sm_ਰਸਤਾ);
		ਜੇ (is_nonbare_repository_dir(&sm_ਰਸਤਾ))
			ਲਿਖੋ(_("Adding existing repo at '%s' to the index\n"),
			       ਜੋੜੋ_ਅੰਕੜੇ->sm_ਰਸਤਾ);
		ਨਹੀਂਤਾਂ
			die(_("'%s' already exists and is not a valid git repo"),
			    ਜੋੜੋ_ਅੰਕੜੇ->sm_ਰਸਤਾ);
		strbuf_release(&sm_ਰਸਤਾ);
		ਮੁਕਤ(submod_gitdir_ਰਸਤਾ);
	} ਨਹੀਂਤਾਂ {
		ਬਣਤਰ ਬੱਚਾ_process cp = ਬੱਚਾ_PROCESS_INIT;
		ਬਣਤਰ strbuf submod_gitdir = STRBUF_INIT;

		submodule_name_ਮੰਜ਼ਿਲ_gitdir(&submod_gitdir, the_repository, ਜੋੜੋ_ਅੰਕੜੇ->sm_name);

		ਜੇ (is_directory(submod_gitdir.buf)) {
			ਜੇ (!ਜੋੜੋ_ਅੰਕੜੇ->force) {
				ਬਣਤਰ strbuf ਸੁਨੇਹਾ = STRBUF_INIT;
				ਅੱਖਰ *die_ਸੁਨੇਹਾ_2;

				strbuf_addf(&ਸੁਨੇਹਾ, _("A git directory for '%s' is found "
						    "locally with remote(s):\n"),
					    ਜੋੜੋ_ਅੰਕੜੇ->sm_name);

				append_fetch_remotes(&ਸੁਨੇਹਾ, submod_gitdir.buf);
				strbuf_release(&submod_gitdir);

				strbuf_addf(&ਸੁਨੇਹਾ, _("If you want to reuse this local git "
						    "directory instead of cloning again from\n"
						    "  %s\n"
						    "use the '--force' option. If the local git "
						    "directory is not the correct repo\n"
						    "or you are unsure what this means choose "
						    "another name with the '--name' option."),
					    ਜੋੜੋ_ਅੰਕੜੇ->realrepo);

				die_ਸੁਨੇਹਾ_2 = strbuf_detach(&ਸੁਨੇਹਾ, ਨਿਰਾ);
				die("%s", die_ਸੁਨੇਹਾ_2);
			} ਨਹੀਂਤਾਂ {
				ਲਿਖੋ(_("Reactivating local git directory for "
					 "submodule '%s'\n"), ਜੋੜੋ_ਅੰਕੜੇ->sm_name);
			}
		}
		strbuf_release(&submod_gitdir);

		clone_ਅੰਕੜੇ.ਅਗੇਤਰ = ਜੋੜੋ_ਅੰਕੜੇ->ਅਗੇਤਰ;
		clone_ਅੰਕੜੇ.ਰਸਤਾ = ਜੋੜੋ_ਅੰਕੜੇ->sm_ਰਸਤਾ;
		clone_ਅੰਕੜੇ.name = ਜੋੜੋ_ਅੰਕੜੇ->sm_name;
		clone_ਅੰਕੜੇ.url = ਜੋੜੋ_ਅੰਕੜੇ->realrepo;
		clone_ਅੰਕੜੇ.quiet = ਜੋੜੋ_ਅੰਕੜੇ->quiet;
		clone_ਅੰਕੜੇ.ਤਰੱਕੀ = ਜੋੜੋ_ਅੰਕੜੇ->ਤਰੱਕੀ;
		ਜੇ (ਜੋੜੋ_ਅੰਕੜੇ->reference_ਰਸਤਾ) {
			ਅੱਖਰ *p = xstrdup(ਜੋੜੋ_ਅੰਕੜੇ->reference_ਰਸਤਾ);

			string_list_append(&reference, p)->util = p;
		}
		clone_ਅੰਕੜੇ.ref_storage_format = ਜੋੜੋ_ਅੰਕੜੇ->ref_storage_format;
		clone_ਅੰਕੜੇ.dissociate = ਜੋੜੋ_ਅੰਕੜੇ->dissociate;
		ਜੇ (ਜੋੜੋ_ਅੰਕੜੇ->ਡੂੰਘਾਈ >= 0)
			clone_ਅੰਕੜੇ.ਡੂੰਘਾਈ = ਜੋੜੋ_ਅੰਕੜੇ->ਡੂੰਘਾਈ;

		ਜੇ (clone_submodule(&clone_ਅੰਕੜੇ, &reference))
			ਜਾਓ cleanup;

		prepare_submodule_repo_env(&cp.env);
		cp.git_cmd = 1;
		cp.dir = ਜੋੜੋ_ਅੰਕੜੇ->sm_ਰਸਤਾ;
		/*
		 * NOTE: we only get here if add_data->force is true, so
		 * passing --force to checkout is reasonable.
		 */
		strvec_pushl(&cp.ਦਲੀਲਾਂ, "checkout", "-f", "-q", ਨਿਰਾ);

		ਜੇ (ਜੋੜੋ_ਅੰਕੜੇ->branch) {
			strvec_pushl(&cp.ਦਲੀਲਾਂ, "-B", ਜੋੜੋ_ਅੰਕੜੇ->branch, ਨਿਰਾ);
			strvec_pushf(&cp.ਦਲੀਲਾਂ, "origin/%s", ਜੋੜੋ_ਅੰਕੜੇ->branch);
		}

		ਜੇ (ਲੜੀਵਾਰ_command(&cp))
			die(_("unable to checkout submodule '%s'"), ਜੋੜੋ_ਅੰਕੜੇ->sm_ਰਸਤਾ);
	}
	ret = 0;

cleanup:
	string_list_clear(&reference, 1);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ config_submodule_ਵਿੱਚ_gitmodules(ਅਚਲ ਅੱਖਰ *name, ਅਚਲ ਅੱਖਰ *var, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ)
{
	ਅੱਖਰ *ਕੁੰਜੀ;
	ਪੂਰਨ ret;

	ਜੇ (!is_writing_gitmodules_ok())
		die(_("please make sure that the .gitmodules file is in the working tree"));

	ਕੁੰਜੀ = xstrfmt("submodule.%s.%s", name, var);
	ret = config_ਸੈੱਟ_ਵਿੱਚ_gitmodules_file_gently(the_repository, ਕੁੰਜੀ, ਮੁੱਲ);
	ਮੁਕਤ(ਕੁੰਜੀ);

	ਵਾਪਸ ret;
}

ਸਥਿਰ ਖਾਲੀ configure_added_submodule(ਬਣਤਰ ਜੋੜੋ_ਅੰਕੜੇ *ਜੋੜੋ_ਅੰਕੜੇ)
{
	ਅੱਖਰ *ਕੁੰਜੀ;
	ਬਣਤਰ ਬੱਚਾ_process ਜੋੜੋ_submod = ਬੱਚਾ_PROCESS_INIT;
	ਬਣਤਰ ਬੱਚਾ_process ਜੋੜੋ_gitmodules = ਬੱਚਾ_PROCESS_INIT;
	ਅਚਲ ਬਣਤਰ string_list *ਮੁੱਲਾਂ;
	ਪੂਰਨ matched = 0;

	ਕੁੰਜੀ = xstrfmt("submodule.%s.url", ਜੋੜੋ_ਅੰਕੜੇ->sm_name);
	repo_config_ਸੈੱਟ_gently(the_repository, ਕੁੰਜੀ, ਜੋੜੋ_ਅੰਕੜੇ->realrepo);
	ਮੁਕਤ(ਕੁੰਜੀ);

	ਜੋੜੋ_submod.git_cmd = 1;
	strvec_pushl(&ਜੋੜੋ_submod.ਦਲੀਲਾਂ, "add",
		     "--no-warn-embedded-repo", ਨਿਰਾ);
	ਜੇ (ਜੋੜੋ_ਅੰਕੜੇ->force)
		strvec_ਧੱਕੋ(&ਜੋੜੋ_submod.ਦਲੀਲਾਂ, "--force");
	strvec_pushl(&ਜੋੜੋ_submod.ਦਲੀਲਾਂ, "--", ਜੋੜੋ_ਅੰਕੜੇ->sm_ਰਸਤਾ, ਨਿਰਾ);

	ਜੇ (ਲੜੀਵਾਰ_command(&ਜੋੜੋ_submod))
		die(_("Failed to add submodule '%s'"), ਜੋੜੋ_ਅੰਕੜੇ->sm_ਰਸਤਾ);

	ਜੇ (config_submodule_ਵਿੱਚ_gitmodules(ਜੋੜੋ_ਅੰਕੜੇ->sm_name, "path", ਜੋੜੋ_ਅੰਕੜੇ->sm_ਰਸਤਾ) ||
	    config_submodule_ਵਿੱਚ_gitmodules(ਜੋੜੋ_ਅੰਕੜੇ->sm_name, "url", ਜੋੜੋ_ਅੰਕੜੇ->repo))
		die(_("Failed to register submodule '%s'"), ਜੋੜੋ_ਅੰਕੜੇ->sm_ਰਸਤਾ);

	ਜੇ (ਜੋੜੋ_ਅੰਕੜੇ->branch) {
		ਜੇ (config_submodule_ਵਿੱਚ_gitmodules(ਜੋੜੋ_ਅੰਕੜੇ->sm_name,
						   "branch", ਜੋੜੋ_ਅੰਕੜੇ->branch))
			die(_("Failed to register submodule '%s'"), ਜੋੜੋ_ਅੰਕੜੇ->sm_ਰਸਤਾ);
	}

	ਜੋੜੋ_gitmodules.git_cmd = 1;
	strvec_pushl(&ਜੋੜੋ_gitmodules.ਦਲੀਲਾਂ,
		     "add", "--force", "--", ".gitmodules", ਨਿਰਾ);

	ਜੇ (ਲੜੀਵਾਰ_command(&ਜੋੜੋ_gitmodules))
		die(_("Failed to register submodule '%s'"), ਜੋੜੋ_ਅੰਕੜੇ->sm_ਰਸਤਾ);

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
	ਜੇ (repo_config_get(the_repository, "submodule.active") || /* key absent */
	    repo_config_get_string_multi(the_repository, "submodule.active", &ਮੁੱਲਾਂ)) {
		/*
		 * If the submodule being added isn't already covered by the
		 * current configured pathspec, set the submodule's active flag
		 */
		ਕੁੰਜੀ = xstrfmt("submodule.%s.active", ਜੋੜੋ_ਅੰਕੜੇ->sm_name);
		repo_config_ਸੈੱਟ_gently(the_repository, ਕੁੰਜੀ, "true");
		ਮੁਕਤ(ਕੁੰਜੀ);
	} ਨਹੀਂਤਾਂ {
		ਲਈ (ਮਾਪ_t i = 0; i < ਮੁੱਲਾਂ->nr; i++) {
			ਅਚਲ ਅੱਖਰ *pat = ਮੁੱਲਾਂ->ਚੀਜ਼ਾਂ[i].string;
			ਜੇ (!wildmatch(pat, ਜੋੜੋ_ਅੰਕੜੇ->sm_ਰਸਤਾ, 0)) { /* match found */
				matched = 1;
				ਰੋਕੋ;
			}
		}
		ਜੇ (!matched) { /* no pattern matched -> force-enable */
			ਕੁੰਜੀ = xstrfmt("submodule.%s.active", ਜੋੜੋ_ਅੰਕੜੇ->sm_name);
			repo_config_ਸੈੱਟ_gently(the_repository, ਕੁੰਜੀ, "true");
			ਮੁਕਤ(ਕੁੰਜੀ);
		}
	}
}

ਸਥਿਰ ਖਾਲੀ die_on_ਸੂਚਕਾਂਕ_ਮੇਲ(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਪੂਰਨ force)
{
	ਬਣਤਰ pathspec ps;
	ਅਚਲ ਅੱਖਰ *ਦਲੀਲਾਂ[] = { ਰਸਤਾ, ਨਿਰਾ };
	parse_pathspec(&ps, 0, PATHSPEC_PREFER_CWD, ਨਿਰਾ, ਦਲੀਲਾਂ);

	ਜੇ (repo_read_ਸੂਚਕਾਂਕ_preload(the_repository, ਨਿਰਾ, 0) < 0)
		die(_("index file corrupt"));

	ਜੇ (ps.nr) {
		ਅੱਖਰ *ps_matched = xcalloc(ps.nr, 1);

		/* TODO: audit for interaction with sparse-index. */
		ensure_ਭਰਿਆ_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ);

		/*
		 * Since there is only one pathspec, we just need to
		 * check ps_matched[0] to know if a cache entry matched.
		 */
		ਲਈ (ਮਾਪ_t i = 0; i < the_repository->ਸੂਚਕਾਂਕ->cache_nr; i++) {
			ce_ਰਸਤਾ_ਮੇਲ(the_repository->ਸੂਚਕਾਂਕ, the_repository->ਸੂਚਕਾਂਕ->cache[i], &ps,
				      ps_matched);

			ਜੇ (ps_matched[0]) {
				ਜੇ (!force)
					die(_("'%s' already exists in the index"),
					    ਰਸਤਾ);
				ਜੇ (!S_ISGITLINK(the_repository->ਸੂਚਕਾਂਕ->cache[i]->ce_ਬਹੁਲਕ))
					die(_("'%s' already exists in the index "
					      "and is not a submodule"), ਰਸਤਾ);
				ਰੋਕੋ;
			}
		}
		ਮੁਕਤ(ps_matched);
	}
	clear_pathspec(&ps);
}

ਸਥਿਰ ਖਾਲੀ die_on_repo_without_commits(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ)
{
	ਬਣਤਰ strbuf sb = STRBUF_INIT;
	strbuf_addstr(&sb, ਰਸਤਾ);
	ਜੇ (is_nonbare_repository_dir(&sb)) {
		ਬਣਤਰ object_id oid;
		ਜੇ (repo_resolve_gitlink_ref(the_repository, ਰਸਤਾ, "HEAD", &oid) < 0)
			die(_("'%s' does not have a commit checked out"), ਰਸਤਾ);
	}
	strbuf_release(&sb);
}

ਸਥਿਰ ਪੂਰਨ module_ਜੋੜੋ(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		      ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ force = 0, quiet = 0, ਤਰੱਕੀ = 0, dissociate = 0;
	ਬਣਤਰ ਜੋੜੋ_ਅੰਕੜੇ ਜੋੜੋ_ਅੰਕੜੇ = ਜੋੜੋ_ਅੰਕੜੇ_INIT;
	ਅਚਲ ਅੱਖਰ *ref_storage_format = ਨਿਰਾ;
	ਅੱਖਰ *ਮੰਜ਼ਿਲ_free = ਨਿਰਾ;
	ਅਚਲ ਬਣਤਰ submodule *existing;
	ਬਣਤਰ strbuf buf = STRBUF_INIT;
	ਅੱਖਰ *sm_name_ਮੰਜ਼ਿਲ_free = ਨਿਰਾ;
	ਬਣਤਰ option options[] = {
		OPT_STRING('b', "branch", &ਜੋੜੋ_ਅੰਕੜੇ.branch, N_("branch"),
			   N_("branch of repository to add as submodule")),
		OPT__FORCE(&force, N_("allow adding an otherwise ignored submodule path"),
			   PARSE_OPT_NOCOMPLETE),
		OPT__QUIET(&quiet, N_("print only error messages")),
		OPT_BOOL(0, "progress", &ਤਰੱਕੀ, N_("force cloning progress")),
		OPT_STRING(0, "reference", &ਜੋੜੋ_ਅੰਕੜੇ.reference_ਰਸਤਾ, N_("repository"),
			   N_("reference repository")),
		OPT_STRING(0, "ref-storage-format", &ref_storage_format, N_("format"),
			   N_("specify the reference storage format to use")),
		OPT_ALIAS_F(0, "ref-format", "ref-storage-format", PARSE_OPT_HIDDEN),
		OPT_BOOL(0, "dissociate", &dissociate, N_("borrow the objects from reference repositories")),
		OPT_STRING(0, "name", &ਜੋੜੋ_ਅੰਕੜੇ.sm_name, N_("name"),
			   N_("sets the submodule's name to the given string "
			      "instead of defaulting to its path")),
		OPT_INTEGER(0, "depth", &ਜੋੜੋ_ਅੰਕੜੇ.ਡੂੰਘਾਈ, N_("depth for shallow clones")),
		OPT_ਸਿਰਾ()
	};
	ਅਚਲ ਅੱਖਰ *ਅਚਲ usage[] = {
		N_("git submodule add [<options>] [--] <repository> [<path>]"),
		ਨਿਰਾ
	};
	ਬਣਤਰ strbuf sb = STRBUF_INIT;
	ਪੂਰਨ ret = 1;

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, usage, 0);

	ਜੇ (!is_writing_gitmodules_ok())
		die(_("please make sure that the .gitmodules file is in the working tree"));

	ਜੇ (ਅਗੇਤਰ && *ਅਗੇਤਰ &&
	    ਜੋੜੋ_ਅੰਕੜੇ.reference_ਰਸਤਾ && !is_absolute_ਰਸਤਾ(ਜੋੜੋ_ਅੰਕੜੇ.reference_ਰਸਤਾ))
		ਜੋੜੋ_ਅੰਕੜੇ.reference_ਰਸਤਾ = xstrfmt("%s%s", ਅਗੇਤਰ, ਜੋੜੋ_ਅੰਕੜੇ.reference_ਰਸਤਾ);

	ਜੇ (argc == 0 || argc > 2)
		usage_with_options(usage, options);

	ਜੇ (ref_storage_format) {
		ਜੋੜੋ_ਅੰਕੜੇ.ref_storage_format = ref_storage_format_by_name(ref_storage_format);
		ਜੇ (ਜੋੜੋ_ਅੰਕੜੇ.ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format);
	}

	ਜੋੜੋ_ਅੰਕੜੇ.repo = argv[0];
	ਜੇ (argc == 1)
		ਜੋੜੋ_ਅੰਕੜੇ.sm_ਰਸਤਾ = git_url_basename(ਜੋੜੋ_ਅੰਕੜੇ.repo, 0, 0);
	ਨਹੀਂਤਾਂ
		ਜੋੜੋ_ਅੰਕੜੇ.sm_ਰਸਤਾ = xstrdup(argv[1]);

	ਜੇ (ਅਗੇਤਰ && *ਅਗੇਤਰ && !is_absolute_ਰਸਤਾ(ਜੋੜੋ_ਅੰਕੜੇ.sm_ਰਸਤਾ)) {
		ਅੱਖਰ *sm_ਰਸਤਾ = ਜੋੜੋ_ਅੰਕੜੇ.sm_ਰਸਤਾ;

		ਜੋੜੋ_ਅੰਕੜੇ.sm_ਰਸਤਾ = xstrfmt("%s%s", ਅਗੇਤਰ, sm_ਰਸਤਾ);
		ਮੁਕਤ(sm_ਰਸਤਾ);
	}

	ਜੇ (starts_with_dot_dot_slash(ਜੋੜੋ_ਅੰਕੜੇ.repo) ||
	    starts_with_dot_slash(ਜੋੜੋ_ਅੰਕੜੇ.repo)) {
		ਜੇ (ਅਗੇਤਰ)
			die(_("Relative path can only be used from the toplevel "
			      "of the working tree"));

		/* dereference source url relative to parent's url */
		ਮੰਜ਼ਿਲ_free = resolve_relative_url(ਜੋੜੋ_ਅੰਕੜੇ.repo, ਨਿਰਾ, 1);
		ਜੋੜੋ_ਅੰਕੜੇ.realrepo = ਮੰਜ਼ਿਲ_free;
	} ਨਹੀਂਤਾਂ ਜੇ (is_dir_sep(ਜੋੜੋ_ਅੰਕੜੇ.repo[0]) || strchr(ਜੋੜੋ_ਅੰਕੜੇ.repo, ':')) {
		ਜੋੜੋ_ਅੰਕੜੇ.realrepo = ਜੋੜੋ_ਅੰਕੜੇ.repo;
	} ਨਹੀਂਤਾਂ {
		die(_("repo URL: '%s' must be absolute or begin with ./|../"),
		    ਜੋੜੋ_ਅੰਕੜੇ.repo);
	}

	/*
	 * normalize path:
	 * multiple //; leading ./; /./; /../;
	 */
	normalize_ਰਸਤਾ_copy(ਜੋੜੋ_ਅੰਕੜੇ.sm_ਰਸਤਾ, ਜੋੜੋ_ਅੰਕੜੇ.sm_ਰਸਤਾ);
	strip_dir_trailing_slashes(ਜੋੜੋ_ਅੰਕੜੇ.sm_ਰਸਤਾ);

	ਜੇ (validate_submodule_ਰਸਤਾ(ਜੋੜੋ_ਅੰਕੜੇ.sm_ਰਸਤਾ) < 0)
		die(ਨਿਰਾ);

	die_on_ਸੂਚਕਾਂਕ_ਮੇਲ(ਜੋੜੋ_ਅੰਕੜੇ.sm_ਰਸਤਾ, force);
	die_on_repo_without_commits(ਜੋੜੋ_ਅੰਕੜੇ.sm_ਰਸਤਾ);

	ਜੇ (!force) {
		ਬਣਤਰ ਬੱਚਾ_process cp = ਬੱਚਾ_PROCESS_INIT;

		cp.git_cmd = 1;
		cp.no_stdout = 1;
		strvec_pushl(&cp.ਦਲੀਲਾਂ, "add", "--dry-run", "--ignore-missing",
			     "--no-warn-embedded-repo", ਜੋੜੋ_ਅੰਕੜੇ.sm_ਰਸਤਾ, ਨਿਰਾ);
		ਜੇ ((ret = pipe_command(&cp, ਨਿਰਾ, 0, ਨਿਰਾ, 0, &sb, 0))) {
			strbuf_complete_ਸਤਰ(&sb);
			fputs(sb.buf, stderr);
			ਜਾਓ cleanup;
		}
	}

	ਜੇ (!ਜੋੜੋ_ਅੰਕੜੇ.sm_name)
		ਜੋੜੋ_ਅੰਕੜੇ.sm_name = ਜੋੜੋ_ਅੰਕੜੇ.sm_ਰਸਤਾ;

	existing = submodule_from_name(the_repository,
					null_oid(the_hash_algo),
					ਜੋੜੋ_ਅੰਕੜੇ.sm_name);

	ਜੇ (existing && existing->ਰਸਤਾ &&
	    strcmp(existing->ਰਸਤਾ, ਜੋੜੋ_ਅੰਕੜੇ.sm_ਰਸਤਾ)) {
		ਜੇ (!force) {
			die(_("submodule name '%s' already used for path '%s'"),
			    ਜੋੜੋ_ਅੰਕੜੇ.sm_name, existing->ਰਸਤਾ);
		}
		/* --force: build <name><n> until unique */
		ਲਈ (ਪੂਰਨ i = 1; ; i++) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "%s%d", ਜੋੜੋ_ਅੰਕੜੇ.sm_name, i);
			ਜੇ (!submodule_from_name(the_repository,
						null_oid(the_hash_algo),
						buf.buf)) {
				ਰੋਕੋ;
			}
		}
		ਜੋੜੋ_ਅੰਕੜੇ.sm_name = sm_name_ਮੰਜ਼ਿਲ_free = strbuf_detach(&buf, ਨਿਰਾ);
	}

	ਜੇ (check_submodule_name(ਜੋੜੋ_ਅੰਕੜੇ.sm_name))
		die(_("'%s' is not a valid submodule name"), ਜੋੜੋ_ਅੰਕੜੇ.sm_name);

	ਜੋੜੋ_ਅੰਕੜੇ.ਅਗੇਤਰ = ਅਗੇਤਰ;
	ਜੋੜੋ_ਅੰਕੜੇ.force = !!force;
	ਜੋੜੋ_ਅੰਕੜੇ.quiet = !!quiet;
	ਜੋੜੋ_ਅੰਕੜੇ.ਤਰੱਕੀ = !!ਤਰੱਕੀ;
	ਜੋੜੋ_ਅੰਕੜੇ.dissociate = !!dissociate;

	ਜੇ (the_repository->repository_format_submodule_ਰਸਤਾ_cfg)
		create_default_gitdir_config(ਜੋੜੋ_ਅੰਕੜੇ.sm_name);

	ਜੇ (ਜੋੜੋ_submodule(&ਜੋੜੋ_ਅੰਕੜੇ))
		ਜਾਓ cleanup;
	configure_added_submodule(&ਜੋੜੋ_ਅੰਕੜੇ);

	ret = 0;
cleanup:
	ਮੁਕਤ(sm_name_ਮੰਜ਼ਿਲ_free);
	ਮੁਕਤ(ਜੋੜੋ_ਅੰਕੜੇ.sm_ਰਸਤਾ);
	ਮੁਕਤ(ਮੰਜ਼ਿਲ_free);
	strbuf_release(&sb);

	ਵਾਪਸ ret;
}

ਪੂਰਨ cmd_submodule__helper(ਪੂਰਨ argc,
			  ਅਚਲ ਅੱਖਰ **argv,
			  ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			  ਬਣਤਰ repository *repo)
{
	parse_opt_subcommand_fn *fn = ਨਿਰਾ;
	ਅਚਲ ਅੱਖਰ *ਅਚਲ usage[] = {
		N_("git submodule--helper <command>"),
		ਨਿਰਾ
	};
	ਬਣਤਰ option options[] = {
		OPT_SUBCOMMAND("migrate-gitdir-configs", &fn, module_migrate),
		OPT_SUBCOMMAND("gitdir", &fn, module_gitdir),
		OPT_SUBCOMMAND("clone", &fn, module_clone),
		OPT_SUBCOMMAND("add", &fn, module_ਜੋੜੋ),
		OPT_SUBCOMMAND("update", &fn, module_update),
		OPT_SUBCOMMAND("foreach", &fn, module_foreach),
		OPT_SUBCOMMAND("init", &fn, module_init),
		OPT_SUBCOMMAND("status", &fn, module_ਸਥਿਤੀ),
		OPT_SUBCOMMAND("sync", &fn, module_sync),
		OPT_SUBCOMMAND("deinit", &fn, module_deinit),
		OPT_SUBCOMMAND("summary", &fn, module_summary),
		OPT_SUBCOMMAND("push-check", &fn, ਧੱਕੋ_check),
		OPT_SUBCOMMAND("absorbgitdirs", &fn, absorb_git_dirs),
		OPT_SUBCOMMAND("set-url", &fn, module_ਸੈੱਟ_url),
		OPT_SUBCOMMAND("set-branch", &fn, module_ਸੈੱਟ_branch),
		OPT_SUBCOMMAND("create-branch", &fn, module_create_branch),
		OPT_SUBCOMMAND("get-default-remote", &fn, module_get_default_remote),
		OPT_ਸਿਰਾ()
	};
	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, usage, 0);

	ਵਾਪਸ fn(argc, argv, ਅਗੇਤਰ, repo);
}
